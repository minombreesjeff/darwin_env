/* $Id: cddposutil.c,v 1.15 2001/12/31 13:47:06 bauer Exp $
*===========================================================================
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
* File Name:  cddposutil.c
*
* Author:  Aron Marchler-Bauer
*
* Initial Version Creation Date: 12/21/1999
*
* $Revision: 1.15 $
*
* File Description: CDD utilities involving position-specific scoring 
*                   matrices (PSSMs)
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cddposutil.c,v $
* Revision 1.15  2001/12/31 13:47:06  bauer
* made block_width a local variable in CddSetUpSearchInternalByLoc to deal with changes in blast data structures
*
* Revision 1.14  2001/02/06 20:54:43  hurwitz
* added a couple asserts
*
* Revision 1.13  2001/01/19 23:34:55  bauer
* fixed memory leaks
*
* Revision 1.12  2001/01/17 20:21:54  bauer
* merged in changes for PSSM calculation
*
* Revision 1.11  2001/01/03 11:11:49  bauer
* renamed posCancel to CddposCancel
*
* Revision 1.10  2000/12/29 00:52:21  hurwitz
* cleaning memory leaks
*
* Revision 1.9  2000/10/26 01:55:54  bauer
* renamed BLASTSetUpSearchInternalByLoc to CddSetUpSearchInternalByLoc
*
* Revision 1.8  2000/09/08 21:43:51  hurwitz
* adding PSSM calculation to DDE
*
* Revision 1.7  2000/08/17 19:00:24  hurwitz
* fixed some codewarrior compiler warnings
*
* Revision 1.6  2000/08/10 22:31:19  bauer
* CddposTakeCheckpoint now declared to return Boolean
*
* Revision 1.5  2000/08/09 21:29:08  hurwitz
* adding cddutil.c to VC++ build
*
* Revision 1.4  2000/08/08 21:18:58  bauer
* changed POSIT_SCALE_FACTOR from 1000 to 200
*
* Revision 1.3  2000/07/28 18:01:59  bauer
* fixed typecasts
*
* Revision 1.2  2000/07/19 19:36:22  bauer
* added modification logging
*
*
* ==========================================================================
*/


#include <ncbi.h>
#include <blastpri.h>
#include <objcode.h>
#include <objseq.h>
#include <objsset.h>
#include <sequtil.h>
#include <posit.h>
#include <txalign.h>
#include "cddposutil.h"
#include <blast.h>
#include <objloc.h>
#include <accid1.h>
#include <cddutil.h>
#include <mblast.h>
#include <profiles.h>
/* #include <blastdef.h> */

#define GAP_CHAR 0
#define GAP_HERE -1
#define POS_RESTING 0
#define POS_COUNTING 1
#define IDENTITY_RATIO 0.98
#define posEpsilon 0.0001
#define posEpsilon2 0.0000001
#define POSIT_SCALE_FACTOR2 200
#define PROTEIN_ALPHABET 26
#define EFFECTIVE_ALPHABET 20

/* function definitions */
static Uint1Ptr LIBCALL CddGetSequenceWithDenseDiag(DenseDiagPtr ddp, Boolean query,
                                                    Int4Ptr start, Int4Ptr stop);
static Uint1Ptr LIBCALL CddGetSequenceWithDenseSeg(DenseSegPtr dsp, Boolean query,
                                                   Int4Ptr start, Int4Ptr length);
static void LIBCALL CddposCancel(posSearchItems *posSearch, compactSearchItems * compactSearch,
                              Int4 first, Int4 second, Int4 matchStart, Int4 intervalLength);
static Nlm_FloatHi ** LIBCALL CddallocatePosFreqs(Int4 length, Int4 alphabetSize);
static Nlm_FloatHi LIBCALL countsFunction(Nlm_FloatHi Sigma, Int4 intervalLength);
static Nlm_FloatHi LIBCALL posit_rounddown(Nlm_FloatHi value);
static Int2 LIBCALL CddSetUpSearchInternalByLoc
                       (BlastSearchBlkPtr search, SeqLocPtr query_slp, BioseqPtr query_bsp,
                        CharPtr prog_name, Int4 qlen, BLAST_OptionsBlkPtr options,
                        int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)));
static void LIBCALL putCkptNumber(void * numberPtr, Int4 numberSize, FILE * ckptFile);
static void LIBCALL putFreqVector(Nlm_FloatHi * theVector, Int4 length, FILE * ckptFile);
static void LIBCALL putCkptFreqMatrix (Nlm_FloatHi **theMatrix, Int4 length, Int4 width, FILE * ckptFile);

/*---------------------------------------------------------------------------*/
/*Allocate memory for  data structures inside posSearch used in              */
/* position-specific caculations                                             */
/* posSearch -- to be filled in                                              */
/* alphabetSize -- number of distinct characters used in the sequences       */
/* querySize -- number of characters in the query sequence                   */
/* numSequences -- number of matching sequences potentially in the model     */
/*---------------------------------------------------------------------------*/

void LIBCALL CddposAllocateMemory(posSearchItems * posSearch, Int4 alphabetSize, Int4 querySize, Int4 numSequences)
{
  Int4 i, j;                                                   /*loop indices*/

  posSearch->QuerySize = querySize;
  posSearch->NumSequences = numSequences;

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
  posSearch->posFreqs = (Nlm_FloatHi **) MemNew((querySize + 1) * sizeof(Nlm_FloatHi *));
  if (NULL == posSearch->posMatrix) exit(EXIT_FAILURE);
  if (NULL == posSearch->posPrivateMatrix) exit(EXIT_FAILURE);
  if (NULL == posSearch->posFreqs) exit(EXIT_FAILURE);
  for(i = 0; i <= querySize; i++) {
    posSearch->posMatrix[i] = (BLAST_Score *) MemNew(alphabetSize * sizeof(BLAST_Score));
    posSearch->posPrivateMatrix[i] = (BLAST_Score *) MemNew(alphabetSize * sizeof(BLAST_Score));
    posSearch->posFreqs[i] = (Nlm_FloatHi *) MemNew(alphabetSize * sizeof(Nlm_FloatHi));
    if (NULL == posSearch->posMatrix[i]) exit(EXIT_FAILURE);
    if (NULL == posSearch->posPrivateMatrix[i]) exit(EXIT_FAILURE);
    if (NULL == posSearch->posFreqs[i]) exit(EXIT_FAILURE);
    for(j = 0; j < alphabetSize; j++) {
      posSearch->posMatrix[i][j] = 0;
      posSearch->posFreqs[i][j] = 0;
    }
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
}


void LIBCALL CddposFreeMemory2(compactSearchItems * compactSearch)
{
  MemFree(compactSearch->standardProb);
}

void LIBCALL CddposFreeMemory(posSearchItems * posSearch) {
/*---------------------------------------------------------------------------*/
/*  free memory that was allocated in posAllocateMemory.                     */
/*---------------------------------------------------------------------------*/
  Int4  i;

  MemFree(posSearch->posCount);
  for (i=0; i<=posSearch->QuerySize; i++) {
    MemFree(posSearch->posMatchWeights[i]);
    MemFree(posSearch->posFreqs[i]);
    MemFree(posSearch->posMatrix[i]);
    MemFree(posSearch->posPrivateMatrix[i]);
    MemFree(posSearch->posC[i]);
  }
  MemFree(posSearch->posC);
  MemFree(posSearch->posMatchWeights);
  MemFree(posSearch->posMatrix);
  MemFree(posSearch->posPrivateMatrix);
  MemFree(posSearch->posFreqs);

  for (i=0; i<=posSearch->posDescMatrixLength; i++) {
    MemFree(posSearch->posDescMatrix[i]);
  }
  MemFree(posSearch->posDescMatrix);

  MemFree(posSearch->posSigma);
  MemFree(posSearch->posIntervalSizes);
  MemFree(posSearch->posExtents);
  MemFree(posSearch->posA);
  MemFree(posSearch->posRowSigma);
  MemFree(posSearch->posInformation);
  MemFree(posSearch->posUseSequences);
}


/*---------------------------------------------------------------------------*/
/* Copy a few fields from the lasrge record search into the small record     */
/* compactSearch, so that a small amount of information                      */
/* is passed into posit.c                                                    */
/*---------------------------------------------------------------------------*/
void LIBCALL CddcopySearchItems(compactSearchItems *compactSearch,
                                BlastSearchBlkPtr   search)
{
   BLAST_ResFreqPtr stdrfp; /* gets standard frequencies in prob field */
   Int4 a; /*index over characters*/

   compactSearch->query = search->context[0].query->sequence;
   compactSearch->qlength = search->context[0].query->length;
   compactSearch->gapped_calculation = search->pbp->gapped_calculation;
   compactSearch->alphabetSize = search->sbp->alphabet_size;
   compactSearch->pseudoCountConst = search->pbp->pseudoCountConst;
   compactSearch->ethresh = search->pbp->ethresh;
   compactSearch->matrix = search->sbp->matrix;
   compactSearch->kbp_psi = search->sbp->kbp_psi;
   compactSearch->kbp_gap_psi = search->sbp->kbp_gap_psi;
   compactSearch->kbp_std = search->sbp->kbp_std;
   compactSearch->kbp_gap_std = search->sbp->kbp_gap_std;
   compactSearch->use_best_align = search->pbp->use_best_align;

/*---------------------------------------------------------------------------*/
/* these are removed, not initialized in CddCposComputation                  */
/*---------------------------------------------------------------------------*/
/*   compactSearch->lambda =  search->sbp->kbp[0]->Lambda; */
/*   compactSearch->lambda_ideal = search->sbp->kbp_ideal->Lambda; */
/*   compactSearch->K_ideal = search->sbp->kbp_ideal->K; */

   stdrfp = BlastResFreqNew(search->sbp);
   BlastResFreqStdComp(search->sbp,stdrfp); 
   compactSearch->standardProb = MemNew(compactSearch->alphabetSize * sizeof(Nlm_FloatHi));
   if (NULL == compactSearch->standardProb)
     exit(EXIT_FAILURE);
   for(a = 0; a < compactSearch->alphabetSize; a++)
     compactSearch->standardProb[a] = stdrfp->prob[a];
   stdrfp = BlastResFreqDestruct(stdrfp);
}

/*---------------------------------------------------------------------------*/
/* - find all sequences, basically, for Cdd-specific purposes                */
/*---------------------------------------------------------------------------*/
void LIBCALL CddfindThreshSequences(posSearchItems *posSearch, SeqAlignPtr listOfSeqAligns, Int4 numalign, Int4 numseq)
{
  SeqAlignPtr curSeqAlign;                /* pointers into list of seqAligns */
  DenseSegPtr curSegs;                          /* Item in list of seqAligns */
  SeqIdPtr    thisId;         /* Ids of target sequences in current SeqAlign */  
  Int4        ordinalNumber;            /* index of sequence within database */
    
/* Allocate boolean array to store values */
  posSearch->posResultSequences = (Int4 *) MemNew(numalign * sizeof(Int4));
  posSearch->posResultsCounter = 0;

  curSeqAlign = listOfSeqAligns;
  ordinalNumber = -1;
  while (curSeqAlign) {
    curSegs = (DenseSegPtr) curSeqAlign->segs;
    thisId = curSegs->ids->next;
/* Get ordinal ids of sequences in result */
    ordinalNumber++;
    posSearch->posResultSequences[posSearch->posResultsCounter]=ordinalNumber;
    posSearch->posResultsCounter++;
    curSeqAlign = curSeqAlign->next;
  }
}

/*---------------------------------------------------------------------------*/
/* - find all sequences, basically, for Cdd-specific purposes                */
/*---------------------------------------------------------------------------*/
void LIBCALL CddfindDenseDiagThreshSequences(posSearchItems *posSearch, SeqAlignPtr listOfSeqAligns, Int4 numalign, Int4 numseq)
{
  SeqAlignPtr  curSeqAlign;               /* pointers into list of seqAligns */
  DenseDiagPtr curSegs;                         /* Item in list of seqAligns */
  SeqIdPtr     thisId;        /* Ids of target sequences in current SeqAlign */  
  Int4         ordinalNumber;           /* index of sequence within database */
    
/* Allocate boolean array to store values */
  posSearch->posResultSequences = (Int4 *) MemNew(numalign * sizeof(Int4));
  posSearch->posResultsCounter = 0;

  curSeqAlign = listOfSeqAligns;
  ordinalNumber = -1;
  while (curSeqAlign) {
    curSegs = (DenseDiagPtr) curSeqAlign->segs;
    thisId = curSegs->id->next;
/* Get ordinal ids of sequences in result */
    ordinalNumber++;
    posSearch->posResultSequences[posSearch->posResultsCounter]=ordinalNumber;
    posSearch->posResultsCounter++;
    curSeqAlign = curSeqAlign->next;
  }
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddposInitializeInformation(posSearchItems *posSearch, BLAST_ScoreBlkPtr sbp, compactSearchItems *compactSearch)
{
  Uint1Ptr query;
  Int4 querySize;
  Int4 c, a, alphabetSize;
  BLAST_ResFreqPtr stdrfp; /*standard frequencies*/
  Nlm_FloatHi lambda;
  Nlm_FloatHi term1, term2, term3, term4;
  Nlm_FloatHi infoSum;
 
  querySize = compactSearch->qlength;
  query = compactSearch->query;
  posSearch->posInformation = (Nlm_FloatHi *) MemNew(querySize * sizeof(Nlm_FloatHi));
  if (NULL == posSearch->posInformation) exit(EXIT_FAILURE);
  for(c = 0; c < querySize; c++)
    posSearch->posInformation[c] = 0.0;
  alphabetSize = compactSearch->alphabetSize;
  /*Compute standard frequencies as in BlastScoreBlkFill in blastkar.c*/
  stdrfp = BlastResFreqNew(sbp);
  BlastResFreqStdComp(sbp,stdrfp); 
  lambda = sbp->kbp[0]->Lambda;
  for(c = 0; c < querySize; c++) {
    infoSum = 0;
    for(a = 0; a < alphabetSize; a++)
      if (stdrfp->prob[a] > posEpsilon) {
        term1 = sbp->matrix[query[c]][a];
	      term2 = term1 * lambda;
	      term3 = exp(term2);
	      term4 = stdrfp->prob[a] * term3;
	      infoSum += term4 * log(term4/stdrfp->prob[a])/NCBIMATH_LN2;
      }
    posSearch->posInformation[c] = infoSum;
  } 
}


/*---------------------------------------------------------------------------*/
/* have added sequence retrieval via ID1 in case BioseqLockById fails        */
/*---------------------------------------------------------------------------*/
static Uint1Ptr LIBCALL CddGetSequenceWithDenseDiag(DenseDiagPtr ddp, Boolean query,
                                                    Int4Ptr start, Int4Ptr stop)
{
  BioseqPtr    bsp;
  Int4         index, offset;
  Int4         length;
  SeqIdPtr     id;
  SeqPortPtr   spp;
  Uint1Ptr     buffer;
  DenseDiagPtr ddpThis;
  SeqEntryPtr  sep;
  Int4         uid;
  Int2         retcode = 1;

  ddpThis = ddp;

  if (ddpThis == NULL) return NULL;
  if (query == TRUE) {
    id = ddpThis->id;
  } else {
    offset = 1;
    id = ddpThis->id->next;
  }

  *start = ddpThis->starts[offset];
  *stop = 0;
  while (ddpThis) {
    *stop = ddpThis->starts[offset] + ddpThis->len - 1;
    ddpThis = ddpThis->next;
  }
	bsp = BioseqLockById(id);
  if (bsp) {
  	spp = SeqPortNew(bsp, *start, *stop, Seq_strand_unknown, Seq_code_ncbistdaa);
    length = *stop - *start + 1;
	  buffer = MemNew((length)*sizeof(Uint1));
	  for (index=0; index<length; index++) buffer[index] = SeqPortGetResidue(spp);
	  spp = SeqPortFree(spp);
	  BioseqUnlock(bsp);
	  return buffer;
  } else {
    uid = ID1FindSeqId(id);
    sep = (SeqEntryPtr) ID1SeqEntryGet(uid,retcode);
    bsp = BioseqFindInSeqEntry(id,sep);
  	spp = SeqPortNew(bsp, *start, *stop, Seq_strand_unknown, Seq_code_ncbistdaa);
    length = *stop - *start + 1;
	  buffer = MemNew((length)*sizeof(Uint1));
	  for (index=0; index<length; index++) buffer[index] = SeqPortGetResidue(spp);
	  spp = SeqPortFree(spp);
	  return buffer;
  } 
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Uint1Ptr LIBCALL CddGetSequenceWithDenseSeg(DenseSegPtr dsp, Boolean query,
                                                   Int4Ptr start, Int4Ptr length)
{
  BioseqPtr  bsp;
  Int4       index, offset;
  SeqIdPtr   id;
  SeqPortPtr spp;
  Uint1Ptr   buffer;

  if (dsp == NULL) return NULL;

  if (query == TRUE) {
    offset = 0;
    id = dsp->ids;
  } else {
    offset = 1;
    id = dsp->ids->next;
  }
  *start = dsp->starts[offset];
  *length = 0;
  for (index=0; index<dsp->numseg; index++) {
    if (dsp->starts[offset+2*index] != -1) *length += dsp->lens[index];
  }
  bsp = BioseqLockById(id);
  spp = SeqPortNew(bsp, *start, (*start)+(*length)-1, Seq_strand_unknown, Seq_code_ncbistdaa);
  buffer = MemNew((*length)*sizeof(Uint1));
  for (index=0; index<*length; index++) buffer[index] = SeqPortGetResidue(spp);
  spp = SeqPortFree(spp);
  BioseqUnlock(bsp);
  return buffer;
}

/*---------------------------------------------------------------------------*/
/* Eliminate the matches from sequence second starting at position           */
/* matchStart and extending for intervalLength characters                    */
/*---------------------------------------------------------------------------*/
static void LIBCALL CddposCancel(posSearchItems *posSearch, compactSearchItems * compactSearch,
                              Int4 first, Int4 second, Int4 matchStart, Int4 intervalLength)
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



/*---------------------------------------------------------------------------*/
/*Eliminate sequences that are identical to the query and partial alignments
  that are identical in two matching sequences*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddposPurgeMatches(posSearchItems *posSearch, compactSearchItems * compactSearch)
{
  Int4    i, j;                                        /*index over sequences*/
  Boolean matchesQuery;      /*Is a matching sequence identical to the query?*/
  Int4    c;                   /*index over demographics of matching sequence*/
  Int4    state;                              /*state of checking for a match*/
  Int4    intervalLength, matchStart; /*Length and start of a matching region*/
  Int4    matchNumber;                        /*number of characters matching*/

  posSearch->posUseSequences =  (Boolean *) MemNew((posSearch->posNumSequences + 1) * sizeof(Boolean));
   if (NULL == posSearch->posUseSequences)  exit(EXIT_FAILURE);
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
    if (matchesQuery) posSearch->posUseSequences[i] = FALSE;
  }
  for(j = 1; j <= posSearch->posNumSequences; j++) {
    if (!posSearch->posUseSequences[j]) continue;
    state = POS_COUNTING;
    c = 0;
    matchStart = 0;
    intervalLength = 0;
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
	        } else  intervalLength++;
	        if (posSearch->posDescMatrix[j][c].used &&
	            (posSearch->posDescMatrix[0][c].letter == posSearch->posDescMatrix[j][c].letter))
	          matchNumber++;
	      }
      } else {
	      if (state == POS_COUNTING) {
	        if ((intervalLength > 0) && (matchNumber == intervalLength))
	          CddposCancel(posSearch,compactSearch,0,j,matchStart,intervalLength);
	        state = POS_RESTING;
	      }
      }
      c++;
    }
    if (state == POS_COUNTING) /*at end of sequence i*/
      if ((intervalLength > 0) && (matchNumber == intervalLength))
	CddposCancel(posSearch,compactSearch,0,j,matchStart,intervalLength);
  }
  for (i = 1; i < posSearch->posNumSequences; i++) {
    if (!posSearch->posUseSequences[i])
      continue;
    for(j = i+1; j <= posSearch->posNumSequences; j++) {
      if (!posSearch->posUseSequences[j])
	continue;
      state = POS_COUNTING;
      c = 0;
      matchStart = 0;
      intervalLength = 0;
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
	}
	else {
	  if (state == POS_COUNTING) {
	    if ((intervalLength > 0) && ((((Nlm_FloatHi) matchNumber)/intervalLength) >= IDENTITY_RATIO))
	      CddposCancel(posSearch,compactSearch,i,j,matchStart,intervalLength);
	    state = POS_RESTING;
	  }
	}
	c++;
      }
      if (state == POS_COUNTING) /*at end of sequence i*/
	if ((intervalLength > 0) && ((((Nlm_FloatHi) matchNumber)/intervalLength) >= IDENTITY_RATIO))
	  CddposCancel(posSearch,compactSearch,i,j,matchStart,intervalLength);
    }
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddposComputeExtents(posSearchItems *posSearch, compactSearchItems * compactSearch)
{
   Int4     seqIndex;                                     /*index of sequence*/
   Int4     length;                                         /*length of query*/
   Int4     qplace, qplace2;                                 /*place in query*/
   Int4     numseq;                     /*number of sequences including query*/
   Uint1Ptr q;                                         /*pointers into query */

   length = compactSearch->qlength;
   numseq = posSearch->posNumSequences;
   q = compactSearch->query;
   for(seqIndex = 0; seqIndex < numseq; seqIndex++) {
     if (!posSearch->posUseSequences[seqIndex+1])
       continue;
     if ((posSearch->posDescMatrix[seqIndex+1][0].used)
	 && (posSearch->posDescMatrix[seqIndex+1][length-1].letter != GAP_CHAR))
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
	 posSearch->posExtents[qplace].leftExtent = MAX(posSearch->posExtents[qplace].leftExtent,
							posSearch->posDescMatrix[seqIndex+1][qplace].leftExtent);
	 posSearch->posExtents[qplace].rightExtent = MIN(posSearch->posExtents[qplace].rightExtent,
							 posSearch->posDescMatrix[seqIndex+1][qplace].rightExtent);
	 
       }

     for(qplace = 0; qplace < length; qplace++) 
     /*used to check qplace for GAP_CHAR here*/
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
	 if(posSearch->posExtents[qplace2].rightExtent >= qplace)
	   posSearch->posIntervalSizes[qplace2]--;
       }
     }
   }
 }
 
/*---------------------------------------------------------------------------*/
/*Compute weight of each sequence and letter in each position*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddposComputeSequenceWeights(posSearchItems *posSearch,
                                          compactSearchItems * compactSearch)
{
   Int4        length;                                      /*length of query*/
   Int4        numseq, seqIndex;          /*number of matches, index for them*/
   Int4        i;                        /*index over a multi-alignment block*/
   Int4        qplace;                                     /*index into query*/
   Nlm_FloatHi Sigma;   /*Number of different characters occurring in matches
                  within a multi-alignment block, excluding identical columns*/
   Nlm_FloatHi intervalSigma;  /*Same as Sigma but includes identical columns*/
   Int4        alphabetSize;               /*number of characters in alphabet*/
   Int4        *participatingSequences;   /*array of part. seq. at a position*/
   Int4        *oldParticipatingSequences;/*array of part. seq. at a position*/
   Int4        posLocalVariety;/*number of different characters at a position*/
   Int4        *posLocalC; /*counts of how many of each letter in this column*/
   Int4        c;
   Int4        thisSeq;
   Int4        numParticipating;     /*number of seq. in this alignment block*/
   Int4        oldNumParticipating;  /*number of seq. in this alignment block*/
   Boolean     newSequenceSet; 
   Int4        p;                                        /*index on sequences*/

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
     if ((posSearch->posCount[qplace] > 1) && (posSearch->posIntervalSizes[qplace] > 0)) {
       oldNumParticipating = numParticipating;
       for(p =0; p < numParticipating; p++)
         oldParticipatingSequences[p] = participatingSequences[p];
       numParticipating = 0;
       for (seqIndex = 0; seqIndex <= numseq; seqIndex++) {
         if (!posSearch->posUseSequences[seqIndex]) continue;
	       if ((posSearch->posDescMatrix[seqIndex][qplace].used) &&
	           (posSearch->posDescMatrix[seqIndex][qplace].letter != GAP_CHAR)) {
	         participatingSequences[numParticipating] = seqIndex;
	         numParticipating++;
	       }
       }
       newSequenceSet = TRUE;
       if (numParticipating == oldNumParticipating) {
         for(p = 0; p < numParticipating; p++)
           if (oldParticipatingSequences[p] != participatingSequences[p]) break;
         if (p == numParticipating) newSequenceSet = FALSE;
       }
       if (newSequenceSet) {
	     Sigma = 0;
	     intervalSigma = 0;
	     for (seqIndex = 0; seqIndex <= numseq; seqIndex++) {
	       if (!posSearch->posUseSequences[seqIndex]) continue;
	       posSearch->posRowSigma[seqIndex] = 0.0;
	       posSearch->posA[seqIndex] = 0.0;
	     }
	     for (i = posSearch->posExtents[qplace].leftExtent;
	          i <= posSearch->posExtents[qplace].rightExtent; i++) {
	       posLocalVariety = 0;
	       for(c = 0; c < alphabetSize; c++) posLocalC[c] = 0;
	       for(seqIndex = 0; seqIndex < numParticipating; seqIndex++) {
	         thisSeq = participatingSequences[seqIndex];
/* used to check for GAP here */
	         if (0 == posLocalC[posSearch->posDescMatrix[thisSeq][i].letter])
/* letter (not a gap) not seen before in this query pos. */
	           posLocalVariety++;  
	         posLocalC[posSearch->posDescMatrix[thisSeq][i].letter]++;
	       }
	       intervalSigma += posLocalVariety;
	       if (posLocalVariety > 1) {
            Sigma += posLocalVariety;
	          for(seqIndex = 0; seqIndex < numParticipating; seqIndex++) {
	            thisSeq = participatingSequences[seqIndex];
/* used to check for gap here */
	            posSearch->posRowSigma[thisSeq] += ( 1.0 / (Nlm_FloatHi) posLocalC[posSearch->posDescMatrix[thisSeq][i].letter]);
	          }
	        }
	      }
      }
      if (Sigma > 0) {
        for (seqIndex = 0; seqIndex < numParticipating; seqIndex++) {
	        thisSeq = participatingSequences[seqIndex];
	        posSearch->posA[thisSeq] = posSearch->posRowSigma[thisSeq]/Sigma;
	      }
      } else {
        for (seqIndex = 0; seqIndex < numParticipating; seqIndex++) {
          thisSeq = participatingSequences[seqIndex];
	        posSearch->posA[thisSeq] = ((Nlm_FloatHi) 1 / (Nlm_FloatHi) numParticipating);
        }
      }
      posSearch->posSigma[qplace] = intervalSigma;
      for (seqIndex = 0; seqIndex < numParticipating; seqIndex++) {
	      thisSeq = participatingSequences[seqIndex];
        posSearch->posMatchWeights[qplace][posSearch->posDescMatrix[thisSeq][qplace].letter] += posSearch->posA[thisSeq];
      }
    }
  }
  MemFree(participatingSequences);
  MemFree(oldParticipatingSequences);
  MemFree(posLocalC);
}

/*---------------------------------------------------------------------------*/
/*check that weights add to 1 in each column */
/*---------------------------------------------------------------------------*/
void LIBCALL CddposCheckWeights(posSearchItems *posSearch, compactSearchItems * compactSearch)
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
       for(a = 0; a < alphabetSize; a++) 
           runningSum += posSearch->posMatchWeights[c][a];
       if((runningSum < 0.99) || (runningSum > 1.01))
         ErrPostEx(SEV_ERROR, 0, 0, "\nERROR IN WEIGHTS, column %d, value %lf\n",c, runningSum);
     }
   }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Nlm_FloatHi ** LIBCALL CddallocatePosFreqs(Int4 length, Int4 alphabetSize)
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


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Nlm_FloatHi LIBCALL countsFunction(Nlm_FloatHi Sigma, Int4 intervalLength)
{
  return(Sigma / intervalLength - 1);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Nlm_FloatHi ** LIBCALL CddposComputePseudoFreqs(posSearchItems *posSearch, compactSearchItems * compactSearch, Boolean Cpos)
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
   posFreqs = CddallocatePosFreqs(length, alphabetSize);

   for(c = 0; c < length; c++) {
     if ((posSearch->posCount[c] > 1) && (Xchar != q[c])) {
       infoSum = 0;
       for(a = 0; a < alphabetSize; a++) {
         if (compactSearch->standardProb[a] > posEpsilon) {
	   pseudo = 0;
	   for (aSub = 0; aSub < alphabetSize; aSub++)
	     if(compactSearch->matrix[a][aSub] != BLAST_SCORE_MIN) 
	       pseudo += (posSearch->posMatchWeights[c][aSub] *
			exp(lambda * compactSearch->matrix[a][aSub]));
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

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* from posit.c as of 12/29/2000                                             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddfreePosFreqs(Nlm_FloatHi ** posFreqs, Int4 length)
{
  Int4 i;

  for (i = 0; i <= length; i++)
    MemFree(posFreqs[i]);
  MemFree(posFreqs); 
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Nlm_FloatHi LIBCALL posit_rounddown(Nlm_FloatHi value)
{
  return (Nlm_FloatHi) Nlm_Nint(value);
}



/*---------------------------------------------------------------------------*/
/*Convert pseudo-count frequencies to a score matrix                         */
/*---------------------------------------------------------------------------*/
void LIBCALL CddposFreqsToMatrix(posSearchItems *posSearch, compactSearchItems * compactSearch)
{
   Uint1Ptr q;                                         /*pointer to the query*/
   Int4 length;                                         /*length of the query*/
   Int4 c;                                                       /*loop index*/
   Int4 a, alphabetSize;                    /*loop index and size of alphabet*/
   Nlm_FloatHi lambda;                            /*Karlin-Altschul parameter*/
   Nlm_FloatHi  qOverPEstimate, value;                   /*intermediate terms*/
   Boolean allZeros;                     /*are all frequencies in a column 0?*/

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
	 posSearch->posPrivateMatrix[c][a] = (BLAST_Score) posit_rounddown(POSIT_SCALE_FACTOR2*value);
       }
     }    
     if (allZeros) {
       for(a = 0; a < alphabetSize; a++) {
         posSearch->posMatrix[c][a] = compactSearch->matrix[q[c]][a];
	 if (compactSearch->matrix[q[c]][a] == BLAST_SCORE_MIN)
		posSearch->posPrivateMatrix[c][a] = BLAST_SCORE_MIN;
	 else
         	posSearch->posPrivateMatrix[c][a] = POSIT_SCALE_FACTOR2*compactSearch->matrix[q[c]][a];
       }
     }
   }
   for(a = 0; a < alphabetSize; a++) {
     posSearch->posPrivateMatrix[length][a] = posSearch->posPrivateMatrix[length][a] = BLAST_SCORE_MIN;
   }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddposScaling(posSearchItems *posSearch, compactSearchItems * compactSearch)
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


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Compute general information about the sequences that matched on the       */
/* i-th pass such as how many matched at each query position and what letter */
/* matched                                                                   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddposDenseDiagDemographics(posSearchItems     *posSearch,
                                         compactSearchItems *compactSearch,
                                         SeqAlignPtr        listOfSeqAligns)
{
   Uint1Ptr     q;                                     /*pointers into query */
   Uint1Ptr     s;                          /*pointer into a matching string */
   Int4         length, subjectLength;          /*length of query and subject*/
   Int4         c;                                      /*index into a string*/
   Int4         numseq, numSeqAligns; /*number of matching seq. and SeqAligns*/
   Int4         seqIndex;         /*index for the array of matching sequences*/
   Int4         matchLength;                              /*length of a match*/
   Int4         queryOffset, subjectOffset, retrievalOffset;  /*offsets needed to make a match align*/
   Int4         qplace, splace; /*index into query string and matching string*/
   SeqAlignPtr  curSeqAlign;                  /*pointers into listOfSeqAligns*/
   DenseDiagPtr curSegs;           /*used to extract alignm. from curSeqAlign*/
   SeqIdPtr     curId; /*Used to compare seq. coming from different SeqAligns*/
   Int4         startQ, startS;    /*Indices into array of starting positions*/
   Int4         segIndex;                   /*Index for which piece we are at*/
   Boolean      is_new_id = FALSE;

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

   numSeqAligns = CddCountDenDiagSeqAligns(listOfSeqAligns, &numseq);
   
   posSearch->posNumSequences = numSeqAligns;
   seqIndex = 0;
   curSeqAlign = listOfSeqAligns;
   for(curSeqAlign = listOfSeqAligns; curSeqAlign != NULL; curSeqAlign = curSeqAlign->next) {
     curSegs = (DenseDiagPtr) curSeqAlign->segs;
     s = CddGetSequenceWithDenseDiag(curSegs, FALSE, &retrievalOffset, &subjectLength);
     startQ = 0; startS = 1;
     while (curSegs) {
       queryOffset = curSegs->starts[0];
       subjectOffset = curSegs->starts[1] - retrievalOffset;
       matchLength = curSegs->len;
       for(c = 0, qplace = queryOffset, splace = subjectOffset; c < matchLength; c++, qplace++, splace++) {
         if (!posSearch->posDescMatrix[seqIndex+1][qplace].used) {
           posSearch->posDescMatrix[seqIndex+1][qplace].letter = (Int1) s[splace]; 
           posSearch->posDescMatrix[seqIndex+1][qplace].used = TRUE; 
           posSearch->posDescMatrix[seqIndex+1][qplace].e_value = 0;
         }
       }
       curSegs = curSegs->next;
     }
     s = MemFree(s);
     seqIndex++;
   } /*closes the while loop over seqAligns*/
}




/*---------------------------------------------------------------------------*/
/* Compute general information about the sequences that matched on the       */
/* i-th pass such as how many matched at each query position and what letter */
/* matched                                                                   */
/*---------------------------------------------------------------------------*/
void LIBCALL CddposDemographics(posSearchItems *posSearch,
                                compactSearchItems *compactSearch,
				SeqAlignPtr listOfSeqAligns)
{
   Uint1Ptr    q;                                      /*pointers into query */
   Uint1Ptr    s;                           /*pointer into a matching string */
   Int4        length, subjectLength;           /*length of query and subject*/
   Int4        c;                                       /*index into a string*/
   Int4        numseq, numSeqAligns;  /*number of matching seqs and SeqAligns*/
   Int4        seqIndex;          /*index for the array of matching sequences*/
   Int4        matchLength;                               /*length of a match*/
   Int4        queryOffset, subjectOffset, retrievalOffset;  /*offsets needed to make a match align*/
   Int4        qplace, splace;  /*index into query string and matching string*/
   SeqAlignPtr curSeqAlign, prevSeqAlign;     /*pointers into listOfSeqAligns*/
   DenseSegPtr curSegs, prevSegs; /*used to extract alignmts from curSeqAlign*/
   SeqIdPtr    curId, prevId;/*Use to comp. seqs coming from difft. SeqAligns*/
   Int4        startQ, startS;     /*Indices into array of starting positions*/
   Int4        numsegs;            /*Number of pieces in the gapped alignment*/
   Int4        segIndex;                    /*Index for which piece we are at*/
   Boolean     is_new_id = FALSE;

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

   numSeqAligns = CddCountSeqAligns(listOfSeqAligns, &numseq);
   
   posSearch->posNumSequences = numSeqAligns;
   seqIndex = 0;
   curSeqAlign = listOfSeqAligns;
   prevSeqAlign = NULL;
   for(curSeqAlign = listOfSeqAligns; curSeqAlign != NULL; curSeqAlign = curSeqAlign->next) {
     is_new_id = FALSE;
     curSegs = (DenseSegPtr) curSeqAlign->segs;
     if (NULL != prevSeqAlign) {
       prevSegs = (DenseSegPtr) prevSeqAlign->segs;
       if(curSegs->ids == NULL) break;
       curId = curSegs->ids->next; 
       prevId = prevSegs->ids->next;
       if (!(SeqIdMatch(curId, prevId))) is_new_id = TRUE;
     }
     if(is_new_id == TRUE) seqIndex++;
     s = CddGetSequenceWithDenseSeg(curSegs, FALSE, &retrievalOffset, &subjectLength);
     startQ = 0; startS = 1;
     numsegs = curSegs->numseg;
     for(segIndex = 0; segIndex < numsegs; segIndex++) {
       queryOffset = curSegs->starts[startQ];
       if (curSegs->starts[startS] != GAP_HERE)
         subjectOffset = curSegs->starts[startS] - retrievalOffset;
       else
         subjectOffset = GAP_HERE;
       matchLength = curSegs->lens[segIndex];
       if ((GAP_HERE ) == queryOffset) {
         ; /*do nothing, gap in query*/
       } else if ((GAP_HERE) == subjectOffset) {
         for(c = 0, qplace = queryOffset; c < matchLength; c++, qplace++) {
           posSearch->posDescMatrix[seqIndex + 1][qplace].used = TRUE;
           posSearch->posDescMatrix[seqIndex + 1][qplace].letter = GAP_CHAR;
           posSearch->posDescMatrix[seqIndex + 1][qplace].e_value = 1.0;
         }
       } else {  /*no gap*/
         for(c = 0, qplace = queryOffset, splace = subjectOffset; c < matchLength; c++, qplace++, splace++) {
           if ((!posSearch->posDescMatrix[seqIndex+1][qplace].used)) 
             if (!posSearch->posDescMatrix[seqIndex+1][qplace].used) {
               posSearch->posDescMatrix[seqIndex+1][qplace].letter = (Int1) s[splace]; 
               posSearch->posDescMatrix[seqIndex+1][qplace].used = TRUE; 
               posSearch->posDescMatrix[seqIndex+1][qplace].e_value = 0;
             }
         }
       }
       startQ += 2;
       startS += 2;
     }
     prevSeqAlign = curSeqAlign;
     s = MemFree(s);
   } /*closes the while loop over seqAligns*/
}


/*---------------------------------------------------------------------------*/
/* copied from BLASTSetUpSearchInternalByLoc                                 */
/*---------------------------------------------------------------------------*/
#define DROPOFF_NUMBER_OF_BITS 10.0
#define INDEX_THR_MIN_SIZE 20000
static Int2 LIBCALL CddSetUpSearchInternalByLoc
                       (BlastSearchBlkPtr search, SeqLocPtr query_slp, BioseqPtr query_bsp,
                        CharPtr prog_name, Int4 qlen, BLAST_OptionsBlkPtr options,
                        int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)))
{
	BioseqPtr bsp_temp, bsp;
	Boolean mask_at_hash=FALSE, private_slp_delete;
	Boolean query_is_na, db_is_na;
	Char buffer[128];
	Int2 retval, status;
	Int4 effective_query_length, query_length, full_query_length,
		index, length, length_adjustment=0, last_length_adjustment, min_query_length;
	Int4 array_size, max_length, block_width;
	Int4Ptr open, extend;
	Nlm_FloatHiPtr lambda, K, H;
	Nlm_FloatHi avglen;
	ReadDBFILEPtr rdfp;
	SeqIdPtr query_id;
	ObjectIdPtr oip;
	SeqPortPtr spp=NULL, spp_reverse=NULL;
	SeqLocPtr filter_slp=NULL, private_slp=NULL, private_slp_rev=NULL;
	GeneticCodePtr gcp;
	Uint1 residue, strand;
	Uint1Ptr sequence;
	Uint1Ptr query_seq, query_seq_start, query_seq_rev, query_seq_start_rev;
	ValNodePtr vnp;

	if (options == NULL)
	{
	  	ErrPostEx(SEV_FATAL, 0, 0, "BLAST_OptionsBlkPtr is NULL\n");
		return 1;
	}

	if (query_slp == NULL && query_bsp == NULL)
	{
	  	ErrPostEx(SEV_FATAL, 0, 0, "Query is NULL\n");
		return 1;
	}

	query_seq = NULL;	/* Gets rid of warning. */
	query_seq_rev = NULL;	/* Gets rid of warning. */
	query_seq_start = NULL;	/* Gets rid of warning. */
	query_seq_start_rev = NULL;	/* Gets rid of warning. */

	if (query_slp)
	{
		strand = SeqLocStrand(query_slp);
		if (strand == Seq_strand_unknown || strand == Seq_strand_plus || strand == Seq_strand_both)
		{
			private_slp = SeqLocIntNew(SeqLocStart(query_slp), SeqLocStop(query_slp), Seq_strand_plus, SeqLocId(query_slp));
		}
		if (strand == Seq_strand_minus || strand == Seq_strand_both)
		{
			private_slp_rev = SeqLocIntNew(SeqLocStart(query_slp), SeqLocStop(query_slp), Seq_strand_minus, SeqLocId(query_slp));
		}
		private_slp_delete = TRUE;
	}
	else
	{
		private_slp = SeqLocIntNew(0, query_bsp->length-1 , Seq_strand_plus, SeqIdFindBest(query_bsp->id, SEQID_GI));
		private_slp_rev = SeqLocIntNew(0, query_bsp->length-1 , Seq_strand_minus, SeqIdFindBest(query_bsp->id, SEQID_GI));
		private_slp_delete = FALSE;
	}

	query_length = 0;
	if (private_slp)
		query_length = SeqLocLen(private_slp);
	else if (private_slp_rev)
		query_length = SeqLocLen(private_slp_rev);
	if (query_length == 0)
	{
		sprintf(buffer, "No valid query sequence");
		BlastConstructErrorMessage("Blast", buffer, 2, &(search->error_return));
		return 1;
	}

	bsp = NULL;
	if (private_slp)
		bsp = BioseqLockById(SeqLocId(private_slp));
	else if (private_slp_rev)
		bsp = BioseqLockById(SeqLocId(private_slp_rev));

	if (bsp == NULL)
	{
	  	ErrPostEx(SEV_WARNING, 0, 0, "No valid query sequence, BioseqLockById returned NULL\n");
		return 1;
	}

	full_query_length = bsp->length;

	BlastGetTypes(prog_name, &query_is_na, &db_is_na);
	if (query_is_na != ISA_na(bsp->mol))
	{
	  	ErrPostEx(SEV_WARNING, 0, 0, "Query molecule is incompatible with %s program", prog_name);
		BioseqUnlock(bsp);
		return 1;
	}

	if (bsp->repr == Seq_repr_virtual)
	{
		BioseqUnlock(bsp);
	  	ErrPostEx(SEV_WARNING, 0, 0, "Virtual sequence detected\n");
		return 1;
	}
	BioseqUnlock(bsp);

	if (query_slp)	
	{
		search->query_slp = query_slp;
	}
	else
	{
		search->query_slp = private_slp;
		search->allocated += BLAST_SEARCH_ALLOC_QUERY_SLP;
	}
		

	search->translation_buffer = NULL;
	search->translation_buffer_size = 0;

	/* 
	Get genetic codes (should be determined from BLAST_OptionsBlkPtr.
	Only needed for blastx, tblast[nx] 
	*/
	if (StringCmp(prog_name, "blastp") != 0 && StringCmp(prog_name, "blastn") != 0)
	{
		if (StringCmp(prog_name, "tblastx") == 0 || StringCmp(prog_name, "tblastn") == 0)
		{
			gcp = GeneticCodeFind(options->db_genetic_code, NULL);
			for (vnp = (ValNodePtr)gcp->data.ptrvalue; vnp != NULL; vnp = vnp->next)
			{
				if (vnp->choice == 3)	/* ncbieaa */
				{
					search->db_genetic_code = (CharPtr)vnp->data.ptrvalue;
					break;
				}
			}
			search->translation_table = (Uint1Ptr) GetPrivatTranslationTable(search->db_genetic_code, FALSE);
			search->translation_table_rc = (Uint1Ptr) GetPrivatTranslationTable(search->db_genetic_code, TRUE);
			max_length = 0;
			rdfp = search->rdfp;
			while (rdfp)
			{
				max_length = MAX(max_length, readdb_get_maxlen(rdfp));
				rdfp = rdfp->next;
			}
			search->translation_buffer = MemNew((3+(max_length/3))*sizeof(Uint1));
			search->translation_buffer_size = 1+(max_length/3);
			search->allocated += BLAST_SEARCH_ALLOC_TRANS_INFO;
		}

		if (StringCmp(prog_name, "blastx") == 0 || StringCmp(prog_name, "tblastx") == 0)
		{
			gcp = GeneticCodeFind(options->genetic_code, NULL);
			for (vnp = (ValNodePtr)gcp->data.ptrvalue; vnp != NULL; vnp = vnp->next)
			{
				if (vnp->choice == 3)	/* ncbieaa */
				{
					search->genetic_code = (CharPtr)vnp->data.ptrvalue;
					break;
				}
			}
		}
	}

	if (options->filter && !options->filter_string)
		options->filter_string = (CharPtr) BlastConstructFilterString(options->filter);

	/* If the query is translated do this below. */ 
	if (StringCmp(prog_name, "blastx") && StringCmp(prog_name, "tblastx")) {
		if (private_slp)
			filter_slp = BlastSeqLocFilterEx(private_slp, options->filter_string, &mask_at_hash);
		else if (private_slp_rev)
			filter_slp = BlastSeqLocFilterEx(private_slp_rev, options->filter_string, &mask_at_hash);
	}


	/* 
           Dusting of query sequence. Only needed for blastn, optional
        */

        if(StringCmp(prog_name, "blastn") == 0) {
		if (filter_slp && !mask_at_hash)
			ValNodeAddPointer(&(search->mask), SEQLOC_MASKING_NOTSET, filter_slp);
        }

	if (StringCmp(prog_name, "blastp") == 0 || StringCmp(prog_name, "tblastn") == 0)
	{
		spp = SeqPortNewByLoc(private_slp, Seq_code_ncbistdaa);
                SeqPortSet_do_virtual(spp, TRUE);
		if (filter_slp && !mask_at_hash)
			ValNodeAddPointer(&(search->mask), SEQLOC_MASKING_NOTSET, filter_slp);
	}
	else if (StringCmp(prog_name, "blastx") == 0 || StringCmp(prog_name, "tblastx") == 0 || StringCmp(prog_name, "blastn") == 0)
	{
		if (private_slp)
		{
			spp = SeqPortNewByLoc(private_slp, Seq_code_ncbi4na);
                	SeqPortSet_do_virtual(spp, TRUE);
		}
		if (private_slp_rev)
		{
			spp_reverse = SeqPortNewByLoc(private_slp_rev, Seq_code_ncbi4na);
                	SeqPortSet_do_virtual(spp_reverse, TRUE);
		}
	}
	else
	{
	  	ErrPostEx(SEV_FATAL, 0, 0, "Only blastn, blastp, blastx, tblastn tblastx is allowed\n");
		return 1;
	}

	if (spp)
	{
		query_seq_start = (Uint1Ptr) MemNew(((query_length)+2)*sizeof(Char));
		query_seq_start[0] = NULLB;
		query_seq = query_seq_start+1;
		index=0;
		while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF)
		{

			if (IS_residue(residue))
			{
				query_seq[index] = residue;
				index++;
			}
		}
		query_seq[index] = NULLB;
		spp = SeqPortFree(spp);
		if (StringCmp(prog_name, "blastn") == 0)
		{
			if (filter_slp)
			{
				if (mask_at_hash)
                			search->context[0].location =
                        			BlastSeqLocFillDoubleInt(filter_slp, query_length, FALSE);
				else
					BlastMaskTheResidues(query_seq, full_query_length, 15, filter_slp, FALSE, SeqLocStart(private_slp));
			}
			for (index=0; index<query_length; index++)
				query_seq[index] = ncbi4na_to_blastna[query_seq[index]];
		}
	}

	if (spp_reverse)
	{
		query_seq_start_rev = (Uint1Ptr) MemNew(((query_length)+2)*sizeof(Char));
		query_seq_start_rev[0] = NULLB;
		query_seq_rev = query_seq_start_rev+1;
		index=0;
		while ((residue=SeqPortGetResidue(spp_reverse)) != SEQPORT_EOF)
		{
			if (IS_residue(residue))
			{
				query_seq_rev[index] = residue;
				index++;
			}
		}
		query_seq_rev[index] = NULLB;
		spp_reverse = SeqPortFree(spp_reverse);
		if (StringCmp(prog_name, "blastn") == 0)
		{
			if (filter_slp)
			{
				if (mask_at_hash)
                			search->context[1].location =
                        			BlastSeqLocFillDoubleInt(filter_slp, query_length, TRUE);
				else
					BlastMaskTheResidues(query_seq_rev, full_query_length, 15, filter_slp, TRUE, full_query_length - SeqLocStop(private_slp_rev) - 1);
			}
			for (index=0; index<query_length; index++)
				query_seq_rev[index] = ncbi4na_to_blastna[query_seq_rev[index]];
		}
	}

/*
	Set the context_factor, which specifies how many different 
	ways the query or db is examined (e.g., blastn looks at both
	stands of query, context_factor is 2).
*/
	if (StringCmp(prog_name, "blastp") == 0)
	{
		search->context_factor = 1;
		length = query_length;
	}
	else if (StringCmp(prog_name, "blastn") == 0)
	{	/* two strands */
		search->context_factor = (search->last_context-search->first_context+1);
		length = query_length;
	}
	else if (StringCmp(prog_name, "blastx") == 0)
	{	/* query translated in six frames. */
		search->context_factor = search->last_context-search->first_context+1;
		length = query_length/3;
	}
	else if (StringCmp(prog_name, "tblastn") == 0)
	{	/* db translated in six frames. */
		search->context_factor = 6;
		length = query_length;
	}
	else if (StringCmp(prog_name, "tblastx") == 0)
	{	/* db and query each translated in six frames. */
		search->context_factor = 6*CODON_LENGTH*(search->last_context-search->first_context+1);
		length = query_length/3;
	}

	if (private_slp)
		query_id = SeqIdFindBest(SeqLocId(private_slp), SEQID_GI);
	else
		query_id = SeqIdFindBest(SeqLocId(private_slp_rev), SEQID_GI);

	search->query_id = SeqIdDup(query_id);

/* Store the query sequence, or the translation thereof. */
	if (StringCmp(prog_name, "blastp") == 0 || StringCmp(prog_name, "tblastn") == 0)
	{	/* One blastp context for now. */
		if (filter_slp)
		{
			if (mask_at_hash)
                		search->context[0].location =
                        		BlastSeqLocFillDoubleInt(filter_slp, query_length, FALSE);
			else
				BlastMaskTheResidues(query_seq, full_query_length, 21, filter_slp, FALSE, SeqLocStart(private_slp));
		}
		BlastSequenceAddSequence(search->context[0].query, NULL, query_seq_start, query_length, query_length, 0);
	}
	else if (StringCmp(prog_name, "blastx") == 0  || StringCmp(prog_name, "tblastx") == 0)
	{
		
		for (index=search->first_context; index<=search->last_context; index++)
		{
		   if (search->context[index].query->frame > 0)
		   {
			sequence = GetTranslation(query_seq, query_length, search->context[index].query->frame, &length, search->genetic_code);
		   }
		   else
		   {
			sequence = GetTranslation(query_seq_rev, query_length, search->context[index].query->frame, &length, search->genetic_code);
		   }
		   if (options->filter_string && length > 0)
		   {
		  	bsp_temp = (BioseqPtr) BlastMakeTempProteinBioseq(sequence+1, length, Seq_code_ncbistdaa);
			bsp_temp->id = SeqIdSetFree(bsp_temp->id);
			/*
			bsp_temp->id = search->query_id;
			*/
			oip = (ObjectIdPtr) UniqueLocalId();
			ValNodeAddPointer(&(bsp_temp->id), SEQID_LOCAL, oip);
			SeqMgrAddToBioseqIndex(bsp_temp);
			
			filter_slp = BlastBioseqFilterEx(bsp_temp, options->filter_string, &mask_at_hash);
			HackSeqLocId(filter_slp, search->query_id);

			SeqMgrDeleteFromBioseqIndex(bsp_temp);
			
			bsp_temp->id = SeqIdSetFree(bsp_temp->id);
			bsp_temp = BioseqFree(bsp_temp);
			if (mask_at_hash)
			{
                		search->context[index].location = 
					BlastSeqLocFillDoubleInt(filter_slp, query_length, FALSE);
			}
			else
			{
				BlastMaskTheResidues(sequence+1, length, 21, filter_slp, FALSE, SeqLocStart(private_slp));
				BlastConvertProteinSeqLoc(filter_slp, search->context[index].query->frame, query_length);
			}
			if (filter_slp && !mask_at_hash)
				ValNodeAddPointer(&(search->mask), FrameToDefine(search->context[index].query->frame), filter_slp);
		   }
		   BlastSequenceAddSequence(search->context[index].query, NULL, sequence, length, query_length, 0);
		}
		query_seq_start = MemFree(query_seq_start);
		query_seq_start_rev = MemFree(query_seq_start_rev);
	}
	else if (StringCmp(prog_name, "blastn") == 0)
	{
		if (search->first_context == 0)
			BlastSequenceAddSequence(search->context[0].query, NULL, query_seq_start, query_length, query_length, 0);
		if (search->last_context == 1)
			BlastSequenceAddSequence(search->context[1].query, NULL, query_seq_start_rev, query_length, query_length, 0);
	}

	if (mask_at_hash)
	{ /* No longer needed. */
		filter_slp = SeqLocSetFree(filter_slp);
	}
	
/* Set the ambiguous residue before the ScoreBlk is filled. */
	if (StringCmp(prog_name, "blastn") != 0)
	{
		search->sbp->read_in_matrix = TRUE;
		BlastScoreSetAmbigRes(search->sbp, 'X');
	}
	else
	{
  	        if(options->matrix!=NULL) {
		     search->sbp->read_in_matrix = TRUE;
	        } else {
		     search->sbp->read_in_matrix = FALSE;
	        }
		BlastScoreSetAmbigRes(search->sbp, 'N');
	}


	search->sbp->penalty = options->penalty;
	search->sbp->reward = options->reward;
        
        /* option is to use alignments choosen by user in PSM computation API (used in WWW PSI-Blast); */
	search->pbp->use_best_align = options->use_best_align;
	
	/* Should culling be used at all? */
	search->pbp->perform_culling = options->perform_culling;
	search->pbp->hsp_range_max = options->hsp_range_max;
        /* This assures that search->pbp->max_pieces is at least one wide. */
        block_width = MIN(query_length, options->block_width);
        if (block_width > 0)
                search->pbp->max_pieces = query_length/block_width;

	search->sbp->query_length = query_length;

	search->result_struct = BLASTResultsStructNew(search->result_size, search->pbp->max_pieces, search->pbp->hsp_range_max);
	if (options->matrix != NULL)
		status = BlastScoreBlkMatFill(search->sbp, options->matrix);
	else
		status = BlastScoreBlkMatFill(search->sbp, "BLOSUM62");
	if (status != 0)
	{
		ErrPostEx(SEV_WARNING, 0, 0, "BlastScoreBlkMatFill returned non-zero status");
		return 1;
	}

	/* This is used right below. */
	search->pbp->gapped_calculation = options->gapped_calculation;
	search->pbp->do_not_reevaluate = options->do_not_reevaluate;
	search->pbp->do_sum_stats = options->do_sum_stats;
	search->pbp->first_db_seq = options->first_db_seq;
	search->pbp->final_db_seq = options->final_db_seq;

	retval = 0;
	for (index=search->first_context; index<=search->last_context; index++)
	{
		status = BlastScoreBlkFill(search->sbp, (CharPtr) search->context[index].query->sequence,search->context[index].query->length, index);
		if (status != 0)
		{
			sprintf(buffer, "Unable to calculate Karlin-Altschul params, check query sequence");
			BlastConstructErrorMessage("BLASTSetUpSearch", buffer, 2, &(search->error_return));
			retval = 1;
		}
		if (search->pbp->gapped_calculation &&
				StringCmp(search->prog_name, "blastn") != 0)
		{
			search->sbp->kbp_gap_std[index] = BlastKarlinBlkCreate();
                	status = BlastKarlinBlkGappedCalc(search->sbp->kbp_gap_std[index], options->gap_open, options->gap_extend, search->sbp->name, &(search->error_return));
			if (status != 0)
			{
				retval = 1;
			}
			search->sbp->kbp_gap_psi[index] = BlastKarlinBlkCreate();
                	status = BlastKarlinBlkGappedCalc(search->sbp->kbp_gap_psi[index], options->gap_open, options->gap_extend, search->sbp->name, &(search->error_return));
			if (status != 0)
			{
				retval = 1;
			}
		}
	}

	search->sbp->kbp_gap = search->sbp->kbp_gap_std;
        search->sbp->kbp = search->sbp->kbp_std;
	if (StringCmp(prog_name, "blastn") != 0)
	{
		array_size = BlastKarlinGetMatrixValues(search->sbp->name, &open, &extend, &lambda, &K, &H, NULL);
		if (array_size > 0)
		{
			for (index=0; index<array_size; index++)
			{
				if (open[index] == INT2_MAX && extend[index] == INT2_MAX)
				{
					search->sbp->kbp_ideal = BlastKarlinBlkCreate();
					search->sbp->kbp_ideal->Lambda = lambda[index];
					search->sbp->kbp_ideal->K = K[index];
					search->sbp->kbp_ideal->H = H[index];
				}
			}
			MemFree(open);
			MemFree(extend);
			MemFree(lambda);
			MemFree(K);
			MemFree(H);
		}
		if (search->sbp->kbp_ideal == NULL)
        		search->sbp->kbp_ideal = BlastKarlinBlkStandardCalcEx(search->sbp);
	}

	/* Adjust the Karlin parameters. */
	if (StringCmp(prog_name, "blastx") == 0  || StringCmp(prog_name, "tblastx") == 0)
	{
                BlastKarlinBlkStandardCalc(search->sbp, search->first_context, search->last_context);
	}

	/* If retval was set non-zero above (by the routines calculating Karlin-Altschul params),
	   return here before these values are used.
	*/
	if (retval)
		return retval;


	if (search->pbp->gapped_calculation &&
		StringCmp(search->prog_name, "blastn") != 0)
		min_query_length = 1/(search->sbp->kbp_gap_std[search->first_context]->K);
	else
		min_query_length = 1/(search->sbp->kbp[search->first_context]->K);

	last_length_adjustment = 0;
	for (index=0; index<5; index++)
	{
		length_adjustment = ((search->sbp->kbp[search->first_context]->logK)+log((Nlm_FloatHi)(length-last_length_adjustment)*(Nlm_FloatHi)(MAX(1, (search->dblen)-(search->dbseq_num*last_length_adjustment)))))/(search->sbp->kbp[search->first_context]->H);
		if (length_adjustment >= length-min_query_length)
		{
			length_adjustment = length-min_query_length;
			break;
		}
	
		if (ABS(last_length_adjustment-length_adjustment) <= 1)
			break;
		last_length_adjustment = length_adjustment;
	}
	search->length_adjustment = MAX(length_adjustment, 0);

	search->dblen_eff = MAX(1, search->dblen - search->dbseq_num*search->length_adjustment);
	effective_query_length = MAX(length - search->length_adjustment, min_query_length);
	
	for (index=search->first_context; index<=search->last_context; index++)
	{
		search->context[index].query->effective_length = effective_query_length;
	}

	if (search->searchsp_eff == 0)
		search->searchsp_eff = ((Nlm_FloatHi) search->dblen_eff)*((Nlm_FloatHi) effective_query_length);

	/* The default is that cutoff_s was not set and is zero. */
	if (options->cutoff_s == 0)
	{
		search->pbp->cutoff_e = options->expect_value;
		search->pbp->cutoff_e_set = TRUE;
		search->pbp->cutoff_s = options->cutoff_s;
		search->pbp->cutoff_s_set = FALSE;
	}
	else
	{
		search->pbp->cutoff_e = options->expect_value;
		search->pbp->cutoff_e_set = FALSE;
		search->pbp->cutoff_s = options->cutoff_s;
		search->pbp->cutoff_s_set = TRUE;
	}
/* For now e2 is set to 0.5 and cutoff_e2_set is FALSE.  This is then
changed to the proper values in blast_set_parameters.  In the final version
of this program (where more blast programs and command-line options are
available) this needs to be set higher up. */
	if (options->cutoff_s2 == 0)
	{
		search->pbp->cutoff_e2 = options->e2;
		search->pbp->cutoff_e2_set = FALSE;
		search->pbp->cutoff_s2 = options->cutoff_s2;
		search->pbp->cutoff_s2_set = FALSE;
	}
	else
	{
		search->pbp->cutoff_e2 = options->e2;
		search->pbp->cutoff_e2_set = FALSE;
		search->pbp->cutoff_s2 = options->cutoff_s2;
		search->pbp->cutoff_s2_set = TRUE;
	}
	
	search->pbp->discontinuous = options->discontinuous;

	
	/* For postion based blast. */
	search->pbp->ethresh = options->ethresh;
	search->pbp->maxNumPasses = options->maxNumPasses;
	search->pbp->pseudoCountConst = options->pseudoCountConst;

	search->pbp->process_num = options->number_of_cpus;
	search->pbp->cpu_limit = options->cpu_limit;
	search->pbp->gap_decay_rate = options->gap_decay_rate;
	search->pbp->gap_size = options->gap_size;
	search->pbp->gap_prob = options->gap_prob;
	search->pbp->old_stats = options->old_stats;
	search->pbp->use_large_gaps = options->use_large_gaps;
	search->pbp->number_of_bits = options->number_of_bits;
	search->pbp->two_pass_method = options->two_pass_method;
	search->pbp->multiple_hits_only = options->multiple_hits_only;
	search->pbp->gap_open = options->gap_open;
	search->pbp->gap_extend = options->gap_extend;
        search->pbp->decline_align = options->decline_align;

	search->pbp->hsp_num_max = options->hsp_num_max;
/* CHANGE HERE??? */
	if (search->pbp->gapped_calculation && StringCmp(search->prog_name, "blastn"))
	{
		search->pbp->cutoff_s2_set = TRUE;
		if (StringCmp(search->prog_name, "blastn") != 0)
		{
			search->pbp->gap_x_dropoff = (BLAST_Score) (options->gap_x_dropoff*NCBIMATH_LN2 / search->sbp->kbp_gap[search->first_context]->Lambda);
			search->pbp->gap_x_dropoff_final = (BLAST_Score) (options->gap_x_dropoff_final*NCBIMATH_LN2 / search->sbp->kbp_gap[search->first_context]->Lambda);
			search->pbp->gap_trigger = (BLAST_Score) ((options->gap_trigger*NCBIMATH_LN2+search->sbp->kbp[search->first_context]->logK)/ search->sbp->kbp[search->first_context]->Lambda);
		}
		else
		{
			search->pbp->gap_x_dropoff = (BLAST_Score) (options->gap_x_dropoff*NCBIMATH_LN2 / search->sbp->kbp[search->first_context]->Lambda);
			search->pbp->gap_x_dropoff_final = (BLAST_Score) (options->gap_x_dropoff_final*NCBIMATH_LN2 / search->sbp->kbp[search->first_context]->Lambda);
			search->pbp->gap_trigger = (BLAST_Score) ((options->gap_trigger*NCBIMATH_LN2+search->sbp->kbp[search->first_context]->logK)/ search->sbp->kbp[search->first_context]->Lambda);
		}
		/* The trigger value sets the s2 cutoff. */
		search->pbp->cutoff_s2 = search->pbp->gap_trigger;
	}
	else
	{
		search->pbp->gap_x_dropoff = (BLAST_Score) (options->gap_x_dropoff*NCBIMATH_LN2 / search->sbp->kbp[search->first_context]->Lambda);
		search->pbp->gap_x_dropoff_final = (BLAST_Score) (options->gap_x_dropoff_final*NCBIMATH_LN2 / search->sbp->kbp[search->first_context]->Lambda);
		search->pbp->gap_trigger = (BLAST_Score) ((options->gap_trigger*NCBIMATH_LN2+search->sbp->kbp[search->first_context]->logK)/ search->sbp->kbp[search->first_context]->Lambda);
		/* Set S and S2 equal if not sum stats. */
		if (search->pbp->do_sum_stats == FALSE)
			search->pbp->cutoff_s2 = search->pbp->cutoff_s;
	}
	/* Ensures that gap_x_dropoff_final is at least as large as gap_x_dropoff. */
	search->pbp->gap_x_dropoff_final = MAX(search->pbp->gap_x_dropoff_final, search->pbp->gap_x_dropoff);

/* "threshold" (first and second) must be set manually for two-pass right now.*/
	search->pbp->threshold_set = TRUE;
	search->pbp->threshold_first = options->threshold_first;
	search->pbp->threshold_second = options->threshold_second;

	search->pbp->window_size = options->window_size;
	search->pbp->window_size_set = TRUE;

	search->whole_query = TRUE;
	if (options->required_start != 0 || options->required_end != -1)
	{
		search->whole_query = FALSE;
		search->required_start = options->required_start;
		if (options->required_end != -1)
			search->required_end = options->required_end;
		else
			search->required_end = query_length;
	}

	if (qlen <= 0)
		qlen = query_length;

	/* Use DROPOFF_NUMBER_OF_BITS as the default if it's set to zero. */
	if (options->dropoff_1st_pass == 0)
		options->dropoff_1st_pass = (Int4) DROPOFF_NUMBER_OF_BITS;

	if (options->dropoff_2nd_pass == 0)
		options->dropoff_2nd_pass = (Int4) DROPOFF_NUMBER_OF_BITS;

	if (StringCmp(search->prog_name, "blastn") != 0)
	{
		avglen = BLAST_AA_AVGLEN;
	}
	else
	{
		avglen = BLAST_NT_AVGLEN;
		/* Use only one type of gap for blastn */
		search->pbp->ignore_small_gaps = TRUE;
	}

	if (blast_set_parameters(search, options->dropoff_1st_pass, options->dropoff_2nd_pass, avglen, search->searchsp_eff, options->window_size) != 0)
		return 1;

	search->thr_info->awake_index = FALSE;

	/* Only do this if this is not a pattern search. */
	if (options->isPatternSearch == FALSE)
	{
	     for (index=search->first_context; index<=search->last_context; index++)
	     {
		if (options->threshold_first > 0)
		{
			search->wfp = search->wfp_first;
			if (search->whole_query == TRUE)
			  if (!(search->positionBased)) /*AAS*/
			    status = BlastFindWords(search, 0, search->context[index].query->length, options->threshold_first, (Uint1) index);
			  else
			    status = BlastNewFindWords(search, 0, search->context[index].query->length, options->threshold_first, (Uint1) index);
			else
			  if (!(search->positionBased)) /*AAS*/
			    status = BlastFindWords(search, search->required_start, search->required_end, options->threshold_first, (Uint1) index);
			  else
			    status = BlastFindWords(search, search->required_start, search->required_end, options->threshold_first, (Uint1) index);
			if (status != 0) {
                            search->thr_info->awake_index = FALSE;
                            ErrPostEx(SEV_WARNING, 0, 0, "BlastFindWords returned non-zero status");
                            return 1;
			}
		}
		search->wfp = search->wfp_second;
		if (StringCmp(prog_name, "blastn") != 0)
		{
		    if (search->allocated & BLAST_SEARCH_ALLOC_WFP_SECOND)
		    {
			if (search->whole_query == TRUE)
			  if (!(search->positionBased))
			    status = BlastFindWords(search, 0, search->context[index].query->length, options->threshold_second, (Uint1) index);
			  else
			    status = BlastNewFindWords(search, 0, search->context[index].query->length, options->threshold_second, (Uint1) index);
			else
			  if (!(search->positionBased))
			    status = BlastFindWords(search, search->required_start, search->required_end, options->threshold_second, (Uint1) index);
			  else
			    status = BlastNewFindWords(search, search->required_start, search->required_end, options->threshold_second, (Uint1) index);
		    }
		}
		else
		{
			status = BlastNtFindWords(search, 0, search->context[index].query->length, 
				                      (Uint1) index);
		}

		if (status > 0)
		{
			search->thr_info->awake_index = FALSE;
			sprintf(buffer, "No valid letters to be indexed");
			BlastConstructErrorMessage("Blast", buffer, 2, &(search->error_return));
			return 1;
		}
		else if (status < 0)
		{
			search->thr_info->awake_index = FALSE;
			sprintf(buffer, "Error finding words");
			BlastConstructErrorMessage("Blast", buffer, 2, &(search->error_return));
			return 1;
		}
	    }
	    lookup_position_aux_destruct(search->wfp->lookup);
	}
	/* 
	Turn off the index thread by setting this flag.  Don't wait for a join, as the
	search will take much longer than the one second for this to die.
	*/
	search->thr_info->awake_index = FALSE;


	if (private_slp && private_slp_delete)
		private_slp = SeqLocFree(private_slp);
	if (private_slp_rev)
		private_slp_rev = SeqLocFree(private_slp_rev);

	return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
BlastSearchBlkPtr LIBCALL CddSetUpSearchWithReadDb(SeqLocPtr query_slp,
                                                   BioseqPtr query_bsp,
                                                   CharPtr prog_name,
                                                   Int4 qlen,
                                                   CharPtr dbname,
                                                   BLAST_OptionsBlkPtr options,
                                                   int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)),
                                                   SeqIdPtr seqid_list,
                                                   BlastDoubleInt4Ptr gi_list,
                                                   Int4 gi_list_total,
                                                   ReadDBFILEPtr rdfp)

{
	BlastSearchBlkPtr search;
	Boolean           multiple_hits, options_alloc=FALSE;
	Int2              status, first_context, last_context;
  Int8	            dblen;
	Int4	            query_length;
  ValNodePtr	      vnp;
  Int4		          i;
	Nlm_FloatHi	      searchsp_eff=0;
	Boolean		        use_private_gilist = FALSE;
	OIDListPtr	      alias_oidlist;
	Int4		          mask_index, virtual_mask_index;
	Uint4		          oid_bit, virtual_oid_bit;
	ReadDBFILEPtr	    tmprdfp;
        
	if (options == NULL) {
		options = BLASTOptionNew(prog_name, FALSE);	options_alloc = TRUE;
	}
        
	if (options->window_size != 0) multiple_hits = TRUE;
	else multiple_hits = FALSE;

	BlastGetFirstAndLastContext(prog_name, query_slp, &first_context, &last_context, options->strand_option);

	if (query_slp) query_length = SeqLocLen(query_slp);
	else query_length = query_bsp->length;
		
/* On the first call query length is used for the subject length. */
	search = BlastSearchBlkNewExtra(options->wordsize, query_length, dbname, multiple_hits, options->threshold_first, options->threshold_second, options->hitlist_size, prog_name, NULL, first_context, last_context, rdfp, options->window_size);

	if (search) {
		readdb_get_totals(search->rdfp, &(dblen), &(search->dbseq_num));
		if (seqid_list) BlastAdjustDbNumbers(search->rdfp, &(dblen), &(search->dbseq_num), seqid_list, NULL, NULL, NULL, 0);

		if (!gi_list) {
/* Ok, we do not have a gi-list specified, but maybe
	we have an a mask database in the list of databases,
	we need to create one mask for all such databases */
      OIDListPtr		virtual_oidlist = NULL;
      Int4		      final_virtual_db_seq=0, final_db_seq=0;
      Int4		      mask, oid, virtual_oid, maskindex, virtual_mask_index, total_virtual_mask, base;
      Uint4		      virtual_oid_bit;

      tmprdfp = search->rdfp;
      while (tmprdfp) {
        final_virtual_db_seq = tmprdfp->stop;
			  if (!tmprdfp->oidlist) final_db_seq = tmprdfp->stop;
			  tmprdfp = tmprdfp->next;
		  }
      tmprdfp = search->rdfp;
      while (tmprdfp) {
			  if (tmprdfp->oidlist) {
			    if (!virtual_oidlist) {
/* create new oidlist for virtual database */
				    virtual_oidlist = (OIDListPtr) MemNew(sizeof(OIDList));
				    virtual_oidlist->total = final_virtual_db_seq + 1;
				    total_virtual_mask = final_virtual_db_seq/MASK_WORD_SIZE + 2;
				    virtual_oidlist->list = (Uint4Ptr) MemNew (total_virtual_mask*sizeof(Int4));
			    }
/* Now populate the virtual_oidlist */
			    maskindex = 0;
			    base = 0;
			    while (maskindex < (tmprdfp->oidlist->total/MASK_WORD_SIZE +1)) {
/* for each long-word mask */
				    mask = Nlm_SwapUint4(tmprdfp->oidlist->list[maskindex]);
            i = 0;
				    while (mask) {
				      if (mask & (((Uint4)0x1)<<(MASK_WORD_SIZE-1))) {
					    oid = base + i;
					    virtual_oid = oid + tmprdfp->start;
              virtual_mask_index = virtual_oid/MASK_WORD_SIZE;
					    virtual_oid_bit = 0x1 << (MASK_WORD_SIZE - 1 - virtual_oid % MASK_WORD_SIZE);
					    virtual_oidlist->list[virtual_mask_index] |= virtual_oid_bit;
				    }
				    mask <<= 1;
				    i++;
				  }
				  maskindex++;
				  base += MASK_WORD_SIZE;
        }
/* free old mask */
			  tmprdfp->oidlist = OIDListFree(tmprdfp->oidlist);
      }
			tmprdfp = tmprdfp->next;
    }
    if (virtual_oidlist) {
      for (i=0; i<total_virtual_mask; i++) {
        virtual_oidlist->list[i] = Nlm_SwapUint4(virtual_oidlist->list[i]);
      }
    }
    search->rdfp->oidlist = virtual_oidlist;
    readdb_get_totals_ex(search->rdfp, &(dblen), &(search->dbseq_num), TRUE);
  }
#if 0
/* Intended for use when a list of gi's is sent in, but the real size is needed. */
/* It's probably still necessary to call BlastAdjustDbNumbers, but it would be nice
	 if this were not required. */
    if (options->use_real_db_size)
			readdb_get_totals(search->rdfp, &(dblen), &(search->dbseq_num));
/* use length and num of seqs of the database from alias file */
		if (search->rdfp->aliaslen && !gi_list) dblen = search->rdfp->aliaslen;
		if (search->rdfp->aliasnseq && !gi_list) search->dbseq_num = search->rdfp->aliasnseq;
/* command-line/options trump alias file. */
		if (options->db_length > 0)	dblen = options->db_length;
		if (options->dbseq_num > 0) search->dbseq_num = options->dbseq_num;
#endif
		if (options->searchsp_eff > 0) searchsp_eff = options->searchsp_eff;
    if (StringCmp(prog_name, "tblastn") == 0 || StringCmp(prog_name, "tblastx") == 0) {
      dblen /= 3.0;
      searchsp_eff /= 3.0;
    }
		search->dblen = dblen;
		search->searchsp_eff = searchsp_eff;
		status = CddSetUpSearchInternalByLoc (search, query_slp, query_bsp, prog_name, qlen, options, callback);
		if (status != 0) {
	  	ErrPostEx(SEV_WARNING, 0, 0, "SetUpBlastSearch failed.");
			search->query_invalid = TRUE;
		}
	}
	if (options_alloc) options = BLASTOptionDelete(options);
	return search;
}

#define  putCkptNlm_FloatHi(d, ckptFile)  (putCkptNumber(&(d),sizeof(Nlm_FloatHi),ckptFile))
#define  putCkptInt4(i, ckptFile)         (putCkptNumber(&(i),sizeof(Int4),ckptFile))
#define  putCkptChar(c, ckptFile)         (putCkptNumber(&(c),sizeof(Char),ckptFile))

/*---------------------------------------------------------------------------*/
/* General routine for putting the internal representation of a number. */
/*---------------------------------------------------------------------------*/
static void LIBCALL putCkptNumber(void * numberPtr, Int4 numberSize, FILE * ckptFile)
{
  FileWrite(numberPtr,numberSize,1,ckptFile) ;
}

/*---------------------------------------------------------------------------*/
/*Code to put a vector of frequencies; put only the interesting entries*/
/*---------------------------------------------------------------------------*/
static void LIBCALL putFreqVector(Nlm_FloatHi * theVector, Int4 length, FILE * ckptFile)
{
   int  vectorRef;
   Int4 charOrder[EFFECTIVE_ALPHABET]; /*standard order of letters according to S. Altschul*/


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

   for(vectorRef = 0; vectorRef < EFFECTIVE_ALPHABET; vectorRef++)
     putCkptNlm_FloatHi(theVector[charOrder[vectorRef]],ckptFile);
}

/*---------------------------------------------------------------------------*/
/* Code to put a matrix, vector-by-vector. */
/*---------------------------------------------------------------------------*/
static void LIBCALL putCkptFreqMatrix (Nlm_FloatHi **theMatrix, Int4 length, Int4 width, FILE * ckptFile)
{
  int  matrixRef;  /*loop index*/
 
  for (matrixRef = 0; matrixRef < length ; matrixRef++ )
    putFreqVector(theMatrix[matrixRef], width, ckptFile);
}


/*---------------------------------------------------------------------------*/
/*Take a checkpoint at the end of the current PSI-BLAST round, stores
query length, query, and position-specific target frequencies.
Returns TRUE if checkpoint was sucessful and FALSE otherwise. */
/*---------------------------------------------------------------------------*/
Boolean LIBCALL CddposTakeCheckpoint(posSearchItems * posSearch, compactSearchItems * compactSearch, CharPtr fileName, ValNodePtr *error_return)
{
  FILE * checkFile; /*file in which to take the checkpoint*/
  Int4 length; /*length of query sequence, and an index for it*/
  Int4 i; /*indices to position and alphabet */
  Char localChar; /*temporary character*/

  checkFile = FileOpen(fileName, "w");
  if (NULL == checkFile) {
    BlastConstructErrorMessage("CddposTakeCheckpoint", "Could not open checkpoint file", 1, error_return);
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

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* modelled after posReadAlignment (posit.c, as of 12/28/2000)               */
/* instead of reading in a text alignment from a file, the posDesc array is  */
/* populated from the DenseDiag alignment                                    */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static posDesc** CddposReadAlignment(compactSearchItems *compactSearch,
                                     SeqAlignPtr salp, Int4 numSeqs,
				     Int4 alignLength, BioseqPtr bsp)
{
  posDesc    **returnArray;                    /*array of sequences to return*/
  SeqAlignPtr  salpThis;
  DenseDiagPtr ddp;
  SeqPortPtr   spp;
  Int4         i,j,c;                                          /*loop indices*/
  Int4         alignPos, queryOffset, subjectOffset, matchLength;
  Int4         index, splace, qplace, retrievalOffset, subjectLength;
  Uint1Ptr     s;

  returnArray = (posDesc**) MemNew(numSeqs * sizeof(posDesc *));
  if (NULL == returnArray) exit(EXIT_FAILURE);
  for (i = 0; i < numSeqs; i++) {
    returnArray[i] = (posDesc *) MemNew(alignLength * sizeof(posDesc));
    if (NULL == returnArray[i]) exit(EXIT_FAILURE);
    for(j = 0; j < alignLength; j++) {
      returnArray[i][j].letter = GAP_CHAR;
      returnArray[i][j].used = TRUE;
    }
  }  

  alignPos = 0; i = 0;
  ASSERT(NULL != bsp);
  spp = SeqPortNew(bsp,0,bsp->length-1,Seq_strand_unknown,Seq_code_ncbistdaa);
  for (index=0; index<alignLength; index++) {
    returnArray[i][index].letter = SeqPortGetResidue(spp);
    returnArray[i][index].used   = TRUE;
  }
  spp = SeqPortFree(spp);
  salpThis = salp;

  while (salpThis) {
    i++;
    ddp = (DenseDiagPtr) salpThis->segs;
    s = CddGetSequenceWithDenseDiag(ddp,FALSE, &retrievalOffset, &subjectLength);
    while (ddp) {
      queryOffset = ddp->starts[0];
      subjectOffset = ddp->starts[1] - retrievalOffset;
      matchLength = ddp->len;
      for(c = 0, qplace = queryOffset, splace = subjectOffset; c < matchLength; c++, qplace++, splace++) {
        ASSERT(qplace < alignLength);
        ASSERT(i < numSeqs);
        returnArray[i][qplace].letter = (Int1) s[splace]; 
        returnArray[i][qplace].used = TRUE; 
      }
      ddp = ddp->next;
    }
    s = MemFree(s);
    salpThis = salpThis->next;
  }
  return(returnArray);
}



/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* modelled after posProcessAlignment (posit.c, as of 12/28/2000)            */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddposProcessAlignment(posSearchItems *posSearch, 
                                    compactSearchItems *compactSearch,
				    SeqAlignPtr salp, Int4 numSeqs,
				    Int4 alignLength, BioseqPtr bsp)
{
  Int4 queryPos, alignPos, linePos; /*placeholder for pos. in query & alignmt*/
  Int4 *queryDesc;      /*position correspondence between alignment and query*/
  Int4 seqIndex;                                      /*counter for sequences*/
  posDesc ** alignArray;
  Int4 queryLength;                                /*length of query sequence*/

  alignArray = CddposReadAlignment(compactSearch,salp,numSeqs,alignLength,bsp);
  queryDesc = (Int4 *) MemNew(alignLength * sizeof(Int4));
  if (NULL == queryDesc) exit(EXIT_FAILURE);
  for(alignPos = 0; alignPos < alignLength; alignPos++) queryDesc[alignPos] = GAP_HERE;
  alignPos = 0;
  queryPos = 0;
  for(linePos = 0; linePos < alignLength; linePos++) {
    queryDesc[alignPos] = queryPos;
    posSearch->posDescMatrix[0][queryPos].letter = alignArray[0][linePos].letter;
    posSearch->posDescMatrix[0][queryPos].used = TRUE;
    posSearch->posDescMatrix[0][queryPos].e_value = compactSearch->ethresh/2;
    posSearch->posDescMatrix[0][queryPos].leftExtent = 0;
    posSearch->posDescMatrix[0][queryPos].rightExtent = compactSearch->qlength - 1;
    posSearch->posC[queryPos][alignArray[0][linePos].letter]++;
    posSearch->posCount[queryPos]++;
    queryPos++;
    alignPos++;
  }


  queryLength = queryPos;
  for(seqIndex = 1; seqIndex < numSeqs; seqIndex++) {
    for(linePos = 0; linePos < alignLength; linePos++) {
      if (!(posSearch->posDescMatrix[0][queryDesc[linePos]].used)) {
        /*mark column as not participating*/
        posSearch->posDescMatrix[seqIndex][queryDesc[linePos]].used = FALSE;
      }
      else {
        posSearch->posDescMatrix[seqIndex][queryDesc[linePos]].letter = alignArray[seqIndex][linePos].letter;
        posSearch->posDescMatrix[seqIndex][queryDesc[linePos]].used = TRUE;
        posSearch->posDescMatrix[seqIndex][queryDesc[linePos]].e_value = compactSearch->ethresh/2;
        posSearch->posDescMatrix[seqIndex][queryDesc[linePos]].leftExtent = 0;
        posSearch->posDescMatrix[seqIndex][queryDesc[linePos]].rightExtent = compactSearch->qlength;
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

/* clean up */
  for (seqIndex=0;seqIndex<numSeqs;seqIndex++) {
    MemFree(alignArray[seqIndex]);
  }
  MemFree(alignArray);
  MemFree(queryDesc);
}

