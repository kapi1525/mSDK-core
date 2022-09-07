#pragma once

// FileIO.hpp
// This file contains definitions and declarations of functions and classes used for file io
// You should use these functions/clases over C++ and C standard library functions/clases

#include "CommonSDK.hpp"
#include "cstdint"



enum class FileError {
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


// File handle looks nicer than plain winapi stuff.
using FileHandle = HFILE;



// File functions
DllImport FileHandle FusionAPI File_OpenA(const char* FileName, int Mode);
DllImport FileHandle FusionAPI File_CreateA(const char* FileName);
DllImport BOOL       FusionAPI File_ExistA(const char* FilePath);
DllImport int        FusionAPI File_Read(FileHandle File, void* Buffer, unsigned long Length);
DllImport int        FusionAPI File_ReadAndCount(FileHandle File, void* Buffer, unsigned long Length);
// DllImport int        FusionAPI File_ReadShortIntelData(FileHandle File, void** BufferPointer);
// DllImport int        FusionAPI File_ReadShortIntelString(FileHandle File, void** BufferPointer);
// DllImport int        FusionAPI File_ReadLongIntelData(FileHandle File, void** BufferPointer);
DllImport int        FusionAPI File_Write(FileHandle File, void* Buffer, unsigned long Length);
DllImport long       FusionAPI File_GetPosition(FileHandle File);
DllImport long       FusionAPI File_SeekBegin(FileHandle File, long Position);
DllImport long       FusionAPI File_SeekCurrent(FileHandle File, long Position);
DllImport long       FusionAPI File_SeekEnd(FileHandle File, long Position);
DllImport long       FusionAPI File_GetLength(FileHandle File );
DllImport void       FusionAPI File_Close(FileHandle File);

// Dont use these functions nor those with A at the end, use the universal functions defined bellow.
DllImport FileHandle FusionAPI File_OpenW(const wchar_t* fname, int mode);
DllImport FileHandle FusionAPI File_CreateW(const wchar_t* FileName);
DllImport BOOL       FusionAPI File_ExistW(const wchar_t* FilePath);



// Universal functions for both unicode and ascii
inline FileHandle File_Open(const TCHAR* FileName, int Mode) {
    #ifdef _UNICODE
    return File_OpenW(FileName, Mode);
    #else
    return File_OpenA(FileName, Mode);
    #endif
}

inline FileHandle File_Create(const TCHAR* FileName) {
    #ifdef _UNICODE
    return File_CreateW(FileName);
    #else
    return File_CreateA(FileName);
    #endif
}

inline FileHandle File_Exist(const TCHAR* FileName) {
    #ifdef _UNICODE
    return File_ExistW(FileName);
    #else
    return File_ExistA(FileName);
    #endif
}



// Abstract file classes

// Input file - abstract class
class DllImport CInputFile {
public:
    virtual ~CInputFile() = default;

    virtual void Delete();

    virtual int Read(uint8_t* dest, unsigned long lsize) = 0;
    virtual int Read (uint8_t* dest, unsigned long lsize, unsigned long* pRead) = 0;
    virtual int ReadByte(uint8_t* dest) = 0;
    virtual int ReadIntelWord(unsigned short* dest);
    virtual int ReadIntelDWord(unsigned long* dest);
    virtual int ReadMacWord(unsigned short* dest);
    virtual int ReadMacDWord(unsigned long* dest);
    virtual int ReadIntelFloat(float* dest);
    virtual int ReadMacFloat(float* dest);
    virtual int ReadIntelWordArray(unsigned short* dest, int count);
    virtual int ReadIntelDWordArray(unsigned long* dest, int count);
    virtual int ReadIntelFloatArray(float* dest, int count);
    virtual int ReadMacWordArray(unsigned short* dest, int count);
    virtual int ReadMacDWordArray(unsigned long* dest, int count);
    virtual int ReadMacFloatArray(float* dest, int count);

    virtual long GetPosition() = 0;
    virtual long GetLength() = 0;
    virtual long Seek(long pos, int method) = 0;

    virtual int Attach(HANDLE hnd) = 0;
    virtual HANDLE Detach() = 0;

    virtual uint8_t* GetBuffer(unsigned int nSize);
    virtual void FreeBuffer(uint8_t* buf);

    virtual LPSTR GetFileNameA() = 0;
    virtual LPWSTR GetFileNameW() = 0;

    // TODO: Turn these into inline functions
    // #ifdef _CFCFILE_UNICODE_DEFS
    // #if defined(_UNICODE)
    // #define GetFileName GetFileNameW
    // #else
    // #define GetFileName GetFileNameA
    // #endif
    // #endif
};


// Bufferized input file
class DllImport CInputBufFile : public CInputFile {
public:
    CInputBufFile();
    virtual ~CInputBufFile();

    static CInputBufFile* NewInstance();

    int Create(FileHandle hf);
    int Create(FileHandle hf, unsigned long dwOffset, unsigned long dwSize);
    int Create(LPCSTR filename);
    int Create(LPCSTR filename, unsigned long dwOffset, unsigned long dwSize);
    int Create(LPCWSTR filename);
    int Create(LPCWSTR filename, unsigned long dwOffset, unsigned long dwSize);

    virtual int Read(uint8_t* dest, unsigned long lsize);
    virtual int Read (uint8_t* dest, unsigned long lsize, unsigned long* pRead);
    virtual int ReadByte(uint8_t* dest);
    // virtual int ReadWord(unsigned short* dest);
    // virtual int ReadDWord(unsigned long* dest);
    virtual long GetPosition();
    virtual long GetLength();
    virtual long Seek(long pos, int method);

    virtual int Attach(HANDLE hnd);
    virtual HANDLE Detach();

    // virtual uint8_t* GetBuffer(unsigned int nSize);
    // virtual void FreeBuffer(uint8_t* buf);

    virtual LPSTR GetFileNameA();
    virtual LPWSTR GetFileNameW();

protected:
    int Attach(HANDLE hnd, unsigned long dwOffset, unsigned long dwSize);
private:
    FileHandle m_hf;
    unsigned long m_curpos;
    uint8_t* m_buffer;
    uint8_t* m_bufcurr;
    unsigned long m_remains;
    LPWSTR m_fnameW;
    LPSTR m_fnameA;

    unsigned long m_startOffset;
    unsigned long m_length;
};
#define BUFFILE_BUFFER_SIZE 16384


// Memory input file
class DllImport CInputMemFile : public CInputFile {
public:
    CInputMemFile();
    virtual ~CInputMemFile();

    static CInputMemFile* NewInstance();

    int Create(uint8_t* buffer, unsigned long lsize);
    int Create(unsigned long lsize);
    uint8_t* GetMemBuffer();

    virtual int Read(uint8_t* dest, unsigned long lsize);
    virtual int Read (uint8_t* dest, unsigned long lsize, unsigned long* pRead);
    virtual int ReadByte(uint8_t* dest);
    // virtual int ReadWord(unsigned short* dest);
    // virtual int ReadDWord(unsigned long* dest);
    virtual long GetPosition();
    virtual long GetLength();
    virtual long Seek(long pos, int method);

    virtual int Attach(HANDLE hnd);
    virtual HANDLE Detach();

    virtual uint8_t* GetBuffer(unsigned int nSize);
    virtual void FreeBuffer(uint8_t* buf);

    virtual LPSTR GetFileNameA();
    virtual LPWSTR GetFileNameW();

private:
    uint8_t* m_buffer;
    uint8_t* m_bufcurr;
    unsigned long m_curpos;
    unsigned long m_remains;
    BOOL m_bAutoDelete;
};

// Output file: base class
class DllImport COutputFile {
public:
    COutputFile();
    virtual ~COutputFile();

    // virtual int WriteByte(BYTE b) = 0;
    // virtual int WriteWord(WORD b) = 0;

    int WriteIntelWord(unsigned short* pw);
    int WriteIntelDWord(unsigned long* pdw);
    int WriteIntelFloat(float* dest);
    int WriteMacWord(unsigned short* pw);
    int WriteMacDWord(unsigned long* pdw);

    virtual int Write(uint8_t* pb, unsigned int sz) = 0;
    virtual int Flush() = 0;
    virtual unsigned long GetLength() = 0;

    virtual long GetPosition() = 0;
    virtual long Seek(long pos, int method) = 0;

    virtual LPSTR GetFileNameA() = 0;
    virtual LPWSTR GetFileNameW() = 0;
};

// Memory output file
class DllImport COutputMemFile : public COutputFile {
public:
    COutputMemFile();
    virtual ~COutputMemFile();

    int Create(unsigned int nBlockSize = 512);
    int Create(uint8_t* buffer, unsigned long nBufferSize = 0x7FFFFFF);
    uint8_t* GetBuffer();
    uint8_t* DetachBuffer();
    static void FreeBuffer(uint8_t* pBuffer);

    virtual int Write(uint8_t* pb, unsigned int sz);
    virtual int Flush();
    virtual unsigned long GetLength();

    virtual long GetPosition();
    virtual long Seek(long pos, int method);

    virtual LPSTR GetFileNameA();
    virtual LPWSTR GetFileNameW();

private:
    uint8_t* m_buffer;
    uint8_t* m_curptr;
    unsigned long m_totalsize;
    unsigned long m_cursize;
    unsigned long m_blocksize;
    BOOL m_bReallocable;
};

// Bufferized output file
class DllImport COutputBufFile : public COutputFile {
public:
    COutputBufFile();
    virtual ~COutputBufFile();

    int Create(FileHandle hf, unsigned int nBufferSize = 4096);
    int Create(LPCSTR fname, unsigned int nBufferSize = 4096);
    int Create(LPCWSTR fname, unsigned int nBufferSize = 4096);

    virtual int Write(uint8_t* pb, unsigned int sz);
    virtual int Flush();
    virtual unsigned long GetLength();

    virtual long GetPosition();
    virtual long Seek(long pos, int method);

    virtual LPSTR GetFileNameA();
    virtual LPWSTR GetFileNameW();

private:
    FileHandle m_hf;
    LPWSTR m_fnameW;
    uint8_t* m_buffer;
    uint8_t* m_curptr;
    unsigned long m_cursize;
    unsigned long m_buffersize;
    BOOL m_bBuffered;
    LPSTR m_fnameA;
};
