/*  prtutil.c
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
* File Name:  prtutil.c
*
* Author:  James Ostell
*   
* Version Creation Date: 4/1/93
*
* $Revision: 6.3 $
*
* File Description:  Object Print Utilities
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 04-24-94 Kans        Template sought first in program, then DATA directory
*
* $Log: prtutil.c,v $
* Revision 6.3  2004/02/02 18:09:34  kans
* typo in PrintTemplateSetLoadEx
*
* Revision 6.2  2004/02/02 17:30:21  kans
* PrintTemplateSetLoadEx takes a local string, used to make internal copy of objprt.prt in Sequin
*
* Revision 6.1  1998/12/29 19:55:53  kans
* more informative error message if FindPath fails
*
* Revision 6.0  1997/08/25 18:06:54  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/06/19 18:38:35  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.0  1996/05/28 13:23:23  ostell
* Set to revision 5.0
*
 * Revision 4.3  1996/02/29  15:37:16  epstein
 * silence SEV_INFO errors from FileOpen()
 *
 * Revision 4.2  1996/02/26  00:46:18  ostell
 * removed unused local variables and integer size mismatch fusses
 *
 * Revision 4.1  1995/10/30  18:39:00  kans
 * changed true and false fields to truepfb and falsepfb (CodeWarrior error)
 *
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 1.15  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#include <prtutil.h>

/******** these are here temporarily... see bottom of file ******/

#include <seqport.h>

NLM_EXTERN CharPtr PersonIdPrint PROTO((PersonIdPtr pip, CharPtr buf));

NLM_EXTERN void LIBCALLBACK PrintStackAddItem(AsnExpOptStructPtr aeosp); /* for building stack */
/*****************************************************************************
*
*   PrintTemplateFind(name)
*   	Just checks in object loaded memory
*
*****************************************************************************/
NLM_EXTERN PrintTemplatePtr PrintTemplateFind (CharPtr name)
{
    Int2 i, PTnum;
	PrintTemplatePtr PNTR PTlist;

	if (name == NULL)
		return NULL;

	PTlist = PrintTemplateInMem(&PTnum);
    for (i = 0; i < PTnum; i++)
    {
    	if (PTlist[i] != NULL) {
        	if (! StringCmp(PTlist[i]->name, name))
       	     return PTlist[i];
        }
    }
    return NULL;
}

/*****************************************************************************
*
*   PrintTemplateSetPtr PrintTemplateSetLoad()
*       loads PrintTemplates
*       If path== NULL, looks for "prt.prt" in the "data" directory
*
*****************************************************************************/
static Boolean PrintTemplateSetLoadInt ( CharPtr path, Boolean verbose )
{
	AsnIoPtr aip;
	FILE *f;
	Boolean retval;
	Char filename [80];
	Char fullpath [PATH_MAX];
	CharPtr ptr;
	ErrSev lastLevel;

	aip = NULL;
	if (path != NULL && *path != '\0' && StringRChr (path, DIRDELIMCHR) != NULL) {
		StringNCpy (fullpath, path, sizeof (fullpath) - 1);
		if ((aip = AsnIoOpen (fullpath, "r")) == NULL) {
		    if (verbose) {
	    		ErrPost(CTX_NCBIOBJ, 1, "Couldn't open [%s]", fullpath);
		    }
			return FALSE;
		}
	} else {
		if (path != NULL && *path != '\0') {
			StringNCpy (filename, path, sizeof (filename) - 1);
		} else {
			StringCpy (filename, "prt.prt");
		}
		ProgramPath (fullpath, sizeof (fullpath) - 1);
		ptr = StringRChr (fullpath, DIRDELIMCHR);
		if (ptr != NULL) {
			*ptr = '\0';
		}
		FileBuildPath (fullpath, NULL, filename);
		/* silence SEV_INFO errors from FileOpen() */
		if ((lastLevel = ErrGetMessageLevel()) < SEV_WARNING)
			ErrSetMessageLevel (SEV_WARNING);
		if ((f = FileOpen (fullpath, "r")) == NULL) {
			ErrSetMessageLevel (lastLevel);
			if (FindPath ("NCBI", "NCBI", "DATA", fullpath, sizeof (fullpath) - 1)) {
				FileBuildPath (fullpath, NULL, filename);
				if ((aip = AsnIoOpen (fullpath, "r")) == NULL) {
				    if (verbose) {
					    ErrPost(CTX_NCBIOBJ, 1, "Couldn't open [%s]", fullpath);
				    }
					return FALSE;
				}
			} else {
			    if (verbose) {
			    	ErrPost(CTX_NCBIOBJ, 1, "FindPath failed in PrintTemplateSetLoad - ncbi configuration file missing or incorrect");
			    }
				return FALSE;
			}
		} else {
			ErrSetMessageLevel (lastLevel);
			FileClose (f);
			if ((aip = AsnIoOpen(fullpath, "r")) == NULL) {
			    if (verbose) {
				    ErrPost(CTX_NCBIOBJ, 1, "Couldn't open [%s]", fullpath);
			    }
				return FALSE;
			}
		}
	}

    retval = PrintTemplateSetAsnRead(aip);

    AsnIoClose(aip);
    return retval;
}

static Boolean LoadObjPrtFromLocalString (CharPtr localStr)

{
  Boolean     retval = FALSE;
#ifndef WIN16
  AsnIoMemPtr aimp;

  aimp = AsnIoMemOpen ("r", (BytePtr) localStr, (Int4) StringLen (localStr));
  if (aimp == NULL || aimp->aip == NULL) return FALSE;
  retval = PrintTemplateSetAsnRead (aimp->aip);
  AsnIoMemClose (aimp);
#endif
  return retval;
}

NLM_EXTERN Boolean PrintTemplateSetLoadEx ( CharPtr path, CharPtr localStr )

{
  Boolean  verbose = TRUE;

  if (StringDoesHaveText (localStr)) {
    verbose = FALSE;
  }
  if (PrintTemplateSetLoadInt (path, verbose)) return TRUE;
  if (verbose) return FALSE;
  return LoadObjPrtFromLocalString (localStr);
}

NLM_EXTERN Boolean PrintTemplateSetLoad ( CharPtr path )

{
  return PrintTemplateSetLoadEx (path, NULL);
}

/*****************************************************************************
*
*   PrintStackItemNew(psp)
*   	allocate and return next PrintStackItem from PrintStack
*
*****************************************************************************/
NLM_EXTERN PrintStackItemPtr PrintStackItemNew(PrintStackPtr psp)
{
	PrintStackItemPtr prev=NULL;

	if (psp == NULL) return NULL;

	if (psp->pstackpos >= psp->pstacksize)
	{
		prev = psp->data;
		psp->data = MemNew((size_t)(sizeof(PrintStackItem) * (psp->pstacksize + PRINT_STACK_SIZE)));
		if (prev != NULL)
		{
			MemCopy(psp->data, prev, (size_t)(sizeof(PrintStackItem) * psp->pstacksize));
			MemFree(prev);
		}
		psp->pstacksize += PRINT_STACK_SIZE;
	}

	psp->pstackpos++;
	return psp->data + (psp->pstackpos - 1);
}

/*****************************************************************************
*
*   PrintStackItemGet(psp, index, indent_offset)
*   	return PrintStackItem from PrintStack including any "use_template"
*         branches
*
*****************************************************************************/
NLM_EXTERN PrintStackItemPtr PrintStackItemGet(PrintStackPtr psp, Int2 index, Int2Ptr indent_offset)
{
	Int2 count = 0, i, j, local_offset = 0;
	PrintStackItemPtr psip;
	PrintStackPtr tmp;

	if (psp == NULL) return NULL;

	if (index >= psp->size)
		return NULL;

	if (psp->is_branch)
		j = 1;
	else
		j = 0;

	for (i = j; i < psp->pstackpos; i++)
	{
		psip = psp->data + i;

		if (count == index)
		{
			*indent_offset = psp->indent;
			return psip;
		}

		count++;

		if (psip->pfip->_template != NULL)  /* branch */
		{
			tmp = (PrintStackPtr)(psip->dv.ptrvalue);
			if ((count + tmp->size - 1) > index)  /* in here */
			{
				psip = PrintStackItemGet(tmp, (Int2)(index - count), &local_offset);
				*indent_offset = psp->indent + local_offset;
				return psip;
			}
			else
				count += (tmp->size - 1);
		}

	}

	return NULL;
}


/*****************************************************************************
*
*   PrintStackGetSize(psp)
*   	Gets Size of PrintStack include "use_template" branches
*
*****************************************************************************/
NLM_EXTERN Int2 PrintStackGetSize (PrintStackPtr psp)
{
	Int2 count = 0, i, tmp;
	PrintStackItemPtr psip;

	if (psp == NULL) return 0;

	for (i = 0; i < psp->pstackpos; i++)
	{
		psip = psp->data + i;
		if (psip->pfip->_template != NULL)  /* branch */
		{
			tmp = PrintStackGetSize((PrintStackPtr)(psip->dv.ptrvalue));
			if (tmp)
				count += (tmp - 1);  /* skip redundant first element */
		}
	}

	count += psp->pstackpos;
	psp->size = count;           /* record total size */

	return count;
}


/*****************************************************************************
*
*   PrintStackFree(psp)
*
*****************************************************************************/
NLM_EXTERN PrintStackPtr PrintStackFree(PrintStackPtr psp)
{
	Int2 i;
	PrintStackItemPtr psip;

	if (psp == NULL)
		return NULL;

	for (i = 0; i < psp->pstackpos; i++)
	{
		psip = psp->data + i;
		if (psip->pfip->_template != NULL)
			PrintStackFree((PrintStackPtr)(psip->dv.ptrvalue));
	}
	MemFree(psp->data);
	return MemFree(psp);
}

/*****************************************************************************
*
*   PrintFormatListBuild(ptp)
*
*****************************************************************************/
NLM_EXTERN PrintFormatListPtr PrintFormatListBuild (PrintTemplatePtr ptp)
{
	PrintFormatListPtr pflp;
	Int2 count = 0;

	if (ptp == NULL) return NULL;

	count = PrintFormatTraverse(ptp->format, NULL, 0, 0);

	if (! count) return NULL;

	pflp = PrintFormatListNew();
	pflp->list = MemNew((size_t)(count * sizeof(PrintFormatItem)));
	pflp->numitem = count;
	pflp->ptp = ptp;

	count = PrintFormatTraverse(ptp->format, pflp, 0, 0);

	return pflp;
}

NLM_EXTERN Int2 PrintFormatListGetMaxKey PROTO((PrintFormatListPtr pflp, Int2 start_key));

/*****************************************************************************
*
*   PrintFormatListGetMaxKey(pflp, start_key)
*   	returns a value 1 higher than the highest sort_key in the (possible)
*   	indent level below that of start_key
*   	returns -1 for maximum
*
*****************************************************************************/
NLM_EXTERN Int2 PrintFormatListGetMaxKey(PrintFormatListPtr pflp, Int2 start_key)
{
	Int2 max_key,indent, num;
	PrintFormatItemPtr pfip;

	max_key = start_key;
	if (pflp == NULL) return -1;
	if (max_key >= pflp->numitem) return -1;

	pfip = &(pflp->list[max_key]);
	indent = pfip->indent_level;
	num = pflp->numitem;

	do
	{
		max_key++;
		if (max_key >= num)
			return max_key;
		pfip++;
	} while (pfip->indent_level > indent);

	return max_key;
}

/*****************************************************************************
*
*   PrintFormatTraverse(pfp, pflp, index, indent)
*
*****************************************************************************/
NLM_EXTERN Int2 PrintFormatTraverse(PrintFormatPtr pfp, PrintFormatListPtr pflp, Int2 index, Int2 indent)
{
	PrintFormatItemPtr pfip;
	CharPtr template_format;
	PrintTemplatePtr usetemplate;
	Int2 count = 0, retcount;
	PrintFormBlockPtr pfbp;
	AsnTypePtr atp;
	AsnTypePtr PNTR typeptr = NULL;
	Int2 typecount = 0;

	while (pfp != NULL)
	{
		usetemplate = NULL;
		if (pfp->form->choice == PrintForm_use_template)
		{
			template_format = (CharPtr)(pfp->form->data.ptrvalue);
			usetemplate = PrintTemplateFind(template_format);
			if (usetemplate == NULL)
			{
				ErrPost(CTX_OBJPRT, 1, "..couldn't find template [%s]\n", template_format);
				return 0;
			}
			
		}
		if (pflp != NULL)
		{
			pfip = &(pflp->list[index]);
			pfip->sort_key = index;
			pfip->indent_level = indent;
			pfip->format = pfp;
			atp = AsnFind(pfp->asn1);   /* get the node */
			if (atp == NULL)
			{
				ErrPost(CTX_OBJPRT, 1, "PrintFormatTraverse: can't find [%s]", pfp->asn1);
				return 0;
			}
			pfip->atp = atp;
			pfip->isa = AsnFindBaseIsa(atp);
			
			pfip->_template = usetemplate;
			typeptr = AsnTypePathFind(NULL, pfp->asn1, &typecount);
			if (typeptr == NULL)
				return 0;
			pfip->types = typeptr;
			pfip->numtypes = typecount;

		}
		index++;
		count++;
		if (pfp->form->choice == PrintForm_block)
		{
			pfbp = (PrintFormBlockPtr)(pfp->form->data.ptrvalue);
			retcount = PrintFormatTraverse(pfbp->components, pflp, index, (Int2)(indent+1));

			if (retcount == 0)  /* failure */
				return 0;
			count += retcount;
			index += retcount;
		}
		pfp = pfp->next;
	}
	return count;
}

/*****************************************************************************
*
*   PrintStackAddItem()
*   	AsnExpOpt callback to add items to print stack
*
*****************************************************************************/
NLM_EXTERN void LIBCALLBACK PrintStackAddItem(AsnExpOptStructPtr aeosp)
{
	PrintFormatItemPtr pfip;
	PrintFormatListPtr pflp, pflp1;
	PrintStackPtr curr, tmp;
	PrintStackItemPtr psip;
	Int2 type_indent, i, j, k, l, start_search;
	PstackPtr pstack;
	AsnTypePtr atp, base;
	Boolean got_one;
	PrintStackListPtr pslp;
	PrintStackListItemPtr pstip;
	AsnTypePtr PNTR types;
	


	type_indent = aeosp->aip->type_indent;
	pstack = aeosp->aip->typestack;
	
	pslp = (PrintStackListPtr)(aeosp->data);
	pstip = pslp->pstip;

	if (pslp->used > 1)
	{
		for (i = 0; i < pslp->used; i++)
		{
			if ((pstip+i)->type_indent >= type_indent)
			{
				pslp->used = i;
				break;
			}
		}
	}

	start_search = pslp->used-1;
	k = (pstip + start_search)->type_indent - 1;
	while (start_search > 0)
	{
		if ((pstip + (start_search - 1))->type_indent < k)
			break;
		start_search--;
	}

	if (pslp->used >= pslp->size)  /* increase array size */
	{
		pslp->pstip = MemNew((size_t)(sizeof(PrintStackListItem) * (pslp->size + 10)));
		MemCopy(pslp->pstip, pstip, (size_t)(sizeof(PrintStackListItem)* pslp->size));
		pslp->size += 10;
		MemFree(pstip);
		pstip = pslp->pstip;
	}

	if (aeosp->the_struct != NULL)   /* it's a struct */
	{
		if (aeosp->dvp->intvalue != START_STRUCT)
			return;
	}

	atp = aeosp->atp;
	base = AsnFindBaseType(atp);


	for (l = start_search; l < pslp->used; l++)
	{
		curr = (pstip + l)->psp;
		pflp = curr->pflp;
		for (i = 0; i < pflp->numitem; i++)
		{
			got_one = FALSE;
			pfip = pflp->list + i;
			j = pfip->numtypes - 1;

			if (! j)
			{
				if (pfip->atp == atp)
					got_one = TRUE;
				else if (pfip->atp == base)
				{                       /* guard against recursion */
					if (curr->is_branch) /* not top level */
					{
						types = curr->branch->types;
						j = curr->branch->numtypes - 1;
					}
				}
			}
			else
				types = pfip->types;

			if ((j) && (types[j] == atp) && (type_indent >= j))
			{
			 	k = type_indent - 1;
				j--;
				while ((j > 0) && (k > 0))
				{
					if (pstack[k].type == types[j])
					{
						j--; k--;
					}
					else
						break;
				}
				if (! j)   /* got all the way through */
				{
					if ((pstack[k].type == types[0]) ||
						(AsnFindBaseType(pstack[k].type) == types[0]))
						got_one = TRUE;
				}
			}

			if (got_one)
			{
				psip = PrintStackItemNew(curr);
				psip->pfip = pfip;
				if (aeosp->the_struct != NULL)
					psip->dv.ptrvalue = aeosp->the_struct;
				else
					psip->dv = *(aeosp->dvp);
				if (pfip->_template != NULL)  /* use template */
				{
					pflp1 = PrintFormatListGet(pfip->_template->name);
					if (pflp1 == NULL)
					{
						ErrPost(CTX_OBJPRT, 1, "Couldn't get format list [%s]",
							pfip->_template->name);
						psip->dv.ptrvalue = NULL;
					}
					else
					{
						tmp = MemNew(sizeof(PrintStack));
						tmp->indent = pfip->indent_level;
						tmp->pflp = pflp1;
						tmp->is_branch = TRUE;
						tmp->branch = pfip;

						psip->dv.ptrvalue = tmp;

						for (j = l+1; j < pslp->used; j++)
						{
							if ((pstip+j)->type_indent == type_indent)
							{
								if (((pstip+j)->psp->indent == tmp->indent) &&
									((pstip+j)->psp->pflp == tmp->pflp))
								{
									(pstip + j)->psp = tmp;
									tmp = NULL;
								}
							}
						}
						if (tmp != NULL)  /* not a duplicate */
						{
							(pstip + pslp->used)->psp = tmp;
							(pstip + pslp->used)->type_indent = type_indent;
							pslp->used++;
						}
					}
				}
			}

		}
	}
	

	return;
}

/*****************************************************************************
*
*   PrintStackBuild(data, func, sflp)
*
*****************************************************************************/
NLM_EXTERN PrintStackPtr PrintStackBuild (Pointer data, AsnWriteFunc func, PrintFormatListPtr pflp)
{
	PrintStackPtr psp;
	AsnIoPtr aip;
	Boolean retval;
	PrintStackListPtr pslp;
	PrintStackListItemPtr pstip;

	if ((data == NULL) || (func == NULL) || (pflp == NULL))
		return NULL;

	aip = AsnIoNullOpen();
	
	pslp = MemNew(sizeof(PrintStackList));
	pstip = MemNew((size_t)(sizeof(PrintStackListItem) * 10));
	pslp->pstip = pstip;
	pslp->size = 10;
	pslp->used = 1;
	psp = MemNew(sizeof(PrintStack));    /* put the PrintStack on aip */
	psp->pflp = pflp;
	pstip->psp = psp;
	AsnExpOptNew(aip, NULL, (Pointer)pslp, PrintStackAddItem);

	retval = (*func)(data, aip, NULL);
	AsnIoClose(aip);
	MemFree(pslp->pstip);
	MemFree(pslp);

	PrintStackGetSize(psp);	  /* set agregate sizes including branches */
	if (! retval)
	{
		Message(MSG_ERROR, "PrintStackBuild: writefunc failed");
	}
	return psp;
}

static void PrintStackSortNextItem PROTO((PrintStackPtr dest, PrintStackPtr src, Int2 index, Int2 indent));
static Int2 PrintStackGetLowest PROTO((PrintStackPtr psp, Int2 index, Int2 indent, Int2 current, Int2 max_key, Int2 min_key));
/*****************************************************************************
*
*   PrintStackSort (psp)
*   	This is a destructive sort. Could be better.
*
*****************************************************************************/
NLM_EXTERN Boolean PrintStackSort (PrintStackPtr psp)
{
	PrintStackPtr tmp;
	PrintStackItemPtr psip;
	Int2 index;
	

	if (psp == NULL) return FALSE;
	if (psp->pstackpos <= 0) return FALSE;  /* nothing to do */

	for (index = 0; index < psp->pstackpos; index++) /* sort subtrees */
	{
		psip = psp->data + index;
		if (psip->pfip->_template != NULL) /* branch */
			PrintStackSort((PrintStackPtr)(psip->dv.ptrvalue));
	}

	tmp = MemNew(sizeof(PrintStack));
	index = PrintStackGetLowest(psp, 0, 0, -1, -1, -1);

	while (index >= 0)  /* sort the items into new pstack */
	{
		PrintStackSortNextItem(tmp, psp, 0, 0);
		index = PrintStackGetLowest(psp, 0, 0, index, -1, -1);
	}
	
	psip = psp->data;
	psp->data = tmp->data;
	tmp->data = psip;

	index = psp->pstacksize;
	psp->pstacksize = tmp->pstacksize;
	tmp->pstacksize = index;

	index = psp->pstackpos;
	psp->pstackpos = tmp->pstackpos;
	tmp->pstackpos = index;

	MemFree(tmp->data);   /* get rid of unsorted version */
	MemFree(tmp);
			
	return TRUE;
}

/*****************************************************************************
*
*   PrintStackSortNextItem(dest, src, index, indent, next)
*   	Copies src[index] item to next available slot in dest
*   	Also copies sorted list of any subcomponents
*   	returns index to next item (based on sort_key) at this indent level
*   	returns -1 when no more items
*
*****************************************************************************/
static void PrintStackSortNextItem (PrintStackPtr dest, PrintStackPtr src,
	Int2 index,	Int2 indent)
{
	Int2 tmp, max_key, min_key;
	PrintStackItemPtr from, to;


	if ((dest == NULL) || (src == NULL)) return;

	to = PrintStackItemNew(dest);                  /* copy the one */
	from = src->data + index;
	MemCopy(to, from, sizeof(PrintStackItem));
	min_key = from->pfip->sort_key;
	max_key = PrintFormatListGetMaxKey(src->pflp, min_key);

									/* any components to copy? */
	tmp = PrintStackGetLowest(src, (Int2)(index + 1), (Int2)(indent + 1), index, max_key, min_key);
	while (tmp >= 0)   /* copy/sort any components */
	{
		PrintStackSortNextItem(dest, src, tmp, (Int2)(indent + 1));
		tmp = PrintStackGetLowest(src, (Int2)(index + 1), (Int2)(indent+1), tmp, max_key, min_key);
	}

	return;
}

/*****************************************************************************
*
*   PrintStackGetLowest(psp, index, indent, current)
*   	gets the next item with:
*   		item->sort_key >= sort_key
*   		item->indent == indent
*   		item[index] >= index
*   		and with the next lowest sort_key at this indent level
*   	returns -1 if no more items found
*
*****************************************************************************/
static Int2 PrintStackGetLowest(PrintStackPtr psp, Int2 index, Int2 indent, Int2 current, Int2 max_key, Int2 min_key)
{
	PrintStackItemPtr psip;
	Int2 tmp, lownum, tmpkey, tmpindent, sort_key;


	if (max_key < 0)
		lownum = INT2_MAX;
	else
		lownum = max_key;
	tmp = index;
	index = -1;
	if ((psp == NULL) || (tmp < 0)) return index;

	if (current == -1)   /* no current entry */
		sort_key = -1;
	else                 /* get current sort_key */
	{
		psip = psp->data + current;
		sort_key = psip->pfip->sort_key;
	}

	while (tmp < psp->pstackpos)
	{
		if (tmp != current)     /* don't take same one */
		{
			psip =	psp->data + tmp;
			tmpindent = psip->pfip->indent_level;
			tmpkey = psip->pfip->sort_key;

			if (tmpindent < indent)  /* higher level */
			{
				if (tmpkey == min_key)  /* out of scope for level */
					return index;
			}
			else if (tmpindent == indent) /* right level */
			{
				if ((tmpkey >= sort_key) && (tmpkey < lownum))
				{
					if (tmp < current)  /* been through before */
					{
						if (tmpkey > sort_key)	/* only take later ones */
						{
							lownum = tmpkey;
							index = tmp;
						}
					}
					else				/* new ground, take anything */
					{
						lownum = tmpkey;
						index = tmp;
					}
				}
			}
		}
		tmp++;
	}

	return index;
}

NLM_EXTERN Boolean PrintStringConvert PROTO((StdPrintOptionsPtr spop, CharPtr str, Boolean had_newline, Int2 indent, Boolean tilde));
/*****************************************************************************
*
*   PrintStackPrint(psp, fp)
*
*****************************************************************************/
NLM_EXTERN void PrintStackPrint(PrintStackPtr psp, StdPrintOptionsPtr spop)
{
	Int2 i, indent, oldindent, j, indents[20], labelused = -1, indent_offset,
		curr_indent;
	CharPtr label = NULL, tmp, separator=NULL, prefix=NULL, suffix=NULL,
		data=NULL, separators[20];
	PrintStackItemPtr psip, nxtpsip;
	PrintFormatItemPtr pfip, pfstack[20];
	Boolean printed, nocomponents, printeds[20], had_newline, firstone,
		firstones[20], freedata;
	PrintFormatPtr format;
	PrintFormPtr form;
	Char localbuf[140];
	ValNodePtr tmpval;
	PrintFormBooleanPtr pfbp;
	PrintFormEnumPtr pfep;
	PrintFormBlockPtr pfkp;
	PrintFormTextPtr pftp;
	UserFormatPtr ufp;
	DataVal datav;
	PrintStackPtr tmppsp;
	
	if (psp == NULL) return;

	if (psp->pflp == NULL || psp->size == 0) return;
/***
	PrintStackDump(psp, fp, 0);
***/	
	if (psp->pflp->ptp->labelfrom != NULL)    /* find referred label */
	{
		tmp = psp->pflp->ptp->labelfrom;

		for (i = 0; i < psp->size; i++)
		{
			psip = PrintStackItemGet(psp, i, &indent_offset);
			if ((psip != NULL) && (! StringCmp(tmp, psip->pfip->format->asn1)))
			{
				pfip = psip->pfip;

				label = pfip->format->label;

				if ((label == NULL) && (pfip->_template != NULL))
					label = pfip->_template->format->label;

				if ((label == NULL) && (psip->pfip->isa == CHOICE_TYPE))
				{							/* not here, maybe inside */
					indent = psip->pfip->indent_level + indent_offset;
					for (i = (i+1); i < psp->size; i++)
					{
						psip = PrintStackItemGet(psp, i, &indent_offset);
						pfip = psip->pfip;
						curr_indent = pfip->indent_level + indent_offset;
						if (curr_indent <= indent) /* end */
							break;
						if (curr_indent == (indent + 1))
						{
							label = pfip->format->label;

							if ((label == NULL) && (pfip->_template != NULL))
								label = pfip->_template->format->label;
						}
						if (label != NULL) break;
						
					}
				}
				break;
			}
		}
	}

	if (label != NULL) labelused = i;

	PrintStringConvert(spop, NULL, FALSE, 0, FALSE);  /* initialize */
	indent = 0;
	oldindent = 0;
	had_newline = FALSE;
	firstone = TRUE;

	    /* get first separator, if any */
	psip = PrintStackItemGet(psp, 0, &indent_offset);
	pfip = psip->pfip;
	if (pfip->_template == NULL)
		form = pfip->format->form;
	else
		form = pfip->_template->format->form;

	if (form->choice == PrintForm_block)
	{
		pfkp = (PrintFormBlockPtr)(form->data.ptrvalue);
		separator = pfkp->separator;
	}


	for (i = 0; i < psp->size; i++)
	{
		printed = FALSE;
		nocomponents = FALSE;
		freedata = FALSE;
		data = NULL;
		prefix = NULL;
		suffix = NULL;

		psip = PrintStackItemGet(psp, i, &indent_offset);
		pfip = psip->pfip;
		curr_indent = pfip->indent_level + indent_offset;

/***
		printf("@\n=====================\n");
		printf("[%d][%s] curr_indent=%d oldindent=%d indent=%d separator=%s\n",
			(int)i, pfip->format->asn1, (int)curr_indent, (int)oldindent,
			(int)indent, separator);

		if (i == 0)
			printf("  [0]\n");
		else
		{
		for (j = 0; j <= oldindent; j++)
			printf("  [%d] indents=%d separators=[%s] printeds=%d firstones=%d\n",
				(int)j, (int)indents[j], separators[j], (int)printeds[j],
				(int) firstones[j]);
		}

		printf("=====================\n");
***/

		
		if (curr_indent >= oldindent)
		{
			indents[curr_indent] = indent;
			separators[curr_indent] = separator;
			if (curr_indent > oldindent)
				firstone = TRUE;
		}
		else                         /* popped the stack */
		{
			printed = printeds[oldindent];
			firstone = firstones[oldindent];
			oldindent--;
			while (oldindent >= curr_indent)
			{
				if (! firstones[oldindent])
					firstone = FALSE;
				if ((printed) || (printeds[oldindent]))
				{
					firstone = FALSE;
					printed = TRUE;
					suffix = pfstack[oldindent]->format->suffix;
					if ((suffix == NULL) && (pfstack[oldindent]->_template != NULL))
						suffix = pfstack[oldindent]->_template->format->suffix;
					if (suffix != NULL)
					{
						had_newline = PrintStringConvert(spop, suffix, had_newline, indents[oldindent], FALSE);
					}
				}
				oldindent--;
			}
			suffix = NULL;
			firstones[curr_indent] = firstone;
			printed = FALSE;
			indent = indents[curr_indent];
			separator = separators[curr_indent];
		}
		pfstack[curr_indent] = pfip;

		if (pfip->_template != NULL)   /* use_template branch */
		{
			format = pfip->_template->format;
			tmppsp = (PrintStackPtr)(psip->dv.ptrvalue);
			datav = tmppsp->data[0].dv;
		}
		else
		{
			format = pfip->format;
			datav = psip->dv;
		}

		form = format->form;

		if ((label == NULL) && (i != labelused))
		{
			label = pfip->format->label;

			if ((label == NULL) && (pfip->_template != NULL))
				label = format->label;
		}
		prefix = pfip->format->prefix;
		if ((prefix == NULL) && (pfip->_template != NULL))
			prefix = format->prefix;
		suffix = pfip->format->suffix;
		if ((suffix == NULL) && (pfip->_template != NULL))
			suffix = format->suffix;
/***
		printf("A[%d] firstone=%d separator=[%s]\n", (int)i,(int)firstone, separator);
**/
		switch (form->choice)
		{
			case PrintForm_boolean:
				pfbp = (PrintFormBooleanPtr) (form->data.ptrvalue);
				if (datav.boolvalue)
					data = pfbp->truepfb;
				else
					data = pfbp->falsepfb;
				break;
			case PrintForm_enum:
				pfep = (PrintFormEnumPtr) (form->data.ptrvalue);
				if (pfep->values == NULL)
					data = AsnEnumTypeStr(pfip->atp, (Int2)(datav.intvalue));
				else
				{
				  	for (j=0, tmpval=pfep->values; ((j < datav.intvalue) && (tmpval != NULL));	j++)
					  tmpval = tmpval->next;
					if (tmpval != NULL)
						data = (CharPtr)(tmpval->data.ptrvalue);
				}
				if (data == NULL)	/* failed on enumerated type */
				{
					sprintf(localbuf, "%ld", (long)(datav.intvalue));
					data = localbuf;
				}
				break;
			case PrintForm_text:
				pftp = (PrintFormTextPtr)(form->data.ptrvalue);
				if (pftp->textfunc != NULL)
					data = pftp->textfunc; /* kludge for now */
				else
				{
					if (ISA_STRINGTYPE(pfip->isa))
						data = (CharPtr)(datav.ptrvalue);
					else if (ISA_INTTYPE(pfip->isa))
					{
						sprintf(localbuf, "%ld", (long)(datav.intvalue));
						data = localbuf;
					}
					else if (pfip->isa == REAL_TYPE)
					{
						sprintf(localbuf, "%g", (double)(datav.realvalue));
						data = localbuf;
					}
					else
						data = "Other Text Type";
				}
				break;
			case PrintForm_null:
				data = pfip->format->asn1;
				break;
			case PrintForm_block:
				nocomponents = TRUE;
				if ((i+1) < psp->size)
				{
					nxtpsip = PrintStackItemGet(psp, (Int2)(i+1), &indent_offset);
					if ((nxtpsip->pfip->indent_level + indent_offset) > curr_indent)
						nocomponents = FALSE;
				}
				break;
			case PrintForm_user:
				ufp = (UserFormatPtr)(form->data.ptrvalue);
				if (! StringCmp("StdSeqLocPrint", ufp->printfunc))
					data = StdSeqLocPrint(&datav);
				else if (! StringCmp("StdDatePrint", ufp->printfunc))
					data = StdDatePrint(&datav);
				else if (! StringCmp("StdAuthListNamesPrint", ufp->printfunc))
					data = StdAuthListNamesPrint(&datav);
				freedata = TRUE;
				break;

		}

		if ((! nocomponents) && ((label != NULL) || (prefix != NULL) || (data != NULL)))
		{								   /** print something */
			printed = TRUE;

			j = curr_indent;
			if (j)      /* check for previous unprinted separators */
			{
				for (j = j-1; ((j > 0) && (! printeds[j])); j--)
					continue;
				j++;
				while (j < curr_indent)
				{
					if ((! printeds[j]) && (! firstones[j]) && (separators[j] != NULL))
					{
						printeds[j] = TRUE;
						had_newline = PrintStringConvert(spop, separators[j], had_newline, indents[j], FALSE);
					}
					j++;
				}
			}
/***
		printf("B[%d] firstone=%d separator=[%s]\n", (int)i,(int)firstone, separator);
***/
			if ((separator != NULL) && (! firstone))
			{
				had_newline = PrintStringConvert(spop, separator, had_newline, indent, FALSE);
			}

			if (label != NULL)
				had_newline = PrintStringConvert(spop, label, had_newline, indent, FALSE);
			if (prefix != NULL)
				had_newline = PrintStringConvert(spop, prefix, had_newline, indent, FALSE);
			if (data != NULL)
				had_newline = PrintStringConvert(spop, data, had_newline, indent, TRUE);
			if (freedata) MemFree(data);
		}

		if ((printed) && (suffix != NULL) && (form->choice != PrintForm_block))
			had_newline = PrintStringConvert(spop, suffix, had_newline, indent, FALSE);

		if (form->choice == PrintForm_block)
		{
			if (! nocomponents)
			{
				if ((printed) && (had_newline))
					indent++;
				pfkp = (PrintFormBlockPtr)(form->data.ptrvalue);
				separator = pfkp->separator;
			}
		}

		if (printed)
			firstone = FALSE;
		label = NULL;
		oldindent = curr_indent;
		printeds[oldindent] = printed;
		firstones[oldindent] = firstone;
	}

	if (oldindent)
	{
		oldindent--;
		while (oldindent >= 0)
		{
			if (printeds[oldindent])
			{
				suffix = pfstack[oldindent]->format->suffix;
				if ((suffix == NULL) && (pfstack[oldindent]->_template != NULL))
					suffix = pfstack[oldindent]->format->suffix;
				if (suffix != NULL)
					had_newline = PrintStringConvert(spop, suffix, had_newline, indent, FALSE);
			}
			oldindent--;
		}
		suffix = NULL;
	}
	return;
}

NLM_EXTERN void PrintCharFunc PROTO((Char c, StdPrintOptionsPtr spop));
/*****************************************************************************
*
*   Boolean PrintStringConvert(spop, str)
*
*****************************************************************************/
NLM_EXTERN Boolean PrintStringConvert(StdPrintOptionsPtr spop, CharPtr str, Boolean had_newline, Int2 indent, Boolean tilde)
{
	CharPtr tmp, tmp2, wrd;
	Char c, prev;
	static Char lastchar = '\0';
	Int2 i, wordlen, linelength, space;
	static Int2 ctr = 0;
	Boolean hit_newline;

	if (str == NULL)
	{
		lastchar = '\0';
		ctr = 0;
		return FALSE;
	}

	if (spop == NULL) return FALSE;

	linelength = spop->linelength;
	if (linelength <= 0)
		linelength = INT2_MAX;
		
	if ((had_newline) && (spop->indent != NULL))
	{
		ctr = 0;
		for (i = 0; i < indent; i++)
		{
			for (tmp = spop->indent; *tmp != '\0'; tmp++)
			{
				PrintCharFunc(*tmp, spop);
				ctr++;
			}
		}
	}
	had_newline = FALSE;

	if (lastchar == '\n')		  /* remove repeated newlines */
	{
		if ((*str == '\\') && (*(str+1) == 'n'))
		{
			str += 2;
		}
	}

	lastchar = '\0';
	c = lastchar;

	prev = 'x';
	while (*str != '\0')
	{
		hit_newline = FALSE;
		for (wrd = str, space = 0; (*wrd == ' '); wrd++)
			space++;
		for (wordlen =0;(*wrd > ' '); wrd++)
		{
			if ((*wrd == '\\') && (! tilde))
			{
				if ((*(wrd+1)== 'n') || (*(wrd+1)== 'r'))
				{
					wordlen++;
					hit_newline = TRUE;
					break;
				}
				else
				{
					wordlen++;
					wrd++;
					if (*wrd == '\0')
						break;
				}
			}
			else if ((tilde) && (*wrd == '~'))
			{
				wordlen++;
				hit_newline = TRUE;
				break;
			}
			else
				wordlen++;
		}
		if (((ctr + space + wordlen) > linelength) && (! hit_newline) &&
			(c != '\n') && (c != '\0'))  /* word wrap */
		{
			for (tmp2 = spop->newline; *tmp2 != '\0'; tmp2++)
				PrintCharFunc(*tmp2, spop);
			ctr = 0;
			for (i = 0; i < indent; i++)
			{
				for (tmp = spop->indent; *tmp != '\0'; tmp++)
				{
					PrintCharFunc(*tmp, spop);
					ctr++;
				}
			}
		}

		hit_newline = FALSE;

		for(wordlen += space; wordlen > 0; wordlen--)
		{
			tmp2 = NULL;
		
			c = *str;
			if ((c == '\\') && (! tilde))
			{
				str++;
				c = *str;
				if (! c)
				{
					lastchar = prev;
					return had_newline;
				}
				switch (c)
				{
					case 'n':
						c = '\n';
						tmp2 = spop->newline;
						had_newline = TRUE;
						hit_newline = TRUE;
						break;
					case 'r':
						c = '\r';
						break;
					case 't':
						tmp2 = spop->tab;
						c = '\t';
						break;
				}
			}
			else if ((c == '~') && (tilde))
			{
				c = '\n';
				tmp2 = spop->newline;
				had_newline = TRUE;
				hit_newline = TRUE;
			}

			if ((prev == '\n') && (c != '\n') && (spop->indent != NULL))
			{
				for (i = 0; i < indent; i++)
				{
					for (tmp = spop->indent; *tmp != '\0'; tmp++)
					{
						PrintCharFunc(*tmp, spop);
						ctr++;
					}
				}
			}
			if (tmp2 == NULL)   /* just print the char */
			{
				PrintCharFunc(c, spop);
				ctr++;
			}
			else
			{
				while (*tmp2 != '\0')
				{
					PrintCharFunc(*tmp2, spop);
					tmp2++; ctr++;
				}
			}
			prev = c;
			str++;
			if (hit_newline)
				ctr = 0;
		}
	}
	lastchar = c;
	return had_newline;
}

/*****************************************************************************
*
*   PrintCharFunc(c, spop)
*   	prints a char to a file or bytestore
*
*****************************************************************************/
NLM_EXTERN void PrintCharFunc(Char c, StdPrintOptionsPtr spop)
{
	if ((c < ' ') || (c > '~'))
	{
		if (c != '\n' && c != '\r' && c != '\t' && spop->fp != NULL)
		{
			fputc((int)'#', spop->fp);
			fflush(spop->fp);
			return;
		}
	}
	if (spop->fp != NULL)
		fputc((int)c, spop->fp);
	else if (spop->bstp != NULL)
		BSPutByte(spop->bstp, (Int2)c);
	return;
}

/*****************************************************************************
*
*   StdFormatPrint(data, func, templatename, options)
*
*****************************************************************************/
NLM_EXTERN Boolean StdFormatPrint (Pointer data, AsnWriteFunc func, CharPtr templatename, StdPrintOptionsPtr options)
{
	PrintStackPtr psp;
	CharPtr ptr = NULL;
	PrintFormatListPtr pflp;

	if ((data == NULL) || (func == NULL) || (templatename == NULL) || (options == NULL))
		return FALSE;

	pflp = PrintFormatListGet(templatename);

	if (pflp == NULL) return FALSE;

	if (options->fp == NULL)
	{
		options->bstp = BSNew(80);
		options->ptr = NULL;
	}
	psp = PrintStackBuild(data, func, pflp);

/***
	printf("Before sort\n");
	PrintStackDump(psp, stdout, 0);
***/
	PrintStackSort(psp);
/***
	printf("After sort\n");
	PrintStackDump(psp, stdout, 0);
	printf("\n\n");
***/

	PrintStackPrint(psp, options);
	PrintStackFree(psp);

	if (options->fp == NULL)
	{
		if (BSLen(options->bstp))
			options->ptr = BSMerge(options->bstp, NULL);
		BSFree(options->bstp);
	}
	return TRUE;
}

static PrintFormatListPtr PNTR PFLlist = NULL;
static Int2 PFLnum = 0;

/**************************************************
*
*    PrintFormatListNew()
*
**************************************************/

NLM_EXTERN PrintFormatListPtr PrintFormatListNew(void)
{
	PrintFormatListPtr pflp;
	PrintFormatListPtr PNTR tmp;
	Int2 i;

	pflp = (PrintFormatListPtr) MemNew(sizeof(PrintFormatList));
    for (i = 0; i < PFLnum; i++)    /* add to Template list */
    {
        if (PFLlist[i] == NULL)
        {
            PFLlist[i] = pflp;
            return pflp;
        }
    }
    tmp = PFLlist;                 /* expand list as necessary */
    PFLlist = MemNew((sizeof(PrintFormatListPtr) * (PFLnum + 20)));
    MemCopy(PFLlist, tmp, (size_t)(sizeof(PrintFormatListPtr) * PFLnum));
    MemFree(tmp);
    PFLlist[PFLnum] = pflp;
    PFLnum += 20;
	return pflp;
}

/**************************************************
*
*    PrintFormatListFree()
*
**************************************************/
NLM_EXTERN PrintFormatListPtr PrintFormatListFree ( PrintFormatListPtr ptr)
{
	Int2 i;

	if (ptr == NULL) return NULL;

	for (i = 0; i < ptr->numitem; i++)
	{
		MemFree(ptr->list[i].types);
	}
	MemFree(ptr->list);
    for (i = 0; i < PFLnum; i++)
    {
        if (PFLlist[i] == ptr)
        {
            PFLlist[i] = NULL;
            return MemFree(ptr);
        }
    }
    ErrPost(CTX_NCBIOBJ, 1, "PrintFormatListFree: pointer not in PFLlist");
	return MemFree(ptr);
}

/*****************************************************************************
*
*   PrintFormatListFreeAll(templates)
*   	frees all PrintFormatLists in memory
*   	if templates == TRUE, frees all templates too.
*
*****************************************************************************/
NLM_EXTERN void PrintFormatListFreeAll(Boolean templates)
{
    Int2 i;

    for (i = 0; i < PFLnum; i++)
    {
		PrintFormatListFree(PFLlist[i]);
    }

	if (templates)
		PrintTemplateFreeAll();

	return;
}

/*****************************************************************************
*
*   PrintFormatListFind(name)
*   	Just checks in object loaded memory
*
*****************************************************************************/
NLM_EXTERN PrintFormatListPtr PrintFormatListFind (CharPtr name)
{
    Int2 i;

	if (name == NULL)
		return NULL;

    for (i = 0; i < PFLnum; i++)
    {
    	if (PFLlist[i] != NULL && PFLlist[i]->ptp != NULL) {
        	if (! StringCmp(PFLlist[i]->ptp->name, name))
          	  return PFLlist[i];
        }
    }
    return NULL;
}

/*****************************************************************************
*
*   PrintFormatListGet(name)
*   	looks in PFLlist to see if it already exists
*       if failure, then tries to build it (template must be loaded)
*   	otherwise fails (returns NULL)
*   	name is a template name
*
*****************************************************************************/
NLM_EXTERN PrintFormatListPtr PrintFormatListGet (CharPtr templatename)
{
	PrintFormatListPtr pflp;
	PrintTemplatePtr ptp;

	if (templatename == NULL) return NULL;

	pflp = PrintFormatListFind(templatename);
	if (pflp != NULL) return pflp;

	ptp = PrintTemplateFind(templatename);
	if (ptp == NULL) return NULL;

	pflp = PrintFormatListBuild(ptp);
	return pflp;
}


/*****************************************************************************
*
*   StdPrintOptionsNew(fp)
*     if (fp == NULL) returns test in spop->ptr
*
*****************************************************************************/
NLM_EXTERN StdPrintOptionsPtr StdPrintOptionsNew (FILE * fp)
{
	StdPrintOptionsPtr spop;

	spop = MemNew(sizeof(StdPrintOptions));
	spop->fp = fp;
	spop->indent = "  ";     /* 2 spaces is std indent */
	spop->newline = "\n";    /* newline is newline */
	spop->tab = "\t";        /* tab is tab */

	return spop;
}

/*****************************************************************************
*
*   StdPrintOptionsFree(spop)
*   	does not free strings or close file
*
*****************************************************************************/
NLM_EXTERN StdPrintOptionsPtr StdPrintOptionsFree (StdPrintOptionsPtr spop)
{
	return (StdPrintOptionsPtr) MemFree(spop);
}


/*****************************************************************************
*
*   These should eventually be moved to their underlying api files
*
*****************************************************************************/


/*****************************************************************************
*
*   StdSeqLocPrint(dvp)
*
*****************************************************************************/
NLM_EXTERN CharPtr StdSeqLocPrint(DataValPtr dvp)
{
	return SeqLocPrint((SeqLocPtr)(dvp->ptrvalue));
}

/*****************************************************************************
*
*   StdDatePrint(dvp)
*
*****************************************************************************/
NLM_EXTERN CharPtr StdDatePrint(DataValPtr dvp)
{
	Char buf[40];

	if ( DatePrint((DatePtr)(dvp->ptrvalue), buf))
		return StringSave(buf);
	else
		return NULL;
}

/*****************************************************************************
*
*   StdAuthListNamesPrint(dvp)
*   	prints just Auth-list.names to a string
*
*****************************************************************************/
NLM_EXTERN CharPtr StdAuthListNamesPrint(DataValPtr dvp)
{
	AuthListPtr alp;
	Boolean 	first = TRUE;
	ValNodePtr   vnp;
	PersonIdPtr  pip;
	AuthorPtr    ap;
	ByteStorePtr bsp;
	CharPtr str;
	Char buf[60];

	if (dvp == NULL) return NULL;
	alp = (AuthListPtr)(dvp->ptrvalue);
	if (alp == NULL) return NULL;

	bsp = BSNew(80);

	for (vnp = alp->names; vnp != NULL; vnp = vnp->next)
	{
		if (! first)     /* separators for names after first one */
		{
			if (vnp->next != NULL)
				BSPutByte(bsp, (Int2)',');
			else
			{
				BSPutByte(bsp, (Int2)' ');
				BSPutByte(bsp, (Int2)'&');
			}
			BSPutByte(bsp, (Int2)' ');
		}

		first = FALSE;
		if (alp->choice == 3 || alp->choice == 2)  /* ml or str */
		{
			BSWrite(bsp, vnp->data.ptrvalue, (Int4)(StringLen((CharPtr)(vnp->data.ptrvalue))));
		}
		else if (alp->choice == 1)   /* std */
		{
			ap = (AuthorPtr) (vnp->data.ptrvalue);
			if (ap != NULL)
			{
				pip = ap->name;
				PersonIdPrint(pip, buf);
				BSWrite(bsp, buf, (Int4)StringLen(buf));
			}
			else
				BSPutByte(bsp, (Int2)'?');
		}
	}

	str = BSMerge(bsp, NULL);
	BSFree(bsp);
	return str;
}

/*****************************************************************************
*
*   PersonIdPrint(pip, buf)
*
*****************************************************************************/
NLM_EXTERN CharPtr PersonIdPrint(PersonIdPtr pip, CharPtr buf)
{
	CharPtr tmp;
	NameStdPtr nsp;

	if (buf == NULL) return NULL;

	tmp = buf;
	*tmp = '?';
	*(tmp + 1) = '\0';

	if (pip == NULL) return buf;

	if (pip->choice == 2)    /* std */
	{
		nsp = (NameStdPtr) (pip->data);
		if (nsp != NULL)
		{
			if (nsp->names [3] != NULL)                 /* full name */
				tmp = StringMove(tmp, nsp->names [3]);
			else if (nsp->names[0] != NULL)             /* make from parts */
			{
				if (nsp->names [1] != NULL && nsp->names [2] != NULL)
				{
					tmp = StringMove(tmp, nsp->names [1]);	/* first */
					*tmp = ' '; tmp++; *tmp='\0';
					tmp = StringMove(tmp, nsp->names [2]);	/* middle */
					*tmp = ' '; tmp++; *tmp='\0';
				}
				else if ((nsp->names [4] != NULL) &&
						(nsp->names [0] != NULL) &&
						(StringLen(nsp->names[4]) < 10))
				{
					tmp = StringMove(tmp, nsp->names [4]);  /* initials */
					*tmp = ' '; tmp++; *tmp='\0';
				}
				else if (nsp->names [1] != NULL)
				{
					tmp = StringMove(tmp, nsp->names [1]);
					*tmp = ' '; tmp++; *tmp='\0';
				}
				tmp = StringMove(tmp, nsp->names [0]);		/* last */
				if (nsp->names[5] != NULL)                  /* suffix */
				{
					*tmp = ' '; tmp++; *tmp='\0';
					tmp = StringMove(tmp, nsp->names[5]);
				}
			}
		}
	}
	else if (pip->choice == 3 || pip->choice == 4) /* ml or str */
		StringMove(tmp, (CharPtr)(pip->data));

	return buf;
}

/*****************************************************************************
*
*   PrintStackDump
*
*****************************************************************************/
NLM_EXTERN void PrintStackDump(PrintStackPtr psp, FILE *fp, Int2 indent)
{
	Int2 i, j;
	PrintStackItemPtr psip;

	if ((psp == NULL) || (fp == NULL)) return;

	for (j = 0; j < indent; j++)
		fputc(32, fp);
	fprintf(fp, "=>[%s] indent=%d is_branch=%d size=%d pstacksize=%d\n",
		psp->pflp->ptp->name, (int)psp->indent, (int)psp->is_branch,
		(int)(psp->size), (int)(psp->pstacksize));

	for (i = 0; i < psp->pstackpos; i++)
	{
		psip = psp->data + i;
		for (j = 0; j < indent; j++)
			fputc(32, fp);
		fprintf(fp, " [%d][%s] sort=%d indent=%d ", (int)i, psip->pfip->format->asn1,
			(int)(psip->pfip->sort_key), (int)(psip->pfip->indent_level));

		if (psip->pfip->_template != NULL)
		{
			fprintf(fp, "Use-template\n");
			PrintStackDump((PrintStackPtr)(psip->dv.ptrvalue), fp, (Int2)(indent+2));
		}
		else switch (psip->pfip->format->form->choice)
		{
			case PrintForm_boolean:
				if (psip->dv.intvalue)
					fprintf(fp, "TRUE\n");
				else
					fprintf(fp, "FALSE\n");
				break;
			case PrintForm_enum:
				fprintf(fp, "%s\n", AsnEnumTypeStr(psip->pfip->atp, (Int2)(psip->dv.intvalue)));
				break;
			case PrintForm_text:
				if (ISA_STRINGTYPE(psip->pfip->isa))
					fprintf(fp, "%s\n", (CharPtr)(psip->dv.ptrvalue));
				else if (ISA_INTTYPE(psip->pfip->isa))
					fprintf(fp, "%ld\n", (long)(psip->dv.intvalue));
				else
					fprintf(fp, "Other Text Type\n");
				break;
			case PrintForm_null:
				fprintf(fp, "NULL\n");
				break;
			case PrintForm_block:
				fprintf(fp, "Block\n");
				break;
			case PrintForm_user:
				fprintf(fp, "UserFormat\n");
				break;

		}
	}

	fflush(fp);
	return;
}

