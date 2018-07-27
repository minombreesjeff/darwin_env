/*
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

File name: needleman.c 

Author: Hugues Sicotte 

Content Basic Global Needleman Wunsch Global Alignment Function with
restriction on the ends.
$Revision: 6.6 $
$Log: needleman.c,v $
Revision 6.6  2000/10/31 21:20:04  vakatov
[WIN32] DLL'zation

Revision 6.5  2000/09/07 04:53:43  sicotte
fix alignment calls, bad matrix calls, and misc alignments problems for sequence update

Revision 6.4  2000/08/28 16:01:01  sicotte
remove BLAST_Score typedef dependency

Revision 6.3  2000/08/28 13:19:57  sicotte
Make independent of blast.ch blastkar.ch

Revision 6.2  2000/08/28 13:17:33  sicotte
Make independent of blast.ch blastkar.ch

Revision 6.1  2000/08/28 13:05:45  sicotte
move from tools to api

Revision 6.5  2000/08/25 19:17:35  sicotte
fix bugs for end gaps

Revision 6.4  2000/08/07 13:27:26  sicotte
printf bug fix.

Revision 6.3  2000/07/28 16:03:56  sicotte
remove ncbi4na_to_blastna definition (in blastkar.c)

Revision 6.2  2000/07/28 15:49:38  kans
removed UNIX-only header

Revision 6.1  2000/07/28 15:32:14  sicotte
Quadratic Global Alignment routine NeedlemanWunschQuadraticByLoc


*****************************************************************************/

#include <needleman.h>
#include <sequtil.h>
#include <seqport.h>


/* 
   from blastkar.c : Cannot have tools/api libraries dependencies.
	translates between ncbi4na and blastna.  the first four elements
	of this array match ncbi2na.
*/

static Uint1 ncbi4na_to_blastna[] = {
15,/* Gap, 0 */
0, /* A,   1 */
1, /* C,   2 */
6, /* M,   3 */
2, /* G,   4 */
4, /* R,   5 */
9, /* S,   6 */
13, /* V,   7 */
3, /* T,   8 */
8, /* W,   9 */
 5, /* Y,  10 */
 12, /* H,  11 */
 7, /* K,  12 */
 11, /* D,  13 */
 10, /* B,  14 */
 14  /* N,  15 */
};



static Nlm_FloatHi estimateEvalue(Nlm_FloatHi effSearchSpace,
                                     Int4 bestScore, Nlm_FloatHi Kappa, Nlm_FloatHi Lambda)
{
    
    Nlm_FloatHi returnValue; /*value to return back*/
    Nlm_FloatHi normalizedScore;  /*score normalized to take into
                                    account for Lambda and K*/

    normalizedScore = Lambda * bestScore - log(Kappa);
    returnValue = effSearchSpace * exp(-(normalizedScore));
    return(returnValue);
    
}

/****************************************************************************
*
*	This function attaches a new ScorePtr to the "*old" one passed
*	in.  If *old is NULL, then *old is set equal to the new ptr.
*	The new pointer (NOT the head of the chain) is returned.
*
*	If the value is an int, then set prob to zero and pass the value in
*	as score; if the value is a Nlm_FloatHi, pass it in as prob.
*
*	The type of score is stored in an Object-id.str
****************************************************************************/

static ScorePtr
NEEDMakeBlastScore (ScorePtr PNTR old, CharPtr scoretype, Nlm_FloatHi prob, Int4 score)

{
	ScorePtr	scrp, scrp1;

	scrp = ScoreNew();

	if (score) 
	{
		scrp->choice = 1;
		scrp->value.intvalue = score;
	}
	else 
	{
		scrp->choice = 2;
		scrp->value.realvalue = (Nlm_FloatHi) prob;
	}

	scrp->id = ObjectIdNew();
	scrp->id->str = StringSave(scoretype);


	if (*old == NULL)
		*old = scrp;
	else
	{
		for (scrp1=*old; scrp1->next; scrp1=scrp1->next)
			;
		scrp1->next = scrp;	
	}
		

	return scrp;
}


/*Gets the scores of an alignment together into a ScorePtr;
  adapted from similar code with a different formula in pseed3.c*/
static ScorePtr addScoresToSeqAlign(Int4 rawScore, 
                                    Nlm_FloatHi Kappa, Nlm_FloatHi Lambda,Int4 queryLength, Int4 effdbLength)
{
  Nlm_FloatHi bitScoreUnrounded; /*conversion for raw score to bit score*/
  Nlm_FloatHi eValue;
  ScorePtr returnScore = NULL;
  Nlm_FloatHi effSearchSpace;
  effSearchSpace = queryLength*effdbLength;

  eValue = estimateEvalue( effSearchSpace,
               rawScore, Kappa, Lambda);
  NEEDMakeBlastScore(&returnScore,"score",0.0, rawScore);
  NEEDMakeBlastScore(&returnScore,"e_value",eValue,0);
  bitScoreUnrounded = ((rawScore * Lambda) - log(Kappa))/NCBIMATH_LN2;
  NEEDMakeBlastScore(&returnScore,"bit_score",bitScoreUnrounded, 0);
  return(returnScore);
}



Uint1Ptr FetchSequenceByLoc(SeqLocPtr slp,Int4* length,Int4* BspLength) {
    SeqIdPtr sip;
    BioseqPtr bsp;
    Boolean is_aa;
    Boolean translate;
    SeqPortPtr spp;
    Uint1 code,residue;
    Uint1 * query;
    Int4 index=0;
    if(length)
    *length =0;
    sip= SeqLocId(slp);
    if(!sip)
        return NULL;
    bsp = BioseqLockById(sip);
    if(!bsp)
        return NULL;
    if(BspLength)
        *BspLength = bsp->length;
    is_aa = (bsp->mol == Seq_mol_aa);
    if(is_aa) {
        code = Seq_code_ncbistdaa;
        translate = FALSE;
    } else {
        translate = TRUE;
        code = Seq_code_ncbi4na;
    }
    spp = SeqPortNewByLoc(slp, code);
    if(spp) {
        query = Malloc(SeqLocLen(slp)+2);
        if(query) {
            while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF)
                {
                    if (IS_residue(residue))
                        {
                            if(translate) 
                                residue = ncbi4na_to_blastna[residue];
                            query[index] = residue;
                            index++;
                        }
                }
            query[index] = NULLB;
        }
        spp = SeqPortFree(spp);
    }
    BioseqUnlock(bsp);
    if(length)
        *length = index;
    return query;
}

/*
  Compute the a global alignment between two SeqLocs using
  the Needleman-Wunsch algorithm.
  This algorithms requires temporary storage of order
  N*M/4 bytes for the traceback. Do not use it for large
  SeqLocs!!
  
  The alignment is assumed to exist between the orientation specified
  in the slp.. If one of the sequences need to be reversed, then the
  slp should be on the minus strand.
  
  In finding the optimal alignments end-gaps are scored.. but
  they are not counted in the returned score.
  
  The alignment returned will start and end at the endpoints of the
  SeqLocs, but may include terminal gaps in order to do so.
  
  One way to obtain the correct Karlin Altschul parameters is to
  call the function in blastkar.c, given a valid matric name.
  Int2 LIBCALL BlastKarlinGetMatrixValues(CharPtr matrix, Int4Ptr open, Int4Ptr extension, FloatHiPtr lambda, FloatHiPtr K, FloatHiPtr H)
  
  if Kappa || Lambda ==0, defaults are provided.. appropriate for nucleotides.

*/

typedef struct SWpairs {
  Int4 noGap;
  Int4 gapExists;
} SWpairs;


NLM_EXTERN SeqAlignPtr LIBCALL NeedlemanWunschQuadraticByLoc(SeqLocPtr slp1,SeqLocPtr slp2, Int4 **matrix, Int4 gapOpen, Int4 gapExtend,  Int4 *score, Nlm_FloatHi Kappa, Nlm_FloatHi Lambda) 
{
    Int4 bestScore; /*best score seen so far*/
    Int4 newScore,tmpScore;  /* score of next entry*/
    Int4 bestQueryPos, bestDbPos; /*position ending best score in
                                    query and database sequences*/
    SWpairs *scoreVector; /* keeps one row of the Smith-Waterman matrix
                             overwrite old row with new row*/
    Int4 *matrixRow; /* one row of score matrix*/
    Int4 newGapCost; /*cost to have a gap of one character*/
    Int4 prevScoreNoGapQuery; /* score one row and column up
                                 with no gaps */
    Int4 prevScoreGapQuery;   /* score if a gap already started in query*/
    Int4 continueGapScore; /* score for continuing a gap in dbSequence*/
    Int4 queryPos, dbPos; /* positions in query and dbSequence*/
    Nlm_FloatHi returnEvalue; /* e-value to return*/
    Int4 QueryMin, QueryMax;
    Int4 numsegs;
    DenseDiagPtr ddp,ddp_head,ddp_next,ddp_last;
    DenseSegPtr dsp;
    SeqAlignPtr sap;
    Uint1 ** edit_array;
    Uint1 edit_c,new_edit,old_edit;
    
    Uint1 * query;
    Int4 queryLength;
    Uint1 *dbSequence;
    Int4 dbLength,dbBytes;
    Boolean qreverse,dbreverse;
    Int4 i,j;
    Int4 rawScore;
    Nlm_FloatHi eValue;
    Int4 slp_start1,slp_start2,BspLen1,BspLen2;
    

#define SW_ALIGN 0
#define SW_GAPQUERY 1
#define SW_GAPDB 2

    /* Need BspLen and slp_start to Offset and reverse SeqAligns Later */
    query      = FetchSequenceByLoc(slp1,&queryLength,&BspLen1);
    dbSequence = FetchSequenceByLoc(slp2,&dbLength,&BspLen2);
    if(SeqLocStrand(slp1)!=Seq_strand_minus) {
        slp_start1 = SeqLocStart(slp1);
    } else {
        slp_start1 = BspLen1-1-SeqLocStop(slp1);
    }
    if(SeqLocStrand(slp2)!=Seq_strand_minus) {
        slp_start2 = SeqLocStart(slp2);
    } else {
        slp_start2 = BspLen2-1-SeqLocStop(slp2);
    }

    scoreVector = (SWpairs *) MemNew(queryLength * sizeof(SWpairs));
    dbBytes = dbLength/4+(dbLength%4==0 ? 0 : 1);
    edit_array = (Uint1 **) MemNew(queryLength*sizeof(Int4Ptr)+queryLength*dbBytes);
    bestQueryPos = 0;
    bestDbPos = 0;
    bestScore = -gapOpen-gapExtend -999;
    newGapCost = gapOpen + gapExtend;
    for (queryPos = 0; queryPos < queryLength; queryPos++) {
        edit_array[queryPos] = ((Uint1Ptr) edit_array)+queryLength*sizeof(Uint1Ptr)+dbBytes*queryPos;
        scoreVector[queryPos].noGap = 0;
        scoreVector[queryPos].gapExists = -(gapOpen);
    }
    for(dbPos = 0; dbPos < dbLength; dbPos++) {
        matrixRow = matrix[dbSequence[dbPos]];
        newScore = 0;
        prevScoreNoGapQuery = 0;
        prevScoreGapQuery = -(gapOpen);
        QueryMin = 0;
        QueryMax = queryLength-1;
        for(queryPos = QueryMin; queryPos <=QueryMax; queryPos++) {
            /*
              testing scores with a gap in query, either starting a new
              gap or extending an existing gap
            */
            if ((tmpScore = newScore - newGapCost) > 
                (prevScoreGapQuery = prevScoreGapQuery - gapExtend)) 
                prevScoreGapQuery = tmpScore;
            /*
              testing scores with a gap in dbSequence, either starting a new
              gap or extending an existing gap
            */
            if ((tmpScore = scoreVector[queryPos].noGap - newGapCost) > (continueGapScore = scoreVector[queryPos].gapExists - gapExtend)) {
                continueGapScore = tmpScore;
            }
            /*
              compute new score extending one position in query and dbSequence
            */
            newScore = prevScoreNoGapQuery + matrixRow[query[queryPos]];
            edit_c = SW_ALIGN;
            /*
         test two alternatives
            */
            if (newScore < prevScoreGapQuery) {
                newScore = prevScoreGapQuery;
                edit_c = SW_GAPQUERY;
            }
            if (newScore < continueGapScore) {
                newScore = continueGapScore;
                edit_c = SW_GAPDB;
            }
            prevScoreNoGapQuery = scoreVector[queryPos].noGap;
            scoreVector[queryPos].noGap = newScore;
            scoreVector[queryPos].gapExists = continueGapScore;
            edit_c = ((edit_array[queryPos][dbPos>>2] | (edit_c <<((dbPos%4)*2))));
            edit_array[queryPos][dbPos>>2]=edit_c;
            if (newScore > bestScore) { /* Best Score not currently used in Global ALignment */
                bestScore = newScore;
                bestDbPos = dbPos;
                bestQueryPos = queryPos;
            }
        }
    }
    MemFree(scoreVector);
     
    
    /*
     Traceback through the global scoring alignment 
    */
    dbPos = dbLength-1;
    queryPos = queryLength-1;
    numsegs=0;
    old_edit = 127;
    ddp_head = NULL;
    rawScore =0;
    ddp=NULL;
    while(dbPos>=0 && queryPos>=0) {
        new_edit = (edit_array[queryPos][dbPos>>2] >> ((dbPos%4)*2)) &3;
        if(old_edit!=new_edit) {
            numsegs++;
            if(ddp_head) {
                ddp->next = DenseDiagNew();
                ddp = ddp->next;
            } else {
                ddp_head = ddp = DenseDiagNew();
            }
            ddp->dim=2;
            ddp->len =0 ;
            ddp->starts = Malloc(2*sizeof(Int4));
            /* neglect strand.. since will discard these DDP */
        }
        ddp->len++;
        switch(new_edit) {
        case SW_ALIGN:
            rawScore += matrix[query[queryPos]][dbSequence[dbPos]];
            if(old_edit!=new_edit) {
                ddp->starts[0] = queryPos+slp_start1;
                ddp->starts[1] = dbPos+slp_start2;
            } else {
                ddp->starts[0] -=1;
                ddp->starts[1] -=1;
            }
            /*                         fprintf(stdout,"%1d %1d\n",query[queryPos],dbSequence[dbPos]); */
            dbPos--;
            queryPos--;
            break;
        case SW_GAPQUERY:
            if(old_edit!=new_edit) {
                ddp->starts[0] = -1;
                ddp->starts[1]= dbPos+slp_start2;
                rawScore += -gapOpen;
            } else {
                ddp->starts[1]-=1;
            }
            rawScore += - gapExtend;
            /*                          fprintf(stdout,"%1s %1d\n","-",dbSequence[dbPos]);  */
            dbPos--;
            break;
        case SW_GAPDB:
            if(old_edit!=new_edit) {
                ddp->starts[0] = queryPos+slp_start1;
                ddp->starts[1] = -1;
                rawScore += -gapOpen;
            } else {
                ddp->starts[0] -=1;;
            }
            rawScore += - gapExtend;

            /*                         fprintf(stdout,"%1d %1s\n",query[queryPos],"-");  */
            queryPos--;
            break;
        }
        old_edit = new_edit;
    }
    free(edit_array);

    /* Alignment algorithm finds first global maximum, ignoring end gaps .. sometimes.*/

    /* Do not score 1st and last end gap */
    if(ddp_head->starts[0]==-1 || ddp_head->starts[1]==-1) {
        rawScore += (gapOpen + ddp_head->len*gapExtend);
        /*         fprintf(stderr,"bug? in needleman.c: Added  end-gap : This may or may not be an acceptable code behavior\n"); */
    }
    if(ddp->starts[0]==-1 || ddp->starts[1]==-1) {
        rawScore += (gapOpen + ddp->len*gapExtend);
        /*         fprintf(stderr,"bug? in needleman.c: This should not happen as far as I understand it\n"); */
    }
    
    if(ddp) {
        /* Add beginning gap */
        if(ddp->starts[0]!=-1 && ddp->starts[0]>slp_start1) {
            if(ddp->starts[1]==-1) {
                ddp->len += ddp->starts[0]-slp_start1;
                ddp->starts[0]=slp_start1;
            } else {
                numsegs++;
                ddp_next = DenseDiagNew();
                ddp_next->len = ddp->starts[0]-slp_start1;
                ddp->next = ddp_next;
                ddp = ddp_next;
                ddp->dim=2;
                ddp->starts = Malloc(2*sizeof(Int4));
                ddp->starts[0]=slp_start1;
                ddp->starts[1]=-1;
            }
        } else if (ddp->starts[1]!=-1 && ddp->starts[1]>slp_start2) {
            numsegs++;
            ddp_next = DenseDiagNew();
            ddp_next->len = ddp->starts[1]-slp_start2;
            ddp->next = ddp_next;
            ddp = ddp_next;
            ddp->dim=2;
            ddp->starts = Malloc(2*sizeof(Int4));
            ddp->starts[0]=-1;
            ddp->starts[1]=slp_start2;
        }
    }
    if(ddp_head) {
        /* Add end gap */
        Int4 max_len1 = slp_start1+queryLength;
        Int4 max_len2 = slp_start2+dbLength;
        if(ddp_head->starts[0]!=-1 && ddp_head->starts[0]+ddp_head->len<max_len1) {
            if(ddp_head->starts[1]==-1) {
                ddp_head->len += max_len1-ddp_head->starts[0]-ddp_head->len;
                ddp_head->starts[0]=ddp_head->starts[0]+ddp_head->len;
            } else {
                numsegs++;
                ddp_next = DenseDiagNew();
                ddp_next->len = max_len1-ddp_head->starts[0]-ddp_head->len;
                ddp_next->starts = Malloc(2*sizeof(Int4));
                ddp_next->starts[0]=ddp_head->starts[0]+ddp_head->len;
                ddp_next->next = ddp_head;
                ddp_head = ddp_next;
                ddp_head->dim=2;
                ddp_head->starts[1]=-1;
            }
        } else if (ddp_head->starts[1]!=-1 && ddp_head->starts[1]+ddp_head->len<max_len2) {
            numsegs++;
            ddp_next = DenseDiagNew();
            ddp_next->len = max_len2-ddp_head->starts[1]-ddp_head->len;
            ddp_next->starts = Malloc(2*sizeof(Int4));
            ddp_next->starts[1]=ddp_head->starts[1]+ddp_head->len;
            ddp_next->next = ddp_head;
            ddp_head = ddp_next;
            ddp_head->dim=2;
            ddp_head->starts[0]=-1;
        }
    }

    /* Reverse order of segments  */

    if(ddp_head) {
        ddp_next = ddp_head->next;
        ddp_last = NULL;
        while(ddp_next!=NULL) {
            ddp_head->next = ddp_last;
            ddp_last = ddp_head;
            ddp_head = ddp_next;
            ddp_next = ddp_next->next;
        }
        ddp_head->next = ddp_last;
    }
    
    /* Make a DenseSeg */
    dsp = DenseSegNew();
    dsp->dim =2;
    dsp->numseg = numsegs;
    dsp->starts = Malloc(2*sizeof(Int4)*dsp->numseg);
    dsp->strands = Malloc(2*sizeof(Uint1)*dsp->numseg);
    dsp->lens = Malloc(sizeof(Int4)*dsp->numseg);
    ddp=ddp_head;
    i=0;
    while(ddp) {
        dsp->starts[2*i] = ddp->starts[0];
        dsp->starts[2*i+1] = ddp->starts[1];
        dsp->strands[2*i]=Seq_strand_plus;
        dsp->strands[2*i+1]=Seq_strand_plus;
        dsp->lens[i]=ddp->len;
       i++;
       ddp=ddp->next;
    }
    
    if(SeqLocStrand(slp1)!=Seq_strand_minus)
        qreverse = FALSE;
    else
       qreverse = TRUE;
    
    if(SeqLocStrand(slp2)!=Seq_strand_minus)
        dbreverse = FALSE;
    else
        dbreverse = TRUE;

    if(qreverse) {
        /* If reverse query..
           then reverse the order of the segments,
           AND 
           change the coordinates to "+" coordinates
          and change the "start" to point to begging of block
          on the minus strand */
        Int4 tmp_start;
        Int4 midpoint =dsp->numseg/2+dsp->numseg %2;
        for(i=0;i<midpoint;i++) {
           j=dsp->numseg-i;
           tmp_start = (BspLen1 -1- dsp->starts[2*i])-(dsp->lens[i]-1);
           dsp->starts[2*i]= (BspLen1-1 - dsp->starts[2*j])-(dsp->lens[j]-1);
           dsp->starts[2*j] = tmp_start;
           tmp_start = dsp->starts[2*i+1];
           /* reverse order of subject too */
           dsp->starts[2*i+1]=dsp->starts[2*j+1];
           dsp->starts[2*j+1]=tmp_start;
           dsp->strands[2*i]= dsp->strands[2*j] = Seq_strand_minus;
        }
    }
    if(dbreverse) {
        /* If reverse on db
           change the coordinates to "+" coordinates
          and change the "start" to point to begging of block
          on the minus strand */
        Int4 tmp_start;
        for(i=0;i<dsp->numseg;i++) {
            dsp->starts[2*i+1]= (BspLen2-1 - dsp->starts[2*i+1])-(dsp->lens[i]-1);
            dsp->strands[2*i+1] = Seq_strand_minus;
        }
    }
    
    
    while(ddp_head) {
       ddp_next = ddp_head->next;
       DenseDiagFree(ddp_head);
       ddp_head = ddp_next;
    }
    dsp->ids = SeqIdDup(SeqLocId(slp1));
   dsp->ids->next = SeqIdDup(SeqLocId(slp2));
   /*
     Put in an E-value..
   */
   if(Kappa == 0.0 || Lambda == 0.0) {
       /* Put in blosum90 defaults, appropriate for nucleotides */
       Kappa = 0.304;
       Lambda = 0.098;
   }
   dsp->scores= addScoresToSeqAlign(rawScore,
                                   Lambda, Kappa,queryLength,dbLength);
   sap = SeqAlignNew();
   sap->segs = dsp;
   sap->type = SAT_GLOBAL; /* global */
   sap->segtype = SAS_DENSEG;
   
   *score = rawScore;
   return sap;
}
