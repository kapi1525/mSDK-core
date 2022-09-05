#pragma once

// CommonSDK.hpp
// Common includes and macros for other sdk headers


// Fancier than WINAPI
#define FusionAPI __stdcall

// stollen from: https://github.com/SortaCore/MMF2Exts/blob/0540807e8c84785f2d46e2a60dceea735fcad83f/DarkEdif/Inc/Windows/WindowsDefines.hpp#L12
// Repleaces .def file and allows us to save more space
#define MFXExport comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)


#define DllImport __declspec(dllimport)
#define DllExport __declspec(dllexport)


// Windows.h
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>

#undef WIN32_LEAN_AND_MEAN
#undef NOMINMAX


// bitmask operators for enum classes
#define EnumClassBitMask(T) \
inline T operator| (T a, T b) { return (T)((int)a | (int)b); }        \
inline T operator& (T a, T b) { return (T)((int)a & (int)b); }        \
inline T operator^ (T a, T b) { return (T)((int)a ^ (int)b); }        \
inline T& operator|= (T& a, T b) { return (T&)((int&)a |= (int)b); }  \
inline T& operator&= (T& a, T b) { return (T&)((int&)a &= (int)b); }  \
inline T& operator^= (T& a, T b) { return (T&)((int&)a ^= (int)b); }
