/*   asn2ff2.c
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
 * File Name:  asn2ff2.c
 *
 * Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov
 *
 * Version Creation Date:   7/15/95
 *
 * $Revision: 6.37 $
 *
 * File Description: 
 *
 * Modifications:  
 * --------------------------------------------------------------------------
 * Date     Name        Description of modification
 * -------  ----------  -----------------------------------------------------
 *
 * $Log: asn2ff2.c,v $
 * Revision 6.37  2004/06/04 18:39:14  bollin
 * removed unused variables, fixed compiler warning
 *
 * Revision 6.36  2001/12/24 16:21:32  kans
 * initialize urf in GetStrForUserObject
 *
 * Revision 6.35  2001/11/09 20:20:41  kans
 * new GENOME ANNOTATION REFSEQ text
 *
 * Revision 6.34  2001/11/09 15:31:47  kans
 * bail on evidence user object missing name or method
 *
 * Revision 6.33  2001/11/09 15:29:19  kans
 * display curator for reviewed refseq
 *
 * Revision 6.32  2001/11/09 14:45:49  kans
 * added VALIDATED REFSEQ message
 *
 * Revision 6.31  2001/11/09 13:38:58  kans
 * changed from TranscriptModelGeneration to ModelEvidence user object
 *
 * Revision 6.30  2001/10/02 19:16:32  yaschenk
 * Removing memory leaks
 *
 * Revision 6.29  2001/08/07 15:51:08  kans
 * use NUM_SEQID, added third party annotation seqids
 *
 * Revision 6.28  2000/12/27 19:43:20  kans
 * empty comment could lead to original dsp->vnp being freed twice, corrupting memory on Mac
 *
 * Revision 6.27  2000/12/04 20:16:25  tatiana
 * mrna evidence comments added
 *
 * Revision 6.26  2000/08/10 13:54:32  tatiana
 * predicted refseq comment added
 *
 * Revision 6.25  2000/08/07 14:02:55  tatiana
 * printf format fix
 *
 * Revision 6.24  2000/07/11 17:01:43  kans
 * fixed bug of insufficient allocated memory block
 *
 * Revision 6.23  2000/07/06 22:42:00  tatiana
 * REGSEQ text revised
 *
 * Revision 6.22  2000/06/19 16:36:27  kans
 * NULL out ajp->Spop->ptr before calling StdFormatPrint, since string is freed elsewhere leaving ajp->Spop->ptr dangling
 *
 * Revision 6.21  2000/06/05 17:52:05  tatiana
 * increase size of feature arrays to Int4
 *
 * Revision 6.20  2000/02/29 16:54:35  bazhin
 * Removed memory leaks.
 *
 * Revision 5.11  1997/06/19 18:36:58  vakatov
 * [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
 *
 * Revision 5.10  1997/04/23 18:03:28  tatiana
 * a typo fixed in HTG comments
 *
 * Revision 5.9  1997/04/11  21:33:18  tatiana
 * fixed %d to %ld in PrintComment
 *
 * Revision 5.8  1997/03/28  18:44:32  tatiana
 * type fixed in htg comments
 *
 * Revision 5.7  1997/02/10  22:13:22  tatiana
 * *** empty log message ***
 *
 * Revision 5.6  1997/01/13  23:28:50  tatiana
 * added HTG comments
 *
 * Revision 5.5  1996/12/12  21:43:50  tatiana
 * a bug fixed in GB_GetSeqDescrComms
 *
 * Revision 5.4  1996/08/14  21:44:11  tatiana
 * *** empty log message ***
 *
 * Revision 5.4  1996/08/14  21:44:11  tatiana
 * *** empty log message ***
 *
 * Revision 5.3  1996/08/12  16:44:31  tatiana
 * ErrPostEx changed to ErrpostStr
 *
 * Revision 5.2  1996/08/06  20:30:46  kans
 * SeqIdFindBest called to handle local IDs and genbank IDs coexisting
 *
 * Revision 5.1  1996/05/29  21:35:56  tatiana
 * GetEMBLDate fixed
 *
 * Revision 4.26  1996/05/21  14:09:46  tatiana
 * a bug fixed in GBDescrComFeat
 *
 * Revision 4.25  1996/05/03  15:34:23  tatiana
 * targeting the entry in GenPept format in GatherDescrByChoice
 *
 * Revision 4.24  1996/04/30  16:53:13  tatiana
 * added support for multiple 'region' and 'method' in COMMENT
 *
 * Revision 4.23  1996/04/29  18:49:23  tatiana
 * print multiple comments by number
 *
 * Revision 4.22  1996/04/15  16:08:15  tatiana
 * new line in DBSOURCE
 *
 * Revision 4.21  1996/04/15  14:33:03  tatiana
 * boolet proof added to GetGenDate
 *
 * Revision 4.19  1996/04/10  17:53:29  tatiana
 * init_comment added
 *
 * Revision 4.18  1996/04/09  16:17:28  tatiana
 * hot link to swiss-prot changed
 *
 * Revision 4.17  1996/04/09  14:03:32  tatiana
 * print COMMENT in blocks
 *
 * Revision 4.15  1996/03/25  15:54:42  tatiana
 * use target in GatherDescrByChoice for GenPept and EMBLPept
 *
 * Revision 4.14  1996/03/25  15:18:50  tatiana
 * boolet-proof n StringForSeqTech call
 *
 * Revision 4.13  1996/03/13  22:08:25  tatiana
 * show GI in DBSOURCE line when GetSeqIdForGI returns NULL
 *
 * Revision 4.12  1996/02/26  03:45:24  ostell
 * fixed usage of GatherDescrListByChoice and changed function to allocate
 * first DescrStruct instead using static storage
 *
 * Revision 4.11  1996/02/22  23:28:04  tatiana
 * typos fixed in PrintDBSourceLine()
 *
 * Revision 4.9  1996/02/18  21:16:05  tatiana
 * memory leaks cleaned up
 *
 * Revision 4.8  1996/02/15  15:51:23  tatiana
 * Gather for temp loaded items added
 *
 * Revision 4.7  1996/01/29  22:33:52  tatiana
 * *** empty log message ***
 *
 * Revision 4.6  1995/12/14  19:13:33  kans
 * fixed EMBL lineage
 *
 * Revision 4.5  1995/12/13  16:31:01  tatiana
 * itemID ... added to descriptors
 *
 * Revision 4.4  1995/11/21  17:53:36  tatiana
 * *** empty log message ***
 *
 * Revision 4.3  1995/11/17  21:28:35  kans
 * asn2ff now uses gather (Tatiana)
 *
 * Revision 4.2  1995/08/22  15:37:53  tatiana
 * GetVersion name changed to GetBiotableVersion
 *
 * Revision 4.1  1995/08/01  14:50:30  tatiana
 * change SeqIdPrint to SeqIdWrite
 *
 * Revision 1.38  1995/07/17  19:33:20  kans
 * parameters combined into Asn2ffJobPtr structure
 *
*
**************************************/

#include <gather.h>
#include <asn2ffg.h>
#include <asn2ffp.h>
#include <a2ferrdf.h>
#include <utilpub.h>
#include <ffprint.h>
#include <seqmgr.h>


NLM_EXTERN Int2 GetGenDate PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp, CharPtr buffer));
NLM_EXTERN void PrintComment PROTO ((CharPtr string, Boolean identifier, Uint1 format));
NLM_EXTERN CharPtr DateToGB PROTO ((CharPtr buf, NCBI_DatePtr ndp));
NLM_EXTERN CharPtr mRNAEvidenceComment PROTO ((UserObjectPtr obj, Boolean add));
static ValNodePtr AddToUniqueList PROTO ((SeqIdPtr sid, ValNodePtr list));
static ValNodePtr GetDBSourceForNuclDB PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));

static Boolean has_comment;

/*---------- order for other id FASTA_LONG (copied from SeqIdWrite) ------- */
 
static Uint1 fasta_order[NUM_SEQID] = {
33, /* 0 = not set */
20, /* 1 = local Object-id */
15,  /* 2 = gibbsq */
16,  /* 3 = gibbmt */
30, /* 4 = giim Giimport-id */
10, /* 5 = genbank */
10, /* 6 = embl */
10, /* 7 = pir */
10, /* 8 = swissprot */
15,  /* 9 = patent */
18, /* 10 = other TextSeqId */
20, /* 11 = general Dbtag */
31,  /* 12 = gi */
10, /* 13 = ddbj */
10, /* 14 = prf */
12, /* 15 = pdb */
10,  /* 16 = tpg */
10,  /* 17 = tpe */
10   /* 18 = tpd */
};

static ComStructPtr ComStructNew(void)
{
	ComStructPtr csp;
	
	csp = (ComStructPtr) MemNew(sizeof(ComStruct));
	csp->string = NULL;
	csp->gsdb_id = FALSE;
	
	return csp;
}

static CharPtr SeqIdWriteProc (Asn2ffJobPtr ajp, SeqIdPtr sip)
{
	StdPrintOptionsPtr Spop=ajp->Spop;

	if (Spop != NULL) {
		Spop->ptr = NULL; /* reused ajp->Spop->ptr left dangling by previous call */
	}

	if (ajp->show_version) {
		if (! StdFormatPrint((Pointer)sip, 
			(AsnWriteFunc)SeqIdAsnWrite, "VersionSeqId", Spop)) {
		    	ErrPostStr (SEV_WARNING, 0, 0, "VersionSeqId_StdFormatPrint failed");
		}
	} else {
		if (! StdFormatPrint((Pointer)sip, 
			(AsnWriteFunc)SeqIdAsnWrite, "StdSeqId", Spop)) {
		    	ErrPostStr (SEV_WARNING, 0, 0, "StdSeqId_StdFormatPrint failed");
		}
	}
	if (Spop->ptr != NULL && *((CharPtr) (Spop->ptr)) != '\0') 
		return Spop->ptr;
	else
		return NULL;
}

static CharPtr BioseqIdPrintProc (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	BioseqPtr bsp=gbp->bsp;
	StdPrintOptionsPtr Spop=ajp->Spop;

	if (Spop != NULL) {
		Spop->ptr = NULL; /* reused ajp->Spop->ptr left dangling by previous call */
	}

	if (gbp == NULL) 
		return NULL;
	if ((bsp=gbp->bsp) == NULL)
		return NULL;
	if (! StdFormatPrint((Pointer)bsp, 
		(AsnWriteFunc)BioseqAsnWrite, "StdBioseqId", Spop))
		    ErrPostStr (SEV_WARNING, 0, 0, "StdBioseqId_StdFormatPrint failed");

	if (Spop->ptr != NULL && *((CharPtr) (Spop->ptr)) != '\0') 
		return Spop->ptr;
	else
		return NULL;
}


static CharPtr TableIdPrintProc (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	BioseqPtr bsp=ajp->asn2ffwep->seg;
	StdPrintOptionsPtr Spop=ajp->Spop;

	if (Spop != NULL) {
		Spop->ptr = NULL; /* reused ajp->Spop->ptr left dangling by previous call */
	}

	if (bsp == NULL)
		return NULL;

	if (! StdFormatPrint((Pointer)bsp, 
		(AsnWriteFunc)BioseqAsnWrite, "TableIdPrintProcStdBioseqId", Spop)) {
                    ErrPostStr (SEV_WARNING, 0, 0, "StdFormatPrint failed");
	}
	if (Spop->ptr != NULL && *((CharPtr) (Spop->ptr)) != '\0') 
		return Spop->ptr;
	else
		return NULL;
}

static CharPtr SPBlockPrintProc (Asn2ffJobPtr ajp, ValNodePtr vnp)
{
	StdPrintOptionsPtr Spop=ajp->Spop;

	if (Spop != NULL) {
		Spop->ptr = NULL; /* reused ajp->Spop->ptr left dangling by previous call */
	}

	if (! StdFormatPrint((Pointer)vnp->data.ptrvalue, 
		(AsnWriteFunc)SPBlockAsnWrite, "StdSPBlock", Spop)) {
                    ErrPostStr (SEV_WARNING, 0, 0, "StdSPBlockStdFormatPrint failed");
	}
	if (Spop->ptr != NULL && *((CharPtr) (Spop->ptr)) != '\0') 
		return Spop->ptr;
	else
		return StringSave ("Empty Data\n");
}

static CharPtr PIRBlockPrintProc (Asn2ffJobPtr ajp, ValNodePtr vnp)
{
	StdPrintOptionsPtr Spop=ajp->Spop;

	if (Spop != NULL) {
		Spop->ptr = NULL; /* reused ajp->Spop->ptr left dangling by previous call */
	}

	if (! StdFormatPrint((Pointer)vnp->data.ptrvalue, 
		(AsnWriteFunc)PirBlockAsnWrite, "StdPirBlock", Spop)) {
                    ErrPostStr (SEV_WARNING, 0, 0, "StdFormatPrint failed");
	}
	if (Spop->ptr != NULL && *((CharPtr) (Spop->ptr)) != '\0') 
		return Spop->ptr;
	else
		return StringSave ("Empty Data\n");
}

static CharPtr PDBBlockPrintProc (Asn2ffJobPtr ajp, ValNodePtr vnp)
{
	StdPrintOptionsPtr Spop=ajp->Spop;

	if (Spop != NULL) {
		Spop->ptr = NULL; /* reused ajp->Spop->ptr left dangling by previous call */
	}

	if (! StdFormatPrint((Pointer)vnp->data.ptrvalue, 
		(AsnWriteFunc)PdbBlockAsnWrite, "StdPDBBlock", Spop))
                    ErrPostStr (SEV_WARNING, 0, 0, "StdFormatPrint failed");

	if (Spop->ptr != NULL && *((CharPtr) (Spop->ptr)) != '\0') 
		return Spop->ptr;
	else
		return StringSave ("Empty Data\n");
}

static CharPtr PRFBlockPrintProc (Asn2ffJobPtr ajp, ValNodePtr vnp)
{
	StdPrintOptionsPtr Spop=ajp->Spop;

	if (Spop != NULL) {
		Spop->ptr = NULL; /* reused ajp->Spop->ptr left dangling by previous call */
	}

	if (! StdFormatPrint((Pointer)vnp->data.ptrvalue, 
		(AsnWriteFunc)PrfBlockAsnWrite, "StdPRFBlock", Spop))
                    ErrPostStr (SEV_WARNING, 0, 0, "StdFormatPrint failed");

	if (Spop->ptr != NULL && *((CharPtr) (Spop->ptr)) != '\0') 
		return Spop->ptr;
	else
		return StringSave ("Empty Data\n");
}


static DescrStructPtr tie_next_dsp(DescrStructPtr head, DescrStructPtr next)
/*  ties next node to the end of the chain */
{
	DescrStructPtr v;

	if (head == NULL) {
		return next;
	}
	for (v = head; v->next != NULL; v = v->next) {
		v = v;
	}
	v->next = next;
	return head;
}


/**************************************************************************
*GetSfpComment
*
*	Get the sfp->comments from sfp's that cover the entire bsp, are
*	sfp's of type comment, and are not of length zero.  The length
*	is checked for in OrganizeSeqFeat.
*
**************************************************************************/

static CharPtr GetSfpComment (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	Int4 index;
	SeqFeatPtr sfp;
	SortStructPtr p;

	if (gbp == NULL || gbp->feat == NULL) {
		return  NULL;
	}
	index = ajp->pap_index;
	p = gbp->feat->Commlist + index;
	if ((sfp = p->sfp) == NULL) {
		GatherItemWithLock(p->entityID, p->itemID, 
			p->itemtype, &sfp, find_item);
	}
	if (sfp != NULL) {
		return sfp->comment;
	}
	return NULL;
}

NLM_EXTERN void PrintCommentByNumber(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	CharPtr newstring;
	ComStructPtr s;
	Int4 i, index = ajp->pap_index;
	
	if (gbp == NULL) {
		return;
	}
	for (s = gbp->comm, i=0; s && i < index; s = s->next, i++)
	{	
	}
	newstring = CheckEndPunctuation(s->string, '.');
	www_PrintComment(newstring, FALSE, (Uint1)ajp->format);
	newstring = MemFree(newstring);
	return;
}
NLM_EXTERN void PrintFirstComment(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	CharPtr newstring;
	ComStructPtr s;
	
	if (gbp == NULL) {
		return;
	}
	if ((s = gbp->comm) != NULL) {
		if (s->next == NULL && s->gsdb_id == TRUE) {
			newstring = StringSave(s->string);
		} else {
			newstring = CheckEndPunctuation(s->string, '.');
		}
		www_PrintComment(newstring, TRUE, (Uint1)ajp->format);
		newstring = MemFree(newstring);
	}
	return;
}
/**************************************************************************
*  ties next node to the end of the chain 
*	returns head
**************************************************************************/
static ComStructPtr tie_next_comm(ComStructPtr head, ComStructPtr next)
{
	ComStructPtr v;

	if (head == NULL) {
		return next;
	}
	for (v = head; v->next != NULL; v = v->next) {
		v = v;
	}
	v->next = next;
	return head;
}

static CharPtr GetStrForBankit(UserObjectPtr uop)
{
    ObjectIdPtr		oip;
	UserFieldPtr	ufp;
	CharPtr			ptr=NULL, ptr1 = NULL;
	Int2			i=0, ptrlen = 0;
	
	if ((oip = uop->type) == NULL) return NULL;
	if (StringCmp(oip->str, "Submission") != 0) return NULL;
	for (ufp=uop->data; ufp; ufp=ufp->next) {
		oip = ufp->label;
		if (StringCmp(oip->str, "UniVecComment") == 0) {
			if (ufp->data.ptrvalue) {
				ptrlen = (StringLen("Vector Explanation: ")
							+ StringLen(ufp->data.ptrvalue) + 1);
				ptr = (CharPtr) MemNew(ptrlen);
				sprintf(ptr, "Vector Explanation: %s", 
										(CharPtr) ufp->data.ptrvalue);
			}
		}
	}
	for (ufp=uop->data; ufp; ufp=ufp->next) {
		oip = ufp->label;
		if (StringCmp(oip->str, "AdditionalComment") == 0) {
			if (ufp->data.ptrvalue) {
				ptr1 = (CharPtr) MemNew(StringLen("Bankit Comment: ")
							+ StringLen(ufp->data.ptrvalue) + 2 + ptrlen);
				if (ptr) {
					sprintf(ptr1, "%s~Bankit Comment: %s", 
									ptr, (CharPtr) ufp->data.ptrvalue);
				} else {
					sprintf(ptr1, "Bankit Comment: %s", 
										(CharPtr) ufp->data.ptrvalue);
				}
				return ptr1;
			}
		}
	}
	
	return ptr;
}

static CharPtr GetStrForUserObject(UserObjectPtr uop)
{
    ObjectIdPtr		oip;
	UserFieldPtr	ufp, tmp, u, urf = NULL;
	CharPtr			curator = NULL, ptr=NULL, st;
	Int2			i=0, acclen;
	CharPtr			p = NULL, pp;
	Int2			review = 0, len;
	
	if ((oip = uop->type) == NULL) return NULL;
	if (StringCmp(oip->str, "RefGeneTracking") != 0) return NULL;
	len = StringLen("The reference sequence was derived from ");
	for (ufp=uop->data; ufp; ufp=ufp->next) {
		oip = ufp->label;
		if (StringCmp(oip->str, "Assembly") == 0) {
			urf = ufp;
		}
		if (StringCmp(oip->str, "Status") == 0) {
			st = (CharPtr) ufp->data.ptrvalue;
			if (StringCmp(st, "Provisional") == 0) {
				review = 1;
			} else if (StringCmp(st, "Reviewed") == 0) {
				review = 2;
			} else if (StringCmp(st, "Predicted") == 0) {
				review = 3;
			} else if (StringCmp(st, "Validated") == 0) {
				review = 4;
			}
		} else if (StringCmp (oip->str, "Collaborator") == 0) {
			st = (CharPtr) ufp->data.ptrvalue;
			if (! StringHasNoText (st)) {
				curator = st;
			}
		}
	}
	if (urf && urf->choice == 11) {
		for (tmp=urf->data.ptrvalue; tmp; tmp=tmp->next, i++) ;
		if (review == 2) {
			if (curator == NULL) {
				curator = "NCBI staff";
			}
			ptr = (CharPtr) MemNew(
			StringLen("REVIEWED REFSEQ: This record has been curated by . ") + StringLen (curator) + len + 18*i + 1);
			sprintf(ptr, "REVIEWED REFSEQ: This record has been curated by %s. ", curator);
		} else if (review == 1) {
			ptr = (CharPtr) MemNew(
			StringLen("PROVISIONAL REFSEQ: This record has not yet been subject to final NCBI review. ") + len + 18*i + 1);
			sprintf(ptr, "PROVISIONAL REFSEQ: This record has not yet been subject to final NCBI review. ");
		} else if (review == 3) {
			ptr = (CharPtr) MemNew(
			StringLen("PREDICTED REFSEQ: The mRNA record is supported by experimental evidence; however, the coding sequence is predicted. ") + len + 18*i + 1);
			sprintf(ptr, "PREDICTED REFSEQ: The mRNA record is supported by experimental evidence; however, the coding sequence is predicted. ");
		} else if (review == 4) {
			ptr = (CharPtr) MemNew(
			StringLen("VALIDATED REFSEQ: This record has undergone preliminary review of the sequence, but has not yet been subject to final NCBI review. ") + len + 18*i + 1);
			sprintf(ptr, "VALIDATED REFSEQ: This record has undergone preliminary review of the sequence, but has not yet been subject to final NCBI review. ");
		} else {
			ptr = (CharPtr) MemNew( StringLen("REFSEQ: ") + len + 18*i + 1);
			sprintf(ptr, "REFSEQ: ");
		}
		if (i > 0) {
			pp = (CharPtr) MemNew(len + 1);
			sprintf(pp, "The reference sequence was derived from ");
			StringCat(ptr, pp);
			MemFree(pp);
		}
		for (tmp=urf->data.ptrvalue; tmp; tmp=tmp->next) {
			for (u = tmp->data.ptrvalue; u; u=u->next) {
				oip = u->label;
				if (StringCmp(oip->str, "accession") == 0) {
					break;
				}
			}
			if (u) {
				if ((acclen = StringLen(u->data.ptrvalue)) > 15) {
					continue;
				}
				p = (CharPtr) MemNew(acclen + 4);
				if (tmp->next) {
					sprintf(p, "%s, ", (CharPtr) u->data.ptrvalue);
				} else {
					sprintf(p, "%s.~~", (CharPtr) u->data.ptrvalue);
				}
			}
			StringCat(ptr, p);
			MemFree(p);
		}
	}
	return ptr;
}

static CharPtr GetStrForMap(DbtagPtr dbtag)
{
	Int2 total;
	Int4 id = -1;
	ObjectIdPtr oip;
	CharPtr db, ptr = NULL, str=NULL;
	
	if (dbtag == NULL) {
		return NULL;
	}
	db=dbtag->db;
	oip=dbtag->tag;
	if (oip) {
		if (oip->id)
			id = oip->id;
		if (oip->str)
			str = oip->str;
	}
	if (db && id != -1) {
		total = StringLen("Map location: (Database ; id # )");
		total += StringLen(db) + 21;
		ptr = MemNew(total);
		sprintf(ptr, "Map location: (Database %s; id # %ld)", db, (long) (id));
	} else if (str) {
		total = StringLen("Map location: ") + StringLen(str) + 1;
		ptr = MemNew(total);
		sprintf(ptr, "Map location: %s", str);
	}
	if (ptr) {
		return (StringSave(ptr));
	}
	return NULL;
}

static CharPtr GetEvidence(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	CharPtr		retval=NULL;
	UserObjectPtr	uop=NULL;
	ValNodePtr tvnp, ds_vnp, vnp;
	DescrStructPtr dsp;

	if (gbp == NULL || gbp->bsp == NULL) {
		return NULL;
	}
	tvnp = GatherDescrListByChoice(ajp, gbp, Seq_descr_user); 
	for (ds_vnp= tvnp; ds_vnp;) {
		dsp = (DescrStructPtr) ds_vnp->data.ptrvalue;
		if((vnp = dsp->vnp) != NULL){
			if(!retval){
				uop = (UserObjectPtr) vnp->data.ptrvalue;
				retval=mRNAEvidenceComment(uop, FALSE);
			}
			MemFree(vnp);
		}
		MemFree(dsp);
		vnp=ds_vnp;
		ds_vnp=ds_vnp->next;
		MemFree(vnp);
	}
	return retval;
}

static CharPtr genanreftext1 = "GENOME ANNOTATION REFSEQ:  This model reference sequence was predicted from NCBI contig";
static CharPtr genanreftext2 = "by automated computational analysis";
static CharPtr genanreftext3 = "using gene prediction method:";
static CharPtr genanreftext4 = "~Also see:~    Documentation of NCBI's Annotation Process~    Evidence Viewer - alignments supporting this model";

static CharPtr GetAnnotationComment(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	UserObjectPtr	uop=NULL;
    ObjectIdPtr		oip;
	UserFieldPtr	ufp;
	CharPtr 		retval = NULL, name = NULL, method = NULL;
	
	ValNodePtr tvnp, ds_vnp, vnp;
	DescrStructPtr dsp;

	if (gbp == NULL || gbp->bsp == NULL) {
		return NULL;
	}
	tvnp = GatherDescrListByChoice(ajp, gbp, Seq_descr_user); 
	for (ds_vnp= tvnp;
				ds_vnp; ds_vnp=ds_vnp->next) {
		dsp = (DescrStructPtr) ds_vnp->data.ptrvalue;
		if ((vnp = dsp->vnp) == NULL)
			continue;
		uop = (UserObjectPtr) vnp->data.ptrvalue;
	}
	if (uop == NULL) return retval;
	if ((oip = uop->type) == NULL) return retval;
	if (StringCmp(oip->str, "ModelEvidence") != 0) return retval;
	for (ufp=uop->data; ufp; ufp=ufp->next) {
		oip = ufp->label;
		if (StringCmp(oip->str, "Contig Name") == 0) {
			name = (CharPtr) ufp->data.ptrvalue;
		}
		if (StringCmp(oip->str, "Method") == 0) {
			if (ufp->data.ptrvalue) {
				method = StringSave((CharPtr) ufp->data.ptrvalue);
			}
		}
	}
	if (name == NULL) return NULL;
	retval = MemNew(StringLen(genanreftext1) + StringLen(genanreftext2) + StringLen(genanreftext3) + StringLen(genanreftext4) + StringLen(name) + StringLen(method) + 15);
	if (method != NULL) {
		sprintf(retval, "%s %s %s %s %s. %s", genanreftext1, name, genanreftext2, genanreftext3, method, genanreftext4);
	} else {
		sprintf(retval, "%s %s %s. %s", genanreftext1, name, genanreftext2, genanreftext4);
	}
	return retval;
	
}

static void GetCommentByChoice(Asn2ffJobPtr ajp, GBEntryPtr gbp, Uint1 choice)
{
	ValNodePtr ds_vnp, tvnp, vnp = NULL;
	DescrStructPtr dsp;
	BioseqPtr bsp = gbp->bsp;
	DeltaSeqPtr d;
	static Char buffer[1024];
	CharPtr sptr = &buffer[0], tmp, tmp3;
	ComStructPtr csp;
	MolInfoPtr mfp;
	CharPtr buf = NULL, ptr = NULL, string=NULL, new_string=NULL, str=NULL;
	Int4 buflen = 0;
	Int4 num_s = 0, num_g = 0;
	Boolean is_other;
	CharPtr p;

	if (bsp && bsp->id) {
		is_other = (bsp->id->choice == SEQID_OTHER) ? TRUE : FALSE; 
	}
	tvnp = GatherDescrListByChoice(ajp, gbp, choice); 
	for (ds_vnp = tvnp;	ds_vnp; ds_vnp=ds_vnp->next) {
		dsp = (DescrStructPtr) ds_vnp->data.ptrvalue;
		vnp = dsp->vnp;
		if (vnp == NULL) {
			continue;
		}
		if (choice == Seq_descr_user) {
			if (ajp->bankit && 
				(new_string = GetStrForBankit(vnp->data.ptrvalue))) {
				csp = ComStructNew();
				csp->string = new_string;
				csp->gsdb_id = FALSE;
				csp->entityID = dsp->entityID;
				csp->itemID = dsp->itemID;
				csp->itemtype = dsp->itemtype;
				gbp->comm = tie_next_comm(gbp->comm, csp);
				gbp->comm_num++;
			}
			if ((new_string = GetStrForUserObject(vnp->data.ptrvalue))) {
				csp = ComStructNew();
				csp->gsdb_id = FALSE;
				csp->string = StringSave(new_string);
				new_string = MemFree(new_string);
				csp->entityID = dsp->entityID;
				csp->itemID = dsp->itemID;
				csp->itemtype = dsp->itemtype;
				gbp->comm = tie_next_comm(gbp->comm, csp);
				gbp->comm_num++;
			}
		}
		csp = ComStructNew();
		csp->string = NULL;
		csp->gsdb_id = FALSE;
		switch (choice) {
		case Seq_descr_comment:
			csp->string = StringSave(vnp->data.ptrvalue);
			break;
		case Seq_descr_maploc:
			csp->string = GetStrForMap((DbtagPtr) vnp->data.ptrvalue);
			break;
		case Seq_descr_region:
			sprintf(sptr, "Region: ");
			new_string = Cat2Strings(sptr , vnp->data.ptrvalue, NULL, 0);
			str = CheckEndPunctuation(new_string, '.');
			new_string = MemFree(new_string);
			csp->string = StringSave(str);
			MemFree(str);
			break;
		case Seq_descr_molinfo:
			if ((mfp = vnp->data.ptrvalue) != NULL) {
				if (is_other == TRUE  && mfp->completeness) {
					switch (mfp->completeness) {
						case 1:
				csp->string = StringSave("COMPLETENESS: full length");
						break;
						case 2:
				csp->string = StringSave("COMPLETENESS: not full length");
						break;
						case 3:
			csp->string = StringSave("COMPLETENESS: incomplete on the 5' end");
						break;
						case 4:
			csp->string = StringSave("COMPLETENESS: incomplete on the 3' end");
						break;
						case 5:
			csp->string = StringSave("COMPLETENESS: incomplete on both ends");
						break;
						case 6:
			csp->string = StringSave("COMPLETENESS: complete on the 5' end");
						break;
						case 7:
			csp->string = StringSave("COMPLETENESS: complete on the 3' end");
						break;
						default:
			csp->string = StringSave("COMPLETENESS: unknown");
						break;
					}
				} else if(mfp->tech > 1) {
					for (d=(DeltaSeqPtr)(bsp->seq_ext); d != NULL; d=d->next) {
						buflen +=80;
					}
					if (buflen > 0) {
						buf = MemNew(buflen+1);
						CountGapsInDeltaSeq(bsp, &num_s, 
							&num_g, NULL, NULL, buf, buflen);
					}
					if (mfp->tech == MI_TECH_htgs_0) {
						if (num_s > 0) {
							sprintf(buffer, 
							"* NOTE: This record contains %ld individual~* sequencing reads that have not been assembled into~* contigs. Runs of N are used to separate the reads~* and the order in which they appear is completely~* arbitrary. Low-pass sequence sampling is useful for~* identifying clones that may be gene-rich and allows~* overlap relationships among clones to be deduced.~* However, it should not be assumed that this clone~* will be sequenced to completion. In the event that~* the record is updated, the accession number will~* be preserved.", (long) (num_s-num_g));
						}
						csp->string = Cat2Strings(buffer, buf, "~", 0);
					} else if (mfp->tech == MI_TECH_htgs_1) {
						tmp = StringSave(
						"* NOTE: This is a \"working draft\" sequence.");
						if (num_s > 0) {
							sprintf(buffer, 
							" It currently~* consists of %ld contigs. The true order of the pieces~* is not known and their order in this sequence record is~* arbitrary. Gaps between the contigs are represented as~* runs of N, but the exact sizes of the gaps are unknown.", (long) (num_s-num_g));
							p = Cat2Strings(tmp, buffer, "", 0);
							MemFree(tmp);
							tmp = p;
						}
						tmp3 = StringSave("* This record will be updated with the finished sequence~* as soon as it is available and the accession number will~* be preserved.");
						p = Cat2Strings(tmp, tmp3, "~", 0);
						MemFree(tmp);
						MemFree(tmp3);
						csp->string = Cat2Strings(p, buf, "~", 0);
						MemFree(p);
					} else if (mfp->tech == MI_TECH_htgs_2) {
						tmp = StringSave(
						"* NOTE: This is a \"working draft\" sequence."); 
						if (num_s > 0) {
							sprintf(buffer, " It currently~* consists of %ld contigs. Gaps between the contigs~* are represented as runs of N. The order of the pieces~* is believed to be correct as given, however the sizes~* of the gaps between them are based on estimates that have~* provided by the submittor.", (long) (num_s-num_g));
							p = Cat2Strings(tmp, buffer, "", 0);
							MemFree(tmp);
							tmp = p;
						}
						tmp3 = StringSave("* This sequence will be replaced~* by the finished sequence as soon as it is available and~* the accession number will be preserved.");
						p = Cat2Strings(tmp, tmp3, "~", 0);
						MemFree(tmp);
						MemFree(tmp3);
						csp->string = Cat2Strings(p, buf, "~", 0);
						MemFree(p);
					} else if ((str = StringForSeqTech(mfp->tech)) != NULL) {
						ptr = MemNew(StringLen(str) + 10);
						sprintf(ptr, "Method: %s.", str); 
						csp->string = StringSave(ptr);
						MemFree(ptr);
					}
					MemFree(buf);
				}
			}
			break;
		case Seq_descr_method:
			if (vnp->data.intvalue > 1) {
				str = StringForSeqMethod((Int2) vnp->data.intvalue);
				ptr = MemNew(StringLen(str) + 10);
				sprintf(ptr, "Method: %s.", str); 
				csp->string = StringSave(ptr);
                                MemFree(ptr);
			}
			break;
		default:
			break;
		}
		if (choice != Seq_descr_user && csp->string) {
			csp->entityID = dsp->entityID;
			csp->itemID = dsp->itemID;
			csp->itemtype = dsp->itemtype;
			gbp->comm = tie_next_comm(gbp->comm, csp);
			gbp->comm_num++;
		} else {
			MemFree(csp);
		}
		MemFree(vnp);
		MemFree(dsp);
	}
	ValNodeFree(tvnp);
	return;
}

NLM_EXTERN Int2 GB_GetSeqDescrComms(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	ComStructPtr csp;
    DbtagPtr	DBtag = NULL;
	static Char buf[1001], tmp[28];
	SeqIdPtr sid = NULL;
	SeqHistPtr hist;
	Int2 num_gi;
	DatePtr dat;
	CharPtr strd = NULL;
	TextSeqIdPtr tsip = NULL;
	Boolean is_contig = FALSE;
	
	gbp->comm_num = 0;
/*  Get  NT_ contig comment */
	for (sid = gbp->bsp->id; sid; sid=sid->next) {
		if (sid->choice == SEQID_OTHER) {
			tsip = (TextSeqIdPtr) sid->data.ptrvalue;
			break;
		}
	}
	if (tsip != NULL) {
		if (StringNCmp(tsip->accession, "NT", 2) == 0) {
			is_contig = TRUE;			
			csp = ComStructNew();
			csp->string = StringSave("GENOME ANNOTATION REFSEQ:  NCBI contigs are derived from assembled genomic sequence data. They may include both draft and finished sequence.");
		} else if (StringNCmp(tsip->accession, "XP_", 3) == 0 || StringNCmp(tsip->accession, "XM_", 3) == 0) {
			is_contig = TRUE;			
			csp = ComStructNew();
			csp->string = GetAnnotationComment(ajp, gbp);
		}
		if (is_contig) {			
			gbp->comm = tie_next_comm(gbp->comm, csp);
			gbp->comm_num++;
		}
		/*
		if ((strd = GetEvidence(ajp, gbp)) != NULL) {
			csp = ComStructNew();
			csp->string =StringSave( strd);
			gbp->comm = tie_next_comm(gbp->comm, csp);
			gbp->comm_num++;
		}
		*/
	}

	GetCommentByChoice(ajp, gbp, Seq_descr_user);

/*  Get GSDB Id */
	for (sid = gbp->bsp->id; sid; sid=sid->next) {
		if (sid->choice != SEQID_GENERAL)
			continue;
		DBtag = sid->data.ptrvalue;
		if (StringCmp(DBtag->db, "GSDB") == 0) {
			break;
		}
	}
	if (sid != NULL) {
		if (DBtag && DBtag->tag) {
			csp = ComStructNew();
			csp->gsdb_id = TRUE;
			sprintf(buf, "GSDB:S:%ld", (long) DBtag->tag->id);
			csp->string = StringSave(buf);
			gbp->comm = tie_next_comm(gbp->comm, csp);
			gbp->comm_num++;
		}
	}
	if ((hist = gbp->bsp->hist) != NULL) {
		if (hist->replaced_by_ids != NULL && hist->replaced_by_date != NULL) {
			num_gi = 0;
			for (sid=hist->replaced_by_ids; sid; sid=sid->next) {
				if (sid->choice == SEQID_GI) {
					num_gi++;
				}
			}
			if ((dat = hist->replaced_by_date) != NULL) {
				strd = PrintDate(dat);
				if (strd[StringLen(strd)-1] == '\n') {
					if (strd[StringLen(strd)-2] == '.') {
						strd[StringLen(strd)-2] = '\0';
					} else {
						strd[StringLen(strd)-1] = '\0';
					}
				}
			}
			csp = ComStructNew();
			csp->string = MemNew((128 + 16*num_gi)*sizeof(Char));
			sprintf(csp->string, "[WARNING] On %s this sequence was replaced by a newer version", strd);
			if(strd != NULL)
				strd = MemFree(strd);
			for (sid=hist->replaced_by_ids; sid; sid=sid->next) {
				if (sid->choice == SEQID_GI) {
					sprintf(tmp, " gi:%ld", (long) sid->data.intvalue);
					StringCat(csp->string, tmp);
				}
			}
			gbp->comm = tie_next_comm(gbp->comm, csp);
			gbp->comm_num++;
		}
		if (hist->replace_ids != NULL && hist->replace_date != NULL) {
			num_gi = 0;
			for (sid=hist->replace_ids; sid; sid=sid->next) {
				if (sid->choice == SEQID_GI) {
					num_gi++;
				}
			}
			if ((dat = hist->replace_date) != NULL) {
				strd = PrintDate(dat);
				if (strd[StringLen(strd)-1] == '\n') {
					if (strd[StringLen(strd)-2] == '.') {
						strd[StringLen(strd)-2] = '\0';
					} else {
						strd[StringLen(strd)-1] = '\0';
					}
				}
			}
			csp = ComStructNew();
			csp->string = MemNew((128 + 16*num_gi)*sizeof(Char));
			sprintf(csp->string, "On %s this sequence version replaced", strd);
			if(strd != NULL)
				strd = MemFree(strd);
			for (sid=hist->replace_ids; sid; sid=sid->next) {
				if (sid->choice == SEQID_GI) {
					sprintf(tmp, " gi:%ld", (long) sid->data.intvalue);
					StringCat(csp->string, tmp);
				}
			}
			gbp->comm = tie_next_comm(gbp->comm, csp);
			gbp->comm_num++;
		}
	}
	GetCommentByChoice(ajp, gbp, Seq_descr_comment);
	GetCommentByChoice(ajp, gbp, Seq_descr_maploc);
	GetCommentByChoice(ajp, gbp, Seq_descr_region);
	GetCommentByChoice(ajp, gbp, Seq_descr_molinfo);
	GetCommentByChoice(ajp, gbp, Seq_descr_method);

	return gbp->comm_num;
}

static CharPtr GetPDBHet (Asn2ffJobPtr ajp, GBEntryPtr gbp, Int2 indent)
{
	Boolean first_done=FALSE, line_return, paranthesis=FALSE;
	CharPtr het_string="Heterogen Groups:", start, temp, temp1, newstring=NULL, string;
	Int2 length;
	ValNodePtr vnp=NULL, tvnp, ds_vnp;
	DescrStructPtr dsp;

	tvnp = GatherDescrListByChoice(ajp, gbp, Seq_descr_het); 
	for (ds_vnp= tvnp;
				ds_vnp; ds_vnp=ds_vnp->next) {
		dsp = (DescrStructPtr) ds_vnp->data.ptrvalue;
		if ((vnp = dsp->vnp) == NULL)
			continue;
		gbp->descr = tie_next_dsp(gbp->descr, dsp);
		string = vnp->data.ptrvalue;
		length = StringLen(string);
		temp = start = MemNew(length*sizeof(Char));
		while (*string != '\0') {
			if (*string == '(') {
				paranthesis = TRUE;
			} else if (*string == ')') {
				paranthesis = FALSE;
			}
			if (paranthesis && *string == ' ') {
				string++;
			} else {
				*temp = *string;
				temp++; string++;
			}
		}
		*temp = '\0';

		if (first_done == FALSE) {
			/* the next two lines initialize DoSpecialLineBreak */
			line_return = DoSpecialLineBreak(NULL, indent);
			line_return = DoSpecialLineBreak(het_string, indent);
			line_return = DoSpecialLineBreak(start, indent);
			if (line_return)
				newstring = Cat2Strings(het_string, start, "~", 0);
			else
				newstring = Cat2Strings(het_string, start, " ", 0);
			first_done = TRUE;
		} else {
			line_return = DoSpecialLineBreak(start, indent);
			if (line_return)
				temp1 = Cat2Strings(newstring, start, ";~", 0);
			else
				temp1 = Cat2Strings(newstring, start, "; ", 0);
			newstring = MemFree(newstring);
			newstring = temp1;
		}
		start = MemFree(start);
		MemFree(vnp);
	}
	ValNodeFree(tvnp);
	return newstring;
}

NLM_EXTERN void GBDescrComFeat(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	CharPtr newstring, string;
	Int2 index;
	Boolean first = FALSE;
	
	if (gbp) {
		gbp->descr = NULL;
	}
	for (index=0; index < gbp->feat->sfpCommsize; index++) {
		ajp->pap_index = (Int4) index;
		if (gbp->comm == NULL && index == 0) {
			first = TRUE;
		}
		string = GetSfpComment(ajp, gbp);
		newstring = CheckEndPunctuation(string, '.');
		PrintComment(newstring, first, (Uint1)ajp->format);
		newstring = MemFree(newstring);
	}
	return;
}

NLM_EXTERN Int2 GP_GetSeqDescrComms(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	ComStructPtr csp;
	Boolean got_comment=FALSE;
	CharPtr string;

	if (ajp->number_of_cds < 2)
	{ /* Only print out comments if there's only one CDS */
	gbp->comm_num = GB_GetSeqDescrComms(ajp, gbp);
	if ((string = GetPDBHet(ajp, gbp, 12)) != NULL) {
		csp = (ComStructPtr) MemNew(sizeof(ComStruct));
		csp->string = StringSave(string);
		if (gbp && gbp->descr) {
			csp->entityID = gbp->descr->entityID;
			csp->itemID = gbp->descr->itemID;
			csp->itemtype = gbp->descr->itemtype;
		}
			gbp->comm = tie_next_comm(gbp->comm, csp);
			gbp->comm_num++;
		}
	}

	return gbp->comm_num;
}	/* GP_GetSeqDescrComms */

/*************************************************************************
*	This function is obsolete. It is replaced by CheckSPBlock() that will
*	check the string in SPBlock and PrintSPBlock() will allocate and print
*	the string (save on allocating memory)
*************************************************************************/
static CharPtr GetSPBlock (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	ValNodePtr vnp=NULL;
	CharPtr temp, temp1, newstring=NULL, string, newline= "~", 
		identifier= "NewSPblock:~";
	ValNodePtr tvnp, ds_vnp;
	DescrStructPtr dsp;

	tvnp = GatherDescrListByChoice(ajp, gbp, Seq_descr_sp); 
	for (ds_vnp= tvnp;
				ds_vnp; ds_vnp=ds_vnp->next) {
		dsp = (DescrStructPtr) ds_vnp->data.ptrvalue;
		if ((vnp = dsp->vnp) == NULL)
			continue;
		gbp->descr = tie_next_dsp(gbp->descr, dsp);
		string = SPBlockPrintProc(ajp, vnp);
		if (newstring == NULL) {
			newstring = StringSave(string);
			string = MemFree(string);
		} else {
			temp = Cat2Strings(newstring, newline, ";", 1); 
			newstring = MemFree(newstring);
			temp1 = Cat2Strings(temp, identifier, NULL, 0); 
			newstring = Cat2Strings(temp1, string, NULL, 0); 
			string = MemFree(string);
			temp = MemFree(temp);
			temp1 = MemFree(temp1);
		}
		MemFree(vnp);
	}
	ValNodeFree(tvnp);
	return newstring;
}
static Boolean CheckSPBlock (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	ValNodePtr vnp=NULL;
	CharPtr string = NULL;
	ValNodePtr tvnp, ds_vnp;
	DescrStructPtr dsp;
	Boolean retval = FALSE;

	tvnp = GatherDescrListByChoice(ajp, gbp, Seq_descr_sp); 
	for (ds_vnp= tvnp;
				ds_vnp; ds_vnp=ds_vnp->next) {
		dsp = (DescrStructPtr) ds_vnp->data.ptrvalue;
		if ((vnp = dsp->vnp) == NULL) {
			continue;
		}
		gbp->descr = tie_next_dsp(gbp->descr, dsp);
		if ((string = SPBlockPrintProc(ajp, vnp)) != NULL) {
			MemFree(string);
			retval = TRUE;
		}
		MemFree(vnp);
	}
	ValNodeFree(tvnp);
	return retval;
}
static CharPtr GetPIRBlock (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	ValNodePtr vnp=NULL;
	CharPtr temp, temp1, newstring=NULL, string, string1, newline= "~", 
		identifier= "NewPIRblock:~";
	ValNodePtr tvnp, ds_vnp;
	DescrStructPtr dsp;

	tvnp = GatherDescrListByChoice(ajp, gbp, Seq_descr_pir); 
	for (ds_vnp= tvnp;
				ds_vnp; ds_vnp=ds_vnp->next) {
		dsp = (DescrStructPtr) ds_vnp->data.ptrvalue;
		if ((vnp = dsp->vnp) == NULL)
			continue;
		gbp->descr = tie_next_dsp(gbp->descr, dsp);
		string1 = PIRBlockPrintProc(ajp, vnp);
		if (StringNCmp("host:host", string1, 9) == 0) {
		 	/* The following takes out the PIR "host". */
			string = Cat2Strings("host", string1+9, ":", 0); 
			string1 = MemFree(string1);
		} else {
			string = string1;
		}
		if (newstring == NULL) {
			newstring = StringSave(string);
			string = MemFree(string);
		} else {
			temp = Cat2Strings(newstring, newline, ";", 1); 
			newstring = MemFree(newstring);
			temp1 = Cat2Strings(temp, identifier, NULL, 0); 
			newstring = Cat2Strings(temp1, string, NULL, 0); 
			string = MemFree(string);
			temp = MemFree(temp);
			temp1 = MemFree(temp1);
		}
		MemFree(vnp);
	}
	ValNodeFree(tvnp);
	return newstring;
}

static CharPtr GetPDBBlock (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	ValNodePtr vnp=NULL;
	CharPtr temp, temp1, newstring=NULL, string, newline= "~", 
		identifier= "NewPDBblock:~";
	ValNodePtr tvnp, ds_vnp;
	DescrStructPtr dsp;

	tvnp = GatherDescrListByChoice(ajp, gbp, Seq_descr_pdb); 
	for (ds_vnp= tvnp;
				ds_vnp; ds_vnp=ds_vnp->next) {
		dsp = (DescrStructPtr) ds_vnp->data.ptrvalue;
		if ((vnp = dsp->vnp) == NULL)
			continue;
		gbp->descr = tie_next_dsp(gbp->descr, dsp); 
		string = PDBBlockPrintProc(ajp, vnp);
		if (newstring == NULL) {
			newstring = StringSave(string);
			string = MemFree(string);
		} else {
			temp = Cat2Strings(newstring, newline, ";", 1); 
			newstring = MemFree(newstring);
			temp1 = Cat2Strings(temp, identifier, NULL, 0); 
			newstring = Cat2Strings(temp1, string, NULL, 0); 
			string = MemFree(string);
			temp = MemFree(temp);
			temp1 = MemFree(temp1);
		}
		MemFree(vnp);
	}
	ValNodeFree(tvnp);
	return newstring;
}


static CharPtr GetPRFBlock (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	ValNodePtr vnp=NULL;
	CharPtr temp, temp1, newstring=NULL, string, newline= "~", 
		identifier= "NewPRFblock:~";
	ValNodePtr tvnp, ds_vnp;
	DescrStructPtr dsp;

	tvnp = GatherDescrListByChoice(ajp, gbp, Seq_descr_prf); 
	for (ds_vnp= tvnp;
				ds_vnp; ds_vnp=ds_vnp->next) {
		dsp = (DescrStructPtr) ds_vnp->data.ptrvalue;
		if ((vnp = dsp->vnp) == NULL)
			continue;
		gbp->descr = tie_next_dsp(gbp->descr, dsp); 
		string = PRFBlockPrintProc(ajp, vnp);
		if (newstring == NULL) {
			newstring = StringSave(string);
			string = MemFree(string);
		} else {
			temp = Cat2Strings(newstring, newline, ";", 1); 
			newstring = MemFree(newstring);
			temp1 = Cat2Strings(temp, identifier, NULL, 0); 
			newstring = Cat2Strings(temp1, string, NULL, 0); 
			string = MemFree(string);
			temp = MemFree(temp);
			temp1 = MemFree(temp1);
		}
		MemFree(vnp);
	}
	ValNodeFree(tvnp);
	return newstring;
}

static ChoicePID(SeqIdPtr sid) 
{
	
	DbtagPtr db;

	if (sid->choice != SEQID_GENERAL) {
		return FALSE;
	}
	db = sid->data.ptrvalue;
	if (db == NULL) {
		return FALSE;
	}
	if ((StringNCmp(db->db, "PIDe", 4) == 0) ||
			(StringNCmp(db->db, "PIDd", 4) == 0) ||
				(StringNCmp(db->db, "PID", 3) == 0)) {
		return TRUE;
	}
	return FALSE;
}

/*************************************************************************
*	Print the DBSource line using the Print Templates.
*	Only used in GenPept mode.
**************************************************************************/
NLM_EXTERN void PrintDBSourceLine(Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	Boolean first=TRUE;
	CharPtr newstring, string=NULL;
	SeqIdPtr sid;
	ValNodePtr vnp, vnp1;

	ff_StartPrint(0, 12, ASN2FF_GB_MAX, NULL);
	ff_AddString("DBSOURCE");
	TabToColumn(13);
	sid = SeqIdSelect(gbp->bsp->id, fasta_order, NUM_SEQID);
	if (sid && ((sid->choice == SEQID_PIR) ||
                (sid->choice == SEQID_SWISSPROT) ||
                (sid->choice == SEQID_PDB) ||
                (sid->choice == SEQID_PRF))) {
/* Use TableIdPrintProc as the higher level Bioseq may have more info
than the individual parts of a segmented set, at least in Swiss-prot. */
		string = TableIdPrintProc(ajp, gbp);
		if (string) {
			ff_AddStringWithTildes(string);
			string  = MemFree(string);
		} else if ((string = BioseqIdPrintProc(ajp, gbp)) != NULL) {
			www_dbsource(string, first, sid->choice);
			string 	= MemFree(string);
		}
	} else if (sid && ((sid->choice == SEQID_EMBL) ||
			(sid->choice == SEQID_GENBANK) || 
			(sid->choice == SEQID_DDBJ) || 
			(sid->choice == SEQID_GIBBSQ) || 
			(sid->choice == SEQID_GIBBMT) || 
            (sid->choice == SEQID_OTHER) ||
			(sid->choice == SEQID_TPG) || 
			(sid->choice == SEQID_TPE) || 
			(sid->choice == SEQID_TPD) || 
			(sid->choice == SEQID_GI) || 
			(sid->choice == SEQID_GIIM || (ChoicePID(sid) == TRUE))))  {
		vnp = GetDBSourceForNuclDB(ajp, gbp);
		for (vnp1=vnp; vnp1; vnp1=vnp1->next) {
			sid = (SeqIdPtr) vnp1->data.ptrvalue;
			if ((string = SeqIdWriteProc(ajp, sid)) != NULL) {
				if (first == FALSE) {
					NewContLine();
					TabToColumn(13);
				}
				www_dbsource(string, first, sid->choice);
				string = MemFree(string);
				first = FALSE;
			}
		}
		vnp = ValNodeFree(vnp);
	} else { /* Group all others as unknown */
		ff_AddString("UNKNOWN");
	}
	if (gbp) {	
		gbp->descr = NULL;
	}
/* Only one of the following four should be non-NULL! */
	if ((string=GetPRFBlock(ajp, gbp)) != NULL) {
		newstring = CheckEndPunctuation(string, '.');
		NewContLine();
		ff_AddStringWithTildes(newstring);
		MemFree(string);
		MemFree(newstring);
	} else if ((string=GetPDBBlock(ajp, gbp)) != NULL) {
		newstring = CheckEndPunctuation(string, '.');
		NewContLine();
		ff_AddStringWithTildes(newstring);
		MemFree(string);
		MemFree(newstring);
	} else if (CheckSPBlock(ajp, gbp) == TRUE) {
		NewContLine();
		PrintSPBlock(ajp, gbp); 
		MemFree(string);
	} else if ((string=GetPIRBlock(ajp, gbp)) != NULL) {
		newstring = CheckEndPunctuation(string, '.');
		NewContLine();
		ff_AddStringWithTildes(newstring);
		MemFree(string);
		MemFree(newstring);
	}

	ff_EndPrint();

}	/* PrintDBSourceLine */


/********************************************************************
*
*	Look for database id's for the DBSource line.  Look first if
*	there's a CDS; if so get the nucleotide id and check if it's
*	unique. 
*
*
*******************************************************************/

static ValNodePtr GetDBSourceForNuclDB (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	BioseqPtr bsp;
	Int2 index, total_feats;
	SeqFeatPtr sfp_in;
	SeqIdPtr sid, id = NULL;
	SeqLocPtr location, tmp;
	ValNodePtr list=NULL, vnp;
	SortStructPtr p;
 
	if (gbp == NULL || gbp->feat == NULL) {
		return NULL;
	}
	total_feats=gbp->feat->sfpListsize;
	p = gbp->feat->List;
	for (index=0; index < total_feats; index++, p++) {
		if ((sfp_in = p->sfp) == NULL) {
			GatherItemWithLock(p->entityID, p->itemID, p->itemtype, 
									&sfp_in, find_item);
		}
		if (sfp_in == NULL) {
			continue;
		}
		if (sfp_in->data.choice == SEQFEAT_CDREGION) {
			location = sfp_in->location;
			tmp = NULL;
			while ((tmp = SeqLocFindNext(location, tmp)) != NULL) {
				sid = SeqLocId(tmp);
				list = AddToUniqueList(sid, list);
			}
		}
	}

/* If list is still NULL, then there was (probably) no CDS and this is
a "gibbsq" protein (from the backbone).  Use the bsp->id of the protein
itself. */
	if (list == NULL) {
		if ((bsp = ajp->asn2ffwep->seg) != NULL)
		{
			sid = SeqIdSelect(bsp->id, fasta_order, NUM_SEQID);
			if (sid->choice == SEQID_GIBBMT) 
				ValNodeAddPointer(&list, 0, bsp->id);	
		}
		bsp = gbp->bsp;
		ValNodeAddPointer(&list, 0, bsp->id);	
	}

/* Look for a bsp associated with this id and select the best id from
bsp->id.  If no bsp, keep the id that was found above by SeqLocId. */
	for (vnp=list; vnp; vnp=vnp->next) {
		id = vnp->data.ptrvalue;
		bsp = BioseqFindCore(id);
		if (bsp) {
			sid = SeqIdSelect(bsp->id, fasta_order, NUM_SEQID);
			vnp->data.ptrvalue = sid;
		} else if (id && id->choice == SEQID_GI) {
			if ((sid = GetSeqIdForGI(id->data.intvalue)) != NULL)
				vnp->data.ptrvalue = sid;
		}
		
	}
	return list;
}

/***********************************************************************
*ValNodePtr AddToUniqueList(SeqIdPtr sid, ValNodePtr list)
*
*
*	Checks that the SeqIdPtr is not already present in the list
*	and adds it if it is not.
***********************************************************************/

static ValNodePtr AddToUniqueList(SeqIdPtr sid, ValNodePtr list)

{
	Boolean present=FALSE;
	ValNodePtr vnp;

	if (sid == NULL)	/* A sid of NULL doesn't work here */
		return list;

	for (vnp=list; vnp; vnp=vnp->next)
	{
		if (SeqIdMatch (sid, (SeqIdPtr) vnp->data.ptrvalue) == TRUE)
		{
			present = TRUE;
			break;
		}
	}

	if (present == FALSE)
		vnp = ValNodeAddPointer(&list, 0, sid);	

	if (list == NULL)
		list = vnp;

	return list;
}


/*********************************************************************
*	Int4 GetGibbsqNumber (BioseqPtr bsp)
*
*	returns the gibbsq id if a backbone record, otherwise 0.
*
*************************************************************************/

NLM_EXTERN Int4 GetGibbsqNumber (BioseqPtr bsp)
{
	Int4 gibbsq=0;
	SeqIdPtr sip;

	for (sip=bsp->id; sip; sip=sip->next)
	{
		if (sip->choice == SEQID_GIBBSQ)
			gibbsq = sip->data.intvalue;
	}

	return gibbsq;
}

NLM_EXTERN Int4 GetGibbsqCommentLength(GBEntryPtr gbp)
{
	Int4 gibbsq=0, total;
	PubdescPtr pdp;
	PubStructPtr psp;
	ValNodePtr vnp1;

	total = 0;
	for (vnp1=gbp->Pub; vnp1; vnp1=vnp1->next) {
		psp = vnp1->data.ptrvalue;
		if ((pdp=psp->descr) != NULL) {
			if (pdp->fig) {
				total += 30;
				total += StringLen(pdp->fig);
			}
			if (pdp->poly_a)
				total += 85;
			if (pdp->maploc) {
				total += 20;
				total += StringLen(pdp->maploc);
			}
		}
	}
	return total;
}

/**************************************************************************
*	CharPtr GetGibbsqComment (GBEntryPtr gbp)
*
*	This function determines whether the entry was a backbone entry
*	and generates a message if it was.  The CharPtr needs to be
*	deleted by the caller.
**************************************************************************/

NLM_EXTERN CharPtr GetGibbsqComment(GBEntryPtr gbp)

{
	CharPtr ptr, temp;
	Int4 gibbsq=0, length, total;
	PubdescPtr pdp;
	PubStructPtr psp;
	ValNodePtr vnp1;

	gibbsq = GetGibbsqNumber(gbp->bsp);

	if (gibbsq > 0) {
		total = GetGibbsqCommentLength(gbp);

		ptr = (CharPtr) MemNew((size_t) total*sizeof(Char));
		for (vnp1=gbp->Pub; vnp1; vnp1=vnp1->next) {
			psp = vnp1->data.ptrvalue;
			if ((pdp=psp->descr) != NULL) {
				if (pdp->fig) {
					temp = CheckEndPunctuation(pdp->fig, '.');
					length = StringLen(ptr);
					sprintf(ptr+length, "~This sequence comes from %s", temp);
					temp = MemFree(temp);
				} if (pdp->poly_a) {
					length = StringLen(ptr);
					sprintf(ptr+length, "~Polyadenylate residues occurring in the figure were omitted from the sequence.");
				} if (pdp->maploc) {
					temp = CheckEndPunctuation(pdp->maploc, '.');
					length = StringLen(ptr);
					sprintf(ptr+length, "~Map location: %s", temp);
					temp = MemFree(temp);
				}
			}
		}
		return ptr;	/* gibbsq found */
	}
	return NULL;	/* gibbsq was zero */

}	/* GetGibbsqComment */

/**************************************************************************
*	CharPtr GetGibbsqStatement(GBEntryPtr gbp, CharPtr ptr)
*
*	This function determines whether the entry was a backbone entry
*	and generates a message if it was.  The caller needs to 
*	include a Char array of 150 characters.
**************************************************************************/

NLM_EXTERN Int4 GetGibbsqStatement(GBEntryPtr gbp, CharPtr ptr)

{
	Int4 gibbsq=0;

	gibbsq = GetGibbsqNumber(gbp->bsp);

	if (gibbsq > 0)
	{
		sprintf(ptr, "GenBank staff at the National Library of Medicine created this entry [NCBI gibbsq %ld] from the original journal article.", (long) gibbsq);
	}
	return gibbsq;	

}	/* GetGibbsqStatement */

/***************************************************************************
*PrintComment
*
***************************************************************************/

NLM_EXTERN void PrintComment (CharPtr string, Boolean identifier, Uint1 format)

{
	if (string != NULL)
	{
		if (format == EMBL_FMT || format == PSEUDOEMBL_FMT ||
			format == EMBLPEPT_FMT) {
			if (identifier == TRUE)
				PrintXX();
			ff_StartPrint(5, 5, ASN2FF_EMBL_MAX, "CC");
		} else {
			ff_StartPrint(0, 12, ASN2FF_GB_MAX, NULL);
			if (identifier == TRUE)
				ff_AddString("COMMENT");
			TabToColumn(13);
		}

		ff_AddStringWithTildes(string);
	
		ff_EndPrint();
	}

	return;
}	/* PrintComment */

NLM_EXTERN void GetGBDate (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	Char buffer[12];
	Int2 status;

	status = GetGenDate(ajp, gbp, buffer);
	if (status != -1)
	{
		StringNCpy(gbp->date, buffer, 11);
	}
	else 
	{ /* Nothing work, use something */
		StringCpy(gbp->date, "01-JAN-1900");
		if (ASN2FF_SHOW_ERROR_MSG == TRUE && ASN2FF_DATE_ERROR_MSG == TRUE)
		{
			ErrPostStr (SEV_WARNING, ERR_DATE_IllegalDate, 
				"GetGBDate: No string or std date found");
		}
	}
	return;
}

NLM_EXTERN void GetGPDate (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	Char buffer[12];
	Int2 status;

	status = GetGenDate(ajp, gbp, buffer);
	if (status != -1)
	{
		StringNCpy(gbp->date, buffer, 11);
	} else {
		status = GetGenDate(ajp, NULL, buffer);
		if (status != -1)
		{
			StringNCpy(gbp->date, buffer, 11);
		}
	}

	if (status == -1)
	{ 
		StringCpy(gbp->date, "01-JAN-1900");
		if (ASN2FF_SHOW_ERROR_MSG == TRUE && ASN2FF_DATE_ERROR_MSG == TRUE)
		{
			ErrPostStr (SEV_WARNING, ERR_DATE_IllegalDate, 
				"GetGPDate: No string or std date found");
		}
	}

	return;
}

static Boolean get_descr_vnp (GatherContextPtr gcp)
{
/* find only one (last!) vnp with given choice */
	ValNodePtr	tmp, vnp;
	DescrStructPtr	PNTR dspp;
	DescrStructPtr	dsp;
	
	dspp = gcp->userdata;
	dsp = *dspp;
	vnp = dsp->vnp;
	switch (gcp->thistype)
	{
		case OBJ_SEQDESC:
			tmp = (ValNodePtr) (gcp->thisitem);
			if (tmp->choice == vnp->choice) {
				if (tmp->data.ptrvalue != NULL) {
					dsp->vnp = tmp;
					dsp->entityID = gcp->entityID;
					dsp->itemID = gcp->itemID;
					dsp->itemtype = gcp->thistype;
					*dspp = dsp;
				}
			} 
			break;
		default:
			break;
	}
	return TRUE;
}

static Boolean get_descr_list (GatherContextPtr gcp)
{
	ValNodePtr	tmp, vnp, vv, v, v_new;
	ValNodePtr	PNTR vnpp;
	DescrStructPtr dsp, new_dsp;
	
	vnpp = gcp->userdata;
	vnp = *vnpp;
	dsp = vnp->data.ptrvalue;
	switch (gcp->thistype)
	{
		case OBJ_SEQDESC:
			tmp = (ValNodePtr) (gcp->thisitem);
			if (tmp->choice == dsp->vnp->choice) {
				v_new = ValNodeNew(NULL);
				v_new = MemCopy(v_new, tmp, sizeof(ValNode));
				v_new->next = NULL;
				if (dsp->vnp->data.ptrvalue == NULL) {
					ValNodeFree(dsp->vnp);
					dsp->vnp = v_new;
					dsp->entityID = gcp->entityID;
					dsp->itemID = gcp->itemID;
					dsp->itemtype = gcp->thistype;
				} else {
					for (v = *vnpp; v; v=v->next) {
						dsp = v->data.ptrvalue;
						if (dsp->vnp->data.ptrvalue == v_new->data.ptrvalue) {
                                                    ValNodeFree(v_new);
                                                    return TRUE;
						}
					}
					new_dsp = (DescrStructPtr) MemNew(sizeof(DescrStruct));
					new_dsp->vnp = v_new;
					new_dsp->entityID = gcp->entityID;
					new_dsp->itemID = gcp->itemID;
					new_dsp->itemtype = gcp->thistype;
					vv = ValNodeNew(NULL);
					vv->data.ptrvalue = new_dsp;
					*vnpp = tie_next(*vnpp, vv);
				}
				return TRUE;
			} 
			break;
		default:
			break;
	}
	return TRUE;
}


NLM_EXTERN ValNodePtr GatherDescrByChoice(Asn2ffJobPtr ajp, GBEntryPtr gbp, Uint1 choice)
{
	ValNodePtr vnp0, vnp;

	GatherScope gsc;
	BioseqPtr bsp;
	SeqLocPtr slp = NULL;
	Uint2 bspID;
	DescrStructPtr dsp;

	vnp = ValNodeNew(NULL);
	vnp->choice = choice;
	vnp0 = vnp;
	dsp = MemNew(sizeof(DescrStruct));
	dsp->vnp = vnp0;
  	MemSet ((Pointer) (&gsc), 0, sizeof (GatherScope));
	MemSet ((Pointer) (gsc.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
	gsc.ignore[OBJ_SEQDESC] = FALSE;
	if (gbp && gbp->bsp != NULL) {
		bsp = gbp->bsp;
		bspID = ObjMgrGetEntityIDForPointer(bsp);
		slp = ValNodeNew(NULL);
		slp->choice = SEQLOC_WHOLE;
		slp->data.ptrvalue = (SeqIdPtr) SeqIdDup (SeqIdFindBest (bsp->id, 0));
		gsc.target = slp;
	} else {
		gsc.target = NULL;
	}
	if (ajp->sep && ajp->sep->choice == 1) {
		if (ajp->only_one || ajp->genome_view) {
			gsc.target = NULL;
		}
	}
	GatherEntity(ajp->entityID, &dsp, get_descr_vnp, &gsc); 
	if (slp) {
		SeqLocFree(slp);
	}
	vnp = dsp->vnp;
	if (vnp && vnp->data.ptrvalue) {
		ValNodeFree(vnp0);
		if (gbp) {
			gbp->descr = tie_next_dsp(gbp->descr, dsp);
		} else {
			MemFree(dsp);
		}
		return vnp;
	}
	ValNodeFree(vnp0);
	MemFree(dsp);
	return NULL;
}

NLM_EXTERN ValNodePtr GatherDescrListByChoice(Asn2ffJobPtr ajp, GBEntryPtr gbp, Uint1 choice)
{
	ValNodePtr vnp0, vnp, ds_vnp;

	GatherScope gsc;
	BioseqPtr bsp;
	SeqLocPtr slp = NULL;
	DescrStructPtr dsp;

	vnp = ValNodeNew(NULL);
	vnp->choice = choice;
	vnp0 = vnp;
	dsp = MemNew(sizeof(DescrStruct));
	dsp->vnp = vnp0;
	ds_vnp = ValNodeNew(NULL);
	ds_vnp->data.ptrvalue = dsp;
  	MemSet ((Pointer) (&gsc), 0, sizeof (GatherScope));
  	gsc.get_feats_location = TRUE;
	MemSet ((Pointer) (gsc.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
	gsc.ignore[OBJ_SEQDESC] = FALSE;
	if (gbp && gbp->bsp != NULL) {
		bsp = gbp->bsp; 
		slp = ValNodeNew(NULL);
		slp->choice = SEQLOC_WHOLE;
		slp->data.ptrvalue = (SeqIdPtr) SeqIdDup (SeqIdFindBest (bsp->id, 0));
		gsc.target = slp;
	} else {
		gsc.target = NULL;
	}
	GatherEntity(ajp->entityID, &ds_vnp, get_descr_list, &gsc); 
	if (slp) {
		SeqLocFree(slp);
	}
	dsp = (DescrStructPtr) ds_vnp->data.ptrvalue;
	vnp = dsp->vnp;
	if (vnp && vnp->data.ptrvalue) {
		return ds_vnp;
	}
	ValNodeFree(vnp0);
	ValNodeFreeData(ds_vnp);
	return NULL;
}

NLM_EXTERN Int2 GetGenDate(Asn2ffJobPtr ajp, GBEntryPtr gbp, CharPtr buffer)
{

	EMBLBlockPtr ebp=NULL;
	GBBlockPtr gblp=NULL;
	Int2 date_choice, status;
	NCBI_DatePtr date=NULL, best_update_date=NULL, best_create_date=NULL;
	SPBlockPtr spp=NULL;
	PdbBlockPtr pbp=NULL;
	PdbRepPtr prp=NULL;
	ValNodePtr vnp;

	date_choice = -1;
	if (gbp) {
		gbp->descr = NULL;
	}
	if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_update_date)) != NULL) {
		if (gbp && gbp->descr) {
			gbp->descr = MemFree(gbp->descr);
		}
		date = (NCBI_DatePtr) vnp->data.ptrvalue;
		if (date->data[0] == 1) {
			if (best_update_date) {
				status = DateMatch(date, best_update_date, FALSE);
				if (status == 1) {
					best_update_date = date; 
				}
			} else {
				best_update_date = date;
			}
		}
	}

	if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_embl)) != NULL) {
	/*	ebp possibly used again below. */
		if (gbp && gbp->descr) {
			gbp->descr = MemFree(gbp->descr);
		}
		ebp = (EMBLBlockPtr) vnp->data.ptrvalue;
		if (ebp->update_date != NULL)
		{
			if (ebp->update_date->data[0] == 1)
			{
				date = (NCBI_DatePtr) ebp->update_date;
				if (best_update_date) {
				     status = DateMatch(date, best_update_date, FALSE);
				     if (status == 1) {
					best_update_date = date; }
				} else {
					best_update_date = date; }
			}
		}
	}

	if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_sp)) != NULL) {
	 /*	spp possibly used again below. */
		if (gbp && gbp->descr) {
			gbp->descr = MemFree(gbp->descr);
		}
		spp = (SPBlockPtr) vnp->data.ptrvalue;
		if (spp->sequpd != NULL && spp->sequpd->data[0] == 1)
		{
			date = (NCBI_DatePtr) spp->sequpd;
			if (best_update_date) {
			     status = DateMatch(date, best_update_date, FALSE);
			     if (status == 1) {
				best_update_date = date; }
			} else {
				best_update_date = date; }
		}
		if (spp->annotupd != NULL && spp->annotupd->data[0] == 1)
		{
			date = (NCBI_DatePtr) spp->annotupd;
			if (best_update_date) {
			     status = DateMatch(date, best_update_date, FALSE);
			     if (status == 1) {
				best_update_date = date; }
			} else {
				best_update_date = date; }
		}
	}

	if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_pdb)) != NULL) {
	 /* pbp possibly used again below. */
		if (gbp && gbp->descr) {
			gbp->descr = MemFree(gbp->descr);
		}
		pbp = (PdbBlockPtr) vnp->data.ptrvalue;
		if ((prp = pbp->replace) != NULL && prp->date != NULL && prp->date->data[0] == 1)
		{
			date = (NCBI_DatePtr) prp->date;
			if (best_update_date) {
			     status = DateMatch(date, best_update_date, FALSE);
			     if (status == 1) {
				best_update_date = date; }
			} else {
				best_update_date = date; }
		}
	}

	if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_create_date)) != NULL) {
		date = (NCBI_DatePtr) vnp->data.ptrvalue;
		if (gbp && gbp->descr) {
			gbp->descr = MemFree(gbp->descr);
		}
		if (date->data[0] == 1) {
			if (best_create_date) {
				status = DateMatch(date, best_create_date, FALSE);
				if (status == 1) {
					best_create_date = date;
				}
			} else {
				best_create_date = date;
			}
		}
	}

	if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_genbank)) != NULL) {
		gblp = (GBBlockPtr) vnp->data.ptrvalue;
		if (gbp && gbp->descr) {
			gbp->descr = MemFree(gbp->descr);
		}
		if (gblp->entry_date != NULL ) {
			if (gblp->entry_date->data[0] == 1)
			{
				date = gblp->entry_date;
				if (best_create_date) {
		  		     status = DateMatch(date, best_create_date, FALSE);
				     if (status == 1) {
					best_create_date = date; }
				} else {
					best_create_date = date; }
			}
		}
	}

	if (ebp) /* EMBLBlockPtr found above */
	{
		if (ebp->creation_date != NULL && ebp->creation_date->data[0] == 1) {
			date = ebp->creation_date;
			if (best_create_date) {
		  	     status = DateMatch(date, best_create_date, FALSE);
			     if (status == 1) {
				best_create_date = date; }
			} else {
				best_create_date = date; }
		}
	}

	if (spp) /* SPBlockPtr found above */
	{
		if (spp->created != NULL && spp->created->data[0] == 1) {
			date = spp->created;
			if (best_create_date) {
				status = DateMatch(date, best_create_date, FALSE);
				if (status == 1) {
					best_create_date = date;
				}
			} else {
				best_create_date = date;
			}
		}
	}

	if (pbp) {
		if (pbp->deposition != NULL && pbp->deposition->data[0] == 1) {
			date = pbp->deposition;
			if (best_create_date) {
				status = DateMatch(date, best_create_date, FALSE);
				if (status == 1) {
					best_create_date = date;
				}
			} else {
				best_create_date = date;
			}
		}
	}

	if (best_update_date || best_create_date) {
		date_choice = 1;
	} else {
		date = NULL;
		if ((vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_update_date)) != NULL) {
		if (gbp && gbp->descr) {
			gbp->descr = MemFree(gbp->descr);
		}
			date = (NCBI_DatePtr) vnp->data.ptrvalue;
			if (date->data[0] == 0)
				best_update_date = date; 
		}

		if (!best_update_date && ebp)
			if (ebp->update_date != NULL && ebp->update_date->data[0] == 0)
				best_update_date = (NCBI_DatePtr) ebp->update_date;

		if (!best_update_date && spp) {
			if (spp->sequpd != NULL && spp->sequpd->data[0] == 0) {
				best_update_date = (NCBI_DatePtr) spp->sequpd;
			}
			else if (spp->annotupd != NULL && spp->annotupd->data[0] == 0) {
				best_update_date = (NCBI_DatePtr) spp->annotupd;
			}
		}

		if (!best_update_date && prp)
			if (prp->date && prp->date->data[0] == 0)
				best_update_date = (NCBI_DatePtr) prp->date;

		if (!best_update_date &&
			(vnp=GatherDescrByChoice(ajp, gbp, Seq_descr_create_date)) != NULL) {
			gbp->descr = MemFree(gbp->descr);
			date = (NCBI_DatePtr) vnp->data.ptrvalue;
			if (date->data[0] == 0) {
				best_create_date = date;
			}
		}

		if (!best_update_date && gblp)
			if (gblp->entry_date && gblp->entry_date->data[0] == 0)
				best_update_date = gblp->entry_date;

		if (!best_create_date && ebp) /* EMBLBlockPtr found above */ {
			if (ebp->creation_date && ebp->creation_date->data[0] == 0)
				best_create_date = ebp->creation_date;
		}

		if (!best_create_date && spp) /* SPBlockPtr found above */ {
			if (spp->created && spp->created->data[0] == 0)
				best_create_date = spp->created;
		}

		if (!best_create_date && pbp) /* PDB Block found above */ {
			if (pbp->deposition && pbp->deposition->data[0] == 0)
				best_create_date = pbp->deposition;
		}
		if (best_update_date || best_create_date)
			date_choice = 0;
	}

	if (date_choice == 1) {
		if (best_update_date && best_create_date) {
			status = DateMatch(best_update_date, best_create_date, FALSE);
			if (status == 0 || status == 1) {
				buffer = DateToGB(buffer, best_update_date);
			} else {
				buffer = DateToGB(buffer, best_create_date);
				if (ASN2FF_SHOW_ERROR_MSG == TRUE && 
										ASN2FF_DATE_ERROR_MSG == TRUE) {
					ErrPostStr(SEV_WARNING, ERR_DATE_Create_after_update, 
							"GetGBDate: std create date after update date");
				}
			}
		} else if (best_update_date) {
			buffer = DateToGB(buffer, best_update_date);
		} else if (best_create_date) {
			buffer = DateToGB(buffer, best_create_date);
		}
	} else if (date_choice == 0)  {
		if (best_update_date) {
			StringNCpy(buffer, best_update_date->str, 11);
			gbp->date[11]='\0';
			if (ASN2FF_SHOW_ERROR_MSG == TRUE && ASN2FF_DATE_ERROR_MSG == TRUE){
				ErrPostStr(SEV_INFO, ERR_DATE_IllegalDate, 
					"GetGBDate: Only string update date found");
			}
		}  else if (best_create_date)  {
			StringNCpy(buffer, best_create_date->str, 11);
			buffer[11]='\0';
			if (ASN2FF_SHOW_ERROR_MSG == TRUE && ASN2FF_DATE_ERROR_MSG == TRUE){
				ErrPostStr(SEV_INFO, ERR_DATE_IllegalDate, 
					"GetGBDate: Only string create date found");
			}
		}
	}
	
	return date_choice;
}	/* GetGenDate */

NLM_EXTERN void GetEMBLDate (Asn2ffJobPtr ajp, GBEntryPtr gbep)

{

	Char buf_update[12], buf_create[12];
	EMBLBlockPtr ebp=NULL;
	GBBlockPtr gbp=NULL;
	Int2 status;
	NCBI_DatePtr date=NULL, best_update_date=NULL, best_create_date=NULL;
	SPBlockPtr spp=NULL;
	PdbBlockPtr pbp=NULL;
	PdbRepPtr prp=NULL;
	ValNodePtr vnp, ds_vnp, tvnp;
	DescrStructPtr dsp;

	buf_create[0] = '\0';
	buf_update[0] = '\0';

	if (gbep) {
		gbep->descr = NULL;
	}
	if ((vnp=GatherDescrByChoice(ajp, gbep, Seq_descr_update_date)) != NULL) {
		date = (NCBI_DatePtr) vnp->data.ptrvalue;
		if (date->data[0] == 1) {
			if (best_update_date) {
		  	     status = DateMatch(date, best_update_date, FALSE);
			     if (status == 1) {
				best_update_date = date; }
			} else {
				best_update_date = date; }
		}
	}

	tvnp = GatherDescrListByChoice(ajp, gbep, Seq_descr_embl); 
	for (ds_vnp= tvnp;
				ds_vnp; ds_vnp=ds_vnp->next) {
		dsp = (DescrStructPtr) ds_vnp->data.ptrvalue;
		vnp = dsp->vnp; 
		ebp = (EMBLBlockPtr) vnp->data.ptrvalue;
		if (ebp->update_date != NULL) {
			if (ebp->update_date->data[0] == 1) {
				date = (NCBI_DatePtr) ebp->update_date;
				if (best_update_date) {
				     status = DateMatch(date, best_update_date, FALSE);
				     if (status == 1) {
						best_update_date = date; 
					}
				} else {
					best_update_date = date;
				}
			}
			gbep->descr = tie_next_dsp(gbep->descr, dsp);
			break;
		}
                ValNodeFree(vnp);
	}
/*	ValNodeFreeData(tvnp); */

	if ((vnp=GatherDescrByChoice(ajp, gbep, Seq_descr_sp)) != NULL) {
	 /*	spp possibly used again below. */
		spp = (SPBlockPtr) vnp->data.ptrvalue;
		if (spp->sequpd != NULL && spp->sequpd->data[0] == 1) {
			date = (NCBI_DatePtr) spp->sequpd;
			if (best_update_date) {
			     status = DateMatch(date, best_update_date, FALSE);
			     if (status == 1) {
				best_update_date = date; }
			} else {
				best_update_date = date; }
		}
		if (spp->annotupd != NULL && spp->annotupd->data[0] == 1) {
			date = (NCBI_DatePtr) spp->annotupd;
			if (best_update_date) {
			     status = DateMatch(date, best_update_date, FALSE);
			     if (status == 1) {
				best_update_date = date; }
			} else {
				best_update_date = date; }
		}
	}

	if ((vnp=GatherDescrByChoice(ajp, gbep, Seq_descr_pdb)) != NULL) {
	 /* pbp possibly used again below. */
		pbp = (PdbBlockPtr) vnp->data.ptrvalue;
		if ((prp = pbp->replace) != NULL && prp->date != NULL && 
											prp->date->data[0] == 1) {
			date = (NCBI_DatePtr) prp->date;
			if (best_update_date) {
			     status = DateMatch(date, best_update_date, FALSE);
			     if (status == 1) {
				best_update_date = date; }
			} else {
				best_update_date = date; }
		}
	}

	if ((vnp=GatherDescrByChoice(ajp, gbep, Seq_descr_create_date)) != NULL) {
		date = (NCBI_DatePtr) vnp->data.ptrvalue;
		if (date->data[0] == 1) {
			if (best_create_date) {
		  	     status = DateMatch(date, best_create_date, FALSE);
			     if (status == 1) {
				best_create_date = date; }
			} else {
				best_create_date = date; }
		}
	}

	tvnp = GatherDescrListByChoice(ajp, gbep, Seq_descr_genbank); 
	for (ds_vnp= tvnp;
				ds_vnp; ds_vnp=ds_vnp->next) {
		dsp = (DescrStructPtr) ds_vnp->data.ptrvalue;
		if ((vnp = dsp->vnp) == NULL)
			continue; 
		gbp = (GBBlockPtr) vnp->data.ptrvalue;
		if (gbp->entry_date != NULL ) {
			if (gbp->entry_date->data[0] == 1) {
				date = gbp->entry_date;
				if (best_create_date) {
		  		     status = DateMatch(date, best_create_date, FALSE);
				     if (status == 1) {
					best_create_date = date; }
				} else {
					best_create_date = date; }
			}
			gbep->descr = tie_next_dsp(gbep->descr, dsp);
			break;
		}
		MemFree(vnp);
	}
	ValNodeFreeData(tvnp);

	if (ebp) {/* EMBLBlockPtr found above */
		if (ebp->creation_date != NULL && ebp->creation_date->data[0] == 1) {
			date = ebp->creation_date;
			if (best_create_date) {
		  	     status = DateMatch(date, best_create_date, FALSE);
			     if (status == 1) {
				best_create_date = date; }
			} else {
				best_create_date = date; }
		}
	}

	if (spp) { /* SPBlockPtr found above */
		if (spp->created != NULL && spp->created->data[0] == 1) {
			date = spp->created;
			if (best_create_date) {
				status = DateMatch(date, best_create_date, FALSE);
				if (status == 1) {
					best_create_date = date;
				}
			} else {
				best_create_date = date;
			}
		}
	}

	if (pbp){  /* PDB Block found above */
		if (pbp->deposition != NULL && pbp->deposition->data[0] == 1) {
			date = pbp->deposition;
			if (best_create_date) {
				status = DateMatch(date, best_create_date, FALSE);
				if (status == 1) {
					best_create_date = date;
				}
			} else {
				best_create_date = date;
			}
		}
	}

	if (best_update_date)
		DateToGB(buf_update, best_update_date);
	if (best_create_date)
		DateToGB(buf_create, best_create_date);

	if (buf_create[0] == '\0') {
		if (!best_create_date && ebp) { /* EMBLBlockPtr found above */
			if (ebp->creation_date && ebp->creation_date->data[0] == 0)
				best_create_date = ebp->creation_date;
		}

		if (!best_create_date && spp) { /* SPBlockPtr found above */
			if (spp->created && spp->created->data[0] == 0)
				best_create_date = spp->created;
		}

		if (!best_create_date && pbp) { /* PDB Block found above */
			if (pbp->deposition && pbp->deposition->data[0] == 0)
				best_create_date = pbp->deposition;
		}
		if (best_create_date) {
			StringNCpy(buf_create, best_create_date->str, 11);
			buf_create[11]='\0';
			if (ASN2FF_SHOW_ERROR_MSG == TRUE && ASN2FF_DATE_ERROR_MSG == TRUE) 
			{
				ErrPostStr(SEV_INFO, ERR_DATE_IllegalDate, 
					"GetEMBLDate: Only string create date found");
			}
		} else {
			StringCpy(buf_create, "01-JAN-1900");
			if (ASN2FF_SHOW_ERROR_MSG == TRUE && ASN2FF_DATE_ERROR_MSG == TRUE)
			{
				ErrPostStr(SEV_INFO, ERR_DATE_IllegalDate, 
					"GetEMBLDate: No string or std create date found");
			}
		}
	}

	if (buf_update[0] == '\0') {
		date = NULL;
		if ((vnp=
			GatherDescrByChoice(ajp, gbep, Seq_descr_update_date)) != NULL){
			date = (NCBI_DatePtr) vnp->data.ptrvalue;
			if (date->data[0] == 0) {
				best_update_date = date;
			} 
		}

		if (!best_update_date && ebp)
			if (ebp->update_date != NULL && ebp->update_date->data[0] == 0)
				best_update_date = (NCBI_DatePtr) ebp->update_date;

		if (!best_update_date && spp) {
			if (spp->sequpd != NULL && spp->sequpd->data[0] == 0) {
				best_update_date = (NCBI_DatePtr) spp->sequpd;
			} else if (spp->annotupd != NULL && spp->annotupd->data[0] == 0) {
				best_update_date = (NCBI_DatePtr) spp->annotupd;
			}
		}

		if (!best_update_date && prp)
			if (prp->date && prp->date->data[0] == 0)
				best_update_date = (NCBI_DatePtr) prp->date;

		if (!best_update_date && (vnp=GatherDescrByChoice(ajp, gbep, 
									Seq_descr_create_date)) != NULL) {
			date = (NCBI_DatePtr) vnp->data.ptrvalue;
			if (date->data[0] == 0) {
				best_create_date = date;
			}
		}

		if (!best_update_date && gbp) {
			if (gbp->entry_date && gbp->entry_date->data[0] == 0) {
				best_update_date = gbp->entry_date;
			}
		}

		if (best_update_date) {
			StringNCpy(buf_update, best_update_date->str, 11);
			buf_update[11]='\0';
			if (ASN2FF_SHOW_ERROR_MSG == TRUE && ASN2FF_DATE_ERROR_MSG == TRUE)
			{
				ErrPostStr(SEV_INFO, ERR_DATE_IllegalDate, 
					"GetEMBLDate: Only string update date found");
			}
		}
	}


	gbep->create_date = StringSave(buf_create);
	if (buf_update[0] != '\0')
		gbep->update_date = StringSave(buf_update);
	
	return;

}	/* GetEMBLDate */

NLM_EXTERN void GetEntryVersion (GBEntryPtr gbp)
{
	BioseqPtr bsp=gbp->bsp;
	SeqIdPtr isip, sip;
	TextSeqIdPtr tsip;

	isip = bsp->id;

	for (sip=isip; sip; sip=sip->next) {
		if (sip->choice == SEQID_EMBL) {
			tsip = (TextSeqIdPtr) sip->data.ptrvalue;
			if (tsip->release)
				gbp->embl_rel = tsip->release;
			if (tsip->version)
				gbp->embl_ver = tsip->version;
		}
	}
}	/* GetVersion */

NLM_EXTERN CharPtr DateToGB (CharPtr buf, NCBI_DatePtr ndp)
{
	static CharPtr month_names[] = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
	"JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
	};

	int month = 1,
		day = 1,
		year = 1900,
		tens_place = 0;	/* in case day is less than ten.	*/
	
	if (ndp->data[0] == 0) {
		StringCpy(buf, ndp->str);
	}  else if (ndp->data[0] == 1) {  /* std date */
		year = 1900 + (int)(ndp->data[1]);
		if (ndp->data[2] != 0)
		   month = (int)(ndp->data[2]);
		if (ndp->data[3] != 0)
			day = (int)(ndp->data[3]);

		if (day < 10) {
			sprintf(buf, "%ld%ld-%s-%ld", (long) tens_place, (long) day, 
							month_names[month-1], (long) year);
		} else {
			sprintf(buf, "%ld-%s-%ld", 
				(long) day, month_names[month-1], (long) year);
		}
	} else {
		if (ASN2FF_SHOW_ERROR_MSG == TRUE && ASN2FF_DATE_ERROR_MSG == TRUE) {
			ErrPostStr(SEV_WARNING, ERR_DATE_IllegalDate, 
								"Unknown Date type in DateToGB");
		}
		*buf = '\0';
	}
	return buf;
}
