/*  asnprint.c
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
* File Name:  asnprint.c
*
* Author:  James Ostell
*
* Version Creation Date: 3/4/91
*
* $Revision: 6.20 $
*
* File Description:
*   Routines for printing ASN.1 value notation (text) messages and
*     ASN.1 module specifications
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 3/4/91   Kans        Stricter typecasting for GNU C and C++
*
* $Log: asnprint.c,v $
* Revision 6.20  2003/12/03 19:31:09  gouriano
* Corrected DTD generation (a different approach)
*
* Revision 6.19  2003/12/02 19:52:48  gouriano
* Corrected DTD generation
*
* Revision 6.18  2003/09/15 16:16:32  kans
* added AsnWriteEx, AsnTxtWriteEx, and AsnPrintStream
*
* Revision 6.17  2002/06/10 15:19:16  kans
* AsnPrintString checks for NULL input string, reports error and returns FALSE
*
* Revision 6.16  2002/03/08 20:13:10  ivanov
* Fixed AsnPrintNewLine(), AsnPrintString() -- accurate print XML
*
* Revision 6.15  2001/10/11 14:39:08  ostell
* added support for XMLModulePrefix
*
* Revision 6.14  2001/03/28 01:25:38  juran
* Removed unused variable.
*
* Revision 6.13  2000/12/12 15:56:14  ostell
* added support BigInt
*
* Revision 6.12  2000/07/27 12:28:04  ostell
* fixed PUBLIC identifier for DTDs in XML
*
* Revision 6.11  2000/07/25 20:30:58  ostell
* added support for printing multiple ASN.1 modules as multiple XML DTD and .mod files
*
* Revision 6.10  2000/06/29 20:15:15  ostell
* minor typos fixed
*
* Revision 6.9  2000/05/26 14:55:31  ostell
* remove apostrophes and H from OCTETS for XML
*
* Revision 6.8  2000/05/22 15:01:06  ostell
* added check to AsnXMLTag for ENUM in SETOF/SEQOF
*
* Revision 6.7  2000/05/12 20:44:01  ostell
* make changes to collect comments from spec and print in DTD
*
* Revision 6.6  2000/05/11 21:35:44  ostell
* made elements and ennumerated prettier in DTD
*
* Revision 6.5  2000/05/10 15:30:01  ostell
* fixed CHOICE poping multiple levels deep for XML
*
* Revision 6.4  2000/05/10 04:34:12  ostell
* couple more xml fixes
*
* Revision 6.3  2000/05/10 03:12:37  ostell
* added support for XML DTD and XML data output
*
* Revision 6.2  1999/07/15 18:52:52  shavirin
* Fixed mantissa overflow in the function AsnPrintReal().
*
* Revision 6.1  1998/06/12 19:27:53  kans
* fixed unix compiler warnings
*
* Revision 6.0  1997/08/25 18:10:18  madden
* Revision changed to 6.0
*
* Revision 5.1  1996/12/03 21:43:48  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
 * Revision 5.0  1996/05/28  14:00:29  ostell
 * Set to revision 5.0
 *
 * Revision 4.1  1996/02/18  16:45:36  ostell
 * changed fix_non_print behavior and added option 3
 *
 * Revision 4.0  1995/07/26  13:47:38  ostell
 * force revision to 4.0
 *
 * Revision 2.17  1995/07/13  14:28:37  madden
 * Changed tbuf in AsnPrintInteger from 11 to 20.
 *
 * Revision 2.16  1995/07/13  14:19:06  madden
 * Changed tbuf in AsnPrintInteger from 10 to 11 bytes.
 *
 * Revision 2.15  1995/05/15  18:38:28  ostell
 * added Log line
 *
*
* ==========================================================================
*/

/*****************************************************************************
*
*   asnprint.c
*   	print routines for asn1 objects
*
*****************************************************************************/

#include "asnbuild.h"


/********
     MakeXMLModuleName()
	makes module name into file name
	returns pointer to \0 at end of string
********/
static CharPtr MakeXMLModuleName(AsnModulePtr amp, CharPtr buf)
{
	CharPtr ptr;
	StringMove(buf, amp->modulename);
	for (ptr = buf; *ptr != '\0'; ptr++)
	{
		if (*ptr == '-')
			*ptr = '_';
	}
	return ptr;
}


/********
     MakeXMLPublicName()
	makes module name into name for PUBLIC declaration
	returns pointer to \0 at end of string
********/
static CharPtr MakeXMLPublicName(CharPtr tobuf, CharPtr frombuf)
{
	CharPtr ptr;
	StringMove(tobuf, frombuf);
	for (ptr = tobuf; *ptr != '\0'; ptr++)
	{
		if (! IS_ALPHANUM(*ptr))
			*ptr = ' ';
	}
	return ptr;
}

	
static CharPtr GetXMLModuleName(AsnTypePtr atp, CharPtr buf)
{
	AsnModulePtr currmod;
	AsnTypePtr curratp, baseatp;

	currmod = AsnAllModPtr();  /* get loaded modules */
	baseatp = AsnFindBaseType(atp);
	if ((currmod == NULL) || (baseatp == NULL))
		return NULL;

	for (currmod; currmod != NULL; currmod = currmod->next)
	{
		for (curratp = currmod->types; curratp != NULL; curratp = curratp->next)
		{
			if (curratp == baseatp)
			{
				MakeXMLModuleName(currmod, buf);
				return buf;
			}
		}
	}
	return NULL; /* didn't find it */
}

static CharPtr GetXMLname(AsnTypePtr atp)
{
	if (atp == NULL)
		return "NULL_PTR";
	if (atp->XMLname != NULL)
		return atp->XMLname;
	if (atp->name != NULL)
		return atp->name;
	return "NO_NAME";
}

static Boolean AsnXMLTag(AsnIoPtr aip, AsnTypePtr atp, Boolean term)
{
	Char buf [100];
	CharPtr tmp;
	AsnTypePtr atp2;
	Int2 isa;
	Boolean noend = FALSE;

	tmp = StringMove(buf, "<");
	if (term)
		tmp = StringMove(tmp, "/");
	if (atp->XMLname != NULL)
		tmp = StringMove(tmp, atp->XMLname);
	else
		tmp = StringMove(tmp, "NONAME_FOUND");

	if (! term) 
    {
        atp2 = AsnFindBaseType(atp);
        
        if ((atp == atp2) || (atp->name == NULL))
        {
            isa = AsnFindBaseIsa(atp);
            switch (isa)
            {
                case NULL_TYPE:
                    tmp = StringMove(tmp, "/");
                    break; /* done */
                case BOOLEAN_TYPE:
                case ENUM_TYPE:
                case INTEGER_TYPE:
                case BIGINT_TYPE:
                    noend = TRUE;
                    break;
                default:
                    break;
            }
        }
    }
	if (! noend)
		tmp = StringMove(tmp, ">");

	AsnPrintCharBlock(buf, aip);
	return noend;
}

static Boolean AsnXMLBegin(AsnIoPtr aip, AsnTypePtr atp)
{
	AsnTypePtr atp2;
	Boolean noend;

	noend = AsnXMLTag(aip, atp, FALSE);

	atp2 = AsnFindBaseType(atp);
	if ((atp2 != NULL) && (atp != atp2))
	{
		if ((atp2->name != NULL) && (atp->name != NULL))
		{
			if (StringCmp(atp->name, atp2->name))
			{
				AsnPrintIndent(TRUE, aip);
				AsnPrintNewLine(aip);
				noend = AsnXMLTag(aip, atp2, FALSE);
			}
		}
	}
	return noend;
}

static void AsnXMLTerm(AsnIoPtr aip, AsnTypePtr atp)
{
	AsnTypePtr atp2;

	atp2 = AsnFindBaseType(atp);
	if ((atp2 != NULL) && (atp != atp2))
	{
		if ((atp2->name != NULL) && (atp->name != NULL))
		{
			if (StringCmp(atp->name, atp2->name))
			{
				AsnXMLTag(aip, atp2, TRUE);
				AsnPrintIndent(FALSE, aip);
				AsnPrintNewLine(aip);
			}
		}
	}
	AsnXMLTag(aip, atp, TRUE);
}

static void AsnXMLTermEmpty(AsnIoPtr aip, AsnTypePtr atp)
{
	AsnTypePtr atp2;
	Boolean do_empty = TRUE;

	atp2 = AsnFindBaseType(atp);
	if ((atp2 != NULL) && (atp != atp2))
	{
		if ((atp2->name != NULL) && (atp->name != NULL))
		{
			if (StringCmp(atp->name, atp2->name))
			{
				AsnPrintCharBlock("/>", aip);
				AsnPrintIndent(FALSE, aip);
				AsnPrintNewLine(aip);
				do_empty = FALSE;
			}
		}
	}
	if (do_empty)
	{
		AsnPrintCharBlock("/>", aip);
		AsnPrintNewLine(aip);
	}
	else
		AsnXMLTag(aip, atp, TRUE);
}

static Boolean AsnPrintTypeXML (AsnTypePtr atp, AsnIoPtr aip);

/*****************************************************************************
*
*   void AsnTxtWrite(aip, atp, valueptr)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL  AsnTxtWriteEx (AsnIoPtr aip, AsnTypePtr atp, DataValPtr dvp, AsnStreamStringFunc stream)
{
	Int2 isa;
	AsnTypePtr atp2;
	AsnValxNodePtr avnp;
	Boolean done, terminalvalue, firstvalue, isXML = FALSE;

	terminalvalue = TRUE;   /* most are terminal values */
	if ((! aip->indent_level) && (aip->typestack[0].type == NULL))
		firstvalue = TRUE;    /* first call to this routine */
	else
		firstvalue = FALSE;

	if (! AsnTypeValidateOut(aip, atp, dvp))
		return FALSE;

	if (aip->type & ASNIO_XML)
		isXML = TRUE;

	atp2 = AsnFindBaseType(atp);
	isa = atp2->type->isa;
	if (ISA_STRINGTYPE(isa))
		isa = GENERALSTRING_TYPE;
	
	if (((isa == SEQ_TYPE) || (isa == SET_TYPE) ||
		 (isa == SEQOF_TYPE) || (isa == SETOF_TYPE))
		 && (dvp->intvalue == END_STRUCT))
	{
		AsnPrintCloseStruct(aip, atp);
		return TRUE;
	}

	if (! aip->first[aip->indent_level]) {
	    AsnPrintNewLine(aip);
	} else
		aip->first[aip->indent_level] = FALSE;

	atp2 = atp;
	if (firstvalue)       /* first item, need ::= */
	{
		if(isXML) {
			AsnPrintCharBlock("<?xml version=\"1.0\"?>", aip);
			AsnPrintNewLine(aip);
		}

		while ((atp2->name == NULL) || (IS_LOWER(*atp2->name)))
			atp2 = atp2->type;    /* find a Type Reference */

		if (isXML)
		{
			Char tbuf[250];
			Char tname[100], pname[100];
			CharPtr xmlname;

			if (GetXMLModuleName(atp2, tname) == NULL)
			{
				StringMove(tname, "Not_Found");
			}
			MakeXMLPublicName(pname, tname);
			xmlname = GetXMLname(atp2);
			sprintf(tbuf, "<!DOCTYPE %s PUBLIC \"-//NCBI//%s/EN\" \"%s%s.dtd\">",
				xmlname, pname, AsnGetXMLmodulePrefix(), tname);
			AsnPrintCharBlock(tbuf, aip);
			AsnPrintNewLine(aip);
		}
	}

	if(isXML)
	{
		AsnXMLBegin(aip, atp2);
    }
	else if (atp2->name != NULL)
	{
		AsnPrintString(atp2->name, aip);   /* put the element name */
		if (IS_LOWER(*atp2->name))
			AsnPrintChar(' ', aip);
		else
			AsnPrintString(" ::= ", aip);
	}

	if (isa == CHOICE_TYPE)     /* show nothing but name on same line */
	{
		if ((aip->type_indent))
		{
			isa = AsnFindBaseIsa(aip->typestack[aip->type_indent - 1].type);
			if ((isa != SEQOF_TYPE) && (isa != SETOF_TYPE))
			{
				AsnPrintIndent(TRUE, aip);
				AsnTypeSetIndent(TRUE, aip, atp);
				AsnPrintNewLine(aip);
			}
			else
			{
			
				if(isXML)
					AsnPrintIndent(TRUE, aip);
			

				AsnTypeSetIndent(TRUE, aip, atp);

				if(isXML)
					AsnPrintNewLine(aip);
			}
		}
		else
		{
			
			if(isXML)
				AsnPrintIndent(TRUE, aip);
			

			AsnTypeSetIndent(TRUE, aip, atp);

			if(isXML)
				AsnPrintNewLine(aip);
		}
		aip->first[aip->indent_level] = TRUE;
		return TRUE;
	}

	switch (isa)
	{
		case SEQ_TYPE:
		case SET_TYPE:
		case SEQOF_TYPE:
		case SETOF_TYPE:
			if (dvp->intvalue == START_STRUCT)   /* open brace */
				AsnPrintOpenStruct(aip, atp);
			else
			{
				AsnIoErrorMsg(aip, 18 );
				return FALSE;
			}
			terminalvalue = FALSE;
			break;
		case BOOLEAN_TYPE:
			AsnPrintBoolean(dvp->boolvalue, aip);
			if (isXML)
				AsnXMLTermEmpty(aip, atp);
			break;
		case INTEGER_TYPE:
		case ENUM_TYPE:
		case BIGINT_TYPE:
			atp2 = AsnFindBaseType(atp);  /* check for names */
			avnp = (AsnValxNodePtr) atp2->branch;
			done = FALSE;
			while (avnp != NULL)
			{
				if (dvp->intvalue == avnp->intvalue)
				{
					if (isXML)
					{
						AsnPrintString(" value=", aip);
						AsnPrintChar('\"', aip);
					}
					AsnPrintString(avnp->name, aip);
					if (isXML)
					{
						AsnPrintChar('\"', aip);
						if (isa == ENUM_TYPE)
						{
							AsnXMLTermEmpty(aip, atp);
							done = TRUE;
						}
					}
					else
						done = TRUE;
					avnp = NULL;
				}
				else
					avnp = avnp->next;
			}
			if (! done)    /* no name */
			{
				if (isXML)
					AsnPrintCharBlock(">", aip);
				if (isa == BIGINT_TYPE)
					AsnPrintBigInt(dvp->bigintvalue, aip);
				else
					AsnPrintInteger(dvp->intvalue, aip);
				if(isXML) AsnXMLTerm(aip, atp);
			}
			break;
	       case REAL_TYPE:
			AsnPrintReal(dvp->realvalue, aip);
			if(isXML) AsnXMLTerm(aip, atp);
			break;
		case GENERALSTRING_TYPE:
			if(!(isXML)) AsnPrintChar('\"', aip);
			if (stream != NULL) {
				if (! AsnPrintStream (dvp->ptrvalue, aip, stream))
					return FALSE;
			} else {
				if (! AsnPrintString((CharPtr) dvp->ptrvalue, aip))
					return FALSE;
			}
			if(isXML) AsnXMLTerm(aip, atp);
			else AsnPrintChar('\"', aip);
			break;
		case NULL_TYPE:
			if (! (isXML))
				AsnPrintString("NULL", aip);
			break;
		case OCTETS_TYPE:
			AsnPrintOctets((ByteStorePtr) dvp->ptrvalue, aip);
			if(isXML) AsnXMLTerm(aip, atp);
			break;
		case STRSTORE_TYPE:
			if (! AsnPrintStrStore((ByteStorePtr) dvp->ptrvalue, aip))
				return FALSE;
			if(isXML) AsnXMLTerm(aip, atp);
			break;
		default:
			AsnIoErrorMsg(aip, 19, AsnErrGetTypeName(atp->name));
			return FALSE;
	}

	if ((terminalvalue) && (aip->type_indent))   /* pop out of choice nests */
	{
		done = FALSE;  /* added outer loop for XML pop not in */
				/* AsnTypeSetIndent */
                while (! done)
		{
		if (AsnFindBaseIsa(aip->typestack[aip->type_indent - 1].type) 
            == CHOICE_TYPE)
		{
			atp2 = aip->typestack[aip->type_indent - 1].type;
			if (aip->type_indent >= 2)
				isa = AsnFindBaseIsa(aip->typestack[aip->type_indent-2].type);
			else
				isa = NULL_TYPE;    /* just fake it */
			if (((isa != SETOF_TYPE) && (isa != SEQOF_TYPE)) ||
				(isXML))
				AsnPrintIndent(FALSE, aip);
			AsnTypeSetIndent(FALSE, aip, atp);
			if(isXML)
			{
				AsnPrintNewLine(aip);
				AsnXMLTerm(aip, atp2);
			}
			else
				done = TRUE;
		}
		else
			done = TRUE;
		}
	}
	return TRUE;														   
}

NLM_EXTERN Boolean LIBCALL  AsnTxtWrite (AsnIoPtr aip, AsnTypePtr atp, DataValPtr dvp)
{
	return AsnTxtWriteEx (aip, atp, dvp, NULL);
}

/*****************************************************************************
*
*   void AsnPrintModule(amp, aip)
*
*****************************************************************************/
NLM_EXTERN void AsnPrintModule (AsnModulePtr amp, AsnIoPtr aip)

{
	AsnTypePtr atp;
	Boolean firstone;
	CharPtr from;

	aip->token = ISMODULE_TOKEN;   /* signal to AsnPrintIndent */
	AsnPrintString(amp->modulename, aip);
	AsnPrintString(" DEFINITIONS ::=", aip);
	AsnPrintNewLine(aip);
	AsnPrintString("BEGIN", aip);
	AsnPrintNewLine(aip);
	AsnPrintNewLine(aip);

	atp = amp->types;		    /* check for EXPORTS */
	firstone = TRUE;
	while (atp != NULL)
	{
		if (atp->exported == TRUE)
		{
			if (firstone)
				AsnPrintString("EXPORTS ", aip);
			else
			{
				AsnPrintString(" ,", aip);
				AsnPrintNewLine(aip);
				AsnPrintString("        ", aip);
			}
			AsnPrintString(atp->name, aip);
			firstone = FALSE;
		}
		atp = atp->next;
	}
	if (! firstone)            /* got at least one */
	{
		AsnPrintString(" ;", aip);
		AsnPrintNewLine(aip);
		AsnPrintNewLine(aip);
	}

	atp = amp->types;		    /* check for IMPORTS */
	firstone = TRUE;
	from = NULL;
	while (atp != NULL)
	{
		if (atp->imported == TRUE)
		{
			if (firstone)
				AsnPrintString("IMPORTS ", aip);
			else
			{
				if (StringCmp((CharPtr) atp->branch, from))    /* new FROM */
				{
					AsnPrintString(" FROM ", aip);
					AsnPrintString(from, aip);
				}
				else
					AsnPrintString(" ,", aip);
				AsnPrintNewLine(aip);
				AsnPrintString("        ", aip);
			}
			AsnPrintString(atp->name, aip);
			firstone = FALSE;
			from = (CharPtr) atp->branch;
		}
		atp = atp->next;
	}
	if (! firstone)            /* got at least one */
	{
		AsnPrintString(" FROM ", aip);
		AsnPrintString(from, aip);
		AsnPrintString(" ;", aip);
		AsnPrintNewLine(aip);
		AsnPrintNewLine(aip);
	}

	atp = amp->types;
	while (atp != NULL)
	{
		if (! atp->imported)
		{
			AsnPrintString(atp->name, aip);
			AsnPrintString(" ::= ", aip);
			AsnPrintType(atp, aip);
			AsnPrintNewLine(aip);
			AsnPrintNewLine(aip);
		}
		atp = atp->next;
	}
	AsnPrintString("END", aip);
	AsnPrintNewLine(aip);
	return;
}

/*****************************************************************************
*
*   void AsnPrintType(atp, aip)
*   	prints a type starting at current line position
*   	(assumes name already printed)
*
*****************************************************************************/
NLM_EXTERN void AsnPrintType (AsnTypePtr atp, AsnIoPtr aip)

{
	AsnValxNodePtr avnp;
	AsnTypePtr atp2;
	Boolean first;

	if (atp->tagclass != TAG_NONE)     /* print tag, if any */
	{
		AsnPrintChar('[', aip);
		AsnPrintChar(' ', aip);
		switch (atp->tagclass)
		{
			case TAG_UNIVERSAL:
				AsnPrintString("UNIVERSAL ", aip);
				break;
			case TAG_APPLICATION:
				AsnPrintString("APPLICATION ", aip);
				break;
			case TAG_PRIVATE:
				AsnPrintString("PRIVATE ", aip);
				break;
			default:      /* context dependent, do nothing */
				break;
		}
		AsnPrintInteger((Int4)atp->tagnumber, aip);
		AsnPrintChar(' ', aip);
		AsnPrintChar(']', aip);
		AsnPrintChar(' ', aip);

		if (atp->implicit)
			AsnPrintString("IMPLICIT ", aip);
	}

	AsnPrintString(atp->type->name, aip);   /* print the type name */

	if (atp->branch != NULL)       /* sub types ? */
	{
		switch (atp->type->isa)
		{
			case SETOF_TYPE:
			case SEQOF_TYPE:
				AsnPrintChar(' ', aip);
				AsnPrintType((AsnTypePtr) atp->branch, aip);
				break;
			case INTEGER_TYPE:
			case ENUM_TYPE:
				AsnPrintChar(' ', aip);
				AsnPrintOpenStruct(aip, atp);
				avnp = (AsnValxNodePtr)atp->branch;
				first = TRUE;
				aip->first[aip->indent_level] = FALSE;
				while (avnp != NULL)
				{
					if (! first)
						AsnPrintNewLine(aip);
					else
						first = FALSE;
					AsnPrintString(avnp->name, aip);
					AsnPrintChar(' ', aip);
					AsnPrintChar('(', aip);
					AsnPrintInteger(avnp->intvalue, aip);
					AsnPrintChar(')', aip);
					avnp = avnp->next;
				}
				AsnPrintCloseStruct(aip, atp);
				break;
			case SEQ_TYPE:
			case SET_TYPE:
			case CHOICE_TYPE:
				AsnPrintChar(' ', aip);
				AsnPrintOpenStruct(aip, atp);
				atp2 = (AsnTypePtr) atp->branch;
				first = TRUE;
				aip->first[aip->indent_level] = FALSE;
				while (atp2 != NULL)
				{
					if (! first)
						AsnPrintNewLine(aip);
					else
						first = FALSE;

					if (atp2->name != NULL)
					{
						AsnPrintString(atp2->name, aip);
						AsnPrintChar(' ', aip);
					}
					AsnPrintType(atp2, aip);
					atp2 = atp2->next;
				}
				AsnPrintCloseStruct(aip, atp);
				break;
			default:			/* everything else */
				break;          /* do nothing */
		}
	}
	
	if (atp->optional)
		AsnPrintString(" OPTIONAL", aip);

	if (atp->hasdefault)
	{
		AsnPrintString(" DEFAULT ", aip);
		avnp = atp->defaultvalue;
		while (! (VALUE_ISA_DEFAULT(avnp->valueisa)))
			avnp = avnp->next;
		switch (avnp->valueisa)
		{
			case VALUE_ISA_PTR:
				AsnPrintChar('\"', aip);
				AsnPrintString(avnp->name, aip);
				AsnPrintChar('\"', aip);
				break;
			case VALUE_ISA_BOOL:
				AsnPrintBoolean((Boolean)avnp->intvalue, aip);
				break;
			case VALUE_ISA_INT:
				AsnPrintInteger(avnp->intvalue, aip);
				break;
			case VALUE_ISA_REAL:
				AsnPrintReal(avnp->realvalue, aip);
				break;
			default:
				AsnPrintString("Error", aip);
				break;
		}
	}
}

/*****************************************************************************
*
*   Boolean AsnPrintStrStore(bsp, aip)
*
*****************************************************************************/
NLM_EXTERN Boolean AsnPrintStrStore (ByteStorePtr bsp, AsnIoPtr aip)

{
	Char buf[101];
	Uint4 len, tlen;

	if (aip->type & ASNIO_CARRIER)           /* pure iterator */
		return TRUE;

	BSSeek(bsp, 0, SEEK_SET);      /* seek to start */
	len = BSLen(bsp);
	if(!(aip->type & ASNIO_XML))
	    AsnPrintChar('\"', aip);
	while (len)
	{
		if (len < 100)
			tlen = len;
		else
			tlen = 100;
		BSRead(bsp, buf, tlen);
		buf[tlen] = '\0';
		if (! AsnPrintString(buf, aip))
			return FALSE;
		len -= tlen;
	}
	if(!(aip->type & ASNIO_XML))
	    AsnPrintChar('\"', aip);
	return TRUE;
}
/*****************************************************************************
*
*   void AsnPrintReal(realvalue, aip)
*
*****************************************************************************/
NLM_EXTERN void AsnPrintReal (FloatHi realvalue, AsnIoPtr aip)

{
	FloatHi thelog, mantissa;
	int characteristic;
	int	ic;
	long	im;
	char tbuf[30];
	Boolean minus;

	if (aip->type & ASNIO_CARRIER)           /* pure iterator */
		return;

	if (aip->type & ASNIO_XML)
	{
		sprintf(tbuf, "%g", (double)realvalue);
		AsnPrintString(tbuf, aip);
		return;
	}
		

	if (realvalue == 0.0)
	{
		ic = 0;
		im = 0;
	}
	else
	{
		if (realvalue < 0.0)
		{
			minus = TRUE;
			realvalue = -realvalue;
		}
		else
			minus = FALSE;

		thelog = log10((double)realvalue);
		if (thelog >= 0.0)
			characteristic = 8 - (int)thelog;/* give it 9 significant digits */
		else
			characteristic = 8 + (int)ceil(-thelog);

		mantissa = realvalue * Nlm_Powi((double)10., characteristic);
		ic = -characteristic; /* reverse direction */

                if(mantissa >= 1.79e+308)
                    im = INT_MAX;
                else
                    im = (long) mantissa;
                

		/* strip trailing 0 */
		while ((im % 10L) == 0L)
		{
			im /= 10L;
			ic++;
		}

		if (minus)
			im = -im;
	}
	sprintf(tbuf, "{ %ld, 10, %d }", im, ic);
	AsnPrintString(tbuf, aip);
	return;
}

/*****************************************************************************
*
*   void AsnPrintInteger(theInt, aip)
*
*****************************************************************************/
NLM_EXTERN void AsnPrintInteger (Int4 theInt, AsnIoPtr aip)

{
	char tbuf[20];

	if (aip->type & ASNIO_CARRIER)           /* pure iterator */
		return;

	sprintf(tbuf, "%ld", (long)theInt);
	AsnPrintString(tbuf, aip);
	return;
}

/*****************************************************************************
*
*   void AsnPrintBigInt(theInt, aip)
*
*****************************************************************************/
NLM_EXTERN void AsnPrintBigInt (Int8 theInt, AsnIoPtr aip)

{
	char tbuf[40];

	if (aip->type & ASNIO_CARRIER)           /* pure iterator */
		return;

	Int8ToString (theInt, tbuf, (size_t) 40);
	AsnPrintString(tbuf, aip);
	return;
}

/*****************************************************************************
*
*   void AsnPrintChar(theChar, aip)
*   	print a single character
*
*****************************************************************************/
NLM_EXTERN void AsnPrintChar (char theChar, AsnIoPtr aip)

{
	if (aip->type & ASNIO_CARRIER)           /* pure iterator */
		return;

	*(aip->linebuf + aip->linepos) = theChar;
	aip->linepos++;
	aip->offset++;
	return;
}

/*****************************************************************************
*
*   void AsnPrintBoolean(value, aip)
*
*****************************************************************************/
NLM_EXTERN void AsnPrintBoolean (Boolean value, AsnIoPtr aip)

{
	if (aip->type & ASNIO_CARRIER)           /* pure iterator */
		return;

	if (aip->type & ASNIO_XML)
	{
		AsnPrintString(" value=", aip);
		AsnPrintChar('\"', aip);
		if (value)
			AsnPrintString("true", aip);
		else
			AsnPrintString("false", aip);
		AsnPrintChar('\"', aip);
	}
	else
	{
		if (value)
			AsnPrintString("TRUE", aip);
		else
			AsnPrintString("FALSE", aip);
	}
	return;
}

/*****************************************************************************
*
*   void AsnPrintOctets(ssp, aip)
*
*****************************************************************************/
NLM_EXTERN void AsnPrintOctets (ByteStorePtr ssp, AsnIoPtr aip)

{
	Int2 value, tval, ctr;
	Char buf[101];

	if (aip->type & ASNIO_CARRIER)           /* pure iterator */
		return;

	if (! (aip->type & ASNIO_XML))
		AsnPrintChar('\'', aip);

	BSSeek(ssp, 0, SEEK_SET);   /* go to start of bytestore */
	ctr = 0;
	buf[100] = '\0';

					/* break it up into lines if necessary */
	while ((value = BSGetByte(ssp)) != -1)
	{
		tval = value / 16;
		if (tval < 10)
			buf[ctr] = (Char)(tval + '0');
		else
			buf[ctr] = (Char)(tval - 10 + 'A');
		ctr++;
		tval = value - (tval * 16);
		if (tval < 10)
			buf[ctr] = (Char)(tval + '0');
		else
			buf[ctr] = (Char)(tval - 10 + 'A');
		ctr++;
		if (ctr == 100)
		{
		    AsnPrintString(buf, aip);
			ctr = 0;
		}
	}
	if (ctr)
	{
		buf[ctr] = '\0';
		AsnPrintString(buf, aip);
	}

	if (! (aip->type & ASNIO_XML))
	{
		AsnPrintChar('\'', aip);
		AsnPrintChar('H', aip);
	}
	return;
}

/*****************************************************************************
*
*   void AsnPrintIndent(increase, aip)
*      increase or decrease indent level
*
*****************************************************************************/
NLM_EXTERN void AsnPrintIndent (Boolean increase, AsnIoPtr aip)

{
	Int1    offset, curr_indent;
	BoolPtr tmp;
	int     decr, isa;
	
	if (increase)
    {
        aip->indent_level++;
        curr_indent = aip->indent_level;
        if (curr_indent == aip->max_indent)   /* expand indent levels */
        {
            tmp = aip->first;
            aip->first = (BoolPtr) MemNew((sizeof(Boolean) *
                                           (aip->max_indent + 10)));
            MemCopy(aip->first, tmp, (size_t)(sizeof(Boolean) * 
                                              aip->max_indent));
            MemFree(tmp);
            aip->max_indent += 10;
        }
        aip->first[curr_indent] = TRUE;     /* set to first time */
        offset = curr_indent * aip->tabsize;
        
        if (! (aip->type & ASNIO_CARRIER))
        {
            while (aip->linepos < offset)
            {
                *(aip->linebuf + aip->linepos) = ' ';
                aip->linepos++;
            }
            aip->offset = aip->linepos + (aip->linebuf - (CharPtr)aip->buf);
        }
    }
	else
    {
        offset = aip->indent_level * aip->tabsize;
        curr_indent = aip->type_indent;
        decr = 1;   /* always backup indent for named element */
        do
        {
            if (aip->indent_level)
                aip->indent_level -= decr;
            if (curr_indent)
                curr_indent--;
            isa = NULL_TYPE;        /* fake key */
            if ((aip->indent_level) && (curr_indent))
            {
                isa = AsnFindBaseIsa(aip->typestack[curr_indent - 1].type);
                if (aip->typestack[curr_indent-1].type->name != NULL)
                    decr = 1;       /* indent for named choices as elements */
                else
                    decr = 0;       /* not referenced choice objects */
            }
        } while ((isa == CHOICE_TYPE) && (aip->token != ISMODULE_TOKEN) &&
                 (! (aip->type & ASNIO_XML)));
        
        if (aip->linepos == offset)    /* nothing written yet */
        {
            curr_indent = aip->indent_level * aip->tabsize;
            while (offset >= curr_indent)
            {
                offset--;
                if (! (aip->type & ASNIO_CARRIER))
                {
                    if ((offset >= 0) && (aip->linebuf[offset] != ' '))
                        curr_indent = 127;
                }
            }
            offset++;
            aip->linepos = offset;
            aip->offset = aip->linepos + (aip->linebuf - (CharPtr)aip->buf);
        }
        if (! aip->indent_level)   /* level 0 - no commas */
            aip->first[0] = TRUE;
    }
	return;
}

/*****************************************************************************
*
*   void AsnPrintNewLine(aip)
*       end a line in the print buffer
*       indent to the proper level on the next line
*
*****************************************************************************/
NLM_EXTERN void AsnPrintNewLine (AsnIoPtr aip)

{
	Int1    tpos, indent;
	CharPtr tmp;
	Boolean do_print = TRUE;

	if (aip->linepos == 0)     /* nothing in buffer yet */
    {
        if ((aip->type & ASNIO_XML) && (aip->token == ISMODULE_TOKEN))
        {            /* print blank line */
            tmp = aip->linebuf;
            *tmp = ' ';
            tmp++;
            *tmp = '\0';
            aip->linepos = tmp - aip->linebuf;
            aip->offset = tmp - (CharPtr)aip->buf;
        }
        else
        {
            aip->no_newline = FALSE;      /* reset to normal */
            return;
        }
    }
		
	if (! (aip->type & ASNIO_CARRIER))    /* really printing */
    {
        tpos = aip->indent_level * aip->tabsize;
        if (tpos == aip->linepos)         /* just an empty indent? */
        {
            if (! ((aip->type & ASNIO_XML) && (aip->token == ISMODULE_TOKEN)))
                do_print = FALSE;         /* assume that's the case */
            for (tmp = aip->linebuf; tpos != 0; tpos--, tmp++)
            {
                if (*tmp != ' ')
                {
                    do_print = TRUE;      /* set sentinel */
                    break;
                }
            }
        }

        if (do_print)   /* not an empty indent */
        {
            tmp = aip->linebuf + aip->linepos;
            
            /* not first line of struct */
            if (aip->first[aip->indent_level] == FALSE)
            {
                if (!(aip->type & ASNIO_XML))   /* add commas */
                {
                    *tmp = ' '; tmp++;
                    *tmp = ','; tmp++;
                }
            }
            else if (!aip->no_newline && aip->linepos)
            /* is first line, remove trailing blanks if just indented */
            {
                tmp--;
                while ((*tmp == ' ') && (tmp > aip->linebuf))
                    tmp--;
                tmp++;
            }
            if ((aip->type & ASNIO_XML) && 
                (aip->token == ISMODULE_TOKEN) && (tmp == aip->linebuf))
            {   /* print an empty line for formatting */
                *tmp = ' ';
                tmp++;
            }
            *tmp = '\0';
            aip->linepos = tmp - aip->linebuf;
            aip->offset = tmp - (CharPtr)aip->buf;
            
            AsnIoPuts(aip);
        }
    }
    
	if ((do_print) && (aip->indent_level))       /* level 0 never has commas */
		aip->first[aip->indent_level] = FALSE;

	if (! (aip->type & ASNIO_CARRIER)) /* really printing */
    {
         tmp = aip->linebuf;
         indent = aip->indent_level * aip->tabsize;
         for (tpos = 0; tpos < indent; tpos++, tmp++)
             *tmp = ' ';
         aip->linepos = tpos;
         aip->offset += tpos;
    }
	aip->no_newline = FALSE;   /* reset to normal */
	return;
}


/*  XML transliteration table
 */

typedef struct {
    const char*  str;
    unsigned int len;
} SXmlChar;

static const SXmlChar s_XmlChar[256] =
{
    { "&#0;",    4 },   /*   0 */
    { "&#1;",    4 },   /*   1 */
    { "&#2;",    4 },   /*   2 */
    { "&#3;",    4 },   /*   3 */
    { "&#4;",    4 },   /*   4 */
    { "&#5;",    4 },   /*   5 */
    { "&#6;",    4 },   /*   6 */
    { "&#7;",    4 },   /*   7 */
    { "&#8;",    4 },   /*   8 */
    { "&#9;",    4 },   /*   9 */
    { "&#10;",   5 },   /*  10 */
    { "&#11;",   5 },   /*  11 */
    { "&#12;",   5 },   /*  12 */
    { "&#13;",   5 },   /*  13 */
    { "&#14;",   5 },   /*  14 */
    { "&#15;",   5 },   /*  15 */
    { "&#16;",   5 },   /*  16 */
    { "&#17;",   5 },   /*  17 */
    { "&#18;",   5 },   /*  18 */
    { "&#19;",   5 },   /*  19 */
    { "&#20;",   5 },   /*  20 */
    { "&#21;",   5 },   /*  21 */
    { "&#22;",   5 },   /*  22 */
    { "&#23;",   5 },   /*  23 */
    { "&#24;",   5 },   /*  24 */
    { "&#25;",   5 },   /*  25 */
    { "&#26;",   5 },   /*  26 */
    { "&#27;",   5 },   /*  27 */
    { "&#28;",   5 },   /*  28 */
    { "&#29;",   5 },   /*  29 */
    { "&#30;",   5 },   /*  30 */
    { "&#31;",   5 },   /*  31 */
    { " ",       1 },   /*  32 */
    { "!",       1 },   /*  33 */
    { "&quot;",  6 },   /*  34 */
    { "#",       1 },   /*  35 */
    { "$",       1 },   /*  36 */
    { "%",       1 },   /*  37 */
    { "&amp;",   5 },   /*  38 */
    { "&apos;",  6 },   /*  39 */
    { "(",       1 },   /*  40 */
    { ")",       1 },   /*  41 */
    { "*",       1 },   /*  42 */
    { "+",       1 },   /*  43 */
    { ",",       1 },   /*  44 */
    { "-",       1 },   /*  45 */
    { ".",       1 },   /*  46 */
    { "/",       1 },   /*  47 */
    { "0",       1 },   /*  48 */
    { "1",       1 },   /*  49 */
    { "2",       1 },   /*  50 */
    { "3",       1 },   /*  51 */
    { "4",       1 },   /*  52 */
    { "5",       1 },   /*  53 */
    { "6",       1 },   /*  54 */
    { "7",       1 },   /*  55 */
    { "8",       1 },   /*  56 */
    { "9",       1 },   /*  57 */
    { ":",       1 },   /*  58 */
    { ";",       1 },   /*  59 */
    { "&lt;",    4 },   /*  60 */
    { "=",       1 },   /*  61 */
    { "&gt;",    4 },   /*  62 */
    { "?",       1 },   /*  63 */
    { "@",       1 },   /*  64 */
    { "A",       1 },   /*  65 */
    { "B",       1 },   /*  66 */
    { "C",       1 },   /*  67 */
    { "D",       1 },   /*  68 */
    { "E",       1 },   /*  69 */
    { "F",       1 },   /*  70 */
    { "G",       1 },   /*  71 */
    { "H",       1 },   /*  72 */
    { "I",       1 },   /*  73 */
    { "J",       1 },   /*  74 */
    { "K",       1 },   /*  75 */
    { "L",       1 },   /*  76 */
    { "M",       1 },   /*  77 */
    { "N",       1 },   /*  78 */
    { "O",       1 },   /*  79 */
    { "P",       1 },   /*  80 */
    { "Q",       1 },   /*  81 */
    { "R",       1 },   /*  82 */
    { "S",       1 },   /*  83 */
    { "T",       1 },   /*  84 */
    { "U",       1 },   /*  85 */
    { "V",       1 },   /*  86 */
    { "W",       1 },   /*  87 */
    { "X",       1 },   /*  88 */
    { "Y",       1 },   /*  89 */
    { "Z",       1 },   /*  90 */
    { "[",       1 },   /*  91 */
    { "\\",      1 },   /*  92 */
    { "]",       1 },   /*  93 */
    { "^",       1 },   /*  94 */
    { "_",       1 },   /*  95 */
    { "`",       1 },   /*  96 */
    { "a",       1 },   /*  97 */
    { "b",       1 },   /*  98 */
    { "c",       1 },   /*  99 */
    { "d",       1 },   /* 100 */
    { "e",       1 },   /* 101 */
    { "f",       1 },   /* 102 */
    { "g",       1 },   /* 103 */
    { "h",       1 },   /* 104 */
    { "i",       1 },   /* 105 */
    { "j",       1 },   /* 106 */
    { "k",       1 },   /* 107 */
    { "l",       1 },   /* 108 */
    { "m",       1 },   /* 109 */
    { "n",       1 },   /* 110 */
    { "o",       1 },   /* 111 */
    { "p",       1 },   /* 112 */
    { "q",       1 },   /* 113 */
    { "r",       1 },   /* 114 */
    { "s",       1 },   /* 115 */
    { "t",       1 },   /* 116 */
    { "u",       1 },   /* 117 */
    { "v",       1 },   /* 118 */
    { "w",       1 },   /* 119 */
    { "x",       1 },   /* 120 */
    { "y",       1 },   /* 121 */
    { "z",       1 },   /* 122 */
    { "{",       1 },   /* 123 */
    { "|",       1 },   /* 124 */
    { "}",       1 },   /* 125 */
    { "~",       1 },   /* 126 */
    { "&#127;",  6 },   /* 127 */
    { "&#128;",  6 },   /* 128 */
    { "&#129;",  6 },   /* 129 */
    { "&#130;",  6 },   /* 130 */
    { "&#131;",  6 },   /* 131 */
    { "&#132;",  6 },   /* 132 */
    { "&#133;",  6 },   /* 133 */
    { "&#134;",  6 },   /* 134 */
    { "&#135;",  6 },   /* 135 */
    { "&#136;",  6 },   /* 136 */
    { "&#137;",  6 },   /* 137 */
    { "&#138;",  6 },   /* 138 */
    { "&#139;",  6 },   /* 139 */
    { "&#140;",  6 },   /* 140 */
    { "&#141;",  6 },   /* 141 */
    { "&#142;",  6 },   /* 142 */
    { "&#143;",  6 },   /* 143 */
    { "&#144;",  6 },   /* 144 */
    { "&#145;",  6 },   /* 145 */
    { "&#146;",  6 },   /* 146 */
    { "&#147;",  6 },   /* 147 */
    { "&#148;",  6 },   /* 148 */
    { "&#149;",  6 },   /* 149 */
    { "&#150;",  6 },   /* 150 */
    { "&#151;",  6 },   /* 151 */
    { "&#152;",  6 },   /* 152 */
    { "&#153;",  6 },   /* 153 */
    { "&#154;",  6 },   /* 154 */
    { "&#155;",  6 },   /* 155 */
    { "&#156;",  6 },   /* 156 */
    { "&#157;",  6 },   /* 157 */
    { "&#158;",  6 },   /* 158 */
    { "&#159;",  6 },   /* 159 */
    { "&#160;",  6 },   /* 160 */
    { "&#161;",  6 },   /* 161 */
    { "&#162;",  6 },   /* 162 */
    { "&#163;",  6 },   /* 163 */
    { "&#164;",  6 },   /* 164 */
    { "&#165;",  6 },   /* 165 */
    { "&#166;",  6 },   /* 166 */
    { "&#167;",  6 },   /* 167 */
    { "&#168;",  6 },   /* 168 */
    { "&#169;",  6 },   /* 169 */
    { "&#170;",  6 },   /* 170 */
    { "&#171;",  6 },   /* 171 */
    { "&#172;",  6 },   /* 172 */
    { "&#173;",  6 },   /* 173 */
    { "&#174;",  6 },   /* 174 */
    { "&#175;",  6 },   /* 175 */
    { "&#176;",  6 },   /* 176 */
    { "&#177;",  6 },   /* 177 */
    { "&#178;",  6 },   /* 178 */
    { "&#179;",  6 },   /* 179 */
    { "&#180;",  6 },   /* 180 */
    { "&#181;",  6 },   /* 181 */
    { "&#182;",  6 },   /* 182 */
    { "&#183;",  6 },   /* 183 */
    { "&#184;",  6 },   /* 184 */
    { "&#185;",  6 },   /* 185 */
    { "&#186;",  6 },   /* 186 */
    { "&#187;",  6 },   /* 187 */
    { "&#188;",  6 },   /* 188 */
    { "&#189;",  6 },   /* 189 */
    { "&#190;",  6 },   /* 190 */
    { "&#191;",  6 },   /* 191 */
    { "&#192;",  6 },   /* 192 */
    { "&#193;",  6 },   /* 193 */
    { "&#194;",  6 },   /* 194 */
    { "&#195;",  6 },   /* 195 */
    { "&#196;",  6 },   /* 196 */
    { "&#197;",  6 },   /* 197 */
    { "&#198;",  6 },   /* 198 */
    { "&#199;",  6 },   /* 199 */
    { "&#200;",  6 },   /* 200 */
    { "&#201;",  6 },   /* 201 */
    { "&#202;",  6 },   /* 202 */
    { "&#203;",  6 },   /* 203 */
    { "&#204;",  6 },   /* 204 */
    { "&#205;",  6 },   /* 205 */
    { "&#206;",  6 },   /* 206 */
    { "&#207;",  6 },   /* 207 */
    { "&#208;",  6 },   /* 208 */
    { "&#209;",  6 },   /* 209 */
    { "&#210;",  6 },   /* 210 */
    { "&#211;",  6 },   /* 211 */
    { "&#212;",  6 },   /* 212 */
    { "&#213;",  6 },   /* 213 */
    { "&#214;",  6 },   /* 214 */
    { "&#215;",  6 },   /* 215 */
    { "&#216;",  6 },   /* 216 */
    { "&#217;",  6 },   /* 217 */
    { "&#218;",  6 },   /* 218 */
    { "&#219;",  6 },   /* 219 */
    { "&#220;",  6 },   /* 220 */
    { "&#221;",  6 },   /* 221 */
    { "&#222;",  6 },   /* 222 */
    { "&#223;",  6 },   /* 223 */
    { "&#224;",  6 },   /* 224 */
    { "&#225;",  6 },   /* 225 */
    { "&#226;",  6 },   /* 226 */
    { "&#227;",  6 },   /* 227 */
    { "&#228;",  6 },   /* 228 */
    { "&#229;",  6 },   /* 229 */
    { "&#230;",  6 },   /* 230 */
    { "&#231;",  6 },   /* 231 */
    { "&#232;",  6 },   /* 232 */
    { "&#233;",  6 },   /* 233 */
    { "&#234;",  6 },   /* 234 */
    { "&#235;",  6 },   /* 235 */
    { "&#236;",  6 },   /* 236 */
    { "&#237;",  6 },   /* 237 */
    { "&#238;",  6 },   /* 238 */
    { "&#239;",  6 },   /* 239 */
    { "&#240;",  6 },   /* 240 */
    { "&#241;",  6 },   /* 241 */
    { "&#242;",  6 },   /* 242 */
    { "&#243;",  6 },   /* 243 */
    { "&#244;",  6 },   /* 244 */
    { "&#245;",  6 },   /* 245 */
    { "&#246;",  6 },   /* 246 */
    { "&#247;",  6 },   /* 247 */
    { "&#248;",  6 },   /* 248 */
    { "&#249;",  6 },   /* 249 */
    { "&#250;",  6 },   /* 250 */
    { "&#251;",  6 },   /* 251 */
    { "&#252;",  6 },   /* 252 */
    { "&#253;",  6 },   /* 253 */
    { "&#254;",  6 },   /* 254 */
    { "&#255;",  6 }    /* 255 */
};


/*****************************************************************************
*
*   Boolean AsnPrintString(str, aip)
*
*****************************************************************************/
NLM_EXTERN Boolean AsnPrintString (CharPtr the_string, AsnIoPtr aip)
{
	register CharPtr current, previous, current_save = NULL, str, str_begin;
	Boolean          isXML = FALSE;
	Boolean          indent_state;
	int              bad_char = 0, bad_char_ctr = 0;
    int              linepos_start = aip->linepos;

	if (the_string == NULL) {
		AsnIoErrorMsg(aip, 106, bad_char, "");
		return FALSE;
	}

    if (aip->type & ASNIO_XML)
		isXML = TRUE;

	if (aip->type & ASNIO_CARRIER) {
        /* Post error if non-printing chars */
        if ((aip->fix_non_print == 0) || (aip->fix_non_print == 3)) {
            for (str = the_string; *str != '\0'; str++) {
                if ((*str < ' ') || (*str > '~')) {
                    bad_char_ctr++;
                    bad_char = (int)(*str);
                }
            }
        }
        goto ret;
    }

	str = the_string;
	indent_state = aip->first[aip->indent_level];
    
    /* Get current ptr to output */
    current = aip->linebuf + aip->linepos;
    str_begin = current;

    /* Break it up into lines and convert chars if necessary */
	while (*str)  {
        char ch = *str;
        previous = current;

        /* Fix non print char if necessary */
        if (aip->fix_non_print != 2  &&  (ch < ' '  ||  ch > '~')) {
            if ( !bad_char_ctr ) {
                bad_char = (int) ch;
            }
            bad_char_ctr++;
            ch = '#';   /* replace with # */
        }
        
        /*  XML and double quotes fixes */
        if ( isXML ) {
            const SXmlChar* xml_char = &s_XmlChar[(unsigned int) ch];
            if (xml_char->len == 1) {
                *current++ = ch;
            } else {
                current = StringMove(current, xml_char->str);
            }
        } else {
            *current++ = ch;
            if (ch == '\"') {   /* must double quotes */
                *current++ = ch;
            }
        }

        /* Correct position in the buffer */
        aip->linepos += (current - previous);
        aip->offset  += (current - previous);

        /* Check to necessety a new line */

        if ( (current >= aip->linebuf + aip->linelength)  && *(str+1) )  {

            /* Get word break */

            size_t pos;
            int cnt_save;
            
            *current = *(str+1);
            *(current+1) = 0;
            pos = AsnPrintGetWordBreak(str_begin, 
                                       aip->linelength - linepos_start);

            /* Save rest of string */

            cnt_save = current - str_begin - pos;
            if ( cnt_save < 0 ) 
                cnt_save = 0;

            if ( cnt_save ) {
                if ( !current_save )
                    current_save = MemNew(aip->linelength);
                if ( !current_save ) 
                    return FALSE;
                MemCopy(current_save, str_begin + pos, cnt_save);
                aip->linepos -= cnt_save;
                aip->offset  -= cnt_save;
            }

            /* Print new line */

            aip->first[aip->indent_level] = TRUE;   /* no commas */
            if ( isXML )                /* don't split XML lines */
                aip->no_newline = TRUE;
            AsnPrintNewLine(aip);       
            aip->offset = aip->offset - aip->linepos + cnt_save;
            aip->linepos = cnt_save;
            linepos_start = 0;
            
            /* Restore saved string and calculate current ptr to output */
            if ( current_save ) {
                MemCopy(aip->linebuf, current_save, cnt_save);
            }
            current = aip->linebuf + aip->linepos;
            str_begin = aip->linebuf;
        }

        /* Move to next char */ 
        str++;
    }
    
    /* Reset indent state */
	aip->first[aip->indent_level] = indent_state;

ret:
    if ( current_save )
        MemFree(current_save);

    /* Check on error */
	if ((bad_char_ctr) && 
        ((aip->fix_non_print == 0) || (aip->fix_non_print == 3))) {
        AsnIoErrorMsg(aip, 106, bad_char, the_string);
    }
	return TRUE;
}

/*****************************************************************************
*
*   Boolean AsnPrintStream(str, aip, stream)
*
*****************************************************************************/
NLM_EXTERN Boolean AsnPrintStream (Pointer object, AsnIoPtr aip, AsnStreamStringFunc stream)

{
	if (aip == NULL) return FALSE;
	if (object == NULL || stream == NULL) return FALSE;

	return stream (object, aip);
}

/*****************************************************************************
*
*   void AsnPrintCharBlock(str, aip)
*      prints string on line if there is room
*      if not prints on next line with no indent.
*
*      Now only used by XML printing
*
*****************************************************************************/
NLM_EXTERN void AsnPrintCharBlock (CharPtr str, AsnIoPtr aip)

{
	Uint4 stringlen;
	Boolean indent_state;
	Int1 templen;
	CharPtr current;

	if (aip->type & ASNIO_CARRIER)           /* pure iterator */
		return;

	stringlen = StringLen(str);
	templen = (Int1)(aip->linelength - aip->linepos);
	indent_state = aip->first[aip->indent_level];

	if (!(aip->type & ASNIO_XML) && (stringlen > (Uint4)templen)) 
    {   /* won't fit on line -- don't split XML lines */
        aip->first[aip->indent_level] = TRUE;   /* no commas */
        AsnPrintNewLine(aip);       /* this call resets no_newline itself */
        aip->offset -= aip->linepos;
        aip->linepos = 0;
    }

	current = aip->linebuf + aip->linepos;
	MemCopy(current, str, (size_t)stringlen);
	aip->linepos += (Int2) stringlen;
	aip->offset += (Int2) stringlen;
	aip->first[aip->indent_level] = indent_state;   /* reset indent state */
	return;
}

/*****************************************************************************
*
*   int AsnPrintGetWordBreak(str, maxlen)
*       return length (<= maxlen) of str to next white space
*
*****************************************************************************/
NLM_EXTERN int AsnPrintGetWordBreak (CharPtr str, int maxlen)

{
	CharPtr tmp;
	int len;
	Uint4 stringlen;

	stringlen = StringLen(str);
	if (stringlen <= (Uint4)maxlen)
		return (int) stringlen;

	tmp = str + maxlen;    /* point just PAST the end of region */
	len = maxlen + 1;
	while ((len) && (! IS_WHITESP(*tmp)))
        {
            len--; tmp--;
        }
	while ((len) && (IS_WHITESP(*tmp)))
        {
            len--;             /* move past white space */
            tmp--;
        }
	if (len < 1)         /* never found any whitespace or only 1 space */
		len = maxlen;    /* have to break a word */

	return len;
}

/*****************************************************************************
*
*   AsnPrintOpenStruct(aip, atp)
*
*****************************************************************************/
NLM_EXTERN void AsnPrintOpenStruct (AsnIoPtr aip, AsnTypePtr atp)

{
    if(aip->type & ASNIO_XML) {
        /* rework for indent? */
    	AsnPrintIndent(TRUE, aip);
    	AsnTypeSetIndent(TRUE, aip, atp);
    	AsnPrintNewLine(aip);
    	aip->first[aip->indent_level] = TRUE;
    } else {
    	AsnPrintChar('{', aip);
    	AsnPrintIndent(TRUE, aip);
    	AsnTypeSetIndent(TRUE, aip, atp);
    	AsnPrintNewLine(aip);
    	aip->first[aip->indent_level] = TRUE;
    }
    return;
}

/*****************************************************************************
*
*   AsnPrintCloseStruct(aip, atp)
*
*****************************************************************************/
NLM_EXTERN void AsnPrintCloseStruct (AsnIoPtr aip, AsnTypePtr atp)

{
	AsnTypePtr atp2;
	Int2 isa;

    if(aip->type & ASNIO_XML) {
    	AsnPrintIndent(FALSE, aip);
	    AsnTypeSetIndent(FALSE, aip, atp);
		AsnPrintNewLine(aip);
        AsnXMLTerm(aip, atp);
		while ((aip->type_indent) && 
               (AsnFindBaseIsa(aip->typestack[aip->type_indent - 1].type) 
                == CHOICE_TYPE))
   		{   /* pop out of choice nests */
			atp2 = aip->typestack[aip->type_indent - 1].type;
            if (aip->type_indent >= 2)
                isa = AsnFindBaseIsa(aip->typestack[aip->type_indent - 2].type);
            else
                isa = NULL_TYPE;
            
            AsnPrintIndent(FALSE, aip);
            
            AsnTypeSetIndent(FALSE, aip, atp);
            AsnPrintNewLine(aip);
            AsnXMLTerm(aip, atp2);
            
		}
    } else {
	    AsnPrintChar(' ', aip);
	    AsnPrintChar('}', aip);
	    AsnPrintIndent(FALSE, aip);
	    AsnTypeSetIndent(FALSE, aip, atp);
    }
	return;
}

static void AsnXMLCommentBegin(AsnIoPtr aip)
{
	AsnPrintCharBlock("<!-- ", aip);
	return;
}

static void AsnXMLCommentEnd(AsnIoPtr aip)
{
	AsnPrintCharBlock(" -->", aip);
	AsnPrintNewLine(aip);
	return;
}

static void AsnXMLElementStart(AsnTypePtr atp, AsnIoPtr aip)
{
	AsnTypePtr atp2;
	Int2 isa;
	AsnOptionPtr aop;
	Boolean didone = FALSE, doenum = FALSE, first = TRUE;
	AsnValxNodePtr avnp;

	aop = NULL;
	while ((aop = AsnOptionGet(atp->hints, OP_COMMENT, 0, aop)) != NULL)
	{
		if (first)
		{
			first = FALSE;
			AsnPrintNewLine(aip);
			AsnXMLCommentBegin(aip);
			AsnPrintNewLine(aip);
		}
		AsnPrintCharBlock((CharPtr)(aop->data.ptrvalue), aip);
		AsnPrintNewLine(aip);
		didone = TRUE;
	}
	
	isa = AsnFindBaseIsa(atp);
	if ((atp->branch != NULL) && (isa == INTEGER_TYPE) && (isa == ENUM_TYPE))
	{
		for (avnp = (AsnValxNodePtr)(atp->branch); 
             avnp != NULL; 
             avnp = avnp->next)
		{
			if (avnp->aop != NULL)  /* comments here */
			{
				doenum = TRUE;
				break;
			}
		}
		if (doenum)
		{
			for (avnp = (AsnValxNodePtr)(atp->branch); 
                 avnp != NULL; 
                 avnp = avnp->next)
			{
				if (avnp->name != NULL)
				{
					if (first)
					{
						AsnXMLCommentBegin(aip);
						AsnPrintNewLine(aip);
						first = FALSE;
					}
					AsnPrintString(avnp->name, aip);
					if (avnp->aop != NULL)
					{
						AsnPrintChar(' ', aip);
						AsnPrintCharBlock((CharPtr)(avnp->aop->data.ptrvalue),
                                          aip);
					}
					AsnPrintNewLine(aip);
					didone = TRUE;
				}
			}
		}
	}
	if (didone)
	{
		AsnXMLCommentEnd(aip);
	}

	AsnPrintCharBlock("<!ELEMENT ", aip);

	AsnPrintString(GetXMLname(atp), aip);

	atp2 = AsnFindBaseTypeDTD(atp);
	if (atp2 == atp)
	{
		if ((isa == ENUM_TYPE) || (isa == BOOLEAN_TYPE) ||
		    (isa == NULL_TYPE))
		{
			AsnPrintString(" ", aip); /* EMPTY tag here */
			return;
		}
	}
	AsnPrintString(" ( ", aip);
	return;
}

static void AsnXMLElementAdd(CharPtr prev, AsnTypePtr atp, CharPtr repeat,
			     AsnIoPtr aip, Boolean do_newline)
{
	Char buf[100];
	CharPtr tmp;

	tmp = buf;
	*tmp = '\0';

	if (prev != NULL)
	{
		tmp = StringMove(tmp, " ");
		tmp = StringMove(tmp, prev);
		if (! do_newline)
			tmp = StringMove(tmp, " ");

		AsnPrintCharBlock(buf, aip);
	}

	if (do_newline)
	{
		AsnPrintNewLine(aip);
		tmp = StringMove(buf, "               ");
	}
	tmp = StringMove(tmp, GetXMLname(atp));

	if (repeat != NULL)
		tmp = StringMove(tmp, repeat);

        AsnPrintString(buf, aip);
	return;
}

static void AsnXMLElementEnd(AsnIoPtr aip)
{
	AsnPrintCharBlock(" )>", aip);
	AsnPrintNewLine(aip);
	return;
}

static void AsnPrintSubTypeXML(AsnTypePtr atp, AsnIoPtr aip)
{
	AsnTypePtr atp2, atp3;
	Int2 isa;
	Boolean doit = FALSE;

	if (atp == NULL) return;
	if (atp->branch == NULL) return;

	isa = atp->type->isa;
	if ((isa == SEQ_TYPE) || (isa == SET_TYPE) || (isa == CHOICE_TYPE))
		doit = TRUE;
	else if ((isa == SEQOF_TYPE) || (isa == SETOF_TYPE))
	{
		atp2 = (AsnTypePtr)(atp->branch);
		atp3 = AsnFindBaseType(atp2);
		if (atp2 == atp3)
			doit = TRUE;
	}

	if (doit)
	{
		for (atp2 = (AsnTypePtr)(atp->branch); atp2 != NULL; atp2 = atp2->next)
		{
			AsnXMLElementStart(atp2, aip);
			if (AsnPrintTypeXML(atp2, aip))
				AsnXMLElementEnd(aip);
		}
			/* check for nested CHOICE  */
		for (atp2 = (AsnTypePtr)(atp->branch); atp2 != NULL; atp2 = atp2->next)
		{
			isa = AsnFindBaseIsa(atp2);
			atp3 = AsnFindBaseType(atp2);
			if (((isa == CHOICE_TYPE) ||
			     (isa == SEQOF_TYPE) || (isa == SETOF_TYPE) ||
			     (isa == SEQ_TYPE) || (isa == SET_TYPE)))
			{
/*				atp3 = (AsnTypePtr)(atp2->branch);
				if ((atp3 != NULL) && (ISA_BASETYPE(atp3->type->isa)))
				{ */
					AsnPrintNewLine(aip);
					AsnPrintSubTypeXML(atp2, aip);
			/*	} */
			}
		}

	}
	return;
	
}

static void AsnPrintTopTypeXML (AsnIoPtr aip, AsnTypePtr atp)
{

	AsnXMLCommentBegin(aip);
	AsnPrintString("Definition of ", aip);
	AsnPrintString(GetXMLname(atp), aip);
	AsnXMLCommentEnd(aip);
	AsnPrintNewLine(aip);
	AsnXMLElementStart(atp, aip);
	if (AsnPrintTypeXML(atp, aip))
		AsnXMLElementEnd(aip);
	AsnPrintNewLine(aip);
	AsnPrintSubTypeXML(atp, aip);
	AsnPrintNewLine(aip);
	AsnPrintNewLine(aip);
	return;
}

#define AMPMAX 50

typedef struct modulelist {
	Int2 num;    /* total number in list */
	AsnModulePtr amps[AMPMAX];
}ModuleList, PNTR ModuleListPtr;

static AsnModulePtr AsnModuleFind (CharPtr modname)
{
	AsnModulePtr amp;

	for (amp = AsnAllModPtr(); amp != NULL; amp = amp->next)
	{
		if (! StringCmp(amp->modulename, modname))
			return amp;
	}
	return amp;
}

static void AsnModuleGetList (AsnModulePtr amp, ModuleListPtr mlp)
{
	Int2 i;
	AsnTypePtr atp;
	CharPtr from;
	AsnModulePtr newmod;

	for (i = 0; i < mlp->num; i++)
	{
		if (mlp->amps[i] == amp)  /* already have it */
			return;
	}

	if (mlp->num == AMPMAX)
	{
		ErrPostEx(SEV_ERROR,0,0,"Exceeded %d amps in AsnModuleGetList",
			  (int)(mlp->num));
		return;
	}

	mlp->amps[mlp->num] = amp;
	mlp->num++;

	atp = amp->types;		    /* check for IMPORTS */
	from = NULL;
	while (atp != NULL)
	{
		if (atp->imported == TRUE)
		{
			if (StringCmp((CharPtr) atp->branch, from))    /* new FROM */
			{
				from = (CharPtr)(atp->branch);
				newmod = AsnModuleFind(from);
				if (newmod != NULL)
					AsnModuleGetList(newmod, mlp);
				else
					ErrPostEx(SEV_ERROR,0,0,"Can't find module %s",
						from);
			}
		}
		atp = atp->next;
	}

	return;
	
}

/**************************************************************************
*
*  AsnPrintModuleXMLInc (amp, name)
*    prints the ENTITY includes for a DTD
*
**************************************************************************/
NLM_EXTERN void AsnPrintModuleXMLInc (AsnModulePtr amp, CharPtr name)
{
	ModuleList ml;
	CharPtr ptr;
	Char buf[200], buf2[200];
	FILE * fp;
	Int2 i;
	static char * f1 = "\n<!ENTITY %% %s_module PUBLIC \"-//NCBI//%s Module//EN\" \"%s%s.mod\">\n";
	static char * f2 = "%%%s_module;\n";

	ml.num = 0;

	AsnModuleGetList(amp, &ml);

	fp = FileOpen(name, "w");

	fprintf(fp, "<!-- %s\n", name);
	fprintf(fp, "  This file is built from a series of basic modules.\n");
	fprintf(fp, "  The actual ELEMENT and ENTITY declarations are in the modules.\n");
        fprintf(fp, "  This file is used to put them together.\n");
	fprintf(fp, "-->\n");

	fprintf(fp, f1, "NCBI_Entity", "NCBI Entity",AsnGetXMLmodulePrefix(),  "NCBI_Entity");
	fprintf(fp, f2, "NCBI_Entity");

	for (i = 0; i < ml.num; i++)
	{
		ptr = MakeXMLModuleName(ml.amps[i], buf);
		ptr = MakeXMLPublicName(buf2, buf);
		
		fprintf(fp, f1, buf, buf2, AsnGetXMLmodulePrefix(), buf);
		fprintf(fp, f2, buf);
	}

	FileClose(fp);
	return;
}

/*****************************************************************************
*
*   void AsnPrintModuleXML(amp, aip)
*
*****************************************************************************/
NLM_EXTERN void AsnPrintModuleXML (AsnModulePtr amp, AsnIoPtr aip)

{
	AsnTypePtr atp;
	Boolean firstone, done;
	CharPtr from;
	AsnOptionPtr aop;
	Int4 ctr, highest;
	

	if (! aip->XMLModuleWritten)   /* first module to XML DTD? */
	{
		aip->XMLModuleWritten = TRUE;
		aip->token = ISMODULE_TOKEN;   /* signal to AsnPrintIndent */
	
		/**** now a separate module **
		AsnPrintCharBlock("<!-- ======================== -->", aip);
		AsnPrintNewLine(aip);
		AsnPrintCharBlock("<!-- NCBI DTD                 -->", aip);
		AsnPrintNewLine(aip);
		AsnPrintCharBlock("<!-- NCBI ASN.1 mapped to XML -->", aip);
		AsnPrintNewLine(aip);
		AsnPrintCharBlock("<!-- ======================== -->", aip);
		AsnPrintNewLine(aip);
		AsnPrintNewLine(aip);
		AsnPrintCharBlock("<!-- Entities used to give specificity to #PCDATA -->", aip);
		AsnPrintNewLine(aip);
		AsnPrintCharBlock("<!ENTITY % INTEGER '#PCDATA'>", aip);
		AsnPrintNewLine(aip);
		AsnPrintCharBlock("<!ENTITY % ENUM 'EMPTY'>", aip);
		AsnPrintNewLine(aip);
		AsnPrintCharBlock("<!ENTITY % BOOLEAN 'EMPTY'>", aip);
		AsnPrintNewLine(aip);
		AsnPrintCharBlock("<!ENTITY % NULL 'EMPTY'>", aip);
		AsnPrintNewLine(aip);
		AsnPrintCharBlock("<!ENTITY % REAL '#PCDATA'>", aip);
		AsnPrintNewLine(aip);
		AsnPrintCharBlock("<!ENTITY % OCTETS '#PCDATA'>", aip);
		AsnPrintNewLine(aip);
		AsnPrintCharBlock("<!-- ============================================ -->", aip);
		AsnPrintNewLine(aip);
		AsnPrintNewLine(aip);
		*****************************/

	}

	AsnPrintCharBlock("<!-- ============================================ -->", aip);
	AsnPrintNewLine(aip);
	AsnXMLCommentBegin(aip);
	AsnPrintString("This section mapped from ASN.1 module ", aip);
	AsnPrintString(amp->modulename, aip);
	AsnXMLCommentEnd(aip);
	AsnPrintNewLine(aip);


	atp = amp->types;		    /* check for EXPORTS */
	firstone = TRUE;
	while (atp != NULL)
	{
		if (atp->exported == TRUE)
		{
			if (firstone)
			{
				AsnXMLCommentBegin(aip);
				AsnPrintString("Elements used by other modules:", aip);
				AsnPrintNewLine(aip);
			}
			else
			{
				AsnPrintString(" ,", aip);
				AsnPrintNewLine(aip);
			}
			AsnPrintString("          ", aip);
			AsnPrintString(GetXMLname(atp), aip);
			firstone = FALSE;
		}
		atp = atp->next;
	}
	if (! firstone)            /* got at least one */
	{
		AsnXMLCommentEnd( aip);
		AsnPrintNewLine(aip);
		AsnPrintNewLine(aip);
	}

	atp = amp->types;		    /* check for IMPORTS */
	firstone = TRUE;
	from = NULL;
	while (atp != NULL)
	{
		if (atp->imported == TRUE)
		{
			if (firstone)
			{
				AsnXMLCommentBegin(aip);
				AsnPrintString("Elements referenced from other modules:  ", 
                               aip);
				AsnPrintNewLine(aip);
			}
			else
			{
				if (StringCmp((CharPtr) atp->branch, from))    /* new FROM */
				{
					AsnPrintString(" FROM ", aip);
					AsnPrintString(from, aip);
				}
				else
					AsnPrintString(" ,", aip);
				AsnPrintNewLine(aip);
			}
			AsnPrintString("          ", aip);
			AsnPrintString(GetXMLname(atp), aip);
			firstone = FALSE;
			from = (CharPtr) atp->branch;
		}
		atp = atp->next;
	}
	if (! firstone)            /* got at least one */
	{
		AsnPrintString(" FROM ", aip);
		AsnPrintString(from, aip);
		AsnXMLCommentEnd(aip);
		AsnPrintNewLine(aip);
		AsnPrintNewLine(aip);
	}

	AsnPrintCharBlock("<!-- ============================================ -->", aip);
	AsnPrintNewLine(aip);

	aop = NULL;    /* check for order information */
	highest = 0;
	for (atp = amp->types; atp != NULL; atp = atp->next)
	{
		aop = AsnOptionGet(atp->hints, OP_TYPEORDER, 0, NULL);
		if (aop != NULL)
		{
			if (aop->data.intvalue > highest)
				highest = aop->data.intvalue;
		}
	}

	if (highest)     /* we do have ordering info */
	{
		for (ctr = 1; ctr <= highest; ctr++)
		{
			done = FALSE;
			for (atp = amp->types;
			     (! done) && ( atp != NULL); atp = atp->next)
			{
				aop = AsnOptionGet(atp->hints, OP_TYPEORDER,
						   0, NULL);
				if (aop != NULL)
				{
					if (aop->data.intvalue == ctr)
					{
						AsnPrintTopTypeXML(aip, atp);
						done = TRUE;
					}
				}
			}
		}
	}
	else             /* nope, just do the default order */
	{
		atp = amp->types;
		while (atp != NULL)
		{
			if (! atp->imported)
			{
				AsnPrintTopTypeXML(aip, atp);
			}
		}
		atp = atp->next;
	}

	AsnPrintNewLine(aip);
	return;
}

/*****************************************************************************
*
*   Boolean AsnPrintTypeXML(atp, aip)
*   	prints a type starting at current line position
*   	(assumes name already printed)
*       returns TRUE if AsnElementXMLEnd() needs to be called
*
*****************************************************************************/
static Boolean AsnPrintTypeXML (AsnTypePtr atp, AsnIoPtr aip)

{
	AsnValxNodePtr avnp;
	AsnTypePtr atp2;
	Boolean first;
	Int2 isa;
	Char buf[100], tbuf[20];
	CharPtr tmp, repeat = NULL;
	Boolean retval = TRUE, gotone, isimplied = FALSE;

	isa = atp->type->isa;
	tmp = buf;
	*tmp = '\0';
	gotone = FALSE;    /* assume no default attribute found */

	if (atp->branch != NULL)       /* sub types ? */
	{
		switch (isa)
		{
			case SETOF_TYPE:
			case SEQOF_TYPE:
				if ((atp->optional) || (atp->hasdefault))
					repeat = "*";
				else
					repeat = "*";
//					repeat = "+";
				AsnXMLElementAdd(NULL, atp->branch, repeat, aip, FALSE);
				break;
			case INTEGER_TYPE:
				tmp = StringMove(tmp, "%INTEGER;");
				AsnPrintString(buf, aip);
				AsnXMLElementEnd(aip);
				isimplied = TRUE;   /* don't need named attributes */
			case ENUM_TYPE:
				if (*buf == '\0')   /* not INTEGER */
				{
					tmp = StringMove(tmp, "%ENUM; >");
					AsnPrintCharBlock(buf, aip);
					AsnPrintNewLine(aip);
				}

				tmp = buf;
				AsnPrintCharBlock("<!ATTLIST", aip);
				tmp = StringMove(tmp, " ");
				tmp = StringMove(tmp, GetXMLname(atp));
				tmp = StringMove(tmp, " value ( ");
				AsnPrintString(buf, aip);
				avnp = (AsnValxNodePtr)atp->branch;
				first = TRUE;
				/*
				aip->first[aip->indent_level] = FALSE;
				*/
				while (avnp != NULL)
				{
					tmp = buf;
					*tmp = '\0';
					if (! first)
						AsnPrintCharBlock(" |", aip);
					else
						first = FALSE;
					AsnPrintNewLine(aip);
					tmp = StringMove(tmp, "               ");
					if (avnp->name != NULL)
						tmp = StringMove(tmp, avnp->name);
					else
					{
						sprintf(tbuf, "%d", (int)(avnp->intvalue));
						tmp = StringMove(tmp, tbuf);
					}
					AsnPrintString(buf, aip);
					avnp = avnp->next;
				}
				AsnPrintCharBlock(" ) ", aip);
				if (atp->hasdefault)
				{
					tmp = buf;
					*tmp = '\0';
					avnp = atp->defaultvalue;
					while (! (VALUE_ISA_DEFAULT(avnp->valueisa)))
						avnp = avnp->next;
					switch (avnp->valueisa)
					{
						case VALUE_ISA_PTR:
							AsnPrintChar('\"', aip);
							AsnPrintString(avnp->name, aip);
							AsnPrintChar('\"', aip);
							gotone = TRUE;
							break;
						default:
							break;
					}
				}
				if (! gotone)    /* no default value */
				{
					if (isimplied)
						AsnPrintString(" #IMPLIED", aip);
					else
						AsnPrintString(" #REQUIRED", aip);
				}
				AsnPrintCharBlock(" >", aip);
				AsnPrintNewLine(aip);
				retval = FALSE;
				break;
			case SEQ_TYPE:
			case SET_TYPE:
				atp2 = (AsnTypePtr) atp->branch;
				first = TRUE;
				aip->first[aip->indent_level] = FALSE;
				tmp = NULL;
				while (atp2 != NULL)
				{
					if (! first)
						tmp = ",";
					else
						first = FALSE;
					
					if ((atp2->optional) || (atp2->hasdefault))
						repeat = "?";
					else
						repeat = NULL;

					AsnXMLElementAdd(tmp, atp2, repeat, aip, TRUE);

					atp2 = atp2->next;
				}

				break;
			case CHOICE_TYPE:
				atp2 = (AsnTypePtr) atp->branch;
				first = TRUE;
				aip->first[aip->indent_level] = FALSE;
				tmp = NULL;
				while (atp2 != NULL)
				{
					if (! first)
						tmp = "|";
					else
						first = FALSE;
					
					if ((atp2->optional) || (atp2->hasdefault))
						repeat = "?";
					else
						repeat = NULL;

					AsnXMLElementAdd(tmp, atp2, repeat, aip, TRUE);

					atp2 = atp2->next;
				}

				break;
			default:			/* everything else */
				AsnPrintString(" DEFAULT_BRANCH ", aip);
				break;          /* do nothing */
		}
	}
	else  /* primitive.. all text */
	{
		atp2 = AsnFindBaseTypeDTD(atp);
		if (atp2 != NULL)
		{
			isa = AsnFindBaseIsa(atp);
			if (atp2 == atp)   /* this is the bottom */
			{
				switch (isa)
				{
					case NULL_TYPE:
						tmp = StringMove(tmp, "%NULL; >");
						AsnPrintCharBlock(buf, aip);
						AsnPrintNewLine(aip);
						return FALSE;

					case BOOLEAN_TYPE:
						tmp = StringMove(tmp, "%BOOLEAN; >");
						AsnPrintCharBlock(buf, aip);
						AsnPrintNewLine(aip);

						tmp = buf;
						*tmp = '\0';
						AsnPrintCharBlock("<!ATTLIST ", aip);
						tmp = StringMove(tmp, GetXMLname(atp));
						tmp = StringMove(tmp, " value ( true | false ) ");
						AsnPrintString(buf, aip);
						gotone = FALSE;
						if (atp->hasdefault)
						{
							avnp = atp->defaultvalue;
							while (! (VALUE_ISA_DEFAULT(avnp->valueisa)))
								avnp = avnp->next;
							switch (avnp->valueisa)
							{
								case VALUE_ISA_BOOL:
									AsnPrintChar('\"', aip);
									if (avnp->intvalue)
										AsnPrintString("true",aip);
									else
										AsnPrintString("false",aip);
									AsnPrintChar('\"', aip);
									gotone = TRUE;
									break;
								default:
									break;
							}
						}
						if (! gotone)
							AsnPrintString(" #REQUIRED", aip);
						AsnPrintCharBlock(" >", aip);
						AsnPrintNewLine(aip);
						*buf = '\0';
						retval = FALSE;
						break;
					case INTEGER_TYPE:
						tmp = StringMove(tmp, "%INTEGER;");
						break;
					case BIGINT_TYPE:
						tmp = StringMove(tmp, "%INTEGER;");
						break;
					case REAL_TYPE:
						tmp = StringMove(tmp, "%REAL;");
						break;
					case OCTETS_TYPE:
						tmp = StringMove(tmp, "%OCTETS;");
						break;
					default:
						tmp = StringMove(tmp, "#PCDATA");
						break;
				}
			}
			else
			{
				tmp = StringMove(tmp, GetXMLname(atp2));
			}
		}
		else
			tmp = StringMove(tmp, "#PCDATA");

		if (*buf != '\0')
			AsnPrintString(buf, aip);
	}
	return retval;
}

static void AsnPrintTreeType(Int2 indent, AsnTypePtr atp, CharPtr prefix, FILE *fp)
{
	Int2 isa;
	Uint4 ptr;
	CharPtr tmp;
	Char buf[1000];
	Int2 i;
	AsnTypePtr atp2;

	if (atp == NULL) return;

	ptr = (Uint4)(atp);
	isa = atp->isa;

	for (i = 0; i < indent; i++)
	{
		buf[i] = '.';
	}
	tmp = buf + indent;
	*tmp = '\0';
	fprintf(fp, "%s", buf);
	if ((indent) && (atp->hints != NULL))  /* primary def */
		fprintf(fp, "#>");
	if (atp->been_here)
		fprintf(fp,"&>");

	if (prefix != NULL)
		fprintf(fp,"%s", prefix);
	fprintf(fp,"[%lu] ", (unsigned long)ptr);
	fprintf(fp,"%d ", (int)isa);
	if (atp->name == NULL)
		fprintf(fp,"(null) ");
	else
		fprintf(fp,"(%s) ", atp->name);
	if (atp->XMLname == NULL)
		fprintf(fp,"|null| ");
	else
		fprintf(fp,"|%s| ", atp->XMLname);
	if (atp->type != NULL)
		fprintf(fp,"type ");
	else
		fprintf(fp,"notype ");
	if (atp->imported)
		fprintf(fp,"import ");
	else
		fprintf(fp,"notimport ");
	if (atp->branch != NULL)
		fprintf(fp,"branch\n");
	else
		fprintf(fp,"nobranch\n");

	if (atp->been_here)
		return;

	if ((indent) && (atp->hints != NULL))  /* primary def */
		return;

	atp->been_here = TRUE;
	AsnPrintTreeType((Int2)(indent + 1), atp->type, "TYPE=", fp);

	isa = AsnFindBaseIsa(atp);
	if (((isa == SEQOF_TYPE) || (isa == SETOF_TYPE) ||
	    (isa == SEQ_TYPE) || (isa == SET_TYPE) ||
	    (isa == CHOICE_TYPE) || (isa > 400)) && (! atp->imported))
	for (atp2 = (AsnTypePtr)(atp->branch); atp2 != NULL; atp2 = atp2->next)
		AsnPrintTreeType((Int2)(indent + 1), atp2, "Branch=", fp);

	atp->been_here = FALSE;  /* just prevent recursion */
	return;

}

NLM_EXTERN void AsnPrintTreeModule(AsnModulePtr amp, FILE * fp)
{
	Int4 highest, ctr;
	AsnOptionPtr aop;
	AsnTypePtr atp;
	Boolean done;

	aop = NULL;    /* check for order information */
	highest = 0;
	for (atp = amp->types; atp != NULL; atp = atp->next)
	{
		aop = AsnOptionGet(atp->hints, OP_TYPEORDER, 0, NULL);
		if (aop != NULL)
		{
			if (aop->data.intvalue > highest)
				highest = aop->data.intvalue;
		}
	}

	if (highest)     /* we do have ordering info */
	{
		for (ctr = 1; ctr <= highest; ctr++)
		{
			done = FALSE;
			for (atp = amp->types;
			     (! done) && ( atp != NULL); atp = atp->next)
			{
				aop = AsnOptionGet(atp->hints, OP_TYPEORDER,
						   0, NULL);
				if (aop != NULL)
				{
					if (aop->data.intvalue == ctr)
					{
						AsnPrintTreeType((Int2)0, atp, NULL, fp);
						fprintf(fp,"\n");
						done = TRUE;
					}
				}
			}
		}
	}

}
