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
* $Revision: 6.183 $
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
#include <asn2ff.h>
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

/*#ifdef INTERNAL_NCBI_SEQUIN*/
/*#ifdef NEW_TAXON_SERVICE*/
#include <taxinc.h>
#include <taxutil.h>
/*#endif*/
/*#endif*/

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

static void AddCitSubForUpdateProc (BaseFormPtr bfp)

{
  CitSubPtr    csp;
  CitSubPtr    best;
  DatePtr      dp;
  GatherScope  gs;
  PubdescPtr   pdp;
  ValNodePtr   sdp;
  SeqEntryPtr  sep;
  ValNodePtr   vnp;

  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  best = NULL;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQDESC] = FALSE;
  gs.scope = sep;
  GatherSeqEntry (sep, (Pointer) (&best), FindBestCitSubCallback, &gs);
  if (best == NULL) {
    Message (MSG_OK, "There is no earlier cit-sub template");
    return;
  }
  dp = DateCurr ();
  if (dp != NULL) {
    if (DateMatch (best->date, dp, FALSE) == 0) {
      DateFree (dp);
      if (StringICmp (best->descr, "Sequence update by submitter") == 0) {
        Message (MSG_OK, "There already exists an update on today's date");
        Update ();
        return;
      } else if (best->descr == NULL) {
        best->descr = StringSave ("Sequence update by submitter");
        Message (MSG_OK, "Adding update indication to existing cit-sub");
        ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
        ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
        Update ();
        return;
      }
    }
    DateFree (dp);
  }
  sdp = CreateNewDescriptor (sep, Seq_descr_pub);
  if (sdp == NULL) return;
  pdp = PubdescNew ();
  if (pdp == NULL) return;
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
  Message (MSG_OK, "The update Cit-sub has been placed on the top Seq-entry");
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  Update ();
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
		if (vnp->choice == SEQID_GENBANK && vnp->data.ptrvalue != NULL) {
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
                         (bssp->_class >= 13 && bssp->_class <= 16))) {
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
    if (sip->choice == SEQID_GENBANK) {
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
  if (bsp->repr != Seq_repr_raw) return;
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
    if (nbsp->repr != Seq_repr_raw) return;
    tsip = GetTSIPforBSP (nbsp);
  }
  if (tsip == NULL) return;
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
  if (digits + 1 + len > 10) {
    prefix += digits + 1 + len - 10;
  }
  for (sep = pbssp->seq_set, segment = 1; sep != NULL; sep = sep->next, segment++) {
    DoFixPartLocus (sep, prefix, segment, digits);
  }
  if (sbsp == NULL) return;
  tsip = NULL;
  for (sip = sbsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_GENBANK) {
      tsip = (TextSeqIdPtr) sip->data.ptrvalue;
    }
  }
  if (tsip == NULL) {
    tsip = TextSeqIdNew ();
    if (tsip == NULL) return;
    ValNodeAddPointer (&(sbsp->id), SEQID_GENBANK, (Pointer) tsip);
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

static void ForceCleanupBtn (IteM i, ButtoN b, Boolean validate)

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
  SeqMgrClearFeatureIndexes (bfp->input_entityID, NULL);
  SeqEntryExplore (sep, (Pointer) bfp, CleanupGenbankBlockCallback);
  SeqEntryExplore (sep, NULL, CleanupEmptyFeatCallback);
  SeqEntryExplore (sep, NULL, MergeAdjacentAnnotsCallback);
  MySeqEntryToAsn3 (sep, TRUE, FALSE, TRUE);
  CorrectGenCodes (sep, bfp->input_entityID);
  CleanUpPseudoProducts (bfp->input_entityID, sep);
  SeqEntryExplore (sep, NULL, GetRidOfRedundantSourceNotes);
  RenormalizeNucProtSets (sep, TRUE);
  CdCheck (sep, NULL);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ObjMgrDeSelect (0, 0, 0, 0, NULL);
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

static void LastDitchLookup (BioSourcePtr biop)

{
  DbtagPtr      dbt;
  Char          orgname [256];
  OrgRefPtr     orp;
  CharPtr       ptr;
  SubSourcePtr  ssp;
  Int4          taxID;
  ValNodePtr    vnp;

  if (biop == NULL) return;
  orp = biop->org;
  if (orp == NULL) return;
  if (orp->db != NULL) {
    for (vnp = orp->db; vnp != NULL; vnp = vnp->next) {
      dbt = (DbtagPtr) vnp->data.ptrvalue;
      if (dbt != NULL) {
        if (StringICmp (dbt->db, "taxon") == 0) return;
      }
    }
  }
  StringNCpy_0 (orgname, orp->taxname, sizeof (orgname));
  if (StringHasNoText (orgname)) return;
  ptr = StringChr (orgname, ' ');
  if (ptr == NULL) return;
  while (*ptr == ' ') {
    ptr++;
  }
  ptr = StringChr (ptr, ' ');
  if (ptr == NULL) return;
  *ptr = '\0';

  taxID = tax1_getTaxIdByName (orgname);
  if (taxID < 1) return;

  ssp = SubSourceNew ();
  if (ssp != NULL) {
    ssp->subtype = 255;
    ssp->name = orp->taxname;
    ssp->next = biop->subtype;
    biop->subtype = ssp;
    orp->taxname = StringSave (orgname);
  }
}

static void LastDitchTaxonFixup (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)

{
	BioseqPtr bsp = NULL;
	BioseqSetPtr bssp = NULL;
	ValNodePtr descr, vnp;
	BioSourcePtr biosp = NULL;
	OrgRefPtr orp = NULL;
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
  if (tax1_init ()) {
    tax1_setSynonyms (TRUE);
    SeqEntryExplore (sep, NULL, LastDitchTaxonFixup);
    tax1_fini ();
  }
  ForceTaxonFixupBtn (i, NULL);
}

static Boolean MergePartsCallback (GatherContextPtr gcp)

{
  BioseqPtr     bsp;
  CodeBreakPtr  cbp;
  CdRegionPtr   crp;
  Boolean       noLeft;
  Boolean       noRight;
  RnaRefPtr     rrp;
  SeqFeatPtr    sfp;
  SeqIdPtr      sip;
  SeqLocPtr     slp;
  tRNAPtr       trna;

  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->location == NULL) return TRUE;
  sip = SeqLocId (sfp->location);
  if (sip == NULL) return TRUE;
  bsp = BioseqFind (sip);
  if (bsp == NULL) return TRUE;
  if (ISA_aa (bsp->mol)) return TRUE;
  CheckSeqLocForPartial (sfp->location, &noLeft, &noRight);
  slp = SegLocToParts (bsp, sfp->location);
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
          slp = SegLocToParts (bsp, cbp->loc);
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
          slp = SegLocToParts (bsp, trna->anticodon);
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

static void MergeToParts (IteM i)

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
      Message (MSG_ERROR, "MergeToParts error");
      return;
    }
    MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
    gs.seglevels = 1;
    MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
    gs.ignore[OBJ_BIOSEQ] = FALSE;
    gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
    gs.ignore[OBJ_SEQANNOT] = FALSE;
    gs.ignore[OBJ_SEQFEAT] = FALSE;
    GatherEntity (bfp->input_entityID, NULL, MergePartsCallback, &gs);
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
  GatherItem (ssp->entityID, ssp->itemID, ssp->itemtype, NULL, MergePartsCallback);
  ObjMgrSetDirtyFlag (ssp->entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ssp->entityID, 0, 0);
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

#define BUFSIZE 128

static void SegToRawCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  ByteStorePtr  bs;
  BioseqPtr     bsp;
  Char          buf [BUFSIZE + 5];
  Int2          j;
  Uint1         residue;
  SeqPortPtr    spp;

  if (sep == NULL) return;
  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL || (bsp->repr != Seq_repr_seg && bsp->repr != Seq_repr_delta)) return;
  if (! ISA_na (bsp->mol)) return;
  spp = SeqPortNew (bsp, 0, -1, 0, Seq_code_iupacna);
  if (spp == NULL) return;
  if (bsp->repr == Seq_repr_delta) {
    SeqPortSet_do_virtual (spp, TRUE);
  }
  bs = BSNew (bsp->length);
  j = 0;
  buf [0] = '\0';
  if (bs != NULL) {
    while ((residue = SeqPortGetResidue (spp)) != SEQPORT_EOF) {
      if (IS_residue (residue)) {
        residue = TO_UPPER (residue);
        buf [j] = (Char) residue;
        j++;
        if (j >= BUFSIZE) {
	      BSWrite (bs, buf, j * sizeof (Char));
	      j = 0;
        }
      }
    }
    if (j > 0) {
	  BSWrite (bs, buf, j * sizeof (Char));
	  j = 0;
    }
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
  SeqPortFree (spp);
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

static void ProtToImpFeatCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  SeqFeatPtr    cds;
  BioseqPtr     bsp = NULL;
  BioseqSetPtr  bssp = NULL;
  Uint1         entityID;
  GBQualPtr     gbqual;
  ImpFeatPtr    ifp;
  CharPtr       name;
  ProtRefPtr    prp;
  SeqAnnotPtr   sap;
  SeqEntryPtr   scope;
  SeqFeatPtr    sfp;
  SeqLocPtr     slp;
  ValNodePtr    vnp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  scope = (SeqEntryPtr) mydata;
  if (scope == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  entityID = ObjMgrGetEntityIDForChoice (sep);
  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        if (sfp->data.choice == SEQFEAT_PROT) {
          prp = (ProtRefPtr) sfp->data.value.ptrvalue;
          if (prp != NULL) {
            if (prp->processed >= 2 && prp->processed <= 4) {
              cds = FindBestCds (entityID, NULL, sfp->location, scope);
              if (cds != NULL) {
                slp = aaLoc_to_dnaLoc (cds, sfp->location);
                if (slp != NULL) {
                  sfp->location = SeqLocFree (sfp->location);
                  sfp->location = slp;
                  ifp = ImpFeatNew ();
                  if (ifp != NULL) {
                    switch (prp->processed) {
                      case 2 :
                        ifp->key = StringSave ("mat_peptide");
                        break;
                      case 3 :
                        ifp->key = StringSave ("sig_peptide");
                        break;
                      case 4 :
                        ifp->key = StringSave ("transit_peptide");
                        break;
                      default :
                        ifp->key = StringSave ("?");
                        break;
                    }
                    name = NULL;
                    vnp = prp->name;
                    if (vnp != NULL) {
                      name = vnp->data.ptrvalue;
                    }
                    if (name == NULL) {
                      name = prp->desc;
                    }
                    if (name != NULL) {
                      gbqual = GBQualNew ();
                      if (gbqual != NULL) {
                        gbqual->qual = StringSave ("product");
                        gbqual->val = StringSave (name);
                        gbqual->next = sfp->qual;
                        sfp->qual = gbqual;
                      }
                    }
                    sfp->data.choice = SEQFEAT_IMP;
                    sfp->data.value.ptrvalue = (Pointer) ifp;
                    ProtRefFree (prp);
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

static void ProtToImpFeatProc (SeqEntryPtr sep)

{
  BioseqSetPtr  bssp = NULL;

  if (sep == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL) return;
    if (bssp->_class == 7 ||
        (bssp->_class >= 13 && bssp->_class <= 16)) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        ProtToImpFeatProc (sep);
      }
      return;
    }
  }
  SeqEntryExplore (sep, (Pointer) sep, ProtToImpFeatCallback);
}

static void ProtToImpFeat (IteM i)

{
  MsgAnswer    ans;
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
  ans = Message (MSG_OKC, "Are you sure you want to (temporarily) make ImpFeats?");
  if (ans == ANS_CANCEL) return;
  WatchCursor ();
  Update ();
  ProtToImpFeatProc (sep);
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
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

extern EnumFieldAssoc  enum_bond_alist [];
extern EnumFieldAssoc  enum_site_alist [];

static Boolean            alistBoxUp;
static Boolean            alistBoxAccept;
static PopuP              alistBoxPopup;
static EnumFieldAssocPtr  alistBoxAlist;

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

static Boolean AlistMessage (EnumFieldAssocPtr al, UIEnumPtr val, UIEnum dflt, CharPtr mssg)

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
    PushButton (c, "Accept", AcceptAlistMessage);
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
  b = PushButton (c, "Accept", DoMultiTransform);
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

static void CommonConvertDescToFeat (IteM i, Boolean pub, Boolean src, Boolean com)

{
  BaseFormPtr   bfp;
  SeqEntryPtr   sep;
  SelStructPtr  sel;
  SeqFeatPtr    sfp;
  ValNodePtr    vnp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
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
          sfp = CreateNewFeature (sep, NULL, SEQFEAT_PUB, NULL);
          if (sfp != NULL) {
            sfp->data.value.ptrvalue = vnp->data.ptrvalue;
            vnp->data.ptrvalue = NULL;
          }
          ValNodeFree (vnp);
        }
      }
    }
    ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
    return;
  }
  if (ConvertPubSrcComDescsToFeats (sep, pub, src, com, FALSE)) {
    ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  }
}

static void ConvertPubDescToFeat (IteM i)

{
  CommonConvertDescToFeat (i, TRUE, FALSE, FALSE);
}

static void ConvertSrcDescToFeat (IteM i)

{
  CommonConvertDescToFeat (i, FALSE, TRUE, FALSE);
}

static void ConvertComDescToFeat (IteM i)

{
  CommonConvertDescToFeat (i, FALSE, FALSE, TRUE);
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

#define REMOVE_QUAL   1
#define CONVERT_FEAT  2
#define CONVERT_QUAL  3
#define EDIT_QUAL     4
#define ADD_QUAL      5

typedef struct qualformdata {
  FEATURE_FORM_BLOCK

  Int2           type;
  LisT           fromfeatlist;
  LisT           tofeatlist;
  LisT           fromquallist;
  LisT           toquallist;
  TexT           findthis;
  TexT           replacewith;
  Uint2          itemtype;
  Uint2          subtype;
  ObjMgrPtr      omp;
  ObjMgrTypePtr  omtp;
  Boolean        stringfound;
  Boolean        abortconvert;
  CharPtr        findStr;
  CharPtr        replaceStr;
  EnumFieldAssoc PNTR realalist;
  EnumFieldAssoc PNTR alist;
  ValNodePtr     bsplist;
} QualFormData, PNTR QualFormPtr;

static void LIBCALLBACK AsnWriteQualifierForDCallBack (AsnExpOptStructPtr pAEOS)

{
  CharPtr      pchFind;
  CharPtr      pchSource;
  QualFormPtr  qfp;

  qfp = (QualFormPtr) pAEOS->data;
  if (ISA_STRINGTYPE (AsnFindBaseIsa (pAEOS->atp))) {
	pchSource = (CharPtr) pAEOS->dvp->ptrvalue;
	pchFind = qfp->findStr;
	if (StringSearch (pchSource, pchFind) != NULL) {
	  qfp->stringfound = TRUE;
	}
  }
}

static Boolean QualifierHasSubstring (ObjMgrTypePtr omtp, AsnIoPtr aip, Pointer ptr, QualFormPtr qfp)

{
  qfp->stringfound = FALSE;
  (omtp->asnwrite) (ptr, aip, NULL);
  return qfp->stringfound;
}

static void CommentToNote (SeqFeatPtr sfp)

{
  GBQualPtr  gbqual;

  if (sfp == NULL || sfp->comment == NULL) return;
  gbqual = GBQualNew ();
  if (gbqual == NULL) return;
  gbqual->qual = StringSave ("note");
  gbqual->val = sfp->comment;
  sfp->comment = NULL;
  gbqual->next = sfp->qual;
  sfp->qual = gbqual;
}

static void NoteToComment (SeqFeatPtr sfp)

{
  GBQualPtr       gbqual;
  size_t          len;
  GBQualPtr       nextqual;
  GBQualPtr PNTR  prevqual;
  CharPtr         str;

  if (sfp == NULL) return;
  gbqual = sfp->qual;
  prevqual = (GBQualPtr PNTR) &(sfp->qual);
  while (gbqual != NULL) {
    nextqual = gbqual->next;
    if (StringICmp (gbqual->qual, "note") == 0) {
       *(prevqual) = gbqual->next;
       gbqual->next = NULL;
       if (sfp->comment == NULL) {
         sfp->comment = gbqual->val;
       } else {
         len = StringLen (sfp->comment) + StringLen (gbqual->val) + 5;
         str = MemNew (sizeof (Char) * len);
         StringCpy (str, sfp->comment);
         StringCat (str, "; ");
         StringCat (str, gbqual->val);
         sfp->comment = MemFree (sfp->comment);
         gbqual->val = MemFree (gbqual->val);
         sfp->comment = str;
       }
       gbqual->val = NULL;
       GBQualFree (gbqual);
    } else {
      prevqual = (GBQualPtr PNTR) &(gbqual->next);
    }
    gbqual = nextqual;
  }
}

static void EditQualifierString (QualFormPtr qfp, GBQualPtr gbqual, CharPtr foundit)

{
  size_t   diff;
  size_t   foundlen;
  size_t   replen;
  CharPtr  newstring;
  CharPtr  tmp;
  CharPtr  tmp2;

  if (qfp == NULL || gbqual == NULL || foundit == NULL) return;
  foundlen = StringLen (qfp->findStr);
  replen = StringLen (qfp->replaceStr);
  if (replen > foundlen) {
    diff = replen - foundlen;
  } else {
    diff = foundlen - replen;
  }
  newstring = MemNew (StringLen (gbqual->val) + diff + 1);
  tmp = gbqual->val;
  tmp2 = newstring;
  while (tmp != foundit) {
    *tmp2 = *tmp;
    tmp++;
    tmp2++;
  }
  if (qfp->replaceStr != NULL) {
    tmp2 = MemCopy (tmp2, qfp->replaceStr, replen);
  }
  tmp2 += replen;
  tmp += foundlen;
  tmp2 = StringMove (tmp2, tmp);
  gbqual->val = MemFree (gbqual->val);
  gbqual->val = newstring;
}

static void CommonQualifierCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  AsnExpOptPtr       aeop;
  AsnIoPtr           aip;
  EnumFieldAssocPtr  ap;
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  Uint2              choice;
  UIEnum             enumval;
  CharPtr            foundit;
  GBQualPtr          gbqual;
  CharPtr            genelocus;
  GeneRefPtr         grp;
  Int2               i;
  ImpFeatPtr         ifp;
  Int2               j;
  Char               label [256];
  Int2               lookfor;
  Uint2              newchoice;
  Int2               newval;
  GBQualPtr          nextqual;
  SeqAnnotPtr        nextsap;
  SeqFeatPtr         nextsfp;
  Boolean            notext;
  ObjMgrTypePtr      omtp;
  GBQualPtr PNTR     prevqual;
  BioseqPtr          productbsp;
  ProtRefPtr         prp;
  QualFormPtr        qfp;
  Int2               qual;
  RnaRefPtr          rrp;
  SeqAnnotPtr        sap;
  SeqFeatPtr         sfp;
  SeqIdPtr           sip;
  CharPtr            str;
  Uint2              subtype;
  tRNAPtr            trp;
  Int2               val;
  ValNodePtr         vnp;

  if (mydata == NULL) return;
  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  qfp = (QualFormPtr) mydata;
  if (qfp == NULL) return;
  if (qfp->abortconvert) return;
  omtp = qfp->omtp;
  if (omtp == NULL || omtp->subtypefunc == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  val = 0;
  newval = 0;
  switch (qfp->type) {
    case REMOVE_QUAL :
      val = GetValue (qfp->fromquallist);
      if (val == 0) return;
      break;
    case CONVERT_FEAT :
      if (GetEnumPopup ((PopuP) qfp->tofeatlist, qfp->alist, &enumval)) {
        newval = (Int2) enumval;
      }
      if (newval == 0) return;
      break;
    case CONVERT_QUAL :
      val = GetValue (qfp->fromquallist);
      if (val == 0) return;
      newval = GetValue (qfp->toquallist);
      if (newval == 0) return;
      break;
    case EDIT_QUAL :
      val = GetValue (qfp->fromquallist);
      if (val == 0) return;
      break;
    case ADD_QUAL :
      val = GetValue (qfp->fromquallist);
      if (val == 0) return;
      break;
    default :
      return;
  }
  qfp->findStr = JustSaveStringFromText (qfp->findthis);
  notext = StringHasNoText (qfp->findStr);
  qfp->replaceStr = JustSaveStringFromText (qfp->replacewith);
  if (qfp->type == EDIT_QUAL && notext) return;
  if (qfp->type == ADD_QUAL && notext) return;
  aip = AsnIoNullOpen ();
  aeop = AsnExpOptNew (aip, NULL, NULL, AsnWriteQualifierForDCallBack);
  if (aeop != NULL) {
    aeop->user_data = (Pointer) qfp;
  }
  while (sap != NULL) {
    nextsap = sap->next;
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        nextsfp = sfp->next;
        subtype = (*(omtp->subtypefunc)) ((Pointer) sfp);
        lookfor = qfp->subtype;
        if (qfp->type != CONVERT_FEAT) {
          if (qfp->subtype == 0) {
            lookfor = subtype;
          }
        }
        if (subtype == lookfor) {
          if (qfp->type == ADD_QUAL) {
            if (sfp->data.choice == SEQFEAT_IMP) {
              if (val - 1 == GBQUAL_partial) {
                sfp->partial = FALSE;
              } else if (val - 1 == GBQUAL_evidence) {
                sfp->exp_ev = 0;
              } else {
                gbqual = GBQualNew ();
                if (gbqual != NULL) {
                  gbqual->qual = StringSave (ParFlat_GBQual_names [val - 1].name);
                  gbqual->val = StringSave (qfp->findStr);
                  gbqual->next = sfp->qual;
                  sfp->qual = gbqual;
                  if (val - 1 == GBQUAL_note) {
                    NoteToComment (sfp);
                  }
                }
              }
            }
          } else if (notext || QualifierHasSubstring (omtp, aip, (Pointer) sfp, qfp)) {
            if (qfp->type == CONVERT_FEAT) {
              choice = FindFeatFromFeatDefType (qfp->subtype);
              newchoice = FindFeatFromFeatDefType (newval);
              /*
              if (choice != newchoice) {
                ArrowCursor ();
                Message (MSG_OK, "This conversion not supported - contact sequindev for instructions.");
                qfp->abortconvert = TRUE;
                return;
              }
              */
              if (choice == SEQFEAT_IMP && newchoice == SEQFEAT_PROT) {
                ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
                if (ifp != NULL) {
                  switch (newval) {
                    case FEATDEF_mat_peptide_aa :
                      ifp->key = MemFree (ifp->key);
                      ifp->key = StringSave ("mat_peptide");
                      break;
                    case FEATDEF_sig_peptide_aa :
                      ifp->key = MemFree (ifp->key);
                      ifp->key = StringSave ("sig_peptide");
                      break;
                    case FEATDEF_transit_peptide_aa :
                      ifp->key = MemFree (ifp->key);
                      ifp->key = StringSave ("transit_peptide");
                      break;
                  }
                }
              } else if (choice == SEQFEAT_IMP &&
                         (newval == FEATDEF_misc_RNA ||
                         newval == FEATDEF_precursor_RNA)) {
                rrp = RnaRefNew ();
                if (rrp != NULL) {
                  sfp->data.value.ptrvalue = ImpFeatFree ((ImpFeatPtr) sfp->data.value.ptrvalue);
                  sfp->data.choice = SEQFEAT_RNA;
                  sfp->data.value.ptrvalue = (Pointer) rrp;
                  if (newval == FEATDEF_precursor_RNA) {
                    rrp->type = 1;
                  } else {
                    rrp->type = 255;
                  }
                }
              } else if ((choice == SEQFEAT_GENE || choice == SEQFEAT_CDREGION) && newchoice == SEQFEAT_RNA) {
                rrp = RnaRefNew ();
                if (rrp != NULL) {
                  FeatDefLabel (sfp, label, sizeof (label), OM_LABEL_CONTENT);
                  if (choice == SEQFEAT_GENE) {
                    sfp->data.value.ptrvalue = GeneRefFree ((GeneRefPtr) sfp->data.value.ptrvalue);
                  } else if (choice == SEQFEAT_CDREGION) {
                    if (sfp->product != NULL) {
                      sip = SeqLocId (sfp->product);
                      if (sip != NULL) {
                        productbsp = BioseqFind (sip);
                        if (productbsp != NULL) {
                          ValNodeAddPointer (&(qfp->bsplist), 0, (Pointer) productbsp);
                        }
                      }
                    }
                    sfp->data.value.ptrvalue = CdRegionFree ((CdRegionPtr) sfp->data.value.ptrvalue);
                  }
                  sfp->data.choice = SEQFEAT_RNA;
                  sfp->data.value.ptrvalue = (Pointer) rrp;
                  switch (newval) {
                    case FEATDEF_preRNA :
                      rrp->type = 1;
                      break;
                    case FEATDEF_mRNA :
                      rrp->type = 2;
                      break;
                    case FEATDEF_tRNA :
                      rrp->type = 3;
                      break;
                    case FEATDEF_rRNA :
                      rrp->type = 4;
                      break;
                    case FEATDEF_snRNA :
                      rrp->type = 5;
                      break;
                    case FEATDEF_scRNA :
                      rrp->type = 6;
                      break;
                    case FEATDEF_snoRNA :
                      rrp->type = 7;
                      break;
                    case FEATDEF_otherRNA :
                      rrp->type = 255;
                      break;
                    default :
                      break;
                  }
                  if (newval == FEATDEF_tRNA) {
                    trp = (tRNAPtr) MemNew (sizeof (tRNA));
                    rrp->ext.choice = 2;
                    rrp->ext.value.ptrvalue = (Pointer) trp;
                    if (trp != NULL) {
                      trp->aa = ParseTRnaString (label, NULL, NULL);
                      trp->aatype = 2;
                      for (j = 0; j < 6; j++) {
                        trp->codon [j] = 255;
                      }
                    }
                  } else if (! StringHasNoText (label)) {
                    rrp->ext.choice = 1;
                    rrp->ext.value.ptrvalue = StringSave (label);
                  }
                }
              } else if (choice == SEQFEAT_REGION && newchoice == SEQFEAT_IMP) {
                ifp = ImpFeatNew ();
                if (ifp != NULL) {
                  str = (CharPtr) sfp->data.value.ptrvalue;
                  sfp->data.choice = SEQFEAT_IMP;
                  sfp->data.value.ptrvalue = (Pointer) ifp;
                  for (i = 1, ap = qfp->alist; ap->name != NULL; i++, ap++) {
                    if (ap->value == newval) {
                      ifp->key = MemFree (ifp->key);
                      ifp->key = StringSave (ap->name);
                    }
                  }
                  if (ifp->key == NULL) {
                    ifp->key = StringSave ("misc_feature");
                  }
                  if (! StringHasNoText (str)) {
                    gbqual = GBQualNew ();
                    if (gbqual != NULL) {
                      gbqual->qual = StringSave ("note");
                      gbqual->val = str;
                      gbqual->next = sfp->qual;
                      sfp->qual = gbqual;
                    }
                  }
                }
              } else if (choice == SEQFEAT_REGION &&
                         (newval == FEATDEF_misc_RNA ||
                         newval == FEATDEF_precursor_RNA)) {
                rrp = RnaRefNew ();
                if (rrp != NULL) {
                  str = (CharPtr) sfp->data.value.ptrvalue;
                  sfp->data.choice = SEQFEAT_RNA;
                  sfp->data.value.ptrvalue = (Pointer) rrp;
                  if (newval == FEATDEF_precursor_RNA) {
                    rrp->type = 1;
                  } else {
                    rrp->type = 255;
                  }
                  if (! StringHasNoText (str)) {
                    rrp->ext.choice = 1;
                    rrp->ext.value.ptrvalue = str;
                  }
                }
              } else if (choice == SEQFEAT_IMP && newchoice == SEQFEAT_RNA) {
                rrp = RnaRefNew ();
                if (rrp != NULL) {
                  sfp->data.value.ptrvalue = ImpFeatFree ((ImpFeatPtr) sfp->data.value.ptrvalue);
                  sfp->data.choice = SEQFEAT_RNA;
                  sfp->data.value.ptrvalue = (Pointer) rrp;
                  switch (newval) {
                    case FEATDEF_preRNA :
                      rrp->type = 1;
                      break;
                    case FEATDEF_mRNA :
                      rrp->type = 2;
                      break;
                    case FEATDEF_tRNA :
                      rrp->type = 3;
                      break;
                    case FEATDEF_rRNA :
                      rrp->type = 4;
                      break;
                    case FEATDEF_snRNA :
                      rrp->type = 5;
                      break;
                    case FEATDEF_scRNA :
                      rrp->type = 6;
                      break;
                    case FEATDEF_snoRNA :
                      rrp->type = 7;
                      break;
                    case FEATDEF_otherRNA :
                      rrp->type = 255;
                      break;
                    default :
                      break;
                  }
                }
              } else if (choice == SEQFEAT_COMMENT && newval == FEATDEF_misc_feature) {
                if (sfp->data.value.ptrvalue == NULL) {
                  ifp = ImpFeatNew ();
                  if (ifp != NULL) {
                    ifp->key = StringSave ("misc_feature");
                    sfp->data.choice = SEQFEAT_IMP;
                    sfp->data.value.ptrvalue = (Pointer) ifp;
                  }
                }
              } else if (choice == SEQFEAT_GENE && newval == FEATDEF_misc_feature) {
                ifp = ImpFeatNew ();
                if (ifp != NULL) {
                  genelocus = NULL;
                  grp = (GeneRefPtr) sfp->data.value.ptrvalue;
                  if (grp != NULL) {
                    genelocus = grp->locus;
                    grp->locus = NULL;
                  }
                  sfp->data.value.ptrvalue = GeneRefFree ((GeneRefPtr) sfp->data.value.ptrvalue);
                  sfp->data.choice = SEQFEAT_IMP;
                  sfp->data.value.ptrvalue = (Pointer) ifp;
                  ifp->key = StringSave ("misc_feature");
                  if (! StringHasNoText (genelocus)) {
                    gbqual = GBQualNew ();
                    if (gbqual != NULL) {
                      gbqual->qual = StringSave ("gene");
                      gbqual->val = genelocus;
                      gbqual->next = sfp->qual;
                      sfp->qual = gbqual;
                    }
                  }
                }
              } else if (choice == SEQFEAT_PROT &&
                         (qfp->subtype == FEATDEF_mat_peptide_aa ||
                          qfp->subtype == FEATDEF_sig_peptide_aa ||
                          qfp->subtype == FEATDEF_transit_peptide_aa) &&
                         newchoice == SEQFEAT_REGION) {
                prp = (ProtRefPtr) sfp->data.value.ptrvalue;
                if (prp != NULL) {
                  vnp = prp->name;
                  if (vnp != NULL && vnp->next == NULL) {
                    str = (CharPtr) vnp->data.ptrvalue;
                    if (! StringHasNoText (str)) {
                      vnp->data.ptrvalue = NULL;
                      sfp->data.value.ptrvalue = ProtRefFree (prp);
                      sfp->data.choice = SEQFEAT_REGION;
                      sfp->data.value.ptrvalue = (Pointer) str;
                    }
                  }
                }
              } else if (choice != newchoice) {
                ArrowCursor ();
                Message (MSG_OK, "This conversion not supported - contact sequindev for instructions.");
                qfp->abortconvert = TRUE;
                return;
              } else if (choice == SEQFEAT_IMP) {
                ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
                if (ifp != NULL) {
                  for (i = 1, ap = qfp->alist; ap->name != NULL; i++, ap++) {
                    if (ap->value == newval) {
                      ifp->key = MemFree (ifp->key);
                      ifp->key = StringSave (ap->name);
                    }
                  }
                }
              } else if (choice == SEQFEAT_RNA) {
                rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
                if (rrp != NULL) {
                  switch (newval) {
                    case FEATDEF_preRNA :
                      rrp->type = 1;
                      break;
                    case FEATDEF_mRNA :
                      rrp->type = 2;
                      break;
                    case FEATDEF_tRNA :
                      rrp->type = 3;
                      break;
                    case FEATDEF_rRNA :
                      rrp->type = 4;
                      break;
                    case FEATDEF_snRNA :
                      rrp->type = 5;
                      break;
                    case FEATDEF_scRNA :
                      rrp->type = 6;
                      break;
                    case FEATDEF_snoRNA :
                      rrp->type = 7;
                      break;
                    case FEATDEF_otherRNA :
                      rrp->type = 255;
                      break;
                    default :
                      break;
                  }
                }
              } else if (choice == SEQFEAT_PROT) {
                prp = (ProtRefPtr) sfp->data.value.ptrvalue;
                if (prp != NULL) {
                  switch (newval) {
                    case FEATDEF_PROT :
                      prp->processed = 0;
                      break;
                    case FEATDEF_preprotein :
                      prp->processed = 1;
                      break;
                    case FEATDEF_mat_peptide_aa :
                      prp->processed = 2;
                      break;
                    case FEATDEF_sig_peptide_aa :
                      prp->processed = 3;
                      break;
                    case FEATDEF_transit_peptide_aa :
                      prp->processed = 4;
                      break;
                    default :
                      break;
                  }
                }
              }
            } else {
              if (val - 1 == GBQUAL_note || newval - 1 == GBQUAL_note) {
                CommentToNote (sfp);
              }
              if (qfp->type != CONVERT_QUAL) {
                if (val - 1 == GBQUAL_partial) {
                  sfp->partial = FALSE;
                } else if (val - 1 == GBQUAL_evidence) {
                  sfp->exp_ev = 0;
                }
              }
              gbqual = sfp->qual;
              prevqual = (GBQualPtr PNTR) &(sfp->qual);
              while (gbqual != NULL) {
                nextqual = gbqual->next;
                qual = GBQualNameValid (gbqual->qual);
                if (qual > -1 && qual == val - 1) {
                  if (qfp->type == CONVERT_QUAL) {
                    gbqual->qual = MemFree (gbqual->qual);
                    gbqual->qual = StringSave (ParFlat_GBQual_names [newval - 1].name);
                    prevqual = (GBQualPtr PNTR) &(gbqual->next);
                  } else if (qfp->type == EDIT_QUAL) {
                    foundit = StringStr (gbqual->val, qfp->findStr);
                    if (foundit != NULL) {
                      EditQualifierString (qfp, gbqual, foundit);
                    }
                    prevqual = (GBQualPtr PNTR) &(gbqual->next);
                  } else {
                    *(prevqual) = gbqual->next;
                    gbqual->next = NULL;
                    GBQualFree (gbqual);
                  }
                } else {
                  prevqual = (GBQualPtr PNTR) &(gbqual->next);
                }
                gbqual = nextqual;
              }
              if (val - 1 == GBQUAL_note || newval - 1 == GBQUAL_note) {
                NoteToComment (sfp);
              }
            }
          }
          if (qfp->type == REMOVE_QUAL && val - 1 == GBQUAL_citation) {
            sfp->cit = PubSetFree (sfp->cit);
          }
        }
        sfp = nextsfp;
      }
    }
    sap = nextsap;
  }
  AsnIoClose (aip);
}

static void DoProcessQualifier (ButtoN b)

{
  BioseqPtr      bsp;
  Uint2          choice;
  UIEnum         enumval;
  Uint2          itemID;
  Uint2          newchoice;
  OMProcControl  ompc;
  QualFormPtr    qfp;
  SeqEntryPtr    sep;
  ValNodePtr     tmp;
  Int2           val;

  qfp = (QualFormPtr) GetObjectExtra (b);
  if (qfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (qfp->input_entityID);
  if (sep == NULL) return;
  Hide (qfp->form);
  WatchCursor ();
  Update ();
  qfp->itemtype = OBJ_SEQFEAT;
  qfp->subtype = 0;
  qfp->abortconvert = FALSE;
  qfp->bsplist = NULL;
  val = 0;
  if (GetEnumPopup ((PopuP) qfp->fromfeatlist, qfp->alist, &enumval)) {
    val = (Int2) enumval;
  }
  if (val > 0) {
    qfp->subtype = val;
  }
  if (qfp->subtype != 0 || qfp->type != CONVERT_FEAT) {
    qfp->omp = ObjMgrGet ();
    qfp->omtp = NULL;
    if (qfp->omp != NULL) {
      qfp->omtp = ObjMgrTypeFind (qfp->omp, OBJ_SEQFEAT, NULL, NULL);
    }
    if (qfp->type == CONVERT_FEAT) {
      val = 0;
      if (GetEnumPopup ((PopuP) qfp->tofeatlist, qfp->alist, &enumval)) {
        val = (Int2) enumval;
      }
      if (val > 0) {
        choice = FindFeatFromFeatDefType (qfp->subtype);
        newchoice = FindFeatFromFeatDefType (val);
        if (choice == SEQFEAT_IMP && newchoice == SEQFEAT_CDREGION) {
          ArrowCursor ();
          Update ();
          qfp->findStr = JustSaveStringFromText (qfp->findthis);
          PrepareToConvertToCDS (sep, qfp->input_entityID, qfp->subtype, qfp->findStr);
          Remove (qfp->form);
          return;
        } else if (choice == SEQFEAT_IMP && newchoice == SEQFEAT_PROT) {
          /* convert to mat_peptide, sig_peptide, or transit_peptide imp feat (briefly) */
        } else if (choice == SEQFEAT_IMP && (val == FEATDEF_misc_RNA || val == FEATDEF_precursor_RNA)) {
          /* convert to misc_RNA or pre_RNA imp feat (briefly) */
        } else if (choice == SEQFEAT_REGION && (val == FEATDEF_misc_RNA || val == FEATDEF_precursor_RNA)) {
        } else if (choice == SEQFEAT_REGION && newchoice == SEQFEAT_IMP) {
          /* now allowed */
        } else if (choice == SEQFEAT_IMP && newchoice == SEQFEAT_RNA) {
          /* now allowed */
        } else if (choice == SEQFEAT_COMMENT && val == FEATDEF_misc_feature) {
          /* convert to misc_feature imp feat (briefly) */
        } else if (choice == SEQFEAT_GENE && val == FEATDEF_misc_feature) {
          /* convert to misc_feature imp feat (briefly) */
        } else if ((choice == SEQFEAT_GENE || choice == SEQFEAT_CDREGION) && newchoice == SEQFEAT_RNA) {
        } else if (choice == SEQFEAT_PROT &&
                   (qfp->subtype == FEATDEF_mat_peptide_aa ||
                    qfp->subtype == FEATDEF_sig_peptide_aa ||
                    qfp->subtype == FEATDEF_transit_peptide_aa) &&
                   newchoice == SEQFEAT_REGION) {
          /* convert from mat_peptide, sig_peptide, or transit_peptide to region */
        } else if (choice != newchoice) {
          ArrowCursor ();
          Update ();
          Message (MSG_OK, "This conversion not supported - contact sequindev for instructions.");
          Remove (qfp->form);
          return;
        }
      }
    }
    if (qfp->itemtype != 0 && qfp->omtp != NULL) {
      SeqEntryExplore (sep, (Pointer) qfp, CommonQualifierCallback);
    }
  }
  ArrowCursor ();
  Update ();
  if (qfp->bsplist != NULL) {
    if (Message (MSG_YN, "Remove protein products?") == ANS_YES) {
      for (tmp = qfp->bsplist; tmp != NULL; tmp = tmp->next) {
        bsp = (BioseqPtr) tmp->data.ptrvalue;
        itemID = GetItemIDGivenPointer (qfp->input_entityID, OBJ_BIOSEQ, (Pointer) bsp);
        if (itemID > 0) {
          MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
          ompc.do_not_reload_from_cache = TRUE;
          ompc.input_entityID = qfp->input_entityID;
          ompc.input_itemID = itemID;
          ompc.input_itemtype = OBJ_BIOSEQ;
          if (! DetachDataForProc (&ompc, FALSE)) {
            Message (MSG_POSTERR, "DetachDataForProc failed");
          }
        }
      }
    }
    RenormalizeNucProtSets (sep, TRUE);
  }
  ObjMgrSetDirtyFlag (qfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, qfp->input_entityID, 0, 0);
  Remove (qfp->form);
}

static void QualMessageProc (ForM f, Int2 mssg)

{
  QualFormPtr  qfp;

  qfp = (QualFormPtr) GetObjectExtra (f);
  if (qfp != NULL) {
    if (qfp->appmessage != NULL) {
      qfp->appmessage (f, mssg);
    }
  }
}

static void CleanupQualForm (GraphiC g, VoidPtr data)

{
  Int2         j;
  QualFormPtr  qfp;

  qfp = (QualFormPtr) data;
  if (qfp != NULL) {
    MemFree (qfp->findStr);
    MemFree (qfp->replaceStr);
    if (qfp->realalist != NULL) {
      for (j = 0; qfp->realalist [j].name != NULL; j++) {
        MemFree (qfp->realalist [j].name);
      }
    }
    MemFree (qfp->realalist);
    ValNodeFree (qfp->bsplist);
  }
  StdCleanupFormProc (g, data);
}

static void ProcessQualifier (IteM i, Int2 type)

{
  EnumFieldAssocPtr  ap;
  BaseFormPtr        bfp;
  ButtoN             b;
  GrouP              c;
  GrouP              g;
  GrouP              h;
  Int2               j;
  GrouP              k;
  Int2               listHeight;
  QualFormPtr        qfp;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  CharPtr            str;
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
  qfp = (QualFormPtr) MemNew (sizeof (QualFormData));
  if (qfp == NULL) return;
  qfp->type = type;
  switch (type) {
    case REMOVE_QUAL :
      title = "Qualifier Removal";
      break;
    case CONVERT_FEAT :
      title = "Feature Conversion";
      break;
    case CONVERT_QUAL :
      title = "Qualifier Conversion";
      break;
    case EDIT_QUAL :
      title = "Edit Qualifier";
      break;
    case ADD_QUAL :
      title = "Add Qualifier";
      break;
    default :
      title = "?";
      break;
  }
  w = FixedWindow (-50, -33, -10, -10, title, StdCloseWindowProc);
  SetObjectExtra (w, qfp, CleanupQualForm);
  qfp->form = (ForM) w;
  qfp->formmessage = QualMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    qfp->appmessage = sepp->handleMessages;
  }

  qfp->input_entityID = bfp->input_entityID;
  qfp->input_itemID = bfp->input_itemID;
  qfp->input_itemtype = bfp->input_itemtype;

  if (type == CONVERT_FEAT) {
    ap = import_featdef_alist (TRUE, FALSE, TRUE);
  } else {
    ap = import_featdef_alist (FALSE, FALSE, FALSE);
  }
  qfp->realalist = ap;
  if (type == CONVERT_FEAT) {
    ap++;
  } else {
    ap->name = MemFree (ap->name);
    ap->name = StringSave ("[ALL FEATURES]");
  }
  qfp->alist = ap;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 0, 2, NULL);

  if (type == CONVERT_FEAT) {
    StaticPrompt (g, "From Feature", 0, 0, programFont, 'c');
  } else {
    StaticPrompt (g, "Feature", 0, 0, programFont, 'c');
  }
  if (indexerVersion) {
    listHeight = 16;
  } else {
    listHeight = 8;
  }
  qfp->fromfeatlist = SingleList (g, 10, listHeight, NULL);
  for (ap = qfp->alist; ap->name != NULL; ap++) {
    ListItem (qfp->fromfeatlist, ap->name);
  }

  if (type == CONVERT_FEAT) {
    StaticPrompt (g, "To Feature", 0, 0, programFont, 'c');
    qfp->tofeatlist = SingleList (g, 10, listHeight, NULL);
    for (ap = qfp->alist; ap->name != NULL; ap++) {
      ListItem (qfp->tofeatlist, ap->name);
    }
  } else {
    if (type == CONVERT_QUAL) {
      StaticPrompt (g, "From Qualifier", 0, 0, programFont, 'c');
    } else {
      StaticPrompt (g, "Qualifier", 0, 0, programFont, 'c');
    }
    qfp->fromquallist = SingleList (g, 10, listHeight, NULL);
    for (j = 0; j < ParFlat_TOTAL_GBQUAL; j++) {
      ListItem (qfp->fromquallist, ParFlat_GBQual_names [j].name);
    }
    if (type == CONVERT_QUAL) {
      StaticPrompt (g, "To Qualifier", 0, 0, programFont, 'c');
      qfp->toquallist = SingleList (g, 10, listHeight, NULL);
      for (j = 0; j < ParFlat_TOTAL_GBQUAL; j++) {
        ListItem (qfp->toquallist, ParFlat_GBQual_names [j].name);
      }
    }
  }

  if (type == EDIT_QUAL) {
    k = HiddenGroup (h, 2, 0, NULL);
  } else {
    k = HiddenGroup (h, 0, 2, NULL);
  }
  if (type == EDIT_QUAL) {
    StaticPrompt (k, "Find", 0, dialogTextHeight, programFont, 'l');
  } else if (type == ADD_QUAL) {
    StaticPrompt (k, "Text", 0, dialogTextHeight, programFont, 'l');
  } else {
    StaticPrompt (k, "Optional string constraint", 0, dialogTextHeight, programFont, 'c');
  }
  qfp->findthis = DialogText (k, "", 14, NULL);
  if (type == EDIT_QUAL) {
    StaticPrompt (k, "Replace", 0, dialogTextHeight, programFont, 'l');
    qfp->replacewith = DialogText (k, "", 14, NULL);
  }

  for (ap = qfp->alist; ap->name != NULL; ap++) {
    str = ap->name;
    if (*str == '~') {
      *str = '-';
    }
  }

  c = HiddenGroup (h, 4, 0, NULL);
  b = PushButton (c, "Accept", DoProcessQualifier);
  SetObjectExtra (b, qfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) k, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Select (qfp->findthis);
  Update ();
}

static void RemoveQualifier (IteM i)

{
  ProcessQualifier (i, REMOVE_QUAL);
}

static void ConvertFeatures (IteM i)

{
  ProcessQualifier (i, CONVERT_FEAT);
}

static void ConvertQualifier (IteM i)

{
  ProcessQualifier (i, CONVERT_QUAL);
}

static void EditQualifier (IteM i)

{
  ProcessQualifier (i, EDIT_QUAL);
}

static void AddQualifier (IteM i)

{
  ProcessQualifier (i, ADD_QUAL);
}

#define REMOVE_SOURCE    1
#define CONVERT_SOURCE   2
#define EDIT_SOURCE      3
#define ADD_SOURCE       4

static ENUM_ALIST(subsource_and_orgmod_subtype_alistX)
  {" ",                       0},
  {"Acronym",                19},
  {"Anamorph",               29},
  {"Authority",              24},
  {"Biotype",                14},
  {"Biovar",                 13},
  {"Breed",                  31},
  {"Cell-line",             108},
  {"Cell-type",             109},
  {"Chemovar",               12},
  {"Chromosome",            101},
  {"Clone",                 103},
  {"Clone-lib",             111},
  {"Common",                 18},
  {"Country",               123},
  {"Cultivar",               10},
  {"Dev-stage",             112},
  {"Dosage",                 20},
  {"Ecotype",                27},
  {"Endogenous_virus-name", 125},
  {"Environmental-sample",  127},
  {"Forma",                  25},
  {"Forma-specialis",        26},
  {"Frequency",             113},
  {"Genotype",              106},
  {"Germline",              114},
  {"Group",                  15},
  {"Haplotype",             105},
  {"Ins-seq-name",          121},
  {"Isolate",                17},
  {"Isolation-source",      128},
  {"Lab-host",              116},
  {"Map",                   102},
  {"Natural-host",           21},
  {"Old Lineage",            53}, /* 253 */
  {"Old Name",               54}, /* 254 */
  {"OrgMod Note",            55},
  {"Pathovar",               11},
  {"Plasmid-name",          119},
  {"Plastid-name",          122},
  {"Pop-variant",           117},
  {"Rearranged",            115},
  {"Segment",               124},
  {"Serogroup",               8},
  {"Serotype",                7},
  {"Serovar",                 9},
  {"Sex",                   107},
  {"Specimen-voucher",       23},
  {"Strain",                  2},
  {"Sub-species",            22},
  {"Subclone",              104},
  {"Subgroup",               16},
  {"SubSource Note",        155},
  {"Substrain",               3},
  {"Subtype",                 5},
  {"Synonym",                28},
  {"Teleomorph",             30},
  {"Tissue-lib",            118},
  {"Tissue-type",           110},
  {"Transgenic",            126},
  {"Transposon-name",       120},
  {"Type",                    4},
  {"Variety",                 6},
END_ENUM_ALIST

static ENUM_ALIST(biosource_genome_alistX)
  {" ",                    0},
  {"Genomic",              1},
  {"Chloroplast",          2},
  {"Chromoplast",          3},
  {"Kinetoplast",          4},
  {"Mitochondrion",        5},
  {"Plastid",              6},
  {"Macronuclear",         7},
  {"Extrachromosomal",     8},
  {"Plasmid",              9},
  {"Transposon",          10},
  {"Insertion Sequence",  11},
  {"Cyanelle",            12},
  {"Proviral",            13},
  {"Virion",              14},
  {"Nucleomorph",         15},
  {"Apicoplast",          16},
  {"Leucoplast",          17},
  {"Proplastid",          18},
  {"Endogenous-virus",    19},
END_ENUM_ALIST

static ENUM_ALIST(orgref_textfield_alist)
  {" ",                    0},
  {"Scientific Name",      1},
  {"Common Name",          2},
  {"Lineage",              3},
  {"Division",             4},
END_ENUM_ALIST

typedef struct sourceformdata {
  FEATURE_FORM_BLOCK

  Int2           type;
  ButtoN         applyToParts;
  TexT           onlyThisPart;
  GrouP          sourceGroup;
  GrouP          modGrp;
  GrouP          genGrp;
  GrouP          refGrp;
  GrouP          txtGrp;
  PopuP          frommod;
  PopuP          tomod;
  PopuP          fromgen;
  PopuP          togen;
  PopuP          fromref;
  PopuP          toref;
  TexT           findthis;
  TexT           replacewith;

  Int2           choice;
  Int2           fromval;
  Int2           toval;
  Int2           onlythis;
  CharPtr        findStr;
  CharPtr        replaceStr;

  Boolean        replaceOldAsked;
  Boolean        doReplaceAll;
} SourceFormData, PNTR SourceFormPtr;

static SubSourcePtr FindSubSource (BioSourcePtr biop, Uint1 subtype, SourceFormPtr sfp, Boolean forceRemove, Boolean convertNote)

{
  MsgAnswer     ans;
  SubSourcePtr  PNTR  prev;
  SubSourcePtr  ssp;
  SubSourcePtr  tmp;

  if (subtype == 55 || subtype == 155) {
    subtype = 255;
  }
  prev = &(biop->subtype);
  for (ssp = biop->subtype; ssp != NULL; ssp = ssp->next) {
    if (ssp->subtype == subtype) {
      if (sfp->type == REMOVE_SOURCE || forceRemove) {
        if (StringHasNoText (sfp->findStr) ||
            StringISearch (ssp->name, sfp->findStr) != NULL) {
          *prev = ssp->next;
          SubSourceFree (ssp);
          return NULL;
        }
      } else if (sfp->type == ADD_SOURCE) {
        if (! sfp->replaceOldAsked) {
          sfp->replaceOldAsked = TRUE;
          ArrowCursor ();
          ans = Message (MSG_YN, "Do you wish to overwrite existing modifiers?");
          WatchCursor ();
          Update ();
          sfp->doReplaceAll = (Boolean) (ans == ANS_YES);
        }
        if (sfp->doReplaceAll) {
          return ssp;
        }
      } else {
        return ssp;
      }
    }
    prev = &(ssp->next);
  }
  if (sfp->type == REMOVE_SOURCE || forceRemove) return NULL;
  if (sfp->type != ADD_SOURCE && (! convertNote)) return NULL;
  ssp = SubSourceNew ();
  if (biop->subtype == NULL) {
    biop->subtype = ssp;
  } else {
    tmp = biop->subtype;
    while (tmp->next != NULL) {
      tmp = tmp->next;
    }
    tmp->next = ssp;
  }
  if (ssp != NULL) {
    ssp->subtype = subtype;
  }
  return ssp;
}

static OrgModPtr FindOrgMod (BioSourcePtr biop, Uint1 subtype, SourceFormPtr sfp, Boolean forceRemove, Boolean convertNote)

{
  MsgAnswer   ans;
  OrgModPtr   mod;
  OrgNamePtr  onp;
  OrgRefPtr   orp;
  OrgModPtr   PNTR  prev;
  OrgModPtr   tmp;

  if (subtype == 55 || subtype == 155) {
    subtype = 255;
  } else if (subtype == 53) {
    subtype = 253;
  } else if (subtype == 54) {
    subtype = 254;
  }
  mod = NULL;
  orp = biop->org;
  if (orp == NULL) {
    if (sfp->type != ADD_SOURCE) return NULL;
    orp = OrgRefNew ();
    biop->org = orp;
  }
  if (orp == NULL) return NULL;
  onp = orp->orgname;
  if (onp == NULL) {
    if (sfp->type != ADD_SOURCE) return NULL;
    onp = OrgNameNew ();
    orp->orgname = onp;
  }
  if (onp == NULL) return NULL;
  prev = &(onp->mod);
  for (mod = onp->mod; mod != NULL; mod = mod->next) {
    if (mod->subtype == subtype) {
      if (sfp->type == REMOVE_SOURCE || forceRemove) {
        if (StringHasNoText (sfp->findStr) ||
            StringISearch (mod->subname, sfp->findStr) != NULL) {
          *prev = mod->next;
          OrgModFree (mod);
          return NULL;
        }
      } else if (sfp->type == ADD_SOURCE) {
        if (! sfp->replaceOldAsked) {
          sfp->replaceOldAsked = TRUE;
          ArrowCursor ();
          ans = Message (MSG_YN, "Do you wish to overwrite existing modifiers?");
          WatchCursor ();
          Update ();
          sfp->doReplaceAll = (Boolean) (ans == ANS_YES);
        }
        if (sfp->doReplaceAll) {
          return mod;
        }
      } else {
        return mod;
      }
    }
    prev = &(mod->next);
  }
  if (sfp->type == REMOVE_SOURCE || forceRemove) return NULL;
  if (sfp->type != ADD_SOURCE && (! convertNote)) return NULL;
  mod = OrgModNew ();
  if (onp->mod == NULL) {
    onp->mod = mod;
  } else {
    tmp = onp->mod;
    while (tmp->next != NULL) {
      tmp = tmp->next;
    }
    tmp->next = mod;
  }
  if (mod != NULL) {
    mod->subtype = subtype;
  }
  return mod;
}

static void ConvertSourceString (OrgRefPtr orp, Int2 fromval, Int2 toval)
{
  CharPtr     tmp;
  OrgNamePtr  onp;

  if (orp == NULL || fromval == toval) return;

  tmp = NULL;
  onp = orp->orgname;

  switch (fromval) {
    case 1 :
      tmp = orp->taxname;
      orp->taxname = NULL;
      break;
    case 2 :
      tmp = orp->common; 
      orp->common = NULL;
      break;
    case 3 :
      if (onp != NULL) {
         tmp = onp->lineage ;
         onp->lineage = NULL;
      }
      break;
    case 4 :
      if (onp != NULL) {
         tmp = onp->div ;
         onp->div = NULL;
      }
      break;
    default :
      break;
  }
 
  if (tmp == NULL) return;

  switch (toval) {
    case 1 :
      orp->taxname = tmp;
      break;
    case 2 :
      orp->common = tmp;
      break;
    case 3 :
      if (onp == NULL) {
         onp = OrgNameNew ();
         orp->orgname = onp;
      }
      onp->lineage = tmp;
      break;
    case 4 :
      onp = orp->orgname;
      if (onp == NULL) {
         onp = OrgNameNew ();
         orp->orgname = onp;
      }
      onp->div = tmp;
      break;
    default :
      break;
  }

  return;

} /* ConvertSourceString */

static void EditSourceString (CharPtr PNTR strptr, SourceFormPtr sfp, CharPtr foundit)

{
  size_t   diff;
  size_t   foundlen;
  size_t   replen;
  CharPtr  newstring;
  CharPtr  tmp;
  CharPtr  tmp2;

  if (sfp == NULL || strptr == NULL || foundit == NULL) return;
  foundlen = StringLen (sfp->findStr);
  replen = StringLen (sfp->replaceStr);
  if (replen > foundlen) {
    diff = replen - foundlen;
  } else {
    diff = foundlen - replen;
  }
  newstring = MemNew (StringLen (*strptr) + diff + 1);
  tmp = *strptr;
  tmp2 = newstring;
  while (tmp != foundit) {
    *tmp2 = *tmp;
    tmp++;
    tmp2++;
  }
  if (sfp->replaceStr != NULL) {
    tmp2 = MemCopy (tmp2, sfp->replaceStr, replen);
  }
  tmp2 += replen;
  tmp += foundlen;
  tmp2 = StringMove (tmp2, tmp);
  *strptr = MemFree (*strptr);
  *strptr = newstring;
}

static Uint1 AssignSubtype (Uint1 subtype)

{
  if (subtype >= 100) {
    subtype -= 100;
  }
  if (subtype == 55) {
    subtype = 255;
  }
  return subtype;
}

static void ProcessBioSourceFunc (BioSourcePtr biop, SourceFormPtr sfp)

{
  CharPtr       foundit;
  OrgModPtr     mod = NULL;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  SubSourcePtr  ssp = NULL;
  CharPtr       str = NULL;

  if (biop == NULL || sfp == NULL) return;
  if (sfp->choice == 1) {
    if (sfp->fromval >= 100) {
      ssp = FindSubSource (biop, sfp->fromval - 100, sfp, FALSE, FALSE);
      if (ssp == NULL) return;
      str = ssp->name;
    } else {
      mod = FindOrgMod (biop, sfp->fromval, sfp, FALSE, FALSE);
      if (mod == NULL) return;
      str = mod->subname;
    }
    switch (sfp->type) {
      case REMOVE_SOURCE :
        break;
      case CONVERT_SOURCE :
        if (sfp->toval < 1) return;
        if (StringHasNoText (sfp->findStr) || StringISearch (str, sfp->findStr) != NULL) {
          if (sfp->toval >= 100 && ssp != NULL) {
            ssp->subtype = AssignSubtype (sfp->toval);
          } else if (sfp->toval < 100 && mod != NULL) {
            mod->subtype = AssignSubtype (sfp->toval);
          } else if (sfp->toval < 100 && ssp != NULL) {
            mod = FindOrgMod (biop, sfp->toval, sfp, FALSE, TRUE);
            if (mod == NULL) return;
            mod->subname = StringSave (str);
            ssp = FindSubSource (biop, sfp->fromval - 100, sfp, TRUE, FALSE);
          } else if (sfp->toval >= 100 && mod != NULL) {
            ssp = FindSubSource (biop, sfp->toval - 100, sfp, FALSE, TRUE);
            if (ssp == NULL) return;
            ssp->name = StringSave (str);
            mod = FindOrgMod (biop, sfp->fromval, sfp, TRUE, FALSE);
          }
        }
        break;
      case EDIT_SOURCE :
        if (ssp != NULL) {
          foundit = StringISearch (ssp->name, sfp->findStr);
          if (foundit != NULL) {
            EditSourceString (&(ssp->name), sfp, foundit);
          }
        } else if (mod != NULL) {
          foundit = StringISearch (mod->subname, sfp->findStr);
          if (foundit != NULL) {
            EditSourceString (&(mod->subname), sfp, foundit);
          }
        }
        break;
      case ADD_SOURCE :
        if (ssp != NULL) {
          ssp->name = MemFree (ssp->name);
          ssp->name = StringSave (sfp->findStr);
          if (ssp->name == NULL &&
              (ssp->subtype == SUBSRC_germline ||
               ssp->subtype == SUBSRC_rearranged ||
               ssp->subtype == SUBSRC_transgenic ||
               ssp->subtype == SUBSRC_environmental_sample)) {
            ssp->name = StringSave ("");
          }
        } else if (mod != NULL) {
          mod->subname = MemFree (mod->subname);
          mod->subname = StringSave (sfp->findStr);
        }
        break;
      default :
        break;
    }
  } else if (sfp->choice == 2) {
    switch (sfp->type) {
      case REMOVE_SOURCE :
        if (sfp->fromval == 0 || biop->genome == sfp->fromval) {
          biop->genome = 0;
        }
        break;
      case CONVERT_SOURCE :
        if (biop->genome == sfp->fromval) {
          biop->genome = sfp->toval;
        }
        break;
      case EDIT_SOURCE :
        break;
      case ADD_SOURCE :
        biop->genome = sfp->fromval;
        break;
      default :
        break;
    }
  } else if (sfp->choice == 3) {
    onp = NULL;
    orp = biop->org;
    if (orp != NULL) {
      onp = orp->orgname;
    }
    switch (sfp->type) {
      case REMOVE_SOURCE :
        switch (sfp->fromval) {
          case 1 :
            if (orp != NULL) {
              orp->taxname = MemFree (orp->taxname);
            }
            break;
          case 2 :
            if (orp != NULL) {
              orp->common = MemFree (orp->common);
            }
            break;
          case 3 :
            if (onp != NULL) {
              onp->lineage = MemFree (onp->lineage);
            }
            break;
          case 4 :
            if (onp != NULL) {
              onp->div = MemFree (onp->div);
            }
            break;
          default :
            break;
        }
        break;
      case CONVERT_SOURCE :
        ConvertSourceString (orp, sfp->fromval, sfp->toval);
        break;
      case EDIT_SOURCE :
        switch (sfp->fromval) {
          case 1 :
            if (orp != NULL) {
              foundit = StringISearch (orp->taxname, sfp->findStr);
              if (foundit != NULL) {
                EditSourceString (&(orp->taxname), sfp, foundit);
              }
            }
            break;
          case 2 :
            if (orp != NULL) {
              foundit = StringISearch (orp->common, sfp->findStr);
              if (foundit != NULL) {
                EditSourceString (&(orp->common), sfp, foundit);
              }
            }
            break;
          case 3 :
            if (onp != NULL) {
              foundit = StringISearch (onp->lineage, sfp->findStr);
              if (foundit != NULL) {
                EditSourceString (&(onp->lineage), sfp, foundit);
              }
            }
            break;
          case 4 :
            if (onp != NULL) {
              foundit = StringISearch (onp->div, sfp->findStr);
              if (foundit != NULL) {
                EditSourceString (&(onp->div), sfp, foundit);
              }
            }
            break;
          default :
            break;
        }
        break;
      case ADD_SOURCE :
        break;
      default :
        break;
    }
  }
}

static Boolean ProcessSourceGatherFunc (GatherContextPtr gcp)

{
  BioSourcePtr   biop;
  SeqFeatPtr     feat;
  SourceFormPtr  sfp;
  ValNodePtr     vnp;

  if (gcp == NULL) return TRUE;
  sfp = (SourceFormPtr) gcp->userdata;
  if (sfp == NULL) return TRUE;
  if (gcp->thisitem == NULL) return TRUE;
  biop = NULL;
  if (gcp->thistype == OBJ_SEQFEAT) {
    feat = (SeqFeatPtr) gcp->thisitem;
    if (feat->data.choice == SEQFEAT_BIOSRC) {
      biop = (BioSourcePtr) feat->data.value.ptrvalue;
    }
  } else if (gcp->thistype == OBJ_SEQDESC) {
    vnp = (ValNodePtr) gcp->thisitem;
    if (vnp->choice == Seq_descr_source) {
      biop = (BioSourcePtr) vnp->data.ptrvalue;
    }
  }
  if (biop == NULL) return TRUE;
  ProcessBioSourceFunc (biop, sfp);
  return TRUE;
}

static void PutBioSourceOnPartAndProcess (SeqEntryPtr sep, Int2 onlythis, SourceFormPtr sfp)

{
  BioSourcePtr       biop;
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  Int2               count;
  SeqMgrDescContext  dcontext;
  SeqEntryPtr        nsep;
  SeqDescrPtr        sdp;
  SeqEntryPtr        tmp;

  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL) {
      if (onlythis != 0 && bssp->_class == BioseqseqSet_class_parts) {
        for (tmp = bssp->seq_set, count = 1;
             tmp != NULL && count != onlythis;
             tmp = tmp->next, count++) continue;
        if (tmp != NULL) {
          PutBioSourceOnPartAndProcess (tmp, onlythis, sfp);
        }
      } else {
        for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
          PutBioSourceOnPartAndProcess (tmp, onlythis, sfp);
        }
      }
      return;
    }
  }
  nsep = FindNucSeqEntry (sep);
  if (nsep == NULL) return;
  bsp = (BioseqPtr) nsep->data.ptrvalue;
  if (bsp == NULL) return;
  if (bsp->repr == Seq_repr_raw) {
    sdp = SeqEntryGetSeqDescr (nsep, Seq_descr_source, NULL);
    if (sdp != NULL) {
      biop = (BioSourcePtr) sdp->data.ptrvalue;
      ProcessBioSourceFunc (biop, sfp);
      return;
    }
    sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
    if (sdp == NULL) return;
    biop = (BioSourcePtr) sdp->data.ptrvalue;
    if (biop != NULL) {
      biop = (BioSourcePtr) AsnIoMemCopy ((Pointer) biop,
                                          (AsnReadFunc) BioSourceAsnRead,
                                          (AsnWriteFunc) BioSourceAsnWrite);
      if (biop != NULL) {
        sdp = CreateNewDescriptor (nsep, Seq_descr_source);
        if (sdp != NULL) {
          sdp->data.ptrvalue = (Pointer) biop;
          ProcessBioSourceFunc (biop, sfp);
        }
      }
    }
  }
}

static void DoProcessSource (ButtoN b)

{
  GatherScope    gs;
  SeqEntryPtr    sep;
  SourceFormPtr  sfp;
  Char           str [32];
  UIEnum         val;

  sfp = (SourceFormPtr) GetObjectExtra (b);
  if (sfp == NULL) return;
  Hide (sfp->form);
  WatchCursor ();
  Update ();
  sfp->fromval = 0;
  sfp->toval = 0;
  sfp->choice = GetValue (sfp->sourceGroup);
  if (sfp->choice == 1) {
    if (GetEnumPopup (sfp->frommod, subsource_and_orgmod_subtype_alistX, &val)) {
      sfp->fromval = (Int2) val;
    }
    if (GetEnumPopup (sfp->tomod, subsource_and_orgmod_subtype_alistX, &val)) {
      sfp->toval = (Int2) val;
    }
  } else if (sfp->choice == 2) {
    if (GetEnumPopup (sfp->fromgen, biosource_genome_alistX, &val)) {
      sfp->fromval = (Int2) val;
    }
    if (GetEnumPopup (sfp->togen, biosource_genome_alistX, &val)) {
      sfp->toval = (Int2) val;
    }
  } else if (sfp->choice == 3) {
    if (GetEnumPopup (sfp->fromref, orgref_textfield_alist, &val)) {
      sfp->fromval = (Int2) val;
    }
    if (GetEnumPopup (sfp->toref, orgref_textfield_alist, &val)) {
      sfp->toval = (Int2) val;
    }
  }
  if (sfp->fromval > 0 || (sfp->choice == 3 || sfp->type == REMOVE_SOURCE)) {
    sfp->findStr = JustSaveStringFromText (sfp->findthis);
    sfp->replaceStr = JustSaveStringFromText (sfp->replacewith);
    if (sfp->type != EDIT_SOURCE) {
      TrimSpacesAroundString (sfp->findStr);
      TrimSpacesAroundString (sfp->replaceStr);
    }
    if (sfp->type != EDIT_SOURCE || (! StringHasNoText (sfp->findStr))) {
      sfp->replaceOldAsked = FALSE;
      sfp->doReplaceAll = FALSE;
      sfp->onlythis = 0;
      if (sfp->applyToParts != NULL && GetStatus (sfp->applyToParts)) {
        GetTitle (sfp->onlyThisPart, str, sizeof (str));
        if (! StrToInt (str, &(sfp->onlythis))) {
          sfp->onlythis = 0;
        }
      }
      if (sfp->onlythis > 0) {
        sep = GetTopSeqEntryForEntityID (sfp->input_entityID);
        PutBioSourceOnPartAndProcess (sep, sfp->onlythis, sfp);
      } else {
        MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
        gs.seglevels = 1;
        MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
        gs.ignore[OBJ_BIOSEQ] = FALSE;
        gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
        gs.ignore[OBJ_SEQANNOT] = FALSE;
        gs.ignore[OBJ_SEQFEAT] = FALSE;
        gs.ignore[OBJ_SEQDESC] = FALSE;
        GatherEntity (sfp->input_entityID, (Pointer) sfp, ProcessSourceGatherFunc, &gs);
      }
    }
  }
  ArrowCursor ();
  Update ();
  if (indexerVersion && sfp->type == ADD_SOURCE) {
    sep = GetTopSeqEntryForEntityID (sfp->input_entityID);
    if (CountSeqEntryComponents (sep) == 1) {
      Message (MSG_OK, "When only one record present, edit the BioSource directly");
    }
  }
  ObjMgrSetDirtyFlag (sfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, sfp->input_entityID, 0, 0);
  Remove (sfp->form);
}


static void ChangeSourceGroup (GrouP g)

{
  SourceFormPtr  sfp;
  Int2           val;

  sfp = (SourceFormPtr) GetObjectExtra (g);
  if (sfp == NULL) return;
  val = GetValue (g);
  switch (val) {
    case 1 :
      SafeHide (sfp->genGrp);
      SafeHide (sfp->refGrp);
      SafeShow (sfp->modGrp);
      SafeShow (sfp->txtGrp);
      break;
    case 2 :
      SafeHide (sfp->modGrp);
      SafeHide (sfp->refGrp);
      SafeHide (sfp->txtGrp);
      SafeShow (sfp->genGrp);
      break;
    case 3 :
      SafeHide (sfp->modGrp);
      SafeHide (sfp->genGrp);
      SafeShow (sfp->txtGrp);
      SafeShow (sfp->refGrp);
      break;
    default :
      SafeHide (sfp->modGrp);
      SafeHide (sfp->genGrp);
      SafeHide (sfp->refGrp);
      SafeHide (sfp->txtGrp);
      break;
  }
  Update ();
}

static void SourceMessageProc (ForM f, Int2 mssg)

{
  SourceFormPtr  sfp;

  sfp = (SourceFormPtr) GetObjectExtra (f);
  if (sfp != NULL) {
    if (sfp->appmessage != NULL) {
      sfp->appmessage (f, mssg);
    }
  }
}

static void CleanupSourceForm (GraphiC g, VoidPtr data)

{
  SourceFormPtr  sfp;

  sfp = (SourceFormPtr) data;
  if (sfp != NULL) {
    MemFree (sfp->findStr);
    MemFree (sfp->replaceStr);
  }
  StdCleanupFormProc (g, data);
}

static PopuP PopupOrSingleList (GrouP prnt, Boolean macLike, PupActnProc actn, EnumFieldAssocPtr al, UIEnum val)

{
#ifdef WIN_MOTIF
  LisT  lst;

  lst = CreateEnumListDialog (prnt, 0, 16, (LstActnProc) actn, al, 0, NULL);
  SetEnumPopup ((PopuP) lst, al, val);
  return (PopuP) lst;
#else
  PopuP  pop;

  pop = CreateEnumPopupDialog (prnt, macLike, actn, al, 0, NULL);
  SetEnumPopup (pop, al, val);
  return pop;
#endif
}

extern Boolean HasPartsSet (SeqEntryPtr sep);

static void SourceApplyToPartsProc (ButtoN b)

{
  SourceFormPtr  sfp;

  sfp = (SourceFormPtr) GetObjectExtra (b);
  if (sfp == NULL) return;
  if (GetStatus (b)) {
    SafeEnable (sfp->onlyThisPart);
  } else {
    SafeDisable (sfp->onlyThisPart);
  }
}

static void ProcessSource (IteM i, Int2 type)

{
  ButtoN             b;
  BaseFormPtr        bfp;
  GrouP              c;
  GrouP              h;
  GrouP              q;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  SourceFormPtr      sfp;
  CharPtr            title;
  WindoW             w;
  GrouP              x;
  GrouP              z;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  sfp = (SourceFormPtr) MemNew (sizeof (SourceFormData));
  if (sfp == NULL) return;
  sfp->type = type;
  switch (type) {
    case REMOVE_SOURCE :
      title = "Source Removal";
      break;
    case CONVERT_SOURCE :
      title = "Source Conversion";
      break;
    case EDIT_SOURCE :
      title = "Edit Source";
      break;
    case ADD_SOURCE :
      title = "Add Source";
      break;
    default :
      title = "?";
      break;
  }
  w = FixedWindow (-50, -33, -10, -10, title, StdCloseWindowProc);
  SetObjectExtra (w, sfp, CleanupSourceForm);
  sfp->form = (ForM) w;
  sfp->formmessage = SourceMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    sfp->appmessage = sepp->handleMessages;
  }

  sfp->input_entityID = bfp->input_entityID;
  sfp->input_itemID = bfp->input_itemID;
  sfp->input_itemtype = bfp->input_itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  z = HiddenGroup (h, 1, 0, NULL);
  if (HasPartsSet (sep)) {
    sfp->applyToParts = CheckBox (z, "Apply to segmented parts, not segmented sequence", SourceApplyToPartsProc);
    SetObjectExtra (sfp->applyToParts, sfp, NULL);
    x = HiddenGroup (z, 2, 0, NULL);
    StaticPrompt (x, "Apply only to particular numbered segment", 0, dialogTextHeight, programFont, 'l');
    sfp->onlyThisPart = DialogText (x, "", 4, NULL);
    Disable (sfp->onlyThisPart);
  }

  sfp->sourceGroup = HiddenGroup (h, 4, 0, ChangeSourceGroup);
  SetObjectExtra (sfp->sourceGroup, sfp, NULL);
  RadioButton (sfp->sourceGroup, "Modifiers");
  RadioButton (sfp->sourceGroup, "Location");
  RadioButton (sfp->sourceGroup, "Strings");
  SetValue (sfp->sourceGroup, 1);

  q = HiddenGroup (h, 0, 0, NULL);

  sfp->modGrp = HiddenGroup (q, -4, 0, NULL);
  switch (type) {
    case REMOVE_SOURCE :
      StaticPrompt (sfp->modGrp, "Remove", 0, popupMenuHeight, programFont, 'l');
      sfp->frommod = PopupOrSingleList (sfp->modGrp, TRUE, NULL, subsource_and_orgmod_subtype_alistX, 0);
      break;
    case CONVERT_SOURCE :
      StaticPrompt (sfp->modGrp, "From", 0, popupMenuHeight, programFont, 'l');
      sfp->frommod = PopupOrSingleList (sfp->modGrp, TRUE, NULL, subsource_and_orgmod_subtype_alistX, 0);

      StaticPrompt (sfp->modGrp, "To", 0, popupMenuHeight, programFont, 'l');
      sfp->tomod = PopupOrSingleList (sfp->modGrp, TRUE, NULL, subsource_and_orgmod_subtype_alistX, 0);
      break;
    case EDIT_SOURCE :
      StaticPrompt (sfp->modGrp, "Type", 0, popupMenuHeight, programFont, 'l');
      sfp->frommod = PopupOrSingleList (sfp->modGrp, TRUE, NULL, subsource_and_orgmod_subtype_alistX, 0);
      break;
    case ADD_SOURCE :
      StaticPrompt (sfp->modGrp, "Type", 0, popupMenuHeight, programFont, 'l');
      sfp->frommod = PopupOrSingleList (sfp->modGrp, TRUE, NULL, subsource_and_orgmod_subtype_alistX, 0);
      break;
    default :
      break;
  }
  Hide (sfp->modGrp);

  sfp->genGrp = HiddenGroup (q, 4, 0, NULL);
  switch (type) {
    case REMOVE_SOURCE :
      StaticPrompt (sfp->genGrp, "Remove", 0, popupMenuHeight, programFont, 'l');
      sfp->fromgen = PopupList (sfp->genGrp, TRUE, NULL);
      InitEnumPopup (sfp->fromgen, biosource_genome_alistX, NULL);
      SetEnumPopup (sfp->fromgen, biosource_genome_alistX, 0);
      break;
    case CONVERT_SOURCE :
      StaticPrompt (sfp->genGrp, "From", 0, popupMenuHeight, programFont, 'l');
      sfp->fromgen = PopupList (sfp->genGrp, TRUE, NULL);
      InitEnumPopup (sfp->fromgen, biosource_genome_alistX, NULL);
      SetEnumPopup (sfp->fromgen, biosource_genome_alistX, 0);

      StaticPrompt (sfp->genGrp, "To", 0, popupMenuHeight, programFont, 'l');
      sfp->togen = PopupList (sfp->genGrp, TRUE, NULL);
      InitEnumPopup (sfp->togen, biosource_genome_alistX, NULL);
      SetEnumPopup (sfp->togen, biosource_genome_alistX, 0);
      break;
    case EDIT_SOURCE :
      StaticPrompt (sfp->genGrp, "Type", 0, popupMenuHeight, programFont, 'l');
      sfp->fromgen = PopupList (sfp->genGrp, TRUE, NULL);
      InitEnumPopup (sfp->fromgen, biosource_genome_alistX, NULL);
      SetEnumPopup (sfp->fromgen, biosource_genome_alistX, 0);
      break;
    case ADD_SOURCE :
      StaticPrompt (sfp->genGrp, "Type", 0, popupMenuHeight, programFont, 'l');
      sfp->fromgen = PopupList (sfp->genGrp, TRUE, NULL);
      InitEnumPopup (sfp->fromgen, biosource_genome_alistX, NULL);
      SetEnumPopup (sfp->fromgen, biosource_genome_alistX, 0);
      break;
    default :
      break;
  }
  Hide (sfp->genGrp);

  sfp->refGrp = HiddenGroup (q, 4, 0, NULL);
  switch (type) {
    case REMOVE_SOURCE :
      StaticPrompt (sfp->refGrp, "Remove", 0, popupMenuHeight, programFont, 'l');
      sfp->fromref = PopupList (sfp->refGrp, TRUE, NULL);
      InitEnumPopup (sfp->fromref, orgref_textfield_alist, NULL);
      SetEnumPopup (sfp->fromref, orgref_textfield_alist, 0);
      break;
    case CONVERT_SOURCE :
      StaticPrompt (sfp->refGrp, "From", 0, popupMenuHeight, programFont, 'l');
      sfp->fromref = PopupList (sfp->refGrp, TRUE, NULL);
      InitEnumPopup (sfp->fromref, orgref_textfield_alist, NULL);
      SetEnumPopup (sfp->fromref, orgref_textfield_alist, 0);

      StaticPrompt (sfp->refGrp, "To", 0, popupMenuHeight, programFont, 'l');
      sfp->toref = PopupList (sfp->refGrp, TRUE, NULL);
      InitEnumPopup (sfp->toref, orgref_textfield_alist, NULL);
      SetEnumPopup (sfp->toref, orgref_textfield_alist, 0);
      break;
    case EDIT_SOURCE :
      StaticPrompt (sfp->refGrp, "Type", 0, popupMenuHeight, programFont, 'l');
      sfp->fromref = PopupList (sfp->refGrp, TRUE, NULL);
      InitEnumPopup (sfp->fromref, orgref_textfield_alist, NULL);
      SetEnumPopup (sfp->fromref, orgref_textfield_alist, 0);
      break;
    case ADD_SOURCE :
      break;
    default :
      break;
  }
  Hide (sfp->refGrp);

  Show (sfp->modGrp);

  sfp->txtGrp = NULL;
  switch (type) {
    case REMOVE_SOURCE :
    case CONVERT_SOURCE :
      sfp->txtGrp = HiddenGroup (h, 0, 2, NULL);
      StaticPrompt (sfp->txtGrp, "Optional string constraint", 0, dialogTextHeight, programFont, 'c');
      sfp->findthis = DialogText (sfp->txtGrp, "", 14, NULL);
      break;
    case ADD_SOURCE :
      sfp->txtGrp = HiddenGroup (h, 2, 0, NULL);
      StaticPrompt (sfp->txtGrp, "Text", 0, dialogTextHeight, programFont, 'c');
      sfp->findthis = DialogText (sfp->txtGrp, "", 14, NULL);
      break;
    case EDIT_SOURCE :
      sfp->txtGrp = HiddenGroup (h, 2, 0, NULL);
      StaticPrompt (sfp->txtGrp, "Find", 0, dialogTextHeight, programFont, 'l');
      sfp->findthis = DialogText (sfp->txtGrp, "", 14, NULL);
      StaticPrompt (sfp->txtGrp, "Replace", 0, dialogTextHeight, programFont, 'l');
      sfp->replacewith = DialogText (sfp->txtGrp, "", 14, NULL);
      break;
    default :
      break;
  }

  c = HiddenGroup (h, 4, 0, NULL);
  b = PushButton (c, "Accept", DoProcessSource);
  SetObjectExtra (b, sfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) c, (HANDLE) sfp->sourceGroup,
                (HANDLE) sfp->modGrp, (HANDLE) sfp->genGrp,
                (HANDLE) sfp->refGrp, (HANDLE) sfp->txtGrp, NULL);
  RealizeWindow (w);
  Show (w);
  Select (sfp->findthis);
  Update ();
}

static void RemoveSource (IteM i)

{
  ProcessSource (i, REMOVE_SOURCE);
}

static void ConvertSource (IteM i)

{
  ProcessSource (i, CONVERT_SOURCE);
}

static void EditSource (IteM i)

{
  ProcessSource (i, EDIT_SOURCE);
}

static void AddSource (IteM i)

{
  ProcessSource (i, ADD_SOURCE);
}

static Boolean RemoveTaxonProc (GatherObjectPtr gop)

{
  BioSourcePtr     biop = NULL;
  DbtagPtr         dbt;
  ValNodePtr       next;
  OrgRefPtr        orp;
  ObjValNodePtr    ovn;
  ValNodePtr PNTR  prev;
  SeqDescPtr       sdp;
  SeqFeatPtr       sfp;
  ValNodePtr       vnp;

  switch (gop->itemtype) {
    case OBJ_SEQFEAT :
      sfp = (SeqFeatPtr) gop->dataptr;
      if (sfp != NULL && sfp->idx.subtype == FEATDEF_BIOSRC) {
        biop = (BioSourcePtr) sfp->data.value.ptrvalue;
      }
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
  if (biop == NULL) return TRUE;
  orp = biop->org;
  if (orp == NULL) return TRUE;
  vnp = orp->db;
  if (vnp == NULL) return TRUE;
  prev = (ValNodePtr PNTR) &(orp->db);
  while (vnp != NULL) {
    next = vnp->next;
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    if (dbt != NULL && StringICmp ((CharPtr) dbt->db, "taxon") == 0) {
      *prev = vnp->next;
      vnp->next = NULL;
      DbtagFree (dbt);
      ValNodeFree (vnp);
    } else {
      prev = (ValNodePtr PNTR) &(vnp->next);
    }
    vnp = next;
  }
  return TRUE;
}

static void RemoveTaxonXrefs (IteM i)

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
                         RemoveTaxonProc, NULL, objMgrFilter);
}

/*=========================================================================*/
/*                                                                         */
/* SuppressGeneXref_Callback () -- Suppresses any gene xref on the feature */
/*                                 that is passed to it.                   */
/*                                                                         */
/*=========================================================================*/

static Boolean LIBCALLBACK SuppressGeneXref_Callback (SeqFeatPtr sfp,
					       SeqMgrFeatContextPtr context)
{
  GeneRefPtr      grp = NULL;
  SeqFeatPtr      newSfp = NULL;
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

/*=========================================================================*/
/*                                                                         */
/* SuppressGeneXrefsForFeatureType () -- Suppresses gene xrefs for all     */
/*                                       features of the given type.       */
/*                                                                         */
/*=========================================================================*/

static void SuppressGeneXrefsForFeatureType (BioseqPtr bsp,
					     Int2      seqfeat,
					     Int2      featdef)
{
  Boolean featDefFilterArray [FEATDEF_MAX];
  Boolean seqFeatFilterArray [SEQFEAT_MAX];
  Uint2   entityID;

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

  /* Force an update and redraw */

  entityID = ObjMgrGetEntityIDForPointer (bsp);
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

static void SuppressGenesOnAllRBS (IteM i)

{
  BaseFormPtr  bfp;
  BioseqPtr    bsp;

  /* Get the Bioseq */

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  bsp =  GetBioseqGivenIDs (bfp->input_entityID,
			   bfp->input_itemID,
			   bfp->input_itemtype);

  /* Suppress the gene xrefs for all RBS features */

  SuppressGeneXrefsForFeatureType (bsp, 0, FEATDEF_RBS);

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

static ENUM_ALIST(molinfo_biomol_alist)
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
  {"Other",                255},
END_ENUM_ALIST

static ENUM_ALIST(molinfo_tech_alist)
  {" ",                  0},
  {"Standard",           1},
  {"EST",                2},
  {"STS",                3},
  {"Survey",             4},
  {"Genetic Map",        5},
  {"Physical Map",       6},
  {"Derived",            7},
  {"Concept-Trans",      8},
  {"Seq-Pept",           9},
  {"Both",              10},
  {"Seq-Pept-Overlap",  11},
  {"Seq-Pept-Homol",    12},
  {"Concept-Trans-A",   13},
  {"HTGS 0",            18},
  {"HTGS 1",            14},
  {"HTGS 2",            15},
  {"HTGS 3",            16},
  {"FLI_cDNA",          17},
  {"HTC",               19},
  {"Other:",            255},
END_ENUM_ALIST

static ENUM_ALIST(molinfo_complete_alist)
  {" ",         0},
  {"Complete",  1},
  {"Partial",   2},
  {"No Left",   3},
  {"No Right",  4},
  {"No Ends",   5},
  {"Other",   255},
END_ENUM_ALIST

static ENUM_ALIST(mol_alist)
{" ",               0},
{"DNA",             Seq_mol_dna},
{"RNA",             Seq_mol_rna},
{"Protein",         Seq_mol_aa},
{"Nucleotide",      Seq_mol_na},
{"Other",           Seq_mol_other},
END_ENUM_ALIST

static ENUM_ALIST(topology_alist)
{" ",               0},
{"Linear",          TOPOLOGY_LINEAR},
{"Circular",        TOPOLOGY_CIRCULAR},
{"Tandem",          TOPOLOGY_TANDEM},
{"Other",           255},
END_ENUM_ALIST

static ENUM_ALIST(strand_alist)
{" ",               Seq_strand_unknown},
{"Single",          Seq_strand_plus},
{"Double",          Seq_strand_minus},
{"Mixed",           Seq_strand_both},
{"Mixed Rev",       Seq_strand_both_rev},
{"Other",           Seq_strand_other},
END_ENUM_ALIST

typedef struct molinfoblock {
  PopuP           moltype;
  PopuP           technique;
  PopuP           complete;
  PopuP           molPopup;
  PopuP           topologyPopup;
  PopuP           strandPopup;
} MolInfoBlock, PNTR MolInfoBlockPtr;

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

static void CreateMolInfoBlock (MolInfoBlockPtr mibp, GrouP h)

{
  GrouP  g;
  Int2   wid;

  if (mibp == NULL || h == NULL) return;
  SelectFont (programFont);
  wid = MaxStringWidths (labels);
  SelectFont (systemFont);

  g = HiddenGroup (h, -2, 0, NULL);

  StaticPrompt (g, "Molecule", wid, popupMenuHeight, programFont, 'l');
  mibp->moltype = PopupList (g, TRUE, NULL);
  InitEnumPopup (mibp->moltype, molinfo_biomol_alist, NULL);
  SetEnumPopup (mibp->moltype, molinfo_biomol_alist, 0);

  StaticPrompt (g, "Technique", wid, popupMenuHeight, programFont, 'l');
  mibp->technique = PopupList (g, TRUE, NULL);
  InitEnumPopup (mibp->technique, molinfo_tech_alist, NULL);
  SetEnumPopup (mibp->technique, molinfo_tech_alist, 0);

  StaticPrompt (g, "Completedness", wid, popupMenuHeight, programFont, 'l');
  mibp->complete = PopupList (g, TRUE, NULL);
  InitEnumPopup (mibp->complete, molinfo_complete_alist, NULL);
  SetEnumPopup (mibp->complete, molinfo_complete_alist, 0);

  StaticPrompt (g, "Class", wid, popupMenuHeight, programFont, 'l');
  mibp->molPopup = PopupList (g, TRUE, NULL);
  InitEnumPopup (mibp->molPopup, mol_alist, NULL);

  StaticPrompt (g, "Topology", wid, popupMenuHeight, programFont, 'l');
  mibp->topologyPopup = PopupList (g, TRUE, NULL);
  InitEnumPopup (mibp->topologyPopup, topology_alist, NULL);

  StaticPrompt (g, "Strand", wid, popupMenuHeight, programFont, 'l');
  mibp->strandPopup = PopupList (g, TRUE, NULL);
  InitEnumPopup (mibp->strandPopup, strand_alist, NULL);
}

static void EditMolInfoCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr       bsp;
  BioseqSetPtr    bssp;
  MolInfoEditPtr  miep;
  MolInfoPtr      mip;
  ValNodePtr      sdp;
  UIEnum          val;

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
      if (GetEnumPopup (miep->from.molPopup, mol_alist, &val) &&
          bsp->mol == (Uint1) val) {
        if (GetEnumPopup (miep->to.molPopup, mol_alist, &val)) {
          bsp->mol = (Uint1) val;
        }
      }
      if (GetEnumPopup (miep->from.strandPopup, strand_alist, &val) &&
          bsp->strand == (Uint1) val) {
        if (GetEnumPopup (miep->to.strandPopup, strand_alist, &val)) {
          bsp->strand = (Uint1) val;
        }
      }
      if (GetEnumPopup (miep->from.topologyPopup, topology_alist, &val) &&
          bsp->topology == (Uint1) val) {
        if (GetEnumPopup (miep->to.topologyPopup, topology_alist, &val)) {
          bsp->topology = (Uint1) val;
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
  } else return;
  while (sdp != NULL) {
    if (sdp->choice == Seq_descr_molinfo && sdp->data.ptrvalue != NULL) {
      mip = (MolInfoPtr) sdp->data.ptrvalue;
      if (GetEnumPopup (miep->from.moltype, molinfo_biomol_alist, &val) &&
          mip->biomol == (Uint1) val) {
        if (GetEnumPopup (miep->to.moltype, molinfo_biomol_alist, &val)) {
          mip->biomol = (Uint1) val;
        }
      }
      if (GetEnumPopup (miep->from.technique, molinfo_tech_alist, &val) &&
          mip->tech == (Uint1) val) {
        if (GetEnumPopup (miep->to.technique, molinfo_tech_alist, &val)) {
          mip->tech = (Uint1) val;
        }
      }
      if (GetEnumPopup (miep->from.complete, molinfo_complete_alist, &val) &&
          mip->completeness == (Uint1) val) {
        if (GetEnumPopup (miep->to.complete, molinfo_complete_alist, &val)) {
          mip->completeness = (Uint1) val;
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
  SetValue (miep->constraint, 1);

  g = HiddenGroup (w, 2, 0, NULL);
  h = HiddenGroup (g, 0, 2, NULL);
  StaticPrompt (h, "From", 0, 0, programFont, 'c');
  CreateMolInfoBlock (&miep->from, h);
  h = HiddenGroup (g, 0, 2, NULL);
  StaticPrompt (h, "To", 0, 0, programFont, 'c');
  CreateMolInfoBlock (&miep->to, h);

  c = HiddenGroup (w, 2, 0, NULL);
  b = PushButton (c, "Accept", DoProcessEditMolInfo);
  SetObjectExtra (b, miep, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) k, (HANDLE) g, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}

static void DoApplyMolInfo (SeqEntryPtr sep, MolInfoEditPtr miep)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  MolInfoPtr    mip;
  UIEnum        val;
  ValNodePtr    vnp;

  if (sep == NULL || sep->data.ptrvalue == NULL || miep == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
/* this also delves into nuc-prot sets */
    if (bssp != NULL && (bssp->_class == 7 ||
                         (bssp->_class >= 13 && bssp->_class <= 16) ||
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
      if (GetEnumPopup (miep->to.molPopup, mol_alist, &val) && val > 0) {
        bsp->mol = (Uint1) val;
      }
      if (GetEnumPopup (miep->to.strandPopup, strand_alist, &val) && val > 0) {
        bsp->strand = (Uint1) val;
      }
      if (GetEnumPopup (miep->to.topologyPopup, topology_alist, &val) && val > 0) {
        bsp->topology = (Uint1) val;
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
    if (GetEnumPopup (miep->to.moltype, molinfo_biomol_alist, &val)) {
      mip->biomol = (Uint1) val;
    }
    if (GetEnumPopup (miep->to.technique, molinfo_tech_alist, &val)) {
      mip->tech = (Uint1) val;
    }
    if (GetEnumPopup (miep->to.complete, molinfo_complete_alist, &val)) {
      mip->completeness = (Uint1) val;
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
  CreateMolInfoBlock (&miep->to, h);

  c = HiddenGroup (w, 2, 0, NULL);
  b = PushButton (c, "Accept", DoProcessApplyMolInfo);
  SetObjectExtra (b, miep, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) k, (HANDLE) g, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}

static void GenePseudoOn (SeqFeatPtr sfp, Pointer userdata)

{
  GeneRefPtr  grp;

  if (sfp->data.choice != SEQFEAT_GENE) return;
  grp = (GeneRefPtr) sfp->data.value.ptrvalue;
  if (grp == NULL) return;
  grp->pseudo = TRUE;
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

static void GenePseudoOff (SeqFeatPtr sfp, Pointer userdata)

{
  GeneRefPtr  grp;

  if (sfp->data.choice != SEQFEAT_GENE) return;
  grp = (GeneRefPtr) sfp->data.value.ptrvalue;
  if (grp == NULL) return;
  grp->pseudo = FALSE;
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
    b = PushButton (c, "Accept", AcceptBioseqSetEditProc);
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
#ifdef WIN_MOTIF
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
    bfp = GetObjectExtra (i);
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
  okay = SeqEntryToFlat (sep, fp, GENBANK_FMT, SEQUIN_MODE);
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
/*#endif*/

/*#ifdef INTERNAL_NCBI_SEQUIN*/
/*#ifdef NEW_TAXON_SERVICE*/
/*
*  Process output in UNIX with
*    'sort -f +0 sorttax | uniq > taxlist.txt'
*  and
*    'sort -n +0 sortlin | uniq > lineages.txt'
*/

static void WriteTaxNode (CharPtr sci, CharPtr com, CharPtr syn,
                          Int4 gc, Int4 mgc, CharPtr div,
                          Int4 taxID, FILE *fout)

{
  Char  str [256];

  if (fout == NULL) return;
  if (sci != NULL && sci [0] != '\0') {
    if (div == NULL || div [0] == '\0') div = "???";
    if (com != NULL && com [0] != '\0') {
      sprintf (str, "%s\t%s\t%ld\t%ld\t%s\t%ld\n", sci, com,
               (long) gc, (long) mgc, div, (long) taxID);
      fprintf (fout, "%s", str);
      /*
      sprintf (str, "%s\tC\t%s\t%ld\t%ld\t%s\t%ld\n", com, sci,
               (long) gc, (long) mgc, div, (long) taxID);
      fprintf (fout, "%s", str);
      */
    /*
    } else if (syn != NULL && syn [0] != '\0') {
      sprintf (str, "%s\tC\t%s\t%ld\t%ld\t%s\t%ld\n", syn, sci,
               (long) gc, (long) mgc, div, (long) taxID);
      fprintf (fout, "%s", str);
    */
    } else {
      sprintf (str, "%s\t\t%ld\t%ld\t%s\t%ld\n", sci,
               (long) gc, (long) mgc, div, (long) taxID);
      fprintf (fout, "%s", str);
    }
  }
}

static void WriteLineage (Int4 taxID, CharPtr lineage, FILE *fout)

{
  if (fout == NULL || lineage == NULL) return;
  fprintf (fout, "%ld\t%s\n", (long) taxID, lineage);
}

static void ProcessTaxNode (CharPtr orgname, FILE *fout1, FILE *fout2)

{
  Int4           gc;
  Int4           mgc;
  OrgNamePtr     onp;
  OrgRefPtr      orp;
  Int4           taxID;
  Taxon1DataPtr  tdp;
  /*
  CharPtr        syn;
  ValNodePtr     vnp;
  */

  if (orgname == NULL || orgname [0] == '\0' || fout1 == NULL || fout2 == NULL) return;
  taxID = tax1_getTaxIdByName (orgname);
  if (taxID < 1) return;
  tdp = tax1_getbyid (taxID);
  if (tdp == NULL) return;
  orp = tdp->org;
  if (orp != NULL) {
    gc = 0;
    mgc = 0;
    onp = orp->orgname;
    if (onp != NULL) {
      gc = onp->gcode;
      mgc = onp->mgcode;
      WriteLineage (taxID, onp->lineage, fout2);
    }
    WriteTaxNode (orp->taxname, orp->common, NULL, gc,
                  mgc, tdp->div, taxID, fout1);
    /*
    vnp = orp->syn;
    while (vnp != NULL) {
      syn = (CharPtr) vnp->data.ptrvalue;
      if (vnp->choice == 0 && syn != NULL && syn [0] != '\0') {
        WriteTaxNode (orp->taxname, NULL, syn, gc,
                      mgc, tdp->div, taxID, fout1);
      }
      vnp = vnp->next;
    }
    */
  }
  Taxon1DataFree (tdp);
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

  Message (MSG_POST, "tax1_init");
  if (tax1_init ()) {
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
          tax1_setSynonyms (TRUE);
          while (goOn) {
            goOn = (fgets (orgname, sizeof (orgname), fin) != NULL);
            if (goOn) {
              ptr = orgname;
              ch = *ptr;
              while (ch != '\n' && ch != '\r' && ch != '\0') {
                ptr++;
                ch = *ptr;
              }
              *ptr = '\0';
              Message (MSG_POST, "Organism '%s'", orgname);
              ProcessTaxNode (orgname, fout1, fout2);
            }
          }
          FileClose (fout2);
        }
        FileClose (fout1);
      }
      FileClose (fin);
    } else {
      Message (MSG_OK, "Could not find orglist file");
    }
    Message (MSG_POST, "tax1_fini");
    tax1_fini ();
  } else {
    Message (MSG_OK, "tax1_init failed");
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
  b = PushButton (c, "Accept", DoProcessPub);
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
                         (bssp->_class >= 13 && bssp->_class <= 16))) {
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


typedef struct edseqendsdata {
  FEATURE_FORM_BLOCK

  TexT           seq;
  TexT           genename;
  GrouP          whichend;
  ButtoN         extendfeat;
  CharPtr        seqstr;
  CharPtr        genestr;
  Int2           endval;
  Int2           frameshift;
  Boolean        adjustframe;
  Boolean        extendflag;
  BioseqPtr      extendedthis;
  SeqEntryPtr    sep;
  Boolean        rsult;
} EditSeqEnds, PNTR EditSeqPtr;

static Boolean GeneFindByNameFunc (GatherContextPtr gcp)

{
  EditSeqPtr  esp;
  GeneRefPtr  grp;
  SeqFeatPtr  sfp;

  if (gcp == NULL) return TRUE;
  esp = (EditSeqPtr) gcp->userdata;
  if (esp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL) return TRUE;
  if (sfp->data.choice != SEQFEAT_GENE) return TRUE;
  grp = (GeneRefPtr) sfp->data.value.ptrvalue;
  if (grp == NULL) return TRUE;
  if (StringICmp (grp->locus, esp->genestr) == 0) {
    esp->rsult = TRUE;
  }
  return TRUE;
}

static Boolean EditSeqEntryHasGene (BioseqPtr bsp, SeqEntryPtr sep, EditSeqPtr esp)

{
  GatherScope  gs;

  if (esp->input_entityID == 0 || esp->sep == NULL) return FALSE;
  if (StringHasNoText (esp->genestr)) return TRUE;
  esp->rsult = FALSE;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = TRUE;
  gs.scope = sep;
  MemSet ((Pointer)(gs.ignore), (int) (TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
  gs.ignore [OBJ_BIOSEQ] = FALSE;
  gs.ignore [OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore [OBJ_SEQFEAT] = FALSE;
  gs.ignore [OBJ_SEQANNOT] = FALSE;
  GatherEntity (esp->input_entityID, (Pointer) esp, GeneFindByNameFunc, &gs);
  gs.target = SeqLocFree (gs.target);
  return esp->rsult;
}

typedef struct findprot {
  SeqLocPtr   slp;
  Int4        min;
  SeqFeatPtr  bestprot;
} FindProtData, PNTR FindProtPtr;

static Boolean ProtFindFunc (GatherContextPtr gcp)

{
  Int4         diff;
  FindProtPtr  fpp;
  SeqFeatPtr   sfp;

  if (gcp == NULL) return TRUE;

  fpp = (FindProtPtr) gcp->userdata;
  if (fpp == NULL) return TRUE;

  if (gcp->thistype == OBJ_SEQFEAT) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp != NULL && sfp->data.choice == SEQFEAT_PROT && sfp->data.value.ptrvalue != NULL) {
      diff = SeqLocAinB (sfp->location, fpp->slp);
      if (diff >= 0) {
        if (diff < fpp->min) {
          fpp->min = diff;
          fpp->bestprot = sfp;
        }
      }
    }
  }

  return TRUE;
}

extern SeqFeatPtr FindBestProtein (Uint2 entityID, SeqLocPtr product)

{
  FindProtData  fpd;
  GatherScope   gs;

  if (entityID == 0 || product == NULL) return NULL;
  fpd.bestprot = NULL;
  fpd.min = INT4_MAX;
  fpd.slp = product;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = TRUE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherEntity (entityID, (Pointer) &fpd, ProtFindFunc, &gs);
  return fpd.bestprot;
}

static Boolean FixACDSFunc (GatherContextPtr gcp)

{
  SeqFeatPtr    bestprot;
  ByteStorePtr  bs;
  BioseqPtr     bsp;
  Char          ch;
  CdRegionPtr   crp;
  EditSeqPtr    esp;
  Int2          frame;
  Boolean       partial5;
  Boolean       partial3;
  CharPtr       prot;
  CharPtr       ptr;
  SeqEntryPtr   sep;
  SeqFeatPtr    sfp;
  SeqIdPtr      sip;
  SeqLocPtr     slp;

  if (gcp == NULL) return TRUE;
  esp = (EditSeqPtr) gcp->userdata;
  if (esp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL) return TRUE;
  if (sfp->data.choice != SEQFEAT_CDREGION) return TRUE;
  slp = SeqLocFindNext (sfp->location, NULL);
  if (slp == NULL) return TRUE;
  CheckSeqLocForPartial (slp, &partial5, &partial3);
  sip = SeqLocId (slp);
  if (sip == NULL) return TRUE;
  bsp = BioseqFind (sip);
  if (bsp == NULL || bsp != esp->extendedthis) return TRUE;
  if (esp->adjustframe) {
    if (GetOffsetInBioseq (slp, bsp, SEQLOC_START) != 0) return TRUE;
    crp = (CdRegionPtr) sfp->data.value.ptrvalue;
    if (crp == NULL) return TRUE;
    frame = crp->frame;
    if (frame == 0) {
      frame = 1;
    }
    frame--;
    frame += esp->frameshift;
    crp->frame = (frame % 3) + 1;
  } else {
    if (GetOffsetInBioseq (slp, bsp, SEQLOC_STOP) != bsp->length - 1) return TRUE;
  }
  sip = SeqLocId (sfp->product);
  if (sip == NULL) return TRUE;
  bsp = BioseqFind (sip);
  if (bsp == NULL) return TRUE;
  if (bsp->repr != Seq_repr_raw) return TRUE;
  if (bsp->mol != Seq_mol_aa) return TRUE;
  bestprot = FindBestProtein (esp->input_entityID, sfp->product);
  bs = ProteinFromCdRegionEx (sfp, FALSE, FALSE);
  if (bs == NULL) return TRUE;
  prot = BSMerge (bs, NULL);
  bs = BSFree (bs);
  if (prot == NULL) return TRUE;
  ptr = prot;
  ch = *ptr;
  while (ch != '\0') {
    *ptr = TO_UPPER (ch);
    ptr++;
    ch = *ptr;
  }
  bs = BSNew (1000);
  if (bs != NULL) {
    ptr = prot;
    /*
    if (prot [0] == '-') {
       ptr++;
    }
    */
    BSWrite (bs, (VoidPtr) ptr, (Int4) StringLen (ptr));
    bsp->seq_data = BSFree (bsp->seq_data);
    bsp->seq_data = bs;
    bsp->length = BSLen (bs);
    bsp->seq_data_type = Seq_code_ncbieaa;
  }
  if (bestprot == NULL) return TRUE;
  sep = SeqMgrGetSeqEntryForData (bsp);
  bestprot->location = SeqLocFree (bestprot->location);
  bestprot->location = CreateWholeInterval (sep);
  SetSeqLocPartial (bestprot->location, partial5, partial3);
  return TRUE;
}

static void FixAndRetranslateCDSs (BioseqPtr bsp, SeqEntryPtr sep,
                                   EditSeqPtr esp, Boolean adjustframe)

{
  GatherScope  gs;

  if (esp->input_entityID == 0 || esp->sep == NULL) return;
  esp->adjustframe = adjustframe;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  gs.scope = sep;
  MemSet ((Pointer)(gs.ignore), (int) (TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
  gs.ignore [OBJ_BIOSEQ] = FALSE;
  gs.ignore [OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore [OBJ_SEQFEAT] = FALSE;
  gs.ignore [OBJ_SEQANNOT] = FALSE;
  GatherEntity (esp->input_entityID, (Pointer) esp, FixACDSFunc, &gs);
  gs.target = SeqLocFree (gs.target);
}

static ValNodePtr CollectAndExtendSingleBaseFeatures (BioseqPtr bsp, Int2 whichend, Int4 len)

{
  SeqMgrFeatContext  context;
  ValNodePtr         head = NULL;
  SeqFeatPtr         sfp;
  SeqLocPtr          slp;
  SeqPntPtr          spp;

  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &context);
  while (sfp != NULL) {
    if (whichend == 1 && context.numivals == 1 && context.right == 0) {
      slp = sfp->location;
      if (slp != NULL && slp->choice == SEQLOC_PNT && slp->next == NULL) {
        spp = (SeqPntPtr) slp->data.ptrvalue;
        if (spp != NULL && spp->point == 0) {
          spp->point = 1;
          ValNodeAddPointer (&head, 1, (Pointer) sfp);
        }
      }
    } else if (whichend == 2 && context.numivals == 1 && context.left == bsp->length - 1) {
      slp = sfp->location;
      if (slp != NULL && slp->choice == SEQLOC_PNT && slp->next == NULL) {
        spp = (SeqPntPtr) slp->data.ptrvalue;
        if (spp != NULL && spp->point == bsp->length - 1) {
          spp->point = bsp->length - 2;
          ValNodeAddPointer (&head, 2, (Pointer) sfp);
        }
      }
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &context);
  }

  return head;
}

static void ReadjustSingleBaseFeatures (ValNodePtr head, BioseqPtr bsp, Int2 whichend, Int4 len)

{
  SeqFeatPtr  sfp;
  SeqIntPtr   sintp;
  SeqLocPtr   slp;
  SeqPntPtr   spp;
  ValNodePtr  vnp;

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    sfp = (SeqFeatPtr) vnp->data.ptrvalue;
    if (sfp != NULL) {
      slp = sfp->location;
      if (slp != NULL && slp->choice == SEQLOC_PNT && slp->next == NULL) {
        spp = (SeqPntPtr) slp->data.ptrvalue;
        if (spp != NULL) {
          if (whichend == 1) {
            sintp = SeqIntNew ();
            if (sintp != NULL) {
              sintp->from = 0;
              sintp->to = spp->point - 1;
              sintp->strand = spp->strand;
              sintp->id = spp->id;
              spp->id = NULL;
              sintp->if_from = spp->fuzz;
              spp->fuzz = NULL;
              slp->choice = SEQLOC_INT;
              slp->data.ptrvalue = (Pointer) sintp;
              SeqPntFree (spp);
            }
          } else if (whichend == 2) {
            sintp = SeqIntNew ();
            if (sintp != NULL) {
              sintp->from = spp->point + 1;
              sintp->to = spp->point + 1 + len;
              sintp->strand = spp->strand;
              sintp->id = spp->id;
              spp->id = NULL;
              sintp->if_to = spp->fuzz;
              spp->fuzz = NULL;
              slp->choice = SEQLOC_INT;
              slp->data.ptrvalue = (Pointer) sintp;
              SeqPntFree (spp);
            }
          }
        }
      }
    }
  }
}

static void EditSeqEndsCallback (Uint2 entityID, EditSeqPtr esp, SeqEntryPtr sep)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  ValNodePtr    head;
  Int4          len;
  SeqEntryPtr   nsep;
  Int4          pos;
  Uint1         residue;
  SeqPortPtr    spp;
  CharPtr       str;
  Char          terminal [2];

  if (sep == NULL || esp == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (bssp->_class >= 13 && bssp->_class <= 16))) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        EditSeqEndsCallback (entityID, esp, sep);
      }
      return;
    }
  }
  nsep = FindNucSeqEntry (sep);
  if (nsep == NULL || nsep->data.ptrvalue == NULL) return;
  if (! IS_Bioseq (nsep)) return;
  bsp = (BioseqPtr) nsep->data.ptrvalue;
  if (! ISA_na (bsp->mol)) return;
  if (! EditSeqEntryHasGene (bsp, sep, esp)) return;
  pos = 0;
  if (esp->endval == 2) {
    pos = bsp->length;
  }
  if (esp->extendflag) {
    esp->frameshift = 0;
    terminal [0] = '\0';
    terminal [1] = '\0';
    residue = 0;
    if (esp->endval == 2) {
      spp = SeqPortNew (bsp, bsp->length - 1, -1, 0, Seq_code_iupacna);
    } else {
      spp = SeqPortNew (bsp, 0, -1, 0, Seq_code_iupacna);
    }
    if (spp != NULL) {
      residue = SeqPortGetResidue (spp);
      if (IS_residue (residue)) {
        terminal [0] = TO_LOWER ((Char) residue);
      }
    }
    SeqPortFree (spp);
    str = MemNew ((size_t) (StringLen (esp->seqstr) + 4));
    if (str != NULL) {
      head = NULL;
      if (esp->endval == 2) {
        esp->extendedthis = bsp;
        StringCpy (str, terminal);
        StringCat (str, esp->seqstr);
        len = StringLen (esp->seqstr);
        pos = bsp->length - 1;
        head = CollectAndExtendSingleBaseFeatures (bsp, 2, len);
        insertchar (str, pos, bsp->id, bsp->mol, FALSE);
        BioseqDelete (bsp->id, bsp->length - 1, bsp->length - 1, TRUE, FALSE);
        ReadjustSingleBaseFeatures (head, bsp, 2, len);
        FixAndRetranslateCDSs (bsp, sep, esp, FALSE);
      } else {
        esp->frameshift = (Int2) StringLen (esp->seqstr);
        esp->extendedthis = bsp;
        StringCpy (str, esp->seqstr);
        StringCat (str, terminal);
        len = StringLen (esp->seqstr);
        pos = 1;
        head = CollectAndExtendSingleBaseFeatures (bsp, 1, len);
        insertchar (str, pos, bsp->id, bsp->mol, FALSE);
        BioseqDelete (bsp->id, 0, 0, TRUE, FALSE);
        ReadjustSingleBaseFeatures (head, bsp, 1, len);
        FixAndRetranslateCDSs (bsp, sep, esp, TRUE);
      }
      ValNodeFree (head);
    }
    MemFree (str);
  } else {
    insertchar (esp->seqstr, pos, bsp->id, bsp->mol, FALSE);
  }
}

static void DoEditSeqEndsProc (ButtoN b)

{
  Char        ch;
  EditSeqPtr  esp;
  CharPtr     p, q;

  esp = (EditSeqPtr) GetObjectExtra (b);
  if (esp == NULL) {
    Remove (ParentWindow (b));
    return;
  }
  Hide (esp->form);
  Update ();
  esp->seqstr = SaveStringFromText  (esp->seq);
  p = esp->seqstr;
  if (p != NULL) {
    /* remove any non-sequence characters */
    q = p;
    ch = *p;
    while (ch != '\0') {
      if (IS_ALPHA (ch)) {
        *q = ch;
        q++;
      }
      p++;
      ch = *p;
    }
    *q = '\0';
  }
  esp->genestr = SaveStringFromText  (esp->genename);
  esp->endval = GetValue (esp->whichend);
  esp->extendflag = GetStatus (esp->extendfeat);
  EditSeqEndsCallback (esp->input_entityID, esp, esp->sep);
  MemFree (esp->seqstr);
  MemFree (esp->genestr);
  ObjMgrSetDirtyFlag (esp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, esp->input_entityID, 0, 0);
  Remove (esp->form);
}

static void EditSeqMessageProc (ForM f, Int2 mssg)

{
  EditSeqPtr  esp;

  esp = (EditSeqPtr) GetObjectExtra (f);
  if (esp != NULL) {
    if (esp->appmessage != NULL) {
      esp->appmessage (f, mssg);
    }
  }
}

static void EditSeqEndsProc (IteM i)

{
  ButtoN             b;
  BaseFormPtr        bfp;
  GrouP              c;
  EditSeqPtr         esp;
  GrouP              g;
  GrouP              h;
  GrouP              k;
  GrouP              q;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  WindoW             w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  esp = (EditSeqPtr) MemNew (sizeof (EditSeqEnds));
  if (esp == NULL) return;

  w = FixedWindow (-50, -33, -10, -10, "Edit Sequence Ends", StdCloseWindowProc);
  SetObjectExtra (w, esp, StdCleanupFormProc);
  esp->form = (ForM) w;
  esp->formmessage = EditSeqMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    esp->appmessage = sepp->handleMessages;
  }

  esp->input_entityID = bfp->input_entityID;
  esp->input_itemID = bfp->input_itemID;
  esp->input_itemtype = bfp->input_itemtype;

  esp->sep = sep;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (g, "End", 0, stdLineHeight, programFont, 'l');
  esp->whichend = HiddenGroup (g, 2, 0, NULL);
  RadioButton (esp->whichend, "5'");
  RadioButton (esp->whichend, "3'");
  SetValue (esp->whichend, 1);

  esp->extendfeat = CheckBox (h, "Extend features", NULL);

  k = HiddenGroup (h, 0, -2, NULL);
  StaticPrompt (k, "Sequence", 0, 0, programFont, 'l');
  esp->seq = ScrollText (k, 25, 5, programFont, TRUE, NULL);

  q = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (q, "Optional gene constraint", 0, dialogTextHeight, programFont, 'l');
  esp->genename = DialogText (q, "", 14, NULL);

  c = HiddenGroup (h, 4, 0, NULL);
  b = PushButton (c, "Accept", DoEditSeqEndsProc);
  SetObjectExtra (b, esp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) esp->extendfeat,
                (HANDLE) k, (HANDLE) q, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Select (w);
  Select (esp->seq);
  Update ();
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

static void TruncProtsCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  ByteStorePtr  bs;
  BioseqPtr     bsp;
  Int4          i;
  Int2          residue;
  SeqPortPtr    spp;

  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;
  if (! ISA_aa (bsp->mol)) return;
  if (bsp->repr != Seq_repr_raw && bsp->repr != Seq_repr_const) return;

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

  SeqPortFree (spp);
  bsp->seq_data = BSFree (bsp->seq_data);
  bsp->seq_data = bs;
  bsp->length = BSLen (bs);
  bsp->seq_data_type = Seq_code_ncbieaa;
}

static void TruncProtsAtStops (IteM i)

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
  SeqEntryExplore (sep, NULL, TruncProtsCallback);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
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

static void TrimProtFeatLengths (IteM i)

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
  SeqEntryExplore (sep, NULL, TrimProtsCallback);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
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

static Boolean LIBCALLBACK VecToN (SeqFeatPtr sfp, SeqMgrFeatContextPtr context)

{
  BioseqPtr  bsp;
  GBQualPtr  gbq;
  Int2       i, j, level, numivals;
  Boolean    isvector;
  Int4Ptr    ivals;
  Int2Ptr    levp;
  Int4       start, stop, swap;

  if (sfp == NULL || context == NULL) return TRUE;
  if (context->featdeftype != FEATDEF_misc_feature) return TRUE;
  levp = (Int2Ptr) context->userdata;
  if (levp == NULL) return TRUE;
  level = *levp;

  for (isvector = FALSE, gbq = sfp->qual; gbq != NULL; gbq = gbq->next) {
    if (StringCmp (gbq->qual, "standard_name") == 0) {
      if (StringCmp (gbq->val, "Vector Contamination") == 0) {
        isvector = TRUE;
      }
    }
  }
  if (! isvector) return TRUE;

  for (isvector = FALSE, gbq = sfp->qual; gbq != NULL; gbq = gbq->next) {
    if (StringCmp (gbq->qual, "phenotype") == 0) {
      if (StringCmp (gbq->val, "Strong match") == 0) {
        isvector = TRUE;
      } else if (StringCmp (gbq->val, "Moderate match") == 0 && level > 1) {
        isvector = TRUE;
      } else if (StringCmp (gbq->val, "Weak match") == 0 && level > 2) {
        isvector = TRUE;
      } else if (StringCmp (gbq->val, "Suspect origin") == 0) {
        isvector = TRUE;
      }
    }
  }
  if (! isvector) return TRUE;

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
  return TRUE;
}

static void VecScreenToNs (IteM i, Int2 level)

{
  BaseFormPtr  bfp;
  Boolean      featDefFilt [FEATDEF_MAX];

#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = (BaseFormPtr) GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  MemSet ((Pointer) featDefFilt, FALSE, sizeof (featDefFilt));
  featDefFilt [FEATDEF_misc_feature] = TRUE;
  SeqMgrVisitFeatures (bfp->input_entityID, (Pointer) &level, VecToN, NULL, featDefFilt);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
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

  align_type = (Int2) gcp->userdata;
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
    GatherItem (sel->entityID, sel->itemID, sel->itemtype, (Pointer) align_type, AddBlastTag);
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

  GenerateAutomaticDefLinesCommon (NULL, FALSE, TRUE, b);
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
                    bfp->input_itemtype, OBJ_SEQFEAT, FEATDEF_COMMENT, 0, 0);
  ArrowCursor ();
  Update ();
}

#ifdef WIN_MOTIF
extern void SUCCommonProc (IteM i, Boolean reverse, ButtoN b);
#endif

static void ToolBtn10 (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;

#ifdef WIN_MOTIF
  SUCCommonProc (NULL, FALSE, b);
#endif
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

static ButtoN SqnPushButton (GrouP prnt, CharPtr title, BtnActnProc actn, BaseFormPtr bfp)

{
  ButtoN  b;

  b = PushButton (prnt, title, actn);
  SetObjectExtra (b, (Pointer) bfp, NULL);
  return b;
}

extern void BioseqViewFormToolBar (GrouP h)

{
  BaseFormPtr  bfp;
  GrouP        g;

  bfp = (BaseFormPtr) GetObjectExtra (h);
  if (bfp == NULL) return;
  g = HiddenGroup (h, 1, 0, NULL);
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
#ifdef WIN_MOTIF
  SqnPushButton (g, "SUC", ToolBtn10, bfp);
#endif
  SqnPushButton (g, "sub_affil", ToolBtn11, bfp);
  SqnPushButton (g, "sub_add", ToolBtn12, bfp);
  SqnPushButton (g, "cit-sub-upd", ToolBtn16, bfp);
  SqnPushButton (g, "del_GBbck", ToolBtn13, bfp);
  SqnPushButton (g, "Validate", ToolBtn14, bfp);
  SqnPushButton (g, "Desktop", ToolBtn15, bfp);
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

static void DoRemoveProtTitles (IteM i)

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

  ClearProteinTitlesInNucProts (bfp->input_entityID, NULL);

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

/* Copied from seqmgr.c -- Make that one non-static instead if we */
/*                         end up using this.                     */

static ObjMgrDataPtr SeqMgrGetOmdpForBioseq (BioseqPtr bsp)

{
  ObjMgrDataPtr  omdp;
  ObjMgrPtr      omp;

  if (bsp == NULL) return NULL;
  omdp = (ObjMgrDataPtr) bsp->omdp;
  if (omdp != NULL) return omdp;
  omp = ObjMgrWriteLock ();
  omdp = ObjMgrFindByData (omp, bsp);
  ObjMgrUnlock ();
  bsp->omdp = (Pointer) omdp;
  return omdp;
}

/*=====================================================================*/
/*                                                                     */
/* DeleteFeature () - Delete a given feature.                          */
/*                                                                     */
/*=====================================================================*/

static Boolean DeleteFeature (SeqFeatPtr sfpDelete, 
			      SeqMgrFeatContextPtr fcontext)
{
  SeqAnnotPtr sapPrev;
  SeqAnnotPtr sapCurrent;
  SeqFeatPtr  sfpList;
  SeqFeatPtr  sfpPrev;
  SeqFeatPtr  sfpCurrent;

  /* Search the annot linked list */
  /* for a feature table.         */

  sapPrev = NULL;
  sapCurrent = fcontext->sap;
  while ((sapCurrent != NULL) && (sapCurrent->type != 1))
    {
      sapPrev = sapCurrent;
      sapCurrent = sapCurrent->next;
    }

  if (NULL == sapCurrent)
    return FALSE;

  sfpList = (SeqFeatPtr) sapCurrent->data;

  /* Search the feature list for the */
  /* feature that we're deleting.    */

  sfpCurrent = sfpList;
  sfpPrev = NULL;

  while ((sfpCurrent != NULL) && (sfpCurrent != sfpDelete))
    {
      sfpPrev = sfpCurrent;
      sfpCurrent = sfpCurrent->next;
    }

  /* If we found a matching seqfeat, unlink */
  /* it from the list and delete it.        */

  if (sfpCurrent != sfpDelete)
    return FALSE;

  if (NULL == sfpPrev)
    sapCurrent->data = sfpCurrent->next;
  else
    sfpPrev = sfpCurrent->next;

  SeqFeatFree (sfpCurrent);

  /* If this was the only seqfeat in the annot */
  /* then remove the annot from the list of    */
  /* annotations and delete it.                */

  if (sfpList == NULL)
    {
      if (NULL == sapPrev)
	fcontext->sap = sapCurrent->next;
      else
	sapPrev = sapCurrent->next;
      SeqAnnotFree (sapCurrent);
    }

  /* Return successfully */

  return TRUE;
}


/*=====================================================================*/
/*                                                                     */
/* RemoveCDS_Callback () - Called for each segment explored in         */
/*                         RemoveFirstCDSfromSegSets(), this function  */
/*                         removes the first CDS of a segment.         */
/*                                                                     */
/*=====================================================================*/

static Boolean LIBCALLBACK RemoveCDS_Callback (SeqLocPtr slp,
					       SeqMgrSegmentContextPtr scontext)
{
  SeqMgrFeatContext fcontext;
  SeqFeatPtr        sfp;
  Boolean           *dirtyFlag;
  Boolean           isDeleted;

  /* Get the coding region for this segment */

  sfp = SeqMgrGetOverlappingCDS (slp, &fcontext);

  /* Delete it */

  dirtyFlag = (Boolean PNTR) scontext->userdata;
  if (sfp != NULL)
    isDeleted = DeleteFeature (sfp, &fcontext);

  /* If a deletion was made, set the dirty flag */

  if (TRUE == isDeleted)
    (*dirtyFlag) = TRUE;

  /* We only want to remove a CDS from the first */
  /* segment so stop the exploring at this point */

  return FALSE;
}

/*=====================================================================*/
/*                                                                     */
/* RemoveFirstCDS() - Remove the CDS from the first segment of a given */
/*                    segmented sequence.                              */
/*                                                                     */
/*=====================================================================*/

static Boolean RemoveFirstCDS (SeqEntryPtr sep)
{
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  Int4           segCount;
  static Boolean dirtyFlag = FALSE;

  if (IS_Bioseq(sep))
    {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (bsp->repr == Seq_repr_seg) 
	segCount = SeqMgrExploreSegments (bsp, &dirtyFlag,
					  RemoveCDS_Callback);
    }
  else /* Bioseq Set */
    {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next)
    	RemoveFirstCDS (sep);
    }

  return dirtyFlag;
}

/*=====================================================================*/
/*                                                                     */
/* RemoveFirstCDSFromSegSets() - Remove the CDS from the first segment */
/*                               of all Segmented Bioseqs.             */
/*                                                                     */
/*=====================================================================*/

static void RemoveFirstCDSFromSegSets (IteM i)
{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;
  Uint2        entityID;
  Boolean      dirtyFlag;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL)
    return;

  WatchCursor ();
  Update ();

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL)
    return;

  dirtyFlag = RemoveFirstCDS (sep);

  if (TRUE == dirtyFlag)
    {
      entityID = ObjMgrGetEntityIDForChoice (sep);
      ObjMgrSetDirtyFlag (entityID, TRUE);
      ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
    }

  ArrowCursor ();
  Update ();
}

extern void LookupAllPubs (IteM i);
extern void ResolveExistingLocalIDs (IteM i);
extern void PromoteAlignsToBestIDProc (IteM i);
extern void SetSourceFocus (IteM i);
extern void ClearSourceFocus (IteM i);
extern void ExtraAccToHistByPos (IteM i);
extern void StrandUnknownToStrandPlus (IteM i);
extern void ClearCdsProducts (IteM i);
extern void ClearMrnaProducts (IteM i);
extern void MakeRedundantGPS (IteM i);
extern void FuseSlpJoins (IteM i);
extern void CreateSeqHistTPA (IteM i);

extern void SetupSpecialMenu (MenU m, BaseFormPtr bfp)

{
  IteM  i;
  MenU  s;
  MenU  x;

  s = SubMenu (m, "Def Line/ D");
  x = SubMenu (s, "Automatic Def Line");
  i = CommandItem (x, "No Source Modifiers", GenerateAutoDefLinesNoMods);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "With Source Modifiers...", GenerateAutoDefLinesWithMods);
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
  SeparatorItem (s);
  i = CommandItem (s, "Parse Def Line to Source", ParseDefToSource);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Parse Local ID to Source", ParseLocalIDToSource);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Parse File to Source", ParseFileToSource);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Parse ATCC Strain to Xref", AtccStrainToXref);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Append Strain to Organism", AddStrainToOrg);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Append Clone to Organism", AddCloneToOrg);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Append Subspecies to Organism", AddSubspeciesToOrg);
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


  s = SubMenu (m, "Apply/ A");
  i = CommandItem (s, "Add CDS", ApplyCDS);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Add RNA", ApplyRRNA);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Add other Feature", ApplyImpFeat);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Add Qualifier", AddQualifier);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Add Source Qual", AddSource);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Add CDS-Gene-Prot", AddCDSet);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Add RNA Qual", AddRNA);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Add Molecule Information", ApplyMolInfo);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Add Gene Pseudo", AddGenePseudo);
  SetObjectExtra (i, bfp, NULL);

  s = SubMenu (m, "Remove/ R");
  i = CommandItem (s, "Remove Descriptors", RemoveDescriptor);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove Features", RemoveFeature);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove Qualifiers", RemoveQualifier);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Remove Alignments", RemoveAlignment);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove Graphs", RemoveGraph);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove Proteins", RemoveProteins);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Remove Source Qual", RemoveSource);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove CDS-Gene-Prot", RemoveCDSet);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove 1st Seg's CDS for Each SegSet",
		   RemoveFirstCDSFromSegSets);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove RNA Qual", RemoveRNA);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove Pub", RemovePub);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove Protein Titles", DoRemoveProtTitles);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove ALL Protein Titles", DoRemoveAllProtTitles);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Strip Serial Numbers", StripSerials);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Remove Text Inside String", RemoveTextInsideString);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Remove Duplicate Genes", RemoveDuplicateGenes);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove Duplicate Feats", RemoveDuplicateFeats);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove Unnecessary Gene Xrefs", RemoveGeneXrefs);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove ALL Gene Xrefs", RemoveAllGeneXrefs);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Remove CDD Regions", RemoveCDDRegions);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Remove Duplicate CDDs", RemoveCDDDups);
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
  SeparatorItem (s);
  i = CommandItem (s, "Clear Gene Pseudo", ClearGenePseudo);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Remove BankIt Comments", RemoveBankitComments);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Remove Taxon Xrefs", RemoveTaxonXrefs);
  SetObjectExtra (i, bfp, NULL);
  if (indexerVersion) {
    SeparatorItem (s);
    i = CommandItem (s, "Remove Local IDs", RemoveLocalIDs);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Remove LOCUS from Parts", RemoveLocusFromParts);
    SetObjectExtra (i, bfp, NULL);
  }

  s = SubMenu (m, "Convert/ C");
  i = CommandItem (s, "Convert Features", ConvertFeatures);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Convert Qualifiers", ConvertQualifier);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Convert Source Qual", ConvertSource);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Convert CDS-Gene-Prot", ConvertCDSet);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Convert RNA Qual", ConvertRNA);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Parse Text from Flatfile", ParseAsnOrFlatfileToAnywhere);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Pub Descriptor to Feature", ConvertPubDescToFeat);
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
  i = CommandItem (s, "Transform to Bond", TransformToBond);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Transform to Site", TransformToSite);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Transform to Region", TransformToRegion);
  SetObjectExtra (i, bfp, NULL);
  if (indexerVersion) {
    SeparatorItem (s);
    i = CommandItem (s, "Resolve Colliding Local IDs", ResolveExistingLocalIDs);
    SetObjectExtra (i, bfp, NULL);
  }

  s = SubMenu (m, "Edit/ E");
  i = CommandItem (s, "Edit Qualifiers", EditQualifier);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Edit Source Qual", EditSource);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Edit CDS-Gene-Prot", EditCDSet);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Edit RNA Qual", EditRNA);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Edit Molecule Information", EditMolInfoFields);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Edit Feature Evidence", EditEvidenceFlag);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Edit Feature Exception", EditExceptionFlag);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Edit Feature Partials", EditFeaturePartials);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Strand Unkown to Strand Plus", StrandUnknownToStrandPlus);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Correct CDS Genetic Codes", CorrectCDSGenCodes);
  SetObjectExtra (i, bfp, NULL);
  /*
  i = CommandItem (s, "Adjust CDS Start Codon", CorrectCDSStartCodon);
  SetObjectExtra (i, bfp, NULL);
  */
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
/*#ifdef EDIT_LOCUS*/
  if (indexerVersion) {
    SeparatorItem (s);
    i = CommandItem (s, "Edit Locus", EditLocusProc);
    SetObjectExtra (i, bfp, NULL);
    SetupEditSecondary (s, bfp);
    i = CommandItem (s, "Convert Accession to LocalID", ConvertToLocalProc);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (s, "Extra Accession to History by Position", ExtraAccToHistByPos);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Promote Features to Best ID", PromoteToBestIDProc);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (s, "Promote Alignments to Best ID", PromoteAlignsToBestIDProc);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (s, "Remove Selected IDs from Bioseqs", RemoveIDsFromBioseqs);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Fuse Joins in Locations", FuseSlpJoins);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Edit Sequence Ends...", EditSeqEndsProc);
    SetObjectExtra (i, bfp, NULL);
  }
/*#endif*/
  SeparatorItem (s);
  i = CommandItem (s, "Suppress Genes On All RBS", SuppressGenesOnAllRBS);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Map RBSs to Downstream Gene", MapRBSsToDownstreamGene);
  SetObjectExtra (i, bfp, NULL);

  s = SubMenu (m, "Select/ S");
  i = CommandItem (s, "Select Descriptors", SelectDescriptor);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Select Features", SelectFeature);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Select Sequences", SelectBioseq);
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
  i = CommandItem (s, "Parse Table of Features", AutoParseFeatureTableProc);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Recompute Suggested Intervals", RecomputeSuggest);
  SetObjectExtra (i, bfp, NULL);
  x = SubMenu (s, "Retranslate Coding Regions");
  i = CommandItem (x, "Obey Stop Codon", RetranslateCdRegionsNoStop);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (x, "Ignore Stop Codon", RetranslateCdRegionsDoStop);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Resynchronize CDS Partials", ResynchronizeCDSPartials);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Resynchronize mRNA Partials", ResynchronizeMRNAPartials);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Map Feature Intervals to Parts", MergeToParts);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Map Intervals to Segmented Sequence", MergeToSegSeq);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Package Features on Parts", PackageOnParts);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Segmented Sequence to Raw Sequence", SegSeqToRawSeq);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Generate GenProdSet Redundancy", MakeRedundantGPS);
  SetObjectExtra (i, bfp, NULL);

  s = SubMenu (m, "Misc/ M");
  i = CommandItem (s, "Clean Up Record", ForceCleanup);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Force Taxonomy Fixup", ForceTaxonFixup);
  SetObjectExtra (i, bfp, NULL);
  if (indexerVersion) {
    i = CommandItem (s, "Genus-Species Fixup", GenSpecTaxonFixup);
    SetObjectExtra (i, bfp, NULL);
  }
  i = CommandItem (s, "Force Locus Fixup", ForceLocusFixup);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Set Source Focus", SetSourceFocus);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (s, "Clear Source Focus", ClearSourceFocus);
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
  i = CommandItem (s, "Protein Subtypes to ImpFeats", ProtToImpFeat);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Split Into Groups of 100 Bioseqs", MakeGroupsOf200);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Add Cit-sub for update", AddCitSubForUpdate);
  SetObjectExtra (i, bfp, NULL);
  SeparatorItem (s);
  i = CommandItem (s, "Alignment Summary...", ViewAlignmentSummary);
  SetObjectExtra (i, bfp, NULL);
  if (indexerVersion) {
    SeparatorItem (s);
    i = CommandItem (s, "Connect CDS to Closest Protein", ReconnectCDSProduct);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Reload Publications", LookupAllPubs);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Add Genomes User Object", AddGenomesUserObject);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (s);
    i = CommandItem (s, "Add Genomic Phrap Graphs", MakePhrap);
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
    SeparatorItem (s);
    i = CommandItem (s, "Create Seq-Hist for TPA", CreateSeqHistTPA);
    SetObjectExtra (i, bfp, NULL);
  }
  if (extraServices) {
    SeparatorItem (s);
    i = CommandItem (s, "Make ToolBar Window", MakeToolBarWindow);
    SetObjectExtra (i, bfp, NULL);
  }
#ifdef WIN_MOTIF
  SeparatorItem (m);
  i = CommandItem (m, "Sort Unique Count/ U", SUCProc);
  SetObjectExtra (i, bfp, NULL);
  i = CommandItem (m, "Sort Unique Count Resorted/ N", SUCRProc);
  SetObjectExtra (i, bfp, NULL);
#endif
/*#ifdef INTERNAL_NCBI_SEQUIN*/
/*#ifdef NEW_TAXON_SERVICE*/
  /*
  SeparatorItem (m);
  i = CommandItem (m, "Prepare TaxList", PrepareTaxListProc);
  */
/*#endif*/
/*#endif*/
}

