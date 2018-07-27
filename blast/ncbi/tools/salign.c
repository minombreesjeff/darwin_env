static char const rcsid[] = "$Id: salign.c,v 6.7 2003/12/29 21:45:37 coulouri Exp $";

/*   salign.c
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
* File Name:  salign.c
*
* Author:  Colombe Chappey 
*
* Version Creation Date:   8/22/99
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

#include <salign.h>
#include <ncbi.h>
#include <objalign.h>
#include <bandalgn.h>
#include <blast.h>
#include <salutil.h>
#include <salsap.h>
#include <salstruc.h>
#include <salpedit.h>

#ifdef SALSA_DEBUG
#include <simutil.h>
#include <toasn3.h>
#include <utilpub.h>
#include <tfuns.h>
#endif


#define BAND_LIMIT 100
 
typedef struct nodehit
{
  Int2        index;
  Boolean     open;
  SeqAlignPtr salp;
  Int4        score;
  Nlm_FloatHi bit_score;
  Nlm_FloatHi evalue;
  struct nodehit PNTR     child;
  struct nodehit PNTR     next;
} NodeHit, PNTR NodeHitPtr;

static ValNodePtr traverseGraph (NodeHitPtr head, NodeHitPtr node, Int2 total, Int2 level, Int2Ptr tab, ValNodePtr vnp, Boolean * change);

NLM_EXTERN MashPtr MashNew (Boolean is_prot)
{
  MashPtr msp;
  
    if((msp = (MashPtr)MemNew(sizeof(Mash)))==NULL) {
      ErrPostEx(SEV_WARNING, 0, 0, "MemNew returned NULL");
      return NULL;
    }
     msp->is_prot = is_prot;
     msp->band_method = 2;
     msp->multidim = FALSE;
     if (msp->is_prot) 
     {
        msp->reward = 1;
        msp->penalty = -2;
        msp->gap_open = 11;
        msp->gap_extend = 1;
	msp->matrixname="BLOSUM62";
        msp->wordsize = 3;
        msp->gap_x_dropoff = 15;
        msp->gap_x_dropoff_final = 500;   /****25****/
        msp->filter = FILTER_SEG;
     } 
     else {
        msp->reward = 2;
        msp->penalty = -3;
        msp->gap_open = 10;
        msp->gap_extend = 2;
	msp->matrixname=NULL;
        msp->wordsize = 11;
        msp->gap_x_dropoff = 50;
        msp->gap_x_dropoff_final = 50;
        msp->filter = FILTER_DUST;
     }
     msp->translate_prot = FALSE;
     msp->transalp = NULL;
     msp->use_gapped_blast = TRUE;
     msp->lg1_ext = 0; 
     msp->rg1_ext =0; 
     msp->lg2_ext =0; 
     msp->rg2_ext =0; 
     msp->lg1_open =0; 
     msp->lg2_open =0; 
     msp->rg1_open =0; 
     msp->rg2_open =0; 
     msp->blast_threshold = 50;
     msp->choice_blastfilter = 2;      /* 1 */
     msp->splicing = TRUE;              /*FALSE;*/
     msp->map_align = FALSE;
     msp->align_ends = TRUE;
     return msp;
}

static Int4Ptr PNTR LIBCALL BlastStyleMatDestruct(Int4Ptr PNTR matrix){
   MemFree(matrix);
   matrix=NULL;
   return matrix;
}

static GlobalBandStructPtr LIBCALL DestructBandStruct(GlobalBandStructPtr gbsp){
   gbsp->matrix = BlastStyleMatDestruct(gbsp->matrix);
   if(gbsp->seq2!=NULL) MemFree(gbsp->seq2);
   if(gbsp->seq1!=NULL) MemFree(gbsp->seq1);
   gbsp->seq1=NULL;
   gbsp->seq2=NULL;
   gbsp=GlobalBandStructDelete(gbsp);
   return gbsp;
}

/* BANDALIGN */
static GlobalBandStructPtr CC_CreatBandStruct(SeqLocPtr slp1, SeqLocPtr slp2, MashPtr msp)
{
   GlobalBandStructPtr gbsp;
   Boolean is_prot;

   is_prot = (Boolean)(msp->is_prot || msp->translate_prot);
   gbsp = CreatBandStruct(slp1, slp2, NULL,(Boolean) is_prot, (Int2) msp->band_method);
   if (( ChangeGlobalBandMatrix(gbsp, is_prot, msp->matrixname,(Int4) msp->penalty, (Int4)msp->reward)) != 0) {
      gbsp = GlobalBandStructDelete (gbsp);
      return NULL;
   }
   SetGlobaltOptions(gbsp,msp->lg1_ext,msp->rg1_ext,
		          msp->lg2_ext, msp->rg2_ext, 
   		          msp->lg1_open, msp->lg2_open, 
		          msp->rg1_open, msp->rg2_open,
		          (Int2)msp->gap_open, (Int2) msp->gap_extend);
   return gbsp;
}
/********************************************************/
static SeqAlignPtr BandAlignTwoSeqLocsFunc (SeqLocPtr slp1, SeqLocPtr slp2, MashPtr msp)
{
  GlobalBandStructPtr gbsp;
  SeqAlignPtr         newalign = NULL;
  SeqIntPtr sit;
  Boolean   is_prot;

  if (slp1==NULL || slp2==NULL)
     return NULL;
  gbsp = CC_CreatBandStruct(slp1, slp2, msp);
  if (gbsp!=NULL) 
  {
    is_prot = (Boolean)(msp->is_prot || msp->translate_prot);
    gbsp->options->low = -(Int4) SeqLocLen(slp1);
    gbsp->options->up =(Int4) SeqLocLen(slp2);
    gbsp->options->start_diag = gbsp->options->low;
    gbsp->options->width = gbsp->options->up-gbsp->options->low + 1;
/*     
    if (gbsp->options->width>2*BAND_LIMIT) {
      SetLowUpFromBlast(gbsp->options, is_prot, (Int2)0, (Int2)30,slp1, slp2);
    }
    else 
    {
       gbsp->search_type = (Uint1) G_BAND_QGAP; 
    }
*/
    newalign = GlobalBandToSeqAlign(gbsp);
    if (newalign != NULL)
    {    
       if (SeqLocStrand(slp1) == Seq_strand_minus) {
          sit=(SeqIntPtr)slp1->data.ptrvalue;
          sit->strand = Seq_strand_plus;
       }
       if (SeqLocStrand(slp2) == Seq_strand_minus) {
          sit=(SeqIntPtr)slp2->data.ptrvalue;
          sit->strand = Seq_strand_plus;
       } 
       AdjustOffSetsInSeqAlign (newalign, slp1, slp2);
    } 
    gbsp = DestructBandStruct(gbsp);
  } 
  else {
    ErrPostEx(SEV_WARNING, 0, 0, "Could not Create GlobalBandStruct");
  }
  return newalign;
}

static SeqAlignPtr BandAlignTwoSeqLocs (SeqLocPtr slp1, SeqLocPtr slp2, MashPtr msp)
{
  SeqAlignPtr         newalign = NULL;
  ValNodePtr          vnp;

  newalign = BandAlignTwoSeqLocsFunc (slp1, slp2, msp);
  if (newalign == NULL) {
     if ((msp->is_prot || msp->translate_prot)) {
        if (msp->reward > 1)
           msp->reward = 1;
        if (msp->penalty < -1)
           msp->penalty = -1;
     }
     else {
        if (msp->reward > 1)
           msp->reward = 1;
        if (msp->penalty < -1)
           msp->penalty = -1;
     }
     newalign = BandAlignTwoSeqLocsFunc (slp1, slp2, msp);
  } 
  if (newalign == NULL) {
     vnp = NULL;
     ValNodeAddPointer (&vnp, 0, slp1);
     ValNodeAddPointer (&vnp, 0, slp2);
     newalign = SeqLocToFastaSeqAlign (vnp); 
     ValNodeFree (vnp);
  }
  return newalign;
}

/********************************************************/
static Int2 number_hits (SeqAlignPtr salp)
{
  SeqAlignPtr salptmp;
  Int2 j=0;

  if (salp == NULL)
     return 0;
  for (salptmp=salp; salptmp!=NULL; salptmp=salptmp->next)
     j++;
  return j;
}

/*****************************************************
***
*** try_insert_seqalign
***    salplst = list of seqalign 
***    salp = tobe inserted
***    choice = 0 everything is kept
***             1 no overlaps
***             2 overlap in seq 1
***             3 overlap in seq 2
***             4 overlaps in both sequences of same strand
***             5 
***
*******************************************************/
static Boolean precede (Int4 pos1, Int4 pos2, Boolean is_plus)
{
  if (is_plus)
     return (Boolean) (pos1 < pos2);
  return (Boolean) (pos1 > pos2);
}

static SeqAlignPtr try_insert_seqalign (SeqAlignPtr salplst, SeqAlignPtr salp, Uint1 choice)
{
  SeqAlignPtr  tmp, tmp2;
  Int4         start, stop, start2, stop2,
               starti, stopi, 
               starti2, stopi2,
               startnext,
               startnext2, stopnext2;
  Boolean      goOn,
               st1, st2;

  if (salp == NULL)
     return NULL;
  if (salplst == NULL) {
     salplst = SeqAlignDup (salp);
     return salplst;
  }
  starti= SeqAlignStart (salp, 0);
  stopi = SeqAlignStop (salp, 0);
  starti2= SeqAlignStart (salp, 1);
  stopi2 = SeqAlignStop (salp, 1);
  st1 = (Boolean) !(SeqAlignStrand(salp,0) == Seq_strand_minus);
  st2 = (Boolean) !(SeqAlignStrand(salp,1) == Seq_strand_minus);

  tmp = salplst;
  start = SeqAlignStart (tmp, 0);
  stop = SeqAlignStop (tmp, 0);
  start2= SeqAlignStart (tmp, 1);
  stop2 = SeqAlignStop (tmp, 1);
  if (choice == 1)
     goOn =(Boolean) (precede(stopi, start, st1) && precede(stopi2, start2, st2));
  else if (choice == 2)
     goOn = (Boolean) (precede(stopi, start, st1) && precede(starti2, start2, st2) && precede(stopi2, stop2, st2));
  else if (choice == 3)
     goOn = (Boolean) (precede(stopi2, start2, st2) && precede(starti, start, st1) && precede(stopi, stop, st1));
  else if (choice == 4)
     goOn = (Boolean) (precede(starti, start, st1) && precede(stopi, stop, st1) && precede(starti2, start2, st2) && precede(stopi2, stop2, st2));
  else if (choice == 5)
     goOn = (Boolean) (precede(starti, start, st1));
  else
     return salplst;
  if (goOn)
  {
     tmp2 = SeqAlignDup (salp);
     tmp2->next = salplst;
     salplst = tmp2;
  }
  else {
     while (tmp != NULL)
     {
        if (choice == 1)
           goOn = (Boolean) (precede(stop, starti, st1) && precede(stop2, starti2, st2));
        else if (choice == 2)
           goOn = (Boolean) (precede(stop, starti, st1) && precede(start2, starti2, st2) && precede(stop2, stopi2, st2));
        else if (choice == 3)
           goOn = (Boolean) (precede(stop2, starti2, st2) && precede(start, starti, st1) && precede(stop, stopi, st1));
        else if (choice == 4)
           goOn = (Boolean) (precede(start, starti, st1) && precede(stop, stopi, st1) && precede(start2, starti2, st2) && precede(stop2, stopi2, st2));
        else 
           goOn=(Boolean)(precede(start, starti, st1));
        if (goOn)
        {
           if (tmp->next == NULL) {
              tmp2 = SeqAlignDup (salp);
              tmp->next = tmp2;
              break;
           }
           else {
              startnext = SeqAlignStart (tmp->next, 0);
              startnext2= SeqAlignStart (tmp->next, 1);
              stopnext2 = SeqAlignStop (tmp->next, 1);
              if (choice == 5) {
                 goOn=(Boolean)(precede(starti, startnext, st1));
              }
              else {
                 goOn=(Boolean)(precede(stopi, startnext, st1)  && precede(starti2, startnext2, st2) && precede(stopi2, stopnext2, st2)) ;
              }
              if (goOn)
              {
                 tmp2 = SeqAlignDup (salp);
                 tmp2->next = tmp->next;
                 tmp->next = tmp2;
                 break;
              }
           }
        }
        tmp = tmp->next;
        if (tmp!=NULL) {
           start = SeqAlignStart (tmp, 0);
           stop = SeqAlignStop (tmp, 0);
           start2= SeqAlignStart (tmp, 1);
           stop2 = SeqAlignStop (tmp, 1);
        }
     }
  }
  return salplst;
}

static SeqAlignPtr SortBlastHits (SeqAlignPtr salp, Int4 threshold, Uint1 choice)
{
  SeqAnnotPtr sap, sap2;
  SeqAlignPtr salpdup,
              salptmp,
              head = NULL,
              select = NULL,
              tmp = NULL, pre=NULL, preselect=NULL;
  Nlm_FloatHi bit_score;
  Nlm_FloatHi evalue;
  FloatLo     gap_length_ratio,
              gap_length_ratio1=1.00;
  Int4        score, 
              number,
              totalvalmax = INT4_MAX, 
              valmax;
  Int4        gap_count = 0, 
              gap_count1= 0;
  Boolean     goOn = TRUE;
  Boolean     ok;

  if (salp == NULL)
     return salp;
  sap = SeqAnnotForSeqAlign (salp);
  sap2 = (SeqAnnotPtr) AsnIoMemCopy ((Pointer) sap, (AsnReadFunc) SeqAnnotAsnRead, (AsnWriteFunc) SeqAnnotAsnWrite);
  if (sap2!=NULL) {
     salpdup = (SeqAlignPtr)sap2->data;
     sap2->data = NULL;
     SeqAnnotFree (sap2);
  }
  while (goOn) 
  { 
     valmax = 0;
     pre = select = preselect = NULL;
     for (salptmp=salpdup; salptmp!=NULL; salptmp=salptmp->next) 
     {
        GetScoreAndEvalue (salptmp, &score, &bit_score, &evalue, &number);
        gap_count = SeqAlignGapCount (salptmp);
        gap_length_ratio = (FloatLo)gap_count/SeqAlignLength(salptmp);
        if (score > valmax || (gap_count1>0 && gap_count == 0 && gap_length_ratio1 > 0.05 && score >= threshold))
        {
           valmax = score;
           select = salptmp;
           preselect = pre;
           if (head==NULL) {
              gap_count1 = gap_count; 
              gap_length_ratio1 = gap_length_ratio;
           }
        }
        pre = salptmp;
     }
     if (valmax < threshold && head == NULL)
     {
        threshold = 20;
     }
     if (valmax >= threshold && select != NULL) 
     {
        if (head!=NULL)
           ok=(Boolean)((SeqAlignStrand(head,0) == SeqAlignStrand(select,0))
              && (SeqAlignStrand(head,1) == SeqAlignStrand(select,1)));
        else ok=TRUE;
        if (ok) {
           head = try_insert_seqalign (head, select, choice);
        }
        tmp=select;
        if (preselect==NULL) {
           salpdup = select->next;
        } else {
           preselect->next = select->next; 
        }   
        tmp->next = NULL;
        SeqAlignSetFree (tmp);
        totalvalmax = valmax;
     }
     else 
        goOn = FALSE;
  }
  if (salpdup!=NULL) 
     SeqAlignSetFree (salpdup);
  return head;
}

static void check_strand_inSeqalign (SeqAlignPtr salp, Uint1 strand, Int2 index)
{
  DenseSegPtr dsp;
  Int2        j; 
  Uint1Ptr    strandp;

  if (salp!=NULL) {
     dsp = (DenseSegPtr) salp->segs;
     if (dsp->strands != NULL) {
        strandp = dsp->strands;
        strandp += index;
        for (j=0; j<dsp->numseg; j++) {
           if (*strandp != strand)
              *strandp = strand;
           strandp += dsp->dim;
        }
     }   
  }
}

static SeqAlignPtr SeqAlignList2PosStrand (SeqAlignPtr salp) 
{
  SeqAlignPtr salptmp;
  Int4Ptr     lenp;
  Int4Ptr     startp;
  Uint1Ptr    strandp;
  Int4        numseg;
  Int2        dim;
  Int4        j, k, n, tmp;


for (salptmp=salp; salptmp!=NULL; salptmp=salptmp->next)
{
  if ((SeqAlignStrand(salptmp,0) == Seq_strand_minus) 
   && (SeqAlignStrand(salptmp,1) != Seq_strand_minus))
  {
     if (salptmp->segtype == 2)
     {
         DenseSegPtr dsp;
        dsp = (DenseSegPtr) salptmp->segs;
        strandp = dsp->strands;
        numseg = dsp->numseg;
        dim = dsp->dim;
        lenp = dsp->lens;
        startp = dsp->starts;
        for (j=0; j < numseg*dim && strandp!=NULL; j++, strandp++)
            {
                if (*strandp == Seq_strand_minus)
                    *strandp = Seq_strand_plus;
                else if (*strandp != Seq_strand_minus)
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
}
return salp;
}

static SeqAlignPtr BlastTwoSeqLocs (SeqLocPtr slp1, SeqLocPtr slp2, Boolean is_prot, MashPtr msp)
{
  BLAST_OptionsBlkPtr options;
  SeqAlignPtr seqalign;
  SeqIntPtr   sit;
  Uint1       strand1, strand2;
  Boolean     delete_mash = FALSE;

  if (slp1 == NULL || slp2 == NULL)
     return NULL;
  if (msp == NULL) {
     msp = MashNew (is_prot);
     delete_mash = TRUE;
  }
  if (msp == NULL)
     return NULL;
  options = BLASTOptionNew((is_prot == FALSE) ? "blastn":"blastp",(Boolean) msp->use_gapped_blast);
  if (msp!=NULL) {
     options->gap_open= msp->gap_open;
     options->gap_extend= msp->gap_extend;
     if(is_prot) {
        if(msp->matrixname!=NULL) 
           options->matrix=StringSave(msp->matrixname);
     }
     else {
        options->reward = msp->reward;
        options->penalty= msp->penalty;
        options->wordsize = msp->wordsize;
     }
/*
     options->filter = 0;                 *//** msp->filter; **/
     options->gap_x_dropoff= msp->gap_x_dropoff;
     options->gap_x_dropoff_final= msp->gap_x_dropoff_final;
  }
  if (is_prot)
     options = BLASTOptionValidate (options, "blastp");
  else
     options = BLASTOptionValidate (options, "blastn");
  if (options == NULL)
     return NULL;
  if (!is_prot) {
     strand1=SeqLocStrand(slp1);
     strand2=SeqLocStrand(slp2);
     sit=(SeqIntPtr)slp1->data.ptrvalue;
     sit->strand = Seq_strand_both;
     sit=(SeqIntPtr)slp2->data.ptrvalue;
     sit->strand = Seq_strand_both;
  }
  seqalign = BlastTwoSequencesByLoc (slp1, slp2, NULL, options);
  if (options->wordsize==0) {
     while (seqalign==NULL && (is_prot==FALSE && options->wordsize>8)) {
        options->wordsize--;
        seqalign = BlastTwoSequencesByLoc (slp1, slp2, NULL, options);
     }
  }
  options=BLASTOptionDelete(options);
  if (delete_mash)
     MemFree (msp);
  if (!is_prot) {
     sit=(SeqIntPtr)slp1->data.ptrvalue;
     sit->strand = strand1;
     sit=(SeqIntPtr)slp2->data.ptrvalue;
     sit->strand = strand2;
  }
  seqalign = SeqAlignList2PosStrand (seqalign);
  return seqalign;
}


static Uint1 find_bestframe (SeqLocPtr slp, Int2 genCode)
{
  ByteStorePtr  bs;
  Int4          len;
  Int4          max = 0;
  Int4          bslen;
  Uint1         bestframe = 0, 
                frame;
  CharPtr       str;

  /* Only works for genCode == ncbieaa */
  for (frame = 1; frame <= 3; frame ++) 
  {
     bs = cds_to_pept (slp, frame, (Int2) Seq_code_ncbieaa, TRUE);
     if (bs!=NULL) {
        str = (CharPtr) BSMerge (bs, NULL);
	bslen=BSLen(bs);
        BSFree (bs);
        for (len = 0; len<bslen; len++)
           if (str[len]=='*')
              break;
        MemFree (str);
        if (len > max) {
           max = len;
           bestframe = frame;
        }
     }
  }
  return bestframe;
}

static SeqLocPtr TranslateSeqLoc (SeqLocPtr slp, Int2 genCode, Uint1 *frame)
{
  BioseqPtr     bsp = NULL;
  ByteStorePtr  bs;
  SeqLocPtr     slpp;
  Int4          length,bslen;
  
  slp = seqloc2fuzzloc (slp, TRUE, TRUE);
  *frame = find_bestframe (slp, genCode);
  bs = cds_to_pept (slp, *frame, (Int2) Seq_code_ncbieaa, TRUE);

  if(genCode != (Int2) Seq_code_ncbieaa ) {
      bslen=BSLen(bs);
      bs=BSConvertSeq(bs, (Uint1) genCode,(Uint1)Seq_code_ncbieaa,(Int4) bslen);
  }
  bsp = BioseqNew ();
  if (bsp != NULL) {
     bsp->repr = Seq_repr_raw;
     bsp->mol = Seq_mol_aa;
     bsp->seq_data_type = (Uint1)genCode;
     bsp->seq_data = bs;
     bsp->length = BSLen (bs);
     bsp->id = MakeNewProteinSeqId (slp, NULL);
     if (*frame==2)
        length = (Int4)((SeqLocLen(slp)-1) / (Int4)3);
     else if (*frame==3) 
        length = (Int4)((SeqLocLen(slp)-2) / (Int4)3);
     else
        length = (Int4)(SeqLocLen(slp) / (Int4)3);
     slpp = SeqLocIntNew (0, length-1, Seq_strand_plus, bsp->id);
  }
  return slpp;
}
 



/*************************************************************
*** GlobalAlignTwoSeqLocs
***   if short sequences (length < 150)
***         aligns using BandAlign only
***   else 
***         Blast the 2 seqlocs
***         if find no hit:
***            aligns using BandAlign
***         else if finds 1 hit (NOW: the longest)
***            if alignment reaches ends (3' or 5')
***               extend seqalign with the missing ends
***            else 
***               aligns unaligned seqlocs using BandAlign
***               merge seqaligns
***
*** !!!SelectBestHit : select 1 hit
***                    should select several if any
***
**************************************************************/
static SeqAlignPtr AlignExtreme5 (SeqAlignPtr salp, MashPtr msp, Int4 slpstart1, Int4 start1, Int4 slpstart2, Int4 slpstop2, Int4 start2, SeqIdPtr sip1, SeqIdPtr sip2, Uint1 strand1, Uint1 strand2)
{
  SeqAlignPtr salp2;
  SeqLocPtr   newslp1, newslp2;
  Boolean     touch_end5;
  
  if (strand2 == Seq_strand_minus) 
  {
         touch_end5 = (Boolean)((slpstart1==start1) || (slpstop2==start2));
         if (touch_end5) 
         {
            salp = SeqAlignEndExtend (salp, slpstart1, slpstop2, -1, -1, start1, start2, -1, -1, strand1, strand2);
         } 
         else 
         {
            start1 -= 1; 
            start2 += 1; 
            newslp1=SeqLocIntNew (0, start1, strand1, sip1);
            newslp2=SeqLocIntNew (start2, slpstop2, strand2, sip2);
	    msp->rg1_ext=(Int4)msp->gap_extend;
	    msp->lg1_ext=0;
	    msp->rg2_ext=(Int4)msp->gap_extend;
	    msp->lg2_ext=0;
	    msp->rg1_open=(Int4)msp->gap_open;
	    msp->lg1_open=0;
	    msp->rg2_open=(Int4)msp->gap_open;
	    msp->lg2_open=0;
            salp2 = BandAlignTwoSeqLocs (newslp1, newslp2, msp);
            salp = SeqAlignMerge (salp, salp2, FALSE);
         }
  } 
  else 
  {
         touch_end5 = (Boolean)((ABS(slpstart1-start1)<3) || (ABS(slpstart2-start2)<2));
         if (touch_end5) 
         {
            salp = SeqAlignEndExtend (salp, slpstart1, slpstart2, -1, -1, start1, start2, -1, -1, strand1, strand2);
         } 
         else 
         {
            start1 -= 1; 
            start2 -= 1; 
            newslp1=SeqLocIntNew (0, start1, strand1, sip1);
            newslp2=SeqLocIntNew (0, start2, strand2, sip2);
	    msp->rg1_ext=(Int4)msp->gap_extend;
	    msp->lg1_ext=0;
	    msp->rg2_ext=(Int4)msp->gap_extend;
	    msp->lg2_ext=0;
	    msp->rg1_open=(Int4)msp->gap_open;
	    msp->lg1_open=0;
	    msp->rg2_open=(Int4)msp->gap_open;
	    msp->lg2_open=0;
            salp2 = BandAlignTwoSeqLocs (newslp1, newslp2, msp);
            salp = SeqAlignMerge (salp, salp2, FALSE);
         }
  }
  return salp;
}

static SeqAlignPtr AlignExtreme3 (SeqAlignPtr salp, MashPtr msp, Int4 slpstop1, Int4 stop1, Int4 slpstart2, Int4 slpstop2, Int4 stop2, SeqIdPtr sip1, SeqIdPtr sip2, Uint1 strand1, Uint1 strand2)
{
  SeqAlignPtr salp2;
  SeqLocPtr   newslp1, newslp2;
  Boolean     touch_end3;
  
  if (strand2 == Seq_strand_minus) 
  {
         touch_end3 = (Boolean)((slpstop1==stop1) || (slpstart2==stop2));
         if (touch_end3) 
         {
            salp = SeqAlignEndExtend (salp, -1, -1, slpstop1, slpstart2, -1, -1, stop1, stop2, strand1, strand2);
         } 
         else 
         {
            stop1 += 1;
            stop2 -= 1;
            newslp1=SeqLocIntNew(stop1,slpstop1, strand1, sip1);
            newslp2=SeqLocIntNew(0,stop2, strand2, sip2);
	    msp->lg1_ext= (Int4)msp->gap_extend;
	    msp->rg1_ext=0;
	    msp->lg2_ext= (Int4) msp->gap_extend;
	    msp->rg2_ext=0;
	    msp->lg1_open= (Int4)msp->gap_open;
	    msp->rg1_open=0;
	    msp->lg2_open= (Int4)msp->gap_open;
	    msp->rg2_open=0;
            salp2= BandAlignTwoSeqLocs (newslp1,newslp2, msp);
            salp = SeqAlignMerge (salp, salp2, TRUE);
         } 
  } 
  else 
  {
         touch_end3 = (Boolean)((ABS(slpstop1-stop1)<3) || (ABS(slpstop2-stop2)<3));
         if (touch_end3) 
         {
            salp = SeqAlignEndExtend (salp, -1, -1, slpstop1, slpstop2, -1, -1, stop1, stop2, strand1, strand2);
         } 
         else 
         {
            stop1 += 1;
            stop2 += 1;
            newslp1=SeqLocIntNew(stop1,slpstop1, strand1, sip1);
            newslp2=SeqLocIntNew(stop2,slpstop2, strand2, sip2);
	    msp->lg1_ext=(Int4)msp->gap_extend;
	    msp->rg1_ext=0;
	    msp->lg2_ext=(Int4) msp->gap_extend;
	    msp->rg2_ext=0;
	    msp->lg1_open=(Int4)msp->gap_open;
	    msp->rg1_open=0;
	    msp->lg2_open=(Int4)msp->gap_open;
	    msp->rg2_open=0;
            salp2 = BandAlignTwoSeqLocs (newslp1, newslp2, msp);
            salp = SeqAlignMerge (salp, salp2, TRUE);
         } 
  }
  return salp;
}

static SeqAlignPtr align_extrem (SeqAlignPtr salp, SeqLocPtr slp1, SeqLocPtr slp2, MashPtr msp)
{
  SeqAlignPtr tmp;
  SeqIdPtr    sip1, sip2;
  Int4        start1, start2,
              stop1, stop2;
  Int4        slpstart1, slpstart2,
              slpstop1, slpstop2;
  Uint1       strand1, strand2;
  
  sip1 = SeqLocId(slp1);
  sip2 = SeqLocId(slp2);
  strand1 = SeqAlignStrand (salp, 0);
  strand2 = SeqAlignStrand (salp, 1);
  slpstart1= SeqLocStart(slp1);
  slpstop1 = SeqLocStop(slp1);
  slpstart2= SeqLocStart(slp2);
  slpstop2 = SeqLocStop(slp2);  
  start1 = SeqAlignStart (salp, 0);
  start2 = SeqAlignStart (salp, 1);
  salp = AlignExtreme5 (salp, msp, slpstart1, start1, slpstart2, slpstop2, start2, sip1, sip2, strand1, strand2);
  check_strand_inSeqalign (salp, strand1, 0);
  check_strand_inSeqalign (salp, strand2, 1);

  if (salp!=NULL)
  {
     tmp = salp;
     while (tmp->next!=NULL)
        tmp = tmp->next;
     stop1 = SeqAlignStop (tmp, 0);
     stop2 = SeqAlignStop (tmp, 1);  
     tmp = AlignExtreme3 (tmp, msp, slpstop1,  stop1,  slpstart2, slpstop2, stop2, sip1, sip2, strand1, strand2);
     check_strand_inSeqalign (tmp, strand1, 0);
     check_strand_inSeqalign (tmp, strand2, 1);
  }
  return salp;
}

/******************************************************
static Boolean is_intron (CharPtr str, Int4 len)
{
  if (str[0]=='G' && str[1]=='T' && str[len-2]=='A' && str[len-1]=='G')
     return TRUE;
  return FALSE;
}
*********************************************************/
static FloatHi is_donor (CharPtr str, Int4 len)
{
  FloatHi one[4]={0.35, 0.35, 0.19, 0.11};
  FloatHi two[4]={0.59, 0.13, 0.14, 0.14};
  FloatHi three[4]={0.08, 0.02, 0.82, 0.08};
  FloatHi four[4]={0.01, 0.01, 1.00, 0.01};
  FloatHi five[4]={0.01, 0.01, 0.01, 1.00};
  FloatHi six[4]={0.51, 0.03, 0.43, 0.03};
  FloatHi seven[4]={0.71, 0.08, 0.12, 0.09};
  FloatHi eight[4]={0.06, 0.05, 0.84, 0.05};
  FloatHi nine[4]={0.15, 0.16, 0.17, 0.52};
  FloatHi score =1.000;
  Int4   i;
  Int4  PNTR num=NULL;

  if ((num = (Int4Ptr)MemNew(len*sizeof(Int4)))==NULL) {
    return(-1);
  }

  for (i = 0; i <= len; i++){
    if (str[i]=='A')
      num[i] = 0;
    if (str[i]=='C')
      num[i] = 1;
    if (str[i]=='G')
      num[i] = 2;
    if (str[i]=='T')
      num[i] = 3;
  }
  score *= one[num[0]];
  score *= two[num[1]];
  score *= three[num[2]];
  score *= four[num[3]];
  score *= five[num[4]];
  score *= six[num[5]];
  score *= seven[num[6]];
  score *= eight[num[7]];
  score *= nine[num[8]];

  MemFree(num);
  num=NULL;

  return score;
}

static Int4 getSplicePos (CharPtr str)
{
  Int4     offset = -1;
  Int4     xcursor = 0;
  Int4     c;
  FloatHi  topscore = -FLT_MAX,
           score;
  Char     tmpstr[9];
  Int4     length;

  if (str == NULL)
    return -1;
  length = MIN(StringLen(str)/2-10, 10);
  while (xcursor <= length)
  {
      for (c = 0; c < 9; c++)
      {
        tmpstr[c] = str[xcursor+c];
      }
      if ((score=is_donor(tmpstr, 8)) > topscore)
      {
        topscore = score;
        offset = xcursor;
      }
      xcursor += 1;
  }
  if (topscore > 0.000010 && offset >=0)
    return offset+3;
  return -1;
}



static SeqAlignPtr align_btwhits (SeqAlignPtr salp, SeqIdPtr sip1, SeqIdPtr sip2, MashPtr msp)
{
  SeqAlignPtr tmp,
              newsalp,
              newsalphead, newtmp;
  SeqLocPtr   slp1, slp2;
  Int4        x1, y1, x2, y2;
  Int4        len;
  Int4        offset;
  Uint1       st1, st2;
  CharPtr     str;
  Boolean     search_intron = FALSE;

  if (salp == NULL) return NULL;
  if (msp != NULL) {
     search_intron = msp->splicing;
  }
  newsalphead = SeqAlignDup (salp);
  newtmp = newsalphead;
  tmp = salp ->next;
  while (tmp != NULL)
  {
     x1 = SeqAlignStop (newtmp, 0);
     y1 = SeqAlignStart (tmp, 0);
     x2 = SeqAlignStop (newtmp, 1);
     y2 = SeqAlignStart (tmp, 1);
     st1= SeqAlignStrand (newtmp, 0);
     st2= SeqAlignStrand (newtmp, 1);

     if (x1 + 1 == y1) {
        if (y2 == x2 + 1)
        {
           newtmp = SeqAlignMerge (newtmp, tmp, TRUE);
           tmp = tmp->next;
        }
        else if (x2 >= y2) {
           SeqAlignTrunc2 (newtmp, 0, -(abs(x2-y2+1)));
        }
        else {
           if(st1!=Seq_strand_minus) y1 -= 1; else y1+=1;
           if(st2!=Seq_strand_minus) y2 -= 1; else y2+=1;
           newtmp = SeqAlignEndExtend (newtmp, -1, -1, y1, y2, -1, -1, x1, x2, st1, st2);
           newtmp = SeqAlignMerge (newtmp, tmp, TRUE);
           tmp = tmp->next;
        }
     }
     else if (x1 >= y1) {
        SeqAlignTrunc2 (newtmp, +(abs(x1-y1+1)), 0);
     }
     else
     {
        if ((st2!=Seq_strand_minus && y2 == x2 + 1) || (st2==Seq_strand_minus && x2 == y2+1))
        {
           if (search_intron) 
           {
              str =load_seq_data(sip1, x1-5, y1+1, FALSE, &len);
              offset = getSplicePos (str);
              if (offset>= -1 && offset<len-1) 
              {
                 offset -= 6;
                 if ((offset>=-6 && offset<0) || (offset>0)) 
                 {
                    SeqAlignTrunc2 (newtmp, 0, abs(offset));
                    SeqAlignTrunc2 (tmp, abs(offset), 0);
                 }
                 x1 = SeqAlignStop (newtmp, 0);
                 y1 = SeqAlignStart (tmp, 0);
                 x2 = SeqAlignStop (newtmp, 1);
                 y2 = SeqAlignStart (tmp, 1);
                 if(st1!=Seq_strand_minus) y1 -= 1; else y1+=1;
                 if(st2!=Seq_strand_minus) y2 -= 1; else y2+=1;
                 newtmp=SeqAlignEndExtend (newtmp,-1,-1,y1, y2, -1, -1, x1, x2, st1, st2);
                 newtmp = SeqAlignMerge (newtmp, tmp, TRUE);
                 tmp = tmp->next;
              }
              MemFree (str);
           } 
           else  {
              if(st1!=Seq_strand_minus) y1 -= 1; else y1+=1; 
              if(st2!=Seq_strand_minus) y2 -= 1; else y2+=1; 
              newtmp=SeqAlignEndExtend (newtmp, -1, -1, y1, y2, -1, -1, x1, x2, st1, st2);
              newtmp = SeqAlignMerge (newtmp, tmp, TRUE);
              tmp = tmp->next;
           }
        }
        else if (st2!=Seq_strand_minus && x2 >= y2) {
            SeqAlignTrunc2 (newtmp, 0, -(abs(x2-y2+1)));
        }
        else if (st2==Seq_strand_minus && y2 >= x2) {
            SeqAlignTrunc2 (tmp, abs(y2-x2+1), 0);
        }
        else {
           slp1 = SeqLocIntNew (x1+1, y1-1, st1, sip1);
           if (st2!=Seq_strand_minus)
              slp2 = SeqLocIntNew (x2+1, y2-1, st2, sip2);
           else 
              slp2 = SeqLocIntNew (y2+1, x2-1, st2, sip2);
           newsalp = BandAlignTwoSeqLocs (slp1, slp2, msp);
           if (newsalp != NULL) 
           {
              newtmp = SeqAlignMerge (newtmp, newsalp, TRUE);
              newtmp = SeqAlignMerge (newtmp, tmp, TRUE);
           } 
           tmp = tmp->next;     
        } 
     }
  }
  return newsalphead;
}

/*************************************/
static Boolean possible_child (SeqAlignPtr salp1, SeqAlignPtr salp2, Uint1 choice)
{
  Int4         start, stop, start2, stop2,
               starti, stopi, 
               starti2, stopi2;
  Boolean      goOn,
               st1, st2;

  starti= SeqAlignStart (salp1, 0);
  stopi = SeqAlignStop (salp1, 0);
  starti2= SeqAlignStart (salp1, 1);
  stopi2 = SeqAlignStop (salp1, 1);
  st1 = (Boolean) !(SeqAlignStrand(salp1,0) == Seq_strand_minus);
  st2 = (Boolean) !(SeqAlignStrand(salp1,1) == Seq_strand_minus);

  start = SeqAlignStart (salp2, 0);
  stop = SeqAlignStop (salp2, 0);
  start2= SeqAlignStart (salp2, 1);
  stop2 = SeqAlignStop (salp2, 1);
  if (choice == 1)
     goOn = (Boolean) (precede(stopi, start, st1) && precede(stopi2, start2, st2));
  else if (choice == 2)
     goOn = (Boolean) (precede(stopi, start, st1) && precede(starti2, start2, st2) && precede(stopi2, stop2, st2));
  else if (choice == 3)
     goOn = (Boolean) (precede(stopi2, start2, st2) && precede(starti, start, st1) && precede(stopi, stop, st1));
  else if (choice == 4)
     goOn = (Boolean) (precede(starti, start, st1) && precede(stopi, stop, st1) && precede(starti2, start2, st2) && precede(stopi2, stop2, st2));
  else if (choice == 5)
     goOn = (Boolean) (precede(starti, start, st1));
  else
     goOn = TRUE;
  return goOn;
}

static NodeHitPtr CreateGraph (SeqAlignPtr salp, Uint1 choice)
{
  NodeHitPtr    headnode, 
                node1, node2,
                newnode,
                child;
  SeqAlignPtr   tmp1, tmp2;
  Nlm_FloatHi   bit_score;
  Nlm_FloatHi   evalue;
  Int4          score, number;
  Int2          j;
  
  headnode = (NodeHitPtr) MemNew (sizeof(NodeHit));
  headnode->index = 1;
  headnode->salp = salp;
  headnode->child = NULL;
  headnode->next = NULL;
  node1 = headnode;
  for (tmp1=salp->next, j=2; tmp1!=NULL; tmp1=tmp1->next, j++) 
  {
     newnode = (NodeHitPtr) MemNew (sizeof(NodeHit));
     newnode->index = j;
     newnode->open = TRUE;
     newnode->salp = tmp1;
     newnode->child = NULL;
     newnode->next = NULL;
     GetScoreAndEvalue (tmp1, &score, &bit_score, &evalue, &number);
     newnode->evalue = evalue;
     newnode->score = score;
     newnode->bit_score = bit_score;
     node1->next = newnode;
     node1 = node1->next;
  }
  node1 = headnode;
  for (tmp1=salp; tmp1!=NULL; tmp1=tmp1->next) 
  {
     child = NULL;
     node2 = headnode;
     for (tmp2=salp; tmp2!=NULL; tmp2=tmp2->next) 
     {
        if (possible_child (tmp1, tmp2, choice)) {
           newnode = (NodeHitPtr) MemNew (sizeof(NodeHit));
           newnode->index = -1;
           newnode->open = TRUE;
           newnode->salp = NULL;
           newnode->child = node2;
           newnode->next = NULL;
           if (node1->child == NULL) {
              node1->child = newnode;
              child = node1->child;
           } else {
              child->next = newnode;
              child = child->next;
           }
        }
        node2 = node2->next;
     }
     node1 = node1->next;
  }
  return headnode;
}

static NodeHitPtr SimplifyGraph (NodeHitPtr headnode)
{
  NodeHitPtr node1, node2, node3, node4, node5;
  Int2       gdchild;

  for (node1 = headnode; node1!=NULL; node1 = node1->next)
  {
     for (node2=node1->child; node2!=NULL; node2 = node2->next)
     {
        node3=node2->child;
        if (node3) {
           for (node4=node3->child; node4!=NULL; node4=node4->next)
           {
              gdchild = node4->child->index;
              for (node5=node1->child; node5!=NULL; node5 = node5->next)
              {
                 if (node5->child->index == gdchild)
                 {
                    node5->open = FALSE;
                    break;
                 }
              }
           }
        }
     }
  }
  return headnode;
}


static SeqAlignPtr link_solution (ValNodePtr vnp, NodeHitPtr head, Int2 total)
{
  NodeHitPtr  nodetmp;
  SeqAlignPtr headsalp = NULL,
              tmpsalp,
              newsalp, salp;
  Int2Ptr     tab;
  Int2        j;
  Boolean     first = TRUE;

  if (vnp==NULL)
     return NULL;
  tab = (Int2Ptr) vnp->data.ptrvalue;
  if (tab==NULL)
     return NULL;
  for (j=0; j<total; j++) 
  {
     if (tab[j] > -1) 
     {
        for(nodetmp=head; nodetmp!=NULL; nodetmp=nodetmp->next) {
           if (nodetmp->index == tab[j])
              break;
        }
        if (nodetmp!=NULL && nodetmp->index == tab[j]) {
           salp=nodetmp->salp;
           if (salp!=NULL) {
              newsalp = SeqAlignDup (salp);
              if (headsalp==NULL) {
                 headsalp = newsalp;
                 tmpsalp = headsalp;
              } else {
                 tmpsalp->next = newsalp;
              }
              tmpsalp = newsalp;
           }
        }
     }
     else break;
  }
  return headsalp;
}

static ValNodePtr find_maxsolution (ValNodePtr vnp, NodeHitPtr head, Int2 total, float *maxscore, float *minintron, Int4 *alignlens)
{
  ValNodePtr  tmp;
  NodeHitPtr  nodetmp;
  SeqAlignPtr salp;
  Int2Ptr     tab;
  float       sum;
  Int4        start, start1, stop;
  Int4        alignlen;
  Int4        bestlen = 0;
  float       bestscore = -100.00;
  float       intronlg;
  float       bestintron = FLT_MAX;
  Int2        j;
  Boolean     first=TRUE;

  for (tmp=vnp; tmp!=NULL; tmp=tmp->next)
  {
     tab = (Int2Ptr) tmp->data.ptrvalue;
     sum = 0;
     intronlg = (float)0.00;
     alignlen = 0;
     for (j=0; j<total; j++) 
     {
      if (tab[j] > -1) 
      {
         for(nodetmp=head; nodetmp!=NULL; nodetmp=nodetmp->next) {
            if (nodetmp->index == tab[j])
               break;
         }
          if (nodetmp!=NULL && nodetmp->index == tab[j]) {
            salp=nodetmp->salp;
            if (salp!=NULL) {
               sum += nodetmp->score;
               start = SeqAlignStart(salp,0);
               if (first) {
                  first = FALSE;
                  start1 = start;
               }
               stop = SeqAlignStop(salp,0);
               alignlen += abs(stop - start);
/***
WriteLog ("%ld..%ld %ld %ld ", start, stop, alignlen, (long)sum);
***/
            }
         }
      }
      else {
         intronlg = (float)(abs(stop - start1) - alignlen)/(float)(j-1);
         if (sum > bestscore) {
            bestscore = sum;
         }
         if (intronlg < bestintron) {
            bestintron = intronlg;
         }
         if (alignlen > bestlen) {
            bestlen = alignlen;
         }
/***
WriteLog ("= %ld > %ld, %f > %f, %ld > %ld\n",(long)sum, (long)bestscore, intronlg, bestintron, alignlen, bestlen);
***/
         break;
      }
     }
  }
  *maxscore = bestscore;
  *minintron = bestintron;
  *alignlens = bestlen;
  return vnp;
}

static ValNodePtr get_solutions (ValNodePtr vnp, NodeHitPtr head, Int2 total, Int4 totalens)
{
  ValNodePtr  tmp, 
              bestvnp;
  NodeHitPtr  nodetmp;
  SeqAlignPtr salp;
  Int2Ptr     tab;
  float       sum;
  Int4        start, start1, stop;
  float       maxscore;
  Int4        maxalignlens, alignlens;
  Int2        index, 
              j;
  float       intronlg;
  float       minintron;
  float       bestratio=0.00;
  Int4        bestratio1=0;
  float       x, y, z;
  Boolean     first=TRUE;

  find_maxsolution (vnp, head, total, &maxscore, &minintron, &maxalignlens);
  bestvnp = NULL;
  index = 1;
  for (tmp=vnp; tmp!=NULL; tmp=tmp->next)
  {
    if(tmp->choice==0) 
    {
     tab = (Int2Ptr) tmp->data.ptrvalue;
     sum = 0;
     intronlg = (float)0.00;
     alignlens = 0;
     first = TRUE;
     for (j=0; j<total; j++) 
     {
      if (tab[j] > -1) 
      {
         for(nodetmp=head; nodetmp!=NULL; nodetmp=nodetmp->next) {
            if (nodetmp->index == tab[j])
               break;
         }
         if (nodetmp!=NULL && nodetmp->index == tab[j]) {
            salp=nodetmp->salp;
            if (salp!=NULL) {
               sum += nodetmp->score;
               start = SeqAlignStart(salp,0);
               if (first) {
                  first = FALSE;
                  start1 = start;
               }
               stop = SeqAlignStop(salp,0);
               alignlens += abs(stop - start);
            }
         }
      }
      else {
         intronlg = (float)(abs(stop - start1) - alignlens)/(float)(j-1);
         x = (float)sum / (float)maxscore;
         y = (float)intronlg / (float)minintron; 
         z = (float)alignlens / (float)maxalignlens;
         if ((Int4)(1000.0*x*z) > bestratio1 && (float)(x*z/y) > (float)bestratio) {
/****
WriteLog ("FFF %ld %ld =%f / %f %d = %f/  %d %d  %f    %f   %f %ld  ++ %d %d %d %d  %f  %f\n", (long)sum, (long)maxscore, x, intronlg, j-1, y, alignlens, maxalignlens, z, (float)((x*z)/y), (float)bestratio, (long)bestratio1, abs(stop - start1), alignlens, start1, stop, (float)(abs(stop - start1) - alignlens), (float)minintron );
*****/
            bestratio1 = (Int4) 1000*(Int4)(x*z);
            bestratio = (float)(x*z/y);
            bestvnp = tmp;
         }
         index++;
         break;
      }
     }
    }
  }
  if (bestvnp!=NULL)
     bestvnp->choice = 5;
  return bestvnp;
}

static ValNodePtr traverseGraph (NodeHitPtr head, NodeHitPtr node, Int2 total, Int2 level, Int2Ptr tab, ValNodePtr vnp, Boolean * change)	
{
  NodeHitPtr  child,
              nhtmp;
  Int2Ptr     tab2;
  Int2        j;
 
  child = node->child;
  tab[level] = node->index;
  *change = TRUE;
 
  while (child!=NULL) 
  {
     nhtmp = child->child;
     if (child->open && nhtmp!=NULL) {
        vnp = traverseGraph(head, nhtmp, total, level+1, tab, vnp, change);
     }
     child = child->next;
  }
  if (level > 0 && *change) 
  {
     tab2 = (Int2Ptr)MemNew ((size_t)((total+2)*sizeof(Int2)));
     for (j=0; j<=level; j++) {
        tab2[j] = tab[j];
     }
     for (; j<total; j++)
        tab2 [j] = -1;
     ValNodeAddPointer (&vnp, 0, (Pointer)tab2);
     tab[level] = -1;
     *change = FALSE;
  }
  return vnp;
}

static SeqAlignPtr seqalign_reverse_sorting (SeqAlignPtr salp)
{
  SeqAlignPtr salptmp, tmp2,next,
              tail,
              salpnew=NULL;

  salptmp = salp;
  while (salptmp->next)
  {
     tmp2=salptmp; 
     next = tmp2->next;
     while (next->next) {
        tmp2=tmp2->next;
        next=next->next;
     }
     if (tmp2->next!=NULL) {
        if (salpnew==NULL) {
           salpnew = tmp2->next;
           tail = salpnew;
        } else  {
           tail->next = tmp2->next;
           tail = tail->next;
        }
        tmp2->next = NULL;
     }
  }
  if (salptmp)
  {
     tail->next = salptmp;
  }
  return salpnew;
}


/*************************************/
static SeqAlignPtr BlastBandAlignTwoSeqLocs (SeqLocPtr slp1, SeqLocPtr slp2, SeqAlignPtr salp, Boolean is_prot, MashPtr msp)
{
  SeqAlignPtr newsalp = NULL;
  Uint1       strand;
  Boolean     delete_mash = FALSE;

  ValNodePtr  vnp;
  NodeHitPtr  headnode, node;
  ValNodePtr  bestsol;
  Int2Ptr     tab;
  Int2        j, total;
  Boolean     change;
 
  if (salp != NULL) {
     if (msp == NULL) {
        msp = MashNew (FALSE);
        delete_mash = TRUE;
     }
     if (msp == NULL)
        return NULL;
     if (number_hits (salp) > 1) 
     {
        newsalp = SortBlastHits (salp, msp->blast_threshold, msp->choice_blastfilter);
     }
     else newsalp = salp;

     if (number_hits (newsalp) > 1) 
     {
        if (newsalp != NULL && (msp->splicing)) 
        {
           headnode = CreateGraph (newsalp, msp->choice_blastfilter);
           headnode = SimplifyGraph (headnode);
           for(total=1, node=headnode; node!=NULL; node=node->next) 
              total++; 
           tab = (Int2Ptr)MemNew ((size_t)((total+2)*sizeof(Int2)));
           for(j=0; j<total; j++)
              tab[j] = -1;
           vnp = NULL;
           for (node=headnode; node!=NULL; node=node->next) {
              vnp = traverseGraph (node, node, total, 0, tab, vnp, &change);
           }
           bestsol = get_solutions (vnp, headnode, total, SeqLocLen(slp2));
           newsalp = link_solution (bestsol, headnode, total);
        }
        if (newsalp != NULL) 
        {
           if ((strand=SeqAlignStrand(newsalp, 0)) == Seq_strand_minus)
           {
              newsalp = SeqAlignListReverseStrand (newsalp);
              newsalp = seqalign_reverse_sorting (newsalp);
           }
           newsalp = align_btwhits (newsalp, SeqLocId(slp1), SeqLocId(slp2), msp);
        }
     }
     if (newsalp != NULL && msp->align_ends) {
        newsalp = align_extrem (newsalp, slp1, slp2, msp);
     }
     if (delete_mash)
        MemFree (msp);
  }
  return newsalp;
}


static SeqAlignPtr AlignAnyway (SeqLocPtr slp1, SeqLocPtr slp2, Boolean is_prot, MashPtr msp, Boolean message)
{
  SeqAlignPtr salp,
              salpblast;
  ValNodePtr  vnp;
  Char        st1[50], st2[50];
 
  SeqIdWrite (SeqLocId(slp1), st1, PRINTID_FASTA_SHORT, 50);
  SeqIdWrite (SeqLocId(slp2), st2, PRINTID_FASTA_SHORT, 50);
  salpblast = BlastTwoSeqLocs (slp1, slp2, is_prot, msp); 
  if (salpblast!=NULL) {
     salp = BlastBandAlignTwoSeqLocs (slp1, slp2, salpblast, is_prot, msp);
     if (salp!=NULL) {
        if (message)
           Message (MSG_OK, "%s - %s : Global alignment based on BLAST local similarity", st1, st2);
        return salp;
     }
     SeqAlignSetFree (salpblast);
  }
  salp = BandAlignTwoSeqLocs (slp1, slp2, msp);
  if (salp!=NULL) {
     if (message)
        Message (MSG_OK, "%s - %s : Global alignment using dynamic programing algorithm", st1, st2);
     return salp;
  }
  vnp = NULL;
  ValNodeAddPointer (&vnp, 0, slp1);
  ValNodeAddPointer (&vnp, 0, slp2);
  salp = SeqLocToFastaSeqAlign (vnp); 
  ValNodeFreeType (&vnp, TypeEmpty);
  if (salp!=NULL) {
     if (message)
        Message (MSG_OK, "Import sequence %s without alignment", st2);
     return salp;
  }
  if (message)
     ErrPostEx (SEV_WARNING, 0, 0, "No alignment");
  return NULL;
}


static SeqAlignPtr AlignmRNA2genomicToSeqAlign (SeqLocPtr slp1, SeqLocPtr slp2, SeqAlignPtr salpblast, MashPtr msp)
{
  SeqAlignPtr salp=NULL;
  Boolean     delete_mash = FALSE;
  
  if (salpblast != NULL) 
  {
     if (msp==NULL) {
        msp = MashNew (FALSE);
        delete_mash = TRUE;
     }
     if (msp!=NULL) {
        msp->splicing = TRUE;
        msp->choice_blastfilter = 2;
        salp = BlastBandAlignTwoSeqLocs (slp1, slp2, salpblast, FALSE, msp);
        if (delete_mash)
           MemFree (msp);
     }
  }
  return salp;
}

/********************************************* 
*** SeqLocListToSeqAlign 
***    aligns the sequences from the SeqLocs list (sqloc_list) 
***    returns a SeqAlign 
***    Alignment methods:
***      1: FASTA (assume that input is FASTA aligned )
***      5: BandAlign (GlobalAlignTwoSeqLocs)
**********************************************/

NLM_EXTERN SeqAlignPtr SeqLocListToSeqAlign (ValNodePtr sqloc_list, Int2 choice, Pointer param)
{
  ValNodePtr       vnp;
  SeqLocPtr        master_slp, 
                   slp=NULL;
  SeqLocPtr        slptmp1,
                   slptmp2;
  SeqAlignPtr      salphead= NULL,
                   salphead2 = NULL,
                   salptmp = NULL,
                   salpnew = NULL,
                   salpblast = NULL,
                   salpna = NULL,
                   salpna2 = NULL;
  SeqAnnotPtr      sapna;
  MashPtr          msp;
  ValNodePtr       framep = NULL;
  Int2             seq_number = 0;
  Uint1            frame;
  Boolean          delete_mash = FALSE;
  Boolean          is_prot = FALSE;

  BioseqPtr        bsp;

  if (sqloc_list == NULL) {
      ErrPostEx(SEV_WARNING, 0, 0, "NULL SeqLocList");
     return NULL;
  }
  if (choice==2 || choice==3 || choice==4 || choice==5)
     return NULL;
  if (choice == 1) 
     return (SeqAlignPtr)SeqLocToFastaSeqAlign (sqloc_list);

  if((master_slp = (SeqLocPtr) sqloc_list->data.ptrvalue)==NULL) {
     ErrPostEx(SEV_WARNING, 0, 0, "Can not read master sequence");
     return NULL;
  }
  seq_number = 1;
  msp = (MashPtr)param;
  if (msp == NULL) {
     bsp = BioseqLockById (SeqLocId(master_slp));
     if (bsp) {
        is_prot = ISA_aa(bsp->mol);
        BioseqUnlock (bsp);
     }
     else
        return NULL;
     msp = MashNew (is_prot);
     delete_mash = TRUE; 
  }
  if (msp == NULL)
     return NULL;

  is_prot = (Boolean)(msp->is_prot || msp->translate_prot);
  if (msp->translate_prot) {
     slptmp1 = master_slp;
     master_slp = TranslateSeqLoc(slptmp1, (Int2) Seq_code_ncbistdaa, &frame);
     ValNodeAddInt (&framep, 1, (Int4)frame);
  }
/*   
  if(msp->is_prot && msp->matrixname==NULL) msp->matrixname="BLOSUM62"; 
*/
  for (vnp = sqloc_list->next; vnp!=NULL; vnp = vnp->next)
  {
     salpnew = NULL;
     slp = (SeqLocPtr) vnp->data.ptrvalue;
     if ( slp != NULL ) 
     {
        if (msp->translate_prot) {
           slptmp2 = slp;
           slp = TranslateSeqLoc(slptmp2, (Int2) Seq_code_ncbistdaa, &frame);
           ValNodeAddInt (&framep, 1, (Int4)frame);
        }
        if (choice == 2) {
           ObjMgrSetHold ();
/*
COMMENT    salpnew = (SeqAlignPtr) SIM2ALN (master_slp, slp, 1); */
           ObjMgrClearHold (); 
        } 
        else if (choice == 3) {
           ObjMgrSetHold ();
/*
COMMENT    salpnew=(SeqAlignPtr)SIM3ALN_choice (master_slp, slp, (Int4) 100, TRUE, TRUE); */
           ObjMgrClearHold (); 
        } 
        else if (choice == 4) {
/*
COMMENT    salpnew = (SeqAlignPtr) CSIM (master_slp, slp, 1, 0.00, NULL); */
        } 
        else if (choice == 5) {
/*
COMMENT    salpnew = SIM4ALN_choice (master_slp, slp, 1000, 8); */
        } 
        else if (choice == 6) {
           salpblast = BlastTwoSeqLocs (master_slp, slp, is_prot, msp);
           if (salpblast!=NULL) {
              salpnew = BandAlignTwoSeqLocs (master_slp, slp, msp);
           }
           SeqAlignSetFree (salpblast);
        } 
        else if (choice == 7) {
           salpnew = BlastTwoSeqLocs (master_slp, slp, is_prot, msp); 
           msp->choice_blastfilter = 0;
           salpnew = SortBlastHits (salpnew, msp->blast_threshold, msp->choice_blastfilter);
        } 
        else if (choice == 10) { 
           salpblast = BlastTwoSeqLocs (master_slp, slp, is_prot, msp); 
           if (salpblast!=NULL) 
           {
              salpnew = BlastBandAlignTwoSeqLocs (master_slp, slp, salpblast, is_prot, msp);
              if (salpnew==NULL)
                 SeqAlignSetFree (salpblast);
           }
        } 
        else if (choice == 9) {
           salpblast = BlastTwoSeqLocs (master_slp, slp, is_prot, msp); 
           if (salpblast!=NULL) {
              salpnew = AlignmRNA2genomicToSeqAlign (master_slp, slp, salpblast, msp);
              if (salpnew==NULL)
                 SeqAlignSetFree (salpblast);
           }
        } 
        else if (choice == 8) {
           salpblast = BlastTwoSeqLocs (master_slp, slp, is_prot, msp); 
           if (salpblast!=NULL) {
              salpnew = AlignmRNA2genomicToSeqAlign (master_slp, slp, salpblast, msp);
              if (salpnew == NULL) 
              {
                 salpnew = BlastBandAlignTwoSeqLocs (master_slp, slp, salpblast, is_prot, msp);
              }
              if (salpnew==NULL)
                 SeqAlignSetFree (salpblast);
           }
        }
        else if (choice == 0)
        {
           if (seq_number<3)
              salpnew = AlignAnyway (master_slp, slp, is_prot, msp, TRUE);
           else
              salpnew = AlignAnyway (master_slp, slp, is_prot, msp, FALSE);
        }
        if (salpnew != NULL) 
        {
           if (msp->translate_prot) {
              master_slp = slptmp1;
              sapna =SeqAnnotForSeqAlign (salpna);
              sapna = aaSeqAnnot_to_dnaSeqAnnotFunc (&sapna, salpnew, sqloc_list, framep);
              salpna = (SeqAlignPtr)sapna->data;
              sapna->data=NULL;
              SeqAnnotFree(sapna);
           }
           if (salphead == NULL) {
              salphead = salpnew;
              salptmp = salpnew;
           }
           else {
              salptmp->next = salpnew;
              salptmp = salptmp->next;
           }
           seq_number++;
        }
     } 
     else {
        ErrPostEx(SEV_WARNING, 0, 0, "Last SeqLoc was NULL");
     }
     
  }
  if (salphead != NULL) {
     if (msp->map_align)
        salphead = SeqAlignMapOnFirstSeq (salphead);
     if (seq_number > 2 && msp->multidim) {
        salphead2 = PairSeqAlign2MultiSeqAlign (salphead);
        if (salphead2!=NULL)
        {
           salphead = SeqAlignSetFree (salphead);
           salphead = salphead2;
        }
     }
  }
  if (salpna != NULL) {
     if (seq_number > 2 && msp->multidim) {
	salpna2 = PairSeqAlign2MultiSeqAlign (salpna);
        if (salpna2!=NULL)
        {
           salpna = SeqAlignSetFree (salpna);
           salpna = salpna2;
        }
     }
     if (salpna !=NULL)
        msp->transalp = salpna;
  }
  if (delete_mash)
     MemFree (msp);
  return salphead;
}


NLM_EXTERN SeqLocPtr AlignmRNA2genomic (BioseqPtr bsp1, BioseqPtr bsp2)
{
  ValNodePtr  vnp = NULL;
  SeqLocPtr   slp = NULL,
              slp1 = NULL,
              slp2 = NULL;
  SeqIdPtr    sip;
  SeqAlignPtr salp,
              salpblast;
  MashPtr     msp = NULL;

  if (bsp1==NULL || bsp2==NULL)
     return NULL;
  if ((Boolean)ISA_aa(bsp1->mol) || (Boolean)ISA_aa(bsp2->mol))
     return NULL;
  sip = SeqIdFindBest (bsp1->id, 0);
  if (sip==NULL)
     return NULL;
  slp1 = SeqLocIntNew (0, bsp1->length - 1, Seq_strand_plus, sip);
  ValNodeAddPointer(&vnp, 0, (Pointer)slp1);
  sip = SeqIdFindBest (bsp2->id, 0);
  if (sip==NULL) 
     return NULL;
  slp2 = SeqLocIntNew (0, bsp2->length - 1, Seq_strand_plus, sip);
  ValNodeAddPointer(&vnp, 0, (Pointer)slp2);
  salpblast = BlastTwoSeqLocs (slp1, slp2, FALSE, msp);
  if (salpblast!=NULL)
  {
     salp = AlignmRNA2genomicToSeqAlign (slp1, slp2, salpblast, msp);
     SeqAlignSetFree (salpblast);
     slp = SeqLocMixFromSeqAlign (salp, NULL);
  }
  return slp;
}

NLM_EXTERN SeqAnnotPtr BlastBandAlignFromBlastSeqAlign (SeqAlignPtr salpblast, Boolean align_ends)
{
  BioseqPtr   bsp;
  SeqAnnotPtr sap, sap2;
  SeqAlignPtr salptmp,
              salpnew,
              salp_next,
              salp_head=NULL,
              salp_cp;
  ValNodePtr  vnp_sameid=NULL,
              vnp;
  SeqLocPtr   master_slp, 
              slp;
  SeqIdPtr    sip,
              siptmp;
  Boolean     found=FALSE,
              is_prot=FALSE;
  MashPtr     msp = NULL;
  Uint1       val;
 
  if (!salpblast)
     return NULL;

  sap = SeqAnnotForSeqAlign (salpblast);

  sap2 = (SeqAnnotPtr) AsnIoMemCopy((Pointer)sap, 
  		(AsnReadFunc)SeqAnnotAsnRead, (AsnWriteFunc)SeqAnnotAsnWrite);

  salp_cp = (SeqAlignPtr)sap2->data;
  sap->data=NULL;
  SeqAnnotFree (sap);
  sap2->data=NULL;
  SeqAnnotFree (sap2);

  sip = SeqIdPtrFromSeqAlign (salp_cp);
  bsp = BioseqLockById (sip);
  if (bsp) {
     is_prot = ISA_aa(bsp->mol);
     master_slp = SeqLocIntNew (0, bsp->length-1, Seq_strand_plus, sip);
     BioseqUnlock (bsp);
  }
  else return NULL;

  ValNodeAddPointer (&vnp_sameid, 0, (Pointer)salp_cp);
  salp_next = salp_cp->next;
  salp_cp->next = NULL;
  salp_cp = salp_next;
  while (salp_cp) {
     sip = SeqIdPtrFromSeqAlign(salp_cp);
     found = FALSE;
     vnp = vnp_sameid;
     while (vnp && !found) 
     {
        salptmp = (SeqAlignPtr)vnp->data.ptrvalue;
        siptmp = SeqIdPtrFromSeqAlign (salptmp);
        if ((val=SeqIdComp(sip->next, siptmp->next)) == SIC_YES)
        {
           while (salptmp->next!=NULL)
              salptmp=salptmp->next;
           salptmp->next = salp_cp;
           salp_next = salp_cp->next;
           salp_cp->next = NULL;
           salp_cp = salp_next;
           found = TRUE;
        }
        vnp=vnp->next;
     }
     if (!found) {
        ValNodeAddPointer (&vnp_sameid, 0, (Pointer)salp_cp);
        salp_next = salp_cp->next;
        salp_cp->next = NULL;
        salp_cp = salp_next;
     }
  }
  if (vnp_sameid) {
     msp = MashNew (is_prot);
     msp->align_ends = align_ends;
     for (vnp=vnp_sameid; vnp!=NULL; vnp=vnp->next) {
        salptmp = (SeqAlignPtr)vnp->data.ptrvalue;
        sip = SeqIdPtrFromSeqAlign (salptmp);
        sip=sip->next;
        bsp = BioseqLockById (sip);
        if (bsp!= NULL)
        {
           slp = SeqLocIntNew (0, bsp->length-1, Seq_strand_plus, sip);
           if (slp != NULL)
           {
              BioseqUnlock (bsp);
              salpnew = BlastBandAlignTwoSeqLocs (master_slp, slp, salptmp, is_prot, msp);
              if (salpnew) {
                 if(salp_head == NULL)
                    salp_head = salpnew;
                 else {
                    for(salptmp=salp_head; salptmp->next!=NULL; salptmp=salptmp->next)
                       continue;
                    salptmp->next=salpnew;
                 }
              }
           }
        }
     }
  }
  if (salp_head==NULL)
     return NULL;
  return SeqAnnotForSeqAlign (salp_head);
}


/*******************************************************
*** SeqEntryAlignToMasterFunc
***    aligns the bioseqs that are present in a SeqEntry (sep)
***    returns a SeqAlign
*** SeqEntryToSeqAlign
***    calls SeqEntryAlignToMaster
***    returns a SeqAnnot
******************************************************/

static SeqAlignPtr SeqEntryAlignToMasterFunc (SeqEntryPtr sep, SeqLocPtr master, Uint1 bsp_mol, Int2
 method)
{
  ValNodePtr         vnp = NULL,
                     vnp2 = NULL;
  SeqAlignPtr        salp = NULL;
  Int2               nb;

  vnp = SeqEntryToSeqLoc (sep, &nb, bsp_mol);
  if (vnp != NULL) {
     if (master != NULL)
     {
        ValNodeAddPointer (&vnp2, 0, master);
        vnp2->next = vnp;
        vnp = vnp2;
     }
     salp = SeqLocListToSeqAlign (vnp, method, NULL);
  }
  return salp;
}

NLM_EXTERN SeqAnnotPtr LIBCALL SeqEntryToSeqAlign (SeqEntryPtr sep, Uint1 bsp_mol)
{
  SeqAlignPtr salp;
  SeqAnnotPtr sap;

  salp = SeqEntryAlignToMasterFunc (sep, NULL, bsp_mol, PRG_ANYALIGN);
  sap = SeqAnnotForSeqAlign (salp);
/* this doesn't seem to produce a valid alignment and nobody knows what
it's for anyway, so we'll comment it out and see who complains -- 8/8/01 SW
  if (ISA_aa(bsp_mol)) {
     sap = aaSeqAnnot_to_dnaSeqAnnotFunc (&sap, salp, NULL, NULL);
  }
*/
  return sap;
}
