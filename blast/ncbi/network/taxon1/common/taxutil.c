#include <utilpub.h>
#include <taxutil.h>
#include <taxuerr.h>
#include <utilpars.h>

static char *this_module ="taxutil";

#ifdef THIS_MODULE
#undef THIS_MODULE
#endif
#define THIS_MODULE this_module

static char *this_file = "taxutil.c";

#ifdef THIS_FILE
#undef THIS_FILE
#endif
#define THIS_FILE this_file


static TaxBlk cash[MAXIDLIST];

static
GeneticCodeListPtr
GeneticCodeListNew(void)
{

	return (GeneticCodeListPtr) MemNew(sizeof(GeneticCodeList));

}

void tax_init(void)
{
	int i;
	
	for (i = 0; i < MAXIDLIST; i++) {
		cash[i].hits = 0;
	}

}

Int4 taxname_replace(CharPtr iname, Taxon1DataPtr new)
{
	Int4 hitmin, imin, i;
	
	hitmin = cash[0].hits;
	imin = 0;
	for (i = 1; i < MAXIDLIST; i++) {
		if (cash[i].hits < hitmin) {
			hitmin = cash[i].hits;
			imin = i;
		}
	}	
	cash[imin].hits = 1;	
	if(cash[imin].tax != NULL)
		Taxon1DataFree(cash[imin].tax);
	cash[imin].tax = new;
	
	return imin;
	 
}

Int4 taxname_match(CharPtr orgname, Boolean err)
{
   Int4 			   i;
   CharPtr             taxstr = NULL;
   Int4					tid;
   Taxon1DataPtr	   new;
   
      ErrSetOptFlags(EO_SHOW_CODES); 
      ErrSetOptFlags(EO_MSG_MSGTEXT);         
	if (orgname == NULL) {
		return -1;
	}
	for (i=0; i < MAXIDLIST; i++) {
		if (cash[i].tax != NULL) {
			if (strcmp(orgname, cash[i].tax->org->taxname) == 0) {
				cash[i].hits++;
				return i; 
			}
		}
	}
	tid = tax1_getTaxIdByName(orgname);
    if (tid == 0 ) {
    	if(err)
			ErrPostEx(SEV_WARNING, ERR_ORGANISM_TaxNameNotFound,
				   "Taxon Id not found for [%s]", orgname);
			return -1;
     } else if (tid < 0) {
    	if(err)
			ErrPostEx(SEV_WARNING, ERR_ORGANISM_TaxIdNotUnique,
							"Not an unique Taxonomic Id for %s", orgname);
			return -1;
     } else {
		new = tax1_getbyid(tid);
		if (new == NULL) {
    		if(err)
			ErrPostEx(SEV_WARNING, ERR_ORGANISM_TaxNameNotFound,
				   "Taxon Id not found for [%s]", orgname);
			return -1;
		} 
    	if (new->is_species_level == FALSE) {
    		if(err)
				ErrPostEx(SEV_WARNING,ERR_ORGANISM_TaxIdNotSpecLevel,
	   			"Taxarch hit is not on species level (%s)", orgname);
    	}
		i = taxname_replace(orgname, new);
		return i; 
     }
}

/****************************************************************************
*	get OrgRef from taxserver by name 
*	doesn't use the input OrgRef for the query (see check_org_ref())
*
****************************************************************************/
OrgRefPtr get_tax_org(CharPtr name)
{
	Int4	i;
	
	i = taxname_match(name, FALSE);
	if (i != -1) {
		return cash[i].tax->org;
	}
	return NULL;
}

CharPtr get_lineage(CharPtr name)
{
	Int4	i;
	OrgRefPtr orp;
	OrgNamePtr onp;
	
	i = taxname_match(name, TRUE);
	if (i != -1) {
		orp = cash[i].tax->org;
		if (orp) {
			onp = orp->orgname;
		}
		return Nlm_StringSave(onp->lineage);  
	}
	return NULL;
}

GeneticCodeListPtr get_gcode(CharPtr name)
{
	Int4	i;
	GeneticCodeListPtr gclp;
	OrgRefPtr orp;
	OrgNamePtr onp;
	
	i = taxname_match(name, FALSE);
	if (i != -1) {
		orp = cash[i].tax->org;
		if (orp) {
			onp = orp->orgname;
		}
		if (onp) {
			gclp = GeneticCodeListNew();
			gclp->genomic = onp->gcode;
			gclp->mitochondrial = onp->mgcode;
			return gclp;
		}
	}
	return NULL;
} 

/**************************************************************************
*  CleanTail:
*  -- delete any tailing ' ', '\n', '\\', ',', ';', '~', '.', ':' characters
****************************************************************************/
static void CleanTail(CharPtr str)
{
   Int4      len;
   CharPtr   q;

	if (str == NULL) {
		return;
	}
	len = StringLen(str);

	for (q = str + len-1; q >= str && len > 0; q--, len--) {
		if (*q == ' ' || *q == '\n' || *q == '\\' || *q == ',' || *q == ';'
                           || *q == '~' || *q == '.' || *q == ':') {
			*q = '\0';
			continue;
		}
		break;
	}

}

static ValNodePtr tie_prev(ValNodePtr head, ValNodePtr prev)
{
 	prev->next = head;
 	return prev;
}

/*-------------------- TokenString() ---------------------*/
/****************************************************************************
*  TokenString:
*  -- parsing string "str" by delimiter or tab key, blank
*  -- parsing stop at newline ('\n') or end of string ('\0')
****************************************************************************/
static ValNodePtr TokenString(CharPtr str, Char delimiter)
{
   CharPtr     bptr, ptr;
   Int2        len;
   ValNodePtr  curtoken, token = NULL;
  
   /* skip first several delimiters if any existed */
	for (ptr = str; *ptr == delimiter; ++ptr) continue;

	for (; *ptr != '\0';) {
		for (bptr = ptr, len = 0; *ptr != delimiter && *ptr != '\0';
                                                              ptr++, ++len) continue;
		curtoken = ValNodeNew(NULL);
		curtoken->data.ptrvalue = TextSave(bptr, len);
		token = tie_prev(token, curtoken);
		while (*ptr == delimiter || *ptr == '\t' || *ptr == ' ')
           ++ptr;
	}
	return (token);

}

static void FreeToken(ValNodePtr token)
{
	ValNodePtr v, vnext;
	
	for (v = token; v; v = vnext) {
		vnext = v->next;
		MemFree(v->data.ptrvalue);
		MemFree(v);
	}
}

GeneticCodeListPtr get_gcode_from_lineage(CharPtr lineage)
{
	Int4 tid;
	GeneticCodeListPtr gclp;
	Taxon1DataPtr tax;
	OrgRefPtr orp;
	OrgNamePtr onp;
	ValNodePtr	  token=NULL;
	CharPtr	stoken;
		
	for (token=TokenString(lineage, ';'); token!=NULL; token = token->next) {
		stoken = token->data.ptrvalue;
		CleanTail(stoken);
		if ((tid = tax1_getTaxIdByName(stoken)) < 0)
			tid = -tid;
			
		tax = tax1_getbyid(tid);
		if (tax && tax->org) {
			orp = tax->org;
			onp = orp->orgname;
			if (onp) {
				gclp = GeneticCodeListNew();
				gclp->genomic = onp->gcode;
				gclp->mitochondrial = onp->mgcode;
				FreeToken(token);
				Taxon1DataFree(tax);
				return gclp;
			}
		}
		if(tax != NULL) {
			Taxon1DataFree(tax);
		}
	}
	FreeToken(token);
	return NULL;
} 


/*---------------------------- check_org_ref() -------------------------*/
/****************************************************************************
*   check_org_ref:
*   -- replace OrgRefPtr by OrgRef from server if replace is TRUE
* 	-- or check OrgRef and return it back (uses cash and requires tax_init)
****************************************************************************/
OrgRefPtr check_org_ref(OrgRefPtr orp, Boolean replace)
{
   OrgRefPtr		   new, tmp;	
   
	if (orp == NULL) {
		return NULL;
	}
	if (orp->taxname != NULL) {
		tmp = get_tax_org(orp->taxname);
	} else if (orp->common != NULL) {
		tmp = get_tax_org(orp->common);
	} else {
		return orp;
	}
	if (replace && tmp != NULL) {
	         new = AsnIoMemCopy(tmp, (AsnReadFunc) OrgRefAsnRead, 
               (AsnWriteFunc) OrgRefAsnWrite); 
        OrgRefFree(orp);  
		return new;
	}
    return orp;
} /* check_org_ref */

/****************************************************************************
*   replace_org:
*   -- replacing OrgRefPtr by OrgRef from server 
*   -- freeing the input OrgRef                                                            *	if taxon id is not found return the input OrgRef
****************************************************************************/
OrgRefPtr replace_org_err(OrgRefPtr orp, Boolean replace)
{
	OrgRefPtr		new;	
	Taxon1DataPtr	tax;
	CharPtr			orgname;
	Int4 			tax_id;

	if (orp == NULL) {
		return NULL;
	}
	if (orp->taxname) {
		orgname = orp->taxname;
	} else if (orp->common) {
		orgname = orp->common;
	} else {
		orgname = "  ";
	}
	if ((tax_id = tax1_getTaxIdByOrgRef(orp)) < 0) {
		ErrPostEx(SEV_WARNING, ERR_ORGANISM_TaxIdNotUnique,
							"Not an unique Taxonomic Id for %s", orgname);
    	return orp;
	}
	tax = tax1_lookup(orp, 1);
	if (tax && tax->org) {
		if (tax->is_species_level != 1) {
			ErrPostEx(SEV_WARNING, ERR_ORGANISM_TaxIdNotSpecLevel,
					   "Taxarch hit is not on species level (%s)", orgname);
		}
		if (replace) {
	         new = AsnIoMemCopy(tax->org, (AsnReadFunc) OrgRefAsnRead, 
               (AsnWriteFunc) OrgRefAsnWrite); 
			tax->org = NULL;
			Taxon1DataFree(tax);
        	OrgRefFree(orp);
        	return new; 
        }
	} else {
		ErrPostEx(SEV_WARNING, ERR_ORGANISM_TaxNameNotFound,
					   "Taxon Id not found for [%s]", orgname);
	}
    return orp;
} /* replace_org_err */

/****************************************************************************
*   replace_org:
*   -- replacing OrgRefPtr by OrgRef from server 
*   -- freeing the input OrgRef                                                            *	if taxon id is not found return NULL!!
****************************************************************************/
OrgRefPtr replace_org(OrgRefPtr orp, Boolean replace)
{
    CharPtr         common = NULL;
	OrgRefPtr		new;	
	Taxon1DataPtr	tax;

	if (orp == NULL) {
		return NULL;
	}
	common = orp->common;
	if (StringDoesHaveText (orp->taxname) && StringDoesHaveText (orp->common)) {
	    orp->common = NULL;
	}
	tax = tax1_lookup(orp, 1);
	orp->common = common;
	if (tax && tax->org) {
		if (replace) {
	         new = AsnIoMemCopy(tax->org, (AsnReadFunc) OrgRefAsnRead, 
               (AsnWriteFunc) OrgRefAsnWrite); 
			tax->org = NULL;
			Taxon1DataFree(tax);
        	OrgRefFree(orp);
        	return new; 
        }
	} 
    return NULL;
} /* replace_org */

CharPtr get_tax_division(OrgRefPtr orp)
{
	Taxon1DataPtr		tax;
	CharPtr				div;  

	if (orp == NULL) {
		return NULL;
	}
	tax = tax1_lookup(orp, 0);
	if (tax) {
		div = StringSave(tax->div);
	    Taxon1DataFree(tax);
    	return div;
	}
	return NULL;
}

CharPtr get_embl_code(OrgRefPtr orp)
{
	Taxon1DataPtr		tax;
	CharPtr				embl_code;  

	if (orp == NULL) {
		return NULL;
	}
	tax = tax1_lookup(orp, 0);
	if (tax) {
		embl_code = StringSave(tax->embl_code);
	    Taxon1DataFree(tax);
    	return embl_code;
	}
	return NULL;
}

void GetTaxserverOrg (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
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
			if ((orp = replace_org(biosp->org, TRUE)) != NULL) {
				biosp->org = orp;
			} 
		}
	}
	for (sap = annot; sap != NULL; sap = sap->next) {
		if (sap->type == 1) {
			for (sfp = (SeqFeatPtr) sap->data; sfp != NULL; sfp = sfp->next) {
				if (sfp->data.choice == SEQFEAT_BIOSRC) {
					biosp = sfp->data.value.ptrvalue;
					if ((orp = replace_org(biosp->org, TRUE)) != NULL) {
						biosp->org = orp;
					} 
				}
			}
		}
	}
}
/* temporary simulant functions for old-new Taxon switch period */

Boolean TaxArchInit (void)
{
	return tax1_init();
}

Boolean TaxArchFini (void)
{
	tax1_fini();
	
	return TRUE;
}

/******************************************************************
*		check Taxon id in Biosource OrgrefPtr and report an error
*		if Id is not found
********************************************************************/
Boolean CheckTaxId(SeqEntryPtr sep)
{
    ValNodePtr vnp;
    BioSourcePtr bsrp;
    OrgRefPtr  orp;
    CharPtr    orgname;
    Taxon1DataPtr tax;
    Boolean retval = TRUE;
    
    ErrSetOptFlags(EO_SHOW_CODES); 
    ErrSetOptFlags(EO_MSG_MSGTEXT);         
    vnp = SeqEntryGetSeqDescr(sep, Seq_descr_source, NULL);
    if (vnp == NULL)
        return FALSE;
    bsrp = (BioSourcePtr) vnp->data.ptrvalue;
    orp = bsrp->org;
    if (orp == NULL) {
        return FALSE;
    }
    if (orp->taxname) {
        orgname = orp->taxname;
    } else if (orp->common) {
        orgname = orp->common;
    } else {
        orgname = "  ";
    }
    if (tax1_getTaxIdByOrgRef(orp) < 0) {
        ErrPostEx(SEV_WARNING, ERR_ORGANISM_TaxIdNotUnique,
                  "Not an unique Taxonomic Id for %s", orgname);
        return FALSE;
    }
    tax = tax1_lookup(orp, 0);
    if (tax == NULL) {
        ErrPostEx(SEV_WARNING, ERR_ORGANISM_TaxNameNotFound,
                  "Taxon Id not found for [%s]", orgname);
        retval = FALSE;
    } else if (tax->is_species_level != 1) {
        ErrPostEx(SEV_WARNING, ERR_ORGANISM_TaxIdNotSpecLevel,
                  "Taxarch hit is not on species level (%s)", orgname);
	}
    if(tax != NULL)
        Taxon1DataFree(tax);
    return retval;
    
}

static Boolean TaxCmpOrgById(BioSourcePtr b1, BioSourcePtr b2)
{
	DbtagPtr d1 = NULL, d2 = NULL;
	ValNodePtr vnp;
	
	if (b1 == NULL || b2 == NULL) {
		return FALSE;
	}
	if (b1->org ==  NULL || b2->org == NULL) {
		return FALSE;
	}
	for (vnp = b1->org->db; vnp; vnp = vnp->next) {
		d1 = (DbtagPtr) vnp->data.ptrvalue;
		if (StringCmp(d1->db, "taxon") == 0) {
			break;
		}
	}
	for (vnp = b2->org->db; vnp; vnp = vnp->next) {
		d2 = (DbtagPtr) vnp->data.ptrvalue;
		if (StringCmp(d2->db, "taxon") == 0) {
			break;
		}
	}
	if (d1 && d2) {
		if (d1->tag->id == d2->tag->id) {
			return TRUE;
		} else {
			/*
			if (tax1_join(d1->tag->id, d2->tag->id) != 1) {
				return TRUE;
			} else {
				return FALSE;
			}
			*/
		}
	} else if (StringICmp(b1->org->taxname, b2->org->taxname) == 0) {
			return TRUE;
	}
	return FALSE;
}

/****************************************************************************
*  local_tie_feat:
*  -- 
* links input to list specified by head pointer               01-12-94
****************************************************************************/
static SeqFeatPtr local_tie_feat(SeqFeatPtr head, SeqFeatPtr next)
{
   SeqFeatPtr v;

   if (head == NULL) {
      return next;
   }
   for (v = head; v->next != NULL; v = v->next)
   		continue;    
   v->next = next;
   return head;
}

static SubSourcePtr local_tie_next_subtype(SubSourcePtr head, SubSourcePtr next)
{
   SubSourcePtr v;

   if (head == NULL) {
      return next;
   }
   for (v = head; v->next != NULL; v = v->next)
   		continue;    
   v->next = next;
   return head;
}

static OrgModPtr local_tie_next_OrgMod(OrgModPtr head, OrgModPtr next)
{
   OrgModPtr v;

   if (head == NULL) {
      return next;
   }
   for (v = head; v->next != NULL; v = v->next)
   		continue;    
   v->next = next;
   return head;
}

static ValNodePtr local_remove_node(ValNodePtr head, ValNodePtr x)
{
	ValNodePtr	v, p;
	
	if (head == NULL) {
		return NULL;
	}
	if (x == head) {
		head = x->next;
		x->next = NULL;
		ValNodeFree(x);
		return head;
	}
	for (v = head; v != NULL && v != x; v = v->next) {
		p = v;
	}
	if (v != NULL) {
		p->next = x->next;
		x->next = NULL;
		ValNodeFree(x);
	}
	return head;
}

static BioSourcePtr local_BioSourceMerge(BioSourcePtr host, BioSourcePtr guest)
{
	SubSourcePtr ssp, sp;
	OrgModPtr omp, homp;
	OrgNamePtr	onp;
	
	if (host == NULL && guest == NULL) {
		return NULL;
	}
	if (host == NULL && guest != NULL) {
		host = AsnIoMemCopy(guest, (AsnReadFunc) BioSourceAsnRead, 
		   						(AsnWriteFunc) BioSourceAsnWrite);
		return host;
	}
	if (host != NULL && guest == NULL) {
		return host;
	}
	if (host->genome == 0 && guest->genome != 0) {
		host->genome = guest->genome;
	}
	if (host->origin == 0 && guest->origin != 0) {
		host->origin = guest->origin;
	}
	for (ssp = guest->subtype; ssp; ssp = ssp->next) {
			sp = AsnIoMemCopy(ssp, (AsnReadFunc) SubSourceAsnRead, 
		   						(AsnWriteFunc) SubSourceAsnWrite);
		host->subtype = local_tie_next_subtype(host->subtype, sp);
	}
	if (guest->org->orgname) {
		for (omp = guest->org->orgname->mod; omp; omp = omp->next) {
			homp = AsnIoMemCopy(omp, (AsnReadFunc) OrgModAsnRead, 
		   						(AsnWriteFunc) OrgModAsnWrite);
		   	if ((onp = host->org->orgname)	== NULL) {
		   		onp = OrgNameNew();
		   		host->org->orgname = onp;
		   	}				
			onp->mod = local_tie_next_OrgMod(onp->mod, homp);
		}
	}
	return host;
}

/**************************************************************************
*	Compare BioSources in one bioseq->descr using Taxonomy to find
*	their join parent
*	merge if organisms are the same or create a feature if different
*
**************************************************************************/
void TaxMergeBSinDescr (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
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
			if (bsrc && TaxCmpOrgById(bsrc, bs) == TRUE) {
				bsrc = local_BioSourceMerge(bsrc, bs);
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
				   sap->data = local_tie_feat(sap->data, sfp); 
			}
		} else {
			ErrPostStr(SEV_WARNING, 0, 0, "Biosource missing Organism info");
		}
		BioSourceFree(bs);
		vnp->next = local_remove_node(vnp->next, v);
		
	}
	return;
}
