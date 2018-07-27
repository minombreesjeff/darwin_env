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
* File Name: searchv.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-09-17
*
* $Revision: 6.2 $
*
* File Description: nucleotide or peptide pattern match
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: searchv.c,v $
* Revision 6.2  1999/03/13 03:43:45  kuzio
* cast
*
* Revision 6.1  1998/09/21 14:49:38  kuzio
* vibrant srchaa srchnt
*
* ==========================================================================
*/

#include <ncbi.h>
#include <vibrant.h>
#include <accentr.h>
#include <gather.h>
#include <tofasta.h>
#include <urkptpf.h>
#include <urkutil.h>
#include <seqscrl.h>

#define TOP_ERROR 1
static char _this_module[] = "searchv";
#undef  THIS_MODULE
#define THIS_MODULE _this_module
static char _this_file[] = __FILE__;
#undef  THIS_FILE
#define THIS_FILE _this_file

typedef struct xtrins
{
  SegmenT picture;
} XIS, PNTR XISPtr;

typedef struct xtrobj
{
  Boolean        flagParamWindow;
  CharPtr        patternfile, patternnamesfile, pattern;
  ValNodePtr     namelist;
  Boolean        flagSkip, flagNeedCutsite;
  Int4           cutoff;
  CharPtr        taxon;
  Int4           mismatchcur, mismatchmin, mismatchmax;
  ComPatPtr      cpp;
  Boolean        flagPatNuc;
  Int4           gi;
  CharPtr        filename;
  GatherScopePtr gsp;
  SeqEntryPtr    sep;
  BioseqPtr      bsp;
  Boolean        flagIsNuc;
} XOS, PNTR XOSPtr;

/* get sequence common */

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

static void GetThisBioseq (XOSPtr xosp)
{
  GatherScopePtr  gsp;
  CharPtr         fastafile;
  Int4            gi;
  FILE            *fiop;
  Boolean         flagHaveNet;
  SeqEntryPtr     sep;
  Int2            moltype;

  gsp = xosp->gsp;
  fastafile = xosp->filename;
  gi = xosp->gi;
  xosp->sep = SeqEntryFree (xosp->sep);

  if (gi > 0)
  {
    if (!EntrezInit ("epiv", FALSE, &flagHaveNet))
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 102,
                 "Entrez init failed");
      ErrShow ();
      return;
    }
    sep = EntrezSeqEntryGet (gi, SEQENTRY_READ_BIOSEQ);
    EntrezFini ();
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
    sep = FastaToSeqEntry (fiop, xosp->flagIsNuc);
    FileClose (fiop);
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
    return;
  }
  else
  {
    xosp->sep = sep;
    xosp->bsp = NULL;
    xosp->gi = gi;
    GatherSeqEntry (sep, (Pointer) xosp, GetBioseq, (Pointer) gsp);
  }

  if (xosp->bsp != NULL)
  {
    if (ISA_na (xosp->bsp->mol))
    {
      xosp->flagIsNuc = TRUE;
      moltype = 0;
    }
    else
    {
      xosp->flagIsNuc = FALSE;
      moltype = 1;
    }
    if (xosp->flagIsNuc != xosp->flagPatNuc)
    {
      xosp->cpp = ComPatFree (xosp->cpp);
    }
    if (xosp->cpp == NULL)
    {
      if (xosp->pattern != NULL)
      {
        xosp->flagPatNuc = xosp->flagIsNuc;
        if ((xosp->cpp = CompilePattern (xosp->pattern, moltype)) != NULL)
          StrCpy (xosp->cpp->name, "User Pattern");
      }
      else
      {
        if (moltype == 0)
        {
          xosp->patternfile = MemFree (xosp->patternfile);
          xosp->patternfile = StringSave ("ncbiren.dat");
          xosp->patternnamesfile = MemFree (xosp->patternnamesfile);
          xosp->patternnamesfile = StringSave ("ncbirnam.dat");
          xosp->namelist = ValNodeFreeData (xosp->namelist);
          xosp->namelist = ReadPatternNames (xosp->patternnamesfile);
          xosp->cpp = ReadRENPattern (xosp->patternfile, xosp->flagNeedCutsite,
                                      xosp->namelist);
          PalindromeCheck (xosp->cpp);
        }
        else
        {
          xosp->patternfile = MemFree (xosp->patternfile);
          xosp->patternfile = StringSave ("ncbipros.dat");
          xosp->patternnamesfile = MemFree (xosp->patternnamesfile);
          xosp->patternnamesfile = StringSave ("ncbipnam.dat");
          xosp->namelist = ValNodeFreeData (xosp->namelist);
          xosp->namelist = ReadPatternNames (xosp->patternnamesfile);
          xosp->cpp = ReadPrositePattern (xosp->patternfile,
                                          xosp->flagSkip, xosp->cutoff,
                                          xosp->taxon, xosp->namelist);
        }
      }
    }
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

  w = ParentWindow (b);
  if ((ssdp = (SeqScrollDataPtr) GetObjectExtra (w)) != NULL)
  {
    if ((xosp = (XOSPtr) GetObjectExtra (b)) != NULL)
    {
      xosp->mismatchcur = (Int4) ssdp->curval;
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

static void SearchParamProc (ButtoN b)
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

  w = FixedWindow (-50, -10, -10, -10,
                   "Search Parameters", CloseParamWindowProc);
  h = HiddenGroup (w, 1, 0, NULL);
  h1 = HiddenGroup (h, 2, 0, NULL);

  g1 = HiddenGroup (h1, 1, 0, NULL);
  g = HiddenGroup (g1, 1, 0, NULL);
  StaticPrompt (g, "mismatch", StringWidth ("12345678"), dialogTextHeight,
                NULL, 'c');
  g = HiddenGroup (g1, 2, 0, NULL);
  AddScrollControl (&ssdp, g, (Int2) xosp->mismatchcur,
                    (Int2) xosp->mismatchmin, (Int2) xosp->mismatchmax, 24);

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

static SegmenT PlotSites (BioseqPtr bsp, SeqLocPtr slp, CharPtr name,
                          SegmenT seg, Int4 top)
{
  Int4      start, length, count;
  Char      buffer[32], tname[1024];

  if (seg == NULL)
    seg = CreatePicture ();
  if (seg == NULL)
    return seg;

  count = 0;
  while (slp != NULL)
  {
    count++;
    start = SeqLocStart (slp);
    AddLine (seg, start, top, start, top-10, FALSE, 0);
    slp = slp->next;
  }
  if (count > 0)
  {
    length = bsp->length;
    AddLine (seg, 0, top, length, top, FALSE, 0);
    AddLine (seg, 0, top-10, length, top-10, FALSE, 0);
    length += 10;
    StrCpy (tname, name);
    if (StrLen (tname) > 10)
      tname[10] = '\0';
    sprintf (buffer, "%10s %6ld", tname, (long) count);
    AddLabel (seg, length, top-5, buffer, SMALL_TEXT, 0, MIDDLE_RIGHT, 0);
  }
  return seg;
}

static void SearchProc (ButtoN b)
{
  XOSPtr         xosp;
  XISPtr         xisp;

  ComPatPtr      cpp;
  CharPtr        title, cptr;
  BioseqPtr      bsp;
  SeqAlignPtr    sap;
  SeqLocPtr      slpn, slp;

  Int4        XAxis, XScale;
  WindoW      w;
  VieweR      v;
  GrouP       g;
  SegmenT     seg;
  Int4        top;

  if ((xosp = (XOSPtr) GetObjectExtra (b)) == NULL)
    return;

  bsp = xosp->bsp;
  title = FastaTitle (bsp, ">", NULL);
  cptr = title;
  while (*cptr != '\0')
  {
    if (*cptr == ' ')
      break;
    cptr++;
  }
  *cptr = '\0';
  if (StrLen (title) > 16)
    title[16] = '\0';

  xisp = (XISPtr) MemNew (sizeof (XIS));
  XAxis = 600;
  XScale = bsp->length / (XAxis-100);
  if (bsp->length % (XAxis-100) != 0)
    XScale++;
  cpp = xosp->cpp;
  top = 100;
  seg = NULL;

  while (cpp != NULL)
  {
    sap = PatternMatchBioseq (bsp, cpp, xosp->mismatchcur);
    if (sap != NULL)
    {
      slp = MatchSa2Sl (&sap);
      seg = PlotSites (bsp, slp, cpp->name, seg, top);
      top -= 10;
      while (slp != NULL)
      {
        slpn = slp->next;
        SeqLocFree (slp);
        slp = slpn;
      }
    }
    cpp = cpp->nextpattern;
  }

  if (seg == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 501, "No Matches");
    ErrShow ();
    MemFree (xisp);
    ArrowCursor ();
    return;
  }

  xisp->picture = seg;
  w = FixedWindow (10, 10, -10, -10, "Search Plot", CloseGraphWindowProc);
  SetObjectExtra (w, xisp, CleanUpGraphWindow);
  g = HiddenGroup (w, -1, 0, NULL);
  v = CreateViewer (g, (Int2) XAxis, 500, TRUE, TRUE);
  AttachPicture (v, seg, INT4_MIN, INT4_MAX, UPPER_LEFT, XScale, 1, NULL);
  PushButton (g, "Close", CloseGraphWindowButton);
  RealizeWindow (w);
  Show (w);

  SeqLocFree (slp);
  MemFree (title);

  ArrowCursor ();
  return;
}

/* button 5 */

static void CloseSearchParentProc (WindoW w)
{
  XOSPtr  xosp;

  if ((xosp = (XOSPtr) GetObjectExtra (w)) != NULL)
  {
    MemFree (xosp->filename);
    xosp->gsp = NULL;          /* is static */
    xosp->sep = SeqEntryFree (xosp->sep);
    xosp->bsp = NULL;          /* should be in seqentry */
    xosp->cpp = ComPatFree (xosp->cpp);
    MemFree (xosp);
  }
  Remove (w);
  QuitProgram ();
  return;
}

static void EndProg (ButtoN b)
{
  CloseSearchParentProc (ParentWindow (b));
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
  xosp->patternfile = NULL;
  xosp->patternnamesfile = NULL;
  xosp->pattern = NULL;
  xosp->namelist = NULL;
  xosp->flagSkip = FALSE;
  xosp->flagNeedCutsite = TRUE;
  xosp->cutoff = 1000000;
  xosp->taxon = NULL;

  xosp->mismatchcur = 0;
  xosp->mismatchmin = 0;
  xosp->mismatchmax = 2;
  xosp->cpp = NULL;
  xosp->flagPatNuc = TRUE;

  xosp->gi = 0;
  xosp->filename = NULL;
  xosp->gsp = gsp;
  xosp->sep = NULL;
  xosp->bsp = NULL;
  xosp->flagIsNuc = TRUE;

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
                   "AA/NT Pattern Search", CloseSearchParentProc);
  g = HiddenGroup (w, 5, 0, NULL);
  b1 = PushButton (g, "  Input GI ", ReadGIProc);
  b2 = PushButton (g, "Input FastA", ReadFileProc);
  b3 = PushButton (g, " Parameters", SearchParamProc);
  b4 = PushButton (g, "   Search  ", SearchProc);
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
