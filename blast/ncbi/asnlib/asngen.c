/*  asngen.c
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
* File Name: asngen.c
*
* Author:  James Ostell
*
* Version Creation Date: 3/4/91
*
* $Revision: 6.3 $
*
* File Description:
*   General application interface routines for ASNlib
*      using these has the advantage that your routine will read binary
*        and print value messages with no changes
*      it has the disadvantage that you link both binary and text routines
*        whether you use them or not
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 3/4/91   Kans        Stricter typecasting for GNU C and C++
* 04-20-93 Schuler     LIBCALL calling convention
*
* $Log: asngen.c,v $
* Revision 6.3  2003/09/15 16:16:33  kans
* added AsnWriteEx, AsnTxtWriteEx, and AsnPrintStream
*
* Revision 6.2  1997/10/29 02:40:40  vakatov
* Type castings to pass through the C++ compiler
*
* Revision 6.1  1997/10/28 15:13:00  epstein
*  add AsnFindNthPieceOfObject
*
* Revision 6.0  1997/08/25 18:09:55  madden
* Revision changed to 6.0
*
* Revision 5.1  1996/12/03 21:43:48  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
 * Revision 5.0  1996/05/28  14:00:29  ostell
 * Set to revision 5.0
 *
 * Revision 4.1  1996/01/03  22:59:49  ostell
 * added ExpOptCheck on AsnReadVal()
 *
 * Revision 4.0  1995/07/26  13:47:38  ostell
 * force revision to 4.0
 *
 * Revision 2.8  1995/05/15  18:38:28  ostell
 * added Log line
 *
*
* ==========================================================================
*/

#include "asnbuild.h"

/*****************************************************************************
*
*   AsnReadId()
*   	generalized read ident
*
*****************************************************************************/
NLM_EXTERN AsnTypePtr LIBCALL  AsnReadId (AsnIoPtr aip, AsnModulePtr amp, AsnTypePtr atp)

{
	if (aip->read_id)
		AsnIoErrorMsg(aip, 104, aip->linenumber);
	aip->read_id = TRUE;
	
    if (aip->type & ASNIO_TEXT)
		return AsnTxtReadId(aip, amp, atp);
    else if (aip->type & ASNIO_BIN)
		return AsnBinReadId(aip, atp);

	return NULL;
}

/*****************************************************************************
*
*   AsnReadVal()
*   	generalized readvalue
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL AsnReadVal (AsnIoPtr aip, AsnTypePtr atp, DataValPtr vp)

{
	Int2 retval = -1;

	if (! aip->read_id)
		AsnIoErrorMsg(aip, 103, aip->linenumber);
	aip->read_id = FALSE;
	
	if (aip->type & ASNIO_TEXT)
		retval = AsnTxtReadVal(aip, atp, vp);
	else if (aip->type & ASNIO_BIN)
		retval = AsnBinReadVal(aip, atp, vp);

	if (aip->aeop != NULL)    /* exploring nodes */
		AsnCheckExpOpt(aip, atp, vp);

	return retval;
}

/*****************************************************************************
*
*   AsnWrite()
*   	generalized write value
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL AsnWrite (AsnIoPtr aip, AsnTypePtr atp, DataValPtr dvp)

{
	Boolean retval = FALSE;
	
	if (aip->aeop != NULL)    /* exploring nodes */
		AsnCheckExpOpt(aip, atp, dvp);

	if (aip->type & ASNIO_TEXT)
		retval = AsnTxtWrite(aip, atp, dvp);
	else if (aip->type & ASNIO_BIN)
		retval = AsnBinWrite(aip, atp, dvp);

	if (aip->io_failure)
		return FALSE;
	return retval;
}

/*****************************************************************************
*
*   AsnWriteEx()
*   	specialized write value
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL AsnWriteEx (AsnIoPtr aip, AsnTypePtr atp, DataValPtr dvp, AsnStreamStringFunc stream)

{
	Boolean retval = FALSE;

	if (aip->aeop != NULL)
		AsnCheckExpOpt(aip, atp, dvp);

	if (aip->type & ASNIO_TEXT)
		retval = AsnTxtWriteEx(aip, atp, dvp, stream);
	else if (aip->type & ASNIO_BIN)
		return FALSE;

	if (aip->io_failure)
		return FALSE;
	return retval;
}

/*****************************************************************************
*
*   AsnSkipValue(aip, atp)
*   	Assumes that a read id has just been done
*   	Assumes all values to be encountered are linked to atp
*   	will skip values until it returns to the former level
*   	returns TRUE if no errors
*       ends with a ReadVal()
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL AsnSkipValue (AsnIoPtr aip, AsnTypePtr atp)

{
	Int2 level;

	level = AsnGetLevel(aip);
	if (! AsnReadVal(aip, atp, NULL))
		return FALSE;

	while (level < AsnGetLevel(aip))
	{
		if ((atp = AsnReadId(aip, NULL, atp)) == NULL)
			return FALSE;
		if (! AsnReadVal(aip, atp, NULL))
			return FALSE;
	}
	return TRUE;
}

/*****************************************************************************
*
*   AsnOpenStruct(aip, atp, ptr)
*      write atp as a START_STRUCT
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL AsnOpenStruct (AsnIoPtr aip, AsnTypePtr atp, Pointer ptr)

{
	DataVal av;
	Boolean retval;

	av.intvalue = START_STRUCT;
	if(aip->aeosp != NULL)
		aip->aeosp->the_struct = ptr;
	retval = AsnWrite(aip, atp, &av);
	if(aip->aeosp != NULL)
		aip->aeosp->the_struct = NULL;
	return retval;
}

/*****************************************************************************
*
*   AsnCloseStruct(aip, atp, ptr)
*      writes an END_STRUCT for atp
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL AsnCloseStruct (AsnIoPtr aip, AsnTypePtr atp, Pointer ptr)

{
	DataVal av;
    Int2 i;
	Boolean retval;

    i = aip->type_indent;
    if (! i)
	{
        AsnIoErrorMsg(aip, 20 );
		return FALSE;
	}

    i--;
    while ((i) &&
        (AsnFindBaseIsa(aip->typestack[i].type) == CHOICE_TYPE))
        i--;
    if (aip->typestack[i].type != atp)
	{
        AsnIoErrorMsg(aip, 21, AsnErrGetTypeName(atp->name),
			AsnErrGetTypeName(aip->typestack[i].type->name));
		return FALSE;
	}

	av.intvalue = END_STRUCT;
	if(aip->aeosp != NULL)
		aip->aeosp->the_struct = ptr;
	retval = AsnWrite(aip, atp, &av);
	if(aip->aeosp != NULL)
		aip->aeosp->the_struct = NULL;
	return retval;
}

/*****************************************************************************
*
*   AsnWriteChoice(aip, atp, the_choice, the_value)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL AsnWriteChoice (AsnIoPtr aip, AsnTypePtr atp, Int2 the_choice, DataValPtr the_value)

{
	Boolean retval;
	DataVal av;

	if (the_value == NULL)
	{
		av.ptrvalue = NULL;
		the_value = &av;
	}
	if(aip->aeosp != NULL)
		aip->aeosp->the_choice = the_choice;
	retval = AsnWrite(aip, atp, the_value);
	if(aip->aeosp != NULL)
		aip->aeosp->the_choice = NO_CHOICE_SET;
	return retval;
}

/*****************************************************************************
*
*   Int2 AsnGetLevel(aip)
*       returns current level in parse stack
*       to traverse a struct (SEQUENCE, SET, SET OF, SEQUENCE OF)
*       atp = AsnReadId(aip, ...)    id for struct
*       level = AsnGetLevel(aip);    record level
*       AsnReadVal(aip, ...)         read open bracket to indent a level
*       while (AsnGetLevel(aip) != level)
*       {
*           AsnReadId()
*           AsnReadVal()
*       }
*       At this point you have read the close bracket for the struct
*   
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL AsnGetLevel (AsnIoPtr aip)

{
    return (Int2)aip->type_indent;
}

/*****************************************************************************
*
*   AsnCheckExpOpt(aip, atp, dvp)
*   	see if atp is a node we are looking for
*       call user callback if it is
*
*****************************************************************************/
NLM_EXTERN void LIBCALL AsnCheckExpOpt (AsnIoPtr aip, AsnTypePtr atp, DataValPtr dvp)

{
	AsnExpOptPtr aeop;
	AsnTypePtr base;
	Boolean got_one;
	Int2 i, j, isa, isa_end_struct;

	aeop = aip->aeop;
	base = AsnFindBaseType(atp);
	while (aeop != NULL)
	{
		got_one = FALSE;
		if (aeop->numtypes == 1)    /* checking one type */
		{
			if ((aeop->types[0] == atp) ||
				(aeop->types[0] == base))
			{
				got_one = TRUE;
			}
		}
		else if (aeop->numtypes == 0)   /* checking all types */
			got_one = TRUE;
		else
		{
		  	if (aeop->types[aeop->numtypes - 1] == atp)
			{
				isa = base->type->isa;

				isa_end_struct = 0;
				if ((isa == SEQ_TYPE) || (isa == SET_TYPE) ||
					 (isa == SEQOF_TYPE) || (isa == SETOF_TYPE))
				{
					 if (dvp->intvalue != START_STRUCT)
						isa_end_struct = 1;
				}

				if ((Int2)aip->type_indent >= (aeop->numtypes - 1 - isa_end_struct))
				{
					if (isa_end_struct)
						i = aip->type_indent - 2;
					else
		 				i = aip->type_indent - 1;
		 			j = aeop->numtypes - 2;
					while ((j > 0) && (i > 0))
					{
						if (aip->typestack[i].type == aeop->types[j])
						{
							j--; i--;
						}
						else
							break;
					}
					if (! j)    /* got through it all */
					{
						if ((aip->typestack[i].type == aeop->types[0]) ||
							(AsnFindBaseType(aip->typestack[i].type) ==
								aeop->types[0]))
								got_one = TRUE;
					}
				}
			}
		}

		if (got_one)   /* found it */
		{
			aip->aeosp->atp = atp;
			aip->aeosp->dvp = dvp;
			aip->aeosp->data = aeop->user_data;
			(* aeop->user_callback)(aip->aeosp);
		}
		aeop = aeop->next;
	}
	return;
}

/*****************************************************************************
*
*   AsnExpOptNew(aip, path, ptr, func)
*
*****************************************************************************/
NLM_EXTERN AsnExpOptPtr LIBCALL AsnExpOptNew (AsnIoPtr aip, CharPtr path, Pointer ptr, AsnExpOptFunc func)   /* user supplied callback */

{
	AsnExpOptPtr curr, prev;
	AsnTypePtr PNTR typeptr = NULL;
	Int2 typecount = 0;

	if (path != NULL)     /* get type array */
	{
		typeptr = AsnTypePathFind(NULL, path, &typecount);
		if (typeptr == NULL)    /* path not found */
			return NULL;
	}
	curr = (AsnExpOptPtr) MemNew(sizeof(AsnExpOpt));
	curr->user_data = ptr;
	curr->user_callback = func;
	curr->types = typeptr;
	curr->numtypes = typecount;
	if (aip->aeop == NULL)   /* first one */
	{
		aip->aeop = curr;
		aip->aeosp = (AsnExpOptStructPtr) MemNew(sizeof(AsnExpOptStruct));
		aip->aeosp->aip = aip;
		aip->aeosp->the_choice = NO_CHOICE_SET;
	}
	else
	{
		prev = aip->aeop;
		while (prev->next != NULL)
			prev = prev->next;
		prev->next = curr;
	}
	return curr;
}

/*****************************************************************************
*
*   AsnExpOptFree(aip, aeop)
*   	if aeop == NULL, free them all
*
*****************************************************************************/
NLM_EXTERN AsnExpOptPtr LIBCALL AsnExpOptFree (AsnIoPtr aip, AsnExpOptPtr aeop)

{
	AsnExpOptPtr curr, next, prev;

	curr = aip->aeop;
	prev = NULL;
	while (curr != NULL)
	{
		if ((aeop == NULL) || (aeop == curr))
		{
			next = curr->next;
			if (prev != NULL)
				prev->next = next;
			else
				aip->aeop = next;
			MemFree(curr->types);
			MemFree(curr);
			curr = next;
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
	if (prev == NULL)    /* nothing left */
		aip->aeosp = (AsnExpOptStructPtr) MemFree(aip->aeosp);   /* free the AsnExpOptStruct */
	return NULL;
}

/*****************************************************************************
*
*   AsnNullValueMsg(aip, node)
*   	posts an error message when a NULL value is passed to the object
*   loaders for node.
*
*****************************************************************************/
NLM_EXTERN void LIBCALL AsnNullValueMsg (AsnIoPtr aip, AsnTypePtr node)

{
	char * tmpbuf;
	CharPtr str;

	tmpbuf = (char*) malloc(512);   /* temp. local buffer for mswindows */
	str = StringMove(tmpbuf, "NULL value passed to object loader for ");
	str = AsnTypeDumpStack(str, aip);
	if (node != NULL)
	{
		if (*(str - 1) == '>')    /* already read or written */
		{
			while (*str != '<')
				str--;
		}
		else
		{		
			*str = '.';
			str++;
		}
		StringMove(str, node->name);
	}
	ErrPost(CTX_NCBIOBJ, 1, tmpbuf);
	free(tmpbuf);
	return;
}



typedef struct {
    Int4 count;
    Pointer val;
} StructInfo;

static void LIBCALLBACK GetNth(AsnExpOptStructPtr aeosp)
{
	StructInfo *sp;

        if (aeosp->dvp->intvalue != START_STRUCT) return;
	sp = (StructInfo *) aeosp->data;
	if (sp->count-- == 1)
	    sp->val = (Pointer) aeosp->the_struct;
}

/* Finds the Nth occurrence of any low-level ASN.1 object within a higher
   level object for which an ASN.1 Write function exists.  The string
   parameter must identify the low-level object for which the search is
   being performed.

   Note that this function can be used to obtain pieces of an object in an
   object-oriented "method", rather than using the native method of
   traversing pointers to fetch the desired piece.  However, note that
   this technique is very inefficient since the entire structure must be
   traversed to obtain each component. */
NLM_EXTERN VoidPtr LIBCALL
AsnFindNthPieceOfObject(AsnWriteFunc wfunc, Pointer datum, CharPtr string, Int4 n)
{
  AsnIoPtr aip;
  StructInfo si;
  AsnExpOptPtr aeop;

  aip = AsnIoNullOpen();
  si.count = n;
  si.val = NULL;
  aeop = AsnExpOptNew(aip, string, (Pointer)&si, GetNth);
  wfunc(datum, aip, NULL);
  AsnIoClose(aip);
  return si.val;
}

