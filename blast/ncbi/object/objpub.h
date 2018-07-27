/*  objpub.h
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
* File Name:  objpub.h
*
* Author:  James Ostell
*   
* Version Creation Date: 4/1/91
*
* $Revision: 6.0 $
*
* File Description:  Object manager interface for module NCBI-Pub
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: objpub.h,v $
* Revision 6.0  1997/08/25 18:50:30  madden
* Revision changed to 6.0
*
* Revision 4.3  1997/06/19 18:41:48  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.2  1996/03/29 21:09:59  ostell
* added support for PubMedId
*
 * Revision 4.1  1995/08/30  18:33:53  ostell
 * added PubLabelUnique function
 *
 * Revision 4.0  1995/07/26  13:48:06  ostell
 * force revision to 4.0
 *
 * Revision 3.3  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#ifndef _NCBI_Pub_
#define _NCBI_Pub_

#ifndef _ASNTOOL_
#include <asn.h>
#endif
#ifndef _NCBI_Biblio_
#include <objbibli.h>
#endif
#ifndef _NCBI_Medline_
#include <objmedli.h>
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
*   loader
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL PubAsnLoad PROTO((void));

/*****************************************************************************
*
*   internal structures for NCBI-Pub objects
*
*****************************************************************************/

/*****************************************************************************
*
*   Pub is a choice using an ValNode, most types in data.ptrvalue
*   choice:
*   0 = not set
    1 = gen Cit-gen ,        -- general or generic unparsed
    2 = sub Cit-sub ,        -- submission
    3 = medline Medline-entry ,
    4 = muid INTEGER ,       -- medline uid (stored in data.intvalue)
    5 = article Cit-art ,
    6 = journal Cit-jour ,
    7 = book Cit-book ,
    8 = proc Cit-proc ,      -- proceedings of a meeting
    9 = patent Cit-pat ,
    10 = pat-id Id-pat ,      -- identify a patent
    11 = man Cit-let         -- manuscript or letter
    12 = equiv Pub-equiv      -- set of equivalent citation forms for 1 pub
	13 = pmid INTEGER         -- PubMedId
*
*****************************************************************************/
NLM_EXTERN Boolean    LIBCALL PubAsnWrite PROTO((ValNodePtr anp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN ValNodePtr LIBCALL PubAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN ValNodePtr LIBCALL PubFree PROTO((ValNodePtr anp));
NLM_EXTERN Int2 LIBCALL PubLabel PROTO((ValNodePtr pub, CharPtr buf, Int2 buflen, Uint1 content));

/*****************************************************************************
*
*   PubLabelUnique(pubptr, buf, buflen, content, unique)
*   	makes a short label for any Pub in buf, up to buflen size
*   	content follows objmgr OM_LABEL_
*       if (unique is TRUE, appends a string based on title words to make
*           unique key base on ANSI std Z39.56-1991
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL PubLabelUnique PROTO((ValNodePtr pub, CharPtr buf, Int2 buflen, Uint1 content, Boolean unique));

#define PUB_Gen 1
#define PUB_Sub 2
#define PUB_Medline 3
#define PUB_Muid 4
#define PUB_Article 5
#define PUB_Journal 6
#define PUB_Book 7
#define PUB_Proc 8
#define PUB_Patent 9
#define PUB_Pat_id 10
#define PUB_Man 11
#define PUB_Equiv 12
#define PUB_PMid 13

/****
*  Pub and PubEquiv Matching functions (same citation, not same form)
*   PubMatch() returns
*   	0 = point to same citation
*       1,-1 = same pub type, but different
*       2,-2 = different put types, don't match
*   PubEquivMatch() returns
*   	0 = point to same citation
*   	1,-1 = point to different citations
*****/
NLM_EXTERN Int2 LIBCALL PubMatch PROTO((ValNodePtr a, ValNodePtr b));
NLM_EXTERN Int2 LIBCALL PubEquivMatch PROTO((ValNodePtr a, ValNodePtr b));

/*****************************************************************************
*
*   PubSet is a choice using an ValNode, PubSet->data.ptrvalue is chain of
*       Pubs (ValNodes) holding data for set for all types.
*   PubSet->choice:
*   0 = not set
    1 = pub Pub    -- set of real Pubs
                   -- the rest are implemented as Pubs anyway
    3 = medline Medline-entry ,
    5 = article Cit-art ,
    6 = journal Cit-jour ,
    7 = book Cit-book ,
    8 = proc Cit-proc ,      -- proceedings of a meeting
    9 = patent Cit-pat ,
*
*****************************************************************************/
NLM_EXTERN Boolean    LIBCALL PubSetAsnWrite PROTO((ValNodePtr anp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN ValNodePtr LIBCALL PubSetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN ValNodePtr LIBCALL PubSetFree PROTO((ValNodePtr anp));
NLM_EXTERN Int2 LIBCALL PubSetLabel PROTO((ValNodePtr pubset, CharPtr buffer, Int2 buflen, Uint1 content));

/*****************************************************************************
*
*   PubEquiv is just a chain of Pubs (ValNodes)
*
*****************************************************************************/
NLM_EXTERN Boolean    LIBCALL PubEquivAsnWrite PROTO((ValNodePtr anp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN ValNodePtr LIBCALL PubEquivAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN ValNodePtr LIBCALL PubEquivFree PROTO((ValNodePtr anp));

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
