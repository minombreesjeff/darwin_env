static char const rcsid[] = "$Id: motif.c,v 6.5 2003/05/30 17:25:37 coulouri Exp $";

/*   motif.c
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
* File Name:  motif.c
*
* Author:  Sarah Wheelan
*
* Version Creation Date:   8/9/01
*
* $Revision: 6.5 $
*
* File Description:  sequence motif search functions
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include <motif.h>

static Int4 MOT_IsItPolyA(Uint1Ptr buf)
{
   BioseqPtr   bsp;
   Int4        count;
   Boolean     done;
   Int4        i;
   Int4        j;
   Uint1       res;
   Int4        start;
  
   done = FALSE;
   i = 0;
   j = 0;
   start = 0;
   count = 0;
   while (((res = buf[j]) != SEQPORT_EOF) && !done)
   {
      if (res != 8)
      {
         if (count >= MOT_LINKERSIZE) /* can have non-A bases up to the MOT_LINKERSIZE position */
            done = TRUE;
         else
            start++;
      } else
         i++;
      count++;
      j++;
   }
   if (start > 0)
   {
      if (i-start < MOT_MINPOLYASIZE) /* "tail" is too short */
         return 0;
   }
   return i;
}

NLM_EXTERN MotifInfoPtr MOT_FindPolyA(BioseqPtr bsp)
{
   Uint1         buf[MOT_WINSIZE];
   Uint1         buf2[MOT_WINSIZE];
   Int4          i;
   Int4          j;
   MotifInfoPtr  mip;
   MotifInfoPtr  mip_head;
   MotifInfoPtr  mip_prev;
   Int4          score;
   Uint1Ptr      seq;
   SeqPortPtr    spp;

   if (bsp == NULL || !ISA_na(bsp->mol))
      return NULL;
   mip_head = mip_prev = NULL;
   for (i=0; i<bsp->length-1; i+=MOT_WINSIZE)
   {
      spp = SeqPortNew(bsp, i, MIN(i+MOT_WINSIZE-1, bsp->length-1), Seq_strand_minus, Seq_code_ncbi4na);
      SeqPortRead(spp, buf2, MIN(MOT_WINSIZE, bsp->length-1-i));
      for (j=0; j<MIN(MOT_WINSIZE, bsp->length-1-i); j++)
      {
         if (buf2[j] == 1)
            buf[j] = 0;
         else if (buf2[j] == 2)
            buf[j] = 1;
         else if (buf2[j] == 4)
            buf[j] = 2;
         else if (buf2[j] == 8)
            buf[j] = 3;
         else
            buf[j] = 4;
      }
      seq = buf2;
      for (j=0; j<MIN(MOT_WINSIZE, bsp->length-1-i); j++)
      {
         score = MOT_IsItPolyA(seq);
         if (score > MOT_MINPOLYASIZE)
         {
            mip = (MotifInfoPtr)MemNew(sizeof(MotifInfo));
            mip->start = i+j;
            mip->stop = mip->start + score - 1;
            mip->score = score;
            mip->strand = Seq_strand_plus;
            if (mip_head != NULL)
            {
               mip_prev->next = mip;
               mip_prev = mip;
            } else
               mip_head = mip_prev = mip;
         }
         seq++;
      }
      SeqPortFree(spp);
   }
   for (i=0; i<bsp->length-1; i+=MOT_WINSIZE)
   {
      spp = SeqPortNew(bsp, i, MIN(i+MOT_WINSIZE-1, bsp->length-1), Seq_strand_minus, Seq_code_ncbi4na);
      SeqPortRead(spp, buf2, MIN(MOT_WINSIZE, bsp->length-1-i));
      for (j=0; j<MIN(MOT_WINSIZE, bsp->length-1-i); j++)
      {
         if (buf2[j] == 1)
            buf[j] = 0;
         else if (buf2[j] == 2)
            buf[j] = 1;
         else if (buf2[j] == 4)
            buf[j] = 2;
         else if (buf2[j] == 8)
            buf[j] = 3;
         else
            buf[j] = 4;
      }
      seq = buf2;
      for (j=0; j<MIN(MOT_WINSIZE, bsp->length-1-i); j++)
      {
         score = MOT_IsItPolyA(seq);
         if (score > MOT_MINPOLYASIZE)
         {
            mip = (MotifInfoPtr)MemNew(sizeof(MotifInfo));
            mip->start = i+j;
            mip->stop = mip->start + score;
            mip->score = score;
            mip->strand = Seq_strand_minus;
            if (mip_head != NULL)
            {
               mip_prev->next = mip;
               mip_prev = mip;
            } else
               mip_head = mip_prev = mip;
         }
         seq++;
      }
      SeqPortFree(spp);
   }
   return mip_head;
}

static void MOT_ScoreKozak(Uint1Ptr sequence, FloatHiPtr score)
{
   Int4     acgtn[5] = {0, 0, 0, 0, 0};
   FloatHi  k[9][5] = {
               {0.2028, 0.2175, 0.4364, 0.1432, 0.25},
               {0.1206, 0.4319, 0.2738, 0.1736, 0.25},
               {0.25, 0.3950, 0.2361, 0.1189, 0.25},
               {0.5142, 0.0503, 0.4043, 0.0311, 0.25},
               {0.2739, 0.3093, 0.2020, 0.2147, 0.25},
               {0.1621, 0.5146, 0.25, 0.0733, 0.25},
               {0.25, 0.2239, 0.6344, 0.1156, 0.25},
               {0.25, 0.3422, 0.2252, 0.1826, 0.25},
               {0.1581, 0.2768, 0.3393, 0.2258, 0.25}};
   Int4     j;
   FloatHi  prob_seqgsite;
   FloatHi  prob_seq;

   if (sequence == NULL || score == NULL)
      return;
   prob_seqgsite = 1;
   prob_seq = 1;
   for (j=0; j<9; j++)
   {
      acgtn[sequence[j]]++;
      prob_seqgsite *= k[j][sequence[j]];
   }
   for (j=0; j<9; j++)
   {
      prob_seq *= acgtn[sequence[j]]/(FloatHi)9;
   }
   *score = (prob_seqgsite/prob_seq);
}

NLM_EXTERN MotifInfoPtr MOT_FindKozak(BioseqPtr bsp)
{
   Uint1         buf[MOT_WINSIZE];
   Uint1         buf2[MOT_WINSIZE];
   Int4          i;
   Int4          j;
   MotifInfoPtr  mip;
   MotifInfoPtr  mip_head;
   MotifInfoPtr  mip_prev;
   FloatHi       score;
   Uint1Ptr      seq;
   SeqPortPtr    spp;

   if (bsp == NULL || !ISA_na(bsp->mol))
      return NULL;
   mip_head = mip_prev = NULL;
   for (i=0; i<bsp->length-1; i+=MOT_WINSIZE)
   {
      spp = SeqPortNew(bsp, i, MIN(i+MOT_WINSIZE-1, bsp->length-1), Seq_strand_minus, Seq_code_ncbi4na);
      SeqPortRead(spp, buf2, MIN(MOT_WINSIZE, bsp->length-1-i));
      for (j=0; j<MIN(MOT_WINSIZE, bsp->length-1-i); j++)
      {
         if (buf2[j] == 1)
            buf[j] = 0;
         else if (buf2[j] == 2)
            buf[j] = 1;
         else if (buf2[j] == 4)
            buf[j] = 2;
         else if (buf2[j] == 8)
            buf[j] = 3;
         else
            buf[j] = 4;
      }
      seq = buf2;
      for (j=0; j<MIN(MOT_WINSIZE, bsp->length-1-i); j++)
      {
         MOT_ScoreKozak(seq, &score);
         if (score > MOT_KOZTHRESH)
         {
            mip = (MotifInfoPtr)MemNew(sizeof(MotifInfo));
            mip->start = i+j;
            mip->stop = mip->start + MOT_KOZLEN - 1;
            mip->score = score;
            mip->strand = Seq_strand_plus;
            if (mip_head != NULL)
            {
               mip_prev->next = mip;
               mip_prev = mip;
            } else
               mip_head = mip_prev = mip;
         }
         seq++;
      }
      SeqPortFree(spp);
   }
   for (i=0; i<bsp->length-1; i+=MOT_WINSIZE)
   {
      spp = SeqPortNew(bsp, i, MIN(i+MOT_WINSIZE-1, bsp->length-1), Seq_strand_minus, Seq_code_ncbi4na);
      SeqPortRead(spp, buf2, MIN(MOT_WINSIZE, bsp->length-1-i));
      for (j=0; j<MIN(MOT_WINSIZE, bsp->length-1-i); j++)
      {
         if (buf2[j] == 1)
            buf[j] = 0;
         else if (buf2[j] == 2)
            buf[j] = 1;
         else if (buf2[j] == 4)
            buf[j] = 2;
         else if (buf2[j] == 8)
            buf[j] = 3;
         else
            buf[j] = 4;
      }
      seq = buf2;
      for (j=0; j<MIN(MOT_WINSIZE, bsp->length-1-i); j++)
      {
         MOT_ScoreKozak(seq, &score);
         if (score > MOT_KOZTHRESH)
         {
            mip = (MotifInfoPtr)MemNew(sizeof(MotifInfo));
            mip->start = i+j;
            mip->stop = mip->start + MOT_KOZLEN;
            mip->score = score;
            mip->strand = Seq_strand_minus;
            if (mip_head != NULL)
            {
               mip_prev->next = mip;
               mip_prev = mip;
            } else
               mip_head = mip_prev = mip;
         }
         seq++;
      }
      SeqPortFree(spp);
   }
   return mip_head;
}

NLM_EXTERN MotifInfoPtr MOT_MotifSearch(CharPtr motif, BioseqPtr bsp, Boolean is_prot)
{
   ACTProfilePtr        app;
   FloatHi              bit_score;
   ByteStorePtr         bs;
   Int4                 bslen;
   BioseqPtr            bsp_mot;
   FloatHi              evalue;
   Int4                 len;
   MotifInfoPtr         mip;
   MotifInfoPtr         mip_head;
   MotifInfoPtr         mip_prev;
   Int4                 number;
   BLAST_OptionsBlkPtr  options;
   CharPtr              ptr;
   SeqAlignPtr          salp;
   SeqAlignPtr          sap;
   Int4                 score;
   SeqIdPtr             sip;
   Int4                 start;
   Int4                 stop;

   if (motif == NULL || bsp == NULL)
      return NULL;
   if (is_prot == TRUE && !ISA_aa(bsp->mol))
      return NULL;
   if (is_prot == FALSE && !ISA_na(bsp->mol))
      return NULL;
   len = 0;
   StrUpper(motif);
   ptr = motif;
   while (*ptr != '\0')
   {
      len++;
      ptr++;
   }
   if (len < 7) /* less than the BLAST wordsize -- eventually do FSA */
      return NULL;
   bs = BSNew(len);
   bslen = BSWrite(bs, (VoidPtr)motif, len);
   if (bslen != len) /* failed to create the byte store */
   {
      BSDelete(bs, len);
      return NULL;
   }
   bsp_mot = BioseqNew();
   bsp_mot->seq_data = bs;
   bsp_mot->length = len;
   if (is_prot == FALSE)
   {
      bsp_mot->seq_data_type = Seq_code_iupacna;
      bsp_mot->mol = Seq_mol_na;
   } else
   {
      bsp_mot->seq_data_type = Seq_code_iupacaa;
      bsp_mot->mol = Seq_mol_na;
   }
   sip = SeqIdFromAccessionDotVersion("MOTIF");
   bsp_mot->id = sip;
   bsp_mot->repr = Seq_repr_raw;
   if (is_prot == FALSE)
   {
      options = BLASTOptionNew("blastn", TRUE);
      options->wordsize = MIN(bsp_mot->length, options->wordsize);
      sap = BlastTwoSequences(bsp_mot, bsp, "blastn", options);
   } else
   {
      options = BLASTOptionNew("blastp", TRUE);
      options->wordsize = MIN(bsp_mot->length, options->wordsize);
      sap = BlastTwoSequences(bsp_mot, bsp, "blastp", options);
   }
   if (sap == NULL)
   {
      BioseqFree(bsp_mot);
      return NULL;
   }
   salp = sap;
   mip_head = mip_prev = NULL;
   while (salp != NULL)
   {
      mip = (MotifInfoPtr)MemNew(sizeof(MotifInfo));
      AlnMgr2IndexSingleChildSeqAlign(salp);
      AlnMgr2GetNthSeqRangeInSA(salp, 2, &start, &stop);
      mip->strand = AlnMgr2GetNthStrand(salp, 1);
      mip->start = start;
      mip->stop = stop;
      GetScoreAndEvalue(salp, &score, &bit_score, &evalue, &number);
      mip->score = score;
      if (mip_head != NULL)
      {
         mip_prev->next = mip;
         mip_prev = mip;
      } else
         mip_head = mip_prev = mip;
      salp = salp->next;
   }
   SeqAlignSetFree(sap);
   return mip_head;
}

static void MOT_ScoreSignalPep(Uint1Ptr sequence, FloatHiPtr score)
{
   Int4     j;
   Int4     p[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
   FloatHi  prob_seqgsite;
   FloatHi  prob_seq;
   FloatHi  s[15][20] = {
               {0.1003, 0.0357, 0.0020, 0.0010, 0.0745, 0.0387, 0.0050, 0.0725, 0.0040, 0.3734, 0.0258, 0.0020, 0.0228, 0.0030, 0.0070, 0.0566, 0.0318, 0.1112, 0.0159, 0.0169},
               {0.1111, 0.0317, 0.0000, 0.0020, 0.0665, 0.0238, 0.0030, 0.0665, 0.0010, 0.4286, 0.0149, 0.0040, 0.0129, 0.0060, 0.0040, 0.0466, 0.0387, 0.1230, 0.0129, 0.0030},
               {0.1050, 0.0337, 0.0030, 0.0040, 0.0792, 0.0257, 0.0040, 0.0812, 0.0010, 0.3931, 0.0208, 0.0040, 0.0168, 0.0040, 0.0040, 0.0396, 0.0297, 0.1257, 0.0198, 0.0059},
               {0.0990, 0.0307, 0.0000, 0.0050, 0.0802, 0.0337, 0.0020, 0.0584, 0.0020, 0.4446, 0.0188, 0.0059, 0.0089, 0.0089, 0.0020, 0.0436, 0.0376, 0.0990, 0.0109, 0.0089},
               {0.1563, 0.0415, 0.0030, 0.0040, 0.0643, 0.0356, 0.0040, 0.0514, 0.0000, 0.3818, 0.0109, 0.0069, 0.0119, 0.0109, 0.0030, 0.0643, 0.0376, 0.0979, 0.0089, 0.0059},
               {0.1266, 0.0653, 0.0020, 0.0059, 0.0574, 0.0495, 0.0099, 0.0514, 0.0030, 0.3254, 0.0297, 0.0069, 0.0129, 0.0188, 0.0069, 0.0752, 0.0485, 0.0851, 0.0129, 0.0069},
               {0.1058, 0.0564, 0.0040, 0.0129, 0.0910, 0.0396, 0.0138, 0.0465, 0.0030, 0.3294, 0.0198, 0.0079, 0.0237, 0.0178, 0.0089, 0.0663, 0.0415, 0.0781, 0.0237, 0.0099},
               {0.1474, 0.0386, 0.0049, 0.0089, 0.0663, 0.0287, 0.0069, 0.0682, 0.0030, 0.2770, 0.0109, 0.0069, 0.0593, 0.0129, 0.0059, 0.0653, 0.0346, 0.1276, 0.0208, 0.0059},
               {0.1446, 0.0297, 0.0248, 0.0277, 0.0248, 0.1069, 0.0228, 0.0218, 0.0198, 0.0970, 0.0050, 0.0248, 0.0970, 0.0545, 0.0337, 0.1158, 0.0693, 0.0594, 0.0129, 0.0079},
               {0.1058, 0.0336, 0.0138, 0.0267, 0.0336, 0.1236, 0.0119, 0.0425, 0.0188, 0.1454, 0.0119, 0.0138, 0.0732, 0.0396, 0.0376, 0.0959, 0.0692, 0.0712, 0.0129, 0.0188},
               {0.2552, 0.0673, 0.0040, 0.0069, 0.0069, 0.0732, 0.0049, 0.0406, 0.0069, 0.0643, 0.0089, 0.0089, 0.0069, 0.0049, 0.0049, 0.1405, 0.1019, 0.1889, 0.0020, 0.0020},
               {0.0791, 0.0208, 0.0435, 0.0653, 0.0455, 0.0376, 0.0524, 0.0178, 0.0237, 0.1632, 0.0138, 0.0366, 0.0168, 0.0712, 0.0554, 0.1108, 0.0435, 0.0445, 0.0326, 0.0257},
               {0.4530, 0.0495, 0.0059, 0.0059, 0.0059, 0.1820, 0.0020, 0.0040, 0.0040, 0.0208, 0.0030, 0.0089, 0.0267, 0.0148, 0.0198, 0.1395, 0.0425, 0.0079, 0.0020, 0.0020},
               {0.1395, 0.0227, 0.0673, 0.0910, 0.0336, 0.0514, 0.0227, 0.0366, 0.0514, 0.0593, 0.0138, 0.0257, 0.0109, 0.1335, 0.0326, 0.0900, 0.0386, 0.0495, 0.0089, 0.0208},
               {0.0544, 0.0267, 0.0653, 0.0870, 0.0277, 0.0564, 0.0218, 0.0336, 0.0396, 0.0504, 0.0119, 0.0524, 0.1642, 0.0415, 0.0396, 0.0870, 0.0564, 0.0554, 0.0059, 0.0227}};

   if (sequence == NULL || score == NULL)
      return;
   prob_seqgsite = 1;
   prob_seq = 1;
   for (j=0; j<15; j++)
   {
      p[sequence[j]]++;
      prob_seqgsite *= s[j][sequence[j]];
   }
   for (j=0; j<15; j++)
   {
      prob_seq *= p[sequence[j]]/(FloatHi)15;
   }
   *score = (prob_seqgsite/prob_seq);
}

NLM_EXTERN MotifInfoPtr MOT_FindSignalPeptide(BioseqPtr bsp)
{
   Uint1         buf[MOT_WINSIZE];
   Uint1         buf2[MOT_WINSIZE];
   Int4          i;
   Int4          j;
   MotifInfoPtr  mip;
   MotifInfoPtr  mip_head;
   MotifInfoPtr  mip_prev;
   FloatHi       score;
/*    Uint1Ptr      seq; */
   SeqPortPtr    spp;

   if (bsp == NULL || !ISA_aa(bsp->mol))
      return NULL;
   mip_head = mip_prev = NULL;
   for (i=0; i<bsp->length-1; i+=MOT_WINSIZE)
   {
      spp = SeqPortNew(bsp, i, MIN(i+MOT_WINSIZE-1, bsp->length-1), Seq_strand_minus, Seq_code_iupacaa);
      SeqPortRead(spp, buf2, MIN(MOT_WINSIZE, bsp->length-1-i));
      for (j=0; j<MIN(MOT_WINSIZE, bsp->length-1-i); j++)
      {
         MOT_ScoreSignalPep(buf2, &score);
         if (score > MOT_SIGTHRESH)
         {
            mip = (MotifInfoPtr)MemNew(sizeof(MotifInfo));
            mip->start = i+j;
            mip->stop = mip->start + MOT_SIGLEN - 1;
            mip->score = score;
            mip->strand = Seq_strand_plus;
            if (mip_head != NULL)
            {
               mip_prev->next = mip;
               mip_prev = mip;
            } else
               mip_head = mip_prev = mip;
         }
/*          seq++; */
      }
      SeqPortFree(spp);
   }
   for (i=0; i<bsp->length-1; i+=MOT_WINSIZE)
   {
      spp = SeqPortNew(bsp, i, MIN(i+MOT_WINSIZE-1, bsp->length-1), Seq_strand_minus, Seq_code_iupacaa);
      SeqPortRead(spp, buf2, MIN(MOT_WINSIZE, bsp->length-1-i));
      for (j=0; j<MIN(MOT_WINSIZE, bsp->length-1-i); j++)
      {
         MOT_ScoreSignalPep(buf2, &score);
         if (score > MOT_SIGTHRESH)
         {
            mip = (MotifInfoPtr)MemNew(sizeof(MotifInfo));
            mip->start = i+j;
            mip->stop = mip->start + MOT_SIGLEN;
            mip->score = score;
            mip->strand = Seq_strand_minus;
            if (mip_head != NULL)
            {
               mip_prev->next = mip;
               mip_prev = mip;
            } else
               mip_head = mip_prev = mip;
         }
/*          seq++; */
      }
      SeqPortFree(spp);
   }
   return mip_head;
}

NLM_EXTERN MotifInfoPtr MOT_FindRepeats(BioseqPtr bsp)
{
   MotifInfoPtr  mip;
   MotifInfoPtr  mip_head;
   MotifInfoPtr  mip_prev;
   SeqLocPtr     slp;
   SeqLocPtr     slp_repeat;

   if (bsp == NULL || !ISA_na(bsp->mol))
      return NULL;
   slp = SeqLocIntNew(0, bsp->length-1, Seq_strand_plus, bsp->id);
   slp_repeat = BlastSeqLocFilter(slp, "R");
   SeqLocFree(slp);
   slp = slp_repeat;
   mip_head = mip_prev = NULL;
   while (slp != NULL)
   {
      mip = (MotifInfoPtr)MemNew(sizeof(MotifInfo));
      mip->start = SeqLocStart(slp);
      mip->stop = SeqLocStop(slp);
      mip->strand = SeqLocStrand(slp);
      if (mip_head != NULL)
      {
         mip_prev->next = mip;
         mip_prev = mip;
      } else
         mip_head = mip_prev = mip;
      slp = slp->next;
   }
   SeqLocSetFree(slp_repeat);
   return mip_head;
}

/*
Int2 Main(void)
{
   BioseqPtr  bsp;
   SeqIdPtr   sip;

   LocalSeqFetchInit(FALSE);
   ID1BioseqFetchEnable("motif", FALSE);
   ErrSetFatalLevel (SEV_MAX);
   ErrClearOptFlags (EO_SHOW_USERSTR);
   UseLocalAsnloadDataAndErrMsg ();
   ErrPathReset ();
   if (! AllObjLoad ())
   {
      Message (MSG_FATAL, "AllObjLoad failed");
      return 1;
   }
   if (! SubmitAsnLoad ())
   {
      Message (MSG_FATAL, "SubmitAsnLoad failed");
      return 1;
   }
   if (! FeatDefSetLoad ())
   {
      Message (MSG_FATAL, "FeatDefSetLoad failed");
      return 1;
   }
   if (! SeqCodeSetLoad ())
   {
      Message (MSG_FATAL, "SeqCodeSetLoad failed");
      return 1;
   }
   if (! GeneticCodeTableLoad ())
   {
      Message (MSG_FATAL, "GeneticCodeTableLoad failed");
      return 1;
   }
   sip = SeqIdFromAccessionDotVersion("NT_001128");
   bsp = BioseqLockById(sip);
   MOT_FindPolyA(bsp);
   MOT_FindSignalPeptide(bsp);
   MOT_FindRepeats(bsp);
   BioseqUnlock(bsp);
   return 0;
}
*/
