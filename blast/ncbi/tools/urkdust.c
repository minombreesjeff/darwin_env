static char const rcsid[] = "$Id: urkdust.c,v 6.10 2003/05/30 17:25:38 coulouri Exp $";

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
* File Name: urkdust.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.10 $
*
* File Description: urkdust
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urkdust.c,v $
* Revision 6.10  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.9  1999/03/12 17:11:24  kuzio
* cast
*
* Revision 6.8  1998/09/16 18:03:33  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#include <urkdust.h>

static char _this_module[] = "dust";
#undef  THIS_MODULE
#define THIS_MODULE _this_module
static char _this_file[] = __FILE__;
#undef  THIS_FILE
#define THIS_FILE _this_file

extern DustDataPtr DustDataNew (void)
{
  DustDataPtr ddp;

  if ((ddp = (DustDataPtr) MemNew (sizeof (DustData))) == NULL)
    return NULL;

  ddp->percent = 20.0;
  ddp->level = 20;
  ddp->windowsize = 64;
  ddp->minwin = 8;
  ddp->linker = 8;
  ddp->method = 0;
  ddp->flagGraph = FALSE;
  ddp->flagBrute = FALSE;

  return ddp;
}

extern DustDataPtr DustDataFree (DustDataPtr ddp)
{
  return (DustDataPtr) MemFree (ddp);
}

extern DustRegionPtr DustRegionNew (DustRegionPtr drp)
{
  DustRegionPtr drpnew;

  if ((drpnew = (DustRegionPtr) MemNew (sizeof (DustRegion))) == NULL)
    return NULL;

  drpnew->percent = -1.0;
  drpnew->from = 0;
  drpnew->to = 0;
  drpnew->next = NULL;
  if (drp != NULL)
  {
    while (drp->next != NULL)
      drp = drp->next;
    drp->next = drpnew;
  }
  return drpnew;
}

extern DustRegionPtr DustRegionFree (DustRegionPtr drp)
{
  DustRegionPtr drpold;
  while (drp != NULL)
  {
    drpold = drp->next;
    MemFree (drp);
    drp = drpold;
  }
  return drp;
}

static void DustMe_o (Uint1Ptr seq, Int4 start, Int4 stop,
                      DustCurValPtr dcvp)
{
  Int4    loop;
  Int4    newlevel, sum;
  Int4    counts[4*4*4];
  Int4Ptr countsptr;

  sum = 0;
  MemSet (counts, 0, sizeof (counts));

  for (loop = 0; loop < stop; loop++)
  {
    countsptr = &counts[*seq++];
    if (*countsptr > 0)
    {
      sum += *countsptr;
      newlevel = 10 * sum / loop;
      if (dcvp->curlevel < newlevel)
      {
        dcvp->curlevel = newlevel;
        dcvp->curstart = start;
        dcvp->curend = loop + 2;       /* triplets */
      }
    }
    (*countsptr)++;
  }
  return;
}

static Int4 DustIt_o (Uint1Ptr seq, Int4 seqstart, Int4 stop,
                      DustCurValPtr dcvp)
{
  Int4     i, start;

  dcvp->curlevel = 0;
  dcvp->curstart = 0;
  dcvp->curend = 0;

  for (i = 0; i < stop; i++)
  {
    start = seqstart + i;
    DustMe_o (seq+start, start, stop-i, dcvp);
  }
  dcvp->curend += dcvp->curstart;
  return dcvp->curend;
}

static void DustMe_h (Uint1Ptr seq, Int4 start, Int4 stop,
                      DustCurValPtr dcvp, Int4 minwin)
{
  Int4    loop;
  Int4    newlevel, sum;
  Int4    counts[4*4*4];
  Int4Ptr countsptr;

  sum = 0;
  MemSet (counts, 0, sizeof (counts));
  minwin -= 2;

  for (loop = 0; loop < stop; loop++)
  {
    countsptr = &counts[*seq++];
    if (*countsptr > 0)
    {
      sum += *countsptr;
      newlevel = 100 * sum / (((loop+1) * loop) / 2);
/*
 minimum player count -- ddp->minwin - 2
 corresponds to original (not length normalized) dust level 20
 9 identical nucleotide run; 7 identical tiled triplets   7 * 6 / 2 = 21
*/
      if (loop > minwin)
      {
        if ((dcvp->curlevel < newlevel) ||
            (dcvp->curlevel <= newlevel) && dcvp->curend == loop+1)
        {
          dcvp->curlevel = newlevel;
          dcvp->curstart = start;
          dcvp->curend = loop + 2;       /* triplets */
        }
      }
    }
    (*countsptr)++;
  }
  return;
}

static Int4 DustIt_h (Uint1Ptr seq, Int4 seqstart, Int4 stop,
                      DustCurValPtr dcvp, Int4 minwin)
{
  Int4     i, start;

  dcvp->curlevel = 0;
  dcvp->curstart = 0;
  dcvp->curend = 0;

  for (i = 0; i < stop; i++)
  {
    start = seqstart + i;
    DustMe_h (seq+start, start, stop-i, dcvp, minwin);
  }
  dcvp->curend += dcvp->curstart;
  return dcvp->curend;
}

static void DustMe_c (Uint1Ptr seq, Int4 start, Int4 stop,
                      DustCurValPtr dcvp)
{
  Int4    loop;
  Int4    counts[4*4*4];
  Int4Ptr countsptr;

  MemSet (counts, 0, sizeof (counts));
  for (loop = 0; loop < stop; loop++)
  {
    countsptr = &counts[*seq++];
    if (*countsptr > 2)
    {
      if (dcvp->curlevel < *countsptr)
      {
        dcvp->curlevel = *countsptr - 1;
        dcvp->curstart = start;
        dcvp->curend = loop + 2;       /* triplets */
      }
    }
    (*countsptr)++;
  }
  return;
}

static Int4 DustIt_c (Uint1Ptr seq, Int4 seqstart, Int4 stop,
                      DustCurValPtr dcvp)
{
  Int4     i, start;

  dcvp->curlevel = 0;
  dcvp->curstart = 0;
  dcvp->curend = 0;

  for (i = 0; i < stop; i++)
  {
    start = seqstart + i;
    DustMe_c (seq+start, start, stop-i, dcvp);
  }
  dcvp->curend += dcvp->curstart;
  return dcvp->curend;
}

static DustRegionPtr Dust (Uint1Ptr seq, Int4 start, Int4 stop,
                           DustDataPtr ddp, DustRegionPtr drp)
{
  Int4          dustwindow, dustmax;
  Int4          i, increment, len, length, windowhalf;
  DustRegionPtr drphead;
  DustCurValPtr dcvp;
  DustCurVal    dcv;

  if (ddp->percent < 0.0 || ddp->percent > 100.0)
    ddp->percent = 20.0;
  if (ddp->level < 5 || ddp->level > 100)
    ddp->level = 20;
  if (ddp->windowsize < 8 || ddp->windowsize > 256)
    ddp->windowsize = 64;
  if (ddp->minwin < 4 || ddp->minwin > 128)
    ddp->minwin = 8;
  if (ddp->linker < 1 || ddp->linker > 32)
    ddp->linker = 8;

  windowhalf = ddp->windowsize / 2;
  length = stop - start + 1;
  dcvp = &dcv;

  drphead = drp;
  if (ddp->flagBrute)
    increment = 1;
  else
    increment = windowhalf;
  for (i = 0; i < length; i += increment)
  {
    len = ((length > i+ddp->windowsize) ? ddp->windowsize : length-i);
    len -= 2;
    switch (ddp->method)
    {
     default:
     case 0:
     case 1:
      DustIt_o (seq, i, len, dcvp);
      break;
     case 2:
      DustIt_h (seq, i, len, dcvp, ddp->minwin);
      break;
     case 3:
      DustIt_c (seq, i, len, dcvp);
      dcvp->curlevel = dcvp->curlevel * 100 / ddp->windowsize;
      break;
    }
    dcvp->curstart += start;
    dcvp->curend += start;

    if (ddp->flagGraph)
    {
      if (dcvp->curlevel > 2)
      {
        drp = DustRegionNew (drp);
        if (drp == NULL)
        {
          ErrPostEx (SEV_FATAL, 3, 1, "memory allocation error");
          ErrShow ();
          return drphead;
        }
        drp->from = dcvp->curstart;
        drp->to = dcvp->curend;
/*
 normalizes original dust score relative to the potential
 max score for half the 64 nt window (32 nt; 30 triplets)
 half seems like a good compromise
*/
        if (ddp->method == 1)
        {
          dustwindow = 30;
          dustmax = dustwindow * (dustwindow - 1) / 2;
          dustwindow = (dcvp->curlevel * (dustwindow-1) / 10);
          drp->percent = (FloatHi) (100 * dustwindow / dustmax);
        }
        else
        {
          drp->percent = (FloatHi) dcvp->curlevel;
        }

        if (drphead == NULL)
          drphead = drp;
/* 3' edge effects -- kill virtually all 3' tiling anomalies */
        if (!ddp->flagBrute)
        {
          if (i + windowhalf > dcvp->curend - (ddp->minwin / 2))
          {
            if (dcvp->curend - (ddp->minwin / 2) > dcvp->curstart)
              i = dcvp->curend - start - (ddp->minwin / 2) - windowhalf;
            else
              i = dcvp->curend - start - windowhalf;
          }
        }
      }
    }
    else
    {
      if (dcvp->curlevel > ddp->level)
      {
        if (drp != NULL &&
            drp->to + ddp->linker >= dcvp->curstart &&
            drp->from <= dcvp->curstart)
        {
/* overlap windows nicely if needed */
          drp->to = dcvp->curend;
          if (dcvp->curlevel > drp->percent)
            drp->percent = dcvp->curlevel;
        }
        else
        {
/* new window or dusted regions do not overlap */
          drp = DustRegionNew (drp);
          if (drp == NULL)
          {
            ErrPostEx (SEV_FATAL, 3, 1, "memory allocation error");
            ErrShow ();
            return drphead;
          }
          drp->from = dcvp->curstart;
          drp->to = dcvp->curend;
/*
 normalizes original dust score relative to the potential
 max score for half the 64 nt window (32 nt; 30 triplets)
 half seems like a good comppromise
*/
          if (ddp->method == 1)
          {
            dustwindow = 30;
            dustmax = dustwindow * (dustwindow - 1) / 2;
            dustwindow = (dcvp->curlevel * (dustwindow-1) / 10);
            drp->percent = (FloatHi) (100 * dustwindow / dustmax);
          }
          else
          {
            drp->percent = (FloatHi) dcvp->curlevel;
          }
          if (drphead == NULL)
            drphead = drp;
        }
/* 3' edge effects -- kill virtually all 3' tiling anomalies */
        if (!ddp->flagBrute)
        {
          if (i + windowhalf > dcvp->curend - (ddp->minwin / 2))
          {
            if (dcvp->curend - (ddp->minwin / 2) > dcvp->curstart)
              i = dcvp->curend - start - (ddp->minwin / 2) - windowhalf;
            else
              i = dcvp->curend - start - windowhalf;
          }
        }
      }
    }
  }
  return drphead;
}

extern DustRegionPtr DustSeq (CharPtr seq, Int4 start, Int4 stop,
                              DustDataPtr ddp)
{
  Uint1    c;
  Uint1Ptr shead, s1, s2, s3;

  shead = (Uint1Ptr) MemNew ((size_t)(sizeof(Uint1)*(stop-start+1)));
  MemSet (shead, 0, (size_t) (sizeof (Uint1) * (stop-start+1)));
  s1 = shead;
  s2 = s1 + 1;
  s3 = s2 + 1;

/* set up 1 */
  if ((c = (Uint1) *seq) == 0)
    return NULL;
  switch (c)
  {
   default:
   case 'A':
   case 'a':
    c = 0;
    break;
   case 'C':
   case 'c':
    c = 1;
    break;
   case 'G':
   case 'g':
    c = 2;
    break;
   case 'T':
   case 't':
    c = 3;
    break;
  }
  *s1 |= c;

/* set up 2 */
  seq++;
  if ((c = (Uint1) *seq) == 0)
    return NULL;
  switch (c)
  {
   default:
   case 'A':
   case 'a':
    c = 0;
    break;
   case 'C':
   case 'c':
    c = 1;
    break;
   case 'G':
   case 'g':
    c = 2;
    break;
   case 'T':
   case 't':
    c = 3;
    break;
  }
  *s1 <<= 2;
  *s1 |= c;
  *s2 |= c;

/* triplet fill loop */
  seq++;
  while ((c = (Uint1) *seq) != 0)
  {
    switch (c)
    {
     default:
     case 'A':
     case 'a':
      c = 0;
      break;
     case 'C':
     case 'c':
      c = 1;
      break;
     case 'G':
     case 'g':
      c = 2;
      break;
     case 'T':
     case 't':
      c = 3;
      break;
    }
    *s1 <<= 2;
    *s2 <<= 2;
    *s1 |= c;
    *s2 |= c;
    *s3 |= c;
    s1++;
    s2++;
    s3++;
  }
  return Dust (shead, start, stop, ddp, NULL);
}

extern DustRegionPtr DustSeqPort (SeqPortPtr spp,
                                  Int4 start, Int4 stop,
                                  DustDataPtr ddp)
{
  Int4          i, posn, pos;
  Uint1         c;
  Uint1Ptr      shead, s1, s2, s3;
  Boolean       flagVS;
  Int4          this_start, this_stop;
  DustRegionPtr drphead = NULL, drp = NULL;
  Int4          invrescount = 0;

  if (spp == NULL || ddp == NULL)
    return NULL;

  if (stop-start+1 > MAXSEQCHUNK)
  {
    this_start = start;
    this_stop = this_start + MAXSEQCHUNK;
  }
  else
  {
    this_start = start;
    this_stop = stop;
  }

  while (this_stop <= stop)
  {
    shead = (Uint1Ptr) MemNew ((size_t)(sizeof (Uchar)*(stop-start+1)));
    if (shead == NULL)
      return NULL;
    MemSet (shead, 0, (size_t) (sizeof (Uint1) * (stop-start+1)));
    s1 = shead;
    s2 = s1 + 1;
    s3 = s2 + 1;

    posn = start-1;

    SeqPortSeek (spp, this_start, SEEK_SET);

/* set up 1 */
    if ((c = SeqPortGetResidue (spp)) == SEQPORT_EOF)
      return NULL;
    if (c == SEQPORT_EOS || c == SEQPORT_VIRT)
      return NULL;
    if (!IS_residue (c))
    {
      c = 0;        /* 255 it's 'A' */
      pos = SeqPortTell (spp);
      if (invrescount < 3)
      {
        ErrPostEx (SEV_INFO, 5, 1,
                   "Invalid residue converted to 'A': %ld", (long) pos);
        ErrShow ();
      }
      invrescount++;
    }
    *s1 |= c;

/* set up 2 */
    if ((c = SeqPortGetResidue (spp)) == SEQPORT_EOF)
      return NULL;
    if (c == SEQPORT_EOS || c == SEQPORT_VIRT)
      return NULL;
    if (!IS_residue (c))
    {
      c = 0;        /* 255 it's 'A' */
      pos = SeqPortTell (spp);
      if (invrescount < 3)
      {
        ErrPostEx (SEV_INFO, 5, 1,
                   "Invalid residue converted to 'A': %ld", (long) pos);
        ErrShow ();
      }
      invrescount++;
    }
    *s1 <<= 2;
    *s1 |= c;
    *s2 |= c;

/* triplet fill loop */
    flagVS = FALSE;
    for (i = this_start+2; i <= this_stop; i++)
    {
      if ((c = SeqPortGetResidue (spp)) != SEQPORT_EOF)
      {
        if (c == INVALID_RESIDUE)  /* 255 */
        {
          c = 0;                   /* ping! -- it's 'A' */
          pos = SeqPortTell (spp);
          if (invrescount < 3)
          {
            ErrPostEx (SEV_INFO, 5, 1,
                       "Invalid residue converted to 'A': %ld", (long) pos);
            ErrShow ();
          }
          invrescount++;
        }
        if (IS_residue (c))
        {
          *s1 <<= 2;
          *s2 <<= 2;
          *s1 |= c;
          *s2 |= c;
          *s3 |= c;
          s1++;
          s2++;
          s3++;
          posn++;
        }
        else
        {
          switch (c)
          {
/* this should be okay unless segements are being used weirdly */
           case SEQPORT_EOS:   /* 252 */
            break;
/* start again at virtual sequence bounderies */
           case SEQPORT_VIRT:  /* 251 */
            flagVS = TRUE;
            break;
/* and/or just ignore anything odd at this point */
           default:
            break;
          }
          if (flagVS)
            break;
        }
      }
    }
    if (this_stop != posn+2+this_start)
    {
      ErrPostEx (SEV_WARNING, 1, 10,
                 "Possible seqport read error: Expected: %ld Actual: %ld",
                 (long) this_stop, (long) posn);
      ErrShow ();
    }
    drp = Dust (shead, this_start, this_stop, ddp, drp);
    if (drphead == NULL)
    {
      drphead = drp;
    }
    MemFree (shead);
    if (this_stop == stop)
    {
      break;
    }
    else
    {
      this_start = posn+3;
      this_stop = this_start + MAXSEQCHUNK;
      if (this_stop > stop)
        this_stop = stop;
    }
  }
  if (invrescount > 0)
  {
    ErrPostEx (SEV_INFO, 3, 2,
               "Total invalid residues found: %ld", (long) invrescount);
    ErrShow ();
  }
  return drphead;
}

extern DustRegionPtr DustBioseq (BioseqPtr bsp,
                                 Int4 start, Int4 stop,
                                 DustDataPtr ddp)
{
  DustRegionPtr drp;
  SeqPortPtr    spp;

  if (bsp == NULL || ddp == NULL)
    return NULL;

  if (!ISA_na (bsp->mol))
  {
    ErrPostEx (SEV_WARNING, 1, 2, "Not nucleic acid");
    ErrShow ();
    return NULL;
  }

/* error msg stuff */
  ErrSetOptFlags (EO_MSG_CODES);

  spp = SeqPortNew (bsp, start, stop, 0, Seq_code_ncbi2na);
  if (spp == NULL)
  {
    ErrPostEx (SEV_ERROR, 1, 4, "SeqPort open failed");
    ErrShow ();
    return NULL;
  }
  drp = DustSeqPort (spp, start, stop, ddp);
  SeqPortFree (spp);
  return drp;
}

extern DustRegionPtr DustSeqLoc (SeqLocPtr slp,
                                 DustDataPtr ddp)
{
  BioseqPtr     bsp;
  Int4          start, end;
  DustRegionPtr drp;

  if (slp == NULL || ddp == NULL)
    return NULL;

  if (slp->choice != SEQLOC_INT)
  {
    ErrPostEx (SEV_ERROR, 2, 1,
               "Will only dust a single SeqLoc interval");
    ErrShow ();
    return NULL;
  }

  if ((bsp = BioseqLockById (SeqLocId (slp))) == NULL)
  {
    ErrPostEx (SEV_ERROR, 2, 5, "Bioseq lock failure");
    ErrShow ();
    return NULL;
  }

  start = SeqLocStart (slp);
  end = SeqLocStop (slp);
  drp = DustBioseq (bsp, start, end, ddp);
  BioseqUnlock (bsp);
  return drp;
}

extern SeqLocPtr DustFilter (DustRegionPtr drp, FloatHi percentcut,
                             SeqIdPtr sip)
{
  SeqLocPtr  slp, slph = NULL;

  while (drp != NULL)
  {
    if (drp->percent >= percentcut)
    {
      slp = SeqLocIntNew (drp->from, drp->to, Seq_strand_unknown, sip);
      ValNodeLink (&slph, slp);
    }
    drp = drp->next;
  }
  return slph;
}

extern FloatHiPtr DustForGraph (DustRegionPtr drp, Int4 length,
                                Int4 start, Int4 stop)
{
  DustRegionPtr drphead;
  FloatHiPtr    fhi, fhead;
  Int4          i;

  if (drp == NULL)
    return NULL;
  if (stop > length)
    return NULL;


  drphead = drp;
  if ((fhead = (FloatHiPtr) MemNew ((size_t)((sizeof(FloatHi))*length)))
             == NULL)
    return NULL;

  while (drp != NULL)
  {
    fhi = fhead;
    if (drp->from >= start && drp->from < length && drp->to < length)
    {
      fhi += drp->from;
      for (i = drp->from; i <= drp->to; i++)
      {
        *fhi = drp->percent;
        fhi++;
      }
    }
    drp = drp->next;
  }
  return fhead;
}

extern SeqLocPtr BioseqDustWrap (BioseqPtr bsp, Int4 start, Int4 stop,
                                 Int2 level, Int2 window, Int2 minwin,
                                 Int2 linker)
{
  SeqLocPtr     slp;
  DustDataPtr   ddp;
  DustRegionPtr drp;

  ddp = DustDataNew ();
  ddp->percent = (FloatHi) level;
  ddp->level = level;
  ddp->windowsize = window;
  ddp->minwin = minwin;
  ddp->linker = linker;

  drp = DustBioseq (bsp, start, stop, ddp);
  slp = DustFilter (drp, ddp->percent, bsp->id);
  DustRegionFree (drp);
  DustDataFree (ddp);

  return slp;
}
