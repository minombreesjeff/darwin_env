/** for ErrPostEx() ****/

static char *this_module = "utilpub";

#ifdef THIS_MODULE
#undef THIS_MODULE
#endif

#define THIS_MODULE this_module
static char *this_file = __FILE__;
#define THIS_FILE this_file

#include <objmgr.h>
#include <sequtil.h>
#include <objpub.h>
#include <toasn3.h>
#include <tfuns.h>
#include <utilpub.h>
#include <puberr.h>

#define BUFLEN 120

NLM_EXTERN ValNodePtr tie_next(ValNodePtr head, ValNodePtr next)
{
	ValNodePtr v;

	if (head == NULL) {
		return next;
	}
	for (v = head; v->next != NULL; v = v->next) {
		v = v;
	}
	v->next = next;
	return head;
}

/***************************************************************
*static void CpPubDescr (PubdescPtr new_desc, PubdescPtr old_desc)
*
*Copy the old_desc to the new_desc.  fields are allocated!!
***************************************************************/

static void CpPubDescr (PubdescPtr new_desc, PubdescPtr old_desc)

{
	if (new_desc && old_desc)
	{
		new_desc->pub = old_desc->pub;
		new_desc->name = old_desc->name;
		new_desc->fig = old_desc->fig;
		new_desc->num = old_desc->num;
		new_desc->numexc = old_desc->numexc;
		new_desc->poly_a = old_desc->poly_a;
		new_desc->align_group = old_desc->align_group;
		new_desc->maploc = old_desc->maploc;
		new_desc->seq_raw = old_desc->seq_raw;
		new_desc->comment = old_desc->comment;
		new_desc->reftype = old_desc->reftype;
	}

	return;
}

static Int2 GetSerialNumber (ValNodePtr pub)
{
	CitGenPtr cgp;
	ValNodePtr vnp;

	if (pub == NULL)
		return -1;

	if (pub->choice == PUB_Equiv)
	{
		for (vnp=(pub->data.ptrvalue); vnp != NULL; vnp = vnp->next)
		{
			if (vnp->choice == PUB_Gen)
			{
				cgp = vnp->data.ptrvalue;
				return cgp->serial_number;
			}
		}
	}
	else if (pub->choice == PUB_Gen)
	{
		cgp = pub->data.ptrvalue;
		return cgp->serial_number;
	}
			
	return -1; /* -1 if no serial number available */
}

static Int2 GetPubChoice (ValNodePtr pub)
{
	CitGenPtr cgp;
	ValNodePtr vnp;

	if (pub == NULL)
		return -1;

	if (pub->choice == PUB_Equiv)
	{
		for (vnp=(pub->data.ptrvalue); vnp != NULL; vnp = vnp->next)
		{
			if (vnp->choice == PUB_Gen) {
				if ((cgp = vnp->data.ptrvalue) != NULL) {
					if (cgp->cit != NULL) {
						return PUB_Gen;  /* 1 */
					}
				}
			} else if (vnp->choice == PUB_Sub) {
				return PUB_Sub;  /* 2 */
			}
		}
	}
	else if (pub->choice == PUB_Gen)
	{
		if ((cgp = pub->data.ptrvalue) != NULL) {
			if (cgp->cit != NULL) {
				return PUB_Gen;  /* 1 */
			}
		}
	}
	else if (pub->choice == PUB_Sub)
	{
		return PUB_Sub;  /* 2 */
	}		
	return -1; /* -1 not PUB_Sub and not unpublis */
}

static DatePtr GetPubDate (ValNodePtr pub)
{
	ValNodePtr vnp;
	CitGenPtr cgp = NULL;
	CitArtPtr cap  = NULL;
	CitBookPtr cbp = NULL;
	CitJourPtr cjp = NULL;
	CitSubPtr csp = NULL;
	CitPatPtr	cpp = NULL;
	ImprintPtr	imp = NULL;
	DatePtr		dp = NULL;

	if (pub == NULL) {
		return NULL;
	}
	if (pub->choice == PUB_Equiv)
	{
		for (vnp=pub->data.ptrvalue; vnp != NULL; vnp = vnp->next) {
			switch (vnp->choice) 
			{
				case (PUB_Gen):
					if ((cgp = (CitGenPtr) vnp->data.ptrvalue) != NULL) {
						dp = (DatePtr) cgp->date;
					}
					break;
				case (PUB_Sub):
					if ((csp = (CitSubPtr) vnp->data.ptrvalue) != NULL) {
						imp = (ImprintPtr) csp->imp;
						if (imp) {
							dp = (DatePtr) imp->date;
						}
						if (csp->date) {
							dp = (DatePtr) csp->date;
						}
					}
					break;
				case (PUB_Article):
					if ((cap = (CitArtPtr) vnp->data.ptrvalue) != NULL) {
						if (cap->from == 1)
						{
							cjp = (CitJourPtr) (cap->fromptr);
							if (cjp && cjp->imp) {
								dp = cjp->imp->date;
							}
						} else if (cap->from == 2 || cap->from == 3) {
							cbp = (CitBookPtr) (cap->fromptr);
							if (cbp && cbp->imp) {
								dp = cbp->imp->date;
							}
						}
					}
					break;
				case (PUB_Patent):
					if ((cpp = (CitPatPtr) vnp->data.ptrvalue) != NULL) {
						dp = (DatePtr) cpp->date_issue;
					}
					break;
				case (PUB_Equiv):
				case (PUB_Muid):
				case (PUB_Man):
				case (PUB_Proc):
				case (PUB_PMid):
				default:
					break;
			}
			if (dp != NULL) {
				return DateDup(dp);
			}
		}
	} else {
		switch (pub->choice) 
		{
			case (PUB_Gen):
				if ((cgp = (CitGenPtr) pub->data.ptrvalue) != NULL) {
					dp = (DatePtr) cgp->date;
				}
				break;
			case (PUB_Sub):
				if ((csp = (CitSubPtr) pub->data.ptrvalue) != NULL) {
					if (imp) {
						dp = (DatePtr) imp->date;
					}
					if (csp->date) {
						dp = (DatePtr) csp->date;
					}
				}
				break;
			case (PUB_Article):
				if ((cap = (CitArtPtr) pub->data.ptrvalue) != NULL) {
					if (cap->from == 1) {
							cjp = (CitJourPtr) (cap->fromptr);
							if (cjp && cjp->imp) {
								dp = cjp->imp->date;
							}
					} else if (cap->from == 2) {
							cbp = (CitBookPtr) (cap->fromptr);
							if (cbp && cbp->imp) {
								dp = cbp->imp->date;
							}
					}
				}
				break;
			case (PUB_Patent):
				if ((cpp = (CitPatPtr) pub->data.ptrvalue) != NULL) {
					dp = (DatePtr) cpp->date_issue;
				}
				break;
			case (PUB_Equiv):
			case (PUB_Muid):
			case (PUB_Man):
			case (PUB_Proc):
			case (PUB_PMid):
			default:
				break;
		}
	}
	if (dp != NULL) {
		return DateDup(dp);
	}
	return NULL;
}

static PubStructPtr CreatePubStruct(PubdescPtr pdp, ValNodePtr pub,
Uint2 entityID, Uint2 itemID, Uint2 itemtype)
{
	PubStructPtr psp = NULL;
	PubdescPtr descr=NULL;
	ValNodePtr vnp;

	if (pdp) {
		descr = AsnIoMemCopy(pdp, (AsnReadFunc) PubdescAsnRead,
											(AsnWriteFunc) PubdescAsnWrite);
		vnp = ValNodeNew(NULL);
		vnp->choice = PUB_Equiv;
		vnp->data.ptrvalue = descr->pub;
		psp = (PubStructPtr) MemNew(sizeof(PubStruct));
		psp->pub = vnp;
		psp->no_free = TRUE;
	} else if (pub) {
		psp = (PubStructPtr) MemNew(sizeof(PubStruct));
		psp->no_free = FALSE;
		psp->pub = AsnIoMemCopy(pub, (AsnReadFunc) PubAsnRead,
											(AsnWriteFunc) PubAsnWrite);
	}
	if (psp) {
		psp->descr = descr;
		psp->number = GetSerialNumber(psp->pub);
		psp->date = GetPubDate(psp->pub);
		psp->choice = GetPubChoice(psp->pub);
		psp->entityID = entityID;
		psp->itemID = itemID;
		psp->itemtype = itemtype;
	}
	
	return psp;
}

static void AddCitFeat (PubStructPtr psp, SeqFeatPtr sfp)

{
	Int2 i, count=psp->citcount;
	SeqFeatPtr PNTR tmp;

	if (sfp == NULL) {
		return;
	}
	tmp = (SeqFeatPtr PNTR) MemNew((count+1)*sizeof(SeqFeatPtr));
	if (count != 0) {
		for (i=0; i < count; i++) {
			tmp[i] = psp->citfeat[i];
		}
		MemFree(psp->citfeat);
	}

	psp->citfeat = tmp;

	psp->citfeat[count] = sfp;
	(psp->citcount)++;

	return;

}

static void AddPubBsp (PubStructPtr psp, BioseqPtr bsp)

{
	Boolean match=FALSE;
	Int2 i, count=psp->bspcount;
	BioseqPtr PNTR tmp;

	if (bsp == NULL) {
		return;
	}
	if (count != 0)
	{ /* Look to see if this BioseqPtr is already in the list. */
		for (i = 0; i < count; i++) {
			if (bsp == psp->bsp[i]) {
				match = TRUE;
				break;
			}
		}
	}
				
	if (match == TRUE) {
		return;
	}
	if (count != 0) {
		tmp = (BioseqPtr PNTR) MemNew(count*sizeof(BioseqPtr));
		for (i=0; i<count; i++)
			tmp[i] = psp->bsp[i];

		MemFree(psp->bsp);
	}

	psp->bsp = 
		(BioseqPtr PNTR) MemNew((count+1)*sizeof(BioseqPtr));

	if (count != 0) {
		for (i=0; i<count; i++)
			psp->bsp[i] = tmp[i];

		MemFree(tmp);
	}

	psp->bsp[count] = bsp;
	(psp->bspcount)++;

	return;
}

NLM_EXTERN ValNodePtr StorePub(BioseqPtr bsp, ValNodePtr vnp, ValNodePtr pub, SeqFeatPtr sfp, Int2 start, Uint2 entityID, Uint2 itemID, Uint2 itemtype)
{
	PubdescPtr pdp, descr = NULL;
	PubStructPtr psp;
	ValNodePtr v, PubSet, Pub;
	ImpFeatPtr	ifp;

	if (pub) {
		pdp = (PubdescPtr) pub->data.ptrvalue;
		if ((psp = CreatePubStruct(pdp, NULL, entityID, itemID, itemtype)) != NULL) {
			ValNodeAddPointer(&(vnp), 0, psp);
			psp->start = start;
			AddPubBsp (psp, bsp);
		}
	} else if (sfp && start == 2) {
		pdp = (PubdescPtr) sfp->data.value.ptrvalue;
		if ((psp = CreatePubStruct(pdp, NULL, entityID, itemID, itemtype))
			 != NULL) {
		ValNodeAddPointer(&(vnp), 0, psp);
			psp->start = start;
			AddPubBsp (psp, bsp);
			AddCitFeat(psp, sfp);
		}
	} else {
		PubSet = (ValNodePtr) sfp->cit;
		Pub = (ValNodePtr) PubSet->data.ptrvalue;
		for (v=Pub; v; v=v->next) {
			if ((psp = CreatePubStruct(NULL, v, entityID, itemID, itemtype)) 
				!= NULL) {
				ValNodeAddPointer(&(vnp), 0, psp);
				AddPubBsp (psp, bsp);
				psp->Pub = Pub;
				psp->start = start;
				if (sfp->data.choice == SEQFEAT_IMP) {
					ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
					if (StringCmp(ifp->key, "Site-ref") == 0
											&& sfp->comment) {
						descr = PubdescNew();
						descr->comment = StringSave(sfp->comment);
						psp->descr = descr;
					}
				}
			}
		}
	}
	return vnp;
}
/***************************************************************
*ValNodePtr StoreNAPubCit(ValNodePtr PubOnCit, BioseqPtr bsp, SeqFeatPtr sfp)
*
*	major KLUDGE warning!  This function stores the pub
*	from an sfp as though it were a pub that covered the
*	entire entry.  This is acceptable, as the Pub-feat 
*	(on a nucleotide entry) WILL cover the entire PROTEIN entry.
*
*	To ensure clickability of the GenPept flat file in Sequin,
*	the original SeqFeatPtr is also stored.  This will allow
*	Sequin to identify the pub.  To avoid "misuse", psp->SeqDescr 
*	is set equal to NULL.  These variables are used in GetPapRefPtr.
*	
***************************************************************/

NLM_EXTERN ValNodePtr StoreNAPubCit(ValNodePtr PubOnCit, BioseqPtr bsp, SeqFeatPtr sfp)

{
	PubdescPtr pdp, descr;
	PubStructPtr psp;
	ValNodePtr vnp;

	psp = (PubStructPtr) MemNew(sizeof(PubStruct));
	ValNodeAddPointer(&(PubOnCit), 0, psp);

	pdp = (PubdescPtr) (sfp->data.value.ptrvalue);
	descr = PubdescNew();
	CpPubDescr(descr, pdp);
	psp->descr = descr;
	vnp = ValNodeNew(NULL);
	vnp->choice = PUB_Equiv;
	vnp->data.ptrvalue = AsnIoMemCopy(pdp->pub, 
			(AsnReadFunc) PubEquivAsnRead, (AsnWriteFunc) PubEquivAsnWrite);
	psp->pub = vnp;
	psp->start = 1;
	psp->number = GetSerialNumber(psp->pub);
	psp->date = GetPubDate(psp->pub);
	psp->choice = GetPubChoice(psp->pub);
	AddPubBsp (psp, bsp);

	return PubOnCit;
}

						/******Unique********/
/****************************************************************************
*
*	"CatSfpStacks" concatenates two SeqFeatPtr PNTR's and returns
*	the result to replace sfpp1.
*
****************************************************************************/

static SeqFeatPtr PNTR CatSfpStacks 
(SeqFeatPtr PNTR sfpp1, Int2 *count1, SeqFeatPtr PNTR sfpp2, Int2 count2)

{

	Int2 i, j, count=(*count1);
	SeqFeatPtr PNTR tmp;
	Boolean redundant;

	if (count2 == 0)
		return sfpp1;

	tmp = (SeqFeatPtr PNTR) MemNew((*count1+count2)*sizeof(SeqFeatPtr));

	for (i=0; i<count; i++)
		tmp[i] = sfpp1[i];

	for (i=0; i<count2; i++)
	{
		redundant = FALSE;
		for (j=0; j<count; j++)
		{
			if (sfpp1[j] == sfpp2[i])
			{
				redundant = TRUE;
				break;
			}
		}
		if (redundant == FALSE)
		{
			tmp[(*count1)] = sfpp2[i];
			(*count1)++;
		}
	}

	if (count != 0)
		MemFree(sfpp1);

	return tmp;
}

/****************************************************************************
*
*	"CatBspStacks" concatenates two BioseqPtr PNTR's and returns
*	the result to replace bspp1.
*
****************************************************************************/

static BioseqPtr PNTR CatBspStacks 
(BioseqPtr PNTR bspp1, Int2 *count1, BioseqPtr PNTR bspp2, Int2 count2)

{

	Int2 i, j, count=(*count1);
	BioseqPtr PNTR tmp;
	Boolean redundant;

	if (count2 == 0)
		return bspp1;

	tmp = (BioseqPtr PNTR) MemNew((*count1+count2)*sizeof(BioseqPtr));

	for (i=0; i<count; i++)
		tmp[i] = bspp1[i];

	for (i=0; i<count2; i++)
	{
		redundant = FALSE;
		for (j=0; j<count; j++)
		{
			if (bspp1[j] == bspp2[i])
			{
				redundant = TRUE;
				break;
			}
		}
		if (redundant == FALSE)
		{
			tmp[(*count1)] = bspp2[i];
			(*count1)++;
		}
	}

	if (count != 0)
		MemFree(bspp1);

	return tmp;
}

/***************************************************************************
*void CatPspInfo (PubStructPtr psp1, PubStructPtr psp2)
*
*	Concatenate some of the information from two different PubStructs
*	that refer to the same publication.
**************************************************************************/

static void CatPspInfo (PubStructPtr psp1, PubStructPtr psp2)

{
  psp2->citfeat = 
    CatSfpStacks(psp2->citfeat, &(psp2->citcount), psp1->citfeat, psp1->citcount); 
  psp2->pubfeat = 
    CatSfpStacks(psp2->pubfeat, &(psp2->pubcount), psp1->pubfeat, psp1->pubcount); 
  psp2->bsp = 
    CatBspStacks(psp2->bsp, &(psp2->bspcount), psp1->bsp, psp1->bspcount); 
  if (psp2->start > psp1->start)
	psp2->start = psp1->start;
   if (psp1->descr && psp2->descr)
   {
	if (psp2->descr->name == NULL && psp1->descr->name)
		psp2->descr->name = StringSave(psp1->descr->name);
	if (psp2->descr->fig == NULL && psp1->descr->fig)
		psp2->descr->fig = StringSave(psp1->descr->fig);
	if (psp2->descr->poly_a == FALSE && psp1->descr->poly_a)
		psp2->descr->poly_a = psp1->descr->poly_a;
	if (psp2->descr->comment == NULL && psp1->descr->comment)
		psp2->descr->comment = StringSave(psp1->descr->comment);
	if (psp2->descr->maploc == NULL && psp1->descr->maploc)
		psp2->descr->maploc = StringSave(psp1->descr->maploc);
   }
   return;
}

static Int2 GetPubScore (ValNodePtr pub)

{
/* Below are the scores assigned to each pub following (roughly) 
Sirotkin's FlatRefBest.  Here 0 (not set) is given a score of 0, 1
(gen) a score of 6, 2 (sub) a score of 11 etc. The higher the score
the better the pub. */
	static Int2 scores[14] = 
	{0, 6, 11, 7, 1, 8, 4, 3, 5, 9, 2, 10, 12, 1};

	if (pub == NULL || pub->choice > 13)
		return 0;

	return scores[pub->choice];

}

/**************************************************************************
*
*	"SelectBestPub" selects the best of two publications in terms
*	of the most complete record (e.g., title etc.).  To be used
*	with "UniquePubs"
*	return values are:
*	>0: pub1 is better than pub2
*	 0: pub1 and pub2 are equivalent (i.e., pub1->choice = pub2->choice)
*	<0: pub2 is better than pub1
*
***************************************************************************/

NLM_EXTERN Int2 SelectBestPub (ValNodePtr pub1, ValNodePtr pub2)

{
	CitArtPtr ca=NULL;
	CitJourPtr jp;
	ImprintPtr imp;
	Int2 score1, score2, temp;
	MedlineEntryPtr ml;
	ValNodePtr vnp, int_pub1 = NULL, int_pub2 = NULL;

	score1=0;
	if (pub1 == NULL || pub2 == NULL) {
		return 0;
	}
	if (pub1->choice == PUB_Equiv)
	{
		for (vnp=pub1->data.ptrvalue; vnp; vnp=vnp->next)
		{
			temp = GetPubScore(vnp);
			if (temp > score1)
			{
				score1 = temp;
				int_pub1 = vnp;
			}
		}
	}
	else
	{
		score1 = GetPubScore(pub1);
		int_pub1 = pub1;
	}

	score2=0;
	if (pub2->choice == PUB_Equiv)
	{
		for (vnp=pub2->data.ptrvalue; vnp; vnp=vnp->next)
		{
			temp = GetPubScore(vnp);
			if (temp > score2)
			{
				score2 = temp;
				int_pub2 = vnp;
			}
		}
	}
	else
	{
		score2 = GetPubScore(pub2);
		int_pub2 = pub2;
	}

/* The following should be improved to encompass other prepub situations
 (i.e., books etc.) as well as (possibly) check for page numbering etc. ????*/

	if (int_pub1 == NULL || int_pub2 == NULL) {
		return 0;
	}
	if (score1 == score2)
	{ /* If the scores are the same, see if one of them is a prepub */
		if ( int_pub1 -> choice == PUB_Medline){
			ml = (MedlineEntryPtr) int_pub1 -> data.ptrvalue;
			ca = (CitArtPtr) ml -> cit;
		}else if (int_pub1->choice == PUB_Article) {
			ca = (CitArtPtr) int_pub1 -> data.ptrvalue;
		}
		if(ca && ca ->fromptr && ca ->from ==1)
		{
			jp = (CitJourPtr) ca -> fromptr;
			if ( jp && (imp=jp->imp) != NULL)
				if (imp->prepub == 1 || imp->prepub == 2)
					score1 = score2-1;
		}
		if ( int_pub2 -> choice == PUB_Medline){
			ml = (MedlineEntryPtr) int_pub2 -> data.ptrvalue;
			ca = (CitArtPtr) ml -> cit;
		}else if (int_pub2->choice == PUB_Article) {
			ca = (CitArtPtr) int_pub2 -> data.ptrvalue;
		}
		if(ca && ca ->fromptr && ca ->from ==1)
		{
			jp = (CitJourPtr) ca -> fromptr;
			if ( jp && (imp=jp->imp) != NULL)
				if (imp->prepub == 1 || imp->prepub == 2)
					score2 = score1-1;
		}
	}
	return score1-score2;

}

/************************************************************************
*
*	"Unique Pubs" to unique the pubs in the PubStructPtr PNTR
*	and then concatenate the information (seqfeats etc.) in 
*	two elements that share a pub.
*
***********************************************************************/

NLM_EXTERN void UniquePubs (ValNodePtr PNTR vnpp)
{
	Int2 status, status1;
	PubStructPtr psp1, psp2;
	ValNodePtr vnp1, vnp2, vnp2next;

	for (vnp1 = *vnpp; vnp1; vnp1 = vnp1->next) {
		for (vnp2=*vnpp; vnp2; vnp2=vnp2next) {
			vnp2next = vnp2->next;
			if (vnp1 == vnp2) {
				continue;
			}
			psp1 = vnp1->data.ptrvalue;
			psp2 = vnp2->data.ptrvalue;
			status = PubMatch(psp1->pub, psp2->pub);
			if (! ABS(status)) {
				status1 = SelectBestPub(psp1->pub, psp2->pub);
				if (status1 >= 0) {
				    CatPspInfo(psp2, psp1);
				    FreePubStruct(vnp2->data.ptrvalue);
				    *vnpp = remove_node(*vnpp, vnp2);
				}
			}
		}
	}
}	/* UniquePubs */

NLM_EXTERN void FreePubStruct(PubStructPtr psp)

{
	if (psp == NULL) {
		return;
	}
	if (psp->bspcount)
		MemFree(psp->bsp);
	if (psp->citcount)
		MemFree(psp->citfeat);
	if (psp->pubcount)
		MemFree(psp->pubfeat);
	if (psp->descr) {
		psp->descr = PubdescFree(psp->descr);
	}
	if (psp->pub) {
		if (psp->no_free) 
			ValNodeFree(psp->pub);
		else
			PubFree(psp->pub);
	}
	if (psp->date) {
		DateFree(psp->date);
	}
	psp = MemFree(psp);
	return;
}

static Boolean GetPMFromPub(ValNodePtr v, Int4Ptr m, Int4Ptr p)
{

	*m = 0;
	*p = 0;
	switch (v->choice) {
		case PUB_Muid:
		  	*m = v->data.intvalue;
		  	return TRUE;
		case PUB_PMid:
		  	*p = v->data.intvalue;
		  	return TRUE;
		case PUB_Equiv:
			for (v = v->data.ptrvalue; v; v = v->next) {
				  if (v->choice == PUB_Muid) { 
					  *m = v->data.intvalue;
				  }
				  if (v->choice == PUB_PMid) { 
					  *p = v->data.intvalue;
				  }
			}
		  	break;
	}
	return (*m != 0 || *p != 0);
}

static Boolean CitGenTitlesMatch (ValNodePtr pub1, ValNodePtr pub2)

{
  CitGenPtr  cgp1, cgp2;

  if (pub1->choice == PUB_Gen) {
    cgp1 = (CitGenPtr) pub1->data.ptrvalue;
    if (cgp1->serial_number != -1 && pub1->next != NULL) {
      pub1 = pub1->next;
    }
  }
  if (pub2->choice == PUB_Gen) {
    cgp2 = (CitGenPtr) pub2->data.ptrvalue;
    if (cgp2->serial_number != -1 && pub2->next != NULL) {
      pub2 = pub2->next;
    }
  }

  if (pub1->choice != PUB_Gen || pub2->choice != PUB_Gen) return TRUE;
  cgp1 = (CitGenPtr) pub1->data.ptrvalue;
  cgp2 = (CitGenPtr) pub2->data.ptrvalue;
  if (cgp1->title == NULL || cgp2->title == NULL) return TRUE;
  if (StringCmp (cgp1->title, cgp2->title) != 0) return FALSE;
  return TRUE;
}

NLM_EXTERN Int2 PubLabelMatch (ValNodePtr pub1, ValNodePtr pub2)
{
	ValNodePtr m1, m2;
	CitGenPtr g1, g2;
	size_t len;
	Int4 am, bm, ap, bp;
	
	if ((m1 = MinimizePub(pub1)) == NULL) {
		return -1;
	}
	if ((m2 = MinimizePub(pub2)) == NULL) {
		PubFree(m1);
		return -1;
	}
	if (GetPMFromPub(m1, &am, &ap) &&  GetPMFromPub(m2, &bm, &bp)) {
		PubFree(m2);
		PubFree(m1);
		if (ap != 0 && ap == bp) {
			return 0;
		} else if (am != 0 && am == bm) {
			return 0;
		} else {
			/* ERROR */
			return -1;
		}
	}
	if (m1->choice == PUB_Gen) {
		if (m2->choice != PUB_Gen) {
			PubFree(m1);
			if (m2->choice == PUB_Equiv)
				PubFree(m2);
			else
				ValNodeFree(m2);
			return -1;
		}
		if ((g1 = m1->data.ptrvalue) == NULL) {
			ValNodeFree(m1);
			PubFree(m2);
			return -1;
		}
		if ((g2 = m2->data.ptrvalue) == NULL) {
			ValNodeFree(m2);
			PubFree(m1);
			return -1;
		}
		if (g1->cit == NULL || g2->cit == NULL) {
			PubFree(m1);
			PubFree(m2);
			return -1;
		}
		len = StringLen (g2->cit);
		if (len > 1 && g2->cit [len - 1] == '>') {
		  g2->cit [len - 1] = '\0';
		}
		len = StringLen (g1->cit);
		if (len > 1 && g1->cit [len - 1] == '>') {
		  g1->cit [len - 1] = '\0';
		}
		len = MIN (StringLen (g1->cit), StringLen (g2->cit));
		if (StringNICmp(g1->cit, g2->cit, len) == 0) {
			if (CitGenTitlesMatch (pub1, pub2)) {
				PubFree(m1);
				PubFree(m2);
				return 0;
			}
		}
	}
	PubFree(m1);
	PubFree(m2);
	return -1;
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

NLM_EXTERN ValNodePtr MinimizePub(ValNodePtr pub)
{
	ValNodePtr  v, min_pub = NULL, epub, apub;
	Char buffer[BUFLEN+1];
	CitGenPtr	cit_gen, gen;
	
	if (pub == NULL) {
		return NULL;
	}
	if (pub->choice == PUB_Equiv) {
		for (v = pub->data.ptrvalue; v != NULL; v= v->next) {
			if (v->choice == PUB_Muid) {
				if (min_pub == NULL) {
					min_pub = AsnIoMemCopy(v, (AsnReadFunc) PubAsnRead,
											(AsnWriteFunc) PubAsnWrite);
				} else {
					epub = ValNodeNew(NULL);
					epub->choice = PUB_Equiv;
					apub = AsnIoMemCopy(v, (AsnReadFunc) PubAsnRead,
											(AsnWriteFunc) PubAsnWrite);
					min_pub->next = apub;
					epub->data.ptrvalue = min_pub;
					return epub;
				}
			}
			if (v->choice == PUB_PMid) {
				if (min_pub == NULL) {
					min_pub = AsnIoMemCopy(v, (AsnReadFunc) PubAsnRead,
											(AsnWriteFunc) PubAsnWrite);
				} else {
					epub = ValNodeNew(NULL);
					epub->choice = PUB_Equiv;
					apub = AsnIoMemCopy(v, (AsnReadFunc) PubAsnRead,
											(AsnWriteFunc) PubAsnWrite);
					min_pub->next = apub;
					epub->data.ptrvalue = min_pub;
					return epub;
				}
			}
		}
		v = pub->data.ptrvalue;
	} else {
		v = pub;
	}
	if (min_pub != NULL) {
			return min_pub;
	}
	if (v->choice == PUB_Gen) {
		gen = v->data.ptrvalue;
		if (gen->serial_number != -1 && v->next != NULL) {
			v = v->next;
		}		
	}
	if (v->choice == PUB_Muid || v->choice == PUB_PMid) {
		min_pub = AsnIoMemCopy(v, (AsnReadFunc) PubAsnRead,
											(AsnWriteFunc) PubAsnWrite);
		return min_pub;
	}
	if ((PubLabelUnique(v, buffer, BUFLEN, OM_LABEL_CONTENT, TRUE)) == 0) {
		min_pub = AsnIoMemCopy(v, (AsnReadFunc) PubAsnRead,
											(AsnWriteFunc) PubAsnWrite);
		return min_pub;
	}
	min_pub = ValNodeNew(NULL);
	min_pub->choice = PUB_Gen;
	cit_gen = CitGenNew();
	cit_gen->cit = space_save(buffer);
	min_pub->data.ptrvalue = cit_gen;
	
	return min_pub;
}

/**********************************************************
*	store all pubs in a chain of PubStruct using StorePub()
*
***********************************************************/
NLM_EXTERN void FindCit (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	BioseqPtr		bsp_sfp, bsp = NULL;
	BioseqSetPtr	bssp;
	SeqAnnotPtr		sap, ap;
	SeqFeatPtr		head, sfp;
	ValNodePtr PNTR	vnpp, descr, tmp;
	ImpFeatPtr		/*UNUSED*/ifp;
	SeqLocPtr		slp;
	
	vnpp = (ValNodePtr PNTR) data;
	if (IS_Bioseq(sep)) {
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		descr = bsp->descr;
		sap = bsp->annot;
	}
	else {
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		descr = bssp->descr;
		sap = bssp->annot;
	}
	for (tmp = descr; tmp != NULL; tmp = tmp->next) {
		if (tmp->choice == Seq_descr_pub) {
			*vnpp = StorePub(bsp, *vnpp, tmp, NULL, 1, 0, 0 ,0);
		}
	}
	for (ap = sap; ap != NULL; ap = ap->next) {
		if (ap->type != 1) {
			continue;
		}
		head = (SeqFeatPtr)(ap->data);
		for (sfp = head; sfp; sfp = sfp->next) {
			if (sfp->data.choice == SEQFEAT_PUB) {
				slp = sfp->location;
				bsp_sfp = BioseqFindCore(SeqLocId(slp));
				*vnpp = StorePub(bsp_sfp, *vnpp, NULL, sfp, 2, 0, 0, 0);
			} 
			if (sfp->data.choice == SEQFEAT_IMP) {
				ifp = sfp->data.value.ptrvalue;
				if (sfp->cit != NULL) {
					slp = sfp->location;
					bsp_sfp = BioseqFindCore(SeqLocId(slp));
					*vnpp = StorePub(bsp_sfp, *vnpp, NULL, sfp, 3, 0, 0, 0);
				}
			}
		}
	}
	
}

/**********************************************************
*	find citation on features and find pub with matching serial number
*	change all /citation to minimum pubs  
*	
***********************************************************/
NLM_EXTERN void ChangeCitQual (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	BioseqPtr		bsp = NULL;
	BioseqSetPtr	bssp;
	SeqAnnotPtr		sap, ap;
	SeqFeatPtr		sfp, head;
	ValNodePtr PNTR	vnpp, /*UNUSED*/descr;
	ValNodePtr		vnp, pubset = NULL, pub_min, /*UNUSED*/pub;
	PubStructPtr	psp;
	Int2			snum;
	CharPtr			qval, s;
	Boolean			first, found;
	
	vnpp = (ValNodePtr PNTR) data;
	if (IS_Bioseq(sep)) {
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		descr = bsp->descr;
		sap = bsp->annot;
	}
	else {
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		descr = bssp->descr;
		sap = bssp->annot;
	}
	for (ap = sap; ap != NULL; ap = ap->next) {
		if (ap->type != 1) {
			continue;
		}
		head = (SeqFeatPtr)(ap->data);
		for (sfp = head; sfp; sfp = sfp->next) {
			first = TRUE;
   			while ((qval = qvalue_extract(&(sfp->qual), "citation"))
   					 != NULL) {
   					 found = FALSE;
				for (s = qval; *s != '\0' && !IS_DIGIT(*s); s++)
					continue;
			    snum = atoi(s);
			    MemFree(qval);
			    for (vnp = *vnpp; vnp != NULL; vnp = vnp->next) {
					psp = (PubStructPtr) (vnp->data.ptrvalue); 
				    if (psp->number == snum) {
				    	if (bsp && psp->bsp ) {
				    		if (psp->bsp[0] != NULL) {
				    			if( bsp != psp->bsp[0]) {
				    				continue;
				    			}
				    		}
				    	}
					    pub_min = MinimizePub(psp->pub);
					    if (first) {
						    pubset = ValNodeNew(NULL);
						     pubset->choice = 1;         /* Pub-set, pub */
						     pubset->data.ptrvalue = pub_min;
						    sfp->cit = pubset;
						     first = FALSE;
					     } else {
						     pubset->data.ptrvalue =
						      tie_next(pubset->data.ptrvalue, pub_min);
						      pub = pubset->data.ptrvalue;
					    }
					    found = TRUE;
					    break;
					}
				} /* vnpp*/
				if (found == FALSE) {
					ErrPostEx(SEV_ERROR, ERR_QUALIFIER_NoRefForCiteQual, 
					"No Reference found for Citation qualifier [%d]", snum);
				}
   			} 
		}
	} /* sap*/
}

/***************************************************************************
*   DeleteSites:
*   -- deletes all ImpFeat "sites"
****************************************************************************/
NLM_EXTERN void DeleteSites (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	BioseqPtr       bsp = NULL;
	BioseqSetPtr    bssp = NULL;
	SeqAnnotPtr     sap, ap, apnext;
	SeqFeatPtr		sfp, cur, curnext;
	ImpFeatPtr		ifp;
	
	if (IS_Bioseq(sep)) {
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		sap = bsp->annot;
	}
	else {
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		sap = bssp->annot;
	}
	for (ap = sap; ap != NULL; ap = apnext) {
		apnext = ap->next;
		sfp = (SeqFeatPtr)(ap->data);
		if (ap->type != 1) {
			continue;
		}
	 	for (cur = sfp; cur; cur = curnext) {
       		curnext = cur->next;
			if (cur->data.choice != SEQFEAT_IMP) {
				continue;
			}
			ifp = cur->data.value.ptrvalue;
			if (StringCmp(ifp->key, "Site-ref") != 0) {
				continue;
			}
			sfp = remove_feat(sfp, cur);
			if (sfp == NULL) {
				break;
			}
		}
		ap->data = sfp;
	}
	
	for (ap = sap; ap != NULL; ap = apnext) {
		apnext = ap->next;
		if (ap->data == NULL) {
			sap = remove_annot(sap, ap);
		}
	}
	
	if (bssp != NULL) {
		bssp->annot = sap;
	} else {
		bsp->annot = sap;
	}
	return;
}

static int LIBCALLBACK CompareByDate (VoidPtr vp1, VoidPtr vp2)
{

	PubStructPtr psp1, psp2;
	Int2 status;
	ValNodePtr vnp1, vnp2;
	ValNodePtr PNTR vnpp1;
	ValNodePtr PNTR vnpp2;

	vnpp1 = (ValNodePtr PNTR) vp1;
	vnpp2 = (ValNodePtr PNTR) vp2;
	vnp1 = *vnpp1;
	vnp2 = *vnpp2;

	psp1 = vnp1->data.ptrvalue;
	psp2 = vnp2->data.ptrvalue;

/* First, three cases that should not be, need to be checked for. */
	if (psp1 == NULL && psp2 != NULL)
		return 1;
	if (psp1 != NULL && psp2 == NULL)
		return -1;
	if (psp1 == NULL && psp2 == NULL)
		return 0;
	status = DateMatch(psp1->date, psp2->date, FALSE);
	if (status == -2) {
		status = 2;
	}

	return status;
}

/***************************************************************************
*
*	"CompareSnForHeap" compares serial numbers, if they exist,
*	to sort pubs in that manner.
*
***************************************************************************/

static int LIBCALLBACK CompareSnForHeap (VoidPtr vp1, VoidPtr vp2)

{
	PubStructPtr psp1, psp2;
	ValNodePtr vnp1, vnp2;
	ValNodePtr PNTR vnpp1;
	ValNodePtr PNTR vnpp2;

	vnpp1 = (ValNodePtr PNTR) vp1;
	vnpp2 = (ValNodePtr PNTR) vp2;
	vnp1 = *vnpp1;
	vnp2 = *vnpp2;

	psp1 = vnp1->data.ptrvalue;
	psp2 = vnp2->data.ptrvalue;

/* First, three cases that should not be, need to be checked for. */
	if (psp1 == NULL && psp2 != NULL)
		return 1;
	if (psp1 != NULL && psp2 == NULL)
		return -1;
	if (psp1 == NULL && psp2 == NULL)
		return 0;

	if (psp1->number > psp2->number)
		return 1;
	else if (psp1->number < psp2->number)
		return -1;
	else 
		return 0;
/* Two pubs should not have the same serial number, but that case is covered 
anyway by the line above.*/
}

/***************************************************************************
*
*	"CompareStartForHeap" compares "start" numbers, found in
*	the PubStruct, so that pubs are sorted into the order
*	descrip, cit, and feat.
*
***************************************************************************/
/*______________________________________________________________________
**
**	This code is not currently used.
**	I do not remove this piece of code, just comment it out.
**	-- Dmitri Lukyanov
*/
#if 0

static int LIBCALLBACK CompareStartForHeap (VoidPtr vp1, VoidPtr vp2)

{
	PubStructPtr psp1, psp2;
	ValNodePtr vnp1, vnp2;
	ValNodePtr PNTR vnpp1;
	ValNodePtr PNTR vnpp2;

	vnpp1 = (ValNodePtr PNTR) vp1;
	vnpp2 = (ValNodePtr PNTR) vp2;
	vnp1 = *vnpp1;
	vnp2 = *vnpp2;

	psp1 = vnp1->data.ptrvalue;
	psp2 = vnp2->data.ptrvalue;


/* First, three cases that should not be, need to be checked for. */
	if (psp1 == NULL && psp2 != NULL)
		return 1;
	if (psp1 != NULL && psp2 == NULL)
		return -1;
	if (psp1 == NULL && psp2 == NULL)
		return 0;

	if (psp1->start > psp2->start)
		return 1;
	else if (psp1->start < psp2->start)
		return -1;
	else 	/* If the two start values are equal */
		return 0;

}

#endif
/*______________________________________________________________________
*/

void VnpHeapSort (ValNodePtr PNTR vnp, int (LIBCALLBACK *compar )PROTO ((Nlm_VoidPtr, Nlm_VoidPtr )))	

{
	Int4 index, total;
	ValNodePtr vnp1;
	ValNodePtr PNTR temp;

	if (vnp == NULL  ||  *vnp == NULL)  return;  /* do nothing */

	total=0;
	for (vnp1 = *vnp; vnp1; vnp1=vnp1->next)
		total++;

	temp = (ValNodePtr PNTR) MemNew(total*sizeof(ValNodePtr));

	index=0;
	for (vnp1 = *vnp; vnp1; vnp1=vnp1->next)
	{
		temp[index] = vnp1;
		index++;
	}

	HeapSort ((VoidPtr) temp, (size_t) index, sizeof(ValNodePtr), compar);

	*vnp = temp[0];
	for (vnp1 = *vnp, index=0; index<(total-1); vnp1=vnp1->next, index++)
	{
		vnp1->next = temp[index+1];
	}
	vnp1 = temp[total-1];
	vnp1->next = NULL;

	temp = MemFree(temp);
}

static void PubHeapSort (ValNodePtr PNTR vnp, int (LIBCALLBACK *compar )PROTO ((Nlm_VoidPtr, Nlm_VoidPtr )))	

{
	PubStructPtr psp;
	ValNodePtr v, ex_v, v_sub=NULL, v_unp=NULL, vnext;

	for (v = *vnp; v != NULL; v = vnext) {
		vnext = v->next;
		psp = (PubStructPtr) v->data.ptrvalue;
		if(psp->choice == PUB_Sub) {
			ex_v = extract_node(vnp, v);
			v_sub = tie_next(v_sub, ex_v);
		} else if (psp->choice == PUB_Gen) {
			ex_v = extract_node(vnp, v);
			v_unp = tie_next(v_unp, ex_v);
		}
	}		
	if (*vnp) {
		VnpHeapSort(vnp, compar);
	}
	if (v_unp) {
		VnpHeapSort(&(v_unp), compar);
	}
	if (v_sub) {
		VnpHeapSort(&(v_sub), compar);
	}
	
	*vnp = tie_next(*vnp, v_unp);
	*vnp = tie_next(*vnp, v_sub);
	
}

/**************************************************************************
*	"OrganizePubList" sorts the pubs (that is a chain of ValNodes with
*	PubStructPtr in data.ptrvalue.  
*	First a check is done for serial numbers and, if they exist, the pubs are
*	output in order without further work.  If not, they are and
*	sorted by date.  
* 	No unique here because it's already done in CheckPubs (Tatiana)
**************************************************************************/
NLM_EXTERN ValNodePtr OrganizePubList(ValNodePtr vnp)
{
	Int2 index;
	PubStructPtr psp;
	ValNodePtr vnp1;
	Boolean numbered=TRUE, ordered=TRUE;

	for (vnp1=vnp; vnp1; vnp1=vnp1->next)
	{
		psp = vnp1->data.ptrvalue;
		if (psp->number == -1)
		{
			numbered = FALSE;
			break;
		}
	}
	
	if (numbered == TRUE)
	{
		for (vnp1=vnp, index=0; vnp1; vnp1=vnp1->next)
		{
			index++;
			psp = vnp1->data.ptrvalue;
			if (psp->number != index)
			{
				ordered = FALSE;
				break;
			}
		}

		if (ordered != TRUE)
		{
			VnpHeapSort(&(vnp), CompareSnForHeap);
		}
	}
	else
	{
	/*	VnpHeapSort(&(vnp), CompareStartForHeap); */
		PubHeapSort(&(vnp), CompareByDate); 
	}

/* New 	serial numbers are assigned for both non-ordered and non-numbered,
as an numbered set of pubs could all have redundant serial numbers. 
Could this cause problems if serial numbers are referenced in the feature
table?????????*/
	if (!ordered || !numbered)
	{
		for (vnp1=vnp, index=0; vnp1; vnp1=vnp1->next)
		{
			index++;
			psp = vnp1->data.ptrvalue;
			psp->number = index;
		}
	}
	return vnp;

}

NLM_EXTERN void cleanup_pub(ValNodePtr pub)
{
	ValNodePtr vnp;
	
	for (vnp = pub; vnp; vnp=vnp->next) {
		FreePubStruct(vnp->data.ptrvalue);
		
	}
}

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

static ValNodePtr del_serial_number(ValNodePtr pub)
{
	CitGenPtr  cit;
	ValNodePtr head = NULL, next, prev = NULL;

	while (pub != NULL)
	{
		next = pub->next;
		pub->next = NULL;
	
		if (pub->choice == PUB_Gen) {
			cit = pub->data.ptrvalue;
			if (cit != NULL) {
				if (cit->serial_number != -1) {
					cit->serial_number = -1;
				}
			}
			if (empty_citgen(cit)) {
				PubFree(pub);
				pub = NULL;
			}
		}
		if (pub != NULL)  /* still have one */
		{
			if (head == NULL)
				head = pub;
			else
				prev->next = pub;
			prev = pub;
		}
		pub = next;
	}
	return head;
}
/*****************************************************************************
*
*	remove all Cit-Gen with serial numbers. Pubs will be sorted by date and
*	serial numbers reassigned in flat file generator
*
*****************************************************************************/
static void StripSerialNumber (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	BioseqPtr 		bsp;
	BioseqSetPtr	bssp;
	ValNodePtr		tmp, descr;
	SeqAnnotPtr		sap, annot;
	SeqFeatPtr		sfp;
	ImpFeatPtr      ifp;
	PubdescPtr  	pdp;
	ValNodePtr      pubset, pub_e, pub;

	if (IS_Bioseq(sep)) {
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		descr = bsp->descr;
		annot = bsp->annot;
	} else {
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		descr = bssp->descr;
		annot = bssp->annot;
	}
	for (tmp = descr; tmp != NULL; tmp = tmp->next) {
		if (tmp->choice == Seq_descr_pub) {
			pdp = tmp->data.ptrvalue;
			if (pdp != NULL) {
				pdp->pub = del_serial_number(pdp->pub);
			}
		}	
	}
	for (sap = annot; sap != NULL; sap = sap->next) {
		if (sap->type != 1) {
			continue;
		}
		for (sfp = sap->data; sfp != NULL; sfp = sfp->next) {
			if (sfp->data.choice == SEQFEAT_PUB) {
				pdp = sfp->data.value.ptrvalue;
				pdp->pub = del_serial_number(pdp->pub);
			}
			if (sfp->data.choice == SEQFEAT_IMP) {
				ifp = sfp->data.value.ptrvalue;
				if (StringCmp(ifp->key,"Site-ref") == 0) {
					pubset = sfp->cit;
					pub_e = pubset->data.ptrvalue;
					for (; pub_e != NULL; pub_e = pub_e->next) {
						pub = pub_e->data.ptrvalue;
						pub_e->data.ptrvalue = del_serial_number(pub);
					}
				}
			}
		}
	} 	
}

static void CheckForSwissProtID (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr  bsp;
  SeqIdPtr   sip;
  BoolPtr    stripSerial;

  if (sep == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp == NULL) return;
    stripSerial = (BoolPtr) mydata;
    if (stripSerial == NULL) return;
    for (sip = bsp->id; sip != NULL; sip = sip->next) {
      if (sip->choice == SEQID_SWISSPROT) {
        *stripSerial = FALSE;
      }
    }
  }
}

NLM_EXTERN void EntryStripSerialNumber (SeqEntryPtr sep)
{
	Boolean  stripSerial = TRUE;

	if (sep == NULL) return;
	SeqEntryExplore (sep, (Pointer) &stripSerial, CheckForSwissProtID);
	if (stripSerial) {
		SeqEntryExplore(sep, NULL, StripSerialNumber);
	}
}

NLM_EXTERN ValNodePtr remove_node(ValNodePtr head, ValNodePtr x)
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

NLM_EXTERN SeqAnnotPtr remove_annot(SeqAnnotPtr head, SeqAnnotPtr x)
{
	SeqAnnotPtr	v, p;
	
	if (head == NULL) {
		return NULL;
	}
	if (x == head) {
		head = x->next;
		x->next = NULL;
		SeqAnnotFree(x);
		return head;
	}
	for (v = head; v != NULL && v != x; v = v->next) {
		p = v;
	}
	if (v != NULL) {
		p->next = x->next;
		x->next = NULL;
		SeqAnnotFree(x);
	}
	return head;
}

NLM_EXTERN ValNodePtr extract_node(ValNodePtr PNTR head, ValNodePtr x)
{
	ValNodePtr	v, p;
	
	if (*head == NULL) {
		return NULL;
	}
	if (x == *head) {
		*head = x->next;
		x->next = NULL;
		return x;
	}
	for (v = *head; v != NULL && v != x; v = v->next) {
		p = v;
	}
	if (v == NULL) {
		return NULL;
	}
	p->next = x->next;
	x->next = NULL;
	return x;
}

NLM_EXTERN SeqFeatPtr remove_feat(SeqFeatPtr head, SeqFeatPtr x)
{
	SeqFeatPtr	v, p;
	
	if (head == NULL) {
		return NULL;
	}
	if (x == head) {
		head = x->next;
		x->next = NULL;
		SeqFeatFree(x);
		return head;
	}
	for (v = head; v != NULL && v != x; v = v->next) {
		p = v;
	}
	if (v != NULL) {
		p->next = x->next;
		x->next = NULL;
		SeqFeatFree(x);
	}
	return head;
}


/***************************************************************************
*  qvalue_extract:
*  -- return qual's value if found the "qual" in the "qlist", and remove
*     the "qual" from the qlist; Otherwise, return NULL
***************************************************************************/
NLM_EXTERN CharPtr qvalue_extract(GBQualPtr PNTR qlist, CharPtr qual)
{
   GBQualPtr curq, preq;
   CharPtr   qvalue = NULL;

   for (preq = NULL, curq = *qlist; curq != NULL; curq = curq->next) {
       if (StringCmp(curq->qual, qual) == 0) {
          if (preq == NULL)
             preq = *qlist = curq->next;
          else
             preq->next = curq->next;

          curq->next = NULL;
          qvalue = StringSave(curq->val); 

          GBQualFree(curq);
          curq = NULL;

          return (qvalue);
       }

       preq = curq;
   }

   return (qvalue);

}
