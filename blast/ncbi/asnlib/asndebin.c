/*  asndebin.c
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
* File Name:  asndebin.c
*
* Author:  James Ostell
*
* Version Creation Date: 3/4/91
*
* $Revision: 6.7 $
*
* File Description:
*   Special binary form (BER) decoder for ASN.1
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 2/11/91  Ostell      AsnBinReadVal - added check for unresolved base types
* 3/4/91   Kans        Stricter typecasting for GNU C and C++
* 3/4/91   Kans        AsnDeBinReadBoolean returns Boolean
* 04-20-93 Schuler     LIBCALL calling convention
* 08-01-93 Gish        AsnDeBinReadString calls MemGet instead of MemNew
*
* $Log: asndebin.c,v $
* Revision 6.7  2004/04/01 13:43:05  lavr
* Spell "occurred", "occurrence", and "occurring"
*
* Revision 6.6  2003/12/10 15:41:19  sirotkin
* As per toolkit RT request 15030485 and Jim Ostell, afety checks after ato2=AsnFindBaseIsa(atp) were added.
*
* Revision 6.5  2002/10/09 19:16:31  ivanov
* Fixed buffer overrun in the AsnDeBinReadReal()
*
* Revision 6.4  2000/12/12 15:56:10  ostell
* added support BigInt
*
* Revision 6.3  1999/12/23 14:18:06  beloslyu
* fix the AsnBinReadVal function if the atp is NULL
*
* Revision 6.2  1998/01/22 20:46:24  volodya
* fix endless loop
*
* Revision 6.1  1997/10/29 02:40:21  vakatov
* Type castings to pass through the C++ compiler
*
* Revision 6.0  1997/08/25 18:09:46  madden
* Revision changed to 6.0
*
* Revision 5.1  1996/12/03 21:43:48  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
 * Revision 5.0  1996/05/28  14:00:29  ostell
 * Set to revision 5.0
 *
 * Revision 4.2  1996/02/18  16:45:36  ostell
 * changed fix_non_print behavior and added option 3
 *
 * Revision 4.1  1995/12/21  22:03:20  epstein
 * per Kevin Kendall, IBI; avoid infinite loop caused by uninformative return value in AsnDeBinDecr()
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

#include "asnbuild.h"

#define ASNDEBIN_EOF -2

/*****************************************************************************
*
*   AsnTypePtr AsnBinReadId(aip, atp)
*   	reads identifier for next value
*
*****************************************************************************/
NLM_EXTERN AsnTypePtr LIBCALL  AsnBinReadId (AsnIoPtr aip, AsnTypePtr atp)
{
	Int4 used;

	Int2 isa;
	AsnTypePtr atp2, atp3, atp4;
	Boolean found;
    PstackPtr psp, prevpsp, currpsp;

	used = 0;

	if (! aip->type_indent)    /* just starting or ending reading */
	{
		if (atp == NULL)       /* starting an unknown item */
		{
			AsnIoErrorMsg(aip, 1);
			return NULL;
		}

		used = AsnDeBinScanTag(aip);   /* read the tag */

		if (used == ASNDEBIN_EOF)      /* end of file */
			return NULL;
                /* see RT toolbox ticket 15030485 for the below fix */

		atp2 = AsnFindBaseType(atp);   /* find the base type */
		if (atp2 == NULL)
			return atp2;
		if (atp2->type == NULL)
			return atp2->type;
		isa = atp2->type->isa;


		if (atp2->type->isa != CHOICE_TYPE)
		{
			atp2 = atp2->type;         /* will be primitive tag */
			if ((aip->tagclass != atp2->tagclass) ||
				(aip->tagnumber != atp2->tagnumber))
				found = FALSE;
			else
				found = TRUE;
		}
		else
		{
			atp2 = (AsnTypePtr) atp->branch;      /* look for a match among choices */
			found = FALSE;
			while ((atp2 != NULL) && (! found))
			{
				if ((aip->tagclass == atp2->tagclass) &&
					(aip->tagnumber == atp2->tagnumber))
					found = TRUE;
				else
					atp2 = atp2->next;
			}
			aip->tagsaved = TRUE;    /* will need tag for which CHOICE next round */
		}

		if (! found)
		{
			AsnIoErrorMsg(aip, 2, AsnErrGetTypeName(atp->name));
			return NULL;
		}

        psp = & aip->typestack[0];
        psp->len = aip->length;
        psp->type = atp;
        psp->resolved = FALSE;
        psp->tag_indef = FALSE;

		return atp;
	}

    prevpsp = & aip->typestack[aip->type_indent - 1];   /* previous level */
    currpsp = prevpsp + 1;                              /* current level */

							/** end of definate length struct ****/
	if (prevpsp->len == 0)
	{
		prevpsp->resolved = TRUE;
		return prevpsp->type;
	}

	if (currpsp->type != NULL)  /* reading a struct */
	{
		if (! aip->tagsaved)       /* already read it? */
		{
			used = AsnDeBinScanTag(aip);    /* read the tag */
			if (! AsnDeBinDecr(used, aip))  /* decrement length counter */
				return NULL;
		}

		if ((aip->length == 0) && (aip->tagclass == 0) &&
			(aip->tagnumber == 0))     /* length 0, could be end of contents */
		{
			if (prevpsp->len == INDEFINATE)
			{
                prevpsp->resolved = TRUE;
				aip->tagsaved = FALSE;
				if (prevpsp->tag_indef)      /* indefinate user tag? */
				{
					used = AsnDeBinScanTag(aip);   /* should be 00 for tag */
					if (! AsnDeBinDecr(used, aip))
						return NULL;
					if ((aip->length == 0) && (aip->tagclass == 0) &&
						(aip->tagnumber == 0))
						prevpsp->tag_indef = FALSE;
					else
					{
						AsnIoErrorMsg(aip, 3, AsnErrGetTypeName(prevpsp->type->name));
						return NULL;
					}
				}
				return prevpsp->type;
			}
		}
		else
			aip->tagsaved = TRUE;
	}


	if (aip->type_indent)     /* check for SEQOF and SETOF */
	{
		atp = prevpsp->type;
		atp2 = AsnFindBaseType(atp);
		isa = atp2->type->isa;
		if ((isa == SEQOF_TYPE) || (isa == SETOF_TYPE))
		{
			atp = (AsnTypePtr) atp2->branch;   /* set of what type? */
			currpsp->type = atp;
            currpsp->len = INDEFINATE;
            currpsp->resolved = FALSE;
            currpsp->tag_indef = FALSE;
			atp2 = AsnFindBaseType(atp);
			isa = atp2->type->isa;
			atp2 = atp2->type;        /* get universal type */
			used = AsnDeBinScanTag(aip);   /* read the tag */
			if (! AsnDeBinDecr(used, aip))
				return NULL;
			if ((aip->tagclass != atp2->tagclass) ||
				(aip->tagnumber != atp2->tagnumber))
			{     
				if ((aip->length == 0) && (aip->tagclass == 0) &&
					(aip->tagnumber == 0))     /* length 0, could be end of contents */
				{      /* this handles an empty SEQ OF/SET OF ***/
					if (prevpsp->len == INDEFINATE)
					{
            		    prevpsp->resolved = TRUE;
						aip->tagsaved = FALSE;
						if (prevpsp->tag_indef)      /* indefinate user tag? */
						{
							used = AsnDeBinScanTag(aip);   /* should be 00 for tag */
							if (! AsnDeBinDecr(used, aip))
								return NULL;
							if ((aip->length == 0) && (aip->tagclass == 0) &&
								(aip->tagnumber == 0))
								prevpsp->tag_indef = FALSE;
							else
							{
								AsnIoErrorMsg(aip, 3, AsnErrGetTypeName(prevpsp->type->name));
								return NULL;
							}
						}
						return prevpsp->type;
					}
				}
				else if (isa == CHOICE_TYPE) /* no tag */
				{
					aip->tagsaved = TRUE;
				}
				else
				{
					AsnIoErrorMsg(aip, 4, AsnErrGetTypeName(atp->name));
					return NULL;
				}
			}
			currpsp->len = aip->length;
			return atp;   /* no type identifier */
		}
	}

	used = AsnDeBinScanTag(aip);    /* read the tag */
	if (! AsnDeBinDecr(used, aip))  /* decrement length counter */
		return NULL;

	if ((aip->length == 0) && (aip->tagclass == 0) &&
		(aip->tagnumber == 0))     /* length 0, could be end of contents */
	{
		if (prevpsp->len == INDEFINATE)
		{
            prevpsp->resolved = TRUE;
			aip->tagsaved = FALSE;
			if (prevpsp->tag_indef)      /* indefinate user tag? */
			{
				used = AsnDeBinScanTag(aip);   /* should be 00 for tag */
				if (! AsnDeBinDecr(used, aip))
					return NULL;
				if ((aip->length == 0) && (aip->tagclass == 0) &&
					(aip->tagnumber == 0))
					prevpsp->tag_indef = FALSE;
				else
				{
					AsnIoErrorMsg(aip, 3, AsnErrGetTypeName(prevpsp->type->name));
					return NULL;
				}
			}
			return prevpsp->type;
		}
	}

	atp2 = AsnFindBaseType(prevpsp->type);
	atp = AsnDeBinFindElement(aip, (AsnTypePtr) atp2->branch);
	if (atp == NULL)
	{
		AsnIoErrorMsg(aip, 5, AsnErrGetTypeName(prevpsp->type->name));
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
					AsnIoErrorMsg(aip, 6, AsnErrGetTypeName(atp->name), AsnErrGetTypeName(atp2->name));
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
				AsnIoErrorMsg(aip, 7, AsnErrGetTypeName(atp3->name), AsnErrGetTypeName(atp2->name));
				return NULL;
			}
			atp3 = atp3->next;
		}
	}

	currpsp->type = atp;
	currpsp->len = aip->length;
	currpsp->resolved = FALSE;    /* mark first use */
	if ((atp->tagclass != TAG_UNIVERSAL) && (atp->tagclass != TAG_NONE)
		&& (aip->length == INDEFINATE))
			currpsp->tag_indef = TRUE;
	return atp;
}

/*****************************************************************************
*
*   Boolean AsnDeBinDecr(used, aip)
*   	decrement all levels above level with used bytes
*
*****************************************************************************/
NLM_EXTERN Boolean AsnDeBinDecr (Int4 used, AsnIoPtr aip)
{
	PstackPtr	psp, psp0;

	if (used == ASNDEBIN_EOF) return FALSE; /* avoid infinite loop in higher levels */

	if (used <= 0) /* tag saved or INDEFINATE */
		return TRUE;

	psp = (psp0 = aip->typestack) + aip->type_indent;
	while (psp > psp0) {
		--psp;
		if (psp->len == INDEFINATE) /* if one INDEFINATE, assume all are */
			return TRUE;
		if ((psp->len -= used) < 0) {
			AsnIoErrorMsg(aip, 8, (int)(psp - psp0) /* level */);
			return FALSE;
		}
	}
	return TRUE;
}

/**** old version ***********************
NLM_EXTERN Boolean AsnDeBinDecr (Int4 used, AsnIoPtr aip)
{
	Int2 level;

	if (used == INDEFINATE)
		return TRUE;

	level = aip->type_indent;
	while (level)
	{
		level--;
		if (aip->typestack[level].len != INDEFINATE)
		{
			aip->typestack[level].len -= used;
			if (aip->typestack[level].len < 0)
			{
				AsnIoErrorMsg(aip, 8, level);
				return FALSE;
			}
		}
	}
	return TRUE;
}
*****************************************/
/*****************************************************************************
*
*   Int4 AsnDeBinScanTag(aip)
*   	scan the next tag-length element
*   	return bytes used from input
*
*****************************************************************************/
NLM_EXTERN Int4 AsnDeBinScanTag (AsnIoPtr aip)
{
	Int4 used;
	Uint4 number;
	Int2 bytes, lenbytes;
	BytePtr bp;
	Byte octet, bit8_7, bit5_1;
	Boolean bit6;

	used = 0;             /* no bytes read yet */
	if (aip->tagsaved)    /* return currently saved tag */
	{
		aip->tagsaved = FALSE;
		return used;        /* no additional bytes read */
	}

	bytes = aip->bytes - aip->offset;    /* current location in buffer */
	if (! bytes)
	{
		bytes = AsnIoReadBlock(aip);
		if (bytes == 0)
			return ASNDEBIN_EOF;
	}
	bp = aip->buf + aip->offset;

						/*** read first octet of tag-id ******/
	octet = *bp;
	bp++; bytes--; used++;

	bit5_1 	= octet & (Byte)0x1f;    /* short form tag number */
	bit6 = (Byte)((octet >> 5) & 0x01);     /*  constructed? */
	bit8_7 = (Byte)((octet >> 6) & 0x03);   /* tag class */

	switch (bit8_7)         /* get tag class */
	{
		case 0x0:
			aip->tagclass = TAG_UNIVERSAL;
			break;
		case 0x1:
			aip->tagclass = TAG_APPLICATION;
			break;
		case 0x2:
			aip->tagclass = TAG_CONTEXT;
			break;
		case 0x3:
			aip->tagclass = TAG_PRIVATE;
			break;
	}

	aip->constructed = bit6;     /* constructed? */

							/*** get the tag-id ****/
	if (bit5_1 != 0x1f)    /* short form */
	{
		aip->tagnumber = bit5_1;    /* that's it */
	}
	else                   /* long form */
	{
		number = 0;     /* have to build it up */
		do
		{
			if (! bytes)
			{
				bytes = AsnIoReadBlock(aip);
                if (! bytes)
                    {AsnIoErrorMsg(aip, 17); return 0;}
				bp = aip->buf;
			}
			octet = *bp;
			bp++; bytes--; used++;
			number = (number << 7) + (octet & 0x7f);
		}
		while ((octet & 0x80) != 0);
		aip->tagnumber = (Int2) number;
	}

								/*** scan the length, if present */
	if (! bytes)
	{
		bytes = AsnIoReadBlock(aip);
        if (! bytes)
             {AsnIoErrorMsg(aip, 17); return 0;}
		bp = aip->buf;
	}

	octet = *bp;
	bp++; bytes--; used++;

	if (! (octet & 0x80))             /* short form used */
		aip->length = octet & 0x7f;
	else if ((octet & 0x7f) == 0x00)    /* indefinate form used */
		aip->length = INDEFINATE;
	else                                 /* long form used */
	{
		number = 0;
		for (lenbytes = (Int2)(octet & 0x7f); lenbytes > 0; lenbytes--)
		{
			if (! bytes)
			{
				bytes = AsnIoReadBlock(aip);
                if (! bytes)
                    {AsnIoErrorMsg(aip, 17); return 0;}
				bp = aip->buf;
			}
			octet = *bp;
			bp++; bytes--; used++;
			number = (number << 8) + octet;
		}
		aip->length = number;
	}

	aip->offset = aip->bytes - bytes;
	aip->used = used;
	return used;
}

/*****************************************************************************
*
*   Int2 AsnBinReadVal(aip, atp, valueptr)
*   	read the value pointed at by atp
*         this should be the same as aip->typestack[aip->type_indent].type
*   	returns START_STRUCT
*               END_STRUCT
*   			1 if ok
*   			0 if an error occurred
*   	if (valueptr == NULL)
*   		just checks that value is of proper type
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL  AsnBinReadVal (AsnIoPtr aip, AsnTypePtr atp, DataValPtr valueptr)
{
	Int4 used;
	Int2 isa, retval;
	Boolean terminalvalue;      /* set if read a terminal value */
	AsnTypePtr atp2;
	Boolean read_value;
	DataVal fake_value;
	AsnTypePtr base_type, curr_type;
	int next_type;
    PstackPtr currpsp, prevpsp;

	currpsp = & aip->typestack[aip->type_indent];
    curr_type = currpsp->type;
    if (aip->type_indent)
        prevpsp = currpsp - 1;
    else
        prevpsp = NULL;

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
            AsnIoErrorMsg(aip,9, AsnErrGetTypeName(atp->name), AsnErrGetTypeName(base_type->name));
			return 0;
		}
       else
		{
            AsnIoErrorMsg(aip,10, AsnErrGetTypeName(atp->name), AsnErrGetTypeName(base_type->name));
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

	retval = 1;       /* normal return */
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
			if ((prevpsp == NULL) || (! prevpsp->resolved))   /* open brace */  
			{
				AsnTypeSetIndent(TRUE, aip, atp);
				while (aip->tagclass != TAG_UNIVERSAL)  /* read to struct */
				{
					used = AsnDeBinScanTag(aip);
					if (! AsnDeBinDecr(used, aip))
						return 0;
				}
				retval = START_STRUCT;
				valueptr->intvalue = START_STRUCT;
				terminalvalue = FALSE;
			}
			else
			{			   /* close brace already read in AsnBinReadId */
				switch (isa)
				{
					case SEQOF_TYPE:
					case SETOF_TYPE:
						break;
					case SEQ_TYPE:       /* check that no more waiting */
        				if (curr_type != NULL)   /* check next one */
		        			atp2 = curr_type->next;
				        else                     /* empty sequence written */
        					atp2 = (AsnTypePtr) base_type->branch;
						while (atp2 != NULL)
						{
							if (! (atp2->optional || atp2->hasdefault))
							{
								AsnIoErrorMsg(aip, 7, AsnErrGetTypeName(atp2->name), AsnErrGetTypeName(atp->name));
								return 0;
							}
							atp2 = atp2->next;
						}
						break;
					default:
						break;
				}
				next_type = aip->type_indent - 2;   /* end any choices */
				while ((next_type >= 0) &&
					(AsnFindBaseIsa(aip->typestack[next_type].type) == CHOICE_TYPE))
				{
                    currpsp = & aip->typestack[next_type];
                    atp2 = currpsp->type;
					if ((atp2->tagclass != TAG_NONE) &&
						(currpsp->tag_indef))
					{
						used = AsnDeBinScanTag(aip);   /* should be 00 for tag */
						if (! AsnDeBinDecr(used, aip))
							return 0;
						if ((aip->length == 0) && (aip->tagclass == 0) &&
							(aip->tagnumber == 0))
							currpsp->tag_indef = FALSE;
						else
						{
							AsnIoErrorMsg(aip, 3, AsnErrGetTypeName(atp2->name));
							return 0;
						}
					}
					next_type--;
				}
				AsnTypeSetIndent(FALSE, aip, curr_type);  
				retval = END_STRUCT;
				valueptr->intvalue = END_STRUCT;
			}
			break;
		case CHOICE_TYPE:
			AsnTypeSetIndent(TRUE, aip, atp);  /* nest down to type */
			terminalvalue = FALSE;
			break;
		default:           /* set up for primitive type */
			base_type = AsnFindBaseType(atp);   
			base_type = base_type->type;        /* point at the type itself */

			while ((aip->tagclass != base_type->tagclass) ||
				(aip->tagnumber != base_type->tagnumber))/* read to element */
			{
				used = AsnDeBinScanTag(aip);
				if (! AsnDeBinDecr(used, aip))
					return 0;
			}
			
			if ((aip->bytes - aip->offset) == 0)
            {
                if (! AsnIoReadBlock(aip))
                    {AsnIoErrorMsg(aip, 17); return 0;}
            }
			switch (isa)
			{
				case BOOLEAN_TYPE:
					valueptr->boolvalue = AsnDeBinReadBoolean(aip, atp);
					break;
				case INTEGER_TYPE:
				case ENUM_TYPE:
					valueptr->intvalue = AsnDeBinReadInteger(aip, atp);
					break;
				case BIGINT_TYPE: 
                                        valueptr->bigintvalue = AsnDeBinReadBigInt(aip, atp);
                                        break;
				case REAL_TYPE:
					valueptr->realvalue = AsnDeBinReadReal(aip);
					break;
				case GENERALSTRING_TYPE:
					if (read_value)
					{
						valueptr->ptrvalue = AsnDeBinReadString(aip, atp);
						if (valueptr->ptrvalue == NULL)
							return 0;
					}
					else
						AsnDeBinSkipString(aip);
					break;
				case NULL_TYPE:
					AsnDeBinReadNull(aip, atp);
					break;
				case OCTETS_TYPE:
				case STRSTORE_TYPE:
					if (read_value)
					{
						valueptr->ptrvalue = AsnDeBinReadOctets(aip);
						if (valueptr->ptrvalue == NULL) return 0;
					}
					else
						AsnDeBinSkipOctets(aip);
					break;
				default:
					AsnIoErrorMsg(aip, 11, AsnErrGetTypeName(atp->name));
					return 0;
			}
			if (currpsp->tag_indef)     /* indefinate user tag */
			{
				used = AsnDeBinScanTag(aip);   /* should be 00 for tag */
				if (! AsnDeBinDecr(used, aip))
					return 0;
				if ((aip->length == 0) && (aip->tagclass == 0) &&
					(aip->tagnumber == 0))
					currpsp->tag_indef = FALSE;
				else
				{
					AsnIoErrorMsg(aip, 3, AsnErrGetTypeName(atp->name));
					return 0;
				}
			}
	}

	if (terminalvalue)       /* pop out of any CHOICE nests */
	{
	  	next_type = aip->type_indent - 1;   /* end any choices */
		if ((next_type >= 0) &&
			(AsnFindBaseIsa(aip->typestack[next_type].type) == CHOICE_TYPE))
		{
			while ((next_type >= 0) &&
				(AsnFindBaseIsa(aip->typestack[next_type].type) == CHOICE_TYPE))
			{
                currpsp = & aip->typestack[next_type];
				atp2 = currpsp->type;
				if ((curr_type->tagclass != TAG_NONE) &&
					(currpsp->tag_indef))
				{
					used = AsnDeBinScanTag(aip);   /* should be 00 for tag */
					if (! AsnDeBinDecr(used, aip))
						return 0;
					if ((aip->length == 0) && (aip->tagclass == 0) &&
						(aip->tagnumber == 0))
						currpsp->tag_indef = FALSE;
					else
					{
						AsnIoErrorMsg(aip, 3, AsnErrGetTypeName(atp2->name));
						return 0;
					}
				}
				next_type--;
			}
			AsnTypeSetIndent(FALSE, aip, curr_type);
		}
	}

	return retval;
}

/*****************************************************************************
*
*   AsnTypePtr AsnDeBinFindType(aip, amp)
*   	returns pointer to type definition in amp
*   	or NULL if not found
*
*****************************************************************************/
NLM_EXTERN AsnTypePtr AsnDeBinFindType (AsnIoPtr aip, AsnModulePtr amp)
{
	AsnTypePtr atp;
	
	while (amp != NULL)
	{
		atp = AsnDeBinFindElement(aip, amp->types);
		if (atp != NULL)
			return atp;

		amp = amp->next;
	}
	return NULL;
}				

/*****************************************************************************
*
*   AsnTypePtr AsnDeBinFindElement(aip, atp)
*   	finds an element in a list of elements
*   	(elements of SEQ, SET, CHOICE)
*
*****************************************************************************/
NLM_EXTERN AsnTypePtr AsnDeBinFindElement (AsnIoPtr aip, AsnTypePtr atp)
{
	while (atp != NULL)
	{
		if ((aip->tagclass == atp->tagclass) &&
			(aip->tagnumber == atp->tagnumber))
			return atp;
		else
			atp = atp->next;
	}
	return NULL;
}


/*****************************************************************************
*
*   Int4 AsnDeBinReadInteger(aip, atp)
*   	expects an INTEGER or ENUMERATED next
*   	assumes none of it has already been read
*   	does not advance to next token
*   	atp points to the definition of this integer for named values
*
*****************************************************************************/
NLM_EXTERN Int4 AsnDeBinReadInteger (AsnIoPtr aip, AsnTypePtr atp)
{
	Int2 bytes, len;
	Uint4 number;
	BytePtr bp;
	AsnValxNodePtr avnp;
	Int4 value;
    AsnTypePtr atp2;

	bytes = aip->bytes - aip->offset;
	bp = aip->buf + aip->offset;
	len = (Int2) aip->length;
	aip->tagsaved = FALSE;

	if ((*bp & 0x80) != 0)      /* negative number */
		number = (Uint4)(-1);
	else
		number = 0;

	while (len)
	{
		number = (number << 8) | (*bp & 0xff);
		bp++; bytes--; len--;
		if (! bytes)
		{
			bytes = AsnIoReadBlock(aip);
            if (! bytes)
                {AsnIoErrorMsg(aip, 17); return 0;}
			bp = aip->buf;
		}
	}

	value = (Int4) number;

	if (! AsnDeBinDecr(aip->length, aip))
		return 0;
	aip->offset = aip->bytes - bytes;

	if (AsnFindBaseIsa(atp) != ENUM_TYPE)
		return value;

			/******************** read a named integer value *********/

    atp2 = AsnFindBaseType(atp);
	if (atp2->branch != NULL)       /* named values */
	{
		avnp = (AsnValxNodePtr) atp2->branch;
		while (avnp != NULL)
		{
			if (avnp->intvalue == value)
				return value;
			avnp = avnp->next;
		}
	}

	AsnIoErrorMsg(aip, 12, value, AsnErrGetTypeName(atp->name));

	return 0;
}

/*****************************************************************************
*
*   Int8 AsnDeBinReadBigInt(aip, atp)
*   	expects an INTEGER next
*   	assumes none of it has already been read
*   	does not advance to next token
*   	atp points to the definition of this integer for named values
*
*****************************************************************************/
NLM_EXTERN Int8 AsnDeBinReadBigInt (AsnIoPtr aip, AsnTypePtr atp)
{
	Int2 bytes, len;
	Uint8 number;
	BytePtr bp;
	Int8 value;

	bytes = aip->bytes - aip->offset;
	bp = aip->buf + aip->offset;
	len = (Int2) aip->length;
	aip->tagsaved = FALSE;

	if ((*bp & 0x80) != 0)      /* negative number */
		number = (Uint8)(-1);
	else
		number = 0;

	while (len)
	{
		number = (number << 8) | (*bp & 0xff);
		bp++; bytes--; len--;
		if (! bytes)
		{
			bytes = AsnIoReadBlock(aip);
            if (! bytes)
                {AsnIoErrorMsg(aip, 17); return 0;}
			bp = aip->buf;
		}
	}

	value = (Int8) number;

	if (! AsnDeBinDecr(aip->length, aip))
		return 0;
	aip->offset = aip->bytes - bytes;

	if (AsnFindBaseIsa(atp) != ENUM_TYPE)
		return value;

			/******************** read a named integer value *********/

	   /* named value not supported for BigInt ****/


	AsnIoErrorMsg(aip, 12, value, AsnErrGetTypeName(atp->name));

	return (Int8)0;
}

/*****************************************************************************
*
*   Boolean AsnDeBinReadBoolean(aip, atp)
*   	expects a BOOLEAN next
*   	assumes none of it has already been read
*   	does not advance to next token
*   	atp points to the definition of this BOOLEN
*
*****************************************************************************/
NLM_EXTERN Boolean AsnDeBinReadBoolean (AsnIoPtr aip, AsnTypePtr atp)
{
	BytePtr bp;
	Byte octet;
	Boolean result = TRUE;

	aip->tagsaved = FALSE;
	if (aip->length != 1)
	{
		AsnIoErrorMsg(aip, 13, AsnErrGetTypeName(atp->name));
		return FALSE;
	}

	bp = aip->buf + aip->offset;
	octet = *bp;

	if (octet == 0)
		result = FALSE;
	else
		result = TRUE;

	aip->offset++;
	AsnDeBinDecr((Int4)1, aip);

	return result;
}

/*****************************************************************************
*
*   Int4 AsnDeBinReadString(aip, atp)
*   	expects any String type next
*   	assumes none of it has already been read
*   	does not advance to next token
*   	atp points to the definition of this String
*   	packs String to single value, if segmented on input
*
*****************************************************************************/
NLM_EXTERN CharPtr AsnDeBinReadString (AsnIoPtr aip, AsnTypePtr atp)
{
	Int4 len;
	register int bytes, amount;
	register BytePtr bp;
	register CharPtr tmp;
	CharPtr result;

	aip->tagsaved = FALSE;
	len = aip->length;
	AsnDeBinDecr(len, aip);
	result = (CharPtr) MemGet((size_t)(len + 1), FALSE);
	if (result == NULL)
	{
		AsnIoErrorMsg(aip, 14, len, AsnErrGetTypeName(atp->name));
		return NULL;
	}
	result[len] = '\0';

	bp = aip->buf + aip->offset;
	bytes = (int)(aip->bytes - aip->offset);
	tmp = result;

	while (len)
	{
		if ((Int4)bytes >= len)
			amount = (int) len;
		else
			amount = bytes;
		bytes -= amount;
		len -= (Int4)amount;
		while (amount)
		{
			if ((aip->fix_non_print != 2) && ((* bp < ' ') || (*bp > '~')))
			{
				*tmp = '\0';
				if ((aip->fix_non_print == 0) || (aip->fix_non_print == 3))
				{
					AsnIoErrorMsg(aip, 106, (int)(*bp), result);
				}
				*bp = '#';   /* repair string with # */
			}
			*tmp = *bp;
			tmp++; bp++; amount--;
		}
		if (! bytes)
		{
			bytes = (int) AsnIoReadBlock(aip);
            if (! bytes)
                {AsnIoErrorMsg(aip, 17); return NULL;}
			bp = aip->buf;
		}
	}
	aip->offset = aip->bytes - (Int2) bytes;
	return result;
}

/*****************************************************************************
*
*   void AsnDeBinSkipString(aip)
*   	expects any String type next
*   	assumes none of it has already been read
*   	does not advance to next token
*   	just reads past end of string
*
*****************************************************************************/
NLM_EXTERN void AsnDeBinSkipString (AsnIoPtr aip)
{
	Int4 len;
	register int bytes, amount;
	register BytePtr bp, tmp;
	int bad;

	aip->tagsaved = FALSE;
	len = aip->length;
	AsnDeBinDecr(len, aip);

	bp = aip->buf + aip->offset;
	tmp = bp;
	bytes = (int)(aip->bytes - aip->offset);

	while (len)
	{
		if ((Int4)bytes >= len)
			amount = (int) len;
		else
			amount = bytes;
		bytes -= amount;
		len -= (Int4)amount;
		while (amount)
		{
			if ((* bp < ' ') || (*bp > '~'))
			{
				bad = (int)(*bp);
				*bp = '\0';
				AsnIoErrorMsg(aip, 106, bad, (CharPtr)tmp);
				aip->io_failure = TRUE;
			}
			bp++; amount--;
		}
		if (! bytes)
		{
			bytes = (int) AsnIoReadBlock(aip);
            if (! bytes)
                {AsnIoErrorMsg(aip, 17); return;}
			bp = aip->buf;
			tmp = bp;
		}
	}
	aip->offset = aip->bytes - (Int2) bytes;
	return;
}

/*****************************************************************************
*
*    AsnDeBinReadOctets(aip)
*   	expects any OCTETS type next
*   	assumes none of it has already been read
*   	does not advance to next token
*
*****************************************************************************/
NLM_EXTERN ByteStorePtr AsnDeBinReadOctets (AsnIoPtr aip)
{
	Int4 len;
	Int2 bytes, amount;
	BytePtr bp;
	ByteStorePtr bsp;
	
	aip->tagsaved = FALSE;
	len = aip->length;
	AsnDeBinDecr(len, aip);
	bsp = BSNew((Uint4)len);
	if (bsp == NULL) return bsp;

	bp = aip->buf + aip->offset;
	bytes = aip->bytes - aip->offset;

	while (len)
	{
		if ((Int4)bytes >= len)
			amount = (Int2) len;
		else
			amount = bytes;
		BSWrite(bsp, bp, amount);
		len -= amount;
		bytes -= amount;
		if (! bytes)
		{
			bytes = AsnIoReadBlock(aip);
            if (! bytes)
                {AsnIoErrorMsg(aip, 17); return NULL;}
			bp = aip->buf;
		}
	}
	aip->offset = aip->bytes - bytes;
	return bsp;
}

/*****************************************************************************
*
*    AsnDeBinSkipOctets(aip)
*   	expects any OCTETS type next
*   	assumes none of it has already been read
*   	does not advance to next token
*
*****************************************************************************/
NLM_EXTERN void AsnDeBinSkipOctets (AsnIoPtr aip)
{
	Int4 len;
	Int2 bytes, amount;
	
	aip->tagsaved = FALSE;
	len = aip->length;
	AsnDeBinDecr(len, aip);

	bytes = aip->bytes - aip->offset;

	while (len)
	{
		if ((Int4)bytes >= len)
			amount = (Int2) len;
		else
			amount = bytes;
		len -= amount;
		bytes -= amount;
		if (! bytes)
		{
			bytes = AsnIoReadBlock(aip);
            if (! bytes)
                {AsnIoErrorMsg(aip, 17); return ;}
		}
	}
	aip->offset = aip->bytes - bytes;
	return;
}

/*****************************************************************************
*
*   FloatHi AsnDeBinReadReal(aip)
*   	expects a REAL next
*   	assumes none of it has already been read
*   	does not advance to next token
*
*****************************************************************************/
NLM_EXTERN FloatHi AsnDeBinReadReal (AsnIoPtr aip)
{
	double result = 0.0;
	Int4 len;
	Int2 bytes, amount;
	BytePtr bp;
	char buf[200], *tmp;

	aip->tagsaved = FALSE;
	len = aip->length;
	AsnDeBinDecr(len, aip);

	bp = aip->buf + aip->offset;
	bytes = aip->bytes - aip->offset;
	tmp = buf;

	while (len)
	{
		if ((Int4)bytes >= len)
			amount = (Int2) len;
		else
			amount = bytes;
		MemCopy(tmp, bp, (size_t)amount);
		tmp += amount;
		len -= amount;
		bytes -= amount;
		if (! bytes)
		{
			bytes = AsnIoReadBlock(aip);
            if (! bytes)
                {AsnIoErrorMsg(aip, 17); return (FloatHi)result;}
			bp = aip->buf;
		}
	}
	aip->offset = aip->bytes - bytes;

	if (buf[0] != '\0')        /* not decimal encoding */
	{
		AsnIoErrorMsg(aip, 15);
		return (FloatHi)result;
	}

	*tmp = '\0';
	                         /* range check base 10 exponents assuming doubles */
	                         
	if ((tmp = StringStr((buf+1), "e")) != NULL)
	{
		int n = atoi(tmp + 1);
		if (n < DBL_MIN_10_EXP)     /** too small, return 0.0 **/
			return (FloatHi)result;
		else if (n > DBL_MAX_10_EXP)   /** too big, return the max */
			return (FloatHi)DBL_MAX;
	}
	
	if (sscanf((buf + 1), "%lf", &result) != 1) {
		result = 0.0;
	}
	return (FloatHi)result;
}

/*****************************************************************************
*
*   void AsnDeBinReadNull(aip, atp)
*
*****************************************************************************/
NLM_EXTERN void AsnDeBinReadNull (AsnIoPtr aip, AsnTypePtr atp)
{
	aip->tagsaved = FALSE;
	if (aip->length != 0)
		AsnIoErrorMsg(aip, 16, AsnErrGetTypeName(atp->name));
		
	return;
}

/*****************************************************************************
*
*   void AsnDeBinSkipStruct(aip)
*   	skips current structure and all substructures
*       Does NOT read following element
*
*****************************************************************************/
NLM_EXTERN void AsnDeBinSkipStruct (AsnIoPtr aip)
{
	aip->tagsaved = FALSE;
	AsnIoErrorMsg(aip, 0);
	return;
}

