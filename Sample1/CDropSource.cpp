// CDropSource.cpp
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.

#include "CDropSource.hpp"

CDropSource::CDropSource() : m_nRefCount(0)
{
}

CDropSource::~CDropSource()
{
}

IDropSource *CDropSource::CreateInstance()
{
    CDropSource *pDropSource = new CDropSource;
    pDropSource->AddRef();
    return pDropSource;
}

HRESULT STDMETHODCALLTYPE
CDropSource::QueryInterface(REFIID riid, void **ppvObject)
{
    OutputDebugString(TEXT("CDropSource::QueryInterface\n"));
    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IDropSource))
    {
        *ppvObject = static_cast<IDropSource *>(this);
        AddRef();
        return S_OK;
    }
    else
    {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }
}

ULONG STDMETHODCALLTYPE
CDropSource::AddRef()
{
    OutputDebugString(TEXT("CDropSource::AddRef\n"));
    return ++m_nRefCount;
}

ULONG STDMETHODCALLTYPE
CDropSource::Release()
{
    OutputDebugString(TEXT("CDropSource::Release\n"));
    --m_nRefCount;
    if (m_nRefCount > 0)
        return m_nRefCount;
    delete this;
    return 0;
}

HRESULT STDMETHODCALLTYPE
CDropSource::QueryContinueDrag(WINBOOL fEscapePressed, DWORD grfKeyState)
{
    OutputDebugString(TEXT("CDropSource::QueryContinueDrag\n"));

    if (fEscapePressed)
        return DRAGDROP_S_CANCEL;

    if (!(grfKeyState & (MK_LBUTTON | MK_RBUTTON)))
        return DRAGDROP_S_DROP;

    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CDropSource::GiveFeedback(DWORD dwEffect)
{
    OutputDebugString(TEXT("CDropSource::GiveFeedback\n"));
    return DRAGDROP_S_USEDEFAULTCURSORS;
}

IDataObject *GetFileDataObject(const TCHAR *pszFile, REFIID riid)
{
    IDataObject *pDataObject = NULL;
    IShellFolder *pShellFolder;
    PIDLIST_RELATIVE pidlRelative;
    PIDLIST_ABSOLUTE pidl;
    HRESULT hr;

    pidl = ILCreateFromPath(pszFile);
    if (!pidl)
        return NULL;

    hr = SHBindToParent(pidl, IID_IShellFolder, (void**)&pShellFolder, (PCUITEMID_CHILD *)&pidlRelative);
    if (FAILED(hr))
    {
        ::CoTaskMemFree(pidl);
        return NULL;
    }

    const ITEMIDLIST *array[1] = { pidlRelative };
    hr = pShellFolder->GetUIObjectOf(NULL, ARRAYSIZE(array), array, riid, NULL, (void**)&pDataObject);

    pShellFolder->Release();
    ILFree(pidlRelative);
    ::CoTaskMemFree(pidl);

    if (FAILED(hr))
        pDataObject = NULL;

    return pDataObject;
}
