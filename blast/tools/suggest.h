/*   suggest.c
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
* File Name:  suggest.c
*
* Author:  Webb Miller, Karl Sirotkin, Warren Gish, Jonathan Kans, Yuri Sadykov
*
* Version Creation Date:   3/17/94
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
*
* ==========================================================================
*/

#ifndef _SUGGEST_
#define _SUGGEST_

#include <ncbi.h>
#include <objall.h>
#include <sequtil.h>

/**************************************************
*
*    SuggestIntervals
*
**************************************************/
typedef struct struct_Suggest_intervals {
   Uint4 OBbits__;
   struct struct_Suggest_parameters PNTR   params;
   ValNodePtr   dna;
   ValNodePtr   protein;
   Int4   code;
} SuggestIntervals, PNTR SuggestIntervalsPtr;

/**************************************************
*
*    SuggestParameters
*
**************************************************/
typedef struct struct_Suggest_parameters {
   Uint4 OBbits__;
#define OB__Suggest_parameters_size 0

   Int4   size;
#define OB__Suggest_parameters_begin_search 1

   Int4   begin_search;
#define OB__Suggest_parameters_end_search 2

   Int4   end_search;
#define OB__Suggest_parameters_term_stop 3

   Uint1   term_stop;
} SuggestParameters, PNTR SuggestParametersPtr;

/**************************************************
*
*    SuggestError
*
**************************************************/
typedef struct struct_Suggest_error {
   Uint4 OBbits__;
   Uint2   level;
   /* following #defines are for enumerated type, not used by object loaders */
#define Suggest_error_level_none 0
#define Suggest_error_level_info 1
#define Suggest_error_level_warn 2
#define Suggest_error_level_error 3
#define Suggest_error_level_fatal 4

   CharPtr   msg;
} SuggestError, PNTR SuggestErrorPtr;




/*****************************************************************************
 * Defines
 */
#define AINT_NONE -1

#define DBINT Int4
#define DBSMALLINT Int2
#define DBTINYINT Int1

#define MAX_AMINO	9000		/* maximum length of protein sequence */
#define MAX_DNA		1000000
#define MAX_LINE	200			/* maximum length of input line */
#define MAX_EXONS	5000			/* maximum number of exons */
#define WORD_SIZE	3			/* length of critical word (DO NOT CHANGE!) */

#define PlusStrand 1
#define MinusStrand 2

/*****************************************************************************
 * Typedefs
 */
typedef struct match {
/*  dna_pos must be *3 to get nucleic acid location */
	Int4 orient, fr_shift, p_pos, dna_pos, length;
/* SPLIT_CODON_FOR_UNASSIGNED the following two fields track 
		the number of bases in the split codons
   	at exon-exon boundaries */
	char left_add_bases, right_add_bases;	
} Match;

/*****************************************************************************
*
*   IdRec
*       Structure to hold sequence identifier strings
*
*****************************************************************************/
typedef struct idrec {
  union {
    CharPtr       accn;
    SeqIdPtr	pSeqId;
  } id;
  Int4          uid;
  struct idrec  PNTR next;
} IdRec, PNTR IdRecPtr;

typedef struct myintervalrec {
  Int4  from;
  Int4  to;
  Int4  orient;
  Int4  id;
  struct myintervalrec * next;
} IntRec, *IntPtr;

/*****************************************************************************
*
*   SeqRec
*       Structure to hold sequence and segment information
*
*****************************************************************************/

typedef struct seqrec {
  ByteStorePtr  rawSeq;
  ByteStorePtr  segLens;
  IdRecPtr      ids;
  CharPtr       sequence;
  CharPtr       reverse;
  Int4          length;
  Boolean       nuc;
  Boolean       lookForStop;
} SeqRec, PNTR SeqRecPtr;

/*****************************************************************************
*
*   SuggestRec
*       Structure to hold suggest interval parameters
*
*****************************************************************************/

typedef struct suggestrec {
  SeqRec   nucleotide;
  SeqRec   protein;
  CharPtr  geneticCode;
  Int4     minVal;
  Int4     maxVal;
  Int4     tooShort;
  Int4     minExon;
  Boolean  lookForStop;
} SuggestRec, PNTR SuggestRecPtr;

/*---------------------------------------------------------------------------
 * This structure is needed because suggest-client and suggest-server share
 * common part of code, but the client and the server have different "output".
 */
typedef struct suggestoutput {
    union {
	FILE		*fileOut;
	SeqAnnotPtr	pSeqAnnot;
    } out;
    Boolean	bServer;
} SuggestOutput, PNTR SuggestOutputPtr;

/*****************************************************************************
 * Globals
 */
extern Match * m_stk;
extern Int4 m_top;

extern Int4 vx[128];

/*****************************************************************************
 * Function prototipes
 */
extern void copy_m_stk_entry(Int4 to, Int4 from);
extern void fix_single_mismatches(void);
extern void find_best_set (Int4 TooShort, Int4 pro_len);
extern void remove_redundant_intervals (Int4 TooShort);
extern void fix_single_unassigned(Char *dna_seq, Char *rev_seq, Char *p_seq, Char *genetic_code);
extern Int4 combine_with_long_mismatches(Int4 TooShort);
extern void fix_overlaps_by_splice_sites(Char *dna_seq, Char *rev_seq, Char *p_seq, Char *genetic_code);
extern void ensure_consistent_set(Int4 pro_len);
/* void reverse(Int4 n); */
extern void ClearSeqRec (SeqRecPtr srp);
extern void SetBatchSuggestFrames (Char *dna_seq, Char *rev_seq, Int4 len);
extern void ClearBatchSuggestFrames (void);

Int4
Get_Genetic_Code (Int4 code, SuggestRecPtr sugrp);

void
OutProteinID(SuggestOutputPtr pSuggestOut, Int2 num);

void
OutLocation(SuggestOutputPtr pSuggestOut, Int4 num_nuc);

Boolean
InitSuggest(void);
 
Boolean
ProcessData(SuggestOutputPtr pSuggestOut, Boolean clearNucSeqRec);

#endif
