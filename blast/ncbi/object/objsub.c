/*  objsub.c
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
* File Name:  objsub.c
*
* Author:  James Ostell
*   
* Version Creation Date: 1/1/91
*
* $Revision: 6.3 $
*
* File Description:  Object manager for module NCBI-Submit
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 05-13-93 Schuler     All public functions are now declared LIBCALL.
*
*
* $Log: objsub.c,v $
* Revision 6.3  2004/05/12 20:41:57  kans
* set aip->io_failure in several erret blocks for compatibility of old object loaders with new ones
*
* Revision 6.2  2004/04/01 13:43:08  lavr
* Spell "occurred", "occurrence", and "occurring"
*
* Revision 6.1  1998/08/24 18:28:12  kans
* removed solaris -v -fd warnings
*
* Revision 6.0  1997/08/25 18:51:01  madden
* Revision changed to 6.0
*
* Revision 4.1  1997/06/19 18:42:11  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.0  1995/07/26 13:48:06  ostell
* force revision to 4.0
*
 * Revision 3.9  1995/06/01  02:50:31  ostell
 * corrected minor typo
 *
 * Revision 3.8  1995/05/30  15:52:32  ostell
 * removed unused local variables
 *
 * Revision 3.7  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/
#include <asnsubmt.h>        /* the AsnTool header */
#include <objsub.h>		   /* the general objects interface */
#include <objmgr.h>

static Boolean loaded = FALSE;

/*****************************************************************************
*
*   SeqSubmit ObjMgr Routines
*
*****************************************************************************/
static CharPtr seqsubtypename = "SeqSubmit";

static Pointer LIBCALLBACK SeqSubmitNewFunc (void)
{
	return (Pointer) SeqSubmitNew();
}

static Pointer LIBCALLBACK SeqSubmitFreeFunc (Pointer data)
{
	return (Pointer) SeqSubmitFree ((SeqSubmitPtr) data);
}

static Boolean LIBCALLBACK SeqSubmitAsnWriteFunc (Pointer data, AsnIoPtr aip, AsnTypePtr atp)
{
	return SeqSubmitAsnWrite((SeqSubmitPtr)data, aip, atp);
}

static Pointer LIBCALLBACK SeqSubmitAsnReadFunc (AsnIoPtr aip, AsnTypePtr atp)
{
	return (Pointer) SeqSubmitAsnRead (aip, atp);
}

static Int2 LIBCALLBACK SeqSubmitLabelFunc ( Pointer data, CharPtr buffer, Int2 buflen, Uint1 content)
{
	return SeqSubmitLabel((SeqSubmitPtr)data, buffer, buflen, content);
}

NLM_EXTERN Int2 LIBCALL SeqSubmitLabel (SeqSubmitPtr ssp, CharPtr buffer, Int2 buflen, Uint1 content)
{
	static CharPtr seqsubtypes[4] = {
		"Not Set",
		"Entries",
		"Annotations",
		"Deletions"};
	Int2 i = 0, len, diff;

	if ((ssp == NULL) || (buflen < 1))
		return 0;

	len = buflen;

	if ((ssp->datatype >= 1) && (ssp->datatype <= 3))
		i = (Int2)(ssp->datatype);

	if (content == OM_LABEL_TYPE)
		return LabelCopy(buffer, seqsubtypes[i], buflen);

	if (content != OM_LABEL_CONTENT)
	{
		diff = LabelCopyExtra(buffer, seqsubtypes[i], buflen, NULL, ": ");
		buflen -= diff;
		buffer += diff;
	}

	if (ssp->sub != NULL)
	{
		if (ssp->sub->contact != NULL)
			diff = ContactInfoLabel(ssp->sub->contact, buffer, buflen, OM_LABEL_CONTENT);
		else
			diff = SubmitBlockLabel(ssp->sub, buffer, buflen, OM_LABEL_CONTENT);
		buflen -= diff;
		buffer += diff;
	}

	return (len - buflen);   /* no special SUMMARY yet */
}

/*****************************************************************************
*
*   SubmitBlock ObjMgr Routines
*
*****************************************************************************/
static CharPtr subblocktypename = "SubmitBlock";

static Pointer LIBCALLBACK SubmitBlockNewFunc (void)
{
	return (Pointer) SubmitBlockNew();
}

static Pointer LIBCALLBACK SubmitBlockFreeFunc (Pointer data)
{
	return (Pointer) SubmitBlockFree ((SubmitBlockPtr) data);
}

static Boolean LIBCALLBACK SubmitBlockAsnWriteFunc (Pointer data, AsnIoPtr aip, AsnTypePtr atp)
{
	return SubmitBlockAsnWrite((SubmitBlockPtr)data, aip, atp);
}

static Pointer LIBCALLBACK SubmitBlockAsnReadFunc (AsnIoPtr aip, AsnTypePtr atp)
{
	return (Pointer) SubmitBlockAsnRead (aip, atp);
}

static Int2 LIBCALLBACK SubmitBlockLabelFunc ( Pointer data, CharPtr buffer, Int2 buflen, Uint1 content)
{
	return SubmitBlockLabel((SubmitBlockPtr)data, buffer, buflen, content);
}

NLM_EXTERN Int2 LIBCALL SubmitBlockLabel (SubmitBlockPtr sbp, CharPtr buffer, Int2 buflen, Uint1 content)
{
	Int2 len, diff;
	CharPtr the_type=NULL;
	Char tbuf[40];

	if ((sbp == NULL) || (buflen < 1))
		return 0;

	len = buflen;
	the_type = AsnEnumTypeStr(SUBMIT_BLOCK_subtype, (Int2)(sbp->subtype));
	if (the_type == NULL)
		the_type = "TypeNotSet";

	if (content == OM_LABEL_TYPE)
		return LabelCopy(buffer, the_type, buflen);

	if (content != OM_LABEL_CONTENT)
	{
		diff = LabelCopyExtra(buffer, the_type, buflen, NULL, ": ");
		buflen -= diff;
		buffer += diff;
	}
    
    if (sbp->hup)
    {
    	diff = LabelCopy(buffer, " HUP",buflen);
    	buflen -= diff;
    	buffer += diff;
    }
    
    if (sbp->reldate)
    {
    	DatePrint(sbp->reldate, tbuf);
    	diff = LabelCopyExtra(buffer, tbuf,buflen," Rel:",NULL);
    	buflen -= diff;
    	buffer += diff;
    }
    
    if (sbp->tool != NULL)
    {
    	diff = LabelCopyExtra(buffer, sbp->tool,buflen," ",NULL);
    	buflen -= diff;
    	buffer += diff;
    }
   
    if (sbp->user_tag != NULL)
    {
    	diff = LabelCopyExtra(buffer, sbp->user_tag,buflen,"[","]");
    	buflen -= diff;
    	buffer += diff;
    }
   
    if (sbp->comment != NULL)
    {
    	diff = LabelCopyExtra(buffer, sbp->comment,buflen," ",NULL);
    	buflen -= diff;
    	buffer += diff;
    }
      
    
	return (len - buflen);   /* no special SUMMARY yet */

}

/*****************************************************************************
*
*   ContactInfo ObjMgr Routines
*
*****************************************************************************/
static CharPtr contactinfotypename = "Contact Info";

static Pointer LIBCALLBACK ContactInfoNewFunc (void)
{
	return (Pointer) ContactInfoNew();
}

static Pointer LIBCALLBACK ContactInfoFreeFunc (Pointer data)
{
	return (Pointer) ContactInfoFree ((ContactInfoPtr) data);
}

static Boolean LIBCALLBACK ContactInfoAsnWriteFunc (Pointer data, AsnIoPtr aip, AsnTypePtr atp)
{
	return ContactInfoAsnWrite((ContactInfoPtr)data, aip, atp);
}

static Pointer LIBCALLBACK ContactInfoAsnReadFunc (AsnIoPtr aip, AsnTypePtr atp)
{
	return (Pointer) ContactInfoAsnRead (aip, atp);
}

static Int2 LIBCALLBACK ContactInfoLabelFunc ( Pointer data, CharPtr buffer, Int2 buflen, Uint1 content)
{
	return ContactInfoLabel((ContactInfoPtr)data, buffer, buflen, content);
}

NLM_EXTERN Int2 LIBCALL ContactInfoLabel (ContactInfoPtr cip, CharPtr buffer, Int2 buflen, Uint1 content)
{
	Int2 len, diff;
	CharPtr the_contact=NULL;

	if ((cip == NULL) || (buflen < 1))
		return 0;

	len = buflen;

	if (content == OM_LABEL_TYPE)
		return LabelCopy(buffer, contactinfotypename, buflen);

	if (content != OM_LABEL_CONTENT)
	{
		diff = LabelCopyExtra(buffer, contactinfotypename, buflen, NULL, ": ");
		buflen -= diff;
		buffer += diff;
	}

	if (cip->contact != NULL)
	{
		diff = PersonIdLabel(cip->contact->name, buffer, buflen, PIDLABEL_GENBANK);
	}
	else
	{
		if (cip->name != NULL)
			the_contact = cip->name;
		else if (cip->last_name != NULL)
			the_contact = cip->last_name;
		else
			the_contact = "NotSet";
		
		diff = LabelCopy(buffer, the_contact, buflen);
	}

	buflen -= diff;
	buffer += diff;

	return (len - buflen);   /* no special SUMMARY yet */

}

/*****************************************************************************
*
*   SubmitAsnLoad()
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SubmitAsnLoad (void)
{
    if (loaded)
        return TRUE;

	if (! SeqSetAsnLoad ())
		return FALSE;
		
    if (AsnLoad())
        loaded = TRUE;

	ObjMgrTypeLoad(OBJ_SEQSUB, "Seq-submit", seqsubtypename, "Sequence Data Submission",
		SEQ_SUBMIT, SeqSubmitNewFunc, SeqSubmitAsnReadFunc, SeqSubmitAsnWriteFunc,
		SeqSubmitFreeFunc, SeqSubmitLabelFunc, NULL);

	ObjMgrTypeLoad(OBJ_SUBMIT_BLOCK, "Submit-block", subblocktypename, "Data Submission Block",
		SUBMIT_BLOCK, SubmitBlockNewFunc, SubmitBlockAsnReadFunc, SubmitBlockAsnWriteFunc,
		SubmitBlockFreeFunc, SubmitBlockLabelFunc, NULL);

	ObjMgrTypeLoad(OBJ_SEQSUB_CONTACT, "Contact-info", contactinfotypename, "Contact Info",
		CONTACT_INFO, ContactInfoNewFunc, ContactInfoAsnReadFunc, ContactInfoAsnWriteFunc,
		ContactInfoFreeFunc, ContactInfoLabelFunc, NULL);

    return loaded;
}

/*****************************************************************************
*
*   SeqSubmit Routines
*
*****************************************************************************/


/*****************************************************************************
*
*   SeqSubmitNew()
*
*****************************************************************************/
NLM_EXTERN SeqSubmitPtr LIBCALL SeqSubmitNew (void)
{
	SeqSubmitPtr ssp;
	
	ssp = (SeqSubmitPtr)MemNew(sizeof(SeqSubmit));
	ObjMgrAdd (OBJ_SEQSUB, (Pointer)ssp);   /* add to objmgr list */
	return ssp;
}
/*****************************************************************************
*
*   SeqSubmitFree(ssp)
*
*****************************************************************************/
NLM_EXTERN SeqSubmitPtr LIBCALL SeqSubmitFree (SeqSubmitPtr ssp)
{
	SeqEntryPtr sep, sepnext;
	SeqAnnotPtr sap, sapnext;

	if (ssp == NULL)
		return ssp;

	SubmitBlockFree(ssp->sub);
	switch	(ssp->datatype)
	{
		case 1:
			sep = (SeqEntryPtr)ssp->data;
			while (sep != NULL)
			{
				sepnext = sep->next;
				SeqEntryFree(sep);
				sep = sepnext;
			}
			break;
		case 2:
			sap = (SeqAnnotPtr)ssp->data;
			while (sap != NULL)
			{
				sapnext = sap->next;
				SeqAnnotFree(sap);
				sap = sapnext;
			}
			break;
		case 3:
			SeqIdSetFree((SeqIdPtr)ssp->data);
			break;
	}
	
	if (! ObjMgrDelete(OBJ_SEQSUB, (Pointer)ssp))
	    ErrPostEx(SEV_ERROR, 0,0, "SeqSubmitFree: pointer not registered");

	return (SeqSubmitPtr)MemFree(ssp);
}

/*****************************************************************************
*
*   SeqSubmitAsnWrite(ssp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqSubmit ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqSubmitAsnWrite (SeqSubmitPtr ssp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;
	SeqEntryPtr sep;
	SeqAnnotPtr sap;

	if (! loaded)
	{
		if (! SubmitAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, SEQ_SUBMIT);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (ssp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)ssp))
        goto erret;

	if (! SubmitBlockAsnWrite(ssp->sub, aip, SEQ_SUBMIT_sub)) goto erret;

	av.ptrvalue = ssp->data;
    if (! AsnWriteChoice(aip, SEQ_SUBMIT_data, (Int2)ssp->datatype, &av)) goto erret;

	switch(ssp->datatype)
	{
		case 1:
			if (! AsnOpenStruct(aip, SEQ_SUBMIT_data_entrys, ssp->data)) goto erret;
			sep = (SeqEntryPtr) ssp->data;
			while (sep != NULL)
			{
				if (! SeqEntryAsnWrite(sep, aip, SEQ_SUBMIT_data_entrys_E)) goto erret;
				sep = sep->next;
			}
			if (! AsnCloseStruct(aip, SEQ_SUBMIT_data_entrys, ssp->data)) goto erret;
			break;
		case 2:
			if (! AsnOpenStruct(aip, SEQ_SUBMIT_data_annots, ssp->data)) goto erret;
			sap = (SeqAnnotPtr) ssp->data;
			while (sap != NULL)
			{
				if (! SeqAnnotAsnWrite(sap, aip, SEQ_SUBMIT_data_annots_E)) goto erret;
				sap = sap->next;
			}
			if (! AsnCloseStruct(aip, SEQ_SUBMIT_data_annots, ssp->data)) goto erret;
			break;
		case 3:
			if (! SeqIdSetAsnWrite((SeqIdPtr)ssp->data, aip, SEQ_SUBMIT_data_delete,
				SEQ_SUBMIT_data_delete_E)) goto erret;
			break;
		default:
			ErrPost(CTX_NCBIOBJ, 1, "Unknown Seq-submit type = %d", (int) ssp->datatype);
			goto erret;
	}
    if (! AsnCloseStruct(aip, atp, (Pointer)ssp))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqSubmitAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN SeqSubmitPtr LIBCALL SeqSubmitAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldatp;
    SeqSubmitPtr ssp=NULL;
	SeqEntryPtr sep, seplast = NULL;
	SeqAnnotPtr sap, saplast = NULL;
	SeqIdPtr sip, siplast = NULL;

	if (! loaded)
	{
		if (! SubmitAsnLoad())
			return ssp;
	}

	if (aip == NULL)
		return ssp;

	if (orig == NULL)           /* SeqSubmit ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_SUBMIT);
	else
		atp = AsnLinkType(orig, SEQ_SUBMIT);    /* link in local tree */
    if (atp == NULL)
        return ssp;

	ssp = SeqSubmitNew();
    if (ssp == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0)    /* read the start struct */
        goto erret;

	atp = AsnReadId(aip, amp, atp);  /* find the submission-block */
    if (atp == NULL) goto erret;
	ssp->sub = SubmitBlockAsnRead(aip, atp);
	if (ssp->sub == NULL) goto erret;

    atp = AsnReadId(aip, amp, atp);  /* read the CHOICE */
    if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;

    atp = AsnReadId(aip, amp, atp);  /* read the data */
    if (atp == NULL) goto erret;
	oldatp = atp;     /* the SET OF */
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;

    while ((atp = AsnReadId(aip, amp, atp)) != oldatp)
    {
		if (atp == SEQ_SUBMIT_data_entrys_E)
		{
			sep = SeqEntryAsnRead(aip, atp);
			if (sep == NULL) goto erret;
			if (IS_Bioseq(sep))
				ObjMgrConnect(OBJ_BIOSEQ, sep->data.ptrvalue,
					OBJ_SEQSUB, (Pointer) ssp);
			else
				ObjMgrConnect(OBJ_BIOSEQSET, sep->data.ptrvalue,
					OBJ_SEQSUB, (Pointer) ssp);

			if (seplast == NULL)
			{
				ssp->data = (Pointer) sep;
				ssp->datatype = 1;
			}
			else
				seplast->next = sep;
			seplast = sep;
		}
		else if (atp == SEQ_SUBMIT_data_annots_E)
		{
			sap = SeqAnnotAsnRead(aip, atp);
			if (sap == NULL) goto erret;
			if (saplast == NULL)
			{
				ssp->data = (Pointer) sap;
				ssp->datatype = 2;
			}
			else
				saplast->next = sap;
			saplast = sap;
		}
		else if (atp == SEQ_SUBMIT_data_delete_E)
		{
			sip = SeqIdAsnRead(aip, atp);
			if (sip == NULL) goto erret;
			if (siplast == NULL)
			{
				ssp->data = (Pointer) sip;
				ssp->datatype = 3;
			}
			else
				siplast->next = sip;
			siplast = sip;
		}
		else
			goto erret;
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end set of */

    atp = AsnReadId(aip, amp, atp);
    if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;  /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return ssp;
erret:
    aip->io_failure = TRUE;
    ssp = SeqSubmitFree(ssp);
    goto ret;
}

/*****************************************************************************
*
*   SubmitBlockNew()
*
*****************************************************************************/
NLM_EXTERN SubmitBlockPtr LIBCALL SubmitBlockNew (void)
{
	return (SubmitBlockPtr)MemNew(sizeof(SubmitBlock));
}

/*****************************************************************************
*
*   SubmitBlockFree(sbp)
*
*****************************************************************************/
NLM_EXTERN SubmitBlockPtr LIBCALL SubmitBlockFree (SubmitBlockPtr sbp)
{
	if (sbp == NULL)
		return sbp;
	ContactInfoFree(sbp->contact);
	CitSubFree(sbp->cit);
	DateFree(sbp->reldate);
	MemFree(sbp->tool);
	MemFree(sbp->user_tag);
	MemFree(sbp->comment);

	ObjMgrDelete(OBJ_SUBMIT_BLOCK, (Pointer)sbp);

	return (SubmitBlockPtr)MemFree(sbp);
}

/*****************************************************************************
*
*   SubmitBlockAsnWrite(sbp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SubmitBlock ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SubmitBlockAsnWrite (SubmitBlockPtr sbp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SubmitAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, SUBMIT_BLOCK);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (sbp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)sbp))
        goto erret;

	if (! ContactInfoAsnWrite(sbp->contact, aip, SUBMIT_BLOCK_contact))
		goto erret;

	if (! CitSubAsnWrite(sbp->cit, aip, SUBMIT_BLOCK_cit)) goto erret;

	if (sbp->hup)
	{
		av.boolvalue = sbp->hup;
		if (! AsnWrite(aip, SUBMIT_BLOCK_hup, &av)) goto erret;
	}

	if (sbp->reldate != NULL)
	{
		if (! DateAsnWrite(sbp->reldate, aip, SUBMIT_BLOCK_reldate))
			goto erret;
	}

	if (sbp->subtype)
	{
		av.intvalue = (Int4)sbp->subtype;
		if (! AsnWrite(aip, SUBMIT_BLOCK_subtype, &av)) goto erret;
	}

	if (sbp->tool != NULL)
	{
		av.ptrvalue = sbp->tool;
		if (! AsnWrite(aip, SUBMIT_BLOCK_tool, &av)) goto erret;
	}

	if (sbp->user_tag != NULL)
	{
		av.ptrvalue = sbp->user_tag;
		if (! AsnWrite(aip, SUBMIT_BLOCK_user_tag, &av)) goto erret;
	}

	if (sbp->comment != NULL)
	{
		av.ptrvalue = sbp->comment;
		if (! AsnWrite(aip, SUBMIT_BLOCK_comment, &av)) goto erret;
	}


    if (! AsnCloseStruct(aip, atp, (Pointer)sbp))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SubmitBlockAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN SubmitBlockPtr LIBCALL SubmitBlockAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldatp;
    SubmitBlockPtr sbp=NULL;

	if (! loaded)
	{
		if (! SubmitAsnLoad())
			return sbp;
	}

	if (aip == NULL)
		return sbp;

	if (orig == NULL)           /* SubmitBlock ::= (self contained) */
		atp = AsnReadId(aip, amp, SUBMIT_BLOCK);
	else
		atp = AsnLinkType(orig, SUBMIT_BLOCK);    /* link in local tree */
    if (atp == NULL)
        return sbp;

	oldatp = atp;
	sbp = SubmitBlockNew();
    if (sbp == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0)    /* read the start struct */
        goto erret;

	while ((atp = AsnReadId(aip, amp, atp)) != oldatp)
	{
		if (atp == NULL) goto erret;
		if (atp == SUBMIT_BLOCK_contact)
		{
			sbp->contact = ContactInfoAsnRead(aip, atp);
			if (sbp->contact == NULL) goto erret;
		}
		else if (atp == SUBMIT_BLOCK_cit)
		{
			sbp->cit = CitSubAsnRead(aip, atp);
			if (sbp->cit == NULL) goto erret;
		}
		else if (atp == SUBMIT_BLOCK_reldate)
		{
			sbp->reldate = DateAsnRead(aip, atp);
			if (sbp->reldate == NULL) goto erret;
		}
		else
		{
			if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
			if (atp == SUBMIT_BLOCK_hup)
				sbp->hup = av.boolvalue;
			else if (atp == SUBMIT_BLOCK_subtype)
				sbp->subtype = (Uint1)av.intvalue;
			else if (atp == SUBMIT_BLOCK_tool)
				sbp->tool = (CharPtr)av.ptrvalue;
			else if (atp == SUBMIT_BLOCK_user_tag)
				sbp->user_tag = (CharPtr)av.ptrvalue;
			else if (atp == SUBMIT_BLOCK_comment)
				sbp->comment = (CharPtr)av.ptrvalue;

		}
	}

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;  /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return sbp;
erret:
    aip->io_failure = TRUE;
    sbp = SubmitBlockFree(sbp);
    goto ret;
}

/*****************************************************************************
*
*   ContactInfoNew()
*
*****************************************************************************/
NLM_EXTERN ContactInfoPtr LIBCALL ContactInfoNew (void)
{
	return (ContactInfoPtr)MemNew(sizeof(ContactInfo));
}

/*****************************************************************************
*
*   ContactInfoFree(cip)
*
*****************************************************************************/
NLM_EXTERN ContactInfoPtr LIBCALL ContactInfoFree (ContactInfoPtr cip)
{
	if (cip == NULL)
		return cip;
	MemFree(cip->name);
	ValNodeFreeData(cip->address);
	MemFree(cip->phone);
	MemFree(cip->fax);
	MemFree(cip->email);
	MemFree(cip->telex);
	ObjectIdFree(cip->owner_id);
	BSFree(cip->password);
        MemFree(cip->last_name);
        MemFree(cip->first_name);
        MemFree(cip->middle_initial);
	AuthorFree(cip->contact);

	ObjMgrDelete(OBJ_SEQSUB_CONTACT, (Pointer)cip);

	return (ContactInfoPtr)MemFree(cip);
}

/*****************************************************************************
*
*   ContactInfoAsnWrite(cip, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (ContactInfo ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ContactInfoAsnWrite (ContactInfoPtr cip, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;
	ValNodePtr vnp;

	if (! loaded)
	{
		if (! SubmitAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, CONTACT_INFO);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (cip == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)cip))
        goto erret;

	if (cip->name != NULL)
	{
		av.ptrvalue = cip->name;
		if (! AsnWrite(aip, CONTACT_INFO_name, &av)) goto erret;
	}

	if (cip->address != NULL)
	{
		vnp = cip->address;
   	 if (! AsnOpenStruct(aip, CONTACT_INFO_address, (Pointer)vnp))
      	  goto erret;
		while (vnp != NULL)
		{
			if (! AsnWrite(aip, CONTACT_INFO_address_E, &vnp->data)) goto erret;
			vnp = vnp->next;
		}
	    if (! AsnCloseStruct(aip, CONTACT_INFO_address, (Pointer)vnp))
   	     goto erret;
	}

	if (cip->phone != NULL)
	{
		av.ptrvalue = cip->phone;
		if (! AsnWrite(aip, CONTACT_INFO_phone, &av)) goto erret;
	}

	if (cip->fax != NULL)
	{
		av.ptrvalue = cip->fax;
		if (! AsnWrite(aip, CONTACT_INFO_fax, &av)) goto erret;
	}

	if (cip->email != NULL)
	{
		av.ptrvalue = cip->email;
		if (! AsnWrite(aip, CONTACT_INFO_email, &av)) goto erret;
	}

	if (cip->telex != NULL)
	{
		av.ptrvalue = cip->telex;
		if (! AsnWrite(aip, CONTACT_INFO_telex, &av)) goto erret;
	}

	if (cip->owner_id != NULL)
	{
		if (! ObjectIdAsnWrite(cip->owner_id, aip, CONTACT_INFO_owner_id)) goto erret;
	}

	if (cip->password != NULL)
	{
		av.ptrvalue = cip->password;
		if (! AsnWrite(aip, CONTACT_INFO_password, &av)) goto erret;
	}

	if (cip->last_name != NULL)
	{
		av.ptrvalue = cip->last_name;
		if (! AsnWrite(aip, CONTACT_INFO_last_name, &av)) goto erret;
	}

	if (cip->first_name != NULL)
	{
		av.ptrvalue = cip->first_name;
		if (! AsnWrite(aip, CONTACT_INFO_first_name, &av)) goto erret;
	}

	if (cip->middle_initial != NULL)
	{
		av.ptrvalue = cip->middle_initial;
		if (! AsnWrite(aip, CONTACT_INFO_middle_initial, &av)) goto erret;
	}

	if (cip->contact != NULL)
	{
		if (! AuthorAsnWrite(cip->contact, aip, CONTACT_INFO_contact))
			goto erret;
	}
	
    if (! AsnCloseStruct(aip, atp, (Pointer)cip))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   ContactInfoAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN ContactInfoPtr LIBCALL ContactInfoAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldatp;
    ContactInfoPtr cip=NULL;
	ValNodePtr vnp = NULL;

	if (! loaded)
	{
		if (! SubmitAsnLoad())
			return cip;
	}

	if (aip == NULL)
		return cip;

	if (orig == NULL)           /* ContactInfo ::= (self contained) */
		atp = AsnReadId(aip, amp, CONTACT_INFO);
	else
		atp = AsnLinkType(orig, CONTACT_INFO);    /* link in local tree */
    if (atp == NULL)
        return cip;
	oldatp = atp;

	cip = ContactInfoNew();
    if (cip == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0)    /* read the start struct */
        goto erret;

	while ((atp = AsnReadId(aip, amp, atp)) != oldatp)
	{
		if (atp == NULL) goto erret;
		if (atp == CONTACT_INFO_contact)
		{
			cip->contact = AuthorAsnRead(aip, atp);
			if (cip->contact == NULL) goto erret;
		}
		else if (atp == CONTACT_INFO_address)
		{
			if (AsnReadVal(aip, atp, &av) <= 0)    /* read the start struct */
   	   	  goto erret;
			while ((atp = AsnReadId(aip, amp, atp)) == CONTACT_INFO_address_E)
			{
				vnp = ValNodeNew(vnp);
				if (AsnReadVal(aip, atp, &vnp->data) <= 0)    /* read the string */
   	 	   	 goto erret;
				if (cip->address == NULL)
					cip->address = vnp;
			}
			if (AsnReadVal(aip, atp, &av) <= 0)    /* read the end struct */
   	   	  goto erret;
		}
		else if (atp == CONTACT_INFO_owner_id)
		{
			cip->owner_id = ObjectIdAsnRead(aip, atp);
			if (cip->owner_id == NULL) goto erret;
		}
		else
		{
			if (AsnReadVal(aip, atp, &av) <= 0)    /* read the data */
    	    	goto erret;
			if (atp == CONTACT_INFO_name)
				cip->name = (CharPtr)av.ptrvalue;
			else if (atp == CONTACT_INFO_phone)
				cip->phone = (CharPtr)av.ptrvalue;
			else if (atp == CONTACT_INFO_fax)
				cip->fax = (CharPtr)av.ptrvalue;
			else if (atp == CONTACT_INFO_email)
				cip->email = (CharPtr)av.ptrvalue;
			else if (atp == CONTACT_INFO_telex)
				cip->telex = (CharPtr)av.ptrvalue;
			else if (atp == CONTACT_INFO_password)
				cip->password = (ByteStorePtr)av.ptrvalue;
			else if (atp == CONTACT_INFO_last_name)
				cip->last_name = (CharPtr)av.ptrvalue;
			else if (atp == CONTACT_INFO_first_name)
				cip->first_name = (CharPtr)av.ptrvalue;
			else if (atp == CONTACT_INFO_middle_initial)
				cip->middle_initial = (CharPtr)av.ptrvalue;
		}
	}

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;  /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return cip;
erret:
    cip = ContactInfoFree(cip);
    goto ret;
}

