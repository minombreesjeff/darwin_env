/*   sequin3.c
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
* File Name:  sequin3.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   1/22/95
*
* $Revision: 6.550 $
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

#include "sequin.h"
#include <document.h>
#include <sequtil.h>
#include <biosrc.h>
#include <import.h>
#include <gather.h>
#include <asn2gnbk.h>
#include <asn2gnbp.h>
#include <edutil.h>
#include <gbfeat.h>
#include <gbftdef.h>
#include <subutil.h>    /* TOPOLOGY_xxx definitions */
#include <salutil.h>
#include <valid.h>
#include <vsm.h>
#include <bspview.h>
#include <toasn3.h>
#include <salstruc.h>
#include <explore.h>
#include <utilpub.h>
#include <tofasta.h>
#include <rpsutil.h>
#include <findrepl.h>
#include <explore.h>
#include <seqpanel.h>
#include <tax3api.h>
#include <saledit.h>
#include <alignmgr2.h>

typedef struct {
  Int2    level;
  Boolean prevWasModified;
} VecToNData, PNTR VecToNDataPtr;

typedef struct {
  Boolean dirtyFlag;
  Int2    number;
} CdsDelStruct, PNTR CdsDelStructPtr;

typedef struct {
  FORM_MESSAGE_BLOCK
  Int2  number;
} CdsDelForm, PNTR CdsDelFormPtr;

static void AddOrgNameToDefLines (IteM i)

{
  CommonAddOrgOrModsToDefLines (i, 0, 0, NULL);
}

static void AddStrainToDefLines (IteM i)

{
  CommonAddOrgOrModsToDefLines (i, ORGMOD_strain, 0, NULL);
}

static void AddCloneToDefLines (IteM i)

{
  CommonAddOrgOrModsToDefLines (i, 0, SUBSRC_clone, NULL);
}

static void AddIsolateToDefLines (IteM i)

{
  CommonAddOrgOrModsToDefLines (i, ORGMOD_isolate, 0, NULL);
}

static void AddHaplotypeToDefLines (IteM i)

{
  CommonAddOrgOrModsToDefLines (i, 0, SUBSRC_haplotype, NULL);
}

static void AddCultivarToDefLines (IteM i)

{
  CommonAddOrgOrModsToDefLines (i, ORGMOD_cultivar, 0, NULL);
}

static Boolean FindBestCitSubCallback (GatherContextPtr gcp)

{
  CitSubPtr   best;
  CitSubPtr   PNTR bestp;
  CitSubPtr   csp;
  PubdescPtr  pdp;
  ValNodePtr  sdp;
  ValNodePtr  vnp;

  if (gcp == NULL) return TRUE;
  bestp = (CitSubPtr PNTR) gcp->userdata;
  if (bestp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQDESC) return TRUE;
  sdp = (ValNodePtr) gcp->thisitem;
  if (sdp == NULL || sdp->choice != Seq_descr_pub) return TRUE;
  pdp = (PubdescPtr) sdp->data.ptrvalue;
  if (pdp == NULL) return TRUE;
  vnp = pdp->pub;
  if (vnp == NULL || vnp->choice != PUB_Sub) return TRUE;
  csp = (CitSubPtr) vnp->data.ptrvalue;
  if (csp == NULL) return TRUE;
  if (*bestp == NULL) {
    *bestp = csp;
    return TRUE;
  }
  best = *bestp;
  if (DateMatch (best->date, csp->date, FALSE) == -1) {
    *bestp = csp;
    return TRUE;
  }
  return TRUE;
}

extern Boolean CreateUpdateCitSubFromBestTemplate (
  SeqEntryPtr top_sep,
  SeqEntryPtr upd_sep
)
{
  CitSubPtr    best;
  CitSubPtr    csp;
  DatePtr      dp;
  GatherScope  gs;
  PubdescPtr   pdp;
  ValNodePtr   sdp;
  ValNodePtr   vnp;

  best = NULL;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQDESC] = FALSE;
  gs.scope = top_sep;
  GatherSeqEntry (top_sep, (Pointer) (&best), FindBestCitSubCallback, &gs);
  if (best == NULL) {
    Message (MSG_OK, "There is no earlier cit-sub template");
    return FALSE;
  }
  dp = DateCurr ();
  if (dp != NULL) {
    if (DateMatch (best->date, dp, FALSE) == 0) {
      DateFree (dp);
	  dp = NULL;
      if (upd_sep == top_sep
        &&StringICmp (best->descr, "Sequence update by submitter") == 0)
      {
        Message (MSG_OK, "There already exists an update on today's date");
        Update ();
        return FALSE;
      } else if (best->descr == NULL) {
        best->descr = StringSave ("Sequence update by submitter");
        Message (MSG_OK, "Adding update indication to existing cit-sub");
        return TRUE;
      }
    }
    DateFree (dp);
  }
  sdp = CreateNewDescriptor (upd_sep, Seq_descr_pub);
  if (sdp == NULL) return FALSE;
  pdp = PubdescNew ();
  if (pdp == NULL) return FALSE;
  sdp->data.ptrvalue = (Pointer) pdp;
  vnp = ValNodeNew (NULL);
  csp = AsnIoMemCopy ((Pointer) best,
                      (AsnReadFunc) CitSubAsnRead,
                      (AsnWriteFunc) CitSubAsnWrite);
  pdp->pub = vnp;
  vnp->choice = PUB_Sub;
  vnp->data.ptrvalue = csp;
  csp->date = DateFree (csp->date);
  csp->date = DateCurr ();
  csp->descr = MemFree (csp->descr);
  csp->descr = StringSave ("Sequence update by submitter");
  if (top_sep == upd_sep)
  {
    Message (MSG_OK, "The update Cit-sub has been placed on the top Seq-entry");
  }
  return TRUE;
}                               

static void AddCitSubForUpdateProc (BaseFormPtr bfp)

{
  SeqEntryPtr  sep;

  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  if ( CreateUpdateCitSubFromBestTemplate (sep, sep))
  {
    ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
    Update ();
  }
  return;
}

static void AddCitSubForUpdate (IteM i)

{
  BaseFormPtr  bfp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  AddCitSubForUpdateProc (bfp);
}

/* FixLocus is copied from taxutil, then modified to remove embl_code use */
static void FixLocus (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	BioseqPtr bsp;
	ValNodePtr vnp;
    TextSeqIdPtr tsip;
    Char tbuf[40];
	CharPtr ptr, embl_code;

	if (! IS_Bioseq(sep))
		return;

	bsp = (BioseqPtr)(sep->data.ptrvalue);
    embl_code = (CharPtr) data;
	/* if (! embl_code) return; */

    for (vnp = bsp->id; vnp != NULL; vnp = vnp->next) {
		if ((vnp->choice == SEQID_GENBANK || vnp->choice == SEQID_TPG) && vnp->data.ptrvalue != NULL) {
			tsip = (TextSeqIdPtr) (vnp->data.ptrvalue);
			if (tsip->accession != NULL)
			{
				tsip->name = MemFree (tsip->name);
				if (bsp->repr != Seq_repr_seg) {
					/* ptr = StringMove (tbuf, embl_code); */
					ptr = tbuf;
					StringMove (ptr, tsip->accession);
					tsip->name = StringSave (tbuf);
					SeqMgrReplaceInBioseqIndex (bsp);
				}
				return;
			}
		}
	}
	
	return;
}

extern void DoFixupLocus (SeqEntryPtr sep);
extern void DoFixupLocus (SeqEntryPtr sep)

{
  BioSourcePtr  biop;
  BioseqSetPtr  bssp;
  CharPtr       embl_code;
  OrgRefPtr     orp;
  ValNodePtr    sdp;

  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (IsPopPhyEtcSet (bssp->_class)))) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        DoFixupLocus (sep);
      }
      return;
    }
  }
  sdp = SeqEntryGetSeqDescr (sep, Seq_descr_source, NULL);
  if (sdp == NULL) return;
  biop = (BioSourcePtr) sdp->data.ptrvalue;
  if (biop == NULL) return;
  orp = biop->org;
  if (orp == NULL) return;
  /* embl_code = get_embl_code (orp); */
  embl_code = NULL;
  SeqEntryExplore (sep, (Pointer) embl_code, FixLocus);
  MemFree (embl_code);
}

static TextSeqIdPtr GetTSIPforBSP (BioseqPtr bsp)

{
  SeqIdPtr      sip;
  TextSeqIdPtr  tsip = NULL;

  if (bsp == NULL) return NULL;

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_GENBANK || sip->choice == SEQID_TPG) {
      tsip = (TextSeqIdPtr) sip->data.ptrvalue;
    }
  }

  return tsip;
}

static void DoFixPartLocus (SeqEntryPtr sep, CharPtr prefix, Int2 segment, Int2 digits)

{
  BioseqPtr     bsp;
  Char          ch;
  Char          digs [16];
  CharPtr       ptr;
  Char          str [32];
  TextSeqIdPtr  tsip;

  if (sep == NULL || prefix == NULL) return;
  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;
  if (bsp->repr != Seq_repr_raw && bsp->repr != Seq_repr_const) return;
  tsip = GetTSIPforBSP (bsp);
  if (tsip == NULL) return;
  tsip->name = MemFree (tsip->name);
  sprintf (digs, "%*d", (int) digits, (int) segment);
  ptr = digs;
  ch = *ptr;
  while (ch != '\0') {
    if (ch == ' ') {
      *ptr = '0';
    }
    ptr++;
    ch = *ptr;
  }
  sprintf (str, "%sS%s", prefix, digs);
  tsip->name = StringSave (str);
}

extern void DoFixupSegSet (SeqEntryPtr sep);
extern void DoFixupSegSet (SeqEntryPtr sep)

{
  CharPtr       accn;
  Uint1         chs;
  Int2          digits;
  BioseqPtr     nbsp;
  BioseqSetPtr  bssp;
  size_t        len;
  SeqEntryPtr   nsep;
  Int2          numsegs;
  BioseqSetPtr  pbssp;
  CharPtr       prefix;
  SeqEntryPtr   psep;
  BioseqPtr     sbsp;
  Int2          segment;
  SeqIdPtr      sip;
  Char          str [32];
  Char          tmp [128];
  TextSeqIdPtr  tsip;

  if (! IS_Bioseq_set (sep)) return;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp == NULL) return;
  if (bssp->_class != BioseqseqSet_class_segset) {
    for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
      DoFixupSegSet (sep);
    }
    return;
  }
  nsep = FindNucSeqEntry (sep);
  if (nsep == NULL) return;
  if (! IS_Bioseq (nsep)) return;
  nbsp = (BioseqPtr) nsep->data.ptrvalue;
  if (nbsp == NULL) return;
  if (nbsp->repr != Seq_repr_seg) return;
  sbsp = NULL;
  tsip = GetTSIPforBSP (nbsp);
  if (tsip == NULL || StringHasNoText (tsip->accession)) {
    sbsp = nbsp;
    nsep = FindNthSeqEntry (sep, 4);
    if (nsep == NULL) return;
    if (! IS_Bioseq (nsep)) return;
    nbsp = (BioseqPtr) nsep->data.ptrvalue;
    if (nbsp == NULL) return;
    if (nbsp->repr != Seq_repr_raw && nbsp->repr != Seq_repr_const) return;
    tsip = GetTSIPforBSP (nbsp);
  }
  if (tsip == NULL) return;
  chs = SEQID_GENBANK;
  for (sip = nbsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_GENBANK || sip->choice == SEQID_TPG) {
      chs = sip->choice;
    }
  }
  accn = tsip->accession;
  if (StringHasNoText (accn)) return;
  psep = FindBioseqSetByClass (sep, BioseqseqSet_class_parts);
  if (psep == NULL) return;
  pbssp = (BioseqSetPtr) psep->data.ptrvalue;
  if (pbssp == NULL) return;
  for (sep = pbssp->seq_set, numsegs = 0; sep != NULL; sep = sep->next, numsegs++) continue;
  StringNCpy_0 (tmp, accn, sizeof (tmp));
  prefix = tmp;
  len = StringLen (prefix);
  if (numsegs > 999) {
    digits = 4;
  } else if (numsegs > 99) {
    digits = 3;
  } else if (numsegs > 9) {
    digits = 2;
  } else {
    digits = 1;
  }
  if (digits + 1 + len > 16) {
    prefix += digits + 1 + len - 16;
  }
  for (sep = pbssp->seq_set, segment = 1; sep != NULL; sep = sep->next, segment++) {
    DoFixPartLocus (sep, prefix, segment, digits);
  }
  if (sbsp == NULL) return;
  tsip = NULL;
  for (sip = sbsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_GENBANK || sip->choice == SEQID_TPG) {
      tsip = (TextSeqIdPtr) sip->data.ptrvalue;
    }
  }
  if (tsip == NULL) {
    tsip = TextSeqIdNew ();
    if (tsip == NULL) return;
    ValNodeAddPointer (&(sbsp->id), chs, (Pointer) tsip);
  }
  tsip->name = MemFree (tsip->name);
  sprintf (str, "SEG_%sS", prefix);
  tsip->name = StringSave (str);
}

static void ForceLocusFixup (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;
  ErrSev       sev;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  sev = ErrSetMessageLevel (SEV_FATAL);
  WatchCursor ();
  Update ();
  DoFixupLocus (sep);
  DoFixupSegSet (sep);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();
  ErrSetMessageLevel (sev);
  ErrClear ();
  ErrShow ();
}

extern void GetRidOfRedundantSourceNotes (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent);

static void CleanupGenbankBlockCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  Boolean        empty;
  GBBlockPtr     gbp;
  ValNodePtr     nextsdp;
  Pointer PNTR   prevsdp;
  ValNodePtr     sdp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sdp = bsp->descr;
    prevsdp = (Pointer PNTR) &(bsp->descr);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sdp = bssp->descr;
    prevsdp = (Pointer PNTR) &(bssp->descr);
  } else return;
  while (sdp != NULL) {
    nextsdp = sdp->next;
    empty = FALSE;
    if (sdp->choice == Seq_descr_genbank && sdp->data.ptrvalue != NULL) {
      gbp = (GBBlockPtr) sdp->data.ptrvalue;
      gbp->source = MemFree (gbp->source); /* remove within Sequin */
      gbp->origin = MemFree (gbp->origin);
      gbp->taxonomy = MemFree (gbp->taxonomy);
      if (gbp->extra_accessions == NULL && gbp->source == NULL &&
          gbp->keywords == NULL && gbp->origin == NULL &&
          gbp->date == NULL && gbp->entry_date == NULL &&
          gbp->div == NULL && gbp->taxonomy == NULL) {
        empty = TRUE;
      }
    }
    if (empty) {
      *(prevsdp) = sdp->next;
      sdp->next = NULL;
      SeqDescFree (sdp);
    } else {
      prevsdp = (Pointer PNTR) &(sdp->next);
    }
    sdp = nextsdp;
  }
}

static void CheckBioSourceForEnvironmentalSample (BioSourcePtr biop, Pointer userdata)
{
  BoolPtr bp;
  
  if (biop == NULL || userdata == NULL) return;
  
  bp = (BoolPtr) userdata;
  if (! *bp) return;
  
  if (biop->org == NULL || biop->org->orgname == NULL)
  {
  	*bp = FALSE;
  	return;
  }
  if (StringSearch (biop->org->orgname->lineage, "environmental sample") == NULL)
  {
  	*bp = FALSE;
  }
}

static Boolean CheckForEnvironmentalSample (BioseqSetPtr bssp)
{
  Boolean all_enviro = TRUE;
  
  if (bssp == NULL) return FALSE;
  
  VisitBioSourcesInSet (bssp, (Pointer) &all_enviro, CheckBioSourceForEnvironmentalSample);
   
  return all_enviro; 
}


static void ConvertToEcoSets (SeqEntryPtr sep)
{
  BioseqSetPtr bssp;
  SeqEntryPtr  sep_entry;
  
  if (sep == NULL) return;
  
  if (!IS_Bioseq_set (sep)) return;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp == NULL) return;
  
  if (bssp->_class == BioseqseqSet_class_mut_set
      || bssp->_class == BioseqseqSet_class_pop_set 
      || bssp->_class == BioseqseqSet_class_phy_set)
  {
  	if (CheckForEnvironmentalSample (bssp))
  	{
  	  bssp->_class = BioseqseqSet_class_eco_set;
  	}
  }
  for (sep_entry = bssp->seq_set; sep_entry != NULL; sep_entry = sep_entry->next)
  {
  	if (IS_Bioseq_set (sep_entry))
  	{
  	  ConvertToEcoSets (sep_entry);
  	}
  }
}

static void AddEnvironmentalSampleQual (BioSourcePtr biop, Pointer userdata)
{
  SubSourcePtr ssp;
  
  if (biop == NULL || biop->org == NULL || biop->org->orgname == NULL) return;

  if (StringSearch (biop->org->orgname->lineage, "environmental sample") == NULL)
  {
  	return;
  }
  
  ssp = biop->subtype;
  while (ssp != NULL && ssp->subtype != SUBSRC_environmental_sample)
  {
    ssp = ssp->next;
  }
  if (ssp == NULL)
  {
    ssp = SubSourceNew ();
    if (ssp != NULL) {
      ssp->subtype = SUBSRC_environmental_sample;
      ssp->name = StringSave ("");
      ssp->next = biop->subtype;
      biop->subtype = ssp;
    }
  }
}

extern void ForceCleanupEntityID (Uint2 entityID)

{
  SeqEntryPtr   sep;
  ModTextFixPtr tfp;

  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep == NULL) return;
  SeqMgrClearFeatureIndexes (entityID, NULL);
  SeqEntryExplore (sep, NULL, CleanupGenbankBlockCallback);
  SeqEntryExplore (sep, NULL, CleanupEmptyFeatCallback);
  SeqEntryExplore (sep, NULL, MergeAdjacentAnnotsCallback);
  MySeqEntryToAsn3 (sep, TRUE, FALSE, TRUE);
  CorrectGenCodes (sep, entityID);
  CleanUpPseudoProducts (entityID, sep);
  SeqEntryExplore (sep, NULL, GetRidOfRedundantSourceNotes);
  RenormalizeNucProtSets (sep, TRUE);
  CdCheck (sep, NULL);
  ConvertToEcoSets (sep);
  VisitBioSourcesInSep (sep, NULL, AddEnvironmentalSampleQual);
  tfp = ModTextFixNew ();
  VisitBioSourcesInSep (sep, tfp, RemoveTextFromTextFreeSubSourceModifiers);
  MemFree (tfp);
  ObjMgrSetDirtyFlag (entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
  ObjMgrDeSelect (0, 0, 0, 0, NULL);
}

static void ForceCleanupBtn (IteM i, ButtoN b, Boolean validate)

{
  BaseFormPtr   bfp;

  if (b != NULL) {
    bfp = GetObjectExtra (b);
  } else {
#ifdef WIN_MAC
    bfp = currentFormDataPtr;
#else
    bfp = GetObjectExtra (i);
#endif
  }
  if (bfp == NULL) return;
  ForceCleanupEntityID (bfp->input_entityID);
  if (validate) {
    ValSeqEntryForm (bfp->form);
  }
}


static void ForceCleanup (IteM i)

{
  ForceCleanupBtn (i, NULL, TRUE);
}

extern void ForceTaxonFixupBtn (IteM i, ButtoN b)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

  if (b != NULL) {
    bfp = GetObjectExtra (b);
  } else {
#ifdef WIN_MAC
    bfp = currentFormDataPtr;
#else
    bfp = GetObjectExtra (i);
#endif
  }
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  /* SeqEntryExplore (sep, (Pointer) bfp, CleanupGenbankBlockCallback); */
  SeqEntryExplore (sep, NULL, CleanupEmptyFeatCallback);
  SeqEntryExplore (sep, NULL, MergeAdjacentAnnotsCallback);
  MySeqEntryToAsn3 (sep, TRUE, FALSE, TRUE);
  CorrectGenCodes (sep, bfp->input_entityID);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void ForceTaxonFixup (IteM i)

{
  ForceTaxonFixupBtn (i, NULL);
}

static void DupFeatRemovalWarning (SeqFeatPtr sfp)

{
  Char     buf [81];
  CharPtr  ctmp;
  CharPtr  loclbl;

  if (sfp == NULL) return;
  FeatDefLabel (sfp, buf, sizeof (buf) - 1, OM_LABEL_CONTENT);
  ctmp = SeqLocPrint (sfp->location);
  loclbl = ctmp;
  if (loclbl == NULL) {
    loclbl = "?";
  }
  Message (MSG_POST, "Deleting duplicate feature: Feature: %s - Location [%s]",
             buf, loclbl);
  MemFree (ctmp);
}

static Boolean LIBCALLBACK RemoveDupGenesOnBioseqs (BioseqPtr bsp, SeqMgrBioseqContextPtr bcontext)

{
  SeqMgrFeatContext  fcontext;
  GeneRefPtr         grp1, grp2;
  SeqFeatPtr         last = NULL;
  CharPtr            label;
  Boolean            leave;
  Int4               left;
  size_t             len;
  Int4               right;
  SeqAnnotPtr        sap;
  SeqFeatPtr         sfp;
  CharPtr            str;
  Uint1              strand;

  sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_GENE, 0, &fcontext);
  while (sfp != NULL) {
    leave = TRUE;
    if (last != NULL) {
      if (fcontext.left == left && fcontext.right == right) {
        if (fcontext.strand == strand ||
            strand == Seq_strand_unknown ||
            fcontext.strand == Seq_strand_unknown) {
          if (StringICmp (fcontext.label, label) == 0 &&
              (fcontext.sap == sap || (fcontext.sap->desc == NULL && sap->desc == NULL))) {
            DupFeatRemovalWarning (sfp);
            grp1 = (GeneRefPtr) last->data.value.ptrvalue;
            grp2 = (GeneRefPtr) sfp->data.value.ptrvalue;
            if (grp1 != NULL && grp2 != NULL) {
              if (grp1->locus == NULL) {
                grp1->locus = grp2->locus;
                grp2->locus = NULL;
              }
              if (grp1->allele == NULL) {
                grp1->allele = grp2->allele;
                grp2->allele = NULL;
              }
              if (grp1->desc == NULL) {
                grp1->desc = grp2->desc;
                grp2->desc = NULL;
              }
              if (grp1->maploc == NULL) {
                grp1->maploc = grp2->maploc;
                grp2->maploc = NULL;
              }
              if (grp1->db == NULL) {
                grp1->db = grp2->db;
                grp2->db = NULL;
              }
              if (grp1->syn == NULL) {
                grp1->syn = grp2->syn;
                grp2->syn = NULL;
              }
              grp2 = GeneRefFree (grp2);
              sfp->data.value.ptrvalue = GeneRefNew ();
              if (last->comment == NULL) {
                last->comment = sfp->comment;
                sfp->comment = NULL;
              } else if (sfp->comment != NULL && StringCmp (last->comment, sfp->comment) != 0) {
                len = StringLen (last->comment) + StringLen (sfp->comment);
                str = MemNew (sizeof (Char) * len + 5);
                if (str != NULL) {
                  StringCpy (str, last->comment);
                  StringCat (str, "; ");
                  StringCat (str, sfp->comment);
                  last->comment = MemFree (last->comment);
                  last->comment = str;
                }
              }
              if (last->qual == NULL) {
                last->qual = sfp->qual;
                sfp->qual = NULL;
              }
              if (last->cit == NULL) {
                last->cit = sfp->cit;
                sfp->cit = NULL;
              }
              if (last->xref == NULL) {
                last->xref = sfp->xref;
                sfp->xref = NULL;
              }
              if (last->dbxref == NULL) {
                last->dbxref = sfp->dbxref;
                sfp->dbxref = NULL;
              }
              leave = FALSE;
            }
          }
        }
      }
    }
    if (leave) {
      last = sfp;
      left = fcontext.left;
      right = fcontext.right;
      label = fcontext.label;
      strand = fcontext.strand;
      sap = fcontext.sap;
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, SEQFEAT_GENE, 0, &fcontext);
  }

  return TRUE;
}

static void RemoveDuplicateGenes (IteM i)

{
  BaseFormPtr  bfp;
  Uint2        entityID;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  entityID = SeqMgrIndexFeatures (bfp->input_entityID, NULL);
  if (entityID < 1) return;
  SeqMgrExploreBioseqs (entityID, 0, NULL, RemoveDupGenesOnBioseqs, TRUE, TRUE, TRUE);
  SeqEntryExplore (sep, NULL, CleanupEmptyFeatCallback);
  SeqEntryExplore (sep, NULL, MergeAdjacentAnnotsCallback);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static Boolean IvalsMatch (Int2 numivals1, Int4Ptr ivals1, Int2 numivals2, Int4Ptr ivals2)

{
  Int2  i, j;

  if (numivals1 != numivals2 || ivals1 == NULL || ivals2 == NULL) return FALSE;
  for (i = 0, j = 0; i < numivals1; i++) {
    if (ivals1 [j] != ivals2 [j]) return FALSE;
    j++;
    if (ivals1 [j] != ivals2 [j]) return FALSE;
    j++;
  }
  return TRUE;
}

static Boolean LIBCALLBACK RemoveDupFeatsOnBioseqs (BioseqPtr bsp, SeqMgrBioseqContextPtr bcontext)

{
  SeqMgrFeatContext  fcontext;
  Uint2              featdeftype;
  Int4Ptr            ivals;
  SeqFeatPtr         last = NULL;
  CharPtr            label;
  Boolean            leave;
  Int4               left;
  size_t             len;
  Int2               numivals;
  Int4               right;
  SeqAnnotPtr        sap;
  SeqFeatPtr         sfp;
  CharPtr            str;
  Uint1              strand;

  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
  while (sfp != NULL) {
    leave = TRUE;
    if (last != NULL) {
      if (fcontext.left == left && fcontext.right == right &&
          fcontext.featdeftype == featdeftype) {
        if (fcontext.strand == strand ||
            strand == Seq_strand_unknown ||
            fcontext.strand == Seq_strand_unknown) {
          if (IvalsMatch (fcontext.numivals, fcontext.ivals, numivals, ivals) &&
              StringICmp (fcontext.label, label) == 0 &&
               (fcontext.sap == sap || (fcontext.sap->desc == NULL && sap->desc == NULL))) {
            DupFeatRemovalWarning (sfp);
            SeqFeatDataFree (&sfp->data);             /* free duplicate feature data */
            sfp->data.choice = SEQFEAT_GENE;         /* fake as empty gene, to be removed */
            sfp->data.value.ptrvalue = GeneRefNew ();
            if (last->comment == NULL) {
              last->comment = sfp->comment;
              sfp->comment = NULL;
            } else if (sfp->comment != NULL && StringCmp (last->comment, sfp->comment) != 0) {
              len = StringLen (last->comment) + StringLen (sfp->comment);
              str = MemNew (sizeof (Char) * len + 5);
              if (str != NULL) {
                StringCpy (str, last->comment);
                StringCat (str, "; ");
                StringCat (str, sfp->comment);
                last->comment = MemFree (last->comment);
                last->comment = str;
              }
            }
            if (last->qual == NULL) {
              last->qual = sfp->qual;
              sfp->qual = NULL;
            }
            if (last->cit == NULL) {
              last->cit = sfp->cit;
              sfp->cit = NULL;
            }
            if (last->xref == NULL) {
              last->xref = sfp->xref;
              sfp->xref = NULL;
            }
            if (last->dbxref == NULL) {
              last->dbxref = sfp->dbxref;
              sfp->dbxref = NULL;
            }
            leave = FALSE;
          }
        }
      }
    }
    if (leave) {
      last = sfp;
      left = fcontext.left;
      right = fcontext.right;
      ivals = fcontext.ivals;
      numivals = fcontext.numivals;
      label = fcontext.label;
      strand = fcontext.strand;
      featdeftype = fcontext.featdeftype;
      sap = fcontext.sap;
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
  }

  return TRUE;
}

static void RemoveDuplicateFeats (IteM i)

{
  BaseFormPtr  bfp;
  Uint2        entityID;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  entityID = SeqMgrIndexFeatures (bfp->input_entityID, NULL);
  if (entityID < 1) return;
  SeqMgrExploreBioseqs (entityID, 0, NULL, RemoveDupFeatsOnBioseqs, TRUE, TRUE, TRUE);
  SeqEntryExplore (sep, NULL, CleanupEmptyFeatCallback);
  SeqEntryExplore (sep, NULL, MergeAdjacentAnnotsCallback);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static Boolean LIBCALLBACK RemoveGeneXrefsOnBioseqs (BioseqPtr bsp, SeqMgrBioseqContextPtr bcontext)

{
  SeqFeatXrefPtr     curr;
  SeqMgrFeatContext  fcontext;
  GeneRefPtr         grp;
  GeneRefPtr         grpx;
  SeqFeatXrefPtr     PNTR last;
  SeqFeatXrefPtr     next;
  Boolean            redundantgenexref;
  SeqFeatPtr         sfp;
  SeqFeatPtr         sfpx;
  CharPtr            syn1;
  CharPtr            syn2;

  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
  while (sfp != NULL) {
    if (sfp->data.choice != SEQFEAT_GENE) {
      grp = SeqMgrGetGeneXref (sfp);
      if (grp != NULL && (! SeqMgrGeneIsSuppressed (grp))) {
        sfpx = SeqMgrGetOverlappingGene (sfp->location, NULL);
        if (sfpx != NULL && sfpx->data.choice == SEQFEAT_GENE) {
          grpx = (GeneRefPtr) sfpx->data.value.ptrvalue;
          if (grpx != NULL) {
            redundantgenexref = FALSE;
            if ((! StringHasNoText (grp->locus)) && (! StringHasNoText (grpx->locus))) {
              if ((StringICmp (grp->locus, grpx->locus) == 0)) {
                redundantgenexref = TRUE;
              }
            } else if ((! StringHasNoText (grp->locus_tag)) && (! StringHasNoText (grpx->locus_tag))) {
              if ((StringICmp (grp->locus_tag, grpx->locus_tag) == 0)) {
                redundantgenexref = TRUE;
              }
            } else if (grp->syn != NULL && grpx->syn != NULL) {
              syn1 = (CharPtr) grp->syn->data.ptrvalue;
              syn2 = (CharPtr) grpx->syn->data.ptrvalue;
              if ((! StringHasNoText (syn1)) && (! StringHasNoText (syn2))) {
                if ((StringICmp (syn1, syn2) == 0)) {
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
        }
      } 
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
  }

  return TRUE;
}

static void RemoveGeneXrefs (IteM i)

{
  BaseFormPtr  bfp;
  Uint2        entityID;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  entityID = SeqMgrIndexFeatures (bfp->input_entityID, NULL);
  if (entityID < 1) return;
  SeqMgrExploreBioseqs (entityID, 0, NULL, RemoveGeneXrefsOnBioseqs, TRUE, TRUE, TRUE);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void DoRemoveNSGeneXrefs (SeqFeatPtr sfp, Pointer userdata)

{
  SeqFeatXrefPtr  curr;
  GeneRefPtr      grp;
  SeqFeatXrefPtr  PNTR last;
  SeqFeatXrefPtr  next;

  last = (SeqFeatXrefPtr PNTR) &(sfp->xref);
  curr = sfp->xref;
  while (curr != NULL) {
    next = curr->next;
    if (curr->data.choice == SEQFEAT_GENE) {
      grp = (GeneRefPtr) curr->data.value.ptrvalue;
      if (SeqMgrGeneIsSuppressed (grp)) {
        last = &(curr->next);
      } else {
        *last = next;
        curr->next = NULL;
        SeqFeatXrefFree (curr);
      }
    } else {
      last = &(curr->next);
    }
    curr = next;
  }
}

static void RemoveNSGeneXrefs (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  VisitFeaturesInSep (sep, NULL, DoRemoveNSGeneXrefs);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void DoRemoveGeneXrefs (SeqFeatPtr sfp, Pointer userdata)

{
  SeqFeatXrefPtr  curr;
  SeqFeatXrefPtr  PNTR last;
  SeqFeatXrefPtr  next;

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

static void RemoveAllGeneXrefs (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  VisitFeaturesInSep (sep, NULL, DoRemoveGeneXrefs);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void DoRefreshGeneXrefs (SeqFeatPtr sfp, Pointer userdata)

{
  SeqFeatXrefPtr    curr;
  GeneRefPtr        grp, grpfeat;
  SeqFeatPtr        gene;
  SeqMgrFeatContext fcontext;
  BioseqPtr         bsp;

  if (sfp == NULL) return;

  for (curr = sfp->xref; curr != NULL; curr = curr->next)
  {
    if (curr->data.choice == SEQFEAT_GENE) {
      grp = (GeneRefPtr) curr->data.value.ptrvalue;
      if (grp != NULL)
      {
        bsp = BioseqFindFromSeqLoc (sfp->location);
        gene = SeqMgrGetFeatureByLabel (bsp, grp->locus, SEQFEAT_GENE, 0, &fcontext);
        if (gene != NULL && gene->data.choice == SEQFEAT_GENE) {
          grpfeat = (GeneRefPtr) gene->data.value.ptrvalue;
          if (grpfeat != NULL) {
            GeneRefFree (grp);
            grp = GeneRefDup (grpfeat);
            curr->data.value.ptrvalue = grp;
          }
        }
      }
    }
  }
}

static void RefreshGeneXRefs (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  VisitFeaturesInSep (sep, NULL, DoRefreshGeneXrefs);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static ValNodePtr RemoveDbxrefList (ValNodePtr vnp)

{
  ValNodePtr  next;

  while (vnp != NULL) {
    next = vnp->next;
    DbtagFree ((DbtagPtr) vnp->data.ptrvalue);
    MemFree (vnp);
    vnp = next;
  }
  return NULL;
}

static void DoRemoveDbxrefs (SeqFeatPtr sfp, Pointer userdata)

{
  GeneRefPtr  grp;
  Uint1       feattype;

  if (sfp == NULL) return;

  if (userdata != NULL && (feattype = *(Uint1 PNTR)userdata) != sfp->data.choice) return;
  sfp->dbxref = RemoveDbxrefList (sfp->dbxref);
  if (sfp->data.choice != SEQFEAT_GENE) return;
  grp = (GeneRefPtr) sfp->data.value.ptrvalue;
  if (grp == NULL) return;
  grp->db = RemoveDbxrefList (grp->db);
}

static void DoRemoveBioSourceDbxrefs (BioSourcePtr biop, Pointer userdata)
{
  if (biop == NULL || biop->org == NULL) return;
  
  biop->org->db = RemoveDbxrefList (biop->org->db);
}

static void RemoveDbxrefsCommon (IteM i, Uint1 PNTR subtype, Boolean remove_source)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  VisitFeaturesInSep (sep, subtype, DoRemoveDbxrefs);
  if (subtype == NULL && remove_source)
  {
    VisitBioSourcesInSep (sep, NULL, DoRemoveBioSourceDbxrefs);
  }
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void RemoveGeneDbxrefs (IteM i)
{
  Uint1        subtype = SEQFEAT_GENE;
  RemoveDbxrefsCommon (i, &subtype, FALSE);
}

static void RemoveRNADbxrefs (IteM i)

{
  Uint1        subtype = SEQFEAT_RNA;

  RemoveDbxrefsCommon (i, &subtype, FALSE);
}

static void RemoveCDSDbxrefs (IteM i)

{
  Uint1        subtype = FEATDEF_CDS;
  RemoveDbxrefsCommon (i, &subtype, FALSE);
}

static void RemoveAllFeatureDbxrefs (IteM i)
{
  RemoveDbxrefsCommon (i, NULL, FALSE);
}

static void RemoveAllDbxrefs (IteM i)

{
  RemoveDbxrefsCommon (i, NULL, TRUE);
}

static void ClearPubFig (PubdescPtr pdp, Pointer userdata)

{
  if (pdp == NULL) return;
  pdp->fig = MemFree (pdp->fig);
  pdp->num = NumberingFree (pdp->num);
}

static void RemovePubFig (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  VisitPubdescsInSep (sep, NULL, ClearPubFig);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void RemoveSelFeats (IteM i)

{
  MsgAnswer          ans;
  BaseFormPtr        bfp;
  SeqMgrFeatContext  fcontext;
  BioseqPtr          prot;
  Boolean            remove_asked = FALSE;
  Boolean            remove_prots = FALSE;
  SelStructPtr       sel;
  SeqEntryPtr        sep;
  SeqFeatPtr         sfp;
  SelStructPtr       ssp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  ssp = ObjMgrGetSelected ();
  if (ssp == NULL) return;

  for (sel = ssp; sel != NULL; sel = sel->next) {
    if (sel->entityID != bfp->input_entityID) continue;
    if (sel->itemtype == OBJ_SEQFEAT) {
      sfp = SeqMgrGetDesiredFeature (sel->entityID, NULL, sel->itemID, 0, NULL, &fcontext);
      if (sfp != NULL) {
        sfp->idx.deleteme = TRUE;
        if (sfp->idx.subtype == FEATDEF_CDS && sfp->product != NULL) {
          prot = BioseqFindFromSeqLoc (sfp->product);
          if (prot != NULL) {
            if (! remove_asked) {
              ans = Message (MSG_YN, "Remove protein products?");
              if (ans == ANS_YES) {
                remove_prots = TRUE;
              }
              remove_asked = TRUE;
            }
            if (remove_prots) {
              prot->idx.deleteme = TRUE;
            }
          }
        }
      }
    }
  }

  ObjMgrSelect (0, 0, 0, 0, NULL);

  DeleteMarkedObjects (bfp->input_entityID, 0, NULL);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void RemoveUnselFeats (IteM i)

{
  MsgAnswer          ans;
  BaseFormPtr        bfp;
  BioseqPtr          bsp;
  SeqMgrFeatContext  fcontext;
  BioseqPtr          prot;
  Boolean            remove_asked = FALSE;
  Boolean            remove_prots = FALSE;
  SelStructPtr       sel;
  SeqEntryPtr        sep;
  SeqFeatPtr         sfp;
  SelStructPtr       ssp;
  SeqEntryPtr        scope;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  ssp = ObjMgrGetSelected ();
  if (ssp == NULL) return;

  scope = SeqEntrySetScope (NULL);
  bsp = NULL;
  for (sel = ssp; sel != NULL && bsp == NULL; sel = sel->next) {
    if (sel->entityID != bfp->input_entityID) continue;
    if (sel->itemtype == OBJ_SEQFEAT) {
      sfp = SeqMgrGetDesiredFeature (sel->entityID, NULL, sel->itemID, 0, NULL, &fcontext);
      if (sfp != NULL) {
        bsp = BioseqFindFromSeqLoc (sfp->location);
      }
    }
  }
  if (bsp == NULL) return;
  SeqEntrySetScope (scope);

  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
  while (sfp != NULL) {
    sfp->idx.deleteme = TRUE;
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
  }

  for (sel = ssp; sel != NULL; sel = sel->next) {
    if (sel->entityID != bfp->input_entityID) continue;
    if (sel->itemtype == OBJ_SEQFEAT) {
      sfp = SeqMgrGetDesiredFeature (0, bsp, sel->itemID, 0, NULL, &fcontext);
      if (sfp != NULL) {
        sfp->idx.deleteme = FALSE;
      }
    }
  }

  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
  while (sfp != NULL) {
    if (sfp->idx.deleteme) {
      if (sfp->idx.subtype == FEATDEF_CDS && sfp->product != NULL) {
        prot = BioseqFindFromSeqLoc (sfp->product);
        if (prot != NULL) {
          if (! remove_asked) {
            ans = Message (MSG_YN, "Remove protein products?");
            if (ans == ANS_YES) {
              remove_prots = TRUE;
            }
            remove_asked = TRUE;
          }
          if (remove_prots) {
            prot->idx.deleteme = TRUE;
          }
        }
      }
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
  }

  ObjMgrSelect (0, 0, 0, 0, NULL);

  DeleteMarkedObjects (bfp->input_entityID, 0, NULL);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void RemoveCDDRegions (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  FreeCDDRegions (sep);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void RemoveCDDAligns (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  FreeCDDAligns (sep);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void RemoveCDDDups (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  RemoveDuplicateCDDs (sep);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void RetainBestCDD (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  LeaveBestCDD (sep);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void LastDitchLookup (BioSourcePtr biop)

{
  DbtagPtr      dbt;
  Char          orgname [256];
  OrgRefPtr     orp;
  CharPtr       ptr;
  CharPtr       binomial_end = NULL, trinomial_end = NULL;
  SubSourcePtr  ssp;
  Int4          taxID;
  ValNodePtr    vnp;

  if (biop == NULL) return;
  orp = biop->org;
  if (orp == NULL) return;
  /* if this organism has already been assigned a taxon ID, skip it */
  if (orp->db != NULL) {
    for (vnp = orp->db; vnp != NULL; vnp = vnp->next) {
      dbt = (DbtagPtr) vnp->data.ptrvalue;
      if (dbt != NULL) {
        if (StringICmp (dbt->db, "taxon") == 0) return;
      }
    }
  }
  /* we are looking for a name that might become a recognizable name if we
   * truncated it after the first four words (if the third word is subsp.),
   * after the first three words (if the third word is not subsp.),
   * or after the first two words if the above two attempts failed.
   */
 
  StringNCpy_0 (orgname, orp->taxname, sizeof (orgname));
  if (StringHasNoText (orgname)) return;
  ptr = StringChr (orgname, ' ');
  if (ptr == NULL) return;
  /* skip over the first word and the spaces after it. */
  while (*ptr == ' ') 
  {
    ptr++;
  }
  ptr = StringChr (ptr, ' ');
  /* if there are only two words, give up. */
  if (ptr == NULL) 
  {
    return;
  }
  binomial_end = ptr;
  while (*ptr == ' ')
  {
    ptr++;
  }
  if (StringNCmp (ptr, "subsp.", 6) == 0)
  {
    ptr += 6;
    while (*ptr == ' ' )
    {
      ptr++;
    }
  }
  
  ptr = StringChr (ptr, ' ');
  if (ptr != NULL)
  {
    trinomial_end = ptr;
  }
  
  /* see if trinomial produces a tax server hit */
  taxID = 0;
  if (trinomial_end != NULL)
  {
    *trinomial_end = '\0';
    taxID = Taxon3GetTaxIdByName (orgname);
  }
  if (taxID < 1)
  {
    /* see if binomial produces a tax server hit */
    *binomial_end = '\0';
    taxID = Taxon3GetTaxIdByName (orgname);
  }
  
  if (taxID < 1) return;
  
  /* create a note that contains the original name of the organism, and truncate
   * the actual organism after the binomial or trinomial.
   */
  ssp = SubSourceNew ();
  if (ssp != NULL) {
    ssp->subtype = 255;
    ssp->name = orp->taxname;
	orp->taxname = NULL;
    ssp->next = biop->subtype;
    biop->subtype = ssp;
    SetTaxNameAndRemoveTaxRef (orp, StringSave (orgname));
  }
}

static void LastDitchTaxonFixup (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)

{
	BioseqPtr bsp = NULL;
	BioseqSetPtr bssp = NULL;
	ValNodePtr descr, vnp;
	BioSourcePtr biosp = NULL;
	SeqAnnotPtr annot, sap;
	SeqFeatPtr sfp;
	
	if (IS_Bioseq(sep)) {
		bsp = sep->data.ptrvalue;
		descr = bsp->descr;
		annot = bsp->annot;
	} else {
		bssp = sep->data.ptrvalue;
		descr = bssp->descr;
		annot = bssp->annot;
	}
	for (vnp = descr; vnp; vnp = vnp->next) {
		if (vnp->choice == Seq_descr_source) {
			biosp = vnp->data.ptrvalue;
			LastDitchLookup (biosp); 
		}
	}
	for (sap = annot; sap != NULL; sap = sap->next) {
		if (sap->type == 1) {
			for (sfp = (SeqFeatPtr) sap->data; sfp != NULL; sfp = sfp->next) {
				if (sfp->data.choice == SEQFEAT_BIOSRC) {
					biosp = sfp->data.value.ptrvalue;
					LastDitchLookup (biosp);
				}
			}
		}
	}
}

static void GenSpecTaxonFixup (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  SeqEntryExplore (sep, NULL, LastDitchTaxonFixup);
  ForceTaxonFixupBtn (i, NULL);
}

extern void MergeFeatureIntervalsToParts (SeqFeatPtr sfp, Boolean ordered)
{
  BioseqPtr     bsp;
  CodeBreakPtr  cbp;
  CdRegionPtr   crp;
  Boolean       noLeft;
  Boolean       noRight;
  RnaRefPtr     rrp;
  SeqIdPtr      sip;
  SeqLocPtr     slp;
  tRNAPtr       trna;
  
  if (sfp == NULL || sfp->location == NULL) return;
  sip = SeqLocId (sfp->location);
  if (sip == NULL) return;
  bsp = BioseqFind (sip);
  if (bsp == NULL) return;
  if (ISA_aa (bsp->mol)) return;
  CheckSeqLocForPartial (sfp->location, &noLeft, &noRight);
  slp = SegLocToPartsEx (bsp, sfp->location, ordered);
  if (slp == NULL) return;
  sfp->location = SeqLocFree (sfp->location);
  sfp->location = slp;
  FreeAllFuzz (sfp->location);
  SetSeqLocPartial (sfp->location, noLeft, noRight);
  sfp->partial = (sfp->partial || noLeft || noRight);
  switch (sfp->data.choice) {
    case SEQFEAT_CDREGION :
      crp = (CdRegionPtr) sfp->data.value.ptrvalue;
      if (crp != NULL && crp->code_break != NULL) {
        for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) {
          slp = SegLocToPartsEx (bsp, cbp->loc, ordered);
          if (slp != NULL) {
            cbp->loc = SeqLocFree (cbp->loc);
            cbp->loc = slp;
            FreeAllFuzz (cbp->loc);
          }
        }
      }
      break;
    case SEQFEAT_RNA :
      rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
      if (rrp != NULL && rrp->type == 3 && rrp->ext.choice == 2) {
        trna = rrp->ext.value.ptrvalue;
        if (trna != NULL && trna->anticodon != NULL) {
          slp = SegLocToPartsEx (bsp, trna->anticodon, ordered);
          if (slp != NULL) {
            trna->anticodon = SeqLocFree (trna->anticodon);
            trna->anticodon = slp;
            FreeAllFuzz (trna->anticodon);
          }
        }
      }
      break;
    default :
      break;
  }  
}

static void MergeToPartsCallback (SeqFeatPtr sfp, Pointer userdata)
{
  BoolPtr ordered;
  
  if (sfp == NULL) return;
  ordered = (BoolPtr) userdata;
  
  MergeFeatureIntervalsToParts (sfp, *ordered);
}

static void MergeToParts (IteM i, Boolean ordered)
{
  BaseFormPtr       bfp;
  SelStructPtr      ssp;
  Boolean           some_sel_not_feat = FALSE;
  Boolean           some_sel_feat = FALSE;
  SeqFeatPtr        sfp;
  SeqMgrFeatContext fcontext;
  SeqEntryPtr       sep;

  ssp = ObjMgrGetSelected ();
  if (ssp == NULL) {
#ifdef WIN_MAC
    bfp = currentFormDataPtr;
#else
    bfp = GetObjectExtra (i);
#endif
    if (bfp == NULL) {
      Message (MSG_ERROR, "MergeToParts error");
      return;
    }
    sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
    if (sep == NULL) return;
    VisitFeaturesInSep (sep, (Pointer) &ordered, MergeToPartsCallback);
    ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  }
  else
  {
    while (ssp != NULL)
    {
      if (ssp->itemtype == OBJ_SEQFEAT)
      {
        sfp = SeqMgrGetDesiredFeature (ssp->entityID, NULL, ssp->itemID, 0, NULL, &fcontext);
        if (sfp != NULL)
        {
          MergeFeatureIntervalsToParts (sfp, ordered);
          some_sel_feat = TRUE;
          ObjMgrSetDirtyFlag (ssp->entityID, TRUE);
          ObjMgrSendMsg (OM_MSG_UPDATE, ssp->entityID, 0, 0);
        }
      }
      else
      {
        some_sel_not_feat = TRUE;        
      }
      ssp = ssp->next;
    }
    if (!some_sel_feat)
    {
      Message (MSG_ERROR, "No features selected!");
    }
    else if (some_sel_not_feat)
    {
      Message (MSG_ERROR, "Some selected items were not features!");
    }
  }
  Update ();
}

extern void MergeToPartsJoin (IteM i)
{
  MergeToParts (i, FALSE);
}

extern void MergeToPartsOrdered (IteM i)
{
  MergeToParts (i, TRUE);
}


static Boolean MergeSegSeqCallback (GatherContextPtr gcp)

{
  BioseqPtr     bsp;
  CodeBreakPtr  cbp;
  CdRegionPtr   crp;
  Boolean       noLeft;
  Boolean       noRight;
  RnaRefPtr     rrp;
  SeqEntryPtr   sep;
  SeqFeatPtr    sfp;
  SeqLocPtr     slp;
  tRNAPtr       trna;

  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->location == NULL) return TRUE;
  bsp = GetBioseqGivenSeqLoc (sfp->location, gcp->entityID);
  if (bsp == NULL) return TRUE;
  if (ISA_aa (bsp->mol)) return TRUE;
  if (bsp->repr != Seq_repr_seg) {
    sep = GetBestTopParentForData (gcp->entityID, bsp);
    if (sep == NULL) return TRUE;
    sep = FindNucSeqEntry (sep);
    if (sep == NULL || sep->choice != 1) return TRUE;
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp == NULL) return TRUE;
  }
  CheckSeqLocForPartial (sfp->location, &noLeft, &noRight);
  slp = SeqLocMerge (bsp, sfp->location, NULL, FALSE, TRUE, FALSE);
  if (slp == NULL) return TRUE;
  sfp->location = SeqLocFree (sfp->location);
  sfp->location = slp;
  FreeAllFuzz (sfp->location);
  SetSeqLocPartial (sfp->location, noLeft, noRight);
  sfp->partial = (sfp->partial || noLeft || noRight);
  switch (sfp->data.choice) {
    case SEQFEAT_CDREGION :
      crp = (CdRegionPtr) sfp->data.value.ptrvalue;
      if (crp != NULL && crp->code_break != NULL) {
        for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) {
          slp = SeqLocMerge (bsp, cbp->loc, NULL, FALSE, TRUE, FALSE);
          if (slp != NULL) {
            cbp->loc = SeqLocFree (cbp->loc);
            cbp->loc = slp;
            FreeAllFuzz (cbp->loc);
          }
        }
      }
      break;
    case SEQFEAT_RNA :
      rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
      if (rrp != NULL && rrp->type == 3 && rrp->ext.choice == 2) {
        trna = rrp->ext.value.ptrvalue;
        if (trna != NULL && trna->anticodon != NULL) {
          slp = SeqLocMerge (bsp, trna->anticodon, NULL, FALSE, TRUE, FALSE);
          if (slp != NULL) {
            trna->anticodon = SeqLocFree (trna->anticodon);
            trna->anticodon = slp;
            FreeAllFuzz (trna->anticodon);
          }
        }
      }
      break;
    default :
      break;
  }
  return TRUE;
}

static void MergeToSegSeq (IteM i)

{
  BaseFormPtr   bfp;
  GatherScope   gs;
  SelStructPtr  ssp;

  ssp = ObjMgrGetSelected ();
  if (ssp == NULL) {
#ifdef WIN_MAC
    bfp = currentFormDataPtr;
#else
    bfp = GetObjectExtra (i);
#endif
    if (bfp == NULL) {
      Message (MSG_ERROR, "MergeToSegSeq error");
      return;
    }
    MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
    gs.seglevels = 1;
    MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
    gs.ignore[OBJ_BIOSEQ] = FALSE;
    gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
    gs.ignore[OBJ_SEQANNOT] = FALSE;
    gs.ignore[OBJ_SEQFEAT] = FALSE;
    GatherEntity (bfp->input_entityID, NULL, MergeSegSeqCallback, &gs);
    ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
    /*
    Message (MSG_ERROR, "No item selected");
    */
    return;
  }
  if (ssp->itemtype != OBJ_SEQFEAT) {
    Message (MSG_ERROR, "Feature must be selected");
    return;
  }
  GatherItem (ssp->entityID, ssp->itemID, ssp->itemtype, NULL, MergeSegSeqCallback);
  ObjMgrSetDirtyFlag (ssp->entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ssp->entityID, 0, 0);
}

static void SegToRawCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr  bsp;

  if (sep == NULL) return;
  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL || (bsp->repr != Seq_repr_seg && bsp->repr != Seq_repr_delta)) return;
  SegOrDeltaBioseqToRaw (bsp);
}

static void SegSeqToRawSeq (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  SeqEntryExplore (sep, (Pointer) bfp, SegToRawCallback);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  Message (MSG_OK, "Some manual desktop manipulations remain");
}

static void RawSeqToDeltaSeq (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  VisitBioseqsInSep (sep, NULL, ConvertNsToGaps);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void RawSeqToDeltaSeqUnknownLengthGaps (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;
  Int4         gap_sizes[2];

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  gap_sizes [0] = 100;
  gap_sizes [1] = -1;
  VisitBioseqsInSep (sep, gap_sizes, ConvertNsToGaps);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void RawSeqToDeltaSeqUnknown100LengthGaps (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;
  Int4         gap_sizes[2];

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  gap_sizes [0] = -1;
  gap_sizes [1] = 0;
  VisitBioseqsInSep (sep, gap_sizes, ConvertNsToGaps);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

#if 0
static void ConvertSegSetToDeltaItem (IteM i)
{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;
  Int4         gap_sizes[2];

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  ConvertSegSetsToDeltaSequences (sep);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}
#endif


typedef struct gapconversiondata 
{
  FEATURE_FORM_BLOCK

  GrouP  unknown_op;
  TexT   unknown_val_txt;
  GrouP  known_op;
  TexT   known_val_txt;
  ButtoN acceptBtn;
  DoC    explanation;

  
} GapConversionData, PNTR GapConversionPtr;

static ParData faParFmt = {FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData faColFmt = {0, 0, 80, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, TRUE};

static void SetConvertGapsAcceptAndText (GapConversionPtr gcp)
{
  Char             str[15];
  Int4 unknown_val, known_val;
  Int4 unknown_op, known_op;
  Char explanation[255];
  	
  if (gcp == NULL || gcp->explanation == NULL) return;
  GetTitle (gcp->unknown_val_txt, str, sizeof (str));
  unknown_val = atoi (str);
  unknown_op = GetValue (gcp->unknown_op);
  GetTitle (gcp->known_val_txt, str, sizeof (str));
  known_val = atoi (str);
  known_op = GetValue (gcp->known_op);

  Reset (gcp->explanation);
  explanation [0] = 0;
  
  if (unknown_val < 0 || known_val < 0)
  {
    sprintf (explanation, "%s", "Negative values are not permitted.");
  	Disable (gcp->acceptBtn);
  }
  else if (known_val == unknown_val)
  {
    sprintf (explanation, 
  	         "You must specify different values for known and unknown gap conversion sizes.");
  	Disable (gcp->acceptBtn);
  }  
  else if (unknown_val == 0 && unknown_op == 1 && known_val == 0 && known_op == 1)
  {
    sprintf (explanation, "%s", "This combination will have no effect.");
    Disable (gcp->acceptBtn);
  }
  else if (unknown_val == 0 && unknown_op == 1 && (known_val == 0 || known_val == 1) && known_op == 2)
  {
    sprintf (explanation, "%s", "All sequences of Ns will be converted to gaps of known length.");
    Enable (gcp->acceptBtn);
  }
  else if ((unknown_val == 0 || unknown_val == 1) && unknown_op == 2 && known_val == 0 && known_op == 1)
  {
    sprintf (explanation, "%s", "All sequences of Ns will be converted to gaps of unknown length (size 100).");
    Enable (gcp->acceptBtn);
  }
  else if (unknown_val == 0 && unknown_op == 1)
  {  	
  	if (known_op == 1)
  	{
  	  sprintf (explanation, 
  	           "All sequences of exactly %d Ns will be converted to gaps of known length.\n"
  	           "All other sequences of Ns will remain as Ns.", known_val);
   	  Enable (gcp->acceptBtn);
 	}
 	else
 	{
	  sprintf (explanation, 
  	             "All sequences of %d or more Ns will be converted to gaps of known length.\n"
  	             "All sequences of less than %d Ns will remain as Ns.", known_val, known_val);
  	  Enable (gcp->acceptBtn);
 	}
  }
  else if (known_val == 0 && known_op == 1)
  {
  	if (unknown_op == 1)
  	{
  	  sprintf (explanation, 
  	           "All sequences of exactly %d Ns will be converted to gaps of unknown length (size 100).\n"
  	           "All other sequences of Ns will remain as Ns.", unknown_val);
  	  Enable (gcp->acceptBtn);
  	}
  	else if (unknown_op == 2)
  	{
  	  sprintf (explanation, 
  	           "All sequences of %d or more Ns will be converted to gaps of unknown length.\n"
  	           "All sequences of less than %d Ns will remain as Ns.", unknown_val, unknown_val);
  	  Enable (gcp->acceptBtn);
  	}
  }
  else if (known_op == 1 && unknown_op == 1)
  {
  	sprintf (explanation, 
  	         "All sequences of exactly %d Ns will be converted to gaps of known length.\n"
  	         "All sequences of exactly %d Ns will be converted to gaps of unknown length (size 100).\n"
  	         "All sequences of Ns with lengths other than %d or %d will remain as Ns.", 
  	         known_val, unknown_val, known_val, unknown_val);
  	Enable (gcp->acceptBtn);
  }
  else if (known_op == 2 && unknown_op == 2)
  {
    /* handle cases where both operators are greater than/equal to */
  	if (known_val > unknown_val)
  	{
  	  if (unknown_val == 0 || unknown_val == 1)
  	  {
  	    sprintf (explanation, 
  	             "All sequences of Ns with lengths >= 1 and <= %d will be converted to gaps of unknown length (size 100).\n"
  	             "All sequences of Ns with lengths >= %d will be converted to gaps of known length.\n",
  	             known_val -1, known_val);         
  	  }
  	  else
  	  {
  	  	sprintf (explanation,
  	             "All sequences of Ns with lengths >= %d and <= %d will be converted to gaps of unknown length (size 100).\n"
  	             "All sequences of Ns with lengths >= %d will be converted to gaps of known length.\n"
  	             "All sequences of Ns with lengths <= %d will remain as Ns.",
  	             unknown_val, known_val -1, known_val, unknown_val - 1);         
  	  }
  	}
  	else
  	{
  	  if (known_val == 0 || known_val == 1)
  	  {
  	    sprintf (explanation, 
  	             "All sequences of Ns with lengths >= 1 and <= %d will be converted to gaps of known length.\n"
  	             "All sequences of Ns with lengths >= %d will be converted to gaps of unknown length (size 100).\n",
  	             unknown_val -1, unknown_val);         
  	  }
  	  else
  	  {
  	  	sprintf (explanation,
  	             "All sequences of Ns with lengths >= %d and <= %d will be converted to gaps of known length.\n"
  	             "All sequences of Ns with lengths >= %d will be converted to gaps of unknown length (size 100).\n"
  	             "All sequences of Ns with lengths <= %d will remain as Ns.",
  	             known_val, unknown_val -1, unknown_val, known_val - 1);         
  	  }
  	}
  }
  else if (known_val > unknown_val)
  {
  	if (known_op == 1)
  	{
  	  /* we know that unknown_op is 2 */
      if (unknown_val == 0 || unknown_val == 1)
  	  {
        sprintf (explanation, 
  	           "All sequences of exactly %d Ns will be converted to gaps of known length.\n"
  	           "All sequences of Ns with lengths >= 1 and <= %d or lengths >= %d will be converted to gaps of unknown length (size 100).\n",
  	           known_val, known_val - 1, known_val + 1);
  	  }
  	  else
  	  {
        sprintf (explanation, 
  	            "All sequences of exactly %d Ns will be converted to gaps of known length.\n"
  	             "All sequences of Ns with lengths >= %d and <= %d or lengths >= %d will be converted to gaps of unknown length (size 100).\n"
  	             "All sequences of Ns with lengths <= %d will remain as Ns.", 
  	             known_val, unknown_val, known_val - 1, known_val + 1, unknown_val - 1);
  	  }
  	  Enable (gcp->acceptBtn);
  	}
  	else if (known_op == 2)
  	{
  	  /* we know that unknown_op is 1 */
  	  if (unknown_val == 1)
  	  {
  	  	sprintf (explanation,
  	           "All sequences of exactly 1 Ns will be converted to gaps of unknown length (size 100).\n"
  	           "All sequences with lengths >= %d will be converted to gaps of known length.\n"
  	           "All sequences of Ns with lengths >= 2 and <= %d will remain as Ns",
  	  	        known_val, known_val - 1);
  	  }
  	  else
  	  {
        sprintf (explanation, 
  	           "All sequences of exactly %d Ns will be converted to gaps of unknown length (size 100).\n"
  	           "All sequences of Ns with lengths >= %d will be converted to gaps of known length.\n"
  	           "All sequences of Ns with lengths >= %d or lengths >= %d and <= %d will remain as Ns.  ",
  	           unknown_val, known_val, unknown_val - 1, unknown_val + 1, known_val - 1);
  	  }
  	  Enable (gcp->acceptBtn);
  	}
  }
  else
  {
  	/* unknown_val > known_val */
    if (known_op == 1)
    {
      /* we know that unknown_op is 2 */
  	  sprintf (explanation, 
  	           "All sequences of exactly %d Ns will be converted to gaps of known length.\n"
  	           "All sequences of Ns with lengths >= %d will be converted to gaps of unknown length (size 100).\n"
  	           "All sequences of Ns with lengths <= %d or lengths >= %d and <= %d will remain as Ns.\n",
  	            known_val, unknown_val, known_val - 1, known_val + 1, unknown_val - 1);
    }
    else
    {
      /* we know that unknown_op is 1, known_op is 2 */
      if (known_val == 0 || known_val == 1)
      {
      	sprintf (explanation,
  	             "All sequences of exactly %d Ns will be converted to gaps of unknown length (size 100).\n"
  	             "All sequences of Ns with lengths >= 1 and <= %d or lengths >= %d will be converted to gaps of known length.\n",
      	         unknown_val, unknown_val - 1, unknown_val + 1);
      }
      else 
      {
      	sprintf (explanation,
  	             "All sequences of exactly %d Ns will be converted to gaps of unknown length (size 100).\n"
  	             "All sequences of Ns with lengths >= %d and <= %d or lengths >= %d will be converted to gaps of known length.\n"
  	             "All sequences of Ns with lengths <= %d will remain as Ns.",
      	         unknown_val, known_val, unknown_val - 1, unknown_val + 1, known_val);
      	
      }
    }
    Enable (gcp->acceptBtn);
  }  
  AppendText (gcp->explanation, explanation, &faParFmt, &faColFmt, programFont);
  UpdateDocument (gcp->explanation, 0, 0);
  Update ();	
}

static void SetGapsConvertAcceptButtonAndTextGroup (GrouP g)
{
  GapConversionPtr gcp;
  
  gcp = (GapConversionPtr) GetObjectExtra (g);
  SetConvertGapsAcceptAndText (gcp);
}

static void SetGapsConvertAcceptButtonAndTextText (TexT nText)
{
  GapConversionPtr gcp;
  
  gcp = (GapConversionPtr) GetObjectExtra (nText);
  SetConvertGapsAcceptAndText (gcp);
}


static void ConvertGaps (ButtoN b)
{
  GapConversionPtr gcp;
  SeqEntryPtr      sep;
  Int4             gap_sizes[2];
  Char             str[15];
  Int4             unknown_val, known_val;
  Int4             unknown_op, known_op;
  
  if (b == NULL) return;
  gcp = (GapConversionPtr) GetObjectExtra (b);
  if (gcp == NULL) return;
  sep = GetTopSeqEntryForEntityID (gcp->input_entityID);
  if (sep == NULL) return;
  
  GetTitle (gcp->unknown_val_txt, str, sizeof (str));
  unknown_val = atoi (str);
  unknown_op = GetValue (gcp->unknown_op);
  if (unknown_op == 1)
  {
  	gap_sizes [0] = unknown_val;
  }
  else
  {
    if (unknown_val == 0)
      unknown_val = 1;
  	gap_sizes[0] = 0 - unknown_val;
  }
  GetTitle (gcp->known_val_txt, str, sizeof (str));
  known_val = atoi (str);
  known_op = GetValue (gcp->known_op);
  if (known_op == 1)
  {
  	gap_sizes [1] = known_val;
  }
  else
  {
    if (known_val == 0)
      known_val = 1;
  	gap_sizes[1] = 0 - known_val;
  }
  
  
  Hide (gcp->form);
  VisitBioseqsInSep (sep, gap_sizes, ConvertNsToGaps); 
  ObjMgrSetDirtyFlag (gcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, gcp->input_entityID, 0, 0);
  Remove (gcp->form);  
}

static void RawSeqToDeltaSeqUnknownWithUnknownLengthGaps (IteM i)
{
  BaseFormPtr      bfp;
  GapConversionPtr gcp;
  WindoW           w;
  GrouP            h, k, l, g, c;
  RecT             r;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  gcp = (GapConversionPtr) MemNew (sizeof (GapConversionData));
  if (gcp == NULL) return;
  
  gcp->input_entityID = bfp->input_entityID;
  w = FixedWindow (-50, -33, -10, -10, "Convert Ns to Gaps of Unknown Length", StdCloseWindowProc);
  SetObjectExtra (w, gcp, StdCleanupFormProc);
  gcp->form = (ForM) w;
  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
  k = HiddenGroup (h, 1, 0, NULL);
  StaticPrompt (k, "Choose size of length of Ns to convert to gaps of unknown length", 0, dialogTextHeight, programFont, 'c');
  g = HiddenGroup (h, 2, 0, NULL);
  gcp->unknown_op = HiddenGroup (g, 0, 2, SetGapsConvertAcceptButtonAndTextGroup);
  SetObjectExtra (gcp->unknown_op, gcp, NULL);
  RadioButton (gcp->unknown_op, "=");
  RadioButton (gcp->unknown_op, ">=");
  SetValue (gcp->unknown_op, 1);
  gcp->unknown_val_txt = DialogText (g, "100", 14, SetGapsConvertAcceptButtonAndTextText);
  SetObjectExtra (gcp->unknown_val_txt, gcp, NULL);
  k = HiddenGroup (h, 1, 0, NULL);
  StaticPrompt (k, "Choose size of length of Ns to convert to gaps of known length", 0, dialogTextHeight, programFont, 'c');
  l = HiddenGroup (h, 2, 0, NULL);
  gcp->known_op = HiddenGroup (l, 0, 2, SetGapsConvertAcceptButtonAndTextGroup);
  SetObjectExtra (gcp->known_op, gcp, NULL);
  RadioButton (gcp->known_op, "=");
  RadioButton (gcp->known_op, ">=");
  SetValue (gcp->known_op, 1);
  gcp->known_val_txt = DialogText (l, "0", 14, SetGapsConvertAcceptButtonAndTextText);
  SetObjectExtra (gcp->known_val_txt, gcp, NULL);
  
  /* status text */
  l = HiddenGroup (h, 1, 0, NULL);
  gcp->explanation = DocumentPanel (l, stdCharWidth * 27, stdLineHeight * 8);
  ObjectRect (gcp->explanation, &r);
  InsetRect (&r, 4, 4);
  faColFmt.pixWidth = r.right - r.left;

  c = HiddenGroup (h, 4, 0, NULL);
  gcp->acceptBtn = PushButton (c, "Accept", ConvertGaps);
  SetObjectExtra (gcp->acceptBtn, gcp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  
  SetConvertGapsAcceptAndText (gcp);
  RealizeWindow (w);
  Show (w);
  Update ();
}

static Uint2 deltaconversionedittypes [] = 
{
  TAGLIST_TEXT, TAGLIST_POPUP, TAGLIST_TEXT
};

ENUM_ALIST(deltaconversionedit_alist)
  {"Unknown length",  0},
  {"Known Length",    1},
END_ENUM_ALIST

EnumFieldAssocPtr deltaconversionedit_alists [] = {
  NULL, deltaconversionedit_alist
};

static Uint2 deltaconversionedit_widths [] = {
  5, 5, 5,
};

typedef struct deltaconversion 
{
  FEATURE_FORM_BLOCK

  DialoG gap_locations;
  GrouP  coord_grp;
  
} DeltaConversionData, PNTR DeltaConversionPtr;

typedef struct gaplocinfo 
{
  Int4    start_pos;
  Boolean is_known;
  Int4    length;
} GapLocInfoData, PNTR GapLocInfoPtr;

static void FixGappedCodingRegionLocation (SeqFeatPtr sfp, Pointer userdata)
{
  BioseqPtr gapped_bioseq, sfp_bsp;
  
  if (sfp == NULL 
      || sfp->data.choice != SEQFEAT_CDREGION
      || sfp->location == NULL
      || userdata == NULL)
  {
    return;
  }
  
  gapped_bioseq = (BioseqPtr) userdata;
  if (gapped_bioseq->repr != Seq_repr_delta)
  {
    return;
  }
  
  sfp_bsp = BioseqFind (SeqLocId (sfp->location));
  if (sfp_bsp != gapped_bioseq)
  {
    return;
  }

  sfp->location = RemoveGapsFromDeltaLocation (sfp->location, (BioseqPtr) userdata);
}

static void 
ConvertRawBioseqToDelta 
(BioseqPtr bsp,
 ValNodePtr gaps,
 SeqAlignPtr salp,
 Int4        aln_row) 

{
  CharPtr       bases;
  Int4          len;
  ValNodePtr    seq_ext;
  SeqLitPtr     slp;
  IntFuzzPtr    ifp;
  ValNodePtr    gap_vnp;
  GapLocInfoPtr glip;
  Int4          orig_seq_offset;
  Char          tmp_ch;
  Int4          gap_start;
  SeqEntryPtr   sep;

  if (bsp == NULL || bsp->repr != Seq_repr_raw || ISA_aa (bsp->mol)
      || gaps == NULL) 
  {
    return;
  }
  if (salp != NULL && aln_row < 1)
  {
    return;
  }

  bases = GetSequenceByBsp (bsp);
  if (bases == NULL) return;

  seq_ext = NULL;
  len = 0;
  orig_seq_offset = 0;

  for (gap_vnp = gaps; gap_vnp != NULL; gap_vnp = gap_vnp->next)
  {
    glip = (GapLocInfoPtr) gap_vnp->data.ptrvalue;
    if (glip == NULL)
    {
      continue;
    }
    gap_start = glip->start_pos;
    /* remap for alignment coordinates if desired */
    if (salp != NULL)
    {
      gap_start = AlnMgr2MapSeqAlignToBioseq(salp, gap_start, aln_row);
    }
    if (gap_start < 1 || gap_start > bsp->length)
    {
      continue;
    }
    
    /* add data since last gap */
    slp = (SeqLitPtr) MemNew (sizeof (SeqLit));
    if (slp != NULL) 
    {
      slp->length = gap_start - orig_seq_offset;
      ValNodeAddPointer (&(seq_ext), (Int2) 2, (Pointer) slp);
      slp->seq_data = BSNew (slp->length);
      slp->seq_data_type = Seq_code_iupacna;
      tmp_ch = bases [gap_start];
      bases [gap_start] = 0;
      AddBasesToByteStore (slp->seq_data, bases + orig_seq_offset);
      bases [gap_start] = tmp_ch;
      len += slp->length;
      orig_seq_offset += slp->length;
    }
    
    /* add gap */
    slp = (SeqLitPtr) MemNew (sizeof (SeqLit));
    if (slp != NULL) 
    {
      ValNodeAddPointer ((ValNodePtr PNTR) &(seq_ext), (Int2) 2, (Pointer) slp);
      if (glip->is_known)
      {
        slp->length = glip->length;
      }
      else
      {
        ifp = IntFuzzNew ();
        ifp->choice = 4;
        slp->fuzz = ifp;
        slp->length = 100;
      }
      len += slp->length;
    }
  }
  
  /* add remaining data after last gap to end */
  slp = (SeqLitPtr) MemNew (sizeof (SeqLit));
  if (slp != NULL) 
  {
    slp->length = bsp->length - orig_seq_offset;
    ValNodeAddPointer (&(seq_ext), (Int2) 2, (Pointer) slp);
    slp->seq_data = BSNew (slp->length);
    slp->seq_data_type = Seq_code_iupacna;
    AddBasesToByteStore (slp->seq_data, bases + orig_seq_offset);
    len += slp->length;
  }
  
  MemFree (bases);

  bsp->seq_data = BSFree (bsp->seq_data);
  bsp->seq_data_type = 0;
  bsp->repr = Seq_repr_delta;
  bsp->seq_ext_type = 4;
  bsp->seq_ext = seq_ext;
  bsp->length = len;

  BioseqPack (bsp);
  
  /* now adjust features for insertion */
  orig_seq_offset = 0;
  for (gap_vnp = gaps; gap_vnp != NULL; gap_vnp = gap_vnp->next)
  {
    glip = (GapLocInfoPtr) gap_vnp->data.ptrvalue;
    if (glip == NULL)
    {
      continue;
    }
    gap_start = glip->start_pos;
    /* remap for alignment coordinates if desired */
    if (salp != NULL)
    {
      gap_start = AlnMgr2MapSeqAlignToBioseq(salp, gap_start, aln_row);
    }
    if (gap_start < 1 || gap_start > bsp->length)
    {
      continue;
    }
    AdjustFeaturesForInsertion (bsp, bsp->id, 
                                gap_start + orig_seq_offset,
                                glip->length,
                                FALSE);
    orig_seq_offset += glip->length;
  }

  sep = GetTopSeqEntryForEntityID (bsp->idx.entityID);

  VisitFeaturesInSep (sep, bsp, FixGappedCodingRegionLocation);
}

static void ConvertBioseqToDeltaWithSequenceGapList (BioseqPtr bsp, Pointer userdata)
{
  ConvertRawBioseqToDelta (bsp, userdata, NULL, -1);
}

static void ConvertBioseqToDeltaWithAlignmentGapList (BioseqPtr bsp, Pointer userdata)
{
  Int4        aln_row = -1;
  SeqIdPtr    sip, sip_next;
  SeqAlignPtr salp;
  
  if (bsp == NULL || bsp->id == NULL || userdata == NULL)
  {
    return;
  }
  salp = FindAlignmentsForBioseq (bsp);

  if (salp == NULL)
  {
    return;
  }
  
  sip = bsp->id;
  
  while (sip != NULL && aln_row == -1)
  {
    sip_next = sip->next;
    sip->next = NULL;
    aln_row = AlnMgr2GetFirstNForSip (salp, sip);
    sip->next = sip_next;
    sip = sip_next;
  }

  ConvertRawBioseqToDelta (bsp, userdata, salp, aln_row);
}

static Pointer DeltaLocToData (DialoG d)
{
  TagListPtr    tlp;
  ValNodePtr    result_list = NULL, vnp;
  CharPtr       str;
  Int4          start_pos, len;
  Boolean       is_known;
  GapLocInfoPtr glip;
  
  tlp = (TagListPtr) GetObjectExtra (d);
  
  if (tlp == NULL) return NULL;
  
  for (vnp = tlp->vnp;
       vnp != NULL;
       vnp = vnp->next)
  {
    str = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 0);
    if (!StringHasNoText(str))
    {
      start_pos = atoi (str) - 1;
      if (start_pos > 0)
      {
        str = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 1);
        if (StringICmp (str, "0"))
        {
          is_known = FALSE;
          len = 100;
        }
        else
        {
          is_known = TRUE;
          len = -1;
          str = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 2);
          if (!StringHasNoText (str))
          {
            len = atoi (str);
          }
          if (len < 1)
          {
            Message (MSG_ERROR, "Must supply a length greater than zero for gaps of known length!");
            result_list = ValNodeFreeData (result_list);
            return NULL;
          }
        }
        glip = (GapLocInfoPtr) MemNew (sizeof (GapLocInfoData));
        if (glip != NULL)
        {
          glip->start_pos = start_pos;
          glip->is_known = is_known;
          glip->length = len;
          ValNodeAddPointer (&result_list, 0, glip);
        }
      }
    }
  }
  return result_list;
}

static void DoConvertRawToDeltaWithGapLocations (ButtoN b)
{
  DeltaConversionPtr dcp;
  ValNodePtr         location_list;
  SeqEntryPtr        sep;

  dcp = (DeltaConversionPtr) GetObjectExtra (b);
  if (dcp == NULL)
  {
    return;
  }
  
  sep = GetTopSeqEntryForEntityID (dcp->input_entityID);
  if (sep == NULL)
  {
    return;
  }

  location_list = DialogToPointer (dcp->gap_locations);
  if (location_list == NULL)
  {
    Message (MSG_ERROR, "Must supply valid gap locations!");
    return;
  }
  
  Hide (dcp->form);
  WatchCursor ();
  Update (); 
  if (dcp->coord_grp == NULL || GetValue (dcp->coord_grp) == 1)
  {
    VisitBioseqsInSep (sep, location_list, ConvertBioseqToDeltaWithSequenceGapList);
  }
  else
  {
    VisitBioseqsInSep (sep, location_list, ConvertBioseqToDeltaWithAlignmentGapList);
  }
  location_list = ValNodeFreeData (location_list);
  ObjMgrSetDirtyFlag (dcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, dcp->input_entityID, 0, 0);
  Remove (dcp->form);    
  ArrowCursor ();
  Update (); 
}

extern DialoG CreateTagListDialogEx (GrouP h, Uint2 rows, Uint2 cols,
                                     Int2 spacing, Uint2Ptr types,
                                     Uint2Ptr textWidths, EnumFieldAssocPtr PNTR alists,
                                     Boolean useBar, Boolean noExtend,
                                     ToDialogFunc tofunc, FromDialogFunc fromfunc);

static void ConvertRawToDeltaWithGapLocations (IteM i)
{
  BaseFormPtr        bfp;
  DeltaConversionPtr dcp;
  WindoW             w;
  GrouP              h, p, c;
  ButtoN             b;
  PrompT             p1, p2, p3;
  SeqEntryPtr        sep;
  RecT               r1, r2;
  TagListPtr         tlp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  dcp = (DeltaConversionPtr) MemNew (sizeof (DeltaConversionData));
  if (dcp == NULL) return;
  
  dcp->input_entityID = bfp->input_entityID;
  sep = GetTopSeqEntryForEntityID (dcp->input_entityID);
  if (sep == NULL)
  {
    return;
  }
  w = FixedWindow (-50, -33, -10, -10, "Convert Raw Sequence to Delta Sequence", StdCloseWindowProc);
  SetObjectExtra (w, dcp, StdCleanupFormProc);
  dcp->form = (ForM) w;
  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
 
  p = HiddenGroup (h, 3, 0, NULL);
  p1 = StaticPrompt (p, "Start", 0, dialogTextHeight, programFont, 'c');
  p2 = StaticPrompt (p, "Type", 0, dialogTextHeight, programFont, 'c');
  p3 = StaticPrompt (p, "Length", 0, dialogTextHeight, programFont, 'c');
  dcp->gap_locations = CreateTagListDialogEx (h, 4, 3, 2,
                                              deltaconversionedittypes, 
                                              deltaconversionedit_widths,
                                              deltaconversionedit_alists,
                                              TRUE, FALSE, NULL, DeltaLocToData);
  dcp->coord_grp = NormalGroup (h, 2, 0, "Coordinates", programFont, NULL);
  RadioButton (dcp->coord_grp, "Sequence");
  RadioButton (dcp->coord_grp, "Alignment");
  SetValue (dcp->coord_grp, 1);
  if (!SeqEntryHasAligns (dcp->input_entityID, sep))
  {
    Disable (dcp->coord_grp);
  }
  
  c = HiddenGroup (h, 2, 0, NULL);
  b = PushButton (c, "Accept", DoConvertRawToDeltaWithGapLocations);
  SetObjectExtra (b, dcp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) dcp->gap_locations,
                              (HANDLE) dcp->coord_grp,
                              (HANDLE) c,
                              NULL);

  tlp = GetObjectExtra (dcp->gap_locations);
  if (tlp != NULL)
  {
    ObjectRect (tlp->control [0], &r1);
    ObjectRect (p1, &r2);
    r2.left = r1.left;
    r2.right = r1.right;
    SetPosition (p1, &r2);
    
    ObjectRect (tlp->control [1], &r1);
    ObjectRect (p2, &r2);
    r2.left = r1.left;
    r2.right = r1.right;
    SetPosition (p2, &r2);
    
    ObjectRect (tlp->control [2], &r1);
    ObjectRect (p3, &r2);
    r2.left = r1.left;
    r2.right = r1.right;
    SetPosition (p3, &r2);
  }
  RealizeWindow (w);
  
  Show (w);
  Update ();  
}

typedef struct xrefgenedata {
  GeneRefPtr  grp;
  SeqLocPtr   slp;
} XrefGeneData, PNTR XrefGenePtr;

static Boolean XrefToGeneCallback (GatherContextPtr gcp)

{
  GeneRefPtr           grp;
  SeqFeatXrefPtr PNTR  last;
  SeqFeatXrefPtr       next;
  SeqFeatPtr           sfp;
  ValNodePtr PNTR      vnpp;
  XrefGenePtr          xgp;
  SeqFeatXrefPtr       xref;

  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  vnpp = (ValNodePtr PNTR) gcp->userdata;
  if (vnpp == NULL) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->location == NULL) return TRUE;
  if (sfp->xref == NULL) return TRUE;
  grp = NULL;
  last = (SeqFeatXrefPtr PNTR) &(sfp->xref);
  xref = sfp->xref;
  while (xref != NULL) {
    next = xref->next;
    if (xref->data.choice == SEQFEAT_GENE) {
      *last = next;
      xref->next = NULL;
      grp = (GeneRefPtr) xref->data.value.ptrvalue;
      xref->data.value.ptrvalue = NULL;
      SeqFeatXrefFree (xref);
    } else {
      last = &(xref->next);
    }
    xref = next;
  }
  if (grp == NULL) return TRUE;
  xgp = MemNew (sizeof (XrefGeneData));
  if (xgp == NULL) return TRUE;
  xgp->grp = grp;
  xgp->slp = AsnIoMemCopy (sfp->location, (AsnReadFunc) SeqLocAsnRead,
                           (AsnWriteFunc) SeqLocAsnWrite);
  ValNodeAddPointer (vnpp, 0, xgp);
  return TRUE;
}

static void XrefToGene (IteM i)

{
  BaseFormPtr   bfp;
  BioseqPtr     bsp;
  Uint2         entityID = 0;
  GatherScope   gs;
  ValNodePtr    head;
  SeqEntryPtr   nsep;
  SeqFeatPtr    sfp;
  SelStructPtr  ssp;
  ValNodePtr    vnp;
  XrefGenePtr   xgp;

  head = NULL;
  ssp = ObjMgrGetSelected ();
  if (ssp == NULL) {
#ifdef WIN_MAC
    bfp = currentFormDataPtr;
#else
    bfp = GetObjectExtra (i);
#endif
    if (bfp == NULL) {
      Message (MSG_ERROR, "XrefToGene error");
      return;
    }
    entityID = bfp->input_entityID;
    MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
    gs.seglevels = 1;
    MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
    gs.ignore[OBJ_BIOSEQ] = FALSE;
    gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
    gs.ignore[OBJ_SEQANNOT] = FALSE;
    gs.ignore[OBJ_SEQFEAT] = FALSE;
    GatherEntity (bfp->input_entityID, (Pointer) &head, XrefToGeneCallback, &gs);
  } else {
    entityID = ssp->entityID;
    if (ssp->itemtype != OBJ_SEQFEAT) {
      Message (MSG_ERROR, "Feature must be selected");
      return;
    }
    GatherItem (ssp->entityID, ssp->itemID, ssp->itemtype, (Pointer) &head, XrefToGeneCallback);
  }
  if (head == NULL || entityID == 0) return;
  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    xgp = (XrefGenePtr) vnp->data.ptrvalue;
    if (xgp != NULL) {
      bsp = GetBioseqGivenSeqLoc (xgp->slp, entityID);
      if (bsp != NULL) {
        nsep = SeqMgrGetSeqEntryForData (bsp);
        if (! ExtendGene (xgp->grp, nsep, xgp->slp)) {
          sfp = CreateNewFeature (nsep, NULL, SEQFEAT_GENE, NULL);
          if (sfp != NULL) {
            sfp->data.value.ptrvalue = (Pointer) xgp->grp;
            xgp->grp = NULL;
            sfp->location = SeqLocFree (sfp->location);
            sfp->location = xgp->slp;
            xgp->slp = NULL;
          }
        }
      }
      GeneRefFree (xgp->grp);
      SeqLocFree (xgp->slp);
    }
  }
  ValNodeFreeData (head);
  ObjMgrSetDirtyFlag (entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
}

static void GeneToXrefCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BaseFormPtr     bfp;
  BioseqPtr       bsp;
  BioseqSetPtr    bssp;
  SeqFeatPtr      gene;
  GeneRefPtr      grp;
  SeqFeatPtr      sfp;
  SeqAnnotPtr     sap;
  SeqFeatXrefPtr  xref;

  if (sep == NULL) return;
  bfp = (BaseFormPtr) mydata;
  if (bfp == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        if (sfp->data.choice != SEQFEAT_GENE) {
          FindGeneAndProtForCDS (bfp->input_entityID, sfp, &gene, NULL);
          if (gene != NULL) {
            grp = (GeneRefPtr) gene->data.value.ptrvalue;
            if (grp != NULL) {
              xref = SeqFeatXrefNew ();
              if (xref != NULL) {
                xref->data.choice = SEQFEAT_GENE;
                xref->data.value.ptrvalue = AsnIoMemCopy ((Pointer) grp,
                                                          (AsnReadFunc) GeneRefAsnRead,
                                                          (AsnWriteFunc) GeneRefAsnWrite);
                xref->next = sfp->xref;
                sfp->xref = xref;
              }
            }
          }
        }
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
}

static void GeneToXref (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) {
    Message (MSG_ERROR, "GeneToXref error");
    return;
  }
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  SeqEntryExplore (sep, (Pointer) bfp, GeneToXrefCallback);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

typedef struct featuretogene
{
  FEATURE_FORM_BLOCK

  Uint1  subtype; 
  PopuP  qualchoice[3];
  PopuP  genechoice;
  ButtoN accept_btn;
  GrouP  qual_caps_grp;
  
  ButtoN single_interval_btn;
  ButtoN selected_features_only_btn;
  
  Boolean single_interval;
  
} FeatureToGeneData, PNTR FeatureToGenePtr;

static void EnableFeatureToGeneControlsPopup (PopuP p)
{
  FeatureToGenePtr fgp;
  Int4             i, j, val;
  
  fgp = (FeatureToGenePtr) GetObjectExtra (p);
  if (fgp == NULL) return;
  Disable (fgp->qual_caps_grp);
  Disable (fgp->genechoice);
  for (i = 0; i < 2; i++)
  {
    val = GetValue (fgp->qualchoice[i]);
  	if (val == 1)
  	{
  	  for (j = i + 1; i < 3; i++)
  	  {
  	  	Disable (fgp->qualchoice[j]);
  	  	SetValue (fgp->qualchoice[j], 1);
  	  }
  	}
  	else
  	{
  	  Enable (fgp->qualchoice [i + 1]);
   	  Enable (fgp->qual_caps_grp);
   	  Enable (fgp->genechoice);
 	}
  }
}

static PopuP MakeCDSPopup (GrouP g, FeatureToGenePtr fgp)
{
  PopuP p;
  
  p = PopupList (g, TRUE, EnableFeatureToGeneControlsPopup);
  SetObjectExtra (p, fgp, NULL);
  PopupItem (p, "None");
  PopupItem (p, "Comment");
  PopupItem (p, "Product");
  SetValue (p, 1);
  return p;	
}

static CharPtr GetCDSPopupText (PopuP p, SeqFeatPtr cds)
{
  Int4              val;
  SeqMgrFeatContext context;
  if (p == NULL || cds == NULL) return NULL;
  val = GetValue (p);
  
  if (val == 2 && !StringHasNoText (cds->comment))
  {
  	return StringSave (cds->comment);
  }
  else if (val == 3)
  {
    cds = SeqMgrGetDesiredFeature (cds->idx.entityID, NULL, 0, 0, cds, &context);
    if (cds == NULL || StringHasNoText (context.label)) return NULL;
    return StringSave (context.label);
  }
  else
  {
  	return NULL;
  }
}

static PopuP MaketRNAPopup (GrouP g, FeatureToGenePtr fgp)
{
  PopuP p;
  
  p = PopupList (g, TRUE, EnableFeatureToGeneControlsPopup);
  SetObjectExtra (p, fgp, NULL);
  PopupItem (p, "None");
  PopupItem (p, "Comment");
  SetValue (p, 1);
  return p;		
}

static CharPtr GettRNAPopupText (PopuP p, SeqFeatPtr cds)
{
  Int4 val;
  if (p == NULL || cds == NULL) return NULL;
  val = GetValue (p);
  
  if (val == 2 && !StringHasNoText (cds->comment))
  {
  	return StringSave (cds->comment);
  }
  else
  {
  	return NULL;
  }
}

static PopuP MakerRNAPopup (GrouP g, FeatureToGenePtr fgp)
{
  PopuP p;
  
  p = PopupList (g, TRUE, EnableFeatureToGeneControlsPopup);
  SetObjectExtra (p, fgp, NULL);
  PopupItem (p, "None");
  PopupItem (p, "Comment");
  PopupItem (p, "Product Name");
  SetValue (p, 1);
  return p;		
}

static CharPtr GetrRNAPopupText (PopuP p, SeqFeatPtr sfp)
{
  Int4 val;
  RnaRefPtr    rrp;

  if (p == NULL || sfp == NULL || sfp->data.choice != SEQFEAT_RNA) return NULL;
  
  val = GetValue (p);
  rrp = (RnaRefPtr) (sfp->data.value.ptrvalue);

  if (val == 2 && !StringHasNoText (sfp->comment))
  {
  	return StringSave (sfp->comment);
  }
  else if (val == 3 && rrp != NULL && rrp->ext.choice == 1 
           && !StringHasNoText (rrp->ext.value.ptrvalue))
  {
  	return StringSave (rrp->ext.value.ptrvalue);
  }
  else
  {
  	return NULL;
  }
}

static PopuP MakemiscRNAPopup (GrouP g, FeatureToGenePtr fgp)
{
  PopuP p;
  
  p = PopupList (g, TRUE, EnableFeatureToGeneControlsPopup);
  SetObjectExtra (p, fgp, NULL);
  PopupItem (p, "None");
  PopupItem (p, "Comment");
  PopupItem (p, "Product Name");
  SetValue (p, 1);
  return p;			
}

static CharPtr GetmiscRNAPopupText (PopuP p, SeqFeatPtr sfp)
{
  Int4 val;
  RnaRefPtr    rrp;

  if (p == NULL || sfp == NULL || sfp->data.choice != SEQFEAT_RNA) return NULL;
  
  val = GetValue (p);
  rrp = (RnaRefPtr) (sfp->data.value.ptrvalue);

  if (val == 2 && !StringHasNoText (sfp->comment))
  {
  	return StringSave (sfp->comment);
  }
  else if (val == 3 && rrp != NULL && rrp->ext.choice == 1 
           && !StringHasNoText (rrp->ext.value.ptrvalue))
  {
  	return StringSave (rrp->ext.value.ptrvalue);
  }
  else
  {
  	return NULL;
  }
}

static void FeatureToGeneCallback (SeqFeatPtr sfp, Pointer userdata)
{
  SeqFeatPtr         gene, overlap_gene;
  GeneRefPtr         grp, overlap_grp = NULL;
  Boolean            partial5, partial3;
  FeatureToGenePtr   fgp;
  CharPtr            gene_val = NULL;
  Int4               i;
  BioseqPtr          bsp;
  CharPtr            cp;
  SeqLocPtr          gene_location;

  if (sfp == NULL || userdata == NULL) return;
  fgp = (FeatureToGenePtr) userdata;
  
  if (sfp->idx.subtype != fgp->subtype) return;
  if (SeqMgrGetGeneXref (sfp) != NULL) return;
  
  bsp = BioseqFindFromSeqLoc (sfp->location);
  if (bsp == NULL) return;
  
  gene_location = SeqLocMerge (bsp, sfp->location, NULL, fgp->single_interval, FALSE, FALSE);
  if (gene_location == NULL) return;
  
  overlap_gene = SeqMgrGetOverlappingGene (sfp->location, NULL);
  if (overlap_gene != NULL && SeqLocCompare (gene_location, overlap_gene->location) == SLC_A_EQ_B)
  {
    gene_location = SeqLocFree (gene_location);
    return;
  }
  
  for (i = 0; gene_val == NULL && i < 2 && GetValue (fgp->qualchoice[i]) != 1; i++)
  {
    switch (fgp->subtype)
    {
      case FEATDEF_CDS:
  	    gene_val = GetCDSPopupText (fgp->qualchoice[i], sfp);
  	    break;
      case FEATDEF_tRNA:
   	    gene_val = GettRNAPopupText (fgp->qualchoice[i], sfp);
  	    break;
      case FEATDEF_rRNA:
   	    gene_val = GetrRNAPopupText (fgp->qualchoice[i], sfp);
  	    break;
      case FEATDEF_otherRNA:
   	    gene_val = GetmiscRNAPopupText (fgp->qualchoice[i], sfp);
  	    break;
    }
  }
  if (gene_val != NULL)
  {
  	/* apply capitalization */
	  switch (GetValue (fgp->qual_caps_grp))
  	{
  	  case 1:
  	    /* do nothing, leave capitalization as is */
  	    break;
  	  case 2:
  	    /* capitalize first letter */
  	    gene_val [0] = toupper (gene_val[0]);
  	    break;
  	  case 3:
  	  	/* capitalize all letters */
  	  	for (cp = gene_val; *cp != 0; cp++)
  	  	{
  	  	  *cp = toupper (*cp);
  	  	}
  	  	break;
  	}
  }

  gene = SeqFeatNew ();
  if (gene == NULL) return;
    
  grp = GeneRefNew ();
  if (grp == NULL) return;
  i = GetValue (fgp->genechoice);
  switch (i)
  {
  	case 1:
  	  grp->locus = gene_val;
  	  break;
  	case 2:
  	  grp->locus_tag = gene_val;
  	  break;
  	case 3:
  	  grp->desc = gene_val;
  	  break;
  	case 4:
  	  grp->allele = gene_val;
  	  break;
  	default:
  	  gene->comment = gene_val;
  	  break;
  }
    
  gene->data.choice = SEQFEAT_GENE;
  gene->data.value.ptrvalue = (Pointer) grp;
  gene->location = gene_location;
  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
  SetSeqLocPartial (gene->location, partial5, partial3);
  gene->partial = sfp->partial;
  gene->next = sfp->next;
  sfp->next = gene;
}

static void FeatureToGeneAccept (ButtoN b)
{
  FeatureToGenePtr  fgp;
  SeqEntryPtr       sep;
  SelStructPtr      sel;
  SeqFeatPtr        sfp;
  SeqMgrFeatContext fcontext;
  
  fgp = (FeatureToGenePtr) GetObjectExtra (b);
  if (fgp == NULL) return;
  Hide (fgp->form);
  
  sep = GetTopSeqEntryForEntityID (fgp->input_entityID);
  if (sep == NULL) return;
  fgp->single_interval = GetStatus (fgp->single_interval_btn);
  if (GetStatus (fgp->selected_features_only_btn))
  {
    sel = ObjMgrGetSelected ();
    while (sel != NULL)
    {
      if (sel->itemtype == OBJ_SEQFEAT)
      {
        sfp = SeqMgrGetDesiredFeature (sel->entityID, NULL, sel->itemID, 0, NULL, &fcontext);
        if (sfp != NULL)
        {
          FeatureToGeneCallback (sfp, fgp);
        }
      }
      sel = sel->next;
    }
  }
  else
  {
    VisitFeaturesInSep (sep, fgp, FeatureToGeneCallback);    
  }
  ObjMgrSetDirtyFlag (fgp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, fgp->input_entityID, 0, 0);
  Remove (fgp->form);
}

static void FeatureToGene (IteM i, Uint1 subtype)
{
  BaseFormPtr       bfp;
  FeatureToGenePtr  fgp;
  WindoW            w;
  Int4              j;
  CharPtr           choice_labels[3] = { "1st Choice", "2nd Choice", "3rd Choice" };
  GrouP             h, g, k, m, n, c;
  ButtoN            b;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) {
    Message (MSG_ERROR, "Feature to Gene error");
    return;
  }
  
  fgp = (FeatureToGenePtr) MemNew (sizeof (FeatureToGeneData));
  if (fgp == NULL) return;
  fgp->subtype = subtype;
  fgp->input_entityID = bfp->input_entityID;
  switch (fgp->subtype)
  {
  	case FEATDEF_CDS:
      w = FixedWindow (-50, -33, -10, -10, "CDS to Gene", StdCloseWindowProc);
  	  break;
  	case FEATDEF_tRNA:
      w = FixedWindow (-50, -33, -10, -10, "tRNA to Gene", StdCloseWindowProc);
  	  break;
  	case FEATDEF_rRNA:
      w = FixedWindow (-50, -33, -10, -10, "rRNA to Gene", StdCloseWindowProc);
  	  break;
  	case FEATDEF_otherRNA:
      w = FixedWindow (-50, -33, -10, -10, "miscRNA to Gene", StdCloseWindowProc);
  	  break;
  	default:
  	  Message (MSG_ERROR, "Unrecognized subtype for feature to gene");
  	  return;
  	  break;
  }
  SetObjectExtra (w, fgp, NULL);
  fgp->form = (ForM) w;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 0, 6, NULL);

  k = NormalGroup (g, 2, 0, "Select qualifier to use in gene", NULL, NULL);
  for (j = 0; j < 2; j++)
  {
    StaticPrompt (k, choice_labels [j], 0, 0, programFont, 'c');
    switch (fgp->subtype)
    {
  	  case FEATDEF_CDS:
  	    fgp->qualchoice[j] = MakeCDSPopup (k, fgp);
  	    break;
  	  case FEATDEF_tRNA:
  	    fgp->qualchoice[j] = MaketRNAPopup (k, fgp);
  	    break;
  	  case FEATDEF_rRNA:
  	    fgp->qualchoice[j] = MakerRNAPopup (k, fgp);
  	    break;
  	  case FEATDEF_otherRNA:
  	    fgp->qualchoice[j] = MakemiscRNAPopup (k, fgp);
  	    break;  
    }
  }  
  fgp->qual_caps_grp = NormalGroup (g, 3, 0,
                     "Capitalization for gene", NULL, NULL);
  RadioButton (fgp->qual_caps_grp, "As is");
  RadioButton (fgp->qual_caps_grp, "Capitalize first initial");
  RadioButton (fgp->qual_caps_grp, "Capitalize all");
  SetValue (fgp->qual_caps_grp, 1);
  Disable (fgp->qual_caps_grp);
  m = HiddenGroup (g, 2, 0, NULL);
  StaticPrompt (m, "Select gene qualifier to populate", 0,0, programFont, 'c');
  fgp->genechoice = PopupList (m, TRUE, NULL);
  PopupItem (fgp->genechoice, "locus");
  PopupItem (fgp->genechoice, "locus_tag");
  PopupItem (fgp->genechoice, "gene description");
  PopupItem (fgp->genechoice, "allele");
  PopupItem (fgp->genechoice, "gene comment");
  SetValue (fgp->genechoice, 1);
  Disable (fgp->genechoice);
  
  n = HiddenGroup (g, 2, 0, NULL);
  fgp->single_interval_btn = CheckBox (n, "Create gene with single interval location", NULL);
  SetStatus (fgp->single_interval_btn, TRUE);
  fgp->selected_features_only_btn = CheckBox (n, "Only create genes for selected features", NULL);
  SetStatus (fgp->selected_features_only_btn, FALSE);
  
  c = HiddenGroup (g, 4, 0, NULL); 
  b = PushButton (c, "Accept", FeatureToGeneAccept);
  SetObjectExtra (b, fgp, NULL);
  
  PushButton (c, "Cancel", StdCancelButtonProc);


  AlignObjects (ALIGN_CENTER, (HANDLE) k, (HANDLE) fgp->qual_caps_grp, (HANDLE) n,
                (HANDLE) c, NULL);
                
  RealizeWindow (w);
  Show (w);
  Update ();
}

static void CdsToGene (IteM i)
{
  FeatureToGene (i, FEATDEF_CDS);
}

static void tRNAtoGene (IteM i)
{
  FeatureToGene (i, FEATDEF_tRNA);
}

static void rRNAtoGene (IteM i)
{
  FeatureToGene (i, FEATDEF_rRNA);
}

static void MiscRNAtoGene (IteM i)
{
  FeatureToGene (i, FEATDEF_otherRNA);
}

typedef struct genetocdsform
{
  FEATURE_FORM_BLOCK

  PopuP   gene_qual_popup;
  PopuP   gene_qual_popup2;
  PopuP   gene_qual_popup3;
  GrouP   qual_caps_grp;
  TexT    append_text;
  ButtoN  remove_qual_btn1;
  ButtoN  remove_qual_btn2;
  ButtoN  remove_qual_btn3;
  ButtoN  accept_btn;
  
  Int4    gene_qual_choice;
  Int4    gene_qual_choice2;
  Int4    gene_qual_choice3;
  CharPtr append_string;
  Int4    caps_choice;
  Boolean remove_gene_qual1;
  Boolean remove_gene_qual2;
  Boolean remove_gene_qual3;
  SeqEntryPtr sep;
  FILE    *log_fp;
  Boolean data_in_log;
  
} GeneToCDSFormData, PNTR GeneToCDSFormPtr;

static CharPtr GetQualStringForQualChoice (GeneRefPtr grp, SeqFeatPtr sfp, Int4 choice)
{
  CharPtr str = NULL;
  switch (choice)
  {
  	case 2:
  	  str = grp->locus;
  	  break;
  	case 3:
  	  str =  grp->desc;
  	  break;
  	case 4:
  	  str =  sfp->comment;
  	  break;
  }
  return str;
}

static Int4 GetGeneQualChoice (GeneToCDSFormPtr gcfp, SeqFeatPtr sfp, GeneRefPtr grp)
{
  if (gcfp == NULL) return 0;
  
  if (gcfp->gene_qual_choice != 1 
    && StringHasNoText (GetQualStringForQualChoice (grp, sfp, gcfp->gene_qual_choice)))
  {
    if (gcfp->gene_qual_choice2 != 1 
      && StringHasNoText (GetQualStringForQualChoice (grp, sfp, gcfp->gene_qual_choice2)))
    {
      return gcfp->gene_qual_choice3;
    }
    else
    {
      return gcfp->gene_qual_choice2;	  	
    }
  }
  else
  {
  	return gcfp->gene_qual_choice;
  }
}

static void LogGeneToCDSNoProtein (GeneToCDSFormPtr gcfp, CharPtr gene_label, SeqIdPtr sip)
{
  Char       id [41];

  if (gcfp == NULL || gcfp->log_fp == NULL || sip == NULL) return;

  id [0] = '\0';
  SeqIdWrite (SeqIdFindBest (sip, 0), id, PRINTID_FASTA_LONG, sizeof (id) - 1);
  
  if (StringHasNoText (gene_label))
  {
    fprintf (gcfp->log_fp, "No protein added for unlabeled gene on sequence %s\n", id);
  }
  else
  {
    fprintf (gcfp->log_fp, "No protein added for %s on sequence %s\n", gene_label, id);
  }
  gcfp->data_in_log = TRUE;
}

static void GeneToCDSCallback (SeqFeatPtr sfp, Pointer userdata)

{
  SeqFeatPtr        cds;
  GeneRefPtr        grp;
  SeqMgrFeatContext context;
  Int2              genCode;
  CdRegionPtr       crp;
  ByteStorePtr      bs;
  BioseqPtr         bsp;
  SeqEntryPtr       psep, nsep, old, topsep;
  MolInfoPtr        mip;
  ProtRefPtr        prp;
  Char              ch;
  ValNodePtr        descr;
  Int2              i;
  Boolean           partial5, partial3;
  CharPtr           prot;
  CharPtr           ptr;
  ValNodePtr        vnp;
  CharPtr           protName = NULL;
  GeneToCDSFormPtr  gcfp;
  Int4              prot_len;
  CharPtr           cp;
  Int4              gene_qual_choice;
  CharPtr           qual_val = NULL;
  
  if (sfp == NULL || sfp->data.choice != SEQFEAT_GENE || userdata == NULL)
	return;

  gcfp = (GeneToCDSFormPtr) userdata;

  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);

  cds = SeqMgrGetOverlappingCDS (sfp->location, &context);
  if (cds != NULL) 
  {
    return;	
  }

  if (SeqMgrGetDesiredFeature (sfp->idx.entityID, NULL,
                               0, 0, sfp, &context) == NULL) return;

  grp = (GeneRefPtr) sfp->data.value.ptrvalue;
  
  /* use the controls to determine the protein name */
  prot_len = 0;
  
  gene_qual_choice = GetGeneQualChoice (gcfp, sfp, grp);
  qual_val = GetQualStringForQualChoice (grp, sfp, gene_qual_choice);

  
  if (gene_qual_choice == 1)
  {
  	if (StringHasNoText (gcfp->append_string))
    {
      LogGeneToCDSNoProtein (gcfp, context.label, SeqLocId (sfp->location));
  	  return;
  	}
  }
  else
  {
    if (StringHasNoText (qual_val))
  	{
  	  LogGeneToCDSNoProtein (gcfp, context.label, SeqLocId (sfp->location));
  	  return;
  	}
  	prot_len += StringLen (qual_val);
  }

  if (!StringHasNoText (gcfp->append_string))
  {
  	if (prot_len > 0)
  	{
  	  prot_len ++;
  	}
  	prot_len += StringLen (gcfp->append_string);
  }
  if (prot_len == 0)
  {
  	LogGeneToCDSNoProtein (gcfp, context.label, SeqLocId (sfp->location));
  	return;
  }

  /* add one to length for terminating null */
  prot_len ++;
  protName = (CharPtr) MemNew (prot_len * sizeof (Char));
  if (protName == NULL) return;
  
  if (gene_qual_choice != 1 && !StringHasNoText (qual_val))
  {
    StringCpy (protName, qual_val);
  	/* apply capitalization */
	switch (gcfp->caps_choice)
  	{
  	  case 1:
  	    /* do nothing, leave capitalization as is */
  	    break;
  	  case 2:
  	    /* capitalize first letter */
  	    protName [0] = toupper (protName[0]);
  	    break;
  	  case 3:
  	  	/* capitalize all letters */
  	  	for (cp = protName; *cp != 0; cp++)
  	  	{
  	  	  *cp = toupper (*cp);
  	  	}
  	  	break;
  	}
  	if (!StringHasNoText (gcfp->append_string))
  	{
  	  StringCat (protName, " ");
  	}
  }
  if (!StringHasNoText (gcfp->append_string))
  {
    StringCat (protName, gcfp->append_string);
  }
  
  if ((gcfp->gene_qual_choice == gene_qual_choice && gcfp->remove_gene_qual1)
    || (gcfp->gene_qual_choice2 == gene_qual_choice && gcfp->remove_gene_qual2)
    || (gcfp->gene_qual_choice3 == gene_qual_choice && gcfp->remove_gene_qual3))
  {
  	switch (gene_qual_choice)
  	{
  	  case 1:
  	    /* do nothing - not using a gene qualifier */
  	    break;
  	  case 2:
  	    grp->locus = MemFree (grp->locus);
  	    break;
  	  case 3:
  	    grp->desc = MemFree (grp->desc);
  	    break;
  	  case 4:
  	    sfp->comment= MemFree (sfp->comment);
  	    break;  		
  	}
  }
  
  cds = SeqFeatNew ();
  if (cds == NULL) return;
  cds->data.choice = SEQFEAT_CDREGION;

  genCode = SeqEntryToGeneticCode (gcfp->sep, NULL, NULL, 0);
  crp = CreateNewCdRgn (1, FALSE, genCode);
  if (crp == NULL) {
    return;
  }
 
  cds->data.value.ptrvalue = (Pointer) crp;
  cds->location = SeqLocFree (cds->location);
  cds->location = AsnIoMemCopy ((Pointer) sfp->location,
                                (AsnReadFunc) SeqLocAsnRead,
                                (AsnWriteFunc) SeqLocAsnWrite);
  cds->partial = sfp->partial;
  cds->next = sfp->next;
  sfp->next = cds;
 
  bs = ProteinFromCdRegionEx (cds, TRUE, FALSE);
  if (bs != NULL) {
    prot = BSMerge (bs, NULL);
    bs = BSFree (bs);
    if (prot != NULL) {
      ptr = prot;
      ch = *ptr;
      while (ch != '\0') {
        *ptr = TO_UPPER (ch);
        ptr++;
        ch = *ptr;
      }
      i = (Int2) StringLen (prot);
      if (i > 0 && prot [i - 1] == '*') {
        prot [i - 1] = '\0';
      }
      bs = BSNew (1000);
	  if (bs != NULL) {
        ptr = prot;
        BSWrite (bs, (VoidPtr) ptr, (Int4) StringLen (ptr));
      }
    }
  }
  bsp = BioseqNew ();
  if (bsp == NULL) return;
  bsp->repr = Seq_repr_raw;
  bsp->mol = Seq_mol_aa;
  bsp->seq_data_type = Seq_code_ncbieaa;
  bsp->seq_data = bs;
  bsp->length = BSLen (bs);
  bs = NULL;
  old = SeqEntrySetScope (NULL);
  bsp->id = MakeNewProteinSeqId (cds->location, NULL);
  SeqMgrAddToBioseqIndex (bsp);
  SeqEntrySetScope (old);
  psep = SeqEntryNew ();
  if (psep != NULL) {
    psep->choice = 1;
    psep->data.ptrvalue = (Pointer) bsp;
    SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, psep);
    mip = MolInfoNew ();
    if (mip != NULL) {
      mip->biomol = 8;
      mip->tech = 8;
      if (partial5 && partial3) {
        mip->completeness = 5;
      } else if (partial5) {
        mip->completeness = 3;
      } else if (partial3) {
        mip->completeness = 4;
      }
      vnp = CreateNewDescriptor (psep, Seq_descr_molinfo);
      if (vnp != NULL) {
        vnp->data.ptrvalue = (Pointer) mip;
      }
    }
    descr = ExtractBioSourceAndPubs (gcfp->sep);
    if (IS_Bioseq_set (gcfp->sep))
    {
      topsep = gcfp->sep;
    }
    else
    {
      topsep = GetBestTopParentForData (gcfp->input_entityID, gcfp->sep->data.ptrvalue);
    }
    AddSeqEntryToSeqEntry (topsep, psep, TRUE);
    nsep = FindNucSeqEntry (gcfp->sep);
    ReplaceBioSourceAndPubs (gcfp->sep, descr);
    SetSeqFeatProduct (cds, bsp);
    prp = CreateNewProtRef (protName, NULL, NULL, NULL);
    MemFree (protName);
    sfp = CreateNewFeature (psep, NULL, SEQFEAT_PROT, NULL);
    if (sfp != NULL) {
       sfp->data.value.ptrvalue = (Pointer) prp;
    }
  }
}

static void GeneToCDSBioseqCallback (BioseqPtr bsp, Pointer userdata)

{
  GeneToCDSFormPtr gcfp;

  if (bsp == NULL || userdata == NULL) return;
  gcfp = (GeneToCDSFormPtr) userdata;
  gcfp->sep = SeqMgrGetSeqEntryForData (bsp);
  VisitFeaturesOnBsp (bsp, (Pointer) gcfp, GeneToCDSCallback);
}

static void GeneToCdsAccept (ButtoN b)
{
  GeneToCDSFormPtr gcfp;
  Char         path [PATH_MAX];

  gcfp = (GeneToCDSFormPtr) GetObjectExtra (b);
  if (gcfp == NULL) return;
  TmpNam (path);
  gcfp->log_fp = FileOpen (path, "wb");
  if (gcfp->log_fp == NULL) return;
  
  Hide (gcfp->form);
  WatchCursor ();
  Update ();

  gcfp->sep = GetTopSeqEntryForEntityID (gcfp->input_entityID);
  if (gcfp->sep == NULL) return;
  
  gcfp->gene_qual_choice = GetValue (gcfp->gene_qual_popup);
  gcfp->gene_qual_choice2 = GetValue (gcfp->gene_qual_popup2);
  gcfp->gene_qual_choice3 = GetValue (gcfp->gene_qual_popup3);
  gcfp->append_string = SaveStringFromText (gcfp->append_text);
  gcfp->caps_choice = GetValue (gcfp->qual_caps_grp);
  gcfp->remove_gene_qual1 = GetStatus (gcfp->remove_qual_btn1);
  gcfp->remove_gene_qual2 = GetStatus (gcfp->remove_qual_btn2);
  gcfp->remove_gene_qual3 = GetStatus (gcfp->remove_qual_btn3);
  
  VisitBioseqsInSep (gcfp->sep, (Pointer) gcfp, GeneToCDSBioseqCallback);
  
  ArrowCursor ();
  Update ();
  Remove (gcfp->form); 
  FileClose (gcfp->log_fp);
  if (gcfp->data_in_log)
  {
    LaunchGeneralTextViewer (path, "Genes without Proteins");
  }
  FileRemove (path);

  ObjMgrSetDirtyFlag (gcfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, gcfp->input_entityID, 0, 0);  
}

static void CleanupGeneToCDSPage (GraphiC g, VoidPtr data)
{
  GeneToCDSFormPtr gcfp;

  gcfp = (GeneToCDSFormPtr) data;
  if (gcfp != NULL) 
  {
    gcfp->append_string = MemFree (gcfp->append_string);
    MemFree (gcfp);
  }
}


static void EnableGeneToCdsControls (GeneToCDSFormPtr gcfp)
{
  CharPtr cp;
  
  if (gcfp == NULL) return;
  
  if (GetValue (gcfp->gene_qual_popup) == 1)
  {
  	Disable (gcfp->remove_qual_btn1);
  	SetStatus (gcfp->remove_qual_btn1, FALSE);
    Disable (gcfp->qual_caps_grp);
    SetValue (gcfp->gene_qual_popup2, 1);
    Disable (gcfp->remove_qual_btn2);
  	SetStatus (gcfp->remove_qual_btn2, FALSE);
  	Disable (gcfp->gene_qual_popup2);
    SetValue (gcfp->gene_qual_popup3, 1);
  	Disable (gcfp->gene_qual_popup3);
  	Disable (gcfp->remove_qual_btn3);
  	SetStatus (gcfp->remove_qual_btn3, FALSE);
    cp = SaveStringFromText (gcfp->append_text);
    if (StringHasNoText (cp))
    {
      Disable (gcfp->accept_btn);
    }
    else
    {
      Enable (gcfp->accept_btn);
    }
    MemFree (cp);
  }
  else
  {
  	Enable (gcfp->remove_qual_btn1);
  	Enable (gcfp->qual_caps_grp);
  	Enable (gcfp->gene_qual_popup2);
  	Enable (gcfp->accept_btn);
  	if (GetValue (gcfp->gene_qual_popup2) == 1)
  	{
   	  Disable (gcfp->remove_qual_btn2);
   	  SetStatus (gcfp->remove_qual_btn2, FALSE);
 	  SetValue (gcfp->gene_qual_popup3, 1);
  	  Disable (gcfp->gene_qual_popup3);
  	  Disable (gcfp->remove_qual_btn3);
  	  SetStatus (gcfp->remove_qual_btn3, FALSE);
  	}
  	else
  	{
  	  Enable (gcfp->gene_qual_popup3);
  	  Enable (gcfp->remove_qual_btn2);
  	  if (GetValue (gcfp->gene_qual_popup3) == 1)
  	  {
  	  	Disable (gcfp->remove_qual_btn3);
  	  	SetStatus (gcfp->remove_qual_btn3, FALSE);
  	  }
  	  else
  	  {
  	  	Enable (gcfp->remove_qual_btn3);
  	  }
  	}
  }
}

static void EnableGeneToCdsControlsPopup (PopuP p)
{
  GeneToCDSFormPtr gcfp;
  
  gcfp = (GeneToCDSFormPtr) GetObjectExtra (p);
  EnableGeneToCdsControls (gcfp);  
}

static void EnableGeneToCdsControlsText (TexT t)
{
  GeneToCDSFormPtr gcfp;
  
  gcfp = (GeneToCDSFormPtr) GetObjectExtra (t);
  EnableGeneToCdsControls (gcfp);  
}

static void GeneToCds (IteM i)

{
  BaseFormPtr      bfp;
  GeneToCDSFormPtr gcfp;
  WindoW           w;
  GrouP            h, g, k, m, c;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) {
    Message (MSG_ERROR, "GeneToCDS error");
    return;
  }
  
  gcfp = (GeneToCDSFormPtr) MemNew (sizeof (GeneToCDSFormData));
  if (gcfp == NULL) return;
  gcfp->input_entityID = bfp->input_entityID;
  gcfp->input_itemID = bfp->input_itemID;
  gcfp->input_itemtype = bfp->input_itemtype;
  
  w = FixedWindow (-50, -33, -10, -10, "Gene to CDS", StdCloseWindowProc);
  SetObjectExtra (w, gcfp, CleanupGeneToCDSPage);
  gcfp->form = (ForM) w;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 0, 6, NULL);

  k = NormalGroup (g, 3, 0, "Select gene qualifier to use in protein name", NULL, NULL);
  StaticPrompt (k, "1st Choice", 0, 0, programFont, 'c');
  gcfp->gene_qual_popup = PopupList (k, TRUE, EnableGeneToCdsControlsPopup);
  SetObjectExtra (gcfp->gene_qual_popup, gcfp, NULL);
  PopupItem (gcfp->gene_qual_popup, "None");
  PopupItem (gcfp->gene_qual_popup, "locus");
  PopupItem (gcfp->gene_qual_popup, "description");
  PopupItem (gcfp->gene_qual_popup, "comment");
  SetValue (gcfp->gene_qual_popup, 1);
  gcfp->remove_qual_btn1 = CheckBox (k, "Remove if used", NULL);
  StaticPrompt (k, "2nd Choice", 0, 0, programFont, 'c');  
  gcfp->gene_qual_popup2 = PopupList (k, TRUE, EnableGeneToCdsControlsPopup);
  SetObjectExtra (gcfp->gene_qual_popup2, gcfp, NULL);
  PopupItem (gcfp->gene_qual_popup2, "None");
  PopupItem (gcfp->gene_qual_popup2, "locus");
  PopupItem (gcfp->gene_qual_popup2, "description");
  PopupItem (gcfp->gene_qual_popup2, "comment");
  SetValue (gcfp->gene_qual_popup2, 1);
  gcfp->remove_qual_btn2 = CheckBox (k, "Remove if used", NULL);
  StaticPrompt (k, "3rd Choice", 0, 0, programFont, 'c');  
  gcfp->gene_qual_popup3 = PopupList (k, TRUE, EnableGeneToCdsControlsPopup);
  SetObjectExtra (gcfp->gene_qual_popup3, gcfp, NULL);
  PopupItem (gcfp->gene_qual_popup3, "None");
  PopupItem (gcfp->gene_qual_popup3, "locus");
  PopupItem (gcfp->gene_qual_popup3, "description");
  PopupItem (gcfp->gene_qual_popup3, "comment");
  SetValue (gcfp->gene_qual_popup3, 1);
  gcfp->remove_qual_btn3 = CheckBox (k, "Remove if used", NULL);
  
  gcfp->qual_caps_grp = NormalGroup (g, 3, 0,
                     "Capitalization for gene qualifier in protein name", NULL, NULL);
  RadioButton (gcfp->qual_caps_grp, "As is");
  RadioButton (gcfp->qual_caps_grp, "Capitalize first initial");
  RadioButton (gcfp->qual_caps_grp, "Capitalize all");
  SetValue (gcfp->qual_caps_grp, 1);
  
  m = HiddenGroup (g, 0, 2, NULL);
  StaticPrompt (m, "Append text to protein name", 0, 0, programFont, 'c');
  gcfp->append_text = DialogText (m, "", 14, EnableGeneToCdsControlsText); 
  SetObjectExtra (gcfp->append_text, gcfp, NULL);
   
  c = HiddenGroup (g, 4, 0, NULL); 
  gcfp->accept_btn = PushButton (c, "Accept", GeneToCdsAccept);
  SetObjectExtra (gcfp->accept_btn, gcfp, NULL);
  EnableGeneToCdsControls (gcfp);
  
  PushButton (c, "Cancel", StdCancelButtonProc);


  AlignObjects (ALIGN_CENTER, (HANDLE) k, (HANDLE) gcfp->qual_caps_grp,
                (HANDLE) m, (HANDLE) c, NULL);
                
  RealizeWindow (w);
  Show (w);
  Update ();
  
}

typedef struct featuretocdsform
{
  FEATURE_FORM_BLOCK
  PopuP   feature_choice;
  
  DialoG  gene_field;
  DialoG  exon_field;
  DialoG  mrna_field;
  GrouP   qual_caps_grp;
  TexT    append_text;
  ButtoN  accept_btn;
  ButtoN  fuse_multiple_btn;
  
  Int4       featdef_choice;
  ValNodePtr gene_field_list;
  ValNodePtr mrna_field_list;
  ValNodePtr exon_field_list;
  CharPtr    append_string;
  Int4       caps_choice;
  Boolean    fuse_multiple;
  SeqEntryPtr sep;
  FILE        *log_fp;
  Boolean     data_in_log;
  
} FeatureToCDSFormData, PNTR FeatureToCDSFormPtr;

#define FEATURE_TO_CDS_GENE 1
#define FEATURE_TO_CDS_MRNA 2
#define FEATURE_TO_CDS_EXON 3

static void EnableFeatureToCDSControls (Pointer data)
{
  FeatureToCDSFormPtr    fp;
  Int4                   feature_choice;
  Boolean                need_name_string = FALSE;
  ValNodePtr             missing = NULL;
  CharPtr                cp;

  fp = (FeatureToCDSFormPtr) data;
  if (fp == NULL)
  {
    return;
  }
  Enable (fp->accept_btn);
  feature_choice = GetValue (fp->feature_choice);
  if (feature_choice == FEATURE_TO_CDS_GENE)
  {
    missing = TestDialog (fp->gene_field);
    if (missing != NULL)
    {
      need_name_string = TRUE;
      ValNodeFree (missing);
    }
  }
  else if (feature_choice == FEATURE_TO_CDS_EXON)
  {
    missing = DialogToPointer (fp->exon_field);
    if (missing == NULL)
    {
      need_name_string = TRUE;
    }
    ValNodeFree (missing);
  }
  else if (feature_choice == FEATURE_TO_CDS_MRNA)
  {
    missing = DialogToPointer (fp->mrna_field);
    if (missing == NULL)
    {
      need_name_string = TRUE;
    }
    ValNodeFree (missing);
  }
  
  if (need_name_string)
  {
    Disable (fp->qual_caps_grp);
    cp = SaveStringFromText (fp->append_text);
    if (StringHasNoText (cp))
    {
      Disable (fp->accept_btn);
    }
    MemFree (cp);
  }
  else
  {
    Enable (fp->qual_caps_grp);
  }
}

static void EnableFeatureToCDSControlsText (TexT t)
{
  FeatureToCDSFormPtr    fp;

  fp = (FeatureToCDSFormPtr) GetObjectExtra (t);
  EnableFeatureToCDSControls (fp);
}

static void ChangeFeatureToCDS (PopuP p)
{
  FeatureToCDSFormPtr    fp;
  Int4                   feature_choice;

  fp = (FeatureToCDSFormPtr) GetObjectExtra (p);
  if (fp != NULL)
  {
    feature_choice = GetValue (fp->feature_choice);
    if (feature_choice == FEATURE_TO_CDS_GENE)
    {
      Show (fp->gene_field);
      Hide (fp->mrna_field);
      Hide (fp->exon_field);
    }
    else if (feature_choice == FEATURE_TO_CDS_MRNA)
    {
      Show (fp->mrna_field);
      Hide (fp->gene_field);
      Hide (fp->exon_field);
    }
    else
    {
      Show (fp->exon_field);
      Hide (fp->mrna_field);
      Hide (fp->gene_field);
    }
  }
  EnableFeatureToCDSControls (fp);
}

static void CleanupFeatureToCDSPage (GraphiC g, VoidPtr data)
{
  FeatureToCDSFormPtr fp;

  fp = (FeatureToCDSFormPtr) data;
  if (fp != NULL) 
  {
    fp->append_string = MemFree (fp->append_string);
    fp->gene_field_list = ValNodeFree (fp->gene_field_list);
    fp->mrna_field_list = ValNodeFree (fp->mrna_field_list);
    fp->exon_field_list = ValNodeFree (fp->exon_field_list);
    MemFree (fp);
  }
}

static void LogFeatureToCDSNoProtein (FeatureToCDSFormPtr fp, CharPtr feature_label, SeqIdPtr sip)
{
  Char       id [41];

  if (fp == NULL || fp->log_fp == NULL || sip == NULL) return;

  id [0] = '\0';
  SeqIdWrite (SeqIdFindBest (sip, 0), id, PRINTID_FASTA_LONG, sizeof (id) - 1);
  
  if (StringHasNoText (feature_label))
  {
    fprintf (fp->log_fp, "No protein added for unlabeled feature on sequence %s\n", id);
  }
  else
  {
    fprintf (fp->log_fp, "No protein added for %s on sequence %s\n", feature_label, id);
  }
  fp->data_in_log = TRUE;
}

static CharPtr GetNewProteinName (SeqFeatPtr sfp, FeatureToCDSFormPtr fp)
{
  SeqMgrFeatContext context;
  CharPtr           prot_name_src = NULL;
  CharPtr           protName = NULL;
  Int4              prot_len;
  CharPtr           cp;
  
  if (sfp == NULL || fp == NULL || sfp->idx.subtype != fp->featdef_choice)
  {
    return NULL;
  }

  if (SeqMgrGetDesiredFeature (sfp->idx.entityID, NULL,
                               0, 0, sfp, &context) == NULL) return NULL;
                               
  /* determine the new protein name */
  if (fp->featdef_choice == FEATDEF_GENE)
  {
    prot_name_src = GetGeneFieldString (sfp, fp->gene_field_list, NULL);
  }
  else if (fp->featdef_choice == FEATDEF_mRNA)
  {
    prot_name_src = GetmRNAFieldString (sfp, fp->mrna_field_list, NULL);
  }
  else if (fp->featdef_choice == FEATDEF_exon)
  {
    prot_name_src = GetExonFieldString (sfp, fp->exon_field_list);
  }
  
  if (StringHasNoText (prot_name_src) && StringHasNoText (fp->append_string))
  {
  	return NULL;
  }

  prot_len = StringLen (prot_name_src) + StringLen (fp->append_string) + 2;
  
  /* add one to length for terminating null */
  protName = (CharPtr) MemNew (prot_len * sizeof (Char));
  if (protName == NULL) return NULL;
  
  if (!StringHasNoText (prot_name_src))
  {
    StringCpy (protName, prot_name_src);
  	/* apply capitalization */
    switch (fp->caps_choice)
  	{
  	  case 1:
  	    /* do nothing, leave capitalization as is */
  	    break;
  	  case 2:
  	    /* capitalize first letter */
  	    protName [0] = toupper (protName[0]);
  	    break;
  	  case 3:
  	  	/* capitalize all letters */
  	  	for (cp = protName; *cp != 0; cp++)
  	  	{
  	  	  *cp = toupper (*cp);
  	  	}
  	  	break;
  	}
  	if (!StringHasNoText (fp->append_string))
  	{
  	  StringCat (protName, " ");
  	}
  }
  prot_name_src = MemFree (prot_name_src);
  if (!StringHasNoText (fp->append_string))
  {
    StringCat (protName, fp->append_string);
  }
  return protName;  
}

static SeqFeatPtr FeatureToCDS (SeqFeatPtr sfp, FeatureToCDSFormPtr fp)

{
  SeqFeatPtr        cds;
  SeqMgrFeatContext context;
  Int2              genCode;
  CdRegionPtr       crp;
  ByteStorePtr      bs;
  BioseqPtr         bsp;
  SeqEntryPtr       psep, nsep, old, topsep;
  MolInfoPtr        mip;
  ProtRefPtr        prp;
  Char              ch;
  ValNodePtr        descr;
  Int2              i;
  Boolean           partial5, partial3;
  CharPtr           prot;
  CharPtr           ptr;
  ValNodePtr        vnp;
  CharPtr           protName = NULL;
  RnaRefPtr         rrp;

  
  if (sfp == NULL || fp == NULL || sfp->idx.subtype != fp->featdef_choice)
  {
    return NULL;
  }

  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);

  cds = SeqMgrGetOverlappingCDS (sfp->location, &context);
  if (cds != NULL) 
  {
    return NULL;	
  }

  if (SeqMgrGetDesiredFeature (sfp->idx.entityID, NULL,
                               0, 0, sfp, &context) == NULL) 
  {
    return NULL;
  }
                               
  protName = GetNewProteinName (sfp, fp);
  if (protName == NULL)
  {
    LogFeatureToCDSNoProtein (fp, context.label, SeqLocId (sfp->location));
    return NULL;
  }

  /* make sure mRNA product name matches CDS protein name */
  if (fp->featdef_choice == FEATDEF_mRNA)
  {
    rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
    if (rrp != NULL)
    {
      rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
      rrp->ext.value.ptrvalue = StringSave (protName);
    }    
  }
  
  /* need to remove string */
  if (fp->featdef_choice == FEATDEF_GENE)
  {
    RemoveGeneFieldString (sfp, fp->gene_field_list);
  }
  else if (fp->featdef_choice == FEATDEF_mRNA)
  {
    RemovemRNAFieldString (sfp, fp->mrna_field_list);
  }
  else if (fp->featdef_choice == FEATDEF_exon)
  {
    RemoveExonFieldString (sfp, fp->exon_field_list);
  }
  
  cds = SeqFeatNew ();
  if (cds == NULL) return NULL;
  cds->data.choice = SEQFEAT_CDREGION;

  genCode = SeqEntryToGeneticCode (fp->sep, NULL, NULL, 0);
  crp = CreateNewCdRgn (1, FALSE, genCode);
  if (crp == NULL) {
    return NULL;
  }
 
  cds->data.value.ptrvalue = (Pointer) crp;
  cds->location = SeqLocFree (cds->location);
  cds->location = AsnIoMemCopy ((Pointer) sfp->location,
                                (AsnReadFunc) SeqLocAsnRead,
                                (AsnWriteFunc) SeqLocAsnWrite);
  cds->partial = sfp->partial;
  cds->next = sfp->next;
  sfp->next = cds;
 
  bs = ProteinFromCdRegionEx (cds, TRUE, FALSE);
  if (bs != NULL) {
    prot = BSMerge (bs, NULL);
    bs = BSFree (bs);
    if (prot != NULL) {
      ptr = prot;
      ch = *ptr;
      while (ch != '\0') {
        *ptr = TO_UPPER (ch);
        ptr++;
        ch = *ptr;
      }
      i = (Int2) StringLen (prot);
      if (i > 0 && prot [i - 1] == '*') {
        prot [i - 1] = '\0';
      }
      bs = BSNew (1000);
	  if (bs != NULL) {
        ptr = prot;
        BSWrite (bs, (VoidPtr) ptr, (Int4) StringLen (ptr));
      }
    }
  }
  bsp = BioseqNew ();
  if (bsp == NULL) return NULL;
  bsp->repr = Seq_repr_raw;
  bsp->mol = Seq_mol_aa;
  bsp->seq_data_type = Seq_code_ncbieaa;
  bsp->seq_data = bs;
  bsp->length = BSLen (bs);
  bs = NULL;
  old = SeqEntrySetScope (NULL);
  bsp->id = MakeNewProteinSeqId (cds->location, NULL);
  SeqMgrAddToBioseqIndex (bsp);
  SeqEntrySetScope (old);
  psep = SeqEntryNew ();
  if (psep != NULL) {
    psep->choice = 1;
    psep->data.ptrvalue = (Pointer) bsp;
    SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, psep);
    mip = MolInfoNew ();
    if (mip != NULL) {
      mip->biomol = 8;
      mip->tech = 8;
      if (partial5 && partial3) {
        mip->completeness = 5;
      } else if (partial5) {
        mip->completeness = 3;
      } else if (partial3) {
        mip->completeness = 4;
      }
      vnp = CreateNewDescriptor (psep, Seq_descr_molinfo);
      if (vnp != NULL) {
        vnp->data.ptrvalue = (Pointer) mip;
      }
    }
    descr = ExtractBioSourceAndPubs (fp->sep);
    if (IS_Bioseq_set (fp->sep))
    {
      topsep = fp->sep;
    }
    else
    {
      topsep = GetBestTopParentForData (fp->input_entityID, fp->sep->data.ptrvalue);
    }
    AddSeqEntryToSeqEntry (topsep, psep, TRUE);
    nsep = FindNucSeqEntry (fp->sep);
    ReplaceBioSourceAndPubs (fp->sep, descr);
    SetSeqFeatProduct (cds, bsp);
    prp = CreateNewProtRef (protName, NULL, NULL, NULL);
    MemFree (protName);
    sfp = CreateNewFeature (psep, NULL, SEQFEAT_PROT, NULL);
    if (sfp != NULL) {
       sfp->data.value.ptrvalue = (Pointer) prp;
    }
  }
  return cds;
}

static void 
LogFeatureToCDSMismatchProtein 
(FeatureToCDSFormPtr fp, 
 SeqIdPtr            sip)
{
  Char       id [41];

  if (fp == NULL || fp->log_fp == NULL || sip == NULL) return;

  id [0] = '\0';
  SeqIdWrite (SeqIdFindBest (sip, 0), id, PRINTID_FASTA_LONG, sizeof (id) - 1);
  
  fprintf (fp->log_fp, "The protein names generated from features on %s do not match\n", id);
  
  fp->data_in_log = TRUE;
}

static void FeatureToCDSBioseqCheckCallback (BioseqPtr bsp, Pointer userdata)

{
  FeatureToCDSFormPtr fp;
  CharPtr             last_prot_name = NULL, new_prot_name;
  SeqMgrFeatContext   feature_context, cds_context;
  SeqFeatPtr          sfp, cds;

  if (bsp == NULL || userdata == NULL) return;
  fp = (FeatureToCDSFormPtr) userdata;
  fp->sep = SeqMgrGetSeqEntryForData (bsp);
  
  /* if we are fusing features, find all the features on this Bioseq for
   * our type.
   */
  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &feature_context);
  while (sfp != NULL)
  {
    if (sfp->idx.subtype == fp->featdef_choice)
    {
      cds = SeqMgrGetOverlappingCDS (sfp->location, &cds_context);
      if (cds == NULL) 
      {
        new_prot_name = GetNewProteinName (sfp, fp);
        if (!fp->fuse_multiple)
        {
          if (new_prot_name == NULL)
          {
            LogFeatureToCDSNoProtein (fp, feature_context.label, SeqLocId (sfp->location));
          }
        }
        else if (last_prot_name == NULL && new_prot_name != NULL)
        {
          if (new_prot_name == NULL)
          {
            LogFeatureToCDSNoProtein (fp, feature_context.label, SeqLocId (sfp->location));
          }
          else
          {
            last_prot_name = new_prot_name;
          }
        }
        else if (new_prot_name == NULL)
        {
          /* do nothing - will use protein name from prior CDS */ 
        }
        else if (StringCmp (last_prot_name, new_prot_name) == 0)
        {
          /* no conflict */
          MemFree (new_prot_name);
        }
        else
        {
          LogFeatureToCDSMismatchProtein (fp, SeqLocId (sfp->location));
          MemFree (last_prot_name);
          last_prot_name = new_prot_name;
        }
      }
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, fp->featdef_choice, &feature_context);
  }
}

static void FeatureToCDSBioseqActCallback (BioseqPtr bsp, Pointer userdata)

{
  FeatureToCDSFormPtr fp;
  SeqFeatPtr          first = NULL;
  CharPtr             new_prot_name;
  SeqMgrFeatContext   feature_context, cds_context;
  SeqFeatPtr          sfp, cds;
  SeqLocPtr           new_cds_location = NULL, slp;
  Boolean             first_partial5, first_partial3;
  Int4                first_start_pos, first_stop_pos;
  Boolean             sfp_partial5, sfp_partial3;
  Int4                sfp_start_pos, sfp_stop_pos;

  if (bsp == NULL || userdata == NULL) return;
  fp = (FeatureToCDSFormPtr) userdata;
  fp->sep = SeqMgrGetSeqEntryForData (bsp);
  
  /* if we are fusing features, find all the features on this Bioseq for
   * our type.
   */
  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &feature_context);
  while (sfp != NULL)
  {
    if (sfp->idx.subtype == fp->featdef_choice)
    {
      cds = SeqMgrGetOverlappingCDS (sfp->location, &cds_context);
      if (cds == NULL) 
      {
        if (fp->fuse_multiple)
        {
          if (first == NULL)
          {
            new_prot_name = GetNewProteinName (sfp, fp);
            if (new_prot_name != NULL)
            {
              first = sfp;
              new_cds_location = (SeqLocPtr) AsnIoMemCopy ((Pointer) sfp->location,
                                                     (AsnReadFunc) SeqLocAsnRead,
                                                     (AsnWriteFunc) SeqLocAsnWrite);
              MemFree (new_prot_name);
            }
          }
          else
          {
            /* preserve partialness of ends */
            CheckSeqLocForPartial (new_cds_location, &first_partial5, &first_partial3);
            first_start_pos = SeqLocStart (new_cds_location);
            first_stop_pos = SeqLocStop (new_cds_location);
            CheckSeqLocForPartial (sfp->location, &sfp_partial5, &sfp_partial3);
            sfp_start_pos = SeqLocStart (sfp->location);
            sfp_stop_pos = SeqLocStop (sfp->location);
            if (first_start_pos > sfp_start_pos)
            {
              first_partial5 = sfp_partial5;
            }
            if (first_stop_pos < sfp_stop_pos)
            {
              first_partial3 = sfp_partial3;
            }
            
            slp = SeqLocMerge (bsp, sfp->location, new_cds_location, FALSE, TRUE, FALSE);
            new_cds_location = SeqLocFree (new_cds_location);
            new_cds_location = slp;
            SetSeqLocPartial (new_cds_location, first_partial5, first_partial3);            
            first_start_pos = SeqLocStart (new_cds_location);
            first_stop_pos = SeqLocStop (new_cds_location);
          }
        }
        else
        {
          FeatureToCDS (sfp, fp);
        }
      }
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, fp->featdef_choice, &feature_context);
  }
  if (fp->fuse_multiple && first != NULL)
  {
    cds = FeatureToCDS (first, fp);
    if (cds != NULL)
    {
      cds->location = SeqLocFree (cds->location);
      cds->location = new_cds_location;
      cds->partial = CheckSeqLocForPartial (cds->location, &first_partial5, &first_partial3);
    }
  }
}

static void FeatureToCDSAccept (ButtoN b)
{
  FeatureToCDSFormPtr    fp;
  Int4                   feature_choice;
  Char                   path [PATH_MAX];
  MsgAnswer              ans = ANS_YES;

  fp = (FeatureToCDSFormPtr) GetObjectExtra (b);
  if (fp == NULL)
  {
    return;
  }
  
  feature_choice = GetValue (fp->feature_choice);
  if (feature_choice == FEATURE_TO_CDS_GENE)
  {
    fp->featdef_choice = FEATDEF_GENE;
    fp->gene_field_list = DialogToPointer (fp->gene_field);
  }
  else if (feature_choice == FEATURE_TO_CDS_MRNA)
  {
    fp->featdef_choice = FEATDEF_mRNA;
    fp->mrna_field_list = DialogToPointer (fp->mrna_field);
  }
  else if (feature_choice == FEATURE_TO_CDS_EXON)
  {
    fp->featdef_choice = FEATDEF_exon;
    fp->exon_field_list = DialogToPointer (fp->exon_field);
  }
  else
  {
    return;
  }

  TmpNam (path);
  fp->log_fp = FileOpen (path, "wb");
  if (fp->log_fp == NULL) return;
  
  Hide (fp->form);
  WatchCursor ();
  Update ();

  fp->sep = GetTopSeqEntryForEntityID (fp->input_entityID);
  if (fp->sep == NULL) return;
  
  fp->caps_choice = GetValue (fp->qual_caps_grp);
  
  fp->append_string = JustSaveStringFromText (fp->append_text);
  
  fp->fuse_multiple = GetStatus (fp->fuse_multiple_btn);

  VisitBioseqsInSep (fp->sep, (Pointer) fp, FeatureToCDSBioseqCheckCallback);
  FileClose (fp->log_fp);
  fp->log_fp = NULL;
  
  if (fp->data_in_log)
  {
    LaunchGeneralTextViewer (path, "Protein Name Problems");    
    ans = Message (MSG_YN, "There are protein name problems - do you want to continue?");
  }
  FileRemove (path);

  if (ans == ANS_YES)
  {
    fp->sep = GetTopSeqEntryForEntityID (fp->input_entityID);
    VisitBioseqsInSep (fp->sep, (Pointer) fp, FeatureToCDSBioseqActCallback);
  }
  
  ArrowCursor ();
  Update ();

  ObjMgrSetDirtyFlag (fp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, fp->input_entityID, 0, 0);  
  Remove (fp->form); 
}

static void FeatureToCds (IteM i)

{
  BaseFormPtr            bfp;
  FeatureToCDSFormPtr    fp;
  WindoW                 w;
  GrouP                  h, g, m, c;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) {
    Message (MSG_ERROR, "FeatureToCDS error");
    return;
  }
  
  fp = (FeatureToCDSFormPtr) MemNew (sizeof (FeatureToCDSFormData));
  if (fp == NULL) return;
  fp->input_entityID = bfp->input_entityID;
  fp->input_itemID = bfp->input_itemID;
  fp->input_itemtype = bfp->input_itemtype;
  
  w = FixedWindow (-50, -33, -10, -10, "Feature to CDS", StdCloseWindowProc);
  SetObjectExtra (w, fp, CleanupFeatureToCDSPage);
  fp->form = (ForM) w;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
  
  fp->feature_choice = PopupList (h, TRUE, ChangeFeatureToCDS);
  PopupItem (fp->feature_choice, "Gene");
  PopupItem (fp->feature_choice, "mRNA");
  PopupItem (fp->feature_choice, "exon");
  SetValue (fp->feature_choice, FEATURE_TO_CDS_GENE);
  SetObjectExtra (fp->feature_choice, fp, NULL);

  g = NormalGroup (h, 0, 0, "Source for new CDS protein name", NULL, NULL);
  fp->gene_field = FeatureFieldChoiceDialog (g, GeneFieldSelectionDialog, TRUE, EnableFeatureToCDSControls, fp);
  fp->mrna_field = FeatureFieldChoiceDialog (g, MRNAFieldSelectionDialog, TRUE, EnableFeatureToCDSControls, fp);
  fp->exon_field = FeatureFieldChoiceDialog (g, ExonFieldSelectionDialog, TRUE, EnableFeatureToCDSControls, fp);
  Hide (fp->mrna_field);
  Hide (fp->exon_field);
  AlignObjects (ALIGN_CENTER, (HANDLE) fp->gene_field,
                              (HANDLE) fp->mrna_field, 
                              (HANDLE) fp->exon_field, 
                              NULL);
  
  fp->qual_caps_grp = NormalGroup (h, 3, 0,
                     "Capitalization for field in protein name", NULL, NULL);
  RadioButton (fp->qual_caps_grp, "As is");
  RadioButton (fp->qual_caps_grp, "Capitalize first initial");
  RadioButton (fp->qual_caps_grp, "Capitalize all");
  SetValue (fp->qual_caps_grp, 1);
  
  m = HiddenGroup (h, 0, 2, NULL);
  StaticPrompt (m, "Append text to protein name", 0, 0, programFont, 'c');
  fp->append_text = DialogText (m, "", 14, EnableFeatureToCDSControlsText); 
  SetObjectExtra (fp->append_text, fp, NULL);
  
  fp->fuse_multiple_btn = CheckBox (h, "Fuse multiple intervals for new CDS", NULL);
   
  c = HiddenGroup (h, 4, 0, NULL); 
  fp->accept_btn = PushButton (c, "Accept", FeatureToCDSAccept);
  SetObjectExtra (fp->accept_btn, fp, NULL);
  EnableFeatureToCDSControls (fp);
  
  PushButton (c, "Cancel", StdCancelButtonProc);


  AlignObjects (ALIGN_CENTER, (HANDLE) fp->feature_choice,
                              (HANDLE) g,
                              (HANDLE) fp->qual_caps_grp,
                              (HANDLE) m, 
                              (HANDLE) fp->fuse_multiple_btn, 
                              (HANDLE) c, NULL);
                
  RealizeWindow (w);
  Show (w);
  Update ();
  
}

static void AddFeatureToBioseq (SeqFeatPtr sfp, BioseqPtr bsp)

{
  SeqFeatPtr   prev;
  SeqAnnotPtr  sap;

  if (sfp == NULL || bsp == NULL) return;
  sap = bsp->annot;
  while (sap != NULL && (sap->name != NULL || sap->desc != NULL || sap->type != 1)) {
    sap = sap->next;
  }
  if (sap == NULL) {
    sap = SeqAnnotNew ();
    if (sap != NULL) {
      sap->type = 1;
      sap->next = bsp->annot;
      bsp->annot = sap;
    }
  }
  sap = bsp->annot;
  if (sap != NULL) {
    if (sap->data != NULL) {
      prev = sap->data;
      while (prev->next != NULL) {
        prev = prev->next;
      }
      prev->next = sfp;
    } else {
      sap->data = (Pointer) sfp;
    }
  }
}

static void PackageFeatureCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BaseFormPtr    bfp;
  BioseqPtr      bsp = NULL;
  BioseqSetPtr   bssp = NULL;
  SeqAnnotPtr    nextsap;
  SeqFeatPtr     nextsfp;
  Pointer PNTR   prevsap;
  Pointer PNTR   prevsfp;
  SeqAnnotPtr    sap;
  SeqFeatPtr     sfp;
  BioseqPtr      target;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  bfp = (BaseFormPtr) mydata;
  if (bfp == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
    prevsap = (Pointer PNTR) &(bsp->annot);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
    prevsap = (Pointer PNTR) &(bssp->annot);
  } else return;
  while (sap != NULL) {
    nextsap = sap->next;
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      prevsfp = (Pointer PNTR) &(sap->data);
      while (sfp != NULL) {
        nextsfp = sfp->next;
        target = GetBioseqGivenSeqLoc (sfp->location, bfp->input_entityID);
        if (target != bsp) {
          *(prevsfp) = sfp->next;
          sfp->next = NULL;
          AddFeatureToBioseq (sfp, target);
        } else {
          prevsfp = (Pointer PNTR) &(sfp->next);
        }
        sfp = nextsfp;
      }
    }
    if (sap->data == NULL) {
      *(prevsap) = sap->next;
      sap->next = NULL;
      SeqAnnotFree (sap);
    } else {
      prevsap = (Pointer PNTR) &(sap->next);
    }
    sap = nextsap;
  }
}

static void PackageOnParts (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  WatchCursor ();
  Update ();
  SeqEntryExplore (sep, bfp, PackageFeatureCallback);
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static BioseqSetPtr GetParentNPS (BioseqPtr bsp)
{
  BioseqSetPtr  bssp;

  if (bsp == NULL) return NULL;
  if (bsp->idx.parenttype != OBJ_BIOSEQSET) return NULL;
  bssp = (BioseqSetPtr) bsp->idx.parentptr;
  while (bssp != NULL && bssp->_class != BioseqseqSet_class_nuc_prot && bssp->idx.parenttype == OBJ_BIOSEQSET) {
    bssp = (BioseqSetPtr) bssp->idx.parentptr;
  }
  if (bssp->_class == BioseqseqSet_class_nuc_prot) return bssp;
  return NULL;
}

static Boolean NucAndProtNotInSameNPS (BioseqPtr nuc, BioseqPtr prt)
{
  BioseqSetPtr    bssp;

  if (nuc == NULL || prt == NULL) return FALSE;
  bssp = GetParentNPS (nuc);
  if (bssp == NULL) return TRUE;
  if (GetParentNPS (prt) != bssp) return TRUE;
  return FALSE;
}

typedef struct nppack {
  BioseqPtr  nuc;
  BioseqPtr  prt;
} NpPack, PNTR NpPackPtr;

static void PackageProteinsCallback (SeqFeatPtr sfp, Pointer userdata)

{
  BioseqSetPtr       bssp;
  SeqMgrDescContext  context;
  ValNodePtr PNTR    headp;
  MolInfoPtr         mip;
  NpPackPtr          npp;
  BioseqPtr          nuc, prt;
  SeqDescrPtr        sdp;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION || sfp->product == NULL) return;
  nuc = BioseqFindFromSeqLoc (sfp->location);
  prt = BioseqFindFromSeqLoc (sfp->product);
  if (nuc == NULL || prt == NULL) return;

  headp = (ValNodePtr PNTR) userdata;

  /* if CDS location is on genomic nucleotide in genomic product set, bail */

  if (nuc->idx.parenttype == OBJ_BIOSEQSET) {
    bssp = (BioseqSetPtr) nuc->idx.parentptr;
    if (bssp != NULL && bssp->_class == BioseqseqSet_class_gen_prod_set) {
      sdp = SeqMgrGetNextDescriptor (nuc, NULL, Seq_descr_molinfo, &context);
      if (sdp != NULL) {
        mip = (MolInfoPtr) sdp->data.ptrvalue;
        if (mip != NULL) {
          if (mip->biomol != MOLECULE_TYPE_MRNA) return;
        }
      }
    }
  }

  if (NucAndProtNotInSameNPS (nuc, prt)) {
    npp = (NpPackPtr) MemNew (sizeof (NpPack));
    if (npp == NULL) return;
    npp->nuc = nuc;
    npp->prt = (BioseqPtr) AsnIoMemCopy ((Pointer) prt,
                                         (AsnReadFunc) BioseqAsnRead,
                                         (AsnWriteFunc) BioseqAsnWrite);
    ValNodeAddPointer (headp, 0, (Pointer) npp);
    prt->idx.deleteme = TRUE;
  }
}

static void PackageOnNucs (IteM i)

{
  BaseFormPtr    bfp;
  ValNodePtr     descr;
  ValNodePtr     head = NULL, vnp;
  NpPackPtr      npp;
  BioseqSetPtr   nps;
  SeqEntryPtr    nsep, psep;
  BioseqPtr      nuc, prt;
  SeqEntryPtr    sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  WatchCursor ();
  Update ();

  VisitFeaturesInSep (sep, &head, PackageProteinsCallback);

  DeleteMarkedObjects (bfp->input_entityID, 0, NULL);
  SeqMgrClearFeatureIndexes (bfp->input_entityID, NULL);

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    npp = (NpPackPtr) vnp->data.ptrvalue;
    if (npp == NULL) continue;
    nuc = npp->nuc;
    prt = npp->prt;
    if (nuc == NULL || prt == NULL) continue;
    nsep = nuc->seqentry;
    nps = GetParentNPS (nuc);
    if (nps != NULL) {
      nsep = nps->seqentry;
    }
    psep = SeqEntryNew ();
    if (nsep == NULL || psep == NULL) continue;
    psep->choice = 1;
    psep->data.ptrvalue = (Pointer) prt;
    SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) prt, psep);
    descr = ExtractBioSourceAndPubs (nsep);
    AddSeqEntryToSeqEntry (nsep, psep, TRUE);
    ReplaceBioSourceAndPubs (nsep, descr);
    AssignIDsInEntity (bfp->input_entityID, 0, NULL);
  }
  ValNodeFreeData (head);

  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

extern EnumFieldAssoc  enum_bond_alist [];
extern EnumFieldAssoc  enum_site_alist [];

static Boolean            alistBoxUp;
static Boolean            alistBoxAccept;
static PopuP              alistBoxPopup;

static void AcceptAlistMessage (ButtoN b)

{
  alistBoxAccept = TRUE;
  alistBoxUp = FALSE;
}

static void CancelAlistMessage (ButtoN b)

{
  alistBoxAccept = FALSE;
  alistBoxUp = FALSE;
}

extern Boolean AlistMessage (EnumFieldAssocPtr al, UIEnumPtr val, UIEnum dflt, CharPtr mssg)

{
  GrouP   c;
  GrouP   g;
  PrompT  ppt;
  WindoW  w;

  if (al == NULL || val == NULL) return FALSE;
  alistBoxUp = TRUE;
  alistBoxAccept = FALSE;
  w = ModalWindow (-50, -20, -20, -20, NULL);
  if (w != NULL) {
    g = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (g, 10, 10);
    ppt = StaticPrompt (g, mssg, 0, 0, programFont, 'c');
    alistBoxPopup = PopupList (g, TRUE, NULL);
    InitEnumPopup (alistBoxPopup, al, NULL);
    SetEnumPopup (alistBoxPopup, al, dflt);
    c = HiddenGroup (g, 2, 0, NULL);
    DefaultButton (c, "Accept", AcceptAlistMessage);
    PushButton (c, "Cancel", CancelAlistMessage);
    AlignObjects (ALIGN_CENTER, (HANDLE) ppt, (HANDLE) alistBoxPopup, (HANDLE) c, NULL);
    RealizeWindow (w);
    Show (w);
    Select (w);
    while (alistBoxUp) {
      ProcessExternalEvent ();
      Update ();
    }
    ProcessAnEvent ();
    if (! GetEnumPopup (alistBoxPopup, al, val)) {
      alistBoxAccept = FALSE;
    }
    Remove (w);
  }
  return alistBoxAccept;
} 

static void DefaultMessageProc (ForM f, Int2 mssg)

{
  StdEditorProcsPtr  sepp;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    if (sepp->handleMessages != NULL) {
      sepp->handleMessages (f, mssg);
    }
  }
}

typedef struct multtransformdata {
  FEATURE_FORM_BLOCK

  Uint2          type;
  LisT           fromfeatlist;
  PopuP          siteorbondlist;
  Uint1          feattype;
  Int4           siteorbondtype;
} MultTransFormData, PNTR MultTransFormPtr;

static void ProcessMultiTrans (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr         bsp;
  BioseqSetPtr      bssp;
  SeqFeatPtr        cds;
  MultTransFormPtr  mfp;
  SeqFeatPtr        newsfp;
  BioseqPtr         pbsp;
  SeqEntryPtr       psep;
  SeqAnnotPtr       sap;
  SeqBondPtr        sbp;
  SeqFeatPtr        sfp;
  SeqIdPtr          sip;
  SeqLocPtr         slp;
  SeqPntPtr         spp;
  Uint1             subtype;

  mfp = (MultTransFormPtr) mydata;
  if (sep == NULL || mfp == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        subtype = FindFeatDefType (sfp);
        if (subtype == mfp->feattype) {
          cds = SeqMgrGetOverlappingCDS (sfp->location, NULL);
          if (cds != NULL) {
            slp = dnaLoc_to_aaLoc (cds, sfp->location, TRUE, NULL, FALSE);
            if (slp != NULL) {
              pbsp = BioseqFindFromSeqLoc (slp);
              if (pbsp != NULL) {
                psep = SeqMgrGetSeqEntryForData (pbsp);
                if (psep != NULL) {
                  newsfp = SeqFeatNew ();
                  if (newsfp != NULL) {
                    newsfp->data.choice = mfp->type;
                    if (mfp->type == SEQFEAT_BOND || mfp->type == SEQFEAT_SITE) {
                      newsfp->data.value.intvalue = (Int4) mfp->siteorbondtype;
                    } else if (mfp->type == SEQFEAT_REGION) {
                      newsfp->data.value.ptrvalue = sfp->comment;
                      sfp->comment = NULL;
                    }
                    CreateNewFeature (psep, NULL, mfp->type, newsfp);
                    newsfp->location = slp;
                    newsfp->comment = sfp->comment;
                    sfp->comment = NULL;
                    SeqFeatDataFree (&sfp->data);            /* free duplicate feature data */
                    sfp->data.choice = SEQFEAT_GENE;         /* fake as empty gene, to be removed */
                    sfp->data.value.ptrvalue = GeneRefNew ();
                    if (mfp->type == SEQFEAT_BOND) {
                      if (slp->choice != SEQLOC_BOND) {
                        sip = SeqLocId (slp);
                        if (sip != NULL) {
                          sbp = SeqBondNew ();
                          if (sbp != NULL) {
                            slp = ValNodeNew (NULL);
                            if (slp != NULL) {
                              slp->choice = SEQLOC_BOND;
                              slp->data.ptrvalue = (Pointer) sbp;
                              spp = SeqPntNew ();
                              if (spp != NULL) {
                                spp->strand = SeqLocStrand (newsfp->location);
                                spp->id = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (sip, 0)));
                                spp->point = SeqLocStart (newsfp->location);
                                sbp->a = spp;
                              }
                              spp = SeqPntNew ();
                              if (spp != NULL) {
                                spp->strand = SeqLocStrand (newsfp->location);
                                spp->id = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (sip, 0)));
                                spp->point = SeqLocStop (newsfp->location);
                                sbp->b = spp;
                              }
                              newsfp->location = SeqLocFree (newsfp->location);
                              newsfp->location = slp;
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
}

static void DoMultiTransform (ButtoN b)

{
  UIEnum            enumval;
  Int2              feattype;
  MultTransFormPtr  mfp;
  SeqEntryPtr       sep;
  Int2              siteorbondtype = 0;
  UIEnumPtr         uptr;

  mfp = (MultTransFormPtr) GetObjectExtra (b);
  if (mfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (mfp->input_entityID);
  if (sep == NULL) return;
  Hide (mfp->form);
  WatchCursor ();
  Update ();
  uptr = (UIEnumPtr) DialogToPointer ((DialoG) mfp->fromfeatlist);
  if (uptr != NULL) {
    enumval = *uptr;
    feattype = (Int2) enumval;
    uptr = (UIEnumPtr) DialogToPointer ((DialoG) mfp->siteorbondlist);
    if (uptr != NULL) {
      enumval = *uptr;
      siteorbondtype = (Int2) enumval;
    }
    if (feattype > 0) {
      mfp->feattype = (Uint1) feattype;
      mfp->siteorbondtype = (Int4) siteorbondtype;
      if (siteorbondtype > 0 || mfp->type == SEQFEAT_REGION) {
        SeqEntryExplore (sep, (Pointer) mfp, ProcessMultiTrans);
      }
    }
  }
  ArrowCursor ();
  Update ();
  SeqEntryExplore (sep, NULL, CleanupEmptyFeatCallback);
  SeqEntryExplore (sep, NULL, MergeAdjacentAnnotsCallback);
  ObjMgrSetDirtyFlag (mfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, mfp->input_entityID, 0, 0);
  Remove (mfp->form);
}

static void MultiTransformProc (IteM i, Uint2 targettype)

{
  EnumFieldAssocPtr  ap;
  ButtoN             b;
  BaseFormPtr        bfp;
  GrouP              c;
  GrouP              g;
  GrouP              h;
  GrouP              k;
  Int2               listHeight;
  MultTransFormPtr   mfp;
  EnumFieldAssocPtr  realalist;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  CharPtr            title = "?";
  WindoW             w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  mfp = (MultTransFormPtr) MemNew (sizeof (MultTransFormData));
  if (mfp == NULL) return;

  if (targettype == SEQFEAT_BOND) {
    title = "Transform to Bond";
  } else if (targettype == SEQFEAT_SITE) {
    title = "Transform to Site";
  } else if (targettype == SEQFEAT_REGION) {
    title = "Transform to Region";
  }

  w = FixedWindow (-50, -33, -10, -10, title, StdCloseWindowProc);
  SetObjectExtra (w, mfp, StdCleanupFormProc);
  mfp->form = (ForM) w;
  mfp->formmessage = DefaultMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    mfp->appmessage = sepp->handleMessages;
  }

  mfp->type = targettype;

  mfp->input_entityID = bfp->input_entityID;
  mfp->input_itemID = bfp->input_itemID;
  mfp->input_itemtype = bfp->input_itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 0, 2, NULL);

  ap = import_featdef_alist (TRUE, FALSE, TRUE);
  realalist = ap;
  ap++;
  StaticPrompt (g, "From Feature", 0, 0, programFont, 'c');
  if (indexerVersion) {
    listHeight = 16;
  } else {
    listHeight = 8;
  }
  mfp->fromfeatlist = CreateEnumListDialog (g, 10, listHeight, NULL, ap, 0, NULL);
  FreeEnumFieldAlist (realalist);

  k = NULL;
  if (targettype != SEQFEAT_REGION) {
    k = HiddenGroup (h, 0, 2, NULL);

    if (targettype == SEQFEAT_BOND) {
      StaticPrompt (k, "Select type of bond", 0, 0, programFont, 'c');
      mfp->siteorbondlist = CreateEnumPopupDialog (k, TRUE, NULL, enum_bond_alist, 0, NULL);
    } else if (targettype == SEQFEAT_SITE) {
      StaticPrompt (k, "Select type of site", 0, 0, programFont, 'c');
      mfp->siteorbondlist = CreateEnumPopupDialog (k, TRUE, NULL, enum_site_alist, 0, NULL);
    }
  }

  c = HiddenGroup (h, 4, 0, NULL);
  b = DefaultButton (c, "Accept", DoMultiTransform);
  SetObjectExtra (b, mfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, (HANDLE) k, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}

static Boolean GetTheSfp (GatherContextPtr gcp)

{
  SeqFeatPtr PNTR  sfpp;

  sfpp = (SeqFeatPtr PNTR) gcp->userdata;
  if (sfpp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  *(sfpp) = (SeqFeatPtr) gcp->thisitem;
  return TRUE;
}

static void CommonTransformProc (IteM i, Uint2 targettype)

{
  EnumFieldAssocPtr  al;
  BioseqPtr          bsp;
  SeqFeatPtr         cds;
  CharPtr            comment;
  Uint2              entityID;
  CharPtr            mssg;
  SeqFeatPtr         newsfp;
  OMProcControl      ompc;
  SeqBondPtr         sbp;
  SeqEntryPtr        scope;
  SelStructPtr       sel;
  SeqEntryPtr        sep;
  SeqFeatPtr         sfp;
  SeqIdPtr           sip;
  SeqLocPtr          slp;
  SeqPntPtr          spp;
  UIEnum             val;

  sel = ObjMgrGetSelected ();
  if (sel == NULL) {
    MultiTransformProc (i, targettype);
    return;
  }
  if (sel == NULL || sel->itemtype != OBJ_SEQFEAT) {
    Message (MSG_ERROR, "Feature must be selected");
    return;
  }
  sfp = NULL;
  GatherItem (sel->entityID, sel->itemID, sel->itemtype, (Pointer) (&sfp), GetTheSfp);
  if (sfp == NULL) {
    Message (MSG_ERROR, "Unable to find this feature");
    return;
  }
  scope = GetBestTopParentForItemID (sel->entityID, sel->itemID, sel->itemtype);
  cds = FindBestCds (sel->entityID, sfp->location, NULL, scope);
  if (cds == NULL) {
    Message (MSG_ERROR, "Unable to find CDS covering this feature");
    return;
  }
  bsp = GetBioseqGivenSeqLoc (cds->location, sel->entityID);
  if (bsp == NULL || ISA_aa (bsp->mol)) {
    Message (MSG_ERROR, "Unable to find DNA bioseq for this feature");
    return;
  }
  slp = dnaLoc_to_aaLoc (cds, sfp->location, TRUE, NULL, FALSE);
  if (slp == NULL) {
    Message (MSG_ERROR, "Unable to convert to protein coordinate");
    return;
  }
  bsp = GetBioseqGivenSeqLoc (slp, sel->entityID);
  if (bsp == NULL) {
    Message (MSG_ERROR, "Unable to find target protein bioseq");
    return;
  }
  sep = SeqMgrGetSeqEntryForData (bsp);
  if (sep == NULL) {
    Message (MSG_ERROR, "Unable to find target protein seq-entry");
    return;
  }
  al = NULL;
  mssg = NULL;
  if (targettype == SEQFEAT_BOND) {
    al = enum_bond_alist;
    mssg = "Select type of bond";
  } else if (targettype == SEQFEAT_SITE) {
    al = enum_site_alist;
    mssg = "Select type of site";
  }
  if (targettype != SEQFEAT_REGION) {
    if (al == NULL) return;
    if (! AlistMessage (al, &val, 1, mssg)) return;
  }
  newsfp = NULL;
  if (targettype == SEQFEAT_BOND || targettype == SEQFEAT_SITE) {
    newsfp = SeqFeatNew ();
    if (newsfp != NULL) {
      newsfp->data.choice = targettype;
      newsfp->data.value.intvalue = (Int4) val;
    }
  } else if (targettype == SEQFEAT_REGION) {
    newsfp = SeqFeatNew ();
    if (newsfp != NULL) {
      newsfp->data.choice = targettype;
      newsfp->data.value.ptrvalue = sfp->comment;
      sfp->comment = NULL;
    }
  }
  if (newsfp != NULL) {
    entityID = sel->entityID;
    comment = sfp->comment;
    sfp->comment = NULL;
    MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
    ompc.do_not_reload_from_cache = TRUE;
    ompc.input_entityID = sel->entityID;
    ompc.input_itemID = sel->itemID;
    ompc.input_itemtype = sel->itemtype;
    if (! DetachDataForProc (&ompc, FALSE)) {
      Message (MSG_ERROR, "DetachDataForProc failed");
    }
    CreateNewFeature (sep, NULL, targettype, newsfp);
    newsfp->location = slp;
    newsfp->comment = comment;
    if (targettype == SEQFEAT_BOND) {
      if (slp->choice != SEQLOC_BOND) {
        sip = SeqLocId (slp);
        if (sip != NULL) {
          sbp = SeqBondNew ();
          if (sbp != NULL) {
            slp = ValNodeNew (NULL);
            if (slp != NULL) {
              slp->choice = SEQLOC_BOND;
              slp->data.ptrvalue = (Pointer) sbp;
              spp = SeqPntNew ();
              if (spp != NULL) {
                spp->strand = SeqLocStrand (newsfp->location);
                spp->id = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (sip, 0)));
                spp->point = SeqLocStart (newsfp->location);
                sbp->a = spp;
              }
              spp = SeqPntNew ();
              if (spp != NULL) {
                spp->strand = SeqLocStrand (newsfp->location);
                spp->id = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (sip, 0)));
                spp->point = SeqLocStop (newsfp->location);
                sbp->b = spp;
              }
              newsfp->location = SeqLocFree (newsfp->location);
              newsfp->location = slp;
            }
          }
        }
      }
    }
    ObjMgrSetDirtyFlag (entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
  }
}

static void TransformToBond (IteM i)

{
  CommonTransformProc (i, SEQFEAT_BOND);
}

static void TransformToSite (IteM i)

{
  CommonTransformProc (i, SEQFEAT_SITE);
}

static void TransformToRegion (IteM i)

{
  CommonTransformProc (i, SEQFEAT_REGION);
}

static Boolean UnlinkPubDesc (GatherContextPtr gcp)

{
  ValNodePtr       sdp;
  ValNodePtr PNTR  vnpp;

  if (gcp->thistype != OBJ_SEQDESC) return TRUE;
  sdp = (ValNodePtr) gcp->thisitem;
  if (sdp == NULL || sdp->choice != Seq_descr_pub) return TRUE;
  vnpp = (ValNodePtr PNTR) gcp->userdata;
  if (vnpp == NULL) return TRUE;
  *(gcp->prevlink) = sdp->next;
  sdp->next = NULL;
  *vnpp = sdp;
  return TRUE;
}

static void AddConvertedDescFeat (SeqEntryPtr sep, ValNodePtr vnp)
{
  BioseqSetPtr  bssp;
  BioseqPtr     bsp;
  SeqFeatPtr    sfp;

  if (sep == NULL || vnp == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
	for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
      AddConvertedDescFeat (sep, vnp);
	}
  } else {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (ISA_aa (bsp->mol)) return;
    sfp = CreateNewFeature (sep, NULL, SEQFEAT_PUB, NULL);
    if (sfp != NULL) {
      sfp->data.value.ptrvalue = AsnIoMemCopy ((Pointer) vnp->data.ptrvalue,
                                               (AsnReadFunc) PubdescAsnRead,
                                               (AsnWriteFunc) PubdescAsnWrite);
    }
  }
}

static void CommonConvertDescToFeat (BaseFormPtr bfp, Boolean pub, Boolean src, Boolean com, CharPtr findstring)

{
  SeqEntryPtr   sep;
  SelStructPtr  sel;
  ValNodePtr    vnp;
  Boolean       asked_about_prop = FALSE;
  Boolean       propagate_descriptors = FALSE;

  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  sel = ObjMgrGetSelected ();
  if (pub && sel != NULL) {
    if (sel->entityID == bfp->input_entityID &&
        sel->next == NULL && sel->itemtype == OBJ_SEQDESC) {
      vnp = NULL;
      sep = GetBestTopParentForItemID (sel->entityID, sel->itemID, sel->itemtype);
      if (sep != NULL) {
        /* unlink changes itemID, so sep must be determined first */
        GatherItem (sel->entityID, sel->itemID, sel->itemtype, (Pointer) &vnp, UnlinkPubDesc);
        if (vnp != NULL) {
          AddConvertedDescFeat (sep, vnp);
          ValNodeFree (vnp);
        }
      }
    }
    ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
    return;
  }
  if (ConvertPubSrcComDescsToFeats (sep, pub, src, com, FALSE, &asked_about_prop, &propagate_descriptors, findstring)) {
    ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  }
}

static void CommonConvertDescToFeatMenuItem (IteM i, Boolean pub, Boolean src, Boolean com)
{
  BaseFormPtr   bfp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  CommonConvertDescToFeat (bfp, pub, src, com, NULL);
}

static void ConvertPubDescToFeat (IteM i)

{
  CommonConvertDescToFeatMenuItem (i, TRUE, FALSE, FALSE);
}

static void ConvertSrcDescToFeat (IteM i)

{
  CommonConvertDescToFeatMenuItem (i, FALSE, TRUE, FALSE);
}

static void ConvertComDescToFeat (IteM i)

{
  CommonConvertDescToFeatMenuItem (i, FALSE, FALSE, TRUE);
}

typedef struct convertpubdescdata {
  DESCRIPTOR_FORM_BLOCK
  BaseFormPtr bfp;
  TexT    findthis;
  Char    findString [255];
} ConvertPubDescData, PNTR ConvertPubDescPtr;


static void DoConvertPubDescStringConstraint (ButtoN b)
{
  ConvertPubDescPtr cpdp;

  cpdp = (ConvertPubDescPtr) GetObjectExtra (b);
  if (cpdp == NULL) return;
  Hide (cpdp->form);

  GetTitle (cpdp->findthis, cpdp->findString, sizeof (cpdp->findString) - 1);
  if (StringHasNoText (cpdp->findString)) {
    CommonConvertDescToFeat (cpdp->bfp, TRUE, FALSE, FALSE, NULL);
  } else {
	CommonConvertDescToFeat (cpdp->bfp, TRUE, FALSE, FALSE, cpdp->findString);
  }

  ObjMgrSetDirtyFlag (cpdp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, cpdp->input_entityID, 0, 0);
  Remove (cpdp->form);
}

static void CreateConvertPubDescStringConstraintDialog (IteM i)
{
  BaseFormPtr       bfp;
  ConvertPubDescPtr cpdp;
  WindoW            w;
  GrouP             h, g, c;
  ButtoN            b;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  cpdp = (ConvertPubDescPtr) MemNew (sizeof (ConvertPubDescData));
  if (cpdp == NULL) return;
  cpdp->bfp = bfp;

  w = FixedWindow (-50, -33, -10, -10, "Convert Pub Descriptors", StdCloseWindowProc);
  SetObjectExtra (w, cpdp, StdCleanupFormProc);
  cpdp->form = (ForM) w;
  cpdp->formmessage = NULL;

  cpdp->input_entityID = bfp->input_entityID;

  h = HiddenGroup (w, 0, 2, NULL);
  g = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (g, "Optional string constraint", 0, dialogTextHeight, programFont, 'c');
  cpdp->findthis = DialogText (g, "", 14, NULL);

  c = HiddenGroup (h, 2, 0, NULL);
  b = DefaultButton (c, "Accept", DoConvertPubDescStringConstraint);
  SetObjectExtra (b, cpdp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}


static void MoveSecondProtName (SeqFeatPtr sfp, Pointer userdata)

{
  ValNodePtr  nxt;
  ProtRefPtr  prp;
  ValNodePtr  vnp;
 
  if (sfp == NULL || sfp->data.choice != SEQFEAT_PROT) return;
  prp = (ProtRefPtr) sfp->data.value.ptrvalue;
  if (prp == NULL || prp->desc != NULL) return;
  vnp = prp->name;
  if (vnp == NULL) return;
  nxt = vnp->next;
  if (nxt == NULL || nxt->next != NULL) return;
  vnp->next = NULL;
  prp->desc = nxt->data.ptrvalue;
  nxt->data.ptrvalue = NULL;
  ValNodeFree (nxt);
}

static void SecondProtNameToDesc (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  VisitFeaturesInSep (sep, NULL, MoveSecondProtName);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void MoveProtDesc (SeqFeatPtr sfp, Pointer userdata)

{
  ProtRefPtr  prp;
  CharPtr     str;
  ValNodePtr  vnp;
 
  if (sfp == NULL || sfp->data.choice != SEQFEAT_PROT) return;
  prp = (ProtRefPtr) sfp->data.value.ptrvalue;
  if (prp == NULL || prp->desc == NULL) return;
  vnp = prp->name;
  if (vnp == NULL) return;
  str = prp->desc;
  prp->desc = NULL;
  ValNodeAddStr (&(prp->name), 0, str);
}

static void ProtDescToSecondName (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  VisitFeaturesInSep (sep, NULL, MoveProtDesc);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* HasInternalStops () -- Checks to see if a given protein has         */
/*                        any internal stop codons.                    */
/*                                                                     */
/*   Returns : TRUE -- If internal stop codons are found.              */
/*             FALSE -- If internal stop codons are NOT found.         */
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean HasInternalStops (SeqFeatPtr sfp)
{
  ByteStorePtr  bs;
  CharPtr       protStr;
  CharPtr       stopStr;

  /* Get the protein sequence in ASCII chars */

  bs = ProteinFromCdRegionEx (sfp, TRUE, FALSE);
  if (NULL == bs)
    return TRUE;
  protStr = BSMerge (bs, NULL);
  bs = BSFree (bs);

  /* If the is a "*" in the str in any position  */
  /* except at the end, then we have an internal */
  /* stop codon.                                 */
  
  stopStr = StringStr (protStr, "*");
  if ((NULL != stopStr) && (StringLen (stopStr) > 1)) {
    MemFree (protStr);
    return TRUE;
  }
  else {
    MemFree (protStr);
    return FALSE;
  }
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* ConvertCDS_FeatureCallback () -- Called once for each CDS feature   */
/*                                  on a Bioseq.  This function does   */
/*                                  the actual CDS deleting.           */
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean LIBCALLBACK ConvertCDS_FeatureCallback (SeqFeatPtr sfp,
					SeqMgrFeatContextPtr fcontext)
{
  CdRegionPtr          cdrp;
  GBQualPtr            gbqual;
  ImpFeatPtr           ifp;
  CharPtr              noteStr;
  BioseqPtr            protBsp;
  SeqMgrFeatContext    protContext;
  CharPtr              protName;
  SeqFeatPtr           protSfp;
  ProtRefPtr           prp;
  ValNodePtr           vnp;

  /* skip CDSs without products */
  if (sfp == NULL || sfp->product == NULL) return TRUE;

  /* If no internal stop codons, then done */

  if (FALSE == HasInternalStops(sfp))
    return TRUE;

  /* Get the CD region part of the feature, and */
  /* the associated protein bioseq.             */

  cdrp = (CdRegionPtr) sfp->data.value.ptrvalue;
  protBsp = BioseqFindFromSeqLoc (sfp->product);

  if (protBsp == NULL) return TRUE;

  /* Convert the CDS feature to a misc_feat */

  CdRegionFree (cdrp);
  sfp->data.value.ptrvalue = NULL;

  ifp = ImpFeatNew ();
  if (NULL == ifp)
    return FALSE;
  ifp->key = StringSave ("misc_feature");

  sfp->data.choice = SEQFEAT_IMP;
  sfp->data.value.ptrvalue = (Pointer) ifp;
  sfp->product = SeqLocFree (sfp->product);

  /* Add a name key to the misc_feature */

  protSfp = SeqMgrGetBestProteinFeature (protBsp, &protContext);
  prp = (ProtRefPtr) protSfp->data.value.ptrvalue;

  if (prp != NULL) {
    vnp = prp->name;
    if (NULL != vnp)
      {
	protName = (CharPtr) vnp->data.ptrvalue;
	if (NULL != protName) {
	  noteStr = (CharPtr) MemNew (StringLen (protName) + 12);
	  sprintf (noteStr, "similar to %s", protName);
	}
      }
  }

  gbqual = GBQualNew ();
  if (gbqual == NULL)
    return FALSE;

  gbqual->qual = StringSave ("note");
  gbqual->val = noteStr;
  gbqual->next = sfp->qual;
  sfp->qual = gbqual;

  /* Delete the protein Bioseq that */
  /* the CDS points to.             */

  protBsp->idx.deleteme = TRUE;

  /* Return TRUE to continue on to the next CDS feature */

  return TRUE;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* ConvertCDS_BioseqCallback () -- Called once for each Bioseq by the  */
/*                                 ConvertCDSWithStopCodons() function.*/
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean LIBCALLBACK ConvertCDS_BioseqCallback (BioseqPtr bsp,
					 SeqMgrBioseqContextPtr bcontext)
{
  Boolean featureFilterArray [SEQFEAT_MAX];

  /* Set up to explore only CDS features */

  MemSet ((Pointer) (featureFilterArray), (int) FALSE, SEQFEAT_MAX);
  featureFilterArray[SEQFEAT_CDREGION] = TRUE;

  /* Explore the Bioseq's CDS features, deleting */
  /* the ones with internal stop codons.         */

  SeqMgrExploreFeatures (bsp, NULL, ConvertCDS_FeatureCallback, NULL,
			 featureFilterArray, NULL);

  /* Return TRUE to continue on to the next Bioseq */

  return TRUE;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* ConvertCDSWithStopCodons () -- Converts all CDS features that have  */
/*                                internal stop codons into misc       */
/*                                features.                            */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void ConvertCDSWithStopCodons (IteM i)
{
  BaseFormPtr  bfp;
  Uint2        parenttype;
  Pointer      parentptr;
  SeqEntryPtr  sep;

  /* Get the current data */

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  if (bfp == NULL)
    return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  GetSeqEntryParent (sep, &parentptr, &parenttype);

  /* Explore all the Bioseqs looking for CDS features */

  SeqMgrExploreBioseqs (bfp->input_entityID, NULL, NULL,
			ConvertCDS_BioseqCallback, TRUE, FALSE, TRUE);

  /* Force an update and redraw */

  DeleteMarkedObjects (bfp->input_entityID, 0, NULL);
  SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();

  /* Return successfully */

  return;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* ConvertCDSAndKeep_FeatureCallback () -- Called once for each CDS    */
/*                                         feature. If the CDS has an  */
/*                                         internal stop codon, marks  */
/*                                         it as pseudo, along with    */
/*                                         any overlapping gene.       */
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean LIBCALLBACK ConvertCDSAndKeep_FeatureCallback (SeqFeatPtr sfp,
					SeqMgrFeatContextPtr fcontext)
{
  CdRegionPtr          cdrp;
  SeqMgrFeatContext    geneContext;
  SeqFeatPtr           geneSfp;
  GeneRefPtr           grp;
  BioseqPtr            protBsp;
  SeqMgrFeatContext    protContext;
  CharPtr              protName = NULL;
  SeqFeatPtr           protSfp;
  ProtRefPtr           prp;
  ValNodePtr           vnp;

  /* If no internal stop codons, then done */

  if (FALSE == HasInternalStops(sfp))
    return TRUE;

  /* Get the CD region part of the feature, and */
  /* the associated protein bioseq.             */

  cdrp = (CdRegionPtr) sfp->data.value.ptrvalue;
  protBsp = BioseqFindFromSeqLoc (sfp->product);
  protSfp = SeqMgrGetBestProteinFeature (protBsp, &protContext);

  /* Get the protein name */

  prp = (ProtRefPtr) protSfp->data.value.ptrvalue;
  if (prp != NULL) {
    vnp = prp->name;
    if (NULL != vnp)
      protName = (CharPtr) vnp->data.ptrvalue;
  }

  /* Mark the CDS as Pseudo */

  protSfp->pseudo = TRUE;
  sfp->pseudo = TRUE;

  /* Get the overlapping gene. If there isn't */
  /* an overlapping gene, create one.         */

  geneSfp = SeqMgrGetOverlappingGene (sfp->location, &geneContext);

  if (NULL == geneSfp) {
    grp = CreateNewGeneRef (protName, NULL, NULL, FALSE);
    if (NULL == grp)
      return FALSE;
    geneSfp = CreateNewFeature (fcontext->bsp->seqentry, NULL,
				SEQFEAT_GENE, NULL);
    if (NULL == geneSfp)
      return FALSE;

    geneSfp->data.value.ptrvalue = (Pointer) grp;
    geneSfp->location = AsnIoMemCopy ((Pointer) sfp->location,
				  (AsnReadFunc) SeqLocAsnRead,
				  (AsnWriteFunc) SeqLocAsnWrite);
  }

  /* Mark the overlapping gene as pseudo */

  geneSfp->pseudo = TRUE;

  /* Return TRUE to continue on to the next CDS feature */

  return TRUE;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* ConvertCDSAndKeep_BioseqCallback () -- Called once for each Bioseq  */
/*                                        to convert any CDS on it into*/
/*                                        pseudogenes while keeping    */
/*                                        the CDS.                     */
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean LIBCALLBACK ConvertCDSAndKeep_BioseqCallback (BioseqPtr bsp,
					 SeqMgrBioseqContextPtr bcontext)
{
  Boolean featureFilterArray [SEQFEAT_MAX];

  /* Set up to explore only CDS features */

  MemSet ((Pointer) (featureFilterArray),
	  (int) FALSE,
	  SEQFEAT_MAX);

  featureFilterArray[SEQFEAT_CDREGION] = TRUE;

  /* Explore the Bioseq's CDS features, marking the */
  /* ones with internal stop codons as pseudo.      */

  SeqMgrExploreFeatures (bsp, NULL, ConvertCDSAndKeep_FeatureCallback, NULL,
			 featureFilterArray, NULL);

  /* Return TRUE to continue on to the next Bioseq */

  return TRUE;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* ConvertCDSToPseudoGeneAndKeep () --                                 */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void ConvertCDSToPseudoGeneAndKeep (IteM i)
{
  BaseFormPtr  bfp;
  Uint2        parenttype;
  Pointer      parentptr;
  SeqEntryPtr  sep;

  /* Get the current data */

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  if (bfp == NULL)
    return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  GetSeqEntryParent (sep, &parentptr, &parenttype);

  /* Explore all the Bioseqs looking for CDS features */

  SeqMgrExploreBioseqs (bfp->input_entityID, NULL, NULL,
			ConvertCDSAndKeep_BioseqCallback, TRUE, FALSE, TRUE);

  /* Force an update and redraw */

  DeleteMarkedObjects (bfp->input_entityID, 0, NULL);
  SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();

  /* Return successfully */

  return;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* ConvertCDSAndRemove_FeatureCallback () -- Called once for each CDS  */
/*                                           feature on a Bioseq. This */
/*                                           converts a CDS feature    */
/*                                           that contains internal    */
/*                                           stop codons into a pseudo */
/*                                           gene and removes the      */
/*                                           original CDS.             */
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean LIBCALLBACK ConvertCDSAndRemove_FeatureCallback
                               (SeqFeatPtr sfp,
				SeqMgrFeatContextPtr fcontext)
{
  SeqFeatPtr           geneSfp;
  GeneRefPtr           grp;
  BioseqPtr            protBsp;
  SeqMgrFeatContext    protContext;
  CharPtr              protName;
  SeqFeatPtr           protSfp;
  ProtRefPtr           prp;
  ValNodePtr           vnp;
  CharPtr              new_comment = NULL;

  /* If no internal stop codons, then done */

  if (FALSE == HasInternalStops(sfp))
    return TRUE;

  /* Get the associated protein bioseq. */

  protBsp = BioseqFindFromSeqLoc (sfp->product);
  protSfp = SeqMgrGetBestProteinFeature (protBsp, &protContext);
  prp = (ProtRefPtr) protSfp->data.value.ptrvalue;
  
  vnp = prp->name;
  if (NULL != vnp)
  {
    protName = (CharPtr) vnp->data.ptrvalue;
  }

  /* see if we have an overlapping gene already */
  geneSfp = SeqMgrGetOverlappingGene (sfp->location, fcontext);
  if (geneSfp == NULL)
  {
    /* convert the existing CDS into a gene */
    geneSfp = sfp;
    geneSfp->data.choice = SEQFEAT_GENE;
    grp = GeneRefNew ();
    geneSfp->data.value.ptrvalue = grp;
    geneSfp->product = SeqLocFree (geneSfp->product);
  }
  else
  {
    /* use the existing gene and remove the CDS feature */
    grp = (GeneRefPtr) geneSfp->data.value.ptrvalue;
    if (grp == NULL)
    {
      grp = GeneRefNew();
      geneSfp->data.value.ptrvalue = grp;
    }
    sfp->idx.deleteme = TRUE;
  }
  
  /* copy the protein name into the gene description field */
  if (grp != NULL && !StringHasNoText (protName))
  {
    if (!StringHasNoText (grp->desc))
    {
      if (StringHasNoText (geneSfp->comment))
      {
        geneSfp->comment = MemFree (geneSfp->comment);
        geneSfp->comment = grp->desc;
        grp->desc = NULL;
      }
      else
      {
        new_comment = (CharPtr) MemNew ((StringLen (geneSfp->comment) 
                                        + StringLen (grp->desc) + 3) * sizeof (Char));
        if (new_comment != NULL)
        {
          StringCpy (new_comment, geneSfp->comment);
          StringCat (new_comment, "; ");
          StringCat (new_comment, grp->desc);
          geneSfp->comment = MemFree (geneSfp->comment);
          geneSfp->comment = new_comment;
          grp->desc = MemFree (grp->desc);
        }
      }
    }
    grp->desc = StringSave (protName);
  }

  /* mark the gene as pseudo */
  geneSfp->pseudo = TRUE;

  /* Delete the protein Bioseq and */
  /* the CDS that points to it.    */

  protBsp->idx.deleteme = TRUE;

  /* Return TRUE to continue on to the next CDS feature */

  return TRUE;
}

/*------------------------------------------------------------------------*/
/*                                                                        */
/* ConvertCDSAndRemove_BioseqCallback () -- Called once for each Bioseq   */
/*                                          to convert any CDS on it into */
/*                                          pseudogenes.                  */
/*                                                                        */
/*------------------------------------------------------------------------*/

static Boolean LIBCALLBACK ConvertCDSAndRemove_BioseqCallback (BioseqPtr bsp,
					 SeqMgrBioseqContextPtr bcontext)
{
  Boolean featureFilterArray [SEQFEAT_MAX];

  /* Set up to explore only CDS features */

  MemSet ((Pointer) (featureFilterArray),
	  (int) FALSE,
	  sizeof (featureFilterArray));

  featureFilterArray[SEQFEAT_CDREGION] = TRUE;

  /* Explore the Bioseq's CDS features, marking the */
  /* ones with internal stop codons as pseudo.      */

  SeqMgrExploreFeatures (bsp, bcontext->userdata,
			 ConvertCDSAndRemove_FeatureCallback,
			 NULL, featureFilterArray, NULL);

  /* Return TRUE to continue on to the next Bioseq */

  return TRUE;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* ConvertCDSToPseudoGeneAndRemove () --                               */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void ConvertCDSToPseudoGeneAndRemove (IteM i)
{
  BaseFormPtr  bfp;
  Uint2        parenttype;
  Pointer      parentptr;
  SeqEntryPtr  sep;

  /* Get the current data */

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  if (bfp == NULL)
    return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  GetSeqEntryParent (sep, &parentptr, &parenttype);

  /* Explore all the Bioseqs looking for CDS features */

  SeqMgrExploreBioseqs (bfp->input_entityID, NULL, bfp,
			ConvertCDSAndRemove_BioseqCallback,
			TRUE, FALSE, TRUE);

  /* Force an update and redraw */

  DeleteMarkedObjects (bfp->input_entityID, 0, NULL);
  SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();

  /* Return successfully */

  return;
}

static void RemoveTaxonXrefsFromList (ValNodePtr PNTR list)
{
  ValNodePtr vnp, next;
  DbtagPtr   dbt;
  
  if (list == NULL) return;
  vnp = *list;
  while (vnp != NULL)
  {
  	next = vnp->next;
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    if (dbt != NULL && StringICmp ((CharPtr) dbt->db, "taxon") == 0) {
      *list = vnp->next;
      vnp->next = NULL;
      DbtagFree (dbt);
      ValNodeFree (vnp);
    } else {
      list = (ValNodePtr PNTR) &(vnp->next);
    }    
    vnp = next;
  }
}

static Boolean RemoveTaxonProc (GatherObjectPtr gop)

{
  BioSourcePtr     biop = NULL;
  OrgRefPtr        orp;
  ObjValNodePtr    ovn;
  SeqDescPtr       sdp;
  SeqFeatPtr       sfp;
  ValNodePtr       vnp;
  BoolPtr          remove_from_biosources;

  switch (gop->itemtype) {
    case OBJ_SEQFEAT :
      sfp = (SeqFeatPtr) gop->dataptr;
      if (sfp != NULL && sfp->idx.subtype == FEATDEF_BIOSRC) {
        biop = (BioSourcePtr) sfp->data.value.ptrvalue;
      }
      RemoveTaxonXrefsFromList (&(sfp->dbxref));
      break;
    case OBJ_SEQDESC :
      sdp = (SeqDescPtr) gop->dataptr;
      ovn = (ObjValNodePtr) sdp;
      if (sdp != NULL && sdp->extended != 0 && ovn->idx.subtype == Seq_descr_source ) {
        biop = (BioSourcePtr) sdp->data.ptrvalue;
      }
      break;
    default :
      return TRUE;
  }
  remove_from_biosources = (BoolPtr) (gop->userdata);
  if (!*remove_from_biosources) return TRUE;
  if (biop == NULL) return TRUE;
  orp = biop->org;
  if (orp == NULL) return TRUE;
  vnp = orp->db;
  if (vnp == NULL) return TRUE;
  RemoveTaxonXrefsFromList (&(orp->db));

  return TRUE;
}

static void RemoveTaxonXrefs (IteM i, Boolean remove_from_biosources)

{
  BaseFormPtr  bfp;
  Boolean      objMgrFilter [OBJ_MAX];
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  MemSet ((Pointer) objMgrFilter, FALSE, sizeof (objMgrFilter));
  objMgrFilter [OBJ_SEQFEAT] = TRUE;
  objMgrFilter [OBJ_SEQDESC] = TRUE;
  GatherObjectsInEntity (bfp->input_entityID, 0, NULL,
                         RemoveTaxonProc, (Pointer) &remove_from_biosources, objMgrFilter);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  Update ();
}

static void RemoveTaxonXrefsFromFeatures (IteM i)
{
  RemoveTaxonXrefs (i, FALSE);
}

static void RemoveTaxonXrefsFromFeaturesAndBioSources (IteM i)
{
  RemoveTaxonXrefs (i, TRUE);
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* SuppressGeneXref_Callback () -- Suppresses any gene xref on the feature */
/*                                 that is passed to it.                   */
/*                                                                         */
/*-------------------------------------------------------------------------*/

static Boolean LIBCALLBACK SuppressGeneXref_Callback (SeqFeatPtr sfp,
					       SeqMgrFeatContextPtr context)
{
  GeneRefPtr      grp = NULL;
  SeqFeatXrefPtr  xref;

  /* If there is a gene xref, then change it */
  /* to a suppression gene xref.             */

  grp = SeqMgrGetGeneXref (sfp);
  if (NULL != grp)
    {
      if (SeqMgrGeneIsSuppressed (grp) == FALSE)
	{
	  if (NULL != grp->locus)
	    {
	      MemFree(grp->locus);
	      grp->locus = NULL;
	    }
	  if (NULL != grp->allele)
	    {
	      MemFree(grp->allele);
	      grp->allele = NULL;
	    }
	  if (NULL != grp->desc)
	    {
	      MemFree(grp->desc);
	      grp->desc = NULL;
	    }
	  if (NULL != grp->maploc)
	    {
	      MemFree (grp->maploc);
	      grp->maploc = NULL;
	    }
	  if (NULL != grp->locus_tag)
	    {
	      MemFree (grp->locus_tag);
	      grp->locus_tag = NULL;
	    }
	  grp->db  = ValNodeFreeData (grp->db);
	  grp->syn = ValNodeFreeData (grp->syn);
	}    
    }

  /* Otherwise, if there is an overlapping gene, add */
  /* a suppression xref for it.                      */

  else if (SeqMgrGetOverlappingGene (sfp->location, NULL) != NULL)
    {
      grp = GeneRefNew ();
      if (grp != NULL) {
	xref = SeqFeatXrefNew ();
	xref->data.choice = SEQFEAT_GENE;
	xref->data.value.ptrvalue = grp;
	xref->next = sfp->xref;
	sfp->xref = xref;
      }
    }

  /* Return successfully */

  return TRUE;
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* SuppressGeneXrefsForFeatureType () -- Suppresses gene xrefs for all     */
/*                                       features of the given type.       */
/*                                                                         */
/*-------------------------------------------------------------------------*/

static void SuppressGeneXrefsForFeatureType (BioseqPtr bsp,
					     Int2      seqfeat,
					     Int2      featdef)
{
  Boolean featDefFilterArray [FEATDEF_MAX];
  Boolean seqFeatFilterArray [SEQFEAT_MAX];

  /* Check for valid parameters */

  if (bsp == NULL)
    return;

  if ((seqfeat == 0) && (featdef == 0))
    return;

  /* Explore all of the given feature type */

  WatchCursor ();
  Update ();

  if (featdef != 0)
    {
      MemSet ((Pointer) (featDefFilterArray),
	      (int) FALSE,
	      sizeof (featDefFilterArray));
      featDefFilterArray [featdef] = TRUE;
      
      SeqMgrExploreFeatures (bsp, NULL, SuppressGeneXref_Callback,
			     NULL, NULL, featDefFilterArray);
    }
  else /* featdef != 0 */
    {
      MemSet ((Pointer) (seqFeatFilterArray),
	      (int) FALSE,
	      sizeof (seqFeatFilterArray));
      seqFeatFilterArray [seqfeat] = TRUE;
      
      SeqMgrExploreFeatures (bsp, NULL, SuppressGeneXref_Callback,
			     NULL, seqFeatFilterArray, NULL);
    }
}

/*=========================================================================*/
/*                                                                         */
/* SuppressError_Callback () --                                            */
/*                                                                         */
/*=========================================================================*/

static void SuppressError_Callback (SeqFeatPtr sfp, Pointer userdata)
{
  GeneRefPtr        grp = NULL;
  SeqFeatXrefPtr    xref;
  SeqFeatPtr        gene;
  SeqMgrFeatContext geneContext;

  /* If we already have a gene xref, then we're OK */

  grp = SeqMgrGetGeneXref (sfp);
  if (grp != NULL)
    return;

  /* If we have a completely overlapping gene, we're OK */

  gene = SeqMgrGetOverlappingGene (sfp->location, &geneContext);
  if (gene != NULL)
    return;

  /* If we have a partially overlapping gene */
  /* then it is a problem.                   */

  gene = SeqMgrGetOverlappingFeature (sfp->location, FEATDEF_GENE, NULL,
				      0, NULL, SIMPLE_OVERLAP, &geneContext);
  if (gene == NULL)
    return;

  /* If we got to here, then we have no gene xref and */
  /* and a partially overlapping gene, so add on a    */
  /* blank gene xref to suppress the error message.   */

  grp = GeneRefNew ();
  if (grp != NULL) {
    xref = SeqFeatXrefNew ();
    xref->data.choice = SEQFEAT_GENE;
    xref->data.value.ptrvalue = grp;
    xref->next = sfp->xref;
    sfp->xref = xref;
  }

  /* Return successfully */

  return;
}

/*=========================================================================*/
/*                                                                         */
/* SuppressCDSGeneRangeError () --                                         */
/*                                                                         */
/*=========================================================================*/

static void SuppressCDSGeneRangeError (IteM i)

{
  BaseFormPtr  bfp;
  BioseqPtr    bsp;
  Uint2        entityID;
  SeqEntryPtr  sep;

  /* Get the Bioseq */

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  bsp =  GetBioseqGivenIDs (bfp->input_entityID, 1, OBJ_BIOSEQ);
  sep = SeqMgrGetSeqEntryForData (bsp);
  entityID = ObjMgrGetEntityIDForChoice (sep);
  sep = GetTopSeqEntryForEntityID (entityID);

  /* Explore the Bioseq's CDS features, searching for */
  /* ones that would generate a CDSGeneRange error.   */

  VisitFeaturesInSep (sep, NULL, SuppressError_Callback);

  /* Force an update and redraw */

  ObjMgrSetDirtyFlag (entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
  ArrowCursor ();
  Update ();

  /* Return successfully */

  return;
}

/*=========================================================================*/
/*                                                                         */
/* RestoreError_Callback () --                                             */
/*                                                                         */
/*=========================================================================*/

static void RestoreError_Callback (SeqFeatPtr sfp, Pointer userdata)
{
  GeneRefPtr        grp = NULL;
  SeqFeatPtr        gene;
  SeqMgrFeatContext geneContext;

  /* Check for a suppressed gene xref */

  grp = SeqMgrGetGeneXref (sfp);
  if (NULL == grp)
    return;

  if (FALSE == SeqMgrGeneIsSuppressed (grp))
    return;

  /* Check to make sure we don't have a fully overlapping gene */

  gene = SeqMgrGetOverlappingGene (sfp->location, &geneContext);
  if (NULL != gene)
    return;

  /* If we have a partially overlapping gene */
  /* then we need to remove the suppression. */

  gene = SeqMgrGetOverlappingFeature (sfp->location, FEATDEF_GENE, NULL,
				      0, NULL, SIMPLE_OVERLAP, &geneContext);
  if (gene == NULL)
    return;

  /* If we got to here, then we have a suppressed xref */
  /* and a partially overlapping gene, so remove the   */
  /* suppression.                                      */

  GeneRefFree (grp);
  sfp->xref = SeqFeatXrefFree (sfp->xref);

  /* Return successfully */

  return;
}

/*=========================================================================*/
/*                                                                         */
/* RestoreCDSGeneRangeError () --                                          */
/*                                                                         */
/*=========================================================================*/

static void RestoreCDSGeneRangeError (IteM i)

{
  BaseFormPtr  bfp;
  BioseqPtr    bsp;
  Uint2        entityID;
  SeqEntryPtr  sep;

  /* Get the Bioseq */

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  bsp =  GetBioseqGivenIDs (bfp->input_entityID, 1, OBJ_BIOSEQ);
  sep = SeqMgrGetSeqEntryForData (bsp);
  entityID = ObjMgrGetEntityIDForChoice (sep);
  sep = GetTopSeqEntryForEntityID (entityID);

  /* Explore the Bioseq's CDS features, searching for */
  /* ones that would generate a CDSGeneRange error.   */

  VisitFeaturesInSep (sep, NULL, RestoreError_Callback);

  /* Force an update and redraw */

  ObjMgrSetDirtyFlag (entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
  ArrowCursor ();
  Update ();

  /* Return successfully */

  return;
}

/*=========================================================================*/
/*                                                                         */
/* SuppressGenesOnAllRBS () -- Suppress gene xrefs on all RBS features     */
/*                                                                         */
/*=========================================================================*/

static void DoOneSuppressGeneOnRBS (BioseqPtr bsp, Pointer userdata)

{
  /* Suppress the gene xrefs for all RBS features */

  SuppressGeneXrefsForFeatureType (bsp, 0, FEATDEF_RBS);
}

static void SuppressGenesOnAllRBS (IteM i)

{
  BaseFormPtr  bfp;
  BioseqPtr    bsp;
  Uint2        entityID;
  SeqEntryPtr  sep;

  /* Get the Bioseq */

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  bsp =  GetBioseqGivenIDs (bfp->input_entityID, 1, OBJ_BIOSEQ);
  sep = SeqMgrGetSeqEntryForData (bsp);
  entityID = ObjMgrGetEntityIDForChoice (sep);
  sep = GetTopSeqEntryForEntityID (entityID);

  VisitBioseqsInSep (sep, NULL, DoOneSuppressGeneOnRBS);

  /* Force an update and redraw */

  ObjMgrSetDirtyFlag (entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
  ArrowCursor ();
  Update ();

  /* Return successfully */

  return;
}

/*=======================================================================*/
/*                                                                       */
/*  CopyGeneRef () -                                                     */
/*                                                                       */
/*=======================================================================*/

static GeneRefPtr CopyGeneRef (GeneRefPtr srcGrp)
{
  GeneRefPtr   destGrp;
  DbtagPtr     destDbt;
  DbtagPtr     srcDbt;
  ValNodePtr   vnp;

  destGrp = GeneRefNew ();

  /* Copy the string fields from source to destination */
  
  if (srcGrp->locus != NULL)
    destGrp->locus = StringSave (srcGrp->locus);

  if (srcGrp->allele != NULL)
    destGrp->allele = StringSave (srcGrp->allele);

  if (srcGrp->desc != NULL)
    destGrp->desc = StringSave (srcGrp->desc);

  if (srcGrp->maploc != NULL)
    destGrp->maploc = StringSave (srcGrp->maploc);

  if (srcGrp->locus_tag != NULL)
    destGrp->locus_tag = StringSave (srcGrp->locus_tag);

  /* Copy the DB references */

  destGrp->db = NULL;

  if (srcGrp->db != NULL)
    {
      vnp = srcGrp->db;
      while (vnp != NULL)
	{
	  ValNodeAdd (&(destGrp->db));
	  destDbt = DbtagNew ();
	  srcDbt  = (DbtagPtr) vnp->data.ptrvalue;
	  if (srcDbt != NULL)
	    {
	      if (srcDbt->db != NULL)
		{
		  destDbt->db = (CharPtr) MemNew (sizeof(srcDbt->db));
		  StringCpy (destDbt->db, srcDbt->db);
		}
	      if (srcDbt->tag != NULL)
		{
		  destDbt->tag = ObjectIdNew ();
		  destDbt->tag->id = srcDbt->tag->id;
		  destDbt->tag->str = (CharPtr)MemNew(sizeof(srcDbt->tag->str));
		  StringCpy (destDbt->tag->str, srcDbt->tag->str);
		}
	    }
	  vnp = vnp->next;
	}
    }

  /* Copy the synonyms */

  if (srcGrp->syn != NULL)
    {
      vnp = srcGrp->syn;
      while (vnp != NULL)
	{
	  ValNodeCopyStr (&(destGrp->syn), vnp->choice,
			  (CharPtr) vnp->data.ptrvalue);
	  vnp = vnp->next;
	}
    }

  /* Return the new gene reference */

  return destGrp;

}

static void ZapRBSGene (SeqFeatPtr sfp, Pointer userdata)

{
  Int4               diff;
  SeqMgrFeatContext  gcontext;
  SeqFeatPtr         gene;
  GeneRefPtr         grp;

  if (sfp->idx.subtype != FEATDEF_RBS) return;
  grp = SeqMgrGetGeneXref (sfp);
  if (grp != NULL && (! SeqMgrGeneIsSuppressed (grp))) return;
  gene = SeqMgrGetOverlappingGene (sfp->location, &gcontext);
  if (gene == NULL) return;
  diff = SeqLocAinB (sfp->location, gene->location);
  if (diff == 0) {
    gene->idx.deleteme = TRUE;
  } else if (diff > 0) {
    SeqLocSubtract (gene->location, sfp->location);
  }
}

static void RemoveRBSGenes (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  WatchCursor ();
  Update ();

  VisitFeaturesInSep (sep, NULL, ZapRBSGene);

  DeleteMarkedObjects (bfp->input_entityID, 0, NULL);
  SeqMgrClearFeatureIndexes (bfp->input_entityID, NULL);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();
}

/*=========================================================================*/
/*                                                                         */
/* MapRBSsToDownstreamGene () -- Creates a xref to the correct gene for    */
/*                               all RBS features.                         */
/*                                                                         */
/*=========================================================================*/

static void MapRBSsToDownstreamGene (IteM i)
{
  BaseFormPtr        bfp;
  BioseqPtr          bsp;
  Uint2              entityID;
  Boolean            found;
  SeqMgrFeatContext  cdsContext;
  Int4               cdsLeft;
  Int4               cdsRight;
  SeqFeatPtr         cdsSfp = NULL;
  SeqLocPtr          cdsSlp;
  GeneRefPtr         geneGrp;
  SeqFeatPtr         geneSfp = NULL;
  Boolean            overshot = FALSE;
  SeqFeatPtr         prevCdsSfp = NULL;
  SeqFeatXrefPtr     prevXref = NULL;
  SeqMgrFeatContext  rbsContext;
  GeneRefPtr         rbsGrp;
  Int4               rbsLeft;
  Int4               rbsRight;
  SeqFeatPtr         rbsSfp = NULL;
  SeqLocPtr          rbsSlp;
  Uint1              strand;
  SeqFeatXrefPtr     xref = NULL;

  /* Get the Bioseq */

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  WatchCursor ();
  Update ();

  bsp =  GetBioseqGivenIDs (bfp->input_entityID,
			   bfp->input_itemID,
			   bfp->input_itemtype);

  rbsSfp = SeqMgrGetNextFeature (bsp, NULL, 0, FEATDEF_RBS, &rbsContext);

  /* Loop through all RBS features */

  while (NULL != rbsSfp)
    {

      /* If the feature has an existing gene xref */
      /* then we need to delete it.               */
  
      xref = rbsSfp->xref;
      while (xref != NULL && xref->data.choice != SEQFEAT_GENE)
	{
	  prevXref = xref;
	  xref = xref->next;
	}
      
      if (xref != NULL)
	{
	  rbsGrp = (GeneRefPtr) xref->data.value.ptrvalue;
	  rbsGrp = GeneRefFree (rbsGrp);
	  if (prevXref != NULL)
	    prevXref->next = xref->next;
	  else
	    rbsSfp->xref = xref->next;
	  SeqFeatXrefFree (xref);
	  xref = NULL;
	}

      /* Search for a 'downstream' gene xref */
  
      if (overshot)
	overshot = FALSE;
      else
	{
	  prevCdsSfp = cdsSfp;
	  cdsSfp = SeqMgrGetNextFeature (bsp, cdsSfp, 0, FEATDEF_CDS,
					 &cdsContext);
	}
      
      rbsSlp  = rbsSfp->location;
      strand  = SeqLocStrand (rbsSlp);

      if (strand == Seq_strand_plus)
	{
	  /* If the gene starts at or 'after' the  */
	  /* RBS, then the RBS probably belongs to */
	  /* it, so make that gene a xref for the  */
	  /* RBS feature.                          */
	  
	  found = FALSE;
	  while ((cdsSfp != NULL) && (!found))
	    {
	      cdsSlp  = cdsSfp->location;
	      cdsLeft = GetOffsetInBioseq (cdsSlp, bsp,
					   SEQLOC_LEFT_END);
	      rbsLeft = GetOffsetInBioseq (rbsSlp , bsp,
					   SEQLOC_LEFT_END);
	      if (rbsLeft <= cdsLeft)
		{
		  found = TRUE;
		  geneSfp = SeqMgrGetOverlappingGene (cdsSlp, NULL);
		  if (geneSfp != NULL) {
		    geneGrp = (GeneRefPtr) geneSfp->data.value.ptrvalue;
		    rbsGrp = CopyGeneRef (geneGrp);
		    if (rbsGrp != NULL)
		      {
		        xref = SeqFeatXrefNew ();
		        xref->data.choice = SEQFEAT_GENE;
		        xref->data.value.ptrvalue = rbsGrp;
		        xref->next = rbsSfp->xref;
		        rbsSfp->xref = xref;
		      }
		  }
		}
	      else
		{
		  if (overshot)
		    overshot = FALSE;
		  else
		    {
		      prevCdsSfp = cdsSfp;
		      cdsSfp = SeqMgrGetNextFeature (bsp, cdsSfp, 0,
						     FEATDEF_CDS,
						     &cdsContext);
		    }
		}
	    }
	}
      else if (strand == Seq_strand_minus)
	{
	  found = FALSE;
	  while ((cdsSfp != NULL) && (!found))
	    {
	      cdsSlp   = cdsSfp->location;
	      cdsRight = GetOffsetInBioseq (cdsSlp, bsp,
					    SEQLOC_RIGHT_END);
	      rbsRight = GetOffsetInBioseq (rbsSlp , bsp,
					    SEQLOC_RIGHT_END);
	      if (cdsRight > rbsRight)
		{
		  found = TRUE;
		  if (prevCdsSfp != NULL)
		    {
		      overshot = TRUE;
		      geneSfp = SeqMgrGetOverlappingGene (prevCdsSfp->location,
							  NULL);
		      if (geneSfp != NULL) {
		        geneGrp = (GeneRefPtr) geneSfp->data.value.ptrvalue;
		        rbsGrp = CopyGeneRef (geneGrp);
		        if (rbsGrp != NULL)
			  {
			    xref = SeqFeatXrefNew ();
			    xref->data.choice = SEQFEAT_GENE;
			    xref->data.value.ptrvalue = rbsGrp;
			    xref->next = rbsSfp->xref;
			    rbsSfp->xref = xref;
			  }
		      }
			
		    }
		}
	      else
		{
		  if (overshot)
		    overshot = FALSE;
		  else
		    {
		      prevCdsSfp = cdsSfp;
		      cdsSfp = SeqMgrGetNextFeature (bsp, cdsSfp, 0,
						     FEATDEF_CDS,
						     &cdsContext);
		    }
		}
	      
	    }
	}
      
      rbsSfp  = SeqMgrGetNextFeature (bsp, rbsSfp, 0, FEATDEF_RBS, &rbsContext);
    }

  /* Force an update and redraw */

  entityID = ObjMgrGetEntityIDForPointer (bsp);
  ObjMgrSetDirtyFlag (entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
  ArrowCursor ();
  Update ();

  /* Return successfully */

  return;

}

static Boolean MarkBankitComments (GatherObjectPtr gop)

{
  ObjectIdPtr    oip;
  ObjValNodePtr  ovn;
  SeqDescrPtr    sdp;
  UserObjectPtr  uop;

  if (gop == NULL || gop->itemtype != OBJ_SEQDESC) return TRUE;
  sdp = (SeqDescrPtr) gop->dataptr;
  if (sdp == NULL || sdp->choice != Seq_descr_user || sdp->extended == 0) return TRUE;
  uop = (UserObjectPtr) sdp->data.ptrvalue;
  if (uop == NULL) return TRUE;
  oip = uop->type;
  if (oip == NULL || StringCmp (oip->str, "Submission") != 0) return TRUE;
  ovn = (ObjValNodePtr) sdp;
  ovn->idx.deleteme = TRUE;
  return TRUE;
}

static void RemoveBankitComments (IteM i)

{
  BaseFormPtr  bfp;
  Boolean      objMgrFilter [OBJ_MAX];
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  MemSet ((Pointer) objMgrFilter, FALSE, sizeof (objMgrFilter));
  objMgrFilter [OBJ_SEQDESC] = TRUE;
  GatherObjectsInEntity (bfp->input_entityID, 0, NULL,
                         MarkBankitComments, NULL, objMgrFilter);
  DeleteMarkedObjects (bfp->input_entityID, 0, NULL);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void DoCopyBankItComments (SeqDescrPtr sdp, Pointer userdata)

{
  ObjectIdPtr    oip;
  CharPtr        str;
  UserFieldPtr   ufp;
  UserObjectPtr  uop;
  SeqDescrPtr    vnp;

  if (sdp == NULL || sdp->choice != Seq_descr_user || sdp->extended == 0) return;
  uop = (UserObjectPtr) sdp->data.ptrvalue;
  if (uop == NULL) return;
  oip = uop->type;
  if (oip == NULL || StringCmp (oip->str, "Submission") != 0) return;

  for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
    oip = ufp->label;
    if (oip != NULL && StringCmp (oip->str, "AdditionalComment") == 0) {
      str = ufp->data.ptrvalue;
      if (! StringHasNoText (str)) {
        vnp = SeqDescrNew (NULL);
        if (vnp != NULL) {
          vnp->choice = Seq_descr_comment;
          vnp->data.ptrvalue = StringSave (str);
          vnp->next = sdp->next;
          sdp->next = vnp;
        }
      }
    }
  }

}

static void CopyBankitComments (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  VisitDescriptorsInSep (sep, NULL, DoCopyBankItComments);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static ENUM_ALIST(molinfo_biomol_alist)
  {"Any",                  254},
  {" ",                      0},
  {"Genomic DNA or RNA",     1},
  {"Precursor RNA",          2},
  {"mRNA [cDNA]",            3},
  {"Ribosomal RNA",          4},
  {"Transfer RNA",           5},
  {"Small nuclear RNA",      6},
  {"Small cytoplasmic RNA",  7},
  {"Peptide",                8},
  {"Other-Genetic",          9},
  {"Genomic-mRNA",          10},
  {"cRNA",                  11},
  {"Small nucleolar RNA",   12},
  {"Transcribed RNA",       13},
  {"Other",                255},
END_ENUM_ALIST

static ENUM_ALIST(molinfo_tech_alist)
  {"Any",               254},
  {" ",                   0},
  {"Standard",            1},
  {"EST",                 2},
  {"STS",                 3},
  {"Survey",              4},
  {"Genetic Map",         5},
  {"Physical Map",        6},
  {"Derived",             7},
  {"Concept-Trans",       8},
  {"Seq-Pept",            9},
  {"Both",               10},
  {"Seq-Pept-Overlap",   11},
  {"Seq-Pept-Homol",     12},
  {"Concept-Trans-A",    13},
  {"HTGS 0",             18},
  {"HTGS 1",             14},
  {"HTGS 2",             15},
  {"HTGS 3",             16},
  {"FLI_cDNA",           17},
  {"HTC",                19},
  {"WGS",                20},
  {"Barcode",            21},
  {"Composite-WGS-HTGS", 22},
  {"Other:",            255},
END_ENUM_ALIST

static ENUM_ALIST(molinfo_complete_alist)
  {"Any",     254},
  {" ",         0},
  {"Complete",  1},
  {"Partial",   2},
  {"No Left",   3},
  {"No Right",  4},
  {"No Ends",   5},
  {"Other",   255},
END_ENUM_ALIST

static ENUM_ALIST(mol_alist)
{"Any",             Seq_mol_other - 1},
{" ",               0},
{"DNA",             Seq_mol_dna},
{"RNA",             Seq_mol_rna},
{"Protein",         Seq_mol_aa},
{"Nucleotide",      Seq_mol_na},
{"Other",           Seq_mol_other},
END_ENUM_ALIST

static ENUM_ALIST(topology_alist)
{"Any",             254},
{" ",               0},
{"Linear",          TOPOLOGY_LINEAR},
{"Circular",        TOPOLOGY_CIRCULAR},
{"Tandem",          TOPOLOGY_TANDEM},
{"Other",           255},
END_ENUM_ALIST

static ENUM_ALIST(strand_alist)
{"Any",             254},
{" ",               Seq_strand_unknown},
{"Single",          Seq_strand_plus},
{"Double",          Seq_strand_minus},
{"Mixed",           Seq_strand_both},
{"Mixed Rev",       Seq_strand_both_rev},
{"Other",           Seq_strand_other},
END_ENUM_ALIST

static ValNodePtr MakeValNodeListFromEnum ( EnumFieldAssocPtr al)
{
  EnumFieldAssocPtr efap;
  ValNodePtr        list;

  efap = al;
  list = NULL;
  while (efap->name != NULL)
  {
    ValNodeAddStr (&list, efap->value, StringSave (efap->name));
    efap ++;
  }
  return list;
}

static void ReplaceValNodeString (ValNodePtr list, CharPtr find, CharPtr repl)
{
  ValNodePtr vnp;

  if (list == NULL || find == NULL || repl == NULL) return;

  for (vnp = list; vnp != NULL; vnp = vnp->next)
  {
    if (StringCmp (vnp->data.ptrvalue, find) == 0)
    {
      MemFree (vnp->data.ptrvalue);
      vnp->data.ptrvalue = StringSave (repl);
    }
  }
}

extern void InitValNodePopup (ValNodePtr list, PopuP p)
{
  ValNodePtr vnp;

  for (vnp = list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->data.ptrvalue != NULL)
    {
      PopupItem (p, vnp->data.ptrvalue);
    }
  }
}

extern Int2 GetValNodePopup (PopuP p, ValNodePtr list)
{
  ValNodePtr vnp;
  Int2       popupval;

  popupval = GetValue (p);
  if (popupval == 0)
  {
    return -1;
  }
  for (vnp = list; vnp != NULL && popupval > 1; vnp = vnp->next)
  {
    if (vnp->data.ptrvalue != NULL)
    {
      popupval --;
    }
  }
  if (popupval > 1 || vnp == NULL) return -1;
  return vnp->choice;
}

extern void SetValNodePopupValue (ValNodePtr list, PopuP p, CharPtr val)
{
  ValNodePtr vnp;
  Int2       popupval;

  popupval = 1;
  for (vnp = list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->data.ptrvalue != NULL)
    {
      if (StringCmp (vnp->data.ptrvalue, val) == 0)
      {
        SetValue (p, popupval);
      }
      popupval ++;
    }
  }
}

typedef struct molinfoblock {
  PopuP           moltype;
  ValNodePtr      moltype_values;
  PopuP           technique;
  ValNodePtr      technique_values;
  PopuP           complete;
  ValNodePtr      complete_values;
  PopuP           molPopup;
  ValNodePtr      mol_values;
  PopuP           topologyPopup;
  ValNodePtr      topology_values;
  PopuP           strandPopup;
  ValNodePtr      strand_values;
} MolInfoBlock, PNTR MolInfoBlockPtr;

static void FreeMolInfoBlockData (MolInfoBlockPtr mibp)
{
  if (mibp == NULL) return;
  ValNodeFreeData (mibp->moltype_values);
  mibp->moltype_values = NULL;
  ValNodeFreeData (mibp->technique_values);
  mibp->technique_values = NULL;
  ValNodeFreeData (mibp->complete_values);
  mibp->complete_values = NULL;
  ValNodeFreeData (mibp->mol_values);
  mibp->mol_values = NULL;
  ValNodeFreeData (mibp->topology_values);
  mibp->topology_values = NULL;
  ValNodeFreeData (mibp->strand_values);
  mibp->strand_values = NULL;
}

typedef struct molinfoedit {
  DESCRIPTOR_FORM_BLOCK
  SeqEntryPtr     sep;
  MolInfoBlock    from;
  MolInfoBlock    to;
  GrouP           constraint;
  Boolean         nucsOK;
  Boolean         protsOK;
} MolInfoEdit, PNTR MolInfoEditPtr;

static CharPtr  labels [] = {
  "Molecule", "Technique", "Completedness",
  "Class", "Topology", "Strand", NULL
};

static void SetupMolInfoBlockPopup (GrouP g, PopuP PNTR p, ValNodePtr PNTR v,
                                    EnumFieldAssocPtr al, Boolean IsTo)
{
  *p = PopupList (g, TRUE, NULL);
  *v = MakeValNodeListFromEnum ( al );
  if (IsTo)
  {
    ReplaceValNodeString (*v, "Any", "No change");
  }
  InitValNodePopup (*v, *p);
  SetValue (*p, 1);
}

static void CreateMolInfoBlock (MolInfoBlockPtr mibp, GrouP h, Boolean IsTo)

{
  GrouP  g;
  Int2   wid;

  if (mibp == NULL || h == NULL) return;
  SelectFont (programFont);
  wid = MaxStringWidths (labels);
  SelectFont (systemFont);

  g = HiddenGroup (h, -2, 0, NULL);

  StaticPrompt (g, "Molecule", wid, popupMenuHeight, programFont, 'l');
  SetupMolInfoBlockPopup (g, &(mibp->moltype), &(mibp->moltype_values),
                          molinfo_biomol_alist, IsTo);

  StaticPrompt (g, "Technique", wid, popupMenuHeight, programFont, 'l');
  SetupMolInfoBlockPopup (g, &(mibp->technique), &(mibp->technique_values),
                          molinfo_tech_alist, IsTo);

  StaticPrompt (g, "Completedness", wid, popupMenuHeight, programFont, 'l');
  SetupMolInfoBlockPopup (g, &(mibp->complete), &(mibp->complete_values),
                          molinfo_complete_alist, IsTo);

  StaticPrompt (g, "Class", wid, popupMenuHeight, programFont, 'l');
  SetupMolInfoBlockPopup (g, &(mibp->molPopup), &(mibp->mol_values),
                          mol_alist, IsTo);

  StaticPrompt (g, "Topology", wid, popupMenuHeight, programFont, 'l');
  SetupMolInfoBlockPopup (g, &(mibp->topologyPopup), &(mibp->topology_values),
                          topology_alist, IsTo);

  StaticPrompt (g, "Strand", wid, popupMenuHeight, programFont, 'l');
  SetupMolInfoBlockPopup (g, &(mibp->strandPopup), &(mibp->strand_values),
                          strand_alist, IsTo);
}

static void EditMolInfoCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)
{
  BioseqPtr       bsp;
  BioseqSetPtr    bssp;
  MolInfoEditPtr  miep;
  MolInfoPtr      mip;
  ValNodePtr      sdp;

  if (mydata == NULL) return;
  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  miep = (MolInfoEditPtr) mydata;
  if (miep == NULL) return;
  sdp = NULL;
  bsp = NULL;
  bssp = NULL;

  if (IS_Bioseq (sep)) {

    bsp = (BioseqPtr) sep->data.ptrvalue;
    if ((ISA_na (bsp->mol) && miep->nucsOK) ||
        (ISA_aa (bsp->mol) && miep->protsOK)) {
      if ((Uint1)GetValNodePopup (miep->from.molPopup,
                                  miep->from.mol_values) == bsp->mol
        || GetValue (miep->from.molPopup) == 1)
      {
        if (GetValue (miep->to.molPopup) != 1)
        {
          bsp->mol = (Uint1) GetValNodePopup (miep->to.molPopup,
                                              miep->to.mol_values);
        }
      }
      if ((Uint1)GetValNodePopup (miep->from.strandPopup,
                                  miep->to.strand_values) == bsp->strand
        || GetValue (miep->from.strandPopup) == 1)
      {
        if (GetValue (miep->to.strandPopup) != 1)
        {
          bsp->strand = (Uint1) GetValNodePopup (miep->to.strandPopup,
                                                 miep->to.strand_values);
        }
      }
      if ((Uint1) GetValNodePopup (miep->from.topologyPopup,
                                   miep->from.topology_values) == bsp->topology
        || GetValue (miep->from.topologyPopup) == 1)
      {
        if (GetValue (miep->to.topologyPopup) != 1)
        {
          bsp->topology = (Uint1) GetValNodePopup (miep->to.topologyPopup,
                                                   miep->to.topology_values);
        }
      }
      sdp = bsp->descr;
    }
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp->_class == 2 || bssp->_class == 4) {
      if (! miep->nucsOK) return;
    }
    sdp = bssp->descr;
  } else {
    return;
  }

  while (sdp != NULL) {
    if (sdp->choice == Seq_descr_molinfo && sdp->data.ptrvalue != NULL) {
      mip = (MolInfoPtr) sdp->data.ptrvalue;
      if ((Uint2) GetValNodePopup (miep->from.moltype,
                                   miep->from.moltype_values) == mip->biomol
        || GetValue (miep->from.moltype) == 1)
      {
        if ( GetValue (miep->to.moltype) != 1)
        {
          mip->biomol = (Uint1) GetValNodePopup (miep->to.moltype,
                                                 miep->to.moltype_values);
        }
      }
      if ((Uint1) GetValNodePopup (miep->from.technique,
                                   miep->from.technique_values) == mip->tech
        || GetValue (miep->from.technique) == 1)
      {
        if ( GetValue (miep->to.technique) != 1)
        {
          mip->tech = (Uint1) GetValNodePopup (miep->to.technique,
                                     miep->to.technique_values);
        }
      }
      if ((Uint1) GetValNodePopup (miep->from.complete,
                              miep->from.complete_values) == mip->completeness
        || GetValue (miep->from.complete) == 1)
      {
        if ( GetValue (miep->to.complete) != 1)
        {
          mip->completeness = (Uint1) GetValNodePopup (miep->to.complete,
                                                   miep->to.complete_values);
        }
      }
    }
    sdp = sdp->next;
  }
}

static void DoProcessEditMolInfo (ButtoN b)

{
  MolInfoEditPtr  miep;
  Int2            val;

  miep = (MolInfoEditPtr) GetObjectExtra (b);
  if (miep == NULL) return;
  Hide (miep->form);
  WatchCursor ();
  Update ();
  val = GetValue (miep->constraint);
  miep->nucsOK = (Boolean) (val == 1 || val == 2);
  miep->protsOK = (Boolean) (val == 1 || val == 3);
  SeqEntryExplore (miep->sep, (Pointer) miep, EditMolInfoCallback);
  FreeMolInfoBlockData (&(miep->from));
  FreeMolInfoBlockData (&(miep->to));
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (miep->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, miep->input_entityID, 0, 0);
  Remove (miep->form);
}

static void EditMolInfoFields (IteM i)

{
  ButtoN          b;
  BaseFormPtr     bfp;
  GrouP           c;
  GrouP           g;
  GrouP           h;
  GrouP           k;
  MolInfoEditPtr  miep;
  SeqEntryPtr     sep;
  WindoW          w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  miep = (MolInfoEditPtr) MemNew (sizeof (MolInfoEdit));
  if (miep == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "Molecule Information Editor", StdCloseWindowProc);
  SetObjectExtra (w, miep, StdCleanupFormProc);
  miep->form = (ForM) w;
  miep->formmessage = NULL;

  miep->sep = sep;
  miep->input_entityID = bfp->input_entityID;

  k = HiddenGroup (w, 2, 0, NULL);
  StaticPrompt (k, "Constraint", 0, stdLineHeight, programFont, 'l');
  miep->constraint = HiddenGroup (k, 4, 0, NULL);
  RadioButton (miep->constraint, "All Sequences");
  RadioButton (miep->constraint, "Nucleotides");
  RadioButton (miep->constraint, "Proteins");
  SetValue (miep->constraint, 2);

  g = HiddenGroup (w, 2, 0, NULL);
  h = HiddenGroup (g, 0, 2, NULL);
  StaticPrompt (h, "From", 0, 0, programFont, 'c');
  CreateMolInfoBlock (&miep->from, h, FALSE);
  h = HiddenGroup (g, 0, 2, NULL);
  StaticPrompt (h, "To", 0, 0, programFont, 'c');
  CreateMolInfoBlock (&miep->to, h, TRUE);

  c = HiddenGroup (w, 2, 0, NULL);
  b = DefaultButton (c, "Accept", DoProcessEditMolInfo);
  SetObjectExtra (b, miep, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) k, (HANDLE) g, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}

static void RibosomalRNAToGenomicDNAVisitFunc (
  BioseqPtr bsp,
  Pointer userdata
)
{
  SeqDescrPtr sdp;
  MolInfoPtr mip;
  SeqMgrDescContext  dcontext;

  if (bsp == NULL) return;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
  if (sdp == NULL 
    || sdp->choice != Seq_descr_molinfo
    || sdp->data.ptrvalue == NULL)
  {
    return;
  }
  mip = (MolInfoPtr) sdp->data.ptrvalue;
  if (mip->biomol == MOLECULE_TYPE_RRNA)
  {
    mip->biomol = MOLECULE_TYPE_GENOMIC;
    bsp->mol = MOLECULE_CLASS_DNA;
    bsp->strand = 0;
    bsp->topology = 0;
  }
}

static void RibosomalRNAToGenomicDNA (BaseFormPtr bfp)
{
  SeqEntryPtr     sep;

  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  VisitBioseqsInSep (sep, NULL, RibosomalRNAToGenomicDNAVisitFunc);

  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void RibosomalRNAToGenomicDNAMenuItem (IteM i)
{
  BaseFormPtr     bfp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  RibosomalRNAToGenomicDNA (bfp);
}

static void RibosomalRNAToGenomicDNAToolBtn (ButtoN b)
{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;
  
  RibosomalRNAToGenomicDNA (bfp);
}

static void DoApplyMolInfo (SeqEntryPtr sep, MolInfoEditPtr miep)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  MolInfoPtr    mip;
  ValNodePtr    vnp;

  if (sep == NULL || sep->data.ptrvalue == NULL || miep == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
/* this also delves into nuc-prot sets */
    if (bssp != NULL && (bssp->_class == 7 ||
                         (IsPopPhyEtcSet (bssp->_class)) ||
                         bssp->_class == 1)) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        DoApplyMolInfo (sep, miep);
      }
      return;
    }
  }
  bsp = NULL;
  bssp = NULL;
  if (SeqEntryGetSeqDescr (sep, Seq_descr_molinfo, NULL) != NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if ((ISA_na (bsp->mol) && miep->nucsOK) ||
        (ISA_aa (bsp->mol) && miep->protsOK)) {
      if ( GetValue (miep->to.molPopup) != 1)
      {
        bsp->mol = (Uint1) GetValNodePopup (miep->to.molPopup,
                                            miep->to.mol_values);
      }
      if ( GetValue (miep->to.strandPopup) != 1)
      {
        bsp->strand = (Uint1) GetValNodePopup (miep->to.strandPopup,
                                               miep->to.strand_values);
      }
      if (GetValue (miep->to.topologyPopup) != 1)
      {
        bsp->topology = (Uint1) GetValNodePopup (miep->to.topologyPopup,
                                                 miep->to.topology_values);
      }
    } else return;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp->_class == 2 || bssp->_class == 4) {
      if (! miep->nucsOK) return;
    }
  } else return;
  mip = MolInfoNew ();
  if (mip == NULL) return;
  vnp = CreateNewDescriptor (sep, Seq_descr_molinfo);
  if (vnp != NULL) {
    vnp->data.ptrvalue = (Pointer) mip;
    if ( GetValue (miep->to.moltype) != 1)
    {
      mip->biomol = (Uint1) GetValNodePopup (miep->to.moltype,
                                             miep->to.moltype_values);
    }
    if ( GetValue (miep->to.technique) != 1)
    {
      mip->tech = (Uint1) GetValNodePopup (miep->to.technique,
                                           miep->to.technique_values);
    }
    if ( GetValue (miep->to.complete) != 1)
    {
      mip->completeness = (Uint1) GetValNodePopup (miep->to.complete,
                                                   miep->to.complete_values);
    }
  }
}

static void DoProcessApplyMolInfo (ButtoN b)

{
  MolInfoEditPtr  miep;
  Int2            val;

  miep = (MolInfoEditPtr) GetObjectExtra (b);
  if (miep == NULL) return;
  Hide (miep->form);
  WatchCursor ();
  Update ();
  val = GetValue (miep->constraint);
  miep->nucsOK = (Boolean) (val == 1);
  miep->protsOK = (Boolean) (val == 2);
  DoApplyMolInfo (miep->sep, miep);
  FreeMolInfoBlockData (&miep->to);
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (miep->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, miep->input_entityID, 0, 0);
  Remove (miep->form);
}

static void ApplyMolInfo (IteM i)

{
  ButtoN          b;
  BaseFormPtr     bfp;
  GrouP           c;
  GrouP           g;
  GrouP           h;
  GrouP           k;
  MolInfoEditPtr  miep;
  SeqEntryPtr     sep;
  WindoW          w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  miep = (MolInfoEditPtr) MemNew (sizeof (MolInfoEdit));
  if (miep == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "Apply Molecule Information", StdCloseWindowProc);
  SetObjectExtra (w, miep, StdCleanupFormProc);
  miep->form = (ForM) w;
  miep->formmessage = NULL;

  miep->sep = sep;
  miep->input_entityID = bfp->input_entityID;

  k = HiddenGroup (w, 2, 0, NULL);
  StaticPrompt (k, "Constraint", 0, stdLineHeight, programFont, 'l');
  miep->constraint = HiddenGroup (k, 4, 0, NULL);
  RadioButton (miep->constraint, "Nucleotides");
  RadioButton (miep->constraint, "Proteins");
  SetValue (miep->constraint, 0);

  g = HiddenGroup (w, 2, 0, NULL);
  h = HiddenGroup (g, 0, 2, NULL);
  CreateMolInfoBlock (&miep->to, h, TRUE);

  c = HiddenGroup (w, 2, 0, NULL);
  b = DefaultButton (c, "Accept", DoProcessApplyMolInfo);
  SetObjectExtra (b, miep, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) k, (HANDLE) g, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}

typedef struct objstringdata 
{
  CharPtr match;
  Boolean found;
  Boolean insensitive;
  Boolean whole_word;
} ObjStringData, PNTR ObjStringPtr;


static Boolean StringMatchesConstraint (CharPtr pchSource, ObjStringPtr osp)
{
  CharPtr pchFind;
  CharPtr pFound;
  
  if (pchSource == NULL || osp == NULL) return FALSE;
  
	pchFind = osp->match;
	  
	if (osp->insensitive)
	{
	  pFound = StringISearch (pchSource, pchFind);
	}
	else
	{
	  pFound = StringSearch (pchSource, pchFind);
	}
	  
	if (pFound != NULL)
	{
	  if (!osp->whole_word)
	  {
	    return TRUE;
	  }
	  else if ((pFound == pchSource || (! isalpha ((Int4)(*(pFound - 1))) && ! isdigit ((Int4)(*(pFound - 1)))))
	           && ! isalpha ((Int4)(*(pFound + StringLen (pchFind))))
	           && ! isdigit ((Int4)(*(pFound + StringLen (pchFind)))))
	  {
	    return TRUE;
	  }
	}
	return FALSE;
}

static void LIBCALLBACK AsnWriteSearchCallBack (AsnExpOptStructPtr pAEOS)

{
  CharPtr        pchSource;
  ObjStringPtr   osp;

  osp = (ObjStringPtr) pAEOS->data;
  if (ISA_STRINGTYPE (AsnFindBaseIsa (pAEOS->atp))) 
  {
	  pchSource = (CharPtr) pAEOS->dvp->ptrvalue;
	  osp->found |= StringMatchesConstraint (pchSource, osp);
  }
}


static Boolean ObjectHasSubstring (ObjMgrTypePtr omtp, AsnIoPtr aip, Pointer ptr, ObjStringPtr osp)

{
  osp->found = FALSE;
  (omtp->asnwrite) (ptr, aip, NULL);
  return osp->found;
}

extern void OperateOnBioseqFeaturesWithText 
(BioseqPtr         bsp,
 Pointer           userdata)
{
  AsnExpOptPtr            aeop;
  AsnIoPtr                aip;
  ObjStringData           osd;
  SeqFeatPtr              sfp;
  ObjMgrPtr               omp;
  ObjMgrTypePtr           omtp;
  SeqMgrFeatContext       fcontext;
  FeaturesWithTextPtr     fdp;
  
  fdp = (FeaturesWithTextPtr) userdata;
  if (bsp == NULL || fdp == NULL || fdp->callback == NULL || fdp->search_text == NULL) return; 
  omp = ObjMgrGet ();
  if (omp == NULL) return;
  omtp = ObjMgrTypeFind (omp, OBJ_SEQFEAT, NULL, NULL);
  if (omtp == NULL) return;

  aip = AsnIoNullOpen ();
  osd.insensitive = fdp->case_insensitive;
  osd.whole_word = fdp->whole_word;
  
  aeop = AsnExpOptNew (aip, NULL, NULL, AsnWriteSearchCallBack);
  if (aeop != NULL) {
    aeop->user_data = (Pointer) &osd;
  }
  osd.match = fdp->search_text;

  sfp = NULL;
  while ((sfp = SeqMgrGetNextFeature (bsp, sfp, fdp->seqFeatChoice, fdp->featDefChoice, &fcontext)) != NULL)
  {
    if (fdp->no_text || ObjectHasSubstring (omtp, aip, (Pointer) sfp, &osd)) 
    {
      fdp->callback (sfp, fdp->userdata);  
    }
    else if (StringMatchesConstraint(fcontext.label, &osd))
    {
      fdp->callback (sfp, fdp->userdata);  
    }
  }
  AsnIoClose (aip);
}

typedef struct seqentryitemswithtext 
{
  FeaturesWithTextPtr    fdp;
  DescriptorsWithTextPtr ddp;
  ObjMgrTypePtr          omtp;
  AsnIoPtr               aip;
  Uint2                  entityID;
  ObjStringData          osd;
} SeqEntryItemsWithTextData, PNTR SeqEntryItemsWithTextPtr;

static void SeqEntryFeaturesWithTextCallback (SeqFeatPtr sfp, Pointer userdata)
{
  SeqMgrFeatContext fcontext;
  SeqEntryItemsWithTextPtr sp;
  
  if (sfp == NULL || userdata == NULL) return;
  sp = (SeqEntryItemsWithTextPtr) userdata;
  sfp = SeqMgrGetDesiredFeature (sp->entityID, NULL, 0, 0, sfp, &fcontext);
  if (sfp == NULL) return;
  
  if (sp->fdp->seqFeatChoice != 0 && sp->fdp->seqFeatChoice != sfp->data.choice) return;
  if (sp->fdp->featDefChoice != 0 && sp->fdp->featDefChoice != sfp->idx.subtype) return;
  
  if (sp->fdp->no_text
      || (! sp->fdp->act_when_string_not_present &&
         (ObjectHasSubstring (sp->omtp, sp->aip, (Pointer) sfp, &(sp->osd))
          || StringMatchesConstraint (fcontext.label, &(sp->osd))))
      || (sp->fdp->act_when_string_not_present
          && ! ObjectHasSubstring (sp->omtp, sp->aip, (Pointer) sfp, &(sp->osd))
          && ! StringMatchesConstraint (fcontext.label, &(sp->osd))))
  {
    sp->fdp->callback (sfp, sp->fdp->userdata);  
  }
}

extern void OperateOnSeqEntryFeaturesWithText (SeqEntryPtr sep, FeaturesWithTextPtr fdp)
{
  SeqEntryItemsWithTextData sd;
  ObjMgrPtr                 omp;
  AsnExpOptPtr              aeop;
  
  if (sep == NULL || fdp == NULL) return;
  
  sd.fdp = fdp;
  sd.entityID = SeqMgrGetEntityIDForSeqEntry (sep);
  omp = ObjMgrGet ();
  if (omp == NULL) return;
  sd.omtp = ObjMgrTypeFind (omp, OBJ_SEQFEAT, NULL, NULL);
  if (sd.omtp == NULL) return;

  sd.aip = AsnIoNullOpen ();
  sd.osd.insensitive = fdp->case_insensitive;
  sd.osd.whole_word = fdp->whole_word;
  aeop = AsnExpOptNew (sd.aip, NULL, NULL, AsnWriteSearchCallBack);
  if (aeop != NULL) {
    aeop->user_data = (Pointer) &(sd.osd);
  }
  sd.osd.match = fdp->search_text;
  
  VisitFeaturesInSep (sep, &sd, SeqEntryFeaturesWithTextCallback);
  AsnIoClose (sd.aip);
}


static void SeqEntryDescriptorsWithTextCallback (SeqDescrPtr sdp, Pointer userdata)
{
  SeqEntryItemsWithTextPtr sp;
  
  if (sdp == NULL || userdata == NULL) return;
  sp = (SeqEntryItemsWithTextPtr) userdata;
  if (sp->ddp->no_text 
      || (! sp->ddp->act_when_string_not_present && ObjectHasSubstring (sp->omtp, sp->aip, (Pointer) sdp, &(sp->osd)))
      || (sp->ddp->act_when_string_not_present && ! ObjectHasSubstring (sp->omtp, sp->aip, (Pointer) sdp, &(sp->osd))))
  {
    sp->ddp->callback (sdp, sp->ddp->userdata);  
  }
}


extern void OperateOnSeqEntryDescriptorsWithText (SeqEntryPtr sep, DescriptorsWithTextPtr ddp)
{
  SeqEntryItemsWithTextData sd;
  ObjMgrPtr                 omp;
  AsnExpOptPtr              aeop;
  
  if (sep == NULL || ddp == NULL) return;
  
  sd.ddp = ddp;
  sd.entityID = SeqMgrGetEntityIDForSeqEntry (sep);
  omp = ObjMgrGet ();
  if (omp == NULL) return;
  sd.omtp = ObjMgrTypeFind (omp, OBJ_SEQDESC, NULL, NULL);
  if (sd.omtp == NULL) return;

  sd.aip = AsnIoNullOpen ();
  sd.osd.insensitive = ddp->case_insensitive;
  sd.osd.whole_word = ddp->whole_word;
  aeop = AsnExpOptNew (sd.aip, NULL, NULL, AsnWriteSearchCallBack);
  if (aeop != NULL) {
    aeop->user_data = (Pointer) &(sd.osd);
  }
  sd.osd.match = ddp->search_text;
  
  VisitDescriptorsInSep (sep, &sd, SeqEntryDescriptorsWithTextCallback);
  AsnIoClose (sd.aip);
}


static void GenePseudoOn (SeqFeatPtr sfp, Pointer userdata)

{
  GeneRefPtr  grp;

  if (sfp->data.choice != SEQFEAT_GENE) return;
  grp = (GeneRefPtr) sfp->data.value.ptrvalue;
  if (grp == NULL) return;
  grp->pseudo = TRUE;
}

static void GenePseudoOff (SeqFeatPtr sfp, Pointer userdata)

{
  GeneRefPtr  grp;

  if (sfp->data.choice != SEQFEAT_GENE) return;
  grp = (GeneRefPtr) sfp->data.value.ptrvalue;
  if (grp == NULL) return;
  grp->pseudo = FALSE;
}

typedef struct genepseudodata 
{
  FORM_MESSAGE_BLOCK
  Boolean set_on;
  TexT    search_text;  
  ButtoN  case_insensitive;
  ButtoN  when_string_not_present;
} GenePseudoData, PNTR GenePseudoPtr;

static void DoGenePseudoConstraint (ButtoN b)
{
  GenePseudoPtr        gsp;
  Char                 search_text[255];
  FeaturesWithTextData fd;
  SeqEntryPtr          sep;
  
  gsp = (GenePseudoPtr) GetObjectExtra (b);
  if (gsp == NULL) return;
  
  sep = GetTopSeqEntryForEntityID (gsp->input_entityID);
  if (sep == NULL) return;
  
  GetTitle (gsp->search_text, search_text, sizeof (search_text) - 1);
  
  /* set up text operation */
  fd.seqFeatChoice = SEQFEAT_GENE;
  fd.featDefChoice = 0;
  fd.search_text = search_text;
  fd.no_text = StringHasNoText (search_text);
  fd.case_insensitive = GetStatus (gsp->case_insensitive);
  fd.whole_word = FALSE;
  fd.act_when_string_not_present = GetStatus (gsp->when_string_not_present);
  fd.userdata = NULL;
  if (gsp->set_on)
  {
    fd.callback = GenePseudoOn;
  }
  else 
  {
  	fd.callback = GenePseudoOff;
  }
  
  OperateOnSeqEntryFeaturesWithText (sep, &fd);
  ObjMgrSetDirtyFlag (gsp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, gsp->input_entityID, 0, 0);  
  Update ();
  Remove (gsp->form);
}

static void GenePseudoConstraint (IteM i, Boolean set_on)
{
  BaseFormPtr        bfp;
  WindoW             w;
  GenePseudoPtr      gsp;
  GrouP              h, g, c;
  ButtoN             b;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  
  gsp = (GenePseudoPtr) MemNew (sizeof (GenePseudoData));
  if (gsp == NULL) return;

  gsp->input_entityID = bfp->input_entityID;
  gsp->set_on = set_on;
  if (gsp->set_on)
  {
    w = FixedWindow (-50, -33, -10, -10, "Set Gene Pseudo", StdCloseWindowProc);
  }
  else
  {
    w = FixedWindow (-50, -33, -10, -10, "Clear Gene Pseudo", StdCloseWindowProc);
  }
  SetObjectExtra (w, gsp, StdCleanupFormProc);
  gsp->form = (ForM) w;
  
  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 0, 4, NULL);
  StaticPrompt (g, "String Constraint", 0, 0, programFont, 'c');
  gsp->search_text = DialogText (g, "", 20, NULL);
  gsp->case_insensitive = CheckBox (g, "Case Insensitive", NULL);
  gsp->when_string_not_present = CheckBox (g, "When String Not Present", NULL);
  
  c = HiddenGroup (h, 4, 0, NULL);
  b = DefaultButton (c, "Accept", DoGenePseudoConstraint);
  SetObjectExtra (b, gsp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  RealizeWindow (w);
  Show (w);
  Select (gsp->search_text);
  Update ();
}

static void AddGenePseudoConstraint (IteM i)
{
  GenePseudoConstraint (i, TRUE);	
}

static void ClearGenePseudoConstraint (IteM i)
{
  GenePseudoConstraint (i, FALSE);	
}


static void AddGenePseudo (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  VisitFeaturesInSep (sep, NULL, GenePseudoOn);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void ClearGenePseudo (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  VisitFeaturesInSep (sep, NULL, GenePseudoOff);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static Boolean ClearOrfCallback (GatherContextPtr gcp)

{
  CdRegionPtr  crp;
  SeqFeatPtr   sfp;

  if (gcp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION) return TRUE;
  crp = (CdRegionPtr) sfp->data.value.ptrvalue;
  if (crp == NULL) return TRUE;
  crp->orf = FALSE;
  return TRUE;
}

static void ClearOrfFlagInCDS (IteM i)

{
  BaseFormPtr  bfp;
  GatherScope  gs;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherEntity (bfp->input_entityID, NULL, ClearOrfCallback, &gs);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static Boolean ClearTranslExceptCallback (GatherContextPtr gcp)

{
  CdRegionPtr  crp;
  SeqFeatPtr   sfp;

  if (gcp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION) return TRUE;
  crp = (CdRegionPtr) sfp->data.value.ptrvalue;
  if (crp == NULL) return TRUE;
  crp->code_break = CodeBreakFree (crp->code_break);
  return TRUE;
}

static void ClearTranslExceptInCDS (IteM i)

{
  BaseFormPtr  bfp;
  GatherScope  gs;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherEntity (bfp->input_entityID, NULL, ClearTranslExceptCallback, &gs);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static Boolean ConvertTranslExceptCallback (GatherContextPtr gcp)

{
  CdRegionPtr  crp;
  SeqFeatPtr   sfp;
  CharPtr      text = "RNA editing";

  if (gcp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION) return TRUE;
  crp = (CdRegionPtr) sfp->data.value.ptrvalue;
  if (crp == NULL || crp->code_break == NULL) return TRUE;
  crp->code_break = CodeBreakFree (crp->code_break);
  sfp->excpt = TRUE;
  if (sfp->except_text != NULL)
  {
  	MemFree (sfp->except_text);
  }
  sfp->except_text = (CharPtr) MemNew (sizeof (Char) * (StringLen (text) + 1));
  if (sfp->except_text != NULL)
  {
  	StringCpy (sfp->except_text, text);
  }
  return TRUE;
}

static void ConvertTranslExceptToRNAEditingException (IteM i)

{
  BaseFormPtr  bfp;
  GatherScope  gs;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherEntity (bfp->input_entityID, NULL, ConvertTranslExceptCallback, &gs);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);	
}

typedef struct bioseqsetform {
  FEATURE_FORM_BLOCK
  BioseqSetPtr       bssp;
  Uint2              entityID;
  PopuP              class_control;
} BioseqSetForm, PNTR BioseqSetFormPtr;

static ENUM_ALIST(bioseqset_class_alist)
  {"  ",                     0},
  {"Nuc-prot",               1},
  {"Segset",                 2},
  {"Conset",                 3},
  {"Parts",                  4},
  {"Gibb",                   5},
  {"GI",                     6},
  {"Genbank",                7},
  {"PIR",                    8},
  {"Pubset",                 9},
  {"Equiv",                 10},
  {"Swissprot",             11},
  {"PDB-entry",             12},
  {"Mut-set",               13},
  {"Pop-set",               14},
  {"Phy-set",               15},
  {"Eco-set",               16},
  {"Gen-prod-set",          17},
  {"WGS-set",               18},
  {"Other",                255},
END_ENUM_ALIST

static void AcceptBioseqSetEditProc (ButtoN b)

{
  BioseqSetFormPtr  bsfp;
  BioseqSetPtr      bssp;
  UIEnum            val;

  bsfp = (BioseqSetFormPtr) GetObjectExtra (b);
  if (bsfp != NULL) {
    Hide (bsfp->form);
    bssp = bsfp->bssp;
    if (bssp == NULL && bsfp->entityID == 0) {
      bssp = BioseqSetNew ();
    }
    if (bssp != NULL) {
      GetEnumPopup (bsfp->class_control, bioseqset_class_alist, &val);
      bssp->_class = (Uint1) val;
      if (bsfp->entityID == 0) {
        if (! ObjMgrRegister (OBJ_BIOSEQSET, (Pointer) bssp)) {
          Message (MSG_ERROR, "ObjMgrRegister failed");
        }
      }
    }
    Remove (bsfp->form);
    ObjMgrSetDirtyFlag (bsfp->entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, bsfp->entityID, 0, 0);
  }
}

static void CreateBioseqSetMessageProc (ForM f, Int2 mssg)

{
  StdEditorProcsPtr  sepp;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    if (sepp->handleMessages != NULL) {
      sepp->handleMessages (f, mssg);
    }
  }
}

static ForM CreateBioseqSetEditForm (BioseqSetPtr bssp, Uint2 entityID)

{
  ButtoN            b;
  BioseqSetFormPtr  bsfp;
  GrouP             c;
  GrouP             h;
  WindoW            w;

  bsfp = (BioseqSetFormPtr) MemNew (sizeof (BioseqSetForm));
  if (bsfp != NULL) {
    w = FixedWindow (-50, -33, -10, -10, "Bioseq Set", NULL);
    SetObjectExtra (w, bsfp, StdCleanupFormProc);
    bsfp->form = (ForM) w;
    bsfp->formmessage = CreateBioseqSetMessageProc;

    bsfp->bssp = bssp;
    bsfp->entityID = entityID;

    h = HiddenGroup (w, -2, 0, NULL);
    StaticPrompt (h, "Class", 0, popupMenuHeight, programFont, 'l');
    bsfp->class_control = PopupList (h, TRUE, NULL);
    SetObjectExtra (bsfp->class_control, bsfp, NULL);
    InitEnumPopup (bsfp->class_control, bioseqset_class_alist, NULL);
    if (bssp != NULL) {
      SetEnumPopup (bsfp->class_control, bioseqset_class_alist, (UIEnum) bssp->_class);
    }

    c = HiddenGroup (w, 2, 0, NULL);
    b = DefaultButton (c, "Accept", AcceptBioseqSetEditProc);
    SetObjectExtra (b, bsfp, NULL);
    PushButton (c, "Cancel", StdCancelButtonProc);
    AlignObjects (ALIGN_CENTER, (HANDLE) h, (HANDLE) c, NULL);
    RealizeWindow (w);
  }
  return (ForM) w;
}

extern Int2 LIBCALLBACK BioseqSetEditFunc (Pointer data)

{
  BioseqSetPtr      bssp;
  OMProcControlPtr  ompcp;
  ForM              w;

  ompcp = (OMProcControlPtr) data;
  bssp = NULL;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQSET :
      bssp = (BioseqSetPtr) ompcp->input_data;
      break;
   case 0 :
      break;
    default :
      return OM_MSG_RET_ERROR;
  }
  /* if (bssp == NULL) return OM_MSG_RET_ERROR; */

  w = CreateBioseqSetEditForm (bssp, ompcp->input_entityID);
  Show (w);
  Select (w);
  return OM_MSG_RET_DONE;
}

/*#ifdef EXTRA_SERVICES*/

static ValNodePtr UniqueAndCountValNodeCS (ValNodePtr list)

{
  Int4          count;
  ValNodePtr    curr;
  CharPtr       last;
  size_t        len;
  ValNodePtr    next;
  Pointer PNTR  prev;
  CharPtr       str;
  CharPtr       tmp;
  ValNodePtr    vnp;

  if (list == NULL) return NULL;
  last = (CharPtr) list->data.ptrvalue;
  vnp = list->next;
  if (vnp == NULL) return list;
  prev = (Pointer PNTR) &(list->next);
  count = 1;
  curr = list;
  while (vnp != NULL) {
    next = vnp->next;
    str = (CharPtr) vnp->data.ptrvalue;
    if (StringCmp (last, str) == 0) {
      vnp->next = NULL;
      *prev = next;
      ValNodeFreeData (vnp);
      count++;
    } else {
      len = StringLen (last) + 20;
      tmp = (CharPtr) MemNew (len);
      if (tmp != NULL) {
        sprintf (tmp, "%6ld   %s", (long) count, last);
        curr->data.ptrvalue = MemFree (curr->data.ptrvalue);
        curr->data.ptrvalue = tmp;
      }
      last = (CharPtr) vnp->data.ptrvalue;
      prev = (Pointer PNTR) &(vnp->next);
      count = 1;
      curr = vnp;
    }
    vnp = next;
  }
  len = StringLen (last) + 20;
  tmp = (CharPtr) MemNew (len);
  if (tmp != NULL) {
    sprintf (tmp, "%6ld   %s", (long) count, last);
    curr->data.ptrvalue = MemFree (curr->data.ptrvalue);
    curr->data.ptrvalue = tmp;
  }

  return list;
}

static int LIBCALLBACK SortVnpByStringCS (VoidPtr ptr1, VoidPtr ptr2)

{
  CharPtr     str1;
  CharPtr     str2;
  ValNodePtr  vnp1;
  ValNodePtr  vnp2;

  if (ptr1 != NULL && ptr2 != NULL) {
    vnp1 = *((ValNodePtr PNTR) ptr1);
    vnp2 = *((ValNodePtr PNTR) ptr2);
    if (vnp1 != NULL && vnp2 != NULL) {
      str1 = (CharPtr) vnp1->data.ptrvalue;
      str2 = (CharPtr) vnp2->data.ptrvalue;
      if (str1 != NULL && str2 != NULL) {
        return StringCmp (str1, str2);
      }
    }
  }
  return 0;
}

static int LIBCALLBACK SortVnpByChoiceAndStringCS (VoidPtr ptr1, VoidPtr ptr2)

{
  CharPtr     str1;
  CharPtr     str2;
  ValNodePtr  vnp1;
  ValNodePtr  vnp2;

  if (ptr1 != NULL && ptr2 != NULL) {
    vnp1 = *((ValNodePtr PNTR) ptr1);
    vnp2 = *((ValNodePtr PNTR) ptr2);
    if (vnp1 != NULL && vnp2 != NULL) {
      str1 = (CharPtr) vnp1->data.ptrvalue;
      str2 = (CharPtr) vnp2->data.ptrvalue;
      if (str1 != NULL && str2 != NULL) {
        if (vnp1->choice > vnp2->choice) {
          return 1;
        } else if (vnp1->choice < vnp2->choice) {
          return -1;
        }
        return StringCmp (str1, str2);
      }
    }
  }
  return 0;
}

static ValNodePtr SortFlatFile (ValNodePtr head, Boolean reverse, Boolean byblock)

{
  ValNodePtr  next;
  ValNodePtr  tail = NULL;
  ValNodePtr  vnp;

  if (head == NULL) return NULL;

  if (byblock) {
    head = ValNodeSort (head, SortVnpByChoiceAndStringCS);
  } else {
    head = ValNodeSort (head, SortVnpByStringCS);
  }
  if (reverse) {
    for (vnp = head; vnp != NULL; vnp = next) {
      next = vnp->next;
      vnp->next = tail;
      tail = vnp;
    }
    head = tail;
  } else {
    head = UniqueAndCountValNodeCS (head);
  }

  return head;
}

static void CaptureFFLine (CharPtr str, Pointer userdata, BlockType blocktype)

{
  Char             ch;
  CharPtr          copy;
  ValNodePtr PNTR  head;
  CharPtr          ptr;
  CharPtr          tmp;
  ValNodePtr       vnp;

  head = (ValNodePtr PNTR) userdata;
  copy = StringSaveNoNull (str);
  if (copy == NULL) return;

  ptr = copy;
  tmp = StringChr (ptr, '\n');
  while (tmp != NULL) {
    ch = *tmp;
    *tmp = '\0';
    vnp = ValNodeCopyStr (NULL, (Uint1) blocktype, ptr);
    if (*head == NULL) {
      *head = vnp;
    } else {
      vnp->next = *head;
      *head = vnp;
    }
    *tmp = ch;
    tmp++;
    ptr = tmp;
    tmp = StringChr (ptr, '\n');
  }

  MemFree (copy);
}

static void CaptureFFLineNoSequence (CharPtr str, Pointer userdata, BlockType blocktype)

{
  Char             ch;
  CharPtr          copy;
  ValNodePtr PNTR  head;
  CharPtr          ptr;
  CharPtr          tmp;
  ValNodePtr       vnp;

  head = (ValNodePtr PNTR) userdata;
  if (blocktype == SEQUENCE_BLOCK) return;
  copy = StringSaveNoNull (str);
  if (copy == NULL) return;

  ptr = copy;
  tmp = StringChr (ptr, '\n');
  while (tmp != NULL) {
    ch = *tmp;
    *tmp = '\0';
    vnp = ValNodeCopyStr (NULL, (Uint1) blocktype, ptr);
    if (*head == NULL) {
      *head = vnp;
    } else {
      vnp->next = *head;
      *head = vnp;
    }
    *tmp = ch;
    tmp++;
    ptr = tmp;
    tmp = StringChr (ptr, '\n');
  }

  MemFree (copy);
}

typedef struct resucoptions
{
  Boolean     reverse;
  Boolean     byblock;
  Boolean     showsequence;
  Uint2       entityID;
} ReSUCOptionsData, PNTR ReSUCOptionsPtr;

static Pointer ReSUCFree (Pointer userdata)
{
  return MemFree (userdata);  
}

static CharPtr ReSUCCallback (Pointer userdata)
{
  ReSUCOptionsPtr rp;
  SeqEntryPtr     sep;
  FILE            *fp;
  ValNodePtr      head = NULL;
  ErrSev          level;
  Boolean         okay;
  SeqEntryPtr     oldscope;
  Char            path [PATH_MAX];
  CharPtr         str;
  ValNodePtr      vnp;
  XtraBlock       xtra;
  
  if (userdata == NULL)
  {
    return NULL;
  }
  
  rp = (ReSUCOptionsPtr) userdata;
  
  sep = GetTopSeqEntryForEntityID (rp->entityID);
  if (sep == NULL) return NULL;
  MemSet ((Pointer) &xtra, 0, sizeof (XtraBlock));
  if (rp->showsequence)
  {
    xtra.ffwrite = CaptureFFLine;  	
  }
  else
  {
  	xtra.ffwrite = CaptureFFLineNoSequence;
  }
  xtra.userdata = (Pointer) &head;
  level = ErrSetMessageLevel (SEV_MAX);
  oldscope = SeqEntrySetScope (sep);
  okay = SeqEntryToGnbk (sep, NULL, GENBANK_FMT, SEQUIN_MODE, NORMAL_STYLE,
                         SHOW_CONTIG_FEATURES, 0, 0, &xtra, NULL);
  SeqEntrySetScope (oldscope);
  ErrSetMessageLevel (level);
  if (okay) {
    head = SortFlatFile (head, FALSE, rp->byblock);
    if (rp->reverse) {
      head = SortFlatFile (head, TRUE, FALSE);
    }

    TmpNam (path);
    fp = FileOpen (path, "w");
    if (fp != NULL) {
      for (vnp = head; vnp != NULL; vnp = vnp->next) {
        str = (CharPtr) vnp->data.ptrvalue;
        fprintf (fp, "%s\n", str);
      }
      FileClose (fp);
      return StringSave (path);
    }
  }
  return NULL;
}

static void SUCCommonProc (IteM i, Boolean reverse, Boolean byblock, 
                           Boolean showsequence, ButtoN b)

{
  BaseFormPtr  bfp;
  FILE         *fp;
  ValNodePtr   head = NULL;
  ErrSev       level;
  Boolean      okay;
  SeqEntryPtr  oldscope;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep;
  CharPtr      str;
  ValNodePtr   vnp;
  XtraBlock    xtra;
  ReSUCOptionsPtr rp;

  if (b != NULL) {
    bfp = GetObjectExtra (b);
  } else {
#ifdef WIN_MAC
    bfp = currentFormDataPtr;
#else
    bfp = GetObjectExtra (i);
#endif
  }
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  WatchCursor ();
  Update ();
  MemSet ((Pointer) &xtra, 0, sizeof (XtraBlock));
  if (showsequence)
  {
    xtra.ffwrite = CaptureFFLine;  	
  }
  else
  {
  	xtra.ffwrite = CaptureFFLineNoSequence;
  }
  xtra.userdata = (Pointer) &head;
  level = ErrSetMessageLevel (SEV_MAX);
  oldscope = SeqEntrySetScope (sep);
  okay = SeqEntryToGnbk (sep, NULL, GENBANK_FMT, SEQUIN_MODE, NORMAL_STYLE,
                         SHOW_CONTIG_FEATURES, 0, 0, &xtra, NULL);
  SeqEntrySetScope (oldscope);
  ErrSetMessageLevel (level);
  if (okay) {
    head = SortFlatFile (head, FALSE, byblock);
    if (reverse) {
      head = SortFlatFile (head, TRUE, FALSE);
    }

    TmpNam (path);
    fp = FileOpen (path, "w");
    if (fp != NULL) {
      for (vnp = head; vnp != NULL; vnp = vnp->next) {
        str = (CharPtr) vnp->data.ptrvalue;
        fprintf (fp, "%s\n", str);
      }
      FileClose (fp);
    }
    
    rp = (ReSUCOptionsPtr) MemNew (sizeof (ReSUCOptionsData));
    if (rp != NULL)
    {
      rp->reverse = reverse;
      rp->byblock = byblock;
      rp->showsequence = showsequence;
      rp->entityID = bfp->input_entityID;
      LaunchGeneralTextViewerWithRepopulate (path, "Sort Unique Count", 
                                             ReSUCCallback, rp, ReSUCFree);
    }
    else
    {
      LaunchGeneralTextViewer (path, "Sort Unique Count");
    }
    
    FileRemove (path);
    ValNodeFreeData (head);
  }
  ArrowCursor ();
  Update ();
}

static void SUCProc (IteM i)

{
  SUCCommonProc (i, FALSE, FALSE, TRUE, NULL);
}

static void SUCRProc (IteM i)

{
  SUCCommonProc (i, TRUE, FALSE, TRUE, NULL);
}

static void SUCBProc (IteM i)

{
  SUCCommonProc (i, FALSE, TRUE, TRUE, NULL);
}

static void SUCBNoSequenceProc (IteM i)

{
  SUCCommonProc (i, FALSE, TRUE, FALSE, NULL);
}

extern void SUCSubmitterProc (IteM i)

{
  SUCCommonProc (i, FALSE, TRUE, TRUE, NULL);
}

/*#endif*/

#if 0
#ifdef OS_UNIX
extern void SUCCommonProc (IteM i, Boolean reverse, ButtoN b);
extern void SUCCommonProc (IteM i, Boolean reverse, ButtoN b)

{
  BaseFormPtr  bfp;
  Char         cmmd [256];
  FILE         *fp;
  ErrSev       level;
  Boolean      okay;
  SeqEntryPtr  oldscope;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep;

  if (b != NULL) {
    bfp = GetObjectExtra (b);
  } else {
#ifdef WIN_MAC
    bfp = currentFormDataPtr;
#else
    bfp = GetObjectExtra (i);
#endif
  }
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp == NULL) return;
  WatchCursor ();
  Update ();
  level = ErrSetMessageLevel (SEV_MAX);
  oldscope = SeqEntrySetScope (sep);
  okay = SeqEntryToGnbk (sep, NULL, GENBANK_FMT, SEQUIN_MODE, NORMAL_STYLE, 0, 0, 0, NULL, fp);
  SeqEntrySetScope (oldscope);
  ErrSetMessageLevel (level);
  FileClose (fp);
  if (okay) {
    if (reverse) {
      sprintf (cmmd, "sort %s | uniq -c | sort -nr > %s.suc; rm %s", path, path, path);
    } else {
      sprintf (cmmd, "sort %s | uniq -c > %s.suc; rm %s", path, path, path);
    }
    system (cmmd); /* removes original flat file */
    StringCat (path, ".suc");
    LaunchGeneralTextViewer (path, "Sort Unique Count");
    FileRemove (path); /* removes sorted/uniqued/counted file */
  } else {
    FileRemove (path);
  }
  ArrowCursor ();
  Update ();
}

static void SUCProc (IteM i)

{
  SUCCommonProc (i, FALSE, NULL);
}

static void SUCRProc (IteM i)

{
  SUCCommonProc (i, TRUE, NULL);
}
#endif
#endif

/*#ifdef INTERNAL_NCBI_SEQUIN*/
/*#ifdef NEW_TAXON_SERVICE*/
/*
*  Get top 900 organisms, remove all ending with '.sp',
*  unknown, unidentified, uncultured, unclassified,
*  cloning vector, synthetic construct, mixed EST library,
*  trim to remaining 800 organisms, remove any counts
*
*  Process output in UNIX with
*    'sort -f +0 sorttax | uniq > taxlist.txt'
*  and
*    'sort -n +0 sortlin | uniq > lineages.txt'
*
*  Then add version number to top of each file
*/

static Boolean WriteTaxNode (CharPtr sci, CharPtr com, CharPtr syn,
                             Int4 gc, Int4 mgc, CharPtr div,
                             Int4 taxID, FILE *fout)

{
  Char  str [256];

  if (fout == NULL) return FALSE;
  if (sci != NULL && sci [0] != '\0') {
    if (div == NULL || div [0] == '\0') div = "???";
    if (com != NULL && com [0] != '\0') {
      sprintf (str, "%s\t%s\t%ld\t%ld\t%s\t%ld\n", sci, com,
               (long) gc, (long) mgc, div, (long) taxID);
      fprintf (fout, "%s", str);
    } else {
      sprintf (str, "%s\t\t%ld\t%ld\t%s\t%ld\n", sci,
               (long) gc, (long) mgc, div, (long) taxID);
      fprintf (fout, "%s", str);
    }
    return TRUE;
  }
  return FALSE;
}

static void WriteLineage (Int4 taxID, CharPtr lineage, FILE *fout)

{
  if (fout == NULL || lineage == NULL) return;
  fprintf (fout, "%ld\t%s\n", (long) taxID, lineage);
}

static void ProcessTaxNode (OrgRefPtr orp, FILE *fout1, FILE *fout2)

{
  Int4           gc;
  Int4           mgc;
  OrgModPtr      omp;
  OrgNamePtr     onp;
  Int4           taxID = -1;
  DbtagPtr       d;
  ValNodePtr     vnp;
  CharPtr        div = NULL;

  if (orp == NULL || fout1 == NULL || fout2 == NULL) return;
  
  for (vnp = orp->db; vnp != NULL; vnp = vnp->next) 
  {
    d = (DbtagPtr) vnp->data.ptrvalue;
    if (StringCmp(d->db, "taxon") == 0) 
    {
      taxID = d->tag->id;
      break;
    }
  }
  if (taxID > -1)
  {
    gc = 0;
    mgc = 0;
    onp = orp->orgname;
    if (onp != NULL) 
    {
      gc = onp->gcode;
      mgc = onp->mgcode;
      div = onp->div;
    }
    if (WriteTaxNode (orp->taxname, orp->common, NULL, gc, mgc, div, taxID, fout1))
    {
      if (onp != NULL) 
      {
        WriteLineage (taxID, onp->lineage, fout2);
        for (omp = onp->mod; omp != NULL; omp = omp->next) 
        {
          if (omp->subtype == ORGMOD_gb_anamorph ||
            omp->subtype == ORGMOD_gb_synonym) 
          {
            WriteTaxNode (omp->subname, orp->common, NULL, gc, mgc, div, taxID, fout1);
          }
        }
      }
    }
  }
}

static void PrepareTaxListProc (IteM i)

{
  Char     ch;
  FILE     *fin;
  FILE     *fout1;
  FILE     *fout2;
  Boolean  goOn;
  Char     orgname [256];
  CharPtr  ptr;
  ValNodePtr org_list = NULL;
  ValNodePtr response_list = NULL;
  ValNodePtr vnp;
  OrgRefPtr  orp;

  orgname [0] = '\0';
  goOn = TRUE;
  Message (MSG_POST, "Finding File");
  fin = FileOpen ("orglist", "r");
  if (fin != NULL) {
    fout1 = FileOpen ("sorttax", "w");
    if (fout1 != NULL) {
      fout2 = FileOpen ("sortlin", "w");
      if (fout2 != NULL) {
        Message (MSG_POST, "Processing");
        while (goOn) {
          goOn = (FileGets (orgname, sizeof (orgname), fin) != NULL);
          if (goOn) {
            ptr = orgname;
            ch = *ptr;
            while (ch != '\n' && ch != '\r' && ch != '\0') {
              ptr++;
              ch = *ptr;
            }
            *ptr = '\0';
            Message (MSG_POST, "Organism '%s'", orgname);
            ValNodeCopyStr (&org_list, 2, orgname);
          }
        }
        Message (MSG_POST, "Retrieving from server");
        response_list =  Taxon3GetOrgRefList (org_list);
        for (vnp = response_list; vnp != NULL; vnp = vnp->next)
        {
          orp = (OrgRefPtr) vnp->data.ptrvalue;
          ProcessTaxNode (orp, fout1, fout2);
          vnp->data.ptrvalue = OrgRefFree (orp);
        }
        response_list = ValNodeFree (response_list);
        org_list = ValNodeFreeData (org_list);

        FileClose (fout2);
        
      }
      FileClose (fout1);
    }
    FileClose (fin);
  } else {
    Message (MSG_OK, "Could not find orglist file");
  }
  
  Message (MSG_POST, "Finished");
}
/*#endif*/
/*#endif*/


#define REMOVE_PUB   1

static ENUM_ALIST(pub_field_alist)
  {" ",                    0},
  {"Remark",               1},
END_ENUM_ALIST

typedef struct pubformdata {
  FEATURE_FORM_BLOCK

  Int2           type;
  PopuP          fromfield;
  Int2           fromval;
  ButtoN         removeIncompletePubs;
} PubFormData, PNTR PubFormPtr;

static Boolean ProcessEachPubFunc (GatherContextPtr gcp)

{
  PubdescPtr  pdp;
  PubFormPtr  pfp;
  ValNodePtr  sdp;
  SeqFeatPtr  sfp;

  if (gcp == NULL) return TRUE;
  pfp = (PubFormPtr) gcp->userdata;
  if (pfp == NULL) return TRUE;
  pdp = NULL;
  if (gcp->thistype == OBJ_SEQDESC) {
    sdp = (ValNodePtr) gcp->thisitem;
    if (sdp != NULL && sdp->choice == Seq_descr_pub) {
      pdp = (PubdescPtr) sdp->data.ptrvalue;
    }
  } else if (gcp->thistype == OBJ_SEQFEAT) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp != NULL && sfp->data.choice == SEQFEAT_PUB) {
      pdp = (PubdescPtr) sfp->data.value.ptrvalue;
    }
  }
  if (pdp == NULL) return TRUE;
  switch (pfp->fromval) {
    case 1 :
      pdp->comment = MemFree (pdp->comment);
      break;
    default :
      break;
  }
  return TRUE;
}

/* PubdescIsIncomplete modified from ValidatePubdesc in valid.c */
static Boolean HasNoText (CharPtr str)

{
  Char  ch;

  if (str != NULL) {
    ch = *str;
    while (ch != '\0') {
      if (ch > ' ') {
        return FALSE;
      }
      str++;
      ch = *str;
    }
  }
  return TRUE;
}

static Boolean HasNoName (ValNodePtr name)

{
	AuthorPtr  ap;
	NameStdPtr  nsp;
	PersonIdPtr  pid;

	if (name != NULL) {
		ap = name->data.ptrvalue;
		if (ap != NULL) {
			pid = ap->name;
			if (pid != NULL) {
				if (pid->choice == 2) {
					nsp = pid->data;
					if (nsp != NULL) {
						if (! HasNoText (nsp->names [0])) {
							return FALSE;
						}
					}
				}
			}
		}
	}
	return TRUE;
}

static Boolean PubdescIsIncomplete (PubdescPtr pdp)

{
	AuthListPtr  alp;
	CitArtPtr  cap;
	Boolean  hasName, hasTitle;
	ValNodePtr  name;
	ValNodePtr  title;
	ValNodePtr  vnp;

	if (pdp == NULL) return TRUE;
	for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
		switch (vnp->choice) {
			case PUB_Article :
				cap = (CitArtPtr) vnp->data.ptrvalue;
				hasName = FALSE;
				hasTitle = FALSE;
				if (cap != NULL) {
					for (title = cap->title; title != NULL; title = title->next) {
						if (! HasNoText ((CharPtr) title->data.ptrvalue)) {
							hasTitle = TRUE;
						}
					}
					if (! hasTitle) {
						return TRUE;
					}
					alp = cap->authors;
					if (alp != NULL) {
						if (alp->choice == 1) {
							for (name = alp->names; name != NULL; name = name->next) {
								if (! HasNoName (name)) {
									hasName = TRUE;
								}
							}
						} else if (alp->choice == 2 || alp->choice == 3) {
							for (name = alp->names; name != NULL; name = name->next) {
								if (! HasNoText ((CharPtr) name->data.ptrvalue)) {
									hasName = TRUE;
								}
							}
						}
					}
					if (! hasName) {
						return TRUE;
					}
				}
				break;
			default :
				break;
		}
	}
	return FALSE;
}

static void RemoveIncompletePubs (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Boolean       empty;
  SeqAnnotPtr   nextsap;
  ValNodePtr    nextsdp;
  SeqFeatPtr    nextsfp;
  PubdescPtr    pdp;
  Pointer PNTR  prevsap;
  Pointer PNTR  prevsdp;
  Pointer PNTR  prevsfp;
  SeqAnnotPtr   sap;
  ValNodePtr    sdp;
  SeqFeatPtr    sfp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sdp = bsp->descr;
    prevsdp = (Pointer PNTR) &(bsp->descr);
    sap = bsp->annot;
    prevsap = (Pointer PNTR) &(bsp->annot);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sdp = bssp->descr;
    prevsdp = (Pointer PNTR) &(bssp->descr);
    sap = bssp->annot;
    prevsap = (Pointer PNTR) &(bssp->annot);
  } else return;
  while (sap != NULL) {
    nextsap = sap->next;
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      prevsfp = (Pointer PNTR) &(sap->data);
      while (sfp != NULL) {
        nextsfp = sfp->next;
        empty = FALSE;
        if (sfp->data.choice == SEQFEAT_PUB && sfp->data.value.ptrvalue != NULL) {
          pdp = (PubdescPtr) sfp->data.value.ptrvalue;
          empty = PubdescIsIncomplete (pdp);
        }
        if (empty) {
          *(prevsfp) = sfp->next;
          sfp->next = NULL;
          SeqFeatFree (sfp);
        } else {
          prevsfp = (Pointer PNTR) &(sfp->next);
        }
        sfp = nextsfp;
      }
    }
    if (sap->data == NULL) {
      *(prevsap) = sap->next;
      sap->next = NULL;
      SeqAnnotFree (sap);
    } else {
      prevsap = (Pointer PNTR) &(sap->next);
    }
    sap = nextsap;
  }
  while (sdp != NULL) {
    nextsdp = sdp->next;
    empty = FALSE;
    if (sdp->choice == Seq_descr_pub) {
      pdp = (PubdescPtr) sdp->data.ptrvalue;
      empty = PubdescIsIncomplete (pdp);
    }
    if (empty) {
      *(prevsdp) = sdp->next;
      sdp->next = NULL;
      SeqDescFree (sdp);
    } else {
      prevsdp = (Pointer PNTR) &(sdp->next);
    }
    sdp = nextsdp;
  }
}

static void DoProcessPub (ButtoN b)

{
  PubFormPtr   pfp;
  GatherScope  gs;
  SeqEntryPtr  sep;
  UIEnum       val;

  pfp = (PubFormPtr) GetObjectExtra (b);
  if (pfp == NULL || pfp->input_entityID == 0) return;
  Hide (pfp->form);
  WatchCursor ();
  Update ();
  pfp->fromval = 0;
  if (GetEnumPopup (pfp->fromfield, pub_field_alist, &val)) {
    pfp->fromval = (Int2) val;
  }
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.ignore[OBJ_SEQDESC] = FALSE;
  GatherEntity (pfp->input_entityID, (Pointer) pfp, ProcessEachPubFunc, &gs);
  if (GetStatus (pfp->removeIncompletePubs)) {
    sep = GetTopSeqEntryForEntityID (pfp->input_entityID);
    if (sep != NULL) {
      SeqEntryExplore (sep, NULL, RemoveIncompletePubs);
    }
  }
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (pfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, pfp->input_entityID, 0, 0);
  ObjMgrDeSelect (0, 0, 0, 0, NULL);
  Remove (pfp->form);
}

static void PubMessageProc (ForM f, Int2 mssg)

{
  PubFormPtr  pfp;

  pfp = (PubFormPtr) GetObjectExtra (f);
  if (pfp != NULL) {
    if (pfp->appmessage != NULL) {
      pfp->appmessage (f, mssg);
    }
  }
}

static void ProcessPub (IteM i, Int2 type)

{
  ButtoN             b;
  BaseFormPtr        bfp;
  GrouP              c;
  GrouP              g;
  GrouP              h;
  PubFormPtr         pfp;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  CharPtr            title;
  WindoW             w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  pfp = (PubFormPtr) MemNew (sizeof (PubFormData));
  if (pfp == NULL) return;
  pfp->type = type;
  switch (type) {
    case REMOVE_PUB :
      title = "Pub Removal";
      break;
    default :
      title = "?";
      break;
  }
  w = FixedWindow (-50, -33, -10, -10, title, StdCloseWindowProc);
  SetObjectExtra (w, pfp, StdCleanupFormProc);
  pfp->form = (ForM) w;
  pfp->formmessage = PubMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    pfp->appmessage = sepp->handleMessages;
  }

  pfp->input_entityID = bfp->input_entityID;
  pfp->input_itemID = bfp->input_itemID;
  pfp->input_itemtype = bfp->input_itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 4, 0, NULL);
  switch (type) {
    case REMOVE_PUB :
      StaticPrompt (g, "Remove", 0, popupMenuHeight, programFont, 'l');
      pfp->fromfield = PopupList (g, TRUE, NULL);
      SetObjectExtra (pfp->fromfield, pfp, NULL);
      InitEnumPopup (pfp->fromfield, pub_field_alist, NULL);
      SetEnumPopup (pfp->fromfield, pub_field_alist, 0);
      break;
    default :
      break;
  }

  pfp->removeIncompletePubs = NULL;
  if (type == REMOVE_PUB) {
    pfp->removeIncompletePubs = CheckBox (h, "Remove Incomplete Publications", NULL);
  }

  c = HiddenGroup (h, 4, 0, NULL);
  b = DefaultButton (c, "Accept", DoProcessPub);
  SetObjectExtra (b, pfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, (HANDLE) pfp->removeIncompletePubs, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}

static void RemovePub (IteM i)

{
  ProcessPub (i, REMOVE_PUB);
}

static void StripSerials (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  EntryStripSerialNumber (sep);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void RemoveLocalIDsProc (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  SeqIdPtr      nextsip;
  Pointer PNTR  prevsip;
  Boolean       replaced;
  SeqIdPtr      sip;

  if (sep == NULL) return;
  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;
  sip = bsp->id;
  prevsip = (Pointer PNTR) &(bsp->id);
  replaced = FALSE;
  while (sip != NULL) {
    nextsip = sip->next;
    if (sip->choice == SEQID_LOCAL) {
      (*prevsip) = sip->next;
      sip->next = NULL;
      SeqIdFree (sip);
      replaced = TRUE;
    } else {
      prevsip = (Pointer PNTR) &(sip->next);
    }
    sip = nextsip;
  }
  if (replaced) {
    SeqMgrReplaceInBioseqIndex (bsp);
  }
}

static void RemoveLocalIDs (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  SeqEntryExplore (sep, NULL, RemoveLocalIDsProc);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void RemoveLocusProc (BioseqPtr bsp, Pointer userdata)

{
  Boolean       reindex = FALSE;
  SeqIdPtr      sip;
  TextSeqIdPtr  tsip;

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    switch (sip->choice) {
      case SEQID_GENBANK :
      case SEQID_EMBL :
      case SEQID_OTHER :
      case SEQID_DDBJ :
      case SEQID_TPG :
      case SEQID_TPE :
      case SEQID_TPD :
        tsip = (TextSeqIdPtr) sip->data.ptrvalue;
        if (tsip != NULL) {
          tsip->name = MemFree (tsip->name);
          reindex = TRUE;
        }
        break;
      default :
        break;
    }
  }
  if (reindex) {
    SeqMgrReplaceInBioseqIndex (bsp);
  }
}

static void RemoveLocusFromParts (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  if (Message (MSG_OKC, "Are you SURE you want to do this?") == ANS_CANCEL) return;
  VisitSequencesInSep (sep, NULL, VISIT_PARTS, RemoveLocusProc);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void RemoveAssemblyProc (BioseqPtr bsp, Pointer userdata)

{
  SeqHistPtr  hist;

  hist = bsp->hist;
  if (hist == NULL) return;
  hist->assembly = SeqAlignFree (hist->assembly);
  if (hist->assembly != NULL || hist->replace_date != NULL ||
      hist->replace_ids != NULL || hist->replaced_by_date != NULL ||
      hist->replaced_by_ids != NULL || hist->deleted_date != NULL) return;
  bsp->hist = SeqHistFree (bsp->hist);
}

static void RemoveSeqHistAssembly (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  VisitBioseqsInSep (sep, NULL, RemoveAssemblyProc);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void LeaveFirstAssemblyProc (BioseqPtr bsp, Pointer userdata)

{
  SeqHistPtr   hist;
  SeqAlignPtr  next;
  SeqAlignPtr  sap;

  hist = bsp->hist;
  if (hist == NULL) return;
  sap = hist->assembly;
  if (sap == NULL) return;
  next = sap->next;
  if (next == NULL) return;
  sap->next = NULL;
  SeqAlignFree (next);
}

static void LeaveFirstSeqHistAssembly (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  VisitBioseqsInSep (sep, NULL, LeaveFirstAssemblyProc);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void FindTPAAccns (SeqDescrPtr sdp, Pointer userdata)

{
  UserFieldPtr     curr;
  ValNodePtr PNTR  head;
  ObjectIdPtr      oip;
  CharPtr          str;
  UserFieldPtr     ufp;
  UserObjectPtr    uop;
  ValNodePtr       vnp;

  if (sdp == NULL || sdp->choice != Seq_descr_user) return;
  uop = (UserObjectPtr) sdp->data.ptrvalue;
  if (uop == NULL) return;
  oip = uop->type;
  if (oip == NULL) return;
  if (StringCmp (oip->str, "TpaAssembly") != 0) return;
  head = (ValNodePtr PNTR) userdata;
  if (head == NULL) return;
  for (curr = uop->data; curr != NULL; curr = curr->next) {
    if (curr->choice != 11) continue;
    for (ufp = curr->data.ptrvalue; ufp != NULL; ufp = ufp->next) {
      if (ufp->choice != 1) continue;
      oip = ufp->label;
      if (oip == NULL || StringICmp (oip->str, "accession") != 0) continue;
      str = (CharPtr) ufp->data.ptrvalue;
      if (StringHasNoText (str)) continue;
      vnp = ValNodeCopyStr (NULL, 0, str);
      vnp->next = *head;
      *head = vnp;
    }
  }
}

typedef struct streamfsa {
  FILE  *fp;
  Char  buf [128];
  Int2  idx;
  Int2  maxlen;
} StreamFsa, PNTR StreamFsaPtr;

static void LIBCALLBACK FsaStreamProc (
  CharPtr sequence,
  Pointer userdata
)

{
  Char          ch;
  StreamFsaPtr  sfp;

  if (StringHasNoText (sequence) || userdata == NULL) return;
  sfp = (StreamFsaPtr) userdata;

  ch = *sequence;
  while (ch != '\0') {
    sfp->buf [sfp->idx] = ch;
    (sfp->idx)++;

    if (sfp->idx >= sfp->maxlen) {
      sfp->buf [sfp->idx] = '\0';
      fprintf (sfp->fp, "%s\n", sfp->buf);
      sfp->idx = 0;
    }

    sequence++;
    ch = *sequence;
  }
}

static void StreamFastaForBsp (BioseqPtr bsp, FILE *fp)

{
  Char       id [41];
  StreamFsa  sf;

  if (bsp == NULL || fp == NULL) return;

  id [0] = '\0';
  SeqIdWrite (bsp->id, id, PRINTID_FASTA_LONG, sizeof (id) - 1);
  fprintf (fp, ">%s\n", id);

  MemSet ((Pointer) &sf, 0, sizeof (StreamFsa));
  sf.fp = fp;
  sf.idx = 0;
  sf.maxlen = 60;
  SeqPortStream (bsp, STREAM_EXPAND_GAPS | STREAM_CORRECT_INVAL, (Pointer) &sf, FsaStreamProc);

  if (sf.idx > 0) {
    sf.buf [sf.idx] = '\0';
    fprintf (fp, "%s\n", sf.buf);
  }
}

static void CreateTPABlastDB (IteM i)

{
  BaseFormPtr  bfp;
  BioseqPtr    bsp;
  FILE         *fp;
  ValNodePtr   head = NULL;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep;
  SeqIdPtr     sip;
  CharPtr      str;
  ValNodePtr   vnp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  VisitDescriptorsInSep (sep, (Pointer) &head, FindTPAAccns);
  if (head == NULL) return;
  head = ValNodeSort (head, SortVnpByString);
  head = UniqueValNode (head);
  if (! GetOutputFileName (path, sizeof (path), NULL)) return;
  fp = FileOpen (path, "w");
  if (fp == NULL) return;
  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    if (StringHasNoText (str)) continue;
    Message (MSG_POST, "Making DB for %s", str);
    sip = SeqIdFromAccessionDotVersion (str);
    bsp = BioseqLockById (sip);
    StreamFastaForBsp (bsp, fp);
    BioseqUnlock (bsp);
  }
  FileClose (fp);
  ValNodeFreeData (head);
}

static void FindBioseqForHist (SeqIdPtr sip, Pointer userdata)

{
  BioseqPtr       bsp;
  BioseqPtr PNTR  bspp;

  bsp = BioseqFindCore (sip);
  if (bsp == NULL) return;
  bspp = (BioseqPtr PNTR) userdata;
  *bspp = bsp;
}

static void AttachSapToHistory (SeqAlignPtr sap)

{
  BioseqPtr   bsp = NULL;
  SeqHistPtr  shp;

  VisitSeqIdsInSeqAlign (sap, (Pointer) &bsp, FindBioseqForHist);
  if (bsp == NULL) return;
  shp = SeqHistNew ();
  shp->assembly = sap;
  bsp->hist = shp;
}

static void ReadSeqHistAssembly (IteM i)

{
  BaseFormPtr  bfp;
  Pointer      dataptr;
  Uint2        datatype;
  FILE         *fp;
  Char         path [PATH_MAX];
  SeqAlignPtr  salp;
  SeqAnnotPtr  sap;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  if (! GetInputFileName (path, sizeof (path), "", "TEXT")) return;
  fp = FileOpen (path, "r");
  if (fp == NULL) return;

  while ((dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, FALSE, TRUE, FALSE)) != NULL) {
    if (datatype == OBJ_SEQANNOT) {
      sap = (SeqAnnotPtr) dataptr;
      if (sap != NULL && sap->type == 2) {
        salp = (SeqAlignPtr) sap->data;
        sap->data = NULL;
        AttachSapToHistory (salp);
        SeqAnnotFree (sap);
      }
    }
  }

  FileClose (fp);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

typedef struct recondata {
  BioseqPtr   prod;
  SeqFeatPtr  cds;
  SeqFeatPtr  prt;
  Boolean     notunique;
} ReconData, PNTR ReconDataPtr;

static Boolean GetReconFunc (GatherContextPtr gcp)

{
  BioseqPtr     bsp;
  ReconDataPtr  rdp;
  SeqFeatPtr    sfp;

  if (gcp == NULL || gcp->thisitem == NULL) return TRUE;
  rdp = (ReconDataPtr) gcp->userdata;
  if (rdp == NULL) return TRUE;
  switch (gcp->thistype) {
    case OBJ_SEQFEAT :
      sfp = (SeqFeatPtr) gcp->thisitem;
      if (sfp->data.choice == SEQFEAT_CDREGION) {
        if (rdp->cds != NULL) {
          rdp->notunique = TRUE;
        } else {
          rdp->cds = sfp;
        }
      } else if (sfp->data.choice == SEQFEAT_PROT) {
        if (rdp->prt == NULL) {
          rdp->prt = sfp; /* gets first protein, not largest, since location wrong */
        }
      }
      break;
    case OBJ_BIOSEQ :
      bsp = (BioseqPtr) gcp->thisitem;
      if (ISA_aa (bsp->mol)) {
        if (rdp->prod != NULL) {
          rdp->notunique = TRUE;
        } else {
          rdp->prod = bsp;
        }
      }
      break;
    default :
      break;
  }
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  return TRUE;
}

static void ReconnectCDSProc (Uint2 entityID, SeqEntryPtr sep)

{
  BioseqSetPtr  bssp;
  GatherScope   gs;
  MolInfoPtr    mip;
  Boolean       partial5;
  Boolean       partial3;
  ReconData     rd;
  SeqLocPtr     slp;
  ValNodePtr    vnp;

  if (sep == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (IsPopPhyEtcSet (bssp->_class)))) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        ReconnectCDSProc (entityID, sep);
      }
      return;
    }
  }

  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.scope = sep;
  rd.prod = NULL;
  rd.cds = NULL;
  rd.prt = NULL;
  rd.notunique = FALSE;
  partial5 = FALSE;
  partial3 = FALSE;
  GatherEntity (entityID, (Pointer) (&rd), GetReconFunc, &gs);
  if (rd.notunique) return;
  if (rd.prod != NULL && rd.cds != NULL) {
    slp = SeqLocFindNext (rd.cds->location, NULL);
    if (slp != NULL) {
      CheckSeqLocForPartial (slp, &partial5, &partial3);
    }
    sep = SeqMgrGetSeqEntryForData (rd.prod);
    if (sep != NULL) {
      SetSeqFeatProduct (rd.cds, rd.prod);
      if (rd.prt != NULL) {
        rd.prt->location = SeqLocFree (rd.prt->location);
        rd.prt->location = CreateWholeInterval (sep);
        SetSeqLocPartial (rd.prt->location, partial5, partial3);
        rd.prt->partial = (partial5 || partial3);
        vnp = SeqEntryGetSeqDescr (sep, Seq_descr_molinfo, NULL);
        if (vnp == NULL) {
          vnp = CreateNewDescriptor (sep, Seq_descr_molinfo);
          if (vnp != NULL) {
            mip = MolInfoNew ();
            vnp->data.ptrvalue = (Pointer) mip;
            if (mip != NULL) {
              mip->biomol = 8;
              mip->tech = 13;
            }
          }
        }
        if (vnp != NULL) {
          mip = (MolInfoPtr) vnp->data.ptrvalue;
          if (mip != NULL) {
            if (partial5 && partial3) {
              mip->completeness = 5;
            } else if (partial5) {
              mip->completeness = 3;
            } else if (partial3) {
              mip->completeness = 4;
            /*
            } else if (partial) {
              mip->completeness = 2;
            */
            } else {
              mip->completeness = 0;
            }
          }
        }
      }
    }
  }
}

static void ReconnectCDSProduct (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  ReconnectCDSProc (bfp->input_entityID, sep);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void DoAddDeltaGap (BioseqPtr bsp, Pointer userdata)

{
  DeltaSeqPtr        del, dsp, next;
  SeqMgrDescContext  dcontext;
  Int4               len = 0;
  Int4Ptr            lenp;
  SeqLitPtr          litp;
  MolInfoPtr         mip;
  SeqDescrPtr        sdp;

  if (bsp == NULL || bsp->repr != Seq_repr_delta || bsp->seq_ext_type != 4) return;
  lenp = (Int4Ptr) userdata;
  if (lenp != NULL) {
    len = *lenp;
  }

  /* if has molinfo, but tech is not htgs1 or htgs2, bail */

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
  if (sdp != NULL) {
    mip = (MolInfoPtr) sdp->data.ptrvalue;
    if (mip != NULL) {
      if (mip->tech != MI_TECH_htgs_1 && mip->tech != MI_TECH_htgs_2) return;
    }
  }

  for (dsp = (DeltaSeqPtr) bsp->seq_ext; dsp != NULL && dsp->next != NULL; dsp = next) {
    next = dsp->next;
    if (dsp->choice != 2) continue;
    litp = (SeqLitPtr) dsp->data.ptrvalue;
    if (litp == NULL) continue;
    if (litp->seq_data == NULL || litp->length == 0) continue;

    if (next->choice == 2) {
      litp = (SeqLitPtr) next->data.ptrvalue;
      if (litp != NULL) {
        if (litp->seq_data == NULL && litp->length > 0) continue;
      }
    }

    del = ValNodeNew (NULL);
    if (del == NULL) continue;
    litp = SeqLitNew ();
    if (litp == NULL) continue;
    litp->length = len;
    del->choice = 2;
    del->data.ptrvalue = litp;

    del->next = next;
    dsp->next = del;

    bsp->length += len;
  }
}

static void AddDelta100Gaps (IteM i)

{
  BaseFormPtr  bfp;
  Int4         len = 100;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  VisitBioseqsInSep (sep, (Pointer) &len, DoAddDeltaGap);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void AddDelta0Gaps (IteM i)

{
  BaseFormPtr  bfp;
  Int4         len = 0;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  VisitBioseqsInSep (sep, (Pointer) &len, DoAddDeltaGap);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void DoDeleteDeltaEndGap (BioseqPtr bsp, Pointer userdata)

{
  DeltaSeqPtr  dsp, last, prev;
  SeqLitPtr    litp, pitp;

  if (bsp == NULL || bsp->repr != Seq_repr_delta || bsp->seq_ext_type != 4) return;

  dsp = (DeltaSeqPtr) bsp->seq_ext;
  if (dsp == NULL) return;
  if (dsp->choice == 2) {
    litp = (SeqLitPtr) dsp->data.ptrvalue;
    if (litp != NULL) {
      if (litp->seq_data == NULL) {
        bsp->length -= litp->length;
        bsp->seq_ext = dsp->next;
        SeqLitFree (litp);
        MemFree (dsp);
      }
    }
  }

  prev = (DeltaSeqPtr) bsp->seq_ext;
  if (prev == NULL) return;
  last = prev->next;
  if (last == NULL) return;
  while (last->next != NULL) {
    prev = last;
    last = last->next;
  }

  if (last->choice == 2) {
    litp = (SeqLitPtr) last->data.ptrvalue;
    if (litp != NULL) {
      if (litp->seq_data == NULL) {
        bsp->length -= litp->length;
        prev->next = NULL;
        SeqLitFree (litp);
        MemFree (last);
      }
    }
  }

  prev = (DeltaSeqPtr) bsp->seq_ext;
  if (prev == NULL) return;
  last = prev->next;
  if (last == NULL) return;
  while (last->next != NULL) {
    if (prev->choice == 2 && last->choice == 2) {
      pitp = (SeqLitPtr) prev->data.ptrvalue;
      litp = (SeqLitPtr) last->data.ptrvalue;
      if (pitp != NULL && litp != NULL) {
        if (pitp->seq_data == NULL && litp->seq_data == NULL) {
          pitp->length += litp->length;
          prev->next = last->next;
          last->next = NULL;
          SeqLitFree (litp);
          MemFree (last);
          last = prev;
        }
      }
    }
    prev = last;
    last = last->next;
  }
}

static void DeleteDeltaEndGaps (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  VisitBioseqsInSep (sep, NULL, DoDeleteDeltaEndGap);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void AttcBioSourceStrainToXref (BioSourcePtr biop)

{
  Char         ch;
  DbtagPtr     dbt;
  ObjectIdPtr  oip;
  OrgModPtr    omp;
  OrgNamePtr   onp;
  OrgRefPtr    orp;
  CharPtr      str;
  ValNodePtr   vnp;

  if (biop == NULL) return;
  orp = biop->org;
  if (orp == NULL) return;
  onp = orp->orgname;
  if (onp == NULL) return;
  for (omp = onp->mod; omp != NULL; omp = omp->next) {
    if (omp->subtype == ORGMOD_strain) {
      if (StringNICmp (omp->subname, "ATCC", 4) == 0) {
        str = omp->subname + 4;
        ch = *str;
        while (IS_WHITESP (ch) || ch == ':') {
          str++;
          ch = *str;
        }
        if (! StringHasNoText (str)) {
          for (vnp = orp->db; vnp != NULL; vnp = vnp->next) {
            dbt = (DbtagPtr) vnp->data.ptrvalue;
            if (dbt != NULL && StringICmp (dbt->db, "ATCC") == 0) {
              oip = dbt->tag;
              if (oip != NULL && oip->str != NULL) {
                if (StringICmp (oip->str, str) == 0) return;
              }
            }
          }
          dbt = DbtagNew ();
          if (dbt == NULL) return;
          dbt->db = StringSave ("ATCC");
          oip = ObjectIdNew ();
          if (oip == NULL) return;
          oip->str = StringSave (str);
          dbt->tag = oip;
          ValNodeAddPointer (&(orp->db), 0, (Pointer) dbt);
        }
      }
    }
  }
}

static Boolean LIBCALLBACK AttcBioSourceDesc (ValNodePtr sdp, SeqMgrDescContextPtr context)

{
  if (sdp->choice != Seq_descr_source) return TRUE;
  AttcBioSourceStrainToXref ((BioSourcePtr) sdp->data.ptrvalue);
  return TRUE;
}

static Boolean LIBCALLBACK AttcBioSourceFeat (SeqFeatPtr sfp, SeqMgrFeatContextPtr context)

{
  if (sfp->data.choice != SEQFEAT_BIOSRC) return TRUE;
  AttcBioSourceStrainToXref ((BioSourcePtr) sfp->data.value.ptrvalue);
  return TRUE;
}

static void AtccStrainToXref (IteM i)

{
  BaseFormPtr  bfp;
  Boolean      bioDescFilter [SEQDESCR_MAX];
  Boolean      bioFeatFilter [SEQFEAT_MAX];
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  MemSet ((Pointer) bioDescFilter, 0, sizeof (bioDescFilter));
  MemSet ((Pointer) bioFeatFilter, 0, sizeof (bioFeatFilter));
  bioDescFilter [Seq_descr_source] = TRUE;
  bioFeatFilter [SEQFEAT_BIOSRC] = TRUE;
  SeqMgrVisitDescriptors (bfp->input_entityID, NULL, AttcBioSourceDesc, bioDescFilter);
  SeqMgrVisitFeatures (bfp->input_entityID, NULL, AttcBioSourceFeat, bioFeatFilter, NULL);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}


typedef struct extendgeneformglobalitemdata {
  Int4		iFeatDef;
  Boolean	item_value;
  Boolean	upstream;
  CharPtr	label;
} ExtendGeneFormGlobalItemData, PNTR ExtendGeneFormGlobalItemPtr;

typedef struct extendgeneformlocalitemdata {
  ButtoN	item_button;
  Boolean	item_value;
  SeqFeatPtr	plus_sfp;
  SeqFeatPtr	minus_sfp;
} ExtendGeneFormLocalItemData, PNTR ExtendGeneFormLocalItemPtr;

static ExtendGeneFormGlobalItemData list_of_regulatory_items[] = {
	{ FEATDEF_attenuator, FALSE, FALSE, "attenuator" },
	{ FEATDEF_CAAT_signal, FALSE, TRUE, "CAAT signal" },
	{ FEATDEF_enhancer, FALSE, TRUE, "enhancer" },
	{ FEATDEF_GC_signal, FALSE, TRUE, "GC signal" },
	{ FEATDEF_misc_binding, FALSE, TRUE, "misc binding" },
	{ FEATDEF_misc_feature, FALSE, TRUE, "misc feature" },
	{ FEATDEF_misc_signal, FALSE, TRUE, "misc signal" },
	{ FEATDEF_misc_structure, FALSE, TRUE, "misc structure" },
	{ FEATDEF_promoter, FALSE, TRUE, "promoter" },
	{ FEATDEF_protein_bind, FALSE, TRUE, "protein bind" },
	{ FEATDEF_RBS, TRUE, TRUE, "RBS" },
	{ FEATDEF_stem_loop, FALSE, TRUE, "stem loop" },
	{ FEATDEF_TATA_signal, FALSE, TRUE, "TATA signal" },
	{ FEATDEF_terminator, FALSE, FALSE, "terminator" },
	{ FEATDEF_5UTR, TRUE, TRUE, "5'UTR" },
	{ FEATDEF_3UTR, TRUE, FALSE, "3'UTR" },
	{ FEATDEF_10_signal, FALSE, TRUE, "-10 signal" },
	{ FEATDEF_35_signal, FALSE, TRUE, "-35 signal" },
};

const Int4 NUM_REGULATORY_ITEMS =
	sizeof(list_of_regulatory_items) / sizeof(ExtendGeneFormGlobalItemData); 

typedef struct extendgeneformdata {
  FEATURE_FORM_BLOCK

  ButtoN	PlusStrand;
  ButtoN	MinusStrand;
  Boolean	doPlusStrand;
  Boolean	doMinusStrand;
  ButtoN	ResetGenes;
  ButtoN	StealFeatures;
  Boolean	doResetGenes;
  Boolean	doStealFeatures;
  ButtoN	LogEvents;
  ButtoN	LogUnextendedEvents;
  Boolean	doLogEvents;
  Boolean	doLogUnextendedEvents;
  ExtendGeneFormLocalItemPtr	item_list;
  FILE		*fp;
} ExtendGeneFormData, PNTR ExtendGeneFormPtr;

static CharPtr GetGeneName(GeneRefPtr grp)
{
  if (!HasNoText (grp->locus)) return grp->locus;
  if (!HasNoText (grp->locus_tag)) return grp->locus_tag;
  if (!HasNoText (grp->desc)) return grp->desc;
  return NULL;
}

static void LogLocation (SeqMgrFeatContextPtr context, FILE *fp)
{
  if (context->strand == Seq_strand_minus)
  {
    fprintf (fp, "at complement(%d..%d)", context->left, context->right);
  }
  else
  {
    fprintf (fp, "at %d..%d", context->left, context->right);
  }
}

typedef struct correctgeneformdata {
  FEATURE_FORM_BLOCK

  ButtoN      setIntervalBtn;
  ButtoN      setStrandBtn;
  ButtoN      logChangesBtn;
  ButtoN      correct_selected_pair;
  ButtoN      leave_dlg_up;
  Boolean     setInterval;
  Boolean     setStrand;
  SeqEntryPtr sep;
  FILE *      fp;
} CorrectGeneFormData, PNTR CorrectGeneFormPtr;

typedef struct genesforcdsdata {
  SeqFeatPtr     cds;
  ValNodePtr     gene_list;
  Boolean        same_strand_only;
} GenesForCDSData, PNTR GenesForCDSPtr;

static void FindGenesForCDS (SeqFeatPtr sfp, Pointer userdata)
{
  GenesForCDSPtr gfc;
  ValNodePtr     vnp;
  Uint1          strand_cds, strand_gene;
  
  gfc = (GenesForCDSPtr) userdata;

  if (gfc == NULL || sfp == NULL || sfp->idx.subtype != FEATDEF_GENE) 
  {
    return;
  }

  /* unless we plan to correct to same strand, only look at locations on same strand */
  if (gfc->same_strand_only)
  {
    strand_cds = SeqLocStrand (gfc->cds->location);
    strand_gene = SeqLocStrand (sfp->location);
    if (strand_cds == Seq_strand_minus && strand_gene != Seq_strand_minus)
    {
      return;
    }
    else if (strand_cds != Seq_strand_minus && strand_gene == Seq_strand_minus)
    {
      return;
    }
  }
  if (SeqLocAinB (gfc->cds->location, sfp->location) > -1
    || SeqLocAinB (sfp->location, gfc->cds->location) > -1)
  {
    vnp = ValNodeNew (gfc->gene_list);
    if (vnp == NULL) return;
    vnp->data.ptrvalue = sfp;
    if (gfc->gene_list == NULL)
    {
      gfc->gene_list = vnp;
    }
  }
}

static void SetSeqLocStrand (SeqLocPtr location, Uint2 strand)
{
  SeqLocPtr slp;
  SeqIntPtr sip;
  SeqPntPtr spp;

  slp = SeqLocFindNext (location, NULL);
  while (slp != NULL)
  {
    if (slp == NULL || slp->data.ptrvalue == NULL) 
    {
      slp = SeqLocFindNext (location, slp);
      continue;
    }
    if (slp->choice == SEQLOC_INT)
    {
      sip = (SeqIntPtr)slp->data.ptrvalue;
      sip->strand = strand;
    }
    else if (slp->choice == SEQLOC_PNT)
    {
      spp = (SeqPntPtr)slp->data.ptrvalue;
      spp->strand = strand;
    }
    slp = SeqLocFindNext (location, slp);
  }
}

static void 
CorrectOneCDSGenePair 
(SeqFeatPtr cds, SeqFeatPtr gene, BioseqPtr bsp, CorrectGeneFormPtr cgp)
{
  Boolean   need_change;
  SeqLocPtr slp;
  Boolean   oldpartial5, oldpartial3, partial5, partial3;
  CharPtr   cds_label;
  CharPtr            genename = NULL;
  SeqMgrFeatContext  fcontext;
  Uint1              strand_cds, strand_gene;
  
  if (cds == NULL || gene == NULL || cgp == NULL)
  {
    return;
  }
  
  if (bsp == NULL)
  {
    bsp = BioseqFindFromSeqLoc (cds->location);
    if (bsp == NULL)
    {
      return;
    }
  }
  
  if (cgp->setInterval)
  {
    need_change = FALSE;
    slp = SeqLocMerge (bsp, cds->location, NULL, TRUE, TRUE, FALSE);
    if (SeqLocCompare (slp, gene->location) != SLC_A_EQ_B)
    {
      need_change = TRUE;
    }
    CheckSeqLocForPartial (gene->location, &oldpartial5, &oldpartial3);
    CheckSeqLocForPartial (cds->location, &partial5, &partial3);
    if (oldpartial5 != partial5 || oldpartial3 != partial3)
    {
      need_change = TRUE;
    }
    if (need_change)
    {
      SetSeqLocPartial (slp, partial5, partial3);
      SeqLocFree (gene->location);
      gene->location = slp;
      if (cgp->fp != NULL)
      {
        genename = GetGeneName ((GeneRefPtr) gene->data.value.ptrvalue);
        cds = SeqMgrGetDesiredFeature (cds->idx.entityID, NULL, 0, 0, cds, &fcontext);
        if (genename == NULL)
        {
          fprintf (cgp->fp, "Unnamed gene ");
          LogLocation (&fcontext, cgp->fp);
        }
        else
        {
          fprintf (cgp->fp, "Gene %s", genename);
        }
        cds_label = StringSave (fcontext.label);
        if (StringLen (cds_label) > 20)
        {
          cds_label[20] = 0;
        }
        fprintf (cgp->fp, " reset to CDS interval (%s)\n", cds_label);
        cds_label = MemFree (cds_label);
      }
    }
    else
    {
      SeqLocFree (slp);      
    }              
  }
  if (cgp->setStrand)
  {
    need_change = FALSE;
    strand_cds = SeqLocStrand (cds->location);
    strand_gene = SeqLocStrand (gene->location);
    if (strand_cds == Seq_strand_minus && strand_gene != Seq_strand_minus)
    {
      need_change = TRUE;
    }
    else if (strand_cds != Seq_strand_minus && strand_gene == Seq_strand_minus)
    {
      need_change = TRUE;
    }
    if (need_change)
    {
      SetSeqLocStrand (gene->location, strand_cds);
      if (cgp->fp != NULL)
      {
        cds = SeqMgrGetDesiredFeature (cds->idx.entityID, NULL, 0, 0, cds, &fcontext);
        genename = GetGeneName ((GeneRefPtr) gene->data.value.ptrvalue);
        if (genename == NULL)
        {
          fprintf (cgp->fp, "Unnamed gene ");
          LogLocation (&fcontext, cgp->fp);
        }
        else
        {
          fprintf (cgp->fp, "Gene %s", genename);
        }
        cds_label = StringSave (fcontext.label);
        if (StringLen (cds_label) > 20)
        {
          cds_label[20] = 0;
        }
        fprintf (cgp->fp, " strand set to match CDS (%s)\n", cds_label);
        cds_label = MemFree (cds_label);
      }
    }
  }
}
  
static void CorrectOneCDSGene (SeqFeatPtr sfp, Pointer userdata)
{
  CorrectGeneFormPtr cgp;
  GenesForCDSData    gfc;
  SeqFeatPtr         gene;
  BioseqPtr          bsp;
  SeqFeatXrefPtr     gene_xref;
  GeneRefPtr         grp;
  SeqMgrFeatContext  fcontext;
  CharPtr            cds_label;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION
      || (cgp = (CorrectGeneFormPtr)userdata) == NULL)
  {
    return;
  }

  gfc.cds = sfp;
  gfc.gene_list = NULL;
  gfc.same_strand_only = ! cgp->setStrand;

  bsp = BioseqFindFromSeqLoc (sfp->location);

  gene_xref = sfp->xref;
  while (gene_xref != NULL)
  {
    if (gene_xref->data.choice == SEQFEAT_GENE && gene_xref->data.value.ptrvalue != NULL)
    {
      grp = (GeneRefPtr) gene_xref->data.value.ptrvalue;
      if (!StringHasNoText (grp->locus))
      {
        gene = SeqMgrGetFeatureByLabel (bsp, grp->locus, SEQFEAT_GENE, 0, &fcontext);
        if (gene != NULL)
        {
          ValNodeAddPointer (&(gfc.gene_list), 0, gene);
        }
      }
    }
    gene_xref = gene_xref->next;
  }

  if (gfc.gene_list == NULL)
  {
    VisitFeaturesInSep (cgp->sep, &gfc, FindGenesForCDS);
  }

  if (gfc.gene_list == NULL || gfc.gene_list->data.ptrvalue == NULL
      || gfc.gene_list->next != NULL)
  {
    sfp = SeqMgrGetDesiredFeature (sfp->idx.entityID, NULL, 0, 0, sfp, &fcontext);
    cds_label = StringSave (fcontext.label);
    if (StringLen (cds_label) > 20)
    {
      cds_label[20] = 0;
    }
    if (cgp->fp != NULL)
    {
      if (gfc.gene_list == NULL || gfc.gene_list->data.ptrvalue == NULL)
      {
        fprintf (cgp->fp, "No gene found for CDS (%s)\n", cds_label);
      }
      else
      {
        fprintf (cgp->fp, "Found more than one gene for CDS (%s)\n", cds_label);
      }
    }
    cds_label = MemFree (cds_label);
  }
  else
  {
    gene = gfc.gene_list->data.ptrvalue;
    CorrectOneCDSGenePair (sfp, gene, bsp, cgp);
  }

  ValNodeFree (gfc.gene_list);
}

static void CorrectGenesForCDSs (ButtoN b)
{
  CorrectGeneFormPtr cgp;
  Char			         path [PATH_MAX];
  SelStructPtr       ssp;
  SeqFeatPtr         gene = NULL, cds = NULL, sfp;
  SeqMgrFeatContext  fcontext;
  Boolean            cancel = FALSE;
  
  cgp = GetObjectExtra (b);
  if (cgp == NULL) return;

  Hide (cgp->form);
  WatchCursor ();
  Update ();

  cgp->setStrand = GetStatus (cgp->setStrandBtn);
  cgp->setInterval = GetStatus (cgp->setIntervalBtn);
  cgp->sep = GetTopSeqEntryForEntityID (cgp->input_entityID);

  /* set up tmp file if a log has been requested */  
  if (GetStatus (cgp->logChangesBtn))
  {
    TmpNam (path);
    cgp->fp = FileOpen (path, "wb");
  }
  else
  {
    cgp->fp = NULL;
  }
  
  if (GetStatus (cgp->correct_selected_pair))
  {
    ssp = ObjMgrGetSelected ();
    while (ssp != NULL && ! cancel)
    {
      if(ssp->itemtype == OBJ_SEQFEAT) 
      {
        sfp = SeqMgrGetDesiredFeature (cgp->input_entityID, NULL, ssp->itemID, 0, NULL, &fcontext);
        if(sfp != NULL && sfp->data.choice == SEQFEAT_CDREGION)
        {
          if (cds == NULL)
          {
            cds = sfp;
          }
          else
          {
            Message (MSG_ERROR, "You have selected more than one CDS!");
            cancel = TRUE;
          }
        }
        else if (sfp != NULL && sfp->data.choice == SEQFEAT_GENE)
        {
          if (gene == NULL)
          {
            gene = sfp;
          }
          else
          {
            Message (MSG_ERROR, "You have selected more than one gene!");
            cancel = TRUE;
          }
        }
      }
      ssp = ssp->next;
    }
    
    if (!cancel && cds != NULL && gene != NULL)
    {
      CorrectOneCDSGenePair (cds, gene, NULL, cgp);
    }
    else if (!cancel)
    {
      Message (MSG_ERROR, "You need to select one CDS and one gene");
    }
  }
  else
  {
    VisitFeaturesInSep (cgp->sep, cgp, CorrectOneCDSGene);
  }
  
  /* close, display, and remove log file */
  if (cgp->fp != NULL)
  {
    FileClose (cgp->fp);
    cgp->fp = NULL;
    LaunchGeneralTextViewer (path, "Gene Change Log");
    FileRemove (path);
  }
  
  ObjMgrSetDirtyFlag (cgp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, cgp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();
  
  if (GetStatus (cgp->leave_dlg_up))
  {
    Show (cgp->form);
  }
  else
  {
    Remove (cgp->form);
  }

}

static void CorrectGenes (IteM i)
{
  BaseFormPtr        bfp;
  CorrectGeneFormPtr cgp;
  WindoW             w;
  GrouP              g, c;
  ButtoN             b;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  cgp = MemNew (sizeof (CorrectGeneFormData));
  if (cgp == NULL) return;
  
  w = FixedWindow (-50, -33, -20, -10, "Correct Genes for CDSs",
                   StdCloseWindowProc);
  SetObjectExtra (w, cgp, StdCleanupFormProc);
  cgp->form = (ForM) w;

  cgp->input_entityID = bfp->input_entityID;
  cgp->input_itemID = bfp->input_itemID;
  cgp->input_itemtype = bfp->input_itemtype;

  g = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (g, 10, 10);
  cgp->setIntervalBtn = CheckBox (g, "Set gene interval to match CDS", NULL);
  SetStatus (cgp->setIntervalBtn, TRUE);
  cgp->setStrandBtn = CheckBox (g, "Set gene strand to match CDS", NULL);
  SetStatus (cgp->setStrandBtn, FALSE);
  cgp->logChangesBtn = CheckBox (g, "Log gene changes", NULL);
  SetStatus (cgp->logChangesBtn, TRUE);
  
  cgp->correct_selected_pair = CheckBox (g, "Correct only selected CDS-gene pair", NULL);

  c = HiddenGroup (g, 3, 0, NULL);
  b = DefaultButton(c, "Accept", CorrectGenesForCDSs);
  SetObjectExtra(b, cgp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  cgp->leave_dlg_up = CheckBox (c, "Leave Dialog Up", NULL);

  AlignObjects (ALIGN_CENTER,
                (HANDLE) cgp->setIntervalBtn,
                (HANDLE) cgp->setStrandBtn,
                (HANDLE) cgp->correct_selected_pair,
	        (HANDLE) c, NULL);
  RealizeWindow(w);
  Show(w);
  Update();
}

static void ResetAllGenes (SeqEntryPtr sep,
			Boolean doMinus, Boolean doPlus, FILE *fp)
{
  SeqEntryPtr		sep_index;
  SeqFeatPtr		gene;
  SeqFeatPtr		cds;
  SeqMgrFeatContext  fcontext;
  SeqMgrFeatContext  cdscontext;
  CharPtr		genename;
  BioseqSetPtr		bssp;
  BioseqPtr		bsp;
  SeqLocPtr		slp;
  SeqLocPtr		tmpslp;
  Boolean               cds_partial5, cds_partial3;
  Boolean               partial5, partial3, partial_flag;

  if (sep == NULL) return;
  if ( IS_Bioseq_set (sep))
  {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    for (sep_index = bssp->seq_set;
	sep_index != NULL;
	sep_index = sep_index->next)
    {
      ResetAllGenes ( sep_index, doMinus, doPlus, fp);
    }
    return;
  }
  else if ( IS_Bioseq(sep))
  {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    gene = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_GENE, 0, &fcontext);
    while (gene != NULL)
    {
      if (fcontext.strand == Seq_strand_minus && !doMinus)
        continue;
      if (fcontext.strand != Seq_strand_minus && !doPlus)
        continue;
      slp = NULL;
      partial3 = FALSE;
      partial5 = FALSE;
      partial_flag = FALSE;
      /* look for next feature, starting right after the gene */
      /* an overlapping CDS won't occur before the gene in the indexing */
      cds = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_CDREGION, 0, &cdscontext);
      while (cds != NULL)
      {
        if (((fcontext.strand == Seq_strand_minus
             && cdscontext.strand == Seq_strand_minus)
            || (fcontext.strand != Seq_strand_minus
             && cdscontext.strand != Seq_strand_minus))
            && SeqLocAinB (cds->location, gene->location) > -1)
        {
          CheckSeqLocForPartial (cds->location, &cds_partial5, &cds_partial3);
          partial5 |= cds_partial5;
          partial3 |= cds_partial3;
          partial_flag |= cds->partial;
          tmpslp = SeqLocMerge (bsp, cds->location, slp, TRUE, TRUE, FALSE);
          if (tmpslp == NULL) return;
          SeqLocFree (slp);
          slp = tmpslp;
        }
        cds = SeqMgrGetNextFeature (bsp, cds, SEQFEAT_CDREGION, 0,
					&cdscontext);
      }

      if (slp != NULL)
      {
        SetSeqLocPartial (slp, partial5, partial3);
        if (fp != NULL
          && (SeqLocStart (gene->location) != SeqLocStart (slp)
            || SeqLocStop (gene->location) != SeqLocStop (slp)))
        {
          genename = GetGeneName ((GeneRefPtr) gene->data.value.ptrvalue);
          if (genename == NULL)
          {
            fprintf (fp, "Unnamed gene ");
            LogLocation (&fcontext, fp);
          }
          else
          {
            fprintf (fp, "Gene %s", genename);
          }
          fprintf (fp, " reset to CDS interval\n");
        }
        SeqLocFree (gene->location);
        gene->location = slp;
        gene->partial = partial_flag;
      }
      gene = SeqMgrGetNextFeature (bsp, gene, SEQFEAT_GENE, 0, &fcontext);
    }
  }
}

static void ExtendGeneMessageProc (ForM f, Int2 mssg)
{
  ExtendGeneFormPtr	egfp;

  egfp = (ExtendGeneFormPtr) GetObjectExtra(f);
  if (egfp != NULL) {
    if (egfp->appmessage != NULL) {
      egfp->appmessage (f, mssg);
    }
  }
}

static void CleanupExtendGenePage (GraphiC g, VoidPtr data)

{
  ExtendGeneFormPtr  egfp;

  egfp = (ExtendGeneFormPtr) data;
  if (egfp != NULL) {
    if (egfp->item_list != NULL)
      MemFree (egfp->item_list);
  }
  StdCleanupFormProc (g, data);
}

static void makeGeneXref (SeqFeatPtr sfpr, SeqFeatPtr sfpg)
{
  SeqFeatXrefPtr  curr;
  GeneRefPtr      grp;
  SeqFeatXrefPtr  PNTR last;
  SeqFeatXrefPtr  next;
  SeqFeatXrefPtr  xref;

  if (sfpr == NULL || sfpg == NULL || sfpg->data.choice != SEQFEAT_GENE) return;
  grp = (GeneRefPtr) sfpg->data.value.ptrvalue;
  if (grp == NULL) return;
  last = (SeqFeatXrefPtr PNTR) &(sfpr->xref);
  curr = sfpr->xref;
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
  grp = GeneRefDup (grp);
  if (grp == NULL) return;
  xref = SeqFeatXrefNew ();
  sfpr->xref = xref;
  if (xref != NULL) {
    xref->data.choice = SEQFEAT_GENE;
    xref->data.value.ptrvalue = (Pointer) grp;
  }
}

static void extendGeneInStreamOnStrand(ExtendGeneFormPtr egfp,
					BioseqPtr bsp,
					SeqFeatPtr GeneToExtend,
					SeqFeatPtr FarthestRegulator,
					Boolean stream,
					Boolean isPlusStrand)
{
  SeqLocPtr		slp;
  Int2			item_index;

  /* extend previous minus gene to last minus_upstream_regulator */
  slp = SeqLocMerge (bsp,
  	GeneToExtend->location, FarthestRegulator->location,
	TRUE, TRUE, FALSE);
  if (slp == NULL) return;
  GeneToExtend->location = SeqLocFree (GeneToExtend->location);
  GeneToExtend->location = slp;
  for (item_index = 0 ; item_index < NUM_REGULATORY_ITEMS; item_index++)
  {
    if (list_of_regulatory_items[item_index].upstream == stream)
    {
      if(isPlusStrand)
      {
        if (egfp->item_list[item_index].plus_sfp != NULL)
        {
          /* Make XRefs for all stream plus regulators */
          makeGeneXref (egfp->item_list[item_index].plus_sfp, GeneToExtend);

          /* Then reset the stream plus pointers */
          egfp->item_list[item_index].plus_sfp = NULL;
        }
      }
      else
      {
        if (egfp->item_list[item_index].minus_sfp != NULL)
        {
          /* Make XRefs for all stream minus regulators */
          makeGeneXref (egfp->item_list[item_index].minus_sfp, GeneToExtend);

          /* Then reset the stream minus pointers */
          egfp->item_list[item_index].minus_sfp = NULL;
        }
      }
    }
  }
}

typedef struct featureloginfo {
  SeqMgrFeatContext	context;
  CharPtr		featurename;
} FeatureLogInfo, FAR *FeatureLogInfoPtr;
 
typedef struct strandloginfo {
  FeatureLogInfo	upstream;
  FeatureLogInfo	downstream;
} StrandLogInfo, FAR *StrandLogInfoPtr;

static void LogExtend (SeqFeatPtr gene, SeqMgrFeatContextPtr genecontext,
		StrandLogInfoPtr log_info, Boolean doLogUnextended, FILE *fp)
{
  GeneRefPtr grp;
  CharPtr genename;

  if (fp == NULL) return;

  if (gene == NULL) return;

  grp = (GeneRefPtr) gene->data.value.ptrvalue;
  if (grp == NULL) return;

  if (!doLogUnextended && log_info->upstream.featurename == NULL && log_info->downstream.featurename == NULL)
    return;

  fprintf (fp, "Gene ");
  genename = GetGeneName (grp);
  if (genename != NULL)
  {
    fprintf (fp, "%s", genename);
  }
  else
  {
    LogLocation (genecontext, fp);
  }

  if (log_info->upstream.featurename == NULL && log_info->downstream.featurename == NULL)
  {
    fprintf (fp, " not extended\n");
  }
  else if (log_info->upstream.featurename != NULL)
  {
    fprintf (fp, " extended to %s ", log_info->upstream.featurename);
    LogLocation (&(log_info->upstream.context), fp);
    if (log_info->downstream.featurename != NULL)
    {
      fprintf (fp, ", %s ", log_info->downstream.featurename);
      LogLocation (&(log_info->downstream.context), fp);
    }
    fprintf (fp, "\n");
  }
  else
  {
    fprintf (fp, " extended to %s ", log_info->downstream.featurename);
    LogLocation (&(log_info->downstream.context), fp);
    fprintf (fp, "\n");
  }  
}

static void ExtendGenesOnSequence (SeqEntryPtr sep, ExtendGeneFormPtr egfp)
{
  SeqEntryPtr		sep_index;
  BioseqSetPtr		bssp;
  BioseqPtr		bsp;
  SeqFeatPtr		sfp;
  SeqMgrFeatContext	fcontext;
  SeqFeatPtr		plus_upstream_regulator;
  SeqFeatPtr		plus_downstream_regulator;
  SeqFeatPtr		plus_gene;
  SeqFeatPtr		minus_upstream_regulator;
  SeqFeatPtr		minus_downstream_regulator;
  SeqFeatPtr		minus_gene;
  Int2			item_index;
  GeneRefPtr		grp;
  SeqFeatPtr		overlapping_gene;
  StrandLogInfo		previous_plus_log;
  StrandLogInfo		next_plus_log;
  StrandLogInfo		previous_minus_log;
  StrandLogInfo		next_minus_log;

  if (sep == NULL) return;
  if ( IS_Bioseq_set (sep))
  {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    for (sep_index = bssp->seq_set;
	sep_index != NULL;
	sep_index = sep_index->next)
    {
      ExtendGenesOnSequence ( sep_index, egfp);
    }
    return;
  }
  else if ( ! IS_Bioseq(sep))
  {
    return;
  }
 
  bsp = (BioseqPtr) sep->data.ptrvalue;

  /* Walk through gene sequence, extending genes to specified regulatory */
  /* elements, respecting strand direction */
  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);

  /* Loop through all features */

  plus_upstream_regulator = NULL;
  plus_downstream_regulator = NULL;
  plus_gene = NULL;
  minus_upstream_regulator = NULL;
  minus_downstream_regulator = NULL;
  minus_gene = NULL;
  previous_plus_log.upstream.featurename = NULL;
  previous_plus_log.downstream.featurename = NULL;
  previous_minus_log.upstream.featurename = NULL;
  previous_minus_log.downstream.featurename = NULL;
  next_plus_log.upstream.featurename = NULL;
  next_plus_log.downstream.featurename = NULL;
  next_minus_log.upstream.featurename = NULL;
  next_minus_log.downstream.featurename = NULL;

  while (NULL != sfp)
  {
    /* determine whether item has gene reference */
    overlapping_gene = SeqMgrGetOverlappingGene (sfp->location, NULL);
    grp = SeqMgrGetGeneXref (sfp);

    /* process strand items separately */
    if (fcontext.strand == Seq_strand_minus)
    {
      if (egfp->doMinusStrand)
      {
        if (fcontext.featdeftype == FEATDEF_GENE)
        {
          if (minus_gene != NULL && minus_upstream_regulator != NULL)
          {
            /* extend previous minus gene to last minus_upstream_regulator */
            extendGeneInStreamOnStrand(egfp, bsp, minus_gene,
					minus_upstream_regulator,
					TRUE, FALSE);
          }
          LogExtend (minus_gene, &fcontext, &previous_minus_log,
			egfp->doLogUnextendedEvents, egfp->fp);
          previous_minus_log = next_minus_log;
          next_minus_log.upstream.featurename = NULL;
          next_minus_log.downstream.featurename = NULL;
          minus_gene = sfp;
          if (minus_gene != NULL && minus_downstream_regulator != NULL)
          {
            /* extend this minus gene to minus_downstream_regulator */
            extendGeneInStreamOnStrand(egfp, bsp, minus_gene,
					minus_downstream_regulator,
					FALSE, FALSE);
          }
          minus_upstream_regulator = NULL;
          minus_downstream_regulator = NULL;
        }
        else
        {
          for (item_index = 0;
  		item_index < NUM_REGULATORY_ITEMS && list_of_regulatory_items[item_index].iFeatDef != fcontext.featdeftype; item_index++ )
          { }
          if (item_index < NUM_REGULATORY_ITEMS)
          {
            if (egfp->item_list[item_index].item_value &&
		((grp == NULL && overlapping_gene == NULL) || egfp->doStealFeatures))
            {
              if (list_of_regulatory_items[item_index].upstream)
              {
                minus_upstream_regulator = sfp;
                previous_minus_log.upstream.featurename = list_of_regulatory_items[item_index].label;
                previous_minus_log.upstream.context = fcontext;
              }
              else
              {
                /* found downstream regulator */
                if (minus_downstream_regulator == NULL)
                {
                  minus_downstream_regulator = sfp;
                  next_minus_log.downstream.featurename = list_of_regulatory_items[item_index].label;
                  next_minus_log.downstream.context = fcontext;
                }
              }
              egfp->item_list[item_index].minus_sfp = sfp;
            }
          }
        }
      }
    }
    else /* treat all other conditions as plus strand */
    {
      if (egfp->doPlusStrand)
      {
        if (fcontext.featdeftype == FEATDEF_GENE)
        {
          if (plus_gene != NULL && plus_downstream_regulator != NULL)
          {
            /* extend previous plus gene to last plus_downstream_regulator */
            extendGeneInStreamOnStrand(egfp, bsp, plus_gene,
					plus_downstream_regulator,
					FALSE, TRUE);
          }
          LogExtend (plus_gene, &fcontext, &previous_plus_log, 
			egfp->doLogUnextendedEvents,egfp->fp);
          plus_gene = sfp;
          if (plus_gene != NULL && plus_upstream_regulator != NULL)
          {
            /* extend previous plus gene to first plus_upstream_regulator */
            extendGeneInStreamOnStrand(egfp, bsp, plus_gene,
					plus_upstream_regulator,
					TRUE, TRUE);
          }
          plus_upstream_regulator = NULL;
          plus_downstream_regulator = NULL;
          previous_plus_log = next_plus_log;
          next_plus_log.upstream.featurename = NULL;
          next_plus_log.downstream.featurename = NULL;
        }
        else
        {
          for (item_index = 0;
		item_index < NUM_REGULATORY_ITEMS && list_of_regulatory_items[item_index].iFeatDef != fcontext.featdeftype; item_index++ )
          { }

          if (item_index < NUM_REGULATORY_ITEMS)
          {
            if (egfp->item_list[item_index].item_value &&
		((grp == NULL && overlapping_gene == NULL) || egfp->doStealFeatures))
            {
              if (list_of_regulatory_items[item_index].upstream)
              {
                if (plus_upstream_regulator == NULL)
                {
                  plus_upstream_regulator = sfp;
                  next_plus_log.upstream.featurename = list_of_regulatory_items[item_index].label;
                  next_plus_log.upstream.context = fcontext;
                }
              }
              else
              {
                /* found downstream regulator */
                plus_downstream_regulator = sfp;
                previous_plus_log.downstream.featurename = list_of_regulatory_items[item_index].label;
                previous_plus_log.downstream.context = fcontext;
              }
              egfp->item_list[item_index].plus_sfp = sfp;
            }
          }
        }
      }
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
  }

  /* extend final minus strand gene */
  if (minus_gene != NULL && minus_upstream_regulator != NULL && egfp->doMinusStrand)
  {
    /* extend this minus gene to minus_upstream_regulator */
    extendGeneInStreamOnStrand(egfp, bsp, minus_gene,
					minus_upstream_regulator,
					TRUE, FALSE);
  }
  LogExtend (minus_gene, &fcontext, &previous_minus_log, 
			egfp->doLogUnextendedEvents,egfp->fp);

  /* extend final plus strand gene */
  if (plus_gene != NULL && plus_downstream_regulator != NULL && egfp->doPlusStrand)
  {
    /* extend this plus gene to plus_downstream_regulator */
    extendGeneInStreamOnStrand(egfp, bsp, plus_gene,
					plus_downstream_regulator,
					FALSE, TRUE);
  }
  LogExtend (plus_gene, &fcontext, &previous_plus_log, 
			egfp->doLogUnextendedEvents,egfp->fp);

}
  
static void DoExtendGene (ButtoN b)
{
  SeqEntryPtr		sep;
  ExtendGeneFormPtr	egfp;
  Char			path [PATH_MAX];
  Int2			item_index;

  egfp = GetObjectExtra (b);
  if (egfp == NULL) return;
  Hide (egfp->form);
  WatchCursor ();
  Update ();

  egfp->doLogEvents = GetStatus (egfp->LogEvents);
  egfp->doLogUnextendedEvents = GetStatus (egfp->LogUnextendedEvents);
  if (egfp->doLogEvents)
  {
    TmpNam (path);
    egfp->fp = FileOpen (path, "wb");
  }
  else
  {
    egfp->fp = NULL;
  }
  
  /* get button statuses */
  for (item_index = 0; item_index < NUM_REGULATORY_ITEMS; item_index ++ )
  {
    egfp->item_list[item_index].item_value = GetStatus (egfp->item_list[item_index].item_button);
  }

  egfp->doPlusStrand = GetStatus (egfp->PlusStrand);
  egfp->doMinusStrand = GetStatus (egfp->MinusStrand);
  egfp->doResetGenes = GetStatus (egfp->ResetGenes);
  egfp->doStealFeatures = GetStatus (egfp->StealFeatures);

  sep = GetTopSeqEntryForEntityID (egfp->input_entityID);

  if (egfp->doResetGenes)
  {
    ResetAllGenes (sep, egfp->doMinusStrand, egfp->doPlusStrand, egfp->fp);
    /* reindex features */
    SeqMgrIndexFeatures (egfp->input_entityID, NULL);
  }

  ExtendGenesOnSequence (sep, egfp);

  /* reindex features */
  SeqMgrIndexFeatures (egfp->input_entityID, NULL);

  /* remove redundant x-refs */
  SeqMgrExploreBioseqs (egfp->input_entityID, 0, NULL, RemoveGeneXrefsOnBioseqs, TRUE, TRUE, TRUE);
  
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (egfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, egfp->input_entityID, 0, 0);
  Remove (egfp->form);
  if (egfp->doLogEvents)
  {
    FileClose (egfp->fp);
    LaunchGeneralTextViewer (path, "Extend Gene Log");
    FileRemove (path);
  }
}

static void DoGeneReset (IteM i)
{
  BaseFormPtr        bfp;
  SeqEntryPtr		sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  
  WatchCursor ();
  Update ();
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);

  ResetAllGenes (sep, TRUE, TRUE, NULL);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();
}

static void DisableLogOption (ButtoN b)
{
  ExtendGeneFormPtr	egfp;
  Boolean	DoLogEvents;

  egfp = (ExtendGeneFormPtr) GetObjectExtra (b);
  DoLogEvents = GetStatus (egfp->LogEvents);
  if (DoLogEvents)
  {
    Enable (egfp->LogUnextendedEvents);
  }
  else
  {
    SetStatus (egfp->LogUnextendedEvents, FALSE);
    Disable (egfp->LogUnextendedEvents);
  }
}

static void ExtendGeneReg (IteM i)
{
  BaseFormPtr        bfp;
  SeqEntryPtr        sep;
  ExtendGeneFormPtr	egfp;
  WindoW	w;
  GrouP		g;
  GrouP		h;
  GrouP		j;
  GrouP		k;
  GrouP		UpstreamGroup;
  GrouP		DownstreamGroup;
  GrouP		m;
  GrouP		c;
  ButtoN	b;
  Int2		item_index;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  egfp = (ExtendGeneFormPtr) MemNew (sizeof (ExtendGeneFormData));
  if (egfp == NULL) return;
  w = FixedWindow (-50, -33, -20, -10, "Extend Gene", StdCloseWindowProc);
  SetObjectExtra (w, egfp, CleanupExtendGenePage);
  egfp->form = (ForM) w;
  egfp->formmessage = ExtendGeneMessageProc;

  egfp->input_entityID = bfp->input_entityID;
  egfp->input_itemID = bfp->input_itemID;
  egfp->input_itemtype = bfp->input_itemtype;

  g = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (g, 10, 10);

  h = HiddenGroup (g, 0, 1, NULL);
  egfp->PlusStrand = CheckBox(h, "Extend on Plus Strand", NULL);
  SetStatus(egfp->PlusStrand, TRUE);
  egfp->MinusStrand = CheckBox(h, "Extend on Minus Strand", NULL);
  SetStatus(egfp->MinusStrand, TRUE);
  k = HiddenGroup (g, 0, 2, NULL);
  egfp->ResetGenes = CheckBox (k, "Reset Genes to CDS Locations before Extending", NULL);
  SetStatus (egfp->ResetGenes, FALSE);
  egfp->StealFeatures = CheckBox (k, "Reassign Features from Other Genes", NULL);
  SetStatus (egfp->StealFeatures, FALSE);
 
  egfp->item_list = MemNew (NUM_REGULATORY_ITEMS * sizeof (ExtendGeneFormLocalItemData));
  if (egfp->item_list == NULL) return;
  for (item_index = 0; item_index < NUM_REGULATORY_ITEMS; item_index ++)
  {
    egfp->item_list[item_index].item_button = NULL;
    egfp->item_list[item_index].item_value =
		list_of_regulatory_items[item_index].item_value;
    egfp->item_list[item_index].plus_sfp = NULL;
    egfp->item_list[item_index].minus_sfp = NULL;
  }   

  j = HiddenGroup (g, 1, 0, NULL);
  SetGroupSpacing (j, 10, 10);
  UpstreamGroup = NormalGroup (j, 0, 4, "Upstream", NULL, NULL);
  for(item_index = 0; item_index < NUM_REGULATORY_ITEMS; item_index ++)
  {
    if(list_of_regulatory_items[item_index].upstream)
    {
      egfp->item_list[item_index].item_button = CheckBox(UpstreamGroup, list_of_regulatory_items[item_index].label, NULL);
      SetStatus(egfp->item_list[item_index].item_button,
              list_of_regulatory_items[item_index].item_value);
    }
  }
  DownstreamGroup = NormalGroup (j, 4, 0, "Downstream", NULL, NULL);
  for(item_index = 0; item_index < NUM_REGULATORY_ITEMS; item_index ++)
  {
    if(! list_of_regulatory_items[item_index].upstream)
    {
      egfp->item_list[item_index].item_button = CheckBox(DownstreamGroup, list_of_regulatory_items[item_index].label, NULL);
      SetStatus(egfp->item_list[item_index].item_button,
              list_of_regulatory_items[item_index].item_value);
    }
  }
  m = HiddenGroup (g, 2, 0, NULL);
  egfp->LogEvents = CheckBox (m, "Log Changes", DisableLogOption);
  SetObjectExtra(egfp->LogEvents, egfp, NULL);
  SetStatus (egfp->LogEvents, FALSE);
  egfp->LogUnextendedEvents = CheckBox (m, "Log Unextended Genes", NULL);
  SetStatus (egfp->LogUnextendedEvents, FALSE);
  Disable (egfp->LogUnextendedEvents);

  c = HiddenGroup (g, 2, 0, NULL);
  b = DefaultButton(c, "Accept", DoExtendGene);
  SetObjectExtra(b, egfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) h, (HANDLE) k,
	(HANDLE) j, (HANDLE) m, (HANDLE) c, NULL);
  RealizeWindow(w);
  Show(w);
  Update();
}

static void ResynchronizeCDSPartials (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  WatchCursor ();
  Update ();
  ResynchCodingRegionPartials (sep);
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void ResynchronizeMRNAPartials (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  WatchCursor ();
  Update ();
  ResynchMessengerRNAPartials (sep);
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void ResynchronizePeptidePartials (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  WatchCursor ();
  Update ();
  ResynchProteinPartials (sep);
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void UnsetPartialForTruncatedProteins (SeqFeatPtr sfp, Pointer userdata)
{
  Boolean partial5, partial3;
  ProtRefPtr prp;
  CharPtr    new_name;
  BoolPtr    change_name;

  if (sfp == NULL || userdata == NULL) return;
  change_name = (BoolPtr) userdata;
  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
  SetSeqLocPartial (sfp->location, partial5, FALSE);
  if (*change_name) {
    prp = sfp->data.value.ptrvalue;
    if (prp != NULL && prp->name != NULL 
        && prp->name->data.ptrvalue != NULL
        && StringNCmp (prp->name->data.ptrvalue, "truncated ", 10) != 0) {
      new_name = (CharPtr) MemNew (StringLen (prp->name->data.ptrvalue)
                                   + 11);
      if (new_name != NULL) {
        sprintf (new_name, "truncated %s", (CharPtr) prp->name->data.ptrvalue);
        MemFree (prp->name->data.ptrvalue);
        prp->name->data.ptrvalue = new_name;
      }
    }
  }
}

static SeqLocPtr TruncateLocation (SeqLocPtr head, Int4 len)
{
	SeqLocPtr slp = NULL;
	SeqIdPtr sip;
	Int4 from = 0, to = 0;
	Boolean changed = FALSE;
	Int4       loc_len = 0;
	Int4       cum_len = 0;
	SeqLocPtr  del_slp;
	ValNodePtr del_slp_list = NULL, vnp;
	Uint1      strand;

	if (head == NULL || len < 1)
		return head;

  slp = SeqLocFindNext (head, slp);
  while (slp != NULL)
  {
    if (cum_len >= len)
    {
      del_slp = AsnIoMemCopy(slp,
					(AsnReadFunc) SeqLocAsnRead, (AsnWriteFunc) SeqLocAsnWrite);
      ValNodeAddPointer (&del_slp_list, 0, del_slp);
    }
    else
    {
		  sip = SeqLocId(slp);
		  from = SeqLocStart(slp);
		  to = SeqLocStop(slp);
		  loc_len = (to - from + 1);
      cum_len += loc_len;
      if (cum_len > len)
      {
        strand = SeqLocStrand (slp);
        /* remove part of this location */
        if (strand == Seq_strand_minus)
        {
          to = cum_len - len + from - 1;
        }
        else
        {
          from = to - (cum_len - len) + 1;
        }
        del_slp = SeqLocIntNew (from, to, strand, sip);
        ValNodeAddPointer (&del_slp_list, 0, del_slp);
      }
    }
		slp = SeqLocFindNext (head, slp);
  }
  
  for (vnp = del_slp_list; vnp != NULL; vnp = vnp->next)
  {
    slp = (SeqLocPtr) vnp->data.ptrvalue;
    if (slp == NULL) continue;
		sip = SeqLocId(slp);
		from = SeqLocStart(slp);
		to = SeqLocStop(slp);
		head = SeqLocDelete(head, sip, from, to, FALSE, &changed);    
  }
  
	return head;
}

static void UnsetCDSPartialForTruncatedProteins (SeqFeatPtr sfp, Pointer userdata) 
{
  Boolean     partial5, partial3;
  BioseqPtr   product_bsp, prot_bsp;
  CdRegionPtr crp;
  Int4        cds_len;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION
      || sfp->location == NULL) return;
  if ((prot_bsp = (BioseqPtr)userdata) == NULL) return;

  product_bsp = BioseqFindFromSeqLoc (sfp->product);
  if (product_bsp == prot_bsp) {
    CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
    SetSeqLocPartial (sfp->location, partial5, FALSE);

    cds_len = (prot_bsp->length + 1) * 3 ;
    crp = (CdRegionPtr) sfp->data.value.ptrvalue;
    if (crp != NULL)
    {
      if (crp->frame == 2)
      {
        cds_len += 1;
      }
      else if (crp->frame == 3)
      {
        cds_len += 2;
      }
    }
    sfp->location = TruncateLocation (sfp->location, cds_len);
  }
}

typedef struct truncprotsdata {
  SeqEntryPtr sep;
  Boolean     change_name;
} TruncProtsData, PNTR TruncProtsPtr;

static void TruncProtsCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  ByteStorePtr  bs;
  BioseqPtr     bsp;
  Int4          i;
  Int2          residue;
  SeqPortPtr    spp;
  Boolean       found_stop;
  TruncProtsPtr tpp;

  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;
  if (! ISA_aa (bsp->mol)) return;
  if (bsp->repr != Seq_repr_raw && bsp->repr != Seq_repr_const) return;

  if ((tpp = (TruncProtsPtr)mydata) == NULL) return;

  bs = BSNew (1000);
  if (bs == NULL) return;
  spp = SeqPortNew (bsp, 0, -1, 0, Seq_code_ncbieaa);
  if (spp == NULL) return;

  i = 0;
  residue = SeqPortGetResidue (spp);
  while (i < bsp->length && residue != '*') {
    BSPutByte (bs, residue);
    i++;
    residue = SeqPortGetResidue (spp);
  }
  if (residue == '*') {
    found_stop = TRUE;
  } else {
    found_stop = FALSE;
  }

  SeqPortFree (spp);
  bsp->seq_data = BSFree (bsp->seq_data);
  bsp->seq_data = bs;
  bsp->length = BSLen (bs);
  bsp->seq_data_type = Seq_code_ncbieaa;
  if (found_stop) {
    VisitFeaturesOnBsp (bsp, (Pointer) &(tpp->change_name), 
                        UnsetPartialForTruncatedProteins);
    VisitFeaturesInSep (tpp->sep, bsp,  UnsetCDSPartialForTruncatedProteins);
  }
}

static void TruncProtsAtStopsExEx (Uint2 entityID, Boolean change_name)

{
  SeqEntryPtr    sep;
  TruncProtsData tpd;

  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep == NULL) return;
  tpd.sep = sep;
  tpd.change_name = change_name;
  SeqEntryExplore (sep, &tpd, TruncProtsCallback);
  ObjMgrSetDirtyFlag (entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
}

static void TruncProtsAtStopsEx (IteM i, Boolean change_name)

{
  BaseFormPtr    bfp;

#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = (BaseFormPtr) GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  TruncProtsAtStopsExEx (bfp->input_entityID, change_name);
}

static void TruncProtsAtStops (IteM i)
{
  TruncProtsAtStopsEx (i, FALSE);
}

static void TruncProtsAtStopsChangeNames (IteM i)
{
  TruncProtsAtStopsEx (i, TRUE);
}

static void TrimProtsCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  SeqAnnotPtr   sap;
  SeqFeatPtr    sfp;
  SeqIntPtr     sintp;
  SeqLocPtr     slp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        if (sfp->data.choice == SEQFEAT_PROT) {
          bsp = BioseqFind (SeqLocId (sfp->location));
          if (bsp != NULL) {
            slp = SeqLocFindNext (sfp->location, NULL);
            if (slp != NULL && slp->choice == SEQLOC_INT) {
              sintp = (SeqIntPtr) slp->data.ptrvalue;
              if (sintp != NULL) {
                if (sintp->from == 0 && sintp->to > bsp->length - 1) {
                  sintp->to = bsp->length - 1;
                }
              }
            }
          }
        }
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
}

static void TrimProtFeatLengthsEx (Uint2 entityID)

{
  SeqEntryPtr  sep;
  
  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep == NULL) return;
  SeqEntryExplore (sep, NULL, TrimProtsCallback);
  ObjMgrSetDirtyFlag (entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
}

static void TrimProtFeatLengths (IteM i)

{
  BaseFormPtr  bfp;

#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = (BaseFormPtr) GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  TrimProtFeatLengthsEx (bfp->input_entityID);
}

static void FixProtsAndCDSsBasedOnStopCodons (Uint2 entityID, Boolean fix_genes, Boolean change_names)
{
  TruncProtsAtStopsExEx (entityID, change_names);
  TrimProtFeatLengthsEx (entityID);
  RecomputeSuggestEx (entityID, fix_genes, TRUE);
  RetranslateCdRegionsEx (entityID, FALSE, FALSE);
}

typedef struct fixprotsandcds
{
  FEATURE_FORM_BLOCK

  ButtoN      FixGenesBtn;
  ButtoN      ChangeNamesBtn;
  Uint2       entityID;
} FixProtsAndCDSData, PNTR FixProtsAndCDSPtr;

static void FixProtsAndCDSsBasedOnStopCodonsButton (ButtoN b)
{
  FixProtsAndCDSPtr fp;
  Boolean           fix_genes;
  Boolean           change_names;
  
  fp = GetObjectExtra (b);
  if (fp == NULL) return;

  fix_genes = GetStatus (fp->FixGenesBtn);
  change_names = GetStatus (fp->ChangeNamesBtn);
  Remove (fp->form);

  FixProtsAndCDSsBasedOnStopCodons (fp->entityID, fix_genes, change_names);
}

static void FixProtsAndCDSsBasedOnStopCodonsItem (IteM i)
{
  BaseFormPtr       bfp;
  FixProtsAndCDSPtr fp;
  WindoW            w;
  GrouP             g, k, c;
  ButtoN            b;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  fp = (FixProtsAndCDSPtr) MemNew (sizeof (FixProtsAndCDSData));
  if (fp == NULL) return;
  
  w = FixedWindow (-50, -33, -20, -10, "Adjust Proteins and CDSs for Stop Codons",
                   StdCloseWindowProc);
  SetObjectExtra (w, fp, StdCleanupFormProc);
  fp->form = (ForM) w;
  fp->entityID = bfp->input_entityID;

  fp->input_entityID = bfp->input_entityID;
  fp->input_itemID = bfp->input_itemID;
  fp->input_itemtype = bfp->input_itemtype;

  g = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (g, 10, 10);
  k = HiddenGroup (g, 0, 2, NULL);
  SetGroupSpacing (g, 10, 10);
  fp->FixGenesBtn = CheckBox (k, "Reset Genes", NULL);
  SetStatus (fp->FixGenesBtn, TRUE);
  fp->ChangeNamesBtn = CheckBox (k, "Append 'Truncated' to Protein Name", NULL);
  SetStatus (fp->ChangeNamesBtn, TRUE);

  c = HiddenGroup (g, 2, 0, NULL);
  b = DefaultButton(c, "Accept", FixProtsAndCDSsBasedOnStopCodonsButton);
  SetObjectExtra(b, fp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) k, (HANDLE) c, NULL);
  RealizeWindow(w);
  Show(w);
  Update();
}


static void TrimNucsCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr    bsp;
  SeqEntryPtr  top;

  if (sep == NULL) return;
  if (! IS_Bioseq (sep)) return;
  top = (SeqEntryPtr) mydata;
  if (top == NULL) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;
  if (! ISA_na (bsp->mol)) return;
  BioseqTrimN (bsp, top);
}

static void TrimNsFromNucs (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = (BaseFormPtr) GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  SeqEntryExplore (sep, (Pointer) sep, TrimNucsCallback);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void TrimNsFromNucsToolBtn (ButtoN b)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

  if (b == NULL) return;
  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  SeqEntryExplore (sep, (Pointer) sep, TrimNucsCallback);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}
static Boolean s_NextQualifies (SeqFeatPtr sfp,
				BioseqPtr  bsp,
				Int2       level,
				SeqMgrFeatContextPtr context)
{
  SeqFeatPtr        nextSfp;
  SeqMgrFeatContext fContext;
  GBQualPtr         gbq;
  Boolean           isvector;

  /* Get the next feature */

  fContext = *context;
  nextSfp = SeqMgrGetNextFeature (bsp, sfp, 0, FEATDEF_misc_feature,
				  &fContext);
  if (nextSfp == NULL)
    return FALSE;

  /* Make sure that it's a Vector Contamination misc feat */

  for (isvector = FALSE, gbq = nextSfp->qual; gbq != NULL; gbq = gbq->next) {
    if (StringCmp (gbq->qual, "standard_name") == 0) {
      if (StringCmp (gbq->val, "Vector Contamination") == 0) {
        isvector = TRUE;
      }
    }
  }
  if (! isvector)
    return FALSE;

  /* Check the match level against the level that we're looking for */

  for (isvector = FALSE, gbq = nextSfp->qual; gbq != NULL; gbq = gbq->next) {
    if (StringCmp (gbq->qual, "phenotype") == 0) {
      if (StringCmp (gbq->val, "Strong match") == 0) {
        isvector = TRUE;
      } else if (StringCmp (gbq->val, "Moderate match") == 0 && level > 1) {
        isvector = TRUE;
      } else if (StringCmp (gbq->val, "Weak match") == 0 && level > 2) {
        isvector = TRUE;
      }
    }
  }
  if (! isvector)
    return FALSE;

  /* If we've made it this far, then the next feature */
  /* qualifies to be overwritten with N's.            */

  return TRUE;
}

static Boolean LIBCALLBACK VecToN (SeqFeatPtr sfp, SeqMgrFeatContextPtr context)

{
  BioseqPtr      bsp;
  GBQualPtr      gbq;
  Int2           i, j, level, numivals;
  Boolean        isvector;
  Int4Ptr        ivals;
  Int4           start, stop, swap;
  VecToNDataPtr  vDataPtr;

  if (sfp == NULL || context == NULL) return TRUE;
  if (context->featdeftype != FEATDEF_misc_feature) return TRUE;
  vDataPtr = (VecToNDataPtr) context->userdata;
  if (vDataPtr == NULL) return TRUE;
  level = vDataPtr->level;

  /* Make sure that it's a Vector Contamination misc feat */

  for (isvector = FALSE, gbq = sfp->qual; gbq != NULL; gbq = gbq->next) {
    if (StringCmp (gbq->qual, "standard_name") == 0) {
      if (StringCmp (gbq->val, "Vector Contamination") == 0) {
        isvector = TRUE;
      }
    }
  }
  if (! isvector) {
    vDataPtr->prevWasModified = FALSE;
    return TRUE;
  }

  /* Check the match level against the level that we're looking for */

  for (isvector = FALSE, gbq = sfp->qual; gbq != NULL; gbq = gbq->next) {
    if (StringCmp (gbq->qual, "phenotype") == 0) {
      if (StringCmp (gbq->val, "Strong match") == 0) {
        isvector = TRUE;
      } else if (StringCmp (gbq->val, "Moderate match") == 0 && level > 1) {
        isvector = TRUE;
      } else if (StringCmp (gbq->val, "Weak match") == 0 && level > 2) {
        isvector = TRUE;
      } else if (StringCmp (gbq->val, "Suspect origin") == 0) {
	if (vDataPtr->prevWasModified ||
	    s_NextQualifies (sfp, context->bsp, level,
			     context))
	  isvector = TRUE;
      }
    }
  }
  if (! isvector) {
    vDataPtr->prevWasModified = FALSE;
    return TRUE;
  }

  numivals = context->numivals;
  ivals = context->ivals;
  if (numivals < 1 || ivals == NULL) return TRUE;

  /* based on BioseqOverwrite */

  bsp = context->bsp;
  if (bsp == NULL || (! ISA_na (bsp->mol)) || bsp->repr != Seq_repr_raw) return TRUE;
  if (bsp->seq_data_type != Seq_code_iupacna) {
    BioseqRawConvert (bsp, Seq_code_iupacna);
  }

  for (i = 0, j = 0; i < numivals; i++) {
    start = ivals [j];
    j++;
    stop = ivals [j];
    j++;
    if (start > stop) {
      swap = start;
      start = stop;
      stop = swap;
    }
    if (start <= stop) {
      BSSeek (bsp->seq_data, start, SEEK_SET);
      while (start <= stop) {
        BSPutByte (bsp->seq_data, (Int2) 'N');
        start++;
      }
    }
  }
  vDataPtr->prevWasModified = TRUE;
  return TRUE;
}

static Boolean LIBCALLBACK BioseqVecToN (BioseqPtr bsp,
					 SeqMgrBioseqContextPtr bContext)
{
  Int2Ptr    levelPtr;
  Boolean    featDefFilt [FEATDEF_MAX];
  VecToNData vData;

  levelPtr = (Int2Ptr) bContext->userdata;
  MemSet ((Pointer) featDefFilt, FALSE, sizeof (featDefFilt));
  featDefFilt [FEATDEF_misc_feature] = TRUE;

  vData.level = *levelPtr;
  vData.prevWasModified = FALSE;

  SeqMgrExploreFeatures (bsp, (Pointer) &vData, VecToN,
			 NULL, NULL, featDefFilt);
  return TRUE;
}

static void VecScreenToNs (IteM i, Int2 level)

{
  BaseFormPtr  bfp;
  Boolean      featDefFilt [FEATDEF_MAX];
  BioseqPtr    bsp;
  VecToNData   vData;

#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = (BaseFormPtr) GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  WatchCursor ();
  Update ();

  MemSet ((Pointer) featDefFilt, FALSE, sizeof (featDefFilt));
  featDefFilt [FEATDEF_misc_feature] = TRUE;
  vData.level = level;
  vData.prevWasModified = FALSE;
  bsp = GetBioseqGivenIDs (bfp->input_entityID,
			   bfp->input_itemID,
			   bfp->input_itemtype);
  if (NULL == bsp)
    SeqMgrExploreBioseqs (bfp->input_entityID, 0, &level, BioseqVecToN,
			  TRUE, TRUE, TRUE);
  else
    SeqMgrExploreFeatures (bsp, (Pointer) &vData, VecToN,
			   NULL, NULL, featDefFilt);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);

  ArrowCursor ();
  Update ();
}

static void VecScreenToNsStrong (IteM i)

{
  VecScreenToNs (i, 1);
}

static void VecScreenToNsModerate (IteM i)

{
  VecScreenToNs (i, 2);
}

static void VecScreenToNsWeak (IteM i)

{
  VecScreenToNs (i, 3);
}

static void AddGenomesUserObject (IteM i)

{
  BaseFormPtr    bfp;
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  ObjectIdPtr    oip;
  SeqEntryPtr    sep;
  UserFieldPtr   ufp;
  UserObjectPtr  uop;
  ValNodePtr     vnp;

#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = (BaseFormPtr) GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL || sep->data.ptrvalue == NULL) return;

  oip = ObjectIdNew ();
  oip->str = StringSave ("info");
  uop = UserObjectNew ();
  uop->type = oip;
  uop->_class = StringSave ("Genomes");

  oip = ObjectIdNew ();
  oip->id = 0;
  ufp = UserFieldNew ();
  ufp->choice = 2;
  ufp->data.intvalue = 0;
  ufp->label = oip;

  uop->data = ufp;

  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    vnp = SeqDescrNew (NULL);
    vnp->choice = Seq_descr_user;
    vnp->data.ptrvalue = (Pointer) uop;
    vnp->next = bsp->descr;
    bsp->descr = vnp;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    vnp = SeqDescrNew (NULL);
    vnp->choice = Seq_descr_user;
    vnp->data.ptrvalue = (Pointer) uop;
    vnp->next = bssp->descr;
    bssp->descr = vnp;
  }

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static Boolean AddBlastTag (GatherContextPtr gcp)

{
  Int2         align_type;
  SeqAnnotPtr  sap;

  align_type = (Int2) (Int4) gcp->userdata;
  if (gcp->thistype != OBJ_SEQANNOT) return TRUE;
  sap = (SeqAnnotPtr) gcp->thisitem;
  if (sap == NULL || sap->type != 2) return TRUE;
  AddAlignInfoToSeqAnnot (sap, align_type);
  return TRUE;
}

static void CommonAddBlastToSeqAnnot (IteM i, Int2 align_type)

{
  BaseFormPtr   bfp;
  SelStructPtr  sel;

#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = (BaseFormPtr) GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  sel = ObjMgrGetSelected ();
  if (sel != NULL && sel->next == NULL &&
      sel->entityID == bfp->input_entityID &&
      sel->itemtype == OBJ_SEQANNOT) {
    GatherItem (sel->entityID, sel->itemID, sel->itemtype, (Pointer) (Int4) align_type, AddBlastTag);
  }

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void AddBlastNToSeqAnnot (IteM i)

{
  CommonAddBlastToSeqAnnot (i, ALIGN_BLASTN);
}

static void AddBlastPToSeqAnnot (IteM i)

{
  CommonAddBlastToSeqAnnot (i, ALIGN_BLASTP);
}

static void AddBlastXToSeqAnnot (IteM i)

{
  CommonAddBlastToSeqAnnot (i, ALIGN_BLASTX);
}

static void AddTBlastNToSeqAnnot (IteM i)

{
  CommonAddBlastToSeqAnnot (i, ALIGN_TBLASTN);
}

static void ToolBtn1 (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

  SaveSeqSubmitProc (bfp, FALSE);
}

static void ToolBtn2 (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

  AutoDefBaseFormCommon (bfp, FALSE, TRUE);
}

/*
static void ToolBtn3 (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

  ForceTaxonFixupBtn (NULL, b);
}
*/

static void ToolBtn3 (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

  ForceCleanupBtn (NULL, b, FALSE);
}

static void ToolBtn4 (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

  CommonAddOrgOrModsToDefLines (NULL, 0, 0, b);
}

static void ToolBtn5 (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

  WatchCursor ();
  Update ();
  GatherProcLaunch (OMPROC_EDIT, FALSE, bfp->input_entityID, bfp->input_itemID,
                    bfp->input_itemtype, OBJ_SEQFEAT, FEATDEF_CDS, 0, 0);
  ArrowCursor ();
  Update ();
}

static void ToolBtn6 (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;


  WatchCursor ();
  Update ();
  GatherProcLaunch (OMPROC_EDIT, FALSE, bfp->input_entityID, bfp->input_itemID,
                    bfp->input_itemtype, OBJ_SEQFEAT, FEATDEF_rRNA, 0, 0);
  ArrowCursor ();
  Update ();
}

static void ToolBtn7 (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

  MRnaFromCdsProc (bfp->input_entityID);
}

static void ToolBtn8 (ButtoN b)

{
  BaseFormPtr  bfp;
  BioseqPtr    bsp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;
  bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (bsp == NULL) return;
  LaunchOrfViewer (bsp, bfp->input_entityID, bfp->input_itemID, FALSE);
}

static void ToolBtn9 (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;


  WatchCursor ();
  Update ();
  GatherProcLaunch (OMPROC_EDIT, FALSE, bfp->input_entityID, bfp->input_itemID,
                    bfp->input_itemtype, OBJ_SEQFEAT, FEATDEF_misc_feature, 0, 0);
  ArrowCursor ();
  Update ();
}

static void ToolBtn10 (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

  SUCCommonProc (NULL, FALSE, TRUE, TRUE, b);
}

static void ToolBtn11 (ButtoN b)

{
  BaseFormPtr  bfp;
  Int2         mssgsub;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

  mssgsub = RegisterFormMenuItemName ("SequinEditSubmitterItem");
  SendMessageToForm (bfp->form, mssgsub);
}

static Boolean MakePubOnTopSep (GatherContextPtr gcp)

{
  Pointer  ptrvalue;

  if (gcp == NULL) return TRUE;
  ptrvalue = (Pointer) gcp->userdata;
  if (ptrvalue == NULL) return TRUE;
  if (gcp->thistype == OBJ_BIOSEQ || gcp->thistype == OBJ_BIOSEQSET) {
    if (ptrvalue == gcp->thisitem) {

      WatchCursor ();
      Update ();
      GatherProcLaunch (OMPROC_EDIT, FALSE, gcp->entityID, gcp->itemID,
                        gcp->thistype, OBJ_SEQDESC, Seq_descr_pub, 0, 0);
      ArrowCursor ();
      Update ();
      return FALSE;
    }
  }
  return TRUE;
}

static void ToolBtn12 (ButtoN b)

{
  BaseFormPtr  bfp;
  GatherScope  gs;
  SeqEntryPtr  sep;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL || sep->data.ptrvalue == NULL) return;

  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  MemSet((Pointer) (gs.ignore), (int) (TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQSET] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  GatherEntity (bfp->input_entityID, (Pointer) sep->data.ptrvalue, MakePubOnTopSep, &gs);
}

static void ToolBtn13 (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

  EditGenbankElements ((Handle) b);
}

static void ToolBtn14 (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

  ValSeqEntryForm (bfp->form);
}

static void ToolBtn15 (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

  VSeqMgrShow ();
}

static void ToolBtn16 (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

  AddCitSubForUpdateProc (bfp);
}

static void MarkHypotheticalProteinTitles (SeqDescrPtr sdp, Pointer userdata)
{
  CharPtr pDefLine;
  CharPtr pCh;
  CharPtr pMatch = "hypothetical protein";
  size_t match_len = StringLen(pMatch);
  ObjValNodePtr ovn;

  if (sdp == NULL || sdp->extended == 0) return;
  if (sdp->choice != Seq_descr_title) return;
  pDefLine = sdp->data.ptrvalue;
  if (pDefLine == NULL) return;
  if(StringNCmp(pDefLine, pMatch, match_len) != 0)
  {
    return;
  }
  pCh = pDefLine + match_len;
  if(*pCh == 0
	|| (*pCh == '.' && *(pCh + 1) == 0)
	|| (*pCh == ' ' && *(pCh + 1) == '['))
  {
    ovn = (ObjValNodePtr) sdp;
    ovn->idx.deleteme = TRUE;
  }
}

static void RemoveBioseqHypotheticalProteinTitles (BioseqPtr bsp, Pointer userdata)
{
  if (bsp == NULL) return;
  if (! ISA_aa(bsp->mol)) return;
  VisitDescriptorsOnBsp(bsp, userdata, MarkHypotheticalProteinTitles);
}

static void RemoveHypotheticalProteinTitles (IteM i)
{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  VisitBioseqsInSep (sep, NULL, RemoveBioseqHypotheticalProteinTitles);
  DeleteMarkedObjects(bfp->input_entityID, 0, NULL);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  Update ();
}

static void RemoveProtTitlesProc (BaseFormPtr bfp)

{
  BioseqPtr    bsp;
  SeqEntryPtr  fsep;
  SeqEntryPtr  sep;
  SeqIdPtr     sip;

  if (bfp == NULL) return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  fsep = FindNthBioseq (sep, 1);
  if (fsep != NULL && IS_Bioseq (fsep)) {
    bsp = (BioseqPtr) fsep->data.ptrvalue;
    if (bsp != NULL) {
      for (sip = bsp->id; sip != NULL; sip = sip->next) {
        if (sip->choice == SEQID_OTHER) {
          Message (MSG_OK, "Use Remove ALL Protein Titles for RefSeq records");
          break;
        }
      }
    }
  }

  ClearProteinTitlesInNucProts (bfp->input_entityID, NULL);

  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void DoRemoveProtTitles (IteM i)

{
  BaseFormPtr  bfp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  RemoveProtTitlesProc (bfp);
}

static void ToolBtn17 (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

  RemoveProtTitlesProc (bfp);
}

static void RemoveBioseqInconsistentProteinTitles (BioseqPtr bsp, Pointer userdata)
{
  BioseqSetPtr       bssp;
  Char               buf [1001];
  SeqMgrDescContext  dcontext;
  MolInfoPtr         mip;
  ObjValNodePtr      ovn;
  SeqDescrPtr        sdp;
  Uint1              tech;
  CharPtr            title;
  ValNodePtr         vnp;

  if (bsp == NULL) return;
  if (ISA_aa (bsp->mol)) {
    vnp = BioseqGetSeqDescr (bsp, Seq_descr_title, NULL);
    if (vnp != NULL) {
      if (bsp->idx.parenttype == OBJ_BIOSEQSET) {
        bssp = (BioseqSetPtr) bsp->idx.parentptr;
        while (bssp != NULL && bssp->_class != BioseqseqSet_class_nuc_prot) {
          if (bssp->idx.parenttype == OBJ_BIOSEQSET) {
            bssp = (BioseqSetPtr) bssp->idx.parentptr;
          } else {
            bssp = NULL;
          }
        }
        if (bssp != NULL && bssp->_class == BioseqseqSet_class_nuc_prot) {
          title = (CharPtr) vnp->data.ptrvalue;
          tech = 0;
          sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
          if (sdp != NULL) {
            mip = (MolInfoPtr) sdp->data.ptrvalue;
            if (mip != NULL) {
              tech = mip->tech;
            }
          }
          if (CreateDefLineEx (NULL, bsp, buf, sizeof (buf) - 1, tech, NULL, NULL, TRUE)) {
            if (StringICmp (buf, title) != 0) {
              ovn = (ObjValNodePtr) vnp;
              ovn->idx.deleteme = TRUE;
            }
          }
        }
      }
    }
  }
}

static void RemoveInconsistentProteinTitles (IteM i)
{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  VisitBioseqsInSep (sep, NULL, RemoveBioseqInconsistentProteinTitles);
  DeleteMarkedObjects(bfp->input_entityID, 0, NULL);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  Update ();
}

static ButtoN SqnPushButton (GrouP prnt, CharPtr title, BtnActnProc actn, BaseFormPtr bfp)

{
  ButtoN  b;

  b = PushButton (prnt, title, actn);
  SetObjectExtra (b, (Pointer) bfp, NULL);
  return b;
}

static void FixCapitalizationToolBtn (ButtoN b);

extern void BioseqViewFormToolBar (GrouP h)

{
  BaseFormPtr  bfp;
  GrouP        g;

  bfp = (BaseFormPtr) GetObjectExtra (h);
  if (bfp == NULL) return;
  g = HiddenGroup (h, 1, 0, NULL);
  if (indexerVersion)
  {
    SqnPushButton (g, "Auto_Def", AutoDefToolBtn, bfp);
    SqnPushButton (g, "Auto_Def Options", AutoDefOptionsToolBtn, bfp);
    if (useTaxon) {
      SqnPushButton (g, "Tax_Fix/Clean_Up", ToolBtn3, bfp);
    }
    SqnPushButton (g, "CDS", ToolBtn5, bfp);
    SqnPushButton (g, "rRNA", ToolBtn6, bfp);
    SqnPushButton (g, "misc_feat", ToolBtn9, bfp);
    SqnPushButton (g, "mRna_CDS", ToolBtn7, bfp);
    SqnPushButton (g, "ORF_Find", ToolBtn8, bfp);
    SqnPushButton (g, "SUC", ToolBtn10, bfp);
    SqnPushButton (g, "rRNA->DNA", RibosomalRNAToGenomicDNAToolBtn, bfp);
    SqnPushButton (g, "Fix Caps", FixCapitalizationToolBtn, bfp);
    SqnPushButton (g, "Remove DefLines", RemoveDefLinesToolBtn, bfp);
    SqnPushButton (g, "Trim Ns", TrimNsFromNucsToolBtn, bfp);
    SqnPushButton (g, "Find ASN.1", FindStringProcToolBtn, bfp);
    SqnPushButton (g, "Local ID->Src", ParseLocalIDToSourceQual, bfp);
    SqnPushButton (g, "Fix Local IDs", ResolveExistingLocalIDsToolBtn, bfp);
    SqnPushButton (g, "Group Explode", GroupExplodeToolBtn, bfp);
    SqnPushButton (g, "cit-sub-upd", ToolBtn16, bfp);
    SqnPushButton (g, "del_GBbck", ToolBtn13, bfp);
    SqnPushButton (g, "rem_prot_titles", ToolBtn17, bfp);
    SqnPushButton (g, "Validate", ToolBtn14, bfp);
    SqnPushButton (g, "Desktop", ToolBtn15, bfp);
  }
  else
  {
    SqnPushButton (g, "Save", ToolBtn1, bfp);
    SqnPushButton (g, "Auto_Def", ToolBtn2, bfp);
    if (useTaxon) {
      /* SqnPushButton (g, "Tax_Fix", ToolBtn3, bfp); */
      SqnPushButton (g, "Tax_Fix/Clean_Up", ToolBtn3, bfp);
    }
    SqnPushButton (g, "Def_Org", ToolBtn4, bfp);
    SqnPushButton (g, "CDS", ToolBtn5, bfp);
    SqnPushButton (g, "rRNA", ToolBtn6, bfp);
    SqnPushButton (g, "mRna_CDS", ToolBtn7, bfp);
    SqnPushButton (g, "ORF_Find", ToolBtn8, bfp);
    SqnPushButton (g, "misc_feat", ToolBtn9, bfp);
    SqnPushButton (g, "SUC", ToolBtn10, bfp);
    SqnPushButton (g, "sub_affil", ToolBtn11, bfp);
    SqnPushButton (g, "sub_add", ToolBtn12, bfp);
    SqnPushButton (g, "cit-sub-upd", ToolBtn16, bfp);
    SqnPushButton (g, "del_GBbck", ToolBtn13, bfp);
    SqnPushButton (g, "rem_prot_titles", ToolBtn17, bfp);
    SqnPushButton (g, "Validate", ToolBtn14, bfp);
    SqnPushButton (g, "Desktop", ToolBtn15, bfp);
  }
}

static void MakeToolBarWindow (IteM i)

{
  BaseFormPtr  bfp;
  ForM         f;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  f = MakeToolFormForBioseqView (bfp, BioseqViewFormToolBar);
  Show (f);
  Select (f);
}

static void DoNCCleanup (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  NC_Cleanup (bfp->input_entityID, NULL);

  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void DoSSECleanup (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  SeriousSeqEntryCleanup (sep, NULL, NULL);

  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void MarkProtTitlesProc (BioseqPtr bsp, Pointer userdata)

{
  ObjValNodePtr  ovp;
  SeqDescrPtr    sdp;

  if (bsp == NULL) return;
  if (! ISA_aa (bsp->mol)) return;

  for (sdp = bsp->descr; sdp != NULL; sdp = sdp->next) {
    if (sdp->choice == Seq_descr_title) {
      if (sdp->extended != 0) {
        ovp = (ObjValNodePtr) sdp;
        ovp->idx.deleteme = TRUE;
      }
    }
  }
}

static void DoRemoveAllProtTitles (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  VisitBioseqsInSep (sep, NULL, MarkProtTitlesProc);
  DeleteMarkedObjects (bfp->input_entityID, 0, NULL);

  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void MarkNucTitlesProc (BioseqPtr bsp, Pointer userdata)

{
  ObjValNodePtr  ovp;
  SeqDescrPtr    sdp;

  if (bsp == NULL) return;
  if (! ISA_na (bsp->mol)) return;

  for (sdp = bsp->descr; sdp != NULL; sdp = sdp->next) {
    if (sdp->choice == Seq_descr_title) {
      if (sdp->extended != 0) {
        ovp = (ObjValNodePtr) sdp;
        ovp->idx.deleteme = TRUE;
      }
    }
  }
}

static void DoRemoveAllNucTitles (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  VisitBioseqsInSep (sep, NULL, MarkNucTitlesProc);
  DeleteMarkedObjects (bfp->input_entityID, 0, NULL);

  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void MarkMrnaTitlesProc (BioseqPtr bsp, Pointer userdata)

{
  SeqMgrDescContext  dcontext;
  MolInfoPtr         mip;
  ObjValNodePtr      ovp;
  SeqDescrPtr        sdp;

  if (bsp == NULL) return;
  if (! ISA_na (bsp->mol)) return;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
  if (sdp == NULL || sdp->choice != Seq_descr_molinfo) return;
  mip = (MolInfoPtr) sdp->data.ptrvalue;
  if (mip == NULL) return;
  if (mip->biomol != MOLECULE_TYPE_MRNA) return;

  for (sdp = bsp->descr; sdp != NULL; sdp = sdp->next) {
    if (sdp->choice == Seq_descr_title) {
      if (sdp->extended != 0) {
        ovp = (ObjValNodePtr) sdp;
        ovp->idx.deleteme = TRUE;
      }
    }
  }
}

static void DoRemoveMrnaTitles (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  VisitBioseqsInSep (sep, NULL, MarkMrnaTitlesProc);
  DeleteMarkedObjects (bfp->input_entityID, 0, NULL);

  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void MarkNPSTitlesProc (BioseqSetPtr bssp, Pointer userdata)

{
  ObjValNodePtr  ovp;
  SeqDescrPtr    sdp;

  if (bssp == NULL) return;
  if (bssp->_class != BioseqseqSet_class_nuc_prot) return;

  for (sdp = bssp->descr; sdp != NULL; sdp = sdp->next) {
    if (sdp->choice == Seq_descr_title) {
      if (sdp->extended != 0) {
        ovp = (ObjValNodePtr) sdp;
        ovp->idx.deleteme = TRUE;
      }
    }
  }
}

static void DoRemoveNPSTitles (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  VisitSetsInSep (sep, NULL, MarkNPSTitlesProc);
  DeleteMarkedObjects (bfp->input_entityID, 0, NULL);

  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void DoPTCleanup (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  InstantiateProteinTitles (bfp->input_entityID, NULL);

  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void TakeProteinsFromGPS (BioseqPtr bsp, Pointer userdata)

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

static void StripMrnaProducts (SeqFeatPtr sfp, Pointer userdata)

{
  if (sfp == NULL || sfp->data.choice != SEQFEAT_RNA) return;
  if (sfp->product == NULL) return;

  sfp->product = SeqLocFree (sfp->product);
}

static void GPStoNPS (IteM i)

{
  BaseFormPtr   bfp;
  BioseqSetPtr  bssp;
  BioseqSet     dum;
  SeqEntryPtr   last, sep, top;
  Uint2         parenttype;
  Pointer       parentptr;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  top = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (top == NULL || top->choice != 2) return;
  bssp = (BioseqSetPtr) top->data.ptrvalue;
  if (bssp != NULL && bssp->_class == BioseqseqSet_class_genbank) {
    sep = bssp->seq_set;
    if (sep != NULL && IS_Bioseq_set (sep)) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
    }
  }
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

  VisitFeaturesInSet (bssp, NULL, StripMrnaProducts);

  move_cds (top);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  Update ();
}

static void CDStoNPS (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  move_cds (sep);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  Update ();
}

static void AddNewMrnaTitle (BioseqPtr bsp, Pointer userdata)

{
  BioSourcePtr       biop;
  SeqMgrFeatContext  ccontext;
  CharPtr            cdslabel = NULL;
  SeqMgrDescContext  dcontext;
  SeqMgrFeatContext  gcontext;
  CharPtr            genelabel = NULL;
  size_t             len;
  MolInfoPtr         mip;
  CharPtr            organism;
  OrgRefPtr          orp;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp;
  CharPtr            str;

  if (bsp == NULL) return;
  if (! ISA_na (bsp->mol)) return;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
  if (sdp == NULL || sdp->choice != Seq_descr_molinfo) return;
  mip = (MolInfoPtr) sdp->data.ptrvalue;
  if (mip == NULL) return;
  if (mip->biomol != MOLECULE_TYPE_MRNA) return;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
  if (sdp == NULL || sdp->choice != Seq_descr_source) return;
  biop = (BioSourcePtr) sdp->data.ptrvalue;
  if (biop == NULL) return;
  orp = biop->org;
  if (orp == NULL) return;
  organism = orp->taxname;

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

static void MrnaTitlesToGPS (IteM i)

{
  BaseFormPtr   bfp;
  BioseqSetPtr  bssp;
  SeqEntryPtr   sep, top;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  top = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (top == NULL || top->choice != 2) return;
  bssp = (BioseqSetPtr) top->data.ptrvalue;
  if (bssp != NULL && bssp->_class == BioseqseqSet_class_genbank) {
    sep = bssp->seq_set;
    if (sep != NULL && IS_Bioseq_set (sep)) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
    }
  }
  if (bssp == NULL || bssp->_class != BioseqseqSet_class_gen_prod_set) return;

  VisitBioseqsInSep (top, NULL, AddNewMrnaTitle);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  Update ();
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

static void SqnMakeNucProtCDS (
  BioseqSetPtr bssp,
  Pointer userdata
)

{
  CodeBreakPtr  cbp;
  SeqFeatPtr    cds;
  CdRegionPtr   crp;
  SeqFeatPtr    mrna;
  NpsSeqs       ns;
  Boolean       partial5, partial3;
  SeqFeatPtr    sfp;
  SeqIdPtr      sip;
  SeqLocPtr     slp;
  Int4          start, stop;
  SeqFeatPtr    temp;

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
}

/* copy gene from contig onto nuc-prot, single interval on cdna bioseq */

static void SqnCopyGene (
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

static void SqnAddMrnaTitles (
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

static Boolean LIBCALLBACK DummyACMProc (
  SeqFeatPtr sfp,
  SeqMgrFeatContextPtr context
)

{
  return TRUE;
}

static Boolean AmbiguousCdsMrna (BioseqPtr bsp)

{
  Int2               count;
  SeqMgrFeatContext  fcontext;
  SeqFeatPtr         sfp;

  if (bsp == NULL) return TRUE;

  sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_CDREGION, 0, &fcontext);
  while (sfp != NULL) {
    count = SeqMgrGetAllOverlappingFeatures (sfp->location, FEATDEF_mRNA, NULL, 0,
                                             CHECK_INTERVALS, NULL, DummyACMProc);
    if (count > 1) return TRUE;
    sfp = SeqMgrGetNextFeature (bsp, sfp, SEQFEAT_CDREGION, 0, &fcontext);
  }

  return FALSE;
}

static void InstantiateMrnaIntoProt (SeqFeatPtr cds, SeqFeatPtr mrna, Int2Ptr ctrp)

{
  ByteStorePtr  bs;
  Int4          len;
  BioseqPtr     mbsp, pbsp;
  MolInfoPtr    mip;
  SeqEntryPtr   msep, psep;
  Boolean       partial5, partial3;
  CharPtr       rnaseq;
  ValNodePtr    vnp;

  if (cds == NULL || mrna == NULL) return;
  if (cds->product == NULL || mrna->product != NULL) return;

  pbsp = BioseqFindFromSeqLoc (cds->product);
  if (pbsp  == NULL) return;
  psep = SeqMgrGetSeqEntryForData (pbsp);
  if (psep == NULL) return;

  rnaseq = GetSequenceByFeature (mrna);
  if (rnaseq == NULL) return;
  len = (Int4) StringLen (rnaseq);

  bs = BSNew (len + 2);
  if (bs == NULL) return;
  BSWrite (bs, (VoidPtr) rnaseq, len);
  MemFree (rnaseq);

  mbsp = BioseqNew ();
  if (mbsp == NULL) return;
  mbsp->repr = Seq_repr_raw;
  mbsp->mol = Seq_mol_rna;
  mbsp->seq_data_type = Seq_code_iupacna;
  mbsp->seq_data = bs;
  mbsp->length = BSLen (bs);
  BioseqPack (mbsp);

  mbsp->id = MakeNewProteinSeqIdEx (mrna->location, NULL, NULL, ctrp);
  CheckSeqLocForPartial (mrna->location, &partial5, &partial3);
  SeqMgrAddToBioseqIndex (mbsp);

  msep = SeqEntryNew ();
  if (msep == NULL) return;
  msep->choice = 1;
  msep->data.ptrvalue = (Pointer) mbsp;
  SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) mbsp, msep);

  mip = MolInfoNew ();
  if (mip == NULL) return;
  mip->biomol = MOLECULE_TYPE_MRNA;
  if (partial5 && partial3) {
    mip->completeness = 5;
  } else if (partial5) {
    mip->completeness = 3;
  } else if (partial3) {
    mip->completeness = 4;
  }
  vnp = CreateNewDescriptor (msep, Seq_descr_molinfo);
  if (vnp != NULL) {
    vnp->data.ptrvalue = (Pointer) mip;
  }

  SetSeqFeatProduct (mrna, mbsp);

  AddSeqEntryToSeqEntry (psep, msep, FALSE);
}

static void NPStoGPS (IteM i)

{
  BaseFormPtr        bfp;
  BioSourcePtr       biop;
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  Int2               ctr = 1;
  SeqMgrFeatContext  fcontext, mcontext;
  SeqFeatPtr         mrna, sfp, lastsfp;
  SeqEntryPtr        old, sep, top;
  CharPtr            organism;
  OrgRefPtr          orp;
  Uint2              parenttype;
  Pointer            parentptr;
  SeqAnnotPtr        sap, annot, lastsap;
  SeqDescrPtr        sdp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  top = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (top == NULL || top->choice != 2) return;
  bssp = (BioseqSetPtr) top->data.ptrvalue;
  if (bssp != NULL && bssp->_class == BioseqseqSet_class_genbank) {
    sep = bssp->seq_set;
    if (sep != NULL && IS_Bioseq_set (sep)) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
    }
  }
  if (bssp == NULL || bssp->_class != BioseqseqSet_class_nuc_prot) return;

  bsp = FindNucBioseq (top);
  if (bsp == NULL) {
    Message (MSG_OK, "Unable to find nucleotide Bioseq");
    return;
  }

  if (AmbiguousCdsMrna (bsp)) {
    Message (MSG_OK, "Unable to proceed because of unresolved CDS-mRNA ambiguity");
    return;
  }

  GetSeqEntryParent (top, &parentptr, &parenttype);

  bssp->_class = BioseqseqSet_class_gen_prod_set;

  sap = bssp->annot;
  bssp->annot = NULL;
  annot = bsp->annot;
  if (annot == NULL) {
    bsp->annot = sap;
  } else if (sap->next == NULL && annot->next == NULL &&
             sap->type == 1 && annot->type == 1 &&
             sap->data != NULL && annot->data != NULL) {
    lastsfp = (SeqFeatPtr) annot->data;
    while (lastsfp->next != NULL) {
      lastsfp = lastsfp->next;
    }
    sfp = (SeqFeatPtr) sap->data;
    lastsfp->next = sfp;
    sap->data = NULL;
    SeqAnnotFree (sap);
  } else {
    lastsap = bsp->annot;
    while (lastsap->next != NULL) {
      lastsap = lastsap->next;
    }
    lastsap->next = sap;
  }

  old = SeqEntrySetScope (top);

  ctr = (Int2) VisitBioseqsInSep (top, NULL, NULL) + 1;

  sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_CDREGION, 0, &fcontext);
  while (sfp != NULL) {
    mrna = SeqMgrGetOverlappingFeature (sfp->location, FEATDEF_mRNA, NULL, 0,
                                        NULL, CHECK_INTERVALS, &mcontext);
    if (mrna != NULL) {
      InstantiateMrnaIntoProt (sfp, mrna, &ctr);
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, SEQFEAT_CDREGION, 0, &fcontext);
  }

  SeqMgrLinkSeqEntry (top, parenttype, parentptr);

  SeqEntrySetScope (old);

  SeqMgrClearFeatureIndexes (bssp->idx.entityID, NULL);

  /* need to reindex to get mRNA and CDS features from cDNA and protein */
  SeqMgrIndexFeatures (bssp->idx.entityID, NULL);
  VisitSetsInSet (bssp, NULL, SqnMakeNucProtCDS);

  /* need to reindex before copying genes, instantiating protein titles */
  SeqMgrIndexFeatures (bssp->idx.entityID, NULL);

  VisitFeaturesInSep (top, NULL, SqnCopyGene);

  /* need to reindex before instantiating mRNA titles */
  SeqMgrIndexFeatures (bssp->idx.entityID, NULL);

  organism = NULL;
  for (sdp = bssp->descr; sdp != NULL; sdp = sdp->next) {
    if (sdp->choice != Seq_descr_source) continue;
    biop = (BioSourcePtr) sdp->data.ptrvalue;
    if (biop == NULL) continue;
    orp = biop->org;
    if (orp == NULL) continue;
    if (! StringHasNoText (orp->taxname)) {
      organism = orp->taxname;
    }
  }

  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, FEATDEF_mRNA, &mcontext);
  while (sfp != NULL) {
    SqnAddMrnaTitles (sfp->product, organism);
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, FEATDEF_mRNA, &mcontext);
  }

  SeqMgrClearFeatureIndexes (bssp->idx.entityID, NULL);

  move_cds (top);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  Update ();
}

static void MakePhrap (IteM i)

{
  BaseFormPtr  bfp;
  BioseqPtr    bsp;
  SeqAnnotPtr  sap;
  SeqEntryPtr  sep, nsep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  nsep = FindNucSeqEntry (sep);
  if (nsep == NULL) return;
  bsp = (BioseqPtr) nsep->data.ptrvalue;
  if (bsp == NULL) return;
  sap = PhrapGraphForContig (bsp);
  if (sap == NULL) return;
  sap->next = bsp->annot;
  bsp->annot = sap;
  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

typedef struct marksegfeatsdata {
  SeqLocPtr seg_loc;
  CdsDelStructPtr   cdsDelInfoPtr;
} MarkSegFeatData, PNTR MarkSegFeatPtr;
  
static Boolean LIBCALLBACK MarkSegmentFeatures (SeqFeatPtr sfp,
					       SeqMgrFeatContextPtr scontext)
{
  MarkSegFeatPtr msfp;

  if (sfp == NULL || sfp->idx.subtype != FEATDEF_CDS) return TRUE;
  msfp = scontext->userdata;
  if (msfp == NULL || msfp->seg_loc == NULL
    || msfp->cdsDelInfoPtr == NULL
    || sfp->location == NULL)
  {
    return TRUE;
  }
  if (SeqLocAinB (sfp->location, msfp->seg_loc) >=0)
  {
    sfp->idx.deleteme = TRUE;
    msfp->cdsDelInfoPtr->dirtyFlag = TRUE;
  }
  return TRUE;
} 

/*=====================================================================*/
/*                                                                     */
/* RemoveCDS_Callback () - Called for each segment explored in         */
/*                         RemoveNthCDSfromSegSets_Callback(), this    */
/*                         removes the first CDS of a segment.         */
/*                                                                     */
/*=====================================================================*/

static Boolean LIBCALLBACK RemoveCDS_Callback (SeqLocPtr slp,
					       SeqMgrSegmentContextPtr scontext)
{
  BioseqPtr         bsp;
  MarkSegFeatData   msf;

  /* If not the Nth segment, skip */

  msf.cdsDelInfoPtr = (CdsDelStructPtr) scontext->userdata;
  if (scontext->index != msf.cdsDelInfoPtr->number)
    return TRUE;

  /* Else, delete its CDS */

  else {

    /* Mark the coding regions for this segment */
    bsp = BioseqFindFromSeqLoc (slp);
    msf.seg_loc = slp;
    SeqMgrExploreFeatures (bsp, (Pointer) &msf, MarkSegmentFeatures, NULL, NULL, NULL);
    
    /* We only want to remove a CDS from the Nth   */
    /* segment so stop the exploring at this point */
    
    return FALSE;
  }
}

/*=====================================================================*/
/*                                                                     */
/* RemoveNthCDS() - Remove the CDS from the first segment of a given   */
/*                  segmented sequence.                                */
/*                                                                     */
/*=====================================================================*/

static void RemoveNthCDS (SeqEntryPtr      sep,
			  CdsDelStructPtr  cdsDelInfoPtr)
{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int4          segCount;

  /* If the SeqEntry is a Bioseq, then remove */
  /* the Nth CDS from each segment.           */

  if (IS_Bioseq(sep))
    {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (bsp->repr == Seq_repr_seg) 
	segCount = SeqMgrExploreSegments (bsp, cdsDelInfoPtr,
					  RemoveCDS_Callback);
    }

  /* If we have a Bioseq set, then recursively call */
  /* this function for each member of the set.      */

  else
    {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next)
    	RemoveNthCDS (sep, cdsDelInfoPtr);
    }

}

/*=====================================================================*/
/*                                                                     */
/* RemoveNthCDS_AcceptCallback () --                                   */
/*                                                                     */
/*=====================================================================*/

static void RemoveNthCDS_AcceptCallback (ButtoN b)
{
  CdsDelFormPtr cdfp;
  SeqEntryPtr   sep;
  Uint2         entityID;
  CdsDelStruct  cdsDelInfo;

  /* Make sure that conditions are right */

  cdfp = GetObjectExtra (b);
  if (NULL == cdfp) {
    Remove (ParentWindow (b));
    Update ();
    return;
  }

  if (cdfp->number == 0) {
    Remove (ParentWindow (b));
    MemFree (cdfp);
    Update ();
    return;
  }

  /* Get the top level SeqEntry */

  sep = GetTopSeqEntryForEntityID (cdfp->input_entityID);
  if (sep == NULL) {
    Remove (ParentWindow (b));
    MemFree (cdfp);
    Update ();
    return;
  }

  /* Remove CDS from segments */

  WatchCursor ();
  Update ();

  cdsDelInfo.dirtyFlag = FALSE;
  cdsDelInfo.number    = cdfp->number;

  RemoveNthCDS (sep, &cdsDelInfo);

  /* If any CDS were marked for deletion */
  /* then delete them and update.        */

  if (TRUE == cdsDelInfo.dirtyFlag)
    {
      entityID = ObjMgrGetEntityIDForChoice (sep);
      DeleteMarkedObjects (entityID, 0, NULL);
      Update ();
      ObjMgrSetDirtyFlag (entityID, TRUE);
      ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
    }

  /* Update the display */

  Remove (ParentWindow (b));
  MemFree (cdfp);
  ArrowCursor ();
  Update ();
}

/*=====================================================================*/
/*                                                                     */
/* GetN_Callback () --                                                 */
/*                                                                     */
/*=====================================================================*/

static void GetN_Callback (TexT nText)

{
  CdsDelFormPtr cdfp;
  CharPtr       nStr;

  cdfp = (CdsDelFormPtr) GetObjectExtra (nText);
  if (cdfp == NULL)
    return;

  nStr = SaveStringFromText (nText);
  cdfp->number = atoi (nStr);

}

/*=====================================================================*/
/*                                                                     */
/* CreateGetNWindow () --                                              */
/*                                                                     */
/*=====================================================================*/

static ForM CreateGetNWindow (CdsDelFormPtr cdfp)
{
  /*
  FindFormPtr        ffp;
  GrouP              q = NULL;
  */

  ButtoN             b;
  GrouP              c;
  GrouP              g;
  GrouP              j;
  StdEditorProcsPtr  sepp;
  WindoW             w;
  TexT               nthText;

  w = ModalWindow (-50, -30, -10, -10, StdCloseWindowProc);
  cdfp->form = (ForM) w;

#ifndef WIN_MAC
  CreateStdEditorFormMenus (w);
#endif

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
  }
  
  j = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (j, 10, 10);
  
  g = HiddenGroup (j, 2, 0, NULL);
  StaticPrompt (g, "CDS to Delete", 0, dialogTextHeight, programFont, 'l');
  nthText = DialogText (g, "", 6, (TxtActnProc) GetN_Callback);
  SetObjectExtra (nthText, cdfp, NULL);
  
  c = HiddenGroup (w, 4, 0, NULL);
  SetGroupSpacing (c, 10, 2);
  b = DefaultButton (c, "Accept", RemoveNthCDS_AcceptCallback);
  SetObjectExtra (b, cdfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) j, (HANDLE) c, NULL);
  
  RealizeWindow (w);
  
  return (ForM) w;
}

/*=====================================================================*/
/*                                                                     */
/* RemoveNthCDSFromSegSets_Callback() - Remove the CDS from the first  */
/*                                      segment of all Bioseqs.        */
/*                                                                     */
/*=====================================================================*/

static void RemoveNthCDSFromSegSets_Callback (IteM i)
{
  ForM          w;
  CdsDelFormPtr cdfp;
  BaseFormPtr   bfp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL)
    return;

  cdfp = (CdsDelFormPtr) MemNew (sizeof (CdsDelForm));

  /* Find out which segments to delete the CDS from */

  cdfp->input_entityID = bfp->input_entityID;
  w = CreateGetNWindow (cdfp);
  Show (w);
  Select (w);
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemoveSeqIdName_FeatureCallback () --                               */
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean LIBCALLBACK RemoveSeqIdName_FeatureCallback
                               (SeqFeatPtr sfp,
				SeqMgrFeatContextPtr fcontext)
{
  SeqIdPtr     sip;
  TextSeqIdPtr tsip;

  /* If we don't have a product then */
  /* go on to the next seq feat.     */

  if (sfp->product == NULL)
    return TRUE;
  
  /* Otherwise check the id name field */

  sip = SeqLocId (sfp->product);
  if (sip != NULL) {
      switch (sip->choice) {
        case SEQID_LOCAL :
	  break;
        case SEQID_GENBANK :
        case SEQID_EMBL :
        case SEQID_DDBJ :
        case SEQID_OTHER :
        case SEQID_TPG :
        case SEQID_TPE :
        case SEQID_TPD :
          tsip = (TextSeqIdPtr) sip->data.ptrvalue;
          if ((tsip != NULL) &&
	      (tsip->accession == NULL) &&
	      (! StringHasNoText (tsip->name)) &&
	      (IsNuclAcc (tsip->name)))
	    tsip->name[0] = '\0';
          break;
        default :
          break;
      }
    }

  /* Return TRUE to continue on to the next feature */

  return TRUE;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemoveSeqIdName_BioseqCallback () --                                */
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean LIBCALLBACK RemoveSeqIdName_BioseqCallback (BioseqPtr bsp,
					 SeqMgrBioseqContextPtr bcontext)
{
  TextSeqIdPtr tsip;

  /* If it is a protein Bioseq then */
  /* check the ID name field.       */

  if (ISA_aa (bsp->mol))
    {
      if (bsp->id != NULL) {
	switch (bsp->id->choice)
	  {
          case SEQID_GENBANK :
          case SEQID_EMBL :
          case SEQID_DDBJ :
          case SEQID_OTHER :
          case SEQID_TPG :
	  case SEQID_TPE :
	  case SEQID_TPD :
	    tsip = (TextSeqIdPtr) bsp->id->data.ptrvalue;
	    if ((tsip != NULL) &&
		(tsip->accession == NULL) &&
		(! StringHasNoText (tsip->name)) &&
		(IsNuclAcc (tsip->name)))
	      {
		tsip->name[0] = '\0';
	      }
	    break;
	  default :
	    break;
	  }
      }
    }

  /* Else for nucl bioseqs check the name */
  /* field of the protein sfps.           */

  else
    SeqMgrExploreFeatures (bsp, NULL, RemoveSeqIdName_FeatureCallback,
			   NULL, NULL, NULL);

  /* Return TRUE to continue on to the next Bioseq */

  return TRUE;
}

/*=====================================================================*/
/*                                                                     */
/* RemoveSeqIdName_Callback() - Remove the contents of the seq-id name */
/*                              field in the cases where there is no   */
/*                              corresponding GI number.               */
/*                                                                     */
/*=====================================================================*/

static void RemoveSeqIdName_Callback (IteM i)
{
  BaseFormPtr   bfp;
  SeqEntryPtr   sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL)
    return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL)
    return;

  SeqMgrExploreBioseqs (bfp->input_entityID, NULL, bfp,
			RemoveSeqIdName_BioseqCallback,
			TRUE, TRUE, TRUE);

  /* Force an update and redraw */

  DeleteMarkedObjects (bfp->input_entityID, 0, NULL);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();

  /* Return successfully */

  return;

}

static void ReverseNameOrderInAuthor (AuthorPtr pAuthor)
{
  NameStdPtr pNameStandard;
  NameStdPtr pNewName;
  CharPtr	first_name;
  CharPtr	middle_initial;
  CharPtr	last_name;
  CharPtr	suffix;
  Char		str[128];
  CharPtr	pTmp;

  if (pAuthor == NULL)
    return;
  else if(pAuthor->name->choice != 2)
    return;
  pNameStandard = pAuthor->name->data;
  if (pNameStandard != NULL)
  {
    pTmp = NameStdPtrToAuthorSpreadsheetString(pNameStandard);
    first_name = ExtractTagListColumn(pTmp, 2);
    middle_initial = ExtractTagListColumn(pTmp, 1);
    last_name = ExtractTagListColumn(pTmp, 0);
    suffix = ExtractTagListColumn(pTmp, 3);
    sprintf(str, "%s\t%s\t%s\t%s\n",
	first_name, middle_initial, last_name, suffix);
    MemFree(first_name);
    MemFree(middle_initial);
    MemFree(last_name);
    MemFree(suffix);
    pNewName = AuthorSpreadsheetStringToNameStdPtr(str);
    NameStdFree(pNameStandard);
    pAuthor->name->data = pNewName;
  }

}

typedef struct replaceitempair {
  CharPtr FindString;
  CharPtr ReplaceString;
} ReplaceItemPair, PNTR ReplaceItemPairPtr;


ReplaceItemPair AbbreviationList[] = {
 { "arabidopsis thaliana", "Arabidopsis thaliana" },
 { "adp", "ADP" },
 { "adp-", "ADP-" },
 { "atp", "ATP" },
 { "atp-", "ATP-" },
 { "bac", "BAC" },
 { "caenorhabditis elegans", "Caenorhabditis elegans" },
 { "cdna", "cDNA" },
 { "coi", "COI" },
 { "coii", "COII" },
 { "danio rerio", "Danio rerio" },
 { "dna", "DNA" },
 { "dna-", "DNA-" },
 { "drosophila melanogaster", "Drosophila melanogaster" },
 { "dsrna", "dsRNA" },
 { "escherichia coli", "Escherichia coli" },
 { "hiv", "HIV" },
 { "hiv-1", "HIV-1" },
 { "hiv-2", "HIV-2" },
 { "hnrna", "hnRNA" },
 { "homo sapiens", "Homo sapiens" },
 { "mhc", "MHC" },
 { "mrna", "mRNA" },
 { "mtdna", "mtDNA" },
 { "mus musculus", "Mus musculus" },
 { "nadh", "NADH" },
 { "nov.", "nov." },
 { "nov..", "nov.." },
 { "pcr", "PCR" },
 { "rattus norvegicus", "Rattus norvegicus" },
 { "rdna", "rDNA" },
 { "rna", "RNA" },
 { "rna-", "RNA-" },
 { "rrna", "rRNA" },
 { "rt-pcr", "RT-PCR" },
 { "saccharomyces cerevisiae", "Saccharomyces cerevisiae" },
 { "scrna", "scRNA" },
 { "siv-1", "SIV-1" },
 { "snrna", "snRNA" },
 { "sp.", "sp." },
 { "sp..", "sp.." },
 { "ssp.", "ssp." },
 { "ssp..", "ssp.." },
 { "ssrna", "ssRNA" },
 { "subsp.", "subsp." },
 { "subsp..", "subsp.." },
 { "trna", "tRNA" },
 { "trna-", "tRNA-" },
 { "var.", "var." },
 { "var..", "var.." },
 { "usa", "USA" },
 {"(hiv)", "(HIV)" },
 {"(hiv1)", "(HIV1)" },
 {"(hiv-1)", "(HIV-1)" }
};

ReplaceItemPair SpecialAbbreviationList[] = {
 { "sp.", "sp." },
 { "nov.", "nov." },
 { "ssp.", "ssp." },
 { "var.", "var." },
 { "subsp.", "subsp." }
};

static void FixAbbreviationsInElement (CharPtr PNTR pEl)
{
  int i;
  CharPtr NewPtr;
  Boolean whole_word;

  if (pEl == NULL) return;
  if (*pEl == NULL) return;

  for (i = 0; i < sizeof (AbbreviationList) / sizeof (ReplaceItemPair); i++)
  {
    if (AbbreviationList[i].FindString[StringLen (AbbreviationList[i].FindString) - 1] == '-')
    {
      whole_word = FALSE;
    }
    else
    {
      whole_word = TRUE;
    }
    FindReplaceString (pEl, AbbreviationList[i].FindString,
			AbbreviationList[i].ReplaceString, FALSE, whole_word);
  }
  for (i = 0; i < sizeof (SpecialAbbreviationList) / sizeof (ReplaceItemPair); i++)
  {
    FindReplaceString (pEl, SpecialAbbreviationList[i].FindString,
			SpecialAbbreviationList[i].ReplaceString, FALSE, TRUE);
    if (StringLen (*pEl) >= StringLen (SpecialAbbreviationList[i].ReplaceString)
      && StringCmp ((*pEl) + StringLen (*pEl) - StringLen (SpecialAbbreviationList[i].ReplaceString), SpecialAbbreviationList[i].ReplaceString) == 0)
    {
      NewPtr = MemNew (StringLen (*pEl) + 2);
      if (NewPtr == NULL) return;
      StringCpy (NewPtr, *pEl);
      StringCat (NewPtr, ".");
      MemFree (*pEl);
      *pEl = NewPtr;
    }
  }
}

ReplaceItemPair ShortWordList[] = {
 { "A", "a" },
 { "About", "about" },
 { "And", "and" },
 { "At", "at" },
 { "But", "but" },
 { "By", "by" },
 { "In", "in" },
 { "Is", "in" },
 { "Of", "of" },
 { "Or", "or" },
 { "The", "the" },
 { "To", "to" },
 { "With", "with" }
};

static void FixShortWordsInElement (CharPtr PNTR pEl)
{
  Int2 i;

  if (pEl == NULL) return;
  if (*pEl == NULL) return;

  for (i = 0; i < sizeof (ShortWordList) / sizeof (ReplaceItemPair); i++)
  {
    FindReplaceString (pEl, ShortWordList[i].FindString,
			ShortWordList[i].ReplaceString, FALSE, TRUE);
  }
  if (isalpha ((Int4)((*pEl)[0])))
  {
    (*pEl)[0] = toupper ((*pEl)[0]);
  }
}

static void 
FixCapitalizationInElement 
(CharPtr PNTR pEl,
 Boolean      bAbbrev, 
 Boolean      bShortWords,
 Boolean      bApostrophes)
{
  CharPtr pCh;
  Boolean bSendToLower;
 
  if(pEl == NULL) return; 
  if(*pEl == NULL) return; 

  bSendToLower = FALSE;
  for(pCh = *pEl; *pCh != 0; pCh++)
  {
    if(isalpha((Int4)(*pCh)))
    {
      if(bSendToLower)
      {
        *pCh = tolower(*pCh);
      }
      else
      {
        *pCh = toupper(*pCh);
        bSendToLower = TRUE;
      }
    }
    else if (bApostrophes || *pCh != '\'')
    {
      bSendToLower = FALSE;
    }
  }
  if (bShortWords)
    FixShortWordsInElement (pEl);
  if (bAbbrev)
    FixAbbreviationsInElement (pEl);
}

static void FixCapitalizationInTitle (CharPtr PNTR pTitle, Boolean first_is_upper)
{
  CharPtr pCh;
  Boolean was_digit = FALSE;

  if (pTitle == NULL) return;
  pCh = *pTitle;
  if (pCh == NULL) return;
  if (*pCh == '\0') return;
  
  if (first_is_upper)
  {
    /* Set first character to upper */
    *pCh = toupper (*pCh);	
  }
  else
  {
    /* set first character to lower */
    *pCh = tolower (*pCh);	
  }
  
  if (isdigit ((Int4)(*pCh)))
  {
  	was_digit = TRUE;
  }
  pCh++;
  /* Set rest of characters to lower */
  while (*pCh != '\0')
  {
    if (was_digit 
        && (*pCh == 'S' || *pCh == 's') 
        && (isspace ((Int4)(*(pCh + 1))) || *(pCh + 1) == 0))
    {
      *pCh = toupper (*pCh);
      was_digit = FALSE;
    }
    else if (isdigit ((Int4)(*pCh)))
    {
      was_digit = TRUE;
    }
    else
    {
      was_digit = FALSE;
      *pCh = tolower (*pCh);
    }
    pCh++;
  }
  FixAbbreviationsInElement (pTitle);
}

static void FixCapitalizationInAuthor (AuthorPtr pAuthor)
{
  NameStdPtr pNameStandard;
  CharPtr    cp;
  
  if (pAuthor == NULL)
    return;
  else if(pAuthor->name->choice != 2)
    return;
  pNameStandard = pAuthor->name->data;
  if (pNameStandard != NULL)
  {
    FixCapitalizationInElement (&(pNameStandard->names[0]), FALSE, FALSE, TRUE);
    FixCapitalizationInElement (&(pNameStandard->names[1]), FALSE, FALSE, FALSE);
    /* Set initials to all caps */
    for (cp = pNameStandard->names[4]; cp != NULL && *cp != 0; cp++)
    {
      *cp = toupper (*cp);
    }
  }
}

static void StripSuffixFromAuthor (AuthorPtr pAuthor)
{
  NameStdPtr pNameStandard;
  
  if (pAuthor == NULL)
    return;
  else if(pAuthor->name->choice != 2)
    return;
  pNameStandard = pAuthor->name->data;
  if (pNameStandard != NULL && pNameStandard->names[5] != NULL)
  {
    pNameStandard->names[5][0] = 0;
  }
}

static void TruncateAuthorMiddleInitials (AuthorPtr pAuthor)
{
  NameStdPtr pNameStandard;
  
  if (pAuthor == NULL)
    return;
  else if(pAuthor->name->choice != 2)
    return;
  pNameStandard = pAuthor->name->data;
  if (pNameStandard != NULL)
  {
    if (pNameStandard->names[4] != NULL
      && StringLen (pNameStandard->names[4]) > 4)
    {
      pNameStandard->names[4][4] = 0;
      pNameStandard->names[4][3] = '.';
    }
  }
}

static void ChangeAuthorLastNameToConsortium (AuthorPtr pAuthor)
{
  NameStdPtr pNameStandard;
  CharPtr    consortium;
  Int4       len;
  MsgAnswer  ans = ANS_YES;
  
  if (pAuthor == NULL || pAuthor->name == NULL || pAuthor->name->choice == 5)
  {
  	return;
  }
  if (pAuthor->name->choice == 2)
  {
    pNameStandard = pAuthor->name->data;
    if (pNameStandard != NULL)
    {
      len = StringLen (pNameStandard->names[0]);
      if (len > 0)
      {
        if (StringISearch (pNameStandard->names[0], "consortium") == NULL)
        {
          ans = Message (MSG_YN, 
                         "'%s' does not contain the word consortium - are you sure you want to convert?",
                         pNameStandard->names [0]);
        }
        if (ans == ANS_NO) return;
  	    consortium = (CharPtr) MemNew ((len + 1) * sizeof (Char));
  	    if (consortium != NULL)
  	    {
  	      StringCpy (consortium, pNameStandard->names[0]);
  	      NameStdFree (pNameStandard);
  	      pAuthor->name->choice = 5;
  	      pAuthor->name->data = consortium;
  	    }
      }
    }  
  }
}


#define FIX_AUTHOR_NAME_ORDER	          1
#define FIX_PUB_AUTHOR_CAPITALIZATION	  2
#define FIX_PUB_TITLE_CAPITALIZATION	  4
#define FIX_PUB_AFFIL_CAPITALIZATION	  8
#define FIX_SELECTED			              16
#define FIX_ALL                         32
#define STRIP_AUTHOR_SUFFIX             64
#define FIX_PUB_SWAP_NAME_CONSORTIUM    128
#define TRUNCATE_AUTHOR_MIDDLE_INITIALS 256

static void FixPubdesc (PubdescPtr pdp, Pointer userdata)
{
  ValNodePtr	vnp;
  AuthListPtr 	alp;
  CitArtPtr    cap;
  CitBookPtr   cbp;
  CitGenPtr    cgp;
  CitPatPtr    cpp;
  CitSubPtr    csp;
  ValNodePtr	names;
  AuthorPtr	ap;
  CharPtr PNTR	title;
  Int4	iType;
  AffilPtr	affil;

  iType = (Int4) userdata;

  if (pdp == NULL) return;
  /* search for PUB_PMid or PUB_Muid - if found, do not edit this pub */
  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == PUB_PMid || vnp->choice == PUB_Muid) return;
  }
  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == PUB_PMid || vnp->choice == PUB_Muid) continue;
    if (vnp->data.ptrvalue == NULL) continue;
    alp = NULL;
    title = NULL;
    switch (vnp->choice) {
      case PUB_Gen :
        cgp = (CitGenPtr) vnp->data.ptrvalue;   
        if( !(cgp->cit != NULL && StringCmp(cgp->cit, "unpublished") != 0)
  	      && !((iType & FIX_SELECTED) || (iType & FIX_ALL)) ) continue;
        alp = cgp->authors;
        title = &(cgp->title);
        break;
      case PUB_Sub :
        csp = (CitSubPtr) vnp->data.ptrvalue;
        alp = csp->authors;
        title = NULL;
        break;
      case PUB_Article :
        if ( !((iType & FIX_SELECTED) || (iType & FIX_ALL)) ) continue;
        cap = (CitArtPtr) vnp->data.ptrvalue;
        alp = cap->authors;
        if(cap->title != NULL) {
          title = (CharPtr PNTR) &(cap->title->data.ptrvalue);
        }
        break;
      case PUB_Book :
      case PUB_Man :
        if ( !((iType & FIX_SELECTED) || (iType & FIX_ALL)) ) continue;
        cbp = (CitBookPtr) vnp->data.ptrvalue;
        alp = cbp->authors;
        if(cbp->title != NULL) {
          title = (CharPtr PNTR) &(cbp->title->data.ptrvalue);
        }
        break;
      case PUB_Patent :
        if ( !((iType & FIX_SELECTED) || (iType & FIX_ALL)) ) continue;
        cpp = (CitPatPtr) vnp->data.ptrvalue;
        alp = cpp->authors;
        title = &(cpp->title);
        break;
      default :
        break;
    }

    if(iType & FIX_PUB_TITLE_CAPITALIZATION ) {
      FixCapitalizationInTitle (title, TRUE);
    }

    if(alp == NULL) continue;

    /* Loop through author list */
    for (names = alp->names; names != NULL; names = names->next) { 
      ap = names->data.ptrvalue;
      if(iType & FIX_PUB_AUTHOR_CAPITALIZATION ) {
        FixCapitalizationInAuthor (ap);
      }
      if(iType & FIX_AUTHOR_NAME_ORDER ) {
        ReverseNameOrderInAuthor (ap);
      }
      if (iType & TRUNCATE_AUTHOR_MIDDLE_INITIALS)
      {
        TruncateAuthorMiddleInitials (ap);
      }
      if (iType & STRIP_AUTHOR_SUFFIX) {
        StripSuffixFromAuthor (ap);
      }
      if (iType & FIX_PUB_SWAP_NAME_CONSORTIUM)
      {
      	ChangeAuthorLastNameToConsortium (ap);
      }
    }

    if (iType & FIX_PUB_AFFIL_CAPITALIZATION ) {
      affil = alp->affil;
      if (affil == NULL) continue;
      FixCapitalizationInElement (&(affil->affil), TRUE, TRUE, FALSE);
      FixCapitalizationInElement (&(affil->div), TRUE, TRUE, FALSE);
      FixCapitalizationInElement (&(affil->city), FALSE, TRUE, FALSE);

      /* special handling for states */
      if (affil->sub != NULL && StringLen (affil->sub) == 2
	      && isalpha((Int4)(affil->sub[0]))	&& isalpha((Int4)(affil->sub[1])))
      {
        affil->sub[0] = toupper(affil->sub[0]);
        affil->sub[1] = toupper(affil->sub[1]);
      } else {
        FixCapitalizationInElement (&(affil->sub), FALSE, TRUE, FALSE);
      }
      FixCapitalizationInElement (&(affil->country), TRUE, TRUE, FALSE);
      FixCapitalizationInElement (&(affil->street), FALSE, TRUE, FALSE);
    }
  }
}

/* This function is used to apply fixes to citations */
static void FixPubs (BaseFormPtr bfp, Int4 iType)
{
  SeqEntryPtr  sep;
  SelStructPtr	sel;
  SeqFeatPtr	sfp;
  SeqMgrFeatContext fcontext;
  SeqDescPtr	sdp;
  SeqMgrDescContext dcontext;
  PubdescPtr	pdp;

  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (iType & FIX_ALL)
  {
    sel = NULL;
  }
  else
  {
    sel = ObjMgrGetSelected ();
  }
  if(sel == NULL)
  {
    VisitPubdescsInSep (sep, (Pointer)iType, FixPubdesc);
  }
  else
  {
    while( sel != NULL )
    {
      pdp = NULL;
      if(sel->entityID == bfp->input_entityID)
      {
        if(sel->itemtype == OBJ_SEQFEAT) 
        {
          sfp = SeqMgrGetDesiredFeature (bfp->input_entityID, NULL, sel->itemID, 0, NULL, &fcontext);
          if(sfp != NULL && sfp->data.choice == SEQFEAT_PUB)
          {
            pdp = sfp->data.value.ptrvalue;
          }
        }
        else if(sel->itemtype == OBJ_SEQDESC)
        {
          sdp = SeqMgrGetDesiredDescriptor (bfp->input_entityID, NULL, sel->itemID, 0, NULL, &dcontext);
          if(sdp != NULL && sdp->choice == Seq_descr_pub)
          {
            pdp = sdp->data.ptrvalue;
          }
        }
      } 
      if (pdp != NULL) FixPubdesc (pdp, (Pointer) (iType | FIX_SELECTED));
      sel = sel->next;      
    }
  }

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  Update ();
}

static void FixPubsMenuItem (IteM i, Int4 iType)
{
  BaseFormPtr bfp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  if (bfp == NULL) return;
  FixPubs (bfp, iType);
}

static void FixCapitalizationToolBtn (ButtoN b)
{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;
  FixPubs (bfp, FIX_PUB_AUTHOR_CAPITALIZATION
              | FIX_PUB_TITLE_CAPITALIZATION
              | FIX_PUB_AFFIL_CAPITALIZATION
              | FIX_ALL);
}

static void FixNameOrder (IteM i)
{
  FixPubsMenuItem (i, FIX_AUTHOR_NAME_ORDER);
}

static void FixAuthorCapitalization (IteM i)
{
  FixPubsMenuItem (i, FIX_PUB_AUTHOR_CAPITALIZATION);
}

static void TruncateAuthorMiddle (IteM i)
{
  FixPubsMenuItem (i, TRUNCATE_AUTHOR_MIDDLE_INITIALS);
}

static void FixTitleCapitalization (IteM i)
{
  FixPubsMenuItem (i, FIX_PUB_TITLE_CAPITALIZATION);
}

static void FixAffiliationCapitalization (IteM i)
{
  FixPubsMenuItem (i, FIX_PUB_AFFIL_CAPITALIZATION);
}

static void StripAuthorSuffix (IteM i)
{
  FixPubsMenuItem (i, STRIP_AUTHOR_SUFFIX);
}

static void ChangeAuthorNameToConsortium (IteM i)
{
  FixPubsMenuItem (i, FIX_PUB_SWAP_NAME_CONSORTIUM);
}

static void FixProductCapitalizationCallback (SeqFeatPtr sfp, Pointer userdata)
{
  ProtRefPtr prp;
  RnaRefPtr  rrp;
  ValNodePtr vnp;
  CharPtr    product_name;
  
  if (sfp == NULL) return;
  if (sfp->data.choice != SEQFEAT_PROT && sfp->data.choice != SEQFEAT_RNA) return;
  
  if (sfp->data.choice == SEQFEAT_PROT)
  {
    prp = (ProtRefPtr) sfp->data.value.ptrvalue;
    if (prp != NULL)
    {
      for (vnp = prp->name; vnp != NULL; vnp = vnp->next)
      {
        product_name = (CharPtr)(vnp->data.ptrvalue);
        FixCapitalizationInTitle (&product_name, FALSE);
        vnp->data.ptrvalue = product_name;
      }
    }
  }
  else if (sfp->data.choice == SEQFEAT_RNA)
  {
    rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
    if (rrp != NULL && rrp->ext.choice == 1)
    {
      product_name = (CharPtr)(rrp->ext.value.ptrvalue);
      FixCapitalizationInTitle (&product_name, FALSE);
      rrp->ext.value.ptrvalue = product_name;
    }
  }
}

static void FixProductCapitalization (IteM i)
{
  BaseFormPtr       bfp;
  SeqEntryPtr       sep;
  SelStructPtr      sel;
  Boolean           any_found = FALSE;
  SeqMgrFeatContext fcontext;
  SeqFeatPtr        sfp;
  RnaRefPtr         rrp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  
  sel = ObjMgrGetSelected ();
  if(sel == NULL)
  {
    VisitFeaturesInSep (sep, NULL, FixProductCapitalizationCallback);
  }
  else
  {
    while( sel != NULL )
    {
      if(sel->entityID == bfp->input_entityID && sel->itemtype == OBJ_SEQFEAT)
      {
        sfp = SeqMgrGetDesiredFeature (bfp->input_entityID, NULL, sel->itemID, 0, NULL, &fcontext);
        if(sfp != NULL)
        {
          if (sfp->data.choice == SEQFEAT_PROT)
          {
          	FixProductCapitalizationCallback (sfp, NULL);
          	any_found = TRUE;
          }
          else if (sfp->data.choice == SEQFEAT_RNA)
          {
            rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
          	if (rrp != NULL && rrp->ext.choice == 1)
          	{
          	  FixProductCapitalizationCallback (sfp, NULL);
          	  any_found = TRUE;
          	}
          }
        }
      } 
      sel = sel->next;      
    }
    if (!any_found)
    {
      VisitFeaturesInSep (sep, NULL, FixProductCapitalizationCallback);      
    }
  }
  
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  Update ();	
}

extern void LookupAllPubs (IteM i);
extern void ResolveExistingLocalIDs (IteM i);
extern void PromoteAlignsToBestIDProc (IteM i);
extern void SetSourceFocus (IteM i);
extern void ClearSourceFocus (IteM i);
extern void ExtraAccToHistByPos (IteM i);
extern void ClearCdsProducts (IteM i);
extern void ClearMrnaProducts (IteM i);
extern void MakeRedundantGPSwithProp (IteM i);
extern void MakeRedundantGPSnoProp (IteM i);
extern void MakeRedundantGPSjustXref (IteM i);
extern void FuseSlpJoins (IteM i);
extern void CreateSeqHistTPA (IteM i);
extern void CreateSeqHistTPADetailed (IteM i);
extern void CreateSeqHistDelta (IteM i);
extern void AddGlobalCodeBreak (IteM i);
extern void FixCdsAfterPropagate (IteM i);
extern void ParseTrinomial (IteM i);
extern void ReprocessPeptideProducts (IteM i);
extern void ReprocessmRNAProducts (IteM i);
extern void EditSeqEndsProc (IteM i);

static void NewAligmentViewer (IteM i)
{
  BaseFormPtr  bfp;
  ForM         f;
  BioseqPtr    bsp;
  Int2         top;
  ValNodePtr   seq_annot_list;
  SeqAnnotPtr  sap;
  SeqAlignPtr  salp = NULL;
  
#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  if (bfp == NULL) return;
  
  bsp = GetBioseqGivenIDs (bfp->input_entityID, 
                           bfp->input_itemID,
                           bfp->input_itemtype);
  if (bsp == NULL) return;
  seq_annot_list = FindAlignSeqAnnotsForBioseq (bsp);
  if (seq_annot_list != NULL && seq_annot_list->data.ptrvalue != NULL)
  {
    sap = (SeqAnnotPtr) seq_annot_list->data.ptrvalue;
    if (sap->type == 2)
    {
      salp = (SeqAlignPtr) sap->data;
    }
  }
  if (salp == NULL)
  {
    Message (MSG_ERROR, "No alignments found for this Bioseq!");
    return;
  }
  
#ifdef WIN_MAC
  top = 53;
#else
  top = 33;
#endif

  f = CreateAlnEditorWindow (33, top, "New Alignment Editor", salp, bfp->input_entityID);

  if (f != NULL)
  {
     Show (f);
     Update ();
     Select (f);
  }
}

static void TestTax3 (IteM i, Int4 taxid, CharPtr name)

{
  BaseFormPtr       bfp;
  OrgNamePtr        onp;
  OrgRefPtr         orp;
  T3DataPtr         tdp;
  T3ErrorPtr        tep;
  Taxon3RequestPtr  t3rq;
  Taxon3ReplyPtr    t3ry;
  T3ReplyPtr        trp;
  
#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  t3rq = CreateTaxon3Request (taxid, name, NULL);
  if (t3rq == NULL) return;
  t3ry = Tax3SynchronousQuery (t3rq);
  Taxon3RequestFree (t3rq);
  if (t3ry != NULL) {
    for (trp = t3ry->reply; trp != NULL; trp = trp->next) {
      switch (trp->choice) {
        case T3Reply_error :
          tep = (T3ErrorPtr) trp->data.ptrvalue;
          if (tep != NULL) {
            Message (MSG_POST, "T3 error %s", tep->message);
          }
          break;
        case T3Reply_data :
          tdp = (T3DataPtr) trp->data.ptrvalue;
          if (tdp != NULL) {
            orp = (OrgRefPtr) tdp->org;
            if (orp != NULL) {
              onp = orp->orgname;
              if (onp != NULL) {
                Message (MSG_POST, "T3 data %s code %d", orp->taxname, (int) onp->gcode);
              }
            }
          }
          break;
        default :
          break;
      }
    }
    Taxon3ReplyFree (t3ry);
  }
}

static void TestTax3A (IteM i)

{
  TestTax3 (i, 9606, NULL);
}

static void TestTax3B (IteM i)

{
  TestTax3 (i, 0, "Escherichia coli");
}

extern void SetupSpecialMenu (MenU m, BaseFormPtr bfp)

{
  IteM  i;
  MenU  s;
  MenU  x;

  s = SubMenu (m, "Def Line/ D");
  x = SubMenu (s, "Automatic Def Line");
  i = CommandItem (x, "Default Options", testAutoDef);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "No Modifiers", AutoDefWithoutModifiers);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Select Options...", testAutoDefWithOptions);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Add Definition Line", ApplyTitle);
  SetObjectExtra (i, bfp, NULL);
  x = SubMenu (s, "Prefix Def Line With");
  i = CommandItem (x, "Organism", AddOrgNameToDefLines);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Strain", AddStrainToDefLines);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Clone", AddCloneToDefLines);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Isolate", AddIsolateToDefLines);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Haplotype", AddHaplotypeToDefLines);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Cultivar", AddCultivarToDefLines);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "List", PrefixDefLines);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Parse Text", ParseDefLineToSourceQual);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "NC_Cleanup", DoNCCleanup);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "PT_Cleanup", DoPTCleanup);
  SetObjectExtra (i, bfp, NULL);
  if (indexerVersion) {
    SeparatorItem (s);
    i = CommandItem (s, "SeriousSeqEntryCleanup", DoSSECleanup);
    SetObjectExtra (i, bfp, NULL);
  }

  s = SubMenu (m, "Organism/ G");
  i = CommandItem (s, "Parse Text", ParseDefLineToSourceQual);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Parse File to Source", ParseFileToSource);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Parse ATCC Strain to Xref", AtccStrainToXref);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Append Modifier to Organism", AddModToOrg);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Trim Organism Name", TrimOrganismName);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Prefix Authority with Organism", PrefixAuthorityWithOrganism);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Parse Trinomial into Subspecies", ParseTrinomial);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Genus-Species Fixup", GenSpecTaxonFixup);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Country Fixup", CountryLookup);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Set Source Focus", SetSourceFocus);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Clear Source Focus", ClearSourceFocus);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Consolidate Organism Notes", ConsolidateOrganismNotes);
  SetObjectExtra (i, bfp, NULL);
  x = SubMenu (s, "Consolidate Like Modifiers");
  i = CommandItem (x, "With semicolons", ConsolidateLikeModifiersWithSemicolons);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Without semicolons", ConsolidateLikeModifiersWithoutSemicolons);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Load Organism Modifiers from File", LoadOrganismModifierTable);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Export Organism Modifiers to File", ExportOrganismTable);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Export Last Lineage Table", ExportLastLineage);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  x = SubMenu (s, "Add Type Strain Comments");
  i = CommandItem (x, "To All", AddTypeStrainCommentsToAll);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "With Constraints", AddTypeStrainCommentsWithConstraint);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  x = SubMenu (s, "Influenza Virus Names");
  i = CommandItem (x, "Parse Strain and Serotype from Influenza A Virus Organisms",
                   ParseInfluenzaAVirusNames);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Add Strain and Serotype to Influenza A Virus Organism Names",
                   AddStrainAndSerotypeToInfluenzaAVirusNames);
  SetObjectExtra (i, bfp, NULL);

  s = SubMenu (m, "Apply/ A");
  i = CommandItem (s, "Add CDS", ApplyCDS);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Add RNA", ApplyRRNA);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Add other Feature", ApplyImpFeat);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Add Qualifier", ApplyGBQual);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Add Source Qual", ApplySourceQual);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Add CDS-Gene-Prot-mRNA", ApplyCDSGeneProt);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Add RNA Qual", ApplyRNAQual);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Add Molecule Information", ApplyMolInfo);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Add Gene Pseudo", AddGenePseudo);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Add Gene Pseudo with String Constraint", AddGenePseudoConstraint);
  SetObjectExtra (i, bfp, NULL);

  s = SubMenu (m, "Remove/ R");
  i = CommandItem (s, "Remove Descriptors", RemoveDescriptor);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove Features", FeatureRemove);
  SetObjectExtra (i, bfp, NULL);

  i = CommandItem (s, "Remove Qualifiers", RemoveGBQual);
  SetObjectExtra (i, bfp, NULL);

  SeparatorItem (s);
  i = CommandItem (s, "Remove Alignments", RemoveAlignment);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove Graphs", RemoveGraph);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove Proteins", RemoveProteins);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove Proteins and Renormalize Nuc-Prot Sets", RemoveProteinsAndRenormalize);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Remove Source Qualifier", RemoveSourceQual);
  SetObjectExtra (i, bfp, NULL);

  i = CommandItem (s, "Remove CDS-Gene-Prot-mRNA", RemoveCDSGeneProt);
  SetObjectExtra (i, bfp, NULL);
  
  i = CommandItem (s, "Remove Nth Seg's CDS for Each SegSet",
		   RemoveNthCDSFromSegSets_Callback);
  SetObjectExtra (i, bfp, NULL);

  i = CommandItem (s, "Remove RNA Qual", RemoveRNAQual);
  SetObjectExtra (i, bfp, NULL);

  i = CommandItem (s, "Remove Pub", RemovePub);
  SetObjectExtra (i, bfp, NULL);
  x = SubMenu (s, "Remove Protein Titles");
  i = CommandItem (x, "All but RefSeq", DoRemoveProtTitles);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "All", DoRemoveAllProtTitles);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Hypothetical", RemoveHypotheticalProteinTitles);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Inconsistent", RemoveInconsistentProteinTitles);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  x = SubMenu (s, "Remove Nucleotide Titles");
  i = CommandItem (x, "All", DoRemoveAllNucTitles);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "mRNA", DoRemoveMrnaTitles);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "NPS", DoRemoveNPSTitles);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  x = SubMenu (s, "Remove Text");
  i = CommandItem (x, "Inside String", RemoveTextInsideString);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Outside String", RemoveTextOutsideString);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  x = SubMenu (s, "Remove Duplicate");
  i = CommandItem (x, "Genes", RemoveDuplicateGenes);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Feats", RemoveDuplicateFeats);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove Unnecessary Gene Xrefs", RemoveGeneXrefs);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove Non-Suppressing Gene Xrefs", RemoveNSGeneXrefs);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove ALL Gene Xrefs", RemoveAllGeneXrefs);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Remove Selected Features", RemoveSelFeats);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove Unselected Features", RemoveUnselFeats);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  x = SubMenu (s, "Remove CDD");
  i = CommandItem (x, "Regions", RemoveCDDRegions);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Alignments", RemoveCDDAligns);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Duplicates", RemoveCDDDups);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Retain Best CDD", RetainBestCDD);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Remove BankIt Comments", RemoveBankitComments);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  x = SubMenu (s, "Remove Taxon Xrefs");
  i = CommandItem (x, "From Features", RemoveTaxonXrefsFromFeatures);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "From Features and BioSources", 
                   RemoveTaxonXrefsFromFeaturesAndBioSources);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Remove Redundant Proprotein Misc Feats",
                   RemoveRedundantProproteinMiscFeats);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Remove Sequences from Alignments", RemoveSequencesFromAlignment);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove Sequences from Record", RemoveSequencesFromRecord);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Remove Author Consortiums", RemovePubConsortiums);
  SetObjectExtra (i, bfp, NULL);

  s = SubMenu (m, "Clear/ L");
  i = CommandItem (s, "Strip Serial Numbers", StripSerials);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Clear CDS Products", ClearCdsProducts);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Clear mRNA Products", ClearMrnaProducts);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Clear GenBank Components", (ItmActnProc) EditGenbankElements);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Clear ORF Flag in CDSs", ClearOrfFlagInCDS);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Clear transl-except in CDSs", ClearTranslExceptInCDS);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Clear Gene Pseudo", ClearGenePseudo);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Clear Gene Pseudo with String Constraint", ClearGenePseudoConstraint);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  x = SubMenu (s, "Remove Db_xrefs");
  i = CommandItem (x, "From Genes", RemoveGeneDbxrefs);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "From RNAs", RemoveRNADbxrefs);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "From CDSs", RemoveCDSDbxrefs);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "From All Features", RemoveAllFeatureDbxrefs);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "From All Features and BioSources", RemoveAllDbxrefs);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Remove Figure from Pubs", RemovePubFig);
  SetObjectExtra (i, bfp, NULL);
  if (indexerVersion) {
    SeparatorItem (s);
    i = CommandItem (s, "Remove Local IDs from Bioseqs", RemoveLocalIDs);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (s, "Remove Seq-ID Name From Prot Feats", RemoveSeqIdName_Callback);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (s, "Remove GenBank IDs from Bioseqs", RemoveGBIDsFromBioseqs);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (s, "Remove GenBank IDs from Proteins", RemoveGBIDsFromProteins);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (s, "Remove GI IDs from Bioseqs", RemoveGIsFromBioseqs);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Remove LOCUS from Parts", RemoveLocusFromParts);
    SetObjectExtra (i, bfp, NULL);
  }

  s = SubMenu (m, "Convert/ C");
  i = CommandItem (s, "Convert Features", ConvertFeatures);
  SetObjectExtra (i, bfp, NULL);

  i = CommandItem (s, "Convert Qualifiers", ConvertGBQual);
  SetObjectExtra (i, bfp, NULL);

  SeparatorItem (s);
  i = CommandItem (s, "Convert Source Qualifier", ConvertSourceQual);
  SetObjectExtra (i, bfp, NULL);

  i = CommandItem (s, "Convert CDS-Gene-Prot-mRNA", ConvertCDSGeneProt);
  SetObjectExtra (i, bfp, NULL);

  i = CommandItem (s, "Convert RNA Qual", ConvertRNAQual);
  SetObjectExtra (i, bfp, NULL);

  i = CommandItem (s, "CDS with internal Stop Codon to Misc Feat",
		   ConvertCDSWithStopCodons);
  SetObjectExtra (i, bfp, NULL);

  x = SubMenu (s, "CDS with internal Stop Codon to Pseudo Gene");
  i = CommandItem (x, "Keep CDS", ConvertCDSToPseudoGeneAndKeep);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Remove CDS", ConvertCDSToPseudoGeneAndRemove);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);

  i = CommandItem (s, "Swap Source Qualifiers", SwapSourceQual);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Swap CDS-Gene-Prot-mRNA Qualifiers", SwapCDSGeneProt);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Swap RNA Qualifiers", SwapRNAQual);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Swap Qualifiers", SwapGBQual);
  SetObjectExtra (i, bfp, NULL);

  SeparatorItem (s);

  i = CommandItem (s, "Parse Text", ParseFlatfileToSourceQual);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);

  i = CommandItem (s, "Parse Codons from tRNA Comment", ParseCodonsFromtRNAComment);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Parse Anticodons from tRNA Comment", ParseAntiCodonsFromtRNAComment);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);

  i = CommandItem (s, "Load Secondary Accession Numbers From File",
                   LoadSecondaryAccessionNumbersFromFile);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Load History Accession Numbers From File",
                   LoadHistoryAccessionNumbersFromFile);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);

  i = CommandItem (s, "BankIt Comment to Comment Descriptor", CopyBankitComments);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);

  i = CommandItem (s, "Pub Descriptor to Feature", ConvertPubDescToFeat);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Pub Descriptor to Feature by Constraint", CreateConvertPubDescStringConstraintDialog);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Source Descriptor to Feature", ConvertSrcDescToFeat);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Comment Descriptor to Feature", ConvertComDescToFeat);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Second Protein Name to Description", SecondProtNameToDesc);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Protein Description to Second Name", ProtDescToSecondName);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Change Ribosomal RNA to Genomic DNA", RibosomalRNAToGenomicDNAMenuItem);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  x = SubMenu (s, "Transform to");
  i = CommandItem (x, "Region", TransformToRegion);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Site", TransformToSite);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Bond", TransformToBond);
  SetObjectExtra (i, bfp, NULL);
  if (indexerVersion) {
    SeparatorItem (s);
    i = CommandItem (s, "Resolve Colliding Local IDs", ResolveExistingLocalIDs);
    SetObjectExtra (i, bfp, NULL);
  }
  SeparatorItem (s);
  i = CommandItem (s, "Refresh Gene Xrefs", RefreshGeneXRefs);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Convert transl-except in CDSs to RNA Editing Exception", ConvertTranslExceptToRNAEditingException);
  SetObjectExtra (i, bfp, NULL);  
  SeparatorItem (s);
  i = CommandItem (s, "Convert Gene Locus Tag to Old Locus Tag", ConvertLocusTagToOldLocusTag);
  SetObjectExtra (i, bfp, NULL);
#if 0
  SeparatorItem (s);
  i = CommandItem (s, "Convert SegSets to Delta Sequences", ConvertSegSetToDeltaItem);
  SetObjectExtra (i, bfp, NULL);
#endif

  s = SubMenu (m, "Edit/ E");
  i = CommandItem (s, "Edit Qualifiers", EditGBQual);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Edit Source Qual", EditSourceQual);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Edit CDS-Gene-Prot-mRNA", EditCDSGeneProt);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Edit RNA Qual", EditRNAQual);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Edit Publications", EditPubs);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Edit Molecule Information", EditMolInfoFields);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  x = SubMenu (s, "Edit Feature");
  i = CommandItem (x, "Evidence", FeatureEvidenceEditor);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Exception", FeatureExceptionEditor);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Partial", FeaturePartialEditor);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Strand", FeatureStrandEditor);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Citation", FeatureCitationEditor);
  SetObjectExtra (i, bfp, NULL);

  SeparatorItem (s);
  i = CommandItem (s, "Extend Gene", ExtendGeneReg);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Correct Genes for CDSs", CorrectGenes);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Insert Gene Locus Tag Prefix", InsertGeneLocusTagPrefix);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Extend Partial Features", ExtendPartialFeatures);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Make Exons from CDS Intervals", MakeExonsFromCDSIntervals);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Make Exons from mRNA Intervals", MakeExonsFromMRNAIntervals);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Load Feature Qualifiers from File",
                   LoadFeatureQualifierTable);
  SetObjectExtra (i, bfp, NULL);
  if (indexerVersion) {
    SeparatorItem (s);
    i = CommandItem (s, "Edit Locus", EditLocusProc);
    SetObjectExtra (i, bfp, NULL);
    SetupEditSecondary (s, bfp);
    x = SubMenu (s, "Convert Accession to LocalID");
    i = CommandItem (x, "For All Sequences", ConvertToLocalProcAll);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (x, "For Nucleotide Sequences", ConvertToLocalProcOnlyNucs);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (x, "For Protein Sequences", ConvertToLocalProcOnlyProts);
    SetObjectExtra (i, bfp, NULL);
    
    i = CommandItem (s, "Extra Accession to History by Position", ExtraAccToHistByPos);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Edit Sequence Ends", EditSeqEndsProc);
    SetObjectExtra (i, bfp, NULL);
  }

  s = SubMenu (m, "Transform/ T");
  i = CommandItem (s, "Correct CDS Genetic Codes", CorrectCDSGenCodes);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Cleanup CDS partials after propagation", FixCdsAfterPropagate);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Trim Ns from Bioseqs", TrimNsFromNucs);
  SetObjectExtra (i, bfp, NULL);
  if (indexerVersion) {
    x = SubMenu (s, "Put Ns under VecScreen Hits");
    i = CommandItem (x, "Strong", VecScreenToNsStrong);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (x, "Strong + Moderate", VecScreenToNsModerate);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (x, "All", VecScreenToNsWeak);
    SetObjectExtra (i, bfp, NULL);
  }
  SeparatorItem (s);
  i = CommandItem (s, "Truncate Proteins at Stops", TruncProtsAtStops);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Trim Protein Feature Lengths", TrimProtFeatLengths);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  if (indexerVersion) {
    i = CommandItem (s, "Promote Features to Best ID", PromoteToBestIDProc);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (s, "Promote Alignments to Best ID", PromoteAlignsToBestIDProc);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Promote Features to Worst ID", PromoteToWorstIDProc);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Fuse Joins in Locations", FuseSlpJoins);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
  }
  i = CommandItem (s, "Suppress Genes On All RBS", SuppressGenesOnAllRBS);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Suppress CDSGeneRange Error", SuppressCDSGeneRangeError);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Restore CDSGeneRange Error", RestoreCDSGeneRangeError);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Map RBSs to Downstream Gene", MapRBSsToDownstreamGene);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Remove RBSs Genes", RemoveRBSGenes);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  x = SubMenu (s, "Add Transl Excepts");
  i = CommandItem (x, "Without Comment", GlobalAddTranslExcept);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "With Comment", AddTranslExceptWithComment);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Fix Publication Title Capitalization", FixTitleCapitalization);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Fix Publication Affiliation Capitalization", FixAffiliationCapitalization);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  x = SubMenu (s, "Fix Authors");
  i = CommandItem (x, "Fix Author Capitalization", FixAuthorCapitalization);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Reverse Author Name Order", FixNameOrder);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Strip Author Suffixes", StripAuthorSuffix);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Truncate Author Middle Initials", TruncateAuthorMiddle);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Change Author Name to Consortium", ChangeAuthorNameToConsortium);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Fix Product Name Capitalization", FixProductCapitalization);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Replace Repeat Region Locus Tags with FLYBASE Dbxref",
                   ReplaceRepeatRegionLocusTagWithDbxref);
  SetObjectExtra (i, bfp, NULL);

  s = SubMenu (m, "Select/ S");
  i = CommandItem (s, "Select Descriptors", SelectDescriptor);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Select Features", SelectFeatures);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Select Sequences", SelectBioseq);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Select Publications", SelectPubs);
  SetObjectExtra (i, bfp, NULL);

  s = SubMenu (m, "Process/ P");
  i = CommandItem (s, "Process Additional FASTA Proteins", ParseInMoreProteins);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Process FASTA Proteins in Order", ParseInProteinsInOrder);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Process Additional FASTA cDNAs", ParseInMoreMRNAs);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Process FASTA Nucleotide Updates", ParseInNucUpdates);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Process Oligonucleotide PCR Primers", ParseInOligoPrimers);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Adjust Proteins and CDSs for Stop Codons", FixProtsAndCDSsBasedOnStopCodonsItem);
  SetObjectExtra (i, bfp, NULL);
  x = SubMenu (s, "Retranslate Coding Regions");
  i = CommandItem (x, "Obey Stop Codon", RetranslateCdRegionsNoStop);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Ignore Stop Codon", RetranslateCdRegionsDoStop);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Ignore Stop Codon Except at End of Complete CDS", RetranslateCdRegionsNoStopExceptEndCompleteCDS);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Retranscribe mRNA Products", ReprocessmRNAProducts);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Regenerate Peptide Products", ReprocessPeptideProducts);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Resynchronize CDS Partials", ResynchronizeCDSPartials);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Resynchronize mRNA Partials", ResynchronizeMRNAPartials);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Resynchronize Peptide Partials", ResynchronizePeptidePartials);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  x = SubMenu (s, "Recompute Suggested Intervals");
  i = CommandItem (x, "And Update Genes", RecomputeSuggestFixGenes);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Do Not Update Genes", RecomputeSuggest);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Update Proteins From CDS", UpdateProteinsFromCDS);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Add Global Code Break", AddGlobalCodeBreak);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Parse Codon Quals to Code Breaks", ParseCodonQualToCodeBreak);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  x = SubMenu (s, "Map Feature Intervals to Parts");
  i = CommandItem (x, "Joined Location", MergeToPartsJoin);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Ordered Location", MergeToPartsOrdered);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Map Intervals to Segmented Sequence", MergeToSegSeq);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Package Features on Parts", PackageOnParts);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Package Proteins on Nucleotides", PackageOnNucs);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Delta or Segmented Sequence to Raw Sequence", SegSeqToRawSeq);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Raw Sequence with Ns to Delta Sequence", 
                   RawSeqToDeltaSeqUnknownWithUnknownLengthGaps);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Convert Raw Sequences To Delta Sequences with Known Gap Locations",
                   ConvertRawToDeltaWithGapLocations);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  x = SubMenu (s, "Generate GenProdSet Redundancy");
  i = CommandItem (x, "With Descriptor Propagation", MakeRedundantGPSwithProp);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Without Descriptor Propagation", MakeRedundantGPSnoProp);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Just Copy CDS Prot Xref", MakeRedundantGPSjustXref);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Convert Pseudo CDS to Misc_feat", ConvertPseudoCDSToMiscFeat);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Process Pseudo Misc_feats", ProcessPseudoMiscFeat);
  SetObjectExtra (i, bfp, NULL);

  s = SubMenu (m, "Projects/ J");
  if (indexerVersion) {
    i = CommandItem (s, "Create Seq-Hist for TPA", CreateSeqHistTPA);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (s, "Create Seq-Hist for TPA - Detailed", 
                     CreateSeqHistTPADetailed);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (s, "Infer Seq-Hist from Delta", CreateSeqHistDelta);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Read Seq-Hist Assembly BLASTs", ReadSeqHistAssembly);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (s, "Create TPA BLAST Database", CreateTPABlastDB);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Remove Seq-Hist Assembly", RemoveSeqHistAssembly);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (s, "Leave First TPA Assembly", LeaveFirstSeqHistAssembly);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Add Gaps of 100 to HTGS Deltas", AddDelta100Gaps);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (s, "Add Gaps of 0 to HTGS Deltas", AddDelta0Gaps);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (s, "Remove Gaps at ends of HTGS Deltas", DeleteDeltaEndGaps);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Add Genomes User Object", AddGenomesUserObject);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Add Genomic Phrap Graphs", MakePhrap);
    SeparatorItem (s);
    i = CommandItem (s, "Load TPA Accession Numbers From File",
                     LoadTPAAccessionNumbersFromFile);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Demote GenProdSet To NucProtSet", GPStoNPS);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Move CDS To NucProtSet", CDStoNPS);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Add mRNA Titles to GenProdSet", MrnaTitlesToGPS);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Promote NucProtSet to GenProdSet", NPStoGPS);
    SetObjectExtra (i, bfp, NULL);
  }

  s = SubMenu (m, "Misc/ M");
  i = CommandItem (s, "Clean Up Record", ForceCleanup);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Force Taxonomy Fixup", ForceTaxonFixup);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Force Locus Fixup", ForceLocusFixup);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Fuse Features", FuseFeature);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Gene Features from Xrefs", XrefToGene);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Gene Xrefs from Features", GeneToXref);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  x = SubMenu (s, "Gene Features From");
  i = CommandItem (x, "CDS", CdsToGene);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "tRNA", tRNAtoGene);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "rRNA", rRNAtoGene);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "miscRNA", MiscRNAtoGene);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "CDS Features from Gene", GeneToCds);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "CDS Features from Gene, mRNA, or exon", FeatureToCds);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Extract Protein Features From CDS comments", ExtractProteinFeaturesFromNote);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Split Into Groups of 100 Bioseqs", MakeGroupsOf200);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Add Cit-sub for update", AddCitSubForUpdate);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Alignment Summary", ViewAlignmentSummary);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remap Feature Locations Based on Alignment Coordinates", FixFeatureIntervals);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Convert Inner CDSs to Mat Peptides", ConvertInnerCDSsToProteinFeatures);
  SetObjectExtra (i, bfp, NULL);
  if (indexerVersion) {
    SeparatorItem (s);
    i = CommandItem (s, "Connect CDS to Closest Protein", ReconnectCDSProduct);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (s, "Merge Multiple CDS Into One", MergeCDS);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Reload Publications", LookupAllPubs);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    x = SubMenu (s, "Add BLAST tag to Seq-annot");
    i = CommandItem (x, "BLASTN", AddBlastNToSeqAnnot);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (x, "BLASTP", AddBlastPToSeqAnnot);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (x, "BLASTX", AddBlastXToSeqAnnot);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (x, "TBLASTN", AddTBlastNToSeqAnnot);
    SetObjectExtra (i, bfp, NULL);
  }
  if (extraServices) {
    SeparatorItem (s);
    i = CommandItem (s, "Make ToolBar Window/ T", MakeToolBarWindow);
    SetObjectExtra (i, bfp, NULL);
  }
  SeparatorItem (m);
  i = CommandItem (m, "Sort Unique Count/ U", SUCProc);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (m, "Sort Unique Count Resorted/ N", SUCRProc);
  SetObjectExtra (i, bfp, NULL);
  s = SubMenu (m, "Sort Unique Count By Group");
  i = CommandItem (s, "With Sequence", SUCBProc);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Without Sequence", SUCBNoSequenceProc);
  SetObjectExtra (i, bfp, NULL);  

  if (indexerVersion) {
    SeparatorItem (m);
    i = CommandItem (m, "Test Alignment Reader", ReadAlignment);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (m, "Export Alignment Interleave", ExportAlignmentInterleave);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (m, "Export Alignment Contiguous", ExportAlignmentContiguous);
    SetObjectExtra (i, bfp, NULL);

    SeparatorItem (m);

    i = CommandItem (m, "New Alignment Editor", NewAligmentViewer);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (m, "Old Alignment Editor", OldAlignmentEditor);
    SetObjectExtra (i, bfp, NULL);
        
    SeparatorItem (m);
    s = SubMenu (m, "Test Taxon3 Server");
    i = CommandItem (s, "9606", TestTax3A);
    i = CommandItem (s, "E. coli", TestTax3B);
    SetObjectExtra (i, bfp, NULL);
  }
/*#ifdef INTERNAL_NCBI_SEQUIN*/
/*#ifdef NEW_TAXON_SERVICE*/
/*
  SeparatorItem (m);
  i = CommandItem (m, "Prepare TaxList", PrepareTaxListProc);
*/
/*#endif*/
/*#endif*/
}

extern void SetupBatchApplyMenu (MenU s, BaseFormPtr bfp)

{
  IteM  i;

  i = CommandItem (s, "Add CDS", ApplyCDS);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Add RNA", ApplyRRNA);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Add other Feature", ApplyImpFeat);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Add Qualifier", PublicApplyGBQual);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Add Source Qual", PublicApplySourceQual);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Add CDS-Gene-Prot-mRNA", PublicApplyCDSGeneProt);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Add RNA Qual", PublicApplyRNAQual);
  SetObjectExtra (i, bfp, NULL);
}

extern void SetupBatchEditMenu (MenU s, BaseFormPtr bfp)

{
  IteM  i;

  i = CommandItem (s, "Edit Qualifiers", PublicEditGBQual);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Edit Source Qual", PublicEditSourceQual);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Edit CDS-Gene-Prot", PublicEditCDSGeneProt);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Edit RNA Qual", PublicEditRNAQual);
  SetObjectExtra (i, bfp, NULL);
}

