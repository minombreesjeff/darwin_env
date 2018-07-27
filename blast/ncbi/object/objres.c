/*  objres.c
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
* File Name:  objres.c
*
* Author:  James Ostell
*   
* Version Creation Date: 4/1/91
*
* $Revision: 6.6 $
*
* File Description:  Object manager for module NCBI-Seqres
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 05-13-93 Schuler     All public functions are now declared LIBCALL.
*
*
* $Log: objres.c,v $
* Revision 6.6  2004/05/12 20:41:56  kans
* set aip->io_failure in several erret blocks for compatibility of old object loaders with new ones
*
* Revision 6.5  2004/04/01 13:43:08  lavr
* Spell "occurred", "occurrence", and "occurring"
*
* Revision 6.4  1998/12/09 20:37:52  kans
* changed compl to compr to avoid new c++ symbol collision
*
* Revision 6.3  1998/08/26 17:43:19  kans
* fixed -v -fd warnings in label functions
*
* Revision 6.2  1998/08/24 18:28:08  kans
* removed solaris -v -fd warnings
*
* Revision 6.1  1997/10/10 17:50:49  bazhin
* Fixed bug related to freing memory in SeqGraphFree() function.
*
* Revision 6.0  1997/08/25 18:50:38  madden
* Revision changed to 6.0
*
* Revision 4.2  1997/06/19 18:41:52  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.1  1996/12/09 16:57:20  kans
* fixed SeqGraphAsnRead to look for both end-structs
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
#include <objres.h>		   /* the SeqRes interface */
#include <asnres.h>        /* the AsnTool header */
#include <objmgr.h>

/*****************************************************************************
*
*   SeqGraph ObjMgr Routines
*
*****************************************************************************/
static CharPtr graphtypename = "SeqGraph";

static Pointer LIBCALLBACK SeqGraphNewFunc (void)
{
	return (Pointer) SeqGraphNew();
}

static Pointer LIBCALLBACK SeqGraphFreeFunc (Pointer data)
{
	return (Pointer) SeqGraphFree ((SeqGraphPtr) data);
}

static Boolean LIBCALLBACK SeqGraphAsnWriteFunc (Pointer data, AsnIoPtr aip, AsnTypePtr atp)
{
	return SeqGraphAsnWrite((SeqGraphPtr)data, aip, atp);
}

static Pointer LIBCALLBACK SeqGraphAsnReadFunc (AsnIoPtr aip, AsnTypePtr atp)
{
	return (Pointer) SeqGraphAsnRead (aip, atp);
}

static Int2 LIBCALLBACK SeqGraphLabelFunc ( Pointer data, CharPtr buffer, Int2 buflen, Uint1 content)
{
	return SeqGraphLabel((SeqGraphPtr)data, buffer, buflen, content);
}

NLM_EXTERN Int2 LIBCALL SeqGraphLabel (SeqGraphPtr sgp, CharPtr buffer, Int2 buflen, Uint1 content)
{
	Int2 rsult = 0;

	if ((sgp == NULL) || (buflen < 1))
		return 0;

	switch (content)
	{
		case OM_LABEL_BOTH:
		case OM_LABEL_CONTENT:
		case OM_LABEL_SUMMARY:
		case OM_LABEL_TYPE:
		default:
			rsult = LabelCopy(buffer, graphtypename, buflen);
	}
	return rsult;
}

static Boolean loaded = FALSE;

/*****************************************************************************
*
*   SeqResAsnLoad()
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqResAsnLoad (void)
{
    if (loaded)
        return TRUE;
    loaded = TRUE;

    if (! SeqLocAsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }
    if (! AsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }

	ObjMgrTypeLoad(OBJ_SEQGRAPH, "Seq-graph", graphtypename, "Sequence Graph",
		SEQ_GRAPH, SeqGraphNewFunc, SeqGraphAsnReadFunc, SeqGraphAsnWriteFunc,
		SeqGraphFreeFunc, SeqGraphLabelFunc, NULL);

    return TRUE;
}
/*****************************************************************************
*
*   SeqRes Routines
*
*****************************************************************************/
/*****************************************************************************
*
*   SeqGraph 
*
*****************************************************************************/

/*****************************************************************************
*
*   SeqGraphNew()
*
*****************************************************************************/
NLM_EXTERN SeqGraphPtr LIBCALL SeqGraphNew (void)
{
    return (SeqGraphPtr)MemNew(sizeof(SeqGraph));
}

/*****************************************************************************
*
*   SeqGraphFree(sgp)
*       Frees one SeqGraph and associated data
*
*****************************************************************************/
NLM_EXTERN SeqGraphPtr LIBCALL SeqGraphFree (SeqGraphPtr sgp)
{
    if (sgp == NULL)
        return sgp;
    
    MemFree(sgp->title);
    MemFree(sgp->comment);
    SeqLocFree(sgp->loc);
    MemFree(sgp->titlex);
    MemFree(sgp->titley);
    if (sgp->flags[2] != 3)    /* real or int */
        MemFree(sgp->values);
    else                       /* byte */
        BSFree((ByteStorePtr)(sgp->values));

	ObjMgrDelete(OBJ_SEQGRAPH, (Pointer)sgp);

    return (SeqGraphPtr)MemFree(sgp);
}

/*****************************************************************************
*
*   SeqGraphAsnWrite(sgp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqGraph ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqGraphAsnWrite (SeqGraphPtr sgp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Int4 numval, i;
    FloatHiPtr rp;
    Int4Ptr ip;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqResAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, SEQ_GRAPH);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (sgp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)sgp)) goto erret;
    if (sgp->title != NULL)
    {
        av.ptrvalue = sgp->title;
        if (! AsnWrite(aip, SEQ_GRAPH_title, &av)) goto erret;
    }
    if (sgp->comment != NULL)
    {
        av.ptrvalue = sgp->comment;
        if (! AsnWrite(aip, SEQ_GRAPH_comment, &av)) goto erret;
    }
    if (! SeqLocAsnWrite(sgp->loc, aip, SEQ_GRAPH_loc)) goto erret;
    if (sgp->titlex != NULL)
    {
        av.ptrvalue = sgp->titlex;
        if (! AsnWrite(aip, SEQ_GRAPH_title_x, &av)) goto erret;
    }
    if (sgp->titley != NULL)
    {
        av.ptrvalue = sgp->titley;
        if (! AsnWrite(aip,SEQ_GRAPH_title_y, &av)) goto erret;
    }
    if (sgp->flags[0])
    {
        av.intvalue = sgp->compr;
        if (! AsnWrite(aip, SEQ_GRAPH_comp, &av)) goto erret;
    }
    if (sgp->flags[1])
    {
        av.realvalue = sgp->a;
        if (! AsnWrite(aip, SEQ_GRAPH_a, &av)) goto erret;
        av.realvalue = sgp->b;
        if (! AsnWrite(aip, SEQ_GRAPH_b, &av)) goto erret;
    }
    numval = sgp->numval;
    av.intvalue = numval;
    if (! AsnWrite(aip, SEQ_GRAPH_numval, &av)) goto erret;
			
	av.ptrvalue = (Pointer)sgp;
    if (! AsnWriteChoice(aip, SEQ_GRAPH_graph, (Int2)sgp->flags[2], &av)) goto erret;

    if (sgp->flags[2] == 1)     /* real graph */
    {
        rp = (FloatHiPtr) sgp->values;
        if (! AsnOpenStruct(aip, SEQ_GRAPH_graph_real, (Pointer)sgp)) goto erret;
        if (! AsnWrite(aip, REAL_GRAPH_max, &sgp->max)) goto erret;
        if (! AsnWrite(aip, REAL_GRAPH_min, &sgp->min)) goto erret;
        if (! AsnWrite(aip, REAL_GRAPH_axis, &sgp->axis)) goto erret;
        if (! AsnOpenStruct(aip, REAL_GRAPH_values, sgp->values)) goto erret;
        for (i = 0; i < numval; i++)
        {
            av.realvalue = rp[i];
            if (! AsnWrite(aip, REAL_GRAPH_values_E, &av)) goto erret;
        }
        if (! AsnCloseStruct(aip, REAL_GRAPH_values, sgp->values)) goto erret;
        if (! AsnCloseStruct(aip, SEQ_GRAPH_graph_real, (Pointer)sgp)) goto erret;
    }
    else if (sgp->flags[2] == 2)    /* int graph */
    {
        ip = (Int4Ptr) sgp->values;
        if (! AsnOpenStruct(aip, SEQ_GRAPH_graph_int, (Pointer)sgp)) goto erret;
        if (! AsnWrite(aip, INT_GRAPH_max, &sgp->max)) goto erret;
        if (! AsnWrite(aip, INT_GRAPH_min, &sgp->min)) goto erret;
        if (! AsnWrite(aip, INT_GRAPH_axis, &sgp->axis)) goto erret;
        if (! AsnOpenStruct(aip, INT_GRAPH_values, sgp->values)) goto erret;
        for (i = 0; i < numval; i++)
        {
            av.intvalue = ip[i];
            if (! AsnWrite(aip, INT_GRAPH_values_E, &av)) goto erret;
        }
        if (! AsnCloseStruct(aip, INT_GRAPH_values, sgp->values)) goto erret;
        if (! AsnCloseStruct(aip, SEQ_GRAPH_graph_int, (Pointer)sgp)) goto erret;
    }
    else if (sgp->flags[2] == 3)   /* byte graph */
    {
        if (! AsnOpenStruct(aip, SEQ_GRAPH_graph_byte, (Pointer)sgp)) goto erret;
        if (! AsnWrite(aip, BYTE_GRAPH_max, &sgp->max)) goto erret;
        if (! AsnWrite(aip, BYTE_GRAPH_min, &sgp->min)) goto erret;
        if (! AsnWrite(aip, BYTE_GRAPH_axis, &sgp->axis)) goto erret;
        av.ptrvalue = sgp->values;         /* write the byte store */
        if (! AsnWrite(aip, BYTE_GRAPH_values, &av)) goto erret;
        if (! AsnCloseStruct(aip, SEQ_GRAPH_graph_byte, (Pointer)sgp)) goto erret;
    }
    if (! AsnCloseStruct(aip, atp, (Pointer)sgp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqGraphAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN SeqGraphPtr LIBCALL SeqGraphAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    SeqGraphPtr sgp=NULL;
    Int4 numval, i;
    Int4Ptr ip;
    FloatHiPtr rp;

	if (! loaded)
	{
		if (! SeqResAsnLoad())
			return sgp;
	}

	if (aip == NULL)
		return sgp;

	if (orig == NULL)           /* SeqGraph ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_GRAPH);
	else
		atp = AsnLinkType(orig, SEQ_GRAPH);    /* link in local tree */
    if (atp == NULL) return sgp;

	sgp = SeqGraphNew();
    if (sgp == NULL) goto erret;

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the start struct */
	atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;  /* get the values */

	if (atp == SEQ_GRAPH_title)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        sgp->title = (CharPtr)av.ptrvalue;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }
	if (atp == SEQ_GRAPH_comment)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        sgp->comment = (CharPtr)av.ptrvalue;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }

    sgp->loc = SeqLocAsnRead(aip, atp);
    if (sgp->loc == NULL) goto erret;
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;

	if (atp == SEQ_GRAPH_title_x)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        sgp->titlex = (CharPtr)av.ptrvalue;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }
	if (atp ==SEQ_GRAPH_title_y)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        sgp->titley = (CharPtr)av.ptrvalue;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }
	if (atp == SEQ_GRAPH_comp)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        sgp->compr = av.intvalue;
        sgp->flags[0] = 1;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }
	if (atp == SEQ_GRAPH_a)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        sgp->a = av.realvalue;
		sgp->b = 0.0;      /* set a default */
        sgp->flags[1] = 1;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }
	if (atp == SEQ_GRAPH_b)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        sgp->b = av.realvalue;
		if (! sgp->flags[1])   /* no a was read */
		{
	        sgp->flags[1] = 1;
			sgp->a = 1.0;      /* set default */
		}
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }

	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;  /* numval */
	numval = av.intvalue;
	sgp->numval = numval;
	
	atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the CHOICE */
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* find what kind of choice */
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the start struct */
    if (atp == SEQ_GRAPH_graph_real)
        sgp->flags[2] = 1;
    else if (atp == SEQ_GRAPH_graph_int)
        sgp->flags[2] = 2;
    else if (atp == SEQ_GRAPH_graph_byte)
        sgp->flags[2] = 3;

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &sgp->max) <= 0) goto erret;
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &sgp->min) <= 0) goto erret;
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &sgp->axis) <= 0) goto erret;

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
	

    if (atp == REAL_GRAPH_values)
    {
        sgp->values = (FloatHiPtr)MemNew((size_t)(sizeof(FloatHi) * numval));
        if (sgp->values == NULL) goto erret;
        rp = (FloatHiPtr)sgp->values;
        for (i = 0; i < numval; i++)
        {
            atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
            if (atp != REAL_GRAPH_values_E)
            {
                ErrPost(CTX_NCBIOBJ,1, "Too few real values in Seq-graph");
                goto erret;
            }
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
            rp[i] = av.realvalue;
        }
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;      /* end SEQ OF */
    }
    else if (atp == INT_GRAPH_values)
    {
        sgp->values = (Int4Ptr)MemNew((size_t)(sizeof(Int4) * numval));
        ip = (Int4Ptr)sgp->values;
        if (ip == NULL) goto erret;
        for (i = 0; i < numval; i++)
        {
            atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
            if (atp != INT_GRAPH_values_E)
            {
                ErrPost(CTX_NCBIOBJ,1, "Too few integer values in Seq-graph");
                goto erret;
            }
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
            ip[i] = av.intvalue;
        }
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;      /* end SEQ OF */
    }
    else if (atp == BYTE_GRAPH_values)
        sgp->values = av.ptrvalue;    /* already in a bytestore */

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* end struct of Seq-graph.graph */
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* end struct of Seq-graph */
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return sgp;
erret:
    aip->io_failure = TRUE;
    sgp = SeqGraphFree(sgp);
    goto ret;
}

/*****************************************************************************
*
*   SeqGraphSetAsnWrite(sgp, aip, set, element)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqGraphSetAsnWrite (SeqGraphPtr sgp, AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element)
{
	AsnTypePtr atp;
	SeqGraphPtr oldsgp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqResAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(element, SEQ_GRAPH);   /* link local tree */
    if (atp == NULL) return FALSE;

	oldsgp = sgp;
    if (! AsnOpenStruct(aip, set, (Pointer)oldsgp)) goto erret;  /* set of */

    while (sgp != NULL)
    {
        if (! SeqGraphAsnWrite(sgp, aip, atp)) goto erret;
        sgp = sgp->next;
    }
    if (! AsnCloseStruct(aip, set, (Pointer)oldsgp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(element);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqGraphSetAsnRead(aip, atp)
*
*****************************************************************************/
NLM_EXTERN SeqGraphPtr LIBCALL SeqGraphSetAsnRead (AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element)
{
	DataVal av;
	AsnTypePtr atp;
    SeqGraphPtr sgp, first = NULL, curr = NULL;

	if (! loaded)
	{
		if (! SeqResAsnLoad())
			return first;
	}

	if (aip == NULL)
		return first;

    AsnLinkType(element, SEQ_GRAPH);    /* link in local tree */

    atp = set;
	if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* read the start struct */
	while ((atp = AsnReadId(aip, amp, atp)) == element)  /* get the values */
    {
        sgp = SeqGraphAsnRead(aip, atp);
        if (sgp == NULL) goto erret;
        if (first == NULL)
            first = sgp;
        else
            curr->next = sgp;
        curr = sgp;
    }
    if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end struct */
ret:
	AsnUnlinkType(element);       /* unlink local tree */
	return first;
erret:
    while (first != NULL)
    {
        curr = first;
        first = curr->next;
        SeqGraphFree(curr);
    }
    goto ret;
}

