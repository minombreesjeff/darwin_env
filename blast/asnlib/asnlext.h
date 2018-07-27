/*  asnlext.h
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
* File Name: asnlext.h
*
* Author:  James Ostell
*
* Version Creation Date: 1/1/91
*
* $Revision: 6.2 $
*
* File Description:
*   typedefs and prototypes used internally by asnio.c
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: asnlext.h,v $
* Revision 6.2  2001/06/26 22:03:17  juran
* Make C++-compatible.
*
* Revision 6.1  2000/05/12 20:44:00  ostell
* make changes to collect comments from spec and print in DTD
*
* Revision 6.0  1997/08/25 18:10:11  madden
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
 * Revision 2.2  1995/05/15  18:38:28  ostell
 * added Log line
 *
*
* ==========================================================================
*/

#ifndef _ASNLEXT_
#define _ASNLEXT_

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


	/*************************************************************************
	*
	*   tokens returned by AsnLexTWord()
	*   	subset returned by AsnLexWord()
	*   	301 - 400  are reserved for asn1 primitive types and map
	*		   	to isa values
	*   		These are defined in asnlex.h
	*   	401 - 500  are used for symbols and other reserved words
	*   	    401-427 defined in asnlex.h
	*          this is a superset which includes ASN.1 additional reserved
	*   		words for parsing syntax specifications
	*       AsnLexTMatchToken()  depends on the order of these defines and
	*   		the strings in asnwords[]
	*
	*************************************************************************/

#define DEF_TOKEN		428     /* DEFINITIONS */
#define BEGIN_TOKEN		429		/* BEGIN */
#define END_TOKEN		430		/* END */
#define AP_TAG_TOKEN	431		/* APPLICATION */
#define PRIV_TAG_TOKEN	432		/* PRIVATE */
#define UNIV_TAG_TOKEN	433		/* UNIVERSAL */
#define COMPS_TOKEN		434		/* COMPONENTS */
#define DEFAULT_TOKEN	435		/* DEFAULT */
#define FALSE_TOKEN		436		/* FALSE */
#define TRUE_TOKEN		437		/* TRUE */
#define IMPLICIT_TOKEN 	438		/* IMPLICIT */
#define OPTIONAL_TOKEN	439		/* OPTIONAL */
#define EXPORTS_TOKEN	440		/* EXPORTS */
#define IMPORTS_TOKEN	441		/* IMPORTS */
#define ABSENT_TOKEN	442		/* ABSENT */
#define BY_TOKEN		443		/* BY */
#define COMP_TOKEN		444		/* COMPONENT */
#define DEFINE_TOKEN	445		/* DEFINED */
#define FROM_TOKEN		446		/* FROM */
#define INCLUDES_TOKEN	447		/* INCLUDES */
#define MIN_TOKEN		448		/* MIN */
#define MINUSI_TOKEN	449		/* MINUS-INFINITY */
#define MAX_TOKEN		450		/* MAX */
#define PRESENT_TOKEN	451		/* PRESENT */
#define PLUSI_TOKEN		452		/* PLUS-INFINITY */
#define SIZE_TOKEN		453		/* SIZE */
#define TAGS_TOKEN		454		/* TAGS */
#define WITH_TOKEN		455		/* WITH */
#define IDENT_TOKEN		456		/* IDENTIFIER (goes with OBJECT) */
#define REVISION_TOKEN  457     /* $Revision   (NCBI only) */
#define ISMODULE_TOKEN 458 /* signal from PrintModule to AsnPrintIndent */
#define COMMENT_TOKEN 459  /* for comment on same line as an element */

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   prototypes
*
*****************************************************************************/

NLM_EXTERN AsnModulePtr AsnLexTReadModule PROTO((AsnIoPtr aip));
NLM_EXTERN Int2 AsnLexTReadTypeAssign PROTO((AsnIoPtr aip, AsnModulePtr amp));
NLM_EXTERN Int2 AsnLexTReadType PROTO((AsnIoPtr aip, AsnModulePtr amp, AsnTypePtr atp));
NLM_EXTERN AsnTypePtr AsnLexTReadElementTypeList PROTO((AsnIoPtr aip, AsnModulePtr amp, AsnTypePtr
parent));
NLM_EXTERN AsnTypePtr AsnLexTReadAlternativeTypeList PROTO((AsnIoPtr aip, AsnModulePtr amp,
AsnTypePtr parent));
NLM_EXTERN AsnModulePtr AsnLexTStartModule PROTO((AsnIoPtr aip));
NLM_EXTERN Int2 AsnLexTMatchToken PROTO((AsnIoPtr aip));
NLM_EXTERN Int2 AsnLexTWord PROTO((AsnIoPtr aip));

NLM_EXTERN AsnValxNodePtr AsnValxNodeNew PROTO((AsnValxNodePtr anvp, Int2 type));
NLM_EXTERN AsnTypePtr AsnGetType PROTO((AsnIoPtr aip, AsnModulePtr amp));
NLM_EXTERN AsnTypePtr AsnTypeNew PROTO((AsnIoPtr aip, AsnModulePtr amp));
NLM_EXTERN AsnTypePtr AsnElementTypeNew PROTO((AsnIoPtr aip));
NLM_EXTERN AsnModulePtr AsnCheckModule PROTO((AsnModulePtr amp, AsnIoPtr aip));
NLM_EXTERN void AsnSetTags PROTO((AsnTypePtr atp));

#ifdef __cplusplus
}
#endif


#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
