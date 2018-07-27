/*   fstyle.h
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  fstyle.h
*
* Author:  Sergei Egorov, Alex Smirnov
*
* Version Creation Date:   9/23/94
*
* $Revision: 6.1 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*/

#ifndef _FSTYLE_
#define _FSTYLE_

/**************************************************************************/
/* INCLUDE */
/**************************************************************************/
#include <objfdef.h>
#include <sequtil.h>
#include <ncbidraw.h>

/**************************************************************************/
/* DEFINES */
/**************************************************************************/
/* Parameter types: */
#define MSM_TRUEFALSE  0  /*Nlm_Boolean*/
#define MSM_COLOR      1  /*Nlm_Int4 R<<16 | G<<8 | B*/
#define MSM_LTYPE      2  /*Nlm_Int1*/
#define MSM_SHADING    3  /*Nlm_Int1*/
#define MSM_PENWIDTH   4  /*Nlm_Int1*/
#define MSM_NUM        5  /*Nlm_Int2*/
#define MSM_STYLE      6  /*Nlm_Int4*/
#define MSM_FONT       7  /*Nlm_FonT*/
#define MSM_HEIGHT     8  /*Nlm_Int4*/
#define MSM_STRING     9  /*Nlm_CharPtr*/
#define MSM_SCALE     10  /*Nlm_Int4*/

/* Parameter class may be any feature type ( see objfdef.h ) or
one of the following macros: */
#define MSM_TOPSTYLE          0 /* top style */
#define MSM_GROUPS           -1 /* groups of features */
#define MSM_ALIGNMENT        -2 /* alignment */
#define MSM_CCOLOR           -3 /* cycle colors */
#define MSM_SEQUENCE         -4 /* sequence */
#define MSM_EXTRA_GENBANK    -5 /* extra genbank record in the display */
#define MSM_EXTRA_MEDLINE    -6 /* extra medline record in the display */
#define MSM_EXTRA_BOTH       -7 /* extra medline record in the display */

/* Table of parameters
-------------------------------------------------------------------------------
  Class:   |  Subclass: | TRUEFALSE    | SHADING      | STYLE        | STRING |
           |            |    | COLOR   |    | PENWIDTH|    | FONT    |    |SCA|
           |            |    |    | LTYPE   |    | NUM|    |    | HEIGHT  |LE |
-------------------------------------------------------------------------------
Any feature| SEGMENT    |    | *  |    | *  |    |    | *1 |    | *  |    |   |
(objfdef.h)| SEG_BORD   | *  | *  | *  |    | *  |    |    |    |    |    |   |
           | FGAP       |    | *  | *  |    | *  |    | *2 |    |    |    |   |
           | FLABEL     |    | *  |    |    |    |    | *3 | *  | *  |    |   |
           | FGROUP     |    |    |    |    |    | *4 |    |    |    |    |   |
           | FORDER     |    |    |    |    |    | *5 |    |    |    |    |   |
           |            |    |    |    |    |    |    |    |    |    |    |   |
 Any extra | SEGMENT    |    | *  |    | *  |    |    | *1 |    | *  |    |   |
           | SEG_BORD   | *  | *  | *  |    | *  |    |    |    |    |    |   |
           | FGAP       |    | *  | *  |    | *  |    | *2 |    |    |    |   |
           | FLABEL     |    | *  |    |    |    |    | *3 | *  | *  |    |   |
           |            |    |    |    |    |    |    |    |    |    |    |   |
 SEQUENCE  | SEGMENT    |    | *  |    | *  |    |    | *6 |    | *  |    |   |
           | SEG_BORD   | *  | *  | *  |    | *  |    |    |    |    |    |   |
           | SLABEL     |    | *  |    |    |    |    | *7 | *  | *  |    |   |
           |            |    |    |    |    |    |    |    |    |    |    |   |
 TOPSTYLE  | NOSUBCLASS |    |    |    |    |    |    | *8 |    |    |    |   |
           | SPACE      |    |    |    |    |    |    |    |    | *9 |    |   |
           | ENDS       |    |    |    |    |    |    |    |    |    |    | * |
           | LABEL      |    |    |    |    |    | *10| *15|    |    |    |   |
           |            |    |    |    |    |    |    |    |    |    |    |   |
 GROUPS    | NOSUBCLASS | *  | *  |    |    |    | *11|    | *  | *  |    |   |
           |<group_num> | *  |    |    |    |    | *12| *16|    |    | *13|   |
           |            |    |    |    |    |    |    |    |    |    |    |   |
 ALIGMENT  | NOSUBCLASS |    |    |    |    |    |    | *14|    |    |    | * |
           | ALINE      |    | *  | *  |    | *  |    |    |    |    |    |   |
           |            |    |    |    |    |    |    |    |    |    |    |   |
 CCOLOR    | NOSUBCLASS |    |    |    |    |    | *  |    |    |    |    |   |
           |<color_num> |    | *  |    |    |    |    |    |    |    |    |   |
-------------------------------------------------------------------------------
*/

/* Parametr subclasses: */
#define MSM_NOSUBCLASS 0  /* unnamed subclass */
#define MSM_SEGMENT    1  /* feature of sequence segment */
#define MSM_SEG_BORD   2  /* segment border */
#define MSM_FGAP       3  /* feature gap */
#define MSM_FLABEL     4  /* feature label */
#define MSM_FGROUP     5  /* feature group */
#define MSM_FORDER     6  /* feature order */
#define MSM_SLABEL     4  /* sequence label */
#define MSM_SPACE      1  /* space between ... */
#define MSM_ENDS       2  /* sequence/feature ends */
#define MSM_LABEL      4  /* style of label */
#define MSM_ALINE      1  /* the line style of for alignment displayed in 
                             the MSM_FIX style */

/* (*1) - feature segment style */
#define MSM_SEG_FORM         0xF  /* form of seqment: */
#define MSM_SEG_BOX          0       /* box */
#define MSM_SEG_LINE         1       /* line */
#define MSM_SEG_SYM_RECT     2       /* symbol rectangle */
#define MSM_SEG_SYM_DIAMOND  3       /* symbol diamond */
#define MSM_SEG_SYM_OVAL     4       /* symbol oval */
#define MSM_SEG_SYM_TRIANGLE 5       /* symbol triangle */
#define MSM_SEG_END          0xF0 /* end of segment: */
#define MSM_SEG_SHOWORIENT   0x10    /* show orientation */
#define MSM_SEG_SHOWTRUNC    0x20    /* show trancation */

/* (*2) - gap style */
#define MSM_GAP_NONE      0       /* gap invisible */
#define MSM_GAP_LINE      1       /* show gap as line */
#define MSM_GAP_ANGLE     2       /* show gap as angle */

/* (*3) - label style */
#define MSM_LABEL_NONE    255     /* no lables */
#define MSM_LABEL_TYPE    0       /* show type */
#define MSM_LABEL_CONTENT 1       /* show content */
#define MSM_LABEL_BOTH    2       /* show type:content */
#define MSM_LABEL_SUMMARY 3       /* show summary */

/* (*4) - feature order - feature order in the group */

/* (*5) - feature group - feature group number */

/* (*6) - sequence segment style likes feature segment style but
 the segment form can not be a symbol ( box or line only ) */

/*(*7) - label type for sequences ( see file sequtil.h 
 for PRINTID_ options) */

/* (*8) - "top" style */
#define MSM_STACKGENE     0x1
#define MSM_HISTORY       0x2
#define MSM_SEGMENTS      0x4

/* (*9) - space between features */

/* (*10) - maximumstring lenght */

/* (*11) - total number of groups */

/* (*12) - order number */

/* (*13) - group name */

/*(*14) - the style for the alignment*/
#define MSM_SEQHIST	1  /*show alignment as a history of a sequence*/
#define MSM_MPAIR	   2  /*multiple pairwise*/
#define MSM_MDIM	   3  /*true multiple alignment*/
#define MSM_FIXED	   4  /*the fixed stype*/

/* (*15) lable layout */
#define MSM_LABEL_TOP      0
#define MSM_LABEL_BOTTOM   1
#define MSM_LABEL_LEFT     2
#define MSM_LABEL_RIGHT    3

/* (*16) group layout */
#define MSM_SPREAD         0
#define MSM_COMPRESS       1

/**************************************************************************/
/* FUNCTION PROTOTYPES */
/**************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

extern void   LIBCALL     Nlm_InitMuskStyles   (void);
extern void   LIBCALL     Nlm_ExitMuskStyles   (void);
extern void   LIBCALL     Nlm_MuskStyleManager (void);
extern BigScalar   Nlm_GetMuskCParam    (Nlm_Int2 p_class, Nlm_Int2 p_subclass,
                                         Nlm_Int2 p_type); 
extern Nlm_Int2    Nlm_GetMuskTotalSt   (void);
extern Nlm_CharPtr Nlm_GetMuskStyleName (Nlm_Int2 styleNum);
extern Nlm_Int2    Nlm_GetMuskCurrentSt (void);
extern void        Nlm_SetMuskCurrentSt (Nlm_CharPtr name);

extern Nlm_Int2    Nlm_CopyMuskStyle    (Nlm_CharPtr name, Nlm_Int2 styleNum);
extern Boolean     Nlm_DeleteMuskStyle  (Nlm_Int2 styleNum);
extern Boolean     Nlm_EditMuskStyle    (Nlm_Int2 styleNum);
extern void        Nlm_SetMuskCParamEd  (Nlm_Int2 p_class, Nlm_Int2 p_subclass,
                                         Nlm_Int2 p_type, BigScalar val );
extern BigScalar   Nlm_GetMuskCParamEd  (Nlm_Int2 p_class, Nlm_Int2 p_subclass,
                                         Nlm_Int2 p_type); 
extern void        Nlm_FreeMuskStyleEd  (Nlm_Boolean save);
extern Nlm_FonT        Nlm_LoadMuskFont     ( Nlm_FontSpecPtr fsp, Int2 fnum, Int2Ptr fsize);

#ifdef __cplusplus
}
#endif

/**************************************************************************/
/* FUNCTION NAME DEFINITION */
/**************************************************************************/
#define InitMuskStyles    Nlm_InitMuskStyles
#define ExitMuskStyles    Nlm_ExitMuskStyles
#define MuskStyleManager  Nlm_MuskStyleManager
#define GetMuskCParam     Nlm_GetMuskCParam   
#define GetMuskTotalSt    Nlm_GetMuskTotalSt  
#define GetMuskStyleName  Nlm_GetMuskStyleName
#define GetMuskCurrentSt  Nlm_GetMuskCurrentSt
#define SetMuskCurrentSt  Nlm_SetMuskCurrentSt

#endif /* _FSTYLE_ */

/*END*/
