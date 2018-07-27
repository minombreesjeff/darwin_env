/* asnenbin.c
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
* File Name: asnenbin.c
*
* Author:  James Ostell
*
* Version Creation Date: 3/4/91
*
* $Revision: 6.6 $
*
* File Description:
*   Special binary (BER) encoder for ASN.1
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 3/4/91   Kans        Stricter typecasting for GNU C and C++
* 04-20-93 Schuler     LIBCALL calling convention
*
* $Log: asnenbin.c,v $
* Revision 6.6  2003/11/07 20:57:02  beloslyu
* fix the c++ style comments to a C ones
*
* Revision 6.5  2003/08/01 16:55:24  gouriano
* workaround MSVC++.NET optimization bug
*
* Revision 6.4  2000/12/12 15:56:11  ostell
* added support BigInt
*
* Revision 6.3  1998/06/12 19:27:49  kans
* fixed unix compiler warnings
*
* Revision 6.2  1998/05/02 18:54:25  kans
* for Mac, use %LG instead of %lg, which is not legal C, and not supported by CodeWarrior
*
* Revision 6.1  1998/05/01 23:35:18  kans
* use %lf on Mac, %lg elsewhere, sprintf bug reported
*
* Revision 6.0  1997/08/25 18:09:50  madden
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
 * Revision 2.9  1995/05/15  18:38:28  ostell
 * added Log line
 *
*
* ==========================================================================
*/

#include "asnbuild.h"

/*****************************************************************************
*
*   Boolean AsnBinWrite(aip, atp, valueptr)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL  AsnBinWrite (AsnIoPtr aip, AsnTypePtr atp, DataValPtr dvp)
{
	Int2 isa;
	AsnTypePtr atp2;
	Boolean terminalvalue;
	int next_type;

	terminalvalue = TRUE;   /* most are terminal values */

	if (! AsnTypeValidateOut(aip, atp, dvp))
		return FALSE;

	atp2 = AsnFindBaseType(atp);
	isa = atp2->type->isa;
	if (ISA_STRINGTYPE(isa))
		isa = GENERALSTRING_TYPE;
	
	if (((isa == SEQ_TYPE) || (isa == SET_TYPE) ||
		 (isa == SEQOF_TYPE) || (isa == SETOF_TYPE))
		 && (dvp->intvalue == END_STRUCT))
	{
		AsnEnBinCloseStruct(aip, atp);
		return TRUE;
	}

	AsnEnBinTags(atp, aip);     /* put in the tags */

	if (isa == CHOICE_TYPE)     /* show nothing */
	{
		AsnTypeSetIndent(TRUE, aip, atp);
		return TRUE;
	}

	switch (isa)
	{
		case SEQ_TYPE:
		case SET_TYPE:
		case SEQOF_TYPE:
		case SETOF_TYPE:
			if (dvp->intvalue == START_STRUCT)   /* open brace */
				AsnEnBinOpenStruct(aip, atp);
			else
			{
				AsnIoErrorMsg(aip, 18 );
				return FALSE;
			}
			terminalvalue = FALSE;
			break;
		case BOOLEAN_TYPE:
			AsnEnBinBoolean(dvp->boolvalue, aip, atp);
			break;
		case INTEGER_TYPE:
		case ENUM_TYPE:
			AsnEnBinInteger(dvp->intvalue, aip, atp);
			break;
		case BIGINT_TYPE:
			AsnEnBinBigInt(dvp->bigintvalue, aip, atp);
			break;
		case REAL_TYPE:
			AsnEnBinReal(dvp->realvalue, aip, atp);
			break;
		case GENERALSTRING_TYPE:
			if (! AsnEnBinString((CharPtr) dvp->ptrvalue, aip, atp))
				return FALSE;
			break;
		case NULL_TYPE:
			AsnEnBinNull(aip, atp);
			break;
		case OCTETS_TYPE:
		case STRSTORE_TYPE:
			AsnEnBinOctets((ByteStorePtr) dvp->ptrvalue, aip, atp);
			break;
		default:
			AsnIoErrorMsg(aip, 19, AsnErrGetTypeName(atp->name));
			return FALSE;
	}

	if (terminalvalue)          /* pop out of choice nests */
	{
		if (atp->tagclass != TAG_NONE)
			AsnEnBinEndIndef(aip);    /* put indefinate encoding */
		next_type = aip->type_indent - 1;   /* end any choices */
		while ((next_type >= 0) &&
			(AsnFindBaseIsa(aip->typestack[next_type].type) == CHOICE_TYPE))
		{
			if (aip->typestack[next_type].type->tagclass != TAG_NONE)
				AsnEnBinEndIndef(aip);
			next_type--;
		}
		if (AsnFindBaseIsa(aip->typestack[aip->type_indent - 1].type) == CHOICE_TYPE)
			AsnTypeSetIndent(FALSE, aip, atp);
	}
	return TRUE;
}

/*****************************************************************************
*
*   void AsnEnBinTags(atp, aip)
*   	encode tags for atp
*
*****************************************************************************/
NLM_EXTERN void AsnEnBinTags (AsnTypePtr atp, AsnIoPtr aip)
{
	int isa, len;
	Int4 tagnumber;
	Byte octets[6], bit87, bit6, bit5_1;

	if (atp->tagclass == TAG_NONE)     /* encode tag, if any */
		return;

	isa = AsnFindBaseIsa(atp);
	if ((isa == SEQ_TYPE) || (isa == SEQOF_TYPE) ||
		(isa == SET_TYPE) || (isa == SETOF_TYPE) ||
		(atp->tagclass == TAG_CONTEXT))
		bit6 = 32;       /* constructed bit set */
	else
		bit6 = 0;

	bit87 = atp->tagclass;    /* map directly */

	tagnumber = atp->tagnumber;

	if (tagnumber <= 30)        /* only need one octet */
	{
		bit5_1 = (Byte)(tagnumber) & (Byte)0x0ff;
		octets[0] = bit87 + bit6 + bit5_1;
		AsnEnBinBytes(octets, (Uint4)1, aip);
	}
	else
	{
		bit5_1 = 31;            /* extended tag */

		octets[0] = bit87 + bit6 + bit5_1;

		if (tagnumber <= 0x07f)
		{
			octets[1] = (Byte)tagnumber;
			len = 2;
		}
		else if (tagnumber <= 0x03fff)
		{
			octets[1] = (Byte)(128 + ((tagnumber >> 7) & 0x07f));
			octets[2] = (Byte)(tagnumber & 0x07f);
			len = 3;
		}
		else if (tagnumber <= 0x01fffff)
		{
			octets[1] = (Byte)(128 + ((tagnumber >> 14) & 0x07f));
			octets[2] = (Byte)(128 + ((tagnumber >> 7) & 0x07f));
			octets[3] = (Byte)(tagnumber & 0x07f);
			len = 4;
		}
		else if (tagnumber <= 0x0fffffff)
		{
			octets[1] = (Byte)(128 + ((tagnumber >> 21) & 0x07f));
			octets[2] = (Byte)(128 + ((tagnumber >> 14) & 0x07f));
			octets[3] = (Byte)(128 + ((tagnumber >> 7) & 0x07f));
			octets[4] = (Byte)(tagnumber & 0x07f);
			len = 5;
		}
		else
		{
			octets[1] = (Byte)(128 + ((tagnumber >> 28) & 0x07f));
			octets[2] = (Byte)(128 + ((tagnumber >> 21) & 0x07f));
			octets[3] = (Byte)(128 + ((tagnumber >> 14) & 0x07f));
			octets[4] = (Byte)(128 + ((tagnumber >> 7) & 0x07f));
			octets[5] = (Byte)(tagnumber & 0x07f);
			len = 6;
		}

		AsnEnBinBytes(octets, (Uint4)len, aip);
	}

	if (atp->tagclass == TAG_CONTEXT)    /* indefinate length on user tags */
		AsnEnBinStartIndef(aip);

	return;
}

/*****************************************************************************
*
*   void AsnEnBinNull(aip, atp)
*
*****************************************************************************/
NLM_EXTERN void AsnEnBinNull (AsnIoPtr aip, AsnTypePtr atp)
{
	AsnTypePtr atp2;

	atp2 = AsnFindBaseType(atp);
	atp2 = atp2->type;
	AsnEnBinTags(atp2, aip);

	AsnEnBinLen((Uint4)0, aip);
	return;
}


/*****************************************************************************
*
*   void AsnEnBinReal(realvalue, aip)
*
*****************************************************************************/
NLM_EXTERN void AsnEnBinReal (FloatHi realvalue, AsnIoPtr aip, AsnTypePtr atp)
{
	AsnTypePtr atp2;
	Byte buf[32];
	Uint4 len;

	atp2 = AsnFindBaseType(atp);
	atp2 = atp2->type;
	AsnEnBinTags(atp2, aip);
	buf[0] = 0;    /* use decimal encoding */
#ifdef OS_MAC
	/* sprintf((char *)(buf + 1), "%lf", realvalue); */
	sprintf((char *)(buf + 1), "%LG", realvalue);
#else
	sprintf((char *)(buf + 1), "%lg", realvalue);
#endif
	len = StringLen((char *)(buf+1)) + 1;
	AsnEnBinLen(len, aip);
	AsnEnBinBytes(buf, len, aip);

	return;
}

/*****************************************************************************
*
*   void AsnEnBinInteger(theInt, aip)
*
*****************************************************************************/
NLM_EXTERN void AsnEnBinInteger (Int4 theInt, AsnIoPtr aip, AsnTypePtr atp)
{
	AsnTypePtr atp2;
	Byte octets[4];
	int msb, len, i;
	Int4 cint;

	atp2 = AsnFindBaseType(atp);
	atp2 = atp2->type;
	AsnEnBinTags(atp2, aip);

	len = 0;
	/**** split integer into bytes *****/
	cint = theInt;
	for (i = 3; i >= 0; i--)
	{
		octets[i] = (Byte)(cint & 0x0ff);
		cint >>= 8;
	}

	/***** find most significant byte ****/
	msb = 0;
	if (theInt >= 0)
	{
		while (msb < 3 && octets[msb] == 0 &&
				((octets[msb+1] & 0x080) == 0))
			msb++;
	}
	else
	{
		while (msb < 3 && octets[msb] == 0x0ff &&
				((octets[msb+1] & 0x080) == 0x080))
			msb++;
	}

	len = 4 - msb;

	AsnEnBinLen((Uint4)len, aip);

	/** shift left to eliminate unnecessary bytes */
	for ( ; msb > 0; msb--)
	{
		for (i = 0; i < 3; i++)
			octets[i] = octets[i+1];
/* dirty workaround of an optimization bug of MSVC++.NET compiler */
#if defined(_MSC_VER) 
#  if _MSC_VER < 1300
		octets[3] = 0;
#  endif
#else
		octets[3] = 0;
#endif
	}

	AsnEnBinBytes(octets, (Uint4)len, aip);

	return;
}

/*****************************************************************************
*
*   void AsnEnBinBigInt(theInt, aip)
*
*****************************************************************************/
NLM_EXTERN void AsnEnBinBigInt (Int8 theInt, AsnIoPtr aip, AsnTypePtr atp)
{
	AsnTypePtr atp2;
	Byte octets[8];
	int msb, len, i;
	Int8 cint;

	atp2 = AsnFindBaseType(atp);
	atp2 = atp2->type;
	AsnEnBinTags(atp2, aip);

	len = 0;
	/**** split integer into bytes *****/
	cint = theInt;
	for (i = 7; i >= 0; i--)
	{
		octets[i] = (Byte)(cint & 0x0ff);
		cint >>= 8;
	}

	/***** find most significant byte ****/
	msb = 0;
	if (theInt >= 0)
	{
		while (msb < 7 && octets[msb] == 0 &&
				((octets[msb+1] & 0x080) == 0))
			msb++;
	}
	else
	{
		while (msb < 7 && octets[msb] == 0x0ff &&
				((octets[msb+1] & 0x080) == 0x080))
			msb++;
	}

	len = 8 - msb;

	AsnEnBinLen((Uint4)len, aip);

	/** shift left to eliminate unnecessary bytes */
	for ( ; msb > 0; msb--)
	{
		for (i = 0; i < 7; i++)
			octets[i] = octets[i+1];
/* dirty workaround of an optimization bug of MSVC++.NET compiler */
#if defined(_MSC_VER) 
#  if _MSC_VER < 1300
		octets[7] = 0;
#  endif
#else
		octets[7] = 0;
#endif
	}

	AsnEnBinBytes(octets, (Uint4)len, aip);

	return;
}

/*****************************************************************************
*
*   void AsnEnBinBoolean(value, aip)
*
*****************************************************************************/
NLM_EXTERN void AsnEnBinBoolean (Boolean value, AsnIoPtr aip, AsnTypePtr atp)
{
	AsnTypePtr atp2;
	Byte octets[1];

	atp2 = AsnFindBaseType(atp);
	atp2 = atp2->type;
	AsnEnBinTags(atp2, aip);

	AsnEnBinLen((Uint4)1, aip);

	if (value)
		octets[0] = 1;
	else
		octets[0] = 0;
	AsnEnBinBytes(octets, (Uint4)1, aip);
	return;
}
/*****************************************************************************
*
*   Boolean AsnEnBinString(str, aip)
*
*****************************************************************************/
NLM_EXTERN Boolean AsnEnBinString (CharPtr str, AsnIoPtr aip, AsnTypePtr atp)
{
	Uint4 stringlen;
	AsnTypePtr atp2;

	atp2 = AsnFindBaseType(atp);
	atp2 = atp2->type;
	AsnEnBinTags(atp2, aip);

	stringlen = StringLen(str);
	AsnEnBinLen(stringlen, aip);
	if (! AsnEnBinTheBytes(str, stringlen, aip, TRUE))
		return FALSE;
	return TRUE;
}

/*****************************************************************************
*
*   AsnEnBinOctets(bsp, aip, atp)
*
*****************************************************************************/
NLM_EXTERN void AsnEnBinOctets (ByteStorePtr bsp, AsnIoPtr aip, AsnTypePtr atp)
{
	Uint4 totlen, tlen;
	AsnTypePtr atp2;
	Byte buf[100];

	atp2 = AsnFindBaseType(atp);
	atp2 = atp2->type;
	AsnEnBinTags(atp2, aip);

	totlen = BSLen(bsp);
	AsnEnBinLen(totlen, aip);

	BSSeek(bsp, 0, SEEK_SET);

	while (totlen)
	{
		if (totlen < 100)
			tlen = totlen;
		else
			tlen = 100;

		BSRead(bsp, buf, tlen);
		AsnEnBinBytes(buf, tlen, aip);
		totlen -= tlen;
	}
	return;
}

/*****************************************************************************
*
*   AsnEnBinOpenStruct(aip, atp)
*
*****************************************************************************/
NLM_EXTERN void AsnEnBinOpenStruct (AsnIoPtr aip, AsnTypePtr atp)
{
	AsnTypePtr atp2;

	atp2 = AsnFindBaseType(atp);
	atp2 = atp2->type;
	AsnEnBinTags(atp2, aip);
	AsnEnBinStartIndef(aip);
	AsnTypeSetIndent(TRUE, aip, atp);
	return;
}

/*****************************************************************************
*
*   AsnEnBinCloseStruct(aip, atp)
*
*****************************************************************************/
NLM_EXTERN void AsnEnBinCloseStruct (AsnIoPtr aip, AsnTypePtr atp)
{
	int next_type, tagclass;

	AsnEnBinEndIndef(aip);     /* end the primitive struct */
	tagclass = aip->typestack[aip->type_indent - 1].type->tagclass;
	if (tagclass == TAG_CONTEXT)
		AsnEnBinEndIndef(aip);   /* end for user tags */
	next_type = aip->type_indent - 2;   /* end any choices */
	while ((next_type >= 0) &&
		(AsnFindBaseIsa(aip->typestack[next_type].type) == CHOICE_TYPE))
	{
		if (aip->typestack[next_type].type->tagclass != TAG_NONE)
			AsnEnBinEndIndef(aip);
		next_type--;
	}
	AsnTypeSetIndent(FALSE, aip, atp);
	return;
}


/*****************************************************************************
*
*   AsnEnBinStartIndef(aip)
*
*****************************************************************************/
NLM_EXTERN void AsnEnBinStartIndef (AsnIoPtr aip)
{
	Byte octets[1];

	octets[0] = 128;     /* indefinate length encoding */
	AsnEnBinBytes(octets, (Uint4)1, aip);
	return;
}

/*****************************************************************************
*
*   AsnEnBinEndIndef(aip)
*
*****************************************************************************/
NLM_EXTERN void AsnEnBinEndIndef (AsnIoPtr aip)
{
	Byte nulls[2];

	nulls[0] = 0;
	nulls[1] = 0;
	AsnEnBinBytes(nulls, (Uint4)2, aip);
	return;
}


/*****************************************************************************
*
*   void AsnEnBinTheBytes(ptr, len, aip, is_string)
*   	write bytes to asnio output
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL  AsnEnBinTheBytes (Pointer ptr, Uint4 len, AsnIoPtr aip, Boolean is_string)
{
	register BytePtr from, to;
	register int bytes, amount, bad_char = 0, bad_char_ctr = 0;

	bytes = (int)(aip->bytes - aip->offset);
	if (! bytes)
		bytes = (int)AsnIoWriteBlock(aip);
	to = aip->buf + aip->offset;
	from = (BytePtr) ptr;

	while (len)
	{
		if (aip->io_failure)   /* physical device write failure */
			return FALSE;

		if (len <= (Uint4)bytes)
			amount = (int)len;
		else
			amount = bytes;
		bytes -= amount;
		len -= (Uint4)amount;
		while (amount)
		{
			*to = *from;
			if (is_string)
			{
				if ((aip->fix_non_print != 2) && ((*from < ' ') || (*from > '~')))
				{
					if (! bad_char_ctr)
						bad_char = (int)(*from);
					bad_char_ctr++;
					*from = '#';
					*to = '#';   /* replace with # */
				}
			}
			from++; to++; amount--;
		}
		if (! bytes)
		{
			aip->offset = aip->bytes;     /* set to write whole block */
			bytes = (int)AsnIoWriteBlock(aip);
			to = aip->buf;
		}
	}
	aip->offset = (Int2) (aip->bytes - (Int2) bytes);
	if ((bad_char_ctr) && ((aip->fix_non_print == 0) || (aip->fix_non_print == 3)))
		AsnIoErrorMsg(aip, 106, bad_char, (CharPtr)ptr);
	return TRUE;
}

/*****************************************************************************
*
*   void AsnBinLen(len, aip)
*   	encode a definate length
*
*****************************************************************************/
NLM_EXTERN void AsnEnBinLen (Uint4 len, AsnIoPtr aip)
{
	Byte octets[5];
	int octnum, i;

	for (octnum = 0; octnum < 5; octnum++)
		octets[octnum] = 0;

	if (len <= 127)      /* use short form */
	{
		octets[0] = (Byte)(len & 0x0ffff);
		AsnEnBinBytes(octets, (Uint4)1, aip);
		return;
	}

	if (len <= 0x0ff)  /* use long form */
		octnum = 1;
	else if (len <= 0x0ffff)
		octnum = 2;
	else if (len <= 0x0ffffff)
		octnum = 3;
	else
		octnum = 4;

	octets[0] = (Byte)(0x080 | (octnum & 0x07f));

	for (i = 1; i <= octnum; i++)
		octets[i] = (Byte)((len >> ((octnum - i) * 8)) & 0x0ff);

	octnum++;
	AsnEnBinBytes(octets, (Uint4)octnum, aip);
	return;
}

