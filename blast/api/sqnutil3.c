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
* $Revision: 6.13 $
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

#define num_site 26
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
  "transmembrane-region"
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
    if (EmptyOrNullString (grp->locus) &&
        EmptyOrNullString (grp->allele) &&
        EmptyOrNullString (grp->desc) &&
        EmptyOrNullString (grp->maploc) &&
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
  { FEATDEF_GC_signal ,          "GC_signal"          },
  { FEATDEF_GENE ,               "Gene"               },
  { FEATDEF_HET ,                "Het"                },
  { FEATDEF_iDNA ,               "iDNA"               },
  { FEATDEF_IMP ,                "Imp"                },
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
  "Imp" ,
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
  "Protein" ,
  "mat_peptide" ,
  "sig_peptide" ,
  "transit_peptide",
  "snoRNA"
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
    } else if (type == FEATDEF_REGION || type == FEATDEF_COMMENT || type == FEATDEF_SITE) {
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

