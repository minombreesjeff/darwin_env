/*   gbseqget.c
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
* File Name:  gbseqget.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   11/4/02
*
* $Revision: 6.13 $
*
* File Description:  Demo to fetch by accession, write GBSet XML
*
* Modifications:
* --------------------------------------------------------------------------
* ==========================================================================
*/

#include <ncbi.h>
#include <objall.h>
#include <objsset.h>
#include <objsub.h>
#include <objfdef.h>
#include <objgbseq.h>
#include <seqport.h>
#include <sequtil.h>
#include <sqnutils.h>
#include <subutil.h>
#include <tofasta.h>
#include <explore.h>
#include <ent2api.h>
#include <pmfapi.h>
#include <asn2gnbp.h>

static CharPtr ReadALine (
  CharPtr str,
  size_t size,
  FILE *fp
)

{
  Char     ch;
  CharPtr  ptr;
  CharPtr  rsult;

  if (str == NULL || size < 1 || fp == NULL) return NULL;
  *str = '\0';
  rsult = FileGets (str, size, fp);
  if (rsult != NULL) {
    ptr = str;
    ch = *ptr;
    while (ch != '\0' && ch != '\n' && ch != '\r') {
      ptr++;
      ch = *ptr;
    }
    *ptr = '\0';
  }
  return rsult;
}

typedef struct lookforids {
  Boolean isGED;
  Boolean isNTorNW;
  Boolean isNC;
  Boolean isTPA;
  Boolean isNuc;
  Boolean isProt;
} LookForIDs, PNTR LookForIDsPtr;

static void LookForSeqIDs (BioseqPtr bsp, Pointer userdata)

{
  LookForIDsPtr  lfip;
  SeqIdPtr       sip;
  TextSeqIdPtr   tsip;

  lfip = (LookForIDsPtr) userdata;
  if (ISA_na (bsp->mol)) {
    lfip->isNuc = TRUE;
  }
  if (ISA_aa (bsp->mol)) {
    lfip->isProt = TRUE;
  }
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    switch (sip->choice) {
      case SEQID_GENBANK :
      case SEQID_EMBL :
      case SEQID_DDBJ :
        lfip->isGED = TRUE;
        break;
      case SEQID_TPG :
      case SEQID_TPE :
      case SEQID_TPD :
        lfip->isTPA = TRUE;
        break;
      case SEQID_OTHER :
        tsip = (TextSeqIdPtr) sip->data.ptrvalue;
        if (tsip != NULL) {
          if (StringNCmp (tsip->accession, "NC_", 3) == 0) {
            lfip->isNC = TRUE;
          } else if (StringNCmp (tsip->accession, "NT_", 3) == 0) {
            lfip->isNTorNW = TRUE;
          } else if (StringNCmp (tsip->accession, "NW_", 3) == 0) {
            lfip->isNTorNW = TRUE;
          }
        }
        break;
      default :
        break;
    }
  }
}

static void LookForGEDetc (
  SeqEntryPtr topsep,
  BoolPtr isGED,
  BoolPtr isNTorNW,
  BoolPtr isNC,
  BoolPtr isTPA,
  BoolPtr isNuc,
  BoolPtr isProt
)

{
  LookForIDs  lfi;

  MemSet ((Pointer) &lfi, 0, sizeof (LookForIDs));
  VisitBioseqsInSep (topsep, (Pointer) &lfi, LookForSeqIDs);
  *isGED = lfi.isGED;
  *isNTorNW = lfi.isNTorNW;
  *isNC = lfi.isNC;
  *isTPA = lfi.isTPA;
  *isNuc = lfi.isNuc;
  *isProt = lfi.isProt;
}

static void DoSeqEntryToGnbk (
  SeqEntryPtr sep,
  FmtType fmt,
  XtraPtr extra
)

{
  CstType  cust = SHOW_TRANCRIPTION | SHOW_PEPTIDE;
  FlgType  flags = SHOW_FAR_TRANSLATION | SHOW_CONTIG_AND_SEQ | PRODUCE_OLD_GBSEQ;
  Boolean  isGED;
  Boolean  isNTorNW;
  Boolean  isNC;
  Boolean  isNuc;
  Boolean  isProt;
  Boolean  isTPA;
  LckType  locks = LOOKUP_FAR_COMPONENTS | LOOKUP_FAR_LOCATIONS | LOOKUP_FAR_PRODUCTS;

  LookForGEDetc (sep, &isGED, &isNTorNW, &isNC, &isTPA, &isNuc, &isProt);

  if (fmt == GENBANK_FMT && (! isNuc)) return;
  if (fmt == GENPEPT_FMT && (! isProt)) return;

  if (isNTorNW || isTPA) {
    flags |= ONLY_NEAR_FEATURES;
  } else if (isNC) {
    flags |= NEAR_FEATURES_SUPPRESS;
  }

  SeqEntryToGnbk (sep, NULL, fmt, ENTREZ_MODE, SEGMENT_STYLE,
                  flags, locks, cust, extra, NULL);
}

static void DoQuery (
  FILE *fp,
  FILE *dfp,
  XtraPtr extra,
  Boolean get_var,
  Boolean do_nuc,
  Boolean do_prot
)

{
  Entrez2BooleanReplyPtr  e2br;
  Entrez2IdListPtr        e2lp;
  Entrez2RequestPtr       e2rq;
  Entrez2ReplyPtr         e2ry;
  Int4                    flags = 0;
  Int4                    i;
  Char                    line [256];
  E2ReplyPtr              reply;
  SeqEntryPtr             sep;
  CharPtr                 str;
  Uint4                   uid;

  if (get_var) {
    flags = 1;
  }

  e2rq = EntrezCreateBooleanRequest (TRUE, FALSE, "Nucleotide", NULL, 0, 0, NULL, 0, 0);
  if (e2rq == NULL) return;

  EntrezAddToBooleanRequest (e2rq, NULL, ENTREZ_OP_LEFT_PAREN, NULL, NULL, NULL, 0, 0, NULL, NULL, TRUE, TRUE);
  EntrezAddToBooleanRequest (e2rq, NULL, ENTREZ_OP_LEFT_PAREN, NULL, NULL, NULL, 0, 0, NULL, NULL, TRUE, TRUE);

  str = ReadALine (line, sizeof (line), fp);
  if (! StringHasNoText (str)) {
    EntrezAddToBooleanRequest (e2rq, NULL, 0, "ACCN", str, NULL, 0, 0, NULL, NULL, TRUE, TRUE);
  }

  while (str != NULL) {
    if (! StringHasNoText (str)) {
      EntrezAddToBooleanRequest (e2rq, NULL, ENTREZ_OP_OR, NULL, NULL, NULL, 0, 0, NULL, NULL, TRUE, TRUE);
      EntrezAddToBooleanRequest (e2rq, NULL, 0, "ACCN", str, NULL, 0, 0, NULL, NULL, TRUE, TRUE);
    }
    str = ReadALine (line, sizeof (line), fp);
  }

  EntrezAddToBooleanRequest (e2rq, NULL, ENTREZ_OP_RIGHT_PAREN, NULL, NULL, NULL, 0, 0, NULL, NULL, TRUE, TRUE);
  EntrezAddToBooleanRequest (e2rq, NULL, ENTREZ_OP_AND, NULL, NULL, NULL, 0, 0, NULL, NULL, TRUE, TRUE);
  EntrezAddToBooleanRequest (e2rq, NULL, ENTREZ_OP_LEFT_PAREN, NULL, NULL, NULL, 0, 0, NULL, NULL, TRUE, TRUE);

  str = ReadALine (line, sizeof (line), dfp);
  if (! StringHasNoText (str)) {
    EntrezAddToBooleanRequest (e2rq, NULL, 0, "MDAT", str, NULL, 0, 0, NULL, NULL, TRUE, TRUE);
  }

  while (str != NULL) {
    if (! StringHasNoText (str)) {
      EntrezAddToBooleanRequest (e2rq, NULL, ENTREZ_OP_OR, NULL, NULL, NULL, 0, 0, NULL, NULL, TRUE, TRUE);
      EntrezAddToBooleanRequest (e2rq, NULL, 0, "MDAT", str, NULL, 0, 0, NULL, NULL, TRUE, TRUE);
    }
    str = ReadALine (line, sizeof (line), dfp);
  }

  EntrezAddToBooleanRequest (e2rq, NULL, ENTREZ_OP_RIGHT_PAREN, NULL, NULL, NULL, 0, 0, NULL, NULL, TRUE, TRUE);
  EntrezAddToBooleanRequest (e2rq, NULL, ENTREZ_OP_RIGHT_PAREN, NULL, NULL, NULL, 0, 0, NULL, NULL, TRUE, TRUE);

  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);

  if (e2ry == NULL) return;
  reply = e2ry->reply;
  if (reply == NULL || reply->choice != E2Reply_eval_boolean) return;
  e2br = EntrezExtractBooleanReply (e2ry);
  if (e2br == NULL) return;

  e2lp = e2br->uids;
  if (e2lp != NULL) {
    BSSeek (e2lp->uids, 0, SEEK_SET);
    for (i = 0; i < e2lp->num; i++) {
      uid = Nlm_BSGetUint4 (e2lp->uids);
      if (uid < 1) continue;

      sep = PubSeqSynchronousQuery (uid, 0, flags);
      if (sep == NULL) continue;

      if (do_nuc) {
        DoSeqEntryToGnbk (sep, GENBANK_FMT, extra);
      }
      if (do_prot) {
        DoSeqEntryToGnbk (sep, GENPEPT_FMT, extra);
      }

      SeqEntryFree (sep);
    }
  }

  Entrez2BooleanReplyFree (e2br);
}

static void ProcessAccession (
  CharPtr accn,
  XtraPtr extra,
  Boolean only_new,
  Boolean get_var,
  Boolean do_nuc,
  Boolean do_prot
)

{
  Char         ch;
  Int4         flags = 0;
  Int4         gi = 0;
  Char         id [41];
  Boolean      is_numeric = TRUE;
  Int4         newgi = 0;
  CharPtr      ptr;
  SeqEntryPtr  sep;
  SeqIdPtr     sip;
  Char         tmp [41];
  long         val;

  ptr = accn;
  ch = *ptr;
  while (ch != '\0' && is_numeric) {
    if (! IS_DIGIT (ch)) {
      is_numeric = FALSE;
    }
    ptr++;
    ch = *ptr;
  }

  if (is_numeric) {
    if (sscanf (accn, "%ld", &val) == 1) {
      gi = (Int4) val;
      if (gi < 1) return;
      if (only_new) {
        sip = GetSeqIdForGI (gi);
        if (sip != NULL) {
          SeqIdWrite (sip, tmp, PRINTID_TEXTID_ACC_VER, sizeof (tmp));
          SeqIdFree (sip);
          ptr = StringChr (tmp, '.');
          if (ptr != NULL) {
            *ptr = '\0';
            sip = SeqIdFromAccessionDotVersion (tmp);
            newgi = GetGIForSeqId (sip);
            SeqIdFree (sip);
            if (newgi == gi) return;
          }
        }
      }
    }
  } else {
    sip = SeqIdFromAccessionDotVersion (accn);
    gi = GetGIForSeqId (sip);
    SeqIdFree (sip);
    if (only_new) {
      sip = GetSeqIdForGI (gi);
      if (sip != NULL) {
        SeqIdWrite (sip, id, PRINTID_TEXTID_ACC_VER, sizeof (id));
        SeqIdFree (sip);
        if (StringICmp (accn, id) == 0) return;
      }
    }
  }
  if (gi < 1) return;

  if (get_var) {
    flags = 1;
  }
  sep = PubSeqSynchronousQuery (gi, 0, flags);
  if (sep == NULL) return;

  if (do_nuc) {
    DoSeqEntryToGnbk (sep, GENBANK_FMT, extra);
  }
  if (do_prot) {
    DoSeqEntryToGnbk (sep, GENPEPT_FMT, extra);
  }

  SeqEntryFree (sep);
}

#define i_argInputFile  0
#define d_argDateFile   1
#define o_argOutputFile 2
#define n_argNewRecords 3
#define v_argVariations 4
#define m_argMolecule   5

Args myargs [] = {
  {"Sequence File Name", "stdin", NULL, NULL,
    FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  {"Date List", NULL, NULL, NULL,
    TRUE, 'd', ARG_FILE_IN, 0.0, 0, NULL},
  {"Output File Name", "stdout", NULL, NULL,
    FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
  {"New Records Only", "F", NULL, NULL,
    TRUE, 'n', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Fetch SNP Variations", "F", NULL, NULL,
    TRUE, 'v', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Molecule (n Nucleotide, p Protein, b Both)", "n", NULL, NULL,
    FALSE, 'm', ARG_STRING, 0.0, 0, NULL},
};

NLM_EXTERN void AsnPrintNewLine PROTO((AsnIoPtr aip));

Int2 Main (void)

{
  AsnIoPtr    aip;
  AsnTypePtr  atp;
  FILE        *dfp = NULL;
  Boolean     do_nuc = FALSE;
  Boolean     do_prot = FALSE;
  XtraPtr     extra;
  FILE        *fp;
  GBSeq       gbsq;
  GBSet       gbst;
  Boolean     get_var;
  Char        line [256];
  Boolean     only_new;
  CharPtr     str;
  Char        xmlbuf [128];
  XtraBlock   xtra;

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
  if (! SeqCodeSetLoad ()) {
    Message (MSG_FATAL, "SeqCodeSetLoad failed");
    return 1;
  }
  if (! GeneticCodeTableLoad ()) {
    Message (MSG_FATAL, "GeneticCodeTableLoad failed");
    return 1;
  }
  if (! objgbseqAsnLoad ()) {
    Message (MSG_POSTERR, "objgbseqAsnLoad failed");
    return 1;
  }

  if (! GetArgs ("gbseqget", sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  fp = FileOpen (myargs [i_argInputFile].strvalue, "r");
  if (fp == NULL) {
    return 1;
  }

  if (! StringHasNoText (myargs [d_argDateFile].strvalue)) {
    dfp = FileOpen (myargs [d_argDateFile].strvalue, "r");
    if (dfp == NULL) {
      return 1;
    }
  }

  if (GetAppParam ("NCBI", "SETTINGS", "XMLPREFIX", NULL, xmlbuf, sizeof (xmlbuf))) {
    AsnSetXMLmodulePrefix (StringSave (xmlbuf));
  }

  MemSet ((Pointer) &xtra, 0, sizeof (XtraBlock));
  MemSet ((Pointer) &gbsq, 0, sizeof (GBSeq));
  xtra.gbseq = &gbsq;
  aip = AsnIoOpen (myargs [o_argOutputFile].strvalue, "wx");

  if (aip == NULL) {
    Message (MSG_POSTERR, "AsnIoOpen failed");
    FileClose (fp);
    return 1;
  }

  only_new = (Boolean) myargs [n_argNewRecords].intvalue;
  get_var = (Boolean) myargs [v_argVariations].intvalue;

  str = myargs [m_argMolecule].strvalue;
  if (StringICmp (str, "n") == 0) {
    do_nuc = TRUE;
  } else if (StringICmp (str, "p") == 0) {
    do_prot = TRUE;
  } else if (StringICmp (str, "b") == 0) {
    do_nuc = TRUE;
    do_prot = TRUE;
  } else {
    do_nuc = TRUE;
  }

  PubSeqFetchEnable ();

  xtra.aip = aip;
  atp = AsnLinkType (NULL, AsnFind ("GBSet"));
  xtra.atp = AsnLinkType (NULL, AsnFind ("GBSet.E"));
  if (atp == NULL || xtra.atp == NULL) {
    Message (MSG_POSTERR, "AsnLinkType or AsnFind failed");
    return 1;
  }
  extra = &xtra;
  MemSet ((Pointer) &gbst, 0, sizeof (GBSet));
  AsnOpenStruct (aip, atp, (Pointer) &gbst);

  if (dfp != NULL) {
    DoQuery (fp, dfp, extra, get_var, do_nuc, do_prot);
  } else {
    str = ReadALine (line, sizeof (line), fp);
    while (str != NULL) {
      if (! StringHasNoText (str)) {
        ProcessAccession (str, extra, only_new, get_var, do_nuc, do_prot);
      }
      str = ReadALine (line, sizeof (line), fp);
    }
  }

  AsnCloseStruct (aip, atp, NULL);
  AsnPrintNewLine (aip);
  AsnIoClose (aip);

  FileClose (dfp);
  FileClose (fp);

  PubSeqFetchDisable ();

  return 0;
}

