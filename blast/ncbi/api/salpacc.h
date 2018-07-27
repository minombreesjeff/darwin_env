
/* ===========================================================================
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
 * $Id: salpacc.h,v 6.17 2004/09/07 19:25:37 bollin Exp $
 Collection of SeqAlign Accession utilities.
 Maintainer: Hugues Sicotte
 Authors of the original routines: Hugues Sicotte, Colombe Chappey, Tom Madden, Jinghui Zhang
*/
#ifndef _NCBI_salpacc_
#define _NCBI_salpacc_

#include <ncbi.h>
#include <objalign.h>
#include <objsset.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif


/* Function that returns a pointer to the SeqId chain already in the SeqAlign.
   For SeqAlign Types with Optional SeqId chain, this function creates the SeqIds 
   puts them in the seqalign and returns a pointer to it.
   This SeqIds Chain can only be used while the SeqAlign exist (not freed)
*/
NLM_EXTERN SeqIdPtr LIBCALL SeqIdPtrFromSeqAlign(SeqAlignPtr salp);
NLM_EXTERN SeqIdPtr LIBCALL SeqAlignId (SeqAlignPtr salp, Int2 index);
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignIdReplace (SeqAlignPtr salp, Int2 index, SeqIdPtr newsip);
NLM_EXTERN Boolean LIBCALL SeqAlignFindSeqId (SeqAlignPtr salphead, SeqIdPtr sip);



NLM_EXTERN Int4 LIBCALL SeqAlignLength (SeqAlignPtr salp);
NLM_EXTERN Int4 LIBCALL SeqAlignLengthForId (SeqAlignPtr salp, SeqIdPtr sip);

/*
  returns the Strand of the "indexed" (0,1,..) sequence.
  Returns the first Strand that is not unknown.. and
  assumes that all strands are the same in that SeqAlign Node.
 */
NLM_EXTERN Uint1 LIBCALL SeqAlignStrand (SeqAlignPtr salp, Int2 index);
NLM_EXTERN Int4 LIBCALL SeqAlignStart (SeqAlignPtr salp, Int2 index);

NLM_EXTERN Int4 LIBCALL SeqAlignStop (SeqAlignPtr salp, Int2 index);

NLM_EXTERN Uint1 LIBCALL SeqAlignMolType (SeqAlignPtr salp);

NLM_EXTERN SeqAnnotPtr LIBCALL SeqAnnotForSeqAlign (SeqAlignPtr salp);




/**********************************************************************
*
*       SeqAlignLink(head, new)
*       link the new align to the end of head align. return the
*       start of the linked chain
*
**********************************************************************/

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignLink(SeqAlignPtr head, SeqAlignPtr a_new);

/***********************************************************************
***    
***    SeqLocListFromSeqAlign
***      read SeqAlignPtr
***      return list of ValNodePtr-SeqLocPtr
***
************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocListFromSeqAlign (SeqAlignPtr salp);

/*
  Returns A chain of "dim" SeqLocs representing the area covered by a
  single SeqAlign
 */

NLM_EXTERN SeqLocPtr LIBCALL SeqLocFromSeqAlign (SeqAlignPtr salp, SeqIdPtr sip);




NLM_EXTERN void LIBCALL SeqAlignScoreRead(SeqAlignPtr seqalign,Int4Ptr Score,FloatHi PNTR Bit_Score,FloatHi PNTR Evalue);

NLM_EXTERN ScorePtr LIBCALL SeqAlignScorePtrGet(SeqAlignPtr salp);


NLM_EXTERN FloatHi LIBCALL ScoreRead(ScorePtr score);


/*
  Quick SeqAlign Output function: Can be used while Debugging 
 */
NLM_EXTERN void LIBCALL SeqAlignWrite(SeqAlignPtr salp,CharPtr file);

/*
  Subroutine to return the order of a Bioseq in the SeqId List of
  an alignment 

   */
NLM_EXTERN void LIBCALL BioseqOrderInSeqIdList(SeqIdPtr sip,BioseqPtr bsp,Int4Ptr order);
/*
  Subroutine to return the order (position) of two
  sequences in a possibly multiple alignment.

 */

NLM_EXTERN void LIBCALL SeqAlignBioseqsOrder(SeqAlignPtr align,BioseqPtr query_bsp,BioseqPtr subject_bsp,Int4Ptr a_order, Int4Ptr b_order);

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignListFree (SeqAlignPtr salp);

NLM_EXTERN Boolean LIBCALL SeqAlignStartStop(Int4Ptr a_start, Int4Ptr a_stop, Int4Ptr b_start, 
			   Int4Ptr b_stop, 
			   SeqAlignPtr align, BioseqPtr query_bsp, BioseqPtr subject_bsp);

/*
  Find the Absolute Starts and Stops of a Linked List of SeqAlign.
 */
NLM_EXTERN void LIBCALL SeqAlignListStartStop(Int4Ptr qstart,Int4Ptr qstop,Int4Ptr sstart,Int4Ptr sstop,SeqAlignPtr salp, BioseqPtr query,BioseqPtr subject);

NLM_EXTERN Boolean SeqAlignStartStopById(SeqAlignPtr align, SeqIdPtr id, Int4Ptr start, Int4Ptr stop, Uint1Ptr strand);

/* Count the number of segments in the SeqAlign */
NLM_EXTERN Uint4 LIBCALL SeqAlignCountSegs(SeqAlignPtr salp);

/* Add SeqAlign in a SeqEntry */
NLM_EXTERN void SeqAlignAddInSeqEntry (SeqEntryPtr sep, SeqAnnotPtr sap);
NLM_EXTERN SeqAlignPtr SeqAlignExtractByIds(SeqAlignPtr PNTR align, SeqIdPtr sip_1, SeqIdPtr sip_2);

NLM_EXTERN Int4 SeqAlignCount(SeqAlignPtr align);

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif

