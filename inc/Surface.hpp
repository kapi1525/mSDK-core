#pragma once

// Surface.hpp
// This header declares cSurface class, other classes related to it, palete functions and predefined color macros.

#include "CommonSDK.hpp"
#include <climits>
#include <cstdint>



// Predefined colors
#define BLACK        RGB(0x00,0x00,0x00)
#define OPAQUE_BLACK RGB(8,0,0)

// maximum opacity
#define OP_MAX 128


// Forwards
class CFillData;
class CFillFlat;
class CFillDir;
class CFillMosaic;
class cSurface;
class cSurfaceImplementation;



// Fill classes

// Base class
class DllImport CFillData {
public:
    CFillData();
    virtual ~CFillData();
    virtual DWORD SetFillOrg(int x, int y);
    virtual BOOL Fill(cSurface* pSf, int l, int t, int r, int b, BOOL bForceOpaqueBlack = false) = 0;
    virtual BOOL Fill(cSurface* pSf, int trueW = INT_MAX, int trueH = INT_MAX, BOOL bForceOpaqueBlack = false);

protected:
    inline  void VERIFY_BLACK(COLORREF& cr);
};


class DllImport CFillFlat : public CFillData {
public:
    CFillFlat(COLORREF cr);
    CFillFlat(const CFillFlat& scr);

private:
    // Attributes
    COLORREF m_cr;

public:
    void SetColor(COLORREF newCr);

    // Implementation
    virtual BOOL Fill(cSurface* pSf, int l, int t, int r, int b, BOOL bForceOpaqueBlack = false);
    COLORREF GetColor();
};


class DllImport CFillDir : public CFillData {
public:
    CFillDir(COLORREF crFrom, COLORREF crTo, BOOL Vertical, int xOrg = 0, int yOrg = 0);
    CFillDir(const CFillDir& src);

private:
    int m_xOrg;
    int m_yOrg;
    // Attributes
    COLORREF m_crFrom;
    COLORREF m_crTo;
    BOOL m_vert;

public:
    void SetColors(COLORREF crFrom, COLORREF crTo);
    void SetDir(BOOL vert);
    // Implementation
    virtual DWORD SetFillOrg(int x, int y);
    virtual BOOL Fill(cSurface* pSf, int l, int t, int r, int b, BOOL bForceOpaqueBlack = false);
    virtual BOOL Fill(cSurface* pSf, int trueW = INT_MAX, int trueH = INT_MAX, BOOL bForceOpaqueBlack = false);
};


class DllImport CFillMosaic : public CFillData {
public:
    CFillMosaic(cSurface* psfMosaic, int xOrg = 0, int yOrg = 0);
    CFillMosaic(const CFillMosaic& src);

private:
    // Attributes
    cSurface* m_pSfTile;
    int m_xOrg;
    int m_yOrg;

public:
    // Implementation
    void SetTile(cSurface* pSf);
    virtual DWORD SetFillOrg(int x, int y);
    virtual BOOL Fill(cSurface* pSf, int l, int t, int r, int b, BOOL bForceOpaqueBlack = false);
};



// Palettes
extern "C" {
    struct csPalette {
        // LOGPALETTE
        unsigned short palVersion;
        unsigned short palNumEntries;
        PALETTEENTRY palEntry[256];

        unsigned char* Cache;           // 32K cache table for quick remap
        HPALETTE hPal;                  // Windows HPALETTE
        unsigned int opaqueBlackIndex;  // Index of opaque black (256 colors only)
        unsigned int refCount;
    };


    // Initialize and free palette manager
    DllImport void FusionAPI Init_Palettes();
    DllImport void FusionAPI Free_Palettes();

    // Create / Delete CSPalette
    DllImport csPalette FusionAPI csPalette_Create(LOGPALETTE pLogPal);
    DllImport void      FusionAPI csPalette_Delete(csPalette* pCsPal);// another name for csPalette_SubRef
    DllImport csPalette FusionAPI csPalette_NormalizePalette(csPalette* pCsPal, unsigned char* pTabRemap);

    // Create default 256 color palette
    // This palette must be deleted using csPalette_Delete or csPalette_SubRef
    DllImport csPalette FusionAPI csPalette_CreateDefaultPalette();

    // GetNearColorIndex with 16bit cache (warning: small loss of color possible)
    DllImport int csPalette_GetNearColorIndex(csPalette* ptCsPal, COLORREF color);
    DllImport void FusionAPI csPalette_GetPaletteEntries(csPalette* ptCsPal, PALETTEENTRY* pe, int start, int count);

    // Normalize palette: add system colors and replace black>0 by OPAQUE_BLACK
    DllImport BOOL FusionAPI NormalizePalette(PALETTEENTRY*destPal, PALETTEENTRY*srcPal, unsigned char* ptabRemap);

    extern unsigned char Palet16[], Palet256[];     // Default palettes
}


// Increment / Decrement CSPalette recount
DllImport void FusionAPI csPalette_AddRef(csPalette* pCsPal);
DllImport void FusionAPI csPalette_SubRef(csPalette* pCsPal);

// Get Windows palette handle
DllImport HPALETTE FusionAPI csPalette_GetPaletteHandle(csPalette* pCsPal);

// Init cache for fast GetNearColorIndex
DllImport int FusionAPI csPalette_InitCache(csPalette* ptCsPal);

// GetNearColorIndex with 16bit cache (warning: small loss of color possible)
DllImport int FusionAPI csPalette_GetNearColorIndex_Fast(csPalette* ptCsPal, COLORREF color);



// Surface enums
enum class BlitMode {
    Opaque,
    Transparent,
    Max
};

enum class BlitOp {
    Copy,                       // None
    Blend,                      // dest = ((dest* coef) + (src* (128-coef)))/128
    Invert,                     // dest = src XOR 0xFFFFFF
    XOR,                        // dest = src XOR dest
    AND,                        // dest = src AND dest
    OR,                         // dest = src OR dest
    BlendRepleaceTransparent,   // dest = ((dest* coef) + ((src==transp)?replace:src* (128-coef)))/128
    DWROP,
    ANDNOT,
    ADD,
    MONO,
    SUB,
    BlendDontReplaceColor,
    EffectEX,
    Max,
    Mask = 0xFFF,
    RGBAFilter = 0x1000,
};
EnumClassBitMask(BlitOp)

enum SetPaletteAction {
    None,                       // Just update palette
    RemapSurface,               // remap current surface pixels to new palette
    Max
};

enum class SurfaceType {
    Memory,                     // Buffer only
    MemoryWithDC,               // Buffer + DC (i.e. DIBSection, DDRAW surface, etc...
    MemoryWithPermanentDC,      // Buffer + permanent DC (i.e. DIBDC)
    DirectDrawMemory,           // Surface Direct Draw en m�moire systeme
    HWAScreen,                  // Screen surface in HWA mode
    HWARenderTargetTexture,     // Render target texture in HWA mode
    HWAUnmanagedTexture,        // HWA texture created in video memory, unmanaged (lost when device is lost)
    HWAManagedTexture,          // HWA texture created in video memory, managed (automatically reloaded when the device is lost)
    Max
};

enum class SurfaceDriver {
    DIB,            // DIB (standard driver: DIBSection)
    WinG,
    DirectDraw,
    Bitmap,         // Win 3.1 bitmap
    _3DFX,
    Direct3D9,
    Direct3D8,
    Direct3D11,
    Max
};


enum class LIFlags {
    None = 0,
    Remap = 1 << 0,
    ChangeSurfaceDepth = 1 << 1,
    DoNotNormalizePalette = 1 << 2
};
EnumClassBitMask(LIFlags)

enum class SIFlags {
    None = 0,
    OnlyHeader = 1 << 0,
    SaveAlpha = 1 << 1
};
EnumClassBitMask(SIFlags)

// TODO: What is this?
// enum {
//     LBF_DONOTCHANGESURFACEDEPTH = 0x0001
// };

// Blilt options
enum BlitFlag {
    BLTF_ANTIA              = 0x0001,       // Anti-aliasing
    BLTF_COPYALPHA          = 0x0002,       // Copy alpha channel to destination alpha channel instead of applying it
    BLTF_SAFESRC            = 0x0010,
    BLTF_TILE               = 0x0020
};

// Stretch& BlitEx options
enum StrechFlag {
    STRF_RESAMPLE           = 0x0001,       // Resample bitmap
    STRF_RESAMPLE_TRANSP    = 0x0002,       // Resample bitmap, but doesn't resample the transparent color
    STRF_COPYALPHA          = 0x0004,       // Copy (stretch) alpha channel to destination alpha channel instead of applying it
    STRF_SAFESRC            = 0x0010,
    STRF_TILE               = 0x0020
};

// Returned by cSurface::GetLastError() (?)
enum class SurfaceError {
	None = 0,
	NotSupported = 0x40010000,
	SurfaceNotCreated,
	Internal,
    Max = 0x40020000
};

// TODO: What is this?
// #define SCMF_FULL        0x0000
// #define SCMF_PLATFORM    0x0001



// Surface structures

struct DisplayMode {
    int mcx;
    int mcy;
    int mDepth;
    // int mNearestBitCount;
    // int mDriver;
};

// Transparent monochrome mask for collisions
struct sMask {
    int mkSize;
    int mkWidth;
    int mkHeight;
    unsigned int mkWidthBytes;
    int mkXSpot;
    int mkYSpot;
    unsigned long mkFlags;
    RECT mkRect;
};



// Surface callbacks

// Convention : SfSrc.FilterBlit(SfDest, MyCallBack, param)
// ==========       will call MyCallBack(pixelDest, pixelSrc, param)
typedef COLORREF (CALLBACK* FILTERBLITPROC)(COLORREF, COLORREF, DWORD);
typedef COLORREF (CALLBACK* MATRIXFILTERBLITPROC)(COLORREF*, COLORREF*, DWORD);

typedef BOOL (CALLBACK* LPENUMSCREENMODESPROC)(DisplayMode*, LPVOID);
typedef void (CALLBACK* LOSTDEVICECALLBACKPROC)(cSurface*, LPARAM);     // Lost device callback function



// Surface types
using RGBAREF = unsigned long;



// Finaly surface functions and cSurface class

// Allocate/Free surface
DllImport cSurface* FusionAPI NewSurface();
DllImport void      FusionAPI DeleteSurface(cSurface* pSurf);

// Get surface prototype
DllImport BOOL FusionAPI GetSurfacePrototype(cSurface** proto, int depth, int st, int drv);

// DIB
DllImport DWORD FusionAPI GetDIBHeaderSize(int depth);
DllImport DWORD FusionAPI GetDIBWidthBytes(int width, int depth);
DllImport DWORD FusionAPI GetDIBSize(int width, int height, int depth);
DllImport unsigned char* FusionAPI GetDIBBitmap(BITMAPINFO* pBmi);

extern "C" DllImport BOOL FusionAPI BuildRemapTable(unsigned char*, LOGPALETTE*, LOGPALETTE*, WORD);


// Color conversions
inline uint8_t AlphaToSemitransparent(uint8_t Alpha) {
    return ((Alpha == 0) ? 128 : (255 - Alpha) / 2);
}

inline uint8_t SemitransparentToAlpha(uint8_t Semitransparent) {
    return ((Semitransparent == 128) ? 0 : (255 - Semitransparent * 2));
}

inline RGBAREF COLORREFToRGBAREF(COLORREF Color, uint8_t Alpha) {
    return ((Color & 0x00FFFFFF) | (static_cast<int>(Alpha) << 24));
}



class DllImport cSurface {
public:
    cSurface();
    ~cSurface();
    cSurface& operator=(const cSurface& source);

    // Init
    static void InitializeSurfaces();
    static void FreeSurfaces();
    static void FreeExternalModules();

    // Create surface implementation
    void Create(int width, int height, cSurface* prototype);                // Create from surface prototype
    void Create(HDC hDC);                                                   // Create pure DC surface from DC
    void Create(HWND hWnd, BOOL IncludeFrame);                              // Create pure DC surface from a window

    void Clone(const cSurface& pSrcSurface, int newW = -1, int newH = -1);  // Clone surface (= create with same size + Blit)
    void Delete();                                                          // Delete surface implementation (before to create another one)

    // Create screen surface (fullscreen mode)
    BOOL CreateScreenSurface();
    BOOL IsScreenSurface();

    BOOL IsValid();
    int GetLastError();

    // Surface info
    int GetWidth() const;
    int GetHeight() const;
    int GetDepth() const;
    BOOL GetInfo(int& width, int& height, int& depth) const;
    int GetType();
    int GetDriver();
    DWORD GetDriverInfo(void* pInfo);

    // TODO: What is origin?
    void SetOrigin(int x, int y);
    void SetOrigin(POINT c);
    void GetOrigin(POINT& pt);
    void GetOrigin(int& x, int& y);
    void OffsetOrigin(int dx, int dy);
    void OffsetOrigin(POINT delta);

    // Buffer
    uint8_t* LockBuffer();
    void UnlockBuffer(uint8_t* spBuffer);
    int GetPitch() const;

    // Double-buffer handling
    void      SetCurrentDevice();
    int       BeginRendering(BOOL bClear, RGBAREF dwRgba);
    int       EndRendering();
    BOOL      UpdateScreen();
    cSurface* GetRenderTargetSurface();
    void      ReleaseRenderTargetSurface(cSurface* psf);
    void      Flush(BOOL bMax);
    void      SetZBuffer(float z2D);

    // Device context for graphic operations
    HDC  GetDC();
    void ReleaseDC(HDC dc);
    void AttachWindow(HWND hWnd);

    // Clipping
    void GetClipRect(int& x, int& y, int& w, int& h);
    void SetClipRect(int x, int y, int w, int h);
    void ClearClipRect();

    // LoadImage/SaveImage (DIB format)
    BOOL LoadImageA(HFILE hf, DWORD lsize, LIFlags loadFlags = LIFlags::None);
    BOOL LoadImageA(LPCSTR fileName, LIFlags loadFlags = LIFlags::None);
    BOOL LoadImageA(HINSTANCE hInst, int bmpID, LIFlags loadFlags = LIFlags::None);
    BOOL LoadImageA(BITMAPINFO* pBmi, uint8_t* pBits = NULL, LIFlags loadFlags = LIFlags::None);

    BOOL LoadImageW(HFILE hf, DWORD lsize, LIFlags loadFlags = LIFlags::None);
    BOOL LoadImageW(LPCWSTR fileName, LIFlags loadFlags = LIFlags::None);
    BOOL LoadImageW(HINSTANCE hInst, int bmpID, LIFlags loadFlags = LIFlags::None);
    BOOL LoadImageW(BITMAPINFO* pBmi, uint8_t* pBits = NULL, LIFlags loadFlags = LIFlags::None);

    // TODO: Turn this macro into inline function
    #undef LoadImage

    #ifdef _UNICODE
        #define LoadImage LoadImageW
    #else
        #define LoadImage LoadImageA
    #endif

    BOOL SaveImage(HFILE hf, SIFlags saveFlags = SIFlags::None);
    BOOL SaveImage(LPCSTR fileName, SIFlags saveFlags = SIFlags::None);
    BOOL SaveImage(LPCWSTR fileName, SIFlags saveFlags = SIFlags::None);
    BOOL SaveImage(BITMAPINFO* pBmi, uint8_t* pBits, SIFlags saveFlags = SIFlags::None);

    DWORD GetDIBSize();


    // Pixel functions
    void SetPixel(int x, int y, COLORREF c);
    void SetPixel(int x, int y, uint8_t R, uint8_t G, uint8_t B);
    void SetPixel(int x, int y, int index);

    BOOL GetPixel(int x, int y, COLORREF& c) const;
    BOOL GetPixel(int x, int y, uint8_t& R, uint8_t& G, uint8_t& B) const;
    BOOL GetPixel(int x, int y, int& index) const;

    // Faster: assume clipping is done, the origin is at (0,0) and the surface is locked
    void SetPixelFast(int x, int y, COLORREF c);
    void SetPixelFast8(int x, int y, int index);

    COLORREF GetPixelFast(int x, int y);
    int GetPixelFast8(int x, int y);


    // Blit functions
    // Blit surface to surface
    BOOL Blit(cSurface& dest) const;

    // New MMF 2.5 : HIBYTE(dwBlitFlags) = blend coefficient
    BOOL Blit(cSurface& dest, int destX, int destY, BlitMode bm = BlitMode::Opaque, BlitOp bo = BlitOp::Copy, LPARAM param = 0, DWORD dwBlitFlags = 0) const;

    // Blit rectangle to surface
    BOOL Blit(cSurface& dest, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, BlitMode bm /*= BlitMode::Opaque*/, BlitOp bo = BlitOp::Copy, LPARAM param = 0, DWORD dwBlitFlags = 0) const;

    // Extended blit : can do stretch& rotate at the same time
    // Only implemented in 3D mode
    BOOL BlitEx(cSurface& dest, float dX, float dY, float fScaleX, float fScaleY, int sX, int sY, int sW, int sH, LPPOINT pCenter, float fAngle, BlitMode bm = BlitMode::Opaque, BlitOp bo = BlitOp::Copy, LPARAM param = 0, DWORD dwFlags = 0) const;

    // Scrolling
    BOOL Scroll(int xDest, int yDest, int xSrc, int ySrc, int width, int height);

    // Blit via callback
    BOOL FilterBlit(cSurface& dest, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, BlitMode bm, FILTERBLITPROC fbProc, LPARAM lUserParam) const;
    BOOL FilterBlit(cSurface& dest, FILTERBLITPROC fbProc, LPARAM lUserParam, BlitMode bm = BlitMode::Opaque) const;

    // Matrix blit via callback
    BOOL MatrixFilterBlit(cSurface& dest, int destX, int destY, int srcX, int srcY, int srcWidth, int srcHeight, int mWidth, int mHeight, int mDXCenter, int mDYCenter, MATRIXFILTERBLITPROC fbProc, LPARAM lUserParam) const;

    // Stretch surface to surface
    BOOL Stretch(cSurface& dest, DWORD dwFlags = 0) const;

    // Stretch surface to rectangle
    BOOL Stretch(cSurface& dest, int destX, int destY, int destWidth, int destHeight, BlitMode bm /*= BlitMode::Opaque*/, BlitOp bo = BlitOp::Copy, LPARAM param = 0, DWORD dwFlags = 0) const;

    // Stretch rectangle to rectangle
    BOOL Stretch(cSurface& dest, int destX, int destY, int destWidth, int destHeight, int srcX, int srcY, int srcWidth, int srcHeight, BlitMode bm /*= BlitMode::Opaque*/, BlitOp bo = BlitOp::Copy, LPARAM param = 0, DWORD dwFlags = 0) const;

    // Revert surface horizontally
    BOOL ReverseX();

    // Revert rectangle horizontally
    BOOL ReverseX(int x, int y, int width, int height);

    // Revert surface vertically
    BOOL ReverseY();

    // Revert rectangle vertically
    BOOL ReverseY(int x, int y, int width, int height);

    // Remove empty borders
    BOOL GetMinimizeRect(RECT*);
    BOOL Minimize();
    BOOL Minimize(RECT* r);


    // Blit from screen or memory DC
    static BOOL CaptureDC(HDC srcDC, HDC dstDC, LONG srcX, LONG srcY, LONG dstX, LONG dstY, LONG srcWidth, LONG srcHeight, LONG dstWidth, LONG dstHeight, BOOL bFlushMessage, BOOL bKeepRatio);

    // Fill surface
    BOOL Fill(COLORREF c);
    BOOL Fill(CFillData* fd);
    BOOL Fill(int index);
    BOOL Fill(int R, int G, int B);

    // Fill block
    BOOL Fill(int x, int y, int w, int h, COLORREF c);
    BOOL Fill(int x, int y, int w, int h, CFillData* fd);
    BOOL Fill(int x, int y, int w, int h, int index);
    BOOL Fill(int x, int y, int w, int h, int R, int G, int B);
    BOOL Fill(int x, int y, int w, int h, COLORREF* pColors, DWORD dwFlags);


    // Geometric Primitives

    // 1. Simple routines : call GDI with Surface DC
    BOOL Ellipse(int left, int top, int right, int bottom, int thickness = 1, COLORREF crOutl = BLACK);
    BOOL Ellipse(int left, int top, int right, int bottom, COLORREF crFill, int thickness /*= 0*/, COLORREF crOutl /*= BLACK*/, BOOL Fill = true);
    BOOL Rectangle(int left, int top, int right, int bottom, int thickness = 1, COLORREF crOutl = BLACK);
    BOOL Rectangle(int left, int top, int right, int bottom, COLORREF crFill, int thickness /*= 0*/, COLORREF crOutl /*= BLACK*/, BOOL bFill = true);
    BOOL Polygon(LPPOINT pts, int nPts, int thickness = 1, COLORREF crOutl = BLACK);
    BOOL Polygon(LPPOINT pts, int nPts, COLORREF crFill, int thickness = 0, COLORREF crOutl = BLACK, BOOL bFill = true);
    BOOL Line(int x1, int y1, int x2, int y2, int thickness = 1, COLORREF crOutl = BLACK); 

    // 2. More complex but slower (variable opacity, anti-alias, custom filling, ...)
    BOOL Ellipse(int left, int top, int right, int bottom, int thickness, CFillData* fdOutl, BOOL AntiA = false, BlitMode bm = BlitMode::Opaque, BlitOp bo = BlitOp::Copy, LPARAM param = 0);
    BOOL Ellipse(int left, int top, int right, int bottom, CFillData* fdFill, BOOL AntiA = false, BlitMode bm = BlitMode::Opaque, BlitOp bo = BlitOp::Copy, LPARAM param = 0);
    BOOL Ellipse(int left, int top, int right, int bottom, CFillData* fdFill, int thickness, CFillData* fdOutl, BOOL AntiA = false, BlitMode bm = BlitMode::Opaque, BlitOp bo = BlitOp::Copy, LPARAM param = 0, BOOL Fill = true);
    BOOL Rectangle(int left, int top, int right, int bottom, int thickness, CFillData* fdOutl, BOOL AntiA = false, BlitMode bm = BlitMode::Opaque, BlitOp bo = BlitOp::Copy, LPARAM param = 0);
    BOOL Rectangle(int left, int top, int right, int bottom, CFillData* fdFill, BOOL AntiA = false, BlitMode bm = BlitMode::Opaque, BlitOp bo = BlitOp::Copy, LPARAM param = 0);
    BOOL Rectangle(int left, int top, int right, int bottom, CFillData* fdFill, int thickness, CFillData* fdOutl, BOOL AntiA = false, BlitMode bm = BlitMode::Opaque, BlitOp bo = BlitOp::Copy, LPARAM param = 0, BOOL Fill = true);
    BOOL Polygon(LPPOINT pts, int nPts, int thickness, CFillData* fdOutl, BOOL AntiA = false, BlitMode bm = BlitMode::Opaque, BlitOp bo = BlitOp::Copy, LPARAM param = 0);
    BOOL Polygon(LPPOINT pts, int nPts, CFillData* fdFill, BOOL AntiA = false, BlitMode bm = BlitMode::Opaque, BlitOp bo = BlitOp::Copy, LPARAM param = 0);
    BOOL Polygon(LPPOINT pts, int nPts, CFillData* fdFill, int thickness, CFillData* fdOutl, BOOL AntiA = false, BlitMode bm = BlitMode::Opaque, BlitOp bo = BlitOp::Copy, LPARAM param = 0, BOOL Fill = true);
    BOOL Line(int x1, int y1, int x2, int y2, int thickness, CFillData* fdOutl, BOOL AntiA, BlitMode bm = BlitMode::Opaque, BlitOp bo = BlitOp::Copy, LPARAM param = 0);

    // Filled Primitives
    BOOL FloodFill(int x, int y, int& left, int& top, int& right, int& bottom, COLORREF crFill, BOOL AntiA = false, int tol = 0, BlitMode bm = BlitMode::Opaque, BlitOp bo = BlitOp::Copy, LPARAM param = 0);
    BOOL FloodFill(int x, int y, COLORREF crFill, BOOL AntiA = false,  int tol = 0, BlitMode bm = BlitMode::Opaque, BlitOp bo = BlitOp::Copy, LPARAM param = 0);


    // Rotate surface
    BOOL Rotate(cSurface& dest, double a, BOOL bAA, COLORREF clrFill = 0L, BOOL bTransp = true);  // radian
    BOOL Rotate(cSurface& dest, int a, BOOL bAA, COLORREF clrFill = 0L, BOOL bTransp = true);     // degree
    BOOL Rotate90(cSurface& dest, BOOL b270);

    // Create rotated surface
    BOOL CreateRotatedSurface(cSurface& ps, double a, BOOL bAA, COLORREF clrFill = 0L, BOOL bTransp = true);
    BOOL CreateRotatedSurface(cSurface& ps, int a, BOOL bAA, COLORREF clrFill = 0L, BOOL bTransp = true);

    static void GetSizeOfRotatedRect(int* pWidth, int* pHeight, float angle);


    // Text
    int TextOutA(LPCSTR text, DWORD dwCharCount, int x, int y, DWORD alignMode, LPRECT pClipRc, COLORREF color = 0, HFONT hFnt = (HFONT)NULL, BlitMode bm = BlitMode::Transparent, BlitOp = BlitOp::Copy, LPARAM param = 0, int AntiA = 0);
    int TextOutW(LPCWSTR text, DWORD dwCharCount, int x, int y, DWORD alignMode, LPRECT pClipRc, COLORREF color = 0, HFONT hFnt = (HFONT)NULL, BlitMode bm = BlitMode::Transparent, BlitOp = BlitOp::Copy, LPARAM param = 0, int AntiA = 0);

    int DrawTextA(LPCSTR text, DWORD dwCharCount, LPRECT pRc, DWORD dtflags, COLORREF color = 0, HFONT hFnt = (HFONT)NULL, BlitMode bm = BlitMode::Transparent, BlitOp bo = BlitOp::Copy, LPARAM param = 0, int AntiA = 0, DWORD dwLeftMargin = 0, DWORD dwRightMargin = 0, DWORD dwTabSize = 8);
    int DrawTextW(LPCWSTR text, DWORD dwCharCount, LPRECT pRc, DWORD dtflags, COLORREF color = 0, HFONT hFnt = (HFONT)NULL, BlitMode bm = BlitMode::Transparent, BlitOp bo = BlitOp::Copy, LPARAM param = 0, int AntiA = 0, DWORD dwLeftMargin = 0, DWORD dwRightMargin = 0, DWORD dwTabSize = 8);

    // TODO: Turn this into a inline function
    #undef TextOut
    #undef DrawText

    #ifdef _UNICODE
        #define TextOut TextOutW
        #define DrawText DrawTextW
    #else
        #define TextOut TextOutA
        #define DrawText DrawTextA
    #endif


    // Color / Palette functions
    BOOL IsTransparent();
    BOOL ReplaceColor(COLORREF newColor, COLORREF oldColor);

    // Test collision fine entre deux surfaces
    BOOL IsColliding(cSurface& dest, int xDest, int yDest, int xSrc, int ySrc, int rcWidth, int rcHeight);

    HICON CreateIcon(int iconWidth, int iconHeight, COLORREF transpColor, POINT *pHotSpot);

    // Palette support
    BOOL Indexed();

    BOOL SetPalette(LOGPALETTE* palette, SetPaletteAction action = SetPaletteAction::None);
    BOOL SetPalette(csPalette* pCsPal, SetPaletteAction action = SetPaletteAction::None);
    BOOL SetPalette(cSurface& src, SetPaletteAction action = SetPaletteAction::None);
    BOOL SetPalette(HPALETTE palette, SetPaletteAction action = SetPaletteAction::None);

    void Remap(cSurface& src);
    void Remap(uint8_t* remapTable);

    csPalette* GetPalette();
    UINT       GetPaletteEntries(LPPALETTEENTRY paletteEntry, int index, int nbColors);
    int        GetNearestColorIndex(COLORREF rgb);
    COLORREF   GetRGB(int index);
    int        GetOpaqueBlackIndex();


    // Full screen
    void EnumScreenModes(LPENUMSCREENMODESPROC pProc, void* lParam);
    BOOL SetScreenMode(HWND hWnd, int width, int height, int depth);
    void RestoreWindowedMode(HWND hWnd);
    void CopyScreenModeInfo(cSurface* pSrc);

    BOOL SetAutoVSync(int nAutoVSync);
    BOOL WaitForVBlank();

    // System colors
    static COLORREF GetSysColor(int colorIndex);
    static void OnSysColorChange();

    // Transparent color
    void SetTransparentColor(COLORREF rgb);
    COLORREF GetTransparentColor();
    int GetTransparentColorIndex();
    // void SetOpaque(BOOL bOpaque);

    // Alpha channel
    BOOL      HasAlpha();
    uint8_t*  LockAlpha();
    void      UnlockAlpha();
    int       GetAlphaPitch();
    void      CreateAlpha();
    void      SetAlpha(uint8_t* pAlpha, int nPitch);
    void      AttachAlpha(uint8_t* pAlpha, int nPitch);
    uint8_t*  DetachAlpha(LPLONG pPitch);
    cSurface* GetAlphaSurface();
    void      ReleaseAlphaSurface(cSurface* pAlphaSf);

    // Transparent monochrome mask
    DWORD CreateMask(sMask* pMask, UINT dwFlags);

    // Lost device callback
    void OnLostDevice();
    void AddLostDeviceCallBack(LOSTDEVICECALLBACKPROC pCallback, LPARAM lUserParam);
    void RemoveLostDeviceCallBack(LOSTDEVICECALLBACKPROC pCallback, LPARAM lUserParam);

    // Friend functions
    DllImport friend cSurfaceImplementation* FusionAPI GetSurfaceImplementation(cSurface& cs);
    DllImport friend void FusionAPI SetSurfaceImplementation(cSurface& cs, cSurfaceImplementation* psi);


protected:
    static void BuildSysColorTable();

    // Clipping
    HRGN SetDrawClip(HDC hDC);
    void RestoreDrawClip(HDC hDC, HRGN hOldClipRgn);


private:
    // FIXME: CInputFile aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
    // BOOL LoadPicture(CInputFile* pFile, DWORD bitmapSize, LIFlags loadFlags);
    BOOL LoadDIB(LPBITMAPINFO pBmi, uint8_t* pBits, LIFlags loadFlags);

    cSurfaceImplementation* m_actual;
    POINT origin;

    // System colors
    static BOOL m_bSysColorTab;
    static COLORREF m_sysColorTab[COLOR_GRADIENTINACTIVECAPTION+1];

protected:
    int m_error;
};
