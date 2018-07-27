/* ===========================================================================
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
* File Name:  salsap.h
*
* Author:  Colombe Chappey
*
* Version Creation Date:   1/27/96
*
* $Revision: 6.3 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: salsap.h,v $
* Revision 6.3  1999/11/24 21:24:28  vakatov
* Fixed for the C++ and/or MSVC DLL compilation
*
* ==========================================================================
*/

#ifndef _SALSAP_
#define _SALSAP_

#include <salsa.h>
#include <seqport.h>
/*
  Put in all the transfered Functions 
 */
#include <salpacc.h> 
/* For prototype for SeqAlignGapCount Function */
#include <salpstat.h> 

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

NLM_EXTERN SeqIdPtr LIBCALL SeqAlignIDList (SeqAlignPtr salp);

/* Used in salsa.c .. mapped to Find SeqIdInSeqAlign*/
NLM_EXTERN Int2 LIBCALL position_inIdlist(SeqIdPtr sip, ValNodePtr list);

NLM_EXTERN Boolean LIBCALL FindSeqIdinSeqAlign (SeqAlignPtr salphead, SeqIdPtr sip);

	
NLM_EXTERN SeqAlignPtr LIBCALL build_seqalign_fromstart (Int2 dim, Int2 numseg, 
	SeqIdPtr sip, Int4Ptr starts, Int4Ptr lens);

NLM_EXTERN SeqAlignPtr LIBCALL SeqLocToFastaSeqAlign (ValNodePtr vnp);

NLM_EXTERN SeqAnnotPtr LIBCALL LocalAlignToSeqAnnotDimn (ValNodePtr seqvnp, SeqIdPtr seqsip, 
	ValNodePtr fromp, Int2 nbseq, Int4 lens, ValNodePtr strands, 
	Boolean trunc_emptyends);

NLM_EXTERN DenseDiagPtr LIBCALL DenseDiagCreate (Int4 dim, SeqIdPtr id, Int4Ptr starts, 
	Int4 len, Uint1Ptr strands, ScorePtr scores);

NLM_EXTERN Pointer LIBCALL FindSeqAlignInSeqEntry (SeqEntryPtr sep, Uint1 choice);

NLM_EXTERN SeqAlignPtr LIBCALL is_salp_in_sap (SeqAnnotPtr sap, Uint1 choice);

NLM_EXTERN Boolean LIBCALL is_dim1seqalign (SeqAlignPtr salp);
	
NLM_EXTERN Boolean LIBCALL is_dim2seqalign (SeqAlignPtr salp);


NLM_EXTERN Boolean LIBCALL SeqAlignSeqLocComp (SeqAlignPtr salphead, ValNodePtr vnp);


NLM_EXTERN Boolean LIBCALL get_pos_from_salp (SeqAlignPtr salp, Int4 pos, 
	Int4 PNTR offset, Int4Ptr PNTR startp, Int4Ptr PNTR lenp, 
	Int4 PNTR numseg);

/**SeqLoc**/
NLM_EXTERN ValNodePtr LIBCALL SeqLocListFromSeqAlign (SeqAlignPtr salp);

NLM_EXTERN SeqLocPtr LIBCALL SeqLocFromSeqAlign (SeqAlignPtr salp, SeqIdPtr sip);

NLM_EXTERN SeqLocPtr LIBCALL SeqLocMixFromSeqAlign (SeqAlignPtr salp, SeqIdPtr sip);

NLM_EXTERN ValNodePtr LIBCALL SeqLocListOfBioseqsFromSeqAlign (SeqAlignPtr salp);

/**Alignment Score**/
NLM_EXTERN Int4 LIBCALL SeqAlignBestScore (SeqAlignPtr salp);


/**Sequence**/
NLM_EXTERN Int4 LIBCALL readbuff_fromseqalign (SeqPortPtr spp, SeqAlignPtr salp, Int2 index, 
	CharPtr buffer, Int4 from, Int4 to, Int4 offset, Boolean strand);

/**misc**/
NLM_EXTERN Boolean LIBCALL is_fasta_seqalign (SeqAlignPtr salp);

/**
*Functions taking a SeqAlign and returning a pointer to the "same" SeqAlign
**/

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignMerge (SeqAlignPtr salp1, SeqAlignPtr salp2, 
	Boolean return_salp);
	
NLM_EXTERN SeqAnnotPtr LIBCALL SeqAnnotMerge (SeqAnnotPtr sap1, SeqAnnotPtr sap2, 
	Boolean return_salp);

NLM_EXTERN DenseDiagPtr LIBCALL DenseDiagLink (DenseDiagPtr *ddp_head, DenseDiagPtr ddp);

NLM_EXTERN DenseDiagPtr LIBCALL DenseDiagInsert (DenseDiagPtr ddp_before, DenseDiagPtr ddp);

NLM_EXTERN DenseDiagPtr LIBCALL DenseDiagPrecede (DenseDiagPtr ddp_after, DenseDiagPtr *ddp);

NLM_EXTERN DenseDiagPtr LIBCALL DenseDiagLinkSort (DenseDiagPtr *ddp_head, DenseDiagPtr ddp);

/**Sort**/
NLM_EXTERN SeqAlignPtr LIBCALL SortSeqAlign (SeqAlignPtr PNTR salp);

NLM_EXTERN SeqAlignPtr LIBCALL SortSeqAlignFromList (SeqAlignPtr salp, Int2Ptr sortlst);

/**SeqId**/

NLM_EXTERN Boolean LIBCALL SeqAlignIDCache (SeqAlignPtr salphead, SeqIdPtr sip);

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignIDUncache (SeqAlignPtr salphead, SeqIdPtr sip);

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignIDUncacheAll (SeqAlignPtr salphead);

/**Extend**/
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignEndExtend (SeqAlignPtr sap, Int4 start1, Int4 start2, 
	Int4 stop1, Int4 stop2, Int4 x1, Int4 y1, Int4 x2, Int4 y2, 
	Uint1 strand1, Uint1 strand2);


/**Delete, Truncate**/
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignDeleteByLoc (SeqLocPtr slp, SeqAlignPtr salp);

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignTrunc (SeqAlignPtr salp, Int4 from, Int4 to);
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignTrunc2 (SeqAlignPtr salp, Int4 from, Int4 to);

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignMapOnFirstSeq (SeqAlignPtr salp);
	
/**Clean**/
NLM_EXTERN SeqAlignPtr LIBCALL CleanStrandsSeqAlign (SeqAlignPtr salp);
	
/**
*Functions taking a SeqAlign and returning a new SeqAlign
**/
/**Duplicate**/
NLM_EXTERN DenseDiagPtr LIBCALL DenseDiagDup (DenseDiagPtr ddp);

/**Transfer of Format**/
NLM_EXTERN SeqAlignPtr LIBCALL  DenseSegToDenseDiag (SeqAlignPtr salp);

/**Nuc/AA**/
NLM_EXTERN SeqAlignPtr LIBCALL aaSeqAlign_to_dnaSeqAlign (SeqAlignPtr salp, ValNodePtr vnp, 
	ValNodePtr framep);

NLM_EXTERN SeqAnnotPtr aaSeqAnnot_to_dnaSeqAnnotFunc
(SeqAnnotPtr PNTR sapnahead, 
 SeqAlignPtr salpnew, ValNodePtr vnp, ValNodePtr framep);
	
/**
*SeqAlign and SeqEntry
**/
NLM_EXTERN void LIBCALL ReplaceSeqAlignInSeqEntry (Uint2 entityID, Uint2 itemID, 
	SeqAlignPtr salp);

/*******************************************************/

NLM_EXTERN SeqAnnotPtr LIBCALL CompSeqAnnotFree (SeqAnnotPtr sap);
NLM_EXTERN SeqAlignPtr LIBCALL CompSeqAlignFree (SeqAlignPtr salp);
NLM_EXTERN SeqAnnotPtr LIBCALL SeqAlignBoolSegCpy (SeqAnnotPtr sap, Int4 from, Int4 to);
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignDenseSegToBoolSeg (SeqAlignPtr salp);
NLM_EXTERN SeqAnnotPtr LIBCALL SeqAnnotDenseSegToBoolSeg (SeqAnnotPtr sap);
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignBoolSegToDenseSeg (SeqAlignPtr salp);
NLM_EXTERN SeqAnnotPtr LIBCALL SeqAnnotBoolSegToDenseSeg (SeqAnnotPtr sap);
NLM_EXTERN void LIBCALL CompSeqAlignPrint (SeqAlignPtr salp);

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignDupRegion (SeqAlignPtr salp, Int2 to_numseg, Int4 subseg, Boolean first_part);
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignDupAdd (SeqAlignPtr *salp_head, SeqAlignPtr salp, Int2 to_numseg, Int4 subseg, Boolean first_part);

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignExtend (SeqAlignPtr salp1, SeqAlignPtr salp2);
NLM_EXTERN SeqAlignPtr LIBCALL DeleteRegion (SeqIntPtr sip, SeqAlignPtr salp);

NLM_EXTERN SeqAlignPtr LIBCALL DenseDiagToDenseSegFunc (SeqAlignPtr salp, Boolean add_ends);
NLM_EXTERN SeqAlignPtr LIBCALL DenseDiagToDenseSeg (SeqAlignPtr salp, Boolean add_ends);

NLM_EXTERN SeqAnnotPtr multseqalign_from_pairseqalign (SeqAlignPtr salp);
NLM_EXTERN SeqAlignPtr PairSeqAlign2MultiSeqAlign (SeqAlignPtr salp);
NLM_EXTERN Int2 LIBCALLBACK MultSeqAlignFromPairSeqAlign (Pointer data);

NLM_EXTERN SeqAlignPtr LIBCALL multseqalign_to_pairseqalign (SeqAlignPtr salp);

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
