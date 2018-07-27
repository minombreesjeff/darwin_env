static char const rcsid[] = "$Id: urkbias.c,v 6.7 2003/05/30 17:25:38 coulouri Exp $";

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
* File Name: urkbias.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.7 $
*
* File Description: codon bias
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urkbias.c,v $
* Revision 6.7  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.6  1998/09/30 21:46:45  kuzio
* no need to reverse bias score
*
* Revision 6.5  1998/09/16 18:03:31  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <accentr.h>
#include <gather.h>
#include <urkcnsrt.h>
#include <urkbias.h>

extern Gather_CDSPtr GatherCDSNew (void)
{
  Gather_CDSPtr gcdsp;

  if ((gcdsp = (Gather_CDSPtr) MemNew (sizeof (Gather_CDS))) == NULL)
    return NULL;
  gcdsp->LOscore = -1.0;
  gcdsp->scorecut = 0.5;

  return gcdsp;
}

extern Gather_CDSPtr GatherCDSFree (Gather_CDSPtr gcdsp)
{
  SeqLocPtr slp, slpn;
  SeqIdPtr  id;

  if (gcdsp == NULL)
    return NULL;

  MemFree (gcdsp->tableGlobal);
  MemFree (gcdsp->tableRefine);

  slp = gcdsp->slpGlobal;
  while (slp != NULL)
  {
    slpn = slp->next;
    id = SeqLocId (slp);
    if (id != NULL)
      id->next = SeqIdSetFree (id->next);
    SeqLocFree (slp);
    slp = slpn;
  }
  slp = gcdsp->slpRefine;
  while (slp != NULL)
  {
    slpn = slp->next;
    id = SeqLocId (slp);
    if (id != NULL)
      id->next = SeqIdSetFree (id->next);
    SeqLocFree (slp);
    slp = slpn;
  }
  slp = gcdsp->slpAll;
  while (slp != NULL)
  {
    slpn = slp->next;
    id = SeqLocId (slp);
    if (id != NULL)
      id->next = SeqIdSetFree (id->next);
    SeqLocFree (slp);
    slp = slpn;
  }
  slp = gcdsp->slpHit;
  while (slp != NULL)
  {
    slpn = slp->next;
    id = SeqLocId (slp);
    if (id != NULL)
      id->next = SeqIdSetFree (id->next);
    SeqLocFree (slp);
    slp = slpn;
  }
  slp = gcdsp->slpFound;
  while (slp != NULL)
  {
    slpn = slp->next;
    id = SeqLocId (slp);
    if (id != NULL)
      id->next = SeqIdSetFree (id->next);
    SeqLocFree (slp);
    slp = slpn;
  }

  return (Gather_CDSPtr) MemFree (gcdsp);
}

extern SeqLocPtr SeqLocDup (SeqLocPtr slpold)
{
  SeqLocPtr slpnew, slpn, slp;
  SeqIdPtr  id;

  if (slpold == NULL)
    return NULL;

  slpnew = (SeqLocPtr) AsnIoMemCopy ((Pointer) slpold,
                                     (AsnReadFunc) SeqLocAsnRead,
                                     (AsnWriteFunc) SeqLocAsnWrite);
  slp = slpnew->next;
  while (slp != NULL)
  {
    slpn = slp->next;
    id = SeqLocId (slp);
    if (id != NULL)
      id->next = SeqIdSetFree (id->next);
    SeqLocFree (slp);
    slp = slpn;
  }
  slpnew->next = NULL;

  return slpnew;
}

extern SeqLocPtr SeqLocDupAll (SeqLocPtr slpold)
{
  SeqLocPtr slpnew;

  if (slpold == NULL)
    return NULL;

  slpnew = (SeqLocPtr) AsnIoMemCopy ((Pointer) slpold,
                                     (AsnReadFunc) SeqLocAsnRead,
                                     (AsnWriteFunc) SeqLocAsnWrite);
  return slpnew;
}

extern SeqLocPtr SeqLocLink (SeqLocPtr PNTR slph, SeqLocPtr slp)
{
  SeqLocPtr slpnext;

  if (slph == NULL || *slph == NULL)
  {
    *slph = slp;
    return *slph;
  }
  slpnext = *slph;
  while (slpnext->next != NULL)
    slpnext = slpnext->next;
  slpnext->next = slp;
  return *slph;
}

extern Boolean SeqLocMatch (SeqLocPtr slplist, SeqLocPtr slpnew)
{
  Int4 start1, stop1, start2, stop2;

  if (slpnew == NULL || slplist == NULL)
  {
    return FALSE;
  }

  while (slplist != NULL)
  {
    start1 = SeqLocStart (slpnew);
    stop1 = SeqLocStop (slpnew);
    start2 = SeqLocStart (slplist);
    stop2 = SeqLocStop (slplist);

    if (start1 == start2 && stop1 == stop2)
      return TRUE;
    slplist = slplist->next;
  }
  return FALSE;
}

extern void UniqueOrfs (SeqLocPtr PNTR pslpFound)
{
  SeqLocPtr slpFound, slpNew, slp;
  SeqIdPtr  id;

  slpFound = *pslpFound;
  slpNew = NULL;
  while (slpFound != NULL)
  {
    if (!SeqLocMatch (slpNew, slpFound))
      SeqLocLink (&slpNew, SeqLocDup (slpFound));
    slpFound = slpFound->next;
  }
  slpFound = *pslpFound;
  while (slpFound != NULL)
  {
    slp = slpFound->next;
    id = SeqLocId (slpFound);
    if (id != NULL)
      id->next = SeqIdSetFree (id->next);
    SeqLocFree (slpFound);
    slpFound = slp;
  }
  *pslpFound = slpNew;
  return;
}

static int LIBCALLBACK SeqLocCompProc (VoidPtr ptr1, VoidPtr ptr2)
{
  SeqLocPtr slp1, slp2;

  if (ptr1 != NULL && ptr2 != NULL)
  {
    slp1 = *((SeqLocPtr PNTR) ptr1);
    slp2 = *((SeqLocPtr PNTR) ptr2);
    if (slp1 != NULL && slp2 != NULL)
    {
      if (SeqLocStart (slp1) > SeqLocStart (slp2))
      {
        return 1;
      }
      else if (SeqLocStart (slp1) < SeqLocStart (slp2))
      {
        return -1;
      }
    }
  }
  return 0;
}

extern void SortByLocOrfs (SeqLocPtr PNTR pslpFound)
{
  SeqLocPtr slpFound, PNTR slpt;
  Int4      num, i;

  num = 0;
  slpFound = *pslpFound;
  while (slpFound != NULL)
  {
    num++;
    slpFound = slpFound->next;
  }

  slpt = MemNew ((size_t)(num+1) * sizeof (SeqLocPtr));
  slpFound = *pslpFound;
  i = 0;
  while (slpFound != NULL)
  {
    slpt[i] = slpFound;
    slpFound = slpFound->next;
    i++;
  }

  HeapSort (slpt, num, sizeof (SeqLocPtr), SeqLocCompProc);

  for (i = 0; i < num; i++)
  {
    slpFound = slpt[i];
    slpFound->next = slpt[i+1];
  }

  *pslpFound = slpt[0];
  MemFree (slpt);

  return;
}

extern void RemoveInternalOrfs (SeqLocPtr PNTR slpFound)
{
  SeqLocPtr slp1, slp2, slp = NULL;
  SeqIdPtr  id;
  Int4      start1, stop1, start2, stop2;
  Boolean   flagInternal;

  slp1 = *slpFound;
  while (slp1 != NULL)
  {
    start1 = SeqLocStart (slp1);
    stop1 = SeqLocStop (slp1);
    flagInternal = FALSE;
    slp2 = *slpFound;
    while (slp2 != NULL)
    {
      start2 = SeqLocStart (slp2);
      stop2 = SeqLocStop (slp2);
      if ((start1 > start2 && start1 < stop2) &&
          (stop1 > start2 && stop1 < stop2))
      {
        flagInternal = TRUE;
        break;
      }
      slp2 = slp2->next;
    }
    if (!flagInternal)
      SeqLocLink (&slp, SeqLocDup (slp1));
    slp1 = slp1->next;
  }
  slp1 = *slpFound;
  while (slp1 != NULL)
  {
    slp2 = slp1->next;
    id = SeqLocId (slp1);
    if (id != NULL)
      id->next = SeqIdSetFree (id->next);
    SeqLocFree (slp1);
    slp1 = slp2;
  }
  *slpFound = slp;
  return;
}

static Boolean ScanCodonUsage (GatherContextPtr gcp)
{
  Gather_CDSPtr gcdsp;
  BioseqPtr     bsp;
  SeqLocPtr     slp, slpt;
  Int4Ptr       cutp;
  FloatHi       score;

  if (gcp == NULL)
    return FALSE;
  if ((gcdsp = (Gather_CDSPtr) gcp->userdata) == NULL)
    return FALSE;
  if (gcp->thistype != OBJ_BIOSEQ)
    return TRUE;
  if ((bsp = (BioseqPtr) (gcp->thisitem)) == NULL)
    return TRUE;

  if (gcdsp->findcount != 0)
    return TRUE;

  slp = gcdsp->slpAll;
  while (slp != NULL)
  {
    cutp = NewCodonTable ();
    slpt = SeqLocDup (slp);
    AddSeqLocToCodonTable (cutp, bsp, slpt, TRUE);
    score = Confide (cutp, gcdsp->tableRefine);
    FreeCodonTable (cutp);
    if (score < gcdsp->scorecut)
    {
      SeqLocLink (&(gcdsp->slpHit), SeqLocDup (slp));
      gcdsp->findcount++;
    }
    SeqLocFree (slpt);
    slp = slp->next;
  }
  return TRUE;
}

static Boolean RefineCodonUsage (GatherContextPtr gcp)
{
  Gather_CDSPtr gcdsp;
  BioseqPtr     bsp;
  SeqLocPtr     slp, slpt;
  Int4Ptr       cutp;
  FloatHi       score;

  if (gcp == NULL)
    return FALSE;
  if ((gcdsp = (Gather_CDSPtr) gcp->userdata) == NULL)
    return FALSE;
  if (gcp->thistype != OBJ_BIOSEQ)
    return TRUE;
  if ((bsp = (BioseqPtr) (gcp->thisitem)) == NULL)
    return TRUE;

  if (gcdsp->tableRefine != NULL)
    return TRUE;

  slp = gcdsp->slpAll;
  while (slp != NULL)
  {
    cutp = NewCodonTable ();
    slpt = SeqLocDup (slp);
    AddSeqLocToCodonTable (cutp, bsp, slpt, TRUE);
    score = Confide (cutp, gcdsp->tableGlobal);
    FreeCodonTable (cutp);
    if (score < gcdsp->scorecut)
    {
      gcdsp->refinecount++;
      if (gcdsp->tableRefine == NULL)
        gcdsp->tableRefine = NewCodonTable ();
      AddSeqLocToCodonTable (gcdsp->tableRefine, bsp, slpt, TRUE);
      SeqLocLink (&(gcdsp->slpRefine), SeqLocDup (slp));
    }
    SeqLocFree (slpt);
    slp = slp->next;
  }
  return TRUE;
}

static Boolean StandardMean (GatherContextPtr gcp)
{
  Gather_CDSPtr gcdsp;
  BioseqPtr     bsp;
  SeqLocPtr     slp, slpt;
  Int4Ptr       cutp;
  FloatHi       score;

  if (gcp == NULL)
    return FALSE;
  if ((gcdsp = (Gather_CDSPtr) gcp->userdata) == NULL)
    return FALSE;
  if (gcp->thistype != OBJ_BIOSEQ)
    return TRUE;
  if ((bsp = (BioseqPtr) (gcp->thisitem)) == NULL)
    return TRUE;

  if (gcdsp->stdcount != 0)
    return TRUE;

  slp = gcdsp->slpAll;
  while (slp != NULL)
  {
    cutp = NewCodonTable ();
    slpt = SeqLocDup (slp);
    AddSeqLocToCodonTable (cutp, bsp, slpt, TRUE);
    score = Confide (cutp, gcdsp->tableGlobal);
    FreeCodonTable (cutp);
    gcdsp->mean += score;
    gcdsp->stdcount++;
    if (gcdsp->LOscore == -1)
      gcdsp->LOscore = score;
    if (score < gcdsp->LOscore)
      gcdsp->LOscore = score;
    if (score > gcdsp->HIscore)
      gcdsp->HIscore = score;
    SeqLocFree (slpt);
    slp = slp->next;
  }
  return TRUE;
}

static Boolean StandardDeviation (GatherContextPtr gcp)
{
  Gather_CDSPtr gcdsp;
  BioseqPtr     bsp;
  SeqLocPtr     slp, slpt;
  Int4Ptr       cutp;
  FloatHi       score;

  if (gcp == NULL)
    return FALSE;
  if ((gcdsp = (Gather_CDSPtr) gcp->userdata) == NULL)
    return FALSE;
  if (gcp->thistype != OBJ_BIOSEQ)
    return TRUE;
  if ((bsp = (BioseqPtr) (gcp->thisitem)) == NULL)
    return TRUE;

  if (gcdsp->stdev != 0.0)
    return TRUE;

  slp = gcdsp->slpAll;
  while (slp != NULL)
  {
    cutp = NewCodonTable ();
    slpt = SeqLocDup (slp);
    AddSeqLocToCodonTable (cutp, bsp, slpt, TRUE);
    score = Confide (cutp, gcdsp->tableGlobal);
    FreeCodonTable (cutp);
    score -= gcdsp->mean;
    score *= score;
    gcdsp->stdev += score;
    SeqLocFree (slpt);
    slp = slp->next;
  }
  return TRUE;
}

static Boolean MakeGlobalTable (GatherContextPtr gcp)
{
  Gather_CDSPtr gcdsp;
  BioseqPtr     bsp;
  SeqLocPtr     slp, slpt;

  if (gcp == NULL)
    return FALSE;
  if ((gcdsp = (Gather_CDSPtr) gcp->userdata) == NULL)
    return FALSE;
  if (gcp->thistype != OBJ_BIOSEQ)
    return TRUE;
  if ((bsp = (BioseqPtr) (gcp->thisitem)) == NULL)
    return TRUE;

  if (gcdsp->tableGlobal != NULL)
    return TRUE;

  slp = gcdsp->slpGlobal;
  while (slp != NULL)
  {
    gcdsp->globalcount++;
    if (gcdsp->tableGlobal == NULL)
      gcdsp->tableGlobal = NewCodonTable ();
    slpt = SeqLocDup (slp);
    AddSeqLocToCodonTable (gcdsp->tableGlobal, bsp, slpt, TRUE);
    SeqLocFree (slpt);
    slp = slp->next;
  }
  return TRUE;
}

static Int4 CullGlobalOrfs (SeqLocPtr PNTR pslpGlobal,
                            SeqLocPtr PNTR pslpRefine)
{
  Int4      globalcount;
  SeqLocPtr slpGlobal, slpRefine, slpNew, slp;

  slpGlobal = *pslpGlobal;
  slpRefine = *pslpRefine;
  slpNew = NULL;
  globalcount = 0;
  while (slpGlobal != NULL)
  {
    if (!SeqLocMatch (slpRefine, slpGlobal))
    {
      globalcount++;
      SeqLocLink (&slpNew, SeqLocDup (slpGlobal));
    }
    slpGlobal = slpGlobal->next;
  }
  slpGlobal = *pslpGlobal;
  while (slpGlobal != NULL)
  {
    slp = slpGlobal->next;
    SeqLocFree (slpGlobal);
    slpGlobal = slp;
  }
  *pslpGlobal = slpNew;
  return globalcount;
}

extern SeqLocPtr FindSimilarBiasOrfs (SeqEntryPtr sep, FloatHi probcut,
                                      Int4 clustmin, Int4 findmin,
                                      SeqLocPtr slpKnown,
                                      SeqLocPtr slpPotential)
{
  static GatherScope  gs;
  GatherScopePtr      gsp;
  Gather_CDSPtr       gcdsp;

  Int4      gcount;
  SeqLocPtr slp, slpn;

  if (probcut == 0.0)
    probcut = 0.5;
  if (clustmin == 0)
    clustmin = 2;
  if (findmin == 0)
    findmin = 4;

  gsp = &gs;
  gcdsp = GatherCDSNew ();

  MemSet ((Pointer) gsp, 0, sizeof (GatherScope));
  MemSet ((Pointer) gsp->ignore, (int) (TRUE),
          (size_t) (OBJ_MAX * sizeof (Boolean)));
  gsp->ignore[OBJ_BIOSEQ] = FALSE;

  slp = slpKnown;
  while (slp != NULL)
  {
    SeqLocLink (&gcdsp->slpGlobal, SeqLocDup (slp));
    slp = slp->next;
  }
  slp = slpPotential;
  while (slp != NULL)
  {
    SeqLocLink (&gcdsp->slpAll, SeqLocDup (slp));
    slp = slp->next;
  }

  GatherSeqEntry (sep, (Pointer) gcdsp, MakeGlobalTable, (Pointer) gsp);

  while (gcdsp->tableGlobal != NULL)
  {
    gcdsp->stdcount = 0;
    gcdsp->refinecount = 0;
    gcdsp->findcount = 0;
    gcdsp->stdev = 0.0;
    gcdsp->HIscore = 0.0;
    gcdsp->LOscore = -1.0;
    gcdsp->mean = 0.0;
    gcdsp->scorecut = 0.5;

    GatherSeqEntry (sep, (Pointer) gcdsp, StandardMean, (Pointer) gsp);
    if (gcdsp->stdcount > 0)
      gcdsp->mean /= gcdsp->stdcount;
    GatherSeqEntry (sep, (Pointer) gcdsp, StandardDeviation, (Pointer) gsp);

    if (gcdsp->stdcount > 1)
      gcdsp->stdev /= (gcdsp->stdcount - 1);
    else
      gcdsp->stdev = 0.0;
    gcdsp->stdev = (FloatHi) sqrt (gcdsp->stdev);
    gcdsp->scorecut = gcdsp->LOscore + (gcdsp->stdev * probcut);

    slp = gcdsp->slpRefine;
    while (slp != NULL)
    {
      slpn = slp->next;
      SeqLocFree (slp);
      slp = slpn;
    }
    gcdsp->slpRefine = slp;

    gcdsp->tableRefine = FreeCodonTable (gcdsp->tableRefine);
    GatherSeqEntry (sep, (Pointer) gcdsp, RefineCodonUsage, (Pointer) gsp);
    if (gcdsp->tableRefine != NULL)
    {
      if (gcdsp->refinecount >= clustmin)
      {
        gcdsp->scorecut *= 1.5; /* increase a bit to see any branch jumps */
        GatherSeqEntry (sep, (Pointer) gcdsp, ScanCodonUsage, (Pointer) gsp);
      }
      if (gcdsp->findcount < findmin)
      {
        slp = gcdsp->slpHit;
        while (slp != NULL)
        {
          slpn = slp->next;
          SeqLocFree (slp);
          slp = slpn;
        }
        gcdsp->slpHit = slp;
      }
      else
      {
        SeqLocLink (&gcdsp->slpFound, gcdsp->slpHit);
        gcdsp->slpHit = NULL;
      }
      gcount = CullGlobalOrfs (&gcdsp->slpGlobal, &gcdsp->slpRefine);
      gcdsp->tableGlobal = FreeCodonTable (gcdsp->tableGlobal);
      if (gcount != gcdsp->globalcount)
      {
        gcdsp->globalcount = 0;
        GatherSeqEntry (sep, (Pointer) gcdsp, MakeGlobalTable, (Pointer) gsp);
      }
    }
    else
    {
      slp = gcdsp->slpGlobal;
      while (slp != NULL)
      {
        slpn = slp->next;
        SeqLocFree (slp);
        slp = slpn;
      }
      gcdsp->slpGlobal = slp;
      gcdsp->tableGlobal = FreeCodonTable (gcdsp->tableGlobal);
    }
  }
  UniqueOrfs (&gcdsp->slpFound);
  slp = gcdsp->slpFound;
  gcdsp->slpFound = NULL;
  GatherCDSFree (gcdsp);
  return slp;
}

extern FloatHiPtr BiasScoreBioseq (BioseqPtr bsp, Int4Ptr tableGlobal,
                                   Int4 tripletwindow, Int4 xframe,
                                   Uint1 xstrand)
{
  FloatHiPtr score;
  Int4       iscore;

  SeqLocPtr  slp;
  SeqIntPtr  sint;
  Int4       start, stop, xstop, xwindow;
  Int4Ptr    cutp;

  if (bsp == NULL)
    return NULL;
  if (!ISA_na (bsp->mol))
    return NULL;
  if (bsp->length < tripletwindow)
    return NULL;

  slp = ValNodeNew (NULL);
  sint = SeqIntNew ();
  slp->choice = SEQLOC_INT;
  slp->data.ptrvalue = sint;

  xwindow = tripletwindow;
  xstop = (bsp->length + 3 - xframe - xwindow) / 3;
  score = (FloatHiPtr) MemNew ((size_t) (sizeof (FloatHi) * xstop));
  xwindow -= 3;
  xstop--;

  start = xframe;
  stop = start + xwindow - 1;
  sint->from = start;
  sint->to = stop;
  sint->strand = xstrand;
  cutp = CodonTableFromSeqLoc (bsp, slp);

  iscore = 0;

  xstop = bsp->length - 3;
  for (start = stop + 1; start <= xstop; start += 3)
  {
    sint->from = start;
    sint->to = start + 2;
    AddSeqLocToCodonTable (cutp, bsp, slp, TRUE);
    score[iscore++] = Confide (cutp, tableGlobal);
    sint->from -= xwindow;
    sint->to -= xwindow;
    AddSeqLocToCodonTable (cutp, bsp, slp, FALSE);
  }

  FreeCodonTable (cutp);
  slp->data.ptrvalue = (Pointer) SeqIntFree (sint);
  SeqLocFree (slp);
  return score;
}
