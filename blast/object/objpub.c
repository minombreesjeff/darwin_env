/*  objpub.c
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
* File Name:  objpub.c
*
* Author:  James Ostell
*   
* Version Creation Date: 4/1/91
*
* $Revision: 6.4 $
*
* File Description:  Object manager for module NCBI-Pub
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 05-13-93 Schuler     All public functions are now declared LIBCALL.
*
*
* $Log: objpub.c,v $
* Revision 6.4  2001/11/20 14:50:54  kans
* PubLabelUnique return immediately if PMID or MUID, already unique
*
* Revision 6.3  2000/06/15 19:13:28  yaschenk
* PubAsnWrite should not fail when pmid is stripped
*
* Revision 6.2  1998/12/30 20:28:34  ostell
* fixed bug in PubLabelUnique() for author list. If alp->names is NULL,
* diff was not initialized. So operations using diff moved inside
* if (vnp2 != NULL)
*
* Revision 6.1  1998/08/24 18:28:08  kans
* removed solaris -v -fd warnings
*
* Revision 6.0  1997/08/25 18:50:27  madden
* Revision changed to 6.0
*
* Revision 4.12  1997/06/19 18:41:46  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.11  1996/09/12 18:42:53  epstein
* spec_version fixes as suggested by J. Kans
*
 * Revision 4.10  1996/07/30  15:22:20  epstein
 * correct logic errors for different spec_versions
 *
 * Revision 4.9  1996/07/01  17:46:53  tatiana
 * month and day added to PubLabelUnique() in submission label
 *
 * Revision 4.8  1996/05/30  21:11:24  tatiana
 * fill in the result buffer if cgp->cit is the only field set (in PubLabelUnique).
 *
 * Revision 4.7  1996/03/29  21:09:59  ostell
 * added support for PubMedId
 *
 * Revision 4.6  1996/03/01  18:08:53  tatiana
 * fix in PubLabelUnique
 *
 * Revision 4.5  1995/10/17  18:10:28  tatiana
 * fix in PubLabelUnique
 *
 * Revision 4.4  1995/10/13  14:04:26  tatiana
 * a bug fixed in PubLabelUnique
 *
 * Revision 4.3  1995/09/07  14:16:55  tatiana
 * fix in PubLabelUnique function
 *
 * Revision 4.2  1995/08/30  22:25:21  kans
 * cap->title uses ->data.ptrvalue
 *
 * Revision 4.1  1995/08/30  18:33:53  ostell
 * added PubLabelUnique function
 *
 * Revision 4.0  1995/07/26  13:48:06  ostell
 * force revision to 4.0
 *
 * Revision 3.7  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/
#include <objpub.h>		   /* the pub interface */
#include <asnpub.h>        /* the AsnTool header */
#include <objmgr.h>

static Boolean loaded = FALSE;

/*****************************************************************************
*
*   Pub ObjMgr Routines
*
*****************************************************************************/
static CharPtr pubtypename = "Pub";

static Pointer LIBCALLBACK PubNewFunc (void)
{
	return (Pointer) ValNodeNew(NULL);
}

static Pointer LIBCALLBACK PubFreeFunc (Pointer data)
{
	return (Pointer) PubFree ((ValNodePtr) data);
}

static Boolean LIBCALLBACK PubAsnWriteFunc (Pointer data, AsnIoPtr aip, AsnTypePtr atp)
{
	return PubAsnWrite((ValNodePtr)data, aip, atp);
}

static Pointer LIBCALLBACK PubAsnReadFunc (AsnIoPtr aip, AsnTypePtr atp)
{
	return (Pointer) PubAsnRead (aip, atp);
}

static Int2 LIBCALLBACK PubLabelFunc ( Pointer data, CharPtr buffer, Int2 buflen, Uint1 content)
{
 	return PubLabel((ValNodePtr)data, buffer, buflen, content);
}

static Uint2 LIBCALLBACK PubSubTypeFunc (Pointer ptr)
{
	if (ptr == NULL)
		return 0;
	return (Uint2)((ValNodePtr)ptr)->choice;
}

/*****************************************************************************
*
*   CitSub ObjMgr Routines
*
*****************************************************************************/
static CharPtr seqsubcittypename = "Cit";

static Pointer LIBCALLBACK CitSubNewFunc (void)
{
	return (Pointer) CitSubNew();
}

static Pointer LIBCALLBACK CitSubFreeFunc (Pointer data)
{
	return (Pointer) CitSubFree ((CitSubPtr) data);
}

static Boolean LIBCALLBACK CitSubAsnWriteFunc (Pointer data, AsnIoPtr aip, AsnTypePtr atp)
{
	return CitSubAsnWrite((CitSubPtr)data, aip, atp);
}

static Pointer LIBCALLBACK CitSubAsnReadFunc (AsnIoPtr aip, AsnTypePtr atp)
{
	return (Pointer) CitSubAsnRead (aip, atp);
}

static Int2 LIBCALLBACK CitSubLabelFunc ( Pointer data, CharPtr buffer, Int2 buflen, Uint1 content)
{
	ValNode vn;

	vn.choice = PUB_Sub;
	vn.data.ptrvalue = data;
	vn.next = NULL;

 	return PubLabel(&vn, buffer, buflen, content);
}


/*****************************************************************************
*
*   PubSet ObjMgr Routines
*
*****************************************************************************/
static CharPtr pubsettypename = "PubSet";

static Pointer LIBCALLBACK PubSetNewFunc (void)
{
	return (Pointer) ValNodeNew(NULL);
}

static Pointer LIBCALLBACK PubSetFreeFunc (Pointer data)
{
	return (Pointer) PubSetFree ((ValNodePtr) data);
}

static Boolean LIBCALLBACK PubSetAsnWriteFunc (Pointer data, AsnIoPtr aip, AsnTypePtr atp)
{
	return PubSetAsnWrite((ValNodePtr)data, aip, atp);
}

static Pointer LIBCALLBACK PubSetAsnReadFunc (AsnIoPtr aip, AsnTypePtr atp)
{
	return (Pointer) PubSetAsnRead (aip, atp);
}

static Int2 LIBCALLBACK PubSetLabelFunc ( Pointer data, CharPtr buffer, Int2 buflen, Uint1 content)
{
	return PubSetLabel ((ValNodePtr)data, buffer, buflen, content);
}

NLM_EXTERN Int2 LIBCALL PubSetLabel (ValNodePtr pubset, CharPtr buffer, Int2 buflen, Uint1 content)
{
	static CharPtr set_names[8] = {
		"NotSet",
		"Pub",
		"Medline",
		"Articles",
		"Journals",
		"Books",
		"Procs",
		"Patents" };
	Int2 diff, len, i = 0;
	Char tbuf[40];
	ValNodePtr vnp;

	if ((pubset == NULL) || (buflen < 1))
		return 0;

	if ((pubset->choice >=1) && (pubset->choice <= 7))
		i= (Int2)(pubset->choice);

	len = buflen;

	if (content == OM_LABEL_TYPE)
		return LabelCopy(buffer, set_names[i], buflen);

	if (content != OM_LABEL_CONTENT)
	{
		diff = LabelCopyExtra(buffer, set_names[i], buflen, NULL, ": ");
		buflen -= diff;
		buffer += diff;
	}

	i=0;
	for (vnp = (ValNodePtr)(pubset->data.ptrvalue); vnp != NULL; vnp = vnp->next)
		i++;

	if (i == 1)
		sprintf(tbuf, "1 pub");
	else
		sprintf(tbuf, "%d pubs", (int)i);

	diff = LabelCopy(buffer, tbuf, buflen);
	buflen -= diff;
	buffer += diff;

	return (len - buflen);   /* no special SUMMARY yet */
}

static Uint2 LIBCALLBACK PubSetSubTypeFunc (Pointer ptr)
{
	if (ptr == NULL)
		return 0;
	return (Uint2)((ValNodePtr)ptr)->choice;
}

/*****************************************************************************
*
*   PubAsnLoad()
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL PubAsnLoad (void)
{
    if (loaded)
        return TRUE;
    loaded = TRUE;

    if (! GeneralAsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }
    if (! BiblioAsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }
    if (! MedlineAsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }
    if (! AsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }

	ObjMgrTypeLoad(OBJ_PUB, "Pub", pubtypename, "Publication",
		PUB, PubNewFunc, PubAsnReadFunc, PubAsnWriteFunc,
		PubFreeFunc, PubLabelFunc, PubSubTypeFunc);

	ObjMgrTypeLoad(OBJ_SEQSUB_CIT, "Cit", seqsubcittypename, "Submission Citation",
		PUB_sub, CitSubNewFunc, CitSubAsnReadFunc, CitSubAsnWriteFunc,
		CitSubFreeFunc, CitSubLabelFunc, NULL);

	ObjMgrTypeLoad(OBJ_SEQFEAT_CIT, "Seqfeat.cit", "Cit", "Feature Citation",
		PUB, PubNewFunc, PubAsnReadFunc, PubAsnWriteFunc,
		PubFreeFunc, PubLabelFunc, PubSubTypeFunc);

	ObjMgrTypeLoad(OBJ_PUB_SET, "Pub-set", pubsettypename, "Set of Publications",
		PUB_SET, PubSetNewFunc, PubSetAsnReadFunc, PubSetAsnWriteFunc,
		PubSetFreeFunc, PubSetLabelFunc, PubSetSubTypeFunc);

    return TRUE;
}


/*****************************************************************************
*
*   Pub Routines
*
*****************************************************************************/
/*****************************************************************************
*
*   Pub is a choice using an ValNode
*   choice:
*   0 = not set
    1 = gen Cit-gen ,        -- general or generic unparsed
    2 = sub Cit-sub ,        -- submission
    3 = medline Medline-entry ,
    4 = muid INTEGER ,       -- medline uid
    5 = article Cit-art ,
    6 = journal Cit-jour ,
    7 = book Cit-book ,
    8 = proc Cit-proc ,      -- proceedings of a meeting
    9 = patent Cit-pat ,
    10 = pat-id Id-pat ,      -- identify a patent
    11 = man Cit-let ,        -- manuscript or letter
    12 = equiv Pub-equiv      -- set of equivalent citation forms for 1 pub
	13 = pmid INTEGER         -- PubMedId
*
*****************************************************************************/
/*****************************************************************************
*
*   PubFree(anp)
*       Frees one pub and associated data
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL PubFree (ValNodePtr anp)
{
    Pointer pnt;

    if (anp == NULL)
        return anp;
    
    pnt = anp->data.ptrvalue;
    switch (anp->choice)
    {
        case 1:                   /* gen */
            CitGenFree((CitGenPtr)pnt);
			break;
        case 2:                   /* sub */
            CitSubFree((CitSubPtr)pnt);
            break;
        case 3:                   /* medline */
            MedlineEntryFree((MedlineEntryPtr)pnt);
            break;
        case 4:                   /* uid in intvalue */
            break;
        case 5:                   /* article */
            CitArtFree((CitArtPtr)pnt);
            break;
        case 6:                   /* journal */
            CitJourFree((CitJourPtr)pnt);
            break;
        case 7:                   /* book */
        case 8:                   /* proceedings */
        case 11:                  /* manuscript */
            CitBookFree((CitBookPtr)pnt);
            break;
        case 9:                   /* patent */
            CitPatFree((CitPatPtr)pnt);
            break;
        case 10:                  /* patent id */
            IdPatFree((IdPatPtr)pnt);
            break;
        case 12:
            PubEquivFree((ValNodePtr)pnt);
            break;
		case 13:                  /* pubmedid */
			break;
    }

	ObjMgrDelete(OBJ_PUB, (Pointer)anp);

	return (ValNodePtr)MemFree(anp);
}

/*****************************************************************************
*
*   PubAsnWrite(anp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (Pub ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL PubAsnWrite (ValNodePtr anp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, writetype = NULL;
    Pointer pnt;
    AsnWriteFunc func = NULL;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! PubAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, PUB);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

	if ((aip->spec_version > 0 && aip->spec_version < 5) && anp->choice >= 13)  /* ASN4 strip new value */
	{
		ErrPostEx(SEV_ERROR,0,0,"ASN4: PubMedId stripped");
		retval=TRUE;
		goto erret;
	}

	av.ptrvalue = (Pointer)anp;
	if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av)) goto erret;

    pnt = anp->data.ptrvalue;
    switch (anp->choice)
    {
        case 1:                   /* gen */
            writetype = PUB_gen;
            func = (AsnWriteFunc) CitGenAsnWrite;
            break;
        case 2:                   /* sub */
            writetype = PUB_sub;
            func = (AsnWriteFunc) CitSubAsnWrite;
            break;
        case 3:                   /* medline */
            writetype = PUB_medline;
            func = (AsnWriteFunc) MedlineEntryAsnWrite;
            break;
        case 4:                   /* uid in intvalue */
            av.intvalue = anp->data.intvalue;
            if (! AsnWrite(aip, PUB_muid, &av)) goto erret;
            break;
        case 5:                   /* article */
            writetype = PUB_article;
            func = (AsnWriteFunc) CitArtAsnWrite;
            break;
        case 6:                   /* journal */
            writetype = PUB_journal;
            func = (AsnWriteFunc) CitJourAsnWrite;
            break;
        case 7:                   /* book */
            writetype = PUB_book;
            func = (AsnWriteFunc) CitBookAsnWrite;
            break;
        case 8:                   /* proceedings */
            writetype = PUB_proc;
            func = (AsnWriteFunc) CitProcAsnWrite;
            break;
        case 11:                  /* manuscript */
            writetype = PUB_man;
            func = (AsnWriteFunc) CitLetAsnWrite;
            break;
        case 9:                   /* patent */
            writetype = PUB_patent;
            func = (AsnWriteFunc) CitPatAsnWrite;
            break;
        case 10:                  /* patent id */
            writetype = PUB_pat_id;
            func = (AsnWriteFunc) IdPatAsnWrite;
            break;
        case 12:                  /* equiv */
            writetype = PUB_equiv;
            func = (AsnWriteFunc) PubEquivAsnWrite;
            break;
        case 13:                   /* pmid in intvalue */
            av.intvalue = anp->data.intvalue;
            if (! AsnWrite(aip, PUB_pmid, &av)) goto erret;
            break;
   }
    if (writetype != NULL)
    {
        if (! (*func)(pnt, aip, writetype)) goto erret;
    }
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}
/*****************************************************************************
*
*   PubAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL PubAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    ValNodePtr anp=NULL;
    Uint1 choice;
    AsnReadFunc func;

	if (! loaded)
	{
		if (! PubAsnLoad())
			return anp;
	}

	if (aip == NULL)
		return anp;

	if (orig == NULL)           /* Pub ::= (self contained) */
		atp = AsnReadId(aip, amp, PUB);
	else
		atp = AsnLinkType(orig, PUB);    /* link in local tree */
    if (atp == NULL) return anp;

	anp = ValNodeNew(NULL);
    if (anp == NULL) goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the CHOICE value (nothing) */
	atp = AsnReadId(aip, amp, atp);  /* find the choice */
    if (atp == NULL) goto erret;

	if (atp == PUB_gen)
    {
        choice = 1;
        func = (AsnReadFunc) CitGenAsnRead;
    }
    else if (atp == PUB_sub)
    {
        choice = 2;
        func = (AsnReadFunc) CitSubAsnRead;
    }
    else if (atp == PUB_medline)
    {
        choice = 3;
        func = (AsnReadFunc) MedlineEntryAsnRead;
    }
    else if (atp == PUB_muid)
    {
        choice = 4;
        if (AsnReadVal(aip, atp, &anp->data) <= 0) goto erret;
    }
    else if (atp == PUB_article)
    {
        choice = 5;
        func = (AsnReadFunc) CitArtAsnRead;
    }
    else if (atp == PUB_journal)
    {
        choice = 6;
        func = (AsnReadFunc) CitJourAsnRead;
    }
    else if (atp == PUB_book)
    {
        choice = 7;
        func = (AsnReadFunc) CitBookAsnRead;
    }
    else if (atp == PUB_proc)
    {
        choice = 8;
        func = (AsnReadFunc) CitProcAsnRead;
    }
    else if (atp == PUB_patent)
    {
        choice = 9;
        func = (AsnReadFunc) CitPatAsnRead;
    }
    else if (atp == PUB_pat_id)
    {
        choice = 10;
        func = (AsnReadFunc) IdPatAsnRead;
    }
    else if (atp == PUB_man)
    {
        choice = 11;
        func = (AsnReadFunc) CitLetAsnRead;
    }
    else if (atp == PUB_equiv)
    {
        choice = 12;
        func = (AsnReadFunc) PubEquivAsnRead;
    }
    else if (atp == PUB_pmid)
    {
        choice = 13;
        if (AsnReadVal(aip, atp, &anp->data) <= 0) goto erret;
    }

    anp->choice = choice;
    if ((choice != 4) && (choice != 13))
    {
        anp->data.ptrvalue = (* func)(aip, atp);
        if (anp->data.ptrvalue == NULL) goto erret;
    }
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return anp;
erret:
    anp = PubFree(anp);
    goto ret;
}

/*****************************************************************************
*
*   PubSet is a choice using an ValNode
*   choice:
*   0 = not set
    1 = pub Pub ,        -- mixed pubs
    3 = medline Medline-entry ,    -- use same mapping as Pub for these
    5 = article Cit-art ,
    6 = journal Cit-jour ,
    7 = book Cit-book ,
    8 = proc Cit-proc ,      -- proceedings of a meeting
    9 = patent Cit-pat ,
*
*   PubSet->data.ptrvalue points to chain of Pub (even if not mixed) so
*      each Pub is self identifying anyway and Pub routines can be used
*****************************************************************************/
/*****************************************************************************
*
*   PubSetFree(anp)
*       Frees one Pub-set and associated data
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL PubSetFree (ValNodePtr anp)
{
    if (anp == NULL)
        return anp;

    PubEquivFree((ValNodePtr)anp->data.ptrvalue);

	ObjMgrDelete(OBJ_PUB_SET, (Pointer)anp);

    return (ValNodePtr)MemFree(anp);
}

/*****************************************************************************
*
*   PubSetAsnWrite(anp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (PubSet ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL PubSetAsnWrite (ValNodePtr anp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, settype, elementtype;
    Pointer pnt;
    Uint1 choice;
    AsnWriteFunc func;
	ValNodePtr oldanp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! PubAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, PUB_SET);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

	av.ptrvalue = (Pointer)anp;
	if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av)) goto erret;
    choice = anp->choice;              /* type of set */
    switch (choice)    
    {
        case 1:              
            settype = PUB_SET_pub;
            elementtype = PUB_SET_pub_E;
            func = (AsnWriteFunc) PubAsnWrite;
            break;
        case 3:              
            settype = PUB_SET_medline;
            elementtype = PUB_SET_medline_E;
            func = (AsnWriteFunc) MedlineEntryAsnWrite;
            break;
        case 5:              
            settype = PUB_SET_article;
            elementtype = PUB_SET_article_E;
            func = (AsnWriteFunc) CitArtAsnWrite;
            break;
        case 6:              
            settype = PUB_SET_journal;
            elementtype = PUB_SET_journal_E;
            func = (AsnWriteFunc) CitJourAsnWrite;
            break;
        case 7:  
            settype = PUB_SET_book;
            elementtype = PUB_SET_book_E;
            func = (AsnWriteFunc) CitBookAsnWrite;
            break;
        case 8:
            settype = PUB_SET_proc;
            elementtype = PUB_SET_proc_E;
            func = (AsnWriteFunc) CitBookAsnWrite;
            break;
        case 9:
            settype = PUB_SET_patent;
            elementtype = PUB_SET_patent_E;
            func = (AsnWriteFunc) CitPatAsnWrite;
            break;
    }

	oldanp = anp;
    if (! AsnOpenStruct(aip, settype, oldanp->data.ptrvalue))   /* start SET OF */
        goto erret;

    anp = (ValNodePtr)anp->data.ptrvalue;

    while (anp != NULL)
    {
        if (choice == 1)
            pnt = (Pointer)anp;
        else
            pnt = anp->data.ptrvalue;
        if (! (*func)(pnt, aip, elementtype)) goto erret;
        anp = anp->next;
    }
    
    if (! AsnCloseStruct(aip, settype, oldanp->data.ptrvalue))  /* end SET OF */
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   PubSetAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL PubSetAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, settype;
    Pointer pnt;
    ValNodePtr anp=NULL, cit, curr;
    Uint1 choice;
    AsnReadFunc func;
    Boolean first;

	if (! loaded)
	{
		if (! PubAsnLoad())
			return anp;
	}

	if (aip == NULL)
		return anp;

	if (orig == NULL)           /* PubSet ::= (self contained) */
		atp = AsnReadId(aip, amp, PUB_SET);
	else
		atp = AsnLinkType(orig, PUB_SET);    /* link in local tree */
    if (atp == NULL) return anp;

	anp = ValNodeNew(NULL);
    if (anp == NULL) goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0)    /* read the CHOICE value (nothing) */
        goto erret;
	settype = AsnReadId(aip, amp, atp);  /* find the choice */
    if (settype == NULL) goto erret;
    if (AsnReadVal(aip, settype, &av) <= 0)    /* read START_STRUCT */
        goto erret;

	if (settype == PUB_SET_pub)
    {
        choice = 1;
        func = (AsnReadFunc) PubAsnRead;
    }
    else if (settype == PUB_SET_medline)
    {
        choice = 3;
        func = (AsnReadFunc) MedlineEntryAsnRead;
    }
    else if (settype == PUB_SET_article)
    {
        choice = 5;
        func = (AsnReadFunc) CitArtAsnRead;
    }
    else if (settype == PUB_SET_journal)
    {
        choice = 6;
        func = (AsnReadFunc) CitJourAsnRead;
    }
    else if (settype == PUB_SET_book)
    {
        choice = 7;
        func = (AsnReadFunc) CitBookAsnRead;
    }
    else if (settype == PUB_SET_proc)
    {
        choice = 8;
        func = (AsnReadFunc) CitBookAsnRead;
    }
    else if (settype == PUB_SET_patent)
    {
        choice = 9;
        func = (AsnReadFunc) CitPatAsnRead;
    }

    anp->choice = choice;
    first = TRUE;
    curr = NULL;
    while ((atp = AsnReadId(aip, amp, settype)) != settype)
    {
        if (atp == NULL) goto erret;
        pnt = (* func)(aip, atp);
        if (pnt == NULL) goto erret;
        if (settype == PUB_SET_pub)   /* already a Pub */
        {
            cit = (ValNodePtr)pnt;
        }
        else                          /* make into a Pub */
        {
            cit = ValNodeNew(NULL);
            if (cit == NULL) goto erret;
            cit->data.ptrvalue = pnt;
            cit->choice = choice;
        }

        if (first)
        {
            anp->data.ptrvalue = (Pointer)cit;
            first = FALSE;
        }
        else
        {
            curr->next = cit;
        }
        curr = cit;
    }
    if (AsnReadVal(aip, settype, &av) <= 0)   /* read END_STRUCT for SET OF */
        goto erret;
	if (anp == NULL)
		ErrPost(CTX_NCBIOBJ, 1, "Empty SET OF Pub.  line %ld", aip->linenumber);
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return anp;
erret:
    anp = PubSetFree(anp);
    goto ret;
}

/*****************************************************************************
*
*   PubEquiv is just a chain of Pubs (ValNodes)
*   
*****************************************************************************/
/*****************************************************************************
*
*   PubEquivFree(anp)
*       Frees a chain of Pubs
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL PubEquivFree (ValNodePtr anp)
{
    ValNodePtr next;

    while (anp != NULL)
    {
        next = anp->next;
        PubFree(anp);  /* each node individually coded as Pub anyway */
        anp = next;
    }
	return anp;
}

/*****************************************************************************
*
*   PubEquivAsnWrite(anp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (PubEquiv ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL PubEquivAsnWrite (ValNodePtr anp, AsnIoPtr aip, AsnTypePtr orig)
{
	AsnTypePtr atp;
	ValNodePtr oldanp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! PubAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, PUB_EQUIV);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

	oldanp = anp;
    if (! AsnOpenStruct(aip, atp, (Pointer)oldanp))   /* start SET OF */
        goto erret;

    while (anp != NULL)
    {
        if (! PubAsnWrite(anp, aip, PUB_EQUIV_E)) goto erret;
        anp = anp->next;
    }

    if (! AsnCloseStruct(aip, atp, (Pointer)oldanp))     /* end SET OF */
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   PubEquivAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL PubEquivAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldtype;
    ValNodePtr anp=NULL, cit, curr;
    Boolean first;

	if (! loaded)
	{
		if (! PubAsnLoad())
			return anp;
	}

	if (aip == NULL)
		return anp;

	first = TRUE;

	if (orig == NULL)           /* PubEquiv ::= (self contained) */
		oldtype = AsnReadId(aip, amp, PUB_EQUIV);
	else
		oldtype = AsnLinkType(orig, PUB_EQUIV);    /* link in local tree */
    if (oldtype == NULL) return anp;

    if (AsnReadVal(aip, oldtype, &av) <= 0)    /* read START_STRUCT */
        goto erret;
    atp = oldtype;

    curr = NULL;
    anp = NULL;
    while ((atp = AsnReadId(aip, amp, atp)) != oldtype)
    {
        if (atp == NULL) goto erret;

        cit = PubAsnRead(aip, atp);
        if (cit == NULL) goto erret;

        if (first)
        {
            anp = cit;
            first = FALSE;
        }
        else
        {
            curr->next = cit;
        }
        curr = cit;
    }
    if (AsnReadVal(aip, oldtype, &av) <= 0)   /* read END_STRUCT for SET OF */
        goto erret;
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return anp;
erret:
    anp = PubEquivFree(anp);
    goto ret;
}

/*****************************************************************************
*
*   Int2 PubMatch(a, b)
*   	returns 0 if can determine pubs POINT TO SAME CITATION (not that
*   		they are identical)
*   	returns positive or negative number if not the same,
*   		for arbitrary ordering
*       -2, +2  = different types
*       -1, +1  = different values, same type
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL PubMatch (ValNodePtr a, ValNodePtr b)
{
	ValNode vn1;
	ValNodePtr ap, bp, tp, vnp[2];
	Int2 retval, i;
	Int4 muid[2];
	Int4 pmid[2];
	CitArtPtr cap[2];
	CharPtr country[2],   /* for patents */
		number[2],
		app_number[2];

	if (a == NULL) return 2;
	if (b == NULL) return -2;

	                        /* default return for different pub types */
	if (a->choice > b->choice)
		retval = 2;
	else
		retval = -2;

							/* if either is a Pub-equiv, treat as both */
	if ((a->choice == PUB_Equiv) || (b->choice == PUB_Equiv))
	{
		ap = (ValNodePtr)(a->data.ptrvalue);
		bp = (ValNodePtr)(b->data.ptrvalue);
		tp = NULL;
		if (a->choice != PUB_Equiv)
		{
			tp = a;
			ap = NULL;
		}
		else if (b->choice != PUB_Equiv)
		{
			tp = b;
			bp = NULL;
		}
		if (tp != NULL)   /* convert one */
		{
			MemCopy((Pointer)&vn1, tp, sizeof(ValNode));  /* make a copy of the Pub */
			vn1.next = NULL;                   /* remove from any chain */
			tp = (ValNodePtr)&vn1;
			if (ap == NULL)
				ap = tp;
			else
				bp = tp;
		}
		return PubEquivMatch(ap, bp);   /* use the PubEquivMatch() */
	}
                                   /** here we are just matching two pubs */
							/* handle Medline/CitArt combinations */
	                        /* handle CitPat, IdPat combinations */
	vnp[0] = a;
	vnp[1] = b;
	for (i = 0; i < 2; i++)
	{
		cap[i] = NULL;
		muid[i] = 0;
		pmid[i] = 0;
		country[i] = NULL;
		number[i] = NULL;
		app_number[i] = NULL;

		switch (vnp[i]->choice)
		{
			case PUB_Medline:
				muid[i] = ((MedlineEntryPtr)(vnp[i]->data.ptrvalue))->uid;
				cap[i] = ((MedlineEntryPtr)(vnp[i]->data.ptrvalue))->cit;
				pmid[i] = ((MedlineEntryPtr)(vnp[i]->data.ptrvalue))->pmid;
				break;
			case PUB_Muid:
				muid[i] = vnp[i]->data.intvalue;
				break;
			case PUB_Article:
				cap[i] = (CitArtPtr)(vnp[i]->data.ptrvalue);
				break;
			case PUB_Patent:
				country[i] = ((CitPatPtr)(vnp[i]->data.ptrvalue))->country;
				number[i] = ((CitPatPtr)(vnp[i]->data.ptrvalue))->number;
				app_number[i] = ((CitPatPtr)(vnp[i]->data.ptrvalue))->app_number;
				break;
			case PUB_Pat_id:
				country[i] = ((IdPatPtr)(vnp[i]->data.ptrvalue))->country;
				number[i] = ((IdPatPtr)(vnp[i]->data.ptrvalue))->number;
				app_number[i] = ((IdPatPtr)(vnp[i]->data.ptrvalue))->app_number;
				break;
			case PUB_PMid:
				pmid[i] = vnp[i]->data.intvalue;
				break;
		}
	}

	if ((pmid[0] > 0) && (pmid[1] > 0))   /* got 2 muids */
	{
		if (pmid[0] == pmid[1])
			return (Int2)0;
		else if (pmid[0] < pmid[1])
			return (Int2)-1;
		else
			return (Int2) 1;
	}

	if ((muid[0] > 0) && (muid[1] > 0))   /* got 2 muids */
	{
		if (muid[0] == muid[1])
			return (Int2)0;
		else if (muid[0] < muid[1])
			return (Int2)-1;
		else
			return (Int2) 1;
	}

	if ((cap[0] != NULL) && (cap[1] != NULL))  /* 2 articles */
	{
		return CitArtMatch(cap[0], cap[1]);
	}

	if ((country[0] != NULL) && (country[1] != NULL))  /* 2 patents */
	{
		retval = (Int2)StringICmp(country[0], country[1]);
		if (retval < 0)   /* different countries */
			return (Int2) -1;
		else if (retval > 0)
			return (Int2) 1;

		if ((number[0] != NULL) && (number[1] != NULL))
		{
			retval = (Int2)StringICmp(number[0], number[1]);
			if (retval < 0)   /* different number */
				return (Int2) -1;
			else if (retval > 0)
				return (Int2) 1;
			else
				return retval;
		}
		if ((app_number[0] != NULL) && (app_number[1] != NULL))
		{
			retval = (Int2)StringICmp(app_number[0], app_number[1]);
			if (retval < 0)   /* different appl number */
				return (Int2) -1;
			else if (retval > 0)
				return (Int2) 1;
			else
				return retval;
		}
		if (number[0] != NULL)
			return (Int2) -1;
		return (Int2) 1;
	}

	if (a->choice != b->choice)    /* all others must be same type */
	{
		return retval;
	}
							/* compare other types */
	switch (a->choice)
	{
		case PUB_Gen:        /* generic */
			return CitGenMatch((CitGenPtr)a->data.ptrvalue,
					(CitGenPtr)b->data.ptrvalue, TRUE);
		case PUB_Sub:        /* Submission */
		 	return CitSubMatch((CitSubPtr)a->data.ptrvalue,
					(CitSubPtr)b->data.ptrvalue);
		case PUB_Journal:
			return CitJourMatch((CitJourPtr)a->data.ptrvalue,
					(CitJourPtr)b->data.ptrvalue);
		case PUB_Book:
		case PUB_Proc:
		case PUB_Man:
			return CitBookMatch((CitBookPtr)a->data.ptrvalue,
					(CitBookPtr)b->data.ptrvalue);
	}

	return retval;
}

/*****************************************************************************
*
*   Int2 PubEquivMatch(a, b)
*   	returns 0 if can determine pubs POINT TO SAME CITATION (not that
*   		they are identical)
*   	returns +1 or -1 if not the same,
*   		for arbitrary ordering
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL PubEquivMatch (ValNodePtr a, ValNodePtr b)
{
	Int2 retval = 0, tmp;
	ValNodePtr vnp;

	while (a != NULL)
	{
		vnp = b;
		while (vnp != NULL)
		{
			tmp = PubMatch(a, vnp);
			if (! tmp)	   /* a match */
			{
				return tmp;
			}
			if (! retval)   /* first one */
				retval = tmp;
			else if ((retval == 2) || (retval == -2))  /* type diffs only */
				retval = tmp;

			vnp = vnp->next;
		}
		a = a->next;
	}
	if (retval < 0)
		retval = -1;
	else
		retval = 1;
	return retval;
}

/*****************************************************************************
*
*   PubLabel(pubptr, buf, buflen, content)
*   	makes a short label for any Pub in buf, up to buflen size
*   	content follows objmgr OM_LABEL_
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL PubLabel (ValNodePtr pub, CharPtr buf, Int2 buflen, Uint1 content)
{
	return PubLabelUnique (pub, buf, buflen, content, FALSE);
}

/*****************************************************************************
*
*   PubLabelUnique(pubptr, buf, buflen, content, unique)
*   	makes a short label for any Pub in buf, up to buflen size
*   	content follows objmgr OM_LABEL_
*       if (unique is TRUE, appends a string based on title words to make
*           unique key base on ANSI std Z39.56-1991
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL PubLabelUnique (ValNodePtr pub, CharPtr buf, Int2 buflen, Uint1 content, Boolean unique)
{
	CharPtr typelabel = NULL;
	static CharPtr pubtypes [14] = {
		"Unknown",
		"Generic",
		"Submit",
		"Medline",
		"MUID",
		"Article",
		"Journal",
		"Book",
		"Proceedings",
		"Patent",
		"PatID",
		"Manuscript",
		"Equiv" ,
		"PMID" };
	ValNodePtr vnp2=NULL, title=NULL;
	Int2 len, diff;
	Char tbuf[41];
	Boolean first = TRUE;

	Int4 muid = 0, pmid = 0;
	AuthListPtr alp=NULL;
	AuthorPtr ap;
	ImprintPtr imp = NULL;
	CharPtr year = NULL,
			volume = NULL,
			issue = NULL,
			pages = NULL,
			title1=NULL,
			title2=NULL,
			titleunique = NULL,
			part_sup = NULL,
			part_supi = NULL;
	CitArtPtr cap;
	CitJourPtr cjp;
	CitBookPtr cbp=NULL;
	CitSubPtr csp;
	CitPatPtr cpp;
	IdPatPtr ipp;
	CitGenPtr cgp;
	MedlineEntryPtr mep;
	DatePtr dp = NULL;
	Boolean unpublished = FALSE, done;
	ValNodePtr eq[5];
	Int2 i;
	CharPtr s, cit;



	if ((buf == NULL) || (buflen < 1)) return 0;

	buf[0] = '?';
	buf[1] = '\0';

	if (pub == NULL) return 0;

	if (pub->choice > 13)
		typelabel = pubtypes[0];
	else
		typelabel = pubtypes[pub->choice];

	len = buflen;

	if (content == OM_LABEL_TYPE)
		return LabelCopy(buf, typelabel, buflen);

	if (content == OM_LABEL_BOTH)
	{
		diff = LabelCopyExtra(buf, typelabel, buflen, NULL, ": ");
		buflen -= diff; buf += diff;
	}

	switch (pub->choice)
	{
		case PUB_Muid:
			sprintf(tbuf, "NLM%ld", (long)(pub->data.intvalue));
			diff = LabelCopy(buf, tbuf, buflen);
			buflen -= diff;
			return (len - buflen); /* already unique */
			break;
		case PUB_PMid:
			sprintf(tbuf, "PM%ld", (long)(pub->data.intvalue));
			diff = LabelCopy(buf, tbuf, buflen);
			buflen -= diff;
			return (len - buflen); /* already unique */
			break;
		case PUB_Equiv:
			for (i = 0; i < 5; i++)
				eq[i] = NULL;
			i = 0;
			for (vnp2 = (ValNodePtr)(pub->data.ptrvalue);
			              ((vnp2 != NULL) && (buflen)); vnp2 = vnp2->next)
			{
				switch (vnp2->choice)
				{
					case PUB_Muid:
						eq[3] = vnp2;
						break;
					case PUB_Gen:
						cgp = (CitGenPtr)(vnp2->data.ptrvalue);
						if (cgp->serial_number > 0)
						{
							eq[4] = vnp2;
							break;
						}
					default:
						if (i < 5)
							eq[i] = vnp2;
						i++;
						break;
				}	
			}
			for (i = 0; i < 5; i++)
			{
				if (eq[i] != NULL)
				{
					if (! first)
					{
						diff = LabelCopy(buf, " ", buflen);
						buflen -= diff; buf += diff;
					}
					else
						first = FALSE;

					diff = PubLabel(eq[i], buf, buflen, OM_LABEL_CONTENT);
					buflen -= diff; buf += diff;
				}
			}
			break;
		case PUB_Medline:
			mep = (MedlineEntryPtr)(pub->data.ptrvalue);
			if (mep->pmid > 0)
				sprintf(tbuf, "PM%ld", (long)(mep->pmid));
			else
				sprintf(tbuf, "NLM%ld", (long)(mep->uid));
			diff = LabelCopyExtra(buf, tbuf, buflen, NULL, " ");
			buflen -= diff;
			cap = mep->cit;
			goto cit_art;
		case PUB_Article:
			cap = (CitArtPtr)(pub->data.ptrvalue);
cit_art:	alp = cap->authors;
			if (cap->title != NULL)
				titleunique = (CharPtr)(cap->title->data.ptrvalue);
			switch (cap->from)
			{
				case 1:
					cjp = (CitJourPtr)(cap->fromptr);
					goto cit_jour;
				case 2:
				case 3:
					cbp = (CitBookPtr)(cap->fromptr);
					goto cit_book;
			}
			break;
		case PUB_Journal:
			cjp = (CitJourPtr)(pub->data.ptrvalue);
cit_jour:	imp = cjp->imp;
			title = cjp->title;
			break;
		case PUB_Book:
		case PUB_Proc:
		case PUB_Man:
			cbp = (CitBookPtr)(pub->data.ptrvalue);
			title = cbp->title;
cit_book:   imp = cbp->imp;
			if (alp == NULL)
				alp = cbp->authors;
			break;
		case PUB_Sub:
			csp = (CitSubPtr)(pub->data.ptrvalue);
			alp = csp->authors;
			imp = csp->imp;
			dp = csp->date;
			break;
		case PUB_Patent:
			cpp = (CitPatPtr)(pub->data.ptrvalue);
			alp = cpp->authors;
			dp = cpp->date_issue;
			if (dp == NULL)
				dp = cpp->app_date;
			title1 = cpp->country;
			title2 = cpp->number;
			if (title2 == NULL)
				title2 = cpp->app_number;
			break;
		case PUB_Pat_id:
			ipp = (IdPatPtr)(pub->data.ptrvalue);
			title1 = ipp->country;
			title2 = ipp->number;
			if (title2 == NULL)
				title2 = ipp->app_number;
			break;
		case PUB_Gen:
			cgp = (CitGenPtr)(pub->data.ptrvalue);
			if (cgp->serial_number > 0)
			{
				sprintf(tbuf, "[%d]", (int)(cgp->serial_number));
				diff = LabelCopy(buf, tbuf, buflen);
				buflen -= diff; buf += diff;
			}
			if (cgp->muid > 0)
			{
				sprintf(tbuf, "NLM%ld", (long)(cgp->muid));
				diff = LabelCopy(buf, tbuf, buflen);
				buflen -= diff; buf += diff;

			}
			dp = cgp->date;
			title = cgp->journal;
			alp = cgp->authors;
			if (cgp->cit != NULL)
			{
				if (! StringICmp("Unpublished", cgp->cit))
					unpublished = TRUE;
				else if (title == NULL)
					title2 = cgp->cit;
			}
			volume = cgp->volume;
			issue=cgp->issue;
			pages=cgp->pages;
            if (cgp->title != NULL)
				titleunique = cgp->title;
			else if (title2 != NULL)
				titleunique = title2;
			else if (title == NULL)
				titleunique = cgp->cit;
			if (title == NULL && alp == NULL && cgp->title == NULL &&
				volume == NULL && pages == NULL && issue == NULL) {
				titleunique = NULL;
				if ((cit = StringSave(cgp->cit)) != NULL) {
					if (!unique) {
						for (s = cit + StringLen(cit) -1; 
									s > cit && *s !='|';   s--) continue;
						if (*s == '|' ) {
							*s = '\0';
						}
					}
					diff = LabelCopy(buf, cit, buflen);
					MemFree(cit);
				}
				return 0;
			}
			break;
		default:
			sprintf(tbuf,"Unknown pub[%d]", (int)(pub->choice));
			title2 = tbuf;
			break;

	}

	if (imp != NULL)
	{
		if (dp == NULL)
			dp = imp->date;
		if (volume == NULL)
			volume = imp->volume;
		if (issue == NULL)
			issue = imp->issue;
		if (pages == NULL)
			pages = imp->pages;
		part_sup = imp->part_sup;
		part_supi = imp->part_supi;
	}

	if (alp != NULL)
	{
		vnp2 = alp->names;
		if (vnp2 != NULL)
		{
			if (alp->choice == 1)   /* std name */
			{
				ap = (AuthorPtr)(vnp2->data.ptrvalue);
				diff = PersonIdLabel(ap->name, buf, buflen, PIDLABEL_GENBANK);
			}
			else
				diff = LabelCopy(buf, (CharPtr)(vnp2->data.ptrvalue), buflen);
		buflen -= diff;
		buf += diff;
		}
	}

	if (dp != NULL)
	{
		if (dp->data[0])
		{
			if (pub->choice == PUB_Sub && dp->data[2] != 0 && dp->data[3] != 0){
				sprintf(tbuf, "%d-%d-%d", (int)(dp->data[2]), 
						(int)(dp->data[3]), (int)(dp->data[1])+1900);
				year = tbuf;
			} else {
				sprintf(tbuf, "%d", (int)(dp->data[1])+1900);
				year = tbuf;
			}
		}
		else
			year = dp->str; 
		diff = LabelCopyExtra(buf, year, buflen, " (", ") ");
		buflen -= diff; buf += diff;
	}

	if ((title != NULL) && (titleunique == NULL))
		titleunique = (CharPtr)(title->data.ptrvalue);

	if (title2 == NULL)
	{
		if (title != NULL)
			title2 = (CharPtr)(title->data.ptrvalue);
			
	}

	if (title2 != NULL)
	{
		if (cbp != NULL)
			title1 = "(in) ";

		diff = LabelCopyExtra(buf, title2, buflen, title1, " ");
		buflen -= diff; buf += diff;
	}

	if (volume != NULL)
	{
		if (part_sup != NULL)
			title1 = part_sup;
		else
			title1 = ":";
			
		diff = LabelCopyExtra(buf, volume, buflen, NULL, title1);
		buflen -= diff; buf += diff;
		if (part_sup != NULL)
		{
			diff = LabelCopyExtra(buf, ":", buflen, NULL, NULL);
			buflen -= diff; buf += diff;
		}
	}

	if (issue != NULL)
	{
		if (part_supi != NULL)
			title1 = part_supi;
		else
			title1 = ")";
			
		diff = LabelCopyExtra(buf, issue, buflen, "(" , title1);
		buflen -= diff; buf += diff;
		if (part_supi != NULL)
		{
			diff = LabelCopyExtra(buf, ")", buflen, NULL, NULL);
			buflen -= diff; buf += diff;
		}
	}

	if (pages != NULL)
	{
		diff = LabelCopy(buf, pages, buflen);
		buflen -= diff; buf += diff;
	}
     
    if (unpublished)
    {
    	diff = LabelCopy(buf, "Unpublished", buflen);
    	buflen -= diff; buf += diff;
    }

	if (unique)  /* put on unique tag made from title */
	{
		done = FALSE;
		i=0;
		if (titleunique != NULL && *titleunique != '\0')
		{
			while ((! done) && (i < 40))
			{
				tbuf[i] = *titleunique;
				i++;
				while (! IS_WHITESP(*titleunique))
				{
					titleunique++;
					if (*titleunique == '\0')
					{
						done = TRUE;
						break;
					}
				}
				while (IS_WHITESP(*titleunique))
				{
					titleunique++;
					if (*titleunique == '\0')
					{
						done = TRUE;
						break;
					}
				}
			}
		}
		tbuf[i] = '\0';
		diff = LabelCopyExtra(buf, tbuf, buflen, "|" , NULL);
		buflen -= diff; buf += diff;
	}

    
	return (len - buflen);
}

