static char const rcsid[] = "$Id: kappa.c,v 6.57 2005/04/13 17:18:02 coulouri Exp $";

/* $Id: kappa.c,v 6.57 2005/04/13 17:18:02 coulouri Exp $ 
*   ==========================================================================
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
* ===========================================================================*/

/*****************************************************************************

File name: kappa.c

Authors: Alejandro Schaffer, Mike Gertz

Contents: Utilities for doing Smith-Waterman alignments and adjusting
    the scoring system for each match in blastpgp

 $Revision: 6.57 $

 $Log: kappa.c,v $
 Revision 6.57  2005/04/13 17:18:02  coulouri
 changes to WindowsFromHSPs for tblastn composition-based statistics

 Revision 6.56  2005/01/24 15:52:54  camacho
 doxygen fixes from Mike Gertz

 Revision 6.55  2005/01/20 16:28:59  camacho
 doxygen fixes

 Revision 6.54  2005/01/18 14:54:13  camacho
 Change in tie-breakers for score comparison, suggestion by Mike Gertz

 Revision 6.53  2004/11/24 15:42:33  coulouri
 do not dereference null pointer

 Revision 6.52  2004/11/23 21:29:02  camacho
 Changes from Mike Gertz:
 - If no alignments are found, I no longer create an empty HSP list and
   process it; I just move on to the next match.
 - For tblastn, culling by containment now occurs before link_hsps as
   it should. This can not have any effect on the current output, as
   tblastn + RedoAlignmentCore is not enabled.
 - I changed some Nlm_Mallocs, etc. to MemNew.
 - Other cosmetic changes, doxygen friendly comments

 Revision 6.51  2004/11/01 20:43:48  camacho
 Added error handling for missing PSSM

 Revision 6.50  2004/10/27 21:00:02  camacho
 Change the order of elements in Score* returned by GetScoreSetFromBlastHsp to
 be consistent with score ordering in other contexts of BLAST.

 Revision 6.49  2004/09/09 13:47:47  papadopo
 from Michael Gertz: remove unnecessary check for the presence of Selenocysteine in translated sequences

 Revision 6.48  2004/08/23 19:37:38  papadopo
 From Michael Gertz: fix memory leak in getStartFreqRatios

 Revision 6.47  2004/08/23 17:12:21  papadopo
 From Michael Gertz: Backported some changes to
 getStartFreqRatios and computeScaledStandardMatrix
 from algo/blast/core/blast_kappa.c code. Also a few
 straightforward changes to getStartFreqRatios,
 because the code has diverged slightly

 Revision 6.46  2004/07/28 17:19:54  kans
 HeapSort callbacks are int LIBCALLBACK for compilation on the PC

 Revision 6.45  2004/07/27 19:59:00  papadopo
 Changes by Michael Gertz to allow RedoAlignmentCore to be used
 for tblastn searches. The current version of the code uses two heuristics:
 1) a heuristic that skips doing re-alignment for an HSP if the old
    alignment of the HSP is contained in a higher-scoring HSP that
    has already been re-aligned.
 2) a heuristic that attempts to ensure that all alignments in the list
    of redone alignments has distinct ends.
 The original code also used these heuristics, but the code that
 implements the heuristics has been rewritten. As a result,
 RedoAlignmentCore will occasionally report better-scoring alignments.
 This is still a heuristic; which HSPs are reported still depends on
 the order in which they are re-aligned.

 Revision 6.44  2004/06/23 14:53:58  camacho
 Use renamed FreqRatios structure from posit.h

 Revision 6.43  2004/06/22 14:16:56  camacho
 Use SFreqRatios structure from algo/blast/core to obtain underlying matrices' frequency ratios.

 Revision 6.42  2004/06/18 15:50:25  madden
 For secondary alignments with Smith-Waterman on, use the E-value from the X-drop alignment computed by ALIGN that has more flexibility in which positions are allowed in the dynamic program.

 Add a sort of alignments for the same query-subject pair because the use of X-drop alignments occasionally reorders such alignments.

 Changes from Mike Gertz, submitted by Alejandro Schaffer.

 Revision 6.41  2004/06/14 21:11:05  papadopo
 From Michael Gertz:
 - Added several casts where casts occur in blast_kappa.c.  These casts
      should have no real effect; the log of blast_kappa.c indicates that
      they suppress compiler warnings.
 - Changed the type of one variable that holds a score from
      Nlm_FloatHi to Int4.
 - moved the definition Kappa_ForbiddenRanges and relevant
      routines earlier in the file.
 - fixed some comments.
 - made a few (~5) changes in whitespace.

 Revision 6.40  2004/06/03 16:10:50  dondosha
 Fix in Kappa_SearchParametersNew: allocate correct number of rows for matrices

 Revision 6.39  2004/03/31 18:12:13  papadopo
 Mike Gertz' refactoring of RedoAlignmentCore

 Revision 6.38  2004/02/06 19:25:16  camacho
 Alejandro Schaffer's corrections to RedoAlignmentCore pointed out by
 Mike Gertz:
 1. Corrected the rule for assigning newSW->isfirstAlignment
 2. Changed upper bound on assignment to forbiddenRanges
 3. Assigned a value to newScore earlier
 4. Eliminated use of skipThis
 5. Restored value of search->gapAlign at the end

 Revision 6.37  2004/01/27 20:31:52  madden
 remove extra setting of kbp_gap

 Revision 6.36  2004/01/06 17:48:44  dondosha
 Do not free Karlin block in RedoAlignmentCore, because its pointer is passed outside

 Revision 6.35  2003/12/01 19:15:27  madden
 Add one byte to filteredMatchingSequence to prevent ABR/ABW

 Revision 6.34  2003/10/22 20:37:19  madden
 Set kbp to rescaled values, use upper-case for SCALING_FACTOR define

 Revision 6.33  2003/10/02 19:59:34  kans
 BlastGetGapAlgnTbck needed FALSE instead of NULL in two parameters - Mac compiler complaint

 Revision 6.32  2003/10/02 19:31:24  madden
 In RedoAlignmentCore, call procedure BlastGetGapAlgnTbck instead of ALIGN
 to redo alignments; this allows the endpoints of the alignment to change.
 Because  BlastGetGapAlgnTbck returns a list of SeqAlign's while ALIGN
 passes back a single-alignment, the post-processing of the redone
 alignments is changed including the addition of the procedure
 concatenateListOfSeqaligns.

 Revision 6.30  2003/05/30 17:25:36  coulouri
 add rcsid

 Revision 6.29  2003/05/13 16:02:53  coulouri
 make ErrPostEx(SEV_FATAL, ...) exit with nonzero status

 Revision 6.28  2002/12/19 14:40:35  kans
 changed C++-style comment to C-style

 Revision 6.27  2002/12/10 22:58:42  bealer
 Keep mappings to sequences from readdb so that "num_ident" code does not
 segfault with multiple databases.

 Revision 6.26  2002/11/06 20:31:14  dondosha
 Added recalculation of the number of identities when computing seqalign from SWResults

 Revision 6.25  2002/10/16 13:33:58  madden
 Corrected initialization of initialUngappedLambda in RedoAlignmentCore

 Revision 6.24  2002/09/03 13:55:14  kans
 changed NULL to 0 for Mac compiler

 Revision 6.23  2002/08/29 15:47:49  camacho
 Changed RedoAlignmentCore to work without readdb facility

 Revision 6.22  2002/08/20 15:43:08  camacho
 Fixed memory leak in getStartFreqRatios

 Revision 6.21  2002/05/23 20:14:21  madden
 Correction for last checkin to cover SmithWaterman type alignment

 Revision 6.20  2001/12/28 18:02:33  dondosha
 Keep score and scoreThisAlign for each local alignment, so as to allow tie-breaking by score

 Revision 6.19  2001/07/26 12:52:25  madden
 Fix memory leaks

 Revision 6.18  2001/07/09 15:12:47  shavirin
 Functions BLbasicSmithWatermanScoreOnly() and BLSmithWatermanFindStart()
 used to calculate Smith-waterman alignments on low level become external.

 Revision 6.17  2001/05/25 19:40:46  vakatov
 Nested comment typo fixed

 Revision 6.16  2001/04/13 20:47:36  madden
 Eliminated use of PRO_K_MULTIPLIER in adjusting E-values Added allocateStartFreqs and freeStartFreqs and getStartFreqRatios to enable the score matrix scaling to work entirely with frequency ratios and round to integers only at the very end of the scaling calculation.

 Revision 6.15  2001/03/20 15:07:34  madden
 Fix from AS for (near) exact matches

 Revision 6.14  2001/01/04 13:48:44  madden
 Correction for 3-parameter gapping

 Revision 6.13  2000/12/05 19:31:33  madden
 Avoid duplicate insertion into heap when ((doThis == FALSE) && (currentState = SWPurging))

 Revision 6.12  2000/10/16 21:08:05  madden
 segResult takes BioseqPtr as argument, produced from readdb_get_bioseq

 Revision 6.11  2000/10/13 19:32:58  madden
 Fix for bug that caused crash

 Revision 6.10  2000/10/10 21:46:03  shavirin
 Added support for BLOSUM50, BLOSUM90, PAM250 with -t T

 Revision 6.9  2000/10/10 19:45:51  shavirin
 Checked for NULL hsp_array in the function RedoAlignmentCore().

 Revision 6.8  2000/08/18 21:28:24  madden
 support for BLOSUM62_20A and BLOSUM62_20B, prevent LambdaRatio from getting above 1.0

 Revision 6.7  2000/08/09 21:10:15  shavirin
 Added paramter discontinuous to the function newConvertSWalignsToSeqAligns()

 Revision 6.6  2000/08/08 21:45:04  shavirin
 Removed initialization of decline_aline parameter to INT2_MAX.

 Revision 6.5  2000/08/03 22:20:10  shavirin
 Added creation of the default posFreqs array if it is empty in
 RedoAlignmentCore(). Fixed some memory leaks.

 Revision 6.4  2000/08/02 18:00:34  shavirin
 Fixed memory leak in RedoAlignmentCore. Fixed a problem specific
 to BLOSUM45 and BLOSUM80 in the procedure scalePosMatrix().

 Revision 6.3  2000/07/26 19:34:13  kans
 removed unix-only headers

 Revision 6.2  2000/07/26 17:06:31  lewisg
 add LIBCALLs

 Revision 6.1  2000/07/25 17:40:05  shavirin
 Initial revision.


******************************************************************************/


#include <ncbi.h>
#include <blast.h>
#include <objseq.h>
#include <objsset.h>
#include <sequtil.h>
#include <seqport.h>
#include <tofasta.h>
#include <blastpri.h>
#include <txalign.h>
#include <simutil.h>
#include <posit.h>
#include <gapxdrop.h>
#include <fcntl.h>
#include <profiles.h>


extern Nlm_FloatHi LIBCALL
impalaKarlinLambdaNR PROTO((BLAST_ScoreFreqPtr sfp,
                            Nlm_FloatHi initialLambda));


/** by what factor might initially reported E-value exceed true Evalue */
#define EVALUE_STRETCH 5
/** number of real aminoacids (i.e.: does not include U, X, B, etc) */
#define PRO_TRUE_ALPHABET_SIZE 20
/** range of scores in a matrix */
#define kScoreMatrixScoreRange 10000

/** For translated subject sequences, the number of amino acids to
    include before and after the existing aligned segment when
    generating a composition-based scoring system. */
#define KAPPA_WINDOW_BORDER 200

/**positions of true characters in protein alphabet*/
static Int4 trueCharPositions[PRO_TRUE_ALPHABET_SIZE] =
  {1,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,22};


/**
 * Create a score set from the data in an HSP.
 *
 * @param hsp               the HSP of interest [in]
 * @param logK              Karlin-Altschul statistical parameter [in]
 * @param lambda            Karlin-Altschul statistical parameter [in]
 * @param scoreDivisor      the value by which reported scores are to be
 *                          divided [in]
 */
static ScorePtr
GetScoreSetFromBlastHsp(
  const BLAST_HSPPtr hsp,
  Nlm_FloatHi lambda,
  Nlm_FloatHi logK,
  Nlm_FloatHi scoreDivisor)
{
  ScorePtr      score_set = NULL;       /* the new score set */
  Int4          score;          /* the score, scaled using scaleDivisor */
  Nlm_FloatHi   bit_score;      /* the integer-valued score, in bits */
  Nlm_FloatHi   evalue;         /* the e-value, with numbers too close to zero
                                   set to zero */

  score = Nlm_Nint(((Nlm_FloatHi) hsp->score) / scoreDivisor);
  if (score > 0) {
    MakeBlastScore(&score_set, "score", 0.0, score);
  }

  evalue = (hsp->evalue >= 1.0e-180) ? hsp->evalue : 0;
  if (hsp->num > 1) {
    MakeBlastScore(&score_set, "sum_e", evalue, 0);
    MakeBlastScore(&score_set, "sum_n", 0.0, hsp->num);
    if( hsp->ordering_method == BLAST_SMALL_GAPS) {
      MakeBlastScore(&score_set, "small_gap", 0.0, 1);
    }
  } else {
    MakeBlastScore(&score_set, "e_value", evalue, 0);
  }

  /* Compute the bit score using the newly computed scaled score. */
  bit_score = (score*lambda*scoreDivisor - logK)/NCBIMATH_LN2;
  if (bit_score >= 0.) {
    MakeBlastScore(&score_set, "bit_score", bit_score, 0);
  }

  if (hsp->ordering_method > 3) {
    /* In new tblastn this means splice junction was found */
    MakeBlastScore(&score_set, "splice_junction", 0.0, 1);
  }

  if (hsp->num_ident > 0) {
    MakeBlastScore(&score_set, "num_ident", 0.0, hsp->num_ident);
  }

  return score_set;
}


/**
 * Converts a collection of alignments represented by a BLAST_HitList
 * into a collection of instances of SeqAlign. Returns the new
 * collection.
 *
 * @param hitlist           the hitlist to convert to SeqAligns [in]
 * @param subject_id        the identifier of the subject sequence [in]
 * @param query_id          the identifier of the query sequence [in]
 * @param lambda            Karlin-Altschul statistical parameter [in]
 * @param logK              Karlin-Altschul statistical parameter [in]
 * @param scoreDivisor      value by which reported scores are to be
 *                          divided [in]
 * @param bestEvalue)       the best (smallest) e-value of all the alignments
 *                          in the hitlist [out]
 */
static SeqAlignPtr
SeqAlignsFromHitlist(
  BLAST_HitListPtr hitlist,
  SeqIdPtr subject_id,
  SeqIdPtr query_id,
  Nlm_FloatHi lambda,
  Nlm_FloatHi logK,
  Nlm_FloatHi scoreDivisor,
  Nlm_FloatHi * bestEvalue)
{
  SeqAlignPtr aligns = NULL;  /* list of SeqAligns to be returned */
  Int4        hsp_index;

  *bestEvalue = INT4_MAX;
  for( hsp_index = hitlist->hspcnt - 1; hsp_index >= 0; hsp_index-- ) {
    /* iterate in reverse order over all HSPs in the hitlist */
    BLAST_HSPPtr hsp;           /* HSP for this iteration */
    SeqAlignPtr seqAlign;       /* the new SeqAlign */

    hsp      = hitlist->hsp_array[hsp_index];
    if( hsp->evalue < *bestEvalue ) *bestEvalue = hsp->evalue;

    seqAlign = GapXEditBlockToSeqAlign(hsp->gap_info, subject_id, query_id);
    seqAlign->score = GetScoreSetFromBlastHsp(hsp, lambda, logK, scoreDivisor);

    seqAlign->next  = aligns;
    aligns          = seqAlign;
  }
  return aligns;
}


/**
 * Remove from a hitlist all HSPs that are completely contained in an
 * HSP that occurs earlier in the list and that:
 * - is on the same strand; and
 * - has equal or greater score.  T
 * The hitlist should be sorted by some measure of significance before
 * this routine is called.
 * @param hsp_array         array to be reaped
 * @param hspcnt            length of hsp_array
 */
static void
HitlistReapContained(
  BLAST_HSPPtr * hsp_array,
  Int4 * hspcnt)
{
  Int4 iread;       /* iteration index used to read the hitlist */
  Int4 iwrite;      /* iteration index used to write to the hitlist */
  Int4 old_hspcnt;  /* number of HSPs in the hitlist on entry */

  old_hspcnt = *hspcnt;

   for( iread = 1; iread < *hspcnt; iread++ ) {
     /* for all HSPs in the hitlist */
     Int4         ireadBack;    /* iterator over indices less than iread */
     BLAST_HSPPtr hsp1;         /* an HSP that is a candidate for deletion */

     hsp1 = hsp_array[iread];
     for( ireadBack = 0; ireadBack < iread && hsp1 != NULL; ireadBack++ ) {
       /* for all HSPs before hsp1 in the hitlist and while hsp1 has not
          been deleted */
       BLAST_HSPPtr hsp2;       /* an HSP that occurs earlier in hsp_array
                                 * than hsp1 */
       hsp2 = hsp_array[ireadBack];

       if( hsp2 == NULL ) {  /* hsp2 was deleted in a prior iteration. */
         continue;
       }
       if(SIGN(hsp2->query.frame)   == SIGN(hsp1->query.frame) &&
          SIGN(hsp2->subject.frame) == SIGN(hsp1->subject.frame)) {
         /* hsp1 and hsp2 are in the same query/subject frame. */
         if(CONTAINED_IN_HSP
            (hsp2->query.offset,   hsp2->query.end,   hsp1->query.offset,
             hsp2->subject.offset, hsp2->subject.end,
             hsp1->subject.offset) &&
            CONTAINED_IN_HSP
            (hsp2->query.offset,   hsp2->query.end,   hsp1->query.end,
             hsp2->subject.offset, hsp2->subject.end,
             hsp1->subject.end)    &&
            hsp1->score <= hsp2->score) {
           hsp1 = hsp_array[iread] = BLAST_HSPFree(hsp_array[iread]);
         }
       } /* end if hsp1 and hsp2 are in the same query/subject frame */
     } /* end for all HSPs before hsp1 in the hitlist */
   } /* end for all HSPs in the hitlist */

   /* Condense the hsp_array, removing any NULL items. */
   iwrite = 0;
   for( iread = 0; iread < *hspcnt; iread++ ) {
     if( hsp_array[iread] != NULL ) {
       hsp_array[iwrite++] = hsp_array[iread];
     }
   }
   *hspcnt = iwrite;
   /* Fill the remaining memory in hsp_array with NULL pointers. */
   for( ; iwrite < old_hspcnt; iwrite++ ) {
     hsp_array[iwrite] = NULL;
   }
}


/**
 * An object of type Kappa_DistinctAlignment represents a distinct
 * alignment of the query sequence to the current subject sequence.
 * These objects are typically part of a singly linked list of
 * distinct alignments, stored in the reverse of the order in which
 * they were computed.
 */
typedef struct Kappa_DistinctAlignment {
  Int4 score;            /**< the score of this alignment */
  Int4 queryStart;       /**< the start of the alignment in the query */
  Int4 queryEnd;         /**< one past the end of the alignment in the query */
  Int4 matchStart;       /**< the start of the alignment in the subject */
  Int4 matchEnd;         /**< one past the end of the alignment in the
                              subject */
  Int4 frame;            /**< the subject frame */
  GapXEditBlockPtr editBlock;   /**< the alignment info for a gapped
                                     alignment */
  struct Kappa_DistinctAlignment * next;  /**< the next alignment in the
                                               list */
} Kappa_DistinctAlignment;


/**
 * Recursively free all alignments in the singly linked list whose
 * head is *palign. Set *palign to NULL.
 *
 * @param palign            pointer to the head of a singly linked list
 *                          of alignments.
 */
static void
Kappa_DistinctAlignmentsFree(Kappa_DistinctAlignment ** palign)
{
  Kappa_DistinctAlignment * align;      /* represents the current
                                           alignment in loops */
  align = *palign;  *palign = NULL;
  while(align != NULL) {
    /* Save the value of align->next, because align is to be deleted. */
    Kappa_DistinctAlignment * align_next = align->next;
    align_next = align->next;

    if(align->editBlock) {
      GapXEditBlockDelete(align->editBlock);
    }
    MemFree(align);

    align = align_next;
  }
}


/**
 * Converts a list of objects of type Kappa_DistinctAlignment to an
 * new object of type BLAST_HitList and returns the result. Conversion
 * in this direction is lossless.  The list passed to this routine is
 * freed to ensure that there is no aliasing of fields between the
 * list of Kappa_DistinctAlignments and the new hitlist.
 *
 * @param search            general search information
 * @param alignments        a list of distinct alignments
 */
static BLAST_HitListPtr
HitlistFromDistinctAlignments(
  BlastSearchBlkPtr search,
  Kappa_DistinctAlignment ** alignments)
{
  /* The context of the query is always zero in current
   * implementations of RedoAlignmentCore. */
  const Int4 context = 0;

  BLAST_HitListPtr hitlist;             /* the new hitlist */
  Kappa_DistinctAlignment * align;      /* represents the current
                                         * alignment in loops */
  if(search->current_hitlist != NULL) {
    search->current_hitlist->hspcnt_max = search->current_hitlist->hspcnt;
    BlastHitListPurge(search->current_hitlist);
  } else {
    search->current_hitlist =  BlastHitListNew(search);
  }
  hitlist = search->current_hitlist;
  hitlist->do_not_reallocate = FALSE;

  for(align = *alignments; align != NULL; align = align->next) {
    BLAST_HSPPtr hsp;           /* the new HSP for this alignment */
    /* queryExtent and matchExtent represent the extent of the
       alignment in the query and subject sequences respectively. */
    Int4 queryExtent = align->queryEnd - align->queryStart;
    Int4 matchExtent = align->matchEnd - align->matchStart;

    BlastSaveCurrentHsp(search, align->score, align->queryStart,
                        align->matchStart, matchExtent, context);

    hsp = hitlist->hsp_array[hitlist->hspcnt - 1];

    hsp->query.length   = queryExtent;
    hsp->query.end      = align->queryEnd;

    hsp->subject.length = matchExtent;
    hsp->subject.end    = align->matchEnd;

    hsp->subject.frame  = align->frame;
    hsp->evalue         = 0.0;  /* E-values are computed after the full
                                   hitlist has been created. */
    hsp->gap_info       = align->editBlock;

    /* Break the aliasing between align->editBlock and hsp->gap_info. */
    align->editBlock = NULL;
  }

  Kappa_DistinctAlignmentsFree(alignments);

  return hitlist;
}


/**
 * Given a list of alignments and a new alignment, create a new list
 * of alignments that conditionally includes the new alignment.
 *
 * If there is an equal or higher-scoring alignment in the preexisting
 * list of alignments that shares an endpoint with the new alignment,
 * then preexisting list is returned.  Otherwise, a new list is
 * returned with the new alignment as its head and the elements of
 * preexisting list that do not share an endpoint with the new
 * alignment as its tail. The order of elements is preserved.
 *
 * Typically, a list of alignments is built one alignment at a time
 * through a call to withDistinctEnds. All alignments in the resulting
 * list have distinct endpoints.  Which items are retained in the list
 * depends on the order in which they were added.
 *
 * Note that an endpoint is a triple, specifying a frame, a location
 * in the query and a location in the subject.  In other words,
 * alignments that are not in the same frame never share endpoints.
 *
 * @param p_newAlign        on input the alignment that may be added to
 *                          the list; on output NULL
 * @param p_oldAlignment    on input the existing list of alignments;
 *                          on output the new list
 */
static void
withDistinctEnds(
  Kappa_DistinctAlignment **p_newAlign,
  Kappa_DistinctAlignment **p_oldAlignments)
{
  /* Deference the input parameters. */
  Kappa_DistinctAlignment * newAlign      = *p_newAlign;
  Kappa_DistinctAlignment * oldAlignments = *p_oldAlignments;
  Kappa_DistinctAlignment * align;      /* represents the current
                                          alignment in loops */
  Boolean include_new_align;            /* true if the new alignment
                                           may be added to the list */
  *p_newAlign        = NULL;
  include_new_align  = 1;

  for(align = oldAlignments; align != NULL; align = align->next) {
    if(align->frame == newAlign->frame &&
       (   (   align->queryStart == newAlign->queryStart
            && align->matchStart == newAlign->matchStart)
        || (   align->queryEnd   == newAlign->queryEnd
            && align->matchEnd   == newAlign->matchEnd))) {
      /* At least one of the endpoints of newAlign matches an endpoint
         of align. */
      if( newAlign->score <= align->score ) {
        /* newAlign cannot be added to the list. */
        include_new_align = 0;
        break;
      }
    }
  }

  if(include_new_align) {
    Kappa_DistinctAlignment **tail;     /* tail of the list being created */

    tail  = &newAlign->next;
    align = oldAlignments;
    while(align != NULL) {
      /* Save align->next because align may be deleted. */
      Kappa_DistinctAlignment * align_next = align->next;
      align->next = NULL;
      if(align->frame == newAlign->frame &&
         (   (   align->queryStart == newAlign->queryStart
              && align->matchStart == newAlign->matchStart)
          || (   align->queryEnd   == newAlign->queryEnd
              && align->matchEnd   == newAlign->matchEnd))) {
        /* The alignment shares an end with newAlign; */
        /* delete the alignment. */
        Kappa_DistinctAlignmentsFree(&align);
      } else { /* The alignment does not share an end with newAlign; */
        /* add it to the output list. */
        *tail =  align;
        tail  = &align->next;
      }
      align = align_next;
    } /* end while align != NULL */
    *p_oldAlignments = newAlign;
  } else { /* do not include_new_align */
    Kappa_DistinctAlignmentsFree(&newAlign);
  } /* end else do not include newAlign */
}


/**
 * The number of bits by which the score of a previously computed
 * alignment must exceed the score of the HSP under consideration for
 * a containment relationship to be reported by the isAlreadyContained
 * routine. */
#define KAPPA_BIT_TOL 2


/**
 * Return true if the HSP is already contained in a
 * previously-computed alignment of sufficiently high score.
 *
 * @param hsp                 HSP to be tested
 * @param alignments          list of alignments
 * @param lambda              Karlin-Altschul statistical parameter
 * @param localScalingFactor  factor by which scores were scaled to
 *                            obtain higher precision
 */

static Boolean
isAlreadyContained(
  BLASTResultHspPtr hsp,
  Kappa_DistinctAlignment * alignments,
  Nlm_FloatHi lambda,
  Nlm_FloatHi localScalingFactor)
{
  Kappa_DistinctAlignment * align;     /* represents the current alignment
                                          in the main loop */

  /* Endpoints of the HSP, using the convention the right endpoint is
   * one past the end of the HSP. */
  Int4 hsp_query_offset    = hsp->query_offset;
  Int4 hsp_query_end       = hsp_query_offset + hsp->query_length;
  Int4 hsp_subject_offset  = hsp->subject_offset;
  Int4 hsp_subject_end     = hsp_subject_offset + hsp->subject_length;

  Nlm_FloatHi scoreTol;   /* the amount by which the score of the current
                             alignment must exceed the score of the HSP for a
                             containment relationship to be reported. */
  scoreTol = KAPPA_BIT_TOL * NCBIMATH_LN2/lambda;

  for( align = alignments; align != NULL; align = align->next ) {
    /* for all elements of alignments */
    if(SIGN(hsp->query_frame)   == SIGN(align->frame)) {
      /* hsp1 and hsp2 are in the same query/subject frame */
      if(CONTAINED_IN_HSP
         (align->queryStart, align->queryEnd, hsp_query_offset,
          align->matchStart, align->matchEnd, hsp_subject_offset) &&
         CONTAINED_IN_HSP
         (align->queryStart, align->queryEnd, hsp_query_end ,
          align->matchStart, align->matchEnd, hsp_subject_end) &&
                 hsp->score * localScalingFactor + scoreTol <= align->score) {
        return 1;
      }
    } /* hsp1 and hsp2 are in the same query/subject frame */
  } /* end for all items in alignments */

  return 0;
}


/**
 * The struct SWheapRecord data type is used below to define the
 * internal structure of a SWheap (see below).  A SWheapRecord
 * represents all alignments of a query sequence to a particular
 * matching sequence.
 */
typedef struct SWheapRecord {
  Nlm_FloatHi   bestEvalue;     /**< best (smallest) evalue of all alignments
                                     in the record */
  Int4          subject_index;  /**< index of the subject sequence in
                                     the database */
  SeqAlignPtr   theseAlignments;  /**< a list of alignments */
} SWheapRecord;


/** Compare two records in the heap.  */
static Boolean
SWheapRecordCompare(SWheapRecord * place1,
                    SWheapRecord * place2)
{
  return ((place1->bestEvalue    >  place2->bestEvalue) ||
          (place1->bestEvalue    == place2->bestEvalue &&
           place1->subject_index >  place2->subject_index));
}


/** swap two records in the heap*/
static void
SWheapRecordSwap(SWheapRecord * heapArray,
                 Int4 i,
                 Int4 j)
{
  /* bestEvalue, theseAlignments and subject_index are temporary
   * variables used to perform the swap. */
  Nlm_FloatHi bestEvalue       = heapArray[i].bestEvalue;
  SeqAlignPtr theseAlignments  = heapArray[i].theseAlignments;
  Int4        subject_index    = heapArray[i].subject_index;

  heapArray[i].bestEvalue      = heapArray[j].bestEvalue;
  heapArray[i].theseAlignments = heapArray[j].theseAlignments;
  heapArray[i].subject_index   = heapArray[j].subject_index;

  heapArray[j].bestEvalue      = bestEvalue;
  heapArray[j].theseAlignments = theseAlignments;
  heapArray[j].subject_index   = subject_index;
}


/**
 * Verifies that the array heapArray[i] .. heapArray[n] is ordered so
 * as to be a valid heap.  This routine checks every element in the array,
 * an so is very time consuming.  It is for debugging purposes only.
 */
static Boolean
SWheapIsValid(SWheapRecord * heapArray,
              Int4 i,
              Int4 n)
{
  /* indices of nodes to the left and right of node i */
  Int4 left = 2 * i, right = 2 * i + 1;        

  if(right <= n) {
    return !SWheapRecordCompare(&(heapArray[right]), &(heapArray[i])) &&
      SWheapIsValid(heapArray, right, n);
  }
  if(left <= n) {
    return !SWheapRecordCompare(&(heapArray[left]), &(heapArray[i])) &&
      SWheapIsValid(heapArray, left, n);
  }
  return TRUE;
}

/** convenience debugging macro for this module */
#ifdef KAPPA_INTENSE_DEBUG
#define KAPPA_ASSERT(expr) ((expr) ? 0 : \
(fprintf( stderr, "KAPPA_ASSERT failed line %d: %s", __LINE__, #expr ), \
exit(1)))
#else
#define KAPPA_ASSERT(expr) (void)(0)
#endif


/** On entry, all but the first element of the array heapArray[i]
 * .. heapArray[n] are in valid heap order.  This routine rearranges
 * the elements so that on exit they all are in heap order.
 * @param heapArray holds the heap [in][out]
 * @param i element of heapArray that may be out of order [in]
 * @param n size of heapArray [in]
 */
static void
SWheapifyDown(SWheapRecord * heapArray,
              Int4 i,
              Int4 n)
{
  Boolean moreswap = TRUE;      /* is more swapping needed */
  Int4 left, right, largest;    /* placeholders for indices in swapping */
  do {
    left  = 2 * i;
    right = 2 * i + 1;
    if((left <= n) &&
       (SWheapRecordCompare(&(heapArray[left]), &(heapArray[i]))))
      largest = left;
    else
      largest = i;
    if((right <= n) &&
       (SWheapRecordCompare(&(heapArray[right]), &(heapArray[largest]))))
      largest  = right;
    if(largest != i) {
      SWheapRecordSwap(heapArray, i, largest);
      /* push largest up the heap */
      i       = largest;       /* check next level down */
    } else
      moreswap = FALSE;
  } while(moreswap);            /* function builds the heap */
  KAPPA_ASSERT(SWheapIsValid(heapArray, i, n));
}


/** On entry, all but the last element of the array heapArray[0]
 * .. heapArray[i] are in valid heap order.  This routine rearranges
 * the elements so that on exit they all are in heap order.
 *
 * @param heapArray      holds the heap [in][out]
 * @param i              element in heap array that may be out of order [in]
 * @param n              size of heapArray
 */
static void
SWheapifyUp(SWheapRecord * heapArray,
            Int4 i,
            Int4 n)
{
  Int4 parent = i / 2;          /* index to the node that is the
                                   parent of node i */
  while(parent >= 1 &&
        SWheapRecordCompare(&(heapArray[i]), &(heapArray[parent]))){
    SWheapRecordSwap(heapArray, i, parent);

    i       = parent;
    parent /= 2;
  }
  KAPPA_ASSERT(SWheapIsValid(heapArray, 1, n));
}

/**
 * A SWheap represents a collection of alignments between one query
 * sequence and several matching subject sequences.  
 *
 * Each matching sequence is allocated one record in a SWheap.  The
 * eValue of a query-subject pair is the best (smallest positive)
 * evalue of all alignments between the two sequences.
 * 
 * A match will be inserted in the the SWheap if:
 * - there are fewer that SWheap::heapThreshold elements in the SWheap;
 * - the eValue of the match is <= SWheap::ecutoff; or
 * - the eValue of the match is less than the largest (worst) eValue
 *   already in the SWheap.
 *
 * If there are >= SWheap::heapThreshold matches already in the SWheap
 * when a new match is to be inserted, then the match with the largest
 * (worst) eValue is removed, unless the largest eValue <=
 * SWheap::ecutoff.  Matches with eValue <= SWheap::ecutoff are never
 * removed by the insertion routine.  As a consequence, the SWheap can
 * hold an arbitrarily large number of matches, although it is
 * atypical for the number of matches to be greater than
 * SWheap::heapThreshold.
 *
 * Once all matches have been collected, the SWheapToFlatList routine
 * may be invoked to return a list of all alignments. (see below).
 *
 * While the number of elements in a heap < SWheap::heapThreshold, the
 * SWheap is implemented as an unordered array, rather than a
 * heap-ordered array.  The SWheap is converted to a heap-ordered
 * array as soon as it becomes necessary to order the matches by
 * evalue.  The routines that operate on a SWheap should behave
 * properly whichever state the SWheap is in.
 */
typedef struct SWheap {
  Int4 n;                       /**< The current number of elements */
  Int4 capacity;                /**< The maximum number of elements
                                     that may be inserted before the
                                     SWheap must be resized */
  Int4 heapThreshold;           /**< see above */
  Nlm_FloatHi ecutoff;          /**< matches with evalue below ecutoff may
                                     always be inserted in the SWheap */
  Nlm_FloatHi worstEvalue;      /**< the worst (biggest) evalue currently in
                                     the heap */

  SWheapRecord *array;          /**< the SWheapRecord array if the SWheap is
                                     being represented as an unordered array */
  SWheapRecord *heapArray;      /**< the SWheapRecord array if the SWheap is
                                     being represented as an heap-ordered
                                     array. At least one of (array, heapArray)
                                     is NULL */

} SWheap;


/** Convert a SWheap from a representation as an unordered array to
 *  a representation as a heap-ordered array.
 *
 *  @param self         the SWheap to convert
 */
static void
ConvertToHeap(SWheap * self)
{
  if(NULL != self->array) {
    Int4 i;                     /* heap node index */
    Int4 n;                     /* number of elements in the heap */
    /* We aren't already a heap */
    self->heapArray = self->array;
    self->array     = NULL;

    n = self->n;
    for(i = n / 2; i >= 1; --i) {
      SWheapifyDown(self->heapArray, i, n);
    }
  }
  KAPPA_ASSERT(SWheapIsValid(self->heapArray, 1, self->n));
}

/** When the heap is about to exceed its capacity, it will be grown by
 * the minimum of a multiplicative factor of SWHEAP_RESIZE_FACTOR
 * and an additive factor of SWHEAP_MIN_RESIZE. The heap never
 * decreases in size */
#define SWHEAP_RESIZE_FACTOR 1.5
/** @sa SWHEAP_RESIZE_FACTOR */
#define SWHEAP_MIN_RESIZE 100

/** Return true if self would insert a match that had the given eValue
 *  @param self         a SWheap
 *  @param eValue       the evalue to be tested.
 */
static Boolean
SWheapWouldInsert(SWheap * self,
                  Nlm_FloatHi eValue)
{
  return self->n < self->heapThreshold ||
    eValue <= self->ecutoff ||
    eValue < self->worstEvalue;
}


/**
 * Try to insert matchRecord into the SWheap. The list of SeqAligns
 * passed to this routine is used directly, i.e. the list is not copied,
 * but is rather stored in the SWheap or deleted.
 *
 * @param self              the heap
 * @param alignments        a list of alignments
 * @param eValue            the best evalue among the alignments
 * @param subject_index     the index of the subject sequence in the database
 */
static void
SWheapInsert(
  SWheap * self,
  SeqAlignPtr alignments,
  Nlm_FloatHi eValue,
  Int4 subject_index)
{
  if(self->array && self->n >= self->heapThreshold) {
    ConvertToHeap(self);
  }
  if(self->array != NULL) {
    /* "self" is currently a list. Add the new alignments to the end */
    SWheapRecord *heapRecord;   /* destination for the new alignments */
    heapRecord                  = &self->array[++self->n];
    heapRecord->bestEvalue      = eValue;
    heapRecord->theseAlignments = alignments;
    heapRecord->subject_index   = subject_index;
    if( self->worstEvalue < eValue ) {
      self->worstEvalue = eValue;
    }
  } else {                      /* "self" is currently a heap */
    if(self->n < self->heapThreshold ||
       (eValue <= self->ecutoff &&
        self->worstEvalue <= self->ecutoff)) {
      SWheapRecord *heapRecord; /* Destination for the new alignments */
      /* The new alignments must be inserted into the heap, and all old
       * alignments retained */
      if(self->n >= self->capacity) {
        /* The heap must be resized */
        Int4 newCapacity;       /* capacity the heap will have after
                                 * it is resized */
        newCapacity      = MAX(SWHEAP_MIN_RESIZE + self->capacity,
                               (Int4) (SWHEAP_RESIZE_FACTOR * self->capacity));
        self->heapArray  = (SWheapRecord *)
          MemMore(self->heapArray, (newCapacity + 1) * sizeof(SWheapRecord));
        self->capacity   = newCapacity;
      }
      /* end if the heap must be resized */
      heapRecord    = &self->heapArray[++self->n];
      heapRecord->bestEvalue      = eValue;
      heapRecord->theseAlignments = alignments;
      heapRecord->subject_index   = subject_index;

      SWheapifyUp(self->heapArray, self->n, self->n);
    } else {
      /* Some set of alignments must be discarded; discardedAlignments
       * will hold a pointer to these alignments. */
      SeqAlignPtr discardedAlignments = NULL;

      if(eValue >= self->worstEvalue) {
        /* The new alignments must be discarded. */
        discardedAlignments = alignments;
      } else {
        /* The largest element in the heap must be discarded. */
        SWheapRecord *heapRecord;     /* destination for the new alignments */
        discardedAlignments         = self->heapArray[1].theseAlignments;

        heapRecord                  = &self->heapArray[1];
        heapRecord->bestEvalue      = eValue;
        heapRecord->theseAlignments = alignments;
        heapRecord->subject_index   = subject_index;

        SWheapifyDown(self->heapArray, 1, self->n);
      }
      /* end else the largest element in the heap must be discarded */
      while(discardedAlignments != NULL) {
        /* There are discarded alignments that have not been freed. */
        SeqAlignPtr thisAlignment;     /* the head of the list of
                                        * discarded alignments */
        thisAlignment        = discardedAlignments;
        discardedAlignments  = thisAlignment->next;

        SeqAlignFree(thisAlignment);
      }
      /* end while there are discarded alignments that have not been freed */
    }
    /* end else some set of alignments must be discarded */

    self->worstEvalue = self->heapArray[1].bestEvalue;
    KAPPA_ASSERT(SWheapIsValid(self->heapArray, 1, self->n));
  }
  /* end else "self" is currently a heap. */
}


/**
 * Return true if only matches with evalue <= self->ecutoff may be
 * inserted.
 *
 * @param self          a SWheap
 */
static Boolean
SWheapWillAcceptOnlyBelowCutoff(SWheap * self)
{
  return self->n >= self->heapThreshold && self->worstEvalue <= self->ecutoff;
}


/** Initialize a new SWheap; parameters to this function correspond
 * directly to fields in the SWheap */
static void
SWheapInitialize(SWheap * self,
                 Int4 capacity,
                 Int4 heapThreshold,
                 Nlm_FloatHi ecutoff)
{
  self->n             = 0;
  self->heapThreshold = heapThreshold;
  self->ecutoff       = ecutoff;
  self->heapArray     = NULL;
  self->capacity      = 0;
  self->worstEvalue   = 0;
  /* Begin life as a list */
  self->array =
    (SWheapRecord *) MemNew((capacity + 1) * sizeof(SWheapRecord));
  self->capacity      = capacity;
}


/**
 * Release the storage associated with the fields of a SWheap. Don't
 * delete the SWheap structure itself.
 *
 * @param self          SWheap whose storage will be released
 */
static void
SWheapRelease(SWheap * self)
{
  if(self->heapArray) free(self->heapArray);
  if(self->array) free(self->array);

  self->n = self->capacity = self->heapThreshold = 0;
  self->heapArray = NULL;
}


/**
 * Remove and return the element in the SWheap with largest (worst) evalue
 *
 * @param self           a SWheap
 */
static SeqAlignPtr
SWheapPop(SWheap * self)
{
  SeqAlignPtr results = NULL;   /* the list of SeqAligns to be returned */

  ConvertToHeap(self);
  if(self->n > 0) { /* The heap is not empty */
    SWheapRecord *first, *last; /* The first and last elements of the
                                 * array that represents the heap. */
    first = &self->heapArray[1];
    last  = &self->heapArray[self->n];

    results = first->theseAlignments;

    first->theseAlignments = last->theseAlignments;
    first->bestEvalue      = last->bestEvalue;
    first->subject_index   = last->subject_index;

    SWheapifyDown(self->heapArray, 1, --self->n);
  }

  KAPPA_ASSERT(SWheapIsValid(self->heapArray, 1, self->n));

  return results;
}


/**
 * Convert a SWheap to a flat list of SeqAligns. Note that there may
 * be more than one alignment per element in the heap.  The new list
 * preserves the order of the SeqAligns associated with each
 * HeapRecord.
 *
 * @param self           a SWheap
 */
static SeqAlignPtr
SWheapToFlatList(SWheap * self)
{
  SeqAlignPtr list = NULL;
  SeqAlignPtr result;

  while(NULL != (result = SWheapPop(self))) {
    SeqAlignPtr oldList = list;
    list = result;
    while( NULL != result->next ) {
      result = result->next;
    }
    result->next = oldList;
  }

  return list;
}


/**
 * computes Smith-Waterman local alignment score and returns the
 * evalue
 *
 * @param matchSeq          is a database sequence matched by this query [in]
 * @param matchSeqLength    is the length of matchSeq in amino acids [in]
 * @param query             is the input query sequence [in]
 * @param queryLength       is the length of query [in]
 * @param matrix            is the position-specific matrix associated with
 *                          query [in]
 * @param gapOpen           is the cost of opening a gap [in]
 * @param gapExtend         is the cost of extending an existing gap by 1
 *                          position [in]
 * @param matchSeqEnd       returns the final position in the matchSeq of an
 *                          optimal local alignment [in]
 * @param queryEnd          returns the final position in query of an optimal
 *                          local alignment [in]. matchSeqEnd and queryEnd can 
 *                          be used to run the local alignment
 *                          in reverse to find optimal starting positions [in]
 * @param score             is used to pass back the optimal score [in]
 * @param kbp               holds the Karlin-Altschul parameters [in]
 * @param effSearchSpace    effective search space [in]
 * @param positionSpecific  determines whether matrix is position
 *                          specific or not [in]
 * @return                  the expect value of the alignment
 */
Nlm_FloatHi
BLbasicSmithWatermanScoreOnly(Uint1 * matchSeq,
   Int4 matchSeqLength, Uint1 *query, Int4 queryLength,
   BLAST_Score **matrix,
   Int4 gapOpen, Int4 gapExtend,  Int4 *matchSeqEnd, Int4 *queryEnd,
   Int4 *score,
   BLAST_KarlinBlkPtr kbp, Nlm_FloatHi effSearchSpace, Boolean positionSpecific)
{

   Int4 bestScore; /*best score seen so far*/
   Int4 newScore;  /* score of next entry*/
   Int4 bestMatchSeqPos, bestQueryPos; /*position ending best score in
                           matchSeq and query sequences*/
   SWpairs *scoreVector; /*keeps one row of the Smith-Waterman matrix
                           overwrite old row with new row*/
   BLAST_Score *matrixRow; /*one row of score matrix*/
   Int4 newGapCost; /*cost to have a gap of one character*/
   Int4 prevScoreNoGapMatchSeq; /*score one row and column up
                               with no gaps*/
   Int4 prevScoreGapMatchSeq;   /*score if a gap already started in matchSeq*/
   Int4 continueGapScore; /*score for continuing a gap in matchSeq*/
   Int4 matchSeqPos, queryPos; /*positions in matchSeq and query*/
   Nlm_FloatHi returnEvalue; /*e-value to return*/


   scoreVector = (SWpairs *) MemNew(matchSeqLength * sizeof(SWpairs));
   bestMatchSeqPos = 0;
   bestQueryPos = 0;
   bestScore = 0;
   newGapCost = gapOpen + gapExtend;
   for (matchSeqPos = 0; matchSeqPos < matchSeqLength; matchSeqPos++) {
     scoreVector[matchSeqPos].noGap = 0;
     scoreVector[matchSeqPos].gapExists = -(gapOpen);
   }
   for(queryPos = 0; queryPos < queryLength; queryPos++) {  
     if (positionSpecific)
       matrixRow = matrix[queryPos];
     else
       matrixRow = matrix[query[queryPos]];
     newScore = 0;
     prevScoreNoGapMatchSeq = 0;
     prevScoreGapMatchSeq = -(gapOpen);
     for(matchSeqPos = 0; matchSeqPos < matchSeqLength; matchSeqPos++) {
       /*testing scores with a gap in matchSeq, either starting a new
         gap or extending an existing gap*/
       if ((newScore = newScore - newGapCost) > 
	   (prevScoreGapMatchSeq = prevScoreGapMatchSeq - gapExtend))
         prevScoreGapMatchSeq = newScore;
       /*testing scores with a gap in query, either starting a new
         gap or extending an existing gap*/
       if ((newScore = scoreVector[matchSeqPos].noGap - newGapCost) >
           (continueGapScore = scoreVector[matchSeqPos].gapExists - gapExtend))
         continueGapScore = newScore;
       /*compute new score extending one position in matchSeq and query*/
       newScore = prevScoreNoGapMatchSeq + matrixRow[matchSeq[matchSeqPos]];
       if (newScore < 0)
       newScore = 0; /*Smith-Waterman locality condition*/
       /*test two alternatives*/
       if (newScore < prevScoreGapMatchSeq)
         newScore = prevScoreGapMatchSeq;
       if (newScore < continueGapScore)
         newScore = continueGapScore;
       prevScoreNoGapMatchSeq = scoreVector[matchSeqPos].noGap; 
       scoreVector[matchSeqPos].noGap = newScore;
       scoreVector[matchSeqPos].gapExists = continueGapScore;
       if (newScore > bestScore) {
         bestScore = newScore;
         bestQueryPos = queryPos;
         bestMatchSeqPos = matchSeqPos;
       }
     }
   }
   MemFree(scoreVector);
   if (bestScore < 0)
     bestScore = 0;
   *matchSeqEnd = bestMatchSeqPos;
   *queryEnd = bestQueryPos;
   *score = bestScore;
   returnEvalue = BlastKarlinStoE_simple(bestScore,kbp, effSearchSpace);
   return(returnEvalue);
}


/**
 * computes where optimal Smith-Waterman local alignment starts given
 * the ending positions and score matchSeqEnd and queryEnd can be used
 * to run the local alignment in reverse to find optimal starting
 * positions these are passed back in matchSeqStart and queryStart the
 * optimal score is passed in to check when it has been reached going
 * backwards the score is also returned
 * @param matchSeq          is a database sequence matched by this query [in]
 * @param matchSeqLength    is the length of matchSeq in amino acids [in]
 * @param query             is the input query sequence  [in]
 * @param matrix            is the position-specific matrix associated with
 *                          query or the standard matrix [in]
 * @param gapOpen           is the cost of opening a gap [in]
 * @param gapExtend         is the cost of extending an existing gap by 1
 *                          position [in]
 * @param matchSeqEnd       is the final position in the matchSeq of an optimal
 *                          local alignment [in]
 * @param queryEnd          is the final position in query of an optimal
 *                          local alignment [in]
 * @param score optimal     score to be obtained [in]
 * @param matchSeqStart     starting point of optimal alignment [out]
 * @param queryStart        starting point of optimal alignment [out]
 * @param positionSpecific  determines whether matrix is position specific
 *                          or not
 */
Int4
BLSmithWatermanFindStart(Uint1 * matchSeq,
   Int4 matchSeqLength, Uint1 *query, Int4 queryLength, BLAST_Score **matrix,
   Int4 gapOpen, Int4 gapExtend,  Int4 matchSeqEnd, Int4 queryEnd, Int4 score,
   Int4 *matchSeqStart, Int4 *queryStart, Boolean positionSpecific)
{

   Int4 bestScore; /*best score seen so far*/
   Int4 newScore;  /* score of next entry*/
   Int4 bestMatchSeqPos, bestQueryPos; /*position starting best score in
                           matchSeq and database sequences*/
   SWpairs *scoreVector; /*keeps one row of the Smith-Waterman matrix
                           overwrite old row with new row*/
   BLAST_Score *matrixRow; /*one row of score matrix*/
   Int4 newGapCost; /*cost to have a gap of one character*/
   Int4 prevScoreNoGapMatchSeq; /*score one row and column up
                               with no gaps*/
   Int4 prevScoreGapMatchSeq;   /*score if a gap already started in matchSeq*/
   Int4 continueGapScore; /*score for continuing a gap in query*/
   Int4 matchSeqPos, queryPos; /*positions in matchSeq and query*/

   scoreVector = (SWpairs *) MemNew(matchSeqLength * sizeof(SWpairs));
   bestMatchSeqPos = 0;
   bestQueryPos = 0;
   bestScore = 0;
   newGapCost = gapOpen + gapExtend;
   for (matchSeqPos = 0; matchSeqPos < matchSeqLength; matchSeqPos++) {
     scoreVector[matchSeqPos].noGap = 0;
     scoreVector[matchSeqPos].gapExists = -(gapOpen);
   }
   for(queryPos = queryEnd; queryPos >= 0; queryPos--) {  
     if (positionSpecific)
       matrixRow = matrix[queryPos];
     else
       matrixRow = matrix[query[queryPos]];
     newScore = 0;
     prevScoreNoGapMatchSeq = 0;
     prevScoreGapMatchSeq = -(gapOpen);
     for(matchSeqPos = matchSeqEnd; matchSeqPos >= 0; matchSeqPos--) {
       /*testing scores with a gap in matchSeq, either starting a new
         gap or extending an existing gap*/
       if ((newScore = newScore - newGapCost) > 
	   (prevScoreGapMatchSeq = prevScoreGapMatchSeq - gapExtend))
         prevScoreGapMatchSeq = newScore;
       /*testing scores with a gap in query, either starting a new
         gap or extending an existing gap*/
       if ((newScore = scoreVector[matchSeqPos].noGap - newGapCost) >
           (continueGapScore = scoreVector[matchSeqPos].gapExists - gapExtend))
         continueGapScore = newScore;
       /*compute new score extending one position in matchSeq and query*/
       newScore = prevScoreNoGapMatchSeq + matrixRow[matchSeq[matchSeqPos]];
       if (newScore < 0)
       newScore = 0; /*Smith-Waterman locality condition*/
       /*test two alternatives*/
       if (newScore < prevScoreGapMatchSeq)
         newScore = prevScoreGapMatchSeq;
       if (newScore < continueGapScore)
         newScore = continueGapScore;
       prevScoreNoGapMatchSeq = scoreVector[matchSeqPos].noGap; 
       scoreVector[matchSeqPos].noGap = newScore;
       scoreVector[matchSeqPos].gapExists = continueGapScore;
       if (newScore > bestScore) {
         bestScore = newScore;
         bestQueryPos = queryPos;
         bestMatchSeqPos = matchSeqPos;
       }
       if (bestScore >= score)
         break;
     }
     if (bestScore >= score)
       break;
   }
   MemFree(scoreVector);
   if (bestScore < 0)
     bestScore = 0;
   *matchSeqStart = bestMatchSeqPos;
   *queryStart = bestQueryPos;
   return(bestScore);
}


/**
 * computes Smith-Waterman local alignment score and returns the
 * evalue assuming some positions are forbidden matchSeqEnd and query
 * can be used to run the local alignment in reverse to find optimal
 * starting positions
 * @param                   matchSeq is the matchSeq sequence [in]
 * @param matchSeqLength    is the length of matchSeq in amino acids [in]
 * @param query             is the input query sequence  [in]
 * @param queryLength       is the length of query [in]
 * @param matrix            is either the position-specific matrix associated
 *                          with query or the standard matrix [in]
 * @param gapOpen           is the cost of opening a gap [in]
 * @param gapExtend         is the cost of extending an existing gap by 1
 *                          position [in]
 * @param matchSeqEnd       returns the final position in the matchSeq of an
 *                          optimal local alignment [in]
 * @param queryEnd          returns the final position in query of an optimal
 *                          local alignment [in]
 * @param score             is used to pass back the optimal score [out]
 * @param kbp               holds the Karlin-Altschul parameters  [in]
 * @param effSearchSpace    effective search space [in]
 * @param numForbidden      number of forbidden ranges [in]
 * @param forbiddenRanges   lists areas that should not be aligned [in]
 * @param positionSpecific  determines whether matrix is position specific
 *                          or not [in]
 */
static Nlm_FloatHi
BLspecialSmithWatermanScoreOnly(Uint1 * matchSeq,
   Int4 matchSeqLength, Uint1 *query, Int4 queryLength, BLAST_Score **matrix,
   Int4 gapOpen, Int4 gapExtend,
   Int4 *matchSeqEnd, Int4 *queryEnd, Int4 *score,
   BLAST_KarlinBlkPtr kbp,  Nlm_FloatHi effSearchSpace,
   Int4 *numForbidden, Int4 ** forbiddenRanges, Boolean positionSpecific)
{

   Int4 bestScore; /*best score seen so far*/
   Int4 newScore;  /* score of next entry*/
   Int4 bestMatchSeqPos, bestQueryPos; /*position ending best score in
                           matchSeq and database sequences*/
   SWpairs *scoreVector; /*keeps one row of the Smith-Waterman matrix
                           overwrite old row with new row*/
   BLAST_Score *matrixRow; /*one row of score matrix*/
   Int4 newGapCost; /*cost to have a gap of one character*/
   Int4 prevScoreNoGapMatchSeq; /*score one row and column up
                               with no gaps*/
   Int4 prevScoreGapMatchSeq;   /*score if a gap already started in matchSeq*/
   Int4 continueGapScore; /*score for continuing a gap in query*/
   Int4 matchSeqPos, queryPos; /*positions in matchSeq and query*/
   Nlm_FloatHi returnEvalue; /*e-value to return*/
   Boolean forbidden; /*is this position forbidden?*/
   Int4 f; /*index over forbidden positions*/


   scoreVector = (SWpairs *) MemNew(matchSeqLength * sizeof(SWpairs));
   bestMatchSeqPos = 0;
   bestQueryPos = 0;
   bestScore = 0;
   newGapCost = gapOpen + gapExtend;
   for (matchSeqPos = 0; matchSeqPos < matchSeqLength; matchSeqPos++) {
     scoreVector[matchSeqPos].noGap = 0;
     scoreVector[matchSeqPos].gapExists = -(gapOpen);
   }
   for(queryPos = 0; queryPos < queryLength; queryPos++) {  
     if (positionSpecific)
       matrixRow = matrix[queryPos];
     else
       matrixRow = matrix[query[queryPos]];
     newScore = 0;
     prevScoreNoGapMatchSeq = 0;
     prevScoreGapMatchSeq = -(gapOpen);
     for(matchSeqPos = 0; matchSeqPos < matchSeqLength; matchSeqPos++) {
       /*testing scores with a gap in matchSeq, either starting a new
         gap or extending an existing gap*/
       if ((newScore = newScore - newGapCost) > 
	   (prevScoreGapMatchSeq = prevScoreGapMatchSeq - gapExtend))
         prevScoreGapMatchSeq = newScore;
       /*testing scores with a gap in query, either starting a new
         gap or extending an existing gap*/
       if ((newScore = scoreVector[matchSeqPos].noGap - newGapCost) >
           (continueGapScore = scoreVector[matchSeqPos].gapExists - gapExtend))
         continueGapScore = newScore;
       /*compute new score extending one position in matchSeq and query*/
       forbidden = FALSE;
       for(f = 0; f < numForbidden[queryPos]; f++) {
         if ((matchSeqPos >= forbiddenRanges[queryPos][2 * f]) &&
	     (matchSeqPos <= forbiddenRanges[queryPos][2*f + 1])) {
	   forbidden = TRUE;
	   break;
	 }
       }
       if (forbidden)
         newScore = BLAST_SCORE_MIN;
       else
	 newScore = prevScoreNoGapMatchSeq + matrixRow[matchSeq[matchSeqPos]];
       if (newScore < 0)
	 newScore = 0; /*Smith-Waterman locality condition*/
       /*test two alternatives*/
       if (newScore < prevScoreGapMatchSeq)
	 newScore = prevScoreGapMatchSeq;
       if (newScore < continueGapScore)
	 newScore = continueGapScore;
       prevScoreNoGapMatchSeq = scoreVector[matchSeqPos].noGap; 
       scoreVector[matchSeqPos].noGap = newScore;
       scoreVector[matchSeqPos].gapExists = continueGapScore;
       if (newScore > bestScore) {
	 bestScore = newScore;
	 bestQueryPos = queryPos;
	 bestMatchSeqPos = matchSeqPos;

       }
     }
   }
   MemFree(scoreVector);
   if (bestScore < 0)
     bestScore = 0;
   *matchSeqEnd = bestMatchSeqPos;
   *queryEnd = bestQueryPos;
   *score = bestScore;
   returnEvalue = BlastKarlinStoE_simple(bestScore,kbp, effSearchSpace);
   return(returnEvalue);
}


/**
 * computes where optimal Smith-Waterman local alignment starts given
 * the ending positions.  matchSeqEnd and queryEnd can be used to run
 * the local alignment in reverse to find optimal starting positions
 * these are passed back in matchSeqStart and queryStart the optimal
 * score is passed in to check when it has been reached going
 * backwards the score is also returned
 * @param matchSeq          is the matchSeq sequence [in]
 * @param matchSeqLength    is the length of matchSeq in amino acids [in]
 * @param query             is the sequence corresponding to some matrix
 *                          profile [in]
 * @param matrix            is the position-specific matrix associated with
 *                          query [in]
 * @param gapOpen           is the cost of opening a gap [in]
 * @param gapExtend         is the cost of extending an existing gap by 1
 *                          position [in]
 * @param matchSeqEnd       is the final position in the matchSeq of an optimal
 *                          local alignment [in]
 * @param queryEnd          is the final position in query of an optimal
 *                          local alignment [in]
 * @param score             optimal score is passed in to check when it has
 *                          been reached going backwards [in]
 * @param matchSeqStart     optimal starting point [in]
 * @param queryStart        optimal starting point [in]
 * @param numForbidden      array of regions not to be aligned. [in]
 * @param numForbidden      array of regions not to be aligned. [in]
 * @param forbiddenRanges   regions not to be aligned. [in]
 * @param positionSpecific  determines whether matrix is position specific
 *                          or not
 * @return the score found
 */
static Int4 BLspecialSmithWatermanFindStart(Uint1 * matchSeq, 
   Int4 matchSeqLength, Uint1 *query, Int4 queryLength, BLAST_Score **matrix, 
   Int4 gapOpen, Int4 gapExtend,  Int4 matchSeqEnd, Int4 queryEnd, Int4 score,
   Int4 *matchSeqStart, Int4 *queryStart, Int4 *numForbidden, 
   Int4 ** forbiddenRanges, Boolean positionSpecific)
{

   Int4 bestScore; /*best score seen so far*/
   Int4 newScore;  /* score of next entry*/
   Int4 bestMatchSeqPos, bestQueryPos; /*position starting best score in
                           matchSeq and database sequences*/
   SWpairs *scoreVector; /*keeps one row of the Smith-Waterman matrix
                           overwrite old row with new row*/
   BLAST_Score *matrixRow; /*one row of score matrix*/
   Int4 newGapCost; /*cost to have a gap of one character*/
   Int4 prevScoreNoGapMatchSeq; /*score one row and column up
                               with no gaps*/
   Int4 prevScoreGapMatchSeq;   /*score if a gap already started in matchSeq*/
   Int4 continueGapScore; /*score for continuing a gap in query*/
   Int4 matchSeqPos, queryPos; /*positions in matchSeq and query*/
   Boolean forbidden; /*is this position forbidden?*/
   Int4 f; /*index over forbidden positions*/

   scoreVector = (SWpairs *) MemNew(matchSeqLength * sizeof(SWpairs));
   bestMatchSeqPos = 0;
   bestQueryPos = 0;
   bestScore = 0;
   newGapCost = gapOpen + gapExtend;
   for (matchSeqPos = 0; matchSeqPos < matchSeqLength; matchSeqPos++) {
     scoreVector[matchSeqPos].noGap = 0;
     scoreVector[matchSeqPos].gapExists = -(gapOpen);
   }
   for(queryPos = queryEnd; queryPos >= 0; queryPos--) {  
     if (positionSpecific)
       matrixRow = matrix[queryPos];
     else
       matrixRow = matrix[query[queryPos]];
     newScore = 0;
     prevScoreNoGapMatchSeq = 0;
     prevScoreGapMatchSeq = -(gapOpen);
     for(matchSeqPos = matchSeqEnd; matchSeqPos >= 0; matchSeqPos--) {
       /*testing scores with a gap in matchSeq, either starting a new
         gap or extending an existing gap*/
       if ((newScore = newScore - newGapCost) > 
	   (prevScoreGapMatchSeq = prevScoreGapMatchSeq - gapExtend))
         prevScoreGapMatchSeq = newScore;
       /*testing scores with a gap in query, either starting a new
         gap or extending an existing gap*/
       if ((newScore = scoreVector[matchSeqPos].noGap - newGapCost) >
           (continueGapScore = scoreVector[matchSeqPos].gapExists - gapExtend))
         continueGapScore = newScore;
       /*compute new score extending one position in matchSeq and query*/
       forbidden = FALSE;
       for(f = 0; f < numForbidden[queryPos]; f++) {
         if ((matchSeqPos >= forbiddenRanges[queryPos][2 * f]) &&
	     (matchSeqPos <= forbiddenRanges[queryPos][2*f + 1])) {
	   forbidden = TRUE;
	   break;
	 }
       }
       if (forbidden)
         newScore = BLAST_SCORE_MIN;
       else
	 newScore = prevScoreNoGapMatchSeq + matrixRow[matchSeq[matchSeqPos]];
       if (newScore < 0)
       newScore = 0; /*Smith-Waterman locality condition*/
       /*test two alternatives*/
       if (newScore < prevScoreGapMatchSeq)
         newScore = prevScoreGapMatchSeq;
       if (newScore < continueGapScore)
         newScore = continueGapScore;
       prevScoreNoGapMatchSeq = scoreVector[matchSeqPos].noGap; 
       scoreVector[matchSeqPos].noGap = newScore;
       scoreVector[matchSeqPos].gapExists = continueGapScore;
       if (newScore > bestScore) {
         bestScore = newScore;
         bestQueryPos = queryPos;
         bestMatchSeqPos = matchSeqPos;
       }
       if (bestScore >= score)
         break;
     }
     if (bestScore >= score)
       break;
   }
   MemFree(scoreVector);
   if (bestScore < 0)
     bestScore = 0;
   *matchSeqStart = bestMatchSeqPos;
   *queryStart = bestQueryPos;
   return(bestScore);
}


/**
 * Kappa_SequenceData - represents a string of amino acids or nucleotides
 */
typedef struct Kappa_SequenceData {
  Uint1Ptr data;                /**< amino acid or nucleotide data */
  Int4 length;                  /**< the length of data. For amino acid data
                                     &data[-1] is a valid address and
                                     data[-1] == 0. */
  Uint1Ptr buffer;              /**< if non-nil, points to memory that
                                     must be freed when this instance of
                                     Kappa_SequenceData is deleted. */
} Kappa_SequenceData;


/** Release the data associated with this object. */
static void
Kappa_SequenceDataRelease(Kappa_SequenceData * self)
{
  if(self->buffer) MemFree(self->buffer);

  self->data   = NULL;
  self->buffer = NULL;
}


/**
 * An instance of Kappa_ForbiddenRanges is used by the Smith-Waterman
 * algorithm to represent ranges in the database that are not to be
 * aligned.
 */
typedef struct Kappa_ForbiddenRanges {
  Boolean  isEmpty;             /**< True if there are no forbidden ranges */
  Int4    *numForbidden;        /**< how many forbidden ranges at each db
                                     position */
  Int4   **ranges;              /**< forbidden ranges for each database
                                     position */
  Int4     queryLength;         /**< length of the query sequence */
} Kappa_ForbiddenRanges;


/**
 * Initialize a new, empty Kappa_ForbiddenRanges
 *
 * @param self              object to be initialized
 * @param queryLength       the length of the query
 */
static void
Kappa_ForbiddenRangesInitialize(
  Kappa_ForbiddenRanges * self,
  Int4 queryLength)
{
  Int4 f;
  self->queryLength  = queryLength;
  self->numForbidden = (Int4 *) MemNew(queryLength * sizeof(Int4));
  self->ranges       = (Int4 **) MemNew(queryLength * sizeof(Int4 *));
  self->isEmpty      = TRUE;

  for(f = 0; f < queryLength; f++) {
    self->numForbidden[f] = 0;
    self->ranges[f]       = (Int4 *) MemNew(2 * sizeof(Int4));
    self->ranges[f][0]    = 0;
    self->ranges[f][1]    = 0;
  }
}


/** Reset self to be empty */
static void
Kappa_ForbiddenRangesClear(Kappa_ForbiddenRanges * self)
{
  Int4 f;
  for(f = 0; f < self->queryLength; f++) {
    self->numForbidden[f] = 0;
  }
  self->isEmpty = TRUE;
}


/** Add some ranges to self
 * @param self          an instance of Kappa_ForbiddenRanges [in][out]
 * @param queryStart    start of the alignment in the query sequence
 * @param queryAlignmentExtent  length of the alignment in the query sequence
 * @param matchStart    start of the alignment in the subject sequence
 * @param matchAlignmentExtent  length of the alignment in the
 *                              subject sequence
 */
static void
Kappa_ForbiddenRangesPush(
  Kappa_ForbiddenRanges * self,
  Int4 queryStart,
  Int4 queryAlignmentExtent,
  Int4 matchStart,
  Int4 matchAlignmentExtent)
{
  Int4 f;
  for(f = queryStart; f < (queryStart + queryAlignmentExtent); f++) {
    Int4 last = 2 * self->numForbidden[f];
    if(0 != last) {    /* we must resize the array */
      self->ranges[f] =
        (Int4 *) MemMore(self->ranges[f], (last + 2) * sizeof(Int4));
    }
    self->ranges[f][last]     = matchStart;
    self->ranges[f][last + 1] = matchStart + matchAlignmentExtent;

    self->numForbidden[f]++;
  }
  self->isEmpty = FALSE;
}


/**
 * Release the storage associated with the fields of self, but do not
 * delete self
 *
 * @param self          an instance of Kappa_ForbiddenRanges [in][out]
 */
static void
Kappa_ForbiddenRangesRelease(Kappa_ForbiddenRanges * self)
{
  Int4 f;
  for(f = 0; f < self->queryLength; f++)  MemFree(self->ranges[f]);

  MemFree(self->ranges);       self->ranges       = NULL;
  MemFree(self->numForbidden); self->numForbidden = NULL;
}


/**
 * Calls BLbasicSmithWatermanScoreOnly if forbiddenRanges is empty and
 * calls BLspecialSmithWatermanScoreOnly otherwise.  This routine has
 * the same parameters and return value as
 * BLspecialSmithWatermanScoreOnly.
 */
static Nlm_FloatHi
SmithWatermanScoreOnly(Kappa_SequenceData * subject,
                       Kappa_SequenceData * query,
                       BLAST_Score **matrix,
                       Int4 gapOpen,
                       Int4 gapExtend,
                       Int4 *matchSeqEnd,
                       Int4 *queryEnd,
                       Int4 *score,
                       BLAST_KarlinBlkPtr kbp,
                       Nlm_FloatHi effSearchSpace,
                       Boolean positionSpecific,
                       Kappa_ForbiddenRanges * forbiddenRanges )
{
  if( forbiddenRanges->isEmpty ) {
    return
      BLbasicSmithWatermanScoreOnly(subject->data, subject->length,
                                    query  ->data, query  ->length,
                                    matrix, gapOpen, gapExtend, matchSeqEnd,
                                    queryEnd, score, kbp, effSearchSpace,
                                    positionSpecific);
  } else {
    return
      BLspecialSmithWatermanScoreOnly(subject->data, subject->length,
                                      query  ->data, query  ->length,
                                      matrix, gapOpen, gapExtend, matchSeqEnd,
                                      queryEnd, score, kbp, effSearchSpace,
                                      forbiddenRanges->numForbidden,
                                      forbiddenRanges->ranges,
                                      positionSpecific);
  }
}


/**
 * Calls BLSmithWatermanFindStart if forbiddenRanges is empty and
 * calls BLspecialSmithWatermanFindStart otherwise.  This routine has
 * the same parameters and return value as
 * BLspecialSmithWatermanFindStart.
 */
static Int4
SmithWatermanFindStart(Kappa_SequenceData * subject,
                       Kappa_SequenceData * query,
                       BLAST_Score **matrix,
                       Int4 gapOpen,
                       Int4 gapExtend,
                       Int4 matchSeqEnd,
                       Int4 queryEnd,
                       Int4 score,
                       Int4 *matchSeqStart,
                       Int4 *queryStart,
                       Boolean positionSpecific,
                       Kappa_ForbiddenRanges * forbiddenRanges)
{
  if( forbiddenRanges->isEmpty ) {
    return
      BLSmithWatermanFindStart(subject->data, subject->length,
                               query  ->data, query  ->length,
                               matrix, gapOpen, gapExtend,
                               matchSeqEnd,   queryEnd,   score,
                               matchSeqStart, queryStart,
                               positionSpecific);
  } else {
    return
      BLspecialSmithWatermanFindStart(subject->data, subject->length,
                                      query  ->data, query  ->length,
                                      matrix, gapOpen, gapExtend,
                                      matchSeqEnd,   queryEnd,   score,
                                      matchSeqStart, queryStart,
                                      forbiddenRanges->numForbidden,
                                      forbiddenRanges->ranges,
                                      positionSpecific);
  }
}


/**
 * allocates  a score matrix with numPositions + 1 rows; all elements in row
 * numPositions are set to  BLAST_SCORE_MIN
 * @param numPositions length of matrix (or query) [in]
 * @return matrix (BLAST_Score**)
 */
static BLAST_Score **allocateScaledMatrix(Int4 numPositions)
{
  BLAST_Score **returnMatrix; /*allocated matrix to return*/
  Int4 row; /*loop index*/
  Int4 c; /*loop index over characters*/

  returnMatrix = (BLAST_Score**) MemNew((numPositions + 1) * sizeof (BLAST_Score *));
  for(row = 0; row <= numPositions; row++)
    returnMatrix[row] = (BLAST_Score *) MemNew(PROTEIN_ALPHABET * sizeof(BLAST_Score));
  for(c = 0; c < PROTEIN_ALPHABET; c++)
    returnMatrix[numPositions][c] = BLAST_SCORE_MIN;
  return(returnMatrix);
}

/** deallocate a score matrix*/
static void freeScaledMatrix(BLAST_Score **matrix, Int4 numPositions)
{
  int row; /*loop index*/

  for(row = 0; row <= numPositions; row++)
    MemFree(matrix[row]);
  MemFree(matrix);
}

/**
 * allocates a frequency ratio matrix with numPositions + 1 rows; all
 * elements in row numPositions are set to BLAST_SCORE_MIN
 *
 * @param numPositions length of matrix (or query) [in]
 * @return matrix (double**)
 */
static Nlm_FloatHi **allocateStartFreqs(Int4 numPositions)
{
  Nlm_FloatHi **returnMatrix; /*allocated matrix to return*/
  Int4 row; /*loop index*/
  Int4 c; /*loop index over characters*/

  returnMatrix = (Nlm_FloatHi**) MemNew((numPositions + 1) * sizeof(Nlm_FloatHi *));
  for(row = 0; row <= numPositions; row++)
    returnMatrix[row] = (Nlm_FloatHi *) MemNew(PROTEIN_ALPHABET * sizeof(Nlm_FloatHi));
  for(c = 0; c < PROTEIN_ALPHABET; c++)
    returnMatrix[numPositions][c] = BLAST_SCORE_MIN;
  return(returnMatrix);
}


/*deallocate a frequency ratio matrix*/
static void freeStartFreqs(Nlm_FloatHi **matrix, Int4 numPositions)
{
  int row; /*loop index*/

  for(row = 0; row <= numPositions; row++)
    MemFree(matrix[row]);
  MemFree(matrix);
}

/**
 * @param matrix          is a position-specific score matrix with matrixLength
 *                        positions
 * @param subjectProbArray  is an array containing the probability of
 *                        occurrence of each residue in the subject
 * @param queryProbArray  is an array containing the probability of
 *                        occurrence of each residue in the query
 * @param scoreArray      is an array of probabilities for each score that is
 *                        to be used as a field in return_sfp
 * @param return_sfp      is a the structure to be filled in and returned
 * @param range           is the size of scoreArray and is an upper bound on
 *                        the difference between maximum score and minimum
 *                        score in the matrix
 * the routine posfillSfp computes the probability of each score
 * weighted by the probability of each query residue and fills those
 * probabilities into scoreArray and puts scoreArray as a field in
 * that in the structure that is returned for indexing convenience the
 * field storing scoreArray points to the entry for score 0, so that
 * referring to the -k index corresponds to score -k
 */
static BLAST_ScoreFreqPtr notposfillSfp(BLAST_Score **matrix, Nlm_FloatHi *subjectProbArray,  Nlm_FloatHi *queryProbArray, Nlm_FloatHi *scoreArray,  BLAST_ScoreFreqPtr return_sfp, Int4 range)
{
  Int4 minScore, maxScore; /*observed minimum and maximum scores*/
  Int4 i,j,k; /* indices */

  minScore = maxScore = 0;

  for(i = 0; i < PROTEIN_ALPHABET; i++) {
    for(j = 0 ; j < PRO_TRUE_ALPHABET_SIZE; j++) {
      k = trueCharPositions[j];
      if ((matrix[i][k] != BLAST_SCORE_MIN) && (matrix[i][k] < minScore))
	minScore = matrix[i][k];
      if (matrix[i][k] > maxScore)
        maxScore = matrix[i][k];
    }
  }
  return_sfp->obs_min = minScore;
  return_sfp->obs_max = maxScore;
  for (i = 0; i < range; i++)
    scoreArray[i] = 0.0;
  return_sfp->sprob = &(scoreArray[-minScore]); /*center around 0*/
  for(i = 0; i < PROTEIN_ALPHABET; i++) {
    for (j = 0; j < PRO_TRUE_ALPHABET_SIZE; j++) {
      k = trueCharPositions[j];
      if(matrix[i][k] >= minScore) {
        return_sfp->sprob[matrix[i][k]] += (queryProbArray[i] * subjectProbArray[k]);
      }
    }
  }
  return_sfp->score_avg = 0;
  for(i = minScore; i <= maxScore; i++)
    return_sfp->score_avg += i * return_sfp->sprob[i];
  return(return_sfp);
}


/**
 *
 * @param matrix            is a position-specific score matrix with
 *                          matrixLength positions
 * @param matrixLength      length of the position-specific matrix above
 * @param subjectProbArray  is an array containing the probability of
 *                          occurrence of each residue in the matching
 *                          sequence often called the subject
 * @param  scoreArray       is an array of probabilities for each score
 *                          that is to be used as a field in return_sfp
 * @param return_sfp        is a the structure to be filled in and returned
 *                          range is the size of scoreArray and is an upper
 *                          bound on the difference between maximum score
 *                          and minimum score in the matrix
 * @param range             is the size of scoreArray and is an upper bound on
 *                          the difference between maximum score and minimum
 *                          score in the matrix
 * the routine posfillSfp computes the probability of each score
 * weighted by the probability of each query residue and fills those
 * probabilities into scoreArray and puts scoreArray as a field in
 * that in the structure that is returned for indexing convenience the
 * field storing scoreArray points to the entry for score 0, so that
 * referring to the -k index corresponds to score -k
 */
static BLAST_ScoreFreqPtr posfillSfp(BLAST_Score **matrix, Int4 matrixLength, Nlm_FloatHi *subjectProbArray, Nlm_FloatHi *scoreArray,  BLAST_ScoreFreqPtr return_sfp, Int4 range)
{
  Int4 minScore, maxScore; /*observed minimum and maximum scores*/
  Int4 i,j,k; /* indices */
  Nlm_FloatHi onePosFrac; /*1/matrix length as a double*/

  minScore = maxScore = 0;

  for(i = 0; i < matrixLength; i++) {
    for(j = 0 ; j < PRO_TRUE_ALPHABET_SIZE; j++) {
      k = trueCharPositions[j];
      if ((matrix[i][k] != BLAST_SCORE_MIN) && (matrix[i][k] < minScore))
	minScore = matrix[i][k];
      if (matrix[i][k] > maxScore)
        maxScore = matrix[i][k];
    }
  }
  return_sfp->obs_min = minScore;
  return_sfp->obs_max = maxScore;
  for (i = 0; i < range; i++)
    scoreArray[i] = 0.0;
  return_sfp->sprob = &(scoreArray[-minScore]); /*center around 0*/
  onePosFrac = 1.0/ ((Nlm_FloatHi) matrixLength);
  for(i = 0; i < matrixLength; i++) {
    for (j = 0; j < PRO_TRUE_ALPHABET_SIZE; j++) {
      k = trueCharPositions[j];
      if(matrix[i][k] >= minScore) {
        return_sfp->sprob[matrix[i][k]] += (onePosFrac * subjectProbArray[k]);
      }
    }
  }
  return_sfp->score_avg = 0;
  for(i = minScore; i <= maxScore; i++)
    return_sfp->score_avg += i * return_sfp->sprob[i];
  return(return_sfp);
}


/**
 * Given a sequence of 'length' amino acid residues, compute the
 * probability of each residue and put that in the array resProb
 */
static void fillResidueProbability(Uint1Ptr sequence, Int4 length, Nlm_FloatHi * resProb)
{
  Int4 frequency[PROTEIN_ALPHABET]; /*frequency of each letter*/
  Int4 i; /*index*/
  Int4 localLength; /*reduce for X characters*/

  localLength = length;
  for(i = 0; i < PROTEIN_ALPHABET; i++)
    frequency[i] = 0;
  for(i = 0; i < length; i++) {
    if (Xchar != sequence[i])
      frequency[sequence[i]]++;
    else
      localLength--;
  }
  for(i = 0; i < PROTEIN_ALPHABET; i++) {
    if (frequency[i] == 0)
      resProb[i] = 0.0;
    else
      resProb[i] = ((Nlm_FloatHi) (frequency[i])) /((Nlm_FloatHi) localLength);
  }
}


#define posEpsilon 0.0001

/**
 * Return the a matrix of the frequency ratios that underlie the score
 * matrix being used on this pass. The returned matrix is
 * position-specific, so if we are in the first pass, use query to
 * convert the 20x20 standard matrix into a position-specific
 * variant. matrixName is the name of the underlying 20x20 score
 * matrix used. numPositions is the length of the query;
 * startNumerator is the matrix of frequency ratios as stored in
 * posit.h. It needs to be divided by the frequency of the second
 * character to get the intended ratio
 *
 * @param search            general search information [in]
 * @param query             the query sequence [in]
 * @param matrixName        name of the underlying matrix [in]
 * @param startNumerator    matrix of frequency ratios [in]
 * @param numPositions      length of the query [in]
 * @param positionSpecific  is this a position-specific search? [in]
 */
static Nlm_FloatHi **getStartFreqRatios(BlastSearchBlkPtr search,
                    Uint1Ptr query,
                    Char *matrixName,
                    Nlm_FloatHi **startNumerator,
                    Int4 numPositions,
                    Boolean positionSpecific)
{
   Nlm_FloatHi ** returnRatios; /*frequency ratios to start
                                  investigating each pair*/
   Int4 i,j;
   FreqRatios * stdFreqRatios = NULL;

   returnRatios = allocateStartFreqs(numPositions);

   stdFreqRatios = PSIMatrixFrequencyRatiosNew(matrixName);
   if (positionSpecific) {
    for(i = 0; i < numPositions; i++) {
       for(j = 0; j < PROTEIN_ALPHABET; j++) {
         returnRatios[i][j] = stdFreqRatios->data[query[i]][j];
       }
     }
   } else {
     for (i = 0; i < PROTEIN_ALPHABET; i++) {
         for (j = 0; j < PROTEIN_ALPHABET; j++) {
             returnRatios[i][j] = stdFreqRatios->data[i][j];
         }
     }
   }
   stdFreqRatios = PSIMatrixFrequencyRatiosFree(stdFreqRatios);

   if (positionSpecific) {
     Nlm_FloatHi *standardProb; /*probabilities of each letter*/
     BLAST_ResFreqPtr stdrfp; /* gets standard frequencies in prob field */

     stdrfp = BlastResFreqNew(search->sbp);
     BlastResFreqStdComp(search->sbp,stdrfp);
     standardProb = &stdrfp->prob[0];

     /*reverse multiplication done in posit.c*/
     for(i = 0; i < numPositions; i++) {
       for(j = 0; j < PROTEIN_ALPHABET; j++) {
         if ((standardProb[query[i]] > posEpsilon) &&
             (standardProb[j] > posEpsilon) &&
             (j != StarChar) && (j != Xchar) &&
             (startNumerator[i][j] > posEpsilon))
         {
           returnRatios[i][j] = startNumerator[i][j]/standardProb[j];
         }
       }
     }
     stdrfp = BlastResFreqDestruct(stdrfp);
   }

   return(returnRatios);
}


/**
 * take every entry of startFreqRatios that is not corresponding to a
 * score of BLAST_SCORE_MIN and take its log, divide by Lambda and
 * multiply by LambdaRatio then round to the nearest integer and put
 * the result in the corresponding entry of matrix. startMatrix and
 * matrix have dimensions numPositions X PROTEIN_ALPHABET
 *
 * @param matrix preallocated matrix to be filled in [out]
 * @param startMatrix matrix to be scaled up [in]
 * @param startFreqRatios frequency ratios of starting matrix [in]
 * @param numPositions length of query [in]
 * @param Lambda A Karlin-Altschul parameter. [in]
 * @param LambdaRatio ratio of correct Lambda to it's original value [in]
 */
static void scaleMatrix(BLAST_Score **matrix, BLAST_Score **startMatrix,
            Nlm_FloatHi **startFreqRatios, Int4 numPositions,
            Nlm_FloatHi Lambda, Nlm_FloatHi LambdaRatio)
{
   Int4 p, c; /*indices over positions and characters*/
   Nlm_FloatHi temp; /*intermediate term in computation*/

   for (p = 0; p < numPositions; p++) {
     for (c = 0; c < PROTEIN_ALPHABET; c++) {
       if (matrix[p][c] == BLAST_SCORE_MIN)
         matrix[p][c] = startMatrix[p][c];
       else {
         temp = log(startFreqRatios[p][c]);
         temp = temp/Lambda;
         temp = temp * LambdaRatio;
         matrix[p][c] = Nlm_Nint(temp);
       }
     }
   }
}


/** SCALING_FACTOR is a multiplicative factor used to get more bits of
 * precision in the integer matrix scores. It cannot be arbitrarily
 * large because we do not want total alignment scores to exceedto
 * -(BLAST_SCORE_MIN) */
#define SCALING_FACTOR 32


/**
 * Compute a scaled up version of the standard matrix encoded by
 * matrix name.  Standard matrices are in half-bit units.
 *
 * @param matrix preallocated matrix [in][out]
 * @param matrixName name of matrix (e.g., BLOSUM62, PAM30). [in]
 * @param Lambda A Karlin-Altschul parameter. [in]
 */
static void
computeScaledStandardMatrix(
  BLAST_Score **matrix,
  Char *matrixName,
  Nlm_FloatHi Lambda)
{
  int i,j; /*loop indices*/
  FreqRatios * freqRatios;  /* frequency ratios for the matrix */

  freqRatios = PSIMatrixFrequencyRatiosNew(matrixName);
  if(freqRatios == NULL) {
    ErrPostEx(SEV_FATAL, 1, 0, "blastpgp: Cannot adjust parameters "
              "for matrix %s\n", matrixName);
  }
  for(i = 0; i < PROTEIN_ALPHABET; i++) {
    for(j = 0; j < PROTEIN_ALPHABET; j++) {
      if(0.0 == freqRatios->data[i][j]) {
        matrix[i][j] = BLAST_SCORE_MIN;
      } else {
        Nlm_FloatHi temp = log(freqRatios->data[i][j])/Lambda;
        matrix[i][j] = Nlm_Nint(temp);
      }
    }
  }
  freqRatios = PSIMatrixFrequencyRatiosFree(freqRatios);
}


/**
 * produce a scaled-up version of the position-specific matrix
 * starting from posFreqs
 *
 * @param fillPosMatrix     is the matrix to be filled
 * @param nonposMatrix      is the underlying position-independent matrix,
 *                          used to fill positions where frequencies are
 *                          irrelevant
 * @param sbp               stores various parameters of the search
 */
void scalePosMatrix(BLAST_Score **fillPosMatrix, BLAST_Score **nonposMatrix, Char *matrixName, Nlm_FloatHi **posFreqs, Uint1 *query, Int4 queryLength, BLAST_ScoreBlkPtr sbp)
{

     posSearchItems *posSearch; /*used to pass data into scaling routines*/
     compactSearchItems *compactSearch; /*used to pass data into scaling routines*/
     Int4 i,j ; /*loop indices*/   
     BLAST_ResFreqPtr stdrfp; /* gets standard frequencies in prob field */
     Int4 a; /*index over characters*/


     posSearch = (posSearchItems *) MemNew (1 * sizeof(posSearchItems));
     compactSearch = (compactSearchItems *) MemNew (1 * sizeof(compactSearchItems));
     posSearch->posMatrix = (BLAST_Score **) MemNew((queryLength + 1) * sizeof(BLAST_Score *));
     posSearch->posPrivateMatrix = fillPosMatrix;
     posSearch->posFreqs = posFreqs;
     posSearch->stdFreqRatios = PSIMatrixFrequencyRatiosNew(matrixName);
     for(i = 0; i <= queryLength; i++) 
       posSearch->posMatrix[i] = (BLAST_Score *) MemNew(PROTEIN_ALPHABET * sizeof(BLAST_Score));

     compactSearch->query = (Uint1Ptr) query;
     compactSearch->qlength = queryLength;
     compactSearch->alphabetSize = PROTEIN_ALPHABET;
     compactSearch->gapped_calculation = TRUE;
     compactSearch->matrix = nonposMatrix;
     compactSearch->lambda =  sbp->kbp_gap_std[0]->Lambda;
     compactSearch->kbp_std = sbp->kbp_std;
     compactSearch->kbp_psi = sbp->kbp_psi;
     compactSearch->kbp_gap_psi = sbp->kbp_gap_psi;
     compactSearch->kbp_gap_std = sbp->kbp_gap_std;
     compactSearch->lambda_ideal = sbp->kbp_ideal->Lambda;
     compactSearch->K_ideal = sbp->kbp_ideal->K;

     stdrfp = BlastResFreqNew(sbp);
     BlastResFreqStdComp(sbp,stdrfp); 
     compactSearch->standardProb = MemNew(compactSearch->alphabetSize * sizeof(Nlm_FloatHi));
     for(a = 0; a < compactSearch->alphabetSize; a++)
       compactSearch->standardProb[a] = stdrfp->prob[a];
     stdrfp = BlastResFreqDestruct(stdrfp);

     posFreqsToMatrix(posSearch,compactSearch);
     impalaScaling(posSearch, compactSearch, ((Nlm_FloatHi) SCALING_FACTOR), FALSE);

     for(i = 0; i <= queryLength; i++)
       MemFree(posSearch->posMatrix[i]);

     MemFree(compactSearch->standardProb);
     MemFree(posSearch->posMatrix);
     PSIMatrixFrequencyRatiosFree(posSearch->stdFreqRatios);
     MemFree(posSearch);
     MemFree(compactSearch);
}


/**
 * Kappa_WindowInfo - a struct whose instances represent a range
 * of data in a sequence. */
typedef struct Kappa_WindowInfo
{
  Int4 begin;  /**< the starting index of the range */
  Int4 end;    /**< one beyond the last item in the range */
  Int4 frame;  /**< the translation frame of this window */
  Int4 hspcnt; /**< the number of HSPs aligned to a subset of the data
                    in this window's range. */
} Kappa_WindowInfo;


/**
 * A datatype used solely to enable a list of windows and of indices
 * to be simultaneously sorted in the WindowsFromHSPs routine.
 */
typedef struct Kappa_WindowIndexPair {
  Kappa_WindowInfo * window;    /**< a window */
  Int4 index;                   /**< an index associated with
                                     "window," typically the index of
                                     the window in a list, before the
                                     list is sorted. */
} Kappa_WindowIndexPair;

/**
 * A comparison routine used to sort a list of Kappa_WindowIndexPair
 * objects first by frame and then by location.
 */
static int LIBCALLBACK
location_compare_windows(void * vp1, void *vp2)
{
  /* w1 and w2 are the windows being compared */
  Kappa_WindowInfo * w1 = ((Kappa_WindowIndexPair *) vp1)->window;
  Kappa_WindowInfo * w2 = ((Kappa_WindowIndexPair *) vp2)->window;

  Int4 result;                   /* result of the comparison */
  if(0 == (result = BLAST_CMP(w1->frame, w2->frame)) &&
     0 == (result = BLAST_CMP(w1->begin, w2->begin))) {
      result = BLAST_CMP(w1->end, w2->end);
  }
  return (int) result;
}


/**
 * Reads a array of HSPs and creates a new array of pointers to
 * Kappa_WindowInfo so that each element in the array of HSPs is
 * contained in exactly one window
 *
 * @param hsp_array         hsp array to be read [in]
 * @param hspcnt            length of hsp_array [in]
 * @param border            Number of extra amino acids to include
 *                          at the start and end of each HSP.
 * @param sequence_length   length of the sequence containing these
 *                          HSPs, in nucleotide coordinates.
 * @param pwindows          a pointer to an array of windows;
 *                          the array may be resized by this routine. [in][out]
 * @param nWindows          the number of windows in *pwindows [in][out]
 * @param lWindows          the allocated length of *pwindows [in][out]
 * @param window_of_hsp     HSP i is contained in the bounds of
 *                          window_of_hsp[i]  [in][out]
 */
static void
WindowsFromHSPs(
  BLASTResultHsp hsp_array[],
  Int4 hspcnt,
  Int4 border,
  Int4 sequence_length,
  Kappa_WindowInfo ***pwindows,
  Int4 * nWindows,
  Int4 * lWindows,
  Int4 * window_of_hsp)
{
  Int4 k, ell;
  Kappa_WindowIndexPair * window_and_index;  /* an array of windows
                                              * paired with the index
                                              * of the HSP that
                                              * generated them */
  Kappa_WindowInfo     ** windows;      /* the output list of windows */
  Int4 start_cluster;    /* start of a cluster of windows to be joined */
  Int4 length_joined;    /* the current length of the list of joined windows */

  windows = *pwindows;
  /* Make the window list have exactly hspcnt windows. */
  if( *lWindows < hspcnt ) {
    *lWindows = 2 * hspcnt;
    windows = *pwindows =
      MemMore(*pwindows, *lWindows *  sizeof(Kappa_WindowInfo*));
  }
  for( k = *nWindows; k < hspcnt; k++ ) {
    windows[k] = MemNew(sizeof(Kappa_WindowInfo));
  }
  for( k = hspcnt; k < *nWindows; k++ ) {
    MemFree(windows[k]);
  }
  *nWindows = hspcnt;

  window_and_index = MemNew(hspcnt * sizeof(Kappa_WindowIndexPair));

  for( k = 0; k < hspcnt; k++ ) { /* for all HSPs */
    /* length of the translation of the nucleotide sequence in this frame */
    Int4 translated_length;

    windows[k]->frame = hsp_array[k].subject_frame;

    if( windows[k]->frame > 0 ) {
      translated_length = (sequence_length - windows[k]->frame + 1)/3;
    } else {
      translated_length = (sequence_length + windows[k]->frame - 1)/3;
    }
    windows[k]->begin = MAX(0, hsp_array[k].subject_offset - border);
    windows[k]->end   = MIN(translated_length,
                            hsp_array[k].subject_offset +
                            hsp_array[k].subject_length + border);
    windows[k]->hspcnt = 1;

    window_and_index[k].index  = k;
    window_and_index[k].window = windows[k];
  }
  HeapSort(window_and_index, hspcnt, sizeof(Kappa_WindowIndexPair),
           location_compare_windows);

  /* Join windows that overlap or are too close together.  */
  start_cluster = 0;
  length_joined = 0;
  for( k = 0; k < hspcnt; k++ ) {       /* for all windows in the
                                           original list */
    Kappa_WindowInfo * window;          /* window at this value of k */
    Kappa_WindowInfo * nextWindow;      /* window at the next value of k, or
                                           NULL if no such window exists */
    window     = window_and_index[k].window;
    nextWindow = ( k + 1 < hspcnt ) ? window_and_index[k+1].window : NULL;

    if(nextWindow != NULL                 && /* there is a next window; and */
       window->frame == nextWindow->frame && /* it is in the same frame; and
                                                it is very near this one */
       window->end >= nextWindow->begin) {
      /* Join the current window with the next window.  Do not add the
         current window to the output list. */
      nextWindow->begin = MIN(window->begin, nextWindow->begin);
      nextWindow->end   = MAX(window->end,   nextWindow->end  );

      MemFree(window);
      window_and_index[k].window = NULL;  /* Set the now dangling
                                             pointer to NULL */
    } else {
      /* Don't join the current window with the next window.  Add the
         current window to the output list instead */
      windows[length_joined] = window;
      for( ell = start_cluster; ell <= k; ell++ ) {
        window_of_hsp[window_and_index[ell].index] = length_joined;
      }
      length_joined++;
      start_cluster = k + 1;
    } /* end else don't join the current window with the next window */
  } /* end for all windows in the original list */
  *nWindows = length_joined;
  for( k = length_joined; k < hspcnt; k++ ) {
    windows[k] = NULL;
  }
  MemFree(window_and_index);
}


/**
 * Redo a S-W alignment using an x-drop alignment.  The result will
 * usually be the same as the S-W alignment. The call to ALIGN
 * attempts to force the endpoints of the alignment to match the
 * optimal endpoints determined by the Smith-Waterman algorithm.
 * ALIGN is used, so that if the data structures for storing BLAST
 * alignments are changed, the code will not break
 *
 * @param query         the query data
 * @param queryStart    start of the alignment in the query sequence
 * @param queryEnd      end of the alignment in the query sequence,
 *                      as computed by the Smith-Waterman algorithm
 * @param subject       the subject (database) sequence
 * @param matchStart    start of the alignment in the subject sequence
 * @param matchEnd      end of the alignment in the query sequence,
 *                      as computed by the Smith-Waterman algorithm
 * @param gap_align     parameters for a gapped alignment
 * @param score         score computed by the Smith-Waterman algorithm
 * @param localScalingFactor    the factor by which the scoring system has
 *                              been scaled in order to obtain greater
 *                              precision
 * @param queryAlignmentExtent  length of the alignment in the query sequence,
 *                              as computed by the x-drop algorithm
 * @param matchAlignmentExtent  length of the alignment in the subject
 *                              sequence, as computed by the x-drop algorithm
 * @param reverseAlignScript    alignment information (script) returned by a
 *                              x-drop alignment algorithm
 * @param newScore              alignment score computed by the x-drop
 *                              algorithm
 */
static void
Kappa_SWFindFinalEndsUsingXdrop(
  Kappa_SequenceData * query,
  Int4 queryStart,
  Int4 queryEnd,
  Kappa_SequenceData * subject,
  Int4 matchStart,
  Int4 matchEnd,
  GapAlignBlkPtr gap_align,
  Int4 score,
  Nlm_FloatHi localScalingFactor,
  Int4 * queryAlignmentExtent,
  Int4 * matchAlignmentExtent,
  Int4 ** reverseAlignScript,
  BLAST_Score * newScore)
{
  Int4 XdropAlignScore;         /* alignment score obtained using X-dropoff
                                 * method rather than Smith-Waterman */
  Int4 doublingCount = 0;       /* number of times X-dropoff had to be
                                 * doubled */
  do {
    Int4 *alignScript;          /* the alignment script that will be
                                   generated below by the ALIGN
                                   routine. */

    *reverseAlignScript = alignScript =
      (Int4 *) MemNew((subject->length + query->length + 3) * sizeof(Int4));

    XdropAlignScore =
      ALIGN(&(query->data[queryStart]) - 1, &(subject->data[matchStart]) - 1,
            queryEnd - queryStart + 1, matchEnd - matchStart + 1,
            *reverseAlignScript, queryAlignmentExtent,
            matchAlignmentExtent, &alignScript,
            gap_align, queryStart - 1, FALSE);

    gap_align->x_parameter *= 2;
    doublingCount++;
    if((XdropAlignScore < score) && (doublingCount < 3)) {
      MemFree(*reverseAlignScript);
    }
  } while((XdropAlignScore < score) && (doublingCount < 3));


  *newScore = XdropAlignScore;
}


/**
 * A Kappa_MatchingSequence represents a subject sequence to be aligned
 * with the query.  This abstract sequence is used to hide the
 * complexity associated with actually obtaining and releasing the
 * data for a matching sequence, e.g. reading the sequence from a DB
 * or translating it from a nucleotide sequence.
 *
 * We draw a distinction between a sequence itself, and strings of
 * data that may be obtained from the sequence.  The amino
 * acid/nucleotide data is represented by an object of type
 * Kappa_SequenceData.  There may be more than one instance of
 * Kappa_SequenceData per Kappa_MatchingSequence, each representing a
 * different range in the sequence, or a different translation frame.
 */
typedef struct Kappa_MatchingSequence {
  Int4          length;         /**< length of this matching sequence */
  Int4          index;          /**< index of this sequence in the database */
  Uint1         prog_number;    /**< identifies the type of blast search being
                                     performed. The type of search determines
                                     how sequence data should be obtained. */
  CharPtr       genetic_code;   /**< genetic code for translated searches */
  BioseqPtr     bsp_db;         /**< An object that represents this sequence
                                   in low level routines. */
  Boolean       bioseq_needs_unlock;    /**< true if the bsp_db must be
                                           disposed of by a call to
                                           BioseqUnlock, rather than
                                           BioseqFree */
  ReadDBFILEPtr rdfp;           /**< A pointer to a database from which
                                     sequences may be obtained */
} Kappa_MatchingSequence;


/**
 * Initialize a new matching sequence, obtaining information about the
 * sequence from the search.
 *
 * @param self              object to be initialized
 * @param search            search information
 * @param subject_index     index of the matching sequence in the database
 */
static void
Kappa_MatchingSequenceInitialize(
  Kappa_MatchingSequence * self,
  BlastSearchBlkPtr search,
  Int4 subject_index)
{
  self->prog_number  = search->prog_number;
  self->rdfp         = search->rdfp;
  self->index        = subject_index;

  if(self->prog_number ==  blast_type_tblastn) {
    self->genetic_code = search->db_genetic_code;
  } else {
    self->genetic_code = NULL;  /* the sequence will not be translated */
  }
  if(self->rdfp) {
    self->rdfp->parameters   |= READDB_KEEP_HDR_AND_SEQ;
    self->bsp_db              = readdb_get_bioseq(self->rdfp, self->index );
    self->bioseq_needs_unlock = FALSE;
  } else {
    self->bsp_db              = BioseqLockById(search->subject_info->sip);
    self->bioseq_needs_unlock = TRUE;
  }
  self->length = self->bsp_db->length;
}


/** Release the resources associated with a matching sequence. */
static void
Kappa_MatchingSequenceRelease(Kappa_MatchingSequence * self)
{
  if(self->bsp_db) {
    if(self->bioseq_needs_unlock) {
      BioseqUnlock(self->bsp_db);
      self->bsp_db        = NULL;
    } else {
      self->bsp_db        = BioseqFree(self->bsp_db);
    }
  }
}


/** NCBIstdaa encoding for 'X' character */
#define BLASTP_MASK_RESIDUE 21
/** Default instructions and mask residue for SEG filtering */
#define BLASTP_MASK_INSTRUCTIONS "S 10 1.8 2.1"


/**
 * Obtain a string of translated data
 *
 * @param self          the sequence from which to obtain the data [in]
 * @param window        the range and tranlation frame to get [in]
 * @param seqData       the resulting data [out]
 */
static void
Kappa_SequenceGetTranslatedWindow(Kappa_MatchingSequence * self,
                                  Kappa_WindowInfo * window,
                                  Kappa_SequenceData * seqData )
{
  Int4 i;
  Int4 nucleotide_start;        /* position of the first nucleotide to be
                                 * translated */
  Int4 num_nucleotides;         /* number of nucleotides to translate */

  nucleotide_start   = 3 * window->begin;
  num_nucleotides    = 3 * (window->end - window->begin);
  { /* scope of nucleotide_data */
    Uint1Ptr nucleotide_data = MemNew((num_nucleotides + 1) * sizeof(Uint1));
    { /* scope of spp */
      SeqPortPtr spp;       /* a SeqPort used to read the sequence data */
      Uint1 strand;         /* a flag indicating which strand to read */
      Uint1 nucleotide;     /* an individual nucleotide */

      strand = (window->frame >= 0) ? Seq_strand_plus : Seq_strand_minus;
      spp    = SeqPortNew(self->bsp_db, FIRST_RESIDUE, LAST_RESIDUE, strand,
                          Seq_code_ncbi4na);
      SeqPortSeek(spp, nucleotide_start, SEEK_SET);

      for(i = 0;
          i < num_nucleotides &&
            (nucleotide = SeqPortGetResidue(spp)) != SEQPORT_EOF;
          i++ ) {
        /* for all nucleotides in the translation range */
        nucleotide_data[i] = nucleotide;
      }

      spp = SeqPortFree(spp);
    }  /* end scope of spp */
    seqData->buffer =
      GetTranslation(nucleotide_data, num_nucleotides, window->frame,
                     &seqData->length, self->genetic_code);
    seqData->data = seqData->buffer + 1; /* This is a protein sequence,
                                            so the first byte is nil. */
    MemFree(nucleotide_data);
  } /* end scope of nucleotide_data */

#ifndef KAPPA_NO_SEG_SEQUENCE_TBLASTN
  { /* scope of variables used for seg filtering */
    BioseqPtr bsp_temp; /* a Bioseq for the translated sequence */
    ObjectIdPtr oip;    /* a unique ObjectId for the translated sequence */
    SeqLocPtr seg_slp;  /* a SeqLoc for SEG filtering */
    bsp_temp     = BlastMakeTempProteinBioseq(seqData->data, seqData->length,
                                              Seq_code_ncbistdaa);
    bsp_temp->id = SeqIdSetFree(bsp_temp->id);

    oip = (ObjectIdPtr) UniqueLocalId();
    ValNodeAddPointer(&(bsp_temp->id), SEQID_LOCAL, oip);
    SeqMgrAddToBioseqIndex(bsp_temp);

    seg_slp = BlastBioseqFilter(bsp_temp, BLASTP_MASK_INSTRUCTIONS);
    if (seg_slp) {
      HackSeqLocId(seg_slp, self->bsp_db->id);
      BlastMaskTheResidues(seqData->data, seqData->length,
                           BLASTP_MASK_RESIDUE, seg_slp, FALSE, 0);
      seg_slp = SeqLocSetFree(seg_slp);
    }

    SeqMgrDeleteFromBioseqIndex(bsp_temp);
    bsp_temp->id = SeqIdSetFree(bsp_temp->id);
    bsp_temp     = BioseqFree(bsp_temp);
  } /* end scope of variables used for seg filtering */
#endif
}


/**
 * Obtain the sequence data that lies within the given window.
 *
 * @param self          sequence information [in]
 * @param window        window specifying the range of data [in]
 * @param seqData       the sequence data obtained [out]
 */
static void
Kappa_SequenceGetWindow(
  Kappa_MatchingSequence * self,
  Kappa_WindowInfo * window,
  Kappa_SequenceData * seqData )
{
  if(self->prog_number ==  blast_type_tblastn) {
    /* The sequence must be translated. */
    Kappa_SequenceGetTranslatedWindow(self, window, seqData);
  } else {
    /* The sequence does not need to be translated. */
    /* Obtain the entire sequence (necessary for SEG filtering.) */
    if(self->rdfp != NULL) {
      Uint1Ptr origData;        /* data obtained from readdb_get_sequence;
                                 * this data cannot be modified, so we copy
                                 * it. */
      seqData->length    = readdb_get_sequence(self->rdfp, self->index,
                                               (Uint1Ptr PNTR) & origData );
      seqData->buffer    = MemNew((seqData->length + 2) * sizeof(Uint1));
      seqData->buffer[0] = '\0';
      seqData->data      = &seqData->buffer[1];
      MemCpy( seqData->data, origData, seqData->length + 1 );
    } else { /* self->rdfp is NULL */
      SeqPortPtr spp = NULL;      /* a SeqPort used to read the
                                     sequence data */
      Uint1      residue;         /* an individual residue */
      Int4       idx;

      seqData->length    = self->bsp_db->length;
      seqData->buffer    = MemNew((seqData->length + 2) * sizeof(Uint1));
      seqData->buffer[0] = '\0';
      seqData->data      = seqData->buffer + 1;

      spp =
        SeqPortNew(self->bsp_db, FIRST_RESIDUE, LAST_RESIDUE,
                   Seq_strand_unknown, Seq_code_ncbistdaa);

      idx = 0;
      while((residue = SeqPortGetResidue(spp)) != SEQPORT_EOF) {
        if(IS_residue(residue)) {
          /* Replace occurrences of amino acid number 24
             (Selenocysteine) with number 21 (Undetermined or
             atypical). */
          if(residue == 24) {
            residue = 21;
            fprintf(stderr, "Selenocysteine (U) at position %ld"
                    " replaced by X\n",
                    (long) idx + 1);
          }
          seqData->data[idx++] = residue;
        }
      }
      seqData->data[idx] = 0;    /* terminate the string */
      spp = SeqPortFree(spp);
    } /* end else self->rdfp is NULL */

#ifndef KAPPA_BLASTP_NO_SEG_SEQUENCE
    {
      SeqLocPtr seg_slp;  /*pointer to structure for SEG filtering*/

      seg_slp = BlastBioseqFilter(self->bsp_db, BLASTP_MASK_INSTRUCTIONS);
      if (seg_slp) {
        BlastMaskTheResidues(seqData->data, seqData->length,
                             BLASTP_MASK_RESIDUE, seg_slp, FALSE, 0);
        seg_slp = SeqLocSetFree(seg_slp);
      }
    }
#endif
    /* Fit the data to the window. */
    seqData ->data    = &seqData->data[window->begin - 1];
    *seqData->data++  = '\0';
    seqData ->length  = window->end - window->begin;
  } /* end else the sequence does not need to be translated */
}


/**
 * Converts an HSP, obtained from a blast search, to a GapAlignBlk that
 * is in a state in which it has all information necessary to redo the
 * computation of a traceback.
 *
 * @param gap_align     the GapAlignBlk to be modified  [out]
 * @param search        general information about the search [in]
 * @param hsp           the HSP to be converted [in]
 * @param window,       the window used to compute the traceback [in]
 * @param query,        the query data [in]
 * @param subject       the subject data [in]
 */
static void
HitToGapAlign(
  GapAlignBlkPtr gap_align,
  BlastSearchBlkPtr search,
  BLAST_HSPPtr       hsp,
  Kappa_WindowInfo * window,
  Kappa_SequenceData * query,
  Kappa_SequenceData * subject)
{
  gap_align->query           = query->data;
  gap_align->query_length    = query->length;
  gap_align->query_frame     = 0;

  gap_align->subject         = subject->data;
  gap_align->subject_length  = subject->length;
  gap_align->subject_frame   = hsp->subject.frame;

  hsp->subject.offset       -= window->begin;
  hsp->subject.gapped_start -= window->begin;
  if(CheckStartForGappedAlignment(search, hsp, gap_align->query,
                                   gap_align->subject, search->sbp->matrix)) {
    /* We may use the starting point supplied by the HSP. */
    gap_align->q_start = hsp->query.gapped_start;
    gap_align->s_start = hsp->subject.gapped_start;
  } else {
    /* We must recompute the start for the gapped alignment, as the
       one in the HSP was unacceptable.*/
    gap_align->q_start =
      GetStartForGappedAlignment(search, hsp, gap_align->query,
                                 gap_align->subject, search->sbp->matrix);
    gap_align->s_start =
      (hsp->subject.offset - hsp->query.offset) + gap_align->q_start;
  }
}


/**
 * Create a new Kappa_DistinctAlignment and append the list of
 * alignments represented by "next."
 *
 * @param query         query sequence data
 * @param queryStart    the start of the alignment in the query
 * @param queryEnd      the end of the alignment in the query
 * @param subject       subject sequence data
 * @param matchStart    the start of the alignment in the subject window
 * @param matchEnd      the end of the alignment in the subject window
 * @param score         the score of this alignment
 * @param window        the subject window of this alignment
 * @param gap_align     alignment info for gapped alignments
 * @param localScalingFactor    the factor by which the scoring system has
 *                              been scaled in order to obtain greater
 *                              precision
 * @param prog_number   the type of alignment being performed
 * @param queryLength   length of the full query sequence
 * @param subjectLength length of the full subject sequence
 * @param next          preexisting list of alignments [out]
 */
static Kappa_DistinctAlignment *
NewAlignmentUsingXdrop(
  Kappa_SequenceData * query,
  Int4 queryStart,
  Int4 queryEnd,
  Kappa_SequenceData * subject,
  Int4 matchStart,
  Int4 matchEnd,
  Int4 score,
  Kappa_WindowInfo * window,
  GapAlignBlkPtr gap_align,
  Nlm_FloatHi localScalingFactor,
  Int4 prog_number,
  Int4 queryLength,
  Int4 subjectLength,
  Kappa_DistinctAlignment * next)
{
  Int4 newScore;
  /* Extent of the alignment as computed by an x-drop alignment
   * (usually the same as (queryEnd - queryStart) and (matchEnd -
   * matchStart)) */
  Int4 queryExtent, matchExtent;
  Int4 * reverseAlignScript;      /* alignment script returned by the
                                     x-drop algorithm */
  Kappa_DistinctAlignment * obj;  /* the new object */

  Kappa_SWFindFinalEndsUsingXdrop(query,   queryStart, queryEnd,
                                  subject, matchStart, matchEnd,
                                  gap_align, score, localScalingFactor,
                                  &queryExtent, &matchExtent,
                                  &reverseAlignScript, &newScore);
  obj = MemNew(sizeof(Kappa_DistinctAlignment));

  obj->editBlock =
    TracebackToGapXEditBlock(NULL, NULL, queryExtent, matchExtent,
                             reverseAlignScript,
                             queryStart,
                             matchStart + window->begin);

  reverseAlignScript = MemFree(reverseAlignScript);

  obj->editBlock->discontinuous    = gap_align->discontinuous;
  obj->editBlock->original_length1 = queryLength;
  obj->editBlock->original_length2 = subjectLength;
  obj->editBlock->translate2       = prog_number == blast_type_tblastn;
  obj->editBlock->frame2           = window->frame;


  obj->score      = newScore;
  obj->queryStart = queryStart;
  obj->queryEnd   = obj->queryStart + queryExtent;
  obj->matchStart = matchStart      + window->begin;
  obj->matchEnd   = obj->matchStart + matchExtent;
  obj->frame      = window->frame;

  obj->next       = next;

  return obj;
}


/**
 * Reads a GapAlignBlk that has been used to compute a traceback, and
 * return a Kappa_DistinctAlignment representing the alignment.
 *
 * @param gap_align         the GapAlignBlk
 * @param window            the window used to compute the traceback
 * @param queryLength       original length of the query sequence
 * @param subjectLength     original length of the subject sequence
 */
static Kappa_DistinctAlignment *
NewAlignmentFromGapAlign(
  GapAlignBlkPtr gap_align,
  Kappa_WindowInfo * window,
  Int4 queryLength,
  Int4 subjectLength)
{
  Kappa_DistinctAlignment * obj; /* the new alignment */
  obj = MemNew(sizeof(Kappa_DistinctAlignment));

  obj->score      = gap_align->score;
  obj->queryStart = gap_align->query_start;
  obj->queryEnd   = gap_align->query_stop + 1;
  obj->matchStart = gap_align->subject_start + window->begin;
  obj->matchEnd   = gap_align->subject_stop  + window->begin + 1;
  obj->frame      = gap_align->subject_frame;

  if(gap_align->edit_block != NULL) {
    gap_align->edit_block->start2           += window->begin;
    gap_align->edit_block->length2          += window->begin;
    gap_align->edit_block->original_length1  = queryLength;
    gap_align->edit_block->original_length2  = subjectLength;
  }
  obj->editBlock        = gap_align->edit_block;
  gap_align->edit_block = NULL; /* set to NULL to avoid aliasing */
  obj->next             = NULL;

  return obj;
}


/**
 * A Kappa_SearchParameters represents the data needed by
 * RedoAlignmentCore to adjust the parameters of a search, including
 * the original value of these parameters
 */
typedef struct Kappa_SearchParameters {
  Int4          gapOpen;        /**< a penalty for the existence of a gap */
  Int4          gapExtend;      /**< a penalty for each residue (or
                                      nucleotide) in the gap */
  Int4          gapDecline;     /**< a penalty for declining to align a pair
                                     of residues */
  Int4          mRows;          /**< the number of rows in a scoring matrix. */
  Int4          nCols;          /**< the number of columns in a scoring
                                     matrix */

  Nlm_FloatHi   scaledUngappedLambda;   /**< The value of Karlin-Altschul
                                             parameter lambda, rescaled
                                             to allow scores to have
                                             greater precision */
  BLAST_Score **origMatrix;     /**< The original matrix values */
  BLAST_Score **startMatrix;    /**< Rescaled values of the original matrix */
                         
  Nlm_FloatHi **startFreqRatios;        /**< frequency ratios to start
                                             investigating each pair */
  Nlm_FloatHi  *scoreArray;     /**< array of score probabilities */
  Nlm_FloatHi  *resProb;        /**< array of probabilities for each residue
                                     in a matching sequence */
  Nlm_FloatHi  *queryProb;      /**< array of probabilities for each residue
                                     in the query */
  Boolean       adjustParameters;       /**< Use composition-based statistics
                                             if true. */
  GapAlignBlkPtr gap_align;

  BLAST_ScoreFreqPtr return_sfp;        /**< score frequency pointers to
                                             compute lambda */
  BLAST_KarlinBlkPtr kbp_gap_orig;  /**< copy of the original gapped
                                      Karlin-Altschul block corresponding to
                                      the first context */
  BLAST_KarlinBlkPtr * orig_kbp_gap_array;  /**< pointer to the array of gapped
                                              Karlin-Altschul block for all
                                              contexts (@todo is this really
                                              needed?) */
} Kappa_SearchParameters;


/**
 * Release the data associated with a Kappa_SearchParameters and
 * delete the object
 * @param searchParams the object to be deleted [in][out]
 */
static void
Kappa_SearchParametersFree(Kappa_SearchParameters ** searchParams)
{
  /* for convenience, remove one level of indirection from searchParams */
  Kappa_SearchParameters *sp = *searchParams;

  if(sp->kbp_gap_orig) BlastKarlinBlkDestruct(sp->kbp_gap_orig);

  if(sp->startMatrix)
    freeScaledMatrix(sp->startMatrix, sp->mRows);
  if(sp->origMatrix)
    freeScaledMatrix(sp->origMatrix, sp->mRows);
  if(sp->startFreqRatios)
    freeStartFreqs(sp->startFreqRatios, sp->mRows);

  if(sp->return_sfp) MemFree(sp->return_sfp);
  if(sp->scoreArray) MemFree(sp->scoreArray);
  if(sp->resProb)    MemFree(sp->resProb);
  if(sp->queryProb)  MemFree(sp->queryProb);

  Nlm_Free(*searchParams);
  *searchParams = NULL;
}


/**
 * Create a new instance of Kappa_SearchParameters
 *
 * @param rows              number of rows in the scoring matrix
 * @param adjustParameters  if true, use composition-based statistics
 * @param positionBased     if true, the search is position-based
 */
static Kappa_SearchParameters *
Kappa_SearchParametersNew(
  Int4 rows,
  Boolean adjustParameters,
  Boolean positionBased)
{
  Kappa_SearchParameters *sp;   /* the new object */
  sp = MemNew(sizeof(Kappa_SearchParameters));

  sp->orig_kbp_gap_array = NULL;
  
  sp->mRows = positionBased ? rows : PROTEIN_ALPHABET;
  sp->nCols = PROTEIN_ALPHABET;
    
  sp->kbp_gap_orig     = NULL;
  sp->startMatrix      = NULL;
  sp->origMatrix       = NULL;
  sp->startFreqRatios  = NULL;
  sp->return_sfp       = NULL;
  sp->scoreArray       = NULL;
  sp->resProb          = NULL;
  sp->queryProb        = NULL;
  sp->adjustParameters = adjustParameters;
  
  if(adjustParameters) {
    sp->kbp_gap_orig = BlastKarlinBlkCreate();
    sp->startMatrix  = allocateScaledMatrix(sp->mRows);
    sp->origMatrix   = allocateScaledMatrix(sp->mRows);
    
    sp->resProb    =
      (Nlm_FloatHi *) MemNew(PROTEIN_ALPHABET * sizeof(Nlm_FloatHi));
    sp->scoreArray =
      (Nlm_FloatHi *) MemNew(scoreRange * sizeof(Nlm_FloatHi));
    sp->return_sfp =
      (BLAST_ScoreFreqPtr) MemNew(1 * sizeof(BLAST_ScoreFreq));

    if(!positionBased) {
      sp->queryProb =
        (Nlm_FloatHi *) MemNew(PROTEIN_ALPHABET * sizeof(Nlm_FloatHi));
    }
  }
  /* end if(adjustParameters) */

  return sp;
}


/**
 * Record the initial value of the search parameters that are to be
 * adjusted.
 */
static void
Kappa_RecordInitialSearch(Kappa_SearchParameters * searchParams,
                          BlastSearchBlkPtr search)
{
  /* Create a gap_align if there is none */
  if(search->gap_align == NULL) {
    search->gap_align                = GapAlignBlkNew(1, 1);
  }
  search->gap_align->matrix        = search->sbp->matrix;
  search->gap_align->positionBased = search->positionBased;
  if(search->positionBased) {
    search->gap_align->posMatrix   = search->sbp->posMatrix;
  }
  search->gap_align->gap_open      = search->pbp->gap_open;
  search->gap_align->gap_extend    = search->pbp->gap_extend;
  search->gap_align->decline_align = search->pbp->decline_align;

  if(searchParams->adjustParameters) {
    Int4 i, j;
    BLAST_KarlinBlkPtr kbp;     /* statistical parameters used to evaluate a
                                 * query-subject pair */
    BLAST_Score **matrix;       /* matrix used to score a local
                                   query-subject alignment */
    Uint1Ptr query;               /* the query sequence */
    Int4 queryLength;             /* the length of the query sequence */

    query      = search->context[0].query->sequence;
    queryLength    = search->context[0].query->length;

    if(search->positionBased) {
      kbp    = search->sbp->kbp_gap_psi[0];
      matrix = search->sbp->posMatrix;
    } else {
      kbp    = search->sbp->kbp_gap_std[0];
      matrix = search->sbp->matrix;
      fillResidueProbability(query, queryLength, searchParams->queryProb);
    }
    searchParams->gapOpen    = search->pbp->gap_open;
    searchParams->gapExtend  = search->pbp->gap_extend;
    searchParams->gapDecline = search->pbp->decline_align;

    searchParams->orig_kbp_gap_array   = search->sbp->kbp_gap;

    searchParams->kbp_gap_orig->Lambda = kbp->Lambda;
    searchParams->kbp_gap_orig->K      = kbp->K;
    searchParams->kbp_gap_orig->logK   = kbp->logK;
    searchParams->kbp_gap_orig->H      = kbp->H;
    searchParams->gap_align            = search->gap_align;

    for(i = 0; i < searchParams->mRows; i++) {
      for(j = 0; j < PROTEIN_ALPHABET; j++) {
        searchParams->origMatrix[i][j] = matrix[i][j];
      }
    }
  }
}


/**
 * Rescale the search parameters in the search object and options
 * object to obtain more precision.
 */
static Nlm_FloatHi
Kappa_RescaleSearch(Kappa_SearchParameters * sp,
                    BlastSearchBlkPtr search,
                    BLAST_OptionsBlkPtr options)
{
  Nlm_FloatHi localScalingFactor;       /* the factor by which to
                                         * scale the scoring system in
                                         * order to obtain greater
                                         * precision */

  if(!sp->adjustParameters) {
    localScalingFactor = 1.0;
  } else {
    Nlm_FloatHi initialUngappedLambda;  /* initial value of the
                                         * statistical parameter
                                         * lambda used to evaluate
                                         * ungapped alignments */
    BLAST_KarlinBlkPtr kbp;     /* the statistical parameters used to
                                 * evaluate alignments of a
                                 * query-subject pair */
    Uint1Ptr query;             /* the query sequence */
    Int4 queryLength;           /* the length of the query sequence */

    if((0 == strcmp(options->matrix, "BLOSUM62_20"))) {
      localScalingFactor = SCALING_FACTOR / 10;
    } else {
      localScalingFactor = SCALING_FACTOR;
    }

    search->pbp->gap_open   = Nlm_Nint(sp->gapOpen   * localScalingFactor);
    search->pbp->gap_extend = Nlm_Nint(sp->gapExtend * localScalingFactor);
    if(sp->gapDecline != INT2_MAX) {
      search->pbp->decline_align =
        Nlm_Nint(sp->gapDecline * localScalingFactor);
    }
    search->gap_align->gap_open      = search->pbp->gap_open;
    search->gap_align->gap_extend    = search->pbp->gap_extend;
    search->gap_align->decline_align = search->pbp->decline_align;

    query       = search->context[0].query->sequence;
    queryLength = search->context[0].query->length;
    if(search->positionBased) {
      sp->startFreqRatios =
        getStartFreqRatios(search, query, options->matrix,
                           search->sbp->posFreqs, queryLength, TRUE);
      scalePosMatrix(sp->startMatrix, search->sbp->matrix, options->matrix,
                     search->sbp->posFreqs, query, queryLength, search->sbp);
      initialUngappedLambda = search->sbp->kbp_psi[0]->Lambda;
    } else {
      sp->startFreqRatios =
        getStartFreqRatios(search, query, options->matrix, NULL,
                           PROTEIN_ALPHABET, FALSE);
      initialUngappedLambda = search->sbp->kbp_ideal->Lambda;
    }
    sp->scaledUngappedLambda = initialUngappedLambda / localScalingFactor;
    if(!search->positionBased) {
      computeScaledStandardMatrix(sp->startMatrix,
                                  options->matrix,
                                  sp->scaledUngappedLambda);
    }
    if(search->positionBased) {
      kbp = search->sbp->kbp_gap_psi[0];
    } else {
      kbp = search->sbp->kbp_gap_std[0];
    }
    kbp->Lambda /= localScalingFactor;
    kbp->logK = log(kbp->K);
  }

  return localScalingFactor;
}


/** LambdaRatioLowerBound is used when the expected score is too large
 * causing impalaKarlinLambdaNR to give a Lambda estimate that
 * is too small, or to fail entirely returning -1*/
#define LambdaRatioLowerBound 0.5

/**
 * Adjust the search parameters
 *
 * @param sp            a record of the initial search parameters [in|out]
 * @param search        the object to be adjusted [in|out]
 * @param subject       data from the subject sequence [in]
 * @param matrix        a scoring matrix to be adjusted [out]
 * @return              scaling-factor to be used.
 */
static Int4
Kappa_AdjustSearch(
  Kappa_SearchParameters * sp,
  BlastSearchBlkPtr search,
  Kappa_SequenceData * subject,
  BLAST_Score ** matrix)
{
  Nlm_FloatHi LambdaRatio;      /* the ratio of the corrected lambda to the
                                 * original lambda */
  if(!sp->adjustParameters) {
    LambdaRatio = 1.0;
  } else {
    /* do adjust the parameters */
    BLAST_ScoreFreqPtr this_sfp; 
    Nlm_FloatHi correctUngappedLambda;  /* new value of ungapped lambda */

    Int4 queryLength;           /* the length of the query */
    queryLength =  search->context[0].query->length;
    /* compute and plug in new matrix here */
    fillResidueProbability(subject->data, subject->length, sp->resProb);

    if(search->positionBased) {
      this_sfp =
        posfillSfp(sp->startMatrix, queryLength, sp->resProb, sp->scoreArray,
                   sp->return_sfp, scoreRange);
    } else {
      this_sfp =
        notposfillSfp(sp->startMatrix, sp->resProb, sp->queryProb,
                      sp->scoreArray, sp->return_sfp, scoreRange);
    }
    correctUngappedLambda =
      impalaKarlinLambdaNR(this_sfp, sp->scaledUngappedLambda);

    /* impalaKarlinLambdaNR will return -1 in the case where the
     * expected score is >=0; however, because of the MAX statement 3
     * lines below, LambdaRatio should always be > 0; the succeeding
     * test is retained as a vestige, in case one wishes to remove the
     * MAX statement and allow LambdaRatio to take on the error value
     * -1 */

    LambdaRatio = correctUngappedLambda / sp->scaledUngappedLambda;
    LambdaRatio = MIN(1, LambdaRatio);
    LambdaRatio = MAX(LambdaRatio, LambdaRatioLowerBound);

    if(LambdaRatio > 0) {
      scaleMatrix(matrix, sp->startMatrix, sp->startFreqRatios, sp->mRows,
                  sp->scaledUngappedLambda, LambdaRatio);
    }
  }
  /* end else do adjust the parameters */

  return LambdaRatio > 0 ? 0 : 1;
}


/**
 * Restore the parameters that were adjusted to their original values
 * @param searchParams      a record of the original values [in]
 * @param search            the search to be restored [out]
 * @param options           the option block to be restored [out]
 * @param matrix            the scoring matrix to be restored [out]
 * @param SmithWaterman     if true, we have performed a Smith-Waterman
 *                          alignment with these search parameters [in]
 */
static void
Kappa_RestoreSearch(
  Kappa_SearchParameters * searchParams,
  BlastSearchBlkPtr search,
  BLAST_OptionsBlkPtr options,
  BLAST_Score ** matrix,
  Boolean SmithWaterman)
{
  if(searchParams->adjustParameters) {
    BLAST_KarlinBlkPtr kbp;     /* statistical parameters used to
                                   evaluate the significance of
                                   alignment of a query-subject
                                   pair */
    Int4 i, j;
    if(!SmithWaterman) {
      search->pbp->gap_x_dropoff_final =
        options->gap_x_dropoff_final * NCBIMATH_LN2 /
        searchParams->kbp_gap_orig->Lambda;;
    }
    search->pbp->gap_open      = searchParams->gapOpen;
    search->pbp->gap_extend    = searchParams->gapExtend;
    search->pbp->decline_align = searchParams->gapDecline;

    if(search->gap_align != searchParams->gap_align) {
      GapAlignBlkDelete(search->gap_align);
      search->gap_align = searchParams->gap_align;
    }
    search->sbp->kbp_gap       = searchParams->orig_kbp_gap_array;

    if(search->positionBased) {
      kbp = search->sbp->kbp_gap_psi[0];
    } else {
      kbp = search->sbp->kbp_gap_std[0];
    }
    kbp->Lambda = searchParams->kbp_gap_orig->Lambda;
    kbp->K      = searchParams->kbp_gap_orig->K;
    kbp->logK   = searchParams->kbp_gap_orig->logK;
    kbp->H      = searchParams->kbp_gap_orig->H;

    for(i = 0; i < searchParams->mRows; i++) {
      for(j = 0; j < PROTEIN_ALPHABET; j++) {
        matrix[i][j] = searchParams->origMatrix[i][j];
      }
    }
  }
}


/**
 *  Top level routine to recompute alignments for each
 *  match found by the gapped BLAST algorithm
 *
 *  @param search           is the structure with all the information about
 *                          the search
 *  @param options          is used to pass certain command line options
 *                          taken in by BLAST
 *  @param hitlist_count    is the number of old matches
 *  @param adjustParameters determines whether we are to adjust the
 *                          Karlin-Altschul parameters and score matrix
 *  @param SmithWaterman    determines whether the new local alignments
 *                          should be computed by the optimal Smith-Waterman
 *                          algorithm; SmithWaterman false means that
 *                          alignments will be recomputed by the current
 *                          X-drop algorithm as implemented in the procedure
 *                          ALIGN.
 *  It is assumed that at least one of adjustParameters and
 *  SmithWaterman is true when this procedure is called A linked list
 *  of alignments is returned; the alignments are sorted according to
 *  the lowest E-value of the best alignment for each matching
 *  sequence; alignments for the same matching sequence are in the
 *  list consecutively regardless of the E-value of the secondary
 *  alignments. Ties in sorted order are much rarer than for the
 *  standard BLAST method, but are broken deterministically based on
 *  the index of the matching sequences in the database.
 */
SeqAlignPtr
RedoAlignmentCore(BlastSearchBlkPtr search,
                  BLAST_OptionsBlkPtr options,
                  Int4 hitlist_count,
                  Boolean adjustParameters,
                  Boolean SmithWaterman)
{
  Int4 match_index;                  /* index over matches */
  Int4 cutoff_s;                /* minimum score that must be achieved
                                   by a newly-computed alignment */
  Boolean do_link_hsps;         /* if true, use BlastLinkHsps to
                                   compute e-values */
  SeqAlignPtr results = NULL;   /* list of SeqAligns to return */
  Kappa_SequenceData query;     /* data for the query sequence */
  Nlm_FloatHi localScalingFactor;       /* the factor by which to
                                         * scale the scoring system in
                                         * order to obtain greater
                                         * precision */

  BLAST_Score      **matrix;    /* score matrix */
  BLAST_KarlinBlkPtr kbp;       /* stores Karlin-Altschul parameters */
  Kappa_SearchParameters *searchParams; /* the values of the search
                                         * parameters that will be
                                         * recorded, altered in the
                                         * search structure in this
                                         * routine, and then restored
                                         * before the routine
                                         * exits. */
  Kappa_ForbiddenRanges   forbidden;    /* forbidden ranges for each
                                         * database position (used in
                                         * Smith-Waterman alignments) */
  SWheap  significantMatches;  /* a collection of alignments of the
                                * query sequence with sequences from
                                * the database */
  Kappa_WindowInfo ** windows; /* windows containing HSPs for
                                * a single query-subject pair */
  Int4 nWindows;               /* number of windows in the array
                                * "windows" */
  Int4 lWindows;               /* allocated size of "windows" */
  Int4 window_index;           /* window index for use in loops */
  BLAST_HSPPtr temp_hsp;       /* an HSP that may be used as a
                                * temporary object, for example if one
                                * needs to convert a Result_HSP to an
                                * HSP before calling a library
                                * routine. */
  temp_hsp  = MemNew(sizeof(BLAST_HSP));

  /* Initialize the window list to have a single window -- the most
     common case */
  lWindows   = 1;   nWindows = 1;
  windows    = MemNew(lWindows * sizeof(Kappa_WindowInfo *));
  windows[0] = MemNew(sizeof(Kappa_WindowInfo));

  SWheapInitialize(&significantMatches, options->hitlist_size,
                   options->hitlist_size, options->ethresh);

  /**** Validate parameters *************/
  if(0 == strcmp(options->matrix, "BLOSUM62_20") && !adjustParameters) {
    return 0;                   /* BLOSUM62_20 only makes sense if
                                 * adjustParameters is on */
  }
  /*****************/
  query.data   = search->context[0].query->sequence;
  query.length = search->context[0].query->length;

  if(SmithWaterman) {
    Kappa_ForbiddenRangesInitialize(&forbidden, query.length);
  }

  if(search->positionBased) {
    kbp    = search->sbp->kbp_gap_psi[0];
    matrix = search->sbp->posMatrix;
    if ( !matrix ) {
        Char* msg = "Cannot perform position-specific search without a PSSM";
        BlastConstructErrorMessage("RedoAlignmentCore", msg, 3,
                                   &(search->error_return));
        return NULL;
    }
    if(search->sbp->posFreqs == NULL) {
      search->sbp->posFreqs = allocatePosFreqs(query.length, PROTEIN_ALPHABET);
    }
  } else {
    kbp    = search->sbp->kbp_gap_std[0];
    matrix = search->sbp->matrix;
  }

  /* Initialize searchParams */
  searchParams =
    Kappa_SearchParametersNew(query.length, adjustParameters,
                              search->positionBased);
  Kappa_RecordInitialSearch(searchParams, search);
  localScalingFactor = Kappa_RescaleSearch(searchParams, search, options);

  do_link_hsps = search->prog_number == blast_type_tblastn;
  if(do_link_hsps) {
    cutoff_s = search->pbp->cutoff_s2 * localScalingFactor;
  } else {
    /* There is no cutoff score; we consider e-values instead */
    cutoff_s = 0;
  }
  for(match_index = 0; match_index < hitlist_count; match_index++) {
    /* for all matching sequences */
    Kappa_MatchingSequence matchingSeq; /* the data for a matching
                                         * database sequence */
    BLASTResultHitlistPtr thisMatch;    /* alignment data for the
                                         * current query-subject
                                         * match */
    Int4 * window_of_hsp;               /* index of each HSP in the
                                         * array "windows" */
    Kappa_WindowInfo * window;          /* current window in the
                                         * subject sequence */
    Kappa_DistinctAlignment * alignments;   /* list of alignments for this
                                             * query-subject pair */
    alignments = NULL;

    thisMatch = search->result_struct->results[match_index];
    if(thisMatch->hsp_array == NULL) {
      continue;
    }

    if(SWheapWillAcceptOnlyBelowCutoff(&significantMatches)) {
      /* Only matches with evalue <= options->ethresh will be saved */

      /* e-value for a sequence is the smallest e-value among the HSPs
       * matching a region of the sequence to the query */
      Nlm_FloatHi minEvalue = thisMatch->best_evalue;
      if(minEvalue > (EVALUE_STRETCH * options->ethresh)) {
        /* This match is likely to have an evalue > options->ethresh
         * and therefore, we assume that all other matches with higher
         * input e-values are also unlikely to get sufficient
         * improvement in a redone alignment */
        break;
      }
    }
    /* Get the sequence for this match */
    Kappa_MatchingSequenceInitialize(&matchingSeq, search,
                                     thisMatch->subject_id);

    window_of_hsp = MemNew(thisMatch->hspcnt * sizeof(Int4));
    if(search->prog_number == blast_type_tblastn) {
      /* Find the multiple translation windows used by tblastn queries. */
      WindowsFromHSPs(thisMatch->hsp_array, thisMatch->hspcnt,
                      KAPPA_WINDOW_BORDER, matchingSeq.length,
                      &windows, &nWindows, &lWindows, window_of_hsp);
    } else { /* the program is not tblastn, i.e. it is blastp */
      /* Initialize the single window used by blastp queries. */
      windows[0]->frame  = 0;
      windows[0]->hspcnt = thisMatch->hspcnt;
      windows[0]->begin  = 0;
      windows[0]->end    = matchingSeq.length;
    } /* else the program is blastp */
    if(SmithWaterman) {
      /* We are performing a Smith-Waterman alignment */
      for(window_index = 0; window_index < nWindows; window_index++) {
        /* for all window */
        Kappa_SequenceData subject;     /* sequence data for this window */

        window = windows[window_index];
        Kappa_SequenceGetWindow( &matchingSeq, window, &subject );

        if(0 ==
           Kappa_AdjustSearch(searchParams, search, &subject, matrix)) {
          /* Kappa_AdjustSearch ran without error; compute the new
             alignments. */
          BLAST_Score aSwScore;             /* score computed by the
                                             * Smith-Waterman algorithm. */
          Boolean alignment_is_significant; /* True if the score/evalue of
                                             * the Smith-Waterman alignment
                                             * is significant. */
          Kappa_ForbiddenRangesClear(&forbidden);
          do {
            Nlm_FloatHi newSwEvalue;    /* evalue as computed by the
                                         * Smith-Waterman algorithm */
            Int4 matchEnd, queryEnd;    /* end points of the alignments
                                         * computed by the Smith-Waterman
                                         * algorithm. */
            newSwEvalue =
              SmithWatermanScoreOnly(&subject, &query, matrix,
                                     search->gap_align->gap_open,
                                     search->gap_align->gap_extend,
                                     &matchEnd, &queryEnd, &aSwScore, kbp,
                                     search->searchsp_eff,
                                     search->positionBased,
                                     &forbidden);
            alignment_is_significant =
              ( do_link_hsps && aSwScore >= cutoff_s) ||
              (!do_link_hsps && newSwEvalue < search->pbp->cutoff_e &&
               SWheapWouldInsert(&significantMatches, newSwEvalue));

            if(alignment_is_significant) {
              Int4 matchStart, queryStart;  /* the start of the
                                             * alignment in the
                                             * match/query sequence */

              SmithWatermanFindStart(&subject, &query, matrix,
                                     search->gap_align->gap_open,
                                     search->gap_align->gap_extend,
                                     matchEnd, queryEnd, aSwScore,
                                     &matchStart, &queryStart,
                                     search->positionBased, &forbidden);

              search->gap_align->x_parameter =
                (Int4) (options->gap_x_dropoff_final *
                        NCBIMATH_LN2 / kbp->Lambda);

              alignments =
                NewAlignmentUsingXdrop(&query,   queryStart, queryEnd,
                                       &subject, matchStart, matchEnd,
                                       aSwScore, window, search->gap_align,
                                       localScalingFactor,
                                       search->prog_number, query.length,
                                       matchingSeq.length, alignments);

              Kappa_ForbiddenRangesPush(&forbidden,
                                        queryStart,
                                        alignments->queryEnd - queryStart,
                                        matchStart,
                                        alignments->matchEnd - matchStart);
            }
            /* end if the next local alignment is significant */
          } while(alignment_is_significant && window->hspcnt > 1);
          /* end do..while the next local alignment is significant, and
           * the original blast search found more than one alignment. */
        } /* end if Kappa_AdjustSearch ran without error.  */
        Kappa_SequenceDataRelease(&subject);
      } /* end for all windows */
    } else {
      /* else we are not performing a Smith-Waterman alignment */
      Int4 hsp_index;
      /* data for the current window */
      Kappa_SequenceData subject = {NULL,0,NULL};
      window_index  = -1;       /* -1 indicates that sequence data has
                                 * not been obtained for any window in
                                 * the list. */
      window        = NULL;

      for(hsp_index = 0; hsp_index < thisMatch->hspcnt; hsp_index++) {
        /* for all HSPs in thisMatch */
        if(!isAlreadyContained(&thisMatch->hsp_array[hsp_index], alignments,
                               kbp->Lambda, localScalingFactor)) {
          Kappa_DistinctAlignment * newAlign;   /* the new alignment */
          Boolean adjust_search_failed = FALSE; /* if true, AdjustSearch was
                                                 * called and failed. */
          if( window_index != window_of_hsp[hsp_index] ) {
            /* The current window doesn't contain this HSP. */
            Kappa_SequenceDataRelease(&subject);

            window_index = window_of_hsp[hsp_index];
            window       = windows[window_index];
            Kappa_SequenceGetWindow(&matchingSeq, window, &subject);

            adjust_search_failed =
              Kappa_AdjustSearch(searchParams, search, &subject, matrix);
          }  /* end if the current window doesn't contain this HSP */
          if(!adjust_search_failed) {
            CopyResultHspToHSP(&thisMatch->hsp_array[hsp_index], temp_hsp);
            HitToGapAlign(search->gap_align, search, temp_hsp, window,
                          &query, &subject);

            search->gap_align->x_parameter =
              options->gap_x_dropoff_final * NCBIMATH_LN2 / kbp->Lambda;

            PerformGappedAlignmentWithTraceback(search->gap_align);

            newAlign = NewAlignmentFromGapAlign(search->gap_align, window,
                                                query.length,
                                                matchingSeq.length);
            withDistinctEnds(&newAlign, &alignments);
          } /* end if adjust search failed */
        } /* end if not isAlreadyContained */
      } /* for all HSPs in thisMatch */
      Kappa_SequenceDataRelease(&subject);
    } /* end else we are not performing a Smith-Waterman alignment */
    MemFree(window_of_hsp);

    if( alignments != NULL) { /* alignments were found */
      BLAST_HitListPtr hitlist; /* a hitlist containing the newly-computed
                                 * alignments */
      Nlm_FloatHi bestEvalue; /* best evalue among alignments in the hitlist */
      Int4 hsp_index;

      HitlistFromDistinctAlignments(search, &alignments);
      hitlist = search->current_hitlist;

      if(hitlist->hspcnt > 1) { /* if there is more than one HSP, */
        /* then eliminate HSPs that are contained in a higher-scoring HSP. */
        HeapSort(hitlist->hsp_array, hitlist->hspcnt, sizeof(BLAST_HSPPtr),
                 score_compare_hsps);

        if(!SmithWaterman || nWindows > 1) {
          /* For SmithWaterman alignments in a single window, the
           * forbidden ranges rule does not allow one alignment to be
           * contained in another, so the call to HitlistReapContained is
           * not needed. */
          HitlistReapContained(hitlist->hsp_array, &hitlist->hspcnt);
        }
      }

      search->subject->length  = matchingSeq.bsp_db->length;
      if(do_link_hsps) {
        BlastLinkHsps(search);
      } else {
        BlastGetNonSumStatsEvalue(search);
      }
      /* Find the evalue of the best alignment in the list -- the list
       * is typically sorted by score and not by evalue, so a search is
       * necessary. */
      bestEvalue = DBL_MAX;
      for( hsp_index = 0; hsp_index < hitlist->hspcnt; hsp_index++ ) {
        if( hitlist->hsp_array[hsp_index]->evalue < bestEvalue ) {
          bestEvalue = hitlist->hsp_array[hsp_index]->evalue;
        }
      }
      if(bestEvalue <= search->pbp->cutoff_e &&
         SWheapWouldInsert(&significantMatches, bestEvalue)) {
        /* If the best alignment is significant, then create and save
         * a list of SeqAligns. */
        SeqAlignPtr aligns;     /* SeqAligns for this query-subject pair */

        BlastReapHitlistByEvalue(search);

        aligns =
          SeqAlignsFromHitlist(hitlist, matchingSeq.bsp_db->id,
                               search->query_id, kbp->Lambda, kbp->logK,
                               localScalingFactor, &bestEvalue );
        SWheapInsert(&significantMatches, aligns, bestEvalue,
                     thisMatch->subject_id );

      } /* end if the best alignment is significant */
    } /* end if any alignments were found */

    Kappa_MatchingSequenceRelease(&matchingSeq);
  }
  /* end for all matching sequences */
  results = SWheapToFlatList( &significantMatches );

  /* Clean up */
  for( window_index = 0; window_index < nWindows; window_index++ ) {
    MemFree(windows[window_index]);
  }
  MemFree(windows);
  MemFree(temp_hsp);

  if (search->current_hitlist) {
    search->current_hitlist->hspcnt_max = search->current_hitlist->hspcnt;
    search->current_hitlist = BlastHitListDestruct(search->current_hitlist);
  }

  SWheapRelease(&significantMatches);
  if(SmithWaterman) Kappa_ForbiddenRangesRelease(&forbidden);

  Kappa_RestoreSearch(searchParams, search, options, matrix, SmithWaterman);
  Kappa_SearchParametersFree(&searchParams);

  return (results);
}
