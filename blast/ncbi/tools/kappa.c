static char const rcsid[] = "$Id: kappa.c,v 6.39 2004/03/31 18:12:13 papadopo Exp $";

/* $Id: kappa.c,v 6.39 2004/03/31 18:12:13 papadopo Exp $ 
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

 $Revision: 6.39 $

 $Log: kappa.c,v $
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


#define EVALUE_STRETCH 5 /*by what factor might initially reported E-value
                           exceed true Evalue*/
#define PRO_TRUE_ALPHABET_SIZE 20
#define scoreRange 10000


extern Nlm_FloatHi LIBCALL impalaKarlinLambdaNR PROTO((BLAST_ScoreFreqPtr sfp, Nlm_FloatHi initialLambda));


/*positions of true characters in protein alphabet*/
Int4 charPositions[20] = {1,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,22};

/**
 * SWResultsNew Create a new instance of the SWResults struct, initializing
 *              it with values common to different kinds of searches 
 *
 *              The parameters of this function correspond directly to fields
 *              in the SWResults data structure */ 
static SWResults *
SWResultsNew(Uint1Ptr sequence,
             BLAST_Score newScore,
             BLAST_Score bestScore,
             Nlm_FloatHi newEvalue,
             Nlm_FloatHi bestEvalue,
             Boolean isFirstAlignment,
             Nlm_FloatHi lambda,
             Nlm_FloatHi logK,
             Int4 subject_index,
             SeqIdPtr subject_id)
{
  SWResults *newSW;             /* The newly created instance of SWResults */

  newSW = (SWResults *) MemNew(1 * sizeof(SWResults));
  if(newSW) {
    newSW->seq     = sequence;
    newSW->score   = bestScore;
    newSW->eValue  = bestEvalue;
    newSW->Lambda  = lambda;
    newSW->logK    = logK;

    newSW->scoreThisAlign   = newScore;
    newSW->eValueThisAlign  = newEvalue;
    newSW->isFirstAlignment = isFirstAlignment;
    newSW->subject_index    = subject_index;
    newSW->subject_id       = SeqIdSetDup(subject_id);

    newSW->next = NULL;
  }
  return newSW;
}


/**
 * An instance of struct Kappa_MatchRecord represents all alignments
 * of a query sequence to a matching subject sequence.
 *
 * For a given query-subject pair, a Kappa_MatchRecord is created once it
 * is known that the eValue of the best alignment is small enough to be 
 * significant.  Then alignments of the two sequences are added to the
 * Kappa_MatchRecord one at a time, using one of the following two routines
 * 
 * - Kappa_MatchRecordInsertSeqAlign inserts the alignment represented
 *   by a single SeqAlign into the match record.
 * - Kappa_MatchRecordInsertSwAlign inserts an alignment computed by
 *   the Smith-Waterman algorithm into the match record.
 * 
 * Alignments should be specified in order of smallest (best) e-value to
 * largest (worst) e-value.
 *
 * The Kappa_MatchRecord::alignments field stores the alignments in
 * the reverse order, i.e. from largest (worst) e-value to smallest
 * (best) e-value.  The reason the alignments are stored in reverse
 * order is that this order is consistent with the order that matches
 * are returned by a SWheap (see below), i.e. worst to best. 
 */ 

struct Kappa_MatchRecord {
  Nlm_FloatHi  eValue;          /* best evalue of all alignments the record */
  BLAST_Score  score;           /* best score of all alignments the record */  
  Uint1Ptr     sequence;        /* the subject sequence */
  Int4         subject_index;   /* the index number of the subject sequence */
  SeqIdPtr     id;              /* the id structure of the subject sequence */
  SWResults   *alignments;      /* a list of query-subject alignments */
};
typedef struct Kappa_MatchRecord Kappa_MatchRecord;


/**
 * Initialize a Kappa_MatchRecord.  Parameters to this function correspond
 * directly to fields of Kappa_MatchRecord */
static void
Kappa_MatchRecordInitialize(Kappa_MatchRecord * self,
                            Nlm_FloatHi eValue,
                            BLAST_Score score,
                            Uint1Ptr sequence,
                            Int4 subject_index,
                            SeqIdPtr id)
{
  self->eValue   = eValue;
  self->score    = score;
  self->sequence = sequence;
  self->id       = id;
  self->subject_index = subject_index;
  self->alignments    = NULL;
}


/**  
 * Insert an alignment represented by a seqAlign into the match
 * record.
 */
static void
Kappa_MatchRecordInsertSeqAlign(
  Kappa_MatchRecord * self,     /* the match record to be altered */
  SeqAlignPtr seqAlign,         /* the alignment to add to the match record */
  BLASTResultHspPtr hsp,        /* the hsp corresponding to the alignment */
  Nlm_FloatHi lambda,           /* a statistical parameter used to
                                 * evaluate the significance of the
                                 * match */
  Nlm_FloatHi logK,             /* a statistical parameter used to
                                 * evaluate the significance of the
                                 * match */
  Nlm_FloatHi localScalingFactor        /* the factor by which the
                                         * scoring system has been
                                         * scaled in order to obtain
                                         * greater precision */
) {
  BLAST_Score newScore;         /* The evalue of the alignment to be 
                                   inserted */
  Nlm_FloatHi newEvalue;        /* The evalue of the alignment to be
                                   inserted */
  SWResults *newSW;             /* A new SWResults object that
                                   represents the alignment to be
                                   inserted */

  newEvalue = hsp->e_value;
  newScore  = Nlm_Nint(((Nlm_FloatHi) hsp->score) / localScalingFactor);

  newSW =
    SWResultsNew(self->sequence, newScore, self->score, newEvalue,
                 self->eValue, (NULL == self->alignments),
                 localScalingFactor * lambda, logK,
                 self->subject_index, self->id);

  newSW->seqAlign   = seqAlign;
  newSW->queryStart = hsp->gap_info->start1;
  newSW->seqStart   = hsp->gap_info->start2;
  newSW->next       = self->alignments;

  self->alignments = newSW;
}


/**  
 * Insert an alignment computed by the Smith-Waterman algorithm into
 * the match record.
 */
static void
Kappa_MatchRecordInsertSwAlign(
  Kappa_MatchRecord * self,     /* the match record to be altered */
  BLAST_Score newScore,         /* the score of the alignment */
  Nlm_FloatHi newEvalue,        /* the evalue of the alignment */
  Nlm_FloatHi lambda,           /* a statistical parameter used to
                                 * evaluate the significance of the
                                 * match */
  Nlm_FloatHi logK,             /* a statistical parameter used to
                                 * evaluate the significance of the
                                 * match */
  Nlm_FloatHi localScalingFactor,        /* the factor by which the
                                         * scoring system has been
                                         * scaled in order to obtain
                                         * greater precision */
  Int4 matchStart,              /* start of the alignment in the subject */
  Int4 matchAlignmentExtent,    /* length of the alignment in the subject */
  Int4 queryStart,              /* start of the alignment in the query */  
  Int4 queryAlignmentExtent,    /* length of the alignment in the query */
  Int4 * reverseAlignScript     /* Alignment information (script)
                                 * returned by the X-drop alignment algorithm */

) {
  SWResults *newSW;             /* A new SWResults object that
                                   represents the alignment to be
                                   inserted */
  if(NULL == self->alignments) {
    /* This is the first sequence recorded for this match. Use the x-drop
     * score, "newScore", as the score for the sequence */
    self->score = newScore;
  }
  newSW =
    SWResultsNew(self->sequence, newScore, self->score, newEvalue,
                 self->eValue, (NULL == self->alignments),
                 lambda * localScalingFactor, logK, self->subject_index,
                 self->id);

  newSW->seqStart   = matchStart;
  newSW->seqEnd     = matchStart + matchAlignmentExtent;
  newSW->queryStart = queryStart;
  newSW->queryEnd   = queryStart + queryAlignmentExtent;
  newSW->next       = self->alignments;
  newSW->reverseAlignScript = reverseAlignScript;

  self->alignments  = newSW;
}


/**
 * The struct SWheapRecord data type is used below to define the
 * internal structure of a SWheap (see below).  A SWheapRecord
 * represents all alignments of a query sequence to a particular
 * matching sequence.
 *
 * The SWResults::theseAlignments field is a linked list of alignments
 * of the query-subject pair.  The list is ordered by evalue in
 * descending order. Thus the first element has biggest (worst) evalue
 * and the last element has smallest (best) evalue. 
 */
typedef struct SWheapRecord {
  Nlm_FloatHi bestEvalue;       /* best (smallest) evalue of all alignments
                                 * in the record */
  SWResults *theseAlignments;   /* a list of alignments */
} SWheapRecord;


/*Compare two records in the heap.  */
static Boolean
SWheapRecordCompare(SWheapRecord * place1,
                    SWheapRecord * place2)
{
  return ((place1->bestEvalue > place2->bestEvalue) ||
          (place1->bestEvalue == place2->bestEvalue &&
           place1->theseAlignments->subject_index >
           place2->theseAlignments->subject_index));
}


/*swap two records in the heap*/
static void
SWheapRecordSwap(SWheapRecord * heapArray,
                 Int4 i,
                 Int4 j)
{
  /* bestEvalue and theseAlignments are temporary variables used to
   * perform the swap. */
  Nlm_FloatHi bestEvalue       = heapArray[i].bestEvalue;
  SWResults *theseAlignments   = heapArray[i].theseAlignments;

  heapArray[i].bestEvalue      = heapArray[j].bestEvalue;
  heapArray[i].theseAlignments = heapArray[j].theseAlignments;

  heapArray[j].bestEvalue      = bestEvalue;
  heapArray[j].theseAlignments = theseAlignments;
}


#ifdef KAPPA_INTENSE_DEBUG

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

#define KAPPA_ASSERT(expr) ((expr) ? 0 : \
(fprintf( stderr, "KAPPA_ASSERT failed line %d: %s", __LINE__, #expr ), \
exit(1)))
#else
#define KAPPA_ASSERT(expr) (void)(0)
#endif


/* On entry, all but the first element of the array heapArray[i]
 * .. heapArray[n] are in valid heap order.  This routine rearranges
 * the elements so that on exit they all are in heap order. */
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


/* On entry, all but the last element of the array heapArray[i]
 * .. heapArray[n] are in valid heap order.  This routine rearranges
 * the elements so that on exit they all are in heap order. */
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
struct SWheap {
  Int4 n;                       /* The current number of elements */
  Int4 capacity;                /* The maximum number of elements that may be 
                                   inserted before the SWheap must be resized 
                                 */
  Int4 heapThreshold;           /* see above */
  Nlm_FloatHi ecutoff;          /* matches with evalue below ecutoff may
                                   always be inserted in the SWheap */
  Nlm_FloatHi worstEvalue;      /* the worst (biggest) evalue currently in
                                   the heap */

  SWheapRecord *array;          /* the SWheapRecord array if the SWheap is
                                   being represented as an unordered array */
  SWheapRecord *heapArray;      /* the SWheapRecord array if the SWheap is
                                   being represented as an heap-ordered
                                   array. At least one of (array, heapArray)
                                   is NULL */

};
typedef struct SWheap SWheap;


/* Convert a SWheap from a representation as an unordered array to
 * a representation as a heap-ordered array. */
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

/*When the heap is about to exceed its capacity, it will be grown by
 *the minimum of a multiplicative factor of SWHEAP_RESIZE_FACTOR
 *and an additive factor of SWHEAP_MIN_RESIZE. The heap never
 *decreases in size */
#define SWHEAP_RESIZE_FACTOR 1.5
#define SWHEAP_MIN_RESIZE 100

/* Return true if self would insert a match that had the given eValue */
static Boolean
SWheapWouldInsert(SWheap * self,
                  Nlm_FloatHi eValue)
{
  return self->n < self->heapThreshold ||
    eValue <= self->ecutoff ||
    eValue < self->worstEvalue;
}


/* Try to insert matchRecord into the SWheap. The alignments stored in
 * matchRecord are used directly, i.e. they are not copied, but are
 * rather stored in the SWheap or deleted */
static void
SWheapInsert(SWheap * self,
             Kappa_MatchRecord * matchRecord)
{
  if(self->array && self->n >= self->heapThreshold) {
    ConvertToHeap(self);
  }
  if(self->array != NULL) {
    /* "self" is currently a list. Add the new alignments to the end */
    SWheapRecord *heapRecord;   /* destination for the new alignments */ 
    heapRecord                  = &self->array[++self->n];
    heapRecord->bestEvalue      = matchRecord->eValue;
    heapRecord->theseAlignments = matchRecord->alignments;
    if( self->worstEvalue < matchRecord->eValue ) {
      self->worstEvalue = matchRecord->eValue;
    }
  } else {                      /* "self" is currently a heap */
    if(self->n < self->heapThreshold ||
       (matchRecord->eValue <= self->ecutoff &&
        self->worstEvalue <= self->ecutoff)) {
      SWheapRecord *heapRecord; /* Destination for the new alignments */
      /* The new alignments must be inserted into the heap, and all old
       * alignments retained */
      if(self->n >= self->capacity) {
        /* The heap must be resized */
        Int4 newCapacity;       /* capacity the heap will have after
                                 * it is resized */
        newCapacity      = MAX(SWHEAP_MIN_RESIZE + self->capacity,
                               SWHEAP_RESIZE_FACTOR * self->capacity);
        self->heapArray  = (SWheapRecord *)
          MemMore(self->heapArray, (newCapacity + 1) * sizeof(SWheapRecord));
        self->capacity   = newCapacity;
      }
      /* end if the heap must be resized */
      heapRecord    = &self->heapArray[++self->n];
      heapRecord->bestEvalue      = matchRecord->eValue;
      heapRecord->theseAlignments = matchRecord->alignments;

      SWheapifyUp(self->heapArray, self->n, self->n);
    } else {
      /* Some set of alignments must be discarded */
      SWResults *discardedAlignments = NULL;      /* alignments that
                                                   * will be discarded
                                                   * so that the new
                                                   * alignments may be
                                                   * inserted. */

      if(matchRecord->eValue >= self->worstEvalue) {
        /* the new alignments must be discarded */
        discardedAlignments = matchRecord->alignments;
      } else {
        /* the largest element in the heap must be discarded */
        SWheapRecord *heapRecord;     /* destination for the new alignments */
        discardedAlignments         = self->heapArray[1].theseAlignments;

        heapRecord                  = &self->heapArray[1];
        heapRecord->bestEvalue      = matchRecord->eValue;
        heapRecord->theseAlignments = matchRecord->alignments;

        SWheapifyDown(self->heapArray, 1, self->n);
      }
      /* end else the largest element in the heap must be discarded */
      while(discardedAlignments != NULL) {
        /* There are discarded alignments that have not been freed */
        SWResults *thisAlignment;     /* the head of the list of
                                       * discarded alignments */
        thisAlignment        = discardedAlignments;
        discardedAlignments  = thisAlignment->next;

        MemFree(thisAlignment->reverseAlignScript);
        SeqIdSetFree(thisAlignment->subject_id);
        MemFree(thisAlignment);
      }
      /* end while there are discarded alignments that have not been freed */
    } 
    /* end else some set of alignments must be discarded */
    
    self->worstEvalue = self->heapArray[1].bestEvalue;
    KAPPA_ASSERT(SWheapIsValid(self->heapArray, 1, self->n));
  }
  /* end else "self" is currently a heap. */

  /* The matchRecord->alignments pointer is no longer valid */
  matchRecord->alignments = NULL;
}


/* Return true if only matches with evalue <= self->ecutoff may be inserted. */
static Boolean
SWheapWillAcceptOnlyBelowCutoff(SWheap * self)
{
  return self->n >= self->heapThreshold && self->worstEvalue <= self->ecutoff;
}


/* Initialize a new SWheap; parameters to this function correspond
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


/* Release the storage associated with the fields of a SWheap. Don't
 * delete the SWheap structure itself. */
static void
SWheapRelease(SWheap * self)
{
  if(self->heapArray) free(self->heapArray);
  if(self->array) free(self->array);

  self->n = self->capacity = self->heapThreshold = 0;
  self->heapArray = NULL;
}


/* Remove and return the element in the SWheap with largest (worst) evalue */
static SWResults *
SWheapPop(SWheap * self)
{
  SWResults *results = NULL;

  ConvertToHeap(self);
  if(self->n > 0) { /* The heap is not empty */
    SWheapRecord *first, *last; /* The first and last elements of the
                                 * array that represents the heap. */
    first = &self->heapArray[1];
    last  = &self->heapArray[self->n];

    results = first->theseAlignments;
    
    first->theseAlignments = last->theseAlignments;
    first->bestEvalue      = last->bestEvalue;

    SWheapifyDown(self->heapArray, 1, --self->n);
  }
  
  KAPPA_ASSERT(SWheapIsValid(self->heapArray, 1, self->n));

  return results;
}


/* Convert a SWheap to a flat list of SWResults. Note that there
 * may be more than one alignment per match.  The list of all
 * alignments are sorted by the following keys:
 * - First by the evalue the best alignment between the query and a
 *   particular matching sequence;
 * - Second by the subject_index of the matching sequence; and
 * - Third by the evalue of each individual alignment.
 */
static SWResults *
SWheapToFlatList(SWheap * self)
{
  SWResults *list = NULL;       /* the new list of SWResults */
  SWResults *result;            /* the next list of alignments to be
                                   prepended to "list" */

  while(NULL != (result = SWheapPop(self))) {
    SWResults *head, *remaining;     /* The head and remaining
                                        elements in a list of
                                        alignments to be prepended to
                                        "list" */
    remaining = result;
    while(NULL != (head = remaining)) {
      remaining   = head->next;
      head->next  = list;
      list        = head;
    }
  }

  return list;
}


/*This procedure convert a list of SWAlign structures into a list of
  SeqAlign structures, which is returned. In the process score-related
  fields in the SeqAlign structures are set and the reverseAlignScript
  associated with each SW alignment is freed*/
SeqAlignPtr concatenateListOfSeqAligns(SWResults * SWAligns)
{
    SeqAlignPtr seqAlignList =NULL; /*list of SeqAligns to return*/
    SeqAlignPtr nextSeqAlign; /*new one to add*/
    SeqAlignPtr lastSeqAlign = NULL; /*last one on list*/
    SWResults *curSW, *oldSW; /*used to iterate down list*/
    ScorePtr thisScorePtr; /*stores the different types of score numbers
                             associated with an alignment*/
    Nlm_FloatHi bitScoreUnrounded; /*used to adjust bit score to unscaled 
				     Lambda*/

    curSW = SWAligns;
    while (curSW != NULL) {
      nextSeqAlign = curSW->seqAlign;
      thisScorePtr = nextSeqAlign->score;
      while ((thisScorePtr != NULL) &&
	     (StringICmp(thisScorePtr->id->str, "score") != 0))
	thisScorePtr = thisScorePtr->next;
      if(NULL == thisScorePtr)
	fprintf(stderr,"Could not find score\n");
      else
	thisScorePtr->value.intvalue = curSW->scoreThisAlign; 
      bitScoreUnrounded = ((curSW->scoreThisAlign * curSW->Lambda) 
			   - curSW->logK)/NCBIMATH_LN2;
      thisScorePtr = nextSeqAlign->score;
      while ((thisScorePtr != NULL) &&
	     (StringICmp(thisScorePtr->id->str, "bit_score") != 0))
	thisScorePtr = thisScorePtr->next;
      if(NULL == thisScorePtr)
	fprintf(stderr,"Could not find bit score\n");
      else
	thisScorePtr->value.realvalue = bitScoreUnrounded; 
      if (NULL == seqAlignList)
	seqAlignList = nextSeqAlign;
      else
	lastSeqAlign->next = nextSeqAlign;
      lastSeqAlign = nextSeqAlign;
      MemFree(curSW->reverseAlignScript);
      oldSW = curSW;
      curSW = curSW->next;
      
      SeqIdSetFree(oldSW->subject_id);
      MemFree(oldSW);
    }

    return(seqAlignList);
}

/*take as input an amino acid  string and its length; compute a filtered
  amino acid string and return the filtered string*/
void segResult(BioseqPtr bsp,
               Uint1 * inputString,
               Uint1 * resultString,
               Int4 length,
               CharPtr instructions,
               Uint1 mask_residue)
{
  SeqLocPtr seg_slp;  /*pointer to structure for seg filtering*/
  Int4 i; /*loop index*/
  Uint1 *buf, *bu; /*buffers for copying*/
  Uint1 *tmp; /*loop index for copying*/

   if (bsp->length != length)
   {
   	ErrPostEx(SEV_FATAL, 1, 0,
              "segResult: length and bsp->length are different");
	return;
   }

   seg_slp = BlastBioseqFilter(bsp, instructions );

    for (i = 0; i < length; i++)
      resultString[i] = inputString[i];

    if (seg_slp) {
      BlastMaskTheResidues(resultString,length,mask_residue,
                             seg_slp,FALSE, 0);
    }

    seg_slp = SeqLocSetFree(seg_slp);
}

/*computes Smith-Waterman local alignment score and returns the
  evalue
  matchSeq is a database sequence matched by this query
  matchSeqLength is the length of matchSeq in amino acids
  query is the input query sequence
  queryLength is the length of query
  matrix is the position-specific matrix associated with query
  gapOpen is the cost of opening a gap
  gapExtend is the cost of extending an existing gap by 1 position
  matchSeqEnd returns the final position in the matchSeq of an optimal
   local alignment
  queryEnd returns the final position in query of an optimal
   local alignment
  matchSeqEnd and queryEnd can be used to run the local alignment in reverse
   to find optimal starting positions
  score is used to pass back the optimal score
  kbp holds the Karlin-Altschul parameters 
  positionSpecific determines whether matrix is position specific or not*/

Nlm_FloatHi BLbasicSmithWatermanScoreOnly(Uint1 * matchSeq, 
   Int4 matchSeqLength, Uint1 *query, Int4 queryLength, BLAST_Score **matrix, 
   Int4 gapOpen, Int4 gapExtend,  Int4 *matchSeqEnd, Int4 *queryEnd, Int4 *score,
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

/*computes where optimal Smith-Waterman local alignment starts given the
  ending positions
  matchSeq is a database sequence matched by this query
  matchSeqLength is the length of matchSeq in amino acids
  query is the input query sequence 
  queryLength is the length of query
  matrix is the position-specific matrix associated with query
    or the standard matrix
  gapOpen is the cost of opening a gap
  gapExtend is the cost of extending an existing gap by 1 position
  matchSeqEnd is the final position in the matchSeq of an optimal
   local alignment
  queryEnd is the final position in query of an optimal
   local alignment
  matchSeqEnd and queryEnd can be used to run the local alignment in reverse
   to find optimal starting positions
  these are passed back in matchSeqStart and queryStart
  the optimal score is passed in to check when it has
   been reached going backwards
  the score is also returned
  positionSpecific determines whether matrix is position specific or not*/
  
Int4 BLSmithWatermanFindStart(Uint1 * matchSeq, 
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


/*computes Smith-Waterman local alignment score and returns the
  evalue assuming some positions are forbidden
  matchSeq is the matchSeq sequence
  matchSeqLength is the length of matchSeq in amino acids
  query is the input query sequence 
  queryLength is the length of query
  matrix is either the position-specific matrix associated with query
    or the standard matrix
  gapOpen is the cost of opening a gap
  gapExtend is the cost of extending an existing gap by 1 position
  matchSeqEnd returns the final position in the matchSeq of an optimal
   local alignment
  queryEnd returns the final position in query of an optimal
   local alignment
  matchSeqEnd and query can be used to run the local alignment in reverse
   to find optimal starting positions
  score is used to pass back the optimal score
  kbp holds the Karlin-Altschul parameters 
  positionSpecific determines whether matrix is position specific or not*/


static Nlm_FloatHi BLspecialSmithWatermanScoreOnly(Uint1 * matchSeq, 
   Int4 matchSeqLength, Uint1 *query, Int4 queryLength, BLAST_Score **matrix, 
   Int4 gapOpen, Int4 gapExtend,  Int4 *matchSeqEnd, Int4 *queryEnd, Int4 *score,
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

/*computes where optimal Smith-Waterman local alignment starts given the
  ending positions
  matchSeq is the matchSeq sequence
  matchSeqLength is the length of matchSeq in amino acids
  query is the sequence corresponding to some matrix profile
  queryLength is the length of dbSequnece
  matrix is the position-specific matrix associated with query
  gapOpen is the cost of opening a gap
  gapExtend is the cost of extending an existing gap by 1 position
  matchSeqEnd is the final position in the matchSeq of an optimal
   local alignment
  queryEnd is the final position in query of an optimal
   local alignment
  matchSeqEnd and queryEnd can be used to run the local alignment in reverse
   to find optimal starting positions
  these are passed back in matchSeqStart and queryStart
  the optimal score is passed in to check when it has
   been reached going backwards
  the score is also returned
  positionSpecific determines whether matrix is position specific or not*/

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

/*The following procedure computes the number of identities in an
 * alignment of query_seq to the matching sequence stored in
 *  SWAlign. The alignment is encoded in gap_info*/
static Int4 SWAlignGetNumIdentical(SWResults *SWAlign, Uint1Ptr query_seq, 
                                   GapXEditBlockPtr gap_info)
{
   Uint1Ptr q, s;  /*hold aligned substrings of the query and the match*/
   GapXEditScriptPtr esp; /*edit script describing the alignment*/
   Int4 num_ident; /*number of identities to return*/
   Int4 i; /*loop index*/

   if (!gap_info)
      return 0;
 
   q = &query_seq[SWAlign->queryStart];
   s = &SWAlign->seq[SWAlign->seqStart];

   num_ident = 0;

   for (esp = gap_info->esp; esp; esp = esp->next) {
      switch (esp->op_type) {
      case GAPALIGN_SUB:
         for (i=0; i<esp->num; i++) {
            if (*q++ == *s++)
               num_ident++;
         }
         break;
      case GAPALIGN_DEL:
         s += esp->num;
         break;
      case GAPALIGN_INS:
         q += esp->num;
         break;
      default: 
         s += esp->num;
         q += esp->num;
         break;
      }
   }
   
   return num_ident;
}

/*converts the list of Smith-Waterman alignments to a corresponding list
  of SeqAlignPtrs. kbp stores parameters for computing the score
  Code is adapted from procedure output_hits of pseed3.c */
static SeqAlignPtr newConvertSWalignsToSeqAligns(SWResults * SWAligns,  
                                                 Uint1Ptr query,
                                                 SeqIdPtr query_id,
                                                 Boolean discontinuous)
{
    SeqAlignPtr seqAlignList =NULL; /*list of SeqAligns to return*/
    SeqAlignPtr nextSeqAlign; /*new one to add*/
    SeqAlignPtr lastSeqAlign = NULL; /*last one on list*/
    SWResults *curSW, *oldSW; /*used to iterate down list*/
    GapXEditBlockPtr nextEditBlock;  /*intermediate structure towards seqlign*/
    Int4 num_ident;

    curSW = SWAligns;
    while (curSW != NULL) {
        nextEditBlock = TracebackToGapXEditBlock(
                     query - 1 + curSW->queryStart,
		     curSW->seq - 1 + curSW->seqStart,
		     curSW->queryEnd - curSW->queryStart, 
                     curSW->seqEnd - curSW->seqStart,
                     curSW->reverseAlignScript, curSW->queryStart, 
		     curSW->seqStart);
        nextEditBlock->discontinuous = discontinuous;
        nextSeqAlign = GapXEditBlockToSeqAlign(nextEditBlock, curSW->subject_id, query_id);
        nextSeqAlign->score = addScoresToSeqAlign(curSW->scoreThisAlign, 
                                                  curSW->eValueThisAlign, curSW->Lambda, curSW->logK);
        

        num_ident = SWAlignGetNumIdentical(curSW, query, nextEditBlock);
    
        if (num_ident > 0)
           MakeBlastScore(&nextSeqAlign->score, "num_ident", 0.0, num_ident);

        nextEditBlock = GapXEditBlockDelete(nextEditBlock);

        if (NULL == seqAlignList)
            seqAlignList = nextSeqAlign;
        else
            lastSeqAlign->next = nextSeqAlign;
        lastSeqAlign = nextSeqAlign;
        oldSW = curSW;
        curSW = curSW->next;

        SeqIdSetFree(oldSW->subject_id);
        MemFree(oldSW);
    }

    return(seqAlignList);
}


/*allocate a score matrix with numPositions positions and return
  the matrix*/
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

/*deallocate a score matrix*/
static void freeScaledMatrix(BLAST_Score **matrix, Int4 numPositions)
{
  int row; /*loop index*/

  for(row = 0; row <= numPositions; row++)
    MemFree(matrix[row]);
  MemFree(matrix);
}

/*allocate a frequency ratio matrix with numPositions positions and return
  the matrix*/
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

/*matrix is a position-specific score matrix with matrixLength positions
  queryProbArray is an array containing the probability of occurrence
  of each residue in the query
  scoreArray is an array of probabilities for each score that is
    to be used as a field in return_sfp
  return_sfp is a the structure to be filled in and returned
  range is the size of scoreArray and is an upper bound on the
   difference between maximum score and minimum score in the matrix
  the routine posfillSfp computes the probability of each score weighted
   by the probability of each query residue and fills those probabilities
   into scoreArray and puts scoreArray as a field in
   that in the structure that is returned
   for indexing convenience the field storing scoreArray points to the
   entry for score 0, so that referring to the -k index corresponds to
   score -k */
static BLAST_ScoreFreqPtr notposfillSfp(BLAST_Score **matrix, Nlm_FloatHi *subjectProbArray,  Nlm_FloatHi *queryProbArray, Nlm_FloatHi *scoreArray,  BLAST_ScoreFreqPtr return_sfp, Int4 range)
{
  Int4 minScore, maxScore; /*observed minimum and maximum scores*/
  Int4 i,j,k; /* indices */
  Nlm_FloatHi onePosFrac; /*1/matrix length as a double*/

  minScore = maxScore = 0;

  for(i = 0; i < PROTEIN_ALPHABET; i++) {
    for(j = 0 ; j < PRO_TRUE_ALPHABET_SIZE; j++) {
      k = charPositions[j];
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
      k = charPositions[j];
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

/*matrix is a position-specific score matrix with matrixLength positions
  subjectProbArray is an array containing the probability of occurrence
  of each residue in the matching sequence often called the subject
  scoreArray is an array of probabilities for each score that is
    to be used as a field in return_sfp
  return_sfp is a the structure to be filled in and returned
  range is the size of scoreArray and is an upper bound on the
   difference between maximum score and minimum score in the matrix
  the routine posfillSfp computes the probability of each score weighted
   by the probability of each query residue and fills those probabilities
   into scoreArray and puts scoreArray as a field in
   that in the structure that is returned
   for indexing convenience the field storing scoreArray points to the
   entry for score 0, so that referring to the -k index corresponds to
   score -k */
static BLAST_ScoreFreqPtr posfillSfp(BLAST_Score **matrix, Int4 matrixLength, Nlm_FloatHi *subjectProbArray, Nlm_FloatHi *scoreArray,  BLAST_ScoreFreqPtr return_sfp, Int4 range)
{
  Int4 minScore, maxScore; /*observed minimum and maximum scores*/
  Int4 i,j,k; /* indices */
  Nlm_FloatHi onePosFrac; /*1/matrix length as a double*/

  minScore = maxScore = 0;

  for(i = 0; i < matrixLength; i++) {
    for(j = 0 ; j < PRO_TRUE_ALPHABET_SIZE; j++) {
      k = charPositions[j];
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
      k = charPositions[j];
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



/*Given a sequence of 'length' amino acid residues, compute the
  probability of each residue and put that in the array resProb*/
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

/*Return the a matrix of the frequency ratios that underlie the
  score matrix being used on this pass. The returned matrix
  is position-specific, so if we are in the first pass, use
  query to convert the 20x20 standard matrix into a position-specific
  variant. matrixName is the name of the underlying 20x20
  score matrix used. numPositions is the length of the query;
  startNumerator is the matrix of frequency ratios as stored
  in posit.h. It needs to be divided by the frequency of the
  second character to get the intended ratio */
static Nlm_FloatHi **getStartFreqRatios(BlastSearchBlkPtr search,
					Uint1Ptr query,
					Char *matrixName, 
					Nlm_FloatHi **startNumerator,
					Int4 numPositions, 
					Boolean positionSpecific)
{
   Nlm_FloatHi ** returnRatios; /*frequency ratios to start investigating each pair*/
   BLAST_ResFreqPtr stdrfp; /* gets standard frequencies in prob field */
   Nlm_FloatHi *standardProb; /*probabilities of each letter*/
   Int4 i,j;
   returnRatios = allocateStartFreqs(numPositions);
   if (positionSpecific) {
     for(i = 0; i < numPositions; i++) {
       for(j = 0; j < PROTEIN_ALPHABET; j++) {
	 if ((0 == strcmp(matrixName,"BLOSUM62")) ||
	     (0 == strcmp(matrixName,"BLOSUM62_20"))) {
	   returnRatios[i][j] = BLOSUM62_FREQRATIOS[query[i]][j];
	 }
	 if (0 == strcmp(matrixName,"BLOSUM62_20A")) {
	   returnRatios[i][j] = 0.9666 * BLOSUM62_FREQRATIOS[query[i]][j];
	 }
	 if (0 == strcmp(matrixName,"BLOSUM62_20B")) {
	   returnRatios[i][j] = 0.9344 * BLOSUM62_FREQRATIOS[query[i]][j];
	 }
	 if (0 == strcmp(matrixName,"BLOSUM45")) {
	   returnRatios[i][j] = BLOSUM45_FREQRATIOS[query[i]][j];
	 }
	 if (0 == strcmp(matrixName,"BLOSUM80")) {
	   returnRatios[i][j] = BLOSUM80_FREQRATIOS[query[i]][j];
	 }
	 if (0 == strcmp(matrixName,"BLOSUM50")) {
	   returnRatios[i][j] = BLOSUM50_FREQRATIOS[query[i]][j];
	 }

	 if (0 == strcmp(matrixName,"BLOSUM90")) {
	   returnRatios[i][j] = BLOSUM90_FREQRATIOS[query[i]][j];
	 }
	 if (0 == strcmp(matrixName,"PAM250")) {
	   returnRatios[i][j] = PAM250_FREQRATIOS[query[i]][j];
	 }
	 if (0 == strcmp(matrixName,"PAM30")) {
	   returnRatios[i][j] = PAM30_FREQRATIOS[query[i]][j];
	 }
	 if (0 == strcmp(matrixName,"PAM70")) {
	   returnRatios[i][j] = PAM70_FREQRATIOS[query[i]][j];
	 }
       }
     }
     stdrfp = BlastResFreqNew(search->sbp);
     BlastResFreqStdComp(search->sbp,stdrfp); 
     standardProb = MemNew(PROTEIN_ALPHABET * sizeof(Nlm_FloatHi));
     for(i = 0; i < PROTEIN_ALPHABET; i++)
       standardProb[i] = stdrfp->prob[i];
     /*reverse multiplication done in posit.c*/
     for(i = 0; i < numPositions; i++) 
       for(j = 0; j < PROTEIN_ALPHABET; j++) 
	 if ((standardProb[query[i]] > posEpsilon) && (standardProb[j] > posEpsilon) &&     
	     (j != StarChar) && (j != Xchar)
	     && (startNumerator[i][j] > posEpsilon))
	   returnRatios[i][j] = startNumerator[i][j]/standardProb[j];
     stdrfp = BlastResFreqDestruct(stdrfp);
     standardProb = MemFree(standardProb);
   }

   else {
     for(i = 0; i < PROTEIN_ALPHABET; i++) {
       for(j = 0; j < PROTEIN_ALPHABET; j++) {
	 if ((0 == strcmp(matrixName,"BLOSUM62")) ||
	     (0 == strcmp(matrixName,"BLOSUM62_20"))) {
	   returnRatios[i][j] = BLOSUM62_FREQRATIOS[i][j];
	 }
	 if (0 == strcmp(matrixName,"BLOSUM62_20A")) {
	   returnRatios[i][j] = 0.9666 * BLOSUM62_FREQRATIOS[i][j];
	 }
	 if (0 == strcmp(matrixName,"BLOSUM62_20B")) {
	   returnRatios[i][j] = 0.9344 * BLOSUM62_FREQRATIOS[i][j];
	 }
	 if (0 == strcmp(matrixName,"BLOSUM45")) {
	   returnRatios[i][j] = BLOSUM45_FREQRATIOS[i][j];
	 }
	 if (0 == strcmp(matrixName,"BLOSUM80")) {
	   returnRatios[i][j] = BLOSUM80_FREQRATIOS[i][j];
	 }
	 if (0 == strcmp(matrixName,"BLOSUM50")) {
	   returnRatios[i][j] = BLOSUM50_FREQRATIOS[i][j];
	 }
	 if (0 == strcmp(matrixName,"BLOSUM90")) {
	   returnRatios[i][j] = BLOSUM90_FREQRATIOS[i][j];
	 }
	 if (0 == strcmp(matrixName,"PAM250")) {
	   returnRatios[i][j] = PAM250_FREQRATIOS[i][j];
	 }
	 if (0 == strcmp(matrixName,"PAM30")) {
	   returnRatios[i][j] = PAM30_FREQRATIOS[i][j];
	 }
	 if (0 == strcmp(matrixName,"PAM70")) {
	   returnRatios[i][j] = PAM70_FREQRATIOS[i][j];
	 }
       }
     }
   }
   return(returnRatios);
}

/************************************************************************
 *take every entry of startFreqRatios that is not corresponding to
 * a score of BLAST_SCORE_MIN and take its log, divide by Lambda and
*multiply  by LambdaRatio then round to the nearest integer and
*put the result in the corresponding entry of matrix.
*startMatrix and matrix have dimensions numPositions X PROTEIN_ALPHABET
************************************************************************/
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

/*SCALING_FACTOR is a multiplicative factor used to get more bits of
 * precision in the integer matrix scores. It cannot be arbitrarily
 * large because we do not want total alignment scores to exceedto
 * -(BLAST_SCORE_MIN) */
#define SCALING_FACTOR 32
/************************************************************************
Compute a scaled up version of the standard matrix encoded by matrix
name. Standard matrices are in half-bit units.
************************************************************************/
static void  computeScaledStandardMatrix(BLAST_Score **matrix, Char *matrixName, Nlm_FloatHi Lambda)
{
   int i,j; /*loop indices*/
   Nlm_FloatHi temp; /*intermediate term in computation*/

   if ((0 == strcmp(matrixName,"BLOSUM62"  )) ||
       (0 == strcmp(matrixName,"BLOSUM62_20"))) {
     for(i = 0; i < PROTEIN_ALPHABET; i++)
       for(j = 0; j < PROTEIN_ALPHABET; j++) {
         if(0.0 == BLOSUM62_FREQRATIOS[i][j])
	   matrix[i][j] = BLAST_SCORE_MIN;
	 else {
	   temp = log(BLOSUM62_FREQRATIOS[i][j])/Lambda;
           matrix[i][j] = Nlm_Nint(temp);
	 }
       }
     return;
   }
   if (0 == strcmp(matrixName,"BLOSUM62_20A")) {
     for(i = 0; i < PROTEIN_ALPHABET; i++)
       for(j = 0; j < PROTEIN_ALPHABET; j++) {
         if(0.0 == BLOSUM62_FREQRATIOS[i][j])
	   matrix[i][j] = BLAST_SCORE_MIN;
	 else {
	   temp =  log(0.9666 * BLOSUM62_FREQRATIOS[i][j])/Lambda;
           matrix[i][j] = Nlm_Nint(temp);
	 }
       }
     return;
   }
   if (0 == strcmp(matrixName,"BLOSUM62_20B")) {
     for(i = 0; i < PROTEIN_ALPHABET; i++)
       for(j = 0; j < PROTEIN_ALPHABET; j++) {
         if(0.0 == BLOSUM62_FREQRATIOS[i][j])
	   matrix[i][j] = BLAST_SCORE_MIN;
	 else {
	   temp = log(0.9344 * BLOSUM62_FREQRATIOS[i][j])/Lambda;
           matrix[i][j] = Nlm_Nint(temp);
	 }
       }
     return;
   }
   if (0 == strcmp(matrixName,"PAM30")) {
     for(i = 0; i < PROTEIN_ALPHABET; i++)
       for(j = 0; j < PROTEIN_ALPHABET; j++) {
         if(0.0 == PAM30_FREQRATIOS[i][j])
	   matrix[i][j] = BLAST_SCORE_MIN;
	 else {
	   temp = log(PAM30_FREQRATIOS[i][j])/Lambda;
           matrix[i][j] = Nlm_Nint(temp);
	 }
       }
     return;
   }
   if (0 == strcmp(matrixName,"PAM70")) {
     for(i = 0; i < PROTEIN_ALPHABET; i++)
       for(j = 0; j < PROTEIN_ALPHABET; j++) {
         if(0.0 == PAM70_FREQRATIOS[i][j])
	   matrix[i][j] = BLAST_SCORE_MIN;
	 else {
	   temp =  log(PAM70_FREQRATIOS[i][j])/Lambda;
           matrix[i][j] = Nlm_Nint(temp);
	 }
       }
     return;
   }
   if (0 == strcmp(matrixName,"BLOSUM45")) {
     for(i = 0; i < PROTEIN_ALPHABET; i++)
       for(j = 0; j < PROTEIN_ALPHABET; j++) {
         if(0.0 == BLOSUM45_FREQRATIOS[i][j])
	   matrix[i][j] = BLAST_SCORE_MIN;
	 else {
	   temp =  log(BLOSUM45_FREQRATIOS[i][j])/Lambda;
           matrix[i][j] = Nlm_Nint(temp);
	 }
       }
     return;
   }
   if (0 == strcmp(matrixName,"BLOSUM80")) {
     for(i = 0; i < PROTEIN_ALPHABET; i++)
       for(j = 0; j < PROTEIN_ALPHABET; j++) {
         if(0.0 == BLOSUM80_FREQRATIOS[i][j])
	   matrix[i][j] = BLAST_SCORE_MIN;
	 else {
	   temp =  log(BLOSUM80_FREQRATIOS[i][j])/Lambda;
           matrix[i][j] = Nlm_Nint(temp);
	 }
       }
     return;
   }
   if (0 == strcmp(matrixName,"BLOSUM50")) {
     for(i = 0; i < PROTEIN_ALPHABET; i++)
       for(j = 0; j < PROTEIN_ALPHABET; j++) {
         if(0.0 == BLOSUM50_FREQRATIOS[i][j])
	   matrix[i][j] = BLAST_SCORE_MIN;
	 else {
	   temp =  log(BLOSUM50_FREQRATIOS[i][j])/Lambda;
           matrix[i][j] = Nlm_Nint(temp);
	 }
       }
     return;
   }
   if (0 == strcmp(matrixName,"BLOSUM90")) {
     for(i = 0; i < PROTEIN_ALPHABET; i++)
       for(j = 0; j < PROTEIN_ALPHABET; j++) {
         if(0.0 == BLOSUM90_FREQRATIOS[i][j])
	   matrix[i][j] = BLAST_SCORE_MIN;
	 else {
	   temp = log(BLOSUM90_FREQRATIOS[i][j])/Lambda;
           matrix[i][j] = Nlm_Nint(temp);
	 }
       }
     return;
   }
   if (0 == strcmp(matrixName,"PAM250")) {
     for(i = 0; i < PROTEIN_ALPHABET; i++)
       for(j = 0; j < PROTEIN_ALPHABET; j++) {
         if(0.0 == PAM250_FREQRATIOS[i][j])
	   matrix[i][j] = BLAST_SCORE_MIN;
	 else {
	   temp =  log(PAM250_FREQRATIOS[i][j])/Lambda;
           matrix[i][j] = Nlm_Nint(temp);
	 }
       }
     return;
   }
   ErrPostEx(SEV_FATAL, 1, 0, "blastpgp: Cannot adjust parameters for matrix %s\n", matrixName);
}

/************************************************************
produce a scaled-up version of the position-specific matrix starting from
posFreqs
fillPosMatrix is the matrix to be filled
nonposMatrix is the underlying position-independent matrix, used to
fill positions where frequencies are irrelevant
sbp stores various parameters of the search
*****************************************************************/
void scalePosMatrix(BLAST_Score **fillPosMatrix, BLAST_Score **nonposMatrix, Char *matrixName, Nlm_FloatHi **posFreqs, Uint1 *query, Int4 queryLength, BLAST_ScoreBlkPtr sbp)
{

     posSearchItems *posSearch; /*used to pass data into scaling routines*/
     compactSearchItems *compactSearch; /*used to pass data into scaling routines*/
     Int4 i,j ; /*loop indices*/   
     BLAST_ResFreqPtr stdrfp; /* gets standard frequencies in prob field */
     Int4 a; /*index over characters*/
     Nlm_FloatHi **standardFreqRatios; /*frequency ratios for standard score matrix*/
     Int4 multiplier; /*bit scale factor for scores*/


     posSearch = (posSearchItems *) MemNew (1 * sizeof(posSearchItems));
     compactSearch = (compactSearchItems *) MemNew (1 * sizeof(compactSearchItems));
     posSearch->posMatrix = (BLAST_Score **) MemNew((queryLength + 1) * sizeof(BLAST_Score *));
     posSearch->posPrivateMatrix = fillPosMatrix;
     posSearch->posFreqs = posFreqs;
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

     standardFreqRatios = (Nlm_FloatHi **) MemNew(PROTEIN_ALPHABET * sizeof(Nlm_FloatHi *));
     for (i = 0; i < PROTEIN_ALPHABET; i++)
       standardFreqRatios[i] = (Nlm_FloatHi *) MemNew(PROTEIN_ALPHABET * sizeof(Nlm_FloatHi));

     if ((0 == strcmp(matrixName,"BLOSUM62")) ||
	 (0 == strcmp(matrixName,"BLOSUM62_20"))) {
       multiplier = 2;
       for(i = 0; i < PROTEIN_ALPHABET; i++)
	 for(j = 0; j < PROTEIN_ALPHABET; j++)
	   standardFreqRatios[i][j] = BLOSUM62_FREQRATIOS[i][j];
     }
     if (0 == strcmp(matrixName,"BLOSUM62_20A")) {
       multiplier = 2;
       for(i = 0; i < PROTEIN_ALPHABET; i++)
	 for(j = 0; j < PROTEIN_ALPHABET; j++)
	   standardFreqRatios[i][j] = 0.9666 * BLOSUM62_FREQRATIOS[i][j];
     }
     if (0 == strcmp(matrixName,"BLOSUM62_20B")) {
       multiplier = 2;
       for(i = 0; i < PROTEIN_ALPHABET; i++)
	 for(j = 0; j < PROTEIN_ALPHABET; j++)
	   standardFreqRatios[i][j] = 0.9344 * BLOSUM62_FREQRATIOS[i][j];
     }
     if (0 == strcmp(matrixName,"BLOSUM45")) {
       multiplier = 3;
       for(i = 0; i < PROTEIN_ALPHABET; i++)
	 for(j = 0; j < PROTEIN_ALPHABET; j++)
	   standardFreqRatios[i][j] = BLOSUM45_FREQRATIOS[i][j];
     }
     if (0 == strcmp(matrixName,"BLOSUM80")) {
       multiplier = 2;
       for(i = 0; i < PROTEIN_ALPHABET; i++)
	 for(j = 0; j < PROTEIN_ALPHABET; j++)
	   standardFreqRatios[i][j] = BLOSUM80_FREQRATIOS[i][j];
     }
     if (0 == strcmp(matrixName,"BLOSUM50")) {
       multiplier = 2;
       for(i = 0; i < PROTEIN_ALPHABET; i++)
	 for(j = 0; j < PROTEIN_ALPHABET; j++)
	   standardFreqRatios[i][j] = BLOSUM50_FREQRATIOS[i][j];
     }
     if (0 == strcmp(matrixName,"BLOSUM90")) {
       multiplier = 2;
       for(i = 0; i < PROTEIN_ALPHABET; i++)
	 for(j = 0; j < PROTEIN_ALPHABET; j++)
	   standardFreqRatios[i][j] = BLOSUM90_FREQRATIOS[i][j];
     }
     if (0 == strcmp(matrixName,"PAM250")) {
       multiplier = 2;
       for(i = 0; i < PROTEIN_ALPHABET; i++)
	 for(j = 0; j < PROTEIN_ALPHABET; j++)
	   standardFreqRatios[i][j] = PAM250_FREQRATIOS[i][j];
     }
     if (0 == strcmp(matrixName,"PAM30")) {
       multiplier = 2;
       for(i = 0; i < PROTEIN_ALPHABET; i++)
	 for(j = 0; j < PROTEIN_ALPHABET; j++)
	   standardFreqRatios[i][j] = PAM30_FREQRATIOS[i][j];
     }
     if (0 == strcmp(matrixName,"PAM70")) {
       multiplier = 2;
       for(i = 0; i < PROTEIN_ALPHABET; i++)
	 for(j = 0; j < PROTEIN_ALPHABET; j++)
	   standardFreqRatios[i][j] = PAM70_FREQRATIOS[i][j];
     }

     posFreqsToMatrix(posSearch,compactSearch, standardFreqRatios, multiplier);
     impalaScaling(posSearch, compactSearch, ((Nlm_FloatHi) SCALING_FACTOR), FALSE);

     for(i = 0; i <= queryLength; i++)
       MemFree(posSearch->posMatrix[i]);
     for(i = 0; i < PROTEIN_ALPHABET; i++)
       MemFree(standardFreqRatios[i]);

     MemFree(standardFreqRatios);
     MemFree(compactSearch->standardProb);
     MemFree(posSearch->posMatrix);
     MemFree(posSearch);
     MemFree(compactSearch);
}


/**
 * A Kappa_MatchingSequence represents a subject sequence to be aligned
 * with the query.  This abstract sequence is used to hide the
 * complexity associated with actually obtaining and releasing the
 * data for a matching sequence, e.g. reading the sequence from a DB
 * or translating it from a nucleotide sequence. */
struct Kappa_MatchingSequence {
  Int4      length;             /* length of the sequence */
  Uint1Ptr  sequence;           /* the sequence data */
  Uint1Ptr  filteredSequence;   /* a copy of the sequence data that
                                 * has been filtered */
  Uint1Ptr  filteredSequenceStart;      /* the address of the chunk of
                                           memory that has been
                                           allocated to hold
                                           "filterSequence". */
  BioseqPtr bsp_db;              /* info used to access the database */
  Boolean   bioseq_needs_unlock; /* true if the sequence need to be unlocked
                                    when it is released */
};
typedef struct Kappa_MatchingSequence Kappa_MatchingSequence;


#define BLASTP_MASK_RESIDUE 21
#define BLASTP_MASK_INSTRUCTIONS "S 10 1.8 2.1"

/* Initialize a new matching sequence, obtaining the data from an
 * appropriate location */
static void
Kappa_MatchingSequenceInitialize(Kappa_MatchingSequence * self,
                                 BlastSearchBlkPtr search,
                                 Int4 subject_id)
{
  if(search->rdfp) {
    Uint1Ptr db_sequence;       /* the raw sequence from the database */
    Int4     db_length;         /* the length of "db_sequence" */

    search->rdfp->parameters |= READDB_KEEP_HDR_AND_SEQ;

    db_length =
      readdb_get_sequence(search->rdfp, subject_id,
                          (Uint1Ptr PNTR) & db_sequence);
    self->bsp_db   = readdb_get_bioseq(search->rdfp, subject_id);
    self->sequence = db_sequence;
    self->length   = db_length;

    self->bioseq_needs_unlock = FALSE;
  } else {
    SeqPortPtr spp = NULL;      /* a SeqPort used to read the sequence data */
    Uint1      residue;         /* an individual residue */
    Int4       idx;

    self->length   = search->subject_info->length;
    self->sequence = MemNew((self->length + 1) * sizeof(Uint1));
    self->bsp_db   = BioseqLockById(search->subject_info->sip);
    spp =
      SeqPortNew(self->bsp_db, FIRST_RESIDUE, LAST_RESIDUE,
                 Seq_strand_unknown, Seq_code_ncbistdaa);

    idx = 0;
    while((residue = SeqPortGetResidue(spp)) != SEQPORT_EOF) {
      if(IS_residue(residue)) {
        if(residue == 24) {     /* change selenocysteine to X */
          residue = 21;
          fprintf(stderr, "Selenocysteine (U) at position %ld replaced by X\n",
                  (long) idx + 1);
        }
        self->sequence[idx++] = residue;
      }
    }
    self->sequence[idx] = 0;    /* terminate the string */
    spp = SeqPortFree(spp);

    self->bioseq_needs_unlock = TRUE;
  }
  self->filteredSequenceStart = MemNew((self->length + 2) * sizeof(Uint1));
  self->filteredSequence      = self->filteredSequenceStart + 1;

#ifndef KAPPA_NO_SEG_SEQUENCE
  segResult(self->bsp_db, self->sequence, self->filteredSequence,
            self->length, BLASTP_MASK_INSTRUCTIONS, BLASTP_MASK_RESIDUE);
#endif
}


/* Release the data associated with a matching sequence */
static void
Kappa_MatchingSequenceRelease(Kappa_MatchingSequence * self)
{
  if(self->sequence != self->filteredSequence) {
    MemFree(self->filteredSequenceStart);
  }
  if(self->bsp_db) {
    if(self->bioseq_needs_unlock) {
      BioseqUnlock(self->bsp_db);
      self->bsp_db   = NULL;
      self->sequence = MemFree(self->sequence);
    } else {
      self->bsp_db   = BioseqFree(self->bsp_db);
    }
  }
}


/* An instance of Kappa_ForbiddenRanges is used by the Smith-Waterman
 * algorithm to represent ranges in the database that are not to be
 * aligned.
 */

struct Kappa_ForbiddenRanges { Int4 *numForbidden; /* how many
  forbidden ranges at each db * position */
  Int4 **ranges;                /* forbidden ranges for each database
                                 * position */
  Int4   queryLength;
};
typedef struct Kappa_ForbiddenRanges Kappa_ForbiddenRanges;


/* Initialize a new, empty Kappa_ForbiddenRanges */
static void
Kappa_ForbiddenRangesInitialize(
  Kappa_ForbiddenRanges * self, /* object to be initialized */
  Int4 queryLength              /* the length of the query */
) {
  Int4 f;
  self->queryLength  = queryLength;
  self->numForbidden = (Int4 *) MemNew(queryLength * sizeof(Int4));
  self->ranges       = (Int4 **) MemNew(queryLength * sizeof(Int4 *));

  for(f = 0; f < queryLength; f++) {
    self->numForbidden[f] = 0;
    self->ranges[f]       = (Int4 *) MemNew(2 * sizeof(Int4));
    self->ranges[f][0]    = 0;
    self->ranges[f][1]    = 0;
  }
}


/* Reset self to be empty */
static void
Kappa_ForbiddenRangesClear(Kappa_ForbiddenRanges * self)
{
  Int4 f;
  for(f = 0; f < self->queryLength; f++) {
    self->numForbidden[f] = 0;
  }
}


/* Add some ranges to self */
static void
Kappa_ForbiddenRangesPush(
  Kappa_ForbiddenRanges * self,
  Int4 queryStart,      /* start of the alignment in the query sequence */
  Int4 queryAlignmentExtent,   /* length of the alignment in the query sequence */
  Int4 matchStart,      /* start of the alignment in the subject sequence */
  Int4 matchAlignmentExtent)   /* length of the alignment in the subject  sequence */
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
}


/* Release the storage associated with the fields of self, but do not 
 * delete self */
static void
Kappa_ForbiddenRangesRelease(Kappa_ForbiddenRanges * self)
{
  Int4 f;
  for(f = 0; f < self->queryLength; f++)  MemFree(self->ranges[f]);
  
  MemFree(self->ranges);       self->ranges       = NULL;
  MemFree(self->numForbidden); self->numForbidden = NULL;
}


/* Redo a S-W alignment using an x-drop alignment.  The result will
 * usually be the same as the S-W alignment. The call to ALIGN
 * attempts to force the endpoints of the alignment to match the
 * optimal endpoints determined by the Smith-Waterman algorithm.
 * ALIGN is used, so that if the data structures for storing BLAST
 * alignments are changed, the code will not break */
static void
Kappa_SWFindFinalEndsUsingXdrop(
  Uint1Ptr query,       /* the query sequence */
  Int4 queryLength,     /* length of the query sequence */
  Int4 queryStart,      /* start of the alignment in the query sequence */
  Int4 queryEnd,        /* end of the alignment in the query sequence,
                           as computed by the Smith-Waterman algorithm */
  Uint1Ptr match,       /* the subject (database) sequence */
  Int4 matchLength,     /* length of the subject sequence */
  Int4 matchStart,      /* start of the alignment in the subject sequence */
  Int4 matchEnd,        /* end of the alignment in the query sequence,
                           as computed by the Smith-Waterman algorithm */
  GapAlignBlkPtr gap_align,     /* parameters for a gapped alignment */
  Int4 score,           /* score computed by the Smith-Waterman algorithm */
  Nlm_FloatHi localScalingFactor,       /* the factor by which the
                                         * scoring system has been
                                         * scaled in order to obtain
                                         * greater precision */
  Int4 * queryAlignmentExtent, /* length of the alignment in the query sequence,
                           as computed by the x-drop algorithm */
  Int4 * matchAlignmentExtent, /* length of the alignment in the subject sequence,
                           as computed by the x-drop algorithm */
  Int4 ** reverseAlignScript,   /* alignment information (script)
                                 * returned by a x-drop alignment algorithm */
  BLAST_Score * newScore        /* alignment score computed by the
                                   x-drop algorithm */
) {
  Int4 XdropAlignScore;         /* alignment score obtained using X-dropoff
                                 * method rather than Smith-Waterman */
  Int4 doublingCount = 0;       /* number of times X-dropoff had to be
                                 * doubled */
  do {
    Int4 *alignScript;          /* the alignment script that will be
                                   generated below by the ALIGN
                                   routine. */
    
    *reverseAlignScript = alignScript =
      (Int4 *) MemNew((matchLength + queryLength + 3) * sizeof(Int4));

    XdropAlignScore =
      ALIGN(&(query[queryStart]) - 1, &(match[matchStart]) - 1,
            queryEnd - queryStart + 1, matchEnd - matchStart + 1,
            *reverseAlignScript, queryAlignmentExtent, matchAlignmentExtent, &alignScript,
            gap_align, queryStart - 1, FALSE);

    gap_align->x_parameter *= 2;
    doublingCount++;
    if((XdropAlignScore < score) && (doublingCount < 3)) {
      MemFree(*reverseAlignScript);
    }
  } while((XdropAlignScore < score) && (doublingCount < 3));

  *newScore = Nlm_Nint(((Nlm_FloatHi) XdropAlignScore) / localScalingFactor);
}


/* A Kappa_SearchParameters represents the data needed by
 * RedoAlignmentCore to adjust the parameters of a search, including
 * the original value of these parameters */
struct Kappa_SearchParameters {
  Int4          gapOpen;        /* a penalty for the existence of a gap */
  Int4          gapExtend;      /* a penalty for each residue (or nucleotide) 
                                 * in the gap */
  Int4          gapDecline;     /* a penalty for declining to align a pair of 
                                 * residues */
  Int4          mRows, nCols;   /* the number of rows an columns in a scoring 
                                 * matrix */
  Nlm_FloatHi   scaledUngappedLambda;   /* The value of Karlin-Altchul
                                         * parameter lambda, rescaled
                                         * to allow scores to have
                                         * greater precision */
  BLAST_Score **startMatrix, **origMatrix;
  Nlm_FloatHi **startFreqRatios;        /* frequency ratios to start
                                         * investigating each pair */
  Nlm_FloatHi  *scoreArray;      /* array of score probabilities */
  Nlm_FloatHi  *resProb;         /* array of probabilities for each residue in 
                                  * a matching sequence */
  Nlm_FloatHi  *queryProb;       /* array of probabilities for each residue in 
                                  * the query */
  Boolean       adjustParameters;

  BLAST_ScoreFreqPtr return_sfp;        /* score frequency pointers to
                                         * compute lambda */
  BLAST_KarlinBlkPtr kbp_gap_orig, *orig_kbp_gap_array;
};
typedef struct Kappa_SearchParameters Kappa_SearchParameters;


/* Release the date associated with a Kappa_SearchParameters and
 * delete the object */
static void
Kappa_SearchParametersFree(Kappa_SearchParameters ** searchParams)
{
  /* for convenience, remove one level of indirection from searchParams */
  Kappa_SearchParameters *sp = *searchParams; 

  if(sp->kbp_gap_orig) BlastKarlinBlkDestruct(sp->kbp_gap_orig);

  if(sp->startMatrix)     freeScaledMatrix(sp->startMatrix, sp->mRows);
  if(sp->origMatrix)      freeScaledMatrix(sp->origMatrix, sp->mRows);
  if(sp->startFreqRatios) freeStartFreqs(sp->startFreqRatios, sp->mRows);

  if(sp->return_sfp) MemFree(sp->return_sfp);
  if(sp->scoreArray) MemFree(sp->scoreArray);
  if(sp->resProb)    MemFree(sp->resProb);
  if(sp->queryProb)  MemFree(sp->queryProb);

  Nlm_Free(*searchParams);
  *searchParams = NULL;
}


/* Create a new instance of Kappa_SearchParameters */
static Kappa_SearchParameters *
Kappa_SearchParametersNew(
  Int4 rows,                    /* number of rows in the scoring matrix */
  Boolean adjustParameters,     /* if true, use composition-based statistics */
  Boolean positionBased         /* if true, the search is position-based */
) {
  Kappa_SearchParameters *sp;   /* the new object */
  sp = Nlm_Malloc(sizeof(Kappa_SearchParameters));

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
    sp->startMatrix  = allocateScaledMatrix(rows);
    sp->origMatrix   = allocateScaledMatrix(rows);
    
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


/* Record the initial value of the search parameters that are to be
 * adjusted. */
static void
Kappa_RecordInitialSearch(Kappa_SearchParameters * searchParams,
                          BlastSearchBlkPtr search)
{
  Uint1Ptr query;               /* the query sequence */
  Int4 queryLength;             /* the length of the query sequence */

  query      = search->context[0].query->sequence;
  queryLength    = search->context[0].query->length;

  if(searchParams->adjustParameters) {
    Int4 i, j;
    BLAST_KarlinBlkPtr kbp;     /* statistical parameters used to evaluate a
                                 * query-subject pair */
    BLAST_Score **matrix;       /* matrix used to score a local
                                   query-subject alignment */

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

    for(i = 0; i < searchParams->mRows; i++) {
      for(j = 0; j < PROTEIN_ALPHABET; j++) {
        searchParams->origMatrix[i][j] = matrix[i][j];
      }
    }
  }
}


/* Rescale the search parameters in the search object and options object to
   obtain more precision. */
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
      computeScaledStandardMatrix(sp->startMatrix, options->matrix,
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


/*LambdaRatioLowerBound is used when the expected score is too large
 * causing impalaKarlinLambdaNR to give a Lambda estimate that
 * is too small, or to fail entirely returning -1*/
#define LambdaRatioLowerBound 0.5

/* Adjust the search parameters */
static Int4
Kappa_AdjustSearch(
  Kappa_SearchParameters * sp,  /* a record of the initial search parameters */
  BlastSearchBlkPtr search,     /* the object to be adjusted */
  Uint1Ptr filteredSequence,    /* a filtered subject sequence */
  Int4 length,                  /* length of the filtered sequence */
  BLAST_Score ** matrix         /* a scoring matrix to be adjusted */
) {   

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
    fillResidueProbability(filteredSequence, length, sp->resProb);

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


/* Restore the parameters that were adjusted to their original values */
static void
Kappa_RestoreSearch(
  Kappa_SearchParameters * searchParams,        /* a record of the
                                                   original values */
  BlastSearchBlkPtr search,     /* the search to be restored */
  BLAST_OptionsBlkPtr options,  /* the option block to be restored */
  BLAST_Score ** matrix,        /* the scoring matrix to be restored */
  Boolean SmithWaterman /* if true, we have performed a Smith-Waterman
                           alignment with these search parameters */
) {
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


/************************************************************************
 *  Top level routine to recompute alignments for each
 *  match found by the gapped BLAST algorithm
 *  search is the structure with all the information about the search
 *  options is used to pass certain command line options taken in by BLAST
 *  hitlist_count is the number of old matches
 *  adjustParameters determines whether we are to adjust the Karlin-Altschul
 *  parameters and score matrix
 *  SmithWaterman determines whether the new local alignments should be
 *  computed by the optimal Smith-Waterman algorithm; SmithWaterman false
 *  means that alignments will be recomputed by the current X-drop
 *  algorithm as implemented in the procedure ALIGN.
 *  It is assumed that at least one of adjustParameters and SmithWaterman
 *  is true when this procedure is called
 *  A linked list of alignments is returned; the alignments are sorted
 *  according to the lowest E-value of the best alignment for each
 *  matching sequence; alignments for the same matching sequence
 *  are in the list consecutively regardless of the E-value of the
 *  secondary alignments. Ties in sorted order are much rarer than
 *  for the standard BLAST method, but are broken deterministically
 *  based on the index of the matching sequences in the database.
 *  
 ************************************************************************/

SeqAlignPtr
RedoAlignmentCore(BlastSearchBlkPtr search,
                  BLAST_OptionsBlkPtr options,
                  Int4 hitlist_count,
                  Boolean adjustParameters,
                  Boolean SmithWaterman)
{

  Int4 index;                   /* index over matches */
  SeqAlignPtr results = NULL;   /* list of seqAligns to return */
  Uint1Ptr    query;            /* the query sequence */
  Int4        queryLength;      /* the length of the query sequence */
  Nlm_FloatHi localScalingFactor;       /* the factor by which to
                                         * scale the scoring system in
                                         * order to obtain greater
                                         * precision */

  BLAST_Score      **matrix;    /* score matrix */
  BLAST_KarlinBlkPtr kbp;       /* stores Karlin-Altschul parameters */
  GapAlignBlkPtr     gap_align; /* keeps track of gapped alignment params */

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

  SWheapInitialize(&significantMatches, options->hitlist_size,
                   options->hitlist_size, options->ethresh);

  /**** Validate parameters *************/
  if(0 == strcmp(options->matrix, "BLOSUM62_20") && !adjustParameters) {
    return 0;                   /* BLOSUM62_20 only makes sense if
                                 * adjustParameters is on */
  }
  /*****************/
  query       = search->context[0].query->sequence;
  queryLength = search->context[0].query->length;

  if(SmithWaterman) {
    Kappa_ForbiddenRangesInitialize(&forbidden, queryLength);
  }

  /* Set up Gap align */
  if(search->gap_align == NULL) {
    search->gap_align = GapAlignBlkNew(1, 1);
  }
  gap_align                = search->gap_align;
  gap_align->matrix        = NULL;
  gap_align->positionBased = search->positionBased;
  if(search->positionBased) {
    gap_align->posMatrix   = search->sbp->posMatrix;
    gap_align->matrix      = search->sbp->matrix;
  } else {
    gap_align->matrix      = search->sbp->matrix;
  }
  gap_align->gap_open      = search->pbp->gap_open;
  gap_align->gap_extend    = search->pbp->gap_extend;
  gap_align->decline_align = search->pbp->decline_align;

  if(search->positionBased) {
    kbp    = search->sbp->kbp_gap_psi[0];
    matrix = search->sbp->posMatrix;
    if(search->sbp->posFreqs == NULL) {
      search->sbp->posFreqs = allocatePosFreqs(queryLength, PROTEIN_ALPHABET);
    }
    } else {
    kbp    = search->sbp->kbp_gap_std[0];
    matrix = search->sbp->matrix;
  }

  /* Initialize searchParams */
  searchParams =
    Kappa_SearchParametersNew(queryLength, adjustParameters,
                              search->positionBased);
  Kappa_RecordInitialSearch(searchParams, search);

  localScalingFactor = Kappa_RescaleSearch(searchParams, search, options);

  for(index = 0; index < hitlist_count; index++) {
    /* for all matching sequences */
    Kappa_MatchingSequence matchingSeq; /* the data for a matching
                                         * database sequence */

    BLASTResultHitlistPtr thisMatch = search->result_struct->results[index];
    if(thisMatch->hsp_array == NULL) {
      continue;
    }

    if(SWheapWillAcceptOnlyBelowCutoff(&significantMatches)) {
      /* Only matches with evalue <= options->ethresh will be saved */

      /* e-value for a sequence is the smallest e-value among the hsps
       * matching a region of the sequence to the query */
      Nlm_FloatHi minEvalue = thisMatch->best_evalue;
      if(minEvalue > (EVALUE_STRETCH * options->ethresh)) {
        /* This match is likely to have an evalue > options->ethresh
         * and therefore, we assume that all other matches with higher
         * input evalues are also unlikely to get sufficient
         * improvement in a redone alignment */
        break;
      }
    }
    /* Get the sequence for this match */
    Kappa_MatchingSequenceInitialize(&matchingSeq, search,
                                     thisMatch->subject_id);

    if(0 == Kappa_AdjustSearch(searchParams, search,
                         matchingSeq.filteredSequence,
                         matchingSeq.length, matrix)) {
      /* Kappa_AdjustSearch ran without error. Compute the new alignments. */
      if(SmithWaterman) {
        /* We are performing a Smith-Waterman alignment */
        Nlm_FloatHi newSwEvalue; /* the evalue computed by the SW algorithm */
        BLAST_Score aSwScore;    /* a score computed by the SW algorithm */
        Int4 matchStart, queryStart;    /* Start positions of a local
                                         * S-W alignment */
        Int4 queryEnd, matchEnd;        /* End positions of a local
                                         * S-W alignment */

        Kappa_ForbiddenRangesClear(&forbidden);

        newSwEvalue =
          BLbasicSmithWatermanScoreOnly(matchingSeq.filteredSequence,
                                        matchingSeq.length, query,
                                        queryLength, matrix,
                                        gap_align->gap_open,
                                        gap_align->gap_extend, &matchEnd,
                                        &queryEnd, &aSwScore, kbp,
                                        search->searchsp_eff,
                                        search->positionBased);

        if(newSwEvalue <= search->pbp->cutoff_e &&
           SWheapWouldInsert(&significantMatches, newSwEvalue ) ) {
          /* The initial local alignment is significant. Continue the
           * computation */
          Kappa_MatchRecord aSwMatch;   /* the newly computed
                                         * alignments of the query to
                                         * the current database
                                         * sequence */
        
          Kappa_MatchRecordInitialize(&aSwMatch, newSwEvalue, aSwScore,
                                      matchingSeq.sequence,
                                      thisMatch->subject_id,
                                      matchingSeq.bsp_db->id);

          BLSmithWatermanFindStart(matchingSeq.filteredSequence,
                                   matchingSeq.length, query, queryLength,
                                   matrix, gap_align->gap_open,
                                   gap_align->gap_extend, matchEnd, queryEnd,
                                   aSwScore, &matchStart, &queryStart,
                                   search->positionBased);

          do {
            /* score computed by an x-drop alignment (usually the same
             * as aSwScore */
            BLAST_Score newXdropScore;  
            /* Lengths of the alignment  as recomputed by an x-drop alignment,
               in the query and the match*/
            Int4 queryAlignmentExtent, matchAlignmentExtent;
            /* Alignment information (script) returned by a x-drop
             * alignment algorithm */
            Int4 *reverseAlignScript;   

            gap_align->x_parameter =
              options->gap_x_dropoff_final * NCBIMATH_LN2 / kbp->Lambda;

            Kappa_SWFindFinalEndsUsingXdrop(query, queryLength, queryStart,
                                            queryEnd,
                                            matchingSeq.filteredSequence,
                                            matchingSeq.length, matchStart,
                                            matchEnd, gap_align, aSwScore,
                                            localScalingFactor,
                                            &queryAlignmentExtent, &matchAlignmentExtent,
                                            &reverseAlignScript,
                                            &newXdropScore);

            Kappa_MatchRecordInsertSwAlign(&aSwMatch, newXdropScore,
                                           newSwEvalue, kbp->Lambda,
                                           kbp->logK, localScalingFactor,
                                           matchStart, matchAlignmentExtent,
                                           queryStart, queryAlignmentExtent,
                                           reverseAlignScript);

            Kappa_ForbiddenRangesPush(&forbidden, queryStart, queryAlignmentExtent,
                                      matchStart, matchAlignmentExtent);
            if(thisMatch->hspcnt > 1) {
              /* There are more HSPs */
              newSwEvalue =
                BLspecialSmithWatermanScoreOnly(matchingSeq.filteredSequence,
                                                matchingSeq.length, query,
                                                queryLength, matrix,
                                                gap_align->gap_open,
                                                gap_align->gap_extend,
                                                &matchEnd, &queryEnd,
                                                &aSwScore, kbp,
                                                search->searchsp_eff,
                                                forbidden.numForbidden,
                                                forbidden.ranges,
                                                search->positionBased);

              if(newSwEvalue <= search->pbp->cutoff_e) {
                /* The next local alignment is significant */
                BLspecialSmithWatermanFindStart(matchingSeq.filteredSequence,
                                                matchingSeq.length, query,
                                                queryLength, matrix,
                                                gap_align->gap_open,
                                                gap_align->gap_extend,
                                                matchEnd, queryEnd, aSwScore,
                                                &matchStart, &queryStart,
                                                forbidden.numForbidden,
                                                forbidden.ranges,
                                                search->positionBased);
              }
              /* end if the next local alignment is significant */
            }
            /* end if there are more HSPs */
          } while(thisMatch->hspcnt > 1 &&
                  newSwEvalue <= search->pbp->cutoff_e);
          /* end do..while there are more HSPs and the next local alignment
           * is significant */

          SWheapInsert(&significantMatches, &aSwMatch);
        }
        /* end if the initial local alignment is significant */
      } else {
        /* We are not doing a Smith-Waterman alignment */
        SeqAlignPtr seqAligns;  /* returned list of realign seqAligns for
                                 * non-SW case */

        search->pbp->gap_x_dropoff_final =
          options->gap_x_dropoff_final * NCBIMATH_LN2 / kbp->Lambda;
        /* recall that index is the counter corresponding to
         * thisMatch; by aliasing, thisMatch will get updated during
         * the following call to BlastGetGapAlgnTbck, so that
         * thisMatch stores newly computed alignments between the
         * query and the matching sequence number index */
        seqAligns =
          BlastGetGapAlgnTbck(search, index, FALSE, FALSE,
                              matchingSeq.filteredSequence,
                              matchingSeq.length, NULL, 0);
        if(seqAligns) {
          /* There are alignments of the query to this matching sequence */
          Nlm_FloatHi bestEvalue = thisMatch->best_evalue;

          if(bestEvalue <= search->pbp->cutoff_e &&
             SWheapWouldInsert(&significantMatches, bestEvalue ) ) {
            /* The best alignment is significant */
            Int4 alignIndex;            /* Iteration index */
            Int4 numNewAlignments;      /* the number of alignments
                                         * just computed */
            Kappa_MatchRecord matchRecord;      /* the newly computed
                                                 * alignments of the
                                                 * query to the
                                                 * current database
                                                 * sequence */
            Nlm_FloatHi bestScore;      /* the score of the highest
                                         * scoring alignment */
            numNewAlignments = thisMatch->hspcnt;  
            bestScore =
              Nlm_Nint(((Nlm_FloatHi) thisMatch->hsp_array[0].score) /
                       localScalingFactor);

            Kappa_MatchRecordInitialize(&matchRecord, bestEvalue, bestScore,
                                        matchingSeq.sequence,
                                        thisMatch->subject_id,
                                        matchingSeq.bsp_db->id);


            for(alignIndex = 0; alignIndex < numNewAlignments; alignIndex++) {
              /* for all alignments of this matching sequence */
              SeqAlignPtr seqAlign;     /* head of the list of seqAligns */
              seqAlign             = seqAligns;
              seqAligns            = seqAligns->next;
              seqAlign->next       = NULL;

              Kappa_MatchRecordInsertSeqAlign(&matchRecord, seqAlign,
                                              &thisMatch->
                                              hsp_array[alignIndex],
                                              kbp->Lambda, kbp->logK,
                                              localScalingFactor);
            }
            /* end for all alignments of this matching sequence */
            SWheapInsert(&significantMatches, &matchRecord);
          }
          /* end if the best alignment is significant */
        }
        /* end there are alignments of the query to this matching sequence */
      }
      /* end else we are not doing a Smith-Waterman alignment */
    }
    /* end if Kappa_AdjustSearch ran without error */
    Kappa_MatchingSequenceRelease(&matchingSeq);
  }
  /* end for all matching sequences */
  {
    SWResults *SWAligns;        /* All new alignments, concatenated
                                   into a single, flat list */
    SWAligns = SWheapToFlatList(&significantMatches);
    if(SWAligns != NULL) {
      if(SmithWaterman) {
        results =
          newConvertSWalignsToSeqAligns(SWAligns, query, search->query_id,
                                        search->pbp->discontinuous);
      } else {
        results = concatenateListOfSeqAligns(SWAligns);
      }
    }
  }
  /* Clean up */
  SWheapRelease(&significantMatches);

  if(SmithWaterman) 
    Kappa_ForbiddenRangesRelease(&forbidden);
  Kappa_RestoreSearch(searchParams, search, options, matrix, SmithWaterman);

  Kappa_SearchParametersFree(&searchParams);

  return (results);
}
