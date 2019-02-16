// CSimpleStream.cpp
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.

#include <objbase.h>
#include "CSimpleStream.hpp"

CSimpleStream::CSimpleStream(HANDLE hFile) : m_nRefCount(0), m_hFile(hFile)
{
}

CSimpleStream::~CSimpleStream()
{
    ::CloseHandle(m_hFile);
}

IStream *CSimpleStream::CreateInstance(const TCHAR *pszFile)
{
    HANDLE hFile = ::CreateFile(pszFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return NULL;

    if (CSimpleStream *pStream = new CSimpleStream(hFile))
    {
        pStream->AddRef();
        return pStream;
    }

    ::CloseHandle(hFile);
    return NULL;
}

HRESULT STDMETHODCALLTYPE CSimpleStream::QueryInterface(REFIID riid, void **ppvObject)
{
    if (IsEqualIID(riid, IID_IUnknown) ||
        IsEqualIID(riid, IID_ISequentialStream) ||
        IsEqualIID(riid, IID_IStream))
    {
        *ppvObject = static_cast<IStream *>(this);
        AddRef();
        return S_OK;
    }

    *ppvObject = NULL;
    return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE CSimpleStream::AddRef()
{
    return ++m_nRefCount;
}

ULONG STDMETHODCALLTYPE CSimpleStream::Release()
{
    m_nRefCount--;
    if (m_nRefCount > 0)
        return m_nRefCount;
    delete this;
    return 0;
}

HRESULT STDMETHODCALLTYPE
CSimpleStream::Read(void *pv, ULONG cb, ULONG *pcbRead)
{
    DWORD   dwSizeRead = 0;

    if (!::ReadFile(m_hFile, pv, cb, &dwSizeRead, NULL) || dwSizeRead == 0)
    {
        return S_FALSE;
    }
    if (pcbRead)
    {
        *pcbRead = dwSizeRead;
    }

    return S_OK;
}

HRESULT STDMETHODCALLTYPE
CSimpleStream::Write(const void *pv, ULONG cb, ULONG *pcbWritten)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE
CSimpleStream::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition)
{
    DWORD           dwMoveMethod;
    ULARGE_INTEGER  libNewPos = {0, 0};

    switch (dwOrigin)
    {
        case STREAM_SEEK_SET:
            dwMoveMethod = FILE_BEGIN;
            break;
        case STREAM_SEEK_CUR:
            dwMoveMethod = FILE_CURRENT;
            break;
        case STREAM_SEEK_END:
            dwMoveMethod = FILE_END;
            break;
    }

    libNewPos.LowPart = SetFilePointer(m_hFile, dlibMove.LowPart, NULL, dwMoveMethod);
    if (plibNewPosition)
        *plibNewPosition = libNewPos;

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CSimpleStream::SetSize(ULARGE_INTEGER libNewSize)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CSimpleStream::CopyTo(IStream *, ULARGE_INTEGER, ULARGE_INTEGER *, ULARGE_INTEGER *)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CSimpleStream::Commit(DWORD)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CSimpleStream::Revert()
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CSimpleStream::LockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CSimpleStream::UnlockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CSimpleStream::Stat(STATSTG *, DWORD)
{
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CSimpleStream::Clone(IStream **)
{
    return E_NOTIMPL;
}
