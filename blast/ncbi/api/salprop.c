/*   salprop.c
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
* File Name:  salprop.c
*
* Author:  Colombe Chappey
*
* Version Creation Date:   8/20/99
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
#include <salprop.h>
#include <salutil.h>
#include <salstruc.h>
#include <salsap.h>
#include <sqnutils.h>
#include <satutil.h>
#include <explore.h>

/**********************************************************************
***   PropagateFeatureProc
***      build features taking one selected feature as template
***
*** 
LIST starts with 15 
***********************************************************************/

#define ADD_TITLE 1
#define ADD_RRNA  2
#define ADD_CDS   3
#define ADD_PROT  4
#define ADD_IMP   5
#define ADD_PUB   6
#define ADD_GENE  7
#define ADD_REGION 8
#define ADD_BOND  9
#define ADD_SITE  10

typedef struct applyformdata {
  Int2           type;
  Boolean        noLeft;
  Boolean        noRight;
  CharPtr        geneName;
  ValNodePtr     protName;
  CharPtr        featcomment;
  CharPtr        defline;
  Uint2          input_entityID;
} ApplyFormData, PNTR ApplyFormPtr;

typedef struct partials {
  Boolean p3, p5;
} ParTials, PNTR ParTialsPtr;

typedef struct ccid {
  SeqIdPtr    sip;
  SeqEntryPtr sep;
  BioseqPtr   bsp;
  Boolean     found;
} CcId, PNTR CcIdPtr;


static void FindSeqEntryForSeqIdCallback (SeqEntryPtr sep, Pointer mydata,
                                          Int4 index, Int2 indent)
{
  BioseqPtr          bsp;
  SeqIdPtr           sip;
  CcIdPtr            cip;
  Uint1              val;

  if (sep != NULL && sep->data.ptrvalue && mydata != NULL) {
     cip = (CcIdPtr)mydata;
     if (!(cip->found) && IS_Bioseq(sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp!=NULL) {
           for (sip = bsp->id; sip!=NULL; sip=sip->next) 
           {
              if ((val=SeqIdComp(cip->sip, sip)) == SIC_YES) {
                 cip->sep = sep;
                 cip->bsp = bsp;
                 cip->found = TRUE;
                 break;
              }
           }
        }
     }
  }
}

static Boolean is_id_unique (Uint2 entityID, SeqIdPtr sip)
{
  SeqEntryPtr sep;
  CcId        ci;
  Boolean     ret;

  sep = GetTopSeqEntryForEntityID (entityID);
  ci.sip = SeqIdDup (sip);
  ci.sep = NULL;
  ci.bsp=NULL;
  ci.found=FALSE;
  SeqEntryExplore(sep,(Pointer)&ci, FindSeqEntryForSeqIdCallback);
  SeqIdFree (ci.sip);
  ci.sep=NULL;
  ci.bsp=NULL;
  return (!(ci.found));
}

static Boolean stringhasnotext (CharPtr str)

{
  Char  ch;

  if (str != NULL) {
    ch = *str;
    while (ch != '\0') {
      if (ch > ' ' && ch <= '~') {
        return FALSE;
      }
      str++;
      ch = *str;
    }
  }
  return TRUE;

}

/**********************************************************
***  GetFeatureForEditor
***
***********************************************************/
static Boolean is_newfeat (ValNodePtr feathead, Uint2 eID, Uint2 subtype, SeqLocPtr slp)
{
  SelEdStructPtr   psp;
  ValNodePtr       vnp = NULL;

  if (feathead == NULL)  {
         return TRUE;
  }
  for (vnp = feathead; vnp != NULL; vnp = vnp->next)
  {
        psp = (SelEdStructPtr) vnp->data.ptrvalue;
        if (psp->entityID == eID && psp->itemsubtype == vnp->choice)
        {
           if (SeqLocCompare(slp, (SeqLocPtr) psp->region) == SLC_A_EQ_B) {
              return FALSE;
           }
        }
  }
  return TRUE;
}


static ValNodePtr get_names_from_prot (ValNodePtr sfp_product)
{
  BioseqPtr bsp;
  SeqAnnotPtr annot;
  SeqFeatPtr sfpp;
  ProtRefPtr prp;
  ValNodePtr vnp,
           new_vnp=NULL;
  CharPtr    name;

  bsp=BioseqLockById (SeqLocId(sfp_product));
  if (bsp){
     if (bsp->annot){
        for (annot=bsp->annot;annot!=NULL;annot=annot->next){
           if (annot->type==1){
              sfpp=(SeqFeatPtr)annot->data;
              if (sfpp->data.choice==SEQFEAT_PROT) {
                 prp=(ProtRefPtr)sfpp->data.value.ptrvalue;
                 for (vnp = prp->name;vnp!=NULL;vnp=vnp->next){
                    name = StringSave((CharPtr)vnp->data.ptrvalue); 
                    ValNodeAddPointer(&new_vnp, 0, name);
                 }
              }
           }
        } 
    }
    BioseqUnlock(bsp);
  } 
  return new_vnp;
}


static Int4 PosInSeqLoc (Int4 position, SeqLocPtr location)
{
  SeqLocPtr slp;
  Int4      start, 
            lens,
            sum=0;
  if (location ==NULL)
     return -1;
  start = SeqLocStart(location);
  if (location->choice == 4) 
  {
     position += start;
     return position;
  }
  if (location->choice == 5) {
     slp=(SeqLocPtr)location->data.ptrvalue;
     for (; slp!=NULL; slp=slp->next) 
     {
        if (slp->choice == 4) {
           lens = SeqLocLen(slp);
           if (position <= (sum+lens)) {
              start = SeqLocStart(slp);
              position = start + (position - sum);   
              return position;
           }
           sum += lens;
        }
     }
  }
/****
  while ((slp=SeqLocFindNext(location, slp))!=NULL)
  {
     lens = SeqLocLen(slp);
     if (position <= (sum+lens)) {
        start = SeqLocStart(slp);
        position = start + (position - sum) -1;   
        return position;
     }
     sum += lens;
  }
***/
  return -1;
} 


static Boolean FindSqFeatItem (GatherContextPtr gcp)
{
  SeqFeatPtr PNTR sfpp;

  sfpp = (SeqFeatPtr PNTR) gcp->userdata;
  if (sfpp != NULL && gcp->thistype == OBJ_SEQFEAT) {
    *sfpp = (SeqFeatPtr) gcp->thisitem;
  }
  return TRUE;
}

/************************************************************
***
*** CopySeqLocFromSeqAlign
***  map source_loc to the target sequence defined by target_sip.
***  Calls map_one_location, that maps one Seq-loc
***    with gap_choice == TAKE_GAP_CHOICE
***    because one SEQLOCINT seqloc has to be stoppped at gaps
***    when mapped, and returns SEQLOCMIX
*** 
#define DEFAULT_GAP_CHOICE 0    * will split seqloc 
                                * if gaps length > MAX_GAP_LEN_BY_DEFAULT *
#define IGNORE_GAP_CHOICE 1     * extends over gaps *
#define TAKE_GAP_CHOICE 2       * split at gaps *
#define MAX_GAP_LEN_BY_DEFAULT 10       *the maximum allowd length of gap*
***
*************************************************************/
static SeqLocPtr CopySeqLocFromSeqAlign (SeqFeatPtr source_sfp, SeqIdPtr target_id, SeqIdPtr s_id, SeqAlignPtr align, Uint1 gap_choice, Uint1 *frame)
{
   GatherRange gr;
   SeqLocPtr slp, source_slp, c_slp;
   SeqLocPtr process_slp, new_slp,
             new_location;
   Boolean check_gap;
   
   SeqIdPtr sip, source_id;
   Boolean map_to_source;
   BioseqPtr source_bsp;
   CdRegionPtr crp;
   Int4 cds_len;
   Int4 aa_start, aa_stop;
   Int4 a_start, a_stop;
   Int4 r_start, r_stop, e_start, e_stop;
   Int4 frame_offset;
   Int4 offset;
   Uint1     new_frame;
   Boolean   stop_here;
   Boolean   had_first_seg;
   Boolean   p5, p3;

   if(source_sfp == NULL || target_id == NULL || align == NULL)
      return NULL;

   if(source_sfp->location == NULL)
      return NULL;

   sip = SeqLocId(source_sfp->location);
   if(sip == NULL)
      return NULL;

   if(s_id == NULL)
      source_id = sip;
   else
      source_id = s_id;

   source_bsp = BioseqLockById(source_id);

   if(source_bsp == NULL)
   {
      return NULL;
   }

   map_to_source = FALSE;   /*segmented sequence may have locations on different segment sequences*/

   if(!BioseqMatch(source_bsp, sip))
   {
      ErrPostEx (SEV_ERROR, 0, 0, "Source Bioseq does not match the Seq-id of the Source Seq-feat");
      BioseqUnlock(source_bsp);
      return NULL;
   }
   else
      map_to_source = TRUE;

   source_slp = NULL;
   if(!map_to_source)
      source_slp = SeqLocIntNew(0, source_bsp->length-1, Seq_strand_plus, sip);
   if(source_sfp->data.choice == 3)
      crp = (CdRegionPtr)source_sfp->data.value.ptrvalue;
   else
      crp = NULL;

   if(gap_choice == DEFAULT_GAP_CHOICE)
   {
      if(source_sfp->data.choice == SEQFEAT_CDREGION || source_sfp->data.choice == SEQFEAT_RNA)
         check_gap = TRUE;
      else
         check_gap = FALSE;
   }
   else
      check_gap = (Boolean) (gap_choice == TAKE_GAP_CHOICE);

   slp = NULL;
   process_slp = NULL;
   cds_len = 0;
   new_frame = 0;
   a_start = -1;
   a_stop = -1;
   e_start = -1;
   e_stop = -1;
   stop_here = FALSE;
   had_first_seg = FALSE;
   while((slp = SeqLocFindNext(source_sfp->location, slp)) != NULL && !stop_here)
   {
      c_slp = NULL;
      new_slp = NULL;

      if(map_to_source == FALSE)
      {   /* map the location to the coordinates on the source */
         if(SeqLocOffset(source_slp, slp, &gr, 0))
         {
            if(gr.l_trunc == FALSE && gr.r_trunc == FALSE)
               c_slp = SeqLocIntNew(gr.left, gr.right, gr.strand, source_id);
         }
      }
      else
         c_slp = slp;

      if(c_slp != NULL)
      {

         new_slp = map_one_location(c_slp, align, target_id, gap_choice, &r_start, &r_stop);
         if(new_slp != NULL)
         {
            ValNodeLink(&process_slp, new_slp);
            if(e_start == -1)
               e_start = r_start;
            else
               e_start = MIN(e_start, r_start);
            if(e_stop == -1)
               e_stop = r_stop;
            else
               e_stop = MAX(e_stop, r_stop);

               if(crp != NULL)   /*for coding region features*/
               {
               /*calculate the frame for the first exon*/
               if(!had_first_seg)
               { 
                  new_frame = (Int4)crp->frame;

/* JINGUI's CALCUL 
THAT I do not understand ******************************
{{
Int4 c_frame_offset;
                  if(crp->frame > 1)
                     frame_offset = (Int4)crp->frame -1L;
                  else
                     frame_offset = 0L;

                  c_frame_offset = frame_offset;
                  if(cds_len > 0)
                  {
                     c_frame_offset = (cds_len - frame_offset)%3;
                     if(c_frame_offset > 0)   
                        c_frame_offset = 3 - c_frame_offset;
                  }
                  if(SeqLocStrand(c_slp) == Seq_strand_minus)
                     frame_offset = (SeqLocStop(c_slp) - r_stop)%3;
                  else
                     frame_offset = (r_start - SeqLocStart(c_slp))%3;
                  if(frame_offset > 0)
                     c_frame_offset = (frame_offset + c_frame_offset)%3;
                  if(c_frame_offset > 1)
                     new_frame = (Uint1) (c_frame_offset + 1);
                  else
                     new_frame = 1;
}}
END of calcul of new_frame **/
               }

               /*calculate the position in the amino acid*/
               if(SeqLocStrand(c_slp) == Seq_strand_minus)
                  offset = SeqLocStop(c_slp) - r_stop;
               else
                  offset = r_start - SeqLocStart(c_slp);
                  
               if(crp->frame > 1)
                  frame_offset = (Int4)crp->frame -1L;
               else
                  frame_offset = 0L;
               aa_start = (cds_len + offset - frame_offset)/3;
               if(aa_start < 0)
                  aa_start = 0 ;

               if(SeqLocStrand(c_slp) == Seq_strand_minus)
                  offset = SeqLocStop(c_slp) - r_start;
               else
                  offset = r_stop- SeqLocStart(c_slp);
               aa_stop= (cds_len + offset - frame_offset)/3;
               if(aa_stop < 0 )
                  aa_stop = 0;
               if(a_start == -1)
                  a_start = aa_start;
               else
                  a_start = MIN(a_start, aa_start);

               if(a_stop == -1)
                  a_stop = aa_stop;
               else
                  a_stop = MAX(a_stop, aa_stop);

              }/*finishing processing the CDS region*/
            had_first_seg = TRUE;

         }
         else
            stop_here = TRUE;
      }
      else
         stop_here = TRUE;
      cds_len += SeqLocLen(c_slp);
      if(c_slp != NULL && !map_to_source)
         SeqLocFree(c_slp);
   }
  if(process_slp == NULL)
  {
      if(source_slp != NULL)
         SeqLocFree(source_slp);
      BioseqUnlock(source_bsp);
      return NULL;
  }
  new_location = SeqLocPackage(process_slp);
  CheckSeqLocForPartial (source_sfp->location, &p5, &p3);
  SetSeqLocPartial (new_location, p5, p3);
  BioseqUnlock(source_bsp);
  *frame = new_frame;
  return new_location;
}





static Boolean CheckPartialInterval (SeqFeatPtr sfp, Int4 nbsp_length, Int4 stoptransl, 
                              Uint1 frame, Boolean include_stop, Boolean exon,
                              ParTialsPtr ppp)
{
  ByteStorePtr  bs;
  CharPtr       prot,
                prot2;
  SeqLocPtr     sfp_location = sfp->location,
                new_sfp_location = sfp_location;
  SeqIdPtr      sfp_sip;
  Int4          strlens,
                strlens2,
                start,
                stop,
                j;
  Boolean       p5=FALSE, 
                p3=FALSE, 
                stop_codon_found = FALSE,
                until_end=TRUE,
                changed;
            
  bs = ProteinFromCdRegion (sfp, include_stop);
  if (bs==NULL)
     return FALSE;
  prot = (CharPtr)BSMerge (bs, NULL);
  if (prot)
  {
     strlens = StringLen (prot);
     p5 = (Boolean) (prot[(Int4)0] != 'M');
     stop_codon_found = FALSE;
     for (j=0; j<strlens; j++) {
        if (prot[j] == '*') {
           stop_codon_found=TRUE;
           break;
        }
     }
     until_end=TRUE; 
     if (!stop_codon_found) 
     {
        stop = (Int4)(nbsp_length-1);
        until_end = (Boolean) (stop==SeqLocStop(sfp_location));
     }
     if (stop_codon_found && j==strlens-1)    /*------- same length (make sure !p3) */
     {
        p3=FALSE;
/*
        prot [(Int4)(strlens-1)] = '\0';
        bs = BSFree (bs);
        bs = BSNew (StringLen (prot)+5);
        BSWrite (bs, (VoidPtr) prot, (Int4) StringLen (prot));
*/
     }
     else if (stop_codon_found && j<strlens-1)  /*---- shorter  */
     {
        if (stoptransl & PROPAG_SETSTOP) 
        {
           prot [(Int4)(j)] = '\0';
           strlens2 = StringLen (prot);
           if (strlens2>1) 
           {
              bs = BSFree (bs);
              bs = BSNew (strlens2 +5);
              BSWrite (bs, (VoidPtr) prot, (Int4) strlens2);
              start = PosInSeqLoc (3*strlens2+2, sfp_location) +1;
              if (frame > 1)
              {
                 start += (frame-1);
              }
              stop = SeqLocStop (sfp_location);
              sfp_sip = SeqLocId (sfp_location);
              sfp_location = SeqLocDelete (sfp_location, sfp_sip, start, stop, FALSE, &changed);
              p3=FALSE;
           }
           else {
              ErrPostEx (SEV_ERROR, 0, 0, "New feature too short");
           }
        }
        else {
           p3=TRUE;
/*
           prot [(Int4)(strlens-1)] = '\0';
           bs = BSFree (bs);
           bs = BSNew (StringLen (prot)+5);
           BSWrite (bs, (VoidPtr) prot, (Int4) StringLen (prot));
*/
        }
     }
     else if (!stop_codon_found && until_end)
     {
           p3=TRUE;
/*
           prot [(Int4)(strlens-1)] = '\0';
           bs = BSFree (bs);
           bs = BSNew (StringLen (prot)+5);
           BSWrite (bs, (VoidPtr) prot, (Int4) StringLen (prot));
*/
     }
     else if (!stop_codon_found && !until_end && exon)
     {
           p3=TRUE;
/*
           prot [(Int4)(strlens-1)] = '\0';
           bs = BSFree (bs);
           bs = BSNew (StringLen (prot)+5);
           BSWrite (bs, (VoidPtr) prot, (Int4) StringLen (prot));
*/
     }
     else if (!stop_codon_found && !until_end && !(stoptransl & PROPAG_GETSTOP))
     {
           p3=TRUE;
/*
           prot [(Int4)(strlens-1)] = '\0';
           bs = BSFree (bs);
           bs = BSNew (StringLen (prot)+5);
           BSWrite (bs, (VoidPtr) prot, (Int4) StringLen (prot));
*/
     }
     else                                 /*------- longer */
     { 
        j = stop - SeqLocStop(sfp_location);
        if (j>1) 
        {
           sfp_sip = SeqLocId (sfp_location);
           stop = SeqLocStop (sfp_location);
           new_sfp_location = (SeqLocPtr)AsnIoMemCopy ((Pointer) sfp_location, (AsnReadFunc) SeqLocAsnRead, (AsnWriteFunc) SeqLocAsnWrite);
           new_sfp_location = SeqLocInsert (new_sfp_location, sfp_sip, stop, j, FALSE, NULL); 
           sfp_location = new_sfp_location;
           ValNodeFree (sfp->location);
           sfp->location = sfp_location;
           bs = BSFree (bs);
           bs = ProteinFromCdRegion (sfp, FALSE);
           if (bs)
           {
              j = 3*BSLen(bs)+2;
              if (j <= SeqLocLen (sfp_location))   /*------- longer with * (!p3) */
              {
                 start = PosInSeqLoc (j, sfp_location) +1;
                 if (frame > 1)
                 {
                    start += (frame-1);
                 }
                 stop = SeqLocStop (sfp_location);
                 sfp_location = SeqLocDelete (sfp_location, sfp_sip, start, stop, FALSE, &changed);
                 p3 = FALSE;
              }
              else {                         /*------- longer without * (p3) */
                 p3 = TRUE;
              }
              prot2 = (CharPtr)BSMerge (bs, NULL);
              p5 = (Boolean) (prot2[(Int4)0] != 'M');
           }
           else
              ErrPostEx (SEV_ERROR, 0, 0, "Propagate error [22]");
        }
        else {
           ErrPostEx (SEV_ERROR, 0, 0, "Propagate error [33]");
        }             
     }
     sfp->location = sfp_location;
  }
  ppp->p3 = p3;
  ppp->p5 = p5;
  return 0;
}            


static Boolean ApplyBioFeatToSeqEntry (SeqEntryPtr sep, ApplyFormPtr afp, SeqLocPtr slp, Uint1 frame, Int4 stoptransl, SeqFeatPtr sfp_source, Boolean exon, SeqIdPtr prot_id)

{
  CdRegionPtr   crp;
  GeneRefPtr    grp;
  SeqEntryPtr   nsep;
  ValNodePtr    sdp;
  SeqFeatPtr    sfp;

  Char          string_id[128];
  Boolean       include_stop = TRUE,
                sfp_p3, sfp_p5;
  ParTials      ppp;
 
  if (sep == NULL || afp == NULL) 
     return FALSE;
  nsep = FindNucSeqEntry (sep);
  if (nsep == NULL) 
     return FALSE;
  string_id[0]='\0';
  if (afp->type == ADD_TITLE) {
    if (! stringhasnotext(afp->defline)) {
      sdp = CreateNewDescriptor (sep, Seq_descr_title);
      if (sdp != NULL) {
        sdp->data.ptrvalue = (Pointer) StringSave (afp->defline);
      }
    }
  } else if (afp->type == ADD_GENE) {  
     sfp = CreateNewFeature (nsep, NULL, SEQFEAT_GENE, sfp_source);
  } else if (afp->type == ADD_CDS) {  
     sfp = CreateNewFeature (nsep, NULL, SEQFEAT_CDREGION, sfp_source);
  } else if (afp->type == ADD_RRNA) { 
     sfp = CreateNewFeature (nsep, NULL, SEQFEAT_RNA, sfp_source);
  } else if (afp->type == ADD_PUB) { 
     sfp = CreateNewFeature (nsep, NULL, SEQFEAT_PUB, sfp_source);
  } else if (afp->type == ADD_IMP) {
     sfp = CreateNewFeature (nsep, NULL, SEQFEAT_IMP, sfp_source);
  } else if (afp->type == ADD_REGION) {
     sfp = CreateNewFeature (nsep, NULL, SEQFEAT_REGION, sfp_source);
  } else if (afp->type == ADD_BOND) {
     sfp = CreateNewFeature (nsep, NULL, SEQFEAT_BOND, sfp_source);
  } else if (afp->type == ADD_SITE) {
     sfp = CreateNewFeature (nsep, NULL, SEQFEAT_SITE, sfp_source);
  }
  if (sfp != NULL)
  {
     sfp->location = SeqLocFree (sfp->location);
     sfp->location = (SeqLocPtr) AsnIoMemCopy ((Pointer) slp, (AsnReadFunc) SeqLocAsnRead, (AsnWriteFunc) SeqLocAsnWrite);
     if (! stringhasnotext(afp->featcomment)) {
        sfp->comment = (CharPtr) StringSave (afp->featcomment);
     }
     SetSeqLocPartial (sfp->location, afp->noLeft, afp->noRight);
     sfp->partial = (afp->noLeft || afp->noRight);

     if (afp->type == ADD_CDS) 
     { 
        if (sfp->product!=NULL) {
           ValNodeFree (sfp->product);
           sfp->product=NULL;
        }
        crp = (CdRegionPtr) (sfp->data.value.ptrvalue);
        if (crp!=NULL) 
        {
           crp->frame = frame;
           CheckPartialInterval (sfp, ((BioseqPtr)nsep->data.ptrvalue)->length, stoptransl, frame, include_stop, exon, &ppp);
           CheckSeqLocForPartial (sfp->location, &sfp_p5, &sfp_p3);
           if (!sfp_p5 && ppp.p5)
               sfp_p5 = ppp.p5;
           afp->noLeft = sfp_p5;
           afp->noRight = ppp.p3;
           SetSeqLocPartial (sfp->location, afp->noLeft, afp->noRight);
           sfp->partial = (afp->noLeft || afp->noRight);
           
           if (prot_id==NULL) {
              prot_id = MakeNewProteinSeqId (sfp->location, NULL);
           }
           SeqIdWrite  (prot_id, string_id , PRINTID_FASTA_LONG, 128);
           AddQualifierToFeature (sfp, "protein_id", string_id);
           if (afp->protName)
              AddQualifierToFeature (sfp, "product", (CharPtr)afp->protName->data.ptrvalue);
        }
     }
     if (afp->type==ADD_CDS || afp->type==ADD_RRNA || afp->type==ADD_IMP) 
     {
      if (! stringhasnotext (afp->geneName)) {
        grp = CreateNewGeneRef (afp->geneName, NULL, NULL, FALSE);
        if (grp != NULL) 
        {
           sfp = CreateNewFeature (nsep, NULL, SEQFEAT_GENE, NULL);
           if (sfp != NULL) 
           {
              sfp->data.value.ptrvalue = (Pointer) grp;
              sfp->location = SeqLocFree (sfp->location);
              sfp->location = (SeqLocPtr)AsnIoMemCopy ((Pointer) slp, (AsnReadFunc) SeqLocAsnRead, (AsnWriteFunc) SeqLocAsnWrite);
              SetSeqLocPartial (sfp->location, afp->noLeft, afp->noRight);
              sfp->partial = (afp->noLeft || afp->noRight);
           }
        }
      }
     }
  }
  return TRUE;
}


static Boolean ApplyNewSeqFeat (ValNodePtr vnpfeat, Int4 stoptransl, Boolean keep_protID)
{
  BioseqPtr        target_bsp;
  ValNodePtr       vnpf;
  ApplyFormData    af;
  SelEdStructPtr   sesp;
  SeqFeatPtr       sfp;
  SeqAnnotPtr      sap;
  SeqEntryPtr      sep,
                   sep2,
                   sep_head;
  SeqLocPtr        new_slp;
  SeqIdPtr         protid = NULL;
  Uint2            entityID;
  CdRegionPtr      crp;
  CodeBreakPtr     cbp;
  CcId             ci;
  Int2             genCode;
  Boolean          val = FALSE,
                   val2,
                   p5, 
                   p3,
                   exon;

  if (vnpfeat!=NULL)
  {
     MemSet ((Pointer)(&af), 0, sizeof(ApplyFormData));
     af.geneName = NULL;
     af.protName = NULL;
     af.featcomment = NULL;
     af.defline = NULL;
     for (vnpf=vnpfeat; vnpf!=NULL; vnpf=vnpf->next) 
     {
        sesp = (SelEdStructPtr) vnpf->data.ptrvalue;
        new_slp = (SeqLocPtr) sesp->region;
        sep_head  = GetTopSeqEntryForEntityID (sesp->entityID);

        exon = FALSE;
        val = FALSE;
        sep2 = NULL;
        ci.sip = SeqIdDup (SeqIdFindBest(SeqLocId(new_slp), 0));
        ci.sep = NULL;
        ci.bsp = NULL;
        ci.found = FALSE;
        SeqEntryExplore(sep_head,(Pointer)&ci, FindSeqEntryForSeqIdCallback);
        if (ci.bsp!=NULL && ci.sep!=NULL) 
        {
          af.input_entityID = SeqMgrGetEntityIDForSeqEntry(ci.sep);
          sep2 = GetBestTopParentForData (af.input_entityID, ci.bsp);
          SeqIdFree (ci.sip);
        }
        if (sep2 != NULL && vnpf!=NULL && new_slp!=NULL) 
        {
           af.input_entityID = SeqMgrGetEntityIDForSeqEntry(sep2);
           if (af.input_entityID != 0) 
           {
              sep = GetTopSeqEntryForEntityID (af.input_entityID);
              sfp = (SeqFeatPtr)(sesp->data->data.ptrvalue);
              if (sep != NULL && sfp!=NULL) 
              {
                 if (sesp->itemsubtype == FEATDEF_GENE) {
                    af.type = ADD_GENE;
                    af.geneName = sesp->label; 
                 }
                 else if (sesp->itemsubtype == FEATDEF_CDS) {
                    af.type = ADD_CDS;
                    
{{
                    CheckSeqLocForPartial (new_slp, &p5, &p3);
                    if (p3) {
                       exon=(Boolean)(SeqLocStop(new_slp) < ci.bsp->length-1);
                    }
}}
/*
                    if (sfp->data.value.ptrvalue!=NULL){
                       grp = (GeneRefPtr) sfp->data.value.ptrvalue;
                       af.geneName= StringSave (grp->locus);  
                    } 
*/
                    if (sfp->data.choice == SEQFEAT_CDREGION) {
                       crp = (CdRegionPtr) sfp->data.value.ptrvalue;
                       if (crp->code_break !=NULL) {
                          for (cbp=crp->code_break; cbp!=NULL; cbp=cbp->next)
                             cbp->loc = SeqLocReplaceID (cbp->loc, SeqLocId(new_slp));
                       }
                       if (sfp->product!=NULL) 
                       {
                          af.protName = get_names_from_prot (sfp->product);
                          if (keep_protID) {
                             if ((val2=is_id_unique (entityID, SeqLocId(sfp->product))))
                                protid = SeqIdDup (SeqLocId(sfp->product));
                          }
                       }
                       
                    }
                    if (af.protName==NULL && sesp->label)
                       ValNodeAddPointer (&(af.protName), 0, sesp->label);
                 }
                 else if (sesp->itemsubtype == FEATDEF_preRNA ) {
                    af.type = ADD_RRNA;
                 }
                 else if (sesp->itemsubtype == FEATDEF_mRNA ) {
                    af.type = ADD_RRNA;
                 }
                 else if (sesp->itemsubtype == FEATDEF_tRNA ) {
                    af.type = ADD_RRNA;
                 }
                 else if (sesp->itemsubtype == FEATDEF_rRNA ) {
                    af.type = ADD_RRNA;
                 }
                 else if (sesp->itemsubtype == FEATDEF_snRNA ) {
                    af.type = ADD_RRNA;
                 }
                 else if (sesp->itemsubtype == FEATDEF_scRNA ) {
                    af.type = ADD_RRNA;
                 }
                 else if (sesp->itemsubtype == FEATDEF_otherRNA ) {
                    af.type = ADD_RRNA;
                 }
                 else if (sesp->itemsubtype == FEATDEF_PUB ) {
                    af.type = ADD_PUB;
                 }
                 else if (sesp->itemsubtype == FEATDEF_REGION ) {
                    af.type = ADD_REGION;
                 }
                 else if (sesp->itemsubtype == FEATDEF_BOND ) {
                    af.type = ADD_BOND;
                 }
                 else if (sesp->itemsubtype == FEATDEF_SITE ) {
                    af.type = ADD_SITE;
                 }
                 else if (sesp->itemsubtype >= first_GBFeat && sesp->itemsubtype < number_GBFeat+first_GBFeat) { 
                    af.type = ADD_IMP;
                 }
                 else af.type = 0;
                 if (af.type > 0) {
                    CheckSeqLocForPartial (new_slp, &p5, &p3);
                    af.noLeft = p5;
                    af.noRight = p3;
                    val = ApplyBioFeatToSeqEntry (sep2, &af, new_slp, sesp->codonstart, stoptransl, sfp, exon, protid);
                    entityID = ObjMgrGetEntityIDForPointer (ci.bsp);
                    target_bsp = BioseqLockById ((SeqIdPtr) SeqLocId(new_slp)); 
                    if (target_bsp) 
                    {
                       genCode = SeqEntryToGeneticCode (sep2, NULL, NULL, 0); 
                       sap = SeqAnnotNew ();
                       sap->type = 1;
                       sap->data = sfp;
                       SetEmptyGeneticCodes (sap, genCode);
                       sap->data= NULL;
                       SeqAnnotFree(sap);
                       PromoteXrefs (sfp, target_bsp, entityID);
                       BioseqUnlock (target_bsp);
                    }
                 }
              } 
           } 
        } 
     }
  } 
  return val;
}

/**********************************************************************
*** PropagateFeatureBySeqLock
***   propagates features (seqfeat) from a bioseq (source_bspitemID)
***   to another (target_sep)
***
**********************************************************************/

NLM_EXTERN void PropagateFeatureBySeqLock (SeqAnnotPtr sap, Uint2 source_bspitemID, Uint2 target_entityID, SeqEntryPtr target_sep, ValNodePtr seqfeat, Uint1 gap_choice)
{
  BioseqPtr        target_bsp;
  SeqFeatPtr       source_sfp;
  SeqAlignPtr      salp;
  SelEdStructPtr   feat;
  SeqIdPtr         featsip;
  SeqIdPtr         target_sip;
  SeqLocPtr        featslp;
  SeqLocPtr        new_slp;
  ValNodePtr       vnpf;
  ValNodePtr       vnpfeat = NULL;
  ValNodePtr       vnpsfp = NULL;
  Uint2            subtype;
  Uint1            frame;
  Uint1            gap_choice_subtype;
  Boolean          val;

  if (sap != NULL && target_sep != NULL && seqfeat != NULL) {
     if (sap != NULL)
     {   
        target_bsp = (BioseqPtr) target_sep->data.ptrvalue;
        salp = (SeqAlignPtr) sap->data;
        for (vnpf= seqfeat; vnpf!=NULL; vnpf=vnpf->next)
        {
           feat  = (SelEdStructPtr) vnpf->data.ptrvalue;
           val = (Boolean)(feat->bsp_itemID == source_bspitemID);
           if (val) {
              featslp = (SeqLocPtr) feat->region;
              featsip = SeqLocId (featslp);
              subtype = vnpf->choice;
              GatherItem (feat->entityID, feat->itemID, OBJ_SEQFEAT, (Pointer) (&source_sfp), FindSqFeatItem);
              if (source_sfp != NULL) {
                 target_sip=target_bsp->id;
                 if (subtype == FEATDEF_GENE)
                    gap_choice_subtype = IGNORE_GAP_CHOICE;
                 else if (subtype == FEATDEF_PUB)
                    gap_choice_subtype = IGNORE_GAP_CHOICE;
                 else
                    gap_choice_subtype = gap_choice;
                 new_slp = CopySeqLocFromSeqAlign (source_sfp, target_sip, featsip, salp, gap_choice_subtype, &frame);
                 if (new_slp != NULL) {
                    if (is_newfeat (seqfeat, target_entityID, subtype, new_slp))
                    {
                       new_slp = SeqLocReplaceID(new_slp, SeqLocId(source_sfp->location));
                       SeqLocFree (source_sfp->location);
                       source_sfp->location = new_slp;
                    }
                 }
              }
           }
        }
     }
  }
  return;
}

NLM_EXTERN void PropagateFeatureByApply (PropaStructPtr psp)
{
  BioseqPtr        target_bsp;
  SeqFeatPtr       source_sfp, 
                   source_dup;
  SeqAlignPtr      salp;
  SelEdStructPtr   feat, 
                   sesp;
  SeqIdPtr         featsip, 
                   target_sip;
  SeqLocPtr        featslp, 
                   new_slp;
  ValNodePtr       vnpf, 
                   vnpfeat = NULL;
  Uint2            subtype;
  Uint1            frame, 
                   gap_choice_subtype;
  Boolean          val;

  if (psp->target_sep != NULL && psp->source_seqfeat != NULL) {
     if (psp->sap != NULL)
     {   
        target_bsp = (BioseqPtr) psp->target_sep->data.ptrvalue;
        salp = (SeqAlignPtr) psp->sap->data;
        for (vnpf= psp->source_seqfeat; vnpf!=NULL; vnpf=vnpf->next)
        {
           feat  = (SelEdStructPtr) vnpf->data.ptrvalue;
           val = (Boolean)(feat->bsp_itemID == psp->source_bspitemID);
           if (val) 
           {
              featslp = (SeqLocPtr) feat->region;
              featsip = SeqLocId (featslp);
              subtype = vnpf->choice;
              GatherItem (feat->entityID, feat->itemID, OBJ_SEQFEAT, (Pointer) 
                          (&source_sfp), FindSqFeatItem);
              if (source_sfp != NULL) 
              {
                 target_sip=target_bsp->id;
                 if (subtype == FEATDEF_GENE)
                    gap_choice_subtype = IGNORE_GAP_CHOICE;
                 else if (subtype == FEATDEF_PUB)
                    gap_choice_subtype = IGNORE_GAP_CHOICE;
                 else 
                    gap_choice_subtype = psp->gap_choice;
                 new_slp = CopySeqLocFromSeqAlign (source_sfp, target_sip, 
                                      featsip, salp, gap_choice_subtype, &frame);
                 if (new_slp != NULL) 
                 {
                    new_slp = SeqLocReplaceID (new_slp, SeqIdFindBest(target_sip, 0));
                    if (is_newfeat (psp->target_seqfeat, psp->target_entityID, 
                                    subtype, new_slp))
                    {
                       source_dup = (SeqFeatPtr) AsnIoMemCopy((Pointer) source_sfp, (AsnReadFunc)SeqFeatAsnRead, (AsnWriteFunc)SeqFeatAsnWrite);
                       sesp = new_seledstruct_fromseqloc (psp->target_entityID, 
                              psp->target_bsp_itemID, (Uint2)OBJ_SEQFEAT, subtype, 
                              psp->target_bsp_itemID, new_slp, feat->label, 
                              (Pointer)source_dup, 0, frame);
                       if (sesp != NULL) {
                          ValNodeAddPointer(&vnpfeat, 0, (Pointer) sesp);
                       }
                    }
                 }
              }
           }
        }
/* ************
{{
  SeqEntryPtr      top_source_sep;
        top_source_sep = GetTopSeqEntryForEntityID (psp->source_entityID);
        ObjMgrFree (OBJ_SEQENTRY, (Pointer)top_source_sep); 
}}
********** */
        ObjMgrFreeByEntityID (psp->source_entityID);
        val = ApplyNewSeqFeat (vnpfeat, psp->stoptransl, psp->keep_protID);
     }
  }
  return;
}

/**********************************************************
***
*** CopyFeatFunc
***
***********************************************************/
NLM_EXTERN PropaStructPtr CreatePropaStruc (SeqIdPtr target_id, 
                              SeqIdPtr source_id, SeqAlignPtr salp)
{
  SeqLocPtr   source_slp,
              target_slp;
  BioseqPtr   source_bsp = NULL;
  BioseqPtr   target_bsp = NULL;
  SeqEntryPtr source_sep = NULL;

  PropaStructPtr psp = NULL;

  if (target_id==NULL || source_id == NULL)
     return NULL;
  source_bsp = BioseqLockById (source_id);
  target_bsp = BioseqLockById (target_id);
  if (source_bsp!=NULL && target_bsp!=NULL)
  {
        psp = (PropaStructPtr) MemNew (sizeof (PropaStruct));
        psp->sap=SeqAnnotForSeqAlign(salp);
        psp->source_sep = SeqMgrGetSeqEntryForData ((Pointer)source_bsp);
        psp->source_entityID = SeqMgrGetEntityIDForSeqEntry (psp->source_sep);
        psp->source_bspitemID = GetItemIDGivenPointer (psp->source_entityID, 
                                               OBJ_BIOSEQ, source_bsp);

        psp->target_sep = SeqMgrGetSeqEntryForData ((Pointer)target_bsp);
        psp->target_entityID = SeqMgrGetEntityIDForSeqEntry (psp->target_sep);
        psp->target_bsp_itemID = GetItemIDGivenPointer (psp->target_entityID, 
                                               OBJ_BIOSEQ, target_bsp);

        source_slp = SeqLocIntNew (0, source_bsp->length-1, Seq_strand_plus, source_id);
        psp->source_seqfeat = NULL;
        psp->source_seqfeat = CollectFeatureForEditor (source_slp, 
                                      psp->source_seqfeat, psp->source_entityID,
                                      psp->source_bspitemID, NULL, TRUE);
        target_slp = SeqLocIntNew (0, target_bsp->length-1, Seq_strand_plus, 
                                      target_id);
        psp->target_seqfeat = NULL;
        psp->target_seqfeat = CollectFeatureForEditor (target_slp, 
                                      psp->target_seqfeat, psp->target_entityID, 
                                      psp->target_bsp_itemID, NULL, TRUE);
        BioseqUnlock (target_bsp); 
        BioseqUnlock (source_bsp); 
/*** TODO free source_slp, target_slp ********/
  }
  return psp;
}


/**********************************************************
***
*** MergeFunc
***
***********************************************************/
static void FreeSourceBsp (SeqIdPtr source_id)
{
  BioseqPtr   bsp;
  SeqEntryPtr sep=NULL;
  Uint2       entityID;

  bsp=BioseqLockById (source_id);
  if (bsp) {
     sep = SeqMgrGetSeqEntryForData (bsp);
     entityID = SeqMgrGetEntityIDForSeqEntry (sep);
     /* sep = GetTopSeqEntryForEntityID (entityID); */
     BioseqUnlock (bsp);
     ObjMgrFreeByEntityID (entityID);
  }
  /*
  if (sep)
     ObjMgrFree (OBJ_SEQENTRY, (Pointer)sep); */

}

static void AddQualToSourceSeqFeat (BioseqPtr source_bsp, SeqIdPtr target_id, Uint2 entityID, Boolean keep_protid)
{
  SeqAnnotPtr sap;
  SeqFeatPtr  sfp;
  ValNodePtr  protName=NULL;
  SeqIdPtr    prot_id=NULL;
  Char        string_id[128];
  Boolean     val;

  for (sap=source_bsp->annot; sap!=NULL; sap=sap->next)
  {
     if (sap->type==1) {
        for(sfp = (SeqFeatPtr)sap->data;sfp!=NULL; sfp=sfp->next)
        {
           if (sfp->data.choice==SEQFEAT_CDREGION) {
              if (sfp->product)
              {
                 if (keep_protid) {
                    if ((val=is_id_unique (entityID, SeqLocId(sfp->product)))) {
                       prot_id = SeqIdDup (SeqLocId(sfp->product));
                    }
                 }
                 if (prot_id==NULL) {
                    prot_id = MakeNewProteinSeqId (NULL, target_id);
                 }
                 SeqIdWrite  (prot_id, string_id , PRINTID_FASTA_LONG, 128);
                 AddQualifierToFeature (sfp, "protein_id", string_id);

                 protName = get_names_from_prot (sfp->product);
                 if (protName)
                    AddQualifierToFeature (sfp, "product", (CharPtr)protName->data.ptrvalue);
                 sfp->product = NULL;
              }
           }
        } 
     }
  }
}

static void PromoteTargetBsp (BioseqPtr target_bsp, Uint2 entityID, Int2 genCode)
{
  SeqAnnotPtr sap;
  SeqFeatPtr  sfp;

  for (sap=target_bsp->annot; sap!=NULL; sap=sap->next)
  {
     if (sap->type==1) {
          SetEmptyGeneticCodes (sap, genCode);
          sfp = (SeqFeatPtr)sap->data;
          PromoteXrefs (sfp, target_bsp, entityID); 
     }
  }
}

static Int4 map_position_seqalign (SeqAlignPtr salp, Int4 pos, ValNodePtr sqlocs)
{
  SeqIdPtr sip1, sip2;
  Int4     tmp_pos,
           froms;
  Int2     chklocp ;

  sip1 = SeqAlignId (salp, 0);
  sip2 = SeqAlignId (salp, 1);
  chklocp = chkloc (sip2, pos, sqlocs, &froms);
  tmp_pos = SeqCoordToAlignCoord (pos, sip2, salp, 0, chklocp);
  tmp_pos = (Int4) AlignCoordToSeqCoord2 (tmp_pos, sip1, salp, sqlocs, 0);
  return tmp_pos;
}


NLM_EXTERN Boolean MergeFunc (SeqIdPtr target_id, SeqIdPtr source_id, SeqIdPtr merge_id, SeqAlignPtr salp, Int4 fromseq2, Int4 toseq2, ValNodePtr sqlocs, Boolean spliteditmode, Boolean keep_protID)
{
  SeqEntryPtr   sep;
  BioseqPtr     merge_bsp,
                target_bsp;
  SeqLocPtr     target_slp;
  Int4          fromseq1, toseq1,
                from1seq1, to1seq1,
                from_overlapp, to_overlapp;
  Int4          caret = -1, 
                overlapp=0;
  Int2          genCode;
  Uint2         target_entityID;
  Boolean       ok = TRUE;

  if (target_id==NULL || source_id==NULL || merge_id==NULL || toseq2 < fromseq2)
     return FALSE;
  merge_bsp =  BioseqLockById (merge_id);
  if (merge_bsp!=NULL) 
  {
   target_bsp = BioseqLockById (target_id);
   if (target_bsp!=NULL) 
   {
     fromseq1= map_position_seqalign(salp,fromseq2, sqlocs);
     toseq1  = map_position_seqalign(salp,toseq2, sqlocs);
     from1seq1= map_position_seqalign(salp,fromseq2-1, sqlocs);
     to1seq1  = map_position_seqalign(salp,toseq2+1, sqlocs);
     overlapp = 0;
     if (toseq1>-1) {
        if (fromseq1>-1) {
           overlapp=toseq1-fromseq1 + 1;
           from_overlapp = fromseq1; 
           to_overlapp = toseq1;
        } else {
           overlapp=toseq1 + 1;
           from_overlapp = 0; 
           to_overlapp = toseq1;
        }
     }
     else if (to1seq1 > -1) {
        if (fromseq1>-1) {
           overlapp = to1seq1 - fromseq1;
           from_overlapp = fromseq1; 
           to_overlapp = to1seq1-1;
        } else if (from1seq1>-1) {
           overlapp = to1seq1 - from1seq1 -1;
           from_overlapp = from1seq1+1; 
           to_overlapp = to1seq1-1;
        }
     }
     else if (toseq1==-2 && to1seq1 ==-2)
     {
        if (fromseq1 > -1 || from1seq1 > -1) {
           if (fromseq1 < 0)
              fromseq1 = from1seq1+1;
           if (target_bsp->length > fromseq1+1) {
              overlapp = target_bsp->length - fromseq1;
              from_overlapp = fromseq1;
              to_overlapp = target_bsp->length-1;
           }
        }
        else ok = FALSE;
     }
     if (to1seq1 == -2) 
        caret = -2;
     else if (to1seq1 == 0)
        caret = 0;
     else if (overlapp <= to1seq1) {
        if (overlapp == 0)
           caret = to1seq1;
        else 
           caret = to1seq1 - overlapp;
     }
     if (ok) {
        if (overlapp > 0) 
        { 
           target_slp=SeqLocIntNew (from_overlapp, to_overlapp, Seq_strand_plus, target_id);
           SeqDeleteByLoc (target_slp, TRUE, spliteditmode);
           SeqLocFree (target_slp);
        }
        target_entityID = ObjMgrGetEntityIDForPointer (target_bsp);
        if (target_entityID) 
        {
           AddQualToSourceSeqFeat (merge_bsp, target_id, target_entityID, keep_protID);
           FreeSourceBsp (source_id);
           ok = BioseqInsert (merge_bsp->id, FIRST_RESIDUE, LAST_RESIDUE, Seq_strand_plus, target_id, caret, TRUE, TRUE, spliteditmode);
           sep = GetBestTopParentForData (target_entityID, target_bsp);
           genCode = SeqEntryToGeneticCode (sep, NULL, NULL, 0);
           PromoteTargetBsp (target_bsp, target_entityID, genCode);
        }
     }
     BioseqUnlock (target_bsp);
   }
   BioseqUnlock (merge_bsp);
  }
  return ok;
}

NLM_EXTERN Boolean FeatListToProp (EditAlignDataPtr adp, ValNodePtr vnpfeat)
{
  SeqLocPtr        featslp;
  SeqIdPtr         featsip,
                   bsp_id;
  ValNodePtr       vnp,
                   pre,
                   next, 
                   datavnp;
  SeqLocPtr        new_slp;
  SelEdStructPtr   sesp;
  SeqFeatPtr       source_sfp=NULL,
                   source_dup;
  SeqAnnotPtr      sap;
  SeqAlignPtr      salp;
  Uint2            feat_itemID;
  Uint1            frame;
  Boolean          val,
                   keep_sesp;

  vnp=vnpfeat;
  pre = NULL;
  while (vnp!=NULL)
  {
     next = vnp->next;
     keep_sesp = FALSE;
     sesp=(SelEdStructPtr) vnp->data.ptrvalue;
     feat_itemID = sesp->itemtype;
     
/**/     
     GatherItem (sesp->entityID, sesp->itemID, OBJ_SEQFEAT, (Pointer) (&source_sfp), FindSqFeatItem); 
/**/     
/**
     SeqMgrIndexFeatures (sesp->entityID, NULL);
     SeqMgrExploreFeatures (bsp, (Pointer)stdin, GetCDSCB, NULL, NULL, NULL);
     SeqMgrClearFeatureIndexes (sesp->entityID, NULL);
**/
     if (source_sfp != NULL) 
     {
        featslp = (SeqLocPtr) sesp->region;
        featsip = SeqLocId (featslp);
        bsp_id = (SeqIdPtr) sesp->data->data.ptrvalue;
        sap = SeqAnnotBoolSegToDenseSeg (adp->sap_align);
        salp = (SeqAlignPtr)sap->data;
        new_slp = CopySeqLocFromSeqAlign (source_sfp, bsp_id, featsip, salp, adp->gap_choice, &frame);
        if (new_slp != NULL) {
           if (is_newfeat (adp->seqfeat, sesp->entityID, sesp->itemsubtype, new_slp) )
           {
              source_dup = (SeqFeatPtr) AsnIoMemCopy((Pointer) source_sfp, (AsnReadFunc)SeqFeatAsnRead, (AsnWriteFunc)SeqFeatAsnWrite);
              sesp->data->data.ptrvalue = ValNodeFree (sesp->data->data.ptrvalue);
              sesp->data = MemFree (sesp->data);
              datavnp = ValNodeNew (NULL);
              datavnp->choice = 0;
              datavnp->data.ptrvalue = (Pointer)source_dup;
              sesp->data = datavnp;
              sesp->codonstart = frame;
              sesp->itemID = sesp->bsp_itemID;
              ValNodeFree((ValNodePtr)sesp->region);
              sesp->region = new_slp;
              keep_sesp = TRUE;
           }
        }
     }
     if (!keep_sesp) {
        vnp->next = NULL;
        sesp->data->data.ptrvalue = ValNodeFree (sesp->data->data.ptrvalue);
        sesp->data = ValNodeFree (sesp->data);
        sesp->region = ValNodeFree((ValNodePtr)sesp->region);
        vnp->data.ptrvalue = MemFree (sesp);
        vnp = ValNodeFree (vnp); 
        if (pre==NULL) {
           vnpfeat = next;
        }
        else 
           pre->next = next;
     }
     else 
        pre = vnp;
     vnp = next;
  }
  val = ApplyNewSeqFeat (vnpfeat, adp->stoptransl, FALSE);
  for (vnp=vnpfeat; vnp!=NULL;vnp=vnp->next) {
     sesp=(SelEdStructPtr) vnp->data.ptrvalue;
     sesp->data->data.ptrvalue = NULL;
     sesp->data = ValNodeFree (sesp->data);
     sesp->region = ValNodeFree((ValNodePtr)sesp->region);
     vnp->data.ptrvalue = MemFree (sesp);
  }
  ValNodeFree (vnpfeat);
  return val;
}
