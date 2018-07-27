static char const rcsid[] = "$Id: toasn3.c,v 6.82 2003/06/18 21:52:21 kans Exp $";

/*****************************************************************************
*
*   toasn3.c
*   	converts a Seq-entry or elements of a Bioseq-set to new Biosource style
*
*****************************************************************************/

#include <gather.h>
#include <toasn3.h>
#include <toporg.h>
#include <tfuns.h>
#include <terr.h>
#include <utilpub.h>
#include "ftusrstr.h"
#include <utilpars.h>    /*ValidAminoAcid PROTO*/
#include <seqport.h>    /*GetFrameFromLoc PROTO*/
#include <asn2ff6.h>    /*AddGBQual PROTO*/
#include <sqnutils.h>
#include <explore.h>
#include <edutil.h>
#include <subutil.h>

static char *this_file = "toasn3";
#ifdef THIS_FILE
#undef THIS_FILE
#endif
#define THIS_FILE this_file
static char *this_module ="toasn3";
#ifdef THIS_MODULE
#undef THIS_MODULE
#endif
#define THIS_MODULE this_module

Boolean had_biosource = FALSE;
static SeqFeatPtr list;

#define num_bond 5
static CharPtr feat_bond[num_bond] = {NULL, "disulfide bond", "thiolester bond", "xlink bond", "thioether bond"};

#define num_site 26
static CharPtr feat_site[num_site] = {NULL, 
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
};

#define num_genome 15
static CharPtr genome[num_genome] = {"unknown", "genomic", "chloroplast", "chromoplast", "kinetoplast", "mitochondrion", "plastid", "macronuclear",
"extrachrom", "plasmid", "transposon", "insertion_seq", "cyanelle", "proviral", "virion"};

#define num_subtype 22
static CharPtr subtype[num_subtype] = {
"chromosome", "map", "clone", "sub_clone", "haplotype", "genotype", "sex",
"cell_line", "cell_type", "tissue_type", "clone_lib", "dev_stage", 
"frequency", "germline", "rearranged", "lab_host", "pop_variant",
"tissue_lib", "plasmid", "transposon", "insertion_seq", "plastid"};

static ORGMOD orgmod_subtype[10] = {
	{ "strain", 2 }, {"sub_strain", 3}, {"variety", 6}, {"serotype",7}, {"cultivar",10}, {"isolate", 17}, {"specific_host", 21}, {"sub_species", 22}, {"note", 255}, { NULL, 0 } 
};

#define num_bad_quals 3
static CharPtr bad_quals[num_bad_quals] = {
"label", "usedin", "citation"};

#define num_organelle 5
static ORGMOD organelle[num_organelle] = {
{"Mitochondrion ", 5}, {"Chloroplast ", 2},
{"Kinetoplast ", 4},  {"Cyanelle ", 12}, {"Plastid ", 6}
};

#define TOTAL_TECH 6
static ORGMOD check_tech[TOTAL_TECH] = {
{"EST", MI_TECH_est}, {"STS", MI_TECH_sts}, {"GSS", MI_TECH_survey},
{"HTG", MI_TECH_htgs_1 },  {"HTG", MI_TECH_htgs_2}, {"HTG", MI_TECH_htgs_3}
};
static void CheckGeneticCode(SeqEntryPtr sep);

static Int2 FindStr(CharPtr PNTR array, Int2 array_num, CharPtr str) {
	Char ch;
	Int2 i;
	size_t len;
	Char tmp [64];
	CharPtr val;
	
	for (i = 0; i < array_num; i++) {
		if (array[i] == NULL) {
			continue;
		}
		StringNCpy_0 (tmp, array[i], sizeof (tmp));
		len = StringLen (tmp);
		if (StringNCmp(str, tmp, len) == 0) {
			return i;
		}
		val = tmp;
		ch = *val;
		while (ch != '\0') {
			if (ch == '-') {
				*val = ' ';
			}
			val++;
			ch = *val;
		}
		if (StringNCmp(str, tmp, len) == 0) {
			return i;
		}
	}
	return -1;
}

/*****************************************************************************
*
*   ToAsn4(sep)
*   	Converts pubs to asn.1 spec 4.0 within SeqEntryPtr - SeqEntryPubsAsn4
*		move tax lineage from GBblock to BioSource
*****************************************************************************/
Int4 ToAsn4 (SeqEntryPtr sep)
{
	CharPtr lineage = NULL;
	
	SeqEntryPubsAsn4(sep);
	SeqEntryExplore(sep, (Pointer) (&lineage), FindOldLineage);
	if (lineage) {
		SeqEntryExplore(sep, (Pointer) (&lineage), NewLineage);
		MemFree(lineage);
	}
	return 0;		
}

static void CopySfpId(SeqFeatPtr new, SeqFeatPtr sfp)
{
	ObjectIdPtr oip, noip;
	DbtagPtr dtp, ndtp;
	
	if (sfp && sfp->id.choice) {
		new->id.choice = sfp->id.choice;
		switch(sfp->id.choice)
		{
			case 1:
				new->id.value.intvalue = sfp->id.value.intvalue;
				break;
			case 2:
				new->id.value.ptrvalue = 
					AsnIoMemCopy((Pointer) sfp->id.value.ptrvalue, 
						(AsnReadFunc) GiimAsnRead,
				 						(AsnWriteFunc) GiimAsnWrite);

				break;
			case 3:
				oip = (ObjectIdPtr) sfp->id.value.ptrvalue;
				noip =  AsnIoMemCopy(oip, (AsnReadFunc) ObjectIdAsnRead,
				 						(AsnWriteFunc) ObjectIdAsnWrite);
				new->id.value.ptrvalue = noip;
				break;
			case 4:
				dtp = (DbtagPtr) sfp->id.value.ptrvalue;
				ndtp =  AsnIoMemCopy(dtp, (AsnReadFunc) DbtagAsnRead,
				 						(AsnWriteFunc) DbtagAsnWrite);
				new->id.value.ptrvalue = ndtp;
				break;
			default:
				break;
		}
	}
	return;
}

static void toasn3_free(ToAsn3Ptr tap) 
{
	OrgFixPtr ofp, next_ofp;
	MolFixPtr mfp, next_mfp;
	
	if (tap == NULL) {
		return;
	}
	ofp = tap->ofp;
	while(ofp) {
		next_ofp = ofp->next;
		if (ofp->sfp) {
			SeqLocFree(ofp->sfp->location);
			if (ofp->sfp->cit) {
				PubSetFree(ofp->sfp->cit);
			}
			MemFree(ofp->sfp);
		}
		MemFree(ofp);
		ofp = next_ofp;
	}
	mfp = tap->mfp;
	while(mfp) {
		next_mfp = mfp->next;
		MemFree(mfp);
		mfp = next_mfp;
	}
}

static void vnp_psp_free(ValNodePtr vnp)
{
	ValNodePtr next_vnp;
	PubStructPtr psp;

	while (vnp) {
		next_vnp = vnp->next;
		psp = (PubStructPtr) vnp->data.ptrvalue;
		if (psp) {
			FreePubStruct(psp);
		}
		MemFree(vnp);
		vnp = next_vnp;
	}
}

static void vnp_list_free(ValNodePtr vnp)
{
	ValNodePtr next_vnp;
	PubdescPtr psp;

	while (vnp) {
		next_vnp = vnp->next;
		psp = (PubdescPtr) vnp->data.ptrvalue;
		if (psp) {
			PubdescFree(psp);
		}
		MemFree(vnp);
		vnp = next_vnp;
	}
}


static Boolean NOT_segment(SeqEntryPtr sep)
{
	BioseqSetPtr bssp;
	SeqEntryPtr seqsep;
	
	if (IS_Bioseq(sep))
		return TRUE;
	bssp = (BioseqSetPtr)(sep->data.ptrvalue);
	if (bssp->_class == 1) {    /*  1 - nucprot set  */
		seqsep = bssp->seq_set;
		if (seqsep == NULL) {
			return TRUE;
		}
		if (seqsep->choice == 1) { 
			return TRUE;
		}
	}
	return FALSE;
}

static OrgFixPtr OrgFixNew(void)
{
	OrgFixPtr ofp;
	
	ofp = MemNew(sizeof(OrgFix));
	ofp->contains = NULL;
	ofp->desc = FALSE;
	ofp->sfp = NULL;
	ofp->imp = NULL;
	ofp->orp = NULL;
	
	return ofp;
}

/*****************************************************************************/
static void AddOrgToFix (OrgRefPtr orp, ToAsn3Ptr tap, ValNodePtr mod,
					SeqEntryPtr sep, ValNodePtr vnp, SeqFeatPtr sfp, Int4 index)
{
	OrgFixPtr ofp;
	SeqFeatPtr feat;

	ofp = OrgFixNew();
	ofp->contains = sep;
	if (vnp == NULL) {
		ofp->desc = FALSE;
	} else {
		ofp->desc = TRUE;
	}
	if (sfp) {
		feat = SeqFeatNew();
		if (sfp->id.choice) {
			CopySfpId(feat, sfp);
		}
		feat->location = 
			AsnIoMemCopy(sfp->location, (AsnReadFunc) SeqLocAsnRead,
				(AsnWriteFunc) SeqLocAsnWrite);
		if (sfp->cit) {
			feat->cit = AsnIoMemCopy(sfp->cit, (AsnReadFunc) PubSetAsnRead,
				(AsnWriteFunc) PubSetAsnWrite);
		}
		ofp->sfp = feat;
	}
	ofp->orp = orp;
	ofp->index = index;
	if (mod != NULL) {
		ofp->modif = mod->data.ptrvalue;
	}
	tap->ofp = tie_next_biosource(tap->ofp, ofp);
	
	return;
}

/*****************************************************************************/
static void AddMolToFix (ToAsn3Ptr tap, SeqEntryPtr sep,
                         Uint1 mol, ValNodePtr mod, Uint1 meth, Int4 index)
{
	MolFixPtr mfp;

	mfp = MemNew(sizeof(MolFix));
	mfp->contains = sep;
	if (mol != 0)
		mfp->mol = mol;
	if (mod != NULL) {
		mfp->modif = mod;
	}
	if (meth != 0)
		mfp->method = meth;
	mfp->index = index;
	tap->mfp = tie_next_mol(tap->mfp, mfp);
	
	return;
}
/*****************************************************************************/

static void AddImpToFix (SeqFeatPtr imp, ToAsn3Ptr tap, SeqEntryPtr sep,
									 ValNodePtr vnp, SeqFeatPtr sfp, Int4 index)
{
	OrgFixPtr ofp;
	SeqFeatPtr	feat;

	ofp = OrgFixNew();
	ofp->contains = sep;
	if (vnp == NULL) {
		ofp->desc = FALSE;
	} else {
		ofp->desc = TRUE;
	}
	if (sfp) {
		feat = SeqFeatNew();
		feat->location = 
			AsnIoMemCopy(sfp->location, (AsnReadFunc) SeqLocAsnRead,
				(AsnWriteFunc) SeqLocAsnWrite);
		if (sfp->cit) {
			feat->cit = AsnIoMemCopy(sfp->cit, (AsnReadFunc) PubSetAsnRead,
				(AsnWriteFunc) PubSetAsnWrite);
		}
		ofp->sfp = feat;
	}
	ofp->imp = imp;
	ofp->index = index;
	tap->ofp = tie_next_biosource(tap->ofp, ofp);
	
	return;
}

/*****************************************************************************/
static void FixToAsn(SeqEntryPtr sep, ToAsn3Ptr tap) 
{
	OrgFixPtr	ofp;
	BioSourcePtr bsp;
	MolFixPtr	 mfp;
	MolInfoPtr	mfi;
	Uint1 		mod;
	ValNodePtr	vnp;
	OrgRefPtr	orp;
	GBQualPtr	q;
	CharPtr		tmp;
	Int2		i, len = 0;
	
	mfp = tap->mfp;
	while (mfp != NULL) {

		mfi = NULL;
		if (mfp->mol != 0) {
			mfi = new_info(mfi);
			mfi->biomol = mfp->mol;
		}
		if (mfp->method != 0) {
			mfi = new_info(mfi);
			mfi->tech = mfp->method + 7;
		}
		for(vnp = mfp->modif; vnp != NULL; vnp=vnp->next) {
			mod = vnp->data.intvalue;
			mfi = ModToMolInfo(mfi, mod);
		}
		mfp->molinfo = mfi;
		mfp = mfp->next;
	
	}

/* look for Org-refs (desc or feature) and create Biosource */
	for (ofp = tap->ofp; ofp != NULL; ofp = ofp->next) {
		if (ofp->orp != NULL && (ofp->desc != FALSE || ofp->sfp != NULL)) {
			bsp = BioSourceNew();
			bsp->org = AsnIoMemCopy(ofp->orp, (AsnReadFunc) OrgRefAsnRead, 
               (AsnWriteFunc) OrgRefAsnWrite);
            tap->had_biosource = TRUE;
            for (mfp = tap->mfp; mfp; mfp = mfp->next) {
            	if (ofp->index < mfp->index) {
            		continue;
            	}
			     for(vnp = mfp->modif; vnp != NULL; vnp=vnp->next) {
				    mod = vnp->data.intvalue;
				    if (bsp == NULL) {
					    bsp = BioSourceNew();
				    }
				    ModToBiosource(bsp, mod);
			    }
			}
			ofp->bsp = bsp;
       }
	} 
/* look for Impfeat: create new bsp for every "source" (desc or feature) */
	for (ofp = tap->ofp; ofp != NULL; ofp = ofp->next) {
		if (ofp->imp == NULL) {
			continue;
		}
		if (ofp->desc == TRUE) {
			bsp = BioSourceNew();
			orp = OrgRefNew();
			for(q = ofp->imp->qual; q != NULL; q = q->next) {
				if (StringCmp(q->qual, "organism") == 0) {
					tmp = MemNew(StringLen(q->val)+1);
					StringCpy(tmp, q->val);
					for (i = 0; i < num_organelle; i++) {
						if (StringNCmp(tmp, organelle[i].name, 
							StringLen(organelle[i].name)) == 0) {
							len = StringLen(organelle[i].name);
							bsp->genome = organelle[i].num;
							break;
						}
					}
					orp->taxname = StringSave(tmp + len);
					MemFree(tmp);
				}
			}	
			bsp->org = orp;
			if (ofp->imp && ofp->imp->qual)
				CheckQualsWithComm(bsp, ofp->imp);
			ofp->bsp = bsp;
		} else if(ofp->sfp != NULL) {
			bsp = BioSourceNew();
			orp = OrgRefNew();
			for(q = ofp->imp->qual; q != NULL; q = q->next) {
				if (StringCmp(q->qual, "organism") == 0) {
					tmp = MemNew(StringLen(q->val)+1);
					StringCpy(tmp, q->val);
					for (i = 0; i < num_organelle; i++) {
						if (StringNCmp(tmp, organelle[i].name, 
							StringLen(organelle[i].name)) == 0) {
							len = StringLen(organelle[i].name);
							bsp->genome = organelle[i].num;
							break;
						}
					}
					orp->taxname = StringSave(tmp + len);
					MemFree(tmp);
				}
			}	
			bsp->org = orp;
			if (ofp->imp && ofp->imp)
				CheckQualsWithComm(bsp, ofp->imp);
			ofp->bsp = bsp;
		}
	}
}

/*****************************************************************************
*
*  Build MolInfo from GIBBmod and GIBBmol GIBBmethod
*
*****************************************************************************/
static void FixMol (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	ValNodePtr 		vnp, tmp;
	BioseqPtr		bsp;
	BioseqSetPtr 	bssp;
	MolFixPtr		mfp;

	mfp = (MolFixPtr)data;

	if (IS_Bioseq(sep))
	{
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		if ((bsp->repr != Seq_repr_raw) && (bsp->repr != Seq_repr_const)
		 		&& (bsp->repr != Seq_repr_delta))
			return;
		vnp = bsp->descr;
	}
	else
	{
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		vnp = bssp->descr;
	}
	while (mfp != NULL) {
		if (mfp->index == index && mfp->molinfo != NULL) {
			tmp = SeqDescrNew(vnp);
			tmp->choice = Seq_descr_molinfo;
			tmp->data.ptrvalue = mfp->molinfo;
		}
		mfp = mfp->next;
	}
	
	return;
}

static void FixProtMolInfo (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)

{
	BioseqPtr   bsp;
	MolInfoPtr  mip = NULL;
	ValNodePtr  vnp;

	if (! IS_Bioseq(sep)) return;
	bsp = (BioseqPtr) sep->data.ptrvalue;
	if (bsp == NULL) return;
	if (! ISA_aa (bsp->mol)) return;
	for (vnp = bsp->descr; vnp != NULL; vnp = vnp->next) {
		if (vnp->choice == Seq_descr_molinfo) {
			mip = (MolInfoPtr) vnp->data.ptrvalue;
			if (mip != NULL) {
				if (mip->biomol == 0) {
					mip->biomol = 8;
				}
			}
		}
	}
	if (mip == NULL) {
		mip = MolInfoNew ();
		if (mip == NULL) return;
		mip->biomol = 8;
		vnp = CreateNewDescriptor (sep, Seq_descr_molinfo);
		if (vnp == NULL) return;
		vnp->data.ptrvalue = (Pointer) mip;
	}
}

static void FuseMolInfos (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)

{
	BioseqPtr        bsp;
	MolInfoPtr       first;
	MolInfoPtr       mip;
	ValNodePtr       next;
	ValNodePtr PNTR  prev;
	Boolean          remove;
	ValNodePtr       vnp;

	if (! IS_Bioseq(sep)) return;
	bsp = (BioseqPtr) sep->data.ptrvalue;
	if (bsp == NULL) return;
	vnp = bsp->descr;
	prev = &(bsp->descr);
	first = NULL;
	while (vnp != NULL) {
		remove = FALSE;
		next = vnp->next;
		if (vnp->choice == Seq_descr_molinfo) {
			mip = (MolInfoPtr) vnp->data.ptrvalue;
			if (first == NULL) {
				first = mip;
			} else if (mip != NULL) {
				if (first->biomol == 0) {
					first->biomol = mip->biomol;
				}
				if (first->tech == 0) {
					first->tech = mip->tech;
				}
				if (first->completeness == 0) {
					first->completeness = mip->completeness;
				}
				if (first->biomol == mip->biomol &&
					first->tech == mip->tech &&
					first->completeness == mip->completeness) {
					if (first->techexp == NULL) {
						first->techexp = mip->techexp;
						mip->techexp = NULL;
					}
					remove = TRUE;
				}
			}
		}
		if (remove) {
			*prev = vnp->next;
			vnp->next = NULL;
			MolInfoFree (mip);
			ValNodeFree (vnp);
		} else {
			prev = &(vnp->next);
		}
		vnp = next;
	}
}

/*****************************************************************************
*
*  Build Biosource from descr-org and features
*
*****************************************************************************/
static void FixOrg (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	ValNodePtr 		vnp, tmp;
	BioseqPtr		bsp;
	BioseqSetPtr 	bssp;
	OrgFixPtr		ofp;
	SeqAnnotPtr		sap;
	SeqFeatPtr		new;

	ofp = (OrgFixPtr)data;

	if (IS_Bioseq(sep))
	{
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		if ((bsp->repr != Seq_repr_raw) && (bsp->repr != Seq_repr_const)
					&& (bsp->repr != Seq_repr_delta))
			return;
		vnp = bsp->descr;
		sap = bsp->annot;
	}
	else
	{
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		vnp = bssp->descr;
		sap = bssp->annot;
	}
	for (; ofp != NULL; ofp = ofp->next) {
		if (ofp->desc == TRUE) {
			if (ofp->index == index && ofp->bsp != NULL) {
				tmp = SeqDescrNew(vnp);
				tmp->choice = Seq_descr_source;
				tmp->data.ptrvalue = ofp->bsp;
			}
		}
		if (ofp->index == index && ofp->sfp != NULL && ofp->bsp != NULL) {
			new = SeqFeatNew();
			if (ofp->sfp->id.choice) {
				CopySfpId(new, ofp->sfp);
			}
			new->data.choice = SEQFEAT_BIOSRC;
			new->data.value.ptrvalue = ofp->bsp;
			new->location =  AsnIoMemCopy(ofp->sfp->location, 
					(AsnReadFunc) SeqLocAsnRead, (AsnWriteFunc) SeqLocAsnWrite);
			if (ofp->sfp->cit) {
				new->cit =  AsnIoMemCopy(ofp->sfp->cit, 
					(AsnReadFunc) PubSetAsnRead, (AsnWriteFunc) PubSetAsnWrite);
			}
			sap->data = tie_feat(sap->data, new);
		}
	}
	return;
}

/*****************************************************************************
*
*   HasSiteRef(sfp, userdata)
*   	Checks for Site-ref ImpFeat before unnecessarily rearranging pub descriptors
*****************************************************************************/
static void HasSiteRef (SeqFeatPtr sfp, Pointer userdata)

{
  BoolPtr     foundP;
  ImpFeatPtr  ifp;

  foundP = (BoolPtr) userdata;
  if (sfp->cit == NULL) return;
  if (sfp->data.choice != SEQFEAT_IMP) return;
  ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
  if (ifp == NULL) return;
  if (StringCmp(ifp->key, "Site-ref") == 0) {
    *foundP = TRUE;
  }
}

/*****************************************************************************
*
*   SeqEntryPubsAsn4(sep)
*   	Converts pubs to asn.1 spec 4.0 within SeqEntryPtr
*****************************************************************************/
Int4 SeqEntryPubsAsn4 (SeqEntryPtr sep)
{
	BioseqSetPtr bioset = NULL;
	CharPtr lineage = NULL;
	ValNodePtr vnp = NULL, publist= NULL, tmp, v;
	PubdescPtr		pubdesc;
	ValNodePtr		descr = NULL;
	Boolean foundSitRef = FALSE;
	
	if (!IS_Bioseq(sep)) {
		bioset = (BioseqSetPtr) (sep->data.ptrvalue); /* top level set */
	} 
	SeqEntryExplore(sep, &vnp, FindCit);
	SeqEntryExplore(sep, &vnp, ChangeCitQual);
	vnp_psp_free(vnp); 
	
	VisitFeaturesInSep (sep, (Pointer) &foundSitRef, HasSiteRef);
	if (foundSitRef) {
		SeqEntryExplore(sep, NULL, NewPubs);
	}
	SeqEntryExplore(sep, NULL, DeleteSites);

/* move pubs in set to the top level */
	if (bioset && bioset->_class != 9) {
		SeqEntryExplore(sep, (Pointer) NULL, MoveSegmPubs);
		SeqEntryExplore(sep, (Pointer) NULL, MoveNPPubs);
/*   unique pubs on the set level*/
		tmp = ValNodeExtractList(&bioset->descr, Seq_descr_pub);
		for (v = tmp; v; v = v->next) {
			pubdesc = v->data.ptrvalue;
			publist = AddToList(publist, NULL, pubdesc);
		}
		bioset->descr = ValNodeLink(&(bioset->descr), publist);
/* check pubs in Bioseqs, delete if they are already on the top */
		for (v = publist; v; v = v->next) {
			pubdesc = v->data.ptrvalue;
			SeqEntryExplore(sep, pubdesc, DeletePubs);
		}
		vnp_list_free(tmp); 
	}
	SeqEntryExplore(sep, NULL, ChangeCitSub);
	return 0;		
}
/*****************************************************************************
*
*   StripAffil, StripAffilFromPep, and StripAffilFromPub
*   	Removes phone and e-mail address from Affil in Pubs.
*****************************************************************************/

static void StripAffilFromPub (ValNodePtr vnp)

{
  AffilPtr     afp;
  AuthListPtr  alp;
  CitArtPtr    cap;
  CitGenPtr    cgp;
  CitPatPtr    cpp;
  CitSubPtr    csp;
  CharPtr      tmp;

  if (vnp != NULL) {
    afp = NULL;
    alp = NULL;
    switch (vnp->choice) {
      case PUB_Gen :
        cgp = (CitGenPtr) vnp->data.ptrvalue;
        if (cgp != NULL) {
          alp = cgp->authors;
        }
        break;
      case PUB_Article :
        cap = (CitArtPtr) vnp->data.ptrvalue;
        if (cap != NULL) {
          alp = cap->authors;
        }
        break;
      case PUB_Sub :
        csp = (CitSubPtr) vnp->data.ptrvalue;
        if (csp != NULL) {
          alp = csp->authors;
        }
        break;
      case PUB_Patent :
        cpp = (CitPatPtr) vnp->data.ptrvalue;
        if (cpp != NULL) {
          alp = cpp->authors;
        }
        break;
      default :
        break;
    }
    if (alp != NULL) {
      afp = alp->affil;
      if (afp != NULL && afp->choice == 2) {
        afp->phone = MemFree (afp->phone);
        if (afp->postal_code != NULL) {
          tmp = MemNew (StringLen (afp->sub) + StringLen (afp->postal_code) + 4);
          if (tmp != NULL) {
            if (afp->sub != NULL) {
              StringCpy (tmp, afp->sub);
              StringCat (tmp, " ");
            }
            StringCat (tmp, afp->postal_code);
            afp->sub = MemFree (afp->sub);
            afp->sub = tmp;
          }
          afp->postal_code = MemFree (afp->postal_code);
        }
      }
    }
  }
}

static void StripAffilFromPep (ValNodePtr vnp)

{
  while (vnp != NULL) {
    StripAffilFromPub (vnp);
    vnp = vnp->next;
  }
}

static void StripAffil (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  PubdescPtr    pdp;
  ValNodePtr    pep;
  ValNodePtr    ppr;
  ValNodePtr    psp;
  SeqAnnotPtr   sap;
  ValNodePtr    sdp;
  SeqFeatPtr    sfp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
    sdp = bsp->descr;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
    sdp = bssp->descr;
  } else {
    return;
  }
  while (sdp != NULL) {
    if (sdp->choice == Seq_descr_pub) {
      pdp = sdp->data.ptrvalue;
      if (pdp != NULL) {
        pep = pdp->pub;
        StripAffilFromPep (pep);
      }
    }
    sdp = sdp->next;
  }
  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        if (sfp->data.choice == SEQFEAT_PUB) {
          pdp = sfp->data.value.ptrvalue;
          if (pdp != NULL) {
            pep = pdp->pub;
            StripAffilFromPep (pep);
          }
        }
        psp = sfp->cit;
        if (psp != NULL) {
          ppr = psp->data.ptrvalue;
          while (ppr != NULL) {
            if (ppr->choice == PUB_Equiv) {
              pep = ppr->data.ptrvalue;
              StripAffilFromPep (pep);
            } else {
              StripAffilFromPub (ppr);
            }
            ppr = ppr->next;
          }
        }
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
}

/*****************************************************************************
*
*   SeqEntryPubsAsn3(sep)
*   	Converts pubs back to asn1.spec 3.0 within SeqEntryPtr
*		moves tax lineage from BioSource to GBblock
*		removes gcode from BioSource
*****************************************************************************/

Int4 SeqEntryPubsAsn3 (SeqEntryPtr sep)
{
	CharPtr lineage = NULL;
	
	OldPubs(sep);
	SeqEntryExplore (sep, NULL, StripAffil);
	SeqEntryExplore(sep, (Pointer) (&lineage), FindNewLineage);
	if (lineage) {
		SeqEntryExplore(sep, (Pointer) (&lineage), OldLineage);
	}
	return 0;
}

/*****************************************************************************
*
*  Remove old (ver 2.0)  asn.1 (with check for the new ver 3.0)
*
*****************************************************************************/
void StripOld (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	ValNodePtr	vnp = NULL, tmp = NULL, tmpnext;
	SeqFeatPtr	sfp, sfp_next;
	OrgRefPtr	orp;
	SeqAnnotPtr	sap, ap, apnext;
	BioseqPtr	bsp = NULL;
	BioseqSetPtr	bssp;
	
	if (IS_Bioseq(sep)) {
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		if ((bsp->repr != Seq_repr_raw) && (bsp->repr != Seq_repr_const) 
					&& (bsp->repr != Seq_repr_delta))
			return;

		vnp = bsp->descr;
		sap = bsp->annot;
	} else {
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		vnp = bssp->descr;
		sap = bssp->annot;
	}
	if (vnp) {
		for (tmp=ValNodeExtractList(&vnp, Seq_descr_modif);tmp;tmp=tmpnext){
			tmpnext = tmp->next;
			ValNodeFree(tmp->data.ptrvalue);
			MemFree(tmp);
		}
		for (tmp=ValNodeExtractList(&vnp, Seq_descr_mol_type);tmp;tmp=tmpnext) {
			tmpnext = tmp->next;
			MemFree(tmp);
		}
		for (tmp=ValNodeExtractList(&vnp, Seq_descr_method); tmp; tmp=tmpnext) {
			tmpnext = tmp->next;
			MemFree(tmp);
		}
		for (tmp = ValNodeExtractList(&vnp, Seq_descr_org); tmp; tmp = tmpnext){
			tmpnext = tmp->next;
			orp = (OrgRefPtr) tmp->data.ptrvalue;
			OrgRefFree(orp);
			MemFree(tmp);
		}
		if (IS_Bioseq(sep)) {
			bsp->descr = vnp;
		} else {
			bssp->descr = vnp;
		}
	}
	for (ap = sap; ap; ap = apnext) {
		apnext = ap->next;
		if (ap->type != 1) 
			continue; 
		/* tmp_sfp = (SeqFeatPtr) (ap->data); */
		for(sfp = ExtractSourceFeatList((SeqFeatPtr PNTR) &(ap->data));sfp;sfp=sfp_next){
			sfp_next=sfp->next;
			SeqFeatFree(sfp);
		}
		for(sfp = SeqFeatExtractList((SeqFeatPtr PNTR) &(ap->data), SEQFEAT_ORG);sfp;sfp=sfp_next){
			sfp_next=sfp->next;
			SeqFeatFree(sfp);
		}
		/* ap->data = tmp_sfp; */
		if (ap->data == NULL) {
			sap = remove_annot(sap, ap);
		}
	}
	if (IS_Bioseq(sep)) {
		bsp->annot = sap;
	} else {
		bssp->annot = sap;
	}
}

/*****************************************************************************
* EMBL may have multiple OS lines that are parsed to multiple descr on
* the top level. In NCBI model only one Biosource descr is allowed, others
* should be moved to the feature table
*****************************************************************************/
ValNodePtr GetMultBiosource(SeqEntryPtr sep)
{
	ValNodePtr bvnp, vnp, retval;
	BioseqPtr bsp;
	BioseqSetPtr bssp;
	
	if (sep == NULL)
		return NULL;
	if (IS_Bioseq(sep)) {    
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		vnp = bsp->descr;
	} else {
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		vnp = bssp->descr;
	}
	bvnp = 	ValNodeExtractList(&vnp, Seq_descr_source);
	if (bvnp == NULL) {
		return NULL;
	}
	if (bvnp->next != NULL) {
		retval = bvnp->next;
		bvnp->next = NULL;
	} else {
		retval = NULL;
	}
	vnp = tie_next(vnp, bvnp);    
	if (IS_Bioseq(sep)) {
		bsp->descr = vnp;    
	} else {
		bssp->descr = vnp;    
	}
	return retval;	
}

/*****************************************************************************
* RemoveEmptyTitleAndPubGenAsOnlyPub removes pub { pub { gen { } } empty pubs
*****************************************************************************/

/* from utilpub.c */
static Boolean empty_citgen(CitGenPtr  cit)
{
	if (cit == NULL)
		return TRUE;
	if (cit->cit)
		return FALSE;
	if (cit->authors)
		return FALSE;
	if (cit->muid > 0)
		return FALSE;
	if (cit->journal)
		return FALSE;
	if (cit->volume)
		return FALSE;
	if (cit->issue)
		return FALSE;
	if (cit->pages)
		return FALSE;
	if (cit->date)
		return FALSE;
	if (cit->serial_number > 0)
		return FALSE;
	if (cit->title)
		return FALSE;
	if (cit->pmid > 0)
		return FALSE;
	return TRUE;
}

static Boolean PubIsEffectivelyEmpty (PubdescPtr pdp)

{
  ValNodePtr       vnp;

  if (pdp == NULL) return FALSE;
  vnp = pdp->pub;
  if (vnp != NULL && vnp->next == NULL && vnp->choice == PUB_Gen) {
    if (empty_citgen ((CitGenPtr) vnp->data.ptrvalue)) {
      return TRUE;
    }
  }
  return FALSE;
}

static void RemoveEmptyTitleAndPubGenAsOnlyPub (SeqEntryPtr sep)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  SeqAnnotPtr   nextsap;
  ValNodePtr    nextsdp;
  SeqFeatPtr    nextsfp;
  Pointer PNTR  prevsap;
  Pointer PNTR  prevsdp;
  Pointer PNTR  prevsfp;
  SeqAnnotPtr   sap = NULL;
  ValNodePtr    sdp = NULL;
  SeqFeatPtr    sfp;
  SeqEntryPtr   tmp;

  if (sep == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp == NULL) return;
    sap = bsp->annot;
    prevsap = (Pointer PNTR) &(bsp->annot);
    sdp = bsp->descr;
    prevsdp = (Pointer PNTR) &(bsp->descr);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL) return;
    for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
      RemoveEmptyTitleAndPubGenAsOnlyPub (tmp);
    }
    sap = bssp->annot;
    prevsap = (Pointer PNTR) &(bssp->annot);
    sdp = bssp->descr;
    prevsdp = (Pointer PNTR) &(bssp->descr);
  } else return;
  while (sap != NULL) {
    nextsap = sap->next;
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      prevsfp = (Pointer PNTR) &(sap->data);
      while (sfp != NULL) {
        nextsfp = sfp->next;
        if (sfp->data.choice == SEQFEAT_PUB && PubIsEffectivelyEmpty ((PubdescPtr) sfp->data.value.ptrvalue)) {
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
    if (sdp->choice == Seq_descr_pub && PubIsEffectivelyEmpty ((PubdescPtr) sdp->data.ptrvalue)) {
      *(prevsdp) = sdp->next;
      sdp->next = NULL;
      SeqDescFree (sdp);
    } else if (sdp->choice == Seq_descr_title && StringHasNoText ((CharPtr) sdp->data.ptrvalue)) {
      *(prevsdp) = sdp->next;
      sdp->next = NULL;
      SeqDescFree (sdp);
    } else {
      prevsdp = (Pointer PNTR) &(sdp->next);
    }
    sdp = nextsdp;
  }
}

/*****************************************************************************
*   SeqEntryToAsn3(sep)
*   	Converts a SeqEntry with old OrgRefs to SeqEntry with Biosource
*		Does the Taxonomy lookup if taxserver = TRUE and taxfun != NULL 
*		ATTENTION: TaxArchInit() should be called before using this function
*		or taxserver set to FALSE !!!
*		Strips old stuff if strip_old=TRUE
*		Moves /map from GeneRef, removes ProtRef xrefs and checks genetic
*		code in CDSs
*		RETURN:
*		INFO_ASNOLD - if the entry is in spec 3.0 (has BioSource) already
*		INFO_ASNNEW - if the entry is converted to new spec
*		ERR_REJECT -  if the entry has internal FATAL errors
*		ERR_INPUT -   if input is NULL
*
*****************************************************************************/
Int4 SeqEntryToAsn3 (SeqEntryPtr sep, Boolean strip_old, Boolean source_correct, Boolean taxserver, SeqEntryFunc taxfun)
{
	return SeqEntryToAsn3Ex(sep, strip_old, source_correct, 
			taxserver, taxfun, NULL);
}
static Boolean is_equiv(SeqEntryPtr sep)
{
	BioseqSetPtr bssp;
	
	if (IS_Bioseq(sep)) {
		return FALSE;
	}
	bssp = (BioseqSetPtr)(sep->data.ptrvalue);
	if (bssp->_class != 10) {    /*  equiv */
		return FALSE;
	}
	return TRUE; 
}

static void RestoreUpdateDatePos (SeqEntryPtr sep, Int2 update_date_pos)

{
  BioseqSetPtr  bssp;
  ValNodePtr    descr;
  ValNodePtr    vnp;

  if (update_date_pos < 0) return;
  if (! IS_Bioseq_set (sep)) return;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp == NULL) return;

  vnp = ValNodeExtractList (&(bssp->descr), Seq_descr_update_date);
  if (vnp == NULL) return;
  if (update_date_pos == 0) {
    vnp->next = bssp->descr;
    bssp->descr = vnp;
  } else {
    descr = bssp->descr;
    while (update_date_pos > 1 && descr != NULL) {
      descr = descr->next;
      update_date_pos--;
    }
    if (descr != NULL) {
      vnp->next = descr->next;
      descr->next = vnp;
    } else {
      bssp->descr = ValNodeLink (&(bssp->descr), vnp);
    }
  }
}

static Int2 GetUpdateDatePos (SeqEntryPtr sep)

{
  BioseqSetPtr  bssp;
  Int2          i;
  ValNodePtr    vnp;

  if (! IS_Bioseq_set (sep)) return -1;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp == NULL) return -1;

  for (vnp = bssp->descr, i = 0; vnp != NULL; vnp = vnp->next, i++) {
    if (vnp->choice == Seq_descr_update_date) return i;
  }
  return -1;
}

/*****************************************************************************
*   SeqEntryToAsn3Ex(sep)
*   	Converts a SeqEntry with old OrgRefs to SeqEntry with Biosource
*		Does the Taxonomy lookup if taxserver = TRUE and taxfun != NULL 
*		ATTENTION: TaxArchInit() should be called before using this function
*		or taxserver set to FALSE !!!
*		Strips old stuff if strip_old=TRUE
*		Moves /map from GeneRef, removes ProtRef xrefs and checks genetic
*		code in CDSs
*		RETURN:
*		INFO_ASNOLD - if the entry is in spec 3.0 (has BioSource) already
*		INFO_ASNNEW - if the entry is converted to new spec
*		ERR_REJECT -  if the entry has internal FATAL errors
*		ERR_INPUT -   if input is NULL
*
*		New argument added SeqEntryFunc taxmerge
*		txfun - GetTaxserverOrg
*		taxmerge - TaxMergeBSinDescr
*****************************************************************************/
Int4 SeqEntryToAsn3Ex (SeqEntryPtr sep, Boolean strip_old, Boolean source_correct, Boolean taxserver, SeqEntryFunc taxfun, SeqEntryFunc taxmerge)
{
	ToAsn3 ta;
	OrgFixPtr ofp = NULL;
	MolFixPtr mfp = NULL;
	CharPtr porg = NULL;
	Boolean multiple = FALSE;
	QualMap qm;
	BSMap bs;
	ValNodePtr mult = NULL;
	Int4 retval = INFO_ASNOLD, ret;
	Int2 update_date_pos;
	
	ta.had_biosource = FALSE;
	ta.had_molinfo = FALSE;
	ta.ofp = NULL;
	ta.mfp = NULL;
	qm.name = NULL;
	qm.same = TRUE;
	bs.same = TRUE;
	bs.bsp = NULL;
	
	if (sep == NULL) {
		return ERR_INPUT;
	}
	update_date_pos = GetUpdateDatePos (sep);
	RemoveEmptyTitleAndPubGenAsOnlyPub (sep);
	if (source_correct) {
		SeqEntryExplore(sep, (Pointer)(&porg), CorrectSourceFeat);
	}
	toporg(sep);
	SeqEntryExplore(sep, (Pointer)(&ta), FindOrg);
	
	if (ta.had_biosource) {    
/* entry is in asn.1 spec 3.0 already do the checks only */
		retval |= INFO_ASNNEW;
		if(strip_old) {
			SeqEntryExplore(sep, NULL, StripOld);
		}
		ToAsn4(sep);   			/* move pubs and lineage */
		CombineBSFeat(sep);
		if (taxserver && taxfun != NULL) {
			SeqEntryExplore(sep, NULL, taxfun);
		}
		if (is_equiv(sep)) {
			/*do nothing*/
		}else if (NOT_segment(sep)) {
			if (taxserver && taxmerge != NULL) {
				SeqEntryExplore(sep, mult, taxmerge);
			} else {
				SeqEntryExplore(sep, mult, MergeBSinDescr);
			}
		} else {
			SeqEntryExplore(sep, (Pointer) (&bs), CheckBS);
			if (bs.same == TRUE) {
				SeqEntryExplore(sep, (Pointer) (&bs), StripBSfromParts);
			} else {
				SeqEntryExplore(sep, (Pointer) (&bs), StripBSfromTop);
			}
		}
		 ret = FixNucProtSet(sep);
		 retval |= ret;
		EntryChangeImpFeat(sep); 
		EntryChangeGBSource(sep); 
		SeqEntryExplore (sep, NULL, FixProtMolInfo);
		SeqEntryExplore (sep, NULL, FuseMolInfos);
		SeqEntryExplore(sep, NULL, StripProtXref);
		SeqEntryExplore(sep, (Pointer)(&qm), CheckMaps);
		if (qm.same == TRUE) {
			SeqEntryExplore(sep, (Pointer)(&qm), StripMaps);
		} else {
			SeqEntryExplore(sep, NULL, MapsToGenref);
		}
		CheckGeneticCode(sep);
		NormalizeSegSeqMolInfo (sep);
		toasn3_free(&ta);
		RestoreUpdateDatePos (sep, update_date_pos);
		if(qm.name != NULL)
		{
			MemFree(qm.name);
		}
		
		return retval;
	}
	if (ta.ofp == NULL) {
		ErrPostStr(SEV_WARNING, ERR_ORGANISM_NotFound, "No information found to create BioSource");
	}
	if (ta.mfp == NULL) {
		ErrPostStr(SEV_WARNING, ERR_ORGANISM_NotFound, "No information found to create MolInfo"); 
	}

	FixToAsn(sep, (Pointer)(&ta));

	if (ta.ofp != NULL) {
		ofp = ta.ofp;
		SeqEntryExplore(sep, (Pointer)ofp, FixOrg);
	}
	if (ta.mfp != NULL) {
		mfp = ta.mfp;
		SeqEntryExplore(sep, (Pointer)mfp, FixMol);
	}

/* entry  is converted to asn.1 spec 3.0, now do the checks */
	retval = INFO_ASNNEW;
	if(ta.had_biosource && strip_old) {
		SeqEntryExplore(sep, NULL, StripOld);
	}
	ToAsn4(sep);          /* move pubs and lineage */
	if (taxserver && taxfun != NULL) {
		SeqEntryExplore(sep, NULL, taxfun);
	}
	if (is_equiv(sep)) {
			/*do nothing*/
	} else if (NOT_segment(sep)) {
		if (taxserver && taxmerge != NULL) {
			SeqEntryExplore(sep, mult, taxmerge);
		} else {
			SeqEntryExplore(sep, mult, MergeBSinDescr);
		}
	} else {
		SeqEntryExplore(sep, (Pointer) (&bs), CheckBS);
		if (bs.same == TRUE) {
			SeqEntryExplore(sep, (Pointer) (&bs), StripBSfromParts);
		} else {
			SeqEntryExplore(sep, (Pointer) (&bs), StripBSfromTop);
		}
	}
	ret = FixNucProtSet(sep);
	retval |= ret;
	EntryChangeImpFeat(sep); 
	EntryChangeGBSource(sep); 
	SeqEntryExplore (sep, NULL, FixProtMolInfo);
	SeqEntryExplore (sep, NULL, FuseMolInfos);
	SeqEntryExplore(sep, NULL, StripProtXref);
	SeqEntryExplore(sep, (Pointer)(&qm), CheckMaps);
	if (qm.same == TRUE) {
		SeqEntryExplore(sep, (Pointer)(&qm), StripMaps);
	} else {
		SeqEntryExplore(sep, NULL, MapsToGenref);
	}
	CheckGeneticCode(sep);
	NormalizeSegSeqMolInfo (sep);
	toasn3_free(&ta);
	RestoreUpdateDatePos (sep, update_date_pos);
	if(qm.name)
		qm.name=MemFree(qm.name);
	return retval;
}

Boolean CheckLocWhole(BioseqPtr bsp, SeqLocPtr slp)
{
	Boolean whole = FALSE;
	SeqIntPtr sip;
	
	if (slp == NULL)
		return FALSE;
		
	if (slp->choice == SEQLOC_WHOLE) {
		return TRUE;
	} else if (slp->choice == SEQLOC_INT) {
		sip = slp->data.ptrvalue;
		if (sip->from == 0 && sip->to == bsp->length-1) {
			return TRUE;
		}
	}
	return FALSE;
}
/*****************************************************************************
*
*   Find all the OrgRefs
*
*****************************************************************************/
void FindOrg (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	ValNodePtr vnp, vnp0;
	BioseqPtr bsp;
	BioseqSetPtr bssp;
	OrgRefPtr orp;
	SeqAnnotPtr sap, ap;
	SeqFeatPtr sfp;
	ToAsn3Ptr tap;
	Uint1		mol = 0, meth = 0;
	ValNodePtr	mod = NULL, org;
	ImpFeatPtr	imp;
	Boolean		info = FALSE;
	Int4		len;
	Boolean 	whole = FALSE;
	GBQualPtr	q;

	tap = (ToAsn3Ptr)data;

	if (tap->had_biosource)
		return;

	if (IS_Bioseq(sep))
	{
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		vnp = bsp->descr;
		sap = bsp->annot;
		len = bsp->length;
	}
	else
	{
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		vnp = bssp->descr;
		sap = bssp->annot;
	}

	vnp0 = vnp;
	while (vnp != NULL) {
		if (vnp->choice == Seq_descr_org) {
			org = vnp;
			orp = (OrgRefPtr)(vnp->data.ptrvalue);
			AddOrgToFix(orp, tap, mod, sep, org, NULL, index);
		}
		if (vnp->choice == Seq_descr_mol_type) {
			mol = vnp->data.intvalue;
			if (mol != 0) {
				info = TRUE;
			}
		} else if (vnp->choice == Seq_descr_method) {
			meth = vnp->data.intvalue;
			if (meth != 0) {
				info = TRUE;
			}
		} else if (vnp->choice == Seq_descr_modif) {
			mod = vnp->data.ptrvalue;
			info = TRUE;
		} else if (vnp->choice == Seq_descr_source) { 
			tap->had_biosource = TRUE;
			return;
		}
		vnp = vnp->next;
	}
	if (info) {
		AddMolToFix(tap, sep, mol, mod, meth, index);
	}
	for (ap = sap; ap; ap = ap->next) {
		if (ap->type != 1) {  /* feature table */
			continue;
		}
		for (sfp = (SeqFeatPtr)(ap->data); sfp; sfp = sfp->next) {
			if (sfp->data.choice == SEQFEAT_ORG) {
				orp = (OrgRefPtr)(sfp->data.value.ptrvalue);
				if ((bsp = BioseqFind(SeqLocId(sfp->location))) != NULL) {
					whole = check_whole(sfp, bsp->length);
				}
				if (whole) {
					AddOrgToFix(orp, tap, NULL, sep, vnp0, NULL, index);
				} else {
					AddOrgToFix(orp, tap, NULL, sep, NULL, sfp, index);
				} 
			}
			if (sfp->data.choice == SEQFEAT_IMP) {
				imp = (ImpFeatPtr)(sfp->data.value.ptrvalue);
				if (StringCmp(imp->key, "source") == 0) {
					for(q = sfp->qual; q != NULL; q = q->next) {
						if (StringCmp(q->qual, "organism") == 0) {
							break;
						}
					} 
					if (q == NULL) {
						continue;
					}
					if ((bsp = BioseqFind(SeqLocId(sfp->location))) != NULL) {
						whole = check_whole(sfp, bsp->length);
					}
					if (whole) {
						AddImpToFix(sfp, tap, sep, vnp0, NULL, index);
					} else {
						AddImpToFix(sfp, tap, sep, NULL, sfp, index);
					} 
				}
			}
			whole = FALSE;
		}
	}
	return;
}

/***********************************************************************
*	0 	same organisms
*  -1	different organisms
************************************************************************/

Int4 BSComparison(BioSourcePtr one, BioSourcePtr two)
{
	OrgRefPtr orp1, orp2;
	OrgNamePtr onp1, onp2;
	OrgModPtr omp1, omp2;
	SubSourcePtr ssp1, ssp2;
	CharPtr name1 = NULL, name2 = NULL;
	CharPtr subname1 = NULL, subname2 = NULL;
	Int4		i, id1 = -1, id2 = -1, retval = -1;
	
	if (one == NULL || two == NULL)
		return -1;
	if ((orp1 = one->org) == NULL)
		return -1;
	if ((orp2 = two->org) == NULL)
		return -1;
	if ((name1 = orp1->taxname) == NULL)
		return -1;
	if (*name1 == '\0') {
		ErrPostStr(SEV_WARNING, ERR_ORGANISM_Empty, "empty organism in source feature");
		return -1;
	}
	if ((name2 = orp2->taxname) == NULL)
		return -1;
	if (*name2 == '\0') {
		ErrPostStr(SEV_WARNING, ERR_ORGANISM_Empty, "empty organism in source feature");
		return -1;
	}
/*  Strip organelle from organism */
	for (i = 0; i < num_organelle; i++) {
		if (StringNCmp(name1, organelle[i].name, 
				StringLen(organelle[i].name)) == 0) {
			name1 += StringLen(organelle[i].name);
		}
		if (StringNCmp(name2, organelle[i].name, 
				StringLen(organelle[i].name)) == 0) {
			name2 += StringLen(organelle[i].name);
		}
	}
	for (; name1 != NULL && *name1 == ' '; name1++) continue;
	for (; name2 != NULL && *name2 == ' '; name2++) continue;
	if (StringICmp(name2, name1) == 0) {
		retval = 0;
	} else {
			ErrPostEx(SEV_ERROR, ERR_ORGANISM_Diff,
	 "Different organisms in one entry: %s|%s", name2, name1);
	 	retval = -1;
	}
	
/* Compare clones - now all subsource and orgmod modifiers */

	for (ssp1 = one->subtype, ssp2 = two->subtype;
		ssp1 != NULL && ssp2 != NULL;
		ssp1 = ssp1->next, ssp2 = ssp2->next) {
		if (ssp1->subtype != ssp2->subtype) return -1;
		if (StringICmp (ssp1->name, ssp2->name) != 0) return -1;
	}
	if (ssp1 != NULL || ssp2 != NULL) return -1;

	onp1 = orp1->orgname;
	onp2 = orp2->orgname;
	if (onp1 == NULL || onp2 == NULL) return retval;

	for (omp1 = onp1->mod, omp2 = onp2->mod;
		omp1 != NULL && omp2 != NULL;
		omp1 = omp1->next, omp2 = omp2->next) {
		if (omp1->subtype != omp2->subtype) return -1;
		if (StringICmp (omp1->subname, omp2->subname) != 0) return -1;
	}
	if (omp1 != NULL || omp2 != NULL) return -1;

	return retval;
}

Int4 BSComparisonEx(BioSourcePtr one, BioSourcePtr two, Boolean clone)
{
	OrgRefPtr orp1, orp2;
	SubSourcePtr ssp1, ssp2;
	CharPtr name1 = NULL, name2 = NULL;
	CharPtr subname1 = NULL, subname2 = NULL;
	Int4		i, id1 = -1, id2 = -1, retval = -1;
	
	if (one == NULL || two == NULL)
		return -1;
	if ((orp1 = one->org) == NULL)
		return -1;
	if ((orp2 = two->org) == NULL)
		return -1;
	if ((name1 = orp1->taxname) == NULL)
		return -1;
	if (*name1 == '\0') {
		ErrPostStr(SEV_WARNING, ERR_ORGANISM_Empty, "empty organism in source feature");
		return -1;
	}
	if ((name2 = orp2->taxname) == NULL)
		return -1;
	if (*name2 == '\0') {
		ErrPostStr(SEV_WARNING, ERR_ORGANISM_Empty, "empty organism in source feature");
		return -1;
	}
/*  Strip organelle from organism */
	for (i = 0; i < num_organelle; i++) {
		if (StringNCmp(name1, organelle[i].name, 
				StringLen(organelle[i].name)) == 0) {
			name1 += StringLen(organelle[i].name);
		}
		if (StringNCmp(name2, organelle[i].name, 
				StringLen(organelle[i].name)) == 0) {
			name2 += StringLen(organelle[i].name);
		}
	}
	for (; name1 != NULL && *name1 == ' '; name1++) continue;
	for (; name2 != NULL && *name2 == ' '; name2++) continue;
	if (StringICmp(name2, name1) == 0) {
		retval = 0;
	} else {
			ErrPostEx(SEV_ERROR, ERR_ORGANISM_Diff,
	 "Different organisms in one entry: %s|%s", name2, name1);
	 	retval = -1;
	}
	
/* Compare clones */
	for (ssp1 = one->subtype; ssp1; ssp1= ssp1->next) {
		if (ssp1->subtype == 3) {  /* clone */
			subname1 = ssp1->name;
		}
	}
	for (ssp2 = two->subtype; ssp2; ssp2= ssp2->next) {
		if (ssp2->subtype == 3) {  /* clone */
			subname2 = ssp2->name;
		}
	}
	if (clone) {
		if (subname1 == NULL || subname2 == NULL) {
			return retval;
		}
	}
	if (StringCmp(subname1, subname2) != 0) {
		return -1;
	}
/* Compare notes (that are kludged to subtype 'other' */
	for (ssp1 = one->subtype; ssp1; ssp1= ssp1->next) {
		if (ssp1->subtype == 255) {  /* other */
			subname1 = ssp1->name;
		}
	}
	for (ssp2 = two->subtype; ssp2; ssp2= ssp2->next) {
		if (ssp2->subtype == 255) {  /* other */
			subname2 = ssp2->name;
		}
	}
	if (clone) {
		if (subname1 == NULL || subname2 == NULL) {
			return retval;
		}
	}
	if (StringCmp(subname1, subname2) != 0) {
		return -1;
	}
	return retval;
}

static CharPtr GetQualValue(GBQualPtr gbqual, CharPtr qual)
{
	GBQualPtr	q;
	CharPtr 	value;
	
		for(q = gbqual; q != NULL; q = q->next) {
			if (StringCmp(q->qual, qual) == 0) {
				value = q->val;
				break;
			}
		}
		return value;
}
	
/*		mapping from source feature qualifiers and comments */
void CheckQualsWithComm(BioSourcePtr bsp, SeqFeatPtr sfp)
{
	CharPtr	tmp;
	OrgModPtr	omp = NULL;
	OrgNamePtr	onp = NULL;
	OrgRefPtr	orp;

	if (bsp == NULL)
		return;
	if (sfp == NULL)
		return;
	if (bsp->org == NULL)
		return;
	CheckQuals(bsp, sfp->qual);
	if (sfp->comment != NULL) {
		tmp  = MemNew(StringLen(sfp->comment) +1); 
		StringCpy(tmp, sfp->comment);
		orp = (OrgRefPtr) bsp->org;
		onp = bsp->org->orgname;
		if (onp == NULL) {
			onp = OrgNameNew();
		}
		omp = OrgModNew();
		omp->subtype = 255;
		omp->subname = StringSave(tmp);
		onp->mod = tie_next_OrgMod(onp->mod, omp);
		MemFree(tmp);
	}
	if (onp != NULL) {
		bsp->org->orgname = onp;
	}
	return;
}

void CheckQuals(BioSourcePtr bsp, GBQualPtr qsfp)
{
	GBQualPtr 	q;
	static Char	msg[51];
	Int2		i;
	SubSourcePtr ssp;
	OrgModPtr	omp = NULL;
	OrgNamePtr	onp;
	OrgRefPtr	orp;
	
	
	if (bsp == NULL)
		return;
	if (bsp->org == NULL)
		return;
	orp = (OrgRefPtr) bsp->org;
	onp = bsp->org->orgname;
	if (onp)
		omp = onp->mod;
	for (q = qsfp; q != NULL; q=q->next) {
		if (StringCmp(q->qual, "organism") == 0) {
			continue;
		}

		if (StringCmp(q->qual, "note") == 0) {
			if (onp == NULL) {
				onp = OrgNameNew();
			}
			omp = OrgModNew();
			omp->subtype = 255;
			omp->subname = StringSave(q->val);
			onp->mod = tie_next_OrgMod(onp->mod, omp);
		}
		for (i = 0; i < num_bad_quals && q->qual != NULL; i++) {
			if (StringCmp(bad_quals[i], q->qual) == 0) {
			StringNCpy(msg, q->val, 50);
			ErrPostEx(SEV_WARNING, ERR_SOURCE_UnwantedQualifiers,
		 "Unwanted qualifier on source feature: %s=%s", q->qual, msg);
		 	continue;
			}
		}
		for (i = 0; i < num_genome && q->qual != NULL; i++) {
			if (StringCmp(genome[i], q->qual) == 0) {
				if (!bsp->genome) {
					bsp->genome = i;
					break;
				} else if (bsp->genome == 5 && i == 4) {
					bsp->genome = i;
					break;
				}
			}
		}
		for (i = 0; i < num_subtype && q->qual != NULL; i++) {
			if (StringCmp(subtype[i], q->qual) == 0) {
				ssp = SubSourceNew();
				ssp->subtype = (Uint1) (i+1);
				if (q->val == NULL) {
					ssp->name = StringSave("");
				} else {
					ssp->name = StringSave(q->val);
				}
				bsp->subtype = tie_next_subtype(bsp->subtype, ssp);
				break;
			}
		}	
		for (i=0; orgmod_subtype[i].name != NULL; i++) {
			if (StringCmp(q->qual, "organism") == 0) {
				continue;
			}
			if (StringCmp(q->qual, orgmod_subtype[i].name) == 0) {
				if (onp == NULL) {
					onp = OrgNameNew();
				}
/* *******************************************************************
We need to find the OrgName here. Now it's optional. Tatiana 10.21.94		
					onp->choice = 2;                    (virus)
					onp->data = Nlm_StringSave("proba");
******************************************************************* */		
				omp = OrgModNew();
				omp->subtype = (Uint1) orgmod_subtype[i].num;
				omp->subname = StringSave(q->val);
				onp->mod = tie_next_OrgMod(onp->mod, omp);
				break;
			}
		}
	}
	if (onp != NULL) {
		bsp->org->orgname = onp;
	}
	return;
}

MolInfoPtr new_info(MolInfoPtr mfi)
{
	return (mfi == NULL) ? MolInfoNew() : mfi;
}

/*****************************************************************************/
MolInfoPtr ModToMolInfo(MolInfoPtr mfi, Uint1 mod)
{
	
		switch(mod) {
			case 10:
				mfi = new_info(mfi);
				mfi->completeness = 2;
				break;
			case 11:
				mfi = new_info(mfi);
				mfi->completeness = 1;
				break;
			case 16:
				mfi = new_info(mfi);
				mfi->completeness = 3;
				break;
			case 17:
				mfi = new_info(mfi);
				mfi->completeness = 4;
				break;
			case 20:
				mfi = new_info(mfi);
				mfi->tech = 2;
				break;
			case 21:
				mfi = new_info(mfi);
				mfi->tech = 3;
				break;
			case 22:
				mfi = new_info(mfi);
				mfi->tech = 4;
				break;
			default:
				break;
		}
		return mfi;
}

/*****************************************************************************/
void ModToBiosource(BioSourcePtr bsp, Uint1 mod)
{
		switch(mod) {
			case 2:
				bsp->genome = 8;   /* extrachrom */
				break;
			case 3:
				bsp->genome = 9;	/*plasmid */
				break;
			case 4:
				bsp->genome = 5;	/* mitochondrion */
				break;
			case 5:
				bsp->genome = 2;	/* chloroplast */
				break;
			case 6:
				bsp->genome = 4;	/* kinetoplast */
				break;
			case 7:
				bsp->genome = 12;	/* cyanelle */
				break;
			case 8:
				bsp->origin = 5; 	/* synthetic */
				break;
			case 12:
				bsp->origin = 3;	/* mutagen */
				break;
			case 13:
				bsp->origin = 2;	/* natmut */
				break;
			case 14:
				bsp->genome = 10;	/*transposon */
				break;
			case 15:
				bsp->genome = 11;	/* insertion-seq */
				break;
			case 18:
				bsp->genome = 7;	/*macronuclear */
				break;
			case 19:
				bsp->genome = 13;	/* proviral*/
				break;
			case 23:
				bsp->genome = 3;	/* chromoplast */
				break;
			default:
				break;
		}
		return;
}

/*****************************************************************************
*
*  if no BioSource found on descr level and feature Biosource found
*	move it to the top
*	(stop using 05-09-96)
*****************************************************************************/
void CkOrg (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	ToAsn3Ptr	tap;
	ValNodePtr	vnp, tmp;
	SeqFeatPtr	sfp, tmp_sfp = NULL;
	SeqAnnotPtr	sap;
	BioseqPtr	bsp = NULL;
	BioseqSetPtr	bssp;
	
	tap = (ToAsn3Ptr)data;
	if (!tap->had_biosource)
		return;
		 
	if (IS_Bioseq(sep)) {
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		vnp = bsp->descr;
		sap = bsp->annot;
	} else {
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		vnp = bssp->descr;
		sap = bssp->annot;
	}
	
	for (tmp = vnp; tmp != NULL; tmp = tmp->next) {
		if (tmp->choice == Seq_descr_source) {
			break;
		}
	}
	if (tmp != NULL) {
		return;
	}
	if (sap == NULL || sap->type != 1) {
		return;
	}
	tmp_sfp = (SeqFeatPtr) (sap->data);
	sfp = SeqFeatExtractList(&(tmp_sfp), SEQFEAT_BIOSRC);
	if (sfp != NULL) {
		tmp = SeqDescrNew(vnp);
		tmp->choice = Seq_descr_source;
		tmp->data.ptrvalue = AsnIoMemCopy(sfp->data.value.ptrvalue, 
		(AsnReadFunc) BioSourceAsnRead, (AsnWriteFunc) BioSourceAsnWrite);
		SeqFeatFree(sfp);
	}
	sap->data = tmp_sfp;
	if (tmp_sfp == NULL) {
		if (IS_Bioseq(sep)) {
			bsp->annot = NULL;
		} else {
			bssp->annot = NULL;
		}
	}
}

/**************************************************************************
*	Compare BioSources in one bioseq->descr,
*	merge if organisms are the same or create a feature if different
*
**************************************************************************/
void MergeBSinDescr (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	BioseqPtr bsp = NULL;
	ValNodePtr vnp, v, vnext, mult;
	SeqAnnotPtr sap = NULL;
	SeqIdPtr		sip;
	SeqFeatPtr		sfp;
	BioSourcePtr bsrc = NULL, bs;

	if (!IS_Bioseq(sep)) {
		return;
	}
	mult = (ValNodePtr) data;
	bsp = (BioseqPtr) sep->data.ptrvalue;
	if ((bsp->repr != Seq_repr_raw) && (bsp->repr != Seq_repr_const) 
			&& (bsp->repr != Seq_repr_delta))
		return;

	if (! ISA_na(bsp->mol))
		return;
	
	sap = bsp->annot;
	bsp->descr = tie_next(bsp->descr, mult);
	for (vnp = bsp->descr; vnp; vnp= vnp->next) {
		if (vnp->choice == Seq_descr_source) {
			bsrc = vnp->data.ptrvalue;
			break;
		}
	}
	if (bsrc == NULL || bsrc->org == NULL) {
		return;
	}
	for (v = vnp->next; v; v = vnext) {
		vnext = v->next;
		if (v->choice != Seq_descr_source) {
			continue;
		}
		bs = v->data.ptrvalue;
		if (bs->org != NULL) {
			if (bsrc && CmpOrgById(bsrc, bs) == TRUE) {
				bsrc = BioSourceMerge(bsrc, bs);
			} else {
				sfp = SeqFeatNew();
			    sfp->location = ValNodeNew(NULL);
			    sfp->location->choice = SEQLOC_WHOLE;
				sip = SeqIdDup(bsp->id);
			    sfp->location->data.ptrvalue = sip ;
				sfp->data.choice = SEQFEAT_BIOSRC;
				sfp->data.value.ptrvalue = 
					AsnIoMemCopy(bs, (AsnReadFunc) BioSourceAsnRead, 
									   (AsnWriteFunc) BioSourceAsnWrite);
				   if (sap == NULL) {
					   sap = SeqAnnotNew();
					   sap->type = 1;
					   bsp->annot = sap;
				   }
				   sap->data = tie_feat(sap->data, sfp); 
			}
		} else {
			ErrPostStr(SEV_WARNING, ERR_ORGANISM_Empty, "Biosource missing Organism info");
		}
		BioSourceFree(bs);
		vnp->next = remove_node(vnp->next, v);
		
	}
	return;
}
/********************************************************************************	Move Biosource to nuc-prot set level
*******************************************************************************/
Int4 FixNucProtSet(SeqEntryPtr sep)
{
	BioseqSetPtr bssp, bseg;
	BioseqPtr bsp = NULL, prot = NULL;
	ValNodePtr descr = NULL;
	ValNodePtr tmp, vnp, v, vnext;
	BioSourcePtr bsrc = NULL, bs;
	SeqEntryPtr seqsep, s;
	SeqAnnotPtr sap = NULL;
	SeqIdPtr		sip;
	SeqFeatPtr		sfp;
	Int4 			retval = INFO_ASNOLD;
	Boolean			bSingle = FALSE;
	
	
	if (IS_Bioseq(sep)) {
		return retval;
	}
	bssp = (BioseqSetPtr)(sep->data.ptrvalue);
	if (bssp->_class != 1) {    /*  do the rest for nuc-prot only */
		return retval;
	}
	seqsep = bssp->seq_set;
	if (seqsep == NULL) {
		return retval;
	}
	if (seqsep->choice == 1) {   /* single bioseq */
		bsp = (BioseqPtr) seqsep->data.ptrvalue;
		descr = bsp->descr;
		sap = bsp->annot;
		bSingle = TRUE;
	} else if (seqsep->choice == 2) { /* segmented set */
		bseg = (BioseqSetPtr) seqsep->data.ptrvalue;
/*   quick fix of core dump in segmented sets with multiple organisms 
	BIOSOURCE feature is created on main segmeted bioseq (not parts) !*/
		s = bseg->seq_set;
		bsp = (BioseqPtr) s->data.ptrvalue;
		descr = bseg->descr;
		sap = bseg->annot;
	}
	if (descr == NULL) {
		return retval;		/* nothing to move */
	}
	for (vnp = bssp->descr; vnp; vnp = vnp->next) {  /* nucprot set level */
		if (vnp->choice == Seq_descr_source) {
			bsrc = vnp->data.ptrvalue;
			break;
		}
	}
	for (v = descr; v; v = vnext) {			/* from bioseq or BioseqSet */
		vnext = v->next;
		if (v->choice != Seq_descr_source) {
			continue;
		}
		bs = v->data.ptrvalue;
		if (bsrc == NULL) {
			bsrc = BioSourceMerge(bsrc, bs);
			tmp = SeqDescrAdd(&(bssp->descr));
			tmp->choice = Seq_descr_source;
			tmp->data.ptrvalue = bsrc;
		} else if (CmpOrgById(bsrc, bs) == TRUE) {
			bsrc = BioSourceMerge(bsrc, bs);
		} else {
			sfp = SeqFeatNew();
	    	sfp->location = ValNodeNew(NULL);
	    	sfp->location->choice = SEQLOC_WHOLE;
			sip = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (bsp->id, 0)));
	    	sfp->location->data.ptrvalue = sip;
			sfp->data.choice = SEQFEAT_BIOSRC;
			sfp->data.value.ptrvalue = 
				AsnIoMemCopy(bs, (AsnReadFunc) BioSourceAsnRead, 
		   						(AsnWriteFunc) BioSourceAsnWrite);
		   	if (sap == NULL) {
		   		sap = SeqAnnotNew();
		   		sap->type = 1;
		   	}
		   	sap->data = tie_feat(sap->data, sfp);
		}
		BioSourceFree(bs);
		descr = remove_node(descr, v);
		
	}
/* remove Biosource from protein sequence if it's there 
	merging BioSource with the one on the top level*/
	for (s = seqsep->next; s; s = s->next) {
		prot = s->data.ptrvalue;
		vnp = ValNodeExtractList(&prot->descr, Seq_descr_source);
		if (vnp != NULL) {
			bs = vnp->data.ptrvalue;
			if (bsrc == NULL) {
				bsrc = BioSourceMerge(bsrc, bs);
				tmp = SeqDescrNew(bssp->descr);
				tmp->choice = Seq_descr_source;
				tmp->data.ptrvalue = bsrc;
			} else if (CmpOrgById(bsrc, bs) == TRUE) {
				bsrc = BioSourceMerge(bsrc, bs);
		 	} else {
		 		ErrPostStr(SEV_ERROR, ERR_ORGANISM_Diff, 
		 		"ATTENTION: different organisms in nuc-prot set");
		 		retval = ERR_REJECT;
		 	}
		 	if (retval == ERR_REJECT) {
		 		prot->descr = ValNodeLink(&prot->descr, vnp);
		 	} else {
		 		BioSourceFree(bs);
		 		ValNodeFree(vnp);
		 	}
		}
	}
	if (bSingle) {
		bsp->descr = descr;
		bsp->annot = sap;
	} else {
		bseg->descr = descr;
		bseg->annot = sap;
	}
	return retval;	
}
		
/*****************************************************************************
*	check BioSource descr for the parts of segmented set,
*	if organisms  are the same and no "clone" Biosources are the same
*	they would be deleted from parts in the next SeqEntryExplore 
*	if different BioSource from the top would be deleted      
*****************************************************************************/
void CheckBS (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	BioseqPtr	bsp = NULL;
	BioseqSetPtr	bssp, tmp;
	SeqEntryPtr	 	segsep, parts;
	BSMapPtr		bmp;
	
	bmp = data;
	if (bmp->same == FALSE) {
		return;
	}
	if (IS_Bioseq(sep)) {
		return;
	}
	bssp = (BioseqSetPtr)(sep->data.ptrvalue);
	if (bssp->_class != 2) {    /*  do the rest for segset only */
		if (bssp->_class >= 7) {
			bmp->same = FALSE;   /* for other sets organisms are different */
		}
		return;
	}
	segsep = bssp->seq_set;
	if (segsep->next == NULL) {
		return;
	}
	if (!IS_Bioseq(segsep->next)) {
		tmp = (BioseqSetPtr) (segsep->next->data.ptrvalue); /*segsep->next=parts*/
		parts = tmp->seq_set;
		if (parts == NULL) {
			return;
		}
		bmp->same = CheckSegDescrChoice(parts, Seq_descr_source);
	}
	
}

Int2 seq_loc_compare( SeqLocPtr a, SeqLocPtr b)
{
	Int2 retval = -1;
	Int4 a_strt, a_stop, b_strt, b_stop;
	SeqIdPtr	a_sip, b_sip;
	
	retval = SeqLocCompare(a, b);
	if (retval > 0) {
		return retval;
	}
	a_sip = SeqLocId(a);
	b_sip = SeqLocId(b);
	if (SeqIdForSameBioseq(a_sip, b_sip)) {
		a_strt = SeqLocStart(a);
		a_stop = SeqLocStop(a);
		b_strt = SeqLocStart(b);
		b_stop = SeqLocStop(b);
		if (a_stop+1 == b_strt || b_stop+1 == a_strt)
			retval = 5;
	}
	return retval;
}

void compare_quals(GBQualPtr PNTR qual1, GBQualPtr PNTR qual2)
{
	GBQualPtr q1, q2, q2next;
	
	for (q1 = *qual1; q1 != NULL; q1 = q1->next) {
		for (q2 = *qual2; q2 != NULL; q2 = q2next) {
			q2next = q2->next;
			if ((StringCmp(q1->qual, q2->qual) == 0) && 
				(StringCmp(q1->val, q2->val) == 0)) {
				*qual2 = remove_qual(*qual2, q2);
			}
		}
	}
}

Boolean feat_join(SeqFeatPtr f1, SeqFeatPtr f2, SeqFeatPtr head)
{
	Boolean new = FALSE;
	Int2	comp;
	Int4 a_strt, a_stop, b_strt, b_stop, a, b;
	GBQualPtr q1, q2, fq, q1next, q2next;
	SeqFeatPtr f;
	ImpFeatPtr imp;
	SeqLocPtr slp;
	SeqIntPtr sip, f1_sip;
	Boolean nmatch = FALSE;
	
	comp = seq_loc_compare(f1->location, f2->location);
	switch (comp) 
	{
		case 0:
			break;
		case 1:
			for (q2 = f2->qual; q2 != NULL; q2 = q2->next) {
				for (q1 = f1->qual; q1 != NULL; q1 = q1next) {
					q1next = q1->next;
					if ((StringCmp(q1->qual, q2->qual) == 0) && 
						(StringCmp(q1->val, q2->val) == 0)) {
						f1->qual = remove_qual(f1->qual, q1);
					}
				}
			}
			break;
		case 3:
			for (q1 = f1->qual; q1 != NULL; q1 = q1->next) {
				for (q2 = f2->qual; q2 != NULL; q2 = q2next) {
					q2next = q2->next;
					if ((StringCmp(q1->qual, q2->qual) == 0) && 
						(StringCmp(q1->val, q2->val) == 0)) {
						continue;
					} else {
						nmatch = TRUE;
						break;
					}
				}
			}
			if (nmatch) {
				GBQualFree(f2->qual);
			ErrPostStr(SEV_WARNING, ERR_SOURCE_DiffQualifiers,
		 "Identical source features with unmatching qualifiers");
			} else {
				GBQualFree(f2->qual);
			ErrPostStr(SEV_WARNING, ERR_SOURCE_Identical,
		 "Identical source features: one is removed");
			}
		case 2:
			for (q1 = f1->qual; q1 != NULL; q1 = q1->next) {
				for (q2 = f2->qual; q2 != NULL; q2 = q2next) {
					q2next = q2->next;
					if ((StringCmp(q1->qual, q2->qual) == 0) && 
						(StringCmp(q1->val, q2->val) == 0)) {
						f2->qual = remove_qual(f2->qual, q2);
					}
				}
			}
			break;
		case 4:
		case 5:
			a_strt = SeqLocStart(f1->location);
			a_stop = SeqLocStop(f1->location);
			b_strt = SeqLocStart(f2->location);
			b_stop = SeqLocStop(f2->location);
			a = a_strt;
			if (b_strt < a_strt)
				a = b_strt;
			b = a_stop;
			if (b_stop > a_stop)
				b = b_stop;
			f = SeqFeatNew();
			imp = ImpFeatNew();
			imp->key = StringSave("source");
			slp = ValNodeNew(NULL);
			slp->choice = SEQLOC_INT;
			sip = SeqIntNew();
			f1_sip = (SeqIntPtr) (f1->location)->data.ptrvalue;
			sip->id = SeqIdDup(f1_sip->id);
			sip->from = a;
			sip->to = b;
			slp->data.ptrvalue = sip;
			f->location = slp;
			for (q1=f1->qual; q1 != NULL; q1 = q1next) {
				q1next = q1->next;
				for (q2=f2->qual; q2 != NULL; q2 = q2next) {
					q2next = q2->next;
					if ((StringCmp(q1->qual, q2->qual) == 0) && 
						(StringCmp(q1->val, q2->val) == 0)) {
						fq = GBQualNew();
						fq->qual = q1->qual;
						q1->qual = NULL;
						fq->val = q1->val;
						q1->val = NULL;
						f->qual = fq;
						f1->qual = remove_qual(f2->qual, q2); 
						f2->qual = remove_qual(f1->qual, q1);
					}
				}
			}
			head = tie_feat(head, f);
			new = TRUE;
			break;
		default:
			break;
	}
	return new;
	
}
void count_join(SeqFeatPtr f1, SeqFeatPtr f2)
{
	Int2	comp, nq1, nq2;
	GBQualPtr q1, q2;
	Boolean nmatch = FALSE;
	static Char msg1[51], msg2[51];
	
	comp = seq_loc_compare(f1->location, f2->location);
	if (comp != 3) {
		return;
	}
	StringNCpy(msg1, SeqLocPrint(f1->location), 50);
	StringNCpy(msg2, SeqLocPrint(f2->location), 50);
	for (q1 = f1->qual, nq1 = 0; q1 != NULL; q1 = q1->next, nq1++) {
	}
	for (q2 = f2->qual, nq2 = 0; q2 != NULL; q2 = q2->next, nq2++) {
	}
	if (nq1 != nq2) {
		ErrPostEx(SEV_WARNING, ERR_SOURCE_DiffQualifiers,
	 "Identical source features with unmatching number of qualifiers %s|%s", 
			 msg1, msg2);
		 return;
	}
	for (q1 = f1->qual, nq1 = 0; q1 != NULL; q1 = q1->next, nq1++) {
		for (q2 = f2->qual; q2 != NULL; q2 = q2->next) {
			if ((StringCmp(q1->qual, q2->qual) == 0) && 
				(StringCmp(q1->val, q2->val) == 0)) {
				break;
			} 
		}
		if (q2 == NULL) {
			ErrPostEx(SEV_WARNING, ERR_SOURCE_DiffQualifiers,
			"Identical source features with unmatching qualifiers %s|%s",
				msg1, msg2);
		}
	}
	ErrPostEx(SEV_WARNING, ERR_SOURCE_Identical, "Identical source features; %s|%s",
		msg1, msg2);
}

static void FindWholeBSFeat (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	BioseqPtr	bsp = NULL;
	SeqFeatPtr  sfp, fp;
	SeqAnnotPtr ap;
	WholeFeatPtr wfp;
	Boolean is_na = FALSE;

	wfp = (WholeFeatPtr) data;
	if (IS_Bioseq(sep)) {
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		if (bsp->mol != Seq_mol_aa) {
			is_na = TRUE;
		}
	} 
	if (bsp == NULL || !is_na) {
		return;
	}
	for (ap = bsp->annot; ap; ap = ap->next) {
		if (ap->type != 1) {
			continue;
		}
		for (sfp = ap->data; sfp != NULL; sfp=sfp->next) {
			if (sfp->data.choice != SEQFEAT_BIOSRC) {
				continue;
			}
			if (check_whole(sfp, bsp->length) == FALSE) {
				continue; 
			} else {
				wfp->count++;
				fp = SeqFeatNew();
				fp->data.choice = sfp->data.choice;
				fp->data.value.ptrvalue = sfp->data.value.ptrvalue;
				wfp->sfp = tie_feat(wfp->sfp, fp);
			}
		}
	}
}
static void MergeWholeBSFeat (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	BioseqPtr	bsp = NULL;
	ValNodePtr vnp;
	SeqAnnotPtr ap;
	SeqFeatPtr  sfp;
	WholeFeatPtr wfp;
	Boolean remove = FALSE;
	Boolean is_na = FALSE;
	BioSourcePtr dbio = NULL, fbio = NULL;

	wfp = (WholeFeatPtr) data;
	if (IS_Bioseq(sep)) {
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		if (bsp->mol != Seq_mol_aa) {
			is_na = TRUE;
		}
	} 
	if (bsp == NULL || !is_na) {
		return;
	}
	for (vnp = bsp->descr; vnp != NULL; vnp = vnp->next) {
		if (vnp->choice != Seq_descr_source) {
			continue;
		}
		dbio = (BioSourcePtr) vnp->data.ptrvalue;
		if (wfp->sfp != NULL) {
			fbio = (BioSourcePtr) wfp->sfp->data.value.ptrvalue;
		}
		if (dbio) {
			if (fbio && CmpOrgById(dbio, fbio) == TRUE) {
				if (BSComparisonEx(dbio, fbio, TRUE) == 0) {
					BioSourceMerge(dbio, fbio);
					remove = TRUE;
				}
			}
		}
	}
	if (remove == FALSE) {
		return;
	}
	for (ap = bsp->annot; ap; ap = ap->next) {
		if (ap->type != 1) {
			continue;
		}
		for (sfp = ap->data; sfp != NULL; sfp=sfp->next) {
			if (sfp->data.choice != SEQFEAT_BIOSRC) {
				continue;
			}
			ap->data = remove_feat(ap->data, sfp);
			break;
		}
	}
}
void CombineBSFeat(SeqEntryPtr sep)
{
	WholeFeatPtr wfp;
	
	wfp = WholeFeatNew();
	SeqEntryExplore(sep, (Pointer)wfp, FindWholeBSFeat);
	if (wfp->count == 1) {
		SeqEntryExplore(sep, (Pointer)wfp, MergeWholeBSFeat);
	}
	WholeFeatFree(wfp);
}

/*****************************************************************************
*
*  Count multiple source features print out error messages
*
*****************************************************************************/
void CountSourceFeat (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	Boolean		whole = FALSE;
	Int2		count=0;
	Int4		len;
	ValNodePtr	vnp;
	SeqFeatPtr	sfp = NULL, f, ff;
	SeqAnnotPtr	sap, ap;
	BioseqPtr	bsp = NULL;
	ImpFeatPtr	imp;
	CharPtr		f_org, ff_org;
	GBQualPtr	q;
	SeqIdPtr	sidp;
	TextSeqIdPtr	tsip = NULL;
	Pointer pnt;
	Boolean PNTR pultiple;
	Boolean 	is_na = FALSE;
	
	pultiple = (Boolean PNTR) data;
	if (IS_Bioseq(sep)) {
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		vnp = bsp->descr;
		sap = bsp->annot;
		len = bsp->length;
		if (bsp->mol != Seq_mol_aa) {
			is_na = TRUE;
		}
	} 
	if (bsp == NULL || !is_na) {
		return;
	}
	for (sidp = bsp->id; sidp != NULL; sidp = sidp->next) {
    	pnt = sidp->data.ptrvalue;
    	switch (sidp->choice) 
    	{
	    	case SEQID_LOCAL:      /* local */
	    	case SEQID_GIBBSQ:      /* gibbseq */
	    	case SEQID_GIBBMT:      /* gibbmt */
	    	case SEQID_GIIM:      /* giimid */
	    	case SEQID_PATENT:      /* patent seq id */
	    	case SEQID_GENERAL:     /* general */
	    	case SEQID_GI:     /* gi */
		    case SEQID_PDB:
			continue;
	    	case SEQID_GENBANK:      /* genbank */
	    	case SEQID_EMBL:      /* embl */
	    	case SEQID_PIR:      /* pir   */
	    	case SEQID_SWISSPROT:      /* swissprot */
	    	case SEQID_OTHER:     /* other */
	    	case SEQID_DDBJ:
		    case SEQID_PRF:
			     tsip = (TextSeqIdPtr) sidp->data.ptrvalue;
		    	break;
	    	default:
		    	continue;
    	}
		if (tsip != NULL) {
    		flat2asn_install_accession_user_string(tsip->accession);
    		flat2asn_install_locus_user_string(tsip->name);
    		break;
    	}
	}
	if (tsip == NULL) {
    	flat2asn_install_accession_user_string("SET_UP");
    	flat2asn_install_locus_user_string("SET_UP");
    }
	for (ap = sap; ap; ap = ap->next) {
		if (sap->type != 1) {
			continue;
		}
		for (f = ap->data; f != NULL; f=f->next) {
			if (f->data.choice == SEQFEAT_IMP) {
				imp = (ImpFeatPtr) f->data.value.ptrvalue;
				if (imp != NULL && StringCmp(imp->key, "source") == 0) {
					for(q = f->qual; q != NULL; q = q->next) {
						if (StringCmp(q->qual, "organism") == 0) {
							break;
						}
					} 
					if (q == NULL) {
		ErrPostStr(SEV_WARNING, ERR_SOURCE_MissingOrganism, "Missing /organism in 'source' feature");
					}
					count++;
					whole = check_whole(f, len);	
				}
			}
		}
	}
	if (count == 0) {
			ErrPostStr(SEV_WARNING, ERR_SOURCE_NotFound, "NO SOURCE feature"); 
			*pultiple = TRUE;
	} else if (count == 1) {
		if (!whole) {
			ErrPostStr(SEV_WARNING, ERR_SOURCE_NotFoundWHole, "one NOT_WHOLE SOURCE feature");
			*pultiple = TRUE;
		}
	} else if (count > 1) {
/* check for /transposon and /insertion_seq and /clone*/
		if (true_multiple(sap, len)) {		
			ErrPostStr(SEV_WARNING, ERR_SOURCE_Multiple, "MULTIPLE SOURCE features");
			*pultiple = TRUE;
		}
		for (ap = sap; ap; ap = ap->next) {
			if (sap->type != 1) {
				continue;
			}
			for (f = ap->data; f != NULL; f = f->next) {
				if (f->qual == NULL)
					continue;
				f_org = NULL;
				for(q = f->qual; q != NULL; q = q->next) {
					if (StringCmp(q->qual, "organism") == 0) {
						f_org = q->val;
						break;
					}
				}
				for (ff = f->next; ff != NULL; ff = ff->next) {
					if (ff->qual == NULL)
						continue;
					ff_org = NULL;
					for(q = ff->qual; q != NULL; q = q->next) {
						if (StringCmp(q->qual, "organism") == 0) {
							ff_org = q->val;
							break;
						}
					}
					if (f_org && ff_org) {
						if (StringCmp(f_org, ff_org) != 0) {
			ErrPostEx(SEV_WARNING, ERR_SOURCE_Diff, "Different SOURCE features: %s|%s",
							f_org, ff_org);
						}
						count_join(f, ff);
					}
				}
			}
		}
	}
	
}
/*****************************************************************************
*
*  Check multiple source features and try to correct them
*
*****************************************************************************/
void CorrectSourceFeat (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	Boolean		whole = FALSE, new;
	Int2		count=0;
	Int4		len;
	ValNodePtr	vnp0, vnp;
	SeqFeatPtr	sfp = NULL, tmp_sfp, f, ff, fnext;
	SeqAnnotPtr	sap;
	BioseqPtr	bsp = NULL;
	BioseqSetPtr	bssp;
	ImpFeatPtr	imp;
	OrgRefPtr	orp;
	CharPtr		name, org_name, f_org, ff_org;
	GBQualPtr	q;
	SeqLocPtr 	slp;
	static Char		msg[51];
	CharPtr PNTR pporg;
	Boolean 	is_na = FALSE;
	
	pporg = (CharPtr PNTR) data;
	if (IS_Bioseq(sep))
	{
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		vnp = bsp->descr;
		sap = bsp->annot;
		len = bsp->length;
		if (bsp->mol != Seq_mol_aa) {
			is_na = TRUE;
		}
	}
	else
	{
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		vnp = bssp->descr;
		sap = bssp->annot;
	}
	
	for (vnp0 = vnp; vnp != NULL; vnp = vnp->next) {
		if (vnp->choice == Seq_descr_org) {
			if ((orp = vnp->data.ptrvalue) != NULL) {
				if (*pporg == NULL) {
					*pporg = orp->taxname;
				}
			}
		}
	}
	org_name = *pporg;
	if (bsp == NULL || !is_na) {
		return;
	}
	if (sap != NULL && sap->type == 1) {
		tmp_sfp = (SeqFeatPtr) (sap->data);
		sfp = ExtractSourceFeatList(&(tmp_sfp));
	}
	for (f = sfp; f != NULL; f=f->next) {
		count++;
		if (f->data.choice != SEQFEAT_IMP) {
			continue;
		}
		imp = (ImpFeatPtr) f->data.value.ptrvalue;
		if (imp != NULL) {
			whole = check_whole(sfp, len);
			if (whole) {
				for(q = f->qual; q != NULL; q = q->next) {
					if (StringCmp(q->qual, "organism") == 0) {
						name = MemNew(StringLen(q->val)+1);
						StringCpy(name, q->val);
					}
				}	
			}
		}
	}
	if (!whole) {
		if (count == 0) {
			ErrPostStr(SEV_WARNING, ERR_SOURCE_NotFound, "NO ORGANISM feature! Create one");
			sfp = SeqFeatNew();
			imp = ImpFeatNew();
			sfp->data.choice = SEQFEAT_IMP;
			sfp->data.value.ptrvalue = imp;
			imp->key = StringSave("source");
			q = GBQualNew();
			q->qual = StringSave("organism");
			if (org_name) {
				q->val = StringSave(org_name);
			} else {
				q->val = StringSave("unknown");
			}
			sfp->qual = q;
			slp = ValNodeNew(NULL);
			slp->choice = SEQLOC_WHOLE;
			slp->data.ptrvalue = (SeqLocPtr) SeqIdDup(bsp->id);
			sfp->location = slp;
			
		} else if (count == 1) {
			StringNCpy(msg, SeqLocPrint(sfp->location), 50);
			ErrPostEx(SEV_WARNING, ERR_SOURCE_NotFoundWHole, 
			"Convert source feature %s to whole", msg);
			slp = ValNodeNew(NULL);
			slp->choice = SEQLOC_WHOLE;
			slp->data.ptrvalue = (SeqLocPtr) SeqIdDup(bsp->id);
			sfp->location = slp;
			
		} 

	}
  if (count > 1) {
	do {
		for (f = sfp; f != NULL; f = f->next) {
			if (f->qual == NULL)
				continue;
			for(q = f->qual; q != NULL; q = q->next) {
				if (StringCmp(q->qual, "organism") == 0) {
					f_org = q->val;
					break;
				}
			}
			for (ff = f->next; ff != NULL; ff = ff->next) {
				if (ff->qual == NULL)
					continue;
				for(q = ff->qual; q != NULL; q = q->next) {
					if (StringCmp(q->qual, "organism") == 0) {
						ff_org = q->val;
						break;
					}
				}
				if (StringCmp(f_org, ff_org) != 0)
					continue;
				new = feat_join(f, ff, sfp);
			}
		}
	} while (new);
  }
	for (f = sfp; f != NULL; f = fnext) {
		fnext = f->next;
		if (f->qual == NULL) {
			sfp = remove_feat(sfp, f);
		}
	}
	tmp_sfp = tie_feat(tmp_sfp, sfp);
	if (sap) {
		sap->data = tmp_sfp;
		if (bsp)
		{
			bsp = (BioseqPtr)(sep->data.ptrvalue);
			bsp->descr = vnp0;
			if (tmp_sfp == NULL) {
				bsp->annot = NULL;
			}
		}
		else
		{
			bssp->descr = vnp0;
			if (tmp_sfp == NULL) {
				bssp->annot = NULL;
			}
		}
	}
}

Int2 BioSourceToGeneticCode (BioSourcePtr biop)
{
  OrgNamePtr  onp;
  OrgRefPtr   orp;

  if (biop != NULL) {
    orp = biop->org;
    if (orp != NULL) {
      onp = orp->orgname;
      if (onp != NULL) {
        if (biop->genome == 4 || biop->genome == 5) {
          return onp->mgcode;
        } else if (biop->genome == GENOME_chloroplast ||
                   biop->genome == GENOME_chromoplast ||
                   biop->genome == GENOME_plastid ||
                   biop->genome == GENOME_cyanelle ||
                   biop->genome == GENOME_apicoplast ||
                   biop->genome == GENOME_leucoplast ||
                   biop->genome == GENOME_proplastid) {
          return 11;
        } else {
          return onp->gcode;
        }
      }
    }
  }
  return 0;
}

static void GetTopBiop (SeqDescrPtr sdp, Pointer userdata)

{
  BioSourcePtr PNTR  biopp;

  if (sdp == NULL || sdp->choice != Seq_descr_source) return;
  biopp = (BioSourcePtr PNTR) userdata;
  if (biopp == NULL) return;
  if (*biopp != NULL) return;
  *biopp = (BioSourcePtr) sdp->data.ptrvalue;
}

static BioSourcePtr GetTopBioSourceFromSep (SeqEntryPtr sep)

{
  BioSourcePtr  biop = NULL;

  VisitDescriptorsInSep (sep, (Pointer) &biop, GetTopBiop);
  return biop;
}

static Boolean get_src (GatherContextPtr gcp)
{
	ValNodePtr	vnp, new;
	ValNodePtr	PNTR vnpp;
	
	vnpp = gcp->userdata;
	switch (gcp->thistype)
	{
		case OBJ_SEQDESC:
			vnp = (ValNodePtr) (gcp->thisitem);
			if (vnp->choice == Seq_descr_source) {
				if (vnp->data.ptrvalue != NULL) {
					new = SeqDescrNew(NULL);
					new = MemCopy(new, vnp, sizeof(ValNode));
					new->next = NULL;
					*vnpp = new;
					return FALSE;  /*only top level BioSource will be returned*/
				}
			} 
			break;
		default:
			break;
	}
	return TRUE;
}

static void FixPIDDbtag(ValNodePtr PNTR vnpp)
{
	ValNodePtr 		vnp;
	DbtagPtr 		db;
	Char 			val[166];
	
		for (vnp = *vnpp; vnp; vnp = vnp->next) {
			if (vnp->choice != SEQID_GENERAL) {
				continue;
			}
			db = vnp->data.ptrvalue;
			if (db == NULL) {
				continue;
			}
			if (StringNCmp(db->db, "PIDe", 4) == 0) {
					MemFree(db->db);
					db->db = StringSave("PID");
					sprintf(val, "e%ld", (long) db->tag->id);
					db->tag->str = StringSave(val);
					db->tag->id = 0;
					vnp->data.ptrvalue = db;
			} else if(StringNCmp(db->db, "PIDd", 4) == 0) {
					MemFree(db->db);
					db->db = StringSave("PID");
					sprintf(val, "d%ld", (long) db->tag->id);
					db->tag->str = StringSave(val);
					db->tag->id = 0;
					vnp->data.ptrvalue = db;
			}
		}
}

static CharPtr GetProduct(ValNodePtr product, ValNodePtr location)
{
	CharPtr protein_seq=NULL, start_ptr=NULL;
	Int4 length;
	SeqPortPtr spp;
	Uint1 residue, code;
	BioseqPtr bsp;
	SeqIdPtr sip;

	if (product == NULL) 
		return NULL;
	sip = SeqLocId(product);
	bsp = BioseqFindCore(sip);
	if (bsp == NULL)   /* Bioseq is (or has been) in memory */
		return NULL;
	code = Seq_code_ncbieaa;
	length = SeqLocLen(product);
	if (length <= 0) {
		return NULL;
	}
	start_ptr = protein_seq = 
		(CharPtr) MemNew((size_t) (length*sizeof(CharPtr)));
	spp = SeqPortNewByLoc(product, code);
	spp->do_virtual = TRUE;
	while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF) {
		if ( !IS_residue(residue) && residue != INVALID_RESIDUE )
			continue;
		if (residue == INVALID_RESIDUE) 
			residue = (Uint1) 'X';
		*protein_seq = residue;
		protein_seq++;
	}
		SeqPortFree(spp);
	return start_ptr;
}

static CharPtr stripStr(CharPtr base, CharPtr str)
{
	CharPtr bptr, eptr;
	
	bptr = StringStr(base, str);
	if (bptr != NULL) {
		eptr = bptr + StringLen(str);
		StringCpy(bptr, eptr);
	}
	
	return base;
}

static CharPtr  space_save(CharPtr str) 
/* deletes spaces from the begining and the end and returns Nlm_StringSave */		           {
	CharPtr s, ss;

	if (str == NULL) {
		return NULL;
	}
	for (; isspace(*str) || *str == ','; str++) continue;
	for (s = str; *s != '\0'; s++) {
		if (*s == '\n') {
			for (ss = s+1; isspace(*ss); ss++) continue;
			*s = ' ';
			strcpy(s+1, ss);
		}
	}
	for (s=str+StringLen(str)-1; s >= str && (*s == ' ' || *s == ';' ||
		 *s == ',' || *s == '\"' || *s == '\t'); s--) {
		*s = '\0';
	}  

	if (*str == '\0') { 
	    return NULL;
	} else {
	    return Nlm_StringSave(str);
	}
}

static SeqFeatPtr StripCDSComment(SeqFeatPtr sfp)
{
    CharPtr	strA = "Author-given protein sequence is in conflict with the conceptual translation.";
    CharPtr strC = "Method: conceptual translation supplied by author.";
    CharPtr	pchComment, comment, eptr;

    pchComment = sfp->comment;
	if (pchComment == NULL)
		return sfp;
	pchComment = stripStr(pchComment, strA);
	pchComment = stripStr(pchComment, strC);

    comment = space_save(pchComment);
    if (comment) {
    	eptr = comment+StringLen(comment) -1;
    	if (*eptr == ';') {
    		*eptr = '\0';
    	}
    }
    MemFree(sfp->comment);
    sfp->comment = comment;
    return sfp;
}

static Boolean CompareTranslation(ByteStorePtr bsp, CharPtr qval)
{
	CharPtr		 		ptr;
	Int2			 residue, residue1, residue2;
	Int4			 len, blen;
	Boolean		 done;

	if(qval == NULL || *qval == '\0')
		return(FALSE);
	len = StringLen(qval);
	BSSeek(bsp, 0, SEEK_SET);

	blen = BSLen(bsp);
	done = FALSE;
	while ((! done) && (len)) {
		  residue1 = qval[(len-1)];
		  if (residue1 == 'X')	/* remove terminal X */
				len--;
		  else
				done = TRUE;
	 }
	 done = FALSE;
	 while ((! done) && (blen)) {
		  BSSeek(bsp, (blen-1), SEEK_SET);
		  residue2 = BSGetByte(bsp);
		  if (residue2 == 'X')
				blen--;
		  else
				done = TRUE;
	 }
		BSSeek(bsp, 0, SEEK_SET);
		if (blen != len) {
			return FALSE;
		} else {
			for (ptr = qval; *ptr != '\0' && 
								(residue = BSGetByte(bsp)) != EOF; ptr++) {

				 if (residue != *ptr) {
					return FALSE;
				 }

			 } /* for */

		 } /* compare two sequences */
			return TRUE;
}

static Boolean check_gcode (GatherContextPtr gcp)
{
	SeqFeatPtr 		sfp, f;
	CdRegionPtr 	cds;
	BioseqPtr 		bsp;
	SeqAnnotPtr     ap;
	ValNodePtr 		gc_vnp, vnp, vnpnext;
	DbtagPtr 		db;
	GeneticCodePtr 	grp;
	Uint1 			gcpvalue;
	CharPtr			protein_seq = NULL;
	ByteStorePtr	byte_sp;
	MolInfoPtr		mfp;
	
	
	gc_vnp = gcp->userdata;
	switch (gcp->thistype) {
		case OBJ_SEQFEAT:
			sfp = (SeqFeatPtr) (gcp->thisitem);
			if (sfp->data.choice != SEQFEAT_CDREGION) {
				return TRUE;
			}
			cds = sfp->data.value.ptrvalue;
			grp = cds->genetic_code;
			if (sfp->product != NULL) {
/*  remove all PID dbxref  */
				for (vnp=sfp->dbxref; vnp; vnp=vnpnext) {
					vnpnext = vnp->next;
					db = vnp->data.ptrvalue;
					if (db->db) {
						if (StringNCmp(db->db, "PID", 3) == 0) {
							sfp->dbxref = remove_node(sfp->dbxref, vnp);
						}
					}
				}
/* change SeqId GENERAL dbtag in ProtRef */
				vnp = SeqLocId(sfp->product);
				FixPIDDbtag(&vnp);
/*  change PID in protein SeqID GENERAL dbtag  */
				bsp = BioseqFind(SeqLocId(sfp->product));
				if (bsp != NULL) {
					FixPIDDbtag(&(bsp->id));
/* change SeqId GENERAL dbtag in ProtRef */
					for (ap = bsp->annot; ap; ap = ap ->next) {
						if (ap->type != 1) {
							continue;
						}
						for (f = ap->data; f; f = f->next) {
							if (f->data.choice != SEQFEAT_PROT) {
								continue;
							}
							vnp = SeqLocId(f->location);
							FixPIDDbtag(&vnp);
						}
					}
				}
			}
			cds = sfp->data.value.ptrvalue;
/* check the translation */
			if (sfp->product) {
				protein_seq = GetProduct(sfp->product, sfp->location);
				byte_sp = ProteinFromCdRegion(sfp, FALSE);
				if (cds->conflict == TRUE) {
					if (CompareTranslation(byte_sp, protein_seq)) {
						cds->conflict = FALSE;
					} else if (bsp != NULL) {
						for (vnp = bsp->descr; vnp != NULL; vnp = vnp->next) {
							if (vnp->choice == Seq_descr_molinfo) {
								break;
							}
						}
						if (vnp != NULL) {
							mfp = vnp->data.ptrvalue;
							mfp->tech = 13; /* _concept_transl_a */
						}
					}
				}
				if (protein_seq)
					MemFree(protein_seq);
				if (byte_sp)
					BSFree(byte_sp);
			}
/* remove asn2ff_generated comments */
			sfp = StripCDSComment(sfp);

/* check genetic code */
			if (GBQualPresent("pseudo", sfp->qual) == TRUE) {
				break;
			}						
			if (cds && gc_vnp) {
				grp = cds->genetic_code;
				if (grp == NULL) {
					gcpvalue = 1;
				} else {
					vnp = grp->data.ptrvalue;
					gcpvalue = vnp->data.intvalue;
				}
				if (gcpvalue != gc_vnp->data.intvalue) {
					CharPtr	str=SeqLocPrint(sfp->location);
					ErrPostEx(SEV_ERROR, ERR_TAXONOMY_GeneticCode, 
	"Genetic code from Taxonomy server is different from the one in CDS %s: %d|%d", str, gc_vnp->data.intvalue, gcpvalue);
					MemFree(str);
				}
			}
			break;
		default:
			break;
	}
	return TRUE;
}

/***************************************************************************
*	This function is twofold
*	1 - checks genetic code with Taxonomy dbase
*	checks db_xref and removes them all if product is present
*	changes PIDe to PID in dbtag
****************************************************************************/
/*
static void CheckGeneticCode(SeqEntryPtr sep)
{
	ValNodePtr vnp = NULL;
	GatherScope gs;
	BioSourcePtr biop;
	Int2 code;
	ValNodePtr gc_vnp;
	Uint2	entityID;	   
	Uint1 focus;

	if (sep == NULL) {
		return;
	}

	entityID = ObjMgrGetEntityIDForChoice(sep);
  	MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
	focus = FocusSeqEntry(sep, &gs);
	MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
	gs.ignore[OBJ_SEQDESC] = FALSE;
	GatherSeqEntry (sep, &vnp, get_src, &gs);
	if (focus == FOCUS_INITIALIZED) {
		gs.target = SeqLocFree(gs.target);
	}
	if (vnp == NULL) {
		ErrPostStr(SEV_WARNING, ERR_SOURCE_NotFound, "BioSource not found");
		return;
	}
	biop = vnp->data.ptrvalue;
	code = BioSourceToGeneticCode(biop);
	ValNodeFree(vnp);
	if (code <= 0) {
		ErrPostStr(SEV_WARNING, ERR_SOURCE_GeneticCode, "Genetic code in BioSource not found");
		gc_vnp = NULL;
	} else {
		gc_vnp = ValNodeNew(NULL);
		gc_vnp->data.intvalue = code;
	}
	MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
	gs.ignore[OBJ_SEQDESC] = TRUE;
	gs.ignore[OBJ_SEQANNOT] = FALSE;
	gs.ignore[OBJ_SEQFEAT] = FALSE;

	GatherSeqEntry (sep, gc_vnp, check_gcode, &gs);
	if (focus == FOCUS_INITIALIZED) {
		gs.target = SeqLocFree(gs.target);
	}
	if (gc_vnp)
		ValNodeFree(gc_vnp);
	
	return;
}
*/

static void CheckGCode (SeqFeatPtr sfp, Pointer userdata)

{
	Int2Ptr  codep;
	Uint1    code;
	SeqFeatPtr      f;
	CdRegionPtr 	cds;
	BioseqPtr 		bsp;
	SeqAnnotPtr     ap;
	ValNodePtr 		vnp, vnpnext;
	DbtagPtr 		db;
	GeneticCodePtr 	grp;
	Uint1 			gcpvalue;
	CharPtr			protein_seq = NULL;
	ByteStorePtr	byte_sp;
	MolInfoPtr		mfp;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION) return;
  codep = (Int2Ptr) userdata;
  if (codep == NULL) return;
  code = (Uint1) *codep;

  cds = (CdRegionPtr) sfp->data.value.ptrvalue;
  if (cds == NULL) return;

			grp = cds->genetic_code;
			if (sfp->product != NULL) {
/*  remove all PID dbxref  */
				for (vnp=sfp->dbxref; vnp; vnp=vnpnext) {
					vnpnext = vnp->next;
					db = vnp->data.ptrvalue;
					if (db->db) {
						if (StringNCmp(db->db, "PID", 3) == 0) {
							sfp->dbxref = remove_node(sfp->dbxref, vnp);
						}
					}
				}
/* change SeqId GENERAL dbtag in ProtRef */
				vnp = SeqLocId(sfp->product);
				FixPIDDbtag(&vnp);
/*  change PID in protein SeqID GENERAL dbtag  */
				bsp = BioseqFind(SeqLocId(sfp->product));
				if (bsp != NULL) {
					FixPIDDbtag(&(bsp->id));
/* change SeqId GENERAL dbtag in ProtRef */
					for (ap = bsp->annot; ap; ap = ap ->next) {
						if (ap->type != 1) {
							continue;
						}
						for (f = ap->data; f; f = f->next) {
							if (f->data.choice != SEQFEAT_PROT) {
								continue;
							}
							vnp = SeqLocId(f->location);
							FixPIDDbtag(&vnp);
						}
					}
				}
			}
			cds = sfp->data.value.ptrvalue;
/* check the translation */
			if (sfp->product) {
				protein_seq = GetProduct(sfp->product, sfp->location);
				byte_sp = ProteinFromCdRegion(sfp, FALSE);
				if (cds->conflict == TRUE) {
					if (CompareTranslation(byte_sp, protein_seq)) {
						cds->conflict = FALSE;
					} else if (bsp != NULL) {
						for (vnp = bsp->descr; vnp != NULL; vnp = vnp->next) {
							if (vnp->choice == Seq_descr_molinfo) {
								break;
							}
						}
						if (vnp != NULL) {
							mfp = vnp->data.ptrvalue;
							mfp->tech = 13; /* _concept_transl_a */
						}
					}
				}
				if (protein_seq)
					MemFree(protein_seq);
				if (byte_sp)
					BSFree(byte_sp);
			}
/* remove asn2ff_generated comments */
			sfp = StripCDSComment(sfp);

/* check genetic code */
			if (GBQualPresent("pseudo", sfp->qual) == TRUE) {
				return;
			}
			if (sfp->pseudo) return;					
			if (cds) {
				grp = cds->genetic_code;
				if (grp == NULL) {
					gcpvalue = 1;
				} else {
					vnp = grp->data.ptrvalue;
					gcpvalue = vnp->data.intvalue;
				}
				if (gcpvalue != code) {
					CharPtr	str=SeqLocPrint(sfp->location);
					ErrPostEx(SEV_ERROR, ERR_TAXONOMY_GeneticCode, 
	"Genetic code from Taxonomy server is different from the one in CDS %s: %d|%d", str, (int) code, (int) gcpvalue);
					MemFree(str);
				}
			}
}

static void CheckGeneticCode (SeqEntryPtr sep)

{
  BioSourcePtr  biop;
  Int2          code;

  if (sep == NULL) return;
  biop = GetTopBioSourceFromSep (sep);
  if (biop == NULL) return;
  code = BioSourceToGeneticCode (biop);

  if (code <= 0) {
    ErrPostStr(SEV_WARNING, ERR_SOURCE_GeneticCode, "Genetic code in BioSource not found");
    return;
  }

  VisitFeaturesInSep (sep, (Pointer) &code, CheckGCode);
}

static Boolean ParseRange(CharPtr pos, Int4 PNTR from, Int4 PNTR to)
{
	CharPtr  ptr, ptr1, ptr2;

	*from = *to = -1;

	if (!IS_DIGIT(*pos))
		return FALSE;
										/* 1st digit */
	for (ptr = pos; IS_DIGIT(*ptr) && *ptr != '\0'; ptr++) continue;

	if (*ptr != '\0') {
		*from = (Int4) atoi(pos);

		ptr1 = ptr;
		if (*ptr1 == '.')
			++ptr1;
		else
			return FALSE;

		if (*ptr1 == '.') {
			++ptr1;
		} else {
			return FALSE;
		}						     /* 2nd digit */
		for (ptr2 = ptr1; IS_DIGIT(*ptr2) && *ptr2 != '\0'; ptr2++) continue;

		if (*ptr2 != '\0') {
			return FALSE;
		} else {
			*to = (Int4) atoi(ptr1);
			return (TRUE);
		}
	} else {
		return FALSE;
	}

}

static SeqLocPtr SeqLocFromPos(SeqIdPtr sid, CharPtr pos)
{
	SeqLocPtr  slp;
	SeqIntPtr  sip;
	Int4		 from, to;

	if (ParseRange(pos, &from, &to)) {

		sip = SeqIntNew();
		sip->from = from - 1;
		sip->to = to - 1;

		slp = ValNodeNew(NULL);
		slp->choice = SEQLOC_INT;
		slp->data.ptrvalue = SeqIdDup(sid);
	} else {
		slp = ValNodeNew(NULL);
		slp->choice = SEQLOC_WHOLE;
		slp->data.ptrvalue = (SeqIdPtr) SeqIdDup(sid);
	}
	return (slp);

}

static CharPtr GetQualValuePos(CharPtr qval)
{
   CharPtr bptr, eptr;

   if ((bptr = StringStr(qval, "(pos:")) == NULL) {
   		return NULL;
   }
    
   bptr += 5;
   while (*bptr == ' ')
       ++bptr;
   if (StringNCmp(bptr, "(complement)", 12) == 0) {
   		bptr += 12;
   }
   while (*bptr == ' ')
       ++bptr;
   for (eptr = bptr; *eptr != ',' && *eptr != '\0'; eptr++) continue;

   return (TextSave(bptr, eptr-bptr));
  
}

static Uint1 GetQualValueAa(CharPtr qval)
{
   CharPtr  str, eptr, ptr;
   Uint1    aa;

	str = StringStr(qval, "aa:");
	if (str != NULL) {
   		str += 3;
   		while (*str == ' ')
       		++str;
   		for (eptr = str; *eptr != ')' && *eptr != ' ' && *eptr != '\0'; eptr++) continue;
	}

    ptr = TextSave(str, eptr-str);
    aa = ValidAminoAcid(ptr);
    MemFree(ptr);  

    return (aa);

}

Boolean ImpFeatToCdregion(SeqFeatPtr sfp)
{
	ImpFeatPtr imp;
	GBQualPtr	q, qnext;
	Int2		frame = -1;
	CdRegionPtr crp;
	CharPtr		pos;
	GeneticCodePtr  gcp;
	Uint1		gc;
	ValNodePtr	vnp;
	Choice			cp;
	CodeBreakPtr	hcbp = NULL, cbp;
	SeqIntPtr		sip;
	SeqLocPtr 		loc;
	BioseqPtr       bsp;
	SeqIdPtr        sidp;
	
	if (sfp == NULL)
		return FALSE;
	if (sfp->data.choice != SEQFEAT_IMP)
		return FALSE;
	imp = sfp->data.value.ptrvalue;
	if (StringCmp(imp->key, "CDS") != 0)
		return FALSE;

	/* do not convert ImpCDS if EMBL or DDBJ */
	bsp = BioseqFindFromSeqLoc (sfp->location);
	if (bsp != NULL) {
		for (sidp = bsp->id;
			sidp != NULL && sidp->choice != SEQID_EMBL && sidp->choice != SEQID_DDBJ;
			sidp = sidp->next) continue;
	}
	if (sidp != NULL) return FALSE;

	sfp->data.choice = SEQFEAT_CDREGION;
	ImpFeatFree(imp);
	crp = CdRegionNew();
	sfp->data.value.ptrvalue = crp;
	for (q = sfp->qual; q; q = qnext) {
		qnext = q->next;
		if (StringCmp(q->qual, "transl_table") == 0) {
			gc = (Uint1) atoi(q->val);
			vnp = ValNodeNew(NULL);
			vnp->choice = 2;
			vnp->data.intvalue = gc;
			gcp = GeneticCodeNew();
			gcp->data.ptrvalue = vnp;
			crp->genetic_code = gcp;
			sfp->qual = remove_qual(sfp->qual, q);
		} else if (StringCmp(q->qual, "translation") == 0) {
			sfp->qual = remove_qual(sfp->qual, q);
		} else if (StringCmp(q->qual, "transl_except") == 0) {
			cp.choice = 1;	/* ncbieaa */
			cp.value.intvalue = (Int4) GetQualValueAa(q->val);
			pos = GetQualValuePos(q->val);
		 	loc = SeqLocFromPos(SeqLocId(sfp->location), pos);
		 	if (loc->choice !=SEQLOC_INT) {
				ErrPostEx(SEV_WARNING, ERR_FEATURE_BadLocation, 
				"Location error for code break [%s]", pos);
		 		MemFree(pos);
				continue;
		 	}
			cbp = CodeBreakNew();
			cbp->aa = cp;
		 	cbp->loc = loc;
			sip = cbp->loc->data.ptrvalue;
			sip->strand = SeqLocStrand(sfp->location);
		 	if (SeqLocCompare(sfp->location, cbp->loc) != SLC_B_IN_A) {
			  CodeBreakFree(cbp);
			  cbp = NULL;
		 	} 
		 	MemFree(pos);
		 	hcbp = tie_next_cbp(hcbp, cbp);
			sfp->qual = remove_qual(sfp->qual, q);
		} else if (StringCmp(q->qual, "codon_start") == 0) {
			frame = (Uint1) atoi(q->val);
			sfp->qual = remove_qual(sfp->qual, q);
			crp->frame = frame; 
		} else if (StringCmp(q->qual, "exception") == 0) {
			sfp->excpt = TRUE; 
		}

	}
	if (frame == -1) {
		frame = GetFrameFromLoc(sfp->location);
		crp->frame = frame; 
	}
	
	return TRUE;	
}

static void NoteToComment (SeqFeatPtr sfp)
{
	GBQualPtr       q, qnext;
	size_t          len;
	CharPtr         str;

	for (q=sfp->qual; q; q=qnext)
	{
		qnext = q->next;
    	if (StringICmp (q->qual, "note") == 0) {
			if (sfp->comment == NULL) {
				sfp->comment = q->val;
			} else {
				len = StringLen (sfp->comment) + StringLen (q->val) + 5;
				str = MemNew (sizeof (Char) * len);
				StringCpy (str, sfp->comment);
				StringCat (str, "; ");
				StringCat (str, q->val);
				sfp->comment = MemFree (sfp->comment);
				q->val = MemFree (q->val);
         		sfp->comment = str;
         	}
			q->val = NULL;
			sfp->qual = remove_qual(sfp->qual, q);
		}
	}
	return;
}
static void ChangeImpFeat (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	BioseqPtr 		bsp;
	BioseqSetPtr	bssp;
	SeqAnnotPtr		sap, annot;
	SeqFeatPtr		sfp;

	if (IS_Bioseq(sep)) {
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		annot = bsp->annot;
	} else {
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		annot = bssp->annot;
	}
	for (sap = annot; sap != NULL; sap = sap->next) {
		if (sap->type != 1) {
			continue;
		}
		for (sfp = sap->data; sfp != NULL; sfp = sfp->next) {
			if (sfp->qual) {
				NoteToComment(sfp);
			}
			if (sfp->data.choice != SEQFEAT_IMP) {
				continue;
			}
			ChangeReplaceToQual(sfp);
			ImpFeatToCdregion(sfp);
		}
	}
}

void ChangeReplaceToQual(SeqFeatPtr sfp)
{
	ImpFeatPtr ifp;
	CharPtr p;
	
	ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
	if(ifp == NULL || ifp->loc == NULL)
		return;
	if ((p = StringStr(ifp->loc, "replace")) != NULL) {
		AddReplaceQual(sfp, p);
		MemFree(ifp->loc);
		ifp->loc = NULL;
	}
	return;
}

/**********************************************************/
void AddReplaceQual(SeqFeatPtr sfp, CharPtr p)
{
	CharPtr s, val;
	
	val = StringChr(p, '\"');
	if(val == NULL)
   		return;
	val++;
	s = p + StringLen(p) - 1;
	if(*s != ')')
   		return;
	for(s--; s > val && *s != '\"'; s--) continue;
		if(*s != '\"')
   			return;
	*s = '\0';
	sfp->qual = (GBQualPtr) AddGBQual(sfp->qual, "replace", val);
	*s = '\"';
	return;
}
/***************************************************************************
*	check and remove HTG keywords automaticly generated by asn2ff
*	HTG info is redundand in GBBlock
***************************************************************************/

static void CheckKeywords(GBBlockPtr gbp, Uint1 tech)
{
	ValNodePtr vnp, vnpnext;
	CharPtr word;
	
	if (gbp == NULL || gbp->keywords == NULL)
		return;
	for (vnp = gbp->keywords; vnp; vnp=vnpnext) {
		vnpnext = vnp->next;
		word = (CharPtr) vnp->data.ptrvalue;
		if (StringCmp(word, "HTG") == 0) {
			MemFree(word);
			gbp->keywords = remove_node(gbp->keywords, vnp);
		}
		else if (tech == MI_TECH_htgs_0 && StringCmp(word, "HTGS_PHASE0") == 0) {
			MemFree(word);
			gbp->keywords = remove_node(gbp->keywords, vnp);
		}
		else if (tech == MI_TECH_htgs_1 && StringCmp(word, "HTGS_PHASE1") == 0) {
			MemFree(word);
			gbp->keywords = remove_node(gbp->keywords, vnp);
		}
		else if (tech == MI_TECH_htgs_2 && StringCmp(word, "HTGS_PHASE2") == 0) {
			MemFree(word);
			gbp->keywords = remove_node(gbp->keywords, vnp);
		}
		else if (tech == MI_TECH_htgs_3 && StringCmp(word, "HTGS_PHASE3") == 0) {
			MemFree(word);
			gbp->keywords = remove_node(gbp->keywords, vnp);
		}
		else if (tech == MI_TECH_est && StringCmp(word, "EST") == 0) {
			MemFree(word);
			gbp->keywords = remove_node(gbp->keywords, vnp);
		}
		else if (tech == MI_TECH_sts && StringCmp(word, "STS") == 0) {
			MemFree(word);
			gbp->keywords = remove_node(gbp->keywords, vnp);
		}
	}
	return;
}

static void ChangeGBDiv (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	BioseqPtr 		bsp=NULL;
	BioseqSetPtr 	bssp;
	ValNodePtr		descr=NULL, vnp;
	CharPtr			div;
	GBBlockPtr		gbp;
	MolInfoPtr		mfp=NULL;
	Int2 i;

	div = (CharPtr) data;
	if (IS_Bioseq(sep)) {
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		descr = bsp->descr;
	} else {
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		descr = bssp->descr;
	}
	for (vnp = descr; vnp != NULL; vnp = vnp->next) {
		if (vnp->choice == Seq_descr_molinfo) {
			mfp = (MolInfoPtr) vnp->data.ptrvalue;
			break;
		}
	}
	for (vnp = descr; vnp != NULL; vnp = vnp->next) {
		if (vnp->choice == Seq_descr_genbank) {
			gbp = (GBBlockPtr) vnp->data.ptrvalue;
			if (gbp == NULL) {
				return;
			}
			if (mfp) {
				if (mfp->tech == MI_TECH_htgs_0 || 
						mfp->tech == MI_TECH_htgs_1 || 
						mfp->tech == MI_TECH_htgs_2 || 
						mfp->tech == MI_TECH_htgs_3 || 
						mfp->tech == MI_TECH_est ||
						mfp->tech == MI_TECH_sts) {
					CheckKeywords(gbp, mfp->tech);
				}
			}
			if (gbp->div == NULL) {
				return;
			}
			for (i=0; i < TOTAL_TECH; i++) {
				if (StringCmp(gbp->div, check_tech[i].name) == 0) {
					break;
				}
			}
			if (i != TOTAL_TECH) {
				if (mfp) {
					if (StringCmp(gbp->div, "HTG") == 0 
						|| StringCmp(gbp->div, "PRI") == 0) {
						if (mfp->tech == MI_TECH_htgs_1 
							|| mfp->tech == MI_TECH_htgs_2 || 
								mfp->tech == MI_TECH_htgs_3) {
							gbp->div = MemFree(gbp->div);
							return;
						}
					} else if (mfp->tech == check_tech[i].num) {
						gbp->div = MemFree(gbp->div);
						return;
					} else if (mfp->tech == 0 && StringCmp (gbp->div, "STS") == 0) {
						mfp->tech = MI_TECH_sts;
						gbp->div = MemFree(gbp->div);
						return;
					}
				}
			}
			if (div != NULL) {
				if (StringCmp(gbp->div, div) == 0) {
					gbp->div = MemFree(gbp->div);
					gbp->taxonomy = MemFree(gbp->taxonomy);
				} else if (StringCmp(gbp->div, "UNA") == 0) {
					gbp->div = MemFree(gbp->div);
				} else if (StringCmp(gbp->div, "UNC") == 0) {
					gbp->div = MemFree(gbp->div);
				}
			} 
		}
	}
}

static void ChangeGBSource (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	BioseqPtr 		bsp;
	ValNodePtr		descr, vnp;
	CharPtr			source, s;
	GBBlockPtr		gbp;

	if (IS_Bioseq(sep)) {
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		descr = bsp->descr;
		source = (CharPtr) data;
	} else {
		return;
	}
	for (vnp = descr; vnp != NULL; vnp = vnp->next) {
		if (vnp->choice == Seq_descr_genbank) {
			gbp = (GBBlockPtr) vnp->data.ptrvalue;
			if (gbp == NULL || gbp->source == NULL) {
				return;
			}
			s = StringSave(gbp->source);
			if (*(s+StringLen(s)-1) =='.') {
				*(s+StringLen(s)-1) = '\0';
			}
			if (StringICmp(source, s) == 0) {
				gbp->source = MemFree(gbp->source);
			}
			MemFree(s);
		}
	}
}

void EntryChangeGBSource (SeqEntryPtr sep)
{
	OrgRefPtr 		orp=NULL;
	BioSourcePtr 	biosp;
	CharPtr 		source=NULL, s, div = NULL;
	ValNodePtr 		vnp = NULL, v;
	/*
	GatherScope 	gs;
	Uint2			entityID;	   
	Uint1 			focus;
	*/
	Int2			len=0;
	
	if (sep == NULL)
		return;

	/*
	entityID = ObjMgrGetEntityIDForChoice(sep);
  	MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
	focus = FocusSeqEntry(sep, &gs);
	MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
	gs.ignore[OBJ_SEQDESC] = FALSE;
	
	GatherSeqEntry (sep, &vnp, get_src, &gs);

        if(gs.target != NULL)
            SeqLocFree(gs.target);

	if (vnp == NULL) {
		ErrPostStr(SEV_WARNING, ERR_SOURCE_NotFound, "BioSource not found");
		return;
	}
	biosp = (BioSourcePtr) vnp->data.ptrvalue;
	vnp=MemFree(vnp);
	*/

	biosp = GetTopBioSourceFromSep (sep);
	if (biosp != NULL) {
		orp = biosp->org;
	}
	if (orp) {
		if (orp->common) {
			len = StringLen(orp->common);
		} else if(orp->taxname) {
			len = StringLen(orp->taxname);
		}
		for (v = orp->mod; v; v = v->next) {
			len += StringLen(v->data.ptrvalue) + 1;
		}
		if (len > 0) {
			source = s = MemNew(len+1);
			*s = '\0';
			if (orp->common) {
				StringCpy(s, orp->common);
			} else if(orp->taxname) {
				StringCpy(s, orp->taxname);
			}
			s += StringLen(s);
			for (v = orp->mod; v; v = v->next) {
				sprintf(s, " %s", (CharPtr) v->data.ptrvalue);
				s += StringLen(s);
			}
			if (*(source+len-1) == '.') {
				*(source+len-1) = '\0';
			}
		}
		if (orp->orgname && orp->orgname->div) {
			div = StringSave(orp->orgname->div);
		}
	}
	SeqEntryExplore(sep, source, ChangeGBSource);
	SeqEntryExplore(sep, div, ChangeGBDiv);
	if (div)
		MemFree(div);
	if (source)
		MemFree(source);
	return;
}

void EntryChangeImpFeat (SeqEntryPtr sep)
{
	if (sep == NULL)
		return;
	SeqEntryExplore(sep, NULL, ChangeImpFeat);
	EntryChangeImpFeatToProt(sep);
	return;
}

static void MergeDupBioSources (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)

{
  BioSourcePtr  biop1, biop2;
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Boolean       fuseanddelete;
  ValNodePtr    mod;
  ValNodePtr    nextvnp;
  OrgModPtr     omp;
  Pointer PNTR  prevvnp;
  ValNodePtr    sdp;
  SubSourcePtr  ssp;
  OrgNamePtr    onp1, onp2;
  OrgRefPtr     orp1, orp2;
  ValNodePtr    vnp;

  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sdp = bsp->descr;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sdp = bssp->descr;
  } else {
  	return;
  }
  while (sdp != NULL) {
    if (sdp->choice == Seq_descr_source && sdp->data.ptrvalue != NULL) {
      biop1 = (BioSourcePtr) sdp->data.ptrvalue;
      orp1 = biop1->org;
      if (orp1 != NULL) {
        vnp = sdp->next;
        prevvnp = (Pointer PNTR) &(sdp->next);
        while (vnp != NULL) {
          nextvnp = vnp->next;
          fuseanddelete = FALSE;
          biop2 = NULL;
          orp2 = NULL;
          if (vnp->choice == Seq_descr_source && vnp->data.ptrvalue != NULL) {
            biop2 = (BioSourcePtr) vnp->data.ptrvalue;
            orp2 = biop2->org;
            if (orp2 != NULL) {
              if ((orp1->taxname != NULL) && (orp2->taxname != NULL) &&
                  StringCmp (orp1->taxname, orp2->taxname) == 0) {
                fuseanddelete = TRUE;
              }
            }
          }
          if (fuseanddelete) {
            *(prevvnp) = vnp->next;
            vnp->next = NULL;
            if (biop2 != NULL) {
              if (biop1->genome == 0) {
                biop1->genome = biop2->genome;
              }
              if (biop1->origin == 0) {
                biop1->origin = biop2->origin;
              }
              if (! biop1->is_focus) {
                biop1->is_focus = biop2->is_focus;
              }
              if (biop1->subtype == NULL) {
                biop1->subtype = biop2->subtype;
                biop2->subtype = NULL;
              } else {
                ssp = biop1->subtype;
                while (ssp->next != NULL) {
                  ssp = ssp->next;
                }
                ssp->next = biop2->subtype;
                biop2->subtype = NULL;
              }
              if (orp1 != NULL && orp2 != NULL) {
                if (orp1->mod == NULL) {
                  orp1->mod = orp2->mod;
                  orp2->mod = NULL;
                } else {
                  mod = orp1->mod;
                  while (mod->next != NULL) {
                    mod = mod->next;
                  }
                  mod->next = orp2->mod;
                  orp2->mod = NULL;
                }
                if (orp1->db == NULL) {
                  orp1->db = orp2->db;
                  orp2->db = NULL;
                }
                if (orp1->syn == NULL) {
                  orp1->syn = orp2->syn;
                  orp2->syn = NULL;
                }
                onp1 = orp1->orgname;
                onp2 = orp2->orgname;
                if (onp1 != NULL && onp2 != NULL) {
                  if (onp1->mod == NULL) {
                    onp1->mod = onp2->mod;
                    onp2->mod = NULL;
                  } else {
                    omp = onp1->mod;
                    while (omp->next != NULL) {
                      omp = omp->next;
                    }
                    omp->next = onp2->mod;
                    onp2->mod = NULL;
                  }
                  if (onp1->gcode == 0) {
                    onp1->gcode = onp2->gcode;
                  }
                  if (onp1->mgcode == 0) {
                    onp1->mgcode = onp2->mgcode;
                  }
                  if (onp1->lineage == NULL) {
                    onp1->lineage = onp2->lineage;
                    onp2->lineage = NULL;
                  }
                  if (onp1->div == NULL) {
                    onp1->div = onp2->div;
                    onp2->div = NULL;
                  }
                }
              }
            }
            SeqDescFree (vnp);
          } else {
            prevvnp = (Pointer PNTR) &(vnp->next);
          }
          vnp = nextvnp;
        }
      }
    }
    sdp = sdp->next;
  }
}

void EntryMergeDupBioSources (SeqEntryPtr sep)

{
  SeqEntryExplore (sep, NULL, MergeDupBioSources);
}

static CharPtr TASNTrimSpacesAndTrailingSemicolons (CharPtr str)

{
  CharPtr  amp;
  Uchar    ch;	/* to use 8bit characters in multibyte languages */
  CharPtr  dst;
  CharPtr  ptr;

  if (str != NULL && str [0] != '\0') {
    dst = str;
    ptr = str;
    ch = *ptr;
    while (ch != '\0' && ch <= ' ') {
      ptr++;
      ch = *ptr;
    }
    while (ch != '\0') {
      *dst = ch;
      dst++;
      ptr++;
      ch = *ptr;
    }
    *dst = '\0';
    amp = NULL;
    dst = NULL;
    ptr = str;
    ch = *ptr;
    while (ch != '\0') {
      if (ch == '&') {
        amp = ptr;
        dst = NULL;
      } else if (ch == ' ') {
        if (dst == NULL) {
          dst = ptr;
        }
        amp = NULL;
      } else if (ch == ';') {
        if (dst == NULL && amp == NULL) {
          dst = ptr;
        }
      } else {
        dst = NULL;
      }
      ptr++;
      ch = *ptr;
    }
    if (dst != NULL) {
      *dst = '\0';
    }
  }
  return str;
}

static CharPtr TASNTrimInternalSemicolons (CharPtr str)

{
  Uchar    ch;	/* to use 8bit characters in multibyte languages */
  CharPtr  dst;
  Boolean  hasspace;
  CharPtr  ptr;
  CharPtr  tmp;

  if (str != NULL && str [0] != '\0') {
    dst = str;
    ptr = str;
    ch = *ptr;
    while (ch != '\0') {
      if (ch == ';') {
        *dst = ch;
        dst++;
        ptr++;
        ch = *ptr;
        tmp = ptr;
        hasspace = FALSE;
        while (ch == ';' || ch == ' ' || ch == '\t') {
          if (ch == ' ') {
            hasspace = TRUE;
          }
          ptr++;
          ch = *ptr;
        }
        if (hasspace) {
          *dst = ' ';
          dst++;
        }
      } else {
        *dst = ch;
        dst++;
        ptr++;
        ch = *ptr;
      }
    }
    *dst = '\0';
  }
  return str;
}

static Boolean TASNStringHasNoText (CharPtr str)

{
  Uchar  ch;	/* to use 8bit characters in multibyte languages */

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

static void CleanVisString (CharPtr PNTR strp)

{
  if (strp == NULL) return;
  if (*strp == NULL) return;
  TASNTrimSpacesAndTrailingSemicolons (*strp);
  TASNTrimInternalSemicolons (*strp);
  if (TASNStringHasNoText (*strp)) {
    *strp = MemFree (*strp);
  }
}

static void CleanVisStringJunk (CharPtr PNTR strp)

{
  if (strp == NULL) return;
  if (*strp == NULL) return;
  TrimSpacesAndJunkFromEnds (*strp, TRUE);
  TASNTrimInternalSemicolons (*strp);
  if (TASNStringHasNoText (*strp)) {
    *strp = MemFree (*strp);
  }
}

static void CleanVisStringList (ValNodePtr PNTR vnpp)

{
  ValNodePtr       next;
  ValNodePtr PNTR  prev;
  ValNodePtr       vnp;

  if (vnpp == NULL) return;
  prev = vnpp;
  vnp = *vnpp;
  while (vnp != NULL) {
    next = vnp->next;
    TASNTrimSpacesAndTrailingSemicolons (vnp->data.ptrvalue);
    if (TASNStringHasNoText (vnp->data.ptrvalue)) {
      *prev = vnp->next;
      vnp->next = NULL;
      ValNodeFreeData (vnp);
    } else {
      prev = &(vnp->next);
    }
    vnp = next;
  }
}

static void CheckGBBlock (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Boolean       empty;
  GBBlockPtr    gbp;
  BoolPtr       hasGB;
  ValNodePtr    nextsdp;
  Pointer PNTR  prevsdp;
  ValNodePtr    sdp;

  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sdp = bsp->descr;
    prevsdp = (Pointer PNTR) &(bsp->descr);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sdp = bssp->descr;
    prevsdp = (Pointer PNTR) &(bssp->descr);
  } else {
  	return;
  }
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;
  hasGB = (BoolPtr) data;
  sdp = bsp->descr;
  prevsdp = (Pointer PNTR) &(bsp->descr);
  while (sdp != NULL) {
    nextsdp = sdp->next;
    empty = FALSE;
    if (sdp->choice == Seq_descr_genbank && sdp->data.ptrvalue != NULL) {
      gbp = (GBBlockPtr) sdp->data.ptrvalue;
      if (gbp->source != NULL || gbp->taxonomy != NULL) {
        if (hasGB != NULL) {
          *hasGB = TRUE;
        }
      } else if (gbp->div != NULL) {
        if (StringCmp (gbp->div, "PAT") != 0 &&
            StringCmp (gbp->div, "SYN")) {
          if (hasGB != NULL) {
            *hasGB = TRUE;
          }
        }
      }
      CleanVisStringList (&(gbp->extra_accessions));
      CleanVisStringList (&(gbp->keywords));
      CleanVisString (&(gbp->source));
      CleanVisString (&(gbp->origin));
      CleanVisString (&(gbp->date));
      CleanVisString (&(gbp->div));
      CleanVisString (&(gbp->taxonomy));
      if (gbp->extra_accessions == NULL && gbp->source == NULL &&
          gbp->keywords == NULL && gbp->origin == NULL &&
          gbp->date == NULL && gbp->entry_date == NULL &&
          gbp->div == NULL && gbp->taxonomy == NULL) {
        empty = TRUE;
        ObjMgrDeSelect (0, 0, 0, 0, NULL);
      }
    }
    if (empty) {
      *(prevsdp) = sdp->next;
      sdp->next = NULL;
      sdp = SeqDescFree (sdp);
    } else {
      prevsdp = (Pointer PNTR) &(sdp->next);
    }
    sdp = nextsdp;
  }
}

extern Boolean EntryCheckGBBlock (SeqEntryPtr sep)

{
  Boolean  hasGBStuff;

  hasGBStuff = FALSE;
  SeqEntryExplore (sep, (Pointer) &hasGBStuff, CheckGBBlock);
  return hasGBStuff;
}

/*****************************************************************************
*
*   CdEndCheck(sfp, fp)
*
*****************************************************************************/
static void CdEndCheck(SeqFeatPtr sfp, FILE *fp)
{
	ByteStorePtr newprot = NULL;
	BioseqPtr protseq, nucseq;
	SeqLocPtr last=NULL, curr = NULL;
	Int4 len, remainder, aas, oldfrom, oldto, protlen, i, oldnum;
	CdRegionPtr crp;
	SeqIdPtr protid, tmp;
	SeqIntPtr sip;
	Int2 residue, residue2;
	Char nuc[40];
	CodeBreakPtr cbp;
	Int4 pos1, pos2, pos;
	SeqLocPtr tmpslp;
	Int4 len2;
	SeqFeatPtr gene = NULL;
	GeneRefPtr grp;
	BioseqPtr bsp;
	SeqLocPtr slp;
  Boolean        hasNulls;
  Boolean        noLeft;
  Boolean        noRight;
  Boolean        noLeftFeat;
  Boolean        noLeftGene;
  Boolean        noRightFeat;
  Boolean        noRightGene;


	grp = SeqMgrGetGeneXref (sfp);
	if (grp == NULL || (! SeqMgrGeneIsSuppressed (grp))) {
	  gene = SeqMgrGetOverlappingGene (sfp->location, NULL);
	}

	crp = (CdRegionPtr)(sfp->data.value.ptrvalue);
	len = SeqLocLen(sfp->location);
	len2 = len;
	switch (crp->frame)
	{
		case 2:
			len -= 1;
			break;
		case 3:
			len -= 2;
			break;
		default:
			break;
	}
	remainder = len % 3;
	aas = len/3;    /* total aas in protein if no stop codon */
	protid = SeqLocId(sfp->product);
	if (protid == NULL)
		return;
	/* protseq = BioseqFind(protid); */
	protseq = BioseqLockById (protid); /* tries BioseqFind, will fetch remotely if enabled */
	if (protseq == NULL) return;
	BioseqUnlock (protseq); /* unlock but do not cache out, easier than unlocking everywhere in code below */
	if (((protseq->length + 1) == aas) && (remainder == 0)) /* correct length with termination */
		return;

	cbp = crp->code_break;
	while (cbp != NULL)
	{
		pos1 = INT4_MAX;
		pos2 = -10;
		tmpslp = NULL;
		while ((tmpslp = SeqLocFindNext(cbp->loc, tmpslp)) != NULL)
		{
			pos = GetOffsetInLoc(tmpslp, sfp->location, SEQLOC_START);
			if (pos < pos1)
				pos1 = pos;
			pos = GetOffsetInLoc(tmpslp, sfp->location, SEQLOC_STOP);
			if (pos > pos2)
				pos2 = pos;
		}
		pos = pos2 - pos1; /* codon length */
		if (/* pos == 2 || */ (pos >= 0 && pos <= 1 && pos2 == len2 - 1))   /*  a codon */
		/* allowing a partial codon at the end */
		{
			return;
		}

		cbp = cbp->next;
	}

	while ((curr = SeqLocFindNext(sfp->location, curr)) != NULL)
	{
		last = curr;
	}

	if (last->choice != SEQLOC_INT)  /* this is too weird */
	{
		return;
	}
	sip = (SeqIntPtr)(last->data.ptrvalue);
	nucseq = BioseqFind(sip->id);
	if (nucseq == NULL)
	{
		return;
	}

	switch (remainder)
	{
		case 0:
			remainder = 3;
			break;
		case 1:
			remainder = 2;
			break;
		case 2:
			remainder = 1;
			break;
	}

	oldfrom = sip->from;
	oldto = sip->to;

	if (sip->strand == Seq_strand_minus)
	{
		if (sip->from < remainder)
		{
			return;
		}
		if (sip->if_from != NULL)
		{
			return;
		}
		oldnum = sip->from;
		sip->from -= remainder;
	}
	else
	{
		if (sip->to >= (nucseq->length - remainder))
		{
			return;
		}
		if (sip->if_to != NULL)
		{
			return;
		}
		oldnum = sip->to;
		sip->to += remainder;
	}

	newprot = ProteinFromCdRegion(sfp, TRUE);   /* include stop codons */
	if (newprot == NULL)
	{
		goto erret;
	}

	protlen = BSLen(newprot);
	if (protlen != ((len + remainder)/3))
	{
		goto erret;
	}

	BSSeek(newprot, (protlen - 1), SEEK_SET);
	residue = BSGetByte(newprot);
	if (residue != '*')
	{
		goto erret;
	}

	BSSeek(newprot, (protlen-1), SEEK_SET);
	BSDelete(newprot, 1);   /* remove termination from protein */
	BSSeek(newprot, 0, SEEK_SET);  /* check for internal termination */
	BSSeek(protseq->seq_data, 0, SEEK_SET);
	protlen = BSLen(newprot);
	for (i = 0; i < protlen; i++)
	{
		residue = BSGetByte(newprot);
		residue2 = BSGetByte(protseq->seq_data);
		if (residue != residue2)
		{
			goto erret;
		}

	}

	BSFree(protseq->seq_data);
	protseq->seq_data = newprot;
	protseq->length = protlen;
	/****** to avoid killing asn2gnbk ***
	protseq->seq_data_type = Seq_code_ncbieaa;
	sfp->partial = FALSE;

	************************************/
	for (tmp = nucseq->id; tmp != NULL; tmp = tmp->next)
	{
		if ((tmp->choice == SEQID_GENBANK) ||
			(tmp->choice == SEQID_EMBL) ||
			(tmp->choice == SEQID_DDBJ))
			break;
	}

	if (tmp == NULL)
		SeqIdPrint(nucseq->id, nuc, PRINTID_FASTA_LONG);
	else
		SeqIdPrint(tmp, nuc, PRINTID_TEXTID_ACCESSION);

	if (fp != NULL)
		fprintf(fp, "%s %ld %d\n", nuc, (long)(oldnum+1), (int)remainder);

	if (gene != NULL) {
		if (SeqLocAinB (sfp->location, gene->location) <= 0) {
            bsp = BioseqFindFromSeqLoc (gene->location);
            if (bsp != NULL) {
              hasNulls = LocationHasNullsBetween (gene->location);
              slp = SeqLocMerge (bsp, gene->location, sfp->location, TRUE, FALSE, hasNulls);
              if (slp != NULL) {
                CheckSeqLocForPartial (gene->location, &noLeftGene, &noRightGene);
                gene->location = SeqLocFree (gene->location);
                gene->location = slp;
                CheckSeqLocForPartial (sfp->location, &noLeftFeat, &noRightFeat);
                if (bsp->repr == Seq_repr_seg) {
                  slp = SegLocToPartsEx (bsp, gene->location, TRUE);
                  gene->location = SeqLocFree (gene->location);
                  gene->location = slp;
                  hasNulls = LocationHasNullsBetween (gene->location);
                  gene->partial = (gene->partial || hasNulls);
                }
                FreeAllFuzz (gene->location);
                noLeft = (noLeftFeat || noLeftGene);
                noRight = (noRightFeat || noRightGene);
                SetSeqLocPartial (gene->location, noLeft, noRight);
                gene->partial = (gene->partial || noLeft || noRight);
              }
            }
          
		}
	}

	return;
erret:
	BSFree(newprot);
	sip->from = oldfrom;
	sip->to = oldto;
	return;
}

static void FindCd (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	SeqAnnotPtr sap;
	SeqFeatPtr sfp;
	BioseqPtr bsp;
	BioseqSetPtr bssp;
	FILE *fp;

	fp = (FILE *)data;
	if (IS_Bioseq(sep))
	{
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		sap = bsp->annot;
	}
	else
	{
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		sap = bssp->annot;
	}

	while (sap != NULL)
	{
		if (sap->type == 1)   /* ftable */
		{
			sfp = (SeqFeatPtr)(sap->data);
			while (sfp != NULL)
			{
				if (sfp->data.choice == 3) { /* cdregion */
					if (! sfp->excpt) { /* if not biological exception */
						CdEndCheck(sfp, fp);
					}
				}
				sfp = sfp->next;
			}
		}
		sap = sap->next;
	}

	return;
}

static SeqLocPtr fake_bond_loc(SeqLocPtr slp)
{
	SeqLocPtr loc, l, lnext, ldata;
	
	
	if (slp == NULL)
		return NULL;
	loc = MemNew(sizeof(SeqLoc));
	MemCopy(loc, slp, sizeof(SeqLoc));
	ldata = (SeqLocPtr) loc->data.ptrvalue;
	if (slp->choice != SEQLOC_MIX)
		return loc;
	for (l=ldata; l; l=lnext) {
		lnext = l->next;
		if (l->choice == SEQLOC_NULL) {
			ldata = remove_node(ldata, l);
		}
	}	
	return loc;
	
}

/*****************************************************************************
*
*   Check for CdRegion ending in middle base of codon
*
*****************************************************************************/

void CdCheck(SeqEntryPtr sep, FILE *fp)
{
	SeqEntryExplore(sep, (Pointer)fp, FindCd);
	return;
}

static Boolean OutOfFramePeptideButEmblOrDdbj (SeqFeatPtr sfp, SeqFeatPtr cds)

{
  BioseqPtr    bsp;
  CdRegionPtr  crp;
  ImpFeatPtr   ifp;
  SeqLocPtr    first = NULL, last = NULL, slp = NULL;
  Boolean      partial5, partial3;
  Int4         pos1, pos2, adjust = 0, mod1, mod2;
  SeqIdPtr     sip;

  if (sfp == NULL || cds == NULL || sfp->data.choice != SEQFEAT_IMP) return FALSE;
 
  ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
  if (ifp == NULL) return FALSE;
  if (StringCmp (ifp->key, "mat_peptide") != 0 &&
      StringCmp (ifp->key, "sig_peptide") != 0 &&
      StringCmp (ifp->key, "transit_peptide") != 0) return FALSE;

  crp = (CdRegionPtr) cds->data.value.ptrvalue;
  if (crp == NULL) return FALSE;
  if (crp->frame == 2) {
    adjust = 1;
  } else if (crp->frame == 3) {
    adjust = 2;
  }

  while ((slp = SeqLocFindNext (sfp->location, slp)) != NULL) {
    last = slp;
    if (first == NULL) {
      first = slp;
    }
  }
  if (first == NULL || last == NULL) return FALSE;

  pos1 = GetOffsetInLoc (first, cds->location, SEQLOC_START) - adjust;
  pos2 = GetOffsetInLoc (last, cds->location, SEQLOC_STOP) - adjust;
  mod1 = pos1 % 3;
  mod2 = pos2 % 3;

  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
  if (partial5) {
    mod1 = 0;
  }
  if (partial3) {
    mod2 = 2;
  }

  if (mod1 == 0 && mod2 == 2) return FALSE;

  bsp = BioseqFindFromSeqLoc (sfp->location);
  if (bsp == NULL) return FALSE;
  for (sip = bsp->id;
       sip != NULL && sip->choice != SEQID_EMBL && sip->choice != SEQID_DDBJ;
       sip = sip->next) continue;
  if (sip != NULL) return TRUE;

  return FALSE;
}

static void ImpFeatToProtRef(SeqFeatArr sfa)
{
	SeqFeatPtr f1, f2, best_cds, sfp;
	SeqLocPtr loc, slp;
	ImpFeatPtr ifp;
	ProtRefPtr prot;
	BioseqPtr bsp;
	SeqAnnotPtr sap;
	Int4 diff_lowest, diff_current, frame;
	ValNodePtr tmp1, tmp2;
	Uint2 retval;
	Int2 i;
	Boolean lfree = FALSE, partial5, partial3;
	CharPtr p, q;
	GBQualPtr qu, qunext;
	
	for (tmp1 = sfa.pept; tmp1; tmp1 = tmp1->next) {
		lfree = FALSE;
		f1 = (SeqFeatPtr) tmp1->data.ptrvalue;
		loc = f1->location;
		if (tmp1->choice == SEQFEAT_BOND) {
			loc = fake_bond_loc(f1->location);
			lfree = TRUE;
		}
		diff_lowest = -1;
		best_cds = NULL;
		for (tmp2=sfa.cds; tmp2; tmp2=tmp2->next) {
			f2 = tmp2->data.ptrvalue;
			diff_current = SeqLocAinB(loc, f2->location);
			if (! diff_current)   /* perfect match */ {
				best_cds = f2;
				break;
			} else if (diff_current > 0) {
				if ((diff_lowest == -1) || (diff_current < diff_lowest)) {
					diff_lowest = diff_current;
					best_cds = f2;
				}
			}
		}
/*		if (lfree)
			SeqLocFree(loc);
*/
		if (best_cds == NULL) { 
			p = SeqLocPrint(f1->location);
			ErrPostEx(SEV_WARNING, ERR_FEATURE_CDSNotFound, 
			"CDS for the peptide feature [%s] not found", p);
			MemFree(p);
		} else {
			if (OutOfFramePeptideButEmblOrDdbj (f1, best_cds))
				continue;
			CheckSeqLocForPartial (f1->location, &partial5, &partial3);
			slp = dnaLoc_to_aaLoc(best_cds, f1->location, TRUE, &frame, FALSE);
			if (slp == NULL) {
			p = SeqLocPrint(f1->location);
			q = SeqLocPrint(best_cds->location);
			ErrPostEx(SEV_ERROR, ERR_FEATURE_CannotMapDnaLocToAALoc, "peptide location:%s| CDS location:%s", p, q);
			MemFree(p);
			MemFree(q);
				continue;
			}
			SetSeqLocPartial (slp, partial5, partial3);
			ifp = (ImpFeatPtr) f1->data.value.ptrvalue;
			sfp = SeqFeatNew();
			sfp->location = slp;

			sfp->partial = (Boolean) (f1->partial || partial5 || partial3);
			sfp->excpt = f1->excpt;
			sfp->exp_ev = f1->exp_ev;
			sfp->pseudo = f1->pseudo;

			sfp->comment = f1->comment;
			f1->comment = NULL;
			sfp->qual = f1->qual;
			f1->qual = NULL;
			sfp->title = f1->title;
			f1->title = NULL;
			sfp->ext = f1->ext;
			f1->ext = NULL;
			sfp->cit = f1->cit;
			f1->cit = NULL;

			sfp->xref = f1->xref;
			f1->xref = NULL;
			sfp->dbxref = f1->dbxref;
			f1->dbxref = NULL;
			sfp->except_text = f1->except_text;
			f1->except_text = NULL;

			if (f1->qual != NULL) {
				sfp->qual = f1->qual;
				f1->qual = NULL;
			}
			if (tmp1->choice == SEQFEAT_PROT) {
				sfp->data.choice = SEQFEAT_PROT;
				prot = ProtRefNew();
				sfp->data.value.ptrvalue = prot;
				if (StringCmp(ifp->key, "mat_peptide") == 0) {
					prot->processed = 2;
					for (qu=sfp->qual; qu; qu=qunext) {
						qunext = qu->next;
						if (StringCmp(qu->qual, "product") == 0) {
							ValNodeAddStr(&(prot->name), 0,StringSave(qu->val));
							sfp->qual = remove_qual(sfp->qual, qu); 
						}
					}
				}
				if (StringCmp(ifp->key, "sig_peptide") == 0)
					prot->processed = 3;
				if (StringCmp(ifp->key, "transit_peptide") == 0)
					prot->processed = 4;
				if (f1->comment != NULL) {
					if ((prot->processed == 2 || prot->name == NULL) && StringICmp (f1->comment, "putative") != 0) {
						ValNodeAddStr(&(prot->name), 0,StringSave(f1->comment));
					} else {
						sfp->comment = StringSave(f1->comment);
					}
				}
			} else if (tmp1->choice == SEQFEAT_SITE) {
				sfp->data.choice = SEQFEAT_SITE;
				if ((i = FindStr(feat_site, num_site, f1->comment)) != -1) {
					sfp->data.value.intvalue = i;
				} else {
					sfp->data.value.intvalue = 255;
				}
			} else if (tmp1->choice == SEQFEAT_BOND) {
				sfp->data.choice = SEQFEAT_BOND;
				if ((i = FindStr(feat_bond, num_bond, f1->comment)) != -1) {
					sfp->data.value.intvalue = i;
				} else {
					sfp->data.value.intvalue = 255;
				}
			}
			if (f1->title)
			{
				if(sfp->comment != NULL)
					MemFree(sfp->comment);
				sfp->comment = StringSave(f1->title);
			}
			CheckSeqLocForPartial (f1->location, &partial5, &partial3);
			sfp->excpt = f1->excpt;
			sfp->partial = (Boolean) (f1->partial || partial5 || partial3);
			sfp->exp_ev = f1->exp_ev;
			sfp->pseudo = f1->pseudo;
			if(sfp->location)
				SeqLocFree(sfp->location);
			sfp->location = 
				dnaLoc_to_aaLoc(best_cds, f1->location, TRUE, &frame, FALSE);
			if (sfp->location == NULL) {
			p = SeqLocPrint(f1->location);
			q = SeqLocPrint(best_cds->location);
			ErrPostEx(SEV_ERROR, ERR_FEATURE_CannotMapDnaLocToAALoc, "peptide location:%s| CDS location:%s", p, q);
				MemFree(sfp);
				MemFree(p);
				MemFree(q);
				continue;
			}
			SetSeqLocPartial (sfp->location, partial5, partial3);
			if(f1->comment != NULL)
				MemFree(f1->comment);
			f1->comment = StringSave("FeatureToBeDeleted");
			if (sfp->partial == FALSE) {
				retval = SeqLocPartialCheck(sfp->location);
				if (retval > SLP_COMPLETE && retval < SLP_NOSTART) {
					sfp->partial = TRUE;
				}
			}
			bsp = BioseqLockById(SeqLocId(best_cds->product));
			if (bsp) {
				if (bsp->annot == NULL) {
					sap = SeqAnnotNew();
					sap->type = 1;
					bsp->annot = sap;
				} else {
					sap = bsp->annot;
				}
				sap->data = tie_feat(sap->data, sfp);
				BioseqUnlock(bsp); 
			}
		}
	}
}

static void GetCdRegionsWithPeptides (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	BioseqPtr 		bsp;
	BioseqSetPtr	bssp;
	SeqAnnotPtr		sap, annot;
	SeqFeatPtr		sfp;
	ImpFeatPtr 		ifp;
	SeqFeatArrPtr	sfap;
	ValNodePtr 		tmp;
	Int2 			i;
	
	sfap = (SeqFeatArrPtr) data;
	if (IS_Bioseq(sep)) {
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		annot = bsp->annot;
	} else {
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		annot = bssp->annot;
	}
	for (sap = annot; sap != NULL; sap = sap->next) {
		if (sap->type != 1) {
			continue;
		}
		for (sfp = sap->data; sfp != NULL; sfp = sfp->next) {
			if (sfp->data.choice == SEQFEAT_CDREGION) {
				tmp = ValNodeNew(NULL);
				tmp->data.ptrvalue = sfp;
				sfap->cds = tie_next(sfap->cds, tmp);
			}
			if (sfp->data.choice == SEQFEAT_IMP) {
				ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
				if (StringCmp(ifp->key, "mat_peptide") == 0 ||
					StringCmp(ifp->key, "sig_peptide") == 0 ||
						StringCmp(ifp->key, "transit_peptide") == 0) {
					tmp = ValNodeNew(NULL);
					tmp->choice = SEQFEAT_PROT;
					tmp->data.ptrvalue = sfp;
					sfap->pept = tie_next(sfap->pept, tmp);
				} else if (StringCmp(ifp->key, "misc_feature") == 0 
						&& sfp->comment != NULL) {
					if ((i = FindStr(feat_site, num_site, sfp->comment)) != -1){
						if (i >= 23 && i <= 25) {
							tmp = ValNodeNew(NULL);
							tmp->choice = SEQFEAT_SITE;
							tmp->data.ptrvalue = sfp;
							sfap->pept = tie_next(sfap->pept, tmp);
						}
					} else if ((i = 
							FindStr(feat_bond, num_bond, sfp->comment)) != -1){
						tmp = ValNodeNew(NULL);
						tmp->choice = SEQFEAT_BOND;
						tmp->data.ptrvalue = sfp;
						sfap->pept = tie_next(sfap->pept, tmp);
					}
				}
			}
		}
	}
}

static void RemovePeptideImpFeats (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	BioseqPtr 		bsp;
	BioseqSetPtr	bssp;
	SeqAnnotPtr		sap, annot, nextsap, PNTR prevsap;
	SeqFeatPtr		sfp, sfpnext;
	ImpFeatPtr 		ifp;
	
	if (IS_Bioseq(sep)) {
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		annot = bsp->annot;
		prevsap = (SeqAnnotPtr PNTR) &(bsp->annot);
	} else {
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		annot = bssp->annot;
		prevsap = (SeqAnnotPtr PNTR) &(bssp->annot);
	}
	sap = annot;
	while (sap != NULL) {
		nextsap = sap->next;
		if (sap->type == 1) {
			for (sfp = sap->data; sfp != NULL; sfp = sfpnext) {
				sfpnext = sfp->next;
				if (sfp->data.choice != SEQFEAT_IMP) {
					continue;
				}
				ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
				if (sfp->comment && 
					StringCmp(sfp->comment, "FeatureToBeDeleted") == 0) {
					sap->data = remove_feat(sap->data, sfp);
				}
			}
		}
		if (sap->data == NULL) {
			*(prevsap) = sap->next;
			sap->next = NULL;
			SeqAnnotFree (sap);
		} else {
			prevsap = (SeqAnnotPtr PNTR) &(sap->next);
		}
		sap = nextsap;
	}
}

static void CleanUpTmpFeatStruct(SeqFeatArrPtr sfap)
{
	ValNodePtr tmp, tmpnext;
	
	for (tmp = sfap->cds; tmp; tmp = tmpnext) {
		tmpnext = tmp->next;
		MemFree(tmp);
	}
	for (tmp = sfap->pept; tmp; tmp = tmpnext) {
		tmpnext = tmp->next;
		MemFree(tmp);
	}
}

void EntryChangeImpFeatToProt (SeqEntryPtr sep)
{

	SeqFeatArr sfa;
	
	if (sep == NULL)
		return;
	MemSet ((Pointer) (&sfa), 0, sizeof (SeqFeatArr));
	SeqEntryExplore(sep, &sfa, GetCdRegionsWithPeptides);
	ImpFeatToProtRef(sfa);
	SeqEntryExplore(sep, NULL, RemovePeptideImpFeats);
	CleanUpTmpFeatStruct(&sfa);
	return;
}

/* functions moved from Sequin */

static void NormalizeAuthors (AuthListPtr alp)

{
  AuthorPtr    ap;
  Char         ch;
  CharPtr      initials;
  Int2         j;
  Int2         k;
  size_t       len;
  ValNodePtr   names;
  NameStdPtr   nsp;
  CharPtr      periods;
  PersonIdPtr  pid;

  if (alp == NULL || alp->choice != 1) return;
  for (names = alp->names; names != NULL; names = names->next) {
    ap = names->data.ptrvalue;
    if (ap != NULL) {
      pid = ap->name;
      if (pid != NULL && pid->choice == 2) {
        nsp = pid->data;
        if (nsp != NULL && nsp->names [4] != NULL) {
          initials = nsp->names [4];
          len = MAX ((size_t) (StringLen (initials) * 2 + 4), (size_t) 64);
          periods = MemNew (len);
          if (periods == NULL) return;
          periods [0] = '\0';
          j = 0;
          k = 0;
          ch = initials [j];
          while (ch != '\0') {
            if (ch == '-') {
              periods [k] = ch;
              k++;
              j++;
              ch = initials [j];
            } else if (ch == '.') {
              j++;
              ch = initials [j];
            } else if (ch == ' ') {
              j++;
              ch = initials [j];
            } else {
              periods [k] = ch;
              k++;
              j++;
              ch = initials [j];
              periods [k] = '.';
              k++;
            }
          }
          periods [k] = '\0';
          nsp->names [4] = MemFree (nsp->names [4]);
          nsp->names [4] = StringSave (periods);
          MemFree (periods);
        }
      }
    }
  }
}

static void NormalizeAPub (ValNodePtr vnp)

{
  AuthListPtr  alp;
  CitArtPtr    cap;
  CitBookPtr   cbp;
  CitGenPtr    cgp;
  CitPatPtr    cpp;
  CitSubPtr    csp;
  ImprintPtr   imp;

  if (vnp == NULL) return;
  if (vnp->choice == PUB_PMid || vnp->choice == PUB_Muid) return;
  if (vnp->data.ptrvalue == NULL) return;
  switch (vnp->choice) {
    case PUB_Gen :
      cgp = (CitGenPtr) vnp->data.ptrvalue;
      NormalizeAuthors (cgp->authors);
      break;
    case PUB_Sub :
      csp = (CitSubPtr) vnp->data.ptrvalue;
      NormalizeAuthors (csp->authors);
      alp = csp->authors;
      imp = csp->imp;
      if (alp != NULL && alp->affil == NULL && imp != NULL && imp->pub != NULL) {
        alp->affil = imp->pub;
        imp->pub = NULL;
      }
      if (csp->date == NULL && imp != NULL && imp->date != NULL) {
        csp->date = imp->date;
        imp->date = NULL;
      }
      if (imp != NULL && imp->pub == NULL) {
        csp->imp = ImprintFree (csp->imp);
      }
      break;
    case PUB_Article :
      cap = (CitArtPtr) vnp->data.ptrvalue;
      NormalizeAuthors (cap->authors);
      break;
    case PUB_Book :
      cbp = (CitBookPtr) vnp->data.ptrvalue;
      NormalizeAuthors (cbp->authors);
      break;
    case PUB_Man :
      cbp = (CitBookPtr) vnp->data.ptrvalue;
      if (cbp->othertype == 2 && cbp->let_type == 3) {
        NormalizeAuthors (cbp->authors);
      }
      break;
    case PUB_Patent :
      cpp = (CitPatPtr) vnp->data.ptrvalue;
      NormalizeAuthors (cpp->authors);
      NormalizeAuthors (cpp->applicants);
      NormalizeAuthors (cpp->assignees);
      break;
    default :
      break;
  }
}

static Boolean NormalizePeriods (GatherContextPtr gcp)

{
  PubdescPtr  pdp;
  ValNodePtr  sdp;
  SeqFeatPtr  sfp;
  ValNodePtr  vnp;

  if (gcp == NULL) return TRUE;
  pdp = NULL;
  if (gcp->thistype == OBJ_SEQFEAT) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp != NULL && sfp->data.choice == SEQFEAT_PUB) {
      pdp = (PubdescPtr) sfp->data.value.ptrvalue;
    }
  } else if (gcp->thistype == OBJ_SEQDESC) {
    sdp = (ValNodePtr) gcp->thisitem;
    if (sdp != NULL && sdp->choice == Seq_descr_pub) {
      pdp = (PubdescPtr) sdp->data.ptrvalue;
    }
  }
  if (pdp == NULL) return TRUE;
  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    NormalizeAPub (vnp);
  }
  return TRUE;
}

void NormalizePeriodsOnInitials (SeqEntryPtr sep)

{
  GatherScope   gs;

  if (sep == NULL) return;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet ((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.ignore[OBJ_SEQDESC] = FALSE;
  GatherSeqEntry (sep, NULL, NormalizePeriods, &gs);
}

static Boolean NormalizeRnas (GatherContextPtr gcp)

{
  GBQualPtr       gbqual;
  GBQualPtr       nextqual;
  GBQualPtr PNTR  prevqual;
  RnaRefPtr       rrp;
  SeqFeatPtr      sfp;
  CharPtr         str;

  if (gcp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->data.choice != SEQFEAT_RNA) return TRUE;
  rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
  if (rrp == NULL) return TRUE;
  if (rrp->type == 0) {
    rrp->type = 255;
  }
  if (rrp->ext.choice != 0 && rrp->ext.choice != 1) return TRUE;
  if (! TASNStringHasNoText (rrp->ext.value.ptrvalue)) return TRUE;
  str = NULL;
  gbqual = sfp->qual;
  prevqual = (GBQualPtr PNTR) &(sfp->qual);
  while (gbqual != NULL) {
    nextqual = gbqual->next;
    if (StringICmp (gbqual->qual, "product") == 0) {
      str = StringSave (gbqual->val);
      *(prevqual) = gbqual->next;
      gbqual->next = NULL;
      gbqual->qual = MemFree (gbqual->qual);
      gbqual->val = MemFree (gbqual->val);
      GBQualFree (gbqual);
    } else {
      prevqual = (GBQualPtr PNTR) &(gbqual->next);
    }
    gbqual = nextqual;
  }
  if (str == NULL) {
    gbqual = sfp->qual;
    prevqual = (GBQualPtr PNTR) &(sfp->qual);
    while (gbqual != NULL) {
      nextqual = gbqual->next;
      if (StringICmp (gbqual->qual, "standard_name") == 0) {
        str = StringSave (gbqual->val);
        *(prevqual) = gbqual->next;
        gbqual->next = NULL;
        gbqual->qual = MemFree (gbqual->qual);
        gbqual->val = MemFree (gbqual->val);
        GBQualFree (gbqual);
      } else {
        prevqual = (GBQualPtr PNTR) &(gbqual->next);
      }
      gbqual = nextqual;
    }
  }
  if (rrp->ext.choice == 1 && rrp->ext.value.ptrvalue != NULL) {
    rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
  }
  if (rrp->ext.choice == 0 || rrp->ext.choice == 1) {
    rrp->ext.choice = 1;
    rrp->ext.value.ptrvalue = str;
    str = NULL;
  }
  MemFree (str);
  return TRUE;
}

void MoveRnaGBQualProductToName (SeqEntryPtr sep)

{
  GatherScope   gs;

  if (sep == NULL) return;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet ((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherSeqEntry (sep, NULL, NormalizeRnas, &gs);
}

static Boolean NormalizeProts (GatherContextPtr gcp)

{
  GBQualPtr       gbqual;
  GBQualPtr       nextqual;
  GBQualPtr PNTR  prevqual;
  ProtRefPtr      prp;
  SeqFeatPtr      sfp;
  CharPtr         str;
  ValNodePtr      vnp;

  if (gcp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->data.choice != SEQFEAT_PROT) return TRUE;
  prp = (ProtRefPtr) sfp->data.value.ptrvalue;
  if (prp == NULL) return TRUE;
  vnp = prp->name;
  if (vnp == NULL || TASNStringHasNoText (vnp->data.ptrvalue)) {
    str = NULL;
    gbqual = sfp->qual;
    prevqual = (GBQualPtr PNTR) &(sfp->qual);
    while (gbqual != NULL) {
      nextqual = gbqual->next;
      if (StringICmp (gbqual->qual, "product") == 0) {
        str = StringSave (gbqual->val);
        *(prevqual) = gbqual->next;
        gbqual->next = NULL;
        gbqual->qual = MemFree (gbqual->qual);
        gbqual->val = MemFree (gbqual->val);
        GBQualFree (gbqual);
      } else {
        prevqual = (GBQualPtr PNTR) &(gbqual->next);
      }
      gbqual = nextqual;
    }
    if (vnp == NULL) {
      vnp = ValNodeNew (NULL);
      prp->name = vnp;
    }
    vnp = prp->name;
    if (vnp != NULL) {
      vnp->data.ptrvalue = str;
      str = NULL;
    }
    MemFree (str);
  }
  vnp = prp->name;
  if (vnp == NULL || TASNStringHasNoText (vnp->data.ptrvalue)) return TRUE;
  if (prp->desc == NULL) return TRUE;
  if (StringICmp (vnp->data.ptrvalue, prp->desc) == 0) {
    prp->desc = MemFree (prp->desc);
  }
  return TRUE;
}

void MoveProtGBQualProductToName (SeqEntryPtr sep)

{
  GatherScope   gs;

  if (sep == NULL) return;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet ((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherSeqEntry (sep, NULL, NormalizeProts, &gs);
}

static Boolean NormalizeCds (GatherContextPtr gcp)

{
  BioseqContextPtr  bcp;
  BioseqPtr         bsp;
  GBQualPtr         gbqual;
  GBQualPtr         nextqual;
  GBQualPtr PNTR    prevqual;
  ProtRefPtr        prp;
  SeqEntryPtr       sep;
  SeqFeatPtr        sfp;
  SeqFeatPtr        sfp2;
  CharPtr           str;
  ValNodePtr        vnp;

  if (gcp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION) return TRUE;
  bsp = BioseqFind (SeqLocId (sfp->product));
  if (bsp == NULL) return TRUE;
  str = NULL;
  gbqual = sfp->qual;
  prevqual = (GBQualPtr PNTR) &(sfp->qual);
  while (gbqual != NULL) {
    nextqual = gbqual->next;
    if (StringICmp (gbqual->qual, "product") == 0) {
      str = StringSave (gbqual->val);
      *(prevqual) = gbqual->next;
      gbqual->next = NULL;
      gbqual->qual = MemFree (gbqual->qual);
      gbqual->val = MemFree (gbqual->val);
      GBQualFree (gbqual);
    } else {
      prevqual = (GBQualPtr PNTR) &(gbqual->next);
    }
    gbqual = nextqual;
  }
  if (str == NULL) return TRUE;

  sfp2 = NULL;
  bcp = BioseqContextNew (bsp);
  sfp2 = BioseqContextGetSeqFeat (bcp, SEQFEAT_PROT, NULL, NULL, 0);
  BioseqContextFree (bcp);
  if (sfp2 == NULL) {
    prp = CreateNewProtRef (str, NULL, NULL, NULL);
    if (prp != NULL) {
      sep = SeqMgrGetSeqEntryForData (bsp);
      if (sep != NULL) {
        sfp = CreateNewFeature (sep, NULL, SEQFEAT_PROT, NULL);
        if (sfp != NULL) {
          sfp->data.value.ptrvalue = (Pointer) prp;
        }
      }
    }
    return TRUE;
  }

  prp = (ProtRefPtr) sfp2->data.value.ptrvalue;
  if (prp == NULL) return TRUE;
  vnp = prp->name;
  if (vnp != NULL && (! TASNStringHasNoText (vnp->data.ptrvalue))) return TRUE;
  if (vnp == NULL) {
    vnp = ValNodeNew (NULL);
    prp->name = vnp;
  }
  vnp = prp->name;
  if (vnp != NULL) {
    vnp->data.ptrvalue = str;
    str = NULL;
  }
  MemFree (str);
  return TRUE;
}

void MoveCdsGBQualProductToName (SeqEntryPtr sep)

{
  GatherScope   gs;

  if (sep == NULL) return;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet ((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherSeqEntry (sep, NULL, NormalizeCds, &gs);
}

static Boolean NormalizeFeatGBQuals (GatherContextPtr gcp)

{
  GBQualPtr       gbqual;
  size_t          len;
  GBQualPtr       nextqual;
  GBQualPtr PNTR  prevqual;
  SeqFeatPtr      sfp;
  CharPtr         str;

  if (gcp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL) return TRUE;
  gbqual = sfp->qual;
  prevqual = (GBQualPtr PNTR) &(sfp->qual);
  while (gbqual != NULL) {
    nextqual = gbqual->next;
    if (StringICmp (gbqual->qual, "partial") == 0) {
      *(prevqual) = gbqual->next;
      gbqual->next = NULL;
      gbqual->qual = MemFree (gbqual->qual);
      gbqual->val = MemFree (gbqual->val);
      GBQualFree (gbqual);
      sfp->partial = TRUE;
    } else if (StringICmp (gbqual->qual, "evidence") == 0) {
      if (StringICmp (gbqual->val, "experimental") == 0) {
        sfp->exp_ev = 1;
      } else if (StringICmp (gbqual->val, "not_experimental") == 0) {
        sfp->exp_ev = 2;
      }
      *(prevqual) = gbqual->next;
      gbqual->next = NULL;
      gbqual->qual = MemFree (gbqual->qual);
      gbqual->val = MemFree (gbqual->val);
      GBQualFree (gbqual);
    } else if (StringICmp (gbqual->qual, "exception") == 0) {
      sfp->excpt = TRUE;
    } else if (StringICmp (gbqual->qual, "note") == 0) {
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
  return TRUE;
}

void MoveFeatGBQualsToFields (SeqEntryPtr sep)

{
  GatherScope   gs;

  if (sep == NULL) return;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet ((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherSeqEntry (sep, NULL, NormalizeFeatGBQuals, &gs);
}

static void StripTitleFromProteinProducts (SeqEntryPtr sep, Pointer mydata,
                                           Int4 index, Int2 indent)

{
  BioseqPtr   bsp;
  SeqIdPtr    sip;
  ValNodePtr  vnp;

  if (sep == NULL) return;
  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;
  if (! ISA_aa (bsp->mol)) return;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_OTHER) return;
  }
  vnp = ValNodeExtract (&(bsp->descr), Seq_descr_title);
  if (vnp == NULL) return;
  ValNodeFreeData (vnp);
}

void StripTitleFromProtsInNucProts (SeqEntryPtr sep)

{
  BioseqSetPtr  bssp;

  if (sep == NULL) return;
  if (! IS_Bioseq_set (sep)) return;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp == NULL) return;
  if (bssp->_class == 7 ||
      (bssp->_class >= 13 && bssp->_class <= 16) ||
      bssp->_class == BioseqseqSet_class_wgs_set) {
    for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
      StripTitleFromProtsInNucProts (sep);
    }
    return;
  }
  if (bssp->_class != BioseqseqSet_class_nuc_prot) return;
  SeqEntryExplore (sep, NULL, StripTitleFromProteinProducts);
}

static void CleanOrgModList (OrgModPtr PNTR ompp)

{
  OrgModPtr       next;
  OrgModPtr       omp;
  OrgModPtr PNTR  prev;

  if (ompp == NULL) return;
  prev = ompp;
  omp = *ompp;
  while (omp != NULL) {
    next = omp->next;
    CleanVisString (&(omp->subname));
    CleanVisString (&(omp->attrib));
    if (TASNStringHasNoText (omp->subname)) {
      *prev = omp->next;
      omp->next = NULL;
      OrgModFree (omp);
    } else {
      prev = &(omp->next);
    }
    omp = next;
  }
}

static void CleanSubSourceList (SubSourcePtr PNTR sspp)

{
  SubSourcePtr       next;
  SubSourcePtr PNTR  prev;
  SubSourcePtr       ssp;

  if (sspp == NULL) return;
  prev = sspp;
  ssp = *sspp;
  while (ssp != NULL) {
    next = ssp->next;
    if (ssp->subtype != SUBSRC_germline &&
        ssp->subtype != SUBSRC_rearranged &&
        ssp->subtype != SUBSRC_transgenic &&
        ssp->subtype != SUBSRC_environmental_sample) {
      CleanVisString (&(ssp->name));
    }
    CleanVisString (&(ssp->attrib));
    if (TASNStringHasNoText (ssp->name) &&
        ssp->subtype != SUBSRC_germline &&
        ssp->subtype != SUBSRC_rearranged &&
        ssp->subtype != SUBSRC_transgenic &&
        ssp->subtype != SUBSRC_environmental_sample) {
      *prev = ssp->next;
      ssp->next = NULL;
      SubSourceFree (ssp);
    } else {
      prev = &(ssp->next);
    }
    ssp = next;
  }
}

static void CleanFeatStrings (SeqFeatPtr sfp)

{
  BioSourcePtr  biop;
  GeneRefPtr    grp;
  ImpFeatPtr    ifp;
  Boolean       noSfpDataPtrValue;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  PubdescPtr    pdp;
  ProtRefPtr    prp;
  RnaRefPtr     rrp;

  if (sfp == NULL) return;
  CleanVisString (&sfp->comment);
  CleanVisString (&sfp->title);
  noSfpDataPtrValue = FALSE;
  switch (sfp->data.choice) {
    case SEQFEAT_BOND :
    case SEQFEAT_SITE :
    case SEQFEAT_PSEC_STR :
    case SEQFEAT_COMMENT:
      noSfpDataPtrValue = TRUE;
      break;
    default :
      break;
  }
  if (noSfpDataPtrValue) return;
  if (sfp->data.value.ptrvalue == NULL) return;
  orp = NULL;
  switch (sfp->data.choice) {
    case SEQFEAT_GENE :
      grp = (GeneRefPtr) sfp->data.value.ptrvalue;
      CleanVisString (&(grp->locus));
      CleanVisString (&(grp->allele));
      CleanVisString (&(grp->desc));
      CleanVisString (&(grp->maploc));
      CleanVisString (&(grp->locus_tag));
      CleanVisStringList (&(grp->syn));
      break;
    case SEQFEAT_ORG :
      orp = (OrgRefPtr) sfp->data.value.ptrvalue;
      break;
    case SEQFEAT_CDREGION :
      break;
    case SEQFEAT_PROT :
      prp = (ProtRefPtr) sfp->data.value.ptrvalue;
      CleanVisString (&(prp->desc));
      CleanVisStringList (&(prp->name));
      CleanVisStringList (&(prp->ec));
      CleanVisStringList (&(prp->activity));
      break;
    case SEQFEAT_RNA :
      rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
      if (rrp->ext.choice == 1) {
        CleanVisString ((CharPtr PNTR) &(rrp->ext.value.ptrvalue));
        if (rrp->ext.value.ptrvalue == NULL) {
          rrp->ext.choice = 0;
        }
      }
      break;
    case SEQFEAT_PUB :
      pdp = (PubdescPtr) sfp->data.value.ptrvalue;
      CleanVisString (&(pdp->comment));
      break;
    case SEQFEAT_SEQ :
      break;
    case SEQFEAT_IMP :
      ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
      CleanVisString (&(ifp->key));
      CleanVisString (&(ifp->loc));
      CleanVisString (&(ifp->descr));
      break;
    case SEQFEAT_REGION :
      CleanVisString ((CharPtr PNTR) &(sfp->data.value.ptrvalue));
      if (sfp->data.value.ptrvalue == NULL) {
        sfp->data.choice = SEQFEAT_COMMENT;
      }
      break;
    case SEQFEAT_COMMENT :
      break;
    case SEQFEAT_BOND :
      break;
    case SEQFEAT_SITE :
      break;
    case SEQFEAT_RSITE :
      break;
    case SEQFEAT_USER :
      break;
    case SEQFEAT_TXINIT :
      break;
    case SEQFEAT_NUM :
      break;
    case SEQFEAT_PSEC_STR :
      break;
    case SEQFEAT_NON_STD_RESIDUE :
      break;
    case SEQFEAT_HET :
      break;
    case SEQFEAT_BIOSRC :
      biop = (BioSourcePtr) sfp->data.value.ptrvalue;
      orp = biop->org;
      CleanSubSourceList (&(biop->subtype));
      break;
    default :
      break;
  }
  if (orp != NULL) {
    CleanVisString (&(orp->taxname));
    CleanVisString (&(orp->common));
    CleanVisStringList (&(orp->mod));
    CleanVisStringList (&(orp->syn));
    onp = orp->orgname;
    while (onp != NULL) {
      CleanVisString (&(onp->attrib));
      CleanVisString (&(onp->lineage));
      CleanVisString (&(onp->div));
      CleanOrgModList (&(onp->mod));
      onp = onp->next;
    }
  }
}

static Boolean OnlyPunctuation (CharPtr str)

{
  Uchar  ch;	/* to use 8bit characters in multibyte languages */

  if (str != NULL) {
    ch = *str;
    while (ch != '\0') {
      if (ch > ' ' && ch != '.' && ch != ',' && ch != '~' && ch != ';') {
        return FALSE;
      }
      str++;
      ch = *str;
    }
  }
  return TRUE;
}

static Boolean IsOnlinePub (PubdescPtr pdp)

{
  CitGenPtr   cgp;
  ValNodePtr  vnp;

  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == PUB_Gen) {
      cgp = (CitGenPtr) vnp->data.ptrvalue;
      if (cgp != NULL) {
        if (StringNICmp (cgp->cit, "Online Publication", 18) == 0) {
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static void CleanDescStrings (ValNodePtr sdp)

{
  BioSourcePtr  biop;
  GBBlockPtr    gbp;
  Boolean       noSdpDataPtrValue;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  PubdescPtr    pdp;

  if (sdp == NULL) return;
  noSdpDataPtrValue = FALSE;
  switch (sdp->choice) {
    case Seq_descr_mol_type :
    case Seq_descr_method :
      noSdpDataPtrValue = TRUE;
      break;
    default :
      break;
  }
  if (noSdpDataPtrValue) return;
  if (sdp->data.ptrvalue == NULL) return;
  orp = NULL;
  switch (sdp->choice) {
    case Seq_descr_mol_type :
      break;
    case Seq_descr_modif :
      break;
    case Seq_descr_method :
      break;
    case Seq_descr_name :
      CleanVisString ((CharPtr PNTR) &sdp->data.ptrvalue);
      break;
    case Seq_descr_title :
      CleanVisString ((CharPtr PNTR) &sdp->data.ptrvalue);
      break;
    case Seq_descr_org :
      orp = (OrgRefPtr) sdp->data.ptrvalue;
      break;
    case Seq_descr_comment :
      CleanVisStringJunk ((CharPtr PNTR) &sdp->data.ptrvalue);
      if (OnlyPunctuation ((CharPtr) sdp->data.ptrvalue)) {
        sdp->data.ptrvalue = MemFree (sdp->data.ptrvalue);
      }
      break;
    case Seq_descr_num :
      break;
    case Seq_descr_maploc :
      break;
    case Seq_descr_pir :
      break;
    case Seq_descr_genbank :
      gbp = (GBBlockPtr) sdp->data.ptrvalue;
      CleanVisStringList (&(gbp->extra_accessions));
      CleanVisStringList (&(gbp->keywords));
      CleanVisString (&(gbp->source));
      CleanVisString (&(gbp->origin));
      CleanVisString (&(gbp->date));
      CleanVisString (&(gbp->div));
      CleanVisString (&(gbp->taxonomy));
      break;
    case Seq_descr_pub :
      pdp = (PubdescPtr) sdp->data.ptrvalue;
      if (IsOnlinePub (pdp)) {
        TrimSpacesAroundString (pdp->comment);
        if (StringHasNoText (pdp->comment)) {
          pdp->comment = MemFree (pdp->comment);
        }
      } else {
        CleanVisString (&(pdp->comment));
      }
      break;
    case Seq_descr_region :
      CleanVisString ((CharPtr PNTR) &sdp->data.ptrvalue);
      break;
    case Seq_descr_user :
      break;
    case Seq_descr_sp :
      break;
    case Seq_descr_dbxref :
      break;
    case Seq_descr_embl :
      break;
    case Seq_descr_create_date :
      break;
    case Seq_descr_update_date :
      break;
    case Seq_descr_prf :
      break;
    case Seq_descr_pdb :
      break;
    case Seq_descr_het :
      break;
    case Seq_descr_source :
      biop = (BioSourcePtr) sdp->data.ptrvalue;
      orp = biop->org;
      CleanSubSourceList (&(biop->subtype));
      break;
    case Seq_descr_molinfo :
      break;
    default :
      break;
  }
  if (orp != NULL) {
    CleanVisString (&(orp->taxname));
    CleanVisString (&(orp->common));
    CleanVisStringList (&(orp->mod));
    CleanVisStringList (&(orp->syn));
    onp = orp->orgname;
    while (onp != NULL) {
      CleanVisString (&(onp->attrib));
      CleanVisString (&(onp->lineage));
      CleanVisString (&(onp->div));
      CleanOrgModList (&(onp->mod));
      onp = onp->next;
    }
  }
}

void GetRidOfEmptyFeatsDescCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  SeqAnnotPtr   nextsap;
  SeqDescrPtr   nextsdp;
  SeqFeatPtr    nextsfp;
  Pointer PNTR  prevsap;
  Pointer PNTR  prevsdp;
  Pointer PNTR  prevsfp;
  SeqAnnotPtr   sap;
  SeqDescrPtr   sdp;
  SeqFeatPtr    sfp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  sap = NULL;
  sdp = NULL;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
    prevsap = (Pointer PNTR) &(bsp->annot);
    sdp = bsp->descr;
    prevsdp = (Pointer PNTR) &(bsp->descr);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
    prevsap = (Pointer PNTR) &(bssp->annot);
    sdp = bssp->descr;
    prevsdp = (Pointer PNTR) &(bssp->descr);
  } else return;
  while (sap != NULL) {
    nextsap = sap->next;
    if (sap->type == 1 && sap->data != NULL) {
      sfp = (SeqFeatPtr) sap->data;
      prevsfp = (Pointer PNTR) &(sap->data);
      while (sfp != NULL) {
        nextsfp = sfp->next;
        CleanFeatStrings (sfp);
        if (sfp->data.choice != SEQFEAT_BOND &&
            sfp->data.choice != SEQFEAT_SITE &&
            sfp->data.choice != SEQFEAT_PSEC_STR &&
            sfp->data.choice != SEQFEAT_COMMENT &&
            sfp->data.value.ptrvalue == NULL) {
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
    CleanDescStrings (sdp);
    if (sdp->choice != Seq_descr_mol_type &&
        sdp->choice != Seq_descr_method &&
        sdp->data.ptrvalue == NULL) {
      *(prevsdp) = sdp->next;
      sdp->next = NULL;
      SeqDescrFree (sdp);
    } else {
      prevsdp = (Pointer PNTR) &(sdp->next);
    }
    sdp = nextsdp;
  }
}

/* move_cds from Serge Bazhin, modified by Kans */

typedef struct bool_bioseq_set {
    Uint2        found;
    BioseqSetPtr bssp;
    Boolean      doPseudo;
} BoolBioseqSet, PNTR BoolBioseqSetPtr;

/**********************************************************/
static void put_cds_on_nps (BioseqSetPtr bssp, SeqFeatPtr sfp)

{
  SeqFeatPtr   prev;
  SeqAnnotPtr  sap;

  if (bssp == NULL || sfp == NULL) return;
  sap = bssp->annot;
  while (sap != NULL && (sap->name != NULL || sap->desc != NULL || sap->type != 1)) {
    sap = sap->next;
  }
  if (sap == NULL) {
    sap = SeqAnnotNew ();
    if (sap != NULL) {
      sap->type = 1;
      sap->next = bssp->annot;
      bssp->annot = sap;
    }
  }
  sap = bssp->annot;
  if (sap == NULL) return;
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

/**********************************************************
 *
 *   void move_cds_within_nucprot(sep, bbsp)
 *
 *      Runs through nuc-prot Bioseq-set components, looks for cdregions
 *   its Seq-entries, and moves their pointers to nuc-prot
 *   Bioseq-set.
 *
 **********************************************************/
static void move_cds_within_nucprot(SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BoolBioseqSetPtr  bbsp;
  BioseqPtr         bsp;
  BioseqSetPtr      bssp;
  SeqAnnotPtr       nextsap;
  SeqFeatPtr        nextsfp;
  Pointer PNTR      prevsap;
  Pointer PNTR      prevsfp;
  SeqAnnotPtr       sap;
  SeqFeatPtr        sfp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  bbsp = (BoolBioseqSetPtr) mydata;
  if (bbsp == NULL) return;
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
        if (sfp->data.choice == SEQFEAT_CDREGION && (! sfp->pseudo) &&
            (sfp->product != NULL || SeqLocLen (sfp->location) >= 6)) {
          *(prevsfp) = sfp->next;
          sfp->next = NULL;
          bbsp->found++;
          ErrPostEx(SEV_WARNING, 0, 0, "Moving cdregion from na Bioseq.annot to Bioseq-set.annot.");
          put_cds_on_nps (bbsp->bssp, sfp);
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

/**********************************************************
 *
 *   Uint2 move_cds(sep)
 *
 *      Moves cdregion features to nuc-prot set level
 *
 **********************************************************/
Uint2 move_cds_ex (SeqEntryPtr sep, Boolean doPseudo)
{
    BioseqSetPtr     bssp;
    Uint2            found;
    BoolBioseqSet    bbsp;

	if (sep == NULL) return 0;
    if (! IS_Bioseq_set (sep)) return 0;
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL) return 0;
    if (bssp->_class == BioseqseqSet_class_genbank ||
        (bssp->_class >= BioseqseqSet_class_mut_set && bssp->_class <= BioseqseqSet_class_eco_set) ||
        bssp->_class == BioseqseqSet_class_gen_prod_set ||
        bssp->_class == BioseqseqSet_class_wgs_set) {
        found = 0;
    	for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
    	  found += move_cds (sep);
    	}
    	return found;
    }
    if (bssp->_class != 1) return 0;
    bbsp.found = 0;
    bbsp.bssp = bssp;
    bbsp.doPseudo = doPseudo;
    for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
    	SeqEntryExplore (sep, (Pointer) &bbsp, move_cds_within_nucprot);
    }
    return(bbsp.found);
}

Uint2 move_cds(SeqEntryPtr sep)
{
  return move_cds_ex (sep, TRUE);
}

static Boolean MoveDbxrefs (GatherContextPtr gcp)

{
	GBQualPtr       qual;
	GBQualPtr       nextqual;
	SeqFeatPtr      sfp;
	DbtagPtr 		db;
	ObjectIdPtr 	oip;
	ValNodePtr		vnp;
	CharPtr			tag, value, p;

	if (gcp == NULL) return TRUE;
	if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
	sfp = (SeqFeatPtr) gcp->thisitem;
	for (qual=sfp->qual; qual; qual = nextqual) {
		nextqual = qual->next;
		if (StringICmp (qual->qual, "db_xref") == 0) {
			vnp = ValNodeNew(NULL);
			db = DbtagNew();
			vnp->data.ptrvalue = db;
			tag = qual->val;
			if ((p = StrChr(tag, ':')) != NULL) {
				value = p+1;
				*p = '\0';
				db->db = StringSave (tag);
				oip = ObjectIdNew();
				oip->str = StringSave (value);
				db->tag = oip;
			} else {
				db->db = StringSave ("?");
				oip = ObjectIdNew();
				oip->str = StringSave (tag);
				db->tag = oip;
			}
			sfp->dbxref = tie_next(sfp->dbxref, vnp);
			sfp->qual = remove_qual(sfp->qual, qual);
		}
	}
  return TRUE;
}

Boolean SeqEntryMoveDbxrefs (SeqEntryPtr sep)

{
  GatherScope   gs;

  if (sep == NULL) return FALSE;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet ((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherSeqEntry (sep, NULL, MoveDbxrefs, &gs);
  return TRUE;
}
