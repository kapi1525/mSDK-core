#pragma once

// FileIO.hpp
// This file contains definitions and declarations of functions and classes used for file io
// You should use these functions/clases over C++ and C standard library functions/clases



namespace FileError {
    enum {
        None = 0,

        OutOfMemory = 0x40000000,
        ReadError,
        EndOfFile,
        WriteError,
        DiskFull,
        CannotOpenFile,
        CannotCreateFile,
        BufferTooSmall,
        CannotSetFileSize,
        Internal,

        Max = 0x40010000
    };
}

namespace FileMode {
    enum {
        Read = 0x00000000,
        Write = 0x00000001,
        ReadWrite = 0x00000002,
        ShareCompat = 0x00000000,
        ShareExclusive = 0x00000010,
        ShareDenyWrite = 0x00000020,
        ShareDenyRead = 0x00000030,
        ShareDenyNone = 0x00000040,
    };
}


using FileHandle = HFILE;



// File functions
__declspec(dllimport) FileHandle __stdcall File_OpenA(const char* FileName, int Mode);
__declspec(dllimport) FileHandle __stdcall File_CreateA(const char* FileName);
__declspec(dllimport) BOOL __stdcall       File_ExistA(const char* FilePath);
__declspec(dllimport) int __stdcall        File_Read(FileHandle File, void* Buffer, unsigned long Length);
__declspec(dllimport) int __stdcall        File_ReadAndCount(FileHandle File, void* Buffer, unsigned long Length);
__declspec(dllimport) int __stdcall        File_ReadShortIntelData(FileHandle File, void** BufferPointer);
__declspec(dllimport) int __stdcall        File_ReadShortIntelString(FileHandle File, void** BufferPointer);
__declspec(dllimport) int __stdcall        File_ReadLongIntelData(FileHandle File, void** BufferPointer);
__declspec(dllimport) int __stdcall        File_Write(FileHandle File, void* Buffer, unsigned long Length);
__declspec(dllimport) long __stdcall       File_GetPosition(FileHandle File);
__declspec(dllimport) long __stdcall       File_SeekBegin(FileHandle File, long Position);
__declspec(dllimport) long __stdcall       File_SeekCurrent(FileHandle File, long Position);
__declspec(dllimport) long __stdcall       File_SeekEnd(FileHandle File, long Position);
__declspec(dllimport) long __stdcall       File_GetLength(FileHandle File );
__declspec(dllimport) void __stdcall       File_Close(FileHandle File);

// mmfs2.lib is old enough to use shorts instead of wchar_t, this causes link errors.
// I hope theres newer lib... somewhere...
__declspec(dllimport) FileHandle __stdcall File_OpenW(const unsigned short* fname, int mode);
__declspec(dllimport) FileHandle __stdcall File_CreateW(const unsigned short* FileName);
__declspec(dllimport) BOOL __stdcall       File_ExistW(const unsigned short* FilePath);



// Universal functions for both unicode and ascii
inline FileHandle File_Open(const TCHAR* FileName, int Mode) {
    #ifdef _UNICODE
    return File_OpenW((const unsigned short*)FileName, Mode);
    #else
    return File_OpenA(FileName, Mode);
    #endif
}

inline FileHandle File_Create(const TCHAR* FileName) {
    #ifdef _UNICODE
    return File_CreateW((const unsigned short*)FileName);
    #else
    return File_CreateA(FileName);
    #endif
}

inline FileHandle File_Exist(const TCHAR* FileName) {
    #ifdef _UNICODE
    return File_ExistW((const unsigned short*)FileName);
    #else
    return File_ExistA(FileName);
    #endif
}



// TODO: Remove these classes?  Does anyone even use them?

class __declspec(dllimport) CInputFile {
    public:
    virtual ~CInputFile() = default;

    virtual void Delete();

    virtual int Read(LPBYTE dest, DWORD lsize) = 0;
    virtual int Read(LPBYTE dest, DWORD lsize, LPDWORD pRead) = 0;
    virtual int ReadByte(LPBYTE dest) = 0;
    virtual int ReadIntelWord(LPWORD dest);
    virtual int ReadIntelDWord(LPDWORD dest);
    virtual int ReadMacWord(LPWORD dest);
    virtual int ReadMacDWord(LPDWORD dest);
    virtual int ReadIntelFloat(PFLOAT dest);
    virtual int ReadMacFloat(PFLOAT dest);
    virtual int ReadIntelWordArray(LPWORD dest, int count);
    virtual int ReadIntelDWordArray(LPDWORD dest, int count);
    virtual int ReadIntelFloatArray(PFLOAT dest, int count);
    virtual int ReadMacWordArray(LPWORD dest, int count);
    virtual int ReadMacDWordArray(LPDWORD dest, int count);
    virtual int ReadMacFloatArray(PFLOAT dest, int count);

    virtual long GetPosition() = 0;
    virtual long GetLength() = 0;
    virtual long Seek(long pos, int method) = 0;

    virtual int Attach(HANDLE hnd) = 0;
    virtual HANDLE Detach() = 0;

    virtual LPBYTE GetBuffer(UINT nSize);
    virtual void FreeBuffer(LPBYTE buf);

    virtual LPSTR GetFileNameA() = 0;
    virtual LPWSTR GetFileNameW() = 0;

    #ifdef _CFCFILE_UNICODE_DEFS
    #if defined(_UNICODE)
        #define GetFileName GetFileNameW
    #else
        #define GetFileName GetFileNameA
    #endif
    #endif
};



#define BUFFILE_BUFFER_SIZE 16384

class __declspec(dllimport) CInputBufFile : public CInputFile {
    public:
    CInputBufFile();
    virtual ~CInputBufFile();

    static CInputBufFile* NewInstance();

    int Create(HFILE hf);
    int Create(HFILE hf, DWORD dwOffset, DWORD dwSize);
    int Create(LPCSTR filename);
    int Create(LPCSTR filename, DWORD dwOffset, DWORD dwSize);
    int Create(LPCWSTR filename);
    int Create(LPCWSTR filename, DWORD dwOffset, DWORD dwSize);

    virtual int Read(LPBYTE dest, DWORD lsize);
    virtual int Read(LPBYTE dest, DWORD lsize, LPDWORD pRead);
    virtual int ReadByte(LPBYTE dest);
    virtual long GetPosition();
    virtual long GetLength();
    virtual long Seek(long pos, int method);
 
    virtual int Attach(HANDLE hnd);
    virtual HANDLE Detach();

    virtual LPSTR GetFileNameA();
    virtual LPWSTR GetFileNameW();

    protected:
    int Attach(HANDLE hnd, DWORD dwOffset, DWORD dwSize);

    private:
    HFILE m_hf;
    DWORD m_curpos;
    LPBYTE m_buffer;
    LPBYTE m_bufcurr;
    DWORD m_remains;
    LPWSTR m_fnameW;
    LPSTR m_fnameA;

    DWORD m_startOffset;
    DWORD m_length;
};



// Memory input file
class __declspec(dllimport) CInputMemFile : public CInputFile {
    public:
    CInputMemFile();
    virtual ~CInputMemFile();

    static CInputMemFile* NewInstance();

    int Create(LPBYTE buffer, DWORD lsize);
    int Create(DWORD lsize);
    LPBYTE GetMemBuffer();

    virtual int Read(LPBYTE dest, DWORD lsize);
    virtual int Read (LPBYTE dest, DWORD lsize, LPDWORD pRead);
    virtual int ReadByte(LPBYTE dest);
    virtual long GetPosition();
    virtual long GetLength();
    virtual long Seek(long pos, int method);
 
    virtual int Attach(HANDLE hnd);
    virtual HANDLE Detach();

    virtual LPBYTE GetBuffer(UINT nSize);
    virtual void FreeBuffer(LPBYTE buf);

    virtual LPSTR GetFileNameA() { return NULL; }
    virtual LPWSTR GetFileNameW() { return NULL; }

    private:
    LPBYTE m_buffer;
    LPBYTE m_bufcurr;
    DWORD m_curpos;
    DWORD m_remains;
    BOOL m_bAutoDelete;
};

// Output file: base class
class __declspec(dllimport) COutputFile {
    public:
    COutputFile() {};
    virtual ~COutputFile() {};

    int WriteIntelWord(LPWORD pw);
    int WriteIntelDWord(LPDWORD pdw);
    int WriteIntelFloat(PFLOAT dest);
    int WriteMacWord(LPWORD pw);
    int WriteMacDWord(LPDWORD pdw);

    virtual int Write(LPBYTE pb, UINT sz) = 0;
    virtual int Flush() = 0;
    virtual DWORD GetLength() = 0;

    virtual long GetPosition() = 0;
    virtual long Seek(long pos, int method) = 0;

    virtual LPSTR GetFileNameA() = 0;
    virtual LPWSTR GetFileNameW() = 0;
};

// Memory output file
class __declspec(dllimport) COutputMemFile : public COutputFile {
    public:
    COutputMemFile();
    virtual ~COutputMemFile();

    int Create(UINT nBlockSize = 512);
    int Create(LPBYTE buffer, DWORD nBufferSize = 0x7FFFFFF);
    LPBYTE GetBuffer();
    LPBYTE DetachBuffer();
    static void FreeBuffer(LPBYTE pBuffer); // car il faut lib�rer � l'interieur de la DLL

    virtual int Write(LPBYTE pb, UINT sz);
    virtual int Flush();
    virtual DWORD GetLength();

    virtual long GetPosition();
    virtual long Seek(long pos, int method);

    virtual LPSTR GetFileNameA() { return NULL; }
    virtual LPWSTR GetFileNameW() { return NULL; }

    private:
    LPBYTE m_buffer;
    LPBYTE m_curptr;
    DWORD m_totalsize;
    DWORD m_cursize;
    DWORD m_blocksize;
    BOOL m_bReallocable;
};

// Bufferized output file
class __declspec(dllimport) COutputBufFile : public COutputFile {
    public:
    COutputBufFile();
    virtual ~COutputBufFile();

    int Create(HFILE hf, UINT nBufferSize = 4096);
    int Create(LPCSTR fname, UINT nBufferSize = 4096);
    int Create(LPCWSTR fname, UINT nBufferSize = 4096);

    virtual int Write(LPBYTE pb, UINT sz);
    virtual int Flush();
    virtual DWORD GetLength();

    virtual long GetPosition();
    virtual long Seek(long pos, int method);

    virtual LPSTR GetFileNameA();
    virtual LPWSTR GetFileNameW();

    private:
    HFILE m_hf;
    LPWSTR m_fnameW;
    LPBYTE m_buffer;
    LPBYTE m_curptr;
    DWORD m_cursize;
    DWORD m_buffersize;
    BOOL m_bBuffered;
    LPSTR m_fnameA;
};

