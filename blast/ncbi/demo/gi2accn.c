/*   gi2accn.c
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
* File Name:  gi2accn.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   4/15/02
*
* $Revision: 6.7 $
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
#include <sequtil.h>
#include <gather.h>
#include <sqnutils.h>
#include <subutil.h>
#include <explore.h>
#include <pmfapi.h>
#include <ent2api.h>

static void ConvertGiToAccn (SeqIdPtr sip, Pointer userdata)

{
  Int4      gi;
  SeqIdPtr  newsip;

  if (sip == NULL) return;
  if (sip->choice != SEQID_GI) return;
  gi = sip->data.intvalue;
  newsip = GetSeqIdForGI (gi);
  if (newsip == NULL) return;
  if (newsip->choice == SEQID_GIBBSQ ||
      newsip->choice == SEQID_GIBBMT ||
      newsip->choice == SEQID_GI) {
    SeqIdFree (newsip);
    return;
  }
  SeqIdStripLocus (newsip);
  sip->choice = newsip->choice;
  sip->data.ptrvalue = newsip->data.ptrvalue;
  newsip->choice = SEQID_NOT_SET;
  newsip->data.ptrvalue = NULL;
  SeqIdFree (newsip);
}

static void UpdateAligns (
  SeqAlignPtr sap,
  Pointer userdata
)

{
  VisitSeqIdsInSeqAlign (sap, NULL, ConvertGiToAccn);
}

static Boolean IsSipMrna (SeqIdPtr sip, Int4Ptr gilist, Int4 count)

{
  BioseqPtr  bsp;
  Int4       gi = 0;
  Int4       L, R, mid;

  if (sip == NULL) return FALSE;
  bsp = BioseqFind (sip);
  if (bsp != NULL) return FALSE;
  if (sip->choice == SEQID_GI) {
    gi = (Int4) sip->data.intvalue;
  } else {
    gi = GetGIForSeqId (sip);
  }
  if (gi < 1) return FALSE;

  L = 0;
  R = count - 1;

  while (L < R) {
    mid = (L + R) / 2;
    if (gilist [mid] < gi) {
      L = mid + 1;
    } else {
      R = mid;
    }
  }

  if (gilist [R] == gi) return TRUE;

  return FALSE;
}

static Boolean IsMrnaAlignment (SeqAlignPtr align, Int4Ptr gilist, Int4 count)

{
  DenseDiagPtr  ddp;
  DenseSegPtr   dsp;
  SeqIdPtr      sip;
  StdSegPtr     ssp;
  SeqLocPtr     tloc;

  if (align == NULL) return FALSE;
  sip = NULL;
  if (align->segtype == 1) {
    ddp = (DenseDiagPtr) align->segs;
    if (ddp != NULL) {
      for (sip = ddp->id; sip != NULL; sip = sip->next) {
        if (IsSipMrna (sip, gilist, count)) return TRUE;
      }
    }
  } else if (align->segtype == 2) {
    dsp = (DenseSegPtr) align->segs;
    if (dsp != NULL) {
      for (sip = dsp->ids; sip != NULL; sip = sip->next) {
        if (IsSipMrna (sip, gilist, count)) return TRUE;
      }
    }
  } else if (align->segtype == 3) {
    ssp = (StdSegPtr) align->segs;
    if (ssp != NULL) {
      for (tloc = ssp->loc; tloc != NULL; tloc = tloc->next) {
        sip = SeqLocId (tloc);
        if (IsSipMrna (sip, gilist, count)) return TRUE;
      }
    }
  }
  return FALSE;
}

static SeqAnnotPtr MakeMrnaSeqAnnot (void)

{
  AnnotDescrPtr  adp;
  SeqAnnotPtr    annot;
  ObjectIdPtr    oip;
  UserFieldPtr   ufp;
  UserObjectPtr  uop;

  annot = SeqAnnotNew ();
  annot->type = 2;

  adp = ValNodeNew (NULL);
  adp->choice = Annot_descr_user;
  uop = UserObjectNew ();
  adp->data.ptrvalue = uop;
  oip = ObjectIdNew ();
  oip->str = StringSave ("Blast Type");
  ufp = UserFieldNew ();
  uop->type = oip;
  uop->data = ufp;
  oip = ObjectIdNew ();
  oip->str = StringSave ("BLASTN - mrna");
  ufp->label = oip;
  ufp->choice = 2;
  ufp->data.intvalue = 1;
  annot->desc = adp;

  adp = ValNodeNew (NULL);
  adp->choice = Annot_descr_user;
  uop = UserObjectNew ();
  adp->data.ptrvalue = uop;
  oip = ObjectIdNew ();
  oip->str = StringSave ("Hist Seqalign");
  ufp = UserFieldNew ();
  uop->type = oip;
  uop->data = ufp;
  oip = ObjectIdNew ();
  oip->str = StringSave ("Hist Seqalign");
  ufp->label = oip;
  ufp->choice = 4;
  ufp->data.boolvalue = TRUE;
  adp->next = annot->desc;
  annot->desc = adp;

  return annot;
}

static SeqAnnotPtr ExtractBlastMrna (SeqAlignPtr sap, Pointer PNTR prevlink, Int4Ptr gilist, Int4 count)

{
  SeqAnnotPtr  annot = NULL;
  SeqAlignPtr  next;

  while (sap != NULL) {
    next = sap->next;

    if (IsMrnaAlignment (sap, gilist, count)) {
      *prevlink = sap->next;
      sap->next = NULL;

      if (annot == NULL) {
        annot = MakeMrnaSeqAnnot ();
      }
      if (annot != NULL) {
        sap->next = annot->data;
        annot->data = sap;
      }

    } else {
      sap->idx.prevlink = prevlink;
      prevlink = (Pointer PNTR) &(sap->next);
    }

    sap = next;
  }

  return annot;
}

static void CountGIsInAligns (SeqIdPtr sip, Pointer userdata)

{
  Int4Ptr  countp;

  if (sip == NULL || sip->choice != SEQID_GI || userdata == NULL) return;
  countp = (Int4Ptr) userdata;
  (*countp)++;
}

typedef struct strgi {
  Int4     count;
  Int4Ptr  uidlist;
} UidData, PNTR UidDataPtr;

static void StoreGIsFromAligns (SeqIdPtr sip, Pointer userdata)

{
  UidDataPtr  udp;

  if (sip == NULL || sip->choice != SEQID_GI || userdata == NULL) return;
  udp = (UidDataPtr) userdata;
  udp->uidlist [udp->count] = (Int4) sip->data.intvalue;
  (udp->count)++;
}

static int LIBCALLBACK SortByInt4 (VoidPtr a, VoidPtr b)

{
  if (a == NULL || b == NULL) return 0;
  return (*(const Int4 *) a) - (*(const Int4 *) b);
}

static void FindBlastMrna (SeqAnnotPtr sap, Int4Ptr uidlist, Int4 count)

{
  SeqAnnotPtr             annot;
  Entrez2BooleanReplyPtr  e2br;
  Entrez2IdListPtr        e2id;
  Entrez2RequestPtr       e2rq;
  Entrez2ReplyPtr         e2ry;
  Int4                    i;
  Int4Ptr                 gilist;
  E2ReplyPtr              reply;

  e2rq = EntrezCreateBooleanRequest (TRUE, FALSE, "nucleotide", NULL, 0, 0, NULL, 0, 0);
  EntrezAddToBooleanRequest (e2rq, "biomol_mrna [PROP]", 0, NULL, NULL, NULL, 0, 0, NULL, NULL, TRUE, TRUE);
  EntrezAddToBooleanRequest (e2rq, NULL, ENTREZ_OP_AND, NULL, NULL, NULL, 0, 0, NULL, NULL, TRUE, TRUE);
  EntrezAddToBooleanRequest (e2rq, NULL, 0, NULL, NULL, NULL, 0, count, uidlist, NULL, TRUE, TRUE);
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry == NULL) return;
  reply = e2ry->reply;
  if (reply == NULL || reply->choice != E2Reply_eval_boolean) return;
  e2br = EntrezExtractBooleanReply (e2ry);
  if (e2br == NULL) return;
  count = e2br->count;
  if (count < 1) return;
  e2id = e2br->uids;
  if (e2id == NULL || e2id->num < 1 || e2id->uids == NULL) return;

  gilist = (Int4Ptr) MemNew (sizeof (Int4) * (e2id->num + 1));
  if (gilist != NULL) {

    BSSeek (e2id->uids, 0, SEEK_SET);
    for (i = 0; i < e2id->num; i++) {
      gilist [i] = Nlm_BSGetUint4 (e2id->uids);
    }
    HeapSort (gilist, e2id->num, sizeof (Int4), SortByInt4);

    annot = ExtractBlastMrna ((SeqAlignPtr) sap->data, (Pointer PNTR) &(sap->data), gilist, e2id->num);
    if (annot != NULL) {
      annot->next = sap->next;
      sap->next = annot;
    }

    MemFree (gilist);
  }

  Entrez2BooleanReplyFree (e2br);
}

static void ProcessBlastNR (SeqAnnotPtr sap)

{
  Int4     count = 0;
  Int4     gi;
  Int4     i;
  Int4     j;
  Int4     last;
  UidData  ud;
  Int4Ptr  uidlist;

  if (sap == NULL || sap->type != 2) return;

  VisitSeqIdsInSeqAnnot (sap, (Pointer) &count, CountGIsInAligns);
  if (count < 1) return;
  uidlist = (Int4Ptr) MemNew (sizeof (Int4) * (count + 1));
  if (uidlist == NULL) return;

  ud.count = 0;
  ud.uidlist = uidlist;
  VisitSeqIdsInSeqAnnot (sap, (Pointer) &ud, StoreGIsFromAligns);

  HeapSort (uidlist, count, sizeof (Int4), SortByInt4);

  /* unique sorted gi list */

  last = 0;
  for (i = 0, j = 0; i < count; i++) {
    gi = uidlist [i];
    if (gi != last) {
      uidlist [j] = gi;
      j++;
    }
    last = gi;
  }
  count = j;

  FindBlastMrna (sap, uidlist, count);

  MemFree (uidlist);
}

static void FindBlastNR (SeqAnnotPtr sap, Pointer userdata)

{
  AnnotDescrPtr  adp;
  ObjectIdPtr    oip;
  UserFieldPtr   ufp;
  UserObjectPtr  uop;

  if (sap == NULL || sap->type != 2) return;
  for (adp = sap->desc; adp != NULL; adp = adp->next) {
    if (adp->choice != Annot_descr_user) continue;
    for (uop = adp->data.ptrvalue; uop != NULL; uop = uop->next) {
      oip = uop->type;
      if (oip == NULL) continue;
      if (StringCmp (oip->str, "Blast Type") != 0) continue;
      ufp = uop->data;
      if (ufp == NULL) continue;
      oip = ufp->label;
      if (oip == NULL) continue;
      if (StringCmp (oip->str, "BLASTN - nr") != 0) continue;
      ProcessBlastNR (sap);
      oip->str = MemFree (oip->str);
      oip->str = StringSave ("BLASTN - nr minus mrna");
    }
  }
}

/* Args structure contains command-line arguments */

#define i_argInputFile     0
#define o_argOutputFile    1
#define c_argConvertGIs    2
#define x_argExtractMrnas  3

Args myargs [] = {
  {"Input File", "stdin", NULL, NULL,
    FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  {"Output File", "stdout", NULL, NULL,
    FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
  {"Convert GIs", "T", NULL, NULL,
    TRUE, 'c', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Extract mRNAs", "T", NULL, NULL,
    TRUE, 'x', ARG_BOOLEAN, 0.0, 0, NULL},
};

Int2 Main (void)

{
  AsnIoPtr     aip;
  Boolean      convertgis;
  Boolean      extractmrnas;
  CharPtr      infile, outfile;
  SeqEntryPtr  sep;

  /* standard setup */

  ErrSetFatalLevel (SEV_MAX);
  ErrClearOptFlags (EO_SHOW_USERSTR);
  UseLocalAsnloadDataAndErrMsg ();
  ErrPathReset ();

  /* finish resolving internal connections in ASN.1 parse tables */

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

  if (! GetArgs ("gi2accn", sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  infile = (CharPtr) myargs [i_argInputFile].strvalue;
  outfile = (CharPtr) myargs [o_argOutputFile].strvalue;
  convertgis = (Boolean) myargs [c_argConvertGIs].intvalue;
  extractmrnas = (Boolean) myargs [x_argExtractMrnas].intvalue;

  aip = AsnIoOpen (infile, "r");
  if (aip == NULL) {
    Message (MSG_FATAL, "AsnIoOpen input file failed");
    return 1;
  }

  sep = SeqEntryAsnRead (aip, NULL);
  AsnIoClose (aip);
  if (sep == NULL) {
    Message (MSG_FATAL, "SeqEntryAsnRead failed");
    return 1;
  }

  PubSeqFetchEnable ();

  LookupFarSeqIDs (sep, FALSE, FALSE, FALSE, TRUE, FALSE, FALSE);

  /* Extract mRNA hits from BLAST against nr */

  if (extractmrnas) {
    VisitAnnotsInSep (sep, NULL, FindBlastNR);
    DeleteMarkedObjects (0, OBJ_SEQENTRY, (Pointer) sep);
  }

  /* convert gi numbers to accession.version */

  if (convertgis) {
    VisitAlignmentsInSep (sep, NULL, UpdateAligns);
  }

  PubSeqFetchDisable ();

  BasicSeqEntryCleanup (sep);

  aip = AsnIoOpen (outfile, "w");
  if (aip == NULL) {
    Message (MSG_FATAL, "AsnIoOpen output file failed");
    return 1;
  }

  SeqEntryAsnWrite (sep, aip, NULL);
  AsnIoClose (aip);

  return 0;
}

