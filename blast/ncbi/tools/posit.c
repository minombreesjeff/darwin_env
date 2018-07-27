static char const rcsid[] = "$Id: posit.c,v 6.72 2004/10/12 15:06:57 papadopo Exp $";

/* $Id: posit.c,v 6.72 2004/10/12 15:06:57 papadopo Exp $
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

   File name: posit.c

  Author: Alejandro Schaffer

  Contents: utilities for position-based BLAST.

  $Revision: 6.72 $ 
 *****************************************************************************

 * $Log: posit.c,v $
 * Revision 6.72  2004/10/12 15:06:57  papadopo
 * 1. Modify residue frequency IO to comply with new scoremat spec
 * 2. Remove check that residue frequencies read from scoremat are <= 1.0
 * 3. Pass gap open and gap extend penalties into BposComputation and
 * 	CposComputation, so that scoremats can contain them
 *
 * Revision 6.71  2004/08/23 17:09:22  papadopo
 * From Michael Gertz: move static arrays out of header and into the one file that needs them
 *
 * Revision 6.70  2004/08/05 17:30:53  camacho
 * Remove initialization of identifier as it is no longer required
 *
 * Revision 6.69  2004/07/24 18:56:01  camacho
 * Fix in posDemographics when GetSequenceWithDenseSeg cannot find sequence data
 *
 * Revision 6.68  2004/07/19 17:13:13  papadopo
 * add capability to perform input and output of residue frequencies in scoremat form; also call PSIMatrixFrequencyRatiosNew before restarting from checkpoint
 *
 * Revision 6.67  2004/07/13 13:54:15  camacho
 * Fix memory leak
 *
 * Revision 6.66  2004/06/25 21:54:51  dondosha
 * Choose ideal values for lambda and K correctly for ungapped search
 *
 * Revision 6.65  2004/06/23 14:53:29  camacho
 * Copy renamed versions of SFreqRatios and its *{New,Free} functions to avoid
 * dependency ncbitool -> blast
 *
 * Revision 6.64  2004/06/22 14:16:46  camacho
 * Changed signature of posFreqsToMatrix, added use of SFreqRatios structure from
 * algo/blast/core/ to obtain underlying matrices' frequency ratios.
 * This change results in using the frequency ratios to provide the scores
 * for the PSSM in columns where all residue frequencies are 0. Previously the
 * standard scoring matrix were used.
 *
 * Revision 6.63  2004/06/08 14:03:48  camacho
 * Alejandro Schaffer's fix to spread out gap costs in posDemographics.
 *
 * Revision 6.62  2004/05/14 12:13:09  camacho
 * Made posDemographics non-static for testing purposes.
 *
 * Revision 6.61  2003/08/04 20:43:55  dondosha
 * Test for selenocysteines when comparing checkpoint sequences with query
 *
 * Revision 6.60  2003/05/30 17:25:37  coulouri
 * add rcsid
 *
 * Revision 6.59  2003/05/13 16:02:53  coulouri
 * make ErrPostEx(SEV_FATAL, ...) exit with nonzero status
 *
 * Revision 6.58  2001/12/11 14:48:54  madden
 * Fix for ABW (reset Xcount to zero in some cases)
 *
 * Revision 6.57  2001/08/29 19:04:48  madden
 * added parameter posComputationCalled to outputPosComputation, extra printing added in revision 6.54 is suppressed if posComputationCalled is FALSE
 *
 * Revision 6.56  2001/08/06 18:09:13  madden
 * Corrected handling of X in posCancel by adding usage of Xcount
 *
 * Revision 6.55  2001/04/09 13:00:09  madden
 * Fixed error in posComputeExtents; adjustment of interval sizes when the query contained an X had been asymmetric.
 *
 * Revision 6.54  2001/04/03 19:38:24  madden
 * Changed IDENTITY_PERCENTAGE to 0.94, Added to output of -Q option in outputPosMatrix
 *
 * Revision 6.53  2001/02/16 16:11:50  dondosha
 * In WposComputation, compute posMatrix from posFreqs if seqalign argument is NULL
 *
 * Revision 6.52  2001/01/03 01:49:38  bauer
 * Changed from static to "LIBCALL":
 *  posAllocateMemory
 *  posPurgeMatches
 *  posCancel
 *  posComputeExtents
 *  posComputeSequenceWeights
 *  posCheckWeights
 *  posComputePseudoFreqs
 *  posScaling
 *
 * Revision 6.51  2000/11/24 22:07:51  shavirin
 * Fixed some memory leaks.
 *
 * Revision 6.50  2000/11/20 14:35:51  madden
 * Changed FileOpen mode for byte-encoded checkpoint files from "r" to "rb" or from "w" to "wb" to solve a problem on Windows NT.
 *
 * Revision 6.49  2000/11/13 14:00:39  madden
 * Added frequency ratios for * in all standard matrices
 *
 * Revision 6.48  2000/11/09 14:27:52  madden
 * psi-blast fixes for star character
 *
 * Revision 6.47  2000/11/01 16:25:57  madden
 * Changes from Futamura for psitblastn
 *
 * Revision 6.46  2000/10/24 16:28:29  madden
 * Changed IDENTITY_RATIO for putging near-identical matches from 0.98 to 0.95
 *
 * Revision 6.45  2000/10/10 21:46:04  shavirin
 * Added support for BLOSUM50, BLOSUM90, PAM250 with -t T
 *
 * Revision 6.44  2000/08/18 21:28:37  madden
 * support for BLOSUM62_20A and BLOSUM62_20B
 *
 * Revision 6.43  2000/07/31 16:41:01  shavirin
 * Reduced POSIT_SCALE_FACTOR from 1000 to 200 to avoid overflow
 * with BLOSUM80; moved declaration os POSIT_SCALE_FACTOR to posit.h
 *
 * Revision 6.42  2000/07/26 13:11:19  shavirin
 * Added magical "LIBCALL" to pacify Windows build function allocatePosFreqs()
 *
 * Revision 6.41  2000/07/25 18:12:04  shavirin
 * WARNING: This is no-turning-back changed related to S&W Blast from
 * Alejandro Schaffer
 *
 * Revision 6.40  2000/05/01 12:48:33  madden
 * changed rules for gaps in processing alignments with -B
 *
 * Revision 6.39  2000/03/02 21:47:07  shavirin
 * Added missing variable for POSIT_DEBUG case
 *
 * Revision 6.38  1999/12/16 19:18:00  egorov
 * Code cleanup
 *
 * Revision 6.37  1999/11/16 21:33:46  shavirin
 * Fixed bug involved posSearch->posResultSequences structure.
 *
 * Revision 6.36  1999/11/16 17:30:41  shavirin
 * Added copying use_best_align parameter in copySearchItems.
 *
 * Revision 6.35  1999/11/15 22:21:09  shavirin
 * Removed nested comments in log space.
 *
 * Revision 6.34  1999/11/15 21:48:31  shavirin
 * Added possibility to take into account best alignments of all
 * alignments (even with e-thresh value larger than threshhold)
 *
 * Revision 6.33  1999/10/21 16:15:04  shavirin
 * Removed unused array and all references to array threshSequences
 *
 * Revision 6.32  1999/09/30 14:15:29  shavirin
 * Fixed bug in the function findThreshSequences().
 *
 * Revision 6.31  1999/09/23 20:58:37  shavirin
 * Fixed some memory leaks.
 *
 * Revision 6.27  1999/09/03 17:24:38  madden
 * Eliminated use of posMaxThresh field in posSearchItems.  Recoded findThreshSequences completely and changed CposComputation so as not to use search->result_struct and thereby eliminate the hidden assumption that search->result_struct and listOfSeqAligns have the matches listed in the same order
 *
 * Revision 6.26  1999/08/04 13:27:10  madden
 * Added -B option
 *
 * Revision 6.25  1999/04/05 14:45:40  madden
 * Fixed format mismatches
 *
 * Revision 6.24  1999/03/21 19:41:51  madden
 * Added 3rd argument matrixfp to outputPosMatrix, Took some of the code in outputPosMatrix outside the #ifdef POSIT_DEBUG for use with -Q option
 *
 * Revision 6.23  1999/01/26 18:27:58  madden
 * Made functions public for AS
 *
 * Revision 6.22  1998/12/09 18:51:51  madden
 * fixed counting bug in posCancel
 *
 * Revision 6.21  1998/09/28 12:31:31  madden
 * Used BlastConstructErrorMessage
 *
 * Revision 6.20  1998/09/09 21:18:33  madden
 * AS fixed warnings
 *
 * Revision 6.19  1998/09/09 16:09:20  madden
 * Changes for PHI-BLAST
 *
 * Revision 6.18  1998/08/26 18:07:00  kans
 * fixed -v -fd warnings (AS)
 *
 * Revision 6.17  1998/06/18 18:20:22  madden
 * Fixed typo in posConvergenceTest
 *
 * Revision 6.16  1998/06/14 19:43:02  madden
 * Added function posFreqsToInformation
 *
 * Revision 6.15  1998/06/12 20:38:48  madden
 * Fix for no hits to build model situation
 *
 * Revision 6.14  1998/06/09 19:38:16  madden
 * Changes rounddown to posit_rounddown to avoid conflict
 *
 * Revision 6.13  1998/04/24 19:29:30  madden
 * Moved rescaling code to blastool.c
 *
 * Revision 6.12  1998/03/25 22:36:17  egorov
 * Change type of posRepeatSequences
 *
 * Revision 6.11  1998/03/23 18:32:30  madden
 * Fix for zero/zero problem
 *
 * Revision 6.10  1998/02/06 18:34:17  madden
 * Added check that residue was not masked in posReadCheckpoint
 *
 * Revision 6.9  1998/02/03 15:57:28  madden
 * Cpos arg in posComputePseudoFreqs set to FALSE for WposComputation call
 *
 * Revision 6.8  1998/01/02 22:19:46  madden
 * Replaced printf by ErrPostEx of SEV_WARNING
 *
 * Revision 6.7  1997/12/23 21:07:06  madden
 * Changes for checkpointing
 *
 * Revision 6.6  1997/12/12 22:14:35  kans
 * changed round to rounddown to avoid CodeWarrior 68K collision
 *
 * Revision 6.5  1997/11/19 15:29:31  madden
 * Changed OS_UNIX ifdef to POSIT_DEBUG
 *
 * Revision 6.4  1997/09/08 13:33:29  madden
 * Added posEpsilon2 to check for small numbers
 *
 * Revision 6.3  1997/09/05 22:29:13  madden
 * Check for zero denominator and replace log(2) by NCBIMATH_LN2
 *
 * Revision 6.2  1997/09/02 22:23:01  madden
 * Removed redundant calls to updateLambdaK
 *
 * Revision 6.1  1997/08/27 21:18:18  madden
 * Fixed problem with deleted matrix
 *
 * Revision 6.0  1997/08/25 18:53:48  madden
 * Revision changed to 6.0
 *
 * Revision 1.22  1997/08/20 21:35:01  madden
 * ALL_ROUNDS replaced by Boolean
 *
 * Revision 1.21  1997/08/11 15:45:24  madden
 * eliminated obsolete fields
 *
 * Revision 1.20  1997/07/28 18:35:06  madden
 * Removed and ifdefed printf
 *
 * Revision 1.19  1997/06/27 19:14:01  madden
 * Fixed two bugs in posComputeSequenceWeights for the special case where all participating sequences are identical in a block
 *
 * Revision 1.17  1997/06/23 14:42:46  madden
 *  Made posComputeSequenceWeights faster by catching the special case where the set of participating sequences does not change from one column to the next.
 *
 * Revision 1.16  1997/05/29 20:35:47  madden
 * Eliminate duplicate sequences and alignments that are 98 perc. identical and ignore columns with all identical sequence weights.
 *
 * Revision 1.15  1997/05/27 20:26:09  madden
 * Fixed problem with matrix
 *
 * Revision 1.14  1997/05/23 20:52:50  madden
 * Fixed bug in setting of matrix for psi-blast
 *
 * Revision 1.13  1997/05/22 21:25:28  madden
 * fixed memory leaks
 *
 * Revision 1.12  1997/05/16 20:56:35  madden
 * replace hard coded numbers by defines
 *
 * Revision 1.11  1997/05/16 20:09:42  madden
 * Fixes for statistical problems
 *
 * Revision 1.10  1997/05/07 21:00:03  madden
 * Call to SeqId2OrdinalId replaces call to readdb_gi2seq
 *
 * Revision 1.9  1997/05/01 15:53:25  madden
 * Addition of extra KarlinBlk's for psi-blast
 *
 * Revision 1.8  1997/04/23  13:31:20  madden
 * Changed diagnostic output.
 *
 * Revision 1.7  1997/04/22  16:36:49  madden
 * Changes for use of psi-blast with www.
 *
 * Revision 1.6  1997/04/10  19:25:53  madden
 * Added casts, COMMAND_LINE replaced by ALL_ROUNDS.
 *
 * Revision 1.5  1997/04/09  20:01:53  madden
 * Functions CposComputation and WposComputation replace posComputations.
 *
 * Revision 1.4  1997/04/04  20:44:55  madden
 * Changed posComputation to return Int4Ptr *.
 *
 * Revision 1.3  1997/03/27  22:30:51  madden
 * Fix for Array Bounds Read.
 *
 * Revision 1.2  1997/03/11  14:38:40  madden
 * Fixes for GI's instead of ordinal numbers.
 *
 * Revision 1.1  1997/02/13  15:22:13  madden
 * Initial revision
 *
*/


#include <ncbi.h>
#include <blastpri.h>
#include <objcode.h>
#include <objseq.h>
#include <objsset.h>
#include <objscoremat.h>
#include <sequtil.h>
#include <posit.h>
#include <txalign.h>

extern BLAST_ScoreFreqPtr BlastScoreFreqNew (BLAST_Score score_min, BLAST_Score score_max);

extern BLAST_ScoreFreqPtr BlastScoreFreqDestruct (BLAST_ScoreFreqPtr sfp);

/*small constants to test against 0*/
#define posEpsilon 0.0001
#define posEpsilon2 0.0000001
/*Representation of a gap in a motif*/
#define GAP_CHAR 0
/*Used inside a seqAlign to reprsent the presence of a gap*/
#define GAP_HERE (-1)
/*Used to check that diagnostics printing routine will work*/
#define EFFECTIVE_ALPHABET 20

#define POSIT_PERCENT 0.05
#define POSIT_NUM_ITERATIONS 10


#define POS_RESTING 0
#define POS_COUNTING 1

#define IDENTITY_RATIO 0.94


/*Allocate memory for  data structures inside posSearch used in
* position-specific caculations
* posSearch -- to be filled in 
* alphabetSize -- number of distinct characters used in the sequences
* querySize -- number of characters in the query sequence
* numSequences -- number of matching sequences potentially in the model */
void LIBCALL posAllocateMemory(posSearchItems * posSearch, 
		       Int4 alphabetSize, Int4 querySize, Int4 numSequences)
{
  Int4 i, j;  /*loop indices*/

  posSearch->posCount = (Int4 *) MemNew(querySize * sizeof(Int4));
  if (NULL == posSearch->posCount)
    exit(EXIT_FAILURE);
  for(i = 0; i < querySize; i++)
    posSearch->posCount[i] = 0;

  posSearch->posC = (Int4 **) MemNew((querySize + 1) * sizeof(Int4 *));
  if (NULL == posSearch->posC)
    exit(EXIT_FAILURE);
  for(i = 0; i <= querySize; i++) {
    posSearch->posC[i] = (Int4 *) MemNew(alphabetSize * sizeof(Int4));
    if (NULL == posSearch->posC[i])
      exit(EXIT_FAILURE);   
    for(j = 0; j < alphabetSize; j++)
      posSearch->posC[i][j] = 0;
 
  }
  posSearch->posGaplessColumnWeights = (Nlm_FloatHi *) MemNew((querySize + 1) * sizeof(Nlm_FloatHi));
  if (NULL == posSearch->posGaplessColumnWeights)
    exit(EXIT_FAILURE);
  posSearch->posMatchWeights = (Nlm_FloatHi **) MemNew((querySize+1) * sizeof(Nlm_FloatHi *));
  if (NULL == posSearch->posMatchWeights)
    exit(EXIT_FAILURE);
  for (i = 0; i <= querySize ; i++) {
    posSearch->posMatchWeights[i] = (Nlm_FloatHi *) MemNew(alphabetSize * sizeof(Nlm_FloatHi));
    if (NULL == posSearch->posMatchWeights[i])
      exit(EXIT_FAILURE);
    for(j = 0; j < alphabetSize; j++) 
      posSearch->posMatchWeights[i][j] = 0.0;
  }  

  posSearch->posMatrix = (BLAST_Score **) MemNew((querySize + 1) * sizeof(BLAST_Score *));
  posSearch->posPrivateMatrix = (BLAST_Score **) MemNew((querySize + 1) * sizeof(BLAST_Score *));
  if (NULL == posSearch->posMatrix)
    exit(EXIT_FAILURE);
  for(i = 0; i <= querySize; i++) {
    posSearch->posMatrix[i] = (BLAST_Score *) MemNew(alphabetSize * sizeof(BLAST_Score));
    posSearch->posPrivateMatrix[i] = (BLAST_Score *) MemNew(alphabetSize * sizeof(BLAST_Score));
    if (NULL == posSearch->posMatrix[i])
      exit(EXIT_FAILURE);   
    for(j = 0; j < alphabetSize; j++)
      posSearch->posMatrix[i][j] = 0;
 
  }

  posSearch->posSigma = (Nlm_FloatHi *) MemNew((querySize) * sizeof(Nlm_FloatHi));
  if (NULL == posSearch->posSigma)
    exit(EXIT_FAILURE);
  for(i = 0; i < querySize; i++) {
    posSearch->posSigma[i] = 0.0;
  }

  posSearch->posIntervalSizes = (Int4 *) MemNew((querySize) * sizeof(Int4));
  if (NULL == posSearch->posIntervalSizes)
    exit(EXIT_FAILURE);
  for(i=0; i < querySize; i++)
    posSearch->posIntervalSizes[i] = 0;

  posSearch->posDescMatrixLength = numSequences;
  posSearch->posDescMatrix = (posDesc **) MemNew((numSequences + 1) * sizeof(posDesc *));
  if (NULL == posSearch->posDescMatrix)
    exit(EXIT_FAILURE);
  for (i = 0; i <= numSequences; i++) {
    posSearch->posDescMatrix[i] = (posDesc *) MemNew(querySize * sizeof(posDesc));
    if (NULL == posSearch->posDescMatrix[i])
      exit(EXIT_FAILURE);
    for(j = 0; j < querySize; j++) {
      posSearch->posDescMatrix[i][j].letter = UNUSED;
      posSearch->posDescMatrix[i][j].used = FALSE;
      posSearch->posDescMatrix[i][j].e_value = 1.0;
      posSearch->posDescMatrix[i][j].leftExtent = -1;
      posSearch->posDescMatrix[i][j].rightExtent = querySize;
    }
  }  
  posSearch->posExtents = (posDesc *) MemNew(querySize * sizeof(posDesc));
  if (NULL == posSearch->posExtents)
    exit(EXIT_FAILURE);
  for(j = 0; j < querySize; j++) {
    posSearch->posExtents[j].used = FALSE;
    posSearch->posExtents[j].leftExtent = -1;
    posSearch->posExtents[j].rightExtent = querySize;
  }
   posSearch->posA = (Nlm_FloatHi *) MemNew((numSequences+ 1) * sizeof(Nlm_FloatHi));
   if (NULL == posSearch->posA)
     exit(EXIT_FAILURE);
   posSearch->posRowSigma = (Nlm_FloatHi *) MemNew((numSequences + 1) * sizeof(Nlm_FloatHi));
   if (NULL == posSearch->posRowSigma)
     exit(EXIT_FAILURE);

   /* populated in posComputePseudoFreqs or on demand */
   posSearch->stdFreqRatios = NULL; 
}

static void freePosFreqs(Nlm_FloatHi ** posFreqs, Int4 length)
{
  Int4 i;

  for (i = 0; i <= length; i++)
    MemFree(posFreqs[i]);
  MemFree(posFreqs); 
}

/*Deallocate memory allocated in posReadCheckpoint
* posSearch -- pointer to record used in building the position-specific model
* querySize -- number of characters in the query sequence
*/
void LIBCALL posCheckpointFreeMemory(posSearchItems *posSearch, Int4 querySize)
{
  Int4 i;  /*loop index*/

  freePosFreqs(posSearch->posFreqs, querySize);
  for(i = 0; i <= querySize; i++){
    MemFree(posSearch->posMatrix[i]);
    MemFree(posSearch->posPrivateMatrix[i]);
  }
  MemFree(posSearch->posMatrix);
  MemFree(posSearch->posPrivateMatrix);
}

/*Deallocate memory allocated in posAllocateMemory
* posSearch -- pointer to record used in building the position-specific model
* querySize -- number of characters in the query sequence
*/
static void posFreeMemory(posSearchItems *posSearch, Int4 querySize)
{
  Int4 i;  /*loop index*/

  MemFree(posSearch->posCount);
  MemFree(posSearch->posExtents);
  MemFree(posSearch->posSigma);

  for(i = 0; i <= querySize; i++){
    MemFree(posSearch->posC[i]);
    MemFree(posSearch->posMatrix[i]);
    MemFree(posSearch->posPrivateMatrix[i]);
    MemFree(posSearch->posMatchWeights[i]);
  }

  MemFree(posSearch->posC);

  for(i = 0; i <= posSearch->posDescMatrixLength; i++)
    MemFree(posSearch->posDescMatrix[i]);

  MemFree(posSearch->posMatrix);
  MemFree(posSearch->posPrivateMatrix);
  MemFree(posSearch->posDescMatrix);
  MemFree(posSearch->posGaplessColumnWeights);
  MemFree(posSearch->posMatchWeights);
  MemFree(posSearch->posA);
  MemFree(posSearch->posRowSigma);
  MemFree(posSearch->posIntervalSizes);
  MemFree(posSearch->posUseSequences);
  posSearch->stdFreqRatios = PSIMatrixFrequencyRatiosFree(posSearch->stdFreqRatios);
  freePosFreqs(posSearch->posFreqs,querySize);
}

/*Cleanup position-specific  data structures after one pass*/
void LIBCALL posCleanup(posSearchItems *posSearch, compactSearchItems * compactSearch)
{
  posFreeMemory(posSearch, compactSearch->qlength);
}


/*extract the e-value that applies to an entire dense
  diagonal alignment from its ScorePtr, based on similar
  code from Tom Madden*/

static Nlm_FloatHi getEvalueFromSeqAlign(SeqAlignPtr thisSeqAlign)
{
  ScorePtr thisScorePtr;

  thisScorePtr = thisSeqAlign->score;
  while ((thisScorePtr != NULL) &&
         (StringICmp(thisScorePtr->id->str, "e_value") != 0) &&
         (StringICmp(thisScorePtr->id->str, "sum_e") != 0))
    thisScorePtr = thisScorePtr->next;
  if(NULL == thisScorePtr)
    return(10.0);
  else
    return((Nlm_FloatHi) (thisScorePtr->value.realvalue));
}

/*Find the lowest e-value among all seqAligns for the sequence represented by
curSeqAlign*/
static Nlm_FloatHi minEvalueForSequence(SeqAlignPtr curSeqAlign, SeqAlignPtr listOfSeqAligns) 
{
   SeqAlignPtr testSeqAlign; /*Index into listOfSeqALigns*/
   DenseSegPtr curSegs, testSegs; /*Used to extract ids from curSeqAlign, testSeqAlign*/
   SeqIdPtr curId, testId; /*Ids of target sequences in testSeqAlign*/
   Nlm_FloatHi  returnValue; /*stores current best e-value*/
   Nlm_FloatHi  testEvalue; /*temporary e-value for one seqAlign*/
   Boolean seen;   /*have we seen a seqAlign matching the sequence yet*/

   returnValue = getEvalueFromSeqAlign(curSeqAlign);
   curSegs = (DenseSegPtr) curSeqAlign->segs;
   curId = curSegs->ids->next; 
   seen = FALSE;

   testSeqAlign = listOfSeqAligns;
   while (NULL != testSeqAlign) {
     testSegs = (DenseSegPtr) testSeqAlign->segs;

     if(testSegs->ids == NULL)
         break;

     testId = testSegs->ids->next; 
     if (SeqIdMatch(curId, testId)) {
         seen = TRUE;
	 if ((testEvalue = getEvalueFromSeqAlign(testSeqAlign)) < returnValue)
	   returnValue = testEvalue;
       }
     else
      /*if we have already seen a match and this one doesn't match,
        then stop looking*/
       if (seen)  
	 break;
     testSeqAlign = testSeqAlign->next;
   }
   return(returnValue);
}


/*Count the number of seqAligns in a list (returned) and count the number of
distinct target sequences represented (passed back in numSequences);
if useThreshold is TRUE, only those sequences with e-values below the threshold are counted.
Important assumption: All SeqAligns with  the same target sequence
are consecutive in the list*/
static Int4 countSeqAligns(SeqAlignPtr listOfSeqAligns, Int4 * numSequences, Boolean useThreshold, Nlm_FloatHi threshold)
{
    SeqAlignPtr curSeqAlign, prevSeqAlign;
    Int4 seqAlignCounter;
    DenseSegPtr curSegs;
    SeqIdPtr curId, prevId; /*Ids of target sequences in current and previous
                              SeqAlign*/
    
    seqAlignCounter = 0;
    *numSequences = 0;
    curSeqAlign = listOfSeqAligns;
    prevSeqAlign = NULL;
    while (NULL != curSeqAlign) {
        curSegs = (DenseSegPtr) curSeqAlign->segs;
        if(curSegs->ids == NULL)
            break;
        curId = curSegs->ids->next; 
        seqAlignCounter++;
        if ((NULL == prevSeqAlign) ||  (!(SeqIdMatch(curId, prevId))))
            if (!useThreshold || (threshold > minEvalueForSequence(curSeqAlign, listOfSeqAligns)))
                (*numSequences)++;
        prevSeqAlign = curSeqAlign;
        prevId = curId;
        curSeqAlign = curSeqAlign->next;
    }
    return(seqAlignCounter);
}

/*Find which sequences that match in the i-th pass have an e-value below
  the specified threshold. These sequences will be used to make the
  score matrix for the next pass*/
static void findThreshSequences(posSearchItems *posSearch, BlastSearchBlkPtr search, SeqAlignPtr listOfSeqAligns, Int4 numalign, Int4 numseq)
{

    Int4 alignIndex; /* indices for sequences and alignments*/
    SeqAlignPtr curSeqAlign, prevSeqAlign; /* pointers into list of seqAligns*/
    DenseSegPtr curSegs;  /*Item in list of seqAligns*/
    SeqIdPtr thisId, prevId; /*Ids of target sequences in current and previous
                               SeqAlign*/  
    Nlm_FloatHi thisEvalue; /*Best E-value for current sequence*/
    Int4 ordinalNumber; /*index of sequence within database*/
    
    /*Allocate boolean array to store values*/
    posSearch->posResultSequences = (Int4 *) MemNew(numseq * sizeof(Int4));
    posSearch->posResultsCounter = 0;

    curSeqAlign = listOfSeqAligns;
    prevSeqAlign = NULL;
    for(alignIndex = 0; alignIndex < numalign; alignIndex++) {
        curSegs = (DenseSegPtr) curSeqAlign->segs;
        thisId = curSegs->ids->next;
        if ((NULL == prevSeqAlign) ||  (!(SeqIdMatch(thisId, prevId)))) {
            thisEvalue = minEvalueForSequence(curSeqAlign, curSeqAlign);
            thisId = curSegs->ids->next;  /*id of target sequence is second*/
            /*Get ordinal ids of sequences in result*/
            ordinalNumber = SeqId2OrdinalId(search->rdfp, thisId);
            if(thisEvalue < (search->pbp->ethresh)) {
                posSearch->posResultSequences[posSearch->posResultsCounter] =
                    ordinalNumber;
                posSearch->posResultsCounter++;
            }
        }
        prevSeqAlign = curSeqAlign;
        prevId = thisId;
        curSeqAlign = curSeqAlign->next;
    }
}



/* Determines if  the search has converged from round to round.
*  Checks whether every new sequence found is in posSearch->posResultSequences.
*  Also sets up posSearch->posRepeatSequences, a boolean array that
*  indicates whether the sequence represented by the i-th new seqAlign is a repeat.
*  This is used in printing the sequences where they are
*  subdivided into two categories: sequences that were found previously
*  and new sequences.
*  posSearch is the data structure representing the parameters of the position-specific part
*  search represents the overall  BLAST search
*  listOfSeqAligns is one representation of the results of the current round.
*  If thissPassNum is 1, then it checks only to see that some sequence
* distinct from the query was found */
void LIBCALL posConvergenceTest(posSearchItems *posSearch, BlastSearchBlkPtr search, SeqAlignPtr listOfSeqAligns, Int4 thisPassNum)
{
  Int4 numseq;   /*Number of sequences found*/
  Int4 numalign; /* Number of items in listOfSeqAligns*/
  Int4 oldSeqIndex; /*Ordinal number of a sequence in old results (previous round)*/
  Int4 alignIndex; /*index into the list of seqAligns*/
  Boolean found;  /*Have we found the new sequence on the old list?*/
  SeqAlignPtr curSeqAlign, prevSeqAlign, startAlign; /* pointers into list of seqAligns*/
  DenseSegPtr curSegs;  /*Item in list of seqAligns*/
  SeqIdPtr thisId, prevId; /*Ids of target sequences in current and previous
			   SeqAlign*/
  Int4 ordinalNumber; /*Ordinal number of a sequence in the database*/
  Nlm_FloatHi thisEvalue; /*lowest evalue from all seqAligns for a sequence*/
  Int4 queryOffset, subjectOffset, retrievalOffset; /*offsets needed
                                                    to make a match align*/
  Int4 qplace, splace; /*index into query string and matching string*/
  Uint1Ptr q,s; /*Pointers into query and matching string*/
  Int4 queryLength; /*length of query*/
  Int4 matchLength; /* length of match*/
  Int4 subjectLength; /* length of a matching string*/
  Int4 c;  /*index into a string*/
  Int4 numsegs; /*number of segments in an alignment*/
  Int4 startQ,startS; /*Indices into array of starting positions*/


  numalign = countSeqAligns(listOfSeqAligns, &numseq, FALSE, 0.0);
  search->posConverged = TRUE;
  curSeqAlign = listOfSeqAligns;
  if (thisPassNum > 1) {
    posSearch->posRepeatSequences = (Int2Ptr) MemNew(numalign * sizeof(Int2));
    prevSeqAlign = NULL;
    for(alignIndex = 0; alignIndex < numalign; alignIndex++) {
      posSearch->posRepeatSequences[alignIndex] = (Int2) 0;
      curSegs = (DenseSegPtr) curSeqAlign->segs;
      thisId = curSegs->ids->next; 
      if ((NULL == prevSeqAlign) ||  (!(SeqIdMatch(thisId, prevId)))) {
	startAlign = curSeqAlign;
	thisEvalue = minEvalueForSequence(curSeqAlign, startAlign);
	if (thisEvalue < search->pbp->ethresh) {    
	  /*Extract the ordinal number from the SeqAlign*/
	  curSegs = (DenseSegPtr) curSeqAlign->segs;
	  thisId = curSegs->ids->next;  /*id of target sequence is second*/
	  /*Get ordinal ids of sequences in result*/
	  ordinalNumber = SeqId2OrdinalId(search->rdfp, thisId);
	  found = FALSE;
	  for(oldSeqIndex = 0; oldSeqIndex < posSearch->posResultsCounter; oldSeqIndex++)
	    if(ordinalNumber ==  posSearch->posResultSequences[oldSeqIndex]) {
	      posSearch->posRepeatSequences[alignIndex] = SEQ_ALIGN_MARK_REPEAT;
	      found = TRUE;
	      break;
	    }      
	  if (!found) 
	    search->posConverged = FALSE;
	}
      }
      else  /*both alignments come from the same sequence*/
	posSearch->posRepeatSequences[alignIndex] = posSearch->posRepeatSequences[alignIndex - 1];
      prevSeqAlign = curSeqAlign;
      prevId = thisId;
      curSeqAlign = curSeqAlign->next;
    }
    MemFree(posSearch->posResultSequences);
  }
  else {
    q = search->context[0].query->sequence;
    queryLength = search->context[0].query->length;
    prevSeqAlign = NULL;
    while (curSeqAlign != NULL) {
      curSegs = (DenseSegPtr) curSeqAlign->segs;
      s = GetSequenceWithDenseSeg(curSegs, FALSE, &retrievalOffset, &subjectLength);
      numsegs = curSegs->numseg;
      thisId = curSegs->ids->next; 
      if ((NULL == prevSeqAlign) ||  (!(SeqIdMatch(thisId, prevId)))) {
	startAlign = curSeqAlign;
	thisEvalue = minEvalueForSequence(curSeqAlign, startAlign);
	if (thisEvalue < search->pbp->ethresh) {    
	  if (numsegs > 1) {
	    search->posConverged = FALSE;
	    return;
	  }
	  startQ = 0;
	  startS = 1;
	  queryOffset = curSegs->starts[startQ];
	  if (curSegs->starts[startS] != GAP_HERE)
	    subjectOffset = curSegs->starts[startS] - retrievalOffset;
	  else
	    subjectOffset = GAP_HERE;
	  matchLength = curSegs->lens[0];
	  if ((queryOffset != 0) || (subjectOffset != 0) ||
	      (matchLength != queryLength) || (matchLength != subjectLength)) {
	    search->posConverged = FALSE;
	    return;
	  }
	  for (c = 0, qplace = queryOffset, splace = subjectOffset;
	       c < matchLength; c++, qplace++, splace++)
	    if (s[splace] != q[qplace]) {
	      search->posConverged = FALSE;
	      return;
	    }        
	}
      }
      prevSeqAlign = curSeqAlign;
      prevId = thisId;
      curSeqAlign = curSeqAlign->next;
    }
  }
}


/*Eliminate the matches from sequence second starting at position
matchStart and extending for intervalLength characters */
void LIBCALL posCancel(posSearchItems *posSearch, compactSearchItems * compactSearch, Int4 first, Int4 second, Int4 matchStart, Int4 intervalLength)
{
  Int4 c, i;
  Boolean stillNeeded;

  for(c = matchStart, i = 0; i < intervalLength; i++, c++) {
    posSearch->posDescMatrix[second][c].used = FALSE;
    posSearch->posDescMatrix[second][c].letter = 0;
  }
  stillNeeded = FALSE;
  for (c = 0; c < compactSearch->qlength; c++)
    if (posSearch->posDescMatrix[second][c].used) {
      stillNeeded = TRUE;
      break;
    }
   if (!stillNeeded)
      posSearch->posUseSequences[second] = FALSE;
}

/*Eliminate sequences that are identical to the query and partial alignments
  that are identical in two matching sequences
  Modified by Natsuhiko Futamura to change order in which
  pairs of sequences are compared*/
void LIBCALL posPurgeMatches(posSearchItems *posSearch, compactSearchItems * compactSearch)
{
  Int4 i, j; /*index over sequences*/
  Int4 k; /*difference between pair of sequence indices*/
  Boolean matchesQuery; /*Is a matching sequence identical to the query?*/
  Int4 c; /*index over demographics of matching sequence*/
  Int4 state; /*state of checking for a match*/
  Int4 intervalLength, matchStart; /*Length and start of a matching region*/
  Int4 matchNumber; /*number of characters matching*/
  Int4 Xcount; /*number of X's in interval*/

  posSearch->posUseSequences =  (Boolean *) MemNew((posSearch->posNumSequences + 1) * sizeof(Boolean));
   if (NULL == posSearch->posUseSequences)
     exit(EXIT_FAILURE);
  for(i = 0; i <= posSearch->posNumSequences; i++)
    posSearch->posUseSequences[i] = TRUE;
  for(i = 1; i <= posSearch->posNumSequences; i++) {
    matchesQuery = TRUE;
    for (c = 0; c < compactSearch->qlength; c++) {
      if ((!posSearch->posDescMatrix[i][c].used) ||
          (posSearch->posDescMatrix[i][c].letter !=
           posSearch->posDescMatrix[0][c].letter)) {
        matchesQuery = FALSE;
        break;
      }
    }
    if (matchesQuery) {
      posSearch->posUseSequences[i] = FALSE;
    }
  }
  for(j = 1; j <= posSearch->posNumSequences; j++) {
    if (!posSearch->posUseSequences[j])
      continue;
    state = POS_COUNTING;
    c = 0;
    matchStart = 0;
    intervalLength = 0;
    Xcount = 0;
    matchNumber = 0;
    while (c < compactSearch->qlength) {
      if (posSearch->posDescMatrix[j][c].used) {
	if ((posSearch->posDescMatrix[0][c].letter != Xchar) &&
	    (posSearch->posDescMatrix[j][c].letter != Xchar)) { 
	  if (state == POS_RESTING) {
	    matchStart = c;
	    intervalLength = 1;
	    state = POS_COUNTING;
	    matchNumber = 0;
	  }
	  else 
	    intervalLength++;
	  if (posSearch->posDescMatrix[j][c].used &&
	      (posSearch->posDescMatrix[0][c].letter == posSearch->posDescMatrix[j][c].letter))
	    matchNumber++;
	}
	else {
	  if (POS_COUNTING == state)
	    Xcount++;
	}
      }
      else {
	if (state == POS_COUNTING) {
	  if ((intervalLength > 0) && (matchNumber == intervalLength))
	    posCancel(posSearch,compactSearch,0,j,matchStart,intervalLength+Xcount);
	  state = POS_RESTING;
	  Xcount = 0;
	}
      }
      c++;
    }
    if (state == POS_COUNTING) /*at end of sequence i*/
      if ((intervalLength > 0) && (matchNumber == intervalLength))
	posCancel(posSearch,compactSearch,0,j,matchStart,intervalLength+Xcount);
  }
  
  for (k=1; k <= posSearch->posNumSequences -1; k++){
    for (i = 1; (i+k) <= posSearch->posNumSequences; i++) {
      if (!posSearch->posUseSequences[i])
	continue;
      j = i+k;
      if (!posSearch->posUseSequences[j])
	continue;

      state = POS_COUNTING;
      c = 0;
      matchStart = 0;
      intervalLength = 0;
      Xcount = 0;
      matchNumber = 0;
      while (c < compactSearch->qlength) {
	if (posSearch->posDescMatrix[i][c].used ||
	    posSearch->posDescMatrix[j][c].used) {
	  if ((posSearch->posDescMatrix[i][c].letter != Xchar) &&
	      (posSearch->posDescMatrix[j][c].letter != Xchar)) { 
	    if (state == POS_RESTING) {
	      matchStart = c;
	      intervalLength = 1;
	      state = POS_COUNTING;
	      matchNumber = 0;
	    }
	    else 
	      intervalLength++;
	    if (posSearch->posDescMatrix[i][c].used &&
		posSearch->posDescMatrix[j][c].used &&
		(posSearch->posDescMatrix[i][c].letter == posSearch->posDescMatrix[j][c].letter))
	      matchNumber++;
	  }
	  else {
	    if (POS_COUNTING == state)
	      Xcount++;
	  }
	}
	else {
	  if (state == POS_COUNTING) {
	    if ((intervalLength > 0) && ((((Nlm_FloatHi) matchNumber)/intervalLength) >= IDENTITY_RATIO))
	      posCancel(posSearch,compactSearch,i,j,matchStart,intervalLength+Xcount);
	    state = POS_RESTING;
	    Xcount = 0;
	  }
	}
	c++;
      }
      if (state == POS_COUNTING) /*at end of sequence i*/
	if ((intervalLength > 0) && ((((Nlm_FloatHi) matchNumber)/intervalLength) >= IDENTITY_RATIO))
	  posCancel(posSearch,compactSearch,i,j,matchStart,intervalLength+Xcount);
    }
  }
}

/*Compute general information about the sequences that matched on the
  i-th pass such as how many matched at each query position and what letter
  matched*/
void LIBCALL posDemographics(posSearchItems *posSearch, 
                             compactSearchItems * compactSearch, 
                             SeqAlignPtr listOfSeqAligns)
{
   Uint1Ptr q; /*pointers into query */
   Uint1Ptr s; /*pointer into a matching string */
   Int4 length, subjectLength;  /*length of query and subject*/
   Int4 c; /*index into a string*/
   Int4 numseq, numSeqAligns;  /*number of matching sequences and SeqAligns*/
   Int4 seqIndex;  /*index for the array of matching sequences*/
   Int4 matchLength; /*length of a match*/
   Int4  queryOffset, subjectOffset, retrievalOffset;  /*offsets needed to make a match align*/
   Int4 qplace, splace; /*index into query string and matching string*/
   SeqAlignPtr curSeqAlign, prevSeqAlign; /*pointers into listOfSeqAligns*/
   DenseSegPtr curSegs, prevSegs;  /*used to extract alignments from curSeqAlign*/
   SeqIdPtr curId, prevId;  /*Used to compare sequences that come from different SeqAligns*/
   Int4 startQ, startS; /*Indices into array of starting positions*/
   Int4 numsegs; /*Number of pieces in the gapped alignment*/
   Int4 segIndex; /*Index for which piece we are at*/
   Nlm_FloatHi thisEvalue;  /*evalue of current partial alignment*/
   Boolean is_new_id = FALSE;

   q = compactSearch->query;
   length = compactSearch->qlength;
   for(c = 0; c < length; c++) {
     posSearch->posDescMatrix[0][c].letter = (Int1) q[c];
     posSearch->posDescMatrix[0][c].used = TRUE;
     posSearch->posDescMatrix[0][c].leftExtent = 0;
     posSearch->posDescMatrix[0][c].rightExtent = length;
     posSearch->posDescMatrix[0][c].e_value = compactSearch->ethresh/2;
     posSearch->posC[c][q[c]]++;
     posSearch->posCount[c]++;
   }

   numSeqAligns = countSeqAligns(listOfSeqAligns, &numseq, 
                                 !compactSearch->use_best_align, 
                                 compactSearch->ethresh);
   
   posSearch->posNumSequences = numseq;
   /*use only those sequences below e-value threshold*/
   seqIndex = 0;
   curSeqAlign = listOfSeqAligns;
   prevSeqAlign = NULL;
   for(curSeqAlign = listOfSeqAligns; curSeqAlign != NULL; 
       curSeqAlign = curSeqAlign->next) {
       is_new_id = FALSE;

       thisEvalue = getEvalueFromSeqAlign(curSeqAlign);

       curSegs = (DenseSegPtr) curSeqAlign->segs;
       if (NULL != prevSeqAlign) {
           prevSegs = (DenseSegPtr) prevSeqAlign->segs;
           
           if(curSegs->ids == NULL) 
               break;
           
           curId = curSegs->ids->next; 
           prevId = prevSegs->ids->next;
           if (!(SeqIdMatch(curId, prevId)))
               is_new_id = TRUE;
       }
       
       if(!(compactSearch->use_best_align && is_new_id)) { 
           if (thisEvalue >= compactSearch->ethresh)
               continue;
       }
       
       if(is_new_id == TRUE)
           seqIndex++;
       
       s = GetSequenceWithDenseSeg(curSegs, FALSE, &retrievalOffset, &subjectLength);
       if (s == NULL) {
           /* Kludge: set all of this sequence's residues to those of the query
            * so that it can be purged in posPurgeMatches */
           for (c = 0; c < length; c++) {
               posSearch->posDescMatrix[seqIndex+1][c].letter = (Int1) q[c];
               posSearch->posDescMatrix[seqIndex+1][c].used = TRUE;
               posSearch->posDescMatrix[seqIndex+1][c].e_value =
                   compactSearch->ethresh/2;
           }
           continue;
       }
       startQ = 0;
       startS = 1;
       numsegs = curSegs->numseg;
       for(segIndex = 0; segIndex < numsegs; segIndex++) {
           queryOffset = curSegs->starts[startQ];
           if (curSegs->starts[startS] != GAP_HERE) /*XX*/
               subjectOffset = curSegs->starts[startS] - retrievalOffset;
           else
               subjectOffset = GAP_HERE;
           matchLength = curSegs->lens[segIndex];
           if ((GAP_HERE ) == queryOffset) {
               ; /*do nothing, gap in query*/
           }
           else
	     if ((GAP_HERE) == subjectOffset) { /*XX*/
                   for(c = 0, qplace = queryOffset;
                       c < matchLength; c++, qplace++) {
                     /*Keep the following test if spreading out gap costs, 
                       so that in that case a lower E-value non-gap trumps
                       a higher E-value gap; if not spreading out gap costs
                       then comment out the test, so that a higher E-value
                       gap trumps a lower E-value letter*/
		     if (!posSearch->posDescMatrix[seqIndex+1][qplace].used)
		       {
			 posSearch->posDescMatrix[seqIndex + 1][qplace].used = TRUE;
			 posSearch->posDescMatrix[seqIndex + 1][qplace].letter = GAP_CHAR;
			 posSearch->posDescMatrix[seqIndex + 1][qplace].e_value = 1.0;
		       }
                   }
               }
               else {  /*no gap*/
                   for(c = 0, qplace = queryOffset, splace = subjectOffset;
                       c < matchLength; c++, qplace++, splace++) {
		     if (!posSearch->posDescMatrix[seqIndex+1][qplace].used)
		       {
			 posSearch->posDescMatrix[seqIndex+1][qplace].letter = (Int1) s[splace]; 
			 posSearch->posDescMatrix[seqIndex+1][qplace].used = TRUE; 
			 posSearch->posDescMatrix[seqIndex+1][qplace].e_value = 
			   thisEvalue;
		       }
                   }
               }
           startQ += 2;
           startS += 2;
       }
       prevSeqAlign = curSeqAlign;
       s = MemFree(s);
   } /*closes the for loop over seqAligns*/
}

void LIBCALL posComputeExtents(posSearchItems *posSearch, compactSearchItems * compactSearch)
{
   Int4 seqIndex; /*index of sequence*/
   Int4 length; /*length of query*/
   Int4 qplace, qplace2; /*place in query*/
   Int4 numseq; /*number of sequences including query*/
   Uint1Ptr q; /*pointers into query */

   length = compactSearch->qlength;
   numseq = posSearch->posNumSequences;
   q = compactSearch->query;
   for(seqIndex = 0; seqIndex < numseq; seqIndex++) {
     if (!posSearch->posUseSequences[seqIndex+1])
       continue; /*XX*/
     if ((posSearch->posDescMatrix[seqIndex+1][0].used) 
	 && (posSearch->posDescMatrix[seqIndex+1][0].letter != GAP_CHAR))
       posSearch->posDescMatrix[seqIndex+1][0].leftExtent = 0;
     for(qplace = 1; qplace < length; qplace++)
       if(posSearch->posDescMatrix[seqIndex+1][qplace].used) {
	 if(posSearch->posDescMatrix[seqIndex+1][qplace-1].used)
	   posSearch->posDescMatrix[seqIndex+1][qplace].leftExtent =
	     posSearch->posDescMatrix[seqIndex+1][qplace -1].leftExtent;
	 else
	   posSearch->posDescMatrix[seqIndex+1][qplace].leftExtent = qplace;
       } 
     if ((posSearch->posDescMatrix[seqIndex+1][length-1].used)
	 && (posSearch->posDescMatrix[seqIndex+1][length-1].letter != GAP_CHAR))
       posSearch->posDescMatrix[seqIndex+1][length-1].rightExtent = length -1;
     for(qplace = length -2; qplace >= 0; qplace--)
       if(posSearch->posDescMatrix[seqIndex+1][qplace].used) {
	 if(posSearch->posDescMatrix[seqIndex+1][qplace+1].used)
	   posSearch->posDescMatrix[seqIndex+1][qplace].rightExtent =
	     posSearch->posDescMatrix[seqIndex+1][qplace + 1].rightExtent;
	 else
	   posSearch->posDescMatrix[seqIndex+1][qplace].rightExtent = qplace;
       }
     for(qplace = 0; qplace < length; qplace++) 
       if (posSearch->posDescMatrix[seqIndex+1][qplace].used) {
	 /* comment next if out to spread gap costs*/
	 /* if (posSearch->posDescMatrix[seqIndex+1][qplace].letter != GAP_CHAR) { */
	   posSearch->posExtents[qplace].leftExtent = MAX(posSearch->posExtents[qplace].leftExtent,
							posSearch->posDescMatrix[seqIndex+1][qplace].leftExtent);
	   posSearch->posExtents[qplace].rightExtent = MIN(posSearch->posExtents[qplace].rightExtent,
							   posSearch->posDescMatrix[seqIndex+1][qplace].rightExtent);
	 
	 }
     /*}*/ /*comment this out if we want to spread gap costs out */

     for(qplace = 0; qplace < length; qplace++) 
       /*used to check qplace for GAP_CHAR here*/ /*XX*/
       if (posSearch->posDescMatrix[seqIndex+1][qplace].used) {
	 posSearch->posC[qplace][posSearch->posDescMatrix[seqIndex+1][qplace].letter]++;
	 posSearch->posCount[qplace]++; /*Add to number of matches in this query position*/
       }
   }
   for(qplace = 0; qplace < length; qplace++)
     posSearch->posIntervalSizes[qplace] = posSearch->posExtents[qplace].rightExtent - 
       posSearch->posExtents[qplace].leftExtent + 1;
   for(qplace =0; qplace < length; qplace++) {
     if(Xchar == q[qplace]) {
       posSearch->posIntervalSizes[qplace] = 0;
       for(qplace2 = 0; qplace2 <qplace; qplace2++) {
	 if((Xchar != q[qplace2]) && (posSearch->posExtents[qplace2].rightExtent >= qplace))
	   posSearch->posIntervalSizes[qplace2]--;
       }
       for(qplace2 = length-1; qplace2 > qplace; qplace2--) {
	 if((Xchar != q[qplace2]) && (posSearch->posExtents[qplace2].leftExtent <= qplace))
	   posSearch->posIntervalSizes[qplace2]--;
       }
     }
   }
 }
 
/*Compute weight of each sequence and letter in each position*/
void LIBCALL posComputeSequenceWeights(posSearchItems *posSearch, compactSearchItems * compactSearch, Nlm_FloatHi weightExponent)
{
   Int4 length; /*length of query*/
   Int4 numseq, seqIndex; /*number of matches, index for them*/
   Int4  i; /*index over a multi-alignment block*/
   Int4 qplace; /*index into query*/
   Nlm_FloatHi Sigma; /*Number of different characters occurring in matches within
                   a multi-alignment block, excluding identical columns*/
   Nlm_FloatHi intervalSigma; /*Same as Sigma but includes identical columns*/
   Int4 alphabetSize; /*number of characters in alphabet*/
   Int4 *participatingSequences; /*array of participating sequences at a position*/
   Int4 *oldParticipatingSequences; /*array of participating sequences at a position*/
   Int4 posLocalVariety;  /*number of different characters at a position*/
   Int4 *posLocalC; /*counts of how many of each letter in this column*/
   Int4 c;
   Int4 thisSeq;
   Int4 numParticipating; /*number of sequences in this alignment block*/
   Int4 oldNumParticipating; /*number of sequences in this alignment block*/
   Boolean newSequenceSet; 
   Int4 p; /*index on sequences*/
   Nlm_FloatHi weightSum; /*Sum of intermediate sequence weights in a column 
                            used to normalize the weights, so they sum to 1*/

   alphabetSize = compactSearch->alphabetSize;
   length = compactSearch->qlength;
   numseq = posSearch->posNumSequences;
   participatingSequences = (Int4 *) MemNew((numseq+1) * sizeof(Int4));
   if (NULL == participatingSequences)
     exit(EXIT_FAILURE);
   oldParticipatingSequences = (Int4 *) MemNew((numseq+1) * sizeof(Int4));
   if (NULL == oldParticipatingSequences)
     exit(EXIT_FAILURE);
   posLocalC = (Int4 *) MemNew(alphabetSize * sizeof(Int4));
   if (NULL == posLocalC)
     exit(EXIT_FAILURE);
   for (qplace = 0; qplace < length; qplace++) {
     posSearch->posSigma[qplace] = 0.0;
   }
   numParticipating = 0;
   for(qplace = 0; qplace < length; qplace++) {
     posSearch->posGaplessColumnWeights[qplace] = 0.0;
     if ((posSearch->posCount[qplace] > 1) && (posSearch->posIntervalSizes[qplace] > 0)) {
       oldNumParticipating = numParticipating;
       for(p =0; p < numParticipating; p++)
         oldParticipatingSequences[p] = participatingSequences[p];
       numParticipating = 0;
       for (seqIndex = 0; seqIndex <= numseq; seqIndex++) {
         if (!posSearch->posUseSequences[seqIndex])
           continue; 
	 /* if ((posSearch->posDescMatrix[seqIndex][qplace].used) &&
	     (posSearch->posDescMatrix[seqIndex][qplace].letter != GAP_CHAR)) {
	 */
	 /*change to this if we want to spread gap costs*/
	 if (posSearch->posDescMatrix[seqIndex][qplace].used) {
	     participatingSequences[numParticipating] = seqIndex; 
	   numParticipating++;
	 }
       }
       newSequenceSet = TRUE;
       if (numParticipating == oldNumParticipating) {
         for(p = 0; p < numParticipating; p++)
           if (oldParticipatingSequences[p] != participatingSequences[p])
             break;
         if (p == numParticipating)
           newSequenceSet = FALSE;
       }
         
       if (newSequenceSet) {
	 Sigma = 0;
	 intervalSigma = 0;
	 for (seqIndex = 0; seqIndex <= numseq; seqIndex++) {
	   if (!posSearch->posUseSequences[seqIndex])
	     continue;
	   posSearch->posRowSigma[seqIndex] = 0.0;
	   posSearch->posA[seqIndex] = 0.0;
	 }
	 for (i = posSearch->posExtents[qplace].leftExtent;
	      i <= posSearch->posExtents[qplace].rightExtent; i++) {
	   posLocalVariety = 0;
	   for(c = 0; c < alphabetSize; c++)
	     posLocalC[c] = 0;
	   for(seqIndex = 0; seqIndex < numParticipating; seqIndex++) {
	     thisSeq = participatingSequences[seqIndex];
	     /*used to check for GAP here*/ /*XX*/
	     if (0 == posLocalC[posSearch->posDescMatrix[thisSeq][i].letter]) 
	       /*letter (not a gap) not seen before in this query pos.*/
	       posLocalVariety++;  
	     posLocalC[posSearch->posDescMatrix[thisSeq][i].letter]++;
	   }
	   intervalSigma += posLocalVariety;
	   if (posLocalVariety > 1) {
	     Sigma += posLocalVariety;
	   }
	   for(seqIndex = 0; seqIndex < numParticipating; seqIndex++) {
	     thisSeq = participatingSequences[seqIndex];
	     /*used to check for gap here*/
	     posSearch->posRowSigma[thisSeq] += 
		( 1.0 / 
  (((Nlm_FloatHi) posLocalC[posSearch->posDescMatrix[thisSeq][i].letter])
    * posLocalVariety));
	   }
	 }
       }
       if (Sigma > 0) {
	 weightSum = 0;
	 for (seqIndex = 0; seqIndex < numParticipating; seqIndex++) {
	   thisSeq = participatingSequences[seqIndex];
	   posSearch->posA[thisSeq] = posSearch->posRowSigma[thisSeq]/
	    (posSearch->posExtents[qplace].rightExtent -
	       posSearch->posExtents[qplace].leftExtent +1);
	   /*spread gap weight here*/
           posSearch->posA[thisSeq] = pow(posSearch->posA[thisSeq],
                                          weightExponent);
	   weightSum += posSearch->posA[thisSeq];
	 }
	 for (seqIndex = 0; seqIndex < numParticipating; seqIndex++) {
	   thisSeq = participatingSequences[seqIndex];
	   posSearch->posA[thisSeq] = posSearch->posA[thisSeq]/weightSum;
	 }
       }
       else {
         for (seqIndex = 0; seqIndex < numParticipating; seqIndex++) {
	   thisSeq = participatingSequences[seqIndex];
	   posSearch->posA[thisSeq] = ((Nlm_FloatHi) 1 / (Nlm_FloatHi) numParticipating);
         }
       }
       posSearch->posSigma[qplace] = intervalSigma;
       for (seqIndex = 0; seqIndex < numParticipating; seqIndex++) {
	 thisSeq = participatingSequences[seqIndex];
	 posSearch->posMatchWeights[qplace][posSearch->posDescMatrix[thisSeq][qplace].letter] += posSearch->posA[thisSeq];
         if(posSearch->posDescMatrix[thisSeq][qplace].letter)
           posSearch->posGaplessColumnWeights[qplace] += posSearch->posA[thisSeq]; 
       }
     }
   }
   MemFree(participatingSequences);
   MemFree(oldParticipatingSequences);
   MemFree(posLocalC);
}

static Nlm_FloatHi countsFunction(Nlm_FloatHi Sigma, Int4 intervalLength)
{
  return(Sigma / intervalLength - 1);
}

static Nlm_FloatHi posit_rounddown(Nlm_FloatHi value)
{
  return (Nlm_FloatHi) Nlm_Nint(value);
}

/*check that weights add to 1 in each column */
void LIBCALL posCheckWeights(posSearchItems *posSearch, compactSearchItems * compactSearch)
{
   Uint1Ptr q;  /*pointer to query*/
   Int4 length, alphabetSize; /*length of query and number of characters in alphabet*/
   Int4  a, c; /*loop indices*/
   Nlm_FloatHi runningSum; /*partial total for a column*/

   length = compactSearch->qlength;
   alphabetSize = compactSearch->alphabetSize;

   q = compactSearch->query;
   for(c = 0; c < length; c++) {
     if ((posSearch->posCount[c] > 1) && (q[c] != Xchar)) {
       runningSum = 0;
       /*       if (posSearch->posMatchWeights[c][0] > 0.0)
		printf("Stop here %d ", c); */
       for(a = 0; a < alphabetSize; a++) 
           runningSum += posSearch->posMatchWeights[c][a];
       if((runningSum < 0.99) || (runningSum > 1.01))
         ErrPostEx(SEV_ERROR, 0, 0, "\nERROR IN WEIGHTS, column %d, value %lf\n",c, runningSum);
       /* spread out gap weight here*/
       for(a = 1; a < alphabetSize; a++) 
	 if (compactSearch->standardProb[a] > posEpsilon)
	   posSearch->posMatchWeights[c][a] = posSearch->posMatchWeights[c][a] +
           (posSearch->posMatchWeights[c][0] * compactSearch->standardProb[a]);
       posSearch->posMatchWeights[c][0] = 0.0;
       runningSum = 0;
       for(a = 0; a < alphabetSize; a++) 
           runningSum += posSearch->posMatchWeights[c][a];
       if((runningSum < 0.99) || (runningSum > 1.01))
         ErrPostEx(SEV_ERROR, 0, 0, "\nERROR IN WEIGHTS, column %d, value %lf\n",c, runningSum);
     }
   }
}

/*Fill in information content per position from pseudo-count frequencies*/
static void  posFreqsToInformation(posSearchItems * posSearch, compactSearchItems * compactSearch)
{
   Int4 length;  /*length of the query*/
   Int4 c; /*loop index*/
   Int4 a, alphabetSize; /*loop index and size of alphabet*/
   Nlm_FloatHi  qOverPEstimate; /*intermediate term*/
   Nlm_FloatHi  infoSum; /*information content sum for this position*/
  
   length = compactSearch->qlength;
   alphabetSize = compactSearch->alphabetSize;
   for (c = 0; c < length; c++) {
     infoSum = 0;
     for(a = 0; a < alphabetSize; a++) {
       if (compactSearch->standardProb[a] > posEpsilon) {
         qOverPEstimate = posSearch->posFreqs[c][a] / compactSearch->standardProb[a];
         if (qOverPEstimate > posEpsilon)
	   infoSum += posSearch->posFreqs[c][a] * log(qOverPEstimate)/
                    NCBIMATH_LN2;
       }
     }
     posSearch->posInformation[c] = infoSum;
   }
}

/*Convert pseudo-count frequencies to a score matrix, where standard
matrix is represented by its frequencies */
void LIBCALL posFreqsToMatrix(posSearchItems *posSearch, 
                              compactSearchItems *compactSearch)
{
   Uint1Ptr q;  /*pointer to the query*/
   Int4 length;  /*length of the query*/
   Int4 c; /*loop index*/
   Int4 a, alphabetSize; /*loop index and size of alphabet*/
   Nlm_FloatHi lambda; /*Karlin-Altschul parameter*/
   Nlm_FloatHi  qOverPEstimate, value; /*intermediate terms*/
   Boolean allZeros; /*are all frequencies in a column 0?*/
   Nlm_FloatHi intermediateValue; /*intermediate value*/

   q = compactSearch->query;
   length = compactSearch->qlength;

   alphabetSize = compactSearch->alphabetSize;
   lambda = compactSearch->lambda_ideal;


   for(c = 0; c < length; c++) {
     allZeros = TRUE;
     for(a = 0; a < alphabetSize; a++) {
       /*Division compensates for multiplication in posComputePsedoFreqs*/
       if (compactSearch->standardProb[a] > posEpsilon)
	 qOverPEstimate = posSearch->posFreqs[c][a]/compactSearch->standardProb[a];
       else
        qOverPEstimate = 0.0;
       if (qOverPEstimate != 0.0)
         allZeros = FALSE;
       if (0.0 == qOverPEstimate || (compactSearch->standardProb[a] < posEpsilon))
	 posSearch->posPrivateMatrix[c][a] = BLAST_SCORE_MIN;
       else {
	 value = log(qOverPEstimate)/lambda;
	 posSearch->posPrivateMatrix[c][a] = (BLAST_Score) posit_rounddown(POSIT_SCALE_FACTOR*value);

       }
       if (((Xchar == a) || (StarChar == a)) && (compactSearch->matrix[q[c]][Xchar] != BLAST_SCORE_MIN))
	 posSearch->posPrivateMatrix[c][a] = 
	   compactSearch->matrix[q[c]][a] * POSIT_SCALE_FACTOR;
     }
     if (allZeros) {
       if ( !posSearch->stdFreqRatios ) {
         ErrPostEx(SEV_FATAL, 1, 0, "Frequency ratios for %s scoring matrix are not available\n", compactSearch->standardMatrixName);
         return;
       }

	 for(a = 0; a < alphabetSize; a++) {
	   posSearch->posMatrix[c][a] = compactSearch->matrix[q[c]][a];
	   if (compactSearch->matrix[q[c]][a] == BLAST_SCORE_MIN)
	     posSearch->posPrivateMatrix[c][a] = BLAST_SCORE_MIN;
	   else {
	     intermediateValue = POSIT_SCALE_FACTOR * 
             posSearch->stdFreqRatios->bit_scale_factor * 
             log(posSearch->stdFreqRatios->data[q[c]][a])/NCBIMATH_LN2;
	     posSearch->posPrivateMatrix[c][a] = Nlm_Nint(intermediateValue);
	   }
	 }
     }
   }
   for(a = 0; a < alphabetSize; a++) {
     posSearch->posPrivateMatrix[length][a] = posSearch->posPrivateMatrix[length][a] = BLAST_SCORE_MIN;
   }
}

/*copy position specific frequency matrix of diminesions qlength * alphabetSize*/
void LIBCALL copyPosFreqs(Nlm_FloatHi **posFreqsFrom, Nlm_FloatHi **posFreqsTo, Int4 qlength, Int4 alphabetSize)
{
  Int4 c, i; /*loop indices*/

  for (i = 0; i < qlength; i++)
    for (c = 0; c < alphabetSize; c++)
      posFreqsTo[i][c] = posFreqsFrom[i][c];
}

Nlm_FloatHi ** LIBCALL allocatePosFreqs(Int4 length, Int4 alphabetSize)
{
  Int4 c, i; /*loop indices*/
  Nlm_FloatHi ** returnArray;

  returnArray = (Nlm_FloatHi **) MemNew((length + 1) * sizeof(Nlm_FloatHi *));
  if (NULL == returnArray)
    exit(EXIT_FAILURE);
  for(i = 0; i <= length; i++) {
    returnArray[i] = (Nlm_FloatHi *) MemNew(alphabetSize * sizeof(Nlm_FloatHi));
    if (NULL == returnArray[i])
      exit(EXIT_FAILURE);   
    for(c = 0; c < alphabetSize; c++)
      returnArray[i][c] = 0.0;
  }
  return(returnArray); 
}

Nlm_FloatHi ** LIBCALL posComputePseudoFreqs(posSearchItems *posSearch, compactSearchItems * compactSearch, Boolean Cpos)
{
   Uint1Ptr q;  /*pointer to the query*/
   Int4 length;  /*length of the query*/
   Int4 c; /*loop index*/
   Int4 a, aSub, alphabetSize; /*loop indices and size of alphabet*/
   Nlm_FloatHi lambda; /*Karlin-Altschul parameter*/
   Nlm_FloatHi Sigma;  /*number of characters in an interval*/ 
   Int4 intervalLength;  /*length of a block*/
   Nlm_FloatHi pseudo, numerator, denominator, qOverPEstimate; /*intermediate terms*/
   Nlm_FloatHi infoSum; /*sum used for information content*/
   Nlm_FloatHi **posFreqs; /*store frequencies*/

   q = compactSearch->query;
   length = compactSearch->qlength;

   alphabetSize = compactSearch->alphabetSize;
   lambda = compactSearch->lambda_ideal;
   posFreqs = allocatePosFreqs(length, alphabetSize);

   if (!posSearch->stdFreqRatios) {
     posSearch->stdFreqRatios =
           PSIMatrixFrequencyRatiosNew(compactSearch->standardMatrixName);
   }

   for(c = 0; c < length; c++) {
     if (Xchar != q[c]) {
       infoSum = 0;
       for(a = 0; a < alphabetSize; a++) {
         if (compactSearch->standardProb[a] > posEpsilon) {
	   pseudo = 0;
           /*changed to matrix specific ratios here May 2000*/
	   for (aSub = 0; aSub < alphabetSize; aSub++)
	     if(compactSearch->matrix[a][aSub] != BLAST_SCORE_MIN) 
	       pseudo += (posSearch->posMatchWeights[c][aSub] *
			posSearch->stdFreqRatios->data[a][aSub]);
	   pseudo *= (compactSearch->pseudoCountConst);
           Sigma = posSearch->posSigma[c];
           intervalLength = posSearch->posIntervalSizes[c];
	   numerator = pseudo + 
             (countsFunction(Sigma, intervalLength) * posSearch->posMatchWeights[c][a]/
                compactSearch->standardProb[a]);
	   denominator = countsFunction(Sigma, intervalLength) + (compactSearch->pseudoCountConst); 
	   qOverPEstimate = numerator / denominator;
	   /*Note artificial multiplication by standard probability to
             normalize*/
           posFreqs[c][a] = qOverPEstimate * compactSearch->standardProb[a];
	 if (0.0 != qOverPEstimate && (compactSearch->standardProb[a] > posEpsilon))
	   infoSum += qOverPEstimate * compactSearch->standardProb[a] * log(qOverPEstimate)/ NCBIMATH_LN2;
	 }
        else
          posFreqs[c][a] = 0.0;
       }
       if (Cpos)
	 posSearch->posInformation[c] = infoSum;
     }
     else
       for(a = 0; a < alphabetSize; a++) {
         posFreqs[c][a] = 0;
       }
   }
  return(posFreqs);
}

void LIBCALL posScaling(posSearchItems *posSearch, compactSearchItems * compactSearch)
{
	BlastMatrixRescalePtr matrix_rescale;

	matrix_rescale = BlastMatrixRescaleNew(compactSearch->alphabetSize, 
						compactSearch->qlength,
						compactSearch->query,
						compactSearch->standardProb,
						posSearch->posMatrix,
						posSearch->posPrivateMatrix,
						compactSearch->kbp_std,
						compactSearch->kbp_psi,
						compactSearch->kbp_gap_std,
						compactSearch->kbp_gap_psi,
						compactSearch->lambda_ideal,
						compactSearch->K_ideal);

	BlastScaleMatrix(matrix_rescale, TRUE);

	matrix_rescale = BlastMatrixRescaleDestruct(matrix_rescale);

	return;
}


Int4Ptr * LIBCALL CposComputation(posSearchItems *posSearch, BlastSearchBlkPtr search, compactSearchItems * compactSearch, SeqAlignPtr listOfSeqAligns, Char *ckptFileName, Boolean patternSearchStart, Int4 scorematOutput, Bioseq *query_bsp, Int4 gap_open, Int4 gap_extend, ValNodePtr * error_return, Nlm_FloatHi weightExponent)
{
    Int4 numalign, numseq; /*number of alignments and matches in previous round*/
    
    search->posConverged = FALSE;
    /*  if (patternSearchStart)
        posAllocateMemory(posSearch, compactSearch->alphabetSize, compactSearch->qlength, posSearch->posNumSequences);
        else {
    */
    numalign = countSeqAligns(listOfSeqAligns, &numseq, FALSE, 0.0);
    posAllocateMemory(posSearch, compactSearch->alphabetSize, compactSearch->qlength, numseq);
    
    if (!patternSearchStart)
        findThreshSequences(posSearch, search, listOfSeqAligns, numalign, numseq);
    posDemographics(posSearch, compactSearch, listOfSeqAligns);
    posPurgeMatches(posSearch, compactSearch);
    posComputeExtents(posSearch, compactSearch);
    posComputeSequenceWeights(posSearch, compactSearch, weightExponent);
    posCheckWeights(posSearch, compactSearch);
    posSearch->posFreqs = posComputePseudoFreqs(posSearch, compactSearch, TRUE);
    if (NULL == search->sbp->posFreqs)
      search->sbp->posFreqs =  allocatePosFreqs(compactSearch->qlength, compactSearch->alphabetSize);
    copyPosFreqs(posSearch->posFreqs,search->sbp->posFreqs, compactSearch->qlength, compactSearch->alphabetSize);
    if (NULL != ckptFileName) {
      if (scorematOutput == NO_SCOREMAT_IO)
        posTakeCheckpoint(posSearch, compactSearch, ckptFileName, error_return);
      else
        posTakeScoremat(posSearch, compactSearch, ckptFileName,
                        scorematOutput, query_bsp, gap_open,
                        gap_extend, error_return);
    }
    posFreqsToMatrix(posSearch,compactSearch);
    posScaling(posSearch, compactSearch);
    return posSearch->posMatrix;
}

/* Top-level routine to compute position-specific matrix, when used through
the Web, one round at a time*/
Int4Ptr * LIBCALL WposComputation(compactSearchItems * compactSearch, SeqAlignPtr listOfSeqAligns, Nlm_FloatHi ** posFreqs)
{
    posSearchItems *posSearch;
    Int4 i, numSeqAligns, numseq, qlength;
    Int2 alphabetSize;
    Int4Ptr *posMatrix;
    
    /* Why isn't posAllocateMemory() called? */
    posSearch = (posSearchItems *) MemNew(1 * sizeof(posSearchItems));
    qlength = compactSearch->qlength;
    alphabetSize = compactSearch->alphabetSize;

    if (listOfSeqAligns != NULL) {
       numSeqAligns = countSeqAligns(listOfSeqAligns, &numseq, FALSE, 0.0);
       posAllocateMemory(posSearch, alphabetSize, 
                         qlength, numseq);
       posDemographics(posSearch, compactSearch, listOfSeqAligns);
       posPurgeMatches(posSearch, compactSearch);
       posComputeExtents(posSearch, compactSearch);
       posComputeSequenceWeights(posSearch, compactSearch, 1.0);
       posCheckWeights(posSearch, compactSearch);
       posSearch->posFreqs = posComputePseudoFreqs(posSearch, compactSearch, 
                                                   FALSE);
       copyPosFreqs(posSearch->posFreqs,posFreqs, qlength, alphabetSize);
    } else {
       /* Assume that posFreqs are already filled, use them to calculate
          posMatrix.
          If listOfSeqAligns is NULL as a result of search, all frequencies are
          0 anyway, so there is no need to compute them. However if it is
          deliberately passed as NULL before search, this means that posFreqs
          are passed as input from PSSM.
       */
       posSearch->posFreqs = posFreqs;
       ASSERT(compactSearch->standardMatrixName);
       posSearch->stdFreqRatios =
           PSIMatrixFrequencyRatiosNew(compactSearch->standardMatrixName);
       posSearch->posMatrix = (BLAST_Score **) MemNew((qlength + 1) * sizeof(BLAST_Score *));
       posSearch->posPrivateMatrix = (BLAST_Score **) MemNew((qlength + 1) * sizeof(BLAST_Score *));
       for(i = 0; i <= qlength; i++) {
          posSearch->posMatrix[i] = (BLAST_Score *) MemNew(alphabetSize * sizeof(BLAST_Score));
          posSearch->posPrivateMatrix[i] = (BLAST_Score *) MemNew(alphabetSize * sizeof(BLAST_Score));
       }
    }
    posFreqsToMatrix(posSearch,compactSearch);
    posScaling(posSearch, compactSearch);
    posMatrix = posSearch->posMatrix;
    
    /* Why isn't posFreeMemory() called? */
    if (listOfSeqAligns != NULL) {
       for(i = 0; i <= qlength ; i++) {
          MemFree(posSearch->posFreqs[i]);
          MemFree(posSearch->posMatchWeights[i]);
          MemFree(posSearch->posC[i]);
       }
       
       MemFree(posSearch->posFreqs);
       MemFree(posSearch->posMatchWeights);
       MemFree(posSearch->posC);
       MemFree(posSearch->posA);
       MemFree(posSearch->posExtents);
       MemFree(posSearch->posSigma);
       MemFree(posSearch->posRowSigma);
       MemFree(posSearch->posIntervalSizes);
       MemFree(posSearch->posCount);
       
       for(i = 0; i <= posSearch->posDescMatrixLength; i++) {
          MemFree(posSearch->posDescMatrix[i]);
       }
       MemFree(posSearch->posDescMatrix);
    }

    for(i = 0; i <= qlength ; i++)
       MemFree(posSearch->posPrivateMatrix[i]);
    MemFree(posSearch->posPrivateMatrix);
    posSearch->stdFreqRatios = PSIMatrixFrequencyRatiosFree(posSearch->stdFreqRatios);
    MemFree(posSearch);

    return posMatrix;
}


static Char getRes(Char input)
{
    switch(input) 
      {
      case 0: 
	return('-');
      case 1: 
	return('A');
      case 2: 
	return('B');
      case 3: 
	return('C');
      case 4: 
	return('D');
      case 5: 
	return('E');
      case 6: 
	return('F');
      case 7: 
	return('G');
      case 8: 
	return('H');
      case 9: 
	return('I');
      case 10: 
	return('K');
      case 11: 
	return('L');
      case 12: 
	return('M');
      case 13: 
	return('N');
      case 14: 
	return('P');
      case 15: 
	return('Q');
      case 16: 
	return('R');
      case 17: 
	return('S');
      case 18: 
	return('T');
      case 19: 
	return('V');
      case 20: 
	return('W');
      case 21: 
	return('X');
      case 22: 
	return('Y');
      case 23: 
	return('Z');
      case 24: 
	return('U');
      case 25: 
	return('*');
      default:
        return('?');
    }
}
Uint1 LIBCALL ResToInt(Char input)
{
    switch(input) 
      {
      case '-': 
        return(0);
      case 'A': 
        return(1);
      case 'B': 
        return(2);
      case 'C': 
        return(3);
      case 'D': 
        return(4);
      case 'E': 
        return(5);
      case 'F': 
        return(6);
      case 'G': 
        return(7);
      case 'H': 
        return(8); 
      case 'I': 
        return(9);
      case 'K': 
        return(10);
      case 'L': 
        return(11);
      case 'M': 
        return(12);
      case 'N': 
        return(13);
      case 'P': 
        return(14);
      case 'Q': 
        return(15);
      case 'R': 
        return(16);
      case 'S': 
        return(17);
      case 'T': 
        return(18);
      case 'V': 
        return(19);
      case 'W': 
        return(20);
      case 'X': 
        return(21);
      case 'Y': 
        return(22);
      case 'Z': 
        return(23);
      case 'U': 
        return(24);
      case '*': 
        return(25);
      default:
        return(-1);
    }
}


/*Print out the position-specific matrix*/
void LIBCALL outputPosMatrix(posSearchItems *posSearch, compactSearchItems *compactSearch, FILE *matrixfp, Boolean posComputationCalled)
{
   Uint1Ptr q; /*query sequence*/
   Int4 i; /*loop indices*/
   Int4 c; /*index over alphabet*/
   Int4 length; /*length of query*/
   Int4 charOrder[EFFECTIVE_ALPHABET]; /*standard order of letters according to S. Altschul*/

   if (compactSearch->alphabetSize != PROTEIN_ALPHABET){
     ErrPostEx(SEV_ERROR, 0, 0, "\nCannot print diagnostic information because alphabet size is not %ld", (long) compactSearch->alphabetSize);
     return;
   }
   
   charOrder[0] =  1;  /*A*/
   charOrder[1] =  16; /*R*/
   charOrder[2] =  13; /*N*/  
   charOrder[3] =  4;  /*D*/ 
   charOrder[4] =  3;  /*C*/
   charOrder[5] =  15; /*Q*/
   charOrder[6] =  5;  /*E*/ 
   charOrder[7] =  7;  /*G*/
   charOrder[8] =  8;  /*H*/
   charOrder[9] =  9;  /*I*/
   charOrder[10] = 11; /*L*/
   charOrder[11] = 10; /*K*/
   charOrder[12] = 12; /*M*/  
   charOrder[13] =  6; /*F*/
   charOrder[14] = 14; /*P*/
   charOrder[15] = 17; /*S*/
   charOrder[16] = 18; /*T*/
   charOrder[17] = 20; /*W*/
   charOrder[18] = 22; /*Y*/
   charOrder[19] = 19; /*V*/

   q = compactSearch->query;
   length = compactSearch->qlength;
   
/* Used ifdef until final decision is made on output. */

#ifdef POSIT_DEBUG
   printf("\nCharacter Frequencies by positon\n");
   printf("         ");
   for (c = 0; c< EFFECTIVE_ALPHABET; c++)
      printf("  %c",getRes((Char) charOrder[c]));
   for(i=0; i < length; i++) {
     printf("\n%5d %c   ", i + 1, getRes(q[i]));
     for (c = 0; c < EFFECTIVE_ALPHABET; c++) 
       printf("%2d ", posSearch->posC[i][charOrder[c]]);
   }
   printf("\n\n");
   printf("\nposition counts used. multiplied by 10 and rounded and");
   printf("\nposition character weights used, multiplied by 10 and rounded\n");
   printf("        Counts");
   for (c = 0; c< EFFECTIVE_ALPHABET; c++)
      printf("  %c",getRes((Char) charOrder[c]));
   printf(" Extent ");
   for(i=0; i < length; i++) {
     printf("\n%5d %c   ", i + 1, getRes(q[i]));
     if ((posSearch->posCount[i] > 1) && (Xchar != q[i]))
       printf("%4d ", (Int4) posit_rounddown(10 * countsFunction
				     (posSearch->posSigma[i],posSearch->posIntervalSizes[i])));
     else
       printf("     ");
     for (c = 0; c< EFFECTIVE_ALPHABET; c++)
       if((posSearch->posMatrix[i][charOrder[c]] == BLAST_SCORE_MIN) ||
             (0.0 == posSearch->posMatchWeights[i][charOrder[c]]))
           printf(" - ");
         else
	   printf("%2d ", (Int4) posit_rounddown(10 * posSearch->posMatchWeights[i][charOrder[c]]));
     printf(" %4d",posSearch->posExtents[i].rightExtent - posSearch->posExtents[i].leftExtent +1);
   }
   printf("\n\n");
#endif
   if (NULL != matrixfp) {
     if (posComputationCalled) {
       fprintf(matrixfp,"\nLast position-specific scoring matrix computed, weighted observed percentages rounded down, information per position, and relative weight of gapless real matches to pseudocounts\n");
     }
     else {
       fprintf(matrixfp,"\nLast position-specific scoring matrix computed\n");
     }
     fprintf(matrixfp,"         ");
     for (c = 0; c< EFFECTIVE_ALPHABET; c++)
       fprintf(matrixfp,"  %c",getRes((Char) charOrder[c]));
     if (posComputationCalled) {
       for (c = 0; c< EFFECTIVE_ALPHABET; c++)
	 fprintf(matrixfp,"   %c",getRes((Char) charOrder[c]));
     }
     for(i=0; i < length; i++) {
       fprintf(matrixfp,"\n%5ld %c   ", (long) (i + 1), getRes(q[i]));
       /*fprintf(matrixfp,"\n          ");*/
       for (c = 0; c < EFFECTIVE_ALPHABET; c++) 
	 if(posSearch->posMatrix[i][charOrder[c]] == BLAST_SCORE_MIN)
	   fprintf(matrixfp,"-I ");
	 else
	   fprintf(matrixfp,"%2ld ", (long) posSearch->posMatrix[i][charOrder[c]]);
       if (posComputationCalled) {
	 for (c = 0; c < EFFECTIVE_ALPHABET; c++) 
	   if(posSearch->posMatrix[i][charOrder[c]] != BLAST_SCORE_MIN)
	     fprintf(matrixfp, "%4d", (Int4) posit_rounddown(100 * posSearch->posMatchWeights[i][charOrder[c]]));
	 fprintf(matrixfp," %5.2lf", posSearch->posInformation[i]); 
	 if ((posSearch->posCount[i] > 1) && (Xchar != q[i]))
	   fprintf(matrixfp," %.2lf", countsFunction(posSearch->posSigma[i],
		     posSearch->posIntervalSizes[i]) * 
                 posSearch->posGaplessColumnWeights[i]/
                 compactSearch->pseudoCountConst);
	 else
	   fprintf(matrixfp,"    0.00");
       }
     }
     fprintf(matrixfp,"\n\n");
     fprintf(matrixfp,"                      K         Lambda\n");
     fprintf(matrixfp,"Standard Ungapped    %6.4f     %6.4f\n",compactSearch->kbp_std[0]->K,compactSearch->kbp_std[0]->Lambda);
     fprintf(matrixfp,"Standard Gapped      %6.4f     %6.4f\n",compactSearch->kbp_gap_std[0]->K,compactSearch->kbp_gap_std[0]->Lambda);
     fprintf(matrixfp,"PSI Ungapped         %6.4f     %6.4f\n",compactSearch->kbp_psi[0]->K,compactSearch->kbp_psi[0]->Lambda);
     fprintf(matrixfp,"PSI Gapped           %6.4f     %6.4f\n",compactSearch->kbp_gap_psi[0]->K,compactSearch->kbp_gap_psi[0]->Lambda);
   }
}


void LIBCALL posPrintInformation(posSearchItems *posSearch, BlastSearchBlkPtr search, Int4 passNum)
{
  Int4 querySize;

  querySize = search->context[0].query->length;

/* Used ifdef until final decision is made on output. */
#ifdef POSIT_DEBUG
  {{
      Int4 c;
  
      printf("\nInformation content by position for pass %d\n", passNum);
      for(c = 0; c < querySize; c++)
          printf(" %5d", c); 
      printf("\n");
      for(c = 0; c < querySize; c++)
          printf(" %5.2lf", posSearch->posInformation[c]); 
      printf("\n");
  }}
#endif
}   
 
void LIBCALL posInitializeInformation(posSearchItems *posSearch, BlastSearchBlkPtr search)
{
  Uint1Ptr query;
  Int4 querySize;
  Int4 c, a, alphabetSize;
  BLAST_ScoreBlkPtr sbp;
  BLAST_ResFreqPtr stdrfp; /*standard frequencies*/
  Nlm_FloatHi lambda;
  Nlm_FloatHi term1, term2, term3, term4;
  Nlm_FloatHi infoSum;
 
  querySize = search->context[0].query->length;
  query = search->context[0].query->sequence;
  posSearch->posInformation = (Nlm_FloatHi *) MemNew(querySize * sizeof(Nlm_FloatHi));
  if (NULL == posSearch->posInformation)
    exit(EXIT_FAILURE);
  for(c = 0; c < querySize; c++)
    posSearch->posInformation[c] = 0.0;
  alphabetSize = search->sbp->alphabet_size;
  /*Compute standard frequencies as in BlastScoreBlkFill in blastkar.c*/
  sbp = search->sbp;
  stdrfp = BlastResFreqNew(sbp);
  BlastResFreqStdComp(sbp,stdrfp); 
  lambda = search->sbp->kbp[0]->Lambda;
  for(c = 0; c < querySize; c++) {
    infoSum = 0;
    for(a = 0; a < alphabetSize; a++)
      if (stdrfp->prob[a] > posEpsilon) {
        term1 = search->sbp->matrix[query[c]][a];
	term2 = term1 * lambda;
	term3 = exp(term2);
	term4 = stdrfp->prob[a] * term3;
	infoSum += term4 * log(term4/stdrfp->prob[a])/NCBIMATH_LN2;
      }
    posSearch->posInformation[c] = infoSum;
  }
  BlastResFreqFree(stdrfp);
}

/*
	Is this function used?
*/

void LIBCALL posFreeInformation(posSearchItems *posSearch)
{
  MemFree(posSearch->posInformation);
}

/*Copy a few fields from the lasrge record search into the small record
  compactSearch, so that a small amount of information
  is passed into posit.c*/
void LIBCALL copySearchItems(compactSearchItems * compactSearch, BlastSearchBlkPtr search, char * matrixName)
{
   BLAST_ResFreqPtr stdrfp; /* gets standard frequencies in prob field */
   Int4 a; /*index over characters*/

   compactSearch->query = search->context[0].query->sequence;
   compactSearch->qlength = search->context[0].query->length;
   compactSearch->gapped_calculation = search->pbp->gapped_calculation;
   compactSearch->alphabetSize = search->sbp->alphabet_size;
   compactSearch->pseudoCountConst = search->pbp->pseudoCountConst;
   compactSearch->ethresh = search->pbp->ethresh;
   compactSearch->lambda =  search->sbp->kbp[0]->Lambda;
   compactSearch->matrix = search->sbp->matrix;
   compactSearch->kbp_psi = search->sbp->kbp_psi;
   compactSearch->kbp_gap_psi = search->sbp->kbp_gap_psi;
   compactSearch->kbp_std = search->sbp->kbp_std;
   compactSearch->kbp_gap_std = search->sbp->kbp_gap_std;
   if (search->pbp->gapped_calculation) {
     compactSearch->lambda_ideal = search->sbp->kbp_ideal->Lambda;
     compactSearch->K_ideal = search->sbp->kbp_ideal->K;
   }
   else {
     compactSearch->lambda_ideal = search->sbp->kbp[0] ->Lambda;
     compactSearch->K_ideal = search->sbp->kbp[0]->K;
   }
   compactSearch->use_best_align = search->pbp->use_best_align;

   stdrfp = BlastResFreqNew(search->sbp);
   BlastResFreqStdComp(search->sbp,stdrfp); 
   compactSearch->standardProb = MemNew(compactSearch->alphabetSize * sizeof(Nlm_FloatHi));
   if (NULL == compactSearch->standardProb)
     exit(EXIT_FAILURE);
   for(a = 0; a < compactSearch->alphabetSize; a++)
     compactSearch->standardProb[a] = stdrfp->prob[a];
   stdrfp = BlastResFreqDestruct(stdrfp);
   strcpy(compactSearch->standardMatrixName,matrixName);
}

/*allocate memory for a record of type compactSearchItems*/
compactSearchItems * LIBCALL  compactSearchNew(compactSearchItems * compactSearch)
{
   compactSearch = MemNew(1 * sizeof(compactSearchItems));
   if (NULL == compactSearch)
     exit(EXIT_FAILURE);
   return(compactSearch);
}

/*De-allocate memory for a record of type compactSearchItems*/
void LIBCALL compactSearchDestruct(compactSearchItems * compactSearch)
{

   MemFree(compactSearch->standardProb);
   MemFree(compactSearch);
}

/*Some of the following checkpointing code is taken and adapted from
code written by K. Shriram for FASTLINK.
Reference:
 A. A. Schaffer, S. K. Gupta, K. Shriram, and R. W. Cottingham, Jr. 
 Avoiding Recomputation in Linkage Analysis,
 Human Heredity 44(1994), pp. 225-237. */


#define  putCkptNlm_FloatHi(d, ckptFile)  (putCkptNumber(&(d),sizeof(Nlm_FloatHi),ckptFile))
#define  putCkptInt4(i, ckptFile)         (putCkptNumber(&(i),sizeof(Int4),ckptFile))
#define  putCkptChar(c, ckptFile)         (putCkptNumber(&(c),sizeof(Char),ckptFile))
 
/* General routine for putting the internal representation of a number. */
 
static void  putCkptNumber(void * numberPtr, Int4 numberSize, FILE * ckptFile )
{
  FileWrite(numberPtr,numberSize,1,ckptFile) ;
}

#if 0
/* TODO: the function is not used in this file */
static void    putCkptNlmFloat_HiVector (Nlm_FloatHi * theVector, Int4 length, FILE * ckptFile)
{
  int  vectorRef;
 
  for(vectorRef = 0; vectorRef < length; vectorRef++)
    putCkptNlm_FloatHi(theVector[vectorRef],ckptFile) ;
}
#endif


/*Code to put a vector of frequencies; put only the interesting
  entries*/
static void  putFreqVector(Nlm_FloatHi * theVector, Int4 length, FILE * ckptFile)
{
   int  vectorRef;
   Int4 charOrder[EFFECTIVE_ALPHABET]; /*standard order of letters according to S. Altschul*/


   charOrder[0] =  1;  /*A*/
   charOrder[1] =  16; /*R*/
   charOrder[2] =  13; /*N*/  
   charOrder[3] =  4;  /*D*/ 
   charOrder[4] =  3;  /*C*/
   charOrder[5] =  15; /*Q*/
   charOrder[6] =  5; /*E*/ 
   charOrder[7] =  7;  /*G*/
   charOrder[8] =  8;  /*H*/
   charOrder[9] =  9;  /*I*/
   charOrder[10] = 11; /*L*/
   charOrder[11] = 10; /*K*/
   charOrder[12] = 12; /*M*/  
   charOrder[13] =  6; /*F*/
   charOrder[14] = 14; /*P*/
   charOrder[15] = 17; /*S*/
   charOrder[16] = 18; /*T*/
   charOrder[17] = 20; /*W*/
   charOrder[18] = 22; /*Y*/
   charOrder[19] = 19; /*V*/

 
   for(vectorRef = 0; vectorRef < EFFECTIVE_ALPHABET; vectorRef++)
     putCkptNlm_FloatHi(theVector[charOrder[vectorRef]],ckptFile);
}

 
/* Code to put a matrix, vector-by-vector. */
static void    putCkptFreqMatrix (Nlm_FloatHi **theMatrix, Int4 length, Int4 width, FILE * ckptFile)
{
  int  matrixRef;  /*loop index*/
 
  for (matrixRef = 0; matrixRef < length ; matrixRef++ )
    putFreqVector(theMatrix[matrixRef], width, ckptFile);
}

 
/* General routine for getting the internal representation of a number. */
 
void  LIBCALL getCkptNumber(void * numberPtr, Int4 numberSize, FILE * ckptFile )
{
  FileRead(numberPtr,numberSize,1,ckptFile) ;
}


#if 0
/* TODO: the function is not used in this file */
/*Code to get a vector of type Nlm_FloatHi*/

static void    getCkptNlmFloat_HiVector (Nlm_FloatHi * theVector, Int4 length, FILE * ckptFile)
{
  int  vectorRef ;
 
  for(vectorRef = 0; vectorRef < length; vectorRef++)
    getCkptNlm_FloatHi(theVector[vectorRef],ckptFile) ;
}
#endif

static void    getFreqVector (Nlm_FloatHi * theVector, Int4 length, FILE * ckptFile)
{
   int  vectorRef ;

   Int4 charOrder[EFFECTIVE_ALPHABET]; /*standard order of letters according to S. Altschul*/


   charOrder[0] =  1;  /*A*/
   charOrder[1] =  16; /*R*/
   charOrder[2] =  13; /*N*/  
   charOrder[3] =  4;  /*D*/ 
   charOrder[4] =  3;  /*C*/
   charOrder[5] =  15; /*Q*/
   charOrder[6] =  5; /*E*/ 
   charOrder[7] =  7;  /*G*/
   charOrder[8] =  8;  /*H*/
   charOrder[9] =  9;  /*I*/
   charOrder[10] = 11; /*L*/
   charOrder[11] = 10; /*K*/
   charOrder[12] = 12; /*M*/  
   charOrder[13] =  6; /*F*/
   charOrder[14] = 14; /*P*/
   charOrder[15] = 17; /*S*/
   charOrder[16] = 18; /*T*/
   charOrder[17] = 20; /*W*/
   charOrder[18] = 22; /*Y*/
   charOrder[19] = 19; /*V*/
 
  for(vectorRef = 0; vectorRef < length; vectorRef++)
    theVector[vectorRef] = 0;
  for(vectorRef = 0; vectorRef < EFFECTIVE_ALPHABET; vectorRef++)
    getCkptNlm_FloatHi(theVector[charOrder[vectorRef]],ckptFile) ;
}

/* Code to frequency matrix, vector-by-vector. */
 
void    LIBCALL getCkptFreqMatrix (Nlm_FloatHi ** theMatrix, Int4 length, Int4 width, FILE * ckptFile)
{
  Int4  matrixRef;  /*loop index*/
 
  for (matrixRef = 0; matrixRef < length ; matrixRef++ )
    getFreqVector(theMatrix[matrixRef], width, ckptFile);
}

/*Take a checkpoint at the end of the current PSI-BLAST round, stores
query length, query, and position-specific target frequencies.
Returns TRUE if checkpoint was sucessful and FALSE otherwise. */
Boolean LIBCALL posTakeCheckpoint(posSearchItems * posSearch, compactSearchItems * compactSearch, CharPtr fileName, ValNodePtr *error_return)
{
  FILE * checkFile; /*file in which to take the checkpoint*/
  Int4 length; /*length of query sequence, and an index for it*/
  Int4 i; /*indices to position and alphabet */
  Char localChar; /*temporary character*/

  checkFile = FileOpen(fileName, "wb");
  if (NULL == checkFile) {
    BlastConstructErrorMessage("posTakeCheckpoint", "Could not open checkpoint file", 1, error_return);
    return(FALSE);
  }
  length = compactSearch->qlength;
  putCkptInt4(length, checkFile);
  for(i = 0; i < length; i++) {
    localChar = getRes(compactSearch->query[i]);
    putCkptChar(localChar, checkFile);
  }  
  putCkptFreqMatrix(posSearch->posFreqs,length,compactSearch->alphabetSize, checkFile);
  FileClose(checkFile);
  return(TRUE);
}

/* Like posTakeCheckpoint, posTakeScoremat will emit the position
   frequencies that have been generated. Unlike that routine, the
   file to be written is an ASN.1 encoded PssmWithParameters object. */

Boolean LIBCALL posTakeScoremat(posSearchItems *posSearch, 
                       compactSearchItems *compactSearch, 
                       CharPtr filename, Int4 scorematOutput,
                       Bioseq *query_bsp, Int4 gap_open, 
                       Int4 gap_extend, ValNodePtr *error_return)
{
  AsnIoPtr outfile = NULL;
  PssmWithParametersPtr scoremat = NULL;
  PssmIntermediateDataPtr freqs = NULL;
  PssmParametersPtr params = NULL;
  FormatRpsDbParametersPtr rpsparams = NULL;
  PssmPtr pssm = NULL;
  Int4 i, j;
  Boolean status = FALSE;
  Bioseq *bsp;

  scoremat = PssmWithParametersNew();
  if (scoremat == NULL) {
    BlastConstructErrorMessage("posTakeScoremat", 
               "Could not allocate PssmWithParameters", 1, error_return);
    goto bail_out;
  }

  /* Add information about the underlying score matrix.
     Note that blastpgp will ignore this information */

  params = scoremat->params = PssmParametersNew();
  if (params == NULL) {
    BlastConstructErrorMessage("posTakeScoremat", 
               "Could not allocate PssmParameters", 1, error_return);
    goto bail_out;
  }
  rpsparams = params->rpsdbparams = FormatRpsDbParametersNew();
  if (params == NULL) {
    BlastConstructErrorMessage("posTakeScoremat", 
               "Could not allocate RpsDbParameters", 1, error_return);
    goto bail_out;
  }
  rpsparams->matrixName = strdup(compactSearch->standardMatrixName);
  rpsparams->gapOpen = gap_open;
  rpsparams->gapExtend = gap_extend;

  /* Build up the objects describing the residue frequencies */

  pssm = scoremat->pssm = PssmNew();
  if (pssm == NULL) {
    BlastConstructErrorMessage("posTakeScoremat", 
               "Could not allocate PSSM object", 1, error_return);
    goto bail_out;
  }
  freqs = pssm->intermediateData = PssmIntermediateDataNew();
  if (freqs == NULL) {
    BlastConstructErrorMessage("posTakeScoremat", 
               "Could not allocate PssmIntermediateData", 1, error_return);
    goto bail_out;
  }

  pssm->isProtein = TRUE;
  pssm->numRows = compactSearch->alphabetSize;
  pssm->numColumns = compactSearch->qlength;

  for (i = 0; i < pssm->numColumns; i++) {
    for (j = 0; j < pssm->numRows; j++) {
      ValNodeAddFloat(&freqs->freqRatios, 0, posSearch->posFreqs[i][j]);
    }
  }

  /* Do not make a copy of the query bioseq; use it directly.
     The '1' below indicates a single bioseq (not a bioseq-set) */

  ValNodeAddPointer(&pssm->query, 1, query_bsp);
  if (pssm->query == NULL) {
    BlastConstructErrorMessage("posTakeScoremat", 
               "Could not attach bioseq to scoremat", 1, error_return);
    goto bail_out;
  }

  if (scorematOutput == ASCII_SCOREMAT)
     outfile = AsnIoOpen(filename, "w");
  else
     outfile = AsnIoOpen(filename, "wb");

  if (outfile == NULL) {
    ErrPostEx(SEV_FATAL, 1, 0, "Unable to open matrix output file %s\n", 
          filename);
    goto bail_out;
  }

  PssmWithParametersAsnWrite(scoremat, outfile, NULL);
  status = TRUE;

bail_out:
  AsnIoClose(outfile);

  /* explicitly free the ValNode pointing to the query bioseq.
     This will prevent the ScoreMatrix freeing routine
     from also freeing the query bioseq, which we did not
     allocate */

  pssm->query = ValNodeFree(pssm->query);

  /* free everything else */

  scoremat = PssmWithParametersFree(scoremat);
  return status;
}

Boolean LIBCALL posReadPosFreqsScoremat(posSearchItems * posSearch, compactSearchItems * compactSearch, CharPtr fileName, Int4 scorematInput, ValNodePtr * error_return)
{
  AsnIoPtr infile = NULL;
  PssmWithParametersPtr scoremat = NULL;
  PssmPtr pssm = NULL;
  PssmIntermediateDataPtr freqs = NULL;
  Int4 i, j, c;
  ValNodePtr freq_list;
  Bioseq *bsp;

  if (scorematInput == ASCII_SCOREMAT)
     infile = AsnIoOpen(fileName, "r");
  else
     infile = AsnIoOpen(fileName, "rb");

  if (infile == NULL) {
    ErrPostEx(SEV_WARNING, 0, 0,"Could not open scoremat file\n");
    return FALSE;
  }

  scoremat = PssmWithParametersAsnRead(infile, NULL);
  AsnIoClose(infile);
  if (scoremat == NULL) {
    ErrPostEx(SEV_WARNING, 0, 0, "Could not read scoremat from input file\n");
    return FALSE;
  }
  pssm = scoremat->pssm;
  if (pssm == NULL) {
    ErrPostEx(SEV_WARNING, 0, 0,"Scoremat is empty\n");
    PssmWithParametersFree(scoremat);
    return FALSE;
  }
  freqs = pssm->intermediateData;
  if (freqs == NULL) {
    ErrPostEx(SEV_WARNING, 0, 0,"Scoremat doesn't contain intermediate data\n");
    PssmWithParametersFree(scoremat);
    return FALSE;
  }
  if (freqs->freqRatios == NULL) {
    ErrPostEx(SEV_WARNING, 0, 0,
            "Scoremat does not contain position frequencies\n");
    PssmWithParametersFree(scoremat);
    return FALSE;
  }
  if (pssm->numRows != compactSearch->alphabetSize) {
    ErrPostEx(SEV_WARNING, 0, 0, "Wrong alphabet size of %d in "
              "input scoremat\n", pssm->numRows);
    PssmWithParametersFree(scoremat);
    return FALSE;
  }
  if (!pssm->query || !pssm->query->data.ptrvalue) {
    ErrPostEx(SEV_WARNING, 0, 0, "Missing sequence data in input scoremat\n");
    PssmWithParametersFree(scoremat);
    return FALSE;
  }
  bsp = (Bioseq *)(pssm->query->data.ptrvalue);
  if (pssm->numColumns != bsp->length) {
    ErrPostEx(SEV_WARNING, 0, 0, "Different sequence lengths "
              "(%d and %d) in input scoremat\n", pssm->numColumns, bsp->length);
    PssmWithParametersFree(scoremat);
    return FALSE;
  }
  if (pssm->numColumns != compactSearch->qlength) {
    ErrPostEx(SEV_WARNING, 0, 0, "Scoremat sequence length "
              "(%d) does not match query length (%d)\n", 
              pssm->numColumns, compactSearch->qlength);
    PssmWithParametersFree(scoremat);
    return FALSE;
  }
  if (!bsp->seq_data || !ISA_aa(bsp->mol) || 
      bsp->seq_data_type != Seq_code_ncbieaa) {
    ErrPostEx(SEV_WARNING, 0, 0, 
              "Sequence within checkpoint file has the wrong format\n");
    PssmWithParametersFree(scoremat);
    return FALSE;
  }
  BSSeek(bsp->seq_data, 0, SEEK_SET);

  /* verify the input query is the same as the sequence
     within the checkpoint file */

  for (i = 0; i < compactSearch->qlength; i++) {
    c = BSGetByte(bsp->seq_data);
    if (c == EOF) {
      ErrPostEx(SEV_WARNING, 0, 0, "Premature end of sequence data\n");
      PssmWithParametersFree(scoremat);
      return FALSE;
    }
    c = ResToInt((Char)c);
    if (c != compactSearch->query[i]) {
      if (compactSearch->query[i] == Xchar) {
        ErrPostEx(SEV_WARNING, 0, 0, 
                     "Query sequence contains '%c' at position %d; "
                     "if filtering was used, rerun the search with "
                     "filtering turned off ('-F F')\n", getRes(Xchar), i);
      }
      else {
        ErrPostEx(SEV_WARNING, 0, 0, 
                     "Query sequence contains '%c' at position %d, "
                     "while sequence withing checkpoint file contains "
                     "'%c' at this position\n", 
                     getRes(compactSearch->query[i]), i, getRes(c));
      }
      PssmWithParametersFree(scoremat);
      return FALSE;
    }
  }

  /* Read in the residue frequencies, verify they fall
     in the correct range, and verify that the linked list
     of residue frequencies is exactly as long as it should be */

  freq_list = freqs->freqRatios;
  if (pssm->byRow == FALSE) {
    for (i = 0; i < pssm->numColumns; i++) {
      for (j = 0; j < pssm->numRows; j++) {
        if (freq_list == NULL)
          break;
        posSearch->posFreqs[i][j] = freq_list->data.realvalue;

        if (posSearch->posFreqs[i][j] < 0.0) {
          ErrPostEx(SEV_WARNING, 0, 0, "position frequency (%d,%d) "
                    "out of bounds\n", i, j);
          PssmWithParametersFree(scoremat);
          return FALSE;
        }

        freq_list = freq_list->next;
      }
      if (j < pssm->numRows)
        break;
    }
  }
  else {
    for (j = 0; j < pssm->numRows; j++) {
      for (i = 0; i < pssm->numColumns; i++) {
        if (freq_list == NULL)
          break;
        posSearch->posFreqs[i][j] = freq_list->data.realvalue;

        if (posSearch->posFreqs[i][j] < 0.0) {
          ErrPostEx(SEV_WARNING, 0, 0, "position frequency (%d,%d) "
                    "out of bounds\n", i, j);
          PssmWithParametersFree(scoremat);
          return FALSE;
        }

        freq_list = freq_list->next;
      }
      if (i < pssm->numColumns)
        break;
    }
  }

  if (i < pssm->numColumns || j < pssm->numRows) {
    ErrPostEx(SEV_WARNING, 0, 0, "Not enough position "
              "frequencies in input scoremat\n");
    PssmWithParametersFree(scoremat);
    return FALSE;
  }
  if (freq_list != NULL) {
    ErrPostEx(SEV_WARNING, 0, 0, "Too many position "
              "frequencies in input scoremat\n");
    PssmWithParametersFree(scoremat);
    return FALSE;
  }
  PssmWithParametersFree(scoremat);
  return TRUE;
}

static Boolean posReadPosFreqsStandard(posSearchItems * posSearch, compactSearchItems * compactSearch, CharPtr fileName, ValNodePtr * error_return)
{
  FILE * checkFile; /*file in which to take the checkpoint*/
  Int4 length1, length2, c; /*length of query sequence, and an index for it*/
  Char  nextRes; /*next residue in stored copy of the query sequence*/
  Uint1Ptr oldQuery; /*array to hold the query sequence*/

  length1 = compactSearch->qlength;

  checkFile = FileOpen(fileName, "rb");  
  if (NULL == checkFile) {
    BlastConstructErrorMessage("posReadPosFreqsStandard", "Could not open checkpoint file\n", 1, error_return);
    return(FALSE);
  }
  getCkptInt4(length2,checkFile);
  if (length1 != length2) {
    ErrPostEx(SEV_WARNING, 0, 0, "Invalid usage of checkpoint recovery; old query has length %ld, new query has length %ld", (long) length2,  (long) length1);
    BlastConstructErrorMessage("posReadPosFreqsStandard", "Failed to recover data\n", 1, error_return);
    FileClose(checkFile);
    return(FALSE);
  }
  oldQuery = (Uint1Ptr) MemNew(length1 * sizeof(Uint1));
  if (NULL == oldQuery) {
    BlastConstructErrorMessage("posReadPosFreqsStandard", "Failed to reconstruct previous query\n", 1, error_return);
    BlastConstructErrorMessage("posReadPosFreqsStandard", "Failed to recover data\n", 1, error_return);
    FileClose(checkFile);
    return(FALSE);
  }  
  for(c = 0; c < length1; c++) {
    getCkptChar(nextRes, checkFile);
    oldQuery[c] = ResToInt(nextRes);


    if ((oldQuery[c] != compactSearch->query[c]) && (oldQuery[c] != Xchar)) {
                                /* Error massage Added by Natsuhiko */
      if (compactSearch->query[c] == Xchar) {
        ErrPostEx(SEV_WARNING, 0, 0, "\nStored query has a %c at position %ld, while new query has a %c there.\n%c appears in query sequence: The query could be filtered. Run with \"-F F\" option to turn the filter off.",getRes(oldQuery[c]), (long) c, getRes(compactSearch->query[c]),
                  getRes(compactSearch->query[c]));
      }
      else{
      ErrPostEx(SEV_WARNING, 0, 0, "Stored query has a %c at position %ld, while new query has a %c there",getRes(oldQuery[c]), (long) c, getRes(compactSearch->query[c]));
      }

      BlastConstructErrorMessage("posReadPosFreqsStandard", "Failed to recover data\n", 1, error_return);
      MemFree(oldQuery);
      FileClose(checkFile);
      return(FALSE);
    }
    if ((oldQuery[c] != compactSearch->query[c]) && (Xchar==oldQuery[c])) {
      ErrPostEx(SEV_WARNING, 0, 0, "Stored query has a %c at position %ld, while new query has a %c there\n%c appears in the stored query: The stored query may be filtered. Run blastpgp with \"-F F\" option to turn the filter off",getRes(oldQuery[c]), (long) c,
                getRes(compactSearch->query[c]), getRes(oldQuery[c])); }

  }
  getCkptFreqMatrix(posSearch->posFreqs,length1,compactSearch->alphabetSize,checkFile);
  MemFree(oldQuery);
  FileClose(checkFile);
  return(TRUE);
}

/*Read a checkpoint from the end of a previous PSI-BLAST round, get
query length, query, and position-specific target frequencies.
Returns TRUE if checkpoint was read sucessfully and FALSE otherwise. */
Boolean LIBCALL  posReadCheckpoint(posSearchItems * posSearch, compactSearchItems * compactSearch, CharPtr fileName, Int4 scorematInput, ValNodePtr * error_return)
{
  Int4 length1;    /*length of query sequence*/
  Int4 i,j;      /*indices for position and character in alphabet*/
  Boolean FreqsRead;

  BlastConstructErrorMessage("posReadCheckpoint", "Attempting to recover data from previous checkpoint\n", 1, error_return);
  length1 = compactSearch->qlength;

  /* allocate memory for the PSSMs and position frequency matrix */

  posSearch->posMatrix = (BLAST_Score **) MemNew((length1 + 1) * sizeof(BLAST_Score *));
  posSearch->posPrivateMatrix = (BLAST_Score **) MemNew((length1 + 1) * sizeof(BLAST_Score *));
  posSearch->posFreqs = (Nlm_FloatHi **) MemNew((length1 + 1) * sizeof(Nlm_FloatHi *));
  ASSERT(compactSearch->standardMatrixName);
  posSearch->stdFreqRatios =
           PSIMatrixFrequencyRatiosNew(compactSearch->standardMatrixName);
  if ((NULL == posSearch->posMatrix) || (NULL == posSearch->posPrivateMatrix) || (NULL == posSearch->posFreqs)) {

    BlastConstructErrorMessage("posReadCheckpoint", "Failed to allocate position-specific score matrix", 1, error_return);
    BlastConstructErrorMessage("posReadCheckpoint", "Failed to recover data\n", 1, error_return);
    return(FALSE);
  }
  for(i = 0; i <= length1; i++) {
    posSearch->posMatrix[i] = (BLAST_Score *) MemNew(compactSearch->alphabetSize * sizeof(BLAST_Score));
    posSearch->posPrivateMatrix[i] = (BLAST_Score *) MemNew(compactSearch->alphabetSize * sizeof(BLAST_Score));
    posSearch->posFreqs[i] = (Nlm_FloatHi *) MemNew(compactSearch->alphabetSize * sizeof(Nlm_FloatHi));

    if ((NULL == posSearch->posMatrix[i]) || (NULL == posSearch->posPrivateMatrix[i]) || (NULL == posSearch->posFreqs[i])) {
      BlastConstructErrorMessage("posReadCheckpoint", "Failed to allocate position-specific score matrix", 1, error_return);
      BlastConstructErrorMessage("posReadCheckpoint", "Failed to recover data\n", 1, error_return);
      return(FALSE);
    }
    for(j = 0; j < compactSearch->alphabetSize; j++) {
      posSearch->posFreqs[i][j] = 0.0;
    }
  }

  if (scorematInput == NO_SCOREMAT_IO) {
    FreqsRead = posReadPosFreqsStandard(posSearch, compactSearch, 
                      fileName, error_return);
  }
  else {
    FreqsRead = posReadPosFreqsScoremat(posSearch, compactSearch, 
                      fileName, scorematInput, error_return);
  }
  if (FreqsRead != TRUE) {
    BlastConstructErrorMessage("posReadCheckpoint", "Data recovery failed\n", 
                                                1, error_return);
    return(FALSE);
  }
  posFreqsToInformation(posSearch,compactSearch);
  posFreqsToMatrix(posSearch,compactSearch);
  posScaling(posSearch, compactSearch);
  BlastConstructErrorMessage("posReadCheckpoint", "Data recovered successfully\n", 1, error_return);
  return(TRUE);
}

/* Two routines taken from */
/* "p2c"  Copyright (C) 1989, 1990, 1991 Free Software Foundation.
 * By Dave Gillespie, daveg@csvax.cs.caltech.edu.  Version --VERSION--.
 * This file may be copied, modified, etc. in any way.  It is not restricted
 * by the licence agreement accompanying p2c itself.
 */


/* Check if at end of file, using Pascal "eof" semantics.  End-of-file for
   stdin is broken; remove the special case for it to be broken in a
   different way. */

static Int4 P_eof(FILE *f) 
{
    register Int4 ch;

    if (feof(f))
	return 1;
    if (f == stdin)
	return 0;    /* not safe to look-ahead on the keyboard! */
    ch = getc(f);
    if (ch == EOF)
	return 1;
    ungetc(ch, f);
    return 0;
}

static Boolean isBlankChar(Char thisChar)
{
  return((thisChar == ' ') || (thisChar == '\t') || (thisChar == '\n') ||
          (thisChar == '\t'));
}


/*preprocess alignment checkpoint file to find number of sequences and
number of blocks. Return number of blocks as return value
and number of sequences through a reference parameter*/
static Int4 posFindAlignmentDimensions(char * fileName, Int4 *numSeqs, ValNodePtr * error_return)
{
  FILE *checkFile;  /*checkpoint file*/
  Char nextLine[ALIGN_LINE_LENGTH];  /*line read in*/
  Boolean foundBlankLine; /*have we found a blank line yet*/
  Int4 numBlocks;  /*number of blocks to be returned*/
  Int4 testCountSeqs; /*counts number of sequences in each block
                        to ensure that each block has the same
                        number of sequences*/
   
  BlastConstructErrorMessage("posFindAlignmentDimensions", "Attempting to recover data from multiple alignment file\n", 1, error_return);
  checkFile = FileOpen(fileName, "rb");  
  if (NULL == checkFile) {
    ErrPostEx(SEV_WARNING, 0, 0, "\nCould not open alignment checkpoint file");
    BlastConstructErrorMessage("posFindAlignmentDimensions", "Could not open alignment checkpoint file\n", 1, error_return);
    return(0);
  }
  do {
    fgets(nextLine, ALIGN_LINE_LENGTH,checkFile);
  } while (isBlankChar(nextLine[0]));
  foundBlankLine = FALSE;
  *numSeqs = 1;
  numBlocks = 0;
  while (!P_eof(checkFile) && (!foundBlankLine)) {
    fgets(nextLine, ALIGN_LINE_LENGTH,checkFile);
    if (!(isBlankChar(nextLine[0])))
      (*numSeqs)++;
    else
      foundBlankLine = TRUE;
  }
  numBlocks = 1;
  while(!P_eof(checkFile)) {
    do {
      fgets(nextLine, ALIGN_LINE_LENGTH,checkFile);    
    } while((!P_eof(checkFile)) && (isBlankChar(nextLine[0])));
    if (!P_eof(checkFile)) {
      numBlocks++;
      testCountSeqs = 0;
    }    
    do {
      fgets(nextLine, ALIGN_LINE_LENGTH,checkFile);    
      testCountSeqs++;
    } while((!P_eof(checkFile)) && !(isBlankChar(nextLine[0])));
    if (!(isBlankChar(nextLine[0])))
      testCountSeqs++;
    if (testCountSeqs != (*numSeqs)) {
      ErrPostEx(SEV_WARNING, 0, 0, "\nInconsistent number of sequences across alignment blocks, first block has %d while block %d has %d sequences",(*numSeqs), numBlocks, testCountSeqs);
      BlastConstructErrorMessage("posFindAlignmentDimensions", "Could not read alignment due to different number of sequences in different blocks\n", 1, error_return);
      FileClose(checkFile);
      return(0);
    }
  }

  FileClose(checkFile);
  return(numBlocks);
}

/*Is thisChar possibly part of an alignment?*/
static Boolean isProteinChar(Char thisChar)
{

  return(((thisChar >= 'A') && (thisChar <= 'Z')) ||
         ((thisChar >= 'a') && (thisChar <= 'z')) ||
         ('-' == thisChar));
}


/*preprocess alignment checkpoint file to find the
start column and end column of each alignment block.
As a consequece the length of
the alignment can be computed and it is returned*/
static Int4 posPreprocessAlignment(char * fileName, Int4 numSeqs, Int4 numBlocks, Int4 * numCols, ValNodePtr * error_return)
{
  FILE *checkFile;  /*checkpoint file*/
  char nextLine[ALIGN_LINE_LENGTH];  /*line read in*/
  Int4 alignLength; /*number of columns in alignment, to be returned*/
  Int4 charIndex; /*index over characters in a row*/
  Int4 blockIndex; /*index for the blocks in the alignment file*/
  Int4 seqIndex;
   
  checkFile = FileOpen(fileName, "rb");  
  if (NULL == checkFile) {
    ErrPostEx(SEV_WARNING, 0, 0, "\nCould not open alignment checkpoint file");
    BlastConstructErrorMessage("posPreprocessAlignment", "Could not open alignment checkpoint file\n", 1, error_return);
    return(0);
  }

  blockIndex = 0;
  alignLength= 0;
  while (!P_eof(checkFile)) {
    do {
      fgets(nextLine, ALIGN_LINE_LENGTH,checkFile);
    } while (isBlankChar(nextLine[0])); /*line belongs to query*/
    charIndex = 0;
    while(!(isBlankChar(nextLine[charIndex])))
      charIndex++;
    while(isBlankChar(nextLine[charIndex]))
      charIndex++;
    numCols[blockIndex] = 0;
    while (isProteinChar(nextLine[charIndex])){
      alignLength++;
      charIndex++;
      numCols[blockIndex]++;
    }
    /*skip over other sequences*/
    for (seqIndex = 0; seqIndex < numSeqs; seqIndex++) 
      fgets(nextLine, ALIGN_LINE_LENGTH,checkFile);
    blockIndex++;
  }
  FileClose(checkFile);
  return(alignLength);
}

/*Find the index of the sequence in the multiple alignment that
  matches the query sequence; if non match return -1*/
static Int4 findQuery(posDesc ** alignArray, compactSearchItems * compactSearch, Int4 numSeqs, Int4 alignLength)
{
   Uint1Ptr query; /*query sequence*/
   Int4 qlength;  /*length of query sequence*/
   Int4 seqIndex; /*index over sequences*/
   Int4 i;        /*index within a sequence*/
   Int4 queryIndex;  /*index within query*/
   Char thisRes;

   query = compactSearch->query;
   qlength = compactSearch->qlength;
   for(seqIndex = 0; seqIndex < numSeqs; seqIndex++) {
     i = 0;
     queryIndex = 0;
     while ((queryIndex < qlength) && (i < alignLength)) {
       if ('-' == alignArray[seqIndex][i].letter)
         i++;
       else {
         /*Need to keep lower-case letters*/
         thisRes = getRes(query[queryIndex]);
         /* Selenocysteines are replaced by X's in query; test for this
            possibility */
         if ((alignArray[seqIndex][i].letter == 'U' ||
             alignArray[seqIndex][i].letter == 'u') &&
             thisRes == 'X')
            thisRes = alignArray[seqIndex][i].letter;
            
         if ((thisRes != (alignArray[seqIndex][i].letter + 'A' - 'a')) &&
             (thisRes != alignArray[seqIndex][i].letter))
           /*character mismatch*/
           break;
         else {
           queryIndex++;
           i++;
         }
       }
     }
     if (queryIndex == qlength) {
       while (i < alignLength) {
         /*chew up gaps at end of alignment sequence*/
         if ('-' != alignArray[seqIndex][i].letter)
           break;
         i++;
       }
       /*found a match! */
       return(seqIndex);
     }
     else
       /*alignment string is prefix of query*/
       continue;
   }
   return (-1);

}


static posDesc** posReadAlignment(compactSearchItems *compactSearch, char * fileName, Int4 numSeqs, Int4 numBlocks, Int4 alignLength, Int4 * numCols,
ValNodePtr * error_return)
{
  FILE *checkFile; /*checkpoint file to read*/
  Char nextline[ALIGN_LINE_LENGTH];
  Int4 blockIndex;
  Int4 linePos; /* moving index for a line*/
  Int4 alignPos; /*placeholder for position alignment*/
  Int4 base; /*base for this block*/
  posDesc **returnArray; /*array of sequences to retunr*/
  Int4 i,j; /*loop indices*/
  Int4 temp; /*temporary character for swapping sequences*/
  Int4 queryIndex; /*which sequnec in the alignment is the query*/

  checkFile = FileOpen(fileName, "rb");  
  if (NULL == checkFile) {
    BlastConstructErrorMessage("posReadAlignment", "Could not open alignment checkpoint file\n", 1, error_return);
    ErrPostEx(SEV_WARNING, 0, 0, "\nCould not open alignment checkpoint file");
  }
  returnArray = (posDesc**) MemNew(numSeqs * sizeof(posDesc *));
  if (NULL == returnArray)
    exit(EXIT_FAILURE);
  for (i = 0; i < numSeqs; i++) {
    returnArray[i] = (posDesc *) MemNew(alignLength * sizeof(posDesc));
    if (NULL == returnArray[i])
      exit(EXIT_FAILURE);
    for(j = 0; j < alignLength; j++) {
      returnArray[i][j].letter = UNUSED;
      returnArray[i][j].used = FALSE;
    }
  }  
  alignPos = 0;
  base = 0;
  for(blockIndex = 0; blockIndex < numBlocks; blockIndex++){
    for(i = 0; i < numSeqs; i++) {
      do {
	fgets(nextline, ALIGN_LINE_LENGTH,checkFile);    
      } while(isBlankChar(nextline[0]));
      linePos = 0;
      while(!isBlankChar(nextline[linePos]))
        linePos++;
      while(isBlankChar(nextline[linePos]))
        linePos++;
      alignPos = base;
      while (alignPos < (base + numCols[blockIndex])) {
        if (!isProteinChar(nextline[linePos])) {
          BlastConstructErrorMessage("posReadAlignment", "Invalid character or wrong number of characters in a sequence\n", 1, error_return);
          ErrPostEx(SEV_WARNING, 0, 0, "\nInvalid character or wrong number of characters in sequence index %d\n", i+1);
        }
	returnArray[i][alignPos].letter = nextline[linePos];
	returnArray[i][alignPos].used = TRUE;
        alignPos++;
        linePos++;
      }
    }
    base += numCols[blockIndex];
  }
  FileClose(checkFile);
  queryIndex = findQuery(returnArray, compactSearch, numSeqs, alignLength);
  if (-1 == queryIndex) {
    BlastConstructErrorMessage("posReadAlignment", "None of the alignment sequences equals the query sequence\n", 1, error_return);
    ErrPostEx(SEV_WARNING, 0, 0, "\nNone of the alignment sequences equals the query sequence");
    BlastConstructErrorMessage("posReadAlignment", "Cannot recover alignment checkpoint\n", 1, error_return);
    ErrPostEx(SEV_WARNING, 0, 0, "\nCannot recover alignment checkpoint");
    exit(EXIT_FAILURE);
  }
  else {
    if (queryIndex > 0) {
      /*swap query with first sequence in alignment*/
      for (alignPos = 0; alignPos < alignLength; alignPos++) {
        temp = returnArray[0][alignPos].letter;
        returnArray[0][alignPos].letter = returnArray[queryIndex][alignPos].letter;
        returnArray[queryIndex][alignPos].letter = temp;
      }
    }
  }
  return(returnArray);
}

static void posProcessAlignment(posSearchItems *posSearch, compactSearchItems *compactSearch, char * fileName, Int4 numSeqs, Int4 numBlocks, Int4 alignLength, Int4 * numCols, ValNodePtr * error_return)
{
  Int4 queryPos, alignPos, linePos; /*placeholder for position in query and
                             alignment*/
  Int4 *queryDesc; /*position correspondence between alignment and query*/
  Int4 seqIndex; /*counter for sequences*/
  posDesc ** alignArray;
  Int4 queryLength; /*length of query sequence*/

  alignArray = posReadAlignment(compactSearch, fileName, numSeqs,  numBlocks, alignLength, numCols, error_return);
  queryDesc = (Int4 *) MemNew(alignLength * sizeof(Int4));
  if (NULL == queryDesc)
    exit(EXIT_FAILURE);
  for(alignPos = 0; alignPos < alignLength; alignPos++)
    queryDesc[alignPos] = GAP_HERE;
  alignPos = 0;
  queryPos = 0;
  for(linePos = 0; linePos < alignLength; linePos++) {
    if (alignArray[0][linePos].letter == '-') 
      queryDesc[alignPos] = GAP_HERE;
    else {
      queryDesc[alignPos] = queryPos;
      if ((alignArray[0][linePos].letter >= 'A' ) && (alignArray[0][linePos].letter <= 'Z')) {
	posSearch->posDescMatrix[0][queryPos].letter = ResToInt(alignArray[0][linePos].letter);
	posSearch->posDescMatrix[0][queryPos].used = TRUE;
	posSearch->posDescMatrix[0][queryPos].e_value = compactSearch->ethresh/2;
	posSearch->posDescMatrix[0][queryPos].leftExtent = 0;
	posSearch->posDescMatrix[0][queryPos].rightExtent = compactSearch->qlength - 1;
	posSearch->posC[queryPos][ResToInt(alignArray[0][linePos].letter)]++;
	posSearch->posCount[queryPos]++;
      }
      else {
	posSearch->posDescMatrix[0][queryPos].used = FALSE;
	posSearch->posDescMatrix[0][queryPos].letter = ResToInt(alignArray[0][linePos].letter + 'A' - 'a');
      }
      queryPos++;
    }
    alignPos++;
  }
  queryLength = queryPos;
  for(seqIndex = 1; seqIndex < numSeqs; seqIndex++) {
    for(linePos = 0; linePos < alignLength; linePos++) {
      if (queryDesc[linePos] != GAP_HERE) {
	if (!(posSearch->posDescMatrix[0][queryDesc[linePos]].used)) {
	  /*mark column as not participating*/
	  posSearch->posDescMatrix[seqIndex][queryDesc[linePos]].used = FALSE;
	}
	else {
	  posSearch->posDescMatrix[seqIndex][queryDesc[linePos]].letter = ResToInt(alignArray[seqIndex][linePos].letter);
	  posSearch->posDescMatrix[seqIndex][queryDesc[linePos]].used = TRUE;
	  posSearch->posDescMatrix[seqIndex][queryDesc[linePos]].e_value = compactSearch->ethresh/2;
	  posSearch->posDescMatrix[seqIndex][queryDesc[linePos]].leftExtent = 0;
	  posSearch->posDescMatrix[seqIndex][queryDesc[linePos]].rightExtent = compactSearch->qlength;
	}
      }
    }
  }
  /*make terminal gaps unused*/
  for(seqIndex = 1; seqIndex < numSeqs; seqIndex++) {
    linePos = 0;
    while((linePos < queryLength) && (posSearch->posDescMatrix[seqIndex][linePos].letter == GAP_CHAR)) {
      posSearch->posDescMatrix[seqIndex][linePos].used = FALSE;
      linePos++;
    }
    linePos = queryLength - 1;
    while((linePos >= 0) && (posSearch->posDescMatrix[seqIndex][linePos].letter == GAP_CHAR)) {
      posSearch->posDescMatrix[seqIndex][linePos].used = FALSE;
      linePos--;
    }
  }
  BlastConstructErrorMessage("posProcessAlignment", "Alignment recovered successfully\n", 1, error_return);
}


/* Top-level routine to compute position-specific matrix, when used for
one round to recover from a multiple alignment checkpoint. */
Int4Ptr * LIBCALL BposComputation(posSearchItems *posSearch, BlastSearchBlkPtr
   search, compactSearchItems * compactSearch, Char *ckptFileName, 
   Char *takeCkptFileName, Int4 scorematOutput, Bioseq *query_bsp,
   Int4 gap_open, Int4 gap_extend, ValNodePtr * error_return)
{
  Int4 numSeqs, numBlocks, alignLength; /*number of sequences, number of pieces
                        of alignment, total length of the alignment*/
  Int4 *numCols;  /*number of columns within each block*/

  search->posConverged = FALSE;

  numBlocks = posFindAlignmentDimensions(ckptFileName, &numSeqs, error_return);
  if (0 == numBlocks) {
    ErrPostEx(SEV_WARNING, 0, 0, "\nCould not recover block structure from checkpoint");
    BlastConstructErrorMessage("BposComputation", "Cannot recover alignment checkpoint\n", 1, error_return);
    return(NULL);
  }
  numCols = (Int4 *) MemNew(numBlocks * sizeof(Int4));
  if (NULL == numCols)
    exit(EXIT_FAILURE);
  alignLength = posPreprocessAlignment(ckptFileName,  numSeqs, numBlocks,  numCols, error_return);
  if (0 == alignLength) {
    ErrPostEx(SEV_WARNING, 0, 0, "\nCould not recover alignment structure from checkpoint");
    BlastConstructErrorMessage("BposComputation", "Cannot recover alignment checkpoint\n", 1, error_return);
    return(NULL);
  } 
  posAllocateMemory(posSearch, compactSearch->alphabetSize, compactSearch->qlength, numSeqs);
  posProcessAlignment(posSearch, compactSearch, ckptFileName, numSeqs,  numBlocks, alignLength, numCols, error_return);
  MemFree(numCols);
  posSearch->posNumSequences = numSeqs;
  posPurgeMatches(posSearch, compactSearch);
  posComputeExtents(posSearch, compactSearch);
  posComputeSequenceWeights(posSearch, compactSearch, 1.0);
  posCheckWeights(posSearch, compactSearch);
  posSearch->posFreqs = posComputePseudoFreqs(posSearch, compactSearch, TRUE);
  if (NULL == search->sbp->posFreqs)
    search->sbp->posFreqs =  allocatePosFreqs(compactSearch->qlength, compactSearch->alphabetSize);
  copyPosFreqs(posSearch->posFreqs,search->sbp->posFreqs, compactSearch->qlength, compactSearch->alphabetSize);
  if (NULL != takeCkptFileName) {
    if (scorematOutput == NO_SCOREMAT_IO)
      posTakeCheckpoint(posSearch, compactSearch, takeCkptFileName, error_return);
    else
      posTakeScoremat(posSearch, compactSearch, takeCkptFileName, scorematOutput, query_bsp, gap_open, gap_extend, error_return);
  }
  posFreqsToMatrix(posSearch,compactSearch);
  posScaling(posSearch, compactSearch);
  return posSearch->posMatrix;
}

/****************************************************************************/
/* PLEASE NOTE: The following structure and the PSIMatrixFrequencyRatios*
 * functions have been copied and renamed from
 * algo/blast/core/matrix_freq_ratios.[hc] to eliminate a dependency from the
 * ncbitool library to the blast library. 
 */
#ifndef BLOSUM62_20A_SCALE_MULTIPLIER
#define BLOSUM62_20A_SCALE_MULTIPLIER 0.9666
#endif

#ifndef BLOSUM62_20B_SCALE_MULTIPLIER
#define BLOSUM62_20B_SCALE_MULTIPLIER 0.9344
#endif


/*underlying Amino Acid frequecies for specific score matrices according
  to Jorja Henikoff for BLOSUMs*/
static Nlm_FloatHi BLOSUM45_FREQUENCIES[PROTEIN_ALPHABET] = {
  0.000, 0.078, 0.000, 0.024, 0.050, 0.058, 0.046, 0.075, 0.025, 0.062, 0.060,
  0.094, 0.025, 0.042, 0.043, 0.036, 0.052, 0.061, 0.052, 0.070, 0.013, 0.000,
  0.034, 0.000, 0.000, 0.000
};

static Nlm_FloatHi BLOSUM62_FREQUENCIES[PROTEIN_ALPHABET] = {
  0.000,   0.07422, 0.000,   0.02469, 0.05363, 0.05431, 0.04742, 0.07415,
  0.02621, 0.06792, 0.05816, 0.09891, 0.02499, 0.04465, 0.03854, 0.03426,
  0.05161, 0.05723, 0.05089, 0.07292, 0.01303, 0.000,   0.03228, 0.000,
  0.000,   0.000
};


static Nlm_FloatHi BLOSUM80_FREQUENCIES[PROTEIN_ALPHABET] = {
  0.000, 0.073, 0.000, 0.029, 0.054, 0.055, 0.047, 0.077, 0.025, 0.067, 0.055,
  0.100, 0.024, 0.043, 0.038, 0.034, 0.050, 0.057, 0.050, 0.073, 0.014, 0.000,
  0.035, 0.000, 0.000, 0.000
};

/*underlying frequency ratios for BLOSUM62 as determined by Stephen Altschul;
  Stephen and Jorja Henikoff used different number for B,Z,X*/
static Nlm_FloatHi BLOSUM62_FREQRATIOS[PROTEIN_ALPHABET][PROTEIN_ALPHABET] = {
  {0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.250},
  {0.000, 3.903, 0.565, 0.868, 0.545, 0.741, 0.465, 1.057, 0.569, 0.632,
   0.775, 0.602, 0.723, 0.588, 0.754, 0.757, 0.613, 1.472, 0.984, 0.936,
   0.416, 0.750, 0.543, 0.747, 0.000, 0.250},
  {0.000, 0.565, 4.438, 0.345, 4.743, 1.335, 0.324, 0.739, 0.925, 0.334,
   0.855, 0.297, 0.405, 4.071, 0.554, 0.944, 0.703, 1.058, 0.826, 0.351,
   0.253, 0.750, 0.409, 1.184, 0.000, 0.250},
  {0.000, 0.868, 0.345, 19.577, 0.301, 0.286, 0.439, 0.420, 0.355, 0.653,
   0.349, 0.642, 0.611, 0.398, 0.380, 0.366, 0.309, 0.738, 0.741, 0.756,
   0.450, 0.750, 0.434, 0.317, 0.000, 0.250},
  {0.000, 0.545, 4.743, 0.301, 7.398, 1.688, 0.299, 0.634, 0.679, 0.339,
   0.784, 0.287, 0.346, 1.554, 0.599, 0.897, 0.573, 0.913, 0.695, 0.337,
   0.232, 0.750, 0.346, 1.382, 0.000, 0.250},
  {0.000, 0.741, 1.335, 0.286, 1.688, 5.470, 0.331, 0.481, 0.960, 0.331,
   1.308, 0.373, 0.500, 0.911, 0.679, 1.902, 0.961, 0.950, 0.741, 0.429,
   0.374, 0.750, 0.496, 4.090, 0.000, 0.250},
  {0.000, 0.465, 0.324, 0.439, 0.299, 0.331, 8.129, 0.341, 0.652, 0.946,
   0.344, 1.155, 1.004, 0.354, 0.287, 0.334, 0.381, 0.440, 0.482, 0.745,
   1.374, 0.750, 2.769, 0.332, 0.000, 0.250},
  {0.000, 1.057, 0.739, 0.420, 0.634, 0.481, 0.341, 6.876, 0.493, 0.275,
   0.589, 0.284, 0.396, 0.864, 0.477, 0.539, 0.450, 0.904, 0.579, 0.337,
   0.422, 0.750, 0.349, 0.503, 0.000, 0.250},
  {0.000, 0.569, 0.925, 0.355, 0.679, 0.960, 0.652, 0.493, 13.506, 0.326,
   0.779, 0.381, 0.584, 1.222, 0.473, 1.168, 0.917, 0.737, 0.557, 0.339,
   0.444, 0.750, 1.798, 1.040, 0.000, 0.250},
  {0.000, 0.632, 0.334, 0.653, 0.339, 0.331, 0.946, 0.275, 0.326, 3.998,
   0.396, 1.694, 1.478, 0.328, 0.385, 0.383, 0.355, 0.443, 0.780, 2.417,
   0.409, 0.750, 0.630, 0.351, 0.000, 0.250},
  {0.000, 0.775, 0.855, 0.349, 0.784, 1.308, 0.344, 0.589, 0.779, 0.396,
   4.764, 0.428, 0.625, 0.940, 0.704, 1.554, 2.077, 0.932, 0.793, 0.457,
   0.359, 0.750, 0.532, 1.403, 0.000, 0.250},
  {0.000, 0.602, 0.297, 0.642, 0.287, 0.373, 1.155, 0.284, 0.381, 1.694,
   0.428, 3.797, 1.994, 0.310, 0.371, 0.477, 0.474, 0.429, 0.660, 1.314,
   0.568, 0.750, 0.692, 0.413, 0.000, 0.250},
  {0.000, 0.723, 0.405, 0.611, 0.346, 0.500, 1.004, 0.396, 0.584, 1.478,
   0.625, 1.994, 6.481, 0.474, 0.424, 0.864, 0.623, 0.599, 0.794, 1.269,
   0.610, 0.750, 0.708, 0.641, 0.000, 0.250},
  {0.000, 0.588, 4.071, 0.398, 1.554, 0.911, 0.354, 0.864, 1.222, 0.328,
   0.940, 0.310, 0.474, 7.094, 0.500, 1.001, 0.859, 1.232, 0.984, 0.369,
   0.278, 0.750, 0.486, 0.946, 0.000, 0.250},
  {0.000, 0.754, 0.554, 0.380, 0.599, 0.679, 0.287, 0.477, 0.473, 0.385,
   0.704, 0.371, 0.424, 0.500, 12.838, 0.641, 0.481, 0.755, 0.689, 0.443,
   0.282, 0.750, 0.363, 0.664, 0.000, 0.250},
  {0.000, 0.757, 0.944, 0.366, 0.897, 1.902, 0.334, 0.539, 1.168, 0.383,
   1.554, 0.477, 0.864, 1.001, 0.641, 6.244, 1.406, 0.966, 0.791, 0.467,
   0.509, 0.750, 0.611, 3.582, 0.000, 0.250},
  {0.000, 0.613, 0.703, 0.309, 0.573, 0.961, 0.381, 0.450, 0.917, 0.355,
   2.077, 0.474, 0.623, 0.859, 0.481, 1.406, 6.666, 0.767, 0.678, 0.420,
   0.395, 0.750, 0.556, 1.133, 0.000, 0.250},
  {0.000, 1.472, 1.058, 0.738, 0.913, 0.950, 0.440, 0.904, 0.737, 0.443,
   0.932, 0.429, 0.599, 1.232, 0.755, 0.966, 0.767, 3.843, 1.614, 0.565,
   0.385, 0.750, 0.557, 0.956, 0.000, 0.250},
  {0.000, 0.984, 0.826, 0.741, 0.695, 0.741, 0.482, 0.579, 0.557, 0.780,
   0.793, 0.660, 0.794, 0.984, 0.689, 0.791, 0.678, 1.614, 4.832, 0.981,
   0.431, 0.750, 0.573, 0.761, 0.000, 0.250},
  {0.000, 0.936, 0.351, 0.756, 0.337, 0.429, 0.745, 0.337, 0.339, 2.417,
   0.457, 1.314, 1.269, 0.369, 0.443, 0.467, 0.420, 0.565, 0.981, 3.692,
   0.374, 0.750, 0.658, 0.444, 0.000, 0.250},
  {0.000, 0.416, 0.253, 0.450, 0.232, 0.374, 1.374, 0.422, 0.444, 0.409,
   0.359, 0.568, 0.610, 0.278, 0.282, 0.509, 0.395, 0.385, 0.431, 0.374,
   38.108, 0.750, 2.110, 0.426, 0.000, 0.250},
  {0.000, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750,
   0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750,
   0.750, 0.750, 0.750, 0.750, 0.000, 0.250},
  {0.000, 0.543, 0.409, 0.434, 0.346, 0.496, 2.769, 0.349, 1.798, 0.630,
   0.532, 0.692, 0.708, 0.486, 0.363, 0.611, 0.556, 0.557, 0.573, 0.658,
   2.110, 0.750, 9.832, 0.541, 0.000, 0.250},
  {0.000, 0.747, 1.184, 0.317, 1.382, 4.090, 0.332, 0.503, 1.040, 0.351,
   1.403, 0.413, 0.641, 0.946, 0.664, 3.582, 1.133, 0.956, 0.761, 0.444,
   0.426, 0.750, 0.541, 3.893, 0.000, 0.250},
  {0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.250},
  {0.250, 0.250, 0.250, 0.250, 0.250, 0.250, 0.250, 0.250, 0.250, 0.250,
   0.250, 0.250, 0.250, 0.250, 0.250, 0.250, 0.250, 0.250, 0.250, 0.250,
   0.250, 0.250, 0.250, 0.250, 0.250, 1.333},
};

static Nlm_FloatHi PAM30_FREQRATIOS[PROTEIN_ALPHABET][PROTEIN_ALPHABET] = {
  {0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.003},
  {0.000, 7.789, 0.302, 0.108, 0.317, 0.453, 0.057, 0.576, 0.083, 0.199,
   0.095, 0.115, 0.189, 0.285, 0.593, 0.235, 0.091, 0.875, 0.827, 0.477,
   0.010, 0.750, 0.070, 0.358, 0.000, 0.003},
  {0.000, 0.302, 8.118, 0.015, 8.456, 1.472, 0.027, 0.338, 0.664, 0.126,
   0.441, 0.049, 0.034, 7.726, 0.102, 0.368, 0.082, 0.604, 0.353, 0.067,
   0.032, 0.750, 0.114, 0.991, 0.000, 0.003},
  {0.000, 0.108, 0.015, 27.591, 0.008, 0.008, 0.012, 0.042, 0.078, 0.119,
   0.008, 0.006, 0.009, 0.024, 0.063, 0.008, 0.068, 0.379, 0.068, 0.132,
   0.005, 0.750, 0.258, 0.008, 0.000, 0.003},
  {0.000, 0.317, 8.456, 0.008, 14.236, 2.342, 0.006, 0.326, 0.271, 0.080,
   0.220, 0.014, 0.023, 1.756, 0.068, 0.427, 0.029, 0.286, 0.200, 0.065,
   0.005, 0.750, 0.020, 1.508, 0.000, 0.003},
  {0.000, 0.453, 1.472, 0.008, 2.343, 13.663, 0.008, 0.232, 0.181, 0.150,
   0.226, 0.043, 0.092, 0.464, 0.153, 1.507, 0.043, 0.226, 0.130, 0.110,
   0.003, 0.750, 0.057, 8.365, 0.000, 0.003},
  {0.000, 0.057, 0.027, 0.012, 0.006, 0.008, 21.408, 0.044, 0.132, 0.473,
   0.009, 0.416, 0.250, 0.051, 0.036, 0.011, 0.042, 0.114, 0.051, 0.067,
   0.213, 0.750, 1.786, 0.009, 0.000, 0.003},
  {0.000, 0.576, 0.338, 0.042, 0.326, 0.232, 0.044, 9.314, 0.048, 0.024,
   0.086, 0.028, 0.057, 0.351, 0.122, 0.094, 0.040, 0.553, 0.135, 0.149,
   0.006, 0.750, 0.009, 0.172, 0.000, 0.003},
  {0.000, 0.083, 0.664, 0.078, 0.271, 0.181, 0.132, 0.048, 22.927, 0.044,
   0.120, 0.123, 0.028, 1.119, 0.248, 1.359, 0.587, 0.132, 0.086, 0.115,
   0.081, 0.750, 0.324, 0.695, 0.000, 0.003},
  {0.000, 0.199, 0.126, 0.119, 0.080, 0.150, 0.473, 0.024, 0.044, 18.632,
   0.127, 0.643, 0.793, 0.179, 0.053, 0.070, 0.159, 0.097, 0.444, 1.938,
   0.009, 0.750, 0.119, 0.115, 0.000, 0.003},
  {0.000, 0.095, 0.441, 0.008, 0.220, 0.226, 0.009, 0.086, 0.120, 0.127,
   9.988, 0.061, 0.563, 0.697, 0.106, 0.384, 1.111, 0.260, 0.336, 0.046,
   0.018, 0.750, 0.042, 0.295, 0.000, 0.003},
  {0.000, 0.115, 0.049, 0.006, 0.014, 0.043, 0.416, 0.028, 0.123, 0.643,
   0.061, 10.019, 1.242, 0.090, 0.090, 0.185, 0.053, 0.058, 0.103, 0.463,
   0.127, 0.750, 0.095, 0.105, 0.000, 0.003},
  {0.000, 0.189, 0.034, 0.009, 0.023, 0.092, 0.250, 0.057, 0.028, 0.792,
   0.563, 1.242, 46.604, 0.047, 0.065, 0.272, 0.243, 0.161, 0.263, 0.631,
   0.013, 0.750, 0.021, 0.171, 0.000, 0.003},
  {0.000, 0.285, 7.726, 0.024, 1.756, 0.464, 0.051, 0.351, 1.119, 0.179,
   0.697, 0.090, 0.047, 14.647, 0.142, 0.299, 0.144, 0.972, 0.531, 0.070,
   0.064, 0.750, 0.223, 0.392, 0.000, 0.003},
  {0.000, 0.593, 0.102, 0.063, 0.068, 0.153, 0.036, 0.122, 0.248, 0.053,
   0.106, 0.090, 0.065, 0.142, 15.809, 0.376, 0.270, 0.571, 0.240, 0.140,
   0.009, 0.750, 0.010, 0.251, 0.000, 0.003},
  {0.000, 0.235, 0.368, 0.008, 0.427, 1.507, 0.011, 0.094, 1.360, 0.070,
   0.384, 0.185, 0.272, 0.299, 0.376, 18.136, 0.585, 0.168, 0.150, 0.103,
   0.012, 0.750, 0.017, 8.754, 0.000, 0.003},
  {0.000, 0.091, 0.082, 0.068, 0.029, 0.043, 0.042, 0.040, 0.587, 0.159,
   1.111, 0.053, 0.243, 0.144, 0.270, 0.585, 18.926, 0.355, 0.106, 0.074,
   0.517, 0.750, 0.030, 0.279, 0.000, 0.003},
  {0.000, 0.875, 0.604, 0.379, 0.286, 0.226, 0.114, 0.553, 0.132, 0.097,
   0.260, 0.058, 0.161, 0.972, 0.571, 0.168, 0.355, 9.028, 1.145, 0.118,
   0.179, 0.750, 0.097, 0.200, 0.000, 0.003},
  {0.000, 0.827, 0.353, 0.068, 0.200, 0.130, 0.051, 0.135, 0.086, 0.444,
   0.336, 0.103, 0.263, 0.531, 0.240, 0.150, 0.106, 1.145, 11.695, 0.391,
   0.013, 0.750, 0.112, 0.139, 0.000, 0.003},
  {0.000, 0.477, 0.067, 0.132, 0.065, 0.110, 0.067, 0.149, 0.115, 1.938,
   0.046, 0.463, 0.631, 0.070, 0.141, 0.103, 0.074, 0.118, 0.391, 11.609,
   0.005, 0.750, 0.081, 0.107, 0.000, 0.003},
  {0.000, 0.010, 0.032, 0.005, 0.005, 0.003, 0.213, 0.006, 0.081, 0.009,
   0.018, 0.127, 0.013, 0.064, 0.009, 0.012, 0.517, 0.179, 0.013, 0.005,
   88.722, 0.750, 0.173, 0.007, 0.000, 0.003},
  {0.000, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750,
   0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750,
   0.750, 0.750, 0.750, 0.750, 0.000, 0.003},
  {0.000, 0.070, 0.114, 0.258, 0.020, 0.057, 1.786, 0.009, 0.324, 0.119,
   0.042, 0.095, 0.021, 0.223, 0.010, 0.017, 0.030, 0.097, 0.112, 0.081,
   0.173, 0.750, 28.442, 0.039, 0.000, 0.003},
  {0.000, 0.358, 0.991, 0.008, 1.508, 8.365, 0.009, 0.172, 0.695, 0.115,
   0.295, 0.105, 0.171, 0.392, 0.251, 8.754, 0.279, 0.200, 0.139, 0.107,
   0.007, 0.750, 0.039, 8.535, 0.000, 0.003},
  {0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.003},
  {0.003, 0.003, 0.003, 0.003, 0.003, 0.003, 0.003, 0.003, 0.003, 0.003,
   0.003, 0.003, 0.003, 0.003, 0.003, 0.003, 0.003, 0.003, 0.003, 0.003,
   0.003, 0.003, 0.003, 0.003, 0.003, 1.333},
};

static Nlm_FloatHi PAM70_FREQRATIOS[PROTEIN_ALPHABET][PROTEIN_ALPHABET] = {
  {0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.002},
  {0.000, 4.900, 0.605, 0.242, 0.619, 0.771, 0.135, 1.015, 0.221, 0.434,
   0.249, 0.250, 0.376, 0.589, 1.030, 0.466, 0.227, 1.350, 1.327, 0.809,
   0.043, 0.750, 0.154, 0.638, 0.000, 0.002},
  {0.000, 0.605, 5.422, 0.066, 5.890, 2.249, 0.079, 0.649, 1.099, 0.253,
   0.778, 0.120, 0.140, 4.879, 0.274, 0.790, 0.253, 0.955, 0.654, 0.185,
   0.078, 0.750, 0.223, 1.613, 0.000, 0.002},
  {0.000, 0.242, 0.066, 24.836, 0.040, 0.039, 0.055, 0.116, 0.165, 0.246,
   0.038, 0.030, 0.044, 0.097, 0.159, 0.039, 0.151, 0.665, 0.186, 0.274,
   0.022, 0.750, 0.523, 0.039, 0.000, 0.002},
  {0.000, 0.618, 5.890, 0.040, 8.900, 3.357, 0.034, 0.639, 0.600, 0.193,
   0.484, 0.064, 0.103, 2.400, 0.213, 0.922, 0.139, 0.600, 0.442, 0.173,
   0.026, 0.750, 0.080, 2.296, 0.000, 0.002},
  {0.000, 0.771, 2.249, 0.039, 3.357, 8.654, 0.038, 0.492, 0.480, 0.284,
   0.466, 0.117, 0.212, 0.965, 0.347, 2.258, 0.179, 0.479, 0.338, 0.247,
   0.019, 0.750, 0.119, 5.867, 0.000, 0.002},
  {0.000, 0.135, 0.079, 0.055, 0.034, 0.038, 17.455, 0.100, 0.277, 0.846,
   0.041, 0.824, 0.508, 0.131, 0.089, 0.052, 0.099, 0.216, 0.138, 0.208,
   0.452, 0.750, 3.378, 0.044, 0.000, 0.002},
  {0.000, 1.015, 0.649, 0.116, 0.639, 0.492, 0.100, 7.309, 0.145, 0.107,
   0.210, 0.081, 0.144, 0.660, 0.308, 0.237, 0.122, 0.964, 0.370, 0.313,
   0.027, 0.750, 0.044, 0.381, 0.000, 0.002},
  {0.000, 0.221, 1.099, 0.165, 0.600, 0.480, 0.277, 0.145, 16.422, 0.140,
   0.335, 0.259, 0.122, 1.677, 0.493, 2.188, 1.058, 0.327, 0.230, 0.233,
   0.186, 0.750, 0.611, 1.224, 0.000, 0.002},
  {0.000, 0.434, 0.253, 0.246, 0.193, 0.284, 0.846, 0.107, 0.140, 11.749,
   0.266, 1.192, 1.369, 0.322, 0.160, 0.189, 0.301, 0.254, 0.761, 3.004,
   0.044, 0.750, 0.281, 0.242, 0.000, 0.002},
  {0.000, 0.249, 0.778, 0.038, 0.484, 0.466, 0.041, 0.210, 0.335, 0.266,
   7.610, 0.153, 0.948, 1.119, 0.256, 0.722, 1.937, 0.520, 0.620, 0.139,
   0.079, 0.750, 0.101, 0.578, 0.000, 0.002},
  {0.000, 0.250, 0.120, 0.030, 0.064, 0.117, 0.824, 0.081, 0.259, 1.192,
   0.153, 8.228, 2.130, 0.185, 0.199, 0.351, 0.137, 0.149, 0.240, 0.908,
   0.267, 0.750, 0.239, 0.219, 0.000, 0.002},
  {0.000, 0.376, 0.140, 0.044, 0.103, 0.212, 0.508, 0.144, 0.122, 1.369,
   0.948, 2.129, 28.592, 0.182, 0.176, 0.486, 0.488, 0.319, 0.498, 1.133,
   0.060, 0.750, 0.097, 0.332, 0.000, 0.002},
  {0.000, 0.589, 4.879, 0.097, 2.400, 0.965, 0.131, 0.660, 1.677, 0.322,
   1.119, 0.185, 0.182, 7.754, 0.344, 0.637, 0.387, 1.366, 0.899, 0.199,
   0.138, 0.750, 0.390, 0.822, 0.000, 0.002},
  {0.000, 1.030, 0.274, 0.159, 0.213, 0.347, 0.089, 0.308, 0.493, 0.160,
   0.256, 0.199, 0.176, 0.344, 11.871, 0.684, 0.527, 0.979, 0.531, 0.307,
   0.042, 0.750, 0.047, 0.494, 0.000, 0.002},
  {0.000, 0.466, 0.790, 0.039, 0.923, 2.258, 0.052, 0.237, 2.188, 0.189,
   0.722, 0.351, 0.486, 0.637, 0.684, 11.469, 1.024, 0.373, 0.329, 0.232,
   0.055, 0.750, 0.074, 6.272, 0.000, 0.002},
  {0.000, 0.227, 0.253, 0.151, 0.139, 0.179, 0.099, 0.122, 1.058, 0.301,
   1.937, 0.137, 0.488, 0.387, 0.527, 1.024, 13.660, 0.610, 0.284, 0.176,
   0.991, 0.750, 0.086, 0.547, 0.000, 0.002},
  {0.000, 1.350, 0.955, 0.665, 0.600, 0.479, 0.216, 0.964, 0.327, 0.254,
   0.520, 0.149, 0.319, 1.366, 0.979, 0.373, 0.610, 5.204, 1.695, 0.303,
   0.324, 0.750, 0.208, 0.433, 0.000, 0.002},
  {0.000, 1.327, 0.654, 0.186, 0.442, 0.338, 0.138, 0.370, 0.230, 0.761,
   0.620, 0.240, 0.498, 0.899, 0.531, 0.329, 0.284, 1.695, 7.337, 0.719,
   0.055, 0.750, 0.228, 0.334, 0.000, 0.002},
  {0.000, 0.809, 0.185, 0.275, 0.173, 0.247, 0.208, 0.313, 0.233, 3.003,
   0.139, 0.908, 1.133, 0.199, 0.307, 0.232, 0.176, 0.303, 0.719, 8.211,
   0.027, 0.750, 0.181, 0.240, 0.000, 0.002},
  {0.000, 0.043, 0.078, 0.022, 0.026, 0.019, 0.452, 0.027, 0.186, 0.044,
   0.079, 0.267, 0.060, 0.138, 0.042, 0.055, 0.991, 0.324, 0.054, 0.027,
   80.645, 0.750, 0.377, 0.035, 0.000, 0.002},
  {0.000, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750,
   0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750,
   0.750, 0.750, 0.750, 0.750, 0.000, 0.002},
  {0.000, 0.154, 0.223, 0.523, 0.080, 0.119, 3.379, 0.044, 0.611, 0.281,
   0.101, 0.239, 0.097, 0.390, 0.047, 0.074, 0.086, 0.208, 0.228, 0.181,
   0.377, 0.750, 23.141, 0.099, 0.000, 0.002},
  {0.000, 0.638, 1.613, 0.039, 2.296, 5.867, 0.044, 0.381, 1.224, 0.242,
   0.578, 0.219, 0.332, 0.822, 0.494, 6.272, 0.547, 0.433, 0.334, 0.240,
   0.035, 0.750, 0.099, 6.043, 0.000, 0.002},
  {0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.002},
  {0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002,
   0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002, 0.002,
   0.002, 0.002, 0.002, 0.002, 0.002, 1.333},
};

static Nlm_FloatHi BLOSUM45_FREQRATIOS[PROTEIN_ALPHABET][PROTEIN_ALPHABET] = {
  {0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.290},
  {0.000, 2.950, 0.735, 0.800, 0.689, 0.825, 0.587, 1.080, 0.654, 0.747,
   0.786, 0.712, 0.821, 0.789, 0.709, 0.867, 0.700, 1.300, 1.001, 1.010,
   0.565, 0.750, 0.639, 0.841, 0.000, 0.290},
  {0.000, 0.735, 3.260, 0.600, 3.594, 1.300, 0.496, 0.886, 1.088, 0.497,
   1.023, 0.472, 0.580, 2.862, 0.686, 0.992, 0.829, 1.053, 0.985, 0.520,
   0.381, 0.750, 0.627, 1.182, 0.000, 0.290},
  {0.000, 0.800, 0.600, 17.090, 0.533, 0.545, 0.602, 0.557, 0.491, 0.543,
   0.547, 0.673, 0.604, 0.680, 0.411, 0.486, 0.472, 0.797, 0.822, 0.715,
   0.334, 0.750, 0.489, 0.523, 0.000, 0.290},
  {0.000, 0.689, 3.594, 0.533, 5.356, 1.643, 0.431, 0.740, 0.976, 0.440,
   0.942, 0.463, 0.494, 1.502, 0.724, 0.958, 0.771, 0.929, 0.876, 0.494,
   0.373, 0.750, 0.645, 1.381, 0.000, 0.290},
  {0.000, 0.825, 1.300, 0.545, 1.643, 3.873, 0.498, 0.576, 0.962, 0.485,
   1.277, 0.571, 0.615, 0.893, 0.911, 1.531, 1.011, 0.912, 0.833, 0.555,
   0.519, 0.750, 0.617, 2.978, 0.000, 0.290},
  {0.000, 0.587, 0.496, 0.602, 0.431, 0.498, 5.748, 0.480, 0.679, 1.064,
   0.529, 1.303, 1.063, 0.572, 0.451, 0.444, 0.590, 0.610, 0.716, 0.953,
   1.355, 0.750, 2.185, 0.477, 0.000, 0.290},
  {0.000, 1.080, 0.886, 0.557, 0.740, 0.576, 0.480, 5.071, 0.662, 0.416,
   0.678, 0.450, 0.585, 1.059, 0.702, 0.687, 0.570, 1.058, 0.693, 0.479,
   0.591, 0.750, 0.549, 0.619, 0.000, 0.290},
  {0.000, 0.654, 1.088, 0.491, 0.976, 0.962, 0.679, 0.662, 9.512, 0.453,
   0.890, 0.670, 0.918, 1.220, 0.661, 1.151, 0.973, 0.854, 0.706, 0.457,
   0.452, 0.750, 1.472, 1.034, 0.000, 0.290},
  {0.000, 0.747, 0.497, 0.543, 0.440, 0.485, 1.064, 0.416, 0.453, 3.233,
   0.532, 1.596, 1.455, 0.564, 0.610, 0.578, 0.488, 0.618, 0.848, 2.176,
   0.565, 0.750, 0.906, 0.521, 0.000, 0.290},
  {0.000, 0.786, 1.023, 0.547, 0.942, 1.277, 0.529, 0.678, 0.890, 0.532,
   3.327, 0.554, 0.738, 1.119, 0.781, 1.330, 1.943, 0.890, 0.885, 0.592,
   0.562, 0.750, 0.737, 1.297, 0.000, 0.290},
  {0.000, 0.712, 0.472, 0.673, 0.463, 0.571, 1.303, 0.450, 0.670, 1.596,
   0.554, 2.997, 1.731, 0.484, 0.478, 0.642, 0.601, 0.556, 0.781, 1.334,
   0.671, 0.750, 0.965, 0.598, 0.000, 0.290},
  {0.000, 0.821, 0.580, 0.604, 0.494, 0.615, 1.063, 0.585, 0.918, 1.455,
   0.738, 1.731, 4.114, 0.682, 0.644, 0.941, 0.776, 0.660, 0.860, 1.236,
   0.634, 0.750, 1.023, 0.739, 0.000, 0.290},
  {0.000, 0.789, 2.862, 0.680, 1.502, 0.893, 0.572, 1.059, 1.220, 0.564,
   1.119, 0.484, 0.682, 4.478, 0.640, 1.032, 0.898, 1.200, 1.115, 0.552,
   0.390, 0.750, 0.606, 0.946, 0.000, 0.290},
  {0.000, 0.709, 0.686, 0.411, 0.724, 0.911, 0.451, 0.702, 0.661, 0.610,
   0.781, 0.478, 0.644, 0.640, 8.819, 0.716, 0.582, 0.750, 0.856, 0.540,
   0.525, 0.750, 0.479, 0.836, 0.000, 0.290},
  {0.000, 0.867, 0.992, 0.486, 0.958, 1.531, 0.444, 0.687, 1.151, 0.578,
   1.330, 0.642, 0.941, 1.032, 0.716, 4.407, 1.329, 1.092, 0.781, 0.547,
   0.645, 0.750, 0.829, 2.630, 0.000, 0.290},
  {0.000, 0.700, 0.829, 0.472, 0.771, 1.011, 0.590, 0.570, 0.973, 0.488,
   1.943, 0.601, 0.776, 0.898, 0.582, 1.329, 4.747, 0.799, 0.715, 0.578,
   0.580, 0.750, 0.807, 1.132, 0.000, 0.290},
  {0.000, 1.300, 1.053, 0.797, 0.929, 0.912, 0.610, 1.058, 0.854, 0.618,
   0.890, 0.556, 0.660, 1.200, 0.750, 1.092, 0.799, 2.782, 1.472, 0.728,
   0.428, 0.750, 0.706, 0.981, 0.000, 0.290},
  {0.000, 1.001, 0.985, 0.822, 0.876, 0.833, 0.716, 0.693, 0.706, 0.848,
   0.885, 0.781, 0.860, 1.115, 0.856, 0.781, 0.715, 1.472, 3.139, 1.040,
   0.454, 0.750, 0.744, 0.813, 0.000, 0.290},
  {0.000, 1.010, 0.520, 0.715, 0.494, 0.555, 0.953, 0.479, 0.457, 2.176,
   0.592, 1.334, 1.236, 0.552, 0.540, 0.547, 0.578, 0.728, 1.040, 2.871,
   0.473, 0.750, 0.809, 0.552, 0.000, 0.290},
  {0.000, 0.565, 0.381, 0.334, 0.373, 0.519, 1.355, 0.591, 0.452, 0.565,
   0.562, 0.671, 0.634, 0.390, 0.525, 0.645, 0.580, 0.428, 0.454, 0.473,
   29.702, 0.750, 1.801, 0.567, 0.000, 0.290},
  {0.000, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750,
   0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750,
   0.750, 0.750, 0.750, 0.750, 0.000, 0.290},
  {0.000, 0.639, 0.627, 0.489, 0.645, 0.617, 2.185, 0.549, 1.472, 0.906,
   0.737, 0.965, 1.023, 0.606, 0.479, 0.829, 0.807, 0.706, 0.744, 0.809,
   1.801, 0.750, 5.753, 0.698, 0.000, 0.290},
  {0.000, 0.841, 1.182, 0.523, 1.381, 2.978, 0.477, 0.619, 1.034, 0.521,
   1.297, 0.598, 0.739, 0.946, 0.836, 2.630, 1.132, 0.981, 0.813, 0.552,
   0.567, 0.750, 0.698, 2.845, 0.000, 0.290},
  {0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.290},
  {0.290, 0.290, 0.290, 0.290, 0.290, 0.290, 0.290, 0.290, 0.290, 0.290,
   0.290, 0.290, 0.290, 0.290, 0.290, 0.290, 0.290, 0.290, 0.290, 0.290,
   0.290, 0.290, 0.290, 0.290, 0.290, 1.333},
};

static Nlm_FloatHi BLOSUM80_FREQRATIOS[PROTEIN_ALPHABET][PROTEIN_ALPHABET] = {
  {0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.140},
  {0.000, 4.773, 0.477, 0.732, 0.451, 0.703, 0.397, 0.957, 0.514, 0.543,
   0.723, 0.505, 0.625, 0.510, 0.771, 0.696, 0.555, 1.535, 0.980, 0.866,
   0.309, 0.750, 0.436, 0.700, 0.000, 0.140},
  {0.000, 0.477, 5.362, 0.252, 5.759, 1.269, 0.252, 0.639, 0.830, 0.234,
   0.793, 0.221, 0.310, 4.868, 0.428, 0.850, 0.609, 0.948, 0.743, 0.268,
   0.179, 0.750, 0.307, 1.109, 0.000, 0.140},
  {0.000, 0.732, 0.252, 20.702, 0.214, 0.180, 0.395, 0.272, 0.221, 0.581,
   0.241, 0.493, 0.499, 0.300, 0.269, 0.295, 0.233, 0.576, 0.602, 0.634,
   0.302, 0.750, 0.308, 0.224, 0.000, 0.140},
  {0.000, 0.451, 5.759, 0.214, 9.106, 1.635, 0.234, 0.541, 0.594, 0.214,
   0.677, 0.197, 0.252, 1.584, 0.452, 0.763, 0.477, 0.774, 0.611, 0.245,
   0.145, 0.750, 0.245, 1.303, 0.000, 0.140},
  {0.000, 0.703, 1.269, 0.180, 1.635, 6.995, 0.249, 0.399, 0.901, 0.264,
   1.195, 0.276, 0.429, 0.811, 0.581, 1.906, 0.832, 0.845, 0.685, 0.369,
   0.241, 0.750, 0.333, 5.054, 0.000, 0.140},
  {0.000, 0.397, 0.252, 0.395, 0.234, 0.249, 9.486, 0.249, 0.572, 0.841,
   0.283, 1.114, 0.893, 0.273, 0.237, 0.285, 0.287, 0.369, 0.445, 0.649,
   1.089, 0.750, 2.780, 0.263, 0.000, 0.140},
  {0.000, 0.957, 0.639, 0.272, 0.541, 0.399, 0.249, 7.882, 0.387, 0.184,
   0.483, 0.210, 0.286, 0.761, 0.347, 0.425, 0.377, 0.784, 0.492, 0.251,
   0.264, 0.750, 0.230, 0.409, 0.000, 0.140},
  {0.000, 0.514, 0.830, 0.221, 0.594, 0.901, 0.572, 0.387, 16.070, 0.258,
   0.740, 0.314, 0.432, 1.124, 0.420, 1.316, 0.925, 0.661, 0.540, 0.289,
   0.390, 0.750, 1.819, 1.059, 0.000, 0.140},
  {0.000, 0.543, 0.234, 0.581, 0.214, 0.264, 0.841, 0.184, 0.258, 4.868,
   0.313, 1.665, 1.512, 0.258, 0.286, 0.309, 0.299, 0.379, 0.701, 2.496,
   0.343, 0.750, 0.539, 0.281, 0.000, 0.140},
  {0.000, 0.723, 0.793, 0.241, 0.677, 1.195, 0.283, 0.483, 0.740, 0.313,
   6.326, 0.357, 0.534, 0.938, 0.597, 1.524, 2.192, 0.820, 0.736, 0.370,
   0.241, 0.750, 0.408, 1.320, 0.000, 0.140},
  {0.000, 0.505, 0.221, 0.493, 0.197, 0.276, 1.114, 0.210, 0.314, 1.665,
   0.357, 4.463, 2.123, 0.250, 0.303, 0.407, 0.363, 0.368, 0.561, 1.220,
   0.439, 0.750, 0.581, 0.326, 0.000, 0.140},
  {0.000, 0.625, 0.310, 0.499, 0.252, 0.429, 0.893, 0.286, 0.432, 1.512,
   0.534, 2.123, 8.883, 0.382, 0.362, 0.887, 0.506, 0.498, 0.758, 1.224,
   0.561, 0.750, 0.550, 0.603, 0.000, 0.140},
  {0.000, 0.510, 4.868, 0.300, 1.584, 0.811, 0.273, 0.761, 1.124, 0.258,
   0.938, 0.250, 0.382, 8.963, 0.398, 0.958, 0.773, 1.165, 0.908, 0.297,
   0.221, 0.750, 0.385, 0.867, 0.000, 0.140},
  {0.000, 0.771, 0.428, 0.269, 0.452, 0.581, 0.237, 0.347, 0.420, 0.286,
   0.597, 0.303, 0.362, 0.398, 15.155, 0.538, 0.446, 0.652, 0.560, 0.370,
   0.178, 0.750, 0.258, 0.565, 0.000, 0.140},
  {0.000, 0.696, 0.850, 0.295, 0.763, 1.906, 0.285, 0.425, 1.316, 0.309,
   1.524, 0.407, 0.887, 0.958, 0.538, 8.340, 1.394, 0.859, 0.724, 0.411,
   0.408, 0.750, 0.462, 4.360, 0.000, 0.140},
  {0.000, 0.555, 0.609, 0.233, 0.477, 0.832, 0.287, 0.377, 0.925, 0.299,
   2.192, 0.363, 0.506, 0.773, 0.446, 1.394, 8.245, 0.695, 0.598, 0.354,
   0.294, 0.750, 0.418, 1.046, 0.000, 0.140},
  {0.000, 1.535, 0.948, 0.576, 0.774, 0.845, 0.369, 0.784, 0.661, 0.379,
   0.820, 0.368, 0.498, 1.165, 0.652, 0.859, 0.695, 5.106, 1.663, 0.494,
   0.271, 0.750, 0.462, 0.850, 0.000, 0.140},
  {0.000, 0.980, 0.743, 0.602, 0.611, 0.685, 0.445, 0.492, 0.540, 0.701,
   0.736, 0.561, 0.758, 0.908, 0.560, 0.724, 0.598, 1.663, 6.205, 0.891,
   0.285, 0.750, 0.474, 0.700, 0.000, 0.140},
  {0.000, 0.866, 0.268, 0.634, 0.245, 0.369, 0.649, 0.251, 0.289, 2.496,
   0.370, 1.220, 1.224, 0.297, 0.370, 0.411, 0.354, 0.494, 0.891, 4.584,
   0.342, 0.750, 0.489, 0.385, 0.000, 0.140},
  {0.000, 0.309, 0.179, 0.302, 0.145, 0.241, 1.089, 0.264, 0.390, 0.343,
   0.241, 0.439, 0.561, 0.221, 0.178, 0.408, 0.294, 0.271, 0.285, 0.342,
   41.552, 0.750, 2.036, 0.304, 0.000, 0.140},
  {0.000, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750,
   0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750,
   0.750, 0.750, 0.750, 0.750, 0.000, 0.140},
  {0.000, 0.436, 0.307, 0.308, 0.245, 0.333, 2.780, 0.230, 1.819, 0.539,
   0.408, 0.581, 0.550, 0.385, 0.258, 0.462, 0.418, 0.462, 0.474, 0.489,
   2.036, 0.750, 12.194, 0.382, 0.000, 0.140},
  {0.000, 0.700, 1.109, 0.224, 1.303, 5.054, 0.263, 0.409, 1.059, 0.281,
   1.320, 0.326, 0.603, 0.867, 0.565, 4.360, 1.046, 0.850, 0.700, 0.385,
   0.304, 0.750, 0.382, 4.789, 0.000, 0.140},
  {0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.140},
  {0.140, 0.140, 0.140, 0.140, 0.140, 0.140, 0.140, 0.140, 0.140, 0.140,
   0.140, 0.140, 0.140, 0.140, 0.140, 0.140, 0.140, 0.140, 0.140, 0.140,
   0.140, 0.140, 0.140, 0.140, 0.140, 1.333},
};

static Nlm_FloatHi BLOSUM50_FREQRATIOS[PROTEIN_ALPHABET][PROTEIN_ALPHABET] = {
  {0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.290},
  {0.000, 3.273, 0.687, 0.888, 0.660, 0.797, 0.546, 1.101, 0.641, 0.715,
   0.748, 0.657, 0.854, 0.720, 0.715, 0.820, 0.668, 1.364, 0.967, 0.982,
   0.464, 0.750, 0.596, 0.806, 0.000, 0.290},
  {0.000, 0.687, 3.676, 0.507, 4.021, 1.319, 0.384, 0.848, 1.118, 0.407,
   0.968, 0.391, 0.516, 3.269, 0.661, 0.998, 0.759, 1.061, 0.955, 0.446,
   0.339, 0.750, 0.556, 1.196, 0.000, 0.290},
  {0.000, 0.888, 0.507, 18.231, 0.428, 0.456, 0.565, 0.524, 0.517, 0.587,
   0.460, 0.650, 0.681, 0.601, 0.403, 0.481, 0.428, 0.830, 0.818, 0.809,
   0.312, 0.750, 0.539, 0.466, 0.000, 0.290},
  {0.000, 0.660, 4.021, 0.428, 6.112, 1.658, 0.337, 0.745, 0.884, 0.370,
   0.890, 0.373, 0.435, 1.558, 0.710, 0.967, 0.678, 0.921, 0.825, 0.432,
   0.307, 0.750, 0.504, 1.394, 0.000, 0.290},
  {0.000, 0.797, 1.319, 0.456, 1.658, 4.437, 0.456, 0.540, 0.911, 0.416,
   1.331, 0.478, 0.605, 0.920, 0.839, 1.671, 0.976, 0.882, 0.819, 0.479,
   0.548, 0.750, 0.652, 3.380, 0.000, 0.290},
  {0.000, 0.546, 0.384, 0.565, 0.337, 0.456, 6.636, 0.414, 0.755, 0.990,
   0.445, 1.262, 1.052, 0.439, 0.376, 0.417, 0.462, 0.553, 0.570, 0.860,
   1.348, 0.750, 2.424, 0.441, 0.000, 0.290},
  {0.000, 1.101, 0.848, 0.524, 0.745, 0.540, 0.414, 5.792, 0.601, 0.370,
   0.655, 0.377, 0.519, 0.969, 0.620, 0.641, 0.517, 0.999, 0.636, 0.403,
   0.505, 0.750, 0.467, 0.579, 0.000, 0.290},
  {0.000, 0.641, 1.118, 0.517, 0.884, 0.911, 0.755, 0.601, 10.449, 0.411,
   0.946, 0.547, 0.760, 1.394, 0.582, 1.209, 0.982, 0.823, 0.653, 0.414,
   0.475, 0.750, 1.570, 1.025, 0.000, 0.290},
  {0.000, 0.715, 0.407, 0.587, 0.370, 0.416, 0.990, 0.370, 0.411, 3.411,
   0.468, 1.697, 1.438, 0.451, 0.511, 0.503, 0.435, 0.546, 0.861, 2.313,
   0.522, 0.750, 0.826, 0.449, 0.000, 0.290},
  {0.000, 0.748, 0.968, 0.460, 0.890, 1.331, 0.445, 0.655, 0.946, 0.468,
   3.881, 0.479, 0.685, 1.060, 0.764, 1.419, 2.065, 0.893, 0.845, 0.523,
   0.462, 0.750, 0.668, 1.365, 0.000, 0.290},
  {0.000, 0.657, 0.391, 0.650, 0.373, 0.478, 1.262, 0.377, 0.547, 1.697,
   0.479, 3.328, 1.790, 0.413, 0.444, 0.563, 0.554, 0.511, 0.759, 1.324,
   0.602, 0.750, 0.858, 0.511, 0.000, 0.290},
  {0.000, 0.854, 0.516, 0.681, 0.435, 0.605, 1.052, 0.519, 0.760, 1.438,
   0.685, 1.790, 4.816, 0.612, 0.545, 0.960, 0.678, 0.682, 0.882, 1.213,
   0.761, 0.750, 0.918, 0.741, 0.000, 0.290},
  {0.000, 0.720, 3.269, 0.601, 1.558, 0.920, 0.439, 0.969, 1.394, 0.451,
   1.060, 0.413, 0.612, 5.285, 0.604, 1.035, 0.854, 1.224, 1.109, 0.463,
   0.376, 0.750, 0.617, 0.964, 0.000, 0.290},
  {0.000, 0.715, 0.661, 0.403, 0.710, 0.839, 0.376, 0.620, 0.582, 0.511,
   0.764, 0.444, 0.545, 0.604, 10.204, 0.750, 0.519, 0.759, 0.749, 0.524,
   0.420, 0.750, 0.468, 0.805, 0.000, 0.290},
  {0.000, 0.820, 0.998, 0.481, 0.967, 1.671, 0.417, 0.641, 1.209, 0.503,
   1.419, 0.563, 0.960, 1.035, 0.750, 4.697, 1.357, 1.069, 0.810, 0.557,
   0.715, 0.750, 0.742, 2.828, 0.000, 0.290},
  {0.000, 0.668, 0.759, 0.428, 0.678, 0.976, 0.462, 0.517, 0.982, 0.435,
   2.065, 0.554, 0.678, 0.854, 0.519, 1.357, 5.378, 0.804, 0.737, 0.512,
   0.529, 0.750, 0.727, 1.122, 0.000, 0.290},
  {0.000, 1.364, 1.061, 0.830, 0.921, 0.882, 0.553, 0.999, 0.823, 0.546,
   0.893, 0.511, 0.682, 1.224, 0.759, 1.069, 0.804, 3.143, 1.497, 0.679,
   0.392, 0.750, 0.651, 0.953, 0.000, 0.290},
  {0.000, 0.967, 0.955, 0.818, 0.825, 0.819, 0.570, 0.636, 0.653, 0.861,
   0.845, 0.759, 0.882, 1.109, 0.749, 0.810, 0.737, 1.497, 3.553, 1.060,
   0.503, 0.750, 0.691, 0.816, 0.000, 0.290},
  {0.000, 0.982, 0.446, 0.809, 0.432, 0.479, 0.860, 0.403, 0.414, 2.313,
   0.523, 1.324, 1.213, 0.463, 0.524, 0.557, 0.512, 0.679, 1.060, 3.118,
   0.485, 0.750, 0.727, 0.509, 0.000, 0.290},
  {0.000, 0.464, 0.339, 0.312, 0.307, 0.548, 1.348, 0.505, 0.475, 0.522,
   0.462, 0.602, 0.761, 0.376, 0.420, 0.715, 0.529, 0.392, 0.503, 0.485,
   31.361, 0.750, 1.765, 0.612, 0.000, 0.290},
  {0.000, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750,
   0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750,
   0.750, 0.750, 0.750, 0.750, 0.000, 0.290},
  {0.000, 0.596, 0.556, 0.539, 0.504, 0.652, 2.424, 0.467, 1.570, 0.826,
   0.668, 0.858, 0.918, 0.617, 0.468, 0.742, 0.727, 0.651, 0.691, 0.727,
   1.765, 0.750, 6.893, 0.686, 0.000, 0.290},
  {0.000, 0.806, 1.196, 0.466, 1.394, 3.380, 0.441, 0.579, 1.025, 0.449,
   1.365, 0.511, 0.741, 0.964, 0.805, 2.828, 1.122, 0.953, 0.816, 0.509,
   0.612, 0.750, 0.686, 3.169, 0.000, 0.290},
  {0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.290},
  {0.290, 0.290, 0.290, 0.290, 0.290, 0.290, 0.290, 0.290, 0.290, 0.290,
   0.290, 0.290, 0.290, 0.290, 0.290, 0.290, 0.290, 0.290, 0.290, 0.290,
   0.290, 0.290, 0.290, 0.290, 0.290, 1.333},
};

static Nlm_FloatHi BLOSUM90_FREQRATIOS[PROTEIN_ALPHABET][PROTEIN_ALPHABET] = {
  {0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.120},
  {0.000, 4.773, 0.477, 0.732, 0.451, 0.703, 0.397, 0.957, 0.514, 0.543,
   0.723, 0.505, 0.625, 0.510, 0.771, 0.696, 0.555, 1.535, 0.980, 0.866,
   0.309, 0.707, 0.436, 0.700, 0.000, 0.120},
  {0.000, 0.477, 5.362, 0.252, 5.759, 1.269, 0.252, 0.639, 0.830, 0.234,
   0.793, 0.221, 0.310, 4.868, 0.428, 0.850, 0.609, 0.948, 0.743, 0.268,
   0.179, 0.707, 0.307, 1.109, 0.000, 0.120},
  {0.000, 0.732, 0.252, 20.702, 0.214, 0.180, 0.395, 0.272, 0.221, 0.581,
   0.241, 0.493, 0.499, 0.300, 0.269, 0.295, 0.233, 0.576, 0.602, 0.634,
   0.302, 0.707, 0.308, 0.224, 0.000, 0.120},
  {0.000, 0.451, 5.759, 0.214, 9.106, 1.635, 0.234, 0.541, 0.594, 0.214,
   0.677, 0.197, 0.252, 1.584, 0.452, 0.763, 0.477, 0.774, 0.611, 0.245,
   0.145, 0.707, 0.245, 1.303, 0.000, 0.120},
  {0.000, 0.703, 1.269, 0.180, 1.635, 6.995, 0.249, 0.399, 0.901, 0.264,
   1.195, 0.276, 0.429, 0.811, 0.581, 1.906, 0.832, 0.845, 0.685, 0.369,
   0.241, 0.707, 0.333, 5.054, 0.000, 0.120},
  {0.000, 0.397, 0.252, 0.395, 0.234, 0.249, 9.486, 0.249, 0.572, 0.841,
   0.283, 1.114, 0.893, 0.273, 0.237, 0.285, 0.287, 0.369, 0.445, 0.649,
   1.089, 0.707, 2.780, 0.263, 0.000, 0.120},
  {0.000, 0.957, 0.639, 0.272, 0.541, 0.399, 0.249, 7.882, 0.387, 0.184,
   0.483, 0.210, 0.286, 0.761, 0.347, 0.425, 0.377, 0.784, 0.492, 0.251,
   0.264, 0.707, 0.230, 0.409, 0.000, 0.120},
  {0.000, 0.514, 0.830, 0.221, 0.594, 0.901, 0.572, 0.387, 16.070, 0.258,
   0.740, 0.314, 0.432, 1.124, 0.420, 1.316, 0.925, 0.661, 0.540, 0.289,
   0.390, 0.707, 1.819, 1.059, 0.000, 0.120},
  {0.000, 0.543, 0.234, 0.581, 0.214, 0.264, 0.841, 0.184, 0.258, 4.868,
   0.313, 1.665, 1.512, 0.258, 0.286, 0.309, 0.299, 0.379, 0.701, 2.496,
   0.343, 0.707, 0.539, 0.281, 0.000, 0.120},
  {0.000, 0.723, 0.793, 0.241, 0.677, 1.195, 0.283, 0.483, 0.740, 0.313,
   6.326, 0.357, 0.534, 0.938, 0.597, 1.524, 2.192, 0.820, 0.736, 0.370,
   0.241, 0.707, 0.408, 1.320, 0.000, 0.120},
  {0.000, 0.505, 0.221, 0.493, 0.197, 0.276, 1.114, 0.210, 0.314, 1.665,
   0.357, 4.463, 2.123, 0.250, 0.303, 0.407, 0.363, 0.368, 0.561, 1.220,
   0.439, 0.707, 0.581, 0.326, 0.000, 0.120},
  {0.000, 0.625, 0.310, 0.499, 0.252, 0.429, 0.893, 0.286, 0.432, 1.512,
   0.534, 2.123, 8.883, 0.382, 0.362, 0.887, 0.506, 0.498, 0.758, 1.224,
   0.561, 0.707, 0.550, 0.603, 0.000, 0.120},
  {0.000, 0.510, 4.868, 0.300, 1.584, 0.811, 0.273, 0.761, 1.124, 0.258,
   0.938, 0.250, 0.382, 8.963, 0.398, 0.958, 0.773, 1.165, 0.908, 0.297,
   0.221, 0.707, 0.385, 0.867, 0.000, 0.120},
  {0.000, 0.771, 0.428, 0.269, 0.452, 0.581, 0.237, 0.347, 0.420, 0.286,
   0.597, 0.303, 0.362, 0.398, 15.155, 0.538, 0.446, 0.652, 0.560, 0.370,
   0.178, 0.707, 0.258, 0.565, 0.000, 0.120},
  {0.000, 0.696, 0.850, 0.295, 0.763, 1.906, 0.285, 0.425, 1.316, 0.309,
   1.524, 0.407, 0.887, 0.958, 0.538, 8.340, 1.394, 0.859, 0.724, 0.411,
   0.408, 0.707, 0.462, 4.360, 0.000, 0.120},
  {0.000, 0.555, 0.609, 0.233, 0.477, 0.832, 0.287, 0.377, 0.925, 0.299,
   2.192, 0.363, 0.506, 0.773, 0.446, 1.394, 8.245, 0.695, 0.598, 0.354,
   0.294, 0.707, 0.418, 1.046, 0.000, 0.120},
  {0.000, 1.535, 0.948, 0.576, 0.774, 0.845, 0.369, 0.784, 0.661, 0.379,
   0.820, 0.368, 0.498, 1.165, 0.652, 0.859, 0.695, 5.106, 1.663, 0.494,
   0.271, 0.707, 0.462, 0.850, 0.000, 0.120},
  {0.000, 0.980, 0.743, 0.602, 0.611, 0.685, 0.445, 0.492, 0.540, 0.701,
   0.736, 0.561, 0.758, 0.908, 0.560, 0.724, 0.598, 1.663, 6.205, 0.891,
   0.285, 0.707, 0.474, 0.700, 0.000, 0.120},
  {0.000, 0.866, 0.268, 0.634, 0.245, 0.369, 0.649, 0.251, 0.289, 2.496,
   0.370, 1.220, 1.224, 0.297, 0.370, 0.411, 0.354, 0.494, 0.891, 4.584,
   0.342, 0.707, 0.489, 0.385, 0.000, 0.120},
  {0.000, 0.309, 0.179, 0.302, 0.145, 0.241, 1.089, 0.264, 0.390, 0.343,
   0.241, 0.439, 0.561, 0.221, 0.178, 0.408, 0.294, 0.271, 0.285, 0.342,
   41.552, 0.707, 2.036, 0.304, 0.000, 0.120},
  {0.000, 0.707, 0.707, 0.707, 0.707, 0.707, 0.707, 0.707, 0.707, 0.707,
   0.707, 0.707, 0.707, 0.707, 0.707, 0.707, 0.707, 0.707, 0.707, 0.707,
   0.707, 0.707, 0.707, 0.707, 0.000, 0.120},
  {0.000, 0.436, 0.307, 0.308, 0.245, 0.333, 2.780, 0.230, 1.819, 0.539,
   0.408, 0.581, 0.550, 0.385, 0.258, 0.462, 0.418, 0.462, 0.474, 0.489,
   2.036, 0.707, 12.194, 0.382, 0.000, 0.120},
  {0.000, 0.700, 1.109, 0.224, 1.303, 5.054, 0.263, 0.409, 1.059, 0.281,
   1.320, 0.326, 0.603, 0.867, 0.565, 4.360, 1.046, 0.850, 0.700, 0.385,
   0.304, 0.707, 0.382, 4.789, 0.000, 0.120},
  {0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.120},
  {0.120, 0.120, 0.120, 0.120, 0.120, 0.120, 0.120, 0.120, 0.120, 0.120,
   0.120, 0.120, 0.120, 0.120, 0.120, 0.120, 0.120, 0.120, 0.120, 0.120,
   0.120, 0.120, 0.120, 0.120, 0.120, 1.333},
};

static Nlm_FloatHi PAM250_FREQRATIOS[PROTEIN_ALPHABET][PROTEIN_ALPHABET] = {
  {0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.170},
  {0.000, 1.516, 1.056, 0.627, 1.070, 1.075, 0.446, 1.339, 0.732, 0.889,
   0.766, 0.646, 0.770, 1.040, 1.294, 0.903, 0.701, 1.290, 1.317, 1.045,
   0.264, 0.750, 0.450, 1.000, 0.000, 0.170},
  {0.000, 1.056, 1.843, 0.365, 2.052, 1.825, 0.354, 1.119, 1.297, 0.617,
   1.130, 0.453, 0.605, 1.600, 0.846, 1.339, 0.862, 1.117, 1.032, 0.638,
   0.297, 0.750, 0.486, 1.613, 0.000, 0.170},
  {0.000, 0.627, 0.365, 15.630, 0.307, 0.295, 0.371, 0.459, 0.452, 0.588,
   0.286, 0.248, 0.299, 0.433, 0.527, 0.289, 0.430, 0.990, 0.601, 0.639,
   0.169, 0.750, 1.078, 0.293, 0.000, 0.170},
  {0.000, 1.070, 2.052, 0.307, 2.433, 2.197, 0.274, 1.147, 1.173, 0.580,
   1.020, 0.398, 0.548, 1.610, 0.806, 1.461, 0.743, 1.069, 0.968, 0.612,
   0.213, 0.750, 0.369, 1.877, 0.000, 0.170},
  {0.000, 1.075, 1.825, 0.295, 2.197, 2.414, 0.287, 1.044, 1.165, 0.626,
   0.988, 0.464, 0.614, 1.393, 0.880, 1.771, 0.782, 0.999, 0.914, 0.660,
   0.201, 0.750, 0.372, 2.134, 0.000, 0.170},
  {0.000, 0.446, 0.354, 0.371, 0.274, 0.287, 8.028, 0.332, 0.659, 1.262,
   0.298, 1.518, 1.043, 0.447, 0.351, 0.343, 0.359, 0.478, 0.488, 0.766,
   1.077, 0.750, 4.975, 0.311, 0.000, 0.170},
  {0.000, 1.339, 1.119, 0.459, 1.147, 1.044, 0.332, 2.991, 0.616, 0.557,
   0.678, 0.395, 0.525, 1.087, 0.894, 0.757, 0.555, 1.278, 0.999, 0.735,
   0.200, 0.750, 0.300, 0.919, 0.000, 0.170},
  {0.000, 0.732, 1.297, 0.452, 1.173, 1.165, 0.660, 0.616, 4.475, 0.571,
   0.990, 0.620, 0.610, 1.440, 0.947, 1.962, 1.431, 0.830, 0.741, 0.597,
   0.544, 0.750, 0.979, 1.512, 0.000, 0.170},
  {0.000, 0.889, 0.617, 0.588, 0.580, 0.626, 1.262, 0.557, 0.571, 2.830,
   0.641, 1.749, 1.650, 0.660, 0.627, 0.625, 0.629, 0.723, 1.017, 2.338,
   0.303, 0.750, 0.800, 0.626, 0.000, 0.170},
  {0.000, 0.766, 1.130, 0.286, 1.020, 0.988, 0.298, 0.678, 0.990, 0.641,
   2.926, 0.519, 1.100, 1.258, 0.770, 1.184, 2.182, 0.962, 0.996, 0.570,
   0.451, 0.750, 0.360, 1.073, 0.000, 0.170},
  {0.000, 0.646, 0.453, 0.248, 0.398, 0.464, 1.518, 0.395, 0.620, 1.749,
   0.519, 3.930, 2.338, 0.516, 0.556, 0.665, 0.501, 0.524, 0.677, 1.532,
   0.647, 0.750, 0.815, 0.551, 0.000, 0.170},
  {0.000, 0.770, 0.605, 0.299, 0.548, 0.614, 1.043, 0.525, 0.609, 1.650,
   1.100, 2.338, 4.404, 0.670, 0.621, 0.796, 0.905, 0.699, 0.873, 1.510,
   0.375, 0.750, 0.568, 0.693, 0.000, 0.170},
  {0.000, 1.040, 1.600, 0.433, 1.610, 1.393, 0.447, 1.087, 1.440, 0.660,
   1.258, 0.516, 0.671, 1.588, 0.893, 1.197, 1.000, 1.173, 1.107, 0.668,
   0.394, 0.750, 0.621, 1.307, 0.000, 0.170},
  {0.000, 1.294, 0.846, 0.527, 0.806, 0.880, 0.351, 0.894, 0.946, 0.627,
   0.770, 0.556, 0.621, 0.893, 3.841, 1.055, 0.960, 1.243, 1.076, 0.758,
   0.275, 0.750, 0.321, 0.956, 0.000, 0.170},
  {0.000, 0.903, 1.339, 0.290, 1.462, 1.772, 0.343, 0.757, 1.962, 0.625,
   1.184, 0.665, 0.796, 1.197, 1.056, 2.541, 1.335, 0.890, 0.833, 0.649,
   0.335, 0.750, 0.395, 2.107, 0.000, 0.170},
  {0.000, 0.701, 0.862, 0.430, 0.743, 0.782, 0.359, 0.555, 1.430, 0.629,
   2.182, 0.501, 0.905, 1.000, 0.960, 1.335, 4.078, 0.928, 0.820, 0.561,
   1.652, 0.750, 0.379, 1.023, 0.000, 0.170},
  {0.000, 1.290, 1.117, 0.990, 1.069, 0.999, 0.478, 1.278, 0.830, 0.722,
   0.962, 0.524, 0.699, 1.173, 1.244, 0.890, 0.928, 1.441, 1.362, 0.799,
   0.566, 0.750, 0.520, 0.951, 0.000, 0.170},
  {0.000, 1.317, 1.032, 0.601, 0.968, 0.914, 0.488, 0.999, 0.741, 1.016,
   0.996, 0.677, 0.873, 1.107, 1.075, 0.833, 0.820, 1.362, 1.806, 1.068,
   0.306, 0.750, 0.530, 0.879, 0.000, 0.170},
  {0.000, 1.045, 0.638, 0.639, 0.612, 0.660, 0.766, 0.735, 0.597, 2.338,
   0.570, 1.532, 1.511, 0.668, 0.758, 0.649, 0.561, 0.799, 1.068, 2.698,
   0.237, 0.750, 0.566, 0.656, 0.000, 0.170},
  {0.000, 0.264, 0.297, 0.169, 0.213, 0.201, 1.077, 0.200, 0.544, 0.303,
   0.450, 0.647, 0.375, 0.394, 0.275, 0.335, 1.651, 0.566, 0.306, 0.237,
   52.679, 0.750, 0.970, 0.259, 0.000, 0.170},
  {0.000, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750,
   0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750, 0.750,
   0.750, 0.750, 0.750, 0.750, 0.000, 0.170},
  {0.000, 0.450, 0.486, 1.078, 0.369, 0.372, 4.976, 0.300, 0.979, 0.800,
   0.360, 0.815, 0.568, 0.621, 0.321, 0.395, 0.379, 0.520, 0.530, 0.566,
   0.970, 0.750, 10.338, 0.382, 0.000, 0.170},
  {0.000, 1.000, 1.613, 0.293, 1.877, 2.134, 0.311, 0.919, 1.512, 0.626,
   1.073, 0.551, 0.693, 1.307, 0.956, 2.107, 1.023, 0.951, 0.879, 0.656,
   0.259, 0.750, 0.382, 2.122, 0.000, 0.170},
  {0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000,
   0.000, 0.000, 0.000, 0.000, 0.000, 0.170},
  {0.170, 0.170, 0.170, 0.170, 0.170, 0.170, 0.170, 0.170, 0.170, 0.170,
   0.170, 0.170, 0.170, 0.170, 0.170, 0.170, 0.170, 0.170, 0.170, 0.170,
   0.170, 0.170, 0.170, 0.170, 0.170, 1.333},
};


FreqRatios*
PSIMatrixFrequencyRatiosNew(const char* matrix_name)
{
    unsigned int i, j;          /* loop indices */
    FreqRatios* retval = NULL;  /* the return value */

    ASSERT(matrix_name);

    retval = (FreqRatios*) Malloc(sizeof(FreqRatios));
    if ( !retval ) {
        return NULL;
    }

    retval->data = (double**) Malloc(sizeof(double*)*PROTEIN_ALPHABET);
    if ( !retval->data ) {
        return PSIMatrixFrequencyRatiosFree(retval);
    }

    for (i = 0; i < PROTEIN_ALPHABET; i++) {
        retval->data[i] = (double*) Malloc(sizeof(double)*PROTEIN_ALPHABET);
        if ( !retval->data[i] ) {
            for (j = 0; j < i; j++) {
                retval->data[j] = MemFree(retval->data[j]);
            }
            return PSIMatrixFrequencyRatiosFree(retval);
        }
    }

    if ( !strcmp(matrix_name, "BLOSUM62") ||
         !strcmp(matrix_name, "BLOSUM62_20")) {
        for (i = 0; i < PROTEIN_ALPHABET; i++) {
            for (j = 0; j < PROTEIN_ALPHABET; j++) {
                retval->data[i][j] = BLOSUM62_FREQRATIOS[i][j];
            }
        }
        retval->bit_scale_factor = 2;
    } else if ( !strcmp(matrix_name, "BLOSUM62_20A")) {
        for (i = 0; i < PROTEIN_ALPHABET; i++) {
            for (j = 0; j < PROTEIN_ALPHABET; j++) {
                retval->data[i][j] = 
                    BLOSUM62_20A_SCALE_MULTIPLIER * BLOSUM62_FREQRATIOS[i][j];
            }
        }
        retval->bit_scale_factor = 2;
    } else if ( !strcmp(matrix_name, "BLOSUM62_20B")) {
        for (i = 0; i < PROTEIN_ALPHABET; i++) {
            for (j = 0; j < PROTEIN_ALPHABET; j++) {
                retval->data[i][j] =
                    BLOSUM62_20B_SCALE_MULTIPLIER * BLOSUM62_FREQRATIOS[i][j];
            }
        }
        retval->bit_scale_factor = 2;
    } else if ( !strcmp(matrix_name, "BLOSUM45") ) {
        for (i = 0; i < PROTEIN_ALPHABET; i++) {
            for (j = 0; j < PROTEIN_ALPHABET; j++) {
                retval->data[i][j] = BLOSUM45_FREQRATIOS[i][j];
            }
        }
        retval->bit_scale_factor = 3;
    } else if ( !strcmp(matrix_name, "BLOSUM80") ) {
        for (i = 0; i < PROTEIN_ALPHABET; i++) {
            for (j = 0; j < PROTEIN_ALPHABET; j++) {
                retval->data[i][j] = BLOSUM80_FREQRATIOS[i][j];
            }
        }
        retval->bit_scale_factor = 2;
    } else if ( !strcmp(matrix_name, "BLOSUM50") ) {
        for (i = 0; i < PROTEIN_ALPHABET; i++) {
            for (j = 0; j < PROTEIN_ALPHABET; j++) {
                retval->data[i][j] = BLOSUM50_FREQRATIOS[i][j];
            }
        }
        retval->bit_scale_factor = 2;
    } else if ( !strcmp(matrix_name, "BLOSUM90") ) {
        for (i = 0; i < PROTEIN_ALPHABET; i++) {
            for (j = 0; j < PROTEIN_ALPHABET; j++) {
                retval->data[i][j] = BLOSUM90_FREQRATIOS[i][j];
            }
        }
        retval->bit_scale_factor = 2;
    } else if ( !strcmp(matrix_name, "PAM30") ) {
        for (i = 0; i < PROTEIN_ALPHABET; i++) {
            for (j = 0; j < PROTEIN_ALPHABET; j++) {
                retval->data[i][j] = PAM30_FREQRATIOS[i][j];
            }
        }
        retval->bit_scale_factor = 2;
    } else if ( !strcmp(matrix_name, "PAM70") ) {
        for (i = 0; i < PROTEIN_ALPHABET; i++) {
            for (j = 0; j < PROTEIN_ALPHABET; j++) {
                retval->data[i][j] = PAM70_FREQRATIOS[i][j];
            }
        }
        retval->bit_scale_factor = 2;
    } else if ( !strcmp(matrix_name, "PAM250") ) {
        for (i = 0; i < PROTEIN_ALPHABET; i++) {
            for (j = 0; j < PROTEIN_ALPHABET; j++) {
                retval->data[i][j] = PAM250_FREQRATIOS[i][j];
            }
        }
        retval->bit_scale_factor = 2;
    } else {
        retval = PSIMatrixFrequencyRatiosFree(retval);
    }

    return retval;
}

FreqRatios*
PSIMatrixFrequencyRatiosFree(FreqRatios* freq_ratios)
{
    if ( !freq_ratios )
        return NULL;

    if (freq_ratios->data) {
        Uint4 i;
        for (i = 0; i < PROTEIN_ALPHABET; i++) {
            freq_ratios->data[i] = MemFree(freq_ratios->data[i]);
        }
        freq_ratios->data = MemFree(freq_ratios->data);
    }

    freq_ratios = MemFree(freq_ratios);
    return NULL;
}

/* END of copied code */
/****************************************************************************/
