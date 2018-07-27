#include <ncbi.h>
#include <objall.h>
#include <objsset.h>
#include <objsub.h>
#include <objfdef.h>
#include <sequtil.h>
#include <edutil.h>
#include <sqnutils.h>
#include <explore.h>
#include <gather.h>
#include <tofasta.h>
#include <pmfapi.h>

static void ProcessAGI (
  Int4 uid,
  FILE *fp
)

{
  BioseqPtr    bsp;
  Int4         gi;
  SeqHistPtr   hist;
  MolInfoPtr   mip;
  SeqDescrPtr  sdp;
  SeqEntryPtr  sep;
  SeqIdPtr     sip;
  ValNode      vn;

  if (uid == 0 || fp == NULL) return;

  sep = PubSeqSynchronousQuery (uid, 0, 0);
  if (sep == NULL) return;

  MemSet ((Pointer) &vn, 0, sizeof (ValNode));
  vn.choice = SEQID_GI;
  vn.data.intvalue = uid;

  bsp = BioseqFind (&vn);
  if (bsp == NULL) {
    sep = SeqEntryFree (sep);
    return;
  }

  AssignIDsInEntity (0, OBJ_SEQENTRY, (Pointer) sep);

  sdp = GetNextDescriptorUnindexed (bsp, Seq_descr_molinfo, NULL);
  if (sdp != NULL) {
    mip = (MolInfoPtr) sdp->data.ptrvalue;
    if (mip != NULL) {
      if (mip->tech == MI_TECH_htgs_1 || mip->tech == MI_TECH_htgs_2) {
        BioseqToFasta (bsp, fp, ISA_na (bsp->mol));
        sep = SeqEntryFree (sep);
        return;
      }
    }
  }

  hist = bsp->hist;
  if (hist != NULL) {
    sip = hist->replace_ids;
    if (sip != NULL) {
      gi = 0;
      if (sip->choice == SEQID_GI) {
        gi = sip->data.intvalue;
      } else {
        gi = GetGIForSeqId (sip);
      }
      if (gi > 0) {
        sep = SeqEntryFree (sep);
        ProcessAGI (gi, fp);
        return;
      }
    }
  }

  SeqEntryFree (sep);
}

/* Args structure contains command-line arguments */

#define a_argInputAccn   0
#define o_argOutputFile  1

Args myargs [] = {
  {"Accession", NULL, NULL, NULL,
    FALSE, 'a', ARG_STRING, 0.0, 0, NULL},
  {"Output File Name", "stdout", NULL, NULL,
    FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
};

Int2 Main (void)

{
  Char      ch;
  CharPtr   accn;
  FILE      *fp;
  Boolean   isgi;
  SeqIdPtr  sip;
  CharPtr   ptr;
  Int4      uid;
  long      val;

  /* standard setup */

  ErrSetFatalLevel (SEV_MAX);
  ErrClearOptFlags (EO_SHOW_USERSTR);
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

  if (! GetArgs ("getunfin", sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  accn = myargs [a_argInputAccn].strvalue;
  if (StringHasNoText (accn)) {
    Message (MSG_FATAL, "Must have accession number");
    return 1;
  }

  fp = FileOpen (myargs [o_argOutputFile].strvalue, "w");
  if (fp == NULL) {
    Message (MSG_FATAL, "FileOpen failed for output file");
    return 1;
  }

  PubSeqFetchEnable ();

  ptr = accn;
  ch = *ptr;
  isgi = TRUE;

  while (ch != '\0') {
    if (! IS_DIGIT (ch)) {
      isgi = FALSE;
    }
    ptr++;
    ch = *ptr;
  }

  if (isgi) {
    sscanf (accn, "%ld", &val);
    uid = (Int4) val;
  } else {
    sip = SeqIdFromAccessionDotVersion (accn);
    uid = GetGIForSeqId (sip);
    SeqIdFree (sip);
  }

  ProcessAGI (uid, fp);

  FileClose (fp);

  PubSeqFetchDisable ();

  return 0;
}

