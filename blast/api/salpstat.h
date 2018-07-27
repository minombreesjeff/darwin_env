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
*/

/*****************************************************************************

Author: Hugues Sicotte
        Original Source code was coded (and can be found) while I was 
        working on "whale" program and "refseq" projects.
        Code was consolidated and polished for the dbSNP project.

*/
#ifndef _SALPSTATS_
#define _SALPSTATS_
#include <ncbi.h>
#include <objseq.h>
#include <objloc.h>
#include <objalign.h>
#include <objgen.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _salpstatsinputblock {
    Int4 window_size;
    Boolean MergeConsistentAlign;
    Boolean Mismatch;
    Boolean WorstWindow;
    Boolean LongestPerfect;
    Boolean GapList;
    Boolean BlastScores;
} SalpStatsInputBlock , *SalpStatsInputBlockPtr;

typedef struct _salpstatsresults {
    SeqIdPtr query_sip;
    SeqIdPtr subject_sip;
    Int4 qbegmatch;
    Int4 qendmatch;
    Int4 sbegmatch;
    Int4 sendmatch;
    Int4 inputqlength;
    Int4 inputslength;
    Int4 mismatches;
    Int4 identities;
    Int4 partial_matches;
    Int4 gapcount;
    Int4Ptr gaplengths;
    Int4 score;
    FloatHi bit_score;
    FloatHi E_value;
    Int4 longest_perfect;
    Int4 worst_window;
    Int4 q_possibleOverlap;
    Int4 s_possibleOverlap;
    Boolean sub_reversed_strand;
} SalpStatsResults, *SalpStatsResultsPtr;

/* provide score and search-space size*/
NLM_EXTERN ScorePtr LIBCALL MakeBlastnStyleScore(Int4 score, FloatHi N,Int4 reward);

NLM_EXTERN ScorePtr LIBCALL MakeBlastStyleScore(Int4 score, FloatHi N,Int4 reward);
/* To fix and Make Blast Compatible the score */
/* User supplies a defaults score if score is not found */
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignScoreFix(SeqAlignPtr seqalign,FloatHi score, FloatHi N,Int4 reward);

NLM_EXTERN Boolean LIBCALL DenseDiagStats(DenseDiagPtr ddp, BioseqPtr bsp_1, BioseqPtr bsp_2, Int4Ptr a_len, Int4Ptr mmatches, Int4Ptr mmismatches, Int4Ptr pmismatches, Int4Ptr mgap_num,FloatHiPtr gap, FloatHiPtr no_gap);
NLM_EXTERN Boolean LIBCALL StdSegStats(StdSegPtr ssp, BioseqPtr bsp_1, BioseqPtr bsp_2, Int4Ptr a_len, Int4Ptr mmatches, Int4Ptr mmismatches, Int4Ptr pmismatches, Int4Ptr mgap_num,FloatHiPtr gap, FloatHiPtr no_gap);
NLM_EXTERN Boolean LIBCALL PackSegStats(PackSegPtr psp, BioseqPtr bsp_1, BioseqPtr bsp_2, Int4Ptr a_len, Int4Ptr mmatches, Int4Ptr mmismatches,Int4Ptr pmismatches, Int4Ptr mgap_num,FloatHiPtr gap, FloatHiPtr no_gap);
NLM_EXTERN Boolean LIBCALL DenseSegStats(DenseSegPtr dsp, BioseqPtr bsp_1, BioseqPtr bsp_2, Int4Ptr a_len, Int4Ptr mmatches, Int4Ptr mmismatches, Int4Ptr pmismatches_ptr, Int4Ptr mgap_num,FloatHiPtr gap, FloatHiPtr no_gap);

NLM_EXTERN Boolean LIBCALL SeqAlignSimpleStats(SeqAlignPtr align, BioseqPtr bsp_1, BioseqPtr bsp_2, FloatHiPtr no_gap, FloatHiPtr gap, Int4Ptr a_len, Int4Ptr mmatches, Int4Ptr mmismatches, Int4Ptr pmismatches_ptr,Int4Ptr mgap_num);

NLM_EXTERN Int4Ptr LIBCALL DenseSegGapList(DenseSegPtr dsp,Int4Ptr gap_num);
NLM_EXTERN Int4Ptr LIBCALL DenseDiagGapList(DenseDiagPtr ddp,Int4Ptr gap_num);
NLM_EXTERN Int4Ptr LIBCALL StdSegGapList(StdSegPtr ssp,Int4Ptr gap_num);
NLM_EXTERN Int4Ptr LIBCALL PackSegGapList(PackSegPtr psp,Int4Ptr gap_num);

NLM_EXTERN Int4Ptr LIBCALL SeqAlignGapList(SeqAlignPtr align, Int4Ptr gap_num);
NLM_EXTERN Int4Ptr LIBCALL SeqAlignListGapList(SeqAlignPtr sap,Int4Ptr gap_num);

  /* Utility subroutine for SeqAlignWindowStats */


/* returns the minimum window contained in this seqalign */

NLM_EXTERN Int4 LIBCALL SeqAlignWindowStats(SeqAlignPtr align, BioseqPtr bsp_1, BioseqPtr bsp_2, 
			Int4 blen, Int4 win_size,
			Int4Ptr a_len, Boolean N_are_not_mismatches,
			Int4Ptr mmin_mismatch,Int4Ptr mmax_mismatch,
			Int4Ptr mmin_gap,Int4Ptr mmax_gap,
			Int4Ptr mmin_gapmismatch,
			Int4Ptr mmax_gapmismatch, 
			Int4Ptr mmin_match, Int4Ptr mmax_match,
			Int4Ptr mmatches, Int4Ptr mmismatches, Int4Ptr mgap_num);


NLM_EXTERN void LIBCALL BestSeqAlignStats(SeqAlignPtr seqalign, BioseqPtr query_bsp, 
                       BioseqPtr subject_bsp, FloatHi *no_gap, 
                       FloatHi * gap,
                       Int4Ptr a_length, Int4Ptr mmatches, 
                       Int4Ptr mmismatches, Int4Ptr pmismatches,Int4Ptr gap_num);/* ## */

NLM_EXTERN SeqAlignPtr LIBCALL SelectLongestSeqAlign (SeqAlignPtr salp);
NLM_EXTERN int LIBCALLBACK SeqAlignLongestFN(VoidPtr a, VoidPtr b);

NLM_EXTERN Int4 LIBCALL SeqAlignLongestSegment(SeqAlignPtr sap,Int4Ptr max_len,Int4Ptr max_qstart,Uint1Ptr max_qstrand,Int4Ptr max_sstart,Uint1Ptr max_sstrand);
NLM_EXTERN Int4 LIBCALL SeqAlignListLongestSegment(SeqAlignPtr sap,Int4Ptr max_len,Int4Ptr max_qstart,Uint1Ptr max_qstrand,Int4Ptr max_sstart,Uint1Ptr max_sstrand);

/* restrict overlapping of seqaligns up to 1 bases per positions,
   for Denseseg(blast2) only keep the hits on same strand as longest hit
   The result is resorted so that earlier segments are first.
   Also, Do not allow a query segment to be used twice.(e.g. repeats)
   Keep the query sub-alignment that is part of the longest segment.
   This is only valid for pairwise alignments.
   returns the rejected SeqAligns as salp_leftover

   */

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignNonOverlap(SeqAlignPtr salp,BioseqPtr query_bsp,BioseqPtr subject_bsp,SeqAlignPtr PNTR salp_leftover_ptr,Int4 Coverage);

NLM_EXTERN Boolean LIBCALL SeqAlignConsistentOverlap(SeqAlignPtr salptmp2, SeqAlignPtr salptmp);

NLM_EXTERN Int4 LIBCALL SeqAlignStartStopOverlap(Int4 a_start1, Int4 a_stop1, Int4 b_start1, Int4 b_stop1,Int4 a_start2, Int4 a_stop2, Int4 b_start2, Int4 b_stop2,Int4Ptr Overlap_lena, Int4Ptr Overlap_lenb);


/*
   Takes a linked list of blast SeqAligns for a pair of sequences and
   make a non-redundant SeqAlign.

   Calls SeqAlignNonOverlap

   restrict overlapping of seqaligns up to Continue bases per positions,
   for Denseseg(blast2) only keep the hits on same strand as longest hit
   The result is resorted so that earlier segments are first.
   Also, Do not allow a query segment to be used twice.(e.g. repeats)
   Keep the query sub-alignment that is part of the longest segment.
   This is only valid for pairwise alignments.

   Should change the algorithm to assume that there is an overlapping
   region.. and to thus look for that.. e.g. 
   Sort SeqAligns by query-start.
   look for overlapping using start/stops 
   .. if it finds it.. it then uses that to merge the alignments.

   Merge overlapping alignments.. (also deals with non-identical alignments
   by )

   Second Pass Sort by alignment length,
   and keep non-overlapping ones.
   */

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignNonRedundant(SeqAlignPtr salp,BioseqPtr query_bsp,BioseqPtr subject_bsp,SeqAlignPtr PNTR salp_leftover_ptr);



NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignFilterByCoverage(SeqAlignPtr salp,BioseqPtr query_bsp,SeqAlignPtr PNTR salp_leftover_ptr,Int4 Coverage,Uint2Ptr PNTR HitsPerBase,Int4Ptr last_a_start,Int4Ptr last_a_stop);

/*
  Function to count the number of mismatches in alignments
   Independent of mol type, works for both types of
   alignments.
   */

NLM_EXTERN Int4 LIBCALL SeqAlignCountMismatches(SeqAlignPtr sap,BioseqPtr query_bsp, 
		     BioseqPtr subject_bsp,
		     Int4Ptr a_length, Int4Ptr mmatches, 
		     Int4Ptr mmismatches, Int4Ptr pmismatches_ptr,Int4Ptr gap_num);

NLM_EXTERN Int4 LIBCALL SeqAlignListCountMismatches(SeqAlignPtr sap,BioseqPtr query_bsp, 
		     BioseqPtr subject_bsp,
		     Int4Ptr a_length, Int4Ptr mmatches, 
		     Int4Ptr mmismatches, Int4Ptr pmismatches_ptr,Int4Ptr gap_num);


/* 
   Returns the possible overlapping length between two sequences
   according to the "phase" of the longest (ungapped) alignment
   segment
   */

NLM_EXTERN Int4 LIBCALL SeqAlignOverlapLen(SeqAlignPtr sap,BioseqPtr query_bsp,BioseqPtr subject_bsp);
NLM_EXTERN Int4 LIBCALL SeqAlignOverlapLenByLoc(SeqAlignPtr sap,SeqLocPtr query_slp,SeqLocPtr subject_slp);


/* Returns the percentage of the sequence that matched (number of matches)
   divided by the length that could have aligned.
   Given the relative offset of the longest segment of the seqalign.
   It is maxed out at 100%.
   */

NLM_EXTERN FloatHi LIBCALL SeqAlignAlignability(SeqAlignPtr sap,BioseqPtr query_bsp, BioseqPtr subject_bsp, Int4Ptr Overlap_len,Int4Ptr aln_len,Int4Ptr virtual_mismatches,Int4Ptr mismatches,Int4Ptr gap_num,Int4Ptr matches);

NLM_EXTERN FloatHi LIBCALL AlignabilityByLoc(SeqAlignPtr sap,SeqLocPtr query_slp, SeqLocPtr subject_slp, Int4Ptr Overlap_len,Int4Ptr aln_len,Int4Ptr virtual_mismatches,Int4Ptr mismatches,Int4Ptr gap_num,Int4Ptr matches);

NLM_EXTERN void LIBCALL SeqAlignListStartStop(Int4Ptr qstart,Int4Ptr qstop,Int4Ptr sstart,Int4Ptr sstop,SeqAlignPtr salp, BioseqPtr query,BioseqPtr subject);

/* Function to return a vector of Blast-Style Score pointers.
   Windows of size win_size
   starting at offset win_offset   (usually 0)
   Each window shifted by win_step (minimum of 1)
   Match = Match score for aligned residues (e.g. 1)
   mismatch = mismatch score for aligned but mismatching residues (e.g. -3)

   As input it requires the sizeof the SearchSpace. This is
   simply (sizeof(bsp1)-word_size)*(sizeof(database)-word_size*NumberOfsequencesInDatabase)
where word_size is the blast word-size (e.g. 11)
sizeof(database) == Number of residues in database



   This program assumes that Each DenseSeg Seqalign is contigous
   and non-overlapping (e.g. that no seqalign is a concatenation of
   two local alignments). Consecutive Seqaligns in the Chain
   don't have to be contigous. This Subroutine does not try
   to resolve segments scores between two different seqaligns.

   Gaps on the edges of each window are not scored.

   if(matrix==NULL) ==> Only searches for longest segment of perfect matches;

   returns a vector of blast scores per window, and the number of windows 

   and the longest perfect segment and it's
   start position (in 0-offset)(perfect_len ==0 if there are perfect blocks)
*/

NLM_EXTERN FloatHi PNTR LIBCALL SeqAlignWindowScoreFN(SeqAlignPtr align, BioseqPtr bsp1, BioseqPtr bsp2,Int4 win_size, Int4 win_offset,Int4 win_step,Int4Ptr PNTR matrix,Int4 match, Int4 g_open, Int4 g_ext, Int4Ptr nwin,Int4Ptr perfect_length,Int4Ptr perfect_position);


/* Returns a vector of FloatHi scores
   corresponding to windows on the sequences.
   
   also returns the length and the position of the first longest block
   of perfect identity
*/
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignSameIdMerge(SeqAlignPtr salp);
NLM_EXTERN FloatHi PNTR LIBCALL SeqAlignWindowScore(SeqAlignPtr align, BioseqPtr bsp1, BioseqPtr bsp2,Int4 win_size, Int4 win_offset,Int4 win_step, Int4Ptr PNTR matrix, Int4 match, Int4 mismatch, Int4 g_open, Int4 g_ext, Int4Ptr nwin,Int4Ptr perfect_length,Int4Ptr perfect_position);

NLM_EXTERN SalpStatsInputBlockPtr LIBCALL SalpStatsInputBlockNew(void);
NLM_EXTERN SalpStatsInputBlockPtr LIBCALL SalpStatsInputBlockFree(SalpStatsInputBlockPtr param);

NLM_EXTERN SalpStatsResultsPtr LIBCALL SalpStatsResultsNew(void);
NLM_EXTERN SalpStatsResultsPtr LIBCALL SalpStatsResultsFree(SalpStatsResultsPtr param);
NLM_EXTERN SalpStatsResultsPtr LIBCALL SeqAlignStats(SeqAlignPtr salp_in, SalpStatsInputBlockPtr param,BioseqPtr query_bsp,BioseqPtr subject_bsp);

NLM_EXTERN SalpStatsResultsPtr LIBCALL SeqAlignListGlobalStats(SeqAlignPtr salp_in, SalpStatsInputBlockPtr param,BioseqPtr query_bsp,BioseqPtr subject_bsp);

NLM_EXTERN Int4 LIBCALL SeqAlignGapCount (SeqAlignPtr salp);

#ifdef __cplusplus
}
#endif

#endif
