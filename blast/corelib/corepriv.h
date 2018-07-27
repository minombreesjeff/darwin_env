/*   corepriv.h
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
* File Name:  ncbi.h
*
* Author:  Gish, Kans, Ostell, Schuler
*
* Version Creation Date:   02/23/94
*
* $Revision: 6.0 $
*
* File Description:  Privated definitions used internally by CoreLib
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 06-29-94 Schuler     Moved some macros here from other files
*
* $Log: corepriv.h,v $
* Revision 6.0  1997/08/25 18:15:03  madden
* Revision changed to 6.0
*
* Revision 5.5  1997/07/22 19:11:15  vakatov
* Separated Main() from GetArg[svc]() functions;  [WIN_MSWIN] converged
* console and GUI libraries; [for WIN32-DLL] encapsulated global variables
*
* Revision 5.4  1997/06/26 16:06:13  vakatov
* [MSWIN,DLL]  Made "targ[vc]" be not DLL-exportable in "ncbi.lib/dll"
*
* Revision 5.3  1996/12/03 21:48:33  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
 * Revision 5.2  1996/11/25  19:02:59  vakatov
 * 'corelibMutex' mutex to synchronize corelib's functions in the
 * multithtread applications.
 *
 * Revision 5.1  1996/07/16  19:55:35  vakatov
 * Added ReleaseAppErrInfo() and ReleaseAppMsgInfo() prototypes
 *
 * Revision 5.0  1996/05/28  13:18:57  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:46:50  ostell
 * force revision to 4.0
 *
 * Revision 2.3  1995/05/15  18:45:58  ostell
 * added Log line
 *
*
* ==========================================================================
*/

#ifndef __COREPRIV_H__
#define __COREPRIV_H__

extern char *g_corelib;

#ifndef THIS_MODULE
#define THIS_MODULE g_corelib
#endif

#include <ncbi.h>
#include <ncbithr.h>
#include <ncbiwin.h>

#ifdef VAR_ARGS
#include <varargs.h>
#define VSPRINTF(buff,fmt)                 \
	{                                  \
		va_list args;              \
		va_start(args);            \
		vsprintf(buff,fmt,args);   \
		va_end(args);              \
	}

#else
#include <stdarg.h>
#define VSPRINTF(buff,fmt)                 \
	{                                  \
		va_list args;              \
		va_start(args,fmt);        \
		vsprintf(buff,fmt,args);   \
		va_end(args);              \
	}
#endif

extern void ReleaseAppErrInfo PROTO( ( void ) );
extern void ReleaseAppMsgInfo PROTO( ( void ) );

extern TNlmMutex corelibMutex;

#ifdef WIN_DUMB
#ifndef WIN32
#define Nlm_HasConsole 1
#else
/* Win32 application may have or not have a console */
extern int Nlm_x_HasConsole PROTO((void));
#define Nlm_HasConsole Nlm_x_HasConsole()
#endif
#else
#define Nlm_HasConsole 0
#endif

#endif

/* EOF */
