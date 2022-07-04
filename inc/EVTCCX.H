#pragma once



struct infosEventsV2 {
    short code;             // ACE ID
    short flags;            // Flags
    short nParams;          // Number of params
    // TODO: Check if this actualy works V
    // Note: Those are arrays not pointers!
    short* param;           // Param types
    short* paramTitle;      // Param titles displayed in Fusion
};


struct eventInformations2{
    short menu;             // Menu ID
    short string;           // String resource ID
    infosEventsV2 infos;    // Other infos
};


#define EVINFO2_NEXT(p)         ((eventInformations2*)((LPBYTE)p + sizeof(eventInformations2) + p->infos.nParams * 2 * sizeof(short)))
#define	EVINFO2_PARAM(p,n)      (*(LPWORD)((LPBYTE)p + sizeof(eventInformations2) + n * sizeof(short)))
#define	EVINFO2_PARAMTITLE(p,n) (*(LPWORD)((LPBYTE)p + sizeof(eventInformations2) + p->infos.nParams * sizeof(short) + n * sizeof(short)))
