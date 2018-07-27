/* asntypes.c
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
* File Name: asntypes.c
*
* Author:  James Ostell
*
* Version Creation Date: 3/4/91
*
* $Revision: 6.9 $
*
* File Description:
*   Routines to deal with internal operations on AsnType objects.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 3/4/91   Kans        Stricter typecasting for GNU C and C++
* 04-20-93 Schuler     LIBCALL calling convention
* 02-24-94 Schuler     Make AsnTypeStringToHex LIBCALL too
*
* $Log: asntypes.c,v $
* Revision 6.9  2003/12/03 19:31:09  gouriano
* Corrected DTD generation (a different approach)
*
* Revision 6.8  2003/12/02 21:40:17  gouriano
* Revert back to rev 6.6
*
* Revision 6.7  2003/12/02 19:52:49  gouriano
* Corrected DTD generation
*
* Revision 6.6  2000/07/25 20:30:58  ostell
* added support for printing multiple ASN.1 modules as multiple XML DTD and .mod files
*
* Revision 6.5  2000/05/26 14:51:23  ostell
* fixed uninitialized variable
*
* Revision 6.4  2000/05/10 03:12:37  ostell
* added support for XML DTD and XML data output
*
* Revision 6.2  1998/06/12 19:27:56  kans
* fixed unix compiler warnings
*
* Revision 6.1  1997/10/29 02:42:10  vakatov
* Type castings to pass through the C++ compiler
*
* Revision 6.0  1997/08/25 18:10:25  madden
* Revision changed to 6.0
*
* Revision 5.3  1997/04/24 12:47:49  ostell
* in AsnTypeStringToHex, allowed argument "left" to be NULL
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
 * Revision 4.0  1995/07/26  13:47:38  ostell
 * force revision to 4.0
 *
 * Revision 2.19  1995/05/20  17:05:26  ostell
 * added ASNTYPE_RUNTIME_LINK
 * if defined enables oldstyle runtime link of AsnLinkType().
 * default is disabled, which assumes all linking of asn.1 parse trees
 * via calls to AsnLoad() functions is done before any AsnREAD or AsnWrite
 *
 * Revision 2.18  1995/05/15  18:38:28  ostell
 * added Log line
 *
*
* ==========================================================================
*/

/*****************************************************************************
*
*   asntypes.c
*   	basic routines for processing asn1 types
*   	does not support lexxing of type definitions - those are in
*   		asnlext.c
*
*****************************************************************************/

#include "asnbuild.h"

/*****************************************************************************
*
*   This stores all loaded parse trees and links them together
*
*****************************************************************************/
static ValNodePtr
    amps ,                /* the AsnModulePtr blocks */
    names ;               /* the load file names */

/*****************************************************************************
*
*   CharPtr AsnFindPrimName(atp)
*   	returns the string name of an ASN.1 primitive type
*
*****************************************************************************/
NLM_EXTERN CharPtr LIBCALL  AsnFindPrimName (AsnTypePtr atp)
{
	AsnTypePtr atp2;

	atp2 = AsnFindBaseType(atp);
	if (atp2 == NULL)
		return NULL;

	return atp2->type->name;
}

/*****************************************************************************
*
*   CharPtr AsnFindBaseName(atp)
*   	returns the string name of an ASN.1 basic entity
*       returns NULL if not resolved
*       returns NULL if resolves to a primitive type (use AsnFindPrimName)
*
*****************************************************************************/
NLM_EXTERN CharPtr LIBCALL  AsnFindBaseName (AsnTypePtr atp)
{
	AsnTypePtr atp2;

	atp2 = AsnFindBaseType(atp);
	if (atp2 == NULL)
		return NULL;

	if (atp2->name == NULL)
		return NULL;
	else if (IS_UPPER(*atp2->name))
		return atp2->name;
	else
		return NULL;
}

/*****************************************************************************
*
*   void AsnKillValue(atp, dvp)
*   	eliminates stored values such as strings
*
*****************************************************************************/
NLM_EXTERN void LIBCALL  AsnKillValue (AsnTypePtr atp, DataValPtr dvp)
{
	Int2 isa;

	isa = AsnFindBaseIsa(atp);
	if (ISA_STRINGTYPE(isa))
		MemFree(dvp->ptrvalue);
	else if ((isa == OCTETS_TYPE) || (isa == STRSTORE_TYPE))
		BSFree((ByteStorePtr) dvp->ptrvalue);
	return;
}

/*****************************************************************************
*
*   void AsnTypeSetIndent(increase, aip, atp)
*   	set the typestack type_indent
*
*****************************************************************************/
NLM_EXTERN void AsnTypeSetIndent (Boolean increase, AsnIoPtr aip, AsnTypePtr atp)
{
	Int1 curr_indent;
	PstackPtr tmptype;
	

	if (increase)
	{
		aip->typestack[aip->type_indent].type = atp;     /* save type */
		aip->type_indent++;
		curr_indent = aip->type_indent;
		if (curr_indent == aip->max_type)   /* expand indent levels */
		{
			tmptype = aip->typestack;
			aip->typestack = (PstackPtr) MemNew((sizeof(Pstack) * (aip->max_type + 10)));
			MemCopy(aip->typestack, tmptype, (size_t)(sizeof(Pstack) * aip->max_type));
			MemFree(tmptype);
			aip->max_type += 10;
		}
		aip->typestack[curr_indent].type = NULL;     /* set to first time */
	}
	else
	{
		do
		{
			aip->typestack[aip->type_indent].type = NULL;
			if (aip->type_indent)
				aip->type_indent--;
		} while ((aip->type_indent) &&
			(AsnFindBaseIsa(aip->typestack[aip->type_indent-1].type) == CHOICE_TYPE)
			 && (aip->token != ISMODULE_TOKEN) && (!(aip->type & ASNIO_XML)));
		/* this is to pop out of choice nests */
		/* XML popping is formatted at ends */
	}
	return;
}

/*****************************************************************************
*
*   AsnTypePtr AsnFindBaseType(atp)
*   	find the primitive type for atp
*
*****************************************************************************/
NLM_EXTERN AsnTypePtr LIBCALL AsnFindBaseType (AsnTypePtr atp)
{
	if (atp == NULL) return NULL;

	if (atp->type == NULL) return NULL;

	while (! ISA_BASETYPE(atp->type->isa))
	{
		atp = atp->type;
		if (atp->type == NULL)   /* not found */
			return NULL;
	}
	return atp;
}

NLM_EXTERN AsnTypePtr LIBCALL AsnFindBaseTypeDTD (AsnTypePtr atp)
{
	if (atp == NULL) return NULL;

	if (atp->type == NULL) return NULL;

	if (! ISA_BASETYPE(atp->type->isa))
	{
		atp = atp->type;
		if (atp->type == NULL)   /* not found */
			return atp;
	}
	return atp;
}

/*****************************************************************************
*
*   Int2 AsnFindBaseIsa(atp)
*   	find the primitive type for atp
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL AsnFindBaseIsa (AsnTypePtr atp)
{
	if (atp == NULL)
		return -1;

	if (ISA_BASETYPE(atp->isa))
		return atp->isa;

	if (atp->type == NULL)
		return -1;

	while (! ISA_BASETYPE(atp->type->isa))
	{
		atp = atp->type;
		if (atp->type == NULL)
			return -1;
	}
	return atp->type->isa;
}

/*****************************************************************************
*
*   Boolean AsnTypeValidateOut(aip, atp, dvp)
*   	if not open or closing a SET, SEQ, SETOF, SEQOF, will
*   		put atp in aip->typestack if it validates.
*
*   KNOWN DEFECT:  SET should check for 1 of each non-optional element
*   	It does not right now, it only checks that an element is a valid
*   	member of the SET.
*
*****************************************************************************/
NLM_EXTERN Boolean AsnTypeValidateOut (AsnIoPtr aip, AsnTypePtr atp, DataValPtr dvp)
{
	Int2 isa;
	AsnTypePtr atp2, parent_type, base_type, curr_type;
	Boolean foundit;
	AsnValxNodePtr avnp;

	if ((aip == NULL) || (atp == NULL))
		return FALSE;
		
	curr_type = aip->typestack[aip->type_indent].type;
	base_type = AsnFindBaseType(atp);
	if (base_type == NULL)
	{
		AsnIoErrorMsg(aip, 10, AsnErrGetTypeName(atp->name), "AsnTypeValidateOut");
		return FALSE;
	}
		
	isa = base_type->type->isa;

	if (((isa == SEQ_TYPE) || (isa == SET_TYPE) ||
		 (isa == SEQOF_TYPE) || (isa == SETOF_TYPE))
		 && (dvp->intvalue == END_STRUCT))
	{
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
						return FALSE;
					}
					atp2 = atp2->next;
				}
				break;
			default:
				break;
		}
		return TRUE;
	}

	if (aip->type_indent)       /* part of a larger struct */
	{
		foundit = FALSE;
		if (aip->type_indent < 1)
			return FALSE;
			
		parent_type = aip->typestack[aip->type_indent - 1].type;
		base_type = AsnFindBaseType(parent_type);
		if (base_type == NULL)
			return FALSE;
		isa = base_type->type->isa;
		atp2 = (AsnTypePtr) base_type->branch;
		while (atp2 != NULL)
		{
			if (atp2 == atp)
			{
				foundit = TRUE;
				break;
			}
			atp2 = atp2->next;
		}
		if (! foundit)
		{
			atp2 = AsnFindBaseType(atp);
			AsnIoErrorMsg(aip, 22, AsnErrGetTypeName(atp->name),
				AsnErrGetTypeName(atp2->name), AsnErrGetTypeName(parent_type->name));
			return FALSE;
		}
		switch (isa)
		{
			case SETOF_TYPE:       /* just has to be right type */
			case SEQOF_TYPE:
				break;
			case CHOICE_TYPE:      /* can only have one value */
				if (curr_type != NULL)
				{
					AsnIoErrorMsg(aip, 23, AsnErrGetTypeName(parent_type->name),
						AsnErrGetTypeName(atp->name));
					return FALSE;
				}
				break;
			case SET_TYPE:         /* SET should check for 1 of each */
				                   /* non-optional element, but doesn't */
				if (curr_type == atp)
				{
					AsnIoErrorMsg(aip, 24, AsnErrGetTypeName(atp->name),
						AsnErrGetTypeName(parent_type->name));
					return FALSE;
				}
				break;
			case SEQ_TYPE:
				if (curr_type == atp)
				{
					AsnIoErrorMsg(aip, 24, AsnErrGetTypeName(atp->name),
						AsnErrGetTypeName(parent_type->name));
					return FALSE;
				}
				 				 /* check preceeding elements */
				if (curr_type != NULL)
				{
					atp2 = (AsnTypePtr) base_type->branch;
					while (atp2 != curr_type->next)
					{
						if (atp == atp2)
						{
							AsnIoErrorMsg(aip, 6, AsnErrGetTypeName(atp->name),
								AsnErrGetTypeName(parent_type->name));
							return FALSE;
						}
						atp2 = atp2->next;
					}
				}
				else
					atp2 = (AsnTypePtr) base_type->branch;
				while ((atp2 != NULL) && (atp != atp2))
				{
					if (! (atp2->optional || atp2->hasdefault))
					{					/* skipped a non-optional element */
						AsnIoErrorMsg(aip, 7, AsnErrGetTypeName(atp2->name),
							AsnErrGetTypeName(parent_type->name));
						return FALSE;
					}
					atp2 = atp2->next;
				}
				if (atp2 == NULL)  /* element out of order */
				{
					AsnIoErrorMsg(aip, 6, AsnErrGetTypeName(atp->name),
						AsnErrGetTypeName(parent_type->name));
					return FALSE;
				}
				break;
			default:
				AsnIoErrorMsg(aip, 25, AsnErrGetTypeName(parent_type->name),
					parent_type->isa);
				return FALSE;
		}
		base_type = AsnFindBaseType(atp);
		isa = base_type->type->isa;
	}

	if (ISA_STRINGTYPE(isa))
		isa = GENERALSTRING_TYPE;

				/******************* maintain typestack ****************/
	switch (isa)          /* set aip->typestack for non-struct types */
	{					  /* this is done for them in AsnTypeSetIndent() */
		case SEQ_TYPE:
		case SET_TYPE:
		case SEQOF_TYPE:
		case SETOF_TYPE:
		case CHOICE_TYPE:
		default:				   /* terminal value */
			aip->typestack[aip->type_indent].type = atp;
			break;
	}

	switch (isa)          /* check ranges and values */
	{
		case ENUM_TYPE:
			avnp = (AsnValxNodePtr) base_type->branch;
			while (avnp != NULL)
			{
				if (avnp->intvalue == dvp->intvalue)
					return TRUE;
				avnp = avnp->next;
			}
			AsnIoErrorMsg(aip, 12, dvp->intvalue, AsnErrGetTypeName(atp->name));
			return FALSE;
		default :
			break;
	}
	return TRUE;	
}

/*****************************************************************************
*
*   AsnTypePathFind(amp, str, countptr)
*   	like AsnTypeFind() but allocates an array of AsnTypePtr and fills
*       it in with the proper path.  Sets countptr to sizeof array.
*       Returns array or NULL on failure
*
*****************************************************************************/
NLM_EXTERN AsnTypePtr PNTR LIBCALL  AsnTypePathFind (AsnModulePtr amp, CharPtr str, Int2Ptr countptr)
{
	AsnModulePtr amp2;
	AsnTypePtr atp;
	Int2 count;
	AsnTypePtr PNTR typeptr;
	CharPtr ptr;

	*countptr = 0;
	count = 1;
	ptr = str;
	while (*ptr != '\0')
	{
		if (*ptr == '.')
			count++;
		ptr++;
	}
	typeptr = (AsnTypePtr*) MemNew(sizeof(AsnTypePtr) * count);

    if (amp == NULL)
	{
		if (amps == NULL)
		{
			AsnIoErrorMsg(NULL, (Int2)105);
			return NULL;
		}
        amp = (AsnModulePtr) amps->data.ptrvalue;
	}

	amp2 = amp;
	atp = NULL;

	while (amp2 != NULL)
	{
		atp = AsnTypeFindType(amp2->types, str, typeptr, count, FALSE);
		if (atp != NULL)
		{
			*countptr = count;
			return typeptr;
		}
		else
			amp2 = amp2->next;
	}

	MemFree(typeptr);
	return NULL;       /* not found */
}


/*****************************************************************************
*
*   AsnTypePtr AsnTypeFind(amp, str)
*   	finds a type in a module or set of modules
*   	str is a path with dot separation.
*   	E is used to indicate an element of SEQOF or SETOF
*   	looks for partial paths as well
*
*   if called with amp == NULL
*       searches all loaded modules
*   returns a single node at the end of the path
*
*****************************************************************************/
NLM_EXTERN AsnTypePtr LIBCALL  AsnTypeFind (AsnModulePtr amp, CharPtr str)
{
	AsnModulePtr amp2;
	AsnTypePtr atp;

    if (amp == NULL)
	{
		if (amps == NULL)
		{
			AsnIoErrorMsg(NULL, (Int2)105);
			return NULL;
		}
        amp = (AsnModulePtr) amps->data.ptrvalue;
	}

	amp2 = amp;
	atp = NULL;

	while (amp2 != NULL)
	{
		atp = AsnTypeFindType(amp2->types, str, NULL, 0, FALSE);
		if (atp != NULL)
			return atp;
		else
			amp2 = amp2->next;
	}

	return atp;       /* not found */
}

/*****************************************************************************
*
*   AsnTypePtr AsnTypeFindType(atp, str, typeptr, count, in_it)
*
*****************************************************************************/
NLM_EXTERN AsnTypePtr AsnTypeFindType (AsnTypePtr atp, CharPtr str, AsnTypePtr PNTR typeptr, Int2 count, Boolean in_it)
{
	AsnTypePtr curr, next;
	CharPtr beg;
	Int2 len, isa;
	Boolean is_element,       /* (unamed) element of setof seqof */
		is_type,              /* if a type, must find the original def */
		got_it;
	AsnTypePtr PNTR typenext;
	Int2 newcount;

	if (count < 0)      /* off array ... error condition */
		return NULL;

	beg = str;
	len = 0;
	if (typeptr == NULL)
	{
		typenext = NULL;
		newcount = 0;
	}
	else
	{
		newcount = count - 1;
		if (newcount >= 0)
			typenext = typeptr + 1;
		else
			typenext = NULL;
	}
	is_element = FALSE;
	is_type = FALSE;
	while ((*beg != '.') && (*beg != '\0'))
	{
		beg++; len++;
	}

	if ((*str == 'E') && (len == 1))
		is_element = TRUE;
	else if (IS_UPPER(*str))
		is_type = TRUE;

	curr = atp;			  	/* look at this level */
	while (curr != NULL)
	{
		got_it = FALSE;
		if (is_element)
			got_it = TRUE;
		else if (StrMatch(curr->name, str, len))   /* found it */
		{
			if (! is_type)
				got_it = TRUE;
			else                    /* check that this is the real def */
			{
				if (! curr->imported)
					got_it = TRUE;
			}
		}

		if (got_it)
		{
			if (typeptr != NULL)
				*typeptr = curr;

			if (*beg == '\0')    /* that's it */
				return curr;
			else                 /* go down the path */
			{
				next = AsnFindBaseType(curr);
				isa = next->type->isa;
				if ((next->branch != NULL)   /* go further */
					&& (isa != INTEGER_TYPE)
					&& (isa != ENUM_TYPE))
				{
					next = AsnTypeFindType((AsnTypePtr) next->branch, (beg + 1), typenext, newcount, TRUE);
					if (next != NULL)
						return next;
				}
			}
		}
		curr = curr->next;
	}
	
	if (in_it)     /* processing a path and can't find next node here */
		return NULL;

	curr = atp;           /* start down a level */
	while (curr != NULL)
	{
		if (curr->branch != NULL)
		{
			next = AsnFindBaseType(curr);
			
			if ((next != NULL) && (next->type != NULL))
			{
				isa = next->type->isa;
				if ((next->branch != NULL)   /* go further */
					&& (isa != INTEGER_TYPE)
					&& (isa != ENUM_TYPE))
				{
					next = AsnTypeFindType((AsnTypePtr) next->branch, str, typenext, newcount, FALSE);
					if (next != NULL)
					{
						if (typeptr != NULL)
							*typeptr = curr;
						return next;
					}
				}
			}
			/** else, an error has occurred, unresolved branch **/
		}
		curr = curr->next;
	}
	return NULL;
}

/*****************************************************************************
*
*   CharPtr AsnTypeDumpStack(str, aip)
*   	dump a typestack into str
*       returns a pointer to the '\0' at the end of the str
*
*****************************************************************************/
NLM_EXTERN CharPtr LIBCALL  AsnTypeDumpStack (CharPtr str, AsnIoPtr aip)
{
    Int2 level, isa, lastnochoice;

    lastnochoice = -1;
    *str = '\0';

	for(level = 0; level <= aip->type_indent; level++)
	{
		if (level == aip->type_indent)
		{
			if ((aip->type & ASNIO_OUT) ||
				((aip->type & ASNIO_IN) && (aip->read_id == FALSE)))
				str = StringMove(str, "<");
		}
		if (aip->typestack[level].type == NULL)
			str = StringMove(str, " ");
		else
		{
			isa = AsnFindBaseIsa(aip->typestack[level].type);
            if (aip->typestack[level].type->name != NULL)
                str = StringMove(str, aip->typestack[level].type->name);
            else if ((lastnochoice == SEQOF_TYPE) ||
                        (lastnochoice == SETOF_TYPE))
                str = StringMove(str, "E");
            else
                str = StringMove(str, "?");
            if (isa != CHOICE_TYPE)
                lastnochoice = isa;
		}
        if (level != aip->type_indent)
            str = StringMove(str, ".");
		else
		{
			if ((aip->type & ASNIO_OUT) ||
				((aip->type & ASNIO_IN) && (aip->read_id == FALSE)))
				str = StringMove(str, ">");
		}
	}
	return str;
}

/*****************************************************************************
*
*   Int4 AsnTypeStringToHex(from, len, to, left)
*   	converts an octet string to binary
*   	returns number of hex digits created if all ok
*       *left is chars left at the end of the buffer including first letter of
*          a remaining digit (from does not have an even number of letters)
*          since this could include white space, could be more than 1
*       returns a negative number on an error
*       skips over internal or trailing white space
*       left can be NULL in which case it is ignored
*
*****************************************************************************/
NLM_EXTERN Int4 LIBCALL AsnTypeStringToHex (Pointer from, Int4 len, Pointer to, Int4Ptr left)
{
	BytePtr f, t;
	Byte octet = 0, value;
	int i;
	Int4 added = 0;

	f = (BytePtr) from;
	t = (BytePtr) to;
	if (left != NULL)
		*left = 0;
	i = 16;

	while (len)
	{
	     if (! IS_WHITESP(*f))  /* skip spaces */
	     {
		if (i == 16)       /* first letter of pair */
		{
			octet = 0;
			if (left != NULL)
				*left = len; /* point at it in case one letter left */
		}

		value = TO_UPPER(*f);
		if ((value >= 'A') && (value <= 'F'))
			octet += (((value - 'A') + 10) * i);
		else if ((value >= '0') && (value <= '9'))
			octet += ((value - '0') * i);
		else
			return (Int4)(-1);

		if (i == 16)     /* first letter of pair */
			i = 1;   /* goto second letter */
		else             /* letter pair was read */
		{
			i = 16;  /* reset for first letter */
			if (left != NULL)
				*left = 0;  /* nothing left so far */
			*t = octet;  /* record the hex digit */
			t++; added++;
		}

	     }
	     len--;
	     f++;
	}
	return added;
}

#ifdef ASNTYPE_RUNTIME_LINK
static Int2 tstack_max;
static AsnTypePtr PNTR tstack;
static Int2Ptr prev;
#endif

/*****************************************************************************
*
*   AsnLinkType(type, localtype)
*      provides for linkage of imported types
*      if (type != NULL)
*         it's an element of type localtype
*         so, set type->type to localtype
*      else
*         it's localtype itself
*         so, return localtype
*      return pointer to correct type
*
*      ifdef'd OUT NOW THAT LINKING IS DONE AT LOAD TIME
*
*****************************************************************************/
NLM_EXTERN AsnTypePtr LIBCALL  AsnLinkType (AsnTypePtr type, AsnTypePtr localtype)
{

	if (type == NULL)
		return localtype;
	else
#ifndef ASNTYPE_RUNTIME_LINK
		return type;        /* assume has been linked at tree load time */
#else
	{
    Int2 i, j;
    Int2Ptr oldprev;
    AsnTypePtr PNTR oldtstack;

        for (i = 0; i < tstack_max; i++)
		{
	        if (tstack[i] == NULL)
                break;
		}

        if (i >= tstack_max)   /* increase stack size */
        {
            oldtstack = tstack;
            oldprev = prev;
            tstack = MemNew(((tstack_max + 20) * sizeof(AsnTypePtr)));
            prev = MemNew(((tstack_max + 20) * sizeof(Int2)));
            MemCopy(tstack, oldtstack, (size_t)(tstack_max * sizeof(AsnTypePtr)));
            MemCopy(prev, oldprev, (size_t)(tstack_max * sizeof(Int2)));
            MemFree(oldtstack);
            MemFree(oldprev);
            tstack_max += 20;
        }

        j = type->tmp;
        if (j)    /* previous type linked */
            prev[i] = j;
        tstack[i] = type->type;
        type->tmp =  (i + 1);
		if (type != localtype)   /* don't link to itself */
			type->type = localtype;
		return type;
	}
#endif
}

/*****************************************************************************
*
*   AsnUnlinkType(type)
*     disconnects local tree.
*
*****************************************************************************/
NLM_EXTERN void LIBCALL  AsnUnlinkType (AsnTypePtr type)
{
#ifdef ASNTYPE_RUNTIME_LINK
    Int2 j;

	if (type != NULL)
	{
        j = type->tmp;
        j--;
		type->type = tstack[j];
        if (prev[j])
            type->tmp = prev[j];
        else
    		type->tmp = 0;
        tstack[j] = NULL;
        prev[j] = 0;
	}
#endif
	return;
}

static void AddXMLname(AsnTypePtr atp, AsnTypePtr PNTR typestack, Int2 stackptr)
{
	Char buf[80];
	AsnTypePtr atp2;
	Boolean found=FALSE, getparent=TRUE, doitem = FALSE, done = FALSE;
	CharPtr tmp;
	Int2 i, isa;

	if (atp->XMLname == NULL)   /* only do it once */
	{
		tmp = buf; *tmp = '\0';
		if (atp->name == NULL)
		{
			doitem = TRUE;
		}
		else if (IS_UPPER(*(atp->name)))
		{
			getparent = FALSE;
			tmp = StringMove(tmp, atp->name);
		}

		if ((getparent) && (stackptr))
		{
			atp2 = typestack[stackptr - 1];
			isa = AsnFindBaseIsa(atp2);
			if ((doitem) && ((isa == SEQOF_TYPE) || (isa == SETOF_TYPE)))
			{
				atp2 = AsnFindBaseTypeDTD(atp);
				if (atp2 != NULL)
				{
				if (atp2->name == NULL)
				{
					atp2 = typestack[stackptr - 1];
					if (atp->type != NULL)
					{
						if (ISA_BASETYPE(atp->type->isa))
						{
							if (atp2->XMLname != NULL)
							{
								tmp = StringMove(tmp, atp2->XMLname);
								getparent = FALSE;
								found = TRUE;
							}
						}
					}
				}
				else
				{
					if (atp2->XMLname != NULL)
						tmp = StringMove(tmp, atp2->XMLname);
					else if (atp2->name != NULL)
						tmp = StringMove(tmp, atp2->name);
					if (*buf != '\0')
					{
						doitem = FALSE;
						getparent = FALSE;
						done = TRUE;
						found = TRUE;
					}
				}
				}
			}

			atp2 = typestack[stackptr - 1];
			if ((getparent) && (atp2->XMLname != NULL))  /* already nested */
			{
				tmp = StringMove(tmp, atp2->XMLname);
				tmp = StringMove(tmp, "_");
				getparent = FALSE;
				found = TRUE;
			}

			if ((stackptr) && (getparent))
			{
				found = FALSE;

				for (i = (stackptr - 1); (i >= 0) && (! found); i--)
				{
					atp2 = AsnFindBaseTypeDTD(typestack[i]);
					if ((atp2 != NULL) && (atp2->name != NULL) && (IS_UPPER(*(atp2->name))))
					{
						while (i < stackptr)
						{
							atp2 = AsnFindBaseTypeDTD(typestack[i]);
							if (atp2->name == NULL)
								tmp = StringMove(tmp, "E");
							else
								tmp = StringMove(tmp, atp2->name);
							tmp = StringMove(tmp, "_");
							i++;
						}
						found = TRUE;
					}
				}
			}

			if ((! done) && found)
			{
				if (doitem)
					tmp = StringMove(tmp, "_E");
				else
					tmp = StringMove(tmp, atp->name);
			}
		}


		if (*buf != '\0')
		{
			atp->XMLname = StringSave(buf);
		}
	}

	atp2 = AsnFindBaseTypeDTD(atp);
	if ((! atp->imported) && (atp2 != NULL) && (atp2->type != NULL))
	{
		isa = atp2->type->isa;
		if ((isa == SEQ_TYPE) || (isa == SET_TYPE))
		{
			typestack[stackptr] = atp;
			for (atp2 = atp->branch; atp2 != NULL; atp2 = atp2->next)
				AddXMLname(atp2, typestack, (Int2)(stackptr + 1));
		}
		else if ((isa == SEQOF_TYPE) || (isa == SETOF_TYPE))
		{
			typestack[stackptr] = atp;
			for (atp2 = atp->branch; atp2 != NULL; atp2 = atp2->next)
				AddXMLname(atp2, typestack, (Int2)(stackptr + 1));
		}			
		else if (isa == CHOICE_TYPE)
		{
			typestack[stackptr] = atp;
			for (atp2 = atp->branch; atp2 != NULL; atp2 = atp2->next)
				AddXMLname(atp2, typestack, (Int2)(stackptr + 1));
		}
	}

	return;

}
/*****************************************************************************
*
*   void AsnModuleLink(amp)
*   	links IMPORT types with EXPORTS from other modules
*   	if cannot link an IMPORT, no errors are generated
*
*****************************************************************************/
NLM_EXTERN void LIBCALL  AsnModuleLink (AsnModulePtr amp)
{
	AsnTypePtr atp, atp2, typestack[10];
	AsnModulePtr currmod, mod;
	Boolean found;

	currmod = amp;
	while (currmod != NULL)
	{
		atp = currmod->types;
		while (atp != NULL)
		{
			if ((atp->imported) && (atp->type == NULL)) /* link imported types */
			{
				mod = amp;
				if (mod == currmod)
					mod = mod->next;
				found = FALSE;
				while ((mod != NULL) && (! found))
				{
					atp2 = mod->types;
					while ((atp2 != NULL) && (! found))
					{
						if ((! atp2->imported) &&
                        	(! StringCmp(atp2->name, atp->name)))
						{
							if (atp2->exported)
							{
								atp->type = atp2;
								found = TRUE;
							}
							else
							{
								AsnIoErrorMsg(NULL, 84,	AsnErrGetTypeName(atp2->name),
									mod->modulename);
								atp2 = atp2->next;
							}
						}
						else
							atp2 = atp2->next;
					}
					mod = mod->next;
					if (mod == currmod)
						mod = mod->next;
				}
			}
			atp = atp->next;
		}
		currmod = currmod->next;
	}

	/*** Fill in the XML names *********/

	currmod = amp;
	while (currmod != NULL)
	{
		atp = currmod->types;
		while (atp != NULL)
		{
			AddXMLname(atp, typestack, (Int2)0);
			atp = atp->next;
		}
		currmod = currmod->next;
	}


	return;
}

/*****************************************************************************
*
*   AsnStoreTree(file, amp)
*
*****************************************************************************/
NLM_EXTERN void LIBCALL AsnStoreTree (CharPtr file, AsnModulePtr amp)
{
    ValNodePtr anp, last;
    AsnModulePtr tamp, tamp2, newamp;
    Int2 numamp;

                    /* check to see if we already have it (could happen) */

    anp = names;
    last = NULL;
    while (anp != NULL)
    {
        if (! StringCmp((CharPtr)anp->data.ptrvalue, file))
            return;   /* have it */
        last = anp;
        anp = anp->next;
    }

                    /* add the name */
    anp = ValNodeNew(last);
    if (last == NULL)
        names = anp;
    anp->data.ptrvalue = (Pointer) StringSave(file);

    last = NULL;
    anp = amps;
    while (anp != NULL)
    {
        last = anp;
        anp = anp->next;
    }
    anp = ValNodeNew(last);
    if (last == NULL)
        amps = anp;

    tamp = amp;
    numamp = 0;
    while (tamp != NULL)
    {
        numamp++;
        tamp = tamp->next;
    }
    newamp = (AsnModulePtr) MemNew(numamp * sizeof(AsnModule));
    anp->data.ptrvalue = (Pointer) newamp;
    tamp = amp;
    tamp2 = newamp;
    while (numamp)
    {
        MemCopy(tamp2, tamp, sizeof(AsnModule));
        tamp2++; tamp++; numamp--;
    }
    if (last != NULL)
    {
        tamp = (AsnModulePtr)last->data.ptrvalue;
        while (tamp->next != NULL)
            tamp = tamp->next;
        tamp->next = newamp;    /* link it in to list copy */
    }

    tamp = (AsnModulePtr) amps->data.ptrvalue;
    AsnModuleLink(tamp);              /* connect the tree */
    return;
}


/*****************************************************************************
*
*   Pointer PointerDecode(index, avn, at)
*
*****************************************************************************/
static Pointer PointerDecode (Int2 index, AsnValxNodePtr avn, AsnTypePtr at)
{
    if (index == -32000)
        return NULL;
    if (index == -32001)
        return (Pointer) avn;
    if (index < 0)
    {
        index *= -1;
        return (Pointer) &avn[index];
    }
    return (Pointer) &at[index];
}

/*****************************************************************************
*
*   AsnTreeLoad(file, &avn, &at, &amp)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL  AsnTreeLoad (char *file, AsnValxNodePtr *avnptr, AsnTypePtr *atptr, AsnModulePtr *ampptr)
{
    AsnValxNodePtr avn = NULL, avncurr;
    AsnTypePtr at = NULL, atcurr;
    AsnModulePtr amp = NULL, ampcurr;
    ValNodePtr anp, ampanp;
    int numval, numtype, nummod, i;
    FILE * fp;
    int isa, next;
    long intvalue;
    double realvalue;
    char name[PATH_MAX], fname[60];
    CharPtr ptr;
    int bools[5], tagclass, tagnumber, defaultvalue, type, branch;

                    /* check to see if we already have it (could happen) */

    anp = names;
    ampanp = amps;
    while (anp != NULL)
    {
        if (! StringCmp((CharPtr)anp->data.ptrvalue, file))
        {                               /* have it */
            if (* ampptr == NULL)       /* fill dynamic load */
            {
                * ampptr = (AsnModulePtr) ampanp->data.ptrvalue;
                * atptr = (* ampptr)->types;
                     /* don't really need avnptr filled in */
            }
            return TRUE;
        }
        ampanp = ampanp->next;
        anp = anp->next;
    }

	if (! ProgMon("Load ASN.1 Trees"))
		return FALSE;

    if (* ampptr != NULL)      /* static load, add to list */
    {
        AsnStoreTree(file, (* ampptr));
        return TRUE;        /* already loaded */
    }

    if (! FindPath("ncbi", "ncbi", "asnload", name, sizeof (name)))
	{
		AsnIoErrorMsg(NULL, 85);
        return FALSE;
	}

    FileBuildPath (name, NULL, file);/* add file to path */

    fp = FileOpen(name, "r");
    if (fp == NULL)
    {
        AsnIoErrorMsg(NULL, 83, (CharPtr) name);
        return FALSE;
    }

    fscanf(fp, "%d %d %d", &numval, &numtype, &nummod);

    if (numval)
        avn = (AsnValxNodePtr) MemNew(numval * sizeof(AsnValxNode));
    at  = (AsnTypePtr)   MemNew(numtype * sizeof(AsnType));
    amp = (AsnModulePtr) MemNew(nummod  * sizeof(AsnModule));

    *avnptr = avn;
    *atptr = at;
    *ampptr = amp;

    avncurr = avn;
    for (i = 0; i < numval; i++)
    {
        fscanf(fp, "%d %s %ld %lf %d", &isa, name, &intvalue, &realvalue, &next);
        avncurr->valueisa = isa;
        if (*name != '-')
            avncurr->name = StringSave(name);
        avncurr->intvalue = (Int4)intvalue;
        avncurr->realvalue = (FloatHi)realvalue;
        avncurr->next = (AsnValxNodePtr) PointerDecode((Int2)next, avn, at);
        avncurr++;
    }

    atcurr = at;
    for (i = 0; i < numtype; i++)
    {
        fscanf(fp, "%d %s %d %d %d %d %d %d %d %d %d %d %d",
            &isa, name, &tagclass, &tagnumber, bools, bools+1, bools+2,
            bools+3, bools+4, &defaultvalue, &type, &branch, &next);
        atcurr->isa = isa;
        if (*name != '-')
        {
            atcurr->name = StringSave(name);
            ptr = atcurr->name;
            while (*ptr != '\0')
            {
                if (*ptr == '!')
                    *ptr = ' ';
                ptr++;
            }
        }
        atcurr->tagclass = (Uint1)tagclass;
        atcurr->tagnumber = tagnumber;
        atcurr->implicit = (Boolean)bools[0];
        atcurr->optional = (Boolean)bools[1];
        atcurr->hasdefault = (Boolean)bools[2];
        atcurr->exported = (Boolean)bools[3];
        atcurr->imported = (Boolean)bools[4];
        atcurr->defaultvalue = (AsnValxNodePtr)PointerDecode((Int2)defaultvalue, avn, at);
        atcurr->type = (AsnTypePtr)PointerDecode((Int2)type, avn, at);
        atcurr->branch = PointerDecode((Int2)branch, avn, at);
        atcurr->next = (AsnTypePtr)PointerDecode((Int2)next, avn,at);
        atcurr++;
    }

    ampcurr = amp;
    for (i = 0; i < nummod; i++)
    {
        fscanf(fp, "%s %s %d %d", name, fname, &type, &next);
        if (*name != '-')
            ampcurr->modulename = StringSave(name);
		if (*fname != '-')
			ampcurr->filename = StringSave(fname);
        ampcurr->types = (AsnTypePtr)PointerDecode((Int2)type, avn, at);
        if (next != -32000)
            ampcurr->next = &amp[next];
        ampcurr++;
    }

    FileClose(fp);
                                  /* store it in list */
    AsnStoreTree(file, (* ampptr));

    return TRUE;
}

/*****************************************************************************
*
*   AsnEnumStr(type, val)
*
*****************************************************************************/
NLM_EXTERN CharPtr LIBCALL  AsnEnumStr (CharPtr type, Int2 val)
{
    AsnTypePtr atp;

	if (amps == NULL)
	{
		AsnIoErrorMsg(NULL, (Int2)105);
		return NULL;
	}
    atp = AsnTypeFind((AsnModulePtr)amps->data.ptrvalue, type);
    return AsnEnumTypeStr(atp, val);
}

/*****************************************************************************
*
*   AsnEnumTypeStr(atp, val)
*       returns string for ennumerated type val or a named integer
*
*****************************************************************************/
NLM_EXTERN CharPtr LIBCALL  AsnEnumTypeStr (AsnTypePtr atp, Int2 val)
{
    AsnValxNodePtr avnp;

    atp = AsnFindBaseType(atp);
    if (atp == NULL)
        return NULL;

    if ((atp->type->isa != ENUM_TYPE) && (atp->type->isa != INTEGER_TYPE))
        return NULL;

	avnp = (AsnValxNodePtr) atp->branch;
	while (avnp != NULL)
	{
		if (val == (Int2) avnp->intvalue)
            return avnp->name;
		else
			avnp = avnp->next;
	}
    return NULL;
}

/*****************************************************************************
*
*   AsnAllModPtr()
*      returns a pointer to the start of all modules in memory
*
*****************************************************************************/
NLM_EXTERN AsnModulePtr LIBCALL  AsnAllModPtr (void)
{
    if (amps == NULL)
        return NULL;
    return (AsnModulePtr)amps->data.ptrvalue;
}



