static char const rcsid[] = "$Id: urkepi.c,v 6.19 2003/05/30 17:25:38 coulouri Exp $";

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
*
* File Name: urkepi.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.19 $
*
* File Description: epi - low complexity
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urkepi.c,v $
* Revision 6.19  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.18  1999/02/25 15:06:48  kuzio
* commutative prob func optimization
*
* Revision 6.17  1998/11/16 14:29:50  kuzio
* flagBoundaryCondition
*
* Revision 6.16  1998/09/16 18:03:33  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#include <urkepi.h>

/* ideal */
/* #define POSSIBLE_RESIDUES 20 */
/* 20 amino acids + selenocysteine (U) + stop (*) + unknown (X) */
/* #define POSSIBLE_RESIDUES 23 */
/* realistic; ideal + others */
#define POSSIBLE_RESIDUES 21
#define POSSIBLE_AA_RESIDUES 21
/* nucleotides ... just A C G T */
/* #define POSSIBLE_NA_RESIDUES 4 */
/* nucleotides ... A C G T N */
#define POSSIBLE_NA_RESIDUES 5

extern EpiDatPtr EpiDatNew (void)
{
  EpiDatPtr epip;

  if ((epip = (EpiDatPtr) MemNew (sizeof (EpiDat))) != NULL)
  {
    epip->method = 0;        /* no optimization */
    epip->linker = 5;        /* less than "classical" epitope */
    epip->mwindow = 6;
    epip->mpercentcut = 0.0;
    epip->window = 16;
    epip->percentcut = 4.0;
    epip->nucleotuple = 3;
    if ((epip->epiarray = (Int4Ptr) MemNew ((size_t) (sizeof (Int4) * 256)))
        == NULL)
    {
      return (EpiDatPtr) MemFree (epip);
    }
  }
  return epip;
}

extern EpiDatPtr EpiDatFree (EpiDatPtr epip)
{
  if (epip != NULL)
  {
    MemFree (epip->sequence);
    MemFree (epip->epiarray);
    MemFree (epip->score);
    epip = (EpiDatPtr) MemFree (epip);
  }
  return epip;
}

static FloatHi ArrayScore (Int4Ptr epiarray)
{
  FloatHi  episcore;
  Int4     i;

  episcore = 1.0;
  for (i = (Int4) 'A'; i <= (Int4) 'Z'; i++)
    episcore *= (FloatHi) epiarray[i];

  return episcore;
}

static FloatHi NucArrayScore (Int4Ptr epiarray, Int4 maxarray)
{
  FloatHi  episcore;
  Int4     i;

  episcore = 1.0;
  for (i = 0; i < maxarray; i++)
    episcore *= (FloatHi) epiarray[i];

  return episcore;
}

static FloatHi DownArrayScore (FloatHi episcore, FloatHi arrayvalue)
{
  return episcore / arrayvalue;
}

static FloatHi UpArrayScore (FloatHi episcore, FloatHi arrayvalue)
{
  return episcore * arrayvalue;
}

static void UpdateScore (FloatHiPtr fptrscan, FloatHi episcore,
                         Int4 start, Int4 end)
{
  Int4 i;

  for (i = start; i <= end; i++)
  {
    if (episcore < *fptrscan)
      *fptrscan = episcore;
    fptrscan++;
  }
  return;
}

static FloatHi MaxScore (Int4 window, Int4 possibleresidues)
{
  FloatHi maxscore;
  Int4    a, b, m, n;

  a = window / possibleresidues;
  if ((m = window % possibleresidues) != 0)
    a++;
  else
    m = possibleresidues;
  b = a + 1;
  n = possibleresidues - m;
  maxscore = (FloatHi) (pow ((FloatHi) a, (FloatHi) n) *
                        pow ((FloatHi) b, (FloatHi) m));
  return maxscore;
}

static FloatHi PercentEpiScore (FloatHi episcore, Int4 win, FloatHi maxscore)
{
  return (episcore - (FloatHi) (win + 1)) * 100.0 / maxscore;
}

static Int4 PredictEpiI0 (CharPtr seqin, Int4 start, Int4 end, Int4 window,
                          EpiDatPtr epip, FloatHiPtr fptrhead,
                          FloatHi percentcut)
{
  Int4       i, j, k;
  CharPtr    seq;
  FloatHiPtr fptr;
  FloatHi    maxscore, episcore, percentscore;
  Int4       minwin;

  if (seqin == NULL || epip == NULL)
    return -1;

  seq = &seqin[start];
  fptr = &fptrhead[start];
  minwin = window;

/* "zero" array for every cycle */
  for (j = (Int4) 'A'; j <= (Int4) 'Z'; j++)
    epip->epiarray[j] = 1;

/* minwin-mer set up */
  k = start + minwin - 1;
  for (j = start; j < k && j <= end; j++)
  {
    switch (*seq)
    {
     case '*':
     case 'B':
     case 'J':
     case 'O':
     case 'U':
     case 'X':
     case 'Z':
      epip->epiarray['X'] += 1;
      break;
     default:
      epip->epiarray[*seq] += 1;
      break;
    }
    seq++;
  }
  maxscore = MaxScore (window, POSSIBLE_AA_RESIDUES);
  episcore = ArrayScore (epip->epiarray);

/* run -- rolling scan */
  for (i = k; i <= end; i++)
  {
    switch (*seq)
    {
     case '*':
     case 'B':
     case 'J':
     case 'O':
     case 'U':
     case 'X':
     case 'Z':
      episcore = DownArrayScore (episcore, epip->epiarray['X']);
      epip->epiarray['X'] += 1;
      episcore = UpArrayScore (episcore, epip->epiarray['X']);
      break;
     default:
      episcore = DownArrayScore (episcore, epip->epiarray[*seq]);
      epip->epiarray[*seq] += 1;
      episcore = UpArrayScore (episcore, epip->epiarray[*seq]);
      break;
    }
    if ((percentscore = PercentEpiScore (episcore, window, maxscore))
          <= percentcut)
      UpdateScore (fptr, percentscore, i-window+1, i);
    fptr++;
    seq++;
    switch (*(seq-window))
    {
     case '*':
     case 'B':
     case 'J':
     case 'O':
     case 'U':
     case 'X':
     case 'Z':
      episcore = DownArrayScore (episcore, epip->epiarray['X']);
      epip->epiarray['X'] -= 1;
      episcore = UpArrayScore (episcore, epip->epiarray['X']);
      break;
     default:
      episcore = DownArrayScore (episcore, epip->epiarray[*(seq-window)]);
      epip->epiarray[*(seq-window)] -= 1;
      episcore = UpArrayScore (episcore, epip->epiarray[*(seq-window)]);
      break;
    }
  }
  return (i - 1);
}

static Int4Ptr NumericSequence (CharPtr seq, Int4 start, Int4 stop, Int4 epi)
{
  Int4Ptr    sn, seqnumarray;
  Int4       i, k;

  if ((sn = seqnumarray = (Int4Ptr) MemNew ((size_t)
                           (sizeof (Int4) * (stop-start+1)))) == NULL)
  {
    return NULL;
  }
/* rock */
  for (i = start; i < start+epi-1; i++)
  {
    *sn *= POSSIBLE_NA_RESIDUES;
    switch (*seq)
    {
     case 'A':
      *sn += 0;
      break;
     case 'C':
      *sn += 1;
      break;
     case 'G':
      *sn += 2;
      break;
     case 'T':
      *sn += 3;
      break;
     default:
      *sn += 4;
      break;
    }
    seq++;
  }
/* roll */
  k = (Int4) pow (POSSIBLE_NA_RESIDUES, (FloatHi) (epi-1));
  for (i = start+epi-1; i <= stop; i++)
  {
    *sn *= POSSIBLE_NA_RESIDUES;
    switch (*seq)
    {
     case 'A':
      *sn += 0;
      break;
     case 'C':
      *sn += 1;
      break;
     case 'G':
      *sn += 2;
      break;
     case 'T':
      *sn += 3;
      break;
     default:
      *sn += 4;
      break;
    }
    *(sn+1) = *sn;
    seq++;
    switch (*(seq-epi))
    {
     case 'A':
      *(sn+1) -= (0 * k);
      break;
     case 'C':
      *(sn+1) -= (1 * k);
      break;
     case 'G':
      *(sn+1) -= (2 * k);
      break;
     case 'T':
      *(sn+1) -= (3 * k);
      break;
     default:
      *(sn+1) -= (4 * k);
      break;
    }
    sn++;
  }
  return seqnumarray;
}

static Int4 PredictEpiI1 (CharPtr seqin, Int4 start, Int4 end, Int4 window,
                          EpiDatPtr epip, FloatHiPtr fptrhead,
                          FloatHi percentcut, Int4 episode)
{
  Int4       i, j, k, kpow;
  CharPtr    seq;
  FloatHiPtr fptr;
  FloatHi    maxscore, episcore, percentscore;
  Int4Ptr    sn, seqnumarray;

  if (seqin == NULL || epip == NULL)
    return -1;

  seq = &seqin[start];
  fptr = &fptrhead[start];

/* "zero" array for every cycle */
  MemFree (epip->epiarray);
  kpow = (Int4) pow (POSSIBLE_NA_RESIDUES, (FloatHi) episode);
  if ((epip->epiarray = (Int4Ptr) MemNew ((size_t)
       (sizeof (Int4) * (kpow+1)))) == NULL)
  {
    return -1;
  }
  for (j = 0; j < kpow; j++)
    epip->epiarray[j] = 1;

/* ACGTN for nucleotide sequence */
  if ((sn = seqnumarray = NumericSequence (seq, start, end, episode)) == NULL)
    return -1;

/* min_window_mer set up */
  k = start + window - 1;
  i = 0;
  for (j = start; j < k && j <= end; j++)
  {
    epip->epiarray[sn[i]] += 1;
    i++;
  }
  maxscore = MaxScore (window, kpow);
  episcore = NucArrayScore (epip->epiarray, kpow);

/* run -- rolling scan */
  for (j = k; j <= end; j++)
  {
    episcore = DownArrayScore (episcore, epip->epiarray[sn[i]]);
    epip->epiarray[sn[i]] += 1;
    episcore = UpArrayScore (episcore, epip->epiarray[sn[i]]);
    if ((percentscore = PercentEpiScore (episcore, window, maxscore))
          <= percentcut)
      UpdateScore (fptr, percentscore, i-window+1, i+episode-1);
    fptr++;
    i++;
    episcore = DownArrayScore (episcore, epip->epiarray[sn[i-window]]);
    epip->epiarray[sn[i-window]] -= 1;
    episcore = UpArrayScore (episcore, epip->epiarray[sn[i-window]]);
  }
  MemFree (seqnumarray);
  return (i - 1);
}

extern FloatHiPtr PredictEpi (CharPtr seqin, Int4 start, Int4 end,
                              EpiDatPtr epip, Boolean flagIsAA)
{
  Int4       i, n, retval;
  FloatHiPtr epiprob, epitemp;
  FloatHi    pcut, dcut;
  Int4       minwin;
  FloatHi    mcut;

  if (seqin == NULL || epip == NULL)
    return NULL;

  if (start + (epip->window - 1) > end)
    return NULL;

  if ((epiprob = (FloatHiPtr) MemNew ((size_t) (sizeof (FloatHi) *
                                      (end-start+2)))) == NULL)
  {
    return NULL;
  }

  if ((epitemp = (FloatHiPtr) MemNew ((size_t) (sizeof (FloatHi) *
                                      (end-start+2)))) == NULL)
  {
    MemFree (epiprob);
    return NULL;
  }

  minwin = epip->mwindow;
  mcut = epip->mpercentcut;
  for (i = start; i <= end; i++)
  {
    epiprob[i] = 100.0;
    epitemp[i] = 100.0;
  }
  dcut = (epip->percentcut - mcut) / (FloatHi) (epip->window - minwin);
  for (n = minwin; n <= epip->window; n++)
  {
    pcut = mcut + ((FloatHi) (n - minwin) * dcut);
    if (flagIsAA)
      retval = PredictEpiI0 (seqin, start, end, n, epip, epitemp, pcut);
    else
      retval = PredictEpiI1 (seqin, start, end, n, epip, epitemp, pcut,
               epip->nucleotuple);
    if (retval < 0)
    {
      ErrPostEx (SEV_ERROR, 1, 1, "EPI prediction error");
      ErrShow ();
      break;
    }
    for (i = start; i <= end; i++)
    {
      if (epitemp[i] < epiprob[i])
        epiprob[i] = epitemp[i];
      epitemp[i] = 100.0;
    }
  }
  MemFree (epitemp);
  return epiprob;
}

/*
 seqport should be opened full length (0 to bsp->length-1)
 start and end reflect where you want to search
*/

extern FloatHiPtr PredictEpiSeqPort (SeqPortPtr spp,
                                     Int4 start, Int4 end,
                                     EpiDatPtr epip, Boolean flagIsAA)
{
  Int4        i, chunk;
  Uint1Ptr    seq, sq;
  FloatHiPtr  epiprob;

  if (spp == NULL || epip == NULL)
    return NULL;

  sq = seq = (Uint1Ptr) MemNew ((size_t) (sizeof (Char) * (end-start+2)));
  if (seq == NULL)
    return NULL;

  i = start;
  chunk = 4096;
  while (i < end)
  {
    SeqPortRead (spp, sq, (Int2) chunk);
    sq += chunk;
    i += chunk;
  }
  seq[end-start+1] = 0;
  epip->sequence = seq;
  epiprob = PredictEpi ((CharPtr) seq, start, end, epip, flagIsAA);
  return epiprob;
}

extern FloatHiPtr PredictEpiBioseq (BioseqPtr bsp,
                                    Int4 start, Int4 end,
                                    EpiDatPtr epip)
{
  SeqPortPtr spp;
  FloatHiPtr epiprob;
  Boolean    flagIsAA;

  if (bsp == NULL || epip == NULL)
    return NULL;

  flagIsAA = (Boolean) ISA_aa (bsp->mol);

  if (flagIsAA)
    spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_iupacaa);
  else
    spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_iupacna);
  epiprob = PredictEpiSeqPort (spp, start, end, epip, flagIsAA);
  SeqPortFree (spp);
  return epiprob;
}

extern FloatHiPtr PredictEpiSeqLoc (SeqLocPtr slp,
                                    EpiDatPtr epip)
{
  BioseqPtr  bsp;
  Int4       start, end;
  FloatHiPtr epiprob;

  if (slp == NULL || epip == NULL)
    return NULL;

  if (slp->choice != SEQLOC_INT)
    return NULL;

  if ((bsp = BioseqLockById (SeqLocId (slp))) == NULL)
    return NULL;

  start = SeqLocStart (slp);
  end = SeqLocStop (slp);
  epiprob = PredictEpiBioseq (bsp, start, end, epip);
  BioseqUnlock (bsp);
  return epiprob;
}

static void OptimizeEpiComp (CharPtr seq, Int4 window, SeqLocPtr slp,
                             Boolean flagIsAA)
{
  Int4      i, start, stop, span;
  SeqIntPtr sint;
  Int4      bintot, bin[256];
  FloatHi   expect, fbin[256];

  if (seq == NULL || slp == NULL)
    return;
  if (StrLen (seq) == 0)
    return;

  while (slp != NULL)
  {
    bintot = 0;
    for (i = (Int4) 'A'; i <= (Int4) 'Z'; i++)
      bin[i] = 0;
    start = SeqLocStart (slp);
    stop = SeqLocStop (slp);
/* don't have to examine the whole sequence; just end optimization */
    if (flagIsAA)
    {
      span = (stop - start) / 2;
      if (span > window / 2)
        span = window / 2;
    }
    else
    {
      span = (stop - start) - 3;
      if (span > window - 2)
        span = window - 2;
    }
    if (flagIsAA)
    {
      for (i = start; i <= stop; i++)
      {
        switch (seq[i])
        {
         case '*':
         case 'B':
         case 'J':
         case 'O':
         case 'U':
         case 'X':
         case 'Z':
          bin['X'] += 1;
          bintot++;
          break;
         default:
          bin[seq[i]] += 1;
          bintot++;
          break;
        }
      }
    }
    else
    {
      for (i = start; i <= stop; i++)
      {
        switch (seq[i])
        {
         case 'A':
         case 'C':
         case 'G':
         case 'T':
          bin[seq[i]] += 1;
          bintot++;
          break;
         default:
          bin['N'] += 1;
          bintot++;
          break;
        }
      }
    }
/* very small windows biased */
    expect = 1.0;
    if (flagIsAA)
    {
      if (bintot > POSSIBLE_AA_RESIDUES/2)
        expect++;
      expect += (FloatHi) (bintot / POSSIBLE_AA_RESIDUES);
    }
    else
    {
      if (bintot > POSSIBLE_NA_RESIDUES/2)
        expect++;
      expect += (FloatHi) (bintot / POSSIBLE_NA_RESIDUES);
    }
    expect /= (FloatHi) bintot;
    for (i = (Int4) 'A'; i <= (Int4) 'Z'; i++)
      fbin[i] = (FloatHi) bin[i] / (FloatHi) bintot;
    sint = slp->data.ptrvalue;
    for (i = start; i <= start + span; i++)
    {
      if (fbin[seq[i]] > expect)
        break;
      sint->from += 1;
    }
    for (i = stop; i >= stop - span; i--)
    {
      if (fbin[seq[i]] > expect)
        break;
      sint->to -= 1;
    }
    slp = slp->next;
  }
  return;
}

typedef struct HPArray
{
  FloatHi score;
  Int4    left, right;
  struct  HPArray PNTR next;
} HPA, PNTR HPAPtr;

static int LIBCALLBACK HPACompProc (VoidPtr ptr1, VoidPtr ptr2)
{
  HPAPtr hpap1, hpap2;

  if (ptr1 != NULL && ptr2 != NULL)
  {
    hpap1 = *((HPAPtr PNTR) ptr1);
    hpap2 = *((HPAPtr PNTR) ptr2);
    if (hpap1 != NULL && hpap2 != NULL)
    {
      if (hpap1->score > hpap2->score)
        return 1;
      else if (hpap1->score < hpap2->score)
        return -1;
    }
  }
  return 0;
}

extern void SortHPArray (HPAPtr PNTR hpap)
{
  HPAPtr hpapt, PNTR hpaph;
  Int4 num, i;

  num = 0;
  hpapt = *hpap;
  while (hpapt != NULL)
  {
    num++;
    hpapt = hpapt->next;
  }

  if (num > 1)
  {
    hpaph = MemNew ((size_t) ((num+1) * sizeof (HPAPtr)));
    i = 0;
    hpapt = *hpap;
    while (hpapt != NULL && i < num)
    {
      hpaph[i] = hpapt;
      hpapt = hpapt->next;
      i++;
    }
    HeapSort (hpaph, num, sizeof (HPAPtr), HPACompProc);
    for (i = 0; i < num; ++i)
    {
      hpapt = hpaph[i];
      hpapt->next = hpaph[i+1];
    }
    *hpap = hpaph[0];
    MemFree (hpaph);
  }
  return;
}

static void OptimizeEpiHP (CharPtr seq, Int4 window, SeqLocPtr slp,
                           Boolean flagIsAA)
{
  Int4      i, n, j, k, start, stop, span;
  SeqIntPtr sint;
  HPAPtr    hpaph = NULL, hpapt, hpap;
  EpiDatPtr epip;
  FloatHi   mincom, maxcom;
  Int4      wincom;

  if (seq == NULL || slp == NULL)
    return;
  if (StrLen (seq) == 0)
    return;

  epip = EpiDatNew ();
  while (slp != NULL)
  {
    sint = slp->data.ptrvalue;
    start = SeqLocStart (slp);
    stop = SeqLocStop (slp);
/* don't have to examine the whole sequence; just end optimization */
    if (flagIsAA)
    {
      span = (stop - start) / 2;
      if (span > window / 2)
        span = window / 2;
    }
    else
    {
      span = (stop - start) - 3;
      if (span > window - 2)
        span = window - 2;
    }
    for (i = start; i <= start + span; i++)
    {
      for (k = (Int4) 'A'; k <= (Int4) 'Z'; k++)
        epip->epiarray[k] = 1;
      if (flagIsAA)
      {
        for (j = i; j <= stop; j++)
        {
          switch (seq[j])
          {
           case '*':
           case 'B':
           case 'J':
           case 'O':
           case 'U':
           case 'X':
           case 'Z':
            epip->epiarray['X'] += 1;
            break;
           default:
            epip->epiarray[seq[j]] += 1;
            break;
          }
        }
      }
      else
      {
        for (j = i; j <= stop; j++)
        {
          switch (seq[j])
          {
           case 'A':
           case 'C':
           case 'G':
           case 'T':
            epip->epiarray[seq[j]] += 1;
            break;
           default:
            epip->epiarray['N'] += 1;
            break;
          }
        }
      }
      for (n = stop; n >= stop - span; n--)
      {
        hpap = (HPAPtr) MemNew (sizeof (HPA));
        if (hpaph == NULL)
        {
          hpaph = hpap;
        }
        else
        {
          hpapt = hpaph;
          while (hpapt->next != NULL)
            hpapt = hpapt->next;
          hpapt->next = hpap;
        }
        hpap->score = ArrayScore (epip->epiarray);
        hpap->left = i;
        hpap->right = n;
        wincom = hpap->right - hpap->left + 1;
        maxcom = MaxScore (wincom, POSSIBLE_AA_RESIDUES);
        hpap->score = PercentEpiScore (hpap->score, wincom, maxcom);
        switch (seq[n])
        {
         case '*':
         case 'B':
         case 'J':
         case 'O':
         case 'U':
         case 'X':
         case 'Z':
          epip->epiarray['X'] -= 1;
          break;
         default:
          epip->epiarray[seq[n]] -= 1;
          break;
        }
      }
    }
    if (hpaph != NULL)
    {
      SortHPArray (&hpaph);
      hpap = hpaph;
      mincom = hpap->score;
      sint->from = hpap->left;
      sint->to = hpap->right;
      while (hpap != NULL && mincom >= hpap->score)
      {
        if (sint->from > hpap->left)
          sint->from = hpap->left;
        if (sint->to < hpap->right)
          sint->to = hpap->right;
        hpap = hpap->next;
      }
      hpap = hpaph;
      while (hpap != NULL)
      {
        hpaph = hpap->next;
        MemFree (hpap);
        hpap = hpaph;
      }
    }
    slp = slp->next;
  }
  EpiDatFree (epip);
  return;
}

typedef struct CFArray
{
  FloatHi score;
  Int4    left, right;
  struct  CFArray PNTR next;
} CFA, PNTR CFAPtr;

static int LIBCALLBACK CFACompProc (VoidPtr ptr1, VoidPtr ptr2)
{
  CFAPtr cfap1, cfap2;

  if (ptr1 != NULL && ptr2 != NULL)
  {
    cfap1 = *((CFAPtr PNTR) ptr1);
    cfap2 = *((CFAPtr PNTR) ptr2);
    if (cfap1 != NULL && cfap2 != NULL)
    {
      if (cfap1->score > cfap2->score)
        return 1;
      else if (cfap1->score < cfap2->score)
        return -1;
    }
  }
  return 0;
}

extern void SortCFArray (CFAPtr PNTR cfap)
{
  CFAPtr cfapt, PNTR cfaph;
  Int4 num, i;

  num = 0;
  cfapt = *cfap;
  while (cfapt != NULL)
  {
    num++;
    cfapt = cfapt->next;
  }

  if (num > 1)
  {
    cfaph = MemNew ((size_t) ((num+1) * sizeof (CFAPtr)));
    i = 0;
    cfapt = *cfap;
    while (cfapt != NULL && i < num)
    {
      cfaph[i] = cfapt;
      cfapt = cfapt->next;
      i++;
    }
    HeapSort (cfaph, num, sizeof (CFAPtr), CFACompProc);
    for (i = 0; i < num; ++i)
    {
      cfapt = cfaph[i];
      cfapt->next = cfaph[i+1];
    }
    *cfap = cfaph[0];
    MemFree (cfaph);
  }
  return;
}

static void OptimizeEpiProbFunc (CharPtr seq, Int4 window, SeqLocPtr slp,
                                 Boolean flagIsAA)
{
  Int4      i, n, j, k, start, stop, loop, fact, left, right;
  Int4      factarr[32], PNTR dfactarr;
  Int4      alphabet;
  FloatHi   score, tscore;
  EpiDatPtr epip;
  CFAPtr    cfaph = NULL, cfapt, cfap;
  SeqIntPtr sint;

  if (seq == NULL || slp == NULL)
    return;
  if (StrLen (seq) == 0)
    return;

  if (flagIsAA)
    alphabet = 21;
  else
    alphabet = 5;
  epip = EpiDatNew ();
  dfactarr = (Int4Ptr) MemNew ((size_t) (sizeof (Int4) * window));
  while (slp != NULL)
  {
    sint = slp->data.ptrvalue;
    start = SeqLocStart (slp);
    stop = SeqLocStop (slp);
    for (i = start; i <= stop - window; i++)
    {
      for (k = (Int4) 'A'; k <= (Int4) 'Z'; k++)
        epip->epiarray[k] = 0;
      loop = 0;
      score = -1.0;
      for (n = i; n <= stop; n++)
      {
        if (flagIsAA)
        {
          switch (seq[n])
          {
           case '*':
           case 'B':
           case 'J':
           case 'O':
           case 'U':
           case 'X':
           case 'Z':
            epip->epiarray['X'] += 1;
            break;
           default:
            epip->epiarray[seq[n]] += 1;
            break;
          }
        }
        else
        {
          switch (seq[n])
          {
           case 'A':
           case 'C':
           case 'G':
           case 'T':
            epip->epiarray[seq[n]] += 1;
            break;
           default:
            epip->epiarray['N'] += 1;
            break;
          }
        }
        loop++;
        if (loop >= window)
        {
          for (j = 0; j < 32; j++)
            factarr[j] = 0;
          for (j = 0; j < window; j++)
            dfactarr[j] = 0;
          for (j = 0, k = (Int4) 'A'; k <= (Int4) 'Z'; j++, k++)
            if (epip->epiarray[k] > 0)
              factarr[j] = epip->epiarray[k];
          for (j = 0; j < 32; j++)
            if (factarr[j] > 0)
              dfactarr[factarr[j]] += 1;
          tscore = 1.0;
/* window */
          for (j = 2; j <= loop; j++)
            tscore *= (FloatHi) j;
/* residues */
          for (j = 0; j < 32; j++)
            for (k = 2; k <= factarr[j]; k++)
              tscore /= (FloatHi) k;
/* alphabet */
          for (j = 2; j <= alphabet; j++)
            tscore *= (FloatHi) j;
/* counts */
          for (j = 0; j < window; j++)
            for (k = 2; k <= dfactarr[j]; k++)
              tscore /= (FloatHi) k;
          fact = 0;
          for (j = 0; j < window; j++)
            if (dfactarr[j] > 0)
              fact++;
          fact = alphabet - fact;
          for (j = 2; j < fact; j++)
            tscore /= (FloatHi) j;
/* score */
          if (score < 0.0)
          {
            left = i;
            right = n;
            score = tscore;
          }
          if (tscore < score)
          {
            left = i;
            right = n;
            score = tscore;
          }
        }
        cfap = (CFAPtr) MemNew (sizeof (CFA));
        cfap->score = score;
        cfap->left = left;
        cfap->right = right;
        if (cfaph == NULL)
        {
          cfaph = cfap;
        }
        else
        {
          cfapt = cfaph;
          while (cfapt->next != NULL)
            cfapt = cfapt->next;
          cfapt->next = cfap;
        }
      }
      if (cfaph != NULL)
      {
        SortCFArray (&cfaph);
        cfap = cfaph;
        tscore = cfap->score;
        sint->from = cfap->left;
        sint->to = cfap->right;
        while (cfap != NULL && tscore >= cfap->score)
        {
          if (sint->from > cfap->left)
            sint->from = cfap->left;
          if (sint->to < cfap->right)
            sint->to = cfap->right;
          cfap = cfap->next;
        }
        cfap = cfaph;
        while (cfap != NULL)
        {
          cfaph = cfap->next;
          MemFree (cfap);
          cfap = cfaph;
        }
      }
    }
    slp = slp->next;
  }
  EpiDatFree (epip);
  MemFree (dfactarr);
  return;
}

extern SeqLocPtr FilterEpi (EpiDatPtr epip, CharPtr seq, Int4 length,
                            SeqIdPtr sip, Boolean flagHighPass,
                            Boolean flagIsAA,
                            Boolean flagBoundaryCondition)
{
  Int4       i;
  Int4       start, stop;
  SeqLocPtr  nextslp, slp, slptmp, slphp, slph = NULL;
  SeqIntPtr  sint;
  FloatHi    curscore, lopr, hipr;

  if (epip == NULL || epip->score == NULL)
    return NULL;

  if (flagBoundaryCondition)
  {
    lopr = epip->percentcut * 0.85;
    hipr = epip->percentcut + (epip->percentcut - lopr);
    for (i = 0; i < length; i++)
    {
      if (epip->score[i] >= lopr && epip->score[i] <= hipr)
        break;
    }
    if (i == length)
      return NULL;

    while (i < length)
    {
      curscore = epip->score[i];
      if (epip->score[i] >= lopr && epip->score[i] <= hipr)
      {
        start = i;
        while (epip->score[i] >= lopr && epip->score[i] <= hipr && i < length)
        {
          if (epip->score[i] > curscore)
            curscore = epip->score[i];
          i++;
        }
        stop = i - 1;
        slp = SeqLocIntNew (start, stop, Seq_strand_unknown, sip);
        ValNodeLink (&slph, slp);
      }
      else
      {
        i++;
      }
    }
  }
  else
  {
    for (i = 0; i < length; i++)
    {
      if (epip->score[i] <= epip->percentcut)
        break;
    }
    if (i == length)
      return NULL;

    while (i < length)
    {
      curscore = epip->score[i];
      if (epip->score[i] <= epip->percentcut)
      {
        start = i;
        while (epip->score[i] <= epip->percentcut && i < length)
        {
          if (epip->score[i] > curscore)
            curscore = epip->score[i];
          i++;
        }
        stop = i - 1;
        slp = SeqLocIntNew (start, stop, Seq_strand_unknown, sip);
        ValNodeLink (&slph, slp);
      }
      else
      {
        i++;
      }
    }
  }

  switch (epip->method)
  {
   default:
   case 0:
    break;
   case 1:
    OptimizeEpiComp (seq, epip->window, slph, flagIsAA);
    break;
   case 2:
    OptimizeEpiHP (seq, epip->window, slph, flagIsAA);
    break;
   case 3:
    OptimizeEpiProbFunc (seq, epip->window, slph, flagIsAA);
    break;
  }

  if (epip->linker > 0)
  {
    slp = slph;
    while (slp != NULL)
    {
      if (slp->next != NULL)
      {
        nextslp = slp->next;
        stop = SeqLocStop (slp);
        start = SeqLocStart (nextslp);
        if (start-stop-1 <= epip->linker)
        {
          sint = slp->data.ptrvalue;
          sint->to = SeqLocStop (nextslp);
          slp->next = nextslp->next;
          nextslp->next = NULL;
          SeqLocFree (nextslp);
          continue;
        }
      }
      slp = slp->next;
    }
  }

  if (flagHighPass)
  {
    slp = slph;
    slphp = NULL;
    stop = -1;
    while (slp != NULL)
    {
      start = SeqLocStart (slp);
      if (start > 0)
      {
        slptmp = SeqLocIntNew (stop+1, start-1, Seq_strand_unknown, sip);
        ValNodeLink (&slphp, slptmp);
      }
      stop = SeqLocStop (slp);
      nextslp = slp->next;
      slp->next = NULL;
      SeqLocFree (slp);
      slp = nextslp;
    }
    if (stop != length-1)
    {
      slptmp = SeqLocIntNew (stop+1, length-1, Seq_strand_unknown, sip);
      ValNodeLink (&slphp, slptmp);
    }
    slph = slphp;
  }

  return slph;
}

extern SeqLocPtr FilterEpiBioseq (EpiDatPtr epip, BioseqPtr bsp,
                                  Boolean flagHighPass,
                                  Boolean flagBoundaryCondition)
{
  Int4       i, chunk;
  SeqLocPtr  slp;
  SeqPortPtr spp;
  Uint1Ptr   sq;
  Boolean    flagIsAA;

  if (epip == NULL || epip->score == NULL || bsp == NULL)
    return NULL;

  flagIsAA = (Boolean) ISA_aa (bsp->mol);

  if (epip->method != 0)
  {
    if (epip->sequence == NULL)
    {
      sq = epip->sequence = (Uint1Ptr) MemNew ((size_t) (sizeof (Char) *
                                               (bsp->length + 2)));
      if (flagIsAA)
        spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_iupacaa);
      else
        spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_iupacna);
      i = 0;
      chunk = 4096;
      while (i < bsp->length)
      {
        SeqPortRead (spp, sq, (Int2) chunk);
        sq += chunk;
        i += chunk;
      }
      epip->sequence[bsp->length] = 0;
      SeqPortFree (spp);
    }
    slp = FilterEpi (epip, (CharPtr) epip->sequence, bsp->length, bsp->id,
                     flagHighPass, flagIsAA, flagBoundaryCondition);
  }
  else
  {
    slp = FilterEpi (epip, NULL, bsp->length, bsp->id, flagHighPass, flagIsAA,
                     flagBoundaryCondition);
  }

  return slp;
}
