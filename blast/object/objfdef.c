/*  objfdef.c
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
* File Name:  objfdef.c
*
* Author:  James Ostell
*   
* Version Creation Date: 9/94
*
* $Revision: 6.21 $
*
* File Description:  Object manager for feature definitions
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: objfdef.c,v 
 * Revision 4.4  1995/09/21  02:26:14  ostel
 * changed label for RNA to keep typelabel in most case
 *
 * Revision 4.3  1995/09/06  22:48:12  kans
 * improved tRNA label function (JO)
 *
 * Revision 4.2  1995/09/06  22:08:56  ostell
 * revision to tRNA label function
 *
 * Revision 4.1  1995/08/30  19:13:28  ostell
 * change LabelContent so Imp-feat get ifp->key even if ONLY_CONTENT
 *
 * Revision 4.0  1995/07/26  13:48:06  ostell
 * force revision to 4.0
 *
 * Revision 1.11  1995/07/20  14:29:51  kans
 * FEATDEF_otherRNA corrected from 7 to 255
 *
 * Revision 1.10  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/
#include <objfdef.h>		   /* the features interface */
#include <asnfdef.h>        /* the AsnTool header */
#include <sequtil.h>
#include <explore.h>  /* new public location of SeqMgrGetBestProteinFeature */

static Boolean loaded = FALSE;

/*****************************************************************************
*
*   FeatDefAsnLoad()
*      requires SeqAsnLoad() to be called first
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL FeatDefAsnLoad (void)
{
    if (loaded)
        return TRUE;
    loaded = TRUE;

    if ( ! AsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************
*
*   FeatDef Routines
*
*****************************************************************************/
/*****************************************************************************
*
*   FeatDefNew()
*
*****************************************************************************/
NLM_EXTERN FeatDefPtr LIBCALL FeatDefNew (void)
{
    return (FeatDefPtr)MemNew(sizeof(FeatDef));
}

/*****************************************************************************
*
*   FeatDefFree(fdp)
*       Frees one FeatDef and associated data
*
*****************************************************************************/
NLM_EXTERN FeatDefPtr LIBCALL FeatDefFree (FeatDefPtr fdp)
{
    if (fdp == NULL)
        return (FeatDefPtr)NULL;

	MemFree(fdp->typelabel);
	MemFree(fdp->menulabel);
	return (FeatDefPtr)MemFree(fdp);
}

/*****************************************************************************
*
*   FeatDefAsnWrite(fdp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (FeatDef ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL FeatDefAsnWrite (FeatDefPtr fdp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! FeatDefAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, FEATDEF);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (fdp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)fdp))
        goto erret;

	av.ptrvalue = fdp->typelabel;
	if (! AsnWrite(aip, FEATDEF_typelabel, &av)) goto erret;

	av.ptrvalue = fdp->menulabel;
	if (! AsnWrite(aip, FEATDEF_menulabel, &av)) goto erret;

	av.intvalue = (Int4)(fdp->featdef_key);
	if (! AsnWrite(aip, FEATDEF_featdef_key, &av)) goto erret;

	av.intvalue = (Int4)(fdp->seqfeat_key);
	if (! AsnWrite(aip, FEATDEF_seqfeat_key, &av)) goto erret;

	av.intvalue = (Int4)(fdp->entrygroup);
	if (! AsnWrite(aip, FEATDEF_entrygroup, &av)) goto erret;

	av.intvalue = (Int4)(fdp->displaygroup);
	if (! AsnWrite(aip, FEATDEF_displaygroup, &av)) goto erret;

	av.intvalue = (Int4)(fdp->molgroup);
	if (! AsnWrite(aip, FEATDEF_molgroup, &av)) goto erret;

    if (! AsnCloseStruct(aip, atp, (Pointer)fdp))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   FeatDefAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN FeatDefPtr LIBCALL FeatDefAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldtype;
    FeatDefPtr fdp;

	if (! loaded)
	{
		if (! FeatDefAsnLoad())
			return (FeatDefPtr)NULL;
	}

	if (aip == NULL)
		return (FeatDefPtr)NULL;

	if (orig == NULL)           /* FeatDef ::= (self contained) */
		atp = AsnReadId(aip, amp, FEATDEF);
	else
		atp = AsnLinkType(orig, FEATDEF);    /* link in local tree */
    oldtype = atp;
    if (atp == NULL)
        return (FeatDefPtr)NULL;

	fdp = FeatDefNew();
    if (fdp == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the start struct */
    while ((atp = AsnReadId(aip, amp, atp)) != oldtype)
    {
        if (atp == NULL) goto erret;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        if (atp == FEATDEF_typelabel)
            fdp->typelabel = (CharPtr)av.ptrvalue;
        else if (atp == FEATDEF_menulabel)
            fdp->menulabel = (CharPtr)av.ptrvalue;
        else if (atp == FEATDEF_featdef_key)
            fdp->featdef_key = (Uint1)(av.intvalue);
        else if (atp == FEATDEF_seqfeat_key)
            fdp->seqfeat_key = (Uint1)(av.intvalue);
        else if (atp == FEATDEF_entrygroup)
            fdp->entrygroup = (Uint1)(av.intvalue);
        else if (atp == FEATDEF_displaygroup)
            fdp->displaygroup = (Uint1)(av.intvalue);
        else if (atp == FEATDEF_molgroup)
            fdp->molgroup = (Uint1)(av.intvalue);
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return fdp;
erret:
    fdp = FeatDefFree(fdp);
    goto ret;
}

/*****************************************************************************
*
*   FeatDefSetFree(fdp)
*       Frees set of FeatDef
*
*****************************************************************************/
NLM_EXTERN FeatDefPtr LIBCALL FeatDefSetFree (FeatDefPtr fdp)
{
	FeatDefPtr next;

    while (fdp != NULL)
	{
		next = fdp->next;
		FeatDefFree(fdp);
		fdp = next;
	}
    return (FeatDefPtr)NULL;
}

/*****************************************************************************
*
*   FeatDefSetAsnWrite(fdp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (FeatDef ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL FeatDefSetAsnWrite (FeatDefPtr fdp, AsnIoPtr aip, AsnTypePtr orig)
{
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! FeatDefAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, FEATDEFSET);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (fdp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)fdp))
        goto erret;

	while (fdp != NULL)
	{
		if (! FeatDefAsnWrite(fdp, aip, FEATDEFSET_E))
			goto erret;
		fdp = fdp->next;
	}

    if (! AsnCloseStruct(aip, atp, (Pointer)fdp))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   FeatDefSetAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN FeatDefPtr LIBCALL FeatDefSetAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldtype;
    FeatDefPtr fdp, first=NULL, last=NULL;

	if (! loaded)
	{
		if (! FeatDefAsnLoad())
			return (FeatDefPtr)NULL;
	}

	if (aip == NULL)
		return (FeatDefPtr)NULL;

	if (orig == NULL)           /* FeatDef ::= (self contained) */
		atp = AsnReadId(aip, amp, FEATDEFSET);
	else
		atp = AsnLinkType(orig, FEATDEFSET);    /* link in local tree */
    oldtype = atp;
    if (atp == NULL)
        return (FeatDefPtr)NULL;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the start struct */
    while ((atp = AsnReadId(aip, amp, atp)) != oldtype)
    {
        if (atp == NULL) goto erret;
		fdp = FeatDefAsnRead(aip, atp);
		if (fdp == NULL) goto erret;
		if (first == NULL) first = fdp;
		if (last != NULL) last->next = fdp;
		last = fdp;
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return first;
erret:
    first = FeatDefSetFree(first);
    goto ret;
}

/*****************************************************************************
*
*   FeatDispGroup Routines
*
*****************************************************************************/
/*****************************************************************************
*
*   FeatDispGroupNew()
*
*****************************************************************************/
NLM_EXTERN FeatDispGroupPtr LIBCALL FeatDispGroupNew (void)
{
    return (FeatDispGroupPtr)MemNew(sizeof(FeatDispGroup));
}

/*****************************************************************************
*
*   FeatDispGroupFree(fdp)
*       Frees one FeatDispGroup and associated data
*
*****************************************************************************/
NLM_EXTERN FeatDispGroupPtr LIBCALL FeatDispGroupFree (FeatDispGroupPtr fdp)
{
    if (fdp == NULL)
        return (FeatDispGroupPtr)NULL;

	MemFree(fdp->groupname);
	return (FeatDispGroupPtr)MemFree(fdp);
}

/*****************************************************************************
*
*   FeatDispGroupAsnWrite(fdp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (FeatDispGroup ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL FeatDispGroupAsnWrite (FeatDispGroupPtr fdp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! FeatDefAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, FEATDISPGROUP);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (fdp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)fdp))
        goto erret;

	av.intvalue = (Int4)(fdp->groupkey);
	if (! AsnWrite(aip, FEATDISPGROUP_groupkey, &av)) goto erret;

	av.ptrvalue = fdp->groupname;
	if (! AsnWrite(aip, FEATDISPGROUP_groupname, &av)) goto erret;

    if (! AsnCloseStruct(aip, atp, (Pointer)fdp))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   FeatDispGroupAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN FeatDispGroupPtr LIBCALL FeatDispGroupAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldtype;
    FeatDispGroupPtr fdp;

	if (! loaded)
	{
		if (! FeatDefAsnLoad())
			return (FeatDispGroupPtr)NULL;
	}

	if (aip == NULL)
		return (FeatDispGroupPtr)NULL;

	if (orig == NULL)           /* FeatDispGroup ::= (self contained) */
		atp = AsnReadId(aip, amp, FEATDISPGROUP);
	else
		atp = AsnLinkType(orig, FEATDISPGROUP);    /* link in local tree */
    oldtype = atp;
    if (atp == NULL)
        return (FeatDispGroupPtr)NULL;

	fdp = FeatDispGroupNew();
    if (fdp == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the start struct */
    while ((atp = AsnReadId(aip, amp, atp)) != oldtype)
    {
        if (atp == NULL) goto erret;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        if (atp == FEATDISPGROUP_groupname)
            fdp->groupname = (CharPtr)av.ptrvalue;
        else if (atp == FEATDISPGROUP_groupkey)
            fdp->groupkey = (Uint1)(av.intvalue);
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return fdp;
erret:
    fdp = FeatDispGroupFree(fdp);
    goto ret;
}

/*****************************************************************************
*
*   FeatDispGroupSetFree(fdp)
*       Frees set of FeatDispGroup
*
*****************************************************************************/
NLM_EXTERN FeatDispGroupPtr LIBCALL FeatDispGroupSetFree (FeatDispGroupPtr fdp)
{
	FeatDispGroupPtr next;

    while (fdp != NULL)
	{
		next = fdp->next;
		FeatDispGroupFree(fdp);
		fdp = next;
	}
    return (FeatDispGroupPtr)NULL;
}

/*****************************************************************************
*
*   FeatDispGroupSetAsnWrite(fdp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (FeatDispGroup ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL FeatDispGroupSetAsnWrite (FeatDispGroupPtr fdp, AsnIoPtr aip, AsnTypePtr orig)
{
	AsnTypePtr atp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! FeatDefAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, FEATDISPGROUPSET);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (fdp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)fdp))
        goto erret;

	while (fdp != NULL)
	{
		if (! FeatDispGroupAsnWrite(fdp, aip, FEATDISPGROUPSET_E))
			goto erret;
		fdp = fdp->next;
	}

    if (! AsnCloseStruct(aip, atp, (Pointer)fdp))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   FeatDispGroupSetAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occured
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occured.
*
*****************************************************************************/
NLM_EXTERN FeatDispGroupPtr LIBCALL FeatDispGroupSetAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldtype;
    FeatDispGroupPtr fdp, first=NULL, last=NULL;

	if (! loaded)
	{
		if (! FeatDefAsnLoad())
			return (FeatDispGroupPtr)NULL;
	}

	if (aip == NULL)
		return (FeatDispGroupPtr)NULL;

	if (orig == NULL)           /* FeatDispGroup ::= (self contained) */
		atp = AsnReadId(aip, amp, FEATDISPGROUPSET);
	else
		atp = AsnLinkType(orig, FEATDISPGROUPSET);    /* link in local tree */
    oldtype = atp;
    if (atp == NULL)
        return (FeatDispGroupPtr)NULL;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the start struct */
    while ((atp = AsnReadId(aip, amp, atp)) != oldtype)
    {
        if (atp == NULL) goto erret;
		fdp = FeatDispGroupAsnRead(aip, atp);
		if (fdp == NULL) goto erret;
		if (first == NULL) first = fdp;
		if (last != NULL) last->next = fdp;
		last = fdp;
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return first;
erret:
    first = FeatDispGroupSetFree(first);
    goto ret;
}

static FeatDefPtr featdefp = NULL;
static FeatDefPtr PNTR featdeflookup;  /* kludge which assumes featdef_key in order */
static Int2 numfdef, numfdispg;
static FeatDispGroupPtr featdgp;

/*****************************************************************************
*
*   featDefSetMemStr as last resort embedded version of featdef.prt
*
*****************************************************************************/

#ifndef WIN16
static CharPtr featDefSetMemStr = "FeatDefGroupSet ::= {\n" \
"groups {\n" \
"{ groupkey 0 , groupname \"Unrecognized Features\" } ,\n" \
"{ groupkey 1 , groupname \"Genes and Named Regions\" } ,\n" \
"{ groupkey 2 , groupname \"Coding Regions and Transcripts\" } ,\n" \
"{ groupkey 3 , groupname \"Structural RNAs\" } ,\n" \
"{ groupkey 4 , groupname \"Bibliographic and Comments\" } ,\n" \
"{ groupkey 5 , groupname \"Sites and Bonds\" } } ,\n" \
"defs {\n" \
"{ typelabel \"???\" , menulabel \"Unsupported feature\" , featdef-key 0 , seqfeat-key 0 , entrygroup 0 , displaygroup 0 , molgroup both } ,\n" \
"{ typelabel \"Gene\" , menulabel \"Gene\" , featdef-key 1 , seqfeat-key 1 , entrygroup 1 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"Org\" , menulabel \"Organism\" , featdef-key 2 , seqfeat-key 2 , entrygroup 4 , displaygroup 4 , molgroup both } ,\n" \
"{ typelabel \"CDS\" , menulabel \"Coding Region\" , featdef-key 3 , seqfeat-key 3 , entrygroup 2 , displaygroup 2 , molgroup na } ,\n" \
"{ typelabel \"Prot\" , menulabel \"Protein Names\" , featdef-key 4 , seqfeat-key 4 , entrygroup 1 , displaygroup 1 , molgroup aa } ,\n" \
"{ typelabel \"preRNA\" , menulabel \"Precursor RNA\" , featdef-key 5 , seqfeat-key 5 , entrygroup 2 , displaygroup 2 , molgroup na } ,\n" \
"{ typelabel \"mRNA\" , menulabel \"Mature Messenger RNA\" , featdef-key 6 , seqfeat-key 5 , entrygroup 2 , displaygroup 2 , molgroup na } ,\n" \
"{ typelabel \"tRNA\" , menulabel \"Transfer RNA\" , featdef-key 7 , seqfeat-key 5 , entrygroup 3 , displaygroup 3 , molgroup na } ,\n" \
"{ typelabel \"rRNA\" , menulabel \"Ribosomal RNA\" , featdef-key 8 , seqfeat-key 5 , entrygroup 3 , displaygroup 3 , molgroup na } ,\n" \
"{ typelabel \"snRNA\" , menulabel \"Small Nuclear RNA\" , featdef-key 9 , seqfeat-key 5 , entrygroup 3 , displaygroup 3 , molgroup na } ,\n" \
"{ typelabel \"scRNA\" , menulabel \"Small Cytoplasmic RNA\" , featdef-key 10 , seqfeat-key 5 , entrygroup 3 , displaygroup 3 , molgroup na } ,\n" \
"{ typelabel \"RNA\" , menulabel \"Other Types of RNA\" , featdef-key 11 , seqfeat-key 5 , entrygroup 3 , displaygroup 3 , molgroup na } ,\n" \
"{ typelabel \"Cit\" , menulabel \"Bibliographic Citations\" , featdef-key 12 , seqfeat-key 6 , entrygroup 4 , displaygroup 4 , molgroup both } ,\n" \
"{ typelabel \"Xref\" , menulabel \"Reference to Another Sequence\" , featdef-key 13 , seqfeat-key 7 , entrygroup 4 , displaygroup 4 , molgroup both } ,\n" \
"{ typelabel \"Imp\" , menulabel \"Unclassified ImpFeat\" , featdef-key 14 , seqfeat-key 8 , entrygroup 0 , displaygroup 0 , molgroup na } ,\n" \
"{ typelabel \"allele\" , menulabel \"Allelic Varient\" , featdef-key 15 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"attenuator\" , menulabel \"Attenuator\" , featdef-key 16 , seqfeat-key 8 , entrygroup 0 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"C_region\" , menulabel \"Immunoglobin/T-cell receptor constant region\" , featdef-key 17 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"CAAT_signal\" , menulabel \"CAAT box\" , featdef-key 18 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"CDS\" , menulabel \"Untranslatable coding region\" , featdef-key 19 , seqfeat-key 8 , entrygroup 0 , displaygroup 4 , molgroup na } ,\n" \
"{ typelabel \"conflict\" , menulabel \"Sequence determinations differ\" , featdef-key 20 , seqfeat-key 8 , entrygroup 0 , displaygroup 4 , molgroup na } ,\n" \
"{ typelabel \"D-loop\" , menulabel \"Displacement Loop\" , featdef-key 21 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"D_segment\" , menulabel \"Diversity Segment of Immunoglobin\" , featdef-key 22 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"enhancer\" , menulabel \"Enhancer\" , featdef-key 23 , seqfeat-key 8 , entrygroup 0 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"exon\" , menulabel \"Exon\" , featdef-key 24 , seqfeat-key 8 , entrygroup 2 , displaygroup 2 , molgroup na } ,\n" \
"{ typelabel \"GC_signal\" , menulabel \"GC box\" , featdef-key 25 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"iDNA\" , menulabel \"intervening DNA\" , featdef-key 26 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"intron\" , menulabel \"Intron\" , featdef-key 27 , seqfeat-key 8 , entrygroup 2 , displaygroup 2 , molgroup na } ,\n" \
"{ typelabel \"J_segment\" , menulabel \"IG joining segment\" , featdef-key 28 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"LTR\" , menulabel \"Long Terminal Repeat\" , featdef-key 29 , seqfeat-key 8 , entrygroup 1 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"mat_peptide\" , menulabel \"Mature Peptide labeled on Nuc Acid\" , featdef-key 30 , seqfeat-key 8 , entrygroup 0 , displaygroup 2 , molgroup na } ,\n" \
"{ typelabel \"misc_binding\" , menulabel \"Miscellaneaous binding site\" , featdef-key 31 , seqfeat-key 8 , entrygroup 0 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"misc_difference\" , menulabel \"Miscellaneous sequence difference\" , featdef-key 32 , seqfeat-key 8 , entrygroup 0 , displaygroup 4 , molgroup na } ,\n" \
"{ typelabel \"misc_feature\" , menulabel \"Miscellaneaous feature\" , featdef-key 33 , seqfeat-key 8 , entrygroup 0 , displaygroup 4 , molgroup na } ,\n" \
"{ typelabel \"misc_recomb\" , menulabel \"Miscellaneous recombination\" , featdef-key 34 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"misc_RNA\" , menulabel \"Miscellaneous RNA\" , featdef-key 35 , seqfeat-key 8 , entrygroup 0 , displaygroup 3 , molgroup na } ,\n" \
"{ typelabel \"misc_signal\" , menulabel \"Miscellaneous signal sequence\" , featdef-key 36 , seqfeat-key 8 , entrygroup 0 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"misc_structure\" , menulabel \"Miscellaneous secondary structure\" , featdef-key 37 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"modified_base\" , menulabel \"Modified base\" , featdef-key 38 , seqfeat-key 8 , entrygroup 0 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"mutation\" , menulabel \"site of mutation in related strain\" , featdef-key 39 , seqfeat-key 8 , entrygroup 0 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"N_region\" , menulabel \"Extra na in rearranged IG\" , featdef-key 40 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"old_sequence\" , menulabel \"Location of sequence revision\" , featdef-key 41 , seqfeat-key 8 , entrygroup 0 , displaygroup 4 , molgroup na } ,\n" \
"{ typelabel \"polyA_signal\" , menulabel \"PolyA addition recognition signal\" , featdef-key 42 , seqfeat-key 8 , entrygroup 5 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"polyA_site\" , menulabel \"Point where polyA tail begins\" , featdef-key 43 , seqfeat-key 8 , entrygroup 5 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"precursor_RNA\" , menulabel \"RNA which is a post-transcriptional intermediate\" , featdef-key 44 , seqfeat-key 8 , entrygroup 0 , displaygroup 2 , molgroup na } ,\n" \
"{ typelabel \"prim_transcript\" , menulabel \"Primary transcript\" , featdef-key 45 , seqfeat-key 8 , entrygroup 0 , displaygroup 2 , molgroup na } ,\n" \
"{ typelabel \"primer_bind\" , menulabel \"Primer binding site\" , featdef-key 46 , seqfeat-key 8 , entrygroup 5 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"promoter\" , menulabel \"Promoter\" , featdef-key 47 , seqfeat-key 8 , entrygroup 0 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"protein_bind\" , menulabel \"Protein binding site\" , featdef-key 48 , seqfeat-key 8 , entrygroup 5 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"RBS\" , menulabel \"Ribosome binding site\" , featdef-key 49 , seqfeat-key 8 , entrygroup 5 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"repeat_region\" , menulabel \"Region containing repeats\" , featdef-key 50 , seqfeat-key 8 , entrygroup 1 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"repeat_unit\" , menulabel \"Single repeat unit\" , featdef-key 51 , seqfeat-key 8 , entrygroup 1 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"rep_origin\" , menulabel \"Origin of replication\" , featdef-key 52 , seqfeat-key 8 , entrygroup 5 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"S_region\" , menulabel \"IG switch region\" , featdef-key 53 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"satellite\" , menulabel \"satellite repeat region\" , featdef-key 54 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"sig_peptide\" , menulabel \"Signal Peptide annotated on Nuc Acid\" , featdef-key 55 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"source\" , menulabel \"Source of Nuc Acid\" , featdef-key 56 , seqfeat-key 8 , entrygroup 0 , displaygroup 4 , molgroup na } ,\n" \
"{ typelabel \"stem_loop\" , menulabel \"Stem loop structure\" , featdef-key 57 , seqfeat-key 8 , entrygroup 1 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"STS\" , menulabel \"Sequence tagged site\" , featdef-key 58 , seqfeat-key 8 , entrygroup 1 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"TATA_signal\" , menulabel \"TATA box\" , featdef-key 59 , seqfeat-key 8 , entrygroup 5 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"terminator\" , menulabel \"Transcription terminator\" , featdef-key 60 , seqfeat-key 8 , entrygroup 5 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"transit_peptide\" , menulabel \"Transit peptide annotated on Nuc Acid\" , featdef-key 61 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"unsure\" , menulabel \"Unsure of exact sequence\" , featdef-key 62 , seqfeat-key 8 , entrygroup 0 , displaygroup 4 , molgroup na } ,\n" \
"{ typelabel \"V_region\" , menulabel \"IG variable region\" , featdef-key 63 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"V_segment\" , menulabel \"IG variable segment\" , featdef-key 64 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"variation\" , menulabel \"Strain differences\" , featdef-key 65 , seqfeat-key 8 , entrygroup 0 , displaygroup 4 , molgroup na } ,\n" \
"{ typelabel \"virion\" , menulabel \"Viral genome\" , featdef-key 66 , seqfeat-key 8 , entrygroup 0 , displaygroup 4 , molgroup na } ,\n" \
"{ typelabel \"3'clip\" , menulabel \"3' region of transcript clipped off in processing\" , featdef-key 67 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"3'UTR\" , menulabel \"3' untranslated region\" , featdef-key 68 , seqfeat-key 8 , entrygroup 2 , displaygroup 2 , molgroup na } ,\n" \
"{ typelabel \"5'clip\" , menulabel \"5' region of transcript clipped off in processing\" , featdef-key 69 , seqfeat-key 8 , entrygroup 0 , displaygroup 1 , molgroup na } ,\n" \
"{ typelabel \"5'UTR\" , menulabel \"5' untranslated region\" , featdef-key 70 , seqfeat-key 8 , entrygroup 2 , displaygroup 2 , molgroup na } ,\n" \
"{ typelabel \"-10_signal\" , menulabel \"Pribnow box\" , featdef-key 71 , seqfeat-key 8 , entrygroup 5 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"-35_signal\" , menulabel \"-35 region of transcription start\" , featdef-key 72 , seqfeat-key 8 , entrygroup 5 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"Site-ref\" , menulabel \"SITES type reference\" , featdef-key 73 , seqfeat-key 8 , entrygroup 0 , displaygroup 4 , molgroup na } ,\n" \
"{ typelabel \"Region\" , menulabel \"Named region of sequence\" , featdef-key 74 , seqfeat-key 9 , entrygroup 1 , displaygroup 1 , molgroup both } ,\n" \
"{ typelabel \"Comment\" , menulabel \"Comment associated with sequence\" , featdef-key 75 , seqfeat-key 10 , entrygroup 4 , displaygroup 4 , molgroup both } ,\n" \
"{ typelabel \"Bond\" , menulabel \"Chemical bonds\" , featdef-key 76 , seqfeat-key 11 , entrygroup 5 , displaygroup 5 , molgroup aa } ,\n" \
"{ typelabel \"Site\" , menulabel \"Binding/Active Sites\" , featdef-key 77 , seqfeat-key 12 , entrygroup 5 , displaygroup 5 , molgroup both } ,\n" \
"{ typelabel \"Rsite\" , menulabel \"Restriction Sites\" , featdef-key 78 , seqfeat-key 13 , entrygroup 5 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"User\" , menulabel \"User Defined feature\" , featdef-key 79 , seqfeat-key 14 , entrygroup 0 , displaygroup 4 , molgroup both } ,\n" \
"{ typelabel \"TxInit\" , menulabel \"Transcription Initiation Site\" , featdef-key 80 , seqfeat-key 15 , entrygroup 5 , displaygroup 5 , molgroup na } ,\n" \
"{ typelabel \"Num\" , menulabel \"Numbering System for Sequence\" , featdef-key 81 , seqfeat-key 16 , entrygroup 4 , displaygroup 4 , molgroup both } ,\n" \
"{ typelabel \"SecStr\" , menulabel \"Protein Secondary Structure\" , featdef-key 82 , seqfeat-key 17 , entrygroup 1 , displaygroup 1 , molgroup aa } ,\n" \
"{ typelabel \"NonStdRes\" , menulabel \"Non-standard Residue\" , featdef-key 83 , seqfeat-key 18 , entrygroup 5 , displaygroup 5 , molgroup aa } ,\n" \
"{ typelabel \"Het\" , menulabel \"Heterogen\" , featdef-key 84 , seqfeat-key 19 , entrygroup 5 , displaygroup 5 , molgroup aa } ,\n" \
"{ typelabel \"Src\" , menulabel \"Biological Source\" , featdef-key 85 , seqfeat-key 20 , entrygroup 4 , displaygroup 4 , molgroup both } ,\n" \
"{ typelabel \"proprotein\" , menulabel \"Proprotein\" , featdef-key 86 , seqfeat-key 4 , entrygroup 1 , displaygroup 1 , molgroup aa } ,\n" \
"{ typelabel \"mat_peptide\" , menulabel \"Mature Peptide\" , featdef-key 87 , seqfeat-key 4 , entrygroup 1 , displaygroup 1 , molgroup aa } ,\n" \
"{ typelabel \"sig_peptide\" , menulabel \"Signal Peptide\" , featdef-key 88 , seqfeat-key 4 , entrygroup 1 , displaygroup 1 , molgroup aa } ,\n" \
"{ typelabel \"transit_peptide\" , menulabel \"Transit Peptide\" , featdef-key 89 , seqfeat-key 4 , entrygroup 1 , displaygroup 1 , molgroup aa } ,\n" \
"{ typelabel \"snoRNA\" , menulabel \"Small Nucleolar RNA\" , featdef-key 90 , seqfeat-key 5 , entrygroup 3 , displaygroup 3 , molgroup na } } };\n";
#endif

/*****************************************************************************
*
*   FeatDefPtr FeatDefSetLoad()
*       loads all current feature defintions
*       looks for "featdef.val" in the "data" directory
*
*****************************************************************************/
static FeatDefPtr LIBCALL FeatDefGroupSetAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	FeatDefPtr fdp;
	FeatDispGroupPtr fdgp;
	Int2 i;
	DataVal av;
	AsnTypePtr atp, oldtype;

	if (! loaded)
	{
		if (! FeatDefAsnLoad())
			return (FeatDefPtr)NULL;
	}

	if (aip == NULL)
		return (FeatDefPtr)NULL;

	if (orig == NULL)           /* FeatDefGroupSet ::= (self contained) */
		atp = AsnReadId(aip, amp, FEATDEFGROUPSET);
	else
		atp = AsnLinkType(orig, FEATDEFGROUPSET);    /* link in local tree */
    oldtype = atp;
    if (atp == NULL)
        return (FeatDefPtr)NULL;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the start struct */
    while ((atp = AsnReadId(aip, amp, atp)) != oldtype)
    {
        if (atp == NULL) goto erret;
		if (atp == FEATDEFGROUPSET_groups)
		{
			featdgp = FeatDispGroupSetAsnRead(aip, atp);
			if (featdgp == NULL) goto erret;
		}
		else if (atp == FEATDEFGROUPSET_defs)
		{
			featdefp = FeatDefSetAsnRead(aip, atp);
			if (featdefp == NULL) goto erret;
		}
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end struct */

	for (numfdef = 0, fdp = featdefp; fdp != NULL; fdp = fdp->next)
		numfdef++;

	featdeflookup = MemNew((size_t)(sizeof(FeatDefPtr) * numfdef));

	for (i = 0, fdp = featdefp; i < numfdef; fdp = fdp->next, i++)
		featdeflookup[i] = fdp;

	for (numfdispg = 0, fdgp = featdgp; fdgp != NULL; fdgp = fdgp->next)
		numfdispg++;

ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return featdefp;
erret:
    featdgp = FeatDispGroupSetFree(featdgp);
	featdefp = FeatDefSetFree(featdefp);
    goto ret;
}

static Boolean LoadFeatDefFromLocalString (void)

{
#ifndef WIN16
  AsnIoMemPtr aimp;

  aimp = AsnIoMemOpen ("r", (BytePtr) featDefSetMemStr, (Int4) StringLen (featDefSetMemStr));
  if (aimp == NULL || aimp->aip == NULL) return FALSE;
  FeatDefGroupSetAsnRead (aimp->aip, NULL);
  AsnIoMemClose (aimp);
#endif
  return (Boolean) (featdefp != NULL);
}

NLM_EXTERN FeatDefPtr LIBCALL FeatDefSetLoad (void)
{
    Char buf[256];
    AsnIoPtr aip;

	if (featdefp != NULL)
		return featdefp;

	if (! loaded)
	{
		if (! FeatDefAsnLoad())
			return (FeatDefPtr)NULL;
	}

	if (! FindPath("ncbi", "ncbi", "data", buf, sizeof (buf)))
	{

		if (LoadFeatDefFromLocalString ()) {
			return featdefp;
		}

		ErrPost(CTX_NCBIOBJ, 1, "FindPath failed in FeatDefSetTableLoad - ncbi configuration file missing or incorrect");
        return featdefp;
	}

    StringCat(buf, "featdef.val");
    if ((aip = AsnIoOpen(buf, "rb")) == NULL)
	{

		if (LoadFeatDefFromLocalString ()) {
			return featdefp;
		}

		ErrPost(CTX_NCBIOBJ, 1, "Couldn't open [%s]", buf);
        return featdefp;
	}

	FeatDefGroupSetAsnRead (aip, NULL);

	AsnIoClose(aip);
    return featdefp;
}

/*****************************************************************************
*
*   FindImpFeatType(sfp)
*   	returns the featdef_key by matching the key of an Imp-feat
*       returns FEATDEF_BAD if can't match it
*
*****************************************************************************/
static Uint1 FindImpFeatType (SeqFeatPtr sfp)
{
	FeatDefPtr PNTR fdpp;
	FeatDefPtr fdp;
	ImpFeatPtr ifp;
	CharPtr tmp;
	Int2 i;

	if (sfp == NULL) return FEATDEF_BAD;
	if (sfp->data.choice != SEQFEAT_IMP) return FEATDEF_BAD;

	ifp = (ImpFeatPtr)(sfp->data.value.ptrvalue);
	tmp = ifp->key;

	if (FeatDefSetLoad() == NULL) return FEATDEF_BAD;
	fdpp = featdeflookup;

	for (i = 0; i < numfdef; i++, fdpp++)
	{
		fdp = *fdpp;
		if (fdp->seqfeat_key == SEQFEAT_IMP)
		{
			if (! StringCmp(fdp->typelabel, tmp))
				return fdp->featdef_key;
		}
	}

	return FEATDEF_BAD;
}


/*****************************************************************************
*
*   FindFeatDefType(sfp)
*   	Finds the featdef_type for a SeqFeat
*       returns FEATDEF_BAD if can't find it
*
*****************************************************************************/
NLM_EXTERN Uint1 LIBCALL FindFeatDefType(SeqFeatPtr sfp)
{
	RnaRefPtr rrp;
	ProtRefPtr prp;

	if (sfp == NULL) return FEATDEF_BAD;
	switch (sfp->data.choice)
	{
		case SEQFEAT_GENE:
			return FEATDEF_GENE;
		case SEQFEAT_ORG:
			return FEATDEF_ORG;
		case SEQFEAT_CDREGION:
			return FEATDEF_CDS;
		case SEQFEAT_PROT:
			prp = (ProtRefPtr)(sfp->data.value.ptrvalue);
			switch (prp->processed)
			{
				case 0:
					return FEATDEF_PROT;
				case 1:
					return FEATDEF_preprotein;
				case 2:
					return FEATDEF_mat_peptide_aa;
				case 3:
					return FEATDEF_sig_peptide_aa;
				case 4:
					return FEATDEF_transit_peptide_aa;
			}
			return FEATDEF_BAD;
		case SEQFEAT_RNA:
			rrp = (RnaRefPtr)(sfp->data.value.ptrvalue);
			switch (rrp->type)
			{
				case 0:
					return FEATDEF_otherRNA; /* unknownRNA mapped to otherRNA */
				case 1:
					return FEATDEF_preRNA;
				case 2:
					return FEATDEF_mRNA;
				case 3:
					return FEATDEF_tRNA;
				case 4:
					return FEATDEF_rRNA;
				case 5:
					return FEATDEF_snRNA;
				case 6:
					return FEATDEF_scRNA;
				case 7:
					return FEATDEF_snoRNA;
				case 255:
					return FEATDEF_otherRNA;
			}
			return FEATDEF_BAD;
		case SEQFEAT_PUB:
			return FEATDEF_PUB;
		case SEQFEAT_SEQ:
			return FEATDEF_SEQ;
		case SEQFEAT_IMP:
			return FindImpFeatType(sfp);
		case SEQFEAT_REGION:
			return FEATDEF_REGION;
		case SEQFEAT_COMMENT:
			return FEATDEF_COMMENT;
		case SEQFEAT_BOND:
			return FEATDEF_BOND;
		case SEQFEAT_SITE:
			return FEATDEF_SITE;
		case SEQFEAT_RSITE:
			return FEATDEF_RSITE;
		case SEQFEAT_USER:
			return FEATDEF_USER;
		case SEQFEAT_TXINIT:
			return FEATDEF_TXINIT;
		case SEQFEAT_NUM:
			return FEATDEF_NUM;
		case SEQFEAT_PSEC_STR:
			return FEATDEF_PSEC_STR;
		case SEQFEAT_NON_STD_RESIDUE:
			return FEATDEF_NON_STD_RESIDUE;
		case SEQFEAT_HET:
			return FEATDEF_HET;
		case SEQFEAT_BIOSRC:
			return FEATDEF_BIOSRC;
	}

	return FEATDEF_BAD;
}

/*****************************************************************************
*
*   FeatDefTypeLabel(sfp)
*   	returns the type label for the feature
*       returns NULL if can't find one
*
*****************************************************************************/
NLM_EXTERN const char * LIBCALL FeatDefTypeLabel(SeqFeatPtr sfp)
{
	Int2 i;

	if (sfp == NULL) return (const char *)NULL;
	if (FeatDefSetLoad() == NULL) return (const char *)NULL;

	i = (Int2) FindFeatDefType(sfp);
	if (i == FEATDEF_BAD) return NULL;

	return (const char *)(featdeflookup[i]->typelabel);
}

static Int2 NEAR FeatDefLabelContent (SeqFeatPtr sfp, CharPtr buf, Int2 buflen, Uint1 labeltype, CharPtr typelabel)
{
	GeneRefPtr grp=NULL;
	OrgRefPtr orp=NULL;
	ProtRefPtr prp=NULL;
	SeqFeatXrefPtr sfxrp;
	SeqIdPtr sip;
	BioseqPtr bsp;
	BioseqContextPtr bcp;
	SeqFeatPtr sfp2;
	RsiteRefPtr rrp;
	UserObjectPtr uop;
	BioSourcePtr bsrcp;
	CharPtr label = NULL;
	RnaRefPtr trrp;
	tRNAPtr trp;
	Char tbuf[40], snpbuf [64];
	Int2 len = buflen, diff;
	GBQualPtr gbp;
	CharPtr prefix=NULL, suffix=NULL, ptr;
	PubdescPtr pdp;
	ValNodePtr vnp;
	ImpFeatPtr ifp;
	Boolean first;
	ValNode vn;
	static CharPtr slash = " /";
	SeqMapTablePtr smtp;
	SeqCodeTablePtr sctp;
	Uint1 aacode;
	DbtagPtr dbt;
	ObjectIdPtr oip;

	if (sfp == NULL) return 0;

	switch (sfp->data.choice)
	{
		case SEQFEAT_GENE:
			grp = (GeneRefPtr)(sfp->data.value.ptrvalue);
generef:	if (grp->locus != NULL)
				label = (grp->locus);
			else if (grp->desc != NULL)
				label = (grp->desc);
			else if (grp->syn != NULL)
				label = (CharPtr)(grp->syn->data.ptrvalue);
			else if (grp->db != NULL)
				return DbtagLabel((DbtagPtr)(grp->db->data.ptrvalue), buf, buflen);
			else if (grp->maploc != NULL)
				label = (grp->maploc);
			break;
		case SEQFEAT_ORG:
			orp = (OrgRefPtr)(sfp->data.value.ptrvalue);
orgref:		if (orp->taxname != NULL)
				label = (orp->taxname);
			else if (orp->common != NULL)
				label = (orp->common);
			else if (orp->db != NULL)
				return DbtagLabel((DbtagPtr)(orp->db->data.ptrvalue), buf, buflen);
			break;
		case SEQFEAT_CDREGION:
			for (sfxrp = sfp->xref; sfxrp != NULL; sfxrp = sfxrp->next)
			{
				switch (sfxrp->data.choice)
				{
					case SEQFEAT_PROT:
						prp = (ProtRefPtr)(sfxrp->data.value.ptrvalue);
						break;
					case SEQFEAT_GENE:
						grp = (GeneRefPtr)(sfxrp->data.value.ptrvalue);
						break;
				}
			}
			if (prp != NULL) goto protref;
			if (sfp->product != NULL)
			{
				sip = SeqLocId(sfp->product);
				bsp = BioseqFind(sip);
				if (bsp != NULL)
				{
					/* first see if preindexed in object manager */
					sfp2 = SeqMgrGetBestProteinFeature (bsp, NULL);
					if (sfp2 != NULL) {
						prp = (ProtRefPtr)(sfp2->data.value.ptrvalue);
						if (prp != NULL) {
							goto protref;
						}
					}
					/* if not preindexed, find with bioseq context */
					bcp = BioseqContextNew(bsp);
					sfp2 = BioseqContextGetSeqFeat(bcp, SEQFEAT_PROT, NULL, NULL, 0);
					BioseqContextFree(bcp);
					if (sfp2 != NULL)
					{
						prp = (ProtRefPtr)(sfp2->data.value.ptrvalue);
						goto protref;
					}
				}
			}
			if (grp != NULL &&
				((grp->locus != NULL && grp->locus [0] != '\0') ||
					grp->allele != NULL || grp->desc != NULL ||
					grp->maploc != NULL || grp->pseudo ||
					grp->db != NULL || grp->syn != NULL)) goto generef;
			break;
		case SEQFEAT_PROT:
			prp = (ProtRefPtr)(sfp->data.value.ptrvalue);
protref:    if (prp->name != NULL)
				label = (prp->name->data.ptrvalue);
			else if (prp->desc != NULL)
				label = (prp->desc);
			else if (prp->db != NULL)
				return DbtagLabel((DbtagPtr)(prp->db->data.ptrvalue), buf, buflen);
			break;
		case SEQFEAT_RNA:
			trrp = (RnaRefPtr)(sfp->data.value.ptrvalue);
			if (labeltype == OM_LABEL_CONTENT)
			{
				prefix = StringMove(tbuf, typelabel);
				StringMove(prefix, "-");
				prefix = tbuf;
			}

			switch (trrp->ext.choice)
			{
				case 0:
					label = sfp->comment;
					if (label != NULL) {	/* if RNA already in comment, skip it */
					  if (StringStr(label, typelabel) != NULL)
					  	prefix = NULL;
					}
					else
					{
						prefix = NULL;
						label = typelabel;
					}
					break;
				case 1:
					label = (CharPtr)(trrp->ext.value.ptrvalue);
					if (label != NULL) {
					  if (StringStr(label, typelabel) != NULL)
					  	prefix = NULL;
					}
					else
					{
						prefix = NULL;
						label = typelabel;
					}
					break;
				case 2:
					trp = (tRNAPtr)(trrp->ext.value.ptrvalue);
					switch (trp->aatype)
					{
						case 1:
							aacode = Seq_code_iupacaa;
							break;
						case 2:
							aacode = Seq_code_ncbieaa;
							break;
						case 3:
							aacode = Seq_code_ncbi8aa;
							break;
						case 4:
							aacode = Seq_code_ncbistdaa;
							break;
						default:
							aacode = 0;
							break;
					}
					if (! aacode)
						break;
					sctp = SeqCodeTableFind(Seq_code_iupacaa3);
					if (sctp == NULL)
					{
						label = prefix;
						prefix = NULL;
						break;
					}
					if (aacode != Seq_code_ncbistdaa)
					{
						smtp = SeqMapTableFind(Seq_code_ncbistdaa, aacode);
						if (smtp == NULL)
						{
							label = prefix;
							prefix = NULL;
							break;
						}
						aacode = SeqMapTableConvert(smtp, trp->aa);
					} else {
						aacode = trp->aa;
					}
					if (aacode == 255) {
						label = prefix;
						prefix = NULL;
						break;
					}
					label = sctp->symbols[aacode - sctp->start_at];
					break;
				default:
					break;
			}
			break;    /* just return the type label for now */
		case SEQFEAT_PUB:
			pdp = (PubdescPtr)(sfp->data.value.ptrvalue);
			vn.choice = PUB_Equiv;
			vn.data.ptrvalue = pdp->pub;
			vn.next = NULL;
			return PubLabel(&vn, buf, buflen, OM_LABEL_CONTENT);
		case SEQFEAT_SEQ:
			break;
		case SEQFEAT_IMP:
			ifp = (ImpFeatPtr)(sfp->data.value.ptrvalue);
			if (! StringICmp("Site-ref", ifp->key))
			{
				if (sfp->cit != NULL)
				{
					first = TRUE;
					for (vnp = (ValNodePtr)(sfp->cit->data.ptrvalue);
					              vnp != NULL; vnp = vnp->next)
					{
						if (! first)
						{
							diff = LabelCopy(buf, ",", buflen);
							buflen -= diff; buf += diff;
						}
						else
							first = FALSE;
						diff = PubLabel(vnp, buf, buflen, OM_LABEL_CONTENT);
						buflen -= diff; buf += diff;
						first = FALSE;
					}
					return (len - buflen);
				}
			}
			else if (labeltype == OM_LABEL_CONTENT) /* show key */
			{
				if (! StringICmp("CDS", ifp->key))
					label = "[CDS]";
				else if ((! StringICmp("repeat_unit", ifp->key))	||
						(! StringICmp("repeat_region", ifp->key)))
				{
					for (gbp = sfp->qual; ((label == NULL) && (gbp != NULL)); gbp = gbp->next)
					{
						if (! StringICmp("rpt_family", gbp->qual))
							label = gbp->val;
					}
					if (label == NULL)
						label = typelabel;
				}
				else if (! StringICmp("STS", ifp->key))
				{
					for (gbp = sfp->qual; ((label == NULL) && (gbp != NULL)); gbp = gbp->next)
					{
						if (! StringICmp("standard_name", gbp->qual))
							label = gbp->val;
					}
					if (label == NULL && (! StringHasNoText (sfp->comment))) {
						StringNCpy_0 (snpbuf, sfp->comment, sizeof (snpbuf));
						ptr = StringChr (snpbuf, ';');
						if (ptr != NULL) {
							*ptr = '\0';
						}
						label = snpbuf;
					}
					if (label == NULL)
						label = typelabel;
				}
				else if (StringICmp("misc_feature", ifp->key)) {
					for (gbp = sfp->qual; label == NULL && gbp != NULL; gbp = gbp->next) {
						if (StringICmp ("standard_name", gbp->qual) == 0) {
							label = gbp->val;
						}
					}
					for (gbp = sfp->qual; label == NULL && gbp != NULL; gbp = gbp->next) {
						if (StringICmp ("function", gbp->qual) == 0) {
							label = gbp->val;
						}
					}
					for (gbp = sfp->qual; label == NULL && gbp != NULL; gbp = gbp->next) {
						if (StringICmp ("number", gbp->qual) == 0) {
							label = gbp->val;
						}
					}
					for (gbp = sfp->qual; label == NULL && gbp != NULL; gbp = gbp->next) {
						label = gbp->val;
					}
					if (label == NULL)
						label = typelabel;
				}
			}
			break;
		case SEQFEAT_REGION:
			label = (sfp->data.value.ptrvalue);
			if (StringICmp (label, "Domain") == 0 && sfp->comment != NULL) {
			  label = sfp->comment;
			}
			break;
		case SEQFEAT_COMMENT:
			label = sfp->comment;
			break;
		case SEQFEAT_BOND:
			label =  AsnEnumStr("SeqFeatData.bond",
				(Int2)(sfp->data.value.intvalue));
			break;
		case SEQFEAT_SITE:
			label =  AsnEnumStr("SeqFeatData.site",
				(Int2)(sfp->data.value.intvalue));
			break;
		case SEQFEAT_RSITE:
			rrp = (RsiteRefPtr)(sfp->data.value.ptrvalue);
			if (rrp->choice == 1)
				label = (rrp->data.ptrvalue);
			else if (rrp->choice == 2) {
				/* return DbtagLabel((DbtagPtr)(rrp->data.ptrvalue), buf, buflen); */
				label = "?";
				dbt = (DbtagPtr) rrp->data.ptrvalue;
				if (dbt != NULL) {
					oip = dbt->tag;
					if (oip != NULL) {
						label = oip->str;
					}
				}
			}
			break;
		case SEQFEAT_USER:
			uop = (UserObjectPtr)(sfp->data.value.ptrvalue);
			label = (uop->_class);
			if (label == NULL) {
				oip = uop->type;
				if (oip != NULL) {
					label = oip->str;
				}
			}
			break;
		case SEQFEAT_TXINIT:
			break;
		case SEQFEAT_NUM:
			break;
		case SEQFEAT_PSEC_STR:
			label =  AsnEnumStr("SeqFeatData.psec-str",
				(Int2)(sfp->data.value.intvalue));
			break;
		case SEQFEAT_NON_STD_RESIDUE:
			label = (sfp->data.value.ptrvalue);
			break;
		case SEQFEAT_HET:
			label = (sfp->data.value.ptrvalue);
			break;
		case SEQFEAT_BIOSRC:
			bsrcp = (BioSourcePtr)(sfp->data.value.ptrvalue);
			orp = bsrcp->org;
			goto orgref;
		default:
			break;
	}
    
	if (label != NULL)
		return LabelCopyExtra (buf, label, buflen, prefix, suffix);
	
	first = TRUE;
	diff = 1;     /* just to make the first pass through loop work */
	for (gbp=sfp->qual; ((gbp != NULL) && (diff)); gbp = gbp->next)
	{
		if (first)
			prefix = (slash + 1);
		else
			prefix = slash;
		first = FALSE;
		diff = LabelCopyExtra (buf, gbp->qual, buflen, prefix, NULL);
		buflen -= diff;
		buf += diff;
		if (gbp->val != NULL)
		{
			prefix = "=";
			diff = LabelCopyExtra(buf, gbp->val, buflen, prefix, NULL);
			buflen -= diff;
			buf += diff;
		}
	}
	
	if (sfp->comment != NULL)
	{
		if (! first)
			prefix = "; ";
		else
			prefix = NULL;
		diff = LabelCopyExtra(buf, sfp->comment, buflen, prefix, NULL);
		buflen -= diff;
	}
	
	return (len - buflen);
}

/*****************************************************************************
*
*   FeatDefLabel(sfp, buf, buflen, type)
*   	fills in buf with a content based label
*   	if longer than buflen, makes the last visible char >
*   	guarantees a '\0' at the end of the string
*   
*   NOTE: buf MUST be (buflen+1) long
*   
*   	This function makes nicer labels since it can combine elements
*   	returns length of string or 0 on failure
*
*   	type is OM_LABEL_TYPE_ defined in objmgr.h
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL FeatDefLabel (SeqFeatPtr sfp, CharPtr buf, Int2 buflen, Uint1 labeltype)
{
	Int2 len, i, diff;
	CharPtr curr, typelabel, tmp;
	Uint1 extras = 0;
	Char tbuf[40];
	ImpFeatPtr ifp;
	CharPtr suffix = NULL;

	if ((sfp == NULL) || (buf == NULL) || (! buflen))
		return 0;

	buf[0] = '\0';
	curr = buf;
	len = buflen;

	if (FeatDefSetLoad() == NULL) return 0;

	i = (Int2) FindFeatDefType(sfp);
	if (i != FEATDEF_BAD)
	{
		typelabel = featdeflookup[i]->typelabel;
		if ((sfp->data.choice == SEQFEAT_IMP) &&
			(! StringCmp("CDS", typelabel)))
		{
			tmp = StringMove(tbuf, "[");
			tmp = StringMove(tmp, typelabel);
			tmp = StringMove(tmp, "]");
			typelabel = tbuf;
		} else if (sfp->data.choice == SEQFEAT_REGION &&
		           StringICmp ((CharPtr) sfp->data.value.ptrvalue, "Domain") == 0 &&
		           sfp->comment != NULL) {
			StringCpy (tbuf, "Domain");
			typelabel = tbuf;
		}
	}
	else
	{
		typelabel = tbuf;
		switch (sfp->data.choice)
		{
			case SEQFEAT_IMP:
				ifp = (ImpFeatPtr)(sfp->data.value.ptrvalue);
				tmp = StringMove(tbuf, "[");
				tmp = StringMove(tmp, ifp->key);
				tmp = StringMove(tmp, "]");
				break;
			default:
				sprintf(tbuf, "[Unknown=%d]", (int)(sfp->data.choice));
				break;
		}
	}

	if ((labeltype == OM_LABEL_TYPE) || (labeltype == OM_LABEL_BOTH))
	{
		if (labeltype == OM_LABEL_BOTH)
			suffix = ": ";
		else
			suffix = NULL;

		diff = LabelCopyExtra(curr, typelabel, buflen, NULL, suffix);
		curr += diff;
		buflen -= diff;
	}

	if ((labeltype == OM_LABEL_TYPE) || (! buflen))
		return (len - buflen);

	diff = FeatDefLabelContent (sfp, curr, buflen, labeltype, typelabel);
	buflen -= diff;

	if ((! diff) && (labeltype == OM_LABEL_CONTENT))
	{
		buflen -= LabelCopy(curr, typelabel, buflen);
	}

	return (len - buflen);
}

/*****************************************************************************
*
*   DispGroupNum()
*   	returns number of display groups
*   	returns 0 on failure
*       loads featdef.val if not already loaded
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL DispGroupNum(void)
{
	FeatDefSetLoad();
	return numfdispg;
}

/*****************************************************************************
*
*   DispGroupFindNext(curr, groupptr, groupname)
*     returns display groups in order
*     start with curr=NULL, then return current in curr until function
*       returns NULL
*     loads featdef.val if necessary
*     groupptr is filled in with the key for the group, used
*       in FeatDefFindNext() below.
*     groupname points to the string naming the group
*
*****************************************************************************/
NLM_EXTERN FeatDispGroupPtr LIBCALL DispGroupFindNext(FeatDispGroupPtr curr, Uint1Ptr groupptr, CharPtr PNTR groupname)
{
	FeatDefSetLoad();
	if (curr == NULL)
		curr = featdgp;
	else
		curr = curr->next;

	if (curr != NULL)
	{
		*groupptr = curr->groupkey;
		*groupname = curr->groupname;
	}
	return curr;
}

/*****************************************************************************
*
*   FeatDefNum()
*   	returns total number of FeatDef
*       loads featdef.val if necessary
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL FeatDefNum(void)
{
	FeatDefSetLoad();
	return numfdef;
}

/*****************************************************************************
*
*   FeatDefFindNext(curr, keyptr, menulabel, group, for_display)
*   	returns next FeatDef within display group
*       if group == FEATDEF_ANY returns all
*       start with curr = NULL and return current in curr until function
*         returns NULL
*       keyptr is filled in with featdef-key
*       menulabel is filled in with menulabel
*       if for_display == TRUE then group must match display group
*         else group must match entrygroup
*       loads featdef.val if necessary
*
*****************************************************************************/
NLM_EXTERN FeatDefPtr LIBCALL FeatDefFindNext (FeatDefPtr curr, Uint1Ptr keyptr,
                           CharPtr PNTR menulabel, Uint1 group, Boolean for_display)
{
	FeatDefSetLoad();
	if (curr == NULL)
		curr = featdefp;
	else
		curr = curr->next;

	if ((group == FEATDEF_ANY) && (curr != NULL))
	{
		*keyptr = curr->featdef_key;
		*menulabel = curr->menulabel;
		return curr;
	}

	while (curr != NULL)
	{
		if (for_display)
		{
			if (group == curr->displaygroup)
			{
				*keyptr = curr->featdef_key;
				*menulabel = curr->menulabel;
				return curr;
			}
		}
		else
		{
			if (group == curr->entrygroup)
			{
				*keyptr = curr->featdef_key;
				*menulabel = curr->menulabel;
				return curr;
			}
		}
		curr = curr->next;
	}
	return curr;
}

