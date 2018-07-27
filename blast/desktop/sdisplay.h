/*   sdisplay.h
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
* File Name:  sdisplay.h
*
* Author:  Sergei Egorov
*
* Version Creation Date:   9/23/94
*
* $Revision: 6.0 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#ifndef _SDISPLAY_
#define _SDISPLAY_

#include <vibrant.h>
#include <picture.h>
#include <viewer.h>
#include <objall.h>
#include <sequtil.h>

typedef struct scaledata {
  Boolean  rulerNotCalculated;
  Int4     ruler;
  Int4     currentSeqLength;
  Int4     top;
} SeqScaleData, PNTR SeqScalePtr;


typedef struct seqlayoutdata {
  SegmenT      pict;
  ValNodePtr   scales;
  Boolean      showPartsLabels;
  Boolean      showFeatLabels;
  Int4         currentTop;
  Int4         currentBottom;
  Int4         currentBspLen;
} LayoutData, PNTR LayoutPtr;

#define SeqLayoutData LayoutData
#define SeqLayoutPtr LayoutPtr
#define FeatLayoutData LayoutData
#define FeatLayoutPtr LayoutPtr


#define SEQUENCE_SPACING  40
#define SEQUENCE_HEIGHT   10
#define SEQPENWIDTH        2


/* functions to populate the picture */

extern void SeqLayoutInit (SeqLayoutPtr lptr, SegmenT pict);
extern void AddSequenceToPicture (ValNodePtr vnp, SeqLayoutPtr lptr, Uint1 repr, Uint1 mol);
extern void AttachSeqPictureToViewer (VieweR vwr, SeqLayoutPtr slptr, Int4 scaleX);

#endif /* ndef _SDISPLAY_ */
