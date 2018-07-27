/*   ncbierr.h
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
* $Id: ncbierr.h,v 6.7 2001/07/12 21:58:03 vakatov Exp $
*
* $Revision: 6.7 $
*
* Authors:  Schuler, Sirotkin (UserErr stuff)
*
* Version Creation Date:   9-19-91
*
* File Description:  Error handling functions
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name         Description of modification
* -------- -----------  -----------------------------------------------------
* 12-10-93 Schuler      Major Revision.  New APIs include:  ErrPostEx,
*                       ErrSetMessageLevel, ErrSetFatalLevel,
* 10-20-94 Schuler      Rearranged some things to make CodeCenter happy
* 11-18-94 Schuler      Changed return type of Get/SetXXXLevel() family
*                       of functions from int to ErrSev to eliminate warnings
*
* $Log: ncbierr.h,v $
* Revision 6.7  2001/07/12 21:58:03  vakatov
* Introduced #NLM_NO_TRACE (define it if you need to prohibit macro #TRACE)
*
* Revision 6.6  2001/02/08 22:27:04  vakatov
* + Nlm_CallErrHandlerOnly() -- to allow processing of posted
* err.messages by the hook only.
*
* Revision 6.5  1999/10/01 14:41:39  kans
* added SEV_REJECT between SEV_ERROR and SEV_FATAL
*
* Revision 6.4  1999/05/20 20:07:03  sirotkin
* ErrPostItem bullet proofed as per vakatov
*
* Revision 6.3  1998/10/22 16:52:41  shavirin
* Added function *Nlm_GetErrLongText able to retrieve long error messages
*
* Revision 6.2  1997/11/04 19:08:36  ostell
* added ErrGetUserStrings() to eliminate direct access to ErrDesc.userstr
* so this could become a thread specific chain
*
* Revision 6.1  1997/08/27 20:40:27  kans
* added ErrPathReset
*
* Revision 6.0  1997/08/25 18:15:27  madden
* Revision changed to 6.0
*
* Revision 5.2  1996/12/03 21:48:33  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
 * Revision 5.1  1996/11/25  19:03:33  vakatov
 * Moved 'busy' flag to the one-per-thread ErrMsgRoot() structure
 * (it was static before and caused a lot of MT-mess).
 * Added Nlm_ErrGetLogfile() and Nlm_AbnormalExitPure() f-protos.
 * Added ASSERT_HARD() and VERIFY_HARD() macros calling
 * Nlm_AbnormalExitPure() instead of Nlm_AbnormalExit().
 *
 * Revision 5.0  1996/05/28  13:18:57  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:46:50  ostell
 * force revision to 4.0
 *
 * Revision 2.30  1995/05/15  18:45:58  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#ifndef _NCBIERR_
#define _NCBIERR_

#include <ncbilcl.h>
#include <ncbistd.h>
#include <ncbimisc.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" {
#endif

#ifndef THIS_FILE
#define THIS_FILE  __FILE__
#endif
#ifndef THIS_MODULE
#define THIS_MODULE  ""
#endif

#ifdef OS_UNIX
#define ERRTEXT_MAX  1024
#else
#define ERRTEXT_MAX  512
#endif
#define CODESTR_MAX  64
#define MODSTR_MAX   32
#define SRCFILE_MAX  92

#ifdef _DEBUG
#define DBFLAG 1
#else
#define DBFLAG 0
#endif


#define E_NoMemory      1
#define E_File          2
#define E_FOpen     3
#define E_FRead     4
#define E_FWrite    5
#define E_CdEject   9
#define E_Math          4
#define E_SGML          3
#define E_Programmer  999


typedef struct ErrMsgNode
{
	struct ErrMsgNode *next;
	struct ErrMsgNode *list;
	const char *name;
	int code;
	int sev;
	long cofs, clen;
	long tofs, tlen;
	const char *cstr;
	const char *tstr;
} ErrMsgNode;
typedef const ErrMsgNode *ErrMsgNodePtr;

typedef struct ErrMsgRoot
{
	struct ErrMsgRoot *next;
	struct ErrMsgNode *list;
	const char *name;
        int   busy;
} ErrMsgRoot;
typedef const ErrMsgRoot *ErrMsgRootPtr;

typedef struct ErrDesc
{
	short severity;
	short context;  /* OBSOLETE */
	char  module[MODSTR_MAX];
	int   errcode;
	int   subcode;
	char  codestr[CODESTR_MAX];
	char  srcfile[SRCFILE_MAX];
	int   srcline;
	char  errtext[ERRTEXT_MAX];
	const ValNode *userstr;
	const ErrMsgRoot *root;
	const ErrMsgNode *node;
	Nlm_Uint2 entityID, itemID, itemtype;  /* for gather.c messages */
} ErrDesc;
typedef const ErrDesc *ErrDescPtr;

typedef struct ErrOpts
{
	unsigned long flags;
	short log_level;
	short msg_level;
	short die_level;
	short actopt;
	short logopt;
} ErrOpts;
typedef const ErrOpts *ErrOptsPtr;


/* prototype for user-supplied error handler */
typedef int (LIBCALLBACK *ErrHookProc)(const ErrDesc *err);

/***************************************************************************\
|                           POSTING AN ERROR                                |
\***************************************************************************/

enum _ErrSev { SEV_NONE=0, SEV_INFO, SEV_WARNING, SEV_ERROR, SEV_REJECT, SEV_FATAL, SEV_MAX };
typedef enum _ErrSev ErrSev;

#define SEV_MIN  SEV_INFO


NLM_EXTERN void CDECL Nlm_ErrPost VPROTO((int ctx, int code, const char *fmt, ...));
NLM_EXTERN int CDECL Nlm_ErrPostEx VPROTO((ErrSev sev, int lev1, int lev2, const char *fmt, ...));
NLM_EXTERN int LIBCALL Nlm_ErrPostStr PROTO((ErrSev sev, int lev1, int lev2, const char *str));
NLM_EXTERN void CDECL Nlm_ErrLogPrintf VPROTO((const char *fmt, ...));
NLM_EXTERN void LIBCALL Nlm_ErrLogPrintStr PROTO((const char *str));
NLM_EXTERN int LIBCALL Nlm_ErrSetContext PROTO((const char *ctx, const char *fn, int ln, int db, Nlm_Uint2 entityID, Nlm_Uint2 itemID, Nlm_Uint2 itemtype));

#define ErrPost	\
	(Nlm_ErrSetContext(THIS_MODULE,THIS_FILE,__LINE__,DBFLAG,0,0,0)) ? (void)0 : \
	Nlm_ErrPost

#define ErrPostEx	\
	(Nlm_ErrSetContext(THIS_MODULE,THIS_FILE,__LINE__,DBFLAG,0,0,0)) ? 0 : \
	Nlm_ErrPostEx

#define ErrPostStr  \
	(Nlm_ErrSetContext(THIS_MODULE,THIS_FILE,__LINE__,DBFLAG,0,0,0)) ? 0 : \
	Nlm_ErrPostStr

#define ErrPostItem  \
	( Nlm_ErrSetContext(THIS_MODULE,THIS_FILE,__LINE__,DBFLAG, \
gcp ? gcp->entityID : 0, \
gcp ? gcp->itemID : 0, \
gcp ? gcp->thistype : 0)) ? 0 : \
	Nlm_ErrPostEx

#define ErrLogPrintf  Nlm_ErrLogPrintf
#define ErrLogPrintStr Nlm_ErrLogPrintStr

/***************************************************************************\
|                     FETCHING AND REPORTING ERRORS                         |
\***************************************************************************/
NLM_EXTERN int LIBCALL Nlm_ErrFetch PROTO((ErrDesc FAR *err));
NLM_EXTERN int LIBCALL Nlm_ErrCopy PROTO((ErrDesc FAR *err));
NLM_EXTERN void LIBCALL Nlm_ErrClear PROTO((void));
NLM_EXTERN int LIBCALL Nlm_ErrShow PROTO((void));
/**************************************************************************
*
*  ErrGetUserStrings(ErrDescPtr err)
*    returns ValNodePtr to chain of user strings for this thread
*
***************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL Nlm_ErrGetUserStrings PROTO((ErrDescPtr err));

#define ErrShow     Nlm_ErrShow
#define ErrFetch    Nlm_ErrFetch
#define ErrClear    Nlm_ErrClear
#define ErrCopy     Nlm_ErrCopy
#define ErrPeek()   Nlm_ErrCopy(NULL)
#define ErrGetUserStrings Nlm_ErrGetUserStrings


/***************************************************************************\
|                           MESSAGE FILE SUPPORT                            |
\***************************************************************************/

/** Don't use these functions **/
NLM_EXTERN ErrMsgRootPtr LIBCALL ErrGetMsgRoot PROTO((const char *context));
NLM_EXTERN const char* LIBCALL ErrGetExplanation PROTO((ErrMsgRootPtr idx, ErrMsgNodePtr item));

/***************************************************************************\
|                             CUSTOMIZATION                                 |
\***************************************************************************/

typedef Nlm_Uint1 ErrStrId;

NLM_EXTERN int LIBCALL Nlm_ErrSetLogfile PROTO((const char *filename, unsigned long flags));
NLM_EXTERN const char* LIBCALL Nlm_ErrGetLogfile PROTO(( void ));
NLM_EXTERN ErrHookProc LIBCALL Nlm_ErrSetHandler PROTO((ErrHookProc));
NLM_EXTERN Nlm_Boolean LIBCALL Nlm_CallErrHandlerOnly(Nlm_Boolean hook_only);
NLM_EXTERN ErrStrId LIBCALL Nlm_ErrUserInstall PROTO((const char *msg, ErrStrId magic_cookie));
NLM_EXTERN Nlm_Boolean LIBCALL Nlm_ErrUserDelete PROTO((ErrStrId magic_cookie));
NLM_EXTERN void LIBCALL Nlm_ErrUserClear PROTO((void));
NLM_EXTERN ErrSev LIBCALL ErrSetLogLevel PROTO((ErrSev level));
NLM_EXTERN ErrSev LIBCALL ErrGetLogLevel PROTO((void));
NLM_EXTERN ErrSev LIBCALL ErrSetMessageLevel PROTO((ErrSev level));
NLM_EXTERN ErrSev LIBCALL ErrGetMessageLevel PROTO((void));
NLM_EXTERN ErrSev LIBCALL ErrSetFatalLevel PROTO((ErrSev level));
NLM_EXTERN ErrSev LIBCALL ErrGetFatalLevel PROTO((void));
NLM_EXTERN unsigned long LIBCALL ErrSetOptFlags PROTO((unsigned long flags));
NLM_EXTERN unsigned long LIBCALL ErrClearOptFlags PROTO((unsigned long flags));
NLM_EXTERN unsigned long LIBCALL ErrTestOptFlags PROTO((unsigned long flags));
NLM_EXTERN void LIBCALL ErrSaveOptions PROTO((ErrOpts *opts));
NLM_EXTERN void LIBCALL ErrRestoreOptions PROTO((const ErrOpts *opts));
NLM_EXTERN void LIBCALL Nlm_ErrPathReset PROTO((void));
NLM_EXTERN char *Nlm_GetErrLongText (char *module, 
                                     int errcode, int subcode);
/* Error Option (EO) flags */
#define EO_LOG_SEVERITY  0x00000001L
#define EO_LOG_CODES     0x00000002L
#define EO_LOG_FILELINE  0x00000004L
#define EO_LOG_USERSTR   0x00000008L
#define EO_LOG_ERRTEXT   0x00000010L
#define EO_LOG_MSGTEXT   0x00000020L
#define EO_MSG_SEVERITY  0x00000100L
#define EO_MSG_CODES     0x00000200L
#define EO_MSG_FILELINE  0x00000400L
#define EO_MSG_USERSTR   0x00000800L
#define EO_MSG_ERRTEXT   0x00001000L
#define EO_MSG_MSGTEXT   0x00002000L
#define EO_LOGTO_STDOUT  0x00010000L
#define EO_LOGTO_STDERR  0x00020000L
#define EO_LOGTO_TRACE   0x00040000L
#define EO_LOGTO_USRFILE 0x00080000L
#define EO_XLATE_CODES   0x01000000L
#define EO_WAIT_KEY      0x02000000L
#define EO_PROMPT_ABORT  0x04000000L
#define EO_BEEP          0x08000000L

#define EO_ALL_FLAGS     0x0F0F3F3FL

#define EO_SHOW_SEVERITY (EO_LOG_SEVERITY | EO_MSG_SEVERITY)
#define EO_SHOW_CODES    (EO_LOG_CODES | EO_MSG_CODES)
#define EO_SHOW_FILELINE (EO_LOG_FILELINE | EO_MSG_FILELINE)
#define EO_SHOW_USERSTR  (EO_LOG_USERSTR | EO_MSG_USERSTR)
#define EO_SHOW_ERRTEXT  (EO_LOG_ERRTEXT | EO_MSG_ERRTEXT)
#define EO_SHOW_MSGTEXT  (EO_LOG_MSGTEXT | EO_MSG_MSGTEXT)

#define EO_DEFAULTS      (EO_SHOW_SEVERITY | EO_LOG_CODES | EO_XLATE_CODES | \
				EO_SHOW_USERSTR | EO_SHOW_ERRTEXT)

/* flags for ErrSetLogfile */
#define ELOG_APPEND		 0x00000001
#define ELOG_BANNER      0x00000002
#define ELOG_NOCREATE    0x00000004

#define ErrSetHandler Nlm_ErrSetHandler
#define ErrUserInstall Nlm_ErrUserInstall
#define ErrUserDelete  Nlm_ErrUserDelete
#define ErrUserClear   Nlm_ErrUserClear
#define ErrSetLogfile   Nlm_ErrSetLogfile
#define ErrSetLog(x)  Nlm_ErrSetLogfile((x),ELOG_BANNER|ELOG_APPEND)
#define ErrPathReset Nlm_ErrPathReset

/***************************************************************************\
|                               DEBUGGING                                   |
\***************************************************************************/
NLM_EXTERN void LIBCALL Nlm_AssertionFailed PROTO((const char*, const char*,const char*,int));
NLM_EXTERN void LIBCALL Nlm_TraceStr PROTO((const char*));
NLM_EXTERN void CDECL   Nlm_Trace VPROTO((const char*, ...));
NLM_EXTERN void LIBCALL Nlm_AbnormalExit PROTO((int));
NLM_EXTERN void LIBCALL Nlm_AbnormalExitPure PROTO((int));

#define AbnormalExit  Nlm_AbnormalExit

#ifdef _DEBUG

#if !defined(TRACE)  &&  !defined(NLM_NO_TRACE)
#define TRACE	Nlm_Trace
#endif
#ifndef ASSERT
#define ASSERT(expr)  ((expr) ? \
	(void)0 : Nlm_AssertionFailed(#expr,THIS_MODULE,THIS_FILE,__LINE__))
#endif
#ifndef ASSERT_HARD
#define ASSERT_HARD(expr)  ((expr) ? (void)0 : Nlm_AbnormalExitPure(-111))
#endif
#ifndef VERIFY
#define VERIFY(expr)  ASSERT(expr)
#endif
#ifndef VERIFY_HARD
#define VERIFY_HARD(expr)  ASSERT_HARD(expr)
#endif

#else /* ! _DEBUG */

#if !defined(TRACE)  &&  !defined(NLM_NO_TRACE)
#define TRACE	1 ? (void)0 : Nlm_Trace
#endif
#ifndef ASSERT
#define ASSERT(expr)	((void)0)
#endif
#ifndef ASSERT_HARD
#define ASSERT_HARD(expr)	((void)0)
#endif
#ifndef VERIFY
#define VERIFY(expr)	((void)(expr))
#endif
#ifndef VERIFY_HARD
#define VERIFY_HARD(expr)	((void)(expr))
#endif

#endif  /* ! _DEBUG */

/********************* OBSOLETE STUFF BELOW THIS LINE **********************/

/* actopt codes */
#define ERR_CONTINUE  1
#define ERR_IGNORE    2
#define ERR_ADVISE    3
#define ERR_ABORT     4
#define ERR_PROMPT    5
#define ERR_TEE       6

/* logopt codes */
#define ERR_LOG_ON    1
#define ERR_LOG_OFF   2

NLM_EXTERN void LIBCALL Nlm_ErrGetOpts PROTO((short * actopt, short * logopt));
NLM_EXTERN void LIBCALL Nlm_ErrSetOpts PROTO((short actopt, short logopt));
#define ErrGetOpts  Nlm_ErrGetOpts
#define ErrSetOpts  Nlm_ErrSetOpts

/* error context codes */
#define CTX_DEBUG      0
#define CTX_UNKNOWN    1
#define CTX_ERRNO      2
#define CTX_NCBICORE   64
#define CTX_NCBIASN1   65
#define CTX_NCBICD	   66
#define CTX_NCBIOBJ    67
#define CTX_NCBILMA    68
#define CTX_NCBIGBPARSE 69
#define CTX_NCBIPIRPARSE 70
#define CTX_NCBI2GB    71
#define CTX_NCBIBB2ASN 72
#define CTX_NCBIMATH   73
#define CTX_NCBIMED2ASN 74
#define CTX_NCBISEQENTRY 75
#define CTX_NCBISEQPORT   200
#define CTX_NCBIIDLOAD 300
#define CTX_NCBIIDPROCESS 301
#define CTX_NCBIIDRETRIEVE 302
#define CTX_NCBIAUTHINPARSE 303
#define CTX_KB2ASN 304

#define CTX_RESERVED   10000
/* context codes > 10000 are available for application use */
#define CTX_USER	   32767    /* default user application context */

/* error codes for CTX_NCBICORE */
#define CORE_UNKNOWN    1
#define CORE_NULLPTR    2    /* NULL pointer passed as an argument */
#define CORE_NULLHND    3    /* NULL handle passed as an argument */
#define CORE_MEMORY     4    /* Memory allocation failure */
#define CORE_BAD_COOKIE 5    /* ErrPost User install misused */
#define CORE_BAD_SGML   6    /* SGML entity or range error */

#define CORE_FILE_ACCESS  10  /* Error accessing file */
#define CORE_FILE_CREATE  11  /* Error creating file */
#define CORE_FILE_OPEN    12  /* Error opening file */
#define CORE_FILE_READ    13  /* Error reading file */
#define CORE_FILE_WRITE   14  /* Error writing file */
#define CORE_FILE_ERROR   15  /* any other file I/O error */

#define ERRPOST(x)	\
	Nlm_ErrSetContext(THIS_MODULE,THIS_FILE,__LINE__,DBFLAG,0,0,0), \
	Nlm_ErrPost x

#define Nlm_ErrSetLog(x)  Nlm_ErrSetLogfile((x),ELOG_BANNER|ELOG_APPEND)

#define EO_LOG_STDOUT  EO_LOGTO_STDOUT
#define EO_LOG_STDERR  EO_LOGTO_STDERR
#define EO_LOG_TRACE   EO_LOGTO_TRACE
#define EO_LOG_USRFILE EO_LOGTO_USRFILE

#ifdef __cplusplus
}
#endif


#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif

