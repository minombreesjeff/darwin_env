/* gbparlex.h
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
* File Name:  gbparse.h
*
* Author:  Karl Sirotkin
*
* $Log: parsegb.h,v $
* Revision 6.1  1999/04/06 19:42:56  bazhin
* Changes, related to flat2asn's ACCESSION.VERSION parsing.
*
* Revision 6.0  1997/08/25 18:06:52  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/06/19 18:38:34  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.0  1996/05/28 13:23:23  ostell
* Set to revision 5.0
*
 * Revision 4.1  1995/11/17  21:28:35  kans
 * asn2ff now uses gather (Tatiana)
 *
 * Revision 1.2  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*/


#ifndef _NCBI_GBParse_
#define _NCBI_GBParse_

#ifndef _ASNTOOL_
#include <asn.h>
#endif
#ifndef _NCBI_General_
#include <objgen.h>
#endif
#ifndef _NCBI_Seqloc_
#include <objloc.h>
#endif

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

typedef void (*Nlm_gbparse_errfunc) PROTO((CharPtr, CharPtr));
typedef Int4 (*Nlm_gbparse_rangefunc) PROTO ((Pointer, SeqIdPtr ));

NLM_EXTERN int Nlm_gbparselex PROTO(( CharPtr linein, ValNodePtr PNTR lexed));
NLM_EXTERN int Nlm_gbparselex_ver PROTO(( CharPtr linein, ValNodePtr PNTR lexed, Boolean accver));
NLM_EXTERN CharPtr Nlm_non_white PROTO(( CharPtr ch));
NLM_EXTERN ValNodePtr Nlm_gbparse_lexfree PROTO(( ValNodePtr anp));
NLM_EXTERN SeqLocPtr Nlm_gbparseint PROTO(( CharPtr raw_intervals, Boolean PNTR keep_rawPt, Boolean PNTR sites, int PNTR num_errsPt, SeqIdPtr seq_id));
NLM_EXTERN SeqLocPtr Nlm_gbparseint_ver PROTO(( CharPtr raw_intervals, Boolean PNTR keep_rawPt, Boolean PNTR sites, int PNTR num_errsPt, SeqIdPtr seq_id, Boolean accver));
NLM_EXTERN CharPtr Nlm_gbparse_point PROTO((ValNodePtr, ValNodePtr));
NLM_EXTERN int deal_with_complement PROTO ((ValNodePtr PNTR, int PNTR, CharPtr, ValNodePtr));
NLM_EXTERN SeqLocPtr Nlm_gbloc PROTO((Boolean PNTR, int PNTR, Boolean PNTR, ValNodePtr PNTR, ValNodePtr, int PNTR, SeqIdPtr));
NLM_EXTERN SeqLocPtr Nlm_gbloc_ver PROTO((Boolean PNTR, int PNTR, Boolean PNTR, ValNodePtr PNTR, ValNodePtr, int PNTR, SeqIdPtr, Boolean));
NLM_EXTERN SeqLocPtr Nlm_gbreplace PROTO((Boolean PNTR, int PNTR, Boolean PNTR, ValNodePtr PNTR, ValNodePtr , int PNTR, SeqIdPtr));
NLM_EXTERN SeqLocPtr Nlm_gbreplace_ver PROTO((Boolean PNTR, int PNTR, Boolean PNTR, ValNodePtr PNTR, ValNodePtr , int PNTR, SeqIdPtr, Boolean));
NLM_EXTERN SeqLocPtr Nlm_gbint PROTO((Boolean PNTR, ValNodePtr PNTR, ValNodePtr , int PNTR, SeqIdPtr));
NLM_EXTERN SeqLocPtr Nlm_gbint_ver PROTO((Boolean PNTR, ValNodePtr PNTR, ValNodePtr , int PNTR, SeqIdPtr, Boolean));
NLM_EXTERN SeqPntPtr Nlm_gbpintpnt PROTO(( SeqLocPtr, SeqIntPtr PNTR));
NLM_EXTERN void Nlm_gbload_number PROTO((Int4 PNTR, IntFuzzPtr PNTR, Boolean PNTR, ValNodePtr PNTR, ValNodePtr , int PNTR, int));
NLM_EXTERN SeqLocPtr Nlm_gbparse_better_be_done PROTO((int PNTR num_errPt, ValNodePtr currentPt, ValNodePtr headPt, SeqLocPtr ret_so_far, Boolean PNTR keep_rawPt, int paren_count));
NLM_EXTERN void do_Nlm_gbparse_error PROTO((CharPtr msg, CharPtr details));
NLM_EXTERN void Nlm_install_gbparse_error_handler PROTO((Nlm_gbparse_errfunc));
NLM_EXTERN void Nlm_install_gbparse_range_func PROTO((Pointer, Nlm_gbparse_rangefunc));
NLM_EXTERN void Nlm_gbparse_error PROTO((CharPtr, ValNodePtr, ValNodePtr));

#define gbparselex Nlm_gbparselex
#define gbparse_lexfree Nlm_gbparse_lexfree
#define gbparseint Nlm_gbparseint 
#define install_gbparse_error_handler Nlm_install_gbparse_error_handler 
#define install_gbparse_range_func Nlm_install_gbparse_range_func

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
