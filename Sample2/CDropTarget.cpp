// CDropTarget.cpp
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.

#include "CDropTarget.hpp"

CDropTarget::CDropTarget() : m_nRefCount(0)
{
}

CDropTarget::~CDropTarget()
{
}

IDropTarget *CDropTarget::CreateInstance()
{
    CDropTarget *pDropTarget = new CDropTarget;
    if (pDropTarget)
    {
        pDropTarget->AddRef();
        return pDropTarget;
    }
    return NULL;
}

HRESULT STDMETHODCALLTYPE
CDropTarget::QueryInterface(REFIID riid, void **ppvObject)
{
    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDropTarget))
    {
        *ppvObject = static_cast<IDropTarget *>(this);
        AddRef();
        return S_OK;
    }

    *ppvObject = NULL;
    return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE CDropTarget::AddRef()
{
    m_nRefCount++;
    return m_nRefCount;
}

ULONG STDMETHODCALLTYPE CDropTarget::Release()
{
    m_nRefCount--;
    if (m_nRefCount > 0)
        return m_nRefCount;
    delete this;
    return 0;
}

HRESULT STDMETHODCALLTYPE
CDropTarget::DragEnter(IDataObject *pDataObject, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
    // Indicates whether or not a drop can be accepted and the drop's effect if accepted.
    ::OutputDebugString(TEXT("CDropTarget::DragEnter\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CDropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
    // Provides target feedback to the user and communicates the drop's effect to the ::DoDragDrop function.
    // grfKeyState: MK_*
    // *pdwEffect: DROPEFFECT_*
    ::OutputDebugString(TEXT("CDropTarget::DragOver\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CDropTarget::DragLeave()
{
    // Removes target feedback and releases the data object.
    ::OutputDebugString(TEXT("CDropTarget::DragLeave\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CDropTarget::Drop(IDataObject *pDataObject, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
    // Incorporates the source data into the target window, removes target feedback, and releases the data object.
    // grfKeyState: MK_*
    // *pdwEffect: DROPEFFECT_*
    ::OutputDebugString(TEXT("CDropTarget::Drop\n"));
    ::MessageBoxW(NULL, L"Dropped!", L"Dropped!", MB_ICONINFORMATION);
    return S_OK;
}
