/*   snpsynon.c
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
* File Name:  snpsynon.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   5/18/00
*
* $Revision: 6.3 $
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
#include <tofasta.h>
#include <gather.h>
#include <explore.h>

static Pointer ReadOneFile (
  CharPtr directory,
  CharPtr base,
  CharPtr suffix,
  Uint2Ptr datatype
)

{
  Pointer  dataptr;
  Char     file [FILENAME_MAX], path [PATH_MAX];
  FILE*    fp;

  *datatype = 0;

  StringNCpy_0 (path, directory, sizeof (path));
  sprintf (file, "%s.%s", base, suffix);
  FileBuildPath (path, NULL, file);

  fp = FileOpen (path, "r");
  if (fp == NULL) return NULL;

  /* ReadAsnFastaOrFlatFile can read ASN.1, FASTA, and feature table formats */

  dataptr = ReadAsnFastaOrFlatFile (fp, datatype, NULL, FALSE, FALSE, TRUE, FALSE);

  FileClose (fp);

  return dataptr;
}

static FILE* CreateOneFile (
  CharPtr directory,
  CharPtr base,
  CharPtr suffix
)

{
  FILE  *fp;
  Char  file [FILENAME_MAX], path [PATH_MAX];

  StringNCpy_0 (path, directory, sizeof (path));
  sprintf (file, "%s.%s", base, suffix);
  FileBuildPath (path, NULL, file);

  fp = FileOpen (path, "w");
  return fp;
}

static Boolean FileAlreadyExists (
  CharPtr directory,
  CharPtr base,
  CharPtr suffix
)

{
  Char  file [FILENAME_MAX], path [PATH_MAX];

  StringNCpy_0 (path, directory, sizeof (path));
  sprintf (file, "%s.%s", base, suffix);
  FileBuildPath (path, NULL, file);

  return (Boolean) (FileLength (path) > 0);
}

static CharPtr xreftags [] = {
  "dbSNP", "dbSTS", "clone", "LocusID", "cdsModel",
  "mrnaModel", "contig", "seq", NULL
};

#define FINDDBXREFBUFSIZE 40

static Boolean FindDbxref (
  SeqFeatPtr sfp,
  Int2Ptr type,
  CharPtr label
)

{
  DbtagPtr     dbt;
  Int2         i;
  Char         numbuf [15];
  ObjectIdPtr  oip;
  ValNodePtr   vnp;

  if (type == NULL || label == NULL) return FALSE;
  *type = 0;
  *label = '\0';
  if (sfp == NULL) return FALSE;

  for (vnp = sfp->dbxref; vnp != NULL; vnp = vnp->next) {
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    for (i = 0; xreftags [i] != NULL; i++) {
      if (StringICmp (dbt->db, xreftags [i]) == 0) {
        oip = dbt->tag;
        if (oip != NULL) {
          if (oip->str != NULL) {
            StringNCpy_0 (label, oip->str, FINDDBXREFBUFSIZE);
          } else {
            sprintf (numbuf, "%ld", (long) oip->id);
            StringNCpy_0 (label, numbuf, FINDDBXREFBUFSIZE);
          }
          *type = i;
          return TRUE;
        }
      }
    }
  }

  return FALSE;
}

static void PrintSnpSynReportLine (
  FILE *fp,
  Int2 type,
  SeqFeatPtr snp,
  SeqMgrFeatContextPtr snpcontext,
  SeqFeatPtr cds,
  SeqMgrFeatContextPtr cdscontext
)

{
  BioseqPtr  bsp;
  Char       label1 [FINDDBXREFBUFSIZE], label2 [FINDDBXREFBUFSIZE];
  SeqIdPtr   sip;
  Int2       type1, type2;

  if (snpcontext == NULL || cdscontext == NULL) return;

  if (! FindDbxref (snp, &type1, label1)) return;

  bsp = BioseqFindFromSeqLoc (cds->product);
  if (bsp != NULL) {
    for (sip = bsp->id; sip != NULL; sip = sip->next) {
      if (sip->choice == SEQID_GI) {
        fprintf (fp, "%s\t%ld\t%ld\t%ld\t\t%ld\t%ld\t%d\n",
                 label1, (long) snpcontext->left + 1, (long) snpcontext->right + 1,
                 (long) sip->data.intvalue, (long) cdscontext->left + 1,
                 (long) cdscontext->right + 1, (int) type);
        return;
      }
    }
  }

  if (! FindDbxref (cds, &type2, label2)) return;

  fprintf (fp, "%s\t%ld\t%ld\t\t%s\t%ld\t%ld\t%d\n",
           label1, (long) snpcontext->left + 1, (long) snpcontext->right + 1,
           label2, (long) cdscontext->left + 1, (long) cdscontext->right + 1,
           (int) type);
}

/* change all feature IDs to entered accession */

static void PromoteSeqId (SeqIdPtr sip, Pointer userdata)

{
  SeqIdPtr  bestid, newid, oldid;

  bestid = (SeqIdPtr) userdata;

  newid = SeqIdDup (bestid);
  if (newid == NULL) return;

  oldid = ValNodeNew (NULL);
  if (oldid == NULL) return;

  MemCopy (oldid, sip, sizeof (ValNode));
  oldid->next = NULL;

  sip->choice = newid->choice;
  sip->data.ptrvalue = newid->data.ptrvalue;

  SeqIdFree (oldid);
  ValNodeFree (newid);

  SeqIdStripLocus (sip);
}

static void CorrectFeatureSeqIds (
  SeqFeatPtr sfp,
  Pointer userdata
)

{
  VisitSeqIdsInSeqLoc (sfp->location, userdata, PromoteSeqId);
}

typedef struct snpsyn {
  FILE                  *fp;
  BioseqPtr             bsp;
  CharPtr               cdsBases;
  Int4                  cdsLen;
  Uint1                 frame;
  SeqFeatPtr            cds;
  SeqMgrFeatContextPtr  cdscontext;
  TransTablePtr         tbl;
} SnpSyn, PNTR SnpSynPtr;

#define IN_EXON   1
#define IN_SPLICE 2
#define IN_INTRON 3

static Int2 IsPosInIntron (
  Int4 pos,
  SeqMgrFeatContextPtr cdscontext
)

{
  Int4     i;
  Int2     j;
  Int4Ptr  ivals;
  Int4     left, right, tmp;

  if (cdscontext == NULL) return 0;
  ivals = cdscontext->ivals;
  if (ivals == NULL) return 0;

  /* shouldn't be out of range, but check anyway */

  if (pos < cdscontext->left) return 0;
  if (pos > cdscontext->right) return 0;

  /* see if within an exon */

  for (i = 0, j = 0; i < cdscontext->numivals; i++) {
    left = ivals [j];
    right = ivals [j + 1];
    if (left > right) {
      tmp = left;
      left = right;
      right = tmp;
    }
    if (pos >= left && pos <= right) return IN_EXON;
    j += 2;
  }

  /* see if within an intron */

  for (i = 1, j = 1; i < cdscontext->numivals; i++) {
    left = ivals [j];
    right = ivals [j + 1];
    if (left > right) {
      tmp = left;
      left = right;
      right = tmp;
    }
    if (pos > left && pos < right) {
      if (pos < left + 3 || pos + 3 > right) return IN_SPLICE;
      return IN_INTRON;
    }
    j += 2;
  }

  /* couldn't map it to an interval - error */

  return 0;
}

static Int2 IsSnpInIntron (
  SeqMgrFeatContextPtr snpcontext,
  SeqMgrFeatContextPtr cdscontext
)

{
  Int2  lefttype, righttype;

  if (snpcontext == NULL || cdscontext == NULL) return -1;

  /* categorize each end of SNP */

  lefttype = IsPosInIntron (snpcontext->left, cdscontext);
  righttype = IsPosInIntron (snpcontext->right, cdscontext);

  if (lefttype == IN_SPLICE || righttype == IN_SPLICE) return 7;
  if (lefttype == IN_INTRON && righttype == IN_INTRON) return 6;
  if (lefttype == IN_EXON && righttype == IN_EXON) return 0;

  /* must straddle intron/exon boundary, so treat as if in splice site */

  return 7;
}

static Boolean FindSNPsOnCDS (
  SeqFeatPtr sfp,
  SeqMgrFeatContextPtr context
)

{
  Char        aaBefore, aaAfter, ch;
  Char        buf [4];
  SeqFeatPtr  cds;
  SeqLocPtr   first = NULL, last = NULL, slp = NULL;
  GBQualPtr   gbq;
  Int2        i;
  Int4        j;
  Int4        pos1, pos2, adjust = 0, mod1;
  Int2        spliceCode;
  SnpSynPtr   ssp;
  Int2        state;

  ssp = (SnpSynPtr) context->userdata;

  /* make sure it isn't in an intron */

  spliceCode = IsSnpInIntron (context, ssp->cdscontext);
  if (spliceCode == 6) {

    /* intron - 6 */

    PrintSnpSynReportLine (ssp->fp, 6, sfp, context, ssp->cds, ssp->cdscontext);

    return TRUE;

  } else if (spliceCode == 7) {

    /* splicing - 7 */

    PrintSnpSynReportLine (ssp->fp, 7, sfp, context, ssp->cds, ssp->cdscontext);

    return TRUE;

  } else if (spliceCode == -1) {

    /* error */

    return TRUE;
  }

  /* figure out position relative to array, get triplet, translate before and after */

  if (ssp->frame == 2) {
    adjust = 1;
  } else if (ssp->frame == 3) {
    adjust = 2;
  }

  while ((slp = SeqLocFindNext (sfp->location, slp)) != NULL) {
    last = slp;
    if (first == NULL) {
      first = slp;
    }
  }
  if (first == NULL || last == NULL) return TRUE;
  if (first != last) {

    /* multi interval reality check - indel - 4 */

    PrintSnpSynReportLine (ssp->fp, 4, sfp, context, ssp->cds, ssp->cdscontext);

    return TRUE;
  }

  cds = ssp->cds;
  pos1 = GetOffsetInLoc (first, cds->location, SEQLOC_START) - adjust;
  pos2 = GetOffsetInLoc (last, cds->location, SEQLOC_STOP) - adjust;

  if (pos1 != pos2) {

    /* indel - 4 */

    PrintSnpSynReportLine (ssp->fp, 4, sfp, context, ssp->cds, ssp->cdscontext);

    return TRUE;
  }

  if (pos1 < 0) {

    /* if in first partial untranslatable codon - 2 */

    PrintSnpSynReportLine (ssp->fp, 2, sfp, context, ssp->cds, ssp->cdscontext);

    return TRUE;
  }

  mod1 = pos1 % 3;

  for (i = 0, j = pos1 - mod1; i < 3; i++, j++) {
    buf [i] = ssp->cdsBases [j];
  }
  buf [3] = '\0';

  for (state = 0, i = 0; i < 3; i++) {
    state = NextCodonState (ssp->tbl, state, buf [i]);
  }
  aaBefore = GetCodonResidue (ssp->tbl, state, TTBL_TOP_STRAND);

  for (gbq = sfp->qual; gbq != NULL; gbq = gbq->next) {
    if (StringICmp (gbq->qual, "allele") != 0) continue;
    if (StringHasNoText (gbq->val)) {

      /* error */

      continue;
    }
    if (StringLen (gbq->val) > 1) {

      /* indel - 4 */

      PrintSnpSynReportLine (ssp->fp, 4, sfp, context, ssp->cds, ssp->cdscontext);

      continue;
    }
    ch = gbq->val [0];
    if (ch == '-') {

      /* indel - 4 */

      PrintSnpSynReportLine (ssp->fp, 4, sfp, context, ssp->cds, ssp->cdscontext);

      continue;
    }

    buf [(int) mod1] = TO_UPPER (ch);

    for (state = 0, i = 0; i < 3; i++) {
      state = NextCodonState (ssp->tbl, state, buf [i]);
    }
    aaAfter = GetCodonResidue (ssp->tbl, state, TTBL_TOP_STRAND);

    if (aaBefore == aaAfter) {

      /* synonymous - 3 */

      PrintSnpSynReportLine (ssp->fp, 3, sfp, context, ssp->cds, ssp->cdscontext);

    } else {

      /* nonsynonymous - 4 */

      PrintSnpSynReportLine (ssp->fp, 4, sfp, context, ssp->cds, ssp->cdscontext);
    }
  }

  return TRUE;
}

static Boolean VisitEachCDS (
  SeqFeatPtr sfp,
  SeqMgrFeatContextPtr context
)

{
  Int2         actual;
  BioseqPtr    bsp;
  Int2         cnt;
  CdRegionPtr  crp;
  Boolean      featDefs [FEATDEF_MAX];
  Int4         len;
  SeqInt       sint;
  SeqPortPtr   spp;
  SnpSynPtr    ssp;
  CharPtr      txt;
  ValNode      vn;

  if (sfp->data.choice != SEQFEAT_CDREGION) return TRUE;
  if (sfp->pseudo) return TRUE;
  crp = (CdRegionPtr) sfp->data.value.ptrvalue;
  if (crp == NULL) return TRUE;

  ssp = (SnpSynPtr) context->userdata;
  bsp = ssp->bsp;

  /* locationFilter will find all SNPs within current CDS */

  MemSet ((Pointer) &vn, 0, sizeof (ValNode));
  vn.choice = SEQLOC_INT;
  vn.data.ptrvalue = &sint;

  MemSet ((Pointer) &sint, 0, sizeof (SeqInt));
  sint.id = SeqLocId (sfp->location);

  sint.from = context->left;
  sint.to = context->right;
  sint.strand = Seq_strand_both;

  MemSet ((Pointer) &featDefs, 0, sizeof (featDefs));
  featDefs [FEATDEF_variation] = TRUE;

  /* also make array of just cds bases */

  len = SeqLocLen (sfp->location);
  if (len < 1) return TRUE;

  ssp->cdsBases = MemNew ((size_t) (len + 2));
  ssp->cdsLen = len;
  ssp->frame = crp->frame;
  ssp->cds = sfp;
  ssp->cdscontext = context;

  /* generalize reading loop for humongous coding regions */

  spp = SeqPortNewByLoc (sfp->location, Seq_code_iupacna);
  if (spp != NULL) {
    cnt = (Int2) MIN (len, 32000L);
    txt = ssp->cdsBases;
    while (cnt > 0 && len > 0) {
      actual = SeqPortRead (spp, (BytePtr) txt, cnt);
      if (actual > 0) {
        len -= actual;
        txt += actual;
        cnt = (Int2) MIN (len, 32000L);
      }
    }
    SeqPortFree (spp);
  }

  if (! StringHasNoText (ssp->cdsBases)) {
    SeqMgrExploreFeatures (bsp, (Pointer) ssp, FindSNPsOnCDS, &vn, NULL, featDefs);
  }

  ssp->cdsBases = MemFree (ssp->cdsBases);

  return TRUE;
}

static void ProcessOneContig (
  CharPtr directory,
  CharPtr base,
  CharPtr suffix,
  TransTablePtr  PNTR tblptr,
  Int2Ptr gcode,
  Int2 genCode
)

{
  BioseqPtr      bsp;
  Char           buf [42];
  Pointer        dataptr = NULL;
  Uint2          datatype, entityID = 0;
  Boolean        featDefs [FEATDEF_MAX];
  SeqEntryPtr    nsep;
  Int2           realGenCode;
  SeqAnnotPtr    sap;
  SeqEntryPtr    sep;
  SeqIdPtr       sip;
  SnpSyn         ss;
  TransTablePtr  tbl;


  /* check for existing .fxn file, abort */

  if (FileAlreadyExists (directory, base, "fxn")) return;

  /* read ASN.1 or FASTA data file */

  dataptr = ReadOneFile (directory, base, suffix, &datatype);
  if (dataptr == NULL) return;

  entityID = ObjMgrRegister (datatype, dataptr);
  if (entityID < 1) return;

  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep == NULL) return;

  nsep = FindNucSeqEntry (sep);
  if (nsep == NULL || (! IS_Bioseq (nsep))) return;
  bsp = (BioseqPtr) nsep->data.ptrvalue;
  if (bsp == NULL) return;

  /* genetic code in record overrides parameter */

  realGenCode = SeqEntryToGeneticCode (sep, NULL, NULL, 0);
  if (realGenCode != 0 && realGenCode != genCode) {
    if (genCode != 0) {
      sip = SeqIdFindBest (bsp->id, 0);
      SeqIdWrite (sip, buf, PRINTID_FASTA_LONG, sizeof (buf) - 1);
      if (StringHasNoText (buf)) {
        StringCpy (buf, "?");
      }
      Message (MSG_POST, "%s: GenCode %d not equal to RealGenCode %d\n",
               buf, (int) genCode, (int) realGenCode);
    }
    genCode = realGenCode;
  }
  if (genCode == 0) {
    genCode = 1;
  }

  /* prime translation finite state machine if genetic code changed */

  tbl = *tblptr;
  if (tbl != NULL && genCode != *gcode) {
    tbl = TransTableFree (tbl);
    *tblptr = tbl;
  }
  if (tbl == NULL) {
    tbl = TransTableNew (genCode);
    *tblptr = tbl;
    *gcode = genCode;
  }

  /* read feature table file (if any) and attach to bioseq */

  sap = (SeqAnnotPtr) ReadOneFile (directory, base, "tbl", &datatype);
  if (sap != NULL && datatype == OBJ_SEQANNOT && sap->type == 1) {
    sap->next = bsp->annot;
    bsp->annot = sap;
  } else {
    ObjMgrFree (datatype, (Pointer) sap);
  }

  /* for parsed in features, just impose proper genetic code */

  for (sap = bsp->annot; sap != NULL; sap = sap->next) {
    if (sap->type == 1) {
      SetEmptyGeneticCodes (sap, genCode);
      /*
      sfp = (SeqFeatPtr) sap->data;
      PromoteXrefs (sfp, bsp, entityID);
      */
    }
  }

  /* put same seqid on all features */

  VisitFeaturesOnBsp (bsp, (Pointer) bsp->id, CorrectFeatureSeqIds);

  /* build feature indexes */

  SeqMgrIndexFeatures (entityID, NULL);

  /* now find all CDSs */

  MemSet ((Pointer) &featDefs, 0, sizeof (featDefs));
  featDefs [FEATDEF_CDS] = TRUE;

  MemSet ((Pointer) &ss, 0, sizeof (SnpSyn));
  ss.fp = CreateOneFile (directory, base, "fxn");
  ss.bsp = bsp;
  ss.tbl = tbl;

  SeqMgrExploreFeatures (bsp, (Pointer) &ss, VisitEachCDS, NULL, NULL, featDefs);

  FileClose (ss.fp);

  /* clean up memory object */

  ObjMgrFreeByEntityID (entityID);
}

/* Args structure contains command-line arguments */

#define i_argInputPath  0
#define i_argGenCode    1

Args myargs [] = {
  {"Path to files", NULL, NULL, NULL,
    TRUE, 'p', ARG_STRING, 0.0, 0, NULL},
  {"Genetic code", "0", "0", "100",
    TRUE, 'c', ARG_INT, 0.0, 0, NULL},
};

Int2 Main (void)

{
  CharPtr        directory, base, ptr;
  Int2           gcode = 0;
  Int2           genCode;
  ValNodePtr     head, vnp;
  TransTablePtr  tbl = NULL;

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

  /* process command line arguments */

  if (! GetArgs ("snpsynon", sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  directory = (CharPtr) myargs [i_argInputPath].strvalue;
  genCode = (Int2) myargs [i_argGenCode].intvalue;

  head = DirCatalog (directory);

  /* process all .sqn or .fsa files in the directory */

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == 0) {
      base = (CharPtr) vnp->data.ptrvalue;
      if (! StringHasNoText (base)) {
        ptr = StringStr (base, ".sqn");
        if (ptr == NULL) {
          ptr = StringStr (base, ".fsa");
        }
        if (ptr != NULL) {
          *ptr = '\0';
          ptr++;
          ProcessOneContig (directory, base, ptr, &tbl, &gcode, genCode);
        }
      }
    }
  }

  ValNodeFreeData (head);

  TransTableFree (tbl);

  return 0;
}

