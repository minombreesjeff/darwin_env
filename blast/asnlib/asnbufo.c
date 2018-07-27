/* asnbufo.c
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
* File Name:  asnbufo.c
*
* Author:  Warren Gish
*
* Version Creation Date: 5/25/94
*
* $Revision: 6.0 $
*
* File Description:
*   Routines to output a buffer in ASN.1
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: asnbufo.c,v $
* Revision 6.0  1997/08/25 18:09:37  madden
* Revision changed to 6.0
*
* Revision 5.1  1996/12/03 21:43:48  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
 * Revision 5.0  1996/05/28  14:00:29  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:47:38  ostell
 * force revision to 4.0
 *
 * Revision 1.3  1995/05/15  18:38:28  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/
#include <ncbi.h>
#include "asnbuild.h"

/*****************************************************************************
*
*   int AsnBufGetWordBreak(str, maxlen)
*       return length (<= maxlen) of str to next white space
*
*****************************************************************************/
static size_t LIBCALL AsnBufGetWordBreak (CharPtr str, size_t stringlen, size_t maxlen)

{
	register CharPtr tmp;
	register size_t len;

	if (stringlen <= maxlen)
		return stringlen;

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
*   void AsnPrintBuf(buf, buflen, aip)
*
*****************************************************************************/
static void LIBCALL AsnPrintBuf (CharPtr buf, size_t buflen, AsnIoPtr aip)
{
	register size_t templen;
	Boolean	first = TRUE;
	register CharPtr current, bufmax;
	Boolean indent_state;

	if (aip->type & ASNIO_CARRIER)           /* pure iterator */
		return;

	indent_state = aip->first[aip->indent_level];

		/* break it up into lines if necessary */
	while (buflen) {
		if (! first) {             /* into multiple lines */
			aip->first[aip->indent_level] = TRUE;   /* no commas */
			AsnPrintNewLine(aip);
			aip->offset -= aip->linepos;
			aip->linepos = 0;
		}
		first = FALSE;

		templen = aip->linelength - aip->linepos;

		if (buflen <= templen)     /* it fits in remaining space */
			templen = buflen;
		else
			templen = AsnBufGetWordBreak(buf, buflen, templen);

		current = aip->linebuf + aip->linepos;
		buflen -= templen;
		aip->linepos += templen;
		aip->offset += templen;
		bufmax = buf + templen;
		while (buf < bufmax) {
			*current = *buf++;
			if (*current++ == '\"') {	/* must double quotes */
				aip->linepos++;
				aip->offset++;
				*current++ = '\"';
			}
		}
	}
	aip->first[aip->indent_level] = indent_state;   /* reset indent state */
	return;
}

/*****************************************************************************
*
*   void AsnPrintBufOctets(buf, buflen, aip)
*
*****************************************************************************/
static void LIBCALL AsnPrintBufOctets (register CharPtr buf, size_t buflen, AsnIoPtr aip)
{
	register unsigned	value, tval;
	register int	ctr = 0;
	register CharPtr	bufmax;
	char	obuf[100];

	if (aip->type & ASNIO_CARRIER)           /* pure iterator */
		return;

	AsnPrintChar('\'', aip);

		/* break it up into lines if necessary */
	bufmax = buf + buflen;
	for (bufmax = buf + buflen; buf < bufmax; ++buf) {
		value = *(unsigned char *)buf;
		tval = value / 16;
		if (tval < 10)
			obuf[ctr] = (char)(tval + '0');
		else
			obuf[ctr] = (char)(tval + ('A' - 10));
		++ctr;
		tval = value & 0x0f;
		if (tval < 10)
			obuf[ctr] = (char)(tval + '0');
		else
			obuf[ctr] = (char)(tval + ('A' - 10));
		++ctr;
		if (ctr == DIM(obuf)) {
		    AsnPrintBuf(obuf, ctr, aip);
			ctr = 0;
		}
	}
	if (ctr > 0)
		AsnPrintBuf(obuf, ctr, aip);

	AsnPrintChar('\'', aip);
	AsnPrintChar('H', aip);
	return;
}

/*****************************************************************************
*
*   void AsnTxtBufWrite(aip, atp, buf, buflen)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL AsnTxtBufWrite (AsnIoPtr aip, AsnTypePtr atp, CharPtr buf, size_t buflen)
{
	Int2 isa;
	AsnTypePtr atp2;
	Boolean firstvalue;

	if ((! aip->indent_level) && (aip->typestack[0].type == NULL))
		firstvalue = TRUE;    /* first call to this routine */
	else
		firstvalue = FALSE;

	atp2 = AsnFindBaseType(atp);
	isa = atp2->type->isa;
	if (ISA_STRINGTYPE(isa))
		isa = GENERALSTRING_TYPE;

	switch (isa) {
	case GENERALSTRING_TYPE:
	case OCTETS_TYPE:
	case STRSTORE_TYPE:
		break;
	default:
		AsnIoErrorMsg(aip, 19, AsnErrGetTypeName(atp->name));
		return FALSE;
	}
	
	if (! AsnTypeValidateOut(aip, atp, NULL))
		return FALSE;

	if (! aip->first[aip->indent_level])
		AsnPrintNewLine(aip);
	else
		aip->first[aip->indent_level] = FALSE;

	atp2 = atp;
	if (firstvalue) {	/* first item, need ::= */
		while ((atp2->name == NULL) || (IS_LOWER(*atp2->name)))
			atp2 = atp2->type;    /* find a Type Reference */
	}

	if (atp2->name != NULL) {
	 	AsnPrintString(atp2->name, aip);	/* put the element name */
		if (IS_LOWER(*atp2->name))
		 	AsnPrintChar(' ', aip);
		else
			AsnPrintString(" ::= ", aip);
	}

	switch (isa) {
	case GENERALSTRING_TYPE:
	case STRSTORE_TYPE:
		AsnPrintChar('\"', aip);
		AsnPrintBuf(buf, buflen, aip);
		AsnPrintChar('\"', aip);
		break;
	case OCTETS_TYPE:
		AsnPrintBufOctets(buf, buflen, aip);
		break;
	default:
		AsnIoErrorMsg(aip, 19, AsnErrGetTypeName(atp->name));
		return FALSE;
	}

	if (aip->type_indent) { /* pop out of choice nests */
		if (AsnFindBaseIsa(aip->typestack[aip->type_indent - 1].type) == CHOICE_TYPE) {
			if (aip->type_indent >= 2)
				isa = AsnFindBaseIsa(aip->typestack[aip->type_indent - 2].type);
			else
				isa = NULL_TYPE;    /* just fake it */
			AsnPrintIndent(FALSE, aip);
			AsnTypeSetIndent(FALSE, aip, atp);
		}
	}
	return TRUE;														   
}

/*****************************************************************************
*
*   void AsnEnBinBuf(buf, buflen, aip)
*
*****************************************************************************/
static void LIBCALL AsnEnBinBuf (CharPtr buf, size_t buflen, AsnIoPtr aip, AsnTypePtr atp)
{
	AsnTypePtr atp2;

	atp2 = AsnFindBaseType(atp);
	atp2 = atp2->type;
	AsnEnBinTags(atp2, aip);

	AsnEnBinLen((Uint4)buflen, aip);

	AsnEnBinBytes(buf, (Uint4)buflen, aip);
	return;
}

/*****************************************************************************
*
*   Boolean AsnBinBufWrite(aip, atp, buf, buflen)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL AsnBinBufWrite (AsnIoPtr aip, AsnTypePtr atp, CharPtr buf, size_t buflen)
{
	int	isa;
	AsnTypePtr atp2;
	int next_type;

	if (! AsnTypeValidateOut(aip, atp, NULL))
		return FALSE;

	atp2 = AsnFindBaseType(atp);
	isa = atp2->type->isa;
	if (ISA_STRINGTYPE(isa))
		isa = GENERALSTRING_TYPE;
	
	AsnEnBinTags(atp, aip);	/* put in the tags */

	switch (isa) {
	case GENERALSTRING_TYPE:
	case OCTETS_TYPE:
	case STRSTORE_TYPE:
		AsnEnBinBuf(buf, buflen, aip, atp);
		break;
	default:
		AsnIoErrorMsg(aip, 19, AsnErrGetTypeName(atp->name));
		return FALSE;
	}

	if (atp->tagclass != TAG_NONE)
		AsnEnBinEndIndef(aip);    /* put indefinite encoding */
	next_type = aip->type_indent - 1;   /* end any choices */
	while ((next_type >= 0) &&
			(AsnFindBaseIsa(aip->typestack[next_type].type) == CHOICE_TYPE)) {
		if (aip->typestack[next_type].type->tagclass != TAG_NONE)
			AsnEnBinEndIndef(aip);
		next_type--;
	}
	if (AsnFindBaseIsa(aip->typestack[aip->type_indent - 1].type) == CHOICE_TYPE)
		AsnTypeSetIndent(FALSE, aip, atp);
	return TRUE;
}

/*****************************************************************************
*
*   AsnBufWrite()
*   	generalized write buffer
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL AsnBufWrite (AsnIoPtr aip, AsnTypePtr atp, CharPtr buf, size_t buflen)
{
	Boolean retval = FALSE;
	
	if (aip->aeop != NULL)    /* exploring nodes */
		AsnCheckExpOpt(aip, atp, NULL);

	if (aip->type & ASNIO_TEXT)
		retval = AsnTxtBufWrite(aip, atp, buf, buflen);
	else if (aip->type & ASNIO_BIN)
		retval = AsnBinBufWrite(aip, atp, buf, buflen);

	if (aip->io_failure)
		return FALSE;
	return retval;
}
