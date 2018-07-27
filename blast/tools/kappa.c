/* $Id: kappa.c,v 6.20 2001/12/28 18:02:33 dondosha Exp $ 
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

Author: Alejandro Schaffer

Contents: Utilities for doing Smith-Waterman alignments and adjusting
    the scoring system for each match in blastpgp

 $Revision: 6.20 $

 $Log: kappa.c,v $
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

typedef struct SWheapRecord {
        Nlm_FloatHi bestEvalue;
        SWResults * theseAlignments;
      } SWheapRecord;

/*Following dat type is used to describe the state of keeping track of
  matches when coverting results from X-dropoff alignments to
  Smith-Waterman alignments. Since computing a Smith-Waterman alignment
  is time consuming, we wish to avoid doing so if the alignment
  will not be used either in the output or in the subsequent
  computation of a position specific score matrix.
  SWFewMatches -- there are few matches and all will be used, so
                  S-W alignments should be computed for all
  SWAllBelowThresh -- the number of matches whose E-value is below
                      the E_value threshold for use in the PSI-BLAST model
                      exceeds the number of matches that will be displayed
                      so only those matches whose E-value may end up
                      below the PSI-BLAST threshold should be converted to
                      S-W alignments
  SWPurging -- the number of matches exceeds the number to display, but
               the number of matches below the PSI-BLAST E-value threshold
               has so far not exceeded the number to display.
*/

typedef enum {
  SWFewMatches, SWPurging, SWAllBelowThresh, SWStopAligning
} SWMatchState;



/*Compare two locations in the heap.  */
static Boolean SWheapCompare(SWheapRecord *place1, SWheapRecord *place2)
{
  return((place1->bestEvalue > place2->bestEvalue));
}

/*swapping function used to swap entries of an SWheap*/
static void SWheapSwap(SWheapRecord *heapArray, Int4 i, Int4 j)
{
  SWheapRecord tempRecord;

  tempRecord.bestEvalue = heapArray[i].bestEvalue;
  tempRecord.theseAlignments = heapArray[i].theseAlignments;
  heapArray[i].bestEvalue = heapArray[j].bestEvalue;
  heapArray[i].theseAlignments = heapArray[j].theseAlignments;
  heapArray[j].bestEvalue = tempRecord.bestEvalue;
  heapArray[j].theseAlignments = tempRecord.theseAlignments;
}

/*The usual Heapify function, tailored for SWheaps*/
static void SWHeapify(SWheapRecord *heapArray, Int4 i, Int4 n)
{
  Boolean moreswap = TRUE; /*is more swapping needed*/
  Int4 left,right,largest; /*placeholders for indices in swapping*/
  do {
    left = 2 *i;
    right = 2*i + 1;
    if ((left <= n) &&	(SWheapCompare(&(heapArray[left]),&(heapArray[i]))))
      largest = left;
    else
      largest = i;
    if ((right <= n) && (SWheapCompare(&(heapArray[right]),&(heapArray[largest]))))
      largest = right;
    if (largest != i){
      SWheapSwap(heapArray, i, largest);     
      /*push largest up the heap*/    
      i = largest;            /*check next level down*/
    }
    else
      moreswap = FALSE;
  } while(moreswap);/*function builds the heap*/
}

static void BuildSWHeap(SWheapRecord *HeapArray, Int4 actualSize)
{
  Int4 i;
  for(i=(actualSize/2); i>=1; --i)
    SWHeapify(HeapArray, i, actualSize);
}


static Nlm_FloatHi SWHeapReportMaxE(SWheapRecord *HeapArray)
{
  return(HeapArray[1].bestEvalue);
}

static Nlm_FloatHi SWHeapReplaceItem(SWResults * OneMatchAligns, SWheapRecord *HeapArray, Int4 actualSize)
{
  Nlm_FloatHi currentMaxE; /*currentMax in heap*/
  SWResults *oldAlignments, *thisAlignment;

  currentMaxE = SWHeapReportMaxE(HeapArray);
  if (OneMatchAligns->eValue >= currentMaxE) {
    oldAlignments = OneMatchAligns;
  }
  else {
    oldAlignments = HeapArray[1].theseAlignments;
    HeapArray[1].bestEvalue = OneMatchAligns->eValue;
    HeapArray[1].theseAlignments = OneMatchAligns;
    SWHeapify(HeapArray, 1, actualSize);
    currentMaxE = SWHeapReportMaxE(HeapArray);
  }
  while(oldAlignments != NULL) {
    MemFree(oldAlignments->reverseAlignScript);
    thisAlignment = oldAlignments;
    oldAlignments = oldAlignments->next;
    SeqIdSetFree(thisAlignment->subject_id);

    MemFree(thisAlignment);
  }
  return(currentMaxE);
}

/*SWAligns is a linked list of alignments; all alignments with the
  same match are consecutive. convertListToHeap assigns one entry
  in the heap to *all* the alignments associated with a single match
  and attaches to this heap entry the best Evalue among those single-match
  alignments. numMatches is the number of hep entries.
  The minimum */
static Nlm_FloatHi convertListToHeap(SWResults * SWAligns, SWheapRecord *HeapArray, Int4 numMatches)
{
  Int4 i; /*loop index*/
  SWResults *curAlignment, *nextAlignment; /*placeholders*/

  curAlignment = SWAligns;
  nextAlignment = curAlignment->next;
  HeapArray[1].theseAlignments = curAlignment;
  HeapArray[1].bestEvalue = curAlignment->eValue;
  while((nextAlignment != NULL) &&
	(!(nextAlignment->isFirstAlignment))) {
    curAlignment = nextAlignment;
    nextAlignment = nextAlignment->next;
  }
  curAlignment->next = NULL;
  for(i = 2; i <= numMatches; i++) {
    if (NULL == nextAlignment) {
      ErrPostEx(SEV_FATAL, 0, 0, "blast: nextAlignment NULL for index %d in convertListToHeap\n", i);
    }
    HeapArray[i].theseAlignments = nextAlignment;
    HeapArray[i].bestEvalue = nextAlignment->eValue;
    curAlignment = nextAlignment;
    nextAlignment = curAlignment->next;
    while((nextAlignment != NULL) &&
	  (!(nextAlignment->isFirstAlignment))) {
      curAlignment = nextAlignment;
      nextAlignment = nextAlignment->next;
    }
    curAlignment->next=NULL;
  }    
  BuildSWHeap(HeapArray, numMatches);
  return(SWHeapReportMaxE(HeapArray));
}

/*Each entry in HeapArray has a list of alignments; convert the array
  back into a single linked-list of alignments, the total number of
  alignments is also passed back in numAligns*/
static SWResults *convertFromHeapToList(SWheapRecord *HeapArray, Int4 ActualSize, Int4 *numAligns)
{
  SWResults *returnAlignList; /*list of alignment records to return*/
  SWResults *alignsFromNextMatch; /*list of alignments for next ehap entry*/
  SWResults *lastAlignment;      /*last Alignment in return list*/
  SWResults *curAlignment;  /*placeholder for loops*/
  Int4 i; /*loop index*/

  returnAlignList = HeapArray[1].theseAlignments;
  (*numAligns) = 0;
  curAlignment = returnAlignList;
  while(curAlignment != NULL) {
    lastAlignment = curAlignment;
    (*numAligns)++;
    curAlignment = curAlignment->next;
  }
  for(i = 2; i <= ActualSize; i++) {
    lastAlignment->next = HeapArray[i].theseAlignments;
    curAlignment = HeapArray[i].theseAlignments;
    while(curAlignment != NULL) {
      lastAlignment = curAlignment;
      (*numAligns)++;
      curAlignment = curAlignment->next;
    }
  }

  return(returnAlignList);
}


/*take as input an amino acid  string and its length; compute a filtered
  amino acid string and return the filtered string*/
void segResult(BioseqPtr bsp, Uint1 * inputString, Uint1 * resultString, Int4 length)
{
  SeqLocPtr seg_slp;  /*pointer to structure for seg filtering*/
  Int4 i; /*loop index*/
  Uint1 *buf, *bu; /*buffers for copying*/
  Uint1 *tmp; /*loop index for copying*/

   if (bsp->length != length)
   {
   	ErrPostEx(SEV_FATAL, 0, 0, "segResult: length and bsp->length are different");
	return;
   }

    seg_slp = BlastBioseqFilter(bsp,"S 10 1.8 2.1");

    for (i = 0; i < length; i++)
      resultString[i] = inputString[i];

    if (seg_slp) {
        BlastMaskTheResidues(resultString,length,21,seg_slp,FALSE, 0);
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
  gapExtend is the cost of extending an exisiting gap by 1 position
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
  gapExtend is the cost of extending an exisiting gap by 1 position
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
  gapExtend is the cost of extending an exisiting gap by 1 position
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
  gapExtend is the cost of extending an exisiting gap by 1 position
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
        
        nextEditBlock = GapXEditBlockDelete(nextEditBlock);
        
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


/*allocate a score matrix with numPositionbs positions and return
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

/*allocate a frequency ratio matrix with numPositionbs positions and return
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
  query to convert the 20x20 standard matrix into a posiion-specific
  variant. matrixname is the name of the underlying 20x20
  score matrix used. numPositions is the length of the query;
  startNumerator is the amtrix of frequency ratios as stored
  in posit.c. It needs to be divided by the frequency of the
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

#define scalingFactor 32
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
   ErrPostEx(SEV_FATAL, 0, 0, "blastpgp: Cannot adjust parameters for matrix %s\n", matrixName);
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
     impalaScaling(posSearch, compactSearch, ((Nlm_FloatHi) scalingFactor), FALSE);

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

#define LambdaRatioLowerBound 0.5

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
  
SeqAlignPtr RedoAlignmentCore(BlastSearchBlkPtr search,
     BLAST_OptionsBlkPtr options,
     Int4 hitlist_count,  Boolean adjustParameters, 
     Boolean SmithWaterman)
{

   Int4 index; /*index over matches*/
   Int4 return_hitlist_count; /*updated hitlist_count to pass back*/
   SeqAlignPtr	results=NULL; /*list of seqAligns to return*/
   BLASTResultHitlistPtr  thisMatch; /*stores local alignments for
                                             one matching sequence*/
   Boolean terminationTest; /*are we done reevaluating thisMatch*/
   Int4 hspIndex; /*index over hsp's for 1 match*/
   BLASTResultHsp thisHSP; /*One HSP for thisMatch*/
   Uint1Ptr query; /*the query sequence*/
   Int4 queryLength; /*the length of the query sequence*/
   Uint1Ptr matchingSequence; /*one sequence that matches the query*/
   Uint1Ptr  filteredMatchingSequence; /*other representations of matchingSequence*/
   Int4 matchingSequenceLength; /*length of matching sequence*/
   Int4 matchStart, queryStart; /*starting positions for local alignment*/
   Int4 queryEnd, matchEnd, finalQueryEnd, finalMatchEnd; /*end positions of optimal local alignment*/
   BLAST_KarlinBlkPtr kbp; /*stores Karlin-Altschul parameters*/
   BLAST_KarlinBlkPtr holdkbp; /*holds Karlin-Altschul parameters from before*/
   Nlm_FloatHi LambdaRatio; /*ratio of ungapped lambdas for correcting matrix*/
   Nlm_FloatHi correctUngappedLambda; /*new value of ungapped lambda*/
   Nlm_FloatHi initialUngappedLambda;   /*ungappedLambda to start*/
   Nlm_FloatHi scaledInitialUngappedLambda; /*iniitialUngappedLambda divided by
					      scalingFactor*/
   SWResults *SWAligns=NULL, *newSW;  /*keeps list of matching alignments*/
   SWResults *lastAlign; /*keeps last alignment on list*/
   SWResults *thisAlignment, *firstAlignmentThisMatch; /*loop index, head of list for one match*/
   SWResults *OneMatchSWAligns; /*keeps sequence alignments for a single match,
				  used when in state SWPurging*/
   Nlm_FloatHi bestEvalue, newEvalue; /*Evalue for best and newest Smith_Waterman alignment*/
   Int4 bestScore, newScore; /*scores for best and newest alignment*/
   BLAST_Score score; /*score of optimal local alignment*/
   Int4 *alignScript, *reverseAlignScript; 
   /*edit script that describes pairwise alignment*/
   Int4 XdropAlignScore; /*alignment score obtained using
			   X-dropoff method rather than Smith-Waterman*/
   Int4 doublingCount; /*number of times X-dropoff had to be doubled*/
   Int4 *numForbidden; /*how many forbidden ranges at each db position*/
   Int4 **forbiddenRanges; /*forbidden ranges for each database position*/
   Int4 f; /* index for numForbidden and forbiddenRanges*/
   Int4 tempIndex; /*index used to copy over rows of forbiddenRanges*/
   Int4 *tempForbidden; /*used to copy over rows of forbiddenRanges*/
   Boolean foundFirstAlignment; /*found at least 1 alignment for this
                                  query/match pair*/
   Boolean doThis=FALSE;  /*should we proceed to test this match*/
   Boolean skipThis = FALSE; /*should we skip this match due to
			       overlapping alignment*/
   BLAST_Score **matrix; /*score matrix*/
   BLAST_Score **startMatrix; /*score matrix to start investigating each pair*/
   Nlm_FloatHi ** startFreqRatios; /*frequency ratios to start investigating each pair*/
   BLAST_Score **holdMatrix; /*matrix to hold what was originally passed in*/
   Int4 gapOpen, gapExtend, gapDecline; /*costs for opening and extending a gap*/
   Nlm_FloatHi localScalingFactor; /*scaling factor for scoring system*/
   GapAlignBlkPtr gap_align; /*keeps track of gapped alignment parameters*/
   Nlm_FloatHi *resProb; /*array of probabilities for each residue in a matching sequence*/
   Nlm_FloatHi *queryProb; /*array of probabilities for each residue in the query*/
   BLAST_ScoreFreqPtr this_sfp, return_sfp; /*score frequency pointers to
                                              compute lambda*/
   Nlm_FloatHi *scoreArray; /*array of score probabilities*/
   Int4 i,j; /*loop indices over characters*/
   Int4 numAligns = 0; /*number of local alignments found*/
   Int4 numMatches = 0; /*number of matching sequences reported*/
   SWMatchState currentState; /*state of whether we have seen enough
                                matches or not*/
   Nlm_FloatHi minEvalue; /*minimum initial Evalue for a match*/
   Nlm_FloatHi worstEvalue = 0; /*highest EValue for a match while in state
			      SWFewMatches*/
   Nlm_FloatHi maxEInHeap; /*maximum Evalue amongst all entries in the heap*/
   SWheapRecord *HeapArray = NULL; /*array for heap*/
   Int4 heapSize; /*number of entries in the heap*/
   BioseqPtr bsp_db; /* Bioseq for a database sequence, used for filtering. */

   query = search->context[0].query->sequence;
   queryLength = search->context[0].query->length;
   gapOpen = search->pbp->gap_open;
   gapExtend = search->pbp->gap_extend;
   gapDecline = search->pbp->decline_align;

   if (adjustParameters)
     localScalingFactor = scalingFactor;
   else
     localScalingFactor = 1.0;
   if ((0 == strcmp(options->matrix,"BLOSUM62_20")))
     localScalingFactor = localScalingFactor/10;

   if (search->gap_align == NULL) {
       search->gap_align = GapAlignBlkNew(1, 1);
   }

   gap_align = search->gap_align;

   gap_align->gap_open = Nlm_Nint(gapOpen * localScalingFactor);
   gap_align->gap_extend =  Nlm_Nint(gapExtend * localScalingFactor);
   /*gap_align->decline_align = (-(BLAST_SCORE_MIN)); */
   if (gapDecline != INT2_MAX)
     gap_align->decline_align = Nlm_Nint(gapDecline *localScalingFactor);
   else
     gap_align->decline_align = gapDecline;
   gap_align->matrix = NULL;
   gap_align->positionBased = search->positionBased;
   if (search->positionBased) {
       matrix = gap_align->posMatrix = gap_align->matrix = search->sbp->posMatrix;
       kbp = search->sbp->kbp_gap_psi[0];
       
       if(search->sbp->posFreqs == NULL) {
           search->sbp->posFreqs = allocatePosFreqs(queryLength, PROTEIN_ALPHABET);
       }
   }
   else {
     matrix = gap_align->matrix = search->sbp->matrix;
     kbp = search->sbp->kbp_gap_std[0];
   }
   if (adjustParameters) {
     holdMatrix = matrix;
     holdkbp = kbp;
   }
   if (SmithWaterman) {
     numForbidden = (Int4 *) MemNew(queryLength * sizeof(Int4));
     forbiddenRanges = (Int4 **) MemNew(queryLength * sizeof(Int4 *));
     for (f = 0; f < queryLength; f++) {
       numForbidden[f] = 0;
       forbiddenRanges[f] = (Int4 *) MemNew(2 * sizeof(Int4));
       forbiddenRanges[f][0] = 0;
       forbiddenRanges[f][1] = 0;
     }
   }
   if (adjustParameters) {
     if (search->positionBased) {
       matrix = allocateScaledMatrix(queryLength);
       startMatrix = allocateScaledMatrix(queryLength);
       startFreqRatios = getStartFreqRatios(search, query, options->matrix, 
					search->sbp->posFreqs,
					queryLength, TRUE); 
       scalePosMatrix(matrix,  search->sbp->matrix, options->matrix, search->sbp->posFreqs,                    query,  queryLength, search->sbp);
       for(i = 0; i < queryLength; i++)
	 for(j = 0; j < PROTEIN_ALPHABET; j++)
	   startMatrix[i][j] = matrix[i][j];
       initialUngappedLambda = search->sbp->kbp_std[0]->Lambda;
       scaledInitialUngappedLambda = initialUngappedLambda/localScalingFactor;
     }
     else {
       matrix = allocateScaledMatrix(PROTEIN_ALPHABET);
       startMatrix = allocateScaledMatrix(PROTEIN_ALPHABET);
       startFreqRatios = getStartFreqRatios(search, query,options->matrix, 
					NULL,PROTEIN_ALPHABET,FALSE);
       initialUngappedLambda = search->sbp->kbp_psi[0]->Lambda;
       scaledInitialUngappedLambda = initialUngappedLambda/localScalingFactor;
       computeScaledStandardMatrix(matrix,options->matrix, scaledInitialUngappedLambda);
       for(i = 0; i < PROTEIN_ALPHABET; i++)
	 for(j = 0; j < PROTEIN_ALPHABET; j++)
	   startMatrix[i][j] = matrix[i][j];
     }
     scaledInitialUngappedLambda = initialUngappedLambda/localScalingFactor;
     /*set up Karlin-Altschul parameters and put in place here*/
     kbp =  BlastKarlinBlkCreate();
     kbp->Lambda = holdkbp->Lambda / localScalingFactor;
     kbp->K = holdkbp->K;
     kbp->logK = log(kbp->K);
     kbp->H = holdkbp->H;
     if (search->positionBased) 
       search->sbp->kbp_gap_psi[0] = kbp;
     else 
       search->sbp->kbp_gap_std[0] = kbp;

     resProb = (Nlm_FloatHi *) MemNew (PROTEIN_ALPHABET * sizeof(Nlm_FloatHi));
     if (!(search->positionBased)) {
       queryProb = (Nlm_FloatHi *) MemNew (PROTEIN_ALPHABET * sizeof(Nlm_FloatHi));
       fillResidueProbability(query, queryLength, queryProb);
     }
     scoreArray = (Nlm_FloatHi *) MemNew(scoreRange * sizeof(Nlm_FloatHi));
     return_sfp = (BLAST_ScoreFreqPtr) MemNew(1 * sizeof(BLAST_ScoreFreq));
   }

   index = 0;
   currentState = SWFewMatches;
   while((index < hitlist_count) && (currentState != SWStopAligning)) {
     thisMatch  = search->result_struct->results[index];

     if(search->result_struct->results[index]->hsp_array == NULL) {
         index++;
         continue;
     }

     hspIndex = 0;

     thisHSP = search->result_struct->results[index]->hsp_array[hspIndex];
     /*e-value for a sequence is the smallest e-value among the
       hsps matching a region of the sequence to the query*/
     minEvalue = search->result_struct->results[index]->best_evalue;
     if ((currentState == SWAllBelowThresh) &&
	 (minEvalue > (EVALUE_STRETCH * options->ethresh))) {
       currentState = SWStopAligning;
       break;
     }
     if (search->rdfp) {
       matchingSequenceLength = readdb_get_sequence(search->rdfp, thisMatch->subject_id, (Uint1Ptr PNTR) &matchingSequence);
       bsp_db = readdb_get_bioseq(search->rdfp, thisMatch->subject_id);
     } else {
       matchingSequenceLength = 0;
       matchingSequence = NULL;
       bsp_db = NULL;
     }

     /*filteredMatchingSequence = matchingSequence;*/

     /* switch to this if we want to filter the match*/
     filteredMatchingSequence = MemNew((matchingSequenceLength+1) * sizeof(Uint1));
     segResult(bsp_db, matchingSequence, filteredMatchingSequence, matchingSequenceLength); 
     bsp_db = BioseqFree(bsp_db);
     foundFirstAlignment = FALSE;
     if (adjustParameters) {
       if (search->positionBased) {
         for(i = 0; i < queryLength; i++)
           for(j = 0; j < PROTEIN_ALPHABET; j++) 
	     matrix[i][j] = startMatrix[i][j];
       }
       else {
         for(i = 0; i < PROTEIN_ALPHABET; i++)
           for(j = 0; j < PROTEIN_ALPHABET; j++) 
	     matrix[i][j] = startMatrix[i][j];
       }
     }
     if (SmithWaterman)
       for (f = 0; f < queryLength; f++) 
         numForbidden[f] = 0;
     do {     
       if (foundFirstAlignment && (doThis || (!adjustParameters))) {
         if (SmithWaterman) {
	   newEvalue = BLspecialSmithWatermanScoreOnly(filteredMatchingSequence, 
		       matchingSequenceLength, query, queryLength, matrix, 
                       gap_align->gap_open, gap_align->gap_extend,  &matchEnd, &queryEnd, &score,
                       kbp,  search->searchsp_eff, numForbidden, forbiddenRanges, 
                       search->positionBased);
	 }
	 else {
	   gap_align->x_parameter = options->gap_x_dropoff_final * 
	     NCBIMATH_LN2/kbp->Lambda;
	   alignScript = (Int4 *) MemNew((matchingSequenceLength + queryLength + 3) * sizeof(Int4));
	   reverseAlignScript = alignScript;
           /*identify queryStart, matchStart, lengths, finalQueryEnd,
               finalMatchEnd*/
           queryStart = thisHSP.query_offset;
	   queryEnd = queryStart + thisHSP.query_length -1;
           matchStart = thisHSP.subject_offset;
	   matchEnd = matchStart + thisHSP.subject_length -1;
	   if (Xchar == filteredMatchingSequence[matchStart]) {
	     filteredMatchingSequence[matchStart] = matchingSequence[matchStart];
	     filteredMatchingSequence[matchStart+1] = matchingSequence[matchStart+1];
	   }
	   skipThis = FALSE;
           XdropAlignScore = ALIGN(&(query[queryStart]) - 1, 
			   &(filteredMatchingSequence[matchStart]) -1, 
			   queryEnd - queryStart + 1,
			   matchEnd - matchStart + 1,
			   reverseAlignScript , &finalQueryEnd, 
			   &finalMatchEnd, &alignScript,
			   gap_align, queryStart - 1, FALSE); 
           if (0 == reverseAlignScript[0]) {
	     newEvalue = BlastKarlinStoE_simple(XdropAlignScore,kbp, 
						search->searchsp_eff);
             newScore = Nlm_Nint(((Nlm_FloatHi) XdropAlignScore) / localScalingFactor);
	   }
	   else
	     skipThis = TRUE;

	   if ((newEvalue > search->pbp->cutoff_e) ||
               (0 != reverseAlignScript[0])) {
	     MemFree(reverseAlignScript);
	   }
	   if (newEvalue < bestEvalue)
	     bestEvalue = newEvalue;
	   if (newScore > bestScore)
	     bestScore = newScore;
	 }
       }
       else {
         if (adjustParameters) {
           /*compute and plug in new matrix here*/
	   fillResidueProbability(filteredMatchingSequence, matchingSequenceLength, resProb);

           if (search->positionBased)
	     this_sfp =  posfillSfp(matrix, queryLength, resProb, scoreArray, return_sfp, scoreRange);
           else
	     this_sfp =  notposfillSfp(matrix, resProb, queryProb, scoreArray, return_sfp, scoreRange);
	   correctUngappedLambda = impalaKarlinLambdaNR(this_sfp, scaledInitialUngappedLambda);

	   LambdaRatio = correctUngappedLambda/scaledInitialUngappedLambda;
	   LambdaRatio = MIN(1,LambdaRatio);
	   LambdaRatio = MAX(LambdaRatio,LambdaRatioLowerBound);

	   doThis = FALSE;
	   if (LambdaRatio > 0) {
	     doThis = TRUE;
	     if (search->positionBased)
	       scaleMatrix(matrix,startMatrix,startFreqRatios, queryLength,
			   scaledInitialUngappedLambda,LambdaRatio);
	     else
	       scaleMatrix(matrix,startMatrix,startFreqRatios,PROTEIN_ALPHABET,
			   scaledInitialUngappedLambda,LambdaRatio);
	     if (search->positionBased) 
	       gap_align->posMatrix = gap_align->matrix = search->sbp->posMatrix = matrix;
	     else 
	       gap_align->matrix = search->sbp->matrix = matrix;
	   }
	   else {
	     doThis = FALSE;
             bestEvalue = newEvalue = 2 *search->pbp->cutoff_e;
	   }
	 }
	 if (doThis || (!adjustParameters)) {
	   if (SmithWaterman) {
	     newEvalue = BLbasicSmithWatermanScoreOnly(filteredMatchingSequence, 
						     matchingSequenceLength, query, queryLength, matrix, 
						     gap_align->gap_open, gap_align->gap_extend,  &matchEnd, &queryEnd, &score,
						     kbp, search->searchsp_eff, search->positionBased);
	   }
	   else {
	     gap_align->x_parameter = options->gap_x_dropoff_final * 
	       NCBIMATH_LN2/kbp->Lambda;
	     alignScript = (Int4 *) MemNew((matchingSequenceLength + queryLength + 3) * sizeof(Int4));
	     reverseAlignScript = alignScript;
	     /*identify queryStart, matchStart, lengths, finalQueryEnd,
               finalMatchEnd*/
	     queryStart = thisHSP.query_offset;
	     queryEnd = queryStart + thisHSP.query_length -1;
	     matchStart = thisHSP.subject_offset;
	     matchEnd = matchStart + thisHSP.subject_length -1;
	     if (Xchar == filteredMatchingSequence[matchStart]) {
	       filteredMatchingSequence[matchStart] = matchingSequence[matchStart];
	       filteredMatchingSequence[matchStart+1] = matchingSequence[matchStart+1];
	     }
	     skipThis = FALSE;
	     XdropAlignScore = ALIGN(&(query[queryStart]) - 1, 
		     &(filteredMatchingSequence[matchStart]) -1, 
			   queryEnd - queryStart + 1,
			   matchEnd - matchStart + 1,
			   reverseAlignScript , &finalQueryEnd, 
			   &finalMatchEnd, &alignScript,
			   gap_align, queryStart - 1, FALSE); 
	     newEvalue = BlastKarlinStoE_simple(XdropAlignScore,kbp, 
					      search->searchsp_eff);
	     if (0 == reverseAlignScript[0]) {
	       newEvalue = BlastKarlinStoE_simple(XdropAlignScore,kbp, 
						  search->searchsp_eff);
               newScore = Nlm_Nint(((Nlm_FloatHi) XdropAlignScore) / localScalingFactor);
	     }
	     else
	       skipThis = TRUE;

	     if ((newEvalue > search->pbp->cutoff_e) ||
		 (0 != reverseAlignScript[0])) {
	       MemFree(reverseAlignScript);
	     }
           }
	   bestEvalue = newEvalue;
	   bestScore = newScore;
	 }
       }

       if (newEvalue <= search->pbp->cutoff_e && (!skipThis)) {
	 if (SmithWaterman) {
	   if (foundFirstAlignment)
	     BLspecialSmithWatermanFindStart(filteredMatchingSequence, 
					     matchingSequenceLength, query, queryLength, matrix, 
					     gap_align->gap_open, gap_align->gap_extend,  matchEnd, queryEnd, score,
					     &matchStart, &queryStart, numForbidden, forbiddenRanges, 
					     search->positionBased);
	   else
	     BLSmithWatermanFindStart(filteredMatchingSequence, 
				      matchingSequenceLength, query, queryLength, matrix, 
				      gap_align->gap_open, gap_align->gap_extend,  matchEnd, queryEnd, score,
				      &matchStart, &queryStart, search->positionBased);
	   gap_align->x_parameter = options->gap_x_dropoff_final * 
	     NCBIMATH_LN2/kbp->Lambda;
	   doublingCount = 0;
	   do {
	   
	     alignScript = (Int4 *) MemNew((matchingSequenceLength + queryLength + 3) * sizeof(Int4));
	     reverseAlignScript = alignScript;
	     skipThis = FALSE;
	     XdropAlignScore = ALIGN(&(query[queryStart]) - 1, 
				   &(filteredMatchingSequence[matchStart]) -1, 
				   queryEnd - queryStart + 1,
				   matchEnd - matchStart + 1,
				   reverseAlignScript , &finalQueryEnd, 
				   &finalMatchEnd, &alignScript,
				   gap_align, queryStart - 1, FALSE); 
	     gap_align->x_parameter *=2;
	     doublingCount++;
	     if ((XdropAlignScore < score) && (doublingCount < 3)) 
	       MemFree(reverseAlignScript);

	   } while ((XdropAlignScore < score) && (doublingCount < 3));
	 }
	 newSW = (SWResults *) MemNew(1 * sizeof(SWResults)); 
	 newSW->seq = matchingSequence;
	 newSW->seqStart = matchStart;
	 newSW->seqEnd = matchStart + finalMatchEnd;
	 newSW->queryStart = queryStart;
	 newSW->queryEnd = queryStart + finalQueryEnd;
	 newSW->reverseAlignScript = reverseAlignScript;
	 newSW->score = bestScore;
	 newSW->scoreThisAlign = newScore;
	 newSW->eValue = bestEvalue;
	 newSW->eValueThisAlign = newEvalue;
	 newSW->isFirstAlignment = (!foundFirstAlignment);
	 newSW->Lambda = kbp-> Lambda * localScalingFactor;
	 newSW->logK = kbp->logK;
	 newSW->subject_index = thisMatch->subject_id;
	 readdb_get_descriptor(search->rdfp, thisMatch->subject_id, 
			       &(newSW->subject_id), NULL);
	 if (SWPurging != currentState) {
	   numAligns++;
	   if (!foundFirstAlignment) {
	     numMatches++;
	     firstAlignmentThisMatch = newSW;
	   }
	   if (newSW->eValue > worstEvalue)
	     worstEvalue = newSW->eValue;
	   newSW->next = NULL;
	   if (NULL == SWAligns) {
	     SWAligns = newSW;
	     lastAlign = newSW;
	   }
	   else {
	     lastAlign->next = newSW;
	     lastAlign = lastAlign->next;
	   }
	 }
	 else {   /*SWPurging is current state*/
	   if (!foundFirstAlignment) {
	     OneMatchSWAligns = newSW;
	     firstAlignmentThisMatch = newSW;
	     lastAlign = newSW;
	     lastAlign->next = NULL;
	   }
	   else {
	     lastAlign->next = newSW;
	     lastAlign = lastAlign->next;
	     lastAlign->next = NULL;
	   }
	 }
	 /*alignments may not be in order of Evalue*/
	 if (foundFirstAlignment && (!SmithWaterman))
	   for(thisAlignment = firstAlignmentThisMatch; 
	       thisAlignment != NULL; thisAlignment = thisAlignment->next)
	     thisAlignment->eValue = bestEvalue;
	 foundFirstAlignment = TRUE;
	 if (SmithWaterman && ((SWPurging != currentState) || (bestEvalue < maxEInHeap))) {
	   for(f = queryStart; f < (queryStart+ finalQueryEnd); f++) {
	     if (0 == numForbidden[f] ) {
	       numForbidden[f] = 1;
	       forbiddenRanges[f][0] = matchStart;
	       forbiddenRanges[f][1] = matchStart + finalMatchEnd - 1;
	     }
	     else {
	       tempForbidden = (Int4*) MemNew(2 * (numForbidden[f] + 1) * sizeof(Int4));
	       for(tempIndex = 0; tempIndex < (2 * numForbidden[f]); tempIndex++)
		 tempForbidden[tempIndex] = forbiddenRanges[f][tempIndex];
	       tempForbidden[tempIndex] = matchStart;
	       tempForbidden[tempIndex + 1] = matchStart + finalMatchEnd;
	       numForbidden[f]++;
	       MemFree(forbiddenRanges[f]);
	       forbiddenRanges[f] = tempForbidden;
	     }
	   }    
	 }
       }  /*end E test*/
       terminationTest = FALSE;
       if (SmithWaterman) {
	 /*continue to look for multiple matches only if there is > 1
	   hsp*/
	 terminationTest = ((newEvalue <= search->pbp->cutoff_e) && 
			    (thisMatch->hspcnt > 1)
	    && ((currentState != SWPurging) || (bestEvalue < maxEInHeap)));
       }
       else {
	 hspIndex++;
	 if (hspIndex < thisMatch->hspcnt)
	   thisHSP = search->result_struct->results[index]->hsp_array[hspIndex];
	 terminationTest = ((newEvalue <= search->pbp->cutoff_e) && 
			    (hspIndex < thisMatch->hspcnt)
	    && ((currentState != SWPurging) || (bestEvalue < maxEInHeap)));
       }
     } while (terminationTest);
     if ((bestEvalue <= search->pbp->cutoff_e) && (!skipThis) && (doThis)) {
       if (SWFewMatches == currentState) {
	 /*reached maximum number of hits and this is not the last hit*/
	 if ((numMatches >= options->hitlist_size) && (index < (hitlist_count -1))) {
	   if (worstEvalue < options->ethresh) 
	     currentState = SWAllBelowThresh;
	   else {
	     currentState = SWPurging;
	     heapSize = numMatches;
	     HeapArray = (SWheapRecord *) MemNew((numMatches + 1) * sizeof(SWheapRecord));
	     maxEInHeap = convertListToHeap(SWAligns, HeapArray, heapSize);
	   }
	 }
       }
       else {
	 if (SWPurging == currentState) {
	   maxEInHeap = SWHeapReplaceItem(OneMatchSWAligns, HeapArray,heapSize);
	   if (maxEInHeap < options->ethresh){
	     SWAligns = convertFromHeapToList(HeapArray, heapSize, &numAligns);
	     lastAlign = SWAligns;
	     /*restore value of lastAlign*/
	     while(lastAlign->next != NULL)
	       lastAlign =lastAlign->next;
	     currentState = SWAllBelowThresh;
	   }
	 }
       }
     }
     index++;
     if (matchingSequence != filteredMatchingSequence)
       MemFree(filteredMatchingSequence);
   }
   if (SWPurging == currentState)
     SWAligns = convertFromHeapToList(HeapArray, heapSize, &numAligns);
   if (SWAligns != NULL) {
     pro_quicksort_hits(numAligns, &SWAligns);
     results = newConvertSWalignsToSeqAligns(SWAligns, query, search->query_id,
                                             search->pbp->discontinuous);
   }
   if (HeapArray != NULL)
     MemFree(HeapArray);
   if (SmithWaterman) {
     for (f = 0; f < queryLength; f++) 
       MemFree(forbiddenRanges[f]);
     MemFree(forbiddenRanges);
     MemFree(numForbidden);
   }
   if (adjustParameters) {
     if (search->positionBased) {
       search->sbp->posMatrix = holdMatrix;
       search->sbp->kbp_gap_psi[0] = holdkbp;
       freeScaledMatrix(matrix,queryLength);
       freeScaledMatrix(startMatrix,queryLength);
       freeStartFreqs(startFreqRatios,queryLength);
     }
     else {
       search->sbp->matrix = holdMatrix;
       search->sbp->kbp_gap_std[0] = holdkbp;
       freeScaledMatrix(matrix,PROTEIN_ALPHABET);
       freeScaledMatrix(startMatrix,PROTEIN_ALPHABET);
       freeStartFreqs(startFreqRatios,PROTEIN_ALPHABET);
       MemFree(queryProb);
     }
     MemFree(kbp);
     MemFree(resProb);
     MemFree(scoreArray);
     MemFree(return_sfp);
   }
   return(results);
}


