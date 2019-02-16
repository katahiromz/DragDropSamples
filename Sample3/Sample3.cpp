#include <windows.h>
#include <windowsx.h>
#include <olectl.h>
#include "CDropTarget.hpp"
#include "CSimpleStream.hpp"

static IPicture *s_pPicture = NULL;

BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    if (IDropTarget *pDropTarget = CDropTarget::CreateInstance(hwnd))
    {
        ::RegisterDragDrop(hwnd, pDropTarget);
        pDropTarget->Release();
    }
    return TRUE;
}

void OnDropFiles(HWND hwnd, HDROP hdrop)
{
    if (s_pPicture)
    {
        s_pPicture->Release();
        s_pPicture = NULL;
    }

    TCHAR szPath[MAX_PATH];
    ::DragQueryFile(hdrop, 0, szPath, ARRAYSIZE(szPath));

    if (IStream *pStream = CSimpleStream::CreateInstance(szPath))
    {
        HRESULT hr = ::OleLoadPicture(pStream, 0, TRUE, IID_IPicture, (void **)&s_pPicture);
        pStream->Release();

        if (!s_pPicture)
        {
            CHAR sz[64];
            wsprintfA(sz, "hr: %08lX", hr);
            MessageBoxA(hwnd, sz, "s_pPicture", MB_ICONERROR);
        }
    }


    ::DragFinish(hdrop);
    ::InvalidateRect(hwnd, NULL, TRUE);
}

void OnPaint(HWND hwnd)
{
    if (!s_pPicture)
        return;

    PAINTSTRUCT ps;
    if (HDC hDC = ::BeginPaint(hwnd, &ps))
    {
        RECT rc;
        ::GetClientRect(hwnd, &rc);

        OLE_XSIZE_HIMETRIC  cxSrc, cySrc;
        s_pPicture->get_Width(&cxSrc);
        s_pPicture->get_Height(&cySrc);

        s_pPicture->Render(hDC,
                           rc.left, rc.top,
                           rc.right - rc.left, rc.bottom - rc.top,
                           0, cySrc, cxSrc, -cySrc,
                           NULL);

        ::EndPaint(hwnd, &ps);
    }
}

void OnClose(HWND hwnd)
{
    ::RevokeDragDrop(hwnd);

    if (s_pPicture)
    {
        s_pPicture->Release();
        s_pPicture = NULL;
    }

    ::EndDialog(hwnd, IDCLOSE);
}

void OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    ::InvalidateRect(hwnd, NULL, TRUE);
}

INT_PTR CALLBACK
DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
        HANDLE_MSG(hwnd, WM_DROPFILES, OnDropFiles);
        HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
        HANDLE_MSG(hwnd, WM_SIZE, OnSize);
        HANDLE_MSG(hwnd, WM_CLOSE, OnClose);
    }
    return FALSE;
}

INT WINAPI
WinMain(HINSTANCE   hInstance,
        HINSTANCE   hPrevInstance,
        LPSTR       lpCmdLine,
        INT         nCmdShow)
{
    ::OleInitialize(NULL);

    ::DialogBox(hInstance, MAKEINTRESOURCE(1), NULL, DialogProc);

    ::OleUninitialize();

    return 0;
}
