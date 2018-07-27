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
 * $Id: salpacc.c,v 6.32 2004/12/29 13:49:01 bollin Exp $
 Collection of SeqAlign Accession utilities.
 Maintainer: Hugues Sicotte
 Authors of the original routines: Hugues Sicotte, Colombe Chappey, Tom Madden, Jinghui Zhang

*/

/** for ErrPostEx() ****/

static char *this_module = "salpacc";
#define THIS_MODULE this_module
static char *this_file = __FILE__;
#define THIS_FILE this_file



#include <salpacc.h>
#include <objsset.h>
#include <sequtil.h>
#include <txalign.h>
#include <salsap.h> /* for CompSegPtr definition */
/* 
   Function to return a pointers to a linked list of SeqAligns
   that are in the SeqAligns. The user may NOT free these
   SeqIds, as they are in the SeqAlign data-structure
*/

NLM_EXTERN SeqIdPtr LIBCALL SeqIdPtrFromSeqAlign(SeqAlignPtr salp) {

    SeqIdPtr sip=NULL;
    if(salp==NULL) 
        return NULL;
    if(salp->segtype==1) { /* DenseDiag */
        DenseDiagPtr ddp;
        ddp=(DenseDiagPtr)salp->segs;
        if(ddp!=NULL) 
            sip=ddp->id;
    } else if (salp->segtype==2) { /* DenseSeg */
        DenseSegPtr dsp;
        dsp = (DenseSegPtr) salp->segs;
        if(dsp!=NULL)
            sip=dsp->ids;
    } else if (salp->segtype==3) { /* StdSeg */
        StdSegPtr ssp;
        ssp = (StdSegPtr)salp->segs;
        if(ssp!=NULL) {
            Int4 numids=0;
            sip=ssp->ids;
            while(sip) {/* SeqIds are optional at this level */
                numids++;
                sip=sip->next;
            }
            if(numids==ssp->dim) { /* salp->dim is optional, while ssp->dim is not */
                sip = ssp->ids;
            } else {/* Look in the SeqLocs */
                SeqLocPtr slp;
                SeqIdPtr sip_tmp,sip_last=NULL;
                if (ssp->ids!=NULL) {
                    ssp->ids = SeqIdSetFree(ssp->ids);
                    numids=0;
                }
                slp = ssp->loc;
                while(slp!=NULL) {
                    sip_tmp = SeqIdDup(SeqLocId(slp));
                    if(ssp->ids==NULL) /* "Cache" SeqIds in ids slot */
                        ssp->ids = sip_tmp;
                    else if(sip_last!=NULL)
                        sip_last->next=sip_tmp;
                    if(sip_tmp!=NULL)
                        sip_last=sip_tmp;
                    else {
                        ErrPostEx(SEV_WARNING,0,0,"SeqLoc in StdSeg has NULL SeqLocId()\n");
                    }
                    numids++;
                    slp=slp->next;
                }
                sip = ssp->ids;
                if(numids!=ssp->dim) {
                    ErrPostEx(SEV_WARNING,0,0,"Number of SeqLocs differs from SeqAlign ssp->dim\n");
                    ssp->dim = numids;
                }
            }
        }
    } else if(salp->segtype==4) { /* Packed Seg. Optimal for editing alignments */
        PackSegPtr psp;
        psp = (PackSegPtr) salp->segs;
        if (psp!=NULL)
            sip = psp->ids;
    } else if (salp->segtype ==5) {/* Assume that Linked list of SeqAlign set
                                      always refers to the same set of sequences */
        SeqAlignPtr salp_tmp;
        salp_tmp = (SeqAlignPtr) salp->segs;
        sip = SeqIdPtrFromSeqAlign(salp_tmp);
    } else if (salp->segtype == 6) { /* Temporarely Supported SeqAlign Type */
        CompSegPtr csp;
        csp = (CompSegPtr) salp->segs;
        sip = csp->ids;
    } else {
        ErrPostEx(SEV_WARNING,0,0,"Undefined Seqalign segtype=%d\n",(int)salp->segtype);
    }
    return sip;
}
NLM_EXTERN SeqIdPtr LIBCALL SeqAlignId (SeqAlignPtr salp, Int2 index)
{
  SeqIdPtr    sip = NULL;
  Int2        j;

  if (salp!=NULL) {
     sip = SeqIdPtrFromSeqAlign(salp);
     if (sip != NULL)
     {
        for (j=0; j<index && sip != NULL; j++)
           sip = sip->next;
     }
  }
  return sip;
}

NLM_EXTERN Boolean LIBCALL SeqAlignFindSeqId (SeqAlignPtr salphead, SeqIdPtr sip)
{
  SeqAlignPtr salp;
  SeqIdPtr    tmpsip;

  for (salp = salphead; salp!= NULL; salp=salp->next)
  {
     tmpsip = SeqIdPtrFromSeqAlign (salp);
     if ((SeqIdOrderInBioseqIdList(sip, tmpsip)) > 0)
     {
        return TRUE;
     }
  }
  return FALSE;
}

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignIdReplace (SeqAlignPtr salp, Int2 index, SeqIdPtr newsip)
{
  DenseSegPtr  dsp;
  DenseDiagPtr ddp;
  SeqIdPtr     sip = NULL,
               presip;
  Int2         j;

  if (newsip==NULL)
     return salp;
  newsip = SeqIdDup(newsip);
  if (salp!=NULL) {
     if (salp->segtype == 2) {
        dsp = (DenseSegPtr) salp->segs;
        if (dsp!=NULL) {
           sip = dsp->ids;
           if (index==0)
              dsp->ids = newsip;
           else
           {
              presip = dsp->ids;
              sip=sip->next;
              for (j=0; j<index-1; j++, sip=sip->next)
                 presip = presip->next;
              presip->next = newsip;
           }
           newsip->next = sip->next;
           sip->next = NULL;
           SeqIdFree (sip);
        }
     } 
     else if (salp->segtype == 1) {
        ddp = (DenseDiagPtr) (salp->segs);
        if(ddp!=NULL) {
           sip = ddp->id;
           if (index==0)
              ddp->id = newsip;
           else
           {
              presip = ddp->id;
              sip=sip->next;
              for (j=0; j<index-1; j++, sip=sip->next)
                 presip = presip->next;
              presip->next = newsip;
           }
           newsip->next = sip->next;
           sip->next = NULL;
           SeqIdFree (sip);
        }
     }
  }
  return salp;
}



/*
  Returns the length (aligned bases+gaps(if type 2)) of a SeqAlign
 */

NLM_EXTERN Int4 LIBCALL SeqAlignLength (SeqAlignPtr salp)
{
  DenseSegPtr  dsp;
  DenseDiagPtr ddp;
  CompSegPtr   csp;
  Int4Ptr      lenp;
  Int4         lens = 0;
  Int2         j;

  if (salp!=NULL) {
     if (salp->segtype == 2) {
        dsp = (DenseSegPtr) salp->segs;
        if (dsp) { 
           lenp = (Int4Ptr) dsp->lens;
           for (j=0; j<dsp->numseg; j++, lenp++)
              lens += *lenp;
        }
     } 
     else if (salp->segtype == 1) {
       ddp = (DenseDiagPtr)salp->segs;
       if(ddp!=NULL) 
          lens=(Int4)ddp->len;
     }
     else if (salp->segtype == COMPSEG) {
        csp = (CompSegPtr)salp->segs;      
        if (csp) {
          lenp = (Int4Ptr) csp->lens;
          for (j=0; j<csp->numseg; j++, lenp++)
             lens += *lenp;
        }
     }
  }
  return lens;
}



NLM_EXTERN Int4 LIBCALL SeqAlignLengthForId (SeqAlignPtr salp, SeqIdPtr sip)
{
  DenseSegPtr  dsp;
  Int4Ptr      lenp, startp;
  Int4         j,
               length=0;
  Int2         index;

  if (salp==NULL)
     return 0;
  index = SeqIdOrderInBioseqIdList(sip,SeqIdPtrFromSeqAlign(salp));
  if (index==0)
     return 0;
  if (salp->segtype == 2) {
     dsp = (DenseSegPtr) salp->segs;
     if (dsp) {
        lenp = (Int4Ptr) dsp->lens;
        startp= (Int4Ptr) dsp->starts;
        startp = startp + (index-1);
        for (j=0; j<dsp->numseg; j++, lenp++, startp += dsp->dim)
        {
           if (*startp > -1)
              length += *lenp;
        }
     }
  }
  return length; 
}


/*
  returns the Strand of the "indexed" (0,1,..) sequence.
 */
NLM_EXTERN Uint1 LIBCALL SeqAlignStrand (SeqAlignPtr salp, Int2 index)
{
  Uint1        strand = Seq_strand_unknown;

  if (salp == NULL)
     return strand;
  if (salp->segtype == SAS_DENDIAG) {
      DenseDiagPtr ddp;
      ddp = (DenseDiagPtr) salp->segs;
      while(ddp && strand==Seq_strand_unknown) {
          Uint1 this_strand=Seq_strand_unknown;
          if(index<ddp->dim) {
              if (ddp->strands)
                  this_strand = ddp->strands[index];
          }
          if(strand==Seq_strand_unknown)
              strand = this_strand;
          else if (this_strand !=Seq_strand_unknown) {
              if(strand!=this_strand)
                  strand = Seq_strand_both;
          }
          if(salp->type == SAT_DIAGS || strand == Seq_strand_unknown)
              ddp = ddp->next;
          else
              ddp=NULL;
      }
  } else if (salp->segtype == SAS_DENSEG) {
      DenseSegPtr dsp;
      dsp = (DenseSegPtr) salp->segs;
      if (dsp) {
        if (dsp->strands && index<dsp->dim) {
            Int2 j = index;
            strand = (dsp->strands)[j];
            while (strand==Seq_strand_unknown) {
                j+=dsp->dim;
                if (j >= (Int4)(dsp->dim*dsp->numseg))
                    break;
                strand = (dsp->strands)[j];
            }
        }
     }
  } else if (salp->segtype == SAS_STD) {
      StdSegPtr ssp;
      SeqLocPtr slp;      
      ssp = (StdSegPtr) salp->segs;
      while (ssp!=NULL) {
          Uint1Ptr this_strand=Seq_strand_unknown;
          Int2 this_index = index;
          slp = ssp->loc;
          while(this_index && slp!=NULL) {
              slp=slp->next;
              this_index--;
          }
          if(slp) {
              Uint1 this_strand=Seq_strand_unknown;
              this_strand = SeqLocStrand(slp);
              if(strand==Seq_strand_unknown)
                  strand = this_strand;
              else if (this_strand !=Seq_strand_unknown) {
                  if(strand!=this_strand)
                      strand = Seq_strand_both;
              }
          }
          if(salp->type == SAT_DIAGS)
              ssp = ssp->next;
          else
              ssp=NULL;
      }
  }
  else if (salp->segtype == SAS_DISC)
  {
    SeqAlignPtr sap;
    Int4        offset;
    
    if (index == 0 || index == 1)
    {
      sap = (SeqAlignPtr) salp->segs;
      strand = SeqAlignStrand (sap, index);
    }
    else
    {
      for (sap = (SeqAlignPtr) salp->segs, offset = 1;
           sap != NULL && offset != index;
           sap = sap->next, offset++)
      { 
      }
      if (sap != NULL)
      {
        strand = SeqAlignStrand (sap, 1);
      }
    }
  }
  return strand;
}

NLM_EXTERN Int4 LIBCALL SeqAlignStart (SeqAlignPtr salp, Int2 index)
{
    DenseDiagPtr  ddp;
    DenseSegPtr   dsp;
    CompSegPtr    csp;
    Int4          j, val = -1, c_val;
    Uint1         strand;
    SeqAlignPtr   sap;
    Int4Ptr       startp;
    StdSegPtr     ssp;
    SeqLocPtr     loc;
    Int2          i;

    if (salp == NULL)
        return -1;

    switch(salp->segtype) {

    case 1:
        ddp = (DenseDiagPtr) salp->segs;
        if (ddp != NULL) {
            startp = ddp->starts;
            if (index > 0)
                startp += index;
            val = *startp;
        }
        break;
    case 2:
        
        dsp = (DenseSegPtr) salp->segs;
        if (dsp!=NULL) {
            strand = SeqAlignStrand (salp, index);
            if (strand==Seq_strand_minus) {
                startp = dsp->starts;
                if (index > 0)
                    startp += index;
                for (j = 0; j<dsp->numseg; j++, startp+=dsp->dim)
                    if (*startp > -1)
                        break;
                if (j < dsp->numseg) {
                    val = *startp + dsp->lens[j] - 1;
                }
            } 
            else {
                startp = dsp->starts;
                if (index > 0)
                    startp += index;
                for (j = 0; j < dsp->numseg; j++, startp += dsp->dim) 
                    if (*startp > -1)
                        break;
                if (j < dsp->numseg) {
                    val = *startp;              
                }
            }
        }
        break;
    case 3:
        ssp = (StdSegPtr) salp->segs;
        val=-1;
        while(val==-1 && ssp!=NULL) {
            loc = ssp->loc;
            for(i=0;i!=index && loc!=NULL;i++,loc=loc->next)
               continue;
            if(i==index && loc!=NULL) {
               if (SeqLocStrand(loc) == Seq_strand_minus)
                  val = SeqLocStop(loc);
               else
                  val = SeqLocStart(loc);
            }
            ssp = ssp->next;
        }
        break;
    case 5:
        sap = (SeqAlignPtr) salp->segs;
        if(SeqAlignStrand(sap,0)==Seq_strand_minus) 
        {
           val = -1;
           for(;sap != NULL; sap = sap->next) {
              if((c_val = SeqAlignStart (sap, index)) != -1)
                 val = MAX(val, c_val);
           }
        }
        else {
           val = INT4_MAX;
           for(;sap != NULL; sap = sap->next) {
              if((c_val = SeqAlignStart (sap, index)) != -1)
                 val = MIN(val, c_val);
           }
           if (val == INT4_MAX)
              val = -1;
        }
        break;
    case COMPSEG:
        csp = (CompSegPtr) salp->segs;
        val = csp->from[index];
        break;
    default:
        val = -1;
        break;
    }

    return val;
}

NLM_EXTERN Int4 LIBCALL SeqAlignStop (SeqAlignPtr salp, Int2 index)
{
  DenseDiagPtr  ddp;
  DenseSegPtr   dsp;
  CompSegPtr    csp;
  Int4Ptr       startp;
  BoolPtr       startb;
  Int4          j, 
                val = (Int4)-1;
  Uint1         strand;
  
  if (salp == NULL)
     return -1;
  if (salp->segtype == 1) 
  {
     ddp = (DenseDiagPtr) salp->segs;
     if (ddp != NULL) {
        startp = ddp->starts;
        if (index > 0)
           startp += index;
        val = *startp + ddp->len - 1;
     }
  } 
  else if (salp->segtype == 2) 
  {
     dsp = (DenseSegPtr) salp->segs;
     if (dsp!=NULL) 
     {
        strand = SeqAlignStrand(salp, index);
        if (strand == Seq_strand_minus) {
            startp = dsp->starts + ((dsp->dim * dsp->numseg) - dsp->dim);
            if (index > 0)
               startp += index;
            for (j = dsp->numseg-1; j >= 0; j--, startp-=dsp->dim)
               if (*startp > -1)
                  break;
            if (j >= 0) {
               val = *startp;
            }
        }
        else {
           startp = dsp->starts + ((dsp->dim * dsp->numseg) - dsp->dim);
           if (index > 0)
              startp += index;
           for (j = dsp->numseg-1; j >= 0; j--, startp-=dsp->dim) 
              if (*startp > -1)
                 break;
           if (j >= 0) {
              val = *startp + dsp->lens[j] - 1;              
           }
        }
     }
  } 
  else if (salp->segtype == 3) 
  {
      StdSegPtr ssp;
      SeqLocPtr loc;
      Int2 i;
      Int4 this_stop;

      val =-1;
      ssp = (StdSegPtr) salp->segs;
      /* Find last non-gap segment */
      while(ssp!=NULL) 
      {
          loc = ssp->loc;
          for(i=0;i!=index && loc!=NULL;i++,loc=loc->next)
             continue;
          if(i==index && loc!=NULL) {
             if (SeqLocStrand(loc) == Seq_strand_minus)
                this_stop = SeqLocStart(loc);
             else
                this_stop = SeqLocStop(loc);
          }
          if(this_stop!=-1)
              val=this_stop;
          ssp=ssp->next;
      }
  }  
  else if (salp->segtype == COMPSEG)
  {
     csp = (CompSegPtr) salp->segs;
     val = csp->from[index];
     startb=csp->starts;
     for (j=0; j<csp->numseg; j++)
     {
        if (*startb)
           val+=csp->lens[j]; 
        startb+=csp->dim;
     } 
  }
  return val;
}


NLM_EXTERN Uint1 LIBCALL SeqAlignMolType (SeqAlignPtr salp)
{
  DenseSegPtr  dsp;
  DenseDiagPtr ddp;
  StdSegPtr    ssp;
  BioseqPtr    bsp;
  SeqIdPtr     sip = NULL;
  Int2         k;
  Int2         dim;
  Uint1        moltype = 0;
  Boolean      molb;
  SeqAlignPtr  tmpsalp;
 
  if (salp==NULL)
     return FALSE;
  if (salp->segtype == SAS_DENDIAG) {
     ddp = (DenseDiagPtr) salp->segs;
     sip = ddp->id;
     dim = ddp->dim;
  }
  else if (salp->segtype == SAS_DENSEG) {
     dsp = (DenseSegPtr) salp->segs;
     sip = dsp->ids;
     dim = dsp->dim;
  }
  else if (salp->segtype == SAS_STD) {
     ssp = (StdSegPtr) salp->segs;
     sip = ssp->ids;
     dim = ssp->dim; 
  }
  else if (salp->segtype == SAS_DISC) 
  {
  	 tmpsalp = (SeqAlignPtr) salp->segs;
  	 return SeqAlignMolType (tmpsalp);
  }
  if (sip!=NULL) {
   for (k = 0; k < dim && sip!=NULL; k++, sip = sip->next)
   {
     bsp = BioseqLockById (sip);
     if (bsp!=NULL)
     {   
        break;
     }
   }
   if (bsp!=NULL) {
      moltype = bsp->mol;
      molb = (Boolean) (ISA_aa(moltype));
      BioseqUnlock (bsp);
      for (; k < dim && sip!=NULL; k++, sip = sip->next)
      {
         bsp = BioseqLockById (sip);
         if (bsp!=NULL)
         {   
            if ((Boolean) (ISA_aa(bsp->mol)) != molb) {
               BioseqUnlock (bsp);
               moltype = 0;
               ErrPostEx (SEV_ERROR, 0, 0, "All Source Bioseq's are not Nucleotide");
               break;
            }
            BioseqUnlock (bsp);
         }
      }
   }
  }
  return moltype;
}

NLM_EXTERN SeqAnnotPtr LIBCALL SeqAnnotForSeqAlign (SeqAlignPtr salp)
{
  SeqAnnotPtr sap;

  if (salp==NULL)
     return NULL;
  sap = SeqAnnotNew ();
  sap->type = 2;
  sap->data = (Pointer) salp;
  return sap;
}



/**********************************************************************
*
*       SeqAlignLink(head, new)
*       link the new align to the end of head align. return the
*       start of the linked chain
*
**********************************************************************/

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignLink(SeqAlignPtr head, SeqAlignPtr a_new)
{
        SeqAlignPtr curr;

        curr = head;
        if(curr!= NULL){
                while(curr->next != NULL)
                        curr= curr->next;
                curr->next = a_new;
                return head;
        }
        else
                return a_new;

}

/***********************************************************************
***    
***    SeqLocListFromSeqAlign
***      read SeqAlignPtr
***      return list of ValNodePtr-SeqLocPtr
***
************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocListFromSeqAlign (SeqAlignPtr salp)
{
  SeqAlignPtr   salptmp;
  ValNodePtr    vnp = NULL;

  SeqLocPtr     slp;
  SeqIdPtr      sip;
  Int4          start, 
                stop;
  Uint1         strand; 
  Int2          offset;
  
  if (salp==NULL) 
     return NULL;
  for (salptmp=salp; salptmp!=NULL; salptmp=salptmp->next)
  {
     sip = SeqAlignId (salptmp, 0);
     if (sip != NULL)
     {
        offset = 0;
        for (; sip != NULL; sip = sip->next)
        {
           start = SeqAlignStart (salptmp, offset);
           stop = SeqAlignStop (salptmp, offset);
           strand = SeqAlignStrand (salptmp, offset);
           slp = SeqLocIntNew (start, stop, strand, sip);
           if (slp!=NULL) {
              ValNodeAddPointer (&vnp, 0, (Pointer) slp);
           }
           offset++;
        }
     }
  }
  return (SeqLocPtr) vnp;
}

/*
  Returns A chain of "dim" SeqLocs representing the area covered by a
  single SeqAlign
 */

NLM_EXTERN SeqLocPtr LIBCALL SeqLocFromSeqAlign (SeqAlignPtr salp, SeqIdPtr sip)
{
  SeqLocPtr     slp = NULL;
  SeqIdPtr      sip1 = NULL, 
                siptmp;
  Int4          start, 
                stop;
  Uint1         strand; 
  Int2          offset;
 
  if (salp != NULL)
  {
     offset = 0;
     sip1=SeqAlignId(salp, 0);
     if (sip  == NULL)
        siptmp = sip1;
     else 
     {
        sip = SeqIdFindBest (sip, 0);
        for (siptmp=sip1; siptmp!=NULL; siptmp=siptmp->next, offset++) {
           if (SeqIdForSameBioseq (sip,siptmp))
              break;
        }
     }
     if (siptmp!=NULL)
     {
        start = SeqAlignStart (salp, offset);
        stop = SeqAlignStop (salp, offset);
        strand = SeqAlignStrand (salp, offset);
        slp = SeqLocIntNew (start, stop, strand, siptmp);
     }
     
  }
  return slp; 
}

NLM_EXTERN void LIBCALL SeqAlignScoreRead(SeqAlignPtr seqalign,Int4Ptr Score,FloatHi PNTR Bit_Score,FloatHi PNTR Evalue) {
    ScorePtr sp=NULL;
    if(Score)
        *Score=0;
    if(Bit_Score)
        *Bit_Score=0.0;
    if(Evalue)
        *Evalue=(FloatHi)INT_MAX;
    if(seqalign!=NULL) {
        for(sp=seqalign->score;sp!=NULL;sp=sp->next) {
            if(sp->id!=NULL) {
                if (StringCmp(sp->id->str,"e_value")==0) {
                    if(Evalue)
                        *Evalue=sp->value.realvalue;
                } else if (StringCmp(sp->id->str,"sum_e")==0) {
                    if(Evalue)
                        *Evalue=sp->value.realvalue;
                } else if (StringCmp(sp->id->str,"score")==0) {
                    if(Score)
                        *Score=sp->value.intvalue;
                } else if (StringCmp(sp->id->str,"bit_score")==0) {
                    if(Bit_Score)
                        *Bit_Score=sp->value.realvalue;
                }
            }
        }
    }
}

NLM_EXTERN ScorePtr LIBCALL SeqAlignScorePtrGet(SeqAlignPtr salp) {
  if(salp!=NULL) {
    if(salp->score!=NULL) {
        return salp->score;
    } else if (salp->segtype == 1) {
        DenseDiagPtr ddp;
        ddp = (DenseDiagPtr) salp->segs;
        return ddp->scores;
    } else if (salp->segtype == 2) {
        DenseSegPtr dsp;
        dsp = (DenseSegPtr) salp->segs;
        return dsp->scores;
    } else if (salp->segtype == 3) {
        /* e.g. for Translating searched */
        StdSegPtr ssp;
        ssp = (StdSegPtr) salp->segs;
        return ssp->scores;
    } else if (salp->segtype == 4) {
        PackSegPtr psp;
        psp = (PackSegPtr) salp->segs;
        return psp->scores;
    }
  }
  return NULL;
}


NLM_EXTERN FloatHi LIBCALL ScoreRead(ScorePtr score) {
  if(score!=NULL) {
    if(score->choice ==1) {
      return (FloatHi) score->value.intvalue;
    } else if(score->choice ==2) {
      return (FloatHi) score->value.realvalue;
    } else {
      return (FloatHi) 0.0;
    }
  }
 return (FloatHi)0.0;
}


/* Quick SeqAlign Output function: Can be used while Debugging 
   to write to a file.
 */
NLM_EXTERN void LIBCALL SeqAlignWrite(SeqAlignPtr salp,CharPtr file) {
    SeqAnnotPtr sap;
    AsnIoPtr aip;
    if(salp) {
        aip=AsnIoOpen(file,"w");
        sap = SeqAnnotForSeqAlign(salp);
        if(sap) {
            if (aip!=NULL) { 
                SeqAnnotAsnWrite((SeqAnnotPtr) sap, aip, NULL);
                AsnIoReset(aip); 
            } 
            sap->data=NULL;
            SeqAnnotFree(sap);
        } else {
            ErrPostEx(SEV_WARNING,0,0,"NULL SeqAnnot from Non-NULL SeqAlign\n");
        }
        AsnIoClose(aip);
    }
}

/*
  Subroutine to return the order of a Bioseq in a SeqId Chain 
  
  returns -1 as order if it fails.

   */
NLM_EXTERN void LIBCALL BioseqOrderInSeqIdList(SeqIdPtr sip,BioseqPtr bsp,Int4Ptr order) {

    if(order)
        *order=-1;
    if(!order || !sip || !bsp)
        return;
    {
        Int4 i;
        for(i =0 ; sip != NULL; i++,sip = sip->next) {
            if(*order ==-1 && BioseqMatch(bsp, sip)) {
                *order = i;
                break;
            }
        }
    }
    return;
}

/*
  Subroutine to return the order (position) of two
  sequences in a possibly multiple alignment.
 */

NLM_EXTERN void LIBCALL SeqAlignBioseqsOrder(SeqAlignPtr align,BioseqPtr query_bsp,BioseqPtr subject_bsp,Int4Ptr a_order_ptr, Int4Ptr b_order_ptr) {
    Int4 a_order=-1,b_order=-1;
    if(align) {
        SeqIdPtr sip;
        sip =SeqIdPtrFromSeqAlign(align);
        if(subject_bsp) 
            BioseqOrderInSeqIdList(sip,subject_bsp,&b_order);
        if(b_order==-1 && align->dim==2) {
            b_order=1;
        }
        if(query_bsp) {
            BioseqOrderInSeqIdList(sip,query_bsp,&a_order);
            if(a_order==1 && b_order==1) {
                if(align->dim==2)
                    b_order = 0;
                else
                    b_order = -1;
            }
        }
        if(a_order==-1) {
            if (b_order>0) {
                a_order = 0;
            } else if(b_order==0 && align->dim==2) {
                a_order = 1;
            }
        }
    }
    if(a_order_ptr)
        *a_order_ptr = a_order;
    if(b_order_ptr)
        *b_order_ptr = b_order;
    return;
}

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignListFree (SeqAlignPtr salp)
{
  SeqAlignPtr next;
  while (salp!=NULL) {
     next = salp->next;
     salp->next = NULL;
     SeqAlignFree (salp);
     salp = next;
  }
  return NULL;
}


NLM_EXTERN Boolean LIBCALL SeqAlignStartStop(Int4Ptr a_start, Int4Ptr a_stop, Int4Ptr b_start, 
			   Int4Ptr b_stop, 
			   SeqAlignPtr align, BioseqPtr query_bsp, BioseqPtr subject_bsp)
{
  Int4 a_order=-1, b_order=-1, i;

  if(align==NULL)
      return FALSE;
  SeqAlignBioseqsOrder(align,query_bsp,subject_bsp,&a_order, &b_order);
  if(a_order==-1 || b_order==-1)
      return FALSE;
  if(a_start)
      *a_start=-1;
  if(b_start)
      *b_start=-1;
  if(a_stop)
      *a_stop=-1;
  if(b_stop)
      *b_stop=-1;

  if(align->segtype==1) {
      DenseDiagPtr ddp;
      Int4 lcl_a_start=-1,lcl_a_stop=-1,lcl_b_start=-1,lcl_b_stop=-1;
      ddp=(DenseDiagPtr) align->segs;
      while(ddp!=NULL) {
          Int4 tmp;
	  if((tmp=ddp->starts[a_order]) != -1) {
	      if(a_start && (lcl_a_start == -1 || lcl_a_start > tmp))
		  lcl_a_start = tmp;
	      if(a_stop &&(lcl_a_stop == -1 || lcl_a_stop < (tmp + ddp->len -1)))
		  lcl_a_stop= tmp + ddp->len -1;
	  }
	  if((tmp=ddp->starts[b_order]) != -1) {
	      if(b_start && (lcl_b_start == -1 || lcl_b_start > tmp))
		  lcl_b_start = tmp;
	      if(b_stop && (lcl_b_stop == -1 || lcl_b_stop < (tmp + ddp->len -1)))
		  lcl_b_stop= tmp + ddp->len -1;
	  }
	  ddp=ddp->next;
      }
      if(a_start)
          *a_start=lcl_a_start;
      if(b_start)
          *b_start=lcl_b_start;
      if(a_stop)
          *a_stop=lcl_a_stop;
      if(b_stop)
          *b_stop=lcl_b_stop;
  } else if (align->segtype==2) {
      DenseSegPtr dsp;
      Int4 lcl_a_start=-1,lcl_a_stop=-1,lcl_b_start=-1,lcl_b_stop=-1;
      dsp = (DenseSegPtr) align->segs;
      for(i = 0; i<dsp->numseg; ++i) {
          Int4 ipos,jpos;
          ipos=2*i+a_order;
          jpos=2*i+b_order;
          if(dsp->starts[ipos] != -1) {
              if(a_start) {
                  Int4 tmp = dsp->starts[ipos];
                  if(lcl_a_start == -1 || lcl_a_start > tmp)
                      lcl_a_start = tmp;
              }
              if(a_stop) {
                  Int4 tmp = dsp->starts[ipos]+dsp->lens[i]-1;
                  if(lcl_a_stop == -1 || lcl_a_stop < tmp) 
                      lcl_a_stop = tmp;
              }
          }
          if(dsp->starts[jpos] != -1) {
              if(b_start) {
                  Int4 tmp = dsp->starts[jpos];
                  if(lcl_b_start == -1 || lcl_b_start > tmp)
                      lcl_b_start = tmp;
              }
              if(b_stop) {
                  Int4 tmp = dsp->starts[jpos]+dsp->lens[i]-1;
                  if(lcl_b_stop == -1 || lcl_b_stop < tmp) 
                      lcl_b_stop = tmp;
              }
          }
      }
      if(a_start)
          *a_start=lcl_a_start;
      if(b_start)
          *b_start=lcl_b_start;
      if(a_stop)
          *a_stop=lcl_a_stop;
      if(b_stop)
          *b_stop=lcl_b_stop;
  } else if (align->segtype==3) {
        /* e.g. for Translating searched */

      ErrPostEx(SEV_WARNING,0,0,"SeqAlignStartStop : Not yet SeqAlign Type=%d\n",align->segtype);
  } else if (align->segtype==4) {
      /* PackSegPtr psp;
      psp = (PackSegPtr) align->segs; */
      ErrPostEx(SEV_WARNING,0,0,"SeqAlignStartStop : Not yet SeqAlign Type=%d\n",align->segtype);
  } else if (align->segtype==5) {
      Int4 lcl_a_start=-1,lcl_b_start=-1,lcl_a_stop=-1,lcl_b_stop=-1;
      SeqAlignPtr salp;
      /* SeqAlignSet */
      salp = (SeqAlignPtr) align->segs;
      while(salp!=NULL) {
          if(SeqAlignStartStop(&lcl_a_start, &lcl_a_stop, &lcl_b_start, &lcl_b_stop,
                               salp, query_bsp, subject_bsp)) {
              if(a_start && (*a_start ==-1 || *a_start>lcl_a_start))
                  *a_start=lcl_a_start;
              if(b_start && (*b_start ==-1 || *b_start>lcl_b_start))
                  *b_start=lcl_b_start;
              if(a_stop && (*a_stop ==-1 || *a_stop<lcl_a_stop))
                  *a_stop=lcl_a_stop;
              if(b_stop && (*b_stop ==-1 || *b_stop<lcl_b_stop))
                  *b_stop=lcl_b_stop;
          }
          salp=salp->next;
      }
  } else {
      ErrPostEx(SEV_WARNING,0,0,"SeqAlignStartStop : Unsupported SeqAlign Type=%d\n",align->segtype);
  }
  return ((!a_start || *a_start != -1) && (!a_stop || *a_stop != -1)  && (!b_start || *b_start != -1) && (!b_stop || *b_stop != -1));
}

/*
  Find the Absolute Starts and Stops of a Linked List of SeqAlign.
 */
NLM_EXTERN void LIBCALL SeqAlignListStartStop(Int4Ptr qstart,Int4Ptr qstop,Int4Ptr sstart,Int4Ptr sstop,SeqAlignPtr salp, BioseqPtr query,BioseqPtr subject) {
    Int4 qs,qe,ss,se;

    if(qstart)
        *qstart = -1;
    if(sstart)
        *sstart = -1;
    if(qstop)
        *qstop=-1;
    if(sstop)
        *sstop=-1;
    while(salp!=NULL) {
        if(SeqAlignStartStop(&qs,&qe,&ss,&se,salp,query,subject)) {
            if(qstart && ((qs!=-1 && qs<*qstart) || *qstart==-1))
                *qstart=qs;
            if(sstart && ((ss!=-1 && ss<*sstart) || *sstart==-1))
                *sstart=ss;
            if(sstop && se!=-1 && se>*sstop)
                *sstop=se;
            if(qstop && qe!=-1 && qe>*qstop)
                *qstop=qe;
        }
        salp=salp->next;
    }
}

/* copied from gather */
static Uint1 align_strand_get(Uint1Ptr strands, Int2 order)
{
        if(strands == NULL)
                return 0;
        else
                return strands[order];
}

static void load_start_stop(Int4Ptr start, Int4Ptr stop, Int4 c_start, Int4 c_stop)
{
	if(*start == -1)
	{
		*start = c_start;
		*stop = c_stop;
	}
	else
	{
		*start = MIN(*start, c_start);
		*stop = MAX(*stop, c_stop);
	}
}

static Int2 get_master_order(SeqIdPtr ids, SeqIdPtr sip)
{
	Int2 i;
	
	for(i =0; ids!=NULL; ids = ids->next, ++i)
	{
		if(SeqIdForSameBioseq(ids, sip))
			return i;
	}
	return -1;
}

NLM_EXTERN Boolean SeqAlignStartStopById(SeqAlignPtr align, SeqIdPtr id, Int4Ptr start, Int4Ptr stop, Uint1Ptr strand)
{
	Int2 i, n;
	Boolean is_found;
	Int4 c_start, c_stop;
	DenseSegPtr dsp;
	DenseDiagPtr ddp;
	StdSegPtr ssp;
	SeqLocPtr loc;
 
	*start = -1;
	*stop = -1;
	switch(align->segtype)
	{
		case 2:         /*DenseSeg*/
			dsp = (DenseSegPtr)(align->segs);
			if(id == NULL)
				i =0;
			else
			{
				i = get_master_order(dsp->ids, id);
				if( i == -1)
					return FALSE;
			}
			for(n =0; n<dsp->numseg; ++n)
			{
				c_start = dsp->starts[n*(dsp->dim) +i];
				if(c_start != -1) /*check for a non-gapped region*/ 
				{
					c_stop = c_start + dsp->lens[n] -1;
					load_start_stop(start, stop, c_start, c_stop);
				}
			}
			*strand = align_strand_get(dsp->strands, i);
			return (*start != -1);
			
		case 3:
			ssp = (StdSegPtr)(align->segs);
			while(ssp)
			{
				is_found = FALSE;
				for (loc = ssp->loc; loc!=NULL && !is_found; loc=loc->next)
				{
					if(loc->choice != SEQLOC_EMPTY)
					{
					if(id == NULL)
						is_found = TRUE;
					else
						
						is_found=SeqIdForSameBioseq(SeqLocId(loc), id);
					if(is_found)
					{
						load_start_stop(start, stop, SeqLocStart(loc), SeqLocStop(loc));
						*strand = SeqLocStrand(loc);
					}
					}
				}
				ssp = ssp->next;
			}
			return (*start != -1);
			
		case 1:
			ddp = (DenseDiagPtr)(align->segs);
			while(ddp)
			{
				if(id == NULL)
					i =0;
				else
					i = get_master_order(ddp->id, id);
				if(i != -1)
				{
					c_start = ddp->starts[i];
					c_stop = ddp->starts[i] + ddp->len -1;
					*strand = align_strand_get(ddp->strands, i);
					load_start_stop(start, stop, c_start, c_stop);
				}
				ddp = ddp->next;
			}			
			return (*start != -1);
			
		default:
			return FALSE;
	}
}
	


/* 
   Function to count the Number of Segments in a Single SeqAlign
*/

NLM_EXTERN Uint4 LIBCALL SeqAlignCountSegs(SeqAlignPtr salp) {
    Uint4 numsegs;
    if(salp==NULL) 
        return 0;
    numsegs=0;
    if(salp->segtype==1) { /* DenseDiag */
        DenseDiagPtr ddp;
        ddp=(DenseDiagPtr)salp->segs;
        while(ddp) {
            numsegs++;
            ddp=ddp->next;
        }
    } else if (salp->segtype==2) { /* DenseSeg */
        DenseSegPtr dsp;
        dsp = (DenseSegPtr) salp->segs;
        if(dsp)
            numsegs=dsp->numseg;
    } else if (salp->segtype==3) { /* StdSeg */
        StdSegPtr ssp;
        ssp = (StdSegPtr)salp->segs;
        if(ssp!=NULL) {
            {
                SeqLocPtr slp;
                slp = ssp->loc;
                while(slp!=NULL) {
                    numsegs++;
                    slp=slp->next;
                }
            }
        }
    } else if(salp->segtype==4) { /* Packed Seg. Optimal for editing alignments */
        PackSegPtr psp;
        psp = (PackSegPtr) salp->segs;
        if (psp)
            numsegs=psp->numseg;
    } else if (salp->segtype ==5) {/* Assume that Linked list of SeqAlign set
                                      always refers to the same set of sequences */
        SeqAlignPtr salp_tmp;
        salp_tmp = (SeqAlignPtr) salp->segs;
        while(salp_tmp!=NULL) {
            numsegs+=SeqAlignCountSegs(salp_tmp);
            salp_tmp=salp_tmp->next;
        }
    } else if (salp->segtype == 6) { /* Temporarely Supported SeqAlign Type */
        CompSegPtr csp;
        csp = (CompSegPtr) salp->segs;
        numsegs=csp->numseg;
    } else {
        ErrPostEx(SEV_WARNING,0,0,"Undefined Seqalign segtype=%d\n",(int)salp->segtype);
    }
    return numsegs;
}


NLM_EXTERN void SeqAlignAddInSeqEntry (SeqEntryPtr sep, SeqAnnotPtr sap)
{
  BioseqSetPtr bssp;
  BioseqPtr    bsp;
  SeqAnnotPtr  saptmp;
  
  if (sap!=NULL && sep != NULL)
  {
     if (sap->data !=NULL) 
     {
        if (IS_Bioseq(sep)) 
        {
           bsp=(BioseqPtr)sep->data.ptrvalue;
           if (bsp->annot==NULL)
              bsp->annot=sap;
           else {
              for (saptmp=bsp->annot; saptmp->next!=NULL; saptmp=saptmp->next)
                 continue;
              saptmp->next = sap;
           }
        }
        else if (IS_Bioseq_set(sep)) 
        {
           bssp = (BioseqSetPtr)sep->data.ptrvalue;
           if (bssp->annot==NULL)
              bssp->annot=sap;
           else {
              for (saptmp=bssp->annot; saptmp->next!=NULL; saptmp=saptmp->next)
                 continue;
              saptmp->next = sap;
           }
        }
     }
  }
}

NLM_EXTERN SeqAlignPtr SeqAlignExtractByIds(SeqAlignPtr PNTR align, SeqIdPtr sip_1, SeqIdPtr sip_2)
{
	DenseSegPtr dsp;
	SeqAlignPtr prev, next, curr;
	Boolean match;
	SeqAlignPtr h_align, p_align;

	prev = NULL;
	curr = *align;
	p_align = NULL;
	h_align = NULL;
	while(curr)
	{
		match = FALSE;
		next = curr->next;
		if(curr->segtype == 2)
		{
			dsp = (DenseSegPtr) curr->segs;
			if(SeqIdMatch(dsp->ids, sip_1) 
				&& SeqIdMatch(dsp->ids->next, sip_2))
				match = TRUE;
		}

		if(match)
		{
			if(prev == NULL)
				*align = next;
			else
				prev->next = next;
			curr->next = NULL;

			if(p_align == NULL)
				h_align = curr;
			else
				p_align->next = curr;
			p_align = curr;
		}
		else
			prev = curr;
		curr = next;
	}

	return h_align;
}

NLM_EXTERN Int4 SeqAlignCount(SeqAlignPtr align)
{
	Int4 num = 0;

	while(align)
	{
		++num;
		align = align->next;
	}

	return num;
}
