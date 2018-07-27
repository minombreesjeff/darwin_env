/* prtutil.h
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
* File Name:  prtutil.h
*
* Author:  James Ostell
*   
* Version Creation Date: 4/1/93
*
* $Revision: 6.1 $
*
* File Description: Object Print Utilities
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: prtutil.h,v $
* Revision 6.1  2004/02/02 17:30:21  kans
* PrintTemplateSetLoadEx takes a local string, used to make internal copy of objprt.prt in Sequin
*
* Revision 6.0  1997/08/25 18:06:57  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/06/19 18:38:38  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.0  1996/05/28 13:23:23  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 1.4  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/
#ifndef _NCBI_PrtUtil_
#define _NCBI_PrtUtil_

#ifndef _NCBI_ObjPrt_
#include <objprt.h>		   /* the object loader interface */
#endif

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   Print Templates
*
*****************************************************************************/
NLM_EXTERN PrintTemplatePtr PrintTemplateFind PROTO((CharPtr name));
NLM_EXTERN Boolean PrintTemplateSetLoad PROTO(( CharPtr path ));
NLM_EXTERN Boolean PrintTemplateSetLoadEx ( CharPtr path, CharPtr localStr );


/*****************************************************************************
*
*   Print Stacks
*
*****************************************************************************/
#define PRINT_STACK_SIZE 10
#define OP_NCBIPRT -10    /* should go to asn.h */
#define CTX_OBJPRT 100    /* should go in ncbierr.h */

typedef struct printformatitem {
	Int2 sort_key,
		indent_level;
	PrintFormatPtr format;   /* the format */
	PrintTemplatePtr _template; /* the format used in use_template */
	AsnTypePtr PNTR types;   /* chain of types for this path */
	Int2 numtypes;           /* number of types (above) */
	AsnTypePtr atp;          /* resolved node for format->asn1 */
	Int2 isa;                /* return from AsnFindBaseIsa(atp) */
} PrintFormatItem, PNTR PrintFormatItemPtr;

typedef struct printformatlist {
	PrintTemplatePtr ptp;        /* the template used */
	Int2 numitem;
	PrintFormatItemPtr list;     /* array of PrintFormatItem */
} PrintFormatList, PNTR PrintFormatListPtr;

typedef struct printstackitem {
	PrintFormatItemPtr pfip;
	DataVal dv;
} PrintStackItem, PNTR PrintStackItemPtr;

typedef struct printstack {
	PrintFormatListPtr pflp;         /* the format list in use */
	CharPtr labelfrom;
	Int2 pstacksize,              /* size of pstack */
		pstackpos;                /* next available cell in pstack */
	PrintStackItemPtr data;
	Int2 size,                     /* total size, including branches */
		indent;                   /* PrintFormatItem.indent_level of parent */
	Boolean is_branch;            /* true if is "use_template" branch */
	PrintFormatItemPtr branch;   /* item branched from if is_branch */
} PrintStack, PNTR PrintStackPtr;

typedef struct pstacklistitem {
	Int2 type_indent;            /* asn.1 indent level when branch added */
	PrintStackPtr psp;           /* the printstack */
} PrintStackListItem, PNTR PrintStackListItemPtr;

typedef struct pstacklist {
	Int2 size,                   /* total size of pstip */
		used;                    /* number of elements used */
	PrintStackListItemPtr pstip;      /* the array of items */
} PrintStackList, PNTR PrintStackListPtr;

typedef struct stdprintoptions {   /* options for StdFormatPrint() */
	FILE * fp;                     /* for output to file */
	Int2 linelength;               /* max line length */
	CharPtr ptr;                   /* returned string, if fp==NULL */
	CharPtr indent ,               /* string to use at each indent level */
	        newline ,              /* string to use for '\n' */
			tab;                   /* string to use for '\t' */
	ByteStorePtr bstp;             /* used internally */	
} StdPrintOptions, PNTR StdPrintOptionsPtr;

NLM_EXTERN PrintStackPtr PrintStackBuild PROTO((Pointer data, AsnWriteFunc func, PrintFormatListPtr pflp));
NLM_EXTERN PrintStackItemPtr PrintStackItemNew PROTO((PrintStackPtr psp));
NLM_EXTERN PrintStackPtr PrintStackFree PROTO((PrintStackPtr psp));
NLM_EXTERN PrintStackItemPtr PrintStackItemGet PROTO((PrintStackPtr psp, Int2 index, Int2Ptr indent_offset));
NLM_EXTERN Int2 PrintStackGetSize PROTO((PrintStackPtr psp));
NLM_EXTERN Boolean PrintStackSort PROTO((PrintStackPtr psp));
NLM_EXTERN void PrintStackDump PROTO((PrintStackPtr psp, FILE * fp, Int2 indent));

NLM_EXTERN PrintFormatListPtr PrintFormatListNew PROTO((void));
NLM_EXTERN PrintFormatListPtr PrintFormatListBuild PROTO((PrintTemplatePtr ptp));
NLM_EXTERN PrintFormatListPtr PrintFormatListFree PROTO((PrintFormatListPtr pflp));
NLM_EXTERN void PrintFormatListFreeAll PROTO((Boolean templates));
NLM_EXTERN PrintFormatListPtr PrintFormatListFind PROTO((CharPtr name));
NLM_EXTERN PrintFormatListPtr PrintFormatListGet PROTO((CharPtr templatename));

NLM_EXTERN Int2 PrintFormatTraverse PROTO((PrintFormatPtr pfp, PrintFormatListPtr pflp, Int2 index, Int2 indent));

NLM_EXTERN void PrintStackPrint PROTO((PrintStackPtr psp, StdPrintOptionsPtr spop));

NLM_EXTERN CharPtr StdSeqLocPrint PROTO((DataValPtr dvp));
NLM_EXTERN CharPtr StdDatePrint PROTO((DataValPtr dvp));
NLM_EXTERN CharPtr StdAuthListPrint PROTO((DataValPtr dvp));

NLM_EXTERN Boolean StdFormatPrint PROTO((Pointer data, AsnWriteFunc func, CharPtr templatename, StdPrintOptionsPtr options));

NLM_EXTERN StdPrintOptionsPtr StdPrintOptionsNew PROTO((FILE * fp));
NLM_EXTERN StdPrintOptionsPtr StdPrintOptionsFree PROTO((StdPrintOptionsPtr spop));


/*****************************************************************************
*
*   These functions are in testprt.c, but will be moved
*
*****************************************************************************/

NLM_EXTERN CharPtr StdSeqLocPrint PROTO((DataValPtr dvp));
NLM_EXTERN CharPtr StdDatePrint PROTO((DataValPtr dvp));
NLM_EXTERN CharPtr StdAuthListNamesPrint PROTO((DataValPtr dvp));

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

