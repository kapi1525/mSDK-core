
#ifndef _EvtCCX_h
#define _EvtCCX_h


// Structure V2 : nombre de parametres variable...
typedef struct infosEventsV2 {
	short 	code;					// Le numero de type + code event
	short	flags;					// Les flags a mettre dans l'event
	short	nParams;				// Le nombre de parametres
//	short	param[X];				// Le type des parametres
//	short	paramTitle[X];			// Le titre de chacun des params
} infosEventsV2;
typedef	infosEventsV2 FAR *			LPINFOEVENTSV2;


// Event information structure
typedef struct {
	short			menu;			// Menu identifier
	short			string;			// String identifier
	infosEventsV2	infos;			// Sub structure
} eventInformations2;
typedef	eventInformations2 FAR *	LPEVENTINFOS2;

#define EVINFO2_NEXT(p)			((LPEVENTINFOS2)((LPBYTE)p + sizeof(eventInformations2) + p->infos.nParams * 2 * sizeof(short)))
#define	EVINFO2_PARAM(p,n)		(*(LPWORD)((LPBYTE)p + sizeof(eventInformations2) + n * sizeof(short)))
#define	EVINFO2_PARAMTITLE(p,n)	(*(LPWORD)((LPBYTE)p + sizeof(eventInformations2) + p->infos.nParams * sizeof(short) + n * sizeof(short)))

#endif // _EvtCCX_h
