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
* File Name: cnsgnv.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.10 $
*
* File Description: consign - codon biased orf assignment
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: cnsgnv.c,v $
* Revision 6.10  1999/03/13 03:43:16  kuzio
* cast
*
* Revision 6.9  1998/10/13 17:15:26  kuzio
* colored graph opt
*
* Revision 6.8  1998/10/01 16:43:54  kuzio
* rewrite framing
*
* Revision 6.7  1998/09/30 19:09:39  kuzio
* more strand frame fixes
*
* Revision 6.6  1998/09/28 16:42:20  kuzio
* no met orf check
*
* Revision 6.5  1998/09/22 02:29:40  kuzio
* orf find patch; frameshift fix
*
* Revision 6.4  1998/09/16 18:40:34  kuzio
* cvs logging
*
* ==========================================================================
*/

#include <ncbi.h>
#include <vibrant.h>
#include <accentr.h>
#include <gather.h>
#include <tofasta.h>
#include <toasn3.h>
#include <tfuns.h>
#include <urkutil.h>
#include <urkbias.h>
#include <urkcnsrt.h>
#include <urkptpf.h>
#include <seqscrl.h>
#include <gphdraw.h>

#define TOP_ERROR 1
static char _this_module[] = "cnsgnv";
#undef  THIS_MODULE
#define THIS_MODULE _this_module
static char _this_file[] = __FILE__;
#undef  THIS_FILE
#define THIS_FILE _this_file

Uint1 AAForCodon (Uint1Ptr cdn, CharPtr gcode);

typedef struct xtrins
{
  SegmenT        picture;
  SeqGraphPtr    sgp;
} XIS, PNTR XISPtr;

typedef struct xtrobj
{
  Boolean        flagParamWindow;
  Int4           gi;
  CharPtr        filename;
  GatherScopePtr gsp;
  SeqEntryPtr    sep;
  BioseqPtr      bsp;
  CharPtr        pattern_file;
  CharPtr        gcd, gcdi;
  Int4           orfcut;
  Int4           cutoff, cutoffmin, cutoffmax;
  Int4           window, windowmin, windowmax;
  ValNodePtr     orflist;
  SeqLocPtr      slpa, slpb, slps, slpk;
  FloatHi        probcut;
  Int4           clustmin;
  Int4           findmin;
  Int4           minimumseed;
} XOS, PNTR XOSPtr;

/* common */

static void CleanUpXOS (XOSPtr xosp)
{
  SeqLocPtr slp, slpn;
  SeqIdPtr  id;

  if (xosp != NULL)
  {
    xosp->gsp  = NULL;          /* is static */
    xosp->filename = (CharPtr) MemFree (xosp->filename);
    xosp->sep = SeqEntryFree (xosp->sep);
    xosp->bsp = NULL;           /* should be in seqentry */
    xosp->gcd = NULL;
    xosp->gcdi = NULL;
    slp = xosp->slpa;
    while (slp != NULL)
    {
      slpn = slp->next;
      id = SeqLocId (slp);
      if (id != NULL)
        id->next = SeqIdSetFree (id->next);
      SeqLocFree (slp);
      slp = slpn;
    }
    xosp->slpa = slp;
    slp = xosp->slpb;
    while (slp != NULL)
    {
      slpn = slp->next;
      id = SeqLocId (slp);
      if (id != NULL)
        id->next = SeqIdSetFree (id->next);
      SeqLocFree (slp);
      slp = slpn;
    }
    xosp->slpb = slp;
    slp = xosp->slps;
    while (slp != NULL)
    {
      slpn = slp->next;
      id = SeqLocId (slp);
      if (id != NULL)
        id->next = SeqIdSetFree (id->next);
      SeqLocFree (slp);
      slp = slpn;
    }
    xosp->slps = slp;
    slp = xosp->slpk;
    while (slp != NULL)
    {
      slpn = slp->next;
      id = SeqLocId (slp);
      if (id != NULL)
        id->next = SeqIdSetFree (id->next);
      SeqLocFree (slp);
      slp = slpn;
    }
    xosp->slpk = slp;
  }
  return;
}

static Boolean GatherKnownOrfs (GatherContextPtr gcp)
{
  XOSPtr      xosp;
  SeqAnnotPtr sap;
  SeqFeatPtr  sfp;
  SeqLocPtr   slp;

  if (gcp == NULL)
    return FALSE;
  if ((xosp = (XOSPtr) gcp->userdata) == NULL)
    return FALSE;
  if (gcp->thistype != OBJ_SEQANNOT)
    return TRUE;
  if ((sap = (SeqAnnotPtr) (gcp->thisitem)) == NULL)
    return TRUE;

  while (sap != NULL)
  {
    sfp = (SeqFeatPtr) sap->data;
    while (sfp != NULL)
    {
      if (sfp->data.choice == SEQFEAT_CDREGION)
      {
        slp = sfp->location;
        SeqLocLink (&(xosp->slpk), SeqLocDup (slp));
      }
      sfp = sfp->next;
    }
    sap = sap->next;
  }
  return TRUE;
}

static void AddBioSourceToSeqEntry (SeqEntryPtr sep)
{
  BioseqPtr    bsp;
  ValNodePtr   descorp;
  BioSourcePtr biop;
  OrgNamePtr   onp;
  OrgRefPtr    orp;

  if (sep == NULL)
    return;
  if ((bsp = sep->data.ptrvalue) == NULL)
    return;

  descorp = ValNodeNew (bsp->descr);
  descorp->choice = Seq_descr_source;
  biop = BioSourceNew ();
  descorp->data.ptrvalue = biop;
  orp = OrgRefNew ();
  biop->org = orp;
  onp = OrgNameNew ();
  orp->orgname = onp;
  biop->genome = 1;
  onp->gcode = 1;
  onp->mgcode = 1;

  if (bsp->descr == NULL)
    bsp->descr = descorp;

  return;
}

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
  CharPtr         filename;
  Int4            gi;
  FILE            *fiop;
  Boolean         flagHaveNet;
  SeqEntryPtr     sep;
  ValNodePtr      vnp;
  Int2            gcode;
  GeneticCodePtr  gcp;
  SeqLocPtr       slp, slpn;
  SeqIdPtr        id;

  gsp = xosp->gsp;

  fiop = NULL;
  filename = xosp->filename;
  gi = xosp->gi;
  xosp->sep = SeqEntryFree (xosp->sep);

  if (gi > 0)
  {
    if (!EntrezInit ("cnsgnv", FALSE, &flagHaveNet))
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 102,
                 "Entrez init failed");
      ErrShow ();
      return;
    }
  }

  if (gi > 0)
  {
    sep = EntrezSeqEntryGet (gi, SEQENTRY_READ_NUC_PROT);
  }
  else if (filename != NULL)
  {
    if ((fiop = FileOpen (filename, "r")) == NULL)
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 103,
                 "Failed to open FastA file");
      ErrShow ();
      return;
    }
    sep = FastaToSeqEntry (fiop, TRUE);
    AddBioSourceToSeqEntry (sep);
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
    CleanUpXOS (xosp);
    xosp->sep = sep;
    xosp->gi = gi;
    xosp->filename = StringSave (filename);
    GatherSeqEntry (sep, (Pointer) xosp, GetBioseq, (Pointer) gsp);
  }

  if (xosp->bsp != NULL)
  {
    vnp = xosp->bsp->descr;
    gcode = 0;
    while (vnp != NULL)
    {
      if (vnp->choice == Seq_descr_source)
      {
        gcode = BioSourceToGeneticCode ((BioSourcePtr) vnp->data.ptrvalue);
        break;
      }
      vnp = vnp->next;
    }
    if (gcode == 0 && sep->choice == 2)
    {
      vnp = ((BioseqSetPtr) (sep->data.ptrvalue))->descr;
      while (vnp != NULL)
      {
        if (vnp->choice == Seq_descr_source)
        {
          gcode = BioSourceToGeneticCode ((BioSourcePtr) vnp->data.ptrvalue);
          break;
        }
        vnp = vnp->next;
      }
    }
    if (gcode == 0)
      gcode = 1; /* standard */
    gcp = GeneticCodeFind (gcode, NULL);
    if (gcp != NULL)
    {
      xosp->gcd = xosp->gcdi = NULL;
      vnp = (ValNodePtr) gcp->data.ptrvalue;
      while (vnp != NULL)
      {
        if (vnp->choice == 6)       /* sncbieaa */
          xosp->gcdi = (CharPtr) vnp->data.ptrvalue;
        else if (vnp->choice == 3)  /* ncbieaa */
          xosp->gcd = (CharPtr) vnp->data.ptrvalue;
        vnp = vnp->next;
      }
    }
    if (xosp->gcdi == NULL)
      xosp->gcdi = xosp->gcd;
    if (xosp->gcdi == NULL)
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 105,
                 "Could not get genetic code for translation");
      ErrShow ();
      xosp->bsp = NULL;
    }
  }

  if (gi > 0)
  {
    slp = xosp->slpk;
    while (slp != NULL)
    {
      slpn = slp->next;
      id = SeqLocId (slp);
      if (id != NULL)
        id->next = SeqIdSetFree (id->next);
      SeqLocFree (slp);
      slp = slpn;
    }
    xosp->slpk = slp;
    GatherSeqEntry (sep, (Pointer) xosp, GatherKnownOrfs, (Pointer) gsp);
  }

  if (gi > 0)
    EntrezFini ();
  else
    FileClose (fiop);

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
      xosp->cutoff = (Int4) ssdp->curval;
      ssdp = ssdp->next;
      xosp->window = (Int4) ssdp->curval;
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

static void ConsignParamProc (ButtoN b)
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
                   "Consign Parameters", CloseParamWindowProc);
  h = HiddenGroup (w, 1, 0, NULL);
  h1 = HiddenGroup (h, 2, 0, NULL);

  g1 = HiddenGroup (h1, 1, 0, NULL);
  g = HiddenGroup (g1, 1, 0, NULL);
  StaticPrompt (g, "CU cut", StringWidth ("12345678"), dialogTextHeight,
                NULL, 'c');
  g = HiddenGroup (g1, 2, 0, NULL);
  AddScrollControl (&ssdp, g, (Int2) xosp->cutoff,
                    (Int2) xosp->cutoffmin, (Int2) xosp->cutoffmax, 24);

  g1 = HiddenGroup (h1, 1, 0, NULL);
  g = HiddenGroup (g1, 1, 0, NULL);
  StaticPrompt (g, "CU win", StringWidth ("12345678"), dialogTextHeight,
                NULL, 'c');
  g = HiddenGroup (g1, 2, 0, NULL);
  AddScrollControl (&ssdp, g, (Int2) xosp->window,
                    (Int2) xosp->windowmin, (Int2) xosp->windowmax, 24);
  SetObjectExtra (w, ssdp, NULL);

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
  XISPtr      xisp;
  SeqGraphPtr sgp, sgpn;

  if ((xisp = (XISPtr) data) != NULL)
  {
    DeletePicture (xisp->picture);
    xisp->picture = NULL;
    sgp = xisp->sgp;
    while (sgp != NULL)
    {
      sgpn = sgp->next;
      SeqGraphFree (sgp);
      sgp = sgpn;
    }
    xisp->sgp = NULL;
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

static void AddOrfClass (SeqLocPtr slp, SegmenT seg,
                         Int4Ptr orftop, Int4 iframe,
                         Int4 frame, Int4 shift, Uint1 strand,
                         Uint1Ptr orfcolor, Uint1 orfwidth)
{
  SeqLocPtr slpThis, slpHead;
  Uint1     strandThis;
  Int4      start, stop, startorig, startthis, stopthis;
  Int4      len, runlen, extraframe;
  Int4      top, topold, startold, stopold;
  Int4      framethis;
  Boolean   flagRTL;

  while (slp != NULL)
  {
    if (slp->choice == SEQLOC_MIX)
      slpThis = (SeqLocPtr) slp->data.ptrvalue;
    else
      slpThis = slp;
    strandThis = SeqLocStrand (slpThis);
    if (strandThis == strand ||
        (strandThis == Seq_strand_unknown && strand == Seq_strand_plus))
    {
      flagRTL = TRUE;
      runlen = 0;
      startold = -1;
      stopold = -1;
      topold = -1;
      slpHead = slpThis;
      if (strandThis != Seq_strand_plus &&
          strandThis != Seq_strand_unknown)
      {
        if (slp->choice == SEQLOC_MIX)
        {
          if (slpHead->next != NULL)
          {
            if (SeqLocStart (slpHead->next) > SeqLocStart (slpHead))
            {
              flagRTL = FALSE;
              while (slpHead->next != NULL)
                slpHead = slpHead->next;
            }
          }
        }
      }
      if (strandThis != Seq_strand_plus &&
          strandThis != Seq_strand_unknown)
        startorig = SeqLocStop (slpHead) - 2;
      else
        startorig = SeqLocStart (slpHead);
      if (startorig > 0 && startorig%3 == frame)
      {
        while (slpThis != NULL)
        {
          start = SeqLocStart (slpThis);
          stop = SeqLocStop (slpThis);
          if (strandThis != Seq_strand_plus &&
              strandThis != Seq_strand_unknown)
          {
            startthis = SeqLocStop (slpThis) - 2;
            stopthis = SeqLocStart (slpThis) - 2;
          }
          else
          {
            startthis = SeqLocStart (slpThis);
            stopthis = SeqLocStop (slpThis);
          }
          framethis = startthis % 3;
          extraframe = runlen % 3;
          framethis += extraframe;
          framethis %= 3;
          if (iframe > 2)
            framethis = 3 + framethis;
          top = orftop[framethis] - shift;

          len = stopthis - startthis;
          if (len < 0)
            len *= -1;
          len += 1;
          runlen += len;

          if (stopold >= 0)
          {
            AddAttribute (seg, (COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT),
                          orfcolor, SOLID_LINE, SOLID_SHADING,
                          STD_PEN_WIDTH, 0);
            if (strand == Seq_strand_minus && flagRTL)
              AddLine (seg, startold, topold, stop, top, FALSE, 0);
            else
              AddLine (seg, stopold, topold, start, top, FALSE, 0);
          }
          AddAttribute (seg, (COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT),
                        orfcolor, SOLID_LINE, SOLID_SHADING,
                        orfwidth, 0);
          AddLine (seg, start, top, stop, top, FALSE, 0);
          AddAttribute (seg, (COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT),
                        BLACK_COLOR, SOLID_LINE, SOLID_SHADING,
                        STD_PEN_WIDTH, 0);
          startold = start;
          stopold = stop;
          topold = top;
          if (slp->choice != SEQLOC_MIX)
            break;
          slpThis = slpThis->next;
        }
      }
    }
    slp = slp->next;
  }
  return;
}

static void ConsignProc (ButtoN b)
{
  XOSPtr         xosp;
  XISPtr         xisp;

  ComPatPtr      cpp, cpph;
  ValNodePtr     orflist;
  SeqLocPtr      slp, slpn;
  Int4           start, stop;
  Uint1          strand;
  SeqPortPtr     spp;
  Uint1Ptr       aaseq;
  Int4           ntpos, aapos;
  Uint1          cdn[3];

  SeqAlignPtr    sap, sapn;

  FloatHi        probcut;
  Int4           clustmin, findmin;

  Int4           i, n, endpos, XLength, XScale, shift;
  Int4           iframe, frame, top, orftop[6];
  FloatHiPtr     score, expandscore;
  FloatHi        maxscore;
  Int4Ptr        tableGlobal;

  SeqGraphPtr  sgp, sgpn;
  WindoW       w;
  VieweR       v;
  GrouP        g;
  SegmenT      seg;
  GraphSentPtr gsp;
  Char         numberbuffer[32];

  if ((xosp = (XOSPtr) GetObjectExtra (b)) == NULL)
    return;

  if (xosp->bsp == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 101, "No Bioseq");
    ErrShow ();
    return;
  }

  WatchCursor ();
  cpph = cpp = ReadPrositePattern (xosp->pattern_file, TRUE, -1, NULL, NULL);
  if (cpph == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 101,
               "read failed %s", xosp->pattern_file);
    ErrShow ();
    ArrowCursor ();
    return;
  }

  xosp->orflist =  GetOrfList (xosp->bsp, (Int2) (xosp->orfcut));
  xosp->orflist =  ClearNonMetOrfs (xosp->orflist);
  orflist = xosp->orflist;
  while (orflist != NULL)
  {
    slp = (SeqLocPtr) orflist->data.ptrvalue;
    if (slp->choice == 0)
    {
      orflist = orflist->next;
      continue;
    }
    if (slp->choice == SEQLOC_MIX)
      slp = (SeqLocPtr) slp->data.ptrvalue;
    start = SeqLocStart (slp);
    stop = SeqLocStop (slp);
    strand = SeqLocStrand (slp);
    if (strand != Seq_strand_both)
      strand = Seq_strand_both;
    if (stop - start + 1 >= xosp->minimumseed)
    {
      spp = SeqPortNew (xosp->bsp, start, stop, strand, Seq_code_ncbi4na);
      aaseq = (Uint1Ptr) MemNew ((size_t)
                                 (sizeof (Uint1) * (((stop-start)/3)+2)));
      ntpos = start;
      aapos = 0;
      while (ntpos < start+3)
      {
        cdn[0] = SeqPortGetResidue (spp);
        ntpos++;
        cdn[1] = SeqPortGetResidue (spp);
        ntpos++;
        cdn[2] = SeqPortGetResidue (spp);
        ntpos++;
        aaseq[aapos] = AAForCodon (cdn, xosp->gcdi);
        aapos++;
      }
      while (ntpos <= stop)
      {
        cdn[0] = SeqPortGetResidue (spp);
        ntpos++;
        cdn[1] = SeqPortGetResidue (spp);
        ntpos++;
        cdn[2] = SeqPortGetResidue (spp);
        ntpos++;
        aaseq[aapos] = AAForCodon (cdn, xosp->gcd);
        aapos++;
      }
      SeqPortFree (spp);
      aaseq[aapos] = 0;
      cpp = cpph;
      while (cpp != NULL)
      {
        sap = PatternMatch (aaseq, 0, Seq_strand_plus, SeqLocId (slp),
                            cpp, 0, Seq_strand_unknown, FALSE);
        if (sap != NULL)
          break;
        cpp = cpp->nextpattern;
      }
      MemFree (aaseq);
      if (sap != NULL)
      {
        SeqLocLink (&(xosp->slps), SeqLocDup (slp));
      }
      while (sap != NULL)
      {
        sapn = sap->next;
        SeqAlignFree (sap);
        sap = sapn;
      }
    }
    orflist = orflist->next;
  }
  ComPatFree (cpph);

  orflist = xosp->orflist;
  while (orflist != NULL)
  {
    slp = (SeqLocPtr) orflist->data.ptrvalue;
    if (slp->choice > 0)
      SeqLocLink (&(xosp->slpa), SeqLocDup (slp));
    while (slp != NULL)
    {
      slpn = slp->next;
      SeqLocFree (slp);
      slp = slpn;
    }
    orflist->data.ptrvalue = NULL;
    orflist = orflist->next;
  }
  xosp->orflist = ValNodeFree (xosp->orflist);

  probcut = xosp->probcut;
  clustmin = xosp->clustmin;
  findmin = xosp->findmin;

  xosp->slpb = FindSimilarBiasOrfs (xosp->sep, probcut, clustmin, findmin,
                                    xosp->slps, xosp->slpa);

  tableGlobal = CodonTableFromSeqLoc (xosp->bsp, xosp->slpb);
  seg = NULL;
  top = 0;
  xisp = (XISPtr) MemNew (sizeof (XIS));
  frame = 0;
  for (iframe = 0; iframe < 6; iframe++)
  {
    endpos = (xosp->bsp->length + 3 - frame - xosp->window) / 3;
    if (iframe < 3)
      score = BiasScoreBioseq (xosp->bsp, tableGlobal, xosp->window,
                               frame, Seq_strand_plus);
    else
      score = BiasScoreBioseq (xosp->bsp, tableGlobal, xosp->window,
                               frame, Seq_strand_minus);
    maxscore = 0.0;
    for (i = 0; i < endpos; i++)
      if (score[i] > maxscore)
        maxscore = score[i];
    expandscore = (FloatHiPtr) MemNew (sizeof (FloatHi) * xosp->bsp->length);
    for (i = 0; i < xosp->window/2; i++)
      expandscore[i] = maxscore;
    n = 0;
    while (i < xosp->bsp->length)
    {
      if (n < endpos)
        expandscore[i] = score[n];
      else
        expandscore[i] = maxscore;
      i++;
      if (i%3 == 0)
        n++;
    }
    MemFree (score);
    score = expandscore;
    sgp = SeqGraphNew ();
    if (xisp->sgp == NULL)
    {
      xisp->sgp = sgp;
    }
    else
    {
      sgpn = xisp->sgp;
      while (sgpn->next != NULL)
        sgpn = sgpn->next;
      sgpn->next = sgp;
    }
    XLength = xosp->bsp->length;
    if (XLength > 1200)
      XLength = 1200;
    XScale = xosp->bsp->length / XLength;
    if (xosp->bsp->length % XLength != 0)
      XScale++;
    sgp->loc = SeqLocIntNew (0, xosp->bsp->length-1, xosp->bsp->strand,
                             xosp->bsp->id);
    sgp->flags[2] = 1;
    sgp->numval = xosp->bsp->length;
    sgp->values = (Pointer) score;
    sgp->max.realvalue = maxscore;
    sgp->min.realvalue = 0.0;
    sgp->flags[1] = 1;
    sgp->a = 4.0;
    sgp->b = 0.0;
    if (seg == NULL)
      seg = CreatePicture ();
    if ((gsp = AddGraphSentinelToPicture (sgp, xosp->bsp, seg, 0,
                                          top, 0, NULL)) != NULL)
    {
      sprintf (numberbuffer, "%ld", 1L);
      AddLabel (seg, gsp->box.left, gsp->bottom-20,
                numberbuffer, SMALL_TEXT, 0, MIDDLE_CENTER, 0);
      sprintf (numberbuffer, "%ld", (long) xosp->bsp->length);
      AddLabel (seg, gsp->box.left+xosp->bsp->length, gsp->bottom-20,
                numberbuffer, SMALL_TEXT, 0, MIDDLE_CENTER, 0);
    }
    shift = (Int4) (maxscore*sgp->a);
    orftop[iframe] = top - shift - 38;
    top -= (shift+56);
    frame++;
    if (frame == 3)
    {
      top -= 24;
      frame = 0;
    }
  }
  frame = 0;
  for (iframe = 0; iframe < 6; iframe++)
  {
    if (iframe < 3)
      strand = Seq_strand_plus;
    else
      strand = Seq_strand_minus;
    shift = 0;
    if (xosp->slpa != NULL)
    {
      AddOrfClass (xosp->slpa, seg, orftop, iframe, frame,
                   shift, strand, YELLOW_COLOR, 5);
      shift += 4;
    }
    if (xosp->slpk != NULL)
    {
      AddOrfClass (xosp->slpk, seg, orftop, iframe, frame,
                   shift, strand, GREEN_COLOR, 5);
      shift += 4;
    }
    if (xosp->slpb != NULL)
    {
      AddOrfClass (xosp->slpb, seg, orftop, iframe, frame,
                   shift, strand, BLUE_COLOR, 5);
      shift += 4;
    }
    if (xosp->slps != NULL)
    {
      AddOrfClass (xosp->slps, seg, orftop, iframe, frame,
                   shift, strand, RED_COLOR, 5);
    }
    frame++;
    if (frame == 3)
      frame = 0;
  }
  MemFree (tableGlobal);

  start = 20;
  stop = 20 + (50*XScale);
  top = orftop[5] - 40;
  if (xosp->slpa != NULL)
  {
    top -= 12;
    AddAttribute (seg, (COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT),
                  YELLOW_COLOR, SOLID_LINE, SOLID_SHADING, 5,
                  0);
    AddLine (seg, start, top, stop, top, FALSE, 0);
    AddAttribute (seg, (COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT),
                  BLACK_COLOR, SOLID_LINE, SOLID_SHADING, STD_PEN_WIDTH,
                  0);
    AddLabel (seg, stop+(20*XScale), top,
              "All Met-init'd ORFs equal to or greater than 50 codons",
              SMALL_TEXT, 0, MIDDLE_RIGHT, 0);
  }
  if (xosp->slpk != NULL)
  {
    top -= 12;
    AddAttribute (seg, (COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT),
                  GREEN_COLOR, SOLID_LINE, SOLID_SHADING, 5,
                  0);
    AddLine (seg, start, top, stop, top, FALSE, 0);
    AddAttribute (seg, (COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT),
                  BLACK_COLOR, SOLID_LINE, SOLID_SHADING, STD_PEN_WIDTH,
                  0);
    AddLabel (seg, stop+(20*XScale), top, "Annotated (reported) ORFs",
              SMALL_TEXT, 0, MIDDLE_RIGHT, 0);
  }
  if (xosp->slpb != NULL)
  {
    top -= 12;
    AddAttribute (seg, (COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT),
                  BLUE_COLOR, SOLID_LINE, SOLID_SHADING, 5,
                  0);
    AddLine (seg, start, top, stop, top, FALSE, 0);
    AddAttribute (seg, (COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT),
                  BLACK_COLOR, SOLID_LINE, SOLID_SHADING, STD_PEN_WIDTH,
                  0);
    AddLabel (seg, stop+(20*XScale), top, "Similar codon usage bias ORFs to seed ORFs",
              SMALL_TEXT, 0, MIDDLE_RIGHT, 0);
  }
  if (xosp->slps != NULL)
  {
    top -= 12;
    AddAttribute (seg, (COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT),
                  RED_COLOR, SOLID_LINE, SOLID_SHADING, 5,
                  0);
    AddLine (seg, start, top, stop, top, FALSE, 0);
    AddAttribute (seg, (COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT),
                  BLACK_COLOR, SOLID_LINE, SOLID_SHADING, STD_PEN_WIDTH,
                  0);
    AddLabel (seg, stop+(20*XScale), top, "Pattern match seed ORFs",
              SMALL_TEXT, 0, MIDDLE_RIGHT, 0);
  }

  xisp->picture = seg;

  w = FixedWindow (10, 10, 640, 720, "Consign", CloseGraphWindowProc);
  SetObjectExtra (w, xisp, CleanUpGraphWindow);
  g = HiddenGroup (w, -1, 0, NULL);
  v = CreateViewer (g, 560, 640, TRUE, TRUE);
  AttachPicture (v, seg, INT4_MIN, INT4_MAX, UPPER_LEFT, XScale, 1, NULL);
  PushButton (g, "Close", CloseGraphWindowButton);
  RealizeWindow (w);
  ArrowCursor ();
  Show (w);

  return;
}

/* button 5 */

static void CloseConsignParentProc (WindoW w)
{
  XOSPtr    xosp;

  if ((xosp = (XOSPtr) GetObjectExtra (w)) != NULL)
  {
    CleanUpXOS (xosp);
    MemFree (xosp->pattern_file);
    MemFree (xosp);
  }
  Remove (w);
  QuitProgram ();
  return;
}

static void EndProg (ButtoN b)
{
  CloseConsignParentProc (ParentWindow (b));
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
  gsp->ignore[OBJ_SEQANNOT] = FALSE;

  xosp = MemNew (sizeof (XOS));

  xosp->flagParamWindow = FALSE;
  xosp->gi = 0;
  xosp->filename = NULL;
  xosp->gsp = gsp;
  xosp->sep = NULL;
  xosp->bsp = NULL;

  xosp->pattern_file = StringSave ("ncbipros.dat");
  xosp->gcd = NULL;
  xosp->gcdi = NULL;

  xosp->orfcut = 150;
  xosp->cutoff = 18;
  xosp->cutoffmin = 0;
  xosp->cutoffmax = 100;
  xosp->window = 360;
  xosp->windowmin = 150;
  xosp->windowmax = 3000;

  xosp->orflist = NULL;
  xosp->slpa = NULL;
  xosp->slpb = NULL;
  xosp->slps = NULL;
  xosp->slpk = NULL;

  xosp->probcut = 0.5;
  xosp->clustmin = 2;
  xosp->findmin = 4;
  xosp->minimumseed = 1800;

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
                   "Consign", CloseConsignParentProc);
  g = HiddenGroup (w, 7, 0, NULL);
  b1 = PushButton (g, "  Input GI ", ReadGIProc);
  b2 = PushButton (g, "Input FastA", ReadFileProc);
  b3 = PushButton (g, " Parameters", ConsignParamProc);
  b4 = PushButton (g, "  Consign  ", ConsignProc);
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
