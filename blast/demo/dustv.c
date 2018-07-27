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
* File Name: dustv.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.7 $
*
* File Description: a dust utility
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: dustv.c,v $
* Revision 6.7  1998/11/12 20:04:11  kuzio
* .
*
* Revision 6.6  1998/09/16 18:40:35  kuzio
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
#include <urkdust.h>
#include <seqscrl.h>
#include <gphdraw.h>

#define TOP_ERROR 1
static char _this_module[] = "dustv";
#undef  THIS_MODULE
#define THIS_MODULE _this_module
static char _this_file[] = __FILE__;
#undef  THIS_FILE
#define THIS_FILE _this_file

typedef struct xtrins
{
  SeqGraphPtr    sgp;
  Int4           Xscale, Yscale;
  Int4           Xaxislen, Yaxislen;
  Int4           Xprelen, Xpostlen, Yprelen, Ypostlen;
  RecT           dr;
} XIS, PNTR XISPtr;

typedef struct xtrobj
{
  Boolean        flagParamWindow;
  Int4           gi;
  CharPtr        filename;
  GatherScopePtr gsp;
  SeqEntryPtr    sep;
  BioseqPtr      bsp;
  DustDataPtr    ddp;
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
      if (xosp->ddp != NULL)
      {
        xosp->ddp->windowsize = (Int4) ssdp->curval;
        ssdp = ssdp->next;
        xosp->ddp->percent = (FloatHi) ssdp->curval;
        xosp->ddp->level = (Int4) ssdp->curval;
        ssdp = ssdp->next;
        xosp->ddp->minwin = (Int4) ssdp->curval;
        ssdp = ssdp->next;
        xosp->ddp->linker = (Int4) ssdp->curval;
      }
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

static void DustParamProc (ButtoN b)
{
  WindoW           w;
  GrouP            h, g;
  ButtoN           b1, b2, b3;
  XOSPtr           xosp;

  SeqScrollDataPtr ssdp_dust = NULL;

  if ((xosp = (XOSPtr) GetObjectExtra (b)) == NULL)
    return;
  if (xosp->flagParamWindow)
    return;

  w = FixedWindow (-50, -10, -10, -10,
                   "Dust Parameters", CloseParamWindowProc);

  h = HiddenGroup (w, 4, 0, NULL);
  StaticPrompt (h, "window", StringWidth ("window"), dialogTextHeight,
                NULL, 'c');
  StaticPrompt (h, "cutoff", StringWidth ("cutoff"), dialogTextHeight,
                NULL, 'c');
  StaticPrompt (h, "minwin", StringWidth ("minwin"), dialogTextHeight,
                NULL, 'c');
  StaticPrompt (h, "linker", StringWidth ("linker"), dialogTextHeight,
                NULL, 'c');
  g = HiddenGroup (h, 2, 0, NULL);
  AddScrollControl (&ssdp_dust, g, 64, 8, 256, 24);
  g = HiddenGroup (h, 2, 0, NULL);
  AddScrollControl (&ssdp_dust, g, 20, 2, 64, 24);
  g = HiddenGroup (h, 2, 0, NULL);
  AddScrollControl (&ssdp_dust, g, 4, 4, 128, 24);
  g = HiddenGroup (h, 2, 0, NULL);
  AddScrollControl (&ssdp_dust, g, 1, 1, 32, 24);
  SetObjectExtra (w, ssdp_dust, NULL);

  b1 = PushButton (h, "Accept", AcceptParamProc);
  SetObjectExtra (b1, xosp, NULL);
  b2 = PushButton (h, "Apply ", UpdateParamProc);
  SetObjectExtra (b2, xosp, NULL);
  b3 = PushButton (h, "Cancel", CancelParamProc);
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

static void CloseGraphWindowProc (WindoW w)
{
  XISPtr  xisp;

  if ((xisp = (XISPtr) GetObjectExtra (w)) != NULL)
  {
    xisp->sgp = SeqGraphFree (xisp->sgp);
    MemFree (xisp);
  }
  Remove (w);
  return;
}

static void CloseGraphPanelProc (PaneL p, PoinT pt)
{
  CloseGraphWindowProc (ParentWindow (p));
  return;
}

/* not yet

static void AttachGraphProc (ButtoN b)
{
  XOSPtr      xosp;
  XISPtr      xisp;
  SeqAnnotPtr annot, annotg;

  if ((xosp = (XOSPtr) GetObjectExtra (ParentWindow (b))) == NULL)
    return;
  if ((xisp = (XISPtr) GetObjectExtra (b)) == NULL)
    return;

  if ((annotg = SeqAnnotNew ()) != NULL)
  {
    annotg->type = 3;
    annotg->name = StringSave ("dust");
    annotg->data = (Pointer) (xisp->sgp);
    xisp->sgp = NULL;
    annot = xosp->bsp->annot;
    if (annot == NULL)
    {
      xosp->bsp->annot = annotg;
    }
    else
    {
      while (annot->next != NULL)
        annot = annot->next;
      annot->next = annotg;
    }
  }
  CloseGraphWindowProc (ParentWindow (b));
  return;
}

 not yet */

static void DrawGraph (PaneL p)
{
  XISPtr  xisp;
  WindoW  w;
  RecT    r;
  Int4    len;

  w = ParentWindow (p);
  if ((xisp = (XISPtr) GetObjectExtra (w)) == NULL)
    return;

  len = xisp->sgp->numval;

/* by two's
  xisp->Xscale = 1;
  while (len > 256)
  {
    len /= 2;
    xisp->Xscale *= 2;
  }
 by two's */

  xisp->Xscale = len / 256;
  if ((len % 256) != 0)
    (xisp->Xscale)++;

  ObjectRect (p, &r);
  xisp->dr.left   = r.left;
  xisp->dr.top    = r.top;
  xisp->dr.right  = r.right;
  xisp->dr.bottom = r.bottom - (Int2) (xisp->Ypostlen);
  DrawGraphToPanel (xisp->sgp, xisp->Xscale, xisp->Yscale,
                    xisp->Yaxislen, &(xisp->dr), 1,
                    xisp->sgp->numval);
  return;
}

static void DustProc (ButtoN b)
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

  Int4           i;
  DustRegionPtr  drp;
  FloatHiPtr     fhi, fhit;
  FloatHi        minscore, maxscore, deltascore;
  Int4           Yscale;

  SeqGraphPtr    sgp;

  WindoW     w;
  PaneL      p;

  if ((xosp = (XOSPtr) GetObjectExtra (b)) == NULL)
    return;

  gsp = xosp->gsp;
  gi = xosp->gi;
  fastafile = xosp->filename;

  if (gi > 0)
  {
    if (!EntrezInit ("dustv", FALSE, &flagHaveNet))
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
    sep = FastaToSeqEntry (fiop, TRUE);
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
      if (ISA_na (xosp->bsp->mol))
      {
        drp = DustBioseq (xosp->bsp, 0, xosp->bsp->length-1, xosp->ddp);
        fhit = fhi = DustForGraph (drp, xosp->bsp->length, 0,
                                   xosp->bsp->length-1);
        DustRegionFree (drp);
        if (fhi == NULL)
        {
          return;
        }
        minscore = 0.0;
        maxscore = 0.0;
        for (i = 0; i < xosp->bsp->length; i++)
        {
          if (*fhit > maxscore)
            maxscore = *fhit;
          fhit++;
        }

        title = FastaTitle (xosp->bsp, "DUST: >", NULL);

        if ((sgp = SeqGraphNew ()) == NULL)
        {
          MemFree (fhi);
          MemFree (title);
          return;
        }

        xisp = MemNew (sizeof (XIS));
        xisp->sgp = sgp;
        xisp->Xscale = 1;
        xisp->Yscale = 1;
        xisp->Xaxislen = 250;
        xisp->Yaxislen = 100;
        xisp->Xprelen = 0;
        xisp->Yprelen = 50;
        xisp->Xpostlen = 50;
        xisp->Ypostlen = 50;
        sgp->loc = SeqLocIntNew (0, xosp->bsp->length-1, xosp->bsp->strand,
                                 xosp->bsp->id);
        sgp->title = title;
        sgp->flags[2] = 1;
        sgp->numval = xosp->bsp->length;
        sgp->values = (Pointer) fhi;

        fhit = fhi;
        deltascore = maxscore - minscore;
        if ((Yscale = 100 / (Int4) deltascore) > 0)
        {
          minscore *= Yscale;
          maxscore *= Yscale;
          for (i = 0; i < xosp->bsp->length; i++)
          {
            *fhit *= Yscale;
            fhit++;
          }
          xisp->Yscale = Yscale;
        }
        sgp->max.realvalue = maxscore;
        sgp->min.realvalue = minscore;

        w = FixedWindow (-50, -50, -10, -10, "Dust", CloseGraphWindowProc);
        SetObjectExtra (w, xisp, NULL);
        p = SimplePanel (w,
                         (Int2) (xisp->Xprelen+xisp->Xaxislen+xisp->Xpostlen),
                         (Int2) (xisp->Yprelen+xisp->Yaxislen+xisp->Ypostlen),
                         DrawGraph);
        SetPanelClick (p, NULL, NULL, NULL, CloseGraphPanelProc);
        RealizeWindow (w);
        Show (w);
      }
      else
      {
        ErrPostEx (SEV_ERROR, TOP_ERROR, 101, "Not a nucleic acid Bioseq");
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

static void CloseDustParentProc (WindoW w)
{
  XOSPtr  xosp;

  if ((xosp = (XOSPtr) GetObjectExtra (w)) != NULL)
  {
    MemFree (xosp->filename);
    xosp->gsp = NULL;          /* is static */
    xosp->sep = SeqEntryFree (xosp->sep);
    xosp->bsp = NULL;          /* should be in seqentry */
    xosp->ddp = DustDataFree (xosp->ddp);
    MemFree (xosp);
  }
  Remove (w);
  QuitProgram ();
  return;
}

static void EndProg (ButtoN b)
{
  CloseDustParentProc (ParentWindow (b));
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

  xosp = MemNew (sizeof (XOS));

  xosp->flagParamWindow = FALSE;
  xosp->gi = 0;
  xosp->filename = NULL;
  xosp->gsp = gsp;
  xosp->sep = NULL;
  xosp->bsp = NULL;

  xosp->ddp = DustDataNew ();

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
                   "Dust Low Complexity", CloseDustParentProc);
  g = HiddenGroup (w, 5, 0, NULL);
  b1 = PushButton (g, "  Input GI ", ReadGIProc);
  b2 = PushButton (g, "Input FastA", ReadFileProc);
  b3 = PushButton (g, " Parameters", DustParamProc);
  b4 = PushButton (g, "    Dust   ", DustProc);
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
