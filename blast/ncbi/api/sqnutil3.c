/*   sqnutil3.c
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
* File Name:  sqnutil3.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   2/7/00
*
* $Revision: 6.55 $
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

#include <sqnutils.h>
#include <gather.h>
#include <subutil.h>
#include <objfdef.h>
#include <seqport.h>
#include <objproj.h>
#include <gbfeat.h>
#include <gbftdef.h>
#include <edutil.h>
#include <tofasta.h>
#include <parsegb.h>
#include <utilpars.h>
#include <validatr.h>
#include <explore.h>
#include <salsap.h>
#include <salutil.h>
#include <salpedit.h>
#include <alignmgr2.h>
#include <actutils.h>

/* general file recursion function */

NLM_EXTERN Int4 DirExplore (
  CharPtr directory,
  CharPtr filter,
  CharPtr suffix,
  DirExpProc proc,
  Pointer userdata
)

{
  Int4        count = 0;
  Char        file [FILENAME_MAX], path [PATH_MAX];
  CharPtr     ptr, str;
  ValNodePtr  head, vnp;

  if (proc == NULL) return 0;
  if (StringHasNoText (directory) || StringHasNoText (suffix)) return 0;

  /* get list of all files in source directory */

  head = DirCatalog (directory);

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == 0) {
      str = (CharPtr) vnp->data.ptrvalue;
      if (! StringHasNoText (str)) {

        /* check filename for indicated suffix */

        ptr = StringStr (str, suffix);
        if (ptr != NULL) {

          /* make sure detected suffix is really at end of filename */

          if (StringCmp (ptr, suffix) == 0) {
            *ptr = '\0';
          } else {
            ptr = NULL;
          }
        }

        if (StringHasNoText (suffix) || ptr != NULL) {

          StringNCpy_0 (path, directory, sizeof (path));
          sprintf (file, "%s%s", str, suffix);
          FileBuildPath (path, NULL, file);

          /* check full path/file name for desired filter */

          if (StringHasNoText (filter) || StringStr (path, filter) != NULL) {

            /* process file that satisfies optional filter and suffix constraints */

            proc (path, userdata);
            count++;
          }
        }
      }
    } else if (vnp->choice == 1) {

      /* recurse into subdirectory */

      StringNCpy_0 (path, directory, sizeof (path));
      str = (CharPtr) vnp->data.ptrvalue;
      FileBuildPath (path, str, NULL);

      count += DirExplore (path, filter, suffix, proc, userdata);
    }
  }

  /* clean up file list */

  ValNodeFreeData (head);

  return count;
}

/* CautiousSeqEntryCleanup section */

static Boolean EmptyOrNullString (CharPtr str)

{
  Char  ch;

  if (str == NULL) return TRUE;
  ch = *str;
  while (ch != '\0') {
    if (ch > ' ' && ch <= '~') return FALSE;
    str++;
    ch = *str;
  }
  return TRUE;
}

/* RemoveMultipleTitles currently removes FIRST title in chain */

static void RemoveMultipleTitles (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  SeqDescrPtr    descr = NULL;
  SeqDescrPtr    lasttitle = NULL;
  ObjValNodePtr  ovp;
  SeqDescrPtr    sdp;

  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp == NULL) return;
    descr = bsp->descr;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL) return;
    descr = bssp->descr;
  } else return;
  for (sdp = descr; sdp != NULL; sdp = sdp->next) {
    if (sdp->choice == Seq_descr_title) continue;
    if (lasttitle != NULL) {
      if (lasttitle->extended != 0) {
        ovp = (ObjValNodePtr) lasttitle;
        ovp->idx.deleteme = TRUE;
      }
      lasttitle = sdp;
    } else {
      lasttitle = sdp;
    }
  }
}

static void MakeBioSourceCopy (SeqEntryPtr sep, Pointer userdata)

{
  BioSourcePtr  biop;
  OrgRefPtr     master;
  OrgRefPtr     orp;
  SeqDescrPtr   sdp;

  master = (OrgRefPtr) userdata;
  sdp = SeqEntryGetSeqDescr (sep, Seq_descr_source, NULL);
  if (sdp != NULL) return;
  biop = BioSourceNew ();
  if (biop == NULL) return;
  orp = OrgRefNew ();
  if (orp == NULL) return;
  biop->org = orp;
  orp->taxname = StringSave (master->taxname);
  orp->common = StringSave (master->common);
  sdp = CreateNewDescriptor (sep, Seq_descr_source);
  if (sdp == NULL) return;
  sdp->data.ptrvalue = (Pointer) biop;
}

static void ReplicatePopPhyMutSetBioSource (SeqEntryPtr sep)

{
  BioSourcePtr   biop;
  BioseqSetPtr   bssp;
  OrgRefPtr      orp;
  ObjValNodePtr  ovp;
  SeqDescrPtr    sdp;

  if (sep == NULL) return;
  if (! IS_Bioseq_set (sep)) return;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp == NULL) return;
  if (bssp->_class == 7 ||
      (bssp->_class >= 13 && bssp->_class <= 16)) {
    sdp = SeqEntryGetSeqDescr (sep, Seq_descr_source, NULL);
    if (sdp == NULL) return;
    biop = (BioSourcePtr) sdp->data.ptrvalue;
    if (biop == NULL) return;
    orp = biop->org;
    if (orp == NULL) return;
    VisitElementsInSep (sep, (Pointer) orp, MakeBioSourceCopy);
    if (sdp->extended != 0) {
      ovp = (ObjValNodePtr) sdp;
      ovp->idx.deleteme = TRUE;
    }
  }
}

typedef struct featcount {
  Boolean     is_mRNA;
  BioseqPtr   bsp;
  Int2        numRNAs;
  SeqFeatPtr  gene;
  Int4        numGene;
  Int4        numCDS;
} FeatCount, PNTR FeatCountPtr;

static void CountGenesAndCDSs (SeqFeatPtr sfp, Pointer userdata)

{
  FeatCountPtr  fcp;

  fcp = (FeatCountPtr) userdata;
  if (sfp->data.choice == SEQFEAT_GENE) {
    (fcp->numGene)++;
    fcp->gene = sfp;
  } else if (sfp->data.choice == SEQFEAT_CDREGION) {
    (fcp->numCDS)++;
  }
}

static void LookForMrna (BioseqPtr bsp, Pointer userdata)

{
  FeatCountPtr  fcp;
  MolInfoPtr    mip;
  SeqDescrPtr   sdp;

  if (bsp == NULL || bsp->length == 0) return;
  if (! ISA_na (bsp->mol)) return;
  fcp = (FeatCountPtr) userdata;
  for (sdp = bsp->descr; sdp != NULL; sdp = sdp->next) {
    if (sdp->choice != Seq_descr_molinfo) continue;
    mip = (MolInfoPtr) sdp->data.ptrvalue;
    if (mip != NULL && mip->biomol == MOLECULE_TYPE_MRNA) {
      fcp->is_mRNA = TRUE;
      fcp->bsp = bsp;
      (fcp->numRNAs)++;
      return;
    }
  }
}

static void ExtendSingleGeneOnMRNA (SeqEntryPtr sep, Pointer userdata)

{
  FeatCount   fc;
  SeqIntPtr   sintp;
  ValNodePtr  vnp;

  fc.is_mRNA = FALSE;
  fc.bsp = NULL;
  fc.numRNAs = 0;
  VisitBioseqsInSep (sep, (Pointer) &fc, LookForMrna);
  if (! fc.is_mRNA) return;
  fc.gene = NULL;
  fc.numGene = 0;
  fc.numCDS = 0;
  VisitFeaturesInSep (sep, (Pointer) &fc, CountGenesAndCDSs);
  if (fc.numGene == 1 && fc.numCDS < 2 && fc.numRNAs == 1 &&
      fc.bsp != NULL && fc.gene != NULL) {
    if (fc.bsp != BioseqFindFromSeqLoc (fc.gene->location)) return;
    for (vnp = fc.gene->location; vnp != NULL; vnp = vnp->next) {
      if (vnp->choice != SEQLOC_INT) continue;
      sintp = (SeqIntPtr) vnp->data.ptrvalue;
      if (sintp == NULL) continue;
      if (sintp->from != 0 || sintp->to != fc.bsp->length - 1) {
        sintp->from = 0;
        sintp->to = fc.bsp->length - 1;
      }
    }
  }
}

static SeqFeatPtr BestCDS (SeqLocPtr loc, ValNodePtr cdslist)

{
  SeqFeatPtr  best_cds = NULL;
  SeqFeatPtr  cds;
  Int4        diff;
  Int4        min = INT4_MAX;
  ValNodePtr  vnp;

  if (loc == NULL || cdslist == NULL) return NULL;
  for (vnp = cdslist; vnp != NULL; vnp = vnp->next) {
    cds = (SeqFeatPtr) vnp->data.ptrvalue;
    diff = SeqLocAinB (loc, cds->location);
    if (diff >= 0) {
      if (diff < min) {
        min = diff;
        best_cds = cds;
      }
    }
  }
  return best_cds;
}

#define num_bond 5
static CharPtr feat_bond [num_bond] = {
  NULL,
  "disulfide bond",
  "thiolester bond",
  "xlink bond",
  "thioether bond"
};

#define num_site 27
static CharPtr feat_site [num_site] = {
  NULL, 
  "active", 
  "binding",
  "cleavage",
  "inhibit",
  "modifi",
  "glycosylation",
  "myristoylation",
  "mutagenized",
  "metal-binding",
  "phosphorylation",
  "acetylation",
  "amidation",
  "methylation",
  "hydroxylation",
  "sulfatation",
  "oxidative-deamination",
  "pyrrolidone-carboxylic-acid",
  "gamma-carboxyglutamic-acid",
  "blocked",
  "lipid-binding",
  "np-binding",
  "dna-binding",
  "signal-peptide",
  "transit-peptide",
  "transmembrane-region",
  "nitrosylation"
};

static Int2 FindStr (CharPtr PNTR array, Int2 array_num, CharPtr str)

{
  Int2 i;

  for (i = 0; i < array_num; i++) {
    if (array [i] == NULL) continue;
    if (StringNCmp (str, array [i], StringLen (array [i])) == 0) return i;
  }
  return -1;
}

static ValNodePtr remove_node (ValNodePtr head, ValNodePtr x)

{
  ValNodePtr  v, p;
  
  if (head == NULL) return NULL;
  if (x == head) {
    head = x->next;
    x->next = NULL;
    ValNodeFree (x);
    return head;
  }
  for (v = head; v != NULL && v != x; v = v->next) {
    p = v;
  }
  if (v != NULL) {
    p->next = x->next;
    x->next = NULL;
    ValNodeFree (x);
  }
  return head;
}

static SeqLocPtr fake_bond_loc (SeqLocPtr slp)

{
  SeqLocPtr loc, l, lnext, ldata;

  if (slp == NULL) return NULL;
  loc = MemNew (sizeof (SeqLoc));
  MemCopy (loc, slp, sizeof (SeqLoc));
  ldata = (SeqLocPtr) loc->data.ptrvalue;
  if (slp->choice != SEQLOC_MIX) return loc;
  for (l = ldata; l != NULL; l = lnext) {
    lnext = l->next;
    if (l->choice == SEQLOC_NULL) {
      ldata = remove_node (ldata, l);
    }
  }
  return loc;
}

static void ConvertImpFeatToProt (SeqFeatPtr feat, Pointer userdata)

{
  SeqFeatPtr  best_cds = NULL;
  Int2        bond = 0;
  BioseqPtr   bsp;
  ValNodePtr  cdslist;
  Uint1       choice = 0;
  Int4        frame;
  ImpFeatPtr  ifp;
  SeqLocPtr   loc;
  Uint1       processed = 0;
  ProtRefPtr  prp;
  SeqFeatPtr  sfp;
  SeqIdPtr    sip;
  Int2        site = 0;
  SeqLocPtr   slp;
  Uint1       subtype = 0;

  if (feat == NULL || feat->data.choice != SEQFEAT_IMP) return;
  ifp = (ImpFeatPtr) feat->data.value.ptrvalue;
  if (ifp == NULL) return;
  cdslist = (ValNodePtr) userdata;
  if (StringCmp (ifp->key, "mat_peptide") == 0) {
    processed = 2;
    choice = SEQFEAT_PROT;
    subtype = FEATDEF_mat_peptide_aa;
  } else if (StringCmp (ifp->key, "sig_peptide") == 0) {
    processed = 3;
    choice = SEQFEAT_PROT;
    subtype = FEATDEF_sig_peptide_aa;
  } else if (StringCmp (ifp->key, "transit_peptide") == 0) {
    processed = 4;
    choice = SEQFEAT_PROT;
    subtype = FEATDEF_transit_peptide_aa;
  } else if (StringCmp (ifp->key, "misc_feature") == 0 && feat->comment != NULL) {
    site = FindStr (feat_site, num_site, feat->comment);
    if (site != -1) {
      choice = SEQFEAT_SITE;
      subtype = FEATDEF_SITE;
    } else {
      bond = FindStr (feat_bond, num_bond, feat->comment);
      if (bond != -1) {
        choice = SEQFEAT_BOND;
        subtype = FEATDEF_BOND;
      }
    }
  }
  if (choice == 0) return;

  if (processed != 0 || site != 0) {
    best_cds = BestCDS (feat->location, cdslist);
  } else if (bond != 0) {
    loc = fake_bond_loc (feat->location);
    best_cds = BestCDS (loc, cdslist);
    SeqLocFree (loc);
  }
  if (best_cds == NULL) return;
  slp = dnaLoc_to_aaLoc (best_cds, feat->location, TRUE, &frame, FALSE);
  if (slp == NULL) return;
  sip = SeqLocId (best_cds->product);
  if (sip == NULL) return;
  bsp = BioseqLockById (sip);
  if (bsp == NULL) return;
  sfp = CreateNewFeatureOnBioseq (bsp, choice, slp);
  BioseqUnlock (bsp);
  if (sfp == NULL) return;

  sfp->partial = feat->partial;
  sfp->excpt = feat->excpt;
  sfp->exp_ev = feat->exp_ev;
  sfp->pseudo = feat->pseudo;

  sfp->comment = feat->comment;
  feat->comment = NULL;
  sfp->qual = feat->qual;
  feat->qual = NULL;
  sfp->title = feat->title;
  feat->title = NULL;
  sfp->ext = feat->ext;
  feat->ext = NULL;
  sfp->cit = feat->cit;
  feat->cit = NULL;

  sfp->xref = feat->xref;
  feat->xref = NULL;
  sfp->dbxref = feat->dbxref;
  feat->dbxref = NULL;
  sfp->except_text = feat->except_text;
  feat->except_text = NULL;

  if (choice == SEQFEAT_PROT) {
    prp = ProtRefNew ();
    sfp->data.value.ptrvalue = (Pointer) prp;
    if (prp != NULL) {
      prp->processed = processed;
    }
    switch (processed) {
    }
  } else if (choice == SEQFEAT_SITE) {
    sfp->data.value.intvalue = site;
  } else if (choice == SEQFEAT_BOND) {
    sfp->data.value.intvalue = bond;
  }
  sfp->idx.subtype = subtype;

  feat->idx.deleteme = TRUE;
}

static void GetListOfCDSs (SeqFeatPtr sfp, Pointer userdata)

{
  ValNodePtr PNTR  head;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION) return;
  head = (ValNodePtr PNTR) userdata;
  ValNodeAddPointer (head, 0, sfp->data.value.ptrvalue);
}

static void ChangeImpFeatToProt (SeqEntryPtr sep)

{
  ValNodePtr  cdslist = NULL;

  VisitFeaturesInSep (sep, (Pointer) &cdslist, GetListOfCDSs);
  VisitFeaturesInSep (sep, (Pointer) cdslist, ConvertImpFeatToProt);
  ValNodeFree (cdslist);
}

static void MergeAdjacentAnnotsCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  SeqAnnotPtr   nextsap;
  SeqAnnotPtr   sap;
  SeqFeatPtr    sfp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  while (sap != NULL) {
    nextsap = sap->next;
    if (sap->type == 1 && nextsap != NULL && nextsap->type == 1) {
      if (sap->id == NULL && nextsap->id == NULL &&
          sap->name == NULL && nextsap->name == NULL &&
          sap->db == 0 && nextsap->db == 0 &&
          sap->desc == NULL && nextsap->desc == NULL &&
          sap->data != NULL && nextsap->data != NULL) {
        sfp = (SeqFeatPtr) sap->data;
        while (sfp->next != NULL) {
          sfp = sfp->next;
        }
        sfp->next = (SeqFeatPtr) nextsap->data;
        nextsap->data = NULL;
        sap->next = nextsap->next;
        SeqAnnotFree (nextsap);
        nextsap = sap->next;
      }
    }
    sap = nextsap;
  }
}

static Boolean empty_citgen (CitGenPtr  cit)

{
  if (cit == NULL) return TRUE;
  if (cit->cit) return FALSE;
  if (cit->authors) return FALSE;
  if (cit->muid > 0) return FALSE;
  if (cit->journal) return FALSE;
  if (cit->volume) return FALSE;
  if (cit->issue) return FALSE;
  if (cit->pages) return FALSE;
  if (cit->date) return FALSE;
  if (cit->serial_number > 0) return FALSE;
  if (cit->title) return FALSE;
  if (cit->pmid > 0) return FALSE;
  return TRUE;
}

static Boolean PubIsEffectivelyEmpty (PubdescPtr pdp)

{
  ValNodePtr  vnp;

  if (pdp == NULL) return FALSE;
  vnp = pdp->pub;
  if (vnp != NULL && vnp->next == NULL && vnp->choice == PUB_Gen) {
    if (empty_citgen ((CitGenPtr) vnp->data.ptrvalue)) {
      return TRUE;
    }
  }
  return FALSE;
}

static void MarkEmptyDescsForCleanup (SeqDescrPtr sdp, Pointer userdata)

{
  GBBlockPtr     gbp;
  ObjValNodePtr  ovp;
  PubdescPtr     pdp;
  CharPtr        str;

  if (sdp == NULL || sdp->extended == 0) return;
  ovp = (ObjValNodePtr) sdp;
  if (sdp->choice == Seq_descr_title) {
    str = (CharPtr) sdp->data.ptrvalue;
    if (StringHasNoText (str)) {
      ovp->idx.deleteme = TRUE;
    }
  } else if (sdp->choice == Seq_descr_pub) {
    pdp = (PubdescPtr) sdp->data.ptrvalue;
    if (pdp == NULL) return;
    if (PubIsEffectivelyEmpty (pdp)) {
      ovp->idx.deleteme = TRUE;
    }
  } else if (sdp->choice == Seq_descr_genbank) {
    gbp = (GBBlockPtr) sdp->data.ptrvalue;
    if (gbp == NULL) return;
    /* gbp->source = MemFree (gbp->source); */
    /* gbp->origin = MemFree (gbp->origin); */
    gbp->taxonomy = MemFree (gbp->taxonomy);
    if (gbp->extra_accessions == NULL && gbp->source == NULL &&
        gbp->keywords == NULL && gbp->origin == NULL &&
        gbp->date == NULL && gbp->entry_date == NULL &&
        gbp->div == NULL && gbp->taxonomy == NULL) {
      ovp->idx.deleteme = TRUE;
    }
  }
}

static void MarkEmptyFeatsForCleanup (SeqFeatPtr sfp, Pointer userdata)

{
  GeneRefPtr  grp;
  PubdescPtr  pdp;
  ProtRefPtr  prp;
  ValNodePtr  vnp;

  if (sfp == NULL) return;
  if (sfp->data.choice == SEQFEAT_GENE && sfp->data.value.ptrvalue != NULL) {
    grp = (GeneRefPtr) sfp->data.value.ptrvalue;
    if (EmptyOrNullString (grp->locus)) {
      grp->locus = MemFree (grp->locus);
    }
    if (EmptyOrNullString (grp->allele)) {
      grp->allele = MemFree (grp->allele);
    }
    if (EmptyOrNullString (grp->desc)) {
      grp->desc = MemFree (grp->desc);
    }
    if (EmptyOrNullString (grp->maploc)) {
      grp->maploc = MemFree (grp->maploc);
    }
    if (EmptyOrNullString (grp->locus_tag)) {
      grp->locus_tag = MemFree (grp->locus_tag);
    }
    if (EmptyOrNullString (grp->locus) &&
        EmptyOrNullString (grp->allele) &&
        EmptyOrNullString (grp->desc) &&
        EmptyOrNullString (grp->maploc) &&
        EmptyOrNullString (grp->locus_tag) &&
        grp->db == NULL && grp->syn == NULL) {
      sfp->idx.deleteme = TRUE;
    }
  } else if (sfp->data.choice == SEQFEAT_PROT && sfp->data.value.ptrvalue != NULL) {
    prp = (ProtRefPtr) sfp->data.value.ptrvalue;
    if (prp->processed != 3 && prp->processed != 4) {
      vnp = prp->name;
      if ((vnp == NULL || EmptyOrNullString ((CharPtr) vnp->data.ptrvalue)) &&
          EmptyOrNullString (prp->desc) &&
          prp->ec == NULL && prp->activity == NULL && prp->db == NULL) {
        sfp->idx.deleteme = TRUE;
      }
    }
  } else if (sfp->data.choice == SEQFEAT_PUB && sfp->data.value.ptrvalue != NULL) {
    pdp = (PubdescPtr) sfp->data.value.ptrvalue;
    if (PubIsEffectivelyEmpty (pdp)) {
      sfp->idx.deleteme = TRUE;
    }
  } else if (sfp->data.choice == SEQFEAT_COMMENT && EmptyOrNullString (sfp->comment)) {
    sfp->idx.deleteme = TRUE;
  }
}

static void ConvertPubFeatDescProc (SeqFeatPtr sfp, Pointer userdata)

{
  BioseqPtr      bsp;
  size_t         len;
  ObjValNodePtr  ovp;
  PubdescPtr     pdp;
  SeqDescPtr     sdp;
  SeqEntryPtr    sep;
  SeqIdPtr       sip;
  CharPtr        str;
  ValNode        vn;

  /* look for publication features */
  if (sfp == NULL || sfp->data.choice != SEQFEAT_PUB) return;
  /* get bioseq by feature location */
  sip = SeqLocId (sfp->location);
  bsp = BioseqFind (sip);
  if (bsp == NULL) return;
  sip = SeqIdFindBest(bsp->id, 0);
  if (sip == NULL) return;
  vn.choice = SEQLOC_WHOLE;
  vn.extended = 0;
  vn.data.ptrvalue = (Pointer) sip;
  vn.next = NULL;
  /* is feature full length? */
  if (SeqLocCompare (sfp->location, &vn) != SLC_A_EQ_B) return;
  sep = SeqMgrGetSeqEntryForData (bsp);
  if (sep == NULL) return;
  sdp = CreateNewDescriptor (sep, Seq_descr_pub);
  if (sdp == NULL) return;
  /* move publication from feature to descriptor */
  sdp->data.ptrvalue = sfp->data.value.ptrvalue;
  if (sdp->extended != 0) {
    ovp = (ObjValNodePtr) sdp;
    ovp->idx.subtype = Seq_descr_pub;
  }
  sfp->data.value.ptrvalue = NULL;
  /* flag old feature for removal */
  sfp->idx.deleteme = TRUE;
  /* move comment to remark */
  if (sfp->comment == NULL) return;
  pdp = (PubdescPtr) sdp->data.ptrvalue;
  if (pdp == NULL) return;
  if (pdp->comment == NULL) {
    pdp->comment = sfp->comment;
    sfp->comment = NULL;
  } else {
    len = StringLen (pdp->comment) + StringLen (sfp->comment) + 5;
    str = MemNew (sizeof (Char) * len);
    StringCpy (str, pdp->comment);
    StringCat (str, "; ");
    StringCat (str, sfp->comment);
    pdp->comment = MemFree (pdp->comment);
    pdp->comment = str;
  }
}

static void ConvertSourceFeatDescProc (SeqFeatPtr sfp, Pointer userdata)

{
  BioSourcePtr   biop;
  BioseqPtr      bsp;
  SubSourcePtr   lastssp;
  ObjValNodePtr  ovp;
  SeqDescPtr     sdp;
  SeqEntryPtr    sep;
  SeqIdPtr       sip;
  SubSourcePtr   ssp;
  ValNode        vn;

  /* look for biosource features */
  if (sfp == NULL || sfp->data.choice != SEQFEAT_BIOSRC) return;
  /* get bioseq by feature location */
  sip = SeqLocId (sfp->location);
  bsp = BioseqFind (sip);
  if (bsp == NULL) return;
  sip = SeqIdFindBest(bsp->id, 0);
  if (sip == NULL) return;
  vn.choice = SEQLOC_WHOLE;
  vn.extended = 0;
  vn.data.ptrvalue = (Pointer) sip;
  vn.next = NULL;
  /* is feature full length? */
  if (SeqLocCompare (sfp->location, &vn) != SLC_A_EQ_B) return;
  sep = SeqMgrGetSeqEntryForData (bsp);
  if (sep == NULL) return;
  sdp = CreateNewDescriptor (sep, Seq_descr_source);
  if (sdp == NULL) return;
  /* move biosource from feature to descriptor */
  sdp->data.ptrvalue = sfp->data.value.ptrvalue;
  if (sdp->extended != 0) {
    ovp = (ObjValNodePtr) sdp;
    ovp->idx.subtype = Seq_descr_source;
  }
  sfp->data.value.ptrvalue = NULL;
  /* flag old feature for removal */
  sfp->idx.deleteme = TRUE;
  /* move comment to subsource note */
  if (sfp->comment == NULL) return;
  biop = (BioSourcePtr) sdp->data.ptrvalue;
  if (biop == NULL) return;
  ssp = SubSourceNew ();
  if (ssp == NULL) return;
  ssp->subtype = SUBSRC_other;
  ssp->name = sfp->comment;
  sfp->comment = NULL;
  /* link in at end, since BasicSeqEntry will have sorted this list */
  if (biop->subtype == NULL) {
    biop->subtype = ssp;
  } else {
    lastssp = biop->subtype;
    while (lastssp->next != NULL) {
      lastssp = lastssp->next;
    }
    lastssp->next = ssp;
  }
}

static void PromoteOrgRefDescToBioSource (SeqDescrPtr sdp, Pointer userdata)

{
  BioSourcePtr   biop;
  OrgRefPtr      orp;
  ObjValNodePtr  ovp;

  if (sdp->choice != Seq_descr_org) return;
  orp = (OrgRefPtr) sdp->data.ptrvalue;
  if (orp == NULL) return;
  biop = BioSourceNew ();
  if (biop == NULL) return;
  biop->org = orp;
  sdp->choice = Seq_descr_source;
  sdp->data.ptrvalue = (Pointer) biop;
  if (sdp->extended != 0) {
    ovp = (ObjValNodePtr) sdp;
    ovp->idx.subtype = Seq_descr_source;
  }
}

static void PromoteOrgRefFeatToBioSource (SeqFeatPtr sfp, Pointer userdata)

{
  BioSourcePtr  biop;
  OrgRefPtr     orp;

  if (sfp->data.choice != SEQFEAT_ORG) return;
  orp = (OrgRefPtr) sfp->data.value.ptrvalue;
  if (orp == NULL) return;
  biop = BioSourceNew ();
  if (biop == NULL) return;
  biop->org = orp;
  sfp->data.choice = SEQFEAT_BIOSRC;
  sfp->data.value.ptrvalue = (Pointer) biop;
  sfp->idx.subtype = FEATDEF_BIOSRC;
}

static void DeleteBadMarkedGeneXrefs (SeqFeatPtr sfp, Pointer userdata)

{
  SeqFeatXrefPtr       nextxref;
  SeqFeatXrefPtr PNTR  prevxref;
  Boolean              unlink;
  SeqFeatXrefPtr       xref;

  if (sfp == NULL) return;
  xref = sfp->xref;
  prevxref = (SeqFeatXrefPtr PNTR) &(sfp->xref);
  while (xref != NULL) {
    nextxref = xref->next;
    unlink = FALSE;
    if (xref->specialCleanupFlag && xref->data.choice == SEQFEAT_GENE) {
      if (SeqMgrGetOverlappingGene (sfp->location, NULL) != NULL) {
        unlink = TRUE;
      }
    }
    xref->specialCleanupFlag = FALSE;
    if (unlink) {
      *(prevxref) = xref->next;
      xref->next = NULL;
      SeqFeatXrefFree (xref);
    } else {
      prevxref = (SeqFeatXrefPtr PNTR) &(xref->next);
    }
    xref = nextxref;
  }
}

static void LookForMarkedGeneXrefs (SeqFeatPtr sfp, Pointer userdata)

{
  BoolPtr         hasMarkedGenes;
  SeqFeatXrefPtr  xref;

  if (sfp == NULL || sfp->xref == NULL) return;
  for (xref = sfp->xref; xref != NULL; xref = xref->next) {
    if (xref->specialCleanupFlag) {
      hasMarkedGenes = (BoolPtr) userdata;
      *hasMarkedGenes = TRUE;
      return;
    }
  }
}

NLM_EXTERN void CautiousSeqEntryCleanup (SeqEntryPtr sep, SeqEntryFunc taxfun, SeqEntryFunc taxmerge)

{
  Boolean      correct = FALSE;
  Uint2        entityID;
  Boolean      hasMarkedGenes;
  ErrSev       lsev;
  ErrSev       msev;
  SeqEntryPtr  oldscope;
  Boolean      strip = TRUE;
  Boolean      taxserver;

  if (sep == NULL) return;
  msev = ErrSetMessageLevel (SEV_MAX);
  lsev = ErrSetLogLevel (SEV_MAX);
  entityID = SeqMgrGetEntityIDForSeqEntry (sep);

  BasicSeqEntryCleanup (sep);

  VisitFeaturesInSep (sep, NULL, PromoteOrgRefFeatToBioSource);
  VisitDescriptorsInSep (sep, NULL, PromoteOrgRefDescToBioSource);

  oldscope = SeqEntrySetScope (sep);
  VisitFeaturesInSep (sep, NULL, ConvertSourceFeatDescProc);
  VisitFeaturesInSep (sep, NULL, ConvertPubFeatDescProc);
  SeqEntrySetScope (oldscope);

  VisitFeaturesInSep (sep, NULL, MarkEmptyFeatsForCleanup);
  VisitDescriptorsInSep (sep, NULL, MarkEmptyDescsForCleanup);
  DeleteMarkedObjects (0, OBJ_SEQENTRY, (Pointer) sep);

  SeqEntryExplore (sep, NULL, MergeAdjacentAnnotsCallback);

  ChangeImpFeatToProt (sep);
  DeleteMarkedObjects (0, OBJ_SEQENTRY, (Pointer) sep);

  VisitElementsInSep (sep, NULL, ExtendSingleGeneOnMRNA);

  ReplicatePopPhyMutSetBioSource (sep);
  SeqEntryExplore (sep, NULL, RemoveMultipleTitles);

  /* LoopSeqEntryToAsn3 section here */
  taxserver = (Boolean) (taxfun != NULL || taxmerge != NULL);

  /*
  if (correct) {
    SeqEntryExplore(sep, (Pointer)(&porg), CorrectSourceFeat);
  }
  */







  /* a few more things to do here */

  hasMarkedGenes = FALSE;
  VisitFeaturesInSep (sep, (Pointer) &hasMarkedGenes, LookForMarkedGeneXrefs);
  if (hasMarkedGenes) {
    SeqMgrIndexFeatures (entityID, NULL);
    VisitFeaturesInSep (sep, NULL, DeleteBadMarkedGeneXrefs);
    SeqMgrClearFeatureIndexes (entityID, NULL);
  }

  BasicSeqEntryCleanup (sep);

  AssignIDsInEntity (entityID, 0, NULL);

  ErrSetMessageLevel (msev);
  ErrSetLogLevel (lsev);
}

/*
static Int4 LoopSeqEntryToAsn3 (SeqEntryPtr sep, Boolean strip, Boolean correct, SeqEntryFunc taxfun, SeqEntryFunc taxmerge)

{
  BioseqSetPtr  bssp;
  SeqEntryPtr   oldscope;
  Int4          rsult;
  Boolean       taxserver;

  rsult = 0;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 || bssp->_class == 13 ||
                         bssp->_class == 14 || bssp->_class == 15)) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        rsult += LoopSeqEntryToAsn3 (sep, strip, correct, taxfun, taxmerge);
      }
      return rsult;
    }
  }
  oldscope = SeqEntrySetScope (sep);
  taxserver = (Boolean) (taxfun != NULL || taxmerge != NULL);
  rsult = SeqEntryToAsn3Ex (sep, strip, correct, taxserver, taxfun, taxmerge);
  SeqEntrySetScope (oldscope);
  return rsult;
}
  LoopSeqEntryToAsn3 (sep, TRUE, FALSE, taxfun, taxmerge);

*/

typedef struct featdefNameStruct {
  Uint1    type;
  CharPtr  name;
} FeatdefNameData, PNTR FeatdefNamePtr;

static FeatdefNameData featdefWithName [] = {
  { FEATDEF_10_signal ,          "-10_signal"         },
  { FEATDEF_35_signal ,          "-35_signal"         },
  { FEATDEF_3clip ,              "3'clip"             },
  { FEATDEF_3UTR ,               "3'UTR"              },
  { FEATDEF_5clip ,              "5'clip"             },
  { FEATDEF_5UTR ,               "5'UTR"              },
  { FEATDEF_attenuator ,         "attenuator"         },
  { FEATDEF_BOND ,               "Bond"               },
  { FEATDEF_CAAT_signal ,        "CAAT_signal"        },
  { FEATDEF_CDS ,                "CDS"                },
  { FEATDEF_PUB ,                "Cit"                },
  { FEATDEF_COMMENT ,            "Comment"            },
  { FEATDEF_conflict ,           "conflict"           },
  { FEATDEF_C_region ,           "C_region"           },
  { FEATDEF_D_loop ,             "D-loop"             },
  { FEATDEF_D_segment ,          "D_segment"          },
  { FEATDEF_enhancer ,           "enhancer"           },
  { FEATDEF_exon ,               "exon"               },
  { FEATDEF_gap ,                "gap"                },
  { FEATDEF_GC_signal ,          "GC_signal"          },
  { FEATDEF_GENE ,               "Gene"               },
  { FEATDEF_HET ,                "Het"                },
  { FEATDEF_iDNA ,               "iDNA"               },
  { FEATDEF_IMP ,                "Import"             },
  { FEATDEF_Imp_CDS ,            "Imp_CDS"            },
  { FEATDEF_intron ,             "intron"             },
  { FEATDEF_J_segment ,          "J_segment"          },
  { FEATDEF_LTR ,                "LTR"                },
  { FEATDEF_mat_peptide_aa ,     "mat_peptide"        },
  { FEATDEF_mat_peptide ,        "mat_peptide_nt"     },
  { FEATDEF_misc_binding ,       "misc_binding"       },
  { FEATDEF_misc_difference ,    "misc_difference"    },
  { FEATDEF_misc_feature ,       "misc_feature"       },
  { FEATDEF_misc_recomb ,        "misc_recomb"        },
  { FEATDEF_otherRNA ,           "misc_RNA"           },
  { FEATDEF_misc_signal ,        "misc_signal"        },
  { FEATDEF_misc_structure ,     "misc_structure"     },
  { FEATDEF_modified_base ,      "modified_base"      },
  { FEATDEF_mRNA ,               "mRNA"               },
  { FEATDEF_NON_STD_RESIDUE ,    "NonStdRes"          },
  { FEATDEF_NUM ,                "Num"                },
  { FEATDEF_N_region ,           "N_region"           },
  { FEATDEF_old_sequence ,       "old_sequence"       },
  { FEATDEF_operon ,             "operon"             },
  { FEATDEF_oriT ,               "oriT"               },
  { FEATDEF_polyA_signal ,       "polyA_signal"       },
  { FEATDEF_polyA_site ,         "polyA_site"         },
  { FEATDEF_preRNA ,             "precursor_RNA"      },
  { FEATDEF_preprotein ,         "preprotein"         },
  { FEATDEF_primer_bind ,        "primer_bind"        },
  { FEATDEF_prim_transcript ,    "prim_transcript"    },
  { FEATDEF_promoter ,           "promoter"           },
  { FEATDEF_PROT ,               "Protein"            },
  { FEATDEF_protein_bind ,       "protein_bind"       },
  { FEATDEF_RBS ,                "RBS"                },
  { FEATDEF_REGION ,             "Region"             },
  { FEATDEF_repeat_region ,      "repeat_region"      },
  { FEATDEF_repeat_unit ,        "repeat_unit"        },
  { FEATDEF_rep_origin ,         "rep_origin"         },
  { FEATDEF_rRNA ,               "rRNA"               },
  { FEATDEF_RSITE ,              "Rsite"              },
  { FEATDEF_satellite ,          "satellite"          },
  { FEATDEF_scRNA ,              "scRNA"              },
  { FEATDEF_PSEC_STR ,           "SecStr"             },
  { FEATDEF_sig_peptide_aa ,     "sig_peptide"        },
  { FEATDEF_sig_peptide ,        "sig_peptide_nt"     },
  { FEATDEF_SITE ,               "Site"               },
  { FEATDEF_site_ref ,           "Site-ref"           },
  { FEATDEF_snoRNA ,             "snoRNA"             },
  { FEATDEF_snRNA ,              "snRNA"              },
  { FEATDEF_source ,             "source"             },
  { FEATDEF_BIOSRC ,             "Src"                },
  { FEATDEF_stem_loop ,          "stem_loop"          },
  { FEATDEF_STS ,                "STS"                },
  { FEATDEF_S_region ,           "S_region"           },
  { FEATDEF_TATA_signal ,        "TATA_signal"        },
  { FEATDEF_terminator ,         "terminator"         },
  { FEATDEF_transit_peptide_aa , "transit_peptide"    },
  { FEATDEF_transit_peptide ,    "transit_peptide_nt" },
  { FEATDEF_tRNA ,               "tRNA"               },
  { FEATDEF_TXINIT ,             "TxInit"             },
  { FEATDEF_unsure ,             "unsure"             },
  { FEATDEF_USER ,               "User"               },
  { FEATDEF_variation ,          "variation"          },
  { FEATDEF_virion ,             "virion"             },
  { FEATDEF_V_region ,           "V_region"           },
  { FEATDEF_V_segment ,          "V_segment"          },
  { FEATDEF_SEQ ,                "Xref"               }
};

NLM_EXTERN Uint1 FindFeatDefTypeFromKey (CharPtr key)

{
  Int2  L, R, mid;

  if (key == NULL || *key == '\0') return FEATDEF_BAD;

  L = 0;
  R = (sizeof (featdefWithName) / sizeof (FeatdefNameData)) - 1;

  while (L < R) {
    mid = (L + R) / 2;
    if (StringICmp (featdefWithName [mid].name, key) < 0) {
      L = mid + 1;
    } else {
      R = mid;
    }
  }

  if (StringICmp (featdefWithName [R].name, key) == 0) {
    return featdefWithName [R].type;
  }

  return FEATDEF_BAD;
}

static CharPtr featurekeys [] = {
  "???" ,
  "Gene" ,
  "Org" ,
  "CDS" ,
  "Protein" ,
  "precursor_RNA" ,
  "mRNA" ,
  "tRNA" ,
  "rRNA" ,
  "snRNA" ,
  "scRNA" ,
  "misc_RNA" ,
  "Cit" ,
  "Xref" ,
  "Import" ,
  "allele" ,
  "attenuator" ,
  "C_region" ,
  "CAAT_signal" ,
  "CDS" ,
  "conflict" ,
  "D-loop" ,
  "D_segment" ,
  "enhancer" ,
  "exon" ,
  "GC_signal" ,
  "iDNA" ,
  "intron" ,
  "J_segment" ,
  "LTR" ,
  "mat_peptide" ,
  "misc_binding" ,
  "misc_difference" ,
  "misc_feature" ,
  "misc_recomb" ,
  "misc_RNA" ,
  "misc_signal" ,
  "misc_structure" ,
  "modified_base" ,
  "mutation" ,
  "N_region" ,
  "old_sequence" ,
  "polyA_signal" ,
  "polyA_site" ,
  "precursor_RNA" ,
  "prim_transcript" ,
  "primer_bind" ,
  "promoter" ,
  "protein_bind" ,
  "RBS" ,
  "repeat_region" ,
  "repeat_unit" ,
  "rep_origin" ,
  "S_region" ,
  "satellite" ,
  "sig_peptide" ,
  "source" ,
  "stem_loop" ,
  "STS" ,
  "TATA_signal" ,
  "terminator" ,
  "transit_peptide" ,
  "unsure" ,
  "V_region" ,
  "V_segment" ,
  "variation" ,
  "virion" ,
  "3'clip" ,
  "3'UTR" ,
  "5'clip" ,
  "5'UTR" ,
  "-10_signal" ,
  "-35_signal" ,
  "Site-ref" ,
  "Region" ,
  "Comment" ,
  "Bond" ,
  "Site" ,
  "Rsite" ,
  "User" ,
  "TxInit" ,
  "Num" ,
  "SecStr" ,
  "NonStdRes" ,
  "Het" ,
  "Src" ,
  "proprotein" ,
  "mat_peptide" ,
  "sig_peptide" ,
  "transit_peptide",
  "snoRNA",
  "gap",
  "operon",
  "oriT"
};

NLM_EXTERN CharPtr FindKeyFromFeatDefType (Uint1 type, Boolean forGBFF)

{
  CharPtr  key;

  if (type < FEATDEF_GENE || type >= FEATDEF_MAX) {
    type = FEATDEF_BAD;
  }
  key = featurekeys [type];

  if (forGBFF) {
    if (type == FEATDEF_GENE) {
      key = "gene";
    } else if (type == FEATDEF_REGION ||
               type == FEATDEF_COMMENT ||
               type == FEATDEF_BOND ||
               type == FEATDEF_SITE) {
      key = "misc_feature";
    }
  }

  return key;
}

/* tRNA codon index to codon string lookup table functions */

typedef struct gcCodonStruct {
  Uint1    index;
  CharPtr  codon;
} GcCodonData, PNTR GcCodonPtr;

static CharPtr    gcCodonStrings = NULL;
static GcCodonPtr codonGcIndex = NULL;

/* mapping from NCBI2na to codon codes */

static Uint1 codon_xref [4] = {
  2,  /* A */
  1,  /* C */
  3,  /* G */
  0   /* T */
};

static int LIBCALLBACK SortCodonByString (
  VoidPtr vp1,
  VoidPtr vp2
)

{
  int         compare;
  GcCodonPtr  gcp1 = vp1;
  GcCodonPtr  gcp2 = vp2;

  if (gcp1 == NULL || gcp2 == NULL) return 0;

  compare = StringICmp (gcp1->codon, gcp2->codon);
  if (compare > 0) {
    return 1;
  } else if (compare < 0) {
    return -1;
  }

  return 0;
}

static void InitGcCodons (void)

{
  Uint1           codon [4], index;
  GcCodonPtr      codonGcIdx;
  CharPtr         gcCodonStr;
  Int2            i, j, k;
  int             idx, offset;
  CharPtr         ptr;
  Uint1           residue;
  SeqMapTablePtr  smtp;

  if (codonGcIndex != NULL && gcCodonStrings != NULL) return;

  gcCodonStr = (CharPtr) MemNew (sizeof (Char) * 256);
  if (gcCodonStr == NULL) return;
  codonGcIdx = (GcCodonPtr) MemNew (sizeof (GcCodonData) * 64);
  if (codonGcIdx == NULL) return;

  smtp = SeqMapTableFind (Seq_code_iupacna, Seq_code_ncbi2na);
  if (smtp == NULL) return;

  for (idx = 0; idx < 64; idx++) {
    index = (Uint1) idx;

    for (i = 0, j = 16; i < 3; i++, j /= 4) {
      residue = (Uint1) ((Int2) index / j);
      index -= (Uint1) (residue * j);
      for (k = 0; k < 4; k++) {
        if (codon_xref [k] == residue) {
          residue = (Uint1) k;
          break;
        }
      }
      residue = SeqMapTableConvert (smtp, residue);
      codon [i] = residue;
    }
    codon [3] = 0;

    offset = 4 * idx;
    ptr = gcCodonStr + offset;
    StringCpy (ptr, (CharPtr) codon);

    codonGcIdx [idx].index = (Uint1) idx;
    codonGcIdx [idx].codon = ptr;
  }

  HeapSort (codonGcIdx, (size_t) 64, sizeof (GcCodonData), SortCodonByString);

  gcCodonStrings = gcCodonStr;
  codonGcIndex = codonGcIdx;
}

NLM_EXTERN Uint1 CodonToGcIndex (CharPtr codon)

{
  Char  ch;
  Int2  i, L, R, mid;
  Char  tmp [4];

  if (codonGcIndex == NULL) {
    InitGcCodons ();
  }
  if (codonGcIndex == NULL) return 255;
  if (StringLen (codon) != 3) return 255;
  StringNCpy_0 (tmp, codon, sizeof (tmp));

  for (i = 0; i < 3; i++) {
    ch = tmp [i];
    ch = TO_UPPER (ch);
    if (ch == 'U') {
       ch = 'T';
    }
    tmp [i] = ch;
  }

  L = 0;
  R = 63;

  while (L < R) {
    mid = (L + R) / 2;
    if (StringICmp (codonGcIndex [mid].codon, tmp) < 0) {
      L = mid + 1;
    } else {
      R = mid;
    }
  }

  if (StringICmp (codonGcIndex [R].codon, tmp) == 0) {
    return codonGcIndex [R].index;
  }

  return 255;
}

NLM_EXTERN CharPtr GcIndextoCodon (Uint1 index)

{
  int      offset;
  CharPtr  ptr;

  if (gcCodonStrings == NULL) {
    InitGcCodons ();
  }
  if (gcCodonStrings == NULL) return NULL;
  if (index > 63) return NULL;

  offset = 4 * index;
  ptr = gcCodonStrings + offset;

  return ptr;
}

static FloatHi GetCddBitScore (SeqFeatPtr sfp)

{
  ObjectIdPtr    oip;
  UserFieldPtr   ufp;
  UserObjectPtr  uop;

  if (sfp == NULL) return 0.0;
  uop = sfp->ext;
  if (uop == NULL) return 0.0;
  oip = uop->type;
  if (oip == NULL || StringICmp (oip->str, "cddScoreData") != 0) return 0.0;
  for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
    oip = ufp->label;
    if (oip != NULL && StringICmp (oip->str, "bit_score") == 0) {
      if (ufp->choice == 3) {
        return ufp->data.realvalue;
      }
    }
  }
  return 0.0;
}

static Boolean FeatIsCDD (
  SeqFeatPtr sfp,
  FloatHi PNTR scoreP
)

{
  DbtagPtr    dbt;
  ValNodePtr  vnp;

  if (scoreP != NULL) {
    *scoreP = 0.0;
  }
  for (vnp = sfp->dbxref; vnp != NULL; vnp = vnp->next) {
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    if (dbt != NULL && StringCmp (dbt->db, "CDD") == 0) {
      if (scoreP != NULL) {
        *scoreP = GetCddBitScore (sfp);
      }
      return TRUE;
    }
  }

  return FALSE;
}
static void BestCDDperBioseq (BioseqPtr bsp, Pointer userdata)

{
  SeqFeatPtr         best;
  SeqMgrFeatContext  context;
  FloatHi            currscore;
  Int4               right;
  SeqFeatPtr         sfp;
  FloatHi            topscore;

  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &context);
  while (sfp != NULL) {
    if (context.featdeftype == FEATDEF_REGION && FeatIsCDD (sfp, &currscore)) {
      best = sfp;
      right = context.right;
      topscore = currscore;
      sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &context);
      while (sfp != NULL && context.featdeftype == FEATDEF_REGION &&
             FeatIsCDD (sfp, &currscore) && context.left < right) {
        right = MAX (context.right, right);
        if (currscore <= topscore) {
          sfp->idx.deleteme = TRUE;
        } else {
          best->idx.deleteme = TRUE;
          best = sfp;
          topscore = currscore;
        }
        sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &context);
      }
    } else {
      sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &context);
    }
  }
}

NLM_EXTERN void LeaveBestCDD (SeqEntryPtr sep)

{
  Uint2  entityID;

  if (sep == NULL) return;
  entityID = ObjMgrGetEntityIDForChoice (sep);
  if (entityID < 1) return;

  if (SeqMgrFeaturesAreIndexed (entityID) == 0) {
    SeqMgrIndexFeatures (entityID, NULL);
  }

  VisitBioseqsInSep (sep, NULL, BestCDDperBioseq);
  DeleteMarkedObjects (entityID, 0, NULL);

  SeqMgrClearFeatureIndexes (entityID, NULL);
}

static CharPtr CompressNonBases (CharPtr str)

{
  Char     ch;
  CharPtr  dst;
  CharPtr  ptr;

  if (str == NULL || str [0] == '\0') return NULL;

  dst = str;
  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    if (IS_ALPHA (ch)) {
      *dst = ch;
      dst++;
    }
    ptr++;
    ch = *ptr;
  }
  *dst = '\0';

  return str;
}

static void LIBCALLBACK SPStreamToRaw (
  CharPtr sequence,
  Pointer userdata
)

{
  ByteStorePtr  bs;
  Char          ch;
  size_t        len;
  CharPtr       tmp;

  bs = (ByteStorePtr) userdata;
  tmp = sequence;
  ch = *tmp;
  while (ch != '\0') {
    if (ch == '\n' || ch == '\r' || ch == '\t') {
      *tmp = ' ';
    } else {
      *tmp = TO_UPPER (ch);
    }
    tmp++;
    ch = *tmp;
  }
  TrimSpacesAroundString (sequence);
  CompressNonBases (sequence);

  len = StringLen (sequence);
  if (len < 1) return;
  BSWrite (bs, sequence, len * sizeof (Char));
}

NLM_EXTERN void SegOrDeltaBioseqToRaw (BioseqPtr bsp)

{
  ByteStorePtr  bs;

  if (bsp == NULL || (bsp->repr != Seq_repr_seg && bsp->repr != Seq_repr_delta)) return;
  if (! ISA_na (bsp->mol)) return;
  bs = BSNew (bsp->length);
  if (bs == NULL) return;

  SeqPortStream (bsp, STREAM_EXPAND_GAPS | STREAM_CORRECT_INVAL, (Pointer) bs, SPStreamToRaw);

  if (bsp->repr == Seq_repr_seg && bsp->seq_ext_type == 1) {
    bsp->seq_ext = SeqLocSetFree ((ValNodePtr) bsp->seq_ext);
    bsp->seq_ext_type = 0;
  } else if (bsp->repr == Seq_repr_delta && bsp->seq_ext_type == 4) {
    bsp->seq_ext = NULL; /* for now just NULL out */
    bsp->seq_ext_type = 0;
  }
  bsp->seq_data = BSFree (bsp->seq_data);
  bsp->seq_data = bs;
  bsp->length = BSLen (bs);
  bsp->repr = Seq_repr_raw;
  bsp->seq_data_type = Seq_code_iupacna;
}

typedef struct segtodelta
{
  ValNodePtr seq_ext;
  Int4       len;
  SeqIdPtr   master_sip;
  BioseqPtr  master_bsp;  
  Int4       num_segs_converted;
} SegToDeltaData, PNTR SegToDeltaPtr;


static ValNodePtr CombineDescriptorLists (ValNodePtr target, ValNodePtr insert)
{
  ValNodePtr combined_list = NULL;
  ValNodePtr vnp, vnp_next;
  ValNodePtr title_descr = NULL, prev_descr = NULL;
  CharPtr    combined_title;
  Int4       combined_title_len;
  
  if (target == NULL)
  {
    combined_list = insert;
  }
  else if (insert == NULL)
  {
    combined_list = target;
  }
  else
  {
    combined_list = target;
	  for (vnp = target; vnp->next != NULL; vnp = vnp->next)
	  {
	    if (vnp->choice == Seq_descr_title)
	    {
	      title_descr = vnp;
	    }
	  }
	  prev_descr = vnp;
	  if (title_descr == NULL)
	  {
	    prev_descr->next = insert;
	  }
	  else
	  {
	    for (vnp = insert; vnp != NULL; vnp = vnp_next)
	    {
	      vnp_next = vnp->next;
	      vnp->next = NULL;
	      if (vnp->choice == Seq_descr_title)
	      {
	        /* combine with previous title */
	        combined_title_len = StringLen (title_descr->data.ptrvalue)
	                            + StringLen (vnp->data.ptrvalue)
	                            + 3;
	        combined_title = (CharPtr) MemNew (sizeof (Char) * combined_title_len);
	        if (combined_title != NULL)
	        {
	          StringCpy (combined_title, title_descr->data.ptrvalue);
	          StringCat (combined_title, "; ");
	          StringCat (combined_title, vnp->data.ptrvalue);
	          title_descr->data.ptrvalue = MemFree (title_descr->data.ptrvalue);
	          title_descr->data.ptrvalue = combined_title;
	        }
	        ValNodeFreeData (vnp);
	      }
	      else
	      {
	        /* add to master list */
	        prev_descr->next = vnp;
	        prev_descr = vnp;
	      }
	    } 
	  }
  }
  return combined_list;
}

static void MoveSegmentLocToMaster (SeqLocPtr slp, SegToDeltaPtr sdp)
{
  SeqIntPtr     sintp;
  SeqLocPtr     slp2;
  SeqPntPtr     spp;
  PackSeqPntPtr pspp;
  Int4          i;
  
  if (slp == NULL || sdp == NULL) return;
  
  switch (slp->choice)
  {
    case SEQLOC_WHOLE:
    case SEQLOC_EMPTY:
      slp->data.ptrvalue = SeqIdFree (slp->data.ptrvalue);
      slp->data.ptrvalue = SeqIdDup (sdp->master_sip);
      break;
    case SEQLOC_INT:
      sintp = (SeqIntPtr) slp->data.ptrvalue;
      if (sintp != NULL)
      {
        sintp->id = SeqIdFree (sintp->id);
        sintp->id = SeqIdDup (sdp->master_sip);
        sintp->from += sdp->len;
        sintp->to += sdp->len;
        /* strand stays the same */
      }
      break;
    case SEQLOC_PACKED_INT:
    case SEQLOC_MIX:    
    case SEQLOC_EQUIV:
			slp2 = (SeqLocPtr)slp->data.ptrvalue;
			while (slp2 != NULL)
			{
				MoveSegmentLocToMaster (slp2, sdp);
				slp2 = slp2->next;
			}
      break;
    case SEQLOC_PNT:
      spp = (SeqPntPtr) slp->data.ptrvalue;
      if (spp != NULL)
      {
        spp->id = SeqIdFree (spp->id);
        spp->id = SeqIdDup (sdp->master_sip);
        spp->point += sdp->len;
      }
      break;
    case SEQLOC_PACKED_PNT:
			pspp = (PackSeqPntPtr)slp->data.ptrvalue;
			while (pspp != NULL)
      {
        for (i = 0; i < pspp->used; i++)
        {
          pspp->pnts[i] += sdp->len;
        }
        pspp->id = SeqIdFree (pspp->id);
        pspp->id = SeqIdDup (sdp->master_sip);
        pspp = pspp->next;
      }
      break;
  }
}

static void MoveSegmentFeaturesToMaster (SeqFeatPtr sfp, Pointer userdata)

{
  SegToDeltaPtr   segdeltptr;

  if (sfp == NULL || userdata == NULL) return;
  
  segdeltptr = (SegToDeltaPtr) userdata;

  MoveSegmentLocToMaster (sfp->location, segdeltptr);
}

static void AdjustAlignmentOffsetsForDeltaConversion (SeqAlignPtr salp, Int4Ptr offsets, BoolPtr is_gap, Int4 num_sets)
{
  DenseSegPtr dsp;
  Int4        aln_seg_num, j, index;
  
  if (salp == NULL || offsets == NULL) return;

  /* adjust alignment starts to match delta sequence coordinates */
  if (salp->segtype == 2)
  {
    dsp = (DenseSegPtr) (salp->segs);
    aln_seg_num = 0;
    for (j = 0; j < num_sets; j++)
    {
      if (!is_gap [j])
      {
        for (index = 0; index < dsp->numseg; index++)
        {
          if (dsp->starts [dsp->dim * index + aln_seg_num] != -1)
          {
            dsp->starts [dsp->dim * index + aln_seg_num] += offsets [j];  
          }
        }
        aln_seg_num++;
      }
    }
  }      
}

static SeqAnnotPtr CombineAnnots (SeqAnnotPtr target, SeqAnnotPtr insert, Int4 offset)
{
  SeqAnnotPtr combined_list = NULL;
  SeqAnnotPtr feature_sap = NULL;
  SeqAnnotPtr prev_sap = NULL;
  SeqAnnotPtr sap, next_sap;
  SeqFeatPtr  last_feat, first_feat;
  
  if (target == NULL)
  {
    combined_list = insert;
  }
  else if (insert == NULL)
  {
    combined_list = target;
  }
  else
  {
    combined_list = target;
    for (sap = target; sap != NULL; sap = sap->next)
    {
      if (sap->type == 1 && sap->name == NULL && sap->desc == NULL)
      {
        feature_sap = sap;
      }
      prev_sap = sap;
    }
    for (sap = insert; sap != NULL; sap = next_sap)
    {
      next_sap = sap->next;
      sap->next = NULL;
      if (sap->type == 1 && sap->name == NULL && sap->desc == NULL && feature_sap != NULL)
      {
        first_feat = (SeqFeatPtr) sap->data;
        if (first_feat != NULL)
        {
          for (last_feat = (SeqFeatPtr) feature_sap->data;
               last_feat != NULL && last_feat->next != NULL;
               last_feat = last_feat->next)
          {  
          }
          if (last_feat == NULL)
          {
            feature_sap->data = first_feat;    
          }
          else
          {
            last_feat->next = first_feat;
          }
        }
        sap->data = NULL;
        SeqAnnotFree (sap);
      }
      else
      {
        prev_sap->next = sap;
        prev_sap = sap;
      }
    }
  }
  return combined_list;
}

static Int4 AddGapSeqLit (ValNodePtr PNTR seq_ext)
{
  SeqLitPtr       slip;
  IntFuzzPtr      ifp;
  CharPtr         gap_chars = "NNNNNNNNNN"
                              "NNNNNNNNNN"
                              "NNNNNNNNNN"
                              "NNNNNNNNNN"
                              "NNNNNNNNNN"
                              "NNNNNNNNNN"
                              "NNNNNNNNNN"
                              "NNNNNNNNNN"
                              "NNNNNNNNNN"
                              "NNNNNNNNNN";
                              
  if (seq_ext == NULL) return 0;
                                
  slip = (SeqLitPtr) MemNew (sizeof (SeqLit));
  if (slip != NULL) {
    slip->length = 100;
    ValNodeAddPointer (seq_ext, (Int2) 2, (Pointer) slip);
    ifp = IntFuzzNew ();
    ifp->choice = 4;
      
    slip->fuzz = ifp;
    slip->seq_data = BSNew (slip->length);
    slip->seq_data_type = Seq_code_iupacna;
    AddBasesToByteStore (slip->seq_data, gap_chars);
    return 100;
  }
  return 0;
}

static Boolean LIBCALLBACK 
AddSegmentToDeltaSeq 
(SeqLocPtr slp,
 SeqMgrSegmentContextPtr context)

{
  SegToDeltaPtr   segdeltptr;
  SeqIdPtr        sip;
  BioseqPtr       bsp;
  CharPtr         bases;
  SeqLitPtr       slip;

  SeqLocPtr         loc;

  if (slp == NULL || context == NULL) return FALSE;
  segdeltptr = (SegToDeltaPtr) context->userdata;
  if (segdeltptr == NULL) return FALSE;

  sip = SeqLocId (slp);
  
  if (sip == NULL) {
    loc = SeqLocFindNext (slp, NULL);
    if (loc != NULL) {
      sip = SeqLocId (loc);
    }
  }
  if (sip == NULL) 
  {
    return TRUE;
  }

  bsp = BioseqFind (sip);

  if (bsp == NULL)
  {
    return TRUE;
  }
  
  bases = GetSequenceByBsp (bsp);
  if (bases == NULL) 
  {
    bsp->idx.deleteme = TRUE;
    return TRUE;    
  }
  
  if (segdeltptr->seq_ext != NULL)
  {
    /* insert gap of unknown length between the previous segment
     * and this one.
     */
    segdeltptr->len += AddGapSeqLit (&(segdeltptr->seq_ext));
  }

  /* move descriptors to master_bsp */
  segdeltptr->master_bsp->descr = CombineDescriptorLists (segdeltptr->master_bsp->descr, bsp->descr);
  bsp->descr = NULL;
  
  /* move features to master_bsp */
  VisitFeaturesOnBsp (bsp, segdeltptr, MoveSegmentFeaturesToMaster);
  segdeltptr->master_bsp->annot = CombineAnnots (segdeltptr->master_bsp->annot, bsp->annot, segdeltptr->len);
  bsp->annot = NULL;
  
  slip = (SeqLitPtr) MemNew (sizeof (SeqLit));
  if (slip != NULL) 
  {
    slip->length = StringLen (bases);
    ValNodeAddPointer (&(segdeltptr->seq_ext), (Int2) 2, (Pointer) slip);
    slip->seq_data = BSNew (slip->length);
    slip->seq_data_type = Seq_code_iupacna;
    AddBasesToByteStore (slip->seq_data, bases);
    segdeltptr->len += slip->length;
  }

  segdeltptr->num_segs_converted ++;
  return TRUE;
}

static BioseqPtr GetDeltaSeqFromMasterSeg (BioseqPtr bsp)
{
  BioseqPtr      new_bsp;
  SegToDeltaData sdd;
  BioseqSetPtr   segset;
  
  if (bsp == NULL || bsp->repr != Seq_repr_seg 
      || bsp->seq_ext == NULL || bsp->seq_ext_type != 1) 
  {
    return NULL;
  }
  
  if (! ISA_na (bsp->mol)) return NULL;

  /* use SeqMgrExploreSegments to build a list of SeqLitPtr */
  sdd.seq_ext = NULL;
  sdd.len = 0;
  sdd.master_bsp = bsp;
  sdd.master_sip = bsp->id;
  sdd.num_segs_converted = 0;
  
  /* move descriptors and features from segset to master seg */
  if (bsp->idx.parenttype == OBJ_BIOSEQSET)
  {
    segset = (BioseqSetPtr) bsp->idx.parentptr;
    if (segset != NULL)
    {
      bsp->descr = CombineDescriptorLists (bsp->descr, segset->descr);
      segset->descr = NULL;
    }
  }  

  SeqMgrExploreSegments (bsp, (Pointer) &sdd, AddSegmentToDeltaSeq);
  
  new_bsp = BioseqNew ();
  new_bsp->descr = bsp->descr;
  bsp->descr = NULL;
  new_bsp->annot = bsp->annot;
  bsp->annot = NULL;
  new_bsp->seq_data = NULL;
  new_bsp->seq_data_type = 0;
  new_bsp->repr = Seq_repr_delta;
  new_bsp->seq_ext_type = 4;
  new_bsp->seq_ext = sdd.seq_ext;
  new_bsp->length = sdd.len;
  new_bsp->id = SeqIdDup (bsp->id); 
/*  new_bsp->id = MakeUniqueSeqID ("delta_"); */
  new_bsp->mol = bsp->mol;

  BioseqPack (new_bsp);  
  return new_bsp;
}

NLM_EXTERN void ConvertSegSetsToDeltaSequences (SeqEntryPtr sep)
{
  BioseqSetPtr  bssp;
  SeqEntryPtr   sub_sep, prev_sep, next_sep;
  ObjMgrDataPtr omdptop;
  ObjMgrData    omdata;
  Uint2         parenttype;
  Pointer       parentptr;
  SeqEntryPtr   new_sep;
  BioseqPtr     bsp, new_bsp;
  BioseqSetPtr  parent_set;
  
  if (sep == NULL || !IS_Bioseq_set (sep)) return;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp->_class == 2)
  {
    SaveSeqEntryObjMgrData (sep, &omdptop, &omdata);
    GetSeqEntryParent (sep, &parentptr, &parenttype);
  
    parent_set = (BioseqSetPtr)(bssp->idx.parentptr);
    prev_sep = NULL;
    for (sub_sep = bssp->seq_set; sub_sep != NULL && !IS_Bioseq (sub_sep); sub_sep = sub_sep->next)
    {
      prev_sep = sub_sep;
    }
    if (sub_sep != NULL)
    {
      bsp = sub_sep->data.ptrvalue;
      new_bsp = GetDeltaSeqFromMasterSeg (sub_sep->data.ptrvalue);
      new_sep = SeqEntryNew();
      new_sep->choice = 1;
      new_sep->data.ptrvalue = new_bsp;
            
      /* add new seq entry to parent set */
      AddSeqEntryToSeqEntry (parent_set->seqentry, new_sep, TRUE);

      /* remove segset */      
      bssp->idx.deleteme = TRUE;
    }
    SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
    RestoreSeqEntryObjMgrData (sep, omdptop, &omdata);
    DeleteMarkedObjects (0, OBJ_BIOSEQSET, parent_set);
    SeqMgrReplaceInBioseqIndex (new_bsp); 
  }
  else
  {
    for (sub_sep = bssp->seq_set; sub_sep != NULL; sub_sep = next_sep)
    {
      next_sep = sub_sep->next;
      ConvertSegSetsToDeltaSequences (sub_sep);
    }
  }
}

static PubMedFetchFunc pmf_pubfetch = NULL;

NLM_EXTERN void LIBCALL PubMedSetFetchFunc (PubMedFetchFunc func)

{
  pmf_pubfetch = func;
}

NLM_EXTERN PubmedEntryPtr LIBCALL GetPubMedForUid (Int4 uid)

{
  PubMedFetchFunc  func;

  if (uid < 1) return NULL;
  func = pmf_pubfetch;
  if (func == NULL) return NULL;
  return func (uid);
}

static Boolean IsTerminator (int c)
{
  if (c == '\n' || c == '\r') {
    return TRUE;
  } else {
    return FALSE;
  }
}

typedef struct bufferedread {
  CharPtr data;
  Int4    len;
  Int4    offset;
} BufferedReadData, PNTR BufferedReadPtr;

static BufferedReadPtr BufferedReadFree (BufferedReadPtr brp)
{
  if (brp == NULL) return NULL;
  if (brp->data != NULL) {
    MemFree (brp->data);
    brp->data = NULL;
  }
  brp->offset = 0;
  brp->len = 0;
  return NULL;
}

extern void FreeBufferedReadList (ValNodePtr vnp)
{
  if (vnp == NULL) return;
  FreeBufferedReadList (vnp->next);
  vnp->next = NULL;
  vnp->data.ptrvalue = BufferedReadFree ( (BufferedReadPtr)vnp->data.ptrvalue); 
  ValNodeFree (vnp);
}

/* three possible return codes:
 * 0 = no terminators seen at all
 * 1 = have terminator plus one character
 * 2 = last is terminator - need more characters
 */
static Int4 HasTerminator (ValNodePtr list, Int4 PNTR len)
{
  CharPtr      cp;
  ValNodePtr   vnp;
  BufferedReadPtr brp;

  if (len == NULL) return 0;
  *len = 0;
  if (list == NULL) return 0;

  for (vnp = list; vnp != NULL; vnp = vnp->next) {
    if (vnp->data.ptrvalue == NULL) continue;
    brp = (BufferedReadPtr) vnp->data.ptrvalue;
    if (brp->data == NULL) continue;
    for (cp = brp->data + brp->offset; *cp != 0; cp++) {
      if (IsTerminator (*cp)) {
        if (* (cp + 1) != 0 || vnp->next != NULL) {
          return 1;
        } else {
          return 2;
        }
      } else { 
        (*len) ++;
      }
    }
  }
  return 0;
}

static CharPtr GetLineFromBuffer (ValNodePtr PNTR current_data, Int4 len)
{
  ValNodePtr      vnp, next_vnp;
  BufferedReadPtr brp;
  CharPtr         cp;
  CharPtr         new_line;
  Int4            ctr;
  Char            this_terminator;
  CharPtr         next_char;

  if (current_data == NULL || *current_data == NULL) return NULL;

  new_line = MemNew (len + 1);
  if (new_line == NULL) return NULL;

  ctr = 0;
  vnp = *current_data;
  while (vnp != NULL && ctr < len) {
    if ((brp = (BufferedReadPtr)vnp->data.ptrvalue) == NULL || brp->data == NULL) {
      next_vnp = vnp->next;
      vnp->next = NULL;
      vnp->data.ptrvalue = BufferedReadFree (brp);
      ValNodeFree (vnp);
      vnp = next_vnp;
    } else {
      if (ctr + brp->len <= len) {
        MemCpy (new_line + ctr, brp->data + brp->offset, brp->len);
        ctr += brp->len;
        next_vnp = vnp->next;
        vnp->next = NULL;
        vnp->data.ptrvalue = BufferedReadFree (brp);
        ValNodeFree (vnp);
        vnp = next_vnp;
      } else {
        MemCpy (new_line + ctr, brp->data + brp->offset, len - ctr);
        brp->offset += len - ctr;
        brp->len -= (len - ctr);
        ctr = len;
      }
    }
  }
  if (vnp != NULL) {
    brp = (BufferedReadPtr)vnp->data.ptrvalue;
    if (brp->len >= 0) {
      cp = brp->data + brp->offset;
      this_terminator = *cp;
      /* handle condition when last character in data is terminator */
      if (* (cp + 1) == 0) {
        next_vnp = vnp->next;
        vnp->next = NULL;
        vnp->data.ptrvalue = BufferedReadFree (brp);
        ValNodeFree (vnp);
        vnp = next_vnp;
        while (vnp != NULL && (brp = (BufferedReadPtr)vnp->data.ptrvalue) == NULL) {
          next_vnp = vnp->next;
          vnp->next = NULL;
          vnp->data.ptrvalue = BufferedReadFree (brp);
          ValNodeFree (vnp);
          vnp = next_vnp;
        }
        if (vnp == NULL) {
          *current_data = NULL;
          new_line [len] = 0;
          return new_line;
        } else {
          next_char = brp->data + brp->offset;
          if (IsTerminator (*next_char) && *next_char != this_terminator) {
            brp->offset ++;
            brp->len --;
            if (brp->len == 0) {
              next_vnp = vnp->next;
              vnp->next = NULL;
              vnp->data.ptrvalue = BufferedReadFree (brp);
              ValNodeFree (vnp);
              vnp = next_vnp;
            }
          }
        }
      } else {
        next_char = cp + 1;
        if (IsTerminator (*next_char) && *next_char != this_terminator) {
          brp->offset += 2;
          brp->len -= 2;
        } else {
          brp->offset ++;
          brp->len --;
        }
      }
      if (brp->len <= 0) {
        next_vnp = vnp->next;
        vnp->next = NULL;
        vnp->data.ptrvalue = BufferedReadFree (brp);
        ValNodeFree (vnp);
        vnp = next_vnp;
      }
    }
  }
  *current_data = vnp;
  new_line [len] = 0;
  return new_line;
}

#define READ_BUFFER_SIZE 5000

static ValNodePtr AddToBuffer (ValNodePtr current_data, FILE *fp)
{
  ValNodePtr vnp;
  BufferedReadPtr brp;

  vnp = ValNodeNew (current_data);
  if (vnp == NULL) return NULL;
 
  brp = (BufferedReadPtr) MemNew (sizeof (BufferedReadData));
  if (brp == NULL) return NULL;
  brp->data = MemNew (READ_BUFFER_SIZE);
  if (brp->data == NULL) return NULL;
  brp->offset = 0;
 
  brp->len = fread (brp->data, 1, READ_BUFFER_SIZE - 1, fp);
  *(char *)(brp->data + brp->len) = 0; 

  vnp->data.ptrvalue = brp;
  return vnp;
}

extern CharPtr MyFGetLine (FILE *fp, ValNodePtr PNTR current_data)
{
  Int4       terminator_status;
  Int4       data_len;
  ValNodePtr last_vnp;

  terminator_status = HasTerminator (*current_data, &data_len);
  while (!feof (fp) && terminator_status == 0) {
    last_vnp = AddToBuffer (*current_data, fp);
    if (*current_data == NULL) {
      *current_data = last_vnp;
    }
    terminator_status = HasTerminator (*current_data, &data_len);
  }

  if (!feof (fp) && terminator_status == 2) {
    AddToBuffer (*current_data, fp);
  }
  return GetLineFromBuffer (current_data, data_len);
} 



#if defined (WIN32)
extern char * __stdcall AbstractReadFunction (Pointer userdata)
#else
extern char * AbstractReadFunction (Pointer userdata)
#endif
{
  ReadBufferPtr rbp;

  if (userdata == NULL) return NULL;

  rbp = (ReadBufferPtr) userdata;

  return MyFGetLine (rbp->fp, &(rbp->current_data));
}

#if defined (WIN32)
extern void __stdcall AbstractReportError (
#else
extern void AbstractReportError (
#endif
  TErrorInfoPtr err_ptr,
  Pointer      userdata
)
{
  TErrorInfoPtr PNTR list;
  TErrorInfoPtr last;

  if (err_ptr == NULL || userdata == NULL) return;

  list = (TErrorInfoPtr PNTR) userdata;

  if (*list == NULL)
  {
    *list = err_ptr;
  }
  else
  {
    for (last = *list; last != NULL && last->next != NULL; last = last->next)
    {}
    last->next = err_ptr;
  }

}

static void AddDefLinesToAlignmentSequences 
(TAlignmentFilePtr afp,
 SeqEntryPtr sep_head)
{
  BioseqSetPtr bssp;
  SeqEntryPtr  sep;
  Int4         index;
  ValNodePtr   sdp;
  CharPtr      new_title;
  Int4         new_title_len;
  Int4         curr_seg;
  Int4         num_sets = 1;
  Boolean      one_defline_per_sequence = TRUE;
  Boolean      all_extra_empty;

 
  if (afp == NULL || sep_head == NULL || ! IS_Bioseq_set (sep_head))
  {
    return;
  }
  if ((afp->num_deflines == 0 || afp->deflines == NULL)
    && (afp->num_organisms == 0 || afp->organisms == NULL))
  {
    return;
  }
  bssp = sep_head->data.ptrvalue;
  
  /* find out if all of our deflines are real */
  if (afp->num_segments > 1 && afp->num_deflines == afp->num_sequences)
  {
    one_defline_per_sequence = FALSE;
    num_sets = afp->num_sequences / afp->num_segments;
    all_extra_empty = TRUE;
    for (curr_seg = num_sets; curr_seg < afp->num_deflines && all_extra_empty; curr_seg ++)
    {
      if (afp->deflines [curr_seg] != NULL)
      {
        all_extra_empty = FALSE;
      }
    }
  	if (all_extra_empty)
  	{
  		one_defline_per_sequence = TRUE;
  	}
  }
  
  for (sep = bssp->seq_set, index = 0;
       sep != NULL && (index < afp->num_deflines || index < afp->num_organisms);
       sep = sep->next, index++)
  {
    new_title_len = 0;
    /* get lengths for organisms for this sequence */
    
    if (afp->num_segments > 1 && afp->num_organisms == afp->num_sequences)
    {
      /* have one organism per segment, in which case use only the first one */
      curr_seg = index * afp->num_segments;
    }
    else
    { /* otherwise one organism per sequence */
      curr_seg = index;
    }
    if (curr_seg < afp->num_organisms)
    {
      new_title_len += StringLen (afp->organisms [curr_seg]) + 1;
    }

    /* get lengths for deflines for this sequence */
    if (! one_defline_per_sequence)
    { /* have one defline per segment, in which use only the first one */
      curr_seg = index * afp->num_segments;
    }
    else
    { /* otherwise one defline per sequence */
      curr_seg = index;	
    }
    if (curr_seg < afp->num_deflines) 
    {
      new_title_len += StringLen (afp->deflines [curr_seg]) + 1;
    }
  	
    if (new_title_len > 0) {
      new_title = (CharPtr) MemNew (new_title_len);
      if (new_title == NULL) return;
      new_title [0] = 0;
      
      /* list organisms at beginning of new defline */
      if (afp->num_segments > 1 && afp->num_organisms == afp->num_sequences)
      { /* have one organism per segment, in which case use only first one */
        curr_seg = index * afp->num_segments;
      }
      else
      { /* otherwise one organism per sequence */
      	curr_seg = index;
      }

      if (curr_seg < afp->num_organisms) {
        StringCat (new_title, afp->organisms [curr_seg]);
        if (new_title_len > StringLen (new_title) + 1)
        {
          StringCat (new_title, " ");
        }
      }
      
      if (!one_defline_per_sequence)
      { /* have one defline per segment, in which case all go to same sequence */
        curr_seg = index * afp->num_segments;
      }
      else 
      {
      	curr_seg = index;
      }
      if (curr_seg < afp->num_deflines) 
      {
    	StringCat (new_title, afp->deflines [curr_seg]);
      }

      sdp = CreateNewDescriptor (sep, Seq_descr_title);
      if (sdp != NULL) {
        sdp->data.ptrvalue = new_title;
      } else {
        MemFree (new_title);
      }
    }
  }
}
    

static SeqEntryPtr 
MakeDeltaSetFromAlignment 
(SeqEntryPtr sep_list,
 TAlignmentFilePtr afp,
 Uint1 moltype,
 Int4  gap_length
 )
{
  BioseqPtr    bsp, deltabsp;
  SeqEntryPtr  this_list, last_sep, next_list, sep, nextsep;
  SeqEntryPtr  topsep, last_delta_sep;
  SeqIdPtr     sip;
  Int4         curr_seg;
  CharPtr      seqbuf;
  ValNodePtr   vnp;
  SeqLitPtr    slp;
  IntFuzzPtr   ifp;
  SeqEntryPtr  delta_list = NULL;
  
  delta_list = NULL;
  last_delta_sep = NULL;
  this_list = sep_list;
  while (this_list != NULL)
  {
    last_sep = this_list;
    curr_seg = 0;
    while (last_sep != NULL && curr_seg < afp->num_segments - 1)
    {
    	last_sep = last_sep->next;
    	curr_seg++;
    }
    if (last_sep == NULL) return NULL;
    next_list = last_sep->next;
    last_sep->next = NULL;
  
    bsp = (BioseqPtr)this_list->data.ptrvalue;
    if (bsp == NULL) return NULL;

    sip = SeqIdDup (bsp->id);
    vnp = ValNodeExtract (&(bsp->descr), Seq_descr_title);

    deltabsp = BioseqNew ();
    if (deltabsp == NULL) return NULL;
    deltabsp->repr = Seq_repr_delta;
    deltabsp->seq_ext_type = 4;
    deltabsp->mol = moltype;
    deltabsp->length = 0;

    topsep = SeqEntryNew ();
    if (topsep == NULL) return NULL;
    topsep->choice = 1;
    topsep->data.ptrvalue = (Pointer) deltabsp;

    for (sep = this_list; sep != NULL; sep = nextsep) {
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
        if (slp->length < 1) {
          slp->length = 0;
          ifp = IntFuzzNew ();
          ifp->choice = 4;
          slp->fuzz = ifp;
        }

        deltabsp->length += slp->length;
      }
      SeqEntryFree (sep);
      
      if (nextsep != NULL)
      {
        /* add gap */
        slp = (SeqLitPtr) MemNew (sizeof (SeqLit));
        if (slp == NULL) continue;
        slp->length = gap_length;
        ValNodeAddPointer ((ValNodePtr PNTR) &(deltabsp->seq_ext), (Int2) 2, (Pointer) slp);
        deltabsp->length += slp->length;        
      }
    }

    ValNodeLink (&(deltabsp->descr), vnp);
    deltabsp->id = sip;
    
    if (last_delta_sep == NULL)
    {
    	delta_list = topsep;
    }
    else 
    {
        last_delta_sep->next = topsep;	
    }
    last_delta_sep = topsep;
    
    this_list = next_list;	
  }
  return delta_list;
}

static void RenameSegSet (SeqEntryPtr sep)
{
  BioseqSetPtr bssp, seg_bssp;
  SeqEntryPtr  seg_sep;
  BioseqPtr    main_bsp = NULL;
  BioseqPtr    seg_bsp = NULL;
  Char         new_id_str [255];
  
  if (sep == NULL || !IS_Bioseq_set (sep) || (bssp = sep->data.ptrvalue) == NULL
      || bssp->_class != BioseqseqSet_class_segset)
  {
  	return;
  }
  
  sep = bssp->seq_set;
  while (sep != NULL && (seg_bsp == NULL || main_bsp == NULL))
  {
  	if (IS_Bioseq (sep))
  	{
  	  main_bsp = (BioseqPtr) sep->data.ptrvalue;
  	}
  	else if (IS_Bioseq_set (sep))
  	{
  	  seg_bssp = (BioseqSetPtr) sep->data.ptrvalue;
  	  if (seg_bssp != NULL && seg_bssp->_class == BioseqseqSet_class_parts)
  	  {
  	  	seg_sep = seg_bssp->seq_set;
  	  	while (seg_sep != NULL && seg_bsp == NULL)
  	  	{
  	  	  if (IS_Bioseq (seg_sep))
  	  	  {
  	  	    seg_bsp = seg_sep->data.ptrvalue;
  	      }
  	      seg_sep = seg_sep->next;
  	    }
  	  }
  	}
  	sep = sep->next;
  }
  if (main_bsp == NULL || seg_bsp == NULL)
  {
  	return;
  }
  SeqIdWrite (seg_bsp->id, new_id_str, PRINTID_FASTA_SHORT, sizeof (new_id_str) - 7);
  StringCat (new_id_str, "_master");
  SeqIdFree (main_bsp->id);
  main_bsp->id = MakeSeqID (new_id_str);
}

static SeqEntryPtr 
MakeSegmentedSetFromAlignment 
(SeqEntryPtr       sep_list,
 TAlignmentFilePtr afp,
 Uint1             moltype,
 Int4Ptr           segs_per_set)
{
  SeqEntryPtr  this_list, last_sep, next_list, nextsep, last_segset;
  Int4         curr_seg;
  Int4         set_index = 0;
  
  this_list = sep_list;
  sep_list = NULL;
  last_segset = NULL;
  while (this_list != NULL)
  {
    last_sep = this_list;
    curr_seg = 0;
    while (last_sep != NULL && curr_seg < segs_per_set [set_index] - 1)
    {
      if (!IS_Bioseq (last_sep)) return NULL;
      last_sep = last_sep->next;
      curr_seg++;
    }
    if (last_sep == NULL) return NULL;
    next_list = last_sep->next;
    last_sep->next = NULL;
    
    last_sep = this_list->next;
    this_list->next = NULL;
    while (last_sep != NULL)
    {
      nextsep = last_sep->next;
      last_sep->next = NULL;
      AddSeqEntryToSeqEntry (this_list, last_sep, FALSE);
      last_sep = nextsep;
    }

    /* fix IDs for seg sets */    
    RenameSegSet (this_list);
    
    if (sep_list == NULL) 
    {
      sep_list = this_list;
    }
    else
    {
      last_segset->next = this_list;
    }
    last_segset = this_list;
    
    this_list = next_list;
    set_index++;
  }
  return sep_list; 	
}


extern CharPtr AlignmentStringToSequenceString (CharPtr aln_str, Uint1 moltype)
{
  CharPtr cp_aln, cp_seq;
  Char    ch;
  CharPtr seq_str;
  
  if (aln_str == NULL) return NULL;
  seq_str = (CharPtr) MemNew (sizeof (Char) * (StringLen (aln_str) + 1));
  if (seq_str == NULL) return NULL;
  cp_seq = seq_str;
  for (cp_aln = aln_str; *cp_aln != 0; cp_aln++)
  {
    ch = *cp_aln;
    ch = TO_UPPER (ch); 
    if ( ISA_na (moltype) ) 
    {
      if (ch == 'U') ch = 'T';
      if (ch == 'X') ch = 'N';
      if ( StringChr ("EFIJLOPQXZ-.*", ch) == NULL )  
      { 
        *cp_seq = ch;
        cp_seq++;
      }
    }
    else 
    {
      if ( StringChr("JO-.", ch) == NULL ) 
      {
        *cp_seq = ch;
        cp_seq++;
      } 
    }
  }
  *cp_seq = 0;
  return seq_str;
}

static SeqEntryPtr SequenceStringToSeqEntry (CharPtr str, SeqIdPtr sip, Uint1 mol_type)
{
  SeqEntryPtr  sep;
  BioseqPtr    bsp;
  ByteStorePtr bs;

  if (str == NULL || sip == NULL) return NULL;
  sep = SeqEntryNew ();
  if (sep == NULL) return NULL;
  bsp = BioseqNew ();
  if (bsp == NULL) 
  { 
    ValNodeFree (sep); 
    return NULL; 
  }
  sep->choice = 1;
  sep->data.ptrvalue = (Pointer) bsp;
  bsp->id = SeqIdDup (sip);
  bsp->id->next = NULL;
  SeqMgrAddToBioseqIndex (bsp);
  bsp->repr = Seq_repr_raw;
  if ( ISA_na (mol_type) ) 
  {
    bsp->mol = Seq_mol_na;
    bsp->seq_data_type = Seq_code_iupacna;
  } 
  else
  {
    bsp->mol = Seq_mol_aa;
    bsp->seq_data_type = Seq_code_ncbieaa;
  }
  bsp->length = StringLen (str);
  if ( bsp->length == 0 ) 
  {
    BioseqFree (bsp);
    ValNodeFree (sep);
    return NULL;
  }
  bs = BSNew (bsp->length);
  bsp->seq_data = bs;
  BSWrite (bs, str, bsp->length);

  return sep;
}

static SeqEntryPtr MakeDeltaSeqsFromAlignmentSequences (TAlignmentFilePtr afp, Uint1 moltype, CharPtr PNTR seq_str)
{
  Int4            num_sets, next_start, k, index;
  SeqIdPtr        sip;
  SeqLitPtr       slip;
  SeqEntryPtr     sep_list = NULL, sep, sep_last = NULL;
  BioseqPtr       new_bsp;
  ValNodePtr      seq_ext = NULL;
  
  if (afp == NULL || seq_str == NULL) return NULL;
  
  num_sets = afp->num_sequences / afp->num_segments;
  for (k = 0; k < num_sets; k++)
  {
    sep = SeqEntryNew ();
    if (sep == NULL) return NULL;
    new_bsp = BioseqNew ();
    if (new_bsp == NULL) return NULL;
    sip = MakeSeqID (afp->ids [k * afp->num_segments]);
    new_bsp->id = sip;
    sep->choice = 1;
    sep->data.ptrvalue = new_bsp;
    SeqMgrAddToBioseqIndex (new_bsp);
    
    if (sep_last == NULL)
    {
      sep_list = sep;
    }
    else
    {
      sep_last->next = sep;
    }
    sep_last = sep;
    
    new_bsp->seq_data = NULL;
    new_bsp->seq_data_type = 0;
    new_bsp->repr = Seq_repr_delta;
    new_bsp->seq_ext_type = 4;
    new_bsp->mol = moltype;
    new_bsp->seq_ext = NULL;
    new_bsp->length = 0;
    next_start = (k + 1) * afp->num_segments;
    seq_ext = NULL;
    for (index = k * afp->num_segments; index < next_start; index++)
    {
      if (seq_ext != NULL)
      {
        /* insert gap of unknown length between the previous segment
         * and this one.
         */
        new_bsp->length += AddGapSeqLit (&seq_ext);
      }

      if (StringHasNoText (seq_str [index]))
      {
        /* add gap to represent missing sequence */
        new_bsp->length += AddGapSeqLit (&seq_ext);        
      }
      else
      {
        slip = (SeqLitPtr) MemNew (sizeof (SeqLit));
        if (slip != NULL) 
        {
          slip->length = StringLen (seq_str [index]);
          ValNodeAddPointer (&seq_ext, (Int2) 2, (Pointer) slip);
          slip->seq_data = BSNew (slip->length);
          slip->seq_data_type = Seq_code_iupacna;
          AddBasesToByteStore (slip->seq_data, seq_str [index]);
          new_bsp->length += slip->length;
        } 
      }
    }
    new_bsp->seq_ext = seq_ext;
    BioseqPack (new_bsp);        
  }
    
  return sep_list;
}

static SeqIdPtr GetFarPointerID (CharPtr id_str)
{
  CharPtr  tmp_id_str;
  CharPtr  cp_start, cp_end;
  Int4     len;
  SeqIdPtr sip;
  
  if (id_str == NULL)
  {
    return NULL;
  }

  cp_start = StringChr (id_str, '|');
  if (cp_start == NULL)
  {
    cp_start = id_str;
    len = StringLen (id_str);
  }
  else
  {
    cp_start++;
    cp_end = StringChr (cp_start, '|');
    if (cp_end == NULL)
    {
      len = StringLen (cp_start);
    }
    else
    {
      len = cp_end - cp_start;
    }
  }
  if (len == 0)
  {
    return NULL;
  }
  tmp_id_str = (CharPtr) MemNew ((len + 4) * sizeof (Char));
  if (tmp_id_str == NULL)
  {
    return NULL;
  }
  StringCpy (tmp_id_str, "acc");
  StringNCat (tmp_id_str, cp_start, len);
  tmp_id_str [len + 3] = 0;
  sip = MakeSeqID (tmp_id_str);
  MemFree (tmp_id_str);
  return sip;
}


extern SeqEntryPtr MakeSequinDataFromAlignmentEx (TAlignmentFilePtr afp, Uint1 moltype, Boolean check_ids) 
{
  SeqIdPtr    PNTR sip_list;
  SeqIdPtr    PNTR sip_prev;
  SeqAnnotPtr sap = NULL;
  SeqAlignPtr salp_list, salp_last;
  ValNodePtr  PNTR seqvnp;
  SeqEntryPtr sep_list;
  SeqEntryPtr sep, sep_prev;
  SeqIdPtr    sip;
  ValNodePtr  vnp;
  Int4        index, curr_seg, num_sets;
  BioseqPtr   bsp;
  CharPtr     tmp_id_str;
  MsgAnswer   ans;
  Int4Ptr      segs_per_set = NULL;
  Int4Ptr      segs_per_aln = NULL;
  Boolean      found_empty_seg = FALSE;
  CharPtr      seq_data = NULL;

  if (afp == NULL) return NULL;
  
  if (afp->num_sequences == 0) return NULL;
  if (afp->num_segments < 1) return NULL;
  
  sip_list = (SeqIdPtr PNTR) MemNew (afp->num_segments * sizeof (SeqIdPtr));  
  sip_prev = (SeqIdPtr PNTR) MemNew (afp->num_segments * sizeof (SeqIdPtr));  
  seqvnp = (ValNodePtr PNTR) MemNew (afp->num_segments * sizeof (ValNodePtr));  
  segs_per_set = (Int4Ptr) MemNew (sizeof (Int4Ptr) * afp->num_sequences);
  segs_per_aln = (Int4Ptr) MemNew (sizeof (Int4Ptr) * afp->num_segments);
  if (sip_list == NULL || sip_prev == NULL || seqvnp == NULL
      || segs_per_set == NULL || segs_per_aln == NULL)
  {
    MemFree (sip_list);
    MemFree (sip_prev);
  	MemFree (seqvnp);
  	MemFree (segs_per_set);
  	MemFree (segs_per_aln);
  	return NULL;
  }
 
  for (curr_seg = 0; curr_seg < afp->num_segments; curr_seg ++)
  {
    sip_list [curr_seg] = NULL;
    sip_prev [curr_seg] = NULL;
  	seqvnp [curr_seg] = NULL;
  	segs_per_aln [curr_seg] = 0;
  }

  sep_list = NULL;
  sep_prev = NULL;
  curr_seg = 0;

  for (index = 0; index < afp->num_sequences; index++) {
    seq_data = AlignmentStringToSequenceString (afp->sequences [index], moltype);
    if (StringHasNoText (seq_data))
    {
      found_empty_seg = TRUE;
    }
    else
    {
      sip = MakeSeqID (afp->ids [index]);
      if (sip != NULL)
      {
        sip->next = SeqIdFree (sip->next);
      }
      if (check_ids && StringNCmp (afp->ids[index], "acc", 3) != 0)
      {
        bsp = BioseqFind (sip);
        if (bsp == NULL)
        {
          sip = SeqIdFree (sip);
          tmp_id_str = (CharPtr) MemNew (sizeof (Char) * (StringLen (afp->ids [index]) + 4));
          sprintf (tmp_id_str, "gb|%s", afp->ids [index]);
          sip = MakeSeqID (tmp_id_str);
          MemFree (tmp_id_str);
          bsp = BioseqFind (sip);
        }
        if (bsp == NULL)
        {
          ans = Message (MSG_YN, "Can't find sequence %s in set - is this a far pointer?", afp->ids[index]);
          if (ans == ANS_YES)
          {
            sip = SeqIdFree (sip);
            sip = GetFarPointerID (afp->ids [index]);
          }
          else
          {
            sip = SeqIdFree (sip);
            sip = MakeSeqID (afp->ids [index]);
          }
          if (sip != NULL)
          {
            sip->next = SeqIdFree (sip->next);
          }
        }
      }

      sep = SequenceStringToSeqEntry (seq_data, sip, moltype);
      if (sep != NULL) {
        if (sep_list == NULL) {
          sep_list = sep;
        } else {
          sep_prev->next = sep;
        }
        sep_prev = sep;
        vnp = ValNodeNew (seqvnp[curr_seg]);
        if (seqvnp[curr_seg] == NULL) seqvnp[curr_seg] = vnp;
        vnp->data.ptrvalue = afp->sequences [index];
      
        /* only add SeqID to list if adding segment */
        if (sip_prev[curr_seg] == NULL) {
          sip_list[curr_seg] = sip;
        } else {
          sip_prev[curr_seg]->next = sip;
        }
        sip_prev[curr_seg] = sip;
        
        /* add to totals for this set and for this alignment */
        segs_per_set [index / afp->num_segments] ++;
        segs_per_aln [index % afp->num_segments] ++;
      }
    }
    seq_data = MemFree (seq_data);
    curr_seg ++;
    if (curr_seg >= afp->num_segments) 
    {
      curr_seg = 0;
    }
  }

  if (found_empty_seg)
  {
    Boolean   indexerVersion;
    MsgAnswer ans = ANS_YES;
    
    if (afp->num_segments > 1)
    {
      indexerVersion = (Boolean) (GetAppProperty ("InternalNcbiSequin") != NULL);
      if (indexerVersion)
      {
        ans = Message (MSG_YN, "This alignment of segmented sets contains a segment that is all gaps - do you wish to continue?");
      }
    }
    else
    {
      Message (MSG_ERROR, "This alignment contains a sequence that is all gaps.");
      ans = ANS_NO;
    }
    if (ans == ANS_NO)
    {
      for (curr_seg = 0; curr_seg < afp->num_segments; curr_seg ++)
      {
        ValNodeFree (seqvnp [curr_seg]);
      }
      MemFree (seqvnp);
      MemFree (sip_list);
      MemFree (sip_prev);
      MemFree (segs_per_set);
      MemFree (segs_per_aln);
      sep_list = SeqEntryFree (sep_list);
      return NULL;
    }
  }

  
  if (afp->num_segments == 1) 
  {
    sap = LocalAlignToSeqAnnotDimn (seqvnp[0], sip_list[0], NULL, afp->num_sequences,
                                    0, NULL, FALSE);
    sep_list = make_seqentry_for_seqentry (sep_list);
    SeqAlignAddInSeqEntry (sep_list, sap);  	
  } 
  else 
  {
    sep_list = MakeSegmentedSetFromAlignment (sep_list, afp, moltype, segs_per_set);
    sep_list = make_seqentry_for_seqentry (sep_list);
    num_sets = afp->num_sequences / afp->num_segments;
    salp_list = NULL;
    salp_last = NULL;

    for (curr_seg = 0; curr_seg < afp->num_segments; curr_seg++)
    {      
      sap = LocalAlignToSeqAnnotDimn (seqvnp[curr_seg], sip_list[curr_seg], NULL, segs_per_aln [curr_seg],
                                    0, NULL, FALSE);
      if (sap != NULL)
      {
        SeqAlignAddInSeqEntry (sep_list, sap);
      }
    }
  }

  for (curr_seg = 0; curr_seg < afp->num_segments; curr_seg ++)
  {
    ValNodeFree (seqvnp [curr_seg]);
  }
  MemFree (seqvnp);
  MemFree (sip_list);
  MemFree (sip_prev);
  MemFree (segs_per_set);
  MemFree (segs_per_aln);

  AddDefLinesToAlignmentSequences (afp, sep_list);

  return sep_list;
}

extern SeqEntryPtr MakeSequinDataFromAlignment (TAlignmentFilePtr afp, Uint1 moltype) 
{
  return MakeSequinDataFromAlignmentEx (afp, moltype, FALSE);
}

/* Create sequences and alignment annotation */

/**********************************************************/
extern SeqEntryPtr make_seqentry_for_seqentry (SeqEntryPtr sep)
{
  SeqEntryPtr  sep1 = NULL,
               tmp;
  BioseqPtr    bsp;
  BioseqSetPtr bssp;
 
  if (sep == NULL) return NULL;

  if (! IS_Bioseq (sep) && ! IS_Bioseq_set (sep)) {
    return sep;
  } else if (sep->next == NULL) {
    return sep;
  } else if ((bssp = BioseqSetNew ()) == NULL) {
    return sep;
  } else {
    bssp->_class = 14;
    bssp->seq_set = sep;
    sep1 = SeqEntryNew ();
    sep1->choice = 2;
    sep1->data.ptrvalue = bssp;
    SeqMgrLinkSeqEntry (sep1, 0, NULL);
          
    for (tmp = bssp->seq_set; tmp!=NULL; tmp=tmp->next) {
      if (IS_Bioseq(tmp)) {
        bsp = (BioseqPtr) tmp->data.ptrvalue;
        ObjMgrConnect (OBJ_BIOSEQ, (Pointer) bsp, OBJ_BIOSEQSET, (Pointer) bssp);
      }
    }
  }
  return sep1;
}

/* These two functions are used for removing mRNAs that overlap pseudo misc_feats
 * and marking genes that overlap pseudo misc_feats as pseudo.
 */
static void PseudoMiscFeatProcessingCallback (SeqFeatPtr sfp, Pointer userdata)
{
  SeqFeatPtr        gene, mRNA;
  SeqMgrFeatContext gcontext, mcontext;
  
  if (sfp == NULL || sfp->idx.subtype != FEATDEF_misc_feature) return;
  /* we only want to process misc_feats if the pseudo flag is set or the 
   * comment contains the word "pseudogene".
   */
#if 0
  if (!sfp->pseudo && StringISearch (sfp->comment, "pseudogene") == NULL) return;
#endif

  gene = SeqMgrGetOverlappingGene (sfp->location, &gcontext);
  mRNA = SeqMgrGetOverlappingFeature (sfp->location, FEATDEF_mRNA, NULL, 0, NULL,
                                      RANGE_MATCH, &mcontext);
  if (gene != NULL)
  {
  	gene->pseudo = TRUE;
  }
  if (mRNA != NULL && mRNA->product == NULL) /* only delete mRNAs without products */
  {
  	mRNA->idx.deleteme = TRUE;
  }  
}

extern void ProcessPseudoMiscFeatsForEntityID (Uint2 entityID)
{
  SeqEntryPtr sep;
  
  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep == NULL) return;
  
  VisitFeaturesInSep (sep, (Pointer) NULL, PseudoMiscFeatProcessingCallback);
  DeleteMarkedObjects (entityID, 0, NULL);  	
}

/* These two functions are used for converting pseudo CDSs to misc_features. */
static void ConvertPseudoCDSToMiscFeatCallback (SeqFeatPtr sfp, Pointer userdata)
{
  BioseqPtr  bsp;
  SeqFeatPtr new_sfp;
  ImpFeatPtr ifp;
  
  if (sfp == NULL || (sfp->data.choice != SEQFEAT_CDREGION) || (! sfp->pseudo)) return;
  
  bsp = BioseqFindFromSeqLoc (sfp->location);  
  if (bsp == NULL) return;
  ifp = ImpFeatNew ();
  if (ifp == NULL) return;
  new_sfp = CreateNewFeatureOnBioseq (bsp, SEQFEAT_IMP, sfp->location);
  if (new_sfp == NULL) 
  {
  	ImpFeatFree (ifp);
  	return;
  }
  new_sfp->data.value.ptrvalue = (Pointer) ifp;
  ifp->key = StringSave ("misc_feature");
  new_sfp->comment = sfp->comment;
  sfp->comment = NULL;
  new_sfp->qual = sfp->qual;
  sfp->qual = NULL;
  
  if (sfp->product != NULL)
  {
  	bsp = BioseqFindFromSeqLoc (sfp->product);
  	sfp->product = SeqLocFree (sfp->product);
  	bsp->idx.deleteme = TRUE;
  }
  sfp->idx.deleteme = TRUE;
}

extern void ConvertPseudoCDSToMiscFeatsForEntityID (Uint2 entityID)
{
  SeqEntryPtr sep;
  
  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep == NULL) return;
  
  VisitFeaturesInSep (sep, (Pointer) NULL, ConvertPseudoCDSToMiscFeatCallback);
  DeleteMarkedObjects (entityID, 0, NULL);  	
}

typedef struct alignmentforbsp
{
  BioseqPtr   bsp;
  SeqAlignPtr salp_list;
  SeqAlignPtr salp_last;
  ValNodePtr  seq_annot_list;
} AlignmentForBspData, PNTR AlignmentForBspPtr;

static void FindAlignmentsForBioseqCallback (SeqAnnotPtr sap, Pointer userdata)
{
  AlignmentForBspPtr   afbp;
  SeqAlignPtr          salp;
  SeqIdPtr             sip;
  Boolean              found = FALSE;

  if (sap == NULL || sap->type != 2 || userdata == NULL) 
  {
    return;
  }
  afbp = (AlignmentForBspPtr) userdata;
  if (afbp->bsp == NULL)
  {
    return;
  }
  salp = (SeqAlignPtr) sap->data;
  if (salp == NULL) return;
  for (sip = afbp->bsp->id; sip != NULL && !found; sip = sip->next)
  {
    if (SeqAlignFindSeqId (salp, sip))
    {
      salp = SeqAlignListDup(salp);
      AlnMgr2IndexSeqAlign(salp);
      if (afbp->salp_last == NULL)
      {
        afbp->salp_list = salp; 
      }
      else
      {
        afbp->salp_last->next = salp;
      }
      afbp->salp_last = salp;
      found = TRUE;
    }
  }
}

extern SeqAlignPtr FindAlignmentsForBioseq (BioseqPtr bsp)
{
  SeqEntryPtr         topsep;
  AlignmentForBspData afbd;
  SeqLocPtr           slp;
  SeqIdPtr            sip;
  
  if (bsp == NULL) return NULL;
  topsep = GetTopSeqEntryForEntityID (bsp->idx.entityID);
  afbd.salp_list = NULL;
  afbd.salp_last = NULL;
  if (bsp->repr == Seq_repr_seg)
  {
    for (slp = bsp->seq_ext; slp != NULL; slp = slp->next)
    {
      sip = SeqLocId (slp);
      afbd.bsp = BioseqFind (sip);
      VisitAnnotsInSep (topsep, &afbd, FindAlignmentsForBioseqCallback);
    }
  }
  else
  {
    afbd.bsp = bsp;
    VisitAnnotsInSep (topsep, &afbd, FindAlignmentsForBioseqCallback);
  }
  
  return afbd.salp_list;
}

static void FindAlignSeqAnnotsForBioseqCallback (SeqAnnotPtr sap, Pointer userdata)
{
  AlignmentForBspPtr   afbp;
  SeqAlignPtr          salp;
  SeqIdPtr             sip;
  Boolean              found = FALSE;

  if (sap == NULL || sap->type != 2 || userdata == NULL) 
  {
    return;
  }
  afbp = (AlignmentForBspPtr) userdata;
  if (afbp->bsp == NULL)
  {
    return;
  }
  salp = (SeqAlignPtr) sap->data;
  if (salp == NULL) return;
  for (sip = afbp->bsp->id; sip != NULL && !found; sip = sip->next)
  {
    if (SeqAlignFindSeqId (salp, sip))
    {
      ValNodeAddPointer (&(afbp->seq_annot_list), 0, sap);
      found = TRUE;
    }
  }
}

extern ValNodePtr FindAlignSeqAnnotsForBioseq (BioseqPtr bsp)
{
  SeqEntryPtr         topsep;
  AlignmentForBspData afbd;
  SeqLocPtr           slp;
  SeqIdPtr            sip;
  
  if (bsp == NULL) return NULL;
  topsep = GetTopSeqEntryForEntityID (bsp->idx.entityID);
  afbd.salp_list = NULL;
  afbd.salp_last = NULL;
  afbd.seq_annot_list = NULL;
  if (bsp->repr == Seq_repr_seg)
  {
    for (slp = bsp->seq_ext; slp != NULL; slp = slp->next)
    {
      sip = SeqLocId (slp);
      afbd.bsp = BioseqFind (sip);
      VisitAnnotsInSep (topsep, &afbd, FindAlignSeqAnnotsForBioseqCallback);
    }
  }
  else
  {
    afbd.bsp = bsp;
    VisitAnnotsInSep (topsep, &afbd, FindAlignSeqAnnotsForBioseqCallback);
  }
  
  return afbd.seq_annot_list;
}

NLM_EXTERN void ChangeSeqIdToWorstID (SeqIdPtr sip)
{
  BioseqPtr       bsp;
  SeqIdPtr        id;
  Pointer         pnt;

  if (sip == NULL)
    return;
  bsp = BioseqFindCore (sip);
  if (bsp == NULL)
    return;
  id = SeqIdDup (SeqIdFindWorst (bsp->id));
  if (id == NULL)
    return;
  /* now remove SeqId contents to reuse SeqId valnode */
  pnt = sip->data.ptrvalue;
  switch (sip->choice) {
  case SEQID_LOCAL:            /* local */
    ObjectIdFree ((ObjectIdPtr) pnt);
    break;
  case SEQID_GIBBSQ:           /* gibbseq */
  case SEQID_GIBBMT:           /* gibbmt */
    break;
  case SEQID_GIIM:             /* giimid */
    GiimFree ((GiimPtr) pnt);
    break;
  case SEQID_GENBANK:          /* genbank */
  case SEQID_EMBL:             /* embl */
  case SEQID_PIR:              /* pir   */
  case SEQID_SWISSPROT:        /* swissprot */
  case SEQID_OTHER:            /* other */
  case SEQID_DDBJ:
  case SEQID_PRF:
  case SEQID_TPG:
  case SEQID_TPE:
  case SEQID_TPD:
  case SEQID_GPIPE:
    TextSeqIdFree ((TextSeqIdPtr) pnt);
    break;
  case SEQID_PATENT:           /* patent seq id */
    PatentSeqIdFree ((PatentSeqIdPtr) pnt);
    break;
  case SEQID_GENERAL:          /* general */
    DbtagFree ((DbtagPtr) pnt);
    break;
  case SEQID_GI:               /* gi */
    break;
  case SEQID_PDB:
    PDBSeqIdFree ((PDBSeqIdPtr) pnt);
    break;
  }
  sip->choice = id->choice;
  sip->data.ptrvalue = id->data.ptrvalue;
  SeqIdStripLocus (sip);
}

NLM_EXTERN void ChangeSeqLocToWorstID (SeqLocPtr slp)
{
  SeqLocPtr       loc;
  PackSeqPntPtr   psp;
  SeqBondPtr      sbp;
  SeqIntPtr       sinp;
  SeqIdPtr        sip;
  SeqPntPtr       spp;

  while (slp != NULL) {
    switch (slp->choice) {
    case SEQLOC_NULL:
      break;
    case SEQLOC_EMPTY:
    case SEQLOC_WHOLE:
      sip = (SeqIdPtr) slp->data.ptrvalue;
      ChangeSeqIdToWorstID (sip);
      break;
    case SEQLOC_INT:
      sinp = (SeqIntPtr) slp->data.ptrvalue;
      if (sinp != NULL) {
        sip = sinp->id;
        ChangeSeqIdToWorstID (sip);
      }
      break;
    case SEQLOC_PNT:
      spp = (SeqPntPtr) slp->data.ptrvalue;
      if (spp != NULL) {
        sip = spp->id;
        ChangeSeqIdToWorstID (sip);
      }
      break;
    case SEQLOC_PACKED_PNT:
      psp = (PackSeqPntPtr) slp->data.ptrvalue;
      if (psp != NULL) {
        sip = psp->id;
        ChangeSeqIdToWorstID (sip);
      }
      break;
    case SEQLOC_PACKED_INT:
    case SEQLOC_MIX:
    case SEQLOC_EQUIV:
      loc = (SeqLocPtr) slp->data.ptrvalue;
      while (loc != NULL) {
        ChangeSeqLocToWorstID (loc);
        loc = loc->next;
      }
      break;
    case SEQLOC_BOND:
      sbp = (SeqBondPtr) slp->data.ptrvalue;
      if (sbp != NULL) {
        spp = (SeqPntPtr) sbp->a;
        if (spp != NULL) {
          sip = spp->id;
          ChangeSeqIdToWorstID (sip);
        }
        spp = (SeqPntPtr) sbp->b;
        if (spp != NULL) {
          sip = spp->id;
          ChangeSeqIdToWorstID (sip);
        }
      }
      break;
    case SEQLOC_FEAT:
      break;
    default:
      break;
    }
    slp = slp->next;
  }
}

/* This function will remove DenDiag and pairwise alignments if they contain
 * the sequence identified by sip, otherwise it will remove the sequence from
 * the alignment.
 */
static SeqAlignPtr RemoveOneSequenceFromAlignment (SeqIdPtr sip, SeqAlignPtr salphead)
{
  Uint4       seqid_order;
  SeqIdPtr    tmpsip;
  SeqAlignPtr salp, salp_next, prev_salp, remove_salp, last_remove;
  
  if (!FindSeqIdinSeqAlign (salphead, sip)) return NULL;
  
  salp = salphead;
  prev_salp = NULL;
  remove_salp = NULL;
  last_remove = NULL;
  while (salp != NULL)
  {
    salp_next = salp->next;
    tmpsip = SeqIdPtrFromSeqAlign (salp);
    seqid_order = SeqIdOrderInBioseqIdList(sip, tmpsip);
    if (seqid_order == 0)
    {
      /* do nothing for this subalignment */
      prev_salp = salp;
    }
    else if (salp->dim == 2 || salphead->segtype ==1)
    {
      /* This is for a pairwise alignment or a DENDIAG alignment */
      if (prev_salp == NULL)
      {
      	salphead = salp->next;
      }
      else
      {
      	prev_salp->next = salp->next;
      }
      /* save the alignments that we want to free in a list and get rid of them
       * at the end - freeing them beforehand causes problems with listing the
       * IDs in the alignment.
       */
      salp->next = NULL;
      if (remove_salp == NULL)
      {
      	remove_salp = salp;
      }
      else
      {
      	last_remove->next = salp;
      }
      last_remove = salp;
    }
    else 
    {
      SeqAlignBioseqDeleteById (salphead, sip);  
      prev_salp = salp;
    }
    salp = salp_next;
  }
  /* Now we can free the alignment */
  SeqAlignFree (remove_salp);
  return salphead;
}

static void RemoveSequenceFromAlignmentsCallback (SeqAnnotPtr sap, Pointer userdata)
{
  SeqAlignPtr salp;
  SeqIdPtr    sip;

  if (sap == NULL || sap->type != 2 || userdata == NULL) return;
  salp = (SeqAlignPtr) sap->data;
  if (salp == NULL) return;
  sip = (SeqIdPtr) userdata;
  sap->data = RemoveOneSequenceFromAlignment (sip, salp);
  /* if we've deleted all of the alignments, get rid of the annotation as well */
  if (sap->data == NULL)
  {
  	sap->idx.deleteme = TRUE;
  }
}

typedef struct checkforremovesequencefromalignments
{
  Boolean  found_problem;
  SeqIdPtr sip;
} CheckForRemoveSequenceFromAlignmentsData, PNTR CheckForRemoveSequenceFromAlignmentsPtr;

/* This is the callback function for looking for pairwise alignments.
 * If we delete the first sequence in a pairwise alignment, we end up deleting
 * the entire alignment because that sequence is paired with every other sequence.
 */
static void CheckForRemoveSequenceFromAlignmentsProblemsCallback (SeqAnnotPtr sap, Pointer userdata)
{
  CheckForRemoveSequenceFromAlignmentsPtr p;
  SeqAlignPtr salphead, salp;
  Uint4       seqid_order;
  SeqIdPtr    tmpsip;
  
  if (sap == NULL || sap->type != 2
      || (p = (CheckForRemoveSequenceFromAlignmentsPtr)userdata) == NULL
      || p->found_problem)
  {
  	return;
  }
  salphead = (SeqAlignPtr) sap->data;
  if (salphead == NULL) return;
  
  if (!FindSeqIdinSeqAlign (salphead, p->sip))
  {
  	return;
  }
  for (salp = salphead; salp != NULL; salp = salp->next)
  {
    tmpsip = SeqIdPtrFromSeqAlign (salp);
    seqid_order = SeqIdOrderInBioseqIdList(p->sip, tmpsip);
    if (seqid_order == 0)
    {
      continue;
    }
    else if (seqid_order == 1 && salp->dim == 2)
    {
      p->found_problem = TRUE;      
    }
  }
}

extern Boolean IsSequenceFirstInPairwise (SeqEntryPtr sep, SeqIdPtr sip)
{
  CheckForRemoveSequenceFromAlignmentsData data;
  
  if (sep == NULL || sip == NULL)
  {
    return FALSE;
  }
  
	data.sip = sip;
	data.found_problem = FALSE;
  
  VisitAnnotsInSep (sep, (Pointer) &data, CheckForRemoveSequenceFromAlignmentsProblemsCallback);
  return data.found_problem;
}

extern Boolean RemoveSequenceFromAlignments (SeqEntryPtr sep, SeqIdPtr sip)
{
  if (sep == NULL || sip == NULL)
  {
    return FALSE;
  }
  if (IsSequenceFirstInPairwise (sep, sip))
  {
    return FALSE;
  }
  VisitAnnotsInSep (sep, (Pointer) sip, RemoveSequenceFromAlignmentsCallback);
  return TRUE;
}
