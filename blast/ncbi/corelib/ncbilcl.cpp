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
* Author:  Gish, Kans, Ostell, Schuler
*
* Version Creation Date:   1/1/91
*
* $Revision: 6.1 $
*
* File Description:
*		system dependent header
*		C++ version
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: ncbilcl.cpp,v $
* Revision 6.1  2002/11/22 20:05:04  lavr
* Configure HAVE_STRDUP and HAVE_STRCASECMP
*
* Revision 6.0  1997/08/25 18:15:51  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 13:18:57  ostell
* Set to revision 5.0
*
Revision 4.0  1995/07/26  13:46:50  ostell
force revision to 4.0

Revision 2.6  1995/05/15  18:45:58  ostell
added Log line

*
*
* ==========================================================================
*/
#ifndef _NCBILCL_
#define _NCBILCL_

/* PLATFORM DEFINITION FOR C++ UNDER SunOS Unix */

#define COMP_CPLUS
#define OS_UNIX
#define OS_UNIX_SUN
#define PROC_SPARC
#define WIN_DUMB

/*----------------------------------------------------------------------*/
/*      Desired or available feature list                               */
/*----------------------------------------------------------------------*/
#define SYSV_IPC_AVAIL
#define SYSV_STREAMS_AVAIL
#define HAVE_STRCASECMP 1
#define HAVE_STRDUP 1

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
#include <sysent.h>
#include <float.h>

/*----------------------------------------------------------------------*/
/*      Missing ANSI-isms                                               */
/*----------------------------------------------------------------------*/
#define CLK_TCK		60
#ifndef FILENAME_MAX
#define FILENAME_MAX 1024
#endif
#ifndef SEEK_SET
#define SEEK_SET	0	/* Set file pointer to offset */
#define SEEK_CUR	1	/* Set file pointer to current plus offset */
#define SEEK_END	2	/* Set file pointer to EOF plus offset */
#endif

/*----------------------------------------------------------------------*/
/*      Aliased Logicals, Datatypes                                     */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*      Misc Macros                                                     */
/*----------------------------------------------------------------------*/
#define PROTO(x)	x
#define VPROTO(x)	x	/* Prototype for variable argument list */
#define DIRDELIMCHR	'/'
#define DIRDELIMSTR	"/"
#define CWDSTR	"."

#define KBYTE	(1024)
#define MBYTE	(1048576)

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
#define EXP2(x) exp2(x)
#define LOG2(x) log2(x)
#define EXP10(x) exp10(x)
#define LOG10(x) log10(x)

/*----------------------------------------------------------------------*/
/*      Macros Defining Limits                                          */
/*----------------------------------------------------------------------*/
#define MAXALLOC	0x40000000 /* Largest permissible memory request */

#endif
