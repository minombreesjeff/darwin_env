/*   sgd2asn.c
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
* File Name:  sgd2asn.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   8/20/99
*
* $Revision: 6.14 $
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
#include <asn2ff.h>
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

static void WriteOneFile (
  CharPtr directory,
  CharPtr base,
  CharPtr suffix,
  SeqEntryPtr sep
)

{
  AsnIoPtr  aip;
  Char      file [FILENAME_MAX], path [PATH_MAX];

  StringNCpy_0 (path, directory, sizeof (path));
  sprintf (file, "%s.%s", base, suffix);
  FileBuildPath (path, NULL, file);

  aip = AsnIoOpen (path, "w");
  if (aip == NULL) return;

  SeqEntryAsnWrite (sep, aip, NULL);

  AsnIoFlush (aip);
  AsnIoClose (aip);
}

static void ValidateOneFile (
  CharPtr directory,
  CharPtr base,
  CharPtr suffix,
  SeqEntryPtr sep
)

{
  Char            file [FILENAME_MAX], path [PATH_MAX];
  ErrSev          oldErrSev;
  ValidStructPtr  vsp;

  StringNCpy_0 (path, directory, sizeof (path));
  sprintf (file, "%s.%s", base, suffix);
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
  CharPtr directory,
  CharPtr base,
  CharPtr suffix,
  SeqEntryPtr sep
)

{
  Char    file [FILENAME_MAX], path [PATH_MAX];
  FILE    *fp;
  ErrSev  oldErrSev;

  StringNCpy_0 (path, directory, sizeof (path));
  sprintf (file, "%s.%s", base, suffix);
  FileBuildPath (path, NULL, file);

  fp = FileOpen (path, "w");
  if (fp == NULL) return;

  oldErrSev = ErrSetMessageLevel (SEV_MAX);
  SeqEntryToFlat (sep, fp, GENBANK_FMT, SEQUIN_MODE);
  ErrSetMessageLevel (oldErrSev);

  FileClose (fp);
}

/* source information for several common organisms sequenced by genome centers */

typedef struct orgstuff {
  CharPtr  taxname;
  CharPtr  lineage;
  CharPtr  division;
  Uint1    gcode;
  Uint1    mgcode;
  Int4     taxID;
} OrgStuff, PNTR OrfStuffPtr;

static OrgStuff commonOrgStuff [] = {
  {
    "Saccharomyces cerevisiae",
    "Eukaryota; Fungi; Ascomycota; Hemiascomycetes; Saccharomycetales; Saccharomycetaceae; Saccharomyces",
    "PLN", 1, 3, 4932
  },
  {
    "Drosophila melanogaster",
    "Eukaryota; Metazoa; Arthropoda; Tracheata; Hexapoda; Insecta; Pterygota; Diptera; Brachycera; Muscomorpha; Ephydroidea; Drosophilidae; Drosophila",
    "INV", 1, 5, 7227
  },
  {
    "Homo sapiens",
    "Eukaryota; Metazoa; Chordata; Vertebrata; Mammalia; Eutheria; Primates; Catarrhini; Hominidae; Homo",
    "PRI", 1, 2, 9606
  },
  {
    "Escherichia coli",
    "Bacteria; Proteobacteria; gamma subdivision; Enterobacteriaceae; Escherichia",
    "BCT", 11, 0, 562
  },
  {
    "Helicobacter pylori",
    "Bacteria; Proteobacteria; epsilon subdivision; Helicobacter group; Helicobacter",
    "BCT", 11, 0, 210
  },
  {
    NULL, NULL, NULL, 0, 0, 0
  }
};

/* organism from title overrides organism from command line */

static BioSourcePtr OldParseTitleIntoBioSource (
  SqnTagPtr stp,
  BioseqPtr bsp,
  CharPtr organism
)

{
  BioSourcePtr  biop = NULL;
  ValNodePtr    db;
  DbtagPtr      dbt;
  Int2          idx;
  ObjectIdPtr   oip;
  OrgModPtr     omp;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  SubSourcePtr  ssp;
  CharPtr       str;
  int           val;

  if (stp == NULL && StringHasNoText (organism)) return NULL;

  str = SqnTagFind (stp, "top");
  if (str != NULL && bsp != NULL) {
    if (StringICmp (str, "linear") == 0) {
      bsp->topology = TOPOLOGY_LINEAR;
    } else if (StringICmp (str, "circular") == 0) {
      bsp->topology = TOPOLOGY_CIRCULAR;
    }
  }
  str = SqnTagFind (stp, "org");
  if (str == NULL) {
    str = organism;
  }
  if (StringHasNoText (str)) return NULL;
   biop = BioSourceNew ();
  if (biop == NULL) return NULL;
  orp = OrgRefNew ();
  if (orp == NULL) return NULL;
  biop->org = orp;
  onp = OrgNameNew ();
  if (onp == NULL) return NULL;
  orp->orgname = onp;
  orp->taxname = StringSave (str);

  /* look for entry of organisms in commonOrgStuff table */

  for (idx = 0; commonOrgStuff [idx].taxname != NULL; idx++) {
    if (StringICmp (orp->taxname, commonOrgStuff [idx].taxname) == 0) {
      onp->gcode = commonOrgStuff [idx].gcode;
      onp->mgcode = commonOrgStuff [idx].mgcode;
      onp->div = StringSave (commonOrgStuff [idx].division);
      onp->lineage = StringSave (commonOrgStuff [idx].lineage);
      db = ValNodeNew (NULL);
      if (db != NULL) {
        dbt = DbtagNew ();
        if (dbt != NULL) {
          oip = ObjectIdNew ();
          if (oip != NULL) {
            oip->id = commonOrgStuff [idx].taxID;
            dbt->db = StringSave ("taxon");
            dbt->tag = oip;
            db->data.ptrvalue = (Pointer) dbt;
            orp->db = db;
          }
        }
      }
    }
  }

  /* otherwise can get genetic codes from FASTA defline */

  str = SqnTagFind (stp, "gcode");
  if (str != NULL && sscanf (str, "%d", &val) == 1) {
    onp->gcode = (Uint1) val; /* cytoplasmic */
  }

  str = SqnTagFind (stp, "mgcode");
  if (str != NULL && sscanf (str, "%d", &val) == 1) {
    onp->mgcode = (Uint1) val; /* mitochondrial */
  }

  str = SqnTagFind (stp, "location");
  if (str != NULL && StringICmp (str, "mitochondrion") == 0) {
    biop->genome = GENOME_mitochondrion;
  } else {
    biop->genome = GENOME_genomic;
  }

  str = SqnTagFind (stp, "strain");
  if (str != NULL) {
    omp = OrgModNew ();
    if (omp != NULL) {
      omp->subtype = ORGMOD_strain;
      omp->subname = StringSave (str);
      onp->mod = omp;
    }
  }

  str = SqnTagFind (stp, "chromosome");
  if (str != NULL) {
    ssp = SubSourceNew ();
    if (ssp != NULL) {
      ssp->subtype = SUBSRC_chromosome;
      ssp->name = StringSave (str);
      biop->subtype = ssp;
    }
  }

  return biop;
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

static void AnnotateBestOrf (
  BioseqPtr bsp,
  Int2 genCode,
  Boolean altstart
  
)

{
  CdRegionPtr     crp;
  Int2            i, best, idx;
  OrfData         od;
  ProtRefPtr      prp;
  SeqFeatPtr      sfp;
  SeqInt          sint;
  TransTablePtr   tbl;
  ValNode         vn;
  SeqFeatXrefPtr  xref;

  if (bsp == NULL) return;
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

  tbl = TransTableNew (genCode);
  if (tbl != NULL) {
    TransTableProcessBioseq (tbl, LookForOrfs, (Pointer) &od, bsp);
  }
  TransTableFree (tbl);
  best = -1;
  idx = -1;
  for (i = 0; i < 6; i++) {
    if (od.bestlen [i] > best) {
      best = od.bestlen [i];
      idx = i;
    }
  }
  if (idx == -1) return;

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

  /* make CDS feature with unknown product */

  sfp = CreateNewFeatureOnBioseq (bsp, SEQFEAT_CDREGION, &vn);
  if (sfp == NULL) return;
  crp = CreateNewCdRgn (1, FALSE, genCode);
  if (crp == NULL) return;
  crp->frame = 1;
  sfp->data.value.ptrvalue = (Pointer) crp;

  prp = ProtRefNew ();
  if (prp == NULL) return;
  xref = SeqFeatXrefNew ();
  if (xref == NULL) return;
  xref->data.choice = SEQFEAT_PROT;
  xref->data.value.ptrvalue = (Pointer) prp;
  xref->next = sfp->xref;
  sfp->xref = xref;
  prp->name = ValNodeCopyStr (NULL, 0, "unknown");
}

static void ProcessOneRecord (
  CharPtr directory,
  CharPtr base,
  CharPtr organism,
  Boolean findorf,
  Boolean altstart,
  Boolean validate,
  Boolean flatfile
)

{
  BioSourcePtr  biop = NULL;
  BioseqPtr     bsp;
  Uint2         datatype, entityID;
  Int2          genCode;
  MolInfoPtr    mip;
  SeqAnnotPtr   sap;
  SeqEntryPtr   sep;
  SeqFeatPtr    sfp;
  SqnTagPtr     stp;
  CharPtr       ttl;
  ValNodePtr    vnp;

  Message (MSG_POST, "Processing %s\n", base);

  bsp = (BioseqPtr) ReadOneFile (directory, base, "fsa", &datatype);
  if (bsp == NULL || datatype != OBJ_BIOSEQ) {
    ObjMgrFree (datatype, (Pointer) bsp);
    return;
  }

  if (bsp->mol == Seq_mol_na) {
    bsp->mol = Seq_mol_dna;
  }

  vnp = ValNodeExtract (&(bsp->descr), Seq_descr_title);
  if (vnp != NULL) {
    ttl = (CharPtr) vnp->data.ptrvalue;
    if (ttl != NULL) {
      stp = SqnTagParse (ttl);
      if (stp != NULL) {
        biop = OldParseTitleIntoBioSource (stp, bsp, organism);
      }
      SqnTagFree (stp);
    }
    ValNodeFreeData (vnp);
  }
  if (biop == NULL) {
    biop = OldParseTitleIntoBioSource (NULL, bsp, organism);
  }
  if (biop != NULL) {
    SeqDescrAddPointer (&(bsp->descr), Seq_descr_source, (Pointer) biop);
  }

  mip = MolInfoNew ();
  if (mip != NULL) {
    mip->biomol = MOLECULE_TYPE_GENOMIC;
    SeqDescrAddPointer (&(bsp->descr), Seq_descr_molinfo, (Pointer) mip);
  }

  entityID = ObjMgrRegister (datatype, (Pointer) bsp);

  sep = GetBestTopParentForData (entityID, bsp);
  genCode = SeqEntryToGeneticCode (sep, NULL, NULL, 0);

  if (findorf) {
    AnnotateBestOrf (bsp, genCode, altstart);
  }

  sap = (SeqAnnotPtr) ReadOneFile (directory, base, "tbl", &datatype);
  if (sap != NULL && datatype == OBJ_SEQANNOT && sap->type == 1) {
    sap->next = bsp->annot;
    bsp->annot = sap;
  } else {
    ObjMgrFree (datatype, (Pointer) sap);
  }

  /* for parsed in features or best ORF, promote CDS products to protein bioseq */

  for (sap = bsp->annot; sap != NULL; sap = sap->next) {
    if (sap->type == 1) {
      SetEmptyGeneticCodes (sap, genCode);
      sfp = (SeqFeatPtr) sap->data;
      PromoteXrefs (sfp, bsp, entityID);
    }
  }

  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep != NULL) {
    SeriousSeqEntryCleanup (sep, NULL, NULL);
    WriteOneFile (directory, base, "sqn", sep);
    if (validate || flatfile) {
      SeqMgrIndexFeatures (entityID, 0);
    }
    if (validate) {
      Message (MSG_POST, "Validating %s\n", base);
      ValidateOneFile (directory, base, "val", sep);
    }
    if (flatfile) {
      Message (MSG_POST, "Flatfile %s\n", base);
      sep = FindNucSeqEntry (sep);
      FlatfileOneFile (directory, base, "gbf", sep);
    }
  }

  ObjMgrFreeByEntityID (entityID);
}

/* command-line argument list */

Args myargs [] = {
  {"Path to files", NULL, NULL, NULL,
    TRUE, 'p', ARG_STRING, 0.0, 0, NULL},
  {"Only this file", NULL, NULL, NULL,
    TRUE, 'f', ARG_STRING, 0.0, 0, NULL},
  {"Organism name", NULL, NULL, NULL,
    TRUE, 'n', ARG_STRING, 0.0, 0, NULL},
  {"Annotate longest ORF", "F", NULL, NULL,
    TRUE, 'r', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Allow alternative starts", "F", NULL, NULL,
    TRUE, 'a', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Validate", "F", NULL, NULL,
    TRUE, 'v', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Generate GenBank file", "F", NULL, NULL,
    TRUE, 'g', ARG_BOOLEAN, 0.0, 0, NULL}
};

Int2 Main (void)

{
  CharPtr     base, directory, organism, ptr;
  Boolean     altstart, findorf, flatfile, validate;
  ValNodePtr  head, vnp;

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
  if (! FeatDefSetLoad ()) {
    Message (MSG_FATAL, "FeatDefSetLoad failed");
    return 1;
  }

  if (! GetArgs ("sgd2asn", sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  directory = (CharPtr) myargs [0].strvalue;
  base = (CharPtr) myargs [1].strvalue;
  organism = (CharPtr) myargs [2].strvalue;
  findorf = (Boolean) myargs [3].intvalue;
  altstart = (Boolean) myargs [4].intvalue;
  validate = (Boolean) myargs [5].intvalue;
  flatfile = (Boolean) myargs [6].intvalue;

  if (! StringHasNoText (base)) {
    ptr = StringStr (base, ".fsa");
    if (ptr != NULL) {
      *ptr = '\0';
      ProcessOneRecord (directory, base, organism, findorf, altstart, validate, flatfile);
    }
  } else {
    head = DirCatalog (directory);
    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      if (vnp->choice == 0) {
        base = (CharPtr) vnp->data.ptrvalue;
        if (! StringHasNoText (base)) {
          ptr = StringStr (base, ".fsa");
          if (ptr != NULL) {
            *ptr = '\0';
            ProcessOneRecord (directory, base, organism, findorf, altstart, validate, flatfile);
          }
        }
      }
    }
    ValNodeFreeData (head);
  }

  return 0;
}

