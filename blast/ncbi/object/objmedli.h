/*  objmedli.h
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
* File Name:  objmedli.h
*
* Author:  James Ostell
*   
* Version Creation Date: 1/1/91
*
* $Revision: 6.1 $
*
* File Description:  Object manager interface for module NCBI-Medline
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: objmedli.h,v $
* Revision 6.1  1997/09/03 12:55:58  ostell
* added support for MedlineEntry.status
*
* Revision 6.0  1997/08/25 18:50:16  madden
* Revision changed to 6.0
*
* Revision 4.5  1997/08/20 19:17:11  madden
* Version 6 of ASN.1
*
* Revision 4.4  1997/06/19 18:41:37  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.3  1996/05/20 19:43:50  ostell
* added pub_type
*
 * Revision 4.2  1996/03/29  21:09:59  ostell
 * added support for PubMedId
 *
 * Revision 4.1  1995/12/22  17:16:07  grisha
 * add function MedlineSiFree(), because new Client/Server
 * library for MedArch server needs this function
 *
 * Revision 4.0  1995/07/26  13:48:06  ostell
 * force revision to 4.0
 *
 * Revision 3.1  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#ifndef _NCBI_Medline_
#define _NCBI_Medline_

#ifndef _ASNTOOL_
#include <asn.h>
#endif
#ifndef _NCBI_General_
#include <objgen.h>
#endif
#ifndef _NCBI_Biblio_
#include <objbibli.h>
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
NLM_EXTERN Boolean LIBCALL MedlineAsnLoad PROTO((void));

/*****************************************************************************
*
*    Medline-mesh
*
*****************************************************************************/
typedef struct mesh {
    Boolean mp;                   /* main point */
    CharPtr term;
    ValNodePtr qual;
    struct mesh PNTR next;
 } MedlineMesh, PNTR MedlineMeshPtr;

NLM_EXTERN MedlineMeshPtr LIBCALL MedlineMeshNew PROTO((void));
NLM_EXTERN MedlineMeshPtr LIBCALL MedlineMeshFree PROTO((MedlineMeshPtr mmp));
NLM_EXTERN MedlineMeshPtr LIBCALL MedlineMeshAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean        LIBCALL MedlineMeshAsnWrite PROTO((MedlineMeshPtr mmp, AsnIoPtr aip, AsnTypePtr atp));

/*****************************************************************************
*
*    Medline-rn
*
*****************************************************************************/
typedef struct rn {
    Uint1 type;                
    CharPtr cit,
            name;
    struct rn PNTR next;
 } MedlineRn, PNTR MedlineRnPtr;

NLM_EXTERN MedlineRnPtr LIBCALL MedlineRnNew PROTO((void));
NLM_EXTERN MedlineRnPtr LIBCALL MedlineRnFree PROTO((MedlineRnPtr mrp));
NLM_EXTERN MedlineRnPtr LIBCALL MedlineRnAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean      LIBCALL MedlineRnAsnWrite PROTO((MedlineRnPtr mrp, AsnIoPtr aip, AsnTypePtr atp));

/*****************************************************************************
*
*    Medline-si
*      ValNode used for structure
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL MedlineSiFree PROTO((ValNodePtr msp));
NLM_EXTERN ValNodePtr LIBCALL MedlineSiAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean    LIBCALL MedlineSiAsnWrite PROTO((ValNodePtr msp, AsnIoPtr aip, AsnTypePtr atp));

/*****************************************************************************
*
*    DocRef
*
*****************************************************************************/
typedef struct df {
    Uint1 type;                
	Int4 uid;
    struct df PNTR next;
 } DocRef, PNTR DocRefPtr;

NLM_EXTERN DocRefPtr LIBCALL DocRefNew PROTO((void));
NLM_EXTERN DocRefPtr LIBCALL DocRefFree PROTO((DocRefPtr drp));
NLM_EXTERN DocRefPtr LIBCALL DocRefAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean      LIBCALL DocRefAsnWrite PROTO((DocRefPtr drp, AsnIoPtr aip, AsnTypePtr atp));


/*****************************************************************************
*
*    Medline-field
*
*****************************************************************************/
typedef struct mlf {
    Uint1 type;                
    CharPtr str;
    DocRefPtr ids;
    struct mlf PNTR next;
 } MedlineField, PNTR MedlineFieldPtr;

NLM_EXTERN MedlineFieldPtr LIBCALL MedlineFieldNew PROTO((void));
NLM_EXTERN MedlineFieldPtr LIBCALL MedlineFieldFree PROTO((MedlineFieldPtr mfp));
NLM_EXTERN MedlineFieldPtr LIBCALL MedlineFieldAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean      LIBCALL MedlineFieldAsnWrite PROTO((MedlineFieldPtr mfp, AsnIoPtr aip, AsnTypePtr atp));


/*****************************************************************************
*
*   Medline-entry
*
*****************************************************************************/
typedef struct medline {
    Int4 uid;
    DatePtr em;
    CitArtPtr cit;
    CharPtr abstract;
    MedlineMeshPtr mesh;
    MedlineRnPtr substance;
    ValNodePtr xref;
    ValNodePtr idnum;
    ValNodePtr gene;
	Int4 pmid;         /* PubMedId */
	ValNodePtr pub_type;
	MedlineFieldPtr mlfield;
    Uint1 status; /* 1=supplied by publisher, 2=premedline, 3 or 0 = medline*/
} MedlineEntry, PNTR MedlineEntryPtr;

NLM_EXTERN MedlineEntryPtr LIBCALL MedlineEntryNew PROTO((void));
NLM_EXTERN MedlineEntryPtr LIBCALL MedlineEntryFree PROTO((MedlineEntryPtr mep));
NLM_EXTERN MedlineEntryPtr LIBCALL MedlineEntryAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean         LIBCALL MedlineEntryAsnWrite PROTO((MedlineEntryPtr mep, AsnIoPtr aip, AsnTypePtr atp));

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
