/*  objmedli.c
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
* File Name:  objmedli.c
*
* Author:  James Ostell
*   
* Version Creation Date: 1/1/91
*
* $Revision: 6.4 $
*
* File Description:  Object manager for module NCBI-Medline
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 05-13-93 Schuler     All public functions are now declared LIBCALL.
*
*
* $Log: objmedli.c,v $
* Revision 6.4  1998/08/26 17:43:17  kans
* fixed -v -fd warnings in label functions
*
* Revision 6.3  1998/08/24 18:28:06  kans
* removed solaris -v -fd warnings
*
* Revision 6.2  1997/10/28 16:09:11  grisha
* fixed bug in function MedlineEntryAsnRead() to load status
*
* Revision 6.1  1997/09/03 12:55:56  ostell
* added support for MedlineEntry.status
*
* Revision 6.0  1997/08/25 18:50:13  madden
* Revision changed to 6.0
*
* Revision 4.9  1997/08/20 19:17:08  madden
* Version 6 of ASN.1
*
* Revision 4.8  1997/06/19 18:41:35  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.7  1996/08/14 20:57:32  grisha
* Check ASN.1 version. Write pub_type if version set to 0
*
 * Revision 4.6  1996/08/12  14:07:21  grisha
 * Generate pub_type if ASN.1 spec version >= 5
 *
 * Revision 4.5  1996/07/30  15:23:01  epstein
 * correct logic errors for different spec_versions
 *
 * Revision 4.4  1996/07/24  16:35:54  ostell
 * corrected AsnRead putting pub_type in gene slot.
 *
 * Revision 4.3  1996/05/20  19:43:50  ostell
 * added pub_type
 *
 * Revision 4.2  1996/03/29  21:09:59  ostell
 * added support for PubMedId
 *
 * Revision 4.1  1995/12/22  17:16:07  grisha
 * add function MedlineSiFree(), because new Client/Server
 * library for MedArch server needs this function
 *
 * Revision 4.0  1995/07/26  13:48:06  ostell
 * force revision to 4.0
 *
 * Revision 3.5  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#include <objmedli.h>
#include <asnmedli.h>
#include <objmgr.h>

static Boolean loaded = FALSE;

/*****************************************************************************
*
*   MedlineEntry ObjMgr Routines
*
*****************************************************************************/
static CharPtr typename = "MedlineEntry";

static Pointer LIBCALLBACK MedlineEntryNewFunc (void)
{
	return (Pointer) MedlineEntryNew();
}

static Pointer LIBCALLBACK MedlineEntryFreeFunc (Pointer data)
{
	return (Pointer) MedlineEntryFree ((MedlineEntryPtr) data);
}

static Boolean LIBCALLBACK MedlineEntryAsnWriteFunc (Pointer data, AsnIoPtr aip, AsnTypePtr atp)
{
	return MedlineEntryAsnWrite((MedlineEntryPtr)data, aip, atp);
}

static Pointer LIBCALLBACK MedlineEntryAsnReadFunc (AsnIoPtr aip, AsnTypePtr atp)
{
	return (Pointer) MedlineEntryAsnRead (aip, atp);
}

static Int2 LIBCALLBACK MedlineEntryLabelFunc ( Pointer data, CharPtr buffer, Int2 buflen, Uint1 content)
{
	Int2 rsult = 0;

	switch (content)
	{
		case OM_LABEL_BOTH:
		case OM_LABEL_CONTENT:
		case OM_LABEL_SUMMARY:
		case OM_LABEL_TYPE:
		default:
			rsult = LabelCopy(buffer, typename, buflen);
	}
	return rsult;
}

/*****************************************************************************
*
*   Medline Object routines
*
*****************************************************************************/

/*****************************************************************************
*
*   MedlineAsnLoad()
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL MedlineAsnLoad (void)
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
    if (! AsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }

	ObjMgrTypeLoad(OBJ_MEDLINE_ENTRY, "Medline-entry", typename, "MEDLINE entry",
		MEDLINE_ENTRY, MedlineEntryNewFunc, MedlineEntryAsnReadFunc, MedlineEntryAsnWriteFunc,
		MedlineEntryFreeFunc, MedlineEntryLabelFunc, NULL);

    return TRUE;
}

/*****************************************************************************
*
*   MedlineMeshNew()
*
*****************************************************************************/
NLM_EXTERN MedlineMeshPtr LIBCALL MedlineMeshNew (void)
{
	MedlineMeshPtr mmp;

	mmp = (MedlineMeshPtr)MemNew(sizeof(MedlineMesh));
	return mmp;
}

/*****************************************************************************
*
*   MedlineMeshFree()
*
*****************************************************************************/
NLM_EXTERN MedlineMeshPtr LIBCALL MedlineMeshFree (MedlineMeshPtr mmp)
{
    if (mmp == NULL)
        return mmp;

    MemFree(mmp->term);
    ValNodeFreeData(mmp->qual);
    
	return (MedlineMeshPtr)MemFree(mmp);
}

/*****************************************************************************
*
*   MedlineMeshAsnRead(aip, atp)
*
*****************************************************************************/
NLM_EXTERN MedlineMeshPtr LIBCALL MedlineMeshAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	MedlineMeshPtr mmp=NULL;
	DataVal av;
	AsnTypePtr atp, oldatp;
    ValNodePtr first, anp;

	if (! loaded)
	{
		if (! MedlineAsnLoad())
			return mmp;
	}

	if (aip == NULL)
		return mmp;

	if (orig == NULL)           /* MedlineMesh ::= */
		atp = AsnReadId(aip, amp, MEDLINE_MESH);
	else
		atp = AsnLinkType(orig, MEDLINE_MESH);
    oldatp = atp;
    if (atp == NULL)
        return mmp;

	mmp = MedlineMeshNew();
    if (mmp == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the SEQUENCE */
    first = NULL;
    while ((atp = AsnReadId(aip, amp, atp)) != oldatp)
    {
        if (atp == NULL) goto erret;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        if (atp == MEDLINE_MESH_qual)     /* SET OF medline-qual */
        {
           while ((atp = AsnReadId(aip, amp, atp)) != MEDLINE_MESH_qual)
           {
               if (atp == NULL) goto erret;
               if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
               if ((atp == MEDLINE_MESH_qual_E) && (av.intvalue == START_STRUCT))
               {
                   anp = ValNodeNew(first);
                   if (anp == NULL) goto erret;
                   if (first == NULL)
                    first = anp;
               }
               else if (atp == MEDLINE_QUAL_mp)
                    anp->choice = (Uint1)av.boolvalue;
                else if (atp == MEDLINE_QUAL_subh)
                    anp->data.ptrvalue = av.ptrvalue;
           }
           mmp->qual = first;
           if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end of SET OF */
        }
        else if (atp == MEDLINE_MESH_mp)
            mmp->mp = av.boolvalue;
        else if (atp == MEDLINE_MESH_term)
            mmp->term = (CharPtr)av.ptrvalue;
    }

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the END STRUCT */
ret:
	AsnUnlinkType(orig);
	return mmp;
erret:
    mmp = MedlineMeshFree(mmp);
    goto ret;
}

/*****************************************************************************
*
*   MedlineMeshAsnWrite(mmp, aip, atp) 
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL MedlineMeshAsnWrite (MedlineMeshPtr mmp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    ValNodePtr anp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! MedlineAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, MEDLINE_MESH);
    if (atp == NULL) return FALSE;

	if (mmp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)mmp)) goto erret;
    if (mmp->mp)
    {
        av.boolvalue = TRUE;
        if (! AsnWrite(aip, MEDLINE_MESH_mp, &av)) goto erret;
    }
    av.ptrvalue = mmp->term;
    if (! AsnWrite(aip, MEDLINE_MESH_term, &av)) goto erret;
    if (mmp->qual != NULL)
    {						    /* start SET OF */
        if (! AsnOpenStruct(aip, MEDLINE_MESH_qual, (Pointer)mmp->qual))
            goto erret;
        anp = mmp->qual;
        while (anp != NULL)
        {
            if (! AsnOpenStruct(aip, MEDLINE_MESH_qual_E, (Pointer)anp))
                goto erret;
            if (anp->choice != 0)
            {
                av.boolvalue = TRUE;
                if (! AsnWrite(aip, MEDLINE_QUAL_mp, &av)) goto erret;
            }
            av.ptrvalue = anp->data.ptrvalue;
            if (! AsnWrite(aip, MEDLINE_QUAL_subh, &av)) goto erret;
            if (! AsnCloseStruct(aip, MEDLINE_MESH_qual_E, (Pointer)anp))
                goto erret;
            anp = anp->next;
        }
        if (! AsnCloseStruct(aip, MEDLINE_MESH_qual, (Pointer)mmp->qual))
            goto erret;
    }

    if (! AsnCloseStruct(aip, atp, (Pointer)mmp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);
	return retval;
}

/*****************************************************************************
*
*   MedlineRnNew()
*
*****************************************************************************/
NLM_EXTERN MedlineRnPtr LIBCALL MedlineRnNew (void)
{
	MedlineRnPtr mrp;

	mrp = (MedlineRnPtr)MemNew(sizeof(MedlineRn));
	return mrp;
}

/*****************************************************************************
*
*   MedlineRnFree()
*
*****************************************************************************/
NLM_EXTERN MedlineRnPtr LIBCALL MedlineRnFree (MedlineRnPtr mrp)
{
    if (mrp == NULL)
        return mrp;

    MemFree(mrp->cit);
    MemFree(mrp->name);
    
	return (MedlineRnPtr)MemFree(mrp);
}

/*****************************************************************************
*
*   MedlineRnAsnRead(aip, atp)
*
*****************************************************************************/
NLM_EXTERN MedlineRnPtr LIBCALL MedlineRnAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	MedlineRnPtr mrp=NULL;
	DataVal av;
	AsnTypePtr atp;

	if (! loaded)
	{
		if (! MedlineAsnLoad())
			return mrp;
	}

	if (aip == NULL)
		return mrp;

	if (orig == NULL)           /* MedlineRn ::= */
		atp = AsnReadId(aip, amp, MEDLINE_RN);
	else
		atp = AsnLinkType(orig, MEDLINE_RN);
    if (atp == NULL) return mrp;

	mrp = MedlineRnNew();
    if (mrp == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the SEQUENCE */
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;  /* type */
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    mrp->type = (Uint1)av.intvalue;
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (atp == MEDLINE_RN_cit)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        mrp->cit = (CharPtr)av.ptrvalue;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    mrp->name = (CharPtr)av.ptrvalue;

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the END STRUCT */
ret:
	AsnUnlinkType(orig);
	return mrp;
erret:
    mrp = MedlineRnFree(mrp);
    goto ret;
}

/*****************************************************************************
*
*   MedlineRnAsnWrite(mrp, aip, atp) 
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL MedlineRnAsnWrite (MedlineRnPtr mrp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = TRUE;

	if (! loaded)
	{
		if (! MedlineAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, MEDLINE_RN);
    if (atp == NULL) return FALSE;

	if (mrp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)mrp)) goto erret;
    av.intvalue = (Int4)mrp->type;
    if (! AsnWrite(aip, MEDLINE_RN_type, &av)) goto erret;
    if (mrp->cit != NULL)
    {
        av.ptrvalue = mrp->cit;
        if (! AsnWrite(aip, MEDLINE_RN_cit, &av)) goto erret;
    }
    av.ptrvalue = mrp->name;
    if (! AsnWrite(aip, MEDLINE_RN_name, &av)) goto erret;
    if (! AsnCloseStruct(aip, atp, (Pointer)mrp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);
	return retval;
}

/*****************************************************************************
*
*   MedlineSiFree(msp)
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL MedlineSiFree (ValNodePtr msp)
{
        if (msp != NULL)
        {
                MemFree (msp->data.ptrvalue);
                MemFree (msp);
        }
         
        return NULL; 
}

/*****************************************************************************
*
*   MedlineSiAsnRead(aip, atp)
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL MedlineSiAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	ValNodePtr msp=NULL;
	DataVal av;
	AsnTypePtr atp;

	if (! loaded)
	{
		if (! MedlineAsnLoad())
			return msp;
	}

	if (aip == NULL)
		return msp;

	if (orig == NULL)           /* MedlineSi ::= */
		atp = AsnReadId(aip, amp, MEDLINE_SI);
	else
		atp = AsnLinkType(orig, MEDLINE_SI);
    if (atp == NULL) return msp;

	msp = ValNodeNew(NULL);
    if (msp == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the SEQUENCE */
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;  /* type */
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    msp->choice = (Uint1)av.intvalue;
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (atp == MEDLINE_SI_cit)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        msp->data.ptrvalue = av.ptrvalue;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;  /* end struct */
ret:
	AsnUnlinkType(orig);
	return msp;
erret:
    msp = ValNodeFreeData(msp);
    goto ret;
}

/*****************************************************************************
*
*   MedlineSiAsnWrite(msp, aip, atp) 
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL MedlineSiAsnWrite (ValNodePtr msp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! MedlineAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, MEDLINE_SI);
    if (atp == NULL) return FALSE;

	if (msp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)msp)) goto erret;
    av.intvalue = (Int4)msp->choice;
    if (! AsnWrite(aip, MEDLINE_SI_type, &av)) goto erret;
    if (msp->data.ptrvalue != NULL)
    {
        av.ptrvalue = msp->data.ptrvalue;
        if (! AsnWrite(aip, MEDLINE_SI_cit, &av)) goto erret;
    }
    if (! AsnCloseStruct(aip, atp, (Pointer)msp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);
	return retval;
}

/*****************************************************************************
*
*   MedlineEntryNew()
*
*****************************************************************************/
NLM_EXTERN MedlineEntryPtr LIBCALL MedlineEntryNew (void)
{
	MedlineEntryPtr mep;

	mep = (MedlineEntryPtr)MemNew(sizeof(MedlineEntry));
	return mep;
}

/*****************************************************************************
*
*   MedlineEntryFree()
*
*****************************************************************************/
NLM_EXTERN MedlineEntryPtr LIBCALL MedlineEntryFree (MedlineEntryPtr mep)
{
    MedlineMeshPtr mmp, mmpnext;
    MedlineRnPtr mrp, mrpnext;
	MedlineFieldPtr mfp, mfpnext;

    if (mep == NULL)
        return mep;

    DateFree(mep->em);
    CitArtFree(mep->cit);
    MemFree(mep->abstract);
    mmp = mep->mesh;
    while (mmp != NULL)
    {
        mmpnext = mmp->next;
        MedlineMeshFree(mmp);
        mmp = mmpnext;
    }
    mrp = mep->substance;
    while (mrp != NULL)
    {
        mrpnext = mrp->next;
        MedlineRnFree(mrp);
        mrp = mrpnext;
    }
    ValNodeFreeData(mep->xref);
    ValNodeFreeData(mep->idnum);
    ValNodeFreeData(mep->gene);
	ValNodeFreeData(mep->pub_type);
	for (mfp = mep->mlfield; mfp != NULL; mfp = mfpnext)
	{
		mfpnext = mfp->next;
		MedlineFieldFree(mfp);
	}

	ObjMgrDelete(OBJ_MEDLINE_ENTRY, (Pointer)mep);

	return (MedlineEntryPtr)MemFree(mep);
}

/*****************************************************************************
*
*   MedlineEntryAsnRead(aip, atp)
*
*****************************************************************************/
NLM_EXTERN MedlineEntryPtr LIBCALL MedlineEntryAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	MedlineEntryPtr mep=NULL;
	DataVal av;
	AsnTypePtr atp, oldatp;
    MedlineMeshPtr mmp, mmplast;
    MedlineRnPtr mrp, mrplast;
	MedlineFieldPtr mfp, mfplast;
    ValNodePtr anp, anplast;

	if (! loaded)
	{
		if (! MedlineAsnLoad())
			return mep;
	}

	if (aip == NULL)
		return mep;

	if (orig == NULL)           /* MedlineEntry ::= */
		atp = AsnReadId(aip, amp, MEDLINE_ENTRY);
	else
		atp = AsnLinkType(orig, MEDLINE_ENTRY);
    oldatp = atp;
    if (atp == NULL) return mep;

	mep = MedlineEntryNew();
    if (mep == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the SEQUENCE */
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;  /* uid */
	if (atp == MEDLINE_ENTRY_uid)								  /* but now its optional */
	{
		if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
		mep->uid = av.intvalue;
		atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;  /* entry month */
	}
    mep->em = DateAsnRead(aip, atp);
    if (mep->em == NULL) goto erret;
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* citation */
    mep->cit = CitArtAsnRead(aip, atp);
    if (mep->cit == NULL) goto erret;
    while ((atp = AsnReadId(aip, amp, atp)) != oldatp)
    {
        if (atp == NULL) goto erret;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        if (atp == MEDLINE_ENTRY_abstract)
        {
            mep->abstract = (CharPtr)av.ptrvalue;
        }
		else if (atp == MEDLINE_ENTRY_pmid)
		{
			mep->pmid = av.intvalue;
		}
        else if (atp == MEDLINE_ENTRY_mesh)
        {
            mmplast = NULL;
            while ((atp = AsnReadId(aip, amp, atp)) != MEDLINE_ENTRY_mesh)
            {
                if (atp == NULL) goto erret;
               mmp = MedlineMeshAsnRead(aip, atp);
                if (mmp == NULL) goto erret;
               if (mmplast == NULL)
                    mep->mesh = mmp;
                else
                    mmplast->next = mmp;
                mmplast = mmp;
            }
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end of SET OF */
        }
        else if (atp == MEDLINE_ENTRY_substance)
        {
            mrplast = NULL;
            while ((atp = AsnReadId(aip, amp, atp)) != MEDLINE_ENTRY_substance)
            {
                if (atp == NULL) goto erret;
                mrp = MedlineRnAsnRead(aip, atp);
                if (mrp == NULL) goto erret;
                if (mrplast == NULL)
                    mep->substance = mrp;
                else
                    mrplast->next = mrp;
                mrplast = mrp;
            }
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end SET OF */
        }
        else if (atp == MEDLINE_ENTRY_xref)
        {
            anplast = NULL;
            while ((atp = AsnReadId(aip, amp, atp)) == MEDLINE_ENTRY_xref_E)
            {
                anp = MedlineSiAsnRead(aip, atp);
                if (anp == NULL) goto erret;
                if (mep->xref == NULL)
                    mep->xref = anp;
                else
                    anplast->next = anp;
                anplast = anp;
            }
            if (atp == NULL) goto erret;
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        }
        else if (atp == MEDLINE_ENTRY_idnum)
        {
            anp = NULL;
            while ((atp = AsnReadId(aip, amp, atp)) != MEDLINE_ENTRY_idnum)
            {
                if (atp == NULL) goto erret;
                if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
                anp = ValNodeNew(anp);
                if (anp == NULL) goto erret;
                if (mep->idnum == NULL)
                    mep->idnum = anp;
                anp->data.ptrvalue = av.ptrvalue;   /* VisibleString */
            }
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end SET OF */
        }
        else if (atp == MEDLINE_ENTRY_gene)
        {
            anp = NULL;
            while ((atp = AsnReadId(aip, amp, atp)) != MEDLINE_ENTRY_gene)
            {
                if (atp == NULL) goto erret;
                if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
                anp = ValNodeNew(anp);
                if (anp == NULL) goto erret;
                if (mep->gene == NULL)
                    mep->gene = anp;
                anp->data.ptrvalue = av.ptrvalue;   /* VisibleString */
            }
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end SET OF */
        }
        else if (atp == MEDLINE_ENTRY_pub_type)
        {
            anp = NULL;
            while ((atp = AsnReadId(aip, amp, atp)) != MEDLINE_ENTRY_pub_type)
            {
                if (atp == NULL) goto erret;
                if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
                anp = ValNodeNew(anp);
                if (anp == NULL) goto erret;
                if (mep->pub_type == NULL)
                    mep->pub_type = anp;
                anp->data.ptrvalue = av.ptrvalue;   /* VisibleString */
            }
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end SET OF */
        }
        else if (atp == MEDLINE_ENTRY_mlfield)
        {
            mfplast = NULL;
            while ((atp = AsnReadId(aip, amp, atp)) != MEDLINE_ENTRY_mlfield)
            {
                if (atp == NULL) goto erret;
               mfp = MedlineFieldAsnRead(aip, atp);
                if (mfp == NULL) goto erret;
               if (mfplast == NULL)
                    mep->mlfield = mfp;
                else
                    mfplast->next = mfp;
                mfplast = mfp;
            }
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end of SET OF */
        }
        else if (atp == MEDLINE_ENTRY_status)
        {
	    mep->status = (Uint1)(av.intvalue);
        }
    }

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the END STRUCT */
ret:
	AsnUnlinkType(orig);
	return mep;
erret:
    mep = MedlineEntryFree(mep);
    goto ret;
}

/*****************************************************************************
*
*   MedlineEntryAsnWrite(mep, aip, atp) 
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL MedlineEntryAsnWrite (MedlineEntryPtr mep, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    MedlineMeshPtr mmp;
    MedlineRnPtr mrp;
	MedlineFieldPtr mfp;
    ValNodePtr anp;
    Boolean retval = FALSE, writemuid = FALSE;

	if (! loaded)
	{
		if (! MedlineAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, MEDLINE_ENTRY);
    if (atp == NULL) return FALSE;

	if (mep == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)mep)) goto erret;

	if (mep->uid > 0)	 /* now it is optional */
	{
		writemuid = TRUE;
	}
	else if (aip->spec_version > 0 && aip->spec_version < 5)
	{
		ErrPostEx(SEV_ERROR,0,0,"ASN4: MUID=0 written");
		writemuid = TRUE;
	}

	if (writemuid)
	{
		av.intvalue = mep->uid;
		if (! AsnWrite(aip, MEDLINE_ENTRY_uid, &av)) goto erret;
	}
    if (! DateAsnWrite(mep->em, aip, MEDLINE_ENTRY_em)) goto erret;
    if (! CitArtAsnWrite(mep->cit, aip, MEDLINE_ENTRY_cit)) goto erret;
    if (mep->abstract != NULL)
    {
        av.ptrvalue = mep->abstract;
        if (! AsnWrite(aip, MEDLINE_ENTRY_abstract, &av)) goto erret;
    }
    if (mep->mesh != NULL)
    {
        if (! AsnOpenStruct(aip, MEDLINE_ENTRY_mesh, (Pointer)mep->mesh))
            goto erret;
        mmp = mep->mesh;
        while (mmp != NULL)
        {
            if (! MedlineMeshAsnWrite(mmp, aip, MEDLINE_ENTRY_mesh_E))
                goto erret;
            mmp = mmp->next;
        }
        if (! AsnCloseStruct(aip, MEDLINE_ENTRY_mesh, (Pointer)mep->mesh))
            goto erret;
    }
    if (mep->substance != NULL)
    {
        if (! AsnOpenStruct(aip, MEDLINE_ENTRY_substance, (Pointer)mep->substance))
            goto erret;
        mrp = mep->substance;
        while (mrp != NULL)
        {
            if (! MedlineRnAsnWrite(mrp, aip, MEDLINE_ENTRY_substance_E))
                goto erret;
            mrp = mrp->next;
        }
        if (! AsnCloseStruct(aip, MEDLINE_ENTRY_substance, (Pointer)mep->substance))
            goto erret;
    }
    if (mep->xref != NULL)
    {
        if (! AsnOpenStruct(aip, MEDLINE_ENTRY_xref, (Pointer)mep->xref))
            goto erret;
        anp = mep->xref;
        while (anp != NULL)
        {
            if (! MedlineSiAsnWrite(anp, aip, MEDLINE_ENTRY_xref_E))
                goto erret;
            anp = anp->next;
        }
        if (! AsnCloseStruct(aip, MEDLINE_ENTRY_xref, (Pointer)mep->xref))
            goto erret;
    }
    if (mep->idnum != NULL)
    {
        if (! AsnOpenStruct(aip, MEDLINE_ENTRY_idnum, (Pointer)mep->idnum))
            goto erret;
        anp = mep->idnum;
        while (anp != NULL)
        {
            av.ptrvalue = anp->data.ptrvalue;
            if (! AsnWrite(aip, MEDLINE_ENTRY_idnum_E, &av)) goto erret;
            anp = anp->next;
        }
        if (! AsnCloseStruct(aip, MEDLINE_ENTRY_idnum, (Pointer)mep->idnum))
            goto erret;
    }
    if (mep->gene != NULL)
    {
        if (! AsnOpenStruct(aip, MEDLINE_ENTRY_gene, (Pointer)mep->gene))
            goto erret;
        anp = mep->gene;
        while (anp != NULL)
        {
            av.ptrvalue = anp->data.ptrvalue;
            if (! AsnWrite(aip, MEDLINE_ENTRY_gene_E, &av)) goto erret;
            anp = anp->next;
        }
        if (! AsnCloseStruct(aip, MEDLINE_ENTRY_gene, (Pointer)mep->gene))
            goto erret;
    }
	if (mep->pmid > 0)	 /* PubMedId */
	{
	        if (aip->spec_version > 0 && aip->spec_version < 5)  /* ASN4 strip new value */
		{
			ErrPostEx(SEV_ERROR,0,0,"ASN4: PubMedId stripped");
		}
		else
		{
			av.intvalue = mep->pmid;
			if (! AsnWrite(aip, MEDLINE_ENTRY_pmid, &av)) goto erret;
		}
	}

    if (    mep->pub_type != NULL
         && (aip->spec_version >= 5 || aip->spec_version == 0) ) {
        if (! AsnOpenStruct(aip, MEDLINE_ENTRY_pub_type, (Pointer)mep->pub_type))
            goto erret;
        anp = mep->pub_type;
        while (anp != NULL)
        {
            av.ptrvalue = anp->data.ptrvalue;
            if (! AsnWrite(aip, MEDLINE_ENTRY_pub_type_E, &av)) goto erret;
            anp = anp->next;
        }
        if (! AsnCloseStruct(aip, MEDLINE_ENTRY_pub_type, (Pointer)mep->pub_type))
            goto erret;
    }

    if (    mep->mlfield != NULL
         && (aip->spec_version >= 6 || aip->spec_version == 0) ) {
        if (! AsnOpenStruct(aip, MEDLINE_ENTRY_mlfield, (Pointer)mep->mlfield))
            goto erret;
        mfp = mep->mlfield;
        while (mfp != NULL)
        {
			if (! MedlineFieldAsnWrite(mfp, aip, MEDLINE_ENTRY_mlfield_E))
				goto erret;
			mfp = mfp->next;
        }
        if (! AsnCloseStruct(aip, MEDLINE_ENTRY_mlfield, (Pointer)mep->mlfield))
            goto erret;
    }

	if (mep->status > 0)	 /* medline processing status level */
	{
		if ((aip->spec_version == 0) || (aip->spec_version >= 6))
		{
			av.intvalue = (Int4)(mep->status);
			if (! AsnWrite(aip, MEDLINE_ENTRY_status, &av)) goto erret;
		}
	}

    if (! AsnCloseStruct(aip, atp, (Pointer)mep)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);
	return retval;
}

/*****************************************************************************
*
*   MedlineFieldNew()
*
*****************************************************************************/
NLM_EXTERN MedlineFieldPtr LIBCALL MedlineFieldNew (void)
{
	MedlineFieldPtr mfp;

	mfp = (MedlineFieldPtr)MemNew(sizeof(MedlineField));
	return mfp;
}

/*****************************************************************************
*
*   MedlineFieldFree()
*
*****************************************************************************/
NLM_EXTERN MedlineFieldPtr LIBCALL MedlineFieldFree (MedlineFieldPtr mfp)
{
	DocRefPtr dfp, next;
    if (mfp == NULL)
        return mfp;

    MemFree(mfp->str);
	for (dfp = mfp->ids; dfp != NULL; dfp = next)
	{
		next = dfp->next;
		DocRefFree(dfp);
	}
    
	return (MedlineFieldPtr)MemFree(mfp);
}

/*****************************************************************************
*
*   MedlineFieldAsnRead(aip, atp)
*
*****************************************************************************/
NLM_EXTERN MedlineFieldPtr LIBCALL MedlineFieldAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	MedlineFieldPtr mfp=NULL;
	DataVal av;
	AsnTypePtr atp, oldatp;
	DocRefPtr drp, prev = NULL;

	if (! loaded)
	{
		if (! MedlineAsnLoad())
			return mfp;
	}

	if (aip == NULL)
		return mfp;

	if (orig == NULL)           /* MedlineField ::= */
		atp = AsnReadId(aip, amp, MEDLINE_FIELD);
	else
		atp = AsnLinkType(orig, MEDLINE_FIELD);
    if (atp == NULL) return mfp;
	oldatp = atp;

	mfp = MedlineFieldNew();
    if (mfp == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the SEQUENCE */
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;  /* type */
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    mfp->type = (Uint1)av.intvalue;
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    mfp->str = (CharPtr)av.ptrvalue;

    while ((atp = AsnReadId(aip, amp, atp)) != oldatp)
	{
		if (atp == NULL) goto erret;
		if (atp == MEDLINE_FIELD_ids_E)
		{
			drp = DocRefAsnRead(aip, atp);
			if (prev == NULL)
				mfp->ids = drp;
			else
				prev->next = drp;
			prev = drp;
		}
		else
			if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
	}
	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the END STRUCT */
ret:
	AsnUnlinkType(orig);
	return mfp;
erret:
    mfp = MedlineFieldFree(mfp);
    goto ret;
}

/*****************************************************************************
*
*   MedlineFieldAsnWrite(mfp, aip, atp) 
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL MedlineFieldAsnWrite (MedlineFieldPtr mfp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = TRUE;
	DocRefPtr drp;

	if (! loaded)
	{
		if (! MedlineAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, MEDLINE_FIELD);
    if (atp == NULL) return FALSE;

	if (mfp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)mfp)) goto erret;
    av.intvalue = (Int4)mfp->type;
    if (! AsnWrite(aip, MEDLINE_FIELD_type, &av)) goto erret;
    av.ptrvalue = mfp->str;
    if (! AsnWrite(aip, MEDLINE_FIELD_str, &av)) goto erret;
	if (mfp->ids != NULL)
	{
		if (! AsnOpenStruct(aip, MEDLINE_FIELD_ids, (Pointer)mfp->ids)) goto erret;
		for (drp = mfp->ids; drp != NULL; drp = drp->next)
		{
			if (! DocRefAsnWrite(drp, aip, MEDLINE_FIELD_ids_E))
				goto erret;
		}
		if (! AsnCloseStruct(aip, MEDLINE_FIELD_ids, (Pointer)mfp->ids)) goto erret;
	}
    if (! AsnCloseStruct(aip, atp, (Pointer)mfp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);
	return retval;
}


/*****************************************************************************
*
*   DocRefNew()
*
*****************************************************************************/
NLM_EXTERN DocRefPtr LIBCALL DocRefNew (void)
{
	DocRefPtr drp;

	drp = (DocRefPtr)MemNew(sizeof(DocRef));
	return drp;
}

/*****************************************************************************
*
*   DocRefFree()
*
*****************************************************************************/
NLM_EXTERN DocRefPtr LIBCALL DocRefFree (DocRefPtr drp)
{
    if (drp == NULL)
        return drp;

	return (DocRefPtr)MemFree(drp);
}

/*****************************************************************************
*
*   DocRefAsnRead(aip, atp)
*
*****************************************************************************/
NLM_EXTERN DocRefPtr LIBCALL DocRefAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DocRefPtr drp=NULL;
	DataVal av;
	AsnTypePtr atp;

	if (! loaded)
	{
		if (! MedlineAsnLoad())
			return drp;
	}

	if (aip == NULL)
		return drp;

	if (orig == NULL)           /* DocRef ::= */
		atp = AsnReadId(aip, amp, DOCREF);
	else
		atp = AsnLinkType(orig, DOCREF);
    if (atp == NULL) return drp;

	drp = DocRefNew();
    if (drp == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the SEQUENCE */
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;  /* type */
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    drp->type = (Uint1)av.intvalue;
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    drp->uid = av.intvalue;

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the END STRUCT */
ret:
	AsnUnlinkType(orig);
	return drp;
erret:
    drp = DocRefFree(drp);
    goto ret;
}

/*****************************************************************************
*
*   DocRefAsnWrite(drp, aip, atp) 
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL DocRefAsnWrite (DocRefPtr drp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = TRUE;

	if (! loaded)
	{
		if (! MedlineAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, DOCREF);
    if (atp == NULL) return FALSE;

	if (drp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)drp)) goto erret;
    av.intvalue = (Int4)drp->type;
    if (! AsnWrite(aip, DOCREF_type, &av)) goto erret;
    av.intvalue = (Int4)drp->uid;
    if (! AsnWrite(aip, DOCREF_uid, &av)) goto erret;
    if (! AsnCloseStruct(aip, atp, (Pointer)drp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);
	return retval;
}


