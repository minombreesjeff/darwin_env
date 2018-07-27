/* $Id: pseed3.c,v 6.36 2001/05/04 14:14:52 madden Exp $ */
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
 
File name: pseed3.c
 
Original Author: Zheng Zhang
Maintainer: Alejandro Schaffer
 
Contents: high-level routines for PHI-BLAST and pseed3

$Revision: 6.36 $

$Log: pseed3.c,v $
Revision 6.36  2001/05/04 14:14:52  madden
Fixes for multiple patterns in phi-blast

Revision 6.35  2000/08/07 20:45:10  madden
Proper casting of int to long for printf

Revision 6.34  2000/07/18 12:56:06  madden
changed init_order to avoid processing characters outside the 20 regular amino acids.

Revision 6.33  2000/07/12 23:07:31  kans
reverse_seq moved from pseed3.c to blastool.c, placed in blast.h header, called by gapxdrop.c

Revision 6.32  2000/06/07 15:54:36  shavirin
Removed bug prevented from searches of multiple patterns

Revision 6.31  2000/05/12 19:15:50  shavirin
Fixed memory leaks in do_the_seed_SEARCH() function.

Revision 6.30  2000/04/25 19:13:17  dondosha
Assign db_chunk_last to first_db_seq before start of search

Revision 6.29  2000/04/25 14:29:05  shavirin
Fixed problem with BlastGetDbChunk function in case of database subsets.

Revision 6.28  2000/02/11 20:00:20  shavirin
Fixes for seed search with list of gis.

Revision 6.27  1999/11/30 20:48:53  shavirin
Corrected the way that returnData->SeedSearch and
returnData->patternSearch are filled in seedParallelFill.

Revision 6.26  1999/11/30 18:23:53  shavirin
Added limit to number of patterns to be displayed.

Revision 6.25  1999/11/16 22:20:46  shavirin
Returned code missing on Revision 6.23

Revision 6.24  1999/11/16 21:33:47  shavirin
Fixed bug involved posSearch->posResultSequences structure.

Revision 6.23  1999/10/21 16:15:05  shavirin
Removed unused array and all references to array threshSequences

Revision 6.22  1999/10/19 20:06:37  shavirin
Added word LIBCALLBACK to the callback SeedSortHits().

Revision 6.21  1999/10/18 19:57:06  shavirin
Now sorting hits will use HeapSort function with additional sorting by
sequence number. Removed unused functions.

Revision 6.20  1999/10/13 16:24:16  shavirin
Reset counter thr_info->db_chunk_last in do_the_seed_search() function.

Revision 6.19  1999/10/13 15:42:40  shavirin
Reset counter thr_info->last_db_seq in do_the_seed_search() function.

Revision 6.18  1999/10/05 19:36:55  shavirin
Changed to use functions from blast.c: BlastGetDbChunk and BlastTickProc.
Removed unused functions.

Revision 6.17  1999/09/28 20:44:30  shavirin
Returned back readdb_attach() functions.

Revision 6.16  1999/09/28 13:43:48  shavirin
Changed function reentrant_get_gb_chunk() to use OID list. Only one
instance of rdfp now used in all threads of the search.

Revision 6.15  1999/09/23 20:39:03  shavirin
Fixed some memory leaks.

Revision 6.14  1999/09/22 17:51:58  shavirin
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

#if defined(OS_UNIX) && !defined(OS_UNIX_SUN) && !defined(OS_UNIX_LINUX)
#include <sys/resource.h>
#include <signal.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define SEED_NTICKS 50

static Int4 get_pat PROTO((FILE *fp, Char *stringForPattern, Char *pname));
static Uint1Ptr reverseSequence PROTO((Uint1Ptr seqFromDb, Int4 lenSeqFromDb));

/*creates duplicate data structure for each thread of parallel process*/
static seedParallelItems * seedParallelFill(ReadDBFILEPtr rdpt, qseq_ptr query_seq, Int4 lenPatMatch, Boolean is_dna, GapAlignBlkPtr gap_align, patternSearchItems * patternSearch, seedSearchItems * seedSearch, BlastThrInfoPtr thr_info);

static void do_the_seed_search PROTO((BlastSearchBlkPtr search, Int4 num_seq, qseq_ptr query_seq, Int4 lenPatMatch, Boolean is_dna, GapAlignBlkPtr gap_align, patternSearchItems * patternSearch, seedSearchItems * seedSearch, Int4 * matchIndex, Int4 * totalOccurrences, seedResultItems * seedResults));

static Boolean BlastSetLimits PROTO((Int4 cpu_limit, Int2 num_cpu));

void PGPOutTextMessages(ValNodePtr info_vnp, FILE *fd)
{
    ValNodePtr vnp;

    for(vnp = info_vnp; vnp != NULL; vnp = vnp->next) {
        fwrite(vnp->data.ptrvalue, 1, StringLen(vnp->data.ptrvalue), fd);
    }

    return;
}

/*seedReturn is a list of lists of SeqAligns in which each list is shortened
    to at most number_of_descriptions elements, the rest are deallocated
   and the list of shortened lists is returned*/
ValNodePtr  LIBCALL SeedPruneHitsFromSeedReturn(ValNodePtr seedReturn, Int4 number_of_descriptions)
{
    ValNodePtr returnList; /*list of SeqAlignPtrs to return*/
    ValNodePtr thisValNode; /*scan down input list of ValNodes*/
    SeqAlignPtr thisList; /*one list of seqAlignPtrs*/
    Int4 counter; /*counter for SeqAligns*/
    SeqAlignPtr prevSeqAlign = NULL;
    SeqAlignPtr currentSeqAlign = NULL , nextSeqAlign = NULL; 
    /*used to walk down list*/
    DenseSegPtr curSegs, testSegs; /*Used to extract ids from curSeqAlign, testSeqAlign*/
    SeqIdPtr curId, testId; /*Ids of target sequences in testSeqAlign*/
    
    curId = NULL;
    returnList = seedReturn;   
    if (number_of_descriptions > 1) {
        thisValNode = returnList;
        while (NULL != thisValNode) {
            thisList = thisValNode->data.ptrvalue;
            counter = 0;
            while ((thisList != NULL) && (counter < (number_of_descriptions + 1))) {
                if (NULL != currentSeqAlign)
                    prevSeqAlign = currentSeqAlign;
                currentSeqAlign = thisList;
                testSegs = (DenseSegPtr) currentSeqAlign->segs;
                if(testSegs->ids == NULL)
                    break;
                testId = testSegs->ids->next; 
                if ((NULL == curId) || (!(SeqIdMatch(curId, testId)))) {
                    counter++;
                    curSegs = testSegs;
                    curId = testId;
                }
                thisList = thisList->next;
            }
            if (counter == (number_of_descriptions + 1)) {
                prevSeqAlign->next = NULL;
                while(NULL != currentSeqAlign) {
                    nextSeqAlign = currentSeqAlign->next;
                    MemFree(currentSeqAlign);
                    currentSeqAlign = nextSeqAlign;
                }
            }
            thisValNode = thisValNode->next;
        }
    }

    return(returnList);
}

/*count the number of occurrences of pattern in sequences that
  do not overlap by more than half the pattern match length*/
static Int4 countEffectiveOccurrences(Int4 numOccur, Int4 *hitArray, patternSearchItems *patternSearch)
{
    Int4 j; /*loop index*/
    Int4 lastEffectiveOccurrence; /*last nonoverlapping occurrence*/
    Int4 count; /*what to return*/

    count = 0;
    if (numOccur > 0) {
      count = 1;
      lastEffectiveOccurrence = hitArray[0];
      for(j = 1; j < numOccur; j++) {
        if (((hitArray[j] - lastEffectiveOccurrence) * 2) >
	    patternSearch->minPatternMatchLength) {
          lastEffectiveOccurrence = hitArray[j];
          count++;
	}
      }
    }
    return(count);
}

/*Retrieve a pattern and return its string description and pass back the
  hits to query
  fp is the file descriptor to read the pattern from
  name is used to pass back the name of pattern, if any
  hitArray is the start position for matches
  numPatOccur is to pass back the number of pattern occurrences
  program_flag tells in which mode the program is being used
  seq is the query sequence
  unfilter_seq is the unfiltered version of seq, if filtering is on
  len is the length of seq
  is_dna says whether seq is DNA or proteins*/
Char * LIBCALL get_a_pat(
   FILE *fp, Char **name, Int4Ptr hitArray, Int4Ptr fullHitArray, 
   Int4 * numPatOccur, Int4 *numEffectiveOccurrences, Int4 program_flag, 
   Uint1Ptr unfilter_seq, Uint1Ptr seq, Int4 len, Boolean is_dna,
   patternSearchItems *patternSearch, seedSearchItems * seedSearch,
   Boolean showDiagnostics, ValNodePtr * error_return, 
   ValNodePtr PNTR info_vnp)
{

    Char line[BUF_SIZE]; /*line of pattern description read in*/
    Char  c; /*single character in the line*/
    Int4 i; /*number of pattern occurrences when occurrences 
             read in from file*/
    Int4  hitIndex; /*index over hits between pattern and seq*/
    Int4 twiceNumHits; /*twice the number of matches*/
    Char tmp[BUF_SIZE]; /*buffer for copying*/
    Int4 *unfilterHitArray = NULL ; /*hitArray for unfiltered hits*/
    Int4 twiceUnfilteredHits; /*twice the number of hits to unfiltered
                                sequence*/
    Int4 linePlace; /*index for characters on a line*/
    Char buffer[512];

    if ((program_flag == PAT_SEED_FLAG) || (program_flag == PAT_MATCH_FLAG)) {
      while (get_pat(fp, seedSearch->pat_space, seedSearch->name_space)) {
	if (init_pattern((Uint1 *) seedSearch->pat_space, is_dna, patternSearch, seedSearch, error_return)>=0) {
          if (NULL == unfilter_seq)
	    twiceNumHits = find_hits(hitArray, seq, len, FALSE, patternSearch);
          else {
            unfilterHitArray = (Int4 *) ckalloc(len * sizeof(Int4));
	    twiceNumHits = find_hits(hitArray, seq, len, FALSE, patternSearch);
            twiceUnfilteredHits = find_hits(unfilterHitArray, unfilter_seq, len, FALSE, patternSearch);
            if ((twiceUnfilteredHits > twiceNumHits) && showDiagnostics ) {
                sprintf(buffer,"\nWARNING: SEG filtering has wiped out "
                        "%ld occurrence(s) of this pattern\n", 
                        (long) ((twiceUnfilteredHits - twiceNumHits)/2));
                ValNodeCopyStr(info_vnp, 0, buffer);
            }
          }

          if (program_flag == PAT_SEED_FLAG) {
              if (showDiagnostics) {
                  sprintf(buffer,"\n%ld occurrence(s) of pattern in query\n", 
                          (long) twiceNumHits/2);
                  ValNodeCopyStr(info_vnp, 0, buffer);
              }
          }

	  if (twiceNumHits >0) {
	    /* copy start and stop positions. */
	    for (hitIndex=0; hitIndex<twiceNumHits; hitIndex++)
		fullHitArray[hitIndex] = hitArray[hitIndex];
		
	    /*Keep starting positions and add 1 to each starting position*/
	    for (hitIndex = 0; hitIndex < twiceNumHits; hitIndex+=2) {
	      hitArray[hitIndex/2] = hitArray[hitIndex+1]+1;
	    }
            *numEffectiveOccurrences = countEffectiveOccurrences(twiceNumHits/2,hitArray,patternSearch);
	    *numPatOccur = twiceNumHits/2;
	    *name = seedSearch->name_space;
            if (unfilterHitArray)
              MemFree(unfilterHitArray);
	    return seedSearch->pat_space;
	  }
	}
      }
      FileClose(fp);
      if (unfilterHitArray)
        MemFree(unfilterHitArray);
      return NULL;
    }
    i = 0;
    seedSearch->pat_space[0] = '\0';
    while (FileGets(line, BUF_SIZE, fp)) {
      if ((c=line[0]) == '\n') 
	continue;
      if (c == 'I') {
	if (i > 0) {
	  strcpy(tmp, seedSearch->name_space);
	  *name = tmp;
	  strcpy(seedSearch->name_space, &line[4]);
	  *numPatOccur = i;
	  init_pattern((Uint1 *) seedSearch->pat_space, is_dna, patternSearch, seedSearch, error_return);

          *numEffectiveOccurrences = countEffectiveOccurrences(i,hitArray,patternSearch);
	  return seedSearch->pat_space;
	} 
	else {	      
	  strcpy(seedSearch->name_space, &line[4]);
	}
      }
      else 
	if (c != 'H') {
	  if (c == 'P') strcat(seedSearch->pat_space, &line[2]);
	  else 
	    strcat(seedSearch->pat_space, line);
	} 
	else {
          linePlace = 2;
          while ((line[linePlace] < '0') || (line[linePlace] > '9'))
            linePlace++;
   	  hitArray[i++] = atoi(&line[linePlace]);
	}
    }
    if (i > 0) {
      *name = seedSearch->name_space;
      *numPatOccur = i;
      init_pattern((Uint1 *) seedSearch->pat_space, is_dna, patternSearch, seedSearch, error_return);
      *numEffectiveOccurrences = countEffectiveOccurrences(i,hitArray,patternSearch);
      return seedSearch->pat_space;
    }
    return NULL;
}

ValNodePtr LIBCALL seedEngineCore(BlastSearchBlkPtr search, 
                                  BLAST_OptionsBlkPtr options, 
                                  Uint1Ptr query, Uint1Ptr unfilter_query,
                                  CharPtr database, CharPtr patfile, 
                                  Int4 program_flag, FILE * patfp, 
                                  Boolean is_dna, Boolean reverseDb, 
                                  seedSearchItems *seedSearch, 
                                  Nlm_FloatHi posEThresh, 
                                  Nlm_FloatLo searchSpEff,
                                  posSearchItems *posSearch, 
                                  SeqLocPtr *seed_seq_loc, 
                                  Boolean showDiagnostics,
                                  ValNodePtr PNTR info_vnp)
{
    Nlm_FloatHi  dbLength, adjustdbLength;  /*total number of characters in database*/
    qseq_ptr query_seq; /*query sequence in record format*/
    Char  *pattern; /*string description of a pettern*/
    Char *pname; /*name of pattern*/
    Int4 seed; /*position in sequence where pattern match starts*/
    Int4 lenPatMatch; /*number of positions taken by pattern match*/
    Int4  num_seq; /*number of sequences in database*/
    Int4 list[MAX_HIT]; /*list of lengths and start positions where
                          pattern matches sequence*/
    Int4  *occurArray; /*places in query where pattern occurs*/
    Int4  *hitArray; /* beginning and end of pattern in query. */
    Int4  numPatOccur, maxNumPatterns; /*number of pattern occurrences in query string*/
    Int4  effectiveOccurrences; /*number of occurrences not overlapping
                                  in more than half the pattern*/
    Int4  occurIndex;  /*index over pattern ocuurences*/
    Int4  twiceNumMatches; /*stores return value from find_hits*/
    Int4  matchIndex;  /*index for matches to a single sequence*/
    Int4 totalOccurrences = 0; /*total occurrences of pattern in database*/
    Int4 totalBelowEThresh = 0;
    seedResultItems *seedResults = NULL; /*holds list of matching sequences*/
    patternSearchItems *patternSearch = NULL; /*holds parameters
                                                related to pattern1.c*/
    SeqAlign *thisSeqAlign = NULL; /*return value from output_hits for
                                     list of matches to one pattern occurrence*/
    ValNodePtr seqAlignList = NULL;  /*list of SeqAlign lists to pass back*/
    ValNodePtr nextValNodePtr = NULL, lastValNodePtr = NULL;  
    /*pointers into list of SeqAlign lists to pass back*/
    
    SeqIntPtr seq_int;
    GapAlignBlkPtr gap_align; /*local holder for gap_align*/

    Char buffer[512];

    /* --------------- The end of parameters definition --------------- */

    if(info_vnp != NULL)
        *info_vnp = NULL;

    gap_align = search->gap_align;
    seedResults = (seedResultItems *) ckalloc(sizeof(seedResultItems));
    patternSearch = (patternSearchItems *) ckalloc(sizeof(patternSearchItems));

    if (program_flag == PATTERN_FLAG) {
        search_pat(search->rdfp, patfile, is_dna, seedSearch, 
                   patternSearch, &(search->error_return), info_vnp);
        return NULL;
    } 
    occurArray = (Int4 *) ckalloc(sizeof(Int4)*search->sbp->query_length*2);
    hitArray = (Int4 *) MemNew(sizeof(Int4)*search->sbp->query_length*2);
    
    dbLength = 0;
    num_seq = readdb_get_num_entries(search->rdfp);
    dbLength = (Nlm_FloatHi) readdb_get_dblen(search->rdfp);
    
    /*correct the effective size of the database to take out
      the number of positions at the end of each sequence where
      pattern match cannot start*/
    
    
    while (pattern = get_a_pat(patfp, &pname, occurArray, hitArray, &numPatOccur, &effectiveOccurrences, program_flag, unfilter_query, query, search->sbp->query_length, is_dna, patternSearch, seedSearch, showDiagnostics, &(search->error_return), info_vnp)) {
        if (patternSearch->patternProbability > PAT_PROB_THRESH &&
            (patternSearch->patternProbability * dbLength > EXPECT_MATCH_THRESH)) {
            sprintf(buffer, "Pattern %s is too likely to occur in the database to be informative\n", pname);
            ValNodeCopyStr(info_vnp, 0, buffer);
        } else {
            if (patternSearch->wildcardProduct > WILDCARD_THRESH) {
                sprintf(buffer, "Due to variable wildcards pattern %s is likely to occur too many times in a single sequence\n", pname);
                ValNodeCopyStr(info_vnp, 0, buffer);
            } else {
                *seed_seq_loc = NULL;
                adjustdbLength = dbLength - (num_seq * patternSearch->minPatternMatchLength);
                if (0.0 < searchSpEff)
                    adjustdbLength = searchSpEff;

                if(options->max_num_patterns == 0 || 
                   numPatOccur < options->max_num_patterns) {
                    maxNumPatterns = numPatOccur;
                } else {

                    sprintf(buffer, "Number of times pattern occur in the sequence %d exceded limit %d. Processing only %d occurences.\n", numPatOccur, options->max_num_patterns, options->max_num_patterns);
                    ValNodeCopyStr(info_vnp, 0, buffer);
                    
                    maxNumPatterns = options->max_num_patterns;
                }

                for (occurIndex = 0; occurIndex < maxNumPatterns; 
                     occurIndex++) {
                    seed = occurArray[occurIndex];

                    if (showDiagnostics) {
                        sprintf(buffer, "%s pattern %s at position %d "
                                "of query sequence\n", pname, pattern, seed);
                        ValNodeCopyStr(info_vnp, 0, buffer);
                    }

                    if ((twiceNumMatches=find_hits(list, &query[seed-1], search->sbp->query_length-seed+1, FALSE, patternSearch)) < 2 || 
                        list[1] != 0) {
                        
                        sprintf(buffer, "twiceNumMatches=%ld list[1]=%ld\n", 
                                (long) twiceNumMatches, (long) list[1]);
                        ValNodeCopyStr(info_vnp, 0, buffer);
                        
                        BlastConstructErrorMessage("seedEngineCore", "pattern does not match the query at the place", 1, &(search->error_return));
                        return NULL;
                    }
                    
                    seq_int = SeqIntNew();
                    seq_int->from = occurArray[occurIndex] - 1;
                    seq_int->to = list[2*occurIndex] - list[2*occurIndex+1]
                        + seq_int->from;
                    seq_int->id = SeqIdDup(search->query_id);
                    ValNodeAddPointer(seed_seq_loc, SEQLOC_INT, seq_int);
                    if (program_flag != PAT_MATCH_FLAG) {
                        lenPatMatch = list[0]+1;
                        matchIndex = 0;
                        query_seq = split_target_seq(query, seed, lenPatMatch, search->sbp->query_length);
                        if (showDiagnostics) {
                            sprintf(buffer, "effective database length=%.1e\n pattern probability=%.1e\nlengthXprobability=%.1e\n", adjustdbLength, patternSearch->patternProbability, patternSearch->patternProbability * adjustdbLength);
                            ValNodeCopyStr(info_vnp, 0, buffer);
                        }
                        
                        if (!is_dna) {
                            /*extra caution about what values are tolerated*/
                            seedSearch->cutoffScore = eValueFit(
                                                                MIN(MAX_EVALUE, 10 * options->expect_value), adjustdbLength, 
                                                                seedSearch, effectiveOccurrences, patternSearch->patternProbability);
                        }
                    }
                    totalOccurrences = 0;
                    
                    if (program_flag != PAT_MATCH_FLAG) {
                        
                        do_the_seed_search(search, num_seq,
                                           query_seq, lenPatMatch, is_dna, 
                                           gap_align, patternSearch, 
                                           seedSearch, &matchIndex, 
                                           &totalOccurrences, seedResults);
                        
                        if (matchIndex > 0) 
                            quicksort_hits(matchIndex, seedResults);

                        if (showDiagnostics) {
                            sprintf(buffer,"\nNumber of occurrences of pattern in the database is %d\n", totalOccurrences);
                            ValNodeCopyStr(info_vnp, 0, buffer);
                        }

                        search->second_pass_hits += totalOccurrences;
                        search->second_pass_extends += totalOccurrences;
                        thisSeqAlign = output_hits(search->rdfp, FALSE, query, query_seq, lenPatMatch, adjustdbLength, gap_align, is_dna, effectiveOccurrences, seedSearch, seedResults, patternSearch, reverseDb, totalOccurrences, options->expect_value, search->query_id, posEThresh, posSearch, matchIndex, &totalBelowEThresh, showDiagnostics, info_vnp);

                        if(query_seq != NULL) {
                            MemFree(query_seq->lseq);
                            MemFree(query_seq);
                        }
                        
                        nextValNodePtr = (ValNodePtr) MemNew(sizeof(ValNode));
                        nextValNodePtr->data.ptrvalue = thisSeqAlign;

                        if (NULL == seqAlignList)
                            seqAlignList = nextValNodePtr;
                        else
                            lastValNodePtr->next = nextValNodePtr;

                        lastValNodePtr = nextValNodePtr;
                         
                        search->number_of_seqs_better_E += totalBelowEThresh;
                        search->second_pass_good_extends += totalBelowEThresh;
                        seed_free_all(seedResults);
                    } /*if (program_flag...) */
                } /*for (occurIndex ...)*/
	    }  /*else*/
        } /*else*/
        MemFree(hitArray); 
    }

    MemFree(occurArray);

    if (seedResults)
        MemFree(seedResults);

    MemFree(patternSearch);

    return(seqAlignList);
}

/*creates duplicate data structure for each thread of parallel process*/
static seedParallelItems * seedParallelFill(
     ReadDBFILEPtr rdpt, qseq_ptr query_seq,
     Int4 lenPatMatch, Boolean is_dna, GapAlignBlkPtr gap_align,
     patternSearchItems * patternSearch, seedSearchItems * seedSearch,
     BlastThrInfoPtr thr_info)
{
  seedParallelItems *returnData; /*structure to return*/
    int i,j; /*loop indices*/

    returnData = (seedParallelItems *) ckalloc(sizeof(seedParallelItems));

    returnData->rdpt = readdb_attach(rdpt);
    
    if (returnData->rdpt == NULL) {
        MemFree(returnData);
        return NULL;
    }

    returnData->query_seq = query_seq;
    returnData->lenPatMatch = lenPatMatch;
    returnData->gap_align = GapAlignBlkNew(1,1); 
    returnData->gap_align->gap_open = gap_align->gap_open;
    returnData->gap_align->gap_extend = gap_align->gap_extend;
    returnData->gap_align->decline_align = gap_align->decline_align;
    returnData->gap_align->x_parameter = gap_align->x_parameter;
    returnData->gap_align->matrix = gap_align->matrix;

    returnData->is_dna = is_dna;
    returnData->patternSearch = (patternSearchItems *) ckalloc(sizeof(patternSearchItems));
    returnData->patternSearch->numWords = patternSearch->numWords;
    returnData->patternSearch->match_mask = patternSearch->match_mask;
    for (i = 0; i < BUF_SIZE; i++)
      returnData->patternSearch->match_maskL[i] = patternSearch->match_maskL[i];
    for( i = 0; i < ASCII_SIZE; i++)
      for(j = 0; j < MaxW; j++)
	returnData->patternSearch->bitPatternByLetter[i][j] =
	  patternSearch->bitPatternByLetter[i][j];
    returnData->patternSearch->whichPositionPtr = patternSearch->whichPositionsByCharacter;
    for( i = 0; i < ASCII_SIZE; i++)
      returnData->patternSearch->whichPositionsByCharacter[i] =
	patternSearch->whichPositionsByCharacter[i];
    for( i = 0; i < MAX_WORDS_IN_PATTERN; i++)
      for(j = 0; j < ASCII_SIZE; j++)
	returnData->patternSearch->SLL[i][j] =
	  patternSearch->SLL[i][j];
    returnData->patternSearch->flagPatternLength = patternSearch->flagPatternLength;
    returnData->patternSearch->patternProbability = patternSearch->patternProbability;
    returnData->patternSearch->whichMostSpecific = patternSearch->whichMostSpecific;
    for( i = 0; i < MAX_WORDS_IN_PATTERN; i++)
      returnData->patternSearch->numPlacesInWord[i] =
	patternSearch->numPlacesInWord[i];

    for( i = 0; i < MAX_WORDS_IN_PATTERN; i++)
      returnData->patternSearch->spacing[i] =
	patternSearch->spacing[i];

    for( i = 0; i < MaxP; i++)
      returnData->patternSearch->inputPatternMasked[i] =
	patternSearch->inputPatternMasked[i];

   returnData->patternSearch->highestPlace = patternSearch->highestPlace;
   returnData->patternSearch->minPatternMatchLength = patternSearch->minPatternMatchLength;
   returnData->patternSearch->wildcardProduct = patternSearch->wildcardProduct;

   returnData->seedSearch = (seedSearchItems *) ckalloc(sizeof(seedSearchItems));
   for( i = 0; i < ALPHABET_SIZE; i++)
     returnData->seedSearch->charMultiple[i] = seedSearch->charMultiple[i];
   returnData->seedSearch->paramC = seedSearch->paramC;
   returnData->seedSearch->paramLambda = seedSearch->paramLambda;
   returnData->seedSearch->paramK = seedSearch->paramK;
   returnData->seedSearch->cutoffScore = seedSearch->cutoffScore;
   for( i = 0; i < ALPHABET_SIZE; i++)
     returnData->seedSearch->standardProb[i] = seedSearch->standardProb[i];
   for( i = 0; i < ASCII_SIZE; i++)
     returnData->seedSearch->order[i] = seedSearch->order[i];
   for( i = 0; i <= ALPHABET_SIZE; i++)
     returnData->seedSearch->pchars[i] = seedSearch->pchars[i];
   for( i = 0; i <= BUF_SIZE; i++)
     returnData->seedSearch->name_space[i] = seedSearch->name_space[i];
   for( i = 0; i <= PATTERN_SPACE_SIZE; i++)
     returnData->seedSearch->pat_space[i] = seedSearch->pat_space[i];

    returnData->seedResults = 
      (seedResultItems *) ckalloc(sizeof(seedResultItems));
    returnData->totalOccurrences = 0;
    returnData->matchIndex = 0;
    returnData->thr_info = thr_info;

    return(returnData);
    /*NEED to free seedResults when concatenating*/
}

/*Handles the time intensive part of the seed search in parallel*/
static VoidPtr
parallel_seed_search(VoidPtr seedParallel)
{
    Int4 seqIndex;  /*index over sequences*/
    Int4 seqIndex1; /*index over array of sequence indices*/
    Int4 lenSeqFromDb;  /* length of seqFromDb */
    Uint1Ptr seqFromDb; /*newly read sequence from database*/
    Int4  newOccurrences = 0; /*number of new occurrences of pattern*/
    hit_ptr hit_list = NULL; /*list of matches to one database sequence*/
    seedParallelItems *localSeedParallel; /*local copy of seedParallel
                                            coerced to be of the desired type*/
    Int4  start=0, stop=0; /*starting and stopping indices for
                             database search*/
    Int4 id_list_length; /*number of ids of strings to test against*/
    Int4Ptr id_list=NULL; /*list of ids of sequences to test against*/
    
    localSeedParallel = (seedParallelItems * ) seedParallel;
    id_list = NULL;

    /* Reading databases with OIDLIST - subsets imply, that number
       of returned ids may exceed db_chunk_size by Int4 bits (32) */
       
    if (localSeedParallel->thr_info->blast_gi_list != NULL ||
        localSeedParallel->rdpt->oidlist != NULL) {
        id_list = MemNew((localSeedParallel->thr_info->db_chunk_size+33)*
                         sizeof(Int4));
    }

    while (BlastGetDbChunk(localSeedParallel->rdpt, &start, &stop, id_list, &id_list_length, localSeedParallel->thr_info) != TRUE) {
        if (id_list) {
            for (seqIndex1 = 0; seqIndex1 < id_list_length; seqIndex1++) {	    	        seqIndex = id_list[seqIndex1];
            lenSeqFromDb = readdb_get_sequence(localSeedParallel->rdpt, 
                                               seqIndex, &seqFromDb);
            hit_list = get_hits(localSeedParallel->query_seq, 
                                localSeedParallel->lenPatMatch, seqFromDb, 
                                lenSeqFromDb, 
                                localSeedParallel->gap_align, 
                                localSeedParallel->is_dna, 
                                localSeedParallel->patternSearch, 
                                localSeedParallel->seedSearch, &newOccurrences);
            if (newOccurrences > 0)
                localSeedParallel->totalOccurrences += newOccurrences;
            if (hit_list) {
                storeOneMatch(hit_list, seqIndex, seqFromDb, 
                              localSeedParallel->seedResults); 
                localSeedParallel->matchIndex++;
            }
            /* Emit a tick if needed and we're not MT. */
            if (localSeedParallel->thr_info->db_mutex == NULL)
                BlastTickProc(seqIndex, localSeedParallel->thr_info);
            }
        } else {
            for (seqIndex = start; seqIndex < stop; seqIndex++) {	    	
                lenSeqFromDb = readdb_get_sequence(localSeedParallel->rdpt, 
                                                   seqIndex, &seqFromDb);
                hit_list = get_hits(localSeedParallel->query_seq, 
                                    localSeedParallel->lenPatMatch, seqFromDb, 
                                    lenSeqFromDb, 
                                    localSeedParallel->gap_align, 
                                    localSeedParallel->is_dna, 
                                    localSeedParallel->patternSearch, 
                                    localSeedParallel->seedSearch, &newOccurrences);
                if (newOccurrences > 0)
                    localSeedParallel->totalOccurrences += newOccurrences;
                if (hit_list) {
                    storeOneMatch(hit_list, seqIndex, seqFromDb, 
                                  localSeedParallel->seedResults); 
                    localSeedParallel->matchIndex++;
                }
                /* Emit a tick if needed and we're not MT. */
                if (localSeedParallel->thr_info->db_mutex == NULL)
                    BlastTickProc(seqIndex, localSeedParallel->thr_info);
                
            }
        }
    }
    
    if (localSeedParallel->thr_info->blast_gi_list != NULL ||
        localSeedParallel->rdpt->oidlist != NULL)    
        MemFree(id_list);
    
    return ((VoidPtr) localSeedParallel);
}


/*Converts the string in program to a number; sets is_dna to TRUE,
  if program name indicates DNA sequences will be used*/
Int4 LIBCALL convertProgramToFlag(Char * program, Boolean * is_dna)
{
  Int4 program_flag;  /*flag for program name to return*/

  if (strsame(program, "seed")) { 
    program_flag = SEED_FLAG; 
    *is_dna = TRUE;
  } 
  else 
    if (strsame(program, "pattern")) {
      program_flag = PATTERN_FLAG; 
      *is_dna = TRUE;
    } 
    else 
      if (strsame(program, "patseed")) {
	program_flag = PAT_SEED_FLAG; 
	*is_dna = TRUE;
      } 
      else 
	if (strsame(program, "patmatch")) {
	  program_flag = PAT_MATCH_FLAG; 
	  *is_dna = TRUE;
	} 
	else 
	  if (strsame(program, "seedp")) 
	    program_flag = SEED_FLAG; 
	  else 
	    if (strsame(program, "patternp")) 
	      program_flag = PATTERN_FLAG;
	    else 
	      if (strsame(program, "patseedp")) 
		program_flag = PAT_SEED_FLAG;
	      else 
		if (strsame(program, "patmatchp")) 
		  program_flag = PAT_MATCH_FLAG;
		else {
		  ErrPostEx(SEV_FATAL, 0, 0, "name of program not recognized %s \n", program);
		  return(1);
		}
  return(program_flag);
}

/*Free all the memory on a list of hits associated with one matching sequence*/
static void free_list(hit_ptr hp)
{
    hit_ptr thisHit, nextHit;  /*placeholders for two items on the list*/
    for (thisHit = hp; thisHit; ) {
	nextHit = thisHit->next;
	MemFree(thisHit);
	thisHit = nextHit;
    }
}

/*Frees all the memory for items on the list of results*/
void LIBCALL seed_free_all(seedResultItems *seedResults)
{
    store_ptr sp, ap; /*this item on list, and next item on lits*/
    for (sp = seedResults->listOfMatchingSequences; sp; sp = ap) {
      free_list(sp->hit_list);
      ap = sp->next;
      MemFree(sp);
    }
    seedResults->listOfMatchingSequences = NULL;
}

/*Set up a structure in which the query sequence is split into three
  pieces
    left piece: from position 0 until just before the seed position
        reversed
    middle piece: from start of seed through end of pattern
    right_piece: from 1 past end of pattern through end of sequence
  put sequences and their lengths into a structure that is returned
  seq is the entire sequence
  seed is the position where the pattern match starts
  len_pat is the number of positions taken by the pattern match
  len_query is the length of seq*/
qseq_ptr LIBCALL split_target_seq(Uint1 *seq, Int4 seed, Int4 len_pat, Int4 len_query)
{
    Int4 i; /*index over seq*/
    Uint1 *str; /*local string used to construct left piece*/
    qseq_ptr qp; /*structure to return*/

    qp = (qseq_ptr) ckalloc(sizeof(query_seq));

    qp->rseq = &seq[seed+len_pat - 1];
    qp->rlen = len_query-seed-len_pat+1;

    qp->sseq = &seq[seed - 1]; 
    qp->slen = len_pat;
    str = (Uint1Ptr) ckalloc(seed);
    /*copy over left piece reversed*/
    for (i = 0; i < seed-1; i++)
      str[i] = seq[seed-2-i];
    str[i] = '\0';
    qp->lseq = str; 
    qp->llen = seed-1;
    return qp;
}

/*insert a hit into hitlist, score, l_score, startPos,
  endPos, bi,bj,ei,ej, mul, describe the hit*/
void insert_hit(Int4 score, Int4 l_score, Int4 startPos, Int4 endPos, 
    Int4 bi, Int4 bj, Int4 ei, Int4 ej, hit_ptr *hit_list, Nlm_FloatHi mul)
{
    hit_ptr newHit, hitListItem; /*new item to initialize and add to list,
                                    and pointer into hit_list */
    
    /*set up all the fields for the new hit*/
    newHit = (hit_ptr) ckalloc(sizeof(hit_node));
    newHit->score = score;
    newHit->mul = mul;
    newHit->l_score = l_score;
    newHit->hit_pos = startPos;
    newHit->hit_end = endPos;
    newHit->bi = bi;
    newHit->bj = bj;
    newHit->ei = ei;
    newHit->ej = ej;
    /*start a new list*/
    if (*hit_list == NULL) {
      *hit_list = newHit; 
      newHit->next = NULL; 
      return;
    }
    /*insert at begining of list*/
    if ((*hit_list)->l_score < l_score) {
      newHit->next = *hit_list; 
      *hit_list = newHit;
    } 
    /*insert in middle or at end of list*/
    else {
      for (hitListItem = *hit_list; hitListItem->next; 
	   hitListItem = hitListItem->next) 
	if (hitListItem->next->l_score <= l_score) 
	  break;
      newHit->next = hitListItem->next;
      hitListItem->next = newHit;
    }
}

/*seq is a packed DNA sequence with 4 DNA letters packed into one
  item of type Uint1, len is the number of DNA characters, dseq
  is the unpacked sequence*/
void unpack_dna_seq(Uint1Ptr seq, Int4 len, Uint1Ptr dseq)
{
  Int4 remainder; /*number of DNA characters packed into last entry
                    of seq*/
  Int4 i, j; /*indices into seq and dseq respectively*/
  Uint1 ch; /*placeholder for one item in seq*/

  remainder = len % 4;

  for (i = 0, j = 0; i < len/4; i++) {
    dseq[j++] = READDB_UNPACK_BASE_1(seq[i]);
    dseq[j++] = READDB_UNPACK_BASE_2(seq[i]);
    dseq[j++] = READDB_UNPACK_BASE_3(seq[i]);
    dseq[j++] = READDB_UNPACK_BASE_4(seq[i]);
  }
  ch = seq[i];
  while (remainder > 0) { 
    dseq[j++] = READDB_UNPACK_BASE_1(ch);
    ch <<= 2; 
    remainder--;
  } 
}

/* Find matches of input sequence qp against database
   sequence seq_db at those places where qp matches the pattern
   qp is a structured representation of the query sequence
   len-of_pat is the length of the pattern
   seq_db is a sequence from the database
   len_seq_db is the length of seq_db
   gap_align is a structure to keep track of a gapped
   alignment between the query sequence and the database sequence
   is_dna indicates whether the sequences are DNA or protein
   return the list of matches in the form of a hit_ptr list*/
hit_ptr LIBCALL get_hits(qseq_ptr qp, Int4 len_of_pat, 
		 Uint1Ptr seq_db, Int4 len_seq_db, GapAlignBlkPtr gap_align, 
		 Boolean is_dna, patternSearchItems * patternSearch,
                 seedSearchItems * seedSearch, Int4 * newOccurrences) 
{
    Int4 matchIndex; /*Index over matches between seq_db and pattern*/
    Int4  twiceNumMatches;  /*twice the number of matches between the
                              databse sequence and the pattern*/
    Uint1  *matchStart, *matchEnd; /*for matches between database sequence
                                     and pattern*/
    Int4 endPoslseq;  /*ending position in qp->lseq for optimal alignment*/
    Int4 endPoslseq_db;  /*ending position of left part of seq_db reversed in
                 optimal alignment with qp->lseq*/
    Int4  endPosrseq; /*ending position in qp->rseq for optimal alignment*/
    Int4 endPosrseq_db;  /*ending position of right part of seq_db  in
                 optimal alignment with qp->lseq*/
    Nlm_FloatHi mul; /*pattern probability multiplier to get 
				  passed back from align_of_pattern*/
    Int4 patWildcardScore; /*score from wildcards */
    Int4 score; /*score for matching pattern against piec of sequence in qp*/
    Int4  scoreLeft, scoreRight, scoreOutside; /*scores for gapped alignments
                                   of left and right parts*/
    Uint1 *leftPartseq_dbReversed;  /*Holds the left part of seq_db
                                     before the match, in reverse order*/
    Int4 lenLeft;  /*length of leftPartseq_dbReversed*/
    Int4 lenRight; /*length of the part of the database sequence
                     to the right of where the match occurs*/
    Uint1 * buffer = NULL; /*used to unpack the database sequence if
                           is a DNA sequence*/
    Uint1  *seq_db_local; /*local copy of seq_db, unpacked if DNA*/
    Uint1Ptr lseq, rseq, sseq; /*three piece of the original input sequence
                                lseq is reversed, sseq is where the
                                pattern matches*/
    Int4 llen, rlen; /*lengths of lseq, rseq*/
    hit_ptr hit_list = NULL;
    Int4 hitL[MAX_HIT]; /*array of hit pairs between seq_db and pattern
                         two entries are used per hit*/
    
    (*newOccurrences) = 0;
    lseq = qp->lseq; 
    llen = qp->llen;
    rseq = qp->rseq; 
    rlen = qp->rlen;
    sseq = qp->sseq;
    leftPartseq_dbReversed = (Uint1Ptr) ckalloc(len_seq_db+1);
    twiceNumMatches = find_hits(hitL, seq_db, len_seq_db, is_dna, patternSearch);
    if (twiceNumMatches > 0 && is_dna) {
      if (len_seq_db > MAXDNA) {
	ErrPostEx(SEV_FATAL, 0, 0, "MAX DNA Sequence length exceeded %d \n",
MAXDNA);
        exit(1);
      }
      buffer = (Uint1 *) MemNew(MAXDNA * sizeof(Uint1));
      unpack_dna_seq(seq_db, len_seq_db, buffer);
      seq_db_local = buffer;
    } 
    else 
      seq_db_local =  seq_db;
    (*newOccurrences) += (twiceNumMatches/2);
    for (matchIndex = 0; matchIndex < twiceNumMatches; matchIndex+= 2) {
      matchStart = &seq_db_local[hitL[matchIndex+1]];
      matchEnd = &seq_db_local[hitL[matchIndex]];
      score = align_of_pattern(sseq, matchStart, len_of_pat,
			   matchEnd-matchStart+1, NULL, NULL, gap_align, 
			   &patWildcardScore, &mul, patternSearch, seedSearch);
      lenLeft = reverse_seq(seq_db_local, matchStart-1, leftPartseq_dbReversed);
      scoreLeft = SEMI_G_ALIGN((Uchar *) (lseq-1), (Uchar *) (leftPartseq_dbReversed-1), llen, lenLeft, NULL,
                       &endPoslseq, &endPoslseq_db, TRUE, NULL, 	gap_align,0, TRUE);
      lenRight = len_seq_db - hitL[matchIndex] /*strlen(matchEnd)+1*/;
      scoreRight = SEMI_G_ALIGN((Uchar *) (rseq-1), (Uchar *) matchEnd, rlen, lenRight, 
			       NULL, &endPosrseq, &endPosrseq_db, TRUE,NULL, gap_align,0, TRUE);
      scoreOutside = scoreRight + scoreLeft;
      if (scoreOutside > seedSearch->cutoffScore) { 
        /*No longer include patWildCardScore in 2nd argument*/
	insert_hit(scoreLeft+scoreRight+score, scoreLeft+scoreRight,
		   lenLeft+1,  hitL[matchIndex], llen-endPoslseq,lenLeft-endPoslseq_db,
		   llen+len_of_pat+endPosrseq, lenLeft+(matchEnd-matchStart)+1+endPosrseq_db, &hit_list, mul);
      }
    }
    free(leftPartseq_dbReversed);
    if (buffer)
      MemFree(buffer);
    return hit_list;
}

/*Return an e_value for getting raw score rawScore in a database of size 
  databaseSize, paramP is pattern probability*/
static Nlm_FloatHi e_value(Int4 rawScore, Nlm_FloatHi databaseSize, 
    Nlm_FloatHi paramC, Nlm_FloatHi paramLambda, Nlm_FloatHi paramP,
    Int4 effectiveOccurrences )
{
    Nlm_FloatHi E; /*e value to return*/
    Nlm_FloatHi localparamC; /*local version of the "constant" C*/

    if (rawScore < 0) 
      return 0.0;
    if (rawScore == 0)
      localparamC = 1.0;
    else
      localparamC = paramC;
    E = effectiveOccurrences*paramP*databaseSize
         *localparamC*(1.0+paramLambda*rawScore)*exp(-paramLambda*rawScore);
    return E;
}

/*return the integer score that just exceeds the e value threshold eThresh*/
Int4 LIBCALL eValueFit(Nlm_FloatHi eThresh, Nlm_FloatHi dbLength, seedSearchItems *seedSearch, 
          Int4 numOccurrences, Nlm_FloatHi patternProbability)
{
  Int4 targetScore;
  Int4 lowScore, highScore; /*for binary search*/

  lowScore = 0;
  highScore = 20;
  while (e_value(highScore, dbLength, seedSearch->paramC, 
            seedSearch->paramLambda, patternProbability, 
            numOccurrences) > eThresh)
      highScore *= 2;
  targetScore = highScore / 2;
  while (lowScore < targetScore) {
    if (e_value(targetScore, dbLength, seedSearch->paramC, 
		seedSearch->paramLambda, patternProbability, 
		numOccurrences) >  eThresh) {
      lowScore = targetScore;
      targetScore = (lowScore + highScore) / 2;
    }
    else {
      highScore = targetScore;
      targetScore = (lowScore + highScore) / 2;
    }
  }
  return(lowScore);
}

/*Gets the 3 scores of an alignment together into a ScorePtr*/
ScorePtr putScoresInSeqAlign(Int4 rawScore, Nlm_FloatHi eValue, Nlm_FloatHi lambda, Nlm_FloatHi C)
{
  Nlm_FloatHi bitScoreUnrounded; /*conversion for raw score to bit score*/
  ScorePtr returnScore = NULL;

  MakeBlastScore(&returnScore,"score",0.0, rawScore);
  MakeBlastScore(&returnScore,"e_value",eValue,0);
  bitScoreUnrounded = (rawScore*lambda - log(C) -log(lambda*rawScore+1.0))/NCBIMATH_LN2;
  MakeBlastScore(&returnScore,"bit_score",bitScoreUnrounded, 0);
  
  return(returnScore);
}

/*output the matches
    rdpt is a pointer to the sequence database
    score_only determines how much output to give per match
    seq1 is the query sequence
    qp is the split reprsentation of the query sequence
    len is the length of the query sequence
    dbLength is the total length of the database
    gap_align is structure to keep track of a gapped alignment
    is_dna tells whether the sequences are DNA or proteins 
    effectiveOccurrences counts number of not significantly overlapping
    occurrences of pattern in seq1*/
    
SeqAlignPtr LIBCALL output_hits(ReadDBFILEPtr rdpt,
	    Boolean score_only, Uint1 *seq1, qseq_ptr qp, 
	    Int4 len, Nlm_FloatHi dbLength, GapAlignBlkPtr gap_align, 
            Boolean is_dna, Int4 effectiveOccurrences,
            seedSearchItems *seedSearch, seedResultItems *seedResults, 
            patternSearchItems * patternSearch, Boolean reverse, 
            Int4 numOccurences, Nlm_FloatHi eThresh,
            SeqIdPtr query_id, Nlm_FloatHi posEthresh, 
            posSearchItems *posSearch, Int4 numMatches,
            Int4 * totalBelowEThresh, Boolean showDiagnostics,
            ValNodePtr PNTR info_vnp)
{
    store_ptr oneMatch;  /*one matching sequence, used as loop index*/
    hit_ptr oneHit; /*one hit reprsenting one place query matches 
                      database sequence; the list of matching
                      places is in oneMatch->hit_list*/
    Uint1 *dbSeqPrefixReversed; /*prefix of database sequence before the
                                 position where the hit occurs, used
                                 in reverse form for alignment purposes*/
    Int4 lenPrefix; /*length of dbSeqPrefixReversed*/
    Uint1  *buffer = NULL; /*buffer to hold an unpacked matching
                            DNA sequence*/
    Uint1  *reverseSeqFromDb;
    Uint1  *lseq, *sseq,  *rseq; /*three pieces of seq1, left of pattern match
                                  at pattern match, right of pattern match*/
    Int4  llen, rlen; /*lengths of lseq, rseq*/
    Char buff[SeqIdBufferSize]; /*buffer to hold descriptor of
                                  database sequence that matches*/
    Nlm_FloatHi mul; /*pattern probability multiplier to get 
				  passed back from align_of_pattern*/
    Int4 patWildcardScore; /*score on wildcards for Altschul stats*/
    Int4  endPosQuery; /*ending position in part of query for optimal alignment*/
    Int4  endPosDbSeq; /*ending position in part of database sequence 
                         for optimal alignment*/
    Int4 *alignScript; /*edit script that describes pairwise alignment*/
    Int4 *reverseAlignScript; /*used to revrse align script for left parts
                              of sequence that are fed in reversed*/
    Int4 *ASptr, *revASptr, temp; /*pointers/indices to the 2 alignment scripts*/
    Int4  lenDbSequence; /*length of sequence from database that matches seq1*/
    SeqIdPtr sip; /*used to extract sequence from database*/
    Nlm_FloatHi eValueForMatch; /*e-value for sequence match*/
    Nlm_FloatHi probability; /*probability of hitting pattern*/
    Boolean  eThreshWarning = TRUE; /*warn about possible missing
                                      sequences*/
    Int4 oldNumber; /*sequence number of previous match*/
    SeqAlignPtr seqAlignList =NULL; /*list of SeqAligns to return*/
    SeqAlignPtr nextSeqAlign; /*new one to add*/
    SeqAlignPtr lastSeqAlign = NULL; /*last one on list*/
    GapXEditBlockPtr nextEditBlock;  /*intermediate structure towards seqlign*/
    Int4 p; /*loop index*/
    Int4 *tempPosThreshSequences, *tempPosResultSequences; /*temporary
                       arrays for copying over old posSearch structure*/
    Int4 totalNumMatches; /*number of <threshold matches from previous calls
                            + numMatches*/
    Int4 posBaseIndex; /*loop index for copying pos results arrays*/

    Char tmpbuf[512];

    probability = ((Nlm_FloatHi) numOccurences /  dbLength);
    oldNumber = -1;
    if (score_only) {
      for (oneMatch = seedResults->listOfMatchingSequences; 
	   oneMatch; oneMatch = oneMatch->next) {
        /*retrieve description of sequence*/
	sip = NULL;
	readdb_get_descriptor(rdpt, oneMatch->seqno, &sip, NULL);
	if (sip)
	  SeqIdWrite(sip, buff, PRINTID_FASTA_LONG, sizeof(buff));
	sip = SeqIdSetFree(sip);
        /*print information about the matching sequence*/
	for (oneHit = oneMatch->hit_list; oneHit && eThreshWarning; oneHit = oneHit->next) {
	  eValueForMatch = e_value(oneHit->l_score, dbLength, 
			 seedSearch->paramC, seedSearch->paramLambda,
                         probability, effectiveOccurrences)
	                 *oneHit->mul;

	  if (eValueForMatch <= eThresh) {
              if (oneMatch->seqno != oldNumber) {
                  sprintf(tmpbuf, "%d\t%s\n", oneMatch->seqno, buff);
                  ValNodeCopyStr(info_vnp, 0, tmpbuf);
              }
              sprintf(tmpbuf, "%.3g Total Score %ld Outside Pattern Score %ld Match start in db seq %ld\n       Extent in query seq %ld %ld Extent in db seq %ld %ld\n", eValueForMatch, (long) oneHit->score, (long) oneHit->l_score, (long) oneHit->hit_pos, (long) oneHit->bi+1, (long) oneHit->ei, (long) oneHit->bj+1, (long) oneHit->ej);
              ValNodeCopyStr(info_vnp, 0, tmpbuf);
	  } else {
              if (oneMatch->seqno != oldNumber)
                  eThreshWarning = FALSE;
              oldNumber = oneMatch->seqno;
          }
	}
      }
    } 
    else {
      lseq = qp->lseq; 
      rseq = qp->rseq; 
      sseq = qp->sseq;
      llen = qp->llen; 
      rlen = qp->rlen;

      if(posEthresh > 0.0) {
          posSearch->posNumSequences = 0;
          posSearch->posResultSequences = (Int4 *) MemNew(numMatches * 
							  sizeof(Int4));
          posSearch->posResultsCounter = 0;
          posBaseIndex = 0;
      }

      for (oneMatch = seedResults->listOfMatchingSequences; 
	   oneMatch; oneMatch = oneMatch->next) {
        /*retrieve description of sequence*/
	sip = NULL;
	readdb_get_descriptor(rdpt, oneMatch->seqno, &sip, NULL);
	/* if (sip)
	  SeqIdWrite(sip, buff, PRINTID_FASTA_LONG, sizeof(buff)); */
	lenDbSequence = readdb_get_sequence(rdpt, oneMatch->seqno, 
					    (Uint1Ptr *) &(oneMatch->seq));
        if (reverse) {
	  reverseSeqFromDb = reverseSequence(oneMatch->seq,lenDbSequence);
          /*MemFree(oneMatch->seq);*/
          oneMatch->seq = reverseSeqFromDb;
	}
	reverseAlignScript =  (Int4 *) ckalloc(sizeof(Int4)*(lenDbSequence+llen+rlen+3));
	dbSeqPrefixReversed = (Uint1Ptr) ckalloc(lenDbSequence + 1);
        buffer = NULL;
	if (is_dna) {
	  buffer = (Uint1 *) MemNew(MAXDNA * sizeof(Uint1));
	  unpack_dna_seq((Uint1Ptr) oneMatch->seq, lenDbSequence, buffer);
	  oneMatch->seq = buffer;
	}
	for (oneHit = oneMatch->hit_list; oneHit && eThreshWarning; oneHit = oneHit->next) {
          eValueForMatch = e_value(oneHit->l_score, 
           dbLength, seedSearch->paramC, seedSearch->paramLambda, probability, effectiveOccurrences);
          eValueForMatch*=oneHit->mul;
          if (eValueForMatch <= eThresh) {
              
              if ((posEthresh > 0.0) && (oneMatch->seqno != oldNumber)) { 
                  if (eValueForMatch < posEthresh) {
                      posSearch->posResultSequences[posSearch->posResultsCounter] = oneMatch->seqno;
                      posSearch->posResultsCounter++;
                  } 
                  posSearch->posNumSequences++;
              }
              
	    /* if (oneMatch->seqno != oldNumber)
	       printf("\n%d\t%s\n", oneMatch->seqno, buff);
	       printf("\n E-value = %.3g: \n", eValueForMatch);
	       printf("\n Total Score = %d, Outside Pattern Score = %d: \n", oneHit->score, oneHit->l_score); */
	    lenPrefix = reverse_seq(oneMatch->seq, &oneMatch->seq[oneHit->hit_pos-2], dbSeqPrefixReversed);
	    SEMI_G_ALIGN((Uchar *) (lseq-1), (Uchar *) (dbSeqPrefixReversed-1), llen, lenPrefix,  
			 reverseAlignScript,  &endPosQuery, &endPosDbSeq, FALSE,
			 &alignScript, gap_align,0, TRUE);
	    for (revASptr = reverseAlignScript, ASptr = alignScript-1; 
		 revASptr < ASptr; revASptr++, ASptr--) {
	      temp = *revASptr; 
	      *revASptr = *ASptr; 
	      *ASptr = temp;
	    }
	    align_of_pattern(sseq, &oneMatch->seq[oneHit->hit_pos-1], len,
			     oneHit->hit_end-oneHit->hit_pos+2, alignScript, &alignScript, 
			     gap_align, &patWildcardScore, &mul, patternSearch,
			     seedSearch);
	    SEMI_G_ALIGN((Uchar *) (rseq-1), (Uchar *) &oneMatch->seq[oneHit->hit_end], rlen, 
			 lenDbSequence-oneHit->hit_end-1, 
			 alignScript,  &endPosQuery, &endPosDbSeq, FALSE, &alignScript,gap_align,0, FALSE);
	    /* DISPLAY(seq1-1+oneHit->bi, oneMatch->seq-1+oneHit->bj, 
	       oneHit->ei-oneHit->bi,
	       oneHit->ej-oneHit->bj, reverseAlignScript, oneHit->bi+1, 
	       oneHit->bj+1, is_dna); */
	    
	    nextEditBlock = TracebackToGapXEditBlock(seq1 - 1 + oneHit->bi,
						     oneMatch->seq-1 + oneHit->bj, oneHit->ei - oneHit->bi,
						     oneHit->ej - oneHit->bj, reverseAlignScript, oneHit->bi,
						     oneHit->bj);  
	    nextSeqAlign = GapXEditBlockToSeqAlign(nextEditBlock, sip, 
						   query_id);
            GapXEditBlockDelete(nextEditBlock);

	    nextSeqAlign->score = putScoresInSeqAlign(oneHit->l_score,
						      eValueForMatch,seedSearch->paramLambda, seedSearch->paramC);
	    if (NULL == seqAlignList)
	      seqAlignList = nextSeqAlign;
	    else
	      lastSeqAlign->next = nextSeqAlign;
	    lastSeqAlign = nextSeqAlign;
	    if (oneMatch->seqno != oldNumber)
              (*totalBelowEThresh)++;
	  }
	  else {
	    if (oneMatch->seqno != oldNumber)
	      eThreshWarning = FALSE;
	  }
	  oldNumber = oneMatch->seqno;
	}
	sip = SeqIdSetFree(sip);
	MemFree(dbSeqPrefixReversed); 
	MemFree(reverseAlignScript); 
      }
    }
    if (eThreshWarning && showDiagnostics) {
        sprintf(tmpbuf, "WARNING: There may be more matching sequences with e-values below the threshold of %lf\n", eThresh);
        ValNodeCopyStr(info_vnp, 0, tmpbuf);
    }
    
    if (buffer)
        MemFree(buffer);
    
    return(seqAlignList);
}

/* Put a new match to seq described by hit_list onto the
   list of matching sequences in seedResults
   seqno is the number of seq in the database indexing*/ 
void LIBCALL storeOneMatch(hit_ptr hit_list, Int4 seqno, Uint1Ptr seq, 
	      seedResultItems *seedResults)
{
    store_ptr sp; /*new node to store this match*/

    sp = (store_ptr) ckalloc(sizeof(store_node));
    sp->seq = seq;
    sp->hit_list = hit_list;
    sp->l_score = hit_list->l_score;
    sp->seqno = seqno;
    sp->next = seedResults->listOfMatchingSequences; 
    seedResults->listOfMatchingSequences = sp;
}

static int LIBCALLBACK SeedSortHits(VoidPtr i, VoidPtr j)
{
    store_ptr sp1, sp2;
    
    
    sp1 =  *((store_ptr *) i);
    sp2 =  *((store_ptr *) j);
    
    if (sp1->l_score > sp2->l_score)
        return (1);
    
    if (sp1->l_score < sp2->l_score)
        return (-1);

    if(sp1->l_score == sp2->l_score) {
        if (sp1->seqno > sp2->seqno)
            return 1;
        if (sp1->seqno < sp2->seqno)
            return -1;
    }
    
    return (0);
}

/*Sort the sequences that hit the query by increasing score;
  This routine converts the list starting at 
  seedResults->listOfMatchingSequences to
  an array for sorting and then converts back to a singly-linked list*/
void LIBCALL quicksort_hits(Int4 no_of_seq, seedResultItems *seedResults)
{
    Int4 i;
    store_ptr sp;
    store_node sentinel;
    store_ptr *qs; /*local array for sorting*/

    /*Copy the list starting from seedResults->listOfMatchingSequences 
      into the array qs*/
    qs = (store_ptr *) MemNew(sizeof(store_ptr)*(no_of_seq));

    for (i = 0, sp = seedResults->listOfMatchingSequences; 
	 i < no_of_seq; i++, sp = sp->next) {
        qs[i] = sp;
    }

    HeapSort(qs, no_of_seq, sizeof(store_ptr), SeedSortHits);

    /*Copy back to the list starting at seedResults->listOfMatchingSequences*/

    for (i = no_of_seq-1; i > 0; i--)
        qs[i]->next = qs[i-1];
    
    qs[0]->next = NULL;

    seedResults->listOfMatchingSequences = qs[no_of_seq-1];
    MemFree(qs);

    return;
}

#define seedepsilon 0.00001

/*Initialize the order of letters in the alphabet, the score matrix,
and the row sums of the score matrix. matrixToFill is the
score matrix, program_flag says which variant of the program is
used; is_dna tells whether the strings are DNA or protein*/
void LIBCALL init_order(Int4 **matrix, Int4 program_flag, Boolean is_dna, seedSearchItems *seedSearch)
{
    Char i, j; /*loop indices for matrix*/ 
    Int4 *matrixRow; /*row of matrixToFill*/ 
    Nlm_FloatHi rowSum; /*sum of scaled substitution probabilities on matrixRow*/
    
    if (is_dna) {
      seedSearch->order['A'] = 0; 
      seedSearch->order['C'] = 1;
      seedSearch->order['G'] = 2; 
      seedSearch->order['T'] = 3;
    } 
    else {
      for (i = 0; i < ALPHABET_SIZE; i++) 
	seedSearch->order[seedSearch->pchars[i]] = i;
    }
    if (program_flag == SEED_FLAG) {
      for (i = 0; i < ALPHABET_SIZE; i++) 
        seedSearch->charMultiple[i] = 0;
      for (i = 0; i < ALPHABET_SIZE; i++) {
	if (seedSearch->standardProb[i] > seedepsilon) {
	  matrixRow = matrix[i];
	  rowSum= 0;
	  for (j = 0; j < ALPHABET_SIZE; j++) {
	    if (seedSearch->standardProb[j] > seedepsilon) 
	      rowSum += seedSearch->standardProb[j]*exp(-(seedSearch->paramLambda)*matrixRow[j]);
	  }
	  seedSearch->charMultiple[i] = rowSum;
	}
      }
    }
}

/*Read in a pattern from a file; fp is the input file descriptor
  stringForPattern stores a string representation of the pattern
  pname is the name of the pattern if there is a name, NULL otherwise
  return value is 1 if pname is not NULL, 0 if pname is NULL*/
static Int4 get_pat(FILE *fp, Char *stringForPattern, Char *pname)
{
    Char line[BUF_SIZE];  /*Line read in from the file*/
    Char *name;  /*name of pattern, if there is a name*/
    Char  *rp;   /*pattern string read in from file*/

    name = NULL; 
    stringForPattern[0] = '\0';
    while (FileGets(line, BUF_SIZE, fp)) {
      /*recognize a pattern name by the string 'ID */
      if (line[0] == 'I' && line[1] == 'D') {
	/*rest of the line is the pattern name*/
	strcpy(pname, &line[3]);
	name = pname;
      } 
      else {
        if (NULL == name) {
          strcpy(pname,"No pattern name given\n");
          name = pname;
        }
	/*recognize pattern content by line starting 'PA'*/
	if (line[0] == 'P' && line[1] == 'A') {
	  /*skip over spaces*/
	  for (rp = &line[2]; *rp == ' '; rp++);
	  /*copy rest of pattern to return parameter stringForPattern*/
	  strcat(stringForPattern, rp);
	  while (FileGets(line, BUF_SIZE, fp)) {
	    if (line[0] == 'P' && line[1] == 'A') {
	      for (rp = &line[2]; *rp == ' '; rp++);
	      strcat(stringForPattern, rp); 
	    } 
	    else {
	      if (line[0] == '/') 
		break;
	      /* PROSITE definition of uniformative*/ 
	      /* if (strncmp(line, "CC   /SKIP-FLAG=TRUE", 20)==0) {
		name = NULL; 
		stringForPattern[0] = '\0'; 
		break;
	      } */
	    }
	  }
	  if (name) {
	    return 1;
	  }
	}
      }
    }
    return 0;
}

/*write out a line with pattern ID pname and pattern content pat*/
static void pat_write_head(char *pat, char *pname, ValNodePtr PNTR info_vnp)
{
    Char buffer[512];

    sprintf(buffer,"\nID  %sPA  %s", pname, pat);
    ValNodeCopyStr(info_vnp, 0, buffer);
    
    return;
}


/*Search for occurrences of all patterns in the file
  patternFileName occurring the database pointed to by rdpt
  is_dna is true if and only if the sequences are DNA sequences*/
void LIBCALL search_pat(ReadDBFILEPtr rdpt, Char *patternFileName, Boolean is_dna, seedSearchItems *seedSearch, patternSearchItems *patternSearch, ValNodePtr * error_return, ValNodePtr PNTR info_vnp)
{
    FILE *fp;  /*file descriptor for pattern file*/
    Char *pat; /*description of current pattern*/
    Char *pname;  /*name of current paatern*/
    Char buff[SeqIdBufferSize];  /*buffer for description of database sequence*/
    Uint1Ptr seq; /*sequence retrieved from the database*/
    Int4  len; /*length of sequence retrieved from the database*/
    Int4 *hitArray; /*array of hit pairs describing positions of matches
                  between the pattern and a database sequence*/
    Int4  i; /*loop index over matches*/
    Int4  numMatches; /*number of matches to current sequence*/
    Int4  seqno; /*index over all sequences*/
    SeqIdPtr sip; /*Description of the sequence from the database*/
    Char buffer[512];

    /*FIXED bug here on amount allocated*/
    hitArray = (Int4 *) ckalloc(sizeof(Int4)*MAX_HIT*2);
    fp = FileOpen(patternFileName, "r");
    pat = ckalloc(PATTERN_BUF_SIZE);
    pname = ckalloc(PATTERN_NAME_SIZE);
    while (get_pat(fp, pat, pname)) {
      if (init_pattern((Uint1 *) pat, is_dna, patternSearch, seedSearch, error_return)>=0) {  
	for (seqno = 0; seqno < rdpt->num_seqs; seqno++) {
          /*if (30 == seqno)
            printf("\n stopping at 30\n"); */
	  len = readdb_get_sequence(rdpt, seqno, &seq);
	  numMatches = find_hits(hitArray, seq, len, is_dna, patternSearch);
	  if (numMatches >0) {
	    sip = NULL;
	    readdb_get_descriptor(rdpt, seqno, &sip, NULL);
	    if (sip)
	      SeqIdWrite(sip, buff, PRINTID_FASTA_LONG, sizeof(buff));
	    sip = SeqIdSetFree(sip);
	    sprintf(buffer,"seqno=%d\t%s\n", seqno, buff);
            ValNodeCopyStr(info_vnp, 0, buffer);
	    pat_write_head(pat, pname, info_vnp);
	  }
	  for (i = 0; i < numMatches; i+=2) {
              if (is_dna) {
                  sprintf(buffer,"HI (%ld %ld)\n", (long) hitArray[i+1], (long) hitArray[i]);
                  ValNodeCopyStr(info_vnp, 0, buffer);
              } else {
                  pat_output(seq, hitArray[i+1], hitArray[i], 
                             patternSearch, info_vnp);
              }
	  }
	}
      }
    }
    MemFree(hitArray);
    MemFree(pat); 
    MemFree(pname);
    FileClose(fp);
}

/*reverse the sequence seqFromDb of length lenSeqFromDb*/
static Uint1Ptr reverseSequence(Uint1Ptr seqFromDb, Int4 lenSeqFromDb)
{
  Int4 indexBack, indexFront;
  Uint1Ptr returnSeq;

  returnSeq = (Uint1Ptr) ckalloc(lenSeqFromDb * sizeof(Uint1));
  indexFront = 0;
  indexBack = lenSeqFromDb - 1;
  while (indexFront < lenSeqFromDb) {
    returnSeq[indexFront] = seqFromDb[indexBack];
    indexFront++;
    indexBack--;
  }
  return(returnSeq);
}

static void do_the_seed_search(BlastSearchBlkPtr search, Int4 num_seq, 
  qseq_ptr query_seq, Int4 lenPatMatch, Boolean is_dna, 
  GapAlignBlkPtr gap_align, patternSearchItems * patternSearch, 
  seedSearchItems * seedSearch, Int4 * matchIndex, Int4 * totalOccurrences,
  seedResultItems * seedResults)
{
    Int2 threadIndex, secondIndex; /*indices over threads*/
    Int4 number_of_entries;  /*number of entries in the database*/
    seedParallelItems **seedParallelArray; /*keeps search information for each thread*/
    TNlmThread PNTR thread_array;
    VoidPtr status=NULL;
    store_ptr endOfList; /*end of list of matching seuqnecs*/
    ReadDBFILEPtr local_rdfp; /*local copy for indexing down a list*/
    
    if (search == NULL)
        return;

    search->thr_info->last_db_seq = 0; /* This should be reset */
    search->thr_info->db_chunk_last = search->pbp->first_db_seq;
    
    search->thr_info->number_seqs_done = 
        search->pbp->first_db_seq;  /* The 1st sequence to compare against. */

    search->thr_info->db_incr = num_seq / BLAST_NTICKS;
    
    /*readjustment of final_db_seq needs to be after initThreadInfo*/
    if (search->pbp->final_db_seq > 0)
        search->thr_info->final_db_seq = search->pbp->final_db_seq;
    else
        search->thr_info->final_db_seq = readdb_get_num_entries_total(search->rdfp);
    
    /* Directive to the function BlastGetDbChunk to use oidlist */
    if(search->rdfp->oidlist != NULL)
        search->thr_info->realdb_done = TRUE;
    else
        search->thr_info->realdb_done = FALSE;
    
    BlastSetLimits(search->pbp->cpu_limit, search->pbp->process_num);	
    
    if (NlmThreadsAvailable() && search->pbp->process_num > 1) {
        number_of_entries = INT4_MAX;
        /* Look for smallest database. */
        /* local_rdfp = readdb_attach(search->rdfp); */
        local_rdfp = search->rdfp;

        while (local_rdfp) {
            number_of_entries = MIN(number_of_entries, readdb_get_num_entries(local_rdfp));
            local_rdfp = local_rdfp->next;
        }
        /* Divide up the chunks differently if db is small. */
        if (search->thr_info->db_chunk_size*(search->pbp->process_num) > number_of_entries) {
            /* check that it is at least one. */
            search->thr_info->db_chunk_size = MAX(number_of_entries/(search->pbp->process_num), 1);
        }
        NlmMutexInit(&(search->thr_info->db_mutex));
        NlmMutexInit(&(search->thr_info->results_mutex));
        
        seedParallelArray = (seedParallelItems **) 
            MemNew((search->pbp->process_num)*sizeof(seedParallelItems *));
        for (threadIndex = 0; threadIndex<search->pbp->process_num; threadIndex++) 
            seedParallelArray[threadIndex] =  seedParallelFill(
                   search->rdfp, query_seq, lenPatMatch, is_dna, gap_align,
                   patternSearch,  seedSearch, search->thr_info);
        
        thread_array = (TNlmThread PNTR) MemNew((search->pbp->process_num)*sizeof(TNlmThread));
        for (threadIndex=0; threadIndex<search->pbp->process_num; threadIndex++) {
            thread_array[threadIndex] = NlmThreadCreateEx(parallel_seed_search, (VoidPtr) seedParallelArray[threadIndex], THREAD_RUN|THREAD_BOUND, eTP_Default, NULL, NULL);
            
            if (NlmThreadCompare(thread_array[threadIndex], NULL_thread)) {
                ErrPostEx(SEV_ERROR, 0, 0, "Unable to open thread.");
            }
        }
        for (threadIndex=0; threadIndex<search->pbp->process_num; threadIndex++)  {
            NlmThreadJoin(thread_array[threadIndex], &status);
        }
        
        seedResults->listOfMatchingSequences = NULL;
        for (threadIndex=0; threadIndex< search->pbp->process_num; threadIndex++) {
            if (NULL != seedParallelArray[threadIndex]->seedResults->listOfMatchingSequences) {
                seedResults->listOfMatchingSequences = seedParallelArray[threadIndex]->seedResults->listOfMatchingSequences;
                break;
            }
        }
        
        endOfList = seedResults->listOfMatchingSequences;
        for (secondIndex= threadIndex; secondIndex< search->pbp->process_num; secondIndex++) {
            while(NULL != endOfList->next)
                endOfList = endOfList->next;
            if (secondIndex < (search->pbp->process_num - 1))
                endOfList->next = seedParallelArray[secondIndex+1]->seedResults->listOfMatchingSequences;
        }
        
        thread_array = MemFree(thread_array);
        
        *matchIndex = 0;
        *totalOccurrences = 0;
        for (threadIndex=0; threadIndex<search->pbp->process_num; threadIndex++) {
            (*matchIndex) += seedParallelArray[threadIndex]->matchIndex;
            (*totalOccurrences) += seedParallelArray[threadIndex]->totalOccurrences;
        }
    } else {
        seedParallelArray = (seedParallelItems **) 
            MemNew(1*sizeof(seedParallelItems *));
        seedParallelArray[0] =  seedParallelFill(search->rdfp, query_seq, 
                                                 lenPatMatch, is_dna, gap_align,
                                                 patternSearch, seedSearch,
                                                 search->thr_info);
	search->thr_info->gi_current = 0;
        parallel_seed_search((VoidPtr) seedParallelArray[0]);
        (*matchIndex) += seedParallelArray[0]->matchIndex;
        (*totalOccurrences) += seedParallelArray[0]->totalOccurrences;
        seedResults->listOfMatchingSequences = 
            seedParallelArray[0]->seedResults->listOfMatchingSequences;
    }

    NlmMutexDestroy(search->thr_info->db_mutex);
    search->thr_info->db_mutex = NULL;
    NlmMutexDestroy(search->thr_info->results_mutex);
    search->thr_info->results_mutex = NULL;
    NlmMutexDestroy(search->thr_info->callback_mutex);
    search->thr_info->callback_mutex = NULL;
    
    for (threadIndex=0; threadIndex < search->pbp->process_num; 
         threadIndex++) {
        readdb_destruct(seedParallelArray[threadIndex]->rdpt);
        seedParallelArray[threadIndex]->rdpt = NULL;
        GapAlignBlkDelete(seedParallelArray[threadIndex]->gap_align);
        MemFree(seedParallelArray[threadIndex]->patternSearch);
        MemFree(seedParallelArray[threadIndex]->seedSearch);
        MemFree(seedParallelArray[threadIndex]->seedResults);
        MemFree(seedParallelArray[threadIndex]);
    }
    MemFree(seedParallelArray); 

#ifdef RLIMIT_CPU
    signal(SIGXCPU, SIG_IGN);
#endif
    return;
}

Boolean  my_time_out_boolean;

/* Called by UNIX signal for timeouts. */
#ifdef RLIMIT_CPU
static void 
timeout_shutdown(int flag)

{
	my_time_out_boolean = TRUE;
	signal(SIGXCPU, SIG_IGN);
}
#endif

static Boolean BlastSetLimits(Int4 cpu_limit, Int2 num_cpu)

{

#ifdef RLIMIT_CPU
	struct rlimit   rl;

	if (cpu_limit <= 0)
		return TRUE;

	if (getrlimit(RLIMIT_CPU, &rl) != -1 )
	{
		if (rl.rlim_cur == RLIM_INFINITY)
			rl.rlim_cur = 0;
		rl.rlim_cur += cpu_limit/num_cpu;
		setrlimit(RLIMIT_CPU, &rl);
#ifdef SIGXCPU
		sigset(SIGXCPU, timeout_shutdown);
#endif
	}
#endif
	my_time_out_boolean = FALSE;

	return TRUE;

}

#ifdef __cplusplus
}
#endif


