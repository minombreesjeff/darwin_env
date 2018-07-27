static char const rcsid[] = "$Id: urkpcc.c,v 6.13 2003/05/30 17:25:38 coulouri Exp $";

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
* File Name: urkpcc.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.13 $
*
* File Description: coiled-coils
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urkpcc.c,v $
* Revision 6.13  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.12  1998/11/16 14:29:49  kuzio
* flagBoundaryCondition
*
* Revision 6.11  1998/09/16 18:03:35  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#include <urkpcc.h>

extern Int4 ReadPccData (PccDatPtr pccp)
{
  FILE    *fin;
  Int4    i, n, val, count;
  FloatHi score;
  Char    buf[PATH_MAX], buff[PATH_MAX], tbuf[4];
  CharPtr bptr;

  if (pccp == NULL)
    return 0;

  if (pccp->pccdatafile == NULL || pccp->scr == NULL)
    return 0;

  if (pccp->window < 7 || pccp->window > 42)
    pccp->window = 22;

  if (FindPath ("ncbi", "ncbi", "data", buf, sizeof (buf)))
    FileBuildPath (buf, NULL, pccp->pccdatafile);
  else
    StrCpy (buf, pccp->pccdatafile);

  if ((fin = FileOpen (buf, "r")) == NULL)
  {
    if ((fin = FileOpen (pccp->pccdatafile, "r")) == NULL)
    {
      return 0;
    }
  }

/* no data available for selenocysteine U -- 24 characters */
  pccp->res = StringSave ("ABCDEFGHIKLMNPQRSTVUWXYZ* ");
  pccp->res[0] = '\0';

  val = 0;
  while ((FileGets (buff, sizeof (buff), fin)) != NULL)
  {
    if ((bptr = StrChr (buff, ';')) != NULL)
      *bptr = '\0';
    if (StrLen (buff) < 16)
      continue;
    bptr = buff;
    while (IS_WHITESP (*bptr))
      bptr++;
    sprintf (tbuf, "%c", *bptr);
    StrCat (pccp->res, tbuf);
    for (n = 0; n < 7; n++)
    {
      while (!IS_WHITESP (*bptr))
        bptr++;
      while (IS_WHITESP (*bptr))
        bptr++;
      sscanf (bptr, "%lf", &score);
      pccp->scr[val][n] = score;
    }
    val++;
    if (val == 24)
      break;
  }
  FileClose (fin);
  count = val;

  for (n = 7; n < pccp->window; n++)
  {
    if (n == 42)
      break;
    val = n % 7;
    for (i = 0; i < count; i++)
      pccp->scr[i][n] = pccp->scr[i][val];
  }
  return count;
}

extern PccDatPtr PccDatNew (void)
{
  PccDatPtr pccp;
  Int4      i;

  if ((pccp = (PccDatPtr) MemNew (sizeof (PccDat))) != NULL)
  {
    pccp->meang  = 0.77;
    pccp->stdg   = 0.20;
    pccp->meanc  = 1.63;
    pccp->stdc   = 0.24;
    pccp->window = 22;
    pccp->linker = 32;

    pccp->pccdatafile = StringSave ("KSpcc.mat");

    if ((pccp->scr = (Pointer) MemNew ((size_t)(sizeof(Pointer)*24)))
         != NULL)
    {
      for (i = 0; i < 24; i++)
      {
        pccp->scr[i] = (FloatHiPtr)
                        MemNew ((size_t)(sizeof(FloatHi)*42));
      }
    }
    else
    {
      pccp = (PccDatPtr) MemFree (pccp);
    }
  }
  return pccp;
}

extern PccDatPtr PccDatFree (PccDatPtr pccp)
{
  Int4      i;

  if (pccp != NULL)
  {
    MemFree (pccp->pccdatafile);
    MemFree (pccp->res);
    for (i = 0; i < 24; i++)
      MemFree (pccp->scr[i]);
    MemFree (pccp->scr);
    pccp = (PccDatPtr) MemFree (pccp);
  }
  return pccp;
}

static FloatHi ProbPredictCC (FloatHi score, PccDatPtr pccp,
                              FloatHi sqrt2pi)
{
  FloatHi   constg, constc;
  FloatHi   evalg, evalc;
  FloatHi   Gg, Gc;
  FloatHi   prob;

  constg = 1.0 / (pccp->stdg * sqrt2pi);
  constc = 1.0 / (pccp->stdc * sqrt2pi);

  evalg = (((score - pccp->meang) / pccp->stdg) *
           ((score - pccp->meang) / pccp->stdg)) / -2.0;
  evalc = (((score - pccp->meanc) / pccp->stdc) *
           ((score - pccp->meanc) / pccp->stdc)) / -2.0;

  evalg = exp (evalg);
  evalc = exp (evalc);
  Gg = constg * evalg;
  Gc = constc * evalc;
  prob = Gc / ((30 * Gg) + Gc);

  return prob;
}

static FloatHi nroot (FloatHi fv, Int4 wv)
{
  FloatHi  fi = 1.0;

  fi = fi / (FloatHi) wv;
  fv = pow (fv, fi);

  return fv;
}

extern FloatHiPtr PredictCC (CharPtr seq, Int4 start, Int4 end,
                             PccDatPtr pccp)
{
  Int4        i, n, col;
  Int4        stop;
  CharPtr     seqhead;
  FloatHiPtr  fptrhead, fptrtemp, fptr;
  FloatHi     sqrt2pi, temp;
  Boolean     flagMatch;

  if (seq == NULL || pccp == NULL)
    return NULL;

  if ((fptr = (FloatHiPtr) MemNew ((size_t) (sizeof (FloatHi) *
                                   (end-start+1+pccp->window)))) == NULL)
  {
    return fptr;
  }

  sqrt2pi = nroot ((2.0*PI), 2);

  fptrhead = fptrtemp = fptr;
  seqhead = seq;
  stop = end - pccp->window;
  for (i = start; i < stop; i++)
  {
    seq = seqhead;
    temp = 1.0;
    for (col = 0; col < pccp->window; col++)
    {
      flagMatch = FALSE;
      n = 0;
      while (pccp->res[n] != '\0')
      {
        if (*seq == pccp->res[n])
        {
          flagMatch = TRUE;
          break;
        }
        n++;
      }
      if (flagMatch)
        temp *= pccp->scr[n][col];
      seq++;
    }
    temp = nroot (temp, pccp->window);
    temp = ProbPredictCC (temp, pccp, sqrt2pi);
    if (temp < 0.0)
      temp = 0.0;
    fptr = fptrtemp;
    for (n = 0; n < pccp->window; n++)
    {
      if (temp > *fptr)
        *fptr = temp;
      fptr++;
    }
    fptrtemp++;
    seqhead++;
  }
  return fptrhead;
}

/*
 seqport should be opened full length (0 to bsp->length-1)
 start and end reflect where you want to search
*/

extern FloatHiPtr PredictCCSeqPort (SeqPortPtr spp,
                                    Int4 start, Int4 end,
                                    PccDatPtr pccp)
{
  CharPtr    seq, seqhead;
  Int4       i;
  FloatHiPtr pccprob;

  if (spp == NULL || pccp == NULL)
    return NULL;

  seq = seqhead = (CharPtr) MemNew ((size_t) (sizeof (Char) *
                                    (end-start+1)));
  if (seq == NULL)
    return NULL;

  SeqPortSeek (spp, start, SEEK_SET);
  for (i = start; i <= end; i++)
  {
    *seq = SeqPortGetResidue (spp);
    seq++;
  }
  pccprob = PredictCC (seqhead, start, end, pccp);
  MemFree (seqhead);
  return pccprob;
}

extern FloatHiPtr PredictCCBioseq (BioseqPtr bsp,
                                   Int4 start, Int4 end,
                                   PccDatPtr pccp)
{
  SeqPortPtr spp;
  FloatHiPtr pccprob;

  if (bsp == NULL || pccp == NULL)
    return NULL;

  if (!ISA_aa (bsp->mol))
    return NULL;

  spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_iupacaa);
  pccprob = PredictCCSeqPort (spp, start, end, pccp);
  SeqPortFree (spp);
  return pccprob;
}

extern FloatHiPtr PredictCCSeqLoc (SeqLocPtr slp,
                                   PccDatPtr pccp)
{
  BioseqPtr  bsp;
  Int4       start, end;
  FloatHiPtr pccprob;

  if (slp == NULL || pccp == NULL)
    return NULL;

  if (slp->choice != SEQLOC_INT)
    return NULL;

  if ((bsp = BioseqLockById (SeqLocId (slp))) == NULL)
    return NULL;

  if (!ISA_aa (bsp->mol))
  {
    BioseqUnlock (bsp);
    return NULL;
  }

  start = SeqLocStart (slp);
  end = SeqLocStop (slp);
  pccprob = PredictCCBioseq (bsp, start, end, pccp);
  BioseqUnlock (bsp);
  return pccprob;
}

extern SeqLocPtr FilterCC (FloatHiPtr score, FloatHi percentcut,
                           Int4 length, Int4 linker, SeqIdPtr sip,
                           Boolean flagBoundaryCondition)
{
  Int4       i;
  Int4       start, stop;
  FloatHi    lopr, hipr;
  SeqLocPtr  nextslp, slp, slph = NULL;
  SeqIntPtr  sint;

  if (score == NULL)
    return NULL;

  percentcut /= 100.0;
  lopr = percentcut * 0.80;
  hipr = percentcut + (percentcut - lopr);

  if (flagBoundaryCondition)
  {
    for (i = 0; i < length; i++)
    {
      if (*score >= lopr && *score <= hipr)
        break;
      score++;
    }
  }
  else
  {
    for (i = 0; i < length; i++)
    {
      if (*score > percentcut)
        break;
      score++;
    }
  }
  if (i == length)
    return NULL;

  if (flagBoundaryCondition)
  {
    while (i < length)
    {
      if (*score >= lopr && *score <= hipr)
      {
        start = i;
        while (*score >= lopr && *score <= hipr && i < length)
        {
          score++;
          i++;
        }
        stop = i - 1;
        slp = SeqLocIntNew (start, stop, Seq_strand_unknown, sip);
        ValNodeLink (&slph, slp);
      }
      else
      {
        score++;
        i++;
      }
    }
  }
  else
  {
    while (i < length)
    {
      if (*score > percentcut)
      {
        start = i;
        while (*score > percentcut && i < length)
        {
          score++;
          i++;
        }
        stop = i - 1;
        slp = SeqLocIntNew (start, stop, Seq_strand_unknown, sip);
        ValNodeLink (&slph, slp);
      }
      else
      {
        score++;
        i++;
      }
    }
  }

  if (linker > 0)
  {
    slp = slph;
    while (slp != NULL)
    {
      if (slp->next != NULL)
      {
        nextslp = slp->next;
        stop = SeqLocStop (slp);
        start = SeqLocStart (nextslp);
        if (start-stop-1 <= linker)
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

  return slph;
}

extern SeqLocPtr FilterCCVS (FloatHiPtr score, FloatHi percentcut,
                             Int4 length, Int4 linker, SeqIdPtr sip,
                             Boolean flagBoundaryCondition)

{
  Int4       i;
  Int4       start, stop;
  FloatHi    lopr, hipr;
  SeqLocPtr  nextslp, slp, slph = NULL;
  SeqIntPtr  sint;
  Int4       cccount;

  if (score == NULL)
    return NULL;

  percentcut /= 100.0;
  lopr = percentcut * 0.80;
  hipr = percentcut + (percentcut - lopr);

  if (flagBoundaryCondition)
  {
    for (i = 0; i < length; i++)
    {
      if (*score >= lopr && *score <= hipr)
        break;
      score++;
    }
  }
  else
  {
    for (i = 0; i < length; i++)
    {
      if (*score > percentcut)
        break;
      score++;
    }
  }
  if (i == length)
    return NULL;

  if (flagBoundaryCondition)
  {
    while (i < length)
    {
      if (*score >= lopr && *score <= hipr)
      {
        start = i;
        while (*score >= lopr && *score <= hipr && i < length)
        {
          score++;
          i++;
        }
        stop = i - 1;
        slp = SeqLocIntNew (start, stop, Seq_strand_unknown, sip);
        ValNodeLink (&slph, slp);
      }
      else
      {
        score++;
        i++;
      }
    }
  }
  else
  {
    while (i < length)
    {
      if (*score > percentcut)
      {
        start = i;
        while (*score > percentcut && i < length)
        {
          score++;
          i++;
        }
        stop = i - 1;
        slp = SeqLocIntNew (start, stop, Seq_strand_unknown, sip);
        ValNodeLink (&slph, slp);
      }
      else
      {
        score++;
        i++;
      }
    }
  }

  if (linker > 0)
  {
    cccount = 0;
    slp = slph;
    while (slp != NULL)
    {
      cccount++;
      slp = slp->next;
    }
    linker *= (cccount-1);
    slp = slph;
    while (slp != NULL)
    {
      if (slp->next != NULL)
      {
        nextslp = slp->next;
        stop = SeqLocStop (slp);
        start = SeqLocStart (nextslp);
        if (start-stop-1 <= linker)
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

  return slph;
}
