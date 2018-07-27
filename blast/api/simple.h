/*  simple.h
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
* File Name:  simple.h
*
* Author:  James Ostell
*   
* Version Creation Date: 7/12/91
*
* $Revision: 6.0 $
*
* File Description:  Simple Sequence Objects
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: simple.h,v $
* Revision 6.0  1997/08/25 18:07:32  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/06/19 18:39:07  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.0  1996/05/28 13:23:23  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 2.1  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/
#ifndef _ASNTOOL_
#include <asn.h>
#endif

#ifndef _NCBI_Simple_
#define _NCBI_Simple_

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
*   loader
*
*****************************************************************************/
NLM_EXTERN Boolean AllAsnLoad PROTO((void));

/*****************************************************************************
*
*   internal structures for simple objects
*
*****************************************************************************/
typedef struct simpleseq {
    CharPtr id[10];       /* room for 10 id strings */
    Int2 numid,
         bestid;          /* most stable id in set */
    CharPtr title;
    ByteStorePtr seq;
    Int4 seqlen;
} SimpleSeq, PNTR SimpleSeqPtr;

NLM_EXTERN SimpleSeqPtr SimpleSeqNew PROTO((void));
NLM_EXTERN SimpleSeqPtr SimpleSeqAsnRead PROTO((AsnIoPtr aip));
NLM_EXTERN SimpleSeqPtr SimpleSeqFree PROTO((SimpleSeqPtr ssp));
NLM_EXTERN Boolean SimpleSeqPrint PROTO((SimpleSeqPtr ssp, FILE * fp, Boolean as_fasta));

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

