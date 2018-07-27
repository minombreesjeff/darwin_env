/* $Id: ncbisrti.h,v 6.3 2003/12/03 02:10:52 kans Exp $
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
* File Name:  $RCSfile: ncbisrti.h,v $
*
* Author:  Sergei Shavirin
*
* Initial Version Creation Date: 03/24/1997
*
* $Revision: 6.3 $
*
* File Description:
*         Internal include file for SORTing library
*
* $Log: ncbisrti.h,v $
* Revision 6.3  2003/12/03 02:10:52  kans
* added defines missing from Mac OS 10.3 headers
*
* Revision 6.2  1998/06/01 14:52:38  madden
* Change to using TmpNam
*
* Revision 6.1  1997/12/02 18:58:19  shavirin
* Chanded defines for sprintf to remove typecast warnings
*
* Revision 6.0  1997/08/25 18:53:39  madden
* Revision changed to 6.0
*
* Revision 1.1  1997/03/13 21:52:44  shavirin
* Initial revision
*
*
* ==========================================================================
*/

#ifndef _NCBISRTI_H_
#define _NCBISRTI_H_ ncbisrti_h

#include <ncbisort.h>

#ifdef OS_UNIX
#include <signal.h>
#endif

/* Initial buffer size for in core sorting.  Will not grow unless a
   line longer than this is seen. */

/* Prefix for temporary file names. */

#ifndef OS_UNIX

#define SORTALLOC 32767
#define PREFIX NULL

#else /* OS_UNIX */

#define SORTALLOC 524288
#define PREFIX "/tmp"                      /* Default prefix */

#endif /* OS_UNIX */

/* Missing from /usr/include/gcc/darwin/3.3/machine/limits.h */
#ifdef __MWERKS__
#ifdef OS_UNIX_DARWIN
#ifndef __SCHAR_MAX__
#define __SCHAR_MAX__ 127
#endif
#endif
#endif
/* End missing from /usr/include/gcc/darwin/3.3/machine/limits.h */

#define MERGEALLOC 16384

#define UCHAR_LIM (UCHAR_MAX + 1)
#define UCHAR(c) ((Uchar) (c))

/* Table of digits. */
static Int4 digits[UCHAR_LIM];

/* Table of white space. */
static Int4 blanks[UCHAR_LIM];

/* Translation table folding upper case to lower. */
static Uchar fold_tolower[UCHAR_LIM];

/* Table mapping 3-letter month names to integers.
   Alphabetic order allows binary search. */

static struct month {
    CharPtr name;
    Int4 val;
} monthtab[] = {
    {"apr", 4},
    {"aug", 8},
    {"dec", 12},
    {"feb", 2},
    {"jan", 1},
    {"jul", 7},
    {"jun", 6},
    {"mar", 3},
    {"may", 5},
    {"nov", 11},
    {"oct", 10},
    {"sep", 9}
};

/* During the merge phase, the number of files to merge at once. */

#define NMERGE 16 
static Boolean tables_initialized = FALSE;

/* Lines are held in core as counted strings. */
typedef struct SORTLine
{
  UcharPtr text;	 /* Text of the line. */
  Int4     length;       /* Length not including final newline. */
  UcharPtr keybeg;       /* Start of first key. */
  UcharPtr keylim;	 /* Limit of first key. */
} SORTLine, PNTR SORTLinePtr;

/* Arrays of lines. */
typedef struct SORTLines
{
  SORTLinePtr lines;            /* Dynamically allocated array of lines. */
  Int4 used;                    /* Number of slots used. */
  Int4 alloc;                   /* Number of slots allocated. */
}SORTLines, PNTR SORTLinesPtr;

/* Input buffers. */
typedef struct SORTBuffer
{
  UcharPtr buf;			/* Dynamically allocated buffer. */
  Int4 used;			/* Number of bytes used. */
  Int4 alloc;			/* Number of bytes allocated. */
  Int4 left;			/* Number of bytes left after line parsing. */
}SORTBuffer, PNTR SORTBufferPtr;

/* The list of temporary files. */

typedef struct SORTempNode
{
  CharPtr name;
  struct SORTempNode *next;
} SORTempNode, PNTR SORTempNodePtr;

typedef struct SORTData 
{
  SORTempNodePtr temphead; 
  SORTKeyFieldPtr keyhead;
  Int4 temp_seq;  
  Int4 sortalloc;
  Int4 mergealloc;
  Int4 linelength;
  CharPtr prefix;
  Boolean reverse;
  Uchar tab;
  Boolean unique;
} SORTData, PNTR SORTDataPtr;

static SORTempNodePtr global_temphead; 

/* To handle ALL temp files opened by (may be)
   multy-thread application */

static void SORTCleanup (SORTempNodePtr temphead);
static void SORTDelTempName(CharPtr name, SORTempNodePtr temphead);
static SORTErrorCode SORTInitBuf (SORTBufferPtr buf, Int4 alloc);
static Int4 SORTFillBuf (SORTBufferPtr buf, FILE *fp);
static SORTErrorCode SORTInitLines (SORTLinesPtr lines, Int4 alloc);
static UcharPtr SORTBegField(SORTLinePtr line, SORTKeyFieldPtr key, 
                             SORTDataPtr sdp);
static UcharPtr SORTLimField(SORTLinePtr line, SORTKeyFieldPtr key, 
                             SORTDataPtr sdp);
static SORTErrorCode SORTFindLines(SORTBufferPtr buf, SORTLinesPtr lines, 
                          SORTDataPtr sdp);
static Int4 SORTFracCompare (UcharPtr a, UcharPtr b);
static Int4 SORTNumCompare (UcharPtr a, UcharPtr b);
static Int4 SORTGetMonth (UcharPtr s, Int4 len);
static Int4 SORTKeyCompare(SORTLinePtr a, SORTLinePtr b, SORTDataPtr sdp);
static Int4 SORTCompare(register SORTLinePtr a, register SORTLinePtr b, 
                        SORTDataPtr sdp);
static Int4 SORTMergeFPS(FILE *fps[], register Int4 nfps, 
                         FILE *ofp, SORTDataPtr sdp);
static void SORTArrayLines(SORTLinePtr lines,  Int4 nlines, 
                      SORTLinePtr temp, SORTDataPtr sdp);
static Int4 SORTCheckOrder(FILE *fp, SORTDataPtr sdp);

#ifdef OS_UNIX
#ifdef OS_UNIX_IRIX
static void inthandler(void);
#else
static void inthandler(int i);
#endif
#endif

#endif
