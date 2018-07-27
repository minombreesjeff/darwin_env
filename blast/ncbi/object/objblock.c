/*  objblock.c
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
* File Name:  objblock.c
*
* Author:  James Ostell
*   
* Version Creation Date: 8/10/92
*
* $Revision: 6.3 $
*
* File Description:  Object manager for module GenBank-General,
*   					EMBL-General, PIR-General, SWISSPROT-General
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 05-13-93 Schuler     All public functions are now declared LIBCALL.
* 05-17-93 Schuler     pdbblock.class renamed pdpblock.pdbclass because
*                      class is a C++ keyword.
*
* $Log: objblock.c,v $
* Revision 6.3  2004/04/01 13:43:07  lavr
* Spell "occurred", "occurrence", and "occurring"
*
* Revision 6.2  2003/05/16 17:21:05  bazhin
* Fixed bug in PrfBlockAsnWrite().
*
* Revision 6.1  1998/08/24 18:27:59  kans
* removed solaris -v -fd warnings
*
* Revision 6.0  1997/08/25 18:49:26  madden
* Revision changed to 6.0
*
* Revision 4.1  1997/06/19 18:40:54  vakatov
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
#include <objblock.h>		   /* the public interface */
#include <asnblock.h>        /* the AsnTool header */

static Boolean loaded = FALSE;

/*****************************************************************************
*
*   SeqBlockAsnLoad()
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqBlockAsnLoad (void)
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
    if (! AsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************
*
*   PirBlockNew()
*
*****************************************************************************/
NLM_EXTERN PirBlockPtr LIBCALL PirBlockNew (void)
{
    return (PirBlockPtr)MemNew(sizeof(PirBlock));
}

/*****************************************************************************
*
*   PirBlockFree(pbp)
*       Frees a PirBlock and associated data
*
*****************************************************************************/
NLM_EXTERN PirBlockPtr LIBCALL PirBlockFree (PirBlockPtr pbp)
{
    if (pbp == NULL)
        return pbp;

    MemFree(pbp->host);
    MemFree(pbp->source);
    MemFree(pbp->summary);
    MemFree(pbp->genetic);
    MemFree(pbp->includes);
    MemFree(pbp->placement);
    MemFree(pbp->superfamily);
    ValNodeFreeData(pbp->keywords);
    MemFree(pbp->cross_reference);
    MemFree(pbp->date);
    MemFree(pbp->seq_raw);
    SeqIdSetFree(pbp->seqref);
	return (PirBlockPtr)MemFree(pbp);
}
/*****************************************************************************
*
*   PirBlockAsnWrite(pbp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (PirBlock ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL PirBlockAsnWrite (PirBlockPtr pbp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    ValNodePtr anp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqBlockAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, PIR_BLOCK);   /* link local tree */
    if (atp == NULL) goto erret;

	if (pbp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)pbp)) goto erret;

    if (pbp->had_punct)
    {
        av.boolvalue = pbp->had_punct;
        if (! AsnWrite(aip, PIR_BLOCK_had_punct, &av)) goto erret;
    }
    if (pbp->host != NULL)
    {
        av.ptrvalue = pbp->host;
        if (! AsnWrite(aip, PIR_BLOCK_host, &av)) goto erret;
    }
    if (pbp->source != NULL)
    {
        av.ptrvalue = pbp->source;
        if (! AsnWrite(aip, PIR_BLOCK_source, &av)) goto erret;
    }
    if (pbp->summary != NULL)
    {
        av.ptrvalue = pbp->summary;
        if (! AsnWrite(aip, PIR_BLOCK_summary, &av)) goto erret;
    }
    if (pbp->genetic != NULL)
    {
        av.ptrvalue = pbp->genetic;
        if (! AsnWrite(aip, PIR_BLOCK_genetic, &av)) goto erret;
    }
    if (pbp->includes != NULL)
    {
        av.ptrvalue = pbp->includes;
        if (! AsnWrite(aip, PIR_BLOCK_includes, &av)) goto erret;
    }
    if (pbp->placement != NULL)
    {
        av.ptrvalue = pbp->placement;
        if (! AsnWrite(aip, PIR_BLOCK_placement, &av)) goto erret;
    }
    if (pbp->superfamily != NULL)
    {
        av.ptrvalue = pbp->superfamily;
        if (! AsnWrite(aip, PIR_BLOCK_superfamily, &av)) goto erret;
    }
    if (pbp->keywords != NULL)
    {
        if (! AsnOpenStruct(aip, PIR_BLOCK_keywords, (Pointer)pbp->keywords)) goto erret;
        anp = pbp->keywords;
        while (anp != NULL)
        {
            if (! AsnWrite(aip, PIR_BLOCK_keywords_E, &anp->data)) goto erret;
            anp = anp->next;
        }
        if (! AsnCloseStruct(aip, PIR_BLOCK_keywords, (Pointer)pbp->keywords)) goto erret;
    }
    if (pbp->cross_reference != NULL)
    {
        av.ptrvalue = pbp->cross_reference;
        if (! AsnWrite(aip, PIR_BLOCK_cross_reference, &av)) goto erret;
    }
    if (pbp->date != NULL)
    {
        av.ptrvalue = pbp->date;
        if (! AsnWrite(aip, PIR_BLOCK_date, &av)) goto erret;
    }
    if (pbp->seq_raw != NULL)
    {
        av.ptrvalue = pbp->seq_raw;
        if (! AsnWrite(aip, PIR_BLOCK_seq_raw, &av)) goto erret;
    }
    if (pbp->seqref != NULL)
	{
        if (! SeqIdSetAsnWrite(pbp->seqref, aip, PIR_BLOCK_seqref, PIR_BLOCK_seqref_E))
			goto erret;
	}

    if (! AsnCloseStruct(aip, atp, (Pointer)pbp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   PirBlockAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN PirBlockPtr LIBCALL PirBlockAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldatp;
    PirBlockPtr pbp=NULL;
    ValNodePtr tmp, tmp2;

	if (! loaded)
	{
		if (! SeqBlockAsnLoad())
			return pbp;
	}

	if (aip == NULL)
		return pbp;

	if (orig == NULL)           /* PirBlock ::= (self contained) */
		atp = AsnReadId(aip, amp, PIR_BLOCK);
	else
		atp = AsnLinkType(orig, PIR_BLOCK);    /* link in local tree */
    oldatp = atp;
    if (atp == NULL) return pbp;

    pbp = PirBlockNew();
    if (pbp == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the start struct */
    
    while ((atp = AsnReadId(aip, amp, atp)) != oldatp)
    {
        if (atp == NULL) goto erret;
        if (atp == PIR_BLOCK_seqref)
		{
            pbp->seqref = SeqIdSetAsnRead(aip, atp, PIR_BLOCK_seqref_E);
			if (pbp->seqref == NULL) goto erret;
		}
		else
		{
	        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    	    if (atp == PIR_BLOCK_host)
        	    pbp->host = (CharPtr)av.ptrvalue;
	        else if (atp == PIR_BLOCK_source)
    	        pbp->source = (CharPtr)av.ptrvalue;
        	else if (atp == PIR_BLOCK_had_punct)
            	pbp->had_punct = av.boolvalue;
	        else if (atp == PIR_BLOCK_cross_reference)
   	         pbp->cross_reference = (CharPtr)av.ptrvalue;
    	    else if (atp == PIR_BLOCK_seq_raw)
        	    pbp->seq_raw = (CharPtr)av.ptrvalue;
	        else if (atp == PIR_BLOCK_summary)
    	        pbp->summary = (CharPtr)av.ptrvalue;
        	else if (atp == PIR_BLOCK_genetic)
            	pbp->genetic = (CharPtr)av.ptrvalue;
	        else if (atp == PIR_BLOCK_includes)
    	        pbp->includes = (CharPtr)av.ptrvalue;
        	else if (atp == PIR_BLOCK_placement)
            	pbp->placement = (CharPtr)av.ptrvalue;
	        else if (atp == PIR_BLOCK_superfamily)
    	        pbp->superfamily = (CharPtr)av.ptrvalue;
        	else if (atp == PIR_BLOCK_date)
            	pbp->date = (CharPtr)av.ptrvalue;
	        else if (atp == PIR_BLOCK_keywords)
    	    {
        	    tmp = NULL;
            	while ((atp = AsnReadId(aip, amp, atp)) == PIR_BLOCK_keywords_E)
	            {
    	            tmp2 = ValNodeNew(tmp);
        	        if (tmp2 == NULL) goto erret;
            	    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
                	tmp2->data.ptrvalue = av.ptrvalue;
	                if (tmp == NULL)
    	                pbp->keywords = tmp2;
        	        tmp = tmp2;
            	}
	            if (atp == NULL) goto erret;
    	        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;  /* end struct */
        	}
		}
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return pbp;
erret:
    pbp = PirBlockFree(pbp);
    goto ret;
}

/*****************************************************************************
*
*   GBBlockNew()
*
*****************************************************************************/
NLM_EXTERN GBBlockPtr LIBCALL GBBlockNew (void)
{
    return (GBBlockPtr)MemNew(sizeof(GBBlock));
}

/*****************************************************************************
*
*   GBBlockFree(gbp)
*       Frees a GBBlock and associated data
*
*****************************************************************************/
NLM_EXTERN GBBlockPtr LIBCALL GBBlockFree (GBBlockPtr gbp)
{
    if (gbp == NULL)
        return gbp;

    ValNodeFreeData(gbp->extra_accessions);
    MemFree(gbp->source);
    MemFree(gbp->origin);
    ValNodeFreeData(gbp->keywords);
    MemFree(gbp->date);
    MemFree(gbp->div);
    MemFree(gbp->taxonomy);
    DateFree(gbp->entry_date);
	return (GBBlockPtr)MemFree(gbp);
}

/*****************************************************************************
*
*   GBBlockAsnWrite(gbp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (GBBlock ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL GBBlockAsnWrite (GBBlockPtr gbp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    ValNodePtr anp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqBlockAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, GB_BLOCK);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (gbp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)gbp)) goto erret;

    if (gbp->extra_accessions != NULL)
    {
        if (! AsnOpenStruct(aip, GB_BLOCK_extra_accessions, (Pointer)gbp->extra_accessions)) goto erret;
        anp = gbp->extra_accessions;
        while (anp != NULL)
        {
            if (! AsnWrite(aip, GB_BLOCK_extra_accessions_E, &anp->data)) goto erret;
            anp = anp->next;
        }
        if (! AsnCloseStruct(aip, GB_BLOCK_extra_accessions, (Pointer)gbp->extra_accessions)) goto erret;
    }
    if (gbp->source != NULL)
    {
        av.ptrvalue = gbp->source;
        if (! AsnWrite(aip, GB_BLOCK_source, &av)) goto erret;
    }
    if (gbp->keywords != NULL)
    {
        if (! AsnOpenStruct(aip, GB_BLOCK_keywords, (Pointer)gbp->keywords)) goto erret;
        anp = gbp->keywords;
        while (anp != NULL)
        {
            if (! AsnWrite(aip, GB_BLOCK_keywords_E, &anp->data)) goto erret;
            anp = anp->next;
        }
        if (! AsnCloseStruct(aip, GB_BLOCK_keywords, (Pointer)gbp->keywords)) goto erret;
    }
    if (gbp->origin != NULL)
    {
        av.ptrvalue = gbp->origin;
        if (! AsnWrite(aip, GB_BLOCK_origin, &av)) goto erret;
    }
    if (gbp->date != NULL)
    {
        av.ptrvalue = gbp->date;
        if (! AsnWrite(aip, GB_BLOCK_date, &av)) goto erret;
    }
    if (gbp->entry_date != NULL)
    {
        if (! DateAsnWrite(gbp->entry_date, aip, GB_BLOCK_entry_date)) goto erret;
    }
    if (gbp->div != NULL)
    {
        av.ptrvalue = gbp->div;
        if (! AsnWrite(aip, GB_BLOCK_div, &av)) goto erret;
    }
    if (gbp->taxonomy != NULL)
    {
        av.ptrvalue = gbp->taxonomy;
        if (! AsnWrite(aip, GB_BLOCK_taxonomy, &av)) goto erret;
    }

    if (! AsnCloseStruct(aip, atp, (Pointer)gbp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   GBBlockAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN GBBlockPtr LIBCALL GBBlockAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldatp;
    GBBlockPtr gbp=NULL;
    ValNodePtr tmp, tmp2;

	if (! loaded)
	{
		if (! SeqBlockAsnLoad())
			return gbp;
	}

	if (aip == NULL)
		return gbp;

	if (orig == NULL)           /* GBBlock ::= (self contained) */
		atp = AsnReadId(aip, amp, GB_BLOCK);
	else
		atp = AsnLinkType(orig, GB_BLOCK);    /* link in local tree */
    oldatp = atp;
    if (atp == NULL) return gbp;

    gbp = GBBlockNew();
    if (gbp == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the start struct */
    
    while ((atp = AsnReadId(aip, amp, atp)) != oldatp)
    {
        if (atp == NULL) goto erret;
        if (atp == GB_BLOCK_entry_date)
        {
            gbp->entry_date = DateAsnRead(aip, atp);
            if (gbp->entry_date == NULL) goto erret;
        }
        else
        {
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
            if (atp == GB_BLOCK_source)
                gbp->source = (CharPtr)av.ptrvalue;
            else if (atp == GB_BLOCK_origin)
                gbp->origin = (CharPtr)av.ptrvalue;
            else if (atp == GB_BLOCK_date)
                gbp->date = (CharPtr)av.ptrvalue;
            else if (atp == GB_BLOCK_keywords)
            {
                tmp = NULL;
                while ((atp = AsnReadId(aip, amp, atp)) == GB_BLOCK_keywords_E)
                {
                    tmp2 = ValNodeNew(tmp);
                    if (tmp2 == NULL) goto erret;
                    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
                    tmp2->data.ptrvalue = av.ptrvalue;
                    if (tmp == NULL)
                        gbp->keywords = tmp2;
                    tmp = tmp2;
                }
                if (atp == NULL) goto erret;
                if (AsnReadVal(aip, atp, &av) <= 0) goto erret;  /* end struct */
            }
            else if (atp == GB_BLOCK_extra_accessions)
            {
                tmp = NULL;
                while ((atp = AsnReadId(aip, amp, atp)) == GB_BLOCK_extra_accessions_E)
                {
                    tmp2 = ValNodeNew(tmp);
                    if (tmp2 == NULL) goto erret;
                    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
                    tmp2->data.ptrvalue = av.ptrvalue;
                    if (tmp == NULL)
                        gbp->extra_accessions = tmp2;
                    tmp = tmp2;
                }
                if (atp == NULL) goto erret;
                if (AsnReadVal(aip, atp, &av) <= 0) goto erret;  /* end struct */
            }
            else if (atp == GB_BLOCK_div)
                gbp->div = (CharPtr)av.ptrvalue;
            else if (atp == GB_BLOCK_taxonomy)
                gbp->taxonomy = (CharPtr)av.ptrvalue;
        }
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return gbp;
erret:
    gbp = GBBlockFree(gbp);
    goto ret;
}

/*****************************************************************************
*
*   SPBlockNew()
*
*****************************************************************************/
NLM_EXTERN SPBlockPtr LIBCALL SPBlockNew (void)
{
    return (SPBlockPtr)MemNew(sizeof(SPBlock));
}

/*****************************************************************************
*
*   SPBlockFree(sbp)
*       Frees a SPBlock and associated data
*
*****************************************************************************/
NLM_EXTERN SPBlockPtr LIBCALL SPBlockFree (SPBlockPtr sbp)
{
    ValNodePtr anp, next;

    if (sbp == NULL)
        return sbp;

    ValNodeFreeData(sbp->extra_acc);
    ValNodeFreeData(sbp->plasnm);
    SeqIdSetFree(sbp->seqref);
    anp = sbp->dbref;
    while (anp != NULL)
    {
        next = anp->next;
        DbtagFree((DbtagPtr)anp->data.ptrvalue);
        MemFree(anp);
        anp = next;
    }
    ValNodeFreeData(sbp->keywords);
	DateFree(sbp->created);
	DateFree(sbp->sequpd);
	DateFree(sbp->annotupd);
	return (SPBlockPtr)MemFree(sbp);
}

/*****************************************************************************
*
*   SPBlockAsnWrite(sbp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SPBlock ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SPBlockAsnWrite (SPBlockPtr sbp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    ValNodePtr anp;
	Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqBlockAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, SP_BLOCK);   /* link local tree */
	if (atp == NULL) return FALSE;

	if (sbp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)sbp)) goto erret;

    av.intvalue = sbp->_class;
    if (! AsnWrite(aip, SP_BLOCK_class, &av)) goto erret;

    if (sbp->extra_acc != NULL)
    {
        if (! AsnOpenStruct(aip, SP_BLOCK_extra_acc, (Pointer)sbp->extra_acc)) goto erret;
        anp = sbp->extra_acc;
        while (anp != NULL)
        {
            if (! AsnWrite(aip, SP_BLOCK_extra_acc_E, &anp->data)) goto erret;
            anp = anp->next;
        }
        if (! AsnCloseStruct(aip, SP_BLOCK_extra_acc, (Pointer)sbp->extra_acc)) goto erret;
    }
    if (sbp->imeth)
    {
        av.boolvalue = sbp->imeth;
        if (! AsnWrite(aip, SP_BLOCK_imeth, &av)) goto erret;
    }
    if (sbp->plasnm != NULL)
    {
        if (! AsnOpenStruct(aip, SP_BLOCK_plasnm, (Pointer)sbp->plasnm)) goto erret;
        anp = sbp->plasnm;
        while (anp != NULL)
        {
            if (! AsnWrite(aip, SP_BLOCK_plasnm_E, &anp->data)) goto erret;
            anp = anp->next;
        }
        if (! AsnCloseStruct(aip, SP_BLOCK_plasnm, (Pointer)sbp->plasnm)) goto erret;
    }
    if (sbp->seqref != NULL)
	{
        if (! SeqIdSetAsnWrite(sbp->seqref, aip, SP_BLOCK_seqref, SP_BLOCK_seqref_E))
			goto erret;
	}

    if (sbp->dbref != NULL)
    {
        if (! AsnOpenStruct(aip, SP_BLOCK_dbref, (Pointer)sbp->dbref)) goto erret;
        anp = sbp->dbref;
        while (anp != NULL)
        {
            if (! DbtagAsnWrite((DbtagPtr)anp->data.ptrvalue, aip, SP_BLOCK_dbref_E))
				goto erret;
            anp = anp->next;
        }
        if (! AsnCloseStruct(aip, SP_BLOCK_dbref, (Pointer)sbp->dbref)) goto erret;
    }

    if (sbp->keywords != NULL)
    {
        if (! AsnOpenStruct(aip, SP_BLOCK_keywords, (Pointer)sbp->keywords)) goto erret;
        anp = sbp->keywords;
        while (anp != NULL)
        {
            if (! AsnWrite(aip, SP_BLOCK_keywords_E, &anp->data)) goto erret;
            anp = anp->next;
        }
        if (! AsnCloseStruct(aip, SP_BLOCK_keywords, (Pointer)sbp->keywords)) goto erret;
    }

	if (sbp->created != NULL)
	{
		if (! DateAsnWrite(sbp->created, aip, SP_BLOCK_created)) goto erret;
	}

	if (sbp->sequpd != NULL)
	{
		if (! DateAsnWrite(sbp->sequpd, aip, SP_BLOCK_sequpd)) goto erret;
	}

	if (sbp->annotupd != NULL)
	{
		if (! DateAsnWrite(sbp->annotupd, aip, SP_BLOCK_annotupd)) goto erret;
	}

    if (! AsnCloseStruct(aip, atp, (Pointer)sbp)) goto erret;
	retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SPBlockAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN SPBlockPtr LIBCALL SPBlockAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldatp;
    SPBlockPtr sbp=NULL;
    ValNodePtr tmp, tmp2;

	if (! loaded)
	{
		if (! SeqBlockAsnLoad())
			return sbp;
	}

	if (aip == NULL)
		return sbp;

	if (orig == NULL)           /* SPBlock ::= (self contained) */
		atp = AsnReadId(aip, amp, SP_BLOCK);
	else
		atp = AsnLinkType(orig, SP_BLOCK);    /* link in local tree */
    oldatp = atp;
	if (atp == NULL) return sbp;

    sbp = SPBlockNew();
	if (sbp == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the start struct */
    
    while ((atp = AsnReadId(aip, amp, atp)) != oldatp)
    {
		if (atp == NULL) goto erret;
        if (atp == SP_BLOCK_seqref)
		{
            sbp->seqref = SeqIdSetAsnRead(aip, atp, SP_BLOCK_seqref_E);
			if (sbp->seqref == NULL) goto erret;
		}
		else if (atp == SP_BLOCK_created)
		{
			sbp->created = DateAsnRead(aip, atp);
			if (sbp->created == NULL) goto erret;
		}
		else if (atp == SP_BLOCK_sequpd)
		{
			sbp->sequpd = DateAsnRead(aip, atp);
			if (sbp->sequpd == NULL) goto erret;
		}
		else if (atp == SP_BLOCK_annotupd)
		{
			sbp->annotupd = DateAsnRead(aip, atp);
			if (sbp->annotupd == NULL) goto erret;
		}
        else
        {
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
            if (atp == SP_BLOCK_class)
                sbp->_class = (Uint1) av.intvalue;
            else if (atp == SP_BLOCK_imeth)
                sbp->imeth = av.boolvalue;
            else if (atp == SP_BLOCK_extra_acc)
            {
                tmp = NULL;
                while ((atp = AsnReadId(aip, amp, atp)) == SP_BLOCK_extra_acc_E)
                {
                    tmp2 = ValNodeNew(tmp);
					if (tmp2 == NULL) goto erret;
                    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
                    tmp2->data.ptrvalue = av.ptrvalue;
                    if (tmp == NULL)
                        sbp->extra_acc = tmp2;
                    tmp = tmp2;
                }
				if (atp == NULL) goto erret;
                if (AsnReadVal(aip, atp, &av) <= 0) goto erret;  /* end struct */
            }
            else if (atp == SP_BLOCK_plasnm)
            {
                tmp = NULL;
                while ((atp = AsnReadId(aip, amp, atp)) == SP_BLOCK_plasnm_E)
                {
                    tmp2 = ValNodeNew(tmp);
					if (tmp2 == NULL) goto erret;
                    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
                    tmp2->data.ptrvalue = av.ptrvalue;
                    if (tmp == NULL)
                        sbp->plasnm = tmp2;
                    tmp = tmp2;
                }
				if (atp == NULL) goto erret;
                if (AsnReadVal(aip, atp, &av) <= 0) goto erret;  /* end struct */
            }
            else if (atp == SP_BLOCK_keywords)
            {
                tmp = NULL;
                while ((atp = AsnReadId(aip, amp, atp)) == SP_BLOCK_keywords_E)
                {
                    tmp2 = ValNodeNew(tmp);
					if (tmp2 == NULL) goto erret;
                    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
                    tmp2->data.ptrvalue = av.ptrvalue;
                    if (tmp == NULL)
                        sbp->keywords = tmp2;
                    tmp = tmp2;
                }
				if (atp == NULL) goto erret;
                if (AsnReadVal(aip, atp, &av) <= 0) goto erret;  /* end struct */
            }
            else if (atp == SP_BLOCK_dbref)
            {
                tmp = NULL;
                while ((atp = AsnReadId(aip, amp, atp)) == SP_BLOCK_dbref_E)
                {
                    tmp2 = ValNodeNew(tmp);
					if (tmp2 == NULL) goto erret;
                    tmp2->data.ptrvalue = (Pointer)DbtagAsnRead(aip, atp);
					if (tmp2->data.ptrvalue == NULL) goto erret;
                    if (tmp == NULL)
                        sbp->dbref = tmp2;
                    tmp = tmp2;
                }
				if (atp == NULL) goto erret;
                if (AsnReadVal(aip, atp, &av) <= 0) goto erret;  /* end struct */
            }
        }
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return sbp;
erret:
	sbp = SPBlockFree(sbp);
	goto ret;
}

/*****************************************************************************
*
*   EMBLBlockNew()
*
*****************************************************************************/
NLM_EXTERN EMBLBlockPtr LIBCALL EMBLBlockNew (void)
{
	EMBLBlockPtr ebp;

    ebp = (EMBLBlockPtr)MemNew(sizeof(EMBLBlock));
	if (ebp == NULL) return NULL;
	ebp->div = 255;    /* for not-set */
	ebp->_class = 1;   /* for standard */
	return ebp;
}

/*****************************************************************************
*
*   EMBLBlockFree(ebp)
*       Frees a EMBLBlock and associated data
*
*****************************************************************************/
NLM_EXTERN EMBLBlockPtr LIBCALL EMBLBlockFree (EMBLBlockPtr ebp)
{
    ValNodePtr anp, next;
	EMBLXrefPtr ecurr, enext;

    if (ebp == NULL)
        return ebp;

	DateFree(ebp->creation_date);
	DateFree(ebp->update_date);
	ecurr = ebp->xref;
	while (ecurr != NULL)
	{
		enext = ecurr->next;
		MemFree(ecurr->name);
	    anp = ecurr->id;
	    while (anp != NULL)
    	{
        	next = anp->next;
			ObjectIdFree((ObjectIdPtr)anp->data.ptrvalue);
	        MemFree(anp);
    	    anp = next;
	    }
		MemFree(ecurr);
		ecurr = enext;
	}
    ValNodeFreeData(ebp->extra_acc);
    ValNodeFreeData(ebp->keywords);
	return (EMBLBlockPtr)MemFree(ebp);
}
/*****************************************************************************
*
*   EMBLBlockAsnWrite(ebp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (EMBLBlock ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL EMBLBlockAsnWrite (EMBLBlockPtr ebp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    ValNodePtr anp;
	EMBLXrefPtr xref;
	Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqBlockAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, EMBL_BLOCK);   /* link local tree */
	if (atp == NULL) return FALSE;

	if (ebp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)ebp)) goto erret;

    if (ebp->_class != 1) {
        av.intvalue = ebp->_class;
        if (! AsnWrite(aip, EMBL_BLOCK_class, &av)) goto erret;
    }
	if (ebp->div != 255)
	{
	    av.intvalue = ebp->div;
    	if (! AsnWrite(aip, EMBL_BLOCK_div, &av)) goto erret;
	}

	if (ebp->creation_date != NULL)
	{
		if (! DateAsnWrite(ebp->creation_date, aip, EMBL_BLOCK_creation_date)) goto erret;
	}

	if (ebp->update_date != NULL)
	{
		if (! DateAsnWrite(ebp->update_date, aip, EMBL_BLOCK_update_date)) goto erret;
	}

    if (ebp->extra_acc != NULL)
    {
        if (! AsnOpenStruct(aip, EMBL_BLOCK_extra_acc, (Pointer)ebp->extra_acc)) goto erret;
        anp = ebp->extra_acc;
        while (anp != NULL)
        {
            if (! AsnWrite(aip, EMBL_BLOCK_extra_acc_E, &anp->data)) goto erret;
            anp = anp->next;
        }
        if (! AsnCloseStruct(aip, EMBL_BLOCK_extra_acc, (Pointer)ebp->extra_acc)) goto erret;
    }

    if (ebp->keywords != NULL)
    {
        if (! AsnOpenStruct(aip, EMBL_BLOCK_keywords, (Pointer)ebp->keywords)) goto erret;
        anp = ebp->keywords;
        while (anp != NULL)
        {
            if (! AsnWrite(aip, EMBL_BLOCK_keywords_E, &anp->data)) goto erret;
            anp = anp->next;
        }
        if (! AsnCloseStruct(aip, EMBL_BLOCK_keywords, (Pointer)ebp->keywords)) goto erret;
    }

    if (ebp->xref != NULL)
    {
        if (! AsnOpenStruct(aip, EMBL_BLOCK_xref, (Pointer)ebp->xref)) goto erret;
        xref = ebp->xref;
		while (xref != NULL)
		{
			if (! AsnOpenStruct(aip, EMBL_BLOCK_xref_E, (Pointer)xref)) goto erret;

			if (xref->name != NULL)
			{
				av.ptrvalue = xref->name;
				if (! AsnWriteChoice(aip, EMBL_XREF_dbname, (Int2)0, &av)) goto erret;
				if (! AsnWrite(aip, EMBL_DBNAME_name, &av)) goto erret;
			}
			else
			{
				av.intvalue = xref->_class;
				if (! AsnWriteChoice(aip, EMBL_XREF_dbname, (Int2)1, &av)) goto erret;
				if (! AsnWrite(aip, EMBL_DBNAME_code, &av)) goto erret;
			}
			if (! AsnOpenStruct(aip, EMBL_XREF_id, (Pointer)xref->id)) goto erret;
			anp = xref->id;
	        while (anp != NULL)
    	    {
        	    if (! ObjectIdAsnWrite((ObjectIdPtr)anp->data.ptrvalue, aip, EMBL_XREF_id_E)) goto erret;
            	anp = anp->next;
	        }
			if (! AsnCloseStruct(aip, EMBL_XREF_id, (Pointer)xref->id)) goto erret;
			if (! AsnCloseStruct(aip, EMBL_BLOCK_xref_E, (Pointer)xref)) goto erret;
			xref = xref->next;
		}
        if (! AsnCloseStruct(aip, EMBL_BLOCK_xref, (Pointer)ebp->xref)) goto erret;
    }

    if (! AsnCloseStruct(aip, atp, (Pointer)ebp)) goto erret;
	retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   EMBLBlockAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN EMBLBlockPtr LIBCALL EMBLBlockAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldatp;
    EMBLBlockPtr ebp=NULL;
    ValNodePtr tmp, tmp2;
	EMBLXrefPtr etmp, etmp2;

	if (! loaded)
	{
		if (! SeqBlockAsnLoad())
			return ebp;
	}

	if (aip == NULL)
		return ebp;

	if (orig == NULL)           /* EMBLBlock ::= (self contained) */
		atp = AsnReadId(aip, amp, EMBL_BLOCK);
	else
		atp = AsnLinkType(orig, EMBL_BLOCK);    /* link in local tree */
    oldatp = atp;
	if (atp == NULL) return ebp;

    ebp = EMBLBlockNew();
	if (ebp == NULL) goto erret;

    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the start struct */
    
    while ((atp = AsnReadId(aip, amp, atp)) != oldatp)
    {
		if (atp == NULL) goto erret;
		if (atp == EMBL_BLOCK_creation_date)
		{
			ebp->creation_date = DateAsnRead(aip, atp);
			if (ebp->creation_date == NULL) goto erret;
		}
		else if (atp == EMBL_BLOCK_update_date)
		{
			ebp->update_date = DateAsnRead(aip, atp);
			if (ebp->update_date == NULL) goto erret;
		}
        else
        {
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
            if (atp == EMBL_BLOCK_class)
                ebp->_class = (Uint1) av.intvalue;
            else if (atp == EMBL_BLOCK_div)
                ebp->div = (Uint1) av.intvalue;
            else if (atp == EMBL_BLOCK_extra_acc)
            {
                tmp = NULL;
                while ((atp = AsnReadId(aip, amp, atp)) == EMBL_BLOCK_extra_acc_E)
                {
                    tmp2 = ValNodeNew(tmp);
					if (tmp2 == NULL) goto erret;
                    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
                    tmp2->data.ptrvalue = av.ptrvalue;
                    if (tmp == NULL)
                        ebp->extra_acc = tmp2;
                    tmp = tmp2;
                }
				if (atp == NULL) goto erret;
                if (AsnReadVal(aip, atp, &av) <= 0) goto erret;  /* end struct */
            }
            else if (atp == EMBL_BLOCK_keywords)
            {
                tmp = NULL;
                while ((atp = AsnReadId(aip, amp, atp)) == EMBL_BLOCK_keywords_E)
                {
                    tmp2 = ValNodeNew(tmp);
					if (tmp2 == NULL) goto erret;
                    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
                    tmp2->data.ptrvalue = av.ptrvalue;
                    if (tmp == NULL)
                        ebp->keywords = tmp2;
                    tmp = tmp2;
                }
				if (atp == NULL) goto erret;
                if (AsnReadVal(aip, atp, &av) <= 0) goto erret;  /* end struct */
            }
            else if (atp == EMBL_BLOCK_xref)
            {
				etmp = NULL;
				while ((atp = AsnReadId(aip, amp, atp)) == EMBL_BLOCK_xref_E)
				{
					if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* start struct */
					etmp2 = (EMBLXrefPtr)MemNew(sizeof(EMBLXref));
					if (etmp2 == NULL) goto erret;
					if (etmp == NULL)
						ebp->xref = etmp2;
					else
						etmp->next = etmp2;
					etmp = etmp2;

					atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* dbname CHOICE */
					if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
					atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* which CHOICE */
					if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
					if (atp == EMBL_DBNAME_name)
						etmp2->name = (CharPtr)av.ptrvalue;
					else
						etmp2->_class = (Uint1) av.intvalue;
					atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* start struct EMBL_XREF_id */
					if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
	                tmp = NULL;
    	            while ((atp = AsnReadId(aip, amp, atp)) == EMBL_XREF_id_E)
        	        {
            	        tmp2 = ValNodeNew(tmp);
						if (tmp2 == NULL) goto erret;
                	    tmp2->data.ptrvalue = (Pointer)ObjectIdAsnRead(aip, atp);
						if (tmp2->data.ptrvalue == NULL) goto erret;

						if (tmp == NULL)
                        	etmp2->id = tmp2;
	                    tmp = tmp2;
    	            }
					if (atp == NULL) goto erret;
        	        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;  /* end struct EMBL_XREF_id */

					atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;  /* end struct EMBL_BLOCK_xref_E */
					if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
				}
				if (atp == NULL) goto erret;
				if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end struct EMBL_BLOCK_xref */
            }
        }
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;    /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return ebp;
erret:
	ebp = EMBLBlockFree(ebp);
	goto ret;
}

/*****************************************************************************
*
*	PRF-Block	- PRF specific block
*		by A. Ogiwara
*
*****************************************************************************/

/*****************************************************************************
*
*	PrfBlockNew()
*
*****************************************************************************/
NLM_EXTERN PrfBlockPtr	LIBCALL PrfBlockNew(void)
{
	return (PrfBlockPtr)MemNew(sizeof(PrfBlock));
}

/*****************************************************************************
*
*	PrfBlockFree(pbp)
*		Frees a PrfBlock and associated data
*
*****************************************************************************/
NLM_EXTERN PrfBlockPtr	LIBCALL PrfBlockFree (PrfBlockPtr pbp)
{
	if ( pbp == NULL )
		return pbp;

	if ( pbp->extra_src ) {
		MemFree( pbp->extra_src->host );
		MemFree( pbp->extra_src->part );
		MemFree( pbp->extra_src->state );
		MemFree( pbp->extra_src->strain );
		MemFree( pbp->extra_src->taxon );
		MemFree(pbp->extra_src);
	}
	ValNodeFreeData( pbp->keywords );
	return (PrfBlockPtr)MemFree( pbp );
}

/*****************************************************************************
*
*	PrfBlockAsnWrite( pbp, aip, atp )
*
*****************************************************************************/
NLM_EXTERN Boolean	LIBCALL PrfBlockAsnWrite (PrfBlockPtr pbp, AsnIoPtr aip, AsnTypePtr	orig)
{
	PrfExtSrcPtr	pxsp;
	AsnTypePtr	atp;
	ValNodePtr	vnp;
	DataVal		val;
	Boolean		retval = FALSE;

	if ( ! loaded ) {
		if ( ! SeqBlockAsnLoad() )
			return FALSE;
	}

	if ( aip == NULL )
		return FALSE;
	
	atp = AsnLinkType( orig, PRF_BLOCK );
	if ( atp == NULL )	return FALSE;

	if ( pbp == NULL ) { AsnNullValueMsg( aip, atp );  goto erret; }

	if ( ! AsnOpenStruct( aip, atp, (Pointer)pbp ) )  goto erret;

	pxsp = pbp->extra_src;
	if ( pxsp && ( pxsp->host || pxsp->part || pxsp->state ||
	     pxsp->strain || pxsp->taxon ) ) {
		if ( ! AsnOpenStruct( aip, PRF_BLOCK_extra_src,
		     (Pointer)pxsp ) )
			goto erret;
		if ( pxsp->host ) {
			val.ptrvalue = pxsp->host;
			if ( ! AsnWrite( aip, PRF_EXTRASRC_host, &val ) )
				goto erret;
		}
		if ( pxsp->part ) {
			val.ptrvalue = pxsp->part;
			if ( ! AsnWrite( aip, PRF_EXTRASRC_part, &val ) )
				goto erret;
		}
		if ( pxsp->state ) {
			val.ptrvalue = pxsp->state;
			if ( ! AsnWrite( aip, PRF_EXTRASRC_state, &val ) )
				goto erret;
		}
		if ( pxsp->strain ) {
			val.ptrvalue = pxsp->strain;
			if ( ! AsnWrite( aip, PRF_EXTRASRC_strain, &val ) )
				goto erret;
		}
		if ( pxsp->taxon ) {
			val.ptrvalue = pxsp->taxon;
			if ( ! AsnWrite( aip, PRF_EXTRASRC_taxon, &val ) )
				goto erret;
		}
		if ( ! AsnCloseStruct( aip, PRF_BLOCK_extra_src,
		     (Pointer)pxsp ) )
			goto erret;
	}

	if ( pbp->keywords ) {
		vnp = pbp->keywords;
		if ( ! AsnOpenStruct( aip, PRF_BLOCK_keywords, (Pointer)vnp ) )
			goto erret;
		for ( ; vnp; vnp = vnp->next ) {
			if ( ! AsnWrite( aip, PRF_BLOCK_keywords_E,
			     &vnp->data ) )
				goto erret;
		}
		if ( ! AsnCloseStruct( aip, PRF_BLOCK_keywords, (Pointer)vnp ) )
			goto erret;
	}

	if ( ! AsnCloseStruct( aip, atp, (Pointer)pbp ) )  goto erret;
	retval = TRUE;

    erret:
	AsnUnlinkType( orig );		/* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*	PrfBlockAsnRead( aip, atp )
*
*****************************************************************************/
NLM_EXTERN PrfBlockPtr	LIBCALL PrfBlockAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	AsnTypePtr	atp, oldatp;
	PrfBlockPtr	pbp=NULL;
	DataVal		val;
	ValNodePtr	vnp, vnp2;

	if ( ! loaded ) {
		if ( ! SeqBlockAsnLoad() )	return pbp;
	}

	if ( aip == NULL )	return pbp;
	if ( orig == NULL )	atp = AsnReadId( aip, amp, PRF_BLOCK );
	else			atp = AsnLinkType( orig, PRF_BLOCK );
	oldatp = atp;
	if ( atp == NULL )	return pbp;

	pbp = PrfBlockNew();
	if ( pbp == NULL )	goto erret;
	pbp->extra_src = NULL;

	if ( AsnReadVal( aip, atp, &val ) <= 0 ) goto erret; /* start struct */

	while ( ( atp = AsnReadId( aip, amp, atp ) ) != oldatp ) {
		if ( atp == NULL )	goto erret;

		if ( atp == PRF_BLOCK_extra_src ) {
			if ( AsnReadVal( aip, atp, &val ) <= 0 ) goto erret;
			if ( pbp->extra_src == NULL )
				pbp->extra_src = (PrfExtSrcPtr)MemNew( sizeof(PrfExtSrc) );
			if ( pbp->extra_src == NULL )	goto erret;
		} else if ( atp == PRF_BLOCK_keywords ) {
			if ( AsnReadVal( aip, atp, &val ) <= 0 ) goto erret;
			vnp = NULL;
			while ( ( atp = AsnReadId( aip, amp, atp ) )
				== PRF_BLOCK_keywords_E ) {
				vnp2 = ValNodeNew( vnp );
				if ( vnp2 == NULL ) goto erret;
				if ( AsnReadVal( aip, atp, &val ) <= 0 )
					goto erret;
				vnp2->data.ptrvalue = val.ptrvalue;
				if ( vnp == NULL )
					pbp->keywords = vnp2;
				vnp = vnp2;
			}
			if ( atp == NULL ) goto erret;
			if ( AsnReadVal( aip, atp, &val ) <= 0 ) goto erret;
		} else {
			if ( AsnReadVal( aip, atp, &val ) <= 0 ) goto erret;
			if ( atp == PRF_EXTRASRC_host ) {
				pbp->extra_src->host = (CharPtr)val.ptrvalue;
			} else if ( atp == PRF_EXTRASRC_part ) {
				pbp->extra_src->part = (CharPtr)val.ptrvalue;
			} else if ( atp == PRF_EXTRASRC_state ) {
				pbp->extra_src->state = (CharPtr)val.ptrvalue;
			} else if ( atp == PRF_EXTRASRC_strain ) {
				pbp->extra_src->strain = (CharPtr)val.ptrvalue;
			} else if ( atp == PRF_EXTRASRC_taxon ) {
				pbp->extra_src->taxon = (CharPtr)val.ptrvalue;
			}
		}
	}

	if ( AsnReadVal( aip, atp, &val ) <= 0 ) goto erret; /* end struct */
    ret:
	AsnUnlinkType( orig );
	return pbp;
    erret:
	pbp = PrfBlockFree( pbp );
	goto ret;
}

/*****************************************************************************
*
*	PDB-Block	- PDB specific block
*
*****************************************************************************/

/*****************************************************************************
*
*	PdbBlockNew()
*
*****************************************************************************/
NLM_EXTERN PdbBlockPtr	LIBCALL PdbBlockNew(void)
{
	return (PdbBlockPtr)MemNew(sizeof(PdbBlock));
}

/*****************************************************************************
*
*	PdbBlockFree(pdbp)
*		Frees a PdbBlock and associated data
*
*****************************************************************************/
NLM_EXTERN PdbBlockPtr	LIBCALL PdbBlockFree (PdbBlockPtr pdbp)
{
	if ( pdbp == NULL )
		return pdbp;

	DateFree(pdbp->deposition);
	MemFree(pdbp->pdbclass);
	ValNodeFreeData( pdbp->compound );
	ValNodeFreeData(pdbp->source);
	MemFree(pdbp->exp_method);
	if (pdbp->replace != NULL)
	{
		DateFree(pdbp->replace->date);
		ValNodeFreeData(pdbp->replace->ids);
		MemFree(pdbp->replace);
	}
	return (PdbBlockPtr)MemFree( pdbp );
}

/*****************************************************************************
*
*	PdbBlockAsnWrite( pdbp, aip, atp )
*
*****************************************************************************/
NLM_EXTERN Boolean	LIBCALL PdbBlockAsnWrite (PdbBlockPtr pdbp,	AsnIoPtr aip, AsnTypePtr orig)
{
	PdbRepPtr	prp;
	AsnTypePtr	atp;
	ValNodePtr	vnp;
	DataVal		val;
	Boolean		retval = FALSE;

	if ( ! loaded ) {
		if ( ! SeqBlockAsnLoad() )
			return FALSE;
	}

	if ( aip == NULL )
		return FALSE;
	
	atp = AsnLinkType( orig, PDB_BLOCK );
	if ( atp == NULL )	return FALSE;

	if ( pdbp == NULL ) { AsnNullValueMsg( aip, atp );  goto erret; }

	if ( ! AsnOpenStruct( aip, atp, (Pointer)pdbp ) )  goto erret;

	if (! DateAsnWrite(pdbp->deposition, aip, PDB_BLOCK_deposition))
		goto erret;

	val.ptrvalue = (Pointer) pdbp->pdbclass;
	if (! AsnWrite(aip, PDB_BLOCK_class, &val)) goto erret;

	if ( pdbp->compound )
	{
		vnp = pdbp->compound;
		if ( ! AsnOpenStruct( aip, PDB_BLOCK_compound, (Pointer)vnp ) )
			goto erret;
		for ( ; vnp; vnp = vnp->next )
		{
			if ( ! AsnWrite( aip, PDB_BLOCK_compound_E, &vnp->data ) )
				goto erret;
		}
		if ( ! AsnCloseStruct( aip, PDB_BLOCK_compound, (Pointer)vnp ) )
			goto erret;
	}

	if ( pdbp->source )
	{
		vnp = pdbp->source;
		if ( ! AsnOpenStruct( aip, PDB_BLOCK_source, (Pointer)vnp ) )
			goto erret;
		for ( ; vnp; vnp = vnp->next )
		{
			if ( ! AsnWrite( aip, PDB_BLOCK_source_E, &vnp->data ) )
				goto erret;
		}
		if ( ! AsnCloseStruct( aip, PDB_BLOCK_source, (Pointer)vnp ) )
			goto erret;
	}

	if (pdbp->exp_method != NULL)
	{
		val.ptrvalue = (Pointer) pdbp->exp_method;
		if (! AsnWrite(aip, PDB_BLOCK_exp_method, &val)) goto erret;
	}

	if (pdbp->replace != NULL)
	{
		prp = pdbp->replace;
		if ( ! AsnOpenStruct( aip, PDB_BLOCK_replace, (Pointer)prp ) )
			goto erret;

		if (! DateAsnWrite(prp->date, aip, PDB_REPLACE_date)) goto erret;

		vnp = prp->ids;
		if ( ! AsnOpenStruct( aip, PDB_REPLACE_ids, (Pointer)vnp ) )
			goto erret;
		for ( ; vnp; vnp = vnp->next )
		{
			if ( ! AsnWrite( aip, PDB_REPLACE_ids_E, &vnp->data ) )
				goto erret;
		}
		if ( ! AsnCloseStruct( aip, PDB_REPLACE_ids, (Pointer)vnp ) )
			goto erret;

		if ( ! AsnCloseStruct( aip, PDB_BLOCK_replace, (Pointer)prp ) )
			goto erret;

	}

	if ( ! AsnCloseStruct( aip, atp, (Pointer)pdbp ) )  goto erret;
	retval = TRUE;

    erret:
	AsnUnlinkType( orig );		/* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*	PdbBlockAsnRead( aip, atp )
*
*****************************************************************************/
NLM_EXTERN PdbBlockPtr	LIBCALL PdbBlockAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	AsnTypePtr	atp, oldatp;
	PdbBlockPtr	pdbp=NULL;
	PdbRepPtr prp = NULL;
	DataVal		val;
	ValNodePtr	vnp;

	if ( ! loaded ) {
		if ( ! SeqBlockAsnLoad() )	return pdbp;
	}

	if ( aip == NULL )	return pdbp;
	if ( orig == NULL )
		atp = AsnReadId( aip, amp, PDB_BLOCK );
	else
		atp = AsnLinkType( orig, PDB_BLOCK );
	oldatp = atp;
	if ( atp == NULL )	return pdbp;

	pdbp = PdbBlockNew();
	if ( pdbp == NULL )	goto erret;

	if ( AsnReadVal( aip, atp, &val ) <= 0 ) goto erret; /* start struct */

	while ( ( atp = AsnReadId( aip, amp, atp ) ) != oldatp )
	{
		if ( atp == NULL )	goto erret;

		if (atp == PDB_BLOCK_deposition)
		{
			pdbp->deposition = DateAsnRead(aip, atp);
			if (pdbp->deposition == NULL) goto erret;
		}
		else if (atp == PDB_REPLACE_date)
		{
			prp->date = DateAsnRead(aip, atp);
			if (prp->date == NULL) goto erret;
		}
		else
		{
			if (! AsnReadVal(aip, atp, &val)) goto erret;
			if (atp == PDB_BLOCK_replace)
			{
				if (prp == NULL)
				{
					prp = (PdbRepPtr)MemNew(sizeof(PdbRep));
					if (prp == NULL) goto erret;
					pdbp->replace = prp;
				}
			}
			else if (atp == PDB_BLOCK_class)
				pdbp->pdbclass = (CharPtr)val.ptrvalue;
			else if (atp == PDB_BLOCK_compound_E)
			{
				vnp = ValNodeNew(pdbp->compound);
				if (vnp == NULL) goto erret;
				if (pdbp->compound == NULL)
					pdbp->compound = vnp;
				vnp->data.ptrvalue = val.ptrvalue;
			}
			else if (atp == PDB_BLOCK_source_E)
			{
				vnp = ValNodeNew(pdbp->source);
				if (vnp == NULL) goto erret;
				if (pdbp->source == NULL)
					pdbp->source = vnp;
				vnp->data.ptrvalue = val.ptrvalue;
			}
			else if (atp == PDB_BLOCK_exp_method)
				pdbp->exp_method = (CharPtr)val.ptrvalue;
			else if (atp == PDB_REPLACE_ids_E)
			{
				vnp = ValNodeNew(prp->ids);
				if (vnp == NULL) goto erret;
				if (prp->ids == NULL)
					prp->ids = vnp;
				vnp->data.ptrvalue = val.ptrvalue;
			}
		}
	}

	if ( AsnReadVal( aip, atp, &val ) <= 0 ) goto erret; /* end struct */
    ret:
	AsnUnlinkType( orig );
	return pdbp;
    erret:
	pdbp = PdbBlockFree( pdbp );
	goto ret;
}

