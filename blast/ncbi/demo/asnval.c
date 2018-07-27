/*   asnval.c
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
* File Name:  asnval.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   11/3/04
*
* $Revision: 1.11 $
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

#include <ncbi.h>
#include <objall.h>
#include <objsset.h>
#include <objsub.h>
#include <objfdef.h>
#include <seqport.h>
#include <sequtil.h>
#include <sqnutils.h>
#include <subutil.h>
#include <gather.h>
#include <explore.h>
#include <lsqfetch.h>
#include <valid.h>
#include <pmfapi.h>
#ifdef INTERNAL_NCBI_ASN2VAL
#include <accpubseq.h>
#endif

#define ASNVAL_APP_VER "1.3"

CharPtr ASNVAL_APPLICATION = ASNVAL_APP_VER;

static ValNodePtr  requested_uid_list = NULL;
static TNlmMutex   requested_uid_mutex = NULL;

static ValNodePtr  locked_bsp_list = NULL;
static TNlmMutex   locked_bsp_mutex = NULL;

static void AddUidToQueue (
  SeqIdPtr sip
)

{
  ValNodePtr  last = NULL, vnp;
  Int4        ret;
  Int4        uid;

  if (sip == NULL || sip->choice != SEQID_GI) return;
  uid = (Int4) sip->data.intvalue;
  if (uid < 1) return;

  ret = NlmMutexLockEx (&requested_uid_mutex);
  if (ret) {
    ErrPostEx (SEV_FATAL, 0, 0, "AddUidToQueue mutex failed [%ld]", (long) ret);
    return;
  }

  /* check against uids already in queue */

  last = NULL;
  for (vnp = requested_uid_list; vnp != NULL; vnp = vnp->next) {
    last = vnp;
    if ((Int4) vnp->data.intvalue == uid) break;
  }

  /* add uid to queue */

  if (vnp == NULL) {
    if (last != NULL) {
      vnp = ValNodeAddInt (&last, 0, uid);
      last = vnp;
    } else {
      requested_uid_list = ValNodeAddInt (NULL, 0, uid);
      last = requested_uid_list;
    }
  }

  NlmMutexUnlock (requested_uid_mutex);
}

static Int4 RemoveUidFromQueue (
  void
)

{
  Int4        ret, uid = 0;
  ValNodePtr  vnp;

  ret = NlmMutexLockEx (&requested_uid_mutex);
  if (ret) {
    ErrPostEx (SEV_FATAL, 0, 0, "RemoveUidFromQueue mutex failed [%ld]", (long) ret);
    return 0;
  }

  /* extract next requested uid from queue */

  if (requested_uid_list != NULL) {
    vnp = requested_uid_list;
    requested_uid_list = vnp->next;
    vnp->next = NULL;
    uid = (Int4) vnp->data.intvalue;
    ValNodeFree (vnp);
  }

  NlmMutexUnlock (requested_uid_mutex);

  return uid;
}

static void QueueFarSegments (SeqLocPtr slp)

{
  BioseqPtr   bsp;
  SeqLocPtr   loc;
  SeqIdPtr    sip;
  ValNodePtr  vnp;

  if (slp == NULL) return;

  sip = SeqLocId (slp);
  if (sip == NULL) {
    loc = SeqLocFindNext (slp, NULL);
    if (loc != NULL) {
      sip = SeqLocId (loc);
    }
  }
  if (sip == NULL) return;

  /* if packaged in record, no need to fetch it */

  if (BioseqFindCore (sip) != NULL) return;

  /* check against currently locked records */

  for (vnp = locked_bsp_list; vnp != NULL; vnp = vnp->next) {
    bsp = (BioseqPtr) vnp->data.ptrvalue;
    if (bsp == NULL) continue;
    if (SeqIdIn (sip, bsp->id)) return;
  }

  AddUidToQueue (sip);
}

static void QueueFarBioseqs (BioseqPtr bsp, Pointer userdata)

{
  DeltaSeqPtr  dsp;
  SeqLocPtr    slp = NULL;
  ValNode      vn;

  if (bsp == NULL) return;

  if (bsp->repr == Seq_repr_seg) {
    vn.choice = SEQLOC_MIX;
    vn.extended = 0;
    vn.data.ptrvalue = bsp->seq_ext;
    vn.next = NULL;
    while ((slp = SeqLocFindNext (&vn, slp)) != NULL) {
      if (slp != NULL && slp->choice != SEQLOC_NULL) {
        QueueFarSegments (slp);
      }
    }
  } else if (bsp->repr == Seq_repr_delta) {
    for (dsp = (DeltaSeqPtr) (bsp->seq_ext); dsp != NULL; dsp = dsp->next) {
      if (dsp->choice == 1) {
        slp = (SeqLocPtr) dsp->data.ptrvalue;
        if (slp != NULL && slp->choice != SEQLOC_NULL) {
          QueueFarSegments (slp);
        }
      }
    }
  }
}

static void AddBspToList (
  BioseqPtr bsp
)

{
  Int4        ret;
  ValNodePtr  vnp;

  if (bsp == NULL) return;

  ret = NlmMutexLockEx (&locked_bsp_mutex);
  if (ret) {
    ErrPostEx (SEV_FATAL, 0, 0, "AddBspToList mutex failed [%ld]", (long) ret);
    return;
  }

  vnp = ValNodeAddPointer (&locked_bsp_list, 0, (Pointer) bsp);

  NlmMutexUnlock (locked_bsp_mutex);
}

static ValNodePtr ExtractBspList (
  void
)

{
  Int4        ret;
  ValNodePtr  vnp;

  ret = NlmMutexLockEx (&locked_bsp_mutex);
  if (ret) {
    ErrPostEx (SEV_FATAL, 0, 0, "ExtractBspList mutex failed [%ld]", (long) ret);
    return NULL;
  }

  vnp = locked_bsp_list;
  locked_bsp_list = NULL;

  NlmMutexUnlock (locked_bsp_mutex);

  return vnp;
}

typedef struct valflags {
  Int2     severity;
  Int2     lowCutoff;
  Int2     highCutoff;
  CharPtr  errcode;
  Boolean  validateAlignments;
  Boolean  alignFindRemoteBsp;
  Boolean  doSeqHistAssembly;
  Boolean  farIDsInAlignments;
  Boolean  alwaysRequireIsoJTA;
  Boolean  farFetchCDSproducts;
  Boolean  farFetchMRNAproducts;
  Boolean  locusTagGeneralMatch;
  Boolean  validateIDSet;
  Boolean  batch;
  Boolean  binary;
  Boolean  compressed;
  Boolean  lock;
  Boolean  lookup;
  Boolean  useThreads;
  Boolean  usePUBSEQ;
  Int2     verbosity;
  Int2     type;
  CharPtr  outpath;
  FILE     *outfp;
  FILE     *logfp;
  Int4     num_errors;
  Int4     fatal_errors;
  Char     longest [64];
  time_t   worsttime;
  Int4     numrecords;
} ValFlagData, PNTR ValFlagPtr;

static VoidPtr DoAsyncLookup (
  VoidPtr arg
)

{
  BioseqPtr   bsp;
  Int4        uid;
  ValFlagPtr  vfp;
  ValNode     vn;

  vfp = (ValFlagPtr) arg;
  if (vfp == NULL) return NULL;

#ifdef INTERNAL_NCBI_ASN2VAL
  if (vfp->usePUBSEQ) {
    PUBSEQInit ();
  }
#endif

  MemSet ((Pointer) &vn, 0, sizeof (ValNode));

  uid = RemoveUidFromQueue ();
  while (uid > 0) {

    vn.choice = SEQID_GI;
    vn.data.intvalue = uid;
    vn.next = NULL;

    bsp = BioseqLockById (&vn);
    if (bsp != NULL) {
      AddBspToList (bsp);
    }

    uid = RemoveUidFromQueue ();
  }

#ifdef INTERNAL_NCBI_ASN2VAL
  if (vfp->usePUBSEQ) {
    PUBSEQFini ();
  }
#endif

  return NULL;
}

#define NUM_ASYNC_LOOKUP_THREADS 5

static void ProcessAsyncLookups (
  ValFlagPtr vfp
)

{
  Int2        i;
  VoidPtr     status;
  TNlmThread  thds [NUM_ASYNC_LOOKUP_THREADS];

  /* spawn several threads for individual BioseqLockById requests */

  for (i = 0; i < NUM_ASYNC_LOOKUP_THREADS; i++) {
    thds [i] = NlmThreadCreate (DoAsyncLookup, (Pointer) vfp);
  }

  /* wait for all fetching threads to complete */

  for (i = 0; i < NUM_ASYNC_LOOKUP_THREADS; i++) {
    NlmThreadJoin (thds [i], &status);
  }
}

static ValNodePtr AsyncLockFarComponents (
  SeqEntryPtr sep,
  ValFlagPtr vfp
)

{
  BioseqPtr    bsp;
  ValNodePtr   bsplist = NULL, sublist, vnp;
  SeqEntryPtr  oldsep;

  if (sep == NULL || vfp == NULL) return NULL;
  oldsep = SeqEntrySetScope (sep);

  /* add far uids to queue */

  VisitBioseqsInSep (sep, NULL, QueueFarBioseqs);

  /* fetching from uid list using several threads */

  ProcessAsyncLookups (vfp);

  sublist = ExtractBspList ();

  /* take list, look for seg or delta, recurse */

  while (sublist != NULL) {
    for (vnp = sublist; vnp != NULL; vnp = vnp->next) {
      bsp = (BioseqPtr) vnp->data.ptrvalue;
      if (bsp == NULL) continue;
      QueueFarBioseqs (bsp, NULL);
    }

    ValNodeLink (&bsplist, sublist);
    sublist = NULL;

    ProcessAsyncLookups (vfp);

    sublist = ExtractBspList ();
  }

  SeqEntrySetScope (oldsep);
  return bsplist;
}


static void LookupSegments (
  SeqLocPtr slp,
  SeqIdPtr sip,
  Pointer userdata
)

{
  BoolPtr    nonGi;
  SeqLocPtr  loc;

  if (slp == NULL && sip == NULL) return;
  if (userdata == NULL) return;
  nonGi = (BoolPtr) userdata;

  if (sip == NULL) {
    sip = SeqLocId (slp);
    if (sip == NULL) {
      loc = SeqLocFindNext (slp, NULL);
      if (loc != NULL) {
        sip = SeqLocId (loc);
      }
    }
  }
  if (sip == NULL) return;

  if (sip->choice != SEQID_GI) {
    *nonGi = TRUE;
  }
}

static void LookupBioseqs (
  BioseqPtr bsp,
  Pointer userdata
)

{
  DeltaSeqPtr  dsp;
  SeqLocPtr    slp = NULL;
  ValNode      vn;

  if (bsp == NULL) return;

  if (bsp->repr == Seq_repr_seg) {
    vn.choice = SEQLOC_MIX;
    vn.extended = 0;
    vn.data.ptrvalue = bsp->seq_ext;
    vn.next = NULL;
    while ((slp = SeqLocFindNext (&vn, slp)) != NULL) {
      if (slp != NULL && slp->choice != SEQLOC_NULL) {
        LookupSegments (slp, NULL, userdata);
      }
    }
  } else if (bsp->repr == Seq_repr_delta) {
    for (dsp = (DeltaSeqPtr) (bsp->seq_ext); dsp != NULL; dsp = dsp->next) {
      if (dsp->choice == 1) {
        slp = (SeqLocPtr) dsp->data.ptrvalue;
        if (slp != NULL && slp->choice != SEQLOC_NULL) {
          LookupSegments (slp, NULL, userdata);
        }
      }
    }
  }
}

static void AccnListLookupFarSeqIDs (
  SeqEntryPtr sep
)

{
  Boolean      nonGi = FALSE;
  SeqEntryPtr  oldsep;

  if (sep == NULL) return;

  oldsep = SeqEntrySetScope (sep);

  VisitBioseqsInSep (sep, (Pointer) &nonGi, LookupBioseqs);

  SeqEntrySetScope (oldsep);

  if (nonGi) {
    LookupFarSeqIDs (sep, TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);
  }
}

static ValNodePtr DoLockFarComponents (
  SeqEntryPtr sep,
  ValFlagPtr vfp
)

{
  ValNodePtr  rsult;
  time_t      start_time, stop_time;

  start_time = GetSecs ();

  if (NlmThreadsAvailable () && vfp->useThreads) {
    rsult = AsyncLockFarComponents (sep, vfp);
  } else if (vfp->useThreads) {
    Message (MSG_POST, "Threads not available in this executable");
    rsult = LockFarComponents (sep);
  } else {
    rsult = LockFarComponents (sep);
  }

  stop_time = GetSecs ();

  return rsult;
}

static Boolean DeltaLitOnly (
  BioseqPtr bsp
)

{
  ValNodePtr  vnp;

  if (bsp == NULL || bsp->repr != Seq_repr_delta) return FALSE;
  for (vnp = (ValNodePtr)(bsp->seq_ext); vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == 1) return FALSE;
  }
  return TRUE;
}

static Boolean SegHasParts (
  BioseqPtr bsp
)

{
  BioseqSetPtr  bssp;
  SeqEntryPtr   sep;

  if (bsp == NULL || bsp->repr != Seq_repr_seg) return FALSE;
  sep = bsp->seqentry;
  if (sep == NULL) return FALSE;
  sep = sep->next;
  if (sep == NULL || (! IS_Bioseq_set (sep))) return FALSE;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp != NULL && bssp->_class == BioseqseqSet_class_parts) return TRUE;
  return FALSE;
}

static CharPtr severityLabel [] = {
  "NONE", "INFO", "WARN", "ERROR", "REJECT", "FATAL", "MAX", NULL
};

static CharPtr compatSeverityLabel [] = {
  "NONE", "NOTE: valid", "WARNING: valid", "ERROR: valid", "REJECT: valid", "FATAL: valid", "MAX", NULL
};

typedef struct vcdaa {
  FILE     *ofp;
  Int2     verbosity;
  Int2     lowCutoff;
  Int2     highCutoff;
  CharPtr  errcode;
} VCData, PNTR VCPtr;

static void LIBCALLBACK ValidCallback (
  ErrSev severity,
  int errcode,
  int subcode,
  CharPtr accession,
  CharPtr message,
  CharPtr objtype,
  CharPtr label,
  CharPtr context,
  CharPtr location,
  CharPtr product,
  Pointer userdata
)

{
  Char     buf [256];
  CharPtr  catname, errname;
  FILE     *fp;
  VCPtr    vcp;

  vcp = (VCPtr) userdata;
  if (vcp == NULL) return;
  fp = vcp->ofp;
  if (fp == NULL) return;

  if (severity < SEV_NONE || severity > SEV_MAX) {
    severity = SEV_MAX;
  }

  if (severity < vcp->lowCutoff || severity > vcp->highCutoff) return;

  catname = GetValidCategoryName (errcode);
  errname = GetValidErrorName (errcode, subcode);

  if (catname == NULL) {
    catname = "?";
  }
  if (errname == NULL) {
    errname = "?";
  }

  if (StringDoesHaveText (vcp->errcode)) {
    if (StringICmp (vcp->errcode, errname) != 0) return;
  }

  if (accession == NULL) {
    accession = "";
  }
  if (message == NULL) {
    message = "";
  }
  if (objtype == NULL) {
    objtype = "";
  }
  if (label == NULL) {
    label = "";
  }

  if (vcp->verbosity == 1) {

    fprintf (fp, "%s [%s.%s] %s %s: %s",
             compatSeverityLabel [severity],
             catname, errname, message, objtype, label);
    if (location != NULL) {
      fprintf (fp, " %s", location);
    }
    if (context != NULL) {
      fprintf (fp, " %s", context);
    }
    if (product != NULL) {
      fprintf (fp, " -> %s", product);
    }
    fprintf (fp, "\n");

  } else if (vcp->verbosity == 2) {

    StringCpy (buf, accession);
    StringCat (buf, "                    ");
    buf [15] = '\0';

    StringCat (buf, severityLabel [severity]);
    StringCat (buf, "                    ");
    buf [30] = '\0';

    StringCat (buf, catname);
    StringCat (buf, "_");
    StringCat (buf, errname);

    fprintf (fp, "%s\n", buf);

  } else if (vcp->verbosity == 3) {

    fprintf (fp, "%s\t%s\t%s_%s\n",
             accession, severityLabel [severity],
             catname, errname);

  }
}

static void DoValidation (
  SeqEntryPtr sep,
  ValFlagPtr vfp,
  FILE *ofp
)

{
  Int2            i;
  VCData          vcd;
  ValidStructPtr  vsp;

  if (vfp == NULL) return;

  vsp = ValidStructNew ();
  if (vsp == NULL) return;

  MemSet ((Pointer) &vcd, 0, sizeof (VCData));

  vsp->useSeqMgrIndexes = TRUE;

  vsp->cutoff = vfp->lowCutoff;
  vsp->validateAlignments = vfp->validateAlignments;
  vsp->doSeqHistAssembly = vfp->doSeqHistAssembly;
  vsp->farIDsInAlignments = vfp->farIDsInAlignments;
  vsp->alwaysRequireIsoJTA = vfp->alwaysRequireIsoJTA;
  vsp->farFetchCDSproducts = vfp->farFetchCDSproducts;
  vsp->farFetchMRNAproducts = vfp->farFetchMRNAproducts;
  vsp->locusTagGeneralMatch = vfp->locusTagGeneralMatch;
  vsp->validateIDSet = vfp->validateIDSet;

  if (ofp == NULL && vfp->outfp != NULL) {
    ofp = vfp->outfp;
  }
  if (ofp != NULL) {
    vcd.ofp = ofp;
    vcd.verbosity = vfp->verbosity;
    vcd.lowCutoff = vfp->lowCutoff;
    vcd.highCutoff = vfp->highCutoff;
    vcd.errcode = vfp->errcode;
    vsp->errfunc = ValidCallback;
    vsp->userdata = (Pointer) &vcd;
    vsp->convertGiToAccn = FALSE;
  }

  ValidateSeqEntry (sep, vsp);

  for (i = 0; i <= 4; i++) {
    vfp->num_errors += vsp->errors [i];
    if (i >= vfp->severity) {
      vfp->fatal_errors += vsp->errors [i];
    }
  }

  ValidStructFree (vsp);
}

static void ProcessSingleRecord (
  CharPtr directory,
  CharPtr base,
  CharPtr suffix,
  ValFlagPtr vfp
)

{
  AsnIoPtr       aip;
  BioseqPtr      bsp;
  ValNodePtr     bsplist;
  BioseqSetPtr   bssp;
  Char           buf [64], file [FILENAME_MAX], path [PATH_MAX];
  Pointer        dataptr = NULL;
  Uint2          datatype, entityID = 0;
  FILE           *fp, *ofp = NULL;
  SeqEntryPtr    fsep, sep;
  time_t         starttime, stoptime;

  if (vfp == NULL) return;

  if (base == NULL) {
    base = "";
  }
  if (suffix == NULL) {
    suffix = "";
  }
  StringNCpy_0 (path, directory, sizeof (path));
  sprintf (file, "%s%s", base, suffix);
  FileBuildPath (path, NULL, file);

  if (StringHasNoText (path)) return;

  if (vfp->type == 1) {
    fp = FileOpen (path, "r");
    if (fp == NULL) {
      Message (MSG_POSTERR, "Failed to open '%s'", path);
      return;
    }

    dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, FALSE, FALSE, FALSE);

    FileClose (fp);

    entityID = ObjMgrRegister (datatype, dataptr);

  } else if (vfp->type >= 2 && vfp->type <= 5) {
    aip = AsnIoOpen (path, vfp->binary? "rb" : "r");
    if (aip == NULL) {
      Message (MSG_POSTERR, "AsnIoOpen failed for input file '%s'", path);
      return;
    }

    switch (vfp->type) {
      case 2 :
        dataptr = (Pointer) SeqEntryAsnRead (aip, NULL);
        datatype = OBJ_SEQENTRY;
        break;
      case 3 :
        dataptr = (Pointer) BioseqAsnRead (aip, NULL);
        datatype = OBJ_BIOSEQ;
        break;
      case 4 :
        dataptr = (Pointer) BioseqSetAsnRead (aip, NULL);
        datatype = OBJ_BIOSEQSET;
        break;
      case 5 :
        dataptr = (Pointer) SeqSubmitAsnRead (aip, NULL);
        datatype = OBJ_SEQSUB;
        break;
      default :
        break;
    }

    AsnIoClose (aip);

    entityID = ObjMgrRegister (datatype, dataptr);

  } else {
    Message (MSG_POSTERR, "Input format type '%d' unrecognized", (int) vfp->type);
    return;
  }

  if (entityID < 1 || dataptr == NULL) {
    Message (MSG_POSTERR, "Data read failed for input file '%s'", path);
    return;
  }

  if (datatype == OBJ_SEQSUB || datatype == OBJ_SEQENTRY ||
        datatype == OBJ_BIOSEQ || datatype == OBJ_BIOSEQSET) {

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

      starttime = GetSecs ();
      buf [0] = '\0';

      if (vfp->logfp != NULL) {
        fsep = FindNthBioseq (sep, 1);
        if (fsep != NULL && fsep->choice == 1) {
          bsp = (BioseqPtr) fsep->data.ptrvalue;
          if (bsp != NULL) {
            SeqIdWrite (bsp->id, buf, PRINTID_FASTA_LONG, sizeof (buf));
            fprintf (vfp->logfp, "%s\n", buf);
            fflush (vfp->logfp);
          }
        }
      }

      if (base == NULL) {
        base = "";
      }
      StringNCpy_0 (path, directory, sizeof (path));
      sprintf (file, "%s%s", base, ".val");
      FileBuildPath (path, NULL, file);

      if (vfp->outpath != NULL) {
        ErrSetLogfile (vfp->outpath, 0);
      } else if (vfp->verbosity == 0) {
        ErrSetLogfile (path, 0);
      } else if (vfp->outfp == NULL) {
        ofp = FileOpen (path, "w");
      }

      bsplist = NULL;
    
      if (vfp->lookup) {
        AccnListLookupFarSeqIDs (sep);
      }
      if (vfp->lock) {
        bsplist = DoLockFarComponents (sep, vfp);
      }

      DoValidation (sep, vfp, ofp);

      bsplist = UnlockFarComponents (bsplist);

      if (ofp != NULL) {
        FileClose (ofp);
      }

      stoptime = GetSecs ();
      if (stoptime - starttime > vfp->worsttime && StringDoesHaveText (buf)) {
        vfp->worsttime = stoptime - starttime;
        StringCpy (vfp->longest, buf);
      }
      (vfp->numrecords)++;
    }
  } else {
    Message (MSG_POSTERR, "Datatype %d not recognized", (int) datatype);
  }

  ObjMgrFree (datatype, dataptr);
}

static void ProcessMultipleRecord (
  CharPtr directory,
  CharPtr base,
  CharPtr suffix,
  ValFlagPtr vfp
)

{
  AsnIoPtr       aip;
  AsnModulePtr   amp;
  AsnTypePtr     atp, atp_bss, atp_desc, atp_se;
  BioseqPtr      bsp;
  ValNodePtr     bsplist;
  Char           buf [64], file [FILENAME_MAX], path [PATH_MAX], longest [64];
  FILE           *fp, *ofp = NULL;
  Int4           numrecords = 0;
  SeqEntryPtr    fsep, sep;
  ObjMgrPtr      omp;
  time_t         starttime, stoptime, worsttime;
#ifdef OS_UNIX
  Char           cmmd [256];
  CharPtr        gzcatprog;
  int            ret;
  Boolean        usedPopen = FALSE;
#endif

  if (vfp == NULL) return;

  if (base == NULL) {
    base = "";
  }
  if (suffix == NULL) {
    suffix = "";
  }
  StringNCpy_0 (path, directory, sizeof (path));
  sprintf (file, "%s%s", base, suffix);
  FileBuildPath (path, NULL, file);

  if (StringHasNoText (path)) return;

#ifndef OS_UNIX
  if (vfp->compressed) {
    Message (MSG_POSTERR, "Can only decompress on-the-fly on UNIX machines");
    return;
  }
#endif

  amp = AsnAllModPtr ();
  if (amp == NULL) {
    Message (MSG_POSTERR, "Unable to load AsnAllModPtr");
    return;
  }

  atp_bss = AsnFind ("Bioseq-set");
  if (atp_bss == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Bioseq-set");
    return;
  }

  atp_desc = AsnFind ("Bioseq-set.descr");
  if (atp_desc == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Bioseq-set.descr");
    return;
  }

  atp_se = AsnFind ("Bioseq-set.seq-set.E");
  if (atp_se == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Bioseq-set.seq-set.E");
    return;
  }

#ifdef OS_UNIX
  if (vfp->compressed) {
    gzcatprog = getenv ("NCBI_UNCOMPRESS_BINARY");
    if (gzcatprog != NULL) {
      sprintf (cmmd, "%s %s", gzcatprog, path);
    } else {
      ret = system ("gzcat -h >/dev/null 2>&1");
      if (ret == 0) {
        sprintf (cmmd, "gzcat %s", path);
      } else if (ret == -1) {
        Message (MSG_POSTERR, "Unable to fork or exec gzcat in ScanBioseqSetRelease");
        return;
      } else {
        ret = system ("zcat -h >/dev/null 2>&1");
        if (ret == 0) {
          sprintf (cmmd, "zcat %s", path);
        } else if (ret == -1) {
          Message (MSG_POSTERR, "Unable to fork or exec zcat in ScanBioseqSetRelease");
          return;
        } else {
          Message (MSG_POSTERR, "Unable to find zcat or gzcat in ScanBioseqSetRelease - please edit your PATH environment variable");
          return;
        }
      }
    }
    fp = popen (cmmd, /* vfp->binary? "rb" : */ "r");
    usedPopen = TRUE;
  } else {
    fp = FileOpen (path, vfp->binary? "rb" : "r");
  }
#else
  fp = FileOpen (path, vfp->binary? "rb" : "r");
#endif
  if (fp == NULL) {
    Message (MSG_POSTERR, "FileOpen failed for input file '%s'", path);
    return;
  }

  aip = AsnIoNew (vfp->binary? ASNIO_BIN_IN : ASNIO_TEXT_IN, fp, NULL, NULL, NULL);
  if (aip == NULL) {
    Message (MSG_ERROR, "AsnIoNew failed for input file '%s'", path);
    return;
  }

  atp = atp_bss;

  longest [0] = '\0';
  worsttime = 0;

  if (base == NULL) {
    base = "";
  }
  StringNCpy_0 (path, directory, sizeof (path));
  sprintf (file, "%s%s", base, ".val");
  FileBuildPath (path, NULL, file);

  if (vfp->outpath != NULL) {
    ErrSetLogfile (vfp->outpath, 0);
  } else if (vfp->verbosity == 0) {
    ErrSetLogfile (path, 0);
  } else if (vfp->outfp == NULL) {
    ofp = FileOpen (path, "w");
  }

  while ((atp = AsnReadId (aip, amp, atp)) != NULL) {
    if (atp == atp_se) {
      sep = SeqEntryAsnRead (aip, atp);
    
      if (sep != NULL) {
        starttime = GetSecs ();
        buf [0] = '\0';

        if (vfp->logfp != NULL) {
          fsep = FindNthBioseq (sep, 1);
          if (fsep != NULL && fsep->choice == 1) {
            bsp = (BioseqPtr) fsep->data.ptrvalue;
            if (bsp != NULL) {
              SeqIdWrite (bsp->id, buf, PRINTID_FASTA_LONG, sizeof (buf));
              fprintf (vfp->logfp, "%s\n", buf);
              fflush (vfp->logfp);
            }
          }
        }

        bsplist = NULL;
    
        if (vfp->lookup) {
          AccnListLookupFarSeqIDs (sep);
        }
        if (vfp->lock) {
          bsplist = DoLockFarComponents (sep, vfp);
        }

        DoValidation (sep, vfp, ofp);

        bsplist = UnlockFarComponents (bsplist);

        stoptime = GetSecs ();
        if (stoptime - starttime > worsttime && StringDoesHaveText (buf)) {
          worsttime = stoptime - starttime;
          StringCpy (longest, buf);
        }
        numrecords++;
      }

      SeqEntryFree (sep);
      omp = ObjMgrGet ();
      ObjMgrReapOne (omp);
      ObjMgrFreeCache (0);
    } else {
      AsnReadVal (aip, atp, NULL);
    }
  }

  if (ofp != NULL) {
    FileClose (ofp);
  }

  AsnIoFree (aip, FALSE);

#ifdef OS_UNIX
  if (usedPopen) {
    pclose (fp);
  } else {
    FileClose (fp);
  }
#else
  FileClose (fp);
#endif

  if (vfp->logfp != NULL && (! StringHasNoText (longest))) {
    fprintf (vfp->logfp, "Longest processing time %ld seconds on %s\n",
             (long) worsttime, longest);
    fprintf (vfp->logfp, "Total number of records %ld\n", (long) numrecords);
    fflush (vfp->logfp);
  }
}

static void ProcessOneRecord (
  CharPtr directory,
  CharPtr base,
  CharPtr suffix,
  ValFlagPtr vfp
)

{
  if (vfp == NULL) return;

  if (vfp->batch) {
    ProcessMultipleRecord (directory, base, suffix, vfp);
  } else {
    ProcessSingleRecord (directory, base, suffix, vfp);
  }
}

static void FileRecurse (
  CharPtr directory,
  CharPtr subdir,
  CharPtr suffix,
  Boolean dorecurse,
  ValFlagPtr vfp
)

{
  Char        path [PATH_MAX];
  CharPtr     ptr, str;
  ValNodePtr  head, vnp;

  /* get list of all files in source directory */

  head = DirCatalog (directory);

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == 0) {
      if (StringHasNoText (subdir) || StringStr (directory, subdir) != NULL) {
        str = (CharPtr) vnp->data.ptrvalue;
        if (! StringHasNoText (str)) {

          /* does filename have desired substring? */

          ptr = StringStr (str, suffix);
          if (ptr != NULL) {
            *ptr = '\0';

            /* process file that has desired suffix (usually .ent) */

            ProcessOneRecord (directory, str, suffix, vfp);
          }
        }
      }
    } else if (vnp->choice == 1 && dorecurse) {

      /* recurse into subdirectory */

      StringNCpy_0 (path, directory, sizeof (path));
      str = (CharPtr) vnp->data.ptrvalue;
      FileBuildPath (path, str, NULL);

      FileRecurse (path, str, suffix, dorecurse, vfp);
    }
  }

  /* clean up file list */

  ValNodeFreeData (head);
}

/* Args structure contains command-line arguments */

#define p_argInputPath     0
#define i_argInputFile     1
#define o_argOutputFile    2
#define x_argSuffix        3
#define u_argRecurse       4
#define R_argSeverity      5
#define Q_argLowCutoff     6
#define P_argHighCutoff    7
#define E_argOnlyThisErr   8
#define A_argAlignments    9
#define J_argIsoJta       10
#define Z_argRemoteCDS    11
#define M_argMatchTag     12
#define Y_argCheckOld     13
#define v_argVerbosity    14
#define a_argType         15
#define b_argBinary       16
#define c_argCompressed   17
#define r_argRemote       18
#define k_argLocalFetch   19
#define l_argLockFar      20
#define G_argGiLookup     21
#define T_argThreads      22
#define L_argLogFile      23

Args myargs [] = {
  {"Path to ASN.1 Files", NULL, NULL, NULL,
    TRUE, 'p', ARG_STRING, 0.0, 0, NULL},
  {"Single Input File", "stdin", NULL, NULL,
    TRUE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  {"Single Output File", NULL, NULL, NULL,
    TRUE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
  {"File Selection Substring", ".ent", NULL, NULL,
    TRUE, 'x', ARG_STRING, 0.0, 0, NULL},
  {"Recurse", "F", NULL, NULL,
    TRUE, 'u', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Severity for Error in Return Code", "4", "0", "4",
    FALSE, 'R', ARG_INT, 0.0, 0, NULL},
  {"Lowest Severity for Error to Show", "3", "0", "4",
    FALSE, 'Q', ARG_INT, 0.0, 0, NULL},
  {"Highest Severity for Error to Show", "4", "0", "4",
    FALSE, 'P', ARG_INT, 0.0, 0, NULL},
  {"Only Error Code to Show", NULL, NULL, NULL,
    TRUE, 'E', ARG_STRING, 0.0, 0, NULL},
  {"Validate Alignments", "F", NULL, NULL,
    TRUE, 'A', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Require ISO-JTA?", "F", NULL, NULL,
    TRUE, 'J', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Remote CDS Product Fetch", "F", NULL, NULL,
    TRUE, 'Z', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Match locus_tag against General ID", "F", NULL, NULL,
    TRUE, 'M', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Check Against Old IDs", "F", NULL, NULL,
    TRUE, 'Y', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Verbosity", "0", "0", "3",
    FALSE, 'v', ARG_INT, 0.0, 0, NULL},
  {"ASN.1 Type (a Any, e Seq-entry, b Bioseq, s Bioseq-set, m Seq-submit, t Batch Processing)", "a", NULL, NULL,
    TRUE, 'a', ARG_STRING, 0.0, 0, NULL},
  {"Bioseq-set is Binary", "F", NULL, NULL,
    TRUE, 'b', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Bioseq-set is Compressed", "F", NULL, NULL,
    TRUE, 'c', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Remote Fetching from ID", "F", NULL, NULL,
    TRUE, 'r', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Local Fetching", "F", NULL, NULL,
    TRUE, 'k', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Lock Components in Advance", "F", NULL, NULL,
    TRUE, 'l', ARG_BOOLEAN, 0.0, 0, NULL},
  {"GI Lookup from Accession", "F", NULL, NULL,
    TRUE, 'G', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Use Threads", "F", NULL, NULL,
    TRUE, 'T', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Log File", NULL, NULL, NULL,
    TRUE, 'L', ARG_FILE_OUT, 0.0, 0, NULL},
};

Int2 Main (void)

{
  Char         app [64], sfx [32];
  CharPtr      base, directory, logfile, outfile, ptr, str, suffix;
  Boolean      batch, binary, compressed, dorecurse,
               local, lock, lookup, remote, usethreads;
  time_t       run_time, start_time, stop_time;
  Int2         type = 0;
  ValFlagData  vfd;

  /* standard setup */

  ErrSetFatalLevel (SEV_MAX);
  ErrSetMessageLevel (SEV_MAX);
  ErrClearOptFlags (EO_SHOW_USERSTR);
  ErrSetLogfile ("stderr", ELOG_APPEND);
  ErrSetOpts (ERR_IGNORE, ERR_LOG_ON);

  UseLocalAsnloadDataAndErrMsg ();
  ErrPathReset ();

  if (! AllObjLoad ()) {
    Message (MSG_FATAL, "AllObjLoad failed");
    return 1;
  }
  if (! SubmitAsnLoad ()) {
    Message (MSG_FATAL, "SubmitAsnLoad failed");
    return 1;
  }
  if (! FeatDefSetLoad ()) {
    Message (MSG_FATAL, "FeatDefSetLoad failed");
    return 1;
  }
  if (! SeqCodeSetLoad ()) {
    Message (MSG_FATAL, "SeqCodeSetLoad failed");
    return 1;
  }
  if (! GeneticCodeTableLoad ()) {
    Message (MSG_FATAL, "GeneticCodeTableLoad failed");
    return 1;
  }

  /* process command line arguments */

  sprintf (app, "asnval %s", ASNVAL_APPLICATION);
  if (! GetArgs (app, sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  /* additional setup modifications */

  MemSet ((Pointer) &vfd, 0, sizeof (ValFlagData));

  directory = (CharPtr) myargs [p_argInputPath].strvalue;
  suffix = (CharPtr) myargs [x_argSuffix].strvalue;
  base = (CharPtr) myargs [i_argInputFile].strvalue;
  outfile = (CharPtr) myargs [o_argOutputFile].strvalue;
  dorecurse = (Boolean) myargs [u_argRecurse].intvalue;
  remote = (Boolean ) myargs [r_argRemote].intvalue;
  local = (Boolean) myargs [k_argLocalFetch].intvalue;
  lock = (Boolean) myargs [l_argLockFar].intvalue;
  lookup = (Boolean) myargs [G_argGiLookup].intvalue;
  usethreads = (Boolean) myargs [T_argThreads].intvalue;

  vfd.severity = (Int2) myargs [R_argSeverity].intvalue;
  vfd.lowCutoff = (Int2) myargs [Q_argLowCutoff].intvalue;
  vfd.highCutoff = (Int2) myargs [P_argHighCutoff].intvalue;
  vfd.errcode = (CharPtr) myargs [E_argOnlyThisErr].strvalue;
  vfd.validateAlignments = (Boolean) myargs [A_argAlignments].intvalue;
  vfd.doSeqHistAssembly = (Boolean) myargs [A_argAlignments].intvalue;
  vfd.farIDsInAlignments = (Boolean) myargs [A_argAlignments].intvalue;
  vfd.alwaysRequireIsoJTA = (Boolean) myargs [J_argIsoJta].intvalue;
  vfd.farFetchCDSproducts = (Boolean) myargs [Z_argRemoteCDS].intvalue;
  vfd.farFetchMRNAproducts = (Boolean) myargs [Z_argRemoteCDS].intvalue;
  vfd.locusTagGeneralMatch = (Boolean) myargs [M_argMatchTag].intvalue;
  vfd.validateIDSet = (Boolean) myargs [Y_argCheckOld].intvalue;

  vfd.verbosity = (Int2) myargs [v_argVerbosity].intvalue;

  batch = FALSE;
  binary = (Boolean) myargs [b_argBinary].intvalue;
  compressed = (Boolean) myargs [c_argCompressed].intvalue;

  str = myargs [a_argType].strvalue;
  if (StringICmp (str, "a") == 0) {
    type = 1;
  } else if (StringICmp (str, "e") == 0) {
    type = 2;
  } else if (StringICmp (str, "b") == 0) {
    type = 3;
  } else if (StringICmp (str, "s") == 0) {
    type = 4;
  } else if (StringICmp (str, "m") == 0) {
    type = 5;
  } else if (StringICmp (str, "t") == 0) {
    type = 1;
    batch = TRUE;
  } else {
    type = 1;
  }

  if ((binary || compressed) && (! batch)) {
    if (type == 1) {
      Message (MSG_FATAL, "-b or -c cannot be used without -t or -a");
      return 1;
    }
  }

  if (StringHasNoText (directory) && StringHasNoText (base)) {
    Message (MSG_FATAL, "Input path or input file must be specified");
    return 1;
  }

  logfile = (CharPtr) myargs [L_argLogFile].strvalue;

  start_time = GetSecs ();

  /* populate parameter structure */

  vfd.batch = batch;
  vfd.binary = binary;
  vfd.compressed = compressed;
  vfd.lock = lock;
  vfd.lookup = lookup;
  vfd.useThreads = usethreads;
  vfd.type = type;
  vfd.logfp = NULL;
  vfd.longest [0] = '\0';
  vfd.worsttime = 0;
  vfd.numrecords = 0;

  if (! StringHasNoText (outfile)) {
    if (vfd.verbosity == 0) {
      vfd.outpath = outfile;
    } else {
      vfd.outfp = FileOpen (outfile, "w");
      if (vfd.outfp == NULL) {
        Message (MSG_FATAL, "Unable to open single output file");
        return 1;
      }
    }
  }

  if (! StringHasNoText (logfile)) {
    vfd.logfp = FileOpen (logfile, "w");
    if (vfd.logfp == NULL) {
      Message (MSG_FATAL, "Unable to open log file");
      return 1;
    }
  }

  /* register fetch functions */

  if (remote) {
#ifdef INTERNAL_NCBI_ASN2VAL
    if (! PUBSEQBioseqFetchEnable ("asnval", FALSE)) {
      Message (MSG_POSTERR, "PUBSEQBioseqFetchEnable failed");
      return 1;
    }
    vfd.usePUBSEQ = TRUE;
    vfd.useThreads = FALSE;
#else
    PubSeqFetchEnable ();
#endif
    if (vfd.validateIDSet) {
      SeqMgrSetSeqIdSetFunc (GiRevHistLookupSeqIdSet);
    }
  }

  if (local) {
    LocalSeqFetchInit (FALSE);
  }

  /* recurse through all files within source directory or subdirectories */

  if (StringDoesHaveText (directory)) {

    FileRecurse (directory, NULL, suffix, dorecurse, &vfd);

  } else if (StringDoesHaveText (base)) {

    ptr = StringRChr (base, '.');
    sfx[0] = '\0';
    if (ptr != NULL) {
      StringNCpy_0 (sfx, ptr, sizeof (sfx));
      *ptr = '\0';
    }
    ProcessOneRecord (directory, base, sfx, &vfd);
  }

  stop_time = GetSecs ();
  run_time = stop_time - start_time;

  if (vfd.outfp != NULL) {
    FileClose (vfd.outfp);
  }

  if (vfd.logfp != NULL) {
    fprintf (vfd.logfp, "Finished in %ld seconds\n", (long) run_time);
    if (StringDoesHaveText (vfd.longest)) {
      fprintf (vfd.logfp, "Longest processing time %ld seconds on %s\n",
               (long) vfd.worsttime, vfd.longest);
      fprintf (vfd.logfp, "Total number of records %ld\n", (long) vfd.numrecords);
    }
    FileClose (vfd.logfp);
  }

  /* close fetch functions */

  if (local) {
    LocalSeqFetchDisable ();
  }

  if (remote) {
#ifdef INTERNAL_NCBI_ASN2VAL
    PUBSEQBioseqFetchDisable ();
#else
    PubSeqFetchDisable ();
#endif
  }

  if (vfd.fatal_errors > 0) return 1;

  return 0;
}

