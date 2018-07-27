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
* File Name: ccpv.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.4 $
*
* File Description: coiled coil prediction
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: ccpv.c,v $
* Revision 6.4  1998/09/16 18:40:33  kuzio
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
#include <urkpcc.h>
#include <seqscrl.h>
#include <gphdraw.h>

#define TOP_ERROR 1
static char _this_module[] = "ccpv";
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
  PccDatPtr      pccp;
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
      if (xosp->pccp != NULL)
      {
        xosp->pccp->window = (Int4) ssdp->curval;
        ssdp = ssdp->next;
        xosp->pccp->linker = (Int4) ssdp->curval;
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

static void CcpParamProc (ButtoN b)
{
  WindoW           w;
  GrouP            h, g, h1;
  ButtoN           b1, b2, b3;
  XOSPtr           xosp;

  SeqScrollDataPtr ssdp_ccp = NULL;

  if ((xosp = (XOSPtr) GetObjectExtra (b)) == NULL)
    return;
  if (xosp->flagParamWindow)
    return;

  w = FixedWindow (-50, -10, -10, -10,
                   "Ccp Parameters", CloseParamWindowProc);

  h = HiddenGroup (w, 1, 0, NULL);
  h1 = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (h1, "window", StringWidth ("window"), dialogTextHeight,
                NULL, 'c');
  StaticPrompt (h1, "linker", StringWidth ("linker"), dialogTextHeight,
                NULL, 'c');
  h1 = HiddenGroup (h, 2, 0, NULL);
  g = HiddenGroup (h1, 2, 0, NULL);
  AddScrollControl (&ssdp_ccp, g, 22, 7, 42, 24);
  g = HiddenGroup (h1, 2, 0, NULL);
  AddScrollControl (&ssdp_ccp, g, 0, 0, 64, 24);
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

  xisp->Xscale = len / xisp->Xaxislen;
  if ((len % xisp->Xaxislen) != 0)
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

static void CcpProc (ButtoN b)
{
  Boolean     flagHaveNet;

  SeqEntryPtr sep;
  Int4        i, gi;
  CharPtr     fastafile;
  FILE        *fiop;
  CharPtr     title;

  GatherScopePtr      gsp;
  XOSPtr              xosp;
  XISPtr              xisp;

  FloatHi        minscore, maxscore;
  FloatHiPtr     pcs, pccscr;
  SeqGraphPtr    sgp;

  WindoW     w;
  PaneL      p;

  if ((xosp = (XOSPtr) GetObjectExtra (b)) == NULL)
    return;

  if (ReadPccData (xosp->pccp) == 0)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 101,
               "Could not open or read %s data file", xosp->pccp->pccdatafile);
    ErrShow ();
    return;
  }

  gsp = xosp->gsp;
  gi = xosp->gi;
  fastafile = xosp->filename;

  if (gi > 0)
  {
    if (!EntrezInit ("ccpv", FALSE, &flagHaveNet))
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 102,
                 "Entrez init failed");
      ErrShow ();
      xosp->pccp->res = (CharPtr) MemFree (xosp->pccp->res);
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
      xosp->pccp->res = (CharPtr) MemFree (xosp->pccp->res);
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
    xosp->pccp->res = (CharPtr) MemFree (xosp->pccp->res);
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
        pcs = pccscr = PredictCCBioseq (xosp->bsp, 0, xosp->bsp->length-1,
                                        xosp->pccp);
        xosp->pccp->res = (CharPtr) MemFree (xosp->pccp->res);
        if (pccscr == NULL)
        {
          return;
        }
        for (i = 0; i < xosp->bsp->length; i++)
        {
          *pcs *= 100.0;
          pcs++;
        }
        minscore = 0.0;
        maxscore = 100.0;

        title = FastaTitle (xosp->bsp, "CCP: >", NULL);

        if ((sgp = SeqGraphNew ()) == NULL)
        {
          MemFree (pccscr);
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
        sgp->max.realvalue = maxscore;
        sgp->min.realvalue = minscore;
        sgp->values = (Pointer) pccscr;

        w = FixedWindow (-50, -50, -10, -10, "Ccp", CloseGraphWindowProc);
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

static void CloseCcpParentProc (WindoW w)
{
  XOSPtr  xosp;

  if ((xosp = (XOSPtr) GetObjectExtra (w)) != NULL)
  {
    MemFree (xosp->filename);
    xosp->gsp = NULL;          /* is static */
    xosp->sep = SeqEntryFree (xosp->sep);
    xosp->bsp = NULL;          /* should be in seqentry */
    xosp->pccp = PccDatFree (xosp->pccp);
    MemFree (xosp);
  }
  Remove (w);
  QuitProgram ();
  return;
}

static void EndProg (ButtoN b)
{
  CloseCcpParentProc (ParentWindow (b));
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

  xosp->pccp = PccDatNew ();

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
                   "Coiled Coil Prediction", CloseCcpParentProc);
  g = HiddenGroup (w, 5, 0, NULL);
  b1 = PushButton (g, "  Input GI ", ReadGIProc);
  b2 = PushButton (g, "Input FastA", ReadFileProc);
  b3 = PushButton (g, " Parameters", CcpParamProc);
  b4 = PushButton (g, "    Ccp    ", CcpProc);
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
