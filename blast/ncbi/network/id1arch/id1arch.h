/*
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
* File Name:  id1arch.h
*
* Authors:  Eugene Yaschenko
*/
#ifndef __ID1ARCH_H_
#define __ID1ARCH_H_
#include <ncbi.h>
#include <sequtil.h>
#include <id1gen.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" {
#endif


#define GI_IS_SUPPRESSED        0x100
#define GI_IS_OVERRIDEN         0x200
#define GI_IS_CONFIDENTIAL      0x400


Boolean ID1ArchInit PROTO((void));
Boolean ID1ArchFini PROTO((void));

SeqEntryPtr ID1ArchSeqEntryGet PROTO((Int4 uid, CharPtr db, Int4 ent, Int4Ptr status, Int2 maxplex));
Int4 ID1ArchGIGet PROTO((SeqIdPtr sip));
SeqIdPtr ID1ArchSeqIdsGet PROTO((Uint4 gi,AsnIoPtr aiopr));
Int4    ID1ArcgGIStateGet PROTO((Uint4 gi));
ID1SeqHistPtr ID1ArchGIHistGet PROTO((Uint4 gi,Boolean rev,AsnIoPtr aiopr));
void    SeqHistPrintTable PROTO ((ID1SeqHistPtr ishp,FILE PNTR fp));
void   id_print_gi_state(Int4 state,CharPtr buf,Uint1 len);

/* # of retries to get a server */
#define ID_SERV_RETRIES 2

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
