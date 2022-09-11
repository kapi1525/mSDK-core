#pragma once

// Filters.hpp
// This header includes image and sound filter functionality that can be used to load images or sounds etc.

#include "CommonSDK.hpp"
#include "Surface.hpp"
#include "FileIO.hpp"
#include <commdlg.h>        // For OPENFILENAMEA and OPENFILENAMEW structures
#include <mmreg.h>          // For WAVEFORMATEX



// Filter manager base
class DllImport CFilterMgr {
public:
    CFilterMgr();
    virtual ~CFilterMgr();

    static CFilterMgr* CreateInstance();

    // Initialization
    virtual void Initialize(const char* pFilterPath, const char* pFileType, unsigned long dwFlags);
    virtual void Free();
    virtual void Delete();

    // Filter info
    virtual int           GetFilterCount();
    virtual const char*   GetFilterNameA(int nIndex);
    virtual const char*   GetFilterFileNameA(int nIndex);
    virtual unsigned long GetFilterID(int nIndex);
    virtual int           GetFilterIndex(unsigned long dwID);
    virtual const char**  GetFilterExtsA(int nIndex);
    virtual const char**  GetFilterDependenciesA(int nIndex);
    virtual BOOL          CanReadFile(int nIndex, CInputFile* pif);

    // Filter object creation
    virtual void* CreateFilterObject(int nIndex);
    virtual void* CreateFilterObject(unsigned long dwID);
    virtual void* CreateFilterObject(CInputFile* pif);

    // Update
    virtual void RemoveFilter(int nIndex);

    // Unicode
    virtual void Initialize(const wchar_t* pFilterPath, const wchar_t* pFileType, unsigned long dwFlags);
    virtual const wchar_t*  GetFilterNameW(int nIndex);
    virtual const wchar_t*  GetFilterFileNameW(int nIndex);
    virtual const wchar_t** GetFilterExtsW(int nIndex);
    virtual const wchar_t** GetFilterDependenciesW(int nIndex);

    // TODO: Turn thsese into inline functions?
    #ifdef _CFCFILTERMGR_UNICODE_DEFS
        #ifdef _UNICODE
            #define GetFilterName GetFilterNameW
            #define GetFilterFileName GetFilterFileNameW
            #define GetFilterExts GetFilterExtsW
            #define GetFilterDependencies GetFilterDependenciesW
        #else
            #define GetFilterName GetFilterNameA
            #define GetFilterFileName GetFilterFileNameA
            #define GetFilterExts GetFilterExtsA
            #define GetFilterDependencies GetFilterDependenciesA
        #endif
    #endif

    void* m_pFilterArray;
};



// Image filters
class CImageFilter;
class CImageFilterMgr;
class CFilterImpl;
class CInputFile;
class COutputFile;
class CFilterImpl;

// Error codes
enum class ImageFilterError{
    None=0,
    OutOfMemory,
    CannotOpenFile,
    CannotCreateFile,
    CannotWriteFile,
    UnknownFormat,
    UnsuportedFormat,
    NoFileSpecified,
    NotSupported,
    NotOpen,
    UnSupportedDepth,
    InvalidFile,
    LessThan256Color,
    IncorrectDimensions,
    BadParameter,
    UninitializedFilter,
    InvalidPalette,
    AlreadyOpen,
    EndOfFile,
    NotAnimationFilter,
    Unknown,
};

// Filter color caps
#define FLTCOLORCAPS_1  0x0001
#define FLTCOLORCAPS_4  0x0002
#define FLTCOLORCAPS_8  0x0004
#define FLTCOLORCAPS_15 0x0008
#define FLTCOLORCAPS_16 0x0010
#define FLTCOLORCAPS_24 0x0020
#define FLTCOLORCAPS_32 0x0040

// Filter type
#define FLTTYPE_IMAGES 0x0001
#define FLTTYPE_ANIMATIONS 0x0002

// Compression
#define COMPRESSION_DEFAULT (-1)
#define COMPRESSION_MIN 0
#define COMPRESSION_MAX 100

// SaveAnimation flags
#define SAVEANIMFLAG_SEPARATEFRAMES  0x00000001

// Progress proc
typedef BOOL (CALLBACK * PROGRESSPROC) (int p);



class DllImport CImageFilterMgr : public CFilterMgr {
public:
    CImageFilterMgr();
    virtual ~CImageFilterMgr();

    static CImageFilterMgr* CreateInstance();

    // Initialize
    void Initialize(const char* pFilterPath, unsigned long dwFlags);
    void Initialize(const wchar_t* pFilterPath, unsigned long dwFlags);
    virtual void Free();

    // Filter infos
    BOOL DoesFilterSupportImages(int nIndex);
    BOOL DoesFilterSupportAnimations(int nIndex);

protected:
    friend class CImageFilter;
};

class DllImport CImageFilter {
public:
    CImageFilter(CImageFilterMgr* pMgr);
    ~CImageFilter();

    // Customisation
    BOOL UseSpecificFilterIndex(int index);
    BOOL UseSpecificFilterID(unsigned long dwID);

    // Save customization
    void SetCompressionLevel(int nLevel=-1);     // 0 -> 100

    // Picture
    int  Open(const char* fileName);
    int  Open(const wchar_t* fileName);
    int  Open(CInputFile* pf);
    int  PrepareLoading(int width, int height, int depth, LOGPALETTE* pPal);
    int  Load(uint8_t* pData, int width, int height, int pitch, int depth=0, LOGPALETTE* pPal=NULL, uint8_t* pAlpha=NULL, int nAlphaPitch=0);
    int  Save(const char* fileName, uint8_t* pData, int nWidth, int nHeight, int nDepth, int nPitch, LOGPALETTE* pPal, uint8_t* pAlpha=NULL, int nAlphaPitch=0);
    int  Save(const wchar_t* fileName, uint8_t* pData, int nWidth, int nHeight, int nDepth, int nPitch, LOGPALETTE* pPal, uint8_t* pAlpha=NULL, int nAlphaPitch=0);
    void Close();
    int  GetWidth(); 
    int  GetHeight(); 
    int  GetPitch();
    int  GetDepth();
    unsigned long GetDataSize();
    LOGPALETTE*   GetPalette();
    BOOL GetTransparentColor(COLORREF* pTranspColor);
    BOOL ContainsAlphaChannel();

    // Load animation
    BOOL IsAnimation();
    int  GetNumberOfFrames();
    int  GetCurrentFrame();
    int  GetFrameDelay(int frameIndex=-1);
    unsigned long GetAnimDuration();
    uint8_t*      GetUserInfo();
    unsigned long GetUserInfoSize();

    void Restart();
    int  GoToImage(uint8_t* pData, int pitch, int n);
    void GetUpdateRect(LPRECT pRc);
    int  GetLoopCount();
    int  GetLoopFrame();

    // Save animation
    int CreateAnimation(const char* fname, int width, int height, int depth, int nFrames, int msFrameDuration, int nLoopCount = 1, int nLoopFrame = 0, uint8_t* pUserInfo = NULL, unsigned long dwUserInfoSize = 0);
    int CreateAnimation(const wchar_t* fname, int width, int height, int depth, int nFrames, int msFrameDuration, int nLoopCount = 1, int nLoopFrame = 0, uint8_t* pUserInfo = NULL, unsigned long dwUserInfoSize = 0);
    int CreateAnimation(COutputFile* pfout, int width, int height, int depth, int nFrames, int msFrameDuration, int nLoopCount = 1, int nLoopFrame = 0, uint8_t* pUserInfo = NULL, unsigned long dwUserInfoSize = 0);
    int SaveAnimationFrame(uint8_t* pData, int width, int height, int pitch, int depth, LOGPALETTE* pPal, uint8_t* pAlpha, int nAlphaPitch, int msFrameDuration, unsigned long dwFlags);
    int SaveAnimationFrame(uint8_t* pData, uint8_t* pPrevData, int width, int height, int pitch, int depth, LOGPALETTE* pPal, uint8_t* pAlpha, uint8_t* pPrevAlpha, int nAlphaPitch, int msFrameDuration, unsigned long dwFlags);
    void AddPreviousFrameDuration(int msFrameDuration);
    unsigned long GetCurrentSaveAnimSize();

    // File handling
    void SetProgressCallBack(PROGRESSPROC pProc);

    // Filter info
    unsigned long  GetFilterID();
    const char*    GetFilterNameA();
    int            GetFilterIndex();
    unsigned long  GetFilterColorCaps();
    BOOL           CanSave();
    BOOL           CanSaveAnim();
    const wchar_t* GetFilterNameW();

protected:
    int ReadHeader();

    // Image filter manager
    CImageFilterMgr* m_pMgr;

    // Input
    CInputFile* m_pfin;
    BOOL m_bAutoDeletePfin;
    BOOL m_bPrepared;

    // Output
    COutputFile* m_pfout;
    BOOL m_bAutoDeletePfout;
    uint8_t* m_prevBuf;

    // Progress callback
    PROGRESSPROC m_pProgressProc;

    // Filter implementation
    CFilterImpl* m_pFilterImpl;
};



// Image filters
#define IMPORT_IMAGE_USESURFACEDEPTH    0x0001
#define IMPORT_IMAGE_USESURFACEPALETTE  0x0002

#define IMPORT_IMAGE_FIRSTPIXELTRANSP   0x0004
#define IMPORT_IMAGE_OPAQUE             0x0008

DllImport BOOL FusionAPI ImportImageA(CImageFilterMgr* pImgMgr, const char* fileName, cSurface* psf, unsigned long* pDWFilterID, unsigned long dwFlags);
DllImport BOOL FusionAPI ImportImageFromInputFile(CImageFilterMgr* pImgMgr, CInputFile* pf, cSurface* psf, unsigned long* pDWFilterID, unsigned long dwFlags);
DllImport BOOL FusionAPI CanImportImageA(CImageFilterMgr* pImgMgr, const char* fileName);
DllImport BOOL FusionAPI ImportPaletteA(CImageFilterMgr* pImgMgr, const char* fileName, LOGPALETTE* pPal);
DllImport BOOL FusionAPI ExportImageA(CImageFilterMgr* pImgMgr, const char* pFileName, cSurface* psf, unsigned long dwFilterID);
DllImport BOOL FusionAPI CanImportAnimationA(CImageFilterMgr* pImgMgr, const char* fileName, BOOL* bNeedConversion);
DllImport BOOL FusionAPI BeginAnimationImportA(CImageFilterMgr* pImgMgr, const char* pFileName, cSurface* psf, CImageFilter* pFilter, unsigned long dwFlags);
DllImport BOOL FusionAPI GetNextAnimationFrame(CImageFilterMgr* pImgMgr, cSurface* psf, CImageFilter* pFilter);
DllImport void FusionAPI EndAnimationImport(CImageFilterMgr* pImgMgr, CImageFilter* pFilter);
DllImport BOOL FusionAPI BeginAnimationExportA(CImageFilterMgr* pImgMgr, const char* pFileName, cSurface* psf, CImageFilter* pFilter, int nFrames, int msFrameDuration, int nLoopCount, int nLoopFrame);
DllImport BOOL FusionAPI ExportNextFrame(CImageFilterMgr* pImgMgr, cSurface* psf, CImageFilter* pFilter, int msFrameDuration);
DllImport void FusionAPI EndAnimationExport(CImageFilterMgr* pImgMgr, CImageFilter* pFilter);

DllImport BOOL FusionAPI ImportImageW(CImageFilterMgr* pImgMgr, const wchar_t* fileName, cSurface* psf, unsigned long* pDWFilterID, unsigned long dwFlags);
DllImport BOOL FusionAPI CanImportImageW(CImageFilterMgr* pImgMgr, const wchar_t* fileName);
DllImport BOOL FusionAPI ImportPaletteW(CImageFilterMgr* pImgMgr, const wchar_t* fileName, LOGPALETTE* pPal);
DllImport BOOL FusionAPI ExportImageW(CImageFilterMgr* pImgMgr, const wchar_t* pFileName, cSurface* psf, unsigned long dwFilterID);
DllImport BOOL FusionAPI CanImportAnimationW(CImageFilterMgr* pImgMgr, const wchar_t* fileName, BOOL* bNeedConversion);
DllImport BOOL FusionAPI BeginAnimationImportW(CImageFilterMgr* pImgMgr, const wchar_t* pFileName, cSurface* psf, CImageFilter* pFilter, unsigned long dwFlags);
DllImport BOOL FusionAPI BeginAnimationExportW(CImageFilterMgr* pImgMgr, const wchar_t* pFileName, cSurface* psf, CImageFilter* pFilter, int nFrames, int msFrameDuration, int nLoopCount, int nLoopFrame);

#ifdef _UNICODE
    #define ImportImage ImportImageW
    #define CanImportImage CanImportImageW
    #define ImportPalette ImportPaletteW
    #define ExportImage ExportImageW
    #define CanImportAnimation CanImportAnimationW
    #define BeginAnimationImport BeginAnimationImportW
    #define BeginAnimationExport BeginAnimationExportW
#else
    #define ImportImage ImportImageA
    #define CanImportImage CanImportImageA
    #define ImportPalette ImportPaletteA
    #define ExportImage ExportImageA
    #define CanImportAnimation CanImportAnimationA
    #define BeginAnimationImport BeginAnimationImportA
    #define BeginAnimationExport BeginAnimationExportA
#endif

// Picture selector
#define PICSEL_IMAGES 0x0001
#define PICSEL_ANIMS  0x0002

DllImport BOOL FusionAPI ChoosePictureA(OPENFILENAMEA* pOFN, BOOL bOpen, CImageFilterMgr* pImgMgr, unsigned long* pDWFilterID, unsigned long dwFlags);
DllImport BOOL FusionAPI ChoosePictureW(OPENFILENAMEW* pOFN, BOOL bOpen, CImageFilterMgr* pImgMgr, unsigned long* pDWFilterID, unsigned long dwFlags);

#ifdef _UNICODE
    #define ChoosePicture ChoosePictureW
#else
    #define ChoosePicture ChoosePictureA
#endif



// Sound conversion
typedef void (*SAMPLECONVERTPROC)(void* pSrc, void* pDst, unsigned long nSrcSample, unsigned long nSrcFreqSample, unsigned long nDstWrite);

extern const SAMPLECONVERTPROC SampleConversionProcArray[];

#define SMP_CONV_8BITMONO_TO_8BITSTEREO     0
#define SMP_CONV_8BITMONO_TO_16BITMONO      1
#define SMP_CONV_8BITMONO_TO_16BITSTEREO    2
#define SMP_CONV_8BITSTEREO_TO_8BITMONO     3
#define SMP_CONV_8BITSTEREO_TO_16BITMONO    4
#define SMP_CONV_8BITSTEREO_TO_16BITSTEREO  5
#define SMP_CONV_16BITMONO_TO_16BITSTEREO   6
#define SMP_CONV_16BITMONO_TO_8BITMONO      7
#define SMP_CONV_16BITMONO_TO_8BITSTEREO    8
#define SMP_CONV_16BITSTEREO_TO_16BITMONO   9
#define SMP_CONV_16BITSTEREO_TO_8BITMONO    10
#define SMP_CONV_16BITSTEREO_TO_8BITSTEREO  11


SAMPLECONVERTPROC GetConversionFunction(WAVEFORMATEX* pSrcFormat, WAVEFORMATEX* pDstFormat);


enum class SoundFilterError {
    None = 0,
    OutOfMemory,
    BadWaveFormat,
    CannotOpenFile,
    NotWaveFile,
    CannotReadFile,
    CorruptWaveFile,
    BadWaveFileFormat,
    CannotOpenMemStream,
    NotAIFFFILE,    // ????????
    Unknown
};


class DllImport CSoundFilter {
public:
    CSoundFilter();
    virtual ~CSoundFilter();

    virtual void Delete() = 0;

    virtual int  Open(CInputFile* pf) = 0;
    virtual void Close() = 0;

    virtual unsigned long GetLength() = 0;
    virtual unsigned long GetPos() = 0;
    virtual BOOL  SetPos(unsigned long dwPos) = 0;
    virtual int   ReadData(uint8_t* lpDstBuffer, unsigned long dwBufSize, unsigned long* dwRead) = 0;

    virtual void SetOutputFormat(WAVEFORMATEX* pStreamFormat);

    WAVEFORMATEX* GetInputFormat() { return(&m_WaveFormatIn); }
    WAVEFORMATEX* GetOutputFormat() { return(&m_WaveFormatOut); }
    int Read(uint8_t* pDest, unsigned long dwBufSize, unsigned long* pDWRead);

protected:

    WAVEFORMATEX m_WaveFormatOut;
    WAVEFORMATEX m_WaveFormatIn;

    BOOL m_bDoConvert;
    float m_fSizeRatio;
    uint8_t* m_lpReadWaveData;
    SAMPLECONVERTPROC m_pConvertProc;
    unsigned long m_nSrcFreqSample;
    unsigned long m_nDstWrite;
};


class DllImport CSoundFilterMgr : public CFilterMgr {
public:
    CSoundFilterMgr();
    virtual ~CSoundFilterMgr();

    static CSoundFilterMgr* CreateInstance();

    // List APIs and Filters
    void Initialize(const char* pFilterPath, unsigned long dwFlags);
    void Initialize(const wchar_t* pFilterPath, unsigned long dwFlags);
    virtual void Free();

    // Create sound manager
    CSoundManager* CreateSoundManager(HWND hWnd);

    // Create sound filter
    CSoundFilter* CreateFilter(CInputFile* pf);
};
