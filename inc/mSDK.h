#pragma once


// Fancier than WINAPI
#ifndef FusionAPI
#define FusionAPI __stdcall
#endif

// stollen from: https://github.com/SortaCore/MMF2Exts/blob/0540807e8c84785f2d46e2a60dceea735fcad83f/DarkEdif/Inc/Windows/WindowsDefines.hpp#L12
// Repleaces .def file and allows us to save more space
#ifndef MFXExport
#define MFXExport comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
#endif