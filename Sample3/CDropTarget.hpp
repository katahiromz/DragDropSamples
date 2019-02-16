// CDropTarget.hpp
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.

#ifndef CDROPTARGET_HPP_
#define CDROPTARGET_HPP_

#include <oleidl.h>

class CDropTarget : IDropTarget
{
public:
    static IDropTarget *CreateInstance(HWND hwnd);

private:
    CDropTarget(HWND hwnd);
    virtual ~CDropTarget();

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();

    virtual HRESULT STDMETHODCALLTYPE DragEnter(IDataObject *pDataObject, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
    virtual HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
    virtual HRESULT STDMETHODCALLTYPE DragLeave();
    virtual HRESULT STDMETHODCALLTYPE Drop(IDataObject *pDataObject, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);

protected:
    LONG m_nRefCount;
    HWND m_hWnd;
};

#endif  // ndef CDROPTARGET_HPP_
