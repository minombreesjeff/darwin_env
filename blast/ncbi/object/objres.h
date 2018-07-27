/*  objres.h
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
* File Name:  objres.h
*
* Author:  James Ostell
*   
* Version Creation Date: 4/1/91
*
* $Revision: 6.3 $
*
* File Description:  Object manager interface for module NCBI-Seqres
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: objres.h,v $
* Revision 6.3  1999/09/27 17:48:38  kans
* using GatherIndex structure
*
* Revision 6.2  1999/09/24 23:09:23  kans
* adds EXTRA_OBJMGR_FIELDS to several objects
*
* Revision 6.1  1998/12/09 20:37:53  kans
* changed compl to compr to avoid new c++ symbol collision
*
* Revision 6.0  1997/08/25 18:50:41  madden
* Revision changed to 6.0
*
* Revision 4.1  1997/06/19 18:41:54  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.0  1995/07/26 13:48:06  ostell
* force revision to 4.0
*
 * Revision 3.2  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#ifndef _NCBI_Seqres_
#define _NCBI_Seqres_

#ifndef _ASNTOOL_
#include <asn.h>
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

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   loader
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqResAsnLoad PROTO((void));

/*****************************************************************************
*
*   internal structures for NCBI-SeqRes objects
*
*****************************************************************************/

/*****************************************************************************
*
*   SeqGraph
*
*****************************************************************************/
typedef struct seqgraph {
    CharPtr title,
        comment;
    SeqLocPtr loc;     /* SeqLoc */
    CharPtr titlex,
        titley;
    Uint1 flags[3];   /* [0]-comp used?,[1]-a,b used?, [2] graphtype */
    Int4 compr;                       /* 1=real, 2=int, 3=byte */
    FloatHi a, b;
    Int4 numval;
    DataVal max,
        min,
        axis;
    Pointer values;  /* real=array of FloatHi, int=array of Int4, */
    struct seqgraph PNTR next;              /* byte = ByteStore */
	GatherIndex idx;      /* internal gather/objmgr tracking fields */
} SeqGraph, PNTR SeqGraphPtr;

NLM_EXTERN SeqGraphPtr LIBCALL SeqGraphNew PROTO((void));
NLM_EXTERN Boolean     LIBCALL SeqGraphAsnWrite PROTO((SeqGraphPtr sgp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqGraphPtr LIBCALL SeqGraphAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqGraphPtr LIBCALL SeqGraphFree PROTO((SeqGraphPtr sgp));
NLM_EXTERN Int2 LIBCALL SeqGraphLabel PROTO((SeqGraphPtr sgp, CharPtr buffer, Int2 buflen, Uint1 content));

/*****************************************************************************
*
*   SeqGraphSet
*
*****************************************************************************/
NLM_EXTERN Boolean     LIBCALL SeqGraphSetAsnWrite PROTO((SeqGraphPtr sgp, AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element));
NLM_EXTERN SeqGraphPtr LIBCALL SeqGraphSetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element));

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
