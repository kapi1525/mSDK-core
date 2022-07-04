#pragma once



// This header contains some usefull macros some of those are used by rc resource file

// RCDATAs
#define	KPX_MAGICNUMBER		200			// Magic number
#define VITALIZE_ID			210

// Icon (v2)
#define	KPX_TOOLICON		200			// Icon

// Strings
#define	KPX_MARK			1			// Code "MF2"
#define	KPX_NAME			2			// Default object name
#define	KPX_HIDDEN			4			// If hidden KCX
#define	EXO_ICON			400			// Icon in New Object window
#define	EXO_IMAGE			401			// Image in frame editor (optional)

// kpxGetInfos
enum {
    KGI_VERSION,						// Version (required)
    KGI_NOTUSED,						// Not used
    KGI_PLUGIN,							// Version for plug-in (required)
    KGI_MULTIPLESUBTYPE,				// Allow sub-types
    KGI_NOTUSED2,						// Reserved
    KGI_ATX_CONTROL,					// Not used
    KGI_PRODUCT,						// Minimum product the extension is compatible with
    KGI_BUILD,							// Minimum build the extension is compatible with
    KGI_UNICODE,						// Returns TRUE if the extension is in Unicode
};

#define	EXT_VERSION1			0x200	// C&C, TGF
#define	EXT_VERSION2			0x300	// MMF1
#define	EXT_VERSION3			0x400	// MMF2

#define	EXT_PLUGIN_VERSION1		0x100
#define	EXT_PLUGIN_VERSION2		0x200


// Values to return for KGI_BUILD
enum {
    PRODUCT_VERSION_HOME=1,					// TGF or above
    PRODUCT_VERSION_STANDARD,				// MMF Standard or above
    PRODUCT_VERSION_DEV,					// MMF Developer or above
    PRODUCT_VERSION_DELUXEGOLDCOLLECTOR,	// :)
};

// GetFilters
#define	GETFILTERS_IMAGES		0x0001
#define	GETFILTERS_SOUNDS		0x0002


// Return codes
#define	CREATE_OBJECT_OK	0
#define	CREATE_OBJECT_ERROR	-1
