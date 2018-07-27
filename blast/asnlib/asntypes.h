/* asntypes.h
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
* File Name: asntypes.h
*
* Author:  James Ostell
*
* Version Creation Date: 1/1/91
*
* $Revision: 6.2 $
*
* File Description:
*   typedefs and prototypes for routines used internally by asntypes.c
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: asntypes.h,v $
* Revision 6.2  2000/12/12 15:56:14  ostell
* added support BigInt
*
* Revision 6.1  1997/12/16 14:51:54  kans
* header needed for asntool/asncode merge
*
* Revision 6.0  1997/08/25 18:10:27  madden
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
 * Revision 2.4  1995/05/15  18:38:28  ostell
 * added Log line
 *
*
* ==========================================================================
*/

#ifndef _ASNTYPES_
#define _ASNTYPES_

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


	/*************************************************************************
	*
	*   tokens returned by AsnLexWord()
	*   	superset returned by AsnLexTWord()
	*   	301 - 350  are reserved for asn1 primitive types and map
	*		   	to isa values
	*   	351 - 400  are reserved for application specific tags and
	*   		map to isa values
	*   	401 - 500  are used for symbols and other reserved words
	*          a superset which includes ASN.1 additional reserved words
	*            for parsing syntax specifications are in asnlext.h
	*       AsnLexTMatchToken()  depends on the order of these defines and
	*   		the strings in asnwords[]
	*
	*************************************************************************/


#define EOF_TOKEN		401		/* End of file */
#define ERROR_TOKEN 	402		/* read error */
#define START_STRING 	403		/* found " string starts */
#define IN_STRING 		404		/* found piece of string */
#define END_STRING 		405		/* end of string found */
#define START_BITHEX 	406		/* found ' bit or hex string starts */
#define IN_BITHEX 		407		/* found piece of bit or hex string */
#define OCTETS 			408		/* bit or hex ends, was a hex string */
#define ASNBITS			409		/* bit or hex ends, was a bit string */
#define COMMA 			410		/* , found */
#define OPEN_PAREN 		413		/* ( found */
#define CLOSE_PAREN 	414		/* ) found */
#define START_TAG		415		/* [ found */
#define END_TAG			416		/* ] found */
#define ISDEF			417		/* ::= found */
#define REF				418		/* typereference (not ASN reserved word) */
#define IDENT			419		/* identifier found (could be valuereference)*/
#define NUMBER			420		/* number found */
#define SEMI_COLON		421		/* ; found */

		/***** is token a predefined ASN.1 type ************/
#define ISA_ASNTYPE(x)	(((x) >= 301) && ((x) <= 350))
	    /***** is token a basic (end of chain) type ********/
#define ISA_BASETYPE(x)	 (((x) >= 301) && ((x) <= 400))

#define ISA_TAGCLASS(x)  (((x) >= 431) && ((x) <= 433))

#define TAG_UNIVERSAL 	0   	       /* for bits 7-8 = 00 */
#define TAG_APPLICATION 64			   /*                01 */
#define TAG_PRIVATE 	192			   /*                11 */
#define TAG_CONTEXT 	128			   /*                10 */
#define TAG_NONE		1              /* has no tag */

#define TAG_BOOLEAN				1 		/* ASN.1 Universal tags */
#define TAG_INTEGER				2
#define TAG_BIT_STRING			3
#define TAG_OCTET_STRING		4
#define TAG_NULL				5
#define TAG_OBJECT_IDENT		6
#define TAG_OBJECT_DESCR		7
#define TAG_EXTERNAL			8
#define TAG_REAL				9
#define TAG_ENUM				10
#define TAG_SEQ					16		/* SEQ and SEQOF */
#define TAG_SET					17		/* SET and SETOF */
#define TAG_NUMERICSTRING		18
#define TAG_PRINTABLESTRING		19
#define TAG_TELETEXSTRING		20
#define TAG_VIDEOTEXSTRING		21
#define TAG_IA5STRING			22
#define TAG_UTCTIME				23
#define TAG_GENERALIZEDTIME		24
#define TAG_GRAPHICSSTRING		25
#define TAG_VISIBLESTRING		26
#define TAG_GENERALSTRING		27
#define TAG_CHARACTERSTRING		28

#define TAG_STRSTORE			1

	/* values for valueisa */
#define VALUE_ISA_PTR	1
#define VALUE_ISA_BOOL	2
#define VALUE_ISA_INT	3
#define VALUE_ISA_REAL	4
#define VALUE_ISA_DEFAULT(x)	(((x) >= VALUE_ISA_PTR) && ((x) <= VALUE_ISA_REAL))
#define VALUE_ISA_SUBTYPE	10		/* subtype chain */
#define VALUE_ISA_NAMED_INT 20		/* named int chain */
#define VALUE_ISA_BIGINT 5

/*--
 *
 * Expanded to full AsnTypePtr for ease of integrating asncode into asntool.
 *    so this new definition is not even needed, but will make equivalent
 *    to AsnType, for safety.
 *
 *       -   Karl Sirotkin
OLD WAY:
typedef struct asnprimtype {
	Int2 isa;
	CharPtr name;
	Uint1 tagclass;
	Int2 tagnumber;
}	AsnPrimType, PNTR AsnPrimTypePtr;
----------------------------------------------------*/

typedef AsnType AsnPrimType  , PNTR AsnPrimTypePtr;


/*****************************************************************************
*
*   prototypes
*       AsnFindBaseType()
*       AsnTypeFind()
*   	AsnFindPrimName()
*   	AsnKillValue() prototyped in asngen.h
*
*****************************************************************************/
NLM_EXTERN void AsnTypeSetIndent PROTO((Boolean increase, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean AsnTypeValidateOut PROTO((AsnIoPtr aip, AsnTypePtr atp, DataValPtr dvp));
NLM_EXTERN AsnTypePtr AsnTypeFindType PROTO((AsnTypePtr atp, CharPtr str, AsnTypePtr PNTR typeptr, Int2 count, Boolean in_it));


#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
