/*   sequin1.c
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
* File Name:  sequin1.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   1/22/95
*
* $Revision: 6.331 $
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
#include <vsm.h>
#include <vsmutil.h>
#include <valid.h>
#include <fstyle.h>
#include <biosrc.h>
#include <seqsub.h>
#include <cdrgn.h>
#include <import.h>
#include <medview.h>
#include <bspview.h>
#include <pubdesc.h>
#include <toasn3.h>
#include <utilpub.h>
#include <tofasta.h>
#include <saledit.h>
#include <salstruc.h>
#include <salfiles.h>
#include <salign.h>
#include <salsap.h>
#include <salutil.h>
#include <salpedit.h>
#include <salptool.h>
#include <pobutil.h>
#include <accutils.h>
#include <netcnfg.h>
#include <asn2ff.h>
#include <ffprint.h>
#include <objproj.h>
#include <suggslp.h>
#include <subutil.h>
#include <explore.h>
#include <actutils.h>
#include <accid1.h>
#include <ddvopen.h>
#include <dotseq.h>
#include <asn2graphicp.h>

/* USE_SMARTNET */
#ifdef USE_SMARTNET
#include <smartnet.h>
#endif

/* USE_ENTREZ */
#include <accentr.h>

#include <objmime.h>
#include <mmdbapi.h>
/*
#ifndef WIN16
#include <cn3dentr.h>
#include <cn3dopen.h>
#endif
*/

#include <entrez.h>

/* USE_LOCAL */
#include <lsqfetch.h>

/* USE_MEDARCH */
#include <medarch.h>
#include <medutil.h>

/*#ifdef USE_TAXON*/
/*
#include <taxondt.h>
*/
#include <taxutil.h>

#ifdef USE_SPELL
#ifdef WIN_MOTIF
#include <spellapi.h>
#endif
#endif

#define SEQ_APP_VER "3.85"

#ifndef CODECENTER
static char* sequin_version_binary = "Sequin Indexer Services Version " SEQ_APP_VER " " __DATE__ " " __TIME__;
#else
static char* sequin_version_binary = "Sequin Indexer Services Version curr today now";
#endif

CharPtr SEQUIN_APPLICATION = SEQ_APP_VER;
CharPtr SEQUIN_SERVICES = NULL;
CharPtr SEQUIN_VERSION = NULL;

extern EnumFieldAssoc  orgmod_subtype_alist [];
extern EnumFieldAssoc  subsource_subtype_alist [];
extern EnumFieldAssoc  biosource_genome_simple_alist [];

Boolean  useDesktop = FALSE;
Boolean  useEntrez = FALSE;
Boolean  useLocal = FALSE;
Boolean  useBlast = FALSE;
Boolean  useMedarch = FALSE;
Boolean  useTaxon = FALSE;
Boolean  allowDownload = FALSE;
Boolean  extraServices = FALSE;
Boolean  indexerVersion = FALSE;

CharPtr  genomeCenter = NULL;

Boolean  leaveAsOldAsn = FALSE;
Boolean  newAlignReader = TRUE;

SeqEntryPtr     globalsep = NULL;
Uint2           globalEntityID = 0;
Char            globalPath [PATH_MAX];

static SequinBlockPtr  globalsbp = NULL;

static Boolean useNewGraphicView = FALSE;
/* static Boolean useUdv = FALSE; */

ForM  startupForm = NULL;

#ifdef WIN_MAC
Boolean  termListUp = FALSE;
Boolean  docSumUp = FALSE;
Boolean  bioseqViewUp = FALSE;
#endif

#ifdef WIN_MAC
static IteM  openItem = NULL;
static IteM  closeItem = NULL;
static IteM  importItem = NULL;
static IteM  exportItem = NULL;
static IteM  duplicateViewItem = NULL;
static IteM  saveItem = NULL;
static IteM  saveAsItem = NULL;
static IteM  restoreItem = NULL;
static IteM  prepareItem = NULL;
static IteM  submitItem = NULL;
static IteM  loadUidItem = NULL;
static IteM  saveUidItem = NULL;
static IteM  printItem = NULL;

static IteM  undoItem = NULL;
static IteM  cutItem = NULL;
static IteM  copyItem = NULL;
static IteM  pasteItem = NULL;
static IteM  deleteItem = NULL;
static IteM  duplicateItem = NULL;

static IteM  orfItem = NULL;
static IteM  aluItem = NULL;
static IteM  targetItem = NULL;
static IteM  findItem = NULL;
static IteM  findFFItem = NULL;
static IteM  findGeneItem = NULL;
static IteM  findProtItem = NULL;
static IteM  findPosItem = NULL;
static IteM  validateItem = NULL;
static IteM  spellItem = NULL;
static IteM  vectorScreenItem = NULL;
static IteM  powerBlastItem = NULL;
static IteM  cddBlastItem = NULL;
static IteM  editsequenceitem = NULL;
static IteM  editseqalignitem = NULL;
static IteM  editseqsubitem = NULL;
static IteM  edithistoryitem = NULL;
static MenU  updateSeqMenu = NULL;
static MenU  addSeqMenu = NULL;
static IteM  featPropItem = NULL;
static IteM  updalignitem = NULL;

static IteM  docsumfontItem = NULL;
static IteM  displayfontItem = NULL;
static IteM  preferencesItem = NULL;
static IteM  clearUnusedItem = NULL;
static IteM  legendItem = NULL;
static ChoicE  queryChoice = NULL;
static ChoicE  neighborChoice = NULL;
static IteM  oldAsnItem = NULL;

extern IteM  addSecondaryItem;
IteM  addSecondaryItem = NULL;
#endif

static Int2  startupStyle = 0;

static ForM  termListForm = NULL;
static ForM  docSumForm = NULL;

static Boolean  loadSaveUidListOK = FALSE;

static MedlineViewProcs    medviewprocs;
SeqViewProcs        seqviewprocs;
static EntrezGlobals       entrezglobals;

static SeqEditViewProcs    seqedprocs;
static StdEditorProcs      stdedprocs;
static StdEditorProcs      valdtrprocs;
static TextViewProcs       txtviewprocs;
static PubdescEditProcs    pubedprocs;
static BioSourceEditProcs  biosrcedprocs;

static PRGD  prgdDict = NULL;

static Boolean  workbenchMode = FALSE;
static Boolean  subtoolMode = FALSE;
static Boolean  stdinMode = FALSE;
static Boolean  bioseqsetMode = FALSE;
static Boolean  binseqentryMode = FALSE;
static Boolean  entrezMode = FALSE;
static Boolean  nohelpMode = FALSE;
static Uint2    subtoolDatatype = 0;
static Uint2    subtoolEntityID = 0;

static Boolean  smartnetMode = FALSE;
static Boolean  dirsubMode = FALSE;

#ifdef WIN_MAC
static MenU     newDescMenu = NULL;
static MenU     newFeatMenu = NULL;
static MenU     newPubMenu = NULL;
static MenU     specialMenu = NULL;
static MenU     analysisMenu = NULL;
static Boolean  initialFormsActive = FALSE;
#endif

static ForM  initSubmitForm = NULL;
static ForM  formatForm = NULL;

static Int2     subtoolTimerLimit = 100;
static Int2     subtoolTimerCount = 0;
static Boolean  subtoolRecordDirty = FALSE;


#ifdef USE_SMARTNET
static Int4 SMWriteBioseqObj(VoidPtr bio_data, SMUserDataPtr sm_usr_data, 
                             VoidPtr data);
static Int4 SMReadBioseqObj(VoidPtr data, CharPtr buffer, 
                            Int4 length, Int4 fd);
#define SMART_KEY 1313
#endif

static void Sequin_RemoveInconsistentAlnsFromSet(SeqAlignPtr sap, Int4 fuzz, Int4 n);
static SeqAlignPtr Sequin_FindBestAlnByDotPlot(SeqLocPtr slp1, SeqLocPtr slp2);
static void SQN_ExtendAlnRight(SeqAlignPtr sap, Int4 which_row, Int4 start, Int4 stop);


static FormatBlock globalFormatBlock = {SEQ_PKG_SINGLE, SEQ_FMT_FASTA, 0};

ForM  helpForm = NULL;

static void write_annot(SeqAlignPtr align);

static CharPtr validFailMsg =
"Submission failed validation test.  Continue?\n\
(Choose Validate in the Search menu to see errors.)";

static void CheckForCookedBioseqs (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BoolPtr    bp;
  BioseqPtr  bsp;

  if (sep == NULL) return;
  if (! IS_Bioseq (sep)) return;
  bp = (BoolPtr) mydata;
  if (bp == NULL) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;
  if (bsp->repr != Seq_repr_raw && bsp->repr != Seq_repr_seg &&
  	bsp->repr != Seq_repr_delta && bsp->repr != Seq_repr_virtual) {
    *bp = FALSE;
  }
}

static Boolean OkayToWriteTheEntity (Uint2 entityID, ForM f)

{
  Boolean         allRawOrSeg = TRUE;
  MsgAnswer       ans;
  Int2            errors;
  Int2            j;
  ErrSev          oldErrSev;
  SeqEntryPtr     sep;
  Char            str [32];
  ValidStructPtr  vsp;

  if (entityID < 1) return FALSE;
  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep == NULL) return FALSE;
  if (GetAppParam ("SEQUIN", "PREFERENCES", "ASKBEFOREVALIDATE", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      if (! (subtoolMode ||smartnetMode) ) {
        if (Message (MSG_YN, "Do you wish to validate this entry?") == ANS_NO) return TRUE;
      }
    }
  }
  WatchCursor ();
  Update ();
  vsp = ValidStructNew ();
  if (vsp != NULL) {
    /*SetChecklistValue (checklistForm, 6);*/
    SeqEntryExplore (sep, (Pointer) (&allRawOrSeg), CheckForCookedBioseqs);
    if (allRawOrSeg) {
      vsp->useSeqMgrIndexes = TRUE;
    }
    oldErrSev = ErrSetMessageLevel (SEV_MAX);
    ValidateSeqEntry (sep, vsp);
    ErrSetMessageLevel (oldErrSev);
    ErrClear ();
    ErrShow ();
    errors = 0;
    if (subtoolMode || smartnetMode) {
      for (j = 0; j < 6; j++) {
        errors += vsp->errors [j];
      }
    } else {
      for (j = 3; j < 6; j++) {
        errors += vsp->errors [j];
      }
    }
    ValidStructFree (vsp);
    UseWindow ((WindoW) f);
    if (errors > 0) {
      ArrowCursor ();
      Update ();
      if (subtoolMode || smartnetMode) {
        ans = Message (MSG_OKC, "%s\nFatal %d, Error %d, Warning %d, Info %d\n%s",
                       "Submission failed validation test with:",
                       (int) vsp->errors [4], (int) vsp->errors [3],
                       (int) vsp->errors [2], (int) vsp->errors [1],
                       "Continue anyway?");
      } else {
        ans = Message (MSG_OKC, validFailMsg);
      }
      if (ans != ANS_OK) {
        /*SetChecklistValue (checklistForm, 5);*/
        return FALSE;
      }
    }
  }
  ArrowCursor ();
  Update ();
  return TRUE;
}

static Int2 GetSequinAppParam (CharPtr section, CharPtr type, CharPtr dflt, CharPtr buf, Int2 buflen)

{
  Int2  rsult;

  rsult = GetAppParam ("SEQUINCUSTOM", section, type, NULL, buf, buflen);
  if (rsult) return rsult;
  rsult = GetAppParam ("SEQUIN", section, type, dflt, buf, buflen);
  return rsult;
}

static Boolean SetSequinAppParam (CharPtr section, CharPtr type, CharPtr value)

{
  Char  tmp [32];

  if (GetAppParam ("SEQUINCUSTOM", section, type, NULL, tmp, sizeof (tmp) - 1)) {
    return SetAppParam ("SEQUINCUSTOM", section, type, value);
  }
  return SetAppParam ("SEQUIN", section, type, value);
}

static void SetSequinAppParamTF (CharPtr section, CharPtr type, Boolean value)

{
  if (value) {
    SetSequinAppParam (section, type, "TRUE");
  } else {
    SetSequinAppParam (section, type, "FALSE");
  }
}

static void ReplaceString (CharPtr PNTR target, CharPtr newstr)

{
  if (target == NULL) return;
  MemFree (*target);
  *target = StringSaveNoNull (newstr);
}

static Boolean WriteTheEntityID (Uint2 entityID, CharPtr path)

{
  AsnIoPtr       aip;
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  ObjMgrDataPtr  omdp;
  Boolean        rsult;
  SeqEntryPtr    sep;
  SeqSubmitPtr   ssp;
  Char           str [16];
#ifdef WIN_MAC
  FILE           *f;
#endif

  rsult = FALSE;
  if (entityID < 1) return rsult;
  if (path == NULL || path [0] == '\0') return rsult;
  ssp = NULL;
  sep = NULL;
  bsp = NULL;
  bssp = NULL;
  omdp = ObjMgrGetData (entityID);
  if (omdp == NULL) return rsult;
  WatchCursor ();
  Update ();
#ifdef WIN_MAC
  f = FileOpen (path, "r");
  if (f != NULL) {
    FileClose (f);
  } else {
    FileCreate (path, "TEXT", "ttxt");
  }
#endif
  if (GetSequinAppParam ("PREFERENCES", "UNCOMPRESS", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      sep = GetTopSeqEntryForEntityID (entityID);
      SeqEntryConvert (sep, Seq_code_iupacna);
      SeqEntryConvert (sep, Seq_code_ncbieaa);
    }
  }
  if (binseqentryMode) {
    aip = AsnIoOpen (path, "w");
    sep = GetTopSeqEntryForEntityID (entityID);
    rsult = SeqEntryAsnWrite (sep, aip, NULL);
    AsnIoClose (aip);
    ArrowCursor ();
    Update ();
    return rsult;
  }
  sep = NULL;
  aip = AsnIoOpen (path, "w");
  if (aip != NULL) {
    switch (omdp->datatype) {
      case OBJ_SEQSUB :
        ssp = (SeqSubmitPtr) omdp->dataptr;
        if (ssp != NULL && ssp->datatype == 1) {
          rsult = SeqSubmitAsnWrite (ssp, aip, NULL);
        }
        break;
      case OBJ_BIOSEQ :
        sep = (SeqEntryPtr) omdp->choice;
        if (sep != NULL && sep->choice == 1 && sep->data.ptrvalue != NULL) {
          if (subtoolMode || bioseqsetMode) {
            bsp = (BioseqPtr) sep->data.ptrvalue;
            rsult = BioseqAsnWrite (bsp, aip, NULL);
          } else {
            rsult = SeqEntryAsnWrite (sep, aip, NULL);
          }
        }
        break;
      case OBJ_BIOSEQSET :
        sep =  (SeqEntryPtr) omdp->choice;
        if (sep != NULL && sep->choice == 2 && sep->data.ptrvalue != NULL) {
          if (subtoolMode || bioseqsetMode) {
            bssp = (BioseqSetPtr) sep->data.ptrvalue;
            rsult = BioseqSetAsnWrite (bssp, aip, NULL);
          } else {
            rsult = SeqEntryAsnWrite (sep, aip, NULL);
          }
        }
        break;
      case OBJ_SEQENTRY :
        sep =  (SeqEntryPtr) omdp->choice;
        if (sep != NULL) {
          rsult = SeqEntryAsnWrite (sep, aip, NULL);
        }
        break;
      default :
        break;
    }
    AsnIoClose (aip);
    if (! smartnetMode) {
      ObjMgrSetDirtyFlag (entityID, FALSE);
    }
  }
  ArrowCursor ();
  Update ();
  return rsult;
}

extern Boolean PropagateFromGenBankBioseqSet (SeqEntryPtr sep, Boolean ask)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Uint1         _class;
  SeqEntryPtr   seqentry;
  ValNodePtr    sourcedescr;

  if (sep != NULL) {
    if (sep->choice == 2 && sep->data.ptrvalue != NULL) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      _class = bssp->_class;
      sourcedescr = bssp->descr;
      if (sourcedescr == NULL) return FALSE;
      if (_class == 7) {
        if (ask) {
          if (Message (MSG_YN, "Propagate descriptors from top-level set?") == ANS_NO) return FALSE;
        }
        seqentry = bssp->seq_set;
        while (seqentry != NULL) {
          if (seqentry->data.ptrvalue != NULL) {
            if (seqentry->choice == 1) {
              bsp = (BioseqPtr) seqentry->data.ptrvalue;
              ValNodeLink (&(bsp->descr),
                           AsnIoMemCopy ((Pointer) sourcedescr,
                                         (AsnReadFunc) SeqDescrAsnRead,
                                         (AsnWriteFunc) SeqDescrAsnWrite));
            } else if (seqentry->choice == 2) {
              bssp = (BioseqSetPtr) seqentry->data.ptrvalue;
              ValNodeLink (&(bssp->descr),
                           AsnIoMemCopy ((Pointer) sourcedescr,
                                         (AsnReadFunc) SeqDescrAsnRead,
                                         (AsnWriteFunc) SeqDescrAsnWrite));
            }
          }
          seqentry = seqentry->next;
        }
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        bssp->descr = SeqDescrFree (bssp->descr);
        return TRUE;
      }
    }
  }
  return FALSE;
}

static void ForcePropagate (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = (BaseFormPtr) GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  PropagateFromGenBankBioseqSet (sep, FALSE);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

#define SEQUIN_EDIT_TEMP_FILE "sequinEdit.temp"
#define SEQUIN_EDIT_BACK_FILE "sequinEdit.back"
#define SEQUIN_EDIT_PREV_FILE "sequinEdit.prev"
#define SEQUIN_EDIT_ARCH_FILE "sequinEdit.arch"

static void SubtoolModeTimerProc (void)

{
  ObjMgrDataPtr  omdp;

  subtoolTimerCount++;
  if (subtoolTimerCount > subtoolTimerLimit) {
    subtoolTimerCount = 0;
    if (subtoolRecordDirty) {
      omdp = ObjMgrGetData (subtoolEntityID);
      if (omdp != NULL) {
        if (WriteTheEntityID (subtoolEntityID, SEQUIN_EDIT_TEMP_FILE)) {
          FileRemove (SEQUIN_EDIT_PREV_FILE);
          FileRename (SEQUIN_EDIT_BACK_FILE, SEQUIN_EDIT_PREV_FILE);
          FileRename (SEQUIN_EDIT_TEMP_FILE, SEQUIN_EDIT_BACK_FILE);
        } else {
          Message (MSG_POSTERR, "Unable to save automatic temporary file");
        }
      }
      subtoolRecordDirty = FALSE;
    }
  }
  SequinCheckSocketsProc ();
}

static Int2 LIBCALLBACK SubtoolModeMsgFunc (OMMsgStructPtr ommsp)

{
  switch (ommsp->message) {
    case OM_MSG_DEL :
    case OM_MSG_CREATE :
    case OM_MSG_UPDATE :
      subtoolRecordDirty = TRUE;
      break;
    default :
      break;
  }
  return OM_MSG_RET_OK;
}

static void GetDefaultTitleFromForm (ForM f, CharPtr str, size_t maxsize, CharPtr filepath)

{
  Char     ch;
  Char     dfault [64];
  Int2     j;
  Int2     k;
  CharPtr  ptr;

  if (f != NULL && str != NULL && maxsize > 0) {
    dfault [0] = '\0';
    if (StringHasNoText (filepath)) {
      GetTitle (f, dfault, sizeof (dfault));
    } else {
      ptr = StringRChr (filepath, DIRDELIMCHR);
      if (ptr != NULL) {
        ptr++;
        StringNCpy_0 (dfault, ptr, sizeof (dfault));
      } else {
        StringNCpy_0 (dfault, filepath, sizeof (dfault));
      }
    }
    j = 0;
    k = 0;
    ch = dfault [j];
    while (j < sizeof (dfault) && ch != '\0') {
      if (ch <= ' ') {
        j++;
      } else {
        dfault [k] = dfault [j];
        k++;
        j++;
      }
      ch = dfault [j];
    }
    dfault [k] = '\0';
#ifdef WIN_MSWIN
    j = 0;
    ch = dfault [j];
    while (j < sizeof (dfault) && ch != '\0') {
      if (ch == '_' || IS_ALPHANUM (ch)) {
        j++;
        ch = dfault [j];
      } else {
        ch = '\0';
      }
    }
    dfault [j] = '\0';
#endif
    StringNCpy_0 (str, dfault, maxsize);
  }
}

static CharPtr gbsub = "gb-sub@ncbi.nlm.nih.gov";
static CharPtr emblsub = "datasubs@ebi.ac.uk";
static CharPtr ddbjsub = "ddbjsub@ddbj.nig.ac.jp";

static CharPtr gbupd = "gb-admin@ncbi.nlm.nih.gov";
static CharPtr emblupd = "update@ebi.ac.uk";
static CharPtr ddbjupd = "ddbjupdt@ddbj.nig.ac.jp";

static CharPtr ReturnSubmissionEmailAddress (Uint2 entityID)

{
  ObjMgrDataPtr   omdp;
  CharPtr         rsult;
  SubmitBlockPtr  sbp;
  SeqSubmitPtr    ssp;
  Char            str [32];
  Boolean         update = FALSE;

  rsult = gbsub;
  omdp = ObjMgrGetData (entityID);
  if (omdp != NULL && omdp->datatype == OBJ_SEQSUB) {
    ssp = (SeqSubmitPtr) omdp->dataptr;
    if (ssp != NULL && ssp->datatype == 1) {
      sbp = ssp->sub;
      if (sbp != NULL && sbp->subtype == 2) {
        update = TRUE;
      }
    }
  }
  if (GetAppParam ("SEQUIN", "PREFERENCES", "DATABASE", NULL, str, sizeof (str))) {
    if (StringICmp (str, "GenBank") == 0) {
      if (update) {
        rsult = gbupd;
      } else {
        rsult = gbsub;
      }
    } else if (StringICmp (str, "EMBL") == 0) {
      if (update) {
        rsult = emblupd;
      } else {
        rsult = emblsub;
      }
    } else if (StringICmp (str, "DDBJ") == 0) {
      if (update) {
        rsult = ddbjupd;
      } else {
        rsult = ddbjsub;
      }
    }
  }
  return rsult;
}

static void DoRemoveAlignmentFromRecord (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  SeqAnnotPtr   nextsap;
  Pointer PNTR  prevsap;
  SeqAnnotPtr   sap;

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
      *(prevsap) = sap->next;
      sap->next = NULL;
      SeqAnnotFree (sap);
    } else {
      prevsap = (Pointer PNTR) &(sap->next);
    }
    sap = nextsap;
  }
}

extern void SubmitToNCBI (IteM i);

static void PrepareSeqSubmitProc (IteM i)

{
  BaseFormPtr  bfp;
  Char         dfault [64];
  Char         path [PATH_MAX];
  CharPtr      ptr;
  SeqEntryPtr  sep;
  CharPtr      str;
  Boolean      update;

#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = (BaseFormPtr) GetObjectExtra (i);
#endif
  update = FALSE;
  if (bfp != NULL) {
    sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
    if (sep != NULL) {
      SeqEntryPack (sep);
      EntryChangeGBSource (sep);
      EntryCheckGBBlock (sep);
      GetRidOfEmptyFeatsDescStrings (0, sep);
      GetRidOfLocusInSeqIds (0, sep);
      if (OkayToWriteTheEntity (bfp->input_entityID, bfp->form)) {
        /*SetChecklistValue (checklistForm, 7);*/
        path [0] = '\0';
        StringNCpy_0 (path, bfp->filepath, sizeof (path));
        dfault [0] = '\0';
        GetDefaultTitleFromForm (bfp->form, dfault, sizeof (dfault), bfp->filepath);
        ptr = StringRChr (dfault, '.');
        if (ptr != NULL) {
          *ptr = '\0';
        }
        if (StringLen (dfault) < sizeof (dfault) - 5) {
          StringCat (dfault, ".sqn");
        }
        if (GetOutputFileName (path, sizeof (path), dfault)) {
          update = PropagateFromGenBankBioseqSet (sep, TRUE);
          if (SeqEntryHasAligns (bfp->input_entityID, sep)) {
            if (Message (MSG_YN, "Remove alignments?") == ANS_YES) {
              SeqEntryExplore (sep, NULL, DoRemoveAlignmentFromRecord);
              update = TRUE;
            }
          }
          if (WriteTheEntityID (bfp->input_entityID, path)) {
            /*SetChecklistValue (checklistForm, 5);*/
            ptr = StringRChr (path, DIRDELIMCHR);
            if (ptr != NULL) {
              ptr++;
              str = MemNew (StringLen (ptr) + 90);
              if (str != NULL) {
                sprintf (str, "Submission is now written.  Please e-mail '%s' to %s", ptr,
                         ReturnSubmissionEmailAddress (bfp->input_entityID));
                UseWindow ((WindoW) bfp->form);
                Message (MSG_OK, str);
                MemFree (str);
                if (update) {
                  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
                }
                return;
              }
            }
            str = MemNew (90);
            if (str != NULL) {
              sprintf (str, "Submission is now written.  Please e-mail to %s",
                       ReturnSubmissionEmailAddress (bfp->input_entityID));
              UseWindow ((WindoW) bfp->form);
              Message (MSG_OK, str);
              MemFree (str);
            }
          } else {
            UseWindow ((WindoW) bfp->form);
            Message (MSG_ERROR, "Unable to write file.");
          }
        } else {
          /*SetChecklistValue (checklistForm, 5);*/
          UseWindow ((WindoW) bfp->form);
        }
      }
    }
    if (update) {
      ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
    }
  }
}

extern Boolean SaveSeqSubmitProc (BaseFormPtr bfp, Boolean saveAs)

{
  Char         dfault [32];
  Char         path [PATH_MAX];
  CharPtr      ptr;
  SeqEntryPtr  sep;
  Boolean      update;

  if (bfp != NULL) {
    sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
    if (sep != NULL) {
      SeqEntryPack (sep);
      EntryChangeGBSource (sep);
      EntryCheckGBBlock (sep);
      GetRidOfEmptyFeatsDescStrings (0, sep);
      GetRidOfLocusInSeqIds (0, sep);
      path [0] = '\0';
      StringNCpy_0 (path, bfp->filepath, sizeof (path));
      if (StringHasNoText (path) || saveAs) {
        dfault [0] = '\0';
        GetDefaultTitleFromForm (bfp->form, dfault, sizeof (dfault), bfp->filepath);
        ptr = StringRChr (dfault, '.');
        if (ptr != NULL) {
          *ptr = '\0';
        }
        if (StringLen (dfault) < sizeof (dfault) - 5) {
          StringCat (dfault, ".sqn");
        }
        if (! (GetOutputFileName (path, sizeof (path), dfault))) return FALSE;
      }
      update = PropagateFromGenBankBioseqSet (sep, TRUE);
      if (WriteTheEntityID (bfp->input_entityID, path)) {
        bfp->filepath = MemFree (bfp->filepath);
        bfp->filepath = StringSave (path);
        if (update) {
          ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
        }
        return TRUE;
      } else {
        Message (MSG_ERROR, "Unable to write file.");
        if (update) {
          ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
        }
        return FALSE;
      }
    }
  }
  return FALSE;
}

static void LIBCALLBACK ValidNotify (ErrSev sev, int errcode, int subcode,
                                     Uint2 entityID, Uint2 itemID, Uint2 itemtype,
                                     Boolean select, Boolean dblClick)

{
  Int2  handled;

  if (dblClick && entityID > 0 && itemID > 0 && itemtype > 0) {
    WatchCursor ();
    handled = GatherProcLaunch (OMPROC_EDIT, FALSE, entityID, itemID,
                                itemtype, 0, 0, itemtype, 0);
    ArrowCursor ();
    if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
      Beep ();
    }
  }
}

#ifndef WIN_MAC
static void RemoveUpdateDates (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  ValNodePtr    nextsdp;
  Pointer PNTR  prevsdp;
  ValNodePtr    sdp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sdp = bsp->descr;
    prevsdp = (Pointer PNTR) &(bsp->descr);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sdp = bssp->descr;
    prevsdp = (Pointer PNTR) &(bssp->descr);
  } else return;
  while (sdp != NULL) {
    nextsdp = sdp->next;
    if (sdp->choice == Seq_descr_update_date) {
      *(prevsdp) = sdp->next;
      sdp->next = NULL;
      SeqDescFree (sdp);
    } else {
      prevsdp = (Pointer PNTR) &(sdp->next);
    }
    sdp = nextsdp;
  }
}

static CharPtr stillHasGBMsg =
"Source information in a GenBank block should be in a BioSource.  Should I fixup?";

#ifdef USE_SMARTNET
static void SmartResetProc (IteM i)
{
    BaseFormPtr   bfp;
    ForM          f = NULL;
    Int4          status;
    ObjMgrDataPtr omdp;  
    OMUserDataPtr omudp;
    SMUserDataPtr sm_usr_data;

    if((bfp = (BaseFormPtr) GetObjectExtra (i)) != NULL) {
        f = bfp->form;
        
        omudp = ObjMgrGetUserData(bfp->input_entityID, 0, 0, SMART_KEY);
        omdp = ObjMgrGetData (bfp->input_entityID);
        
        sm_usr_data = (SMUserDataPtr) omudp->userdata.ptrvalue;
        
        if(omdp->dirty == FALSE) {
            status = sm_usr_data->header->status;
            sm_usr_data->header->status = SMStatClosed;
            SMSendMsgToClient(sm_usr_data);
            sm_usr_data->header->status = status;
            return;
        }
    }
    return;
}
#endif

#ifdef USE_SMARTNET
static Boolean LIBCALLBACK AllGenBankOrRefSeq (BioseqPtr bsp, SeqMgrBioseqContextPtr bcontext)

{
  SeqMgrDescContext  dcontext;
  MolInfoPtr         mip;
  BoolPtr            resetUpdateDate;
  ValNodePtr         sdp;
  SeqIdPtr           sip;

  resetUpdateDate = (BoolPtr) bcontext->userdata;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_EMBL || sip->choice == SEQID_DDBJ) {
      *resetUpdateDate = FALSE;
    }
  }
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
  if (sdp != NULL) {
    mip = (MolInfoPtr) sdp->data.ptrvalue;
    if (mip != NULL) {
      if (mip->tech == MI_TECH_htgs_1 || mip->tech == MI_TECH_htgs_2 ||
          mip->tech == MI_TECH_htgs_3 || mip->tech == MI_TECH_htgs_0) {
        *resetUpdateDate = FALSE;
      }
    }
  }
  return TRUE;
}

static void SmartnetDoneFunc (BaseFormPtr bfp)

{
    MsgAnswer     ans;
    ForM          f = NULL;
    Boolean       hasGBStuff;
    ValNodePtr    sdp;
    SeqEntryPtr   sep;
    Boolean       update;
    
    ObjMgrDataPtr omdp;  
    OMUserDataPtr omudp;
    SMUserDataPtr sm_usr_data;
    Uint2         entityID;

    Boolean       resetUpdateDate = TRUE;

    if(bfp != NULL) {
        f = bfp->form;
        
        omudp = ObjMgrGetUserData(bfp->input_entityID, 0, 0, SMART_KEY);
        if (omudp == NULL) return;
        omdp = ObjMgrGetData (bfp->input_entityID);

        sm_usr_data = (SMUserDataPtr) omudp->userdata.ptrvalue;

        if(omdp->dirty == FALSE || (sm_usr_data->header->dirty & 0x02)) {
            sm_usr_data->header->status = SMStatClosed;
            SMSendMsgToClient(sm_usr_data);
            
            /*
            entityID = bfp->input_entityID;
            RemoveSeqEntryViewer (bfp->form);
            ObjMgrFreeUserData(entityID, 0, 0, SMART_KEY); 
            */
            
            /* ObjMgrSendMsg (OM_MSG_DEL, entityID, 0, 0); */
            ObjMgrFree(omdp->datatype, omdp->dataptr);
            
            return;
        }

        sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
        if (sep != NULL) {
            
            if (EntrezASN1Detected (sep)) {
                Message(MSG_OK, 
                        "You may not commit entry retrieved from Entrez.\n"
                        "Please close this window instead");
                return;
            }
            
            SeqEntryPack (sep);
            EntryChangeGBSource (sep);
            hasGBStuff = EntryCheckGBBlock (sep);
            GetRidOfEmptyFeatsDescStrings (0, sep);
            GetRidOfLocusInSeqIds (0, sep);

            if (hasGBStuff) {
                /*  ans = Message (MSG_YNC, stillHasGBMsg);
                    if (ans == ANS_CANCEL) return;
                    if (ans == ANS_YES) { */
                MySeqEntryToAsn3 (sep, TRUE, FALSE, FALSE);
                /* } */
            }
            move_cds (sep);
            /* now instantiating protein titles */
            InstantiateProteinTitles (bfp->input_entityID, NULL);
            update = PropagateFromGenBankBioseqSet (sep, FALSE);

            if (! OkayToWriteTheEntity (bfp->input_entityID, f)) {
                if (update && bfp != NULL) {
                    ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
                }
                return;
            }
            /* ans = Message (MSG_YN, "Reset Update Date?"); */
            ans = ANS_YES;

            /* reset update date in smart mode if GenBank or RefGene, and not HTGS */

            SeqMgrExploreBioseqs (entityID, 0, (Pointer) resetUpdateDate, AllGenBankOrRefSeq, TRUE, TRUE, TRUE);
            if (/* ans == ANS_YES */ resetUpdateDate) {
                SeqEntryExplore (sep, NULL, RemoveUpdateDates);
                sdp = CreateNewDescriptor (sep, Seq_descr_update_date);
                if (sdp != NULL) {
                    sdp->data.ptrvalue = DateCurr ();
                }
                PropagateFromGenBankBioseqSet (sep, FALSE);
            }
            /*   SeqEntryPubsAsn3 (sep);   */
            CdCheck (sep, NULL);
        }

        if(sm_usr_data->header->format == OBJ_SEQENTRY) {
            SMWriteBioseqObj(sep, sm_usr_data, NULL);
        } else {
            sm_usr_data->header->format = omdp->datatype;
            SMWriteBioseqObj(omdp->dataptr, sm_usr_data, NULL); 
        }

        /* ObjMgrDelete(omdp->datatype, omdp->dataptr); */

        omdp->dirty = FALSE;
        
        /*
        entityID = bfp->input_entityID;
        RemoveSeqEntryViewer (bfp->form);  
        ObjMgrFreeUserData(entityID, 0, 0, SMART_KEY); 
        */

        /* ObjMgrSendMsg (OM_MSG_DEL, entityID, 0, 0); */
        ObjMgrFree(omdp->datatype, omdp->dataptr);

        subtoolRecordDirty = FALSE;
        FileRemove (SEQUIN_EDIT_TEMP_FILE);
        FileRemove (SEQUIN_EDIT_PREV_FILE);
        /* FileRemove (SEQUIN_EDIT_BACK_FILE); */
        FileRemove (SEQUIN_EDIT_ARCH_FILE);
        FileRename (SEQUIN_EDIT_BACK_FILE, SEQUIN_EDIT_ARCH_FILE);
        return;
        
    } else {
        Message(MSG_ERROR, "NULL pointer for brp ...? BUG!BUG!BUG!");
        return;
    }
}

static void SmartnetDoneProc (IteM i)

{
	BaseFormPtr bfp;

	bfp = (BaseFormPtr) GetObjectExtra (i);
	SmartnetDoneFunc (bfp);
}
#endif

static void SubtoolDoneProc (IteM i)

{
  MsgAnswer    ans;
  BaseFormPtr  bfp;
  ForM         f;
  Boolean      hasGBStuff;
  ValNodePtr   sdp;
  SeqEntryPtr  sep;
  Boolean      update;

  if (subtoolEntityID > 0) {
    sep = GetTopSeqEntryForEntityID (subtoolEntityID);
    if (sep != NULL) {
      SeqEntryPack (sep);
      EntryChangeGBSource (sep);
      hasGBStuff = EntryCheckGBBlock (sep);
      GetRidOfEmptyFeatsDescStrings (0, sep);
      GetRidOfLocusInSeqIds (0, sep);
      if (hasGBStuff) {
        ans = Message (MSG_YNC, stillHasGBMsg);
        if (ans == ANS_CANCEL) return;
        if (ans == ANS_YES) {
          MySeqEntryToAsn3 (sep, TRUE, FALSE, FALSE);
        }
      }
      move_cds (sep);
      /* now instantiating protein titles */
      InstantiateProteinTitles (subtoolEntityID, NULL);
      update = PropagateFromGenBankBioseqSet (sep, FALSE);
      f = NULL;
      bfp = (BaseFormPtr) GetObjectExtra (i);
      if (bfp != NULL) {
        f = bfp->form;
      }
      if (! OkayToWriteTheEntity (subtoolEntityID, f)) {
        if (update && bfp != NULL) {
          ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
        }
        return;
      }
      /* ans = Message (MSG_YN, "Reset Update Date?"); */
      ans = ANS_YES;
      if (ans == ANS_YES) {
        SeqEntryExplore (sep, NULL, RemoveUpdateDates);
        sdp = CreateNewDescriptor (sep, Seq_descr_update_date);
        if (sdp != NULL) {
          sdp->data.ptrvalue = DateCurr ();
        }
        PropagateFromGenBankBioseqSet (sep, FALSE);
      }
      /*
      SeqEntryPubsAsn3 (sep);
      */
      CdCheck (sep, NULL);
    }
    /*SetChecklistValue (checklistForm, 7);*/
    if (WriteTheEntityID (subtoolEntityID, "stdout")) {
      subtoolRecordDirty = FALSE;
      FileRemove (SEQUIN_EDIT_TEMP_FILE);
      FileRemove (SEQUIN_EDIT_PREV_FILE);
      /* FileRemove (SEQUIN_EDIT_BACK_FILE); */
      FileRemove (SEQUIN_EDIT_ARCH_FILE);
      FileRename (SEQUIN_EDIT_BACK_FILE, SEQUIN_EDIT_ARCH_FILE);
    } else {
      Message (MSG_POSTERR, "Unable to write ASN.1 file");
      return;
    }
    /*SetChecklistValue (checklistForm, 5);*/
  }
  QuitProgram ();
}
#endif

static void ProcessDoneButton (ForM f)

{
  Boolean         allRawOrSeg = TRUE;
  BaseFormPtr     bfp;
  Int2            errors;
  Int2            j;
  ErrHookProc     oldErrHook;
  ErrSev          oldErrSev;
  SeqEntryPtr     sep;
  CharPtr         str;
  ValidStructPtr  vsp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
#ifndef WIN_MAC
  if (smartnetMode) {
#ifdef USE_SMARTNET
    SmartnetDoneProc ((IteM) f);
#endif
    return;
  }
  if (subtoolMode || stdinMode || binseqentryMode) {
    SubtoolDoneProc ((IteM) f);
    return;
  }
#endif
  WatchCursor ();
  Update ();
  vsp = ValidStructNew ();
  if (vsp != NULL) {
    SeqEntryExplore (sep, (Pointer) (&allRawOrSeg), CheckForCookedBioseqs);
    if (allRawOrSeg) {
      vsp->useSeqMgrIndexes = TRUE;
    }
    oldErrSev = ErrSetMessageLevel (SEV_MAX);
    ValidateSeqEntry (sep, vsp);
    ErrSetMessageLevel (oldErrSev);
    ErrClear ();
    ErrShow ();
    errors = 0;
    for (j = 3; j < 6; j++) {
      errors += vsp->errors [j];
    }
    ValidStructFree (vsp);
    if (errors > 0) {
      ArrowCursor ();
      Update ();
      if (Message (MSG_YN,
        "Submission failed validation test.  Do you wish to see the errors?") == ANS_YES) {
        WatchCursor ();
        Update ();
        vsp = ValidStructNew ();
        if (vsp != NULL) {
          /*SetChecklistValue (checklistForm, 6);*/
          CreateValidateWindowEx (ValidNotify, "Sequin Validation Errors",
                                  programFont, SEV_INFO, TRUE, bfp, ProcessDoneButton, TRUE);
          ClearValidateWindow ();
          SeqEntryExplore (sep, (Pointer) (&allRawOrSeg), CheckForCookedBioseqs);
          if (allRawOrSeg) {
            vsp->useSeqMgrIndexes = TRUE;
          }
          HideValidateDoc ();
          vsp->suppressContext = ShouldSetSuppressContext ();
          oldErrHook = ErrSetHandler (ValidErrHook);
          oldErrSev = ErrSetMessageLevel (SEV_NONE);
          ValidateSeqEntry (sep, vsp);
          ErrSetMessageLevel (oldErrSev);
          ErrSetHandler (oldErrHook);
          ErrClear ();
          ShowValidateDoc ();
          errors = 0;
          for (j = 0; j < 6; j++) {
            errors += vsp->errors [j];
          }
          if (errors == 0) {
            ArrowCursor ();
            Message (MSG_OK, "Validation test succeeded.");
            FreeValidateWindow ();
          } else {
            RepopulateValidateFilter ();
          }
          ValidStructFree (vsp);
          /*SetChecklistValue (checklistForm, 5);*/
        }
        ArrowCursor ();
        Update ();
        return;
      }
    }
    ArrowCursor ();
    Update ();
    if (Message (MSG_YN, "Are you ready to save the record?") == ANS_YES) {
      if (SaveSeqSubmitProc (bfp, TRUE)) {
        str = MemNew (90);
        if (str != NULL) {
          sprintf (str, "Submission is now written.  Please e-mail to %s",
                   ReturnSubmissionEmailAddress (bfp->input_entityID));
          UseWindow ((WindoW) bfp->form);
          Message (MSG_OK, str);
          MemFree (str);
        }
      }
    }
  }
  ArrowCursor ();
  Update ();
}

static void CloseAboutWindowProc (WindoW w)

{
  Remove (w);
}

static void CloseAboutPanelProc (PaneL p, PoinT pt)

{
  WindoW  w;

  w = ParentWindow (p);
  Remove (w);
}

static void AboutProc (IteM i)

{
  PaneL   p;
  WindoW  w;

  w = ModalWindow (-50, -33, -1, -1, CloseAboutWindowProc);
  p = SimplePanel (w, AboutBoxWidth (), AboutBoxHeight (), DrawAbout);
  SetPanelClick (p, NULL, NULL, NULL, CloseAboutPanelProc);
  Show (w);
  Select (w);
}

static void StyleManagerProc (IteM i)

{
  MuskStyleManager ();
}

extern Boolean SequinEntrezInit (CharPtr appl_id, Boolean no_warnings, BoolPtr is_network)

{
  MonitorPtr  mon;
  Boolean     rsult;

  mon = MonitorStrNewEx ("Sequin", 30, FALSE);
  MonitorStrValue (mon, "Connecting to Entrez service");
  Update ();
  rsult = EntrezInit (appl_id, no_warnings, is_network);
  MonitorFree (mon);
  Update ();
  return rsult;
}

/*
#ifndef WIN16
static void Cn3DWinShowProc (IteM i)
{
  WindoW  w;

  if (! BiostrucAvail ()) return;
  if (! EntrezIsInited ()) {
    SequinEntrezInit ("Sequin", FALSE, NULL);
  }
  w = Cn3DWin_Entrez(NULL, useEntrez);
  if (w == NULL) return;
  Show (w);
  Select (w);
}
#endif
*/

extern void ValSeqEntryForm (ForM f)

{
  Boolean         allRawOrSeg = TRUE;
  BaseFormPtr     bfp;
  Int2            errors;
  Int2            j;
  ErrHookProc     oldErrHook;
  ErrSev          oldErrSev;
  SeqEntryPtr     sep;
  ValidStructPtr  vsp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
    if (sep != NULL) {
      /*
      if (! EntrezIsInited ()) {
        SequinEntrezInit ("Sequin", FALSE, NULL);
      }
      */
      vsp = ValidStructNew ();
      if (vsp != NULL) {
        WatchCursor ();
        Update ();
        /*SetChecklistValue (checklistForm, 6);*/
        CreateValidateWindowEx (ValidNotify, "Sequin Validation Errors",
                                programFont, SEV_INFO, TRUE, bfp, ValSeqEntryForm, TRUE);
        ClearValidateWindow ();
        SeqEntryExplore (sep, (Pointer) (&allRawOrSeg), CheckForCookedBioseqs);
        if (allRawOrSeg) {
          vsp->useSeqMgrIndexes = TRUE;
        }
        HideValidateDoc ();
        vsp->suppressContext = ShouldSetSuppressContext ();
        vsp->validateAlignments = TRUE;
        vsp->farIDsInAlignments = (Boolean) (subtoolMode || smartnetMode || dirsubMode);
        oldErrHook = ErrSetHandler (ValidErrHook);
        oldErrSev = ErrSetMessageLevel (SEV_NONE);
        ValidateSeqEntry (sep, vsp);
        ErrSetMessageLevel (oldErrSev);
        ErrSetHandler (oldErrHook);
        ErrClear ();
        ShowValidateDoc ();
        errors = 0;
        for (j = 0; j < 6; j++) {
          errors += vsp->errors [j];
        }
        if (errors == 0) {
          ArrowCursor ();
          Message (MSG_OK, "Validation test succeeded.");
          FreeValidateWindow ();
        } else {
          RepopulateValidateFilter ();
        }
        ValidStructFree (vsp);
        /*SetChecklistValue (checklistForm, 5);*/
        ArrowCursor ();
        Update ();
      }
    }
  }
}

static void ValSeqEntryProc (IteM i)

{
  BaseFormPtr  bfp;

#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = (BaseFormPtr) GetObjectExtra (i);
#endif
  if (bfp != NULL) {
    ValSeqEntryForm (bfp->form);
  }
}

#ifdef USE_SPELL
#ifdef WIN_MOTIF
static void SpellCheckTheForm (ForM f)

{
  Boolean         allRawOrSeg = TRUE;
  BaseFormPtr     bfp;
  Int2            errors;
  Int2            j;
  MonitorPtr      mon;
  ErrHookProc     oldErrHook;
  ErrSev          oldErrSev;
  SeqEntryPtr     sep;
  ValidStructPtr  vsp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
    if (sep != NULL) {
      WatchCursor ();
      Update ();
      vsp = ValidStructNew ();
      if (vsp != NULL) {
        /*SetChecklistValue (checklistForm, 6);*/
        mon = MonitorStrNewEx ("SpellCheck", 40, FALSE);
        MonitorStrValue (mon, "Connecting to Spell");
        Update ();
        if (! SpellInit ()) {
          /*SetChecklistValue (checklistForm, 5);*/
          ArrowCursor ();
          MonitorFree (mon);
          Update ();
          Message (MSG_ERROR, "Unable to initialize Spell service.");
          Update ();
          return;
        }
        MonitorStrValue (mon, "Performing Spell Check");
        Update ();
        vsp->spellfunc = SpellCheck;
        vsp->spellcallback = SpellCallBack;
        vsp->onlyspell = TRUE;
        vsp->justwarnonspell = TRUE;
        CreateValidateWindowEx (ValidNotify, "Sequin Spell Check Errors",
                                programFont, SEV_INFO, TRUE, bfp, SpellCheckTheForm, TRUE);
        ClearValidateWindow ();
        SeqEntryExplore (sep, (Pointer) (&allRawOrSeg), CheckForCookedBioseqs);
        if (allRawOrSeg) {
          vsp->useSeqMgrIndexes = TRUE;
        }
        HideValidateDoc ();
        vsp->suppressContext = ShouldSetSuppressContext ();
        oldErrHook = ErrSetHandler (ValidErrHook);
        oldErrSev = ErrSetMessageLevel (SEV_NONE);
        ValidateSeqEntry (sep, vsp);
        ErrSetMessageLevel (oldErrSev);
        ErrSetHandler (oldErrHook);
        ErrClear ();
        ShowValidateDoc ();
        errors = 0;
        for (j = 0; j < 6; j++) {
          errors += vsp->errors [j];
        }
        if (errors == 0) {
          ArrowCursor ();
          Message (MSG_OK, "Spelling check succeeded.");
          FreeValidateWindow ();
        } else {
          RepopulateValidateFilter ();
        }
        ValidStructFree (vsp);
        MonitorStrValue (mon, "Closing Spell Check");
        Update ();
        SpellFini ();
        /*SetChecklistValue (checklistForm, 5);*/
        MonitorFree (mon);
      }
      ArrowCursor ();
      Update ();
    }
  }
}

static void SpellCheckSeqEntryProc (IteM i)

{
  BaseFormPtr  bfp;

#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = (BaseFormPtr) GetObjectExtra (i);
#endif
  if (bfp != NULL) {
    SpellCheckTheForm (bfp->form);
  }
}
#endif
#endif

extern Int4 MySeqEntryToAsn3Ex (SeqEntryPtr sep, Boolean strip, Boolean correct, Boolean force, Boolean dotaxon);
extern Int4 MySeqEntryToAsn3 (SeqEntryPtr sep, Boolean strip, Boolean correct, Boolean force)

{
  Boolean  dotaxon;

  dotaxon = FALSE;
/*#ifdef INTERNAL_NCBI_SEQUIN*/
  if (indexerVersion) {
    if (subtoolMode || smartnetMode) {
      dotaxon = TRUE;
    }
  }
/*#endif*/
  return MySeqEntryToAsn3Ex (sep, strip, correct, force, dotaxon);
}

static Int2  onthisline;
static Int2  remaining;

static void AddMessageToString (CharPtr str, ValNodePtr vnp)

{
  if (str != NULL && vnp != NULL) {
    StringCat (str, (CharPtr) vnp->data.ptrvalue);
    onthisline++;
    remaining--;
    if (remaining == 0) {
      StringCat (str, "\n\n");
    } else if (onthisline < 3) {
      StringCat (str, ", ");
    } else {
      StringCat (str, ",\n");
      onthisline = 0;
    }
  }
}

static Boolean DisplayTestResults (ValNodePtr head)

{
  MsgAnswer    ans;
  Int2         errors;
  size_t       len;
  CharPtr      str;
  ValNodePtr   vnp;
  Int2         warnings;

  if (head != NULL) {

    vnp = head;
    len = 200;
    while (vnp != NULL) {
      len += StringLen ((CharPtr) vnp->data.ptrvalue) + 2;
      vnp = vnp->next;
    }

    errors = 0;
    warnings = 0;
    vnp = head;
    while (vnp != NULL) {
      if (vnp->choice == 0) {
        warnings++;
      } else {
        errors++;
      }
      vnp = vnp->next;
    }

    str = MemNew (len + 2);
    if (str == NULL) return TRUE;
    str [0] = '\0';

    if (errors > 0) {
      StringCat (str, "The following essential information is missing:\n\n");
      onthisline = 0;
      remaining = errors;
      vnp = head;
      while (vnp != NULL) {
        if (vnp->choice != 0) {
          AddMessageToString (str, vnp);
        }
        vnp = vnp->next;
      }
      StringCat (str, "Please fill in the essential information.");
      ans = Message (MSG_OK, str);
      str = MemFree (str);
      return FALSE;
    }

    if (warnings > 0) {
      StringCat (str, "The following desired information is missing:\n\n");
      onthisline = 0;
      remaining = warnings;
      vnp = head;
      while (vnp != NULL) {
        if (vnp->choice == 0) {
          AddMessageToString (str, vnp);
        }
        vnp = vnp->next;
      }
      StringCat (str, "Do you wish to proceed anyway?");
      ans = Message (MSG_YN, str);
      str = MemFree (str);
      if (ans == ANS_NO) return FALSE;
      return TRUE;
    }

    str = MemFree (str);
  }
  return TRUE;
}

extern void JustRegisterSeqEntry (BaseFormPtr bfp, Boolean freeit)

{
  Int2  handled;

  if (bfp != NULL) {
    Hide (bfp->form);
  }
  seqviewprocs.filepath = globalPath;
  seqviewprocs.forceSeparateViewer = TRUE;
  SeqEntrySetScope (NULL);
  handled = GatherProcLaunch (OMPROC_VIEW, FALSE, globalEntityID, 1,
                              OBJ_BIOSEQ, 0, 0, OBJ_BIOSEQ, 0);
  seqviewprocs.filepath = NULL;
  ArrowCursor ();
  if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
    Message (MSG_FATAL, "Unable to launch viewer.");
  } else {
    SendHelpScrollMessage (helpForm, "Editing the Record", NULL);
  }
  ObjMgrSetOptions (OM_OPT_FREE_IF_NO_VIEW, globalEntityID);
  ObjMgrSetDirtyFlag (globalEntityID, TRUE);
  if (bfp != NULL && freeit) {
    Remove (bfp->form);
  }
}

extern void JustRegisterSeqEntryBtn (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  JustRegisterSeqEntry (bfp, TRUE);
}

static void JustRegisterSeqEntryForm (ForM f)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  JustRegisterSeqEntry (bfp, FALSE);
}

extern void AddSubmitBlockToSeqEntry (ForM f)

{
  AffilPtr        affil;
  AuthListPtr     authors;
  BaseFormPtr     bfp;
  CitSubPtr       csp;
  ValNodePtr      head;
  SubmitBlockPtr  sbp;
  SeqSubmitPtr    ssp;
  CitSubPtr       tmp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    head = TestForm (bfp->form);
    if (head != NULL) {
      if (! DisplayTestResults (head)) {
        ValNodeFreeData (head);
        return;
      }
      ValNodeFreeData (head);
    }
    Hide (bfp->form);
    /*
    globalsbp = (SequinBlockPtr) FormToPointer (bfp->form);
    if (globalsbp == NULL) {
      Message (MSG_OK, "Record will be a Seq-entry instead of a Seq-submit.");
    }
    */
    sbp = (SubmitBlockPtr) FormToPointer (bfp->form);
    if (sbp == NULL) {
      Message (MSG_OK, "Record will be a Seq-entry instead of a Seq-submit.");
    }
    Update ();
    /*
    globalEntityID = PackageFormResults (globalsbp, globalsep, FALSE);
    */
    globalEntityID = 0;
    if (globalsep != NULL) {
      if (sbp != NULL) {
        if (sbp->contact != NULL && sbp->cit != NULL) {
          tmp = CitSubFromContactInfo (sbp->contact);
          csp = sbp->cit;
          if (csp->authors != NULL) {
            authors = csp->authors;
            if (authors->affil == NULL) {
              if (tmp != NULL && tmp->authors != NULL) {
                authors = tmp->authors;
                affil = authors->affil;
                authors->affil = NULL;
                authors = csp->authors;
                authors->affil = affil;
                if (affil != NULL) {
                  affil->phone = MemFree (affil->phone);
                  affil->fax = MemFree (affil->fax);
                  affil->email = MemFree (affil->email);
                }
              }
            }
          }
          CitSubFree (tmp);
        }
        ssp = SeqSubmitNew ();
        if (ssp != NULL) {
          ssp->datatype = 1;
          ssp->sub = sbp;
          ssp->data = (Pointer) globalsep;
          ObjMgrConnect (OBJ_SEQENTRY, globalsep->data.ptrvalue, OBJ_SEQSUB, (Pointer) ssp);
          if (! ObjMgrRegister (OBJ_SEQSUB, (Pointer) ssp)) {
            ErrPostEx (SEV_ERROR, 0, 0, "ObjMgrRegister failed.");
          }
        } else {
          if (! ObjMgrRegister (OBJ_SEQENTRY, (Pointer) globalsep)) {
            ErrPostEx (SEV_ERROR, 0, 0, "ObjMgrRegister failed.");
          }
        }
      } else {
        if (! ObjMgrRegister (OBJ_SEQENTRY, (Pointer) globalsep)) {
          ErrPostEx (SEV_ERROR, 0, 0, "ObjMgrRegister failed.");
        }
      }
      if (EntrezASN1Detected (globalsep)) {
        ErrPostEx (SEV_WARNING, 0, 0, "This record was retrieved from Entrez.");
      }
      globalEntityID = ObjMgrGetEntityIDForChoice (globalsep);
    }
    globalsbp = NULL;
    globalsep = NULL;
    JustRegisterSeqEntryForm (f);
  }
}

#ifdef WIN_MAC
static void SubmitBlockActivateProc (WindoW w);
static void GenomeFormActivateProc (WindoW w);
#else
#define SubmitBlockActivateProc NULL
#define GenomeFormActivateProc NULL
#endif

CharPtr repackageMsg =
"Do you plan to submit this as an update to one of the databases?";

static DialoG hiddengenbio = NULL;

extern Boolean ProcessOneNucleotideTitle (Int2 seqPackage, DialoG genbio, PopuP genome,
                                          PopuP gencode, SeqEntryPtr nsep, SeqEntryPtr top,
                                          BioSourcePtr masterbiop);

static Boolean HandleOneNewAsnProc (BaseFormPtr bfp, Boolean removeold, Boolean askForSubmit,
                                    CharPtr path, Pointer dataptr, Uint2 datatype, Uint2 entityID,
                                    Uint2Ptr updateEntityIDPtr)

{
  BioseqPtr     bsp;
  ValNodePtr    bsplist = NULL;
  BioseqSetPtr  bssp;
  Int2          handled;
  SeqEntryPtr   nsep;
  Boolean       processonenuc;
  SeqEntryPtr   sep;
  ForM          w;
  WindoW        wnd;

  if (dataptr != NULL && entityID > 0) {
    if (datatype == OBJ_SEQSUB || datatype == OBJ_SEQENTRY ||
        datatype == OBJ_BIOSEQ || datatype == OBJ_BIOSEQSET) {
      WatchCursor ();
      sep = GetTopSeqEntryForEntityID (entityID);
      if (sep == NULL) {
        sep = SeqEntryNew ();
        if (sep != NULL) {
          if (datatype == OBJ_BIOSEQ) {
            bsp = (BioseqPtr) dataptr;
            sep->choice = 1;
            sep->data.ptrvalue = bsp;
            SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, sep);
          } else if (datatype == OBJ_BIOSEQSET) {
            bssp = (BioseqSetPtr) dataptr;
            sep->choice = 2;
            sep->data.ptrvalue = bssp;
            SeqMgrSeqEntry (SM_BIOSEQSET, (Pointer) bssp, sep);
          } else {
            sep = SeqEntryFree (sep);
          }
        }
        sep = GetTopSeqEntryForEntityID (entityID);
      }
      if (sep != NULL) {
        if (seqviewprocs.lockFarComponents) {
          bsplist = LockFarComponents (sep);
        }
        nsep = FindNucSeqEntry (sep);
        if (hiddengenbio == NULL) {
          wnd = FixedWindow (-50, -33, -10, -10, "you should not see me", NULL);
          hiddengenbio = CreateSimpleBioSourceDialog (wnd, "");
          RealizeWindow (wnd); /* never show this window - just for biosource dialog */
        }
        processonenuc = TRUE;
        if (IS_Bioseq_set (sep)) {
          bssp = (BioseqSetPtr) sep->data.ptrvalue;
          if (bssp != NULL) {
            if (bssp->_class >= BioseqseqSet_class_mut_set &&
                bssp->_class <= BioseqseqSet_class_eco_set) {
              processonenuc = FALSE;
            } else if (bssp->_class == 7) {
              processonenuc = FALSE;
            } else if (bssp->_class == 1 || bssp->_class == 2) {
              processonenuc = FALSE;
            } else if (bssp->_class == BioseqseqSet_class_gen_prod_set) {
              processonenuc = FALSE;
            } else if (bssp->_class == 0) {
              processonenuc = FALSE;
            }
          }
        }
        if (processonenuc) {
          ProcessOneNucleotideTitle (SEQ_PKG_SINGLE, hiddengenbio, NULL, NULL, nsep, sep, NULL);
        }
        if (! leaveAsOldAsn) {
          MySeqEntryToAsn3 (sep, TRUE, FALSE, FALSE);
        }
      }
      if (sep != NULL) {
        if (EntrezASN1Detected (sep)) {
          ErrPostEx (SEV_WARNING, 0, 0, "This record was retrieved from Entrez");
        }
      }
      if (askForSubmit && datatype != OBJ_SEQSUB) {
        ArrowCursor ();
        Update ();
        if (Message (MSG_YN, repackageMsg) == ANS_YES) {
          UnlockFarComponents (bsplist);
          globalEntityID = entityID;
          globalsep = sep;
          StringNCpy_0 (globalPath, path, sizeof (globalPath));
          WatchCursor ();
          Update ();
          w = CreateSubmitBlockForm (-50, -33, "Submitting Authors",
                                     FALSE, TRUE, NULL, JustRegisterSeqEntryBtn,
                                     AddSubmitBlockToSeqEntry);
          ArrowCursor ();
          if (w != NULL) {
            Show (w);
            Select (w);
            SendHelpScrollMessage (helpForm, "Submitting Authors Form", NULL);
            return TRUE;
          } else {
            Message (MSG_FATAL, "Unable to create window.");
            return FALSE;
          }
        }
      }
      seqviewprocs.filepath = path;
      seqviewprocs.forceSeparateViewer = TRUE;
      SeqEntrySetScope (NULL);
      handled = GatherProcLaunch (OMPROC_VIEW, FALSE, entityID, 1,
                                  OBJ_BIOSEQ, 0, 0, OBJ_BIOSEQ, 0);
      seqviewprocs.filepath = NULL;
      UnlockFarComponents (bsplist);
      ArrowCursor ();
      if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
        Message (MSG_FATAL, "Unable to launch viewer.");
        return FALSE;
      } else {
        SendHelpScrollMessage (helpForm, "Editing the Record", NULL);
      }
      ObjMgrSetOptions (OM_OPT_FREE_IF_NO_VIEW, entityID);
      ObjMgrSetDirtyFlag (entityID, TRUE);
      if (bfp != NULL && removeold) {
        Remove (bfp->form);
      }
      return TRUE;
    } else if (datatype == OBJ_SEQANNOT && dataptr != NULL) {
      entityID = 0;
      if (bfp != NULL) {
        entityID = bfp->input_entityID;
      }
      SeqEntrySetScope (NULL);
      entityID = SmartAttachSeqAnnotToSeqEntry (entityID, (SeqAnnotPtr) dataptr);
      ArrowCursor ();
      if (entityID != 0) {
        /* code to inhibit multiple updates when attaching multiple feature tables to the same entity */
        if (updateEntityIDPtr != NULL) {
          if (*updateEntityIDPtr == 0) {
            *updateEntityIDPtr = entityID;
            return TRUE;
          } else if (*updateEntityIDPtr == entityID) {
            return TRUE;
          }
        }
        ObjMgrSetDirtyFlag (entityID, TRUE);
        ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
        return TRUE;
      }
    } else if (datatype == OBJ_PROJECT && dataptr != NULL) {
      SeqEntrySetScope (NULL);
      HandleProjectAsn ((ProjectPtr) dataptr, entityID);
      ArrowCursor ();
      return TRUE;
    } else {
      Message (MSG_ERROR, "Unable to process object type %d.", (int) datatype);
      ObjMgrDelete (datatype, dataptr);
    }
  }
  return FALSE;
}

typedef struct multbioseqform {
  FORM_MESSAGE_BLOCK

  BaseFormPtr  bfp;
  Char         filename [PATH_MAX];
  Boolean      removeold;
  Boolean      askForSubmit;
  ValNodePtr   sephead;
} MultBioseqForm, PNTR MultBioseqFormPtr;

static void CommonHandleMultBioseqs (ButtoN b, Int2 whichbutton)

{
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  Uint1              choice = 0;
  Uint2              datatype = 0;
  Uint2              entityID = 0;
  Boolean            is_na = TRUE;
  SeqEntryPtr        list;
  MultBioseqFormPtr  mfp;
  SeqEntryPtr        next;
  ValNodePtr         pip;
  ProjectPtr         proj;
  SeqEntryPtr        sep;

  mfp = (MultBioseqFormPtr) GetObjectExtra (b);
  if (mfp == NULL || mfp->sephead == NULL) return;
  Hide (mfp->form);
  Update ();
  switch (whichbutton) {
    case 1 :
      proj = ProjectNew ();
      if (proj != NULL) {
        pip = ValNodeNew (NULL);
        if (pip != NULL) {
          bsp = (BioseqPtr) mfp->sephead->data.ptrvalue;
          if (bsp != NULL) {
            is_na = (Boolean) ISA_na (bsp->mol);
          }
          if (is_na) {
            choice = ProjectItem_nucent;
          } else {
            choice = ProjectItem_protent;
          }
          pip->choice = choice;
          proj->data = pip;
          pip->data.ptrvalue = (Pointer) mfp->sephead;
          mfp->sephead = NULL;
          HandleProjectAsn (proj, 0);
        }
      }
      break;
    case 2 :
      list = mfp->sephead;
      sep = NULL;
      mfp->sephead = NULL;
      while (list != NULL) {
        next = list->next;
        list->next = NULL;
        if (sep != NULL) {
          AddSeqEntryToSeqEntry (sep, list, TRUE);
        } else {
          sep = list;
        }
        list = next;
      }
      if (sep != NULL) {
        if (sep->choice == 1) {
          datatype = OBJ_BIOSEQ;
        } else if (sep->choice == 2) {
          datatype = OBJ_BIOSEQSET;
        }
        entityID = ObjMgrRegister (datatype, (Pointer) sep->data.ptrvalue);
        HandleOneNewAsnProc (mfp->bfp, mfp->removeold, mfp->askForSubmit,
                             mfp->filename, (Pointer) sep, OBJ_SEQENTRY,
                             entityID, NULL);
      }
      break;
    case 3 :
      sep = ValNodeNew (NULL);
      if (sep != NULL) {
        bssp = BioseqSetNew ();
        if (bssp != NULL) {
          sep->choice = 2;
          sep->data.ptrvalue = (Pointer) bssp;
          bssp->_class = 14;
          bssp->seq_set = mfp->sephead;
          mfp->sephead = NULL;
          SeqMgrSeqEntry (SM_BIOSEQSET, (Pointer) bssp, sep);
          SeqMgrLinkSeqEntry (sep, 0, NULL);
          entityID = ObjMgrRegister (OBJ_SEQENTRY, (Pointer) sep);
          HandleOneNewAsnProc (mfp->bfp, mfp->removeold, mfp->askForSubmit,
                               mfp->filename, (Pointer) sep, OBJ_SEQENTRY,
                               entityID, NULL);
        }
      }
      break;
    default :
      break;
  }
  ArrowCursor ();
  Remove (mfp->form);
  Update ();
}

static void MultToDocSum (ButtoN b)

{
  CommonHandleMultBioseqs (b, 1);
}

static void MultToSegSeq (ButtoN b)

{
  CommonHandleMultBioseqs (b, 2);
}

static void MultToPopSet (ButtoN b)

{
  CommonHandleMultBioseqs (b, 3);
}

static void MultToSingleSeq (ButtoN b)

{
  CommonHandleMultBioseqs (b, 2);
}

static void MultBioseqFormMessage (ForM f, Int2 mssg)

{
  MultBioseqFormPtr  mfp;

  mfp = (MultBioseqFormPtr) GetObjectExtra (f);
  if (mfp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        Remove (f);
        break;
      default :
        if (mfp->appmessage != NULL) {
          mfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void ProcessMultipleBioseqs (BaseFormPtr bfp, CharPtr filename, Boolean removeold,
                                    Boolean askForSubmit, ValNodePtr sephead)

{
  ButtoN             b;
  GrouP              c;
  MultBioseqFormPtr  mfp;
  StdEditorProcsPtr  sepp;
  WindoW             w;
#ifndef WIN_MAC
  MenU               m;
#endif

  mfp = (MultBioseqFormPtr) MemNew (sizeof (MultBioseqForm));
  if (mfp == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "Sequence Input", NULL);
  SetObjectExtra (w, mfp, StdCleanupFormProc);
  mfp->form = (ForM) w;
  mfp->formmessage = MultBioseqFormMessage;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    mfp->appmessage = sepp->handleMessages;
  }

  mfp->bfp = bfp;
  mfp->removeold = removeold;
  mfp->askForSubmit = askForSubmit;
  mfp->sephead = sephead;
  StringNCpy_0 (mfp->filename, filename, sizeof (mfp->filename));

#ifndef WIN_MAC
  m = PulldownMenu (w, "File");
  FormCommandItem (m, "Close", (BaseFormPtr) mfp, VIB_MSG_CLOSE);
#endif

  c = HiddenGroup (w, 0, 3, NULL);
  SetGroupSpacing (c, 10, 10);
  b = PushButton (c, "Load into Document Window", MultToDocSum);
  SetObjectExtra (b, mfp, NULL);
  if (sephead->next != NULL) {
    b = PushButton (c, "Load as Segmented Sequence", MultToSegSeq);
    SetObjectExtra (b, mfp, NULL);
    b = PushButton (c, "Load as Population Study", MultToPopSet);
    SetObjectExtra (b, mfp, NULL);
  } else {
    b = PushButton (c, "Load as Single Sequence", MultToSingleSeq);
    SetObjectExtra (b, mfp, NULL);
  }

  RealizeWindow (w);
  Show (w);
  Select (w);
  ArrowCursor ();
  Update ();
}

static void ProcessMultipleSimpleSeqs (BaseFormPtr bfp, CharPtr filename, Boolean removeold,
                                       Boolean askForSubmit, ValNodePtr simples)

{
  EntrezGlobalsPtr  egp;

  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL || egp->retrieveSimpleProc == NULL) return;
  egp->retrieveSimpleProc (NULL, simples);
}

static Boolean HandledAnnotatedProteins (BaseFormPtr bfp, ValNodePtr bioseqs)

{
  BioseqPtr    bsp;
  Int2         code;
  ValNodePtr   descr;
  MolInfoPtr   mip;
  BioseqPtr    nucbsp;
  ValNodePtr   sdp;
  SeqEntryPtr  sep;
  SeqLocPtr    slp;
  CharPtr      title;
  SeqEntryPtr  top = NULL;
  ValNode      vn;
  ValNodePtr   vnp;

  if (bfp == NULL || bioseqs == NULL) return FALSE;
  nucbsp = GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (nucbsp == NULL) return FALSE;
  if (! ISA_na (nucbsp->mol)) return FALSE;
  /* top = GetBestTopParentForData (bfp->input_entityID, nucbsp); */
  top = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (top == NULL) return FALSE;
  for (vnp = bioseqs; vnp != NULL; vnp = vnp->next) {
    bsp = (BioseqPtr) vnp->data.ptrvalue;
    if (bsp == NULL) return FALSE;
    if (! ISA_aa (bsp->mol)) return FALSE;
    title = BioseqGetTitle (bsp);
    if (title == NULL) return FALSE;
    if (StringChr (title, '[') == NULL) return FALSE;
  }
  code = SeqEntryToGeneticCode (top, NULL, NULL, 0);
  SetBatchSuggestNucleotide (nucbsp, code);
  descr = ExtractBioSourceAndPubs (top);
  vn.choice = SEQLOC_WHOLE;
  vn.data.ptrvalue = (Pointer) nucbsp->id;
  slp = &vn;
  for (vnp = bioseqs; vnp != NULL; vnp = vnp->next) {
    bsp = (BioseqPtr) vnp->data.ptrvalue;
    bsp->id = SeqIdFree (bsp->id);
    bsp->id = MakeNewProteinSeqId (slp, NULL);
    SeqMgrReplaceInBioseqIndex (bsp);
    sep = SeqMgrGetSeqEntryForData (bsp);
    if (sep != NULL) {
      mip = MolInfoNew ();
      if (mip != NULL) {
        mip->biomol = 8;
        mip->tech = 13;
        sdp = CreateNewDescriptor (sep, Seq_descr_molinfo);
        if (sdp != NULL) {
          sdp->data.ptrvalue = (Pointer) mip;
        }
      }
      AddSeqEntryToSeqEntry (top, sep, TRUE);
      AutomaticProteinProcess (top, sep, code, FALSE, NULL);
      ValNodeExtract (&(bsp->descr), Seq_descr_title);
    }
  }
  ClearBatchSuggestNucleotide ();
  ReplaceBioSourceAndPubs (top, descr);
  return TRUE;
}

static Boolean DoReadAnythingLoop (BaseFormPtr bfp, CharPtr filename, CharPtr path,
                                   Boolean removeold, Boolean askForSubmit, Boolean alwaysMult,
                                   Boolean parseFastaSeqId, Boolean fastaAsSimpleSeq)

{
  ValNodePtr   bioseqs;
  BioseqPtr    bsp;
  Pointer      dataptr;
  Uint2        datatype;
  Boolean      each;
  Uint2        entityID;
  FILE         *fp;
  ValNodePtr   head = NULL;
  ValNodePtr   projects;
  Boolean      rsult;
  SeqEntryPtr  sep;
  SeqEntryPtr  sephead = NULL;
  ValNodePtr   simples;
  Uint2        updateEntityID;
  ValNodePtr   vnp;

  if (filename == NULL) return FALSE;
  fp = FileOpen (filename, "r");
  if (fp != NULL) {
    rsult = FALSE;
    while ((dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, FALSE,
                                              parseFastaSeqId, fastaAsSimpleSeq)) != NULL) {
      ValNodeAddPointer (&head, datatype, dataptr);
    }
    FileClose (fp);
    bioseqs = ValNodeExtractList (&head, OBJ_BIOSEQ);
    projects = ValNodeExtractList (&head, OBJ_PROJECT);
    simples = ValNodeExtractList (&head, OBJ_FASTA);
    updateEntityID = 0;
    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      datatype = vnp->choice;
      dataptr = vnp->data.ptrvalue;
      entityID = ObjMgrRegister (datatype, dataptr);
      each = HandleOneNewAsnProc (bfp, removeold, askForSubmit, path, dataptr, datatype, entityID, &updateEntityID);
      removeold = FALSE;
      rsult = (Boolean) (rsult || each);
    }
    ValNodeFree (head);
    if (updateEntityID != 0) {
      ObjMgrSetDirtyFlag (updateEntityID, TRUE);
      ObjMgrSendMsg (OM_MSG_UPDATE, updateEntityID, 0, 0);
    }
    for (vnp = projects; vnp != NULL; vnp = vnp->next) {
      HandleProjectAsn ((ProjectPtr) vnp->data.ptrvalue, 0);
    }
    ValNodeFree (projects);
    if (bioseqs != NULL) {
      if (HandledAnnotatedProteins (bfp, bioseqs)) {
        rsult = TRUE;
        ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
        ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
      } else if (bioseqs->next == NULL && (! alwaysMult)) {
        bsp = (BioseqPtr) bioseqs->data.ptrvalue;
        bioseqs->data.ptrvalue = NULL;
        if (bsp != NULL) {
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
        entityID = ObjMgrRegister (OBJ_BIOSEQ, (Pointer) bsp);
        each = HandleOneNewAsnProc (bfp, FALSE, FALSE, path, (Pointer) bsp, OBJ_BIOSEQ, entityID, NULL);
        removeold = FALSE;
        rsult = (Boolean) (rsult || each);
      } else {
        sephead = NULL;
        for (vnp = bioseqs; vnp != NULL; vnp = vnp->next) {
          bsp = (BioseqPtr) vnp->data.ptrvalue;
          if (bsp != NULL) {
            sep = SeqMgrGetSeqEntryForData (bsp);
            if (sep == NULL) {
              sep = SeqEntryNew ();
              if (sep != NULL) {
                sep->choice = 1;
                sep->data.ptrvalue = bsp;
                SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, sep);
              }
            }
            if (sep != NULL) {
              ValNodeLink (&sephead, sep);
            }
          }
        }
        ProcessMultipleBioseqs (bfp, filename, removeold, askForSubmit, sephead);
        bioseqs = NULL;
        rsult = TRUE;
      }
    }
    ValNodeFree (bioseqs);
    if (simples != NULL) {
      ProcessMultipleSimpleSeqs (bfp, filename, removeold, askForSubmit, simples);
      rsult = TRUE;
    }
    ValNodeFree (simples);
    return rsult;
  }
  return FALSE;
}

/* Sarah's stuff */

static CharPtr FGetLine (FILE *fp)
{
  ValNodePtr charp=NULL,
             vnp;
  CharPtr    buffer = NULL;
  Char       c;
  Int4       j, len=0;

  c = (Char)fgetc(fp);
  while (c!=255 && c!=NULLB && c!=EOF && c!='\0' && c!='\n' && c != '\r')
  {
     if (c!='\t' && c!='\015' && c != '\013') {
        ValNodeAddInt (&charp, 1, (Int4)c);
        len++;
     }
     c = (Char)fgetc(fp);
  }
  if (charp) {
     buffer = (CharPtr)MemNew((size_t)((len+5)*sizeof(Char)));
     for (j=0, vnp=charp; vnp!=NULL; vnp=vnp->next, j++)
        buffer[j] = (Char) vnp->data.intvalue;
     buffer[j]='\0';
     ValNodeFree (charp);
  }
  else if (c=='\0' || c=='\n' || c == '\r') {
     buffer = (CharPtr)MemNew((size_t)(sizeof(Char)));
     buffer[0]='\0';
  }
  return buffer;
}

#define SQACT_TXTALN 1
#define SQACT_NOTTXTALN 2

static Int4 SQACT_GuessWhatIAm (FILE *fp)
{
   Boolean  found;
   Int4     len;
   CharPtr  line;
   Int4     prevlen;
   Int4     seq;
   CharPtr  tmp;

   if (fp == NULL) return SQACT_NOTTXTALN;
   line = NULL;
   line = FGetLine(fp);
   found = FALSE;
   while (line != NULL && !found) /* find first non-empty line */
   {
      if (!StringHasNoText(line))
      {
         if (StringLen(line) > 0)
            found = TRUE;
      } else
      {
         MemFree(line);
         line = FGetLine(fp);
      }
   }
   if (!found)
      return -1;
   tmp = StringStr(line, "::="); /* ASN.1 */
   if (tmp != NULL)
   {
      MemFree(line);
      return SQACT_NOTTXTALN;
   }
   tmp = StringStr(line, "LOCUS");  /* GenBank flat file */
   if (tmp != NULL)
   {
      MemFree(line);
      return SQACT_NOTTXTALN;
   }
   if (line[0] == '>')  /* FASTA sequences or FASTA alignment */
   {
      if (StringNCmp (line, ">PubMed", 7) == 0 ||
          StringNCmp (line, ">Protein", 8) == 0 ||
          StringNCmp (line, ">Nucleotide", 11) == 0 ||
          StringNCmp (line, ">Structure", 10) == 0 ||
          StringNCmp (line, ">Genome", 7) == 0 ||
          StringNCmp (line, ">Feature", 8) == 0 ||
          StringNCmp (line, ">Vector", 7) == 0 ||
          StringNCmp (line, ">Restriction", 12) == 0 ||
          StringNCmp (line, ">Contig", 7) == 0 ||
          StringNCmp (line, ">Virtual", 8) == 0 ||
          StringNCmp (line, ">Message", 8) == 0) {
        MemFree(line);
      	return SQACT_NOTTXTALN;
      }
      MemFree(line);
      line = FGetLine(fp);
      prevlen = -1;
      len = 0;
      seq = 0;
      while (line != NULL)
      {
         seq++;
         while (line != NULL && line[0] != '>')
         {
            tmp = StringStr(line, "-");
            if (tmp != NULL)  /* found a gap -> alignment */
            {
               MemFree(line);
               return SQACT_TXTALN;
            }
            len += StringLen(line);
            MemFree(line);
            line = FGetLine(fp);
         }
         if (line != NULL)
         {
            MemFree(line);
            line = FGetLine(fp);
         }
         if (prevlen == -1)
            prevlen = len;
         if (len != prevlen)  /* sequences of different length -> FASTA seq */
         {
            MemFree(line);
            return SQACT_NOTTXTALN;
         }
      }
      if (seq > 1)
         return SQACT_TXTALN;
      else
         return SQACT_NOTTXTALN;
   } else
   {
      MemFree(line);
      return SQACT_TXTALN;  /* NEXUS, PHYLIP, etc */
   }
}

static Uint1 sq_transform_code(Uint1 res, Int4 which)
{
   if (which == 4) /* ncbi2na */
   {
      if (res == 1)
         return 0;
      else if (res == 3)
         return 1;
      else if (res == 7)
         return 2;
      else if (res == 18)
         return 3;
      else
         return 0;
   } else if (which == 2) /* ncbi4na */
   {
      if (res == 1)
         return 1;
      else if (res == 3)
         return 2;
      else if (res == 12)
         return 3;
      else if (res == 7)
         return 4;
      else if (res == 16)
         return 5;
      else if (res == 17)
         return 6;
      else if (res == 19)
         return 7;
      else if (res == 18)
         return 8; 
      else if (res == 20)
         return 9;
      else if (res == 22)
         return 10;
      else if (res == 8)
         return 11;
      else if (res == 10)
         return 12;
      else if (res == 4)
         return 13;
      else if (res == 2)
         return 14;
      else if (res == 13)
         return 15;
       else
         return 15;
   }
   return 0;
}

static void SQACT_FixBioseqs (BioseqPtr bsp, Pointer userdata)
{
   Uint1Ptr           array;
   ByteStorePtr       bs;
   Int4               compact;
   SeqMgrDescContext  context;
   Int4               i;
   Int4               j;
   Int4               len;
   MolInfoPtr         mip;
   Uint1              res;
   Uint1              res1;
   Int4               shift;
   SeqPortPtr         spp;
   ValNodePtr         vnp;

   if (bsp == NULL)
      return;
   len = bsp->length-1;
   compact = 2;
   spp = SeqPortNew(bsp, 0, len, 0, Seq_code_ncbistdaa);
   while ((res = SeqPortGetResidue(spp)) != SEQPORT_EOF)
   {
      if (res != 1 && res != 3 && res != 0 && res != 18 && res != 7 && res != 13 && res != 24 && res != 21)
      {
         SeqPortFree(spp);
         return;
      } else if (res != 1 && res != 3 && res != 7 && res != 18)
         compact = 4;
   }
   SeqPortFree(spp);
   bs = BSNew((bsp->length+compact-1)/compact);
   array = (Uint1Ptr)MemNew((bsp->length+compact)*sizeof(Uint1));
   spp = SeqPortNew(bsp, 0, len, 0, Seq_code_ncbistdaa);
   for (i=0; i<bsp->length; i++)
   {
      res = SeqPortGetResidue(spp);
      array[i] = sq_transform_code(res, compact);
   }
   for (i=0; i<compact; i++)
      array[i+bsp->length] = 0;
   if (compact == 4)
      shift = 2;
   else
      shift = 4;
   for (j=0; j<bsp->length; j+=compact)
   {
      res = 0;
      res1 = 0;
      for (i=0; i<compact; i++)
      {
         res = array[j+i];
         res1<<=shift;
         res1 |= res;
      }
      BSPutByte(bs, res1);
   }
   MemFree(bsp->seq_data);
   bsp->seq_data = bs;
   if (compact == 4)
      bsp->seq_data_type = Seq_code_ncbi2na;
   else
      bsp->seq_data_type = Seq_code_ncbi4na;
   bsp->mol = Seq_mol_na;
   bsp->repr = Seq_repr_raw;
   SeqMgrIndexFeatures(0, (Pointer)(bsp));
   vnp = NULL;
   vnp = SeqMgrGetNextDescriptor(bsp, vnp, Seq_descr_molinfo, &context);
   if (vnp != NULL)
   {
      vnp = context.sdp;
      mip = (MolInfoPtr)(vnp->data.ptrvalue);
      mip->biomol = 1;
   }
   MemFree(array);
}

static Boolean CommonReadNewAsnProc (Handle obj, Boolean removeold, Boolean askForSubmit)

{
  BaseFormPtr  bfp;
  Pointer      dataptr;
  Uint2        datatype;
  Uint2        entityID;
  FILE         *fp;
  Char         path [PATH_MAX];
  Boolean      rsult;
  SeqEntryPtr  sep;
  Int4         type;
  Uint2        updateEntityID;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (obj);
#endif
  rsult = FALSE;
  path [0] = '\0';
  if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
    Update ();
    fp = FileOpen (path, "r");
    if (fp == NULL) {
      WatchCursor ();
      return FALSE;
    }
    type = SQACT_GuessWhatIAm (fp);
    FileClose (fp);
    if (type == SQACT_TXTALN) {
      sep = ReadAnyAlignment (TRUE, path);
      if (sep == NULL) {
        ErrPostEx (SEV_WARNING, 0, 0, "Error reading file");
        return rsult;
      }
      VisitBioseqsInSep (sep, NULL, SQACT_FixBioseqs);
      if (IS_Bioseq (sep)) {
        datatype = OBJ_BIOSEQ;
      } else {
        datatype = OBJ_BIOSEQSET;
      }
      dataptr = (Pointer) sep->data.ptrvalue;
      entityID = ObjMgrRegister (datatype, dataptr);
      rsult = HandleOneNewAsnProc (bfp, FALSE, askForSubmit, path, dataptr, datatype, entityID, &updateEntityID);
      if (updateEntityID != 0) {
        ObjMgrSetDirtyFlag (updateEntityID, TRUE);
        ObjMgrSendMsg (OM_MSG_UPDATE, updateEntityID, 0, 0);
      }
    } else {
      rsult = DoReadAnythingLoop (bfp, path, path, removeold, askForSubmit, FALSE, TRUE, FALSE);
    }
  }
  ArrowCursor ();
  return rsult;
}

static void ReadNewAsnProc (IteM i)

{
  CommonReadNewAsnProc ((Handle) i, FALSE, FALSE);
}

static void ReadNewAsnAndDeleteProc (IteM i)

{
  CommonReadNewAsnProc ((Handle) i, TRUE, FALSE);
}

extern Boolean LIBCALLBACK SequinOpenMimeFile (CharPtr filename)

{
  Boolean  rsult = FALSE;

#ifdef WIN_MAC
  SafeHide (startupForm); /* if just Hide, triggers MacDeactProc, does not reactivate */
  Update ();
#endif
  rsult = DoReadAnythingLoop (NULL, filename, filename, FALSE, FALSE, FALSE, TRUE, FALSE);
  if (! rsult) {
/*
#ifndef WIN16
    if (BiostrucAvail () && OpenMimeFileWithDeletion (filename, FALSE)) {
      Handle   www_cn3d;
      www_cn3d = Cn3DWin_Entrez(NULL, useEntrez);
      Show (www_cn3d);
      Select (www_cn3d);
    } else {
      Show (startupForm);
      Select (startupForm);
    }
#else
    Show (startupForm);
    Select (startupForm);
#endif
*/
    Show (startupForm);
    Select (startupForm);
  }
#ifdef OS_MAC
  /* the Web browsers on other platforms get upset if you delete the file, */
  /* but apparently on the Mac you must delete it - comment from cn3dmain.c */
  /* but here it's for response to apple events, so should not free this file */
  /* FileRemove (filename); */
#endif
  return rsult;
}

extern Boolean LIBCALLBACK SequinOpenResultFile (CharPtr filename)

{
  return DoReadAnythingLoop (NULL, filename, NULL, FALSE, FALSE, FALSE, TRUE, FALSE);
}

extern Boolean LIBCALLBACK SequinHandleNetResults (CharPtr filename)

{
  return DoReadAnythingLoop (NULL, filename, NULL, FALSE, FALSE, TRUE, TRUE, TRUE);
}

#ifdef WIN_MAC
static void MacReadNewAsnProc (IteM i)

{
  if (initialFormsActive) {
    CommonReadNewAsnProc ((Handle) i, TRUE, FALSE);
  } else {
    CommonReadNewAsnProc ((Handle) i, FALSE, FALSE);
  }
}
#endif

static Boolean CountAlignmentsCallback (GatherContextPtr gcp)

{
  Int4Ptr  rsultptr;

  if (gcp == NULL) return TRUE;

  rsultptr = (Int4Ptr) gcp->userdata;
  if (rsultptr == NULL ) return TRUE;

  switch (gcp->thistype) {
    case OBJ_SEQALIGN :
    case OBJ_SEQHIST_ALIGN :
      (*rsultptr)++;
      return TRUE;
    default :
      break;
  }
  return TRUE;
}

static Int4 LIBCALL CountSeqEntryAligns (Uint2 entityID, SeqEntryPtr sep)

{
  GatherScope  gs;
  Int4         rsult;

  rsult = 0;
  if (entityID == 0 || sep == NULL) return 0;
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
  GatherEntity (entityID, (Pointer) (&rsult), CountAlignmentsCallback, &gs);
  return rsult;
}

static void EnableEditAlignItem (BaseFormPtr bfp)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  IteM          editalign;
  IteM          editdup;
  IteM          editfeatprop;
  IteM          editupwthaln;
  Int2          mssgalign;
  Int2          mssgdup;
  Int2          mssgfeatprop;
  Int2          mssgupwthaln;
  Int4          num;
  SeqEntryPtr   sep;
  SelStructPtr  sel;

  if (bfp == NULL && bfp->input_entityID != 0) return;
  mssgalign = RegisterFormMenuItemName ("SequinEditAlignmentItem");
  mssgdup = RegisterFormMenuItemName ("SequinDuplicateItem");
  mssgfeatprop = RegisterFormMenuItemName ("SequinFeaturePropagate");
  mssgupwthaln = RegisterFormMenuItemName ("SequinUpdateWithAlignment");
  editalign = FindFormMenuItem (bfp, mssgalign);
  editdup = FindFormMenuItem (bfp, mssgdup);
  editfeatprop = FindFormMenuItem (bfp, mssgfeatprop);
  editupwthaln = FindFormMenuItem (bfp, mssgupwthaln);
  sel = ObjMgrGetSelected ();
  sep = NULL;
  /*
  bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (bsp != NULL) {
    sep = GetBestTopParentForData (bfp->input_entityID, bsp);
  } else {
    sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  }
  */
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  num = CountSeqEntryAligns (bfp->input_entityID, sep);
  if (sep != NULL && num > 0) {
    if (sel != NULL && sel->entityID == bfp->input_entityID &&
        (sel->itemtype == OBJ_SEQALIGN || sel->itemtype == OBJ_SEQHIST_ALIGN)) {
      Enable (editalign);
      Enable (editupwthaln);
    } else /* if (sel == NULL) */ {
      if (FindSeqAlignInSeqEntry (sep, OBJ_SEQALIGN) != NULL) {
        Enable (editalign);
      } else {
        Disable (editalign);
      }
      Disable (editupwthaln);
    } /* else {
      Disable (editalign);
      Disable (editupwthaln);
    } */
  } else {
    Disable (editalign);
    Disable (editupwthaln);
  }
  if (sel != NULL &&
      (sel->itemtype == OBJ_SEQDESC || sel->itemtype == OBJ_SEQFEAT)) {
    Enable (editdup);
  } else {
    Disable (editdup);
  }
  bsp = GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (num > 0 && bsp != NULL && sep != NULL && IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL) {
      if (bssp->_class >= 13 && bssp->_class <= 16) {
        Enable (editfeatprop);
      } else if (bssp->_class == 7 && indexerVersion) {
        Enable (editfeatprop);
      } else {
        Disable (editfeatprop);
      }
    } else {
      Disable (editfeatprop);
    }
  } else {
    Disable (editfeatprop);
  }
}

static void EnableEditSeqAlignAndSubItems (BaseFormPtr bfp)

{
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  IteM           editseq;
  IteM           editsub;
  MenU           editupd;
  MenU           editadd;
  Int2           mssgadd;
  Int2           mssgseq;
  Int2           mssgsub;
  Int2           mssgupd;
  ObjMgrDataPtr  omdp;
  SeqEntryPtr    sep;

  if (bfp == NULL && bfp->input_entityID != 0) return;
  mssgseq = RegisterFormMenuItemName ("SequinEditSequenceItem");
  mssgsub = RegisterFormMenuItemName ("SequinEditSubmitterItem");
  mssgupd = RegisterFormMenuItemName ("SequinUpdateSeqSubmenu");
  mssgadd = RegisterFormMenuItemName ("SequinAddSeqSubmenu");
  editseq = FindFormMenuItem (bfp, mssgseq);
  editsub = FindFormMenuItem (bfp, mssgsub);
  editupd = (MenU) FindFormMenuItem (bfp, mssgupd);
  editadd = (MenU) FindFormMenuItem (bfp, mssgadd);
  bsp = GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (bsp != NULL) {
    Enable (editseq);
    Enable (editupd);
    sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
    if (sep != NULL && IS_Bioseq_set (sep)) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      if (bssp != NULL && (bssp->_class == 7 || (bssp->_class >= 13 && bssp->_class <= 16))) {
        Enable (editadd);
      } else {
        Disable (editadd);
      }
    } else {
      Disable (editadd);
    }
#ifdef WIN_MAC
    Enable (featPropItem);
#endif
  } else {
    Disable (editseq);
    Disable (editupd);
    Disable (editadd);
#ifdef WIN_MAC
    Disable (featPropItem);
#endif
  }
  omdp = ObjMgrGetData (bfp->input_entityID);
  if (omdp != NULL && omdp->datatype == OBJ_SEQSUB) {
    Enable (editsub);
#ifdef WIN_MAC
    Enable (prepareItem);
    Enable (submitItem);
#endif
  } else {
    Disable (editsub);
#ifdef WIN_MAC
    Disable (prepareItem);
    Disable (submitItem);
#endif
  }
  EnableEditAlignItem (bfp);
}

#ifdef WIN_MAC
static void MedlineViewFormActivated (WindoW w)

{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  initialFormsActive = FALSE;
  RepeatProcOnHandles (Enable,
                   (HANDLE) closeItem,
                   (HANDLE) duplicateViewItem,
                   (HANDLE) exportItem,
                   (HANDLE) printItem,
                   (HANDLE) copyItem,
                   (HANDLE) displayfontItem,
                   NULL);
}

static void BioseqViewFormActivated (WindoW w)

{
  bioseqViewUp = TRUE;
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  initialFormsActive = FALSE;
  /* SafeSetTitle (importItem, "Import Nucleotide FASTA..."); */
  RepeatProcOnHandles (Enable,
                   (HANDLE) openItem,
                   (HANDLE) closeItem,
                   (HANDLE) saveItem,
                   (HANDLE) saveAsItem,
                   (HANDLE) copyItem,
                   (HANDLE) deleteItem,
                   (HANDLE) displayfontItem,
                   (HANDLE) findItem,
                   (HANDLE) findFFItem,
                   (HANDLE) findGeneItem,
                   (HANDLE) findProtItem,
                   (HANDLE) findPosItem,
                   (HANDLE) exportItem,
                   (HANDLE) duplicateViewItem,
                   (HANDLE) restoreItem,
                   (HANDLE) printItem,
                   (HANDLE) orfItem,
                   (HANDLE) targetItem,
                   (HANDLE) newDescMenu,
                   (HANDLE) newFeatMenu,
                   (HANDLE) newPubMenu,
                   (HANDLE) prepareItem,
                   (HANDLE) validateItem,
                   (HANDLE) edithistoryitem,
                   NULL);
  Enable (aluItem);
  Enable (submitItem);
  Enable (specialMenu);
  Enable (analysisMenu);
  Enable (vectorScreenItem);
  Enable (powerBlastItem);
  Enable (cddBlastItem);
  Enable (spellItem);
  Enable (addSecondaryItem);
  Disable (importItem);
  EnableFeaturesPerTarget ((BaseFormPtr) currentFormDataPtr);
  EnableAnalysisItems ((BaseFormPtr) currentFormDataPtr, FALSE);
  EnableEditSeqAlignAndSubItems ((BaseFormPtr) currentFormDataPtr);
}

static void TermSelectionActivateProc (WindoW w)

{
  termListUp = TRUE;
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  initialFormsActive = FALSE;
  if (UsingTextQuery ((ForM) w)) {
    SafeSetValue (queryChoice, 2);
  } else {
    SafeSetValue (queryChoice, 1);
  }
  RepeatProcOnHandles (Enable,
                   (HANDLE) importItem,
                   (HANDLE) exportItem,
                   (HANDLE) cutItem,
                   (HANDLE) copyItem,
                   (HANDLE) pasteItem,
                   (HANDLE) deleteItem,
                   (HANDLE) preferencesItem,
                   (HANDLE) queryChoice,
                   (HANDLE) clearUnusedItem,
                   NULL);
  Enable (loadUidItem);
  Enable (saveUidItem);
}

static void DocumentSummaryActivateProc (WindoW w)

{
  docSumUp = TRUE;
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  initialFormsActive = FALSE;
  if (UsingDelayedNeighbor ((ForM) w)) {
    SafeSetValue (neighborChoice, 2);
  } else {
    SafeSetValue (neighborChoice, 1);
  }
  RepeatProcOnHandles (Enable,
                   (HANDLE) saveAsItem,
                   (HANDLE) closeItem,
                   (HANDLE) importItem,
                   (HANDLE) exportItem,
                   (HANDLE) printItem,
                   (HANDLE) copyItem,
                   (HANDLE) preferencesItem,
                   (HANDLE) neighborChoice,
                   (HANDLE) docsumfontItem,
                   (HANDLE) analysisMenu,
                   NULL);
  Enable (loadUidItem);
  Enable (saveUidItem);
  EnableAnalysisItems ((BaseFormPtr) currentFormDataPtr, TRUE);
}

static void SeqEditFormActivated (WindoW w)

{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  initialFormsActive = FALSE;
  RepeatProcOnHandles (Enable,
                   (HANDLE) openItem,
                   (HANDLE) closeItem,
                   (HANDLE) cutItem,
                   (HANDLE) copyItem,
                   (HANDLE) pasteItem,
                   (HANDLE) deleteItem,
                   (HANDLE) importItem,
                   (HANDLE) exportItem,
                   NULL);
}

static void StdEditorFormActivated (WindoW w)

{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  initialFormsActive = FALSE;
  RepeatProcOnHandles (Enable,
                   (HANDLE) openItem,
                   (HANDLE) closeItem,
                   (HANDLE) cutItem,
                   (HANDLE) copyItem,
                   (HANDLE) pasteItem,
                   (HANDLE) deleteItem,
                   (HANDLE) importItem,
                   (HANDLE) exportItem,
                   NULL);
}

static void StdValidatorFormActivated (WindoW w)

{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  initialFormsActive = FALSE;
  RepeatProcOnHandles (Enable,
                   (HANDLE) openItem,
                   (HANDLE) closeItem,
                   (HANDLE) cutItem,
                   (HANDLE) copyItem,
                   (HANDLE) pasteItem,
                   (HANDLE) deleteItem,
                   (HANDLE) importItem,
                   (HANDLE) exportItem,
                   (HANDLE) printItem,
                   NULL);
}

extern void UpdateSequenceFormActivated (WindoW w);
extern void UpdateSequenceFormActivated (WindoW w)

{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  initialFormsActive = FALSE;
  RepeatProcOnHandles (Enable,
                   (HANDLE) openItem,
                   (HANDLE) closeItem,
                   NULL);
}

static void TextViewProcFormActivated (WindoW w)

{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  initialFormsActive = FALSE;
  RepeatProcOnHandles (Enable,
                   (HANDLE) openItem,
                   (HANDLE) closeItem,
                   (HANDLE) cutItem,
                   (HANDLE) copyItem,
                   (HANDLE) pasteItem,
                   (HANDLE) deleteItem,
                   (HANDLE) importItem,
                   (HANDLE) exportItem,
                   (HANDLE) printItem,
                   NULL);
}

static void ConfigFormActivated (WindoW w)

{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  initialFormsActive = FALSE;
  RepeatProcOnHandles (Enable,
                   (HANDLE) cutItem,
                   (HANDLE) copyItem,
                   (HANDLE) pasteItem,
                   (HANDLE) deleteItem,
                   NULL);
}

static void StartupActivateProc (WindoW w)

{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  initialFormsActive = FALSE;
  Enable (openItem);
}

static void FormatActivateProc (WindoW w)

{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  initialFormsActive = TRUE;
  Enable (openItem);
}

static void SubmitBlockActivateProc (WindoW w)

{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  initialFormsActive = TRUE;
  RepeatProcOnHandles (Enable,
                   (HANDLE) openItem,
                   (HANDLE) cutItem,
                   (HANDLE) copyItem,
                   (HANDLE) pasteItem,
                   (HANDLE) deleteItem,
                   (HANDLE) importItem,
                   (HANDLE) exportItem,
                   NULL);
}

static void GenomeFormActivateProc (WindoW w)

{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  initialFormsActive = TRUE;
  RepeatProcOnHandles (Enable,
                   (HANDLE) openItem,
                   (HANDLE) cutItem,
                   (HANDLE) copyItem,
                   (HANDLE) pasteItem,
                   (HANDLE) deleteItem,
                   (HANDLE) importItem,
                   (HANDLE) exportItem,
                   NULL);
}

static void OrgAndSeqsActivateProc (WindoW w)

{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  initialFormsActive = TRUE;
  RepeatProcOnHandles (Enable,
                   (HANDLE) openItem,
                   (HANDLE) cutItem,
                   (HANDLE) copyItem,
                   (HANDLE) pasteItem,
                   (HANDLE) deleteItem,
                   (HANDLE) importItem,
                   (HANDLE) exportItem,
                   NULL);
}

static void HelpActivateProc (WindoW w)

{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  RepeatProcOnHandles (Enable,
                   (HANDLE) openItem,
                   (HANDLE) closeItem,
                   (HANDLE) cutItem,
                   (HANDLE) copyItem,
                   (HANDLE) pasteItem,
                   (HANDLE) deleteItem,
                   (HANDLE) exportItem,
                   (HANDLE) printItem,
                   NULL);
}

static void MacDeactProc (WindoW w)

{
  termListUp = FALSE;
  docSumUp = FALSE;
  bioseqViewUp = FALSE;
  currentFormDataPtr = NULL;
  initialFormsActive = FALSE;
  Enable (openItem);
  SafeSetTitle (importItem, "Import...");
  SafeSetTitle (exportItem, "Export...");
  SafeSetValue (queryChoice, 0);
  SafeSetValue (neighborChoice, 0);
  RepeatProcOnHandles (Disable,
                   (HANDLE) saveItem,
                   (HANDLE) saveAsItem,
                   (HANDLE) closeItem,
                   (HANDLE) cutItem,
                   (HANDLE) copyItem,
                   (HANDLE) pasteItem,
                   (HANDLE) deleteItem,
                   (HANDLE) duplicateViewItem,
                   (HANDLE) importItem,
                   (HANDLE) exportItem,
                   (HANDLE) displayfontItem,
                   (HANDLE) findItem,
                   (HANDLE) findFFItem,
                   (HANDLE) findGeneItem,
                   (HANDLE) findProtItem,
                   (HANDLE) findPosItem,
                   (HANDLE) orfItem,
                   (HANDLE) targetItem,
                   (HANDLE) newDescMenu,
                   (HANDLE) newFeatMenu,
                   (HANDLE) newPubMenu,
                   (HANDLE) prepareItem,
                   (HANDLE) validateItem,
                   (HANDLE) editsequenceitem,
                   (HANDLE) editseqalignitem,
                   (HANDLE) editseqsubitem,
                   (HANDLE) edithistoryitem,
                   (HANDLE) updateSeqMenu,
                   (HANDLE) addSeqMenu,
                   (HANDLE) featPropItem,
                   (HANDLE) updalignitem,
                   NULL);
  Disable (aluItem);
  Disable (submitItem);
  Disable (vectorScreenItem);
  Disable (powerBlastItem);
  Disable (cddBlastItem);
  Disable (spellItem);
  Disable (docsumfontItem);
  Disable (queryChoice);
  Disable (clearUnusedItem);
  Disable (neighborChoice);
  Disable (legendItem);
  Disable (duplicateItem);
  Disable (addSecondaryItem);
  Disable (printItem);
  Disable (restoreItem);
  Disable (specialMenu);
  Disable (analysisMenu);
  Disable (loadUidItem);
  Disable (saveUidItem);
}
#endif

#ifndef WIN_MAC
#define MedlineViewFormActivated NULL
#define BioseqViewFormActivated NULL
#define TermSelectionActivateProc NULL
#define DocumentSummaryActivateProc NULL
#define SeqEditFormActivated NULL
#define StdEditorFormActivated NULL
#define StdValidatorFormActivated NULL
#define TextViewProcFormActivated NULL
#define ConfigFormActivated NULL
#define StartupActivateProc NULL
#define FormatActivateProc NULL
#define SubmitBlockActivateProc NULL
#define OrgAndSeqsActivateProc NULL
#define GenomeFormActivateProc NULL
#define HelpActivateProc NULL
#endif

static void HideHelpForm (ButtoN b)

{
  Hide (ParentWindow (b));
}

static void DisplayHelpFormProc (IteM i)

{
  if (helpForm == NULL) {
    WatchCursor ();
    helpForm = CreateHelpForm (-95, -5, "Sequin Help", "sequin.hlp",
                               HideHelpForm, HelpActivateProc);
    ArrowCursor ();
  }
  if (helpForm != NULL) {
    Show (helpForm);
    Select (helpForm);
  }
}

/*#ifdef USE_MEDARCH*/
static ValNodePtr LookupAnArticleFunc (ValNodePtr oldpep)

{
  FindPubOption  fpo;
  MonitorPtr     mon;
  Int4           muid = 0;
  ValNodePtr     pep;
  Int4           pmid = 0;
  ValNodePtr     pub;
  ValNodePtr     vnp;

  pub = NULL;
  if (oldpep != NULL) {
    WatchCursor ();
    mon = MonitorStrNewEx ("Lookup Article", 40, FALSE);
    MonitorStrValue (mon, "Connecting to MedArch");
    Update ();
    if (MedArchInit ()) {
      pep = AsnIoMemCopy (oldpep, (AsnReadFunc) PubEquivAsnRead,
                          (AsnWriteFunc) PubEquivAsnWrite);
      fpo.always_look = TRUE;
      fpo.replace_cit = TRUE;
      fpo.lookups_attempted = 0;
      fpo.lookups_succeeded = 0;
      fpo.fetches_attempted = 0;
      fpo.fetches_succeeded = 0;
      MonitorStrValue (mon, "Performing Lookup");
      Update ();
      pub = FixPubEquiv (pep, &fpo);
      if (! fpo.fetches_succeeded) {
        ErrShow ();
        Update ();
      } else {
        for (vnp = pub; vnp != NULL; vnp = vnp->next) {
          if (vnp->choice == PUB_Muid) {
            muid = vnp->data.intvalue;
          } else if (vnp->choice == PUB_PMid) {
            pmid = vnp->data.intvalue;
          }
        }
        if (muid != 0 && pmid == 0) {
          pmid = MedArchMu2Pm (muid);
          if (pmid != 0) {
            ValNodeAddInt (&pub, PUB_PMid, pmid);
          }
        } else if (pmid != 0 && muid == 0) {
          muid = MedArchPm2Mu (pmid);
          if (muid != 0) {
            ValNodeAddInt (&pub, PUB_Muid, muid);
          }
        }
      }
      MonitorStrValue (mon, "Closing MedArch");
      Update ();
      MedArchFini ();
      MonitorFree (mon);
      ArrowCursor ();
      Update ();
    } else {
      ArrowCursor ();
      Message (MSG_ERROR, "Unable to connect to MedArch");
      MonitorFree (mon);
      Update ();
    }
  }
  return pub;
}

static Boolean LookupJournalFunc (CharPtr title, size_t maxsize, Int1Ptr jtaType)

{
  MonitorPtr  mon;
  Int4        num;
  Char        str [256];
  CharPtr     titles [1];
  Int1        types [1];

  WatchCursor ();
  mon = MonitorStrNewEx ("Lookup Journal", 40, FALSE);
  MonitorStrValue (mon, "Connecting to MedArch");
  Update ();
  if (MedArchInit ()) {
    StringNCpy_0 (str, title, sizeof (str));
    if (str [0] != '\0') {
      MonitorStrValue (mon, "Performing Lookup");
      Update ();
      num = MedArchGetTitles (titles, types, str, (Int1) Cit_title_jta,
                              Cit_title_iso_jta, 1);
      if (num > 0 && types [0] == Cit_title_iso_jta) {
        StringNCpy_0 (title, titles [0], maxsize);
        if (jtaType != NULL) {
          *jtaType = types [0];
        }
        MemFree (titles [0]);
      } else {
        Message (MSG_OK, "Unable to match journal");
      }
    }
    MonitorStrValue (mon, "Closing MedArch");
    Update ();
    MedArchFini ();
    MonitorFree (mon);
    ArrowCursor ();
    Update ();
    return TRUE;
  } else {
    ArrowCursor ();
    Message (MSG_ERROR, "Unable to connect to MedArch");
    MonitorFree (mon);
    Update ();
  }
  return FALSE;
}
/*#endif*/

/*#ifdef USE_TAXON*/
static Boolean LookupTaxonomyFunc (Uint2 entityID)

{
  SeqEntryPtr  sep;

  if (entityID < 1) return FALSE;
  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep == NULL) return FALSE;
  if (! leaveAsOldAsn) {
    MySeqEntryToAsn3 (sep, TRUE, FALSE, TRUE);
  }
  return TRUE;
}
/*#endif*/

extern void QuitProc (void);
extern void QuitProc (void)

{
  Boolean        dirty;
  Uint2          j;
  Uint2          num;
  ObjMgrPtr      omp;
  ObjMgrDataPtr  PNTR omdpp;
  ObjMgrDataPtr  tmp;
#ifdef USE_SMARTNET
  SMUserDataPtr  sm_usr_data = NULL;
  OMUserDataPtr  omudp;
#endif

#ifdef WIN_MAC
  if (initialFormsActive) {
    if (Message (MSG_YN,
        "This will end your session without saving any data.\nAre you sure you want to exit?") == ANS_YES) {
      QuitProgram ();
    }
    return;
  }
#endif

  if (smartnetMode) {
#ifdef USE_SMARTNET
      
      omp = ObjMgrGet ();
      num = omp->HighestEntityID;
      for(j = 1; j <= omp->HighestEntityID; j++) {
          if((omudp = ObjMgrGetUserData(j, 0,0, SMART_KEY)) != NULL) {
            if((sm_usr_data = 
                (SMUserDataPtr) omudp->userdata.ptrvalue) != NULL &&
               sm_usr_data->fd != 0) {
              sm_usr_data->header->status = SMStatClosed;
              SMSendMsgToClient(sm_usr_data);
            }              
          }
      }
#endif
      QuitProgram ();
      return;
  }

  dirty = FALSE;
  omp = ObjMgrGet ();
  num = omp->currobj;
  for (j = 0, omdpp = omp->datalist; j < num && omdpp != NULL; j++, omdpp++) {
    tmp = *omdpp;
    if (tmp->parentptr == NULL) {
      if (tmp->dirty) {
        dirty = TRUE;
      }
    }
  }
  if (dirty) {
    if (Message (MSG_YN,
        "Some data have not been saved.\nAre you sure you want to exit?") == ANS_NO) {
      return;
    }
  } else if (subtoolMode || smartnetMode) {
    if (Message (MSG_YN,
        "Are you sure you want to exit?") == ANS_NO) {
      return;
    }
  }
#ifndef WIN_MAC
  if (subtoolMode || smartnetMode) {
    subtoolRecordDirty = FALSE;
    FileRemove (SEQUIN_EDIT_TEMP_FILE);
    /* FileRemove (SEQUIN_EDIT_PREV_FILE); */
    FileRemove (SEQUIN_EDIT_BACK_FILE);
    FileRemove (SEQUIN_EDIT_ARCH_FILE);
    FileRename (SEQUIN_EDIT_PREV_FILE, SEQUIN_EDIT_ARCH_FILE);
  }
#endif
  QuitProgram ();
}

static void CloseProc (BaseFormPtr bfp)

{
  Uint2          entityID;
  Boolean        freeEditors = FALSE;
  Int2           j;
  Int2           num;
  Boolean        numview;
  ObjMgrPtr      omp;
  ObjMgrDataPtr  PNTR omdpp;
  OMUserDataPtr  omudp;
  ObjMgrDataPtr  tmp;
#ifdef USE_SMARTNET
  ObjMgrDataPtr  PNTR omdp;
  int fd;
  SMUserDataPtr sm_usr_data = NULL;
#endif

#ifdef WIN_MAC
  if (initialFormsActive) return;
#endif
  if (bfp != NULL) {
    omp = ObjMgrGet ();
    num = omp->currobj;
    for (j = 0, omdpp = omp->datalist; j < num && omdpp != NULL; j++, omdpp++) {
      tmp = *omdpp;
      if (tmp->parentptr == NULL && tmp->dirty &&
          tmp->EntityID == bfp->input_entityID) {
        numview = 0;

        for (omudp = tmp->userdata; omudp != NULL; omudp = omudp->next) {
          if (omudp->proctype == OMPROC_VIEW) {
            numview++;
          }
        }
        if (numview < 2) {
          if (Message (MSG_OKC,
              "Closing the window will lose unsaved data.") != ANS_OK) {
            return;
          }
          freeEditors = TRUE;
        }
      }
    }
    numview = 0;
    for (j = 0, omdpp = omp->datalist; j < num && omdpp != NULL; j++, omdpp++) {
      tmp = *omdpp;

      if (tmp->parentptr == NULL) {
          for (omudp = tmp->userdata; omudp != NULL; omudp = omudp->next) {

              if (omudp->proctype == OMPROC_VIEW) {
                  numview++;
              }
          }
      }
    }

    if(!smartnetMode) {
        entityID = bfp->input_entityID;
        /* RemoveSeqEntryViewer (bfp->form); */ /* can go back to Remove */
        Remove (bfp->form);
        if (freeEditors) {
          ObjMgrSendMsg (OM_MSG_DEL, entityID, 0, 0);
        }
    }

    if (numview <= 1) {
        if (subtoolMode || stdinMode || binseqentryMode) {
            Message (MSG_OK, "No more viewers, quitting program.");
            QuitProgram ();
            return;
        } else if (smartnetMode) {
#ifdef USE_SMARTNET
          if((omudp = 
              ObjMgrGetUserData(bfp->input_entityID, 0, 0, SMART_KEY)) != NULL) {
              if((sm_usr_data = 
                  (SMUserDataPtr) omudp->userdata.ptrvalue) != NULL && 
                 sm_usr_data->fd != NULL) {
                  sm_usr_data->header->status = SMStatClosed;
                  SMSendMsgToClient(sm_usr_data); 
              }

              entityID = bfp->input_entityID;
              RemoveSeqEntryViewer (bfp->form);
              ObjMgrFreeUserData(entityID, 0, 0, SMART_KEY); 
              if (freeEditors) {
                ObjMgrSendMsg (OM_MSG_DEL, entityID, 0, 0);
              }
              
          }
          return;
#endif
        }

        WatchCursor ();
        Update ();
        if (! workbenchMode) {
            if (termListForm != NULL || docSumForm != NULL) {
            } else {
                Show (startupForm);
                Select (startupForm);
                SendHelpScrollMessage (helpForm, "Initial Entry", NULL);
            }
            ArrowCursor ();
            Update ();
        }
    } else { /* numview > 1 */
        if(smartnetMode) {
            entityID = bfp->input_entityID;
            RemoveSeqEntryViewer (bfp->form);
#ifdef USE_SMARTNET 
            /* sssddd   ObjMgrFreeUserData(entityID, 0, 0, SMART_KEY); */
#endif 
            if (freeEditors) {
              ObjMgrSendMsg (OM_MSG_DEL, entityID, 0, 0);
            }
        }

    }
  }
}

static void EditSubmitBlock (BaseFormPtr bfp)

{
  Boolean        handled;
  ObjMgrDataPtr  omdp;

  if (bfp != NULL && bfp->input_entityID != 0) {
    omdp = ObjMgrGetData (bfp->input_entityID);
    if (omdp != NULL) {
      if (omdp->datatype == OBJ_SEQSUB) {
        WatchCursor ();
        handled = GatherProcLaunch (OMPROC_EDIT, FALSE, bfp->input_entityID, 1,
                                    OBJ_SUBMIT_BLOCK, 0, 0, OBJ_SUBMIT_BLOCK, 0);
        ArrowCursor ();
        if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
          Message (MSG_FATAL, "Unable to launch editor.");
        }
      } else {
        Message (MSG_OK, "Record has no submit block.  This may be added when record is first read.");
      }
    }
  }
}

/*
static void DisplayFontChangeProc (IteM i)

{
  BaseFormPtr  bfp;
  FonT         fnt;
  FontSpec     fs;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp != NULL) {
    MemSet ((Pointer) (&fs), 0, sizeof (FontSpec));
    fnt = programFont;
    if (seqviewprocs.displayFont != NULL) {
      fnt = seqviewprocs.displayFont;
    }
    GetFontSpec (fnt, &fs);
    if (ChooseFont (&fs, CFF_READ_FSP | CFF_MONOSPACE, NULL)) {
      seqviewprocs.displayFont = CreateFont (&fs);
      SendMessageToForm (bfp->form, VIB_MSG_REDRAW);
    }
  }
}
*/

static void DuplicateViewProc (IteM i)

{
  BaseFormPtr  bfp;
  Int2         handled;
  Uint2        itemID;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  if (bfp->input_itemtype == OBJ_BIOSEQ) {
    WatchCursor ();
    itemID = bfp->input_itemID;
    if (itemID == 0) {
      itemID = 1;
    }
    seqviewprocs.forceSeparateViewer = TRUE;
    SeqEntrySetScope (NULL);
    handled = GatherProcLaunch (OMPROC_VIEW, FALSE, bfp->input_entityID, itemID,
                                OBJ_BIOSEQ, 0, 0, OBJ_BIOSEQ, 0);
    ArrowCursor ();
    if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
      Message (MSG_ERROR, "Unable to launch additional viewer.");
    }
  } else if (bfp->input_itemtype == OBJ_MEDLINE_ENTRY) {
    WatchCursor ();
    itemID = bfp->input_itemID;
    if (itemID == 0) {
      itemID = 1;
    }
    handled = GatherProcLaunch (OMPROC_VIEW, FALSE, bfp->input_entityID, itemID,
                                OBJ_MEDLINE_ENTRY, 0, 0, OBJ_MEDLINE_ENTRY, 0);
    ArrowCursor ();
    if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
      Message (MSG_ERROR, "Unable to launch additional viewer.");
    }
  }
}

static SeqEntryPtr RestoreFromFile (CharPtr path)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Pointer       dataptr;
  Uint2         datatype;
  Uint2         entityID;
  SeqEntryPtr   rsult;
  SeqEntryPtr   sep;

  rsult = NULL;
  if (path != NULL && path [0] != '\0') {
    dataptr = ObjMgrGenericAsnTextFileRead (path, &datatype, &entityID);
    if (dataptr != NULL && entityID > 0) {
      if (datatype == OBJ_SEQSUB || datatype == OBJ_SEQENTRY ||
          datatype == OBJ_BIOSEQ || datatype == OBJ_BIOSEQSET) {
        WatchCursor ();
        sep = GetTopSeqEntryForEntityID (entityID);
        if (sep == NULL) {
          sep = SeqEntryNew ();
          if (sep != NULL) {
            if (datatype == OBJ_BIOSEQ) {
              bsp = (BioseqPtr) dataptr;
              sep->choice = 1;
              sep->data.ptrvalue = bsp;
              SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, sep);
            } else if (datatype == OBJ_BIOSEQSET) {
              bssp = (BioseqSetPtr) dataptr;
              sep->choice = 2;
              sep->data.ptrvalue = bssp;
              SeqMgrSeqEntry (SM_BIOSEQSET, (Pointer) bssp, sep);
            } else {
              sep = SeqEntryFree (sep);
            }
          }
          sep = GetTopSeqEntryForEntityID (entityID);
        }
        if (sep != NULL) {
          rsult = SeqEntryNew ();
          if (rsult != NULL) {
            rsult->choice = sep->choice;
            rsult->data.ptrvalue = sep->data.ptrvalue;
            sep->data.ptrvalue = NULL;
            if (datatype == OBJ_SEQSUB) {
              SeqSubmitFree ((SeqSubmitPtr) dataptr);
            } else {
              SeqEntryFree (sep);
            }
            /*Message (MSG_OK, "Call new object manager free function");*/
          }
        }
        ArrowCursor ();
      }
      /*
      ObjMgrDelete (entityID);
      */
    }
  }
  return rsult;
}

static void RestoreSeqEntryProc (IteM i)

{
  SeqEntryPtr  currsep;
  BaseFormPtr  bfp;
  SeqEntryPtr  oldsep;
  Char         path [PATH_MAX];

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp != NULL && bfp->input_itemtype == OBJ_BIOSEQ) {
    if (GetInputFileName (path, sizeof (path), "", "TEXT")) {
      SeqEntrySetScope (NULL);
      oldsep = RestoreFromFile (path);
      currsep = GetTopSeqEntryForEntityID (bfp->input_entityID);
      ReplaceSeqEntryWithSeqEntry (currsep, oldsep, TRUE);
      SeqEntrySetScope (NULL);
      bfp->input_entityID = ObjMgrGetEntityIDForChoice (currsep);
      ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
      ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
    }
  }
}

void AddAboutAndHelpMenuItems (MenU m)

{
  CommandItem (m, "About Sequin...", AboutProc);
  CommandItem (m, "Help...", DisplayHelpFormProc);
  SeparatorItem (m);
}

static void FindOrf (IteM i)

{
  BaseFormPtr  bfp;
  BioseqPtr    bsp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (bsp == NULL) return;
  LaunchOrfViewer (bsp, bfp->input_entityID, bfp->input_itemID, FALSE);
}

static void FindAlu (IteM i)

{
  MsgAnswer    ans;
  BaseFormPtr  bfp;
  BioseqPtr    bsp;
  Boolean      got_some;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (bsp == NULL) return;
  if (bsp->length >= 100) {
    ans = Message (MSG_OKC, "This calculation may take a while");
    if (ans == ANS_CANCEL) return;
  }
  WatchCursor ();
  got_some = FindHumanRepeats (bsp, TRUE);
  ArrowCursor ();
  if (got_some) {
    Message (MSG_OK, "Repeat regions were found.");
    ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  } else {
    Message (MSG_OK, "No Repeat regions found.");
  }
}

typedef struct changetargetform {
  FORM_MESSAGE_BLOCK
  TexT               seqid;
  BaseFormPtr        base;
} ChangeTargetForm, PNTR ChangeTargetFormPtr;

static void AcceptChangeTargetProc (ButtoN b)

{
  ChangeTargetFormPtr  cfp;
  Char                 str [128];

  cfp = (ChangeTargetFormPtr) GetObjectExtra (b);
  if (cfp == NULL) return;
  Hide (cfp->form);
  GetTitle (cfp->seqid, str, sizeof (str));
  SetBioseqViewTarget (cfp->base, str);
  Remove (cfp->form);
}

static void ChangeTargetMessageProc (ForM f, Int2 mssg)

{
  StdEditorProcsPtr  sepp;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    if (sepp->handleMessages != NULL) {
      sepp->handleMessages (f, mssg);
    }
  }
}

static void DoChangeTarget (IteM i)

{
  ButtoN               b;
  BaseFormPtr          bfp;
  GrouP                c;
  ChangeTargetFormPtr  cfp;
  GrouP                g;
  StdEditorProcsPtr    sepp;
  WindoW               w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  cfp = (ChangeTargetFormPtr) MemNew (sizeof (ChangeTargetForm));
  if (cfp == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "Select Target by SeqID", StdCloseWindowProc);
  SetObjectExtra (w, cfp, StdCleanupFormProc);
  cfp->form = (ForM) w;
  cfp->formmessage = ChangeTargetMessageProc;

  g = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (g, 10, 10);

  cfp->seqid = DialogText (g, "", 12, NULL);
  cfp->base = bfp;
  c = HiddenGroup (g, 2, 0, NULL);
  b = DefaultButton (c, "Accept", AcceptChangeTargetProc);
  SetObjectExtra (b, cfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) cfp->seqid, (HANDLE) c, NULL);
  RealizeWindow (w);
  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
  }
  Show (w);
  Select (w);
}

static void ConfigFormMessage (ForM f, Int2 mssg)

{
  BaseFormPtr        bfp;
  StdEditorProcsPtr  sepp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
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
        sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
        if (sepp != NULL && sepp->handleMessages != NULL) {
          sepp->handleMessages (f, mssg);
        }
        break;
    }
  }
}

static void ConfigAccepted (void)

{
  SetAppParam ("SEQUIN", "SETTINGS", "PUBLICNETWORKSEQUIN", "TRUE");
  Message (MSG_OK, "Setting will take affect when you restart Sequin");
}

static void ConfigCancelled (void)

{
  Message (MSG_OK, "No changes to the network configuration have been made");
}

static void ConfigTurnedOff (void)

{
  SetAppParam ("SEQUIN", "SETTINGS", "PUBLICNETWORKSEQUIN", "FALSE");
  Message (MSG_OK, "Setting will take affect when you restart Sequin");
}

typedef struct prefsform {
  FORM_MESSAGE_BLOCK
  DialoG             prefs;
} PrefsForm, PNTR PrefsFormPtr;

static void AcceptPrefsProc (ButtoN b)

{
  EntrezPrefsPtr  epp;
  PrefsFormPtr    pfp;

  pfp = (PrefsFormPtr) GetObjectExtra (b);
  if (pfp == NULL) return;
  Hide (pfp->form);
  epp = (EntrezPrefsPtr) DialogToPointer (pfp->prefs);
  if (epp != NULL) {
    entrezglobals.persistDefault = epp->persistDefault;
    entrezglobals.alignDefault = epp->alignDefault;
    entrezglobals.lookupDirect = epp->lookupDirect;
    entrezglobals.showAsn = epp->showAsn;
    ReplaceString (&entrezglobals.initDatabase, epp->initDatabase);
    ReplaceString (&entrezglobals.initField, epp->initField);
    ReplaceString (&entrezglobals.initMode, epp->initMode);
    ReplaceString (&medviewprocs.initMedLabel, epp->initMedLabel);
    ReplaceString (&seqviewprocs.initNucLabel, epp->initNucLabel);
    ReplaceString (&seqviewprocs.initProtLabel, epp->initProtLabel);
    ReplaceString (&seqviewprocs.initGenomeLabel, epp->initGenomeLabel);
    SetSequinAppParamTF ("PREFERENCES", "PARENTSPERSIST", entrezglobals.persistDefault);
    SetSequinAppParamTF ("PREFERENCES", "ALIGNCHECKED", entrezglobals.alignDefault);
    SetSequinAppParamTF ("PREFERENCES", "LOOKUPDIRECT", entrezglobals.lookupDirect);
    SetSequinAppParamTF ("PREFERENCES", "SHOWASNPAGE", entrezglobals.showAsn);
    SetSequinAppParam ("SETTINGS", "DATABASE", entrezglobals.initDatabase);
    SetSequinAppParam ("SETTINGS", "FIELD", entrezglobals.initField);
    SetSequinAppParam ("SETTINGS", "MODE", entrezglobals.initMode);
    SetSequinAppParam ("SETTINGS", "MEDPAGE", medviewprocs.initMedLabel);
    SetSequinAppParam ("SETTINGS", "NUCPAGE", seqviewprocs.initNucLabel);
    SetSequinAppParam ("SETTINGS", "PRTPAGE", seqviewprocs.initProtLabel);
    SetSequinAppParam ("SETTINGS", "GENMPAGE", seqviewprocs.initGenomeLabel);
  }
  EntrezPrefsFree (epp);
  Remove (pfp->form);
}

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

static void PreferencesProc (IteM i)

{
  ButtoN          b;
  GrouP           c;
  EntrezPrefsPtr  epp;
  GrouP           g;
  PrefsFormPtr    pfp;
  WindoW          w;

  pfp = (PrefsFormPtr) MemNew (sizeof (PrefsForm));
  if (pfp == NULL) return;
  if (! EntrezIsInited ()) {
    /* EntrezBioseqFetchEnable ("Sequin", TRUE); */
    SequinEntrezInit ("Sequin", FALSE, NULL);
  }
  w = FixedWindow (-50, -33, -10, -10, "Preferences", StdCloseWindowProc);
  SetObjectExtra (w, pfp, StdCleanupFormProc);
  pfp->form = (ForM) w;
  pfp->formmessage = DefaultMessageProc;
  g = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (g, 10, 10);
  pfp->prefs = CreateEntrezPrefsDialog (g, NULL);
  c = HiddenGroup (g, 2, 0, NULL);
  b = PushButton (c, "Accept", AcceptPrefsProc);
  SetObjectExtra (b, pfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) pfp->prefs, (HANDLE) c, NULL);
  RealizeWindow (w);
  epp = EntrezPrefsNew ();
  if (epp != NULL) {
    epp->persistDefault = entrezglobals.persistDefault;
    epp->alignDefault = entrezglobals.alignDefault;
    epp->lookupDirect = entrezglobals.lookupDirect;
    epp->showAsn = entrezglobals.showAsn;
    epp->initDatabase = StringSaveNoNull (entrezglobals.initDatabase);
    epp->initField = StringSaveNoNull (entrezglobals.initField);
    epp->initMode = StringSaveNoNull (entrezglobals.initMode);
    epp->initMedLabel = StringSaveNoNull (medviewprocs.initMedLabel);
    epp->initNucLabel = StringSaveNoNull (seqviewprocs.initNucLabel);
    epp->initProtLabel = StringSaveNoNull (seqviewprocs.initProtLabel);
    epp->initGenomeLabel = StringSaveNoNull (seqviewprocs.initGenomeLabel);
  }
  PointerToDialog (pfp->prefs, (Pointer) epp);
  EntrezPrefsFree (epp);
  Show (w);
  Select (w);
}

void NetConfigureProc (IteM i)

{
  Boolean  netCurrentlyOn = FALSE;
  Char     str [32];

  if (GetAppParam ("SEQUIN", "SETTINGS", "PUBLICNETWORKSEQUIN", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      netCurrentlyOn = TRUE;
    }
  }
  if (useEntrez) {
    netCurrentlyOn = TRUE;
  }
  SendHelpScrollMessage (helpForm, "Misc Menu", "Net Configure");
  ShowNetConfigForm (ConfigFormActivated, ConfigFormMessage,
                     ConfigAccepted, ConfigCancelled,
                     ConfigTurnedOff, netCurrentlyOn);
}

/*
static void DoUpdateSeq (IteM i, CharPtr label, Uint2 import_format)

{
  BaseFormPtr  bfp;
  BioseqPtr    bsp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  bsp = GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (bsp == NULL) return;
  SetAppProperty ("ReadAlignViewType", label);
  ReadAlignView (bsp, import_format);
  SetAppProperty ("ReadAlignViewType", NULL);
}

static void ReplaceSeq (IteM i)

{
  DoUpdateSeq (i, "Replace", SALSA_FASTA);
}

static void ExtendSeq5 (IteM i)

{
  DoUpdateSeq (i, "Extend5", SALSA_FASTA);
}

static void ExtendSeq3 (IteM i)

{
  DoUpdateSeq (i, "Extend3", SALSA_FASTA);
}

static void CombineRecs (IteM i)

{
  DoUpdateSeq (i, "CombineRecs", SALSA_ASN1);
}
*/

static void HasPubsCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  BoolPtr       rsult;
  ValNodePtr    sdp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  rsult = (BoolPtr) mydata;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sdp = bsp->descr;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sdp = bssp->descr;
  } else return;
  while (sdp != NULL) {
    if (sdp->choice == Seq_descr_pub) {
      *rsult = TRUE;
      return;
    }
    sdp = sdp->next;
  }
}

static Boolean LIBCALL SeqEntryHasPubs (SeqEntryPtr sep)

{
  Boolean  rsult;

  rsult = FALSE;
  if (sep != NULL) {
    SeqEntryExplore (sep, (Pointer) &rsult, HasPubsCallback);
  }
  return rsult;
}

static CharPtr convPubDescMssg =
"Do you wish to convert publications to apply only to the appropriate ranges?";

static FloatHi act_get_eval(Int4 exp)
{
  FloatHi eval;
  Int4 i;

  eval = 1;
  for (i=1; i<=exp; i++)
  {
     eval = eval/10;
  }
  return eval;
}

static void Sequin_GetNthSeqRangeInSASet(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop)
{
   SeqAlignPtr  salp;
   Int4         start_tmp;
   Int4         stop_tmp;
   Int4         tmp1;
   Int4         tmp2;
  
   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return;
   salp = (SeqAlignPtr)(sap->segs);
   start_tmp = stop_tmp = -1;
   while (salp != NULL)
   {
      if (n > salp->dim)
      {
         if (start)
            *start = -1;
         if (stop)
            *stop = -1;
         return;
      }
      AlnMgrGetNthSeqRangeInSA(salp, n, &tmp1, &tmp2);
      if (tmp1 < start_tmp || start_tmp == -1)
         start_tmp = tmp1;
      if (tmp2 > stop_tmp)
         stop_tmp = tmp2;
      salp = salp->next;
   }
   if (start)
      *start = start_tmp;
   if (stop)
      *stop = stop_tmp;
}

static SeqAlignPtr Sequin_FindPiece(BioseqPtr bsp1, BioseqPtr bsp2, Int4 start1, Int4 stop1, Int4 start2, Int4 stop2, Uint1 strand)
{
   AMAlignIndexPtr      amaip;
   Int4                 i;
   Int4                 nstart1;
   Int4                 nstart2;
   Int4                 nstop1;
   Int4                 nstop2;
   BLAST_OptionsBlkPtr  options;
   CharPtr              program;
   SeqAlignPtr          sap;
   SeqAlignPtr          sap_head;
   SeqAlignPtr          sap_new;
   SeqAlignPtr          sap_prev;
   SeqLocPtr            slp1;
   SeqLocPtr            slp2;

   if (stop1 - start1 < 4 || stop2 - start2 < 4) /* too small even for dotplot */
      return NULL;
   if (stop1 - start1 < 7 || stop2 - start2 < 7) /* can't do these by BLAST -- wordsize can't go that small */
   {
      slp1 = SeqLocIntNew(start1, stop1, Seq_strand_plus, bsp1->id);
      slp2 = SeqLocIntNew(start2, stop2, strand, bsp2->id);
      sap = Sequin_FindBestAlnByDotPlot(slp1, slp2);
      SeqLocFree(slp1);
      SeqLocFree(slp2);
      return sap;
   }
   if (ISA_aa(bsp1->mol))
   {
      if (ISA_aa(bsp2->mol))
         program = StringSave("blastp");
      else
         return NULL;
   } else if (ISA_na(bsp1->mol))
   {
      if (ISA_na(bsp2->mol))
         program = StringSave("blastn");
      else
         return NULL;
   }
   options = BLASTOptionNew(program, TRUE);
   options->gapped_calculation = TRUE;
   options->expect_value = 10;
   options->gap_x_dropoff_final = 100;
   options->gap_open = 5;
   options->gap_extend = 1;
   options->penalty = -1;
   options->wordsize = 7;
   slp1 = SeqLocIntNew(start1, stop1, Seq_strand_plus, bsp1->id);
   slp2 = SeqLocIntNew(start2, stop2, strand, bsp2->id);
   sap = BlastTwoSequencesByLoc(slp1, slp2, program, options);
   BLASTOptionDelete(options);
   MemFree(program);
   if (sap == NULL)
      sap = Sequin_FindBestAlnByDotPlot(slp1, slp2);
   SeqLocFree(slp1);
   SeqLocFree(slp2);
   if (sap == NULL)
      return NULL;
   AlnMgrIndexLite(sap);
   Sequin_RemoveInconsistentAlnsFromSet(sap, 20, 1);
   amaip = (AMAlignIndexPtr)(sap->saip);
   AlnMgrSortAlnSetByNthRowPos(sap, 1);
   Sequin_GetNthSeqRangeInSASet(sap, 1, &nstart1, &nstop1);
   Sequin_GetNthSeqRangeInSASet(sap, 2, &nstart2, &nstop2);
   strand = AlnMgrGetNthStrand(amaip->saps[0], 2);
   sap_head = NULL;
   if (strand != Seq_strand_minus)
   {
      if (nstart1 > start1+20 && nstart2 > start2+20)
      {
         slp1 = SeqLocIntNew(start1, nstart1, Seq_strand_plus, bsp1->id);
         slp2 = SeqLocIntNew(start2, nstart2, strand, bsp2->id);
         sap_head = Sequin_FindBestAlnByDotPlot(slp1, slp2);
         SeqLocFree(slp1);
         SeqLocFree(slp2);
      }
   } else
   {
      if (nstart1 > start1+20 && nstop2 < stop2 - 20)
      {
         slp1 = SeqLocIntNew(start1, nstart1, Seq_strand_plus, bsp1->id);
         slp2 = SeqLocIntNew(nstop2, stop2, strand, bsp2->id);
         sap_head = Sequin_FindBestAlnByDotPlot(slp1, slp2);
         SeqLocFree(slp1);
         SeqLocFree(slp2);
      }
   }
   for (i=0; i<amaip->numsaps-1; i++)
   {
      AlnMgrGetNthSeqRangeInSA(amaip->saps[i], 1, NULL, &nstart1);
      AlnMgrGetNthSeqRangeInSA(amaip->saps[i+1], 1, &nstop1, NULL);
      if (strand != Seq_strand_minus)
      {
         AlnMgrGetNthSeqRangeInSA(amaip->saps[i], 2, NULL, &nstart2);
         AlnMgrGetNthSeqRangeInSA(amaip->saps[i+1], 2, &nstop2, NULL);
      } else
      {
         AlnMgrGetNthSeqRangeInSA(amaip->saps[i], 2, &nstop2, NULL);
         AlnMgrGetNthSeqRangeInSA(amaip->saps[i+1], 2, NULL, &nstart2);
      }
      if (nstart1 < nstop1 - 5 && nstart2 < nstop2 - 5)
      {
         slp1 = SeqLocIntNew(nstart1, nstop1, Seq_strand_plus, bsp1->id);
         slp2 = SeqLocIntNew(nstart2, nstop2, strand, bsp2->id);
         sap_new = Sequin_FindBestAlnByDotPlot(slp1, slp2);
         SeqLocFree(slp1);
         SeqLocFree(slp2);
         if (sap_head)
         {
            sap_prev->next = sap_new;
            if (sap_new != NULL)
               sap_prev = sap_new;
         } else
            sap_head = sap_prev = sap_new;
      }
   }
   Sequin_GetNthSeqRangeInSASet(sap, 1, &nstart1, &nstop1);
   Sequin_GetNthSeqRangeInSASet(sap, 2, &nstart2, &nstop2);
   if (strand != Seq_strand_minus)
   {
      if (nstop1 < stop1-20 && nstop2 < stop2-20)  /* missing piece at the end */
      {
         slp1 = SeqLocIntNew(nstop1, stop1, Seq_strand_plus, bsp1->id);
         slp2 = SeqLocIntNew(nstop2, stop2, strand, bsp2->id);
         sap_new = Sequin_FindBestAlnByDotPlot(slp1, slp2);
         SeqLocFree(slp1);
         SeqLocFree(slp2);
         if (sap_new != NULL)
         {
            if (sap_head != NULL)
            {
               sap_prev->next = sap_new;
               sap_prev = sap_new;
            } else
              sap_head = sap_new;
         }
      }
   } else
   {
      if (nstop1 < stop1-20 && nstart2 > start2 + 20)
      {
         slp1 = SeqLocIntNew(nstop1, stop1, Seq_strand_plus, bsp1->id);
         slp2 = SeqLocIntNew(start2, nstart2, strand, bsp2->id);
         sap_new = Sequin_FindBestAlnByDotPlot(slp1, slp2);
         SeqLocFree(slp1);
         SeqLocFree(slp2);
         if (sap_new != NULL)
         {
            if (sap_head != NULL)
            {
               sap_prev->next = sap_new;
               sap_prev = sap_new;
            } else
               sap_head = sap_new;
         }
      }
   }
   sap_new = (SeqAlignPtr)(sap->segs);
   while (sap_new->next != NULL)
   {
      sap_new = sap_new->next;
   }
   sap_new->next = sap_head;
   sap_head = (SeqAlignPtr)(sap->segs);
   sap->segs = NULL;
   SeqAlignFree(sap);
   return sap_head;
}

static SeqAlignPtr SPI_CreateContinuousAln(SeqAlignPtr PNTR saps, Int4 numsaps)
{
   DenseSegPtr  dsp;
   DenseSegPtr  dsp_tmp;
   Int4         i;
   Int4         j;
   Int4         n1;
   Int4         n2;
   Int4         numseg;
   SeqAlignPtr  salp;
   Int4         start1;
   Int4         start2;
   Int4         stop1;
   Int4         stop2;
   Uint1        strand;

   for (i=0; i<numsaps-1; i++)
   {
      AlnMgrGetNthSeqRangeInSA(saps[i], 1, &start1, &stop1);
      AlnMgrGetNthSeqRangeInSA(saps[i+1], 1, &start2, &stop2);
      if (start2 - stop1 > 1)
         SQN_ExtendAlnRight(saps[i], 1, stop1+1, start2-1);
      AlnMgrGetNthSeqRangeInSA(saps[i], 2, &start1, &stop1);
      AlnMgrGetNthSeqRangeInSA(saps[i+1], 2, &start2, &stop2);
      strand = AlnMgrGetNthStrand(saps[i], 2);
      if (strand == Seq_strand_minus)
      {
         if (stop2 - start1 > 1)
            SQN_ExtendAlnRight(saps[i], 2, stop2+1, start1-1);
      } else
      {
         if (start2 - stop1 > 1)
            SQN_ExtendAlnRight(saps[i], 2, stop1+1, start2-1);
      }
   }
   numseg = 0;
   for (i=0; i<numsaps; i++)
   {
      dsp_tmp = (DenseSegPtr)(saps[i]->segs);
      numseg += dsp_tmp->numseg;
   }
   dsp = DenseSegNew();
   dsp->dim = 2;
   dsp->numseg = numseg;
   dsp->starts = (Int4Ptr)MemNew(2*numseg*sizeof(Int4));
   dsp->lens = (Int4Ptr)MemNew(numseg*sizeof(Int4));
   dsp->strands = (Uint1Ptr)MemNew(2*numseg*sizeof(Uint1));
   n1 = n2 = 0;
   for (i=0; i<numsaps; i++)
   {
      dsp_tmp = (DenseSegPtr)(saps[i]->segs);
      if (dsp->ids == NULL)
         dsp->ids = SeqIdDupList(dsp_tmp->ids);
      for (j=0; j<2*dsp_tmp->numseg; j++)
      {
         dsp->starts[n1+j] = dsp_tmp->starts[j];
         dsp->strands[n1+j] = dsp_tmp->strands[j];
      }
      for (j=0; j<dsp_tmp->numseg; j++)
      {
         dsp->lens[n2+j] = dsp_tmp->lens[j];
      }
      n1 += 2*dsp_tmp->numseg;
      n2 += dsp_tmp->numseg;
   }
   salp = SeqAlignNew();
   salp->type = SAT_PARTIAL;
   salp->segtype = SAS_DENSEG;
   salp->dim = 2;
   salp->segs = (Pointer)(dsp);
   AlnMgrIndexSingleChildSeqAlign(salp);
   return salp;
}

static void SQN_ExtendAlnRight(SeqAlignPtr sap, Int4 which_row, Int4 start, Int4 stop)
{
   DenseSegPtr  dsp;
   Int4         i;
   Int4Ptr      lens;
   Int4Ptr      starts;
   Uint1Ptr     strands;
 
   if (sap == NULL)
      return;
   if (which_row > 2)
      return;
   dsp = (DenseSegPtr)(sap->segs);
   if (dsp->starts[2*(dsp->numseg-1) + which_row - 1] == -1 || dsp->starts[2*(dsp->numseg-1) + (2-which_row)] != -1)
   {
      starts = (Int4Ptr)MemNew((dsp->numseg+1)*2*sizeof(Int4));
      strands = (Uint1Ptr)MemNew((dsp->numseg+1)*2*sizeof(Uint1));
      lens = (Int4Ptr)MemNew((dsp->numseg+1)*sizeof(Int4));
      for (i=0; i<dsp->numseg; i++)
      {
         lens[i] = dsp->lens[i];
      }
      for (i=0; i<=(dsp->dim)*(dsp->numseg-1)+1; i++)
      {
         starts[i] = dsp->starts[i];
         strands[i] = dsp->strands[i];
      }
      lens[dsp->numseg] = stop - start + 1;
      if (dsp->strands[which_row-1] != Seq_strand_minus)
         starts[(dsp->dim)*(dsp->numseg) + which_row - 1] = start;
      else
         starts[(dsp->dim)*(dsp->numseg) + which_row - 1] = stop;
      starts[(dsp->dim)*(dsp->numseg) + (2-which_row)] = -1;
      strands[(dsp->dim)*(dsp->numseg) + which_row - 1] = dsp->strands[which_row-1];
      strands[(dsp->dim)*(dsp->numseg) + (2-which_row)] = dsp->strands[2-which_row];
      MemFree(dsp->starts);
      MemFree(dsp->lens);
      MemFree(dsp->strands);
      dsp->numseg++;
      dsp->starts = starts;
      dsp->strands = strands;
      dsp->lens = lens;
   } else
   {
      dsp->lens[dsp->numseg-1] += stop - start + 1;
      if (dsp->strands[which_row-1] == Seq_strand_minus)
         dsp->starts[(dsp->dim)*(dsp->numseg-1) + which_row - 1] = stop;
   }
   SAIndexFree(sap->saip);
   sap->saip = NULL;
   AlnMgrIndexSingleChildSeqAlign(sap);
}

static SeqAlignPtr Sequin_CleanUpAlignments(SeqAlignPtr sap, Int4 len1, Int4 len2)
{
   AMAlignIndexPtr  amaip;
   Int4             diff;
   DenseSegPtr      dsp;
   Int4             i;
   Int4             j;
   Int4             numseg;
   SeqAlignPtr      sap_head;
   SeqAlignPtr      sap_prev;
   Int4             start1;
   Int4             start2;
   Int4             stop1;
   Int4             stop2;
   Uint1            strand;
   Int4             tmp;

   if (sap == NULL)
      return NULL;
   AlnMgrSortAlnSetByNthRowPos(sap, 1);
   amaip = (AMAlignIndexPtr)(sap->saip);
   strand = AlnMgrGetNthStrand(amaip->saps[0], 2);
   numseg = 0;
   AlnMgrGetNthSeqRangeInSA(amaip->saps[0], 1, &start1, NULL);
   AlnMgrGetNthSeqRangeInSA(amaip->saps[0], 2, &start2, &stop2);
   if (strand != Seq_strand_minus)
      diff = start2;
   else
      diff = len2 - stop2;
   if (start1 > 0 && diff > 0)
      numseg += 2;
   for (i=0; i<amaip->numsaps-1; i++)
   {
      while (amaip->saps[i] == NULL && i<amaip->numsaps-1)
      {
         i++;
      }
      j = i+1;
      while (amaip->saps[j] == NULL && j<amaip->numsaps)
      {
         j++;
      }
      if (j < amaip->numsaps && amaip->saps[i] != NULL && amaip->saps[j] != NULL)
      {
         dsp = (DenseSegPtr)(amaip->saps[i]->segs);
         numseg += dsp->numseg;
         AlnMgrGetNthSeqRangeInSA(amaip->saps[i], 1, NULL, &start1);
         AlnMgrGetNthSeqRangeInSA(amaip->saps[j], 1, &stop1, &tmp);
         if (stop1 < start1+1)
         {
            if (!AlnMgrTruncateSAP(amaip->saps[j], start1+1, tmp, 1))
            {
               SeqAlignFree(amaip->saps[j]);
               amaip->saps[j] = NULL;
            }
         }
         if (amaip->saps[j] != NULL)
         {
            if (strand != Seq_strand_minus)
            {
               AlnMgrGetNthSeqRangeInSA(amaip->saps[i], 2, NULL, &start2);
               AlnMgrGetNthSeqRangeInSA(amaip->saps[j], 2, &stop2, &tmp);
               if (stop2 < start2+1)
               {
                  if (!AlnMgrTruncateSAP(amaip->saps[j], start2+1, tmp, 2))
                  {
                     SeqAlignFree(amaip->saps[j]);
                     amaip->saps[j] = NULL;
                  }
               }
            } else
            {
               AlnMgrGetNthSeqRangeInSA(amaip->saps[i], 2, &stop2, &tmp);
               AlnMgrGetNthSeqRangeInSA(amaip->saps[j], 2, NULL, &start2);
               if (stop2 < start2 + 1)
               {
                  if (!AlnMgrTruncateSAP(amaip->saps[i], start2+1, tmp, 2))
                  {
                     SeqAlignFree(amaip->saps[i]);
                     amaip->saps[i] = NULL;
                  }
               }
            }
            if (amaip->saps[i] != NULL && amaip->saps[j] != NULL)
            {
               AlnMgrGetNthSeqRangeInSA(amaip->saps[i], 1, NULL, &start1);
               AlnMgrGetNthSeqRangeInSA(amaip->saps[j], 1, &stop1, &tmp);
               if (strand != Seq_strand_minus)
               {
                  AlnMgrGetNthSeqRangeInSA(amaip->saps[i], 2, NULL, &start2);
                  AlnMgrGetNthSeqRangeInSA(amaip->saps[j], 2, &stop2, &tmp);
               } else
               {
                  AlnMgrGetNthSeqRangeInSA(amaip->saps[i], 2, &stop2, &tmp);
                  AlnMgrGetNthSeqRangeInSA(amaip->saps[j], 2, NULL, &start2);
               }
               if (stop1 > start1+1)
                  numseg++;
               if (stop2 > start2+1)
                  numseg++;
            } else if (amaip->saps[j] == NULL)
               i--;
         }
      }
   }
   sap_head = sap_prev = NULL;
   j = 0;
   for (i=0; i<amaip->numsaps; i++)
   {
      if (amaip->saps[i] != NULL)
      {
         j++;
         if (sap_head != NULL)
         {
            sap_prev->next = amaip->saps[i];
            amaip->saps[i]->next = NULL;
            sap_prev = amaip->saps[i];
         } else
         {
            sap_head = sap_prev = amaip->saps[i];
            sap_head->next = NULL;
         }
      }
   }
   amaip->numsaps = j;
   MemFree(amaip->saps);
   amaip->saps = (SeqAlignPtr PNTR)MemNew(j*sizeof(SeqAlignPtr));
   sap_prev = sap_head;
   for (i=0; i<amaip->numsaps; i++)
   {
      amaip->saps[i] = sap_prev;
      sap_prev = sap_prev->next;
   }
   sap->segs = (Pointer)sap_head;
   dsp = (DenseSegPtr)(amaip->saps[amaip->numsaps-1]->segs);
   numseg += dsp->numseg;
   AlnMgrGetNthSeqRangeInSA(amaip->saps[amaip->numsaps-1], 1, NULL, &stop1);
   AlnMgrGetNthSeqRangeInSA(amaip->saps[amaip->numsaps-1], 2, &start2, &stop2);
   if (strand != Seq_strand_minus)
      diff = len2 - stop2;
   else
      diff = start2;
   if (stop1 < len1 && diff > 0)
      numseg += 2;
   return (SPI_CreateContinuousAln(amaip->saps, amaip->numsaps));
}

static Int4 Sequin_FixEnds(SeqAlignPtr sap, Int4 len1, Int4 len2, Int4 overhang)
{
   Int4         chlen;
   DenseSegPtr  dsp;
   DenseSegPtr  dsp_new;
   Int4         i;
   Int4         n;
   Int4         seg;
   Int4         start1;
   Int4         start2;
   Int4         stop1;
   Int4         stop2;
   Uint1        strand;

   n = 0;
   if (sap == NULL)
      return 0;
   if (sap->saip == NULL)
      AlnMgrIndexSingleChildSeqAlign(sap);
   AlnMgrGetNthSeqRangeInSA(sap, 1, &start1, &stop1);
   chlen = 0;
   if (start1 < overhang && start1 > 0)
   {
      chlen += start1;
      n++;
   }
   if (len1 - stop1 > 1 && len1 - stop1 < overhang)
   {
      chlen += len1 - stop1 - 1;
      n++;
   }
   AlnMgrGetNthSeqRangeInSA(sap, 2, &start2, &stop2);
   if (start2 < overhang && start2 > 0)
   {
      chlen += start2;
      n++;
   }
   if (len2 - stop2 > 1 && len2 - stop2 < overhang)
   {
      chlen += len2 - stop2 - 1;
      n++;
   }
   if (n == 0)
      return 0; /* didn't change alignments */
   dsp = (DenseSegPtr)(sap->segs);
   dsp_new = DenseSegNew();
   dsp_new->dim = dsp->dim;
   dsp_new->numseg = dsp->numseg + n;
   dsp_new->ids = dsp->ids;
   dsp->ids = NULL;
   dsp_new->starts = (Int4Ptr)MemNew(dsp_new->dim*dsp_new->numseg*sizeof(Int4));
   dsp_new->lens = (Int4Ptr)MemNew(dsp_new->numseg*sizeof(Int4));
   strand = AlnMgrGetNthStrand(sap, 2);
   dsp_new->strands = (Uint1Ptr)MemNew(dsp_new->dim*dsp_new->numseg*sizeof(Uint1));
   for (i=0; i<dsp_new->numseg; i++)
   {
      dsp_new->strands[dsp_new->dim*i] = Seq_strand_plus;
      dsp_new->strands[dsp_new->dim*i+1] = strand;
   }
   seg = 0;
   if (start1 < overhang && start1 > 0)
   {
      dsp_new->starts[0] = 0;
      dsp_new->starts[1] = -1;
      dsp_new->lens[0] = start1;
      seg = 1;
   }
   if (strand == Seq_strand_minus)
   {
      if (len2 - stop2 > 1 && len2 - stop2 < overhang)
      {
         dsp_new->starts[2*seg+1] = stop2+1;
         dsp_new->starts[2*seg] = -1;
         dsp_new->lens[seg] = len2 - stop2 - 1;
         seg++;
      }
   } else
   {
      if (start2 < overhang && start2 > 0)
      {
         dsp_new->starts[2*seg] = -1;
         dsp_new->starts[2*seg+1] = 0;
         dsp_new->lens[seg] = start2;
         seg++;
      }
   }
   for (i=seg; i<dsp->numseg+seg; i++)
   {
      dsp_new->starts[i*dsp_new->dim] = dsp->starts[(i-seg)*dsp_new->dim];
      dsp_new->starts[i*dsp_new->dim+1] = dsp->starts[(i-seg)*dsp_new->dim+1];
      dsp_new->lens[i] = dsp->lens[i-seg];
   }
   seg = dsp_new->numseg - 1; /* fill last seg 1st */
   if (len1 - stop1 > 1 && len1 - stop1 < overhang)
   {
      dsp_new->starts[(dsp_new->numseg-1)*dsp_new->dim] = stop1+1;
      dsp_new->starts[(dsp_new->numseg-1)*dsp_new->dim+1] = -1;
      dsp_new->lens[dsp_new->numseg-1] = len1-stop1-1;
      seg--;
   }
   if (strand == Seq_strand_minus)
   {
      if (start2 < overhang && start2 > 0)
      {
         dsp_new->starts[seg*dsp_new->dim+1] = 0;
         dsp_new->starts[seg*dsp_new->dim] = -1;
         dsp_new->lens[seg] = start2;
      }
   } else
   {
      if (len2 - stop2 > 1 & len2 - stop2 < overhang)
      {
         dsp_new->starts[seg*dsp_new->dim+1] = stop2+1;
         dsp_new->starts[seg*dsp_new->dim] = -1;
         dsp_new->lens[seg] = len2 - stop2 - 1;
      }
   }
   sap->segs = (Pointer)(dsp_new);
   DenseSegFree(dsp);
   SAIndexFree(sap->saip);
   sap->saip = NULL;
   AlnMgrIndexSingleChildSeqAlign(sap);
   return chlen; /* did change the alignments -> need to warn */
}


extern SeqAlignPtr Sequin_GlobalAlignTwoSeq (BioseqPtr bsp1, BioseqPtr bsp2, Int4Ptr endsfixed)
{
   AMAlignIndexPtr      amaip;
   Int4                 chlen;
   Int4                 i;
   BLAST_OptionsBlkPtr  options;
   CharPtr              program;
   SeqAlignPtr          sap;
   SeqAlignPtr          sap_final;
   SeqAlignPtr          sap_head;
   SeqAlignPtr          sap_new;
   SeqAlignPtr          sap_prev;
   Int4                 start1;
   Int4                 start2;
   Int4                 stop1;
   Int4                 stop2;
   Uint1                strand;

   if (bsp1 == NULL || bsp2 == NULL)
      return NULL;
   if (ISA_aa(bsp1->mol))
   {
      if (ISA_aa(bsp2->mol))
         program = StringSave("blastp");
      else
         return NULL;
   } else if (ISA_na(bsp1->mol))
   {
      if (ISA_na(bsp2->mol))
         program = StringSave("blastn");
      else
         return NULL;
   }
   options = BLASTOptionNew(program, TRUE);
   options->gapped_calculation = TRUE;
   options->expect_value = 0.001;
   if (bsp1->length > 10000 || bsp2->length > 10000)
   {
      options->expect_value = act_get_eval(60);
      options->wordsize = 20;
      options->filter_string = StringSave ("m L");
   }
   sap = BlastTwoSequences(bsp1, bsp2, program, options);
   BLASTOptionDelete(options);
   MemFree(program);
   if (sap == NULL)
   {
      Message(MSG_OK,"BLAST finds no sequence similarity");
      return NULL;
   }
   AlnMgrIndexLite(sap);
   Sequin_RemoveInconsistentAlnsFromSet(sap, 20, 1);
   amaip = (AMAlignIndexPtr)(sap->saip);
   AlnMgrSortAlnSetByNthRowPos(sap, 1);
   Sequin_GetNthSeqRangeInSASet(sap, 1, &start1, &stop1);
   Sequin_GetNthSeqRangeInSASet(sap, 2, &start2, &stop2);
   strand = AlnMgrGetNthStrand(amaip->saps[0], 2);
   sap_head = NULL;
   if (strand != Seq_strand_minus)
   {
      if (start1 > 6 && start2 > 6) /* missing a piece at the beginning */
         sap_head = Sequin_FindPiece(bsp1, bsp2, 0, start1, 0, start2, strand);
   } else
   {
      if (start1 > 6 && stop2 < bsp2->length - 6)
         sap_head = Sequin_FindPiece(bsp1, bsp2, 0, start1, stop2, bsp2->length-1, strand);
   }
   if (sap_head != NULL)
   {
      sap_prev = sap_head;
      while (sap_prev->next != NULL)
      {
         sap_prev = sap_prev->next;
      }
   }
   for (i=0; i<amaip->numsaps-1; i++)
   {
      AlnMgrGetNthSeqRangeInSA(amaip->saps[i], 1, NULL, &start1);
      AlnMgrGetNthSeqRangeInSA(amaip->saps[i+1], 1, &stop1, NULL);
      if (strand != Seq_strand_minus)
      {
         AlnMgrGetNthSeqRangeInSA(amaip->saps[i], 2, NULL, &start2);
         AlnMgrGetNthSeqRangeInSA(amaip->saps[i+1], 2, &stop2, NULL);
      } else
      {
         AlnMgrGetNthSeqRangeInSA(amaip->saps[i], 2, &stop2, NULL);
         AlnMgrGetNthSeqRangeInSA(amaip->saps[i+1], 2, NULL, &start2);
      }
      sap_new = Sequin_FindPiece(bsp1, bsp2, start1, stop1, start2, stop2, strand);
      if (sap_new != NULL && sap_head != NULL)
      {
         sap_prev->next = sap_new;
         if (sap_new != NULL)
            sap_prev = sap_new;
      } else if (sap_new != NULL)
         sap_head = sap_prev = sap_new;
   }
   Sequin_GetNthSeqRangeInSASet(sap, 1, &start1, &stop1);
   Sequin_GetNthSeqRangeInSASet(sap, 2, &start2, &stop2);
   if (strand != Seq_strand_minus)
   {
      if (stop1 < bsp1->length-6 && stop2 < bsp2->length-6)  /* missing piece at the end */
      {
         sap_new = Sequin_FindPiece(bsp1, bsp2, stop1, bsp1->length-1, stop2, bsp2->length-1, strand);
         if (sap_new != NULL)
         {
            if (sap_head != NULL)
            {
               sap_prev->next = sap_new;
               sap_prev = sap_new;
            } else
              sap_head = sap_new;
         }
      }
   } else
   {
      if (stop1 < bsp1->length-6 && start2 > 6)
      {
         sap_new = Sequin_FindPiece(bsp1, bsp2, stop1, bsp1->length-1, 0, start2, strand);
         if (sap_new != NULL)
         {
            if (sap_head != NULL)
            {
               sap_prev->next = sap_new;
               sap_prev = sap_new;
            } else
               sap_head = sap_new;
         }
      }
   }
   sap_new = (SeqAlignPtr)(sap->segs);
   while (sap_new->next != NULL)
   {
      sap_new = sap_new->next;
   }
   sap_new->next = sap_head;  /* put the new alignments in the original set */
   AMAlignIndexFree(amaip);
   sap->saip = NULL;
   AlnMgrIndexLite(sap);  /* reindex the alignments */
   sap_final = Sequin_CleanUpAlignments(sap, bsp1->length, bsp2->length);
   chlen = Sequin_FixEnds(sap_final, bsp1->length, bsp2->length, 1000);
   if (endsfixed != NULL)
      *endsfixed = chlen;
   return sap_final;
}


static SeqAlignPtr Sequin_FindBestAlnByDotPlot(SeqLocPtr slp1, SeqLocPtr slp2)
{
   DOTDiagPtr      ddp;
   DenseSegPtr     dsp;
   Int4            i;
   DOTMainDataPtr  mip;
   SeqAlignPtr     sap;
   SeqAlignPtr     sap_head;
   SeqAlignPtr     sap_prev;
   ScorePtr        scp;
   Int4            start1;
   Int4            start2;
   Uint1           strand;

   ErrSetMessageLevel(SEV_MAX);
   mip = DOT_CreateAndStorebyLoc (slp1, slp2, 6, 10);
   ErrSetMessageLevel(SEV_WARNING);
   sap = sap_head = sap_prev = NULL;
   if (mip == NULL || mip->hitlist == NULL)
      return NULL;
   i = 0;
   ddp = mip->hitlist[i];
   start1 = SeqLocStart(slp1);
   start2 = SeqLocStart(slp2);
   strand = SeqLocStrand(slp2);
   /* copy each ddp (a single ungapped alignment) into a one-segment dense-seg alignment */
   while (ddp != NULL && i < mip->index)
   {
      ddp = mip->hitlist[i];
      i++;
      sap = SeqAlignNew();
      dsp = DenseSegNew();
      sap->type = SAT_PARTIAL;
      sap->segtype = SAS_DENSEG;
      sap->dim = 2;
      dsp->dim = 2;
      dsp->numseg = 1;
      dsp->ids = SeqIdDup(SeqLocId(slp1));
      dsp->ids->next = SeqIdDup(SeqLocId(slp2));
      dsp->strands = (Uint1Ptr)MemNew(2*sizeof(Uint1));
      dsp->strands[0] = SeqLocStrand(slp1);
      dsp->strands[1] = SeqLocStrand(slp2);
      dsp->starts = (Int4Ptr)MemNew(2*sizeof(Int4));
      dsp->lens = (Int4Ptr)MemNew(sizeof(Int4));
      dsp->starts[0] = ddp->q_start;
      if (dsp->strands[1] == Seq_strand_minus)
         dsp->starts[1] = ddp->s_start - ddp->length + 1;
      else
         dsp->starts[1] = ddp->s_start;
      if (ddp->length > SeqLocLen(slp2))
         dsp->lens[0] = SeqLocLen(slp2);
      else
         dsp->lens[0] = ddp->length - 1;
      scp = ScoreNew();
      scp->id = ObjectIdNew();
      scp->id->str = StringSave("score");
      scp->choice = 1;
      scp->value.intvalue = ddp->score;
      dsp->scores = scp;
      sap->segs = (Pointer)(dsp);
      if (sap_head != NULL)
      {
         sap_prev->next = sap;
         sap_prev = sap;
      } else
         sap_head = sap_prev = sap;
   }
   if (sap_head == NULL)
      return NULL;
   AlnMgrIndexLite(sap_head);
   Sequin_RemoveInconsistentAlnsFromSet(sap_head, 6, 1);
   sap = (SeqAlignPtr)(sap_head->segs);
   sap_head->segs = NULL;
   SeqAlignFree(sap_head);
   MemFree(mip->matrix);
   MemFree(mip->qseq);
   MemFree(mip->sseq);
   MemFree(mip->qname);
   MemFree(mip->sname);
   i = 0;
   while (ddp != NULL && i < mip->index)
   {
      ddp = mip->hitlist[i];
      MemFree(ddp);
      i++;
   }
   MemFree(mip->hitlist);
   return sap;
}

typedef struct seq_spin {
   Int4  n1;
   Int4  n2;
   Int4  n3;
   Int4  n4;
   Int4  n5;
} SQN_n, PNTR SQN_nPtr;

static int LIBCALLBACK SQN_CompareSpins(VoidPtr ptr1, VoidPtr ptr2)
{
   SQN_nPtr  spin1;
   SQN_nPtr  spin2;

   spin1 = *((SQN_nPtr PNTR) ptr1);
   spin2 = *((SQN_nPtr PNTR) ptr2);
   if (spin1 == NULL || spin2 == NULL)
      return 0;
   if (spin1->n3 > spin2->n3)
      return -1;
   if (spin1->n3 < spin2->n3)
      return 1;
   if (spin1->n2 < spin2->n2)
      return -1;
   if (spin1->n2 > spin2->n2)
      return 1;
   return 0;
}

static void Sequin_RemoveInconsistentAlnsFromSet(SeqAlignPtr sap, Int4 fuzz, Int4 n)
{
   AMAlignIndexPtr  amaip;
   Boolean          conflict;
   Int4             curr;
   Int4             i;
   Int4             indextype;
   SeqAlignPtr      salp;
   SeqAlignPtr      salp_head;
   SeqAlignPtr      salp_prev;
   SQN_nPtr         PNTR spin;
   Int4             start;
   Int4             stop;
   Int4             strand;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return;
   if (n > 2)
      return;
   amaip = (AMAlignIndexPtr)(sap->saip);
   indextype = amaip->mstype;
   /* make sure that everything is on the plus strand of the nth sequence */
   for (i=0; i<amaip->numsaps; i++)
   {
      salp = amaip->saps[i];
      strand = AlnMgrGetNthStrand(salp, n);
      if (strand == Seq_strand_minus)
      {
         SAIndexFree(salp->saip);
         salp->saip = NULL;
         salp->next = NULL;
         SeqAlignListReverseStrand(salp);
         AlnMgrIndexSingleChildSeqAlign(salp);
      }
   }
   /* spin structure: n1 = which alignment, n2 = start on first row, n3 =
      alignment length on 1st row, n4 = start on 2nd row, n5 = 2nd strand */
   spin = (SQN_nPtr PNTR)MemNew((amaip->numsaps)*sizeof(SQN_nPtr));
   for (i=0; i<amaip->numsaps; i++)
   {
      spin[i] = (SQN_nPtr)MemNew(sizeof(SQN_n));
      salp = amaip->saps[i];
      spin[i]->n1 = i;
      AlnMgrGetNthSeqRangeInSA(salp, n, &start, &stop);
      spin[i]->n3 = stop - start;
      spin[i]->n2 = start;
      AlnMgrGetNthSeqRangeInSA(salp, 3-n, &start, &stop);
      spin[i]->n4 = start;
      strand = AlnMgrGetNthStrand(salp, 3-n);
      if (strand == Seq_strand_minus)
         spin[i]->n5 = -1;
      else
         spin[i]->n5 = 1;
   }
   HeapSort((Pointer)spin, (size_t)(amaip->numsaps), sizeof(SQN_nPtr), SQN_CompareSpins);
   strand = spin[0]->n5;
   for (i=1; i<amaip->numsaps; i++)
   {
      if (spin[i]->n5 != strand)
      {
         salp = amaip->saps[spin[i]->n1];
         salp->next = NULL;
         SeqAlignFree(salp);
         amaip->saps[spin[i]->n1] = NULL;
         spin[i]->n1 = -1;
      }
   }
   for (curr=0; curr<amaip->numsaps; curr++)
   {
      if (spin[curr]->n1 != -1)
      {
         for (i=curr+1; i<amaip->numsaps; i++)
         {
            if (spin[i]->n1 != -1)
            {
               conflict = FALSE;
            /* check first for conflict on first row */
               if (spin[i]->n2 + spin[i]->n3 - 1 > spin[curr]->n2 + fuzz)
               {
                  if (spin[i]->n2 < spin[curr]->n2)
                     conflict = TRUE;
               }
               if (spin[i]->n2 < spin[curr]->n2 + spin[curr]->n3 - 1 - fuzz)
               {
                  if (spin[i]->n2 + spin[i]->n3 - 1 > spin[curr]->n2 + spin[curr]->n3 - 1)
                     conflict = TRUE;
               }
               if (spin[i]->n2 >= spin[curr]->n2)
               {
                  if (spin[i]->n2 + spin[i]->n3 - 1 <= spin[curr]->n2 + spin[curr]->n3 - 1)
                     conflict = TRUE;
               }
            /* then check for conflict and consistency on second row */
               if (spin[i]->n2 + spin[i]->n3 - 1 < spin[curr]->n2 + fuzz)
               {
                  if (strand == 1)
                  {
                     if (spin[i]->n4 + spin[i]->n3 - 1 > spin[curr]->n4 + fuzz)
                        conflict = TRUE;
                  } else if (strand == -1)
                  {
                     if (spin[curr]->n4 + spin[curr]->n3 - 1 - fuzz > spin[i]->n4)
                        conflict = TRUE;
                  }
               } else
               {
                  if (strand == 1)
                  {
                     if (spin[i]->n4 < spin[curr]->n4 + spin[curr]->n3 - fuzz)
                        conflict = TRUE;
                  } else if (strand == -1)
                  {
                     if (spin[i]->n4 + spin[i]->n3 - 1 - fuzz > spin[curr]->n4)
                        conflict = TRUE;
                  }
               }
               if (conflict)
               {
                  salp = amaip->saps[spin[i]->n1];
                  salp->next = NULL;
                  SeqAlignFree(salp);
                  amaip->saps[spin[i]->n1] = NULL;
                  spin[i]->n1 = -1;
               }
            }
         }
      }
   }
   salp_head = salp_prev = NULL;
   for (i=0; i<amaip->numsaps; i++)
   {
      MemFree(spin[i]);
      if (amaip->saps[i] != NULL)
      {
         amaip->saps[i]->next = NULL;
         if (salp_prev != NULL)
         {
            salp_prev->next = amaip->saps[i];
            salp_prev = salp_prev->next;
         } else
            salp_head = salp_prev = amaip->saps[i];
      }
   }
   sap->segs = (Pointer)(salp_head);
   if (indextype == AM_LITE)
   {
      AMAlignIndexFree(sap->saip);
      sap->saip = NULL;
      AlnMgrIndexLite(sap);
   } else
      AlnMgrReIndexSeqAlign(sap);
   MemFree(spin);
}

static void SqnReadAlignViewEx (BioseqPtr target_bsp, SeqEntryPtr source_sep, Int2 choice)

{
  /*
  ValNodePtr  vnp=NULL;  
  SeqLocPtr   slp=NULL;
  SeqLocPtr   source_slp;
  */
  SeqIdPtr    sip /* , source_sip */;
  SeqAlignPtr salp; 
  BioseqPtr   source_bsp = NULL;
  MsgAnswer   ans;
  SeqEntryPtr target_sep;
  Uint2       entityID;
 
  if (target_bsp==NULL)  
     return;
  entityID = ObjMgrGetEntityIDForPointer (target_bsp);
  target_sep = GetBestTopParentForData (entityID, target_bsp);
  if (source_sep!=NULL && source_sep->data.ptrvalue != NULL) {
     if (SeqEntryHasPubs (source_sep) || SeqEntryHasPubs (target_sep)) {
       ans = Message (MSG_YN, convPubDescMssg);
       if (ans == ANS_YES) {
         ConvertPubSrcComDescsToFeats (source_sep, TRUE, FALSE, FALSE, FALSE);
         ConvertPubSrcComDescsToFeats (target_sep, TRUE, FALSE, FALSE, FALSE);
       }
     }
     if (IS_Bioseq_set(source_sep))
        source_sep = FindNucSeqEntry(source_sep);
     if (IS_Bioseq(source_sep)) 
     {
        if (! ISA_na (target_bsp->mol)) {
          Message (MSG_ERROR, "Target Bioseq is not a Nucleotide");
          return;
        }
        sip = SeqIdFindBest(target_bsp->id, 0);
        /* slp = SeqLocIntNew (0, target_bsp->length - 1, Seq_strand_plus, sip); */
        SeqEntryReplaceSeqID (source_sep, sip);
        source_bsp = (BioseqPtr)source_sep->data.ptrvalue;
        if (! ISA_na (source_bsp->mol)) {
          Message (MSG_ERROR, "Source Bioseq is not a Nucleotide");
          return;
        }
        /*
        source_sip = SeqIdFindBest(source_bsp->id, 0);
        if (source_sip==NULL)
           source_sip = source_bsp->id;
        source_slp = SeqLocIntNew (0, source_bsp->length-1, Seq_strand_plus, source_sip);
        ValNodeAddPointer(&vnp, 0, (Pointer)slp);
        ValNodeAddPointer(&vnp, 0, (Pointer)source_slp);
        salp = SeqLocListToSeqAlign (vnp, choice, NULL);
        */
        salp = Sequin_GlobalAlignTwoSeq (target_bsp, source_bsp, NULL);
        if (salp != NULL) {
           LaunchAlignViewer (salp);
        }
     }
  }
}


extern void PrepareToUpdateSequences (
  BioseqPtr oldbsp,
  BioseqPtr newbsp
);
extern void SqnReadAlignView (BioseqPtr target_bsp, SeqEntryPtr source_sep)

{
  BioseqPtr  nbsp;

  /*
  SqnReadAlignViewEx (target_bsp, source_sep, PRGALIGNDEFAULT);
  */
  if (target_bsp == NULL || source_sep == NULL) return;
  nbsp = FindNucBioseq (source_sep);
  if (nbsp == NULL) return;

  PrepareToUpdateSequences (target_bsp, nbsp);
}


static void DoUpdateSeq (IteM i, Uint2 import_format)

{
  BaseFormPtr  bfp;
  BioseqPtr    bsp;
  SeqEntryPtr  sep;
  SeqEntryPtr  source_sep = NULL;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  SendHelpScrollMessage (helpForm, "Edit Menu", "Update Sequence");
  bsp = GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (bsp == NULL) return;
  switch (import_format) { 
     case SALSA_FASTA:   
        source_sep = FastaRead (NULL, Seq_mol_na);
        break; 
     case SALSA_ASN1: 
        source_sep = AsnReadForSalsa (NULL);
        break;    
     default: 
        break; 
  }  
  SqnReadAlignView (bsp, source_sep);
}

extern void NewUpdateSequence (
  IteM i
);

extern void NewFeaturePropagate (
  IteM i
);

/*
static void UpdateSeqTEST (IteM i)

{
  NewUpdateSequence (i);
}
*/

static void UpdateSeqWithFASTA (IteM i)

{
  /*
  DoUpdateSeq (i, SALSA_FASTA);
  */
  NewUpdateSequence (i);
}

static void UpdateSeqWithRec (IteM i)

{
  /*
  DoUpdateSeq (i, SALSA_ASN1);
  */
  NewUpdateSequence (i);
}

/*******COLOMBE ***********/
typedef struct ccid3 {
  SeqEntryPtr target_sep;
  ValNode     sal;
} CcId3, PNTR CcId3Ptr;

typedef struct ccid2 {
  CharPtr     source_sip;
  BioseqPtr   source_bsp;
  ValNode     sal;
} CcId2, PNTR CcId2Ptr;

static void FindBioseqCB2 (SeqEntryPtr sep, Pointer mydata,
                                          Int4 index, Int2 indent)
{
  BioseqPtr          target_bsp,
                     source_bsp;
  SeqIdPtr           tmp_sip;
  Char               target_str[120];
  CcId2Ptr           ci2p;
  SeqLocPtr          slp;
  ValNodePtr         vnp=NULL;
  SeqEntryPtr        source_sep;

  if (sep != NULL && sep->data.ptrvalue && mydata != NULL) {
     ci2p = (CcId2Ptr)mydata;
     if (ci2p->sal.data.ptrvalue==NULL && IS_Bioseq(sep)) 
     {
        target_bsp = (BioseqPtr) sep->data.ptrvalue;
        if (target_bsp->id)
        {
           SeqIdWrite (target_bsp->id, target_str, PRINTID_FASTA_LONG, 120);
           if ((StringStr(target_str, ci2p->source_sip))!=NULL)
           {
              source_bsp = ci2p->source_bsp;
              source_sep = SeqMgrGetSeqEntryForData (source_bsp);
              tmp_sip = MakeSeqID (ci2p->source_sip);
              SeqEntryReplaceSeqID (source_sep, source_bsp->id);
              slp=SeqLocIntNew(0, target_bsp->length-1, Seq_strand_plus, target_bsp->id);
              ValNodeAddPointer (&vnp, 0, (Pointer)slp);
              slp=SeqLocIntNew(0, source_bsp->length-1, Seq_strand_plus, source_bsp->id);
              ValNodeAddPointer (&vnp, 0, (Pointer)slp);
              ci2p->sal.data.ptrvalue=SeqLocListToSeqAlign (vnp, PRGALIGNDEFAULT, NULL);

           }
        }
     }
  }
}

static void FindBioseqCB1 (SeqEntryPtr sep, Pointer mydata,
                                          Int4 index, Int2 indent)
{
  BioseqPtr          source_bsp;
  SeqIdPtr           sip;
  CcId3Ptr            cip;
  CcId2              ci2;
  ObjectIdPtr        oid;

  if (sep != NULL && sep->data.ptrvalue && mydata != NULL) {
     cip = (CcId3Ptr)mydata;
     if (IS_Bioseq(sep)) {
        source_bsp = (BioseqPtr) sep->data.ptrvalue;
        if (source_bsp!=NULL && ISA_na (source_bsp->mol)) {
           sip = SeqIdFindBest(source_bsp->id, 0);
           if (sip->choice==SEQID_LOCAL) {
              oid = (ObjectIdPtr) sip->data.ptrvalue;
              if (oid != NULL && oid->str != NULL) {
                 ci2.source_sip = oid->str;
                 ci2.source_bsp = source_bsp;
                 ci2.sal.data.ptrvalue = NULL;
                 SeqEntryExplore (cip->target_sep, &ci2, FindBioseqCB2);
                 if (ci2.sal.data.ptrvalue) {
                    if (cip->sal.data.ptrvalue==NULL)
                       cip->sal.data.ptrvalue=ci2.sal.data.ptrvalue;
                    else
                       SeqAlignLink(cip->sal.data.ptrvalue, ci2.sal.data.ptrvalue);
                    ci2.sal.data.ptrvalue=NULL;
                 }
              }
           }
        }
     }
  }
}

static void Updates (SeqEntryPtr target_sep, SeqEntryPtr source_sep)
{
  CcId3         cc;
  SeqAlignPtr  salp=NULL;

  if (target_sep==NULL || source_sep==NULL)
     return;
  if (IS_Bioseq_set (target_sep) && IS_Bioseq_set (source_sep))
  {
     cc.target_sep = target_sep;
     cc.sal.data.ptrvalue = NULL;
     SeqEntryExplore (source_sep, &cc, FindBioseqCB1);
     if (cc.sal.data.ptrvalue) {
        salp = (SeqAlignPtr)cc.sal.data.ptrvalue;
        LaunchAlignViewer (salp);
     }
  }
}


static void DoUpdatesSeq (IteM i)
{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;
  SeqEntryPtr  source_sep = NULL;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  SendHelpScrollMessage (helpForm, "Edit Menu", "Update Sequence");
  source_sep = FastaRead (NULL, Seq_mol_na);
  Updates (sep, source_sep);
}

static void CommonAddSeq (IteM i, Int2 type)

{
  EnumFieldAssocPtr  ap;
  BaseFormPtr        bfp;
  Uint1              biomol;
  BioSourcePtr       biop = NULL;
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  Pointer            dataptr;
  Uint2              datatype;
  FILE               *fp;
  MolInfoPtr         mip;
  MolInfoPtr         molinf = NULL;
  OrgRefPtr          orp;
  SeqEntryPtr        nuc;
  Char               path [PATH_MAX];
  CharPtr            ptr;
  SeqEntryPtr        sep;
  BioSourcePtr       src = NULL;
  Char               str [128];
  CharPtr            tax = NULL;
  CharPtr            title = NULL;
  SeqEntryPtr        top;
  ValNodePtr         vnp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  top = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (top == NULL) return;
  if (type == 1 || type == 2) {
    if (! GetInputFileName (path, sizeof (path),"","TEXT")) return;
    fp = FileOpen (path, "r");
    if (fp == NULL) return;
    if (IS_Bioseq_set (top)) {
      bssp = (BioseqSetPtr) top->data.ptrvalue;
      if (bssp != NULL && bssp->_class != BioseqseqSet_class_phy_set) {
        sep = bssp->seq_set;
        vnp = SeqEntryGetSeqDescr (top, Seq_descr_source, NULL);
        if (vnp != NULL) {
          src = (BioSourcePtr) vnp->data.ptrvalue;
          if (src != NULL) {
            orp = biop->org;
            if (orp != NULL) {
              tax = orp->taxname;
            }
          }
        }
      }
    }
    nuc = FindNucSeqEntry (top);
    vnp = SeqEntryGetSeqDescr (nuc, Seq_descr_molinfo, NULL);
    if (vnp != NULL) {
      molinf = (MolInfoPtr) vnp->data.ptrvalue;
    }
    while ((dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, FALSE, TRUE, FALSE)) != NULL) {
      if (datatype == OBJ_BIOSEQ || datatype == OBJ_BIOSEQSET) {
        bsp = NULL;
        bssp = NULL;
        sep = SeqMgrGetSeqEntryForData (dataptr);
        if (sep == NULL) {
          sep = SeqEntryNew ();
          if (datatype == OBJ_BIOSEQ) {
            bsp = (BioseqPtr) dataptr;
            sep->choice = 1;
            sep->data.ptrvalue = bsp;
            SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, sep);
          } else if (datatype == OBJ_BIOSEQSET) {
            bssp = (BioseqSetPtr) dataptr;
            sep->choice = 2;
            sep->data.ptrvalue = bssp;
            SeqMgrSeqEntry (SM_BIOSEQSET, (Pointer) bssp, sep);
          } else {
            sep = SeqEntryFree (sep);
          }
        }
        if (sep != NULL) {
          AddSeqEntryToSeqEntry (top, sep, TRUE);
          title = SeqEntryGetTitle (sep);
          vnp = SeqEntryGetSeqDescr (sep, Seq_descr_source, NULL);
          if (vnp == NULL || title != NULL) {
            ptr = StringISearch (title, "[org=");
            if (ptr != NULL) {
              StringNCpy_0 (str, ptr + 5, sizeof (str));
              ptr = StringChr (str, ']');
            } else {
              ptr = StringISearch (title, "[organism=");
              if (ptr != NULL) {
                StringNCpy_0 (str, ptr + 10, sizeof (str));
                ptr = StringChr (str, ']');
              }
            }
            if (ptr != NULL) {
              *ptr = '\0';
              biop = BioSourceNew ();
              if (biop != NULL) {
                orp = OrgRefNew ();
                biop->org = orp;
                if (orp != NULL) {
                  orp->taxname = StringSave (str);
                }
                vnp = CreateNewDescriptor (sep, Seq_descr_source);
                if (vnp != NULL) {
                  vnp->data.ptrvalue = (Pointer) biop;
                }
              }
            }
          }
          if (vnp == NULL && tax != NULL) {
            biop = BioSourceNew ();
            if (biop != NULL) {
              orp = OrgRefNew ();
              biop->org = orp;
              if (orp != NULL) {
                orp->taxname = StringSave (tax);
              }
              vnp = CreateNewDescriptor (sep, Seq_descr_source);
              if (vnp != NULL) {
                vnp->data.ptrvalue = (Pointer) biop;
              }
            }
          }
          vnp = SeqEntryGetSeqDescr (sep, Seq_descr_source, NULL);
          if (vnp != NULL) {
            biop = (BioSourcePtr) vnp->data.ptrvalue;
          }
          if (biop != NULL && title != NULL) {
            for (ap = orgmod_subtype_alist; ap->name != NULL; ap++) {
              MakeSearchStringFromAlist (str, ap->name);
              AddToOrgMod (biop, title, str, (Int2) ap->value);
              ExciseString (title, str, "]");
            }
            for (ap = subsource_subtype_alist; ap->name != NULL; ap++) {
              MakeSearchStringFromAlist (str, ap->name);
              AddToSubSource (biop, title, str, (Int2) ap->value);
              ExciseString (title, str, "]");
            }
            AddToOrgMod (biop, title, "[note=", 255);
            ExciseString (title, "[note=", "]");
            AddToSubSource (biop, title, "[subsource=", 255);
            ExciseString (title, "[subsource=", "]");
            ExciseString (title, "[org=", "]");
            ExciseString (title, "[organism=", "]");
            if (bsp != NULL) {
              ptr = StringISearch (title, "[molecule=");
              if (ptr != NULL) {
                StringNCpy_0 (str, ptr + 10, sizeof (str));
                ptr = StringChr (str, ']');
                if (ptr != NULL) {
                  *ptr = '\0';
                  if (StringCmp (str, "dna") == 0) {
                    bsp->mol = Seq_mol_dna;
                  } else if (StringCmp (str, "rna") == 0) {
                    bsp->mol = Seq_mol_rna;
                  }
                }
              }
            }
            ptr = StringISearch (title, "[location=");
            if (ptr != NULL) {
              StringNCpy_0 (str, ptr + 10, sizeof (str));
              ptr = StringChr (str, ']');
              if (ptr != NULL) {
                *ptr = '\0';
                if (StringICmp (str, "Mitochondrial") == 0) { /* alternative spelling */
                  biop->genome = 5;
                }
                for (ap = biosource_genome_simple_alist; ap->name != NULL; ap++) {
                  if (StringICmp (str, ap->name) == 0) {
                    biop->genome = (Uint1) ap->value;
                  }
                }
              }
            }
            ptr = StringISearch (title, "[moltype=");
            if (ptr != NULL) {
              biomol = 0;
              StringNCpy_0 (str, ptr + 8, sizeof (str));
              ptr = StringChr (str, ']');
              if (ptr != NULL) {
                *ptr = '\0';
                if (StringICmp (str, "genomic") == 0) {
                  biomol = MOLECULE_TYPE_GENOMIC;
                } else if (StringICmp (str, "mRNA") == 0) {
                  biomol = MOLECULE_TYPE_MRNA;
                }
                if (biomol != 0) {
                  mip = MolInfoNew ();
                  if (mip != NULL) {
                    mip->biomol = biomol;
                    vnp = CreateNewDescriptor (sep, Seq_descr_molinfo);
                    if (vnp != NULL) {
                      vnp->data.ptrvalue = (Pointer) mip;
                    }
                  }
                }
              }
            }
          }
          ExciseString (title, "[molecule=", "]");
          ExciseString (title, "[moltype=", "]");
          ExciseString (title, "[location=", "]");
          TrimSpacesAroundString (title);
          if (title != NULL && StringHasNoText (title)) {
            vnp = NULL;
            if (IS_Bioseq (sep)) {
              bsp = (BioseqPtr) sep->data.ptrvalue;
              vnp = ValNodeExtract (&(bsp->descr), Seq_descr_title);
            } else if (IS_Bioseq_set (sep)) {
              bssp = (BioseqSetPtr) sep->data.ptrvalue;
              vnp = ValNodeExtract (&(bssp->descr), Seq_descr_title);
            }
            if (vnp != NULL && StringHasNoText ((CharPtr) vnp->data.ptrvalue)) {
              vnp = ValNodeFreeData (vnp);
            }
          }
          sep = FindNucSeqEntry (sep);
          vnp = SeqEntryGetSeqDescr (sep, Seq_descr_molinfo, NULL);
          if (vnp == NULL && molinf != NULL) {
            mip = MolInfoNew ();
            if (mip != NULL) {
              vnp = CreateNewDescriptor (sep, Seq_descr_molinfo);
              if (vnp != NULL) {
                vnp->data.ptrvalue = (Pointer) mip;
                mip->biomol = molinf->biomol;
                mip->tech = molinf->tech;
                mip->completeness = molinf->completeness;
              }
            }
          }
        }
      }
    }
    FileClose (fp);
    ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
    Update ();
  } else {
    Message (MSG_OK, "Not yet implemented");
  }
}

static void AddSeqWithFASTA (IteM i)

{
  CommonAddSeq (i, 1);
}

static void AddSeqWithRec (IteM i)

{
  CommonAddSeq (i, 2);
}

/*#ifdef ALLOW_DOWNLOAD*/
BioseqPtr  updateTargetBspKludge = NULL;

static void UpdateSeqWithAcc (IteM i)

{
  BaseFormPtr  bfp;
  BioseqPtr    bsp;
  SeqEntryPtr  sep;
  SeqEntryPtr  source_sep = NULL;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  bsp = GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (bsp == NULL) return;
  updateTargetBspKludge = bsp;
  CommonFetchFromNet (DownloadAndUpdateProc, StdCancelButtonProc);
}
/*#endif*/

static CharPtr obsoletemsg =
"Please use Import and Export items to load and save Uid lists.\n\
The Load and Save Uid List items are obsolete, and will disappear\n\
in the next release.";

static void ObsoleteUidListProc (IteM i)

{
  Message (MSG_OK, "%s", obsoletemsg);
}

#ifndef WIN_MAC
static void MedlineViewFormMenus (WindoW w)

{
  BaseFormPtr  bfp;
  IteM         i;
  MenU         m;

  bfp = (BaseFormPtr) GetObjectExtra (w);
  if (bfp != NULL) {
    m = PulldownMenu (w, "File");
    FormCommandItem (m, "Close", bfp, VIB_MSG_CLOSE);
    SeparatorItem (m);
    i = CommandItem (m, "Duplicate", DuplicateViewProc);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (m);
    FormCommandItem (m, "Export...", bfp, VIB_MSG_EXPORT);
    SeparatorItem (m);
    /*
    FormCommandItem (m, "Save", bfp, VIB_MSG_SAVE);
    FormCommandItem (m, "Save As...", bfp, VIB_MSG_SAVE_AS);
    SeparatorItem (m);
    */
    FormCommandItem (m, "Print...", bfp, VIB_MSG_PRINT);

    m = PulldownMenu (w, "Edit");
    FormCommandItem (m, COPY_MENU_ITEM, bfp, VIB_MSG_COPY);
  }
}

static void BioseqViewFormMenus (WindoW w)

{
  BaseFormPtr    bfp;
  IteM           i;
  MenU           m;
  Int2           mssgadd;
  Int2           mssgalign;
  Int2           mssgdup;
  Int2           mssgfeatprop;
  Int2           mssgseq;
  Int2           mssgsub;
  Int2           mssgupd;
  Int2           mssgupwthaln;
  ObjMgrDataPtr  omdp;
  MenU           sub;

  bfp = (BaseFormPtr) GetObjectExtra (w);
  if (bfp != NULL) {
    m = PulldownMenu (w, "File/ F");
/*#ifdef INTERNAL_NCBI_SEQUIN*/
    if (indexerVersion) {
      if (subtoolMode || stdinMode || binseqentryMode) {
        FormCommandItem (m, "Abort Session", bfp, VIB_MSG_QUIT);
        SeparatorItem (m);
        CommandItem (m, "Accept Changes", SubtoolDoneProc);
        SeparatorItem (m);
      } else if (smartnetMode) {
#ifdef USE_SMARTNET
        FormCommandItem (m, "Abort Session", bfp, VIB_MSG_CLOSE);
        SeparatorItem (m);
        i = CommandItem (m, "Accept Changes", SmartnetDoneProc);
        SetObjectExtra (i, bfp, NULL);
        SeparatorItem (m);
#endif
      }
    }
/*#endif*/
    AddAboutAndHelpMenuItems (m);
    i = CommandItem (m, "Open...", ReadNewAsnProc);
    SetObjectExtra (i, bfp, NULL);
    if (indexerVersion) {
      i = CommandItem (m, "FASTA Nucleotide Direct to Sequence Editor", FastaNucDirectToSeqEdProc);
      SetObjectExtra (i, bfp, NULL);
    }
    SeparatorItem (m);
    FormCommandItem (m, "Close", bfp, VIB_MSG_CLOSE);
    SeparatorItem (m);
    /* FormCommandItem (m, "Import Nucleotide FASTA...", bfp, VIB_MSG_IMPORT); */
    FormCommandItem (m, "Export...", bfp, VIB_MSG_EXPORT);
    SeparatorItem (m);
    i = CommandItem (m, "Duplicate View", DuplicateViewProc);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (m);
    FormCommandItem (m, "Save", bfp, VIB_MSG_SAVE);
    FormCommandItem (m, "Save As...", bfp, VIB_MSG_SAVE_AS);
    SeparatorItem (m);
    i = CommandItem (m, "Restore...", RestoreSeqEntryProc);
    SetObjectExtra (i, bfp, NULL);
    if ((! subtoolMode) && (! stdinMode) &&
        (! binseqentryMode) && (! smartnetMode)) {
      SeparatorItem (m);
      i = CommandItem (m, "Prepare Submission...", PrepareSeqSubmitProc);
      SetObjectExtra (i, bfp, NULL);
      omdp = ObjMgrGetData (bfp->input_entityID);
      if (omdp != NULL && omdp->datatype != OBJ_SEQSUB) {
        Disable (i);
      }
      /*
      i = CommandItem (m, "Submit to NCBI", SubmitToNCBI);
      SetObjectExtra (i, bfp, NULL);
      if (omdp != NULL && omdp->datatype != OBJ_SEQSUB) {
        Disable (i);
      }
      */
    }
/*#ifdef INTERNAL_NCBI_SEQUIN*/
    if (indexerVersion) {
      if ((! subtoolMode) && (! stdinMode) &&
          (! binseqentryMode) && (! smartnetMode)) {
        SeparatorItem (m);
        FormCommandItem (m, "Print", bfp, VIB_MSG_PRINT);
        SeparatorItem (m);
        FormCommandItem (m, "Quit/Q", bfp, VIB_MSG_QUIT);
      } else {
        SeparatorItem (m);
        i = CommandItem (m, "Propagate Top Descriptors", ForcePropagate);
        SetObjectExtra (i, bfp, NULL);
        SeparatorItem (m);
        FormCommandItem (m, "Print", bfp, VIB_MSG_PRINT);
        if (smartnetMode) {
          SeparatorItem (m);
          FormCommandItem (m, "Quit/Q", bfp, VIB_MSG_QUIT);
        }
      }
    } else {
/*#else*/
      if (subtoolMode || stdinMode || binseqentryMode) {
        FormCommandItem (m, "Print", bfp, VIB_MSG_PRINT);
        SeparatorItem (m);
        FormCommandItem (m, "Abort Session", bfp, VIB_MSG_QUIT);
        SeparatorItem (m);
        CommandItem (m, "Accept Changes", SubtoolDoneProc);
      } else if (smartnetMode) {
#ifdef USE_SMARTNET
        FormCommandItem (m, "Print", bfp, VIB_MSG_PRINT);
        SeparatorItem (m);
        FormCommandItem (m, "Abort Session", bfp, VIB_MSG_CLOSE);
        SeparatorItem (m);
        i = CommandItem (m, "Accept Changes", SmartnetDoneProc);
        SetObjectExtra (i, bfp, NULL);
        SeparatorItem (m);
        FormCommandItem (m, "Quit/Q", bfp, VIB_MSG_QUIT);
#endif
      } else {
        SeparatorItem (m);
        FormCommandItem (m, "Print", bfp, VIB_MSG_PRINT);
        SeparatorItem (m);
        FormCommandItem (m, "Quit/Q", bfp, VIB_MSG_QUIT);
      }
    }
/*#endif*/

    m = PulldownMenu (w, "Edit/ E");
    if (subtoolMode || smartnetMode) {
      FormCommandItem (m, UNDO_MENU_ITEM, bfp, VIB_MSG_UNDO);
      SeparatorItem (m);
    }
    FormCommandItem (m, COPY_MENU_ITEM, bfp, VIB_MSG_COPY);
    FormCommandItem (m, CLEAR_MENU_ITEM, bfp, VIB_MSG_DELETE);
    SeparatorItem (m);
    if (extraServices) {
      mssgdup = RegisterFormMenuItemName ("SequinDuplicateItem");
      FormCommandItem (m, "Duplicate...", bfp, mssgdup);
      SeparatorItem (m);
    }
    if (genomeCenter != NULL || indexerVersion) {
      SetupEditSecondary (m, bfp);
      SeparatorItem (m);
    }
    mssgseq = RegisterFormMenuItemName ("SequinEditSequenceItem");
    mssgalign = RegisterFormMenuItemName ("SequinEditAlignmentItem");
    mssgsub = RegisterFormMenuItemName ("SequinEditSubmitterItem");
    mssgupd = RegisterFormMenuItemName ("SequinUpdateSeqSubmenu");
    mssgfeatprop = RegisterFormMenuItemName ("SequinFeaturePropagate");
    mssgadd = RegisterFormMenuItemName ("SequinAddSeqSubmenu");
    FormCommandItem (m, "Edit Sequence...", bfp, mssgseq);
    FormCommandItem (m, "Edit Alignment...", bfp, mssgalign);
    FormCommandItem (m, "Edit Submitter Info...", bfp, mssgsub);
    if (indexerVersion) {
      SeparatorItem (m);
      i = CommandItem (m, "Edit History....", EditSequenceHistory);
      SetObjectExtra (i, bfp, NULL);
    }
    SeparatorItem (m);
    sub = SubMenu (m, "Update Sequence");
    SetFormMenuItem (bfp, mssgupd, (IteM) sub);
    /*
    i = CommandItem (sub, "Replace Sequence...", ReplaceSeq);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (sub, "Extend Sequence 5'...", ExtendSeq5);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (sub, "Extend Sequence 3'...", ExtendSeq3);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (sub, "Combine Records...", CombineRecs);
    SetObjectExtra (i, bfp, NULL);
    */
    /*
    if (indexerVersion) {
      i = CommandItem (sub, "Read File, New Algorithm...", UpdateSeqTEST);
      SetObjectExtra (i, bfp, NULL);
      SeparatorItem (sub);
    }
    */
    i = CommandItem (sub, "Read FASTA File...", UpdateSeqWithFASTA);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (sub, "Read Sequence Record...", UpdateSeqWithRec);
    SetObjectExtra (i, bfp, NULL);
    if (useEntrez) {
      i = CommandItem (sub, "Download Accession...", UpdateSeqWithAcc);
      SetObjectExtra (i, bfp, NULL);
      mssgupwthaln = RegisterFormMenuItemName ("SequinUpdateWithAlignment");
      FormCommandItem (sub, "Selected Alignment...", bfp, mssgupwthaln);
    }
    if (indexerVersion) {
      SeparatorItem (sub);
      i = CommandItem (sub, "FASTA Set", DoUpdatesSeq);
      SetObjectExtra (i, bfp, NULL);
    }
    SeparatorItem (m);
    i = CommandItem (m, "Feature Propagate...", NewFeaturePropagate);
    SetObjectExtra (i, bfp, NULL);
    SetFormMenuItem (bfp, mssgfeatprop, i);
    SeparatorItem (m);
    sub = SubMenu (m, "Add Sequence");
    SetFormMenuItem (bfp, mssgadd, (IteM) sub);
    i = CommandItem (sub, "Add FASTA File...", AddSeqWithFASTA);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (sub, "Add Sequence Record...", AddSeqWithRec);
    SetObjectExtra (i, bfp, NULL);

    m = PulldownMenu (w, "Search/ R");
    i = CommandItem (m, "Find ASN.1.../ F", FindStringProc);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (m, "Find FlatFile.../ G", FindFlatfileProc);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (m);
    i = CommandItem (m, "Find by Gene...", FindGeneProc);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (m, "Find by Protein...", FindProtProc);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (m, "Find by Position...", FindPosProc);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (m);
    i = CommandItem (m, "Validate.../ V", ValSeqEntryProc);
    SetObjectExtra (i, bfp, NULL);
#ifdef USE_SPELL
#ifdef WIN_MOTIF
    SeparatorItem (m);
    i = CommandItem (m, "Spell Check...", SpellCheckSeqEntryProc);
    SetObjectExtra (i, bfp, NULL);
#endif
#endif
/*#ifdef USE_BLAST*/
    if (useBlast) {
      /*
      SeparatorItem (m);
      i = CommandItem (m, "Power BLAST...", SimplePowerBlastProc);
      SetObjectExtra (i, bfp, NULL);
      */
      SeparatorItem (m);
      i = CommandItem (m, "CDD BLAST...", SimpleCDDBlastProc);
      SetObjectExtra (i, bfp, NULL);
      /*
      i = CommandItem (m, "Vector Screen...", VectorScreenProc);
      SetObjectExtra (i, bfp, NULL);
      */
    }
/*#endif*/
    SeparatorItem (m);
    i = CommandItem (m, "ORF Finder...", FindOrf);
    SetObjectExtra (i, bfp, NULL);
    /*
    i = CommandItem (m, "Repeat Finder...", FindAlu);
    SetObjectExtra (i, bfp, NULL);
    */
    SeparatorItem (m);
    i = CommandItem (m, "Select Target...", DoChangeTarget);
    SetObjectExtra (i, bfp, NULL);

    if (! indexerVersion) {
      m = PulldownMenu (w, "Options");
      sub = SubMenu (m, "Font Selection");
      i = CommandItem (sub, "Display Font...", DisplayFontChangeProc);
      SetObjectExtra (i, bfp, NULL);
      SeparatorItem (m);
      CreateLegendItem (m, bfp);
    }

/*#ifdef EXTRA_SERVICES*/
    if (extraServices) {
      m = PulldownMenu (w, "Special/ S");
      SetupSpecialMenu (m, bfp);
    }
/*#endif*/

    m = PulldownMenu (w, "Misc");
    CommandItem (m, "Style Manager...", StyleManagerProc);
    SeparatorItem (m);
    CommandItem (m, "Net Configure...", NetConfigureProc);
    if (useEntrez) {
      SeparatorItem (m);
      CommandItem (m, "Entrez Query...", EntrezQueryProc);
/*
#ifndef WIN16
      if (BiostrucAvail ()) {
        SeparatorItem (m);
        CommandItem (m, "Cn3D Window...", Cn3DWinShowProc);
      }
#endif
*/
    }
    if (useDesktop) {
      SeparatorItem (m);
      VSMAddToMenu (m, VSM_DESKTOP);
    }

    CreateAnalysisMenu (w, bfp, TRUE, FALSE);

    m = PulldownMenu (w, "Annotate/ A");
    SetupNewFeaturesMenu (m, bfp);
    SeparatorItem (m);
    sub = SubMenu (m, "Publications");
    SetupNewPublicationsMenu (sub, bfp);
    SeparatorItem (m);
    sub = SubMenu (m, "Descriptors");
    SetupNewDescriptorsMenu (sub, bfp);
    SeparatorItem (m);
    i = CommandItem (m, "Generate Definition Line", GenerateAutoDefLinesSmartMods);
    SetObjectExtra (i, bfp, NULL);

    if (indexerVersion) {
      m = PulldownMenu (w, "Options");
      sub = SubMenu (m, "Font Selection");
      i = CommandItem (sub, "Display Font...", DisplayFontChangeProc);
      SetObjectExtra (i, bfp, NULL);
      SeparatorItem (m);
      CreateLegendItem (m, bfp);
    }
  }
}

static void TermListFormMenus (WindoW w)

{
  BaseFormPtr  bfp;
  IteM         i;
  MenU         m;
  MenU         sub;

  bfp = (BaseFormPtr) GetObjectExtra (w);
  if (bfp != NULL) {
    m = PulldownMenu (w, "File");
    AddAboutAndHelpMenuItems (m);
    FormCommandItem (m, "Close", bfp, VIB_MSG_CLOSE);
    SeparatorItem (m);
    if (loadSaveUidListOK) {
      FormCommandItem (m, "Import Uid List...", bfp, VIB_MSG_IMPORT);
      FormCommandItem (m, "Export Uid List...", bfp, VIB_MSG_EXPORT);
      SeparatorItem (m);
      /*
      i = CommandItem (m, "Load Uid List...", ObsoleteUidListProc);
      SetObjectExtra (i, bfp, NULL);
      i = CommandItem (m, "Save Uid List...", ObsoleteUidListProc);
      SetObjectExtra (i, bfp, NULL);
      SeparatorItem (m);
      */
    }
    FormCommandItem (m, "Quit/Q", bfp, VIB_MSG_QUIT);

    m = PulldownMenu (w, "Edit");
    FormCommandItem (m, CUT_MENU_ITEM, bfp, VIB_MSG_CUT);
    FormCommandItem (m, COPY_MENU_ITEM, bfp, VIB_MSG_COPY);
    FormCommandItem (m, PASTE_MENU_ITEM, bfp, VIB_MSG_PASTE);
    FormCommandItem (m, CLEAR_MENU_ITEM, bfp, VIB_MSG_DELETE);

    m = PulldownMenu (w, "Options");
    CommandItem (m, "Preferences...", PreferencesProc);
    SeparatorItem (m);
    sub = SubMenu (m, "Query Style");
    CreateQueryTypeChoice (sub, bfp);
    CreateClearUnusedItem (m, bfp);

    m = PulldownMenu (w, "Misc");
    CommandItem (m, "Style Manager...", StyleManagerProc);
/*
#ifndef WIN16
    if (BiostrucAvail ()) {
      SeparatorItem (m);
      CommandItem (m, "Cn3D Window...", Cn3DWinShowProc);
    }
#endif
*/
    SeparatorItem (m);
    VSMAddToMenu (m, VSM_DESKTOP);
  }
}

static void DocSumFormMenus (WindoW w)

{
  BaseFormPtr  bfp;
  ChoicE       c;
  IteM         i;
  MenU         m;
  MenU         sub;

  bfp = (BaseFormPtr) GetObjectExtra (w);
  if (bfp != NULL) {
    m = PulldownMenu (w, "File");
    FormCommandItem (m, "Close", bfp, VIB_MSG_CLOSE);
    SeparatorItem (m);
    FormCommandItem (m, "Import FASTA or Uid List...", bfp, VIB_MSG_IMPORT);
    FormCommandItem (m, "Export...", bfp, VIB_MSG_EXPORT);
    SeparatorItem (m);
    /*
    FormCommandItem (m, "Save", bfp, VIB_MSG_SAVE);
    FormCommandItem (m, "Save As...", bfp, VIB_MSG_SAVE_AS);
    SeparatorItem (m);
    */
    if (loadSaveUidListOK) {
      /*
      i = CommandItem (m, "Load Uid List...", ObsoleteUidListProc);
      SetObjectExtra (i, bfp, NULL);
      i = CommandItem (m, "Save Uid List...", ObsoleteUidListProc);
      SetObjectExtra (i, bfp, NULL);
      SeparatorItem (m);
      */
    }
    FormCommandItem (m, "Print...", bfp, VIB_MSG_PRINT);

    m = PulldownMenu (w, "Edit");
    FormCommandItem (m, COPY_MENU_ITEM, bfp, VIB_MSG_COPY);

    m = PulldownMenu (w, "Options");
    CommandItem (m, "Preferences...", PreferencesProc);
    SeparatorItem (m);
    sub = SubMenu (m, "Font Selection");
    i = CommandItem (sub, "DocSum Font...", DocSumFontChangeProc);
    SetObjectExtra (i, bfp, NULL);
    /*
    i = CommandItem (sub, "Display Font...", DisplayFontChangeProc);
    SetObjectExtra (i, bfp, NULL);
    */
    SeparatorItem (m);
    sub = SubMenu (m, "Neighbor Policy");
    c = CreateNeighborDelayChoice (sub, bfp);
    SetValue (c, 2); /* UseDelayedNeighbor */
    SeparatorItem (m);
    LoadDocsumOptionsMenu (m);
    seqviewprocs.alignWithChecked = entrezglobals.alignWithChecked;
    seqviewprocs.alignDefault = entrezglobals.alignDefault;

    CreateAnalysisMenu (w, bfp, FALSE, TRUE);
  }
}
#endif

static void GetRidCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  SeqAnnotPtr   nextsap;
  Pointer PNTR  prevsap;
  SeqAnnotPtr   sap;

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

static void GetRidOfEmptyAnnotTables (Uint2 entityID)

{
  SeqEntryPtr  sep;

  if (entityID < 1) return;
  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep == NULL) return;
  SeqEntryExplore (sep, NULL, GetRidCallback);
}

typedef struct findgenedata {
  GeneRefPtr  grp;
  SeqFeatPtr  sfp;
  SeqLocPtr   slp;
  Int4        min;
} FindGeneData, PNTR FindGenePtr;

typedef struct tripletdata {
    ObjMgrPtr  omp;
	Uint2      entityID;
	Uint2      itemID;
	Uint2      itemtype;
	Pointer    lookfor;
    Char       label [81];
} TripletData, PNTR TripletDataPtr;

static Boolean FindOverlappingGeneFunc (GatherContextPtr gcp)

{
  Int4         diff;
  FindGenePtr  fgp;
  GeneRefPtr   grp;
  SeqFeatPtr   sfp;

  if (gcp == NULL) return TRUE;

  fgp = (FindGenePtr) gcp->userdata;
  if (fgp == NULL) return TRUE;
  if (gcp->thistype == OBJ_SEQFEAT) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp != NULL && sfp->data.choice == SEQFEAT_GENE) {
      grp = (GeneRefPtr) sfp->data.value.ptrvalue;
      if (grp != NULL) {
        diff = SeqLocAinB (fgp->slp, sfp->location);
        if (diff >= 0) {
          if (diff < fgp->min) {
            fgp->min = diff;
            fgp->grp = grp;
            fgp->sfp = sfp;
          }
        }
      }
    }
  }
  return TRUE;
}

static Boolean FindTripletItem (GatherContextPtr gcp)

{
  ObjMgrTypePtr   omtp;
  TripletDataPtr  tdp;

  tdp = (TripletDataPtr) gcp->userdata;
  if (tdp != NULL && gcp->thisitem == tdp->lookfor) {
    tdp->entityID = gcp->entityID;
    tdp->itemID = gcp->itemID;
    tdp->itemtype = gcp->thistype;
    omtp = ObjMgrTypeFind (tdp->omp, gcp->thistype, NULL, NULL);
    if (omtp != NULL) {
      if (omtp->labelfunc != NULL) {
        (*(omtp->labelfunc)) (gcp->thisitem, tdp->label, 80, OM_LABEL_CONTENT);
      }
    }
  }
  return TRUE;
}

static CharPtr deleteCdsMsg =
"In addition to deleting the CDS feature, you should probably\n\
delete the protein product, which is a separate data element in\n\
the record.  You will be asked about this in a subsequent message.\n\
Do you wish to continue deleting the CDS?";

static CharPtr deleteProtMsg =
"The protein product of a CDS (shown in the /translation qualifier)\n\
is actually a separate data element in the record.  Unless explicitly\n\
deleted, it will remain hidden in the record after you delete the CDS.\n\
Do you wish to delete the protein product";

static CharPtr deleteGeneMsg =
"The /gene qualifier is generated from an overlapping gene feature.\n\
If you delete a CDS you may also want to delete this separate gene.\n\
Do you wish to delete the overlapping gene";

static CharPtr deleteMrnaMsg =
"In addition to deleting the mRNA feature, you should probably\n\
delete the cDNA and protein products, which are separate data\n\
 elements in the record.  Do you wish to continue deleting the mRNA?";

static CharPtr deleteCdnaMsg =
"The cDNA product of an mRNA is actually a separate data element\n\
in the record.  Unless explicitly deleted, it will remain hidden\n\
in the record after you delete the mRNA.  Do you wish to delete\n\
the cDNA product";

static Boolean DeleteSelectedFeatureOrDescriptor (GatherContextPtr gcp)

{
  MsgAnswer       ans;
  BioseqPtr       bsp;
  BioseqPtr       cdna;
  FindGeneData    fgd;
  Uint2           entityID;
  SeqFeatPtr      gene;
  GatherScope     gs;
  Uint2           itemID;
  Uint2           itemtype;
  SeqFeatPtr      mrna;
  BioseqSetPtr    nps;
  ObjMgrPtr       omp;
  OMProcControl   ompc;
  BioseqSetPtr    parent;
  SeqEntryPtr     scope;
  SeqFeatPtr      sfp;
  SeqIdPtr        sip;
  TripletData     td;
  SeqFeatXrefPtr  xref;
#ifdef USE_SMARTNET
  SeqDescrPtr     sdp;
  UserObjectPtr   uop;
#endif


#ifdef USE_SMARTNET
  /* This code will prevent from deletion SMART User Object */

  if(gcp->thistype == OBJ_SEQDESC) {
      if((sdp = (SeqDescrPtr) gcp->thisitem) != NULL) {
          if(sdp->choice == 14 &&
             ((uop = ( UserObjectPtr) sdp->data.ptrvalue) != NULL)) {
              if(!StringCmp(uop->_class, SMART_OBJECT_LABEL)) {
                  Message(MSG_ERROR, "You may not delete SMART Object Label");
                  return TRUE;
              }
          }
      }
  }
#endif

  sfp = NULL;
  bsp = NULL;
  gene = NULL;
  mrna = NULL;
  cdna = NULL;
  nps = NULL;
  entityID = gcp->entityID;
  itemID = gcp->itemID;
  itemtype = gcp->thistype;
  omp = ObjMgrGet ();
  scope = GetBestTopParentForItemID (entityID, itemID, itemtype);
  if (gcp->thistype == OBJ_SEQFEAT) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp != NULL && sfp->idx.subtype == FEATDEF_mRNA && sfp->product != NULL) {
      mrna = sfp;
      sip = SeqLocId (mrna->product);
      if (sip != NULL) {
        cdna = BioseqFind (sip);
        if (cdna != NULL) {
          if (cdna->idx.parenttype == OBJ_BIOSEQSET) {
            parent = (BioseqSetPtr) cdna->idx.parentptr;
            while (parent != NULL) {
              if (parent->_class == BioseqseqSet_class_nuc_prot) {
                nps = parent;
              }
              if (parent->idx.parenttype == OBJ_BIOSEQSET) {
                parent = (BioseqSetPtr) parent->idx.parentptr;
              } else {
                parent = NULL;
              }
            }
          }
        }
      }
    }
    if (sfp != NULL && sfp->data.choice != SEQFEAT_CDREGION) {
      sfp = NULL;
    }
  }
  if (sfp != NULL) {
    if (sfp->product != NULL) {
      sip = SeqLocId (sfp->product);
      if (sip != NULL) {
        bsp = BioseqFind (sip);
      }
    }
    xref = sfp->xref;
    while (xref != NULL && xref->data.choice != SEQFEAT_GENE) {
      xref = xref->next;
    }
    if (xref == NULL) {
      MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
      gs.seglevels = 1;
      gs.get_feats_location = TRUE;
      MemSet ((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
      gs.ignore[OBJ_BIOSEQ] = FALSE;
      gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
      gs.ignore[OBJ_SEQFEAT] = FALSE;
      gs.ignore[OBJ_SEQANNOT] = FALSE;
      gs.scope = scope;
      fgd.min = INT4_MAX;
      fgd.grp = NULL;
      fgd.sfp = NULL;
      fgd.slp = sfp->location;
      GatherEntity (entityID, (Pointer) &fgd, FindOverlappingGeneFunc, &gs);
      if (fgd.sfp != NULL) {
        gene = fgd.sfp;
      }
    }
  }
  if (sfp != NULL) {
    ans = Message (MSG_OKC, "%s", deleteCdsMsg);
    if (ans == ANS_CANCEL) {
      return TRUE;
    }
  }
  if (nps != NULL || cdna != NULL) {
    ans = Message (MSG_OKC, "%s", deleteMrnaMsg);
    if (ans == ANS_CANCEL) {
      return TRUE;
    }
  }
  MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
  ompc.do_not_reload_from_cache = TRUE;
  ompc.input_entityID = entityID;
  ompc.input_itemID = itemID;
  ompc.input_itemtype = itemtype;
  if (! DetachDataForProc (&ompc, FALSE)) {
    Message (MSG_ERROR, "DetachDataForProc failed");
  }
  if (nps != NULL || cdna != NULL) {
    ans = Message (MSG_YN, "%s", deleteCdnaMsg, td.label);
    if (ans == ANS_YES) {
      if (nps != NULL) {
        nps->idx.deleteme = TRUE;
      } else if (cdna != NULL) {
        cdna->idx.deleteme = TRUE;
      }
    }
  }
  if (bsp != NULL) {
    td.omp = omp;
    td.entityID = 0;
    td.itemID = 0;
    td.itemtype = 0;
    td.lookfor = (Pointer) bsp;
    td.label [0] = '\0';
    MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
    gs.seglevels = 1;
    gs.get_feats_location = TRUE;
    MemSet ((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
    gs.ignore[OBJ_BIOSEQ] = FALSE;
    gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
    gs.ignore[OBJ_SEQFEAT] = FALSE;
    gs.ignore[OBJ_SEQANNOT] = FALSE;
    gs.scope = scope;
    GatherEntity (entityID, (Pointer) (&td), FindTripletItem, &gs);
    if (td.entityID > 0 && td.itemID > 0 && td.itemtype > 0) {
      ans = Message (MSG_YN, "%s (%s)?", deleteProtMsg, td.label);
      if (ans == ANS_YES) {
        MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
        ompc.do_not_reload_from_cache = TRUE;
        ompc.input_entityID = td.entityID;
        ompc.input_itemID = td.itemID;
        ompc.input_itemtype = td.itemtype;
        if (! DetachDataForProc (&ompc, FALSE)) {
          Message (MSG_ERROR, "DetachDataForProc failed");
        }
      }
    }
  }
  if (gene != NULL) {
    td.omp = omp;
    td.entityID = 0;
    td.itemID = 0;
    td.itemtype = 0;
    td.lookfor = (Pointer) gene;
    td.label [0] = '\0';
    MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
    gs.seglevels = 1;
    gs.get_feats_location = TRUE;
    MemSet ((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
    gs.ignore[OBJ_BIOSEQ] = FALSE;
    gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
    gs.ignore[OBJ_SEQFEAT] = FALSE;
    gs.ignore[OBJ_SEQANNOT] = FALSE;
    gs.scope = scope;
    GatherEntity (entityID, (Pointer) (&td), FindTripletItem, &gs);
    if (td.entityID > 0 && td.itemID > 0 && td.itemtype > 0) {
      ans = Message (MSG_YN, "%s (%s)?", deleteGeneMsg, td.label);
      if (ans == ANS_YES) {
        MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
        ompc.do_not_reload_from_cache = TRUE;
        ompc.input_entityID = td.entityID;
        ompc.input_itemID = td.itemID;
        ompc.input_itemtype = td.itemtype;
        if (! DetachDataForProc (&ompc, FALSE)) {
          Message (MSG_ERROR, "DetachDataForProc failed");
        }
      }
    }
  }
  return TRUE;
}

static void TermSelectionFormMessage (ForM f, Int2 mssg)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        Hide (f);
        break;
      case VIB_MSG_QUIT :
        QuitProc ();
        break;
      default :
        break;
    }
  }
}

static void DocumentSummaryFormMessage (ForM f, Int2 mssg)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    switch (mssg) {
      case VIB_MSG_CHANGE :
        EnableAnalysisItems (bfp, TRUE);
        break;
      case VIB_MSG_CLOSE :
        Hide (f);
        break;
      case VIB_MSG_QUIT :
        QuitProc ();
        break;
      default :
        break;
    }
  }
}

static void SequinMedlineFormMessage (ForM f, Int2 mssg)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        Remove (f);
        break;
      case VIB_MSG_QUIT :
        QuitProc ();
        break;
      default :
        break;
    }
  }
}

static Boolean UpdateWithAln (GatherContextPtr gcp)

{
  SeqAlignPtr   align;
  BaseFormPtr   bfp;
  BioseqPtr     bsp;
  DenseDiagPtr  ddp;
  DenseSegPtr   dsp;
  Uint2         entityID;
  Uint2         itemID;
  SeqEntryPtr   sep;
  SeqIdPtr      sip;
  SeqLocPtr     slp;
  StdSegPtr     ssp;
/* SeqLocPtr     tloc; */
  ValNode       vn;

  if (gcp == NULL || gcp->thisitem == NULL) {
    Beep ();
    return FALSE;
  }
  bfp = (BaseFormPtr) gcp->userdata;
  if (bfp == NULL) return FALSE;
  bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (bsp == NULL) return FALSE;
  updateTargetBspKludge = bsp;
  vn.choice = SEQLOC_WHOLE;
  vn.data.ptrvalue = (Pointer) bsp->id;
  slp = &vn;
  switch (gcp->thistype) {
    case OBJ_SEQALIGN :
    case OBJ_SEQHIST_ALIGN :
      align = (SeqAlignPtr) gcp->thisitem;
      sip = NULL;
      if (align->segtype == 1) {
        ddp = (DenseDiagPtr) align->segs;
        if (ddp != NULL) {
          for (sip = ddp->id; sip != NULL; sip = sip->next) {
            if (! SeqIdForSameBioseq (sip, SeqLocId (slp)))
              break;
          }
        }
      } else if (align->segtype == 2) {
        dsp = (DenseSegPtr) align->segs;
        if (dsp != NULL) {
          if (dsp->ids != NULL) {
            sip = dsp->ids->next;
          }
          /*
          for (sip = dsp->ids; sip != NULL; sip = sip->next) {
            if (! SeqIdForSameBioseq (sip, SeqLocId (slp)))
              break;
          }
          */
        }
      } else if (align->segtype == 3) {
        ssp = (StdSegPtr) align->segs;
        if (ssp != NULL && ssp->loc != NULL) {
          if (ssp->loc->next != NULL) {
            sip = SeqLocId (ssp->loc->next);
          }
          /*
          for (tloc = ssp->loc; tloc != NULL; tloc = tloc->next) {
            if (! SeqIdForSameBioseq (SeqLocId (tloc), SeqLocId (slp))) {
              sip = SeqLocId (tloc);
              break;
            }
          }
          */
        }
      }
      if (sip != NULL) {
        bsp = BioseqLockById (sip);
        if (bsp != NULL) {
          entityID = BioseqFindEntity (sip, &itemID);
          sep = GetBestTopParentForData (entityID, bsp);
          if (sep != NULL) {
            /*
            SqnReadAlignViewEx (updateTargetBspKludge, sep, PRGALIGNALL);
            */
            SqnReadAlignView (updateTargetBspKludge, sep);
          }
          BioseqUnlockById (sip);
        }
      }
      return TRUE;
    default :
      break;
  }
  return FALSE;
}

typedef struct seltbl {
  size_t             count;
  SelStructPtr PNTR  selarray;
  Boolean            geneasked;
  Boolean            productasked;
  Boolean            removegene;
  Boolean            removeproduct;
} SelTbl, PNTR SelTblPtr;

static Boolean MarkSelectedItem (GatherObjectPtr gop)

{
  Int2           L, R, mid;
  SelStructPtr   ssp;
  SelTblPtr      tbl;
  MsgAnswer      ans;
  BioseqPtr      bsp;
  ObjValNodePtr  ovn;
  SeqAlignPtr    sap;
  SeqDescrPtr    sdp;
  SeqFeatPtr     sfp, gene;
  SeqGraphPtr    sgp;

  if (gop == NULL) return TRUE;
  tbl = (SelTblPtr) gop->userdata;
  if (tbl == NULL) return TRUE;

  L = 0;
  R = tbl->count - 1;
  while (L <= R) {
    mid = (L + R) / 2;
    ssp = tbl->selarray [mid];
    if (ssp == NULL) return TRUE;
    if (ssp->entityID > gop->entityID) {
      R = mid - 1;
    } else if (ssp->entityID < gop->entityID) {
      L = mid + 1;
    } else if (ssp->itemtype > gop->itemtype) {
      R = mid - 1;
    } else if (ssp->itemtype < gop->itemtype) {
      L = mid + 1;
    } else if (ssp->itemID > gop->itemID) {
      R = mid - 1;
    } else if (ssp->itemID < gop->itemID) {
      L = mid + 1;
    } else if (gop->dataptr != NULL &&
               ssp->entityID == gop->entityID &&
               ssp->itemtype == gop->itemtype &&
               ssp->itemID == gop->itemID) {
      switch (gop->itemtype) {
        case OBJ_SEQDESC :
          sdp = (SeqDescrPtr) gop->dataptr;
          if (sdp != NULL && sdp->extended != 0) {
            ovn = (ObjValNodePtr) sdp;
            ovn->idx.deleteme = TRUE;
          }
          break;
        case OBJ_SEQFEAT :
          sfp = (SeqFeatPtr) gop->dataptr;
          if (sfp != NULL) {
            sfp->idx.deleteme = TRUE;
            if (SeqMgrGetGeneXref (sfp) == NULL) {
              gene = SeqMgrGetOverlappingGene (sfp->location, NULL);
              if (gene != NULL) {
                if (! tbl->geneasked) {
                  ans = Message (MSG_YN, "Remove overlapping gene?");
                  if (ans == ANS_YES) {
                    tbl->removegene = TRUE;
                  }
                  tbl->geneasked = TRUE;
                }
                if (tbl->removegene) {
                  gene->idx.deleteme = TRUE;
                }
              }
            }
            if (sfp->data.choice == SEQFEAT_CDREGION) {
              bsp = BioseqFind (SeqLocId (sfp->product));
              if (bsp != NULL) {
                if (! tbl->productasked) {
                  ans = Message (MSG_YN, "Remove protein products?");
                  if (ans == ANS_YES) {
                    tbl->removeproduct = TRUE;
                  }
                  tbl->productasked = TRUE;
                }
                if (tbl->removeproduct) {
                  bsp->idx.deleteme = TRUE;
                }
              }
            }
          }
          break;
        case OBJ_SEQALIGN :
          sap = (SeqAlignPtr) gop->dataptr;
          if (sap != NULL) {
            sap->idx.deleteme = TRUE;
          }
          break;
        case OBJ_SEQGRAPH :
          sgp = (SeqGraphPtr) gop->dataptr;
          if (sgp != NULL) {
            sgp->idx.deleteme = TRUE;
          }
          break;
        default :
          break;
      }
      return TRUE;
    }
  }

  return TRUE;
}

static int LIBCALLBACK SortSelStructByIDs (VoidPtr ptr1, VoidPtr ptr2)

{
  SelStructPtr  ssp1, ssp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  ssp1 = *((SelStructPtr PNTR) ptr1);
  ssp2 = *((SelStructPtr PNTR) ptr2);
  if (ssp1 == NULL || ssp2 == NULL) return 0;

  if (ssp1->entityID > ssp2->entityID) return 1;
  if (ssp1->entityID < ssp2->entityID) return -1;

  if (ssp1->itemtype > ssp2->itemtype) return 1;
  if (ssp1->itemtype < ssp2->itemtype) return -1;

  if (ssp1->itemID > ssp2->itemID) return 1;
  if (ssp1->itemID < ssp2->itemID) return -1;

  return 0;
}

static void DeleteMultipleSelections (Uint2 entityID, SelStructPtr selhead)

{
  size_t             count;
  SelStructPtr       sel;
  SelStructPtr PNTR  selarray;
  SelTbl             tbl;

  if (entityID < 1 || selhead == NULL) return;

  for (count = 0, sel = selhead; sel != NULL; sel = sel->next, count++) continue;
  if (count < 1) return;

  selarray = (SelStructPtr PNTR) MemNew (sizeof (SelStructPtr) * (size_t) (count + 1));
  if (selarray == NULL) return;
  for (count = 0, sel = selhead; sel != NULL; sel = sel->next, count++) {
    selarray [count] = sel;
  }

  HeapSort (selarray, (size_t) count, sizeof (SelStructPtr), SortSelStructByIDs);

  MemSet ((Pointer) &tbl, 0, sizeof (tbl));
  tbl.count = count;
  tbl.selarray = selarray;

  GatherObjectsInEntity (entityID, 0, NULL, MarkSelectedItem, (Pointer) &tbl, NULL);

  MemFree (selarray);
  DeleteMarkedObjects (entityID, 0, NULL);
}

static void SequinSeqViewFormMessage (ForM f, Int2 mssg)

{
  MsgAnswer     ans;
  BaseFormPtr   bfp;
  BioseqPtr     bsp;
  Uint2         entityID;
  Uint2         itemID;
  Int2          mssgalign;
  Int2          mssgdup;
  Int2          mssgseq;
  Int2          mssgsub;
  Int2          mssgupwthaln;
  SeqEntryPtr   oldsep;
  OMProcControl  ompc;
/******** COLOMBE
  SeqAlignPtr   salp; ********COLOMBE END*/
  SeqAnnotPtr   sap;
  SelStructPtr  sel;
  SeqEntryPtr   sep;
  SeqIdPtr      sip;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    switch (mssg) {
      case VIB_MSG_SAVE :
        SaveSeqSubmitProc (bfp, FALSE);
        break;
      case VIB_MSG_SAVE_AS :
        SaveSeqSubmitProc (bfp, TRUE);
        break;
      case VIB_MSG_CLOSE :
        CloseProc (bfp);
        break;
      case VIB_MSG_QUIT :
        QuitProc ();
        break;
      case VIB_MSG_ACCEPT :
        ProcessDoneButton (f);
        break;
      case VIB_MSG_RESET :
#ifdef USE_SMARTNET
        SmartResetProc ((IteM)f);
#endif
        break;
      case VIB_MSG_IMPORT :
        /* ReadFastaProc (); */
        break;
      case VIB_MSG_CUT :
        break;
      case VIB_MSG_COPY :
        break;
      case VIB_MSG_UNDO :
        if (subtoolMode || smartnetMode) {
          if (FileLength (SEQUIN_EDIT_PREV_FILE) > 0) {
            sep = GetTopSeqEntryForEntityID (subtoolEntityID);
            if (Message (MSG_YN, "Restore from backup?") == ANS_YES) {
              oldsep = RestoreFromFile (SEQUIN_EDIT_PREV_FILE);
              ReplaceSeqEntryWithSeqEntry (sep, oldsep, TRUE);
              subtoolEntityID = ObjMgrGetEntityIDForChoice (sep);
              ObjMgrSetDirtyFlag (subtoolEntityID, TRUE);
              ObjMgrSendMsg (OM_MSG_UPDATE, subtoolEntityID, 0, 0);
            }
          }
        }
        break;
      case VIB_MSG_PASTE :
        break;
      case VIB_MSG_DELETE :
        sel = ObjMgrGetSelected ();
        if (sel != NULL) {
          if (sel->itemtype == OBJ_BIOSEQ && sel->next == NULL) {
            if (! indexerVersion) {
              ans = Message (MSG_OKC, "Are you sure you want to delete this Bioseq?");
              if (ans == ANS_CANCEL) return;
              ans = Message (MSG_OKC, "Are you REALLY sure you want to delete this Bioseq?");
              if (ans == ANS_CANCEL) return;
            }
            bsp = GetBioseqGivenIDs (sel->entityID, sel->itemID, sel->itemtype);
            sip = SeqIdDup (bsp->id);
            entityID = sel->entityID;
            MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
            ompc.do_not_reload_from_cache = TRUE;
            ompc.input_entityID = sel->entityID;
            ompc.input_itemID = sel->itemID;
            ompc.input_itemtype = sel->itemtype;
            if (! DetachDataForProc (&ompc, FALSE)) {
              Message (MSG_ERROR, "DetachDataForProc failed");
            }
            sep = GetTopSeqEntryForEntityID (entityID);
            SeqAlignBioseqDeleteByIdFromSeqEntry (sep, sip);
            SeqIdFree (sip);
            /* see VSeqMgrDeleteProc - probably leaving one dangling SeqEntry */
            BioseqFree (bsp);
            ObjMgrSetDirtyFlag (entityID, TRUE);
            ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
            ObjMgrDeSelect (0, 0, 0, 0, NULL);
            Update ();
          }
          if (sel->itemtype == OBJ_SEQANNOT && sel->next == NULL && extraServices) {
            entityID = sel->entityID;
            MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
            ompc.do_not_reload_from_cache = TRUE;
            ompc.input_entityID = sel->entityID;
            ompc.input_itemID = sel->itemID;
            ompc.input_itemtype = sel->itemtype;
            if (! DetachDataForProc (&ompc, FALSE)) {
              Message (MSG_ERROR, "DetachDataForProc failed");
            }
            ObjMgrSetDirtyFlag (entityID, TRUE);
            ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
            ObjMgrDeSelect (0, 0, 0, 0, NULL);
            Update ();
          }
          if (sel->next == NULL && (sel->itemtype == OBJ_SEQDESC || sel->itemtype == OBJ_SEQFEAT)) {
            entityID = sel->entityID;
            GatherItem (sel->entityID, sel->itemID, sel->itemtype,
                        NULL, DeleteSelectedFeatureOrDescriptor);
            DeleteMarkedObjects (entityID, 0, NULL);
            GetRidOfEmptyAnnotTables (entityID);
            sep = GetTopSeqEntryForEntityID (entityID);
            RenormalizeNucProtSets (sep, TRUE);
            ObjMgrSetDirtyFlag (entityID, TRUE);
            ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
            ObjMgrDeSelect (0, 0, 0, 0, NULL);
            Update ();
          } else {
            /* Message (MSG_OK, "Unable to delete multiple objects"); */
            ans = Message (MSG_YN, "Are you sure you want to delete multiple objects?");
            if (ans == ANS_NO) return;
            entityID = bfp->input_entityID;
            DeleteMultipleSelections (entityID, sel);
            GetRidOfEmptyAnnotTables (entityID);
            sep = GetTopSeqEntryForEntityID (entityID);
            RenormalizeNucProtSets (sep, TRUE);
            ObjMgrSetDirtyFlag (entityID, TRUE);
            ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
            ObjMgrDeSelect (0, 0, 0, 0, NULL);
          }
        } else {
          Message (MSG_OK, "Nothing selected");
        }
        break;
      case VIB_MSG_CHANGE :
        EnableFeaturesPerTarget (bfp);
        EnableAnalysisItems (bfp, FALSE);
        EnableEditSeqAlignAndSubItems (bfp);
        break;
      case VIB_MSG_SELECT :
        EnableEditAlignItem (bfp);
        break;
      default :
        mssgseq = RegisterFormMenuItemName ("SequinEditSequenceItem");
        mssgalign = RegisterFormMenuItemName ("SequinEditAlignmentItem");
        mssgsub = RegisterFormMenuItemName ("SequinEditSubmitterItem");
        mssgdup = RegisterFormMenuItemName ("SequinDuplicateItem");
        mssgupwthaln = RegisterFormMenuItemName ("SequinUpdateWithAlignment");
        if (mssg == mssgseq) {
          bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
          if (bsp != NULL) {
            WatchCursor ();
            Update ();
            SeqEntrySetScope (NULL);
            GatherProcLaunch (OMPROC_EDIT, FALSE, bfp->input_entityID, bfp->input_itemID,
                              bfp->input_itemtype, 0, 0, bfp->input_itemtype, 0);
            ArrowCursor ();
            Update ();
          }
        } else if (mssg == mssgalign) {
          sel = ObjMgrGetSelected ();
          if (sel != NULL &&
              (sel->itemtype == OBJ_SEQALIGN || sel->itemtype == OBJ_SEQHIST_ALIGN)) {
            WatchCursor ();
            Update ();
            SeqEntrySetScope (NULL);
            GatherProcLaunch (OMPROC_EDIT, FALSE, sel->entityID, sel->itemID,
                              sel->itemtype, 0, 0, sel->itemtype, 0);
            ArrowCursor ();
            Update ();
          } else /* if (sel == NULL) */ {
            sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
/******** COLOMBE
            salp = FindSeqAlignInSeqEntry (sep, OBJ_SEQALIGN);
            if (salp != NULL) {
              itemID = GetItemIDGivenPointer (bfp->input_entityID, OBJ_SEQALIGN, (Pointer) salp);
              if (itemID != 0) {
                WatchCursor ();
                Update ();
                GatherProcLaunch (OMPROC_EDIT, FALSE, bfp->input_entityID, itemID,
                                  OBJ_SEQALIGN, 0, 0, OBJ_SEQALIGN, 0);
                ArrowCursor ();
                Update ();
              }
            }
COLOMBE END *******************/
            sap  = (SeqAnnotPtr)FindSeqAlignInSeqEntry (sep, OBJ_SEQANNOT);
            if (sap) {
              itemID = GetItemIDGivenPointer (bfp->input_entityID, OBJ_SEQANNOT, (Pointer)sap);
              if (itemID != 0) {
                WatchCursor ();
                Update ();
                SeqEntrySetScope (NULL);
                GatherProcLaunch (OMPROC_EDIT, FALSE, bfp->input_entityID, itemID, OBJ_SEQANNOT, 0, 0, OBJ_SEQANNOT, 0);
                ArrowCursor ();
                Update ();
              }
            }
          }
        } else if (mssg == mssgsub) {
          EditSubmitBlock (bfp);
        } else if (mssg == mssgdup) {
          sel = ObjMgrGetSelected ();
          if (sel != NULL &&
              (sel->itemtype == OBJ_SEQDESC || sel->itemtype == OBJ_SEQFEAT)) {
            stdedprocs.duplicateExisting = TRUE;
            WatchCursor ();
            Update ();
            SeqEntrySetScope (NULL);
            GatherProcLaunch (OMPROC_EDIT, FALSE, sel->entityID, sel->itemID,
                              sel->itemtype, 0, 0, sel->itemtype, 0);
            ArrowCursor ();
            Update ();
            stdedprocs.duplicateExisting = FALSE;
          }
        } else if (mssg == mssgupwthaln) {
          sel = ObjMgrGetSelected ();
          if (sel != NULL &&
              (sel->itemtype == OBJ_SEQALIGN || sel->itemtype == OBJ_SEQHIST_ALIGN)) {
            SeqEntrySetScope (NULL);
            GatherItem (sel->entityID, sel->itemID, sel->itemtype, (Pointer) bfp, UpdateWithAln);
          }
        }
        break;
    }
  }
}

static void SequinSeqEditFormMessage (ForM f, Int2 mssg)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    switch (mssg) {
      case VIB_MSG_QUIT :
        QuitProc ();
        break;
      default :
        break;
    }
  }
}

/* from salfiles.c */
static Int4 AccessionToGi (CharPtr string, CharPtr program)
{
   CharPtr str;
   LinkSetPtr lsp;
   Int4 gi;

   EntrezInit (program, TRUE, NULL);
   str = MemNew (StringLen (string) + 10);
   sprintf (str, "\"%s\" [ACCN]", string);
   lsp = EntrezTLEvalString (str, TYP_NT, -1, NULL, NULL);
   MemFree (str);
   if (lsp == NULL)
       return 0;
   if (lsp->num <= 0) {
       LinkSetFree (lsp);
       return 0;
   }
   gi = lsp->uids [0];
   LinkSetFree (lsp);
   EntrezFini ();
   return gi;
}

static SeqEntryPtr LIBCALLBACK SeqEdDownload (CharPtr program, CharPtr accession,
                                              Int4 uid, Boolean is_na, BoolPtr is_new)

{
  BioseqPtr    bsp;
  LinkSetPtr   lsp;
  SeqEntryPtr  sep = NULL;
  Int2         seqtype;
  SeqId        sid;
  Char         str [64];

  if (is_new != NULL) {
     *is_new = TRUE;
  }
/*********/
  if (uid==0 && ! StringHasNoText (accession))
    uid = AccessionToGi (accession, program);
  if (uid > 0) {
    sid.choice = SEQID_GI;
    sid.data.intvalue = uid;
    sid.next = NULL;
    bsp = BioseqFind (&sid);
    if (bsp) {
      sep = SeqMgrGetSeqEntryForData (bsp);
    }
  }
  if (sep) {
    if (is_new != NULL) {
      *is_new = FALSE;
    }
    return sep;
  }
/********************/
  EntrezInit (program, TRUE, NULL);
  if (uid==0 && ! StringHasNoText (accession)) {
    if (is_na) {
      seqtype = TYP_NT;
    } else {
      seqtype = TYP_AA;
    }
    sprintf (str, "\"%s\" [ACCN]", accession);
    lsp = EntrezTLEvalString (str, seqtype, -1, NULL, NULL);
    if (lsp != NULL) {
      uid = lsp->uids [0];
    }
    LinkSetFree (lsp);
  }
  if (uid > 0) {
    sid.choice = SEQID_GI;
    sid.data.intvalue = uid;
    sid.next = NULL;
    bsp = BioseqLockById (&sid);
    if (bsp != NULL) {
      sep = SeqMgrGetSeqEntryForData (bsp);
      BioseqUnlock (bsp);
    } else {
      sep = EntrezSeqEntryGet (uid, 0);
      if (is_new != NULL) {
        *is_new = TRUE;
      }
    }
  }
  EntrezFini ();
  return sep;
}

static void SequinStdEditorFormMessage (ForM f, Int2 mssg)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        Remove (f);
        break;
      case VIB_MSG_QUIT :
        QuitProc ();
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
        break;
    }
  }
}

static void ProcessHelpMessage (CharPtr heading, CharPtr section)

{
  SendHelpScrollMessage (helpForm, heading, section);
}

static void MakeTermListForm (void)

{
  if (! EntrezIsInited ()) {
    /* EntrezBioseqFetchEnable ("Sequin", TRUE); */
    SequinEntrezInit ("Sequin", FALSE, NULL);
  }
  if (termListForm != NULL) return;
  termListForm = CreateTermListForm (-50, -33, "Query",
                                     TermSelectionActivateProc,
                                     TermSelectionFormMessage);
}

static void MakeDocSumForm (void)

{
  if (! EntrezIsInited ()) {
    /* EntrezBioseqFetchEnable ("Sequin", TRUE); */
    SequinEntrezInit ("Sequin", FALSE, NULL);
  }
  if (docSumForm != NULL) return;
  docSumForm = CreateDocSumForm (-10, -90, "Document",
                                 DocumentSummaryActivateProc,
                                 DocumentSummaryFormMessage);
  /* if (indexerVersion) { */
  UseDelayedNeighbor (docSumForm, TRUE);
  /* } */
}

void EntrezQueryProc (IteM i)

{
  MakeTermListForm ();
  MakeDocSumForm ();
  Show (termListForm);
  Select (termListForm);
  Update ();
}

/*
*  The following callbacks process requests between forms.  This can be extended
*  to have the application track multiple term list and docsum windows.
*/

static void DoRetrieveDocuments (ForM f, Int2 num, Int2 parents, Int4Ptr uids, Int2 db)

{
  if (useEntrez) {
    MakeTermListForm ();
    MakeDocSumForm ();
  }
  RetrieveDocuments (docSumForm, num, parents, uids, db);
}

static void DoRetrieveProject (ForM f, Pointer proj)

{
  if (useEntrez) {
    MakeTermListForm ();
    MakeDocSumForm ();
  }
  PointerToForm (docSumForm, proj);
}

static void DoRetrieveSimple (ForM f, ValNodePtr simpleSeqs)

{
  if (useEntrez) {
    MakeTermListForm ();
    MakeDocSumForm ();
  }
  RetrieveSimpleSeqs (docSumForm, simpleSeqs);
}

static void DoLoadNamedUidList (ForM f, CharPtr term, Int4 num, Int4Ptr uids, Int2 db)

{
  if (useEntrez) {
    MakeTermListForm ();
    MakeDocSumForm ();
  }
  LoadNamedUidList (termListForm, term, num, uids, db);
}

static void DoLaunchRecordViewer (ForM f, Int4 uid, Int2 numAlign, Int4Ptr alignuids, Int2 db)

{
  if (useEntrez) {
    MakeTermListForm ();
    MakeDocSumForm ();
  }
  LaunchRecordViewer (uid, numAlign, alignuids, db);
}

static GrouP DoMakeMedViewerLinkControls (GrouP prnt, BaseFormPtr bfp, Int2 doctype, Int4 uid)

{
  if (useEntrez) {
    if (! EntrezIsInited ()) {
      /* EntrezBioseqFetchEnable ("Sequin", TRUE); */
      SequinEntrezInit ("Sequin", FALSE, NULL);
    }
    return MakeViewerLinkControls (prnt, bfp, doctype, uid, FALSE);
  }
  return NULL;
}

static GrouP DoMakeSeqViewerLinkControls (GrouP prnt, BaseFormPtr bfp, Int2 doctype, Int4 uid)

{
  if (useEntrez) {
    if (! EntrezIsInited ()) {
      /* EntrezBioseqFetchEnable ("Sequin", TRUE); */
      SequinEntrezInit ("Sequin", FALSE, NULL);
    }
    return MakeViewerLinkControls (prnt, bfp, doctype, uid, TRUE);
  }
  return NULL;
}

typedef struct aligngroup {
  DIALOG_MESSAGE_BLOCK
  BaseFormPtr        bfp;
  ButtoN             retrieve;
  PopuP              target;
  Int2               targetDb;
  Uint2              align_type;
  ButtoN             onlyFromThis;
} AlignGroup, PNTR AlignGroupPtr;

static void RetrieveAligns (ButtoN b)

{
  AlignGroupPtr     agp;
  BaseFormPtr       bfp;
  BioseqPtr         bsp;
  EntrezGlobalsPtr  egp;
  ValNodePtr        head;
  Int2              i;
  Int4              num;
  SeqEntryPtr       sep;
  Int4Ptr           uids;
  ValNodePtr        vnp;

  agp = (AlignGroupPtr) GetObjectExtra (b);
  if (agp == NULL) return;
  bfp = agp->bfp;
  if (bfp == NULL) return;
  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL || egp->retrieveDocsProc == NULL) return;
  if (! EntrezIsInited ()) {
    SequinEntrezInit ("Sequin", FALSE, NULL);
  }
  uids = NULL;
  sep = NULL;
  if (GetStatus (agp->onlyFromThis)) {
    bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
    if (bsp != NULL) {
      sep = SeqMgrGetSeqEntryForData (bsp);
    }
  } else {
    sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  }
  head = GetUidsForSeqEntryAligns (sep);
  num = 0;
  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == agp->align_type) {
      num++;
    }
  }
  if (num > 0) {
    uids = MemNew ((size_t) (num + 1) * sizeof (DocUid));
    if (uids != NULL) {
      i = 0;
      for (vnp = head; i < num && vnp != NULL; vnp = vnp->next) {
        if (vnp->choice == agp->align_type) {
          uids [i] = vnp->data.intvalue;
          i++;
        }
      }
    }
    egp->retrieveDocsProc (bfp->form, num, 0, uids, agp->targetDb);
    MemFree (uids);
  }
  ValNodeFree (head);
}

static Boolean DoUpdateFetchCounts (GrouP g, SeqEntryPtr sep)

{
  AlignGroupPtr  agp;
  BaseFormPtr    bfp;
  BioseqPtr      bsp;
  ValNodePtr     head;
  Int4           num;
  Int4           rsult;
  Char           tmp [32];
  Int2           val;
  ValNodePtr     vnp;

  agp = (AlignGroupPtr) GetObjectExtra (g);
  if (agp == NULL) return FALSE;
  bfp = agp->bfp;
  if (bfp == NULL) return FALSE;
  val = GetValue (agp->target);
  num = 0;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  head = GetIdStringsForSeqEntryAligns (sep);
  rsult = ValNodeLen (head);
  if (GetStatus (agp->onlyFromThis)) {
    head = ValNodeFreeData (head);
    bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
    if (bsp != NULL) {
      sep = SeqMgrGetSeqEntryForData (bsp);
      head = GetIdStringsForSeqEntryAligns (sep);
    }
  }
  agp->align_type = 0;
  agp->targetDb = TYP_NT;
  val = GetValue (agp->target);
  switch (val) {
    case 1 :
      agp->align_type = ALIGN_BLASTN;
      agp->targetDb = TYP_NT;
      break;
    case 2 :
      agp->align_type = ALIGN_BLASTP;
      agp->targetDb = TYP_AA;
      break;
    case 3 :
      agp->align_type = ALIGN_BLASTX;
      agp->targetDb = TYP_AA;
      break;
    case 4 :
      agp->align_type = ALIGN_TBLASTN;
      agp->targetDb = TYP_NT;
      break;
    default :
      break;
  }
  num = 0;
  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == agp->align_type) {
      num++;
    }
  }
  ValNodeFreeData (head);
  sprintf (tmp, "Retrieve %ld", (long) num);
  SafeSetTitle (agp->retrieve, tmp);
  if (num > 0) {
    SafeEnable (agp->retrieve);
  } else {
    SafeDisable (agp->retrieve);
  }
  return (Boolean) (rsult > 0);
}

static void ChangeAlignTarget (PopuP p)

{
  AlignGroupPtr  agp;
  BaseFormPtr    bfp;
  SeqEntryPtr    sep;

  agp = (AlignGroupPtr) GetObjectExtra (p);
  if (agp == NULL) return;
  bfp = agp->bfp;
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  DoUpdateFetchCounts ((GrouP) agp->dialog, sep);
}

static GrouP DoMakeViewerAlignBtn (GrouP prnt, BaseFormPtr bfp)

{
  AlignGroupPtr     agp;
  EntrezGlobalsPtr  egp;
  GrouP             g;
  Boolean           macLike;
  PrompT            ppt;

  if (useEntrez) {
    /*
    if (! EntrezIsInited ()) {
      SequinEntrezInit ("Sequin", FALSE, NULL);
    }
    */
    egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
    if (egp == NULL) return NULL;
    macLike = egp->popdownBehavior;

    agp = (AlignGroupPtr) MemNew (sizeof (AlignGroup));
    if (agp == NULL) return NULL;

    g = HiddenGroup (prnt, 5, 0, NULL);
    SetGroupSpacing (g, 10, 10);
    SetObjectExtra (g, agp, StdCleanupExtraProc);

    agp->dialog = (DialoG) g;
    agp->bfp = bfp;
    agp->retrieve = PushButton (g, "Retrieve 00000", RetrieveAligns);
    SetObjectExtra (agp->retrieve, agp, NULL);
    SetTitle (agp->retrieve, "Retrieve 0");
    SafeDisable (agp->retrieve);

    ppt = StaticPrompt (g, "Alignment:", 0, popupMenuHeight, programFont, 'l');
    agp->target = PopupList (g, macLike, (PupActnProc) ChangeAlignTarget);
    SetObjectExtra (agp->target, agp, NULL);
    PopupItem (agp->target, "BLASTN");
    PopupItem (agp->target, "BLASTP");
    PopupItem (agp->target, "BLASTX");
    PopupItem (agp->target, "TBLASTN");
    SetValue (agp->target, 1);

    agp->onlyFromThis = CheckBox (g, "Just from this sequence", (BtnActnProc) ChangeAlignTarget);
    SetObjectExtra (agp->onlyFromThis, agp, NULL);
    SetStatus (agp->onlyFromThis, TRUE);
    SafeHide (agp->onlyFromThis);

    AlignObjects (ALIGN_MIDDLE, (HANDLE) agp->retrieve, (HANDLE) ppt,
                  (HANDLE) agp->target, (HANDLE) agp->onlyFromThis, NULL);

    return g;
  }
  return NULL;
}

extern void SetupBioseqPageList (void)

{
  Char  str [32];

  seqviewprocs.pageSpecs = BioseqPageListFree (seqviewprocs.pageSpecs);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &mapPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &sumPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &gphPageData);
  if (useNewGraphicView) {
    AddBioseqPageToList (&(seqviewprocs.pageSpecs), &asn2gphGphPageData);
  }
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &alnPageData);
  /*
  if (useUdv) {
    AddBioseqPageToList (&(seqviewprocs.pageSpecs), &udvPageData);
    AddBioseqPageToList (&(seqviewprocs.pageSpecs), &ddvPageData);
  } else {
    AddBioseqPageToList (&(seqviewprocs.pageSpecs), &seqPageData);
  }
  */
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &seqPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &gbgnPageData);
  if (Nlm_GetAppProperty ("SequinUseEMBLStyle") != NULL) {
    AddBioseqPageToList (&(seqviewprocs.pageSpecs), &emblPageData);
    AddBioseqPageToList (&(seqviewprocs.pageSpecs), &gnbkPageData);
    if (GetSequinAppParam ("SETTINGS", "NUCPAGE", "EMBL", str, sizeof (str))) {
      seqviewprocs.initNucLabel = MemFree (seqviewprocs.initNucLabel);
      seqviewprocs.initNucLabel = StringSaveNoNull (str);
    }
  } else if (Nlm_GetAppProperty ("SequinUseDDBJStyle") != NULL) {
    AddBioseqPageToList (&(seqviewprocs.pageSpecs), &ddbjPageData);
    AddBioseqPageToList (&(seqviewprocs.pageSpecs), &emblPageData);
    if (GetSequinAppParam ("SETTINGS", "NUCPAGE", "DDBJ", str, sizeof (str))) {
      seqviewprocs.initNucLabel = MemFree (seqviewprocs.initNucLabel);
      seqviewprocs.initNucLabel = StringSaveNoNull (str);
    }
  } else {
    AddBioseqPageToList (&(seqviewprocs.pageSpecs), &gnbkPageData);
    AddBioseqPageToList (&(seqviewprocs.pageSpecs), &emblPageData);
    if (GetSequinAppParam ("SETTINGS", "NUCPAGE", "GenBank", str, sizeof (str))) {
      seqviewprocs.initNucLabel = MemFree (seqviewprocs.initNucLabel);
      seqviewprocs.initNucLabel = StringSaveNoNull (str);
    }
  }
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &gnptPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &fstaPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &qualPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &asnPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &xmlPageData);

  if (extraServices || useDesktop) {
    AddBioseqPageToList (&(seqviewprocs.pageSpecs), &dskPageData);
  }
}

static void SetupDesktop (void)

{
  Boolean     allowalign;
  FeatDefPtr  curr;
  Uint1       key;
  CharPtr     label = NULL;
  Char        proclabel [64];
  Char        procname [64];
  Boolean     readOnlyDbTags;
  Char        str [PATH_MAX];
  Uint2       subtype;
  Int2        val;
  Boolean     validateExons;

  SetAppProperty ("SequinAppVersion", SEQUIN_APPLICATION);

  MemSet ((Pointer) (&medviewprocs), 0, sizeof (MedlineViewProcs));
  medviewprocs.cleanupObjectPtr = FALSE;
  medviewprocs.activateForm = MedlineViewFormActivated;
  medviewprocs.closeForm = NULL;
  medviewprocs.useFolderTabs = CHANGE_VIEW_POPUP;
  /*
  medviewprocs.useFolderTabs = CHANGE_VIEW_RADIOBUTTONS;
  medviewprocs.initPage = CITATION_PAGE;
  */
#ifndef WIN_MAC
  medviewprocs.createMenus = MedlineViewFormMenus;
#endif
  medviewprocs.showAsnPage = TRUE;
#ifdef WIN_MOTIF
  if (indexerVersion) {
    medviewprocs.useScrollText = TRUE;
  } else {
    medviewprocs.useScrollText = FALSE;
  }
#else
  medviewprocs.useScrollText = FALSE;
#endif
  medviewprocs.handleMessages = SequinMedlineFormMessage;
  medviewprocs.makeControls = DoMakeMedViewerLinkControls;
  SetAppProperty ("MedlineDisplayForm", &medviewprocs);

  MemSet ((Pointer) (&seqviewprocs), 0, sizeof (SeqViewProcs));
  seqviewprocs.hasTargetControl = TRUE;
  seqviewprocs.hasDoneButton = TRUE;
  seqviewprocs.launchEditors = TRUE;
  seqviewprocs.launchSubviewers = FALSE;
  seqviewprocs.sendSelectMessages = TRUE;
  seqviewprocs.highlightSelections = TRUE;
  seqviewprocs.cleanupObjectPtr = FALSE;
  seqviewprocs.activateForm = BioseqViewFormActivated;
  seqviewprocs.closeForm = StdSendCloseWindowMessageProc;
  seqviewprocs.useFolderTabs = CHANGE_VIEW_POPUP;
  /*
  seqviewprocs.initNucPage = NUCASN2FF_PAGE_1;
  seqviewprocs.initProtPage = PROTGENPEPT_PAGE;
  */
#ifndef WIN_MAC
  seqviewprocs.createMenus = BioseqViewFormMenus;
#endif
#ifdef WIN_MOTIF
  if (indexerVersion) {
    seqviewprocs.createToolBar = BioseqViewFormToolBar; /* now in separate window */
  }
#endif
/*#ifdef INTERNAL_NCBI_SEQUIN*/
  if (indexerVersion) {
    seqviewprocs.allowScrollText = TRUE;
    seqviewprocs.startInScrollText = FALSE;
  } else {
    seqviewprocs.allowScrollText = FALSE;
    seqviewprocs.startInScrollText = FALSE;
  }
/*#endif*/
  seqviewprocs.handleMessages = SequinSeqViewFormMessage;
  seqviewprocs.makeControls = DoMakeSeqViewerLinkControls;
  seqviewprocs.updateControls = UpdateViewerLinkTarget;
  seqviewprocs.makeAlignBtn = DoMakeViewerAlignBtn;
  seqviewprocs.updateCounts = DoUpdateFetchCounts;

  SetupBioseqPageList ();

  SetAppProperty ("SeqDisplayForm", &seqviewprocs);

  MemSet ((Pointer) (&seqedprocs), 0, sizeof (SeqEditViewProcs));
#ifdef WIN_MAC
  seqedprocs.activateForm = SeqEditFormActivated;
#endif
  seqedprocs.handleMessages = SequinSeqEditFormMessage;
  seqedprocs.minPixelWidth = 0;
  seqedprocs.minPixelHeight = 0;
  seqedprocs.showfeat = FALSE;
  seqedprocs.extended_align_menu = FALSE;
  seqedprocs.extended_dist_menu = FALSE;
  seqedprocs.extended_tree_menu = FALSE;

  if (useEntrez) {
    seqedprocs.download = SeqEdDownload;
  }

  seqedprocs.col1r=seqedprocs.col1g=seqedprocs.col1b=0;
  seqedprocs.col2r=seqedprocs.col2g=seqedprocs.col2b=0;
  seqedprocs.col3r=seqedprocs.col3g=seqedprocs.col3b=0;
  seqedprocs.col4r=seqedprocs.col4g=seqedprocs.col4b=0;
  seqedprocs.col5r=seqedprocs.col5g=seqedprocs.col5b=0;
  seqedprocs.col6r=seqedprocs.col6g=seqedprocs.col6b=0;
  seqedprocs.col7r=seqedprocs.col7g=seqedprocs.col7b=0;

  SetAppProperty ("SeqEditDisplayForm", &seqedprocs);

  MemSet ((Pointer) (&stdedprocs), 0, sizeof (StdEditorProcs));
#ifdef WIN_MAC
  stdedprocs.activateForm = StdEditorFormActivated;
#endif
  stdedprocs.handleMessages = SequinStdEditorFormMessage;
  stdedprocs.duplicateExisting = FALSE;
  SetAppProperty ("StdEditorForm", &stdedprocs);

  MemSet ((Pointer) (&valdtrprocs), 0, sizeof (StdEditorProcs));
#ifdef WIN_MAC
  valdtrprocs.activateForm = StdValidatorFormActivated;
#endif
  valdtrprocs.handleMessages = SequinStdEditorFormMessage;
  valdtrprocs.duplicateExisting = FALSE;
  SetAppProperty ("StdValidatorForm", &valdtrprocs);

  MemSet ((Pointer) (&txtviewprocs), 0, sizeof (TextViewProcs));
#ifdef WIN_MAC
  txtviewprocs.activateForm = TextViewProcFormActivated;
#endif
#ifdef WIN_MOTIF
  if (indexerVersion) {
    txtviewprocs.useScrollText = TRUE;
  } else {
    txtviewprocs.useScrollText = FALSE;
  }
#else
  txtviewprocs.useScrollText = FALSE;
#endif
  SetAppProperty ("TextDisplayForm", &txtviewprocs);

  SetAppProperty ("HelpMessageProc", (Pointer) ProcessHelpMessage);

  MemSet ((Pointer) (&pubedprocs), 0, sizeof (PubdescEditProcs));
  if (useMedarch) {
    pubedprocs.lookupArticle = LookupAnArticleFunc;
    pubedprocs.lookupJournal = LookupJournalFunc;
  }
/*#ifdef REPLACE_THIS*/
  if (indexerVersion) {
    pubedprocs.replaceThis = TRUE;
  }
/*#endif*/
  SetAppProperty ("PubdescEditForm", &pubedprocs);

  MemSet ((Pointer) (&biosrcedprocs), 0, sizeof (BioSourceEditProcs));
  if (useTaxon) {
    biosrcedprocs.lookupTaxonomy = LookupTaxonomyFunc;
  }
  SetAppProperty ("BioSourcEditForm", &biosrcedprocs);

  readOnlyDbTags = TRUE;

/*#ifdef INTERNAL_NCBI_SEQUIN*/
  if (indexerVersion) {
    SetAppProperty ("InternalNcbiSequin", (void *) 1024);
    readOnlyDbTags = FALSE;
  }
/*#endif*/

  if (genomeCenter != NULL) {
    SetAppProperty ("GenomeCenterSequin", (void *) 1024);
    readOnlyDbTags = FALSE;
  }

  if (readOnlyDbTags) {
    SetAppProperty ("ReadOnlyDbTags", (void *) 1024);
  }

  if (useNewGraphicView) {
    SetAppProperty ("NewSequinGraphicalViewer", (void *) 1024);
    SetAppProperty ("NewSequinLayoutOverride", (void *) 1024);
  }

  if (GetSequinAppParam ("SETTINGS", "BROWSER", NULL, str, sizeof (str))) {
    SetAppProperty ("MedviewBrowserPath", (void *) StringSave (str));
  }

  if (GetSequinAppParam ("PREFERENCES", "MINPIXELWIDTH", NULL, str, sizeof (str))) {
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, screenRect.right);
      medviewprocs.minPixelWidth = val;
      seqviewprocs.minPixelWidth = val;
    }
  }

  if (GetSequinAppParam ("PREFERENCES", "MINPIXELHEIGHT", NULL, str, sizeof (str))) {
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, screenRect.bottom);
      medviewprocs.minPixelHeight = val;
      seqviewprocs.minPixelHeight = val;
    }
  }

  if (GetSequinAppParam ("PREFERENCES", "SEQEDPIXELWIDTH", NULL, str, sizeof (str))) {
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, screenRect.right);
      seqedprocs.minPixelWidth = val;
    }
  }

  if (GetSequinAppParam ("PREFERENCES", "SEQEDPIXELHEIGHT", NULL, str, sizeof (str))) {
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, screenRect.bottom);
      seqedprocs.minPixelHeight = val;
    }
  }

  if (GetSequinAppParam ("PREFERENCES", "TEXTPIXELWIDTH", NULL, str, sizeof (str))) {
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, screenRect.right);
      txtviewprocs.minPixelWidth = val;
    }
  }

  if (GetSequinAppParam ("PREFERENCES", "TEXTPIXELHEIGHT", NULL, str, sizeof (str))) {
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, screenRect.bottom);
      txtviewprocs.minPixelHeight = val;
    }
  }

  if (GetSequinAppParam ("PREFERENCES", "SEQEDSHOWFEAT", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      seqedprocs.showfeat = TRUE;
    }
  }

  if (GetSequinAppParam ("PREFERENCES", "SEQEDEXTENDALIGN", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      seqedprocs.extended_align_menu = TRUE;
    }
  }

  if (GetSequinAppParam ("PREFERENCES", "SEQEDEXTENDDIST", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      seqedprocs.extended_dist_menu = TRUE;
    }
  }

  if (GetSequinAppParam ("PREFERENCES", "SEQEDEXTENDTREE", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      seqedprocs.extended_tree_menu = TRUE;
    }
  }

  if (GetSequinAppParam ("PREFERENCES", "USEOLDASN", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      leaveAsOldAsn = TRUE;
    }
  }

  if (GetSequinAppParam ("SETTINGS", "XMLPREFIX", NULL, str, sizeof (str))) {
    AsnSetXMLmodulePrefix (StringSave (str));
  }

  if (GetSequinAppParam ("PREFERENCES", "COL1R", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col1r = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL1G", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col1g = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL1B", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col1b = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL2R", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col2r = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL2G", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col2g = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL2B", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col2b = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL3R", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col3r = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL3G", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col3g = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL3B", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col3b = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL4R", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col4r = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL4G", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col4g = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL4B", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col4b = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL5R", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col5r = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL5G", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col5g = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL5B", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col5b = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL6R", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col6r = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL6G", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col6g = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL6B", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col6b = val;
    }
  }

  if (GetSequinAppParam ("PREFERENCES", "COL7R", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col7r = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL7G", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col7g = val;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "COL7B", NULL, str, sizeof (str)))
{
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, 255);
      seqedprocs.col7b = val;
    }
  }

  validateExons = TRUE;
  if (GetSequinAppParam ("PREFERENCES", "VALIDATEEXONS", NULL, str, sizeof (str))) {
    if (StringICmp (str, "FALSE") == 0) {
      validateExons = FALSE;
    }
  }
  if (validateExons) {
    SetAppProperty ("ValidateExons", (void *) 1024);
  }
  SetAppProperty ("SpliceValidateAsError", (void *) 1024);

  /*
  if (GetSequinAppParam ("SETTINGS", "NUCFIELD", NULL, str, sizeof (str))) {
    if (StrToInt (str, &val) && val >= 0) {
      seqviewprocs.initNucPage = val;
    }
  }

  if (GetSequinAppParam ("SETTINGS", "PRTFIELD", NULL, str, sizeof (str))) {
    if (StrToInt (str, &val) && val >= 0) {
      seqviewprocs.initProtPage = val;
    }
  }
  */

  if (GetSequinAppParam ("SETTINGS", "TARGETCONTROL", NULL, str, sizeof (str))) {
    if (StrToInt (str, &val) && val >= 0) {
      seqviewprocs.useFolderTabs = val;
    }
  }

  if (GetAppParam ("SEQUIN", "SETTINGS", "ALLOWSCROLLTEXT", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      seqviewprocs.allowScrollText = TRUE;
      seqviewprocs.startInScrollText = FALSE;
    }
  }

  if (GetSequinAppParam ("SETTINGS", "MEDPAGE", "Abstract", str, sizeof (str))) {
    medviewprocs.initMedLabel = StringSaveNoNull (str);
  }
  if (GetSequinAppParam ("SETTINGS", "NUCPAGE", "GenBank", str, sizeof (str))) {
    seqviewprocs.initNucLabel = StringSaveNoNull (str);
  }
  if (GetSequinAppParam ("SETTINGS", "PRTPAGE", "GenPept", str, sizeof (str))) {
    seqviewprocs.initProtLabel = StringSaveNoNull (str);
  }
  if (GetSequinAppParam ("SETTINGS", "GENMPAGE", "Map", str, sizeof (str))) {
    seqviewprocs.initGenomeLabel = StringSaveNoNull (str);
  }

  if (GetSequinAppParam ("SETTINGS", "LOCKFAR", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      seqviewprocs.lockFarComponents = TRUE;
    }
  }

  MemSet ((Pointer) (&entrezglobals), 0, sizeof (EntrezGlobals));
  entrezglobals.retrieveDocsProc = DoRetrieveDocuments;
  entrezglobals.retrieveProjectProc = DoRetrieveProject;
  entrezglobals.retrieveSimpleProc = DoRetrieveSimple;
  entrezglobals.loadNamedUidProc = DoLoadNamedUidList;
  entrezglobals.launchViewerProc = DoLaunchRecordViewer;
#ifndef WIN_MAC
  entrezglobals.createTrmLstMenus = TermListFormMenus;
  entrezglobals.createDocSumMenus = DocSumFormMenus;
#endif
  SetAppProperty ("EntrezGlobals", &entrezglobals);

  entrezglobals.showAsn = TRUE;

  entrezglobals.persistDefault = TRUE;
  if (GetSequinAppParam ("PREFERENCES", "PARENTSPERSIST", NULL, str, sizeof (str) - 1)) {
    if (StringICmp (str, "FALSE") == 0) {
      entrezglobals.persistDefault = FALSE;
    }
  }
  entrezglobals.alignDefault = TRUE;
  if (GetSequinAppParam ("PREFERENCES", "ALIGNCHECKED", NULL, str, sizeof (str) - 1)) {
    if (StringICmp (str, "FALSE") == 0) {
      entrezglobals.alignDefault = FALSE;
    }
  }
  seqviewprocs.alignDefault = entrezglobals.alignDefault;
  if (GetAppParam ("SEQUIN", "PREFERENCES", "POPDOWN", NULL, str, sizeof (str) - 1)) {
    if (StringICmp (str, "TRUE") == 0) {
      entrezglobals.popdownBehavior = TRUE;
    }
  }
  if (GetSequinAppParam ("PREFERENCES", "LOOKUPDIRECT", NULL, str, sizeof (str) - 1)) {
    if (StringICmp (str, "TRUE") == 0) {
      entrezglobals.lookupDirect = TRUE;
    }
  }
  entrezglobals.sortFields = TRUE;
  if (GetAppParam ("SEQUIN", "PREFERENCES", "SORTFIELDS", NULL, str, sizeof (str) - 1)) {
    if (StringICmp (str, "FALSE") == 0) {
      entrezglobals.sortFields = FALSE;
    }
  }
  if (GetSequinAppParam ("SETTINGS", "DATABASE", "MEDLINE", str, sizeof (str))) {
    entrezglobals.initDatabase = StringSaveNoNull (str);
  }
  if (GetSequinAppParam ("SETTINGS", "FIELD", "All Fields", str, sizeof (str))) {
    entrezglobals.initField = StringSaveNoNull (str);
  }
  if (GetSequinAppParam ("SETTINGS", "MODE", "Automatic", str, sizeof (str))) {
    entrezglobals.initMode = StringSaveNoNull (str);
  }

/*#ifdef INTERNAL_NCBI_SEQUIN*/
  if (indexerVersion) {
    allowalign = TRUE;
  } else {
/*#else*/
    allowalign = FALSE;
  }
/*#endif*/
  if (GetSequinAppParam ("SETTINGS", "ALLOWALIGNMENT", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      allowalign = TRUE;
    }
  }
  if (allowalign) {
    SetAppProperty ("AllowAlignment", (void *) 1024);
  }

  /* register types and functions first */

  REGISTER_MEDLINE_VIEW;
  if (smartnetMode) {
    REGISTER_SMART_SEQENTRY_VIEW;
  } else {
    REGISTER_NEW_SEQENTRY_VIEW;
  }

  curr = FeatDefFindNext (NULL, &key, &label, FEATDEF_ANY, TRUE);
  while (curr != NULL) {
    if (key != FEATDEF_BAD && curr->seqfeat_key == SEQFEAT_IMP) {
      subtype = curr->featdef_key;
      if (subtype != FEATDEF_source) {
        sprintf (procname, "Edit %s", curr->menulabel);
        StringNCpy_0 (proclabel, curr->typelabel, sizeof (proclabel));
        if (proclabel [0] == '-') {
          proclabel [0] = '~';
        }
        REGISTER_IMPORT_EDIT(procname,proclabel,subtype);
      }
    }
    curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
  }

  curr = FeatDefFindNext (NULL, &key, &label, FEATDEF_ANY, TRUE);
  while (curr != NULL) {
    if (key != FEATDEF_BAD && curr->seqfeat_key == SEQFEAT_RNA) {
      subtype = curr->featdef_key;
      sprintf (procname, "Edit %s", curr->menulabel);
      StringNCpy_0 (proclabel, curr->typelabel, sizeof (proclabel));
      if (proclabel [0] == '-') {
        proclabel [0] = '~';
      }
      REGISTER_RNA_EDIT(procname,proclabel,subtype);
    }
    curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
  }

  curr = FeatDefFindNext (NULL, &key, &label, FEATDEF_ANY, TRUE);
  while (curr != NULL) {
    if (key != FEATDEF_BAD && curr->seqfeat_key == SEQFEAT_PROT) {
      subtype = curr->featdef_key;
      sprintf (procname, "Edit %s", curr->menulabel);
      StringNCpy_0 (proclabel, curr->typelabel, sizeof (proclabel));
      if (proclabel [0] == '-') {
        proclabel [0] = '~';
      }
      REGISTER_PROT_EDIT(procname,proclabel,subtype);
    }
    curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
  }

  REGISTER_GENE_EDIT;
  REGISTER_CDRGN_EDIT;
  REGISTER_RGNFEAT_EDIT;
  REGISTER_CMNTFEAT_EDIT;
  REGISTER_BIOSOURCE_FEAT_EDIT;
  REGISTER_BIOSOURCE_DESC_EDIT;
  REGISTER_MOLINFO_EDIT;
  REGISTER_PUBDESC_FEAT_EDIT;
  REGISTER_PUBDESC_DESC_EDIT;
  REGISTER_TITLE_EDIT;
  REGISTER_COMMENT_EDIT;
  REGISTER_NAME_EDIT;
  REGISTER_REGION_EDIT;
  REGISTER_BOND_EDIT;
  REGISTER_SITE_EDIT;
  REGISTER_PSEC_EDIT;
  REGISTER_GENBANK_EDIT;
  REGISTER_CREATE_DATE_EDIT;
  REGISTER_UPDATE_DATE_EDIT;


  REGISTER_NEW_BIOSEQ_EDIT;
  REGISTER_NEW_SEQALIGN_EDIT;
  REGISTER_NEW_SEQANNOT_EDIT;
  REGISTER_NEW_SEQALIGN_VIEW; 
  REGISTER_BIOSEQ_SEG_EDIT;
  REGISTER_BIOSEQ_SET_EDIT;
  REGISTER_SUBMITBLOCK_EDIT;

  SetupSequinFilters ();
}

static FonT ChooseAFont (CharPtr param, CharPtr dfault)

{
  FonT  f;
  Char  str [128];

  f = NULL;
  if (GetSequinAppParam ("FONTS", param, NULL, str, sizeof (str))) {
    f = ParseFont (str);
  } else {
    if (! indexerVersion) {
      SetSequinAppParam ("FONTS", param, dfault);
    }
    f = ParseFont (dfault);
  }
  return f;
}

static void SetupCommonFonts (void)

{
#ifdef WIN_MAC
  medviewprocs.jourfnt = ChooseAFont ("JOURNAL", "Geneva,10,i");
  medviewprocs.volfnt = ChooseAFont ("VOLUME", "Geneva,10,b");
  medviewprocs.pagesfnt = ChooseAFont ("PAGES", "Geneva,10");
  medviewprocs.titlefnt = ChooseAFont ("TITLE", "Times,14,b");
  medviewprocs.authorsfnt = ChooseAFont ("AUTHORS", "Times,14");
  medviewprocs.affilfnt = ChooseAFont ("AFFILIATION", "Times,12");
  medviewprocs.abstractfnt = ChooseAFont ("ABSTRACT", "Geneva,10");
  medviewprocs.meshfnt = ChooseAFont ("MESH", "Monaco,9");
  medviewprocs.displayFont = ChooseAFont ("DISPLAY", "Monaco,9");
#endif
#ifdef WIN_MSWIN
  medviewprocs.jourfnt = ChooseAFont ("JOURNAL", "Arial,11,i");
  medviewprocs.volfnt = ChooseAFont ("VOLUME", "Arial,11,b");
  medviewprocs.pagesfnt = ChooseAFont ("PAGES", "Arial,11");
  medviewprocs.titlefnt = ChooseAFont ("TITLE", "Times New Roman,14,b");
  medviewprocs.authorsfnt = ChooseAFont ("AUTHORS", "Times New Roman,14");
  medviewprocs.affilfnt = ChooseAFont ("AFFILIATION", "Times New Roman,11");
  medviewprocs.abstractfnt = ChooseAFont ("ABSTRACT", "Times New Roman,11");
  medviewprocs.meshfnt = ChooseAFont ("MESH", "Times New Roman,9");
  medviewprocs.displayFont = ChooseAFont ("DISPLAY", "Courier New,10");
#endif
#ifdef WIN_MOTIF
  medviewprocs.jourfnt = ChooseAFont ("JOURNAL", "Helvetica,12,i");
  medviewprocs.volfnt = ChooseAFont ("VOLUME", "Helvetica,12,b");
  medviewprocs.pagesfnt = ChooseAFont ("PAGES", "Helvetica,12");
  medviewprocs.titlefnt = ChooseAFont ("TITLE", "Times,18,b");
  medviewprocs.authorsfnt = ChooseAFont ("AUTHORS", "Times,18");
  medviewprocs.affilfnt = ChooseAFont ("AFFILIATION", "Times,14");
  medviewprocs.abstractfnt = ChooseAFont ("ABSTRACT", "Times,14");
  medviewprocs.meshfnt = ChooseAFont ("MESH", "Times,12");
  medviewprocs.displayFont = ChooseAFont ("DISPLAY", "Courier,10");
#endif

#ifdef WIN_MAC
  seqviewprocs.displayFont = ChooseAFont ("DISPLAY", "Monaco,9");
#endif
#ifdef WIN_MSWIN
  seqviewprocs.displayFont = ChooseAFont ("DISPLAY", "Courier New,10");
#endif
#ifdef WIN_MOTIF
  seqviewprocs.displayFont = ChooseAFont ("DISPLAY", "Courier,10");
#endif

#ifdef WIN_MAC
  entrezglobals.docsumFont = ChooseAFont ("FETCHED", "Monaco,9");
#endif
#ifdef WIN_MSWIN
  entrezglobals.docsumFont = ChooseAFont ("FETCHED", "Courier New,10");
#endif
#ifdef WIN_MOTIF
  entrezglobals.docsumFont = ChooseAFont ("FETCHED", "Courier,10");
#endif

#ifdef WIN_MAC
  txtviewprocs.displayFont = ChooseAFont ("DISPLAY", "Monaco,9");
#endif
#ifdef WIN_MSWIN
  txtviewprocs.displayFont = ChooseAFont ("DISPLAY", "Courier New,10");
#endif
#ifdef WIN_MOTIF
  txtviewprocs.displayFont = ChooseAFont ("DISPLAY", "Courier,10");
#endif
}

static void ToggleOldAsnProc (IteM i)

{
  leaveAsOldAsn = GetStatus (i);
}

static void RevStringUpper (CharPtr str)
{
	CharPtr nd;
	Char tmp;

		if (str == NULL)
			return;
    nd = str;
	while (*nd != '\0')
		nd++;
	nd--;

	while (nd > str)
	{
		tmp = TO_UPPER(*nd);
		*nd = TO_UPPER(*str);
		*str = tmp;
		nd--; str++;
	}

	if (nd == str)
		*nd = TO_UPPER(*nd);
	return;
}

static void ObjMgrReport (IteM i)

{
  BioseqPtr                  bsp;
  Char                       buf [41];
  FILE                       *fp;
  Int4                       j, num;
  ObjMgrDataPtr              omdp;
  ObjMgrDataPtr PNTR         omdpp;
  ObjMgrPtr                  omp;
  Char                       path [PATH_MAX];
  SeqIdIndexElementPtr PNTR  sipp;
  SeqMgrPtr                  smp;

  omp = ObjMgrGet ();
  if (omp == NULL) return;
  smp = SeqMgrGet ();
  if (smp == NULL) return;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp == NULL) return;
  fprintf (fp, "totobj %d, currobj %d\n", (int) omp->totobj, (int) omp->currobj);
  omdpp = omp->datalist;
  if (omdpp != NULL) {
    num = omp->currobj;
    for (j = 0; j < num; j++) {
      omdp = omdpp [j];
      if (omdp != NULL && omdp->datatype == OBJ_BIOSEQ) {
        bsp = (BioseqPtr) omdp->dataptr;
        if (bsp != NULL) {
          SeqIdWrite (bsp->id, buf, PRINTID_REPORT, sizeof (buf) - 1);
          fprintf (fp, "%4ld %s\n", (long) j, buf);
        }
      }
    }
  }
  fprintf (fp, "seqid index count %ld\n", (long) smp->BioseqIndexCnt);
  num = smp->BioseqIndexCnt;
  sipp = smp->BioseqIndex;
  if (sipp != NULL) {
    for (j = 0; j < num; j++) {
      StringNCpy_0 (buf, sipp [j]->str, sizeof (buf));
      RevStringUpper (buf);
      fprintf (fp, "%4ld %s\n", (long) j, buf);
    }
  }
  FileClose (fp);
  LaunchGeneralTextViewer (path, "Object Manager Report");
  FileRemove (path);
}

#ifdef WIN_MAC
static void SetupMacMenus (void)

{
  MenU  m;
  Int2  mssgadd;
  Int2  mssgalign;
  Int2  mssgdup;
  Int2  mssgfeatprop;
  Int2  mssgseq;
  Int2  mssgsub;
  Int2  mssgupd;
  Int2  mssgupwthaln;
  MenU  sub;

  m = AppleMenu (NULL);
  AddAboutAndHelpMenuItems (m);
  DeskAccGroup (m);

  m = PulldownMenu (NULL, "File");
  openItem = CommandItem (m, "Open.../O", MacReadNewAsnProc);
  closeItem = FormCommandItem (m, "Close", NULL, VIB_MSG_CLOSE);
  SeparatorItem (m);
  importItem = FormCommandItem (m, "Import.../I", NULL, VIB_MSG_IMPORT);
  exportItem = FormCommandItem (m, "Export.../E", NULL, VIB_MSG_EXPORT);
  SeparatorItem (m);
  duplicateViewItem = CommandItem (m, "Duplicate View", DuplicateViewProc);
  SeparatorItem (m);
  saveItem = FormCommandItem (m, "Save/S", NULL, VIB_MSG_SAVE);
  saveAsItem = FormCommandItem (m, "Save As...", NULL, VIB_MSG_SAVE_AS);
  SeparatorItem (m);
  restoreItem = CommandItem (m, "Restore...", RestoreSeqEntryProc);
  SeparatorItem (m);
  prepareItem = CommandItem (m, "Prepare Submission...", PrepareSeqSubmitProc);
  /*
  submitItem = CommandItem (m, "Submit to NCBI", SubmitToNCBI);
  */
  SeparatorItem (m);
  if (loadSaveUidListOK) {
    /*
    loadUidItem = CommandItem (m, "Load Uid List...", ObsoleteUidListProc);
    saveUidItem = CommandItem (m, "Save Uid List...", ObsoleteUidListProc);
    SeparatorItem (m);
    */
  }
  printItem = FormCommandItem (m, "Print", NULL, VIB_MSG_PRINT);
  SeparatorItem (m);
  FormCommandItem (m, "Quit/Q", NULL, VIB_MSG_QUIT);

  m = PulldownMenu (NULL, "Edit");
  undoItem = FormCommandItem (m, UNDO_MENU_ITEM, NULL, VIB_MSG_UNDO);
  Disable (undoItem);
  SeparatorItem (m);
  cutItem = FormCommandItem (m, CUT_MENU_ITEM, NULL, VIB_MSG_CUT);
  copyItem = FormCommandItem (m, COPY_MENU_ITEM, NULL, VIB_MSG_COPY);
  pasteItem = FormCommandItem (m, PASTE_MENU_ITEM, NULL, VIB_MSG_PASTE);
  deleteItem = FormCommandItem (m, CLEAR_MENU_ITEM, NULL, VIB_MSG_DELETE);
  SeparatorItem (m);
  if (extraServices) {
    mssgdup = RegisterFormMenuItemName ("SequinDuplicateItem");
    duplicateItem = FormCommandItem (m, "Duplicate...", NULL, mssgdup);
    SeparatorItem (m);
  }
  if (genomeCenter != NULL || indexerVersion) {
    SetupEditSecondary (m, NULL);
    SeparatorItem (m);
  }
  mssgseq = RegisterFormMenuItemName ("SequinEditSequenceItem");
  mssgalign = RegisterFormMenuItemName ("SequinEditAlignmentItem");
  mssgsub = RegisterFormMenuItemName ("SequinEditSubmitterItem");
  mssgupd = RegisterFormMenuItemName ("SequinUpdateSeqSubmenu");
  mssgfeatprop = RegisterFormMenuItemName ("SequinFeaturePropagate");
  mssgadd = RegisterFormMenuItemName ("SequinAddSeqSubmenu");
  editsequenceitem = FormCommandItem (m, "Edit Sequence...", NULL, mssgseq);
  editseqalignitem = FormCommandItem (m, "Edit Alignment...", NULL, mssgalign);
  editseqsubitem = FormCommandItem (m, "Edit Submitter Info...", NULL, mssgsub);
  if (indexerVersion) {
    SeparatorItem (m);
    edithistoryitem = CommandItem (m, "Edit History....", EditSequenceHistory);
  }
  SeparatorItem (m);
  updateSeqMenu = SubMenu (m, "Update Sequence");
  SetFormMenuItem (NULL, mssgupd, (IteM) updateSeqMenu);
  /*
  CommandItem (updateSeqMenu, "Replace Sequence...", ReplaceSeq);
  CommandItem (updateSeqMenu, "Extend Sequence 5'...", ExtendSeq5);
  CommandItem (updateSeqMenu, "Extend Sequence 3'...", ExtendSeq3);
  CommandItem (updateSeqMenu, "Combine Records...", CombineRecs);
  */
  /*
  if (indexerVersion) {
    CommandItem (updateSeqMenu, "Read File, New Algorithm...", UpdateSeqTEST);
    SeparatorItem (updateSeqMenu);
  }
  */
  CommandItem (updateSeqMenu, "Read FASTA File...", UpdateSeqWithFASTA);
  CommandItem (updateSeqMenu, "Read Sequence Record...", UpdateSeqWithRec);
  if (useEntrez) {
    CommandItem (updateSeqMenu, "Download Accession...", UpdateSeqWithAcc);
    mssgupwthaln = RegisterFormMenuItemName ("SequinUpdateWithAlignment");
    updalignitem = FormCommandItem (updateSeqMenu, "Selected Alignment...", NULL, mssgupwthaln);
  }
  if (indexerVersion) {
    SeparatorItem (updateSeqMenu);
    CommandItem (updateSeqMenu, "FASTA Set", DoUpdatesSeq);
  }
  SeparatorItem (m);
  featPropItem = CommandItem (m, "Feature Propagate...", NewFeaturePropagate);
  SetFormMenuItem (NULL, mssgfeatprop, featPropItem);
  SeparatorItem (m);
  addSeqMenu = SubMenu (m, "Add Sequence");
  SetFormMenuItem (NULL, mssgadd, (IteM) addSeqMenu);
  CommandItem (addSeqMenu, "Add FASTA File...", AddSeqWithFASTA);
  CommandItem (addSeqMenu, "Add Sequence Record...", AddSeqWithRec);

  m = PulldownMenu (NULL, "Search");
  findItem = CommandItem (m, "Find ASN.1.../F", FindStringProc);
  findFFItem = CommandItem (m, "Find FlatFile.../G", FindFlatfileProc);
  SeparatorItem (m);
  findGeneItem = CommandItem (m, "Find by Gene...", FindGeneProc);
  findProtItem = CommandItem (m, "Find by Protein...", FindProtProc);
  findPosItem = CommandItem (m, "Find by Position...", FindPosProc);
  SeparatorItem (m);
  validateItem = CommandItem (m, "Validate...", ValSeqEntryProc);
#ifdef USE_SPELL
#ifdef WIN_MOTIF
  SeparatorItem (m);
  spellItem = CommandItem (m, "Spell Check...", SpellCheckSeqEntryProc);
#endif
#endif
/*#ifdef USE_BLAST*/
  if (useBlast) {
    /*
    SeparatorItem (m);
    powerBlastItem = CommandItem (m, "Power BLAST...", SimplePowerBlastProc);
    */
    SeparatorItem (m);
    cddBlastItem = CommandItem (m, "CDD BLAST...", SimpleCDDBlastProc);
    /*
    vectorScreenItem = CommandItem (m, "Vector Screen...", VectorScreenProc);
    */
  }
/*#endif*/
  SeparatorItem (m);
  orfItem = CommandItem (m, "ORF Finder...", FindOrf);
  /*
  aluItem = CommandItem (m, "Repeat Finder...", FindAlu);
  */
  SeparatorItem (m);
  targetItem = CommandItem (m, "Select Target...", DoChangeTarget);

  m = PulldownMenu (NULL, "Options");
  if (useEntrez) {
    preferencesItem = CommandItem (m, "Preferences...", PreferencesProc);
    SeparatorItem (m);
  }
  sub = SubMenu (m, "Font Selection");
  if (useEntrez) {
    docsumfontItem = CommandItem (sub, "DocSum Font...", DocSumFontChangeProc);
  }
  displayfontItem = CommandItem (sub, "Display Font...", DisplayFontChangeProc);
  SeparatorItem (m);
  legendItem = CreateLegendItem (m, NULL);
  SeparatorItem (m);
  sub = SubMenu (m, "Query Style");
  queryChoice = CreateQueryTypeChoice (sub, NULL);
  clearUnusedItem = CreateClearUnusedItem (m, NULL);
  SeparatorItem (m);
  sub = SubMenu (m, "Neighbor Policy");
  neighborChoice = CreateNeighborDelayChoice (sub, NULL);
  SeparatorItem (m);
  LoadDocsumOptionsMenu (m);
  seqviewprocs.alignWithChecked = entrezglobals.alignWithChecked;
  seqviewprocs.alignDefault = entrezglobals.alignDefault;
  if (indexerVersion) {
    SeparatorItem (m);
    oldAsnItem = StatusItem (m, "Use Old ASN.1", ToggleOldAsnProc);
    SetStatus (oldAsnItem, leaveAsOldAsn);
  }

/*#ifdef EXTRA_SERVICES*/
  if (extraServices) {
    specialMenu = PulldownMenu (NULL, "Special");
    SetupSpecialMenu (specialMenu, NULL);
  }
/*#endif*/

  m = PulldownMenu (NULL, "Misc");
  CommandItem (m, "Style Manager...", StyleManagerProc);
  SeparatorItem (m);
  CommandItem (m, "Net Configure...", NetConfigureProc);
  if (useEntrez) {
    SeparatorItem (m);
    CommandItem (m, "Entrez Query...", EntrezQueryProc);
/*
#ifndef WIN16
    if (BiostrucAvail ()) {
      SeparatorItem (m);
      CommandItem (m, "Cn3D Window...", Cn3DWinShowProc);
    }
#endif
*/
  }
  if (useDesktop) {
    SeparatorItem (m);
    VSMAddToMenu (m, VSM_DESKTOP);
  }

  /*
  if (indexerVersion) {
    SeparatorItem (m);
    CommandItem (m, "Object Manager Report", ObjMgrReport);
  }
  */

  analysisMenu = CreateAnalysisMenu (NULL, NULL, TRUE, TRUE);

  newFeatMenu = PulldownMenu (NULL, "Annotate");
  SetupNewFeaturesMenu (newFeatMenu, NULL);
  SeparatorItem (newFeatMenu);
  newPubMenu = SubMenu (newFeatMenu, "Publications");
  SetupNewPublicationsMenu (newPubMenu, NULL);
  SeparatorItem (newFeatMenu);
  newDescMenu = SubMenu (newFeatMenu, "Descriptors");
  SetupNewDescriptorsMenu (newDescMenu, NULL);
  SeparatorItem (newFeatMenu);
  CommandItem (newFeatMenu, "Generate Definition Line", GenerateAutoDefLinesSmartMods);
}
#endif

extern Boolean allowUnableToProcessMessage;
Boolean allowUnableToProcessMessage = TRUE;

static CharPtr canfeatpropagate =
"Since this record has an alignment, you can annotate features on " \
"one record and then use use feature propagation to annotate the "\
"equivalent features on the other records. This is much faster " \
"than annotating everything manually.";

static void FinishPuttingTogether (ForM f)

{
  BaseFormPtr   bfp;
  BioseqSetPtr  bssp;
  Uint2         entityID = 0;
  Int2          handled;
  SeqEntryPtr   sep = NULL;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    sep = (SeqEntryPtr) FormToPointer (bfp->form);
    if (sep != NULL) {
/*#ifdef USE_TAXON*/
      if (! leaveAsOldAsn) {
        MySeqEntryToAsn3 (sep, TRUE, FALSE, FALSE);
      }
/*#endif*/
      entityID = PackageFormResults (globalsbp, sep, TRUE);
      globalsbp = NULL;
      WatchCursor ();
      seqviewprocs.forceSeparateViewer = TRUE;
      SeqEntrySetScope (NULL);
      handled = GatherProcLaunch (OMPROC_VIEW, FALSE, entityID, 1,
                                  OBJ_BIOSEQ, 0, 0, OBJ_BIOSEQ, 0);
      ArrowCursor ();
      if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
        Message (MSG_FATAL, "Unable to launch viewer.");
      } else {
        SendHelpScrollMessage (helpForm, "Editing the Record", NULL);
      }
      ObjMgrSetOptions (OM_OPT_FREE_IF_NO_VIEW, entityID);
      ObjMgrSetDirtyFlag (entityID, TRUE);
    } else if (allowUnableToProcessMessage) {
      Message (MSG_FATAL, "Unable to process Seq-entry.");
    }
    /*SetChecklistValue (checklistForm, 5);*/
    Remove (bfp->form);
    if (sep != NULL && entityID > 0 && IS_Bioseq_set (sep)) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      if (bssp != NULL && (bssp->_class >= 13 && bssp->_class <= 16)) {
        if (SeqEntryHasAligns (entityID, sep)) {
          Message (MSG_OK, "%s", canfeatpropagate);
        }
      }
    }
  }
}

static CharPtr multcomponent = "\
ERROR - You may not enter multiple segments for a single sequence submission.\n\
You should either clear the nucleotide and import a single FASTA record, or \n\
return to the Sequence Format form and choose the proper submission type.";

static void PutItTogether (ButtoN b)

{
  MsgAnswer    ans;
  BaseFormPtr  bfp;
  ValNodePtr   head;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp != NULL) {
    head = TestForm (bfp->form);
    if (head != NULL) {
      if (! DisplayTestResults (head)) {
        ValNodeFreeData (head);
        return;
      }
      ValNodeFreeData (head);
    }
    if (SequencesFormHasTooManyNucleotides (bfp->form)) {
      Message (MSG_OK, "%s", multcomponent);
      return;
    }
    if (! SequencesFormHasProteins (bfp->form)) {
      ans = Message (MSG_OKC, "You have not entered proteins.  Is this correct?");
      if (ans == ANS_CANCEL) return;
    }
    Hide (bfp->form);
    ConfirmSequencesFormParsing (bfp->form, FinishPuttingTogether);
  }
}

static void GetOrgAndSeq (ButtoN b);
static void BackToStartup (ButtoN b);
static void BackToFormat (ButtoN b)

{
  MsgAnswer    ans;
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp != NULL) {
    ans = Message (MSG_OKC, "Are you sure?  Organism and sequence information will be lost.");
    if (ans == ANS_CANCEL) return;
    Remove (bfp->form);
    WatchCursor ();
    Update ();
    PointerToForm (formatForm, &globalFormatBlock);
    Show (formatForm);
    Select (formatForm);
    SendHelpScrollMessage (helpForm, "Sequence Format Form", NULL);
    ArrowCursor ();
    Update ();
  }
}

static void GetOrgAndSeq (ButtoN b)

{
  FormatBlockPtr  fbp;
  MonitorPtr      mon;
  ForM            w;

  WatchCursor ();
  Hide (formatForm);
  fbp = (FormatBlockPtr) FormToPointer (formatForm);
  if (fbp != NULL) {
    globalFormatBlock.seqPackage = fbp->seqPackage;
    globalFormatBlock.seqFormat = fbp->seqFormat;
    globalFormatBlock.numSeqs = fbp->numSeqs;
  }
  MemFree (fbp);
  WatchCursor ();
  mon = MonitorStrNewEx ("Sequin New Submission", 30, FALSE);
  MonitorStrValue (mon, "Creating Sequences Form");
  Update ();
  w = CreateInitOrgNucProtForm (-5, -67, "Organism and Sequences",
                                &globalFormatBlock,
                                PutItTogether, BackToFormat,
                                OrgAndSeqsActivateProc);
  ArrowCursor ();
  /*SetChecklistValue (checklistForm, 1);*/
  MonitorFree (mon);
  Update ();
  if (w != NULL) {
    Show (w);
    Select (w);
    SendHelpScrollMessage (helpForm, "Organism and Sequences Form", NULL);
  } else {
    Message (MSG_FATAL, "Unable to create window.");
  }
  Update ();
}

static void GetFormat (ButtoN b)

{
  ValNodePtr   head;

  head = TestForm (initSubmitForm);
  if (head != NULL) {
    if (! DisplayTestResults (head)) {
      ValNodeFreeData (head);
      return;
    }
    ValNodeFreeData (head);
  }
  Hide (initSubmitForm);
  globalsbp = (SequinBlockPtr) FormToPointer (initSubmitForm);
  if (globalsbp == NULL) {
    Message (MSG_OK, "Record will be a Seq-entry instead of a Seq-submit.");
  }
  PointerToForm (formatForm, &globalFormatBlock);
  Show (formatForm);
  Select (formatForm);
  SendHelpScrollMessage (helpForm, "Sequence Format Form", NULL);
  Update ();
}

static void BackToSubmitter (ButtoN b)

{
  MsgAnswer    ans;

  ans = Message (MSG_OKC, "Are you sure?  Format information will be lost.");
  if (ans == ANS_CANCEL) return;
  Hide (formatForm);
  Update ();
  PointerToForm (initSubmitForm, globalsbp);
  globalsbp = SequinBlockFree (globalsbp);
  Show (initSubmitForm);
  Select (initSubmitForm);
  SendHelpScrollMessage (helpForm, "Submitting Authors Form", NULL);
  Update ();
  globalFormatBlock.seqPackage = SEQ_PKG_SINGLE;
  globalFormatBlock.seqFormat = SEQ_FMT_FASTA;
  globalFormatBlock.numSeqs = 0;
}

static void BackToStartup (ButtoN b)

{
  MsgAnswer    ans;

  ans = Message (MSG_OKC, "Are you sure?  Submitter information will be lost.");
  if (ans == ANS_CANCEL) return;
  Hide (initSubmitForm);
  Update ();
  Show (startupForm);
  Select (startupForm);
  SendHelpScrollMessage (helpForm, "Introduction", NULL);
  Update ();
}

static void ShowHelp (ButtoN b)

{
  if (helpForm == NULL) {
    WatchCursor ();
    helpForm = CreateHelpForm (-95, -5, "Sequin Help", "sequin.hlp",
                               HideHelpForm, HelpActivateProc);
    ArrowCursor ();
  }
  if (helpForm != NULL) {
    Show (helpForm);
    Select (helpForm);
  }
}

static void StartNew (ButtoN b)

{
  Hide (startupForm);
  Update ();
  PointerToForm (initSubmitForm, NULL);
  Show (initSubmitForm);
  Select (initSubmitForm);
  SendHelpScrollMessage (helpForm, "Submitting Authors Form", NULL);
  Update ();
}

static void FinishGenome (ButtoN b)

{
  BaseFormPtr   bfp;
  Uint2         entityID;
  Int2          handled;
  SeqEntryPtr   sep;
  SeqSubmitPtr  ssp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;
  ssp = (SeqSubmitPtr) FormToPointer (bfp->form);
  if (ssp == NULL) return;
  if (ssp->datatype != 1) return;
  sep = (SeqEntryPtr) ssp->data;
  if (sep == NULL) return;
  ObjMgrConnect (OBJ_SEQENTRY, sep->data.ptrvalue, OBJ_SEQSUB, (Pointer) ssp);
  if (! ObjMgrRegister (OBJ_SEQSUB, (Pointer) ssp)) {
    ErrPostEx (SEV_ERROR, 0, 0, "ObjMgrRegister failed.");
  }
  if (! leaveAsOldAsn) {
    MySeqEntryToAsn3 (sep, TRUE, FALSE, FALSE);
  }
  entityID = ObjMgrGetEntityIDForChoice (sep);
  WatchCursor ();
  seqviewprocs.forceSeparateViewer = TRUE;
  SeqEntrySetScope (NULL);
  handled = GatherProcLaunch (OMPROC_VIEW, FALSE, entityID, 1,
                              OBJ_BIOSEQ, 0, 0, OBJ_BIOSEQ, 0);
  ArrowCursor ();
  if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
    Message (MSG_FATAL, "Unable to launch viewer.");
  } else {
    SendHelpScrollMessage (helpForm, "Editing the Record", NULL);
  }
  ObjMgrSetOptions (OM_OPT_FREE_IF_NO_VIEW, entityID);
  ObjMgrSetDirtyFlag (entityID, TRUE);
}

static void CancelGenome (ButtoN b)

{
  Show (startupForm);
  Select (startupForm);
  SendHelpScrollMessage (helpForm, "Introduction", NULL);
  Update ();
}

static void StartFa2htgs (ButtoN b)

{
  ForM  f;

  Hide (startupForm);
  Update ();
  f = CreateGenomeCenterForm (-50, -33, "HTGS Submission",
                              FinishGenome, CancelGenome, FALSE, FALSE,
                              GenomeFormActivateProc);
  if (f != NULL) {
    Show (f);
    Select (f);
  } else {
    Show (startupForm);
  }
  Update ();
}

static void StartPhrap (ButtoN b)

{
  ForM  f;

  Hide (startupForm);
  Update ();
  f = CreateGenomeCenterForm (-50, -33, "PHRAP Submission",
                              FinishGenome, CancelGenome, TRUE, FALSE,
                              GenomeFormActivateProc);
  if (f != NULL) {
    Show (f);
    Select (f);
  } else {
    Show (startupForm);
  }
  Update ();
}

static void BuildContig (ButtoN b)

{
  ForM  f;

  Hide (startupForm);
  Update ();
  f = CreateGenomeCenterForm (-50, -33, "Contig Submission",
                              FinishGenome, CancelGenome, FALSE, TRUE,
                              GenomeFormActivateProc);
  if (f != NULL) {
    Show (f);
    Select (f);
  } else {
    Show (startupForm);
  }
  Update ();
/*
  Hide (startupForm);
  Update ();
  if (! DoBuildContig ()) {
    Show (startupForm);
  }
  Update ();
*/
}

static void ReadOld (ButtoN b)

{
  Hide (startupForm);
  Update ();
  if (! CommonReadNewAsnProc (NULL, FALSE, TRUE)) {
    Show (startupForm);
    Select (startupForm);
  }
}

static void ReadSettings (void)

{
  Char  str [64];

  useDesktop = FALSE;
  useEntrez = FALSE;
  useLocal = FALSE;
  useBlast = FALSE;
  useMedarch = FALSE;
  useTaxon = FALSE;
  allowDownload = FALSE;
  extraServices = FALSE;
  indexerVersion = FALSE;

  genomeCenter = NULL;

#ifdef USE_DESKTOP
  useDesktop = TRUE;
#endif
  if (GetAppParam ("SEQUIN", "SETTINGS", "USEDESKTOP", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      useDesktop = TRUE;
    }
  }

#ifdef USE_ENTREZ
  useEntrez = TRUE;
#endif
  if (GetAppParam ("SEQUIN", "SETTINGS", "USEENTREZ", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      useEntrez = TRUE;
    }
  }

#ifdef USE_BLAST
  useBlast = TRUE;
#endif
  if (GetAppParam ("SEQUIN", "SETTINGS", "USEBLAST", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      useBlast = TRUE;
    }
  }

#ifdef USE_MEDARCH
  useMedarch = TRUE;
#endif
  if (GetAppParam ("SEQUIN", "SETTINGS", "USEMEDARCH", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      useMedarch = TRUE;
    }
  }

#ifdef USE_TAXON
  useTaxon = TRUE;
#endif
  if (GetAppParam ("SEQUIN", "SETTINGS", "USETAXON", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      useTaxon = TRUE;
    }
  }

#ifdef ALLOW_DOWNLOAD
  allowDownload = TRUE;
#endif
  if (GetAppParam ("SEQUIN", "SETTINGS", "ALLOWDOWNLOAD", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      allowDownload = TRUE;
      useEntrez = TRUE;
    }
  }

#ifdef EXTRA_SERVICES
  extraServices = TRUE;
#endif
  if (GetAppParam ("SEQUIN", "SETTINGS", "USEEXTRASERVICES", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      extraServices = TRUE;
    }
  }

#ifdef INTERNAL_NCBI_SEQUIN
  indexerVersion = TRUE;
  extraServices = TRUE;
  useDesktop = TRUE;
  useEntrez = TRUE;
  useLocal = TRUE;
  useBlast = TRUE;
  useMedarch = TRUE;
  useTaxon = TRUE;
  allowDownload = TRUE;
#endif
  if (GetAppParam ("SEQUIN", "SETTINGS", "INDEXERVERSION", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      indexerVersion = TRUE;
      extraServices = TRUE;
      useDesktop = TRUE;
      useEntrez = TRUE;
      useLocal = TRUE;
      useBlast = TRUE;
      useMedarch = TRUE;
      useTaxon = TRUE;
      allowDownload = TRUE;
    }
  }

#ifdef PUBLIC_NETWORK_SEQUIN
  useDesktop = TRUE;
  useEntrez = TRUE;
  useLocal = TRUE;
  useBlast = TRUE;
  useMedarch = TRUE;
  useTaxon = TRUE;
  allowDownload = TRUE;
#endif
  if (GetAppParam ("SEQUIN", "SETTINGS", "PUBLICNETWORKSEQUIN", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      useDesktop = TRUE;
      useEntrez = TRUE;
      useLocal = TRUE;
      useBlast = TRUE;
      useMedarch = TRUE;
      useTaxon = TRUE;
      allowDownload = TRUE;
    }
  }

  if (GetAppParam ("SEQUIN", "SETTINGS", "SUPPRESSENTREZ", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      allowDownload = FALSE;
      useEntrez = FALSE;
    }
  }

#ifdef USE_LOCAL
  useLocal = TRUE;
#endif
  if (GetAppParam ("SEQUIN", "SETTINGS", "USELOCAL", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      useLocal = TRUE;
    } else if (StringICmp (str, "FALSE") == 0) {
      useLocal = FALSE;
    }
  }

  if (GetAppParam ("SEQUIN", "SETTINGS", "GENOMETAG", NULL, str, sizeof (str))) {
    TrimSpacesAroundString (str);
    if (! StringHasNoText (str)) {
      genomeCenter = StringSave (str);
      extraServices = TRUE;
    }
  } else if (GetAppParam ("SEQUIN", "SETTINGS", "GENOMECENTER", NULL, str, sizeof (str))) {
    TrimSpacesAroundString (str);
    if (! StringHasNoText (str)) {
      genomeCenter = StringSave (str);
      extraServices = TRUE;
    }
  }

  loadSaveUidListOK = useEntrez;
  if (GetAppParam ("SEQUIN", "PREFERENCES", "LOADSAVEUIDLIST", NULL, str, sizeof (str))) {
    if (StringICmp (str, "FALSE") == 0) {
      loadSaveUidListOK = FALSE;
    }
  }

#ifdef NEW_GRAPHICAL_VIEWER
  useNewGraphicView = TRUE;
#endif
  if (GetAppParam ("SEQUIN", "SETTINGS", "USENEWGRAPHICAL", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      useNewGraphicView = TRUE;
    }
  }

  /*
  if (GetAppParam ("SEQUIN", "SETTINGS", "USEUDV", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      useUdv = TRUE;
    }
  }
  */
}

static void DoQuit (ButtoN b)

{
  QuitProgram ();
}

static void CleanupSequin (void)

{
  FiniSequinExtras ();

  ExitMuskStyles ();
  FreeOrganismTable ();
  FreeGeneticCodes ();
  FreePrintOptions ();

/*#ifdef USE_LOCAL*/
  if (useLocal) {
    LocalSeqFetchDisable ();
  }
/*#endif*/

/*#ifdef USE_ENTREZ*/
  if (useEntrez) {
    /* EntrezBioseqFetchDisable (); */
    if (EntrezIsInited ()) {
      EntrezFini ();
    }
    ID1BioseqFetchDisable ();
  }
/*#endif*/
}

static Pointer SubtoolModeAsnTextFileRead (CharPtr filename,
                                           Uint2Ptr datatypeptr, Uint2Ptr entityIDptr)

{
  AsnIoPtr       aip;
  BioseqSetPtr   bssp = NULL, bssp2;
  ObjMgrData     omdata;
  ObjMgrDataPtr  omdptop = NULL;
  Uint2          parenttype = 0;
  Pointer        parentptr = NULL;
  Pointer        ptr = NULL;
  SeqEntryPtr    sep = NULL, sep1, sep2;

  if (filename == NULL) return NULL;
  if (datatypeptr != NULL) *datatypeptr = 0;
  if (entityIDptr != NULL) *entityIDptr = 0;

  aip = AsnIoOpen (filename, "r");
  if (aip == NULL) return NULL;
  aip->scan_for_start = TRUE;
  while ((bssp2 = BioseqSetAsnRead (aip, NULL)) != NULL) {
    if (sep == NULL) {
      sep2 = SeqEntryNew ();
      if (sep2 != NULL) {
        sep2->choice = 2;
        sep2->data.ptrvalue = bssp2;
        SeqMgrSeqEntry (SM_BIOSEQSET, (Pointer) bssp2, sep2);
        sep = sep2;
        bssp = bssp2;
        SaveSeqEntryObjMgrData (sep, &omdptop, &omdata);
        GetSeqEntryParent (sep, &parentptr, &parenttype);
      }
    } else {
      for (sep1 = bssp->seq_set; sep1->next != NULL; sep1 = sep1->next) continue;
      sep1->next = bssp2->seq_set;
      bssp2->seq_set = NULL;
      BioseqSetFree (bssp2);
    }
  }
  if (sep != NULL) {
    SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
    RestoreSeqEntryObjMgrData (sep, omdptop, &omdata);
  }
  AsnIoClose (aip);
  ptr = (Pointer) bssp;
  if (ptr == NULL) {
    ErrPostEx (SEV_ERROR,0,0,"Couldn't read [%s], type [OBJ_BIOSEQSET]", filename);
  } else {
    if (datatypeptr != NULL) *datatypeptr = OBJ_BIOSEQSET;
    if (entityIDptr != NULL) *entityIDptr = ObjMgrRegister (OBJ_BIOSEQSET, ptr);
  }
  return ptr;
}

#ifdef USE_SMARTNET
static CharPtr dirsubfetchproc = "DirSubBioseqFetch";

static CharPtr dirsubfetchcmd = NULL;

extern Pointer ReadFromDirSub (CharPtr accn, Uint2Ptr datatype, Uint2Ptr entityID);
extern Pointer ReadFromDirSub (CharPtr accn, Uint2Ptr datatype, Uint2Ptr entityID)

{
  Char     cmmd [256];
  Pointer  dataptr;
  FILE*    fp;
  Char     path [PATH_MAX];

  if (datatype != NULL) {
    *datatype = 0;
  }
  if (entityID != NULL) {
    *entityID = 0;
  }
  if (! dirsubMode) return NULL;
  if (StringHasNoText (accn)) return NULL;

  if (dirsubfetchcmd == NULL) {
    if (GetAppParam ("SEQUIN", "DIRSUB", "FETCHSCRIPT", NULL, cmmd, sizeof (cmmd))) {
    	dirsubfetchcmd = StringSaveNoNull (cmmd);
    }
  }
  if (dirsubfetchcmd == NULL) return NULL;

  TmpNam (path);

  sprintf (cmmd, "csh %s %s > %s", dirsubfetchcmd, accn, path);
  system (cmmd);
  fp = FileOpen (path, "r");
  if (fp == NULL) {
    FileRemove (path);
    return NULL;
  }
  dataptr = ReadAsnFastaOrFlatFile (fp, datatype, entityID, FALSE, FALSE, TRUE, FALSE);
  FileClose (fp);
  FileRemove (path);
  return dataptr;
}


static Int2 LIBCALLBACK DirSubBioseqFetchFunc (Pointer data)

{
  BioseqPtr         bsp;
  Char              cmmd [256];
  Pointer           dataptr;
  Uint2             datatype;
  Uint2             entityID;
  FILE*             fp;
  OMProcControlPtr  ompcp;
  ObjMgrProcPtr     ompp;
  Char              path [PATH_MAX];
  SeqEntryPtr       sep = NULL;
  SeqIdPtr          sip;
  TextSeqIdPtr      tsip;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  ompp = ompcp->proc;
  if (ompp == NULL) return OM_MSG_RET_ERROR;
  sip = (SeqIdPtr) ompcp->input_data;
  if (sip == NULL) return OM_MSG_RET_ERROR;

  if (sip->choice != SEQID_GENBANK) return OM_MSG_RET_ERROR;
  tsip = (TextSeqIdPtr) sip->data.ptrvalue;
  if (tsip == NULL || StringHasNoText (tsip->accession)) return OM_MSG_RET_ERROR;

  if (dirsubfetchcmd == NULL) {
    if (GetAppParam ("SEQUIN", "DIRSUB", "FETCHSCRIPT", NULL, cmmd, sizeof (cmmd))) {
    	dirsubfetchcmd = StringSaveNoNull (cmmd);
    }
  }
  if (dirsubfetchcmd == NULL) return OM_MSG_RET_ERROR;

  TmpNam (path);

  sprintf (cmmd, "csh %s %s > %s", dirsubfetchcmd, tsip->accession, path);
  system (cmmd);
  fp = FileOpen (path, "r");
  if (fp == NULL) {
    FileRemove (path);
    return OM_MSG_RET_ERROR;
  }
  dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, &entityID, FALSE, FALSE, TRUE, FALSE);
  FileClose (fp);
  FileRemove (path);

  if (dataptr == NULL) return OM_MSG_RET_OK;

  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep == NULL) return OM_MSG_RET_ERROR;
  bsp = BioseqFindInSeqEntry (sip, sep);
  ompcp->output_data = (Pointer) bsp;
  ompcp->output_entityID = ObjMgrGetEntityIDForChoice (sep);
  return OM_MSG_RET_DONE;
}

static Boolean DirSubFetchEnable (void)

{
  ObjMgrProcLoad (OMPROC_FETCH, dirsubfetchproc, dirsubfetchproc,
                  OBJ_SEQID, 0, OBJ_BIOSEQ, 0, NULL,
                  DirSubBioseqFetchFunc, PROC_PRIORITY_DEFAULT);
  return TRUE;
}

static CharPtr smartfetchproc = "SmartBioseqFetch";

static CharPtr smartfetchcmd = NULL;

extern Pointer ReadFromSmart (CharPtr accn, Uint2Ptr datatype, Uint2Ptr entityID);
extern Pointer ReadFromSmart (CharPtr accn, Uint2Ptr datatype, Uint2Ptr entityID)

{
  Char     cmmd [256];
  Pointer  dataptr;
  FILE*    fp;
  Char     path [PATH_MAX];

  if (datatype != NULL) {
    *datatype = 0;
  }
  if (entityID != NULL) {
    *entityID = 0;
  }
  if (! dirsubMode) return NULL;
  if (StringHasNoText (accn)) return NULL;

  if (smartfetchcmd == NULL) {
    if (GetAppParam ("SEQUIN", "SMART", "FETCHSCRIPT", NULL, cmmd, sizeof (cmmd))) {
    	smartfetchcmd = StringSaveNoNull (cmmd);
    }
  }
  if (smartfetchcmd == NULL) return NULL;

  TmpNam (path);

  sprintf (cmmd, "csh %s %s > %s", smartfetchcmd, accn, path);
  system (cmmd);
  fp = FileOpen (path, "r");
  if (fp == NULL) {
    FileRemove (path);
    return NULL;
  }
  dataptr = ReadAsnFastaOrFlatFile (fp, datatype, entityID, FALSE, FALSE, TRUE, FALSE);
  FileClose (fp);
  FileRemove (path);
  return dataptr;
}


static Int2 LIBCALLBACK SmartBioseqFetchFunc (Pointer data)

{
  BioseqPtr         bsp;
  Char              cmmd [256];
  Pointer           dataptr;
  Uint2             datatype;
  Uint2             entityID;
  FILE*             fp;
  OMProcControlPtr  ompcp;
  ObjMgrProcPtr     ompp;
  Char              path [PATH_MAX];
  SeqEntryPtr       sep = NULL;
  SeqIdPtr          sip;
  TextSeqIdPtr      tsip;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  ompp = ompcp->proc;
  if (ompp == NULL) return OM_MSG_RET_ERROR;
  sip = (SeqIdPtr) ompcp->input_data;
  if (sip == NULL) return OM_MSG_RET_ERROR;

  if (sip->choice != SEQID_GENBANK) return OM_MSG_RET_ERROR;
  tsip = (TextSeqIdPtr) sip->data.ptrvalue;
  if (tsip == NULL || StringHasNoText (tsip->accession)) return OM_MSG_RET_ERROR;

  if (smartfetchcmd == NULL) {
    if (GetAppParam ("SEQUIN", "SMART", "FETCHSCRIPT", NULL, cmmd, sizeof (cmmd))) {
    	smartfetchcmd = StringSaveNoNull (cmmd);
    }
  }
  if (smartfetchcmd == NULL) return OM_MSG_RET_ERROR;

  TmpNam (path);

  sprintf (cmmd, "csh %s %s > %s", smartfetchcmd, tsip->accession, path);
  system (cmmd);
  fp = FileOpen (path, "r");
  if (fp == NULL) {
    FileRemove (path);
    return OM_MSG_RET_ERROR;
  }
  dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, &entityID, FALSE, FALSE, TRUE, FALSE);
  FileClose (fp);
  FileRemove (path);

  if (dataptr == NULL) return OM_MSG_RET_OK;

  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep == NULL) return OM_MSG_RET_ERROR;
  bsp = BioseqFindInSeqEntry (sip, sep);
  ompcp->output_data = (Pointer) bsp;
  ompcp->output_entityID = ObjMgrGetEntityIDForChoice (sep);
  return OM_MSG_RET_DONE;
}

static Boolean SmartFetchEnable (void)

{
  ObjMgrProcLoad (OMPROC_FETCH, smartfetchproc, smartfetchproc,
                  OBJ_SEQID, 0, OBJ_BIOSEQ, 0, NULL,
                  SmartBioseqFetchFunc, PROC_PRIORITY_DEFAULT);
  return TRUE;
}

static VoidPtr LIBCALLBACK SmartUserDataFree (VoidPtr Pointer)
{
    SMUserDataPtr sm_user_data = (SMUserDataPtr) Pointer;

    if(sm_user_data != NULL) {
        MemFree(sm_user_data->header);
        MemFree(sm_user_data);
    }

    return NULL;
}
static void SMCancelAllEdit(void)
{
    ObjMgrPtr      omp;
    ObjMgrDataPtr  tmp;
    OMUserDataPtr  omudp;
    Int2           num;
    SMUserDataPtr  sm_usr_data = NULL;
    Uint2 j;

    omp = ObjMgrGet ();
    num = omp->HighestEntityID;
    
    for(j = 1; j <= omp->HighestEntityID; j++) {
        if((omudp = ObjMgrGetUserData(j, 0,0, SMART_KEY)) != NULL) {
            if((sm_usr_data = 
                (SMUserDataPtr) omudp->userdata.ptrvalue) != NULL &&
               sm_usr_data->fd != 0) {
                sm_usr_data->header->status = SMStatClosed;
                SMSendMsgToClient(sm_usr_data);
                SmartUserDataFree(omudp->userdata.ptrvalue);                
                omudp->userdata.ptrvalue = NULL;

                /* Deleting all */
                ObjMgrSendMsg (OM_MSG_DEL, j, 0, 0);
            }
        }
    }
    
    return;
}

extern ForM smartBioseqViewForm;

static Int4 SMReadBioseqObj(VoidPtr data, CharPtr buffer, Int4 length, Int4 fd)
{
    AsnIoMemPtr    aimp;
    BaseFormPtr    bfp;
    VoidPtr        bio_data;
    Int2           handled; 
    Uint2          entityID = 0;
    OMUserDataPtr  omudp;
    SMMsgHeaderPtr header;
    Int4           headlen;
    BioseqPtr      bsp;
    BioseqSetPtr   bssp = NULL, bssp2;
    SeqEntryPtr    sep = NULL, sep1, sep2, oldsep;
    ObjMgrData     omdata;
    ObjMgrDataPtr  omdptop = NULL;
    Uint2          parenttype = 0;
    Pointer        parentptr = NULL;
    SMUserDataPtr  sm_user_data;
    Int4           bio_type;

    if(buffer == NULL || length < sizeof(SMMsgHeader))
        return -1; 
    
    /* Reading request header */
    
    headlen = sizeof(SMMsgHeader); 
    header = (SMMsgHeaderPtr) MemNew(headlen);
    MemCpy((Pointer) header, buffer, headlen);
    
    /* ----------------------- */

    switch(header->status) {
    case SMTaskEditBinary:
        aimp = AsnIoMemOpen("rb", 
                            (UcharPtr) buffer+headlen, length - headlen);
        break;
    case SMTaskEditText:
        aimp = AsnIoMemOpen("r", (UcharPtr) buffer+headlen, length - headlen);
        break;
    case SMTaskCancel:
        SMCancelAllEdit();
        return TRUE;
    default:
        Message(MSG_ERROR, "This request type (%d) is not implemented yet",
                header->status);
        return FALSE;    
    }

    AsnIoSetErrorMsg(aimp->aip, SMAsnErrorFunc);
    bio_type = header->format;

    switch (header->format) {
    case OBJ_SEQSUB:
        bio_data = (VoidPtr) SeqSubmitAsnRead(aimp->aip, NULL);
        break;
    case OBJ_SEQENTRY:
        bio_data = (VoidPtr) SeqEntryAsnRead(aimp->aip, NULL);
        if((sep = (SeqEntryPtr) bio_data) != NULL) {
            if (sep->choice == 1) {
                bio_type = OBJ_BIOSEQ;
            } else {
                bio_type = OBJ_BIOSEQSET;
            }
        }
        break;
    case OBJ_BIOSEQ:
        bio_data = (VoidPtr) BioseqAsnRead(aimp->aip, NULL);
        bsp = (BioseqPtr) bio_data;
        sep = SeqEntryNew ();
        sep->choice = 1;
        sep->data.ptrvalue = bsp;
        SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, sep);

        break;
    case OBJ_BIOSEQSET:

        aimp->aip->scan_for_start = TRUE;
        while ((bssp2 = BioseqSetAsnRead (aimp->aip, NULL)) != NULL) {
            if (sep == NULL) {
                sep2 = SeqEntryNew ();
                if (sep2 != NULL) {
                    sep2->choice = 2;
                    sep2->data.ptrvalue = bssp2;
                    SeqMgrSeqEntry (SM_BIOSEQSET, (Pointer) bssp2, sep2);
                    sep = sep2;
                    bssp = bssp2;
                    SaveSeqEntryObjMgrData (sep, &omdptop, &omdata);
                    GetSeqEntryParent (sep, &parentptr, &parenttype);
                }
            } else {
                for (sep1 = bssp->seq_set; sep1->next != NULL; 
                     sep1 = sep1->next) continue;
                sep1->next = bssp2->seq_set;
                bssp2->seq_set = NULL;
                BioseqSetFree (bssp2);
            }
        }
        if (sep != NULL) {
            SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
            RestoreSeqEntryObjMgrData (sep, omdptop, &omdata);
        }
        bio_data = (Pointer) bssp;
        break;
    default:
        Message(MSG_OK, "This datatype: %d is not implemented yet",
                header->format);
        AsnIoMemClose(aimp);  
        return FALSE;
    }
    
    sm_user_data = MemNew(sizeof(SMUserData));
    sm_user_data->fd = fd;
    sm_user_data->header = header;

    if(bio_data == NULL) {
        Message(MSG_ERROR, "Error parcing or processing BLOB");
        sm_user_data->header->status = SMStatClosed;
        SMSendMsgToClient(sm_user_data); 
        MemFree(sm_user_data->header);
        MemFree(sm_user_data);
    } else {

        if (smartBioseqViewForm != NULL) {
            bfp = (BaseFormPtr) GetObjectExtra (smartBioseqViewForm);
            if (bfp != NULL) {
                /*
                entityID = bfp->input_entityID;
                RemoveSeqEntryViewerEx (bfp->form, FALSE);
                ObjMgrSendMsg (OM_MSG_DEL, entityID, 0, 0);
                bfp->input_entityID = 0;
                */
                seqviewprocs.keepSmartViewerVisible = TRUE;
                SmartnetDoneFunc (bfp);
                seqviewprocs.keepSmartViewerVisible = FALSE;
            }
        }

        entityID = ObjMgrRegister (bio_type, bio_data);
        subtoolEntityID = entityID;

        sep = GetTopSeqEntryForEntityID (entityID);
        MySeqEntryToAsn3 (sep, TRUE, FALSE, TRUE);

        /* now instantiating protein titles */
        InstantiateProteinTitles (entityID, NULL);

        if((omudp = ObjMgrGetUserData (entityID, 0, 0, SMART_KEY)) != NULL) {
            SmartUserDataFree(omudp->userdata.ptrvalue);
        } else {
            omudp = ObjMgrAddUserData (entityID, 0, 0, SMART_KEY);
        }

        ObjMgrSetOptions (OM_OPT_FREE_IF_NO_VIEW, entityID);
        
        /* We will set dirty flag from header info */

        if(header->dirty & 0x01) {
            ObjMgrSetDirtyFlag (entityID, TRUE);
        } else {
            ObjMgrSetDirtyFlag (entityID, FALSE);
        }

        if (omudp != NULL) {
            omudp->userdata.ptrvalue = (VoidPtr) sm_user_data;
            omudp->messagefunc = SubtoolModeMsgFunc; /* ? */
            omudp->freefunc = SmartUserDataFree;
        }
 
        seqviewprocs.forceSeparateViewer = FALSE;
        SeqEntrySetScope (NULL);
        handled = GatherProcLaunch (OMPROC_VIEW, FALSE, entityID, 1,
                                    OBJ_BIOSEQ, 0, 0, OBJ_BIOSEQ, 0);
        ArrowCursor ();
        
        if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
            Message (MSG_FATAL, "Unable to launch viewer.");
            CleanupSequin ();
            return FALSE;
        } 
    }

    AsnIoMemClose(aimp);  
    return TRUE;
}

static Int4 SMWriteBioseqObj(VoidPtr bio_data, 
                             SMUserDataPtr sm_usr_data, 
                             VoidPtr data)
{
    ByteStorePtr    bsp;
    Nlm_BSUnitPtr   bsup;
    AsnIoBSPtr      aibp;
    CharPtr buffer;
    Int4 length, totlen, bytes =0;

    bsp = BSNew(1024);

    if(sm_usr_data->header->status == SMTaskEditBinary) {  
        aibp = AsnIoBSOpen("wb", bsp);
    } else {
        aibp = AsnIoBSOpen("w", bsp);
    }

    switch (sm_usr_data->header->format) {
    case OBJ_SEQSUB:
        SeqSubmitAsnWrite((SeqSubmitPtr) bio_data, aibp->aip, NULL);
        break;
    case OBJ_SEQENTRY:
        SeqEntryAsnWrite((SeqEntryPtr) bio_data, aibp->aip, NULL);
        break;
    case OBJ_BIOSEQ:
        BioseqAsnWrite((BioseqPtr) bio_data, aibp->aip, NULL);
        break;
    case OBJ_BIOSEQSET:
        BioseqSetAsnWrite((BioseqSetPtr) bio_data, aibp->aip, NULL);
        break;
    default:
        Message(MSG_OK, "This datatype: %d is not implemented yet");
        AsnIoBSClose(aibp);
        BSFree(bsp);
        return FALSE;
    }

    AsnIoBSClose(aibp);
    
    BSSeek(bsp, 0, SEEK_SET);
    
    length = BSLen(bsp);
    buffer = MemNew(length+1);
    BSRead(bsp, buffer, length);
    
    totlen = sizeof(SMMsgHeader) + length;

    if(SMWriteToClient(sm_usr_data->fd, 
                       (CharPtr) &totlen, sizeof(Int4)) != SMNoError) {
        Message(MSG_OK, "Write error. Errno = %d", errno);
        return FALSE;
    }
    
    if(SMWriteToClient(sm_usr_data->fd, (CharPtr) sm_usr_data->header, 
                       sizeof(SMMsgHeader)) != SMNoError) {
        Message(MSG_OK, "Write error. Errno = %d", errno);
        return FALSE;
    }

    if(SMWriteToClient(sm_usr_data->fd, buffer, length) != SMNoError) {
        Message(MSG_OK, "Write error. Errno = %d", errno);
        return FALSE;
    }

    BSFree(bsp);
    MemFree(buffer);
    return TRUE;
}
#endif

Int2 Main (void)

{
  AsnIoPtr       aip;
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  Pointer        dataptr = NULL;
  BtnActnProc    fetchProc;
  FILE           *fp;
  Int2           handled;
  SeqEntryPtr    oldsep;
  /*
  ObjMgrPtr      omp;
  OMProcControl  ompc;
  ObjMgrProcPtr  ompp;
  */
  OMUserDataPtr  omudp;
  PaneL          p;
  Int2           procval = OM_MSG_RET_NOPROC;
  CharPtr        ptr;
  SeqEntryPtr    sep;
  Int4           smartPort = 0; 
  Char           str [80];
  Int2           val;
  WindoW         w;
#ifdef WIN_MOTIF
  Int2           i;
  RecT           r;
#endif

  ErrSetFatalLevel (SEV_MAX);
  ErrClearOptFlags (EO_SHOW_USERSTR);
  ProcessUpdatesFirst (FALSE);

  UseLocalAsnloadDataAndErrMsg ();
  ErrPathReset ();

  ReadSettings ();

  sprintf (str, "Sequin Application Version %s", SEQUIN_APPLICATION);
  SEQUIN_VERSION = StringSave (str);

  ptr = "Standard Release";
  if (useEntrez || useBlast) {
    ptr = "Network Aware";
  }
  if (indexerVersion) {
    ptr = "Indexer Services";
  }
  if (genomeCenter != NULL) {
    ptr = "Genome Center";
  }
  if (indexerVersion) {
    sprintf (str, "%s [%s - %s]", ptr, date_of_compilation, time_of_compilation);
  } else {
    sprintf (str, "%s [%s]", ptr, date_of_compilation);
  }
  SEQUIN_SERVICES = StringSave (str);

  if (! indexerVersion) {
    ErrSetLogfile (NULL, 0);
  }

  w = FixedWindow (-50, -33, -10, -10, "Sequin", NULL);
  p = SimplePanel (w, AboutBoxWidth (), AboutBoxHeight (), DrawAbout);
  Show (w);
#ifdef WIN_MOTIF
  Select (w);
  ObjectRect (p, &r);
  Select (p);
  InsetRect (&r, 3, 3);
  InvalRect (&r);
#endif
  Update ();

  workbenchMode = FALSE;
  subtoolMode = FALSE;
  stdinMode = FALSE;
  bioseqsetMode = FALSE;
  binseqentryMode = FALSE;
  entrezMode = FALSE;
  nohelpMode = FALSE;
  subtoolDatatype = 0;
  subtoolEntityID = 0;
  leaveAsOldAsn = FALSE;

#ifdef WIN_MOTIF
  {{
    Nlm_Int4         argc = GetArgc();
    Nlm_CharPtr PNTR argv = GetArgv();
    for (i = 1;  i < argc;  i++)
      {
        if      (StringCmp (argv[i], "-s") == 0)
          subtoolMode = TRUE;
        else if (StringCmp (argv[i], "-w") == 0)
          workbenchMode = TRUE;
        else if (StringCmp (argv[i], "-x") == 0)
          stdinMode = TRUE;
        else if (StringCmp (argv[i], "-bse") == 0)
          binseqentryMode = TRUE;
        else if (StringCmp (argv[i], "-e") == 0)
          entrezMode = TRUE;
        else if (StringCmp (argv[i], "-h") == 0)
          nohelpMode = TRUE;
        /*
        else if (StringCmp (argv[i], "-udv") == 0)
          useUdv = TRUE;
        */
        else if (StringCmp (argv[i], "-gph") == 0)
          useNewGraphicView = TRUE;
        else if (StringCmp (argv[i], "-gc") == 0) {
            indexerVersion = FALSE;
            extraServices = TRUE;
            genomeCenter = StringSave ("genome center tag goes here");
        }
        if (StringCmp (argv[i], "-b") == 0) {
          bioseqsetMode = TRUE;
        } else if (StringCmp (argv[i], "-oldaln") == 0) {
          newAlignReader = FALSE;
        }
#ifdef USE_SMARTNET
        else if (StringCmp (argv[i], "-ds") == 0) {
          dirsubMode = TRUE;
          nohelpMode = TRUE;
        }
        else if (StringNCmp (argv[i], "-z", 2) == 0) {
          smartnetMode = TRUE;
          dirsubMode = TRUE;
          if(*(argv[i]+2) != NULLB)
            smartPort = atoi(argv[i]+2);
          else
            smartPort = SM_SERVER_PORT;
        }
#endif
      }
  }}
#endif

#ifdef WIN_MSWIN
  {{
    Nlm_Int2         i;
    Nlm_Int4         argc = GetArgc();
    Nlm_CharPtr PNTR argv = GetArgv();
    for (i = 1;  i < argc;  i++)
      {
        if (StringCmp (argv[i], "-x") == 0)
          stdinMode = TRUE;
        else if (StringCmp (argv[i], "-e") == 0)
          entrezMode = TRUE;
        else if (StringCmp (argv[i], "-h") == 0)
          nohelpMode = TRUE;
      }
  }}
#endif

  if (subtoolMode || smartnetMode) {
    Char  tmp [PATH_MAX];
    ProgramPath (tmp, sizeof (tmp));
    ptr = StringRChr (tmp, DIRDELIMCHR);
    if (ptr != NULL) {
      ptr++;
    }
    if (smartPort > 0) {
      sprintf (str, "- %s %s (%ld) [%s - %s]", ptr, SEQUIN_APPLICATION, (long) smartPort, date_of_compilation, time_of_compilation);
    } else {
      sprintf (str, "- %s %s [%s - %s]", ptr, SEQUIN_APPLICATION, date_of_compilation, time_of_compilation);
    }
    SetAppProperty ("SmartSequinTimeStampTitle", StringSave (str));
  }

  if (GetAppParam ("SEQUIN", "SETTINGS", "STDINMODE", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      stdinMode = TRUE;
    }
  }
  if (GetAppParam ("SEQUIN", "SETTINGS", "HIDEHELPFORM", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      nohelpMode = TRUE;
    }
  }

  WatchCursor ();

  SetTitle (w, "Loading parse tables");
  if (! AllObjLoad ()) {
    ArrowCursor ();
    Message (MSG_FATAL, "AllObjLoad failed");
    return 0;
  }
  if (! SubmitAsnLoad ()) {
    ArrowCursor ();
    Message (MSG_FATAL, "SubmitAsnLoad failed");
    return 0;
  }
  if (! objprojAsnLoad ()) {
    ArrowCursor ();
    Message (MSG_FATAL, "ProjectAsnLoad failed");
    return 0;
  }

  SetTitle (w, "Loading print templates");
  if (! PrintTemplateSetLoad ("objprt.prt")) {
    ArrowCursor ();
    Message (MSG_FATAL, "PrintTemplateSetLoad objprt.prt failed");
    return 0;
  }
  if (! PrintTemplateSetLoad ("asn2ff.prt")) {
    ArrowCursor ();
    Message (MSG_FATAL, "PrintTemplateSetLoad asn2ff.prt failed");
    return 0;
  }
  /*
  if (! PrintTemplateSetLoad ("makerpt.prt")) {
    ArrowCursor ();
    Message (MSG_FATAL, "PrintTemplateSetLoad makerpt.prt failed");
    return 0;
  }
  */

  SetTitle (w, "Loading sequence alphabet converter");
  if (! SeqCodeSetLoad ()) {
    ArrowCursor ();
    Message (MSG_FATAL, "SeqCodeSetLoad failed");
    return 0;
  }

  SetTitle (w, "Loading organism table");
  if (! LoadOrganismTable ()) {
    ArrowCursor ();
    Message (MSG_POSTERR, "LoadOrganismTable failed");
  }

  SetTitle (w, "Loading genetic code table");
  if (! GeneticCodeTableLoad ()) {
    ArrowCursor ();
    Message (MSG_FATAL, "GeneticCodeTableLoad failed");
    return 0;
  }

  SetTitle (w, "Loading feature definitions");
  if (! FeatDefSetLoad ()) {
    ArrowCursor ();
    Message (MSG_FATAL, "FeatDefSetLoad failed");
    return 0;
  }

  SetupGeneticCodes ();

  if (! SetupPrintOptions ()) {
    ArrowCursor ();
    Message (MSG_FATAL, "StdPrintOptionsNew failed");
    return 0;
  }

/*
#ifndef WIN16
  SetTitle (w, "Loading structure dictionary");
  if (OpenMMDBAPI ((POWER_VIEW ^ FETCH_ENTREZ), NULL)) {
    prgdDict = GetPRGDDictionary ();
    if (BiostrucAvail ()) {
      Cn3DWin_Entrez(NULL, useEntrez);
    }
  }
#endif
*/

  SetTitle (w, "Creating menus");
  SetupDesktop ();
  SetupCommonFonts ();

  InitSequinExtras ();

  VSMFileInit ();
  VSeqMgrInit (FALSE);
  WatchCursor ();

#ifdef USE_SMARTNET
  if (dirsubMode) {
    DirSubFetchEnable ();
    SmartFetchEnable ();
  }
#endif

/*#ifdef USE_ENTREZ*/
  if (useEntrez) {
    /* EntrezBioseqFetchEnable ("Sequin", FALSE); */
    ID1BioseqFetchEnable ("Sequin", FALSE);
  }
/*#endif*/

/*#ifdef USE_LOCAL*/
  if (useLocal) {
    LocalSeqFetchInit (FALSE);
  }
/*#endif*/

#ifdef WIN_MAC
  SetDeactivate (NULL, MacDeactProc);
  SetupMacMenus ();
#endif

  SetTitle (w, "Creating window");
  InitMuskStyles ();

  startupStyle = 0;
  if (GetSequinAppParam ("SETTINGS", "DEFAULTSTYLE", NULL, str, sizeof (str))) {
    if (StrToInt (str, &val) && val >= 0) {
      startupStyle = val;
      SetMuskCurrentSt (GetMuskStyleName (val));
    }
  }

  /* SequinCheckSocketsProc is called by SubtoolModeTimerProc,
  so that can safely override the Metronome call */

  Metronome (SequinCheckSocketsProc);

  subtoolTimerLimit = 100;
  subtoolTimerCount = 0;
  subtoolRecordDirty = FALSE;
  if (subtoolMode || smartnetMode) {
    if (GetSequinAppParam ("SETTINGS", "TIMERLIMIT", NULL, str, sizeof (str))) {
      if (StrToInt (str, &val) && val >= 0) {
        subtoolTimerLimit = val;
      }
    }
  }

/* -------------------------- SmartNet -------------------------- */
  if(smartnetMode) {
/*
    omp = ObjMgrGet();
    MemSet((Pointer)(&ompc), 0, sizeof(OMProcControl));
    ompc.input_entityID = 0;
    ompc.input_itemID = 0;
    ompc.input_itemtype = OBJ_BIOSEQ;
    ompc.input_data = NULL;
    ompp = NULL;
    while (procval != OM_MSG_RET_DONE &&
           (ompp = ObjMgrProcFindNext (omp, OMPROC_VIEW, OBJ_BIOSEQ, OBJ_BIOSEQ, ompp)) != NULL) {
      if (! ompp->subinputtype) {
        ompc.proc = ompp;
        procval = (*(ompp->func))((Pointer)&ompc);
        if (procval == OM_MSG_RET_ERROR) {
          ErrShow ();
        }
      }
    }
*/

#ifdef USE_SMARTNET
    Remove (w);
    ArrowCursor ();

    if(!SMListenRequests(NULL,
                         SMReadBioseqObj,
                         smartPort)) {
      Message(MSG_OK, "Cannot start accept thread");
      QuitProgram();
    }
    subtoolTimerCount = 0;
    subtoolRecordDirty = FALSE;
    Metronome (SubtoolModeTimerProc);
    ProcessEvents();

    /* No more code in Main() for smartnet Mode */
    return 0;

#endif
/* --------------------------------------------------------------- */

  } else if (subtoolMode || stdinMode || binseqentryMode) {
    Remove (w);
    ArrowCursor ();
    if (subtoolMode) {
      dataptr = SubtoolModeAsnTextFileRead ("stdin", &subtoolDatatype, &subtoolEntityID);
    } else if (binseqentryMode) {
      dataptr = NULL;
      aip = AsnIoOpen ("stdin", "rb");
      sep = SeqEntryAsnRead (aip, NULL);
      AsnIoClose (aip);
      if (sep != NULL) {
        if (sep->choice == 1) {
          subtoolDatatype = OBJ_BIOSEQ;
          dataptr = (Pointer) sep->data.ptrvalue;
        } else if (sep->choice == 2) {
          subtoolDatatype = OBJ_BIOSEQSET;
          dataptr = (Pointer) sep->data.ptrvalue;
        }
        if (dataptr != NULL) {
          subtoolEntityID = ObjMgrRegister (subtoolDatatype, dataptr);
        }
      }
    } else {
      fp = FileOpen ("stdin", "r");
      dataptr = ReadAsnFastaOrFlatFile (fp, &subtoolDatatype,  &subtoolEntityID, FALSE, FALSE, TRUE, FALSE);
      FileClose (fp);
      /*
      dataptr = ObjMgrGenericAsnTextFileRead ("stdin", &subtoolDatatype, &subtoolEntityID);
      */
    }
    if (dataptr != NULL && subtoolEntityID > 0) {
      if (subtoolDatatype == OBJ_SEQSUB || subtoolDatatype == OBJ_SEQENTRY ||
          subtoolDatatype == OBJ_BIOSEQ || subtoolDatatype == OBJ_BIOSEQSET) {
        WatchCursor ();
        sep = GetTopSeqEntryForEntityID (subtoolEntityID);
        if (sep == NULL) {
          sep = SeqEntryNew ();
          if (sep != NULL) {
            if (subtoolDatatype == OBJ_BIOSEQ) {
              bsp = (BioseqPtr) dataptr;
              sep->choice = 1;
              sep->data.ptrvalue = bsp;
              SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, sep);
            } else if (subtoolDatatype == OBJ_BIOSEQSET) {
              bssp = (BioseqSetPtr) dataptr;
              sep->choice = 2;
              sep->data.ptrvalue = bssp;
              SeqMgrSeqEntry (SM_BIOSEQSET, (Pointer) bssp, sep);
            } else {
              sep = SeqEntryFree (sep);
            }
          }
          sep = GetTopSeqEntryForEntityID (subtoolEntityID);
        }
        if (sep != NULL) {
          if (EntrezASN1Detected (sep)) {
            ErrPostEx (SEV_WARNING, 0, 0, "This record was retrieved from Entrez");
          }
          if (! leaveAsOldAsn) {
            if (subtoolMode) {
              MySeqEntryToAsn3 (sep, TRUE, FALSE, TRUE);

              /* now instantiating protein titles */
              InstantiateProteinTitles (subtoolEntityID, NULL);
            } else {
              MySeqEntryToAsn3 (sep, TRUE, FALSE, FALSE);
            }
          }
          if (subtoolMode) {
            if (sep->choice == 2 && sep->data.ptrvalue != NULL) {
              bssp = (BioseqSetPtr) sep->data.ptrvalue;
              if (bssp->_class != 7) {
                Message (MSG_OK, "WARNING: Converting from bioseq set of class %d", (int) bssp->_class);
              }
              bssp->_class = 7;
            }
            if (FileLength (SEQUIN_EDIT_BACK_FILE) > 0) {
              if (Message (MSG_YN, "Restore from backup?") == ANS_YES) {
                oldsep = RestoreFromFile (SEQUIN_EDIT_BACK_FILE);
                ReplaceSeqEntryWithSeqEntry (sep, oldsep, TRUE);
                subtoolEntityID = ObjMgrGetEntityIDForChoice (sep);
              }
            }
          }
        }
        seqviewprocs.forceSeparateViewer = TRUE;
        SeqEntrySetScope (NULL);
        handled = GatherProcLaunch (OMPROC_VIEW, FALSE, subtoolEntityID, 1,
                                    OBJ_BIOSEQ, 0, 0, OBJ_BIOSEQ, 0);
        ArrowCursor ();
        subtoolTimerCount = 0;
        subtoolRecordDirty = FALSE;
        if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
          Message (MSG_FATAL, "Unable to launch viewer.");
          CleanupSequin ();
          return 0;
        } else {
          SendHelpScrollMessage (helpForm, "Editing the Record", NULL);
        }
        ObjMgrSetOptions (OM_OPT_FREE_IF_NO_VIEW, subtoolEntityID);
        if (! subtoolMode) {
          ObjMgrSetDirtyFlag (subtoolEntityID, TRUE);
        }
        if (subtoolMode) {
          Metronome (SubtoolModeTimerProc);
          omudp = ObjMgrAddUserData (subtoolEntityID, 0, 0, 0);
          if (omudp != NULL) {
            omudp->messagefunc = SubtoolModeMsgFunc;
          }
          subtoolRecordDirty = TRUE;
        }
      } else {
        Message (MSG_FATAL, "Unable to recognize ASN.1 type %d", (int) subtoolDatatype);
        CleanupSequin ();
        return 0;
      }
    } else {
      Message (MSG_FATAL, "Unable to read ASN.1 file");
      CleanupSequin ();
      return 0;
    }
  } else if (workbenchMode) {
    Remove (w);
    ArrowCursor ();
  } else if (entrezMode) {
    Remove (w);
    ArrowCursor ();
  } else {
    if (! nohelpMode) {
      SetTitle (w, "Creating help window");
      if (helpForm == NULL) {
        helpForm = CreateHelpForm (-95, -5, "Sequin Help", "sequin.hlp",
                                   HideHelpForm, HelpActivateProc);
      }
    }
    SetTitle (w, "Creating initial window");
    fetchProc = NULL;
    if (allowDownload) {
      fetchProc = FetchFromNet;
    }
    if (genomeCenter != NULL) {
      startupForm = CreateStartupForm (-5, -67, "Welcome to Sequin",
                                       StartFa2htgs, StartPhrap, BuildContig,
                                       StartNew, ReadOld, fetchProc,
                                       ShowHelp, DoQuit, StartupActivateProc);
    } else {
      startupForm = CreateStartupForm (-5, -67, "Welcome to Sequin",
                                       NULL, NULL, NULL, StartNew, ReadOld, fetchProc,
                                       ShowHelp, DoQuit, StartupActivateProc);
    }
    globalFormatBlock.seqPackage = SEQ_PKG_SINGLE;
    globalFormatBlock.seqFormat = SEQ_FMT_FASTA;
    globalFormatBlock.numSeqs = 0;
    Remove (w);
    ArrowCursor ();
  }


  if (subtoolMode || stdinMode || binseqentryMode) {
  } else if (workbenchMode) {
  } else if (entrezMode) {
    MakeTermListForm ();
    if (termListForm != NULL) {
      Show (termListForm);
      Select (termListForm);
      Update ();
      MakeDocSumForm ();
      if (docSumForm != NULL) {
      } else {
        Message (MSG_FATAL, "Unable to create document window");
        CleanupSequin ();
        return 0;
      }
    } else {
      Message (MSG_FATAL, "Unable to create term list window");
      CleanupSequin ();
      return 0;
    }
  } else if (startupForm != NULL) {
    Show (startupForm);
    Select (startupForm);
    if (! nohelpMode) {
      Show (helpForm);
      SendHelpScrollMessage (helpForm, "Introduction", NULL);
    }
    Update ();
    initSubmitForm = CreateInitSubmitterForm (-5, -67, "Submitting Authors",
                                              GetFormat, BackToStartup,
                                              SubmitBlockActivateProc);
    formatForm = CreateFormatForm (-5, -67, "Sequence Format",
                                   GetOrgAndSeq, BackToSubmitter, FormatActivateProc);
  } else {
    Message (MSG_FATAL, "Unable to create window.");
    CleanupSequin ();
    return 0;
  }

#if defined(WIN_MAC)  ||  defined(WIN_MSWIN)
  RegisterDropProc (SequinOpenMimeFile);
  RegisterResultProc (SequinOpenResultFile);
#endif

  if (workbenchMode) {
    VSeqMgrRun ("NCBI Desktop", "Sequin Workbench");
  } else {
    ProcessEvents ();
  }

  WatchCursor ();
  val = GetMuskCurrentSt ();
  if (val < 0) {
    val = 0;
  }
  if (val >= GetMuskTotalSt ()) {
    val = 0;
  }
  if (val != startupStyle) {
    if (val > 0) {
      sprintf (str, "%d", (int) val);
      SetSequinAppParam ("SETTINGS", "DEFAULTSTYLE", str);
    } else {
      SetSequinAppParam ("SETTINGS", "DEFAULTSTYLE", "0");
    }
  }

  Remove (startupForm);
  Remove (initSubmitForm);
  Remove (formatForm);
  Remove (helpForm);
  ArrowCursor ();
  Update ();

  CleanupSequin ();

  return 0;
}

static void write_annot(SeqAlignPtr align)
{
        SeqAnnotPtr annot;
        AsnIoPtr aip;

        annot = SeqAnnotNew();
        annot->type = 2;
        annot->data = align;

        aip = AsnIoOpen("temp2.sat", "w");
        SeqAnnotAsnWrite(annot, aip, NULL);
        AsnIoClose(aip); 

        annot->data = NULL;
        SeqAnnotFree(annot);
}
