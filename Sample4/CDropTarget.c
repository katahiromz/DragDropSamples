// CDropTarget.cpp
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.

#define CINTERFACE
#define COBJMACROS
#include "CDropTarget.h"

#ifndef STDMETHODCALLTYPE
    #define STDMETHODCALLTYPE __stdcall
#endif

typedef struct CDropTargetVtbl
{
    // IUnknown interface
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(IUnknown *This, REFIID riid, void **ppvObject);
    ULONG (STDMETHODCALLTYPE *AddRef)(IUnknown *This);
    ULONG (STDMETHODCALLTYPE *Release)(IUnknown *This);
    // IDropTarget interface
    HRESULT (STDMETHODCALLTYPE *DragEnter)(IDropTarget *This, IDataObject *pDataObject, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
    HRESULT (STDMETHODCALLTYPE *DragOver)(IDropTarget *This, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
    HRESULT (STDMETHODCALLTYPE *DragLeave)(IDropTarget *This);
    HRESULT (STDMETHODCALLTYPE *Drop)(IDropTarget *This, IDataObject *pDataObject, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
} CDropTargetVtbl;

typedef struct CDropTarget
{
    CDropTargetVtbl* lpVtbl;
    LONG m_nRefCount;
    HWND m_hwnd;
} CDropTarget;

static void CDropTarget_Free(CDropTarget *this)
{
    free(this->lpVtbl);
    free(this);
}

static HRESULT STDMETHODCALLTYPE
CDropTarget_QueryInterface(IUnknown *This, REFIID riid, void **ppvObject)
{
    if (IsEqualIID(riid, &IID_IUnknown) || IsEqualIID(riid, &IID_IDropTarget))
    {
        *ppvObject = (IDropTarget *)(This);
        IUnknown_AddRef(This);
        return S_OK;
    }

    *ppvObject = NULL;
    return E_NOINTERFACE;
}

static ULONG STDMETHODCALLTYPE CDropTarget_AddRef(IUnknown *This)
{
    CDropTarget *this = (CDropTarget *)This;
    this->m_nRefCount++;
    return this->m_nRefCount;
}

static ULONG STDMETHODCALLTYPE CDropTarget_Release(IUnknown *This)
{
    CDropTarget *this = (CDropTarget *)This;
    this->m_nRefCount--;
    if (this->m_nRefCount > 0)
        return this->m_nRefCount;

    CDropTarget_Free(this);
    return 0;
}

static HRESULT STDMETHODCALLTYPE
CDropTarget_DragEnter(IDropTarget *This, IDataObject *pDataObject, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
    return S_OK;
}

static HRESULT STDMETHODCALLTYPE
CDropTarget_DragOver(IDropTarget *This, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
    return S_OK;
}

static HRESULT STDMETHODCALLTYPE
CDropTarget_DragLeave(IDropTarget *This)
{
    return S_OK;
}

static HRESULT STDMETHODCALLTYPE
CDropTarget_Drop(IDropTarget *This, IDataObject *pDataObject, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
    FORMATETC fme;
    STGMEDIUM stgm;
    CDropTarget *this = (CDropTarget *)This;

    ZeroMemory(&fme, sizeof(fme));
    fme.cfFormat = CF_HDROP;
    fme.ptd = NULL;
    fme.dwAspect = DVASPECT_CONTENT;
    fme.lindex = -1;
    fme.tymed = TYMED_HGLOBAL;

    ZeroMemory(&stgm, sizeof(stgm));
    if (SUCCEEDED(pDataObject->lpVtbl->GetData(pDataObject, &fme, &stgm)))
    {
        SendMessage(this->m_hwnd, WM_DROPFILES, (WPARAM)stgm.hGlobal, 0);

        ReleaseStgMedium(&stgm);
    }

    return S_OK;
}

static BOOL CDropTarget_Init(CDropTarget *this, HWND hwnd)
{
    this->lpVtbl = malloc(sizeof(CDropTargetVtbl));
    if (!this->lpVtbl)
        return FALSE;

    this->lpVtbl->QueryInterface = CDropTarget_QueryInterface;
    this->lpVtbl->AddRef = CDropTarget_AddRef;
    this->lpVtbl->Release = CDropTarget_Release;

    this->lpVtbl->DragEnter = CDropTarget_DragEnter;
    this->lpVtbl->DragOver = CDropTarget_DragOver;
    this->lpVtbl->DragLeave = CDropTarget_DragLeave;
    this->lpVtbl->Drop = CDropTarget_Drop;

    this->m_nRefCount = 0;
    this->m_hwnd = hwnd;

    return TRUE;
}

IDropTarget *CDropTarget_CreateInstance(HWND hwnd)
{
    CDropTarget *This = malloc(sizeof(CDropTarget));
    if (This == NULL)
        return NULL;

    if (!CDropTarget_Init(This, hwnd))
    {
        free(This);
        return NULL;
    }

    IUnknown_AddRef((IUnknown *)This);
    return (IDropTarget *)This;
}
