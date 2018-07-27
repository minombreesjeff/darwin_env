static char const rcsid[] = "$Id: actutils.c,v 6.36 2004/02/11 20:51:48 bollin Exp $";

/* ===========================================================================
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
*  any work or product based on this material.
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
* File Name:  actutils.c
*
* Author:  Sarah Wheelan
*
* Version Creation Date:   2/00
*
* $Revision: 6.36 $
*
* File Description: utility functions for alignments
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: actutils.c,v $
* Revision 6.36  2004/02/11 20:51:48  bollin
* use error log instead of popup message to indicate that  BLAST found no sequence similarity
*
* Revision 6.35  2004/01/02 18:11:28  kans
* Sqn_GlobalAlign2Seq flips code break, anticodon when reverse complementing sequence
*
* Revision 6.34  2003/06/30 15:01:29  whlavina
* Correct minus strand handling in CreaeContinuousAln functions; previous
* code could corrupt alignments (stop2-start1>1 would imply len<-2 if
* ExtendAlnRight ever gets called).
*
* Revision 6.33  2003/05/30 17:25:35  coulouri
* add rcsid
*
* Revision 6.32  2002/12/19 14:02:28  johnson
* change c++-style comments to c-style
*
* Revision 6.31  2002/11/20 16:00:03  johnson
* Revamped CpG routines; now equivalent to "strict" human CpG mapviewer track
*
* Revision 6.30  2002/06/11 15:25:12  johnson
* two minor bug fixes to CpG functions
*
* Revision 6.29  2002/04/02 21:45:32  wheelan
* added CpG finding functions written by Philip Johnson
*
* Revision 6.28  2002/03/27 17:35:24  todorov
* 1) AMAlignIndexFreeEitherIndex instead of AMFreeAllIndexes
* 2) recreated ACT_MakeProfileFromSA
*
* Revision 6.27  2002/03/27 14:40:41  kans
* #include <alignmgr.h>
*
* Revision 6.26  2002/03/26 19:56:21  todorov
* alignmgr to alignmgr2 transition
*
* Revision 6.25  2001/12/28 21:22:32  wheelan
* bug fix in Sqn_GlobalAlign2Seqs
*
* Revision 6.24  2001/09/04 13:47:15  wheelan
* made several functions extern
*
* Revision 6.23  2001/07/13 14:17:57  wheelan
* moved Sqn_GlobalAlign2Seq and associated functions here
*
* Revision 6.22  2001/05/22 12:01:07  wheelan
* changes to avoid overflow on alpha platform
*
* Revision 6.21  2001/03/26 16:45:57  wheelan
* fixed uninitialized variables
*
* Revision 6.20  2001/01/09 23:18:55  lewisg
* fix memory leaks
*
* Revision 6.19  2000/10/23 18:43:30  wheelan
* minor bug fix
*
* Revision 6.18  2000/09/07 04:53:40  sicotte
* fix alignment calls, bad matrix calls, and misc alignments problems for sequence update
*
* Revision 6.16  2000/08/28 16:19:11  sicotte
* moved AlnMgrSeqAlignMergeTwoPairwiseEx AlnMgrSeqAlignMergeTwoPairwise AlnMgrSeqAlignMergePairwiseSet to actutils.c from alignmgr.c
*
* Revision 6.15  2000/07/21 21:41:04  sicotte
* fix bug in AlnMgrForcePairwiseContinuousEx, that was inserting two copies of all the seqaligns when trying to realign both ends
*
* Revision 6.14  2000/05/05 11:53:10  wheelan
* bug fixes in ACT_MakeProfileFromSA
*
* Revision 6.13  2000/05/04 16:45:19  wheelan
* changes to profile builder to accomodate IBMed BLAST results
*
* Revision 6.12  2000/05/03 19:59:42  wheelan
* added fix for NULL alignments
*
* Revision 6.11  2000/05/02 12:00:21  wheelan
* fixed memory leaks
*
* Revision 6.10  2000/05/01 19:54:26  wheelan
* fixed memory leak
*
* Revision 6.9  2000/04/22 15:54:57  wheelan
* bug fixes in profile maker
*
* Revision 6.8  2000/04/18 13:57:14  wheelan
* added AlnMgrForcePairwiseContinuousEx
*
* Revision 6.7  2000/04/11 14:50:28  wheelan
* bug fix in AlnMgrForceContinuous
*
* Revision 6.6  2000/03/16 12:53:45  wheelan
* bug fix in AlnMgrForceContinuous
*
* Revision 6.5  2000/03/14 11:25:47  wheelan
* added ACT_ProfileFree functions
*
* Revision 6.4  2000/03/10 16:57:37  wheelan
* fixed AlnMgrForceContinuous
*
* Revision 6.3  2000/03/07 17:55:02  wheelan
* bug fixes in AlnMgrForcePairwiseContinuous
*
* Revision 6.2  2000/03/02 21:11:06  lewisg
* use bandalign for import sequence, make standalone ddv use viewmgr, make dialogs modal, send color update
*
* Revision 6.1  2000/02/11 17:31:44  kans
* initial checkin of functions depending upon blast/bandalign (SW)
*
* ==========================================================================
*/

#include <actutils.h>
#include <viewmgr.h>
#include <alignmgr.h>

static void StateTableSearch (TextFsaPtr tbl, CharPtr txt, Int2Ptr state, Int4 pos, ACT_sitelistPtr PNTR asp_prev, ACT_sitelistPtr PNTR asp_head);
static Boolean am_isa_gap(Int4 start, Int4 prevstop, Uint1 strand);
static void am_fix_strand(SeqAlignPtr sap, Uint1 strand1, Uint1 strand2);

#define CG_MINLEN 500 /*minimum length for CpG island; should be equivalent to mapviewer track setting! */

/*-----------------------------------------------------------------------------
  PRE : locked nucleotide bioseq ptr
  POST: entire sequence loaded into memory in iupacna format
-----------------------------------------------------------------------------*/
static CharPtr LoadSequence(BioseqPtr bsp)
{
    SeqPortPtr spp;
    CharPtr buf;
    Int4 readCount;

    buf = (CharPtr) MemNew(bsp->length * sizeof(Char));
    if (!buf) {
        ErrPostStr(SEV_ERROR, 0,0, "Error allocating memory for sequence!");
        return NULL;
    }

    spp = SeqPortNew(bsp, 0, bsp->length-1, Seq_strand_unknown,
                     Seq_code_iupacna);

    readCount = 0;
    while (readCount < bsp->length) {
        readCount += SeqPortRead(spp, (UcharPtr) buf+readCount, 25000);
        assert(readCount != 0);
    }
    SeqPortFree(spp);

    return buf;
}

/*-----------------------------------------------------------------------------
  PRE : sequence, position to be added, cgIsland information
  POST: cgIsle adjusted according to the nucleotide at position i & i-1
-----------------------------------------------------------------------------*/
static void AddPosition(CharPtr seq, Int4 i, ACT_CGInfoPtr cgIsle)
{
    switch(seq[i]) {
    case 'A': cgIsle->a++; break;
    case 'C': cgIsle->c++; break;
    case 'G': cgIsle->g++;
        if (i > 0 && seq[i-1] == 'C')
            cgIsle->cg++;
        break;
    case 'T': cgIsle->t++; break;
    case 'N': cgIsle->n++; break;
    }
}

/*-----------------------------------------------------------------------------
  PRE : sequence, position to be removed, cgIsland information
  POST: cgIsle adjusted according to the nucleotide at position i & i-1
-----------------------------------------------------------------------------*/
static void RemovePosition(CharPtr seq, Int4 i, ACT_CGInfoPtr cgIsle)
{
    switch(seq[i]) {
    case 'A': cgIsle->a--; break;
    case 'C': cgIsle->c--; break;
    case 'G': cgIsle->g--;
        if (i > 0 && seq[i-1] == 'C')
            cgIsle->cg--;
        break;
    case 'T': cgIsle->t--; break;
    case 'N': cgIsle->n--; break;
    }
}

/*-----------------------------------------------------------------------------
  PRE : sequence, length of sequence, (ptr to) cgIsle structure with to &
  from fields filled in
  POST: cgInfo filled with stats (#a's, c's, etc.) for window (to ->|
  from)
-----------------------------------------------------------------------------*/
static void CalcWindowStats(CharPtr seq, ACT_CGInfoPtr cgIsle)
{
    Int4 i;

    cgIsle->a = cgIsle->t = cgIsle->g = cgIsle->c = cgIsle->cg = 0;
    cgIsle->length = cgIsle->to - cgIsle->from + 1;

    for (i = cgIsle->from; i <= cgIsle->to; i++)
        AddPosition(seq, i, cgIsle);
}

/*-----------------------------------------------------------------------------
  PRE : island structure filled
  POST: whether or not we consider this to be a CpG island
-----------------------------------------------------------------------------*/
static Boolean IsIsland(ACT_CGInfoPtr isle)
{
    Uint4 len = isle->to-isle->from+1;

    return (100*(isle->c + isle->g) > 50*len &&
            100* isle->cg*len > 60* isle->c*isle->g);
}


/*-----------------------------------------------------------------------------
  PRE : sequence, length of sequence, win->from field filled in
  POST: whether or not we found a window further down the sequence that
  meets the mimimum criteria; if so, 'win' is set to that window
-----------------------------------------------------------------------------*/
static Boolean SlideToHit(CharPtr seq, Int4 seqLength, ACT_CGInfoPtr win)
{
    Boolean inIsland, done;

    win->to = win->from + CG_WINDOWSIZE - 1;

    if (win->to >= seqLength)
        return FALSE;

    CalcWindowStats(seq, win);

    inIsland = FALSE;
    done = FALSE;

    while (win->to < seqLength && !IsIsland(win)) {
        /* remove 1 nt from left side */
        RemovePosition(seq, win->from, win);

        /* advance */
        ++win->from;
        ++win->to;

        if (win->to < seqLength) {
            /* add 1 nt onto right side */
            AddPosition(seq, win->to, win);
        }
    }
    
    return IsIsland(win);
}

/*-----------------------------------------------------------------------------
  PRE : sequence, length of sequence, window that meets the GC & CpG criteria
  POST: whether or not the island can be extended to reach at least the
  minimum length; if so, isle is set to that window
-----------------------------------------------------------------------------*/
static Boolean ExtendHit(CharPtr seq, Int4 seqLength, ACT_CGInfoPtr isle)
{
    ACT_CGInfoPtr win = (ACT_CGInfoPtr) MemNew(sizeof(ACT_CGInfo));
    memcpy(win, isle, sizeof(ACT_CGInfo));

    /* jump by 200bp increments */
    while (win->to + CG_WINDOWSIZE < seqLength && IsIsland(win)) {
        win->from += CG_WINDOWSIZE;
        win->to += CG_WINDOWSIZE;
        CalcWindowStats(seq, win);
    }

    /* if we overshot, slide back by 1bp increments */
    while (!IsIsland(win)) {
        RemovePosition(seq, win->to, win);
        --win->from;
        --win->to;
        AddPosition(seq, win->from, win);
    }

    /* trim ends of entire island until we're above criteria again */
    isle->to = win->to;
    CalcWindowStats(seq, isle);
    while(!IsIsland(isle) && (isle->from < isle->to)) {
        RemovePosition(seq, isle->to, isle);
        RemovePosition(seq, isle->from, isle);
        --isle->to;
        ++isle->from;
    }

    free(win);

    if (isle->from >= isle->to) {/* in case we trimmed to nothing */
        isle->to = isle->from;
        return FALSE;
    }
    return (isle->to - isle->from + 1 > CG_MINLEN);
}

/*-----------------------------------------------------------------------------
  PRE : locked nucleotide bioseq ptr
  POST: linked list of CpG islands (if any).  Uses algorithm from Takai and
  Jones, Proc Natl Acad Sci U S A 2002 Mar 19;99(6):3740-5.
  Cutoffs used in the "strict" human CpG map viewer track as of 11/20/02:
  CG_WINDOWSIZE=200, CG_MINLEN=500, CpG obs/exp >= 0.6, G+C >= 0.5
-----------------------------------------------------------------------------*/
NLM_EXTERN ACT_CGInfoPtr ACT_FindCpG(BioseqPtr bsp)
{
    CharPtr seq;
    ACT_CGInfoPtr cgHead, isle;

    if (bsp == NULL || bsp->length < CG_WINDOWSIZE)
        return NULL;
    if (bsp->mol == Seq_mol_aa) {
        Message(SEV_WARNING, "Must use nucleotide sequence\n");
        return NULL;
    }

    if (!(seq = LoadSequence(bsp)))
        return NULL;/* error message displayed in LoadSequence */


    cgHead = NULL;
    isle = (ACT_CGInfoPtr) MemNew(sizeof(ACT_CGInfo));
  
    isle->from = 0;

    while (SlideToHit(seq, bsp->length, isle)) {
        if (ExtendHit(seq, bsp->length, isle)) {
            isle->next = cgHead;
            cgHead = isle;
            isle = (ACT_CGInfoPtr) MemNew(sizeof(ACT_CGInfo));
            isle->to = cgHead->to;
        }
        isle->from = isle->to + 1;
    }

    free(isle);
    free(seq);
    return cgHead;
}

/*NLM_EXTERN ACT_CGInfoPtr ACT_FindCpG(BioseqPtr bsp)
{
   ACT_CGInfoPtr    acg;
   ACT_CGInfoPtr    acg_head;
   ACT_CGInfoPtr    acg_prev;
   ACT_sitelistPtr  asp;
   ACT_sitelistPtr  asp_head;
   ACT_sitelistPtr  asp_prev;
   ACT_sitelistPtr  asp_tmp;
   Uint1Ptr         buf;
   CharPtr          c;
   Int2             j;
   Int4             max_len;
   Int4             offset;
   Int4             pos;
   Uint1            prev;
   Uint1            r;
   Uint1            res1;
   Uint1            res2;
   Uint1            residue;
   Int4             start;
   Int2             state;
   Int4             state_r;
   Int2             state_test;
   TextFsaPtr       tbl;
   Int4             x;
   FloatHi          y;

   if (bsp == NULL)
      return NULL;
   if (bsp->mol == Seq_mol_aa)
   {
      Message(SEV_WARNING, "Must use nucleotide sequence\n");
      return NULL;
   }
   if (bsp->length < MAX_LEN)
      max_len = bsp->length+1;
   else
      max_len = MAX_LEN;
   buf = (Uint1Ptr)MemNew((max_len)*sizeof(Uint1));
   state = 0;
   prev = 0;
   pos = 0;
   state_r = 0;
   asp_prev = asp_head = NULL;
   tbl = TextFsaNew();
   if (tbl == NULL)
      return NULL;
   TextFsaAdd(tbl, "CCCGGG");
   TextFsaAdd(tbl, "CCGCGG");
   state_test = 0;
   acg = (ACT_CGInfoPtr)MemNew(sizeof(ACT_CGInfo));
   acg_head = NULL;
   offset = 0;
   while ((residue = ACT_GetResidue(pos, buf, &offset, bsp)) != 0)
   {
      if (residue == 65)
         c = "A";
      else if (residue == 67)
         c = "C";
      else if (residue == 71)
         c = "G";
      else if (residue == 84)
         c = "T";
      else
         c = "N";
      StateTableSearch(tbl, c, &state_test, pos, &asp_prev, &asp_head);
      acg->length++;
      acg->to = pos;
      pos++;
      if (residue == 65)
         acg->a++;
      else if (residue == 67)
         acg->c++;
      else if (residue == 71)
      {
         if (prev == 67)
            acg->cg++;
         acg->g++;
      } else if (residue == 84)
         acg->t++;
      else
         acg->n++;
      prev = residue;
      if (acg->length >= 200)
      {
         if (state == 0)
         {
            if (100*(acg->cg)*(acg->length) > 60*(acg->c)*(acg->g) && 10*(acg->c+acg->g) > 6*(acg->length))
            {
               state = 1;
            } else
            {
               res1 = ACT_GetResidue(acg->from, buf, &offset, bsp);
               if (res1 == 67)
               {
                  res2 = ACT_GetResidue(acg->from+1, buf, &offset, bsp);
                  if (res2 == 71)
                     acg->cg--;
               }
               if (res1 == 65)
                  acg->a--;
               else if (res1 == 67)
                  acg->c--;
               else if (res1 == 71)
                  acg->g--;
               else if (res1 == 84)
                  acg->t--;
               else
                  acg->n--;
               acg->from++;
               acg->length--;
            }
         } else if (state == 1)
         {
            if (100*(acg->cg)*(acg->length) <= 60*(acg->c)*(acg->g) || 10*(acg->c+acg->g) < 6*(acg->length))
            {
               state = 0;
               if (acg_head)
               {
                  acg_prev->next = acg;
                  acg_prev = acg;
               } else
                  acg_head = acg_prev = acg;
               j=0;
               if (acg->from - 2000 < 0)
                  start = 0;
               else
                  start = acg->from - 2000;
               r = 1;
               acg->sequence = (CharPtr)MemNew(20000*sizeof(Char));
               for (x=start; x<(acg->to+2000) && r > 0; x++, j++)
               {
                  r = ACT_GetResidue(x, buf, &offset, bsp);
                  if (r == 65)
                     acg->sequence[j] = 'A';
                  else if (r == 67)
                     acg->sequence[j] = 'C';
                  else if (r == 71)
                     acg->sequence[j] = 'G';
                  else if (r == 84)
                     acg->sequence[j] = 'T';
                  else
                     acg->sequence[j] = 'N';
               }
               y = (FloatHi)((acg->cg)*(acg->length))/(FloatHi)((acg->c)*(acg->g));
               printf("Coordinates: %d to %d CpG: %d to %d conf: %f\n%s\n", start, x, acg->from, acg->to, y, acg->sequence);
               MemFree(acg->sequence);
               acg = (ACT_CGInfoPtr)MemNew(sizeof(ACT_CGInfo));
               acg->from = pos;
               acg->length = 1;
               if (residue == 65)
                  acg->a++;
               else if (residue == 67)
                  acg->c++;
               else if (residue == 71)
                  acg->g++;
               else if (residue == 84)
                  acg->t++;
               else
                  acg->n++;
            } else if (acg->length > 1000)
            {
               res1 = ACT_GetResidue(acg->from, buf, &offset, bsp);
               if (res1 == 65 || res1 == 84)
               {
                  acg->from++;
                  acg->length--;
                  if (res1 == 65)
                     acg->a--;
                  else if (res1 == 84)
                     acg->t--;
               }
            }
         }
      }
   }
    check for restriction sites in potential islands found 
   acg = acg_head;
   return acg_head;
   acg_prev = NULL;  Statement not reached... 
   while (acg)
   {
      asp_prev = NULL;
      asp = asp_head;
      while (asp)
      {
         if (asp->start >= acg->from && asp->start < acg->to - 9)
         {
            if (asp_prev)
            {
               asp_prev->next = asp->next;
               asp_tmp = asp->next;
            } else
               asp_head = asp_tmp = asp->next;
            asp->next = acg->asp;
            acg->asp = asp;
            asp = asp_tmp;
         } else
         {
            asp_prev = asp;
            asp = asp->next;
         }
      }
      if (acg->asp == NULL)
      {
         if (acg_prev != NULL)
         {
            acg_prev->next = acg->next;
            acg->next = NULL;
            MemFree(acg);
            acg = acg_prev->next;
         } else
         {
            acg_head = acg->next;
            acg->next = NULL;
            MemFree(acg);
            acg = acg_head;
         }
      } else
      {
         if (acg->asp->next == NULL)
         {
            if (acg_prev != NULL)
            {
               acg_prev->next = acg->next;
               acg->next = NULL;
               MemFree(acg);
               acg = acg_prev->next;
            } else
            {
               acg_head = acg->next;
               acg->next = NULL;
               MemFree(acg);
               acg = acg_head;
            }
         } else
         {
            acg_prev = acg;
            acg = acg->next;
         }
      }
   }
   MemFree(buf);
   return acg_head;
}*/

NLM_EXTERN Uint1 ACT_GetResidue(Int4 pos, Uint1Ptr buf, Int4Ptr offset, BioseqPtr bsp)
{
   Int4        bufsize;
   SeqPortPtr  spp;

   if (offset == NULL)
      return 0;
   if (pos > bsp->length - 1)
      return 0;
   if (buf[0] == 0 || pos > (*offset + MAX_LEN - 1) || pos < *offset)
   {
      if (pos > *offset + MAX_LEN - 1)
         *offset = *offset + MAX_LEN;
      else if (pos < *offset)
         *offset = pos;
      else
         *offset = 0;
      if (bsp->length < MAX_LEN)
      {
         bufsize = bsp->length;
         spp = SeqPortNew(bsp, 0, -1, 0, Seq_code_iupacna);
      } else if (bsp->length < *offset + MAX_LEN-1)
      {
         bufsize = bsp->length - *offset + 1;
         spp = SeqPortNew(bsp, *offset, bsp->length-1, 0, Seq_code_iupacna);
      } else
      {
         bufsize = MAX_LEN;
         spp = SeqPortNew(bsp, *offset, *offset+MAX_LEN-1, 0, Seq_code_iupacna);
      }
      if (spp == NULL)
      {
         Message(SEV_WARNING, "Couldn't create SeqPort\n");
         return 0;
      }
      SeqPortRead(spp, buf, bufsize);
   }
   return (buf[pos-(*offset)]);
}

static void StateTableSearch (TextFsaPtr tbl, CharPtr txt, Int2Ptr state, Int4 pos, ACT_sitelistPtr PNTR asp_prev, ACT_sitelistPtr PNTR asp_head)
{
   ACT_sitelistPtr  asp;
   Char             ch;
   ValNodePtr       matches;
   CharPtr          ptr;
   ValNodePtr       vnp;

   if (tbl == NULL || txt == NULL) return;

   for (ptr = txt, ch = *ptr; ch != '\0'; ptr++, ch = *ptr) {
      *state = TextFsaNext (tbl, *state, ch, &matches);
      for (vnp = matches; vnp != NULL; vnp = vnp->next) {
         asp = (ACT_sitelistPtr)MemNew(sizeof(ACT_sitelist));
         if (asp_prev)
         {
            if (*asp_prev)
            {
               (*asp_prev)->next = asp;
               *asp_prev = asp;
            } else
               *asp_prev = *asp_head = asp;
            asp->name = (CharPtr) vnp->data.ptrvalue;
            asp->start = pos;
         }
      }
   }
}

NLM_EXTERN ACTProfilePtr ACT_ProfileNew(Boolean nuc)
{
   ACTProfilePtr  app;
   FloatHiPtr     PNTR freq;

   app = (ACTProfilePtr)MemNew(sizeof(ACTProfile));
   if (nuc)
   {
      freq = (FloatHiPtr PNTR)MemNew(ACT_NUCLEN*sizeof(FloatHiPtr));
      app->freq = freq;
      app->nuc = TRUE;
   } else
   {
      freq = (FloatHiPtr PNTR)MemNew(ACT_PROTLEN*sizeof(FloatHiPtr));
      app->freq = freq;
      app->nuc = FALSE;
   }
   return app;
}

/***************************************************************************
*
*  ACT_ProfileFree frees a single profile; ACT_ProfileSetFree frees an
*  entire linked list of profiles.
*
***************************************************************************/
NLM_EXTERN ACTProfilePtr ACT_ProfileFree(ACTProfilePtr app)
{
   Int4  i;
   Int4  j;

   if (app == NULL)
      return NULL;
   if (app->nuc)
      j = ACT_NUCLEN;
   else
      j = ACT_PROTLEN;
   for (i=0; i<j; i++)
   {
      MemFree(app->freq[i]);
   }
   MemFree(app->freq);
   app->next = NULL;
   MemFree(app);
   return NULL;
}

NLM_EXTERN ACTProfilePtr ACT_ProfileSetFree(ACTProfilePtr app)
{
   ACTProfilePtr  app_next;

   while (app != NULL)
   {
      app_next = app->next;
      app->next = NULL;
      ACT_ProfileFree(app);
      app = app_next;
   }
   return NULL;
}

NLM_EXTERN void ACT_BuildProfile(SeqLocPtr slp, ACTProfilePtr PNTR app, Int4Ptr count, Int4 length)
{
   Int4        i;
   Int4        len;
   Uint1       res;
   SeqPortPtr  spp;

   if (app == NULL)
      return;
   if (slp == NULL)
   {
      *count = *count+length;
      if ((*app)->len <= *count)
      {
         *count = 0;
         *app = (*app)->next;
      }
      return;
   }
   len = SeqLocLen(slp);
   if (len <= 0)
      return;
   if ((*app)->len == 0)
   {
      (*app)->len = len;
      if ((*app)->nuc)
      {
         for (i=0; i<ACT_NUCLEN; i++)
         {
            (*app)->freq[i] = (FloatHiPtr)MemNew((*app)->len*sizeof(FloatHi));
         }
      } else
      {
         for (i=0; i<ACT_PROTLEN; i++)
         {
            (*app)->freq[i] = (FloatHiPtr)MemNew((*app)->len*sizeof(FloatHi));
         }
      }
   } else
   {
      if (len > (*app)->len) /* seqloc is longer than the */
         return;          /* existing profile -- don't add it     */
   }
   if ((*app)->nuc)
      spp = SeqPortNewByLoc(slp, Seq_code_ncbi4na);
   else
      spp = SeqPortNewByLoc(slp, Seq_code_ncbistdaa);
   if (spp == NULL)
      return;
   if (*count == 0)
     (*app)->numseq++;
   i=0;
   if ((*app)->nuc == FALSE)
   {
      while ((res = SeqPortGetResidue(spp)) != SEQPORT_EOF && i+*count<((*app)->len))
      {
         (*app)->freq[res][i+*count]++;
         i++;
      }
   } else
   {
      while ((res = SeqPortGetResidue(spp)) != SEQPORT_EOF && i+*count<((*app)->len))
      {
         if (res == 1)
         {
            (*app)->freq[0][i+*count]++;
         } else if (res == 2)
         {
            (*app)->freq[1][i+*count]++;
         } else if (res == 4)
         {
            (*app)->freq[2][i+*count]++;
         } else if (res == 8)
         {
            (*app)->freq[3][i+*count]++;
         } else
         {
            (*app)->freq[4][i+*count]++;
         }
         i++;
      }
   }
   SeqPortFree(spp);
   if (len+*count == (*app)->len)
   {
      *app = (*app)->next;
      *count = 0;
   } else
      *count = *count + len;
   return;
}

NLM_EXTERN FloatHi ACT_ScoreProfile(BioseqPtr bsp, Int4 pos, Uint1 strand, ACTProfilePtr app)
{
   Int4        i;
   Uint1       res;
   FloatHi     retval;
   SeqPortPtr  spp;

   retval = 0;
   if (bsp == NULL || app == NULL || pos < 0)
      return retval;
   if (pos + app->len-1 >= bsp->length)
      return retval;
   if (ISA_na(bsp->mol))
   {
      spp = SeqPortNew(bsp, pos, pos+app->len-1, strand, Seq_code_ncbi4na);
      if (spp == NULL)
         return retval;
      i = 0;
      while ((res = SeqPortGetResidue(spp)) != SEQPORT_EOF && i<app->len)
      {
         if (res == 1)
         {
            retval += app->freq[0][i];
         } else if (res == 2)
         {
            retval += app->freq[1][i];
         } else if (res == 4)
         {
            retval += app->freq[2][i];
         } else if (res == 8)
         {
            retval += app->freq[3][i];
         } else
         {
            retval += app->freq[4][i];
         }
         i++;
      }
      retval = retval / app->len;
      return retval;
   } else
   {
      spp = SeqPortNew(bsp, pos, pos+app->len-1, strand, Seq_code_ncbistdaa);
      if (spp == NULL)
         return retval;
      i = 0;
      while ((res = SeqPortGetResidue(spp)) != SEQPORT_EOF && i<app->len)
      {
         retval += app->freq[res][i];
         i++;
      }
      retval = retval / app->len;
      return retval;
   }
}

NLM_EXTERN void ACT_EstimateConfidence(ACTProfilePtr app)
{
   FloatHi  conf;
   Int4     i;
   Int4     j;
   Int4     max;
   Int4     numres;

   if (app == NULL)
      return;
   if (app->nuc)
      numres = ACT_NUCLEN;
   else
      numres = ACT_PROTLEN;
   conf = 1;
   while (app)
   {
      for (i=0; i<app->len; i++)
      {
         max = 0;
         for (j=0; j<numres; j++)
         {
            if (app->freq[j][i] > max)
               max = app->freq[j][i];
         }
         if (max > 0)
            conf = conf*max;
         if (conf > INT4_MAX)
            conf = INT4_MAX;
      }
      app->confidence = conf;
      app = app->next;
   }
   return;
}

NLM_EXTERN ACTProfilePtr ACT_SortProfilesByConfidence(ACTProfilePtr app)
{
   ACTProfilePtr  app_head;
   ACTProfilePtr  PNTR array;
   Int4           count;
   Int4           i;

   if (app == NULL)
      return NULL;
   app_head = app;
   count = 0;
   while (app != NULL)
   {
      count++;
      app = app->next;
   }
   array = (ACTProfilePtr PNTR)MemNew(count*sizeof(ACTProfilePtr));
   app = app_head;
   count = 0;
   while (app != NULL)
   {
      array[count] = app;
      count++;
      app = app->next;
   }
   HeapSort((Pointer)array, (size_t)count, sizeof(ACTProfilePtr), ACT_CompareProfileConfidence);
   app_head = app = array[0];
   for (i=1; i<count; i++)
   {
      app->next = array[i];
      app = app->next;
   }
   return app_head;
}

NLM_EXTERN int LIBCALLBACK ACT_CompareProfileConfidence(VoidPtr base, VoidPtr large_son)
{
   ACTProfilePtr  app1;
   ACTProfilePtr  app2;

   app1 = *((ACTProfilePtr PNTR) base);
   app2 = *((ACTProfilePtr PNTR) large_son);
   if (app1 == NULL || app2 == NULL)
      return 0;
   if (app1->confidence > app2->confidence)
      return -1;
   else if (app1->confidence < app2->confidence)
      return 1;
   else
      return 0;
}

NLM_EXTERN ACTProfilePtr ACT_MakeProfileFromSA(SeqAlignPtr sap)
{
   AMAlignIndex2Ptr  amaip;
   AlnMsg2Ptr        amp;
   ACTProfilePtr    app;
   ACTProfilePtr    app_head;
   ACTProfilePtr    app_prev;
   BioseqPtr        bsp;
   Int4             count;
   Int4             i;
   Int4             j;
   Boolean          more;
   Boolean          nuc;
   Int4             numrows;
   SeqIdPtr         sip;
   SeqLocPtr        slp;

   if (sap == NULL)
      return NULL;
   if (sap->saip == NULL)
      return NULL;
   if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return NULL;
   }
   sip = AlnMgr2GetNthSeqIdPtr(sap, 1);
   bsp = BioseqLockById(sip);
   if (bsp == NULL)
      return NULL;
   if (ISA_na(bsp->mol))
      nuc = TRUE;
   else
      nuc = FALSE;
   BioseqUnlockById(sip);
   sip = SeqIdFree(sip);
   amp = AlnMsgNew2();
   amp->to_aln = -1;
   amp->row_num = 1;
   app_head = NULL;
/*
   if (sap->saip->indextype == INDEX_PARENT)
   {
      for (i=0; i<amaip->numseg; i++)
      {
         app = ACT_ProfileNew(nuc);
         app->len = amaip->lens[i];
         if (nuc)
         {
            for (j=0; j<ACT_NUCLEN; j++)
            {
               app->freq[j] = (FloatHiPtr)MemNew(app->len*sizeof(FloatHi));
            }
         } else
         {
            for (j=0; j<ACT_PROTLEN; j++)
            {
               app->freq[j] = (FloatHiPtr)MemNew(app->len*sizeof(FloatHi));
            }
         }
         if (app_head != NULL)
         {
            app_prev->next = app;
            app_prev = app;
         } else
            app_head = app_prev = app;
      }
   } else
   {
*/
      while ((Boolean) (more = AlnMgr2GetNextAlnBit(sap, amp)))
      {
         app = ACT_ProfileNew(nuc);
         app->len = amp->to_row - amp->from_row + 1;;
         if (nuc)
         {
            for (j=0; j<ACT_NUCLEN; j++)
            {
               app->freq[j] = (FloatHiPtr)MemNew(app->len*sizeof(FloatHi));
            }
         } else
         {
            for (j=0; j<ACT_PROTLEN; j++)
            {
               app->freq[j] = (FloatHiPtr)MemNew(app->len*sizeof(FloatHi));
            }
         }
         if (app_head != NULL)
         {
            app_prev->next = app;
            app_prev = app;
         } else
            app_head = app_prev = app;
      }
/*
   }
*/
   numrows = AlnMgr2GetNumRows(sap);
   for (i=1; i<=numrows; i++)
   {
      AlnMsgReNew2(amp);
      amp->to_aln = -1;
      amp->row_num = i;
      app = app_head;

      sip = AlnMgr2GetNthSeqIdPtr(sap, i);
      bsp = BioseqLockById(sip);
      count = 0;
      while ((Boolean) (more = AlnMgr2GetNextAlnBit(sap, amp)) && app != NULL)
      {
         if (amp->type == AM_SEQ && bsp != NULL)
         {
            slp = SeqLocIntNew(amp->from_row, amp->to_row, amp->strand, sip);
            ACT_BuildProfile(slp, &app, &count, 0);
            SeqLocFree(slp);
         } else if (amp->type == AM_GAP)
            ACT_BuildProfile(NULL, &app, &count, (amp->to_row - amp->from_row + 1));
      }
      BioseqUnlockById(sip);
      sip = SeqIdFree(sip);
   }
   ACT_EstimateConfidence(app_head);
   AlnMsgFree2(amp);
   return app_head;
}

NLM_EXTERN ACT_TopScorePtr PNTR ACT_SortAndTruncate(ACT_TopScorePtr PNTR ats)
{
   return NULL;
}

NLM_EXTERN ACT_PositionPtr ACT_PlaceByScore(ACT_PlaceBoundsPtr abp)
{
   Int4             i;
   FloatHi          score;

   score = ACT_CalcScore(abp);
   if (score > abp->apos->score)
   {
      abp->apos->score = score;
      for (i=0; i<abp->nprof; i++)
      {
         abp->apos->posarray[i] = abp->boundarray[i];
      }
   }
   while (abp->currpos[abp->currprof] < abp->numats[abp->currprof] - 1)
   {
      abp->currpos[abp->currprof]++;
      abp->currats[abp->currprof] = abp->currats[abp->currprof]->next;
      abp->boundarray[abp->currprof] = abp->currats[abp->currprof]->pos;
      score = ACT_CalcScore(abp);
      if (score > abp->apos->score)
      {
         abp->apos->score = score;
         for (i=0; i<abp->nprof; i++)
         {
            abp->apos->posarray[i] = abp->boundarray[i];
         }
      }
   }
   while(abp->currpos[abp->currprof] >= abp->numats[abp->currprof]-1 && abp->currprof >= 0)
   {
      abp->currprof--;
   }
   if (abp->currprof < 0)
      return (abp->apos);
   for (i=abp->currprof+1; i<abp->nprof; i++)
   {
      abp->currpos[i] = 0;
      abp->currats[i] = abp->ats[i];
      abp->boundarray[i] = abp->currats[i]->pos;
      while (abp->boundarray[i] <= abp->boundarray[i-1])
      {
         if (abp->currpos[abp->currprof] >= abp->numats[abp->currprof]-1)
            return (abp->apos);
         abp->currpos[i]+=1;
         abp->currats[i] = abp->currats[i]->next;
         abp->boundarray[i] = abp->currats[i]->pos;
      }
   }
   abp->currpos[abp->currprof]++;
   abp->currats[abp->currprof] = abp->currats[abp->currprof]->next;
   abp->boundarray[abp->currprof] = abp->currats[abp->currprof]->pos;
   abp->currprof = abp->nprof-1;
   abp->apos = ACT_PlaceByScore(abp);
   return (abp->apos);
}

NLM_EXTERN FloatHi ACT_CalcScore(ACT_PlaceBoundsPtr abp)
{
   ACTProfilePtr    app;
   ACT_TopScorePtr  ats;
   Int4             i;
   Int4             j;
   FloatHi          score;

   app = abp->app;
   score = 0;
   for (i=1; i<abp->nprof; i++)
   {
      if (app == NULL)
         return 0;
      if (abp->boundarray[i-1] + app->len >= abp->boundarray[i])
         return 0;
      j=0;
      ats = abp->ats[i-1];
      while (j<abp->currpos[i-1])
      {
         if (ats == NULL)
            return 0;
         ats = ats->next;
      }
      score += app->confidence*ats->score;
      app = app->next;
   }
   return score;
}

NLM_EXTERN ACT_TopScorePtr ACT_FindPeakScores(FloatHiPtr scorearray, Int4 len)
{
   ACT_TopScorePtr  ats;
   ACT_TopScorePtr  ats_head;
   ACT_TopScorePtr  ats_new;
   ACT_TopScorePtr  ats_newhead;
   ACT_TopScorePtr  ats_newprev;
   ACT_TopScorePtr  ats_prev;
   FloatHi          diff;
   FloatHi          diff_prev;
   Int4             i;

   if (scorearray == NULL)
      return NULL;
   diff = diff_prev = 0;
   diff_prev = scorearray[1] - scorearray[0];
   ats_head = NULL;
   for (i=1; i<len-1; i++)
   {
      diff = scorearray[i+1]-scorearray[i];
      if (diff < 0 && diff_prev >= 0) /* peak */
      {
         ats = (ACT_TopScorePtr)MemNew(sizeof(ACT_TopScore));
         ats->score = scorearray[i-1];
         ats->pos = i-1;
         if (ats_head != NULL)
         {
            ats_prev->next = ats;
            ats_prev = ats;
         } else
            ats_head = ats_prev = ats;
      }
      diff_prev = diff;
   }
   ats = ats_prev = ats_head;
   ats = ats->next;
   ats_newhead = NULL;
   diff_prev = 0;
   while (ats)
   {
      diff = ats->score - ats_prev->score;
      if (diff < 0 && diff_prev >= 0)
      {
         ats_new = (ACT_TopScorePtr)MemNew(sizeof(ACT_TopScore));
         ats_new->score = ats_prev->score;
         ats_new->pos = ats_prev->pos;
         if (ats_newhead != NULL)
         {
            ats_newprev->next = ats_new;
            ats_newprev = ats_new;
         } else
            ats_newhead = ats_newprev = ats_new;
      }
      diff_prev = diff;
      ats_prev = ats;
      ats = ats->next;
   }
   ats_prev = ats_head;
   while (ats_prev)
   {
      ats = ats_prev->next;
      ats_prev->next = NULL;
      MemFree(ats_prev);
      ats_prev = ats;
   }
   return ats_newhead;
}

static FloatHi act_get_eval(Int4 exp)
{
  FloatHi eval;
  Int4 i;

  eval = 1;
  for (i=1; i<=exp; i++)
  {
     eval = eval/10;
  }
  return eval;
}

static Boolean am_isa_gap(Int4 start, Int4 prevstop, Uint1 strand)
{
   if (strand != Seq_strand_minus)
   {
      if (start > prevstop+1)
         return TRUE;
      else
         return FALSE;
   } else
   {
      if (prevstop > start+1)
         return TRUE;
      else
         return FALSE;
   }
}

static void am_fix_strand(SeqAlignPtr sap, Uint1 strand1, Uint1 strand2)
{
   DenseSegPtr  dsp;
   Int4         i;

   if (sap == NULL || strand1 == 0 || strand2 == 0)
      return;
   if (sap->segtype != SAS_DENSEG)
      return;
   dsp = (DenseSegPtr)(sap->segs);
   if (dsp->dim != 2)
      return;
   for (i=0; i<dsp->numseg; i++)
   {
      dsp->strands[i*2] = strand1;
      dsp->strands[(i*2) + 1] = strand2;
   }
   return;
}

typedef struct sq_spin {
   Int4  n1;
   Int4  n2;
   Int4  n3;
   Int4  n4;
   Int4  n5;
} SQN_n, PNTR SQN_nPtr;

#define SQN_LEFT    1
#define SQN_RIGHT   2
#define SQN_MIDDLE  3

#define SQN_MAXGAP  4

#define SQN_WINDOW  30 /* window in which to search for missing pieces */

static int LIBCALLBACK ACT_CompareSpins (VoidPtr ptr1, VoidPtr ptr2)
{
   SQN_nPtr  spin1;
   SQN_nPtr  spin2;

   spin1 = *((SQN_nPtr PNTR) ptr1);
   spin2 = *((SQN_nPtr PNTR) ptr2);
   if (spin1 == NULL || spin2 == NULL)
      return 0;
   if (spin1->n3 > spin2->n3)
      return -1;
   if (spin1->n3 < spin2->n3)
      return 1;
   if (spin1->n2 < spin2->n2)
      return -1;
   if (spin1->n2 > spin2->n2)
      return 1;
   return 0;
}

static void ACT_RemoveInconsistentAlnsFromSet (SeqAlignPtr sap, Int4 fuzz, Int4 n)
{
   AMAlignIndex2Ptr  amaip;
   Boolean          conflict;
   Int4             curr;
   Int4             i;
   Int4             indextype;
   SeqAlignPtr      salp;
   SeqAlignPtr      salp_head;
   SeqAlignPtr      salp_prev;
   SQN_nPtr         PNTR spin;
   Int4             start;
   Int4             stop;
   Int4             strand;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return;
   if (n > 2)
      return;
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   indextype = amaip->alnstyle;
   /* make sure that everything is on the plus strand of the nth sequence */
   for (i=0; i<amaip->numsaps; i++)
   {
      salp = amaip->saps[i];
      strand = AlnMgr2GetNthStrand(salp, n);
      if (strand == Seq_strand_minus)
      {
         SAIndex2Free2(salp->saip);
         salp->saip = NULL;
         salp->next = NULL;
         SeqAlignListReverseStrand(salp);
         AlnMgr2IndexSingleChildSeqAlign(salp);
      }
   }
   /* spin structure: n1 = which alignment, n2 = start on first row, n3 =
      alignment length on 1st row, n4 = start on 2nd row, n5 = 2nd strand */
   spin = (SQN_nPtr PNTR)MemNew((amaip->numsaps)*sizeof(SQN_nPtr));
   for (i=0; i<amaip->numsaps; i++)
   {
      spin[i] = (SQN_nPtr)MemNew(sizeof(SQN_n));
      salp = amaip->saps[i];
      spin[i]->n1 = i;
      AlnMgr2GetNthSeqRangeInSA(salp, n, &start, &stop);
      spin[i]->n3 = stop - start;
      spin[i]->n2 = start;
      AlnMgr2GetNthSeqRangeInSA(salp, 3-n, &start, &stop);
      spin[i]->n4 = start;
      strand = AlnMgr2GetNthStrand(salp, 3-n);
      if (strand == Seq_strand_minus)
         spin[i]->n5 = -1;
      else
         spin[i]->n5 = 1;
   }
   HeapSort((Pointer)spin, (size_t)(amaip->numsaps), sizeof(SQN_nPtr), ACT_CompareSpins);
   strand = spin[0]->n5;
   for (i=1; i<amaip->numsaps; i++)
   {
      if (spin[i]->n5 != strand)
      {
         salp = amaip->saps[spin[i]->n1];
         salp->next = NULL;
         SeqAlignFree(salp);
         amaip->saps[spin[i]->n1] = NULL;
         spin[i]->n1 = -1;
      }
   }
   for (curr=0; curr<amaip->numsaps; curr++)
   {
      if (spin[curr]->n1 != -1)
      {
         for (i=curr+1; i<amaip->numsaps; i++)
         {
            if (spin[i]->n1 != -1)
            {
               conflict = FALSE;
            /* check first for conflict on first row */
               if (spin[i]->n2 + spin[i]->n3 - 1 > spin[curr]->n2 + fuzz)
               {
                  if (spin[i]->n2 < spin[curr]->n2)
                     conflict = TRUE;
               }
               if (spin[i]->n2 < spin[curr]->n2 + spin[curr]->n3 - 1 - fuzz)
               {
                  if (spin[i]->n2 + spin[i]->n3 - 1 > spin[curr]->n2 + spin[curr]->n3 - 1)
                     conflict = TRUE;
               }
               if (spin[i]->n2 >= spin[curr]->n2)
               {
                  if (spin[i]->n2 + spin[i]->n3 - 1 <= spin[curr]->n2 + spin[curr]->n3 - 1)
                     conflict = TRUE;
               }
            /* then check for conflict and consistency on second row */
               if (spin[i]->n2 + spin[i]->n3 - 1 < spin[curr]->n2 + fuzz)
               {
                  if (strand == 1)
                  {
                     if (spin[i]->n4 + spin[i]->n3 - 1 > spin[curr]->n4 + fuzz)
                        conflict = TRUE;
                  } else if (strand == -1)
                  {
                     if (spin[curr]->n4 + spin[curr]->n3 - 1 - fuzz > spin[i]->n4)
                        conflict = TRUE;
                  }
               } else
               {
                  if (strand == 1)
                  {
                     if (spin[i]->n4 < spin[curr]->n4 + spin[curr]->n3 - fuzz)
                        conflict = TRUE;
                  } else if (strand == -1)
                  {
                     if (spin[i]->n4 + spin[i]->n3 - 1 - fuzz > spin[curr]->n4)
                        conflict = TRUE;
                  }
               }
               if (conflict)
               {
                  salp = amaip->saps[spin[i]->n1];
                  salp->next = NULL;
                  SeqAlignFree(salp);
                  amaip->saps[spin[i]->n1] = NULL;
                  spin[i]->n1 = -1;
               }
            }
         }
      }
   }
   salp_head = salp_prev = NULL;
   for (i=0; i<amaip->numsaps; i++)
   {
      MemFree(spin[i]);
      if (amaip->saps[i] != NULL)
      {
         amaip->saps[i]->next = NULL;
         if (salp_prev != NULL)
         {
            salp_prev->next = amaip->saps[i];
            salp_prev = salp_prev->next;
         } else
            salp_head = salp_prev = amaip->saps[i];
      }
   }
   sap->segs = (Pointer)(salp_head);
   if (indextype == AM2_LITE)
   {
      AMAlignIndex2Free2(sap->saip);
      sap->saip = NULL;
      AlnMgr2IndexLite(sap);
   } else
      AlnMgr2ReIndexSeqAlign(sap);
   MemFree(spin);
}

NLM_EXTERN void ACT_GetNthSeqRangeInSASet(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop)
{
   SeqAlignPtr  salp;
   Int4         start_tmp;
   Int4         stop_tmp;
   Int4         tmp1;
   Int4         tmp2;
  
   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return;
   salp = (SeqAlignPtr)(sap->segs);
   start_tmp = stop_tmp = -1;
   while (salp != NULL)
   {
      if (n > salp->dim)
      {
         if (start)
            *start = -1;
         if (stop)
            *stop = -1;
         return;
      }
      AlnMgr2GetNthSeqRangeInSA(salp, n, &tmp1, &tmp2);
      if (tmp1 < start_tmp || start_tmp == -1)
         start_tmp = tmp1;
      if (tmp2 > stop_tmp)
         stop_tmp = tmp2;
      salp = salp->next;
   }
   if (start)
      *start = start_tmp;
   if (stop)
      *stop = stop_tmp;
}

static SeqAlignPtr ACT_FindBestAlnByDotPlot(SeqLocPtr slp1, SeqLocPtr slp2)
{
   DOTDiagPtr      ddp;
   DenseSegPtr     dsp;
   Int4            i;
   DOTMainDataPtr  mip;
   SeqAlignPtr     sap;
   SeqAlignPtr     sap_head;
   SeqAlignPtr     sap_prev;
   ScorePtr        scp;
   Int4            start1;
   Int4            start2;
   Uint1           strand;

   ErrSetMessageLevel(SEV_MAX);
   mip = DOT_CreateAndStorebyLoc (slp1, slp2, 6, 10);
   ErrSetMessageLevel(SEV_WARNING);
   sap = sap_head = sap_prev = NULL;
   if (mip == NULL || mip->hitlist == NULL)
      return NULL;
   i = 0;
   ddp = mip->hitlist[i];
   start1 = SeqLocStart(slp1);
   start2 = SeqLocStart(slp2);
   strand = SeqLocStrand(slp2);
   /* copy each ddp (a single ungapped alignment) into a one-segment dense-seg alignment */
   while (ddp != NULL && i < mip->index)
   {
      ddp = mip->hitlist[i];
      i++;
      sap = SeqAlignNew();
      dsp = DenseSegNew();
      sap->type = SAT_PARTIAL;
      sap->segtype = SAS_DENSEG;
      sap->dim = 2;
      dsp->dim = 2;
      dsp->numseg = 1;
      dsp->ids = SeqIdDup(SeqLocId(slp1));
      dsp->ids->next = SeqIdDup(SeqLocId(slp2));
      dsp->strands = (Uint1Ptr)MemNew(2*sizeof(Uint1));
      dsp->strands[0] = SeqLocStrand(slp1);
      dsp->strands[1] = SeqLocStrand(slp2);
      dsp->starts = (Int4Ptr)MemNew(2*sizeof(Int4));
      dsp->lens = (Int4Ptr)MemNew(sizeof(Int4));
      dsp->starts[0] = ddp->q_start;
      if (dsp->strands[1] == Seq_strand_minus)
         dsp->starts[1] = ddp->s_start - ddp->length + 1;
      else
         dsp->starts[1] = ddp->s_start;
      if (ddp->length > SeqLocLen(slp2))
         dsp->lens[0] = SeqLocLen(slp2);
      else
         dsp->lens[0] = ddp->length - 1;
      scp = ScoreNew();
      scp->id = ObjectIdNew();
      scp->id->str = StringSave("score");
      scp->choice = 1;
      scp->value.intvalue = ddp->score;
      dsp->scores = scp;
      sap->segs = (Pointer)(dsp);
      if (sap_head != NULL)
      {
         sap_prev->next = sap;
         sap_prev = sap;
      } else
         sap_head = sap_prev = sap;
   }
   if (sap_head == NULL)
      return NULL;
   AlnMgr2IndexLite(sap_head);
   ACT_RemoveInconsistentAlnsFromSet(sap_head, 6, 1);
   sap = (SeqAlignPtr)(sap_head->segs);
   sap_head->segs = NULL;
   SeqAlignFree(sap_head);
   MemFree(mip->matrix);
   MemFree(mip->qseq);
   MemFree(mip->sseq);
   MemFree(mip->qname);
   MemFree(mip->sname);
   i = 0;
   while (ddp != NULL && i < mip->index)
   {
      ddp = mip->hitlist[i];
      MemFree(ddp);
      i++;
   }
   MemFree(mip->hitlist);
   return sap;
}

NLM_EXTERN SeqAlignPtr ACT_FindPiece(BioseqPtr bsp1, BioseqPtr bsp2, Int4 start1, Int4 stop1, Int4 start2, Int4 stop2, Uint1 strand, Int4 which_side)
{
   AMAlignIndex2Ptr      amaip;
   DenseSegPtr          dsp;
   Int4                 i;
   Int4                 nstart1;
   Int4                 nstart2;
   Int4                 nstop1;
   Int4                 nstop2;
   BLAST_OptionsBlkPtr  options;
   CharPtr              program;
   SeqAlignPtr          sap;
   SeqAlignPtr          sap_head;
   SeqAlignPtr          sap_new;
   SeqAlignPtr          sap_prev = NULL;
   SeqLocPtr            slp1;
   SeqLocPtr            slp2;

   if (stop1 - start1 < 7 || stop2 - start2 < 7) /* can't do these by BLAST -- wordsize can't go that small */
      return NULL;
   if (ISA_aa(bsp1->mol))
   {
      if (ISA_aa(bsp2->mol))
         program = StringSave("blastp");
      else
         return NULL;
   } else if (ISA_na(bsp1->mol))
   {
      if (ISA_na(bsp2->mol))
         program = StringSave("blastn");
      else
         return NULL;
   }
   options = BLASTOptionNew(program, TRUE);
   options->gapped_calculation = TRUE;
   options->expect_value = 10;
   options->gap_x_dropoff_final = 100;
   options->gap_open = 5;
   options->gap_extend = 1;
   options->penalty = -1;
   options->wordsize = 7;
   slp1 = SeqLocIntNew(start1, stop1, Seq_strand_plus, bsp1->id);
   slp2 = SeqLocIntNew(start2, stop2, strand, bsp2->id);
   sap = BlastTwoSequencesByLoc(slp1, slp2, program, options);
   BLASTOptionDelete(options);
   MemFree(program);
   if (sap == NULL)
   {
      sap = SeqAlignNew();
      dsp = DenseSegNew();
      dsp->numseg = 1;
      dsp->starts = (Int4Ptr)MemNew(2*sizeof(Int4));
      dsp->lens = (Int4Ptr)MemNew(sizeof(Int4));
      dsp->strands = (Uint1Ptr)MemNew(2*sizeof(Uint1));
      dsp->dim = 2;
      dsp->ids = SeqIdDup(bsp1->id);
      dsp->ids->next = SeqIdDup(bsp2->id);
      dsp->lens[0] = MIN(stop1-start1+1, stop2-start2+1);
      dsp->strands[0] = dsp->strands[1] = Seq_strand_plus;
      if (which_side == SQN_LEFT || which_side == SQN_MIDDLE)
      {
         dsp->starts[0] = stop1 - dsp->lens[0] + 1;
         dsp->starts[1] = stop2 - dsp->lens[0] + 1;
      } else if (which_side == SQN_RIGHT)
      {
         dsp->starts[0] = start1;
         dsp->starts[1] = start2;
      }
      sap->segs = (Pointer)dsp;
      sap->segtype = SAS_DENSEG;
      return sap;
   }
   SeqLocFree(slp1);
   SeqLocFree(slp2);
   if (sap == NULL)
      return NULL;
   AlnMgr2IndexLite(sap);
   ACT_RemoveInconsistentAlnsFromSet(sap, 20, 1);
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   AlnMgr2SortAlnSetByNthRowPos(sap, 1);
   ACT_GetNthSeqRangeInSASet(sap, 1, &nstart1, &nstop1);
   ACT_GetNthSeqRangeInSASet(sap, 2, &nstart2, &nstop2);
   strand = AlnMgr2GetNthStrand(amaip->saps[0], 2);
   sap_head = NULL;
   if (strand != Seq_strand_minus)
   {
      if (nstart1 > start1+20 && nstart2 > start2+20)
      {
         slp1 = SeqLocIntNew(start1, nstart1, Seq_strand_plus, bsp1->id);
         slp2 = SeqLocIntNew(start2, nstart2, strand, bsp2->id);
         sap_head = ACT_FindBestAlnByDotPlot(slp1, slp2);
         SeqLocFree(slp1);
         SeqLocFree(slp2);
      }
   } else
   {
      if (nstart1 > start1+20 && nstop2 < stop2 - 20)
      {
         slp1 = SeqLocIntNew(start1, nstart1, Seq_strand_plus, bsp1->id);
         slp2 = SeqLocIntNew(nstop2, stop2, strand, bsp2->id);
         sap_head = ACT_FindBestAlnByDotPlot(slp1, slp2);
         SeqLocFree(slp1);
         SeqLocFree(slp2);
      }
   }
   for (i=0; i<amaip->numsaps-1; i++)
   {
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 1, NULL, &nstart1);
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i+1], 1, &nstop1, NULL);
      if (strand != Seq_strand_minus)
      {
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 2, NULL, &nstart2);
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i+1], 2, &nstop2, NULL);
      } else
      {
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 2, &nstop2, NULL);
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i+1], 2, NULL, &nstart2);
      }
   }
   ACT_GetNthSeqRangeInSASet(sap, 1, &nstart1, &nstop1);
   ACT_GetNthSeqRangeInSASet(sap, 2, &nstart2, &nstop2);
   sap_prev = sap_head;
   if (sap_prev != NULL)
   {
      while (sap_prev->next != NULL)
      {
         sap_prev = sap_prev->next;
      }
   }
   if (strand != Seq_strand_minus)
   {
      if (nstop1 < stop1-20 && nstop2 < stop2-20)  /* missing piece at the end */
      {
         slp1 = SeqLocIntNew(nstop1, stop1, Seq_strand_plus, bsp1->id);
         slp2 = SeqLocIntNew(nstop2, stop2, strand, bsp2->id);
         sap_new = ACT_FindBestAlnByDotPlot(slp1, slp2);
         SeqLocFree(slp1);
         SeqLocFree(slp2);
         if (sap_new != NULL)
         {
            if (sap_head != NULL)
            {
               sap_prev->next = sap_new;
               sap_prev = sap_new;
            } else
              sap_head = sap_prev = sap_new;
         }
      }
   } else
   {
      if (nstop1 < stop1-20 && nstart2 > start2 + 20)
      {
         slp1 = SeqLocIntNew(nstop1, stop1, Seq_strand_plus, bsp1->id);
         slp2 = SeqLocIntNew(start2, nstart2, strand, bsp2->id);
         sap_new = ACT_FindBestAlnByDotPlot(slp1, slp2);
         SeqLocFree(slp1);
         SeqLocFree(slp2);
         if (sap_new != NULL)
         {
            if (sap_head != NULL)
            {
               sap_prev->next = sap_new;
               sap_prev = sap_new;
            } else
               sap_head = sap_prev = sap_new;
         }
      }
   }
   sap_new = (SeqAlignPtr)(sap->segs);
   while (sap_new->next != NULL)
   {
      sap_new = sap_new->next;
   }
   sap_new->next = sap_head;
   sap_head = (SeqAlignPtr)(sap->segs);
   sap->segs = NULL;
   SeqAlignFree(sap);
   return sap_head;
}

static void ACT_ExtendAlnRight(SeqAlignPtr sap, Int4 which_row, Int4 start, Int4 stop)
{
   DenseSegPtr  dsp;
   Int4         i;
   Int4Ptr      lens;
   Int4Ptr      starts;
   Uint1Ptr     strands;
 
   if (sap == NULL)
      return;
   if (which_row > 2)
      return;
   dsp = (DenseSegPtr)(sap->segs);
   if (dsp->starts[2*(dsp->numseg-1) + which_row - 1] == -1 || dsp->starts[2*(dsp->numseg-1) + (2-which_row)] != -1)
   {
      starts = (Int4Ptr)MemNew((dsp->numseg+1)*2*sizeof(Int4));
      strands = (Uint1Ptr)MemNew((dsp->numseg+1)*2*sizeof(Uint1));
      lens = (Int4Ptr)MemNew((dsp->numseg+1)*sizeof(Int4));
      for (i=0; i<dsp->numseg; i++)
      {
         lens[i] = dsp->lens[i];
      }
      for (i=0; i<=(dsp->dim)*(dsp->numseg-1)+1; i++)
      {
         starts[i] = dsp->starts[i];
         strands[i] = dsp->strands[i];
      }
      lens[dsp->numseg] = stop - start + 1;
      if (dsp->strands[which_row-1] != Seq_strand_minus)
         starts[(dsp->dim)*(dsp->numseg) + which_row - 1] = start;
      else
         starts[(dsp->dim)*(dsp->numseg) + which_row - 1] = stop;
      starts[(dsp->dim)*(dsp->numseg) + (2-which_row)] = -1;
      strands[(dsp->dim)*(dsp->numseg) + which_row - 1] = dsp->strands[which_row-1];
      strands[(dsp->dim)*(dsp->numseg) + (2-which_row)] = dsp->strands[2-which_row];
      MemFree(dsp->starts);
      MemFree(dsp->lens);
      MemFree(dsp->strands);
      dsp->numseg++;
      dsp->starts = starts;
      dsp->strands = strands;
      dsp->lens = lens;
   } else
   {
      dsp->lens[dsp->numseg-1] += stop - start + 1;
      if (dsp->strands[which_row-1] == Seq_strand_minus)
         dsp->starts[(dsp->dim)*(dsp->numseg-1) + which_row - 1] = stop;
   }
   SAIndex2Free2(sap->saip);
   sap->saip = NULL;
   AlnMgr2IndexSingleChildSeqAlign(sap);
}

static SeqAlignPtr ACT_CreateContinuousAln(SeqAlignPtr PNTR saps, Int4 numsaps)
{
   DenseSegPtr  dsp;
   DenseSegPtr  dsp_tmp;
   Int4         i;
   Int4         j;
   Int4         n1;
   Int4         n2;
   Int4         numseg;
   SeqAlignPtr  salp;
   Int4         start1;
   Int4         start2;
   Int4         stop1;
   Int4         stop2;
   Uint1        strand;

   for (i=0; i<numsaps-1; i++)
   {
      AlnMgr2GetNthSeqRangeInSA(saps[i], 1, &start1, &stop1);
      AlnMgr2GetNthSeqRangeInSA(saps[i+1], 1, &start2, &stop2);
      if (start2 - stop1 > 1)
         ACT_ExtendAlnRight(saps[i], 1, stop1+1, start2-1);
      AlnMgr2GetNthSeqRangeInSA(saps[i], 2, &start1, &stop1);
      AlnMgr2GetNthSeqRangeInSA(saps[i+1], 2, &start2, &stop2);
      strand = AlnMgr2GetNthStrand(saps[i], 2);
      if (strand == Seq_strand_minus)
      {
         if (start1 - stop2 > 1)
            ACT_ExtendAlnRight(saps[i], 2, stop2+1, start1-1);
      } else
      {
         if (start2 - stop1 > 1)
            ACT_ExtendAlnRight(saps[i], 2, stop1+1, start2-1);
      }
   }
   numseg = 0;
   for (i=0; i<numsaps; i++)
   {
      dsp_tmp = (DenseSegPtr)(saps[i]->segs);
      numseg += dsp_tmp->numseg;
   }
   dsp = DenseSegNew();
   dsp->dim = 2;
   dsp->numseg = numseg;
   dsp->starts = (Int4Ptr)MemNew(2*numseg*sizeof(Int4));
   dsp->lens = (Int4Ptr)MemNew(numseg*sizeof(Int4));
   dsp->strands = (Uint1Ptr)MemNew(2*numseg*sizeof(Uint1));
   n1 = n2 = 0;
   for (i=0; i<numsaps; i++)
   {
      dsp_tmp = (DenseSegPtr)(saps[i]->segs);
      if (dsp->ids == NULL)
         dsp->ids = SeqIdDupList(dsp_tmp->ids);
      for (j=0; j<2*dsp_tmp->numseg; j++)
      {
         dsp->starts[n1+j] = dsp_tmp->starts[j];
         dsp->strands[n1+j] = dsp_tmp->strands[j];
      }
      for (j=0; j<dsp_tmp->numseg; j++)
      {
         dsp->lens[n2+j] = dsp_tmp->lens[j];
      }
      n1 += 2*dsp_tmp->numseg;
      n2 += dsp_tmp->numseg;
   }
   salp = SeqAlignNew();
   salp->type = SAT_PARTIAL;
   salp->segtype = SAS_DENSEG;
   salp->dim = 2;
   salp->segs = (Pointer)(dsp);
   AlnMgr2IndexSingleChildSeqAlign(salp);
   return salp;
}

NLM_EXTERN SeqAlignPtr ACT_CleanUpAlignments(SeqAlignPtr sap, Int4 len1, Int4 len2)
{
   AMAlignIndex2Ptr  amaip;
   Int4             diff;
   DenseSegPtr      dsp;
   Int4             i;
   Int4             numseg;
   Int4             start1;
   Int4             start2;
   Int4             stop1;
   Int4             stop2;
   Uint1            strand;
   Int4             tmp;

   if (sap == NULL)
      return NULL;
   AlnMgr2SortAlnSetByNthRowPos(sap, 1);
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   strand = AlnMgr2GetNthStrand(amaip->saps[0], 2);
   numseg = 0;
   AlnMgr2GetNthSeqRangeInSA(amaip->saps[0], 1, &start1, NULL);
   AlnMgr2GetNthSeqRangeInSA(amaip->saps[0], 2, &start2, &stop2);
   if (strand != Seq_strand_minus)
      diff = start2;
   else
      diff = len2 - stop2;
   if (start1 > 0 && diff > 0)
      numseg += 2;
   for (i=0; i<amaip->numsaps-1; i++)
   {
      dsp = (DenseSegPtr)(amaip->saps[i]->segs);
      numseg += dsp->numseg;
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 1, NULL, &start1);
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i+1], 1, &stop1, &tmp);
      if (stop1 < start1+1)
         AlnMgr2TruncateSAP(amaip->saps[i+1], start1+1, tmp, 1);
      if (strand != Seq_strand_minus)
      {
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 2, NULL, &start2);
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i+1], 2, &stop2, &tmp);
         if (stop2 < start2+1)
            AlnMgr2TruncateSAP(amaip->saps[i+1], start2+1, tmp, 2);
      } else
      {
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 2, &stop2, &tmp);
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i+1], 2, NULL, &start2);
         if (stop2 < start2 + 1)
            AlnMgr2TruncateSAP(amaip->saps[i], start2+1, tmp, 2);
      }
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 1, NULL, &start1);
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i+1], 1, &stop1, &tmp);
      if (strand != Seq_strand_minus)
      {
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 2, NULL, &start2);
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i+1], 2, &stop2, &tmp);
      } else
      {
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 2, &stop2, &tmp);
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i+1], 2, NULL, &start2);
      }
      if (stop1 > start1+1)
         numseg++;
      if (stop2 > start2+1)
         numseg++;
   }
   dsp = (DenseSegPtr)(amaip->saps[amaip->numsaps-1]->segs);
   numseg += dsp->numseg;
   AlnMgr2GetNthSeqRangeInSA(amaip->saps[amaip->numsaps-1], 1, NULL, &stop1);
   AlnMgr2GetNthSeqRangeInSA(amaip->saps[amaip->numsaps-1], 2, &start2, &stop2);
   if (strand != Seq_strand_minus)
      diff = len2 - stop2;
   else
      diff = start2;
   if (stop1 < len1 && diff > 0)
      numseg += 2;
   AlnMgr2SortAlnSetByNthRowPos(sap, 1);
   return (ACT_CreateContinuousAln(amaip->saps, amaip->numsaps));
}

static void SQN_AddToAln(SeqAlignPtr sap, Int4 offset, Int2 which_end, Uint1 strand)
{
   DenseSegPtr  dsp;
   Int4Ptr      lens;
   Int4         i;
   Int4         j;
   Int4Ptr      starts;
   Uint1Ptr     strands;

   if (sap == NULL || offset == 0)
      return;
   dsp = (DenseSegPtr)(sap->segs);
   if (which_end == SQN_LEFT)
   {
      if (dsp->starts[0] != -1 && dsp->starts[1] != -1) /* neither sequence is gapped */
      {
         dsp->starts[0] -= offset;
         if (strand != Seq_strand_minus)
            dsp->starts[1] -= offset;
         dsp->lens[0] += offset;
      } else /* one of the sequences is gapped -> add a new segment */
      {
         starts = (Int4Ptr)MemNew(2*(dsp->numseg+1)*sizeof(Int4));
         lens = (Int4Ptr)MemNew((dsp->numseg+1)*sizeof(Int4));
         strands = (Uint1Ptr)MemNew(2*(dsp->numseg+1)*sizeof(Uint1));
         AlnMgr2GetNthSeqRangeInSA(sap, 1, &i, &j);
         starts[0] = i - offset;
         AlnMgr2GetNthSeqRangeInSA(sap, 1, &i, &j);
         if (strand == Seq_strand_minus)
            starts[1] = j + 1;
         else
            starts[1] = i - offset;
         lens[0] = offset;
         strands[0] = Seq_strand_plus;
         strands[1] = strand;
         for (i=0; i<dsp->numseg; i++)
         {
           starts[i+1] = dsp->starts[i];
           starts[2*(i+1)] = dsp->starts[2*i];
           lens[i+1] = dsp->lens[i];
           strands[i+1] = dsp->strands[i];
           strands[2*(i+1)] = dsp->strands[2*i];
         }
         dsp->numseg++;
         MemFree(dsp->starts);
         MemFree(dsp->lens);
         MemFree(dsp->strands);
         dsp->starts = starts;
         dsp->lens = lens;
         dsp->strands = strands;
      }
   } else if (which_end == SQN_RIGHT)
   {
      if (dsp->starts[2*(dsp->numseg-1)] != -1 && dsp->starts[2*(dsp->numseg-1)+1] != -1)
      {
         dsp->lens[dsp->numseg-1] += offset;
         if (strand == Seq_strand_minus)
            dsp->starts[2*(dsp->numseg-1)+1] -= offset;
      } else /* one of the sequences is gapped -> add a new segment */
      {
         starts = (Int4Ptr)MemNew(2*(dsp->numseg+1)*sizeof(Int4));
         lens = (Int4Ptr)MemNew((dsp->numseg+1)*sizeof(Int4));
         strands = (Uint1Ptr)MemNew(2*(dsp->numseg+1)*sizeof(Uint1));
         AlnMgr2GetNthSeqRangeInSA(sap, 1, &i, &j);
         starts[2*(dsp->numseg)-1] = i+1;
         AlnMgr2GetNthSeqRangeInSA(sap, 2, &i, &j);
         if (strand == Seq_strand_minus)
           starts[2*(dsp->numseg)] = i - offset;
         else
           starts[2*(dsp->numseg)] = j + 1;
         lens[dsp->numseg] = offset;
         strands[2*(dsp->numseg)-1] = Seq_strand_plus;
         strands[2*(dsp->numseg)] = strand;
         for (i=0; i<dsp->numseg; i++)
         {
            starts[i] = dsp->starts[i];
            starts[2*i] = dsp->starts[2*i];
            lens[i] = dsp->lens[i];
            strands[i] = dsp->strands[i];
            strands[2*i] = dsp->strands[2*i];
         }
         dsp->numseg++;
         MemFree(dsp->starts);
         MemFree(dsp->lens);
         MemFree(dsp->strands);
         dsp->starts = starts;
         dsp->lens = lens;
         dsp->strands = strands;
      }
   }
   /* free the old index and reindex the alignment */
   SAIndex2Free2(sap->saip);
   sap->saip = NULL;
   AlnMgr2IndexSingleChildSeqAlign(sap);
}

NLM_EXTERN void SQN_ExtendAlnAlg(SeqAlignPtr sap, Int4 ovl, Int4 which_side, Uint1 strand)
{
   BioseqPtr    bsp1;
   BioseqPtr    bsp2;
   Uint1        buf1[20];
   Uint1        buf2[20];
   DenseSegPtr  dsp;
   DenseSegPtr  dsp_new;
   Int4         gap;
   Int4         i;
   Int4         j;
   Boolean      mismatch;
   SeqIdPtr     sip1;
   SeqIdPtr     sip2;
   SeqPortPtr   spp;
   Int4         start1;
   Int4         start2;
   Int4         stop1;
   Int4         stop2;

   if (sap == NULL || ovl == 0)
      return;
   AlnMgr2GetNthSeqRangeInSA(sap, 1, &start1, &stop1);
   AlnMgr2GetNthSeqRangeInSA(sap, 2, &start2, &stop2);
   sip1 = AlnMgr2GetNthSeqIdPtr(sap, 1);
   sip2 = AlnMgr2GetNthSeqIdPtr(sap, 2);
   bsp1 = BioseqLockById(sip1);
   bsp2 = BioseqLockById(sip2);
   if (which_side == SQN_LEFT && (start1<ovl+SQN_MAXGAP || start2<ovl+SQN_MAXGAP))
   {
      if (start1 < ovl)
         ovl = start1;
      if (start2 < ovl)
         ovl = start2;
      if (ovl <= 0) {
         SeqIdFree(sip1);
         SeqIdFree(sip2);
         BioseqUnlock(bsp1);
         BioseqUnlock(bsp2);
         return;
      }
      dsp = (DenseSegPtr)(sap->segs);
      dsp_new = DenseSegNew();
      dsp_new->dim = 2;
      dsp_new->numseg = dsp->numseg + 1;
      dsp_new->starts = (Int4Ptr)MemNew(2*(dsp_new->numseg)*sizeof(Int4));
      dsp_new->lens = (Int4Ptr)MemNew((dsp_new->numseg)*sizeof(Int4));
      dsp_new->strands = (Uint1Ptr)MemNew(2*(dsp_new->numseg)*sizeof(Int4));
      dsp_new->ids = dsp->ids;
      dsp->ids = NULL;
      dsp_new->starts[0] = start1-ovl;
      dsp_new->starts[1] = start2-ovl;
      dsp_new->lens[0] = ovl;
      dsp_new->strands[0] = dsp_new->strands[1] = Seq_strand_plus;
      for (i=1; i<dsp_new->numseg; i++)
      {
         dsp_new->lens[i] = dsp->lens[i-1];
         dsp_new->starts[i*dsp->dim] = dsp->starts[(i-1)*dsp->dim];
         dsp_new->starts[i*dsp->dim+1] = dsp->starts[(i-1)*dsp->dim+1];
         dsp_new->strands[i*dsp->dim] = dsp_new->strands[i*dsp->dim+1] = Seq_strand_plus;
      }
      DenseSegFree(dsp);
      sap->segs = (Pointer)(dsp_new);
      SAIndex2Free2(sap->saip);
      sap->saip = NULL;
      AlnMgr2IndexSingleChildSeqAlign(sap);
      SeqIdFree(sip1);
      SeqIdFree(sip2);
      BioseqUnlock(bsp1);
      BioseqUnlock(bsp2);
      return;
   } else if (which_side == SQN_RIGHT && (bsp1->length-1-stop1 < ovl+SQN_MAXGAP || bsp2->length-1-stop2<ovl+SQN_MAXGAP))
   {
      if (bsp1->length-1-stop1<ovl)
         ovl = bsp1->length-1-stop1;
      if (bsp2->length-1-stop2<ovl)
         ovl = bsp2->length-1-stop2;
      if (ovl <= 0) {
         SeqIdFree(sip1);
         SeqIdFree(sip2);
         BioseqUnlock(bsp1);
         BioseqUnlock(bsp2);
         return;
      }
      dsp = (DenseSegPtr)(sap->segs);
      dsp_new = DenseSegNew();
      dsp_new->dim = 2;
      dsp_new->numseg = dsp->numseg + 1;
      dsp_new->starts = (Int4Ptr)MemNew(2*(dsp_new->numseg)*sizeof(Int4));
      dsp_new->lens = (Int4Ptr)MemNew((dsp_new->numseg)*sizeof(Int4));
      dsp_new->strands = (Uint1Ptr)MemNew(2*(dsp_new->numseg)*sizeof(Int4));
      dsp_new->ids = dsp->ids;
      dsp->ids = NULL;
      for (i=0; i<dsp->numseg; i++)
      {
         dsp_new->lens[i] = dsp->lens[i];
         dsp_new->starts[i*dsp->dim] = dsp->starts[i*dsp->dim];
         dsp_new->starts[i*dsp->dim+1] = dsp->starts[i*dsp->dim+1];
         dsp_new->strands[i*dsp->dim] = dsp_new->strands[i*dsp->dim+1] = Seq_strand_plus;
      }
      dsp_new->lens[dsp_new->numseg-1] = ovl;
      dsp_new->starts[(dsp_new->numseg-1)*2] = stop1+1;
      dsp_new->starts[(dsp_new->numseg-1)*2+1] = stop2+1;
      dsp_new->strands[(dsp_new->numseg-1)*2] = dsp_new->strands[(dsp_new->numseg-1)*2+1] = Seq_strand_plus;
      DenseSegFree(dsp);
      sap->segs = (Pointer)dsp_new;
      SAIndex2Free2(sap->saip);
      sap->saip = NULL;
      AlnMgr2IndexSingleChildSeqAlign(sap);
      SeqIdFree(sip1);
      SeqIdFree(sip2);
      BioseqUnlock(bsp1);
      BioseqUnlock(bsp2);
      return;
   }
   if (which_side == SQN_LEFT)
   {
      spp = SeqPortNew(bsp1, MAX(0, start1-(SQN_MAXGAP+ovl)), start1-1, Seq_strand_plus, Seq_code_ncbi4na);
      SeqPortRead(spp, buf1, 20);
      SeqPortFree(spp);
      if (strand == Seq_strand_minus)
      {
         spp = SeqPortNew(bsp2, stop2+1, stop2 + 1 + ovl, strand, Seq_code_ncbi4na);
         SeqPortRead(spp, buf2, 20);
         SeqPortFree(spp);
      } else
      {
         spp = SeqPortNew(bsp2, start2-1-ovl, start2-1, strand, Seq_code_ncbi4na);
         SeqPortRead(spp, buf2, 20);
         SeqPortFree(spp);
      }
      gap = -1;
      for (i=0; i<SQN_MAXGAP; i++)
      {
         mismatch = FALSE;
         for (j=0; j<ovl; j++)
         {
            if (buf2[j] != buf1[i+j])
               mismatch = TRUE;
         }
         if (mismatch == FALSE)
            gap = SQN_MAXGAP-i;
      }
      if (gap > 0)
      {
         dsp = (DenseSegPtr)(sap->segs);
         dsp_new = DenseSegNew();
         dsp_new->dim = 2;
         dsp_new->numseg = dsp->numseg+2;
         dsp_new->ids = dsp->ids;
         dsp->ids = NULL;
         dsp_new->starts = (Int4Ptr)MemNew((dsp_new->numseg)*2*sizeof(Int4));
         dsp_new->lens = (Int4Ptr)MemNew((dsp_new->numseg)*sizeof(Int4));
         dsp_new->strands = (Uint1Ptr)MemNew((dsp_new->numseg)*2*sizeof(Uint1));
         for (i=2; i<dsp_new->numseg; i++)
         {
            dsp_new->starts[i*2] = dsp->starts[(i-2)*2];
            dsp_new->starts[i*2+1] = dsp->starts[(i-2)*2+1];
            dsp_new->strands[i+2] = dsp->strands[(i-2)*2];
            dsp_new->strands[i*2+1] = dsp->strands[(i-2)*2+1];
            dsp_new->lens[i] = dsp->lens[i-2];
         }
         dsp_new->starts[0] = dsp->starts[0] - gap - ovl;
         dsp_new->starts[2] = dsp_new->starts[0] + ovl;
         dsp_new->starts[3] = -1;
         dsp_new->strands[0] = dsp_new->strands[2] = dsp->strands[0];
         dsp_new->strands[1] = dsp_new->strands[2] = dsp->strands[1];
         dsp_new->lens[0] = ovl;
         dsp_new->lens[1] = gap;
         if (strand == Seq_strand_minus)
            dsp_new->starts[1] = stop2 + 1;
         else
            dsp_new->starts[1] = start2 - ovl;
         sap->segs = (Pointer)dsp_new;
         DenseSegFree(dsp);
         SAIndex2Free2(sap->saip);
         sap->saip = NULL;
         AlnMgr2IndexSingleChildSeqAlign(sap);
      } else
         SQN_AddToAln(sap, ovl, SQN_LEFT, strand);
   } else if (which_side == SQN_RIGHT)
   {
      spp = SeqPortNew(bsp1, MIN(stop1, bsp1->length-1), MIN(stop1+SQN_MAXGAP, bsp1->length-1), Seq_strand_plus, Seq_code_ncbi4na);
      SeqPortRead(spp, buf1, 20);
      SeqPortFree(spp);
      if (strand == Seq_strand_minus)
      {
         spp = SeqPortNew(bsp2, start2-ovl, start2-1-ovl, strand, Seq_code_ncbi4na);
         SeqPortRead(spp, buf2, 20);
         SeqPortFree(spp);
      } else
      {
         spp = SeqPortNew(bsp2, MIN(stop2+1, bsp2->length-1), MIN(stop2+1+ovl, bsp2->length-1), strand, Seq_code_ncbi4na);
         SeqPortRead(spp, buf2, 20);
         SeqPortFree(spp);
      }
      gap = -1;
      for (i=0; i<SQN_MAXGAP; i++)
      {
         mismatch = FALSE;
         for (j=0; j<ovl; j++)
         {
            if (buf1[i+j] != buf1[j])
               mismatch = TRUE;
         }
         if (mismatch == FALSE && gap == -1)
            gap = i;
      }
      if (gap > 0)
      {
         dsp = (DenseSegPtr)(sap->segs);
         dsp_new = DenseSegNew();
         dsp_new->dim = 2;
         dsp_new->numseg = dsp->numseg+2;
         dsp_new->ids = dsp->ids;
         dsp->ids = NULL;
         dsp_new->starts = (Int4Ptr)MemNew((dsp_new->numseg)*2*sizeof(Int4));
         dsp_new->lens = (Int4Ptr)MemNew((dsp_new->numseg)*sizeof(Int4));
         dsp_new->strands = (Uint1Ptr)MemNew((dsp_new->numseg)*2*sizeof(Uint1));
         for (i=0; i<dsp->numseg; i++)
         {
            dsp_new->starts[i*2] = dsp->starts[i*2];
            dsp_new->starts[i*2+1] = dsp->starts[i*2+1];
            dsp_new->strands[i+2] = dsp->strands[i*2];
            dsp_new->strands[i*2+1] = dsp->strands[i*2+1];
            dsp_new->lens[i] = dsp->lens[i];
         }
         dsp_new->strands[2*dsp->numseg] = dsp_new->strands[2*dsp->numseg+2] = dsp->strands[0];
         dsp_new->strands[2*dsp->numseg+1] = dsp_new->strands[2*dsp->numseg+3] = dsp->strands[1];
         dsp_new->lens[dsp->numseg] = gap;
         dsp_new->lens[dsp->numseg+1] = ovl;
         dsp_new->starts[2*dsp->numseg] = dsp_new->starts[2*(dsp->numseg-1)]+dsp_new->lens[dsp->numseg-1];
         dsp_new->starts[2*dsp->numseg+2] = dsp_new->starts[2*dsp->numseg] + gap;
         if (strand == Seq_strand_minus)
            dsp_new->starts[2*dsp->numseg+3] = start2 - ovl;
         else
            dsp_new->starts[2*dsp->numseg+3] = stop2 + 1;
         dsp_new->starts[2*dsp->numseg+1] = -1;
         sap->segs = (Pointer)dsp_new;
         DenseSegFree(dsp);
         SAIndex2Free2(sap->saip);
         sap->saip = NULL;
         AlnMgr2IndexSingleChildSeqAlign(sap);
      } else
         SQN_AddToAln(sap, ovl, SQN_RIGHT, strand);
   }
   SeqIdFree(sip1);
   SeqIdFree(sip2);
   BioseqUnlock(bsp1);
   BioseqUnlock(bsp2);
}

static Uint4 sqn_binary_search_on_uint4_list(Uint4Ptr list, Uint4 pos, Uint4 listlen)
{
   Uint4  L;
   Uint4  mid;
   Uint4  R;

   if (list == NULL || listlen == 0)
      return 0;
   L = 0;
   R = listlen - 1;
   while (L < R)
   {
      mid = (L+R)/2;
      if (list[mid + 1] <= pos)
      {
         L = mid + 1;
      } else
      {
         R = mid;
      }
   }
   return R;
}

static Int4 MapRowCoordsSpecial(SeqAlignPtr sap, Uint4 pos, Int4 row, Int4 which_end)
{
   DenseSegPtr  dsp;
   Int4         idx;
   Int4         offset;
   SAIndex2Ptr   saip;
   Int4         start;

   if (sap == NULL || row < 0)
      return -1;
   if (sap->saip == NULL)
      return -1;
   saip = (SAIndex2Ptr)sap->saip;
   dsp = (DenseSegPtr)sap->segs;
   start = sqn_binary_search_on_uint4_list(saip->aligncoords, pos, dsp->numseg);
   offset = pos - saip->aligncoords[start];
   idx = (dsp->dim*start) + row - 1;
   if (dsp->starts[idx] == -1)
   {
      if (which_end == SQN_RIGHT)
      {
         /* round down */
         while (start >= 0) {
            idx = (dsp->dim*start) + row - 1;
            if (dsp->starts[idx] != -1)
               return (dsp->starts[idx] + dsp->lens[start] - 1);
            start--;
         }
         return -2;
      } else if (which_end == SQN_LEFT)
      {
         /* round up */
         while (start < dsp->numseg) {
            idx = (dsp->dim*start) + row - 1;
            if (dsp->starts[idx] != -1)
               return (dsp->starts[idx]);
            start++;
         }
         return -2;
      }
   } else
   {
      idx = (dsp->dim*start) + row - 1;
      if (dsp->strands[idx] != Seq_strand_minus)
         return (dsp->starts[idx] + offset);
      else
         return (dsp->starts[idx] + dsp->lens[start] - 1 - offset);
   }
   return -1;
}

static Int4 MapBioseqToBioseqSpecial(SeqAlignPtr sap, Int4 begin, Int4 fin, Int4 pos, Int4 which_end)
{
   Int4  bspos;
   Int4  sapos;
   Int4  start1;
   Int4  start2;
   Int4  stop1;
   Int4  stop2;

   if (sap == NULL || sap->saip == NULL)
      return -2;
   AlnMgr2GetNthSeqRangeInSA(sap, begin, &start1, &stop1);
   AlnMgr2GetNthSeqRangeInSA(sap, fin, &start2, &stop2);
   /* check to see whether the position is outside the alignment */
   if (pos < start1)
      return (start2 - (start1 - pos));
   else if (pos > stop1)
      return (stop2 + (pos-stop1));
   sapos = AlnMgr2MapBioseqToSeqAlign(sap, pos, begin);
   bspos = MapRowCoordsSpecial(sap, sapos, fin, which_end);
   if (bspos >= 0)
      return bspos;
   else if (which_end == SQN_LEFT)
      return (start2-1);
   else if (which_end == SQN_RIGHT)
      return (stop2+1);
   else
      return 0;
}

static void SPI_flip_sa_list (SeqAlignPtr sap)
{
   DenseSegPtr  dsp;
   Int4         i;
   SeqIdPtr     sip;
   SeqIdPtr     sip_next;
   Int4         tmp_start;
   Uint1        tmp_strand;

   if (sap == NULL || sap->segtype != SAS_DENSEG)
      return;
   while (sap != NULL)
   {
      dsp = (DenseSegPtr)(sap->segs);
      if (dsp->dim == 2) /* skip anything with more than 2 rows */
      {
         /* first switch the ids */
         sip = dsp->ids;
         sip_next = sip->next;
         sip_next->next = sip;
         sip->next = NULL;
         dsp->ids = sip_next;
         /* then switch the starts and strands */
         for (i = 0; i<dsp->numseg; i++)
         {
            tmp_start = dsp->starts[2*i];
            dsp->starts[2*i] = dsp->starts[2*i+1];
            dsp->starts[2*i+1] = tmp_start;
            tmp_strand = dsp->strands[2*i];
            dsp->strands[2*i] = dsp->strands[2*i+1];
            dsp->strands[2*i+1] = tmp_strand;
         }
      }
      if (sap->saip != NULL) /* free indexes, reindex */
      {
         SAIndex2Free2(sap->saip);
         sap->saip = NULL;
         AlnMgr2IndexSingleChildSeqAlign(sap);
      }
      sap = sap->next;
   }
}

NLM_EXTERN SeqAlignPtr Sqn_GlobalAlign2Seq (BioseqPtr bsp1, BioseqPtr bsp2, BoolPtr revcomp)
{
   AMAlignIndex2Ptr     amaip;
   CodeBreakPtr         cbp;
   CdRegionPtr          crp;
   Int4                 i;
   BLAST_OptionsBlkPtr  options;
   CharPtr              program = "blastn";
   SeqAlignPtr          sap;
   SeqAlignPtr          sap_final;
   SeqAlignPtr          sap_head;
   SeqAlignPtr          sap_new;
   SeqAlignPtr          sap_prev;
   Int4                 start1;
   Int4                 start2;
   Int4                 stop1;
   Int4                 stop2;
   Uint1                strand;
   Int4                 extnd = 20;
   SeqMgrFeatContext    context;
   Uint2                entityID;
   RnaRefPtr            rrp;
   SeqFeatPtr           sfp;
   SeqIdPtr             sip;
   SeqLocPtr            slp;
   tRNAPtr              trp;

   if (bsp1 == NULL || bsp2 == NULL)
      return NULL;
   if (ISA_aa (bsp1->mol)) {
     program = "blastp";
   }
   options = BLASTOptionNew(program, TRUE);
   options->gapped_calculation = TRUE;
   options->expect_value = 0.001;
   if (bsp1->length > 10000 || bsp2->length > 10000)
   {
      options->expect_value = act_get_eval(60);
      options->wordsize = 20;
      options->filter_string = StringSave ("m L");
   }
   sap = BlastTwoSequences(bsp1, bsp2, program, options);
   BLASTOptionDelete(options);
   if (sap == NULL)
   {
      ErrPostEx (SEV_ERROR, 0, 0, "BLAST finds no sequence similarity");
      return NULL;
   }
   AlnMgr2IndexLite(sap);
   ACT_RemoveInconsistentAlnsFromSet(sap, 20, 1);
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   AlnMgr2SortAlnSetByNthRowPos(sap, 1);
   ACT_GetNthSeqRangeInSASet(sap, 1, &start1, &stop1);
   ACT_GetNthSeqRangeInSASet(sap, 2, &start2, &stop2);
   strand = AlnMgr2GetNthStrand(amaip->saps[0], 2);

   /* if opposite strand submitted, reverse complement, realign */

   if (strand == Seq_strand_minus) {
     if (revcomp != NULL) {
       *revcomp = TRUE;
     }
     AMAlignIndexFreeEitherIndex (sap);
     sap = SeqAlignFree (sap);
     BioseqRevComp (bsp2);
     entityID = ObjMgrGetEntityIDForPointer (bsp2);
     if (! SeqMgrFeaturesAreIndexed (entityID)) {
       SeqMgrIndexFeatures (entityID, NULL);
     }
     sfp = SeqMgrGetNextFeature (bsp2, NULL, 0, 0, &context);
     while (sfp != NULL) {
       sip = SeqLocId (sfp->location);
       slp = SeqLocCopyRegion (sip, sfp->location, bsp2, 0, bsp2->length - 1, Seq_strand_minus, FALSE);
       sfp->location = SeqLocFree (sfp->location);
       sfp->location = slp;
       switch (sfp->data.choice) {
         case SEQFEAT_CDREGION :
           crp = (CdRegionPtr) sfp->data.value.ptrvalue;
           if (crp != NULL) {
             for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) {
               sip = SeqLocId (cbp->loc);
               slp = SeqLocCopyRegion (sip, cbp->loc, bsp2, 0, bsp2->length - 1, Seq_strand_minus, FALSE);
               cbp->loc = SeqLocFree (cbp->loc);
               cbp->loc = slp;
             }
           }
           break;
         case SEQFEAT_RNA :
           rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
           if (rrp != NULL && rrp->ext.choice == 2) {
             trp = (tRNAPtr) rrp->ext.value.ptrvalue;
             if (trp != NULL && trp->anticodon != NULL) {
               sip = SeqLocId (trp->anticodon);
               slp = SeqLocCopyRegion (sip, trp->anticodon, bsp2, 0, bsp2->length - 1, Seq_strand_minus, FALSE);
               trp->anticodon = SeqLocFree (trp->anticodon);
               trp->anticodon = slp;
             }
           }
           break;
         default :
           break;
       }
       sfp = SeqMgrGetNextFeature (bsp2, sfp, 0, 0, &context);
     }
     SeqMgrClearFeatureIndexes (entityID, NULL);

     options = BLASTOptionNew(program, TRUE);
     options->gapped_calculation = TRUE;
     options->expect_value = 0.001;
     if (bsp1->length > 10000 || bsp2->length > 10000)
     {
        options->expect_value = act_get_eval(60);
        options->wordsize = 20;
        options->filter_string = StringSave ("m L");
     }
     sap = BlastTwoSequences(bsp1, bsp2, program, options);
     BLASTOptionDelete(options);
     if (sap == NULL)
     {
        Message(MSG_OK,"BLAST finds no sequence similarity in reverse complement");
        return NULL;
     }
     AlnMgr2IndexLite(sap);
     ACT_RemoveInconsistentAlnsFromSet(sap, 20, 1);
     amaip = (AMAlignIndex2Ptr)(sap->saip);
     AlnMgr2SortAlnSetByNthRowPos(sap, 1);
     ACT_GetNthSeqRangeInSASet(sap, 1, &start1, &stop1);
     ACT_GetNthSeqRangeInSASet(sap, 2, &start2, &stop2);
     strand = AlnMgr2GetNthStrand(amaip->saps[0], 2);
   }

   /* done with any reverse complementing and reblasting, now extend frayed ends */

   sap_head = NULL;

   if (start1 > 6 && start1 < extnd)
      sap_head = sap_prev = ACT_FindPiece(bsp1, bsp2, MAX(start1-SQN_WINDOW, 0), start1, MAX(start1-SQN_WINDOW, 0), start2, strand, SQN_LEFT);
   else if (start1 > 0 && start1 < extnd)
      SQN_ExtendAlnAlg(amaip->saps[0], start1, SQN_LEFT, Seq_strand_plus);
   ACT_GetNthSeqRangeInSASet(sap, 1, &start1, &stop1);
   ACT_GetNthSeqRangeInSASet(sap, 2, &start2, &stop2);
   if (start2 > 6 && start2 < extnd)
   {
      sap_new = ACT_FindPiece(bsp2, bsp1, MAX(start2-SQN_WINDOW, 0), start2, MAX(start1-SQN_WINDOW, 0), start1, strand, SQN_LEFT);
      if (sap_new != NULL)
         SPI_flip_sa_list(sap_new);
      if (sap_head != NULL)
      {
         sap_prev->next = sap_new;
         sap_prev = sap_new;
      } else
         sap_head = sap_prev = sap_new;
   } else if (start2 > 0 && start2 < extnd)
      SQN_ExtendAlnAlg(amaip->saps[0], start2, SQN_LEFT, Seq_strand_plus);
   for (i=0; i<amaip->numsaps-1; i++)
   {
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 1, NULL, &start1);
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i+1], 1, &stop1, NULL);
      if (strand != Seq_strand_minus)
      {
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 2, NULL, &start2);
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i+1], 2, &stop2, NULL);
      } else
      {
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 2, &stop2, NULL);
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i+1], 2, NULL, &start2);
      }
      sap_new = ACT_FindPiece(bsp1, bsp2, start1, stop1, start2, stop2, strand, SQN_MIDDLE);
      if (sap_head)
      {
         sap_prev->next = sap_new;
         if (sap_new != NULL)
            sap_prev = sap_new;
      } else
         sap_head = sap_prev = sap_new;
   }
   ACT_GetNthSeqRangeInSASet(sap, 1, &start1, &stop1);
   ACT_GetNthSeqRangeInSASet(sap, 2, &start2, &stop2);
   if (bsp1->length-stop1 > 6 && bsp1->length-stop1 < extnd)
   {
      sap_new = ACT_FindPiece(bsp1, bsp2, stop1, MIN(bsp1->length-1, stop1 + SQN_WINDOW), stop2, MIN(bsp2->length-1, stop2+SQN_WINDOW), strand, SQN_RIGHT);
      if (sap_new != NULL)
      {
         if (sap_head != NULL)
         {
            sap_prev->next = sap_new;
            sap_prev = sap_new;
         } else
           sap_head = sap_prev = sap_new;
      }
   } else if (bsp1->length-stop1 > 0 && bsp1->length-stop1 < extnd)
      SQN_ExtendAlnAlg(amaip->saps[amaip->numsaps-1], bsp1->length-stop1, SQN_RIGHT, Seq_strand_plus);
   ACT_GetNthSeqRangeInSASet(sap, 1, &start1, &stop1);
   ACT_GetNthSeqRangeInSASet(sap, 2, &start2, &stop2);
   if (bsp2->length-stop2 > 6 && bsp2->length-stop2 < extnd)
   {
      sap_new = ACT_FindPiece(bsp2, bsp1, stop2, MIN(bsp2->length-1, stop2 + SQN_WINDOW), stop1, MIN(bsp1->length-1, stop1+SQN_WINDOW), strand, SQN_RIGHT);
      if (sap_new != NULL)
      {
         SPI_flip_sa_list(sap_new);
         if (sap_head != NULL)
         {
            sap_prev->next = sap_new;
            sap_prev = sap_new;
         } else
           sap_head = sap_prev = sap_new;
      }
   } else if (bsp2->length-stop2 > 0 && bsp2->length-stop2 < extnd)
      SQN_ExtendAlnAlg(amaip->saps[amaip->numsaps-1], bsp2->length-stop2, SQN_RIGHT, Seq_strand_plus);
   sap_new = (SeqAlignPtr)(sap->segs);
   while (sap_new->next != NULL)
   {
      sap_new = sap_new->next;
   }
   sap_new->next = sap_head;  /* put the new alignments in the original set */
   AMAlignIndex2Free2(amaip);
   sap->saip = NULL;
   AlnMgr2IndexLite(sap);  /* reindex the alignments */
   sap_final = ACT_CleanUpAlignments(sap, bsp1->length, bsp2->length);
   return sap_final;
}

/***************************************************************************
*
*   AlnMgrTruncateSAP truncates a given seqalign to contain only the
*   bioseq coordinates from start to stop on the indicated row.  Anything
*   before those coordinates is discarded; anything remaining afterwards
*   is made into another seqalign and put in sap->next (the original next,
*   if any, is now at sap->next->next).  Doesn't work on parent seqaligns.
*   The function returns TRUE if the orignal alignment extended past stop.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgr2TruncateSAP(SeqAlignPtr sap, Int4 start, Int4 stop, Int4 row)
{
   DenseDiagPtr  ddp;
   DenseDiagPtr  ddp2;
   DenseSegPtr   dsp;
   Int4          from;
   Int4          i;
   Int4          mstart;
   Int4          mstop;
   SeqAlignPtr   sap1;
   SeqAlignPtr   sap2;
   Int4          tmp;
   Int4          to;

   if (sap == NULL || stop<start || row < 1)
      return FALSE;
   if (sap->segtype == SAS_DENSEG)
   {
      if (sap->saip == NULL)
         AlnMgr2IndexSingleChildSeqAlign(sap);
      AlnMgr2GetNthSeqRangeInSA(sap, row, &mstart, &mstop);
      if (mstart > start || mstop < stop)
         return FALSE;
      if (mstart == start)
      {
         if (mstop == stop)
            return FALSE;
         else if (mstop > stop)
         {
            from = AlnMgr2MapBioseqToSeqAlign(sap, start, row);
            to = AlnMgr2MapBioseqToSeqAlign(sap, stop, row);
            if (to < from)
            {
               tmp = to;
               to = from;
               from = tmp;
            }
            sap1 = AlnMgr2GetSubAlign(sap, from, to, 0, FALSE);
            AlnMgr2IndexSingleChildSeqAlign(sap1);
            from = AlnMgr2MapBioseqToSeqAlign(sap, stop+1, row);
            if (from < 0)
               return FALSE;
            to = AlnMgr2MapBioseqToSeqAlign(sap, mstop, row);
            if (to < from)
            {
               tmp = to;
               to = from;
               from = tmp;
            }
            sap2 = AlnMgr2GetSubAlign(sap, from, to, 0, FALSE);
            sap2->next = sap->next;
            sap->next = sap2;
            dsp = sap->segs;
            sap->segs = (Pointer)(sap1->segs);
            sap1->segs = NULL;
            DenseSegFree(dsp);
            SeqAlignFree(sap1);
            AlnMgr2IndexSingleChildSeqAlign(sap);
            AlnMgr2IndexSingleChildSeqAlign(sap2);
            return TRUE;
         }
      } else if (mstart < start) /* throw away the first part */
      {
         from = AlnMgr2MapBioseqToSeqAlign(sap, start, row);
         to = AlnMgr2MapBioseqToSeqAlign(sap, stop, row);
         if (to < from)
         {
            tmp = to;
            to = from;
            from = tmp;
         }
         sap1 = AlnMgr2GetSubAlign(sap, from, to, 0, FALSE);
         if (mstop == stop) /* done */
         {
            dsp = sap->segs;
            sap->segs = (Pointer)(sap1->segs);
            sap1->segs = NULL;
            DenseSegFree(dsp);
            SeqAlignFree(sap1);
            AlnMgr2IndexSingleChildSeqAlign(sap);
            return TRUE;
         } else if (mstop > stop)
         {
            from = AlnMgr2MapBioseqToSeqAlign(sap, stop+1, row);
            if (from < 0)
               return FALSE;
            to = AlnMgr2MapBioseqToSeqAlign(sap, mstop, row);
            if (to < from)
            {
               tmp = to;
               to = from;
               from = tmp;
            }
            sap2 = AlnMgr2GetSubAlign(sap, from, to, 0, FALSE);
            sap2->next = sap->next;
            sap->next = sap2;
            AlnMgr2IndexSingleChildSeqAlign(sap2);
            dsp = sap->segs;
            sap->segs = (Pointer)(sap1->segs);
            sap1->segs = NULL;
            DenseSegFree(dsp);
            SeqAlignFree(sap1);
            AlnMgr2IndexSingleChildSeqAlign(sap);
            return TRUE;
         }
      }
   } else if (sap->segtype == SAS_DENDIAG)
   {
      ddp = (DenseDiagPtr)(sap->segs);
      if (ddp->dim < row)
         return FALSE;
      mstart = ddp->starts[row-1];
      mstop = mstart + ddp->len - 1;
      if (mstart > start || mstop < stop)
         return FALSE;
      if (mstart == start)
      {
         if (mstop == stop)
            return FALSE;
         else if (mstop > stop)
         {
            ddp2 = DenseDiagNew();
            ddp2->dim = ddp->dim;
            ddp2->starts = (Int4Ptr)MemNew((ddp->dim)*sizeof(Int4));
            ddp2->id = SeqIdDupList(ddp->id);
            ddp2->strands = (Uint1Ptr)MemNew((ddp->dim)*sizeof(Uint1));
            ddp2->scores = ScoreDup(ddp->scores);
            for (i=0; i<ddp->dim; i++)
            {
               ddp2->starts[i] = ddp->starts[i] + ddp->len - (mstop - stop);
               ddp2->strands[i] = ddp->strands[i];
            }
            ddp2->len = mstop - stop;
            ddp->len = ddp->len - (mstop - stop);
            sap2 = SeqAlignNew();
            sap2->type = SAT_PARTIAL;
            sap2->segtype = SAS_DENSEG;
            sap2->segs = (Pointer)ddp2;
            sap2->next = sap->next;
            sap->next = sap2;
            AlnMgr2IndexSingleChildSeqAlign(sap2);
            return TRUE;
         }
      } else if (mstart < start)
      {
         for (i=0; i<ddp->dim; i++)
         {
            ddp->starts[i] = ddp->starts[i] + start - mstart;
         }
         ddp->len = ddp->len - (start - mstart);
         AlnMgr2IndexSingleChildSeqAlign(sap);
         if (mstop == stop)
            return FALSE;
         else if (mstop > stop)
         {
            ddp2 = DenseDiagNew();
            ddp2->dim = ddp->dim;
            ddp2->starts = (Int4Ptr)MemNew((ddp->dim)*sizeof(Int4));
            ddp2->id = SeqIdDupList(ddp->id);
            ddp2->strands = (Uint1Ptr)MemNew((ddp->dim)*sizeof(Uint1));
            ddp2->scores = ScoreDup(ddp->scores);
            for (i=0; i<ddp->dim; i++)
            {
               ddp2->starts[i] = ddp->starts[i] + ddp->len - (mstop - stop);
               ddp2->strands[i] = ddp->strands[i];
            }
            ddp2->len = mstop - stop;
            ddp->len = ddp->len - (mstop - stop);
            sap2 = SeqAlignNew();
            sap2->type = SAT_PARTIAL;
            sap2->segtype = SAS_DENSEG;
            sap2->segs = (Pointer)ddp2;
            sap2->next = sap->next;
            sap->next = sap2;
            AlnMgr2IndexSingleChildSeqAlign(sap2);
            return TRUE;
         }   
      }
   } else
      return FALSE;
   return FALSE;
}

