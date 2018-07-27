/*   salsap.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  salsap.c
*
* Author:  Colombe Chappey
*
* Version Creation Date:   1/27/96
*
* $Revision: 6.9 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/
#include <salsap.h>
#include <salpedit.h>
#include <salutil.h>
#include <sqnutils.h>
#include <edutil.h>
#include <objmgr.h>
#include <seqmgr.h>

NLM_EXTERN SeqIdPtr LIBCALL SeqAlignIDList (SeqAlignPtr salp)
{
    return SeqIdPtrFromSeqAlign(salp);
}

/* Used in salsa.c once */
NLM_EXTERN Boolean LIBCALL FindSeqIdinSeqAlign (SeqAlignPtr salphead, SeqIdPtr sip)
{
    return SeqAlignFindSeqId (salphead, sip);
}

/*
static Boolean LIBCALL SeqIdInSeqLocList (SeqIdPtr sip, ValNodePtr vnp)
{
    return SeqIdInSeqLocList(sip, vnp);
}
*/
NLM_EXTERN Boolean LIBCALL is_dim1seqalign (SeqAlignPtr salp)
{
  if (salp->dim == 1) { 
     return TRUE;
  }
  return FALSE;
}  

NLM_EXTERN Boolean LIBCALL is_dim2seqalign (SeqAlignPtr salp)
{
  SeqAlignPtr nextsalp;
  DenseSegPtr dsp;
  SeqIdPtr    sip;

  if (salp->dim == 2 && salp->next != NULL) {
     if (salp->segtype == 2) {
        dsp = (DenseSegPtr) salp->segs;
        sip = dsp->ids;
        nextsalp = salp->next;
        while (nextsalp) {
           if (nextsalp->dim != 2)
              break;
           if (nextsalp->segtype != 2) 
              break;
           dsp = (DenseSegPtr) nextsalp->segs;
           if (! SeqIdForSameBioseq (sip, dsp->ids))
              break;
           nextsalp = nextsalp->next;
        }
        if (nextsalp == NULL) 
           return TRUE;
     }
  }
  return FALSE;
}

NLM_EXTERN Boolean LIBCALL is_fasta_seqalign (SeqAlignPtr salp)
{
  DenseSegPtr dsp;
  Int4Ptr     startp;
  Boolean     gap;
  Int4        k;
  Int2        j;
  Boolean     one_gap = FALSE;

  if (salp->dim > 2) {
     if (salp->segtype == 2) {
        dsp = (DenseSegPtr) salp->segs;
        for (j=0; j<dsp->dim; j++) {
           gap=FALSE;
           for (k=0; k<dsp->numseg; k++) {
              startp=dsp->starts;
              if (startp[dsp->dim*k + j] < 0) {
                 gap = TRUE;
                 one_gap = TRUE;
              }
              else if (gap)
                 return FALSE;
           }
        }
     }
  }
  else 
     return FALSE;
  if (!one_gap)
     return FALSE;
  return TRUE;
}


NLM_EXTERN SeqAlignPtr LIBCALL is_salp_in_sap (SeqAnnotPtr sap, Uint1 choice)
{
  SeqAlignPtr      salp = NULL;

  if (sap != NULL) {
     for (; sap!= NULL; sap=sap->next) {
        if (sap->type == choice) {
           salp = (SeqAlignPtr) sap->data;
           return salp;
        }
     }   
  }
  return NULL;
}




/*
  Function to Check if at least one of the SeqIds of a SeqLocList
  is the same as one of the SeqIds in the SeqAlign
  */

NLM_EXTERN Boolean LIBCALL SeqAlignSeqLocComp (SeqAlignPtr salphead, ValNodePtr vnp)
{
  SeqAlignPtr salp;
  ValNodePtr tmp;
  SeqLocPtr  slp;
  SeqIdPtr   sip,
             tmpsip;

  for (tmp=vnp; tmp!=NULL; tmp=tmp->next)
  {
     slp= (SeqLocPtr)tmp->data.ptrvalue;
     if (slp)
     {
        sip=SeqLocId(slp);
        for (salp = salphead; salp!= NULL; salp=salp->next)
        {
           tmpsip =SeqIdPtrFromSeqAlign (salp);
           if ((SeqIdOrderInBioseqIdList(sip, tmpsip)) > 0)
           {
              return TRUE;
           }
        }
     }
  } 
  return FALSE;
}

/*
  SeqAlignBestScore : Find The Last Score of type "score"
  In the SeqAlign: Does not traverse Chain.
  */
NLM_EXTERN Int4 LIBCALL SeqAlignBestScore (SeqAlignPtr salp)
{
  ScorePtr    score;
  ObjectIdPtr oip;
  Int4        val = 0;

  if (salp!=NULL) {
     if (salp->segtype == 2) {
        for (score = (ScorePtr) salp->score; score!=NULL; score=score->next)
        {
           oip = (ObjectIdPtr) score->id;
           if (oip != NULL) {
              if (StringStr(oip->str, "score")!=NULL) {
                 if (score->choice == 1) {
                    val = (Int4)score->value.intvalue; 
                 }
              }
           }
        }
     }
  }
  return val;
}

NLM_EXTERN SeqLocPtr LIBCALL SeqLocMixFromSeqAlign (SeqAlignPtr salp, SeqIdPtr sip)
{
  DenseSegPtr dsp;
  SeqLocPtr   headslp = NULL,
              slp = NULL,
              tmp;
  SeqIdPtr    siptmp;
  Int4Ptr     lens, start,
              start2;
  Int4        salp_start, salp_stop;
  Int2        offset, j;

  if (salp == NULL)
     return NULL;
  if (salp->segtype != 2)
     return NULL;
  dsp =  (DenseSegPtr) salp->segs;
  if (dsp == NULL)
     return NULL;
  offset = 0;
  if (sip  == NULL)
     sip = dsp->ids;
  else {
     sip = SeqIdFindBest (sip, 0);
     for (siptmp=dsp->ids; siptmp!=NULL; siptmp=siptmp->next, offset++) {
        if (SeqIdForSameBioseq (sip,siptmp))
           break;
     }
     if (siptmp == NULL)
        return NULL;
  }
  start=dsp->starts + offset;
  if (offset==0)
     start2 = start+1;
  else
     start2 = dsp->starts;
  lens=dsp->lens;
  for (j=0; j<dsp->numseg; j++) { 
     if (*start > -1) 
        break;
     start +=dsp->dim;
     start2+=dsp->dim;
     lens++;
  }
  for (; j<dsp->numseg; j++, lens++) {
     if (*start > -1 && *start2 > -1) {
        salp_start = *start;
        salp_stop = salp_start + *lens -1;
        slp = SeqLocIntNew (salp_start, salp_stop, Seq_strand_plus, sip);
        if (headslp == NULL) {
           headslp = slp;
        }
        else if (headslp->choice == SEQLOC_MIX) {
           tmp = (ValNodePtr)(headslp->data.ptrvalue);
           while (tmp->next != NULL)
              tmp = tmp->next;
           tmp->next = slp;
        }
        else {
           tmp = ValNodeNew(NULL);
           tmp->choice = SEQLOC_MIX;
           tmp->data.ptrvalue = (Pointer)headslp;
           headslp = tmp;
           tmp = (ValNodePtr)(headslp->data.ptrvalue);
           tmp->next = slp;
        }
     }
     start +=dsp->dim;
     start2+=dsp->dim;
  }
  return headslp; 
}


NLM_EXTERN ValNodePtr LIBCALL SeqLocListOfBioseqsFromSeqAlign (SeqAlignPtr salp)
{
  SeqAlignPtr   salptmp;
  BioseqPtr     bsp;
  ValNodePtr    vnp = NULL;
  DenseSegPtr   dsp;
  DenseDiagPtr  ddp;
  SeqIdPtr      sip,
                siptmp;
  SeqLocPtr     slp;

  if (salp==NULL) 
     return NULL;
  for (salptmp = salp; salptmp!=NULL; salptmp=salptmp->next)
  {
     sip = NULL;
     if (salptmp->segtype == 1)
     {
        ddp = (DenseDiagPtr) salptmp->segs;
        sip = ddp->id;        
     }
     else if (salptmp->segtype == 2)
     {
        dsp = (DenseSegPtr) salptmp->segs;
        sip = dsp->ids;
     }
     if (sip != NULL) 
     {
        for (; sip != NULL; sip = sip->next)
        {
           siptmp = SeqIdDup (sip); 
           if (vnp==NULL || (vnp!=NULL && !SeqIdInSeqLocList (siptmp, vnp)))
           {
              bsp = BioseqLockById (siptmp);
              if (bsp!= NULL) 
              {
                 slp = SeqLocIntNew (0, bsp->length-1, Seq_strand_plus, siptmp);
                 if (slp != NULL)
                 {
                    ValNodeAddPointer (&vnp, 0, (Pointer) slp);
                 }
                 BioseqUnlock (bsp);
              }
           }
        }
     }
  }
  return vnp;
}


static SeqIdPtr LIBCALL MySeqIdSetFree (SeqIdPtr sip)
{
  SeqIdPtr next;
 
    while (sip != NULL)
    {
        next = sip->next;
        sip->next = NULL;
        if (sip!=NULL) {
            if (sip->choice > 0 && sip->choice < 30) SeqIdFree(sip);
        }
        else break;
        sip = next;
    }
    return NULL;
}

NLM_EXTERN SeqAlignPtr LIBCALL CompSeqAlignFree (SeqAlignPtr salp)
{
  CompSegPtr csp;
 
  csp = (CompSegPtr) salp->segs;
  if (csp->ids != NULL) MySeqIdSetFree (csp->ids);
  csp->ids=NULL;
  if (csp->from != NULL) MemFree (csp->from);
  csp->from=NULL;
  if (csp->lens != NULL) MemFree (csp->lens);
  csp->lens=NULL;
  if (csp->starts!= NULL) MemFree (csp->starts);
  csp->starts=NULL;
  MemFree ( csp);
  salp->segs = NULL;
  SeqAlignSetFree (salp);
  return NULL;
}

NLM_EXTERN SeqAnnotPtr LIBCALL CompSeqAnnotFree (SeqAnnotPtr sap)
{
  SeqAlignPtr salp;

  if ( sap == NULL ) return NULL; 
  if ( sap->type != 2 ) {
      ErrPostEx(SEV_WARNING, 0, 0, "fail in CompSeqAnnotFree [1] %d", (int)sap->type); 
      return NULL; 
  }
  if ( ( salp = (SeqAlignPtr) sap->data ) == NULL ) {
      ErrPostEx(SEV_WARNING, 0, 0, "fail in CompSeqAnnotFree [2]"); 
      return NULL; 
  }
  if ( salp->segtype == COMPSEG ) 
      CompSeqAlignFree (salp);
  else if ( salp->segtype == 2 )
      SeqAnnotFree (sap);
  else {
      ErrPostEx(SEV_WARNING, 0, 0, "fail in CompSeqAnnotFree [3] %d", salp->segtype); 
      return NULL; 
  }
  sap->data = NULL;
  return NULL;
}

NLM_EXTERN SeqAnnotPtr LIBCALL SeqAlignBoolSegCpy (SeqAnnotPtr sap, Int4 from, Int4 to)
{
  SeqAnnotPtr sapnew =NULL;
  SeqAlignPtr salp, salpnew =NULL, salpre =NULL;
  CompSegPtr  csp =NULL;
  CompSegPtr  newcsp =NULL;
  BoolPtr     cspstart =NULL;
  Int4Ptr     csplens =NULL, newcsplens =NULL;
  Int4Ptr     cspfrom =NULL, newcspfrom =NULL;
  Int4        j, k, nbseq, numseg, newnumseg;
  Int4        sumlens, offset;

  if ( sap == NULL ) return NULL; 
  if ( sap->type != 2 ) {
      ErrPostEx(SEV_WARNING, 0, 0, "fail in SeqAlignBoolSegCpy [1]"); 
      return NULL; 
  }
  if ( ( salp = (SeqAlignPtr) sap->data ) == NULL ) {
      ErrPostEx(SEV_WARNING, 0, 0, "fail in SeqAlignBoolSegCpy [1-2]"); 
      return NULL; 
  }
  if ( salp->segtype != COMPSEG ) {
      ErrPostEx(SEV_WARNING, 0, 0, "fail in SeqAlignBoolSegCpy [2]"); 
      return NULL; 
  }
  sapnew = SeqAnnotNew ();
  if (sapnew == NULL) {
      return NULL; 
  }
  sapnew->type = 2;

  while ( salp != NULL )
  {
      salpnew = SeqAlignNew ();
      if ( salpnew == NULL ) {
          SeqAnnotFree (sapnew);
          return NULL; 
      }
      salpnew->type = 3;
      salpnew->segtype = COMPSEG;
      salpnew->dim = salp->dim;
      csp = (CompSegPtr) salp->segs;
      newcsp = (CompSegPtr) MemNew ( sizeof (CompSeg) );
      if ( newcsp == NULL ) {
          ErrPostEx(SEV_WARNING, 0, 0, "fail in SeqAlignBoolSegCpy [4]"); 
          SeqAnnotFree (sapnew);
          return NULL; 
      }
      salpnew->segs = (Pointer) newcsp;
      nbseq = newcsp->dim = csp->dim;
      newcsp->ids = SeqIdDupList (csp->ids);
      numseg = newcsp->numseg = csp->numseg;

          /* copy the first position (from) + sum of length */
      newcsp->from = (Int4Ptr) MemNew ((size_t) ((nbseq +2) * sizeof (Int4)));
      if ( newcsp->from == NULL ) {
          SeqAnnotFree (sapnew);
          return NULL; 
      }
      for (j = 0; j < nbseq; j++) {
          offset = -1;
          sumlens = 0;
          csplens = csp->lens;
          cspstart = csp->starts;
          cspstart += j;
          for (k = 0; k < numseg ; k++, csplens++, cspstart +=nbseq) {
               if ( from >= sumlens && from < sumlens + *csplens ) {
                    if ( (Boolean)(*cspstart) ) offset =(sumlens +*csplens -from);
                    break;
               }
               if ( (Boolean)(*cspstart) ) sumlens += (*csplens);
          }
          if ( offset < 0 ) 
               for (; k < numseg && offset < 0; k++) {
                    if ( (Boolean)(*cspstart) ) offset = 0;
               }
          newcsp->from [j] = csp->from [j] +offset;
      }
          /* count number of new segments between from and to */
      sumlens =0;
      newnumseg =0;
      cspstart = csp->starts;
      for (k=0; k<numseg; k++, sumlens += (Int4)(*csplens), csplens++, cspstart += (Int4)(nbseq))
      {
          if ( from >= sumlens && from < sumlens + *csplens ) break;
      }
      for (; k<numseg; k++, sumlens += *csplens, csplens++, cspstart +=nbseq)
      {
          newnumseg++;
          if ( to >= sumlens && to < sumlens + *csplens ) break;
      }
          /* copy segment lengths within from and to */
      newcsp->lens =(Int4Ptr) MemNew ((size_t) ((newnumseg+2) * sizeof (Int4)));
      if ( newcsp->lens == NULL ) {
          SeqAnnotFree (sapnew);
          return NULL; 
      }
      sumlens =0;
      csplens = csp->lens;
      cspstart = csp->starts;
      newcsplens = newcsp->lens;
      for (k=0; k<numseg; k++, sumlens += *csplens, csplens++, cspstart +=nbseq)
      {
          if ( from >= sumlens && from < sumlens + *csplens ) break;
      }
      *newcsplens = sumlens +*csplens -from;
      newcsplens++;
      csplens++;
      k++;
      for (; k<numseg; k++, sumlens += *csplens, csplens++, cspstart +=nbseq)
      {
          *newcsplens = *csplens;
          if ( to >= sumlens && to < sumlens + *csplens ) break;
          newcsplens++;
      }
      *newcsplens = sumlens -to;
      newcsp->starts =(BoolPtr) MemNew((size_t)((nbseq *newnumseg +2) * sizeof(Boolean)));
      if ( newcsp->starts == NULL ) {
          SeqAnnotFree (sapnew);
          return NULL; 
      }
      for (j = 0; j < nbseq; j++) 
      {
          sumlens = 0;
          csplens = csp->lens;
          cspfrom = csp->from;
          cspfrom += j;
          newcspfrom = newcsp->from;
          newcspfrom += j;
          for(k=0;k<numseg; k++,sumlens +=*csplens, csplens++, cspstart +=nbseq)
          {
               if ( from >= sumlens && from < sumlens + *csplens ) break;
          }
          for (; k<numseg; k++, sumlens +=*csplens, csplens++, cspstart +=nbseq)
          {
               *newcsplens = *csplens;
               if ( to >= sumlens && to < sumlens + *csplens ) break;
          }
      }
/*
      csp->strands
      csp->scores
*/
      if ( salpre == NULL ) sapnew->data = (Pointer) salpnew;
      else salpre->next = salpnew;
      salpre = salpnew;
      salp = salp->next;
  }
  return sapnew; 
}

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignDenseSegToBoolSeg (SeqAlignPtr salp)
{
  SeqAlignPtr salpnew =NULL;
  DenseSegPtr dsp =NULL;
  Int4Ptr     dspstarts =NULL, dspstarts2 = NULL;
  Int4Ptr     dsplens =NULL;
  Uint1Ptr    strandp;
  Uint1       strand;
  CompSegPtr  csp =NULL;
  Int4        j; 
  Int4        nbseq, numseg;

  salpnew = SeqAlignNew ();
  if ( salpnew == NULL ) {
          return NULL; 
  }
  salpnew->type = 3;
  salpnew->segtype = COMPSEG;
  salpnew->dim = salp->dim;

  salpnew->score = salp->score;
  salp->score = NULL;

  dsp = (DenseSegPtr) salp->segs;
  csp = (CompSegPtr) MemNew ( sizeof (CompSeg) );
  if ( csp == NULL ) {
      return NULL; 
  }
  salpnew->segs = (Pointer) csp;
  csp->dim = dsp->dim;
  nbseq = dsp->dim;

  csp->ids = SeqIdDupBestList (dsp->ids);
  csp->numseg = dsp->numseg;
  numseg = dsp->numseg;

  if (dsp->strands != NULL) {
     strandp = dsp->strands;
     csp->strands=(Uint1Ptr)MemNew((size_t)((nbseq*numseg+4)*sizeof (Uint1)));
     for (j=0; j<nbseq*numseg; j++, strandp++)
        csp->strands[j] = *strandp;
  }
  csp->lens = (Int4Ptr) MemNew ((size_t) ((numseg+2) * sizeof (Int4)));
  if ( csp->lens == NULL ) {
     return NULL; 
  }
  for (j = 0, dsplens = dsp->lens; j < numseg ; j++, dsplens++ ) 
     csp->lens [j] = *dsplens;
  csp->from = (Int4Ptr) MemNew ((size_t) ((nbseq +2) * sizeof (Int4)));
  if ( csp->from == NULL ) {
     return NULL; 
  }
  for (j = 0; j < nbseq +2; j++) 
     csp->from [j] = 0;
  dspstarts=dsp->starts;
  strandp = dsp->strands;
  if (strandp!=NULL)
     strand = *strandp;
  else 
     strand = Seq_strand_unknown;
  for (j = 0; j < nbseq ; j++, dspstarts++)
  {
     dsplens = dsp->lens;
     if (*dspstarts < 0) {
        for(dspstarts2=dspstarts;dspstarts2!=NULL;dspstarts2+=nbseq) {
           if (*dspstarts2 > -1) 
              break;
           dsplens++;
        }
        if (dspstarts2!=NULL) { 
           if (strand==Seq_strand_minus)
              csp->from [j] = *dspstarts2 + *dsplens;
           else
              csp->from [j] = *dspstarts2;
        }
     }
     else {
        if (strand==Seq_strand_minus) 
           csp->from [j] = *dspstarts + *dsplens;
        else
           csp->from [j] = *dspstarts;
     }
     if (strandp!=NULL) {
        strandp++;
        strand = *strandp;
     } else strand = Seq_strand_unknown;
  }
  csp->starts=(BoolPtr) MemNew((size_t)((nbseq *numseg +2) * sizeof(Boolean)));
  if ( csp->starts == NULL ) {
     return NULL; 
  }
  for (j =0, dspstarts =dsp->starts; j <nbseq*numseg ; j++, dspstarts++ ) {
          csp->starts [j] = (Boolean) (*dspstarts >= 0);
  }
  return salpnew;
}

NLM_EXTERN SeqAnnotPtr LIBCALL SeqAnnotDenseSegToBoolSeg (SeqAnnotPtr sap)
{
  SeqAnnotPtr sapnew =NULL;
  SeqAlignPtr salp=NULL, salpnew=NULL, salpre=NULL;

  if ( sap == NULL ) return NULL; 
  if ( sap->type != 2 ) {
      return NULL; 
  }
  if ( ( salp = (SeqAlignPtr) sap->data ) == NULL ) {
      return NULL; 
  }
  if ( salp->segtype == COMPSEG ) return sap;
  if ( salp->segtype != 2 ) {
      return NULL; 
  }
  sapnew = SeqAnnotNew ();
  if (sapnew == NULL) {
      return NULL; 
  }
  sapnew->type = 2;
  while ( salp != NULL )
  {
      salpnew = SeqAlignDenseSegToBoolSeg (salp);
      if ( salpre == NULL ) sapnew->data = (Pointer) salpnew;
      else salpre->next = salpnew;
      salpre = salpnew;
      salp = salp->next;
  }
  return sapnew; 
}

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignBoolSegToDenseSeg (SeqAlignPtr salp)
{
  SeqAlignPtr salpnew =NULL;
  DenseSegPtr dsp =NULL;
  CompSegPtr  csp =NULL;
  BoolPtr     cspstarts =NULL;
  Int4Ptr     cspfrom =NULL;
  Int4Ptr     csplens =NULL;
  Uint1Ptr    strandp;
  Uint1       strand;
  Int4Ptr     dspstarts =NULL;
  Int4        sumlens, k;
  Int4        j, nbseq, numseg;

  salpnew = SeqAlignNew ();
  if (salpnew != NULL) {
     salpnew->type = 3;
     salpnew->segtype = 2;
     salpnew->dim = salp->dim;

     salpnew->score = salp->score;
     salp->score = NULL;

     csp = (CompSegPtr) salp->segs;
     dsp = DenseSegNew ();
     if ( dsp == NULL ) {
        return NULL; 
     }
     salpnew->segs = (Pointer) dsp;
     nbseq = csp->dim;
     dsp->dim = csp->dim;
     dsp->ids = SeqIdDupList (csp->ids);
     dsp->numseg = csp->numseg;
     numseg = csp->numseg;

     dsp->lens  =(Int4Ptr)MemNew((size_t)((numseg + 2) * sizeof (Int4))); 
     for (j = 0, csplens = csp->lens; j < numseg ; j++, csplens++ ) 
        dsp->lens [j] = *csplens;
     if (csp->strands != NULL) {
        strandp=csp->strands;
        dsp->strands =(Uint1Ptr)MemNew((size_t)((nbseq*numseg+4)*sizeof (Uint1)));
        for (j=0; j<nbseq*numseg; j++, strandp++)
           dsp->strands[j] = *strandp;
     }
     dsp->starts=(Int4Ptr)MemNew((size_t)((nbseq *numseg +4) * sizeof (Int4)));
     for (k = 0; k < nbseq *numseg +4; k++) 
        dsp->starts[k] = -1;
     cspstarts = csp->starts;
     dspstarts = dsp->starts;
     strandp = csp->strands;
     if (strandp!=NULL)
        strand = *strandp;
     else 
        strand = Seq_strand_unknown;
     for (j = 0, cspfrom = csp->from; j < nbseq ; j++, cspfrom++) 
     {
        csplens = csp->lens;
        sumlens = 0;
        for (k = 0; k < numseg; k++, csplens++) {
           if ( (Boolean)(cspstarts [nbseq *k +j]) ) {
              if (strand == Seq_strand_minus) {
                 sumlens += *csplens;
                 dspstarts [nbseq *k +j] = *cspfrom - sumlens;
              }
              else {
                 dspstarts [nbseq *k +j] = *cspfrom + sumlens;
                 sumlens += *csplens;
              }
           }
        }
        if (strandp!=NULL) {
           strandp++;
           strand = *strandp;
        } 
        else 
           strand = Seq_strand_unknown;
     }
  }
  return salpnew;
}

NLM_EXTERN SeqAnnotPtr LIBCALL SeqAnnotBoolSegToDenseSeg (SeqAnnotPtr sap)
{
  SeqAnnotPtr sapnew =NULL;
  SeqAlignPtr salp, salpnew =NULL, salpre =NULL;

  if ( sap == NULL ) return NULL; 
  if ( sap->type != 2 ) {
      return NULL; 
  }
  if ( ( salp = (SeqAlignPtr) sap->data ) == NULL ) {
      return NULL; 
  }
  if ( salp->segtype == 2 ) return sap;
  if ( salp->segtype != COMPSEG ) {
      return NULL; 
  }
  sapnew = SeqAnnotNew ();
  if (sapnew == NULL) return NULL;
  sapnew->type = 2;

  while ( salp != NULL )
  {
      salpnew = SeqAlignBoolSegToDenseSeg (salp);
      if ( salpre == NULL ) sapnew->data = (Pointer) salpnew;
      else salpre->next = salpnew;
      salpre = salpnew;
      salp = salp->next;
  }
  return sapnew; 
}


NLM_EXTERN void LIBCALL CompSeqAlignPrint (SeqAlignPtr salp)
{
  CompSegPtr  csp =NULL;
  SeqIdPtr    sip;
  BoolPtr     startp;
  Int4        j, k;
  FILE        *fout;
  Char    strLog[128];

  csp = (CompSegPtr) salp->segs;
  if (csp!=NULL) {
     fout = FileOpen ("LogFile", "w");
     if (fout!=NULL) {
        fprintf (fout, "\n");
        for (j=0, sip=csp->ids; sip!=NULL; sip=sip->next, j++)
        {
            SeqIdWrite (sip, strLog, PRINTID_FASTA_LONG, 120);
            fprintf (fout, "%d %s \n", (int)(j+1), strLog);
        } 
        fprintf (fout, "\n");
        for (j=0; j<csp->dim; j++) 
           fprintf (fout, " %d ", (int)csp->from[j]); 
        fprintf (fout, "\n");
        startp = csp->starts;
        for (j = 0; j < csp->numseg; j++) {
            fprintf (fout, "%3d lg %6ld ", (int)j, (long)csp->lens[j]);
            for (k = 0; k < csp->dim; k++) { 
               fprintf (fout, " %d", (int)*startp);
               startp++;
            }
/*
            for ( k = 0; k < csp->dim; k++) { 
               if (csp->strands!=NULL)
                 fprintf (fout, " %d", (int)csp->strands[(Int4)csp->dim*k+j]);
            }
*/
            fprintf (fout, "\n"); 
        } 
        fprintf (fout, "\n"); 
        FileClose (fout);
     }
  }
}

NLM_EXTERN SeqAlignPtr LIBCALL build_seqalign_fromstart (Int2 dim, Int2 numseg, SeqIdPtr sip, Int4Ptr starts, Int4Ptr lens)
{
  SeqAlignPtr salp;
  DenseSegPtr dsp;
  SeqIdPtr    next;

  salp = SeqAlignNew ();
  if (salp != NULL) {
     salp->type = 3;
     salp->segtype = 2;
     salp->dim = dim;
     dsp = DenseSegNew ();
     if (dsp != NULL) {
        salp->segs = (Pointer) dsp;
        dsp->dim = dim;
        dsp->ids = sip;
        dsp->numseg = numseg;
        dsp->starts = starts;
        dsp->lens = lens;
        return salp;
     }
  }
  MemFree (starts);
  while (sip != NULL) {
     next = sip->next;
     sip->next = NULL;
     SeqIdFree (sip);
     sip = next;
  }
  return NULL;
}

NLM_EXTERN DenseDiagPtr LIBCALL DenseDiagDup (DenseDiagPtr ddp)
{
  DenseDiagPtr ddp2;
  ddp2 = (DenseDiagPtr) AsnIoMemCopy ((Pointer) ddp, (AsnReadFunc) DenseDiagAsnRead, (AsnWriteFunc) DenseDiagAsnWrite);
  return ddp2;
}

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignDupRegion (SeqAlignPtr salp, Int2 to_numseg, Int4 subseg, Boolean first_part)
{
  SeqAlignPtr newsalp = NULL;
  DenseSegPtr dsp = NULL, newdsp = NULL;
  Int4Ptr     dspstart = NULL;
  Int4Ptr     dsplen = NULL;
  Int4Ptr     newstart = NULL;
  Int4Ptr     newlens = NULL;
  Int2        newseg;
  Int2        n;
  Int2        k;
  Int4        j;

  dsp = (DenseSegPtr) salp->segs;
  newsalp = SeqAlignNew ();
  if ( newsalp == NULL ) return NULL;
  newsalp->type = salp->type;
  newsalp->segtype = salp->segtype;
  newsalp->dim = salp->dim;
  newdsp = DenseSegNew ();
  if ( newdsp == NULL ) return NULL;
  newsalp->segs = (Pointer) newdsp;
  n = newdsp->dim = dsp->dim;
  newdsp->ids = SeqIdDupList (dsp->ids);
  if ( to_numseg > dsp->numseg ) to_numseg = 0;
  if ( to_numseg == 0 )
         newseg = dsp->numseg;
  else if ( first_part )
         newseg = to_numseg;
  else if ( !first_part )
         newseg = dsp->numseg - to_numseg;
  newdsp->numseg = newseg;
  newdsp->starts = (Int4Ptr) MemNew ((size_t) ((n*newseg+4) * sizeof (Int4)));
  if ( newdsp->starts == NULL ) return NULL;
  for (j = 0; j < n * newseg + 4; j++) newdsp->starts [j] = -1;
  newdsp->lens  = (Int4Ptr) MemNew ((size_t) ((newseg + 2) * sizeof (Int4))); 
  if ( newdsp->lens == NULL ) return NULL;
  for (j = 0; j < newseg + 2; j++)     newdsp->lens [j] = 0;
  newstart = newdsp->starts;
  newlens  = newdsp->lens;
  dspstart = dsp->starts;
  dsplen   = dsp->lens;
  if ( first_part )
  {
         for ( k = 0; k < newseg - 1; k++ ) 
         {
                for (j = 0; j < n; j++) newstart [j] = dspstart[j];
                *newlens = *dsplen;
                newstart += n;
                newlens++;
                dspstart += n;
                dsplen++;
         }
         for (j = 0; j < n; j++) newstart [j] = dspstart[j];
         *newlens = subseg;
  }
  else {
         for ( k = 0; k < to_numseg-1; k++ ) 
         {
                dspstart += n;
                dsplen++;
         } 
         for (j = 0; j < n; j++) 
                       if ( dspstart[j] > -1 )
                              newstart [j] = dspstart[j] + subseg;
                       else 
                              newstart [j] = dspstart[j];
         *newlens = *dsplen - subseg;
         newstart += n;
         newlens++;
         dspstart += n;
         dsplen++;
         k++;
         for ( ; k < to_numseg + newseg; k++ ) 
         {
                for (j = 0; j < n; j++) newstart [j] = dspstart[j];
                *newlens = *dsplen;
                newstart += n;
                newlens++;
                dspstart += n;
                dsplen++;
         }
  }
  return newsalp;
}

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignDupAdd (SeqAlignPtr *salp_head, SeqAlignPtr salp, Int2 to_numseg, Int4 subseg, Boolean first_part)
{
  SeqAlignPtr salp_tmp, salp_copy;

  salp_copy = SeqAlignDupRegion (salp, to_numseg, subseg, first_part);
  if ( salp_copy == NULL )
         return *salp_head;
  if ( (salp_tmp = *salp_head) != NULL ) 
  {
         while ( salp_tmp->next != NULL ) 
                salp_tmp = salp_tmp->next;
         salp_tmp->next = salp_copy;
  }
  else *salp_head = salp_copy;
  return *salp_head;
}

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignEndExtend (SeqAlignPtr sap, Int4 start1, Int4 start2, Int4 stop1, Int4 stop2, Int4 x1, Int4 y1, Int4 x2, Int4 y2, Uint1 strand1, Uint1 strand2)
{
   DenseSegPtr   dsp;
   Int4Ptr       n_starts, n_lens;
   Uint1Ptr      n_strands;
   Int4          index1,
                 minlen = 0;
   Int2          n;
   Int2          offset=0;
   Boolean       is_strands = FALSE;

   if (sap==NULL)
      return NULL;
   if (start1==x1 && start2==y1 && stop1==x2 && stop2==y2)
      return sap;
   if (sap->segtype == 2)
    {
      dsp = (DenseSegPtr) sap->segs;
      is_strands = (Boolean) (dsp->strands!=NULL);
      n = dsp->numseg;
      n_starts = (Int4Ptr) MemNew((2*(n+2)+4)*sizeof(Int4));
      n_lens = (Int4Ptr) MemNew((n+4)*sizeof(Int4));
      if (is_strands)
         n_strands = (Uint1Ptr)MemNew((2*(n+2)+4)*sizeof(Uint1));
      if (x1 > start1 && y1 > start2) { 
         minlen = MIN ((x1-start1), (y1-start2)); 
         n = 0;
      }
      if ((x1 > start1 || y1 > start2)  && ((x1-start1) != (y1-start2))) { 
         if (x1 > start1 && (x1-start1) > minlen) {
            n_starts[0] = 0;
            n_starts[1] = -1;
            n_lens[0] = (x1 - start1) - minlen;
            if (is_strands) {
               n_strands[0] = strand1;
               n_strands[1] = strand2; 
            }
         } 
         else if (y1 > start2 && (y1-start2) > minlen) {
            n_starts[0] = -1;
            n_starts[1] = 0;
            n_lens[0] = (y1 - start2) - minlen;
            if (is_strands) {
               n_strands[0] = strand1;
               n_strands[1] = strand2; 
            }
         }
         offset += 1;
      }
      if (minlen > 0) {
         n += offset;
         n_starts[2*n] = x1 - minlen;
         n_starts[2*n+1] = y1 - minlen;
         n_lens[n] = minlen;
         if (is_strands) {
            n_strands[2*n] = strand1;
            n_strands[2*n+1] = strand2;
         }
         offset += 1;
      }
      n = dsp->numseg;
      for (index1=0; index1<n; index1++) {
         n_starts [2*(index1+offset)] = dsp->starts [2*index1];
         if (is_strands)
            n_strands[2*(index1+offset)] = dsp->strands[2*index1];
         n_starts [2*(index1+offset)+1]=dsp->starts [2*index1+1];
         if (is_strands)
            n_strands[2*(index1+offset)+1]=dsp->strands[2*index1+1];
         n_lens[index1+offset] = dsp->lens[index1];
      }
      if (x2 < stop1 &&  y2 < stop2) {
         n += offset;
         minlen = MIN ((stop1-x2), (stop2-y2));
         n_starts[2*n] = x2 + 1;
         n_starts[2*n+1] = y2 +1;
         n_lens[n] = minlen;
         if (is_strands) {
            n_strands[2*n] = strand1;
            n_strands[2*n+1] = strand2;
         }
         x2 += minlen;
         y2 += minlen;
         offset += 1;
      }
      if (x2 < stop1 || y2 < stop2) { 
         n += offset;
         if (x2 < stop1) {
            n_starts[2*n] = x2 +1;
            n_starts[2*n+1] = -1;
            n_lens[n] = stop1 - x2;
            if (is_strands) {
               n_strands[2*n] = strand1;
               n_strands[2*n+1] = strand2;
            }
         } 
         else if (y2 < stop2) {
            n_starts[2*n] = -1;
            n_starts[2*n+1] = y2 +1;
            n_lens[n] = stop2 - y2;
            if (is_strands) {
               n_strands[2*n] = strand1;
               n_strands[2*n+1] = strand2;
            }
         }
         offset += 1;
      }
      dsp->numseg = n+1;
      MemFree(dsp->starts);
      if (is_strands)
         MemFree(dsp->strands);
      MemFree(dsp->lens);
      dsp->starts = n_starts;
      dsp->lens = n_lens;
      if (is_strands)
         dsp->strands = n_strands;
   }
   return sap;
}

NLM_EXTERN Boolean LIBCALL get_pos_from_salp (SeqAlignPtr salp, Int4 pos, Int4 PNTR offset, 
Int4Ptr PNTR startp, Int4Ptr PNTR lenp, Int4 PNTR numseg)
{
  DenseSegPtr dsp;
  Int4Ptr     dspstart,
              dsplens;
  Int4        numsg = 0,
              sumlens = 0;

  Boolean     seen=FALSE;
  
  if (salp == NULL)
     return FALSE;
  if (salp->segtype == 2) 
  {
     dsp = (DenseSegPtr) salp->segs;
     dspstart = dsp->starts;
     dsplens = dsp->lens;
     while ( !seen && numsg < dsp->numseg ) 
     {
        numsg++;
        if (pos >= sumlens && pos < sumlens +*dsplens ) 
        {
           *offset = pos - sumlens;
           seen = TRUE;
        }
        else if (numsg == dsp->numseg)
        {
           break;
        }
        else
        {
            dspstart += dsp->dim;
            sumlens += *dsplens;
            dsplens++;
        }
     }
     if (seen) {
        *startp = dspstart;
        *lenp = dsplens;
        *numseg = numsg;
     }
  }
  return seen;
}

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignTrunc (SeqAlignPtr salp, Int4 from, Int4 to)
{
  DenseSegPtr dsp;
  Int4Ptr     startp,
              lenp,
              startmp;
  Int4        tot_length,
              numseg,
              offset,
              j;
  Boolean     ble;
  
  if (salp == NULL)
     return salp;
  tot_length = SeqAlignLength (salp);
  if (salp->segtype == 2) 
  {
     dsp = (DenseSegPtr) salp->segs;
     if (to > 0 && to < tot_length) 
     {
        ble = get_pos_from_salp (salp, to, &offset, &startp, &lenp, &numseg);
        if(ble) 
        {
           if (numseg > 0) 
           {
              dsp->numseg = numseg;
           }
           if (offset>-1 && offset+1 < *lenp) 
           {   
              *lenp = offset + 1;
           }
        }
     }
     if (from > 0 && from < tot_length) 
     {
        ble = get_pos_from_salp (salp, from, &offset, &startp, &lenp, &numseg);
        if(ble) 
        {
           if (numseg > 1) 
           {
              dsp->numseg -= (numseg-1);
              dsp->starts = startp;
              dsp->lens = lenp;
           }
           if (offset > 0) {
              for (startmp=dsp->starts, j=0; j<dsp->dim; startmp++, j++)
                 if (*startmp > -1)
                    *startmp += offset+1;
           }
           *(dsp->lens) -= offset;
        }
     }
  }
  return salp;
}

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignTrunc2 (SeqAlignPtr salp, Int4 from, Int4 to)
{
  DenseSegPtr dsp;
  Int4Ptr     int4p, 
              int4lp;
  Int4        k;
  Int2        j;

  if (salp == NULL)
     return NULL;
  if (salp->segtype == 2) {
     dsp = (DenseSegPtr) salp->segs;
     int4p = dsp->starts;
     int4lp = dsp->lens;
     for (j=0; j<dsp->dim;j++, int4p++) 
     {
       if (SeqAlignStrand(salp, j)!=Seq_strand_minus)
       {
          if (from != 0) {
             if (*int4p > -1)
                *int4p += from;
          }
       }
       else if (SeqAlignStrand(salp, j)==Seq_strand_minus)
       {
          if (to != 0) {
             for (k=0; k<(dsp->numseg-1); k++)
                int4p++;
             if (*int4p > -1)
                *int4p -= to;
          }
       }
     }
     if (from != 0)
        *int4lp -= from;
     if (to != 0) {
        for (k=0; k<(dsp->numseg-1); k++)
           int4lp++;
        *int4lp += to;
     }
  }
  return salp;
}

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignMapOnFirstSeq (SeqAlignPtr salp)
{
  SeqAlignPtr tmp;
  DenseSegPtr dsp;
  Int4Ptr     startp;
  Int4Ptr     lenp;
  Int4        j;
  
  if (salp == NULL)
     return NULL;
  for (tmp=salp; tmp!=NULL; tmp=tmp->next) {
     if (tmp->segtype == 2) {
        dsp = (DenseSegPtr) tmp->segs;
        for (j=0, startp = dsp->starts; j<(dsp->numseg-1); j++)
           startp += dsp->dim;
        while (*startp < 0) {
           startp -= dsp->dim;
           dsp->numseg--;
        }
        startp = dsp->starts;
        lenp = dsp->lens;
        while (*startp < 0) {
           startp += dsp->dim;
           lenp++;
           dsp->numseg--;
        }
        dsp->starts = startp;
        dsp->lens = lenp;
     }     
  }
  return salp;
}

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignMerge (SeqAlignPtr salp1, SeqAlignPtr salp2, Boolean return_salp)
{
  SeqAlignPtr salp_toreturn;
  DenseSegPtr dsp1= NULL, 
              dsp2 = NULL;
  Int4Ptr     dspstarts= NULL;
  Int4Ptr     dsptmp= NULL;
  Int4Ptr     dsplens= NULL;
  Uint1Ptr    dspstrands = NULL;
  Uint1Ptr    dsptmp1;
  Int4        j, k, n, newseg;
  Uint1       st1, st2;
  Boolean     merge_segment = FALSE;

  if (salp1==NULL) {
     if (salp2 == NULL)
        return NULL;
     else
        return salp2;
  }
  else if (salp2 == NULL)
     return salp1;
  
  if (return_salp)
     salp_toreturn = salp2;
  else 
     salp_toreturn = salp1;

  if ( salp1->segtype != 2 || salp2->segtype != 2 ) {
     return salp_toreturn; 
  }
  if (return_salp) {
     dsp1 = (DenseSegPtr) salp1->segs;
     dsp2 = (DenseSegPtr) salp2->segs;
  }
  else {
     dsp1 = (DenseSegPtr) salp2->segs;
     dsp2 = (DenseSegPtr) salp1->segs;
  }
  if ( dsp1==NULL || dsp2==NULL || dsp1->dim != dsp2->dim) {
     return salp_toreturn; 
  }
  n = dsp1->dim;
  newseg = dsp1->numseg + dsp2->numseg; 
  dspstarts = (Int4Ptr) MemNew ((size_t) ((n*newseg+4) * sizeof (Int4)));
  if ( dspstarts == NULL ) {
     return salp_toreturn; 
  }
  st1 = SeqAlignStrand (salp1, 0);
  st2 = SeqAlignStrand (salp1, 1);
  dsptmp = dsp1->starts;
  for (j = 0; j < n * dsp1->numseg; j++, dsptmp++) {
     dspstarts [j] = *dsptmp;
  }
  dsptmp = dsp2->starts;
  if (n==2) {
     if (dspstarts [j-2]> -1 && dsptmp[0] > -1) {
        if (dspstarts [j-1] > -1 && dsptmp[1] > -1) 
           merge_segment = TRUE;
     }    
  }
  if (merge_segment) {
     if (st1==Seq_strand_minus)
        dspstarts [j-2] = dsptmp[0];
     if (st2==Seq_strand_minus)
        dspstarts [j-1] = dsptmp[1];
     newseg--;
     k=n;
     dsptmp += n;
  }
  else 
     k = 0;
  for (; k < n * dsp2->numseg; k++, j++, dsptmp++) {
     dspstarts [j] = *dsptmp;
  }
  dsplens  = (Int4Ptr) MemNew ((size_t) ((newseg + 2) * sizeof (Int4))); 
  if ( dsplens == NULL ) {
     return salp_toreturn; 
  }
  dsptmp = dsp1->lens;
  for (j = 0; j < dsp1->numseg; j++, dsptmp++) 
     dsplens [j] = *dsptmp;
  dsptmp = dsp2->lens;
  if (merge_segment) {
     dsplens [j-1] += *dsptmp;
     k=1;
     dsptmp ++;
  } 
  else
     k = 0; 
  for (; k < dsp2->numseg; k++, j++, dsptmp++) 
     dsplens [j] = *dsptmp;
  if (dsp1->strands && dsp2->strands)
  {
     dspstrands = (Uint1Ptr) MemNew ((size_t) ((n*newseg+4) * sizeof (Uint1)));
     if ( dspstrands != NULL ) {
        dsptmp1=dsp1->strands;
        for (j = 0; j < n * dsp1->numseg; j++, dsptmp1++) {
           dspstrands [j] = *dsptmp1;
        }
        dsptmp1 = dsp2->strands; 
        if (merge_segment) {
           k=n; 
           dsptmp1 += n;
        } 
        else 
           k = 0; 
        for (; k < n * dsp2->numseg; k++, j++, dsptmp1++) {
           dspstrands [j] = *dsptmp1;
        }
     }
  }
  dsp1 = (DenseSegPtr) salp1->segs;
  dsp1->numseg = newseg;
  MemFree(dsp1->starts);
  dsp1->starts = dspstarts;
  MemFree(dsp1->lens);
  dsp1->lens = dsplens;
  MemFree(dsp1->strands);
  dsp1->strands = dspstrands; 
  return salp1;
}

NLM_EXTERN SeqAnnotPtr LIBCALL SeqAnnotMerge (SeqAnnotPtr sap1, SeqAnnotPtr sap2, Boolean return_salp)
{
  SeqAlignPtr salp1=NULL, salp2=NULL;

  if ( sap1 == NULL ) return sap2;
  if ( sap2 == NULL ) return sap1;
  if ( sap1->type != 2 ||  sap2->type != 2 ) {
         return NULL; 
  }
  salp1 = (SeqAlignPtr) sap1->data;
  salp2 = (SeqAlignPtr) sap2->data;
  if (return_salp) 
     salp1 = SeqAlignMerge (salp1, salp2, TRUE);
  else 
     salp1 = SeqAlignMerge (salp1, salp2, FALSE);
  return sap1;
}

static SeqAlignPtr LIBCALL SeqAlignAddSeg (SeqAlignPtr salp, Int4 pos1, Int4 pos2, Int4 len)
{
  DenseSegPtr dsp;
  Int4Ptr     startp;
  Int4Ptr     lenp;
  Uint1Ptr    dspstrands,
              dsptmp1;
  Int4Ptr     dsptmp;
  Int4        j;

  dsp = (DenseSegPtr) salp->segs;

  startp = (Int4Ptr) MemNew ((size_t) ((dsp->dim*dsp->numseg+4) * sizeof (Int4)));
  if ( startp == NULL ) {
     return NULL;
  }
  dsptmp = dsp->starts;
  for (j = 0; j < dsp->dim*dsp->numseg; j++, dsptmp++) {
     startp [j] = *dsptmp;
  }
  startp[j] = pos1;
  startp[j+1] = pos2;
  MemFree(dsp->starts);
  dsp->starts = startp;

  lenp  = (Int4Ptr) MemNew ((size_t) ((dsp->numseg + 2) * sizeof (Int4)));
  if ( lenp == NULL ) {
     return NULL;
  }
  dsptmp = dsp->lens;
  for (j = 0; j < dsp->numseg; j++, dsptmp++)
     lenp [j] = *dsptmp;
  lenp [j] = len;
  MemFree(dsp->lens);
  dsp->lens = lenp;

  dspstrands = (Uint1Ptr) MemNew ((size_t) ((dsp->dim*dsp->numseg+4) * sizeof (Uint1)));
  if ( dspstrands == NULL ) {
     return NULL;
  }
  dsptmp1=dsp->strands;
  for (j = 0; j < dsp->dim * dsp->numseg; j++, dsptmp1++) {
     dspstrands [j] = *dsptmp1;
  }
  dspstrands [j] = Seq_strand_unknown;
  dspstrands [j+1] = Seq_strand_unknown;
  MemFree(dsp->strands);
  dsp->strands = dspstrands;

  dsp->numseg += 1;
  return salp; 
}

static SeqAlignPtr LIBCALL shrinksap5 (SeqAlignPtr salp, Int4 offset)
{
  DenseSegPtr dsp;
  Int4Ptr     dsptmp;
  Int4        j;

  dsp = (DenseSegPtr) salp->segs;
  dsptmp = dsp->starts;
  for (j = 0; j < dsp->dim; j++)
     dsptmp[j] += offset; 
  dsptmp = dsp->lens;
  *dsptmp -= offset;
  return salp;
}

static SeqAlignPtr LIBCALL shrinksap3 (SeqAlignPtr salp, Int4 offset)
{
  DenseSegPtr dsp;
  Int4Ptr     dsptmp;

  dsp = (DenseSegPtr) salp->segs;
  dsptmp = dsp->lens;
  *dsptmp -= offset;
  return salp;
}

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignExtend (SeqAlignPtr salp1, SeqAlignPtr salp2)
{
  SeqAlignPtr salptmp;
  SeqLocPtr   slp1, slp2,
              slp1b, slp2b;
  ValNodePtr  vnp1, vnp2,
              slpt1, slpt2;
  Int4        gaplen;
  Int4        gaplen1, stop, stopb;
  Int4        j;
  Uint1       choice;
  Boolean     goOn=TRUE;

  slpt1 = SeqLocListFromSeqAlign (salp1);
  while (goOn) {
     goOn = FALSE;
     for (salptmp = salp2; salptmp!=NULL; salptmp=salptmp->next) {
        slpt2 = SeqLocListFromSeqAlign (salptmp);   
        choice = 0;
        for (vnp1=slpt1, vnp2=slpt2, j=0; vnp1!=NULL && vnp2!=NULL; j++) {
           slp1=(SeqLocPtr)vnp1->data.ptrvalue;   
           slp2=(SeqLocPtr)vnp2->data.ptrvalue;   
           gaplen1 = SeqLocStart(slp2) - SeqLocStop(slp1);
           if (gaplen1 ==0) {
              if (j==0) choice =1;
              else choice =2;
              break;
           }
           gaplen1 = SeqLocStart(slp1) - SeqLocStop(slp2);
           if (gaplen1 ==0 || gaplen1==1 || gaplen1==-1 || gaplen1==-2) {
              if (j==0) choice = 3;
              else choice =4;
              break;
           }
           vnp1=vnp1->next;
           vnp2=vnp2->next;
        } 
        if (choice) {
           if (choice==1 || choice==3) {
              vnp1=vnp1->next;
              vnp2=vnp2->next;
           }
           else {
              vnp1=slpt1;
              vnp2=slpt2;
           }
           slp1b=(SeqLocPtr)vnp1->data.ptrvalue;
           slp2b=(SeqLocPtr)vnp2->data.ptrvalue;
           if (choice ==1 || choice == 2) {
              gaplen = SeqLocStart(slp2b) - SeqLocStop(slp1b) -1;   
              if (gaplen >= 1) {
                 stop = SeqLocStop(slp1);
                 stopb = SeqLocStop(slp1b);
                 if (gaplen1==0) {
                    shrinksap5 (salptmp, 1);
                    gaplen-=1;
                 }
                 if (gaplen > 1) {
                    if (choice ==1) {
                       SeqAlignAddSeg(salp1, -1, stopb+1, gaplen);
                    }
                    else {
                       SeqAlignAddSeg(salp1,stop+1, -1, gaplen);
                    }
                 }      
                 salp1 = SeqAlignMerge (salp1, salptmp, TRUE);
                 goOn=TRUE;
              } 
           } else {
              gaplen = SeqLocStart(slp1b) - SeqLocStop(slp2b) -1;
              if (gaplen >= 1) { 
                 stop = SeqLocStop(slp2);
                 stopb = SeqLocStop(slp2b);
                 if (gaplen1==0 || gaplen1==-1 || gaplen1==-2) {
                    gaplen1=ABS(gaplen1)+1;
                    shrinksap3 (salptmp, gaplen1);
                    stop-=gaplen1;
                    stopb-=gaplen1;
                    gaplen+=gaplen1;
                 }
                 if (gaplen > 1) {      
                    if (choice ==3) {
                       SeqAlignAddSeg(salptmp,-1,stop+1, gaplen); 
                    }
                    else {
                       SeqAlignAddSeg(salptmp, stopb+1,-1, gaplen); 
                    }
                 }
                 salp1 = SeqAlignMerge (salp1, salptmp, FALSE);
                 goOn=TRUE;
              }
           }   
           if (goOn) {
              ValNodeFreeType (&slpt1, TypeSeqLoc);
              slpt1 = SeqLocListFromSeqAlign (salp1); 
              break;
           }
        } 
        ValNodeFreeType (&slpt2, TypeSeqLoc);
     }   
  }
  ValNodeFreeType (&slp1, TypeSeqLoc);
  return salp1;
}

NLM_EXTERN SeqAlignPtr LIBCALL CleanStrandsSeqAlign (SeqAlignPtr salp)
{
  SeqAlignPtr salptmp;
  DenseSegPtr dsp;
  CompSegPtr  csp;
  Int4Ptr     lenp;
  Int4Ptr     startp;
  Uint1Ptr    strandp = NULL;
  Int4        numseg;
  Int2        dim;
  Int4        j, k, n, tmp;
  Boolean     retourne = FALSE;
  Boolean     delete_me = FALSE;

  for (salptmp=salp; salptmp!=NULL; salptmp=salptmp->next) 
  {
     if (salptmp->segtype == COMPSEG) 
     {   
        csp=(CompSegPtr) salptmp->segs;
        strandp = csp->strands;
        numseg = csp->numseg;
        dim = csp->dim;
        lenp = csp->lens;
     }
     else if (salptmp->segtype == 2) 
     {  
        dsp = (DenseSegPtr) salptmp->segs;
        strandp = dsp->strands;
        numseg = dsp->numseg;
        dim = dsp->dim;
        lenp = dsp->lens;
        startp = dsp->starts;
     }
     if (strandp!=NULL) {
        for (j=0; j<numseg*dim; j++)
           if (*strandp==Seq_strand_minus)
              break;
        delete_me = (Boolean)(j==numseg*dim);
     }
     if (delete_me) {
        if (salptmp->segtype == COMPSEG)
           csp->strands = NULL;
        else if (salptmp->segtype == 2)
           dsp->strands = NULL;
        MemFree (strandp);
        return salp;
     }
     if (strandp!=NULL) {
        if (*strandp != Seq_strand_plus && *strandp!=Seq_strand_minus)
           for (j=0; j<numseg; j++, strandp+=dim) {
              if (*strandp == Seq_strand_plus || *strandp==Seq_strand_minus)
                 break;
           }
        if (strandp!=NULL) 
           retourne = (Boolean) (*strandp == Seq_strand_minus);
     }
     if (retourne) {
        for (j=0; j < numseg*dim && strandp!=NULL; j++, strandp++) 
        {
           if (*strandp == Seq_strand_minus) 
              *strandp = Seq_strand_plus;
           else if (*strandp == Seq_strand_plus)
              *strandp = Seq_strand_minus;
        }
        for (j=0, k=numseg-1; j<numseg/2; j++, k--) {
           tmp=lenp[j];
           lenp[j]=lenp[k];
           lenp[k]=tmp;
        }
        for (j=0, k=(dim*numseg-dim); j<(dim*numseg-1)/2; j+=dim, k-=dim) {
           for (n=0; n<dim; n++) {
              tmp=startp[j+n];
              startp[j+n]=startp[k+n];
              startp[k+n]=tmp;
           }
        }
     }
  }
  return salp;
}

/*************************************************
***  
***      LocalAlignToSeqAnnotDimn
***              
*************************************************/
NLM_EXTERN SeqAnnotPtr LIBCALL LocalAlignToSeqAnnotDimn (ValNodePtr seqvnp, SeqIdPtr seqsip, ValNodePtr fromp, Int2 nbseq, Int4 lens, ValNodePtr strands, Boolean trunc_emptyends)
{
  SeqAnnotPtr  sap;
  SeqAlignPtr  salp;
  DenseSegPtr  dsp;
  ValNodePtr   vnp;
  CharPtr      seqstr;
  Boolean PNTR filter;
  Boolean PNTR begun;
  Int4Ptr      from;
  Int4Ptr      lgseq;
  Int4Ptr      start;
  Uint1Ptr     strandp=NULL;
  Uint1        seq_strand;
  Int4         lenstmp, the_lens;
  Int4         j;
  Int2         seg, numseg;
  Int2         k;
  Boolean      nogap;

  for (k =0, vnp =seqvnp; k < nbseq && vnp !=NULL; k++, vnp =vnp->next)
  {
         seqstr = (CharPtr) vnp->data.ptrvalue;
         lenstmp = StringLen (seqstr);
         if (k==0) 
            the_lens = lenstmp;
         else if (lenstmp != the_lens) {
            ErrPostEx (SEV_ERROR, 0, 0, "Sequence alignment of different lengths"); 
            return NULL;
         }
  }
  lens = the_lens;

         /*****************************
         ** count number of segments **
         *****************************/
  filter= (BoolPtr)MemNew ((size_t) ((nbseq + 1) * sizeof (Boolean)));
  j = 0;
  for (k =0, vnp =seqvnp; k < nbseq && vnp !=NULL; k++, vnp =vnp->next) 
  {
         seqstr = (CharPtr) vnp->data.ptrvalue;
         filter [k] = (Boolean)( seqstr [j] != '-' );
  }
  numseg = 1;
  while ( j < lens ) 
  {
         seg = 0;
         for (k =0, vnp =seqvnp; k <nbseq && vnp !=NULL; k++, vnp =vnp->next) 
         {
                seqstr = (CharPtr) vnp->data.ptrvalue;
                nogap = (Boolean) ( seqstr [j] != '-' );
                if ( filter [k] != nogap ) {
                   seg++;
                   filter [k] = nogap;
                }
         } 
         if ( seg > 0 ) ++numseg;
         j++;
  }
         /********************************************
         ** allocate SeqAnnot + SeqAlign + DenseSeg  **
         *********************************************/
  sap = SeqAnnotNew ();
  if (sap == NULL) return NULL;
  sap->type = 2;
  salp = SeqAlignNew ();
  if (salp == NULL) return NULL;
  salp->type = 3;
  salp->segtype = 2;
  salp->dim = nbseq;
  sap->data = (Pointer) salp;
  dsp = DenseSegNew ();
  salp->segs = (Pointer) dsp;
  dsp->dim = nbseq;
  dsp->ids = SeqIdDupList (seqsip);
  dsp->numseg = numseg;
  dsp->starts = (Int4Ptr)MemNew((size_t)((nbseq *numseg + 4) * sizeof (Int4)));
  for (j = 0; j < nbseq *numseg + 4; j++) 
     dsp->starts[j] = -1;
  dsp->lens   = (Int4Ptr) MemNew ((size_t) ((numseg + 2) * sizeof (Int4))); 
  for (k = 0; k < numseg + 2; k++) 
     dsp->lens[k] = 0;
  if (strands)
  {
     dsp->strands = (Uint1Ptr)MemNew((size_t) ((numseg*nbseq+4)*sizeof (Uint1)));
     strandp = dsp->strands;
     for (j = 0; j < numseg*nbseq ; j++, strandp++) 
        *(strandp) = Seq_strand_unknown; 
     strandp = dsp->strands;
     for (k=0; k<numseg; k++) {
        for (j = 0, vnp=strands; j < nbseq && vnp!=NULL; j++, vnp=vnp->next) {
           *strandp = (Uint1)vnp->data.intvalue;
           strandp++;
        }
     }
  }
  j = 0;
  for (k =0, vnp =seqvnp; k < nbseq && vnp !=NULL; k++, vnp =vnp->next)
  {
         seqstr = (CharPtr) vnp->data.ptrvalue;
         filter [k] = (Boolean)( seqstr [j] != '-' );
  }
  lenstmp= 0;
  numseg = 0;
  while ( j < lens )
  {
         seg = 0;
         for (k =0, vnp =seqvnp; k <nbseq && vnp !=NULL; k++, vnp =vnp->next)
         {
            seqstr = (CharPtr) vnp->data.ptrvalue;
            nogap = (Boolean) ( seqstr [j] != '-' );
            if ( filter [k] != nogap ) {
               seg++;
               filter [k] = nogap;
            }
         }
         if ( seg > 0 ) {
            dsp->lens[numseg] = lenstmp;
            ++numseg;
            lenstmp = 0;
         }
         lenstmp++;
         j++;
  }
  if (lenstmp > 0)
     dsp->lens[numseg] = lenstmp;
         /******************************
         ***  store the segments      **
         ******************************/
  lgseq = (Int4Ptr)MemNew ((size_t) ((nbseq + 1) * sizeof (Int4)));
  from = (Int4Ptr)MemNew ((size_t) ((nbseq + 1) * sizeof (Int4)));
  begun = (BoolPtr)MemNew ((size_t) ((nbseq + 1) * sizeof (Boolean)));
  if ( lgseq == NULL || from == NULL || begun == NULL )
     return NULL;
  for (k = 0; k < nbseq; k++) 
     lgseq[k] = 0;
  if (fromp == NULL)
     for (k = 0; k < nbseq; k++) from [k] = 0;
  else {
     for (k=0, vnp=fromp; k<nbseq && vnp!=NULL; vnp=vnp->next, k++)
     {
        from [k] = (Int4)vnp->data.intvalue; 
     }
  }
  for (k = 0; k < nbseq; k++) 
     begun[k] = FALSE;
  start = dsp->starts;
  strandp = dsp->strands;
  j = 0;
  for (k =0, vnp =seqvnp; k <nbseq && vnp !=NULL; k++, vnp =vnp->next) 
  {
     seqstr = (CharPtr) vnp->data.ptrvalue;
     filter [k] = (Boolean)( seqstr [j] != '-' );
     if ( filter [k] ) {
        start [k] = lgseq [k] + from [k];
        begun [k] = TRUE;
     }
  }
  j++;
  numseg = 0;
  while ( j < lens ) 
  {
         seg = 0;
         for (k =0, vnp =seqvnp; k <nbseq && vnp !=NULL; k++, vnp =vnp->next) 
         {
                seqstr = (CharPtr) vnp->data.ptrvalue;
                nogap = ( seqstr [j] != '-' );
                if  ( nogap && begun [k] ) 
                   ++lgseq [k]; 
                if ( filter [k] != nogap ) {
                   seg++;
                   filter[k] = nogap;
                   begun [k] = TRUE;
                }
         }   
         if ( seg > 0 ) {
                start += nbseq;
                for (k = 0; k < nbseq; k++) {
                   if (filter[k]) {
                      seq_strand = Seq_strand_unknown;
                      if (strandp)
                         seq_strand = strandp[k];
                      if (seq_strand==Seq_strand_minus) {
                         start[k] = from[k] - lgseq[k];
                      } else {
                         start[k] = from[k] + lgseq[k];
                      }
                   }
                }
                ++numseg;
         }
         j++;
  }
  MemFree (filter);
  MemFree (lgseq);
  MemFree (from);
  MemFree (begun);
  if (trunc_emptyends && salp!=NULL) {
     nogap = TRUE;
     dsp = (DenseSegPtr) salp->segs;
     while (nogap && dsp->numseg>0) {
        start = dsp->starts;
        start += (dsp->dim * (dsp->numseg-1) );
        for (j=0; j < dsp->dim; j++, start++) {
           if (*start > -1) 
              break;
        }   
        if (j == dsp->dim)
           dsp->numseg --;
        else nogap = FALSE;
     }
  }
  if (strandp)
  {
     for (k=0, vnp=seqvnp; k <nbseq && vnp!=NULL; k++, vnp=vnp->next) {
        if (dsp->strands[k] == Seq_strand_minus) {
           start = dsp->starts; 
           start += k;
           lgseq = dsp->lens;
           for (j=0; j<dsp->numseg; j++, lgseq++) {
              if (*start > -1) {
                 *start -= *lgseq;
              }
              start += nbseq;
           }
        }
     }
  }
  if (salp == NULL || dsp->numseg == 0) {
     sap = SeqAnnotFree (sap);
     return NULL;
  }
  if (salp == NULL) {  
     sap = SeqAnnotFree (sap);
     return NULL; 
  } 
  return sap;
}



NLM_EXTERN Boolean LIBCALL SeqAlignIDCache (SeqAlignPtr salphead, SeqIdPtr sip)
{
  SeqAlignPtr salp;
  SeqIdPtr    tmpsip;
  Boolean     ok = FALSE;

  for (salp = salphead; salp!= NULL; salp=salp->next)
  {
     tmpsip =SeqIdPtrFromSeqAlign (salp);
     if ((SeqIdOrderInBioseqIdList(sip, tmpsip)) > 1) 
     {
        if (salp->segtype == 1 || salp->dim == 2) 
        {
           salp->type = 0;
           ok = TRUE;
        } 
        else if (salp->segtype == 2)
        {
           SeqAlignBioseqDeleteById (salp, sip); 
           ok = TRUE;
        }
     }
  }
  return ok; 
}

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignIDUncache (SeqAlignPtr salphead, SeqIdPtr sip)
{
  SeqAlignPtr salp;
  SeqIdPtr    tmpsip;

  for (salp = salphead; salp!= NULL; salp=salp->next)
  {
     tmpsip =SeqIdPtrFromSeqAlign (salp);
     if ((SeqIdOrderInBioseqIdList(sip, tmpsip)) > 1) 
     {
        if (salp->segtype == 1 || salp->dim == 2) 
        {
           salp->type = 3;
        } 
     }
  }
  return salphead; 
}

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignIDUncacheAll (SeqAlignPtr salphead)
{
  SeqAlignPtr salp;


  for (salp = salphead; salp!= NULL; salp=salp->next)
  {
     if (salp->type < 1)
        if (salp->segtype == 1 || salp->dim == 2) 
           salp->type = 3;
  }
  return salphead; 
}

/**************************************************
***
***************************************************/
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignDeleteByLoc (SeqLocPtr slp, SeqAlignPtr salp)
{
  SeqIdPtr    sip;
  DenseSegPtr dsp;
  Int4Ptr     dspstart=NULL;
  Int4Ptr     newstartp=NULL, 
              newstart=NULL;
  Int4Ptr     dsplens=NULL;
  Int4Ptr     newlensp=NULL, 
              newlens=NULL;
  Uint1Ptr    strandp=NULL;
  Uint1Ptr    newstrdp=NULL, 
              newstrd=NULL;
  Int4        from;
  Int4        sumlens = 0;
  Int4        seqlens = 0;
  Int4        lensplus;
  Int4        position;
  Int2        newseg;
  Int2        j, tmp;
  Int2        numseg;
  Int2        inter_salp = 0;
  Int2        index;
  Boolean     seen = FALSE;
  Boolean     delete_before = FALSE;
  Int2    intersalpwidth=0;
  Int2    is_end=0;
  Int4        num_new_strands;
  Int4        num_addl_strands;

  if (salp == NULL)
     return NULL;
  sip = SeqLocId(slp);
  dsp = (DenseSegPtr) salp->segs;
  if (dsp == NULL) {
         return salp;
  }
  index = SeqIdOrderInBioseqIdList (sip, dsp->ids);
  if (index == 0) {
         return salp;
  }
  index -= 1;
  from = SeqAlignStart(salp, index);
/**/     
  delete_before = (Boolean) (SeqLocStart(slp) <= from);
  if (delete_before) 
     position = SeqLocStop(slp) +1;
  else
     position = SeqLocStart(slp);
/**/
  dspstart = dsp->starts + index;
  dsplens = dsp->lens;
  if (dspstart == NULL || dsplens == NULL ) {
         return salp;
  }
  numseg = 1;
  while ( !seen && numseg <= dsp->numseg ) 
  {
     if (position>=from+seqlens && position<from+seqlens+*dsplens)
     {
        if (*dspstart > -1)
           lensplus = (Int4)ABS(position - from - seqlens);
        seen = TRUE;
     }
     else if (*dspstart > -1 && position<=from+seqlens+*dsplens 
     && is_end==APPEND_RESIDUE) {
                lensplus = (Int4)ABS(position - from - seqlens);
                seen = TRUE;
     }
     else if ( numseg == dsp->numseg )
     {
                break;
    }
     else if (numseg < dsp->numseg)
     {
                sumlens += *dsplens;
                if (*dspstart > -1) 
                   seqlens += *dsplens;
                dspstart += dsp->dim;
                dsplens++;
     }
     if ( !seen ) numseg++;
  }       
  if ( !seen ) {
    return salp;
  }
  if (position != from+seqlens) 
  {
     newseg = dsp->numseg+1;
     newstart =(Int4Ptr)MemNew((size_t)((dsp->dim*newseg+4)*sizeof (Int4)));
     newlens  =(Int4Ptr)MemNew((size_t) ((newseg + 2) * sizeof (Int4)));
     if (dsp->strands!=NULL) {
        num_new_strands = dsp->dim*newseg+4;
        newstrd  =(Uint1Ptr)MemNew((size_t)((num_new_strands)*sizeof(Uint1)));
     } else {
        num_new_strands = 0;
     }
     if (newstart!=NULL && newlens!=NULL) 
     {
        if (dsp->strands!=NULL) 
        {
            MemCpy (newstrd, dsp->strands, dsp->dim * dsp->numseg);
            num_addl_strands = num_new_strands - dsp->dim * dsp->numseg;
            if (num_addl_strands > dsp->dim * dsp->numseg) {
              num_addl_strands = dsp->dim * dsp->numseg;
            }
            MemCpy (newstrd + dsp->dim * dsp->numseg, dsp->strands, num_addl_strands);
        }
        dspstart = dsp->starts;
        dsplens = dsp->lens;
        newstartp = newstart;
        newlensp = newlens;
/*------*/
        if (delete_before) 
        {
           for(tmp=0; tmp<numseg-1; tmp++) {
              for (j = 0; j < dsp->dim; j++) {
                 if (j == index)
                    *newstartp = -1;
                 else
                    *newstartp = *dspstart;
                 newstartp++; 
                 dspstart++;
              }
              *newlensp = *dsplens;
              newlensp++; dsplens++;
           }        
        }
        else {
           for(tmp=0; tmp<numseg -1; tmp++) {
              for (j = 0; j < dsp->dim; j++) {
                 *newstartp = *dspstart;
                 newstartp++; 
                 dspstart++;
              }
              *newlensp = *dsplens;
              newlensp++; dsplens++;
           }
        }
/*------*/
        *newlensp = lensplus+intersalpwidth*inter_salp;
        for (j = 0; j < dsp->dim; j++) {
           if (delete_before)
           {
              if (j == index) {
                 *newstartp = -1;
                 *(newstartp + dsp->dim) = *dspstart + *newlensp;
              }
              else {
                 *newstartp = *dspstart;
                 if (*dspstart < 0)
                    *(newstartp + dsp->dim) = -1;
                 else
                    *(newstartp + dsp->dim) = *dspstart + *newlensp;
              }
           }
           else {           
              *newstartp = *dspstart;
              if (j == index || *dspstart < 0)
                 *(newstartp + dsp->dim) = -1;
              else 
                 *(newstartp + dsp->dim) = *dspstart + *newlensp;
           }
           newstartp++; 
           dspstart++;
        }
        newstartp += dsp->dim;
        newlensp++; 
        tmp++;
        *newlensp = *dsplens-(lensplus+intersalpwidth*inter_salp);
        newlensp++; 
        dsplens++;
/*------*/
        if (delete_before)
        {
           for(; tmp < dsp->numseg; tmp++) {
              for (j = 0; j < dsp->dim; j++) {
                 *newstartp = *dspstart;
                 newstartp++; 
                 dspstart++;
              }
              *newlensp = *dsplens;
              newlensp++; dsplens++;
           }
        }
        else 
        {
           for(; tmp < dsp->numseg; tmp++) {
              for (j = 0; j < dsp->dim; j++) {
                 if (j == index)
                    *newstartp = -1; 
                 else
                    *newstartp = *dspstart;
                 newstartp++; 
                 dspstart++;
              }
              *newlensp = *dsplens;
              newlensp++; dsplens++;
           }
        }
/*------*/
        dsp->numseg = newseg;
        dspstart = dsp->starts;
        dsp->starts = newstart;
        MemFree (dspstart);
        dsplens = dsp->lens;
        dsp->lens = newlens;
        MemFree (dsplens);
        strandp = dsp->strands;
        dsp->strands = newstrd;
        MemFree (strandp);
     }
     if (delete_before)
     {
        sumlens = from;
        dspstart = dsp->starts + index;
        dsplens = dsp->lens;
        for(tmp=0; tmp< dsp->numseg; tmp++) {
           if (*dspstart > -1) {
              *dspstart = sumlens;
              sumlens+=*dsplens;
           }
           dspstart += dsp->dim;
           dsplens++; 
        }
     }
  }
  else if (position == from+seqlens && numseg <= dsp->numseg) {
     if (delete_before) 
     {
        dspstart = dsp->starts + index;
        for(tmp=0; tmp< numseg-1; tmp++) {
           *dspstart = -1;
           dspstart += dsp->dim;
        }     
        /*------------------*/ 
        sumlens = from;
        dspstart = dsp->starts + index;
        dsplens = dsp->lens;
        for(tmp=0; tmp< dsp->numseg; tmp++) {
           if (*dspstart > -1) {
              *dspstart = sumlens;
              sumlens+=*dsplens;
           }
           dspstart += dsp->dim;
           dsplens++; 
        }
     }
     else {
        while (numseg <= dsp->numseg) {
           *dspstart = -1;
           numseg++;
           dspstart += dsp->dim;
        }
     }
  }
  return salp;
}

/*******************************************
***
***   DeleteRegion
***  !@!!!!!!!!!!!!!!!!!!!!!!! > CompSegPtr
********************************************/
NLM_EXTERN SeqAlignPtr LIBCALL DeleteRegion (SeqIntPtr sip, SeqAlignPtr salp)
{
  CompSegPtr  dsp;
  SeqAlignPtr salp1 =  NULL;
  BoolPtr     dspstart = NULL;
  Int4Ptr     dsplens = NULL;
  Int4        delete_from;
  Int4        delete_to;
  Int2        numseg_before = 0;
  Int2        subseglens = 0;
  Int4        sumseglens = 0;
  Boolean     seen = FALSE;

  if ( sip == NULL ) return salp;
  delete_from = sip->from;
  delete_to = sip->to;

        /*****************************************
         *** copy salp(s) until delete_from
         *****************************************/
  if ( (dsp = (CompSegPtr) salp->segs ) == NULL) {
         return NULL;
  }
  dspstart = dsp->starts;
  dsplens = dsp->lens;
  while ( !seen )
  {
         if ( !(seen = locate_in_seqalign (delete_from, dsp->dim, dsp->numseg, &dspstart, &dsplens, &numseg_before, &subseglens, &sumseglens)) ) 
         {
                salp1 = SeqAlignDupAdd (&salp1, salp, 0, 0, 0);
                if ( salp->next == NULL ) break;
                else { 
                      salp = salp->next;
                      dsp = (CompSegPtr) salp->segs;
                      dspstart = dsp->starts;
                      dsplens = dsp->lens;
                }
         }
  }
  if ( !seen ) {
    return NULL;
  }
  salp1 = SeqAlignDupAdd (&salp1, salp, numseg_before, subseglens, TRUE);
        /*****************************************
         *** delete salp until delete_to
         *****************************************/
  seen = FALSE;
  while ( !seen )
  {
         if ( !(seen = locate_in_seqalign (delete_to, dsp->dim, dsp->numseg, &dspstart, &dsplens, &numseg_before, &subseglens, &sumseglens)) ) 
         {
                if ( salp->next == NULL ) break;
                else { 
                      salp = salp->next;
                      dsp = (CompSegPtr) salp->segs;
                      dspstart = dsp->starts;
                      dsplens = dsp->lens;
                }
         }
  }
  if ( !seen ) {
         return NULL;
  }
        /*****************************************
         *** copy salp from delete_to to the end
         *****************************************/
  salp1 = SeqAlignDupAdd (&salp1, salp, numseg_before, subseglens, FALSE);
  if ( ( salp = salp->next) != NULL )
     while ( salp != NULL )
     {
        salp1 = SeqAlignDupAdd (&salp1, salp, 0, 0, 0);
        salp = salp->next;
     }
  return salp1;
}

/*********************************************************
***
***  DenseDiagPtr procedures
***
**********************************************************/

NLM_EXTERN SeqAlignPtr LIBCALL DenseDiagToDenseSegFunc (SeqAlignPtr salp, Boolean add_ends)
{
  BioseqPtr    bsp;
  SeqAlignPtr  newsalp;
  DenseSegPtr  dsp;
  DenseDiagPtr ddp;
  DenseDiagPtr nextddp;
  ValNodePtr   head;
  ValNodePtr   vnp;
  SeqIdPtr     sip1, sip2;
  Int4Ptr      ddp_starts,
               nextddp_starts,
               dspstart;
  Int4Ptr      dsplen;
  Int4         minstart, laststart;
  Int4         ddp_st1, ddp_st2;
  Int4         nextddp_st1, nextddp_st2;
  Int4         ddlen;
  Int4         interlen1, interlen2;
  Int4         intermax, intermin;
  Int4         j;
  Int4         start1 = 0, start2 = 0,
               stop1 = 0, stop2 = 0;
  Int4         slpstart1 = 0, slpstart2 = 0,
               slpstop1 = 0, slpstop2 = 0;
  Uint1        strand1, strand2;
  Uint1Ptr     strandp=NULL;
  Int2         numseg;
  Boolean      delete_me;

  newsalp = SeqAlignNew ();
  newsalp->type = 3;
  newsalp->segtype = 2;
  newsalp->dim = 2;
  dsp = DenseSegNew ();
  newsalp->segs = (Pointer) dsp;
  dsp->dim = 2;

  ddp =(DenseDiagPtr)salp->segs;
  dsp->ids = SeqIdDupList (ddp->id);

  numseg = 0;
  for (ddp =(DenseDiagPtr)salp->segs; ddp != NULL; ddp = ddp->next) 
     numseg++;
  numseg = numseg *3 -2;
 
  head = NULL; 
  laststart = -10;
  for (j=0; j<numseg; j++) {
     minstart = INT4_MAX; 
     nextddp = NULL;
     for (ddp = (DenseDiagPtr)salp->segs; ddp!=NULL; ddp=ddp->next) {
        if (laststart < *(ddp->starts) && minstart > *(ddp->starts)) {
           minstart = *(ddp->starts);
           nextddp = ddp;
        } 
     }
     if (nextddp!=NULL) {
        ValNodeAddPointer(&head, 0, nextddp);
        laststart = *(nextddp->starts);
     }
  }
  if (head==NULL)
     return NULL;
  for (vnp=head; vnp!=NULL; vnp=vnp->next) {
     ddp = (DenseDiagPtr) vnp->data.ptrvalue;
     ddp->next = NULL;
  }
  salp->segs = (Pointer) head->data.ptrvalue;
  head->data.ptrvalue = NULL;
  vnp = head->next;
  nextddp = (DenseDiagPtr) salp->segs;
  for (; vnp!=NULL; vnp=vnp->next) {
     ddp = (DenseDiagPtr) vnp->data.ptrvalue;
     nextddp->next = ddp;
     nextddp = nextddp->next;
     vnp->data.ptrvalue = NULL;
  }
  head = ValNodeFree (head);
  ddp = (DenseDiagPtr) salp->segs;
  ddlen = ddp->len;
  ddp_starts = ddp->starts;
  ddp_st1 = *ddp_starts;
  ddp_starts++;
  ddp_st2 = *ddp_starts;
  ddp_starts++;
  while (ddp!=NULL) {
     delete_me=FALSE;
     if (ddp->next != NULL)
     {   
        nextddp = ddp->next;
        nextddp_starts = nextddp->starts;
        nextddp_st1 = *nextddp_starts;
        nextddp_starts++;
        nextddp_st2 = *nextddp_starts;
        interlen1 = nextddp_st1 - ddp_st1 - ddlen;
        interlen2 = nextddp_st2 - ddp_st2 - ddlen;
        if (interlen1 < 0 || interlen2 < 0) {
           if (interlen1 < 0 && interlen2 < 0) {
              ddp->next = nextddp->next;
              nextddp->next = NULL;
              DenseDiagFree(nextddp);
              delete_me=TRUE;
           }
           else if (interlen1 < 0) {
              if (ABS(interlen1) >= ddlen) {
                ddp->next = nextddp->next;
                nextddp->next = NULL;
                DenseDiagFree(nextddp);
                delete_me=TRUE;
              }  
           }
           else if (interlen2 < 0) {
              if (ABS(interlen2) >= ddlen) {
                ddp->next = nextddp->next;
                nextddp->next = NULL;
                DenseDiagFree(nextddp);
                delete_me=TRUE;
              }  
           }
        }
     } 
     if (!delete_me) {
        ddp = ddp->next;
        if (ddp != NULL)
        {
         ddlen = ddp->len;
         ddp_starts = ddp->starts;
         ddp_st1 = *ddp_starts;
         ddp_starts++;
         ddp_st2 = *ddp_starts;
         ddp_starts++;
        }
     }
  }
  dsp->starts = (Int4Ptr) MemNew ((size_t) ((2*numseg + 4) * sizeof (Int4)));
  dsp->lens  = (Int4Ptr) MemNew ((size_t) ((numseg + 2) * sizeof (Int4))); 
  for (j = 0; j < 2*numseg + 4; j++) dsp->starts [j] = -1;
  for (j = 0; j < numseg + 2; j++)   dsp->lens [j] = 0;
  dspstart = dsp->starts;
  dsplen = dsp->lens;

  ddp =(DenseDiagPtr)salp->segs;
  ddlen = ddp->len;
  ddp_starts = ddp->starts;
  ddp_st1 = *ddp_starts;
  ddp_starts++;
  ddp_st2 = *ddp_starts;
  ddp_starts++;
  numseg = 0;
  while (ddp != NULL) 
  {
     numseg++;
     *dspstart = ddp_st1;
     dspstart++;
     *dspstart = ddp_st2;
     dspstart++;
     if (ddp->next != NULL) 
     {
        nextddp = ddp->next;
        nextddp_starts = nextddp->starts;
        nextddp_st1 = *nextddp_starts;
        nextddp_starts++;
        nextddp_st2 = *nextddp_starts;
        interlen1 = nextddp_st1 - ddp_st1 - ddlen;
        interlen2 = nextddp_st2 - ddp_st2 - ddlen;
        if (interlen1 < 0 || interlen2 < 0) {
           if (interlen1 < 0 && interlen2 < 0) {
              return NULL;
           }
           if (interlen1 < 0) {
              if (ABS(interlen1) < ddlen) ddlen += interlen1;
              else {
                 return NULL;
              }
           }
           if (interlen2 < 0) {
              if (ABS(interlen2) < ddlen) ddlen += interlen2;
              else {
                 return NULL;
              }
           }
           interlen1 = nextddp_st1 - ddp_st1 - ddlen;
           interlen2 = nextddp_st2 - ddp_st2 - ddlen;
        }
        *dsplen = ddlen;
        dsplen++;
        if (interlen1 <= 0)
        {
           if (interlen2>0) {
              numseg++;
              *dspstart = -1;
              dspstart++;
              *dspstart = ddp_st2 + ddlen;
              dspstart++;
              *dsplen = interlen2;
              dsplen++;
           }
        }
        else if (interlen2 <= 0)
        {
           numseg++;
           *dspstart = ddp_st1 + ddlen;
           dspstart++;
           *dspstart = -1;
           dspstart++;
           *dsplen = interlen1;
           dsplen++;
        }
        else if (interlen1 == interlen2)
        {
           numseg++;
           *dspstart = ddp_st1 + ddlen;
           dspstart++;
           *dspstart = ddp_st2 + ddlen;
           dspstart++;
           *dsplen = interlen1;
           dsplen++;
        }
        else 
        {
           if (interlen1 > interlen2) {
              intermax = interlen1;
              intermin = interlen2;
           }
           else  {
              intermax = interlen2;
              intermin = interlen1;
           }
           numseg++;
           *dspstart = ddp_st1 + ddlen;
           dspstart++;
           *dspstart = ddp_st2 + ddlen;
           dspstart++;
           *dsplen = intermin;
           dsplen++;
           numseg++;
           if (interlen1 > interlen2) {
              *dspstart = ddp_st1 + ddlen + intermin;
              dspstart++;
              *dspstart = -1; 
           }
           else {
              *dspstart = -1; 
              dspstart++;
              *dspstart = ddp_st2 + ddlen + intermin;
           }
           dspstart++;
           *dsplen = intermax - intermin;
           dsplen++;
        }
     }
     else {
        *dsplen = ddlen;
        dsplen++;
     }
     ddp = ddp->next;
     if (ddp != NULL) 
     {
        ddlen = ddp->len;
        ddp_starts = ddp->starts;
        ddp_st1 = *ddp_starts;
        ddp_starts++;
        ddp_st2 = *ddp_starts;
        ddp_starts++;
     }
  }
  dsp->numseg = numseg;
  if (add_ends && newsalp!=NULL) 
  {
     strand1 = SeqAlignStrand (salp, 0);
     strand2 = SeqAlignStrand (salp, 1);
     start1 = SeqAlignStart (newsalp, 0);
     start2 = SeqAlignStart (newsalp, 1);
     sip1 = SeqAlignId (newsalp, 0);
     sip2 = SeqAlignId (newsalp, 1);
     slpstart1= 0;
     bsp = BioseqLockById(sip1);
     if (bsp!=NULL) {
        slpstop1 = bsp->length-1;
        BioseqUnlock (bsp);
     }
     else
        slpstop1 = stop1;
     slpstart2 = 0; 
     bsp = BioseqLockById(sip2);
     if (bsp!=NULL) {
        slpstop2 = bsp->length-1;
        BioseqUnlock (bsp);
     }  
     else
        slpstop2 = stop2;
/**
     newsalp = SeqAlignEndExtend (newsalp, slpstart1, slpstart2, -1, -1, start1, start2, -1, -1, strand1, strand2); 
     stop1 = SeqAlignStop (newsalp, 0);
     stop2 = SeqAlignStop (newsalp, 1); 
     newsalp = SeqAlignEndExtend (newsalp, -1, -1, slpstop1, slpstop2, -1, -1, stop1, stop2, strand1, strand2);
**/
     if (strand1!=strand2)
     {
       strandp =(Uint1Ptr)MemNew((size_t)((dsp->dim*dsp->numseg+4)*sizeof(Uint1)));
       dsp->strands = strandp;
       for (j = 0; j < 2*numseg; j+=2) 
          dsp->strands [j] = strand1;
       for (j = 1; j < 2*numseg; j+=2) 
          dsp->strands [j] = strand2;
     }
  } 
  return newsalp;
}

static SeqAlignPtr Compact (SeqAlignPtr salp)
{
  DenseSegPtr     dsp;
  Int4Ptr         startp,
                  stmp,
                  lenp,
                  ltmp;
  Int4            st1, st2, st3, st4;
  Int2            numseg,
                  j, k, n;
                    
  dsp = (DenseSegPtr) salp->segs;
  if (dsp==NULL)
     return salp;
  lenp = (Int4Ptr) dsp->lens;
  startp = (Int4Ptr) dsp->starts;
  j=0;
  numseg=dsp->numseg;
  while(j<numseg-1) {
     st1=*startp;
     st2=*(startp+1);
     st3=*(startp+2);
     st4=*(startp+3);
     if (st1>-1 && st2>-1 && st3>-1 && st4>-1) {
        stmp=startp;
        stmp+=dsp->dim;
        for (k=j; k<dsp->numseg-2; k++) {
           for (n=0; n<dsp->dim; n++) {
              *stmp=*(stmp+dsp->dim);
              stmp++;
           }
        }
        ltmp=lenp;
        ltmp++;
        *lenp += *ltmp;
        for (k=j; k<dsp->numseg-2; k++, ltmp++) {
           *ltmp = *(ltmp+1);
        }
        numseg--;
     }
     j++;
     startp+=dsp->dim;
     lenp++;
  }
  dsp->numseg = numseg;
  return salp;
}

NLM_EXTERN SeqAlignPtr LIBCALL DenseDiagToDenseSeg (SeqAlignPtr salp, Boolean add_ends)
{
  SeqAlignPtr cur = NULL, 
              newsalp = NULL, 
              new1 = NULL, 
              new2 = NULL;

  for (cur=salp; cur!= NULL; cur = cur->next) {
     newsalp = DenseDiagToDenseSegFunc(cur, add_ends);
     newsalp = Compact (newsalp);
     if (newsalp != NULL) {
        if (new1 == NULL) { 
           new1 = newsalp;
           new2 = new1;
        } else {
           new2->next = newsalp; 
           new2 = new2->next;
        }
     }
  }
  return new1;
}

NLM_EXTERN SeqAlignPtr LIBCALL  DenseSegToDenseDiag (SeqAlignPtr salp)
{
  SeqAlignPtr     salptmp,
                  salp2 = NULL,
                  salp2tmp;
  DenseSegPtr     dsp;
  DenseDiagPtr    ddp,
                  ddphead = NULL;
  SeqIdPtr        sip;
  Int4Ptr         startp,
                  lenp;
  Int2            j;

  if (salp!=NULL)
  {
     for (salptmp = salp; salptmp!=NULL; salptmp = salptmp->next)
     {
        if (salptmp->segtype == 2 && salptmp->dim == 2)
        {
           ddphead = NULL;
           dsp = (DenseSegPtr) salptmp->segs;
           if (dsp!=NULL)
           {
              sip = dsp->ids;
              lenp = (Int4Ptr) dsp->lens;
              startp = dsp->starts;
              for (j=0; j<dsp->numseg; j++, lenp++)
              {
                 if (*startp > -1 && *(startp+1) > -1)
                 {
                    ddp = DenseDiagCreate (2, sip, startp, *lenp, NULL, NULL);
                    DenseDiagLink (&ddphead, ddp);
                 }
                 startp += dsp->dim;
              }
           }
           if (ddphead != NULL)
           {
              salp2tmp = SeqAlignNew ();
              if (salp2tmp != NULL) {
                 salp2tmp->type = 3;
                 salp2tmp->segtype = 1;
                 salp2tmp->dim = dsp->dim;
                 salp2tmp->segs = (Pointer) ddphead;
                 salp2 = SeqAlignLink (salp2, salp2tmp);
              }
           }
        }
        else if (salptmp->segtype == 1)
        {
           salp2tmp = SeqAlignDup (salptmp);
           salp2 = SeqAlignLink (salp2, salp2tmp);
        }
     }
  }
  return salp2;
}

NLM_EXTERN DenseDiagPtr LIBCALL DenseDiagCreate (Int4 dim, SeqIdPtr id, Int4Ptr starts, Int4 len, Uint1Ptr strands, ScorePtr scores)

{
  DenseDiagPtr ddp_copy;
  Int4         j;

  ddp_copy = DenseDiagNew();
  ddp_copy->dim = dim;
  if (id != NULL) {
     ddp_copy->id = SeqIdDupList (id);
  }
  ddp_copy->starts = (Int4Ptr)MemNew((size_t)((dim+1)*sizeof(Int4)));
  for (j = 0; j < dim; j++, starts++) {
         ddp_copy->starts [j] = *starts;   
  }
  ddp_copy->len = len;
  if ( strands != NULL ) 
     ddp_copy->strands = strands;
  if ( scores != NULL ) 
     ddp_copy->scores = scores;
  return ddp_copy;
}

NLM_EXTERN DenseDiagPtr LIBCALL DenseDiagLink (DenseDiagPtr *ddp_head, DenseDiagPtr ddp)
{
  DenseDiagPtr ddp_tmp;

  if (ddp!=NULL)
  {
     if (*ddp_head != NULL)
     {
        ddp_tmp = *ddp_head;
        while (ddp_tmp->next != NULL) 
           ddp_tmp = ddp_tmp->next; 
        ddp_tmp->next = ddp;
     } 
     else 
        *ddp_head = ddp;
  }
  return *ddp_head;
}

NLM_EXTERN DenseDiagPtr LIBCALL DenseDiagInsert (DenseDiagPtr ddp_before, DenseDiagPtr ddp)

{
  DenseDiagPtr ddp_tmp;

  if ( (ddp_tmp = ddp_before->next) == NULL) {
         ddp_before->next = ddp;
         return ddp_before;
  }
  ddp_before->next = ddp;
  ddp->next = ddp_tmp;
  return ddp_before;
}

NLM_EXTERN DenseDiagPtr LIBCALL DenseDiagPrecede (DenseDiagPtr ddp_after, DenseDiagPtr *ddp)

{
  DenseDiagPtr ddp_tmp;

  ddp_tmp = *ddp;
  ddp_tmp->next = ddp_after;
  return *ddp;
}

NLM_EXTERN DenseDiagPtr LIBCALL DenseDiagLinkSort (DenseDiagPtr *ddp_head, DenseDiagPtr ddp)

{
  DenseDiagPtr ddp_tmp;

  if ( (ddp_tmp = *ddp_head) != NULL ) {
         if ( *(ddp->starts) < *(ddp_tmp->starts) ) 
                *ddp_head = DenseDiagPrecede (ddp_tmp, &ddp);

         else if ( *(ddp->starts) > *(ddp_tmp->starts) && ddp_tmp->next == NULL)
                ddp_tmp->next = ddp;

         else {
                while ( ddp_tmp->next != NULL ) {
                       if ( *(ddp->starts) < *(ddp_tmp->next->starts) ) break;
                       ddp_tmp = ddp_tmp->next; 
                }
                ddp_tmp = DenseDiagInsert (ddp_tmp, ddp);
         }
  } 
  else *ddp_head = ddp;
  return *ddp_head;
}

NLM_EXTERN SeqAlignPtr LIBCALL SeqLocToFastaSeqAlign (ValNodePtr vnp)
{
  SeqAlignPtr  salp;
  ValNodePtr   vnptmp;
  DenseSegPtr  dsp;
  Int4Ptr      lengthsort;
  Int4         maxlen,
               len, min, pre_min;
  Int4         j, j1;
  Int2         nseq,
               k, numseg;

  nseq = 0;
  for (vnptmp=vnp; vnptmp!=NULL;vnptmp=vnptmp->next)
     if (vnptmp->data.ptrvalue != NULL) nseq++;
  if (nseq == 0)
     return NULL;
  salp = SeqAlignNew ();
  salp->type = 3;
  salp->segtype = 2;
  salp->dim = nseq;
  dsp = DenseSegNew ();
  salp->segs = (Pointer) dsp;
  dsp->dim = nseq;
  dsp->ids = SeqIdListfromSeqLoc (vnp);

         /****************************
         ** count nb of segments
         ****************************/
  maxlen = MaxLengthSeqLoc (vnp);
  lengthsort = (Int4Ptr)MemNew((size_t) ((nseq+1)*sizeof(Int4)));
  pre_min = 0;
  numseg = 1;
  lengthsort [numseg] = 0;
  for ( j = 0; j < nseq; j++ ) 
  {
         vnptmp = vnp;
         min = maxlen;
         for ( k = 0; k < nseq; k++ ) 
         {
                 len = SeqLocLen ((SeqLocPtr) vnptmp->data.ptrvalue);
                 if ( len < min && len > pre_min ) min = len;
                 vnptmp = vnptmp->next;
                 if ( vnptmp == NULL ) break;
         }
         if ( min > pre_min ) 
         {
                  lengthsort [numseg] = min;
                  pre_min = min;
                  numseg++;
         }
  }
         /****************************
         ** copy starts, lens
         ****************************/
  dsp->starts = (Int4Ptr) MemNew ((size_t) ((nseq*numseg + 4) * sizeof (Int4)));
  dsp->lens   = (Int4Ptr) MemNew ((size_t) ((numseg + 2) * sizeof (Int4))); 
  for (j = 0; j < nseq*numseg + 4; j++) dsp->starts[j] = -1;
  for (j = 0; j < numseg + 2; j++) dsp->lens[j] = 0;
  for ( k = 1; k < numseg; k++ ) 
  {
         dsp->lens [k-1] = lengthsort [k] - lengthsort [k-1];
  }
  numseg--;
  dsp->numseg = numseg;
  vnptmp = vnp;
  for ( j = 0; j < nseq; j++ ) 
  {
     if (SeqLocStrand((SeqLocPtr) vnptmp->data.ptrvalue)==Seq_strand_minus)
     {
        j1=nseq*numseg-nseq+j;
        dsp->starts[j1] = SeqLocStart ((SeqLocPtr) vnptmp->data.ptrvalue);
        j1-=nseq;
        for(k=numseg-1;k>-1;k--)
        {
           if (dsp->starts[j1+nseq]+ dsp->lens[k] < SeqLocLen((SeqLocPtr) vnptmp->data.ptrvalue))
              dsp->starts [j1]= dsp->starts[j1+nseq]+ dsp->lens[k];
           j1-=nseq;
        }
     }
     else{
         j1=j;
         dsp->starts[j1] = SeqLocStart ((SeqLocPtr) vnptmp->data.ptrvalue);
         j1+=nseq;
         for ( k = 1; k < numseg; k++ )
         {
            if (dsp->starts[j1-nseq]+ dsp->lens[(k-1)] < SeqLocLen((SeqLocPtr) vnptmp->data.ptrvalue))
               dsp->starts [j1]= dsp->starts[j1-nseq]+ dsp->lens[(k-1)];
            j1+=nseq;
         }
     }    
     vnptmp = vnptmp->next;
     if ( vnptmp == NULL ) break;
  }
  dsp->strands= (Uint1Ptr) MemNew ((size_t) ((numseg*nseq+4)*sizeof (Uint1)));
  j1=0;
  for(k=0; k<numseg; k++)
  {
         vnptmp = vnp;
         for ( j = 0; j < nseq; j++ ) 
         {
            dsp->strands[j1] = SeqLocStrand((SeqLocPtr) vnptmp->data.ptrvalue);
            vnptmp = vnptmp->next;
            j1++;
            if ( vnptmp == NULL ) break;
         }
  }
  MemFree (lengthsort);
  return salp;
}

static Boolean LIBCALL sap_replace (SeqAnnotPtr sap, SeqAlignPtr salp, Uint1 choice)
{
  if (sap != NULL) {
     for (; sap!= NULL; sap=sap->next) {
        if (sap->type == choice) {
           SeqAlignSetFree ((SeqAlignPtr)sap->data);
           sap->data = (Pointer)salp;
           return TRUE;
        }
     }   
  }
  return FALSE;
}

NLM_EXTERN void LIBCALL ReplaceSeqAlignInSeqEntry (Uint2 entityID, Uint2 itemID, SeqAlignPtr salp)
{
  SeqEntryPtr      sep,
                   sep1 = NULL;
  SeqEntryPtr      sept = NULL;
  BioseqSetPtr     bssp = NULL;
  BioseqPtr        bsp = NULL;
  SeqAnnotPtr      sap = NULL;

  sep = GetBestTopParentForItemID (entityID, itemID, OBJ_BIOSEQ);
  if (sep != NULL) {
     if (IS_Bioseq(sep)) {
        entityID = ObjMgrGetEntityIDForChoice (sep);
        sep1 = GetTopSeqEntryForEntityID (entityID);
        bsp = (BioseqPtr) sep->data.ptrvalue;
     }   
     else if(IS_Bioseq_set(sep)) {
        sep1 = sep;
     }   
     if (sep1 != NULL) {
        bssp = NULL; bsp = NULL;
        if (IS_Bioseq(sep1)) {
           bsp = (BioseqPtr) sep1->data.ptrvalue;
           sap_replace(bsp->annot, salp, 2);
        }
        else if(IS_Bioseq_set(sep1)) {
           bssp = (BioseqSetPtr)sep1->data.ptrvalue;
           while (bssp!=NULL && bssp->_class == 7) {
              sept = bssp->seq_set;
              bssp = NULL; bsp = NULL;
              if (IS_Bioseq(sept))  {
                 bsp = (BioseqPtr) sept->data.ptrvalue;
                 break;
              }
              else if (IS_Bioseq_set(sept))
                 bssp = (BioseqSetPtr) sept->data.ptrvalue;
           }
           if (bssp!=NULL) {
              sap = bssp->annot;
              if((sap==NULL || salp==NULL) && IS_Bioseq(sep)) {
                 bsp = (BioseqPtr) sep->data.ptrvalue;
                 sap_replace(bsp->annot, salp, 2);
              }
              else 
                 sap_replace(sap, salp, 2);
              if (sap==NULL && IS_Bioseq_set(sep)) {
                 bssp = (BioseqSetPtr) sep->data.ptrvalue;
                 for (sept = bssp->seq_set; sept!=NULL; sept=sept->next) {
                    if (IS_Bioseq(sept)) {
                       bsp = (BioseqPtr) sept->data.ptrvalue;
                       sap_replace(bsp->annot, salp, 2);
                    }
                 }  
              }
           }
           else if (bsp!=NULL) {
              sap_replace(bsp->annot, salp, 2);
           }
        }
     }   
  }     
  return;
}

/*********************************************************/
static Pointer LIBCALL sap_empty (SeqAnnotPtr sap, Uint1 type, Pointer PNTR ptr)
{
  SeqAlignPtr      salp = NULL;

  if (sap != NULL) {
     for (; sap!= NULL; sap=sap->next) {
        if (sap->type == type) {
           salp = (SeqAlignPtr) sap->data;
           *ptr = (Pointer) sap;
           break;
        }
     }   
  }
  return salp;
}

typedef struct ccid2 {
  Uint1      choice;
  SeqIdPtr   sip;
  Pointer    sap;
} CcId2, PNTR CcId2Ptr;


static void FindSeqAlignCallback (SeqEntryPtr sep, Pointer mydata,
                                          Int4 index, Int2 indent)
{
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  SeqAlignPtr        salp, 
                     salptmp,
                     curr_sap;
  DenseSegPtr        dsp;
  CcId2Ptr           cip;
  Boolean            found;
  Pointer            this_sap;

  cip = (CcId2Ptr)mydata;
  if (sep != NULL && sep->data.ptrvalue && mydata != NULL) {
     if (IS_Bioseq(sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp!=NULL) {
           salp=(SeqAlignPtr)sap_empty(bsp->annot, 2, &this_sap);
           if (salp!=NULL) {
              found=FALSE;
              salptmp=salp;
              if (cip->sip!=NULL) {
                 while (!found && salptmp!=NULL) 
                 {
                   switch (salptmp->segtype) {
                   case 5: /* disc */
                     curr_sap = (SeqAlignPtr)salptmp->segs;
                     while (!found && curr_sap!=NULL) {
                       dsp = (DenseSegPtr)curr_sap->segs;
                       found = (Boolean)(SeqIdOrderInBioseqIdList(cip->sip, dsp->ids)>0);
                       curr_sap = curr_sap->next;
                     }
                     break;
                   default:
                     dsp = (DenseSegPtr)salptmp->segs;
                     found = (Boolean)(SeqIdOrderInBioseqIdList(cip->sip, dsp->ids)>0);
                   }
                   salptmp=salptmp->next;
                 }
              }
              if (found || cip->sip==NULL) {
                 if (cip->sap==NULL) {
                    if (cip->choice==OBJ_SEQALIGN)
                       cip->sap = (Pointer)salp;
                    else if (cip->choice==OBJ_SEQANNOT)
                       cip->sap = (Pointer) this_sap;
                 }
              }
           }
        }
     }
     else if(IS_Bioseq_set(sep)) {
        bssp = (BioseqSetPtr)sep->data.ptrvalue;
        if (bssp!=NULL) {
           salp=(SeqAlignPtr)sap_empty(bssp->annot, 2, &this_sap);
           if (salp!=NULL) {
              found=FALSE;
              salptmp=salp;
              if (cip->sip!=NULL) {
                 while (!found && salptmp!=NULL) 
                 {
                   switch (salptmp->segtype) {
                   case 5: /* disc */
                     curr_sap = (SeqAlignPtr)salptmp->segs;
                     while (!found && curr_sap!=NULL) {
                       dsp = (DenseSegPtr)curr_sap->segs;
                       found = (Boolean)(SeqIdOrderInBioseqIdList(cip->sip, dsp->ids)>0);
                       curr_sap = curr_sap->next;
                     }
                     break;
                   default:
                     dsp = (DenseSegPtr)salptmp->segs;
                     found = (Boolean)(SeqIdOrderInBioseqIdList(cip->sip, dsp->ids)>0);
                   }
                   salptmp=salptmp->next;
                 }
              }
              if (found || cip->sip==NULL) {
                 if (cip->sap==NULL) {
                    if (cip->choice==OBJ_SEQALIGN)
                       cip->sap = (Pointer)salp;
                    else if (cip->choice==OBJ_SEQANNOT)
                       cip->sap = (Pointer) this_sap;
                 }
              }
           }
        }
     }
  }
}

NLM_EXTERN Pointer LIBCALL FindSeqAlignInSeqEntry (SeqEntryPtr sep, Uint1 choice)
{
  SeqEntryPtr      sep_head;
  BioseqPtr        bsp;
  Uint2            entityID;
  CcId2            ci;

  if (sep==NULL)
     return NULL;
  ci.sap = NULL;
  ci.sip = NULL;
  if (choice != OBJ_SEQALIGN && choice != OBJ_SEQANNOT)
     return NULL;
  ci.choice = choice;
  if (IS_Bioseq(sep)) {
     bsp = (BioseqPtr) sep->data.ptrvalue;
     if (bsp!=NULL)
        ci.sip = SeqIdDup (bsp->id);
  }
  entityID = ObjMgrGetEntityIDForChoice (sep);
  sep_head = GetTopSeqEntryForEntityID (entityID);
  SeqEntryExplore (sep_head, (Pointer)&ci, FindSeqAlignCallback);
  if (ci.sip != NULL)
     SeqIdFree (ci.sip);
  return ci.sap;
}

/***********************************************
***  ReadBuffer from spp+sap  
***    in : spp, sap, from + to in seq coordinates
***    out: length of buffer + buffer
************************************************/
NLM_EXTERN Int4 LIBCALL readbuff_fromseqalign (SeqPortPtr spp, SeqAlignPtr salp,  Int2 index, CharPtr buffer, Int4 from, Int4 to, Int4 offset, Boolean strand)
{
  BioseqPtr   bsp;
  DenseSegPtr dsp;
  SeqIdPtr    sip;
  Int4Ptr     dspstart;
  Int4Ptr     dsplens;
  Int4        sumlens, sumstart;
  Int4        bufflen, buffstart;
  Int4        seglenstobuffer = 0;
  Int4        j;
  Int4        maxlen = 0;
  Int2        numseg;
  Boolean     seen = FALSE;
  Boolean     nogap;
  Boolean     ok = TRUE;

  if (spp == NULL) {
    return 0;
  }
  if (buffer == NULL) {
    return 0;
  }
                   /**********************************
                    ***  locate segment including 'from'
                    ***********************************/
  if ( (dsp = (DenseSegPtr) salp->segs ) == NULL) {
         return 0;
  }  
  if (strand == Seq_strand_minus) {
     sip = dsp->ids;
     for (j = 0; sip!=NULL && j < index; j++) 
        sip=sip->next;
     if (sip!=NULL) {
        bsp = BioseqLockById (sip);
        if (bsp!=NULL) {
           maxlen = bsp->length;
           BioseqUnlock(bsp);
        }
     }     
     if (maxlen==0)
        return 0;
  }
  dsplens = dsp->lens;
  dspstart = dsp->starts + index;
  seen = LocateInSeqAlignDenSeg (from, dsp->dim, dsp->numseg, &dspstart, &dsplens, &numseg, &seglenstobuffer);
  if (!seen) {
    ErrPostEx (SEV_ERROR, 0, 0, "fail in readbuff_fromseqalign_sap [4] %ld %ld %ld %ld %ld %ld",
               (long) from, (long) dsp->dim, (long) dsp->numseg, (long) *dspstart, (long) *dsplens, (long) seglenstobuffer);
    return 0;
  }
                    /***********************************
                    ***  read segments until 'to'
                    ***********************************/
  bufflen = MIN((Int4)(*dsplens-seglenstobuffer),(Int4)(to - from));
  if (strand == Seq_strand_minus) {
     buffstart = *dspstart + seglenstobuffer;
     if (buffstart>-1)
        buffstart = maxlen - buffstart - bufflen;
  } else {
     buffstart = *dspstart + seglenstobuffer;
  }
  nogap = (*dspstart >= 0);
  if (offset < 0 && !nogap)
     offset = 0;
  sumlens = bufflen;
  sumstart = 0;
/**
        WriteLog ("/ %d %d %d %d %d %d %d\n", (int)seglenstobuffer,
        (int)buffstart, (int)bufflen, (int) *dsplens, (int)*dspstart, to, from);
**/
  while ( ok ) 
  {
    if ( nogap ) 
    {
       if (strand == Seq_strand_minus) {
        offset = ReadBufferFromSep (spp, buffer, (Int4)buffstart,
                                   (Int4)(buffstart+bufflen), offset);
       } else {
        offset = ReadBufferFromSep (spp, buffer, (Int4)buffstart,
                                   (Int4)(buffstart+bufflen), offset);
       }
    } 
    else 
    {
        for (j = 0; j < bufflen; j++, offset++) 
                buffer[offset] = '-';
        buffer[offset] = '\0';
    }
    ok = (numseg < dsp->numseg);
    if (!ok) 
    {
        if ( salp->next == NULL ) break; 
    }
    else {
        numseg++;
        dspstart += dsp->dim; 
        dsplens++;
    }
    nogap = (*dspstart >= 0);
    bufflen = MIN ((Int4) (*dsplens), (Int4) (to - from));
    if ( nogap ) {
       if (strand == Seq_strand_minus) {
          buffstart = *dspstart + sumstart;
          buffstart = maxlen - buffstart - bufflen; 
       } else {
          buffstart = *dspstart + sumstart;
       }
    }
    sumlens += bufflen;
/**
    if (index != 0) 
        WriteLog ("- %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld \n", 
        (long)offset, (long)seglenstobuffer,
        (long)sumstart, (long)*dspstart, (long)buffstart, 
        (long)*dsplens, (long)from, (long)to, (long)bufflen, (long)sumlens);
**/
  }
  return ((Int4)offset);
}

NLM_EXTERN SeqAlignPtr LIBCALL aaSeqAlign_to_dnaSeqAlign (SeqAlignPtr salp, ValNodePtr vnp, ValNodePtr framep)
{
  SeqAlignPtr      tmp;
  DenseSegPtr      dsp;
  ValNodePtr       vnptmp,
                   fvnp=NULL;
  SeqIdPtr         sip,
                   siptmp;
  Int4Ptr          startp;
  Int4Ptr          lenp;
  Int4             from, sumlens;
  Int2             j, k;
  Uint1            frame;

  if (salp == NULL) 
     return NULL;
  for (tmp=salp; tmp!=NULL; tmp=tmp->next) {
     if (tmp->segtype == 2) {
        dsp = (DenseSegPtr) tmp->segs;
        if (dsp != NULL)  {
  
           if (vnp==NULL) {
              for (j=0, sip = dsp->ids; j<dsp->dim && sip!=NULL; j++, sip=sip->next) {
                 siptmp = MakeNewProteinSeqId (NULL, sip);
                 ValNodeAddPointer (&vnp, 0, siptmp); 
              }
           }
           for (j=0, vnptmp = vnp; vnptmp != NULL; vnptmp = vnptmp->next) {
              if (vnptmp->data.ptrvalue != NULL) 
                 j++;
              else 
                 break;
           }
           if (j == dsp->dim) {

              dsp->ids = SeqIdFree (dsp->ids);
              dsp->ids = ValNodeSeqIdListDup (vnp);
              vnp = ValNodeFreeType (&vnp, TypeSeqId);

              lenp = dsp->lens;
              for (j = 0; j < dsp->numseg; j++, lenp++) {   
                 *lenp = *lenp * 3;
              }
              frame=0;
              if (framep!=NULL) {
                 fvnp = framep;
              }
              for (k = 0; k < dsp->dim; k++) 
              {  
                 lenp = dsp->lens;
                 startp = dsp->starts;
                 startp += k;
                 while (*startp < 0) startp += dsp->dim;
                 from = *startp;
                 if (fvnp!=NULL)
                    frame=(Uint1) fvnp->data.intvalue;
                 if (frame == 2)
                    from +=1;
                 else if (frame == 3)
                    from+=2;
                 startp = dsp->starts;
                 startp += k;
                 sumlens = 0;
                 for (j = 0; j < dsp->numseg; j++) {
                    if (*startp > -1) {
                       *startp = from + sumlens;
                       sumlens += *lenp;
                    }
                    startp += dsp->dim;
                    lenp++;
                 }
                 if (framep!=NULL) 
                    fvnp=fvnp->next;
              }
           }
        }
     }
  }
  return salp;
}


NLM_EXTERN SeqAnnotPtr aaSeqAnnot_to_dnaSeqAnnotFunc (SeqAnnotPtr PNTR sapnahead, SeqAlignPtr salpnew, ValNodePtr vnp, ValNodePtr framep)
{
  SeqAnnotPtr      sap1, sap2, sapna, saptmp;
  SeqAlignPtr      salpna;

  sap1 = SeqAnnotForSeqAlign (salpnew);
  sap2 = (SeqAnnotPtr) AsnIoMemCopy ((Pointer) sap1, (AsnReadFunc) SeqAnnotAsnRead, (AsnWriteFunc) SeqAnnotAsnWrite);
  salpna = aaSeqAlign_to_dnaSeqAlign ((SeqAlignPtr)(sap2->data), vnp, framep);
  sap1->data = NULL;
  sap2->data = NULL;
  SeqAnnotFree (sap1);
  SeqAnnotFree (sap2);
  if (salpna != NULL) {
     sapna = SeqAnnotForSeqAlign (salpna);
     if (*sapnahead == NULL)
	*sapnahead = sapna;
     else {
        for (saptmp=*sapnahead; saptmp->next!=NULL; saptmp=saptmp->next)
           continue;
        saptmp->next = sapna;
     }
  }
  return *sapnahead;
}

NLM_EXTERN SeqAlignPtr LIBCALL SortSeqAlign (SeqAlignPtr PNTR salp)
{
  SeqAlignPtr      salp1, salptmp,
                   newsalp = NULL, 
                   presalp = NULL, 
                   minsalp = NULL, 
                   preminsalp = NULL;
  DenseSegPtr      dsp,
                   dsptmp;
  SeqIdPtr         sip, siptmp;
  Int4Ptr          start;
  Int4             minstart;
  BioseqPtr        bsp;

  salp1=*salp;
  while (salp1!=NULL)
  {
     minstart = INT4_MAX;
     presalp=NULL;
     minsalp = preminsalp = NULL;
     salptmp=salp1;
     while (salptmp!=NULL) {
        dsp=(DenseSegPtr) salptmp->segs;
        if (dsp!=NULL) {
           start = dsp->starts;
           if (*start < minstart) {
              minstart = *start;
              minsalp = salptmp;
              preminsalp = presalp;
           }
        }
        presalp=salptmp;
        salptmp=salptmp->next;
     } 
     if (minsalp != NULL) {
        dsp=(DenseSegPtr) minsalp->segs;
        sip=dsp->ids->next;
        bsp=BioseqLockById(sip);
        if (bsp==NULL) {
           minsalp=NULL;
        }
        else BioseqUnlock (bsp); 
     }
     if (minsalp != NULL && newsalp != NULL) {
        dsp=(DenseSegPtr) minsalp->segs;
        sip=dsp->ids->next;
        salptmp=newsalp;
        while (salptmp!=NULL) {
           dsptmp=(DenseSegPtr) salptmp->segs;
           siptmp=dsptmp->ids->next;
           if (SeqIdForSameBioseq(sip, siptmp)) {
              break;
           }
           salptmp=salptmp->next;
        }
        if (salptmp!=NULL)
           minsalp=NULL;
     }
     if (minsalp != NULL) {
        if (preminsalp==NULL) {
           salp1 = salp1->next;
           minsalp->next = NULL;
        }
        else {
           preminsalp->next = minsalp->next;
           minsalp->next = NULL;
        }
        newsalp = SeqAlignLink (newsalp, minsalp);
     }
     else break;
  }
  *salp = newsalp;
  return newsalp;
}

NLM_EXTERN SeqAlignPtr LIBCALL SortSeqAlignFromList (SeqAlignPtr salp, Int2Ptr sortlst)
{
  SeqAlignPtr newsalp;
  CompSegPtr  csp, 
              newcsp;
  SeqIdPtr    sip;
  Int4Ptr     lenp;
  Uint1Ptr    stp, st2p;
  Int2        j, k, m;

  csp = (CompSegPtr) salp->segs;
  newsalp = SeqAlignNew ();
  if ( newsalp == NULL ) return NULL;
  newsalp->type = salp->type;
  newsalp->segtype = salp->segtype;
  newsalp->dim = salp->dim;
  newcsp = (CompSegPtr) MemNew ( sizeof (CompSeg) );
  newsalp->segs = newcsp;
  newcsp->dim = csp->dim;
  newcsp->numseg = csp->numseg;
  newcsp->ids = NULL;
  for (j=0; j < csp->dim; j++)
  {
     for (k=0, sip = csp->ids; k < csp->dim && sip!=NULL; k++) {
        if (sortlst[k] == (j+1)) {
           newcsp->ids = AddSeqId (&(newcsp->ids), sip);
           break;
        }
        sip = sip->next;
     }
  }
  newcsp->from = (Int4Ptr) MemNew ((size_t) ((csp->dim + 2) * sizeof (Int4)));
  for (j=0; j < csp->dim; j++)
  {
     for (k=0, lenp = csp->from; k < csp->dim; k++, lenp++)
        if (sortlst[k] == (j+1)) {
           newcsp->from [j] = *lenp;
           break;
        }
  }
  newcsp->starts =(BoolPtr)MemNew((size_t)((csp->dim*csp->numseg+ 4) * sizeof (Boolean)));
  for (j=0; j < csp->dim; j++)
  {
     for (k=0, stp = csp->starts; k < csp->dim; k++, stp ++)
        if (sortlst[k] == (j+1)) {
           for (m=0, st2p = stp; m < csp->numseg; m++, st2p+=csp->dim) {
              newcsp->starts [m*csp->dim + j] = *st2p;
           }
           break;
        }
  }
  newcsp->lens=(Int4Ptr) MemNew((size_t)((csp->numseg + 2) * sizeof (Int4)));
  for (j = 0; j < csp->numseg + 2; j++) 
     newcsp->lens[j] = csp->lens[j];
  
  return newsalp;
}


NLM_EXTERN SeqAnnotPtr multseqalign_from_pairseqalign (SeqAlignPtr salp)
{
  SeqAlignPtr      salptmp,
                   tmp;
  DenseSegPtr      dsp = NULL;
  SeqPortPtr       spp;
  SeqLocPtr        slp;
  SeqIdPtr         sip, 
                   siphead, siptmp, 
                   sipcur;
  BioseqPtr        bsp;
  CharPtr          bufferin[500];
  CharPtr          bufferout[500];
  CharPtr          buffertmp[2];
  Int4Ptr          starttmp;
  Int4Ptr          lenp;
  Uint1Ptr         strandp;

  Int4             algfrom, algto, alglens;
  Int4             seqfrom, seqto;
  Int4             cur;
  Int4             dsp_len, bsp_len, sum_lens;
  Int4             max_lens = 0;
  Int4             seqoffset;
  Int4             gapoffset;
  Int4             gaptotal;
  Int4             step = SALSALENLIMIT;
  Int2             curnseq;
  Int2             j;
  Int4             k, k1, k2, k3;
  Char             c1, c2;

  ValNodePtr       vnp;
  ValNodePtr       vnpfrom;
  ValNodePtr       vnpstrand;
  SeqAnnotPtr      sap;
 
  Int4             letter;
  Uint1            strandtmp = Seq_strand_unknown;
  Boolean          strand_nonull;
  Boolean          ok;
  CharPtr       str;

  if (salp==NULL)
     return NULL;

  for (salptmp=salp; salptmp!=NULL; salptmp=salptmp->next) {
     if (salptmp->type > 0 && salptmp->segtype==2) {
        dsp = (DenseSegPtr) salptmp->segs;
        break;
     }
  }  
  if (dsp==NULL)
     return NULL;
  siptmp = SeqIdDup (dsp->ids);
  bsp = BioseqLockById (siptmp);
  SeqIdFree (siptmp);
  if (bsp == NULL) {
     return NULL;
  }
  bsp_len = bsp->length;
  sip = SeqIdFindBest(bsp->id, 0);
  BioseqUnlock (bsp);

  /*----- check if not all sequences start with gaps ----*/
  for (salptmp=salp; salptmp!=NULL; salptmp=salptmp->next) {
     if (salptmp->type > 0 && salptmp->segtype==2) {
	dsp = (DenseSegPtr) salptmp->segs;
        for (j=0, starttmp=dsp->starts; j<dsp->dim; j++, starttmp++)
           if ( *starttmp > -1) 
              break;
        ok = (Boolean) (j < dsp->dim);
        if (!ok)
           break;
     }
  }
  if (!ok)
     return NULL;
  /*---- find longest seqalign --------*/
  sum_lens = 0;
  for (salptmp=salp; salptmp!=NULL; salptmp=salptmp->next) {
     if (salptmp->type > 0 && salptmp->segtype==2) {
        dsp_len = SeqAlignStart(salptmp, 0) + SeqAlignLength (salptmp);
        if (dsp_len > sum_lens)
           sum_lens = dsp_len; 
     }
  }  
  if (sum_lens > SALSALENLIMIT) {
     ErrPostEx (SEV_ERROR, 0, 0, "Too long alignment.\n Wait for next Sequin version");
     return NULL;
  }
  step = 2*sum_lens;
  step = MIN ((Int4)step, (Int4)SALSALENLIMIT);
  for (j=0; j<2; j++) 
  {
     str = (CharPtr)MemNew ((size_t) ((step+10) * sizeof(Char)));
     buffertmp[j] = str;
     for (k=0; k<step; k++) buffertmp[j][k] = '-';
     str = (CharPtr)MemNew ((size_t) ((step+10) * sizeof(Char)));
     bufferout[j] = str;
     for (k=0; k<step; k++) bufferout[j][k] = '-';
     str = (CharPtr)MemNew ((size_t) ((step+10) * sizeof(Char)));
     bufferin[j] = str;
     for (k=0; k<step; k++) bufferin[j][k] = '-';
  }  
  siphead = SeqIdDup (sip);
  sipcur = siphead;
  vnpstrand = NULL;
  vnpfrom = NULL;
  ValNodeAddInt (&vnpfrom, 1, (Int4) 0);

  for (salptmp=salp; salptmp!=NULL; salptmp=salptmp->next) 
     if (salptmp->type > 0 && salptmp->segtype==2) 
        break;

  /**************** if 1rst sequence start with gaps in one alignment ***/
  gapoffset = gaptotal = 0;
  for (tmp=salptmp, j=0; tmp!=NULL; tmp=tmp->next, j++) {
     if (tmp->type > 0 && tmp->segtype==2) {
        dsp = (DenseSegPtr) tmp->segs;
        if (*(dsp->starts) < 0) { 
           if (j==0)
              gapoffset = *(dsp->lens);
           if (*(dsp->lens) > gaptotal) {
              gaptotal = *(dsp->lens);
           }
        }
     }
  }
  if (gaptotal > 0) {
     if (gaptotal <= gapoffset)
        gapoffset = gaptotal - gapoffset; 
  }
  algfrom = 0;
  algto = 1;
  for (cur = algfrom; cur < algto; cur += 2*step)
  {
     curnseq = 2;
     dsp = (DenseSegPtr) salptmp->segs;
     seqoffset = *(dsp->starts);
     strand_nonull = (Boolean) (dsp->strands != NULL);
     if (strand_nonull)
        strandp = dsp->strands;
     else
        strandtmp = Seq_strand_unknown;
     for (sip = dsp->ids, j=0; sip!=NULL; sip=sip->next, j++)
     {
        starttmp = dsp->starts;
        if (j==0) {
           seqfrom = 0;
        } else {
           starttmp += j;
           seqfrom = *starttmp;
        }
        lenp = dsp->lens;
        dsp_len = 0;
        sum_lens = 0;
        for(k=0; k < dsp->numseg; k++, lenp++, starttmp += dsp->dim) {
           if ( *starttmp >= 0 ) dsp_len += *lenp;
           sum_lens += *lenp;
        }
        siptmp = SeqIdDup (sip);
        bsp = BioseqLockById (siptmp);
        SeqIdFree (siptmp);
        if (bsp == NULL) {
           return NULL;
        }
        seqto = MIN ((Int4) (seqfrom + step), (Int4) (bsp->length -1));
        if ( j != 0 ) {
           siptmp = SeqIdDup(SeqIdFindBest(bsp->id, 0));
           sipcur->next = siptmp;
           sipcur = siptmp;
           starttmp = dsp->starts;
           starttmp += j;
           lenp = dsp->lens;
           for(k=0; k < dsp->numseg; k++, lenp++, starttmp += dsp->dim) 
              if ( *starttmp >= 0 ) break;
           if (strand_nonull && *strandp==Seq_strand_minus)
              ValNodeAddInt (&vnpfrom, 1, (Int4) (*starttmp+*lenp));
           else 
              ValNodeAddInt (&vnpfrom, 1, (Int4) *starttmp);
        }
        BioseqUnlock (bsp);
        if (strand_nonull)
           strandtmp = *strandp;
        slp = SeqLocIntNew (0, seqto, strandtmp, sip);
        if ( slp == NULL) {
           return NULL;
        }
        spp = SeqPortNewByLoc (slp, Seq_code_iupacna);
        SeqLocFree (slp);
/**
        SeqIdWrite (sip, strLog, PRINTID_FASTA_LONG, 120);
        if (spp!=NULL) WriteLog("mergesalp2 %s\n", strLog);
        else WriteLog("PRINTFspp NULL\n");
        WriteLog("1>  %ld  %ld    %ld %ld  > %ld   \n", dsp_len, sum_lens, seqto, seqoffset, seqfrom);
**/
        if (seqoffset < 0) seqoffset = 0;  /*!!!!!!!!!!!!!!!!!!!!*/

        if ( j == 0 && seqoffset > seqfrom) {
           seqfrom = ReadBufferFromSep (spp, bufferin[j], (Int4)seqfrom,
                                         (Int4)(seqfrom + seqoffset), 0);
        }
/**
WriteLog ("3> %d  %d   %ld  %ld \n", j, seqfrom, sum_lens, seqfrom + sum_lens);
**/
        alglens = readbuff_fromseqalign (spp, salptmp, j, bufferin[j], seqfrom, seqfrom + sum_lens, seqoffset+gapoffset, strandtmp);
        if (alglens == 0) {
           return NULL;
        }
/**
        WriteLog("4>  %d  %ld   %ld %ld   %ld < %ld   %d\n", j, alglens, sum_lens, seqfrom+sum_lens, seqfrom + dsp_len, bsp_len, strandtmp);
**/
        if ( j == 0 && (seqfrom + dsp_len) < bsp_len) {
           alglens = ReadBufferFromSep (spp, bufferin[j], 
                           (Int4)(seqfrom + dsp_len), (Int4)bsp_len, alglens);
           bufferin[j][alglens] = '-';
        }
        else { 
           for (k = alglens; k < step; k++) 
              if (bufferin[j][k] != '-') bufferin[j][k] = '-';
        }
        SeqPortFree(spp);
        ValNodeAddInt (&vnpstrand, 1, (Int4)(strandtmp));
        if (strand_nonull)
           strandp++;
     }
     for (salptmp = salptmp->next; salptmp != NULL; salptmp = salptmp->next)
     {
      if (salptmp->type > 0 && salptmp->segtype==2)
      {
/**
WriteLog ("\n\nCURRENT ALIGN %d\n", curnseq);
**/
       dsp = (DenseSegPtr) salptmp->segs;
       seqoffset = *(dsp->starts);
       if (seqoffset < 0) {
          gapoffset = gaptotal - *(dsp->lens);
       }
       else 
          gapoffset = 0;
       strand_nonull = (Boolean) (dsp->strands != NULL);
       if (strand_nonull)
           strandp = dsp->strands;
       else
           strandtmp = Seq_strand_unknown;
       for (sip = dsp->ids, j=0; sip!=NULL; sip=sip->next, j++)
       {
           dsp = (DenseSegPtr) salptmp->segs;
           for (k=0; k<step; k++) 
              buffertmp[j][k] = '-';
           starttmp = dsp->starts;
           starttmp += j;
           if (j == 0) seqfrom = 0;
           else seqfrom = *starttmp;
           lenp = dsp->lens;
           dsp_len = 0;
           sum_lens = 0;
           for(k=0; k < dsp->numseg; k++, lenp++, starttmp += dsp->dim) {
              if ( *starttmp >= 0 ) 
                 dsp_len += *lenp;
              sum_lens += *lenp;
           }
           siptmp = SeqIdDup (sip);
           bsp = BioseqLockById (siptmp);
           SeqIdFree (siptmp);
           if (bsp == NULL){
              return NULL;
           }
           seqto = MIN ((Int4) (seqfrom + step), (Int4) (bsp->length -1));
           if ( j != 0 ) {
              siptmp = SeqIdDup(SeqIdFindBest(bsp->id, 0));
              sipcur->next = siptmp;
              sipcur = siptmp;
              starttmp = dsp->starts;
              starttmp += j;
              lenp = dsp->lens;
              for(k=0; k < dsp->numseg; k++, lenp++, starttmp += dsp->dim) 
                 if ( *starttmp >= 0 ) break;
              if (strand_nonull && *strandp==Seq_strand_minus)
                 ValNodeAddInt (&vnpfrom, 1, (Int4) (*starttmp+*lenp));
              else 
                 ValNodeAddInt (&vnpfrom, 1, (Int4) *starttmp);
           }
           BioseqUnlock (bsp);
           if (strand_nonull)
              strandtmp = *strandp;
           slp = SeqLocIntNew (0, seqto, strandtmp, sip);
           if ( slp == NULL) {
              return NULL;
           }
           spp = SeqPortNewByLoc (slp, Seq_code_iupacna);
           SeqLocFree (slp);
/**
           SeqIdWrite (sip, strLog, PRINTID_FASTA_LONG, 120);
           if (spp!=NULL) WriteLog ("mergesalp2 %s\n", strLog);
           else WriteLog ("spp NULL\n");
           WriteLog ("1>  %ld  %ld  from %ld  to %ld  offset %ld  \n", dsp_len, sum_lens, seqfrom, seqto, seqoffset);
**/
           if (seqoffset < 0) seqoffset = 0;  /*!!!!!!!!!!!!!!!!!!!!*/
           if ( j == 0 && seqoffset > seqfrom) {
              seqfrom = ReadBufferFromSep (spp, buffertmp[j], (Int4)seqfrom,
                                         (Int4)(seqfrom + seqoffset), 0);
           }
/**
           if ( j == 0)
             WriteLog ("2> %d   %ld  %ld \n", seqfrom, sum_lens, seqfrom + sum_lens);
           else
             WriteLog ("2> %d   %ld  %ld \n", seqfrom, sum_lens, seqfrom + sum_lens);
**/
           alglens = readbuff_fromseqalign (spp, salptmp, j, buffertmp[j], seqfrom, seqfrom + sum_lens, (seqoffset + gapoffset), strandtmp);
           if (alglens == 0) {
              return NULL;
           }
/**
           WriteLog ("3> %d  %c%c%c \n", j, buffertmp[j][0], buffertmp[j][1], buffertmp[j][2]);
           WriteLog ("4>  %ld     %ld %ld  %ld < %ld\n", alglens, sum_lens, sum_lens + seqfrom, seqfrom + dsp_len, bsp_len);
**/
           if ( j == 0 && (seqfrom + dsp_len) < bsp_len) {
              alglens = ReadBufferFromSep (spp, buffertmp[j], (Int4)(seqfrom + dsp_len), (Int4)bsp_len, alglens);
           }
           else { 
              for (k = alglens; k < step; k++) 
                 if (buffertmp[j][k] != '-') buffertmp[j][k] = '-';
           }
           SeqPortFree(spp);
           if (j>0)
              ValNodeAddInt (&vnpstrand, 1, (Int4)strandtmp);
           if (strand_nonull)
              strandp++;
       }
       str = (CharPtr)MemNew ((size_t) ((step+10) * sizeof(Char)));
       bufferout [curnseq] = str;
       for (j=0; j < curnseq + 1; j++) {
           for (k=0; k<step; k++) 
              bufferout [j][k] = '-';
       }  
       k1=k2=k3=letter=0;
       while ( k1 < step && k2 < step && k3 < step)
       {
           c1 = bufferin[0][k1];
           c2 = buffertmp[0][k2];
           if ((c1 != '-' && c2 != '-') || (c1 == '-' && c2 == '-')) {
              if (c1 != '-' && c2 != '-' && c1 != c2) {
/**
                 WriteLog ("ERROR %d [%c]  %d [%c] %c%c%c%c%c  %c%c%c%c%c", k1, c1, k2, c2, c1,bufferin[0][k1+1], bufferin[0][k1+2], bufferin[0][k1+3], bufferin[0][k1+4], c2,buffertmp[0][k2+1], buffertmp[0][k2+2], buffertmp[0][k2+3], buffertmp[0][k2+4]); 
**/
                 break;
              }
              for (j = 0; j < curnseq; ++j) {
                 bufferout[j][k3] = bufferin[j][k1];
              }
              for (j = 1; j < 2; ++j) {
                 bufferout[curnseq + j -1][k3] = buffertmp[j][k2];
              }
              if (bufferout[0][k3] !='-') letter++;
/***
              WriteLog ("%d %d>%c ", (int)k3, (int)letter, c2);
              for (j=0; j<curnseq + 1; j++) WriteLog ("%c", bufferout[j][k3]);
              WriteLog ("\n");
***/
              k1++;
              k2++;
              k3++;
           }
           else if (c1 == '-' && c2 != '-') {
              for (j = 0; j < curnseq; ++j) {
                 bufferout[j][k3] = bufferin[j][k1];
              }
              if (bufferout[0][k3] !='-') letter++;
/***
              WriteLog ("%d %d*%c ", (int)k3, (int)letter, c2);
              for (j=0; j<curnseq + 1; j++) WriteLog ("%c", bufferout[j][k3]);
              WriteLog ("\n");
***/
              k1++;
              k3++;
           }
           else if (c1 != '-' && c2 == '-') {
              for (j = 1; j < 2; ++j) {
                 bufferout[curnseq + j -1][k3] = buffertmp[j][k2];
              }
              if (bufferout[0][k3] !='-') letter++;
/***
              WriteLog ("%d %d<%c ", (int)k3, (int)letter,c2);
              for (j=0; j<curnseq + 1; j++) WriteLog ("%c", bufferout[j][k3]);
              WriteLog ("\n");
***/
              k2++;
              k3++;
           }
       }
       if (k3 > 0) {
           if (k3 > max_lens) 
              max_lens = k3;
           for (j=0; j < curnseq ; j++) {
              for (k=0; k<step; k++) {
                 bufferin[j][k] = bufferout[j][k];
              }
           }
           str = (CharPtr)MemNew ((size_t) ((step+10) * sizeof(Char)));
           bufferin[j] = str;
           for (k=0; k<step; k++) {
              bufferin[j][k] = bufferout[j][k] ;
           }
           curnseq++;
       }
      }
     }
  }
  MemFree (buffertmp[0]);
  MemFree (buffertmp[1]);
  for (j=0; j < curnseq ; j++) 
     MemFree (bufferout[j]);
  
  vnp = NULL;
  for (j=0; j < curnseq; j++) {
     ValNodeAddPointer (&vnp, 0, (Pointer)(bufferin[j])); 
  }
  sap = LocalAlignToSeqAnnotDimn (vnp, siphead, vnpfrom, curnseq, max_lens, vnpstrand, TRUE);
  sipcur = siphead;
  while (sipcur) {
     siptmp = sipcur->next;
     SeqIdFree (sipcur);
     sipcur = siptmp;
  }
  ValNodeFreeData (vnp);
  ValNodeFree (vnpfrom);
  ValNodeFree (vnpstrand);
  return sap;
}

NLM_EXTERN SeqAlignPtr PairSeqAlign2MultiSeqAlign (SeqAlignPtr salp)
{
  SeqAnnotPtr sap;
  SeqAlignPtr salptmp = NULL;

  if (salp!=NULL) 
  {
   if (is_dim2seqalign (salp)) 
   {
     sap = multseqalign_from_pairseqalign (salp);
     if (sap!=NULL && sap->type==2) 
     {
        salptmp = (SeqAlignPtr) sap->data;
        sap->data = NULL;
        SeqAnnotFree (sap);
/******************************** DO NOT FREE Original SeqAlign anymore 
        salp = SeqAlignSetFree (salp);
****************************************/
     }
   }
  }
  return salptmp;   
}


NLM_EXTERN Int2 LIBCALLBACK MultSeqAlignFromPairSeqAlign (Pointer data)

{
  OMProcControlPtr  ompcp;
  SeqAnnotPtr       sap;
  SeqAlignPtr       salp;
  Uint2             entityID;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_SEQALIGN :
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (ompcp->input_data == NULL) return OM_MSG_RET_ERROR;
  salp = (SeqAlignPtr)ompcp->input_data;
  if (salp == NULL) return OM_MSG_RET_ERROR;
  if (is_dim2seqalign (salp))  {
     SortSeqAlign (&salp);
     sap = multseqalign_from_pairseqalign (salp);
  } else {
     sap = SeqAnnotNew ();
     sap->type = 2;
     sap->data = (Pointer) salp;
  }
  if (sap != NULL) {
    entityID = ObjMgrRegister (OBJ_SEQANNOT, (Pointer) sap);
    if (entityID > 0) {
      ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
    }
  }
  return OM_MSG_RET_DONE;
}

NLM_EXTERN SeqAlignPtr LIBCALL multseqalign_to_pairseqalign (SeqAlignPtr salp)
{
  SeqAlignPtr salpnew =NULL,
              salphead=NULL,
              nextsalp;
  DenseSegPtr dsp =NULL,
              newdsp;
  SeqIdPtr    sip;
  Int4Ptr     dspstarts =NULL,
              dsplen;
  Int4        k, j;
  Int2        salp_dim,
              dim;

  if (salp==NULL)
     return NULL;
  if (salp->segtype != 2)
     return salp;
  if (is_dim2seqalign (salp)) 
     return salp;
  salp_dim = salp->dim;
  dsp = (DenseSegPtr) salp->segs;
  salp->segs = NULL;
  for (dim=0; dim<salp_dim-1; dim++)
  {
     newdsp = DenseSegNew ();
     if ( newdsp != NULL ) 
     {
           newdsp->dim = 2;
           newdsp->ids = SeqIdDup (dsp->ids);
           for (sip=dsp->ids, j=0; sip!=NULL && j<dim+1; sip=sip->next, j++)
              continue;
           newdsp->ids->next = SeqIdDup (sip);
           newdsp->numseg = dsp->numseg;
           
           newdsp->lens  =(Int4Ptr)MemNew((size_t)((newdsp->numseg + 2) * sizeof (Int4))); 
           for (j = 0, dsplen = dsp->lens; j < newdsp->numseg; j++, dsplen++ ) 
              newdsp->lens [j] = *dsplen;
           
           newdsp->starts=(Int4Ptr)MemNew((size_t)((2 *newdsp->numseg +4) * sizeof (Int4)));
           for (k = 0; k < newdsp->numseg; k++) {
              for (j=0; j<dsp->dim; j++) {
                 if (j==0) 
                    newdsp->starts[k*newdsp->dim]=dsp->starts[k*dsp->dim];
                 if (j==dim+1)
                    newdsp->starts[k*newdsp->dim+1]=dsp->starts[k*dsp->dim+j];
              }
           }

           if (dsp->strands != NULL) {
              newdsp->strands =(Uint1Ptr)MemNew((size_t)((2 *newdsp->numseg+4)*sizeof (Uint1)));
              for (k = 0; k < newdsp->numseg; k++) {
                 for (j=0; j<dsp->dim; j++) {
                    if (j==0) 
                       newdsp->starts[k*newdsp->dim]=dsp->starts[k*dsp->dim];
                    if (j==dim+1)
                       newdsp->starts[k*newdsp->dim+1]=dsp->starts[k*dsp->dim+j];
                 }
              }
           }
        if (salphead==NULL) {
           salp->type = 3;
           salp->segtype = 2;
           salp->dim = 2;
           salp->score = NULL;
           salp->segs = (Pointer) newdsp;
           salphead= salp;
           nextsalp = salp;
        }
        else {
           salpnew = SeqAlignNew();
           if (salpnew) {
              salpnew->type = 3;
              salpnew->segtype = 2;
              salpnew->dim = 2;
              salpnew->score = NULL;
              salpnew->segs = (Pointer) newdsp;
              salpnew = Compact (salpnew);
              nextsalp->next = salpnew;
              nextsalp = salpnew;
           }
        }
     }
  }
  return salphead;
}

