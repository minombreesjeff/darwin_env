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
* $Revision: 6.107 $
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
#include <tofasta.h>
#include <simple.h>
#include <aliparse.h>
#include <util/creaders/alnread.h>
#include <pmfapi.h>
#include <tax3api.h>

#define TBL2ASN_APP_VER "3.6"

CharPtr TBL2ASN_APPLICATION = TBL2ASN_APP_VER;

typedef struct tblargs {
  Boolean  fastaset;
  Int2     whichclass;
  Boolean  deltaset;
  Boolean  alignset;
  Boolean  genprodset;
  Boolean  forcelocalid;
  Boolean  gpstonps;
  Boolean  gnltonote;
  Boolean  removeunnecxref;
  Boolean  dotaxlookup;
  CharPtr  accn;
  CharPtr  organism;
  CharPtr  srcquals;
  CharPtr  comment;
  Boolean  findorf;
  Boolean  altstart;
  Boolean  conflict;
  Boolean  validate;
  Boolean  flatfile;
  Boolean  seqidfromfile;
  Boolean  smartfeats;
  CharPtr  aln_beginning_gap;
  CharPtr  aln_end_gap;
  CharPtr  aln_middle_gap;
  CharPtr  aln_missing;
  CharPtr  aln_match;
  Boolean  aln_is_protein;
} TblArgs, PNTR TblArgsPtr;

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
    vsp->seqSubmitParent = TRUE;
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
  SeqEntryToGnbk (sep, NULL, GENBANK_FMT, ENTREZ_MODE, NORMAL_STYLE, 0, 0, 0, NULL, fp);
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
  if (prp->name == NULL && prp->desc == NULL) {
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

static void PromoteSeqId (
  SeqIdPtr sip,
  Pointer userdata
)

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
    "Pan troglodytes", "chimpanzee",
    "Eukaryota; Metazoa; Chordata; Craniata; Vertebrata; Euteleostomi; Mammalia; Eutheria; Primates; Catarrhini; Hominidae; Pan",
    "PRI", 1, 2, 9598
  },
  {
    "Candida albicans", "",
    "Eukaryota; Fungi; Ascomycota; Saccharomycotina; Saccharomycetes; Saccharomycetales; mitosporic Saccharomycetales; Candida",
    "PLN", 12, 4, 5476
  },
  {
    "Candida albicans SC5314", "",
    "Eukaryota; Fungi; Ascomycota; Saccharomycotina; Saccharomycetes; Saccharomycetales; mitosporic Saccharomycetales; Candida",
    "PLN", 12, 4, 237561
  },
  {
    "Trypanosoma brucei", "",
    "Eukaryota; Euglenozoa; Kinetoplastida; Trypanosomatidae; Trypanosoma",
    "INV", 1, 4, 5691
  },
  {
    "Trypanosoma cruzi", "",
    "Eukaryota; Euglenozoa; Kinetoplastida; Trypanosomatidae; Trypanosoma; Schizotrypanum",
    "INV", 1, 4, 5693
  },
  {
    "Oryza sativa", "",
    "Eukaryota; Viridiplantae; Streptophyta; Embryophyta; Tracheophyta; Spermatophyta; Magnoliophyta; Liliopsida; Poales; Poaceae; Ehrhartoideae; Oryzeae; Oryza",
    "PLN", 1, 1, 4530
  },
  {
    "Oryza sativa (indica cultivar-group)", "",
    "Eukaryota; Viridiplantae; Streptophyta; Embryophyta; Tracheophyta; Spermatophyta; Magnoliophyta; Liliopsida; Poales; Poaceae; Ehrhartoideae; Oryzeae; Oryza",
    "PLN", 1, 1, 39946
  },
  {
    "Oryza sativa (japonica cultivar-group)", "",
    "Eukaryota; Viridiplantae; Streptophyta; Embryophyta; Tracheophyta; Spermatophyta; Magnoliophyta; Liliopsida; Poales; Poaceae; Ehrhartoideae; Oryzeae; Oryza",
    "PLN", 1, 1, 39947
  },
  {
    "Aspergillus nidulans FGSC A4", "",
    "Eukaryota; Fungi; Ascomycota; Pezizomycotina; Eurotiomycetes; Eurotiales; Trichocomaceae; Emericella",
    "PLN", 1, 4, 227321
  },
  {
    "environmental sequence", "",
    "unclassified; environmental samples",
    "UNA", 1, 2, 256318
  },
  {
    NULL, NULL, NULL, NULL, 0, 0, 0
  }
};

static Boolean HasTaxon (
  OrgRefPtr orp
)

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

static void AddMissingSourceInfo (
  BioSourcePtr biop
)

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

static BioseqPtr GetBioseqReferencedByAnnot (
  SeqAnnotPtr sap,
  Uint2 entityID
)

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
          bsp = BioseqFindFromSeqLoc (slp);
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
              bsp = BioseqFindFromSeqLoc (tloc);
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
          bsp = BioseqFindFromSeqLoc (slp);
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

static Int2 GetGenCodeForBsp (
  BioseqPtr bsp
)

{
  BioSourcePtr  biop;
  Boolean       mito;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  SeqDescrPtr   sdp;

  sdp = GetNextDescriptorUnindexed (bsp, Seq_descr_source, NULL);
  if (sdp == NULL) return 1;
  biop = (BioSourcePtr) sdp->data.ptrvalue;
  if (biop == NULL) return 1;
  orp = biop->org;
  if (orp == NULL) return 1;
  onp = orp->orgname;
  if (onp == NULL) return 1;
  mito = (Boolean) (biop->genome == 4 || biop->genome == 5);
  if (mito) {
    if (onp->mgcode == 0) {
      return 1;
    }
    return onp->mgcode;
  }
  if (onp->gcode == 0) {
    return 1;
  }
  return onp->gcode;
}

typedef struct gcmdata {
  SeqFeatPtr  gene;
  SeqFeatPtr  feat;
} GmcData, PNTR GmcDataPtr;

static int LIBCALLBACK SortByGenePtr (
  VoidPtr vp1,
  VoidPtr vp2
)

{
  GmcDataPtr gdp1, gdp2;

  if (vp1 == NULL || vp2 == NULL) return 0;
  gdp1 = (GmcDataPtr) vp1;
  gdp2 = (GmcDataPtr) vp2;
  if (gdp1 == NULL || gdp2 == NULL) return 0;

  if (gdp1->gene > gdp2->gene) return -1;
  if (gdp1->gene < gdp2->gene) return 1;

  if (gdp1->feat > gdp2->feat) return -1;
  if (gdp1->feat < gdp2->feat) return 1;

  return 0;
}

static void EnhanceOneCDS (
  SeqFeatPtr sfp,
  Boolean alt_splice
)

{
  DbtagPtr        dbt;
  GBQualPtr       gbq;
  Char            id [64];
  SeqIdPtr        ids, sip;
  ObjectIdPtr     oip;
  ProtRefPtr      prp;
  CharPtr         ptr, str;
  Char            tmp [256];
  SeqFeatXrefPtr  xref;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION) return;

  prp = NULL;
  for (xref = sfp->xref; xref != NULL; xref = xref->next) {
    if (xref->data.choice == SEQFEAT_PROT) {
      prp = (ProtRefPtr) xref->data.value.ptrvalue;
    }
  }

  id [0] = '\0';
  for (gbq = sfp->qual; gbq != NULL; gbq = gbq->next) {
    if (StringICmp (gbq->qual, "protein_id") == 0) {
      StringNCpy_0 (id, gbq->val, sizeof (id));
    }
  }
  if (StringDoesHaveText (id) && StringChr (id, '|') != NULL) {
    str = NULL;
    ids = SeqIdParse (id);
    for (sip = ids; sip != NULL; sip = sip->next) {
      if (sip->choice != SEQID_GENERAL) continue;
      dbt = (DbtagPtr) sip->data.ptrvalue;
      if (dbt == NULL) continue;
      if (StringICmp (dbt->db, "TMSMART") == 0) continue;
      if (StringICmp (dbt->db, "BankIt") == 0) continue;
      oip = dbt->tag;
      if (oip == NULL) continue;
      str = oip->str;
    }

    if (StringDoesHaveText (str)) {
      if (prp != NULL && prp->name != NULL) {
        if (alt_splice) {
          AddQualifierToFeature (sfp, "product", str);
        } else {
          AddQualifierToFeature (sfp, "product", str);
        }
      } else {
        if (alt_splice) {
          ptr = StringChr (str, '-');
          if (ptr != NULL && StringLen (ptr) == 3) {
            ptr++;
            ptr++;
            sprintf (tmp, "%s, isoform %s", str, ptr);
            AddQualifierToFeature (sfp, "product", tmp);
            /*
            AddQualifierToFeature (sfp, "product", str);
            */
          } else {
            AddQualifierToFeature (sfp, "product", str);
          }
        } else {
          AddQualifierToFeature (sfp, "product", str);
        }
      }
    }

    SeqIdSetFree (ids);
  }
}

static void EnhanceOneMrna (
  SeqFeatPtr sfp,
  Boolean alt_splice
)

{
  DbtagPtr     dbt;
  GBQualPtr    gbq;
  Char         id [64];
  SeqIdPtr     ids, sip;
  CharPtr      name, ptr, str;
  ObjectIdPtr  oip;
  RnaRefPtr    rrp;
  Char         tmp [256];

  if (sfp == NULL || sfp->data.choice != SEQFEAT_RNA) return;

  name = NULL;
  rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
  if (rrp != NULL && rrp->type == 2 && rrp->ext.choice == 1) {
    name = rrp->ext.value.ptrvalue;
  }

  id [0] = '\0';
  for (gbq = sfp->qual; gbq != NULL; gbq = gbq->next) {
    if (StringICmp (gbq->qual, "transcript_id") == 0) {
      StringNCpy_0 (id, gbq->val, sizeof (id));
    }
  }
  if (StringDoesHaveText (id) && StringChr (id, '|') != NULL) {
    str = NULL;
    ids = SeqIdParse (id);
    for (sip = ids; sip != NULL; sip = sip->next) {
      if (sip->choice != SEQID_GENERAL) continue;
      dbt = (DbtagPtr) sip->data.ptrvalue;
      if (dbt == NULL) continue;
      if (StringICmp (dbt->db, "TMSMART") == 0) continue;
      if (StringICmp (dbt->db, "BankIt") == 0) continue;
      oip = dbt->tag;
      if (oip == NULL) continue;
      str = oip->str;
    }

    if (StringDoesHaveText (str)) {
      if (StringDoesHaveText (name)) {
        if (alt_splice) {
          AddQualifierToFeature (sfp, "note", str);
        } else {
          AddQualifierToFeature (sfp, "note", str);
        }
      } else {
        if (alt_splice) {
          ptr = StringChr (str, '-');
          if (ptr != NULL && StringLen (ptr) == 3) {
            ptr++;
            ptr++;
            sprintf (tmp, "%s, transcript variant %s", str, ptr);
            AddQualifierToFeature (sfp, "product", tmp);
            /*
            AddQualifierToFeature (sfp, "note", str);
            */
          } else {
            AddQualifierToFeature (sfp, "product", str);
          }
        } else {
          AddQualifierToFeature (sfp, "product", str);
        }
      }
    }

    SeqIdSetFree (ids);
  }
}

static void EnhanceFeatureAnnotation (
  SeqFeatPtr features,
  BioseqPtr bsp
)

{
  GmcDataPtr  gdp, head;
  GeneRefPtr  grp;
  Int2        i, j, k, numgene, numcds, nummrna;
  SeqFeatPtr  sfp;

  if (features == NULL || bsp == NULL) return;

  numgene = 0;
  numcds = 0;
  nummrna = 0;

  for (sfp = features; sfp != NULL; sfp = sfp->next) {
    switch (sfp->idx.subtype) {
      case FEATDEF_GENE :
        numgene++;
        break;
      case FEATDEF_CDS :
        numcds++;
        break;
      case FEATDEF_mRNA :
        nummrna++;
        break;
      default :
        break;
    }
  }

  if (numgene == 0) return;

  if (numcds > 0) {
    head = (GmcDataPtr) MemNew (sizeof (GmcData) * (size_t) (numcds + 1));
    if (head != NULL) {
      gdp = head;
      for (sfp = features; sfp != NULL; sfp = sfp->next) {
        if (sfp->idx.subtype == FEATDEF_CDS) {
          gdp->feat = sfp;
          grp = SeqMgrGetGeneXref (sfp);
          if (grp == NULL || (! SeqMgrGeneIsSuppressed (grp))) {
            gdp->gene = SeqMgrGetOverlappingGene (sfp->location, NULL);
          }
          gdp++;
        }
      }
      HeapSort (head, (size_t) numcds, sizeof (GmcData), SortByGenePtr);
      for (i = 0; i < numcds; i += j) {
        sfp = head [i].gene;
        for (j = 1; i + j < numcds && sfp == head [i + j].gene; j++) continue;
        if (j == 1) {
          /* no alt splicing */
          EnhanceOneCDS (head [i].feat, FALSE);
        } else {
          /* is alt splicing */
          for (k = 0; k < j; k++) {
            EnhanceOneCDS (head [i + k].feat, TRUE);
          }
        }
      }
    }
    MemFree (head);
  }

  if (nummrna > 0) {
    head = (GmcDataPtr) MemNew (sizeof (GmcData) * (size_t) (nummrna + 1));
    if (head != NULL) {
      gdp = head;
      for (sfp = features; sfp != NULL; sfp = sfp->next) {
        if (sfp->idx.subtype == FEATDEF_mRNA) {
          gdp->feat = sfp;
          grp = SeqMgrGetGeneXref (sfp);
          if (grp == NULL || (! SeqMgrGeneIsSuppressed (grp))) {
            gdp->gene = SeqMgrGetOverlappingGene (sfp->location, NULL);
          }
          gdp++;
        }
      }
      HeapSort (head, (size_t) nummrna, sizeof (GmcData), SortByGenePtr);
      for (i = 0; i < nummrna; i += j) {
        sfp = head [i].gene;
        for (j = 1; i + j < nummrna && sfp == head [i + j].gene; j++) continue;
        if (j == 1) {
          /* no alt splicing */
          EnhanceOneMrna (head [i].feat, FALSE);
        } else {
          /* is alt splicing */
          for (k = 0; k < j; k++) {
            EnhanceOneMrna (head [i + k].feat, TRUE);
          }
        }
      }
    }
    MemFree (head);
  }
}

static BioseqPtr AttachSeqAnnotEntity (
  Uint2 entityID,
  SeqAnnotPtr sap,
  TblArgsPtr tbl
)

{
  SeqAnnotPtr  anp;
  BioseqPtr    bsp;
  Int2         genCode;
  SeqEntryPtr  oldscope;
  SeqEntryPtr  sep;
  SeqFeatPtr   sfp = NULL;

  if (sap == NULL || tbl == NULL) return NULL;

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
      genCode = GetGenCodeForBsp (bsp);
      SetEmptyGeneticCodes (sap, genCode);
    }
    if (bsp->annot == NULL) {
      bsp->annot = sap;
    } else {
      anp = bsp->annot;
      while (anp->next != NULL) {
        anp = anp->next;
      }
      anp->next = sap;
    }
    if (sfp != NULL) {
      if (tbl->smartfeats) {

        /* indexing needed to find mRNA and CDS within each gene */

        SeqMgrIndexFeatures (entityID, NULL);

        EnhanceFeatureAnnotation (sfp, bsp);
      }

      PromoteXrefsExEx (sfp, bsp, entityID, TRUE, FALSE, tbl->genprodset, tbl->forcelocalid);
    }
  } else {
    Message (MSG_POSTERR, "Feature table identifiers do not match record");
  }
  return bsp;
}

static CharPtr TrimBracketsFromString (
  CharPtr str,
  SqnTagPtr stp
)

{
  Uchar    ch;	/* to use 8bit characters in multibyte languages */
  Int2     count;
  CharPtr  dst;
  CharPtr  ptr;

  if (StringHasNoText (str) || stp == NULL) return str;

  /* remove bracketed fields */

  count = 0;
  dst = str;
  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    if (ch == '[') {
      if (count < stp->num_tags && (! stp->used [count])) {
        *dst = ch;
        dst++;
        ptr++;
        ch = *ptr;
        while (ch != '\0' && ch != ']') {
          *dst = ch;
          dst++;
          ptr++;
          ch = *ptr;
        }
        *dst = ch;
        dst++;
        ptr++;
        ch = *ptr;
      } else {
        ptr++;
        ch = *ptr;
        while (ch != '\0' && ch != ']' && ch != '"') {
          ptr++;
          ch = *ptr;
        }
        if (ch == '"') {
          ptr++;
          ch = *ptr;
          while (ch != '\0' && ch != '"') {
            ptr++;
            ch = *ptr;
          }
        }
        while (ch != '\0' && ch != ']') {
          ptr++;
          ch = *ptr;
        }
        ptr++;
        ch = *ptr;
      }
      count++;
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

static Boolean HasTpaAccession (
  UserObjectPtr uop
)

{
  UserFieldPtr  curr;
  ObjectIdPtr   oip;
  CharPtr       str;
  UserFieldPtr  ufp;

  if (uop == NULL) return FALSE;
  if ((oip = uop->type) == NULL) return FALSE;
  if (StringCmp (oip->str, "TpaAssembly") != 0) return FALSE;

  for (curr = uop->data; curr != NULL; curr = curr->next) {
    if (curr->choice != 11) continue;
    for (ufp = curr->data.ptrvalue; ufp != NULL; ufp = ufp->next) {
      if (ufp->choice != 1) continue;
      oip = ufp->label;
      if (oip == NULL || StringICmp (oip->str, "accession") != 0) continue;
      str = (CharPtr) ufp->data.ptrvalue;
      if (! StringHasNoText (str)) return TRUE;
    }
  }

  return FALSE;
}

static void ProcessOneNuc (
  Uint2 entityID,
  BioseqPtr bsp,
  BioSourcePtr src,
  TblArgsPtr tbl
)

{
  BioSourcePtr   biop = NULL;
  SeqFeatPtr     cds;
  GBBlockPtr     gbp;
  Int2           genCode;
  size_t         len;
  MolInfoPtr     mip;
  Boolean        mito;
  OrgNamePtr     onp;
  OrgRefPtr      orp;
  SeqHistPtr     shp;
  SqnTagPtr      stp = NULL;
  CharPtr        str;
  CharPtr        ttl = NULL;
  UserObjectPtr  uop;
  ValNodePtr     vnp;

  if (bsp == NULL) return;

  genCode = GetGenCodeForBsp (bsp);

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
  }

  if (ttl != NULL || tbl->srcquals != NULL) {
    len = StringLen (ttl) + StringLen (tbl->srcquals) + 5;
    str = (CharPtr) MemNew (len * sizeof (Char));
    if (str != NULL) {
      StringCpy (str, ttl);
      if (ttl != NULL && tbl->srcquals != NULL) {
        StringCat (str, "; ");
      }
      StringCat (str, tbl->srcquals);
      stp = SqnTagParse (str);
    }
    MemFree (str);
  }

  if (stp != NULL) {
    biop = ParseTitleIntoBioSource (stp, tbl->organism, src);
    ParseTitleIntoBioseq (stp, bsp);
  }
  if (biop == NULL) {
    biop = ParseTitleIntoBioSource (NULL, tbl->organism, src);
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
      switch (mip->biomol) {
        case MOLECULE_TYPE_PRE_MRNA :
        case MOLECULE_TYPE_MRNA :
        case MOLECULE_TYPE_RRNA :
        case MOLECULE_TYPE_TRNA :
        case MOLECULE_TYPE_SNRNA :
        case MOLECULE_TYPE_SCRNA :
        case MOLECULE_TYPE_CRNA :
        case MOLECULE_TYPE_SNORNA :
        case MOLECULE_TYPE_TRANSCRIBED_RNA :
          if (bsp->mol == Seq_mol_dna) {
            str = SqnTagFind (stp, "molecule");
            if (str == NULL) {
              str = SqnTagFind (stp, "mol");
            }
            if (str != NULL) {
              if (StringICmp (str, "dna") == 0) break;
            }
            bsp->mol = Seq_mol_rna;
          }
          break;
        default :
          break;
      }
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

  if (! StringHasNoText (tbl->comment)) {
    str = StringSave (tbl->comment);
    SeqDescrAddPointer (&(bsp->descr), Seq_descr_comment, (Pointer) str);
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

  if (stp != NULL) {
    uop = ParseTitleIntoTpaAssembly (stp, NULL);
    if (uop != NULL && HasTpaAccession (uop)) {
      SeqDescrAddPointer (&(bsp->descr), Seq_descr_user, (Pointer) uop);
    } else {
      uop = UserObjectFree (uop);
    }
  }

  if (tbl->findorf) {
    cds = AnnotateBestOrf (bsp, genCode, tbl->altstart, stp);
    if (cds != NULL) {
      PromoteXrefsExEx (cds, bsp, entityID, TRUE, FALSE, FALSE, tbl->forcelocalid);
    }
  }

  TrimBracketsFromString (ttl, stp);
  if (! StringHasNoText (ttl)) {
    str = StringSave (ttl);
    SeqDescrAddPointer (&(bsp->descr), Seq_descr_title, (Pointer) str);
  }

  if (stp != NULL) {
    SqnTagFree (stp);
  }

  ValNodeFreeData (vnp);
}

static void ProcessOneAnnot (
  SeqAnnotPtr sap,
  Uint2 entityID,
  TblArgsPtr tbl
)

{
  BioseqPtr   bsp;
  Int2        genCode;
  ValNodePtr  generalIDs;
  SeqFeatPtr  sfp;
  SeqIdPtr    sip;

  if (sap == NULL || tbl == NULL) return;

  bsp = AttachSeqAnnotEntity (entityID, sap, tbl);
  if (bsp == NULL) return;

  /* correct all idx parent pointers */

  AssignIDsInEntity (entityID, 0, NULL);

  genCode = GetGenCodeForBsp (bsp);

  /* if existing accession, coerce all SeqIds */

  if (! StringHasNoText (tbl->accn)) {
    sip = SeqIdFromAccession (tbl->accn, 0, NULL);
    if (sip != NULL) {
      generalIDs = ValNodeExtractList (&(bsp->id), SEQID_GENERAL);
      bsp->id = SeqIdSetFree (bsp->id);
      bsp->id = sip;
      if (generalIDs != NULL) {
        ValNodeLink (&(bsp->id), generalIDs);
      } 
      SeqMgrReplaceInBioseqIndex (bsp);
      VisitFeaturesOnBsp (bsp, (Pointer) bsp->id, CorrectFeatureSeqIds);
    }
  }

  /* for parsed in features or best ORF, promote CDS products to protein bioseq */

  for (sap = bsp->annot; sap != NULL; sap = sap->next) {
    if (sap->type == 1) {
      SetEmptyGeneticCodes (sap, genCode);
      sfp = (SeqFeatPtr) sap->data;
      PromoteXrefsExEx (sfp, bsp, entityID, TRUE, FALSE, tbl->genprodset, tbl->forcelocalid);
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

static void ReplaceOneRNA (
  SimpleSeqPtr ssp,
  Boolean conflict
)

{
  ByteStorePtr  bs;
  BioseqPtr     bsp;
  SeqIdPtr      sip;
  CharPtr       str1, str2;

  if (ssp == NULL || ssp->numid < 1) return;

  sip = MakeSeqID (ssp->id [0]);
  bsp = BioseqFind (sip);
  SeqIdFree (sip);
  if (bsp == NULL || bsp->repr != Seq_repr_raw) return;

  /* remove trailing X and * */

  bs = ssp->seq;
  ssp->seqlen = BSLen (bs);

  str1 = BSMerge (ssp->seq, NULL);
  str2 = GetSequenceByBsp (bsp);

  if (StringCmp (str1, str2) != 0) {

    /* swap sequence byte stores */

    bs = bsp->seq_data;
    bsp->seq_data = ssp->seq;
    ssp->seq = bs;
    bsp->length = BSLen (bsp->seq_data);
    bsp->seq_data_type = Seq_code_iupacna;

    /*
    mrna = SeqMgrGetRNAgivenProduct (bsp, NULL);
    if (mrna != NULL) {

      if (conflict) {
        mrna->excpt = TRUE;
        if (StringHasNoText (mrna->except_text)) {
          mrna->except_text = StringSave ("RNA editing");
        }
      }
    }
    */
  }

  MemFree (str1);
  MemFree (str2);
}

static Uint2 ProcessOneAsn (
  FILE* fp,
  BioSourcePtr src,
  TblArgsPtr tbl,
  CharPtr localname,
  SeqEntryPtr gsep
)

{
  BioseqPtr      bsp = NULL;
  BioseqSetPtr   bssp;
  Pointer        dataptr;
  Uint2          datatype, entityID;
  ObjMgrDataPtr  omdptop;
  ObjMgrData     omdata;
  Uint2          parenttype;
  Pointer        parentptr;
  SeqEntryPtr    sep;
  SeqIdPtr       sip;

  if (fp == NULL) return 0;

  if (gsep != NULL) {
    bssp = (BioseqSetPtr) gsep->data.ptrvalue;
    if (bssp == NULL) return 0;

    SaveSeqEntryObjMgrData (gsep, &omdptop, &omdata);
    GetSeqEntryParent (gsep, &parentptr, &parenttype);

    dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, TRUE, FALSE, TRUE, FALSE);
    if (datatype == OBJ_BIOSEQ) {
      bssp->seq_set = SeqMgrGetSeqEntryForData (dataptr);
      SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) dataptr, gsep);
    } else if (datatype == OBJ_BIOSEQSET) {
      bssp->seq_set = SeqMgrGetSeqEntryForData (dataptr);
      SeqMgrSeqEntry (SM_BIOSEQSET, (Pointer) dataptr, gsep);
    } else return 0;

    SeqMgrLinkSeqEntry (gsep, parenttype, parentptr);
    RestoreSeqEntryObjMgrData (gsep, omdptop, &omdata);

    entityID = ObjMgrRegister (OBJ_BIOSEQSET, (Pointer) bssp);
  } else {
    dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, &entityID, TRUE, FALSE, TRUE, FALSE);
  }
  if (dataptr == NULL) return 0;

  sep = GetTopSeqEntryForEntityID (entityID);
  bsp = FindNucBioseq (sep);
  if (bsp == NULL) {
    ObjMgrFreeByEntityID (entityID);
    return 0;
  }

  if (! StringHasNoText (localname)) {
    sip = MakeSeqID (localname);
    if (sip != NULL) {
      bsp->id = SeqIdSetFree (bsp->id);
      bsp->id = sip;
      SeqMgrReplaceInBioseqIndex (bsp);
      VisitFeaturesOnBsp (bsp, (Pointer) bsp->id, CorrectFeatureSeqIds);
    }
  }

  ProcessOneNuc (entityID, bsp, src, tbl);

  return entityID;
}

static Uint2 ProcessBulkSet (
  FILE* fp,
  BioSourcePtr src,
  TblArgsPtr tbl
)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Uint2         entityID;
  SeqEntryPtr   lastsep, sep, topsep;
  /*
  Pointer       dataptr;
  Uint2         datatype;
  */

  if (fp == NULL || tbl == NULL) return 0;

  bssp = BioseqSetNew ();
  if (bssp == NULL) return 0;

  switch (tbl->whichclass) {
    case 1 :
      bssp->_class = BioseqseqSet_class_pop_set;
      break;
    case 2 :
      bssp->_class = BioseqseqSet_class_phy_set;
      break;
    case 3 :
      bssp->_class = BioseqseqSet_class_mut_set;
      break;
    case 4 :
      bssp->_class = BioseqseqSet_class_eco_set;
      break;
    default :
      bssp->_class = BioseqseqSet_class_genbank;
      break;
  }

  topsep = SeqEntryNew ();
  if (topsep == NULL) return 0;
  topsep->choice = 2;
  topsep->data.ptrvalue = (Pointer) bssp;

  entityID = ObjMgrRegister (OBJ_BIOSEQSET, (Pointer) bssp);

  lastsep = NULL;

/*
  while ((dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, TRUE, FALSE, TRUE, FALSE)) != NULL) {
    if (datatype == OBJ_BIOSEQ) {

      sep = SeqMgrGetSeqEntryForData (dataptr);
      if (lastsep == NULL) {
        bssp->seq_set = sep;
      } else {
        lastsep->next = sep;
      }
      lastsep = sep;

      bsp = (BioseqPtr) dataptr;
      ProcessOneNuc (entityID, bsp, src, tbl);

    } else {
      ObjMgrFree (datatype, dataptr);
    }
  }
*/

while ((bsp = ReadDeltaFasta (fp, NULL)) != NULL) {

    sep = SeqMgrGetSeqEntryForData (bsp);
    if (lastsep == NULL) {
      bssp->seq_set = sep;
    } else {
      lastsep->next = sep;
    }
    lastsep = sep;

    ProcessOneNuc (entityID, bsp, src, tbl);
  }

  SeqMgrLinkSeqEntry (topsep, 0, NULL);

  return entityID;
}

static SeqEntryPtr FA2SEP (
  FILE *fp
)

{
  BioseqPtr    bsp;
  Pointer      dataptr;
  Uint2        datatype;
  SeqEntryPtr  sep;

  if (fp == NULL) return NULL;

  dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, TRUE, FALSE, TRUE, FALSE);
  if (datatype == OBJ_BIOSEQ) {
    sep = SeqMgrGetSeqEntryForData (dataptr);
    if (sep == NULL) {
      sep = SeqEntryNew ();
      if (sep != NULL) {
        bsp = (BioseqPtr) dataptr;
        sep->choice = 1;
        sep->data.ptrvalue = bsp;
        SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, sep);
      }
    }
    return sep;
  }

  return NULL;
}

static Uint2 ProcessDeltaSet (
  FILE* fp,
  BioSourcePtr src,
  TblArgsPtr tbl,
  CharPtr localname,
  SeqEntryPtr gsep
)

{
  BioseqPtr      bsp, deltabsp;
  BioseqSetPtr   bssp;
  Uint2          entityID;
  SeqEntryPtr    firstsep, lastsep, nextsep, sep, topsep;
  IntFuzzPtr     ifp;
  Boolean        is_unk100;
  ObjectIdPtr    oip;
  ObjMgrDataPtr  omdptop;
  ObjMgrData     omdata;
  Uint2          parenttype;
  Pointer        parentptr;
  CharPtr        seqbuf;
  SeqIdPtr       sip, virtid;
  SeqLitPtr      slp;
  ValNodePtr     vnp;

  if (fp == NULL) return 0;

  firstsep = NULL;
  lastsep = NULL;

  /*
  sep = FastaToSeqEntry (fp, TRUE);
  */
  sep = FA2SEP (fp);
  if (sep == NULL) return 0;

  /* loop to collect subsequent entries */

  while (sep != NULL) {
    if (firstsep == NULL) {
      firstsep = sep;
    }
    if (lastsep != NULL) {
      ValNodeLink (&lastsep, sep);
    }
    lastsep = sep;
    /*
    sep = FastaToSeqEntry (fp, TRUE);
    */
    sep = FA2SEP (fp);
  }

  /* if only one FASTA, treat as raw */

  if (firstsep->next == NULL) {
    bsp = FindNucBioseq (firstsep);
    if (bsp == NULL) return 0;

    if (gsep != NULL) {
      bssp = (BioseqSetPtr) gsep->data.ptrvalue;
      if (bssp == NULL) return 0;

      SaveSeqEntryObjMgrData (gsep, &omdptop, &omdata);
      GetSeqEntryParent (gsep, &parentptr, &parenttype);

      bssp->seq_set = SeqMgrGetSeqEntryForData (bsp);
      SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, gsep);

      SeqMgrLinkSeqEntry (gsep, parenttype, parentptr);
      RestoreSeqEntryObjMgrData (gsep, omdptop, &omdata);

      entityID = ObjMgrRegister (OBJ_BIOSEQSET, (Pointer) bssp);
    } else {
      entityID = ObjMgrRegister (OBJ_BIOSEQ, (Pointer) bsp);
    }

    ProcessOneNuc (entityID, bsp, src, tbl);
    return entityID;
  }

  /* now process delta */

  bsp = FindNucBioseq (firstsep);
  if (bsp == NULL) return 0;

  sip = SeqIdSetDup (bsp->id);
  vnp = ValNodeExtract (&(bsp->descr), Seq_descr_title);

  deltabsp = BioseqNew ();
  if (deltabsp == NULL) return 0;
  deltabsp->repr = Seq_repr_delta;
  deltabsp->seq_ext_type = 4;
  deltabsp->mol = Seq_mol_dna;
  deltabsp->length = 0;

  topsep = SeqEntryNew ();
  if (topsep == NULL) return 0;
  topsep->choice = 1;
  topsep->data.ptrvalue = (Pointer) deltabsp;

  if (gsep != NULL) {
    bssp = (BioseqSetPtr) gsep->data.ptrvalue;
    if (bssp == NULL) return 0;

    SaveSeqEntryObjMgrData (gsep, &omdptop, &omdata);
    GetSeqEntryParent (gsep, &parentptr, &parenttype);

    bssp->seq_set = topsep;
    SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) deltabsp, gsep);

    SeqMgrLinkSeqEntry (gsep, parenttype, parentptr);
    RestoreSeqEntryObjMgrData (gsep, omdptop, &omdata);

    entityID = ObjMgrRegister (OBJ_BIOSEQSET, (Pointer) bssp);
  } else {
    entityID = ObjMgrRegister (OBJ_BIOSEQ, (Pointer) deltabsp);
  }

  for (sep = firstsep; sep != NULL; sep = nextsep) {
    nextsep = sep->next;
    sep->next = NULL;

    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp == NULL) continue;

    if (bsp->repr == Seq_repr_raw) {
      BioseqRawConvert (bsp, Seq_code_iupacna);
      seqbuf = BSMerge ((ByteStorePtr) bsp->seq_data, NULL);
      slp = (SeqLitPtr) MemNew (sizeof (SeqLit));
      if (slp == NULL) continue;

      slp->length = bsp->length;
      ValNodeAddPointer ((ValNodePtr PNTR) &(deltabsp->seq_ext), (Int2) 2, (Pointer) slp);
      slp->seq_data = BSNew (slp->length);
      slp->seq_data_type = Seq_code_iupacna;
      AddBasesToByteStore (slp->seq_data, seqbuf);
      MemFree(seqbuf);

      deltabsp->length += slp->length;

    } else if (bsp->repr == Seq_repr_virtual) {
      slp = (SeqLitPtr) MemNew (sizeof (SeqLit));
      if (slp == NULL) continue;
      slp->length = bsp->length;
      if (slp == NULL) continue;

      slp->length = bsp->length;
      ValNodeAddPointer ((ValNodePtr PNTR) &(deltabsp->seq_ext), (Int2) 2, (Pointer) slp);

      is_unk100 = FALSE;
      virtid = bsp->id;
      if (virtid != NULL && virtid->choice == SEQID_LOCAL) {
        oip = (ObjectIdPtr) virtid->data.ptrvalue;
        if (oip != NULL) {
          if (StringCmp (oip->str, "unk100") == 0) {
            is_unk100 = TRUE;
          }
        }
      }
      if (slp->length < 1 || is_unk100) {
        if (slp->length < 1) {
          slp->length = 0;
        }
        ifp = IntFuzzNew ();
        ifp->choice = 4;
        slp->fuzz = ifp;
      }

      deltabsp->length += slp->length;
    }

    SeqEntryFree (sep);
  }

  ValNodeLink (&(deltabsp->descr), vnp);
  deltabsp->id = sip;

  if (gsep == NULL) {
    SeqMgrLinkSeqEntry (topsep, 0, NULL);
  }

  if (! StringHasNoText (localname)) {
    sip = MakeSeqID (localname);
    if (sip != NULL) {
      bsp->id = SeqIdSetFree (bsp->id);
      bsp->id = sip;
      SeqMgrReplaceInBioseqIndex (bsp);
      VisitFeaturesOnBsp (bsp, (Pointer) bsp->id, CorrectFeatureSeqIds);
    }
  }

  ProcessOneNuc (entityID, deltabsp, src, tbl);

  return entityID;
}

static Boolean DoSequenceLengthsMatch (
  TAlignmentFilePtr afp
)
{
  int    seq_index;
  Int4   seq_len;

  if (afp == NULL || afp->sequences == NULL || afp->num_sequences == 0) {
    return TRUE;
  }
  seq_len = StringLen (afp->sequences[0]);
  for (seq_index = 1; seq_index < afp->num_sequences; seq_index++) {
    if (StringLen (afp->sequences[seq_index]) != seq_len) {
      return FALSE;
    }
  }
  return TRUE;
}

static void ShowAlignmentNotes 
(TAlignmentFilePtr afp,
 TErrorInfoPtr error_list)
{
  TErrorInfoPtr eip;
  Int4         index;

  for (eip = error_list; eip != NULL; eip = eip->next) {
    printf ("*****\nError category %d\n", eip->category);
    if (eip->line_num > -1) {
	  printf ("Line number %d\n", eip->line_num);
    }
    if (eip->id != NULL) {
      printf ("Sequence ID %s\n", eip->id);
    }
    if (eip->message != NULL) {
      printf ("%s\n", eip->message);
    }
  }
  if (afp == NULL) {
    printf ("Catastrophic failure during reading\n");
  } else {
    printf ("Found %d sequences\n", afp->num_sequences);
    printf ("Found %d organisms\n", afp->num_organisms);
    for (index = 0; index < afp->num_sequences; index++)
    {
      printf ("\t%s\t", afp->ids [index]);
      if (index < afp->num_organisms) {
        printf ("%s\n", afp->organisms [index]);
      } else {
        printf ("No organism information\n");
      }
    }
    while (index < afp->num_organisms) {
      printf ("Unclaimed organism: %s\n", afp->organisms [index]);
      index++;
    }
  }
}

static Uint2 ProcessAlignSet (
  FILE *fp,
  BioSourcePtr src,
  TblArgsPtr tbl
)

{
  TSequenceInfoPtr  sequence_info;
  TErrorInfoPtr     error_list;
  ReadBufferData    rbd;
  TAlignmentFilePtr afp;
  SeqEntryPtr       sep = NULL;
  BioseqPtr         bsp;
  BioseqSetPtr      bssp;
  Char              ch;
  Uint2             entityID;
  SeqEntryPtr       tmp;
  Char              nucleotide_alphabet[] = "ABCDGHKMRSTUVWXYabcdghkmrstuvwxy";
  Char              protein_alphabet[] = "ABCDEFGHIKLMPQRSTUVWXYZabcdefghiklmpqrstuvwxyz";
  Uint1             moltype = Seq_mol_dna;

  if (fp == NULL) return 0;

  sequence_info = SequenceInfoNew ();
  if (sequence_info == NULL) return 0;

  /* format sequence options based on commandline arguments */
  /* set sequence alphabet */
  if (tbl->aln_is_protein) {
    moltype = Seq_mol_aa;
    sequence_info->alphabet = StringSave (protein_alphabet);
  } else {
    moltype = Seq_mol_dna;
    sequence_info->alphabet = StringSave (nucleotide_alphabet);
  }
  
  sequence_info->beginning_gap = MemFree (sequence_info->beginning_gap);
  if (StringHasNoText (tbl->aln_beginning_gap)) {
    sequence_info->beginning_gap = StringSave (".-?");
  } else {
    sequence_info->beginning_gap = StringSave (tbl->aln_beginning_gap);
  }
  sequence_info->middle_gap = MemFree (sequence_info->middle_gap);
  if (StringHasNoText (tbl->aln_middle_gap)) {
    sequence_info->middle_gap = StringSave ("-");
  } else {
    sequence_info->middle_gap = StringSave (tbl->aln_middle_gap);
  }
  sequence_info->end_gap = MemFree (sequence_info->end_gap);
  if (StringHasNoText (tbl->aln_end_gap)) {
    sequence_info->end_gap = StringSave (".-?");
  } else {
    sequence_info->end_gap = StringSave (tbl->aln_end_gap);
  }
  sequence_info->missing = MemFree (sequence_info->missing);
  if (StringHasNoText (tbl->aln_missing)) {
    sequence_info->missing = StringSave ("Nn?");
  } else {
    sequence_info->missing = StringSave (tbl->aln_missing);
  }
  sequence_info->match = MemFree (sequence_info->match);
  if (StringHasNoText (tbl->aln_match)) {
    sequence_info->match = StringSave (".");
  } else {
    sequence_info->match = StringSave (tbl->aln_match);
  }

  error_list = NULL;
  rbd.fp = fp;
  rbd.current_data = NULL;
  afp = ReadAlignmentFile ( AbstractReadFunction,
                            (Pointer) &rbd,
                            AbstractReportError,
                            (Pointer) &error_list,
                            sequence_info);

  ShowAlignmentNotes (afp, error_list);
  ErrorInfoFree (error_list);
  if (afp != NULL) {
    if (afp->num_organisms == 0 && src == NULL) {
	  printf ("No organisms supplied!\n");
    } else if (afp->num_organisms != 0 && afp->num_organisms != afp->num_sequences) {
      printf ( "Number of organisms must match number of sequences!");
    } else {
	  ch = 'y';
      if (! DoSequenceLengthsMatch (afp)) {
        printf ("Sequences are not all the same length - are you sure you want to continue?");
		ch = getchar ();
      }
      if (ch == 'y' || ch == 'Y') {
        sep = MakeSequinDataFromAlignment (afp, moltype);
      }
    }
  }
  SequenceInfoFree (sequence_info);

  AlignmentFileFree (afp);

  if (sep == NULL || sep->data.ptrvalue == NULL) return 0;

  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
  	entityID = ObjMgrRegister (OBJ_BIOSEQ, (Pointer) bsp);
  	ProcessOneNuc (entityID, bsp, src, tbl);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    bssp->_class = BioseqseqSet_class_phy_set;
  	entityID = ObjMgrRegister (OBJ_BIOSEQSET, (Pointer) bssp);
  	for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
  	  if (IS_Bioseq (tmp)) {
  	  	bsp = (BioseqPtr) tmp->data.ptrvalue;
        ProcessOneNuc (entityID, bsp, src, tbl);
  	  }
  	}
  } else return 0;

  SeqMgrLinkSeqEntry (sep, 0, NULL);

  return entityID;
}

static SeqAnnotPtr NewGraphSeqAnnot (
  CharPtr name,
  SeqGraphPtr sgp
)

{
  SeqAnnotPtr  sap = NULL;

  if (sgp == NULL) return NULL;
  sap = SeqAnnotNew ();
  if (sap == NULL) return NULL;

  if (! StringHasNoText (name)) {
    ValNodeAddPointer (&(sap->desc), Annot_descr_name, StringSave (name));
  }
  sap->type = 3;
  sap->data = (Pointer) sgp;

  return sap;
}

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

typedef struct npsseqs {
  BioseqPtr  nuc;
  BioseqPtr  prot;
} NpsSeqs, PNTR NpsSeqsPtr;

static void FindNucProtSeqs (
  BioseqPtr bsp,
  Pointer userdata
)

{
  NpsSeqsPtr  nsp;

  if (bsp == NULL) return;
  nsp = (NpsSeqsPtr) userdata;
  if (nsp == NULL) return;

  if (ISA_na (bsp->mol)) {
    nsp->nuc = bsp;
  } else if (ISA_aa (bsp->mol)) {
    nsp->prot = bsp;
  }
}

static Boolean InRightNps (
  CharPtr gbqval,
  SeqIdPtr protids,
  Boolean force_local_id
)

{
  Char      id [64];
  Char      lcl [64];
  SeqIdPtr  sip;
  CharPtr   ptr;
  Boolean   rsult;
  long int  val;
  Uint4     version = 0;

  StringNCpy_0 (id, gbqval, sizeof (id));
  if (! StringHasNoText (id)) {
    if (StringChr (id, '|') != NULL) {
      sip = SeqIdParse (id);
    } else if (force_local_id) {
      sprintf (lcl, "lcl|%s", id);
      sip = SeqIdParse (lcl);
    } else {
      ptr = StringChr (id, '.');
      if (ptr != NULL) {
        *ptr = '\0';
        ptr++;
        if (sscanf (ptr, "%ld", &val) == 1) {
          version = (Uint4) val;
        }
      }
      sip = SeqIdFromAccession (id, version, NULL);
    }
  }
  if (sip == NULL) return FALSE;
  rsult = SeqIdIn (sip, protids);
  SeqIdFree (sip);
  return rsult;
}

static void MakeNucProtCDS (
  BioseqSetPtr bssp,
  Pointer userdata
)

{
  CodeBreakPtr    cbp;
  SeqFeatPtr      cds;
  CdRegionPtr     crp;
  GBQualPtr       gbq;
  Char            id [64];
  SeqFeatPtr      mrna;
  GBQualPtr       nextqual;
  NpsSeqs         ns;
  Boolean         partial5, partial3;
  GBQualPtr PNTR  prevqual;
  SeqFeatPtr      sfp;
  SeqIdPtr        sip;
  SeqLocPtr       slp;
  Int4            start, stop;
  TblArgsPtr      tbl;
  SeqFeatPtr      temp;

  tbl = (TblArgsPtr) userdata;
  if (tbl == NULL) return;

  ns.nuc = NULL;
  ns.prot = NULL;
  if (VisitBioseqsInSet (bssp, (Pointer) &ns, FindNucProtSeqs) != 2) return;
  if (ns.nuc == NULL || ns.prot == NULL) return;

  cds = SeqMgrGetCDSgivenProduct (ns.prot, NULL);
  mrna = SeqMgrGetRNAgivenProduct (ns.nuc, NULL);
  if (cds == NULL || mrna == NULL) return;

  CheckSeqLocForPartial (cds->location, &partial5, &partial3);

  start = GetOffsetInLoc (cds->location, mrna->location, SEQLOC_START);
  stop = GetOffsetInLoc (cds->location, mrna->location, SEQLOC_STOP);

  if (start < 0 || start >= ns.nuc->length ||
      stop < 0 || stop >= ns.nuc->length) return;

  sip = SeqIdFindBest (ns.nuc->id, 0);
  if (sip == NULL) return;

  /* copy cds feature fields to paste into new cds feature */
  temp = AsnIoMemCopy (cds,
                       (AsnReadFunc) SeqFeatAsnRead,
                       (AsnWriteFunc) SeqFeatAsnWrite);
  if (temp == NULL) return;

  sfp = CreateNewFeatureOnBioseq (ns.nuc, SEQFEAT_CDREGION, NULL);
  if (sfp == NULL) return;

  sfp->location = SeqLocFree (sfp->location);
  if (StringISearch (cds->except_text, "ribosomal slippage") == NULL &&
      StringISearch (cds->except_text, "ribosome slippage") == NULL &&
      StringISearch (cds->except_text, "trans splicing") == NULL &&
      StringISearch (cds->except_text, "trans-splicing") == NULL &&
      StringISearch (cds->except_text, "artificial frameshift") == NULL) {
    sfp->location = AddIntervalToLocation (NULL, sip, start, stop, partial5, partial3);
  } else {
    slp = SeqLocFindNext (cds->location, NULL);
    while (slp != NULL) {
      start = GetOffsetInLoc (slp, mrna->location, SEQLOC_START);
      stop = GetOffsetInLoc (slp, mrna->location, SEQLOC_STOP);
      sfp->location = AddIntervalToLocation (sfp->location, sip, start, stop, partial5, partial3);
      slp = SeqLocFindNext (cds->location, slp);
    }
    sfp->location = SeqLocMergeEx (ns.nuc, sfp->location, NULL, FALSE, TRUE, FALSE, FALSE);
  }
  SetSeqFeatProduct (sfp, ns.prot);

  /* paste fields from temp copy of original cds */
  crp = (CdRegionPtr) temp->data.value.ptrvalue;
  sfp->data.value.ptrvalue = (Pointer) crp;

  sfp->partial = temp->partial;
  sfp->excpt = temp->excpt;
  sfp->comment = temp->comment;
  sfp->qual = temp->qual;
  sfp->title = temp->title;
  sfp->ext = temp->ext;
  sfp->cit = temp->cit;
  sfp->exp_ev = temp->exp_ev;
  sfp->xref = temp->xref;
  sfp->dbxref = temp->dbxref;
  sfp->pseudo = temp->pseudo;
  sfp->except_text = temp->except_text;

  MemFree (temp); /* do not SeqFeatFree */

  /* update code break locations */
  for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) {
    CheckSeqLocForPartial (cbp->loc, &partial5, &partial3);
    start = GetOffsetInLoc (cbp->loc, mrna->location, SEQLOC_START);
    stop = GetOffsetInLoc (cbp->loc, mrna->location, SEQLOC_STOP);
    if (start < 0 || start >= ns.nuc->length ||
        stop < 0 || stop >= ns.nuc->length) continue;
	cbp->loc = SeqLocFree (cbp->loc);
	cbp->loc = AddIntervalToLocation (NULL, sip, start, stop, partial5, partial3);;
  }

  /* get rid of protein_id in mRNA if it matches protein Seq-id */
  gbq = mrna->qual;
  prevqual = (GBQualPtr PNTR) &(mrna->qual);
  id [0] = '\0';
  sip = NULL;
  while (gbq != NULL) {
    nextqual = gbq->next;
    if (StringICmp (gbq->qual, "protein_id") == 0 &&
        InRightNps (gbq->val, ns.prot->id, tbl->forcelocalid)) {
      *(prevqual) = gbq->next;
      gbq->next = NULL;
      StringNCpy_0 (id, gbq->val, sizeof (id));
      GBQualFree (gbq);
    } else {
      prevqual = (GBQualPtr PNTR) &(gbq->next);
    }
    gbq = nextqual;
  }
}

/* copy gene from contig onto nuc-prot, single interval on cdna bioseq */

static void CopyGene (
  SeqFeatPtr sfp,
  Pointer userdata
)

{
  BioseqPtr          bsp;
  SeqMgrFeatContext  gcontext;
  SeqFeatPtr         gene, copy, temp;
  GeneRefPtr         grp, xref;
  Boolean            partial5, partial3;

  /* input mrna features are multi-interval on contig */

  if (sfp->data.choice != SEQFEAT_RNA) return;

  /* find cdna product of mrna */

  bsp = BioseqFindFromSeqLoc (sfp->product);
  if (bsp == NULL) return;

  /* check for gene xref */

  xref = SeqMgrGetGeneXref (sfp);
  if (xref != NULL) {
    if (SeqMgrGeneIsSuppressed (xref)) return;

    /* copy gene xref for new gene feature */

    grp = AsnIoMemCopy (xref,
                        (AsnReadFunc) GeneRefAsnRead,
                        (AsnWriteFunc) GeneRefAsnWrite);
    if (grp == NULL) return;

    /* make new gene feature on full-length of cdna */

    copy = CreateNewFeatureOnBioseq (bsp, SEQFEAT_GENE, NULL);
    if (copy == NULL) return;

    copy->data.value.ptrvalue = grp;
    return;
  }

  /* overlapping gene should be single interval on contig */

  gene = SeqMgrGetOverlappingGene (sfp->location, &gcontext);
  if (gene == NULL) return;

  CheckSeqLocForPartial (gene->location, &partial5, &partial3);

  /* copy gene feature fields to paste into new gene feature */

  temp = AsnIoMemCopy (gene,
                       (AsnReadFunc) SeqFeatAsnRead,
                       (AsnWriteFunc) SeqFeatAsnWrite);
  if (temp == NULL) return;

  /* make new gene feature on full-length of cdna */

  copy = CreateNewFeatureOnBioseq (bsp, SEQFEAT_GENE, NULL);
  if (copy == NULL) {
    SeqFeatFree (temp);
    return;
  }

  /* paste fields from temp copy of original gene */

  copy->data.value.ptrvalue = temp->data.value.ptrvalue;
  copy->partial = temp->partial;
  copy->excpt = temp->excpt;
  copy->comment = temp->comment;
  copy->qual = temp->qual;
  copy->title = temp->title;
  copy->ext = temp->ext;
  copy->cit = temp->cit;
  copy->exp_ev = temp->exp_ev;
  copy->xref = temp->xref;
  copy->dbxref = temp->dbxref;
  copy->pseudo = temp->pseudo;
  copy->except_text = temp->except_text;

  SetSeqLocPartial (copy->location, partial5, partial3);

  SeqLocFree (temp->location);
  MemFree (temp); /* do not SeqFeatFree */
}

static void ClearRnaProducts (
  SeqFeatPtr sfp,
  Pointer userdata
)

{
  if (sfp == NULL || sfp->data.choice != SEQFEAT_RNA) return;
  if (sfp->product == NULL) return;

  sfp->product = SeqLocFree (sfp->product);
}

static void RemoveGBQualIDs (
  SeqFeatPtr sfp,
  Pointer userdata
)

{
  GBQualPtr       gbq;
  GBQualPtr       nextqual;
  GBQualPtr PNTR  prevqual;

  if (sfp->data.choice != SEQFEAT_CDREGION && sfp->data.choice != SEQFEAT_RNA) return;

  gbq = sfp->qual;
  prevqual = (GBQualPtr PNTR) &(sfp->qual);
  while (gbq != NULL) {
    nextqual = gbq->next;
    if (StringICmp (gbq->qual, "transcript_id") == 0 ||
        StringICmp (gbq->qual, "protein_id") == 0) {
      *(prevqual) = gbq->next;
      gbq->next = NULL;
      GBQualFree (gbq);
    } else {
      prevqual = (GBQualPtr PNTR) &(gbq->next);
    }
    gbq = nextqual;
  }
}

typedef struct dupprot {
  SeqFeatPtr  firstprot;
  SeqFeatPtr  secondprot;
} DupProt, PNTR DupProtPtr;

static void FindDupProtFeats (
  SeqFeatPtr sfp,
  Pointer userdata
)

{
  DupProtPtr  dpp;
  ProtRefPtr  prp;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_PROT) return;
  dpp = (DupProtPtr) userdata;
  prp = (ProtRefPtr) sfp->data.value.ptrvalue;
  if (dpp == NULL || prp == NULL) return;
  if (prp->processed != 0) return;
  if (dpp->firstprot == NULL) {
    dpp->firstprot = sfp;
  } else if (dpp->secondprot == NULL) {
    dpp->secondprot = sfp;
  }
}

static void RemoveDupProtFeats (
  BioseqPtr bsp,
  Pointer userdata
)

{
  DupProt  dp;

  if (bsp == NULL) return;
  if (! ISA_aa (bsp->mol)) return;
  dp.firstprot = NULL;
  dp.secondprot = NULL;
  VisitFeaturesOnBsp (bsp, (Pointer) &dp, FindDupProtFeats);
  if (dp.firstprot == NULL && dp.secondprot == NULL) return;
  if (AsnIoMemComp ((Pointer) dp.firstprot, (Pointer) dp.secondprot, (AsnWriteFunc) SeqFeatAsnWrite)) {
    dp.firstprot->idx.deleteme = TRUE;
  }
}

static void RemoveUnnecGeneXref (
  SeqFeatPtr sfp,
  Pointer userdata
)

{
  SeqFeatXrefPtr  curr, next;
  SeqFeatXrefPtr  PNTR last;
  GeneRefPtr      grp, grpx;
  Boolean         redundantgenexref;
  SeqFeatPtr      sfpx;
  CharPtr         syn1, syn2;

  if (sfp == NULL || sfp->data.choice == SEQFEAT_GENE) return;
  grp = SeqMgrGetGeneXref (sfp);
  if (grp == NULL || SeqMgrGeneIsSuppressed (grp)) return;
  sfpx = SeqMgrGetOverlappingGene (sfp->location, NULL);
  if (sfpx == NULL || sfpx->data.choice != SEQFEAT_GENE) return;
  grpx = (GeneRefPtr) sfpx->data.value.ptrvalue;
  if (grpx == NULL) return;

  redundantgenexref = FALSE;
  if (StringDoesHaveText (grp->locus) && StringDoesHaveText (grpx->locus)) {
    if ((StringICmp (grp->locus, grpx->locus) == 0)) {
      redundantgenexref = TRUE;
    }
  } else if (StringDoesHaveText (grp->locus_tag) && StringDoesHaveText (grpx->locus_tag)) {
    if ((StringICmp (grp->locus_tag, grpx->locus_tag) == 0)) {
      redundantgenexref = TRUE;
    }
  } else if (grp->syn != NULL && grpx->syn != NULL) {
    syn1 = (CharPtr) grp->syn->data.ptrvalue;
    syn2 = (CharPtr) grpx->syn->data.ptrvalue;
    if (StringDoesHaveText (syn1) && StringDoesHaveText (syn2)) {
      if (StringICmp (syn1, syn2) == 0) {
        redundantgenexref = TRUE;
      }
    }
  }

  if (redundantgenexref) {
	last = (SeqFeatXrefPtr PNTR) &(sfp->xref);
	curr = sfp->xref;
	while (curr != NULL) {
	  next = curr->next;
	  if (curr->data.choice == SEQFEAT_GENE) {
		*last = next;
		curr->next = NULL;
		SeqFeatXrefFree (curr);
	  } else {
		last = &(curr->next);
	  }
	  curr = next;
	}
  }
}

static void AddMrnaTitles (
  SeqLocPtr slp,
  Pointer userdata
)

{
  BioseqPtr          bsp;
  SeqMgrFeatContext  ccontext;
  CharPtr            cdslabel = NULL;
  SeqMgrFeatContext  gcontext;
  CharPtr            genelabel = NULL;
  size_t             len;
  CharPtr            organism;
  SeqFeatPtr         sfp;
  CharPtr            str;

  if (slp == NULL) return;
  bsp = BioseqFindFromSeqLoc (slp);
  if (bsp == NULL) return;
  if (! ISA_na (bsp->mol)) return;
  organism = (CharPtr) userdata;
  if (BioseqGetTitle (bsp) != NULL) return;
  sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_GENE, 0, &gcontext);
  if (sfp != NULL) {
    genelabel = gcontext.label;
    if (StringHasNoText (genelabel)) {
      genelabel = NULL;
    }
  }
  sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_CDREGION, 0, &ccontext);
  if (sfp != NULL) {
    cdslabel = ccontext.label;
    if (StringHasNoText (cdslabel)) {
      cdslabel = NULL;
    }
  }
  len = StringLen (organism) + StringLen (genelabel) + StringLen (cdslabel) +
        StringLen (" mRNA, complete cds.") + 10;
  str = (CharPtr) MemNew (len * sizeof (Char));
  if (str == NULL) return;
  str [0] = '\0';

  if (! StringHasNoText (organism)) {
    StringCat (str, organism);
  }
  if (cdslabel != NULL) {
    StringCat (str, " ");
    StringCat (str, cdslabel);
  }
  if (genelabel != NULL) {
      StringCat (str, " (");
      StringCat (str, genelabel);
      StringCat (str, ")");
  }
  if (cdslabel != NULL && genelabel != NULL) {
    if (ccontext.partialL || ccontext.partialR) {
      StringCat (str, " mRNA, partial cds.");
    } else {
      StringCat (str, " mRNA, complete cds.");
    }
  } else if (genelabel != NULL) {
    StringCat (str, " mRNA.");
  }
  SeqDescrAddPointer (&(bsp->descr), Seq_descr_title, (Pointer) str);
}

typedef struct gosearch {
  TextFsaPtr  gotags;
  Boolean     isbad;
} GoSearch, PNTR GoSearchPtr;

static void LookForGo (
  SeqFeatPtr sfp,
  Pointer userdata
)

{
  Char         ch;
  GoSearchPtr  gsp;
  CharPtr      ptr;
  Int2         state;
  ValNodePtr   matches;

  if (sfp == NULL || StringHasNoText (sfp->comment)) return;
  gsp = (GoSearchPtr) userdata;

  state = 0;
  ptr = sfp->comment;
  ch = *ptr;
  while (ch != '\0') {
    matches = NULL;
    state = TextFsaNext (gsp->gotags, state, ch, &matches);
    if (matches != NULL) {
      gsp->isbad = TRUE;
    }
    ptr++;
    ch = *ptr;
  }
}

static Boolean HasGoTermsInNote (
  SeqEntryPtr sep,
  TextFsaPtr gotags
)

{
  GoSearch  gs;

  gs.gotags = gotags;
  gs.isbad = FALSE;
  VisitFeaturesInSep (sep, (Pointer) &gs, LookForGo);
  return gs.isbad;
}

static void TakeProteinsFromGPS (
  BioseqPtr bsp,
  Pointer userdata
)

{
  SeqEntryPtr PNTR  lastp;
  SeqEntryPtr       sep;

  if (bsp == NULL || (! ISA_aa (bsp->mol))) return;
  lastp = (SeqEntryPtr PNTR) userdata;
  if (lastp == NULL) return;

  /* unlink from existing chain */

  sep = bsp->seqentry;
  if (sep != NULL) {
    sep->data.ptrvalue = NULL;
  }

  /* link after genomic sequence */

  sep = ValNodeAddPointer (lastp, 1, (Pointer) bsp);
  *lastp = sep;
}

static void GPStoNPS (
  SeqEntryPtr top,
  Uint2 entityID
)

{
  BioseqSetPtr  bssp;
  BioseqSet     dum;
  SeqEntryPtr   last, sep;
  Uint2         parenttype;
  Pointer       parentptr;

  if (top == NULL || top->choice != 2) return;
  bssp = (BioseqSetPtr) top->data.ptrvalue;
  if (bssp == NULL || bssp->_class != BioseqseqSet_class_gen_prod_set) return;

  GetSeqEntryParent (top, &parentptr, &parenttype);

  sep = bssp->seq_set;
  if (sep == NULL || sep->choice != 1) return;

  /* unlink nuc-prot sets, etc., from genomic Bioseq */

  MemSet ((Pointer) &dum, 0, sizeof (BioseqSet));
  dum._class = 1;
  dum.seq_set = sep->next;
  sep->next = NULL;

  last = sep;
  VisitBioseqsInSet (&dum, (Pointer) &last, TakeProteinsFromGPS);

  /* leave dum.seq_set dangling for now */

  bssp->_class = BioseqseqSet_class_nuc_prot;

  SeqMgrLinkSeqEntry (top, parenttype, parentptr);

  SeqMgrClearFeatureIndexes (bssp->idx.entityID, NULL);

  VisitFeaturesInSet (bssp, NULL, ClearRnaProducts);

  move_cds (top);
}

static void GeneralToNote (
  SeqFeatPtr sfp,
  Pointer userdata
)

{
  BioseqPtr  bsp;
  Char       buf [41];
  DbtagPtr   dbt;
  size_t     len;
  SeqIdPtr   sip;
  CharPtr    str;

  if (sfp == NULL || sfp->product == NULL) return;
  if (sfp->data.choice != SEQFEAT_RNA) return;

  bsp = BioseqFindFromSeqLoc (sfp->product);
  if (bsp == NULL) return;

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice != SEQID_GENERAL) continue;
    dbt = (DbtagPtr) sip->data.ptrvalue;
    if (dbt == NULL) continue;
    if (StringICmp (dbt->db, "TMSMART") == 0) continue;

    SeqIdWrite (sip, buf, PRINTID_REPORT, sizeof (buf) - 1);

    if (sfp->comment == NULL) {
      sfp->comment = StringSave (buf);
    } else {
      len = StringLen (sfp->comment) + StringLen (buf) + 5;
      str = MemNew (sizeof (Char) * len);
      StringCpy (str, sfp->comment);
      StringCat (str, "; ");
      StringCat (str, buf);
      sfp->comment = MemFree (sfp->comment);
      sfp->comment = str;
    }
  }
}

static SeqEntryPtr PropagateDescsFromGenBankSet (
  SeqEntryPtr sep
)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  SeqEntryPtr   firstsep = NULL;
  SeqEntryPtr   seqentry;
  ValNodePtr    sourcedescr;

  if (sep == NULL) return NULL;
  if (! IS_Bioseq_set (sep)) return sep;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp == NULL) return sep;
  sourcedescr = bssp->descr;
  if (sourcedescr == NULL) return sep;
  firstsep = bssp->seq_set;
  seqentry = firstsep;
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
  return firstsep;
}

static CharPtr orgmod_list [] = {
  "?", "?", "strain", "substrain", "type", "subtype", "variety",
  "serotype", "serogroup", "serovar", "cultivar", "pathovar", "chemovar",
  "biovar", "biotype", "group", "subgroup", "isolate", "common name",
  "acronym", "dosage", "natural host", "sub-species", "specimen-voucher",
  "authority", "forma", "forma-specialis", "ecotype", "synonym",
  "anamorph", "teleomorph", "breed", NULL
};

static CharPtr subsource_list [] = {
  "?", "chromosome", "map", "clone", "subclone", "haplotype",
  "genotype", "sex", "cell-line", "cell-type", "tissue-type",
  "clone-lib", "dev-stage", "frequency", "germline", "rearranged",
  "lab-host", "pop-variant", "tissue-lib", "plasmid-name", "transposon-name",
  "ins-seq-name", "plastid-name", "country", "segment", "endogenous-virus-name",
  "transgenic", "environmental-sample", "isolation-source",
  "lat-lon", "collection-date", "collected-by", "identified-by",
  "fwd-primer-seq", "rev-primer-seq", "fwd-primer-name", "rev-primer-name",
  NULL
};

typedef struct srcdata {
  Boolean  isSeqId;
  Boolean  isOrganism;
  Uint1    orgmodType;
  Uint1    subsourceType;
} SrcData, PNTR SrcDataPtr;

static void ParseOneOrgLabel (
  SrcDataPtr field,
  CharPtr label
)

{
  Int2  i;

  if (field == NULL || StringHasNoText (label)) return;

  if (StringICmp (label, "local_id") == 0 ||
      StringICmp (label, "local id") == 0 ||
      StringICmp (label, "SequenceID") == 0 ||
      StringICmp (label, "Sequence_ID") == 0 ||
      StringICmp (label, "Sequence ID") == 0 ||
      StringICmp (label, "SeqID") == 0 ||
      StringICmp (label, "Seq_ID") == 0 ||
      StringICmp (label, "Seq ID") == 0) {
    field->isSeqId = TRUE;
    return;
  }
  if (StringICmp (label, "organism") == 0) {
    field->isOrganism = TRUE;
    return;
  }
  for (i = 0; orgmod_list [i] != NULL; i++) {
    if (StringICmp (label, orgmod_list [i]) == 0) {
      field->orgmodType = (Uint1) i;
      return;
    }
  }
  for (i = 0; subsource_list [i] != NULL; i++) {
    if (StringICmp (label, subsource_list [i]) == 0) {
      field->subsourceType = (Uint1) i;
      return;
    }
  }
  if (StringICmp (label, "note") == 0) {
    field->subsourceType = (Uint1) SUBSRC_other;
  }
}

static void ProcessSourceTable (
  FILE *fp
)

{
  BioSourcePtr  biop;
  BioseqPtr     bsp;
  CharPtr       columns [80];
  FileCache     fc;
  SrcData       fields [80];
  Int2          i, numfields;
  Char          line [4095];
  OrgModPtr     omp;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  CharPtr       ptr, str;
  SeqDescrPtr   sdp;
  SeqIdPtr      sip;
  SubSourcePtr  ssp;

  if (fp == NULL) return;

  MemSet ((Pointer) fields, 0, sizeof (fields));
  numfields = 0;

  FileCacheSetup (&fc, fp);

  /* read first line with field names */

  str = FileCacheReadLine (&fc, line, sizeof (line), NULL);
  if (str == NULL) return;

  TrimSpacesAroundString (str);
  while (StringDoesHaveText (str) && numfields < 78) {
    ptr = StringChr (str, '\t');
    if (ptr != NULL) {
      *ptr = '\0';
      ptr++;
    }
    TrimSpacesAroundString (str);
    ParseOneOrgLabel (&(fields [numfields]), str);
    numfields++;
    str = ptr;
  }

  if (! fields [0].isSeqId) return;

  /* read remaining lines with source data */

  str = FileCacheReadLine (&fc, line, sizeof (line), NULL);
  while (str != NULL) {

    MemSet ((Pointer) columns, 0, sizeof (columns));

    TrimSpacesAroundString (str);
    i = 0;
    while (StringDoesHaveText (str) && i < numfields) {
      ptr = StringChr (str, '\t');
      if (ptr != NULL) {
        *ptr = '\0';
        ptr++;
      }
      TrimSpacesAroundString (str);
      columns [i] = str;
      i++;
      str = ptr;
    }

    if (StringDoesHaveText (columns [0])) {
      sip = MakeSeqID (columns [0]);
      if (sip != NULL) {
        bsp = BioseqFind (sip);
        if (bsp != NULL) {
          biop = NULL;
          sdp = GetNextDescriptorUnindexed (bsp, Seq_descr_source, NULL);
          if (sdp != NULL) {
            biop = (BioSourcePtr) sdp->data.ptrvalue;
          }
          if (biop == NULL) {
            biop = BioSourceNew ();
            if (biop != NULL) {
              SeqDescrAddPointer (&(bsp->descr), Seq_descr_source, (Pointer) biop);
            }
          }
          if (biop != NULL) {
            for (i = 1; i < numfields; i++) {
              if (StringHasNoText (columns [i])) continue;
              if (fields [i].isOrganism) {
                if (biop->org == NULL) {
                  biop->org = OrgRefNew ();
                }
                orp = biop->org;
                if (orp != NULL) {
                  orp->taxname = MemFree (orp->taxname);
                  orp->taxname = StringSave (columns [i]);
                }
              } else if (fields [i].orgmodType > 0) {
                if (biop->org == NULL) {
                  biop->org = OrgRefNew ();
                }
                orp = biop->org;
                if (orp != NULL) {
                  if (orp->orgname == NULL) {
                    orp->orgname = OrgNameNew ();
                  }
                  onp = orp->orgname;
                  if (onp != NULL) {
                    omp = OrgModNew ();
                    if (omp != NULL) {
                      omp->subtype = (Uint1) fields [i].orgmodType;
                      omp->subname = StringSave (columns [i]);
                      omp->next = onp->mod;
                      onp->mod = omp;
                    }
                  }
                }
              } else if (fields [i].subsourceType > 0) {
                ssp = SubSourceNew ();
                if (ssp != NULL) {
                  ssp->subtype = (Uint1) fields [i].subsourceType;
                  ssp->name = StringSave (columns [i]);
                  ssp->next = biop->subtype;
                  biop->subtype = ssp;
                }
              }
            }
          }
        }
        sip = SeqIdFree (sip);
      }
    }

    str = FileCacheReadLine (&fc, line, sizeof (line), NULL);
  }
}

static void ProcessOneRecord (
  SubmitBlockPtr sbp,
  PubdescPtr pdp,
  BioSourcePtr src,
  CharPtr directory,
  CharPtr results,
  CharPtr base,
  CharPtr suffix,
  SeqDescrPtr sdphead,
  TblArgsPtr tbl,
  TextFsaPtr gotags,
  AsnIoPtr aip
)

{
  AsnTypePtr         atp_bssse;
  BioSourcePtr       biop;
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  Char               buf [256];
  SeqMgrFeatContext  context;
  Pointer            dataptr;
  Uint2              datatype, entityID;
  SeqDescrPtr        descr;
  DatePtr            dp;
  BioseqSetPtr       dssp;
  FILE               *fp;
  Boolean            goOn;
  SeqEntryPtr        gsep = NULL;
  SeqGraphPtr        lastsgp;
  CharPtr            localname = NULL;
  ErrSev             msev;
  ObjMgrDataPtr      omdp;
  CharPtr            organism;
  OrgRefPtr          orp;
  CharPtr            ptr;
  SeqAnnotPtr        sap;
  SeqDescrPtr        sdp;
  SeqEntryPtr        sep;
  SeqFeatPtr         sfp;
  SeqGraphPtr        sgp;
  SeqIdPtr           sip;
  SeqSubmitPtr       sub;
  SimpleSeqPtr       ssp;
  CharPtr            str;
  SeqEntryPtr        tmp;

  fp = OpenOneFile (directory, base, suffix);
  if (fp == NULL) return;

  /* if genomic product set, make parent set */

  if (tbl->genprodset) {
    bssp = BioseqSetNew ();
    if (bssp == NULL) return;
    bssp->_class = BioseqseqSet_class_gen_prod_set;

    gsep = SeqEntryNew ();
    if (gsep == NULL) return;
    gsep->choice = 2;
    gsep->data.ptrvalue = (Pointer) bssp;
    SeqMgrSeqEntry (SM_BIOSEQSET, (Pointer) bssp, gsep);
  }

  if (tbl->seqidfromfile) {
    localname = base;
  }

  /* read one or more ASN.1 or FASTA sequence files */

  if (tbl->fastaset) {
    entityID = ProcessBulkSet (fp, src, tbl);
  } else if (tbl->deltaset) {
    entityID = ProcessDeltaSet (fp, src, tbl, localname, gsep);
  } else if (tbl->alignset) {
    entityID = ProcessAlignSet (fp, src, tbl);
  } else {
    entityID = ProcessOneAsn (fp, src, tbl, localname, gsep);
  }
  FileClose (fp);

  if (entityID == 0) return;

  /* optionally do network taxonomy lookup */

  if (tbl->dotaxlookup) {
    sep = GetTopSeqEntryForEntityID (entityID);
    Taxon3ReplaceOrgInSeqEntry (sep, FALSE);
  }

  organism = NULL;
  if (tbl->genprodset) {
    descr = ExtractBioSourceAndPubs (bssp->seq_set);
    for (sdp = descr; sdp != NULL; sdp = sdp->next) {
      if (sdp->choice != Seq_descr_source) continue;
      biop = (BioSourcePtr) sdp->data.ptrvalue;
      if (biop == NULL) continue;
      orp = biop->org;
      if (orp == NULL) continue;
      if (! StringHasNoText (orp->taxname)) {
        organism = orp->taxname;
      }
    }
    ReplaceBioSourceAndPubs (gsep, descr);
  }

  /* read one or more feature tables from .tbl file */

  fp = OpenOneFile (directory, base, ".tbl");
  if (fp != NULL) {

    /* indexing needed to find segmented bsp if location is on part */

    SeqMgrIndexFeatures (entityID, NULL);

    while ((dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, FALSE, TRUE, FALSE)) != NULL) {
      if (datatype == OBJ_SEQANNOT) {

        sap = (SeqAnnotPtr) dataptr;
        ProcessOneAnnot (sap, entityID, tbl);

      } else {
        ObjMgrFree (datatype, dataptr);
      }
    }
    FileClose (fp);
  }

  /* if genomic product set, copy CDS into nucprot sets */

  if (tbl->genprodset) {
    /* need to reindex to get mRNA and CDS features from cDNA and protein */
    SeqMgrIndexFeatures (entityID, NULL);
    VisitSetsInSet (bssp, (Pointer) tbl, MakeNucProtCDS);
  }

  /* read source qualifiers for set of sequences from .src file */

  fp = OpenOneFile (directory, base, ".src");
  if (fp != NULL) {

    ProcessSourceTable (fp);

    FileClose (fp);
  }

  /* read one or more feature tables from .pep file */

  fp = OpenOneFile (directory, base, ".pep");
  if (fp != NULL) {

    /* indexing needed to find CDS from protein product to set conflict flag */

    SeqMgrIndexFeatures (entityID, NULL);

    while ((dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, TRUE, TRUE, TRUE)) != NULL) {
      if (datatype == OBJ_FASTA) {

        ssp = (SimpleSeqPtr) dataptr;
        ReplaceOnePeptide (ssp, tbl->conflict);
        SimpleSeqFree (ssp);

      } else {
        ObjMgrFree (datatype, dataptr);
      }
    }
    FileClose (fp);
  }

  /* read one or more feature tables from .rna file */

  fp = OpenOneFile (directory, base, ".rna");
  if (fp != NULL) {

    /* indexing needed to find mRNA from transcript product to set RNA editing exception */

    SeqMgrIndexFeatures (entityID, NULL);

    while ((dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, TRUE, TRUE, TRUE)) != NULL) {
      if (datatype == OBJ_FASTA) {

        ssp = (SimpleSeqPtr) dataptr;
        ReplaceOneRNA (ssp, tbl->conflict);
        SimpleSeqFree (ssp);

      } else {
        ObjMgrFree (datatype, dataptr);
      }
    }
    FileClose (fp);
  }

  /* read one or more quality score blocks from .qvl file */

  fp = OpenOneFile (directory, base, ".qvl");
  if (fp != NULL) {

    goOn = TRUE;
    while (goOn) {
      str = ReadALine (buf, sizeof (buf), fp);
      if (str == NULL) {
        goOn = FALSE;
      } else if (! StringHasNoText (str)) {
        if (str [0] == '>') {
          ptr = StringChr (str, ' ');
          if (ptr != NULL) {
            *ptr = '\0';
          }
          sip = MakeSeqID (str + 1);
          bsp = BioseqFind (sip);
          if (bsp != NULL) {
            sgp = ReadPhrapQuality (fp, bsp);
            if (sgp != NULL) {
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
                  sap->next = NewGraphSeqAnnot ("Graphs", sgp);
                } else {
                  bsp->annot = NewGraphSeqAnnot ("Graphs", sgp);
                }
              }
            }
          }
          SeqIdFree (sip);
        }
      }
    }
    FileClose (fp);
  }

  /* finish processing */

  if (sbp == NULL) {
    omdp = ObjMgrGetData (entityID);
    if (omdp != NULL && omdp->datatype == OBJ_SEQSUB) {

      /* if read a Seq-submit, write out a Seq-submit */

      sub = (SeqSubmitPtr) omdp->dataptr;
      if (sub != NULL && sub->datatype == 1) {
        sbp = sub->sub;
      }
    }
  }

  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep != NULL) {

    if (tbl->gnltonote) {
      VisitFeaturesInSep (sep, NULL, GeneralToNote);
    }

    if (tbl->gpstonps) {
      GPStoNPS (sep, entityID);
    }

    if (! tbl->genprodset) {
      VisitFeaturesInSep (sep, NULL, RemoveGBQualIDs);
    }
    if (sdphead != NULL) {
      if (IS_Bioseq (sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        ValNodeLink (&(bsp->descr),
                     AsnIoMemCopy ((Pointer) sdphead,
                                   (AsnReadFunc) SeqDescrAsnRead,
                                   (AsnWriteFunc) SeqDescrAsnWrite));
      } else if (IS_Bioseq_set (sep)) {
        dssp = (BioseqSetPtr) sep->data.ptrvalue;
        ValNodeLink (&(dssp->descr),
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
    msev = ErrSetMessageLevel (SEV_MAX);
    move_cds (sep);

    /* if reading nucleotide and protein tables, remove duplicate prot feat */
    VisitBioseqsInSep (sep, NULL, RemoveDupProtFeats);
    DeleteMarkedObjects (entityID, 0, NULL);

    /* need to reindex before extending CDS to stop codon */
    SeqMgrIndexFeatures (entityID, NULL);
    CdCheck (sep, NULL);

    /* need to reindex before copying genes, instantiating protein titles */
    SeqMgrIndexFeatures (entityID, NULL);
    EntryChangeImpFeat (sep);

    if (tbl->removeunnecxref) {
      /* if not removed, xref will prevent locus, maploc, dbxref from being copied */
      VisitFeaturesInSep (sep, NULL, RemoveUnnecGeneXref);
    }

    if (tbl->genprodset) {
      VisitFeaturesInSep (sep, NULL, CopyGene);
    }
    if (! tbl->genprodset) {
      VisitFeaturesInSep (sep, NULL, ClearRnaProducts);
    }

    if (tbl->removeunnecxref) {
      /* need to reindex before removing unnecesary gene xrefs in nuc-prot sets */
      SeqMgrIndexFeatures (entityID, NULL);

      VisitFeaturesInSep (sep, NULL, RemoveUnnecGeneXref);
    }

    if (SeqMgrFeaturesAreIndexed (entityID)) {
      InstantiateProteinTitles (entityID, NULL);
    } else {
      Message (MSG_POSTERR, "Unable to instantiate protein titles due to dropped index");
    }

    if (tbl->genprodset) {
      /* need to reindex before instantiating mRNA titles */
      SeqMgrIndexFeatures (entityID, NULL);
      bsp = FindNucBioseq (sep);
      sfp = SeqMgrGetNextFeature (bsp, NULL, 0, FEATDEF_mRNA, &context);
      while (sfp != NULL) {
        AddMrnaTitles (sfp->product, organism);
        sfp = SeqMgrGetNextFeature (bsp, sfp, 0, FEATDEF_mRNA, &context);
      }
    }
    SeqMgrClearFeatureIndexes (entityID, NULL);
    BasicSeqEntryCleanup (sep);
    ErrSetMessageLevel (msev);
    /*
    SeriousSeqEntryCleanup (sep, NULL, NULL);
    */
    if (StringHasNoText (results)) {
      results = directory;
    }

    if (aip != NULL) {
      atp_bssse = AsnFind ("Bioseq-set.seq-set.E");
      if (atp_bssse == NULL) {
        Message (MSG_POSTERR, "Unable to find ASN.1 type Bioseq-set.seq-set.E");
      } else if (tbl->fastaset) {
        /* already has genbank wrapper, write individual components */
        tmp = PropagateDescsFromGenBankSet (sep);
        while (tmp != NULL) {
          SeqEntryAsnWrite (tmp, aip, atp_bssse);
          tmp = tmp->next;
        }
      } else {
        SeqEntryAsnWrite (sep, aip, atp_bssse);
      }
    } else {
      WriteOneFile (results, base, ".sqn", sep, sbp);
    }

    if (HasGoTermsInNote (sep, gotags)) {
      Message (MSG_OK, "Illegal GO term format detected in note - contact database for instructions");
    }

    if (tbl->validate || tbl->flatfile) {
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
      if (tbl->flatfile) {
        Message (MSG_POST, "Flatfile %s\n", base);
        FlatfileOneFile (results, base, ".gbf", sep);
      }
      if (tbl->validate) {
        Message (MSG_POST, "Validating %s\n", base);
        ValidateOneFile (results, base, ".val", sep);
      }
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

static void FileRecurse (
  SubmitBlockPtr sbp,
  PubdescPtr pdp,
  BioSourcePtr src,
  CharPtr directory,
  CharPtr results,
  CharPtr suffix,
  SeqDescrPtr sdphead,
  TblArgsPtr tbl,
  TextFsaPtr gotags,
  AsnIoPtr aip
)

{
  Char        path [PATH_MAX];
  CharPtr     ptr;
  CharPtr     str;
  ValNodePtr  head, vnp;

  /* get list of all files in source directory */

  head = DirCatalog (directory);

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == 0) {
      str = (CharPtr) vnp->data.ptrvalue;
      if (! StringHasNoText (str)) {

        /* does filename have desired substring? */

        ptr = StringStr (str, suffix);
        if (ptr != NULL) {
          *ptr = '\0';

          /* process file that has desired suffix (usually .fsa) */

          ProcessOneRecord (sbp, pdp, src, directory, results, str, suffix, sdphead, tbl, gotags, aip);
        }
      }
    } else if (vnp->choice == 1) {

      /* recurse into subdirectory */

      StringNCpy_0 (path, directory, sizeof (path));
      str = (CharPtr) vnp->data.ptrvalue;
      FileBuildPath (path, str, NULL);
      FileRecurse (sbp, pdp, src, path, results, suffix, sdphead, tbl, gotags, aip);
    }
  }

  /* clean up file list */

  ValNodeFreeData (head);
}

static AsnTypePtr DoFirstPrefix (
  AsnIoPtr aip,
  SubmitBlockPtr sbp
)

{
  AsnTypePtr  atp_se, atp_ses, atp_ss, atp_ssd, atp_ssde, atp_ssdee, atp_sss, sep_atp, ssp_atp;
  DataVal     av;
  SeqEntry    se;
  SeqSubmit   ss;

  if (aip == NULL || sbp == NULL) return NULL;

  atp_ss = AsnFind ("Seq-submit");
  if (atp_ss == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Seq-submit");
    return NULL;
  }

  atp_sss = AsnFind ("Seq-submit.sub");
  if (atp_sss == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Seq-submit.sub");
    return NULL;
  }

  atp_ssd = AsnFind ("Seq-submit.data");
  if (atp_ssd == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Seq-submit.data");
    return NULL;
  }

  atp_ssde = AsnFind ("Seq-submit.data.entrys");
  if (atp_ssde == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Seq-submit.data.entrys");
    return NULL;
  }

  atp_se = AsnFind ("Seq-entry");
  if (atp_se == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Seq-entry");
    return NULL;
  }

  atp_ses = AsnFind ("Seq-entry.set");
  if (atp_ses == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Seq-entry.set");
    return NULL;
  }

  atp_ssdee = AsnFind ("Seq-submit.data.entrys.E");
  if (atp_ssdee == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Seq-submit.data.entrys.E");
    return NULL;
  }


  ssp_atp = AsnLinkType (NULL, atp_ss);
  if (ssp_atp == NULL) return NULL;

  MemSet ((Pointer) &ss, 0, sizeof (SeqSubmit));
  MemSet ((Pointer) &se, 0, sizeof (SeqEntry));
  se.choice = 2;

  if (! AsnOpenStruct (aip, ssp_atp, (Pointer) &ss)) return NULL;

  if (! SubmitBlockAsnWrite (sbp, aip, atp_sss)) return NULL;

  av.ptrvalue = (Pointer) &se;
  if (! AsnWriteChoice (aip, atp_ssd, (Int2) 1, &av)) return NULL;

  if (! AsnOpenStruct (aip, atp_ssde, (Pointer) &se)) return NULL;

  sep_atp = AsnLinkType (atp_ssdee, atp_se);
  if (sep_atp == NULL) return NULL;

  av.ptrvalue = (Pointer) &se;
  se.choice = 2;
  if (! AsnWriteChoice (aip, sep_atp, (Int2) 2, &av)) return NULL;

  return ssp_atp;
}

static AsnTypePtr DoSecondPrefix (
  AsnIoPtr aip
)

{
  AsnTypePtr  atp_bsc, atp_bss, atp_bsss, atp_ses, bssp_atp;
  DataVal     av;
  BioseqSet   bs;

  atp_ses = AsnFind ("Seq-entry.set");
  if (atp_ses == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Seq-entry.set");
    return NULL;
  }

  atp_bss = AsnFind ("Bioseq-set");
  if (atp_bss == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Bioseq-set");
    return NULL;
  }

  atp_bsc = AsnFind ("Bioseq-set.class");
  if (atp_bsc == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Bioseq-set.class");
    return NULL;
  }

  atp_bsss = AsnFind ("Bioseq-set.seq-set");
  if (atp_bsss == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Bioseq-set.seq-set");
    return NULL;
  }


  bssp_atp = AsnLinkType (atp_ses, atp_bss);
  if (bssp_atp == NULL) return NULL;

  MemSet ((Pointer) &bs, 0, sizeof (BioseqSet));

  if (! AsnOpenStruct (aip, bssp_atp, (Pointer) &bs)) return NULL;

  av.intvalue = BioseqseqSet_class_genbank;
  if (! AsnWrite (aip, atp_bsc, &av)) return NULL;

  if (! AsnOpenStruct (aip, atp_bsss, (Pointer) &bs.seq_set)) return NULL;

  return bssp_atp;
}

static Boolean DoFirstSuffix (
  AsnIoPtr aip,
  AsnTypePtr ssp_atp
)

{
  AsnTypePtr  atp_bsss, atp_ssde, atp_ssdee;
  BioseqSet   bs;
  SeqEntry    se;
  SeqSubmit   ss;

  if (aip == NULL || ssp_atp == NULL) return FALSE;

  atp_ssde = AsnFind ("Seq-submit.data.entrys");
  if (atp_ssde == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Seq-submit.data.entrys");
    return FALSE;
  }

  atp_ssdee = AsnFind ("Seq-submit.data.entrys.E");
  if (atp_ssdee == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Seq-submit.data.entrys.E");
    return FALSE;
  }

  atp_bsss = AsnFind ("Bioseq-set.seq-set");
  if (atp_bsss == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Bioseq-set.seq-set");
    return FALSE;
  }


  MemSet ((Pointer) &ss, 0, sizeof (SeqSubmit));
  MemSet ((Pointer) &se, 0, sizeof (SeqEntry));
  MemSet ((Pointer) &bs, 0, sizeof (BioseqSet));

  if (! AsnCloseStruct (aip, atp_ssde, &se)) return FALSE;

  if (! AsnCloseStruct (aip, ssp_atp, (Pointer) &ss)) return FALSE;

  AsnUnlinkType (atp_ssdee);

  return TRUE;
}

static Boolean DoSecondSuffix (
  AsnIoPtr aip,
  AsnTypePtr bssp_atp
)

{
  AsnTypePtr  atp_bsss, atp_ses;
  BioseqSet   bs;
 
   if (aip == NULL || bssp_atp == NULL) return FALSE;

  atp_ses = AsnFind ("Seq-entry.set");
  if (atp_ses == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Seq-entry.set");
    return FALSE;
  }

  atp_bsss = AsnFind ("Bioseq-set.seq-set");
  if (atp_bsss == NULL) {
    Message (MSG_POSTERR, "Unable to find ASN.1 type Bioseq-set.seq-set");
    return FALSE;
  }


  MemSet ((Pointer) &bs, 0, sizeof (BioseqSet));

  if (! AsnCloseStruct(aip, atp_bsss, (Pointer) &bs.seq_set)) return FALSE;

  if (! AsnCloseStruct (aip, bssp_atp, (Pointer) &bs)) return FALSE;

  AsnUnlinkType (atp_ses);

  return TRUE;
}

/* Args structure contains command-line arguments */

#define p_argInputPath   0
#define r_argOutputPath  1
#define i_argInputFile   2
#define o_argOutputFile  3
#define x_argSuffix      4
#define t_argTemplate    5
#define s_argFastaSet    6
#define w_argWhichClass  7
#define d_argDeltaSet    8
#define l_argAlignment   9
#define g_argGenProdSet 10
#define a_argAccession  11
#define n_argOrgName    12
#define j_argSrcQuals   13
#define y_argComment    14
#define c_argFindOrf    15
#define m_argAltStart   16
#define k_argConflict   17
#define v_argValidate   18
#define b_argGenBank    19
#define q_argFileID     20
#define u_argUndoGPS    21
#define h_argGnlToNote  22
#define B_argBegGap     23
#define E_argEndGap     24
#define G_argMidGap     25
#define X_argMissing    26
#define M_argMatch      27
#define P_argIsProt     28
#define R_argRemote     29
#define S_argSmart      30
#define U_argUnnecXref  31
#define L_argLocalID    32
#define T_argTaxLookup  33

Args myargs [] = {
  {"Path to files", NULL, NULL, NULL,
    TRUE, 'p', ARG_STRING, 0.0, 0, NULL},
  {"Path for results", NULL, NULL, NULL,
    TRUE, 'r', ARG_STRING, 0.0, 0, NULL},
  {"Single input file", NULL, NULL, NULL,
    TRUE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  {"Single output file", NULL, NULL, NULL,
    TRUE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
  {"Suffix", ".fsa", NULL, NULL,
    TRUE, 'x', ARG_STRING, 0.0, 0, NULL},
  {"Template file", NULL, NULL, NULL,
    TRUE, 't', ARG_FILE_IN, 0.0, 0, NULL},
  {"Read FASTAs as Set", "F", NULL, NULL,
    TRUE, 's', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Fasta Set Class (1 Pop, 2 Phy, 3 Mut, 4 Eco)", "0", "0", "4",
    FALSE, 'w', ARG_INT, 0.0, 0, NULL},
  {"Read FASTAs as Delta", "F", NULL, NULL,
    TRUE, 'd', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Read FASTA+Gap Alignment", "F", NULL, NULL,
    TRUE, 'l', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Genomic Product Set", "F", NULL, NULL,
    TRUE, 'g', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Accession", NULL, NULL, NULL,
    TRUE, 'a', ARG_STRING, 0.0, 0, NULL},
  {"Organism name", NULL, NULL, NULL,
    TRUE, 'n', ARG_STRING, 0.0, 0, NULL},
  {"Source qualifiers", NULL, NULL, NULL,
    TRUE, 'j', ARG_STRING, 0.0, 0, NULL},
  {"Comment", NULL, NULL, NULL,
    TRUE, 'y', ARG_STRING, 0.0, 0, NULL},
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
  {"Seq ID from file name", "F", NULL, NULL,
    TRUE, 'q', ARG_BOOLEAN, 0.0, 0, NULL},
  {"GenProdSet to NucProtSet", "F", NULL, NULL,
    TRUE, 'u', ARG_BOOLEAN, 0.0, 0, NULL},
  {"General ID to Note", "F", NULL, NULL,
    TRUE, 'h', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Alignment Beginning Gap Characters",  NULL, NULL, NULL,
    TRUE, 'B', ARG_STRING, 0.0, 0, NULL},
  {"Alignment End Gap Characters",  NULL, NULL, NULL,
    TRUE, 'E', ARG_STRING, 0.0, 0, NULL},
  {"Alignment Middle Gap Characters",  NULL, NULL, NULL,
    TRUE, 'G', ARG_STRING, 0.0, 0, NULL},
  {"Alignment Missing Characters",  NULL, NULL, NULL,
    TRUE, 'X', ARG_STRING, 0.0, 0, NULL},
  {"Alignment Match Characters",  NULL, NULL, NULL,
    TRUE, 'M', ARG_STRING, 0.0, 0, NULL},
  {"Alignment Is Proteins", "F", NULL, NULL,
    TRUE, 'P', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Remote Fetching from ID", "F", NULL, NULL,
    TRUE, 'R', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Smart Feature Annotation", "F", NULL, NULL,
    TRUE, 'S', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Remove Unnecessary Gene Xref", "F", NULL, NULL,
    TRUE, 'U', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Force Local protein_id/transcript_id", "F", NULL, NULL,
    TRUE, 'L', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Remote Taxonomy Lookup", "F", NULL, NULL,
    TRUE, 'T', ARG_BOOLEAN, 0.0, 0, NULL},
};

Int2 Main (void)

{
  AsnIoPtr        aip = NULL;
  Char            app [64];
  CharPtr         base;
  AsnTypePtr      bssp_atp = NULL;
  CitSubPtr       csp;
  Pointer         dataptr;
  Uint2           datatype;
  CharPtr         directory;
  FILE            *fp;
  TextFsaPtr      gotags;
  CharPtr         os;
  CharPtr         outfile;
  Pubdesc         pd;
  PubdescPtr      pdp = NULL;
  ValNode         pb;
  CharPtr         ptr;
  Boolean         remote;
  CharPtr         results;
  SubmitBlockPtr  sbp = NULL;
  SeqDescrPtr     sdphead = NULL;
  SeqEntryPtr     sep;
  Char            sfx [32];
  BioSourcePtr    src = NULL;
  SeqSubmitPtr    ssp = NULL;
  AsnTypePtr      ssp_atp = NULL;
  Char            str [64];
  CharPtr         suffix;
  TblArgs         tbl;
  CharPtr         tmplate;

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

  sprintf (app, "tbl2asn %s", TBL2ASN_APPLICATION);
  if (! GetArgs (app, sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  directory = (CharPtr) myargs [p_argInputPath].strvalue;
  results = (CharPtr) myargs [r_argOutputPath].strvalue;
  if (StringHasNoText (results)) {
    results = NULL;
  }
  suffix = (CharPtr) myargs [x_argSuffix].strvalue;
  base = (CharPtr) myargs [i_argInputFile].strvalue;
  outfile = (CharPtr) myargs [o_argOutputFile].strvalue;
  tmplate = (CharPtr) myargs [t_argTemplate].strvalue;

  remote = (Boolean) myargs [R_argRemote].intvalue;

  tbl.fastaset = (Boolean) myargs [s_argFastaSet].intvalue;
  tbl.whichclass = (Boolean) myargs [w_argWhichClass].intvalue;
  tbl.deltaset = (Boolean) myargs [d_argDeltaSet].intvalue;
  tbl.alignset = (Boolean) myargs [l_argAlignment].intvalue;
  tbl.genprodset = (Boolean) myargs [g_argGenProdSet].intvalue;
  tbl.forcelocalid = (Boolean) myargs [L_argLocalID].intvalue;
  tbl.gpstonps = (Boolean) myargs [u_argUndoGPS].intvalue;
  tbl.gnltonote = (Boolean) myargs [h_argGnlToNote].intvalue;
  tbl.accn = (CharPtr) myargs [a_argAccession].strvalue;
  tbl.organism = (CharPtr) myargs [n_argOrgName].strvalue;
  tbl.srcquals = (CharPtr) myargs [j_argSrcQuals].strvalue;
  tbl.comment = (CharPtr) myargs [y_argComment].strvalue;
  tbl.findorf = (Boolean) myargs [c_argFindOrf].intvalue;
  tbl.altstart = (Boolean) myargs [m_argAltStart].intvalue;
  tbl.conflict = (Boolean) myargs [k_argConflict].intvalue;
  tbl.validate = (Boolean) myargs [v_argValidate].intvalue;
  tbl.flatfile = (Boolean) myargs [b_argGenBank].intvalue;
  tbl.seqidfromfile = (Boolean) myargs [q_argFileID].intvalue;
  tbl.smartfeats = (Boolean) myargs [S_argSmart].intvalue;
  tbl.removeunnecxref = (Boolean) myargs [U_argUnnecXref].intvalue;
  tbl.dotaxlookup = (Boolean) myargs [T_argTaxLookup].intvalue;

  /* arguments for alignment reading */
  tbl.aln_beginning_gap = (CharPtr) myargs [B_argBegGap].strvalue;
  tbl.aln_end_gap = (CharPtr) myargs [E_argEndGap].strvalue;
  tbl.aln_middle_gap = (CharPtr) myargs [G_argMidGap].strvalue;
  tbl.aln_missing = (CharPtr) myargs [X_argMissing].strvalue;
  tbl.aln_match = (CharPtr) myargs [M_argMatch].strvalue;
  tbl.aln_is_protein = (Boolean) myargs [P_argIsProt].intvalue;


  if (StringHasNoText (tbl.accn)) {
    tbl.accn = NULL;
  }
  if (StringHasNoText (tbl.organism)) {
    tbl.organism = NULL;
  }
  if (StringHasNoText (tbl.srcquals)) {
    tbl.srcquals = NULL;
  }
  if (StringHasNoText (tbl.comment)) {
    tbl.comment = NULL;
  }

  if (tbl.fastaset && (tbl.deltaset || tbl.alignset || tbl.genprodset)) {
    Message (MSG_FATAL, "-s cannot be used with -d, -g or -l");
    return 1;
  }

  if (! tbl.alignset && (! StringHasNoText (tbl.aln_beginning_gap)
	  || ! StringHasNoText (tbl.aln_end_gap)
	  || ! StringHasNoText (tbl.aln_middle_gap)
	  || ! StringHasNoText (tbl.aln_missing)
	  || ! StringHasNoText (tbl.aln_match)
	  || tbl.aln_is_protein)) {
    Message (MSG_FATAL, "Alignment options (-B, -E, -G, -X, -M, -P) can only be used with -l");
	return 1;
  }

  if (StringHasNoText (base) && (! StringHasNoText (tbl.accn))) {
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
      os = GetOpSysString ();
      if (os != NULL) {
        sprintf (str, "tbl2asn %s - %s", TBL2ASN_APPLICATION, os);
      } else {
        sprintf (str, "tbl2asn %s", TBL2ASN_APPLICATION);
      }
      sbp->tool = StringSave (str);
      MemFree (os);
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

  gotags = TextFsaNew ();
  TextFsaAdd (gotags, "go_component");
  TextFsaAdd (gotags, "go_function");
  TextFsaAdd (gotags, "go_process");

  /* register fetch function */

  if (remote) {
    PubSeqFetchEnable ();
  }

  /* process one or more records */

  if (! StringHasNoText (outfile)) {
    aip = AsnIoOpen (outfile, "w");
    if (aip == NULL) {
      Message (MSG_FATAL, "Unable to open single output file");
      return 1;
    }
    ssp_atp = DoFirstPrefix (aip, sbp);
    bssp_atp = DoSecondPrefix (aip);
  }

  if (! StringHasNoText (base)) {
    ptr = StringRChr (base, '.');
    sfx[0] = '\0';
    if (ptr != NULL) {
      StringNCpy_0 (sfx, ptr, sizeof (sfx));
      *ptr = '\0';
    }
    ProcessOneRecord (sbp, pdp, src, directory, results, base, sfx, sdphead, &tbl, gotags, aip);

  } else {

    FileRecurse (sbp, pdp, src, directory, results, suffix, sdphead, &tbl, gotags, aip);
  }

  if (aip != NULL) {
    DoSecondSuffix (aip, bssp_atp);
    DoFirstSuffix (aip, ssp_atp);
    AsnIoClose (aip);
  }

  if (sbp != NULL) {
    SubmitBlockFree (sbp);
  }
  if (src != NULL) {
    BioSourceFree (src);
  }

  SeqDescrFree (sdphead);

  TransTableFreeAll ();

  TextFsaFree (gotags);

  /* close fetch function */

  if (remote) {
    PubSeqFetchDisable ();
  }

  return 0;
}

