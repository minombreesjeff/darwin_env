/*  ncbitime.c
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
* File Name:  ncbitime.c
*
* Author:  Ostell, Kans
*
* Version Creation Date:  1/1/90
*
* $Revision: 6.2 $
*
* File Description:
*   misc portable routines for
*   dates, times, timers
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ncbitime.c,v $
* Revision 6.2  2001/01/19 20:26:12  kans
* support for OS_UNIX_DARWIN (contributed by William Van Etten)
*
* Revision 6.1  1997/10/29 02:45:09  vakatov
* Type castings to pass through the C++ compiler
*
* Revision 6.0  1997/08/25 18:17:21  madden
* Revision changed to 6.0
*
* Revision 5.7  1997/07/17 16:09:31  vakatov
* Encapsulated "NCBI_months" global variable into a {function + cpp macros}
*
* Revision 5.6  1996/12/03 21:48:33  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
 * Revision 5.5  1996/11/25  19:05:52  vakatov
 * Made Nlm_GetDayTime() MT-safe
 *
 * Revision 5.3  1996/09/17  15:23:25  sad
 * Corrected compilation problem under SunOS 4 related with CLK_TCK constant
 *
 * Revision 5.2  1996/09/16  20:19:57  kans
 * structures cannot be empty, so needed to add #else condition in two places
 *
 * Revision 5.1  1996/09/16  19:37:19  sad
 * Added set of functions for measuring CPU time.
 * Added StopWatch set of functions.
 *
* 04-15-93 Schuler     Changed _cdecl to LIBCALL
*
* ==========================================================================
*/

#include <ncbi.h>
#include <ncbithr.h>
#include <ncbiwin.h>

#ifdef OS_UNIX
#include <sys/times.h>
#include <unistd.h>
#endif

static char *STATIC__NCBI_months[12] = {
  "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };
NLM_EXTERN char **x_NCBI_months(void)  { return &STATIC__NCBI_months[0]; }

static char *ampm[2] = { "AM", "PM" };


/*****************************************************************************
*
*   Nlm_GetSecs()
*   	returns computer time in seconds - for timing purposes only
*
*****************************************************************************/
NLM_EXTERN time_t LIBCALL  Nlm_GetSecs (void)
{
  return time(NULL);
}

/*****************************************************************************
*
*   Nlm_GetDayTime(DayTimePtr)
*
*****************************************************************************/
NLM_EXTERN Nlm_Boolean LIBCALL  Nlm_GetDayTime (Nlm_DayTimePtr dtp)
{
#if (defined(SOLARIS_THREADS_AVAIL)  ||  defined(POSIX_THREADS_AVAIL)  ||  defined(WIN32)) && !defined(OS_UNIX_DARWIN)
  time_t t = time( NULL );
#ifdef WIN32
  static TNlmMutex localtime_lock;
  if (NlmMutexLockEx( &localtime_lock ) != 0)
    abort();
  *dtp = *localtime( &t );
  NlmMutexUnlock( localtime_lock );
#else
  localtime_r(&t, dtp);
#endif
  return TRUE;

#else
  time_t ltime;
  struct tm *dt;
  Nlm_MemFill ((Nlm_VoidPtr) dtp, 0, sizeof(Nlm_DayTime));
	time(&ltime);
  if ((dt = localtime (&ltime)) != NULL)
 	{
        Nlm_MemCopy ((Nlm_VoidPtr) dtp, (Nlm_VoidPtr) dt, sizeof (struct tm));
        return TRUE;
    }
    return FALSE;
#endif
}

/*****************************************************************************
*
*   Nlm_DayTimeStr(buf, date, time)
*   	fills buffer with formatted date and/or time
*   	date requires a 13 character buffer
*       time requeres a 10 character buffer
*   	data and time takes 23
*
*       eg.   "Jan 28, 1988 12:59 PM"
*
*****************************************************************************/
NLM_EXTERN Nlm_Boolean LIBCALL  Nlm_DayTimeStr (Nlm_CharPtr buf, Nlm_Boolean nlm_date, Nlm_Boolean nlm_time)

{
	Nlm_DayTime dt;
	char localbuf[30];
	int pm;

	buf[0] = '\0';

	if ( ! Nlm_GetDayTime(&dt))
		return FALSE;

	if (nlm_date)
	{
		sprintf (localbuf, "%s %d, %d", NCBI_months [dt.tm_mon],
                    dt.tm_mday, dt.tm_year + 1900);
		StringCpy (buf, localbuf);
	}
	if (nlm_time)
	{
		pm = 1;
		if (dt.tm_hour == 0)
		{
			dt.tm_hour = 12;
			pm = 0;
		}
		else if (dt.tm_hour < 12)
			pm = 0;
		else if (dt.tm_hour > 12)
			dt.tm_hour -= 12;

	    sprintf (localbuf, "%3d:%2d %s", dt.tm_hour, dt.tm_min, ampm[pm]);
		if (dt.tm_min < 10)
			localbuf[4] = '0';
		StringCat (buf, localbuf);
	}
	return TRUE;
}

struct _StopWatch {
#if defined(WIN32)
    FILETIME	start;
    FILETIME	stop;
#elif defined(OS_UNIX)
    clock_t	start;
    clock_t	stop;
#else
    int         dummy;
#endif
};
typedef struct _StopWatch Nlm_StopWatch;

NLM_EXTERN Nlm_StopWatchPtr Nlm_StopWatchNew(void)
{
#if defined(WIN32) || defined(OS_UNIX)
    return (Nlm_StopWatchPtr) Nlm_MemNew(sizeof(Nlm_StopWatch));
#else
    ErrPostEx(SEV_WARNING, 0, 0,
	"StopWatch is not implemented for this platform");
    return NULL;
#endif
}

NLM_EXTERN Nlm_StopWatchPtr Nlm_StopWatchFree(Nlm_StopWatchPtr pStopWatch)
{
#if defined(WIN32) || defined(OS_UNIX)
    return (Nlm_StopWatchPtr) Nlm_MemFree(pStopWatch);
#else
    ErrPostEx(SEV_WARNING, 0, 0,
	"StopWatch is not implemented for this platform");
    return NULL;
#endif
}

NLM_EXTERN Nlm_StopWatchPtr Nlm_StopWatchStart(Nlm_StopWatchPtr pSW)
{
#if defined(WIN32)
    SYSTEMTIME	systime;

    GetSystemTime(&systime);
    SystemTimeToFileTime(&systime, &pSW->start);
#elif defined(OS_UNIX)
    struct tms	foo;

    pSW->start = times(&foo);
#else
    ErrPostEx(SEV_WARNING, 0, 0,
	"StopWatch is not implemented for this platform");
#endif
    return pSW;
}

NLM_EXTERN Nlm_StopWatchPtr Nlm_StopWatchStop(Nlm_StopWatchPtr pSW)
{
#if defined(WIN32)
    SYSTEMTIME	systime;

    GetSystemTime(&systime);
    SystemTimeToFileTime(&systime, &pSW->stop);
#elif defined(OS_UNIX)
    struct tms	foo;

    pSW->stop = times(&foo);
#else
    ErrPostEx(SEV_WARNING, 0, 0,
	"StopWatch is not implemented for this platform");
#endif
    return pSW;
}

NLM_EXTERN Nlm_FloatHi Nlm_GetElapsedTime(Nlm_StopWatchPtr pSW)
{
    Nlm_FloatHi	res = 0;
#if defined(WIN32)
    FILETIME	ft;

    if (pSW->start.dwLowDateTime > pSW->stop.dwLowDateTime) {
	ft.dwLowDateTime = (0xFFFFFFFFL - pSW->start.dwLowDateTime) + 1
	    + pSW->stop.dwLowDateTime;
	ft.dwHighDateTime =
	    pSW->stop.dwHighDateTime - pSW->start.dwHighDateTime - 1;
    } else {
	ft.dwLowDateTime =
	    pSW->stop.dwLowDateTime - pSW->start.dwLowDateTime;
	ft.dwHighDateTime =
	    pSW->stop.dwHighDateTime - pSW->start.dwHighDateTime;
    }

    return res = (Nlm_FloatHi)ft.dwHighDateTime/5000000L*0x80000000L +
	(Nlm_FloatHi)ft.dwLowDateTime/10000000L;
#elif defined(OS_UNIX)
    res = ((Nlm_FloatHi)pSW->stop - pSW->start) / sysconf(_SC_CLK_TCK);
#else
    ErrPostEx(SEV_WARNING, 0, 0,
	"StopWatch is not implemented for this platform");
#endif

    return res;
}


struct _CPUTime {
#if defined(WIN32)
	FILETIME	crtime;
	FILETIME	extime;
	FILETIME	systime;
	FILETIME	usrtime;
#elif defined(OS_UNIX)
    struct tms	times;
#else
    int         dummy;
#endif
};

typedef struct _CPUTime Nlm_CPUTime;

NLM_EXTERN Nlm_CPUTimePtr Nlm_CPUTimeMeasure(void)
{
#if defined(WIN32) || defined(OS_UNIX)
  Nlm_CPUTimePtr pTime = (Nlm_CPUTimePtr) Nlm_MemNew(sizeof(Nlm_CPUTime));
  if (pTime != NULL) {
#endif

#if defined(WIN32)
    GetProcessTimes(GetCurrentProcess(),
                    &pTime->crtime,
                    &pTime->extime,
                    &pTime->systime,
                    &pTime->usrtime
                    );
#elif defined(OS_UNIX)
    times(&pTime->times);
#else
    ErrPostEx(SEV_WARNING, 0, 0,
              "CPU time measuring is not implemented for this platform");
    return NULL;
#endif

#if defined(WIN32) || defined(OS_UNIX)
    }

    return pTime;
#endif
}

NLM_EXTERN Nlm_CPUTimePtr Nlm_CPUTimeFree(Nlm_CPUTimePtr pTime)
{
#if defined(WIN32) || defined(OS_UNIX)
  return (Nlm_CPUTimePtr) Nlm_MemFree(pTime);
#else
  ErrPostEx(SEV_WARNING, 0, 0,
            "CPU time measuring is not implemented for this platform");
  return NULL;
#endif
}

NLM_EXTERN Nlm_FloatHi Nlm_CPUTimeGetSys(Nlm_CPUTimePtr pTime)
{
#if defined(WIN32)
  return (Nlm_FloatHi)pTime->systime.dwHighDateTime * 0x80000000L/5000000L
		+ (Nlm_FloatHi)pTime->systime.dwLowDateTime / 10000000L;
#elif defined(OS_UNIX)
  return (Nlm_FloatHi)pTime->times.tms_stime / sysconf(_SC_CLK_TCK);
#else
  ErrPostEx(SEV_WARNING, 0, 0,
            "CPU time measuring is not implemented for this platform");
  return 0;
#endif
}

NLM_EXTERN Nlm_FloatHi Nlm_CPUTimeGetUser(Nlm_CPUTimePtr pTime)
{
#if defined(WIN32)
  return (Nlm_FloatHi)pTime->usrtime.dwHighDateTime * 0x80000000L/5000000L
    + (Nlm_FloatHi)pTime->usrtime.dwLowDateTime / 10000000L;
#elif defined(OS_UNIX)
  return (Nlm_FloatHi)pTime->times.tms_utime / sysconf(_SC_CLK_TCK);
#else
  ErrPostEx(SEV_WARNING, 0, 0,
            "CPU time measuring is not implemented for this platform");
  return 0;
#endif
}

