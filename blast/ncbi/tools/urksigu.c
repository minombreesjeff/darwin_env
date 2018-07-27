static char const rcsid[] = "$Id: urksigu.c,v 6.5 2003/05/30 17:25:38 coulouri Exp $";

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
* File Name: urksigu.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.5 $
*
* File Description: sigme utilities
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urksigu.c,v $
* Revision 6.5  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.4  1998/11/24 15:40:53  kuzio
* refine boundary condition for multiple potential leader pepides
*
* Revision 6.3  1998/11/16 14:29:54  kuzio
* flagBoundaryCondition
*
* Revision 6.2  1998/09/16 18:03:37  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#include <urksigu.h>

extern SeqLocPtr CheckOverlap (SeqLocPtr slp, Int4 ctermsig)
{
  SeqLocPtr slphead = NULL, slptmp;
  Int4      start;

  if (ctermsig == 0)
    return slp;

  while (slp != NULL)
  {
    start = SeqLocStop (slp);
    if (start < ctermsig)
    {
      slptmp = slp->next;
      slp->next = NULL;
      SeqLocFree (slp);
      slp = slptmp;
      continue;
    }
    if (slphead == NULL)
      slphead = slp;
    slp = slp->next;
  }
  return slphead;
}

extern Int4 EndOfSig (SeqLocPtr slp)
{
  Int4 stop, ctermsig = 0;

  while (slp != NULL)
  {
    stop = SeqLocStop (slp);
    if (stop > ctermsig)
      ctermsig = stop;
    slp = slp->next;
  }
  return ctermsig;
}

extern SeqLocPtr FilterSigSeq (BioseqPtr bsp,
                               ComProfPtr pppl, ComProfPtr pppc,
                               FloatHi leadcutoff, FloatHi cutcutoff,
                               Int4 range, SeqIdPtr sip,
                               Boolean flagBoundaryCondition,
                               Boolean flagReportIfAllFuzzyOnly)
{
  Int4        lstart, lstop, cstart, cstop;
  FloatHi     score;
  SeqAlignPtr sap1h, sap1, sap2h, sap2;
  StdSegPtr   ssp1, ssp2;
  SeqLocPtr   slp1, slp2;
  SeqLocPtr   slp, slpt, slph = NULL;

  if (bsp == NULL || pppl == NULL || pppc == NULL)
    return NULL;

  sap1h = sap1 = ProfileMatchBioseq (bsp, pppl, NULL, leadcutoff,
                                     flagBoundaryCondition);
  sap2h = sap2 = ProfileMatchBioseq (bsp, pppc, NULL, cutcutoff,
                                     flagBoundaryCondition);
  if (flagReportIfAllFuzzyOnly)
  {
    score = leadcutoff * 0.9;
    score = leadcutoff + (leadcutoff - score);
    while (sap1 != NULL)
    {
      if (sap1->score->value.realvalue > score)
      {
        while (sap1h != NULL)
        {
          sap1 = sap1h->next;
          SeqAlignFree (sap1h);
          sap1h = sap1;
        }
        break;
      }
      sap1 = sap1->next;
    }
    score = cutcutoff * 0.9;
    score = cutcutoff + (cutcutoff - score);
    while (sap2 != NULL)
    {
      if (sap2->score->value.realvalue > score)
      {
        while (sap2h != NULL)
        {
          sap2 = sap2h->next;
          SeqAlignFree (sap2h);
          sap2h = sap2;
        }
        break;
      }
      sap2 = sap2->next;
    }
  }
  sap1 = sap1h;
  sap2 = sap2h;
  while (sap1 != NULL)
  {
    ssp1 = (StdSegPtr) sap1->segs;
    slp1 = ssp1->loc;
    lstart = SeqLocStart (slp1);
    lstop = SeqLocStop (slp1);
    if (range < 0 || lstop < range)
    {
      sap2 = sap2h;
      while (sap2 != NULL)
      {
        ssp2 = (StdSegPtr) sap2->segs;
        slp2 = ssp2->loc;
        cstart = SeqLocStart (slp2);
        cstop = SeqLocStop (slp2);
        if (cstart - lstop > 0 && cstart - lstop < 8)
        {
          slpt = slph;
          while (slpt != NULL)
          {
            if (cstop == SeqLocStop (slpt))
              break;
            slpt = slpt->next;
          }
          if (slpt == NULL)
          {
            slp = SeqLocIntNew (lstart, cstop, Seq_strand_unknown, sip);
            ValNodeLink (&slph, slp);
          }
        }
        sap2 = sap2->next;
      }
    }
    sap1 = sap1->next;
  }

  while (sap1h != NULL)
  {
    sap1 = sap1h->next;
    SeqAlignFree (sap1h);
    sap1h = sap1;
  }
  while (sap2h != NULL)
  {
    sap2 = sap2h->next;
    SeqAlignFree (sap2h);
    sap2h = sap2;
  }

  return slph;
}
