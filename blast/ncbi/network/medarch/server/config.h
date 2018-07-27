/*
*
*
* RCS Modification History:
* $Log: config.h,v $
* Revision 6.0  1997/08/25 18:36:06  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:54:34  epstein
* add RCS log revision history
*
*/


#define	ENV_DATABASE	"MEDLINE_DATABASE"
#define	ENV_USER	"MEDLINE_USER"
#define	ENV_SERVER	"MEDLINE_SERVER"
#define ENV_PASSWORD	"MEDLINE_PASSWORD"
#define	ENV_CONFIGFILE	"MEDLINE_RETRIEVAL_CONFIG"

#define	DEF_SERVER	"MEDLINE"
#define	DEF_DATABASE	"medline"
#define	DEF_PASSWORD	NULL
#define	DEF_USER	NULL
#ifdef	MEDLINE_CONFIG_FILE
#define	DEF_CONFIGFILE	MEDLINE_CONFIG_FILE
#else
#define	DEF_CONFIGFILE	"/sun/lib/medline.cfg"
#endif

#ifndef	MAX_OUTPUT_STREAMS
#define	MAX_OUTPUT_STREAMS	15
#endif

#ifndef	DEFAULT_OUTPUT_FORMAT
#define	DEFAULT_OUTPUT_FORMAT	"asn1"
#endif
#ifndef	DEFAULT_OUTPUT_FILE
#define	DEFAULT_OUTPUT_FILE	"-"
#endif

extern	char	*programName;
extern	char	*flagDatabase;
extern	char	*flagServer;
extern	char	*flagUser;
extern	char	*flagPassword;
extern	char	*flagConfigFile;
extern	short	 flagISO;
extern	short	 flagVerbose;
extern	short	 flagDebug;
extern	short	 flagTrace;
extern	short	 flagReload;
extern	int	 argumentCount;
extern	char	**argumentList;

#ifndef	FALSE
#define	FALSE	(1==0)
#endif
#ifndef	TRUE
#define	TRUE	(1==1)
#endif

#define	EXIT_STATUS_OK		0
#define	EXIT_STATUS_USAGE	1
#define	EXIT_STATUS_ERROR	2
