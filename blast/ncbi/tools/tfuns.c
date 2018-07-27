static char const rcsid[] = "$Id: tfuns.c,v 6.12 2004/04/01 13:43:08 lavr Exp $";

#include <stdio.h>
#include <ncbi.h>
#include <objpub.h>
#include <toasn3.h>
#include <tfuns.h>
#include <subutil.h>
#include <utilpub.h>
#include <gather.h>
#include <terr.h>

static char *this_file = "tfuns.c";
#ifdef THIS_FILE
#undef THIS_FILE
#endif
#define THIS_FILE this_file
static char *this_module ="toasn3";
#ifdef THIS_MODULE
#undef THIS_MODULE
#endif
#define THIS_MODULE this_module

Uint1 AAForCodon (Uint1Ptr codon, CharPtr codes); /* in seqport.c */

CharPtr type[] = {
"chromosome", "map", "clone", "sub_clone", "haplotype", "genotype", "sex",
"cell_line", "cell_type", "tissue_type", "clone_lib", "dev_stage", 
"frequency", "germline", "rearranged", "lab_host", "pop_variant",
"tissue_lib", "plasmid", "transposon", "insertion_seq", "plastid"};

static ORGMOD orgmod_subtype[10] = {
	{ "strain", 2 }, {"sub_strain", 3}, {"variety", 6}, {"serotype",7}, {"cultivar",10}, {"isolate", 17}, {"specific_host", 21}, {"sub_species", 22}, {"note", 255}, { NULL, 0 } 
};

CharPtr true_qual[3] = {"insertion_seq", "transposon", "clone"};

/*****************************************************************************
*   FlatSafeSize:
*     -- Set the limit to heap ptr used in Greg's error function
*     -- return truncated ptr (Greg's limit is 512)
******************************************************************************/
CharPtr FlatSafeSize(CharPtr ptr, Int4 limit)
{
	Int4 len;
	CharPtr retval = ptr;
	
	if (ptr != NULL) {
		len = StringLen(ptr);
		if (len > limit) {
			retval = MemNew(limit+1);
			StringNCpy(retval, ptr, limit);
			MemFree(ptr);
		}
	}
	return retval;
}
/*****************************************************************************
*	Implementation of various string processing functions 
******************************************************************************/
/*============================================================================*\
 *	StrStripSpaces:
 *	Strips all spaces in string in following manner. If the function
 *	meet several spaces (spaces and tabs) in succession it replaces them
 *	with one space.

\*----------------------------------------------------------------------------*/
CharPtr StrStripSpaces(CharPtr pchSrc, CharPtr pchDest)
{
    CharPtr	pchReturn;
    Boolean	bSpace = FALSE;

    if (pchDest == NULL)
		pchDest = MemNew(StrLen(pchSrc));

    if (pchDest == NULL)
		return NULL;

    pchReturn = pchDest;
    
    while (*pchSrc != '\0') {
		if (*pchSrc == ' ' || *pchSrc == '\t') {
	    	if (!bSpace) {
				*pchDest++ = ' ';
				bSpace = TRUE;
	    	}
		} else {
	    	*pchDest++ = *pchSrc;
	    	bSpace = FALSE;
		}
		pchSrc++;
    }
    *pchDest = '\0';

    return pchReturn;
}

/*============================================================================*\
 *	StringIsEmpty:
 *	Checks is string empty. Empty means that string has zero length or
 *	contains white spaces (see isspace()) only.
 * Return:
 *	TRUE if string is empty otherwise FALSE.
 *	If pointer to a string equals NULL the function returns TRUE.
\*----------------------------------------------------------------------------*/
Boolean IsStringEmpty(CharPtr str)
{
 	if (str == NULL)
 		return TRUE;
 	if (*str == '\0')
 		return TRUE;
    while (*str) {
		if (!isspace(*str)) {
	    	return FALSE;
		} else {
	    	str++;
	    }
	}

    return TRUE;
}

/*============================================================================*\
 *	ShortenString:
 *	Makes new copy of a string with new length <= given.
 *
 * Note:
 *	This function allocate memory for a new string by MemNew, so the caller
 *	have to call MemFree to free the memory.
\*----------------------------------------------------------------------------*/
CharPtr ShortenString(CharPtr str, Int4 iLen)
{
    CharPtr	pchNew;
    Int4	iNewLen;

    iNewLen = StringLen(str);
    iNewLen = iNewLen < iLen ? iNewLen + 1 : iLen + 1;
    pchNew = MemNew(iNewLen);
    if (pchNew == NULL)
	return NULL;

    StringNCpy(pchNew, str, iNewLen - 1);

    return pchNew;
}

/*============================================================================*\
 *	StringNStr:
 *	Like StringStr, but search is limited by given length.
 *
 * Note:
 *	If the substring is empty (has zero length) the function returns 
 *	pchSource. If length of substring is > cMax, the function returns NULL.
 *
\*----------------------------------------------------------------------------*/
CharPtr StringNStr(CharPtr pchSource, CharPtr pchTemplate, size_t cMax)
{
    CharPtr pchResult;
    size_t  cTemplLen;

    if (pchSource == NULL || pchTemplate == NULL || cMax == 0)
	return NULL;

    cTemplLen = StringLen(pchTemplate);
    if ( cTemplLen == 0)
		return pchSource;

    if (cTemplLen > cMax)
		return NULL;

    pchResult = StringStr(pchSource, pchTemplate);
    if (pchResult == NULL)
		return NULL;

    if (pchResult + cTemplLen > pchSource + cMax)
	 	return NULL;

    return pchResult;
}

Int2 StringExtCmp(CharPtr s1, CharPtr s2)
{
	if (s1 == NULL || s2 == NULL)
		return 1;
	return StringCmp(s1, s2);
}

CharPtr  StripAllSpace(CharPtr str) 
{
	CharPtr s, new;

	if (str == NULL) {
		return NULL;
	}
	new = MemNew(strlen(str) + 1);
	for (s = new; *str != '\0'; str++) {
		if (!isspace(*str)) {
			*s++ = *str;
		}
	}
	*s = '\0';
	return new;
}

Int2 StringIgnoreSpaceCmp(CharPtr s1, CharPtr s2)
{
	Int2 retval = 0;
	
	if (s1 == NULL || s2 == NULL) {
		return retval;
	}
	s1 = StripAllSpace(s1);
	s2 = StripAllSpace(s2);
	retval = StringICmp(s1, s2);
	MemFree(s1);
	MemFree(s2);
	
	return retval;
}
Int2 AStringIgnoreSpaceCmp(CharPtr s1, CharPtr s2)
{
	Int2 retval = 0;
	Int2 c1, c2;
	
	if (s1 == NULL || s2 == NULL) {
		return retval;
	}
	for (; *s1 != '\0' && *s2 != '\0'; s1++, s2++) {
		for (; isspace(*s1); s1++) ;
		for (; isspace(*s2); s2++) ;
		c1 = islower(*s1) ? toupper(*s1) : *s1;
		c2 = islower(*s2) ? toupper(*s2) : *s2;
		if ((retval = (c1 - c2)) != 0) {
			break;
		}
	}
	return retval;
}

/*----------------------------------------------------------------------------*/

WholeFeatPtr WholeFeatNew(void)
{
	WholeFeatPtr wfp;
	
	wfp = MemNew(sizeof(WholeFeat));
	wfp->count = 0;
	wfp->sfp = NULL;
	
	return wfp;
}

void WholeFeatFree(WholeFeatPtr wfp)
{
	SeqFeatPtr fp, fpnext;
	
	for (fp = wfp->sfp; fp; fp = fpnext) {
		fpnext = fp->next;
		MemFree(fp);
	}
	MemFree(wfp);
}

/****************************************************************************
*  tie_feat:
*  -- 
* links input to list specified by head pointer               01-12-94
****************************************************************************/
SeqFeatPtr tie_feat(SeqFeatPtr head, SeqFeatPtr next)
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

MolFixPtr tie_next_mol(MolFixPtr head, MolFixPtr next)
{
	MolFixPtr v;

	if (head == NULL) {
		return next;
	}
	for (v = head; v->next != NULL; v = v->next) {
		v = v;
	}
	v->next = next;
	return head;
}

CodeBreakPtr tie_next_cbp(CodeBreakPtr head, CodeBreakPtr next)
{
   CodeBreakPtr v;

   if (head == NULL) {
      return next;
   }
   for (v = head; v->next != NULL; v = v->next)
   		continue;    
   v->next = next;
   return head;
}
/****************************************************************************
*  tie_next_biosource:
*  -- ties next BioSource to the end of the chain
*                                                                    08-4-93
****************************************************************************/
OrgFixPtr tie_next_biosource(OrgFixPtr head, OrgFixPtr next)
{
	OrgFixPtr v;

	if (head == NULL) {
		return next;
	}
	for (v = head; v->next != NULL; v = v->next) {
		v = v;
	}
	v->next = next;
	return head;
}

/****************************************************************************
*  tie_next_subtype:
*  -- ties next SubSource to the end of the chain
****************************************************************************/
SubSourcePtr tie_next_subtype(SubSourcePtr head, SubSourcePtr next)
{
	SubSourcePtr v;
	CharPtr	sv, sn;
	static Char msg1[51], msg2[51];
	CharPtr p;
	CharPtr q;

	if (head == NULL) {
		return next;
	}
	if (next == NULL) {
		return head;
	}
	if ((sn = next->name) == NULL) {
		sn = "";
	}
	for (v = head; v != NULL; v = v->next) {
		if ((sv = v->name) == NULL) {
			sv = "";
		}
		if (v->subtype == next->subtype) { 
			if (AStringIgnoreSpaceCmp(sv, sn) == 0) {
				SubSourceFree(next);
				return head;
			} else {
				p = StringSave(sv);
				StringNCpy(msg1, p, 50);
				MemFree(p);
				q = StringSave(sn);
				StringNCpy(msg2, q, 50);
				MemFree(q);
				if (v->subtype <  23) {
					ErrPostEx(SEV_WARNING, ERR_SOURCE_QualDiffValues,
						"/%s different values: %s|%s", type[v->subtype-1], msg1, msg2);
				} else if (v->subtype == 255) {
					ErrPostEx(SEV_WARNING, ERR_SOURCE_QualDiffValues,
						"/note different values: %s|%s", msg1, msg2);
				} else {
					ErrPostEx(SEV_WARNING, ERR_SOURCE_IllegalQual,
						"%d", v->subtype);
				}
			}
		}
		if (v->next == NULL) {
			break;
		}
	}
	v->next = next;
	return head;
}

/****************************************************************************
*  tie_next_OrgMod:
*  -- ties next OrgMod to the end of the chain
*                                                                    08-4-93
****************************************************************************/
OrgModPtr tie_next_OrgMod(OrgModPtr head, OrgModPtr next)
{
	OrgModPtr v;
	CharPtr	sv, sn;
	Int2 i;
	static Char msg1[51], msg2[51];
	CharPtr p;
	CharPtr q;

	if (head == NULL) {
		return next;
	}
	if ((sn = next->subname) == NULL) {
		sn = "";
	}
	for (v = head; v != NULL; v = v->next) {
		if ((sv = v->subname) == NULL) {
			sv = "";
		}
		if (v->subtype == next->subtype) {
			if (AStringIgnoreSpaceCmp(v->subname, next->subname) == 0) {
				OrgModFree(next);
				return head;
			} else {
				for (i=0; i < 10; i++) {
					if (v->subtype == orgmod_subtype[i].num)
						break;
				}
				if (i == 10) {
					ErrPostEx(SEV_INFO, ERR_SOURCE_QualUnknown,
		"OrgMod.subtype [%d] can't be mapped to GenBank qualifier", v->subtype);
				} else {
					p = StringSave(sv);
					StringNCpy(msg1, p, 50);
					MemFree(p);
					q = StringSave(sn);
					StringNCpy(msg2, q, 50);
					MemFree(q);
					ErrPostEx(SEV_WARNING, ERR_SOURCE_QualDiffValues,
				"/%s different values: %s|%s", 
		 						orgmod_subtype[i].name, msg1, msg2);
		 		}
		 	}
		}
		if (v->next == NULL) {
			break;
		}
	}
	v->next = next;
	return head;
}

/*****************************************************************************
*
*   SeqFeatPtr SeqFeatExtract(headptr, choice)
*       removes first feature in chain where ->choice == choice
*       rejoins chain after removing the node
*       sets node->next to NULL
*
*****************************************************************************/
SeqFeatPtr SeqFeatExtract (SeqFeatPtr PNTR headptr, Nlm_Int2 choice)
{
    SeqFeatPtr last = NULL,
        vnp = * headptr;

    while (vnp != NULL)
    {
        if (vnp->data.choice == (Nlm_Uint1)choice)
        {
            if (last == NULL)    /* first one */
                * headptr = vnp->next;
            else
                last->next = vnp->next;

            vnp->next = NULL;
            return vnp;
        }
        else
        {
            last = vnp;
            vnp = vnp->next;
        }
    }

    return NULL;    /* not found */
}

/*****************************************************************************
*
*   SeqFeatPtr SeqFeatExtractList(headptr, choice)
*       removes ALL features in chain where ->choice == choice
*       rejoins chain after removing the nodes
*       returns independent chain of extracted nodes
*
*****************************************************************************/
SeqFeatPtr SeqFeatExtractList (SeqFeatPtr PNTR headptr, Nlm_Int2 choice)
{
    SeqFeatPtr last = NULL, first = NULL, vnp;

    while ((vnp = SeqFeatExtract(headptr, choice)) != NULL)
    {
		if (last == NULL)
		{
			last = vnp;
			first = vnp;
		}
		else
			last->next = vnp;
		last = vnp;
	}

    return first;
}
/*****************************************************************************
*
*   SeqFeatPtr SourceFeatExtract(headptr)
*       removes first feature in chain where ->choice == SEQFEAT_IMP
*       and qual "source"
*       rejoins chain after removing the node
*       sets node->next to NULL
*
*****************************************************************************/
SeqFeatPtr SourceFeatExtract (SeqFeatPtr PNTR headptr)
{
    SeqFeatPtr last = NULL,
        vnp = * headptr;
    ImpFeatPtr	ifp;

    while (vnp != NULL)
    {
        if (vnp->data.choice == SEQFEAT_IMP) {
        	ifp = (ImpFeatPtr) vnp->data.value.ptrvalue;
        	if (StringCmp(ifp->key, "source") == 0) {
            	if (last == NULL)    /* first one */
                	* headptr = vnp->next;
            	else
                	last->next = vnp->next;

            	vnp->next = NULL;
            	return vnp;
            } else {
            	last = vnp;
            	vnp = vnp->next;
            }
        } else {
            last = vnp;
            vnp = vnp->next;
        }
    }

    return NULL;    /* not found */
}
/*****************************************************************************
*
*   SeqFeatPtr ExtractSourceFeatList(headptr, choice)
*       removes ALL features in chain where ->choice == choice
*       and qual "source"
*       rejoins chain after removing the nodes
*       returns independent chain of extracted nodes
*
*****************************************************************************/
SeqFeatPtr ExtractSourceFeatList (SeqFeatPtr PNTR headptr)
{
    SeqFeatPtr last = NULL, first = NULL, vnp;

    while ((vnp = SourceFeatExtract(headptr)) != NULL)
    {
		if (last == NULL)
		{
			last = vnp;
			first = vnp;
		}
		else
			last->next = vnp;
		last = vnp;
	}

    return first;
}

SeqIdPtr find_id(SeqEntryPtr sep)
{
	SeqIdPtr sip;
	BioseqPtr	bsp;
	
		if (IS_Bioseq(sep))
	{
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		sip = (SeqIdPtr) SeqIdDup(bsp->id);
		return sip;
	}
	else {
		return NULL;
	}

}

SubSourcePtr remove_subtype(SubSourcePtr head, SubSourcePtr x)
{
	SubSourcePtr	v, p;
	
	if (head == NULL) {
		return NULL;
	}
	if (x == NULL) {
		return SubSourceSetFree (head);
	}
	if (x == head) {
		head = x->next;
		x->next = NULL;
		SubSourceFree(x);
		return head;
	}
	for (v = head; v != NULL && v != x; v = v->next) {
		p = v;
	}
	if (v != NULL) {
		p->next = x->next;
		x->next = NULL;
		SubSourceFree(x);
	}
	return head;
}

OrgModPtr remove_OrgMod(OrgModPtr head, OrgModPtr x)
{
	OrgModPtr	v, p;
	
	if (head == NULL) {
		return NULL;
	}
	if (x == NULL) {
		return OrgModSetFree (head);
	}
	if (x == head) {
		head = x->next;
		x->next = NULL;
		OrgModFree(x);
		return head;
	}
	for (v = head; v != NULL && v != x; v = v->next) {
		p = v;
	}
	if (v != NULL) {
		p->next = x->next;
		x->next = NULL;
		OrgModFree(x);
	}
	return head;
}

SeqFeatXrefPtr remove_xref(SeqFeatXrefPtr head, SeqFeatXrefPtr x)
{
	SeqFeatXrefPtr	v, p;
	
	if (head == NULL) {
		return NULL;
	}
	if (x == head) {
		head = x->next;
		x->next = NULL;
		SeqFeatXrefFree(x);
		return head;
	}
	for (v = head; v != NULL && v != x; v = v->next) {
		p = v;
	}
	if (v != NULL) {
		p->next = x->next;
		x->next = NULL;
		SeqFeatXrefFree(x);
	}
	return head;
}

GBQualPtr remove_qual(GBQualPtr head, GBQualPtr x)
{
	GBQualPtr	v, p;
	
	if (head == NULL) {
		return NULL;
	}
	if (x == head) {
		head = x->next;
		x->next = NULL;
		GBQualFree(x);
		return head;
	}
	for (v = head; v != NULL && v != x; v = v->next) {
		p = v;
	}
	if (v != NULL) {
		p->next = x->next;
		x->next = NULL;
		GBQualFree(x);
	}
	return head;
}

Boolean check_whole(SeqFeatPtr f, Int4 len)
{
	Boolean whole = FALSE;
	SeqLocPtr		slp;
	SeqIntPtr		sip;
	
		slp = f->location;
		if (slp->choice == SEQLOC_WHOLE) {
			whole = TRUE;
		} else if (slp->choice == SEQLOC_INT) {
			sip = slp->data.ptrvalue;
			if (sip->from == 0 && sip->to == len-1) {
				whole = TRUE;
			}
		}
	return whole;
}

/* check for /transposon and /insertion_seq and /clone*/

Boolean true_multiple(SeqAnnotPtr sap, Int4 len)
{
	SeqFeatPtr	tmp_sfp, sfp, f;
	Boolean first = TRUE;
	CharPtr word = NULL;
	GBQualPtr		q;
	Int2	i;

		tmp_sfp = (SeqFeatPtr) (sap->data);
		sfp = ExtractSourceFeatList(&(tmp_sfp)); 
		for (f = sfp; f != NULL; f = f->next) {
			if (check_whole(f, len)) {
				continue;
			}
			for(q = f->qual; q != NULL; q = q->next) {
				if (first) {
					for (i=0; i<3; i++) {
						if (StringCmp(q->qual, true_qual[i]) == 0) {
							first = FALSE;
							word = true_qual[i];
							break;
						}
					}
				if (word != NULL)
					break;
				} else {
					if (StringCmp(q->qual, word) == 0) {
						break;
					}
				}
			}
			if (word == NULL) {
				sfp = tie_feat(sfp, tmp_sfp);
				if (sap != NULL && sap->type == 1) {
					if (sfp)
						sap->data = sfp;
				} 
				return TRUE;
			}
			if (q == NULL) {
				sfp = tie_feat(sfp, tmp_sfp);
				if (sap != NULL && sap->type == 1) {
					if (sfp)
						sap->data = sfp;
				} 
				return TRUE;
			}
		}
		sfp = tie_feat(sfp, tmp_sfp);
		if (sap != NULL && sap->type == 1) {
			if (sfp)
				sap->data = sfp;
		} 
		return FALSE;
}

/***************************************************************************
*  copy_qvalue:
*  -- return qual's value if found the "qual" in the "qlist",
***************************************************************************/
CharPtr copy_qvalue(GBQualPtr qlist, CharPtr qual)
{
   GBQualPtr curq;
   CharPtr   qvalue = NULL;

   for (curq = qlist; curq != NULL; curq = curq->next) {
       if (StringCmp(curq->qual, qual) == 0) {
       	  if (curq->val != NULL) { 
          	if (StringCmp(curq->val, "\"\"") == 0) {
   			  return (NULL);
          	} else {
              qvalue = StringSave(curq->val);
   			  return (qvalue);
   		  	}
   		  }
       }
   }

   return (qvalue);

}

/**********************************************************/
CharPtr get_qvalue(GBQualPtr curq, CharPtr qual)
{
    for(; curq != NULL; curq = curq->next)
        if(StringCmp(curq->qual, qual) == 0 && curq->val != NULL)
            break;

    if(curq == NULL || StringCmp(curq->val, "\"\"") == 0)
        return(NULL);

    return(curq->val);
}

/*****************************************************************************
*   laststr:
*     -- strstr, last occurrence
******************************************************************************/
CharPtr laststr(CharPtr str, CharPtr word)
{
	CharPtr s, s1;
	
	s1 = str-1;
	while(( s = StringStr(s1+1, word)) !=NULL) {
		s1 = s;
	}
	if (s1 == str-1) {
		return NULL;
	}
	return s1;
}

void DelFeatFromList (SeqFeatPtr PNTR gbqp, SeqFeatPtr curq, SeqFeatPtr preq)
{
    SeqFeatPtr   temp;
     
          if (preq == NULL)
             *gbqp = curq->next;  /* will change first_q in calling function */
          else
             preq->next = curq->next;  /* will cause next_q == preq -> next */

          temp = curq;
          temp->next = NULL;
          SeqFeatFree(temp);
}

void DelNodeFromList (ValNodePtr PNTR gbqp, ValNodePtr curq, ValNodePtr preq)
{
    ValNodePtr   temp;
     
          if (preq == NULL)
             *gbqp = curq->next;  /* will change first_q in calling function */
          else
             preq->next = curq->next;  /* will cause next_q == preq -> next */

          temp = curq;
          temp->next = NULL;
          ValNodeFree(temp);
}

/*****************************************************************************
*  orf
******************************************************************************/

static void find_orf(SeqPortPtr spp, Int2 ir, CharPtr vals, CharPtr codes, ValNodePtr PNTR list, Uint1 strand, SeqIdPtr id, Int2 limit)
{
	Int4 pos;
	Uint1 codon[3], aa;
	Int4 len;
	Int4 start = -1, stop = -1, met_start = -1;
	SeqLocPtr slp = NULL, m_slp = NULL;
	ValNodePtr prev = NULL;
	IntFuzzPtr infrom=NULL, into=NULL;

	SeqPortSeek(spp, ir, SEEK_SET);
	len = spp->totlen;
	for (pos=0; pos < len-2; pos += 3) {
		codon[0] = SeqPortGetResidue(spp);
		codon[1] = SeqPortGetResidue(spp);
		codon[2] = SeqPortGetResidue(spp);
		aa = AAForCodon(codon, codes);
		if (aa == '*') {
			stop = pos+2;
			if (met_start != -2) {
				if (met_start == -1) {
					met_start = 0;
				}	
				if (stop - met_start > limit) {
					if (strand == Seq_strand_plus) {
						m_slp = SeqLocIntNew(met_start+ir, stop+ir, strand, id);
					} else {
						m_slp = SeqLocIntNew(len-1-(stop+ir), 
								len-1-(met_start+ir),strand, id);
					}
				}
				met_start = -2;
			}
			if (start != -2) {
				if (start == -1) {
					start = 0;
				}	
				if (stop - start > limit) {
					if (strand == Seq_strand_plus) {
						slp = SeqLocIntNew(start+ir, stop+ir, strand, id);
					} else {
						slp = SeqLocIntNew(len-1-(stop+ir), len-1-(start+ir),
														 strand, id);
					}
				}
				start = -2;
			}
		}
		if (m_slp && slp) {
			m_slp->next = slp;
			if (prev == NULL) {
				prev = ValNodeAddPointer(list, ir, m_slp);
			} else {
				prev = ValNodeAddPointer(&prev, ir, m_slp);
			}
			m_slp = slp = NULL;
		}
		if (aa == 'M') {
			if (met_start < 0) {
				met_start = pos;
			}
		}
		aa = AAForCodon(codon, vals);
		if (aa == 'M') {
			if (start < 0) {
				start = pos;
			}
		}
	}
/*  treat end of sequence as a stop */
	if (met_start != -2) {
		stop = pos-1;	
		if (met_start == -1) {
			met_start = 0;
		}
		if (stop-met_start > limit) {
			if (strand == Seq_strand_plus) {
				stop = stop+ir;
				if (stop > len-1) {
					stop = len-1;
				}
				m_slp = SeqLocIntNew(met_start+ir, stop, strand, id);
			} else {
				m_slp = SeqLocIntNew(len-1-(stop+ir), len-1-(met_start+ir), strand, id);
			}
		}
	}
	if (start != -2) {
		stop = pos-1;	
		if (start == -1) {
			start = 0;
		}
		if (stop-start > limit) {
			if (strand == Seq_strand_plus) {
				stop = stop+ir;
				if (stop > len-1) {
					stop = len-1;
				}
				slp = SeqLocIntNew(start+ir, stop, strand, id);
			} else {
				slp = SeqLocIntNew(len-1-(stop+ir), len-1-(start+ir), strand, id);
			}
		}
	}
	if (m_slp) {
		m_slp->next = slp;
		ValNodeAddPointer(list, ir, m_slp);
		m_slp = slp = NULL;
	} else if (slp) {
		m_slp = ValNodeNew(NULL);
		m_slp->next = slp;
		ValNodeAddPointer(list, ir, m_slp);
		m_slp = slp = NULL;
	}
	return;
}


static void find_orf_from_circle (SeqPortPtr spp, Boolean flagIsCircle,
                                  CharPtr icodes, CharPtr codes,
                                  ValNodePtr PNTR list, Uint1 strand,
                                  SeqIdPtr id, Int2 limit)
{
  Int4 pos;
  Int2 loop, spec3;
  Uint1 codon[3], aa;
  Int4 len, checklen;
  Int4 start, stop, spec1, spec2;
  SeqLocPtr slp, slpwrap = NULL;
  ValNodePtr smp;
  ValNodePtr prev = NULL;
  Char charcheck;
  CharPtr this_code;

  loop = 0;
  SeqPortSeek (spp, loop, SEEK_SET);
  if (flagIsCircle)
    SeqPortSet_is_circle (spp, TRUE);

  checklen = 0;
  charcheck = 'M';
  this_code = icodes;

  len = spp->totlen;
  start = 0;
  for (pos = 0; pos < len && loop < 3; pos += 3)
  {
    codon[0] = SeqPortGetResidue (spp);
    codon[1] = SeqPortGetResidue (spp);
    codon[2] = SeqPortGetResidue (spp);
    aa = AAForCodon (codon, this_code);
    if (aa == charcheck || slpwrap != NULL)
    {
      if (aa == '*' && charcheck == '*')
      {
        checklen += pos+2-start+1;
        if (checklen > limit)
        {
/* check for stop codon on junction */
          if (pos+2 < len)
          {
            if (strand == Seq_strand_plus)
            {
              stop = pos+2;
              slp = SeqLocIntNew (start, stop, strand, id);
            }
            else
            {
              stop = len-pos-3;
              slp = SeqLocIntNew (len-start-1, stop, strand, id);
            }
          }
          else
          {
            if (strand == Seq_strand_plus)
            {
              stop = pos+2-len;
              slp = SeqLocIntNew (start, len-1, strand, id);
              slp->next = SeqLocIntNew (0, stop, strand, id);
            }
            else
            {
              stop = len+len-pos-3;
              slp = SeqLocIntNew (stop, len-1, strand, id);
              slp->next = SeqLocIntNew (0, len-start-1, strand, id);
            }
          }
          if (slpwrap != NULL)
          {
            ValNodeLink (&slpwrap, slp);
            smp = ValNodeNew (NULL);
            smp->choice = 8;               /* SeqLocMix */
            smp->data.ptrvalue = slpwrap;
            slp = (SeqLocPtr) smp;
          }
          if (prev == NULL)
            prev = ValNodeAddPointer (list, loop, slp);
          else
            prev = ValNodeAddPointer (&prev, loop, slp);
          slpwrap = NULL;
        }
        else /* didn't make the cut */
        {
          if (slpwrap != NULL)
            slpwrap = SeqLocFree (slpwrap);
        }
        checklen = 0;
        charcheck = 'M';
        this_code = icodes;
      }
      if (aa == 'M' && charcheck == 'M')
      {
        start = pos;
        charcheck = '*';
        this_code = codes;
      }
    }
    if (pos+3 < len)
      continue;
/* treat end of sequence special if circle and in or out of frame on loop */
    if (flagIsCircle)
    {
      if (charcheck == '*') /* check for cross junction ORF */
      {
        if (len%3 == 0) /* in frame */
        {
          spec1 = loop;
          spec2 = start+1; /* cause the A could be part of stop codon */
          spec3 = loop - 1;
        }
        else
        {
          spec1 = pos-len+3;
          spec2 = len;
          spec3 = loop - 3;
        }
        if (strand == Seq_strand_plus)
          slp = SeqLocIntNew (start, len-1, strand, id);
        else
          slp = SeqLocIntNew (0, len-start-1, strand, id);
        if (slpwrap != NULL)
          ValNodeLink (&slpwrap, slp);
        else
          slpwrap = slp;
        checklen += len-start;
        for (pos = spec1; pos < spec2 && spec3 < loop; pos += 3)
        {
          codon[0] = SeqPortGetResidue (spp);
          codon[1] = SeqPortGetResidue (spp);
          codon[2] = SeqPortGetResidue (spp);
          aa = AAForCodon (codon, this_code);
          if (aa == '*')
          {
            checklen += pos;
            if (checklen > limit)
            {
/* check for stop codon on junction */
              if (pos+2 < len)
              {
                if (strand == Seq_strand_plus)
                {
                  stop = pos+2;
                  slp = SeqLocIntNew (spec1, stop, strand, id);
                }
                else
                {
                  stop = len-pos-3;
                  slp = SeqLocIntNew (stop, len-1, strand, id);
                }
              }
              else
              {
                if (strand == Seq_strand_plus)
                {
                  stop = pos+2-len;
                  slp = SeqLocIntNew (pos, len-1, strand, id);
                  slp->next = SeqLocIntNew (0, stop, strand, id);
                }
                else
                {
                  stop = len+len-pos-3;
                  slp = SeqLocIntNew (stop, len-1, strand, id);
                  slp->next = SeqLocIntNew (0, len-spec1-1, strand, id);
                }
              }
              ValNodeLink (&slpwrap, slp);  /* slpwrap set to get this far */
              smp = ValNodeNew (NULL);
              smp->choice = 8;              /* SeqLocMix */
              smp->data.ptrvalue = slpwrap;
              slpwrap = (SeqLocPtr) smp;
              if (prev == NULL)
                prev = ValNodeAddPointer (list, loop, slpwrap);
              else
                prev = ValNodeAddPointer (&prev, loop, slpwrap);
              slpwrap = NULL;
            }
            break;
          }
          if (pos+3 < spec2)
            continue;
          if (len%3 != 0)
          {
            if (strand == Seq_strand_plus)
              slp = SeqLocIntNew (spec1, len-1, strand, id);
            else
              slp = SeqLocIntNew (0, len-spec1-1, strand, id);
            ValNodeLink (&slpwrap, slp);    /* slpwrap set to get this far */
            checklen += spec2-spec1;
          }
          SeqPortSeek (spp, pos-spec2+3, SEEK_SET); /* redundant check */
          pos = pos-spec2;
          spec1 = pos+3;
          spec3++;
        }                   /* for (pos = ... break to here */
        if (slpwrap != NULL)
          slpwrap = SeqLocFree (slpwrap);
      }                     /* if (charcheck == '*') looking for a stop */
    }                       /* if (flagCircle) has to be round */
    pos = loop+1;
    SeqPortSeek (spp, pos, SEEK_SET);
    pos -= 3;
    checklen = 0;
    charcheck = 'M';
    this_code = icodes;
    loop++;
  }
  if (slpwrap != NULL)
    slpwrap = SeqLocFree (slpwrap);
  return;
}

static int LIBCALLBACK CompareLoc (VoidPtr vp1, VoidPtr vp2)
{
	ValNodePtr vnp1, vnp2;
	ValNodePtr PNTR vnpp1;
	ValNodePtr PNTR vnpp2;
	Int4 l1, l2;

	vnpp1 = (ValNodePtr PNTR) vp1;
	vnpp2 = (ValNodePtr PNTR) vp2;
	vnp1 = *vnpp1;
	vnp2 = *vnpp2;
	l1 = SeqLocLen((SeqLocPtr) vnp1->data.ptrvalue);
	l2 = SeqLocLen((SeqLocPtr) vnp2->data.ptrvalue);
	
	if (l1 > l2)
		return -1;
	else if (l1 < l2)
		return 1;
	else 
		return 0;
}

static ValNodePtr find_all_orf(BioseqPtr bsp, CharPtr vals, CharPtr codes, Int2 limit, Int4 from, Int4 to)
{
	Int2 ir;
	ValNodePtr list = NULL;
	SeqPortPtr spp;
	SeqLocPtr slp = NULL;
	
	if (from > 0 || to > 0) {
		slp = SeqLocIntNew(from, to, Seq_strand_plus, bsp->id);
		spp = SeqPortNewByLoc(slp, Seq_code_ncbi4na);
	} else {
		spp = SeqPortNew(bsp, 0, -1, Seq_strand_plus, Seq_code_ncbi4na);
	}
	for (ir=0; ir < 3; ir++) {
		find_orf(spp, ir, vals, codes, &list, Seq_strand_plus, bsp->id, limit);
	}
	SeqPortFree(spp);
	if (slp) {
		SeqLocFree(slp);
	}
	if (from > 0 || to > 0) {
		slp = SeqLocIntNew(from, to, Seq_strand_minus, bsp->id);
		spp = SeqPortNewByLoc(slp, Seq_code_ncbi4na);
	} else {
		spp = SeqPortNew(bsp, 0, -1, Seq_strand_minus, Seq_code_ncbi4na);
	}
	for (ir=0; ir < 3; ir++) {
		find_orf(spp, ir, vals, codes, &list, Seq_strand_minus, bsp->id, limit);
	}
	SeqPortFree(spp);
	if (slp) {
		SeqLocFree(slp);
	}
	if (list != NULL) {
		VnpHeapSort(&list, CompareLoc);
	}
	return list;
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

static Int2 GetGcodeFromBioseq(BioseqPtr bsp)
{
	GatherScope gs;
	BioSourcePtr biop;
	Int2 code = -1;
	Uint2	entityID;
	ValNodePtr vnp = NULL;	
	   
	entityID = ObjMgrGetEntityIDForPointer(bsp);
  	MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  	gs.get_feats_location = TRUE;
	MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
	gs.ignore[OBJ_SEQDESC] = FALSE;
	
	GatherEntity(entityID, &vnp, get_src, &gs);
	if (vnp == NULL) {
		ErrPostStr(SEV_WARNING, 0, 0, "BioSource not found");
		return code;
	}
	if (vnp) {
		biop = vnp->data.ptrvalue;
		code = BioSourceToGeneticCode(biop);
		if (code == 0) {
			code = 1;  /* try standard genetic code if it's not found */
		}
	}
	return code;
}

extern ValNodePtr GetOrfList (BioseqPtr bsp, Int2 limit)
{
	Int2 gcode;
	GeneticCodePtr gcp;
	CharPtr vals, codes;
	ValNodePtr vnp, list;

	gcode = GetGcodeFromBioseq(bsp);
	if (gcode == -1)
		gcode = 1;   /* use universal */
	gcp = GeneticCodeFind(gcode, NULL);
	vals = NULL;
	codes = NULL;
	if (gcp == NULL) {
		return NULL;
	}
	for (vnp = (ValNodePtr)gcp->data.ptrvalue; vnp != NULL; vnp = vnp->next)
	{
		if (vnp->choice == 6)   /* sncbieaa */
			vals = (CharPtr)vnp->data.ptrvalue;
		else if (vnp->choice == 3)  /* ncbieaa */
			codes = (CharPtr)vnp->data.ptrvalue;
	}
	if (vals == NULL) {
		vals = codes;
	}
	list = find_all_orf(bsp, vals, codes, limit, 0, 0);
	
	return list;
}

extern ValNodePtr GetAltList (ValNodePtr list)
{
	ValNodePtr vnp;
	SeqLocPtr slp, slpnext;
	
	
	for (vnp = list; vnp; vnp = vnp->next) {
		slp = (SeqLocPtr) vnp->data.ptrvalue;
		if (slp && slp->next) {
			slpnext = slp->next;
			slp->next = NULL;
			slpnext->next = slp;
			vnp->data.ptrvalue = slpnext;
		}
	}
	return list;
}

