// =======================================================================
// Internal errors
// =======================================================================
#ifndef _CfcError_h
#define	_CfcError_h


// Surface errors
#define	SURFACEERROR_MIN	0x40010000
#define	SURFACEERROR_MAX	0x40020000
enum 
{
	SURFACEERROR_NOERR = 0,
	SURFACEERROR_NOT_SUPPORTED			= SURFACEERROR_MIN,
	SURFACEERROR_SURFACE_NOT_CREATED,
	SURFACEERROR_INTERNAL
};

#endif	// _CfcError_h
