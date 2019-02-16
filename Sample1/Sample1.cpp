// Sample1.cpp
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.

#include "CDropSource.hpp"
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <cassert>

static const TCHAR s_szName[] = TEXT("DropSrcSample");

HWND g_hWnd = NULL;
HWND g_hListView = NULL;
HIMAGELIST g_himl = NULL;

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    g_hWnd = hwnd;

    DWORD style = WS_CHILD | WS_VISIBLE | LVS_REPORT |
        LVS_AUTOARRANGE | LVS_SHOWSELALWAYS | LVS_SINGLESEL |
        WS_BORDER | WS_VSCROLL | WS_HSCROLL;
    DWORD exstyle = WS_EX_CLIENTEDGE;
    g_hListView = ::CreateWindowEx(exstyle, WC_LISTVIEW, NULL, style,
        0, 0, 0, 0, hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);

    TCHAR szName[] = TEXT("Name");
    TCHAR szFileSize[] = TEXT("File Size");
    TCHAR szCRC[] = TEXT("CRC32");

    LV_COLUMN column;
    ZeroMemory(&column, sizeof(column));
    column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    column.pszText = szName;
    column.cx = 100;
    column.iSubItem = 0;
    ListView_InsertColumn(g_hListView, 0, &column);
    column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    column.pszText = szFileSize;
    column.cx = 100;
    column.iSubItem = 1;
    ListView_InsertColumn(g_hListView, 1, &column);
    column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    column.pszText = szCRC;
    column.cx = 100;
    column.iSubItem = 2;
    ListView_InsertColumn(g_hListView, 2, &column);

    g_himl = ImageList_Create(16, 16, ILC_COLOR | ILC_MASK, 10, 10);

    TCHAR szText[64] = TEXT("TEST.TXT");

    SHFILEINFO shfi;
    ZeroMemory(&shfi, sizeof(shfi));
    SHGetFileInfo(szText, 0, &shfi, sizeof(shfi), SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES);
    assert(shfi.hIcon);

    INT iIcon = ImageList_AddIcon(g_himl, shfi.hIcon);

    LV_ITEM item;
    ZeroMemory(&item, sizeof(item));
    item.mask = LVIF_TEXT | LVIF_IMAGE;
    item.pszText = szText;
    item.iImage = iIcon;
    ListView_InsertItem(g_hListView, &item);

    ::DestroyIcon(shfi.hIcon);

    ListView_SetImageList(g_hListView, g_himl, LVSIL_SMALL);

    ::PostMessage(hwnd, WM_SIZE, 0, 0);
    return TRUE;
}

void OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    RECT rc;
    ::GetClientRect(hwnd, &rc);
    ::MoveWindow(g_hListView, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
}

void OnDestroy(HWND hwnd)
{
    ImageList_Destroy(g_himl);
    ::PostQuitMessage(0);
}

LRESULT OnNotify(HWND hwnd, int idFrom, LPNMHDR pnmhdr)
{
    NMLVDISPINFO *pDispInfo = (NMLVDISPINFO *)pnmhdr;
    NMLISTVIEW *pListView = (NMLISTVIEW *)pnmhdr;

    TCHAR szFile[] = TEXT("C:\\TEST.TXT");
    if (GetFileAttributes(szFile) == 0xFFFFFFFF)
    {
        OutputDebugString(TEXT("File C:\\TEST.TXT doesn't exists!"));
        return 0;
    }

    switch (pnmhdr->code)
    {
    case LVN_BEGINDRAG:
        if (IDataObject *pDataObject = GetFileDataObject(szFile, IID_IDataObject))
        {
            if (IDropSource *pSource = CDropSource::CreateInstance())
            {
                DWORD dwEffect;
                ::DoDragDrop(pDataObject, pSource, DROPEFFECT_COPY, &dwEffect);

                pSource->Release();
            }
            pDataObject->Release();
        }
        break;
    }
    return 0;
}

LRESULT CALLBACK
WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hwnd, WM_SIZE, OnSize);
        HANDLE_MSG(hwnd, WM_NOTIFY, OnNotify);
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

INT WINAPI
WinMain(HINSTANCE   hInstance,
        HINSTANCE   hPrevInstance,
        LPSTR       lpCmdLine,
        INT         nCmdShow)
{
    ::OleInitialize(NULL);
    ::InitCommonControls();

    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
    wc.lpszClassName = s_szName;
    if (!::RegisterClass(&wc))
    {
        ::MessageBoxA(NULL, "RegisterClass", NULL, MB_ICONERROR);
        return 1;
    }

    HWND hwnd = ::CreateWindow(s_szName, s_szName, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 200,
        NULL, NULL, hInstance, NULL);
    if (!hwnd)
    {
        ::MessageBoxA(NULL, "CreateWindow", NULL, MB_ICONERROR);
        return 2;
    }

    ::ShowWindow(hwnd, nCmdShow);
    ::UpdateWindow(hwnd);

    MSG msg;
    while (::GetMessage(&msg, NULL, 0, 0))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }

    ::OleUninitialize();
    return 0;
}
