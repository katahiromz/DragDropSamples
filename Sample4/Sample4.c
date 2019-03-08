// Sample2.cpp
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.

#define CINTERFACE
#define COBJMACROS
#include <windows.h>
#include <windowsx.h>
#include <olectl.h>
#include "CDropTarget.h"

BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    IDropTarget *pDropTarget = CDropTarget_CreateInstance(hwnd);
    if (pDropTarget)
    {
        RegisterDragDrop(hwnd, pDropTarget);
        IUnknown_Release(pDropTarget);
    }
    return TRUE;
}

void OnClose(HWND hwnd)
{
    RevokeDragDrop(hwnd);
    EndDialog(hwnd, IDCLOSE);
}

void OnDropFiles(HWND hwnd, HDROP hdrop)
{
    TCHAR szText[MAX_PATH];

    DragQueryFile(hdrop, 0, szText, MAX_PATH);
    MessageBox(hwnd, szText, TEXT("Dropped"), MB_ICONINFORMATION);
    DragFinish(hdrop);
}

INT_PTR CALLBACK
DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
        HANDLE_MSG(hwnd, WM_CLOSE, OnClose);
        HANDLE_MSG(hwnd, WM_DROPFILES, OnDropFiles);
    }
    return FALSE;
}

INT WINAPI
WinMain(HINSTANCE   hInstance,
        HINSTANCE   hPrevInstance,
        LPSTR       lpCmdLine,
        INT         nCmdShow)
{
    OleInitialize(NULL);

    DialogBox(hInstance, MAKEINTRESOURCE(1), NULL, DialogProc);

    OleUninitialize();

    return 0;
}
