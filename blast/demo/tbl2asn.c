/*   tbl2asn.c
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
* File Name:  tbl2asn.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   5/5/00
*
* $Revision: 6.25 $
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
#include <edutil.h>
#include <seqport.h>
#include <gather.h>
#include <sqnutils.h>
#include <subutil.h>
#include <toasn3.h>
#include <valid.h>
#include <asn2gnbk.h>
#include <explore.h>
#include <simple.h>

static FILE* OpenOneFile (
  CharPtr directory,
  CharPtr base,
  CharPtr suffix
)

{
  Char  file [FILENAME_MAX], path [PATH_MAX];

  if (base == NULL) {
    base = "";
  }
  if (suffix == NULL) {
    suffix = "";
  }
  StringNCpy_0 (path, directory, sizeof (path));
  sprintf (file, "%s%s", base, suffix);
  FileBuildPath (path, NULL, file);

  return FileOpen (path, "r");
}

static void WriteOneFile (
  CharPtr results,
  CharPtr base,
  CharPtr suffix,
  SeqEntryPtr sep,
  SubmitBlockPtr sbp
)

{
  AsnIoPtr   aip;
  Char       file [FILENAME_MAX], path [PATH_MAX];
  SeqSubmit  ssb;

  MemSet ((Pointer) &ssb, 0, sizeof (SeqSubmit));
  ssb.sub = sbp;
  ssb.datatype = 1;
  ssb.data = (Pointer) sep;

  StringNCpy_0 (path, results, sizeof (path));
  sprintf (file, "%s%s", base, suffix);
  FileBuildPath (path, NULL, file);

  aip = AsnIoOpen (path, "w");
  if (aip == NULL) return;

  if (sbp != NULL) {
    SeqSubmitAsnWrite (&ssb, aip, NULL);
  } else {
    SeqEntryAsnWrite (sep, aip, NULL);
  }

  AsnIoFlush (aip);
  AsnIoClose (aip);
}

static void ValidateOneFile (
  CharPtr results,
  CharPtr base,
  CharPtr suffix,
  SeqEntryPtr sep
)

{
  Char            file [FILENAME_MAX], path [PATH_MAX];
  ErrSev          oldErrSev;
  ValidStructPtr  vsp;

  StringNCpy_0 (path, results, sizeof (path));
  sprintf (file, "%s%s", base, suffix);
  FileBuildPath (path, NULL, file);

  ErrSetOptFlags (EO_LOGTO_USRFILE);
  ErrSetLogfile (path, ELOG_APPEND | ELOG_NOCREATE);

  vsp = ValidStructNew ();
  if (vsp != NULL) {
    vsp->useSeqMgrIndexes = TRUE;
    vsp->suppressContext = TRUE;
    oldErrSev = ErrSetMessageLevel (SEV_NONE);
    ValidateSeqEntry (sep, vsp);
    ValidStructFree (vsp);
    ErrSetMessageLevel (oldErrSev);
  }

  ErrSetLogfile (NULL, ELOG_APPEND | ELOG_NOCREATE);
  ErrClearOptFlags  (EO_LOGTO_USRFILE);
}

static void FlatfileOneFile (
  CharPtr results,
  CharPtr base,
  CharPtr suffix,
  SeqEntryPtr sep
)

{
  Char    file [FILENAME_MAX], path [PATH_MAX];
  FILE    *fp;
  ErrSev  oldErrSev;

  StringNCpy_0 (path, results, sizeof (path));
  sprintf (file, "%s%s", base, suffix);
  FileBuildPath (path, NULL, file);

  fp = FileOpen (path, "w");
  if (fp == NULL) return;

  oldErrSev = ErrSetMessageLevel (SEV_MAX);
  SeqEntryToGnbk (sep, NULL, GENBANK_FMT, ENTREZ_MODE, NORMAL_STYLE, 0, fp);
  ErrSetMessageLevel (oldErrSev);

  FileClose (fp);
}

/* for full-length cDNAs, allow automatic annotation of largest internal ORF */

typedef struct orfdata {
  Int4     curlen [6], bestlen [6], currstart [6], beststart [6], sublen [6];
  Boolean  inorf [6], altstart;
} OrfData, PNTR OrfDataPtr;

static void LIBCALLBACK LookForOrfs (
  Int4 position,
  Char residue,
  Boolean atgStart,
  Boolean altStart,
  Boolean orfStop,
  Int2 frame,
  Uint1 strand,
  Pointer userdata
)

{
  Int2        idx;
  OrfDataPtr  odp;

  odp = (OrfDataPtr) userdata;
  if (strand == Seq_strand_plus) {

    /* top strand */

    idx = frame;
    if (odp->inorf [idx]) {
      if (orfStop) {
        odp->inorf [idx] = FALSE;
        if (odp->curlen [idx] > odp->bestlen [idx]) {
          odp->bestlen [idx] = odp->curlen [idx];
          odp->beststart [idx] = odp->currstart [idx];
        }
      } else {
        (odp->curlen [idx])++;
      }
    } else if (atgStart || (altStart && odp->altstart)) {
      odp->inorf [idx] = TRUE;
      odp->curlen [idx] = 1;
      odp->currstart [idx] = position - frame;
    }
  } else {

    /* bottom strand */

    idx = frame + 3;
    if (orfStop) {
      odp->curlen [idx] = 0;
      odp->sublen [idx] = 0;
      odp->currstart [idx] = position - frame;
    } else if (atgStart || (altStart && odp->altstart)) {
      (odp->sublen [idx])++;
      odp->curlen [idx] = odp->sublen [idx];
      if (odp->curlen [idx] > odp->bestlen [idx]) {
        odp->bestlen [idx] = odp->curlen [idx];
        odp->beststart [idx] = odp->currstart [idx];
      }
    } else {
      (odp->sublen [idx])++;
    }
  }
}

static SeqFeatPtr AnnotateBestOrf (
  BioseqPtr bsp,
  Int2 genCode,
  Boolean altstart,
  SqnTagPtr stp
  
)

{
  SeqFeatPtr      cds = NULL;
  CdRegionPtr     crp;
  GeneRefPtr      grp;
  Int2            i, best, idx;
  OrfData         od;
  ProtRefPtr      prp;
  SeqFeatPtr      sfp;
  SeqInt          sint;
  CharPtr         str;
  TransTablePtr   tbl;
  ValNode         vn;
  SeqFeatXrefPtr  xref;

  if (bsp == NULL) return NULL;
  for (i = 0; i < 6; i++) {
    od.curlen [i] = INT4_MIN;
    od.bestlen [i] = 0;
    od.currstart [i] = 0;
    od.beststart [i] = 0;
    od.sublen [i] = INT4_MIN;
    od.inorf [i] = FALSE;
  }
  od.altstart = altstart;

  /* use simultaneous 6-frame translation finite state machine */

  tbl = PersistentTransTableByGenCode (genCode);
  if (tbl != NULL) {
    TransTableProcessBioseq (tbl, LookForOrfs, (Pointer) &od, bsp);
  }
  /* TransTableFree (tbl); - now using persistent tables, free at end */
  best = -1;
  idx = -1;
  for (i = 0; i < 6; i++) {
    if (od.bestlen [i] > best) {
      best = od.bestlen [i];
      idx = i;
    }
  }
  if (idx == -1) return NULL;

  /* make feature location on largest ORF */

  if (idx < 3) {
    MemSet ((Pointer) &sint, 0, sizeof (SeqInt));
    sint.from = od.beststart [idx] + idx;
    sint.to = sint.from + (od.bestlen [idx]) * 3 + 2;
    sint.id = SeqIdFindBest (bsp->id, 0);
    sint.strand = Seq_strand_plus;
    vn.choice = SEQLOC_INT;
    vn.extended = 0;
    vn.data.ptrvalue = (Pointer) &sint;
    vn.next = NULL;
  } else {
    MemSet ((Pointer) &sint, 0, sizeof (SeqInt));
    sint.from = od.beststart [idx] + idx - 3;
    sint.to = sint.from + (od.bestlen [idx]) * 3 + 2;
    sint.id = SeqIdFindBest (bsp->id, 0);
    sint.strand = Seq_strand_minus;
    vn.choice = SEQLOC_INT;
    vn.extended = 0;
    vn.data.ptrvalue = (Pointer) &sint;
    vn.next = NULL;
  }

  /* make CDS feature with unknown product - now check [protein=...] */

  cds = CreateNewFeatureOnBioseq (bsp, SEQFEAT_CDREGION, &vn);
  if (cds == NULL) return NULL;
  crp = CreateNewCdRgn (1, FALSE, genCode);
  if (crp == NULL) return NULL;
  crp->frame = 1;
  cds->data.value.ptrvalue = (Pointer) crp;

  prp = ProtRefNew ();
  if (prp == NULL) return cds;
  xref = SeqFeatXrefNew ();
  if (xref == NULL) return cds;
  xref->data.choice = SEQFEAT_PROT;
  xref->data.value.ptrvalue = (Pointer) prp;
  xref->next = cds->xref;
  cds->xref = xref;
  prp = ParseTitleIntoProtRef (stp, prp);
  if (prp->name == NULL) {
    prp->name = ValNodeCopyStr (NULL, 0, "unknown");
  }

  /* parse CDS comment ("note" goes to biosource) and experimental evidence */

  str = SqnTagFind (stp, "comment");
  if (! StringHasNoText (str)) {
    cds->comment = StringSave (str);
  }

  str = SqnTagFind (stp, "evidence");
  if (StringICmp (str, "experimental") == 0) {
    cds->exp_ev = 1;
  }

  /* now check [gene=...], make gene feature if locus or synonym present */

  grp = GeneRefNew ();
  if (grp == NULL) return cds;
  grp = ParseTitleIntoGeneRef (stp, grp);
  if (grp->locus == NULL && grp->syn == NULL) {
    GeneRefFree (grp);
    return cds;
  }
  sfp = CreateNewFeatureOnBioseq (bsp, SEQFEAT_GENE, NULL);
  if (sfp == NULL) return cds;
  sfp->data.value.ptrvalue = (Pointer) grp;

  return cds;
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

/* source information for several common organisms sequenced by genome centers */

typedef struct orgstuff {
  CharPtr  taxname;
  CharPtr  common;
  CharPtr  lineage;
  CharPtr  division;
  Uint1    gcode;
  Uint1    mgcode;
  Int4     taxID;
} OrgStuff, PNTR OrfStuffPtr;

static OrgStuff commonOrgStuff [] = {
  {
    "Saccharomyces cerevisiae", "baker's yeast",
    "Eukaryota; Fungi; Ascomycota; Saccharomycetes; Saccharomycetales; Saccharomycetaceae; Saccharomyces",
    "PLN", 1, 3, 4932
  },
  {
    "Drosophila melanogaster", "fruit fly",
    "Eukaryota; Metazoa; Arthropoda; Tracheata; Hexapoda; Insecta; Pterygota; Neoptera; Endopterygota; Diptera; Brachycera; Muscomorpha; Ephydroidea; Drosophilidae; Drosophila",
    "INV", 1, 5, 7227
  },
  {
    "Homo sapiens", "human",
    "Eukaryota; Metazoa; Chordata; Craniata; Vertebrata; Euteleostomi; Mammalia; Eutheria; Primates; Catarrhini; Hominidae; Homo",
    "PRI", 1, 2, 9606
  },
  {
    "Escherichia coli", "",
    "Bacteria; Proteobacteria; gamma subdivision; Enterobacteriaceae; Escherichia",
    "BCT", 11, 0, 562
  },
  {
    "Helicobacter pylori", "",
    "Bacteria; Proteobacteria; epsilon subdivision; Helicobacter group; Helicobacter",
    "BCT", 11, 0, 210
  },
  {
    "Arabidopsis thaliana", "thale cress",
    "Eukaryota; Viridiplantae; Embryophyta; Tracheophyta; Spermatophyta; Magnoliophyta; eudicotyledons; core eudicots; Rosidae; eurosids II; Brassicales; Brassicaceae; Arabidopsis",
    "PLN", 1, 1, 3702
  },
  {
    "Mus musculus", "house mouse",
    "Eukaryota; Metazoa; Chordata; Craniata; Vertebrata; Euteleostomi; Mammalia; Eutheria; Rodentia; Sciurognathi; Muridae; Murinae; Mus",
    "ROD", 1, 2, 10090
  },
  {
    "Rattus norvegicus", "Norway rat",
    "Eukaryota; Metazoa; Chordata; Craniata; Vertebrata; Euteleostomi; Mammalia; Eutheria; Rodentia; Sciurognathi; Muridae; Murinae; Rattus",
    "ROD", 1, 2, 10116
  },
  {
    "Danio rerio", "zebrafish",
    "Eukaryota; Metazoa; Chordata; Craniata; Vertebrata; Euteleostomi; Actinopterygii; Neopterygii; Teleostei; Euteleostei; Ostariophysi; Cypriniformes; Cyprinidae; Rasborinae; Danio",
    "VRT", 1, 2, 7955
  },
  {
    "Zea mays", "",
    "Eukaryota; Viridiplantae; Embryophyta; Tracheophyta; Spermatophyta; Magnoliophyta; Liliopsida; Poales; Poaceae; Zea",
    "PLN", 1, 1, 4577
  },
  {
    "Caenorhabditis elegans", "",
    "Eukaryota; Metazoa; Nematoda; Chromadorea; Rhabditida; Rhabditoidea; Rhabditidae; Peloderinae; Caenorhabditis",
    "INV", 1, 5, 6239
  },
  {
    "Caenorhabditis briggsae", "",
    "Eukaryota; Metazoa; Nematoda; Chromadorea; Rhabditida; Rhabditoidea; Rhabditidae; Peloderinae; Caenorhabditis",
    "INV", 1, 5, 6238
  },
  {
    "Anopheles gambiae", "African malaria mosquito",
    "Eukaryota; Metazoa; Arthropoda; Tracheata; Hexapoda; Insecta; Pterygota; Neoptera; Endopterygota; Diptera; Nematocera; Culicoidea; Anopheles",
    "INV", 1, 5, 7165
  },
  {
    "Anopheles gambiae str. PEST", "African malaria mosquito",
    "Eukaryota; Metazoa; Arthropoda; Tracheata; Hexapoda; Insecta; Pterygota; Neoptera; Endopterygota; Diptera; Nematocera; Culicoidea; Anopheles",
    "INV", 1, 5, 180454
  },
  {
    "Tetrahymena thermophila", "",
    "Eukaryota; Alveolata; Ciliophora; Oligohymenophorea; Hymenostomatida; Tetrahymenina; Tetrahymena",
    "INV", 6, 4, 5911
  },
  {
    NULL, NULL, NULL, 0, 0, 0
  }
};

static Boolean HasTaxon (OrgRefPtr orp)

{
  ValNodePtr  db;
  DbtagPtr    dbt;

  if (orp == FALSE) return FALSE;
  for (db = orp->db; db != NULL; db = db->next) {
    dbt = (DbtagPtr) db->data.ptrvalue;
    if (dbt != NULL && dbt->db != NULL &&
        StringICmp (dbt->db, "taxon") == 0) return TRUE;
  }
  return FALSE;
}

static void AddMissingSourceInfo (BioSourcePtr biop)

{
  ValNodePtr   db;
  DbtagPtr     dbt;
  Int2         idx;
  ObjectIdPtr  oip;
  OrgNamePtr   onp;
  OrgRefPtr    orp;
  OrfStuffPtr  osp;

  if (biop == NULL) return;
  orp = biop->org;
  if (orp == NULL) return;
  onp = orp->orgname;
  if (onp == NULL) return;

  /* look for entry of organisms in commonOrgStuff table */

  for (idx = 0; commonOrgStuff [idx].taxname != NULL; idx++) {
    osp = &(commonOrgStuff [idx]);
    if (StringICmp (orp->taxname, osp->taxname) == 0) {
      if (StringHasNoText (orp->common) && (! StringHasNoText (osp->common))) {
        orp->common = StringSave (osp->common);
      }
      if (onp->gcode == 0) {
        onp->gcode = osp->gcode;
      }
      if (onp->mgcode == 0) {
        onp->mgcode = osp->mgcode;
      }
      if (StringHasNoText (onp->div)) {
        onp->div = StringSave (osp->division);
      }
      if (StringHasNoText (onp->lineage)) {
        onp->lineage = StringSave (osp->lineage);
      }
      if (! HasTaxon (orp)) {
        db = ValNodeNew (NULL);
        if (db != NULL) {
          dbt = DbtagNew ();
          if (dbt != NULL) {
            oip = ObjectIdNew ();
            if (oip != NULL) {
              oip->id = osp->taxID;
              dbt->db = StringSave ("taxon");
              dbt->tag = oip;
              db->data.ptrvalue = (Pointer) dbt;
              orp->db = db;
            }
          }
        }
      }
    }
  }
}

static BioseqPtr SqnGetBioseqGivenSeqLoc (SeqLocPtr slp, Uint2 entityID)

{
  BioseqPtr    bsp;
  SeqEntryPtr  sep;
  SeqIdPtr     sip;
  SeqLocPtr    tmp;

  if (slp == NULL) return NULL;
  bsp = NULL;
  sip = SeqLocId (slp);
  if (sip != NULL) {
    bsp = BioseqFind (sip);
  } else {
    tmp = SeqLocFindNext (slp, NULL);
    if (tmp != NULL) {
      sip = SeqLocId (tmp);
      if (sip != NULL) {
        bsp = BioseqFind (sip);
        if (bsp != NULL) {
          sep = SeqMgrGetSeqEntryForData (bsp);
          entityID = ObjMgrGetEntityIDForChoice (sep);
          bsp = GetBioseqGivenSeqLoc (slp, entityID);
        }
      }
    }
  }
  return bsp;
}

static BioseqPtr GetBioseqReferencedByAnnot (SeqAnnotPtr sap, Uint2 entityID)

{
  SeqAlignPtr   align;
  BioseqPtr     bsp;
  DenseDiagPtr  ddp;
  DenseSegPtr   dsp;
  SeqFeatPtr    feat;
  SeqGraphPtr   graph;
  SeqIdPtr      sip;
  SeqLocPtr     slp;
  StdSegPtr     ssp;
  SeqLocPtr     tloc;

  if (sap == NULL) return NULL;
  switch (sap->type) {
    case 1 :
      feat = (SeqFeatPtr) sap->data;
      while (feat != NULL) {
        slp = feat->location;
        if (slp != NULL) {
          bsp = SqnGetBioseqGivenSeqLoc (slp, entityID);
          if (bsp != NULL) return bsp;
        }
        feat = feat->next;
      }
      break;
    case 2 :
      align = (SeqAlignPtr) sap->data;
      while (align != NULL) {
        if (align->segtype == 1) {
          ddp = (DenseDiagPtr) align->segs;
          if (ddp != NULL) {
            for (sip = ddp->id; sip != NULL; sip = sip->next) {
              bsp = BioseqFind (sip);
              if (bsp != NULL) return bsp;
            }
          }
        } else if (align->segtype == 2) {
          dsp = (DenseSegPtr) align->segs;
          if (dsp != NULL) {
            for (sip = dsp->ids; sip != NULL; sip = sip->next) {
              bsp = BioseqFind (sip);
              if (bsp != NULL) return bsp;
            }
          }
        } else if (align->segtype == 3) {
          ssp = (StdSegPtr) align->segs;
          if (ssp != NULL && ssp->loc != NULL) {
            for (tloc = ssp->loc; tloc != NULL; tloc = tloc->next) {
              bsp = BioseqFind (SeqLocId (tloc));
              if (bsp != NULL) return bsp;
            }
          }
        }
        align = align->next;
      }
      break;
    case 3 :
      graph = (SeqGraphPtr) sap->data;
      while (graph != NULL) {
        slp = graph->loc;
        if (slp != NULL) {
          bsp = SqnGetBioseqGivenSeqLoc (slp, entityID);
          if (bsp != NULL) return bsp;
        }
        graph = graph->next;
      }
      break;
    default :
      break;
  }
  return NULL;
}

static BioseqPtr AttachSeqAnnotEntity (Uint2 entityID, SeqAnnotPtr sap)

{
  BioseqPtr      bsp;
  Int2           genCode;
  SeqEntryPtr    oldscope;
  OMProcControl  ompc;
  SeqEntryPtr    sep;
  SeqFeatPtr     sfp = NULL;

  if (sap == NULL) return NULL;
  bsp = GetBioseqReferencedByAnnot (sap, entityID);
  if (bsp == NULL) {
    oldscope = SeqEntrySetScope (NULL);
    if (oldscope != NULL) {
      bsp = GetBioseqReferencedByAnnot (sap, entityID);
      SeqEntrySetScope (oldscope);
    }
  }
  if (bsp != NULL) {
    sep = SeqMgrGetSeqEntryForData (bsp);
    entityID = ObjMgrGetEntityIDForChoice (sep);
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      sep = GetBestTopParentForData (entityID, bsp);
      genCode = SeqEntryToGeneticCode (sep, NULL, NULL, 0);
      SetEmptyGeneticCodes (sap, genCode);
    }
    MemSet ((Pointer) &ompc, 0, sizeof (OMProcControl));
    ompc.input_entityID = entityID;
    ompc.input_itemID = GetItemIDGivenPointer (entityID, OBJ_BIOSEQ, (Pointer) bsp);
    ompc.input_itemtype = OBJ_BIOSEQ;
    ompc.output_itemtype = OBJ_SEQANNOT;
    ompc.output_data = (Pointer) sap;
    if (! AttachDataForProc (&ompc, FALSE)) {
      Message (MSG_POSTERR, "AttachSeqAnnotEntity failed");
    } else if (sfp != NULL) {
      PromoteXrefs (sfp, bsp, entityID);
    }
  } else {
    Message (MSG_POSTERR, "Feature table identifiers do not match record");
  }
  return bsp;
}

static CharPtr TrimBracketsFromString (CharPtr str)

{
  Uchar    ch;	/* to use 8bit characters in multibyte languages */
  CharPtr  dst;
  CharPtr  ptr;

  if (StringHasNoText (str)) return str;

  /* remove bracketed fields */

  dst = str;
  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    if (ch == '[') {
      ptr++;
      ch = *ptr;
      while (ch != '\0' && ch != ']') {
        ptr++;
        ch = *ptr;
      }
      ptr++;
      ch = *ptr;
    } else {
      *dst = ch;
      dst++;
      ptr++;
      ch = *ptr;
    }
  }
  *dst = '\0';

  /* remove runs of whitespace characters */

  dst = str;
  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    if (IS_WHITESP (ch)) {
      *dst = ch;
      dst++;
      ptr++;
      ch = *ptr;
      while (IS_WHITESP (ch)) {
        ptr++;
        ch = *ptr;
      }
    } else {
      *dst = ch;
      dst++;
      ptr++;
      ch = *ptr;
    }
  }
  *dst = '\0';

  return str;
}

static void ProcessOneNuc (
  Uint2 entityID,
  BioseqPtr bsp,
  BioSourcePtr src,
  CharPtr organism,
  Boolean findorf,
  Boolean altstart
)

{
  BioSourcePtr  biop = NULL;
  SeqFeatPtr    cds;
  GBBlockPtr    gbp;
  Int2          genCode;
  MolInfoPtr    mip;
  Boolean       mito;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  SeqEntryPtr   sep;
  SeqHistPtr    shp;
  SqnTagPtr     stp = NULL;
  CharPtr       str;
  CharPtr       ttl = NULL;
  ValNodePtr    vnp;

  if (bsp == NULL) return;

  sep = GetBestTopParentForData (entityID, bsp);
  genCode = SeqEntryToGeneticCode (sep, NULL, NULL, 0);

  if (bsp->mol == Seq_mol_na) {
    bsp->mol = Seq_mol_dna;
  }

  if (src != NULL) {
    src = AsnIoMemCopy ((Pointer) src,
                        (AsnReadFunc) BioSourceAsnRead,
                        (AsnWriteFunc) BioSourceAsnWrite);
  }

  vnp = ValNodeExtract (&(bsp->descr), Seq_descr_title);
  if (vnp != NULL) {
    ttl = (CharPtr) vnp->data.ptrvalue;
    if (ttl != NULL) {
      stp = SqnTagParse (ttl);
    }
  }

  if (stp != NULL) {
    biop = ParseTitleIntoBioSource (stp, organism, src);
    ParseTitleIntoBioseq (stp, bsp);
  }
  if (biop == NULL) {
    biop = ParseTitleIntoBioSource (NULL, organism, src);
  }
  if (biop != NULL) {
    SeqDescrAddPointer (&(bsp->descr), Seq_descr_source, (Pointer) biop);
    AddMissingSourceInfo (biop);
  }

  if (BioseqGetSeqDescr (bsp, Seq_descr_molinfo, NULL) == NULL) {
    mip = MolInfoNew ();
    if (mip != NULL) {
      if (stp != NULL) {
        mip = ParseTitleIntoMolInfo (stp, mip);
      }
      if (mip->biomol == 0) {
        mip->biomol = MOLECULE_TYPE_GENOMIC;
      }
      SeqDescrAddPointer (&(bsp->descr), Seq_descr_molinfo, (Pointer) mip);
    }
  }

  if (genCode == 0 && biop != NULL) {
    orp = biop->org;
    if (orp != NULL) {
      onp = orp->orgname;
      if (onp != NULL) {
        mito = (Boolean) (biop->genome == 4 || biop->genome == 5);
        if (mito) {
          genCode = onp->mgcode;
        } else {
          genCode = onp->gcode;
        }
      }
    }
  }

  if (stp != NULL) {
    gbp = ParseTitleIntoGenBank (stp, NULL);
    if (gbp != NULL && gbp->extra_accessions != NULL) {
      SeqDescrAddPointer (&(bsp->descr), Seq_descr_genbank, (Pointer) gbp);
    } else {
      gbp = GBBlockFree (gbp);
    }

    shp = ParseTitleIntoSeqHist (stp, NULL);
    if (shp != NULL && shp->replace_ids != NULL) {
      bsp->hist = SeqHistFree (bsp->hist);
      bsp->hist = shp;
    } else {
      shp = SeqHistFree (shp);
    }
  }

  if (findorf) {
    cds = AnnotateBestOrf (bsp, genCode, altstart, stp);
    if (cds != NULL) {
      PromoteXrefs (cds, bsp, entityID);
    }
  }

  if (stp != NULL) {
    SqnTagFree (stp);
  }

  TrimBracketsFromString (ttl);
  if (! StringHasNoText (ttl)) {
    str = StringSave (ttl);
    SeqDescrAddPointer (&(bsp->descr), Seq_descr_title, (Pointer) str);
  }

  ValNodeFreeData (vnp);
}

static void ProcessOneAnnot (
  SeqAnnotPtr sap,
  Uint2 entityID,
  CharPtr accn
)

{
  BioseqPtr    bsp;
  Int2         genCode;
  SeqEntryPtr  sep;
  SeqFeatPtr   sfp;
  SeqIdPtr     sip;

  if (sap == NULL) return;

  bsp = AttachSeqAnnotEntity (entityID, sap);
  if (bsp == NULL) return;

  sep = GetBestTopParentForData (entityID, bsp);
  genCode = SeqEntryToGeneticCode (sep, NULL, NULL, 0);

  /* if existing accession, coerce all SeqIds */

  if (! StringHasNoText (accn)) {
    sip = SeqIdFromAccession (accn, 0, NULL);
    if (sip != NULL) {
      bsp->id = SeqIdSetFree (bsp->id);
      bsp->id = sip;
      SeqMgrReplaceInBioseqIndex (bsp);
      VisitFeaturesOnBsp (bsp, (Pointer) bsp->id, CorrectFeatureSeqIds);
    }
  }

  /* for parsed in features or best ORF, promote CDS products to protein bioseq */

  for (sap = bsp->annot; sap != NULL; sap = sap->next) {
    if (sap->type == 1) {
      SetEmptyGeneticCodes (sap, genCode);
      sfp = (SeqFeatPtr) sap->data;
      PromoteXrefs (sfp, bsp, entityID);
    }
  }
}

static void ReplaceOnePeptide (
  SimpleSeqPtr ssp,
  Boolean conflict
)

{
  Uint1         aa;
  ByteStorePtr  bs;
  BioseqPtr     bsp;
  SeqFeatPtr    cds;
  CdRegionPtr   crp;
  SeqFeatPtr    prt;
  SeqIntPtr     sintp;
  SeqIdPtr      sip;
  SeqLocPtr     slp;
  CharPtr       str1, str2;

  if (ssp == NULL || ssp->numid < 1) return;

  sip = MakeSeqID (ssp->id [0]);
  bsp = BioseqFind (sip);
  SeqIdFree (sip);
  if (bsp == NULL || bsp->repr != Seq_repr_raw) return;

  /* remove trailing X and * */

  bs = ssp->seq;
  BSSeek (bs, -1, SEEK_END);
  aa = (Uint1) BSGetByte (bs);
  while ((aa == 'X' || aa == '*') && ssp->seqlen > 0) {
    BSSeek (bs, -1, SEEK_END);
    BSDelete (bs, 1);
    BSSeek (bs, -1, SEEK_END);
    aa = (Uint1) BSGetByte (bs);
  }
  ssp->seqlen = BSLen (bs);

  str1 = BSMerge (ssp->seq, NULL);
  str2 = BSMerge (bsp->seq_data, NULL);

  if (StringCmp (str1, str2) != 0) {

    /* swap sequence byte stores */

    bs = bsp->seq_data;
    bsp->seq_data = ssp->seq;
    ssp->seq = bs;
    bsp->length = BSLen (bsp->seq_data);
    bsp->seq_data_type = Seq_code_ncbieaa;

    cds = SeqMgrGetCDSgivenProduct (bsp, NULL);
    if (cds != NULL) {
      crp = (CdRegionPtr) cds->data.value.ptrvalue;

      /* conditionally set CDS conflict flag, suppress validator complaint */

      if (crp != NULL && conflict) {
        crp->conflict = TRUE;
      }
    }

    prt = SeqMgrGetBestProteinFeature (bsp, NULL);
    if (prt != NULL) {
      slp = prt->location;
      if (slp != NULL && slp->choice == SEQLOC_INT) {
        sintp = (SeqIntPtr) slp->data.ptrvalue;
        if (sintp != NULL) {
          sintp->to = bsp->length - 1;
        }
      }
    }
  }

  MemFree (str1);
  MemFree (str2);
}

static Uint2 ProcessOneAsn (
  FILE* fp,
  BioSourcePtr src,
  CharPtr organism,
  Boolean findorf,
  Boolean altstart
)

{
  BioseqPtr    bsp = NULL;
  Pointer      dataptr;
  Uint2        datatype, entityID;
  SeqEntryPtr  sep;

  if (fp == NULL) return 0;

  dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, &entityID, FALSE, FALSE, TRUE, FALSE);
  if (dataptr == NULL) return 0;

  sep = GetTopSeqEntryForEntityID (entityID);
  bsp = FindNucBioseq (sep);
  if (bsp == NULL) {
    ObjMgrFreeByEntityID (entityID);
    return 0;
  }

  ProcessOneNuc (entityID, bsp, src, organism, findorf, altstart);

  return entityID;
}

static Uint2 ProcessAsnSet (
  FILE* fp,
  BioSourcePtr src,
  CharPtr organism,
  Boolean findorf,
  Boolean altstart
)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Pointer       dataptr;
  Uint2         datatype, entityID;
  SeqEntryPtr   sep, topsep;

  bssp = BioseqSetNew ();
  if (bssp == NULL) return 0;
  bssp->_class = BioseqseqSet_class_genbank;

  topsep = SeqEntryNew ();
  if (topsep == NULL) return 0;
  topsep->choice = 2;
  topsep->data.ptrvalue = (Pointer) bssp;

  entityID = ObjMgrRegister (OBJ_BIOSEQSET, (Pointer) bssp);

  while ((dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, FALSE, TRUE, FALSE)) != NULL) {
    if (datatype == OBJ_BIOSEQ) {

      sep = SeqMgrGetSeqEntryForData (dataptr);
      AddSeqEntryToSeqEntry (topsep, sep, FALSE);

      bsp = (BioseqPtr) dataptr;
      ProcessOneNuc (entityID, bsp, src, organism, findorf, altstart);

    } else {
      ObjMgrFree (datatype, dataptr);
    }
  }

  SeqMgrLinkSeqEntry (topsep, 0, NULL);

  return entityID;
}

static void ProcessOneRecord (
  SubmitBlockPtr sbp,
  PubdescPtr pdp,
  BioSourcePtr src,
  CharPtr directory,
  CharPtr results,
  CharPtr base,
  CharPtr suffix,
  Boolean fastaset,
  CharPtr accn,
  CharPtr organism,
  SeqDescrPtr sdphead,
  Boolean findorf,
  Boolean altstart,
  Boolean conflict,
  Boolean validate,
  Boolean flatfile
)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Pointer       dataptr;
  Uint2         datatype, entityID;
  DatePtr       dp;
  FILE          *fp;
  SeqAnnotPtr   sap;
  SeqDescrPtr   sdp;
  SeqEntryPtr   sep;
  SimpleSeqPtr  ssp;

  fp = OpenOneFile (directory, base, suffix);
  if (fp == NULL) return;

  /* read one or more ASN.1 or FASTA sequence files */

  if (fastaset) {
    entityID = ProcessAsnSet (fp, src, organism, findorf, altstart);
  } else {
    entityID = ProcessOneAsn (fp, src, organism, findorf, altstart);
  }
  FileClose (fp);

  if (entityID == 0) return;

  /* read one or more feature tables from .tbl file */

  fp = OpenOneFile (directory, base, ".tbl");
  if (fp != NULL) {

    while ((dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, FALSE, TRUE, FALSE)) != NULL) {
      if (datatype == OBJ_SEQANNOT) {

        sap = (SeqAnnotPtr) dataptr;
        ProcessOneAnnot (sap, entityID, accn);

      } else {
        ObjMgrFree (datatype, dataptr);
      }
    }
    FileClose (fp);
  }

  /* read one or more feature tables from .pep file */

  fp = OpenOneFile (directory, base, ".pep");
  if (fp != NULL) {

    /* indexing needed to find CDS from protein product to set conflict flag */

    SeqMgrIndexFeatures (entityID, NULL);

    while ((dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, FALSE, TRUE, TRUE)) != NULL) {
      if (datatype == OBJ_FASTA) {

        ssp = (SimpleSeqPtr) dataptr;
        ReplaceOnePeptide (ssp, conflict);
        SimpleSeqFree (ssp);

      } else {
        ObjMgrFree (datatype, dataptr);
      }
    }
    FileClose (fp);
  }

  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep != NULL) {
    if (sdphead != NULL) {
      if (IS_Bioseq (sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        ValNodeLink (&(bsp->descr),
                     AsnIoMemCopy ((Pointer) sdphead,
                                   (AsnReadFunc) SeqDescrAsnRead,
                                   (AsnWriteFunc) SeqDescrAsnWrite));
      } else if (IS_Bioseq_set (sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        ValNodeLink (&(bssp->descr),
                     AsnIoMemCopy ((Pointer) sdphead,
                                   (AsnReadFunc) SeqDescrAsnRead,
                                   (AsnWriteFunc) SeqDescrAsnWrite));
      }
    }
    dp = DateCurr ();
    if (dp != NULL) {
      sdp = CreateNewDescriptor (sep, Seq_descr_create_date);
      if (sdp != NULL) {
        sdp->data.ptrvalue = (Pointer) dp;
      }
    }
    SeriousSeqEntryCleanup (sep, NULL, NULL);
    WriteOneFile (results, base, ".sqn", sep, sbp);

    if (validate || flatfile) {
      if (pdp != NULL) {

        /* copy in citsub as publication for validator and flatfile */

        sdp = CreateNewDescriptor (sep, Seq_descr_pub);
        if (sdp != NULL) {
          sdp->data.ptrvalue = AsnIoMemCopy ((Pointer) pdp,
                                             (AsnReadFunc) PubdescAsnRead,
                                             (AsnWriteFunc) PubdescAsnWrite);
        }
      }
      SeqMgrIndexFeatures (entityID, 0);
    }
    if (validate) {
      Message (MSG_POST, "Validating %s\n", base);
      ValidateOneFile (results, base, ".val", sep);
    }
    if (flatfile) {
      Message (MSG_POST, "Flatfile %s\n", base);
      sep = FindNucSeqEntry (sep);
      FlatfileOneFile (results, base, ".gbf", sep);
    }
  }

  ObjMgrFreeByEntityID (entityID);
}

static void GetFirstBiop (
  BioSourcePtr biop,
  Pointer userdata
)

{
  BioSourcePtr PNTR biopp;

  biopp = (BioSourcePtr PNTR) userdata;
  if (biop == NULL || biopp == NULL) return;
  if (*biopp != NULL) return;
  *biopp = biop;
}

static CharPtr overwriteMsg = "Your template with a .sqn suffix will be overwritten.  Do you wish to continue?";

static Boolean TemplateOverwriteRisk (
  CharPtr tmplate,
  CharPtr single,
  CharPtr directory,
  CharPtr suffix
)

{
  Char     file [FILENAME_MAX], path [PATH_MAX];
  CharPtr  ptr;


  if (StringStr (tmplate, ".sqn") == NULL) return FALSE;
  if (! StringHasNoText (single)) {
    StringNCpy_0 (file, tmplate, sizeof (file));
    ptr = StringStr (file, ".");
    if (ptr != NULL) {
      *ptr = '\0';
    }
    ptr = StringStr (single, ".");
    if (ptr != NULL) {
      StringCat (file, ptr);
    }
    if (StringCmp (file, single) == 0) return TRUE;
  } else if (! StringHasNoText (directory)) {
    StringNCpy_0 (path, directory, sizeof (path));
    StringNCpy_0 (file, tmplate, sizeof (file));
    ptr = StringStr (file, ".");
    if (ptr != NULL) {
      *ptr = '\0';
    }
    StringCat (file, suffix);
    FileBuildPath (path, NULL, file);
    if (FileLength (path) > 0) return TRUE;
  }
  return FALSE;
}

/* Args structure contains command-line arguments */

#define p_argInputPath  0
#define r_argOutputPath 1
#define f_argSingleFile 2
#define x_argSuffix     3
#define t_argTemplate   4
#define s_argFastaSet   5
#define a_argAccession  6
#define n_argOrgName    7
#define c_argFindOrf    8
#define m_argAltStart   9
#define k_argConflict  10
#define v_argValidate  11
#define b_argGenBank   12

Args myargs [] = {
  {"Path to files", NULL, NULL, NULL,
    TRUE, 'p', ARG_STRING, 0.0, 0, NULL},
  {"Path for results", NULL, NULL, NULL,
    TRUE, 'r', ARG_STRING, 0.0, 0, NULL},
  {"Only this file", NULL, NULL, NULL,
    TRUE, 'f', ARG_FILE_IN, 0.0, 0, NULL},
  {"Suffix", ".fsa", NULL, NULL,
    TRUE, 'x', ARG_STRING, 0.0, 0, NULL},
  {"Template file", NULL, NULL, NULL,
    TRUE, 't', ARG_FILE_IN, 0.0, 0, NULL},
  {"Read Set of FASTAs", "F", NULL, NULL,
    TRUE, 's', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Accession", NULL, NULL, NULL,
    TRUE, 'a', ARG_STRING, 0.0, 0, NULL},
  {"Organism name", NULL, NULL, NULL,
    TRUE, 'n', ARG_STRING, 0.0, 0, NULL},
  {"Annotate longest ORF", "F", NULL, NULL,
    TRUE, 'c', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Allow alternative starts", "F", NULL, NULL,
    TRUE, 'm', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Set conflict on mismatch", "F", NULL, NULL,
    TRUE, 'k', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Validate", "F", NULL, NULL,
    TRUE, 'v', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Generate GenBank file", "F", NULL, NULL,
    TRUE, 'b', ARG_BOOLEAN, 0.0, 0, NULL},
};

Int2 Main (void)

{
  Boolean         altstart, conflict, fastaset, findorf, flatfile, validate;
  CharPtr         base, directory, results, suffix, accn, organism, ptr, tmplate;
  Pointer         dataptr;
  Uint2           datatype;
  CitSubPtr       csp;
  FILE            *fp;
  ValNodePtr      head, vnp;
  Pubdesc         pd;
  PubdescPtr      pdp = NULL;
  ValNode         pb;
  SubmitBlockPtr  sbp = NULL;
  SeqDescrPtr     sdphead = NULL;
  SeqEntryPtr     sep;
  Char            sfx [32];
  BioSourcePtr    src = NULL;
  SeqSubmitPtr    ssp = NULL;

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

  if (! GetArgs ("tbl2asn", sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  directory = (CharPtr) myargs [p_argInputPath].strvalue;
  results = (CharPtr) myargs [r_argOutputPath].strvalue;
  if (StringHasNoText (results)) {
    results = directory;
  }
  suffix = (CharPtr) myargs [x_argSuffix].strvalue;
  base = (CharPtr) myargs [f_argSingleFile].strvalue;
  tmplate = (CharPtr) myargs [t_argTemplate].strvalue;
  fastaset = (Boolean) myargs [s_argFastaSet].intvalue;
  accn = (CharPtr) myargs [a_argAccession].strvalue;
  organism = (CharPtr) myargs [n_argOrgName].strvalue;
  findorf = (Boolean) myargs [c_argFindOrf].intvalue;
  altstart = (Boolean) myargs [m_argAltStart].intvalue;
  conflict = (Boolean) myargs [k_argConflict].intvalue;
  validate = (Boolean) myargs [v_argValidate].intvalue;
  flatfile = (Boolean) myargs [b_argGenBank].intvalue;

  if (StringHasNoText (base) && (! StringHasNoText (accn))) {
    Message (MSG_FATAL, "Accession can be entered only for a single record");
    return 1;
  }

  /* Seq-submit or Submit-block template is optional */

  if (! StringHasNoText (tmplate)) {
    if (TemplateOverwriteRisk (tmplate, base, directory, suffix)) {
      if (Message (MSG_YN, overwriteMsg) == ANS_NO) return 0;
    }
    fp = FileOpen (tmplate, "r");
    if (fp != NULL) {
      while ((dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, FALSE, TRUE, FALSE)) != NULL) {
        if (datatype == OBJ_SEQSUB) {
          ssp = (SeqSubmitPtr) dataptr;
        } else if (datatype == OBJ_SUBMIT_BLOCK) {
          sbp = (SubmitBlockPtr) dataptr;
        } else if (datatype == OBJ_SEQDESC) {
          ValNodeLink (&sdphead, (SeqDescrPtr) dataptr);
        } else {
          ObjMgrFree (datatype, dataptr);
        }
      }
      FileClose (fp);
    }

    if (ssp != NULL && sbp == NULL) {
      sbp = ssp->sub;
    }
    if (sbp == NULL) {
      Message (MSG_FATAL, "Unable to read required template file");
      return 1;
    }

    if (sbp != NULL) {
      if (ssp != NULL) {

        /* copy submit block, will free SeqSubmit before processing */

        sbp = AsnIoMemCopy ((Pointer) sbp,
                            (AsnReadFunc) SubmitBlockAsnRead,
                            (AsnWriteFunc) SubmitBlockAsnWrite);
      }
      sbp->tool = MemFree (sbp->tool);
      sbp->tool = StringSave ("tbl2asn");
      sbp->hup = FALSE;
      sbp->reldate = DateFree (sbp->reldate);
      csp = sbp->cit;
      if (csp != NULL) {
        csp->date = DateFree (csp->date);
        csp->date = DateCurr ();
        MemSet ((Pointer) &pd, 0, sizeof (Pubdesc));
        MemSet ((Pointer) &pb, 0, sizeof (ValNode));
        pb.choice = PUB_Sub;
        pb.data.ptrvalue = (Pointer) csp;
        pd.pub = &pb;
        pdp = &pd;
      }
    }
    if (ssp != NULL && ssp->datatype == 1) {
      sep = (SeqEntryPtr) ssp->data;
      if (sep != NULL) {
        VisitBioSourcesInSep (sep, (Pointer) &src, GetFirstBiop);
        if (src != NULL) {

          /* copy top biosource */

          src = AsnIoMemCopy ((Pointer) src,
                              (AsnReadFunc) BioSourceAsnRead,
                              (AsnWriteFunc) BioSourceAsnWrite);
        }
      }

      /* in case template has colliding ID, free it now */

      SeqSubmitFree (ssp);
    }
  }

  /* process one or more records */

  if (! StringHasNoText (base)) {
    ptr = StringStr (base, ".");
    if (ptr != NULL) {
      StringNCpy_0 (sfx, ptr, sizeof (sfx));
      *ptr = '\0';
    }
    ProcessOneRecord (sbp, pdp, src, directory, results, base, sfx, fastaset, accn,
                      organism, sdphead, findorf, altstart, conflict, validate, flatfile);
  } else {

    /* get list of all files in source directory */

    head = DirCatalog (directory);

    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      if (vnp->choice == 0) {
        base = (CharPtr) vnp->data.ptrvalue;
        if (! StringHasNoText (base)) {
          ptr = StringStr (base, suffix);
          if (ptr != NULL) {
            *ptr = '\0';
            Message (MSG_POST, "Processing %s\n", base);
            ProcessOneRecord (sbp, pdp, src, directory, results, base, suffix, fastaset, NULL,
                              organism, sdphead, findorf, altstart, conflict, validate, flatfile);
          }
        }
      }
    }

    /* clean up file list */

    ValNodeFreeData (head);
  }

  if (sbp != NULL) {
    SubmitBlockFree (sbp);
  }
  if (src != NULL) {
    BioSourceFree (src);
  }

  SeqDescrFree (sdphead);

  TransTableFreeAll ();

  return 0;
}

