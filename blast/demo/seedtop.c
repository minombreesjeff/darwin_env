/* $Id: seedtop.c,v 6.6 1999/09/22 17:54:21 shavirin Exp $ */
/**************************************************************************
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
/*****************************************************************************
 
File name: seedtop.c
 
Authors: Zheng Zhang and Alejandro Schaffer
Maintainer: Alejandro Schaffer
 
Contents: main routine for pseed3, stand-alone counterpart to PHI-BLAST.
 
$Revision: 6.6 $

$Log: seedtop.c,v $
Revision 6.6  1999/09/22 17:54:21  shavirin
Now functions will collect messages in ValNodePtr before printing out.
 
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
#include <gapxdrop.h>
#include <posit.h>
#include <readdb.h>
#include <ncbithr.h>
#include <seed.h> 

#ifdef __cplusplus
extern "C" {
#endif


#define NUMARG 21

static Args myargs [NUMARG] = {
  { "Database", 
	"nr", NULL, NULL, FALSE, 'd', ARG_STRING, 0.0, 0, NULL},
  { "Query File", 
	"stdin", NULL, NULL, FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  { "Hit File", 
	"hit_file", NULL, NULL, FALSE, 'k', ARG_FILE_IN, 0.0, 0, NULL},
  { "Output File for Alignment", 
	"stdout", NULL, NULL, TRUE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
  { "Cost to open a gap",
	"11", NULL, NULL, FALSE, 'G', ARG_INT, 0.0, 0, NULL},	
  { "Cost to extend a gap",
	"1", NULL, NULL, FALSE, 'E', ARG_INT, 0.0, 0, NULL},
  { "Cost decline to align",
	"99999", NULL, NULL, FALSE, 'D', ARG_INT, 0.0, 0, NULL},
  { "X dropoff value for gapped alignment (in bits)",
	"15", NULL, NULL, FALSE, 'X', ARG_INT, 0.0, 0, NULL},	
  { "Cutoff cost",
        "30", NULL, NULL, FALSE, 'S', ARG_INT, 0.0, 0, NULL},
  { "Score only or not",
	"1", NULL, NULL, FALSE, 'C', ARG_INT, 0.0, 0, NULL},       
  { "Show GI's in deflines", 
        "F", NULL, NULL, FALSE, 'I', ARG_BOOLEAN, 0.0, 0, NULL},
  { "Expectation value(E)",
        "10.0", NULL, NULL, FALSE, 'e', ARG_FLOAT, 0.0, 0, NULL},
  { "Believe the query defline", 
        "F", NULL, NULL, FALSE, 'J', ARG_BOOLEAN, 0.0, 0, NULL},
  { "SeqAlign file ('Believe the query defline' must be TRUE)",
	NULL, NULL, NULL, TRUE, 'O', ARG_FILE_OUT, 0.0, 0, NULL},
  { "Matrix", 
	"BLOSUM62", NULL, NULL, FALSE, 'M', ARG_STRING, 0.0, 0, NULL},
  { "Number of one-line descriptions (V)",
        "500", NULL, NULL, FALSE, 'v', ARG_INT, 0.0, 0, NULL},
  { "Number of alignments to show (B)",
        "250", NULL, NULL, FALSE, 'b', ARG_INT, 0.0, 0, NULL}, 
  { "Program Name", 
	"patmatchp", NULL, NULL, FALSE, 'p', ARG_STRING, 0.0, 0, NULL},
  { "Cost for a match",
	"10", NULL, NULL, FALSE, 'r', ARG_INT, 0.0, 0, NULL},
  { "Cost for a mismatch",
	"-10", NULL, NULL, FALSE, 'q', ARG_INT, 0.0, 0, NULL},
  { "Filter query sequence with SEG",
	"F", NULL, NULL, FALSE, 'F', ARG_BOOLEAN, 0.0, 0, NULL}     
};


Int2 Main(void)
{
        BioseqPtr query_bsp;  /*structure to hold query information*/
        SeqEntryPtr sep;      /*structure to hold query retrieval result*/
        Int4  queryLength;  /*length of query sequence*/
        Int4  queryPos;    /*index over query sequence*/
        Int4  i, j;  /*indices over DNA characters*/
        Nlm_FloatHi  dbLength, adjustdbLength;  /*total number of characters in database*/
	Uint1Ptr query =NULL;  /*query sequence read in*/
	Uint1Ptr unfilter_query =NULL;  /*needed if seg  will filter query*/
        SeqLocPtr seg_slp; /*pointer to structure for seg filtering*/
	Uint1Ptr seqFromDb; /*newly read sequence from database*/
        Uint1Ptr reverseSeqFromDb = NULL; /*reverse of seqFromDb*/
        Int4 lenSeqFromDb;  /* length of seqFromDb */
        Char  *pattern; /*string description of a pettern*/
        Char *pname; /*name of pattern*/
	CharPtr queryfile; /*name of file containing query string*/
        CharPtr  database; /*name of database to use*/
        CharPtr  patfile;  /*file describing patterns to match*/
        CharPtr  outputfile; /*file to which output should be written*/
        Int4 seed; /*position in sequence where pattern match starts*/
        Int4 lenPatMatch; /*number of positions taken by pattern match*/
        Int4  num_seq; /*number of sequences in database*/
        Int4  program_flag; /*which program is being called*/
	hit_ptr hit_list; /*list of matches to one database sequence*/
	Int4 list[MAX_HIT]; /*list of lengths and start positions where
                              pattern matches sequence*/
        Int4  *occurArray; /*places in query where pattern occurs*/
        Int4  *hitArray; /* beginning and end of pattern in query. */
        Int4  numPatOccur; /*number of pattern occurrences in query string*/
        Int4  effectiveOccurrences; /*number of occurrences not overlapping
                                      in more than half the pattern*/
        Int4  occurIndex;  /*index over pattern ocuurences*/
        Int4  twiceNumMatches; /*stores return value from find_hits*/
        Int4  matchIndex;  /*index for matches to a single sequence*/
        Int4 totalOccurrences = 0, newOccurrences; /*total occurrences of pattern in database*/
        Nlm_FloatHi  eThresh;  /*e-value threshold for hits*/
	ReadDBFILEPtr rdpt=NULL;  /*holds result of attempt to read database*/
 	GapAlignBlkPtr gap_align; /*structure to keep track of gapped
                                    alingment information*/
        BLAST_ScoreBlkPtr sbp; /*BLAST structure used to hold matrix*/
	FILE *infp, *patfp; /*file descriptors for query file and pattern file*/
        FILE *outfp;  /*file descriptor for output file*/
	Boolean is_dna; /*are we working with DNA or protein sequences*/
	qseq_ptr query_seq;  /*query sequence in record format*/
        seedSearchItems *seedSearch; /*holds parameters related to seed*/
        seedResultItems *seedResults = NULL; /*holds list of matching sequences*/
        patternSearchItems *patternSearch = NULL; /*holds parameters
                                                     related to pattern1.c*/
        BLAST_OptionsBlkPtr options; /*used as placeholder for fillCandLambda*/
        ValNodePtr error_returns=NULL; /*store error messages*/
        ValNodePtr info_vnp = NULL;    /* store information messages */

        seedSearch = (seedSearchItems *) ckalloc(sizeof(seedSearchItems));
        seedResults = (seedResultItems *) ckalloc(sizeof(seedResultItems));
        patternSearch = (patternSearchItems *) ckalloc(sizeof(patternSearchItems));
        if (! GetArgs ("pseed3", NUMARG, myargs))
        {
                return (1);
        }

        if (! SeqEntryLoad())
                return (1);

	database = myargs[0].strvalue;
	queryfile = myargs[1].strvalue;
	if ((infp = FileOpen(queryfile, "r")) == NULL)
	{
	  ErrPostEx(SEV_FATAL, 0, 0, "seed: Unable to open input file %s\n", queryfile);
	  return (1);
	}

	patfile = myargs[2].strvalue;
	if ((patfp = FileOpen(patfile, "r")) == NULL)
	{
	  ErrPostEx(SEV_FATAL, 0, 0, "seed: Unable to open pattern file %s\n", patfile);
	  return (1);
	}
	outputfile = myargs[3].strvalue;
        outfp = NULL;
        if (outputfile != NULL)
	{
	  if ((outfp = FileOpen(outputfile, "w")) == NULL)
	    {
	      ErrPostEx(SEV_FATAL, 0, 0, "seed: Unable to open output file %s\n", outputfile);
	      return (1);
	    }
	}
	is_dna = FALSE;
        program_flag = convertProgramToFlag(myargs[17].strvalue, &is_dna);

	gap_align = GapAlignBlkNew(1, 1);
	gap_align->gap_open = myargs[4].intvalue;
	gap_align->gap_extend = myargs[5].intvalue;
	gap_align->decline_align = myargs[6].intvalue;


        options = BLASTOptionNew("blastp", TRUE);
	BLASTOptionSetGapParams(options, myargs[14].strvalue, 0, 0);
	options->gap_open = myargs[4].intvalue;
	options->gap_extend = myargs[5].intvalue;
	fillCandLambda(seedSearch, myargs[14].strvalue, options);
	gap_align->x_parameter = myargs[7].intvalue*NCBIMATH_LN2/seedSearch->paramLambda;

	/* seedSearch->cutoffScore = myargs[8].intvalue; */
        eThresh = (Nlm_FloatHi) myargs[11].floatvalue;
        if (eThresh > MAX_EVALUE) {
          ErrPostEx(SEV_FATAL, 0, 0, "E-value threshold is too high\n");
          return 1;
        }

        if (!is_dna)
          initProbs(seedSearch);

	if (program_flag != PATTERN_FLAG) {
	  if (program_flag != PAT_MATCH_FLAG) {
	    sbp = BLAST_ScoreBlkNew(Seq_code_ncbistdaa, 1);
	    sbp->read_in_matrix = TRUE;
	    BlastScoreBlkMatFill(sbp, myargs[14].strvalue);
	    gap_align->matrix = sbp->matrix;

	    if (is_dna) {
	      for (i = 0; i < DNA_ALPHABET_SIZE; i++) 
		for (j = 0; j < DNA_ALPHABET_SIZE; j++) 
		  if (i==j)  /*characters match */
		    gap_align->matrix[i][j] =  myargs[18].intvalue;
		  else       /* characters mismatch*/
		    gap_align->matrix[i][j] =  myargs[19].intvalue;
	    }
	  }

	  sep = FastaToSeqEntryEx(infp, is_dna, NULL, (Boolean) myargs[12].intvalue);
	  if (sep != NULL)
	    {
	      query_bsp = NULL;
	      if (is_dna)
		{
		  SeqEntryExplore(sep, &query_bsp, FindNuc);
		}
	      else
		{
		  SeqEntryExplore(sep, &query_bsp, FindProt);
		}
	      if (query_bsp == NULL)
		{
		  ErrPostEx(SEV_FATAL, 0, 0, "Unable to obtain bioseq\n");
		  return 2;
		}
              query = BlastGetSequenceFromBioseq(query_bsp, &queryLength);
	      seg_slp = BlastBioseqFilter(query_bsp, options->filter_string);
	      if (seg_slp) {
		unfilter_query = MemNew((queryLength + 1) * sizeof(Uint1));
		for (i = 0; i < queryLength; i++)
		  unfilter_query[i] = query[i];
		BlastMaskTheResidues(query,queryLength,21,seg_slp,FALSE, 0);
	      }

	      init_order(gap_align->matrix, program_flag, is_dna, seedSearch);
              /*translation from ASCII to 0,1,2,3 alphabet*/
	      for (queryPos = 0; queryPos < queryLength; queryPos++) 
		query[queryPos] = seedSearch->order[query[queryPos]]; 
              if (unfilter_query)
		for (queryPos = 0; queryPos < queryLength; queryPos++) 
		  unfilter_query[queryPos] = seedSearch->order[unfilter_query[queryPos]]; 
	    }
	}
        else {
	  init_order(NULL, program_flag, is_dna, seedSearch);
	}
	rdpt = readdb_new(database, !is_dna);
	if (program_flag == PATTERN_FLAG) {
	    search_pat(rdpt, patfile, is_dna, seedSearch, patternSearch, &error_returns, &info_vnp);
            PGPOutTextMessages(info_vnp, outfp);
            info_vnp = ValNodeFreeData(info_vnp);

            BlastErrorPrint(error_returns);
	    rdpt = readdb_destruct(rdpt);
	    return(0);
	} 
	occurArray = (Int4 *) ckalloc(sizeof(Int4)*queryLength*2);
	hitArray = (Int4 *) MemNew(sizeof(Int4)*queryLength*2);

	dbLength = 0;
	num_seq = readdb_get_num_entries(rdpt);
	dbLength = (Nlm_FloatHi) readdb_get_dblen(rdpt);              
	/*correct the effective size of the database to take out
	  the number of positions at the end of each sequence where
	  pattern match cannot start*/
		 
	while (pattern = get_a_pat(patfp, &pname, occurArray, hitArray, &numPatOccur, 
                                   &effectiveOccurrences,
				   program_flag, unfilter_query, query, 
                                   queryLength, is_dna,
                                   patternSearch, seedSearch, TRUE, &error_returns, &info_vnp)) {

            PGPOutTextMessages(info_vnp, outfp);
            info_vnp = ValNodeFreeData(info_vnp);

            BlastErrorPrint(error_returns);
          if (patternSearch->patternProbability > PAT_PROB_THRESH &&
	      (patternSearch->patternProbability * dbLength > EXPECT_MATCH_THRESH)) {
             fprintf(outfp,"Pattern %s is too likely to occur in the database to be informative\n",pname);
          }
	  else {
            if (patternSearch->wildcardProduct > WILDCARD_THRESH) {
              fprintf(outfp, "Due to variable wildcards pattern %s is likely to occur too many times in a single sequence\n",pname);
            }
	    else {

	      adjustdbLength = dbLength - (num_seq * patternSearch->minPatternMatchLength);

	      for (occurIndex = 0; occurIndex < numPatOccur; occurIndex++) {
		seed = occurArray[occurIndex];
		totalOccurrences = 0;
		fprintf(outfp,"Name  %sPattern %s At position %d of query sequence\n",
		     pname, pattern, seed);
		if ((twiceNumMatches=find_hits(list, &query[seed-1], queryLength-seed+1, FALSE, patternSearch)) < 2 || 
		    list[1] != 0) {
		  fprintf(outfp,"twiceNumMatches=%d list[1]=%d\n", i, list[1]);
		  ErrPostEx(SEV_FATAL, 0, 0, "pattern does not match the query at the place\n");
		  return 1;
		}
		if (program_flag != PAT_MATCH_FLAG) {
		  lenPatMatch = list[0]+1;
	      
		  matchIndex = 0;
		  query_seq = split_target_seq(query, seed, lenPatMatch, queryLength);

		  fprintf(outfp, "effective database length=%.1e\n pattern probability=%.1e\nlengthXprobability=%.1e\n", (Nlm_FloatHi) adjustdbLength, 
			  patternSearch->patternProbability, 
			  patternSearch->patternProbability * adjustdbLength);
		  if (!is_dna) {
		    /*extra caution about what values are tolerated*/
		    seedSearch->cutoffScore = eValueFit(
							MIN(MAX_EVALUE, 10 * eThresh), adjustdbLength, 
							seedSearch, effectiveOccurrences, patternSearch->patternProbability);
		  }
		  else
		    seedSearch->cutoffScore = myargs[8].intvalue;

		  for (num_seq = 0; num_seq < rdpt->num_seqs; num_seq++) {	    	
		    lenSeqFromDb = readdb_get_sequence(rdpt, num_seq, &seqFromDb);
		    /*if (num_seq == 3530)
		      printf("\n Stop here");*/
		    hit_list = get_hits(query_seq, lenPatMatch, seqFromDb, 
					lenSeqFromDb, gap_align, is_dna, patternSearch, 
					seedSearch, &newOccurrences);
		    if (newOccurrences > 0)
		      totalOccurrences += newOccurrences;
		    if (hit_list) {
		      storeOneMatch(hit_list, num_seq, seqFromDb, seedResults); 
		      matchIndex++;
		    }
		  }
		  if (matchIndex > 0) 
		    quicksort_hits(matchIndex, seedResults);
		  fprintf(outfp,"\nNumber of occurrences of pattern in the database is %d\n", totalOccurrences);
		  /*Note: for stand-alone program, score only will be shown*/
		  output_hits(rdpt, TRUE, query, query_seq, 
			      lenPatMatch, adjustdbLength, gap_align, is_dna, 
			      effectiveOccurrences, seedSearch, seedResults,
			      patternSearch, FALSE, totalOccurrences, eThresh,
			      NULL, 0.0, NULL, matchIndex, NULL, TRUE, 
                              &info_vnp);

                  if(query_seq != NULL) {
                      MemFree(query_seq->lseq);
                      MemFree(query_seq);
                  }

                  PGPOutTextMessages(info_vnp, outfp);
                  info_vnp = ValNodeFreeData(info_vnp);

		  seed_free_all(seedResults);
		}
	      }
	    }
	  }
	}

        BLAST_ScoreBlkDestruct(sbp);
        GapAlignBlkDelete(gap_align);
        BLASTOptionDelete(options);
	rdpt = readdb_destruct(rdpt);
        MemFree(occurArray);
        MemFree(hitArray);
        MemFree(query);
        MemFree(unfilter_query);
        MemFree(seedSearch);
        MemFree(seedResults);
        MemFree(patternSearch);

	return(0);
}

#ifdef __cplusplus
}
#endif

