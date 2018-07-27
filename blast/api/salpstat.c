/* ===========================================================================
 *
 *                            PUBLIC DOMAIN NOTICE
 *               National Center for Biotechnology Information
 *
 *  This software/database is a "United States Government Work" under the
 *  terms of the United States Copyright Act.  It was written as part of
 *  the author's official duties as a United States Government employee and
 *  thus cannot be copyrighted.  This software/database is freely available
 *  to the public for use. The National Library of Medicine and the U.S.
 *  Government x1have not placed any restriction on its use or reproduction.
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

Author: Hugues Sicotte
        Original Source code was coded (and can be found) while I was 
        working on "whale" program and "refseq" projects.
        Code was consolidated and polished for the dbSNP project.

*/
#include <ncbi.h>
#include <ncbimisc.h> /* For HeapSort */
#include <objseq.h>
#include <objloc.h>
#include <objalign.h>
#include <objgen.h>
#include <salpacc.h>
#include <sequtil.h>
#include <stdio.h>
#include <limits.h>
#include <salpstat.h>
#include <seqport.h>
#include <salpedit.h> /* For SeqAlignSortByStart */

/** for ErrPostEx() ****/

#ifdef THIS_MODULE
#undef THIS_MODULE
#endif
#ifdef THIS_FILE
#undef THIS_FILE
#endif
static char *this_module = "salpstat";
#define THIS_MODULE this_module
static char *this_file = __FILE__;
#define THIS_FILE this_file


/* For nucleotides, Lambda, Kappa, H anre rather insensitive to
   parameters since alphabet is small.. b we are restricted to 
   rather close homology.
   */

#define Lambda 1.37
#define Kappa  0.711
#define logK -0.341082849178896
#define H 1.31

/* 
   Make a Blastn Style ScorePtr Chain for SeqAligns who have lost
   or have an invalid ScorePtr chain.
   User provide score and search-space size.
   */

NLM_EXTERN ScorePtr LIBCALL MakeBlastnStyleScore(Int4 score, FloatHi N,Int4 reward) {
  ObjectIdPtr obid;
  ScorePtr scrp,scrp_head;
  FloatHi evalue,bit_score;
  scrp = scrp_head = (ScorePtr) MemNew(sizeof(Score));
  scrp->choice = 2;
  scrp->id = ObjectIdNew();
  obid = scrp->id;
  obid->str = StringSave("score");
  scrp->value.intvalue = score;
  scrp->next = (ScorePtr) MemNew(sizeof(Score));
  scrp=scrp->next;
  scrp->choice = 3;
  scrp->id = ObjectIdNew();
  obid = scrp->id;
  obid->str = StringSave("e_value");
  bit_score = ((((FloatHi)score/((FloatHi)reward))*Lambda) - logK)/NCBIMATH_LN2;
  if(bit_score<0.0) bit_score=0.0;
  if(bit_score>0.0001) {
    evalue = N/pow((FloatHi)2.0,bit_score);
  } else {
    evalue=N;
  }
  scrp->value.realvalue = evalue;
  scrp->next = (ScorePtr) MemNew(sizeof(Score));
  scrp=scrp->next;
  scrp->choice = 3;
  scrp->id = ObjectIdNew();
  obid = scrp->id;
  obid->str = StringSave("bit_score");
  scrp->value.realvalue = bit_score;
  scrp->next = NULL;

  return scrp_head;
}

/* 
   To fix and Make Blastn-Compatible the score 
   User supplies a defaults score if score is not found.
   Invalid ScorePtr's should be removed prior to calling this function.
   */

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignScoreFix(SeqAlignPtr seqalign,FloatHi score, FloatHi N,Int4 reward) {
  ScorePtr sp=NULL,sp_last;
  ObjectIdPtr obid;
  FloatHi evalue,bit_score;
  Boolean score_set=FALSE, evalue_set=FALSE,bit_score_set=FALSE;
  Int4 int_score= (Int4) score;

  if(seqalign!=NULL) {
    if(seqalign->score==NULL) {
      seqalign->score=MakeBlastnStyleScore(int_score,N,reward);
      score_set  = evalue_set = bit_score_set = TRUE;
    } else {
      for(sp=seqalign->score;sp!=NULL;sp=sp->next) {
	if(sp->id==NULL) {
	  sp->id = ObjectIdNew();
	  sp->id->str = StringSave("unknown");
	  /* sp->id NOT set to anything */
	} else {
	  if (StringCmp(sp->id->str,"e_value")==0) {
	    evalue_set=TRUE;
	  } else if (StringCmp(sp->id->str,"sum_e")==0) {
	    evalue_set=TRUE;
	  } else if (StringCmp(sp->id->str,"score")==0) {
	    score_set=TRUE;
	    score=sp->value.intvalue;
	  } else if (StringCmp(sp->id->str,"bit_score")==0) {
	    bit_score_set=TRUE;
	  }
	}
      }
      bit_score = ((((FloatHi)score/((FloatHi)reward))*Lambda) - logK)/NCBIMATH_LN2;
      if(bit_score<0.0) bit_score=0.0;
      if(bit_score>0.00001) {
	evalue = N/pow((FloatHi)2.0,bit_score);
      } else {
	evalue= N;
      }
      if(evalue_set == FALSE || score_set ==FALSE || bit_score_set==FALSE) {
	for(sp=seqalign->score;sp!=NULL;sp=sp->next) {sp_last=sp;}
	if(evalue_set == FALSE || score_set ==FALSE || bit_score_set==FALSE) {
	  for(sp=seqalign->score;sp!=NULL;sp=sp->next) {sp_last=sp;}
	  if (score_set == FALSE) {
	    sp_last->next = (ScorePtr) MemNew(sizeof(Score));
	    sp_last = sp_last->next;
	    sp_last->choice = 2;
	    sp_last->id = ObjectIdNew();
	    obid = sp_last->id;
	    obid->str = StringSave("score");
	    sp_last->value.intvalue = (Int4) score;
	    sp_last->next=NULL;
	  }
	  if (evalue_set == FALSE) {
	    sp_last->next = (ScorePtr) MemNew(sizeof(Score));
	    sp_last = sp_last->next;
	    sp_last->choice = 3;
	    sp_last->id = ObjectIdNew();
	    obid = sp_last->id;
	    obid->str = StringSave("e_value");
	    sp_last->value.realvalue = evalue;
	    sp_last->next=NULL;
	  }
	  if (evalue_set == FALSE) {
	    sp_last->next = (ScorePtr) MemNew(sizeof(Score));
	    sp_last = sp_last->next;
	    sp_last->choice = 3;
	    sp_last->id = ObjectIdNew();
	    obid = sp_last->id;
	    obid->str = StringSave("sum_e");
	    sp_last->value.realvalue = evalue;
	    sp_last->next=NULL;
	  }
	  if (bit_score_set == FALSE) {
	    sp_last->next = (ScorePtr) MemNew(sizeof(Score));
	    sp_last = sp_last->next;
	    sp_last->choice = 3;
	    sp_last->id = ObjectIdNew();
	    obid = sp_last->id;
	    obid->str = StringSave("bit_score");
	    sp_last->value.realvalue = bit_score;
	    sp_last->next=NULL;
	  }
	}
      }
    }
  }
  return seqalign;
}
/*
  Creates a DNA Scoring Matrix: For the alphabet size =16 (alsize==16), 
  assumes that the alphabet is 
  ncbi4na, and fills scoring matric for  ambiguity codes
  to free matrix, just use Free(matrix).
 */

NLM_EXTERN Int4Ptr PNTR   DNAScoringMatrix(Int4 mismatch, Int4 reward,Int4 alsize){
  Int4Ptr PNTR alignMatrix;
  Int4 i,j,k;
  Int4 nbase1,nbase2;
  Int4 base1[4],base2[4];
  Int4 nmatches;
  /* alsize = alphabet size.. should be a 4 or 16. */
  /* If "ncbi'ized" the codes.. could make matrix this way,
     then use the ncbi translation tools 
     */
  /*  if(alsize !=4 && alsize!=16) {
    ErrPostEx(SEV_WARNING,0,0,"DNAScoringMatrix: No ambiguity codes for alphabet size=%ld\n",alsize);
    } */
  alignMatrix= (Int4Ptr PNTR) Calloc(alsize*(alsize+1),
                                     MAX(sizeof(Int4),sizeof(Int4Ptr)));
  for(i=0;i<alsize;i++) {
    *(alignMatrix+i)=(Int4Ptr) (alignMatrix+(i+1)*alsize);
  }

  for(i=0;i<alsize;i++) {
    for(j=0;j<alsize;j++) {
      alignMatrix[i][j]=mismatch;
    }
  }
  for(i=0;i<alsize;i++)
      alignMatrix[i][i]=reward;
  if(alsize!=16) 
      return alignMatrix;

  /* For ncbi4na. "presence: of bases is Bit-encoded A=1,C=2,G=4,T=8"*/
  /* Score gaps as mismatches... should not be there */
  /*   for(i=0;i<alsize;i++) alignMatrix[0][i]=alignMatrix[i][0]=mismatch; */

  for(i=0;i<alsize;i++) {
    base1[0] = i&1;
    base1[1] = (i>>1)&1;
    base1[2] = (i>>2)&1;
    base1[3] = (i>>3)&1;
    nbase1 = base1[0]+base1[1]+base1[2]+base1[3];
    for(j=0;j<alsize;j++) {
      base2[0] = j&1;
      base2[1] = (j>>1)&1;
      base2[2] = (j>>2)&1;
      base2[3] = (j>>3)&1;
      nbase2 = base2[0]+base2[1]+base2[2]+base2[3];
      nmatches=0;
      for(k=0;k<4;k++) nmatches += (base1[k]&base2[k]);  /* matches */
      if(nbase1==0 || nbase2==0) {
        alignMatrix[i][j]=mismatch; /*gaps */
      } else {
        alignMatrix[i][j] =(reward*nmatches+(nbase1*nbase2-nmatches)*mismatch)/(nbase1*nbase2);
      }
    }
  }
  return alignMatrix;
}

/* 
   NA
   */

NLM_EXTERN Boolean LIBCALL DenseDiagStats(DenseDiagPtr ddp, BioseqPtr bsp_1, BioseqPtr bsp_2, Int4Ptr a_len, Int4Ptr mmatches, Int4Ptr mmismatches, Int4Ptr pmismatches, Int4Ptr mgap_num,FloatHiPtr gap, FloatHiPtr no_gap) {
    ErrPostEx(SEV_WARNING,0,0,"DenseDiagStats not yet implemented for DenseDiag SeqAlign\n");
    return FALSE;
}
/* 
   NA
   */

NLM_EXTERN Boolean LIBCALL StdSegStats(StdSegPtr ssp, BioseqPtr bsp_1, BioseqPtr bsp_2, Int4Ptr a_len, Int4Ptr mmatches, Int4Ptr mmismatches, Int4Ptr pmismatches, Int4Ptr mgap_num,FloatHiPtr gap, FloatHiPtr no_gap) {
        /* e.g. for Translating searched */
    ErrPostEx(SEV_WARNING,0,0,"StdSegStats not yet implemented for StdSeg SeqAlign\n");
    return FALSE;
}
/* 
   NA
   */

NLM_EXTERN Boolean LIBCALL PackSegStats(PackSegPtr psp, BioseqPtr bsp_1, BioseqPtr bsp_2, Int4Ptr a_len, Int4Ptr mmatches, Int4Ptr mmismatches,Int4Ptr pmismatches, Int4Ptr mgap_num,FloatHiPtr gap, FloatHiPtr no_gap) {
    ErrPostEx(SEV_WARNING,0,0,"PackSegStats not yet implemented for PackSeg SeqAlign\n");
    return FALSE;
}
/* 
   Compute Statistics for a single DenseDiag
 */

NLM_EXTERN Boolean LIBCALL DenseSegStats(DenseSegPtr dsp, BioseqPtr bsp_1, BioseqPtr bsp_2, Int4Ptr a_len, Int4Ptr mmatches, Int4Ptr mmismatches, Int4Ptr pmismatches_ptr, Int4Ptr mgap_num,FloatHiPtr gap, FloatHiPtr no_gap) {
  Int4 i,j,k;
  Int4 start_1, start_2,len;
  Int4 matches = 0, mismatches = 0,pmismatches=0;
  Int4 gap_num = 0;
  Uint1 res_1 = 0, res_2 = 0;
  Uint1 strand0,strand1;
  SeqPortPtr spp_1, spp_2;
  Uint1 code;
  Boolean status = TRUE;
  if(dsp && (((mmatches || mmismatches) && bsp_1 && bsp_2)) || !(mmatches || mmismatches)) {
      Boolean is_gap,seek_1=TRUE,seek_2=TRUE;
      if(dsp->strands != NULL) {
          strand0 = dsp->strands[0];
          strand1 = dsp->strands[1];
      } else {
          strand0 = strand1 = 0;
      }
      if(bsp_1->mol == Seq_mol_aa)
          code = Seq_code_ncbieaa;
      else
          code = Seq_code_ncbi4na;
      if(mmatches || mmismatches) {
          spp_1 = SeqPortNew(bsp_1, 0, -1, strand0, code);
          spp_2 = SeqPortNew(bsp_2, 0, -1, strand1, code);
          SeqPortSeek(spp_1, 0, SEEK_SET);
          SeqPortSeek(spp_2, 0, SEEK_SET);
      }
      if(a_len)
          *a_len = 0;
      for(i =0,k=0; i<dsp->numseg; ++i,k+=2) {
          start_1 = dsp->starts[k];
          start_2 = dsp->starts[k+1];
          len = dsp->lens[i];
          if(a_len)
              *a_len += len;
          is_gap = ((start_1 == -1 ) || (start_2 == -1));
          if(is_gap)
              gap_num+=len;
          if(mmatches || mmismatches) {
              if(seek_1 && start_1!=-1) {
                  /* Scan the first time we encounter a real segment */
                  if(strand0 == Seq_strand_minus) 
                      SeqPortSeek(spp_1,(bsp_1->length - start_1-len),SEEK_SET);
                  else
                      SeqPortSeek(spp_1,start_1,SEEK_SET);
                  seek_1 = FALSE;
              }
              if (seek_2 && start_2!=-1) {
                  if(strand1 == Seq_strand_minus) 
                      SeqPortSeek(spp_2,bsp_2->length - (start_2+len),SEEK_SET);
                  else
                      SeqPortSeek(spp_2,start_2,SEEK_SET);
                  seek_2 = FALSE;
              }
              if(is_gap) {
                  /* scan forward */
                  if (start_2 !=-1) {
                      for(j =0; j < len; ++j) {
                          res_2 =  SeqPortGetResidue(spp_2);
                      }
                  } else if(start_1!=-1) {
                      for(j =0; j<len; ++j) {
                          res_1 =  SeqPortGetResidue(spp_1);
                      }
                  }
              } else {
                  for(j =0; j<len; ++j) {
                      res_1 = SeqPortGetResidue(spp_1);
                      res_2 = SeqPortGetResidue(spp_2);
                      if(res_1 == res_2) {
                          ++matches;
                      } else {
                          ++mismatches;
                          if((code == Seq_code_ncbi4na && (res_1&res_2)) || code == Seq_code_ncbieaa)
                              pmismatches++;
                      }
                  }
              }
          }
      }
      SeqPortFree(spp_1);
      SeqPortFree(spp_2);
  } else {
      status = FALSE;
  }
  if(pmismatches_ptr)
      *pmismatches_ptr = pmismatches;
  if(mmatches)
      *mmatches = matches;
  if(mmismatches)
      *mmismatches = mismatches;
  if (no_gap || gap) {
      FloatHi tmp_1,tmp_2;
      tmp_1 = (FloatHi)(matches);
      tmp_2 = (FloatHi)(matches + mismatches);
      if(no_gap) {
          if(tmp_2>0.0)
              *no_gap = 100.0 * (tmp_1/tmp_2);
          else
              *no_gap = 100.0;
      }
      tmp_2 += (FloatHi)gap_num;
      if(gap) {
          if(tmp_2>0.0) 
              *gap = (100.0) * tmp_1/tmp_2;
          else
              *gap = 100.0;
      }
  }
  if(mgap_num)
      *mgap_num=gap_num;
  return status;
}

/*
  Compute Pairwise Statistics for a Single SeqAlign (part of a chain or not)
 */

NLM_EXTERN Boolean LIBCALL SeqAlignSimpleStats(SeqAlignPtr align, BioseqPtr bsp_1, BioseqPtr bsp_2, FloatHiPtr no_gap_ptr, FloatHiPtr gap_ptr, Int4Ptr a_len_ptr, Int4Ptr mmatches_ptr, Int4Ptr mmismatches_ptr, Int4Ptr pmismatches_ptr_in,Int4Ptr mgap_num_ptr) {
  Int4 matches = 0, mismatches = 0, gap_num = 0,pmismatches=0,a_len;
  Int4Ptr matches_ptr, mismatches_ptr, gap_num_ptr,pmismatches_ptr;
  
  if(align && align->segs) {
      if(mmatches_ptr || no_gap_ptr || gap_ptr)
          matches_ptr = &matches;
      else
          matches_ptr = NULL;
      if(mmismatches_ptr || no_gap_ptr || gap_ptr)
          mismatches_ptr = &mismatches;
      else
          mismatches_ptr = NULL;

      if(pmismatches_ptr_in )
          pmismatches_ptr = &pmismatches;
      else
          pmismatches_ptr = NULL;

      if(mgap_num_ptr || gap_ptr)
          gap_num_ptr = &gap_num;
      else
          gap_num_ptr = NULL;
      if(align->segtype==1) {
          DenseDiagPtr ddp;
          ddp = (DenseDiagPtr) align->segs;
          DenseDiagStats(ddp, bsp_1, bsp_2,a_len_ptr,matches_ptr,mismatches_ptr,pmismatches_ptr,gap_num_ptr,gap_ptr, no_gap_ptr);
      } else if (align->segtype==2) {
          DenseSegPtr dsp;
          dsp = (DenseSegPtr) align->segs;
          DenseSegStats(dsp, bsp_1, bsp_2,a_len_ptr,matches_ptr,mismatches_ptr,pmismatches_ptr,gap_num_ptr,gap_ptr, no_gap_ptr);
      } else if (align->segtype==3) {
        /* e.g. for Translating searched */
          StdSegPtr ssp;
          ssp = (StdSegPtr) align->segs;
          StdSegStats(ssp, bsp_1, bsp_2,a_len_ptr,matches_ptr,mismatches_ptr,pmismatches_ptr,gap_num_ptr,gap_ptr, no_gap_ptr);
      } else if (align->segtype==4) {
          PackSegPtr psp;
          psp = (PackSegPtr) align->segs;
          PackSegStats(psp, bsp_1, bsp_2 ,a_len_ptr,matches_ptr,mismatches_ptr,pmismatches_ptr,gap_num_ptr,gap_ptr, no_gap_ptr);
      } else if (align->segtype==5) {
          SeqAlignPtr salp;
          Int4 lcl_a_len,lcl_matches,lcl_mismatches,lcl_pmismatches,lcl_gap_num;
          salp = (SeqAlignPtr) align->segs;
          a_len=matches = mismatches=pmismatches = gap_num=0;
          while(salp!=NULL) {
              SeqAlignSimpleStats(salp,bsp_1, bsp_2, NULL,NULL, &lcl_a_len, &lcl_matches, &lcl_mismatches, &lcl_pmismatches,&lcl_gap_num);
              a_len += lcl_a_len;
              matches +=lcl_matches;
              mismatches += lcl_mismatches;
              pmismatches += lcl_pmismatches;
              gap_num += lcl_gap_num;
              salp=salp->next;
          }
          if(a_len_ptr)
              *a_len_ptr = a_len;
          if(gap_num_ptr)
              *gap_num_ptr = gap_num;
      }
  }
  if(mmatches_ptr)
      *mmatches_ptr = matches;
  if(mmismatches_ptr)
      *mmismatches_ptr = mismatches;
  if(pmismatches_ptr_in)
      *pmismatches_ptr_in = pmismatches;
  if(mgap_num_ptr)
    *mgap_num_ptr = gap_num;
  if (no_gap_ptr || gap_ptr) {
      FloatHi tmp_1,tmp_2;
      tmp_1 = (FloatHi)(matches);
      tmp_2 = (FloatHi)(matches + mismatches);
      if(no_gap_ptr) {
          if(tmp_2>0.0)
              *no_gap_ptr = 100.0 * (tmp_1/tmp_2);
          else
              *no_gap_ptr = 100.0;
      }
      tmp_2 += (FloatHi)gap_num;
      if(gap_ptr) {
          if(tmp_2>0.0) 
              *gap_ptr = (100.0) * tmp_1/tmp_2;
          else
              *gap_ptr = 100.0;
      } 
  }
  return TRUE;
}

/* 
   returns an ordered list of the length of gaps
   for a DenseSeg SeqAlign
   */
NLM_EXTERN Int4Ptr LIBCALL DenseSegGapList(DenseSegPtr dsp,Int4Ptr gap_num) {
    Int4 start_1,start_2;
    Int4 gapnum=0;
    Int4Ptr gap_list=NULL;
    if(dsp){
      Boolean is_gap;
      Int4 i,k;
      for(i =0,k=0; i<dsp->numseg; ++i,k+=2) {
          start_1 = dsp->starts[k];
          start_2 = dsp->starts[k+1];
          is_gap = ((start_1 == -1 ) || (start_2 == -1));
          if(is_gap) 
              ++gapnum;
      }
      if(gapnum) {
          Int4 len;
          gap_list = Malloc(gapnum*sizeof(Int4));
          gapnum=0;
          for(i =0,k=0; i<dsp->numseg; ++i,k+=2) {
              start_1 = dsp->starts[k];
              start_2 = dsp->starts[k+1];
              len = dsp->lens[i];
              is_gap = ((start_1 == -1 ) || (start_2 == -1));
              if(is_gap) {
                  gap_list[gapnum++]=len;
              }
          }
      } else {
          gap_list=NULL;
      }
    }
    if(gap_num)
        *gap_num = gapnum;
    return gap_list;
}

/* 
   NA
   */
NLM_EXTERN Int4Ptr LIBCALL DenseDiagGapList(DenseDiagPtr ddp,Int4Ptr gap_num) {
    ErrPostEx(SEV_WARNING,0,0,"SeqAlignGapList not implemented for DenseSeg SeqAlign\n");
    return NULL;
}
/* 
   NA
   */

NLM_EXTERN Int4Ptr LIBCALL StdSegGapList(StdSegPtr ssp,Int4Ptr gap_num) {
    ErrPostEx(SEV_WARNING,0,0,"SeqAlignGapList not implemented for StdSeg SeqAlign\n");
    return NULL;
}
/* 
   NA
   */

NLM_EXTERN Int4Ptr LIBCALL PackSegGapList(PackSegPtr psp,Int4Ptr gap_num) {
    ErrPostEx(SEV_WARNING,0,0,"SeqAlignGapList not implemented for PackSeg SeqAlign\n");
    return NULL;
}
/*
  Returns a List of the lengths of the Segments with gaps for a single SeqAlign
  limited to pairwise for now.
 */

NLM_EXTERN Int4Ptr LIBCALL SeqAlignGapList(SeqAlignPtr align, Int4Ptr gap_num) {
    if(gap_num)
        *gap_num=0;
    if(align && align->segs) {
        if(align->segtype==1) {
            DenseDiagPtr ddp;
            ddp = (DenseDiagPtr) align->segs;
            return DenseDiagGapList(ddp,gap_num);
        } else if (align->segtype==2) {
            DenseSegPtr dsp;
            dsp = (DenseSegPtr) align->segs;
            return DenseSegGapList(dsp, gap_num);
        } else if (align->segtype==3) {
            /* e.g. for Translating searched */
            StdSegPtr ssp;
            ssp = (StdSegPtr) align->segs;
            return StdSegGapList(ssp, gap_num);
        } else if (align->segtype==4) {
            PackSegPtr psp;
            psp = (PackSegPtr) align->segs;
            return PackSegGapList(psp, gap_num);
        }
    }
    return NULL;
}
/*
  Returns a List of the lengths of the Segments with gaps for a
  linked list of SeqAlign
  limited to pairwise for now.
 */


NLM_EXTERN Int4Ptr LIBCALL SeqAlignListGapList(SeqAlignPtr sap,Int4Ptr gap_num){
  SeqAlignPtr c_align;
  Int4Ptr gap_list=NULL;
  Int4Ptr lcl_gap_list;
  Int4 this_gap_num;
  if(gap_num)
      *gap_num=0;
  if(!sap || !gap_num)
      return NULL;
  for(c_align = sap; c_align != NULL; c_align = c_align->next) {
      lcl_gap_list = SeqAlignGapList(c_align, &this_gap_num);
      if(lcl_gap_list) { 
           gap_list = Realloc(gap_list,
					sizeof(Int4)*(*gap_num+this_gap_num));
           MemCopy(gap_list+*gap_num,lcl_gap_list,sizeof(Int4)*this_gap_num);
           *gap_num+=this_gap_num;
           Free(lcl_gap_list);
           lcl_gap_list=NULL;
      }
  }
  return gap_list;
}


  /* Utility subroutine for SeqAlignWindowStats */

static void LIBCALL UpdateWindow(Int4 win_size,Int4 cur_pos,Uint1Ptr win_buf1,Uint1Ptr win_buf2,Uint1 res_1,Uint1 res_2,Uint1 code,Int4Ptr win_gap,Int4Ptr win_gapmismatch,Int4Ptr win_mismatch,Int4Ptr win_match,Boolean N_are_not_mismatches){ 
  Uint1 exit_char1,exit_char2;
  Int4 loc;
  if(win_size<=0) return; /* For case where user didn't care about window */
  loc = cur_pos % win_size;
  exit_char1=win_buf1[loc];
  win_buf1[loc] = res_1;
  exit_char2=win_buf2[loc];
  win_buf2[loc]=res_2;
  
  /* because of ambiguity codes.. a mismatch and a match are
     not the "converse" of each other */
  if(res_1=='-' || res_2=='-') {
    (*win_gap)+=1;
    (*win_gapmismatch)+=1;
  } else if (res_1==res_2 && res_1 !=0 && res_2 != 0) {
    if (code == Seq_code_iupacna) {
      if (res_1 == 'C' || res_1 == 'G' || res_1 =='A' || res_1 == 'T') {
	(*win_match)+=1;
      }
    } else {
      if (res_1 !='X' && res_1 !='*' && res_1 != 'Z') {
	(*win_match)+=1;
      }
    }
    
  } else if (res_1 != 0 && res_2 != 0){
    if (code == Seq_code_iupacna) {
      if (!N_are_not_mismatches || (res_1 != 'N' && res_2 != 'N')) {
	(*win_mismatch)+=1;
	(*win_gapmismatch)+=1;
      }
    } else {
      if (!N_are_not_mismatches || (res_1 != 'X' && res_2 != 'X')) {
	(*win_mismatch)+=1;
	(*win_gapmismatch)+=1;
      }
    }
    
  }
  
  if(exit_char1=='-' || exit_char2=='-') {
    (*win_gap)-=1;
    (*win_gapmismatch)-=1;
  } else if (exit_char1==exit_char2 && exit_char1 != 0 && exit_char2 != 0) {
    if (code == Seq_code_iupacna) {
      if (exit_char1 == 'C' || exit_char1 == 'G' || exit_char1 =='A' || exit_char1 == 'T') {
	(*win_match)-=1;
      }
    } else {
      if (exit_char1 !='X' && exit_char1 !='*' && exit_char1 != 'Z') {
	(*win_match)-=1;
      }
    }
  } else if (exit_char1 != 0 && exit_char2 != 0) {
    if (code == Seq_code_iupacna) {
      if (!N_are_not_mismatches || (exit_char1 != 'N' && exit_char2 != 'N')) {
	(*win_mismatch)-=1;
	(*win_gapmismatch)-=1;
      }
    } else {
      if (!N_are_not_mismatches || (exit_char1 != 'X' && exit_char2 != 'X')) {
	(*win_mismatch)-=1;
	(*win_gapmismatch)-=1;
      }
    }
  } 
}

/* Utility subroutine for SeqAlignWindowStats */

static void LIBCALL CheckMinMax(Int4 win_size,Int4 cur_pos,Int4 win_gap,
			Int4 win_gapmismatch, Int4 win_mismatch,Int4 win_match,
			Int4 *min_mismatch,Int4 *max_mismatch,
			Int4 *min_gap,Int4 *max_gap,
			Int4 *min_gapmismatch,Int4 *max_gapmismatch,
			Int4 *min_match, Int4 *max_match){
  if (cur_pos>=win_size) {
    if (win_gap<*min_gap) *min_gap=win_gap;
    if (win_gapmismatch<*min_gapmismatch) *min_gapmismatch=win_gapmismatch;
    if (win_mismatch<*min_mismatch) *min_mismatch=win_mismatch;
    if (win_match<*min_match) *min_match=win_match;
  }
  if (win_gap>*max_gap) *max_gap=win_gap;
  if (win_gapmismatch>*max_gapmismatch) *max_gapmismatch=win_gapmismatch;
  if (win_mismatch>*max_mismatch) *max_mismatch=win_mismatch;
  if (win_match>*max_match) *max_match=win_match;
}

/* 
   returns the minimum scoring window contained in this seqalign 
   The Scoring system is a simple match/mismatch scheme for now.
*/

NLM_EXTERN Int4 LIBCALL SeqAlignWindowStats(SeqAlignPtr align, BioseqPtr bsp_1, BioseqPtr bsp_2, 
			Int4 blen, Int4 win_size,
			Int4Ptr a_len, Boolean N_are_not_mismatches,
			Int4Ptr mmin_mismatch,Int4Ptr mmax_mismatch,
			Int4Ptr mmin_gap,Int4Ptr mmax_gap,
			Int4Ptr mmin_gapmismatch,
			Int4Ptr mmax_gapmismatch, 
			Int4Ptr mmin_match, Int4Ptr mmax_match,
			Int4Ptr mmatches, Int4Ptr mmismatches, Int4Ptr mgap_num) {
  DenseSegPtr dsp;

  Int4 start_1, stop_1, start_2, stop_2, offset, offset_1, offset_2;
  Int4 astart=0,bstart=0;
  Int4 matches = 0, mismatches = 0, gap_num = 0;
  Boolean is_gap;
  Int4 win_mismatch=0,win_gap=0,win_gapmismatch=0,win_match=0;
  Int4 i, j,len,cur_pos;
  Uint1 res_1 = 0, res_2 = 0;

  Uint1 strand;
  Int4 a;
  SeqPortPtr spp_1, spp_2;
  Uint1 code;
  Uint1Ptr win_buf1;
  Uint1Ptr win_buf2;

  *mmin_mismatch=INT_MAX;
  *mmax_mismatch=0;
  *mmin_gap=INT_MAX;
  *mmax_gap=0;
  *mmin_gapmismatch=INT_MAX;
  *mmax_gapmismatch=0;
  *mmin_match=INT_MAX;
  *mmax_match=0;

  win_buf1=(Uint1Ptr) Calloc(win_size+1,sizeof(Uint1));
  win_buf2=(Uint1Ptr) Calloc(win_size+1,sizeof(Uint1));
  
  dsp = (DenseSegPtr)(align->segs);
  if(dsp->strands != NULL)
    strand = dsp->strands[1];
  else
    strand = 0;
  if(bsp_1->mol == Seq_mol_aa)
    code = Seq_code_ncbieaa;
  else
    code = Seq_code_iupacna;

  spp_1 = SeqPortNew(bsp_1, 0, -1, dsp->strands[0], code);
  spp_2 = SeqPortNew(bsp_2, 0, -1, dsp->strands[1], code);
  SeqPortSeek(spp_1, 0, SEEK_SET);
  SeqPortSeek(spp_2, 0, SEEK_SET);
  
  *a_len = 0;
  cur_pos=1;

  /* Assume that seq1 is on the plus strand */
  for(i =0; i<dsp->numseg; ++i)
    {
      start_1 = dsp->starts[i*2];
      start_2 = dsp->starts[i*2 +1];
      if(i == 0)
	{ 
	  offset_1 = start_1 - astart;
	  if(strand == Seq_strand_minus)
	    offset_2 = blen - (start_2+dsp->lens[0]-1);
	  else
	    offset_2 = start_2 - bstart;
	  
	  /* Advance to first common residue */
	  if(offset_1 != 0 || offset_2 != 0)
	    {
	      a = offset_1; 
	      while(a >0)
		{
		  res_1 =  SeqPortGetResidue(spp_1);
		  --a;
		}
	      a = offset_2;
	      while(a>0) 
		{
		  res_2 =  SeqPortGetResidue(spp_2);
		  --a;
		}
	    }
	  offset = 0;
	}
      *a_len += (dsp->lens[i]);
      is_gap = ((start_1 == -1 ) || (start_2 == -1));
      if (start_1==-1 && start_2==-1) 
	ErrPostEx(SEV_WARNING,0,0,"Illegal seqalign with gaps on both sequences \n!");
      if(is_gap)
	{
	  for(j =0; j<dsp->lens[i]; ++j)
	    {
	      if(start_1 == -1)
		{
		  res_1='-';
		  res_2 =  SeqPortGetResidue(spp_2);
		} else { /* if(start_2 == -1) */
		  res_1 =  SeqPortGetResidue(spp_1);
		  res_2= '-';
		}
	      UpdateWindow(win_size,cur_pos,win_buf1,win_buf2,res_1,res_2,code,&win_gap,&win_gapmismatch,&win_mismatch,&win_match,N_are_not_mismatches);
	      CheckMinMax(win_size,cur_pos,win_gap,
			  win_gapmismatch,win_mismatch,win_match,
			  mmin_mismatch,mmax_mismatch,
			  mmin_gap,mmax_gap,
			  mmin_gapmismatch,mmax_gapmismatch,
			  mmin_match,mmax_match);
	      cur_pos+=1;
	    }
	  ++gap_num;
	} else {
	  stop_1 = start_1 + dsp->lens[i] -1;
	  len = stop_1 - start_1 +1;
	  start_2 += offset;
	  stop_2 = start_2 + len -1;
	  if(dsp->strands[0] == Seq_strand_minus)
	    SeqPortSeek(spp_1, bsp_1->length -1 - stop_1, SEEK_SET);
	  else
	    SeqPortSeek(spp_1, start_1, SEEK_SET);
	  if(dsp->strands[1] == Seq_strand_minus)
	    SeqPortSeek(spp_2, bsp_2->length-1 - stop_2, SEEK_SET);
	  else
	    SeqPortSeek(spp_2, start_2, SEEK_SET);
	  for(j =0; j<len; ++j)
	    {
	      res_1 =  SeqPortGetResidue(spp_1);
	      res_2 = SeqPortGetResidue(spp_2);
	      if(res_1 == res_2 && 
		 (N_are_not_mismatches || 
		  (code == Seq_code_ncbieaa && res_1 != (Uint1) 'X') ||
		  (code == Seq_code_iupacna && res_1 != (Uint1) 'N')))
		{
		  ++matches;
		}
	      else
		++mismatches;
	      UpdateWindow(win_size,cur_pos,win_buf1,win_buf2,res_1,res_2,code,&win_gap,&win_gapmismatch,&win_mismatch,&win_match,N_are_not_mismatches);
	      CheckMinMax(win_size,cur_pos,win_gap,
			  win_gapmismatch,win_mismatch,win_match,
			  mmin_mismatch,mmax_mismatch,
			  mmin_gap,mmax_gap,
			  mmin_gapmismatch,mmax_gapmismatch,
			  mmin_match,mmax_match);
	      cur_pos+=1;
	    }
	}
    }
  SeqPortFree(spp_1);
  SeqPortFree(spp_2);

  Free(win_buf1);
  Free(win_buf2);
  
  *mmatches = matches;
  *mmismatches = mismatches;
  *mgap_num=gap_num;
  if (cur_pos <=win_size) {
    /* When don't have full window. force minimum values to be 
       that of this maximum available window */
    CheckMinMax(win_size,win_size,win_gap,
		win_gapmismatch,win_mismatch,win_match,
		mmin_mismatch,mmax_mismatch,
		mmin_gap,mmax_gap,
		mmin_gapmismatch,mmax_gapmismatch,
		mmin_match,mmax_match);
    
  } 
  return MIN(cur_pos-1,win_size);
}


/*
  For Pairwise SeqAlign: 
  Compute SeqAlign Statistics for SeqAlign that covers the most bases of the
  second sequence.
 */

NLM_EXTERN void LIBCALL BestSeqAlignStats(SeqAlignPtr seqalign, BioseqPtr query_bsp, 
                       BioseqPtr subject_bsp, FloatHi *no_gap, 
                       FloatHi * gap,
                       Int4Ptr a_length, Int4Ptr mmatches, 
                       Int4Ptr mmismatches, Int4Ptr pmismatches,Int4Ptr gap_num){
    SeqAlignPtr c_align, align=NULL;
    Int4 max_len=0;
    Int4 astart,astop,bstart,bstop;
    
    for(c_align = seqalign; c_align != NULL; c_align = c_align->next) {
        if(!SeqAlignStartStop(&astart, &astop, &bstart, &bstop, c_align, query_bsp, subject_bsp)) {
            ErrPostEx(SEV_WARNING,0,0,"BestSeqAlignStats: Alignment is only gaps\n");
        } else {
            if((bstop - bstart + 1) > max_len)
                {
                    max_len = bstop - bstart;
                    align = c_align;
                }
	}
    }


    if(align != NULL)
        {
            if(!SeqAlignSimpleStats(align, query_bsp, subject_bsp, no_gap, gap, 
                                    a_length, mmatches, mmismatches,pmismatches, gap_num)) {
                ErrPostEx(SEV_WARNING,0,0,"problems computing alignment statistics\n");
            }
        } else
            ErrPostEx(SEV_WARNING,0,0,"BestSeqAlignStats: No good SeqAlign\n");
    
}

/*
  Returns the first Longest SeqAlign in a SeqAlign Linked list.
  all other SeqAligns are freed.
  
 */

NLM_EXTERN SeqAlignPtr LIBCALL SelectLongestSeqAlign (SeqAlignPtr salp)
{
  SeqAlignPtr salptmp,
              tmp,
              select;
  Int4        lens=0,
              lmax=-1;
  Int4        j=0;

  if (salp == NULL)
     return NULL;
  for (j=0,salptmp=salp; salptmp!=NULL; salptmp=salptmp->next)
     j++;
  if (j<=1)
     return salp;
  for (salptmp=salp; salptmp!=NULL; salptmp=salptmp->next) {
     lens = SeqAlignLength (salptmp);
     if (lens > lmax)
        lmax = lens;
  }
  select = salp;
  salptmp= salp;
  while (salptmp!=NULL) {
      lens = SeqAlignLength (salptmp);
      if (lens == lmax) {
          select = salptmp;
          salptmp = salptmp->next;
          select->next = NULL;
          salptmp = SeqAlignListFree (salptmp);
      } else {
          tmp = salptmp;
          salptmp = salptmp->next; 
          tmp->next = NULL;
          SeqAlignFree (tmp);
      }
  }
  return select;
}
/*
  CallBack for Sorting by SeqAlign by Length
 */

NLM_EXTERN int LIBCALLBACK SeqAlignLongestFN(VoidPtr a, VoidPtr b){
  SeqAlignPtr sa;
  SeqAlignPtr sb;
  Int4 lena,lenb;
  sa = (SeqAlignPtr) *((SeqAlignPtr PNTR) a);
  sb = (SeqAlignPtr) *((SeqAlignPtr PNTR) b);
  if(sa==NULL|| sb==NULL) {
    if (sa==NULL && sb == NULL) 
      return 0; /* that would be a bug in qsort */
    if(sa==NULL) {
      return 1;
    } else if (sb==NULL) {
      return -1;
    } else {
      return 0;
    }
  }

  lena = SeqAlignLength(sa);
  lenb = SeqAlignLength(sb);
  if(lena > lenb) {
    return (int)-1;
  } else if (lena < lenb ) {
    return (int)1;
  } else {
    return (int)0;
  }
}




/* restrict overlapping of seqaligns up to 1 bases per positions,
   for Denseseg(blast2) only keep the hits on same strand as longest hit
   The result is resorted so that earlier segments are first.
   Also, Do not allow a query segment to be used twice.(e.g. repeats)
   Keep the query sub-alignment that is part of the longest segment.
   This is only valid for pairwise alignments.
   returns the rejected SeqAligns as salp_leftover

   */

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignNonOverlap(SeqAlignPtr salp,BioseqPtr query_bsp,BioseqPtr subject_bsp,SeqAlignPtr PNTR salp_leftover_ptr,Int4 Coverage) {
    SeqAlignPtr salptmp;
    Uint1Ptr HitsPerQBase;
    Uint1Ptr HitsPerSBase;
    Uint1Ptr AcceptList;
    Int4 i,j,k,l;
    Int4 qpos,spos,qend_pos,send_pos;
    Int4 a_start,b_start,a_stop,b_stop;
    DenseSegPtr dsp;
    DenseDiagPtr ddp;
    SeqAlignPtr align,new_salp,salp_leftover=NULL;
    SeqAlignPtr salp_left_tmp=NULL;
    SeqAlignPtr PNTR salp_list;

    Int4 a_order,b_order;

    Int4 ipos,jpos,n_sap;
    Int2 best_qstrand=-1,best_sstrand=-1;

    if (salp == NULL)
        return NULL;
  
    /* Count Seq-aligns */
    for (j=0,salptmp=salp; salptmp!=NULL; salptmp=salptmp->next) {
        j++;
    }
    if (j<=1)
        return salp;
    n_sap=j;
    
    a_start = INT4_MAX;
    b_start = INT4_MAX;
    a_stop=-1;
    b_stop=-1;
    for (salptmp=salp; salptmp!=NULL; salptmp=salptmp->next) {
        Int4 lcl_a_start,lcl_a_stop,lcl_b_start,lcl_b_stop;
        if(SeqAlignStartStop(&lcl_a_start, &lcl_a_stop, &lcl_b_start, &lcl_b_stop,
                             salptmp, query_bsp, subject_bsp)) {
            if(lcl_a_start!=-1 && lcl_a_start<a_start)
                a_start=lcl_a_start;
            if(lcl_b_start!=-1 && lcl_b_start<b_start)
                b_start=lcl_b_start;
            if(lcl_a_stop>a_stop)
                a_stop=lcl_a_stop;
            if(lcl_b_stop>b_stop)
                b_stop=lcl_b_stop;
        }
    }
    salp_list=(SeqAlignPtr PNTR) Malloc((size_t)((n_sap+1)*(sizeof(salp_list[0]))));
    for (j=0,salptmp=salp; salptmp!=NULL; salptmp=salptmp->next,j++) {
        salp_list[j]=salptmp;
    }
    for (j=0; j<n_sap; j++) {
        salp_list[j]->next=NULL;
    }
    HeapSort((void *)salp_list,(size_t)n_sap,(size_t)sizeof(salp_list[0]),SeqAlignLongestFN);    
    
    HitsPerQBase = (Uint1Ptr) Calloc(a_stop+1,sizeof(Uint1));
    HitsPerSBase = (Uint1Ptr) Calloc(b_stop+1,sizeof(Uint1));
    AcceptList = (Uint1Ptr) Calloc(n_sap+1,sizeof(Uint1));

    /* Loop over length-ordered Alignments: Keep Best Non-overlappping regions*/
    for(i=0;i<n_sap;i++) {
        align=salp_list[i];
        if(align!=NULL) {
            SeqAlignBioseqsOrder(align,query_bsp,subject_bsp,&a_order,&b_order);
            /* Check if any bases were not already covered. */
            /* bases on the "non-gapped sequence" of gapped segments
               are considered covered. */
            if(align->segtype==1) {
                ddp = (DenseDiagPtr) (align->segs);
                if(ddp) {
                    AcceptList[i]=1;
                }
                while(ddp!=NULL && AcceptList[i]!=0) {
                    if(best_qstrand ==-1) best_qstrand = (Int2)ddp->strands[a_order];
                    if(best_sstrand ==-1) best_sstrand = (Int2)ddp->strands[b_order];
                    qpos=ddp->starts[a_order];
                    if(qpos>=0) {
                        if(best_qstrand != (Int2)ddp->strands[a_order]) {
                            AcceptList[i]=0;
                            break;
                        }
                    }
                    spos=ddp->starts[b_order];
                    if(spos>=0) {
                        if(best_sstrand != (Int2)ddp->strands[b_order]) {
                            AcceptList[i]=0;
                            break;
                        }
                    }
                    qend_pos=qpos+ddp->len;
                    send_pos=spos+ddp->len;
                    for(k=qpos,l=spos;k<qend_pos && l<send_pos;k++,l++) {
                        if((qpos!=-1 && HitsPerQBase[k]>=Coverage)  || (spos!=-1 && HitsPerSBase[l]>=Coverage)) {
                            AcceptList[i]=0;
                            qpos+=qend_pos;
                        }
                    }
                    ddp = ddp->next;
                } 
            } else if(align->segtype==2) {
                dsp = (DenseSegPtr) align->segs;
                if(dsp!=NULL) {
                    AcceptList[i]=1;
                    if(best_qstrand ==-1) {
                        if(dsp->strands!=NULL)
                            best_qstrand = (Int2)dsp->strands[a_order];
                        else
                            best_qstrand = 0;
                    }
                    if(best_sstrand ==-1) {
                        if(dsp->strands!=NULL) 
                            best_sstrand = (Int2)dsp->strands[b_order];
                        else
                            best_sstrand = 0;
                    }
                    for(j = 0; j<dsp->numseg; ++j) {
                        ipos = 2*j + a_order;
                        qpos=dsp->starts[ipos];
                        if(qpos>=0) {
                            if(dsp->strands!=NULL && best_qstrand != (Int2)dsp->strands[ipos]) {
                                AcceptList[i]=0;
                                break;
                            }
                        }
                        jpos = 2*j + b_order;
                        spos=dsp->starts[jpos];
                        if(spos>=0) {
                            if(dsp->strands!=NULL && best_sstrand != (Int2)dsp->strands[jpos]) {
                                AcceptList[i]=0;
                                break;
                            }
                        }
                        qend_pos=qpos+dsp->lens[j];
                        send_pos=spos+dsp->lens[j];
                        {
                            for(k=qpos,l=spos;k<qend_pos &&l<send_pos;k++,l++) {
                                if((qpos!=-1 && HitsPerQBase[k]>=Coverage)  || (spos!=-1 && HitsPerSBase[l]>=Coverage)) {
                                    AcceptList[i]=0;
                                    k+=qend_pos; 
                                    j+=dsp->numseg;
                                }
                            }
                        }
                    }
                }
            }
            /* If we've accepted the new segment(s), 
               .. and there is room to accep them.. increment counters */
            if(AcceptList[i]==1) {/* increment B-value counter for bases */
                if(align->segtype==1) {
                    ddp=(DenseDiagPtr)align->segs;
                    while(ddp!=NULL) {
                        qpos=ddp->starts[a_order];
                        spos=ddp->starts[b_order];
                        if(qpos!=-1 && spos!=-1) {
                            qend_pos=qpos+ddp->len;
                            send_pos=spos+ddp->len;
                            for(k=qpos,l=spos;k<qend_pos && l<send_pos;k++,l++) {
                                if(qpos!=-1) 
                                    HitsPerQBase[k]+=1;
                                if(spos!=-1) 
                                    HitsPerSBase[l]+=1;
                                if((qpos!=-1 && HitsPerQBase[k]>Coverage)  || (spos!=-1 && HitsPerSBase[l]>Coverage)) {
                                    AcceptList[i]=0;
                                    k+=qend_pos;
                                    j+=dsp->numseg;
                                }
                            }
                        }
                        ddp = ddp->next;
                    } 
                } else if(align->segtype==2) {
                    dsp = (DenseSegPtr) align->segs;
                    if(dsp!=NULL) {
                        for(j = 0; j<dsp->numseg; ++j) {
                            ipos = 2*j + a_order;
                            jpos = 2*j + b_order;
                            qpos=dsp->starts[ipos];
                            spos=dsp->starts[jpos];
                            qend_pos=qpos+dsp->lens[j];
                            send_pos=spos+dsp->lens[j];
                            for(k=qpos,l=spos;k<qend_pos && l<send_pos;k++,l++) {
                                if(qpos!=-1) 
                                    HitsPerQBase[k]+=1;
                                if(spos!=-1) 
                                    HitsPerSBase[l]+=1;
                                if((qpos!=-1 && HitsPerQBase[k]>Coverage)  || (spos!=-1 && HitsPerSBase[l]>Coverage)) {
                                    ErrPostEx(SEV_WARNING,0,0,"SeqAlign maps more than once bases in the subject\n");
                                    AcceptList[i]=0;
                                    k+=qend_pos;
                                    j+=dsp->numseg;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    /* Shift down list */
    salp=new_salp=salp_list[0]; /* Keep at least first one */
    for(i=1;i<n_sap;i++) {
        salptmp=salp_list[i];
        if (salptmp==NULL)
            break;
        if(AcceptList[i]==0) {
            if(salp_leftover_ptr) {
                if(salp_leftover) {
                    salp_left_tmp->next=salptmp;
                } else {
                    salp_leftover=salptmp;
                }
                salp_left_tmp=salptmp;
            } else {
                salptmp->next=NULL;
                salp_list[i] = SeqAlignFree (salptmp);
            }
        } else {
            salp->next=salptmp;
            salp=salptmp;
        }
    }
    salp->next=NULL;
    if(salp_left_tmp) 
        salp_left_tmp->next=NULL;
    if(salp_leftover_ptr)
        *salp_leftover_ptr=salp_leftover;
    Free(AcceptList);
    Free(HitsPerQBase);
    Free(HitsPerSBase);
    
    Free(salp_list);
    return SeqAlignSortByStart(new_salp,(Int2)0);
}

NLM_EXTERN Boolean LIBCALL SeqAlignConsistentOverlap(SeqAlignPtr salptmp2, SeqAlignPtr salptmp) {
    ErrPostEx(SEV_WARNING,0,0,"One Alignment may be truncated due to repeats\n");
    return FALSE;
}

/*
  Take Two sets of Starts and Stops and returns the Overlap.
 */
NLM_EXTERN Int4 LIBCALL SeqAlignStartStopOverlap(Int4 a_start1, Int4 a_stop1, Int4 b_start1, Int4 b_stop1,Int4 a_start2, Int4 a_stop2, Int4 b_start2, Int4 b_stop2,Int4Ptr Overlap_lena, Int4Ptr Overlap_lenb) {
    Int4 overlap_lena=0,overlap_lenb=0;
    Boolean overlapa=FALSE,overlapb=FALSE;

    if(a_start1>=a_start2 && a_stop2>=a_start1) {
        overlapa=TRUE;
        overlap_lena=a_stop2-a_start1+1;
    } else if (a_start2>=a_start1 && a_stop1>=a_stop2) {
        overlapa=TRUE;
        overlap_lena=a_stop1-a_start2+1;
    }
    if(b_start1>=b_start2 && b_stop2>=b_stop1) {
        overlapb=TRUE;
        overlap_lenb=b_stop2-b_start1+1;
    } else if(b_start2>=b_start1 && b_stop1>=b_stop2) {
        overlapb=TRUE;
        overlap_lenb=b_stop1-b_start2+1;
    }
    if(Overlap_lena)
        *Overlap_lena=overlap_lena;
    if(Overlap_lenb)
        *Overlap_lenb=overlap_lenb;
    return (overlapa || overlapb);
}

/* 
   Takes a linked list of blast SeqAligns for a pair of sequences and
   make a non-redundant SeqAlign.
   
   */

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignNonRedundant(SeqAlignPtr salp,BioseqPtr query_bsp,BioseqPtr subject_bsp,SeqAlignPtr PNTR salp_leftover_ptr) {
    SeqAlignPtr salptmp,salptmp2,salpprev2,salptmp2next;
    SeqAlignPtr salptmplast;
    Int4 i,j;
    
    SeqAlignPtr salp_leftover=NULL;
    Boolean NeedsMerging,OverlapOK;

    if (salp == NULL)
        return NULL;
  
    /* Count Seq-aligns */
    for (j=0,salptmp=salp; salptmp!=NULL; salptmp=salptmp->next) {
        j++;
    }
    if (j<=1)
        return salp;
    
    salp = SeqAlignNonOverlap(salp,query_bsp,subject_bsp,&salp_leftover,1);
    if(!salp_leftover)
        return salp;

    /* Count Seq-aligns */
    for (j=0,salptmp=salp; salptmp!=NULL; salptmp=salptmp->next) {
        j++;
    }

    /* Count Seq-aligns */
    for (j=0,salptmp=salp_leftover; salptmp!=NULL; salptmp=salptmp->next) {
        j++;
    }
    salpprev2=NULL;
    for(salptmp2=salp_leftover,i=0;salptmp2!=NULL;salptmp2=salptmp2next,i++) {
        Int4 left_a_start,left_a_stop,left_b_start,left_b_stop;
        salptmp2next=salptmp2->next;
        if(SeqAlignStartStop(&left_a_start, &left_a_stop, &left_b_start, &left_b_stop,salptmp2, query_bsp, subject_bsp)) {
            OverlapOK = TRUE; /* Set to TRUE if it is OK to add to list. */
            NeedsMerging = FALSE; /* Set to TRUE if need to Merge SeqAligns before adding */
            salptmplast=NULL;
            /* Loop over all the alignment already accepted.. and
               make sure that no overlap is inconsistent */
            for (salptmp=salp,j=0; salptmp!=NULL && OverlapOK; salptmp=salptmp->next,j++) {
                Int4 lcl_a_start,lcl_a_stop,lcl_b_start,lcl_b_stop;
                Int4 olap_a,olap_b;
                SeqAlignStartStop(&lcl_a_start, &lcl_a_stop, &lcl_b_start, &lcl_b_stop, salptmp, query_bsp, subject_bsp);
                /* Simple Check to see if the two SeqAlign overlap in coverage.
                 */
                if(SeqAlignStartStopOverlap(left_a_start, left_a_stop, left_b_start, left_b_stop, lcl_a_start, lcl_a_stop, lcl_b_start, lcl_b_stop,&olap_a,&olap_b)) {
                    NeedsMerging = TRUE;
                    if(olap_a==olap_b && SeqAlignConsistentOverlap(salptmp2,salptmp)) {
                        /* XXXX Overlapping region of alignment is the
                           same.. => Remember that this one must
                           be truncated
                           */
                    } else {
                        OverlapOK=FALSE;
                    }
                }
                salptmplast=salptmp;
            }
            if(OverlapOK) {
                    if(NeedsMerging) {
                        /* delete from salp2 */
                        for (salptmp=salp,j=0; salptmp!=NULL && OverlapOK; salptmp=salptmp->next,j++) {
                            Int4 lcl_a_start,lcl_a_stop,lcl_b_start,lcl_b_stop;
                            SeqAlignStartStop(&lcl_a_start, &lcl_a_stop, &lcl_b_start, &lcl_b_stop, salptmp, query_bsp, subject_bsp);
                            if(SeqAlignStartStopOverlap(left_a_start, left_a_stop, left_b_start, left_b_stop, lcl_a_start, lcl_a_stop, lcl_b_start, lcl_b_stop,NULL,NULL)) {
                                /* SeqAlignDeleteSeqLocs(); */
                            }
                        }
                    }
                    if(salptmplast)
                        salptmplast->next=salptmp2;
                    else
                        salp = salptmp2;
                    salptmp2->next=NULL;
                    if(salpprev2) {
                        salpprev2->next=salptmp2next;
                    } else {
                        salp_leftover = salptmp2next;
                    }
            }else
                salpprev2=salptmp2;
        } else
            salpprev2=salptmp2;
    }
    if(salp_leftover_ptr)
        *salp_leftover_ptr = salp_leftover;
    return salp;
}







/*
   restrict overlapping of seqaligns up to Cov(Coverage) bases per positions,
   for Denseseg(blast2) only keep the hits on same strand as longest hit
   The result is resorted so that earlier segments are first.
   Also, Do not allow a query segment to be used twice.(e.g. repeats)
   Keep the query sub-alignment that is part of the longest segment.
   This is only valid for pairwise alignments.

   Should change the algorithm to assume that there is an overlapping
   region.. and to thus look for that.. e.g. 
   Sort SeqAligns by query-start.
   look for overlapping using start/stops 
   .. if it finds it.. it then uses that to merge the alignments.

   Merge overlapping alignments.. (also deals with non-identical alignments
   by )

   Second Pass Sort by alignment length,
   and keep non-overlapping ones.
   */







/*
  Function to count the number of mismatches in alignments
   Independent of mol type, works for both types of
   alignments.
   */

NLM_EXTERN Int4 LIBCALL SeqAlignListCountMismatches(SeqAlignPtr sap,BioseqPtr query_bsp, 
		     BioseqPtr subject_bsp,
		     Int4Ptr a_length, Int4Ptr mmatches, 
		     Int4Ptr mmismatches, Int4Ptr pmismatches_ptr,Int4Ptr gap_num){
  SeqAlignPtr align;
  FloatHi no_gap, gap;
  Int4 mismatches=0;

  if(a_length)
      *a_length=0;
  if(mmatches)
      *mmatches=0;
  if(mmismatches)
      *mmismatches=0;
  if(pmismatches_ptr)
      *pmismatches_ptr=0;
  if(gap_num)
      *gap_num=0;

  for(align = sap; align != NULL; align = align->next) {
      Int4 this_mismatch=0,this_match=0,this_alength=0,this_gap_num=0;
      Int4 this_pmismatches=0;
      Int4 astart=-1,astop=-1,bstart=-1,bstop=-1;
      if(!SeqAlignStartStop(&astart, &astop, &bstart, &bstop, align, query_bsp, subject_bsp)) {
	  ErrPostEx(SEV_WARNING,0,0,"problems computing alignment min/max for 1 sub-seqalign\n");
	  continue;
      } else if (!SeqAlignSimpleStats(align, query_bsp, subject_bsp, 
				&no_gap, &gap,
				&this_alength, 
				&this_match, &this_mismatch, &this_pmismatches,
				&this_gap_num)) {
	    ErrPostEx(SEV_WARNING,0,0,"problems computing alignment statistics\n");
	    continue;
      }
      if(a_length)
	  *a_length+=this_alength;
      if(mmatches) 
	  *mmatches+=this_match;
      mismatches+=this_mismatch;
      if(gap_num)
	  *gap_num+=this_gap_num;
      if(pmismatches_ptr)
          *pmismatches_ptr +=this_pmismatches;
  }
  if(mmismatches)
      *mmismatches = mismatches;
  return mismatches;
}

/*
  Function to count the number of mismatches in alignments
   Independent of mol type, works for both types of
   alignments.
   */

NLM_EXTERN Int4 LIBCALL SeqAlignCountMismatches(SeqAlignPtr sap,BioseqPtr query_bsp, 
		     BioseqPtr subject_bsp,
		     Int4Ptr a_length, Int4Ptr mmatches, 
		     Int4Ptr mmismatches, Int4Ptr pmismatches_ptr,Int4Ptr gap_num){
  FloatHi no_gap, gap;
  Int4 mismatches=0;
  if(a_length)
      *a_length=0;
  if(mmatches)
      *mmatches=0;
  if(mmismatches)
      *mmismatches=0;
  if(pmismatches_ptr)
      *pmismatches_ptr=0;
  if(gap_num)
      *gap_num=0;

  if (sap) {
      Int4 this_mismatch=0,this_match=0,this_alength=0,this_gap_num=0;
      Int4 this_pmismatches=0;
      Int4 astart=-1,astop=-1,bstart=-1,bstop=-1;
      if(!SeqAlignStartStop(&astart, &astop, &bstart, &bstop, sap, query_bsp, subject_bsp)) {
	  ErrPostEx(SEV_WARNING,0,0,"problems computing alignment min/max for 1 sub-seqalign\n");
      } else if (!SeqAlignSimpleStats(sap, query_bsp, subject_bsp, 
				&no_gap, &gap,
				&this_alength, 
				&this_match, &this_mismatch, &this_pmismatches,
				&this_gap_num)) {
	    ErrPostEx(SEV_WARNING,0,0,"problems computing alignment statistics\n");
      }
      if(a_length)
	  *a_length=this_alength;
      if(mmatches) 
	  *mmatches=this_match;
      mismatches=this_mismatch;
      if(gap_num)
	  *gap_num=this_gap_num;
      if(pmismatches_ptr)
          *pmismatches_ptr =this_pmismatches;
  }
  if(mmismatches)
      *mmismatches = mismatches;
  return mismatches;
}

/* Find length of Longest Segment for a pairwise SeqAlign */

NLM_EXTERN Int4 LIBCALL SeqAlignLongestSegment(SeqAlignPtr sap,Int4Ptr max_len,Int4Ptr max_qstart,Uint1Ptr max_qstrand,Int4Ptr max_sstart,Uint1Ptr max_sstrand) {
  Int4 j,a_order,b_order,ipos,jpos;
  Int4 qpos,spos,qposl=0,sposl=0;
  Int4 maxlen=-1;
  Uint1 sstrand=0,qstrand=0;
  if(sap==NULL)
    return 0;
  {
      BioseqPtr query_bsp=NULL,subject_bsp=NULL;
      SeqAlignBioseqsOrder(sap,query_bsp,subject_bsp,&a_order,&b_order);
      if(sap->segtype==1) {
          DenseDiagPtr ddp;
          ddp = (DenseDiagPtr) (sap->segs);
          while(ddp!=NULL) {
              qpos=ddp->starts[a_order];
              spos=ddp->starts[b_order];
              if(qpos!=-1 && spos!=-1 ) {
                  if(ddp->len>maxlen) { 
                      maxlen=ddp->len;
                      qposl=qpos;
                      sposl=spos;
                      qstrand=ddp->strands[a_order];
                      sstrand=ddp->strands[b_order];
                  }
              }
              ddp = ddp->next;
          }
      } else if(sap->segtype==2) {
          DenseSegPtr dsp;
          dsp = (DenseSegPtr) sap->segs;
          if(dsp!=NULL) {
              for(j = 0; j<dsp->numseg; ++j) {
                  ipos = 2*j + a_order;
                  qpos=dsp->starts[ipos];
                  jpos = 2*j + b_order;
                  spos=dsp->starts[jpos];
                  if(qpos!=-1 && spos!=-1 ) {
                      if(dsp->lens[j]>maxlen) {	      
                          maxlen=dsp->lens[j];
                          qposl=qpos;
                          sposl=spos;
                          qstrand=dsp->strands[ipos];
                          sstrand=dsp->strands[jpos];
                      }
                  }
              }
          }
      }
      sap=sap->next;
  }
  if(max_len)
      *max_len=maxlen;
  if(max_qstart)
      *max_qstart=qposl;
  if(max_sstart)
      *max_sstart=sposl;
  if(max_qstrand)
      *max_qstrand=qstrand;
  if(max_sstrand)
      *max_sstrand=sstrand;
 
  if(maxlen==-1)
      return -2;
  else
      return 0;

}

NLM_EXTERN Int4 LIBCALL SeqAlignListLongestSegment(SeqAlignPtr sap,Int4Ptr max_len,Int4Ptr max_qstart,Uint1Ptr max_qstrand,Int4Ptr max_sstart,Uint1Ptr max_sstrand) {
    Int4 maxlen;
    if(!sap)
        return -1;
    else {
        Int4 lcl_max_sstart,lcl_max_qstart;
        Int4 lcl_max_len;
        Uint1 lcl_max_qstrand,lcl_max_sstrand;
        maxlen=-1;
        while(sap) {
            if(SeqAlignLongestSegment(sap,&lcl_max_len,&lcl_max_qstart,&lcl_max_qstrand,&lcl_max_sstart,&lcl_max_sstrand)==0) {
                if(lcl_max_len >=maxlen) {
                    maxlen=lcl_max_len;
                    if(max_len)
                        *max_len = maxlen;
                    if(max_qstart)
                        *max_qstart = lcl_max_qstart;
                    if(max_sstart)
                        *max_sstart=lcl_max_sstart;
                    if(max_qstrand)
                        *max_qstrand=lcl_max_qstrand;
                    if(max_sstrand)
                        *max_sstrand=lcl_max_qstrand;
                }
            }
            sap=sap->next;
        }
    }
    if(maxlen==-1)
        return -1;
    else
        return 0;
}

/* 
   Returns the possible overlapping length between two sequences
   according to the "phase" of the longest (ungapped) alignment
   segment
   */

NLM_EXTERN Int4 LIBCALL SeqAlignOverlapLen(SeqAlignPtr sap,BioseqPtr query_bsp,BioseqPtr subject_bsp) {
  Int4 Overlap;
  Int4 qposl,sposl;
  Int4 maxlen=-1,lens,lenq;
  Int4 diag;
  Uint1 sstrand,qstrand;

  if(sap==NULL || query_bsp==NULL || subject_bsp==NULL) {
      return 0;
  }

  if(SeqAlignLongestSegment(sap,&maxlen,&qposl,&qstrand,&sposl,&sstrand)!=0)
      return 0;
  if(maxlen==-1)
      return 0;
  lenq=query_bsp->length;
  lens=subject_bsp->length;

  /* Find intersections using simple plane geometry and a
     few shortcuts
     */  
  if(qstrand==Seq_strand_minus) 
      qposl+=(maxlen-1);
  if(sstrand==Seq_strand_minus) 
      sposl+=(maxlen-1);

  if(qstrand==sstrand || (qstrand!=Seq_strand_minus && sstrand !=Seq_strand_minus)) {
      diag = qposl-sposl;
      if(diag>0) {
          Overlap = lenq-diag;
          if(Overlap>lens) 
              Overlap= lens;
      } else {
          Overlap = lens+diag;
          if(Overlap>lenq) 
              Overlap= lenq;
      }
  } else { /* Opposite strands */
      Int4 Intercept,Interceptl;
      diag=qposl+sposl;
      /* posA+posB = diag = constant 
         
       */
      Overlap = diag+1;
      if(lenq<=diag)
          Overlap -= (diag-lenq+1);
      if(lens<=diag)
          Overlap -= (diag-lens+1);
  }
  if(Overlap<0 || Overlap>lenq || Overlap>lens) {
    ErrPostEx(SEV_ERROR,0,0,"Overlap may have bugs!\n");
    if (Overlap<0) {
      Overlap=0;
    }
    if (Overlap>lenq)
      Overlap=lenq;
    if (Overlap>lens)
      Overlap=lens;
  }
  return Overlap;
}


/* Find the SeqAlignOverlap for a SeqAlign obtained from aligning
   two sequence Intervals .. The Overlap is computed
   according to the query&subject positions
   of the longest ungapped block.
   */

NLM_EXTERN Int4 LIBCALL SeqAlignOverlapLenByLoc(SeqAlignPtr sap,SeqLocPtr query_slp,SeqLocPtr subject_slp) {
  Int4 Overlap;
  Int4 qposl,sposl;
  Int4 maxlen=-1;
  Int4 diag;
  Int4 qmax,smax,qmin,smin;

  Uint1 sstrand,qstrand;

  if(sap==NULL || query_slp==NULL || subject_slp==NULL) {
    return 0;
  }

  if(SeqAlignLongestSegment(sap,&maxlen,&qposl,&qstrand,&sposl,&sstrand)!=0)
      return 0;

  if(maxlen==-1) 
    return 0;
  
  /* Set Up Window Coordinated. */
  qmax = SeqLocStop(query_slp);
  qmin = SeqLocStart(query_slp);

  smax = SeqLocStop(subject_slp);
  smin = SeqLocStart(subject_slp);
  /*
    Correct Positions For Minus Strand:
    SeqAlign Starts are always lowest Coord of block, even on Minus strand 
    */
  if(qstrand==Seq_strand_minus) 
      qposl+=(maxlen-1);
  if(sstrand==Seq_strand_minus) 
      sposl+=(maxlen-1);

  if(qstrand==sstrand) {
    diag = qposl-sposl;
    if(diag>0) {
        Overlap = qmax-diag +1;
        if(Overlap>smax) 
            Overlap= smax;
        Overlap -= smin;
    } else {
        Overlap = smax+diag +1;
        if(Overlap>qmax) 
            Overlap= qmax;
        Overlap -=qmin;
    }
    Overlap+=1;
  } else { /* Opposite strands */
      Int4 Intercept,Interceptl;
    diag=qposl+sposl;
    /* posA+posB = diag = constant 
       
       */
    Intercept = diag-smin;
    if(Intercept>qmax)
        Intercept=qmax;
    Interceptl = diag-smax;
    if(qmin>Interceptl)
        Intercept-=qmin;
    else
        Intercept-=Interceptl;

    Overlap=Intercept;
  }
  if(Overlap<0)
      Overlap=0;

  return Overlap;
}



/* Returns the percentage of the sequence that matched (number of matches)
   divided by the length that could have aligned.
   Given the relative offset of the longest segment of the seqalign.
   It is maxed out at 100%.
   */

NLM_EXTERN FloatHi LIBCALL SeqAlignAlignability(SeqAlignPtr sap,BioseqPtr query_bsp, BioseqPtr subject_bsp, Int4Ptr Overlap_len,Int4Ptr aln_len,Int4Ptr virtual_mismatches,Int4Ptr mismatches,Int4Ptr gap_num,Int4Ptr matches) {
  FloatHi alignability=0.0;
  Int4 overlap,lcl_matches;
  if(sap!=NULL) {
      SeqAlignCountMismatches(sap,query_bsp, subject_bsp,
                      aln_len, &lcl_matches, 
                      mismatches, NULL,gap_num);
      overlap=SeqAlignOverlapLen(sap,query_bsp,subject_bsp);
      if(matches)
          *matches = lcl_matches;
      if(overlap!=0) {
          alignability= (FloatHi) 100.0*(lcl_matches)/overlap;
          if(alignability>100.0) 
              alignability=100.0;
      } else {
          alignability= (FloatHi) 0.0;
      }
      if(virtual_mismatches)
          *virtual_mismatches = overlap-lcl_matches;
      if(Overlap_len)
          *Overlap_len=overlap;
  } else {
      if(aln_len)
          *aln_len=0;
      if(mismatches)
          *mismatches=0;
      if(matches)
          *matches=0;
      if(gap_num)
          *gap_num=0;
      if(Overlap_len)
          *Overlap_len=0;
      if(virtual_mismatches)
          *virtual_mismatches =0;
  }
  return alignability;
}

/* Assumes that the SeqAlign was obtained by aligning the two SeqLocs..
no check is made to verify that.
*/

NLM_EXTERN FloatHi LIBCALL AlignabilityByLoc(SeqAlignPtr sap,SeqLocPtr query_slp, SeqLocPtr subject_slp, Int4Ptr Overlap_len,Int4Ptr aln_len,Int4Ptr virtual_mismatches,Int4Ptr mismatches,Int4Ptr gap_num,Int4Ptr matches) {
  FloatHi alignability=0.0;
  Int4 overlap=0,lcl_matches;
  if(sap!=NULL) {
      BioseqPtr query_bsp,subject_bsp;
      query_bsp = BioseqLockById(SeqLocId(query_slp));
      subject_bsp = BioseqLockById(SeqLocId(subject_slp));
      SeqAlignCountMismatches(sap,query_bsp, subject_bsp,
                              aln_len, &lcl_matches, 
                              mismatches, NULL,gap_num);
      BioseqUnlock(query_bsp);
      BioseqUnlock(subject_bsp);
      overlap=SeqAlignOverlapLenByLoc(sap,query_slp,subject_slp);
      if(overlap!=0) {
          alignability= (FloatHi) 100.0*(lcl_matches)/overlap;
          if(alignability>100.0) 
              alignability=100.0;
      } else {
          alignability= (FloatHi) 0.0;
      }
  } else {
      if(aln_len)
          *aln_len=0;
      if(mismatches)
          *mismatches=0;
      if(gap_num)
          *gap_num=0;
      lcl_matches=0;
  }
  if(matches)
      *matches=lcl_matches;
  if(virtual_mismatches)
      *virtual_mismatches = overlap-lcl_matches;
  if(Overlap_len)
      *Overlap_len=overlap;
  return alignability;
}



/* Function to return a vector of Blast-Style Score pointers.
   Windows of size win_size
   starting at offset win_offset   (usually 0)
   Each window shifted by win_step (minimum of 1)
   Match = Match score for aligned residues (e.g. 1)
   mismatch = mismatch score for aligned but mismatching residues (e.g. -3)

   As input it requires the sizeof the SearchSpace. This is
   simply (sizeof(bsp1)-word_size)*(sizeof(database)-word_size*NumberOfsequencesInDatabase)
where word_size is the blast word-size (e.g. 11)
sizeof(database) == Number of residues in database



   This program assumes that Each DenseSeg Seqalign is contigous
   and non-overlapping (e.g. that no seqalign is a concatenation of
   two local alignments). Consecutive Seqaligns in the Chain
   don't have to be contigous. This Subroutine does not try
   to resolve segments scores between two different seqaligns.

   Gaps on the edges of each window are not scored.

   if(matrix==NULL) ==> Only searches for longest segment of perfect matches;

   returns a vector of blast scores per window, and the number of windows.

   and the longest perfect segment and it's
   start position (in 0-offset)(perfect_len ==0 if there are perfect blocks)
*/

NLM_EXTERN FloatHi PNTR LIBCALL SeqAlignWindowScoreFN(SeqAlignPtr align, BioseqPtr bsp1, BioseqPtr bsp2,Int4 win_size, Int4 win_offset,Int4 win_step,Int4Ptr PNTR matrix,Int4 match, Int4 g_open, Int4 g_ext, Int4Ptr nwin,Int4Ptr perfect_length,Int4Ptr perfect_position) {
  DenseSegPtr dsp;
  Int4 lcl_nwin;
  Int4 start_1, start_2,offset_1, offset_2;
  SeqPortPtr spp_1,spp_2;
  Boolean is_gap,FirstBase;
  Int4 a_len,b_len;
  Int4 i, j,len,pos,last_pos,last_start,i_win,win_end;
  Int4 gap_num=0,N_prev_gaps;
  Int4 score,win_score,gap_score;
  FloatHiPtr score_vector=NULL;
  Int4Ptr residue_score=NULL,qgaps_score=NULL;
  Int4Ptr gbl_score;
  Int4 perfect_len, perfect_pos,perfect=0;

  Uint1Ptr strand;
  Int4 a_order,b_order;
  Uint1 code1,code2;
  Uint1 res_1, res_2;

  if(align==NULL || align->segs==NULL) {
      return NULL;
  }
  if(align->segtype !=2 ) {
      ErrPostEx(SEV_ERROR,0,0,"SeqAlignWindowScore: unsupported SeqAlign Type %d\n",align->segtype);
      return NULL;
  }
  if(g_open<0 || g_ext<0) { /* are positive "penalties" */
      ErrPostEx(SEV_WARNING,0,0,"SeqAlignWindowScore: Gap penalties must be positive\n");
      return NULL;
  }
  if(!bsp1 || !bsp2) {
      if(!bsp1)
          ErrPostEx(SEV_WARNING,0,0,"SeqAlignWindowFN: Query Bioseq is NULL! Not allowed\n");
      if(!bsp2)
          ErrPostEx(SEV_WARNING,0,0,"SeqAlignWindowFN: Subject Bioseq is NULL! Not allowed\n");
      return NULL;
  }
  
  if((bsp1->mol == Seq_mol_aa))
      code1 = Seq_code_ncbieaa;
  else
      code1 = Seq_code_ncbi4na;
  if((bsp2->mol == Seq_mol_aa))
      code2 = Seq_code_ncbieaa;
  else
      code2 = Seq_code_ncbi4na;

  if(win_step<=0)
      win_step=1;
  
  a_len = bsp1->length;
  b_len = bsp2->length;
  
  if(win_offset>=a_len)
      return NULL;
  lcl_nwin = (a_len-win_offset+1-win_size)/win_step;
  if(lcl_nwin*win_step+win_size-1<a_len)
      lcl_nwin++;
  /* Keep best Global scores for windows.. for separate SeqAligns */
  
  gbl_score = (Int4Ptr) Malloc((lcl_nwin+1)*sizeof(Int4));
  residue_score = (Int4Ptr) Malloc((a_len+1)*sizeof(Int4));
  /* To deal with gap_extensions in the subject sequence .. */
  qgaps_score = (Int4Ptr) Malloc((a_len+1)*sizeof(Int4)); 
  for(i=0;i<lcl_nwin;i++)
      gbl_score[i]=INT4_MIN;

  perfect_len=0;
  perfect_pos=-1;

  if (align!=NULL) {
      for(i=0;i<a_len;i++) {
          residue_score[i]=INT4_MIN;
          qgaps_score[i]=0;
      }
      SeqAlignBioseqsOrder(align,bsp1,bsp2 ,&a_order, &b_order);
      if(align->segtype==2) {
          dsp=(DenseSegPtr) align->segs;
          strand = dsp->strands;          
          spp_1 = SeqPortNew(bsp1, 0, -1, strand==NULL ? ((Uint1) 0) : strand[a_order], code1);
          spp_2 = SeqPortNew(bsp2, 0, -1, strand==NULL ? ((Uint1)0) : strand[b_order], code2);
          last_pos = 0;
          perfect=0;
          for(i =0; i<dsp->numseg; ++i) {
              Int4 ipos,jpos;
              ipos = 2*i+a_order;
              jpos = 2*i+b_order;
              start_1 = dsp->starts[ipos];
              start_2 = dsp->starts[jpos];
              is_gap = ((start_1 == -1 ) || (start_2 == -1));
              len = dsp->lens[i];
              if(start_1!=-1) {
                  if(strand && strand[ipos] == Seq_strand_minus) {
                      offset_1 = a_len - (start_1+len);
                  } else {
                      offset_1 = start_1;
                  }
                  SeqPortSeek(spp_1, offset_1, SEEK_SET);
              }
              if(start_2 !=-1) {
                  if(strand && strand[jpos] == Seq_strand_minus) {
                      offset_2 = b_len - (start_2+len);
                  } else {
                      offset_2 = start_2;
                  }
                  SeqPortSeek(spp_2, offset_2, SEEK_SET);
              }
              if(is_gap){
                  if(start_1 == -1) {
                      res_1='-';
                      for(j=0;j<len;j++) {
                          res_2 =  SeqPortGetResidue(spp_2);
                      }
                      qgaps_score[last_pos]=-(g_open+len*g_ext);
                  } else { /* if(start_2 == -1) */
                      res_2= '-';
                      for(j=0;j<len;j++) {
                          res_1 =  SeqPortGetResidue(spp_1);
                          pos = start_1+j;
                          residue_score[pos] = INT4_MIN/2;
                      }
                  }
                  ++gap_num;
              } else { /* no gaps */
                  perfect=0;
                  for(j =0,pos=start_1; j<len; ++j,pos++) {
                      res_1 = SeqPortGetResidue(spp_1);
                      res_2 = SeqPortGetResidue(spp_2);
                      if(matrix)
                          score = matrix[res_1][res_2];
                      if(residue_score[pos]==INT4_MIN) {
                          residue_score[pos]=score;
                      } else {
                          ErrPostEx(SEV_ERROR,0,0,"Invalid non-local Seq-Align cannot be used with this routine\n");
                          Free(qgaps_score);
                          Free(residue_score);
                          Free(gbl_score);
                          return NULL;
                      }
                      if(res_1==res_2) {
                          perfect++;
                      } else {
                          if(perfect && perfect>perfect_len) {
                              perfect_len = perfect;
                              perfect_pos = start_1+j-perfect;
                          }
                          perfect=0;
                      }
                  }
                  pos=start_1+len-1;
                  if(perfect && perfect>perfect_len) {
                      perfect_len = perfect;
                      perfect_pos = pos-perfect+1;
                  }
              }
              last_pos = pos;
          }
          if(matrix) {
              last_start = a_len-win_size+win_step;
              /* Allow last window to be partial to allow complete coverage of query */
              for(i=win_offset;i<last_start ;i+=win_step) {
                  i_win = (i-win_offset)/win_step;
                  win_end = i+win_size;
                  if(win_end>a_len)
                      win_end=a_len;
                  FirstBase = FALSE;
                  N_prev_gaps = 0;
                  win_score =0;
                  for(j=i;j<win_end;j++) {
                      score = residue_score[j];
                      if(score==(INT4_MIN/2)) { /* gaps */
                          if(FirstBase) {
                              if(N_prev_gaps==0) /* Internal gap */
                                  win_score-=g_open;
                              win_score-=g_ext;
                              N_prev_gaps++;
                          }
                      } else if(score==INT4_MIN){ /* no Hit on that base */
                          /***commented out by UP
                            if(N_prev_gaps>0)  Internal Hole 
                            win_score += (g_open+N_prev_gaps*g_ext); 
                            ***/
                          N_prev_gaps=0;
                          FirstBase=FALSE;
                      } else {
                          N_prev_gaps=0;
                          FirstBase=TRUE;
                          win_score+=score;
                          /* gaps in query .. not allowed on ends. */
                          if((j<(win_end-1)) && residue_score[j+1]>INT4_MIN/2) {
                              gap_score = qgaps_score[j];
                              win_score += gap_score;
                          }
                      }
                  }
                  if(N_prev_gaps>0) /* reject end gaps */
                      win_score +=(g_open+N_prev_gaps*g_ext);
                  
                  if(win_score>gbl_score[i_win]) /* Keep Best Score */
                      gbl_score[i_win]=win_score;
              }
          }
          SeqPortFree(spp_1);
          SeqPortFree(spp_2);
      }
      /*       align = align->next; */
  }
  if(nwin)
      *nwin = lcl_nwin;
  if(perfect_position)
      *perfect_position = perfect_pos;
  if(perfect_length)
      *perfect_length = perfect_len;
  Free(qgaps_score);
  Free(residue_score);
  if(matrix) {
      score_vector = (FloatHiPtr) Malloc(lcl_nwin*sizeof(FloatHi));
      for(i=0;i<lcl_nwin;i++) {
          score_vector[i]=gbl_score[i];
      }
  }
  Free(gbl_score);
  return score_vector;
}

/* Returns a vector of FloatHi scores
   corresponding to windows on the sequences.
   The Scores are a match/mismatch system 
   also returns the length and the position of the first longest block
   of perfect identity
*/
NLM_EXTERN FloatHi PNTR LIBCALL SeqAlignWindowScore(SeqAlignPtr align, BioseqPtr bsp1, BioseqPtr bsp2,Int4 win_size, Int4 win_offset,Int4 win_step, Int4Ptr PNTR matrix, Int4 match, Int4 mismatch, Int4 g_open, Int4 g_ext, Int4Ptr nwin,Int4Ptr perfect_length,Int4Ptr perfect_position) {
    Boolean na1,na2;
    BioseqPtr bsp_1,bsp_2;
    Int4Ptr PNTR this_matrix=NULL;
    FloatHiPtr scoreptr;
    
    if(align==NULL)
        return NULL;
    if(bsp1) {
        bsp_1 = bsp1;
    } else {
        SeqIdPtr sip ;
        sip = SeqIdPtrFromSeqAlign(align);
        if(sip) {
            bsp_1 = BioseqLockById(sip);
        } else {
            ErrPostEx(SEV_WARNING,0,0,"Could not lock query Bioseq by ID from SeqAlign\n");
            return NULL;
        }
    }
    if(bsp_1) {
        na1 = ISA_na(bsp_1->mol);
    } else {
        ErrPostEx(SEV_WARNING,0,0,"NULL Bioseq\n");
        return NULL;
    }
    if(bsp2) {
        bsp_2 = bsp2;
    } else {
        SeqIdPtr sip ;
        sip = SeqIdPtrFromSeqAlign(align);
        if(sip && sip->next) {
            bsp_2 = BioseqLockById(sip);
        } else {
            ErrPostEx(SEV_WARNING,0,0,"Could not lock subject Bioseq by ID from SeqAlign\n");
            return NULL;
        }
    }
    if(bsp_2) {
        na2 = ISA_na(bsp_2->mol);
    } else {
        ErrPostEx(SEV_WARNING,0,0,"NULL Bioseq\n");
        return NULL;
    }
    if(na1 && na2) {/* blastn or tblastx*/
        /* XXX I'm not sure how to score tblastx alignments.. for
           for now assume it's the same as dna alignments */
        if(matrix==NULL)
            this_matrix = DNAScoringMatrix(mismatch, match,16);
        else
            this_matrix=matrix;
    } else if (!na1 && !na2) {/* blastp */
        if(matrix==NULL) {
            ErrPostEx(SEV_WARNING,0,0,"SeqAlignWindowScore: Protein Alignment Stats requires Matrix for Seq_code_ncbieaa\n");
        } else
            this_matrix=matrix;
    } else if (na1 && !na2) { /* blastx */
        if(matrix==NULL) {
            ErrPostEx(SEV_WARNING,0,0,"blastx Matrix Creation not implemented\n");
        } else
            this_matrix=matrix;
    } else { /* tblastn */
        if(matrix==NULL) {
            ErrPostEx(SEV_WARNING,0,0,"tblastn Matrix Creation not implemented\n");
        } else
            this_matrix=matrix;
    }
    if(this_matrix!=NULL) {
        scoreptr = SeqAlignWindowScoreFN(align, bsp_1, bsp_2, win_size,win_offset,win_step,this_matrix,match,g_open, g_ext, nwin,perfect_length,perfect_position);
        if(matrix==NULL)
            Free(this_matrix);
    }
    if(!bsp1)
        BioseqUnlock(bsp_1);
    if(!bsp2)
        BioseqUnlock(bsp_2);
    return scoreptr;
}



/* Take a blast-style Linked list of SeqAlign and Make a Linked
   list of SeqAlignSets(type 5) .. where each Node is the start of an
   "orthogonal" SeqAlign Linked list of seqaligns referring to the same
   SeqId pairs.
   */

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignSameIdMerge(SeqAlignPtr salp) {
    SeqAlignPtr seqalign=NULL;
    SeqAlignPtr salpnext,salp_prev;
    SeqIdPtr sip,sipnext;
    Int4 n_salp;
    Uint1 sameq,sames;
    if(!salp)
        return NULL;
    salpnext= salp->next;
    salp->next=NULL;
    if(salpnext==NULL)
        return salp;
    while(salpnext!=NULL) {
        n_salp=0;
        sip =SeqIdPtrFromSeqAlign(salp);
        sipnext = SeqIdPtrFromSeqAlign(salpnext);
        sameq = SeqIdComp(sip,sipnext);

        while(sameq==SIC_YES && salp!=NULL) {
            SeqAlignPtr head=salp;
            SeqAlignPtr prev=salp;

            sames = SeqIdComp(sip->next,sipnext->next);
            while(sameq==SIC_YES && sames==SIC_YES && salpnext!=NULL) {
                n_salp++;
                prev->next = salpnext;
                prev = salpnext;
                salpnext=salpnext->next;
                prev->next=NULL;
                if(salpnext!=NULL) {
                    sipnext = SeqIdPtrFromSeqAlign(salpnext);
                    sameq = SeqIdComp(sip,sipnext);
                    sames = SeqIdComp(sip->next,sipnext->next);
                } else {
                    sameq=SIC_DIFF; 
                    sames=SIC_DIFF;
                }
            }
            if(seqalign==NULL) {
                    seqalign=SeqAlignNew();
                    seqalign->segtype = 5;
                    seqalign->segs = head;
                salp_prev = seqalign;
            } else {
                    salp_prev->next = SeqAlignNew();
                    salp_prev->next->segtype=5;
                    salp_prev->next->segs = head;
                salp_prev = salp_prev->next;
            }
            salp=salpnext;
            if(salp) {
                salpnext=salp->next;
                salp->next=NULL;
            } else {
                salpnext=NULL;
            }
            sip =SeqIdPtrFromSeqAlign(salp);
            sipnext = SeqIdPtrFromSeqAlign(salpnext);
            sameq = SeqIdComp(sip,sipnext);
        }
        if(n_salp==0) {
            if(seqalign==NULL) {
                seqalign=SeqAlignNew();
                seqalign->segtype = 5;
                seqalign->segs = salp;
            } else {
                salp_prev->next = SeqAlignNew();
                salp_prev->next->segtype=5;
                salp_prev->next->segs = salp;
            }
        }
    }
    return seqalign;
}

NLM_EXTERN SalpStatsInputBlockPtr LIBCALL SalpStatsInputBlockNew(void) {
    return (SalpStatsInputBlockPtr) MemNew(sizeof(SalpStatsInputBlock));
}
NLM_EXTERN SalpStatsInputBlockPtr LIBCALL SalpStatsInputBlockFree(SalpStatsInputBlockPtr param) {
    return (SalpStatsInputBlockPtr) MemFree(param);
}

NLM_EXTERN SalpStatsResultsPtr LIBCALL SalpStatsResultsNew(void) {
    return (SalpStatsResultsPtr) MemNew(sizeof(SalpStatsResults));
}
NLM_EXTERN SalpStatsResultsPtr LIBCALL SalpStatsResultsFree(SalpStatsResultsPtr results) {
    if(!results)
        return NULL;
    if(results->gaplengths)
        Free(results->gaplengths);
    if(results->query_sip)
	SeqIdFree(results->query_sip);
    if(results->subject_sip)
        SeqIdFree(results->subject_sip);
    Free(results);
    return (SalpStatsResultsPtr) NULL;
}


NLM_EXTERN SalpStatsResultsPtr LIBCALL SeqAlignStats(SeqAlignPtr salp_in, SalpStatsInputBlockPtr param,BioseqPtr query_bsp,BioseqPtr subject_bsp) {
    Boolean Duplicated=FALSE;
    SeqAlignPtr salp=NULL,sapnext=NULL;
    SalpStatsResultsPtr results;
    SeqIdPtr sip;
    Int4 a_length;
    Boolean lock_qbsp=FALSE,lock_sbsp=FALSE;

    if(!salp_in || !param)
        return NULL;
    if(salp_in->segtype == 5) {
        salp_in=(SeqAlignPtr) (salp_in->segs);
	 } else {
        sapnext=salp_in->next;
	     salp_in->next=NULL;
    }
    salp = salp_in;
    results = SalpStatsResultsNew();
    sip =SeqIdPtrFromSeqAlign(salp);
    results->query_sip = SeqIdDup(sip);
    if(results->query_sip)
        results->query_sip->next=NULL;
    if(sip && sip->next){
        results->subject_sip = SeqIdDup(sip->next);
        if(results->subject_sip)
	    results->subject_sip->next=NULL;
    }
    if(!query_bsp && results->query_sip) {
        lock_qbsp=TRUE;
        query_bsp = BioseqLockById(sip);
    }
    if(query_bsp==NULL) {
        ErrPostEx(SEV_WARNING,0,0,"Could not Lock query Bioseq\n");
         
	 } else {
        results->inputqlength=query_bsp->length;
    }
        
    if(!subject_bsp && results->subject_sip) {
        lock_sbsp=TRUE;
        subject_bsp = BioseqLockById(results->subject_sip);
    }
    if(subject_bsp==NULL) {
        ErrPostEx(SEV_WARNING,0,0,"Could not Lock query Bioseq\n");
    } else {
        results->inputslength=subject_bsp->length;
    }
    SeqAlignListStartStop(&(results->qbegmatch),&(results->qendmatch),&(results->sbegmatch),&(results->sendmatch),salp, query_bsp,subject_bsp);
    
    if(param->Mismatch) {
        SeqAlignListCountMismatches(salp,query_bsp, 
		     subject_bsp,
		     &(a_length), &(results->identities),
		     &(results->mismatches), &(results->partial_matches),&(results->gapcount));
    }
    if(param->GapList) {
        results->gaplengths = SeqAlignListGapList(salp,&(results->gapcount));
    }
    if(param->LongestPerfect || param->WorstWindow) {
        FloatHiPtr windows;
        FloatHi worst;
        Int4 nwin,i;
        windows= SeqAlignWindowScore(salp, query_bsp,subject_bsp,param->window_size,0,1, NULL, 1,/* match */
				-3,/* mismatch */
				  5,2 /* gap opening, gap extension penalties */
					, &nwin,param->LongestPerfect ? &(results->longest_perfect) : NULL,NULL);
        if(param->WorstWindow) {
            if(windows && nwin) {
                worst=windows[0];
                for(i=1;i<nwin;i++) {
                    if(worst>windows[i])
                        worst = windows[i];
                }
                results->worst_window = (Int4)worst;
            } else {
                results->worst_window = INT_MIN;
            }
        }
        Free(windows);
    }
    if(param->BlastScores) {
        SeqAlignScoreRead(salp,&(results->score),&(results->bit_score),&(results->E_value));
    }
    
    results->q_possibleOverlap = SeqAlignOverlapLen(salp,query_bsp,subject_bsp);
    results->s_possibleOverlap =    results->q_possibleOverlap;
    if(query_bsp->mol ==Seq_mol_aa)
            results->sub_reversed_strand = FALSE;
    else {
        Uint1 strand1,strand2;
        strand1 = SeqAlignStrand(salp,0);
        strand2 = SeqAlignStrand(salp,1);
        results->sub_reversed_strand = ((strand1==Seq_strand_minus && strand2!=strand1) || (strand2==Seq_strand_minus && strand2!=strand1));
    }

    if(lock_qbsp)
        BioseqUnlock(query_bsp);
    if(lock_sbsp)
        BioseqUnlock(subject_bsp);
    if(Duplicated)
        salp=SeqAlignSetFree(salp);
    if(sapnext) {/* Relink */
       salp_in->next = sapnext;
    }
    return results;
}

/*
  Find Pairwise Statistics for a linked list of SeqAlign.
  Consider that the linked list represents a Global Alignment..
  and choose the best set of segments that is consistent
  with this. See SeqAlignStats for options.
 */

NLM_EXTERN SalpStatsResultsPtr LIBCALL SeqAlignListGlobalStats(SeqAlignPtr salp_in, SalpStatsInputBlockPtr param,BioseqPtr query_bsp,BioseqPtr subject_bsp) {
    SalpStatsResultsPtr ssrp;
    SeqAlignPtr salp;
    Boolean Duplicated=FALSE;
    if(!salp_in)
        return NULL;
    salp=salp_in;
    if(salp->next!=NULL) {
        SeqAlignPtr salp_leftover;
        Duplicated=TRUE;
        salp = SeqAlignDup(salp_in);
        if(param->MergeConsistentAlign) {
            salp = SeqAlignNonRedundant(salp,query_bsp,subject_bsp,&salp_leftover);
        } else {
            salp = SeqAlignNonOverlap(salp,query_bsp,subject_bsp,&salp_leftover,1);
        }
        SeqAlignSetFree(salp_leftover);
    }
    ssrp = SeqAlignStats(salp, param,query_bsp,subject_bsp);
    if(Duplicated)
        salp=SeqAlignSetFree(salp);
    return ssrp;
}

/* 
   SeqAlignGapCount: Count Gaps in a SeqAlign
 */
NLM_EXTERN Int4 LIBCALL SeqAlignGapCount (SeqAlignPtr salp)
{
  DenseSegPtr   dsp;
  Int4          sum,
                j;
  Int2          k;

  if (salp == NULL)
     return 0;
  if (salp->segtype != 2)
     return 0;
  sum=0;
  if (salp->segtype == 2)
  {
     dsp = (DenseSegPtr) salp->segs;
     if (dsp!=NULL)
     {
        for (j=0; j<dsp->numseg; j++)
           for (k=0; k<dsp->dim; k++)
           {
              if (dsp->starts[(dsp->dim*j)+k] < 0)
                 sum+= dsp->lens[j];
           }
     }
  }
  return sum;
}


/*
  Keep All SeqAligns that provide some new coverage
  new coverage is defined as.. contributing to the coverage of the
    first "Coverage" covering of the SeqAlign.

    Coverage==1: Means any alignment that contributes at least one new base coverage.

  when the SeqAligns are considered in the input Order. 
  (might want to sort by score or length before input)

  The rejected SeqAligns are returned.

  Consider that a Gap aligned to the query is "informative"
  
  If HitsPerBase !=NULL, 
  it returns the cumulative coverage upon exit.
  if *HitsPerBase !=NULL it takesthis as input for the already existing
  coverage.. So can call the routine with Different list of SeqAligns.

  if passing pointer last_a_start and last_a_stop.. first time around they
  must be initialized to *last_a_start == INT4_MAX and *last_a_stop =-1

   */

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignFilterByCoverage(SeqAlignPtr salp,BioseqPtr query_bsp,SeqAlignPtr PNTR salp_leftover_ptr,Int4 Coverage,Uint2Ptr PNTR HitsPerBase,Int4Ptr last_a_start,Int4Ptr last_a_stop) {
    SeqAlignPtr salptmp;
    Uint2Ptr HitsPerQBase;
    Uint1Ptr AcceptList;
    Int4 i,j,k,l;
    Int4 qpos,spos,qend_pos,send_pos;
    Int4 a_start,a_stop;
    DenseSegPtr dsp;
    DenseDiagPtr ddp;
    SeqAlignPtr align,new_salp,salp_leftover=NULL;
    SeqAlignPtr salp_left_tmp=NULL;
    SeqAlignPtr PNTR salp_list;

    Int4 a_order,b_order;

    Int4 ipos,n_sap;

    if (salp == NULL)
        return salp;
    if(Coverage>65535)
        Coverage = 65535;
    
    if(last_a_start!=NULL && *last_a_start != INT4_MAX)
        a_start = *last_a_start;
    else
        a_start = INT4_MAX;
    if(last_a_stop!=NULL && *last_a_stop >=0)
        a_stop = *last_a_stop;
    else
        a_stop=-1;
    for (n_sap=0,salptmp=salp; salptmp!=NULL; salptmp=salptmp->next) {
        Int4 lcl_a_start,lcl_a_stop,lcl_b_start,lcl_b_stop;
        n_sap++;
        if(SeqAlignStartStop(&lcl_a_start, &lcl_a_stop, &lcl_b_start, &lcl_b_stop,
                             salptmp, query_bsp, NULL)) {
            if(lcl_a_start!=-1 && lcl_a_start<a_start)
                a_start=lcl_a_start;
            if(lcl_a_stop>a_stop)
                a_stop=lcl_a_stop;
        }
    }

    salp_list=(SeqAlignPtr PNTR) Malloc((size_t)((n_sap+1)*(sizeof(salp_list[0]))));
    for (j=0,salptmp=salp; salptmp!=NULL; salptmp=salptmp->next,j++) {
        salp_list[j]=salptmp;
    }
    for (j=0; j<n_sap; j++) {
        salp_list[j]->next=NULL;
    }
    
    if(HitsPerBase!=NULL && *HitsPerBase!=NULL) {
        HitsPerQBase = *HitsPerBase;
        if(last_a_start!=NULL || last_a_stop!=NULL) {
            Int4 start_extra=0,stop_extra=0,extra=0;
            if(last_a_start!=NULL && *last_a_start>a_start)
                start_extra = *last_a_start-a_start;
            if(last_a_stop!=NULL && *last_a_stop<a_stop)
                stop_extra = a_stop-*last_a_stop;
            extra = start_extra+stop_extra;
            if(extra) {
                HitsPerQBase = Realloc(HitsPerQBase,(a_stop-a_start+2)*sizeof(Uint2));
                memmove(HitsPerQBase+start_extra,HitsPerQBase,(a_stop-a_start-extra+1)*sizeof(Uint2));
                memset(HitsPerQBase,0,start_extra*sizeof(Uint2));
                memset(HitsPerQBase+a_stop-a_start-stop_extra+1,0,(stop_extra+1)*sizeof(Uint2));
                *HitsPerBase = HitsPerQBase;
                if(last_a_start!=NULL)
                    *last_a_start = a_start;
                if(last_a_stop!=NULL)
                    *last_a_stop = a_stop;
            }
        }
    } else {
        HitsPerQBase = (Uint2Ptr) Calloc(a_stop-a_start+2,sizeof(Uint2));
        if(HitsPerBase)
            *HitsPerBase = HitsPerQBase;
        if(last_a_start!=NULL)
            *last_a_start=a_start;

        if(last_a_stop!=NULL)
            *last_a_stop=a_stop;
    }
    AcceptList = (Uint1Ptr) Calloc(n_sap+1,sizeof(Uint1));

    /* Loop over length-ordered Alignments: Keep Best Non-overlappping regions
       
     */
    for(i=0;i<n_sap;i++) {
        align=salp_list[i];
        if(align!=NULL) {
            SeqAlignBioseqsOrder(align,query_bsp,NULL,&a_order,&b_order);
            if(align->segtype==1) {
                ddp = (DenseDiagPtr) (align->segs);
                AcceptList[i]=0;
                while(ddp!=NULL && AcceptList[i]==0) {
                    qpos=ddp->starts[a_order];
                    spos=ddp->starts[b_order];
                    qend_pos=qpos+ddp->len;
                    send_pos=spos+ddp->len;
                    if(qpos!=-1) {
                        for(k=qpos,l=spos;k<qend_pos && l<send_pos;k++,l++) {
                            if(HitsPerQBase[k-a_start]<Coverage) {
                                AcceptList[i]=1;
                                qpos+=qend_pos;
                            }
                        }
                    }
                    ddp = ddp->next;
                }
            } else if(align->segtype==2) {
                dsp = (DenseSegPtr) align->segs;
                if(dsp!=NULL) {
                    AcceptList[i]=0;
                    for(j = 0; j<dsp->numseg; ++j) {
                        ipos = 2*j + a_order;
                        qpos=dsp->starts[ipos];
                        qend_pos=qpos+dsp->lens[j];
                        {
                            if(qpos!=-1) {
                                for(k=qpos;k<qend_pos;k++) {
                                    if((HitsPerQBase[k-a_start]<Coverage)) {
                                        AcceptList[i]=1;
                                        k+=qend_pos; 
                                        j+=dsp->numseg;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            /* If we've accepted the new segment(s), 
            */
            if(AcceptList[i]==1) {/* increment B-value counter for bases */
                if(align->segtype==1) {
                    ddp=(DenseDiagPtr)align->segs;
                    while(ddp!=NULL) {
                        qpos=ddp->starts[a_order];
                        qend_pos=qpos+ddp->len;
                        if(qpos!=-1) {
                            for(k=qpos;k<qend_pos;k++) {
                                if(HitsPerQBase[k-a_start]<65535)
                                    HitsPerQBase[k-a_start]+=1;
                            }
                        }
                        ddp = ddp->next;
                    }
                } else if(align->segtype==2) {
                    dsp = (DenseSegPtr) align->segs;
                    if(dsp!=NULL) {
                        for(j = 0; j<dsp->numseg; ++j) {
                            ipos = 2*j + a_order;
                            qpos=dsp->starts[ipos];
                            qend_pos=qpos+dsp->lens[j];
                            if(qpos!=-1) {
                                for(k=qpos;k<qend_pos;k++) {
                                    if(HitsPerQBase[k-a_start]<65535)
                                       HitsPerQBase[k-a_start]+=1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    /* Shift down list */
    salp=new_salp=NULL;
    for(i=0;i<n_sap;i++) {
        salptmp=salp_list[i];
        if (salptmp==NULL)
            break;
        if(AcceptList[i]==0) {
            if(salp_leftover_ptr) {
                if(salp_leftover) {
                    salp_left_tmp->next=salptmp;
                } else {
                    salp_leftover=salptmp;
                }
                salp_left_tmp=salptmp;
            } else {
                salptmp->next=NULL;
                salp_list[i] = SeqAlignFree (salptmp);
            }
        } else {
            if(salp)
                salp->next=salptmp;
            else {
                salp=salptmp;
                new_salp=salp;
            }
            salp=salptmp;
        }
    }
    if(salp)
        salp->next=NULL;
    if(salp_left_tmp) 
        salp_left_tmp->next=NULL;
    if(salp_leftover_ptr)
        *salp_leftover_ptr=salp_leftover;
    Free(AcceptList);
    if(!HitsPerBase)
        Free(HitsPerQBase);
    Free(salp_list);
    return new_salp;
}
