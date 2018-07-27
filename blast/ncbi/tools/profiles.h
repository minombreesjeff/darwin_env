/* $Id: profiles.h,v 6.15 2004/09/21 14:45:26 camacho Exp $
***************************************************************************
*                                                                         *
*                             COPYRIGHT NOTICE                            *
*                                                                         *
* This software/database is categorized as "United States Government      *
* Work" under the terms of the United States Copyright Act.  It was       *
* produced as part of the author's official duties as a Government        *
* employee and thus can not be copyrighted.  This software/database is    *
* freely available to the public for use without a copyright notice.      *
* Restrictions can not be placed on its present or future use.            *
*                                                                         *
* Although all reasonable efforts have been taken to ensure the accuracy  *
* and reliability of the software and data, the National Library of       *
* Medicine (NLM) and the U.S. Government do not and can not warrant the   *
* performance or results that may be obtained by using this software,     *
* data, or derivative works thereof.  The NLM and the U.S. Government     *
* disclaim any and all warranties, expressed or implied, as to the        *
* performance, merchantability or fitness for any particular purpose or   *
* use.                                                                    *
*                                                                         *
* In any work or product derived from this material, proper attribution   *
* of the author(s) as the source of the software or data would be         *
* appreciated.                                                            *
*                                                                         *
**************************************************************************/

/**************************************************************************
File name: profiles.h

Author: Alejandro Schaffer

Contents: utilities for IMPALA

**************************************************************************/

#ifndef __PROFILES_H
#define __PROFILES_H

#define MAXLINELEN 2000
#define MAX_NAME_LENGTH 500

#define   ARG_DB 0
#define   ARG_QUERY_FILE 1
#define   ARG_E_VALUE 2
#define   ARG_ALIGN_VIEW 3
#define   ARG_OUTPUT_FILE 4
#define   ARG_FILTER 5
#define   ARG_GAP_OPEN 6
#define   ARG_GAP_EXTEND 7
#define   ARG_X_DROP  8
#define   ARG_PROCESSORS 9
#define   ARG_SHOW_GI 10
#define   ARG_E_MULTIPASS 11
#define   ARG_PSEUDO 12
#define   ARG_NUM_PASS 13
#define   ARG_BELIEVE_DEF 14
#define   ARG_SEQALIGN_FILE 15
#define   ARG_MATRIX 16
#define   ARG_MATRICES_DB 17
#define   ARG_DB_LENGTH  18
#define   ARG_NUM_DEFLINES 20
#define   ARG_NUM_ALIGNS 21


#define PRO_VERSION "1.1"
#define PRO_DATE "20-December-1999"
#define PRO_MAX_HIT_LIST 250
#define PRO_NUM_TICKS 50

#define PRO_ALPHABET_SIZE  26
#define SORT_THRESHOLD 20

#define PRO_DEFAULT_SCALING_UP  100
#define PRO_DEFAULT_SCALING_DOWN 0.01

#define RPS_MAGIC_NUMBER 7702

#define Xchar 21
#define Xscore (-1)

#define scoreRange 10000

/*factor used to multiply the gapped K parameter to make it
  more accurate in most cases*/
#define PRO_K_MULTIPLIER 1.2

typedef BLAST_Score ScoreRow[PRO_ALPHABET_SIZE];

typedef struct SWpairs {
  Int4 noGap;
  Int4 gapExists;
} SWpairs;

typedef struct proDemographicsItems {
  Uint1 matrixName[MAX_NAME_LENGTH];
  Int4  numSequencesTested;
  Int4  numBetterThanEthresh;
  Int4  numCallsALIGN;
  Int4  queryLength;
  Int4  dbLength;
  Nlm_FloatHi effDbLength;
  Nlm_FloatHi effSearchSpace;
  Nlm_FloatHi X;
  Nlm_FloatHi XinBits;
} proDemographicsItems;

#ifdef __cplusplus
extern "C" {
#endif

Char * LIBCALL addSuffixToName PROTO((Char *prefix, Char *suffix));


Char LIBCALL getRes PROTO((Char input));

Boolean LIBCALL IMPALAPrintReference PROTO((Boolean html, Int4 line_length, FILE *outfp));

Nlm_FloatHi LIBCALL
impalaKarlinLambdaNR PROTO((BLAST_ScoreFreqPtr sfp, Nlm_FloatHi initialLambda));

Boolean LIBCALL impalaScaling PROTO((posSearchItems *posSearch, compactSearchItems * compactSearch, Nlm_FloatHi scalingFactor, Boolean doBinarySearch));

Boolean LIBCALL  impalaReadCheckpoint PROTO((posSearchItems * posSearch, compactSearchItems * compactSearch, CharPtr fileName, ValNodePtr * error_return,
Nlm_FloatHi scalingFactor));

void  LIBCALL impalaMakeFileNames PROTO((Char * matrixDbName, Char * auxiliaryFileName, Char * mmapFileName, Char * seqFileName, Char *matrixFileName, Char * ckptFileName,  Char *directoryPrefix));

Boolean LIBCALL IMPALAPrintHelp PROTO((Boolean html, Int4 line_length, Char * programName, FILE *outfp));

Nlm_FloatHi LIBCALL IMPALAfindUngappedLambda PROTO((Char *matrixName));

void LIBCALL IMPALAfillResidueProbability PROTO((Uint1Ptr sequence, Int4 length, Nlm_FloatHi * resProb));

BLAST_ScoreFreqPtr LIBCALL IMPALAfillSfp PROTO((BLAST_Score **matrix, Int4 matrixLength, Nlm_FloatHi *queryProbArray, Nlm_FloatHi *scoreArray,  BLAST_ScoreFreqPtr return_sfp, Int4 range));

ScorePtr LIBCALL addScoresToSeqAlign PROTO((Int4 rawScore, Nlm_FloatHi eValue, Nlm_FloatHi Lambda, Nlm_FloatHi logK));

void LIBCALL pro_quicksort_hits PROTO((Int4 no_of_seq, SWResults **proResultsList));

#ifdef __cplusplus
}
#endif

#endif  /* __PROFILES_H */
