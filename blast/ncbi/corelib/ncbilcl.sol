/*   ncbilcl.h
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
* File Name:  ncbilcl.h
*
* Author:  Gish, Kans, Ostell, Schuler, Epstein, Vakatov
*
* Version Creation Date:   3/16/93
*
* $Revision: 6.12 $
*
* File Description:
*		system dependent header
*		for Solaris 2.7 on SPARC
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ncbilcl.sol,v $
* Revision 6.12  2003/09/05 21:24:16  beloslyu
* reverse _LARGEFILE_SOURCE back
*
* Revision 6.11  2003/09/05 21:22:58  beloslyu
* add DARWIN def
*
* Revision 6.10  2002/11/22 20:05:05  lavr
* Configure HAVE_STRDUP and HAVE_STRCASECMP
*
* Revision 6.9  2002/07/11 19:26:31  ivanov
* Added macro HAVE_MADVISE
*
* Revision 6.8  2000/08/07 16:50:06  vakatov
* From now on, use the "standard" Int4 (as defined in <ncbistd.h>)
* for 32-bit, too.
*
* Revision 6.7  1999/08/24 18:04:02  vakatov
* Switched the default thread lib from "Solaris native"(-lthread) to
* "POSIX"(-lpthread)
*
* Revision 6.6  1999/07/28 22:05:36  vakatov
* [non-32-bit only!]  Do not pre-define "Int4" to "long";  let it be "int"
*
* Revision 6.5  1999/04/20 18:59:43  vakatov
* [__cplusplus && NCBI_USE_NEW_HEADERS]  Use new standard C++ system headers
*
* Revision 6.4  1999/03/19 17:03:42  vakatov
* Made "Int4" be "long" again, for the 32-to64-bit transition period
* (mostly for the PubMed group)
*
* Revision 6.3  1998/02/24 19:20:38  vakatov
* #define __EXTENSIONS__ only if it is not #define'd yet(- cpp warning)
*
* Revision 6.2  1997/12/23 19:06:25  vakatov
* + #OS_UNIX_SOL; - cpp warnings on #SOLARIS_THREADS_AVAIL
*
* Revision 6.1  1997/11/26 20:19:45  vakatov
* Explicitly #define __EXTENSIONS__ to pass through "gcc" compiler
*
* Revision 5.1  1996/11/25 19:00:33  vakatov
* [MPROC_AVAIL]  define SOLARIS_THREADS_AVAIL only if POSIX_THREADS_AVAIL
* undefined.  Removed <thread.h> and <synch.h> from #include's.
*
* Revision 4.1  1996/05/08  15:34:18  epstein
* enable MPROC_AVAIL
* ==========================================================================
*/

#ifndef _NCBILCL_
#define _NCBILCL_

/* PLATFORM DEFINITION FOR Solaris Unix on SparcStation */

#define COMP_SUNPRO
#define OS_UNIX
#define OS_UNIX_SYSV
#define OS_UNIX_SOL
#define PROC_SPARC
#define WIN_DUMB

#ifndef __EXTENSIONS__
#define __EXTENSIONS__
#endif

/*----------------------------------------------------------------------*/
/*      Desired or available feature list                               */
/*----------------------------------------------------------------------*/
#define MPROC_AVAIL
#define SYSV_IPC_AVAIL	/* System V Interprocess Communication available */
#define SYSV_STREAMS_AVAIL	/* System V STREAMS module available */
/*
#define _LARGEFILE_SOURCE
#define _FILE_OFFSET_BITS	64
*/
#define HAVE_STRCASECMP 1
#define HAVE_STRDUP 1

#ifdef MPROC_AVAIL
/* For Solaris, _REENTRANT must be defined when compiling all modules or none*/
#  ifndef _REENTRANT
#    define _REENTRANT
#  endif
/* Use POSIX thread library by default */
#  if !defined(POSIX_THREADS_AVAIL)  &&  !defined(SOLARIS_THREADS_AVAIL)
#    define POSIX_THREADS_AVAIL
#  endif
#endif

/*----------------------------------------------------------------------*/
/*      #includes                                                       */
/*----------------------------------------------------------------------*/
#if defined(__cplusplus) && defined(NCBI_USE_NEW_HEADERS)
#include <sys/types.h>
#include <climits>
#include <sys/stat.h>
#include <cstddef>
#include <cstdio>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <cfloat>
#include <cerrno>
#include <malloc.h>
#include <unistd.h>
#else /* __cplusplus && NCBI_USE_NEW_HEADERS */
#include <sys/types.h>
#include <limits.h>
#include <sys/stat.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <unistd.h>
#include <errno.h>
#endif /* __cplusplus && NCBI_USE_NEW_HEADERS */

/*----------------------------------------------------------------------*/
/*      Missing ANSI-isms                                               */
/*----------------------------------------------------------------------*/
#define noalias /* "noalias" keyword not accepted by SunPro C */
#ifndef FILENAME_MAX
#define FILENAME_MAX	1024
#endif

/*----------------------------------------------------------------------*/
/*      Aliased Logicals, Datatypes                                     */
/*----------------------------------------------------------------------*/


/*----------------------------------------------------------------------*/
/*      Misc Macros                                                     */
/*----------------------------------------------------------------------*/
#define PROTO(x)	x
#define VPROTO(x)	x
#define DIRDELIMCHR	'/'
#define DIRDELIMSTR	"/"
#define CWDSTR	"."

#define KBYTE	(1024)
#define MBYTE	(1048576)

#define IS_BIG_ENDIAN
#define TEMPNAM_AVAIL
#define HAVE_MADVISE

/*----------------------------------------------------------------------*/
/*      For importing MS_DOS code                                       */
/*----------------------------------------------------------------------*/
#define near
#define far
#define huge
#define cdecl
#define pascal
#define _pascal
#define _near
#define _far
#define _huge
#define _cdecl

/*----------------------------------------------------------------------*/
/*      Macros for Floating Point                                       */
/*----------------------------------------------------------------------*/
#define EXP2(x) exp((x)*LN2)
#define LOG2(x) (log(x)*(1./LN2))
#define EXP10(x) exp((x)*LN10)
#define LOG10(x) log10(x)

/*----------------------------------------------------------------------*/
/*      Macros Defining Limits                                          */
/*----------------------------------------------------------------------*/
#define MAXALLOC	0x40000000 /* Largest permissible memory request */

#endif /* _NCBILCL_ */
