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
* File Name: cnsrtv.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.2 $
*
* File Description: consort - codon usage/bias tree
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: cnsrtv.c,v $
* Revision 6.2  1998/09/16 18:40:34  kuzio
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
#include <urkcnsrt.h>
#include <urktree.h>
#include <gtrdraw.h>
#include <seqscrl.h>

#define TOP_ERROR 1
static char _this_module[] = "cnsrtv";
#undef  THIS_MODULE
#define THIS_MODULE _this_module
static char _this_file[] = __FILE__;
#undef  THIS_FILE
#define THIS_FILE _this_file

typedef struct xtrins
{
  Int4           Xscale, Yscale, Xdelta;
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
  Int4           xdeltaval, xdeltamin, xdeltamax;
  Int4           ydeltaval, ydeltamin, ydeltamax;
  Int4           treestyle;
  GrouP          gstyle;
} XOS, PNTR XOSPtr;

/* common */

static Boolean GetBioseq (GatherContextPtr gcp)
{
  XOSPtr         xosp;
  BioseqPtr      bsp;
  Int4           entrezgi;

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
  if (xosp->gi > 0)
  {
    entrezgi = GetGIForSeqId (bsp->id);
    if (xosp->gi == entrezgi)
      xosp->bsp = bsp;
    return TRUE;
  }
  if (xosp->filename != NULL)
  {
    xosp->bsp = bsp;
    return TRUE;
  }
  return TRUE;
}

static void GetThisBioseq (XOSPtr xosp)
{
  GatherScopePtr  gsp;
  CharPtr         asnseqentfile;
  Int4            gi;
  AsnIoPtr        aiop;
  Boolean         flagHaveNet;
  SeqEntryPtr     sep;

  gsp = xosp->gsp;

  aiop = NULL;
  asnseqentfile = xosp->filename;
  gi = xosp->gi;
  xosp->sep = SeqEntryFree (xosp->sep);

  if (gi > 0)
  {
    if (!EntrezInit ("twopv", FALSE, &flagHaveNet))
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
  else if (asnseqentfile != NULL)
  {
    if ((aiop = AsnIoOpen (asnseqentfile, "r")) == NULL)
    {
      Message (MSG_ERROR, "Failed to open: %s", asnseqentfile);
    }
    sep = SeqEntryAsnRead (aiop, NULL);
  }
  else
  {
    sep = NULL;
  }

  if (sep == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 104,
               "No SeqEntry");
    ErrShow ();
  }
  else
  {
    xosp->sep = sep;
    xosp->bsp = NULL;
    xosp->gi = gi;
    GatherSeqEntry (sep, (Pointer) xosp, GetBioseq, (Pointer) gsp);
  }

  if (gi > 0)
    EntrezFini ();
  else
    AsnIoClose (aiop);

  if (xosp->bsp != NULL)
  {
    if (!ISA_na (xosp->bsp->mol))
    {
      xosp->sep = SeqEntryFree (xosp->sep);
      xosp->bsp = NULL;
      xosp->filename = (CharPtr) MemFree (xosp->filename);
      xosp->gi = 0;
      ErrPostEx (SEV_ERROR, TOP_ERROR, 101, "Not nucleic acid Bioseq");
      ErrShow ();
    }
  }
  else
  {
    xosp->sep = SeqEntryFree (xosp->sep);
    xosp->filename = (CharPtr) MemFree (xosp->filename);
    xosp->gi = 0;
    ErrPostEx (SEV_ERROR, TOP_ERROR, 101, "No Bioseq");
    ErrShow ();
  }
  return;
}

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
      GetThisBioseq (xosp);
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
      GetThisBioseq (xosp);
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

  if ((xosp = (XOSPtr) GetObjectExtra (b)) != NULL)
  {
    w = ParentWindow (b);
    if ((ssdp = (SeqScrollDataPtr) GetObjectExtra (w)) != NULL)
    {
      xosp->xdeltaval = (Int4) ssdp->curval;
      ssdp = ssdp->next;
      xosp->ydeltaval = (Int4) ssdp->curval;
    }
    xosp->treestyle = (Int4) GetValue (xosp->gstyle);
    xosp->treestyle -= 1;
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

static void ConsortParamProc (ButtoN b)
{
  WindoW           w;
  GrouP            h, h1, g, g1;
  ButtoN           b1, b2, b3;
  XOSPtr           xosp;

  SeqScrollDataPtr ssdp = NULL;

  if ((xosp = (XOSPtr) GetObjectExtra (b)) == NULL)
    return;
  if (xosp->flagParamWindow)
    return;

  w = FixedWindow (-75, -10, -10, -10,
                   "Consort Parameters", CloseParamWindowProc);
  h = HiddenGroup (w, 1, 0, NULL);
  h1 = HiddenGroup (h, 2, 0, NULL);

  g1 = HiddenGroup (h1, 1, 0, NULL);
  g = HiddenGroup (g1, 1, 0, NULL);
  StaticPrompt (g, "xdelta", StringWidth ("1234567890"), dialogTextHeight,
                NULL, 'c');
  g = HiddenGroup (g1, 2, 0, NULL);
  AddScrollControl (&ssdp, g, (Int2) xosp->xdeltaval,
                    (Int2) xosp->xdeltamin, (Int2) xosp->xdeltamax, 24);

  g1 = HiddenGroup (h1, 1, 0, NULL);
  g = HiddenGroup (g1, 1, 0, NULL);
  StaticPrompt (g, "ydelta", StringWidth ("1234567890"), dialogTextHeight,
                NULL, 'c');
  g = HiddenGroup (g1, 2, 0, NULL);
  AddScrollControl (&ssdp, g, (Int2) xosp->ydeltaval,
                    (Int2) xosp->ydeltamin, (Int2) xosp->ydeltamax, 24);

  SetObjectExtra (w, ssdp, NULL);

  h1 = HiddenGroup (h, 1, 0, NULL);
  StaticPrompt (h1, "tree style", 0, dialogTextHeight, NULL, 'l');
  g = HiddenGroup (h1, 1, 0, NULL);
  RadioButton (g, "LR");
  RadioButton (g, "TB");
  SetValue (g, 1);
  xosp->gstyle = g;

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

static void CleanUpGraphWindow (GraphiC g, VoidPtr data)
{
  XISPtr  xisp;

  if ((xisp = (XISPtr) data) != NULL)
  {
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

static void ConsortProc (ButtoN b)
{
  XOSPtr         xosp;
  XISPtr         xisp;
  TreeNodePtr    ptrNode;

  WindoW      w;
  VieweR      v;
  GrouP       g;
  SegmenT     seg;
  Int4        xmax, xmin;
  Int4        xoffset, yoffset;

  if ((xosp = (XOSPtr) GetObjectExtra (b)) == NULL)
    return;

  if (xosp->sep == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 101, "No SeqEntry");
    ErrShow ();
    return;
  }

  WatchCursor ();

  xoffset = xosp->xdeltaval;
  yoffset = xosp->ydeltaval;

  ptrNode = ConsortSeqEntry (xosp->sep);
  SetAllLeftRightRootedTreeNeighbors (ptrNode);
  SetAllPivotNodes (ptrNode);
  switch (xosp->treestyle)
  {
   default:
   case 0:
    SetAllTheoreticalCoordinates (ptrNode, 300, 50);
    AdjustAllTheoreticalCoordinates (ptrNode, 300);
    ChangeTreeScale (ptrNode, 300, 50, (FloatHi) xoffset, (FloatHi) yoffset);
    break;
   case 1:
    SetAllCoordinates (ptrNode, 300, 50, xoffset, yoffset);
    break;
  }
  xmin = GetMinimumTreeXcoord (ptrNode);
  if (xmin-50 > 0)
    ShiftTree (ptrNode, (-1*(xmin-50)), 0);
  if (xmin < 0)
    ShiftTree (ptrNode, ((-1*xmin)+50), 0);

  xmin = GetMinimumTreeXcoord (ptrNode);
  xmax = GetMaximumTreeXcoord (ptrNode);
  if (xmin < 0)
    xmin *= -1;
  if (xmax < 0)
    xmax *= -1;
  xisp = (XISPtr) MemNew (sizeof (XIS));
  if (xmax > xmin)
    xisp->Xdelta = (2*(xmax+100)) + 10;
  else
    xisp->Xdelta = (2*(xmin+100)) + 10;
  seg = DrawTree (ptrNode);
  xisp->picture = seg;

  w = FixedWindow (10, 10, 640, 640, "Consort", CloseGraphWindowProc);
  SetObjectExtra (w, xisp, CleanUpGraphWindow);
  g = HiddenGroup (w, -1, 0, NULL);
  v = CreateViewer (g, 560, 560, TRUE, TRUE);
  AttachPicture (v, seg, INT4_MIN, INT4_MAX, UPPER_LEFT, 1, 1, NULL);
  PushButton (g, "Close", CloseGraphWindowButton);
  RealizeWindow (w);
  ArrowCursor ();
  Show (w);

  return;
}

/* button 5 */

static void CloseConsortParentProc (WindoW w)
{
  XOSPtr  xosp;

  if ((xosp = (XOSPtr) GetObjectExtra (w)) != NULL)
  {
    xosp->gsp  = NULL;          /* is static */
    MemFree (xosp->filename);
    xosp->sep = SeqEntryFree (xosp->sep);
    xosp->bsp = NULL;           /* should be in seqentry */
    MemFree (xosp);
  }
  Remove (w);
  QuitProgram ();
  return;
}

static void EndProg (ButtoN b)
{
  CloseConsortParentProc (ParentWindow (b));
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
  xosp->xdeltamin = 1;
  xosp->xdeltamax = 64;
  xosp->xdeltaval = 20;
  xosp->ydeltamin = 1;
  xosp->ydeltamax = 64;
  xosp->ydeltaval = 20;
  xosp->treestyle = 0;

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
                   "Consort", CloseConsortParentProc);
  g = HiddenGroup (w, 7, 0, NULL);
  b1 = PushButton (g, " Input GI ", ReadGIProc);
  b2 = PushButton (g, " Input ASN", ReadFileProc);
  b3 = PushButton (g, "Parameters", ConsortParamProc);
  b4 = PushButton (g, "  Consort ", ConsortProc);
  b5 = PushButton (g, "  Finish  ", EndProg);

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
