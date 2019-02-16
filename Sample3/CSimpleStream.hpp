// CSimpleStream.hpp
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>
// This file is public domain software.

class CSimpleStream : public IStream
{
public:
    static IStream *CreateInstance(const TCHAR *pszFile);

private:
    CSimpleStream(HANDLE);
    virtual ~CSimpleStream();

    // IUnknown
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void **);
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();

    // ISequentialStream
    virtual HRESULT STDMETHODCALLTYPE Read(void *, ULONG, ULONG *);
    virtual HRESULT STDMETHODCALLTYPE Write(const void *, ULONG, ULONG *);

    // IStream
    virtual HRESULT STDMETHODCALLTYPE Seek(LARGE_INTEGER, DWORD, ULARGE_INTEGER *);
    virtual HRESULT STDMETHODCALLTYPE SetSize(ULARGE_INTEGER);
    virtual HRESULT STDMETHODCALLTYPE CopyTo(IStream *, ULARGE_INTEGER, ULARGE_INTEGER *, ULARGE_INTEGER *);
    virtual HRESULT STDMETHODCALLTYPE Commit(DWORD);
    virtual HRESULT STDMETHODCALLTYPE Revert();
    virtual HRESULT STDMETHODCALLTYPE LockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD);
    virtual HRESULT STDMETHODCALLTYPE UnlockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD);
    virtual HRESULT STDMETHODCALLTYPE Stat(STATSTG *, DWORD);
    virtual HRESULT STDMETHODCALLTYPE Clone(IStream **);

    LONG m_nRefCount;
    HANDLE m_hFile;
};
