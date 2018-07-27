static char const rcsid[] = "$Id: profiles.c,v 6.36 2003/05/30 17:31:10 coulouri Exp $";

/* $Id: profiles.c,v 6.36 2003/05/30 17:31:10 coulouri Exp $
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
*/

/*****************************************************************************

File name: profiles.c

Author: Alejandro Schaffer

Contents: main routines for impala program to search a database of
  PSI-BLAST-generated position-specific score matrices

=======
 $Revision: 6.36 $

 $Log: profiles.c,v $
 Revision 6.36  2003/05/30 17:31:10  coulouri
 add rcsid

 Revision 6.35  2003/05/13 16:02:42  coulouri
 make ErrPostEx(SEV_FATAL, ...) exit with nonzero status

 Revision 6.34  2002/10/01 20:46:34  madden
 Comment out call to SeqEntryFree that generates warning from ObjMgr when -J is set to T

 Revision 6.33  2001/12/28 18:02:33  dondosha
 Keep score and scoreThisAlign for each local alignment, so as to allow tie-breaking by score

 Revision 6.32  2000/11/20 14:37:20  madden
 Changed FileOpen mode for byte-encoded checkpoint files from "r" to "rb" or from "w" to "wb" to solve a problem on Windows NT.

 Revision 6.31  2000/11/08 18:39:52  kans
 added <sqnutils.h> for Mac compiler, removed UNIX-only headers that are already included by ncbilcl.h for UNIX

 Revision 6.30  2000/10/04 13:11:57  madden
 put query translation before call to BlastMaskTheResidues

 Revision 6.29  2000/09/27 19:32:28  dondosha
 Use original square substitution matrix to pass to txalign on all iterations

 Revision 6.28  2000/09/13 18:34:36  dondosha
 Create BLAST_Matrix from ScoreBlk before converting it to txalign-style matrix

 Revision 6.27  2000/09/12 21:52:46  dondosha
 Pass the correct scoring matrix to ShowTextAlignFromAnnot

 Revision 6.26  2000/07/25 18:14:07  shavirin
 WARNING: This is no-turning-back changed related to S&W Blast from
 Alejandro Schaffer


*****************************************************************************/

#include <ncbi.h>
#include <objseq.h>
#include <objsset.h>
#include <sequtil.h>
#include <seqport.h>
#include <tofasta.h>
#include <blast.h>
#include <blastpri.h>
#include <txalign.h>
#include <simutil.h>
#include <posit.h>
#include <profiles.h>
#include <gapxdrop.h>
#include <stdlib.h>
/*
#include <sys/types.h>
#include <sys/stat.h>
*/
#include <fcntl.h>
#include <sqnutils.h>


/* Used by the callback function. */
FILE *global_fp=NULL;
/*
	Callback to print out ticks, in UNIX only due to file systems
	portability issues.
*/

static int LIBCALLBACK
tick_callback(Int4 sequence_number, Int4 number_of_positive_hits)

{

#ifdef OS_UNIX

	fprintf(global_fp, "%s", ".");
	fflush(global_fp);
#endif
	return 0;
}

/*initialize the data struture proDemographics to collect
  statistics about the execution of the program
  queryLength is the length of the input query sequence
  matrix name is the name of the underlying matrix
  X is the input value of the X-dropoff parameter for gapped alignments*/
static void initDemographics(proDemographicsItems *proDemographics, Int4 queryLength, Char *matrixName, Nlm_FloatHi X, Int4 effDbLength)
{
   strcpy((Char *) proDemographics->matrixName, (Char *)matrixName);
   proDemographics->numSequencesTested = 0;
   proDemographics->numBetterThanEthresh = 0;
   proDemographics->numCallsALIGN = 0;
   proDemographics->queryLength = queryLength;
   proDemographics->dbLength = 0;
   proDemographics->effDbLength = effDbLength;
   proDemographics->effSearchSpace = 0;
   proDemographics->XinBits = X;

}

/*print out the information on the execution of the program
  prodemographics stores the information
  outfp is the file descriptor for output
  eThresh is the e-value threshold to declare matches printable*/
static void printDemographics(proDemographicsItems proDemographics, FILE * outfp, Nlm_FloatHi eThresh)
{
   fprintf(outfp,"Underlying Matrix: %s\n",proDemographics.matrixName);
   fprintf(outfp,"Number of sequences tested against query: %ld\n", (long) proDemographics.numSequencesTested);
   fprintf(outfp,"Number of sequences better than %.1lf: %ld \n", eThresh, (long) proDemographics.numBetterThanEthresh);
   fprintf(outfp,"Number of calls to ALIGN: %ld \n", (long) proDemographics.numCallsALIGN);
   fprintf(outfp,"Length of query: %ld \n",(long) proDemographics.queryLength);
   fprintf(outfp,"Total length of test sequences: %ld  \n", (long) proDemographics.dbLength);
   fprintf(outfp,"Effective length of test sequences: %.1lf\n", proDemographics.effDbLength);
   fprintf(outfp,"Effective search space size: %.1lf\n",  proDemographics.effSearchSpace); 
   fprintf(outfp,"Initial X dropoff for ALIGN: %.1lf bits\n", proDemographics.XinBits);
}


/*counts the number of items in sequencesFile and matricesFile, assumed to
  be one per line, and checks that the numbers are equal.
  returns the number if equal, 0 if unequal, rewinds the file descriptors
  before returning*/
static Int4 countProfiles(FILE *sequencesFile)
{
  Int4 sequencesCount = 0; /*count for sequencesFile*/
  Char oneFileName[MAXLINELEN]; /*for reading one line per file*/
  
  while (fgets(oneFileName,MAXLINELEN,sequencesFile))
    sequencesCount++;
  rewind(sequencesFile);
  return(sequencesCount);
}

/*read in a sequence representing a matrix profile from the
  file descriptor sequenceFile
  the sequence read in is returned
  the length is passed back in sequenceLength
  if makeID is TRUE then a SeqId needs to be assigned and passed back
  in sequence_id
  MAXLEN is the maximum length of the sequence to be read in, used for
  allocating memory */
static Uint1 *  readSequence(FILE * sequenceFile, Int4 * sequenceLength, SeqIdPtr * sequence_id, Boolean makeID, Int4 MAXLEN, Int4 index)
{

  Uint1 * descr;  /*description of sequence*/
  Uint1 * query;  /*sequence read in*/
  Uint1 lineOfQuery[MAXLINELEN]; /*one line of sequence*/
  Int4 charIndex, lineIndex; /*indices to copy into query*/
  BioseqPtr query_bsp, fake_bsp;  /*structure to hold query information*/
  SeqEntryPtr sep;      /*structure to hold query retrieval result*/
  Int4  queryLength;  /*length of query sequence*/
  Int4 c; /*index over query sequence*/
  SeqLocPtr   private_slp=NULL; /*used to get a sequence id*/
  SeqIdPtr local_sequence_id; /*represents sequence id used to display alignments*/
  ObjectIdPtr obidp; /*object id pointer used to store new identifier for
                       a string*/

   if (makeID) {
     descr = (Uint1 *) MemNew(MAXLINELEN * sizeof(Uint1 *)); 
     fgets((CharPtr) descr, MAXLINELEN, sequenceFile);       
     if (strlen(((CharPtr) descr)) > (MAXLINELEN -2))
       fscanf(sequenceFile, "%*[^\n]");
     sep = FastaToSeqEntry(sequenceFile, FALSE);
     if (sep != NULL) {
       query_bsp = NULL;
       SeqEntryExplore(sep, &query_bsp, FindProt);
       if (query_bsp == NULL) {
	 ErrPostEx(SEV_FATAL, 1, 0, "Unable to obtain bioseq\n");
	 return NULL;
       }
       fake_bsp = BioseqNew();
       fake_bsp->descr = query_bsp->descr;
       fake_bsp->repr = query_bsp->repr;
       fake_bsp->mol = query_bsp->mol;
       fake_bsp->length = query_bsp->length;
       fake_bsp->seq_data_type = query_bsp->seq_data_type;
       fake_bsp->seq_data = query_bsp->seq_data;
       descr[strlen((CharPtr) descr) - 1] = '\0';

       obidp = ObjectIdNew();
       obidp->str = StringSave((CharPtr) &(descr[1]));
       ValNodeAddPointer(&(fake_bsp->id), SEQID_LOCAL, obidp);

       *sequence_id = SeqIdDup(fake_bsp->id); 

       query = BlastGetSequenceFromBioseq(fake_bsp, &queryLength);
     }
   }  
   else {
     query = (Uint1 *) MemNew(MAXLEN * sizeof(Uint1 )); 
     fgets((CharPtr) lineOfQuery, MAXLINELEN, sequenceFile);       
     if (strlen(((CharPtr) lineOfQuery)) > (MAXLINELEN -2))
       fscanf(sequenceFile, "%*[^\n]");
     charIndex = 0;
     while (fgets((CharPtr) lineOfQuery, MAXLEN, sequenceFile)) {
       lineIndex = 0;
       while(isalpha(lineOfQuery[lineIndex])) {
         query[charIndex] = lineOfQuery[lineIndex];
         charIndex++;
         lineIndex++;
       }
     }
     query[charIndex] = '\0';
     queryLength = charIndex;
   }
   *sequenceLength = queryLength;
   for (c= 0; c < queryLength; c++)
     query[c] = ResToInt((Char) query[c]);  
   return(query);     
}

/*free the memory associated with the position-specific score matrices*/
static void  freeMatrix(BLAST_Score **posMatrix, Int4 dbSequenceLength)
{
  Int4 i; /*row index*/

  for (i = 0; i < dbSequenceLength; i++)
    MemFree(posMatrix[i]);
  MemFree(posMatrix);
}

/*allocate memory for the position-specific score matrices
  enough memory is allocated to hold the largest matrix
  the memory is reused for each different matrix*/
static BLAST_Score ** allocateMatrix(Int4 maxSequenceLength, Int4 alphabetSize)
{
  Int4 i; /*row index for matrix*/
  BLAST_Score **returnMatrix; /*matrix to be returned*/

  returnMatrix = (BLAST_Score **) MemNew(maxSequenceLength * sizeof(BLAST_Score *));
  for(i = 0; i < maxSequenceLength; i++) 
    returnMatrix[i] = (BLAST_Score *) MemNew(alphabetSize * sizeof(BLAST_Score));

  return(returnMatrix);
}

/* read in parameters of a position-specific score matrix from thisMatrixFile
   the number of positions is dbSequenceLength
   kbp keeps the Karlin-Altschul parameters */
static void readkbp(FILE * thisMatrixFile, Int4 dbSequenceLength, 
              BLAST_KarlinBlkPtr kbp, Char *sequenceFileName)
{
  Char c; /*index over alphabet*/
  Int4 i; /*row index for matrix*/
  Int4 lengthInFile; /*length of query*/
  Char junkChar; /*used to read in useless characters*/
  Nlm_FloatHi junkLambda, junkK, junklogK, junkH; /*used to read in useless
						    Karlin blocks*/
  Char *sequence;  /*sequence to read in*/
  Char rowScores[MAXLINELEN]; /*one row of scores to be read in*/

  fscanf(thisMatrixFile, "%d", &lengthInFile);
  if (dbSequenceLength != lengthInFile) {
    ErrPostEx(SEV_FATAL, 1, 0, "profile: for file %s length in sequence file is %ld, length in matrix file is %ld, probably due to a blank in sequence file\n", sequenceFileName, (long) dbSequenceLength, (long) lengthInFile);
    return;
  }
  sequence = (Char *) MemNew((dbSequenceLength+1) * sizeof(Char));
  fscanf(thisMatrixFile,"%s",sequence);
  MemFree(sequence);
  /*read in useless Karlin block*/
  fscanf(thisMatrixFile,"%le", &junkLambda);
  fscanf(thisMatrixFile,"%le", &junkK);
  fscanf(thisMatrixFile,"%le", &junklogK);
  fscanf(thisMatrixFile,"%le", &junkH);
  /*read in position-specific Karlin block*/
  fscanf(thisMatrixFile,"%le", &(kbp->Lambda));
  fscanf(thisMatrixFile,"%le", &(kbp->K));
  fscanf(thisMatrixFile,"%le", &(kbp->logK));
  fscanf(thisMatrixFile,"%le", &(kbp->H));
  kbp->K *= PRO_K_MULTIPLIER;
  kbp->logK = log(kbp->K);
}

/*convert a raw score recorded as bestScore into an e-value that is
  returned. 
  effSearchSpace is the effective length of the profile database *
  querySize
  bestScore is the score to be converted
  kbp passes some paramters related to the Karlin-Altschul statistics */

static Nlm_FloatHi scoreToEvalue(Nlm_FloatHi effSearchSpace, 
   Int4 bestScore,   BLAST_KarlinBlkPtr kbp)
{
  Nlm_FloatHi returnValue; /*value to return back*/
      Nlm_FloatHi normalizedScore; /*score normalized to take into
                                     account for Lambda and K*/

      normalizedScore = kbp->Lambda * bestScore - log(kbp->K);
      returnValue = effSearchSpace * exp(-(normalizedScore));
      return(returnValue);
} 

/*convert a raw score recorded as bestScore into an 
  estimated e-value that is   returned. 
  effSearchSpace is the effective length of the profile database *
  querySize
  bestScore is the score to be converted
  LambdaRatio is the ratio between the estimated gapped Lambda
  for this matrix and the standard gapped Lambda
  kbp passes some paramters related to the Karlin-Altschul statistics 
  */

static Nlm_FloatHi estimateEvalue(Nlm_FloatHi effSearchSpace,
     Int4 bestScore, Nlm_FloatHi LambdaRatio, BLAST_KarlinBlkPtr kbp)
{

      Nlm_FloatHi returnValue; /*value to return back*/
      Nlm_FloatHi normalizedScore;  /*score normalized to take into
                                     account for Lambda and K*/

      normalizedScore = LambdaRatio * kbp->Lambda * bestScore - log(kbp->K);
      returnValue = effSearchSpace * exp(-(normalizedScore));
      return(returnValue);

}

/*compute the adjusted query length
  queryLength is the actual length
  L is the adjustment to due the end pieces
  minGappedK is the minimum gapped K amongst all matrices*/
static Nlm_FloatHi computeAdjustedQueryLength(Int4 queryLength, Nlm_FloatHi L,
    Nlm_FloatHi minGappedK)
{
   return(MAX((queryLength - L), 1/minGappedK));
}

/*compute  a term L used as an
  intermediate term in the calculation of e-values
  L is the last intermediate term that can be caluclated based
  on just the matrices and their Karlin-Altschul parameters
  matrixAuxiliaryFile is a file descriptor for a file with
  some Karlin-ALtschul parameters for each matrix
  query is the query sequence
  queryLength is the the length in amino acids of query
  matrixName is the underlying score matrix; it is used to check that
    the same underlying matrix was used in preprocessing
  numProfiles is the number of profiles in the database
  the adjusted database length is return in adjustedDbLength
  minGappedK holds the minimum gapped K among all the different
    position-specific matrices
    proDemographics keeps some statistical information about this program run*/
static Nlm_FloatHi computeL(FILE *matrixAuxiliaryFile, Uint1Ptr query, Int4 queryLength, Int4 numProfiles, Nlm_FloatHi *minGappedK, proDemographicsItems *proDemographics)
{
  Int4 dbLength; /*sum of actual lengths of database sequences*/
  Int4 Nestimate; /*estimate of effective search space size*/
  Char *underlyingMatrixName; /*name of basic matrix used for preprocessing*/
  Nlm_FloatHi returnL; /*value of L to return*/
  Int4 *dbSeqLengths; /*array of lengths of profile sequences*/
  Nlm_FloatHi *gappedKArray; /*gapped K values for profile matrices*/
  Int4 profileIndex; /*index over profiles*/
  Int4 gap_open, gap_extend; /*used to scan in auxiliary file*/
  Nlm_FloatHi Kungapped, Hungapped; /*H and K for ungapped alignments with
				      each matrix*/
  Nlm_FloatHi scalingFactor; /*used to read in auxiliary file*/
  Int4 tempDbLength;  /*used to estimate total length of all profiles*/
  BLAST_ScoreBlkPtr sbp;
  Int4 maxLength, sequenceDbLength; /*used to read in extra data*/
  Int4 queryLengthMinusX; /*query length without X characters*/
  Int4 i; /*loop index*/

   /*check that preprocessing and main program are using the same underlying
     matrix*/
   underlyingMatrixName = MemNew(MAXLINELEN * sizeof(Char));
   fscanf(matrixAuxiliaryFile,"%s",underlyingMatrixName);
   fscanf(matrixAuxiliaryFile,"%d\n", &gap_open);
   fscanf(matrixAuxiliaryFile,"%d\n", &gap_extend);
   fscanf(matrixAuxiliaryFile, "%le", &Kungapped);
   fscanf(matrixAuxiliaryFile, "%le", &Hungapped);
   sbp = BLAST_ScoreBlkNew(Seq_code_ncbistdaa, 1);
   sbp->read_in_matrix = TRUE;
   sbp->protein_alphabet = TRUE;
   sbp->posMatrix = NULL;
   sbp->number_of_contexts = 1;
   BlastScoreBlkMatFill(sbp, underlyingMatrixName);       
   BlastScoreBlkFill(sbp,  (CharPtr) query, queryLength, 0);
   Kungapped = sbp->kbp_std[0]->K;
   Hungapped = sbp->kbp_std[0]->H;
   BLAST_ScoreBlkDestruct(sbp);
   MemFree(underlyingMatrixName);


   fscanf(matrixAuxiliaryFile, "%d", &maxLength);
   fscanf(matrixAuxiliaryFile, "%d", &sequenceDbLength);
   fscanf(matrixAuxiliaryFile, "%lf", &scalingFactor);

   queryLengthMinusX = queryLength;
   for(i = 0; i < queryLength; i++)
     if (Xchar == query[i])
       queryLengthMinusX--;


   dbSeqLengths = (Int4 *) MemNew(numProfiles * sizeof(Int4));
   gappedKArray = (Nlm_FloatHi *) MemNew(numProfiles * sizeof(Nlm_FloatHi));
   dbLength = 0;
   *minGappedK = 1.0;
   for(profileIndex = 0; profileIndex < numProfiles; profileIndex++) {
     fscanf(matrixAuxiliaryFile, "%d", &(dbSeqLengths[profileIndex]));
     dbLength += dbSeqLengths[profileIndex];
     fscanf(matrixAuxiliaryFile, "%le", &(gappedKArray[profileIndex]));
     if (gappedKArray[profileIndex] < (*minGappedK))
       *minGappedK = gappedKArray[profileIndex];
   }
   Nestimate = queryLengthMinusX * dbLength;
   proDemographics->queryLength = queryLength;
   proDemographics->dbLength = dbLength;
   returnL = log(((Nlm_FloatHi) Nestimate) * Kungapped) /Hungapped;
   if (((0 == sequenceDbLength) && (proDemographics->effDbLength <= 0))
        || (0 == proDemographics->effDbLength)) {
     tempDbLength = 0;
     for(profileIndex = 0; profileIndex < numProfiles; profileIndex++) {
     tempDbLength += MAX((dbSeqLengths[profileIndex] - returnL),
                       1/ gappedKArray[profileIndex]);
     }
     proDemographics->effDbLength = tempDbLength;
   }
   else
     if ((-1) == proDemographics->effDbLength)
       proDemographics->effDbLength = sequenceDbLength;
   proDemographics->effSearchSpace = proDemographics->effDbLength *
       computeAdjustedQueryLength(queryLengthMinusX, returnL, *minGappedK);
   MemFree(dbSeqLengths);
   MemFree(gappedKArray);
   return(returnL);
}

/*findProfileParams scans matrixAuxiliaryFile to find the
  maximum number of positions among all the position-specific matrices
  it returns the underlying matrix name, but also passes back the
  maximum length, the total length, the scaling and the 
  unscaled gap costs*/
static Char * findProfileParams(FILE *matrixAuxiliaryFile, Int4 numProfiles,
   Int4 *return_gap_open, Int4 *return_gap_extend,  Int4* returnMaxLength,
   Int4 * returnTotalLength, Int4 * returnSequenceDbLength, Nlm_FloatHi *returnScaling)
{
   Int4 maxLength; /*maximum length of sequence*/
   Int4 thisLength; /*length of next sequence*/
   Int4 totalLength; /*total length to return*/
   Int4 i; /*loop index*/
   Nlm_FloatHi Kungapped, Hungapped; /*two values to read*/
   Char * underlyingMatrixName; /*name of matrix to read*/
   Int4 sequenceDbSize; /*used to read in size of underlying sequence db*/
   Int4 local_gap_open, local_gap_extend; /*used to read in gap costs*/
   Nlm_FloatHi scalingFactor; /*factor by which matrix scores are scaled*/

   underlyingMatrixName = MemNew(MAXLINELEN * sizeof(Char));
   fscanf(matrixAuxiliaryFile,"%s",underlyingMatrixName);
   fscanf(matrixAuxiliaryFile,"%d\n", &local_gap_open);
   fscanf(matrixAuxiliaryFile,"%d\n", &local_gap_extend);
   fscanf(matrixAuxiliaryFile, "%le", &Kungapped); 
   fscanf(matrixAuxiliaryFile, "%le", &Hungapped);
   fscanf(matrixAuxiliaryFile, "%d", &maxLength);
   fscanf(matrixAuxiliaryFile, "%d", &sequenceDbSize);
   fscanf(matrixAuxiliaryFile, "%lf\n", &scalingFactor);
   totalLength = 0;
   for (i = 0; i < numProfiles; i++) {
     fscanf(matrixAuxiliaryFile, "%d", &thisLength);
     fscanf(matrixAuxiliaryFile, "%le", &Kungapped);
     totalLength += thisLength;
   }
   rewind(matrixAuxiliaryFile);
   *returnTotalLength = totalLength;
   *returnMaxLength = maxLength;
   *returnScaling = scalingFactor;
   *return_gap_open = local_gap_open;
   *return_gap_extend = local_gap_extend;
   *returnSequenceDbLength = sequenceDbSize;
   return(underlyingMatrixName);
}

/*computes Smith-Waterman local alignment score and returns the
  evalue
  query is the query sequence
  queryLength is the length of query in amino acids
  dbSequence is the sequence corresponding to some matrix profile
  dbLength is the length of dbSequnece
  matrix is the position-specific matrix associated with dbSequence
  gapOpen is the cost of opening a gap
  gapExtend is the cost of extending an exisiting gap by 1 position
  queryEnd returns the final position in the query of an optimal
   local alignment
  dbEnd returns the final position in dbSequence of an optimal
   local alignment
  queryEnd and dbEnd can be used to run the local alignment in reverse
   to find optimal starting positions
  score is used to pass back the optimal score
  kbp holds the Karlin-Altschul paramters
  L holds an intermediate term for E-value computation
  adjustedDbLength is the adjusted database length used for e-value computation
  minGappedK holds the minimum gapped K for all matrices in the
  database, and is used for e-value computation */

static Nlm_FloatHi basicSmithWatermanScoreOnly(Uint1 * query, 
   Int4 queryLength, Uint1 *dbSequence, Int4 dbLength, BLAST_Score **matrix, 
   Int4 gapOpen, Int4 gapExtend,  Int4 *queryEnd, Int4 *dbEnd, Int4 *score,
   BLAST_KarlinBlkPtr kbp, Nlm_FloatHi L, Nlm_FloatHi effSearchSpace,
   Nlm_FloatHi minGappedK)
{

   Int4 bestScore; /*best score seen so far*/
   Int4 newScore;  /* score of next entry*/
   Int4 bestQueryPos, bestDbPos; /*position ending best score in
                           query and database sequences*/
   SWpairs *scoreVector; /*keeps one row of the Smith-Waterman matrix
                           overwrite old row with new row*/
   BLAST_Score *matrixRow; /*one row of score matrix*/
   Int4 newGapCost; /*cost to have a gap of one character*/
   Int4 prevScoreNoGapQuery; /*score one row and column up
                               with no gaps*/
   Int4 prevScoreGapQuery;   /*score if a gap already started in query*/
   Int4 continueGapScore; /*score for continuing a gap in dbSequence*/
   Int4 queryPos, dbPos; /*positions in query and dbSequence*/
   Nlm_FloatHi returnEvalue; /*e-value to return*/


   scoreVector = (SWpairs *) MemNew(queryLength * sizeof(SWpairs));
   bestQueryPos = 0;
   bestDbPos = 0;
   bestScore = 0;
   newGapCost = gapOpen + gapExtend;
   for (queryPos = 0; queryPos < queryLength; queryPos++) {
     scoreVector[queryPos].noGap = 0;
     scoreVector[queryPos].gapExists = -(gapOpen);
   }
   for(dbPos = 0; dbPos < dbLength; dbPos++) {  
     matrixRow = matrix[dbPos];
     newScore = 0;
     prevScoreNoGapQuery = 0;
     prevScoreGapQuery = -(gapOpen);
     for(queryPos = 0; queryPos < queryLength; queryPos++) {
       /*testing scores with a gap in query, either starting a new
         gap or extending an existing gap*/
       if ((newScore = newScore - newGapCost) > 
	   (prevScoreGapQuery = prevScoreGapQuery - gapExtend))
         prevScoreGapQuery = newScore;
       /*testing scores with a gap in dbSequence, either starting a new
         gap or extending an existing gap*/
       if ((newScore = scoreVector[queryPos].noGap - newGapCost) >
           (continueGapScore = scoreVector[queryPos].gapExists - gapExtend))
         continueGapScore = newScore;
       /*compute new score extending one position in query and dbSequence*/
       newScore = prevScoreNoGapQuery + matrixRow[query[queryPos]];
       if (newScore < 0)
       newScore = 0; /*Smith-Waterman locality condition*/
       /*test two alternatives*/
       if (newScore < prevScoreGapQuery)
         newScore = prevScoreGapQuery;
       if (newScore < continueGapScore)
         newScore = continueGapScore;
       prevScoreNoGapQuery = scoreVector[queryPos].noGap; 
       scoreVector[queryPos].noGap = newScore;
       scoreVector[queryPos].gapExists = continueGapScore;
       if (newScore > bestScore) {
         bestScore = newScore;
         bestDbPos = dbPos;
         bestQueryPos = queryPos;
       }
     }
   }
   MemFree(scoreVector);
   if (bestScore < 0)
     bestScore = 0;
   *queryEnd = bestQueryPos;
   *dbEnd = bestDbPos;
   *score = bestScore;
   returnEvalue = scoreToEvalue(effSearchSpace, bestScore,  kbp);
   return(returnEvalue);
}

/*computes where optimal Smith-Waterman local alignment starts given the
  ending positions
  query is the query sequence
  queryLength is the length of query in amino acids
  dbSequence is the sequence corresponding to some matrix profile
  dbLength is the length of dbSequnece
  matrix is the position-specific matrix associated with dbSequence
  gapOpen is the cost of opening a gap
  gapExtend is the cost of extending an exisiting gap by 1 position
  queryEnd is the final position in the query of an optimal
   local alignment
  dbEnd is the final position in dbSequence of an optimal
   local alignment
  queryEnd and dbEnd can be used to run the local alignment in reverse
   to find optimal starting positions
  these are passed back in queryStart and dbStart
  the optimal score is passed in to check when it has
   been reached going backwards
  the score is also returned
  */
static Int4 SmithWatermanFindStartGivenEnd(Uint1 * query, 
   Int4 queryLength, Uint1 *dbSequence, Int4 dbLength, BLAST_Score **matrix, 
   Int4 gapOpen, Int4 gapExtend,  Int4 queryEnd, Int4 dbEnd, Int4 score,
   Int4 *queryStart, Int4 *dbStart)
{

   Int4 bestScore; /*best score seen so far*/
   Int4 newScore;  /* score of next entry*/
   Int4 bestQueryPos, bestDbPos; /*position starting best score in
                           query and database sequences*/
   SWpairs *scoreVector; /*keeps one row of the Smith-Waterman matrix
                           overwrite old row with new row*/
   BLAST_Score *matrixRow; /*one row of score matrix*/
   Int4 newGapCost; /*cost to have a gap of one character*/
   Int4 prevScoreNoGapQuery; /*score one row and column up
                               with no gaps*/
   Int4 prevScoreGapQuery;   /*score if a gap already started in query*/
   Int4 continueGapScore; /*score for continuing a gap in dbSequence*/
   Int4 queryPos, dbPos; /*positions in query and dbSequence*/

   scoreVector = (SWpairs *) MemNew(queryLength * sizeof(SWpairs));
   bestQueryPos = 0;
   bestDbPos = 0;
   bestScore = 0;
   newGapCost = gapOpen + gapExtend;
   for (queryPos = 0; queryPos < queryLength; queryPos++) {
     scoreVector[queryPos].noGap = 0;
     scoreVector[queryPos].gapExists = -(gapOpen);
   }
   for(dbPos = dbEnd; dbPos >= 0; dbPos--) {  
     matrixRow = matrix[dbPos];
     newScore = 0;
     prevScoreNoGapQuery = 0;
     prevScoreGapQuery = -(gapOpen);
     for(queryPos = queryEnd; queryPos >= 0; queryPos--) {
       /*testing scores with a gap in query, either starting a new
         gap or extending an existing gap*/
       if ((newScore = newScore - newGapCost) > 
	   (prevScoreGapQuery = prevScoreGapQuery - gapExtend))
         prevScoreGapQuery = newScore;
       /*testing scores with a gap in dbSequence, either starting a new
         gap or extending an existing gap*/
       if ((newScore = scoreVector[queryPos].noGap - newGapCost) >
           (continueGapScore = scoreVector[queryPos].gapExists - gapExtend))
         continueGapScore = newScore;
       /*compute new score extending one position in query and dbSequence*/
       newScore = prevScoreNoGapQuery + matrixRow[query[queryPos]];
       if (newScore < 0)
       newScore = 0; /*Smith-Waterman locality condition*/
       /*test two alternatives*/
       if (newScore < prevScoreGapQuery)
         newScore = prevScoreGapQuery;
       if (newScore < continueGapScore)
         newScore = continueGapScore;
       prevScoreNoGapQuery = scoreVector[queryPos].noGap; 
       scoreVector[queryPos].noGap = newScore;
       scoreVector[queryPos].gapExists = continueGapScore;
       if (newScore > bestScore) {
         bestScore = newScore;
         bestDbPos = dbPos;
         bestQueryPos = queryPos;
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
   *queryStart = bestQueryPos;
   *dbStart = bestDbPos;
   return(bestScore);
}


/*computes Smith-Waterman local alignment score and returns the
  evalue assuming some positions are forbidden
  query is the query sequence
  queryLength is the length of query in amino acids
  dbSequence is the sequence corresponding to some matrix profile
  dbLength is the length of dbSequnece
  matrix is the position-specific matrix associated with dbSequence
  gapOpen is the cost of opening a gap
  gapExtend is the cost of extending an exisiting gap by 1 position
  queryEnd returns the final position in the query of an optimal
   local alignment
  dbEnd returns the final position in dbSequence of an optimal
   local alignment
  queryEnd and dbEnd can be used to run the local alignment in reverse
   to find optimal starting positions
  score is used to pass back the optimal score
  kbp holds the Karlin-Altschul paramters
  L holds an intermediate term for E-value computation
  adjustedDbLength is the adjusted database length used for e-value computation
  minGappedK holds the minimum gapped K for all matrices in the
  database, and is used for e-value computation */

static Nlm_FloatHi specialSmithWatermanScoreOnly(Uint1 * query, 
   Int4 queryLength, Uint1 *dbSequence, Int4 dbLength, BLAST_Score **matrix, 
   Int4 gapOpen, Int4 gapExtend,  Int4 *queryEnd, Int4 *dbEnd, Int4 *score,
   BLAST_KarlinBlkPtr kbp, Nlm_FloatHi L, Nlm_FloatHi effSearchSpace,
   Nlm_FloatHi minGappedK, Int4 *numForbidden, Int4 ** forbiddenRanges)
{

   Int4 bestScore; /*best score seen so far*/
   Int4 newScore;  /* score of next entry*/
   Int4 bestQueryPos, bestDbPos; /*position ending best score in
                           query and database sequences*/
   SWpairs *scoreVector; /*keeps one row of the Smith-Waterman matrix
                           overwrite old row with new row*/
   BLAST_Score *matrixRow; /*one row of score matrix*/
   Int4 newGapCost; /*cost to have a gap of one character*/
   Int4 prevScoreNoGapQuery; /*score one row and column up
                               with no gaps*/
   Int4 prevScoreGapQuery;   /*score if a gap already started in query*/
   Int4 continueGapScore; /*score for continuing a gap in dbSequence*/
   Int4 queryPos, dbPos; /*positions in query and dbSequence*/
   Nlm_FloatHi returnEvalue; /*e-value to return*/
   Boolean forbidden; /*is this position forbidden?*/
   Int4 f; /*index over forbidden positions*/


   scoreVector = (SWpairs *) MemNew(queryLength * sizeof(SWpairs));
   bestQueryPos = 0;
   bestDbPos = 0;
   bestScore = 0;
   newGapCost = gapOpen + gapExtend;
   for (queryPos = 0; queryPos < queryLength; queryPos++) {
     scoreVector[queryPos].noGap = 0;
     scoreVector[queryPos].gapExists = -(gapOpen);
   }
   for(dbPos = 0; dbPos < dbLength; dbPos++) {  
     matrixRow = matrix[dbPos];
     newScore = 0;
     prevScoreNoGapQuery = 0;
     prevScoreGapQuery = -(gapOpen);
     for(queryPos = 0; queryPos < queryLength; queryPos++) {
       /*testing scores with a gap in query, either starting a new
         gap or extending an existing gap*/
       if ((newScore = newScore - newGapCost) > 
	   (prevScoreGapQuery = prevScoreGapQuery - gapExtend))
         prevScoreGapQuery = newScore;
       /*testing scores with a gap in dbSequence, either starting a new
         gap or extending an existing gap*/
       if ((newScore = scoreVector[queryPos].noGap - newGapCost) >
           (continueGapScore = scoreVector[queryPos].gapExists - gapExtend))
         continueGapScore = newScore;
       /*compute new score extending one position in query and dbSequence*/
       forbidden = FALSE;
       for(f = 0; f < numForbidden[dbPos]; f++) {
         if ((queryPos >= forbiddenRanges[dbPos][2 * f]) &&
	     (queryPos <= forbiddenRanges[dbPos][2*f + 1])) {
	   forbidden = TRUE;
	   break;
	 }
       }
       if (forbidden)
         newScore = BLAST_SCORE_MIN;
       else
	 newScore = prevScoreNoGapQuery + matrixRow[query[queryPos]];
       if (newScore < 0)
	 newScore = 0; /*Smith-Waterman locality condition*/
       /*test two alternatives*/
       if (newScore < prevScoreGapQuery)
	 newScore = prevScoreGapQuery;
       if (newScore < continueGapScore)
	 newScore = continueGapScore;
       prevScoreNoGapQuery = scoreVector[queryPos].noGap; 
       scoreVector[queryPos].noGap = newScore;
       scoreVector[queryPos].gapExists = continueGapScore;
       if (newScore > bestScore) {
	 bestScore = newScore;
	 bestDbPos = dbPos;
	 bestQueryPos = queryPos;

       }
     }
   }
   MemFree(scoreVector);
   if (bestScore < 0)
     bestScore = 0;
   *queryEnd = bestQueryPos;
   *dbEnd = bestDbPos;
   *score = bestScore;
   returnEvalue = scoreToEvalue(effSearchSpace, bestScore,  kbp);
   return(returnEvalue);
}

/*computes where optimal Smith-Waterman local alignment starts given the
  ending positions
  query is the query sequence
  queryLength is the length of query in amino acids
  dbSequence is the sequence corresponding to some matrix profile
  dbLength is the length of dbSequnece
  matrix is the position-specific matrix associated with dbSequence
  gapOpen is the cost of opening a gap
  gapExtend is the cost of extending an exisiting gap by 1 position
  queryEnd is the final position in the query of an optimal
   local alignment
  dbEnd is the final position in dbSequence of an optimal
   local alignment
  queryEnd and dbEnd can be used to run the local alignment in reverse
   to find optimal starting positions
  these are passed back in queryStart and dbStart
  the optimal score is passed in to check when it has
   been reached going backwards
  the score is also returned
  */
static Int4 specialSmithWatermanFindStartGivenEnd(Uint1 * query, 
   Int4 queryLength, Uint1 *dbSequence, Int4 dbLength, BLAST_Score **matrix, 
   Int4 gapOpen, Int4 gapExtend,  Int4 queryEnd, Int4 dbEnd, Int4 score,
   Int4 *queryStart, Int4 *dbStart, Int4 *numForbidden, 
   Int4 ** forbiddenRanges)
{

   Int4 bestScore; /*best score seen so far*/
   Int4 newScore;  /* score of next entry*/
   Int4 bestQueryPos, bestDbPos; /*position starting best score in
                           query and database sequences*/
   SWpairs *scoreVector; /*keeps one row of the Smith-Waterman matrix
                           overwrite old row with new row*/
   BLAST_Score *matrixRow; /*one row of score matrix*/
   Int4 newGapCost; /*cost to have a gap of one character*/
   Int4 prevScoreNoGapQuery; /*score one row and column up
                               with no gaps*/
   Int4 prevScoreGapQuery;   /*score if a gap already started in query*/
   Int4 continueGapScore; /*score for continuing a gap in dbSequence*/
   Int4 queryPos, dbPos; /*positions in query and dbSequence*/
   Boolean forbidden; /*is this position forbidden?*/
   Int4 f; /*index over forbidden positions*/

   scoreVector = (SWpairs *) MemNew(queryLength * sizeof(SWpairs));
   bestQueryPos = 0;
   bestDbPos = 0;
   bestScore = 0;
   newGapCost = gapOpen + gapExtend;
   for (queryPos = 0; queryPos < queryLength; queryPos++) {
     scoreVector[queryPos].noGap = 0;
     scoreVector[queryPos].gapExists = -(gapOpen);
   }
   for(dbPos = dbEnd; dbPos >= 0; dbPos--) {  
     matrixRow = matrix[dbPos];
     newScore = 0;
     prevScoreNoGapQuery = 0;
     prevScoreGapQuery = -(gapOpen);
     for(queryPos = queryEnd; queryPos >= 0; queryPos--) {
       /*testing scores with a gap in query, either starting a new
         gap or extending an existing gap*/
       if ((newScore = newScore - newGapCost) > 
	   (prevScoreGapQuery = prevScoreGapQuery - gapExtend))
         prevScoreGapQuery = newScore;
       /*testing scores with a gap in dbSequence, either starting a new
         gap or extending an existing gap*/
       if ((newScore = scoreVector[queryPos].noGap - newGapCost) >
           (continueGapScore = scoreVector[queryPos].gapExists - gapExtend))
         continueGapScore = newScore;
       /*compute new score extending one position in query and dbSequence*/
       forbidden = FALSE;
       for(f = 0; f < numForbidden[dbPos]; f++) {
         if ((queryPos >= forbiddenRanges[dbPos][2 * f]) &&
	     (queryPos <= forbiddenRanges[dbPos][2*f + 1])) {
	   forbidden = TRUE;
	   break;
	 }
       }
       if (forbidden)
         newScore = BLAST_SCORE_MIN;
       else
	 newScore = prevScoreNoGapQuery + matrixRow[query[queryPos]];
       if (newScore < 0)
       newScore = 0; /*Smith-Waterman locality condition*/
       /*test two alternatives*/
       if (newScore < prevScoreGapQuery)
         newScore = prevScoreGapQuery;
       if (newScore < continueGapScore)
         newScore = continueGapScore;
       prevScoreNoGapQuery = scoreVector[queryPos].noGap; 
       scoreVector[queryPos].noGap = newScore;
       scoreVector[queryPos].gapExists = continueGapScore;
       if (newScore > bestScore) {
         bestScore = newScore;
         bestDbPos = dbPos;
         bestQueryPos = queryPos;
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
   *queryStart = bestQueryPos;
   *dbStart = bestDbPos;
   return(bestScore);
}


/*converts the list of Smith-Waterman alignments to a corresponding list
  of SeqAlignPtrs. kbp stores parameters for computing the score
  Code is adapted from procedure output_hits of pseed3.c */
static SeqAlignPtr convertSWalignsToSeqAligns(SWResults * SWAligns,  Uint1Ptr query,
  SeqIdPtr query_id)
{
    SeqAlignPtr seqAlignList =NULL; /*list of SeqAligns to return*/
    SeqAlignPtr nextSeqAlign; /*new one to add*/
    SeqAlignPtr lastSeqAlign = NULL; /*last one on list*/
    SWResults *curSW; /*used to iterate down list*/
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
      nextSeqAlign = GapXEditBlockToSeqAlign(nextEditBlock, curSW->subject_id, query_id);
      nextSeqAlign->score = addScoresToSeqAlign(curSW->scoreThisAlign, 
           curSW->eValueThisAlign, curSW->Lambda, curSW->logK);
      if (NULL == seqAlignList)
        seqAlignList = nextSeqAlign;
      else
        lastSeqAlign->next = nextSeqAlign;
      lastSeqAlign = nextSeqAlign;
      curSW = curSW->next;
    }
    return(seqAlignList);
}

/*top level procedure to find all profiles that match the input query
  matrixAuxiliaryFile is a file descriptor for a file
    containing information about each matrix profile
  matricesFileName has the list of file names, one per matrix
  sequencesFile is afile descriptor for a file with the list of file names, 
          one per sequence
  numProfiles is the number of profiles in the database
  maxlength is the maximum length of a profile
  totalLength is the total length of the profile database
  initialUngappedLambda is the precomputed ungapped lambda for this
    combination of underlying matrix and gap costs
  querySequence is the query protein sequence
  queryLength is the length of querySequence in amino acids
  gapOpen is the cost of openign a gap
  gapExtend is the cost of extending a gap by one position
  x_dropoff is used to find the final gapped alignment using
     ALIGN in gapxdrop.c
  ethreshfirst is used to make a preliminary estimate of which sequences
  may be interesting
  ethresh is the E-value threshold for which matches are printable
  matrixName is the name of the underlyign score matrix
  query_id is used to be able to print alignments involving
     querySequence
  maxLength is the maximum length of a matrix profile
  matrixTemplate is pre-allocated memory to hold each position-specific
  score amtrix
  proDemographics stores statistical information about the run */
SeqAlignPtr findMatchingProfiles(FILE *matrixAuxiliaryFile,
      Char *bigMatrixFileName, Char *matrixListFileName,
      FILE *sequencesFile, Int4 numProfiles, Int4 maxLength, Int4 totalLength,
      Nlm_FloatHi initialUngappedLambda, Nlm_FloatHi scalingFactor,
      Uint1Ptr querySequence, Int4 queryLength,
      Int4 gapOpen, Int4 gapExtend, Int4 x_dropoff, Nlm_FloatHi ethresh,
      Nlm_FloatHi ethreshfirst, SeqIdPtr query_id, 
      proDemographicsItems * proDemographics, Char *directoryPrefix)
{
   SeqAlignPtr results = NULL; /*holds return value accumulated one alignment
                                 at a time*/
   FILE *matricesFile; /*file listing where to find score matrices*/
   Char oneSequenceFileName[MAXLINELEN], oneMatrixFileName[MAXLINELEN];
   /*corresponding file names containing one sequence and one profile*/
   Char relativeSequenceFileName[MAXLINELEN], relativeMatrixFileName[MAXLINELEN];
   /*file names as read in, these will be appended to the directoryPrefix*/
   Int4 c1,c2; /*loop index over characters*/
   Int4 prefixLength; /*length of directoryPrefix*/
   FILE *thisMatrixFile, *thisSequenceFile; /*files for matrix and sequence*/
   Int4 i; /* Index over sequences/profiles*/
   Uint1 *dbSequence; /*sequence representing a profile*/
   Int4 dbSequenceLength; /*length of dbSequence*/
   Int4 dbStart, queryStart; /*starting positions for local alignment*/
   ScoreRow *bigMatrix; /*stores combined big matrix*/
   BLAST_Score **posMatrix; /*matrix for this profile*/
   BLAST_Score **copyMatrix; /*matrix used for scaling*/
   BLAST_KarlinBlkPtr kbp; /*stores Karlin-Altschul parameters*/
   SWResults *SWAligns=NULL, *newSW;  /*keeps list of matching alignments*/
   Int4 numAligns; /*number of matching alignments*/
   Nlm_FloatHi firstEvalue, lowerBoundEvalue, thisEvalue, bestEvalue; 
       /*e-values for one profile, first two are estimates*/
   Int4 bestScore, newScore; /*best score for this match, score for 1 alignment*/
   Int4 queryEnd, dbEnd, finalQueryEnd, finalDbEnd; /*end positions of optimal local alignment*/
   Int4 score; /*score of optimal local alignment*/
   Int4 *alignScript, *reverseAlignScript; 
        /*edit script that describes pairwise alignment*/
   Int4 *ASptr, *revASptr, temp; /*pointers/indices to the 2 alignment scripts*/
   GapAlignBlkPtr gap_align; /*keeps track of gapped alignment parameters*/
   Int4 XdropAlignScore; /*alignment score obtained using
			   X-dropoff method rather than Smith-Waterman*/
   Nlm_FloatHi L, adjustedDbLength, minGappedK; /*variables for e-value caluclation*/
   Int4 tickInterval, lastTick; /*for deciding when to print a progress tick*/
   SeqIdPtr subject_id;
   Int4 startPos; /*position in overall big matrix*/
   Nlm_MemMapPtr mmapResult; /*result of mmap operataion*/
   CharPtr  addressForMatrix;  /*address for memory mapped files*/
   FILE * matrixFileDesc; /*file descriptor for matrix file */
   Int4 p, c, row; /* loop indices*/
   Nlm_FloatHi LambdaRatio; /*ratio of ungapped lambdas for correcting matrix*/
   Nlm_FloatHi temp1; /*intermediate variable for adjusting matrix*/
   Int4 temp2; /*intermediate variable for adjusting matrix*/
   Nlm_FloatHi *resProb; /*array of probabilities for each residue*/
   BLAST_ScoreFreqPtr this_sfp, return_sfp; /*score frequency pointers to
                                              compute lambda*/
   Nlm_FloatHi correctUngappedLambda; /*new value of ungapped lambda*/
   Nlm_FloatHi scaledInitialUngappedLambda; /*initialUngappedLambda divided
                                              by scalingFactor*/
   Nlm_FloatHi *scoreArray; /*array of score probabilities*/
   Int4 doublingCount; /*number of times X-dropoff had to be doubled*/
   Int4 *numForbidden; /*how many forbidden ranges at each db position*/
   Int4 **forbiddenRanges; /*forbidden ranges for each database position*/
   Int4 f; /* index for numForbidden and forbiddenRanges*/
   Int4 tempIndex; /*index used to copy over rows of forbiddenRanges*/
   Int4 *tempForbidden; /*used to copy over rows of forbiddenRanges*/
   Boolean foundMatchForThisMatrix; /*did we already find a match*/

   resProb = (Nlm_FloatHi *) MemNew (PRO_ALPHABET_SIZE * sizeof(Nlm_FloatHi));
   scoreArray = (Nlm_FloatHi *) MemNew(scoreRange * sizeof(Nlm_FloatHi));
   return_sfp = (BLAST_ScoreFreqPtr) MemNew(1 * sizeof(BLAST_ScoreFreq));

   if ((matricesFile = FileOpen(matrixListFileName, "r")) == NULL)  {
     ErrPostEx(SEV_FATAL, 1, 0, "profiles: Unable to open file with all matrices %s\n", matrixListFileName);
	return (NULL);
   }
   if ((matrixFileDesc = FileOpen(bigMatrixFileName, "rb")) == NULL)  {
     ErrPostEx(SEV_FATAL, 1, 0, "profiles: Unable to open file with all matrices %s\n", bigMatrixFileName);
	return (NULL);
   }
   FileClose(matrixFileDesc);
   if (0 == numProfiles)
     return(NULL);

   kbp =  BlastKarlinBlkCreate();
   numAligns = 0;
   gap_align = GapAlignBlkNew(1,1);
   gap_align->gap_open = Nlm_Nint(gapOpen * scalingFactor);
   gap_align->gap_extend =  Nlm_Nint(gapExtend * scalingFactor);
   gap_align->decline_align = (-(BLAST_SCORE_MIN));
   gap_align->matrix = NULL;
   gap_align->positionBased = TRUE;
   L =  computeL(matrixAuxiliaryFile, querySequence, queryLength, 
        numProfiles, &minGappedK, proDemographics);

   lastTick = 0;
   tickInterval = numProfiles / PRO_NUM_TICKS;
   proDemographics->numSequencesTested = numProfiles;

   /* addressForMatrix = mmap((caddr_t) 0, totalLength * sizeof(ScoreRow), PROT_READ,
              MAP_PRIVATE, matrixFileDesc, 0);
   if (addressForMatrix == MAP_FAILED) {
       ErrPostEx(SEV_FATAL, 1, 0, "profiles: mmap failed\n");
	return (NULL);
   } */
   mmapResult =  Nlm_MemMapInit(bigMatrixFileName);
   if (NULL == mmapResult) {
       ErrPostEx(SEV_FATAL, 1, 0, "profiles: mmap failed\n");
	return (NULL);
   } 
   addressForMatrix = mmapResult -> mmp_begin;
   bigMatrix = (ScoreRow *) addressForMatrix;
   startPos = 0;
   posMatrix = (BLAST_Score **) MemNew(maxLength * sizeof(BLAST_Score *));
   copyMatrix = (BLAST_Score **) MemNew(maxLength * sizeof(BLAST_Score *));
   for (p = 0; p < maxLength; p++)
     copyMatrix[p] = (BLAST_Score *) MemNew(PRO_ALPHABET_SIZE * sizeof(BLAST_Score ));


   numForbidden = (Int4 *) MemNew(maxLength * sizeof(Int4));
   forbiddenRanges = (Int4 **) MemNew(maxLength * sizeof(Int4 *));
   for (f = 0; f < maxLength; f++) {
     numForbidden[f] = 0;
     forbiddenRanges[f] = (Int4 *) MemNew(2 * sizeof(Int4));
     forbiddenRanges[f][0] = 0;
     forbiddenRanges[f][1] = 0;
   }

   if ('\0' != directoryPrefix[0]) {
     strcpy(oneSequenceFileName, directoryPrefix);
     strcpy(oneMatrixFileName, directoryPrefix);
     prefixLength = strlen(directoryPrefix);
   }     
   for(i = 0; i < numProfiles; i++) {     
     if (i > (lastTick + tickInterval)) {
       lastTick = i;
       tick_callback(i, 0);
     }
     foundMatchForThisMatrix = FALSE;
     if ('\0' == directoryPrefix[0])
       fscanf(sequencesFile,"%s", oneSequenceFileName); 
     else {
       fscanf(sequencesFile,"%s", relativeSequenceFileName); 
       for(c1 = prefixLength, c2 = 0; relativeSequenceFileName[c2] != '\0';
          c1++, c2++)
         oneSequenceFileName[c1] = relativeSequenceFileName[c2];
       oneSequenceFileName[c1] = '\0';
     }
     if ((thisSequenceFile = FileOpen(oneSequenceFileName, "r")) == NULL)  {
       ErrPostEx(SEV_FATAL, 1, 0, "profiles: Unable to open sequence file %s\n", oneSequenceFileName);
	return (NULL);
     }
     if ('\0' == directoryPrefix[0])
       fscanf(matricesFile,"%s", oneMatrixFileName); 
     else {
       fscanf(matricesFile,"%s", relativeMatrixFileName); 
       for(c1 = prefixLength, c2 = 0; relativeMatrixFileName[c2] != '\0';
          c1++, c2++)
         oneMatrixFileName[c1] = relativeMatrixFileName[c2];
       oneMatrixFileName[c1] = '\0';
     }
     if ((thisMatrixFile = FileOpen(oneMatrixFileName, "r")) == NULL)  {
       ErrPostEx(SEV_FATAL, 1, 0, "profiles: Unable to open matrix file %s\n", oneMatrixFileName);
	return (NULL);
     }
     dbSequence = readSequence(thisSequenceFile, &dbSequenceLength, 
			       &subject_id, FALSE, maxLength+2, i+1);

     readkbp(thisMatrixFile, dbSequenceLength, kbp, oneSequenceFileName);

     for (p = 0, row = startPos; p < dbSequenceLength; p++, row++)
        posMatrix[p] = (BLAST_Score *) &(bigMatrix[row][0]);
     
     do {
       if (foundMatchForThisMatrix)
	 firstEvalue = specialSmithWatermanScoreOnly(querySequence, queryLength, 
						     dbSequence, dbSequenceLength, posMatrix, gap_align->gap_open, 
						     gap_align->gap_extend,  
						     &queryEnd, &dbEnd, &score, kbp, L, (Nlm_FloatHi) 
						     proDemographics->effSearchSpace,  minGappedK, numForbidden, forbiddenRanges);
       else { 
	 firstEvalue = basicSmithWatermanScoreOnly(querySequence, queryLength, 
						   dbSequence, dbSequenceLength, posMatrix, gap_align->gap_open, 
						   gap_align->gap_extend,  
						   &queryEnd, &dbEnd, &score, kbp, L, (Nlm_FloatHi) 
						   proDemographics->effSearchSpace,  minGappedK);
	 FileClose(thisSequenceFile);
	 FileClose(thisMatrixFile);
       }
       if (firstEvalue < ethreshfirst) {        
	 if (!foundMatchForThisMatrix) {
	   IMPALAfillResidueProbability(querySequence, queryLength, resProb);
	   this_sfp =  IMPALAfillSfp(posMatrix, dbSequenceLength, resProb, scoreArray, return_sfp, scoreRange);
	   scaledInitialUngappedLambda = initialUngappedLambda/scalingFactor;
	   correctUngappedLambda = impalaKarlinLambdaNR(this_sfp, scaledInitialUngappedLambda);
	   LambdaRatio = correctUngappedLambda/scaledInitialUngappedLambda;
	 }
	 if (LambdaRatio > 0) {
	   lowerBoundEvalue = estimateEvalue(proDemographics->effSearchSpace, score,LambdaRatio, kbp);
	   if ((lowerBoundEvalue < (1.1 *ethresh)) || 
	       (correctUngappedLambda > scaledInitialUngappedLambda)) {
	     for (p = 0; p < dbSequenceLength; p++) {
	       for (c = 0; c < PRO_ALPHABET_SIZE; c++) {
		 if ((posMatrix[p][c] == BLAST_SCORE_MIN) || (Xchar == c))
		   copyMatrix[p][c] = posMatrix[p][c];
		 else {
		   temp1 = ((Nlm_FloatHi) (posMatrix[p][c]));
		   temp1 = temp1 * LambdaRatio;
		   temp2 = Nlm_Nint(temp1);
		   copyMatrix[p][c] = temp2;
		 }
	       }
	     }
	     if (foundMatchForThisMatrix) 
	       thisEvalue = specialSmithWatermanScoreOnly(querySequence, queryLength, 
							dbSequence, dbSequenceLength, copyMatrix, 
							gap_align->gap_open, gap_align->gap_extend,  
							&queryEnd, &dbEnd, &score, kbp, L, (Nlm_FloatHi) 
							proDemographics->effSearchSpace,  minGappedK, 
							numForbidden, forbiddenRanges);	 
	     else
	       thisEvalue = basicSmithWatermanScoreOnly(querySequence, queryLength, 
						      dbSequence, dbSequenceLength, copyMatrix, 
						      gap_align->gap_open, gap_align->gap_extend,  
						      &queryEnd, &dbEnd, &score, kbp, L, (Nlm_FloatHi) 
						      proDemographics->effSearchSpace,  minGappedK);	 
	     if (thisEvalue < ethresh) { 
	       if (!foundMatchForThisMatrix) 
		 proDemographics->numBetterThanEthresh++;
	       if (foundMatchForThisMatrix) 
		 specialSmithWatermanFindStartGivenEnd(querySequence, queryLength, 
						     dbSequence, dbSequenceLength, copyMatrix, 
						     gap_align->gap_open, gap_align->gap_extend,  
						     queryEnd,  dbEnd, score, &queryStart, &dbStart,
						     numForbidden, forbiddenRanges);
	       else
		 SmithWatermanFindStartGivenEnd(querySequence, queryLength, 
			  dbSequence, dbSequenceLength, copyMatrix, 
					      gap_align->gap_open, gap_align->gap_extend,  
					      queryEnd,  dbEnd, score, &queryStart, &dbStart);
	       gap_align->x_parameter = x_dropoff*NCBIMATH_LN2/kbp->Lambda;
	       gap_align->posMatrix = copyMatrix;
	       do {
		 doublingCount = 0;
		 alignScript = (Int4 *) MemNew((dbSequenceLength + queryLength + 3) * sizeof(Int4));
		 reverseAlignScript = alignScript;
		 proDemographics->numCallsALIGN++;
		 XdropAlignScore = ALIGN(&(dbSequence[dbStart]) - 1, 
				       &(querySequence[queryStart]) -1, 
				       dbEnd - dbStart + 1,
				       queryEnd - queryStart + 1,
				       reverseAlignScript , &finalDbEnd, &finalQueryEnd, &alignScript,
				       gap_align, dbStart - 1, FALSE); 
		 gap_align->x_parameter *=2;
		 doublingCount++;
		 if ((XdropAlignScore < score) && (doublingCount < 3)) 
		   MemFree(reverseAlignScript);
	       } while ((XdropAlignScore < score) && (doublingCount < 3));
               newScore = Nlm_Nint(((Nlm_FloatHi) XdropAlignScore) / scalingFactor);
	       thisSequenceFile = FileOpen(oneSequenceFileName, "r");
	       dbSequence = readSequence(thisSequenceFile, &dbSequenceLength, 
					 &subject_id, TRUE, dbSequenceLength+2, i+1);
	       FileClose(thisSequenceFile);
	       for (revASptr = reverseAlignScript, ASptr = alignScript-1; 
		    revASptr <= ASptr; revASptr++) {
		 /* complement alignment script to put query on top*/
		 *revASptr = -(*revASptr); 
	       }
	       if (!foundMatchForThisMatrix) {
		 bestEvalue = thisEvalue;
		 bestScore = newScore;
	       }

	       newSW = (SWResults *) MemNew(1 * sizeof(SWResults)); 
	       newSW->seq = dbSequence;
	       newSW->seqStart = dbStart;
	       newSW->seqEnd = dbStart + finalDbEnd;
	       newSW->queryStart = queryStart;
	       newSW->queryEnd = queryStart + finalQueryEnd;
	       newSW->reverseAlignScript = reverseAlignScript;
	       newSW->score = bestScore;
	       newSW->scoreThisAlign = newScore;
	       newSW->eValue = bestEvalue;
	       newSW->eValueThisAlign = thisEvalue;
	       newSW->subject_index = i;
	       newSW->Lambda = kbp-> Lambda * scalingFactor;
	       newSW->logK = kbp->logK;
	       newSW->subject_id = subject_id;
	       if (NULL == SWAligns) 
		 newSW->next = NULL;
	       else
		 newSW->next = SWAligns;
	       SWAligns = newSW;
	       numAligns++; 
	       foundMatchForThisMatrix = TRUE;
	       for(f = dbStart; f < (dbStart+ finalDbEnd); f++) {
		 if (0 == numForbidden[f] ) {
		   numForbidden[f] = 1;
		   forbiddenRanges[f][0] = queryStart;
		   forbiddenRanges[f][1] = queryStart + finalQueryEnd - 1;
		 }
		 else {
		   tempForbidden = (Int4*) MemNew(2 * (numForbidden[f] + 1) * sizeof(Int4));
		   for(tempIndex = 0; tempIndex < (2 * numForbidden[f]); tempIndex++)
		     tempForbidden[tempIndex] = forbiddenRanges[f][tempIndex];
		   tempForbidden[tempIndex] = queryStart;
		   tempForbidden[tempIndex + 1] = queryStart + finalQueryEnd;
		   numForbidden[f]++;
		   MemFree(forbiddenRanges[f]);
		   forbiddenRanges[f] = tempForbidden;
		 }
	       }
	     }
	     else {
	       MemFree(dbSequence);
	       thisEvalue = ethresh +1;
	     }
	   }
	   else {
	     MemFree(dbSequence);
	     thisEvalue = ethresh +1;
	   }
	 }
	 else {
	     MemFree(dbSequence);
	     thisEvalue = ethresh +1;
	 }
       }
       else {
	 MemFree(dbSequence);
	 thisEvalue = ethresh + 1;
       }
     } while (thisEvalue < ethresh);
     if (foundMatchForThisMatrix) {
       for (f = 0; f < maxLength; f++) {
	 numForbidden[f] = 0;
	 forbiddenRanges[f][0] = 0;
	 forbiddenRanges[f][1] = 0;
       }
     }
     startPos += dbSequenceLength;
   }
   MemFree(gap_align);
   MemFree(posMatrix);
   FileClose(matricesFile);
   FileClose(sequencesFile);
   /*close(matrixFileDesc);*/
   Nlm_MemMapFini(mmapResult);
   if (numAligns > 0) {
     pro_quicksort_hits(numAligns, &SWAligns);
     results = convertSWalignsToSeqAligns(SWAligns, querySequence, query_id);
   }
   else
     results = NULL;
   MemFree(kbp);
   MemFree(scoreArray);
   for(p = 0; p < maxLength; p++) {
     MemFree(copyMatrix[p]);
   }
   MemFree(copyMatrix);
   for (f = 0; f < maxLength; f++) 
     MemFree(forbiddenRanges[f]);
   MemFree(forbiddenRanges);
   MemFree(numForbidden);
   return(results);
}

/*print out the current version number and date*/
static Boolean PrintVersionInfo(FILE *outfp)
{
   if (NULL == outfp)
     return(FALSE);
   fprintf(outfp,"IMPALA version %s [%s]\n", PRO_VERSION, PRO_DATE);
   return(TRUE);
}

#define NUMARG 22

static Args myargs [NUMARG] = {
  { "Database", 
	"nr", NULL, NULL, FALSE, 'd', ARG_STRING, 0.0, 0, NULL},
  { "Query File", 
	"stdin", NULL, NULL, FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  { "Expectation value (E)", 
	"10.0", NULL, NULL, FALSE, 'e', ARG_FLOAT, 0.0, 0, NULL},
  { "alignment view options: 0 = pairwise, 1 = master-slave showing identities, 2 = master-slave no identities, 3 = flat master-slave, show identities, 4 = flat master-slave, no identities, 5 = master-slave no identities and blunt ends, 6 = flat master-slave, no identities and blunt ends", 
        "0", NULL, NULL, FALSE, 'm', ARG_INT, 0.0, 0, NULL},
  { "Output File for Alignment", 
	"stdout", NULL, NULL, TRUE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
  { "Filter query sequence with SEG",
        "F", NULL, NULL, FALSE, 'F', ARG_STRING, 0.0, 0, NULL},
  { "Cost to open a gap",
	"11", NULL, NULL, FALSE, 'G', ARG_INT, 0.0, 0, NULL},
  { "Cost to extend a gap",
	"1", NULL, NULL, FALSE, 'E', ARG_INT, 0.0, 0, NULL},
  { "Dropoff (X) for attempted computation of alignments from endpoints",
        "7.0", NULL, NULL, FALSE, 'y', ARG_FLOAT, 0.0, 0, NULL},
  { "Number of processors to use",
	  "1", NULL, NULL, FALSE, 'a', ARG_INT, 0.0, 0, NULL},
  { "Show GI's in deflines", 
        "F", NULL, NULL, FALSE, 'I', ARG_BOOLEAN, 0.0, 0, NULL},
  { "e-value threshold for inclusion in multipass model",
        "0.001", NULL, NULL, FALSE, 'h', ARG_FLOAT, 0.0, 0, NULL},
  { "Constant in pseudocounts for multipass version",
        "10", NULL, NULL, FALSE, 'c', ARG_INT, 0.0, 0, NULL},
  { "Maximum number of passes to use in  multipass version",
        "1", NULL, NULL, FALSE, 'j', ARG_INT, 0.0, 0, NULL},
  { "Believe the query defline", 
        "F", NULL, NULL, FALSE, 'J', ARG_BOOLEAN, 0.0, 0, NULL},
  { "SeqAlign file ('Believe the query defline' must be TRUE)",
	NULL, NULL, NULL, TRUE, 'O', ARG_FILE_OUT, 0.0, 0, NULL},
  { "Matrix", 
	"BLOSUM62", NULL, NULL, FALSE, 'M', ARG_STRING, 0.0, 0, NULL},
  { "Database name for matrix profiles", 
	"stdin", NULL, NULL, FALSE, 'P', ARG_FILE_IN, 0.0, 0, NULL},
  { "Effective length of the database (default is 0 for actual size, use -1 for size computed in makemat)",
        "0", NULL, NULL, FALSE, 'z', ARG_INT, 0.0, 0, NULL},
  { "Print help; overrides all other arguments",
        "F", NULL, NULL, FALSE, 'H', ARG_BOOLEAN, 0.0, 0, NULL},
  { "Number of one-line descriptions to print",
      "250", NULL, NULL, FALSE, 'v', ARG_INT, 0.0, 0, NULL},
  { "Number of alignments to show ",
    "250", NULL, NULL, FALSE, 'b', ARG_INT, 0.0, 0, NULL}
};  



Int2  Main(void)

{
   
   CharPtr blast_database, blast_inputfile, blast_outputfile; /*string 
           names for the database the query file and the output file*/
   FILE *infp, *outfp; /*file descriptors for  query file and output file*/
   FILE *auxiliaryfp; /*file descriptor for matrix uxiliary file*/
   FILE *sequencesfp; /*file descriptor for file containing list of sequences*/
   Boolean query_is_na; /*is the query a nucleic acid sequence?*/
   Boolean show_gi; /*should the gi number be shown in the output?*/ 
   Boolean believe_query=FALSE; /*do we believe the def line in the query file?*/
   SeqEntryPtr sep; /*sequence entry pointer representation of query*/
   BioseqPtr query_bsp; /*bioseq pointer representation of query*/
   BioseqPtr fake_bsp; /*extra pointer used when query def line is not
                         believed*/
   Int4  queryLength;  /*length of query sequence*/
   Int4  queryPos;    /*index over query sequence*/
   Uint1Ptr query =NULL;  /*query sequence read in*/        
   SeqLocPtr seg_slp; /*pointer to structure for seg filtering*/
   ObjectIdPtr obidp; /*object id associated with fake bsp*/
   BLAST_OptionsBlkPtr options; /*stores some of the options that may
                                  be specified on the command line*/
   BlastSearchBlkPtr search; /*stores parameters of the search*/
   AsnIoPtr aip=NULL; /*descriptor for writing sequence alignments
                        in ASN to a separate file*/
   BlastPruneSapStructPtr prune; /*stores possibly-reduced  of sequence
                                   alignments for output*/
   Int4 number_of_descriptions, number_of_alignments; /* maximum number 
        		 of header lines and alignments to be displayed*/

   SeqAlignPtr  head; /*holds the list of matches as alignments*/
   SeqAnnotPtr  seqannot = NULL; /*annotated version of head*/
   Uint1 featureOrder[FEATDEF_ANY]; /*dummy argument for alignment display*/
   Uint1 groupOrder[FEATDEF_ANY]; /*dummy argument for alignment display*/
   Uint4 align_options, print_options; /*store as masks options for displaying
					 alignments and headers */
   TxDfDbInfoPtr dbinfo=NULL; /*placeholder for information about the database*/
   ValNodePtr  mask_loc; /*placeholder for masking information*/
   ValNodePtr other_returns; /*holds list of miscellaneous return values
                               to be printed*/
   ValNodePtr vnp; /*loop index to iterate over other_returns*/
   SeqLocPtr   private_slp=NULL; /*used to get a sequence id*/
   SeqLocPtr   filter_slp=NULL; /*used for filtering*/
   SeqPortPtr  spp=NULL; /*used for filtering*/
   SeqIdPtr local_sequence_id; /*represents sequence id used to display alignments*/
   Boolean mask_at_hash=FALSE; /*used to decide where filtering happens*/
   proDemographicsItems proDemographics; /*computes stats for each run*/
   Int4 maxLength; /*maximum length of a sequnce*/
   Int4 totalLength; /*maximum length of a sequnce*/
   Char auxiliaryFileName[MAX_NAME_LENGTH]; /*name of auxiliary file*/
   Char mmapFileName[MAX_NAME_LENGTH]; /*name of memory mapped matrices*/
   Char seqFileName[MAX_NAME_LENGTH]; /*name of file with sequence names*/
   Char matrixFileName[MAX_NAME_LENGTH]; /*name of file with matrix names*/
   Int4 numProfiles; /*number of profiles in the profiles database*/
   Int4 read_gap_open, read_gap_extend; /*gap costs to read in*/
   Int4 readMaxLength; /*maximum profile length to read in*/
   Int4 readTotalLength; /*total profile database size to read in*/
   Int4 readSequenceDbLength; /*length of underlying sequence db to read in*/
   Nlm_FloatHi readScaling; /*scaling factor to read in*/
   Char *matrixName; /*underlying score matrix used for profile library*/
   Nlm_FloatHi targetUngappedLambda; /*ungapped Lambda for matrixName*/
   Char *directoryPrefix; /*directory where profile library is kept, used
                            to reach other directories indirectly*/
   BLAST_MatrixPtr matrix;
   Int4Ptr PNTR txmatrix;
        
   if (! GetArgs ("impala", NUMARG, myargs))
     {
        return (1);
     }

   if (! SeqEntryLoad())
     return 1;

   UseLocalAsnloadDataAndErrMsg();

   if ((Boolean) myargs[19].intvalue) {
     IMPALAPrintHelp(FALSE, 90, "impala", stdout);
     return(1);
   }
   blast_database = myargs [ARG_DB].strvalue;
   blast_inputfile = myargs [ARG_QUERY_FILE].strvalue;
   blast_outputfile = myargs [ARG_OUTPUT_FILE].strvalue;
   if ((infp = FileOpen(blast_inputfile, "r")) == NULL)
     {
       ErrPostEx(SEV_FATAL, 1, 0, "profiles: Unable to open input file %s\n", blast_inputfile);
       return (1);
     }

   outfp = NULL;
   if (blast_outputfile != NULL)
     {
       if ((outfp = FileOpen(blast_outputfile, "w")) == NULL)
	 {
	    ErrPostEx(SEV_FATAL, 1, 0, "profiles: Unable to open output file %s\n", blast_outputfile);
	    return (1);
	 }
     }

   directoryPrefix = (Char *) MemNew(MAX_NAME_LENGTH *sizeof(char));
   strcpy(directoryPrefix,myargs[ARG_MATRICES_DB].strvalue);
   impalaMakeFileNames(myargs[ARG_MATRICES_DB].strvalue,auxiliaryFileName,
		 mmapFileName,seqFileName,matrixFileName, NULL, 
		 directoryPrefix);

   if ((auxiliaryfp = FileOpen(auxiliaryFileName, "r")) == NULL)
     {
       ErrPostEx(SEV_FATAL, 1, 0, "profiles: Unable to open auxiliary file %s\n", auxiliaryFileName);
       return (1);
     }

   if ((sequencesfp = FileOpen(seqFileName, "r")) == NULL)  {
     ErrPostEx(SEV_FATAL, 1, 0, "profiles: Unable to open file with all sequences %s\n", seqFileName);
	return (1);
   }

   believe_query = FALSE;
   if (myargs[ARG_BELIEVE_DEF].intvalue != 0)
     believe_query = TRUE;
   query_is_na = FALSE;
   if (myargs[ARG_SEQALIGN_FILE].strvalue != NULL)
     {
       if (believe_query == FALSE)
	 {
	   ErrPostEx(SEV_FATAL, 1, 0, "-J option must be TRUE to use this option");
	   return (1);
	 }
       if ((aip = AsnIoOpen (myargs[ARG_SEQALIGN_FILE].strvalue,"w")) == NULL)
	 {
	   ErrPostEx(SEV_FATAL, 1, 0, "profiles: Unable to open output file %s\n", myargs[ARG_SEQALIGN_FILE].strvalue);
	   return 1;
	 }
     }

   sep = FastaToSeqEntryEx(infp, query_is_na, NULL, believe_query);
   if (sep != NULL)
     {
	query_bsp = NULL;
	SeqEntryExplore(sep, &query_bsp, FindProt);
	if (query_bsp == NULL)
          {
	    ErrPostEx(SEV_FATAL, 1, 0, "Unable to obtain bioseq\n");
	    return 2;
	  }
		
	options = BLASTOptionNew("blastp", TRUE);
	options->isPatternSearch = FALSE;

	/* Set default gap params for matrix. */
	BLASTOptionSetGapParams(options, myargs[ARG_MATRIX].strvalue, 0, 0);

        options->expect_value  = (Nlm_FloatHi) myargs [ARG_E_VALUE].floatvalue;
        options->hitlist_size = PRO_MAX_HIT_LIST;
	options->gap_open = myargs[ARG_GAP_OPEN].intvalue;
	options->gap_extend = myargs[ARG_GAP_EXTEND].intvalue;
        options->gap_x_dropoff = myargs[ARG_X_DROP].floatvalue;
	
	if (StringICmp(myargs[ARG_FILTER].strvalue, "T") == 0)
	  {
	    options->filter_string = StringSave("S");
	  }
	else
	  {
	    options->filter_string = StringSave(myargs[ARG_FILTER].strvalue);
	  }
	options->number_of_cpus = (Int2) myargs[ARG_PROCESSORS].intvalue;
	show_gi = (Boolean) myargs[ARG_SHOW_GI].intvalue;
        options->ethresh = (Nlm_FloatHi) myargs[ARG_E_MULTIPASS].floatvalue;
        options->pseudoCountConst = myargs[ARG_PSEUDO].intvalue;
        options->maxNumPasses = myargs[ARG_NUM_PASS].intvalue;
        /*zero out e-value threshold if it will not be used*/
        if (options->maxNumPasses == 1)
          options->ethresh = 0.0;
	if (myargs[ARG_DB_LENGTH].intvalue)
          options->db_length = myargs[ARG_DB_LENGTH].intvalue;

	options->perform_culling = FALSE;
	options = BLASTOptionValidate(options, "blastp");

	if (options == NULL)
          {
	    return 1;
	  }

	query = BlastGetSequenceFromBioseq(query_bsp, &queryLength);
	seg_slp = BlastBioseqFilter(query_bsp, options->filter_string);
	for (queryPos= 0; queryPos < queryLength; queryPos++)
	  query[queryPos] = ResToInt((Char) query[queryPos]);  
	if (seg_slp) {
	  BlastMaskTheResidues(query,queryLength,21,seg_slp,FALSE,0);
	}


	if (believe_query)
	  {
	    fake_bsp = query_bsp;
	  }
	else
	  {
	    fake_bsp = BioseqNew();
	    fake_bsp->descr = query_bsp->descr;
	    fake_bsp->repr = query_bsp->repr;
	    fake_bsp->mol = query_bsp->mol;
	    fake_bsp->length = query_bsp->length;
	    fake_bsp->seq_data_type = query_bsp->seq_data_type;
	    fake_bsp->seq_data = query_bsp->seq_data;
	
            obidp = ObjectIdNew();
            obidp->str = StringSave("QUERY");
            ValNodeAddPointer(&(fake_bsp->id), SEQID_LOCAL, obidp);
			
	    /* FASTA defline not parsed, ignore the "lcl|tempseq" ID. */
	    query_bsp->id = SeqIdSetFree(query_bsp->id);
	  }
 
          options->isPatternSearch = TRUE;
          search = (BlastSearchBlkPtr) MemNew(1 * sizeof(BlastSearchBlk));
          options->isPatternSearch=  FALSE;

          if (search == NULL)
	    return 1;

	  search->positionBased = TRUE;

	  global_fp = outfp;

	  init_buff_ex(90);
	  PrintVersionInfo(outfp);
          fprintf(outfp, "\n");
	  IMPALAPrintReference(FALSE, 90, outfp);
	  fprintf(outfp, "\n");
	  AcknowledgeBlastQuery(query_bsp, 70, outfp, believe_query, FALSE);
	  free_buff();

	  MemSet((Pointer)(groupOrder), 0, (size_t)(FEATDEF_ANY* sizeof(Uint1)));
	  MemSet((Pointer)(featureOrder), 0, (size_t)(FEATDEF_ANY* sizeof(Uint1)));

	  search->error_return = NULL;

          numProfiles = countProfiles(sequencesfp);
          matrixName = findProfileParams(auxiliaryfp,  numProfiles,
              &read_gap_open, &read_gap_extend,  &readMaxLength,
              &readTotalLength, &readSequenceDbLength, &readScaling);


          search->sbp = BLAST_ScoreBlkNew(Seq_code_ncbistdaa, 1);
          search->sbp->read_in_matrix = TRUE;
          BlastScoreBlkMatFill(search->sbp, myargs[ARG_MATRIX].strvalue);

	  search->gap_align = GapAlignBlkNew(1,1);
	  search->gap_align->gap_open = read_gap_open;
	  search->gap_align->gap_extend = read_gap_extend;
	  search->gap_align->matrix = search->sbp->matrix;
          private_slp = SeqLocIntNew(0, fake_bsp->length-1 , Seq_strand_plus, SeqIdFindBest(fake_bsp->id, SEQID_GI));
          local_sequence_id = SeqIdFindBest(SeqLocId(private_slp), SEQID_GI);
          search->query_id = SeqIdDup(local_sequence_id);
          /*set up possible filtering of query for display of SeqAnnot*/
	  filter_slp = BlastSeqLocFilterEx(private_slp, options->filter_string, &mask_at_hash);
          spp = SeqPortNewByLoc(private_slp, Seq_code_ncbistdaa);
	  if (filter_slp && !mask_at_hash)
	    ValNodeAddPointer(&(search->mask), SEQLOC_MASKING_NOTSET, filter_slp);

          initDemographics(&proDemographics, queryLength,
			   matrixName, 
			   options->gap_x_dropoff_final,
			   myargs[ARG_DB_LENGTH].intvalue);
          targetUngappedLambda = IMPALAfindUngappedLambda(matrixName);
	  if (0.0 == targetUngappedLambda) {
	    ErrPostEx(SEV_FATAL, 1, 0, "Cannot identify matrix %s",matrixName);
	    return (1);
          }

#ifdef OS_UNIX
	  fprintf(global_fp, "%s", "Searching");
	  fflush(global_fp);
#endif


	  head = findMatchingProfiles(auxiliaryfp, mmapFileName, 
				      matrixFileName, sequencesfp, numProfiles,
                                      readMaxLength, readTotalLength, 
                                      targetUngappedLambda, readScaling,
				      query,  queryLength,
				      options->gap_open, options->gap_extend, 
				      options->gap_x_dropoff_final, 
				      options->expect_value,
				      5 * options->expect_value,
				      search->query_id, &proDemographics, 
                                      directoryPrefix);
#ifdef OS_UNIX
          fprintf(global_fp, "%s", "done");
#endif

	  ReadDBBioseqFetchEnable ("blastpgp", blast_database, FALSE, TRUE);
                
          print_options = 0;
          align_options = 0;
          align_options += TXALIGN_COMPRESS;
          align_options += TXALIGN_END_NUM;
          if (show_gi) {
	    align_options += TXALIGN_SHOW_GI;
	    print_options += TXALIGN_SHOW_GI;
	  } 

          if (myargs[ARG_ALIGN_VIEW].intvalue != 0)
            {
	      align_options += TXALIGN_MASTER;
	      if (myargs[ARG_ALIGN_VIEW].intvalue == 1 || 
                  myargs[ARG_ALIGN_VIEW].intvalue == 3)
		align_options += TXALIGN_MISMATCH;
		if (myargs[ARG_ALIGN_VIEW].intvalue == 3 || 
		    myargs[ARG_ALIGN_VIEW].intvalue == 4 || 
		    myargs[ARG_ALIGN_VIEW].intvalue == 6)
		  align_options += TXALIGN_FLAT_INS;
		if (myargs[ARG_ALIGN_VIEW].intvalue == 5 || 
		    myargs[ARG_ALIGN_VIEW].intvalue == 6)
		  align_options += TXALIGN_BLUNT_END;
	    }
	  else
	    {
	      align_options += TXALIGN_MATRIX_VAL;
	      align_options += TXALIGN_SHOW_QS;
	    }

	  /*other_returns = BlastOtherReturnsPrepare(search);*/
	  BlastErrorPrint(search->error_return);
          number_of_descriptions = myargs[ARG_NUM_DEFLINES].intvalue;
          number_of_alignments = myargs[ARG_NUM_ALIGNS].intvalue;

          matrix = NULL;
          txmatrix = NULL;
          if (search->sbp->matrix) {
             matrix = BLAST_MatrixFill(search->sbp, FALSE);
             txmatrix = BlastMatrixToTxMatrix(matrix);
          }

	  if (head)
	    {
	      if (seqannot)
		seqannot = SeqAnnotFree(seqannot);
	      seqannot = SeqAnnotNew();
	      seqannot->type = 2;
	      AddAlignInfoToSeqAnnot(seqannot, 2);
	      seqannot->data = head;
	      if (outfp)
		{
		  fprintf(outfp, "\nResults from profile search\n");
		  prune = BlastPruneHitsFromSeqAlign(head, number_of_descriptions, NULL);
		  ObjMgrSetHold();
		  init_buff_ex(85);
		  PrintDefLinesFromSeqAlign(prune->sap, 80, outfp, print_options, FIRST_PASS, NULL);
		  free_buff();
		  prune = BlastPruneHitsFromSeqAlign(head, number_of_alignments, prune);
		  seqannot->data = prune->sap;
		  /*need to fix penultimate argument*/
		  if (myargs[ARG_ALIGN_VIEW].intvalue != 0)
		    ShowTextAlignFromAnnot2(seqannot, 60, outfp, featureOrder, groupOrder, align_options, txmatrix, search->mask, NULL, NULL, NULL);
		  else
		    ShowTextAlignFromAnnot2(seqannot, 60, outfp, featureOrder, groupOrder, align_options, txmatrix, search->mask, FormatScoreFunc, NULL, NULL);
		}
	      seqannot->data = head;
	      prune = BlastPruneSapStructDestruct(prune);
	      ObjMgrClearHold();
	      ObjMgrFreeCache(0);
	    }
	  else
	    {
	      fprintf(outfp, "\n\n ***** No hits found ******\n\n");
	    }
	  if (aip && seqannot)
	    {
	      SeqAnnotAsnWrite((SeqAnnotPtr) seqannot, aip, NULL);
	      AsnIoReset(aip);
	      aip = AsnIoClose(aip);
	    }
     }
   printDemographics(proDemographics,outfp, options->expect_value);

   free_buff();

   ReadDBBioseqFetchDisable();

   matrix = BLAST_MatrixDestruct(matrix);
   if (txmatrix)
      txmatrix = TxMatrixDestruct(txmatrix);
   
   if (NULL != query)
     MemFree(query);
   options = BLASTOptionDelete(options);
   /*sep = SeqEntryFree(sep);*/
   search = BlastSearchBlkDestruct(search);
   if (believe_query == FALSE)
     {
       fake_bsp->descr = NULL;
       fake_bsp->length = 0;
       fake_bsp->seq_data = NULL;
     }
   FileClose(infp);
   return 0;
}



