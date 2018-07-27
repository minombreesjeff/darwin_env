/* $Id: ncbisort.h,v 6.0 1997/08/25 18:53:37 madden Exp $
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
* File Name:  $RCSfile: ncbisort.h,v $
*
* Author:  Sergei Shavirin
*
* Initial Version Creation Date: 03/24/1997
*
* $Revision: 6.0 $
*
* File Description:
*         External include file for SORTing library
*
* $Log: ncbisort.h,v $
* Revision 6.0  1997/08/25 18:53:37  madden
* Revision changed to 6.0
*
* Revision 1.2  1997/05/01 17:23:35  shavirin
* Added definition for the function SORTObjectFree()
*
 * Revision 1.1  1997/03/13  21:52:27  shavirin
 * Initial revision
 *
*
* ==========================================================================
*/

#ifndef _NCBISORT_H_
#define _NCBISORT_H_ ncbisort_h

/****************************************************************************/
/* INCLUDES */
/****************************************************************************/

#ifndef _NCBI_
#include <ncbi.h>
#endif

/****************************************************************************/
/* DEFINES */
/****************************************************************************/

#define ISBLANK(c) ((c) == ' ' || (c) == '\t')

typedef enum {
  SORTNoError         =  0,
  SORTNoSpace         =  -2,
  SORTBadFileName     =  -3,
  SORTNotImplemented  =  -4,
  SORTReadError       =  -5,
  SORTNoOrder         =  -6,
  SORTWriteError      =  -7,
  SORTBadParameter    =  -8,
  SORTNoMemory        =  -9,
  SORTMiscError       =  -99
} SORTErrorCode;

/****************************************************************************/
/* TYPEDEFS */
/****************************************************************************/
typedef VoidPtr SORTObjectPtr;

/* Lists of key field comparisons to be tried. */
typedef struct SORTKeyField
{
  Int4 sword;			/* Zero-origin 'word' to start at. */
  Int4 schar;			/* Additional characters to skip. */
  Int4 skipsblanks;		/* Skip leading white space at start. */
  Int4 eword;			/* Zero-origin first word after field. */
  Int4 echar;			/* Additional characters in field. */
  Int4 skipeblanks;		/* Skip trailing white space at finish. */
  Int4Ptr  ignore;              /* Boolean array of characters to ignore. */
  UcharPtr translate;	        /* Translation applied to characters. */
  Int4 numeric;			/* Flag for numeric comparison. */
  Int4 month;			/* Flag for comparison by month name. */
  Int4 reverse;			/* Reverse the sense of comparison. */
  struct SORTKeyField *next;	/* Next keyfield to try. */
} SORTKeyField, PNTR SORTKeyFieldPtr;

/****************************************************************************/
/* FINCTION DEFINITIONS */
/****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
/* ----------------------  SORTObjectNew --------------------------
   Purpose:     Creates SORT object structure with given/default 
                parameters
                
   Parameters:  listed below
   Returns:     Poiner to created object structure
   NOTE:        All settings may be overwriten later

  ------------------------------------------------------------------*/
SORTObjectPtr SORTObjectNew(
                CharPtr prefix,  /* Prefix for TEMP files */
                Uchar tab,       /* Default delimiter for strings */
                Int4 linelength, /* Average line length (try to guess!) */
                Boolean reverse, /* If TRUE sorted in reverce order */
                Boolean unique); /* If TRUE resulted file has unique lines */

/* ----------------------  SORTObjectFree --------------------------
   Purpose:     Free SORT object structure
                
   Parameters:  SORT Object
  ------------------------------------------------------------------*/
void SORTObjectFree(SORTObjectPtr object);


/* ---------------------- SORTMergeFiles --------------------------
   Purpose:     Merge list of sorted files into single
                sorted file.
                
   Parameters:  listed below
   Returns:     SORTErrorCode number 
   NOTE:        
  ------------------------------------------------------------------*/
SORTErrorCode SORTMergeFiles(
                CharPtr PNTR files, /* Pointer to list of filename strings */
                Int4 nfiles,        /* Number of files to merge */
                FILE *ofp,          /* Output FILE* */
                SORTObjectPtr sop   /* Sort options */
                );

/* ---------------------- SORTFiles --------------------------
   Purpose:     Sort list of files into single sorted file.
                
   Parameters:  listed below
   Returns:     SORTErrorCode number 
   NOTE:        This may sort just sinngle file
  ------------------------------------------------------------------*/
SORTErrorCode SORTFiles (
                CharPtr PNTR files, /* Pointer to list of filename strings */
                Int4 nfiles,        /* Number of files to merge */
                FILE *ofp,          /* Output FILE* */
                SORTObjectPtr sop); /* Sort options */
  
/* ---------------------- SORTCheckOrderS --------------------------
   Purpose:     Check, that all passed files are in order
                
   Parameters:  listed below
   Returns:     Number NON-sorted files (0 mean all sorted)
   NOTE:        This may sort just sinngle file
  ------------------------------------------------------------------*/
Int4 SORTCheckOrderS(
                CharPtr PNTR files, /* Pointer to list of filename strings */ 
                Int4 nfiles,        /* Number of files to merge */
                SORTObjectPtr sop   /* Sort options */
                );

/* ---------------------- SORTSet.. functions -----------------------
   Purpose:     These functions override initial settings in
                SORTObjectPtr structure
   Parameters:  various
   Returns:     SORTErrorCode
   NOTE:        Be careful with function SORTInserKey()!
  ------------------------------------------------------------------*/
SORTErrorCode SORTSetReverse(Boolean reverse, SORTObjectPtr sop);
SORTErrorCode SORTSetUnique(Boolean unique, SORTObjectPtr sop);
SORTErrorCode SORTSetLineLength(Int4 linelength, SORTObjectPtr sop); 
SORTErrorCode SORTSetTab(Uchar tab, SORTObjectPtr sop);
SORTErrorCode SORTSetPrefix(CharPtr prefix, SORTObjectPtr sop);
SORTKeyFieldPtr SORTGetKeyHead(SORTObjectPtr sop);
SORTErrorCode SORTInsertKey(SORTKeyFieldPtr key, SORTKeyFieldPtr keyhead);

/* ---------------------- SORTAddTempName --------------------------
   Purpose:     To add one more entry to teporary file table to
                be deleted in the end
   Parameters:  SORTObjectPtr
   Returns:     Pointer to created filename string
   NOTE:        Created filenamed platform-dependent
  ------------------------------------------------------------------*/  
CharPtr SORTAddTempName(SORTObjectPtr sop);

#ifdef __cplusplus
}
#endif

#endif
