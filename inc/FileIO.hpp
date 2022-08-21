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
