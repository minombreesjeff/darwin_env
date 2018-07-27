/*  objcode.c
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
* File Name:  objcode.c
*
* Author:  James Ostell
*   
* Version Creation Date: 4/1/91
*
* $Revision: 6.10 $
*
* File Description:  Object manager for module NCBI-SeqCode
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 05-13-93 Schuler     All public functions are now declared LIBCALL.
*
*
* $Log: objcode.c,v $
* Revision 6.10  2004/04/01 13:43:07  lavr
* Spell "occurred", "occurrence", and "occurring"
*
* Revision 6.9  2002/09/13 15:07:17  kans
* restored U Selenocysteine to iupacaa rather than kludge asn2gnbk
*
* Revision 6.8  2002/06/27 19:45:39  kans
* reverted U Selenocysteine addition to iupac
*
* Revision 6.7  2002/06/27 13:24:39  kans
* added U Selenocysteine to iupacaa, changed conversion tables to reflect that
*
* Revision 6.6  1999/04/23 12:47:43  madden
* Use PATH_MAX in SeqCodeSetLoad
*
* Revision 6.5  1999/01/19 19:38:57  vakatov
* Typo fixed(WIN16 rather than WIN_16)
*
* Revision 6.4  1999/01/19 19:08:14  kans
* text asn.1 for genetic code and sequence conversion tables only if not WIN_16
*
* Revision 6.3  1999/01/06 22:02:15  kans
* load gen code and seq code tables from asn strings in static memory as last resort to avoid need for data directory for some programs
*
* Revision 6.2  1998/12/29 19:56:26  kans
* more informative error message if FindPath fails
*
* Revision 6.1  1998/08/24 18:28:01  kans
* removed solaris -v -fd warnings
*
* Revision 6.0  1997/08/25 18:49:32  madden
* Revision changed to 6.0
*
* Revision 4.1  1997/06/19 18:40:59  vakatov
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
#include <objcode.h>		   /* the pub interface */
#include <asncode.h>        /* the AsnTool header */

static Boolean loaded = FALSE;

static SeqCodeSetPtr scspl = NULL; /* loaded SeqCodeTables and SeqMapTables */

/*****************************************************************************
*
*   SeqCodeAsnLoad()
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqCodeAsnLoad (void)
{
    if (loaded)
        return TRUE;
    loaded = TRUE;

    if (! AsnLoad())
    {
        loaded = FALSE;
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************
*
*   SeqMapTableNew()
*
*****************************************************************************/
NLM_EXTERN SeqMapTablePtr LIBCALL SeqMapTableNew (void)
{
    return (SeqMapTablePtr)MemNew(sizeof(SeqMapTable));
}

/*****************************************************************************
*
*   SeqMapTableFree(smtp)
*       Frees a SeqMapTable and associated data
*
*****************************************************************************/
NLM_EXTERN SeqMapTablePtr LIBCALL SeqMapTableFree (SeqMapTablePtr smtp)
{
    if (smtp == NULL)
        return smtp;
    MemFree(smtp->table);
	return (SeqMapTablePtr)MemFree(smtp);
}
/*****************************************************************************
*
*   SeqMapTableAsnWrite(smtp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqMapTable ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMapTableAsnWrite (SeqMapTablePtr smtp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Uint1 i, num;
    Uint1Ptr ipnt;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqCodeAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, SEQ_MAP_TABLE);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (smtp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)smtp)) goto erret;
    
    av.intvalue = smtp->from;
    if (! AsnWrite(aip, SEQ_MAP_TABLE_from, &av)) goto erret;
    av.intvalue = smtp->to;
    if (! AsnWrite(aip, SEQ_MAP_TABLE_to, &av)) goto erret;
    num = smtp->num;
    av.intvalue = (Int4)num;
    if (! AsnWrite(aip, SEQ_MAP_TABLE_num, &av)) goto erret;

    if (smtp->start_at)
    {
        av.intvalue = (Int4)smtp->start_at;
        if (! AsnWrite(aip, SEQ_MAP_TABLE_start_at, &av)) goto erret;
    }
    if (! AsnOpenStruct(aip, SEQ_MAP_TABLE_table, (Pointer)smtp->table)) goto erret;
    ipnt = smtp->table;
    for (i = 0; i < num; i++, ipnt++)
    {
        av.intvalue = (Int4)*ipnt;
        if (! AsnWrite(aip, SEQ_MAP_TABLE_table_E, &av)) goto erret;
    }
    if (! AsnCloseStruct(aip, SEQ_MAP_TABLE_table, (Pointer)smtp->table)) goto erret;

    if (! AsnCloseStruct(aip, atp, (Pointer)smtp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqMapTableAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN SeqMapTablePtr LIBCALL SeqMapTableAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    SeqMapTablePtr smtp=NULL;
    Uint1 i, num;
    Uint1Ptr ipnt;

	if (! loaded)
	{
		if (! SeqCodeAsnLoad())
			return smtp;
	}

	if (aip == NULL)
		return smtp;

	if (orig == NULL)           /* SeqMapTable ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_MAP_TABLE);
	else
		atp = AsnLinkType(orig, SEQ_MAP_TABLE);    /* link in local tree */
    if (atp == NULL) return smtp;

    smtp = SeqMapTableNew();
    if (smtp == NULL) goto erret;
    
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the start struct */
    
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    smtp->from = (Uint1) av.intvalue;

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    smtp->to = (Uint1) av.intvalue;

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    num = (Uint1)av.intvalue;
    smtp->num = num;
    ipnt = (Uint1Ptr)MemNew((num * sizeof(Uint1)));
    if (ipnt == NULL) goto erret;
    smtp->table = ipnt;

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;    /* start SEQUENCE OF */
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    if (atp == SEQ_MAP_TABLE_start_at)
    {
        smtp->start_at = (Uint1)av.intvalue;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;    /* start SEQUENCE OF */
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    }
    i = 0;
    while ((atp = AsnReadId(aip, amp, atp)) == SEQ_MAP_TABLE_table_E)
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        if (i < num)
            *ipnt = (Uint1)av.intvalue;
        else
		{
            ErrPost(CTX_NCBIOBJ, 1, "Too many codes in Seq-map-table. line %ld",
                aip->linenumber);
			goto erret;
		}
        ipnt++; i++;
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end SEQUENCE OF */
    if (i != num)
	{
        ErrPost(CTX_NCBIOBJ, 1, "Too few codes in Seq-map-table. line %ld",
            aip->linenumber);
		goto erret;
	}

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* end struct */
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return smtp;
erret:
    smtp = SeqMapTableFree(smtp);
    goto ret;
}

/*****************************************************************************
*
*   SeqMapTablePtr SeqMapTableFindObj(to, from)
*
*****************************************************************************/
NLM_EXTERN SeqMapTablePtr LIBCALL SeqMapTableFindObj (Uint1 to, Uint1 from)
{
    SeqMapTablePtr smtp=NULL;

    if (scspl == NULL)
    {
        if ((scspl = SeqCodeSetLoad()) == NULL)
            return smtp;
    }

    smtp = scspl->maps;
    while (smtp != NULL)
    {
        if ((smtp->to == to) && (smtp->from == from))
            return smtp;
        smtp = smtp->next;
    }
    return smtp;
}

/*****************************************************************************
*
*   SeqCodeTableNew()
*
*****************************************************************************/
NLM_EXTERN SeqCodeTablePtr LIBCALL SeqCodeTableNew (void)
{
    return (SeqCodeTablePtr)MemNew(sizeof(SeqCodeTable));
}

/*****************************************************************************
*
*   SeqCodeTableFree(sctp)
*       Frees a SeqCodeTable and associated data
*
*****************************************************************************/
NLM_EXTERN SeqCodeTablePtr LIBCALL SeqCodeTableFree (SeqCodeTablePtr sctp)
{
    CharPtr PNTR tmp;
    Uint1 num, i;

    if (sctp == NULL)
        return sctp;

    MemFree(sctp->letters);
    num = sctp->num;
    if (sctp->symbols != NULL)
    {
        tmp = sctp->symbols;
        for (i = 0; i < num; i++, tmp++)
            MemFree(*tmp);
        MemFree(sctp->symbols);
    }
    if (sctp->names != NULL)
    {
        tmp = sctp->names;
        for (i = 0; i < num; i++, tmp++)
            MemFree(*tmp);
        MemFree(sctp->names);
    }
    if (sctp->comps != NULL)
        MemFree(sctp->comps);
	return (SeqCodeTablePtr)MemFree(sctp);
}

/*****************************************************************************
*
*   SeqCodeTableAsnWrite(sctp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqCodeTable ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqCodeTableAsnWrite (SeqCodeTablePtr sctp, AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    Uint1 i, num;
    Char tbuf[2];
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqCodeAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, SEQ_CODE_TABLE);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (sctp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)sctp)) goto erret;
    
    av.intvalue = sctp->code;
    if (! AsnWrite(aip, SEQ_CODE_TABLE_code, &av)) goto erret;
    num = sctp->num;
    av.intvalue = (Int4)num;
    if (! AsnWrite(aip, SEQ_CODE_TABLE_num, &av)) goto erret;
    av.boolvalue = sctp->one_letter;
    if (! AsnWrite(aip, SEQ_CODE_TABLE_one_letter, &av)) goto erret;
    if (sctp->start_at)
    {
        av.intvalue = (Int4)sctp->start_at;
        if (! AsnWrite(aip, SEQ_CODE_TABLE_start_at, &av)) goto erret;
    }

    if (! AsnOpenStruct(aip, SEQ_CODE_TABLE_table, (Pointer)sctp)) goto erret;
    tbuf[1] = '\0';

    for (i = 0; i < num; i++)
    {
        if (! AsnOpenStruct(aip, SEQ_CODE_TABLE_table_E, (Pointer)sctp)) goto erret;
        if (sctp->one_letter)
        {
            tbuf[0] = sctp->letters[i];
            av.ptrvalue = tbuf;
        }
        else
            av.ptrvalue = sctp->symbols[i];
        if (! AsnWrite(aip, SEQ_CODE_TABLE_table_E_symbol, &av)) goto erret;

        av.ptrvalue = sctp->names[i];
        if (! AsnWrite(aip, SEQ_CODE_TABLE_table_E_name, &av)) goto erret;
        if (! AsnCloseStruct(aip, SEQ_CODE_TABLE_table_E, (Pointer)sctp)) goto erret;
    }
    if (! AsnCloseStruct(aip, SEQ_CODE_TABLE_table, (Pointer)sctp)) goto erret;

    if (sctp->comps != NULL)
    {
        if (! AsnOpenStruct(aip, SEQ_CODE_TABLE_comps, (Pointer)sctp->comps)) goto erret;
        for (i = 0; i < num; i++)
        {
            av.intvalue = (Int4)sctp->comps[i];
            if (! AsnWrite(aip, SEQ_CODE_TABLE_comps_E, &av)) goto erret;
        }
        if (! AsnCloseStruct(aip, SEQ_CODE_TABLE_comps, (Pointer)sctp->comps)) goto erret;
    }
    if (! AsnCloseStruct(aip, atp, (Pointer)sctp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqCodeTableAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN SeqCodeTablePtr LIBCALL SeqCodeTableAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp;
    SeqCodeTablePtr sctp=NULL;
    Uint1 i, num;

	if (! loaded)
	{
		if (! SeqCodeAsnLoad())
			return sctp;
	}

	if (aip == NULL)
		return sctp;

	if (orig == NULL)           /* SeqCodeTable ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_CODE_TABLE);
	else
		atp = AsnLinkType(orig, SEQ_CODE_TABLE);    /* link in local tree */
    if (atp == NULL) return sctp;

    sctp = SeqCodeTableNew();
    if (sctp == NULL) goto erret;
    
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the start struct */
    
    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    sctp->code = (Uint1) av.intvalue;

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    num = (Uint1)av.intvalue;
    sctp->num = num;

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    sctp->one_letter = av.boolvalue;

    if (sctp->one_letter)
    {
        sctp->letters = (CharPtr)MemNew((num * sizeof(Char)));
        if (sctp->letters == NULL) goto erret;
    }
    else
    {
        sctp->symbols = (CharPtr PNTR)MemNew((num * sizeof(CharPtr)));
        if (sctp->symbols == NULL) goto erret;
    }
	sctp->names = (CharPtr PNTR)MemNew((num * sizeof(CharPtr)));
    if (sctp->names == NULL) goto erret;

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;    /* start SEQUENCE OF */
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    if (atp == SEQ_CODE_TABLE_start_at)
    {
        sctp->start_at = (Uint1)av.intvalue;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;    /* start SEQUENCE OF */
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
    }
    i = 0;
    while ((atp = AsnReadId(aip, amp, atp)) == SEQ_CODE_TABLE_table_E)
    {
        if (i >= num)
		{
            ErrPost(CTX_NCBIOBJ, 1, "Too many codes in Seq-code-table %s. line %ld",
                sctp->code, aip->linenumber);
			goto erret;
		}
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* start struct */
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* symbol */
        if (sctp->one_letter)
        {
            sctp->letters[i] = *(CharPtr)av.ptrvalue;
            MemFree(av.ptrvalue);
        }
        else
            sctp->symbols[i] = (CharPtr)av.ptrvalue;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* name */
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        sctp->names[i] = (CharPtr)av.ptrvalue;
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;   /* end struct */
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        i++;
    }
    if (atp == NULL) goto erret;
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end SEQUENCE OF */
    if (i != num)
	{
        ErrPost(CTX_NCBIOBJ, 1, "Too few codes in Seq-code-table %s. line %ld",
            sctp->code, aip->linenumber);
		goto erret;
	}

    atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    if (atp == SEQ_CODE_TABLE_comps)   /* comps present */
    {
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
        sctp->comps = (Uint1Ptr)MemNew((num * sizeof(Uint1)));
        if (sctp->comps == NULL) goto erret;
        i = 0;
        while ((atp = AsnReadId(aip, amp, atp)) == SEQ_CODE_TABLE_comps_E)
        {
            if (i == num)
			{
                ErrPost(CTX_NCBIOBJ, 1, "Too many comps in Seq-code-table. line %ld",
                    aip->linenumber);
				goto erret;
			}
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
            sctp->comps[i] = (Uint1)av.intvalue;
            i++;
        }
        if (i != num)
		{
            ErrPost(CTX_NCBIOBJ, 1, "Too few comps in Seq-code-table. line %ld",
                aip->linenumber);
			goto erret;
		}
        if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* end sequence of */
        atp = AsnReadId(aip, amp, atp); if (atp == NULL) goto erret;
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return sctp;
erret:
    sctp = SeqCodeTableFree(sctp);
    goto ret;
}

/*****************************************************************************
*
*   SeqCodeTablePtr SeqCodeTableFindObj(code)
*
*****************************************************************************/
NLM_EXTERN SeqCodeTablePtr LIBCALL SeqCodeTableFindObj (Uint1 code)
{
    SeqCodeTablePtr sctp=NULL;

    if (scspl == NULL)
    {
        if ((scspl = SeqCodeSetLoad()) == NULL)
            return sctp;
    }

    sctp = scspl->codes;
    while (sctp != NULL)
    {
        if (sctp->code == code)
            return sctp;
        sctp = sctp->next;
    }
    return sctp;
}

/*****************************************************************************
*
*   SeqCodeSetNew()
*
*****************************************************************************/
NLM_EXTERN SeqCodeSetPtr LIBCALL SeqCodeSetNew (void)
{
    return (SeqCodeSetPtr)MemNew(sizeof(SeqCodeSet));
}

/*****************************************************************************
*
*   SeqCodeSetFree(scsp)
*       Frees a SeqCodeSet and associated data
*
*****************************************************************************/
NLM_EXTERN SeqCodeSetPtr LIBCALL SeqCodeSetFree (SeqCodeSetPtr scsp)
{
    SeqCodeTablePtr sctp, sctpnext;
    SeqMapTablePtr smtp, smtpnext;

    sctp = scsp->codes;
    while (sctp != NULL)
    {
        sctpnext = sctp->next;
        SeqCodeTableFree(sctp);
        sctp = sctpnext;
    }
    smtp = scsp->maps;
    while (smtp != NULL)
    {
        smtpnext = smtp->next;
        SeqMapTableFree(smtp);
        smtp = smtpnext;
    }
	return (SeqCodeSetPtr)MemFree(scsp);
}

/*****************************************************************************
*
*   SeqCodeSetAsnWrite(scsp, aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*       if atp == NULL, then assumes it stands alone (SeqCodeSet ::=)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqCodeSetAsnWrite (SeqCodeSetPtr scsp, AsnIoPtr aip, AsnTypePtr orig)
{
	AsnTypePtr atp;
    SeqMapTablePtr smtp;
    SeqCodeTablePtr sctp;
    Boolean retval = FALSE;

	if (! loaded)
	{
		if (! SeqCodeAsnLoad())
			return FALSE;
	}

	if (aip == NULL)
		return FALSE;

	atp = AsnLinkType(orig, SEQ_CODE_SET);   /* link local tree */
    if (atp == NULL) return FALSE;

	if (scsp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

    if (! AsnOpenStruct(aip, atp, (Pointer)scsp)) goto erret;

    if (scsp->codes != NULL)
    {
        if (! AsnOpenStruct(aip, SEQ_CODE_SET_codes, (Pointer)scsp->codes)) goto erret;
        sctp = scsp->codes;
        while (sctp != NULL)
        {
            if (! SeqCodeTableAsnWrite(sctp, aip, SEQ_CODE_SET_codes_E)) goto erret;
            sctp = sctp->next;
        }
        if (! AsnCloseStruct(aip, SEQ_CODE_SET_codes, (Pointer)scsp->codes)) goto erret;
    }

    if (scsp->maps != NULL)
    {
        if (! AsnOpenStruct(aip, SEQ_CODE_SET_maps, (Pointer)scsp->maps)) goto erret;
        smtp = scsp->maps;
        while (smtp != NULL)
        {
            if (! SeqMapTableAsnWrite(smtp, aip, SEQ_CODE_SET_maps_E)) goto erret;
            smtp = smtp->next;
        }
        if (! AsnCloseStruct(aip, SEQ_CODE_SET_maps, (Pointer)scsp->maps)) goto erret;
    }

    if (! AsnCloseStruct(aip, atp, (Pointer)scsp)) goto erret;
    retval = TRUE;
erret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return retval;
}

/*****************************************************************************
*
*   SeqCodeSetAsnRead(aip, atp)
*   	atp is the current type (if identifier of a parent struct)
*            assumption is readIdent has occurred
*       if atp == NULL, then assumes it stands alone and read ident
*            has not occurred.
*
*****************************************************************************/
NLM_EXTERN SeqCodeSetPtr LIBCALL SeqCodeSetAsnRead (AsnIoPtr aip, AsnTypePtr orig)
{
	DataVal av;
	AsnTypePtr atp, oldatp;
    SeqCodeSetPtr scsp=NULL;
    SeqMapTablePtr map, currmap = NULL;
    SeqCodeTablePtr code, currcode = NULL;

	if (! loaded)
	{
		if (! SeqCodeAsnLoad())
			return scsp;
	}

	if (aip == NULL)
		return scsp;

	if (orig == NULL)           /* SeqCodeSet ::= (self contained) */
		atp = AsnReadId(aip, amp, SEQ_CODE_SET);
	else
		atp = AsnLinkType(orig, SEQ_CODE_SET);    /* link in local tree */
    oldatp = atp;
	if (atp == NULL) return scsp;

    scsp = SeqCodeSetNew();
	if (scsp == NULL) goto erret;
    
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;   /* read the start struct */
    
    while((atp = AsnReadId(aip, amp, atp)) != oldatp)
    {
		if (atp == NULL) goto erret;
        if (atp == SEQ_CODE_SET_codes_E)
        {
            code = SeqCodeTableAsnRead(aip, atp);
			if (code == NULL) goto erret;
            if (scsp->codes == NULL)
                scsp->codes = code;
            else
                currcode->next = code;
            currcode = code;
        }
        else if (atp == SEQ_CODE_SET_maps_E)
        {
            map = SeqMapTableAsnRead(aip, atp);
			if (map == NULL) goto erret;
            if (scsp->maps == NULL)
                scsp->maps = map;
            else
                currmap->next = map;
            currmap = map;
        }
        else                 /* the other struct ends */
		{
            if (AsnReadVal(aip, atp, &av) <= 0) goto erret;
		}
    }
    if (AsnReadVal(aip, atp, &av) <= 0) goto erret;  /* end struct */
ret:
	AsnUnlinkType(orig);       /* unlink local tree */
	return scsp;
erret:
	scsp = SeqCodeSetFree(scsp);
	goto ret;
}

/*****************************************************************************
*
*   seqCodeSetMemStr as last resort embedded version of seqcode.prt
*
*****************************************************************************/

#ifndef WIN16
static CharPtr seqCodeSetMemStr = "Seq-code-set ::= {\n" \
"codes {\n" \
"{ code iupacna , num 25 , one-letter TRUE , start-at 65 , table {\n" \
"{ symbol \"A\", name \"Adenine\" },\n" \
"{ symbol \"B\" , name \"G or T or C\" },\n" \
"{ symbol \"C\", name \"Cytosine\" },\n" \
"{ symbol \"D\", name \"G or A or T\" },\n" \
"{ symbol \"\", name \"\" },\n" \
"{ symbol \"\", name \"\" },\n" \
"{ symbol \"G\", name \"Guanine\" },\n" \
"{ symbol \"H\", name \"A or C or T\" } ,\n" \
"{ symbol \"\", name \"\" },\n" \
"{ symbol \"\", name \"\" },\n" \
"{ symbol \"K\", name \"G or T\" },\n" \
"{ symbol \"\", name \"\"},\n" \
"{ symbol \"M\", name \"A or C\" },\n" \
"{ symbol \"N\", name \"A or G or C or T\" } ,\n" \
"{ symbol \"\", name \"\" },\n" \
"{ symbol \"\", name \"\" },\n" \
"{ symbol \"\", name \"\"},\n" \
"{ symbol \"R\", name \"G or A\"},\n" \
"{ symbol \"S\", name \"G or C\"},\n" \
"{ symbol \"T\", name \"Thymine\"},\n" \
"{ symbol \"\", name \"\"},\n" \
"{ symbol \"V\", name \"G or C or A\"},\n" \
"{ symbol \"W\", name \"A or T\" },\n" \
"{ symbol \"\", name \"\"},\n" \
"{ symbol \"Y\", name \"T or C\"} } ,\n" \
"comps { 84, 86, 71, 72, 69, 70, 67, 68, 73, 74, 77, 76, 75, 78, 79, 80, 81, 89, 83, 65, 85, 66, 87, 88, 82 } } ,\n" \
"{ code iupacaa , num 26 , one-letter TRUE , start-at 65 , table {\n" \
"{ symbol \"A\", name \"Alanine\" },\n" \
"{ symbol \"B\" , name \"Asp or Asn\" },\n" \
"{ symbol \"C\", name \"Cysteine\" },\n" \
"{ symbol \"D\", name \"Aspartic Acid\" },\n" \
"{ symbol \"E\", name \"Glutamic Acid\" },\n" \
"{ symbol \"F\", name \"Phenylalanine\" },\n" \
"{ symbol \"G\", name \"Glycine\" },\n" \
"{ symbol \"H\", name \"Histidine\"} ,\n" \
"{ symbol \"I\", name \"Isoleucine\" },\n" \
"{ symbol \"\", name \"\" },\n" \
"{ symbol \"K\", name \"Lysine\" },\n" \
"{ symbol \"L\", name \"Leucine\" },\n" \
"{ symbol \"M\", name \"Methionine\" },\n" \
"{ symbol \"N\", name \"Asparagine\"} ,\n" \
"{ symbol \"\", name \"\" },\n" \
"{ symbol \"P\", name \"Proline\" },\n" \
"{ symbol \"Q\", name \"Glutamine\"},\n" \
"{ symbol \"R\", name \"Arginine\"},\n" \
"{ symbol \"S\", name \"Serine\"},\n" \
"{ symbol \"T\", name \"Threonine\"},\n" \
"{ symbol \"U\", name \"Selenocysteine\"},\n" \
"{ symbol \"V\", name \"Valine\"},\n" \
"{ symbol \"W\", name \"Tryptophan\" },\n" \
"{ symbol \"X\", name \"Undetermined or atypical\"},\n" \
"{ symbol \"Y\", name \"Tyrosine\"},\n" \
"{ symbol \"Z\", name \"Glu or Gln\" } } } ,\n" \
"{ code ncbieaa , num 49 , one-letter TRUE , start-at 42 , table {\n" \
"{ symbol \"*\", name \"Termination\" } ,\n" \
"{ symbol \"\", name \"\" } ,\n" \
"{ symbol \"\", name \"\" } ,\n" \
"{ symbol \"-\", name \"Gap\" } ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"\", name \"\"} ,\n" \
"{ symbol \"A\", name \"Alanine\" },\n" \
"{ symbol \"B\" , name \"Asp or Asn\" },\n" \
"{ symbol \"C\", name \"Cysteine\" },\n" \
"{ symbol \"D\", name \"Aspartic Acid\" },\n" \
"{ symbol \"E\", name \"Glutamic Acid\" },\n" \
"{ symbol \"F\", name \"Phenylalanine\" },\n" \
"{ symbol \"G\", name \"Glycine\" },\n" \
"{ symbol \"H\", name \"Histidine\" } ,\n" \
"{ symbol \"I\", name \"Isoleucine\" },\n" \
"{ symbol \"\", name \"\" },\n" \
"{ symbol \"K\", name \"Lysine\" },\n" \
"{ symbol \"L\", name \"Leucine\" },\n" \
"{ symbol \"M\", name \"Methionine\" },\n" \
"{ symbol \"N\", name \"Asparagine\" } ,\n" \
"{ symbol \"\", name \"\" },\n" \
"{ symbol \"P\", name \"Proline\" },\n" \
"{ symbol \"Q\", name \"Glutamine\"},\n" \
"{ symbol \"R\", name \"Arginine\"},\n" \
"{ symbol \"S\", name \"Serine\"},\n" \
"{ symbol \"T\", name \"Threonine\"},\n" \
"{ symbol \"U\", name \"Selenocysteine\"},\n" \
"{ symbol \"V\", name \"Valine\"},\n" \
"{ symbol \"W\", name \"Tryptophan\" },\n" \
"{ symbol \"X\", name \"Undetermined or atypical\"},\n" \
"{ symbol \"Y\", name \"Tyrosine\"},\n" \
"{ symbol \"Z\", name \"Glu or Gln\" } } } ,\n" \
"{ code iupacaa3 , num 26 , one-letter FALSE , table {\n" \
"{ symbol \"---\", name \"Gap\" } ,\n" \
"{ symbol \"Ala\", name \"Alanine\" },\n" \
"{ symbol \"Asx\", name \"Asp or Asn\" },\n" \
"{ symbol \"Cys\", name \"Cysteine\" },\n" \
"{ symbol \"Asp\", name \"Aspartic Acid\" },\n" \
"{ symbol \"Glu\", name \"Glutamic Acid\" },\n" \
"{ symbol \"Phe\", name \"Phenylalanine\" },\n" \
"{ symbol \"Gly\", name \"Glycine\" },\n" \
"{ symbol \"His\", name \"Histidine\" } ,\n" \
"{ symbol \"Ile\", name \"Isoleucine\" },\n" \
"{ symbol \"Lys\", name \"Lysine\" },\n" \
"{ symbol \"Leu\", name \"Leucine\" },\n" \
"{ symbol \"Met\", name \"Methionine\" },\n" \
"{ symbol \"Asn\", name \"Asparagine\" } ,\n" \
"{ symbol \"Pro\", name \"Proline\" },\n" \
"{ symbol \"Gln\", name \"Glutamine\"},\n" \
"{ symbol \"Arg\", name \"Arginine\"},\n" \
"{ symbol \"Ser\", name \"Serine\"},\n" \
"{ symbol \"Thr\", name \"Threonine\"},\n" \
"{ symbol \"Val\", name \"Valine\"},\n" \
"{ symbol \"Trp\", name \"Tryptophan\" },\n" \
"{ symbol \"Xxx\", name \"Undetermined or atypical\"},\n" \
"{ symbol \"Tyr\", name \"Tyrosine\"},\n" \
"{ symbol \"Glx\", name \"Glu or Gln\" },\n" \
"{ symbol \"Sec\", name \"Selenocysteine\"},\n" \
"{ symbol \"Ter\", name \"Termination\" } } } ,\n" \
"{ code ncbistdaa , num 26 , one-letter TRUE , table {\n" \
"{ symbol \"-\", name \"Gap\" } ,\n" \
"{ symbol \"A\", name \"Alanine\" },\n" \
"{ symbol \"B\" , name \"Asp or Asn\" },\n" \
"{ symbol \"C\", name \"Cysteine\" },\n" \
"{ symbol \"D\", name \"Aspartic Acid\" },\n" \
"{ symbol \"E\", name \"Glutamic Acid\" },\n" \
"{ symbol \"F\", name \"Phenylalanine\" },\n" \
"{ symbol \"G\", name \"Glycine\" },\n" \
"{ symbol \"H\", name \"Histidine\" } ,\n" \
"{ symbol \"I\", name \"Isoleucine\" },\n" \
"{ symbol \"K\", name \"Lysine\" },\n" \
"{ symbol \"L\", name \"Leucine\" },\n" \
"{ symbol \"M\", name \"Methionine\" },\n" \
"{ symbol \"N\", name \"Asparagine\" } ,\n" \
"{ symbol \"P\", name \"Proline\" },\n" \
"{ symbol \"Q\", name \"Glutamine\"},\n" \
"{ symbol \"R\", name \"Arginine\"},\n" \
"{ symbol \"S\", name \"Serine\"},\n" \
"{ symbol \"T\", name \"Threoine\"},\n" \
"{ symbol \"V\", name \"Valine\"},\n" \
"{ symbol \"W\", name \"Tryptophan\" },\n" \
"{ symbol \"X\", name \"Undetermined or atypical\"},\n" \
"{ symbol \"Y\", name \"Tyrosine\"},\n" \
"{ symbol \"Z\", name \"Glu or Gln\" },\n" \
"{ symbol \"U\", name \"Selenocysteine\"},\n" \
"{ symbol \"*\", name \"Termination\" } } } ,\n" \
"{ code ncbi2na , num 4 , one-letter TRUE , table {\n" \
"{ symbol \"A\", name \"Adenine\" },\n" \
"{ symbol \"C\", name \"Cytosine\" },\n" \
"{ symbol \"G\", name \"Guanine\" },\n" \
"{ symbol \"T\", name \"Thymine/Uracil\"} } ,\n" \
"comps { 3, 2, 1, 0 } } ,\n" \
"{ code ncbi4na , num 16 , one-letter TRUE , table {\n" \
"{ symbol \"-\", name \"Gap\"} ,\n" \
"{ symbol \"A\", name \"Adenine\" },\n" \
"{ symbol \"C\", name \"Cytosine\" },\n" \
"{ symbol \"M\", name \"A or C\" },\n" \
"{ symbol \"G\", name \"Guanine\" },\n" \
"{ symbol \"R\", name \"G or A\"},\n" \
"{ symbol \"S\", name \"G or C\"},\n" \
"{ symbol \"V\", name \"G or C or A\"},\n" \
"{ symbol \"T\", name \"Thymine/Uracil\"},\n" \
"{ symbol \"W\", name \"A or T\" },\n" \
"{ symbol \"Y\", name \"T or C\"} ,\n" \
"{ symbol \"H\", name \"A or C or T\" } ,\n" \
"{ symbol \"K\", name \"G or T\" },\n" \
"{ symbol \"D\", name \"G or A or T\" },\n" \
"{ symbol \"B\" , name \"G or T or C\" },\n" \
"{ symbol \"N\", name \"A or G or C or T\" } } ,\n" \
"comps { 0 , 8 , 4 , 12, 2 , 10, 6 , 14, 1 , 9 , 5 , 13, 3 , 11, 7 , 15 } } , } ,\n" \
"maps { { from iupacna , to ncbi2na , num 25 , start-at 65 ,\n" \
"table { 0, 1, 1, 2, 255, 255, 2, 0, 255, 255, 2, 255, 1, 0, 255, 255, 255, 2, 1, 3, 255, 0, 3, 255, 3 } } ,\n" \
"{ from iupacna , to ncbi4na , num 26 , start-at 64 ,\n" \
"table { 0, 1, 14, 2, 13, 255, 255, 4, 11, 255, 255, 12, 255, 3, 15, 255, 255, 255, 5, 6, 8, 255, 7, 9, 255, 10 } } ,\n" \
"{ from ncbi2na , to iupacna , num 4 ,\n" \
"table { 65, 67, 71, 84 } } ,\n" \
"{ from ncbi2na , to ncbi4na , num 4 ,\n" \
"table { 1, 2, 4, 8 } } ,\n" \
"{ from ncbi4na , to iupacna , num 16 ,\n" \
"table { 78, 65, 67, 77, 71, 82, 83, 86, 84, 87, 89, 72, 75, 68, 66, 78 } } ,\n" \
"{ from ncbi4na , to ncbi2na , num 16 ,\n" \
"table { 3, 0, 1, 1, 2, 2, 1, 0, 3, 3, 3, 0, 2, 2, 1, 0 } } ,\n" \
"{ from iupacaa , to ncbieaa , num 26 , start-at 65 ,\n" \
"table { 65 , 66 , 67 , 68, 69, 70, 71, 72, 73, 255, 75, 76, 77, 78, 255, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90 } } ,\n" \
"{ from ncbieaa , to iupacaa , num 49 , start-at 42 ,\n" \
"table { 88 , 255, 255, 88, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,\n" \
"65 , 66 , 67 , 68, 69, 70, 71, 72, 73, 255, 75, 76, 77, 78, 255, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90 } } ,\n" \
"{ from iupacaa , to ncbistdaa , num 26 , start-at 65 ,\n" \
"table { 1 , 2 , 3 , 4, 5, 6, 7, 8, 9, 255, 10, 11, 12, 13, 255, 14, 15, 16, 17, 18, 24, 19, 20, 21, 22, 23 } } ,\n" \
"{ from ncbieaa , to ncbistdaa , num 49 , start-at 42 ,\n" \
"table { 25, 255, 255, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,\n" \
"1 , 2 , 3 , 4, 5, 6, 7, 8, 9, 255, 10, 11, 12, 13, 255, 14, 15, 16, 17, 18, 24, 19, 20, 21, 22, 23 } } ,\n" \
"{ from ncbistdaa , to ncbieaa , num 26 ,\n" \
"table { 45 , 65 , 66 , 67 , 68, 69, 70, 71, 72, 73, 75, 76, 77, 78, 80, 81, 82, 83, 84, 86, 87, 88, 89, 90, 85, 42} } ,\n" \
"{ from ncbistdaa , to iupacaa , num 26 ,\n" \
"table { 255 , 65 , 66 , 67 , 68, 69, 70, 71, 72, 73, 75, 76, 77, 78, 80, 81, 82, 83, 84, 86, 87, 88, 89, 90, 85, 255} } } };\n";
#endif

/*****************************************************************************
*
*   SeqCodeSetPtr SeqCodeSetLoad()
*       loads all current seqcodes
*       looks for "seqcode.val" in the "data" directory
*       now uses seqCodeSetMemStr if unable to find "data" directory
*
*****************************************************************************/
static Boolean LoadSeqCodeFromLocalString (void)

{
#ifndef WIN16
  AsnIoMemPtr aimp;

  aimp = AsnIoMemOpen ("r", (BytePtr) seqCodeSetMemStr, (Int4) StringLen (seqCodeSetMemStr));
  if (aimp == NULL || aimp->aip == NULL) return FALSE;
  scspl = SeqCodeSetAsnRead (aimp->aip, NULL);
  AsnIoMemClose (aimp);
#endif
  return (Boolean) (scspl != NULL);
}

NLM_EXTERN SeqCodeSetPtr LIBCALL SeqCodeSetLoad (void)
{
    Char buf[PATH_MAX];
    AsnIoPtr aip;

	if (scspl != NULL)
		return scspl;

    if (! FindPath("ncbi", "ncbi", "data", buf, sizeof (buf)))
	{

		if (LoadSeqCodeFromLocalString ()) {
			return scspl;
		}

		ErrPost(CTX_NCBIOBJ, 1, "FindPath failed in SeqCodeSetLoad - ncbi configuration file missing or incorrect");
        return scspl;
	}

    StringCat(buf, "seqcode.val");
    if ((aip = AsnIoOpen(buf, "rb")) == NULL)
	{

		if (LoadSeqCodeFromLocalString ()) {
			return scspl;
		}

		ErrPost(CTX_NCBIOBJ, 1, "Couldn't open [%s]", buf);
        return scspl;
	}

    scspl = SeqCodeSetAsnRead(aip, NULL);

    AsnIoClose(aip);
    return scspl;
}


