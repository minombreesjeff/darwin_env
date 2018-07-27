/*   asn2graphic.h
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
* File Name:  asn2graphic.h
*
* Author:  Eric Northup
*
* Version Creation Date:   11/8/01
*
* $Revision: 6.8 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
*
* ==========================================================================
*/

#ifndef _ASN2GRAPHIC_
#define _ASN2GRAPHIC_

#include <ncbi.h>
#include <picture.h>
#include <objseq.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" {
#endif

typedef struct graphicViewExtras {
    CharPtr alignScoreName;
    CharPtr alignScoreCutoff;
} GraphicViewExtras, PNTR GraphicViewExtrasPtr;

NLM_EXTERN SegmenT CreateGraphicView (
  BioseqPtr bsp,
  SeqLocPtr location,
  Int4 scale,
  CharPtr styleName,
  CharPtr filterName,
  CharPtr overrideLayout,
  GraphicViewExtrasPtr extras
);

/* These return a NULL-Pointer-terminated array of CharPtrs parsed from the configuration file */

NLM_EXTERN CharPtr PNTR GetStyleNameList (void);
NLM_EXTERN CharPtr PNTR GetFilterNameList (void);
NLM_EXTERN CharPtr PNTR GetAlnScoreNameList (void);
NLM_EXTERN CharPtr PNTR GetAlnScoreCutoffList (void);
NLM_EXTERN CharPtr PNTR GetLayoutNameList (void);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* _ASN2GRAPHIC_ */

