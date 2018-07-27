/*  objseq.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE                          
*               National Center for Biotechnology Information
*                                                                          
*  This software/database is a "United States Government Work" under the   
*  terms of the United States Copyright Act.  It was written as part of    
*  the author's official duties as a United States Government employee and 
*  thus cannot be copyrighted.  This software/database is freely available 
*  to the public for use. The National Library of Medicine and the U.S.    
*  Government have not placed any restriction on its use or reproduction.  
*                                                                          
*  Although all reasonable efforts have been taken to ensure the accuracy  
*  and reliability of the software and data, the NLM and the U.S.          
*  Government do not and cannot warrant the performance or results that    
*  may be obtained by using this software or data. The NLM and the U.S.    
*  Government disclaim all warranties, express or implied, including       
*  warranties of performance, merchantability or fitness for any particular
*  purpose.                                                                
*                                                                          
*  Please cite the author in any work or product based on this material.   
*
* ===========================================================================
*
* File Name:  objseq.c
*
* Author:  James Ostell
*   
* Version Creation Date: 4/1/91
*
* $Revision: 6.18 $
*
* File Description:  Object manager for module NCBI-Seq
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 05-13-93 Schuler     All public functions are now declared LIBCALL.
*
*
* $Log: objseq.c,v 
 * Revision 4.2  1995/09/21  02:26:14  ostel
 * added ProgMon call
 *
 * Revision 4.1  1995/08/29  20:23:05  ostell
 * changed SeqIdPrint to SeqIdWrite
 *
 * Revision 4.0  1995/07/26  13:48:06  ostell
 * force revision to 4.0
 *
 * Revision 3.14  1995/07/25  01:00:07  ostell
 * added checks to strip asn1 spec 4.0 objects on demand
 *
 * Revision 3.13  1995/07/22  21:59:13  ostell
 * added support for ASN.1 spec 4.0
 *
 * Revision 3.12  1995/07/11  21:10:12  kans
 * changed SEQDESC_neighbors to SEQDESC_dbxref
 *
 * Revision 3.11  1995/07/11  18:46:25  ostell
 * replaced Seqdesc neighbors with dbxref
 * added reftype to Pubdesc
 *
 * Revision 3.10  1995/05/27  01:51:28  ostell
 * changes mips to mipsptr to satisfy SGI compilers
 *
 * Revision 3.9  1995/05/26  02:28:33  ostell
 * added MolInfo content label function
 *
 * Revision 3.8  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

/** for ErrPostEx() ****/

static char *this_module = "ncbiobj";
#define THIS_MODULE this_module
static char *this_file = __FILE__;
#define THIS_FILE this_file

/**********************/

#include <objseq.h>		   /* the pub interface */
#include <asnseq.h>        /* the AsnTool header */
#include <objmgr.h>
#include <sequtil.h>
#include <objgbseq.h>
#include <objinsdseq.h>
#include <objtseq.h>

static Boolean SeqDescrAsnWriteExtra (ValNodePtr anp, AsnIoPtr aip, AsnTypePtr orig,
					Boolean anp_not_null);
static Boolean SeqAnnotSetAsnWriteExtra (SeqAnnotPtr sap, AsnIoPtr aip, AsnTypePtr set,
					 AsnTypePtr element, Boolean sap_not_null);
static Boolean SeqAnnotAsnWriteExtra (SeqAnnotPtr sap, AsnIoPtr aip, AsnTypePtr orig,
						  ValNodePtr extras);

/*****************************************************************************
*
*   Bioseq ObjMgr Routines
*
*****************************************************************************/
static CharPtr bioseqtypename = "Bioseq";

static Pointer LIBCALLBACK BioseqNewFunc (void)
{
	return (Pointer) BioseqNew();
}

static Pointer LIBCALLBACK BioseqFreeFunc (Pointer data)
{
	return (Pointer) BioseqFree ((BioseqPtr) data);
}

static Boolean LIBCALLBACK BioseqAsnWriteFunc (Pointer data, AsnIoPtr aip, AsnTypePtr atp)
{
	return BioseqAsnWrite((BioseqPtr)data, aip, atp);
}

static Pointer LIBCALLBACK BioseqAsnReadFunc (AsnIoPtr aip, AsnTypePtr atp)
{
	return (Pointer) BioseqAsnRead (aip, atp);
}

static Int2 LIBCALLBACK BioseqLabelFunc ( Pointer data, CharPtr buffer, Int2 buflen, Uint1 content)
{
	return BioseqLabel ((BioseqPtr)data, buffer, buflen, content);
}

NLM_EXTERN Int2 LIBCALL BioseqLabel ( BioseqPtr bsp, CharPtr buffer, Int2 buflen, Uint1 content)
{
	CharPtr tmp;
	Char label[40];
	Int2 diff, len;

	if ((bsp == NULL) || (buflen < 1))
		return 0;

	len = buflen;
	label[0] = '\0';

	if (content != OM_LABEL_TYPE)
	{
		SeqIdWrite(bsp->id, label, PRINTID_FASTA_LONG, 39);
		if (content == OM_LABEL_CONTENT)
			return LabelCopy(buffer, label, buflen);

		diff = LabelCopyExtra(buffer, label, buflen, NULL, ": ");
		buflen -= diff;
		buffer += diff;
	}

	label[0] = '\0';
	tmp = label;
	tmp = StringMove(tmp, AsnEnumTypeStr(SEQ_INST_repr, (Int2)(bsp->repr)));
	tmp = StringMove(tmp, ", ");
	tmp = StringMove(tmp, AsnEnumTypeStr(SEQ_INST_mol, (Int2)(bsp->mol)));
	sprintf(tmp, " len= %ld", (long)(bsp->length));
	diff = LabelCopy(buffer, label, buflen);
	buflen -= diff;
	buffer += diff;

	if (content != OM_LABEL_SUMMARY)
		return (len - buflen);
	
	return (len - buflen);     /* SUMMARY not done yet */
}

static Uint2 LIBCALLBACK BioseqSubTypeFunc (Pointer ptr)
{
	if (ptr == NULL)
		return 0;
	return (Uint2)((BioseqPtr)ptr)->repr;
}

/*****************************************************************************
*
*   SeqAnnot ObjMgr Routines
*
*****************************************************************************/
static CharPtr seqannottypename = "SeqAnnot";

static Pointer LIBCALLBACK SeqAnnotNewFunc (void)
{
	return (Pointer) SeqAnnotNew();
}

static Pointer LIBCALLBACK SeqAnnotFreeFunc (Pointer data)
{
	return (Pointer) SeqAnnotFree ((SeqAnnotPtr) data);
}

static Boolean LIBCALLBACK SeqAnnotAsnWriteFunc (Pointer data, AsnIoPtr aip, AsnTypePtr atp)
{
	return SeqAnnotAsnWrite((SeqAnnotPtr)data, aip, atp);
}

static Pointer LIBCALLBACK SeqAnnotAsnReadFunc (AsnIoPtr aip, AsnTypePtr atp)
{
	return (Pointer) SeqAnnotAsnRead (aip, atp);
}

static Int2 LIBCALLBACK SeqAnnotLabelFunc ( Pointer data, CharPtr buffer, Int2 buflen, Uint1 content)
{
	return SeqAnnotLabel ((SeqAnnotPtr)data, buffer, buflen, content);
}

NLM_EXTERN Int2 LIBCALL SeqAnnotLabel (SeqAnnotPtr sap, CharPtr buffer, Int2 buflen, Uint1 content)
{
	CharPtr name;
	static CharPtr sap_types[6] = {
		"SeqAnnot..",
		"FeatureTable",
		"Alignments",
		"Graphs" ,
		"SeqIds" ,
		"SeqLocs" };
	Int2 rsult = 0;

	if ((sap == NULL) || (buflen < 1))
		return 0;

	if (sap->type <= 5)
		name = sap_types[sap->type];
	else
		name = sap_types[0];

	switch (content)
	{
		case OM_LABEL_BOTH:
		case OM_LABEL_CONTENT:
		case OM_LABEL_SUMMARY:
		case OM_LABEL_TYPE:
		default:
			rsult = LabelCopy(buffer, name, buflen);
	}
	return rsult;
}

static Uint2 LIBCALLBACK SeqAnnotSubTypeFunc (Pointer ptr)
{
	if (ptr == NULL)
		return 0;
	return (Uint2)((SeqAnnotPtr)ptr)->type;
}

/*****************************************************************************
*
*   SeqDesc ObjMgr Routines
*
*****************************************************************************/
static CharPtr seqdesctypename = "SeqDesc";

static Pointer LIBCALLBACK SeqDescNewFunc (void)
{
	return (Pointer) SeqDescrNew(NULL);
}

static Pointer LIBCALLBACK SeqDescFreeFunc (Pointer data)
{
	return (Pointer) SeqDescFree ((SeqDescPtr) data);
}

static Boolean LIBCALLBACK SeqDescAsnWriteFunc (Pointer data, AsnIoPtr aip, AsnTypePtr atp)
{
	return SeqDescAsnWrite((SeqDescPtr)data, aip, atp);
}

static Pointer LIBCALLBACK SeqDescAsnReadFunc (AsnIoPtr aip, AsnTypePtr atp)
{
	return (Pointer) SeqDescAsnRead (aip, atp);
}


static Int2 NEAR SeqDescLabelContent (ValNodePtr vnp, CharPtr buf, Int2 buflen)
{
	OrgRefPtr orp=NULL;
	UserObjectPtr uop;
	BioSourcePtr bsrcp;
	PubdescPtr pdp;
	CharPtr label = NULL;
	Char tbuf[40];
	ValNodePtr vnp2;
	ValNode vn;
	Int2 diff, len;
	DbtagPtr dbt;
	CharPtr prefix = NULL;
	MolInfoPtr mip;
	CharPtr mipsptr[4];
	Int2 i;
	ObjectIdPtr oip;

	if (vnp == NULL) return 0;

	switch (vnp->choice)
	{
		case Seq_descr_comment:
		case Seq_descr_region:
		case Seq_descr_het:
		case Seq_descr_title:
		case Seq_descr_name:
			label = (CharPtr)(vnp->data.ptrvalue);
			break;

		case Seq_descr_create_date:
		case Seq_descr_update_date:
			if (vnp->data.ptrvalue != NULL && DatePrint((DatePtr)(vnp->data.ptrvalue), tbuf))
				label = tbuf;
			break;

		case Seq_descr_org:
			orp = (OrgRefPtr)(vnp->data.ptrvalue);
orgref:		if (orp != NULL) {
				if (orp->taxname != NULL)
					label = (orp->taxname);
				else if (orp->common != NULL)
					label = (orp->common);
			}
			break;

		case Seq_descr_pub:
			pdp = (PubdescPtr)(vnp->data.ptrvalue);
			if (pdp == NULL) return 0;
			vn.choice = PUB_Equiv;
			vn.data.ptrvalue = pdp->pub;
			vn.next = NULL;
			return PubLabel(&vn, buf, buflen, OM_LABEL_CONTENT);

		case Seq_descr_user:
			uop = (UserObjectPtr)(vnp->data.ptrvalue);
			if (uop == NULL) return 0;
			label = (uop->_class);
			if (label == NULL) {
				oip = uop->type;
				if (oip != NULL) {
					label = oip->str;
				}
			}
			break;

		case Seq_descr_method:
			label =  AsnEnumTypeStr(SEQDESC_method,(Int2)(vnp->data.intvalue));
			break;

		case Seq_descr_mol_type:
			label =  AsnEnumTypeStr(SEQDESC_mol_type,(Int2)(vnp->data.intvalue));
			break;

		case Seq_descr_modif:
			len = buflen;
			vnp2 = (ValNodePtr)(vnp->data.ptrvalue);
			diff = 0;
			while (vnp2 != NULL)
			{
				if (diff)
				{
					prefix = ",";
				}
				label = AsnEnumTypeStr(SEQDESC_modif_E, (Int2)(vnp2->data.intvalue));
				diff = LabelCopyExtra(buf, label, buflen, prefix, NULL);
				buflen -= diff;
				buf += diff;
				vnp2 = vnp2->next;
			}
			return (len - buflen);

		case Seq_descr_source:
			bsrcp = (BioSourcePtr)(vnp->data.ptrvalue);
			if (bsrcp == NULL) return 0;
			orp = bsrcp->org;
			goto orgref;
			
		case Seq_descr_maploc:
			dbt = (DbtagPtr)(vnp->data.ptrvalue);
			if (dbt == NULL) return 0;
			len = buflen;
			if (dbt->db != NULL)
			{
				diff = LabelCopyExtra(buf, dbt->db, buflen, NULL, ": ");
				buflen -= diff;
				buf += diff;
			}
			if (dbt->tag != NULL)
			{
				diff = LabelCopy(buf, dbt->tag->str, buflen);
				buflen -= diff;
			}
			return len - buflen;

		case Seq_descr_molinfo:
			mip = (MolInfoPtr)(vnp->data.ptrvalue);
			if (mip == NULL) return 0;
			if (mip->biomol)
				mipsptr[0] = AsnEnumTypeStr(MOLINFO_biomol, (Int2)(mip->biomol));
			else
				mipsptr[0] = NULL;
			if (mip->tech)
				mipsptr[1] = AsnEnumTypeStr(MOLINFO_tech, (Int2)(mip->tech));
			else
				mipsptr[1] = NULL;
			mipsptr[2] = mip->techexp;
			if (mip->completeness)
				mipsptr[3] = AsnEnumTypeStr(MOLINFO_completeness, (Int2)(mip->completeness));
			else
				mipsptr[3] = NULL;
			prefix = NULL;
			len = buflen;
			for (i = 0; i < 4; i++)
			{
			    if (mipsptr[i] != NULL)
				{
					diff = LabelCopyExtra(buf, mipsptr[i], buflen, prefix, NULL);
					buflen -= diff;
					buf += diff;
					prefix = ", ";
				}
			}
			return len - buflen;


		case Seq_descr_dbxref:
			if (vnp->data.ptrvalue == NULL) return 0;
			len = buflen;
			diff = DbtagLabel((DbtagPtr)(vnp->data.ptrvalue), buf, buflen);
			buflen -= diff;
			buf += diff;
			return len - buflen;

		default:
			break;
	}

	return LabelCopy (buf, label, buflen);
}

static Int2 LIBCALLBACK SeqDescLabelFunc ( Pointer data, CharPtr buffer, Int2 buflen, Boolean content)
{
	return SeqDescLabel ((ValNodePtr)data, buffer, buflen, content);
}

NLM_EXTERN Int2 LIBCALL SeqDescLabel (ValNodePtr vnp, CharPtr buffer, Int2 buflen, Boolean content)
{
	CharPtr name;
	static CharPtr descrs [25] = {
		"SeqDesc..",
		"MolType",
		"Modifier",
		"Method",
		"Name",
		"Title",
		"Organism",
		"Comment",
		"Numbering",
		"MapLocation",
		"PIR",
		"Genbank",
		"Pub",
		"Region",
		"UserObj",
		"SWISSPROT",
		"DbXref",
		"EMBL",
		"CreateDate",
		"UpdateDate",
		"PRF",
		"PDB",
		"Heterogen",
		"BioSrc",
		"MolInfo"
		};
	Int2 len, diff, rsult = 0;

	if ((vnp == NULL) || (buflen < 1))
		return 0;

	if (vnp->choice <= 24)
		name = descrs[vnp->choice];
	else
		name = descrs[0];

	len = buflen;
	switch (content)
	{
		case OM_LABEL_BOTH:
			diff = LabelCopyExtra(buffer, name, buflen, NULL, ": ");
			buffer += diff;
			buflen -= diff;
		case OM_LABEL_CONTENT:
		case OM_LABEL_SUMMARY:
			diff = SeqDescLabelContent(vnp, buffer, buflen);
			buflen -= diff;
			return (len - buflen);
		case OM_LABEL_TYPE:
		default:
			rsult = LabelCopy(buffer, name, buflen);
	}
	return rsult;
}

static Uint2 LIBCALLBACK SeqDescSubTypeFunc (Pointer ptr)
{
	if (ptr == NULL)
		return 0;
	return (Uint2)((ValNodePtr)ptr)->choice;
}

static Boolean loaded = FALSE;

/*****************************************************************************
*
*   SeqAsnLoad()
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqAsnLoad (void)
{
    if (loaded)
        return TRUE;
    loaded = TRUE;

    if (! GeneralAsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }
    if (! SeqLocAsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }
    if (! SeqAlignAsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }
    if (! SeqFeatAsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }
    if (! SeqResAsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }
    if (! PubAsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }
	if (! SeqCodeAsnLoad())
	{
		loaded = FALSE;
		return FALSE;
	}
	if (! SeqBlockAsnLoad())
	{
		loaded = FALSE;
		return FALSE;
	}
	if (! objgbseqAsnLoad())
	{
		loaded = FALSE;
		return FALSE;
	}
	if (! objinsdseqAsnLoad()) {
		loaded = FALSE;
		return FALSE;
	}
	if (! objtseqAsnLoad())
	{
		loaded = FALSE;
		return FALSE;
	}
    if (! AsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }

	ObjMgrTypeLoad(OBJ_BIOSEQ, "Bioseq", bioseqtypename, "Biological Sequence",
		BIOSEQ, BioseqNewFunc, BioseqAsnReadFunc, BioseqAsnWriteFunc,
		BioseqFreeFunc, BioseqLabelFunc, BioseqSubTypeFunc);

	ObjMgrTypeLoad(OBJ_SEQANNOT, "Seq-annot", seqannottypename, "Sequence Annotation",
		SEQ_ANNOT, SeqAnnotNewFunc, SeqAnnotAsnReadFunc, SeqAnnotAsnWriteFunc,
		SeqAnnotFreeFunc, SeqAnnotLabelFunc, SeqAnnotSubTypeFunc);

	ObjMgrTypeLoad(OBJ_SEQDESC, "Seqdesc", seqdesctypename, "Sequence Descriptor",
		SEQDESC, SeqDescNewFunc, SeqDescAsnReadFunc, SeqDescAsnWriteFunc,
		SeqDescFreeFunc, SeqDescLabelFunc, SeqDescSubTypeFunc);

    return TRUE;
}


/*****************************************************************************
*
*   Bioseq Routines
*
*****************************************************************************/

/*****************************************************************************
*
*   BioseqNew()
*
*****************************************************************************/
NLM_EXTERN BioseqPtr LIBCALL BioseqNew (void)
{
    BioseqPtr bsp;

    bsp = (BioseqPtr)MemNew(sizeof(Bioseq));
	if (bsp == NULL) return bsp;

    bsp->length = -1;    /* not set */
	bsp->topology = 1;   /* DEFAULT = linear */

	SeqMgrAdd (SM_BIOSEQ, (Pointer)bsp);   /* add to Bioseq list */

    return bsp;
}

/*****************************************************************************
*
*   BioseqFree(bsp)
*       Frees one Bioseq and associated data
*
*****************************************************************************/
NLM_EXTERN BioseqPtr LIBCALL BioseqFree (BioseqPtr bsp)
{
	Boolean top = FALSE;

    if (bsp == NULL)
        return bsp;

	if (bsp->idx.parentptr == NULL || bsp->idx.parenttype == OBJ_SEQSUB) {
		if (bsp->seqentry != NULL) {
			SeqMgrDeleteIndexesInRecord (bsp->seqentry);
			top = TRUE;
		}
	}

	if (! SeqMgrDelete(SM_BIOSEQ, (Pointer)bsp))
	    ErrPostEx(SEV_ERROR, 0,0, "BioseqFree: pointer not registered");

    SeqIdSetFree(bsp->id);
	BioseqFreeComponents(bsp);

	if (top) {
		ObjMgrDeleteAllInRecord ();
	}

	return (BioseqPtr)MemFree(bsp);
}

/*****************************************************************************
*
*   BioseqFreeComponents(bsp)
*       Frees data associated with the Bioseq.
*   	Does not free SeqIds
*   	Does not free Bioseq itself
*   	Used by SeqMgr for caching out
*   	Called by BioseqFree()
*
*****************************************************************************/
NLM_EXTERN BioseqPtr LIBCALL BioseqFreeComponents (BioseqPtr bsp)
{
    SeqAnnotPtr sp, spnext;
    SeqFeatPtr sfp, sfpnext;
    DeltaSeqPtr dsp, dspnext;

    if (bsp == NULL)
        return bsp;

    bsp->descr = SeqDescrFree(bsp->descr);
    bsp->seq_data = BSFree(bsp->seq_data);
    bsp->fuzz = IntFuzzFree(bsp->fuzz);
    switch (bsp->seq_ext_type)
    {
        case 1:    /* seg-ext */
            SeqLocSetFree((ValNodePtr)bsp->seq_ext);
            break;
        case 2:   /* reference */
            SeqLocFree((ValNodePtr)bsp->seq_ext);
            break;
        case 3:   /* map */
            sfp = (SeqFeatPtr)bsp->seq_ext;
            while (sfp != NULL)
            {
                sfpnext = sfp->next;
                SeqFeatFree(sfp);
                sfp = sfpnext;
            }
            break;
        case 4:  /* delta */
            dsp = (DeltaSeqPtr)(bsp->seq_ext);
            while (dsp != NULL)
            {
                dspnext = dsp->next;
                DeltaSeqFree(dsp);
                dsp = dspnext;
	    }
            break;

    }
	bsp->seq_ext = NULL;
    sp = bsp->annot;
    while (sp != NULL)
    {
        spnext = sp->next;
        SeqAnnotFree(sp);
        sp = spnext;
    }
	bsp->annot = NULL;
	bsp->hist = SeqHistFree(bsp->hist);

	return bsp;
}

/*****************************************************************************
*
*   BioseqAsnWrite(bsp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (Bioseq ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL BioseqAsnWrite (BioseqPtr bsp, AsnIoPtr aip, AsnTypePtr orig)
{
	AsnTypePtr atp;
    ValNodePtr anp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	if (! ProgMon("Write Bioseq"))
		return FALSE;

	atp = AsnLinkType(orig, BIOSEQ);   /* link local tree */
    if (atp == NULL) goto erret;

	if (bsp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

	if (! AsnOpenStruct(aip, atp, (Pointer)bsp)) goto erret;
    
    if (! AsnOpenStruct(aip, BIOSEQ_id, (Pointer)bsp->id)) goto erret;    /* ids required */
    anp = bsp->id;
    while (anp != NULL)
    {
        if (! SeqIdAsnWrite(anp, aip, BIOSEQ_id_E)) goto erret;
        anp = anp->next;
    }
    if (! AsnCloseStruct(aip, BIOSEQ_id, (Pointer)bsp->id)) goto erret;

    if (bsp->descr != NULL)              /* Seq-descr optional */
    {
        if (! SeqDescrAsnWrite(bsp->descr, aip, BIOSEQ_descr)) goto erret;
    }
    else
    {
        if (! SeqDescrExtraCheck(aip, BIOSEQ_descr)) goto erret;
    }

	if (! BioseqInstAsnWrite(bsp, aip, BIOSEQ_inst)) goto erret;

    if (bsp->annot != NULL)              /* annotation optional */
    {
        if (! SeqAnnotSetAsnWrite(bsp->annot, aip, BIOSEQ_annot, BIOSEQ_annot_E))
            goto erret;
    }
    else   /* look for extra features from SeqEntryAsnOut() */
    {
        if (! SeqAnnotSetExtraCheck(aip, BIOSEQ_annot, BIOSEQ_annot_E)) goto erret;
    }

    if (! AsnCloseStruct(aip, atp, (Pointer)bsp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   BioseqAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN BioseqPtr LIBCALL BioseqAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    BioseqPtr bsp=NULL;
	AsnOptionPtr aop;
	Op_objseqPtr osp;
	Boolean check_seqid = FALSE;
	Int2 level;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return bsp;
	}

	if (aip == NULL)
		return bsp;

	if (! ProgMon("Read Bioseq"))
		return bsp;

	if ((aop = AsnIoOptionGet(aip, OP_NCBIOBJSEQ, BIOSEQ_CHECK_ID, NULL)) != NULL)
	{
		osp = (Op_objseqPtr)aop->data.ptrvalue;
		if ((osp->found_it) && (osp->load_by_id))   /* already got it */
		{
			AsnSkipValue(aip, orig);
			return bsp;
		}
		check_seqid = TRUE;
	}

	if (orig == NULL)           /* Bioseq ::= (self contained) */
		atp = AsnReadId(aip, amp, BIOSEQ);
	else
		atp = AsnLinkType(orig, BIOSEQ);    /* link in local tree */
    if (atp == NULL) return bsp;

	bsp = BioseqNew();
    if (bsp == NULL) goto erret;

	level = AsnGetLevel(aip);     /* for skipping */

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the start struct */

	atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;  /* id required, start struct */
    bsp->id = SeqIdSetAsnRead(aip, atp, BIOSEQ_id_E);
    if (bsp->id == NULL) goto erret;
	if (check_seqid)
	{
		if (SeqIdIn(osp->sip, bsp->id))
			osp->found_it = TRUE;
		else if (osp->load_by_id)   /* don't load it */
		{
			while (AsnGetLevel(aip) > level)  /* skip everything else */
			{
				atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
				if (AsnReadVal(aip, atp, NULL) <= 0) goto erret;
			}
            goto erret;      /* didn't find it -- not really an error */
		}
	}

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (atp == BIOSEQ_descr)           /* descr optional */
    {
        bsp->descr = SeqDescrAsnRead(aip, atp);
        if (bsp->descr == NULL) goto erret;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }

	if (! BioseqInstAsnRead(bsp, aip, atp)) goto erret;

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;

    if (atp == BIOSEQ_annot)
	{
        bsp->annot = SeqAnnotSetAsnRead(aip, atp, BIOSEQ_annot_E);
        if (bsp->annot == NULL) goto erret;
		atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
	}

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end Bioseq */
ret:
    AsnUnlinkType(orig);       /* unlink local tree */
	return bsp;
erret:
    bsp = BioseqFree(bsp);
    goto ret;
}

/*****************************************************************************
*
*   BioseqInstAsnWrite(bsp, aip, orig)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL BioseqInstAsnWrite (BioseqPtr bsp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
	Boolean retval = FALSE;
	Uint1 newcode;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	if (! ProgMon("Write SeqInst"))
		return FALSE;

	atp = AsnLinkType(orig, SEQ_INST);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (bsp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)bsp)) goto erret; /* instance required */
    av.intvalue = bsp->repr;
    if (! AsnWrite(aip, SEQ_INST_repr, &av)) goto erret;
    av.intvalue = bsp->mol;
    if (! AsnWrite(aip, SEQ_INST_mol, &av)) goto erret;
    if (bsp->length >= 0)
    {
        av.intvalue = bsp->length;
        if (! AsnWrite(aip, SEQ_INST_length, &av)) goto erret;
    }
    if (bsp->fuzz != NULL)
    {
        if (! IntFuzzAsnWrite(bsp->fuzz, aip, SEQ_INST_fuzz)) goto erret;
    }
    if (bsp->topology != 1)
    {
        av.intvalue = bsp->topology;
        if (! AsnWrite(aip, SEQ_INST_topology, &av)) goto erret;
    }
    if (bsp->strand)
    {
        av.intvalue = bsp->strand;
        if (! AsnWrite(aip, SEQ_INST_strand, &av)) goto erret;
    }

	              /** for XML, make it text ****/
    if (aip->type & ASNIO_XML)
    {
	if (ISA_aa(bsp->mol))
		newcode = Seq_code_ncbieaa;
	else
		newcode = Seq_code_iupacna;
	BioseqConvert(bsp, newcode);
    }

    if (bsp->seq_data != NULL)
    {
		if (! SeqDataAsnWrite(bsp->seq_data, bsp->seq_data_type, aip, SEQ_INST_seq_data))
			goto erret;
    }
    if (bsp->seq_ext != NULL)
    {
		av.ptrvalue = (Pointer)bsp->seq_ext;
        if (! AsnWriteChoice(aip, SEQ_INST_ext, (Int2)bsp->seq_ext_type, &av)) goto erret;
        switch (bsp->seq_ext_type)
        {
            case 1:		/* seg */
                if (! SeqLocSetAsnWrite((ValNodePtr)bsp->seq_ext, aip, SEQ_EXT_seg, SEG_EXT_E))
                    goto erret;
                break;
            case 2:	   /* ref */
                if (! SeqLocAsnWrite((ValNodePtr)bsp->seq_ext, aip, SEQ_EXT_ref)) goto erret;
                break;
            case 3:              /* map ext */
                if (! SeqFeatSetAsnWrite((SeqFeatPtr)bsp->seq_ext, aip, SEQ_EXT_map, MAP_EXT_E)) goto erret;
                break;
            case 4:		/* delta */
                if (! DeltaSeqSetAsnWrite((DeltaSeqPtr)bsp->seq_ext, aip, SEQ_EXT_delta, DELTA_EXT_E))
                    goto erret;
                break;
        }
    }

    if (bsp->hist != NULL)              /* history optional */
    {
        if (! SeqHistAsnWrite(bsp->hist, aip, SEQ_INST_hist))
            goto erret;
    }

    if (! AsnCloseStruct(aip, atp, (Pointer)bsp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   BioseqInstAsnRead(bsp, aip, atp)   
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL BioseqInstAsnRead (BioseqPtr bsp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Uint1 choice;
	Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if ((aip == NULL) || (bsp == NULL))
		return FALSE;

	if (! ProgMon("Read SeqInst"))
		return FALSE;

	if (orig == NULL)           /* Seq-inst ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_INST);
	else
		atp = AsnLinkType(orig, SEQ_INST);    /* link in local tree */
    if (atp == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;         /* inst required */

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    bsp->repr = (Uint1) av.intvalue;

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    bsp->mol = (Uint1) av.intvalue;

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;

    if (atp == SEQ_INST_length)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        bsp->length = av.intvalue;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }

    if (atp == SEQ_INST_fuzz)
    {
        bsp->fuzz = IntFuzzAsnRead(aip, atp);
        if (bsp->fuzz == NULL) goto erret;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }

    if (atp == SEQ_INST_topology)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        bsp->topology = (Uint1) av.intvalue;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }

    if (atp == SEQ_INST_strand)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        bsp->strand = (Uint1) av.intvalue;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }

    if (atp == SEQ_INST_seq_data)
    {
		bsp->seq_data = SeqDataAsnRead(aip, atp, &bsp->seq_data_type, bsp->length);
		if (bsp->seq_data == NULL) goto erret;

        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }

    if (atp == SEQ_INST_ext)            /* extensions */
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* CHOICE */
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* what choice? */
        choice = 0;
        if (atp == SEQ_EXT_seg)
            choice = 1;
        else if (atp == SEQ_EXT_ref)
            choice = 2;
        else if (atp == SEQ_EXT_map)
            choice = 3;
		else if (atp == SEQ_EXT_delta)
			choice = 4;
        bsp->seq_ext_type = choice;
        switch (choice)
        {
            case 1:
                bsp->seq_ext = (Pointer) SeqLocSetAsnRead(aip, atp, SEG_EXT_E);
                break;
            case 2:
                bsp->seq_ext = (Pointer) SeqLocAsnRead(aip, atp);
                break;
            case 3:
                bsp->seq_ext = (Pointer) SeqFeatSetAsnRead(aip, atp, MAP_EXT_E);
                break;
            case 4:
                bsp->seq_ext = (Pointer) DeltaSeqSetAsnRead(aip, atp, DELTA_EXT_E);
                break;
        }
        if (bsp->seq_ext == NULL) goto erret;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }

    if (atp == SEQ_INST_hist)
	{
        bsp->hist = SeqHistAsnRead(aip, atp);
        if (bsp->hist == NULL) goto erret;
		atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
	}

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end struct inst */
    retval = TRUE;
erret:
    AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqDataAsnWrite(bsp, seqtype, aip, orig)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqDataAsnWrite (ByteStorePtr bsp, Uint1 seqtype, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, tmp;
	Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, SEQ_DATA);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (bsp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    av.ptrvalue = bsp;
    if (! AsnWriteChoice(aip, atp, (Int2)seqtype, &av)) goto erret;   /* CHOICE */
    tmp = NULL;
    switch (seqtype)
    {
        case Seq_code_iupacna:
            tmp = SEQ_DATA_iupacna;
            break;
        case Seq_code_iupacaa:
            tmp = SEQ_DATA_iupacaa;
            break;
        case Seq_code_ncbi2na:
            tmp = SEQ_DATA_ncbi2na;
            break;
        case Seq_code_ncbi4na:
            tmp = SEQ_DATA_ncbi4na;
            break;
        case Seq_code_ncbi8na:
            tmp = SEQ_DATA_ncbi8na;
            break;
        case Seq_code_ncbipna:
            tmp = SEQ_DATA_ncbipna;
            break;
        case Seq_code_ncbi8aa:
            tmp = SEQ_DATA_ncbi8aa;
            break;
        case Seq_code_ncbieaa:
            tmp = SEQ_DATA_ncbieaa;
            break;
        case Seq_code_ncbipaa:
            tmp = SEQ_DATA_ncbipaa;
            break;
        case Seq_code_ncbistdaa:
            tmp = SEQ_DATA_ncbistdaa;
            break;
    }
    if (! AsnWrite(aip, tmp, &av)) goto erret;

    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqDataAsnRead(aip, atp, typeptr, length)   
*
*****************************************************************************/
NLM_EXTERN ByteStorePtr LIBCALL SeqDataAsnRead (AsnIoPtr aip, AsnTypePtr orig, Uint1Ptr typeptr, Int4 length)
{
	DataVal av;
	AsnTypePtr atp;
    Uint1 choice;
	ByteStorePtr retval = NULL, tmpbs;
	Int2 residue;
	Int4 ctr1, ctr2;
	Char buf[100];
	Boolean is_one_let = FALSE;  /* assume not a 1 letter upper case code */

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if ((aip == NULL) || (typeptr == NULL))
		return FALSE;

	if (orig == NULL)           /* Seq-inst ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_DATA);
	else
		atp = AsnLinkType(orig, SEQ_DATA);    /* link in local tree */
    if (atp == NULL) goto erret;

        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* CHOICE */
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* what kind of choice? */
        choice = 0;
        if (atp == SEQ_DATA_iupacna)
		{
			is_one_let = TRUE;
            choice = Seq_code_iupacna;
		}
        else if (atp == SEQ_DATA_iupacaa)
		{
			is_one_let = TRUE;
            choice = Seq_code_iupacaa;
		}
        else if (atp == SEQ_DATA_ncbi2na)
            choice = Seq_code_ncbi2na;
        else if (atp == SEQ_DATA_ncbi4na)
            choice = Seq_code_ncbi4na;
        else if (atp == SEQ_DATA_ncbi8na)
            choice = Seq_code_ncbi8na;
        else if (atp == SEQ_DATA_ncbipna)
            choice = Seq_code_ncbipna;
        else if (atp == SEQ_DATA_ncbi8aa)
            choice = Seq_code_ncbi8aa;
        else if (atp == SEQ_DATA_ncbieaa)
		{
			is_one_let = TRUE;
            choice = Seq_code_ncbieaa;
		}
        else if (atp == SEQ_DATA_ncbipaa)
            choice = Seq_code_ncbipaa;
        else if (atp == SEQ_DATA_ncbistdaa)
            choice = Seq_code_ncbistdaa;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        retval = (ByteStorePtr)av.ptrvalue;
		*typeptr = choice;
		ctr1 = BSLen(retval);
		if ((is_one_let) && (ctr1 > length) &&
			(aip->type == ASNIO_TEXT_IN))  /* could have spaces */
		{
			tmpbs = retval;
			retval = BSNew(length);
			BSSeek(tmpbs, 0, SEEK_SET);
			ctr2 = 0;
			while (ctr1)
			{
				residue = BSGetByte(tmpbs);
				ctr1--;
				if (IS_UPPER(residue))
				{
					buf[ctr2] = (Char)residue;
					ctr2++;
					if (ctr2 == 100)
					{
						BSWrite(retval, buf, ctr2);
						ctr2 = 0;
					}
				}
			}
			if (ctr2)
				BSWrite(retval, buf, ctr2);
			BSFree(tmpbs);
		}

ret:
    AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
erret:
	retval = BSFree(retval);
	goto ret;
}

/*****************************************************************************
*
*   SeqDescrFree(anp)
*       Frees a chain of SeqDescr and associated data
*
*****************************************************************************/
NLM_EXTERN SeqDescrPtr LIBCALL SeqDescrFree (SeqDescrPtr anp)
{
    SeqDescrPtr next;

    while (anp != NULL)
    {
        next = anp->next;
		SeqDescFree(anp);
        anp = next;
    }
	return anp;
}

/*****************************************************************************
*
*   SeqDescrFree(anp)
*       Frees a chain of SeqDescr and associated data
*
*****************************************************************************/
NLM_EXTERN SeqDescPtr LIBCALL SeqDescFree (SeqDescPtr anp)
{
    Pointer pnt;

	if (anp == NULL) return anp;

	ObjMgrDelete(OBJ_SEQDESC, (Pointer)anp);

    pnt = anp->data.ptrvalue;
    switch (anp->choice)
    {
        case 1:     /* mol-type */
        case 3:      /* method */
            break;
        case 2:     /* modif */
            ValNodeFree((ValNodePtr)pnt);
            break;
        case 4:      /* name */
        case 5:      /* title */
        case 7:      /* comment */
        case 13:     /* region */
		case 22:     /* heterogen */
            MemFree(pnt);
            break;
        case 6:      /* org */
            OrgRefFree((OrgRefPtr)pnt);
            break;
        case 8:      /* num */
            NumberingFree((ValNodePtr)pnt);
            break;
        case 9:      /* maploc */
            DbtagFree((DbtagPtr)pnt);
            break;
        case 10:     /* pir */
            PirBlockFree((PirBlockPtr)pnt);
            break;
        case 11:     /* genbank */
            GBBlockFree((GBBlockPtr)pnt);
            break;
        case 12:     /* pub */
            PubdescFree((PubdescPtr)pnt);
            break;
        case 14:     /* user */
            UserObjectFree((UserObjectPtr)pnt);
            break;
        case 15:     /* SP */
            SPBlockFree((SPBlockPtr)pnt);
            break;
        case 16:     /* dbxref */
            DbtagFree((DbtagPtr)pnt);
            break;
        case 17:     /* EMBL */
            EMBLBlockFree((EMBLBlockPtr)pnt);
            break;
		case 18:     /* create-date */
		case 19:     /* update-date */
			DateFree((DatePtr)pnt);
			break;
        case 20:     /* PRF */
            PrfBlockFree((PrfBlockPtr)pnt);
            break;
        case 21:     /* PDB */
            PdbBlockFree((PdbBlockPtr)pnt);
            break;
        case 23:     /* BioSource */
            BioSourceFree((BioSourcePtr)pnt);
            break;
        case 24:     /* MolInfo */
            MolInfoFree((MolInfoPtr)pnt);
            break;
    }
    anp = (SeqDescPtr)MemFree(anp);
	return anp;
}


/*****************************************************************************
*
*   SeqDescrAsnWrite(anp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqDescr ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqDescrAsnWrite (ValNodePtr anp, AsnIoPtr aip, AsnTypePtr orig)
{
	return SeqDescrAsnWriteExtra(anp, aip, orig, TRUE);
}

/*****************************************************************************
*
*   SeqDescrExtraCheck(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqDescr ::=)
*       checks for extra descriptors from SeqEntryAsnOut, when descr is NULL
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqDescrExtraCheck (AsnIoPtr aip, AsnTypePtr orig)
{
	return SeqDescrAsnWriteExtra((ValNodePtr)NULL, aip, orig, FALSE);
}
/*****************************************************************************
*
*   SeqDescrAsnWriteExtra(anp, aip, atp, anp_not_null)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqDescr ::=)
*       either anp or extras from SeqEntryAsnOut can be written
*       if anp_not_null, means anp cannot be NULL
*
*****************************************************************************/
static Boolean SeqDescrAsnWriteExtra (ValNodePtr anp, AsnIoPtr aip, AsnTypePtr orig,
					Boolean anp_not_null)
{
	AsnTypePtr atp;
    ValNodePtr oldanp = NULL, tanp;
    Boolean retval = FALSE, had_extra = FALSE, had_anp = FALSE;
	AsnOptionPtr aopp;
	ValNodePtr extras = NULL;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	if (anp != NULL) had_anp = TRUE;
	aopp = AsnIoOptionGet(aip, OP_NCBIOBJSEQ, CHECK_EXTRA_DESC, NULL);
	if (aopp != NULL)
	{
		had_extra = TRUE;
		extras = (ValNodePtr)(aopp->data.ptrvalue);
	}
	else if ((anp == NULL) && (! anp_not_null))  /* nothing to write */
		return TRUE;

	if (! ProgMon("Write SeqDescr"))
		return FALSE;

	atp = AsnLinkType(orig, SEQ_DESCR);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (anp_not_null)
		oldanp = anp;
	else if (extras != NULL)
		oldanp = (ValNodePtr)(extras->data.ptrvalue);

	if (oldanp == NULL)
		{ AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)oldanp)) goto erret; /* SET OF */

    while (extras != NULL)    /* extras first */
    {
	tanp = (ValNodePtr)(extras->data.ptrvalue);
	if (! SeqDescAsnWrite(tanp, aip, SEQ_DESCR_E)) goto erret;
	extras = extras->next;
    }

    while (anp != NULL)
    {
		if (! SeqDescAsnWrite (anp, aip, SEQ_DESCR_E)) goto erret;
        anp = anp->next;
    }
    if (! AsnCloseStruct(aip, atp, oldanp)) goto erret;
    retval = TRUE;
erret:
	if (had_extra)
	{
		AsnIoOptionFree(aip, OP_NCBIOBJSEQ, CHECK_EXTRA_DESC);
	}
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqDescAsnWrite(anp, aip, orig)
*   	Writes one Seqdesc
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqDescAsnWrite (SeqDescPtr anp, AsnIoPtr aip, AsnTypePtr orig)
{
    DataValPtr dvp;
	AsnTypePtr atp;
    Pointer pnt;
    ValNodePtr tmp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, SEQDESC);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

   dvp = &anp->data;
		
   if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, dvp)) goto erret;
   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
       case 1:                   /* moltype */
           if (! AsnWrite(aip, SEQDESC_mol_type, dvp)) goto erret;
           break;
       case 2:                   /* modif */
           if (! AsnOpenStruct(aip, SEQDESC_modif, anp->data.ptrvalue)) goto erret;
           tmp = (ValNodePtr)anp->data.ptrvalue;
           while (tmp != NULL)
           {
               if (! AsnWrite(aip, SEQDESC_modif_E, &tmp->data)) goto erret;
               tmp = tmp->next;
           }
           if (! AsnCloseStruct(aip, SEQDESC_modif, anp->data.ptrvalue)) goto erret;
           break;
       case 3:                   /* method */
           if (! AsnWrite(aip, SEQDESC_method, dvp)) goto erret;
           break;
       case 4:                   /* name */
           if (! AsnWrite(aip, SEQDESC_name, dvp)) goto erret;
           break;
       case 5:                   /* title */
           if (! AsnWrite(aip, SEQDESC_title, dvp)) goto erret;
           break;
       case 6:                   /* org */
           if (! OrgRefAsnWrite((OrgRefPtr)pnt, aip, SEQDESC_org)) goto erret;
           break;
       case 7:                   /* comment */
           if (! AsnWrite(aip, SEQDESC_comment, dvp)) goto erret;
           break;
       case 8:                   /* num */
           if (! NumberingAsnWrite((ValNodePtr)pnt, aip, SEQDESC_num)) goto erret;
           break;
       case 9:                  /* maploc */
           if (! DbtagAsnWrite((DbtagPtr)pnt, aip, SEQDESC_maploc)) goto erret;
           break;
       case 10:                   /* pir */
           if (! PirBlockAsnWrite((PirBlockPtr)pnt, aip, SEQDESC_pir)) goto erret;
           break;
       case 11:                  /* genbank */
           if (! GBBlockAsnWrite((GBBlockPtr)pnt, aip, SEQDESC_genbank)) goto erret;
           break;
       case 12:                  /* pubdesc */
           if (! PubdescAsnWrite((PubdescPtr)pnt, aip, SEQDESC_pub)) goto erret;
           break;
       case 13:                  /* region */
           if (! AsnWrite(aip, SEQDESC_region, dvp)) goto erret;
           break;
       case 14:                   /* user */
           if (! UserObjectAsnWrite((UserObjectPtr)pnt, aip, SEQDESC_user)) goto erret;
           break;
       case 15:                   /* SP */
           if (! SPBlockAsnWrite((SPBlockPtr)pnt, aip, SEQDESC_sp)) goto erret;
           break;
       case 16:                   /* dbxref */
           if (! DbtagAsnWrite((DbtagPtr)pnt, aip, SEQDESC_dbxref)) goto erret;
           break;
       case 17:                   /* EMBL */
           if (! EMBLBlockAsnWrite((EMBLBlockPtr)pnt, aip, SEQDESC_embl)) goto erret;
           break;
	case 18:                   /* create-date */
		if (! DateAsnWrite((DatePtr)pnt, aip, SEQDESC_create_date)) goto erret;
		break;
	case 19:                   /* update-date */
		if (! DateAsnWrite((DatePtr)pnt, aip, SEQDESC_update_date)) goto erret;
		break;
       case 20:                   /* PRF */
           if (! PrfBlockAsnWrite((PrfBlockPtr)pnt, aip, SEQDESC_prf)) goto erret;
           break;
       case 21:                   /* PDB */
           if (! PdbBlockAsnWrite((PdbBlockPtr)pnt, aip, SEQDESC_pdb)) goto erret;
           break;
	case 22:                   /* Heterogen */
           if (! AsnWrite(aip, SEQDESC_het, dvp)) goto erret;
           break;
       case 23:                   /* BioSource */
           if (! BioSourceAsnWrite((BioSourcePtr)pnt, aip, SEQDESC_source)) goto erret;
           break;
       case 24:                   /* MolInfo */
           if (! MolInfoAsnWrite((MolInfoPtr)pnt, aip, SEQDESC_molinfo)) goto erret;
           break;
   }
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqDescrAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL SeqDescrAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    ValNodePtr anp, first=NULL, prev=NULL;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return first;
	}

	if (aip == NULL)
		return first;

	if (! ProgMon("Read SeqDescr"))
		return first;

	if (orig == NULL)           /* SeqDescr ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_DESCR);
	else
		atp = AsnLinkType(orig, SEQ_DESCR);    /* link in local tree */
    if (atp == NULL) return first;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the start struct */

    while ((atp = AsnReadId(aip, amp, atp)) == SEQ_DESCR_E)
    {
		anp = SeqDescAsnRead(aip, atp);
		if (anp == NULL) goto erret;

        if (first == NULL)
            first = anp;
		if (prev != NULL)
			prev->next = anp;
		prev = anp;
    }
    if (atp == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return first;
erret:
    first = SeqDescrFree(first);
    goto ret;
}

/*****************************************************************************
*
*   SeqDescAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN SeqDescPtr LIBCALL SeqDescAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    SeqDescPtr anp=NULL,tmp, tmp2;
    Uint1 choice;
    AsnReadFunc func;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return anp;
	}

	if (aip == NULL)
		return anp;

	if (orig == NULL)           /* SeqDesc ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQDESC);
	else
		atp = AsnLinkType(orig, SEQDESC);    /* link in local tree */
    if (atp == NULL) return anp;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the CHOICE value (nothing) */
	atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;  /* find the choice */

    func = NULL;
    choice = 0;
	if (atp == SEQDESC_mol_type)
    {
        choice = 1;
    }
    else if (atp == SEQDESC_modif)
    {
        choice = 2;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* start struct */
        tmp = NULL;
        while ((atp = AsnReadId(aip, amp, atp)) == SEQDESC_modif_E)
        {
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
            tmp2 = ValNodeNew(tmp);
            if (tmp2 == NULL) goto erret;
            if (tmp == NULL)
                tmp = tmp2;
            tmp2->data.intvalue = av.intvalue;
        }
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end struct */
    }
    else if (atp == SEQDESC_method)
    {
        choice = 3;
    }
    else if (atp == SEQDESC_name)
    {
        choice = 4;
    }
    else if (atp == SEQDESC_title)
    {
        choice = 5;
    }
    else if (atp == SEQDESC_org)
    {
        choice = 6;
        func = (AsnReadFunc) OrgRefAsnRead;
    }
    else if (atp == SEQDESC_comment)
    {
        choice = 7;
    }
    else if (atp == SEQDESC_num)
    {
        choice = 8;
        func = (AsnReadFunc) NumberingAsnRead;
    }
    else if (atp == SEQDESC_maploc)
    {
        choice = 9;
        func = (AsnReadFunc) DbtagAsnRead;
    }
    else if (atp == SEQDESC_pir)
    {
        choice = 10;
        func = (AsnReadFunc) PirBlockAsnRead;
    }
    else if (atp == SEQDESC_genbank)
    {
        choice = 11;
        func = (AsnReadFunc) GBBlockAsnRead;
    }
    else if (atp == SEQDESC_pub)
    {
        choice = 12;
        func = (AsnReadFunc) PubdescAsnRead;
    }
    else if (atp == SEQDESC_region)
    {
        choice = 13;
    }
    else if (atp == SEQDESC_user)
    {
        choice = 14;
        func = (AsnReadFunc) UserObjectAsnRead;
    }
    else if (atp == SEQDESC_sp)
    {
        choice = 15;
        func = (AsnReadFunc) SPBlockAsnRead;
    }
    else if (atp == SEQDESC_dbxref)
    {
        choice = 16;
        func = (AsnReadFunc) DbtagAsnRead;
    }
    else if (atp == SEQDESC_embl)
    {
        choice = 17;
        func = (AsnReadFunc) EMBLBlockAsnRead;
    }
	else if (atp == SEQDESC_create_date)
	{
		choice = 18;
		func = (AsnReadFunc) DateAsnRead;
	}
	else if (atp == SEQDESC_update_date)
	{
		choice = 19;
		func = (AsnReadFunc) DateAsnRead;
	}
    else if (atp == SEQDESC_prf)
    {
        choice = 20;
        func = (AsnReadFunc) PrfBlockAsnRead;
    }
    else if (atp == SEQDESC_pdb)
    {
        choice = 21;
        func = (AsnReadFunc) PdbBlockAsnRead;
    }
    else if (atp == SEQDESC_het)
    {
        choice = 22;
    }
    else if (atp == SEQDESC_source)
    {
        choice = 23;
        func = (AsnReadFunc) BioSourceAsnRead;
    }
    else if (atp == SEQDESC_molinfo)
    {
        choice = 24;
        func = (AsnReadFunc) MolInfoAsnRead;
    }

    anp = SeqDescrNew(NULL);
    if (anp == NULL) goto erret;
    anp->choice = choice;
    if (func != NULL)
    {
        anp->data.ptrvalue = (* func)(aip, atp);
        if (anp->data.ptrvalue == NULL) goto erret;
    }
    else if (choice != 2)
    {
        if (AsnReadVal(aip, atp, &anp->data) <= 0) goto erret;
    }
    else
        anp->data.ptrvalue = (Pointer)tmp;

ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return anp;
erret:
    anp = SeqDescFree(anp);
    goto ret;
}

/*****************************************************************************
*
*   NumberingFree(anp)
*       Frees a chain of Numbering and associated data
*
*****************************************************************************/
NLM_EXTERN NumberingPtr LIBCALL NumberingFree (NumberingPtr anp)
{
    Pointer pnt;

    if (anp == NULL)
        return anp;
    
    pnt = anp->data.ptrvalue;
    switch (anp->choice)
    {
        case 1:     /* num-cont */
            NumContFree((NumContPtr)pnt);
            break;
        case 2:      /* num-enum */
            NumEnumFree((NumEnumPtr)pnt);
            break;
        case 3:     /* ref, sources */
        case 4:      /* ref, aligns */
            if (pnt != NULL)
                SeqAlignFree((SeqAlignPtr)pnt);
            break;
        case 5:     /* real */
            NumRealFree((NumRealPtr)pnt);
            break;
    }
    
	return (NumberingPtr)MemFree(anp);
}
/*****************************************************************************
*
*   NumberingAsnWrite(anp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (Numbering ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL NumberingAsnWrite (NumberingPtr anp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Pointer pnt;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, NUMBERING);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

	av.ptrvalue = (Pointer)anp;
    if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av)) goto erret;

    pnt = anp->data.ptrvalue;
    switch (anp->choice)
    {
        case 1:                   /* num-cont */
            if (! NumContAsnWrite((NumContPtr)pnt, aip, NUMBERING_cont)) goto erret;
            break;
        case 2:                   /* num-enum */
            if (! NumEnumAsnWrite((NumEnumPtr)pnt, aip, NUMBERING_enum)) goto erret;
            break;
        case 3:                   /* num-ref, sources */
        case 4:                   /* num-ref, aligns */
            if (! AsnOpenStruct(aip, NUMBERING_ref, pnt)) goto erret;
            av.intvalue = anp->choice - 2;
            if (! AsnWrite(aip, NUM_REF_type, &av)) goto erret;
            if (pnt != NULL)
            {
                if (! SeqAlignAsnWrite((SeqAlignPtr)pnt, aip, NUM_REF_aligns)) goto erret;
            }
            if (! AsnCloseStruct(aip, NUMBERING_ref, pnt)) goto erret;
            break;
        case 5:                   /* real */
            if (! NumRealAsnWrite((NumRealPtr)pnt, aip, NUMBERING_real)) goto erret;
            break;
    }
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}
/*****************************************************************************
*
*   NumberingAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN NumberingPtr LIBCALL NumberingAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    NumberingPtr anp = NULL;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return anp;
	}

	if (aip == NULL)
		return anp;

	if (orig == NULL)           /* Numbering ::= (self contained) */
		atp = AsnReadId(aip, amp, NUMBERING);
	else
		atp = AsnLinkType(orig, NUMBERING);    /* link in local tree */
    if (atp == NULL) return anp;

    anp = ValNodeNew(NULL);
    if (anp == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the CHOICE */
    
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* kind of choice */
    if (atp == NUMBERING_cont)
    {
        anp->choice = 1;
        anp->data.ptrvalue = (Pointer) NumContAsnRead(aip, atp);
    }
    else if (atp == NUMBERING_enum)
    {
        anp->choice = 2;
        anp->data.ptrvalue = (Pointer) NumEnumAsnRead(aip, atp);
    }
    else if (atp == NUMBERING_real)
    {
        anp->choice = 5;
        anp->data.ptrvalue = (Pointer) NumRealAsnRead(aip, atp);
    }
    else if (atp == NUMBERING_ref)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* start struct */
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* type */
        if (av.intvalue == 1)
            anp->choice = 3;
        else if (av.intvalue == 2)
            anp->choice = 4;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
        if (atp == NUM_REF_aligns)
            anp->data.ptrvalue = (Pointer) SeqAlignAsnRead(aip, atp);
    }
    if (anp->data.ptrvalue == NULL) goto erret;
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return anp;
erret:
    anp = NumberingFree(anp);
    goto ret;
}

/*****************************************************************************
*
*   NumContNew()
*
*****************************************************************************/
NLM_EXTERN NumContPtr LIBCALL NumContNew (void)
{
    NumContPtr ncp;
    ncp = (NumContPtr)MemNew(sizeof(NumCont));
	if (ncp == NULL) return NULL;

    ncp->ascending = TRUE;   /* default states */
    ncp->refnum = 1;
    return ncp;
}

/*****************************************************************************
*
*   NumContFree(ncp)
*       Frees a NumCont and associated data
*
*****************************************************************************/
NLM_EXTERN NumContPtr LIBCALL NumContFree (NumContPtr ncp)
{
    if (ncp == NULL)
        return ncp;

	return (NumContPtr)MemFree(ncp);
}
/*****************************************************************************
*
*   NumContAsnWrite(ncp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (NumCont ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL NumContAsnWrite (NumContPtr ncp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, NUM_CONT);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (ncp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)ncp)) goto erret;
    if (ncp->refnum != 1)
    {
        av.intvalue = ncp->refnum;
        if (! AsnWrite(aip, NUM_CONT_refnum, &av)) goto erret;
    }
    if (ncp->has_zero)
    {
        av.boolvalue = ncp->has_zero;
        if (! AsnWrite(aip, NUM_CONT_has_zero, &av)) goto erret;
    }
    if (! ncp->ascending)
    {
        av.boolvalue = ncp->ascending;
        if (! AsnWrite(aip, NUM_CONT_ascending, &av)) goto erret;
    }

    if (! AsnCloseStruct(aip, atp, (Pointer)ncp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   NumContAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN NumContPtr LIBCALL NumContAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldatp;
    NumContPtr ncp=NULL;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return ncp;
	}

	if (aip == NULL)
		return ncp;

	if (orig == NULL)           /* NumCont ::= (self contained) */
		atp = AsnReadId(aip, amp, NUM_CONT);
	else
		atp = AsnLinkType(orig, NUM_CONT);    /* link in local tree */
    oldatp = atp;
    if (atp == NULL) return ncp;

    ncp = NumContNew();
    if (ncp == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the start struct */
    
    while ((atp = AsnReadId(aip, amp, atp)) != oldatp)
    {
        if (atp == NULL) goto erret;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        if (atp == NUM_CONT_refnum)
            ncp->refnum = av.intvalue;
        else if (atp == NUM_CONT_has_zero)
            ncp->has_zero = av.boolvalue;
        else if (atp == NUM_CONT_ascending)
            ncp->ascending = av.boolvalue;
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return ncp;
erret:
    ncp = NumContFree(ncp);
    goto ret;
}

/*****************************************************************************
*
*   NumEnumNew()
*
*****************************************************************************/
NLM_EXTERN NumEnumPtr LIBCALL NumEnumNew (void)
{
    NumEnumPtr nep;
    nep = (NumEnumPtr)MemNew(sizeof(NumEnum));
    return nep;
}

/*****************************************************************************
*
*   NumEnumFree(nep)
*       Frees a NumEnum and associated data
*
*****************************************************************************/
NLM_EXTERN NumEnumPtr LIBCALL NumEnumFree (NumEnumPtr nep)
{
    if (nep == NULL)
        return nep;
    
    MemFree(nep->buf);
    MemFree(nep->names);
	return (NumEnumPtr)MemFree(nep);
}

/*****************************************************************************
*
*   NumEnumAsnWrite(nep, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (NumEnum ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL NumEnumAsnWrite (NumEnumPtr nep, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Int4 i, num;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, NUM_ENUM);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (nep == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)nep)) goto erret;

    num = nep->num;
    av.intvalue = num;
    if (! AsnWrite(aip, NUM_ENUM_num, &av)) goto erret;
    if (! AsnOpenStruct(aip, NUM_ENUM_names, (Pointer)nep->names)) goto erret;
    for (i = 0; i < num; i++)
    {
        av.ptrvalue = (Pointer) nep->names[i];
        if (! AsnWrite(aip, NUM_ENUM_names_E, &av)) goto erret;
    }
    if (! AsnCloseStruct(aip, NUM_ENUM_names, (Pointer)nep->names)) goto erret;

    if (! AsnCloseStruct(aip, atp, (Pointer)nep)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   NumEnumAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN NumEnumPtr LIBCALL NumEnumAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    NumEnumPtr nep=NULL;
    Int4 num, i, total, used, oldtotal, j;
    CharPtr pnt, tmp, tmp2;
    CharPtr PNTR names2;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return nep;
	}

	if (aip == NULL)
		return nep;

	if (orig == NULL)           /* NumEnum ::= (self contained) */
		atp = AsnReadId(aip, amp, NUM_ENUM);
	else
		atp = AsnLinkType(orig, NUM_ENUM);    /* link in local tree */
    if (atp == NULL) return nep;

    nep = NumEnumNew();
    if (nep == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the start struct */
    
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* get num */
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    num = av.intvalue;
    nep->num = num;
    i = 0;
    total = num * 4;
    used = 0;
    nep->buf = (CharPtr)MemNew((size_t)total);   /* assume an average of 3 chars/name */
    if (nep->buf == NULL) goto erret;
    nep->names = (CharPtr PNTR)MemNew((size_t)(num * sizeof(CharPtr)));
    if (nep->names == NULL) goto erret;
    pnt = nep->buf;
    names2 = nep->names;

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* name list */
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;

    while ((atp = AsnReadId(aip, amp, atp)) == NUM_ENUM_names_E)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        tmp = (CharPtr)av.ptrvalue;
        used += StringLen(tmp) + 1;
        if (i == num)       /* out of pointers -- num is in ERROR */
        {
            num += 10;
            names2 = nep->names;
            nep->names = (CharPtr PNTR)MemNew((size_t)(num * sizeof(CharPtr)));
            if (nep->names == NULL)
            {
                nep->names = names2;
                goto erret;
            }
            MemCopy(nep->names, names2, (size_t)(i * sizeof(CharPtr)));
            MemFree(names2);
            names2 = nep->names;
        }
        if (used > total)     /* out of buffer space */
        {
            oldtotal = total;
            total += 100;
            tmp2 = nep->buf;
            nep->buf = (CharPtr)MemNew((size_t)total);
            if (nep->buf == NULL)
            {
                nep->buf = tmp2;
                goto erret;
            }
            MemCopy(nep->buf, tmp2, (size_t)oldtotal);
            MemFree(tmp2);
            pnt = nep->buf;
            for (j = 0; j < i; j++)
            {
                names2[j] = pnt;
                while (* pnt != '\0')
                    pnt++;
                pnt++;
            }
        }
        names2[i] = pnt;
        pnt = StringMove(pnt, tmp);
        pnt++;   /* move past \0 */
        i++;
        MemFree(tmp);
    }
    if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* end struct names */

    nep->num = i;    /* record real count of names -- self correcting */

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return nep;
erret:
    nep = NumEnumFree(nep);
    goto ret;
}

/*****************************************************************************
*
*   NumRealNew()
*
*****************************************************************************/
NLM_EXTERN NumRealPtr LIBCALL NumRealNew (void)
{
    return (NumRealPtr)MemNew(sizeof(NumReal));
}

/*****************************************************************************
*
*   NumRealFree(nrp)
*       Frees a NumReal and associated data
*
*****************************************************************************/
NLM_EXTERN NumRealPtr LIBCALL NumRealFree (NumRealPtr nrp)
{
    if (nrp == NULL)
        return nrp;

    MemFree(nrp->units);
	return (NumRealPtr)MemFree(nrp);
}

/*****************************************************************************
*
*   NumRealAsnWrite(nrp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (NumReal ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL NumRealAsnWrite (NumRealPtr nrp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, NUM_REAL);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (nrp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)nrp)) goto erret;

    av.realvalue = nrp->a;
    if (! AsnWrite(aip, NUM_REAL_a, &av)) goto erret;

    av.realvalue = nrp->b;
    if (! AsnWrite(aip, NUM_REAL_b, &av)) goto erret;

    if (nrp->units != NULL)
    {
        av.ptrvalue = nrp->units;
        if (! AsnWrite(aip, NUM_REAL_units, &av)) goto erret;
    }

    if (! AsnCloseStruct(aip, atp, (Pointer)nrp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   NumRealAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN NumRealPtr LIBCALL NumRealAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldatp;
    NumRealPtr nrp=NULL;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return nrp;
	}

	if (aip == NULL)
		return nrp;

	if (orig == NULL)           /* NumReal ::= (self contained) */
		atp = AsnReadId(aip, amp, NUM_REAL);
	else
		atp = AsnLinkType(orig, NUM_REAL);    /* link in local tree */
    oldatp = atp;
    if (atp == NULL) return nrp;

    nrp = NumRealNew();
    if (nrp == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the start struct */
    
    while ((atp = AsnReadId(aip, amp, atp)) != oldatp)
    {
        if (atp == NULL) goto erret;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        if (atp == NUM_REAL_a)
            nrp->a = av.realvalue;
        else if (atp == NUM_REAL_b)
            nrp->b = av.realvalue;
        else if (atp == NUM_REAL_units)
            nrp->units = (CharPtr)av.ptrvalue;
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return nrp;
erret:
    nrp = NumRealFree(nrp);
    goto ret;
}

/*****************************************************************************
*
*   PubdescNew()
*
*****************************************************************************/
NLM_EXTERN PubdescPtr LIBCALL PubdescNew (void)
{
    return (PubdescPtr)MemNew(sizeof(Pubdesc));
}

/*****************************************************************************
*
*   PubdescFree(pdp)
*       Frees a Pubdesc and associated data
*
*****************************************************************************/
NLM_EXTERN PubdescPtr LIBCALL PubdescFree (PubdescPtr pdp)
{
    if (pdp == NULL)
        return pdp;

    PubEquivFree(pdp->pub);
    MemFree(pdp->name);
    MemFree(pdp->fig);
    NumberingFree(pdp->num);
    MemFree(pdp->maploc);
    MemFree(pdp->seq_raw);
	MemFree(pdp->comment);
	return (PubdescPtr)MemFree(pdp);
}

/*****************************************************************************
*
*   PubdescAsnWrite(pdp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (Pubdesc ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL PubdescAsnWrite (PubdescPtr pdp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, PUBDESC);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (pdp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)pdp)) goto erret;
    if (! PubEquivAsnWrite(pdp->pub, aip, PUBDESC_pub)) goto erret;

    if (pdp->name != NULL)
    {
        av.ptrvalue = pdp->name;
        if (! AsnWrite(aip, PUBDESC_name, &av)) goto erret;
    }
    if (pdp->fig != NULL)
    {
        av.ptrvalue = pdp->fig;
        if (! AsnWrite(aip, PUBDESC_fig, &av)) goto erret;
    }
    if (pdp->num != NULL)
    {
        if (! NumberingAsnWrite(pdp->num, aip, PUBDESC_num)) goto erret;
    }
    if (pdp->numexc)
    {
        av.boolvalue = pdp->numexc;
        if (! AsnWrite(aip, PUBDESC_numexc, &av)) goto erret;
    }
    if (pdp->poly_a)
    {
        av.boolvalue = pdp->poly_a;
        if (! AsnWrite(aip, PUBDESC_poly_a, &av)) goto erret;
    }
    if (pdp->maploc != NULL)
    {
        av.ptrvalue = pdp->maploc;
        if (! AsnWrite(aip, PUBDESC_maploc, &av)) goto erret;
    }
    if (pdp->seq_raw != NULL)
    {
        av.ptrvalue = pdp->seq_raw;
        if (! AsnWrite(aip, PUBDESC_seq_raw, &av)) goto erret;
    }
    if (pdp->align_group)
    {
        av.intvalue = (Int4)pdp->align_group;
        if (! AsnWrite(aip, PUBDESC_align_group, &av)) goto erret;
    }
    if (pdp->comment != NULL)
    {
        av.ptrvalue = pdp->comment;
        if (! AsnWrite(aip, PUBDESC_comment, &av)) goto erret;
    }
    if (pdp->reftype)
    {
        av.intvalue = (Int4)(pdp->reftype);
        if (! AsnWrite(aip, PUBDESC_reftype, &av)) goto erret;
    }

    if (! AsnCloseStruct(aip, atp, (Pointer)pdp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   PubdescAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN PubdescPtr LIBCALL PubdescAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldatp;
    PubdescPtr pdp=NULL;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return pdp;
	}

	if (aip == NULL)
		return pdp;

	if (orig == NULL)           /* Pubdesc ::= (self contained) */
		atp = AsnReadId(aip, amp, PUBDESC);
	else
		atp = AsnLinkType(orig, PUBDESC);    /* link in local tree */
    oldatp = atp;
    if (atp == NULL) return pdp;

    pdp = PubdescNew();
    if (pdp == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the start struct */
    
    while ((atp = AsnReadId(aip, amp, atp)) != oldatp)
    {
        if (atp == NULL) goto erret;
        if (atp == PUBDESC_pub)
        {
            pdp->pub = PubEquivAsnRead(aip, atp);
            if (pdp->pub == NULL) goto erret;
        }
        else if (atp == PUBDESC_num)
        {
            pdp->num = NumberingAsnRead(aip, atp);
            if (pdp->num == NULL) goto erret;
        }
        else
        {
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
            if (atp == PUBDESC_name)
                pdp->name = (CharPtr)av.ptrvalue;
            else if (atp == PUBDESC_fig)
                pdp->fig = (CharPtr)av.ptrvalue;
            else if (atp == PUBDESC_numexc)
                pdp->numexc = av.boolvalue;
            else if (atp == PUBDESC_poly_a)
                pdp->poly_a = av.boolvalue;
            else if (atp == PUBDESC_maploc)
                pdp->maploc = (CharPtr)av.ptrvalue;
            else if (atp == PUBDESC_seq_raw)
                pdp->seq_raw = (CharPtr)av.ptrvalue;
            else if (atp == PUBDESC_align_group)
                pdp->align_group = (Uint1)av.intvalue;
			else if (atp == PUBDESC_comment)
				pdp->comment = (CharPtr)av.ptrvalue;
            else if (atp == PUBDESC_reftype)
                pdp->reftype = (Uint1)av.intvalue;
        }
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return pdp;
erret:
    pdp = PubdescFree(pdp);
    goto ret;
}

/*****************************************************************************
*
*   SeqAnnotNew()
*
*****************************************************************************/
NLM_EXTERN SeqAnnotPtr LIBCALL SeqAnnotNew (void)
{
    return (SeqAnnotPtr)MemNew(sizeof(SeqAnnot));
}

/*****************************************************************************
*
*   SeqAnnotFree(sap)
*       Frees a SeqAnnot and associated data
*
*****************************************************************************/
NLM_EXTERN SeqAnnotPtr LIBCALL SeqAnnotFree (SeqAnnotPtr sap)
{
    SeqFeatPtr sfp, sfpnext;
    SeqAlignPtr sp, spnext;
    SeqGraphPtr sgp, sgpnext;
	SeqIdPtr sip;
	SeqLocPtr slp;

    if (sap == NULL)
        return sap;

	ObjMgrDelete(OBJ_SEQANNOT, (Pointer)sap);

    AnnotIdSetFree(sap->id);
    MemFree(sap->name);
    AnnotDescrFree(sap->desc);
    switch (sap->type)
    {
        case 1:   /* ftable */
            sfp = (SeqFeatPtr)sap->data;
            while (sfp != NULL)
            {
				sfpnext = sfp->next;
                SeqFeatFree(sfp);
                sfp = sfpnext;
            }
            break;
        case 2:   /* alignments */
            sp = (SeqAlignPtr)sap->data;
            while (sp != NULL)
            {
				spnext = sp->next;
                SeqAlignFree(sp);
                sp = spnext;
            }
            break;
        case 3:
            sgp = (SeqGraphPtr)sap->data;
            while (sgp != NULL)
            {
				sgpnext = sgp->next;
                SeqGraphFree(sgp);
                sgp = sgpnext;
            }
            break;
		case 4:    /* SeqIds */
			sip = (SeqIdPtr)sap->data;
			SeqIdSetFree(sip);
			break;
		case 5:   /* SeqLocs */
			slp = (SeqLocPtr)sap->data;
			SeqLocSetFree(slp);
			break;

    }
	return (SeqAnnotPtr)MemFree(sap);
}

/*****************************************************************************
*
*   SeqAnnotAsnWrite(sap, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqAnnot ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqAnnotAsnWrite (SeqAnnotPtr sap, AsnIoPtr aip, AsnTypePtr orig)
{
	return SeqAnnotAsnWriteExtra(sap, aip, orig, (ValNodePtr)NULL);
}

/*****************************************************************************
*
*   SeqAnnotAsnWriteExtra(sap, aip, atp, extras)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqAnnot ::=)
*       if extras != NULL
*           assumes sap has no data ptr
*           assumes extras is a ValNode chain pointing to SeqFeats from
*              SeqEntryAsnOut()
*       else
*           ignores extras and writes the SeqAnnot
*
*****************************************************************************/
static Boolean SeqAnnotAsnWriteExtra (SeqAnnotPtr sap, AsnIoPtr aip, AsnTypePtr orig,
						  ValNodePtr extras)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	if (! ProgMon("Write SeqAnnot"))
		return FALSE;

	if ((aip->spec_version == 3 )  &&    /* ASN3 strip new value */
		(sap->type >3))
	{
		ErrPostEx(SEV_ERROR,0,0,"ASN3: SeqAnnot > 3 stripped");
		return TRUE;
	}

	atp = AsnLinkType(orig, SEQ_ANNOT);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (sap == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)sap)) goto erret;

    if (sap->id != NULL)
    {
        if (! AnnotIdSetAsnWrite(sap->id, aip, SEQ_ANNOT_id, SEQ_ANNOT_id_E)) goto erret;
    }
    if (sap->db)
    {
		av.intvalue = (Int4)(sap->db);
		if (! AsnWrite(aip, SEQ_ANNOT_db, &av)) goto erret;
    }
    if (sap->name != NULL)
    {
        av.ptrvalue = sap->name;
        if (! AsnWrite(aip, SEQ_ANNOT_name, &av)) goto erret;
    }
    if (sap->desc != NULL)
    {
		if (! AnnotDescrAsnWrite(sap->desc, aip, SEQ_ANNOT_desc)) goto erret;
    }
	av.ptrvalue = sap->data;
    if (! AsnWriteChoice(aip, SEQ_ANNOT_data, (Int2)sap->type, &av)) goto erret;
    switch (sap->type)
    {
        case 1:
	    if (extras != NULL)  /* from SeqEntryAsnOut */
	    {
		if (! SeqFeatSetAsnWriteExtra((SeqFeatPtr)sap->data, aip, SEQ_ANNOT_data_ftable,
					      SEQ_ANNOT_data_ftable_E, extras)) goto erret;
	    }
	    else
	    {
		if (! SeqFeatSetAsnWrite((SeqFeatPtr)sap->data, aip, SEQ_ANNOT_data_ftable, SEQ_ANNOT_data_ftable_E)) goto erret;
	    }
            break;
        case 2:
            if (! SeqAlignSetAsnWrite((SeqAlignPtr)sap->data, aip, SEQ_ANNOT_data_align, SEQ_ANNOT_data_align_E)) goto erret;
            break;
        case 3:
            if (! SeqGraphSetAsnWrite((SeqGraphPtr)sap->data, aip, SEQ_ANNOT_data_graph, SEQ_ANNOT_data_graph_E)) goto erret;
            break;
        case 4:
            if (! SeqIdSetAsnWrite((SeqIdPtr)sap->data, aip, SEQ_ANNOT_data_ids, SEQ_ANNOT_data_ids_E)) goto erret;
            break;
        case 5:
            if (! SeqLocSetAsnWrite((SeqLocPtr)sap->data, aip, SEQ_ANNOT_data_locs, SEQ_ANNOT_data_locs_E)) goto erret;
            break;
    }

    if (! AsnCloseStruct(aip, atp, (Pointer)sap)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqAnnotAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN SeqAnnotPtr LIBCALL SeqAnnotAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    SeqAnnotPtr sap=NULL;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return sap;
	}

	if (aip == NULL)
		return sap;

	if (! ProgMon("Read SeqAnnot"))
		return sap;

	if (orig == NULL)           /* SeqAnnot ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_ANNOT);
	else
		atp = AsnLinkType(orig, SEQ_ANNOT);    /* link in local tree */
    if (atp == NULL) return sap;

    sap = SeqAnnotNew();
    if (sap == NULL) goto erret;
    
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the start struct */
    
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (atp == SEQ_ANNOT_id)
    {
        sap->id = AnnotIdSetAsnRead(aip, atp, SEQ_ANNOT_id_E);
        if (sap->id == NULL) goto erret;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }
    if (atp == SEQ_ANNOT_db)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        sap->db = (Uint1)av.intvalue;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }
    if (atp == SEQ_ANNOT_name)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        sap->name = (CharPtr)av.ptrvalue;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }
    if (atp == SEQ_ANNOT_desc)
    {
        sap->desc = AnnotDescrAsnRead(aip, atp);
		if (sap->desc == NULL) goto erret;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* CHOICE */
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;    /* type of choice */
    if (atp == SEQ_ANNOT_data_ftable)
    {
        sap->type = 1;
        sap->data = (Pointer) SeqFeatSetAsnRead(aip, atp, SEQ_ANNOT_data_ftable_E);
    }
    else if (atp == SEQ_ANNOT_data_align)
    {
        sap->type = 2;
        sap->data = (Pointer) SeqAlignSetAsnRead(aip, atp, SEQ_ANNOT_data_align_E);
    }
    else if (atp == SEQ_ANNOT_data_graph)
    {
        sap->type = 3;
        sap->data = (Pointer) SeqGraphSetAsnRead(aip, atp, SEQ_ANNOT_data_graph_E);
    }
    else if (atp == SEQ_ANNOT_data_ids)
    {
        sap->type = 4;
        sap->data = (Pointer) SeqIdSetAsnRead(aip, atp, SEQ_ANNOT_data_ids_E);

		if (aip->spec_version == 3)    /* ASN3 strip new value */
		{
			ErrPostEx(SEV_ERROR,0,0,"ASN3: SeqFeat.ids stripped");
			SeqIdSetFree((SeqIdPtr)(sap->data));
			sap->data = NULL;
			sap->type = 3;
		}
    }
    else if (atp == SEQ_ANNOT_data_locs)
    {
        sap->type = 5;
        sap->data = (Pointer) SeqLocSetAsnRead(aip, atp, SEQ_ANNOT_data_locs_E);

		if (aip->spec_version == 3)    /* ASN3 strip new value */
		{
			ErrPostEx(SEV_ERROR,0,0,"ASN3: SeqFeat.locs stripped");
			SeqLocSetFree((SeqLocPtr)(sap->data));
			sap->data = NULL;
			sap->type = 3;
		}
    }

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* end struct */
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return sap;
erret:
    sap = SeqAnnotFree(sap);
    goto ret;
}

/*****************************************************************************
*
*   SeqAnnotSetAsnWrite(sap, aip, set, element)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqAnnot ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqAnnotSetAsnWrite (SeqAnnotPtr sap, AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element)
{
	return SeqAnnotSetAsnWriteExtra(sap, aip, set, element, TRUE);
}

/*****************************************************************************
*
*   SeqAnnotSetExtraCheck(aip, set, element)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqAnnot ::=)
*       called to check for extra features from SeqEntryAsnOut when sap is NULL
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqAnnotSetExtraCheck (AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element)
{
	return SeqAnnotSetAsnWriteExtra((SeqAnnotPtr)NULL, aip, set, element, FALSE);
}

/*****************************************************************************
*
*   SeqAnnotSetAsnWriteExtra(sap, aip, set, element, sap_not_null)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqAnnot ::=)
*
*****************************************************************************/
static Boolean SeqAnnotSetAsnWriteExtra (SeqAnnotPtr sap, AsnIoPtr aip, AsnTypePtr set,
					 AsnTypePtr element, Boolean sap_not_null)
{
	AsnTypePtr atp;
	SeqAnnotPtr oldsap = NULL, tsap = NULL;
    Boolean retval = FALSE, had_sap = FALSE, had_extra = FALSE;
	AsnOptionPtr aopp;
	ValNodePtr extras = NULL;
	SeqAnnot sa;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	aopp = AsnIoOptionGet(aip, OP_NCBIOBJSEQ, CHECK_EXTRA_FEAT, NULL);
	if (aopp != NULL)
	{
		had_extra = TRUE;
		extras = aopp->data.ptrvalue;
		tsap = &sa;
		MemSet(tsap, 0, sizeof(SeqAnnot));
		tsap->type = 1;  /* feature table */
	}
	else if ((sap == NULL) && (! sap_not_null))  /* nothing to write */
		return TRUE;
		
	atp = AsnLinkType(element, SEQ_ANNOT);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (sap_not_null)
		oldsap = sap;
	else
		oldsap = tsap;

	if (oldsap == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, set, (Pointer)oldsap)) goto erret;

    while (sap != NULL)
    {
        if (! SeqAnnotAsnWrite(sap, aip, atp)) goto erret;
        sap = sap->next;
    }

    if (tsap != NULL)
    {
	if (! SeqAnnotAsnWriteExtra(tsap, aip, atp, extras)) goto erret;
    }

    if (! AsnCloseStruct(aip, set, (Pointer)oldsap)) goto erret;
    retval = TRUE;
erret:
	if (had_extra)
		AsnIoOptionFree(aip, OP_NCBIOBJSEQ, CHECK_EXTRA_FEAT);
	AsnUnlinkType(element);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqAnnotSetAsnRead(aip, set, element)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN SeqAnnotPtr LIBCALL SeqAnnotSetAsnRead (AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element)
{
	DataVal av;
	AsnTypePtr atp;
    SeqAnnotPtr sap, first=NULL, curr;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return first;
	}

	if (aip == NULL)
		return first;

    first = NULL;
    curr = NULL;
	AsnLinkType(element, SEQ_ANNOT);    /* link in local tree */

    if (AsnReadVal(aip, set, &av) <= 0) goto erret;   /* read the start struct */
    
    atp = set;

    while ((atp = AsnReadId(aip, amp, atp)) != set)
    {
        if (atp == NULL) goto erret;
        sap = SeqAnnotAsnRead(aip, atp);
        if (sap == NULL) goto erret;
        if (first == NULL)
            first = sap;
        else
            curr->next = sap;
        curr = sap;
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end SET OF */
	if (first == NULL)
		ErrPostEx(SEV_WARNING, 0,0, "Empty SET OF Seq-annot.  line %ld", (long) aip->linenumber);
ret:
	AsnUnlinkType(element);       /* unlink local tree */
	return first;
erret:
    while (first != NULL)
    {
        curr = first;
        first = curr->next;
        SeqAnnotFree(curr);
    }
    goto ret;
}

/*****************************************************************************
*
*   BioseqLoad()
*       sets up a program to read/write and manipulate Bioseqs
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL BioseqLoad (void)
{
    if (! SeqAsnLoad())
        return FALSE;

    if (SeqCodeSetLoad() == NULL)
    {
        ErrPostEx(SEV_ERROR, 0,0,  "SeqCodeSetLoad failed");
        return FALSE;
    }

    return TRUE;
}

/*****************************************************************************
*
*   SeqHistNew()
*
*****************************************************************************/
NLM_EXTERN SeqHistPtr LIBCALL SeqHistNew (void)
{
    SeqHistPtr shp;
    shp = (SeqHistPtr)MemNew(sizeof(SeqHist));
    return shp;
}

/*****************************************************************************
*
*   SeqHistFree(shp)
*       Frees a SeqHist and associated data
*
*****************************************************************************/
NLM_EXTERN SeqHistPtr LIBCALL SeqHistFree (SeqHistPtr shp)
{
	SeqAlignPtr sap, sapnext;

    if (shp == NULL)
        return shp;

	for (sap = shp->assembly; sap != NULL; sap = sapnext)
	{
		sapnext = sap->next;
		SeqAlignFree(sap);
	}
	DateFree(shp->replace_date);
	SeqIdSetFree(shp->replace_ids);
	DateFree(shp->replaced_by_date);
	SeqIdSetFree(shp->replaced_by_ids);
	DateFree(shp->deleted_date);
		
	return (SeqHistPtr)MemFree(shp);
}

/*****************************************************************************
*
*   SeqHistAsnWrite(shp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqHist ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqHistAsnWrite (SeqHistPtr shp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal dv;
	AsnTypePtr atp;
    Boolean retval = FALSE;
	SeqAlignPtr sap;
	Int2 choice, ctr = 0;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, SEQ_HIST);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (shp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)shp)) goto erret;
    if (shp->assembly != NULL)
    {
       if (! AsnOpenStruct(aip, SEQ_HIST_assembly, (Pointer)shp->assembly)) goto erret;
       sap = shp->assembly;
       while (sap != NULL)
       {
           if (! SeqAlignAsnWrite(sap, aip, SEQ_HIST_assembly_E)) goto erret;
           sap = sap->next;
			ctr++;
			if (ctr == 10)
			{
				if (! ProgMon("Write SeqHist"))
					goto erret;
				ctr = 0;
			}
       }
       if (! AsnCloseStruct(aip, SEQ_HIST_assembly, (Pointer)shp->assembly)) goto erret;
    }

    if ((shp->replace_date != NULL) || (shp->replace_ids != NULL))
    {
       if (! AsnOpenStruct(aip, SEQ_HIST_replaces, (Pointer)shp)) goto erret;
		if (shp->replace_date != NULL)
		{
			if (! DateAsnWrite(shp->replace_date, aip, SEQ_HIST_REC_date))
				goto erret;
		}
		if (shp->replace_ids != NULL)
		{
			if (! SeqIdSetAsnWrite(shp->replace_ids, aip, SEQ_HIST_REC_ids, SEQ_HIST_REC_ids_E))
				goto erret;
		}
       if (! AsnCloseStruct(aip, SEQ_HIST_replaces, (Pointer)shp)) goto erret;
    }

    if ((shp->replaced_by_date != NULL) || (shp->replaced_by_ids != NULL))
    {
       if (! AsnOpenStruct(aip, SEQ_HIST_replaced_by, (Pointer)shp)) goto erret;
		if (shp->replaced_by_date != NULL)
		{
			if (! DateAsnWrite(shp->replaced_by_date, aip, SEQ_HIST_REC_date))
				goto erret;
		}
		if (shp->replaced_by_ids != NULL)
		{
			if (! SeqIdSetAsnWrite(shp->replaced_by_ids, aip, SEQ_HIST_REC_ids, SEQ_HIST_REC_ids_E))
				goto erret;
		}
       if (! AsnCloseStruct(aip, SEQ_HIST_replaced_by, (Pointer)shp)) goto erret;
    }

	if ((shp->deleted) || (shp->deleted_date != NULL))
	{
		dv.ptrvalue = (Pointer)shp;
		if (shp->deleted)
			choice = 0;
		else
			choice = 1;
        if (! AsnWriteChoice(aip, SEQ_HIST_deleted, choice, &dv)) goto erret;   /* CHOICE */
		if (shp->deleted)
		{
			dv.boolvalue = TRUE;
			if (! AsnWrite(aip, SEQ_HIST_deleted_bool, &dv)) goto erret;
		}
		else
		{
			if (! DateAsnWrite(shp->deleted_date, aip, SEQ_HIST_deleted_date))
				goto erret;
		}
	}
    if (! AsnCloseStruct(aip, atp, (Pointer)shp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqHistAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN SeqHistPtr LIBCALL SeqHistAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal dv;
	AsnTypePtr atp, oldatp;
    SeqHistPtr shp=NULL;
	SeqAlignPtr sap, saplast = NULL;
	Int2 in_replace = -1, ctr = 0;
	DatePtr date;
	SeqIdPtr sip;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return shp;
	}

	if (aip == NULL)
		return shp;

	if (orig == NULL)           /* SeqHist ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_HIST);
	else
		atp = AsnLinkType(orig, SEQ_HIST);    /* link in local tree */
    oldatp = atp;
    if (atp == NULL) return shp;

    shp = SeqHistNew();
    if (shp == NULL) goto erret;

    if (AsnReadVal(aip, atp, &dv) <= 0) goto erret;   /* read the start struct */
    
    while ((atp = AsnReadId(aip, amp, atp)) != oldatp)
    {
        if (atp == NULL) goto erret;
		if (atp == SEQ_HIST_assembly_E)
		{
			sap = SeqAlignAsnRead(aip, atp);
			if (sap == NULL) goto erret;
			if (saplast == NULL)
				shp->assembly = sap;
			else
				saplast->next = sap;
			saplast = sap;
			ctr++;
			if (ctr == 10)
			{
				if (! ProgMon("Read SeqHist"))
					goto erret;
				ctr = 0;
			}
		}
		else if (atp == SEQ_HIST_REC_date)
		{
			date = DateAsnRead(aip, atp);
			if (date == NULL) goto erret;
			if (in_replace == 1)
				shp->replace_date = date;
			else
				shp->replaced_by_date = date;
		}
		else if (atp == SEQ_HIST_REC_ids)
		{
			sip = SeqIdSetAsnRead(aip, atp, SEQ_HIST_REC_ids_E);
			if (sip == NULL) goto erret;
			if (in_replace == 1)
				shp->replace_ids = sip;
			else
				shp->replaced_by_ids = sip;
		}
		else if (atp == SEQ_HIST_deleted_date)
		{
			date = DateAsnRead(aip, atp);
			if (date == NULL) goto erret;
			shp->deleted_date = date;
		}
        else
        {
            if (AsnReadVal(aip, atp, &dv) <= 0) goto erret;
            if (atp == SEQ_HIST_replaces)
                in_replace *= -1;
			else if (atp == SEQ_HIST_deleted_bool)
				shp->deleted = dv.boolvalue;
        }
    }
    if (AsnReadVal(aip, atp, &dv) <= 0) goto erret;    /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return shp;
erret:
    shp = SeqHistFree(shp);
    goto ret;
}

/*****************************************************************************
*
*   MolInfoNew()
*
*****************************************************************************/
NLM_EXTERN MolInfoPtr LIBCALL MolInfoNew (void)
{
    return (MolInfoPtr)MemNew(sizeof(MolInfo));
}

/*****************************************************************************
*
*   MolInfoFree(mip)
*       Frees a MolInfo and associated data
*
*****************************************************************************/
NLM_EXTERN MolInfoPtr LIBCALL MolInfoFree (MolInfoPtr mip)
{
    if (mip == NULL)
        return mip;

    MemFree(mip->techexp);
	return (MolInfoPtr)MemFree(mip);
}

/*****************************************************************************
*
*   MolInfoAsnWrite(mip, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (MolInfo ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL MolInfoAsnWrite (MolInfoPtr mip, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, MOLINFO);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (mip == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)mip)) goto erret;

	if (mip->biomol)
	{
	    av.intvalue = (Int4)mip->biomol;
	    if (! AsnWrite(aip, MOLINFO_biomol, &av)) goto erret;
	}

	if (mip->tech)
	{
	    av.intvalue = (Int4)mip->tech;
	    if (! AsnWrite(aip, MOLINFO_tech, &av)) goto erret;
	}

	if (mip->techexp != NULL)
	{
	    av.ptrvalue = (Pointer)mip->techexp;
	    if (! AsnWrite(aip, MOLINFO_techexp, &av)) goto erret;
	}

	if (mip->completeness)
	{
	    av.intvalue = (Int4)mip->completeness;
	    if (! AsnWrite(aip, MOLINFO_completeness, &av)) goto erret;
	}

    if (! AsnCloseStruct(aip, atp, (Pointer)mip)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   MolInfoAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN MolInfoPtr LIBCALL MolInfoAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldatp;
    MolInfoPtr mip=NULL;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return mip;
	}

	if (aip == NULL)
		return mip;

	if (orig == NULL)           /* MolInfo ::= (self contained) */
		atp = AsnReadId(aip, amp, MOLINFO);
	else
		atp = AsnLinkType(orig, MOLINFO);    /* link in local tree */
    oldatp = atp;
    if (atp == NULL) return mip;

    mip = MolInfoNew();
    if (mip == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the start struct */
    
    while ((atp = AsnReadId(aip, amp, atp)) != oldatp)
    {
        if (atp == NULL) goto erret;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        if (atp == MOLINFO_biomol)
            mip->biomol = (Uint1)av.intvalue;
        else if (atp == MOLINFO_tech)
            mip->tech = (Uint1)av.intvalue;
        else if (atp == MOLINFO_techexp)
            mip->techexp = (CharPtr)av.ptrvalue;
        else if (atp == MOLINFO_completeness)
            mip->completeness = (Uint1)av.intvalue;
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return mip;
erret:
    mip = MolInfoFree(mip);
    goto ret;
}

/*****************************************************************************
*
*   SeqLitNew()
*
*****************************************************************************/
NLM_EXTERN SeqLitPtr LIBCALL SeqLitNew (void)
{
    SeqLitPtr slp;

    slp = (SeqLitPtr)MemNew(sizeof(SeqLit));
    return slp;
}

/*****************************************************************************
*
*   SeqLitFree(slp)
*       Frees a SeqLit and associated data
*
*****************************************************************************/
NLM_EXTERN SeqLitPtr LIBCALL SeqLitFree (SeqLitPtr slp)
{
    if (slp == NULL)
        return slp;

	BSFree(slp->seq_data);
	IntFuzzFree(slp->fuzz);
	return (SeqLitPtr)MemFree(slp);
}
/*****************************************************************************
*
*   SeqLitAsnWrite(slp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqLit ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqLitAsnWrite (SeqLitPtr slp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, SEQ_LITERAL);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (slp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)slp)) goto erret;

    av.intvalue = slp->length;
    if (! AsnWrite(aip, SEQ_LITERAL_length, &av)) goto erret;

    if (slp->fuzz != NULL)
    {
        if (! IntFuzzAsnWrite(slp->fuzz, aip, SEQ_LITERAL_fuzz)) goto erret;
    }

    if (slp->seq_data != NULL)
    {
        if (! SeqDataAsnWrite(slp->seq_data, slp->seq_data_type, aip, SEQ_LITERAL_seq_data)) goto erret;
    }

    if (! AsnCloseStruct(aip, atp, (Pointer)slp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqLitAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN SeqLitPtr LIBCALL SeqLitAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldatp;
    SeqLitPtr slp=NULL;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return slp;
	}

	if (aip == NULL)
		return slp;

	if (orig == NULL)           /* SeqLit ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_LITERAL);
	else
		atp = AsnLinkType(orig, SEQ_LITERAL);    /* link in local tree */
    oldatp = atp;
    if (atp == NULL) return slp;

    slp = SeqLitNew();
    if (slp == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the start struct */
    
    while ((atp = AsnReadId(aip, amp, atp)) != oldatp)
    {
        if (atp == NULL) goto erret;
        if (atp == SEQ_LITERAL_length)
		{
	        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
            slp->length = av.intvalue;
		}
        else if (atp == SEQ_LITERAL_fuzz)
		{
            slp->fuzz = IntFuzzAsnRead(aip, atp);
			if (slp->fuzz == NULL) goto erret;
		}
        else if (atp == SEQ_LITERAL_seq_data)
		{
            slp->seq_data = SeqDataAsnRead(aip, atp, &(slp->seq_data_type), slp->length);
			if (slp->seq_data == NULL) goto erret;
		}
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return slp;
erret:
    slp = SeqLitFree(slp);
    goto ret;
}

/*****************************************************************************
*
*   DeltaSeqFree(anp)
*       Frees one DeltaSeq and associated data
*
*****************************************************************************/
NLM_EXTERN DeltaSeqPtr LIBCALL DeltaSeqFree (DeltaSeqPtr dsp)
{
    if (dsp == NULL)
        return dsp;
    
    switch (dsp->choice)
    {
		case 1:
			SeqLocFree((SeqLocPtr)(dsp->data.ptrvalue));
			break;
		case 2:
			SeqLitFree((SeqLitPtr)(dsp->data.ptrvalue));
			break;
    }
	return (DeltaSeqPtr)MemFree(dsp);
}

/*****************************************************************************
*
*   DeltaSeqAsnWrite(dsp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (DeltaSeq ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL DeltaSeqAsnWrite (DeltaSeqPtr dsp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, DELTA_SEQ);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (dsp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

	av.ptrvalue = (Pointer)dsp;
	if (! AsnWriteChoice(aip, atp, (Int2)dsp->choice, &av))
        goto erret;

    switch (dsp->choice)
    {
        case 1:
            retval = SeqLocAsnWrite((SeqLocPtr)(dsp->data.ptrvalue), aip, DELTA_SEQ_loc);
            break;
        case 2:
			retval = SeqLitAsnWrite((SeqLitPtr)(dsp->data.ptrvalue), aip, DELTA_SEQ_literal);
            break;
    }
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   DeltaSeqAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN DeltaSeqPtr LIBCALL DeltaSeqAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    DeltaSeqPtr dsp=NULL;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return dsp;
	}

	if (aip == NULL)
		return dsp;

	if (orig == NULL)           /* DeltaSeq ::= (self contained) */
		atp = AsnReadId(aip, amp, DELTA_SEQ);
	else
		atp = AsnLinkType(orig, DELTA_SEQ);    /* link in local tree */
    if (atp == NULL)
        return dsp;

	dsp = ValNodeNew(NULL);
    if (dsp == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0)  /* read the CHOICE value (nothing) */
        goto erret;
	atp = AsnReadId(aip, amp, atp);  /* find the choice */
    if (atp == NULL)
        goto erret;

	if (atp == DELTA_SEQ_loc)
    {
        dsp->choice = 1;
		dsp->data.ptrvalue = (Pointer) SeqLocAsnRead(aip, atp);
    }
    else if (atp == DELTA_SEQ_literal)
    {
        dsp->choice = 2;
		dsp->data.ptrvalue = (Pointer) SeqLitAsnRead(aip, atp);
    }

	if (dsp->data.ptrvalue == NULL) goto erret;
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return dsp;
erret:
    dsp = DeltaSeqFree(dsp);
    goto ret;
}

/*****************************************************************************
*
*   DeltaSeqSetAsnWrite(dsp, aip, atp, set, element)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (DeltaSeqSet ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL DeltaSeqSetAsnWrite (DeltaSeqPtr dsp, AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element)
{
    AsnTypePtr atp2;
	DeltaSeqPtr olddsp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

    atp2 = AsnLinkType(element, DELTA_SEQ);
    if (atp2 == NULL)
        return FALSE;

	if (dsp == NULL) { AsnNullValueMsg(aip, set); goto erret; }

	olddsp = dsp;
    if (! AsnOpenStruct(aip, set, (Pointer)olddsp))
        goto erret;
    while (dsp != NULL)
    {
        if (! DeltaSeqAsnWrite(dsp, aip, atp2))
            goto erret;
        dsp = dsp->next;
    }
    if (! AsnCloseStruct(aip, set, (Pointer)olddsp))
        goto erret;
    retval = TRUE;
erret:
    AsnUnlinkType(element);
	return retval;
}

/*****************************************************************************
*
*   DeltaSeqSetAsnRead(aip, atp, set, element)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN DeltaSeqPtr LIBCALL DeltaSeqSetAsnRead (AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element)
{
	DataVal av;
	AsnTypePtr atp, atp2;
    DeltaSeqPtr dsp, first=NULL, prev;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return first;
	}

	if (aip == NULL)
		return first;

    prev = NULL;

    atp2 = AsnLinkType(element, DELTA_SEQ);
    if (atp2 == NULL)
        return first;
    atp = set;
	if (AsnReadVal(aip, atp, &av) <= 0)    /* read the START_STRUCT */
        goto erret;
    while ((atp = AsnReadId(aip, amp, atp)) != set)
    {
        if (atp == NULL)
            goto erret;
        dsp = DeltaSeqAsnRead(aip, atp2);
        if (dsp == NULL)
            goto erret;
        if (first == NULL)
            first = dsp;
        else
            prev->next = dsp;
        prev = dsp;
    }
    if (AsnReadVal(aip, atp, &av) <= 0)      /* read END_STRUCT */
        goto erret;
	if (first == NULL)
		ErrPostEx(SEV_WARNING, 0,0, "Empty SET OF Delta-seq.  line %ld", (long) aip->linenumber);
ret:
    AsnUnlinkType(element);
	return first;
erret:
    first = DeltaSeqSetFree(first);
    goto ret;
}

/*****************************************************************************
*
*   DeltaSeqSetFree(dsp)
*
*****************************************************************************/
NLM_EXTERN DeltaSeqPtr LIBCALL DeltaSeqSetFree (DeltaSeqPtr dsp)
{
    DeltaSeqPtr next;

    while (dsp != NULL)
    {
        next = dsp->next;
        DeltaSeqFree(dsp);
        dsp = next;
    }
    return dsp;
}

/*****************************************************************************
*
*   AnnotIdFree(anp)
*       Frees one AnnotId and associated data
*
*****************************************************************************/
NLM_EXTERN AnnotIdPtr LIBCALL AnnotIdFree (AnnotIdPtr anp)
{
    Pointer pnt;

    if (anp == NULL)
        return anp;
    
    pnt = anp->data.ptrvalue;
    switch (anp->choice)
    {
        case ANNOTID_LOCAL:      /* local */
            ObjectIdFree((ObjectIdPtr)pnt);
            break;
        case ANNOTID_NCBI:      /* ncbi */
			break;
        case ANNOTID_GENERAL:      /* general */
            DbtagFree((DbtagPtr)pnt);
            break;
    }
	return (AnnotIdPtr)MemFree(anp);
}

/*****************************************************************************
*
*   AnnotIdAsnWrite(anp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (AnnotId ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL AnnotIdAsnWrite (AnnotIdPtr anp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return retval;
	}

	if (aip == NULL)
		return retval;

	atp = AsnLinkType(orig, ANNOT_ID);   /* link local tree */
    if (atp == NULL)
        return retval;

	if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

	av.ptrvalue = (Pointer)anp;
    if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av))
        goto erret;

    switch (anp->choice)
    {
        case ANNOTID_LOCAL:      /* local */
			retval = ObjectIdAsnWrite((ObjectIdPtr)(anp->data.ptrvalue), aip, ANNOT_ID_local);
            break;
        case ANNOTID_NCBI:      /* ncbi */
			av.intvalue = anp->data.intvalue;
            retval = AsnWrite(aip, ANNOT_ID_ncbi, &av);
            break;
        case ANNOTID_GENERAL:      /* general */
		if (aip->spec_version == 3 )    /* ASN3 strip new value */
		{
			ErrPostEx(SEV_ERROR,0,0,"ASN3: AnnotIdGeneral stripped");
		}
		else
			retval = DbtagAsnWrite((DbtagPtr)(anp->data.ptrvalue), aip, ANNOT_ID_general);
            break;
    }
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   AnnotIdAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN AnnotIdPtr LIBCALL AnnotIdAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    AnnotIdPtr anp=NULL;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return anp;
	}

	if (aip == NULL)
		return anp;

	if (orig == NULL)           /* AnnotId ::= (self contained) */
		atp = AsnReadId(aip, amp, ANNOT_ID);
	else
		atp = AsnLinkType(orig, ANNOT_ID);    /* link in local tree */
    if (atp == NULL)
        return anp;

	anp = ValNodeNew(NULL);
    if (anp == NULL)
        goto erret;

    if ( AsnReadVal(aip, atp, &av) <= 0)
        goto erret;    /* read the CHOICE value (nothing) */
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)
        goto erret;  /* find the choice */

	if (atp == ANNOT_ID_local)
    {
        anp->choice = ANNOTID_LOCAL;
		anp->data.ptrvalue = (Pointer)ObjectIdAsnRead(aip, atp);
		if (anp->data.ptrvalue == NULL) goto erret;
    }
    else if (atp == ANNOT_ID_ncbi)
    {
        anp->choice = ANNOTID_NCBI;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        anp->data.intvalue = av.intvalue;
    }
	else if (atp == ANNOT_ID_general)
    {
        anp->choice = ANNOTID_GENERAL;
		anp->data.ptrvalue = (Pointer)DbtagAsnRead(aip, atp);
		if (anp->data.ptrvalue == NULL) goto erret;
    }
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return anp;
erret:
    anp = AnnotIdFree(anp);
    goto ret;
}

/*****************************************************************************
*
*   AnnotIdSetFree()
*       frees a chain of AnnotId's
*
*****************************************************************************/
NLM_EXTERN AnnotIdPtr LIBCALL AnnotIdSetFree (AnnotIdPtr anp)
{
    AnnotIdPtr next;

    while (anp != NULL)
    {
        next = anp->next;
        AnnotIdFree(anp);
        anp = next;
    }
    return anp;
}

/*****************************************************************************
*
*   AnnotIdSetAsnRead(aip, settype, elementtype)
*       read a set/seq of AnnotId's
*
*****************************************************************************/
NLM_EXTERN AnnotIdPtr LIBCALL AnnotIdSetAsnRead (AsnIoPtr aip, AsnTypePtr settype, AsnTypePtr elementtype)
{
    DataVal av;
    AsnTypePtr atp, atp2;
    AnnotIdPtr first = NULL, curr = NULL, anp;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return first;
	}

    atp2 = AsnLinkType(elementtype, ANNOT_ID);
    if (atp2 == NULL)
        return first;

    if (AsnReadVal(aip, settype, &av) <= 0)   /* start struct */
        goto erret;

    atp = settype;
    while ((atp = AsnReadId(aip, amp, atp)) != settype)
    {
        if (atp == NULL)
            goto erret;
        anp = AnnotIdAsnRead(aip, atp2);
        if (anp == NULL)
            goto erret;

		if ((aip->spec_version == 3 ) &&   /* ASN3 strip new value */
			(anp->choice == ANNOTID_GENERAL))
		{
			ErrPostEx(SEV_ERROR,0,0,"ASN3: AnnotIdGeneral stripped");
			AnnotIdFree(anp);
		}
		else
        {
        if (first == NULL)
            first = anp;
        else
            curr->next = anp;
        curr = anp;
	    }
    }
    if (AsnReadVal(aip, atp, &av) <= 0)    /* end struct */
        goto erret;
	if (first == NULL)
		ErrPostEx(SEV_WARNING, 0,0, "Empty SET OF Annot-id. line %ld", (long) aip->linenumber);
ret:
    AsnUnlinkType(elementtype);
    return first;
erret:
    first = AnnotIdSetFree(first);
    goto ret;
}

/*****************************************************************************
*
*   AnnotIdSetAsnWrite(anp, aip, settype, elementtype)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL AnnotIdSetAsnWrite (AnnotIdPtr anp, AsnIoPtr aip, AsnTypePtr settype, AsnTypePtr elementtype)
{
    AsnTypePtr atp2;
	AnnotIdPtr oldanp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return retval;
	}

    if (aip == NULL)
        return retval;

    atp2 = AsnLinkType(elementtype, ANNOT_ID);
    if (atp2 == NULL)
        return retval;

	if (anp == NULL) { AsnNullValueMsg(aip, settype); goto erret; }

	oldanp = anp;
    if (! AsnOpenStruct(aip, settype, (Pointer)oldanp))
        goto erret;
    while (anp != NULL)
    {
        if (! AnnotIdAsnWrite(anp, aip, atp2))
            goto erret;
        anp = anp->next;
    }
    if (! AsnCloseStruct(aip, settype, (Pointer)oldanp))
        goto erret;
    retval = TRUE;
erret:
    AsnUnlinkType(elementtype);
    return retval;
}

/*****************************************************************************
*
*   AnnotDescrFree(anp)
*       Frees a chain of AnnotDescr and associated data
*
*****************************************************************************/
NLM_EXTERN AnnotDescrPtr LIBCALL AnnotDescrFree (AnnotDescrPtr anp)
{
    AnnotDescrPtr next;

    while (anp != NULL)
    {
        next = anp->next;
		AnnotDescFree(anp);
        anp = next;
    }
	return anp;
}

/*****************************************************************************
*
*   AnnotDescrFree(anp)
*       Frees a chain of AnnotDescr and associated data
*
*****************************************************************************/
NLM_EXTERN AnnotDescPtr LIBCALL AnnotDescFree (AnnotDescPtr anp)
{
    Pointer pnt;

	if (anp == NULL) return anp;

    pnt = anp->data.ptrvalue;
    switch (anp->choice)
    {
        case Annot_descr_name:      /* name */
        case Annot_descr_title:      /* title */
        case Annot_descr_comment:      /* comment */
            MemFree(pnt);
            break;
		case Annot_descr_pub:      /* pub */
			PubdescFree((PubdescPtr)pnt);
			break;
        case Annot_descr_user:     /* user */
            UserObjectFree((UserObjectPtr)pnt);
            break;
		case Annot_descr_create_date:     /* create-date */
		case Annot_descr_update_date:     /* update-date */
			DateFree((DatePtr)pnt);
			break;
        case Annot_descr_src:     /* src */
            SeqIdFree((SeqIdPtr)pnt);
            break;
        case Annot_descr_align:     /* align */
            AlignDefFree((AlignDefPtr)pnt);
            break;
		case Annot_descr_region:  /* region */
			SeqLocFree((SeqLocPtr)pnt);
			break;
    }
    anp = (AnnotDescPtr)MemFree(anp);
	return anp;
}

/*****************************************************************************
*
*   AnnotDescrAsnWrite(anp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (AnnotDescr ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL AnnotDescrAsnWrite (ValNodePtr anp, AsnIoPtr aip, AsnTypePtr orig)
{
	AsnTypePtr atp;
    ValNodePtr oldanp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, ANNOT_DESCR);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

	oldanp = anp;
    if (! AsnOpenStruct(aip, atp, (Pointer)oldanp)) goto erret; /* SET OF */

    while (anp != NULL)
    {
		if (! AnnotDescAsnWrite (anp, aip, ANNOT_DESCR_E)) goto erret;
        anp = anp->next;
    }
    if (! AsnCloseStruct(aip, atp, oldanp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   AnnotDescAsnWrite(anp, aip, orig)
*   	Writes one Seqdesc
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL AnnotDescAsnWrite (AnnotDescPtr anp, AsnIoPtr aip, AsnTypePtr orig)
{
    DataValPtr dvp;
	AsnTypePtr atp;
    Pointer pnt;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, ANNOTDESC);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

   dvp = &anp->data;
		
   if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, dvp)) goto erret;
   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
       case Annot_descr_name:                   /* name */
           if (! AsnWrite(aip, ANNOTDESC_name, dvp)) goto erret;
           break;
       case Annot_descr_title:                   /* title */
           if (! AsnWrite(aip, ANNOTDESC_title, dvp)) goto erret;
           break;
       case Annot_descr_comment:                   /* comment */
           if (! AsnWrite(aip, ANNOTDESC_comment, dvp)) goto erret;
           break;
       case Annot_descr_pub:                  /* pubdesc */
           if (! PubdescAsnWrite((PubdescPtr)pnt, aip, ANNOTDESC_pub)) goto erret;
           break;
       case Annot_descr_user:                   /* user */
           if (! UserObjectAsnWrite((UserObjectPtr)pnt, aip, ANNOTDESC_user)) goto erret;
           break;
	case Annot_descr_create_date:                   /* create-date */
		if (! DateAsnWrite((DatePtr)pnt, aip, ANNOTDESC_create_date)) goto erret;
		break;
	case Annot_descr_update_date:                   /* update-date */
		if (! DateAsnWrite((DatePtr)pnt, aip, ANNOTDESC_update_date)) goto erret;
		break;
       case Annot_descr_src:                   /* src */
           if (! SeqIdAsnWrite((SeqIdPtr)pnt, aip, ANNOTDESC_src)) goto erret;
           break;
       case Annot_descr_align:                   /* align */
		if (aip->spec_version == 3 )    /* ASN3 strip new value */
		{
			ErrPostEx(SEV_ERROR,0,0,"ASN3: AnnotDescrAlign stripped");
		}
		else
           if (! AlignDefAsnWrite((AlignDefPtr)pnt, aip, ANNOTDESC_align)) goto erret;
           break;
        case Annot_descr_region:                   /* region */
		  if ((aip->spec_version == 0) || (aip->spec_version >= 6))
		{
           if (! SeqLocAsnWrite((SeqLocPtr)pnt, aip, ANNOTDESC_region)) goto erret;
		}
		else
		{
			ErrPostEx(SEV_ERROR,0,0,"ASN6: AnnotDescrRegion stripped");
		}
		   break;
  }
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   AnnotDescrAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL AnnotDescrAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    ValNodePtr anp, first=NULL, prev=NULL;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return first;
	}

	if (aip == NULL)
		return first;

	if (! ProgMon("Read AnnotDescr"))
		return first;

	if (orig == NULL)           /* AnnotDescr ::= (self contained) */
		atp = AsnReadId(aip, amp, ANNOT_DESCR);
	else
		atp = AsnLinkType(orig, ANNOT_DESCR);    /* link in local tree */
    if (atp == NULL) return first;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the start struct */

    while ((atp = AsnReadId(aip, amp, atp)) == ANNOT_DESCR_E)
    {
		anp = AnnotDescAsnRead(aip, atp);
		if (anp == NULL) goto erret;

		if ((aip->spec_version == 3) && (anp->choice == Annot_descr_align))    /* ASN3 strip new value */
		{
			ErrPostEx(SEV_ERROR,0,0,"ASN3: AnnotDescrAlign stripped");
			AnnotDescFree(anp);
		}
		else
		{
        if (first == NULL)
            first = anp;
		if (prev != NULL)
			prev->next = anp;
		prev = anp;
		}
    }
    if (atp == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return first;
erret:
    first = AnnotDescrFree(first);
    goto ret;
}

/*****************************************************************************
*
*   AnnotDescAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN AnnotDescPtr LIBCALL AnnotDescAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    AnnotDescPtr anp=NULL;
    Uint1 choice;
    AsnReadFunc func;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return anp;
	}

	if (aip == NULL)
		return anp;

	if (orig == NULL)           /* AnnotDesc ::= (self contained) */
		atp = AsnReadId(aip, amp, ANNOTDESC);
	else
		atp = AsnLinkType(orig, ANNOTDESC);    /* link in local tree */
    if (atp == NULL) return anp;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the CHOICE value (nothing) */
	atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;  /* find the choice */

    func = NULL;
    choice = 0;
    if (atp == ANNOTDESC_name)
    {
        choice = Annot_descr_name;
    }
    else if (atp == ANNOTDESC_title)
    {
        choice = Annot_descr_title;
    }
    else if (atp == ANNOTDESC_comment)
    {
        choice = Annot_descr_comment;
    }
    else if (atp == ANNOTDESC_pub)
    {
        choice = Annot_descr_pub;
        func = (AsnReadFunc) PubdescAsnRead;
    }
    else if (atp == ANNOTDESC_user)
    {
        choice = Annot_descr_user;
        func = (AsnReadFunc) UserObjectAsnRead;
    }
	else if (atp == ANNOTDESC_create_date)
	{
		choice = Annot_descr_create_date;
		func = (AsnReadFunc) DateAsnRead;
	}
	else if (atp == ANNOTDESC_update_date)
	{
		choice = Annot_descr_update_date;
		func = (AsnReadFunc) DateAsnRead;
	}
    else if (atp == ANNOTDESC_src)
    {
        choice = Annot_descr_src;
        func = (AsnReadFunc) SeqIdAsnRead;
    }
    else if (atp == ANNOTDESC_align)
    {
        choice = Annot_descr_align;
        func = (AsnReadFunc) AlignDefAsnRead;
    }
    else if (atp == ANNOTDESC_region)
    {
        choice = Annot_descr_region;
        func = (AsnReadFunc) SeqLocAsnRead;
    }

    anp = ValNodeNew(NULL);
    if (anp == NULL) goto erret;
    anp->choice = choice;
    if (func != NULL)
    {
        anp->data.ptrvalue = (* func)(aip, atp);
        if (anp->data.ptrvalue == NULL) goto erret;
    }
    else
    {
        if (AsnReadVal(aip, atp, &anp->data) <= 0) goto erret;
    }

ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return anp;
erret:
    anp = AnnotDescFree(anp);
    goto ret;
}

/*****************************************************************************
*
*   AlignDefNew()
*
*****************************************************************************/
NLM_EXTERN AlignDefPtr LIBCALL AlignDefNew (void)
{
	return (AlignDefPtr)MemNew(sizeof(AlignDef));
}

/*****************************************************************************
*
*   AlignDefFree(adp)
*       Frees one AlignDef and associated data
*
*****************************************************************************/
NLM_EXTERN AlignDefPtr LIBCALL AlignDefFree (AlignDefPtr adp)
{
    if (adp == NULL)
        return adp;
    SeqIdSetFree(adp->ids);
	return (AlignDefPtr)MemFree(adp);
}

/*****************************************************************************
*
*   AlignDefAsnWrite(adp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (AlignDef ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL AlignDefAsnWrite (AlignDefPtr adp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return retval;
	}

	if (aip == NULL)
		return retval;

	atp = AsnLinkType(orig, ALIGN_DEF);   /* link local tree */
    if (atp == NULL)
        return retval;

	if (adp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)adp))
        goto erret;

	av.intvalue = (Int4)(adp->align_type);
    retval = AsnWrite(aip, ALIGN_DEF_align_type, &av);
	if (! retval) goto erret;

	if (adp->ids != NULL)
	{
		retval = SeqIdSetAsnWrite(adp->ids, aip, ALIGN_DEF_ids, ALIGN_DEF_ids_E);
		if (! retval) goto erret;
	}

	retval = AsnCloseStruct(aip, atp, (Pointer)adp);

erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   AlignDefAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN AlignDefPtr LIBCALL AlignDefAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldatp;
    AlignDefPtr adp=NULL;

	if (! loaded)
	{
		if (! SeqAsnLoad())
			return adp;
	}

	if (aip == NULL)
		return adp;

	if (orig == NULL)           /* AlignDef ::= (self contained) */
		atp = AsnReadId(aip, amp, ALIGN_DEF);
	else
		atp = AsnLinkType(orig, ALIGN_DEF);    /* link in local tree */
    if (atp == NULL)
        return adp;
	oldatp = atp;

	adp = AlignDefNew();
    if (adp == NULL)
        goto erret;

    if ( AsnReadVal(aip, atp, &av) <= 0)
        goto erret;    /* read the open struct */

    while ((atp = AsnReadId(aip, amp, atp)) != oldatp)
	{
	   if (atp == NULL) goto erret;
	   if (atp == ALIGN_DEF_align_type)
		{
			if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
			adp->align_type = (Uint1)(av.intvalue);
		}
		else if (atp == ALIGN_DEF_ids)
		{
			adp->ids = SeqIdSetAsnRead(aip, atp, ALIGN_DEF_ids_E);
		}
	}

    if ( AsnReadVal(aip, atp, &av) <= 0)
        goto erret;    /* read the close struct */

ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return adp;
erret:
    adp = AlignDefFree(adp);
    goto ret;
}


