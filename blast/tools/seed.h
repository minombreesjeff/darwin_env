/* $Id: seed.h,v 6.16 2000/08/01 17:21:13 shavirin Exp $
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
 
File name: seed.h
Author: Alejandro Schaffer
 
Contents: header file for PHI-BLAST and pseed3.

$Revision: 6.16 $

$Log: seed.h,v $
Revision 6.16  2000/08/01 17:21:13  shavirin
Added protection for using C++ compiler.

Revision 6.15  1999/10/18 19:54:43  shavirin
Removed unused definition.

Revision 6.14  1999/10/05 19:36:54  shavirin
Changed to use functions from blast.c: BlastGetDbChunk and BlastTickProc.
Removed unused functions.

Revision 6.13  1999/09/22 17:50:18  shavirin
Now functions will collect messages in ValNodePtr before printing out.
 
 
*****************************************************************************/

#if !defined(SEED__H)
#define SEED__H


#ifdef __cplusplus
extern "C" {
#endif

#define ALPHABET_SIZE 25
#define DNA_ALPHABET_SIZE 4
#define MAX_HIT 2000
#define PATTERN_SPACE_SIZE 1000
#define BUF_SIZE 100
#define PATTERN_BUF_SIZE 2000
#define PATTERN_NAME_SIZE 200
#define SeqIdBufferSize   64 /*buffer size for SeqIdWrite*/
#define BITS_PACKED_PER_WORD 30
#define OVERFLOW1  (1 << BITS_PACKED_PER_WORD)
#define allone  ((1 << ALPHABET_SIZE) - 1)

#define SEED_INFINITY 1000000 /*large score for array sentinel*/

#define MAX_EVALUE 1000 /*maximum e-value allowed as threshold*/


/*The following 3 flags define 3 options for running the program*/
#define SEED_FLAG 1
#define PATTERN_FLAG 2
#define PAT_SEED_FLAG 3
#define PAT_MATCH_FLAG 4

#define MaxW   11
#define MaxP   (BITS_PACKED_PER_WORD * MaxW) /*threshold pattern length*/
#ifdef WIN16
#define MAX_WORDS_IN_PATTERN 16
#else
#define MAX_WORDS_IN_PATTERN 100
#endif
#define ASCII_SIZE 256

#define DEFAULT_PARAM_C 0.6
#define DEFAULT_PARAM_LAMBDA 0.270

#define ONE_WORD_PATTERN  0
#define MULTI_WORD_PATTERN 1
#define PATTERN_TOO_LONG  2

#define PAT_PROB_THRESH  0.002  /*upper threshold for pattern probabilities*/
#define EXPECT_MATCH_THRESH 20000 /*upper threshold for number of occurrences*/
#define WILDCARD_THRESH  30 /*threshold for product of variable-length wildcards*/

/*band amounts for banded alignment*/
#define BAND_LOW (-5)
#define BAND_HIGH 5

/*Limit on length of DNA sequence*/
#define MAXDNA 200000

/*   The following integer codes used for trace back in align.
     Each node must implcitly store three pointers to decide where a CC DD, or
     e value comes from. For example the  CC value can come from a sub del or
     ins edge. A DD can come from extension of a gap or an initiation of
     and new gap.  So th three flags are needed.
     One flag have three states, the other 2 has 2 states.
     The flags are packed into one integer. Let the flags be s1, s2, s3.
     The integer state is then s3*20+s2*10+s1. where s1={0,1,2} s2=s3={0,1}. 
     There numbers above are from this packing. */

#define DELETE_CODE 20
#define INSERT_CODE 10
#define DIAGONAL_DELETE 2
#define DIAGONAL_INSERT 1

/* #define BLAST_DB_CHUNK_SIZE 500
   #define BLAST_NTICKS 50 */

typedef struct hit_str {
    Int4 score;
    Int4 l_score;
    Nlm_FloatHi mul; /*multiplier for scores of characters*/
    Int4 hit_pos, hit_end;
    Int4 bi, bj, ei, ej; /*beginning and end of pattern occurence
                           in query sequence
                           and database sequence, respectively*/
    struct hit_str *next;  /*next hit in linked list*/
} *hit_ptr, hit_node;

typedef struct store_str {
    Int4 l_score;
    Uint1Ptr seq;
    Char *header;
    Int4 seqno;
    hit_ptr hit_list;
    struct store_str *next;
} store_node, *store_ptr;

typedef struct qseq {
     Uint1Ptr lseq, rseq, sseq;
     Int4 llen, rlen, slen;
} query_seq, *qseq_ptr;

typedef struct seedSearchItems {

    Nlm_FloatHi  charMultiple[ALPHABET_SIZE];
    Nlm_FloatHi  paramC; /*used in e-value computation*/
    Nlm_FloatHi  paramLambda; /*used in e-value computation*/
    Nlm_FloatHi  paramK; /*used in the bit score computation*/
    Int4         cutoffScore; /*lower bound for what is a hit*/
    Nlm_FloatHi  standardProb[ALPHABET_SIZE]; /*probability of each letter*/
    Char         order[ASCII_SIZE];
    Char         pchars[ALPHABET_SIZE+1];
    Char         name_space[BUF_SIZE];  /*name of a pattern*/
    Char         pat_space[PATTERN_SPACE_SIZE];  /*string description
                                                   of pattern*/
} seedSearchItems;

typedef struct seedResultItems {

    store_ptr    listOfMatchingSequences;
} seedResultItems;

typedef struct patternSearchItems {
  
   Int4   numWords;  /*number of words need to hold bit representation
                       of pattern*/
   Int4   match_mask;   /*bit mask representation of input pattern
                          for patterns that fit in a word*/
   Int4   match_maskL[BUF_SIZE]; /*bit mask representation of input pattern
                                   for long patterns*/
  /*which positions can a character occur in for long patterns*/
   Int4   bitPatternByLetter[ASCII_SIZE][MaxW]; 
   Int4   *whichPositionPtr; /*used to pass a piece a row of the arrays*/
   Uint4   *DNAwhichPrefixPosPtr, *DNAwhichSuffixPosPtr; /*similar for DNA patterns*/
  /*which positions can a character occur in for short patterns*/
   Int4   whichPositionsByCharacter[ASCII_SIZE];
   Uint4   DNAwhichPrefixPositions[ASCII_SIZE]; /*for DNA sequence where
                             prefix of DNA 4-mer matches pattern*/
   Uint4   DNAwhichSuffixPositions[ASCII_SIZE]; /*similar to above for suffixes*/
    /*for each letter in the alphabet and each word in the masked
      pattern representation, holds a bit pattern saying for which
      positions the letter will match*/
   Int4   SLL[MAX_WORDS_IN_PATTERN][ASCII_SIZE];  /*similar to
                  whichPositionsByCharacter for many-word patterns*/
   Uint4   DNAprefixSLL[MAX_WORDS_IN_PATTERN][ASCII_SIZE];
  /*similar to DNAwhichPrefixPositions for many word patterns*/
   Uint4   DNAsuffixSLL[MAX_WORDS_IN_PATTERN][ASCII_SIZE];
  /*similar to DNAwhichSuffixPositions for many word patterns*/
   Char   flagPatternLength; /*indicates if pattern fits in 1 word,
                               some words, or is too long*/
   Nlm_FloatHi  patternProbability;  /*probability of this letter
                                        combination*/
   Int4   whichMostSpecific; /*which word in an extra long pattern
                               has the lowest probability of a match*/
   Int4   numPlacesInWord[MAX_WORDS_IN_PATTERN]; /*when pattern has more than 7
             words keep track of how many places of pattern in each word of 
             the  representation; was called lening */
   Int4   spacing[MAX_WORDS_IN_PATTERN]; /*spaces until next word due to
                                          wildcard*/
   Int4   inputPatternMasked[MaxP];
   Int4   highestPlace; /*number of places in pattern representation
                          as computed in input_pattern; was called num*/
  Int4   minPatternMatchLength; /*minimum length of string to match this pattern*/
  Int4   wildcardProduct; /*product of wildcard lengths*/
} patternSearchItems;

typedef struct alignSearchItems {
   Int4** matrix;   /*score matrix*/
   Int4   gapOpen;  /*penalty to open a gap*/
   Int4   gapExtend; /*penalty to extend a gap one position*/
   Int4   gapCost;   /*gapOpen + gapExtend*/
} alignSearchItems;

#if 0
typedef struct threadInfoItems {
     Int4 global_gi_current;
     Boolean global_gi_being_used;
     TNlmMutex db_mutex;  /*lock for access to database*/
     TNlmMutex results_mutex; /*lock for storing results */
     TNlmMutex callback_mutex; /*lock for issuing update ticks on the screen*/
     SeqIdPtr global_seqid_ptr;
     SeqIdPtr  global_seqid_list;
     Int4 db_chunk_size;
     Int4 db_chunk_last;
     Int4 final_db_seq;  /*index of last sequence to process*/
     Int4 number_seqs_done;  /*number of sequences already tested*/
     Int4 db_incr;  /*size of a database chunk to get*/
     BlastGiListPtr blast_gi_list;
/*
	Callback functions to indicate progress, or lack thereof.
*/
     int (LIBCALLBACK *tick_callback)PROTO((Int4 done, Int4 positives));
} threadInfoItems; 
#endif

typedef struct seedParallelItems {
    ReadDBFILEPtr rdpt; /*pointer to database*/
    qseq_ptr query_seq; /*multi-piece representation of query sequence*/
    Int4 lenPatMatch;  /*number of characters in the pattern occurrence*/
    GapAlignBlkPtr gap_align; /*structure for description of the gapped
                                alignment*/
    Boolean is_dna;  /*is this DNA or protein data*/
    patternSearchItems * patternSearch; /*holds items about the pattern*/
    seedResultItems * seedResults; /*holds the results for this thread*/
     seedSearchItems * seedSearch; /*holds preprocessing info about the
                                     search*/
    Int4 totalOccurrences;  /*total number of pattern occurrences 
                              found in this thread*/
    Int4 matchIndex; /* total number of matches with reportable score in
                        this thread; match ===> occurrenece, but
                        occurrence !===> match */
    /* threadInfoItems *threadInfo; */
    BlastThrInfoPtr thr_info;
} seedParallelItems;


void PGPOutTextMessages(ValNodePtr info_vnp, FILE *fd);

Char * LIBCALL strsave PROTO((Char *s));
ValNodePtr LIBCALL  seedEngineCore PROTO((BlastSearchBlkPtr search, 
  BLAST_OptionsBlkPtr options, Uint1Ptr query, Uint1Ptr unfilter_query,
  CharPtr database, CharPtr patfile, Int4 program_flag,  FILE * patfp, 
  Boolean is_dna, Boolean reverseDb, seedSearchItems *seedSearch,
   Nlm_FloatHi posEThresh, Nlm_FloatLo searchSpEff,
   posSearchItems * posSearch, SeqLocPtr *seed_seq_loc, Boolean showDiagnostics, ValNodePtr PNTR info_vnp));
void LIBCALL init_order PROTO((Int4 **matrix, Int4 program_flag, Boolean is_dna,
   seedSearchItems *seedSearch));

Int4 LIBCALL convertProgramToFlag PROTO((Char * program, Boolean * is_dna));

void LIBCALL initProbs PROTO((seedSearchItems * seedSearch));

Int4 LIBCALL find_hits PROTO((Int4 *hitArray, Uint1Ptr seq, Int4 len, Boolean is_dna,  patternSearchItems * patternSearch));

Int4 LIBCALL init_pattern PROTO((Uint1 *pattern, Boolean is_dna, patternSearchItems * patternSearch,  seedSearchItems *seedSearch, ValNodePtr * error_return));


Int4 LIBCALL align_of_pattern PROTO((Uint1 *querySeq, Uint1 *dbSeq, Int4 lenQuerySeq,  Int4 lenDbSeq, Int4 *alignScript,  Int4 **tback,  GapAlignBlkPtr gap_align, Int4 *useful_score,  Nlm_FloatHi *multiple, patternSearchItems *patternSearch, seedSearchItems * seedSearch));

void LIBCALL pat_output PROTO((Uint1 *seq, Int4 begin, Int4 end, patternSearchItems *patternSearch, ValNodePtr PNTR info_vnp));

qseq_ptr LIBCALL split_target_seq PROTO((Uint1 *seq, Int4 seed, Int4 len_pat, Int4 len_query));

hit_ptr LIBCALL get_hits PROTO((qseq_ptr qp, Int4 len_of_pat, 
		 Uint1Ptr seq_db, Int4 len_seq_db, GapAlignBlkPtr gap_align, 
		 Boolean is_dna, patternSearchItems * patternSearch,
                 seedSearchItems * seedSearch, Int4 * newOccurrences));
void LIBCALL search_pat PROTO((ReadDBFILEPtr rdpt, Char *patternFileName, Boolean is_dna, seedSearchItems *seedSearch, patternSearchItems *patternSearch, ValNodePtr * error_return, ValNodePtr PNTR info_vnp));

SeqAlignPtr LIBCALL output_hits PROTO((ReadDBFILEPtr rdpt,
	    Boolean score_only, Uint1 *seq1, qseq_ptr qp, 
	    Int4 len, Nlm_FloatHi dbLength, GapAlignBlkPtr gap_align, Boolean is_dna,
            Int4 effectiveOccurrences, seedSearchItems *seedSearch, seedResultItems *seedResults, 
            patternSearchItems * patternSearch, Boolean reverse, 
            Int4 numOccurrences, Nlm_FloatHi eThresh,
            SeqIdPtr query_id, Nlm_FloatHi posEthresh, 
            posSearchItems *posSearch, Int4 numMatches, 
            Int4 *totalBelowEThresh, Boolean showDiagnostics,
            ValNodePtr PNTR info_vnp));

Char*  LIBCALL get_a_pat PROTO((FILE *fp, Char **name, Int4Ptr hitArray, Int4Ptr fullHitArray, 
   Int4 * numPatOccur, Int4 *numEffectiveOccurrences, Int4 program_flag, 
   Uint1Ptr unfilter_seq, Uint1Ptr seq, Int4 len, Boolean is_dna,
   patternSearchItems *patternSearch, seedSearchItems * seedSearch,
   Boolean showDiagnostics, ValNodePtr * error_return, 
   ValNodePtr PNTR info_vnp));


void LIBCALL quicksort_hits PROTO((Int4 no_of_seq, seedResultItems *seedResults));

Int4 LIBCALL eValueFit PROTO((Nlm_FloatHi eThresh, Nlm_FloatHi dbLength, 
        seedSearchItems *seedSearch, Int4 numOccurrences, 
        Nlm_FloatHi patternProbability));

void LIBCALL storeOneMatch PROTO((hit_ptr hit_list, Int4 seqno, Uint1Ptr seq, 
	      seedResultItems *seedResults));

void LIBCALL seed_free_all PROTO((seedResultItems *seedResults));

ValNodePtr  LIBCALL SeedPruneHitsFromSeedReturn PROTO((ValNodePtr seedReturn, Int4 number_of_descriptions));


#ifdef __cplusplus
}
#endif

#endif /*define SEED__H*/
