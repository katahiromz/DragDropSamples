// Sample2.cpp
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.

#include <windows.h>
#include <windowsx.h>
#include <olectl.h>
#include "CDropTarget.hpp"

BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    if (IDropTarget *pDropTarget = CDropTarget::CreateInstance())
    {
        ::RegisterDragDrop(hwnd, pDropTarget);
        pDropTarget->Release();
    }
    return TRUE;
}

void OnClose(HWND hwnd)
{
    RevokeDragDrop(hwnd);
    EndDialog(hwnd, IDCLOSE);
}

INT_PTR CALLBACK
DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
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
    OleInitialize(NULL);

    DialogBox(hInstance, MAKEINTRESOURCE(1), NULL, DialogProc);

    OleUninitialize();

    return 0;
}
