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
* File Name: sigmev.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.6 $
*
* File Description: signal peptides and transmembrane regions
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: sigmev.c,v $
* Revision 6.6  1998/11/24 15:42:01  kuzio
* refine boundary condition for multiple potential leader pepides
*
* Revision 6.5  1998/11/16 14:35:14  kuzio
* flagBoundaryCondition
*
* Revision 6.4  1998/09/16 18:40:36  kuzio
* cvs logging
*
* ==========================================================================
*/

#include <ncbi.h>
#include <vibrant.h>
#include <accentr.h>
#include <gather.h>
#include <tofasta.h>
#include <urkutil.h>
#include <urkfltr.h>
#include <urkptpf.h>
#include <urksigu.h>
#include <seqscrl.h>
#include <gphdraw.h>

#define TOP_ERROR 1
static char _this_module[] = "sigmev";
#undef  THIS_MODULE
#define THIS_MODULE _this_module
static char _this_file[] = __FILE__;
#undef  THIS_FILE
#define THIS_FILE _this_file

typedef struct xtrins
{
  SeqGraphPtr    sgplead, sgpcut, sgptm;
  Int4           Xscale, Yscale;
  Int4           Xaxislen, Yaxislen;
  Int4           Xprelen, Xpostlen, Yprelen, Ypostlen;
  RecT           dr;
  SegmenT        picture;
} XIS, PNTR XISPtr;

typedef struct xtrobj
{
  Boolean        flagParamWindow;
  Int4           gi;
  CharPtr        filename;
  GatherScopePtr gsp;
  SeqEntryPtr    sep;
  BioseqPtr      bsp;
  ComProfPtr     leadprofile, cutprofile;
  FloatHi        leadcutoff, cutcutoff;
  Boolean        flagBoundaryCondition;
  Int4           sigrange;
  FloatHi        sigscale;
  CharPtr        leadmat;
  CharPtr        cutmat;
  FloatHi        tmcutoff;
  Int4           tmwindow;
  FilterDatPtr   fltp;
} XOS, PNTR XOSPtr;

/* button 1 */

static void CloseGIWindowProc (WindoW w)
{
  Remove (w);
  return;
}

static void SetGIProc (ButtoN b)
{
  TexT     pc;
  XOSPtr   xosp;
  Char     buf[16];

  if ((pc = (TexT) GetObjectExtra (b)) != NULL)
  {
    if ((xosp = (XOSPtr) GetObjectExtra (pc)) != NULL)
    {
      GetTitle (pc, buf, sizeof (buf));
      xosp->gi = atol (buf);
      xosp->filename = (CharPtr) MemFree (xosp->filename);
    }
  }
  CloseGIWindowProc (ParentWindow (b));
  return;
}

static void ReadGIProc (ButtoN b)
{
  WindoW   w;
  GrouP    h, g;
  ButtoN   bt;
  TexT     pc;
  CharPtr  winprmt = "Entrez GI";
  XOSPtr   xosp;

  if ((xosp = (XOSPtr) GetObjectExtra (b)) != NULL)
  {
    w = MovableModalWindow (-60, -33, -10, -10, winprmt, CloseGIWindowProc);
    h = HiddenGroup (w, 1, 0, NULL);
    StaticPrompt (h, winprmt, StringWidth (winprmt),
                  dialogTextHeight, NULL, 'c');
    g = HiddenGroup (h, 2, 0, NULL);
    pc = DialogText (g, "", 8, NULL);
    bt = PushButton (h, "  Okay  ", SetGIProc);
    SetObjectExtra (pc, xosp, NULL);
    SetObjectExtra (bt, pc, NULL);
    Show (w);
    Select (w);
  }
  return;
}

/* button 2 */

static void ReadFileProc (ButtoN b)
{
  XOSPtr  xosp;
  Char    filename[PATH_MAX];

  if ((xosp = (XOSPtr) GetObjectExtra (b)) != NULL)
  {
    if (!GetInputFileName (filename, PATH_MAX, "", "TEXT"))
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 101, "Could not read directory");
      ErrShow ();
    }
    else
    {
      MemFree (xosp->filename);
      xosp->filename = StringSave (filename);
      xosp->gi = 0;
    }
  }
  return;
}

/* button 3 */

static void CloseParamWindowProc (WindoW w)
{
  SeqScrollDataPtr ssdp;

  if ((ssdp = (SeqScrollDataPtr) GetObjectExtra (w)) != NULL)
    SeqScrollDataFree (ssdp);
  Remove (w);
  return;
}

static void ApplyParams (ButtoN b)
{
  WindoW  w;
  XOSPtr  xosp;

  SeqScrollDataPtr ssdp;

  w = ParentWindow (b);
  if ((ssdp = (SeqScrollDataPtr) GetObjectExtra (w)) != NULL)
  {
    if ((xosp = (XOSPtr) GetObjectExtra (b)) != NULL)
    {
      xosp->sigrange = (Int4) ssdp->curval;
      ssdp = ssdp->next;
      xosp->leadcutoff = (FloatHi) ssdp->curval;
      ssdp = ssdp->next;
      xosp->cutcutoff = (FloatHi) ssdp->curval;
      ssdp = ssdp->next;
      xosp->tmwindow = (Int4) ssdp->curval;
      ssdp = ssdp->next;
      xosp->tmcutoff = (FloatHi) ssdp->curval;

      xosp->leadcutoff /= 10.0;
      xosp->cutcutoff /= 10.0;
    }
  }
  return;
}

static void ClearWindowFlag (ButtoN b)
{
  XOSPtr  xosp;

  if ((xosp = (XOSPtr) GetObjectExtra (b)) != NULL)
    xosp->flagParamWindow = FALSE;
  return;
}

static void AcceptParamProc (ButtoN b)
{
  ApplyParams (b);
  CloseParamWindowProc (ParentWindow (b));
  ClearWindowFlag (b);
  return;
}

static void UpdateParamProc (ButtoN b)
{
  ApplyParams (b);
  return;
}

static void CancelParamProc (ButtoN b)
{
  CloseParamWindowProc (ParentWindow (b));
  ClearWindowFlag (b);
  return;
}

static void SigMeParamProc (ButtoN b)
{
  WindoW           w;
  GrouP            h, h1;
  GrouP            s1, s2, s3, s4, s5;
  GrouP            g1, g2, g3, g4, g5;
  ButtoN           b1, b2, b3;
  XOSPtr           xosp;

  SeqScrollDataPtr ssdp_ccp = NULL;

  if ((xosp = (XOSPtr) GetObjectExtra (b)) == NULL)
    return;
  if (xosp->flagParamWindow)
    return;

  w = FixedWindow (-50, -10, -10, -10,
                   "SigMe Parameters", CloseParamWindowProc);

  h = HiddenGroup (w, 1, 0, NULL);
  h1 = HiddenGroup (h, 5, 0, NULL);
  SetGroupSpacing (h1, 10, 10);
  s1 = HiddenGroup (h1, 1, 0, NULL);
  StaticPrompt (s1, "sp-range", StringWidth ("sp-range"),
                dialogTextHeight, NULL, 'c');
  s2 = HiddenGroup (h1, 1, 0, NULL);
  StaticPrompt (s2, "sig-cut", StringWidth ("sig-cut"),
                dialogTextHeight, NULL, 'c');
  s3 = HiddenGroup (h1, 1, 0, NULL);
  StaticPrompt (s3, "cut-cut", StringWidth ("cut-cut"),
                dialogTextHeight, NULL, 'c');
  s4 = HiddenGroup (h1, 1, 0, NULL);
  StaticPrompt (s4, "tm-win", StringWidth ("tm-win"),
                dialogTextHeight, NULL, 'c');
  s5 = HiddenGroup (h1, 1, 0, NULL);
  StaticPrompt (s5, "tm-cut", StringWidth ("tm-cut"),
                dialogTextHeight, NULL, 'c');

  h1 = HiddenGroup (h, 5, 0, NULL);
  SetGroupSpacing (h1, 10, 10);
  g1 = HiddenGroup (h1, 2, 0, NULL);
  AddScrollControl (&ssdp_ccp, g1, 40, 20, 80, 24);
  g2 = HiddenGroup (h1, 2, 0, NULL);
  AddScrollControl (&ssdp_ccp, g2, 33, 10, 50, 24);
  g3 = HiddenGroup (h1, 2, 0, NULL);
  AddScrollControl (&ssdp_ccp, g3, 21, 10, 50, 24);
  g4 = HiddenGroup (h1, 2, 0, NULL);
  AddScrollControl (&ssdp_ccp, g4, 19, 10, 40, 24);
  g5 = HiddenGroup (h1, 2, 0, NULL);
  AddScrollControl (&ssdp_ccp, g5, 50, 20, 80, 24);

  SetObjectExtra (w, ssdp_ccp, NULL);

  h1 = HiddenGroup (h, 3, 0, NULL);
  b1 = PushButton (h1, "Accept", AcceptParamProc);
  SetObjectExtra (b1, xosp, NULL);
  b2 = PushButton (h1, "Apply ", UpdateParamProc);
  SetObjectExtra (b2, xosp, NULL);
  b3 = PushButton (h1, "Cancel", CancelParamProc);
  SetObjectExtra (b3, xosp, NULL);

  Show (w);
  Select (w);
  xosp->flagParamWindow = TRUE;

  return;
}

/* button 4 */

static Boolean GetBioseq (GatherContextPtr gcp)
{
  XOSPtr         xosp;
  BioseqPtr      bsp;
  Int4           gi, entrezgi;

  if (gcp == NULL)
    return FALSE;
  if ((xosp = (XOSPtr) gcp->userdata) == NULL)
    return FALSE;

  if (xosp->bsp != NULL)
    return TRUE;
  if (gcp->thistype != OBJ_BIOSEQ)
    return TRUE;
  if ((bsp = (BioseqPtr) (gcp->thisitem)) == NULL)
    return TRUE;

  gi = xosp->gi;
  if (gi > 0)
  {
    entrezgi = GetGIForSeqId (bsp->id);
    if (gi == entrezgi)
      xosp->bsp = bsp;
    return TRUE;
  }
  else
  {
    xosp->bsp = bsp;
    return TRUE;
  }
}

static void CleanUpGraphWindow (GraphiC g, VoidPtr data)
{
  XISPtr  xisp;

  if ((xisp = (XISPtr) data) != NULL)
  {
    SeqGraphFree (xisp->sgplead);
    SeqGraphFree (xisp->sgpcut);
    SeqGraphFree (xisp->sgptm);
    DeletePicture (xisp->picture);
  }
  StdCleanupExtraProc (g, data);
}

static void CloseGraphWindowProc (WindoW w)
{
  Remove (w);
  return;
}

static void CloseGraphWindowButton (ButtoN b)
{
  CloseGraphWindowProc (ParentWindow (b));
  return;
}

static FloatHiPtr ScoreArrayFromAlign (SeqAlignPtr sap, Int4 length)
{
  FloatHi    score;
  FloatHiPtr scorearray;
  StdSegPtr  ssp;
  SeqLocPtr  slp;
  Int4       i, start, stop;

  if (sap == NULL)
    return NULL;
  scorearray = (FloatHiPtr) MemNew ((size_t) (sizeof (FloatHi) * (length+1)));
  if (scorearray == NULL)
    return NULL;

  while (sap != NULL)
  {
    if (sap->segtype == 3)
    {
      if ((ssp = sap->segs) != NULL)
      {
        if ((slp = ssp->loc) != NULL)
        {
          score = 0.0;
          if (sap->score != NULL)
          {
            if (sap->score->choice == 2)
              score = sap->score->value.realvalue;
          }
          start = SeqLocStart (slp);
          stop = SeqLocStop (slp);
          for (i = start; i <= stop; i++)
            scorearray[i] = score;
        }
      }
    }
    sap = sap->next;
  }
  return scorearray;
}

static void ScaleScore (FloatHiPtr score, Int4 length, FloatHi scale)
{
  Int4    i;

  if (score == NULL)
    return;
  for (i = 0; i < length; i++)
  {
    *score *= scale;
    score++;
  }
  return;
}

static FloatHi MinimumScore (FloatHiPtr score, Int4 length)
{
  Int4    i;
  FloatHi min;

  if (score == NULL)
    return 0.0;
  min = *score;
  for (i = 0; i < length; i++)
  {
    if (*score < min)
      min = *score;
    score++;
  }
  return min;
}

static FloatHi MaximumScore (FloatHiPtr score, Int4 length)
{
  Int4    i;
  FloatHi max;

  if (score == NULL)
    return 0.0;
  max = *score;
  for (i = 0; i < length; i++)
  {
    if (*score > max)
      max = *score;
    score++;
  }
  return max;
}

static void ShiftScore (FloatHiPtr score, Int4 length, Int4 shift)
{
  Int4    i, stop;

  if (score == NULL)
    return;
  if (shift <= 0)
    return;
  stop = length - (shift + shift);
  for (i = stop-1; i >= 0; i--)
    score[i+shift] = score[i];
  for (i = 0; i < shift; i++)
    score[i] = 0.0;
  return;
}

static void SigMeProc (ButtoN b)
{
  Boolean     flagHaveNet;

  SeqEntryPtr sep;
  Int4        gi;
  CharPtr     fastafile;
  FILE        *fiop;
  CharPtr     title;

  GatherScopePtr      gsp;
  XOSPtr              xosp;
  XISPtr              xisp;

  SeqAlignPtr    sap, sapn;
  FloatHi        minscorelead, maxscorelead;
  FloatHi        minscorecut, maxscorecut;
  FloatHi        minscoretm, maxscoretm;
  FloatHiPtr     scorelead, scorecut, scoretm;
  SeqGraphPtr    sgplead, sgpcut, sgptm;

  SeqLocPtr   slpsig, slptm, slpn;
  Int4        ctermsig;
  Char        infobuf[128];

  WindoW      w;
  VieweR      v;
  GrouP       g;
  SegmenT     seg;
  Int4        top;

  if ((xosp = (XOSPtr) GetObjectExtra (b)) == NULL)
    return;

  gsp = xosp->gsp;
  gi = xosp->gi;
  fastafile = xosp->filename;

  if (gi > 0)
  {
    if (!EntrezInit ("sigmev", FALSE, &flagHaveNet))
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 102,
                 "Entrez init failed");
      ErrShow ();
      return;
    }
  }

  if (gi > 0)
  {
    sep = EntrezSeqEntryGet (gi, SEQENTRY_READ_BIOSEQ);
  }
  else if (fastafile != NULL)
  {
    if ((fiop = FileOpen (fastafile, "r")) == NULL)
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 103,
                 "Failed to open FastA file");
      ErrShow ();
      return;
    }
    sep = FastaToSeqEntry (fiop, FALSE);
  }
  else
  {
    sep = NULL;
  }

  if (sep == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 104,
               "No seqentry");
    ErrShow ();
    return;
  }
  else
  {
    xosp->sep = sep;
    xosp->bsp = NULL;
    xosp->gi = gi;
    GatherSeqEntry (sep, (Pointer) xosp, GetBioseq,
                    (Pointer) gsp);
    if (xosp->bsp != NULL)
    {
      if (ISA_aa (xosp->bsp->mol))
      {
/* hydrophobic leader */
        sap = ProfileMatchBioseq (xosp->bsp, xosp->leadprofile,
                                  NULL, xosp->leadcutoff,
                                  xosp->flagBoundaryCondition);
        scorelead = ScoreArrayFromAlign (sap, xosp->bsp->length);
        while (sap != NULL)
        {
          sapn = sap->next;
          SeqAlignFree (sap);
          sap = sapn;
        }

        ScaleScore (scorelead, xosp->bsp->length, (FloatHi) xosp->sigscale);
        minscorelead = MinimumScore (scorelead, xosp->bsp->length);
        maxscorelead = MaximumScore (scorelead, xosp->bsp->length);

        if ((sgplead = SeqGraphNew ()) == NULL)
        {
          MemFree (scorelead);
          return;
        }

/* cut site */
        sap = ProfileMatchBioseq (xosp->bsp, xosp->cutprofile,
                                  NULL, xosp->cutcutoff,
                                  xosp->flagBoundaryCondition);
        scorecut = ScoreArrayFromAlign (sap, xosp->bsp->length);
        while (sap != NULL)
        {
          sapn = sap->next;
          SeqAlignFree (sap);
          sap = sapn;
        }

        ScaleScore (scorecut, xosp->bsp->length, (FloatHi) xosp->sigscale);
        minscorecut = MinimumScore (scorecut, xosp->bsp->length);
        maxscorecut = MaximumScore (scorecut, xosp->bsp->length);

        if ((sgpcut = SeqGraphNew ()) == NULL)
        {
          MemFree (scorelead);
          MemFree (scorecut);
          return;
        }

/* tm filter */
        scoretm = FilterBioseq (xosp->bsp, 0, xosp->bsp->length-1,
                                    xosp->fltp);
        minscoretm = MinimumScore (scoretm, xosp->bsp->length);
        maxscoretm = MaximumScore (scoretm, xosp->bsp->length);
        ShiftScore (scoretm, xosp->bsp->length, xosp->tmwindow/2);

        if ((sgptm = SeqGraphNew ()) == NULL)
        {
          MemFree (scorelead);
          MemFree (scorecut);
          MemFree (scoretm);
          return;
        }

/* determine if sigpep exists */
        slpsig = FilterSigSeq (xosp->bsp, xosp->leadprofile, xosp->cutprofile,
                               xosp->leadcutoff, xosp->cutcutoff,
                               xosp->sigrange, xosp->bsp->id,
                               xosp->flagBoundaryCondition, FALSE);
        ctermsig = EndOfSig (slpsig);

/* determine if tm region exists with no overlap with sigpep */
        slptm = FilterFilter (scoretm, xosp->fltp->maxscr,
                              xosp->tmwindow, xosp->tmcutoff,
                              xosp->bsp->length, xosp->bsp->id,
                              TRUE, FALSE);
        slptm = CheckOverlap (slptm, ctermsig);

        xisp = (XISPtr) MemNew (sizeof (XIS));
        xisp->Xscale = 1;
        xisp->Yscale = 1;
        xisp->Xaxislen = 250;
        xisp->Xprelen = 0;
        xisp->Yprelen = 50;
        xisp->Xpostlen = 50;
        xisp->Ypostlen = 50;
        xisp->sgplead = sgplead;
        xisp->sgpcut = sgpcut;
        xisp->sgptm = sgptm;

        sgplead->loc = SeqLocIntNew (0, xosp->bsp->length-1, xosp->bsp->strand,
                                     xosp->bsp->id);
        title = FastaTitle (xosp->bsp, "SigMe leader: >", NULL);
        sgplead->title = title;
        sgplead->flags[2] = 1;
        sgplead->numval = xosp->bsp->length;

        sgpcut->loc = SeqLocIntNew (0, xosp->bsp->length-1, xosp->bsp->strand,
                                    xosp->bsp->id);
        title = FastaTitle (xosp->bsp, "SigMe leader cut: >", NULL);
        sgpcut->title = title;
        sgpcut->flags[2] = 1;
        sgpcut->numval = xosp->bsp->length;

        sgptm->loc = SeqLocIntNew (0, xosp->bsp->length-1, xosp->bsp->strand,
                                   xosp->bsp->id);
        title = FastaTitle (xosp->bsp, "SigMe tm: >", NULL);
        sgptm->title = title;
        sgptm->flags[2] = 1;
        sgptm->numval = xosp->bsp->length;

        if ((Int4) (maxscorelead - minscorelead) < 100)
          xisp->Yaxislen = (Int4) (maxscorelead - minscorelead);
        else
          xisp->Yaxislen = 100;
        sgplead->max.realvalue = maxscorelead;
        sgplead->min.realvalue = minscorelead;
        sgplead->values = (Pointer) scorelead;
        top = 200;
        seg = DrawSeqGraphSegment (sgplead, xosp->bsp, xosp->bsp->length,
                                   top, 0, 1, xosp->bsp->length, NULL);

        if ((Int4) (maxscorecut - minscorecut) < 100)
          xisp->Yaxislen = (Int4) (maxscorecut - minscorecut);
        else
          xisp->Yaxislen = 100;
        sgpcut->max.realvalue = maxscorecut;
        sgpcut->min.realvalue = minscorecut;
        sgpcut->values = (Pointer) scorecut;
        top -= ((Int4)(maxscorelead-minscorelead)+80);
        seg = DrawSeqGraphSegment (sgpcut, xosp->bsp, xosp->bsp->length,
                                   top, 0, 1, xosp->bsp->length, seg);

        if ((Int4) (maxscoretm - minscoretm) < 100)
          xisp->Yaxislen = (Int4) (maxscoretm - minscoretm);
        else
          xisp->Yaxislen = 100;
        sgptm->max.realvalue = maxscoretm;
        sgptm->min.realvalue = minscoretm;
        sgptm->values = (Pointer) scoretm;
        top -= ((Int4)(maxscorecut-minscorecut)+80);
        seg = DrawSeqGraphSegment (sgptm, xosp->bsp, xosp->bsp->length,
                                   top, 0, 1, xosp->bsp->length, seg);
        xisp->picture = seg;
        if (seg != NULL)
        {
          if (slpsig != NULL)
          {
            sprintf (infobuf, "signal peptide with cut site: Yes");
            while (slpsig != NULL)
            {
              slpn = slpsig->next;
              SeqLocFree (slpsig);
              slpsig = slpn;
            }
          }
          else
          {
            sprintf (infobuf, "signal peptide with cut site: No");
          }
          top -= ((Int4)(maxscoretm-minscoretm)+60);
          AddLabel (seg, 0, top, infobuf,
                    SMALL_TEXT, 5, MIDDLE_RIGHT, 0);
          if (slptm != NULL)
          {
            sprintf (infobuf, "transmembrane region: Yes");
            while (slptm != NULL)
            {
              slpn = slptm->next;
              SeqLocFree (slptm);
              slptm = slpn;
            }
          }
          else
          {
            sprintf (infobuf, "transmembrane region: No");
          }
          top -= 15;
          AddLabel (seg, 0, top, infobuf,
                    SMALL_TEXT, 5, MIDDLE_RIGHT, 0);
        }
/* upper left */
        w = FixedWindow (10, 10, -10, -10, "SigMe", CloseGraphWindowProc);
        SetObjectExtra (w, xisp, CleanUpGraphWindow);
        g = HiddenGroup (w, -1, 0, NULL);
        v = CreateViewer (g, 500, 400, TRUE, TRUE);
        AttachPicture (v, seg, INT4_MIN, INT4_MAX, UPPER_LEFT,
                       1, 1, NULL);
        PushButton (g, "Close", CloseGraphWindowButton);
        RealizeWindow (w);
        Show (w);
      }
      else
      {
        ErrPostEx (SEV_ERROR, TOP_ERROR, 101, "Not an amino acid Bioseq");
        ErrShow ();
      }
    }
    else
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 101, "No Bioseq in SeqEntry");
      ErrShow ();
    }
    xosp->sep = sep = SeqEntryFree (sep);
  }

  if (gi > 0)
    EntrezFini ();
  else
    FileClose (fiop);
  return;
}

/* button 5 */

static void CloseSigMeParentProc (WindoW w)
{
  XOSPtr  xosp;

  if ((xosp = (XOSPtr) GetObjectExtra (w)) != NULL)
  {
    MemFree (xosp->filename);
    xosp->gsp = NULL;          /* is static */
    xosp->sep = SeqEntryFree (xosp->sep);
    xosp->bsp = NULL;          /* should be in seqentry */
    xosp->leadmat = (CharPtr) MemFree (xosp->leadmat);
    xosp->cutmat = (CharPtr) MemFree (xosp->cutmat);
    xosp->leadprofile = ComProfFree (xosp->leadprofile);
    xosp->cutprofile = ComProfFree (xosp->cutprofile);
    xosp->fltp = FilterDatFree (xosp->fltp);
    MemFree (xosp);
  }
  Remove (w);
  QuitProgram ();
  return;
}

static void EndProg (ButtoN b)
{
  CloseSigMeParentProc (ParentWindow (b));
  return;
}

/* parent */

static XOSPtr SetUp (void)
{
  static GatherScope  gs;
  GatherScopePtr      gsp;
  XOSPtr              xosp;

  gsp = &gs;
  MemSet ((Pointer) gsp, 0, sizeof (GatherScope));
  MemSet ((Pointer) gsp->ignore, (int) (TRUE),
          (size_t) (OBJ_MAX * sizeof (Boolean)));
  gsp->ignore[OBJ_BIOSEQ] = FALSE;

  xosp = (XOSPtr) MemNew (sizeof (XOS));

  xosp->flagParamWindow = FALSE;
  xosp->gi = 0;
  xosp->filename = NULL;
  xosp->gsp = gsp;
  xosp->sep = NULL;
  xosp->bsp = NULL;

  xosp->leadmat = StringSave ("KSesigl.mat");
  xosp->cutmat = StringSave ("KSesigc.mat");
  xosp->leadprofile = ReadProfile (xosp->leadmat);
  xosp->cutprofile = ReadProfile (xosp->cutmat);
  xosp->leadcutoff = 3.3;
  xosp->cutcutoff = 2.1;
  xosp->flagBoundaryCondition = FALSE;
  xosp->sigrange = 40;
  xosp->sigscale = 2;

  xosp->tmcutoff = 50.0;
  xosp->tmwindow = 19;
  xosp->fltp = FilterDatNew (AA_FILTER_COMP_KYTE, xosp->tmwindow);
  xosp->fltp->filterdatafile = StringSave ("KSkyte.flt");
  if (ReadFilterData (xosp->fltp) != 24)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 110, "Could not read filter data");
    ErrShow ();
  }
  return xosp;
}

Int2 Main (void)
{
  WindoW  w;
  GrouP   g;
  ButtoN  b1, b2, b3, b4, b5;

  XOSPtr  xosp;

  ProcessUpdatesFirst (FALSE);

  xosp = SetUp ();

  w = FixedWindow (-50, -33, -10, -10,
       "N-terminal Signal Sequence / Transmembrane", CloseSigMeParentProc);
  g = HiddenGroup (w, 5, 0, NULL);
  b1 = PushButton (g, "  Input GI ", ReadGIProc);
  b2 = PushButton (g, "Input FastA", ReadFileProc);
  b3 = PushButton (g, " Parameters", SigMeParamProc);
  b4 = PushButton (g, "   SigMe   ", SigMeProc);
  b5 = PushButton (g, "   Finish  ", EndProg);

  SetObjectExtra (w,  xosp, NULL);
  SetObjectExtra (b1, xosp, NULL);
  SetObjectExtra (b2, xosp, NULL);
  SetObjectExtra (b3, xosp, NULL);
  SetObjectExtra (b4, xosp, NULL);
  SetObjectExtra (b5, xosp, NULL);

  Show (w);
  ProcessEvents ();
  return 0;
}
