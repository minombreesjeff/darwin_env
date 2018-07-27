
/*  simutil.h
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
* File Name:  simutil.h
*
* Author:  Jinghui Zhang
*
* Version Creation Date: 5/25/95
*
* $Revision: 6.3 $
*
* File Description:  Utilities for sim and sim2 algorithm
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: simutil.h,v $
* Revision 6.3  1998/09/09 11:47:43  sirotkin
* MAXSEG to NLM_MAX_SEG to avoid RS6000 occasional conflict
*
* Revision 6.2  1997/09/16 21:51:29  kans
* uncommented SIM4ALN_choice (JZ)
*
* Revision 6.1  1997/09/16 20:16:41  zjing
* move AddAlignmentInfo to jzmisc.c
*
* Revision 5.3  1997/03/13 21:01:59  zjing
* get rid of new variable
*
 * Revision 5.2  1996/09/19  22:27:35  zjing
 * make the SIM3_choice global, added the end_gap_choice
 *
 * Revision 5.1  1996/06/19  14:45:59  zjing
 * move the function AddAlignInfoToSeqAnnot from pobutil to simutil
 *
 * Revision 4.2  1996/02/23  18:06:59  zjing
 * Add the option for heristic methods only to run sim3
 *
 * Revision 4.1  1996/01/04  18:23:00  zjing
 * .
 *
*
*
* ==========================================================================
*/

#ifndef _SIMUTIL_
#define _SIMUTIL_

#ifndef _NCBI_SeqUtil_
#include <sequtil.h>
#endif

#ifndef _NCBI_Seqport_
#include <seqport.h>
#endif

#ifndef _NCBI_Seqalign_
#include <objalign.h>
#endif

#ifndef _NCBI_Seqloc_
#include <objloc.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif



/********************************************************************
*
*	default values for SIM search
*
********************************************************************/
#define DEFAULT_M       ((FloatHi)1.0)
#define DEFAULT_I       ((FloatHi)-1.0)
#define DEFAULT_V       ((FloatHi)-1.0)
#define DEFAULT_O       ((FloatHi)6.0)
#define DEFAULT_E       ((FloatHi)0.2)
#define DEFAULT_PAM_O   ((FloatHi) 12.0)
#define DEFAULT_PAM_E   ((FloatHi)4.0)

#define NLM_MAXSEG 10000                    /**maximal number of segments in one**/
#define SIZE_MATRIX     128

typedef struct psimpam {	/*for parameters of sim1*/
	FloatHi M_val;		/*cost of a matched pair, for DNA sequence*/
	FloatHi I_val;		/*cost of transition, for DNA sequence*/
	FloatHi V_val;		/*cost of transversion, for DNA sequence*/
	FloatHi O;		/*gap open penalty*/
	FloatHi E;		/*gap extension penalty*/
	CharPtr pam_file;	/*file name for PAM. NULL=PAM200*/
	Boolean def_gap;	/*if TRUE, use the default gap penalty*/
}PSimPam, PNTR PSimPamPtr;
Boolean DefaultPSimPam PROTO((PSimPamPtr pspp));

/***************************************************************************
***
*       CSIM(): compute K top alignment between two sequence locations (loc1,
*       loc2).
*       loc1, loc2: tow sequence locations. For DNA sequences, if the strand
*                   of loc2 is unknown, it will search in both orientations
*       K:              the number of alignments to compute
*       cut_off:        the cut off score for each alignment
*       sim1_pam:       Parameters for SIM1. set to NULL for default
*
*****************************************************************************
***/
SeqAlignPtr CSIM PROTO((SeqLocPtr loc1, SeqLocPtr loc2, Int4 K, FloatHi cut_off, PSimPamPtr sim1_pam));



/********************************************************************
*
*	default values for SIM2 search
*			      ^
*
********************************************************************/
#define SIM2_cutoff	0.0
#define SIM2_mismatch	0.1
#define SIM2_gap_open	3.0
#define SIM2_gap_ext	0.2
#define SIM2_mismatch_2	-1.0
#define SIM2_gap_open_2	3.0
#define SIM2_gap_ext_2	0.5
 
#ifdef WIN_MSWIN
#define SIM2_word 6
#else
#define SIM2_word	8 
#endif
      
#ifdef WIN_MSWIN
#define MAX_WINLEN	10000
#endif



#define DIGIT 		10.0	/*used in penatly. default*digit=search_val*/

/************************************************************************
*
*	FilterProc(minlen, zscore, score): filtering the output from SIM2
*	minlen: minimun length of the alignment
*	zscore: the minimum % of mismatches
*	score: the minimum of the sim2 score. 
*
************************************************************************/
typedef Boolean (*FilterProc)(Int4, FloatHi, FloatHi);



/***********************************************************************
*	simpan: for loading the searching parameters for SIM2
*
***********************************************************************/
typedef struct simpam{
	FloatHi cutoff;	/*the cutoff score*/
	Int4 mismatch;	/*penalty for mismatches*/
	Int4 gap_open;	
	Int4 gap_ext;
	Int4 mismatch_2;
	Int4 gap_open_2;	
	Int4 gap_ext_2;
	Int4 word;
}SimPam, PNTR SimPamPtr;

/*********************************************************************
*
*	DefaultSimPan(spp)
*	load the default value to spp. 
*	return FALSE if spp == NULL
*
*********************************************************************/
Boolean DefaultSimPam PROTO((SimPamPtr spp));

#define SUB ((Uint1)0)  /*op types within the edit script*/
#define INS ((Uint1)1)
#define DEL ((Uint1)2)

/* EditScript is the internal representation of an alignment in SIM3. */
typedef struct edit_script {
	Uint1 op_type;	/* SUB, INS, or DEL */
	Int4 num;	/* Number of operations */
	struct edit_script PNTR next;
} EditScript, PNTR EditScriptPtr;

EditScriptPtr CreateEditScript PROTO((Int4Ptr S, Int4 M, Int4 N));

EditScriptPtr Script_free PROTO((EditScriptPtr head));

/*
*	convert EditScript Structure into a Seq-align
*
*/
SeqAlignPtr ConvertEditScript PROTO((EditScriptPtr esp, SeqLocPtr slp1, SeqLocPtr slp2, Int4 offset_1, Int4 offset_2, Boolean show_end_gap));


/*******************************************************************
*
*	recordends: structure of saving the end points of the 
*	alignment
*
*******************************************************************/
typedef struct recordends{
	Int4 start, stop;	/*the range of ends*/
	Uint1 strand;		/*the orientation */
	FloatHi gscore;		/*Greg's score, not in use*/ 
	FloatHi zscore;		/*the % of mismatch*/
	FloatHi  score;		/*the SIM2 score*/
	Int2 count;		/*number of sequence hits*/
	Char name[20];		/*name of the hit sequence*/
}RecordEnds, PNTR RecordEndsPtr;



/********************************************************************
	the different output type for SIM2. 
*********************************************************************/
#define OUTPUT_ALIGN	1	/*get Seq-align only*/
#define OUTPUT_ENDS	2	/*get ends only*/
#define OUTPUT_BOTH	3	/*get both the ends and the Seq-aligns*/



/*********************************************************************
*
*	SIM2(cslp1, cslp2, k, spp, ends_list, output_type, filter)
*	Computes alignment between Seq-loc cslp1 and cslp2. return the 
*	Seq-align object
*
*	cslp1: the Seq-loc for the first sequence
*	cslp2: the Seq-loc for the second sequence
*	k:	number of alignment to compute
*	spp: parameters for alignment. If NULL, use the default values
*	ends_list: stores the list of recordends when OUTPUT_ENDS or
*	OUTPUT_BOTH is selected
*	output_type: types for SIM2 output
*	filter: filtering low values based on sequence similarity and scores
*
***********************************************************************/
	
SeqAlignPtr SIM2 PROTO((SeqLocPtr cslp1, SeqLocPtr cslp2, Int4 k, SimPamPtr spp, ValNodePtr PNTR ends_list, Uint1 output_type, FilterProc filter));



/**********************************************************************
*
*	SIM2ALN(cslp1, cslp2, K)
*	Computes top K alignment beteeen cslp1 and cslp2, return the Seq-align
*	object only
*
***********************************************************************/
SeqAlignPtr SIM2ALN PROTO((SeqLocPtr cslp1, SeqLocPtr cslp2, Int4 K));



/**********************************************************************
*
*	SIM2ENDS(cslp1, csp2, K, filter)
*	Computes top K alignment between cslp1, cslp2 and return the 
*	a linked list (ValNodePtr) of RecordEnds
*	filter: filter based on percent identity. set to NULL when 
*	it is not required
*
**********************************************************************/
ValNodePtr SIM2ENDS PROTO((SeqLocPtr cslp1, SeqLocPtr cslp2, Int4 K, FilterProc filter));


/********************************************************************** 
*
*       falign(slp1, slp2, A, B, w, k, r, o, e) 
*       compute the diagnols between slp1 and slp2. It is the first  
*       part of sim2. returns the Dense-Diag as Seq-align 
*       A, B: string of Char for slp1 and slp2. Can be set to NULL as well 
*       w: word size
*       k: top K alignment 
*       r: penalty for replacement
*       o: penalty for open gap 
*       e: penalty for gap extension 
* 
**********************************************************************/ 
SeqAlignPtr falign PROTO((SeqLocPtr slp1, SeqLocPtr slp2, CharPtr A, CharPtr B, Int4 w, Int4 k, Int4 r, Int4 o, Int4 e));

/************************************************************************
*
*       Region(sap, loc1, loc2, A, B, ms, G, H, Width, new_list,
*       output_type, filter)
*       second part of SIM2. produces gaped alignment from HSPs.
*	returns Seq-align of type DenseSeg
*       sap: the HSPs stored as DenseDiag
*       loc1: the first sequence
*       loc2: the seconde sequence
*       A, B: strings of Chars for loc1, loc2. Can be set to NULL
*       ms: mismatch score
*       G: open gap
*       H: gap extension
*       Width: set to -1, I don't know what is the purpose  
*       new_list: to store the ends of the alignment 
*       otuput_type: determine get ends, or Seq-align or both? 
*       filter: filter based on the mismatch rate 
* 
************************************************************************/ 
SeqAlignPtr Region PROTO((SeqAlignPtr sap, SeqLocPtr loc1, SeqLocPtr loc2, CharPtr A, CharPtr B, Int4 ms, Int4 G, Int4 H, Int4 Width, ValNodePtr PNTR new_list, Uint1 output_type, FilterProc filter));



/* Prototype - added on Feb. 22, 1994, functions from C.M. Chao */
extern void fatal(CharPtr);
extern void fatalf(CharPtr, CharPtr);
extern FILE *ckopen(CharPtr, CharPtr);
extern CharPtr ckalloc(Int4);
extern Int4 strsame(CharPtr, CharPtr);




/*####################################################################
#
#	little utilities used in sim and sim2
#
####################################################################*/

/********************************************************************** 
* 
*       write_sim_output(align, out_name)
*       Create a Seq-annot for the list of align and save the Seq-annot 
*       to the file out_name. 
*       FREE the Seq-align and returns a NULL pointer 
* 
**********************************************************************/ 
SeqAlignPtr write_sim_output PROTO((SeqAlignPtr align, CharPtr out_name));


/********************************************************************
* 
*       check_strand_mol(loc, is_dna) 
*       if the bioseq for loc is DNA, *is_dna = TRUE 
*       return TRUE for computing both strand, FALSE for one strand
*
********************************************************************/ 
Boolean check_strand_mol PROTO((SeqLocPtr loc, BoolPtr is_dna));


/******************************************************************
****
*       Change_Loc_Strand(loc, strand) 
*       Change the loc strand to strnad. 
*       only works for SEQLOC_INT.  
*       return FALSE for fail
*
*******************************************************************
***/   
Boolean Change_Loc_Strand PROTO((SeqLocPtr loc, Uint1 strand));


/***********************************************************************
*
*       make_sim_seq(slp, is_sim2, t_seq) 
*       return the instatiated sequence made from slp for sim2/sim search.  
*       sequence starts from 1 of the array
*       if(t_seq !=NULL), load data to t_seq, return t_seq 
*       else create a new array.
*       t_seq is used for the saving of time in computing both strand 
* 
***********************************************************************/ 
CharPtr make_sim_seq PROTO((SeqLocPtr slp, Boolean is_sim2, CharPtr t_seq));


/***********************************************************************
*
*	make_align(x, y, n, score, SegScores, loc1, loc2)
*	x, y is the two arrays storing the start, stop of the aligned seg
*	for the two sequences
*	score: the score of the alignment
*	SegScores: the scores for each aligned seg
*	loc1: the Seq-loc of the first sequence
*	loc2: the Seq-loc of the second sequence
*
*	return Seq-align made from the above data
*
************************************************************************/
SeqAlignPtr make_align PROTO((Int4Ptr x, Int4Ptr y, Int2 n, FloatHi score, FloatHiPtr SegScores, SeqLocPtr loc1, SeqLocPtr loc2));


/**********************************************************************
* 
*       link_align(new, head) 
*       link the new align to the end of head align. return the  
*       start of the linked chain
*
**********************************************************************/ 
SeqAlignPtr link_align PROTO((SeqAlignPtr a_new, SeqAlignPtr head));

/*********************************************************************** 
* 
*       free_align_list(head) 
*       Free all the Seq-align linked to head
*       return NULL 
*
**********************************************************************/ 
SeqAlignPtr free_align_list PROTO((SeqAlignPtr head));

/******************************************************************* 
* 
*       sort_align_list(align) 
*       sort the list of Seq-align to the descending order of alignment 
*       score
*
*******************************************************************/ 
SeqAlignPtr sort_align_list PROTO((SeqAlignPtr align));


/****************************************************************** 
* 
*       get_top_K_alignment(align, K, cut_off) 
*       get the top K SCORED alignment and all the scores should be 
*       above the cutoff 
*       align   Seq-align
*       K       the number of alignment to keep 
*       cutoff  the cut off score
* 
*******************************************************************/ 
SeqAlignPtr get_top_K_alignment PROTO((SeqAlignPtr align, Int4 K, FloatHi cut_off));



/*****************************************************************
*
*       get_align_score(sap, score) 
*       get the score from the alignment. return FALSE if there is  
*       no score. 
*       sap: Seq-align
*       score: for laoding the score 
* 
*****************************************************************/
Boolean get_align_score PROTO((SeqAlignPtr sap, FloatHiPtr score));



/*##################################################################
#
#	functions related to merging the record-ends as the results
#	of alu search. Still a little fishy
#
####################################################################*/


/********************************************************************
*
*       LoadNewEnds()
*       load the related RecordEnds info to the structrue and link it  
*       to the header of the list. (This is only for convienience) 
*
*********************************************************************/
ValNodePtr LoadNewEnds PROTO((Int4 start, Int4 stop, Uint1 strand, FloatHi score, FloatHi zscore, CharPtr seq_name, Boolean sort_zscore, ValNodePtr PNTR ends_list));


/*******************************************************************
*
*       getp_top_K(head, K) 
*       get only the top K of the list, free the others.  
*       The top K is simplily determined by the order in the list
*
********************************************************************/
ValNodePtr get_top_K PROTO((ValNodePtr head, Int4 K));


/****************************************************************** 
* 
*       merge_two_list(oldlist, newlist)
*       merge any overlapping piece of RecordEnds in the two list 
*       link the new ends to the oldlist
*       resort the oldlist with the region 
*       return the head of the oldlist 
* 
*******************************************************************/
ValNodePtr merge_two_list PROTO((ValNodePtr oldlist, ValNodePtr newlist));


/********************************************************************** 
* 
*       SortEndsList(vnp) 
*       sort the list of RecordEnds to the ascending order of the  
*       LOCATION 
*
**********************************************************************/ 
ValNodePtr SortEndsList PROTO((ValNodePtr vnp));


/******************************************************************** 
* 
*       CleanNewList(newlist) 
*       merge any overlappign RecordEnds in the current list 
* 
********************************************************************/
ValNodePtr CleanNewList PROTO((ValNodePtr newlist));

/******************************************************************
*
*	prepare_align_data(buf, bsp)
*
*	convert data in buf in the format used by SIM ans SIM2 to a 
*	Seq-loc in a Bioseq
*	buf: a special formatted buffer which can be converted into 
*	a Seq-loc
*	bsp: store the Bioseq data
*	return the Seq-loc made from the buf. NULL if the format is 
*	not correct or the Bioseq is NULL
*
******************************************************************/
SeqLocPtr prepare_align_data PROTO((CharPtr buf, BioseqPtr PNTR bsp));


SeqAlignPtr SIM3ALN PROTO((SeqLocPtr cslp1, SeqLocPtr cslp2, Int4 limit));

SeqAlignPtr SIM3ALN_heuristic PROTO((SeqLocPtr cslp1, SeqLocPtr cslp2, Int4 limit));

SeqAlignPtr SIM3ALN_choice PROTO((SeqLocPtr cslp1, SeqLocPtr cslp2, Int4 limit, Boolean heuristic_only, Boolean show_end_gap));
SeqAlignPtr SIM4ALN_choice PROTO((SeqLocPtr cslp1, SeqLocPtr cslp2, Int4 limit, Int4 word_size));



#define DEFAULT_W       8	/*the default word size used by sim4*/ 
#define DEF_LIMIT 1000		/*the default value of the limit, used by sim3 and sim4*/



#ifdef __cplusplus
}
#endif

#endif

