/*  asnlex.c
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
* File Name: asnlex.c
*
* Author:  James Ostell
*
* Version Creation Date: 3/4/91
*
* $Revision: 6.5 $
*
* File Description:
*   Routines for parsing ASN.1 value nototation (text) messages
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 2/11/91  Ostell      AsnTxtReadVal - added check for unresolved base types
* 3/4/91   Kans        Stricter typecasting for GNU C and C++
* 3/4/91   Kans        AsnLexReadBoolean returns Boolean
* 04-20-93 Schuler     LIBCALL calling convention
*
* $Log: asnlex.c,v $
* Revision 6.5  2000/12/12 15:56:12  ostell
* added support BigInt
*
* Revision 6.4  2000/03/10 18:02:05  kans
* increased size of tbuf in AsnLexReadOctets to handle long lines in hand-edited (?) ASN.1 records being submitted
*
* Revision 6.3  1999/12/23 17:25:44  kans
* AsnTxtReadVal checks for NULL atp - same as recent check in AsnBinReadVal
*
* Revision 6.2  1998/06/12 19:27:51  kans
* fixed unix compiler warnings
*
* Revision 6.1  1997/10/29 02:41:31  vakatov
* Type castings to pass through the C++ compiler
*
* Revision 6.0  1997/08/25 18:10:04  madden
* Revision changed to 6.0
*
* Revision 5.3  1997/04/23 23:03:33  ostell
* just added a typecast
*
 * Revision 5.2  1997/04/23  21:23:03  ostell
 * changed BitHex reading routine to strip internal spaces and allow for linewraps at
 * any spacing.
 *
 * Revision 5.1  1996/12/03  21:43:48  vakatov
 * Adopted for 32-bit MS-Windows DLLs
 *
 * Revision 5.0  1996/05/28  14:00:29  ostell
 * Set to revision 5.0
 *
 * Revision 4.2  1996/02/18  16:45:36  ostell
 * changed fix_non_print behavior and added option 3
 *
 * Revision 4.1  1995/10/28  15:02:06  ostell
 * added casts to quiet DOS compile warnings
 *
 * Revision 4.0  1995/07/26  13:47:38  ostell
 * force revision to 4.0
 *
 * Revision 2.16  1995/05/15  18:38:28  ostell
 * added Log line
 *
*
* ==========================================================================
*/

/*****************************************************************************
*
*   asnlex.c
*   	special text lexxer for asn.1
*
*****************************************************************************/

#include "asnbuild.h"


/*****************************************************************************
*
*   Int2 AsnLexScan(aip, name)
*     scans until name ::= to start reading asn.1 past garbage
*
*****************************************************************************/
static Int2 AsnLexScan PROTO((AsnIoPtr aip, CharPtr name));

/*****************************************************************************
*
*   AsnTypePtr AsnTxtReadId(aip, amp, atp)
*   	reads identifier for next value
*
*****************************************************************************/
NLM_EXTERN AsnTypePtr LIBCALL  AsnTxtReadId (AsnIoPtr aip, AsnModulePtr amp, AsnTypePtr atp)
{
	Int2 token, isa;
	AsnTypePtr atp2, atp3, atp4;
    PstackPtr currpsp, prevpsp;
	Boolean is_ref;

	if (! aip->type_indent)    /* just starting reading */
	{
		if (atp == NULL)       /* starting an unknown item */
		{
			token = AsnLexWord(aip);
			if (token == EOF_TOKEN)
				return NULL;

			if (token != REF)
			{
				AsnIoErrorMsg(aip, 26, aip->linenumber);
				return NULL;
			}
	
			atp = AsnLexFindType(aip, amp);
			if (atp == NULL)
			{
				AsnIoErrorMsg(aip, 27, aip->linenumber);
				return NULL;
			}

			token = AsnLexWord(aip);
			if (token != ISDEF)
			{
				AsnIoErrorMsg(aip, 28, aip->linenumber);
				return NULL;
			}
		}
		else		   /* reading a known, possibly internal value */
		{
			if ((atp->name != NULL) &&    /* do we need to read a type ref? */
				(IS_UPPER(*atp->name)))
				is_ref = TRUE;
			else
				is_ref = FALSE;

			if ((is_ref) && (aip->scan_for_start))
			{
				token = AsnLexScan(aip, atp->name);  /* scan to start */
				if (token == EOF_TOKEN)
					return NULL;
				if (token != ISDEF)
				{
					AsnIoErrorMsg(aip, 28, aip->linenumber);
					return NULL;
				}
			}
			else if ((is_ref) || (aip->bytes))   /* reading an internal value */
			{
				token = AsnLexWord(aip);
				if (token == EOF_TOKEN)
					return NULL;

				if (! StrMatch(atp->name, aip->word, aip->wordlen))
				{
					AsnIoErrorMsg(aip, 29,AsnErrGetTypeName(atp->name), aip->linenumber);
					return NULL;
				}
				if (token == REF)        /* read of a non-internal value */
				{
					token = AsnLexWord(aip);
					if (token != ISDEF)
					{
						AsnIoErrorMsg(aip, 28, aip->linenumber);
						return NULL;
					}
				}
			}
			
		}

		aip->typestack[0].type = atp;         /* load first type */
		aip->typestack[0].resolved = FALSE;
	
		return atp;
	}

    currpsp = & aip->typestack[aip->type_indent];
    prevpsp = currpsp - 1;

	if (currpsp->type != NULL)   /* reading in a struct */
	{
		token = AsnLexWord(aip);
		switch (token)
		{
			case COMMA:
				break;
			case END_STRUCT:
				return prevpsp->type;

			default:
				AsnIoErrorMsg(aip, 30, aip->linenumber);
				return NULL;
		}
	}

	    /* check for SEQOF and SETOF */
	
	atp = prevpsp->type;
	atp2 = AsnFindBaseType(atp);
	isa = atp2->type->isa;
	if ((isa == SEQOF_TYPE) || (isa == SETOF_TYPE))
	{
		token = AsnLexWord(aip);
        if (token == END_STRUCT)    /* empty set */
            return prevpsp->type;
        aip->tagsaved = TRUE;       /* not empty, note not read */
		atp = (AsnTypePtr) atp2->branch;   /* set of what type? */
		currpsp->type = atp;
		currpsp->resolved = FALSE;
		return atp;   /* no type identifier */
	}
	

	token = AsnLexWord(aip);

	if (token == EOF_TOKEN)
	{
		AsnIoErrorMsg(aip, 17 );
		return NULL;
	}

    if ((token == END_STRUCT) && ((isa == SEQ_TYPE) || (isa == SET_TYPE)))
        return prevpsp->type;   /* empty SET/SEQ */

	if (token != IDENT)
	{
		AsnIoErrorMsg(aip, 31, aip->linenumber);
		return NULL;
	}

	atp = AsnLexFindElement(aip, (AsnTypePtr) atp2->branch);
	if (atp == NULL)
	{
		AsnIoErrorMsg(aip, 32, AsnErrGetTypeName(prevpsp->type->name), aip->linenumber);
		return NULL;
	}

	if (atp2->type->isa == SEQ_TYPE)      /* check sequence order */
	{
		atp3 = currpsp->type;
		if (atp3 != NULL)
		{
			atp4 = (AsnTypePtr) atp2->branch;
			atp3 = atp3->next;
			while (atp4 != atp3)
			{
				if (atp == atp4)
				{
					AsnIoErrorMsg(aip, 33,AsnErrGetTypeName(atp->name), AsnErrGetTypeName(atp2->name), aip->linenumber);
					return NULL;
				}
				atp4 = atp4->next;
			}
		}
		else
			atp3 = (AsnTypePtr) atp2->branch;

		while ((atp3 != NULL) && (atp3 != atp))
		{
			if (! (atp3->optional || atp3->hasdefault))
			{
				AsnIoErrorMsg(aip, 34,AsnErrGetTypeName(atp3->name), AsnErrGetTypeName(atp2->name), aip->linenumber);
				return NULL;
			}
			atp3 = atp3->next;
		}
	}

	currpsp->type = atp;
	currpsp->resolved = FALSE;    /* mark first use */
	return atp;
}

/*****************************************************************************
*
*   Int2 AsnTxtReadVal(aip, atp, valueptr)
*   	read the value pointed at by atp
*   	returns START_STRUCT
*               END_STRUCT
*   			or 1 if ok
*   			0 if an error
*   	if (valueptr == NULL)
*   		just checks that value is of proper type
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL  AsnTxtReadVal (AsnIoPtr aip, AsnTypePtr atp, DataValPtr valueptr)
{
	Int2 isa, token, retval;
	Boolean terminalvalue;      /* set if read a terminal value */
	AsnTypePtr atp2;
	Boolean read_value;
	DataVal fake_value;
	AsnTypePtr base_type, curr_type;
    PstackPtr currpsp;

	retval = 1;  /* assume success */
    currpsp = & aip->typestack[aip->type_indent];
	curr_type = currpsp->type;

	if (atp == NULL)
		return 0;

	base_type = AsnFindBaseType(atp);     
    if (base_type == NULL)     /* not found */
    {
        base_type = atp;   
        while (base_type->type != NULL)
            base_type = base_type->type;
        if (base_type->imported)
		{
            AsnIoErrorMsg(aip,35, AsnErrGetTypeName(atp->name), base_type->name, aip->linenumber);
			return 0;
		}
       else
		{
            AsnIoErrorMsg(aip,36, AsnErrGetTypeName(atp->name), base_type->name, aip->linenumber);
			return 0;
		}
    }
	isa = base_type->type->isa;

	if (valueptr == NULL)       /* just check the value */
	{
		read_value = FALSE;
		valueptr = &fake_value;
	}
	else
		read_value = TRUE;

	terminalvalue = TRUE;    /* most values are terminal values */

	if (ISA_STRINGTYPE(isa))
		isa = GENERALSTRING_TYPE;

	switch (isa)
	{
		case SEQ_TYPE:
		case SET_TYPE:
		case SETOF_TYPE:
		case SEQOF_TYPE:
			terminalvalue = FALSE;  /* handled by AsnTypeSetIndent() */
			if (aip->token != END_STRUCT)   /* should be open brace */
			{
				token = AsnLexWord(aip);    /* read open brace */
				if (token != START_STRUCT)
				{
					AsnIoErrorMsg(aip, 37, aip->linenumber);
					return 0;
				}
				AsnTypeSetIndent(TRUE, aip, atp);  
				retval = START_STRUCT;
				valueptr->intvalue = START_STRUCT;
				terminalvalue = FALSE;
			}
			else
			{			   /* close brace already read in AsnTxtReadId */
				switch (isa)
				{
					case SEQOF_TYPE:
					case SETOF_TYPE:
						break;
					case SEQ_TYPE:       /* check that no more waiting */
        				if (curr_type != NULL)   /* check next one */
		        			atp2 = curr_type->next;
				        else                     /* empty sequence written */
        					atp2 = (AsnTypePtr)base_type->branch;
						while (atp2 != NULL)
						{
							if (! (atp2->optional || atp2->hasdefault))
							{

                                                                AsnIoErrorMsg(aip, 34, AsnErrGetTypeName(atp2->name), base_type->name, aip->linenumber);
								return 0;
							}
							atp2 = atp2->next;
						}
						break;
					default:
						break;
				}
				AsnTypeSetIndent(FALSE, aip, atp);  
				retval = END_STRUCT;
				valueptr->intvalue = END_STRUCT;
			}
			break;
		case CHOICE_TYPE:
			AsnTypeSetIndent(TRUE, aip, atp);  /* nest down to type */
			terminalvalue = FALSE;
			break;
		case BOOLEAN_TYPE:
			valueptr->boolvalue = AsnLexReadBoolean(aip, atp);
			break;
		case INTEGER_TYPE:
		case ENUM_TYPE:
			valueptr->intvalue = AsnLexReadInteger(aip, atp);
			break;
		case BIGINT_TYPE:
			valueptr->bigintvalue = AsnLexReadBigInt(aip, atp);
			break;
		case REAL_TYPE:
			valueptr->realvalue = AsnLexReadReal(aip, atp);
			break;
		case GENERALSTRING_TYPE:
			if (read_value)
			{
				valueptr->ptrvalue = AsnLexReadString(aip, atp);
				if (valueptr->ptrvalue == NULL)
					return 0;
			}
			else
			{
				if (! AsnLexSkipString(aip, atp))
					return 0;
			}
			break;
		case NULL_TYPE:
			AsnLexReadNull(aip, atp);
			break;
		case OCTETS_TYPE:
			if (read_value)
			{
				valueptr->ptrvalue = AsnLexReadOctets(aip, atp);
				if (valueptr->ptrvalue == NULL) return 0;
			}
			else
				AsnLexSkipOctets(aip, atp);
			break;
		case STRSTORE_TYPE:
			if (read_value)
			{
				valueptr->ptrvalue = (ByteStorePtr) AsnLexReadString(aip, atp);
				if (valueptr->ptrvalue == NULL)
					return 0;
			}
			else
			{
				if (! AsnLexSkipString(aip, atp))
					return 0;
			}
			break;
		default:
			AsnIoErrorMsg(aip, 38,  AsnErrGetTypeName(atp->name),aip->linenumber);
			return 0;
	}

	if (terminalvalue)       /* pop out of any CHOICE nests */
	{
		while ((aip->type_indent) &&
			(AsnFindBaseIsa(aip->typestack[aip->type_indent - 1].type) == CHOICE_TYPE))
			AsnTypeSetIndent(FALSE, aip, atp);
	}

	return retval;
}

/*****************************************************************************
*
*   AsnTypePtr AsnLexFindType(aip, amp)
*   	returns pointer to type definition in amp
*   	or NULL if not found
*
*****************************************************************************/
NLM_EXTERN AsnTypePtr AsnLexFindType (AsnIoPtr aip, AsnModulePtr amp)
{
	AsnTypePtr atp;
	
	while (amp != NULL)
	{
		atp = AsnLexFindElement(aip, amp->types);
		if (atp != NULL)
			return atp;

		amp = amp->next;
	}
	return NULL;
}				

/*****************************************************************************
*
*   AsnTypePtr AsnLexFindElement(aip, atp)
*   	finds an element in a list of elements
*   	(elements of SEQ, SET, CHOICE)
*
*****************************************************************************/
NLM_EXTERN AsnTypePtr AsnLexFindElement (AsnIoPtr aip, AsnTypePtr atp)
{
	while (atp != NULL)
	{
		if (StrMatch(atp->name, aip->word, aip->wordlen))   /* it matches */
			return atp;
		else
			atp = atp->next;
	}
	return NULL;
}


/*****************************************************************************
*
*   Int4 AsnLexReadInteger(aip, atp)
*   	expects an INTEGER or ENUMERATED next
*   	assumes none of it has already been read
*   	does not advance to next token
*   	atp points to the definition of this integer for named values
*
*****************************************************************************/
NLM_EXTERN Int4 AsnLexReadInteger (AsnIoPtr aip, AsnTypePtr atp)
{
	Int2 token;
	AsnValxNodePtr avnp;
	AsnTypePtr atp2;

	token = AsnLexWord(aip);      /* read the integer */

	if (token == NUMBER)    /* just a number */
	{
		return AsnLexInteger(aip);
	}

	if (token != IDENT)    /* not number or named value */
	{
		AsnIoErrorMsg(aip, 39, AsnErrGetTypeName(atp->name), aip->linenumber);
		return 0;
	}

			/******************** read a named integer value *********/
	atp2 = AsnFindBaseType(atp);
	if (atp2->branch != NULL)       /* named values */
	{
		avnp = (AsnValxNodePtr) atp2->branch;
		while (avnp != NULL)
		{
			if (StrMatch(avnp->name, aip->word, aip->wordlen))
				return avnp->intvalue;
			avnp = avnp->next;
		}
	}

	if (atp2->type->isa == ENUM_TYPE)   /* enumerated MUST match named value */
	{
		AsnIoErrorMsg(aip, 40, AsnErrGetTypeName(atp->name), aip->linenumber);
		return 0;
	}

		   /******************* could it be a previously defined value? ***/

	AsnIoErrorMsg(aip, 41, AsnErrGetTypeName(atp->name), aip->linenumber);

	return 0;
}

/*****************************************************************************
*
*   Int8 AsnLexReadBigInt(aip, atp)
*   	expects an INTEGER next
*   	assumes none of it has already been read
*   	does not advance to next token
*   	atp points to the definition of this integer for named values
*
*****************************************************************************/
NLM_EXTERN Int8 AsnLexReadBigInt (AsnIoPtr aip, AsnTypePtr atp)
{
	Int2 token;

	token = AsnLexWord(aip);      /* read the integer */

	if (token == NUMBER)    /* just a number */
	{
		return AsnLexBigInt(aip);
	}

	if (token != IDENT)    /* not number or named value */
	{
		AsnIoErrorMsg(aip, 39, AsnErrGetTypeName(atp->name), aip->linenumber);
		return 0;
	}

			/******************** read a named integer value *********/
			/**** not supported for bigint *****/

		   /******************* could it be a previously defined value? ***/

	AsnIoErrorMsg(aip, 41, AsnErrGetTypeName(atp->name), aip->linenumber);

	return (Int8)0;
}

/*****************************************************************************
*
*   Boolean AsnLexReadBoolean(aip, atp)
*   	expects a BOOLEAN next
*   	assumes none of it has already been read
*   	does not advance to next token
*   	atp points to the definition of this BOOLEN
*
*****************************************************************************/
NLM_EXTERN Boolean AsnLexReadBoolean (AsnIoPtr aip, AsnTypePtr atp)
{
	AsnLexWord(aip);      /* read the boolean */

	if (StrMatch("TRUE", aip->word, aip->wordlen))
		return TRUE;
	else if (StrMatch("FALSE", aip->word, aip->wordlen))
		return FALSE;

	AsnIoErrorMsg(aip, 42, AsnErrGetTypeName(atp->name), aip->linenumber);

	return FALSE;      /* just for lint */
}

/*****************************************************************************
*
*   Int4 AsnLexReadString(aip, atp)
*   	expects any String type next
*   	assumes none of it has already been read
*   	does not advance to next token
*   	atp points to the definition of this String
*   	if atp->type->isa == a string type
*   		packs String to single value, if segmented on input
*       otherwise returns the stringstoreptr itself
*
*****************************************************************************/
NLM_EXTERN Pointer AsnLexReadString (AsnIoPtr aip, AsnTypePtr atp)
{
	Int2 token;
	ByteStorePtr ssp;
	CharPtr result=NULL;
	Int4 bytes;

	token = AsnLexWord(aip);      /* read the start */
	if (token != START_STRING)
	{
		AsnIoErrorMsg(aip, 43, AsnErrGetTypeName(atp->name), aip->linenumber);
		return NULL;
	}

	ssp = BSNew(0);
	if (ssp == NULL) return result;
	token = AsnLexWord(aip);   /* read the string(s) */
	while (token == IN_STRING)
	{
		bytes = BSWrite(ssp, aip->word, (Int4)aip->wordlen);
		if (bytes != (Int4)(aip->wordlen))
		{
			BSFree(ssp);
			return result;
		}
		token = AsnLexWord(aip);
	}

	if (token == ERROR_TOKEN) return NULL;

	if (token != END_STRING)
	{
		AsnIoErrorMsg(aip, 44, AsnErrGetTypeName(atp->name), aip->linenumber);
		return NULL;
	}
	if (AsnFindBaseIsa(atp) == STRSTORE_TYPE)    /* string store */
		return ssp;

	result = (CharPtr) BSMerge(ssp, NULL);
	BSFree(ssp);

	return result;
}

/*****************************************************************************
*
*   void AsnLexSkipString(aip, atp)
*   	expects any String type next
*   	assumes none of it has already been read
*   	does not advance to next token
*   	atp points to the definition of this String
*   	just reads past end of string
*
*****************************************************************************/
NLM_EXTERN Boolean AsnLexSkipString (AsnIoPtr aip, AsnTypePtr atp)
{
	Int2 token;

	token = AsnLexWord(aip);      /* read the start */
	if (token != START_STRING)
	{
		AsnIoErrorMsg(aip, 43, AsnErrGetTypeName(atp->name), aip->linenumber);
		return FALSE;
	}

	token = AsnLexWord(aip);   /* read the string(s) */
	while (token == IN_STRING)
		token = AsnLexWord(aip);

	if (token == ERROR_TOKEN) return FALSE;

	if (token != END_STRING)
		AsnIoErrorMsg(aip, 44, AsnErrGetTypeName(atp->name), aip->linenumber);

	return TRUE;
}

/*****************************************************************************
*
*   ByteStorePtr AsnLexReadOctets(aip, atp)
*   	expects Octets type next
*   	assumes none of it has already been read
*   	does not advance to next token
*   	atp points to the definition of this OCTET STRING
*
*****************************************************************************/
NLM_EXTERN ByteStorePtr AsnLexReadOctets (AsnIoPtr aip, AsnTypePtr atp)
{
	Int2 token, len;
	ByteStorePtr ssp = NULL;
	Byte tbuf[256]; /* was 101 - changed to handle occasional hand-edited ASN.1? */
	Int4 bytes, left, added;

	token = AsnLexWord(aip);      /* read the start */
	if (token != START_BITHEX)
	{
		AsnIoErrorMsg(aip, 45, AsnErrGetTypeName(atp->name), aip->linenumber);
		return NULL;
	}
	ssp = BSNew(0);
	if (ssp == NULL) return ssp;
	token = AsnLexWord(aip);   /* read the octet(s) */
	left = 0;
	while (token == IN_BITHEX)
	{
		len = (Int2)(aip->wordlen + left);
		MemCopy((tbuf + left), aip->word, (len - left));
		tbuf[len] = '\0';
		added = AsnTypeStringToHex(tbuf, len, tbuf, &left);
		if (added < 0)
		{
			AsnIoErrorMsg(aip, 46, AsnErrGetTypeName(atp->name), aip->linenumber);
			return NULL;
		}
		if (added)
		{
			bytes = BSWrite(ssp, tbuf, added);
			if (bytes != added)
			{
				ssp = BSFree(ssp);
				return ssp;
			}
		}
		if (left)   /* left a char */
		{
			MemCopy(tbuf, ((aip->word)+(aip->wordlen - left)),left);
		}

		token = AsnLexWord(aip);
	}
	
	if (token != OCTETS)
	{
		AsnIoErrorMsg(aip, 47, AsnErrGetTypeName(atp->name), aip->linenumber);
		return NULL;
	}

	return ssp;
}

/*****************************************************************************
*
*   void AsnLexSkipOctets(aip, atp)
*   	expects Octets type next
*   	assumes none of it has already been read
*   	does not advance to next token
*   	atp points to the definition of this OCTET STRING
*
*****************************************************************************/
NLM_EXTERN void AsnLexSkipOctets (AsnIoPtr aip, AsnTypePtr atp)
{
	Int2 token, left, len;
    Int4 line;

	token = AsnLexWord(aip);      /* read the start */
	if (token != START_BITHEX)
	{
		AsnIoErrorMsg(aip, 45, AsnErrGetTypeName(atp->name), aip->linenumber);
		return;
	}

    line = aip->linenumber;
	token = AsnLexWord(aip);   /* read the octet(s) */
    left = 0;
	while (token == IN_BITHEX)
	{
        len = aip->wordlen + left;
        left = len % 2;
		token = AsnLexWord(aip);
	}

    if (left)
	{
        AsnIoErrorMsg(aip, 48, AsnErrGetTypeName(atp->name), line);
		return;
	}
	
	if (token != OCTETS)
		AsnIoErrorMsg(aip, 47, AsnErrGetTypeName(atp->name), aip->linenumber);

	return;
}

/*****************************************************************************
*
*   FloatHi AsnLexReadReal(aip, atp)
*   	expects a REAL next
*   	assumes none of it has already been read
*   	does not advance to next token
*   	atp points to the definition of this REAL
*
*****************************************************************************/
NLM_EXTERN FloatHi AsnLexReadReal (AsnIoPtr aip, AsnTypePtr atp)
{
	Int2 token;
	double mantissa, result;
	int	base, exponent;
	char buf[15];

	result = 0.0;
	token = AsnLexWord(aip);      /* read the { */
	if (token != START_STRUCT)
	{
		AsnIoErrorMsg(aip, 49, AsnErrGetTypeName(atp->name), aip->linenumber);
		return (FloatHi)result;
	}

	token = AsnLexWord(aip);      /* read mantissa */
	if (token != NUMBER)
	{
		AsnIoErrorMsg(aip, 50, AsnErrGetTypeName(atp->name), aip->linenumber);
		return (FloatHi)result;
	}
	MemCopy(buf, aip->word, (size_t)aip->wordlen);
	buf[aip->wordlen] = '\0';
	mantissa = atof(buf);

	token = AsnLexWord(aip);      /* read comma */
	if (token != COMMA)
	{
		AsnIoErrorMsg(aip, 51, AsnErrGetTypeName(atp->name), aip->linenumber);
		return (FloatHi)result;
	}

	token = AsnLexWord(aip);      /* read base */
	if (token != NUMBER)
	{
		AsnIoErrorMsg(aip, 52, AsnErrGetTypeName(atp->name), aip->linenumber);
		return (FloatHi)result;
	}
	MemCopy(buf, aip->word, (size_t)aip->wordlen);
	buf[aip->wordlen] = '\0';
	base = atoi(buf);
	if ((base != 2) && (base != 10))
	{
		AsnIoErrorMsg(aip, 53, AsnErrGetTypeName(atp->name), aip->linenumber);
		return (FloatHi)result;
	}

	token = AsnLexWord(aip);
	if (token != COMMA)
	{
		AsnIoErrorMsg(aip, 54, AsnErrGetTypeName(atp->name), aip->linenumber);
		return (FloatHi)result;
	}

	token = AsnLexWord(aip);
	if (token != NUMBER)
	{
		AsnIoErrorMsg(aip, 55, AsnErrGetTypeName(atp->name), aip->linenumber);
		return (FloatHi)result;
	}
	MemCopy(buf, aip->word, (size_t)aip->wordlen);
	buf[aip->wordlen] = '\0';
	exponent = atoi(buf);

	token = AsnLexWord(aip);
	if (token != END_STRUCT)
	{
		AsnIoErrorMsg(aip, 56, AsnErrGetTypeName(atp->name), aip->linenumber);
		return (FloatHi)result;
	}
    
    if (base == 10)      /* range checking only on base 10, for doubles */
    {
    	if (exponent > DBL_MAX_10_EXP)   /* exponent too big */
    		return	(FloatHi)DBL_MAX;
    	else if (exponent < DBL_MIN_10_EXP)  /* exponent too small */
    		return (FloatHi)result;
    }
    	
	result = mantissa * Nlm_Powi((double)base, exponent);

	return (FloatHi)result;
}

/*****************************************************************************
*
*   void AsnLexReadNull(aip, atp)
*
*****************************************************************************/
NLM_EXTERN void AsnLexReadNull (AsnIoPtr aip, AsnTypePtr atp)
{
	AsnLexWord(aip);
	if (! StrMatch("NULL", aip->word, aip->wordlen))
	{
		AsnIoErrorMsg(aip, 57, AsnErrGetTypeName(atp->name), aip->linenumber);
		return;
	}
	return;
}

/*****************************************************************************
*
*   CharPtr AsnLexSaveWord(aip)
*   	create a new null terminated string from
*       a non null terminated string (word) of length (len)
*
*****************************************************************************/
NLM_EXTERN CharPtr AsnLexSaveWord (AsnIoPtr aip)
{
	CharPtr ptr;

	ptr = (CharPtr) MemNew((size_t)(aip->wordlen + 1));
	MemCopy(ptr, aip->word, (size_t)aip->wordlen);
	return ptr;
}

/*****************************************************************************
*
*   void AsnLexSkipStruct(aip)
*   	skips current structure and all substructures
*       Does NOT read following element
*
*****************************************************************************/
NLM_EXTERN void AsnLexSkipStruct (AsnIoPtr aip)
{
	int type_indent = 1;
	Int2 token;

	while (type_indent)
	{
		token = AsnLexWord(aip);
		switch (token)
		{
			case START_STRUCT:       /* another substructure */
				type_indent++;
				break;
			case END_STRUCT: 		/* a close brace */
				type_indent--;
				break;
			case EOF_TOKEN:
				AsnIoErrorMsg(aip, 17);
				return;
			default:
				break;
		}
	}

	return;
}

/*****************************************************************************
*
*   Int4 AsnLexInteger(aip)
*
*****************************************************************************/
NLM_EXTERN Int4 AsnLexInteger (AsnIoPtr aip)
{
	char tbuf[11];
	Int4 value;

	MemCopy(tbuf, aip->word, (size_t)aip->wordlen);
	tbuf[aip->wordlen] = '\0';
	value = atol(tbuf);
	return value;
}

/*****************************************************************************
*
*   Int8 AsnLexBigInt(aip)
*
*****************************************************************************/
NLM_EXTERN Int8 AsnLexBigInt (AsnIoPtr aip)
{
	char tbuf[40];
	Int8 value;
	const char * result;

	MemCopy(tbuf, aip->word, (size_t)aip->wordlen);
	tbuf[aip->wordlen] = '\0';
	value = Nlm_StringToInt8 (tbuf, &result);
	return value;
}

/*****************************************************************************
*
*   Int2 AsnLexWord(aip)
*   	reads words, punctuation
*   	returns tokens defined in asntypes.h
*   	does not return ASN reserved words - use AsnLexWord() for that
*
*****************************************************************************/

NLM_EXTERN Int2 AsnLexWord (AsnIoPtr aip)
{
	register CharPtr pos;
	Int1 state;
	Int2 token;
	register int linepos, len;
	int done;

    if (aip->tagsaved)     /* had to read ahead */
    {
        aip->tagsaved = FALSE;
        return aip->token;
    }

    if (! aip->bytes)        /* no data loaded */
		AsnIoGets(aip);
		
	linepos = aip->linepos;
	pos = aip->linebuf + linepos;
	state = aip->state;
	len = 0;

	while (*pos == '\n' || *pos == '\r')    /* skip empty lines */
	{
		pos = AsnIoGets(aip);   /* get a line */

		if (pos == NULL)
			return EOF_TOKEN;
	}
	
	if (state == IN_STRING_STATE)
	{
		aip->word = pos;
		if ((* pos == '\"') && (*(pos + 1) != '\"'))    /* end of string */
		{
			token = END_STRING;
			pos++;
			state = 0;        /* reset state */
		}
		else
		{
			token = IN_STRING;
			while ((* pos != '\"') && (* pos != '\n') && (* pos != '\r'))
			{
				if ((aip->fix_non_print != 2) && ((*pos < ' ') || (*pos > '~')))
				{
					done = (int)(*pos);
					*pos = '\0';
					if ((aip->fix_non_print == 0) || (aip->fix_non_print == 3))
					{
						AsnIoErrorMsg(aip, 106, done, aip->word);
					}
					done = 0;
					*pos = '#';   /* replace with # */
				}
				pos++; len++;
			}

			if ((*pos != '\n') && (*pos != '\r') && (* (pos + 1) == '\"')) /* internal quote */
			{
				len++;        /* include in previous string */
				pos += 2;     /* point to rest of string */
			}
		}
	}
	else if (state == IN_BITHEX_STATE)
	{
		aip->word = pos;
		if (*pos == '\'')  			  /* end of binhex */
		{
			state = 0;              /* set to normal */
			pos++;                       /* move past quote */
			while (IS_WHITESP(*pos))
			{
				if (*pos == '\n' || *pos == '\r')    /* skip empty lines */
				{
					pos = AsnIoGets(aip);   /* get a line */
					
					if (pos == NULL)
					return EOF_TOKEN;
				}
				else
					pos++;
			}
			if (* pos == 'H')
				token = OCTETS;
			else if (* pos == 'B')
				token = ASNBITS;
			else
			{
				AsnIoErrorMsg(aip, 58, aip->linenumber);
				token = ERROR_TOKEN;
			}
			pos++;         /* move past H or B */
		}
		else
		{
			token = IN_BITHEX;
			while ((* pos != '\'') && (* pos != '\n') && (* pos != '\r'))
			{
				pos++; len++;
			}
		}   
	}
	else              /* normal scanning */
	{
		while ((* pos <= ' ') || ((*pos == '-') && (*(pos+1) == '-')))     /* skip leading white space */
		{
			if (*pos == '\n' || *pos == '\r')
			{
				pos = AsnIoGets(aip);

				if (pos == NULL)
					return EOF_TOKEN;
			}
			else if ((*pos == '-') && (*(pos+1) == '-'))   /* skip comments */
			{
				pos += 2;
				done = 0;
				while (! done)
				{
					if ((*pos == '-') && (*(pos +1) == '-'))
					{
						pos += 2;
						done = 1;
					}
					else if (*pos == '\n' || *pos == '\r')
						done = 1;
					else
						pos++;
				}
			}
			else
				pos++;
			
		}

		aip->word = pos;
		if (* pos == '\"')
		{
			token = START_STRING;
			state = IN_STRING_STATE;
		}
		else if (* pos == '\'')
		{
			token = START_BITHEX;
			state = IN_BITHEX_STATE;
		}
		else if (* pos == ',')
			token = COMMA;
		else if (* pos == '{')
			token = START_STRUCT;
		else if (* pos == '}')
			token = END_STRUCT;
		else if (* pos == '[')
			token = START_TAG;
		else if (* pos == ']')
			token = END_TAG;
		else if (* pos == '(')
			token = OPEN_PAREN;
		else if (* pos == ')')
			token = CLOSE_PAREN;
		else if (* pos == ';')
			token = SEMI_COLON;
		else if (* pos == ':')
		{
			if ((*(pos + 1) == ':') && (*(pos + 2) == '='))
			{
				token = ISDEF;
				pos += 2;
				len = 3;
			}
			else
			{
				AsnIoErrorMsg(aip, 59, *pos, aip->linenumber);
				token = ERROR_TOKEN;
			}
		}
		else if (IS_UPPER(*pos))  /* a reference or keyword */
		{
			token = REF;
			while ((IS_ALPHANUM(*pos)) || (*pos == '-'))
			{
				pos++; len++;
			}
			pos--;    /* move back for increment at end */
			len--;
		}
		else if (IS_LOWER(*pos))  /* an identifier or valuereference */
		{
			token = IDENT;
			while ((IS_ALPHANUM(*pos)) || (*pos == '-'))
			{
				pos++; len++;
			}
			pos--;		  /* move back for increment at end */
			len--;
		}
		else if ((IS_DIGIT(*pos)) || ((*pos == '-') && (IS_DIGIT(*(pos+1)))))
		{
			token = NUMBER;
			if (*pos == '-')
			{
				pos++; len++;
			}

			while (IS_DIGIT(*pos))
			{
				pos++; len++;
			}
			pos--;    /* move back for increment at end */
			len--;
		}
		else
		{
			AsnIoErrorMsg(aip, 59, *pos, aip->linenumber);
			token = ERROR_TOKEN;
		}
		len++; pos++;     /* move over last symbol */
	}

	aip->linepos = pos - aip->linebuf;
/******************** check on MSWIN
	linepos = 0;
	while (pos != linebuf)
	{
		linepos++;
		pos++;
	}
	aip->linepos = linepos;
**********************/
	aip->state = state;
	aip->wordlen = len;
	aip->token = token;
	return token;
}

/*****************************************************************************
*
*   Boolean StrMatch(a, b, len)
*   	returns TRUE if a matches b for EXACTLY len
*       and a has a '\0' after that
*
*****************************************************************************/
NLM_EXTERN Boolean StrMatch (CharPtr a, CharPtr b, Int2 len)
{
	if ((a == NULL) || (b == NULL))
		return FALSE;
		
	while (len)
	{
		if (*a != *b)
			return FALSE;
		a++; b++; len--;
	}
	if (*a != '\0')
		return FALSE;
	return TRUE;
}


/*****************************************************************************
*
*   Int2 AsnLexScan(aip, name)
*     scans until name ::= to start reading asn.1 past garbage
*
*****************************************************************************/
static Int2 AsnLexScan (AsnIoPtr aip, CharPtr name)
{
	register CharPtr pos;
	Int2 token = 0;
	register int linepos, len;
	int done;
	Boolean started = FALSE, matched_ref = FALSE;

	if (aip->type_indent)   /* only for start */
		return ERROR_TOKEN;

   if (! aip->bytes)        /* no data loaded */
		AsnIoGets(aip);
		
	linepos = aip->linepos;
	pos = aip->linebuf + linepos;
	len = 0;

	while (*pos == '\n' || *pos == '\r')    /* skip empty lines */
	{
		pos = AsnIoGets(aip);   /* get a line */

		if (pos == NULL)
			return EOF_TOKEN;
	}

	while (! started)
	{
		while ((* pos <= ' ') || ((*pos == '-') && (*(pos+1) == '-')))     /* skip leading white space */
		{
			if (*pos == '\n' || *pos == '\r')
			{
				pos = AsnIoGets(aip);

				if (pos == NULL)
					return EOF_TOKEN;
			}
			else if ((*pos == '-') && (*(pos+1) == '-'))   /* skip comments */
			{
				pos += 2;
				done = 0;
				while (! done)
				{
					if ((*pos == '-') && (*(pos +1) == '-'))
					{
						pos += 2;
						done = 1;
					}
					else if (*pos == '\n' || *pos == '\r')
						done = 1;
					else
						pos++;
				}
			}
			else
				pos++;
			
		}

		aip->word = pos;
		len = 0;
		if (* pos == ':')
		{
			if ((*(pos + 1) == ':') && (*(pos + 2) == '='))
			{
				token = ISDEF;
				pos += 2;
				len = 3;
				if (matched_ref)
					started = TRUE;
				else
				{
					AsnIoErrorMsg(aip, 59, *pos, aip->linenumber);
					return ERROR_TOKEN;
				}
			}
			else
				matched_ref = FALSE;
		}
		else if (IS_UPPER(*pos))  /* a reference or keyword */
		{
			token = REF;
			while ((IS_ALPHANUM(*pos)) || (*pos == '-'))
			{
				pos++; len++;
			}
			pos--;    /* move back for increment at end */
			aip->wordlen = len;
         if (StrMatch(name, aip->word, (Int2)len))
				matched_ref = TRUE;
			else
				matched_ref = FALSE;
		}
		else
			matched_ref = FALSE;
		pos++;     /* move over last symbol */
	}

	                  /* found it , do normal return after ::= */

	aip->linepos = pos - aip->linebuf;
	aip->wordlen = len;
	aip->token = token;
	return token;
}


