/*  objacces.c
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
* File Name:  objacces.c
*
* Author:  James Ostell
*   
* Version Creation Date: 1/1/91
*
* $Revision: 6.1 $
*
* File Description:  Object manager for module NCBI-Access
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 05-13-93 Schuler     All public functions are now declared LIBCALL.
*
* $Log: objacces.c,v $
* Revision 6.1  2004/04/01 13:43:07  lavr
* Spell "occurred", "occurrence", and "occurring"
*
* Revision 6.0  1997/08/25 18:49:05  madden
* Revision changed to 6.0
*
* Revision 4.1  1997/06/19 18:40:33  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.0  1995/07/26 13:48:06  ostell
* force revision to 4.0
*
 * Revision 3.1  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/
#include <asnacces.h>        /* the AsnTool header */
#include <objacces.h>		   /* the general objects interface */

static Boolean loaded = FALSE;

/*****************************************************************************
*
*   AccessAsnLoad()
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL AccessAsnLoad (void)
{
    if (loaded)
        return TRUE;

    if (AsnLoad())
        loaded = TRUE;
    return loaded;
}

/*****************************************************************************
*
*   LinkSet Routines
*
*****************************************************************************/


/*****************************************************************************
*
*   LinkSetNew()
*
*****************************************************************************/
NLM_EXTERN LinkSetPtr LIBCALL LinkSetNew (void)
{
	return (LinkSetPtr)MemNew(sizeof(LinkSet));
}
/*****************************************************************************
*
*   LinkSetFree(lsp)
*
*****************************************************************************/
NLM_EXTERN LinkSetPtr LIBCALL LinkSetFree (LinkSetPtr lsp)
{
	if (lsp == NULL)
		return (LinkSetPtr)NULL;
	MemFree(lsp->uids);
	MemFree(lsp->weights);
	return (LinkSetPtr)MemFree(lsp);
}

/*****************************************************************************
*
*   LinkSetAsnWrite(lsp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (LinkSet ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL LinkSetAsnWrite (LinkSetPtr lsp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Int4 num, i;
    Int4Ptr ptr;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! AccessAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, LINK_SET);   /* link local tree */
    if (atp == NULL)
        return FALSE;

	if (lsp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)lsp))
        goto erret;
    num = lsp->num;
    av.intvalue = num;
    if (! AsnWrite(aip, LINK_SET_num, &av))
        goto erret;
    ptr = lsp->uids;
    if (! AsnOpenStruct(aip, LINK_SET_uids, (Pointer)lsp->uids))
        goto erret;
    for (i = 0; i < num; i++)
    {
        av.intvalue = ptr[i];
        if (! AsnWrite(aip, LINK_SET_uids_E, &av))
            goto erret;
    }
    if (! AsnCloseStruct(aip, LINK_SET_uids, (Pointer)lsp->uids))
        goto erret;
    if (lsp->weights != NULL)
    {
        ptr = lsp->weights;
        if (! AsnOpenStruct(aip, LINK_SET_weights, (Pointer)lsp->weights))
            goto erret;
        for (i = 0; i < num; i++)
        {
            av.intvalue = ptr[i];
            if (! AsnWrite(aip, LINK_SET_weights_E, &av))
                goto erret;
        }
        if (! AsnCloseStruct(aip, LINK_SET_weights, (Pointer)lsp->weights))
            goto erret;
    }
    if (! AsnCloseStruct(aip, atp, (Pointer)lsp))
        goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   LinkSetAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN LinkSetPtr LIBCALL LinkSetAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    LinkSetPtr lsp=NULL;
    Int4 num, i;
    Int4Ptr ptr;

	if (! loaded)
	{
		if (! AccessAsnLoad())
			return lsp;
	}

	if (aip == NULL)
		return lsp;

	if (orig == NULL)           /* LinkSet ::= (self contained) */
		atp = AsnReadId(aip, amp, LINK_SET);
	else
		atp = AsnLinkType(orig, LINK_SET);    /* link in local tree */
    if (atp == NULL)
        return lsp;

	lsp = LinkSetNew();
    if (lsp == NULL)
        goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0)    /* read the start struct */
        goto erret;
	atp = AsnReadId(aip, amp, atp);  /* find the num */
    if (atp == NULL)
        goto erret;
	if (AsnReadVal(aip, atp, &av) <= 0)     /* get the num */
        goto erret;
    num = av.intvalue;
    lsp->num = num;

    atp = AsnReadId(aip, amp, atp);  /* start seq of uids */
    if (atp == NULL)
        goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0)
        goto erret;
    ptr = (Int4Ptr)MemNew((size_t)(sizeof(Int4) * (num + 1)));  /* 0 sentinel at end */
    if (ptr == NULL)
        goto erret;
    lsp->uids = ptr;
    i = 0;
    while ((atp = AsnReadId(aip, amp, atp)) == LINK_SET_uids_E)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        ptr[i] = av.intvalue;
        i++;
        if (i > num)
            break;
    }
    if (atp == NULL)
        goto erret;
    if (i != num)
    {
        ErrPost(CTX_NCBIOBJ, 0, "Incorrect number of uids in Link-set. line %ld",
            aip->linenumber);
        goto erret;
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end seq of */

    atp = AsnReadId(aip, amp, atp);
    if (atp == NULL)
        goto erret;
    if (atp == LINK_SET_weights)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        ptr = (Int4Ptr)MemNew((size_t)(sizeof(Int4) * (num + 1)));  /* 0 sentinel at end */
        if (ptr == NULL)
            goto erret;
        lsp->weights = ptr;
        i = 0;
        while ((atp = AsnReadId(aip, amp, atp)) == LINK_SET_weights_E)
        {
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
            ptr[i] = av.intvalue;
            i++;
            if (i > num)
                break;
        }
        if (atp == NULL)
            goto erret;
        if (i != num)
        {   
            ErrPost(CTX_NCBIOBJ, 0, "Incorrect number of weights in Link-set. line %ld",
                aip->linenumber);
            goto erret;
        }
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end seq of */

	    if ((atp = AsnReadId(aip, amp, atp)) == NULL)
	        goto erret;
    }

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;  /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return lsp;
erret:
    lsp = LinkSetFree(lsp);
    goto ret;
}

