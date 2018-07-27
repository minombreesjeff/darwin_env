static char const rcsid[] = "$Id: urkfltr.c,v 6.8 2003/05/30 17:25:38 coulouri Exp $";

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
* File Name: urkfltr.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.8 $
*
* File Description: filter
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urkfltr.c,v $
* Revision 6.8  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.7  1998/12/23 21:34:04  kuzio
* SeqPortStuff
*
* Revision 6.6  1998/09/16 18:03:34  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#include <urkfltr.h>

extern Int4 ReadFilterData (FilterDatPtr fltp)
{
  FILE    *fin;
  Int2    i, val;
  Int4    type;
  FloatHi score, temp;
  Char    buf[PATH_MAX], buff[PATH_MAX];

  if (fltp == NULL)
    return 0;

  if (fltp->filterdatafile == NULL || fltp->scr == NULL)
    return 0;

  if (FindPath ("ncbi", "ncbi", "data", buf, sizeof (buf)))
    FileBuildPath (buf, NULL, fltp->filterdatafile);
  else
    StrCpy(buf, fltp->filterdatafile);

  if ((fin = FileOpen (buf, "r")) == NULL)
  {
    if ((fin = FileOpen (fltp->filterdatafile, "r")) == NULL)
    {
      return 0;
    }
  }

  val = 0;
  type = -1;
  while (FileGets (buff, sizeof (buff), fin) != NULL)
  {
    if (buff[0] == ';')
      continue;
    if (type == -1)
    {
      sscanf (buff, "%d", &type);
      if (type != 1)
      {
        FileClose (fin);
        return 0;
      }
      if (FileGets (buff, sizeof (buff), fin) == NULL)
      {
        FileClose (fin);
        return 0;
      }
      sscanf (buff, "%d", &type);
      fltp->window = type;
      continue;
    }
    sscanf (buff, "%lf8.3", &score);
    fltp->scr[val] = score;
    val++;
  }
  FileClose (fin);
  fltp->maxscr = 0.0;
  for (i = 0; i < val; i++)
  {
    temp = fltp->scr[i];
    if (temp < 0.0)
      temp *= -1.0;
    if (temp > fltp->maxscr)
      fltp->maxscr = temp;
  }
  fltp->maxscr *= (fltp->window);
  return val;
}

extern FilterDatPtr FilterDatNew (Int4 filtertype, Int4 window)
{
  FilterDatPtr fltp;

  if ((fltp = (FilterDatPtr) MemNew (sizeof (FilterDat))) != NULL)
  {
    fltp->window = window;
    fltp->filtertype = filtertype;
    if (fltp->filtertype >= AA_FILTER_COMP)
      fltp->filtersize = 24;
    else
      fltp->filtersize = 15;
    fltp->filterdatafile = NULL;
    fltp->maxscr = 0.0;

    if (fltp->filtertype >= AA_FILTER_COMP)
/* no data available for selenocysteine U -- 24 characters */
      fltp->res = StringSave ("ABCDEFGHIKLMNPQRSTVWXYZ*");
    else
/* standard -- 15 characters */
      fltp->res = StringSave ("ABCDGHKMNRSTUWY");

    fltp->scr =
     (FloatHiPtr) MemNew ((size_t)(sizeof(FloatHi)*fltp->filtersize));
    if (fltp->scr != NULL)
    {
      MemSet ((Pointer) fltp->scr, 0,
              (size_t) (sizeof (FloatHi) * fltp->filtersize));
    }
    else
    {
      fltp = (FilterDatPtr) MemFree (fltp);
    }
  }
  return fltp;
}

extern FilterDatPtr FilterDatFree (FilterDatPtr fltp)
{
  if (fltp != NULL)
  {
    MemFree (fltp->filterdatafile);
    MemFree (fltp->res);
    MemFree (fltp->scr);
    fltp = (FilterDatPtr) MemFree (fltp);
  }
  return fltp;
}

extern FloatHiPtr urkFilterSeq (CharPtr seq, Int4 start, Int4 end,
                                FilterDatPtr fltp)
{
  FloatHiPtr   ringptr;
  FloatHiPtr   fhead, fptr;
  Int4         i, n, iring, length;
  FloatHi      run_score;
  Boolean      flagMatch;

  length = end - start + 1;
  if (length < fltp->window)
    return NULL;

  if ((ringptr = (FloatHiPtr)
      MemNew ((size_t) (sizeof (FloatHi) * fltp->window))) == NULL)
    return NULL;

  if ((fhead = fptr = (FloatHiPtr)
      MemNew ((size_t) (sizeof (FloatHi) * length))) == NULL)
  {
    MemFree (ringptr);
    return NULL;
  }

  MemSet ((Pointer) fptr, 0, (size_t) (sizeof (FloatHi) * length));

  iring = 0;
  run_score = 0.0;
  for (i = 0; i < (fltp->window)-1; i++)
  {
    flagMatch = FALSE;
    n = 0;
    while (fltp->res[n] != '\0')
    {
      if (seq[i] == fltp->res[n])
      {
        flagMatch = TRUE;
        break;
      }
      n++;
    }
    if (flagMatch)
    {
      run_score += fltp->scr[n];
      ringptr[iring] = fltp->scr[n];
    }
    else
    {
      ringptr[iring] = 0.0;
    }
    iring++;
  }
  ringptr[iring] = 0.0; /* dummy last value in setup */

  fptr += (fltp->window/2);
  for (i = fltp->window; i < length; i++)
  {
    run_score = run_score - ringptr[iring];
    flagMatch = FALSE;
    n = 0;
    while (fltp->res[n] != '\0')
    {
      if (seq[i] == fltp->res[n])
      {
        flagMatch = TRUE;
        break;
      }
      n++;
    }
    if (flagMatch)
    {
      run_score += fltp->scr[n];
      ringptr[iring] = fltp->scr[n];
    }
    else
    {
      ringptr[iring] = 0.0;
    }
    iring++;
    if (iring == fltp->window)
      iring = 0;
/* separate
    *fptr = run_score/fltp->window;
*/
    *fptr = run_score;
    fptr++;
  }
  MemFree (ringptr);
  return fhead;
}

extern FloatHiPtr FilterSeqPort (SeqPortPtr spp,
                                 Int4 start, Int4 end,
                                 FilterDatPtr fltp)
{
  CharPtr    seq, seqhead;
  Int4       i, loop, this_start, this_end;
  FloatHiPtr fltscr, fltmp, flthead = NULL;

  if (spp == NULL || fltp == NULL)
    return NULL;

  if (end-start+1 > MAXSEQCHUNK)
  {
    this_start = start;
    this_end = this_start + MAXSEQCHUNK;
  }
  else
  {
    this_start = start;
    this_end = end;
  }

  loop = 0;
  while (this_end <= end)
  {
    seq = seqhead = (CharPtr) MemNew ((size_t) (sizeof (Char) *
                                      (this_end-this_start+1)));
    if (seq == NULL)
      return NULL;

    SeqPortSeek (spp, this_start, SEEK_SET);
    for (i = this_start; i <= this_end; i++)
      *seq++ = (Char) SeqPortGetResidue (spp);
    *seq = '\0';
    fltscr = urkFilterSeq (seqhead, this_start, this_end, fltp);
    MemFree (seqhead);
    if (this_start == start && this_end == end)
    {
      flthead = fltscr;
      break;
    }
    if (this_end == end)
    {
      flthead = Realloc (flthead,
                (size_t) ((loop * MAXSEQCHUNK) +
                 (sizeof (FloatHi) * (this_end-this_start+1))));
      i = fltp->window/2;
      if (!(fltp->window/2))
        i++;
      fltmp = flthead + (loop * (MAXSEQCHUNK-i));
      fltscr += ((fltp->window)/2);
      for (i = this_start; i <= this_end; i++)
        *fltmp++ = *fltscr++;
      break;
    }
    if (this_start == start)
    {
      flthead = fltscr;
    }
    else
    {
      flthead = Realloc (flthead, (size_t) ((loop+1)*(MAXSEQCHUNK)));
      i = fltp->window/2;
      if (!(fltp->window/2))
        i++;
      fltmp = flthead + (loop * (MAXSEQCHUNK-i));
      for (i = this_start; i <= this_end; i++)
        *fltmp++ = *fltscr++;
    }
    this_start = this_end+1-fltp->window;
    this_end = this_start + MAXSEQCHUNK;
    if (this_end > end)
      this_end = end;
    loop++;
  }
  return flthead;
}

extern FloatHiPtr FilterBioseq (BioseqPtr bsp,
                                Int4 start, Int4 end,
                                FilterDatPtr fltp)
{
  SeqPortPtr spp;
  FloatHiPtr fltscr;

  if (bsp == NULL || fltp == NULL)
    return NULL;

  if (fltp->filtertype >= AA_FILTER_COMP && !ISA_aa (bsp->mol))
    return NULL;

  if (fltp->filtertype < AA_FILTER_COMP && !ISA_na (bsp->mol))
    return NULL;

  if (ISA_aa (bsp->mol))
    spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_iupacaa);
  else
    spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_iupacna);
  fltscr = FilterSeqPort (spp, start, end, fltp);
  SeqPortFree (spp);
  return fltscr;
}

extern FloatHiPtr FilterSeqLoc (SeqLocPtr slp,
                                FilterDatPtr fltp)
{
  BioseqPtr  bsp;
  Int4       start, end;
  FloatHiPtr fltscr;

  if (slp == NULL || fltp == NULL)
    return NULL;

  if (slp->choice != SEQLOC_INT)
    return NULL;

  if ((bsp = BioseqLockById (SeqLocId (slp))) == NULL)
    return NULL;

  start = SeqLocStart (slp);
  end = SeqLocStop (slp);
  fltscr = FilterBioseq (bsp, start, end, fltp);
  BioseqUnlock (bsp);
  return fltscr;
}

extern SeqLocPtr FilterFilter (FloatHiPtr score, FloatHi maxscore,
                               Int4 window, FloatHi percentcut,
                               Int4 length, SeqIdPtr sip,
                               Boolean flagMeld, Boolean flagHighPass)
{
  Int4       i;
  Int4       start, stop, left, right;
  SeqLocPtr  slp, slpt, slph = NULL;
  SeqIntPtr  sint, sin1, sin2;
  FloatHiPtr scr;

  if (score == NULL)
    return NULL;

  scr = score;
  for (i = 0; i < length; i++)
  {
    *scr = *scr / maxscore * 100;
    scr++;
  }

  if (flagHighPass)
  {
    for (i = 0; i < length; i++)
    {
      if (*score < percentcut)
        break;
      score++;
    }
    if (i == length)
      return NULL;
/* just continue */
    while (i < length)
    {
      if (*score < percentcut)
      {
        start = i;
        while (*score < percentcut && i < length)
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
    for (i = 0; i < length; i++)
    {
      if (*score >= percentcut)
        break;
      score++;
    }
    if (i == length)
      return NULL;
/* just continue */
    while (i < length)
    {
      if (*score >= percentcut)
      {
        start = i;
        while (*score >= percentcut && i < length)
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
  if (window != 0)
  {
    left = right = window / 2;
    if (window%2 == 0)
      left--;
    slp = slph;
    while (slp != NULL)
    {
      sint = slp->data.ptrvalue;
      start = sint->from;
      stop = sint->to;
      start -= left;
      if (start < 0)
        start = 0;
      stop += right;
      if (stop > length-1)
        stop = length - 1;
      sint->from = start;
      sint->to = stop;
      slp = slp->next;
    }
  }
  if (flagMeld)
  {
    slp = slph;
    while (slp != NULL)
    {
      sin1 = slp->data.ptrvalue;
      while (slp->next != NULL)
      {
        slpt = slp->next;
        sin2 = slpt->data.ptrvalue;
        if (sin2->from > sin1->to)
          break;
        sin1->to = sin2->to;
        slp->next = slpt->next;
        slpt->next = NULL;
        SeqLocFree (slpt);
      }
      slp = slp->next;
    }
  }
  return slph;
}
