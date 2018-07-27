/*   snpscan.c
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
* File Name:  snpscan.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   3/8/00
*
* $Revision: 6.10 $
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

static CharPtr xreftypes [] = {
  "SNP", "STS", "Clone", "LOCUS", "MODELCDS", "MODELMRNA", "contig", "component", NULL
};

static CharPtr xreftags [] = {
  "dbSNP", "dbSTS", "clone", "LocusID", "cdsModel", "mrnaModel", "contig", "seq", NULL
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

static void PrintSnpScanReportLine (
  FILE *fp,
  Int2 type,
  SeqFeatPtr sfp1,
  SeqMgrFeatContextPtr sfp1context,
  SeqFeatPtr sfp2,
  SeqMgrFeatContextPtr sfp2context
)

{
  Char  label1 [FINDDBXREFBUFSIZE], label2 [FINDDBXREFBUFSIZE];
  Int2  type1, type2;

  if (! FindDbxref (sfp1, &type1, label1)) return;
  if (! FindDbxref (sfp2, &type2, label2)) return;

  fprintf (fp, "%s\t%s\t%ld\t%ld\t%s\t%s\t%ld\t%ld\t%d\n",
           label1, xreftypes [type1],
           (long) sfp1context->left + 1, (long) sfp1context->right + 1,
           label2, xreftypes [type2],
           (long) sfp2context->left + 1, (long) sfp2context->right + 1,
           (int) type);
}

typedef struct snpdata {
  FILE                  *fp;
  SeqFeatPtr            sfp;
  SeqMgrFeatContextPtr  sfpcontext;
  Uint2                 ignoreID;
  Int4                  left;
  Int4                  right;
} SnpData, PNTR SnpDataPtr;

static Boolean TestProximity (
  SeqMgrFeatContextPtr context,
  Int4 left,
  Int4 right
)

{
  /* check for simple overlap */

  if (context->left <= left && context->right > left) return TRUE;
  if (context->left < right && context->right >= right) return TRUE;

  /* check for proximity - 2000 bp 5' or 500 bp 3' */

  if (context->strand == Seq_strand_minus) {

    if (context->left > right && context->left <= right + 500) return TRUE;
    if (context->right < left && context->right + 2000 >= left) return TRUE;

  } else {

    if (context->left > right && context->left <= right + 2000) return TRUE;
    if (context->right < left && context->right + 500 >= left) return TRUE;

  }

  return FALSE;
}

static Boolean GenesOnSlp (
  SeqFeatPtr sfp,
  SeqMgrFeatContextPtr context
)

{
  SnpDataPtr  sdp;

  sdp = (SnpDataPtr) context->userdata;
  if (context->itemID == sdp->ignoreID) return TRUE;
  if (! TestProximity (context, sdp->left, sdp->right)) return TRUE;

  PrintSnpScanReportLine (sdp->fp, /* 3 */ 1, sdp->sfp, sdp->sfpcontext, sfp, context);
  return TRUE;
}

static void LookForGenes (
  FILE *fp,
  BioseqPtr bsp,
  SeqFeatPtr sfp,
  VoidPtr exonarray,
  Int4 numexons,
  SeqMgrFeatContextPtr sfpcontext
)

{
  SeqFeatPtr         cds, exon, mrna;
  SeqMgrFeatContext  fcontext;
  Boolean            featDefs [FEATDEF_MAX];
  Uint2              ignoreID = 0;
  SnpData            sd;
  SeqInt             sint;
  ValNode            vn;

  if (sfp == NULL || sfp->location == NULL) return;

  /* probe with strand_both to get features on both strands */

  MemSet ((Pointer) &vn, 0, sizeof (ValNode));
  vn.choice = SEQLOC_INT;
  vn.data.ptrvalue = &sint;

  MemSet ((Pointer) &sint, 0, sizeof (SeqInt));
  sint.id = SeqLocId (sfp->location);

  sint.from = sfpcontext->left;
  sint.to = sfpcontext->right;
  sint.strand = Seq_strand_both;

  /* first look for overlapping coding region */

  cds = SeqMgrGetOverlappingFeature (&vn, FEATDEF_CDS, NULL, 0, NULL,
                                     SIMPLE_OVERLAP, &fcontext);
  if (cds != NULL) {
    PrintSnpScanReportLine (fp, /* 1 */ 2, sfp, sfpcontext, cds, &fcontext);
    ignoreID = fcontext.itemID;

  } else {

    /* check exons next */

    exon = SeqMgrGetOverlappingFeature (&vn, 0, exonarray, numexons, NULL,
                                        SIMPLE_OVERLAP, &fcontext);
    if (exon != NULL) {
      PrintSnpScanReportLine (fp, /* 2 */ 5, sfp, sfpcontext, exon, &fcontext);
      ignoreID = fcontext.itemID;

    } else {

      /* then look for overlapping transcript */

      mrna = SeqMgrGetOverlappingFeature (&vn, FEATDEF_mRNA, NULL, 0, NULL,
                                          SIMPLE_OVERLAP, &fcontext);
      if (mrna != NULL) {
        PrintSnpScanReportLine (fp, /* 2 */ 5, sfp, sfpcontext, mrna, &fcontext);
        ignoreID = fcontext.itemID;
      }
    }
  }

  /* proximity checks only done for SNP and STS features, not clones */

  if (sfp->idx.subtype != FEATDEF_variation && sfp->idx.subtype != FEATDEF_STS) return;

  /* now scan for gene, mRNA, and CDS proximity searches */

  MemSet ((Pointer) &featDefs, 0, sizeof (featDefs));
  featDefs [FEATDEF_GENE] = TRUE;
  featDefs [FEATDEF_CDS] = TRUE;
  featDefs [FEATDEF_mRNA] = TRUE;

  sd.fp = fp;
  sd.sfp = sfp;
  sd.sfpcontext = sfpcontext;
  sd.ignoreID = ignoreID;
  sd.left = sfpcontext->left;
  sd.right = sfpcontext->right;

  /* restrict candidates with location filter of 2000 bp within SNP or STS */

  sint.from = MAX ((Int4) 0, (Int4) sfpcontext->left - 2000);
  sint.to = MIN ((Int4) bsp->length - 1, (Int4) sfpcontext->right + 2000);
  sint.strand = Seq_strand_both;

  SeqMgrExploreFeatures (bsp, (Pointer) &sd, GenesOnSlp, &vn, NULL, featDefs);
}

static void LookForSTSs (
  FILE *fp,
  BioseqPtr bsp,
  SeqFeatPtr snp,
  SeqMgrFeatContextPtr snpcontext
)

{
  SeqMgrFeatContext  fcontext, lftcontext, rgtcontext;
  SeqFeatPtr         sts, bestlft = NULL, bestrgt = NULL;

  MemSet ((Pointer) &lftcontext, 0, sizeof (SeqMgrFeatContext));
  MemSet ((Pointer) &rgtcontext, 0, sizeof (SeqMgrFeatContext));

  sts = SeqMgrGetNextFeature (bsp, NULL, 0, FEATDEF_STS, &fcontext);
  while (sts != NULL) {

    /* check for simple overlap */

    if ((fcontext.left <= snpcontext->left && fcontext.right > snpcontext->left) ||
        (fcontext.left < snpcontext->right && fcontext.right >= snpcontext->right)) {
      PrintSnpScanReportLine (fp, /* 1 */ 2, snp, snpcontext, sts, &fcontext);
      return;
    }

    /* check for closest on each side - 10000 bp 5' or 3' */

    if (fcontext.right < snpcontext->left && fcontext.right + 10000 >= snpcontext->left) {
      bestlft = sts;
      MemCopy ((Pointer) &lftcontext, (Pointer) &fcontext, sizeof (SeqMgrFeatContext));
    }

    if (fcontext.left > snpcontext->right && fcontext.left <= snpcontext->left + 10000) {
      if (bestrgt == NULL) {
        bestrgt = sts;
      MemCopy ((Pointer) &rgtcontext, (Pointer) &fcontext, sizeof (SeqMgrFeatContext));
      }
    }

    /* get next STS feature */

    sts = SeqMgrGetNextFeature (bsp, sts, 0, FEATDEF_STS, &fcontext);
  }

  if (bestlft != NULL) {
    PrintSnpScanReportLine (fp, /* 2 */ 5, snp, snpcontext, bestlft, &lftcontext);
  }

  if (bestrgt != NULL) {
    PrintSnpScanReportLine (fp, /* 2 */ 5, snp, snpcontext, bestrgt, &rgtcontext);
  }
}

static void LookForClones (
  FILE *fp,
  BioseqPtr bsp,
  SeqFeatPtr sfp,
  VoidPtr clonearray,
  Int4 numclones,
  SeqMgrFeatContextPtr sfpcontext
)

{
  SeqFeatPtr         clone;
  SeqMgrFeatContext  fcontext;
  SeqInt             sint;
  ValNode            vn;

  if (sfp == NULL || sfp->location == NULL) return;

  /* probe with strand_both to get features on both strands */

  MemSet ((Pointer) &vn, 0, sizeof (ValNode));
  vn.choice = SEQLOC_INT;
  vn.data.ptrvalue = &sint;

  MemSet ((Pointer) &sint, 0, sizeof (SeqInt));
  sint.id = SeqLocId (sfp->location);

  sint.from = sfpcontext->left;
  sint.to = sfpcontext->right;
  sint.strand = Seq_strand_both;

  /* look for overlapping clone */

  clone = SeqMgrGetOverlappingFeature (&vn, 0, clonearray, numclones, NULL,
                                       SIMPLE_OVERLAP, &fcontext);
  if (clone != NULL) {
    PrintSnpScanReportLine (fp, /* 1 */ 2, sfp, sfpcontext, clone, &fcontext);
  }
}

static Boolean GeneCDSmRNA (
  SeqFeatPtr sfp,
  SeqMgrFeatContextPtr sfpcontext
)

{
  FILE              *fp;
  SeqFeatPtr         gene;
  SeqMgrFeatContext  genecontext;
  GeneRefPtr         grp;

  fp = (FILE*) sfpcontext->userdata;

  grp = SeqMgrGetGeneXref (sfp);
  if (grp != NULL && SeqMgrGeneIsSuppressed (grp)) return TRUE;

  gene = SeqMgrGetOverlappingGene (sfp->location, &genecontext);
  if (gene != NULL) {
    PrintSnpScanReportLine (fp, /* 1 */ 2, gene, &genecontext, sfp, sfpcontext);
  }

  return TRUE;
}

typedef struct mdfiles {
  FILE     *fp;
  FILE     *snpfp;
  FILE     *stsfp;
  FILE     *clonefp;
  FILE     *locusfp;
  FILE     *modelfp;
  FILE     *contigfp;
  FILE     *seqfp;
  CharPtr  chrom;
} MdFiles, PNTR MdFilesPtr;

static FILE* FileForType (MdFilesPtr mfp, Int2 type)

{
  if (mfp == NULL) return NULL;
  switch (type) {
    case 0 :
      return mfp->snpfp;
    case 1 :
      return mfp->stsfp;
    case 2 :
      return mfp->clonefp;
    case 3 :
      return mfp->locusfp;
    case 4 :
    case 5 :
      /* combine MODELCDS and MODELMRNA into seq_model.md */
      return mfp->modelfp;
    case 6 :
      return mfp->contigfp;
    case 7 :
      return mfp->seqfp;
    default :
      break;
  }
  return NULL;
}

static Boolean ExamineBSPs (
  BioseqPtr bsp,
  SeqMgrBioseqContextPtr bspcontext
)

{
  VoidPtr            clonearray, exonarray;
  Boolean            featDefs [FEATDEF_MAX];
  FILE               *fp;
  Int2               i, j, type;
  Char               label [FINDDBXREFBUFSIZE];
  Int4               left, right, tmp, numclones, numexons, taxid = 9606;
  MdFilesPtr         mfp;
  SeqFeatPtr         snp, sts, clone, sfp;
  SeqMgrFeatContext  sfpcontext;
  Char               strand;

  mfp = (MdFilesPtr) bspcontext->userdata;
  if (bsp == NULL || mfp == NULL) return TRUE;

  exonarray = SeqMgrBuildFeatureIndex (bsp, &numexons, 0, FEATDEF_exon);
  clonearray = SeqMgrBuildFeatureIndex (bsp, &numclones, 0, FEATDEF_misc_feature);

  /* get first SNP feature */

  snp = SeqMgrGetNextFeature (bsp, NULL, 0, FEATDEF_variation, &sfpcontext);
  while (snp != NULL) {

    LookForGenes (mfp->fp, bsp, snp, exonarray, numexons, &sfpcontext);

    LookForSTSs (mfp->fp, bsp, snp, &sfpcontext);

    LookForClones (mfp->fp, bsp, snp, clonearray, numclones, &sfpcontext);

    /* get next SNP feature */

    snp = SeqMgrGetNextFeature (bsp, snp, 0, FEATDEF_variation, &sfpcontext);
  }

  /* get first STS feature */

  sts = SeqMgrGetNextFeature (bsp, NULL, 0, FEATDEF_STS, &sfpcontext);
  while (sts != NULL) {

    LookForGenes (mfp->fp, bsp, sts, exonarray, numexons, &sfpcontext);

    LookForClones (mfp->fp, bsp, sts, clonearray, numclones, &sfpcontext);

    /* get next STS feature */

    sts = SeqMgrGetNextFeature (bsp, sts, 0, FEATDEF_STS, &sfpcontext);
  }

  /* get first clone feature */

  clone = SeqMgrGetNextFeature (bsp, NULL, 0, FEATDEF_misc_feature, &sfpcontext);
  while (clone != NULL) {

    LookForGenes (mfp->fp, bsp, clone, exonarray, numexons, &sfpcontext);

    /* get next clone feature */

    clone = SeqMgrGetNextFeature (bsp, clone, 0, FEATDEF_misc_feature, &sfpcontext);
  }

  /* now find genes overlapping mRNA and CDS */

  MemSet ((Pointer) &featDefs, 0, sizeof (featDefs));
  featDefs [FEATDEF_CDS] = TRUE;
  featDefs [FEATDEF_mRNA] = TRUE;
  SeqMgrExploreFeatures (bsp, (Pointer) mfp->fp, GeneCDSmRNA, NULL, NULL, featDefs);

  MemFree (exonarray);
  MemFree (clonearray);

  if (mfp->chrom != NULL) {

    /* first extra line */

    for (type = 0; xreftypes [type] != NULL; type++) {
      fp = FileForType (mfp, type);
      if (fp != NULL) {
        fprintf (fp, "%ld\t%s\t0\t0\t+\t-1\t%s\tstart\t10\n",
                 (long) taxid, mfp->chrom,
                 xreftypes [type]);
      }
    }

    /* get all features for .md files */

    sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &sfpcontext);
    while (sfp != NULL) {

      if (FindDbxref (sfp, &type, label)) {
        fp = FileForType (mfp, type);
        if (fp != NULL && sfpcontext.ivals != NULL) {
          strand = '+';
          if (sfpcontext.strand == Seq_strand_minus) {
            strand = '-';
          }
          for (i = 0, j = 0; i < sfpcontext.numivals; i++, j += 2) {
            left = sfpcontext.ivals [j];
            right = sfpcontext.ivals [j + 1];
            if (left > right) {
              tmp = left;
              left = right;
              right = tmp;
            }
            fprintf (fp, "%ld\t%s\t%ld\t%ld\t%c\t%s\t%s\n",
                     (long) taxid, mfp->chrom,
                     (long) (left + 1),
                     (long) (right + 1),
                     (char) strand, label, xreftypes [type]);
          }
        }
      }

      sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &sfpcontext);
    }

    /* second extra line */

    for (type = 0; xreftypes [type] != NULL; type++) {
      fp = FileForType (mfp, type);
      if (fp != NULL) {
        fprintf (fp, "%ld\t%s\t%ld\t%ld\t+\t-2\t%s\tend\t10\n",
                 (long) taxid, mfp->chrom,
                 (long) (bsp->length),
                 (long) (bsp->length),
                 xreftypes [type]);
      }
    }
  }

  return TRUE;
}

/* code modified from Sequin to impose bioseq id on features */

#define NUM_ORDER 16

static SeqIdPtr SeqIdFindBestForPromotion (SeqIdPtr sip)

{
  Uint1  order [NUM_ORDER];

  SeqIdBestRank (order, NUM_ORDER);
  order [SEQID_OTHER] = 52;
  return SeqIdSelect (sip, order, NUM_ORDER);
}

static void PromoteSeqId (SeqIdPtr sip, SeqIdPtr bestid)

{
  SeqIdPtr  newid, oldid;

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

static void PromoteSeqIdList (SeqIdPtr sip, SeqIdPtr bestid)

{
  while (sip != NULL) {
    PromoteSeqId (sip, bestid);
    sip = sip->next;
  }
}

static void PromoteSeqLocList (SeqLocPtr slp, SeqIdPtr newid)

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
        PromoteSeqIdList (sip, newid);
        break;
      case SEQLOC_INT :
        sinp = (SeqIntPtr) slp->data.ptrvalue;
        if (sinp != NULL) {
          sip = sinp->id;
          PromoteSeqIdList (sip, newid);
        }
        break;
      case SEQLOC_PNT :
        spp = (SeqPntPtr) slp->data.ptrvalue;
        if (spp != NULL) {
          sip = spp->id;
          PromoteSeqIdList (sip, newid);
        }
        break;
      case SEQLOC_PACKED_PNT :
        psp = (PackSeqPntPtr) slp->data.ptrvalue;
        if (psp != NULL) {
          sip = psp->id;
          PromoteSeqIdList (sip, newid);
        }
        break;
      case SEQLOC_PACKED_INT :
      case SEQLOC_MIX :
      case SEQLOC_EQUIV :
        loc = (SeqLocPtr) slp->data.ptrvalue;
        while (loc != NULL) {
          PromoteSeqLocList (loc, newid);
          loc = loc->next;
        }
        break;
      case SEQLOC_BOND :
        sbp = (SeqBondPtr) slp->data.ptrvalue;
        if (sbp != NULL) {
          spp = (SeqPntPtr) sbp->a;
          if (spp != NULL) {
            sip = spp->id;
            PromoteSeqIdList (sip, newid);
          }
          spp = (SeqPntPtr) sbp->b;
          if (spp != NULL) {
            sip = spp->id;
            PromoteSeqIdList (sip, newid);
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

static void CorrectFeatureSeqIds (
  SeqFeatPtr sfp,
  Pointer userdata
)

{
  PromoteSeqLocList (sfp->location, (SeqIdPtr) userdata);
}

static void ProcessOneContig (
  CharPtr directory,
  CharPtr base,
  CharPtr chrom,
  Int4 num
)

{
  BioseqPtr          bsp;
  Pointer            dataptr = NULL;
  Uint2              datatype, entityID = 0;
  Char               file [FILENAME_MAX], path [PATH_MAX];
  MdFiles            mf;
  Int4               max = 0;
  FILE               *fp;
  SeqAnnotPtr        sap;
  SeqEntryPtr        sep;
  SeqFeatPtr         sfp;
  SeqMgrFeatContext  sfpcontext;
  Char               str [32];

  StringNCpy_0 (path, directory, sizeof (path));
  sprintf (file, "%s.feat.off", base);
  FileBuildPath (path, NULL, file);

  fp = FileOpen (path, "r");
  if (fp == NULL) return;

  /* read ASN.1 data file */

  /*
  dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, &entityID,
                                    FALSE, FALSE, TRUE, FALSE);
  */

  /* make fake virtual bioseq */

  sep = SeqEntryNew ();
  if (sep != NULL) {
    bsp = BioseqNew ();
    if (bsp != NULL) {
      sep->choice = 1;
      sep->data.ptrvalue = (Pointer) bsp;
      SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, sep);

      bsp->mol = Seq_mol_na;

      bsp->repr = Seq_repr_virtual;
      bsp->length = INT4_MAX;
      sprintf (str, "lcl|%ld", (long) num);
      bsp->id = MakeSeqID (str);
      SeqMgrAddToBioseqIndex (bsp);

      datatype = OBJ_BIOSEQ;
      entityID = ObjMgrRegister (OBJ_BIOSEQ, (Pointer) bsp);
      dataptr = (Pointer) bsp;
    }
  }

  if (entityID < 1 || dataptr == NULL) {
    FileClose (fp);
    return;
  }

  if (datatype == OBJ_BIOSEQ) {
    bsp = (BioseqPtr) dataptr;
    if (bsp->repr == Seq_repr_virtual) {

      /* read feature tables after >Virtual line */

      while ((dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL,
                                                FALSE, FALSE, TRUE, FALSE)) != NULL) {
        if (datatype == OBJ_SEQANNOT) {
          sap = (SeqAnnotPtr) dataptr;
          if (sap->type == 1) {
            sap->next = bsp->annot;
            bsp->annot = sap;
          } else {
            ObjMgrFree (datatype, dataptr);
          }
        } else {
          ObjMgrFree (datatype, dataptr);
        }
      }
    }
  }

  FileClose (fp);

  /* put same seqid on all features */

  VisitFeaturesOnBsp (bsp, (Pointer) bsp->id, CorrectFeatureSeqIds);

  /* build feature indexes */

  SeqMgrIndexFeatures (entityID, NULL);

  /* find virtual length of sequence from features */

  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &sfpcontext);
  while (sfp != NULL) {
    if (sfpcontext.right > max) {
      max = sfpcontext.right;
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &sfpcontext);
  }
  bsp->length = max + 1;

  /* now process features in record */

  MemSet ((Pointer) &mf, 0, sizeof (MdFiles));

  StringNCpy_0 (path, directory, sizeof (path));
  sprintf (file, "%s.dat", base);
  FileBuildPath (path, NULL, file);

  mf.fp = FileOpen (path, "w");
  if (mf.fp != NULL) {

    mf.chrom = chrom;
    if (chrom != NULL) {

      /* open map files for append */

      StringNCpy_0 (path, directory, sizeof (path));
      FileBuildPath (path, NULL, "seq_snp.md");
      mf.snpfp = FileOpen (path, "a");

      StringNCpy_0 (path, directory, sizeof (path));
      FileBuildPath (path, NULL, "seq_sts.md");
      mf.stsfp = FileOpen (path, "a");

      StringNCpy_0 (path, directory, sizeof (path));
      FileBuildPath (path, NULL, "seq_clone.md");
      mf.clonefp = FileOpen (path, "a");

      StringNCpy_0 (path, directory, sizeof (path));
      FileBuildPath (path, NULL, "seq_locus.md");
      mf.locusfp = FileOpen (path, "a");

      StringNCpy_0 (path, directory, sizeof (path));
      FileBuildPath (path, NULL, "seq_model.md");
      mf.modelfp = FileOpen (path, "a");

      StringNCpy_0 (path, directory, sizeof (path));
      FileBuildPath (path, NULL, "seq_contig.md");
      mf.contigfp = FileOpen (path, "a");

      StringNCpy_0 (path, directory, sizeof (path));
      FileBuildPath (path, NULL, "seq_component.md");
      mf.seqfp = FileOpen (path, "a");
    }

    /* visit callback for all primary nucleotides (not parts of segments) */

    SeqMgrExploreBioseqs (entityID, NULL, (Pointer) &mf, ExamineBSPs,
                          TRUE, FALSE, FALSE);

    FileClose (mf.fp);
    FileClose (mf.snpfp);
    FileClose (mf.stsfp);
    FileClose (mf.clonefp);
    FileClose (mf.locusfp);
    FileClose (mf.modelfp);
    FileClose (mf.contigfp);
    FileClose (mf.seqfp);
  }

  /* clean up memory object */

  ObjMgrFreeByEntityID (entityID);
}

/* Args structure contains command-line arguments */

#define i_argInputPath  0
#define i_argSingleFile 1

Args myargs [] = {
  {"Path to files", NULL, NULL, NULL,
    TRUE, 'p', ARG_STRING, 0.0, 0, NULL},
  {"Single ASN.1 file", NULL, NULL, NULL,
    TRUE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
};

Int2 Main (void)

{
  Pointer     dataptr = NULL;
  Uint2       datatype, entityID = 0;
  CharPtr     directory, base, singlefile, chrom, ptr;
  Char        file [FILENAME_MAX], path [PATH_MAX];
  FILE        *fp;
  ValNodePtr  head, vnp;
  MdFiles     mf;
  Int4        num;

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

  if (! GetArgs ("snpscan", sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  directory = (CharPtr) myargs [i_argInputPath].strvalue;
  singlefile = (CharPtr) myargs [i_argSingleFile].strvalue;

  if (! StringHasNoText (singlefile)) {

    fp = FileOpen (singlefile, "r");
    if (fp != NULL) {

      dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, &entityID,
                                        FALSE, FALSE, TRUE, FALSE);

      FileClose (fp);

      if (entityID > 0 && dataptr != NULL) {

        /* build feature indexes */

        SeqMgrIndexFeatures (entityID, NULL);

        /* now process features in record */

        MemSet ((Pointer) &mf, 0, sizeof (MdFiles));

        path [0] = '\0';
        sprintf (file, "%s.dat", singlefile);
        FileBuildPath (path, NULL, file);

        mf.fp = FileOpen (path, "w");
        if (mf.fp != NULL) {

          /* visit callback for all primary nucleotides (not parts of segments) */

          SeqMgrExploreBioseqs (entityID, NULL, (Pointer) &mf, ExamineBSPs,
                                TRUE, FALSE, FALSE);

          FileClose (mf.fp);
        }

        /* clean up memory object */

        ObjMgrFreeByEntityID (entityID);
      }
    }

  } else {

    /* chr_1.feat.off through chr_y.feat.off and NT_xxxx.feat.off */

    head = DirCatalog (directory);

    for (vnp = head, num = 0; vnp != NULL; vnp = vnp->next) {
      if (vnp->choice == 0) {
        base = (CharPtr) vnp->data.ptrvalue;
        if (! StringHasNoText (base)) {
          ptr = StringStr (base, ".feat.off");
          if (ptr != NULL) {
            *ptr = '\0';
            chrom = NULL;
            if (StringNICmp (base, "chr_", 4) == 0) {
              chrom = base + 4;
            }
            num++;
            ProcessOneContig (directory, base, chrom, num);
          }
        }
      }
    }
    ValNodeFreeData (head);
  }

  return 0;
}

