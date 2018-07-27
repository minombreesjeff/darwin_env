

/***********************************************************************
*
*	Header file for satutil.c (for copy Seq-feat based on sequence
*	alignment
*
************************************************************************/
#ifndef _SATUTIL_
#define _SATUTIL_

#include <ncbi.h>
#include <objfeat.h>
#include <objseq.h>
#include <objloc.h>
#include <objalign.h>
#include <sequtil.h>
#include <edutil.h>
#include <seqport.h>
#include <seqmgr.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#define DEFAULT_GAP_CHOICE 0
#define IGNORE_GAP_CHOICE 1
#define TAKE_GAP_CHOICE 2
#define MAX_GAP_LEN_BY_DEFAULT 10	/*the maximum allowd length of gap*/

/******************************************************************
*
*       CopyFeatureFromAlign(source_sfp, target_id, source_id, align,
*       gap_choice, check_trunc
*
*       Copy the source_sfp to the target sequence based on their
*       alignment
*       source_sfp: Seq-feat of the source
*       target_id:  Seq-id of the target
*       source_id:  Seq-id of the source in the alignment. This is
*       neccessary when the source_sfp may composed from different
*       segment of a Bioseq-set
*       align:      The Seq-align that contains alignment of the source
*       and the target
*       gap_choice: choice to select to map the gap to the new feature
*       can have default, ignore or take
*       check_trunc: load the fuzziness of the new Seq-feat
*
******************************************************************/
NLM_EXTERN SeqFeatPtr CopyFeatureFromAlign PROTO((SeqFeatPtr source_sfp, SeqIdPtr target_id, SeqIdPtr source_id, SeqAlignPtr align, Uint1 gap_choice, Boolean check_trunc));

/***************************************************************
*
*       Load a protein Bioseq to replace the Bogo product
*       sfp: the coding region feature
*       new_prot_id: if NULL a new default will be generated
*
***************************************************************/
NLM_EXTERN Boolean LoadProteinForCdRegion PROTO((SeqFeatPtr sfp, SeqIdPtr new_prot_id));


/********************************************************
*
*       functions related to the Bogo product
*
*********************************************************/

/*make the bogo product UserObject and link it to the chain*/
NLM_EXTERN void make_bogo_product PROTO((UserObjectPtr PNTR head));


/************************************************************* 
* 
*       check if the UserObject contains the identifyer for the 
*       BOGO product 
*
**************************************************************/ 
NLM_EXTERN Boolean IS_BOGO_Product PROTO((UserObjectPtr uop));


/************************************************************ 
* 
*       DeleteBogoProduct(head)
*       Delete the UserObject of Bogo Product from the chain 
* 
**************************************************************/
NLM_EXTERN void DeleteBogoProduct PROTO((UserObjectPtr PNTR head));

/**********************************************************************
*
*	map_one_location()
*	map the source_loc to the target by the Sequence alignment
*	source_loc:	the source Seq-loc
*	align:		the alignment between the source and the target
*	target_sip:	the Seq-id for the target 
*	gap_choice:	choice for mapping the gaps
*	r_start, r_stop:	the real start, stop value in the source 
*			sequence during the mapping. This value allows 
*			the mapping of truncation info for the results
*	return NULL for failure
*
*********************************************************************/
NLM_EXTERN SeqLocPtr map_one_location PROTO((SeqLocPtr source_loc, SeqAlignPtr align, SeqIdPtr target_sip, Uint1 gap_choice, Int4Ptr r_start, Int4Ptr r_stop));

/******************************************************************** 
* 
*  SeqAlignReplaceId () 
*       replaces a Seq-Id (oldId) in a Seq-Id list in a Seq-Align (salp)
*       with a new Seq-Id (newId)
* 
*********************************************************************/
NLM_EXTERN SeqAlignPtr SeqAlignReplaceId (SeqIdPtr oldId, SeqIdPtr newId, SeqAlignPtr salp); 
 

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
