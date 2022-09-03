#pragma once



struct infosEventsV2 {
    short code;             // ACE ID
    short flags;            // Flags
    short nParams;          // Number of params
    short param[];          // Param types and starting from index nParams param titles displayed in Fusion
    // short paramTitle[];  // Having multiple 0 length arrays in struct is illegal
};


struct eventInformations2{
    short menu;             // Menu ID
    short string;           // String resource ID
    infosEventsV2 infos;    // Other infos
};


#define EVINFO2_NEXT(p)         ((eventInformations2*)((LPBYTE)p + sizeof(eventInformations2) + p->infos.nParams * 2 * sizeof(short)))
#define	EVINFO2_PARAM(p,n)      (*(LPWORD)((LPBYTE)p + sizeof(eventInformations2) + n * sizeof(short)))
#define	EVINFO2_PARAMTITLE(p,n) (*(LPWORD)((LPBYTE)p + sizeof(eventInformations2) + p->infos.nParams * sizeof(short) + n * sizeof(short)))
