// CDropSource.hpp
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.

#ifndef CDROPSOURCE_HPP_
#define CDROPSOURCE_HPP_

#define COBJMACROS
#include <windows.h>
#include <shobjidl.h>
#include <shlobj.h>
#include <oleidl.h>     // for IDropSource, IDataObject

class CDropSource : public IDropSource
{
public:
    static IDropSource *CreateInstance();

private:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();

    HRESULT STDMETHODCALLTYPE QueryContinueDrag(WINBOOL fEscapePressed, DWORD grfKeyState);
    HRESULT STDMETHODCALLTYPE GiveFeedback(DWORD dwEffect);

    CDropSource();
    virtual ~CDropSource();

protected:
    LONG m_nRefCount;
};

IDataObject *GetFileDataObject(const TCHAR *pszFile, REFIID riid);

#endif  // ndef CDROPSOURCE_HPP_
