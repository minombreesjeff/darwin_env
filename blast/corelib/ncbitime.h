/*  ncbitime.h
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
* File Name:  ncbitime.h
*
* Author:  Ostell, Kans
*
* Version Creation Date:  1/1/90
*
* $Revision: 6.0 $
*
* File Description:
*   misc portable routines with
*   dates, times, timers
*   prototypes and defines
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 09-19-91 Schuler     Redefined DayTime struct as ASNI struct tm.
* 09-19-91 Schuler     Use ANSI functions for time services instead of
*                       the Macintosh and Windows functions.
* 04-15-93 Schuler     Changed _cdecl to LIBCALL
*
*
* $Log: ncbitime.h,v $
* Revision 6.0  1997/08/25 18:17:23  madden
* Revision changed to 6.0
*
* Revision 5.4  1997/07/17 16:09:33  vakatov
* Encapsulated "NCBI_months" global variable into a {function + cpp macros}
*
* Revision 5.3  1996/12/03 21:48:33  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
 * Revision 5.2  1996/09/16  20:19:57  kans
 * corrected prototype misspelling
 *
 * Revision 5.1  1996/09/16  19:37:19  sad
 * Added set of functions for measuring CPU time.
 * Added StopWatch set of functions.
 *
* Revision 5.0  1996/05/28 13:18:57  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:46:50  ostell
 * force revision to 4.0
 *
 * Revision 2.2  1995/05/15  18:45:58  ostell
 * added Log line
 *
*
* ==========================================================================
*/

#ifndef _NCBITIME_
#define _NCBITIME_

#include <time.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" {
#endif

NLM_EXTERN char **x_NCBI_months(void);
#define NCBI_months x_NCBI_months()

typedef struct tm   Nlm_DayTime, PNTR Nlm_DayTimePtr;

NLM_EXTERN time_t LIBCALL  Nlm_GetSecs PROTO((void));
NLM_EXTERN Nlm_Boolean LIBCALL  Nlm_GetDayTime PROTO((Nlm_DayTimePtr dtp));
NLM_EXTERN Nlm_Boolean LIBCALL  Nlm_DayTimeStr PROTO((Nlm_CharPtr buf, Nlm_Boolean date, Nlm_Boolean time));

#define DayTime Nlm_DayTime
#define DayTimePtr Nlm_DayTimePtr
#define GetSecs Nlm_GetSecs
#define GetDayTime Nlm_GetDayTime
#define DayTimeStr Nlm_DayTimeStr


struct _StopWatch;
typedef struct _StopWatch PNTR Nlm_StopWatchPtr;

NLM_EXTERN Nlm_StopWatchPtr	Nlm_StopWatchNew	PROTO((void));
NLM_EXTERN Nlm_StopWatchPtr	Nlm_StopWatchFree	PROTO((Nlm_StopWatchPtr pStopWatch));
NLM_EXTERN Nlm_StopWatchPtr	Nlm_StopWatchStart	PROTO((Nlm_StopWatchPtr pSW));
NLM_EXTERN Nlm_StopWatchPtr	Nlm_StopWatchStop	PROTO((Nlm_StopWatchPtr pSW));
NLM_EXTERN Nlm_FloatHi		Nlm_GetElapsedTime	PROTO((Nlm_StopWatchPtr pSW));

#define StopWatchPtr	Nlm_StopWatchPtr
#define StopWatchNew	Nlm_StopWatchNew
#define StopWatchFree	Nlm_StopWatchFree
#define StopWatchStart	Nlm_StopWatchStart
#define StopWatchStop	Nlm_StopWatchStop
#define GetElapsedTime	Nlm_GetElapsedTime


struct _CPUTime;
typedef struct _CPUTime PNTR Nlm_CPUTimePtr;

NLM_EXTERN Nlm_CPUTimePtr	Nlm_CPUTimeMeasure	PROTO((void));
NLM_EXTERN Nlm_CPUTimePtr	Nlm_CPUTimeFree		PROTO((Nlm_CPUTimePtr pTime));
NLM_EXTERN Nlm_FloatHi	Nlm_CPUTimeGetSys	PROTO((Nlm_CPUTimePtr pTime));
NLM_EXTERN Nlm_FloatHi	Nlm_CPUTimeGetUser	PROTO((Nlm_CPUTimePtr pTime));

#define CPUTimePtr	Nlm_CPUTimePtr
#define CPUTimeMeasure	Nlm_CPUTimeMeasure
#define CPUTimeFree	Nlm_CPUTimeFree
#define CPUTimeGetSys	Nlm_CPUTimeGetSys
#define CPUTimeGetUser	Nlm_CPUTimeGetUser

#ifdef __cplusplus
}
#endif


#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#endif

#endif

