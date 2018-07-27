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
* File Name: twopv.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.6 $
*
* File Description: region of best blast identity
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: twopv.c,v $
* Revision 6.6  1998/12/18 16:24:58  kuzio
* big GIs
*
* Revision 6.5  1998/09/16 18:40:36  kuzio
* cvs logging
*
* ==========================================================================
*/

#include <ncbi.h>
#include <vibrant.h>
#include <accentr.h>
#include <gather.h>
#include <tofasta.h>
#include <blast.h>
#include <urkutil.h>
#include <urkptpf.h>
#include <seqscrl.h>
#include <gxydraw.h>
#include <picture.h>

#define TOP_ERROR 1
static char _this_module[] = "twopv";
#undef  THIS_MODULE
#define THIS_MODULE _this_module
static char _this_file[] = __FILE__;
#undef  THIS_FILE
#define THIS_FILE _this_file

typedef struct xtrins
{
  Int4           Xscale, Yscale;
  SegmenT        picture;
} XIS, PNTR XISPtr;

typedef struct xtrobj
{
  Boolean        flagParamWindow;
  GrouP          gpostblast;
  Boolean        flagPattern;
  ButtoN         bTG, bPG, bTF, bPF, bThis;
  Int4           giT, giP;
  GatherScopePtr gsp;
  CharPtr        filenameT, filenameP;
  SeqEntryPtr    sepT, sepP;
  BioseqPtr      bspT, bspP;
  Int4           dotcutoff, dotcutoffmin, dotcutoffmax;
} XOS, PNTR XOSPtr;

static void CloseParamWindowProc (WindoW w);

static Boolean GetBioseq (GatherContextPtr gcp)
{
  XOSPtr         xosp;
  BioseqPtr      bsp;
  Int4           entrezgi;

  if (gcp == NULL)
    return FALSE;
  if ((xosp = (XOSPtr) gcp->userdata) == NULL)
    return FALSE;

  if (xosp->bTG == xosp->bThis || xosp->bTF == xosp->bThis)
  {
    if (xosp->bspT != NULL)
      return TRUE;
  }
  else
  {
    if (xosp->bspP != NULL)
      return TRUE;
  }

  if (gcp->thistype != OBJ_BIOSEQ)
    return TRUE;
  if ((bsp = (BioseqPtr) (gcp->thisitem)) == NULL)
    return TRUE;

  if (xosp->giT > 0 && xosp->bTG == xosp->bThis)
  {
    entrezgi = GetGIForSeqId (bsp->id);
    if (xosp->giT == entrezgi)
      xosp->bspT = bsp;
    return TRUE;
  }
  if (xosp->filenameT != NULL && xosp->bTF == xosp->bThis)
  {
    xosp->bspT = bsp;
    return TRUE;
  }
  if (xosp->giP > 0 && xosp->bPG == xosp->bThis)
  {
    entrezgi = GetGIForSeqId (bsp->id);
    if (xosp->giP == entrezgi)
      xosp->bspP = bsp;
    return TRUE;
  }
  if (xosp->filenameP != NULL && xosp->bPF == xosp->bThis)
  {
    xosp->bspP = bsp;
    return TRUE;
  }
  return TRUE;
}

static void GetThisBioseq (XOSPtr xosp)
{
  GatherScopePtr  gsp;
  CharPtr         fastafile;
  Int4            gi;
  FILE            *fiop;
  Boolean         flagHaveNet;
  SeqEntryPtr     sep;

  gsp = xosp->gsp;

  if (xosp->bTG == xosp->bThis || xosp->bTF == xosp->bThis)
  {
    fastafile = xosp->filenameT;
    gi = xosp->giT;
    xosp->sepT = SeqEntryFree (xosp->sepT);
  }
  else
  {
    fastafile = xosp->filenameP;
    gi = xosp->giP;
    xosp->sepP = SeqEntryFree (xosp->sepP);
  }

  if (gi > 0)
  {
    if (!EntrezInit ("twopv", FALSE, &flagHaveNet))
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 102,
                 "Entrez init failed");
      ErrShow ();
      return;
    }
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
               "No SeqEntry");
    ErrShow ();
  }
  else
  {
    if (xosp->bTG == xosp->bThis || xosp->bTF == xosp->bThis)
    {
      xosp->sepT = sep;
      xosp->bspT = NULL;
      xosp->giT = gi;
    }
    else
    {
      xosp->sepP = sep;
      xosp->bspP = NULL;
      xosp->giP = gi;
    }
    GatherSeqEntry (sep, (Pointer) xosp, GetBioseq, (Pointer) gsp);
  }

  if (gi > 0)
    EntrezFini ();
  else
    FileClose (fiop);

  return;
}

/* button 1 & 2 */

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
      if (xosp->bTG == xosp->bThis)
      {
        xosp->giT = atol (buf);
        xosp->filenameT = (CharPtr) MemFree (xosp->filenameT);
      }
      else
      {
        xosp->giP = atol (buf);
        xosp->filenameP = (CharPtr) MemFree (xosp->filenameP);
      }
      GetThisBioseq (xosp);
    }
  }
  CloseGIWindowProc (ParentWindow (b));
  xosp->bThis = NULL;
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
    xosp->bThis = b;
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

/* button 3 & 4 */

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
      if (xosp->bTF == b)
      {
        MemFree (xosp->filenameT);
        xosp->filenameT = StringSave (filename);
        xosp->giT = 0;
      }
      else
      {
        MemFree (xosp->filenameP);
        xosp->filenameP = StringSave (filename);
        xosp->giP = 0;
      }
      xosp->bThis = b;
      GetThisBioseq (xosp);
      xosp->bThis = NULL;
    }
  }
  return;
}

/* button 5 */

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
      xosp->dotcutoff = (Int4) ssdp->curval;
    }
    xosp->flagPattern = (Boolean) GetValue (xosp->gpostblast);
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

static void TwopParamProc (ButtoN b)
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
                   "Blast 2 Proteins Parameters", CloseParamWindowProc);
  h = HiddenGroup (w, 1, 0, NULL);
  h1 = HiddenGroup (h, 2, 0, NULL);
  g1 = HiddenGroup (h1, 1, 0, NULL);
  g = HiddenGroup (g1, 1, 0, NULL);
  StaticPrompt (g, "win cut", StringWidth ("12345678"), dialogTextHeight,
                NULL, 'c');
  g = HiddenGroup (g1, 2, 0, NULL);
  AddScrollControl (&ssdp, g, (Int2) xosp->dotcutoff,
                    (Int2) xosp->dotcutoffmin, (Int2) xosp->dotcutoffmax, 24);

  SetObjectExtra (w, ssdp, NULL);

  h1 = HiddenGroup (h, 1, 0, NULL);
  g = HiddenGroup (h1, 1, 0, NULL);
  CheckBox (g, "Post Blast stringent pattern match", NULL);
  SetValue (g, 0);
  xosp->gpostblast = g;

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

/* button 6 */

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

static Int4 SapCount (SeqAlignPtr sap)
{
  Int4 count;

  count = 0;
  while (sap != NULL)
  {
    count++;
    sap = sap->next;
  }
  return count;
}

static void TwopProc (ButtoN b)
{
  Boolean        flagError;
  XOSPtr         xosp;
  XISPtr         xisp;

  BioseqPtr      bspT, bspP;
  SeqLocPtr      slpT, slpP;
  BLAST_OptionsBlkPtr blstopt;

  Int4           i, loop, cutoff;
  Int4           startT, startP, stopT, stopP;
  SeqAlignPtr    sap, sapn, sapkeep;
  DenseSegPtr    dsp;
  SeqPortPtr     sppT, sppP;
  Uint1          seqT, seqP;
  Int4           effective_length, eff_len_max, seq_identity, seq_id_max;
  Char           infobuf[512];
  CharPtr        titleT, titleP;
  CharPtr        cptr;
  Int4           keepAlign, keepAlignCount;

  CharPtr        pattern_file = "ncbipros.dat";
  ComPatPtr      cpp, cpph;
  SeqAlignPtr    sapT, sapP, sapTt, sapPt, sappattern, sapnew;
  SeqLocPtr      slp;
  StdSegPtr      ssp;
  Int4           lenT, lenP, countT, countP;
  Int4Ptr        starts, lens;
  Boolean        flagID;
  ValNodePtr     vnphead, vnpname;

  WindoW      w;
  VieweR      v;
  GrouP       g;
  SegmenT     seg;
  Int4        top;

  flagError = FALSE;
  if ((xosp = (XOSPtr) GetObjectExtra (b)) == NULL)
    return;

  bspT = xosp->bspT;
  bspP = xosp->bspP;
  cutoff = xosp->dotcutoff;

  if (bspT == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 101, "No Target Bioseq");
    ErrShow ();
    flagError = TRUE;
  }
  if (bspP == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 101, "No Probe Bioseq");
    ErrShow ();
    flagError = TRUE;
  }
  if (flagError)
    return;

  if (!ISA_aa (bspT->mol))
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 101, "Target not amino acid Bioseq");
    ErrShow ();
    flagError = TRUE;
  }
  if (!ISA_aa (bspP->mol))
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 101, "Probe not amino acid Bioseq");
    ErrShow ();
    flagError = TRUE;
  }
  if (flagError)
    return;

  WatchCursor ();
  if (xosp->flagPattern)
    cpph = ReadPrositePattern (pattern_file, FALSE, 25, NULL, NULL);
  else
    cpph = NULL;

  blstopt = BLASTOptionNew ("blastp", TRUE);
  blstopt->wordsize = 2;
  blstopt->expect_value = 10.0;
  blstopt->filter = 0;
  blstopt->gap_open = 9;
  blstopt->gap_extend = 2;
  blstopt->gap_x_dropoff = 50;
  blstopt->threshold_second = 9;
  blstopt->two_pass_method = FALSE;
  blstopt->multiple_hits_only = FALSE;

  slpT = SeqLocIntNew (0, bspT->length-1, Seq_strand_unknown, bspT->id);
  slpP = SeqLocIntNew (0, bspP->length-1, Seq_strand_unknown, bspP->id);

  sapn = sap = BlastTwoSequencesByLoc (slpT, slpP, "blastp", blstopt);

  BLASTOptionDelete (blstopt);

  seq_id_max = 0;
  eff_len_max = 0;
  keepAlign = keepAlignCount = 0;
  while (sap != NULL)
  {
    keepAlignCount++;
    effective_length = 0;
    seq_identity = 0;
    if (sap->segtype == 2)   /* DenseSeg gapped search */
    {
      sppT = SeqPortNew (bspT, 0, bspT->length-1, 0, Seq_code_iupacaa);
      sppP = SeqPortNew (bspP, 0, bspP->length-1, 0, Seq_code_iupacaa);
      dsp = sap->segs;
      for (loop = 0; loop < (Int4) dsp->numseg; loop++)
      {
        if (dsp->starts[loop*2] == -1 || dsp->starts[(loop*2)+1] == -1)
        {
          effective_length += dsp->lens[loop];
        }
        else
        {
          effective_length += dsp->lens[loop];
          startT = dsp->starts[loop*2];
          startP = dsp->starts[(loop*2)+1];
          SeqPortSeek (sppT, startT, SEEK_SET);
          SeqPortSeek (sppP, startP, SEEK_SET);
          for (i = 0; i < dsp->lens[loop]; i++)
          {
            seqT = SeqPortGetResidue (sppT);
            seqP = SeqPortGetResidue (sppP);
            if (seqT == seqP)
              seq_identity++;
          }
        }
      }
      SeqPortFree (sppT);
      SeqPortFree (sppP);
    }
    if (effective_length > eff_len_max)
    {
      seq_id_max = seq_identity;
      eff_len_max = effective_length;
      keepAlign = keepAlignCount;
    }
    sap = sap->next;
  }
  sap = sapn;
  sapkeep = NULL;
  keepAlignCount = 0;
  while (sap != NULL)
  {
    keepAlignCount++;
    sapn = sap->next;
    if (keepAlign == keepAlignCount)
    {
      sapkeep = sap;
      sapkeep->next = NULL;
    }
    else
    {
      SeqAlignFree (sap);
    }
    sap = sapn;
  }

  if (bspT->length == bspP->length && bspT->length == seq_identity)
    flagID = TRUE;
  else
    flagID = FALSE;

  vnphead = NULL;
  sappattern = NULL;
  cpp = cpph;
  while (cpp != NULL)
  {
    sapT = PatternMatchBioseq (bspT, cpp, 0);
    sapP = PatternMatchBioseq (bspP, cpp, 0);
    if (sapT != NULL && sapP != NULL)
    {
      vnpname = ValNodeNew (vnphead);
      if (vnphead == NULL)
        vnphead = vnpname;
      vnpname->data.ptrvalue = (Pointer) StringSave (cpp->name);
      if ((sapnew = SeqAlignNew ()) != NULL)
      {
        sapnew->type = 3;
        sapnew->segtype = 2;
        sapnew->dim = 2;
        dsp = DenseSegNew ();
        sapnew->segs = dsp;
        countT = SapCount (sapT);
        countP = SapCount (sapP);
        if (flagID)
          dsp->numseg = (Int2) (countT * countP * 2);
        else
          dsp->numseg = (Int2) (countT * countP);
        dsp->starts = (Int4Ptr) MemNew (sizeof (Int4) * dsp->numseg * 2);
        dsp->lens = (Int4Ptr) MemNew (sizeof (Int4) * dsp->numseg);
        starts = dsp->starts;
        lens = dsp->lens;
        sapTt = sapT;
        sapPt = sapP;
        while (sapTt != NULL)
        {
          ssp = (StdSegPtr) sapTt->segs;
          slp = ssp->loc;
          startT = SeqLocStart (slp);
          lenT = SeqLocLen (slp);
          while (sapPt != NULL)
          {
            ssp = (StdSegPtr) sapPt->segs;
            slp = ssp->loc;
            startP = SeqLocStart (slp);
            lenP = SeqLocLen (slp);
            *starts = startT;
            starts++;
            *starts = startP;
            starts++;
            *lens = (lenT+lenP)/2;
            lens++;
            if (flagID)
            {
              *starts = startP;
              starts++;
              *starts = startT;
              starts++;
              *lens = (lenT+lenP)/2;
              lens++;
            }
            sapPt = sapPt->next;
          }
          sapTt = sapTt->next;
        }
      }
      if (sappattern == NULL)
      {
        sappattern = sapnew;
      }
      else
      {
        sap = sappattern;
        while (sap->next != NULL)
          sap = sap->next;
        sap->next = sapnew;
      }
    }
    sap = sapT;
    while (sap != NULL)
    {
      sapn = sap->next;
      SeqAlignFree (sap);
      sap = sapn;
    }
    sap = sapP;
    while (sap != NULL)
    {
      sapn = sap->next;
      SeqAlignFree (sap);
      sap = sapn;
    }
    cpp = cpp->nextpattern;
  }

  titleT = FastaTitle (bspT, ">", NULL);
  titleP = FastaTitle (bspP, ">", NULL);

  cptr = titleT;
  while (*cptr != '\0')
  {
    if (*cptr == ' ')
      break;
    cptr++;
  }
  *cptr = '\0';
  if (StrLen (titleT) > 16)
    titleT[16] = '\0';
  cptr = titleP;
  while (*cptr != '\0')
  {
    if (*cptr == ' ')
      break;
    cptr++;
  }
  *cptr = '\0';
  if (StrLen (titleP) > 16)
    titleP[16] = '\0';

  xisp = (XISPtr) MemNew (sizeof (XIS));
  startT = SeqLocStart (slpT);
  stopT = SeqLocStop (slpT);
  startP = SeqLocStart (slpP);
  stopP = SeqLocStart (slpP);
  xisp->Xscale = ScalePlot (stopT - startT + 1, stopP - startP + 1, 400);
  xisp->Yscale = xisp->Xscale;

  seg = NULL;
  seg = DotXYPlot (bspT, bspP, cutoff, xisp->Xscale, xisp->Yscale, seg, FALSE);
  seg = XYPlot (bspT, bspP, sapkeep, xisp->Xscale, xisp->Yscale,
                BLUE_COLOR, seg, TRUE);
  seg = XYPlot (bspT, bspP, sappattern, xisp->Xscale, xisp->Yscale,
                GREEN_COLOR, seg, TRUE);
  if (seg == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 501, "Failed to create picture");
    ErrShow ();
    ArrowCursor ();
    return;
  }
  xisp->picture = seg;

  vnpname = vnphead;
  if (vnpname != NULL)
  {
    sap = sappattern;
    while (sap != NULL)
    {
      if (vnpname != NULL)
      {
        dsp = (DenseSegPtr) sap->segs;
        AddLabel (seg, 25 + (dsp->starts[0]/xisp->Xscale),
                       390 - (dsp->starts[1]/xisp->Yscale),
                  (CharPtr) vnpname->data.ptrvalue,
                  SMALL_TEXT, 5, LOWER_RIGHT, 0);
        vnpname = vnpname->next;
      }
      sap = sap->next;
    }
  }
  ValNodeFreeData (vnphead);

  if (eff_len_max > 0)
  {
    sprintf (infobuf,
     "X:TwoP%s %ld - Y:TwoP%s %ld - %6.2f identity - %ld residues",
     titleT, (long) bspT->length, titleP, (long) bspP->length,
     (FloatHi) seq_id_max / (FloatHi) eff_len_max * 100.0,
     (long) eff_len_max);
  }
  else
  {
    sprintf (infobuf,
     "X:TwoP%s %ld - Y:TwoP%s %ld - %6.2f identity",
     titleT, (long) bspT->length, titleP, (long) bspP->length, 0.0);
  }
  top = -10;
  AddLabel (seg, 0, top, infobuf, SMALL_TEXT, 5, MIDDLE_RIGHT, 0);

  w = FixedWindow (10, 10, -10, -10, "2 Proteins", CloseGraphWindowProc);
  SetObjectExtra (w, xisp, CleanUpGraphWindow);
  g = HiddenGroup (w, -1, 0, NULL);
  v = CreateViewer (g, 600, 500, TRUE, TRUE);
  AttachPicture (v, seg, INT4_MIN, INT4_MAX, UPPER_LEFT, 1, 1, NULL);
  PushButton (g, "Close", CloseGraphWindowButton);
  RealizeWindow (w);
  Show (w);

  ComPatFree (cpph);
  sap = sappattern;
  while (sap != NULL)
  {
    sapn = sap->next;
    SeqAlignFree (sap);
    sap = sapn;
  }
  SeqAlignFree (sapkeep);
  SeqLocFree (slpT);
  SeqLocFree (slpP);
  MemFree (titleT);
  MemFree (titleP);

  ArrowCursor ();
  return;
}

/* button 7 */

static void CloseTwopParentProc (WindoW w)
{
  XOSPtr  xosp;

  if ((xosp = (XOSPtr) GetObjectExtra (w)) != NULL)
  {
    xosp->gsp  = NULL;          /* is static */
    MemFree (xosp->filenameT);
    MemFree (xosp->filenameP);
    xosp->sepT  = SeqEntryFree (xosp->sepT);
    xosp->sepP  = SeqEntryFree (xosp->sepP);
    xosp->bspT  = NULL;         /* should be in seqentry */
    xosp->bspP  = NULL;         /* should be in seqentry */
    MemFree (xosp);
  }
  Remove (w);
  QuitProgram ();
  return;
}

static void EndProg (ButtoN b)
{
  CloseTwopParentProc (ParentWindow (b));
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
  xosp->gpostblast = NULL;
  xosp->flagPattern = FALSE;
  xosp->bTG = NULL;
  xosp->bPG = NULL;
  xosp->bTF = NULL;
  xosp->bPF = NULL;
  xosp->bThis = NULL;
  xosp->giT = 0;
  xosp->giP = 0;
  xosp->filenameT = NULL;
  xosp->filenameP = NULL;
  xosp->gsp = gsp;
  xosp->sepT = NULL;
  xosp->sepP = NULL;
  xosp->bspT = NULL;
  xosp->bspP = NULL;
  xosp->dotcutoff = 6;
  xosp->dotcutoffmin = 0;
  xosp->dotcutoffmax = 8;

  return xosp;
}

Int2 Main (void)
{
  WindoW  w;
  GrouP   g;
  ButtoN  b1, b2, b3, b4, b5, b6, b7;

  XOSPtr  xosp;

  ProcessUpdatesFirst (FALSE);

  xosp = SetUp ();

  w = FixedWindow (-50, -33, -10, -10,
                   "Two Protein Blast", CloseTwopParentProc);
  g = HiddenGroup (w, 7, 0, NULL);
  b1 = PushButton (g, "  Target GI ", ReadGIProc);
  b2 = PushButton (g, "   Probe GI ", ReadGIProc);
  b3 = PushButton (g, "Target FastA", ReadFileProc);
  b4 = PushButton (g, " Probe FastA", ReadFileProc);
  b5 = PushButton (g, " Parameters ", TwopParamProc);
  b6 = PushButton (g, "Blast 2 Prot", TwopProc);
  b7 = PushButton (g, "   Finish   ", EndProg);

  xosp->bTG = b1;
  xosp->bPG = b2;
  xosp->bTF = b3;
  xosp->bPF = b4;

  SetObjectExtra (w,  xosp, NULL);
  SetObjectExtra (b1, xosp, NULL);
  SetObjectExtra (b2, xosp, NULL);
  SetObjectExtra (b3, xosp, NULL);
  SetObjectExtra (b4, xosp, NULL);
  SetObjectExtra (b5, xosp, NULL);
  SetObjectExtra (b6, xosp, NULL);
  SetObjectExtra (b7, xosp, NULL);

  Show (w);
  ProcessEvents ();
  return 0;
}
