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
* Version Creation Date:   8/1/94
*
* $Revision: 6.1 $
*
* File Description:
*		system dependent header
*       version for Linux PPC.
*
* Untested. Contributed by Gary Bader <gary.bader@utoronto.ca>
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ncbilcl.plx,v $
* Revision 6.1  2001/04/17 13:38:28  beloslyu
* PPC linux
*
* ==========================================================================
*/
#ifndef _NCBILCL_
#define _NCBILCL_

/* PLATFORM DEFINITION FOR Linux */

#define COMP_SYSV
#define OS_UNIX
#define OS_UNIX_LINUX
#define OS_UNIX_PPCLINUX
#define PROC_PPC
#define WIN_DUMB

/*----------------------------------------------------------------------*/
/*      Desired or available feature list                               */
/*----------------------------------------------------------------------*/
#define SYSV_IPC_AVAIL	/* System V Interprocess Communication available */
/*#define _POSIX_C_SOURCE 199309L*/

#ifndef _REENTRANT
#define _REENTRANT
#endif

/* good for the EGCS C/C++ compiler on Linux(e.g. putenv(), tempnam() proto) */
/*#define _SVID_SOURCE 1 */


/*----------------------------------------------------------------------*/
/*      #includes                                                       */
/*----------------------------------------------------------------------*/
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
#include <errno.h>
#include <float.h>
#include <unistd.h>

/* Check if there are POSIX threads available */
#ifdef _POSIX_THREADS
#define POSIX_THREADS_AVAIL
#endif


/*----------------------------------------------------------------------*/
/*      Missing ANSI-isms                                               */
/*----------------------------------------------------------------------*/
#define noalias	

#ifndef SEEK_SET
#define SEEK_SET	0	/* Set file pointer to offset */
#define SEEK_CUR	1	/* Set file pointer to current plus offset */
#define SEEK_END	2	/* Set file pointer to EOF plus offset */
#endif
#ifndef FILENAME_MAX
#define FILENAME_MAX 1024
#endif

/*----------------------------------------------------------------------*/
/*      Aliased Logicals, Datatypes                                     */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*      Misc Macros                                                     */
/*----------------------------------------------------------------------*/
#define PROTO(x) x	/* Prototypes are acceptable */
#define VPROTO(x) x   /* Prototype for variable argument list */
#define DIRDELIMCHR     '/'
#define DIRDELIMSTR     "/"
#define CWDSTR          "."

#define KBYTE           (1024)
#define MBYTE           (1048576)

#define IS_BIG_ENDIAN
#define TEMPNAM_AVAIL

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

#endif

