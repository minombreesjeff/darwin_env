/*   sequin7.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  sequin7.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   1/3/98
*
* $Revision: 6.196 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#ifndef CODECENTER
static char *date_of_compilation = __DATE__;
static char *time_of_compilation = __TIME__;
#else
static char *date_of_compilation = "today";
static char *time_of_compilation = "now";
#endif

#include "sequin.h"
#include <gather.h>
#include <edutil.h>
#include <cdrgn.h>
#include <subutil.h>
#include <tofasta.h>
#include <vsm.h>
#include <document.h>
#include <maputil.h>
#include <asn2gnbp.h>
#include <bspview.h>
#include <findrepl.h>
#include <toasn3.h>
#include <toporg.h>
#include <utilpub.h>
#include <salsap.h>
#include <salptool.h>
#include <salutil.h>
#include <explore.h>
#include <seqpanel.h>
#include <alignmgr2.h>
#include <actutils.h>
#include <tax3api.h>

#define CONVERT_TO_JOIN  1
#define CONVERT_TO_ORDER 2
#define DO_NOT_CONVERT   3

NLM_EXTERN SeqEntryPtr FastaToSeqEntryInternal
(
 VoidPtr input,          /* input pointer (file or memory) */
 Int4 type,              /* type of inquiry FASTA_MEM_IO or FASTA_FILE_IO */
 CharPtr PNTR last_char, /* returned pointer to next FASTA sequence */
 Boolean is_na,          /* type of sequence */
 CharPtr PNTR errormsg,  /* error messge for debugging */
 Boolean parseSeqId,     /* Parse SeqID from def line */
 CharPtr special_symbol     /* Returns special symbol if no SeqEntry */
 );

NLM_EXTERN SeqEntryPtr SequinFastaToSeqEntryEx 
  (
    FILE *fp,               /* file to get sequence from */ 
    Boolean is_na,          /* type of sequence */
    CharPtr PNTR errormsg,  /* error message for debugginq */
    Boolean parseSeqId,     /* Parse SeqID from def line */
    CharPtr special_symbol  /* Returns special symbol if no SeqEntry */
  )
{
  BioseqPtr    bsp;
  FileCache    fc;
  Boolean      forceNuc = FALSE;
  Boolean      forceProt = FALSE;
  Pointer      dataptr;
  Uint2        datatype;
  Char         line [128];
  Int4         pos;
  SeqEntryPtr  sep = NULL;
  CharPtr      str;

  if (errormsg != NULL) {
    *errormsg = NULL;
  }
  if (special_symbol != NULL) {
    *special_symbol = NULLB;
  }
  if (is_na) {
    forceNuc = TRUE;
  } else {
    forceProt = TRUE;
  }
  dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, forceNuc, forceProt, parseSeqId, FALSE);
  if (dataptr != NULL) {
    if (datatype == OBJ_BIOSEQ) {
      bsp = (BioseqPtr) dataptr;
      sep = SeqMgrGetSeqEntryForData (bsp);
      if (sep == NULL) {
        sep = SeqEntryNew ();
        if (sep != NULL) {
          sep->choice = 1;
          sep->data.ptrvalue = bsp;
          SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, sep);
        }
      }
    }
  } else if (special_symbol != NULL) {
    /* look ahead to see what character caused inability to interpret line */
    FileCacheSetup (&fc, fp);
    /* pos = FileCacheTell (&fc); */
    str = FileCacheReadLine (&fc, line, sizeof (line), NULL);
    if (str != NULL && StringDoesHaveText (str)) {
      TrimSpacesAroundString (str);
    }
    *special_symbol = line [0];
    /* seek to start of next line after one that could not be interpreted */
    pos = FileCacheTell (&fc);
    FileCacheSetup (&fc, fp);
    FileCacheSeek (&fc, pos);
    fseek (fp, pos, SEEK_SET);
  }
  /* return FastaToSeqEntryInternal((void *)fp, 2, NULL,is_na, errormsg, parseSeqId, special_symbol); */
  return sep;
}

static FonT  titleFont = NULL;

#ifndef WIN_MAC
void CreateSqnInitialFormMenus (WindoW w)

{
  BaseFormPtr   bfp;
  MenU          m;

  bfp = (BaseFormPtr) GetObjectExtra (w);
  if (bfp != NULL) {
    m = PulldownMenu (w, "File");
    AddAboutAndHelpMenuItems (m);
    if (bfp->importform != NULL || bfp->exportform != NULL) {
      if (bfp->importform != NULL) {
        FormCommandItem (m, "Import...", bfp, VIB_MSG_IMPORT);
      }
      if (bfp->exportform != NULL) {
        FormCommandItem (m, "Export...", bfp, VIB_MSG_EXPORT);
      }
      SeparatorItem (m);
    }
    FormCommandItem (m, "Quit", bfp, VIB_MSG_QUIT);
    m = PulldownMenu (w, "Edit");
    FormCommandItem (m, CUT_MENU_ITEM, bfp, VIB_MSG_CUT);
    FormCommandItem (m, COPY_MENU_ITEM, bfp, VIB_MSG_COPY);
    FormCommandItem (m, PASTE_MENU_ITEM, bfp, VIB_MSG_PASTE);
    FormCommandItem (m, CLEAR_MENU_ITEM, bfp, VIB_MSG_DELETE);
  }
}
#endif

static void DefaultMessageProc (ForM f, Int2 mssg)

{
  StdEditorProcsPtr  sepp;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    if (sepp->handleMessages != NULL) {
      sepp->handleMessages (f, mssg);
    }
  }
}

typedef struct startupform {
  FORM_MESSAGE_BLOCK
} StartupForm, PNTR StartupFormPtr;

static void ChangeDestination (GrouP g)

{
  Char  str [64];
  Int2  val;

  val = GetValue (g);
  switch (val) {
    case 1 :
      RemoveAppProperty ("SequinUseEMBLStyle");
      RemoveAppProperty ("SequinUseDDBJStyle");
      if (GetAppParam ("SEQUIN", "PREFERENCES", "DATABASE", NULL, str, sizeof (str))) {
        if (! StringHasNoText (str)) {
          if (StringICmp (str, "GenBank") != 0) {
            WriteSequinAppParam ("PREFERENCES", "DATABASE", "GenBank");
          }
        }
      }
      break;
    case 2 :
      SetAppProperty ("SequinUseEMBLStyle", (void *) 1024);
      RemoveAppProperty ("SequinUseDDBJStyle");
      WriteSequinAppParam ("PREFERENCES", "DATABASE", "EMBL");
      break;
    case 3 :
      RemoveAppProperty ("SequinUseEMBLStyle");
      SetAppProperty ("SequinUseDDBJStyle", (void *) 1024);
      WriteSequinAppParam ("PREFERENCES", "DATABASE", "DDBJ");
      break;
    default :
      break;
  }
  SetupBioseqPageList ();
}

static void CenterString (RectPtr rptr, CharPtr text, FonT fnt, Int2 inc)

{
  if (fnt != NULL) {
    SelectFont (fnt);
  }
  rptr->bottom = rptr->top + LineHeight ();
  DrawString (rptr, text, 'c', FALSE);
  rptr->top = rptr->bottom + inc;
}

extern void DrawAbout (PaneL p)

{
  RecT  r;


  if (titleFont == NULL) {
#ifdef WIN_MAC
    titleFont = GetFont ("Geneva", 18, TRUE, TRUE, FALSE, "");
#endif
#ifdef WIN_MSWIN
    titleFont = GetFont ("Arial", 24, TRUE, TRUE, FALSE, "");
#endif
#ifdef WIN_MOTIF
    titleFont = GetFont ("Courier", 24, TRUE, TRUE, FALSE, "");
#endif
  }

  ObjectRect (p, &r);
  InsetRect (&r, 4, 4);
  r.top += 5;
  Blue ();
  CenterString (&r, "Sequin", titleFont, 5);
  CenterString (&r, SEQUIN_VERSION, programFont, 5);
  CenterString (&r, SEQUIN_SERVICES, programFont, 10);
  CenterString (&r, "National Center for Biotechnology Information", systemFont, 5);
  CenterString (&r, "National Library of Medicine", systemFont, 5);
  CenterString (&r, "National Institutes of Health", systemFont, 10);
  CenterString (&r, "(301) 496-2475", systemFont, 5);
  CenterString (&r, "info@ncbi.nlm.nih.gov", systemFont, 0);
}

extern Int2 AboutBoxWidth (void)

{
  Int2     max;
  CharPtr  ptr;
  Char     sequinServices [60];
  Char     sequinVersion [60];
  Int2     wid;


  if (titleFont == NULL) {
#ifdef WIN_MAC
    titleFont = GetFont ("Geneva", 18, TRUE, TRUE, FALSE, "");
#endif
#ifdef WIN_MSWIN
    titleFont = GetFont ("Arial", 24, TRUE, TRUE, FALSE, "");
#endif
#ifdef WIN_MOTIF
    titleFont = GetFont ("Courier", 24, TRUE, TRUE, FALSE, "");
#endif
  }

  sprintf (sequinVersion, "Sequin Application Version %s", SEQUIN_APPLICATION);
  ptr = "Standard Release";
/*#ifdef USE_ENTREZ*/
  if (useEntrez || useBlast) {
    ptr = "Network Aware";
  }
/*#endif*/
/*#ifdef INTERNAL_NCBI_SEQUIN*/
  if (indexerVersion) {
    ptr = "Indexer Services";
  }
/*#endif*/
  if (genomeCenter != NULL) {
    ptr = "Genome Center";
  }
  sprintf (sequinServices, "%s [%s]", ptr, date_of_compilation);

  SelectFont (titleFont);
  max = StringWidth ("Sequin");
  SelectFont (programFont);
  wid = StringWidth (sequinVersion);
  if (wid > max) {
    max = wid;
  }
  wid = StringWidth (sequinServices);
  if (wid > max) {
    max = wid;
  }
  SelectFont (systemFont);
  wid = StringWidth ("National Center for Biotechnology Information");
  if (wid > max) {
    max = wid;
  }
  max += 2 * stdCharWidth + 2;
  return max;
}

extern Int2 AboutBoxHeight (void)

{
  Int2  hgt;

  if (titleFont == NULL) {
#ifdef WIN_MAC
    titleFont = GetFont ("Geneva", 18, TRUE, TRUE, FALSE, "");
#endif
#ifdef WIN_MSWIN
    titleFont = GetFont ("Arial", 24, TRUE, TRUE, FALSE, "");
#endif
#ifdef WIN_MOTIF
    titleFont = GetFont ("Courier", 24, TRUE, TRUE, FALSE, "");
#endif
  }

  SelectFont (titleFont);
  hgt = LineHeight () + 5;
  SelectFont (programFont);
  hgt += 2 * LineHeight () + 15;
  SelectFont (systemFont);
  hgt += 5 * LineHeight () + 25;
  hgt += 18;
  return hgt;
}

extern ForM CreateStartupForm (Int2 left, Int2 top, CharPtr title,
                               BtnActnProc startFa2htgs,
                               BtnActnProc startPhrap,
                               BtnActnProc buildContig,
                               BtnActnProc startNew,
                               BtnActnProc readExisting,
                               BtnActnProc fetchFromNet,
                               BtnActnProc showHelp,
                               BtnActnProc quitProgram,
                               WndActnProc activateForm)

{
  ButtoN          b;
  GrouP           c;
  GrouP           d;
  GrouP           k;
  PaneL           p;
  StartupFormPtr  sfp;
  Char            str [32];
  WindoW          w;
#ifndef WIN_MAC
  MenU            m;
#endif

  w = NULL;
  sfp = MemNew (sizeof (StartupForm));
  if (sfp != NULL) {
    w = FixedWindow (left, top, -10, -10, title, NULL);
    SetObjectExtra (w, sfp, StdCleanupFormProc);
    sfp->form = (ForM) w;
    sfp->formmessage = DefaultMessageProc;

#ifndef WIN_MAC
    m = PulldownMenu (w, "Misc");
    CommandItem (m, "Net Configure...", NetConfigureProc);
    if (useEntrez) {
      SeparatorItem (m);
      CommandItem (m, "Entrez Query...", EntrezQueryProc);
      SeparatorItem (m);
      CommandItem (m, "Entrez2 Query...", Entrez2QueryProc);
      if (extraServices) {
        SeparatorItem (m);
        CommandItem (m, "Process FASTA Nucleotide Updates", ParseInNucUpdates);
      }
    }
    if (useDesktop) {
      SeparatorItem (m);
      VSMAddToMenu (m, VSM_DESKTOP);
    }
#endif

    p = SimplePanel (w, AboutBoxWidth (), AboutBoxHeight (), DrawAbout);

    k = HiddenGroup (w, 4, 0, NULL);
    SetGroupSpacing (k, 3, 10);
    StaticPrompt (k, "Database for submission", 0, stdLineHeight, programFont, 'l');
    d = HiddenGroup (k, 4, 0, ChangeDestination);
    RadioButton (d, "GenBank");
    RadioButton (d, "EMBL");
    RadioButton (d, "DDBJ");
    if (GetAppParam ("SEQUIN", "PREFERENCES", "DATABASE", NULL, str, sizeof (str))) {
      if (StringICmp (str, "GenBank") == 0) {
        SetValue (d, 1);
      } else if (StringICmp (str, "EMBL") == 0) {
        SetValue (d, 2);
      } else if (StringICmp (str, "DDBJ") == 0) {
        SetValue (d, 3);
      } else {
        SetValue (d, 1);
      }
    } else {
      SetValue (d, 1);
    }
    ChangeDestination (d);

    c = HiddenGroup (w, 1, 0, NULL);
    SetGroupSpacing (c, 10, 5);

    if (startFa2htgs != NULL) {
      b = PushButton (c, "New FA2HTGS Submission", startFa2htgs);
      SetObjectExtra (b, sfp, NULL);
    }
    if (startPhrap != NULL) {
      b = PushButton (c, "New PHRAP Submission", startPhrap);
      SetObjectExtra (b, sfp, NULL);
    }
    if (buildContig != NULL) {
      b = PushButton (c, "Read CONTIG Instructions", buildContig);
      SetObjectExtra (b, sfp, NULL);
    }
    b = PushButton (c, "Start New Submission", startNew);
    SetObjectExtra (b, sfp, NULL);
    b = PushButton (c, "Read Existing Record", readExisting);
    SetObjectExtra (b, sfp, NULL);
    if (fetchFromNet != NULL) {
      b = PushButton (c, "Download From Entrez", fetchFromNet);
      SetObjectExtra (b, sfp, NULL);
    }
    b = PushButton (c, "Show Help", showHelp);
    SetObjectExtra (b, sfp, NULL);
    b = PushButton (c, "Quit Program", quitProgram);
    SetObjectExtra (b, sfp, NULL);

    AlignObjects (ALIGN_CENTER, (HANDLE) p, (HANDLE) c, (HANDLE) k, NULL);

    RealizeWindow (w);

    if (activateForm != NULL) {
      SetActivate (w, activateForm);
    }
  }
  return (ForM) w;
}

typedef struct formatform {
  FORM_MESSAGE_BLOCK

  GrouP           package;
  GrouP           format;
  GrouP           submType;
  ButtoN          alignmentButton;
  ButtoN          originalButton;
  ButtoN          tpaButton;
  TexT            numseqs;

  Int2            restoreFormatTo;
} FormatForm, PNTR FormatFormPtr;

static Boolean allowGenomicPlusCDNA = FALSE;

static void FormatBlockPtrToFormatForm (ForM f, Pointer data)

{
  FormatBlockPtr  fbp;
  FormatFormPtr   ffp;
  Char            str [32];

  ffp = (FormatFormPtr) GetObjectExtra (f);
  fbp = (FormatBlockPtr) data;
  if (ffp == NULL) return;
  if (fbp != NULL) {
    if (fbp->seqPackage > 0 && fbp->seqPackage <= NUM_SEQ_PKG) {
      if ((! allowGenomicPlusCDNA) && fbp->seqPackage >= SEQ_PKG_GENOMICCDNA) {
        SafeSetValue (ffp->package, fbp->seqPackage - 1);
      } else {
        SafeSetValue (ffp->package, fbp->seqPackage);
      }
      if (fbp->seqPackage <= SEQ_PKG_GENOMICCDNA || fbp->seqPackage == SEQ_PKG_GENBANK) {
        SafeDisable (ffp->alignmentButton);
      } else {
        SafeEnable (ffp->alignmentButton);
      }
    } else {
      SafeSetValue (ffp->package, SEQ_PKG_SINGLE);
      SafeDisable (ffp->alignmentButton);
    }
    if (fbp->seqFormat > 0 && fbp->seqFormat <= NUM_SEQ_FMT) {
      SafeSetValue (ffp->format, fbp->seqFormat);
    } else {
      SafeSetValue (ffp->format, SEQ_FMT_FASTA);
    }
    if (fbp->numSeqs > 0) {
      IntToStr (fbp->numSeqs, str, 0, sizeof (str));
      SafeSetTitle (ffp->numseqs, str);
    } else {
      SafeSetTitle (ffp->numseqs, "");
    }
  } else {
    SafeSetValue (ffp->package, SEQ_PKG_SINGLE);
    SafeDisable (ffp->alignmentButton);
    SafeSetValue (ffp->format, SEQ_FMT_FASTA);
    SafeSetTitle (ffp->numseqs, "");
    ffp->restoreFormatTo = SEQ_FMT_FASTA;
  }
}

static Pointer FormatFormToFormatBlockPtr (ForM f)

{
  FormatBlockPtr  fbp;
  FormatFormPtr   ffp;
  Char            str [32];
  Int2            val;

  fbp = NULL;
  ffp = (FormatFormPtr) GetObjectExtra (f);
  if (ffp == NULL) return NULL;
  fbp = (FormatBlockPtr) MemNew (sizeof (FormatBlock));
  if (fbp == NULL) return NULL;
  fbp->seqPackage = GetValue (ffp->package);
  if ((! allowGenomicPlusCDNA) && fbp->seqPackage >= SEQ_PKG_GENOMICCDNA) {
    (fbp->seqPackage)++;
  }
  fbp->seqFormat = GetValue (ffp->format);
  fbp->submType = GetValue (ffp->submType);
  GetTitle (ffp->numseqs, str, sizeof (str));
  if (StrToInt (str, &val) && val > 0) {
    fbp->numSeqs = val;
  } else {
    fbp->numSeqs = 0;
  }
  return (Pointer) fbp;
}

static void EnableOrDisableFormats (GrouP g)

{
  FormatFormPtr  ffp;
  Int2           val;

  ffp = (FormatFormPtr) GetObjectExtra (g);
  if (ffp == NULL) return;
  val = GetValue (g);
  if ((! allowGenomicPlusCDNA) && val >= SEQ_PKG_GENOMICCDNA) {
    val++;
  }
  if (val <= SEQ_PKG_GENOMICCDNA || val == SEQ_PKG_GENBANK) {
    if (Enabled (ffp->alignmentButton)) {
      ffp->restoreFormatTo = GetValue (ffp->format);
    }
    SafeSetValue (ffp->format, SEQ_FMT_FASTA);
    SafeDisable (ffp->alignmentButton);
  } else {
    if (! Enabled (ffp->alignmentButton)) {
      SafeSetValue (ffp->format, ffp->restoreFormatTo);
    }
    SafeEnable (ffp->alignmentButton);
  }
}

static Boolean ExportTemplateMenu (ForM f, CharPtr filename)
{
  WindoW                w;
  GrouP                 h, g1, g2, c;
  ButtoN                b;
  DialoG                org_dlg;
  TexT                  comment_txt;
  ModalAcceptCancelData acd;
  SeqEntryPtr           sep;
  BioseqSetPtr          bssp;
  BioSourcePtr          biop;
  SeqDescrPtr           sdp;
  CharPtr               org_name;
  Boolean               done;
  
  w = MovableModalWindow (-20, -13, -10, -10, "Submission Template", NULL);
  h = HiddenGroup(w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
  
  g1 = NormalGroup (h, 1, 0, "Organism Name", programFont, NULL);
  org_dlg = OrganismSelectionDialog (g1, "");
  g2 = NormalGroup (h, 2, 0, "Comment", programFont, NULL);
  comment_txt = DialogText (g2, "", 30, NULL);  
  
  c = HiddenGroup (h, 2, 0, NULL);
  b = PushButton (c, "Accept", ModalAcceptButton);
  SetObjectExtra (b, &acd, NULL);
  b = PushButton (c, "Cancel", ModalCancelButton);
  SetObjectExtra (b, &acd, NULL);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) g1, (HANDLE) g2,
                              (HANDLE) c, (HANDLE) NULL);

  Show (w);
  Select (w);
  done = FALSE;
  while (!done)
  { 
    acd.accepted = FALSE;
    acd.cancelled = FALSE;
    while (!acd.accepted && ! acd.cancelled)
    {
      ProcessExternalEvent ();
      Update ();
    }
    ProcessAnEvent ();
  
    if (acd.cancelled)
    {
      done = TRUE;
    }
    else
    {
      bssp = BioseqSetNew ();
      sep = SeqEntryNew ();
      sep->choice = 2;
      sep->data.ptrvalue = bssp;
    
      org_name = DialogToPointer (org_dlg);
      if (!StringHasNoText (org_name))
      {
        biop = BioSourceNew ();
        biop->org = OrgRefNew ();
        biop->org->taxname = org_name;
        sdp = CreateNewDescriptor (sep, Seq_descr_source);
        sdp->data.ptrvalue = biop;
      }
      else
      {
        org_name = MemFree (org_name);
      }
    
      sdp = NULL;   
      if (!TextHasNoText (comment_txt))
      {
        sdp = CreateNewDescriptor (sep, Seq_descr_comment);
        sdp->data.ptrvalue = SaveStringFromText (comment_txt);
      }
    
      done = ExportSubmitterBlockTemplate (sep, sdp);
      SeqEntryFree (sep);
    }
  }
  Remove (w);
  return acd.accepted;
}

static void FormatFormMessage (ForM f, Int2 mssg)

{
  switch (mssg) {
    case VIB_MSG_EXPORT :
      ExportTemplateMenu (NULL, NULL);
      break;
    case VIB_MSG_CUT :
      StdCutTextProc (NULL);
      break;
    case VIB_MSG_COPY :
      StdCopyTextProc (NULL);
      break;
    case VIB_MSG_PASTE :
      StdPasteTextProc (NULL);
      break;
    case VIB_MSG_DELETE :
      StdDeleteTextProc (NULL);
      break;
    default :
      DefaultMessageProc (f, mssg);
      break;
  }
}

static void CreateFormatFormMenus (WindoW w)
{
#ifndef WIN_MAC  
  BaseFormPtr   bfp;
  MenU          m;

  bfp = (BaseFormPtr) GetObjectExtra (w);
  if (bfp != NULL) {
    m = PulldownMenu (w, "File");
    FormCommandItem (m, "Export Template", bfp, VIB_MSG_EXPORT);
  }
#endif
}

extern ForM CreateFormatForm (Int2 left, Int2 top, CharPtr title,
                              BtnActnProc goToNext,
                              BtnActnProc goBack,
                              WndActnProc activateForm)

{
  ButtoN         b;
  GrouP          c;
  FormatFormPtr  ffp;
  GrouP          g1, g2, g3;
  GrouP          h;
  PrompT         ppt;
  Char           str [32];
  WindoW         w;

  w = NULL;
  ffp = MemNew (sizeof (FormatForm));
  if (ffp != NULL) {
    w = FixedWindow (left, top, -10, -10, title, NULL);
    SetObjectExtra (w, ffp, StdCleanupFormProc);
    ffp->form = (ForM) w;
    ffp->toform = FormatBlockPtrToFormatForm;
    ffp->fromform = FormatFormToFormatBlockPtr;
    ffp->formmessage = FormatFormMessage;
    ffp->exportform = ExportTemplateMenu;

    SetGroupSpacing (w, 10, 10);

    CreateFormatFormMenus (w);

    h = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (h, 3, 10);

    g1 = HiddenGroup (h, 2, 0, NULL);

    allowGenomicPlusCDNA = FALSE;
    if (GetAppParam ("SEQUIN", "SETTINGS", "GENOMICPLUSTRANSCRIPTS", NULL, str, sizeof (str))) {
      if (StringICmp (str, "TRUE") == 0) {
        allowGenomicPlusCDNA = TRUE;
      }
    }

    ppt = StaticPrompt (g1, "Submission type", 0, 0, programFont, 'l');
    ffp->package = HiddenGroup (g1, 2, 0, EnableOrDisableFormats);
    SetObjectExtra (ffp->package, ffp, NULL);
    RadioButton (ffp->package, "Single sequence");
    RadioButton (ffp->package, "Segmented sequence");
    RadioButton (ffp->package, "Gapped sequence");
    if (allowGenomicPlusCDNA) {
      RadioButton (ffp->package, "Genomic + Transcripts");
    }
    RadioButton (ffp->package, "Population study");
    RadioButton (ffp->package, "Phylogenetic study");
    RadioButton (ffp->package, "Mutation study");
    RadioButton (ffp->package, "Environmental samples");
    RadioButton (ffp->package, "Batch submission");
    SetValue (ffp->package, SEQ_PKG_SINGLE);
    AlignObjects (ALIGN_MIDDLE, (HANDLE) ppt, (HANDLE) ffp->package, NULL);

    g2 = HiddenGroup (h, 2, 0, NULL);

    ppt = StaticPrompt (g2, "Sequence data format", 0, 0, programFont, 'l');
    ffp->format = HiddenGroup (g2, -1, 0, NULL);
    SetObjectExtra (ffp->format, ffp, NULL);
    RadioButton (ffp->format, "FASTA (no alignment)");
    ffp->alignmentButton = RadioButton (ffp->format, "Alignment (FASTA+GAP, NEXUS, Phylip, etc.)");
    Disable (ffp->alignmentButton);
    SetValue (ffp->format, SEQ_FMT_FASTA);
    ffp->restoreFormatTo = SEQ_FMT_FASTA;
    AlignObjects (ALIGN_MIDDLE, (HANDLE) ppt, (HANDLE) ffp->format, NULL);

    g3 = HiddenGroup (h, 2, 0, NULL);

    ppt = StaticPrompt (g3, "Submission category", 0, 0, programFont, 'l');
    ffp->submType = HiddenGroup (g3, -1, 0, NULL);
    SetObjectExtra (ffp->submType, ffp, NULL);
    ffp->originalButton = RadioButton (ffp->submType, "Original Submission");
    ffp->tpaButton = RadioButton (ffp->submType, "Third Party Annotation");
    SetValue (ffp->submType, SEQ_ORIG_SUBMISSION);
    AlignObjects (ALIGN_MIDDLE, (HANDLE) ppt, (HANDLE) ffp->submType, NULL);

    c = HiddenGroup (w, 4, 0, NULL);
    SetGroupSpacing (c, 10, 2);
    b = PushButton (c, " << Prev Form ", goBack);
    SetObjectExtra (b, ffp, NULL);
    b = PushButton (c, " Next Form >> ", goToNext);
    SetObjectExtra (b, ffp, NULL);

    AlignObjects (ALIGN_LEFT, (HANDLE) g1, (HANDLE) g2, (HANDLE) g3, NULL);
    AlignObjects (ALIGN_CENTER, (HANDLE) h, (HANDLE) c, NULL);

    RealizeWindow (w);

    if (activateForm != NULL) {
      SetActivate (w, activateForm);
    }
  }
  return (ForM) w;
}

extern SequinBlockPtr SequinBlockFree (SequinBlockPtr sbp)

{
  if (sbp != NULL) {
    AuthorFree (sbp->contactperson);
    AuthListFree (sbp->citsubauthors);
    AffilFree (sbp->citsubaffil);
    MemFree (sbp->citsubtitle);
    DateFree (sbp->releasedate);
  }
  return NULL;
}

extern void ExciseString (CharPtr str, CharPtr from, CharPtr to)

{
  Char     ch;
  CharPtr  ptrf;
  CharPtr  ptrt;

  if (str == NULL || from == NULL || to == NULL) return;
  ptrf = StringISearch (str, from);
  if (ptrf == NULL) return;
  ptrt = StringISearch (ptrf, to);
  if (ptrt == NULL) return;
  ptrt += StringLen (to);
  ch = *ptrt;
  while (ch != '\0') {
    *ptrf = ch;
    ptrf++;
    ptrt++;
    ch = *ptrt;
  }
  *ptrf = '\0';
}

typedef struct geneextendlist {
  GeneRefPtr  grp;
  SeqLocPtr   slp;
  ObjMgrPtr   omp;
  Boolean     rsult;
  Char        label [41];
} GeneExtendList, PNTR GeneExtendPtr;

static Boolean GeneExtendFunc (GatherContextPtr gcp)

{
  BioseqPtr      bsp;
  GeneExtendPtr  gep;
  GeneRefPtr     grp;
  Boolean        hasNulls;
  ObjMgrTypePtr  omtp;
  SeqFeatPtr     sfp;
  SeqLocPtr      slp;
  Char           thislabel [41];

  if (gcp == NULL) return TRUE;

  gep = (GeneExtendPtr) gcp->userdata;
  if (gep == NULL ) return TRUE;

  thislabel [0] = '\0';

  if (gcp->thistype == OBJ_SEQFEAT) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp != NULL && sfp->data.choice == SEQFEAT_GENE && sfp->data.value.ptrvalue != NULL) {
      grp = (GeneRefPtr) sfp->data.value.ptrvalue;
      omtp = ObjMgrTypeFind (gep->omp, gcp->thistype, NULL, NULL);
      if (omtp == NULL) {
        return TRUE;
      }
      if (omtp->labelfunc != NULL) {
        (*(omtp->labelfunc)) (gcp->thisitem, thislabel, 40, OM_LABEL_CONTENT);
      }
      if (thislabel [0] != '\0') {
        if (StringICmp (thislabel, gep->label) == 0) {
          if (SeqLocCompare (gep->slp, sfp->location) != SLC_NO_MATCH) {
            bsp = GetBioseqGivenSeqLoc (sfp->location, gcp->entityID);
            if (bsp != NULL) {
              slp = SeqLocMerge (bsp, sfp->location, gep->slp, TRUE, FALSE, FALSE);
              if (slp != NULL) {
                sfp->location = SeqLocFree (sfp->location);
                sfp->location = slp;
                if (bsp->repr == Seq_repr_seg) {
                  slp = SegLocToPartsEx (bsp, sfp->location, TRUE);
                  sfp->location = SeqLocFree (sfp->location);
                  sfp->location = slp;
                  hasNulls = LocationHasNullsBetween (sfp->location);
                  sfp->partial = (sfp->partial || hasNulls);
                }
                FreeAllFuzz (slp);
                gep->rsult = TRUE;
              }
            }
          }
          return FALSE;
        }
      }
    }
  }
  return TRUE;
}

extern Boolean ExtendGene (GeneRefPtr grp, SeqEntryPtr nsep, SeqLocPtr slp)

{
  GeneExtendList  gel;
  GatherScope     gs;
  ObjMgrTypePtr   omtp;
  SeqFeatPtr      sfp;

  if (grp == NULL || nsep == NULL || slp == NULL) return FALSE;
  gel.grp = grp;
  gel.slp = slp;
  gel.omp = ObjMgrGet ();
  gel.label [0] = '\0';
  gel.rsult = FALSE;
  omtp = ObjMgrTypeFind (gel.omp, OBJ_SEQFEAT, NULL, NULL);
  if (omtp != NULL && omtp->labelfunc != NULL) {
    sfp = SeqFeatNew ();
    if (sfp != NULL) {
      sfp->data.choice = SEQFEAT_GENE;
      sfp->data.value.ptrvalue = (Pointer) grp;
      (*(omtp->labelfunc)) ((Pointer) sfp, gel.label, 40, OM_LABEL_CONTENT);
      sfp->data.value.ptrvalue = NULL;
      SeqFeatFree (sfp);
    }
  }
  MemSet ((Pointer)(&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = TRUE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherSeqEntry (nsep, (Pointer) &gel, GeneExtendFunc, &gs);
  return gel.rsult;
}

/*=====================================================================*/
/*                                                                     */
/* CreateGeneAndProtFeats() -                                          */
/*                                                                     */
/*=====================================================================*/

static void CreateGeneAndProtFeats (SeqEntryPtr nsep, SeqEntryPtr psep,
                                    SeqLocPtr slp, CdRegionPtr crp, CharPtr title,
                                    CharPtr best, size_t maxsize, CharPtr PNTR ttl)

{
  Char        activity [256];
  CharPtr     allele = NULL;
  BioseqPtr   bsp;
  Char        ec [32];
  GeneRefPtr  grp = NULL;
  SeqLocPtr   gslp;
  Boolean     hasNulls;
  BioseqPtr   nbsp;
  BioseqPtr   pbsp;
  ProtRefPtr  prp;
  CharPtr     ptr;
  SeqFeatPtr  sfp;
  SeqIdPtr    sip;
  Char        str [256];
  Char        str2 [256];
  Char        geneSynStr[256];

  if (nsep != NULL && psep != NULL && slp != NULL && crp != NULL && title != NULL) {
    if (best != NULL) {
      best [0] = '\0';
    }
    if (IS_Bioseq (nsep) && IS_Bioseq (psep)) {
      nbsp = (BioseqPtr) nsep->data.ptrvalue;
      pbsp = (BioseqPtr) psep->data.ptrvalue;
      if (nbsp != NULL && pbsp != NULL) {

	/*---------------------------------*/
	/* Parse the gene and gene-related */
	/* fields from the title.          */
	/*---------------------------------*/

        ptr = StringISearch (title, "[gene=");
        if (ptr != NULL) {
          StringNCpy_0 (str, ptr + 6, sizeof (str));
          ptr = StringChr (str, ']');
          if (ptr != NULL) {
            *ptr = '\0';
            ptr = StringChr (str, ';');
            if (ptr != NULL) {
              *ptr = '\0';
              ptr++;
              allele = StringChr (ptr, ';');
              if (allele != NULL) {
                *allele = '\0';
                allele++;
              }
            }
            StringNCpy_0 (best, str, maxsize);
            if (StringHasNoText (best)) {
              StringNCpy_0 (best, ptr, maxsize);
            }
	    
	    /*--------------------------------------*/
	    /* Create a Gene Reference pointer with */
	    /* the gene, gene description, allele,  */
	    /* and gene_syn information.            */
	    /*--------------------------------------*/

            grp = CreateNewGeneRef (str, allele, ptr, FALSE);
          }
        }

	    /*-----------------------------------------*/
	    /* Parse the gene_syn field from the title */
	    /*-----------------------------------------*/

        ptr = StringISearch (title, "[gene_syn=");
        if (ptr != NULL) {
          StringNCpy_0 (geneSynStr, ptr + 10, sizeof (str));
          ptr = StringChr (geneSynStr, ']');
          if (ptr != NULL) {
            *ptr = '\0';
          }

          if (grp == NULL) {
            grp = GeneRefNew ();
          }
          ValNodeCopyStr(&(grp->syn),0,geneSynStr);
        }

        if (grp != NULL) {
          if (ExtendGene (grp, nsep, slp)) {
            grp = GeneRefFree (grp);
          } else {
            sfp = CreateNewFeature (nsep, NULL, SEQFEAT_GENE, NULL);
            if (sfp != NULL) {
              sfp->data.value.ptrvalue = (Pointer) grp;
              sfp->location = SeqLocFree (sfp->location);
              sfp->location = AsnIoMemCopy ((Pointer) slp,
                                            (AsnReadFunc) SeqLocAsnRead,
                                            (AsnWriteFunc) SeqLocAsnWrite);
              sip = SeqLocId (sfp->location);
              if (sip != NULL) {
                bsp = BioseqFind (sip);
              } else {
                bsp = nbsp;
              }
              if (bsp != NULL) {
                gslp = SeqLocMerge (bsp, sfp->location, NULL, TRUE, FALSE, FALSE);
                if (gslp != NULL) {
                  sfp->location = SeqLocFree (sfp->location);
                  sfp->location = gslp;
                  if (bsp->repr == Seq_repr_seg) {
                    gslp = SegLocToPartsEx (bsp, sfp->location, TRUE);
                    sfp->location = SeqLocFree (sfp->location);
                    sfp->location = gslp;
                    hasNulls = LocationHasNullsBetween (sfp->location);
                    sfp->partial = (sfp->partial || hasNulls);
                  }
                  FreeAllFuzz (gslp);
                }
              }
            }
          }
        }

	/*-----------------------------------------*/
	/* Parse the function field from the title */
	/*-----------------------------------------*/

        activity [0] = '\0';
        ec [0] = '\0';
        ptr = StringISearch (title, "[function=");
        if (ptr != NULL) {
          StringNCpy_0 (activity, ptr + 10, sizeof (str));
          ptr = StringChr (activity, ']');
          if (ptr != NULL) {
            *ptr = '\0';
          }
        }

	/*------------------------------------------*/
	/* Parse the EC_number field from the title */
	/*------------------------------------------*/

        ptr = StringISearch (title, "[EC_number=");
        if (ptr != NULL) {
          StringNCpy_0 (ec, ptr + 11, sizeof (str));
          ptr = StringChr (ec, ']');
          if (ptr != NULL) {
            *ptr = '\0';
          }
        }

	/*---------------------------------*/
	/* Parse the protein and prot_desc */
	/* fields from the title.          */
	/*---------------------------------*/

        ptr = StringISearch (title, "[prot=");
        if (ptr != NULL) {
          StringNCpy_0 (str, ptr + 6, sizeof (str));
          ptr = StringChr (str, ']');
        } else {
          ptr = StringISearch (title, "[protein=");
          if (ptr != NULL) {
            StringNCpy_0 (str, ptr + 9, sizeof (str));
            ptr = StringChr (str, ']');
          }
        }

	/*---------------------------------*/
	/* If we found a protein value ... */
	/*---------------------------------*/

        if (ptr != NULL)
	  {
	    /*------------------------------------*/
	    /* ... trim off extraneous characters */
	    /*------------------------------------*/

	    *ptr = '\0';

	    /*----------------------------------------------*/
	    /* ... search for a protein description, either */
	    /*     in the prot field (seperated by a ';')   */
	    /*     or in its own 'prot_desc' field.         */
	    /*----------------------------------------------*/

	    ptr = StringChr (str, ';');
	    if (ptr != NULL)
	      {
		*ptr = '\0';
		ptr++;
	      }
	    else
	      {
		ptr = StringISearch (title, "[prot_desc=");
		if (ptr != NULL)
		  {
		    StringNCpy_0 (str2, ptr + 11, sizeof (str2));
		    ptr = StringChr (str2, ']');
		    if (ptr != NULL)
		      {
			*ptr = '\0';
			ptr = str2;
		      }
		  }
	      }

	    /*-----------*/
	    /*  ... ???  */
	    /*-----------*/

	    StringNCpy_0 (best, str, maxsize);
	    if (StringHasNoText (best))
	      StringNCpy_0 (best, ptr, maxsize);

	    /*--------------------------------*/
	    /* ... add the prot and prot_desc */
	    /*     to the Seq Features.       */
	    /*--------------------------------*/

	    prp = CreateNewProtRef (str, ptr, ec, activity);
	    if (prp != NULL)
	      {
		sfp = CreateNewFeature (psep, NULL, SEQFEAT_PROT, NULL);
		if (sfp != NULL)
		  sfp->data.value.ptrvalue = (Pointer) prp;
	      }
	  }

	/*---------------------*/
	/* Parse the ORF field */
	/*---------------------*/

        ptr = StringISearch (title, "[orf]");
        if (ptr != NULL) {
          crp->orf = TRUE;
        }

	/*-------------------------*/
	/* Parse the comment field */
	/*-------------------------*/

        ptr = StringISearch (title, "[comment=");
        if (ptr != NULL) {
          StringNCpy_0 (str, ptr + 9, sizeof (str));
          ptr = StringChr (str, ']');
          if (ptr != NULL) {
            *ptr = '\0';
            if (ttl != NULL) {
              *ttl = StringSave (str);
            }
          }
        }

	/*----------------------*/
	/* Parse the note field */
	/*----------------------*/

        ptr = StringISearch (title, "[note=");
        if (ptr != NULL) {
          StringNCpy_0 (str, ptr + 6, sizeof (str));
          ptr = StringChr (str, ']');
          if (ptr != NULL) {
            *ptr = '\0';
            if (ttl != NULL && *ttl == NULL) {
              *ttl = StringSave (str);
            }
          }
        }
      }
    }
  }
}

static Boolean  intBoxUp;
static Boolean  intBoxRsult;

static void AcceptAskProc (ButtoN b)

{
  intBoxRsult = TRUE;
  intBoxUp = FALSE;
}

static void CancelAskProc (ButtoN b)

{
  intBoxRsult = FALSE;
  intBoxUp = FALSE;
}

static SeqLocPtr AskForInterval (SeqEntryPtr sep, BioseqPtr nuc, BioseqPtr prot)

{
  GrouP       c;
  DialoG      d;
  GrouP       g;
  GrouP       m;
  SeqIdPtr    sip;
  SeqLocPtr   slp;
  Char        str [128];
  ValNodePtr  vnp;
  WindoW      w;

  slp = NULL;
  if (sep == NULL || nuc == NULL || prot == NULL) return NULL;

  if (GetAppParam ("SEQUIN", "PREFERENCES", "ASKIFSUGGESTFAILED", NULL, str, sizeof (str))) {
    if (StringICmp (str, "FALSE") == 0) {
      sip = SeqIdFindWorst (prot->id);
      SeqIdWrite (sip, str, PRINTID_REPORT, sizeof (str));
      Message (MSG_POSTERR, "Suggest failure for %s", str);
      return NULL;
    }
  }

  w = MovableModalWindow (-50, -33, -10, -10, "Enter coding region interval", NULL);
  g = HiddenGroup (w, -1, 0, NULL);
  m = NULL;
  SetGroupSpacing (g, 3, 10);
  if (prot->descr != NULL) {
    vnp = ValNodeFindNext (prot->descr, NULL, Seq_descr_title);
    if (vnp != NULL && vnp->data.ptrvalue != NULL) {
      m = MultiLinePrompt (g, (CharPtr) vnp->data.ptrvalue, stdCharWidth * 28, programFont);
    }
  }
  d = CreateIntervalEditorDialog (g, NULL, 4, 2, sep, TRUE, FALSE);
  c = HiddenGroup (g, 2, 0, NULL);
  SetGroupSpacing (c, 10, 2);
  DefaultButton (c, "Accept", AcceptAskProc);
  PushButton (c, "Cancel", CancelAskProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) d, (HANDLE) c, (HANDLE) m, NULL);
  Show (w);
  Select (w);
  intBoxUp = TRUE;
  intBoxRsult = FALSE;
  while (intBoxUp) {
    ProcessEventOrIdle ();
  }
  ProcessAnEvent ();
  if (intBoxRsult) {
    slp = (SeqLocPtr) DialogToPointer (d);
  }
  Remove (w);
  return slp;
}

extern Boolean AutomaticProteinProcess (SeqEntryPtr esep, SeqEntryPtr psep,
                                        Int2 code, Boolean makeMRNA, BioseqPtr forceTarget)

{
  SeqFeatPtr   cds;
  CdRegionPtr  crp;
  Char         mRnaName [128];
  BioseqPtr    nbsp;
  SeqEntryPtr  nsep;
  BioseqPtr    pbsp;
  SeqFeatPtr   rna;
  RnaRefPtr    rrp;
  SeqEntryPtr  sep;
  SeqLocPtr    slp;
  CharPtr      ttl;
  ValNodePtr   vnp;
  CharPtr      vnpstr;

  if (esep == NULL || psep == NULL) return FALSE;

  nsep = FindNucSeqEntry (esep);
  if (nsep == NULL || (! IS_Bioseq (nsep)) || (! IS_Bioseq (psep))) return FALSE;

  nbsp = (BioseqPtr) nsep->data.ptrvalue;
  pbsp = (BioseqPtr) psep->data.ptrvalue;
  if (nbsp == NULL || pbsp == NULL) return FALSE;

  cds = NULL;
  WatchCursor ();
  Update ();
  if (forceTarget == NULL) {
    slp = PredictCodingRegion (nbsp, pbsp, code);
    if (slp == NULL) {
      ArrowCursor ();
      Update ();
      slp = AskForInterval (nsep, nbsp, pbsp);
    }
  } else {
    sep = SeqMgrGetSeqEntryForData (forceTarget);
    slp = CreateWholeInterval (sep);
  }
  if (slp == NULL) return FALSE;

  mRnaName [0] = '\0';
  ttl = NULL;
  crp = CreateNewCdRgn (0, FALSE, code);
  if (crp != NULL) {
    if (pbsp->descr != NULL) {
      vnp = ValNodeFindNext (pbsp->descr, NULL, Seq_descr_title);
      if (vnp != NULL && vnp->data.ptrvalue != NULL) {
        vnpstr = (CharPtr) vnp->data.ptrvalue;
        CreateGeneAndProtFeats (nsep, psep, slp, crp, vnpstr, mRnaName, sizeof (mRnaName), &ttl);
        ExciseString (vnpstr, "[gene=", "]");
        ExciseString (vnpstr, "[prot=", "]");
        ExciseString (vnpstr, "[protein=", "]");
        ExciseString (vnpstr, "[function=", "]");
        ExciseString (vnpstr, "[EC_number=", "]");
        ExciseString (vnpstr, "[orf", "]");
        ExciseString (vnpstr, "[comment", "]");
        ExciseString (vnpstr, "[note", "]");
        TrimSpacesAroundString (vnpstr);
        if (StringHasNoText (vnpstr)) {
          ValNodeExtract (&(pbsp->descr), Seq_descr_title);
        }
      }
    }
    if (makeMRNA) {
      rrp = RnaRefNew ();
      if (rrp != NULL) {
        rrp->type = 2;
        if (! StringHasNoText (mRnaName)) {
          rrp->ext.choice = 1;
          rrp->ext.value.ptrvalue = StringSave (mRnaName);
        }
        rna = CreateNewFeature (nsep, NULL, SEQFEAT_RNA, NULL);
        if (rna != NULL) {
          rna->data.value.ptrvalue = (Pointer) rrp;
          rna->location = SeqLocFree (rna->location);
          rna->location = AsnIoMemCopy ((Pointer) slp,
                                        (AsnReadFunc) SeqLocAsnRead,
                                        (AsnWriteFunc) SeqLocAsnWrite);
        }
      }
    }
    cds = CreateNewFeature (nsep, NULL, SEQFEAT_CDREGION, NULL);
    if (cds != NULL) {
      cds->data.value.ptrvalue = (Pointer) crp;
      cds->location = SeqLocFree (cds->location);
      cds->location = slp;
      slp = NULL;
      SetSeqFeatProduct (cds, pbsp);
      if (! StringHasNoText (ttl)) {
        cds->comment = ttl;
      }
    }
  }

  SeqLocFree (slp);
  return TRUE;
}

typedef struct fa2htgsform {
  FORM_MESSAGE_BLOCK

  SeqSubmitPtr       ssp;
  SeqEntryPtr        sep;

  GrouP              templateblock;
  GrouP              fastablock;
  GrouP              orderblock;
  GrouP              controlblock;
  GrouP              contigtype;

  DialoG             contigorder;
  EnumFieldAssocPtr  alists [1];
  GrouP              htgsphase;
  ButtoN             draft;
  ButtoN             fulltop;
  ButtoN             activefin;
  TexT               orgname;
  TexT               seqname;
  ButtoN             update;
  TexT               accession;
  TexT               knownlength;
  TexT               gaplength;
  TexT               remark;
  TexT               clone;
  TexT               strain;
  TexT               chromosome;
  TexT               title;
  DialoG             secondaries;

  SeqEntryPtr        seplist;

  ButtoN             okBtn;
  BtnActnProc        finish;
  BtnActnProc        cancel;
  Boolean            readPhrap;
  Boolean            buildContig;

} Fa2htgsForm, PNTR Fa2htgsFormPtr;

/*------------- MakeAc2GBSeqId() -----------------------*/
/***************************************************************
*   MakeAc2GBSeqId:
*   -- return NULL if acnum == null
*                                             Hsiu-Chuan 4-18-97
****************************************************************/
static SeqIdPtr  SqnMakeAc2GBSeqId(CharPtr accession)
{
   TextSeqIdPtr tsip;
   SeqIdPtr sip;

   if (accession == NULL || *accession == '\0')
      return NULL;

   sip = ValNodeNew(NULL);
   sip->choice = SEQID_GENBANK;
   tsip = TextSeqIdNew();
   sip->data.ptrvalue = tsip;
   tsip->accession = StringSave(accession);

   return sip;

} /* MakeAc2GBSeqId */

/*----------- AddExtraAc2Entry() ----------------------------*/
/***************************************************************
*   AddExtraAc2Entry:
*                                             Hsiu-Chuan 4-11-97, modified by JK
****************************************************************/
static void SqnAddDraft2Entry (SeqEntryPtr entry, CharPtr keyword)

{
   BioseqPtr  bsp;
   ValNodePtr vnp;
   GBBlockPtr gbp;

   if (entry == NULL) return;

   bsp = (BioseqPtr)(entry->data.ptrvalue);

   for (gbp= NULL, vnp = bsp->descr; vnp != NULL; vnp = vnp->next)
   {
       if (vnp->choice == Seq_descr_genbank)
       {
          gbp = vnp->data.ptrvalue;
          break;
       }
   }

   if (gbp == NULL)
   {
      vnp = (ValNodePtr) NewDescrOnSeqEntry (entry, Seq_descr_genbank);
      gbp = GBBlockNew();
      vnp->data.ptrvalue = (Pointer)gbp;
   }

   if (gbp != NULL) {
      ValNodeCopyStr (&gbp->keywords, 0, keyword);
   }
}

static Boolean SqnAddExtraAc2Entry (SeqEntryPtr entry , ValNodePtr extra_accs )
{
   BioseqPtr  bsp;
   ValNodePtr vnp;
   GBBlockPtr gbp;
   Char       acnum[17];
   CharPtr    p;
   Int4       i, j;
   SeqHistPtr shp;
   SeqIdPtr   sip;
   ValNodePtr tmp;

   if ((entry == NULL) || (extra_accs == NULL))
      return FALSE;

   bsp = (BioseqPtr)(entry->data.ptrvalue);

   for (gbp= NULL, vnp = bsp->descr; vnp != NULL; vnp = vnp->next)
   {
       if (vnp->choice == Seq_descr_genbank)
       {
          gbp = vnp->data.ptrvalue;
          break;
       }
   }

   shp = bsp->hist; 

   if (gbp == NULL)
   {
      vnp = (ValNodePtr) NewDescrOnSeqEntry (entry, Seq_descr_genbank);
      gbp = GBBlockNew();
      vnp->data.ptrvalue = (Pointer)gbp;
   }
   
   for (tmp = extra_accs; tmp != NULL; tmp = tmp->next)
   {
       p = (CharPtr) tmp->data.ptrvalue;
       if (p == NULL) continue;
       for (i = 0; isalnum((Int4)(*p)) && *p != '\0'; ++p, ++i)
           acnum[i] = *p;
       acnum[i] = '\0'; 
               /* check one_letter+5digits or two_letter+6digits */
       if (i == 6 || i == 8)
       {
          if (!isalpha((Int4)(acnum[0])) || (!(isdigit((Int4)(acnum[1])) && i == 6) &&
              !(isalpha((Int4)(acnum[1])) && i == 8)))
          {
             ErrPostEx(SEV_ERROR,0,0,
 "Invalid accession (one_letter+5digits or two_letter+6digits): %s",
                                                           acnum);
             return FALSE;
          }

          for (j = 2; j < i; ++j)
          {
              if (!(isdigit((Int4)(acnum[j]))))
              {
                 ErrPostEx(SEV_ERROR,0,0,
 "Invalid accession (one_letter+5digits or two_letter+6digits): %s",
                                                           acnum);
                 return FALSE;
              }
          }

          ValNodeCopyStr(&gbp->extra_accessions, 0, acnum);
          sip = SqnMakeAc2GBSeqId (acnum);
          if (shp == NULL)
          {
             shp = SeqHistNew();
             bsp->hist = shp;
          }
          ValNodeLink(&shp->replace_ids, sip);
       }
       else
       {
          ErrPostEx(SEV_ERROR,0,0,
 "Invalid accession (one_letter+5digits or two_letter+6digits): %s",
                                                           acnum);
          return FALSE;
       }

       while (!isalnum((Int4)(*p)) && *p != '\0')
           ++p;
   }

   return TRUE;

} /* AddExtraAc2Entry */

static void RescueSeqGraphs (BioseqPtr bsp, Int2 index, ValNodePtr PNTR vnpp)

{
  SeqAnnotPtr   nextsap;
  SeqGraphPtr   nextsgp;
  Pointer PNTR  prevsap;
  Pointer PNTR  prevsgp;
  SeqAnnotPtr   sap;
  SeqGraphPtr   sgp;

  if (bsp == NULL || vnpp == NULL) return;
  sap = bsp->annot;
  prevsap = (Pointer PNTR) &(bsp->annot);
  while (sap != NULL) {
    nextsap = sap->next;
    if (sap->type == 3) {
      sgp = (SeqGraphPtr) sap->data;
      prevsgp = (Pointer PNTR) &(sap->data);
      while (sgp != NULL) {
        nextsgp = sgp->next;
        *(prevsgp) = sgp->next;
        sgp->next = NULL;
        ValNodeAddPointer (vnpp, index, (Pointer) sgp);
        sgp = nextsgp;
      }
    }
    if (sap->data == NULL) {
      *(prevsap) = sap->next;
      sap->next = NULL;
      SeqAnnotFree (sap);
    } else {
      prevsap = (Pointer PNTR) &(sap->next);
    }
    sap = nextsap;
  }
}

static SeqAnnotPtr NewSeqAnnotType3 (CharPtr name, SeqGraphPtr sgp)

{
  SeqAnnotPtr  sap = NULL;

  if (sgp == NULL) return NULL;
  sap = SeqAnnotNew ();
  if (sap == NULL) return NULL;

  if (! StringHasNoText (name)) {
    SeqDescrAddPointer (&(sap->desc), Annot_descr_name, StringSave (name));
  }
  sap->type = 3;
  sap->data = (Pointer) sgp;

  return sap;
}

static void OffsetAndLinkSeqGraph (BioseqPtr bsp, SeqGraphPtr sgp, Int2 index)

{
  DeltaSeqPtr  dsp;
  SeqGraphPtr  lastsgp;
  Int4         len;
  SeqLitPtr    litp;
  SeqAnnotPtr  sap;
  SeqIntPtr    sintp;
  SeqLocPtr    slp;

  if (bsp == NULL || sgp == NULL || index < 1) return;
  len = 0;
  if (bsp->repr == Seq_repr_delta && bsp->seq_ext_type == 4) {
    for (dsp = (DeltaSeqPtr) (bsp->seq_ext);
         dsp != NULL && index > 1; dsp = dsp->next, index--) {
      if (dsp->choice == 1) {
        len += SeqLocLen ((SeqLocPtr) dsp->data.ptrvalue);
      } else if (dsp->choice == 2) {
        litp = (SeqLitPtr) dsp->data.ptrvalue;
        if (litp != NULL) {
          len += litp->length;
        }
      }
    }
  }
  slp = sgp->loc;
  if (slp != NULL && slp->choice == SEQLOC_INT) {
    sintp = (SeqIntPtr) slp->data.ptrvalue;
    if (sintp != NULL) {
      sintp->from += len;
      sintp->to += len;
      sintp->id = SeqIdFree (sintp->id);
      sintp->id = SeqIdDup (bsp->id);
    }
  }
  for (sap = bsp->annot; sap != NULL; sap = sap->next) {
    if (sap->type == 3) {
      for (lastsgp = sap->data; lastsgp->next != NULL; lastsgp = lastsgp->next) {
        continue;
      }
      lastsgp->next = sgp;
      break;
    }
  }
  if (sap == NULL) {
    if (bsp->annot != NULL) {
      for (sap = bsp->annot; sap->next != NULL; sap = sap->next) {
        continue;
      }
      sap->next = NewSeqAnnotType3 ("Graphs", sgp);
    } else {
      bsp->annot = NewSeqAnnotType3 ("Graphs", sgp);
    }
  }
}

static CharPtr phrapBoilerPlate = "Sequence Quality Assessment:~ \
This entry has been annotated with sequence quality~ \
estimates computed by the Phrap assembly program.~ \
All manually edited bases have been reduced to quality zero.~ \
Quality levels above 40 are expected to have less than ~ \
1 error in 10,000 bp.~ \
Base-by-base quality values are not generally visible from the~ \
GenBank flat file format but are available as part~ \
of this entry's ASN.1 file.~----------------------~";

static void ProcessFa2htgs (Fa2htgsFormPtr ffp, SeqSubmitPtr ssp)

{
  SeqEntryPtr  sep, oldsep, the_entry, nextsep;
  NCBISubPtr nsp;
  Int2 htgs_phase = -1;
  Uint1 tech;
  CharPtr seqname = NULL, accession = NULL, orgname = NULL;
  CharPtr clone = NULL, strain = NULL, chromosome = NULL;
  CharPtr remark = NULL, title = NULL, seqbuf = NULL;
  Int4 length = 0, cumlength = 0, gaplen;
  BioseqPtr bsp;
  BioseqSetPtr bssp;
  SeqLitPtr slp;
  ValNodePtr vnp, PNTR prevpnt, next, extra_accs;
  Boolean lastwasraw, draft, fulltop, activefin, usedelta = FALSE;
  Char str [64];
  long int val;
  Int2 index = 0;
  ValNodePtr rescuedsgps = NULL;
  ValNodePtr seqlitlist = NULL;
  IntFuzzPtr ifp;
  ObjectIdPtr    oip;
  UserFieldPtr   ufp;
  UserObjectPtr  uop;
  DatePtr dp;

  if (ffp == NULL || ssp == NULL) return;

  htgs_phase = GetValue (ffp->htgsphase) - 1;
  orgname = SaveStringFromText (ffp->orgname);
  seqname = SaveStringFromText (ffp->seqname);
  if (GetStatus (ffp->update)) {
    accession = SaveStringFromText (ffp->accession);
  }
  clone = SaveStringFromText (ffp->clone);
  strain = SaveStringFromText (ffp->strain);
  chromosome = SaveStringFromText (ffp->chromosome);
  remark = SaveStringFromText (ffp->remark);
  title = SaveStringFromText (ffp->title);
  extra_accs = DialogToPointer (ffp->secondaries);
  draft = GetStatus (ffp->draft);
  fulltop = GetStatus (ffp->fulltop);
  activefin = GetStatus (ffp->activefin);

  length = 0;
/* may need to really calculate length */
  GetTitle (ffp->knownlength, str, sizeof (str));
  if (! StringHasNoText (str)) {
    if (sscanf (str, "%ld", &val) == 1 && val > 0) {
      length = (Int4) val;
    }
  }

  gaplen = 0;
/* now usually filling in with gaps of 100 bases */
  GetTitle (ffp->gaplength, str, sizeof (str));
  if (! StringHasNoText (str)) {
    if (sscanf (str, "%ld", &val) == 1 && val > 0) {
      gaplen = (Int4) val;
    }
  }

/* modified from fa2htgs */
   oldsep = (SeqEntryPtr)(ssp->data);  /* clear out template */
   ssp->data = NULL;
   MemFree(ssp->sub->tool);
   sprintf (str, "Sequin %s", SEQUIN_APPLICATION);
   ssp->sub->tool = StringSave (str);
   nsp = MemNew(sizeof(NCBISub));
   nsp->ssp = ssp;
   nsp->submittor_key = StringSave (genomeCenter);
   /*
   MemFree(ssp->sub->cit->descr);
   ssp->sub->cit->descr = remark;
   */

   cumlength = 0;
   index = 0;

   sep = ffp->seplist;
   if (sep != NULL && sep->next != NULL) {
     usedelta = TRUE;
   }

   if (ffp->buildContig) {
     ssp->data = (Pointer) ffp->seplist;
     the_entry = ffp->seplist;
     sep = the_entry;

     oip = ObjectIdNew ();
     oip->str = StringSave ("info");
     uop = UserObjectNew ();
     uop->type = oip;
     uop->_class = StringSave ("Genomes");

     oip = ObjectIdNew ();
     oip->id = 0;
     ufp = UserFieldNew ();
     ufp->choice = 2;
     ufp->data.intvalue = 0;
     ufp->label = oip;

     uop->data = ufp;

     if (IS_Bioseq (sep)) {
       bsp = (BioseqPtr) sep->data.ptrvalue;
       vnp = SeqDescrNew (NULL);
       vnp->choice = Seq_descr_user;
       vnp->data.ptrvalue = (Pointer) uop;
       vnp->next = bsp->descr;
       bsp->descr = vnp;
       cumlength = bsp->length;
     }

   }
   else if (htgs_phase < 3 || usedelta)
   {
      the_entry = AddDeltaSeqOnlyToSubmission (
                        nsp,
                        seqname,
                        NULL,
                        accession,
                        0,
                        MOLECULE_CLASS_DNA,
                        MOLECULE_TYPE_GENOMIC,
                        length,
                        TOPOLOGY_LINEAR,
                        STRANDEDNESS_DOUBLE);

      sep = ffp->seplist;
      lastwasraw = FALSE;
      while (sep != NULL)
      {
         nextsep = sep->next;
         sep->next = NULL;
         bsp = (BioseqPtr)(sep->data.ptrvalue);
         if (bsp->repr == Seq_repr_raw)
         {
            if (lastwasraw) {
               slp = AddFakeGapToDeltaSeq(nsp, the_entry, gaplen);
               ValNodeAddPointer (&seqlitlist, 0, (Pointer) slp);
               index++;
               cumlength += gaplen;
            }
            BioseqRawConvert(bsp, Seq_code_iupacna);
            seqbuf = BSMerge((ByteStorePtr)(bsp->seq_data), NULL);
            slp = AddLiteralToDeltaSeq(nsp, the_entry,
               bsp->length);
            AddBasesToLiteral(nsp, slp, seqbuf);
            MemFree(seqbuf);
            lastwasraw = TRUE;
            index++;
         }
         else
         {
            if (bsp->length < 0)
               bsp->length = 0;  /* -1 may be set */
            AddGapToDeltaSeq(nsp, the_entry,
               bsp->length);
            lastwasraw = FALSE;
            index++;
         }
         cumlength += bsp->length;
         RescueSeqGraphs (bsp, index, &rescuedsgps);
         SeqEntryFree(sep);
         sep = nextsep;
      }
   }
   else
   {
    the_entry = AddSeqOnlyToSubmission (
                        nsp,
                        seqname,
                        NULL,
                        accession,
                        0,
                        MOLECULE_CLASS_DNA,
                        MOLECULE_TYPE_GENOMIC,
                        length,
                        TOPOLOGY_LINEAR,
                        STRANDEDNESS_DOUBLE);

      sep = ffp->seplist;
      nextsep = sep->next;
      sep->next = NULL;
      bsp = (BioseqPtr)(sep->data.ptrvalue);
      if (bsp->repr == Seq_repr_raw)
      {
         BioseqRawConvert(bsp, Seq_code_iupacna);
         seqbuf = BSMerge((ByteStorePtr)(bsp->seq_data), NULL);
         AddBasesToBioseq(nsp, the_entry, seqbuf);
         MemFree(seqbuf);
         index++;
      }
      cumlength += bsp->length;
      RescueSeqGraphs (bsp, index, &rescuedsgps);
      SeqEntryFree(sep);
      if (nextsep != NULL) {
        ErrPostEx (SEV_ERROR ,0, 0, "Only the first contig was used for HTGS 3");
      }
      if (length > 0 && length != cumlength) {
        ErrPostEx (SEV_ERROR ,0, 0, "Length is exactly %ld, not %ld",
                   (long) cumlength, (long) length);
        length = cumlength;
      }
   }

    /* get data from template: pub, organism, and comment */
   if (IS_Bioseq(oldsep))
   {
      bsp = (BioseqPtr)(oldsep->data.ptrvalue);
      prevpnt = &(bsp->descr);
   }
   else
   {
      bssp = (BioseqSetPtr)(oldsep->data.ptrvalue);
      prevpnt = &(bssp->descr);
   }

   bsp = (BioseqPtr)(the_entry->data.ptrvalue);
   if (bsp != NULL) {
     bsp->length = MAX (cumlength, length);
   }

   for (vnp = *prevpnt; vnp != NULL; vnp = next)
   {
      next = vnp->next;
      if (vnp->choice == Seq_descr_pub)
      {
         *prevpnt = next;
         vnp->next = NULL;
         ValNodeLink(&(bsp->descr), vnp);
      }
      else
         prevpnt = &(vnp->next);
   }
   if (remark != NULL) {
     vnp = SeqDescrNew (NULL);
     if (vnp != NULL) {
       vnp->choice = Seq_descr_comment;
       vnp->data.ptrvalue = remark;
       ValNodeLink(&(bsp->descr), vnp);
     }
   }

   SeqEntryFree(oldsep);

   AddOrganismToEntryNew(nsp, the_entry, orgname, NULL, NULL, NULL,
                         NULL, NULL, NULL, NULL);

   AddGenomeToEntry(nsp, the_entry, 1);
   if (clone != NULL)
      AddSubSourceToEntry(nsp, the_entry, 3, clone);
   if (chromosome != NULL)
       AddSubSourceToEntry(nsp, the_entry, 1, chromosome);
   if (clone != NULL)
      AddOrgModToEntry(nsp, the_entry, 2, strain);
   if (title != NULL)
      AddTitleToEntry(nsp, the_entry, title);
   if (ffp->readPhrap) {
      AddCommentToEntry(nsp, the_entry, phrapBoilerPlate);
   }

   if (extra_accs != NULL) {
      SqnAddExtraAc2Entry(the_entry, extra_accs);
   }

   if (draft) {
      SqnAddDraft2Entry(the_entry, "HTGS_DRAFT");
   }
   if (fulltop) {
      SqnAddDraft2Entry(the_entry, "HTGS_FULLTOP");
   }
   if (activefin) {
      SqnAddDraft2Entry(the_entry, "HTGS_ACTIVEFIN");
   }

   AddBiomolToEntry(nsp, the_entry, 1);
   if (ffp->buildContig) {
   } else {
     switch (htgs_phase) {
       case 0 :
         tech = MI_TECH_htgs_0;
         break;
       case 1 :
         tech = MI_TECH_htgs_1;
         break;
       case 2 :
         tech = MI_TECH_htgs_2;
         break;
       case 3 :
         tech = MI_TECH_htgs_3;
         break;
       default :
         tech = MI_TECH_htgs_3;
         break;
     }
     AddTechToEntry(nsp, the_entry, tech);
   }
   vnp = NewDescrOnSeqEntry (the_entry, Seq_descr_create_date);
   if (vnp != NULL) {
     dp = DateCurr ();
     vnp->data.ptrvalue = (Pointer) dp;
   }

   if (bsp != NULL) {
     for (vnp = rescuedsgps; vnp != NULL; vnp = vnp->next) {
       OffsetAndLinkSeqGraph (bsp, (SeqGraphPtr) vnp->data.ptrvalue, (Int2) vnp->choice);
       vnp->data.ptrvalue = NULL;
     }
   }
   rescuedsgps = ValNodeFreeData (rescuedsgps);

   for (vnp = seqlitlist; vnp != NULL; vnp = vnp->next) {
     slp = (SeqLitPtr) vnp->data.ptrvalue;
     if (slp != NULL) {
       ifp = IntFuzzNew();
       ifp->choice = 4;    /* lim - unk*/
       slp->fuzz = ifp;
     }
   }
   seqlitlist = ValNodeFree (seqlitlist);

   MemFree (nsp);

}

static Pointer Fa2htgsToSeqSubmitPtr (ForM f)

{
  Fa2htgsFormPtr  ffp;
  SeqSubmitPtr    ssp;

  ffp = (Fa2htgsFormPtr) GetObjectExtra (f);
  if (ffp == NULL) return NULL;
  ProcessFa2htgs (ffp, ffp->ssp);
  ssp = ffp->ssp;
  ffp->ssp = NULL;
  ffp->sep = NULL;
  ffp->seplist = NULL;
  return (Pointer) ssp;
}

static void Fa2htgsFormMessage (ForM f, Int2 mssg)

{
  Fa2htgsFormPtr  ffp;

  ffp = (Fa2htgsFormPtr) GetObjectExtra (f);
  if (ffp) {
    switch (mssg) {
      case VIB_MSG_CUT :
        StdCutTextProc (NULL);
        break;
      case VIB_MSG_COPY :
        StdCopyTextProc (NULL);
        break;
      case VIB_MSG_PASTE :
        StdPasteTextProc (NULL);
        break;
      case VIB_MSG_DELETE :
        StdDeleteTextProc (NULL);
        break;
      default :
        if (ffp->appmessage != NULL) {
          ffp->appmessage (f, mssg);
        }
        break;
    }
  }
}

#ifndef WIN_MAC
static void CreateFa2htgsFormMenus (WindoW w)

{
  BaseFormPtr  bfp;
  MenU         m;

  bfp = (BaseFormPtr) GetObjectExtra (w);
  if (bfp != NULL) {
    m = PulldownMenu (w, "File");
    AddAboutAndHelpMenuItems (m);
    FormCommandItem (m, "Quit", bfp, VIB_MSG_QUIT);
    m = PulldownMenu (w, "Edit");
    FormCommandItem (m, CUT_MENU_ITEM, bfp, VIB_MSG_CUT);
    FormCommandItem (m, COPY_MENU_ITEM, bfp, VIB_MSG_COPY);
    FormCommandItem (m, PASTE_MENU_ITEM, bfp, VIB_MSG_PASTE);
    FormCommandItem (m, CLEAR_MENU_ITEM, bfp, VIB_MSG_DELETE);
  }
}
#endif

static void FillInFa2htgsFields (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr       bsp;
  BioseqSetPtr    bssp;
  Fa2htgsFormPtr  ffp;
  ValNodePtr      sdp = NULL;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  ffp = (Fa2htgsFormPtr) mydata;
  if (ffp == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sdp = bsp->descr;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sdp = bssp->descr;
  } else return;
  while (sdp != NULL) {
    if (sdp->choice == Seq_descr_comment) {
      SetTitle (ffp->remark, (CharPtr) sdp->data.ptrvalue);
    }
    sdp = sdp->next;
  }
}

static void ReadTemplate (ButtoN b)

{
  AsnIoPtr        aip;
  Fa2htgsFormPtr  ffp;
  Char            path [PATH_MAX];
  SeqEntryPtr     sep;
  SeqSubmitPtr    ssp;
  Char            str [128];

  ffp = (Fa2htgsFormPtr) GetObjectExtra (b);
  if (ffp == NULL) return;
  if (GetInputFileName (path, sizeof (path), "", "TEXT")) {
    ffp->ssp = SeqSubmitFree (ffp->ssp);
    aip = AsnIoOpen (path, "r");
    if (aip != NULL) {
      ffp->ssp = SeqSubmitAsnRead (aip, NULL);
    }
    AsnIoClose (aip);
  }
  if (ffp->ssp != NULL) {
    SafeShow (ffp->fastablock);
    SafeDisable (b);
    ssp = ffp->ssp;
    if (ssp->datatype == 1) {
      sep = (SeqEntryPtr) ssp->data;
      if (sep != NULL) {
        SeqEntryToGeneticCode (sep, NULL, str, sizeof (str));
        SetTitle (ffp->orgname, str);
        SeqEntryExplore (sep, (Pointer) ffp, FillInFa2htgsFields);
      }
    }
  }
}

static Boolean Fa2htgsFormOkay (Fa2htgsFormPtr ffp)

{
  if (ffp == NULL) return FALSE;
  if (ffp->ssp == NULL) return FALSE;
  if (ffp->seplist == NULL) return FALSE;
  if (GetValue (ffp->htgsphase) == 0 && (! ffp->buildContig)) return FALSE;
  if (TextHasNoText (ffp->orgname)) return FALSE;
  if (TextHasNoText (ffp->seqname)) return FALSE;
  return TRUE;
}

static void ReadFastaHtgsFile (ButtoN b)

{
  Fa2htgsFormPtr  ffp;
  FILE            *fp;
  SeqEntryPtr     head;
  Char            path [PATH_MAX];
  SeqEntryPtr     sep;
  CharPtr         ttl;

  ffp = (Fa2htgsFormPtr) GetObjectExtra (b);
  if (ffp == NULL) return;
  if (! GetInputFileName (path, sizeof (path), "", "TEXT")) return;
  if (! StringHasNoText (path)) {
    fp = FileOpen (path, "r");
    if (fp != NULL) {
      head = NULL;
      sep = FastaToSeqEntry (fp, TRUE);
      while (sep != NULL) {
        ValNodeLink (&head, sep);
        sep = FastaToSeqEntry (fp, TRUE);
      }
      if (head != NULL) {
        ttl = SeqEntryGetTitle (head);
        SetTitle (ffp->title, ttl);
      }
      ffp->seplist = head;
    }
    FileClose (fp);
    if (ffp->seplist != NULL) {
      SafeShow (ffp->controlblock);
      SafeDisable (b);
      if (TextHasNoText (ffp->orgname)) {
        Select (ffp->orgname);
      } else {
        Select (ffp->seqname);
      }
      if (Fa2htgsFormOkay (ffp)) {
        SafeEnable (ffp->okBtn);
      } else {
        SafeDisable (ffp->okBtn);
      }
    }
  }
}

static EnumFieldAssocPtr MakePhrapAlists (SeqEntryPtr head)

{
  EnumFieldAssocPtr  alist = NULL;
  BioseqPtr          bsp;
  Int2               count;
  Int2               j;
  SeqEntryPtr        sep;
  SeqIdPtr           sip;
  Char               str [128];

  if (head == NULL) return NULL;
  count = ValNodeLen (head);
  alist = MemNew (sizeof (EnumFieldAssoc) * (size_t) (count + 4));
  if (alist == NULL) return NULL;

  j = 0;
  alist [j].name = StringSave ("                              ");
  alist [j].value = (UIEnum) 0;
  for (j = 1, sep = head; j <= count && sep != NULL; j++, sep = sep->next) {
    if (sep != NULL && sep->choice == 1 && sep->data.ptrvalue != NULL) {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      sip = SeqIdFindWorst (bsp->id);
      SeqIdWrite (sip, str, PRINTID_REPORT, sizeof (str));
      str [30] = '\0';
      alist [j].name = StringSave (str);
      alist [j].value = (UIEnum) j;
    }
  }
  j = count + 1;
  alist [j].name = NULL;
  alist [j].value = (UIEnum) 0;


  return alist;
}

static void ReadAPhrapFile (ButtoN b)

{
  PopuP           control;
  Int2            count;
  Fa2htgsFormPtr  ffp;
  FILE            *fp;
  SeqEntryPtr     head;
  Int2            i;
  Char            path [PATH_MAX];
  RecT            r;
  TagListPtr      tlp;

  ffp = (Fa2htgsFormPtr) GetObjectExtra (b);
  if (ffp == NULL) return;
  if (! GetInputFileName (path, sizeof (path), "", "TEXT")) return;
  if (! StringHasNoText (path)) {
    WatchCursor ();
    fp = FileOpen (path, "r");
    if (fp != NULL) {
      head = ReadPhrapFile (fp);
      ffp->seplist = head;
    }
    FileClose (fp);
    tlp = (TagListPtr) GetObjectExtra (ffp->contigorder);
    if (tlp != NULL) {
      ffp->alists [0] = MakePhrapAlists (ffp->seplist);
      tlp->alists = ffp->alists;
      for (i = 0; i < tlp->rows; i++) {
        control = (PopuP) tlp->control [i * MAX_TAGLIST_COLS + 0];
        if (control != NULL) {
          GetPosition (control, &r);
          Reset (control);
          InitEnumPopup (control, ffp->alists [0], NULL);
          SetEnumPopup (control, ffp->alists [0], 0);
          SetPosition (control, &r);
        }
      }
      count = ValNodeLen (ffp->seplist);
      for (i = 0; i < count; i++) {
        ValNodeCopyStr (&(tlp->vnp), 0, "0");
      }
      tlp->max = MAX ((Int2) 0, (Int2) (count - tlp->rows));
      CorrectBarMax (tlp->bar, tlp->max);
      CorrectBarPage (tlp->bar, tlp->rows - 1, tlp->rows - 1);
    }
    SafeShow (ffp->orderblock);
    SafeDisable (b);
    ArrowCursor ();
  }
}

static void PhrapOrderChosen (ButtoN b)

{
  SeqEntryPtr     PNTR collision;
  Int2            count;
  Fa2htgsFormPtr  ffp;
  Int2            i;
  Int2            j;
  SeqEntryPtr     lastsep;
  SeqEntryPtr     nextsep;
  Boolean         okay;
  SeqEntryPtr     PNTR order;
  SeqEntryPtr     sep;
  CharPtr         str;
  TagListPtr      tlp;
  int             val;
  ValNodePtr      vnp;
  /*
  Char            contigs [256];
  Char            tmp [256];
  BioseqPtr       bsp;
  SeqIdPtr        sip;
  Boolean         notfirst;
  */

  ffp = (Fa2htgsFormPtr) GetObjectExtra (b);
  if (ffp == NULL) return;
  tlp = (TagListPtr) GetObjectExtra (ffp->contigorder);
  if (tlp == NULL) return;
  count = ValNodeLen (tlp->vnp);
  order = MemNew (sizeof (SeqEntryPtr) * (count + 1));
  if (order == NULL) return;
  collision = MemNew (sizeof (SeqEntryPtr) * (count + 1));
  if (collision == NULL) return;

  okay = TRUE;
  for (i = 0, vnp = tlp->vnp; i < count && vnp != NULL; i++, vnp = vnp->next) {
    str = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 0);
    if (str != NULL && sscanf (str, "%d", &val) == 1 && val > 0) {
      val--;
      for (j = 0, sep = ffp->seplist; j < (Int2) val && sep != NULL; j++, sep = sep->next) continue;
      if (sep != NULL) {
        if (collision [j] != NULL) {
          okay = FALSE;
        }
        collision [j] = sep;
        order [i] = sep;
      }
    }
    MemFree (str);
  }
  if (! okay) {
    Message (MSG_ERROR, "You must not select a contig more than once");
    MemFree (order);
    MemFree (collision);
    return;
  }

  okay = FALSE;
  for (i = 0; i < count; i++) {
    if (order [i] != NULL) {
      okay = TRUE;
    }
  }
  /* if no contigs selected, use all in order */
  if (! okay) {
    for (j = 0, sep = ffp->seplist; j < count && sep != NULL; j++, sep = sep->next) {
      order [j] = sep;
    }
    okay = TRUE;
  }
  /*
  if (! okay) {
    Message (MSG_ERROR, "You must select at least one contig");
    MemFree (order);
    MemFree (collision);
    return;
  }
  */

  /* use spreadsheet to reorder ffp->seplist, delete unwanted items */

  /*
  contigs [0] = '\0';
  notfirst = FALSE;
  for (i = 0; i < count; i++) {
    if (order [i] != NULL) {
      sep = order [i];
      bsp = (BioseqPtr) sep->data.ptrvalue;
      sip = SeqIdFindWorst (bsp->id);
      SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
      if (notfirst) {
        StringCat (contigs, " ");
      }
      StringCat (contigs, tmp);
      notfirst = TRUE;
    }
  }
  ffp->seplist = SetPhrapContigOrder (ffp->seplist, contigs);
  */

  for (i = 0; i < count; i++) {
    if (order [i] != NULL) {
      sep = ffp->seplist;
      lastsep = NULL;
      while (sep != NULL && sep != order [i]) {
        lastsep = sep;
        sep = sep->next;
      }
      if (sep != NULL) {
        if (lastsep != NULL) {
          lastsep->next = sep->next;
          sep->next = NULL;
        } else {
          ffp->seplist = sep->next;
          sep->next = NULL;
        }
      }
    }
  }
  sep = ffp->seplist;
  while (sep != NULL) {
    nextsep = sep->next;
    sep->next = NULL;
    SeqEntryFree (sep);
    sep = nextsep;
  }
  ffp->seplist = NULL;

  for (i = 0; i < count; i++) {
    if (order [i] != NULL) {
      ValNodeLink (&(ffp->seplist), order [i]);
    }
  }

  MemFree (order);
  MemFree (collision);
  SafeDisable (b);
  SafeHide (ffp->orderblock);
  SafeShow (ffp->controlblock);
  if (TextHasNoText (ffp->orgname)) {
    Select (ffp->orgname);
  } else {
    Select (ffp->seqname);
  }
  if (Fa2htgsFormOkay (ffp)) {
    SafeEnable (ffp->okBtn);
  } else {
    SafeDisable (ffp->okBtn);
  }
}

static void ReadContigFile (ButtoN b)

{
  BioseqPtr       bsp;
  Fa2htgsFormPtr  ffp;
  FILE            *fp;
  Boolean         onMaster;
  Char            path [PATH_MAX];
  SeqEntryPtr     sep = NULL;

  ffp = (Fa2htgsFormPtr) GetObjectExtra (b);
  if (ffp == NULL) return;
  if (! GetInputFileName (path, sizeof (path), "", "TEXT")) return;
  if (! StringHasNoText (path)) {
    WatchCursor ();
    fp = FileOpen (path, "r");
    if (fp != NULL) {
      onMaster = (Boolean) (GetValue (ffp->contigtype) == 2);
      sep = ReadContigList (fp, onMaster);
      if (sep != NULL && sep->choice == 1) {
        ffp->seplist = sep;
        bsp = (BioseqPtr) sep->data.ptrvalue;
        SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, sep);
      }
    }
    FileClose (fp);
    ArrowCursor ();
    if (ffp->seplist != NULL) {
      SafeShow (ffp->controlblock);
      SafeDisable (b);
      SafeDisable (ffp->contigtype);
      if (TextHasNoText (ffp->orgname)) {
        Select (ffp->orgname);
      } else {
        Select (ffp->seqname);
      }
      if (Fa2htgsFormOkay (ffp)) {
        SafeEnable (ffp->okBtn);
      } else {
        SafeDisable (ffp->okBtn);
      }
    }
  }
}

static void AcceptFa2htgs (ButtoN b)

{
  Fa2htgsFormPtr  ffp;

  ffp = (Fa2htgsFormPtr) GetObjectExtra (b);
  if (ffp == NULL) return;
  Hide (ffp->form);
  Update ();
  if (ffp->finish != NULL) {
    ffp->finish (b);
  }
  SeqSubmitFree (ffp->ssp);
  SeqEntryFree (ffp->sep);
  Remove (ffp->form);
}

static void CancelFa2htgs (ButtoN b)

{
  Fa2htgsFormPtr  ffp;

  ffp = (Fa2htgsFormPtr) GetObjectExtra (b);
  if (ffp == NULL) return;
  Hide (ffp->form);
  Update ();
  if (ffp->cancel != NULL) {
    ffp->cancel (b);
  }
  SeqSubmitFree (ffp->ssp);
  SeqEntryFree (ffp->sep);
  Remove (ffp->form);
}

static void SetFa2htgsAcceptBtn (Handle control)

{
  Fa2htgsFormPtr  ffp;

  ffp = (Fa2htgsFormPtr) GetObjectExtra (control);
  if (ffp == NULL) return;
  if (Fa2htgsFormOkay (ffp)) {
    SafeEnable (ffp->okBtn);
  } else {
    SafeDisable (ffp->okBtn);
  }
}

static void SetFa2htgsUpdate (ButtoN b)

{
  Fa2htgsFormPtr  ffp;

  ffp = (Fa2htgsFormPtr) GetObjectExtra (b);
  if (ffp == NULL) return;
  if (GetStatus (b)) {
    SafeEnable (ffp->accession);
  } else {
    SafeDisable (ffp->accession);
  }
  SetFa2htgsAcceptBtn ((Handle) b);
}

static void CleanupGenomeCenterForm (GraphiC g, VoidPtr data)

{
  Fa2htgsFormPtr  ffp;
  SeqEntryPtr     next;
  SeqEntryPtr     sep;

  ffp = (Fa2htgsFormPtr) data;
  if (ffp != NULL) {
    sep = ffp->seplist;
    while (sep != NULL) {
      next = sep->next;
      sep->next = NULL;
      SeqEntryFree (sep);
      sep = next;
    }
    if (ffp->alists [0] != NULL) {
      FreeEnumFieldAlist (ffp->alists [0]);
    }
  }
  StdCleanupFormProc (g, data);
}

static CharPtr secaccstrings [] = {"Secondary", "Accessions", NULL};

static Uint2 contigorder_types [] = {
  TAGLIST_POPUP
};

static ENUM_ALIST(contigdefault_alist)
  {"                              ", 0},
END_ENUM_ALIST

static EnumFieldAssocPtr contigdefault_alists [] = {
  contigdefault_alist
};

extern DialoG CreateTagListDialogEx (GrouP h, Uint2 rows, Uint2 cols,
                                     Int2 spacing, Uint2Ptr types,
                                     Uint2Ptr textWidths, EnumFieldAssocPtr PNTR alists,
                                     Boolean useBar, Boolean noExtend,
                                     ToDialogFunc tofunc, FromDialogFunc fromfunc);

extern ForM CreateGenomeCenterForm (Int2 left, Int2 top, CharPtr title,
                                    BtnActnProc finish,
                                    BtnActnProc cancel,
                                    Boolean readPhrap,
                                    Boolean buildContig,
                                    WndActnProc activateForm)

{
  ButtoN             b;
  GrouP              c;
  Fa2htgsFormPtr     ffp;
  GrouP              g;
  GrouP              h;
  PrompT             ppt;
  GrouP              q;
  GrouP              sa;
  StdEditorProcsPtr  sepp;
  WindoW             w;
  Int2               wid;
  GrouP              x;
  GrouP              y;

  ffp = (Fa2htgsFormPtr) MemNew (sizeof (Fa2htgsForm));
  if (ffp == NULL) return NULL;
  ffp->finish = finish;
  ffp->cancel = cancel;
  ffp->readPhrap = readPhrap;
  ffp->buildContig = buildContig;

  w = FixedWindow (left, top, -10, -10, title, NULL);
  if (w == NULL) return NULL;
  SetObjectExtra (w, ffp, CleanupGenomeCenterForm);

  ffp->form = (ForM) w;
  ffp->toform = NULL;
  ffp->fromform = Fa2htgsToSeqSubmitPtr;
  ffp->formmessage = Fa2htgsFormMessage;

  ffp->ssp = NULL;
  ffp->sep = NULL;
  ffp->seplist = NULL;
  ffp->alists [0] = NULL;

#ifndef WIN_MAC
  CreateFa2htgsFormMenus (w);
#endif

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    ffp->appmessage = sepp->handleMessages;
  }

  SetGroupSpacing (w, 10, 10);

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  ffp->templateblock = HiddenGroup (h, -1, 0, NULL);
  b = PushButton (ffp->templateblock, "Read Seq-submit Template", ReadTemplate);
  SetObjectExtra (b, ffp, NULL);

  ffp->fastablock = HiddenGroup (h, -1, 0, NULL);
  if (readPhrap) {
    b = PushButton (ffp->fastablock, "Read PHRAP File", ReadAPhrapFile);
  } else if (buildContig) {
    SetGroupSpacing (ffp->fastablock, 10, 10);
    y = HiddenGroup (ffp->fastablock, 1, 0, NULL);
    StaticPrompt (y, "Coordinates are on", 0, stdLineHeight, programFont, 'c');
    ffp->contigtype = HiddenGroup (y, 3, 0, NULL);
    RadioButton (ffp->contigtype, "Individual Accessions");
    RadioButton (ffp->contigtype, "Master Sequence");
    SetValue (ffp->contigtype, 1);
    b = PushButton (ffp->fastablock, "Read CONTIG Instructions", ReadContigFile);
    AlignObjects (ALIGN_CENTER, (HANDLE) y, (HANDLE) b, NULL);
  } else {
    b = PushButton (ffp->fastablock, "Read FASTA File", ReadFastaHtgsFile);
  }
  SetObjectExtra (b, ffp, NULL);
  Hide (ffp->fastablock);

  x = HiddenGroup (h, 0, 0, NULL);

  ffp->orderblock = HiddenGroup (x, -1, 0, NULL);
  SetGroupSpacing (ffp->orderblock, 5, 5);
  ppt = StaticPrompt (ffp->orderblock, "Enter contigs in desired order", 0, 0, programFont, 'c');
  ffp->contigorder = CreateTagListDialogEx (ffp->orderblock, 8, 1, 2,
                                            contigorder_types, NULL,
                                            contigdefault_alists,
                                            TRUE, TRUE, NULL, NULL);
  b = PushButton (ffp->orderblock, "Proceed", PhrapOrderChosen);
  SetObjectExtra (b, ffp, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) ppt, (HANDLE) ffp->contigorder,
                (HANDLE) b, NULL);
  Hide (ffp->orderblock);

  ffp->controlblock = HiddenGroup (x, -1, 0, NULL);
  g = HiddenGroup (ffp->controlblock, 2, 0, NULL);

  ffp->htgsphase = NULL;
  if (! buildContig) {
    StaticPrompt (g, "Phase", 0, stdLineHeight, programFont, 'l');
    ffp->htgsphase = HiddenGroup (g, 4, 0, (GrpActnProc) SetFa2htgsAcceptBtn);
    SetObjectExtra (ffp->htgsphase, ffp, NULL);
    RadioButton (ffp->htgsphase, "HTGS-0");
    RadioButton (ffp->htgsphase, "HTGS-1");
    RadioButton (ffp->htgsphase, "HTGS-2");
    RadioButton (ffp->htgsphase, "HTGS-3");
    StaticPrompt (g, "HTGS_", 0, stdLineHeight, programFont, 'l');
    q = HiddenGroup (g, 3, 0, NULL);
    ffp->draft = CheckBox (q, "Draft", NULL);
    ffp->fulltop = CheckBox (q, "Fulltop", NULL);
    ffp->activefin = CheckBox (q, "Activefin", NULL);
  }

  StaticPrompt (g, "Organism", 0, dialogTextHeight, programFont, 'l');
  ffp->orgname = DialogText (g, "", 10, (TxtActnProc) SetFa2htgsAcceptBtn);
  SetObjectExtra (ffp->orgname, ffp, NULL);

  StaticPrompt (g, "Sequence name", 0, dialogTextHeight, programFont, 'l');
  ffp->seqname = DialogText (g, "", 10, (TxtActnProc) SetFa2htgsAcceptBtn);
  SetObjectExtra (ffp->seqname, ffp, NULL);

  StaticPrompt (g, "Length", 0, dialogTextHeight, programFont, 'l');
  ffp->knownlength = DialogText (g, "", 10, NULL);
  SetObjectExtra (ffp->knownlength, ffp, NULL);

  StaticPrompt (g, "Gap Length", 0, dialogTextHeight, programFont, 'l');
  ffp->gaplength = DialogText (g, "100", 10, NULL);
  SetObjectExtra (ffp->gaplength, ffp, NULL);

  ffp->update = CheckBox (g, "Update", SetFa2htgsUpdate);
  SetObjectExtra (ffp->update, ffp, NULL);
  ffp->accession = DialogText (g, "", 10, (TxtActnProc) SetFa2htgsAcceptBtn);
  SetObjectExtra (ffp->accession, ffp, NULL);
  Disable (ffp->accession);

  StaticPrompt (g, "Chromosome", 0, dialogTextHeight, programFont, 'l');
  ffp->chromosome = DialogText (g, "", 10, (TxtActnProc) SetFa2htgsAcceptBtn);
  SetObjectExtra (ffp->chromosome, ffp, NULL);

  StaticPrompt (g, "Clone", 0, dialogTextHeight, programFont, 'l');
  ffp->clone = DialogText (g, "", 10, (TxtActnProc) SetFa2htgsAcceptBtn);
  SetObjectExtra (ffp->clone, ffp, NULL);

  StaticPrompt (g, "Strain", 0, dialogTextHeight, programFont, 'l');
  ffp->strain = DialogText (g, "", 10, (TxtActnProc) SetFa2htgsAcceptBtn);
  SetObjectExtra (ffp->strain, ffp, NULL);

  wid = MaxStringWidths (secaccstrings) + 2;
  sa = MultiLinePrompt (g, "Secondary Accessions", wid, programFont);
  ffp->secondaries = CreateVisibleStringDialog (g, 3, -1, 15);
  AlignObjects (ALIGN_MIDDLE, (HANDLE) sa, (HANDLE) ffp->secondaries, NULL);

  q = HiddenGroup (ffp->controlblock, 1, 0, NULL);

  StaticPrompt (q, "Title", 0, 0, programFont, 'c');
  ffp->title = ScrollText (q, 20, 4, programFont, TRUE, (TxtActnProc) SetFa2htgsAcceptBtn);
  SetObjectExtra (ffp->title, ffp, NULL);

  StaticPrompt (q, "Remark", 0, 0, programFont, 'c');
  ffp->remark = ScrollText (q, 20, 4, programFont, TRUE, (TxtActnProc) SetFa2htgsAcceptBtn);
  SetObjectExtra (ffp->remark, ffp, NULL);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) q, NULL);
  Hide (ffp->controlblock);

  c = HiddenGroup (h, 2, 0, NULL);
  ffp->okBtn = DefaultButton (c, "Accept", AcceptFa2htgs);
  SetObjectExtra (ffp->okBtn, ffp, NULL);
  Disable (ffp->okBtn);
  b = PushButton (c, "Cancel", CancelFa2htgs);
  SetObjectExtra (b, ffp, NULL);

  AlignObjects (ALIGN_CENTER, (HANDLE) ffp->templateblock, (HANDLE) ffp->fastablock,
                (HANDLE) ffp->orderblock, (HANDLE) ffp->controlblock, (HANDLE) c, NULL);

  RealizeWindow (w);

  if (activateForm != NULL) {
    SetActivate (w, activateForm);
  }

  Show (w);
  Select (w);
  Select (ffp->orgname);

  return (ForM) w;
}

typedef struct convcdsdata {
  FEATURE_FORM_BLOCK

  SeqEntryPtr    sep;
  TexT           geneName;
  TexT           protName;
  TexT           featcomment;
  ButtoN         retain;
  Uint2          subtype;
  Int2           errcount;
  Char           findThis [128];
  SeqLocPtr      slp;
  SeqEntryPtr    nsep;
  ObjMgrPtr      omp;
  ObjMgrTypePtr  omtp;
} ConvCdsData, PNTR ConvCdsPtr;

static Boolean CollectCDSAncestorGatherFunc (GatherContextPtr gcp)

{
  BioseqPtr      bsp;
  ConvCdsPtr     ccp;
  Boolean        noLeft;
  Boolean        noRight;
  ObjMgrTypePtr  omtp;
  SeqEntryPtr    sep;
  SeqFeatPtr     sfp;
  SeqLocPtr      slp;
  Uint2          subtype;

  if (gcp == NULL) return TRUE;

  ccp = (ConvCdsPtr) gcp->userdata;
  if (ccp == NULL ) return TRUE;

  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  omtp = ccp->omtp;
  if (omtp == NULL || omtp->subtypefunc == NULL) return TRUE;

  sfp = (SeqFeatPtr) gcp->thisitem;
  subtype = (*(omtp->subtypefunc)) ((Pointer) sfp);
  if (subtype != ccp->subtype) return TRUE;

  bsp = GetBioseqGivenSeqLoc (sfp->location, gcp->entityID);
  if (bsp == NULL) return TRUE;
  if (ISA_aa (bsp->mol)) return TRUE;
  if (bsp->repr != Seq_repr_seg) {
    sep = ccp->nsep;
    if (sep == NULL || sep->choice != 1) return TRUE;
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp == NULL) return TRUE;
  }
  CheckSeqLocForPartial (sfp->location, &noLeft, &noRight);
  slp = SeqLocMerge (bsp, sfp->location, ccp->slp, FALSE, TRUE, FALSE);
  if (slp == NULL) return TRUE;
  SetSeqLocPartial (slp, noLeft, noRight);

  ccp->slp = SeqLocFree (ccp->slp);
  ccp->slp = slp;

  return TRUE;
}

static void FinishConvertingToCDS (SeqEntryPtr sep, Uint2 entityID, ConvCdsPtr ccp)

{
  ByteStorePtr  bs;
  BioseqPtr     bsp;
  Char          ch;
  CdRegionPtr   crp;
  ValNodePtr    descr;
  Uint1         frame;
  Int2          genCode;
  Int2          i;
  Int4          len;
  Int4          lens [4];
  Int4          max;
  Boolean       noLeft;
  Boolean       noRight;
  MolInfoPtr    mip;
  SeqEntryPtr   nsep;
  SeqEntryPtr   old;
  CharPtr       prot;
  ProtRefPtr    prp;
  SeqEntryPtr   psep;
  CharPtr       ptr;
  SeqFeatPtr    sfp;
  Char          str [128];
  ValNodePtr    vnp;

  if (sep == NULL || ccp == NULL) return;
  genCode = SeqEntryToGeneticCode (sep, NULL, NULL, 0);
  crp = CreateNewCdRgn (1, FALSE, genCode);
  if (crp == NULL) return;
  sfp = CreateNewFeature (ccp->nsep, NULL, SEQFEAT_CDREGION, NULL);
  if (sfp == NULL) {
    CdRegionFree (crp);
    return;
  }
  sfp->data.value.ptrvalue = (Pointer) crp;
  sfp->location = SeqLocFree (sfp->location);
  sfp->location = AsnIoMemCopy ((Pointer) ccp->slp,
                                (AsnReadFunc) SeqLocAsnRead,
                                (AsnWriteFunc) SeqLocAsnWrite);
  CheckSeqLocForPartial (sfp->location, &noLeft, &noRight);
  sfp->partial = (sfp->partial || noLeft || noRight);
  if (! TextHasNoText (ccp->featcomment)) {
    sfp->comment = SaveStringFromTextAndStripNewlines (ccp->featcomment);
  }
  max = 0;
  frame = 0;
  for (i = 1; i <= 3; i++) {
    crp->frame = (Uint1) i;
    bs = ProteinFromCdRegionEx (sfp, FALSE, FALSE);
    len = BSLen (bs);
    BSFree (bs);
    lens [i] = len;
    if (len > max) {
      max = len;
      frame = (Uint1) i;
    }
  }
  for (i = 1; i <= 3; i++) {
    if (lens [i] == max && i != frame) {
      (ccp->errcount)++;
    }
  }
  crp->frame = frame;
  bs = ProteinFromCdRegionEx (sfp, TRUE, FALSE);
  if (bs == NULL) return;
  prot = BSMerge (bs, NULL);
  bs = BSFree (bs);
  if (prot == NULL) return;
  ptr = prot;
  ch = *ptr;
  while (ch != '\0') {
    *ptr = TO_UPPER (ch);
    ptr++;
    ch = *ptr;
  }
  i = (Int2) StringLen (prot);
  if (i > 0 && prot [i - 1] == '*') {
    prot [i - 1] = '\0';
  }
  bs = BSNew (1000);
  if (bs != NULL) {
    ptr = prot;
    /*
    if (prot [0] == '-') {
      ptr++;
    }
    */
    BSWrite (bs, (VoidPtr) ptr, (Int4) StringLen (ptr));
  }
  MemFree (prot);
  if (bs == NULL) return;
  bsp = BioseqNew ();
  if (bsp == NULL) return;
  bsp->repr = Seq_repr_raw;
  bsp->mol = Seq_mol_aa;
  bsp->seq_data_type = Seq_code_ncbieaa;
  bsp->seq_data = bs;
  bsp->length = BSLen (bs);
  bs = NULL;
  old = SeqEntrySetScope (sep);
  bsp->id = MakeNewProteinSeqId (sfp->location, NULL);
  SeqMgrAddToBioseqIndex (bsp);
  SeqEntrySetScope (old);
  psep = SeqEntryNew ();
  if (psep != NULL) {
    psep->choice = 1;
    psep->data.ptrvalue = (Pointer) bsp;
    SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, psep);
    mip = MolInfoNew ();
    if (mip != NULL) {
      mip->biomol = 8;
      mip->tech = 8;
      if (noLeft && noRight) {
        mip->completeness = 5;
      } else if (noLeft) {
        mip->completeness = 3;
      } else if (noRight) {
        mip->completeness = 4;
      }
      vnp = CreateNewDescriptor (psep, Seq_descr_molinfo);
      if (vnp != NULL) {
        vnp->data.ptrvalue = (Pointer) mip;
      }
    }
    descr = ExtractBioSourceAndPubs (sep);
    /*
    AddSeqEntryToSeqEntry (sep, psep, FALSE);
    */
    AddSeqEntryToSeqEntry (sep, psep, TRUE);
    nsep = FindNucSeqEntry (sep);
    ReplaceBioSourceAndPubs (sep, descr);
    SetSeqFeatProduct (sfp, bsp);
    GetTitle (ccp->protName, str, sizeof (str));
    if (! StringHasNoText (str)) {
      prp = CreateNewProtRef (str, NULL, NULL, NULL);
      if (prp != NULL) {
        sfp = CreateNewFeature (psep, NULL, SEQFEAT_PROT, NULL);
        if (sfp != NULL) {
          sfp->data.value.ptrvalue = (Pointer) prp;
          SetSeqLocPartial (sfp->location, noLeft, noRight);
          sfp->partial = (sfp->partial || noLeft || noRight);
        }
      }
    }
  }
}

static void RemoveCDSAncestorCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  ConvCdsPtr     ccp;
  SeqAnnotPtr    nextsap;
  SeqFeatPtr     nextsfp;
  ObjMgrTypePtr  omtp;
  Pointer PNTR   prevsap;
  Pointer PNTR   prevsfp;
  SeqAnnotPtr    sap;
  SeqFeatPtr     sfp;
  Uint2          subtype;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  ccp = (ConvCdsPtr) mydata;
  if (ccp == NULL) return;
  omtp = ccp->omtp;
  if (omtp == NULL || omtp->subtypefunc == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
    prevsap = (Pointer PNTR) &(bsp->annot);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
    prevsap = (Pointer PNTR) &(bssp->annot);
  } else return;
  while (sap != NULL) {
    nextsap = sap->next;
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      prevsfp = (Pointer PNTR) &(sap->data);
      while (sfp != NULL) {
        nextsfp = sfp->next;
        subtype = (*(omtp->subtypefunc)) ((Pointer) sfp);
        if (subtype == ccp->subtype) {
          *(prevsfp) = sfp->next;
          sfp->next = NULL;
          SeqFeatFree (sfp);
        } else {
          prevsfp = (Pointer PNTR) &(sfp->next);
        }
        sfp = nextsfp;
      }
    }
    if (sap->data == NULL) {
      *(prevsap) = sap->next;
      sap->next = NULL;
      SeqAnnotFree (sap);
    } else {
      prevsap = (Pointer PNTR) &(sap->next);
    }
    sap = nextsap;
  }
}

static void ConvertToCDSCallback (SeqEntryPtr sep, Uint2 entityID, ConvCdsPtr ccp)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  GeneRefPtr    grp;
  GatherScope   gs;
  SeqLocPtr     gslp;
  Boolean       hasNulls;
  Boolean       noLeft;
  Boolean       noRight;
  SeqEntryPtr   nsep;
  SeqFeatPtr    sfp;
  SeqIdPtr      sip;
  SeqLocPtr     slp;
  Char          str [128];

  if (sep == NULL || ccp == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (IsPopPhyEtcSet (bssp->_class)))) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        ConvertToCDSCallback (sep, entityID, ccp);
      }
      return;
    }
  }
  ccp->nsep = FindNucSeqEntry (sep);
  if (ccp->nsep == NULL) return;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.scope = sep;
  ccp->slp = NULL;
  GatherEntity (entityID, (Pointer) ccp, CollectCDSAncestorGatherFunc, &gs);
  if (ccp->slp != NULL) {
    CheckSeqLocForPartial (ccp->slp, &noLeft, &noRight);
    sip = SeqLocId (ccp->slp);
    if (sip != NULL) {
      bsp = BioseqFind (sip);
      if (bsp != NULL && ISA_na (bsp->mol)) {
        slp = SegLocToParts (bsp, ccp->slp);
        if (slp != NULL) {
          ccp->slp = SeqLocFree (ccp->slp);
          ccp->slp = slp;
          FreeAllFuzz (ccp->slp);
          SetSeqLocPartial (ccp->slp, noLeft, noRight);
        }
      }
    }
    FinishConvertingToCDS (sep, entityID, ccp);
    nsep = FindNucSeqEntry (sep);
    GetTitle (ccp->geneName, str, sizeof (str));
    if (! StringHasNoText (str)) {
      grp = CreateNewGeneRef (str, NULL, NULL, FALSE);
      if (grp != NULL) {
        if (ExtendGene (grp, nsep, ccp->slp)) {
          grp = GeneRefFree (grp);
        } else {
          sfp = CreateNewFeature (nsep, NULL, SEQFEAT_GENE, NULL);
          if (sfp != NULL) {
            sfp->data.value.ptrvalue = (Pointer) grp;
            sfp->location = SeqLocFree (sfp->location);
            sfp->location = AsnIoMemCopy ((Pointer) ccp->slp,
                                          (AsnReadFunc) SeqLocAsnRead,
                                          (AsnWriteFunc) SeqLocAsnWrite);
            sip = SeqLocId (sfp->location);
            if (sip != NULL) {
              bsp = BioseqFind (sip);
              if (bsp != NULL) {
                gslp = SeqLocMerge (bsp, sfp->location, NULL, TRUE, FALSE, FALSE);
                if (gslp != NULL) {
                  sfp->location = SeqLocFree (sfp->location);
                  sfp->location = gslp;
                  if (bsp->repr == Seq_repr_seg) {
                    gslp = SegLocToPartsEx (bsp, sfp->location, TRUE);
                    sfp->location = SeqLocFree (sfp->location);
                    sfp->location = gslp;
                    hasNulls = LocationHasNullsBetween (sfp->location);
                    sfp->partial = (sfp->partial || hasNulls);
                  }
                  FreeAllFuzz (gslp);
                  SetSeqLocPartial (sfp->location, noLeft, noRight);
                  sfp->partial = (sfp->partial || noLeft || noRight);
                }
              }
            }
          }
        }
      }
    }
  }
  ccp->slp = SeqLocFree (ccp->slp);
}

static void DoConvertToCDS (ButtoN b)

{
  ConvCdsPtr  ccp;
  CharPtr     plural;
  Char        str [128];

  ccp = GetObjectExtra (b);
  if (ccp == NULL) {
    Remove (ParentWindow (b));
    return;
  }
  GetTitle (ccp->protName, str, sizeof (str));
  if (StringHasNoText (str)) {
    Message (MSG_OK, "Protein name is required");
    return;
  }
  Hide (ccp->form);
  WatchCursor ();

  ccp->omp = ObjMgrGet ();
  if (ccp->omp != NULL) {
    ccp->omtp = ObjMgrTypeFind (ccp->omp, OBJ_SEQFEAT, NULL, NULL);
    if (ccp->omtp != NULL && ccp->omtp->subtypefunc != NULL) {
      ConvertToCDSCallback (ccp->sep, ccp->input_entityID, ccp);
      if (! GetStatus (ccp->retain)) {
        SeqEntryExplore (ccp->sep, (Pointer) ccp, RemoveCDSAncestorCallback);
      }
    }
  }

  ArrowCursor ();
  Update ();

  if (ccp->errcount > 0) {
    if (ccp->errcount > 1) {
      plural = "records";
    } else {
      plural = "record";
    }
    Message (MSG_ERROR, "Possible ambiguous frames detected in %d %s",
             (int) ccp->errcount, plural);
  }

  ObjMgrSetDirtyFlag (ccp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ccp->input_entityID, 0, 0);
  Remove (ccp->form);
  Update ();
}

extern void PrepareToConvertToCDS (SeqEntryPtr sep, Uint2 entityID,
                                   Uint2 subtype, CharPtr findthis)

{
  ButtoN             b;
  GrouP              c;
  ConvCdsPtr         ccp;
  GrouP              g;
  GrouP              h;
  StdEditorProcsPtr  sepp;
  WindoW             w;

  if (sep == NULL || entityID == 0 || subtype == 0) return;

  ccp = (ConvCdsPtr) MemNew (sizeof (ConvCdsData));
  if (ccp == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "Convert to CDS", StdCloseWindowProc);
  SetObjectExtra (w, ccp, StdCleanupFormProc);
  ccp->form = (ForM) w;
  ccp->formmessage = DefaultMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    ccp->appmessage = sepp->handleMessages;
  }

  ccp->input_entityID = entityID;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  ccp->sep = sep;
  ccp->subtype = subtype;
  StringNCpy_0 (ccp->findThis, findthis, sizeof (ccp->findThis));
  ccp->errcount = 0;

  g = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (g, "Gene Symbol", 0, dialogTextHeight, programFont, 'l');
  ccp->geneName = DialogText (g, "", 20, NULL);
  StaticPrompt (g, "Protein Name", 0, dialogTextHeight, programFont, 'l');
  ccp->protName = DialogText (g, "", 20, NULL);
  StaticPrompt (g, "Comment", 0, 4 * Nlm_stdLineHeight, programFont, 'l');
  ccp->featcomment = ScrollText (g, 20, 4, programFont, TRUE, NULL);

  ccp->retain = CheckBox (h, "Retain original features", NULL);

  c = HiddenGroup (h, 4, 0, NULL);
  b = DefaultButton (c, "Accept", DoConvertToCDS);
  SetObjectExtra (b, ccp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, (HANDLE) ccp->retain, NULL);
  RealizeWindow (w);
  Show (w);
  Select (ccp->geneName);
  Update ();
}

typedef struct alignform {
  FORM_MESSAGE_BLOCK
  DoC            doc;
} AlignForm, PNTR AlignFormPtr;

static ParData bioseqParFmt = {FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData bioseqColFmt [] = {
  {0, 5, 0, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, TRUE}
};

static ParData annotParFmt = {FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData annotColFmt [] = {
  {0, 10, 0, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, TRUE}
};

static ParData alignParFmt = {FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData alignColFmt [] = {
  {0, 15, 0, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE},
  {0, 5, 0, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE},
  {0, 0, 0, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, TRUE}
};

static Boolean AlignFormPopulateProc (GatherContextPtr gcp)

{
  AlignFormPtr      afp;
  SeqAlignPtr       align;
  Char              annotDB [32];
  Uint1             annot_type;
  BioseqContextPtr  bcp;
  BioseqPtr         bsp;
  DenseDiagPtr      ddp;
  DenseSegPtr       dsp;
  ProtRefPtr        prp;
  CharPtr           ptr;
  SeqAnnotPtr       sap;
  SeqEntryPtr       sep;
  SeqFeatPtr        sfp;
  SeqIdPtr          sip;
  StdSegPtr         ssp;
  Char              str [256];
  Char              tmp [128];

  if (gcp == NULL) return TRUE;

  afp = (AlignFormPtr) gcp->userdata;
  if (afp == NULL ) return TRUE;

  switch (gcp->thistype) {
    case OBJ_BIOSEQ :
      bsp = (BioseqPtr) gcp->thisitem;
      if (bsp != NULL) {
        SeqIdWrite (bsp->id, str, PRINTID_REPORT, sizeof (str));
        bcp = BioseqContextNew (bsp);
        sfp = BioseqContextGetSeqFeat (bcp, SEQFEAT_PROT, NULL, NULL, 0);
        BioseqContextFree (bcp);
        if (sfp != NULL) {
          prp = (ProtRefPtr) sfp->data.value.ptrvalue;
          if (prp != NULL) {
            if (prp->name != NULL && (! StringHasNoText (prp->name->data.ptrvalue))) {
              StringCat (str, " (");
              StringCat (str, (CharPtr) prp->name->data.ptrvalue);
              StringCat (str, ")");
            } else if (! StringHasNoText (prp->desc)) {
              StringCat (str, " (");
              StringCat (str, (CharPtr) prp->desc);
              StringCat (str, ")");
            }
          }
        }
        StringCat (str, ":\n");
        AppendText (afp->doc, str, &bioseqParFmt, bioseqColFmt, programFont);
      }
      break;
    case OBJ_SEQANNOT :
      sap = (SeqAnnotPtr) gcp->thisitem;
      if (sap != NULL && sap->type == 2) {
        get_align_annot_qual (sap, annotDB, sizeof (annotDB), &annot_type);
        if (annot_type == ANNOT_BLAST) {
          StringCpy (str, annotDB);
          StringCat (str, ":\n");
          AppendText (afp->doc, str, &annotParFmt, annotColFmt, programFont);
        }
      }
      break;
    case OBJ_SEQALIGN :
    case OBJ_SEQHIST_ALIGN :
      align = (SeqAlignPtr) gcp->thisitem;
      sip = NULL;
      if (align->segtype == 1) {
        ddp = (DenseDiagPtr) align->segs;
        if (ddp != NULL) {
          for (sip = ddp->id; sip != NULL && sip->next != NULL; sip = sip->next) {
          }
        }
      } else if (align->segtype == 2) {
        dsp = (DenseSegPtr) align->segs;
        if (dsp != NULL) {
          for (sip = dsp->ids; sip != NULL && sip->next != NULL; sip = sip->next) {
          }
        }
      } else if (align->segtype == 3) {
        ssp = (StdSegPtr) align->segs;
        if (ssp != NULL) {
          for (sip = ssp->ids; sip != NULL && sip->next != NULL; sip = sip->next) {
          }
        }
      }
      if (sip != NULL) {
        bsp = BioseqLockById (sip);
        if (bsp != NULL) {
          /* SeqIdWrite (bsp->id, str, PRINTID_FASTA_LONG, sizeof (str)); */
          SeqIdWrite (bsp->id, str, PRINTID_REPORT, sizeof (str));
          StringNCpy_0 (tmp, BioseqGetTitle (bsp), sizeof (tmp));
          StringCat (str, "\t");
          if (! StringHasNoText (tmp)) {
            StringCat (str, tmp);
          }
          tmp [0] = '\0';
          sep = SeqMgrGetSeqEntryForData (bsp);
          if (sep != NULL) {
            SeqEntryToBioSource (sep, NULL, tmp, sizeof (tmp), NULL);
            if (! StringHasNoText (tmp)) {
              ptr = StringStr (tmp, "(");
              if (ptr != NULL) {
                *ptr = '\0';
              }
              StringCat (str, " [");
              StringCat (str, tmp);
              StringCat (str, "]");
            }
          }
          StringCat (str, "\t");
          sprintf (tmp, "%d %d %d", (int) gcp->entityID,
                   (int) gcp->itemID, (int) gcp->thistype);
          StringCat (str, tmp);
          StringCat (str, "\n");
          AppendText (afp->doc, str, &alignParFmt, alignColFmt, programFont);
        }
        BioseqUnlock (bsp);
      }
      return TRUE;
    default :
      break;
  }
  return TRUE;
}

static void PopulateAlignForm (ForM f, SeqEntryPtr sep)

{
  AlignFormPtr  afp;
  GatherScope   gs;
  RecT          r;
  Int2          width;

  if (f == NULL || sep == NULL) return;
  afp = (AlignFormPtr) GetObjectExtra (f);
  if (afp == NULL) return;
  Reset (afp->doc);
  SetDocAutoAdjust (afp->doc, FALSE);
  ObjectRect (afp->doc, &r);
  InsetRect (&r, 4, 4);
  width = r.right - r.left;
  alignColFmt [0].pixWidth = width / 5;
  alignColFmt [1].pixWidth = width - alignColFmt [0].pixWidth;
  bioseqColFmt [0].pixWidth = width;
  annotColFmt [0].pixWidth = width;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  MemSet((Pointer) (gs.ignore), (int) (TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQALIGN] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.ignore[OBJ_SEQHIST] = FALSE;
  gs.ignore[OBJ_SEQHIST_ALIGN] = FALSE;
  gs.scope = sep;
  GatherEntity (afp->input_entityID, (Pointer) afp, AlignFormPopulateProc, &gs);
  AdjustDocScroll (afp->doc);
}

static void AlignMessageProc (ForM f, Int2 mssg)

{
  FILE          *fp;
  AlignFormPtr  afp;
  Char          path [PATH_MAX];

  afp = (AlignFormPtr) GetObjectExtra (f);
  if (afp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        Remove (f);
        break;
      case VIB_MSG_PRINT :
        PrintDocument (afp->doc);
        break;
      case VIB_MSG_EXPORT :
        if (GetOutputFileName (path, sizeof (path), "align.txt")) {
          WatchCursor ();
#ifdef WIN_MAC
          fp = FileOpen (path, "r");
          if (fp != NULL) {
            FileClose (fp);
          } else {
            FileCreate (path, "TEXT", "ttxt");
          }
#endif
          fp = FileOpen (path, "w");
          if (fp != NULL) {
            SaveDocument (afp->doc, fp);
            FileClose (fp);
          }
          ArrowCursor ();
        }
        break;
      default :
        if (afp->appmessage != NULL) {
          afp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void AlignFormActivate (WindoW w)

{
  AlignFormPtr  afp;
  IteM          exportItm;
  IteM          printItm;

  afp = (AlignFormPtr) GetObjectExtra (w);
#ifdef WIN_MAC
  currentFormDataPtr = (VoidPtr) afp;
#endif
  if (afp != NULL) {
    if (afp->activate != NULL) {
      afp->activate (w);
    }
    exportItm = FindFormMenuItem ((BaseFormPtr) afp, VIB_MSG_EXPORT);
    SafeSetTitle (exportItm, "Export Align Summary...");
    SafeEnable (exportItm);
    printItm = FindFormMenuItem ((BaseFormPtr) afp, VIB_MSG_PRINT);
    SafeEnable (printItm);
  }
}

static void AlignNotifyProc (DoC d, Int2 item, Int2 row, Int2 col, Boolean dblClick)

{
  unsigned int  entityID;
  unsigned int  itemID;
  unsigned int  itemtype;
  CharPtr       txt;

  if (d == NULL || item < 1 || row < 1 || col < 1) return;
  txt = GetDocText (d, item, row, 3);
  if (! StringHasNoText (txt)) {
    if (sscanf (txt, "%u %u %u", &entityID, &itemID, &itemtype) == 3) {
      if (shftKey) {
        ObjMgrAlsoSelect (entityID, itemID, itemtype, 0, NULL);
      } else {
        ObjMgrSelect (entityID, itemID, itemtype, 0, NULL);
      }
    }
  }
  MemFree (txt);
}

static ForM CreateAlignForm (Uint2 entityID)

{
  AlignFormPtr       afp;
  GrouP              c;
  GrouP              h;
  StdEditorProcsPtr  sepp;
  WindoW             w;
#ifndef WIN_MAC
  MenU               m;
#endif

  w = NULL;
  afp = MemNew (sizeof (AlignForm));
  if (afp != NULL) {
    w = FixedWindow (-50, -33, -10, -10, "Alignment Summary", StdCloseWindowProc);
    SetObjectExtra (w, afp, StdCleanupFormProc);
    afp->form = (ForM) w;
    afp->formmessage = AlignMessageProc;
    afp->input_entityID = entityID;

    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      afp->appmessage = sepp->handleMessages;
    }

#ifndef WIN_MAC
    m = PulldownMenu (w, "File");
    FormCommandItem (m, "Close", (BaseFormPtr) afp, VIB_MSG_CLOSE);
    SeparatorItem (m);
    FormCommandItem (m, "Export...", (BaseFormPtr) afp, VIB_MSG_EXPORT);
    SeparatorItem (m);
    FormCommandItem (m, "Print...", (BaseFormPtr) afp, VIB_MSG_PRINT);
#endif

    h = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (h, 10, 10);

    afp->doc = DocumentPanel (h, 35 * stdCharWidth, 20 * stdLineHeight);
    SetObjectExtra (afp->doc, afp, NULL);
    SetDocNotify (afp->doc, AlignNotifyProc);

    c = HiddenGroup (w, 4, 0, NULL);
    PushButton (c, "Cancel", StdCancelButtonProc);

    AlignObjects (ALIGN_CENTER, (HANDLE) afp->doc, (HANDLE) c, NULL);

    RealizeWindow (w);
    SetActivate (w, AlignFormActivate);
  }
  return (ForM) w;
}

extern void ViewAlignmentSummary (IteM i)

{
  BaseFormPtr  bfp;
  ForM         f;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL || bfp->input_entityID == 0) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  WatchCursor ();
  Update ();
  f = CreateAlignForm (bfp->input_entityID);
  PopulateAlignForm (f, sep);
  ArrowCursor ();
  Update ();
  Show (f);
  Select (f);
}

static ENUM_ALIST(prot_subtype_alist)
  {" ",                    5},
  {"Product",              0},
  {"Pre-protein",          1},
  {"Mature",               2},
  {"Signal peptide",       3},
  {"Transit peptide",      4},
END_ENUM_ALIST

typedef struct geneprotfind {
  SeqFeatPtr     cds;
  SeqFeatPtr     gene;
  SeqFeatPtr     prot;
  SeqLocPtr      location;
  SeqLocPtr      product;
  Int4           mingene;
  Int4           minprot;
} GeneProtFind, PNTR GeneProtPtr;

static Boolean GeneProtFindFunc (GatherContextPtr gcp)

{
  Int4         diff;
  GeneProtPtr  gpp;
  SeqFeatPtr   sfp;

  if (gcp == NULL) return TRUE;
  gpp = (GeneProtPtr) gcp->userdata;
  if (gpp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->data.value.ptrvalue == NULL) return TRUE;
  if (sfp->data.choice == SEQFEAT_GENE && gpp->location != NULL) {
    diff = SeqLocAinB (gpp->location, sfp->location);
    if (diff >= 0) {
      if (diff < gpp->mingene) {
        gpp->gene = sfp;
        gpp->mingene = diff;
      }
    }
  } else if (sfp->data.choice == SEQFEAT_PROT && gpp->product != NULL) {
    diff = SeqLocAinB (gpp->product, sfp->location);
    if (diff >= 0) {
      if (diff < gpp->minprot) {
        gpp->prot = sfp;
        gpp->minprot = diff;
      }
    }
  }
  return TRUE;
}

void FindGeneAndProtForCDS (Uint2 entityID, SeqFeatPtr cds,
                            SeqFeatPtr PNTR gene, SeqFeatPtr PNTR prot)

{
  GeneProtFind  gpf;
  GatherScope   gs;

  if (gene != NULL) {
    *gene = NULL;
  }
  if (prot != NULL) {
    *prot = NULL;
  }
  if (entityID == 0 || cds == NULL) return;
  MemSet ((Pointer) (&gpf), 0, sizeof (GeneProtFind));
  gpf.cds = cds;
  gpf.gene = NULL;
  gpf.prot = NULL;
  gpf.location = cds->location;
  gpf.product = cds->product;
  gpf.mingene = INT4_MAX;
  gpf.minprot = INT4_MAX;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherEntity (entityID, (Pointer) &gpf, GeneProtFindFunc, &gs);
  if (gene != NULL) {
    *gene = gpf.gene;
  }
  if (prot != NULL) {
    *prot = gpf.prot;
  }
}


CharPtr JustSaveStringFromText (TexT t)

{
  size_t   len;
  CharPtr  str;

  len = TextLength (t);
  if (len > 0) {
    str = (CharPtr) MemNew(len + 1);
    if (str != NULL) {
      GetTitle (t, str, len + 1);
      /* TrimSpacesAroundString (str); */
      if (str[0] == 0) {
        str = (CharPtr) MemFree(str);
      }
      return str;
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
}

#define FIND_ASN   1
#define FIND_FLAT  2
#define FIND_GENE  3
#define FIND_PROT  4
#define FIND_POS   5

typedef struct findform {
  FORM_MESSAGE_BLOCK
  TexT            findTxt;
  TexT            replaceTxt;
  ButtoN          caseCounts;
  ButtoN          wholeWord;
  ButtoN          doSeqIdLocal;
  ButtoN          findAllBtn;
  ButtoN          replaceAllBtn;
  Int2            type;
  ButtoN          findFirstBtn;
  ButtoN          findNextBtn;
  Int4            last_paragraph_found;
} FindForm, PNTR FindFormPtr;

extern CharPtr CompressSpaces (CharPtr str)

{
  Char     ch;
  CharPtr  dst;
  Char     last;
  CharPtr  ptr;

  if (str != NULL && str [0] != '\0') {
    dst = str;
    ptr = str;
    ch = *ptr;
    while (ch != '\0' && ch <= ' ') {
      ptr++;
      ch = *ptr;
    }
    while (ch != '\0') {
      *dst = ch;
      dst++;
      ptr++;
      last = ch;
      ch = *ptr;
      if (ch != '\0' && ch < ' ') {
        *ptr = ' ';
        ch = *ptr;
      }
      while (ch != '\0' && last <= ' ' && ch <= ' ') {
        ptr++;
        ch = *ptr;
      }
    }
    *dst = '\0';
    dst = NULL;
    ptr = str;
    ch = *ptr;
    while (ch != '\0') {
      if (ch != ' ') {
        dst = NULL;
      } else if (dst == NULL) {
        dst = ptr;
      }
      ptr++;
      ch = *ptr;
    }
    if (dst != NULL) {
      *dst = '\0';
    }
  }
  return str;
}

extern CharPtr SearchForString (CharPtr str, CharPtr sub, Boolean case_counts, Boolean whole_word)

{
  CharPtr  ptr = NULL;
  CharPtr  tmp;

  if (case_counts) {
    ptr = StringSearch (str, sub);
  } else {
    ptr = StringISearch (str, sub);
  }
  if (ptr == NULL) return NULL;
  if (whole_word) {
    if (ptr > str) {
      tmp = ptr - 1;
      if (! IS_WHITESP (*tmp)) return NULL;
    }
    tmp = ptr + StringLen (sub);
    if (*tmp != '\0' && (! IS_WHITESP (*tmp))) return NULL;
  }
  return ptr;
}


static Int4
FindInFlatFileNext 
(Asn2gbJobPtr ajp,
 CharPtr sub,
 Boolean case_counts,
 Boolean whole_word,
 Boolean stop_after_one_found,
 Int4    start_index)
{
  Int4             index;
  CharPtr          string;
  BaseBlockPtr     bbp;
  SelStructPtr     sel;
  unsigned int     entID;
  unsigned int     itmID;
  unsigned int     itmtype;
  Boolean          already = FALSE;

  if (ajp == NULL) return -1;
      
  for (index = start_index + 1; index < ajp->numParagraphs; index++) {
    string = asn2gnbk_format (ajp, (Int4) index);
    if (string != NULL && *string != '\0') {
      CompressSpaces (string);
      if (SearchForString (string, sub, case_counts, whole_word) != NULL) {
        bbp = ajp->paragraphArray [index];
        if (bbp != NULL) {
          entID = bbp->entityID;
          itmID = bbp->itemID;
          itmtype = bbp->itemtype;
          already = FALSE;
          for (sel = ObjMgrGetSelected (); sel != NULL; sel = sel->next) {
            if (sel->entityID == entID && sel->itemID == itmID && sel->itemtype == itmtype) {
              already = TRUE;
            }
          }
          if (! already) {
            ObjMgrAlsoSelect (entID, itmID, itmtype, 0, NULL);
            if (stop_after_one_found) return index;
          }
        }
      }
    }
  }
  return -1;
}


static Int4 
FindInFlatFile 
(Uint2 entityID,
 Uint2 itemID,
 Uint2 itemtype,                           
 Uint1 format,
 Uint1 mode,                           
 CharPtr sub,
 Boolean case_counts,
 Boolean whole_word,                           
 Boolean stop_after_one,
 Int4 start_index)

{
  Asn2gbJobPtr     ajp;
  BioseqPtr        bsp;
  BioseqSetPtr     bssp;
  ErrSev           level;
  SeqEntryPtr      oldsep;
  SeqEntryPtr      sep = NULL;
  SeqEntryPtr      topsep;
  SeqEntryPtr      usethetop = NULL;
  Int4             retval = -1;

  if (itemID == 0) {
    sep = GetTopSeqEntryForEntityID (entityID);
    usethetop = sep;
  } else {
    bsp = GetBioseqGivenIDs (entityID, itemID, itemtype);
    if (bsp == NULL)
      return retval;
    sep = SeqMgrGetSeqEntryForData (bsp);
    if (bsp->repr == Seq_repr_seg) {
      sep = GetBestTopParentForData (entityID, bsp);
    }
  }

  if (sep == NULL)
    return retval;

  level = ErrSetMessageLevel (SEV_MAX);
  WatchCursor ();
  Update ();

  topsep = GetTopSeqEntryForEntityID (entityID);
  oldsep = SeqEntrySetScope (topsep);

  if (usethetop != NULL && IS_Bioseq_set (usethetop)) {
    bssp = (BioseqSetPtr) usethetop->data.ptrvalue;
    ajp = asn2gnbk_setup (NULL, bssp, NULL, (FmtType) format, (ModType) mode, NORMAL_STYLE, (FlgType) 0, (LckType) 0, (CstType) 0, NULL);
  } else {
    ajp = asn2gnbk_setup (bsp, NULL, NULL, (FmtType) format, (ModType) mode, NORMAL_STYLE, (FlgType) 0, (LckType) 0, (CstType) 0, NULL);
  }
  if (ajp != NULL) {
    retval = FindInFlatFileNext (ajp, sub, case_counts, whole_word, stop_after_one, start_index);
    asn2gnbk_cleanup (ajp);
  }

  SeqEntrySetScope (oldsep);

  ErrSetMessageLevel (level);
  ArrowCursor ();
  Update ();
  return retval;
}

static void FindFlatProc (ButtoN b)

{
  Boolean      caseCounts;
  FindFormPtr  ffp;
  CharPtr      findme;
  Boolean      wholeWord;

  ffp = (FindFormPtr) GetObjectExtra (b);
  if (ffp == NULL) return;
  findme = SaveStringFromText (ffp->findTxt);
  ObjMgrDeSelect (0, 0, 0, 0, NULL);
  caseCounts = GetStatus (ffp->caseCounts);
  wholeWord = GetStatus (ffp->wholeWord);
  CompressSpaces (findme);
  ffp->last_paragraph_found = FindInFlatFile (ffp->input_entityID, ffp->input_itemID,
                  ffp->input_itemtype, GENBANK_FMT, SEQUIN_MODE,
                  findme, caseCounts, wholeWord, FALSE, -1);
  MemFree (findme);
}

static void FindFlatProcFirst (ButtoN b)

{
  Boolean      caseCounts;
  FindFormPtr  ffp;
  CharPtr      findme;
  Boolean      wholeWord;

  ffp = (FindFormPtr) GetObjectExtra (b);
  if (ffp == NULL) return;
  findme = SaveStringFromText (ffp->findTxt);
  ObjMgrDeSelect (0, 0, 0, 0, NULL);
  caseCounts = GetStatus (ffp->caseCounts);
  wholeWord = GetStatus (ffp->wholeWord);
  CompressSpaces (findme);
  ffp->last_paragraph_found = FindInFlatFile (ffp->input_entityID, ffp->input_itemID,
                  ffp->input_itemtype, GENBANK_FMT, SEQUIN_MODE,
                  findme, caseCounts, wholeWord, TRUE, -1);
  MemFree (findme);
}

static void FindFlatProcNext (ButtoN b)

{
  Boolean      caseCounts;
  FindFormPtr  ffp;
  CharPtr      findme;
  Boolean      wholeWord;

  ffp = (FindFormPtr) GetObjectExtra (b);
  if (ffp == NULL) return;
  findme = SaveStringFromText (ffp->findTxt);
  ObjMgrDeSelect (0, 0, 0, 0, NULL);
  caseCounts = GetStatus (ffp->caseCounts);
  wholeWord = GetStatus (ffp->wholeWord);
  CompressSpaces (findme);
  ffp->last_paragraph_found = FindInFlatFile (ffp->input_entityID, ffp->input_itemID,
                  ffp->input_itemtype, GENBANK_FMT, SEQUIN_MODE,
                  findme, caseCounts, wholeWord, TRUE, ffp->last_paragraph_found);
  MemFree (findme);
}

static void FindTextProc (TexT t)

{
  FindFormPtr  ffp;

  ffp = (FindFormPtr) GetObjectExtra (t);
  if (ffp != NULL) {
    if (TextLength (t) > 0) {
      SafeEnable (ffp->findAllBtn);
      SafeEnable (ffp->replaceAllBtn);
      if (ffp->type == FIND_FLAT) {
        SafeEnable (ffp->findFirstBtn);
        SafeEnable (ffp->findNextBtn);
      }
      if (ffp->type == FIND_GENE || ffp->type == FIND_PROT) {
        SafeEnable (ffp->findFirstBtn);
      }
    } else {
      SafeDisable (ffp->findAllBtn);
      SafeDisable (ffp->replaceAllBtn);
      if (ffp->type == FIND_FLAT) {
        SafeDisable (ffp->findFirstBtn);
        SafeDisable (ffp->findNextBtn);
      }
      if (ffp->type == FIND_GENE || ffp->type == FIND_PROT) {
        SafeDisable (ffp->findFirstBtn);
      }
    }
  }
}

static void CommonFindReplaceProc (ButtoN b, Boolean replace, Boolean replaceAll)

{
  Boolean      caseCounts;
  CharPtr      changeme;
  Boolean      doSeqIdLocal;
  FindFormPtr  ffp;
  CharPtr      findme;
  Boolean      wholeWord;

  ffp = (FindFormPtr) GetObjectExtra (b);
  if (ffp != NULL) {
    findme = JustSaveStringFromText (ffp->findTxt);
    ObjMgrDeSelect (0, 0, 0, 0, NULL);
    caseCounts = GetStatus (ffp->caseCounts);
    wholeWord = GetStatus (ffp->wholeWord);
    doSeqIdLocal = GetStatus (ffp->doSeqIdLocal);
    if (replace) {
      changeme = JustSaveStringFromText (ffp->replaceTxt);
      FindReplaceInEntity (ffp->input_entityID, findme, changeme, caseCounts, wholeWord, replaceAll,
                           FALSE, UPDATE_ONCE, NULL, NULL, NULL, doSeqIdLocal);
      GetRidOfEmptyFeatsDescStrings (ffp->input_entityID, NULL);
      ObjMgrSetDirtyFlag (ffp->input_entityID, TRUE);
      ObjMgrSendMsg (OM_MSG_UPDATE, ffp->input_entityID, 0, 0);
      MemFree (changeme);
    } else {
      FindReplaceInEntity (ffp->input_entityID, findme, NULL, caseCounts, wholeWord, FALSE,
                           TRUE, UPDATE_ONCE, NULL, NULL, NULL, doSeqIdLocal);
    }
    MemFree (findme);
    Update ();
  }
}

static void FindAllProc (ButtoN b)

{
  CommonFindReplaceProc (b, FALSE, FALSE);
}

static void ReplaceAllProc (ButtoN b)

{
  CommonFindReplaceProc (b, TRUE, TRUE);
}

static ObjMgrDataPtr SeqMgrGetOmdpForBioseq (BioseqPtr bsp)

{
  ObjMgrDataPtr  omdp;
  ObjMgrPtr      omp;

  if (bsp == NULL) return NULL;
  omdp = (ObjMgrDataPtr) bsp->omdp;
  if (omdp != NULL) return omdp;
  omp = ObjMgrWriteLock ();
  omdp = ObjMgrFindByData (omp, bsp);
  ObjMgrUnlock ();
  bsp->omdp = (Pointer) omdp;
  return omdp;
}

static SeqFeatPtr 
FindNthFeatureOnBspByLabel 
(BioseqPtr              bsp,
 CharPtr                label,
 Uint1                  seqFeatChoice,
 Uint1                  featDefChoice,
 Int4                   n,
 Int4 PNTR              last_found,
 SeqMgrFeatContext PNTR context)
{
  SMFeatItemPtr PNTR  array;
  BioseqExtraPtr      bspextra;
  Uint2               entityID;
  SMFeatItemPtr       feat;
  Int4                L;
  Int4                mid;
  Int4                num;
  ObjMgrDataPtr       omdp;
  Int4                R;
  SeqFeatPtr          sfp;
  Uint1               seqfeattype;
  Int4                index = 0;

  if (context != NULL) {
    MemSet ((Pointer) context, 0, sizeof (SeqMgrFeatContext));
  }
  if (last_found != NULL) {
    *last_found = -1;
  }

  if (bsp == NULL || StringHasNoText (label)) return NULL;

  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return NULL;

  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;
  array = bspextra->featsByLabel;
  num = bspextra->numfeats;

  if (array == NULL || num < 1) return NULL;

  if (n < 0 || n > bspextra->numfeats) return NULL;

  entityID = ObjMgrGetEntityIDForPointer (omdp->dataptr);

  /* binary search to leftmost candidate within the featsByLabel array */

  L = 0;
  R = num - 1;
  while (L < R) {
    mid = (L + R) / 2;
    feat = array [mid];
    if (feat != NULL && StringICmp (feat->label, label) < 0) {
      L = mid + 1;
    } else {
      R = mid;
    }
  }

  feat = array [R];

  /* linear scan to find desired label on desired feature type */

  while (R < num && feat != NULL && StringICmp (feat->label, label) == 0) {
    sfp = feat->sfp;
    if (sfp != NULL) {
      seqfeattype = sfp->data.choice;
      if ((seqFeatChoice == 0 || seqfeattype == seqFeatChoice) &&
          (featDefChoice == 0 || feat->subtype == featDefChoice) &&
          (! feat->ignore)) {
        if (context != NULL) {
          context->entityID = entityID;
          context->itemID = feat->itemID;
          context->sfp = sfp;
          context->sap = feat->sap;
          context->bsp = feat->bsp;
          context->label = feat->label;
          context->left = feat->left;
          context->right = feat->right;
          context->dnaStop = feat->dnaStop;
          context->partialL = feat->partialL;
          context->partialR = feat->partialR;
          context->farloc = feat->farloc;
          context->strand = feat->strand;
          context->seqfeattype = seqfeattype;
          context->featdeftype = feat->subtype;
          context->numivals = feat->numivals;
          context->ivals = feat->ivals;
          context->userdata = NULL;
          context->omdp = (Pointer) omdp;
          context->index = R + 1;
        }
        if (index == n) {
          if (last_found != NULL) {
            *last_found = index;
          }
          return sfp;
        } else {
          if (last_found != NULL) {
            *last_found = index;
          }
          index++;
        }
      }
    }

    R++;
    feat = array [R];
  }

  return NULL;
}

typedef struct findnthfeatdata {
  CharPtr                findme;
  Uint1                  seqFeatChoice;
  Uint1                  featDefChoice;
  Int4                   n;
  Int4                   passed_so_far;
  SeqMgrFeatContext PNTR context;
  SeqFeatPtr             sfp;
} FindNthFeatData, PNTR FindNthFeatPtr;

static void FindNthFeatureByLabelInSeqEntryBspProc (BioseqPtr bsp, Pointer userdata)
{
  FindNthFeatPtr fnfp;
  Int4           this_search_index;
  Int4           passed_this_bsp = 0;

  if (bsp == NULL || (fnfp = (FindNthFeatPtr)userdata) == NULL
      || fnfp->sfp != NULL) {
    return;
  }

  this_search_index = fnfp->n - fnfp->passed_so_far;
  fnfp->sfp = FindNthFeatureOnBspByLabel (bsp,
                                          fnfp->findme,
                                          fnfp->seqFeatChoice,
                                          fnfp->featDefChoice,
                                          this_search_index,
                                          &passed_this_bsp,
                                          fnfp->context);
  if (fnfp->sfp == NULL) {
    fnfp->passed_so_far += passed_this_bsp + 1;
  }
}

static SeqFeatPtr FindNthFeatureByLabelInSeqEntry
(SeqEntryPtr            sep,
 CharPtr                findme,
 Uint1                  seqFeatChoice,
 Uint1                  featDefChoice,
 Int4                   n,
 SeqMgrFeatContext PNTR context)
{
  FindNthFeatData fnf;
  
  fnf.findme = findme;
  fnf.seqFeatChoice = seqFeatChoice;
  fnf.featDefChoice = featDefChoice;
  fnf.n = n;
  fnf.context = context;
  fnf.passed_so_far = 0;
  fnf.sfp = NULL;

  VisitBioseqsInSep (sep, &fnf, FindNthFeatureByLabelInSeqEntryBspProc);

  return fnf.sfp;
}

static void FindByLabelOrPosProc (ButtoN b)

{
  Boolean            already;
  BioseqPtr          bsp;
  SeqMgrFeatContext  context;
  FindFormPtr        ffp;
  CharPtr            findme;
  SelStructPtr       sel;
  SeqEntryPtr        sep = NULL;
  SeqFeatPtr         sfp = NULL;
  Int4               val;

  ffp = (FindFormPtr) GetObjectExtra (b);
  if (ffp == NULL) return;

  if (ffp->input_itemID == 0) {
    sep = GetTopSeqEntryForEntityID (ffp->input_entityID);
  } else {
    bsp = GetBioseqGivenIDs (ffp->input_entityID, ffp->input_itemID, ffp->input_itemtype);
    if (bsp == NULL) return;
    sep = SeqMgrGetSeqEntryForData (bsp);
    if (bsp->repr == Seq_repr_seg) {
      sep = GetBestTopParentForData (ffp->input_entityID, bsp);
    }
  }
  if (sep == NULL) return;

  findme = SaveStringFromText (ffp->findTxt);
  ObjMgrDeSelect (0, 0, 0, 0, NULL);
  CompressSpaces (findme);

  switch (ffp->type) {
    case FIND_GENE :
      sfp = FindNthFeatureByLabelInSeqEntry (sep, findme, SEQFEAT_GENE, 0,
                                             ffp->last_paragraph_found + 1,
                                             &context);
      if (sfp == NULL) {
        ffp->last_paragraph_found = -1;
      } else {
        ffp->last_paragraph_found ++;
      }
      break;
    case FIND_PROT :
      sfp = FindNthFeatureByLabelInSeqEntry (sep, findme, SEQFEAT_CDREGION, 0,
                                             ffp->last_paragraph_found + 1,
                                             &context);
      if (sfp == NULL) {
        ffp->last_paragraph_found = -1;
      } else {
        ffp->last_paragraph_found ++;
      }
      break;
    case FIND_POS :
      if (StrToLong (findme, &val)) {
        if (val > 0 && val <= bsp->length) {
          val--;
          sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &context);
          while (sfp != NULL) {
            if (context.left >= val) break;
            sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &context);
          }
        }
      }
      break;
    default :
      break;
  }

  MemFree (findme);

  if (sfp == NULL) return;

  already = FALSE;
  for (sel = ObjMgrGetSelected (); sel != NULL; sel = sel->next) {
    if (sel->entityID == context.entityID &&
        sel->itemID == context.itemID &&
        sel->itemtype == OBJ_SEQFEAT) {
      already = TRUE;
    }
  }
  if (! already) {
    ObjMgrAlsoSelect (context.entityID, context.itemID, OBJ_SEQFEAT, 0, NULL);
  }

}

static void FindByLabelOrPosProcFindFirst (ButtoN b)
{
  FindFormPtr ffp;

  ffp = (FindFormPtr) GetObjectExtra (b);
  if (ffp == NULL) return;
  ffp->last_paragraph_found = -1;
  FindByLabelOrPosProc (b);
}

static void ClearFindTextProc (ButtoN b)

{
  FindFormPtr  ffp;

  ffp = (FindFormPtr) GetObjectExtra (b);
  if (ffp == NULL) return;
  SafeSetTitle (ffp->findTxt, "");
  SafeSetTitle (ffp->replaceTxt, "");
  if (ffp->type == FIND_FLAT) {
    ObjMgrDeSelect (0, 0, 0, 0, NULL);
    ffp->last_paragraph_found = -1;
  }
  FindTextProc (ffp->findTxt);
  Select (ffp->findTxt);
}

static void FindFormMessage (ForM f, Int2 mssg)

{
  FindFormPtr  ffp;

  ffp = (FindFormPtr) GetObjectExtra (f);
  if (ffp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        Remove (f);
        break;
      case VIB_MSG_CUT :
        StdCutTextProc (NULL);
        break;
      case VIB_MSG_COPY :
        StdCopyTextProc (NULL);
        break;
      case VIB_MSG_PASTE :
        StdPasteTextProc (NULL);
        break;
      case VIB_MSG_DELETE :
        StdDeleteTextProc (NULL);
        break;
      default :
        if (ffp->appmessage != NULL) {
          ffp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static ForM CreateFindForm (Int2 left, Int2 top, CharPtr title,
                            Uint2 entityID, Uint2 itemID, Uint2 itemtype,
                            Int2 type)

{
  ButtoN             b;
  GrouP              c;
  GrouP              g;
  FindFormPtr        ffp;
  GrouP              j;
  GrouP              q = NULL;
  StdEditorProcsPtr  sepp;
  WindoW             w;

  w = NULL;
  ffp = MemNew (sizeof (FindForm));
  if (ffp != NULL) {
    w = FixedWindow (left, top, -10, -10, title, StdCloseWindowProc);
    SetObjectExtra (w, ffp, StdCleanupFormProc);
    ffp->form = (ForM) w;
    ffp->formmessage = FindFormMessage;
    ffp->input_entityID = entityID;
    ffp->input_itemID = itemID;
    ffp->input_itemtype = itemtype;
    ffp->type = type;
    ffp->last_paragraph_found = -1;

#ifndef WIN_MAC
    CreateStdEditorFormMenus (w);
#endif

    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      SetActivate (w, sepp->activateForm);
      ffp->appmessage = sepp->handleMessages;
    }

    j = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (j, 10, 10);

    g = HiddenGroup (j, 2, 0, NULL);
    StaticPrompt (g, "Find", 0, dialogTextHeight, programFont, 'l');
    ffp->findTxt = DialogText (g, "", 25, FindTextProc);
    SetObjectExtra (ffp->findTxt, ffp, NULL);
    if (type == FIND_ASN) {
      StaticPrompt (g, "Replace", 0, dialogTextHeight, programFont, 'l');
      ffp->replaceTxt = DialogText (g, "", 25, NULL);
      SetObjectExtra (ffp->replaceTxt, ffp, NULL);
    }

    if (type == FIND_ASN || type == FIND_FLAT) {
      q = HiddenGroup (w, 3, 0, NULL);
      ffp->caseCounts = CheckBox (q, "Case Sensitive", NULL);
      ffp->wholeWord = CheckBox (q, "Entire Word", NULL);
      if (indexerVersion && type == FIND_ASN) {
        ffp->doSeqIdLocal = CheckBox (q, "SeqID LOCAL", NULL);
      }
    }

    c = HiddenGroup (w, 5, 0, NULL);
    SetGroupSpacing (c, 10, 2);
    if (type == FIND_ASN) {
      ffp->findAllBtn = DefaultButton (c, "Find All", FindAllProc);
      SetObjectExtra (ffp->findAllBtn, ffp, NULL);
      Disable (ffp->findAllBtn);
      ffp->replaceAllBtn = PushButton (c, "Replace All", ReplaceAllProc);
      SetObjectExtra (ffp->replaceAllBtn, ffp, NULL);
      Disable (ffp->replaceAllBtn);
    } else if (type == FIND_FLAT) {
      ffp->findFirstBtn = PushButton (c, "Find First", FindFlatProcFirst);
      SetObjectExtra (ffp->findFirstBtn, ffp, NULL);
      Disable (ffp->findFirstBtn);
      ffp->findNextBtn = PushButton (c, "Find Next", FindFlatProcNext);
      SetObjectExtra (ffp->findNextBtn, ffp, NULL);
      Disable (ffp->findNextBtn);
      ffp->findAllBtn = DefaultButton (c, "Find All", FindFlatProc);
      SetObjectExtra (ffp->findAllBtn, ffp, NULL);
      Disable (ffp->findAllBtn);
    } else if (type == FIND_GENE) {
      ffp->findFirstBtn = DefaultButton (c, "Find First Gene", FindByLabelOrPosProcFindFirst);
      SetObjectExtra (ffp->findFirstBtn, ffp, NULL);
      Disable (ffp->findFirstBtn);
      ffp->findAllBtn = PushButton (c, "Find Next Gene", FindByLabelOrPosProc);
      SetObjectExtra (ffp->findAllBtn, ffp, NULL);
      Disable (ffp->findAllBtn);
    } else if (type == FIND_PROT) {
     ffp->findFirstBtn = DefaultButton (c, "Find First Protein", FindByLabelOrPosProcFindFirst);
      SetObjectExtra (ffp->findFirstBtn, ffp, NULL);
      Disable (ffp->findFirstBtn);
      ffp->findAllBtn = PushButton (c, "Find Next Protein", FindByLabelOrPosProc);
      SetObjectExtra (ffp->findAllBtn, ffp, NULL);
      Disable (ffp->findAllBtn);
    } else if (type == FIND_POS) {
      ffp->findAllBtn = DefaultButton (c, "Find by Position", FindByLabelOrPosProc);
      SetObjectExtra (ffp->findAllBtn, ffp, NULL);
      Disable (ffp->findAllBtn);
    }
    b = PushButton (c, "Clear", ClearFindTextProc);
    SetObjectExtra (b, ffp, NULL);
    PushButton (c, "Cancel", StdCancelButtonProc);

    AlignObjects (ALIGN_CENTER, (HANDLE) j, (HANDLE) c, (HANDLE) q, NULL);

    RealizeWindow (w);
    Select (ffp->findTxt);
  }
  return (ForM) w;
}

extern void FindStringProc (IteM i)

{
  BaseFormPtr  bfp;
  ForM         w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp != NULL) {
    w = CreateFindForm (-90, -66, "Find", bfp->input_entityID,
                        bfp->input_itemID, bfp->input_itemtype, FIND_ASN);
    Show (w);
    Select (w);
  }
}

extern void FindStringProcToolBtn (ButtoN b)

{
  BaseFormPtr  bfp;
  ForM         w;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

  w = CreateFindForm (-90, -66, "Find", bfp->input_entityID,
                      bfp->input_itemID, bfp->input_itemtype, FIND_ASN);
  Show (w);
  Select (w);

}

extern void FindFlatfileProc (IteM i)

{
  BaseFormPtr  bfp;
  ForM         w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp != NULL) {
    w = CreateFindForm (-90, -66, "Flat File Find", bfp->input_entityID,
                        bfp->input_itemID, bfp->input_itemtype, FIND_FLAT);
    Show (w);
    Select (w);
  }
}

extern void FindGeneProc (IteM i)

{
  BaseFormPtr  bfp;
  ForM         w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp != NULL) {
    w = CreateFindForm (-90, -66, "Find", bfp->input_entityID,
                        bfp->input_itemID, bfp->input_itemtype, FIND_GENE);
    Show (w);
    Select (w);
  }
}

extern void FindProtProc (IteM i)

{
  BaseFormPtr  bfp;
  ForM         w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp != NULL) {
    w = CreateFindForm (-90, -66, "Find", bfp->input_entityID,
                        bfp->input_itemID, bfp->input_itemtype, FIND_PROT);
    Show (w);
    Select (w);
  }
}

extern void FindPosProc (IteM i)

{
  BaseFormPtr  bfp;
  ForM         w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp != NULL) {
    w = CreateFindForm (-90, -66, "Find", bfp->input_entityID,
                        bfp->input_itemID, bfp->input_itemtype, FIND_POS);
    Show (w);
    Select (w);
  }
}

extern EnumFieldAssoc  orgmod_subtype_alist [];
extern EnumFieldAssoc  subsource_subtype_alist [];

static void RemoveRedundantSourceNote (CharPtr str, CharPtr keyword, CharPtr name)

{
  Char     ch;
  Boolean  extract;
  CharPtr  tmp1, tmp2;

  if (str == NULL || keyword == NULL || name == NULL) return;
  while (str != NULL && *str != '\0') {
    extract = TRUE;
    tmp1 = str;
    ch = *tmp1;
    while (ch == ' ' || ch == ';') {
      tmp1++;
      ch = *tmp1;
    }
    tmp2 = keyword;
    ch = TO_UPPER (*tmp1);
    while (ch != '\0' && ch == TO_UPPER (*tmp2)) {
      tmp1++;
      tmp2++;
      ch = TO_UPPER (*tmp1);
    }
    if (*tmp2 == '\0' && ch == ' ') {
      while (ch != '\0' && ch == ' ') {
        tmp1++;
        ch = *tmp1;
      }
      tmp2 = name;
      while (ch != '\0' && ch == *tmp2) {
        tmp1++;
        tmp2++;
        ch = *tmp1;
      }
      if (*tmp2 == '\0' && (ch == '\0' || ch == ' ' || ch == ';')) {
        while (ch != '\0' && ch == ' ') {
          tmp1++;
          ch = *tmp1;
        }
        /* now ready to extract */
      } else {
        extract = FALSE;
      }
    } else {
      extract = FALSE;
    }
    if (extract) {
      while (ch == ' ' || ch == ';') {
        tmp1++;
        ch = *tmp1;
      }
      tmp2 = str;
      while (ch != '\0') {
        *tmp2 = ch;
        tmp1++;
        tmp2++;
        ch = *tmp1;
      }
      *tmp2 = '\0';
    } else {
      ch = *tmp1;
      while (ch != '\0' && ch != ';') {
        tmp1++;
        ch = *tmp1;
      }
      if (ch == ';') {
        tmp1++;
        ch = *tmp1;
      }
      while (ch != '\0' && (ch == ' ' || ch == ';')) {
        tmp1++;
        ch = *tmp1;
      }
      str = tmp1;
    }
  }
}

static CharPtr TrimSpacesAndSemicolonsAroundString (CharPtr str)

{
  Uchar    ch;	/* to use 8bit characters in multibyte languages */
  CharPtr  dst;
  CharPtr  ptr;

  if (str != NULL && str [0] != '\0') {
    dst = str;
    ptr = str;
    ch = *ptr;
    while (ch != '\0' && (ch <= ' ' || ch == ';')) {
      ptr++;
      ch = *ptr;
    }
    while (ch != '\0') {
      *dst = ch;
      dst++;
      ptr++;
      ch = *ptr;
    }
    *dst = '\0';
    dst = NULL;
    ptr = str;
    ch = *ptr;
    while (ch != '\0') {
      if (ch != ' ' && ch != ';') {
        dst = NULL;
      } else if (dst == NULL) {
        dst = ptr;
      }
      ptr++;
      ch = *ptr;
    }
    if (dst != NULL) {
      *dst = '\0';
    }
  }
  return str;
}

extern void GetRidOfRedundantSourceNotes (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent);
extern void GetRidOfRedundantSourceNotes (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  EnumFieldAssocPtr  ap;
  BioSourcePtr       biop;
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  OrgModPtr          mod;
  OrgNamePtr         onp;
  OrgRefPtr          orp;
  ValNodePtr         sdp;
  SubSourcePtr       ssp;
  CharPtr            str1, str2;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sdp = bsp->descr;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sdp = bssp->descr;
  } else return;
  while (sdp != NULL) {
    if (sdp->choice == Seq_descr_source) {
      str1 = NULL;
      str2 = NULL;
      onp = NULL;
      biop = (BioSourcePtr) sdp->data.ptrvalue;
      if (biop != NULL) {
        orp = biop->org;
        if (orp != NULL) {
          onp = orp->orgname;
          if (onp != NULL) {
            mod = onp->mod;
            while (mod != NULL) {
              if (mod->subtype == 255) {
                str1 = mod->subname;
              }
              mod = mod->next;
            }
          }
        }
        ssp = biop->subtype;
        while (ssp != NULL) {
          if (ssp->subtype == 255) {
            str2 = ssp->name;
          }
          ssp = ssp->next;
        }
        if (str1 != NULL || str2 != NULL) {
          if (onp != NULL) {
            mod = onp->mod;
            while (mod != NULL) {
              if (mod->subtype != 255) {
                for (ap = orgmod_subtype_alist; ap->name != NULL; ap++) {
                  if (ap->value == mod->subtype) {
                    RemoveRedundantSourceNote (str1, ap->name, mod->subname);
                    RemoveRedundantSourceNote (str2, ap->name, mod->subname);
                  }
                }
              }
              mod = mod->next;
            }
          }
          ssp = biop->subtype;
          while (ssp != NULL) {
            if (ssp->subtype != 255) {
              for (ap = subsource_subtype_alist; ap->name != NULL; ap++) {
                if (ap->value == ssp->subtype) {
                  RemoveRedundantSourceNote (str1, ap->name, ssp->name);
                  RemoveRedundantSourceNote (str2, ap->name, ssp->name);
                }
              }
            }
            ssp = ssp->next;
          }
        }
      }
      TrimSpacesAndSemicolonsAroundString (str1);
      TrimSpacesAndSemicolonsAroundString (str2);
    }
    sdp = sdp->next;
  }
}

typedef struct convaccdata {
  CharPtr        currID;
  CharPtr        newID;
} ConvAccData, PNTR ConvAccPtr;

static void ChangeSeqIdListToAccLocalID (SeqIdPtr sip, CharPtr currID, CharPtr newID)

{
  ObjectIdPtr   oip;

  while (sip != NULL) {
    switch (sip->choice) {
      case SEQID_LOCAL :
        oip = (ObjectIdPtr) sip->data.ptrvalue;
        if (oip != NULL) {
          if (StringCmp (oip->str, currID) == 0) {
            MemFree (oip->str);
            oip->str = StringSave (newID);
          }
        }
        break;
      default :
        break;
    }
    sip = sip->next;
  }
}

static void ChangeSeqLocListToAccLocalID (SeqLocPtr slp, CharPtr currID, CharPtr newID)

{
  SeqLocPtr      loc;
  PackSeqPntPtr  psp;
  SeqBondPtr     sbp;
  SeqIntPtr      sinp;
  SeqIdPtr       sip;
  SeqPntPtr      spp;

  while (slp != NULL) {
    switch (slp->choice) {
      case SEQLOC_NULL :
        break;
      case SEQLOC_EMPTY :
      case SEQLOC_WHOLE :
        sip = (SeqIdPtr) slp->data.ptrvalue;
        ChangeSeqIdListToAccLocalID (sip, currID, newID);
        break;
      case SEQLOC_INT :
        sinp = (SeqIntPtr) slp->data.ptrvalue;
        if (sinp != NULL) {
          sip = sinp->id;
          ChangeSeqIdListToAccLocalID (sip, currID, newID);
        }
        break;
      case SEQLOC_PNT :
        spp = (SeqPntPtr) slp->data.ptrvalue;
        if (spp != NULL) {
          sip = spp->id;
          ChangeSeqIdListToAccLocalID (sip, currID, newID);
        }
        break;
      case SEQLOC_PACKED_PNT :
        psp = (PackSeqPntPtr) slp->data.ptrvalue;
        if (psp != NULL) {
          sip = psp->id;
          ChangeSeqIdListToAccLocalID (sip, currID, newID);
        }
        break;
      case SEQLOC_PACKED_INT :
      case SEQLOC_MIX :
      case SEQLOC_EQUIV :
        loc = (SeqLocPtr) slp->data.ptrvalue;
        while (loc != NULL) {
          ChangeSeqIdListToAccLocalID (loc, currID, newID);
          loc = loc->next;
        }
        break;
      case SEQLOC_BOND :
        sbp = (SeqBondPtr) slp->data.ptrvalue;
        if (sbp != NULL) {
          spp = (SeqPntPtr) sbp->a;
          if (spp != NULL) {
            sip = spp->id;
            ChangeSeqIdListToAccLocalID (sip, currID, newID);
          }
          spp = (SeqPntPtr) sbp->b;
          if (spp != NULL) {
            sip = spp->id;
            ChangeSeqIdListToAccLocalID (sip, currID, newID);
          }
        }
        break;
      case SEQLOC_FEAT :
        break;
      default :
        break;
    }
    slp = slp->next;
  }
}

static void ChangeAlignListToAccLocalID (SeqAlignPtr align, CharPtr currID, CharPtr newID)

{
  DenseDiagPtr  ddp;
  DenseSegPtr   dsp;
  StdSegPtr     ssp;

  if (align == NULL) return;
  if (align->segtype == 1) {
    ddp = (DenseDiagPtr) align->segs;
    if (ddp != NULL) {
      ChangeSeqIdListToAccLocalID (ddp->id, currID, newID);
    }
  } else if (align->segtype == 2) {
    dsp = (DenseSegPtr) align->segs;
    if (dsp != NULL) {
      ChangeSeqIdListToAccLocalID (dsp->ids, currID, newID);
    }
  } else if (align->segtype == 3) {
    ssp = (StdSegPtr) align->segs;
    if (ssp != NULL) {
       ChangeSeqLocListToAccLocalID (ssp->loc, currID, newID);
    }
  }
}

static void CopyAccToLocalCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  ConvAccPtr    cap;
  SeqAlignPtr   sal;
  SeqAnnotPtr   sap;
  SeqFeatPtr    sfp;
  SeqIdPtr      sip;
  SeqLocPtr     slp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  cap = (ConvAccPtr) mydata;
  if (cap == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sip = bsp->id;
    ChangeSeqIdListToAccLocalID (sip, cap->currID, cap->newID);
    SeqMgrReplaceInBioseqIndex (bsp);
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        slp = sfp->location;
        ChangeSeqLocListToAccLocalID (slp, cap->currID, cap->newID);
        slp = sfp->product;
        ChangeSeqLocListToAccLocalID (slp, cap->currID, cap->newID);
        sfp = sfp->next;
      }
    } else if (sap->type == 2) {
      sal = (SeqAlignPtr) sap->data;
      while (sal != NULL) {
        ChangeAlignListToAccLocalID (sal, cap->currID, cap->newID);
        sal = sal->next;
      }
    }
    sap = sap->next;
  }
}

static void RecordAccToConvert (ValNodePtr PNTR vnpp, BioseqPtr bsp, CharPtr newID)

{
  ConvAccPtr   cap;
  ObjectIdPtr  oip;
  SeqIdPtr     sip;

  if (vnpp == NULL || bsp == NULL || StringHasNoText (newID)) return;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_LOCAL) {
      oip = (ObjectIdPtr) sip->data.ptrvalue;
      if (oip != NULL) {
        if (! StringHasNoText (oip->str)) {
          cap = (ConvAccPtr) MemNew (sizeof (ConvAccData));
          if (cap == NULL) return;
          cap->currID = StringSave (oip->str);
          cap->newID = StringSave (newID);
          ValNodeAddPointer (vnpp, 0, (Pointer) cap);
          return;
        }
      }
    }
  }
}

static Boolean IsLegalAccession (CharPtr acnum, Int2 i)

{
  Int2  j;

  if (! isalpha ((Int4)(acnum [0]))) return FALSE;
  if (!(isdigit((Int4)(acnum[1])) && i == 6) && !(isalpha((Int4)(acnum[1])) && i == 8)) return FALSE;
  for (j = 2; j < i; j++) {
    if (!(isdigit((Int4)(acnum[j])))) return FALSE;
  }
  return TRUE;
}

static void FindAccInDefCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  Char         acnum [17];
  BioseqPtr    bsp;
  Int2         i;
  CharPtr      p;
  Char         str [128];
  ValNodePtr   ttl;
  ValNodePtr   PNTR vnpp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  if (! IS_Bioseq (sep)) return;
  vnpp = (ValNodePtr PNTR) mydata;
  if (vnpp == NULL) return;
  ttl = SeqEntryGetSeqDescr (sep, Seq_descr_title, NULL);
  while (ttl != NULL) {
    StringNCpy_0 (str, (CharPtr) ttl->data.ptrvalue, sizeof (str));
    TrimSpacesAroundString (str);
    if (! StringHasNoText (str)) {
      if (str [0] == 'a' && str [1] == 'c' && str [2] == 'c') {
        p = str + 3;
        for (i = 0; isalnum ((Int4)(*p)) && *p != '\0'; p++, i++) {
          acnum [i] = *p;
        }
        acnum [i] = '\0';
        if (i == 6 || i == 8) {
          if (IsLegalAccession (acnum, i)) {
            bsp = (BioseqPtr) sep->data.ptrvalue;
            RecordAccToConvert (vnpp, bsp, str);
          }
        }
      }
    }
    ttl = SeqEntryGetSeqDescr (sep, Seq_descr_title, ttl);
  }
}

static void ConvertAccInDefToLocalIdProc (SeqEntryPtr sep)

{
  ConvAccPtr  cap;
  ValNodePtr  head;
  ValNodePtr  vnp;

  if (sep == NULL) return;
  head = NULL;
  SeqEntryExplore (sep, (Pointer) &head, FindAccInDefCallback);
  if (head == NULL) return;
  if (Message (MSG_YN, "Convert accLNNNNN or accLLNNNNNN in title to local ID?") == ANS_NO) return;
  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    cap = (ConvAccPtr) vnp->data.ptrvalue;
    if (cap != NULL) {
      SeqEntryExplore (sep, (Pointer) cap, CopyAccToLocalCallback);
    }
  }
  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    cap = (ConvAccPtr) vnp->data.ptrvalue;
    if (cap != NULL) {
      MemFree (cap->currID);
      MemFree (cap->newID);
      vnp->data.ptrvalue = MemFree (cap);
    }
  }
  ValNodeFree (head);
}

static Int2  taxonCount;

static Int4 DoSeqEntryToAsn3 (SeqEntryPtr sep, Boolean strip, Boolean correct,
                              Boolean force, Boolean dotaxon, MonitorPtr mon)

{
  BioseqSetPtr  bssp;
  SeqEntryPtr   oldscope;
  Int4          rsult;
  Char          str [32];

  rsult = 0;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (IsPopPhyEtcSet (bssp->_class)))) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        rsult += DoSeqEntryToAsn3 (sep, strip, correct, force, dotaxon, mon);
      }
      return rsult;
    }
  }
/*#ifdef USE_TAXON*/
  if (dotaxon && mon != NULL) {
    taxonCount++;
    sprintf (str, "Processing Component %d", (int) taxonCount);
    MonitorStrValue (mon, str);
  }
/*#endif*/
/*#ifdef INTERNAL_NCBI_SEQUIN*/
  if (indexerVersion) {
    if ((! force) && (! NoBiosourceOrTaxonId (sep))) return 0;
  } else {
/*#else*/
    if ((! force) && SeqEntryGetSeqDescr (sep, Seq_descr_source, NULL) != NULL) return 0;
  }
/*#endif*/
  oldscope = SeqEntrySetScope (sep);
  if (dotaxon) {
    rsult = SeqEntryToAsn3Ex (sep, strip, correct, TRUE, NULL, Tax3MergeSourceDescr);
    DeleteMarkedObjects (0, OBJ_SEQENTRY, sep);
  } else {
    rsult = SeqEntryToAsn3Ex (sep, strip, correct, FALSE, NULL, NULL);
  }
  SeqEntrySetScope (oldscope);
  return rsult;
}

/* CheckSeqAlignCallback copied from salsa.c */
static void CheckSeqAlignCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)
{
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  SeqAnnotPtr        sap,
                     pre;

  if (sep != NULL && sep->data.ptrvalue) {
     if (IS_Bioseq(sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp!=NULL) {
           pre=NULL;
           sap=bsp->annot;
           while (sap!= NULL)
           {
              if (sap->type == 2) {
                 if (is_dim1seqalign ((SeqAlignPtr) sap->data)) {
                    if (pre==NULL) {
                       bsp->annot=sap->next;
                       sap->next=NULL;
                       SeqAnnotFree (sap);
                       sap=bsp->annot; 
                    }
                    else {
                       pre->next=sap->next;
                       pre=sap->next;
                       sap->next=NULL; 
                       SeqAnnotFree (sap);
                       sap=pre;
                    }
                 }
                 else {
                    pre=sap;
                    sap=sap->next;
                 }
              }
              else {
                 pre=sap;
                 sap=sap->next;
              }
           }
        }
     }
     else if(IS_Bioseq_set(sep)) {
        bssp = (BioseqSetPtr)sep->data.ptrvalue;
        if (bssp!=NULL) {
           pre=NULL;
           sap=bssp->annot;
           while (sap!= NULL)
           {
              if (sap->type == 2) {
                 if (is_dim1seqalign ((SeqAlignPtr) sap->data)) {
                    if (pre==NULL) {
                       bssp->annot=sap->next;
                       sap->next=NULL;
                       SeqAnnotFree (sap);
                       sap=bssp->annot; 
                    }
                    else {
                       pre=sap->next;
                       sap->next=NULL; 
                       SeqAnnotFree (sap);
                       sap=sap->next;
                    }
                 }
                 else {
                    pre=sap;
                    sap=sap->next;
                 }
              }
              else {
                 pre=sap;
                 sap=sap->next;
              }
           }
        }
     }
  }
}

/* RemoveMultipleTitles currently removes FIRST title in chain */

static void RemoveMultipleTitles (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  SeqDescrPtr    descr = NULL;
  SeqDescrPtr    lasttitle = NULL;
  ObjValNodePtr  ovp;
  SeqDescrPtr    sdp;

  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp == NULL) return;
    descr = bsp->descr;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL) return;
    descr = bssp->descr;
  } else return;
  for (sdp = descr; sdp != NULL; sdp = sdp->next) {
    if (sdp->choice != Seq_descr_title) continue;
    if (lasttitle != NULL) {
      if (lasttitle->extended != 0) {
        ovp = (ObjValNodePtr) lasttitle;
        ovp->idx.deleteme = TRUE;
      }
      lasttitle = sdp;
    } else {
      lasttitle = sdp;
    }
  }
}

typedef struct featcount {
  Boolean     is_mRNA;
  BioseqPtr   bsp;
  Int2        numRNAs;
  SeqFeatPtr  gene;
  Int4        numGene;
  Int4        numCDS;
} FeatCount, PNTR FeatCountPtr;

static void CountGenesAndCDSs (SeqFeatPtr sfp, Pointer userdata)

{
  FeatCountPtr  fcp;

  fcp = (FeatCountPtr) userdata;
  if (sfp->data.choice == SEQFEAT_GENE) {
    (fcp->numGene)++;
    fcp->gene = sfp;
  } else if (sfp->data.choice == SEQFEAT_CDREGION) {
    (fcp->numCDS)++;
  }
}

static void LookForMrna (BioseqPtr bsp, Pointer userdata)

{
  FeatCountPtr  fcp;
  MolInfoPtr    mip;
  SeqDescrPtr   sdp;

  if (bsp == NULL || bsp->length == 0) return;
  if (! ISA_na (bsp->mol)) return;
  fcp = (FeatCountPtr) userdata;
  for (sdp = bsp->descr; sdp != NULL; sdp = sdp->next) {
    if (sdp->choice != Seq_descr_molinfo) continue;
    mip = (MolInfoPtr) sdp->data.ptrvalue;
    if (mip != NULL && mip->biomol == MOLECULE_TYPE_MRNA) {
      fcp->is_mRNA = TRUE;
      fcp->bsp = bsp;
      (fcp->numRNAs)++;
      return;
    }
  }
}

static void ExtendSingleGeneOnMRNA (SeqEntryPtr sep, Pointer userdata)

{
  FeatCount   fc;
  SeqIntPtr   sintp;
  ValNodePtr  vnp;

  fc.is_mRNA = FALSE;
  fc.bsp = NULL;
  fc.numRNAs = 0;
  VisitBioseqsInSep (sep, (Pointer) &fc, LookForMrna);
  if (! fc.is_mRNA) return;
  fc.gene = NULL;
  fc.numGene = 0;
  fc.numCDS = 0;
  VisitFeaturesInSep (sep, (Pointer) &fc, CountGenesAndCDSs);
  if (fc.numGene == 1 && fc.numCDS < 2 && fc.numRNAs == 1 &&
      fc.bsp != NULL && fc.gene != NULL) {
    if (fc.bsp != BioseqFindFromSeqLoc (fc.gene->location)) return;
    for (vnp = fc.gene->location; vnp != NULL; vnp = vnp->next) {
      if (vnp->choice != SEQLOC_INT) continue;
      sintp = (SeqIntPtr) vnp->data.ptrvalue;
      if (sintp == NULL) continue;
      if (sintp->from != 0 || sintp->to != fc.bsp->length - 1) {
        sintp->from = 0;
        sintp->to = fc.bsp->length - 1;
      }
    }
  }
}

extern Int4 MySeqEntryToAsn3Ex (SeqEntryPtr sep, Boolean strip, Boolean correct, Boolean force, Boolean dotaxon);
extern Int4 MySeqEntryToAsn3Ex (SeqEntryPtr sep, Boolean strip, Boolean correct, Boolean force, Boolean dotaxon)

{
  Uint2       entityID;
  MonitorPtr  mon;
  Boolean     needstaxfix;
  Int4        rsult;
  ErrSev      sev;

  rsult = 0;
  sev = ErrSetMessageLevel (SEV_FATAL);
  BasicSeqEntryCleanup (sep);
  EntryChangeImpFeat(sep);     /* change any CDS ImpFeat to real CdRegion */
  /* NormalizePeriodsOnInitials (sep); */ /* put periods on author initials */
  /* MoveRnaGBQualProductToName (sep); */ /* move rna gbqual product to rna-ref.ext.name */
  /* MoveProtGBQualProductToName (sep); */ /* move prot gbqual product to prot-ref.name */
  /* MoveCdsGBQualProductToName (sep); */ /* move cds gbqual product to prot-ref.name */
  /* MoveFeatGBQualsToFields (sep); */ /* move feature partial, exception to fields */
  if (indexerVersion) {
    /*
    StripTitleFromProtsInNucProts (sep);
    */
    move_cds (sep); /* move CDS features to nuc-prot set */
  }
  /* ExtendGeneFeatIfOnMRNA (0, sep); */ /* gene on mRNA is full length */
  VisitElementsInSep (sep, NULL, ExtendSingleGeneOnMRNA); /* needs < 2 CDS */
  RemoveBioSourceOnPopSet (sep, NULL);
  /* SeqEntryExplore (sep, NULL, CleanupEmptyFeatCallback); */
  SeqEntryExplore (sep, NULL, DeleteMultipleTitles); /* do it old way in Sequin */
  /*
  SeqEntryExplore (sep, NULL, RemoveMultipleTitles);
  DeleteMarkedObjects (0, OBJ_SEQENTRY, (Pointer) sep);
  */
  SeqEntryPack (sep);
  if (indexerVersion) {
    ConvertAccInDefToLocalIdProc (sep); /* if title is accXNNNNN, convert to seqID */
  }
  if (useEntrez) {
    ValidateSeqAlignandACCInSeqEntry (sep, TRUE, FALSE, FALSE, TRUE, TRUE, FALSE); /* remove components with seqID accXNNNNN */
  }
  SeqEntryExplore (sep, NULL, CheckSeqAlignCallback); /* remove alignments with single dimension */
  needstaxfix = FALSE;
  if (! force) {
    needstaxfix = NoBiosourceOrTaxonId (sep);
  }
  if ((! force) && (! needstaxfix)) {
    ConvertFullLenSourceFeatToDesc (sep);
    ConvertFullLenPubFeatToDesc (sep);
    /* EntryStripSerialNumber(sep); */ /* strip citation serial numbers */
    EntryChangeGBSource (sep);   /* at least remove redundant information in GBBlocks */
    EntryCheckGBBlock (sep);
    /* SeqEntryMoveDbxrefs (sep); */ /* db_xref gbqual to sfp->dbxref */
    EntryMergeDupBioSources (sep);
    GetRidOfEmptyFeatsDescStrings (0, sep);
    GetRidOfLocusInSeqIds (0, sep);
    /* reindex, since CdEndCheck (from CdCheck) gets best overlapping gene */
    entityID = ObjMgrGetEntityIDForChoice (sep);
    SeqMgrIndexFeatures (entityID, NULL);
    CdCheck (sep, NULL);
    BasicSeqEntryCleanup (sep);
    ErrSetMessageLevel (sev);
    return rsult;
  }
  if (force && useTaxon) {
    dotaxon = TRUE;
  }
  mon = NULL;
  taxonCount = 0;
/*#ifdef USE_TAXON*/
  if (dotaxon) {
    WatchCursor ();
    mon = MonitorStrNewEx ("Taxonomy Lookup", 40, FALSE);
    MonitorStrValue (mon, "Processing Organism Info");
    Update ();
  }
/*#endif*/

  /* set dirty flag if no lineage or division in any biosource */
  if (dotaxon && needstaxfix) {
    entityID = ObjMgrGetEntityIDForChoice (sep);
    ObjMgrSetDirtyFlag (entityID, TRUE);
  }

  EntryMergeDupBioSources (sep); /* do before and after SE2A3 */

  if (dotaxon) {
    Taxon3ReplaceOrgInSeqEntry (sep, FALSE);
  }
  rsult = DoSeqEntryToAsn3 (sep, strip, correct, force, dotaxon, mon);
/*#ifdef USE_TAXON*/
  if (dotaxon) {
    MonitorStrValue (mon, "Closing Taxon");
    Update ();
    MonitorFree (mon);
    ArrowCursor ();
    Update ();
  }
/*#endif*/
  ConvertFullLenSourceFeatToDesc (sep);
  ConvertFullLenPubFeatToDesc (sep);
  /* EntryStripSerialNumber(sep); */ /* strip citation serial numbers */
  MovePopPhyMutPubs (sep);
  EntryChangeGBSource (sep);   /* remove redundant information in GBBlocks again */
  EntryCheckGBBlock (sep);
  /* SeqEntryMoveDbxrefs (sep); */ /* db_xref gbqual to sfp->dbxref */
  EntryMergeDupBioSources (sep);
  GetRidOfEmptyFeatsDescStrings (0, sep);
  GetRidOfLocusInSeqIds (0, sep);
  /* reindex, since CdEndCheck (from CdCheck) gets best overlapping gene */
  entityID = ObjMgrGetEntityIDForChoice (sep);
  SeqMgrIndexFeatures (entityID, NULL);
  CdCheck (sep, NULL);
  BasicSeqEntryCleanup (sep);
  ErrSetMessageLevel (sev);
  ErrClear ();
  ErrShow ();
  return rsult;
}

typedef struct partialformdata {
  FEATURE_FORM_BLOCK

  ValNodePtr         featlist;
  LisT               feature;
  PopuP              change5;
  PopuP              change3;
  PopuP              orderJoinState;
  GrouP              nucprotchoice;
  TexT               findThis;
  CharPtr            findThisStr;
  Int2               subtype;
  Int2               leftpolicy;
  Int2               rightpolicy;
  Int2               nucprotpolicy;
  Int2               orderjoinpolicy;
  ObjMgrPtr          omp;
  ObjMgrTypePtr      omtp;
  ButtoN             extend5_btn;
  ButtoN             extend3_btn;
  Boolean            extend5;
  Boolean            extend3;
  ButtoN             leaveDlgUp;
  Boolean            case_insensitive;
  ButtoN             case_insensitive_btn;
  Boolean            when_string_not_present;
  ButtoN             when_string_not_present_btn;
} PartialFormData, PNTR PartialFormPtr;

static Boolean CDSMeetsStringConstraint (SeqFeatPtr sfp,
				      CharPtr     findThisStr,
				      Boolean     case_insensitive)
{
  BioseqPtr		protbsp;
  SeqFeatPtr		protsfp;
  SeqMgrFeatContext	context;
  ProtRefPtr		prp;

  if (sfp == NULL) return FALSE;
  protbsp = BioseqFindFromSeqLoc (sfp->product);
  if (protbsp == NULL) return FALSE;
  protsfp = SeqMgrGetBestProteinFeature (protbsp, &context);
  if ((case_insensitive && StringISearch (context.label, findThisStr) != NULL)
    || (!case_insensitive && StringSearch (context.label, findThisStr) != NULL))
  {
    return TRUE;
  }
  if (protsfp == NULL) return FALSE;
  prp = (ProtRefPtr) protsfp->data.value.ptrvalue;
  if (prp->name != NULL)
  {
    if ((case_insensitive && StringISearch (prp->name->data.ptrvalue, findThisStr) != NULL)
      || (!case_insensitive && StringSearch (prp->name->data.ptrvalue, findThisStr) != NULL))
    {
      return TRUE;
    }
  }
  if (prp->desc != NULL)
  {
  	if ((case_insensitive && StringISearch (prp->desc, findThisStr) != NULL)
  	  || (!case_insensitive && StringSearch (prp->desc, findThisStr) != NULL))
  	{
  	  return TRUE;
  	}
  }
  return FALSE;
}

extern Boolean MeetsStringConstraint (SeqFeatPtr  sfp,
				                      CharPtr     findThisStr,
				                      Boolean     case_insensitive)
{
  GBQualPtr         gbqp;
  GeneRefPtr        grp;
  RnaRefPtr         rrp;
  SeqMgrFeatContext context;
  Boolean           have_context = FALSE;

  /* If no string constraint, then everyone matches */

  if (NULL == findThisStr)
    return TRUE;

  /* Search for the string constraint */
  /* in the feature title field */
  if (StringISearch (sfp->title, findThisStr))
    return TRUE;

  /* Search for the string constraint */
  /* in the feature comment field.    */

  if (StringISearch (sfp->comment, findThisStr))
    return TRUE;

  /* Search for the string constraint */
  /* in GB qualifiers.                */

  gbqp = sfp->qual;
  while (NULL != gbqp)
    {
      if ((NULL != gbqp->val) && StringISearch (gbqp->val, findThisStr))
	return TRUE;
      gbqp = gbqp->next;
    }

  if (SeqMgrGetDesiredFeature (sfp->idx.entityID, NULL, 0, 0, sfp, &context) != NULL)
  {
    if (!case_insensitive && StringSearch (context.label, findThisStr) != NULL)
    {
    	return TRUE;
    }
    else if (case_insensitive && StringISearch (context.label, findThisStr) != NULL)
  	{
  	  return TRUE;
  	}
  	have_context = TRUE;
  }

  if (sfp->data.choice == SEQFEAT_GENE)
  {
    grp = sfp->data.value.ptrvalue;
    if (!case_insensitive && 
        (StringSearch (grp->locus, findThisStr) != NULL
        || StringSearch (grp->desc, findThisStr) != NULL
        || StringSearch (grp->desc, findThisStr) != NULL
        || StringSearch (grp->locus_tag, findThisStr) != NULL))
    {
      return TRUE;    	
    }
    else if (case_insensitive &&
        (StringISearch (grp->locus, findThisStr) != NULL
        || StringISearch (grp->desc, findThisStr) != NULL
        || StringISearch (grp->desc, findThisStr) != NULL
        || StringISearch (grp->locus_tag, findThisStr) != NULL))
    {
      return TRUE;    	
    }
  }
  else if (sfp->data.choice == SEQFEAT_CDREGION)
  {
    if (CDSMeetsStringConstraint (sfp, findThisStr, case_insensitive))
      return TRUE;
  }
  else if (sfp->data.choice == SEQFEAT_RNA)
  {
    rrp = sfp->data.value.ptrvalue;

    if (rrp->ext.choice == 1) {
      if ((!case_insensitive && StringSearch ((CharPtr) rrp->ext.value.ptrvalue, findThisStr) != NULL)
        || (case_insensitive && StringISearch ((CharPtr) rrp->ext.value.ptrvalue, findThisStr) != NULL))
      {
        return TRUE;
      }
    }
    else if (rrp->type == 3 && rrp->ext.choice == 2 && have_context) 
    {
      /* look for the label as it appears to the user */
      if ((!case_insensitive && StringNCmp(findThisStr, "tRNA-", 5) == 0
          && StringSearch (context.label, findThisStr + 5))
          || (case_insensitive && StringNICmp (findThisStr, "tRNA-", 5) == 0
          && StringISearch (context.label, findThisStr + 5)))
      {
      	return TRUE;
      }
    }
  }

  /* If we got to here, then the string constraint was not found */

  return FALSE;
}


extern void SetBestFrame (SeqFeatPtr sfp)
{
  SeqAlignPtr  salp;
  CdRegionPtr  crp;
  BioseqPtr    old_prot, new_prot;
  ByteStorePtr bs = NULL;
  Int4         original_frame, test_frame;
  Boolean      revcomp;
  ErrSev       level;
  CharPtr      seq_str1, seq_str2;
  Int4         best_len = -1, new_aln_len;
  Int4         best_frame = -1;
  
  if (sfp == NULL || sfp->idx.subtype != FEATDEF_CDS) return;
  
  crp = sfp->data.value.ptrvalue;
  if (crp == NULL) return;

  old_prot = BioseqFindFromSeqLoc (sfp->product);
  if (old_prot == NULL) return;
  
  new_prot = BioseqNew ();
  new_prot->id = SeqIdParse ("lcl|CdRgnTransl");
  new_prot->repr = Seq_repr_raw;
  new_prot->mol = Seq_mol_aa;
  new_prot->seq_data_type = Seq_code_ncbieaa;
  bs = ProteinFromCdRegionEx (sfp, TRUE, FALSE);
  new_prot->seq_data = bs;
  new_prot->length = BSLen (bs);
  
  original_frame = crp->frame;

  /* suppress BLAST error messages when no similarity is found */
  level = ErrSetMessageLevel (SEV_MAX);
  seq_str1 = BSMerge((ByteStorePtr)(old_prot->seq_data), NULL);
  seq_str2 = BSMerge((ByteStorePtr)(new_prot->seq_data), NULL);
  
  for (test_frame = 1; test_frame <= 3; test_frame ++)
  {
    new_prot->seq_data = BSFree (new_prot->seq_data);
    crp->frame = test_frame;
    new_prot->seq_data = ProteinFromCdRegionEx (sfp, TRUE, FALSE);
    salp = Sequin_GlobalAlign2Seq (old_prot, new_prot, &revcomp);
    if (salp != NULL)
    {
      new_aln_len = SeqAlignLength (salp);
      if (new_aln_len > best_len)
      {
        best_len = new_aln_len;
        best_frame = test_frame;
      }
      salp = SeqAlignFree (salp);
    }
  }  	
  
  if (best_frame > -1)
  {
    crp->frame = best_frame;
  }
  else
  {
    crp->frame = original_frame;
  }

  ErrSetMessageLevel (level);
  BioseqFree (new_prot);
}


/*
static Boolean AddOrgToDefGatherFunc (GatherContextPtr gcp)

{
  CharPtr     def;
  CharPtr     ptr;
  ValNodePtr  sdp;
  CharPtr     str;
  CharPtr     text;

  if (gcp == NULL || gcp->thisitem == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQDESC) return TRUE;
  text = (CharPtr) gcp->userdata;
  if (text == NULL || StringHasNoText (text)) return TRUE;
  sdp = (ValNodePtr) gcp->thisitem;
  if (sdp->choice != Seq_descr_title) return TRUE;
  def = (CharPtr) sdp->data.ptrvalue;
  if (StringHasNoText (def)) return TRUE;

  ptr = StringISearch (def, text);
  if (ptr != NULL && ptr == def) return TRUE;
  str = MemNew ((StringLen (text) + StringLen (def) + 4) * sizeof (Char));
  if (str != NULL) {
    StringCpy (str, text);
    StringCat (str, " ");
    StringCat (str, def);
    sdp->data.ptrvalue = MemFree (sdp->data.ptrvalue);
    sdp->data.ptrvalue = str;
    ObjMgrSetDirtyFlag (gcp->entityID, TRUE);
  }
  return TRUE;
}
*/

static void AppendOrgToString (Uint2 entityID, SeqDescrPtr sdp, CharPtr text)

{
  CharPtr     def;
  CharPtr     ptr;
  CharPtr     str;

  def = (CharPtr) sdp->data.ptrvalue;
  if (StringHasNoText (def)) return;

  ptr = StringISearch (def, text);
  if (ptr != NULL && ptr == def) return;
  str = MemNew ((StringLen (text) + StringLen (def) + 4) * sizeof (Char));
  if (str != NULL) {
    StringCpy (str, text);
    StringCat (str, " ");
    StringCat (str, def);
    sdp->data.ptrvalue = MemFree (sdp->data.ptrvalue);
    sdp->data.ptrvalue = str;
    ObjMgrSetDirtyFlag (entityID, TRUE);
  }
}

static void AddOrgToDefElement (Uint2 entityID, SeqEntryPtr sep, Int2 orgmod, Int2 subsource)

{
  EnumFieldAssocPtr  ap;
  BioSourcePtr       biop;
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  Char               ch;
  SeqMgrDescContext  dcontext;
  OrgModPtr          mod;
  OrgNamePtr         onp;
  OrgRefPtr          orp;
  CharPtr            ptr;
  SeqDescrPtr        sdp;
  SubSourcePtr       ssp;
  Char               str [96];
  Char               text [64];
  CharPtr            title;

  if (sep == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 1 || bssp->_class == 2 ||
                         bssp->_class == 4)) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        AddOrgToDefElement (entityID, sep, orgmod, subsource);
      }
      return;
    }
  }
  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  biop = NULL;
  text [0] = '\0';
  str [0] = '\0';
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
  if (sdp != NULL) {
    biop = (BioSourcePtr) sdp->data.ptrvalue;
  }
  if (biop == NULL) return;
  /* SeqEntryToBioSource (sep, NULL, str, sizeof (str) - 1, &biop); */
  if (orgmod == 0 && subsource == 0) {
    orp = biop->org;
    if (orp == NULL) return;
    StringNCpy_0 (str, orp->taxname, sizeof (str));
    /*
    ptr = StringSearch (str, "(");
    if (ptr != NULL) {
      *ptr = '\0';
    }
    */
    TrimSpacesAroundString (str);
    if ((StringICmp (str, "Human immunodeficiency virus type 1") == 0) ||
	(StringICmp (str, "Human immunodeficiency virus 1") == 0)) {
      StringCpy (str, "HIV-1");
    } else if ((StringICmp (str,"Human immunodeficiency virus type 2")==0) ||
	       (StringICmp (str,"Human immunodeficiency virus 2") == 0)) {
      StringCpy (str, "HIV-2");
    }
    str [0] = TO_UPPER (str [0]);
  } else if (biop != NULL && biop->org != NULL) {
    text [0] = '\0';
    str [0] = '\0';
    orp = biop->org;
    if (orgmod > 0) {
      onp = orp->orgname;
      if (onp != NULL) {
        mod = onp->mod;
        while (mod != NULL) {
          if (mod->subtype == orgmod) {
            StringNCpy_0 (text, mod->subname, sizeof (text));
            for (ap = orgmod_subtype_alist; ap->name != NULL; ap++) {
              if (ap->value == orgmod) {
                StringNCpy_0 (str, ap->name, sizeof (str));
              }
            }
          }
          mod = mod->next;
        }
      }
    } else if (subsource > 0) {
      ssp = biop->subtype;
      while (ssp != NULL) {
        if (ssp->subtype == subsource) {
          StringNCpy_0 (text, ssp->name, sizeof (text));
          for (ap = subsource_subtype_alist; ap->name != NULL; ap++) {
            if (ap->value == subsource) {
              StringNCpy_0 (str, ap->name, sizeof (str));
            }
          }
        }
        ssp = ssp->next;
      }
    }
    if (StringHasNoText (text)) {
      str [0] = '\0';
      text [0] = '\0';
    } else {
      StringCat (str, " ");
      ptr = str;
      while (*ptr != '\0') {
        ch = *ptr;
        *ptr = TO_LOWER (ch);
        ptr++;
      }
      StringCat (str, text);
    }
  }
  /*
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.ignore[OBJ_SEQDESC] = FALSE;
  gs.scope = sep;
  GatherSeqEntry (sep, (Pointer) str, AddOrgToDefGatherFunc, &gs);
  */
  sdp = SeqEntryGetSeqDescr (sep, Seq_descr_title, NULL);
  if (sdp == NULL) {
    sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_title, &dcontext);
    if (sdp == NULL) return;
    title = (CharPtr) sdp->data.ptrvalue;
    if (title == NULL) return;
    sdp = SeqDescrAdd (&(bsp->descr));
    if (sdp == NULL) return;
    sdp->choice = Seq_descr_title;
    sdp->data.ptrvalue = StringSave (title);
  }
  if (sdp == NULL) return;
  AppendOrgToString (entityID, sdp, str);
}

static void AddOrgToDef (Uint2 entityID, SeqEntryPtr sep, Int2 orgmod, Int2 subsource)

{
  BioseqSetPtr       bssp;

  if (sep == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (IsPopPhyEtcSet (bssp->_class)))) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        AddOrgToDef (entityID, sep, orgmod, subsource);
      }
      return;
    }
  }
  AddOrgToDefElement (entityID, sep, orgmod, subsource);
}

extern void CommonAddOrgOrModsToDefLines (IteM i, Int2 orgmod, Int2 subsource, ButtoN b)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

  if (b != NULL) {
    bfp = GetObjectExtra (b);
  } else {
#ifdef WIN_MAC
    bfp = currentFormDataPtr;
#else
    bfp = GetObjectExtra (i);
#endif
  }
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  WatchCursor ();
  Update ();
  AddOrgToDef (bfp->input_entityID, sep, orgmod, subsource);
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void RemoveAlignmentCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  SeqAlignPtr    nextsalp;
  SeqAnnotPtr    nextsap;
  Pointer PNTR   prevsalp;
  Pointer PNTR   prevsap;
  SeqAlignPtr    salp;
  SeqAnnotPtr    sap;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
    prevsap = (Pointer PNTR) &(bsp->annot);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
    prevsap = (Pointer PNTR) &(bssp->annot);
  } else return;
  while (sap != NULL) {
    nextsap = sap->next;
    if (sap->type == 2) {
      salp = (SeqAlignPtr) sap->data;
      prevsalp = (Pointer PNTR) &(sap->data);
      while (salp != NULL) {
        nextsalp = salp->next;
        if (salp->type <= 255) {
          *(prevsalp) = salp->next;
          salp->next = NULL;
          SeqAlignFree (salp);
        } else {
          prevsalp = (Pointer PNTR) &(salp->next);
        }
        salp = nextsalp;
      }
    }
    if (sap->data == NULL) {
      *(prevsap) = sap->next;
      sap->next = NULL;
      SeqAnnotFree (sap);
    } else {
      prevsap = (Pointer PNTR) &(sap->next);
    }
    sap = nextsap;
  }
}

extern void RemoveAlignment (IteM i)

{
  BaseFormPtr bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  SeqEntryExplore (sep, NULL, RemoveAlignmentCallback);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ObjMgrDeSelect (0, 0, 0, 0, NULL);
  Update ();
}

static void RemoveGraphCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  SeqAnnotPtr    nextsap;
  SeqGraphPtr    nextsgp;
  Pointer PNTR   prevsap;
  Pointer PNTR   prevsgp;
  SeqAnnotPtr    sap;
  SeqGraphPtr    sgp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
    prevsap = (Pointer PNTR) &(bsp->annot);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
    prevsap = (Pointer PNTR) &(bssp->annot);
  } else return;
  while (sap != NULL) {
    nextsap = sap->next;
    if (sap->type == 3) {
      sgp = (SeqGraphPtr) sap->data;
      prevsgp = (Pointer PNTR) &(sap->data);
      while (sgp != NULL) {
        nextsgp = sgp->next;
        if (sgp->flags [2] >= 1 && sgp->flags [2] <= 3) {
          *(prevsgp) = sgp->next;
          sgp->next = NULL;
          SeqGraphFree (sgp);
        } else {
          prevsgp = (Pointer PNTR) &(sgp->next);
        }
        sgp = nextsgp;
      }
    }
    if (sap->data == NULL) {
      *(prevsap) = sap->next;
      sap->next = NULL;
      SeqAnnotFree (sap);
    } else {
      prevsap = (Pointer PNTR) &(sap->next);
    }
    sap = nextsap;
  }
}

extern void RemoveGraph (IteM i)

{
  BaseFormPtr bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  SeqEntryExplore (sep, NULL, RemoveGraphCallback);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ObjMgrDeSelect (0, 0, 0, 0, NULL);
  Update ();
}

static void MarkProteinCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr        bsp;
  ValNodePtr PNTR  vnpp;

  if (mydata == NULL) return;
  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  vnpp = (ValNodePtr PNTR) mydata;
  if (vnpp == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (ISA_aa (bsp->mol)) {
      ValNodeAddPointer (vnpp, 0, (Pointer) bsp);
    }
  }
}

static void RemoveProteinsAndOptionallyRenormalize (IteM i, Boolean renormalize)

{
  BaseFormPtr    bfp;
  BioseqPtr      bsp;
  Uint2          itemID;
  ObjMgrDataPtr  omdptop;
  ObjMgrData     omdata;
  OMProcControl  ompc;
  Uint2          parenttype;
  Pointer        parentptr;
  SeqEntryPtr    sep;
  ValNodePtr     tmp;
  ValNodePtr     vnp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  SaveSeqEntryObjMgrData (sep, &omdptop, &omdata);
  GetSeqEntryParent (sep, &parentptr, &parenttype);
  vnp = NULL;
  SeqEntryExplore (sep, (Pointer) &vnp, MarkProteinCallback);
  for (tmp = vnp; tmp != NULL; tmp = tmp->next) {
    bsp = (BioseqPtr) tmp->data.ptrvalue;
    itemID = GetItemIDGivenPointer (bfp->input_entityID, OBJ_BIOSEQ, (Pointer) bsp);
    if (itemID > 0) {
      MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
      ompc.do_not_reload_from_cache = TRUE;
      ompc.input_entityID = bfp->input_entityID;
      ompc.input_itemID = itemID;
      ompc.input_itemtype = OBJ_BIOSEQ;
      if (! DetachDataForProc (&ompc, FALSE)) {
        Message (MSG_POSTERR, "DetachDataForProc failed");
      }
    }
  }
  ValNodeFree (vnp);
  SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
  RestoreSeqEntryObjMgrData (sep, omdptop, &omdata);
  if (renormalize) 
  {
    RenormalizeNucProtSets (sep, TRUE);
  }
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ObjMgrDeSelect (0, 0, 0, 0, NULL);
  Update ();
}

extern void RemoveProteins (IteM i)
{
  RemoveProteinsAndOptionallyRenormalize (i, FALSE);
}

extern void RemoveProteinsAndRenormalize (IteM i)
{
  RemoveProteinsAndOptionallyRenormalize (i, TRUE);
}

#define EDIT_FIVE_PRIME  1
#define EDIT_THREE_PRIME 2

#define ADD_TO_END       1
#define TRIM_FROM_END    2

#define TRIM_BY_SEQUENCE 1
#define TRIM_BY_COUNT    2

typedef struct edseqendsdata {
  FEATURE_FORM_BLOCK

  TexT           seq;
  TexT           genename;
  GrouP          whichend;
  GrouP          addOrTrim;
  Boolean        addOrTrimBool;
  GrouP          trimBy;
  Boolean        trimByBool;
  Int4           trimCount;
  TexT           trimCountText;
  ButtoN         extendfeat;
  LisT           nuc_sequence_list_ctrl;
  ButtoN         addCitSub;
  CharPtr        seqstr;
  CharPtr        genestr;
  Int2           endval;
  Int2           frameshift;
  Boolean        adjustframe;
  Boolean        extendflag;
  BioseqPtr      extendedthis;
  SeqEntryPtr    sep;
  Boolean        rsult;
} EditSeqEnds, PNTR EditSeqPtr;

static Boolean GeneFindByNameFunc (GatherContextPtr gcp)

{
  EditSeqPtr  esp;
  GeneRefPtr  grp;
  SeqFeatPtr  sfp;

  if (gcp == NULL) return TRUE;
  esp = (EditSeqPtr) gcp->userdata;
  if (esp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL) return TRUE;
  if (sfp->data.choice != SEQFEAT_GENE) return TRUE;
  grp = (GeneRefPtr) sfp->data.value.ptrvalue;
  if (grp == NULL) return TRUE;
  if (StringICmp (grp->locus, esp->genestr) == 0) {
    esp->rsult = TRUE;
  }
  return TRUE;
}

static Boolean EditSeqEntryHasGene (BioseqPtr bsp, SeqEntryPtr sep, EditSeqPtr esp)

{
  GatherScope  gs;

  if (esp->input_entityID == 0 || esp->sep == NULL) return FALSE;
  if (StringHasNoText (esp->genestr)) return TRUE;
  esp->rsult = FALSE;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = TRUE;
  gs.scope = sep;
  MemSet ((Pointer)(gs.ignore), (int) (TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
  gs.ignore [OBJ_BIOSEQ] = FALSE;
  gs.ignore [OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore [OBJ_SEQFEAT] = FALSE;
  gs.ignore [OBJ_SEQANNOT] = FALSE;
  GatherEntity (esp->input_entityID, (Pointer) esp, GeneFindByNameFunc, &gs);
  gs.target = SeqLocFree (gs.target);
  return esp->rsult;
}

typedef struct findprot {
  SeqLocPtr   slp;
  Int4        min;
  SeqFeatPtr  bestprot;
} FindProtData, PNTR FindProtPtr;

static Boolean ProtFindFunc (GatherContextPtr gcp)

{
  Int4         diff;
  FindProtPtr  fpp;
  SeqFeatPtr   sfp;

  if (gcp == NULL) return TRUE;

  fpp = (FindProtPtr) gcp->userdata;
  if (fpp == NULL) return TRUE;

  if (gcp->thistype == OBJ_SEQFEAT) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp != NULL && sfp->data.choice == SEQFEAT_PROT && sfp->data.value.ptrvalue != NULL) {
      diff = SeqLocAinB (sfp->location, fpp->slp);
      if (diff >= 0) {
        if (diff < fpp->min) {
          fpp->min = diff;
          fpp->bestprot = sfp;
        }
      }
    }
  }

  return TRUE;
}

extern SeqFeatPtr FindBestProtein (Uint2 entityID, SeqLocPtr product)

{
  FindProtData  fpd;
  GatherScope   gs;

  if (entityID == 0 || product == NULL) return NULL;
  fpd.bestprot = NULL;
  fpd.min = INT4_MAX;
  fpd.slp = product;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = TRUE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherEntity (entityID, (Pointer) &fpd, ProtFindFunc, &gs);
  return fpd.bestprot;
}

static Boolean FixACDSFunc (GatherContextPtr gcp)

{
  SeqFeatPtr    bestprot;
  ByteStorePtr  bs;
  BioseqPtr     bsp;
  Char          ch;
  CdRegionPtr   crp;
  EditSeqPtr    esp;
  Int2          frame;
  Boolean       partial5;
  Boolean       partial3;
  CharPtr       prot;
  CharPtr       ptr;
  SeqEntryPtr   sep;
  SeqFeatPtr    sfp;
  SeqIdPtr      sip;
  SeqLocPtr     slp;

  if (gcp == NULL) return TRUE;
  esp = (EditSeqPtr) gcp->userdata;
  if (esp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL) return TRUE;
  if (sfp->data.choice != SEQFEAT_CDREGION) return TRUE;
  slp = SeqLocFindNext (sfp->location, NULL);
  if (slp == NULL) return TRUE;
  CheckSeqLocForPartial (slp, &partial5, &partial3);
  sip = SeqLocId (slp);
  if (sip == NULL) return TRUE;
  bsp = BioseqFind (sip);
  if (bsp == NULL || bsp != esp->extendedthis) return TRUE;
  if (esp->adjustframe) {
    if (GetOffsetInBioseq (slp, bsp, SEQLOC_START) != 0) return TRUE;
    crp = (CdRegionPtr) sfp->data.value.ptrvalue;
    if (crp == NULL) return TRUE;
    frame = crp->frame;
    if (frame == 0)
      frame = 1;
    if (esp->addOrTrimBool == ADD_TO_END)
      {
	frame--;
	frame += esp->frameshift;
	crp->frame = (frame % 3) + 1;
      }
    else if (esp->addOrTrimBool == TRIM_FROM_END)
      {
	frame = ABS(frame - esp->frameshift);
	crp->frame = 3 - (frame % 3);
      }
  } else {
    if (GetOffsetInBioseq (slp, bsp, SEQLOC_STOP) != bsp->length - 1)
      return TRUE;
  }
  sip = SeqLocId (sfp->product);
  if (sip == NULL) return TRUE;
  bsp = BioseqFind (sip);
  if (bsp == NULL) return TRUE;
  if (bsp->repr != Seq_repr_raw) return TRUE;
  if (bsp->mol != Seq_mol_aa) return TRUE;
  bestprot = FindBestProtein (esp->input_entityID, sfp->product);
  bs = ProteinFromCdRegionEx (sfp, FALSE, FALSE);
  if (bs == NULL) return TRUE;
  prot = BSMerge (bs, NULL);
  bs = BSFree (bs);
  if (prot == NULL) return TRUE;
  ptr = prot;
  ch = *ptr;
  while (ch != '\0') {
    *ptr = TO_UPPER (ch);
    ptr++;
    ch = *ptr;
  }
  bs = BSNew (1000);
  if (bs != NULL) {
    ptr = prot;
    /*
    if (prot [0] == '-') {
       ptr++;
    }
    */
    BSWrite (bs, (VoidPtr) ptr, (Int4) StringLen (ptr));
    bsp->seq_data = BSFree (bsp->seq_data);
    bsp->seq_data = bs;
    bsp->length = BSLen (bs);
    bsp->seq_data_type = Seq_code_ncbieaa;
  }
  if (bestprot == NULL) return TRUE;
  sep = SeqMgrGetSeqEntryForData (bsp);
  bestprot->location = SeqLocFree (bestprot->location);
  bestprot->location = CreateWholeInterval (sep);
  SetSeqLocPartial (bestprot->location, partial5, partial3);
  return TRUE;
}

static void FixAndRetranslateCDSs (BioseqPtr bsp, SeqEntryPtr sep,
                                   EditSeqPtr esp, Boolean adjustframe)

{
  GatherScope  gs;

  if (esp->input_entityID == 0 || esp->sep == NULL) return;
  esp->adjustframe = adjustframe;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  gs.scope = sep;
  MemSet ((Pointer)(gs.ignore), (int) (TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
  gs.ignore [OBJ_BIOSEQ] = FALSE;
  gs.ignore [OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore [OBJ_SEQFEAT] = FALSE;
  gs.ignore [OBJ_SEQANNOT] = FALSE;
  GatherEntity (esp->input_entityID, (Pointer) esp, FixACDSFunc, &gs);
  gs.target = SeqLocFree (gs.target);
}

static ValNodePtr CollectAndExtendSingleBaseFeatures (BioseqPtr bsp, Int2 whichend, Int4 len)

{
  SeqMgrFeatContext  context;
  ValNodePtr         head = NULL;
  SeqFeatPtr         sfp;
  SeqLocPtr          slp;
  SeqPntPtr          spp;

  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &context);
  while (sfp != NULL) {
    if (whichend == 1 && context.numivals == 1 && context.right == 0) {
      slp = sfp->location;
      if (slp != NULL && slp->choice == SEQLOC_PNT && slp->next == NULL) {
        spp = (SeqPntPtr) slp->data.ptrvalue;
        if (spp != NULL && spp->point == 0) {
          spp->point = 1;
          ValNodeAddPointer (&head, 1, (Pointer) sfp);
        }
      }
    } else if (whichend == 2 && context.numivals == 1 && context.left == bsp->length - 1) {
      slp = sfp->location;
      if (slp != NULL && slp->choice == SEQLOC_PNT && slp->next == NULL) {
        spp = (SeqPntPtr) slp->data.ptrvalue;
        if (spp != NULL && spp->point == bsp->length - 1) {
          spp->point = bsp->length - 2;
          ValNodeAddPointer (&head, 2, (Pointer) sfp);
        }
      }
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &context);
  }

  return head;
}

static void ReadjustSingleBaseFeatures (ValNodePtr head, BioseqPtr bsp, Int2 whichend, Int4 len)

{
  SeqFeatPtr  sfp;
  SeqIntPtr   sintp;
  SeqLocPtr   slp;
  SeqPntPtr   spp;
  ValNodePtr  vnp;

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    sfp = (SeqFeatPtr) vnp->data.ptrvalue;
    if (sfp != NULL) {
      slp = sfp->location;
      if (slp != NULL && slp->choice == SEQLOC_PNT && slp->next == NULL) {
        spp = (SeqPntPtr) slp->data.ptrvalue;
        if (spp != NULL) {
          if (whichend == 1) {
            sintp = SeqIntNew ();
            if (sintp != NULL) {
              sintp->from = 0;
              sintp->to = spp->point - 1;
              sintp->strand = spp->strand;
              sintp->id = spp->id;
              spp->id = NULL;
              sintp->if_from = spp->fuzz;
              spp->fuzz = NULL;
              slp->choice = SEQLOC_INT;
              slp->data.ptrvalue = (Pointer) sintp;
              SeqPntFree (spp);
            }
          } else if (whichend == 2) {
            sintp = SeqIntNew ();
            if (sintp != NULL) {
              sintp->from = spp->point + 1;
              sintp->to = spp->point + 1 + len;
              sintp->strand = spp->strand;
              sintp->id = spp->id;
              spp->id = NULL;
              sintp->if_to = spp->fuzz;
              spp->fuzz = NULL;
              slp->choice = SEQLOC_INT;
              slp->data.ptrvalue = (Pointer) sintp;
              SeqPntFree (spp);
            }
          }
        }
      }
    }
  }
}

static void TrimFromSequenceEnd (EditSeqPtr  esp,
				 SeqEntryPtr sep,
				 BioseqPtr   bsp)
{
  CharPtr currSeqStr;
  Int2    length;
  Int4    pos;

  /* Get the current sequence string */

  currSeqStr = GetSequenceByBsp (bsp);

  /* Trim from either the 5' end (i.e., */
  /* the beginning of the string...     */

  if (esp->endval == EDIT_FIVE_PRIME)
    {
      /* Find end point */

      if (esp->trimByBool == TRIM_BY_SEQUENCE)
	{
	  length = StringLen (esp->seqstr);
	  if (StringNICmp (esp->seqstr, currSeqStr, length) != 0)
	    return;
	  pos = length - 1;
	}
      else  if (esp->trimByBool == TRIM_BY_COUNT)
	pos = esp->trimCount - 1;
      else
	return;
      
      /* Trim from beginning of string to end point */
      
      esp->frameshift = length;
      BioseqDelete (bsp->id, 0, pos, TRUE, FALSE);
      esp->extendedthis = bsp;
      FixAndRetranslateCDSs (bsp, sep, esp, TRUE);
    }
  
  /* .. or the 3' end (i.e., the */
  /* end of the string.          */
  
  else if (esp->endval == EDIT_THREE_PRIME)
    {
      /* Find trim point */

      if (esp->trimByBool == TRIM_BY_SEQUENCE)
	{
	  length = StringLen (esp->seqstr);
	  pos = bsp->length - length;
	  if (StringICmp (esp->seqstr, &currSeqStr[pos]) != 0)
	    return;
	}
      else  if (esp->trimByBool == TRIM_BY_COUNT)
	pos = bsp->length - esp->trimCount;
      else
	return;
      
      /* Trim from there to end of string */
      
      BioseqDelete (bsp->id, pos, bsp->length - 1, TRUE, FALSE);
      esp->extendedthis = bsp;
      FixAndRetranslateCDSs (bsp, sep, esp, FALSE);
    }
}

static void AddToSequenceEnd (EditSeqPtr  esp,
			      SeqEntryPtr sep,
			      BioseqPtr   bsp)
{
  ValNodePtr    head;
  Int4          len;
  Int4          pos;
  Uint1         residue;
  SeqPortPtr    spp;
  CharPtr       str;
  Char          terminal [2];

  pos = 0;
  if (esp->endval == 2) {
    pos = bsp->length;
  }
  if (esp->extendflag) {
    esp->frameshift = 0;
    terminal [0] = '\0';
    terminal [1] = '\0';
    residue = 0;
    if (esp->endval == 2) {
      spp = SeqPortNew (bsp, bsp->length - 1, -1, 0, Seq_code_iupacna);
    } else {
      spp = SeqPortNew (bsp, 0, -1, 0, Seq_code_iupacna);
    }
    if (spp != NULL) {
      residue = SeqPortGetResidue (spp);
      if (IS_residue (residue)) {
        terminal [0] = TO_LOWER ((Char) residue);
      }
    }
    SeqPortFree (spp);
    str = MemNew ((size_t) (StringLen (esp->seqstr) + 4));
    if (str != NULL) {
      head = NULL;
      if (esp->endval == 2) {
        esp->extendedthis = bsp;
        StringCpy (str, terminal);
        StringCat (str, esp->seqstr);
        len = StringLen (esp->seqstr);
        pos = bsp->length - 1;
        head = CollectAndExtendSingleBaseFeatures (bsp, 2, len);
        insertchar (str, pos, bsp->id, bsp->mol, FALSE);
        BioseqDelete (bsp->id, bsp->length - 1, bsp->length - 1,
		      TRUE, FALSE);
        ReadjustSingleBaseFeatures (head, bsp, 2, len);
        FixAndRetranslateCDSs (bsp, sep, esp, FALSE);
      } else {
        esp->frameshift = (Int2) StringLen (esp->seqstr);
        esp->extendedthis = bsp;
        StringCpy (str, esp->seqstr);
        StringCat (str, terminal);
        len = StringLen (esp->seqstr);
        pos = 1;
        head = CollectAndExtendSingleBaseFeatures (bsp, 1, len);
        insertchar (str, pos, bsp->id, bsp->mol, FALSE);
        BioseqDelete (bsp->id, 0, 0, TRUE, FALSE);
        ReadjustSingleBaseFeatures (head, bsp, 1, len);
        FixAndRetranslateCDSs (bsp, sep, esp, TRUE);
      }
      ValNodeFree (head);
    }
    MemFree (str);
  } else {
    insertchar (esp->seqstr, pos, bsp->id, bsp->mol, FALSE);
  }
}


static void DoEditSeqEndsProc (ButtoN b)

{
  Char        ch;
  EditSeqPtr  esp;
  CharPtr     p, q;
  CharPtr     tempStr;
  ValNodePtr  sip_list, vnp;
  SeqIdPtr    sip;
  BioseqPtr   bsp;
  SeqEntryPtr sep;
  Boolean     add_cit_subs = FALSE;

  esp = (EditSeqPtr) GetObjectExtra (b);
  if (esp == NULL) {
    Remove (ParentWindow (b));
    return;
  }
  sip_list = GetSelectedSequenceList (esp->nuc_sequence_list_ctrl);
  if (sip_list == NULL)
  {
    Message (MSG_ERROR, "You have not specified any sequences to edit!");
    return;
  }

  Hide (esp->form);
  Update ();
  esp->seqstr = SaveStringFromText  (esp->seq);
  p = esp->seqstr;
  if (p != NULL) {
    /* remove any non-sequence characters */
    q = p;
    ch = *p;
    while (ch != '\0') {
      if (IS_ALPHA (ch)) {
        *q = ch;
        q++;
      }
      p++;
      ch = *p;
    }
    *q = '\0';
  }
  esp->genestr       = SaveStringFromText  (esp->genename);
  esp->endval        = GetValue (esp->whichend);
  esp->extendflag    = GetStatus (esp->extendfeat);
  esp->addOrTrimBool = GetValue (esp->addOrTrim);
  esp->trimByBool    = GetValue (esp->trimBy);
  tempStr            = SaveStringFromText (esp->trimCountText);
  if (tempStr != NULL)
    esp->trimCount   = atoi (tempStr);
  else
    esp->trimCount   = 0;
  
  add_cit_subs = GetStatus (esp->addCitSub);

  for (vnp = sip_list; vnp != NULL; vnp = vnp->next)
  {
    sip = (SeqIdPtr) vnp->data.ptrvalue;
    bsp = BioseqFind (sip);
    sep = SeqMgrGetSeqEntryForData (bsp);
    if (bsp != NULL && sep != NULL && EditSeqEntryHasGene (bsp, sep, esp))
    {
      if (esp->addOrTrimBool == 1)
        AddToSequenceEnd (esp, sep, bsp);
      else
        TrimFromSequenceEnd (esp, sep, bsp);
      if (add_cit_subs)
      {
        AddCitSubToUpdatedSequence (bsp, esp->input_entityID);
      }
    }
  }

  MemFree (esp->seqstr);
  MemFree (esp->genestr);
  ObjMgrSetDirtyFlag (esp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, esp->input_entityID, 0, 0);
  Remove (esp->form);
}

static void EditSeqMessageProc (ForM f, Int2 mssg)

{
  EditSeqPtr  esp;

  esp = (EditSeqPtr) GetObjectExtra (f);
  if (esp != NULL) {
    if (esp->appmessage != NULL) {
      esp->appmessage (f, mssg);
    }
  }
}

static void ChangeAddOrTrim_Callback (GrouP g)

{
  EditSeqPtr  esp;
  Int2        val;
  Int2        trimByState;

  val = GetValue (g);
  esp = GetObjectExtra (g);

  switch (val) {
    case ADD_TO_END :
      SafeDisable (esp->trimBy);
      SafeEnable (esp->extendfeat);
      SafeDisable (esp->trimCountText);
      SafeEnable (esp->seq);
      break;
    case TRIM_FROM_END :
      SafeDisable (esp->extendfeat);
      SafeEnable (esp->trimBy);
      trimByState = GetValue (esp->trimBy);
      switch (trimByState) {
        case TRIM_BY_SEQUENCE :
	  SafeDisable (esp->trimCountText);
	  SafeEnable (esp->seq);
	  break;
        case TRIM_BY_COUNT :
	  SafeEnable (esp->trimCountText);
	  SafeDisable (esp->seq);
	  break;
        default :
	  break;
      }
      break;
    default :
      break;
  }
}

static void ChangeTrimBy_Callback (GrouP g)

{
  EditSeqPtr  esp;
  Int2        val;

  val = GetValue (g);
  esp = GetObjectExtra (g);

  switch (val) {
    case TRIM_BY_SEQUENCE :
      SafeDisable (esp->trimCountText);
      SafeEnable (esp->seq);
      break;
    case TRIM_BY_COUNT :
      SafeEnable (esp->trimCountText);
      SafeDisable (esp->seq);
      break;
    default :
      break;
  }
}

static void SelectAllSequencesForExtend (ButtoN b)
{
  EditSeqPtr    esp;

  esp = (EditSeqPtr) GetObjectExtra (b);
  if (esp == NULL)
  {
    return;
  }
  SelectAllSequencesInListCtrl (esp->nuc_sequence_list_ctrl);  
}

static void UnSelectAllSequencesForExtend (ButtoN b)
{
  EditSeqPtr    esp;

  esp = (EditSeqPtr) GetObjectExtra (b);
  if (esp == NULL)
  {
    return;
  }
  UnSelectAllSequencesInListCtrl (esp->nuc_sequence_list_ctrl);  
}

extern void EditSeqEndsProc (IteM i);

extern void EditSeqEndsProc (IteM i)

{
  ButtoN             b;
  BaseFormPtr        bfp;
  GrouP              c;
  EditSeqPtr         esp;
  GrouP              g;
  GrouP              h;
  GrouP              k;
  GrouP              p;
  GrouP              q;
  GrouP              r;
  GrouP              s;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  WindoW             w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  esp = (EditSeqPtr) MemNew (sizeof (EditSeqEnds));
  if (esp == NULL) return;

  w = FixedWindow (-50, -33, -10, -10, "Edit Sequence Ends", StdCloseWindowProc);
  SetObjectExtra (w, esp, StdCleanupFormProc);
  esp->form = (ForM) w;
  esp->formmessage = EditSeqMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    esp->appmessage = sepp->handleMessages;
  }

  esp->input_entityID = bfp->input_entityID;
  esp->input_itemID = bfp->input_itemID;
  esp->input_itemtype = bfp->input_itemtype;

  esp->sep = sep;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 2, 0, NULL);

  StaticPrompt (g, "End", 0, stdLineHeight, programFont, 'l');
  esp->whichend = HiddenGroup (g, 2, 0, NULL);
  RadioButton (esp->whichend, "5'");
  RadioButton (esp->whichend, "3'");
  SetValue (esp->whichend, 1);

  esp->addOrTrim = HiddenGroup (h, 2, 0, ChangeAddOrTrim_Callback);
  SetObjectExtra (esp->addOrTrim, esp, NULL);
  RadioButton (esp->addOrTrim, "Add to end");
  RadioButton (esp->addOrTrim, "Trim from end");
  SetValue (esp->addOrTrim, 1);

  k = HiddenGroup (h, 0, -2, NULL);
  StaticPrompt (k, "Sequence", 0, 0, programFont, 'l');
  esp->seq = ScrollText (k, 25, 5, programFont, TRUE, NULL);

  q = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (q, "Optional gene constraint", 0, dialogTextHeight,
		programFont, 'l');
  esp->genename = DialogText (q, "", 14, NULL);

  esp->extendfeat = CheckBox (h, "Extend features", NULL);

  esp->trimBy = HiddenGroup (h, 2, 0, ChangeTrimBy_Callback);
  SetObjectExtra (esp->trimBy, esp, NULL);
  RadioButton (esp->trimBy, "Trim by sequence");
  RadioButton (esp->trimBy, "Trim by count");
  SetValue (esp->trimBy, 1);
  SafeDisable (esp->trimBy);

  p = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (p, "Trim count", 0, dialogTextHeight, programFont, 'l');
  esp->trimCountText = DialogText (p, "", 5, NULL);
  SafeDisable (esp->trimCountText);
  
  s = NormalGroup (h, -1, 0, "Choose Sequences To Edit", programFont, NULL);
  esp->nuc_sequence_list_ctrl = MakeSequenceListControl (s, sep, NULL, NULL, TRUE, FALSE);
  
  r = HiddenGroup (s, 2, 0, NULL);
  b = PushButton (r, "Select All", SelectAllSequencesForExtend);
  SetObjectExtra (b, esp, NULL);
  b = PushButton (r, "Unselect All", UnSelectAllSequencesForExtend);
  SetObjectExtra (b, esp, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) esp->nuc_sequence_list_ctrl,
                (HANDLE) r, NULL);

  esp->addCitSub = CheckBox (h, "Add Cit Subs to edited sequences", NULL);
  
  c = HiddenGroup (h, 4, 0, NULL);
  b = DefaultButton (c, "Accept", DoEditSeqEndsProc);
  SetObjectExtra (b, esp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) esp->extendfeat,
		(HANDLE) esp->addOrTrim, (HANDLE) esp->trimBy,
		(HANDLE) k, (HANDLE) q, (HANDLE) p, 
		(HANDLE) s, (HANDLE) esp->addCitSub, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Select (w);
  Select (esp->seq);
  Update ();
}

static void SetAlignmentDim (SeqAlignPtr salp)
{
  AMAlignIndex2Ptr amaip;
  DenseSegPtr      dsp;
  
  if (salp == NULL || salp->dim > 0 || salp->saip == NULL) return;
  
  if (salp->saip->indextype == INDEX_PARENT)
  {
    amaip = (AMAlignIndex2Ptr)(salp->saip);
    salp->dim = amaip->sharedaln->dim;
  }
  else if (salp->saip->indextype == INDEX_CHILD)
  {
    dsp = (DenseSegPtr)(salp->segs);
  	salp->dim = dsp->dim;
  }
}  

static void IndexAlignmentSet (SeqAlignPtr salp)
{
  SeqAlignPtr tmp_salp, next_salp;
  
  if (salp == NULL || salp->saip != NULL) return;
  
  if (salp->next != NULL && salp->dim > 2)
  {
  	for (tmp_salp = salp; tmp_salp != NULL; tmp_salp = tmp_salp->next)
  	{
  	  next_salp = tmp_salp->next;
  	  tmp_salp->next = NULL;
      if (tmp_salp->segtype == SAS_DENSEG  &&  tmp_salp->next == NULL) {
        AlnMgr2IndexSingleChildSeqAlign(tmp_salp);
      } else {
        AlnMgr2IndexSeqAlign(tmp_salp);
      }  		
      SetAlignmentDim (tmp_salp);
      tmp_salp->next = next_salp;
  	}
  }
  else
  {
    if (salp->segtype == SAS_DENSEG  &&  salp->next == NULL) {
      AlnMgr2IndexSingleChildSeqAlign(salp);
    } else {
      AlnMgr2IndexSeqAlign(salp);
    }  
    SetAlignmentDim (salp);		
  }	
}

static void WriteSeqEntryAlignmentToFile (SeqEntryPtr sep, FILE *fp, Boolean Interleave)
{
  BioseqSetPtr bssp;
  SeqAnnotPtr  sap;
  SeqAlignPtr  salp = NULL;

  if (sep == NULL || ! IS_Bioseq_set (sep)) return;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp == NULL) return;
  for (sap = bssp->annot; sap != NULL; sap = sap->next) {
    if (sap->type == 2) {
      salp = SeqAlignListDup((SeqAlignPtr) sap->data);
      IndexAlignmentSet (salp);

      if (Interleave) {
        if (salp->next != NULL)
        {
          Message (MSG_ERROR, "Unable to write segmented alignments as interleave");
          return;
        }
        WriteAlignmentInterleaveToFile (salp, fp, 40, FALSE);
      } else {
        WriteAlignmentContiguousToFile (salp, fp, 40, FALSE);
      }
      SeqAlignFree (salp);
      salp = NULL;
    }
  }

  for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
    WriteSeqEntryAlignmentToFile (sep, fp, Interleave);
  }
}

static void ExportAlignment (IteM i, Boolean Interleave)
{
  BaseFormPtr bfp;
  SeqEntryPtr sep;
  Char        path [PATH_MAX];
  FILE *      fp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  if (! GetOutputFileName (path, sizeof (path), "")) return;
  if (! StringHasNoText (path)) {
    fp = FileOpen (path, "w");
    if (fp != NULL) {
      WatchCursor ();
      Update();
      WriteSeqEntryAlignmentToFile (sep, fp, Interleave);
      ArrowCursor ();
      Update();
      FileClose (fp);
    } else {
      Message (MSG_ERROR, "Unable to open file");
    }
  }
}

extern void ExportAlignmentInterleave (IteM i)
{
  ExportAlignment (i, TRUE);
}

extern void ExportAlignmentContiguous (IteM i)
{
  ExportAlignment (i, FALSE);
}

static Int4 FindFeaturePos (SeqIdPtr sip, SeqAlignPtr salp, Int4 pos)
{
  Int4        aln_row;
  Int4        new_pos;
  
  if (sip == NULL || salp == NULL) return pos;
  aln_row = AlnMgr2GetFirstNForSip(salp, sip);
  if (aln_row > 0)
  {
  	new_pos = AlnMgr2MapSeqAlignToBioseq (salp, pos, aln_row);
  	if (new_pos < 0)
  	{
  	  return pos;
  	}
  	else
  	{
  	  return new_pos;
  	}
  }
  return pos;
}

static void FixFeatureIntervalSeqLoc (SeqLocPtr slp, SeqAlignPtr salp)
{
  SeqIntPtr     sintp;
  SeqBondPtr    sbp;
  SeqPntPtr     spp;
  SeqIdPtr      tmpsip;
  SeqLocPtr     this_slp;
 
  if (slp == NULL || slp->data.ptrvalue == NULL || salp == NULL) return;

  tmpsip = SeqIdPtrFromSeqAlign (salp);

  switch (slp->choice)
  {
    case SEQLOC_INT:
      sintp = slp->data.ptrvalue;
      sintp->from = FindFeaturePos (sintp->id, salp, sintp->from);
      sintp->to = FindFeaturePos (sintp->id, salp, sintp->to);
      break;
  	case SEQLOC_PNT:
  	  spp = slp->data.ptrvalue;
  	  spp->point = FindFeaturePos (spp->id, salp, spp->point);
  	  break;
    case SEQLOC_BOND:   /* bond -- 2 seqs */
      sbp = (SeqBondPtr)(slp->data.ptrvalue);
      spp = sbp->a;
  	  spp->point = FindFeaturePos (spp->id, salp, spp->point);
      spp = sbp->b;
  	  spp->point = FindFeaturePos (spp->id, salp, spp->point);
      break;
    case SEQLOC_MIX:    /* mix -- more than one seq */
    case SEQLOC_EQUIV:    /* equiv -- ditto */
    case SEQLOC_PACKED_INT:    /* packed int */
      for (this_slp = slp->data.ptrvalue; this_slp != NULL; this_slp = this_slp->next)
      {
      	FixFeatureIntervalSeqLoc (this_slp, salp);
      }
      break;
    default:
      break;	
  }
}

typedef struct featurefixdata 
{
  Uint2 entityID;
  SeqAlignPtr salp;	
} FeatureFixData, PNTR FeatureFixPtr;

static void FixFeatureIntervalCallback (SeqFeatPtr sfp, Pointer userdata)

{
  BioseqPtr     bsp;
  FeatureFixPtr ffp;
 
  if (sfp == NULL || userdata == NULL) return;  
  
  ffp = (FeatureFixPtr) userdata;
 
  FixFeatureIntervalSeqLoc (sfp->location, ffp->salp);
  if (sfp->idx.subtype == FEATDEF_CDS)
  {
    bsp = BioseqFindFromSeqLoc (sfp->location);
  	SeqEdTranslateOneCDS (sfp, bsp, ffp->entityID);
  }
}

extern void FixFeatureIntervals (IteM i)
{
  BaseFormPtr    bfp;
  SeqEntryPtr    sep;
  FeatureFixData ffd;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  ffd.entityID = bfp->input_entityID;
  ffd.salp = SeqAlignListDup((SeqAlignPtr) FindSeqAlignInSeqEntry (sep, OBJ_SEQALIGN));
  if (ffd.salp == NULL)
  {
    Message (MSG_ERROR, "No alignment present - cannot remap intervals");
    return;
  }
      
  if (ffd.salp->segtype == SAS_DENSEG  &&  ffd.salp->next == NULL) 
  {
    AlnMgr2IndexSingleChildSeqAlign(ffd.salp);
  } else {
    AlnMgr2IndexSeqAlign(ffd.salp);
  }

  VisitFeaturesInSep (sep, (Pointer) &ffd, FixFeatureIntervalCallback);
  SeqAlignFree (ffd.salp);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static SeqLocPtr 
CreateProteinLoc 
(SeqLocPtr  nuc_loc,
 SeqFeatPtr top_cds,
 Int4       offset, 
 BioseqPtr  prot_bsp,
 LogInfoPtr lip)
{
  SeqLocPtr    prot_loc = NULL;
  SeqIntPtr    sintp_nuc, sintp_prot;
  Boolean      partial5, partial3;
  ByteStorePtr bs;
  CharPtr      prot_str;
  Boolean      ends_with_stop = FALSE;
  Int4         prot_len = 0;
  Char         ch;
  
  if (nuc_loc == NULL || top_cds == NULL || prot_bsp == NULL)
  {
    return NULL;
  }
  if (nuc_loc->choice != SEQLOC_INT)
  {
    Message (MSG_ERROR, "Unable to translate locations that are not simple intervals");
  }
  else
  {
    sintp_nuc = (SeqIntPtr)(nuc_loc->data.ptrvalue);
    if (sintp_nuc != NULL)
    {
      if (sintp_nuc->from < offset || (sintp_nuc->from - offset) % 3 != 0
          || sintp_nuc->to < offset || (sintp_nuc->to + 1 - offset) % 3 != 0)
      {
        if (lip != NULL && lip->fp != NULL)
        {
          fprintf (lip->fp, "Invalid coordinates for mat_peptide conversion");
          lip->data_in_log = TRUE;
        }
        else
        {
          Message (MSG_ERROR, "Invalid coordinates for mat_peptide conversion");
        }
      }
      else
      {  
        bs = ProteinFromCdRegionEx (top_cds, TRUE, FALSE);
        if (bs != NULL)
        {
          prot_str = BSMerge (bs, NULL);
          prot_len = StringLen (prot_str);
          ch = prot_str [prot_len - 1];
          if (ch == '*')
          {
            ends_with_stop = TRUE;
          }
          MemFree (prot_str);
          bs = BSFree (bs);
        }

        sintp_prot = SeqIntNew ();
        if (sintp_prot != NULL)
        {
          CheckSeqLocForPartial (nuc_loc, &partial5, &partial3);
          sintp_prot->id = SeqIdDup (SeqIdFindBest(prot_bsp->id, 0));
          sintp_prot->from = (sintp_nuc->from - offset) / 3;
          sintp_prot->to = (sintp_nuc->to - offset) / 3;
          if (sintp_prot->to == prot_len - 1)
          {
            if (ends_with_stop)
            {
              sintp_prot->to --;
            }
            else
            {
              partial3 = TRUE;
            }
          }
          ValNodeAddPointer (&prot_loc, SEQLOC_INT, sintp_prot);
          SetSeqLocPartial (prot_loc, partial5, partial3);
        }
      }
    }
  }
  return prot_loc;
}

static CharPtr GetCDSProteinDesc (SeqFeatPtr cds)
{
  BioseqPtr         bsp;
  SeqFeatPtr        prot_feat;
  SeqMgrFeatContext context;
  ProtRefPtr        prp;
  
  if (cds == NULL || cds->product == NULL)
  {
    return NULL;
  }
  
  bsp = BioseqFindFromSeqLoc (cds->product);
  prot_feat = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_PROT, 0, &context);
  if (prot_feat != NULL && prot_feat->data.value.ptrvalue != NULL)
  {
    prp = (ProtRefPtr) prot_feat->data.value.ptrvalue;
    return StringSave (prp->desc);
  }
  else
  {
    return NULL;
  }
}

extern void ConvertInnerCDSsToMatPeptidesCallback (BioseqPtr bsp, Pointer userdata)
{
  SeqFeatPtr        sfp = NULL;
  ValNodePtr        top_level_cds_list = NULL;
  SeqMgrFeatContext context, gene_context;
  ValNodePtr        vnp;
  SeqFeatPtr        top_cds;
  BioseqPtr         prot_bsp;
  SeqLocPtr         prot_loc;
  Int4              offset;
  SeqFeatPtr        new_sfp;
  ProtRefPtr        prp;
  SeqFeatPtr        gene;
  CdRegionPtr       crp;
  LogInfoPtr        lip;
  
  if (bsp == NULL || ! ISA_na (bsp->mol)) return;
  lip = (LogInfoPtr) userdata;
  
  sfp = SeqMgrGetNextFeature (bsp, sfp, SEQFEAT_CDREGION, 0, &context);
  while (sfp != NULL)
  {
    top_cds = NULL;
    offset = 0;
    for (vnp = top_level_cds_list; vnp != NULL && top_cds == NULL; vnp = vnp->next)
    {
      top_cds = (SeqFeatPtr) vnp->data.ptrvalue;
      if (top_cds != NULL)
      {
        if (SeqLocCompare (top_cds->location, sfp->location) == SLC_B_IN_A)
        {
          offset = vnp->choice;
        }
        else
        {
          top_cds = NULL;
        }
      }
    }
    /* Only handling the simplest cases of SeqInt locs inside other SeqInt locs */
    if (top_cds == NULL)
    {
      if (sfp->location != NULL 
          && sfp->location->choice == SEQLOC_INT
          && sfp->location->data.ptrvalue != NULL
          && sfp->data.value.ptrvalue != NULL)
      {
        /* add to list of top level CDSs */
        crp = (CdRegionPtr) sfp->data.value.ptrvalue;
        offset = context.left;
        if (crp->frame == 2)
        {
          offset += 1;
        }
        else if (crp->frame == 3)
        {
          offset += 2;
        }
        ValNodeAddPointer (&top_level_cds_list, offset, sfp);
      }
    }
    else
    {
      prot_bsp = BioseqFindFromSeqLoc (top_cds->product);
      if (prot_bsp != NULL)
      {
        /* convert location by subtracting top_cds start from sfp locations and
         * divide all locations by 3
         */
        prot_loc = CreateProteinLoc (sfp->location, top_cds, offset, prot_bsp, lip);
        if (prot_loc != NULL)
        {
          /* Create new feature on prot_bsp */
          new_sfp = CreateNewFeatureOnBioseq (prot_bsp, SEQFEAT_PROT, prot_loc);
          if (new_sfp != NULL)
          {
            prp = ProtRefNew ();
            if (prp != NULL)
            {
              ValNodeCopyStr (&(prp->name), 0, context.label);
              prp->processed = 2;
              prp->desc = GetCDSProteinDesc (sfp);
            }
            new_sfp->data.value.ptrvalue = prp;
            
            /* mark old product for deletion */
            prot_bsp = BioseqFindFromSeqLoc (sfp->product);
            if (prot_bsp != NULL)
            {
              prot_bsp->idx.deleteme = 1;
            }
         
            /* Mark old feature for deletion */
            sfp->idx.deleteme = 1;
            gene = SeqMgrGetOverlappingGene (sfp->location, &gene_context);
            if (gene != NULL && SeqLocCompare (gene->location, sfp->location) == SLC_A_EQ_B)
            {
              gene->idx.deleteme = 1;
            }
          }
        }
      }
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, SEQFEAT_CDREGION, 0, &context);
  }
  ValNodeFree (top_level_cds_list);
}



extern void ConvertInnerCDSsToProteinFeatures (IteM i)
{
  BaseFormPtr    bfp;
  SeqEntryPtr    sep;
  LogInfoPtr     lip;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  
  lip = OpenLog ("CDS to Mat Peptide Conversion");
  VisitBioseqsInSep (sep, lip, ConvertInnerCDSsToMatPeptidesCallback);
  CloseLog (lip);
  lip = FreeLog (lip);
  DeleteMarkedObjects (bfp->input_entityID, 0, NULL);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  Update ();  
}
