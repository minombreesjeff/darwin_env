/*  objsub.h
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
* RCS $Id: objsub.h,v 6.2 1999/09/27 17:48:39 kans Exp $
*
* Author:  James Ostell
*   
* Version Creation Date: 6/20/92
*
* $Revision: 6.2 $
*
* File Description:  Object manager interface for module NCBI-Sub
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: objsub.h,v $
* Revision 6.2  1999/09/27 17:48:39  kans
* using GatherIndex structure
*
* Revision 6.1  1999/09/26 19:42:51  kans
* added EXTRA_OBJMGR_FIELDS to SeqSubmit
*
* Revision 6.0  1997/08/25 18:51:04  madden
* Revision changed to 6.0
*
* Revision 4.1  1997/06/19 18:42:14  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.0  1995/07/26 13:48:06  ostell
* force revision to 4.0
*
 * Revision 3.5  1995/06/20  15:29:39  schuler
 * Eliminated name conflict on struct/struc member "conflict"
 *
 * Revision 3.4  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#ifndef _NCBI_Submit_
#define _NCBI_Submit_

#ifndef _NCBI_Seqset_
#include <objsset.h>
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
NLM_EXTERN Boolean LIBCALL SubmitAsnLoad PROTO((void));

/*****************************************************************************
*
*   internal structures for NCBI-Submit objects
*
*****************************************************************************/

/*****************************************************************************
*
*   Contact Info
*
*****************************************************************************/
typedef struct ContactInfo {
	CharPtr name;
	ValNodePtr address;            /* chain of strings */
	CharPtr phone,
		fax,
		email,
		telex;
	ObjectIdPtr owner_id;
	ByteStorePtr password;
        CharPtr last_name,
                first_name,
                middle_initial;
	AuthorPtr contact;
} ContactInfo, PNTR ContactInfoPtr;

NLM_EXTERN ContactInfoPtr LIBCALL ContactInfoNew PROTO((void));
NLM_EXTERN ContactInfoPtr LIBCALL ContactInfoFree PROTO(( ContactInfoPtr cip));
NLM_EXTERN ContactInfoPtr LIBCALL ContactInfoAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean        LIBCALL ContactInfoAsnWrite PROTO((ContactInfoPtr cip, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Int2 LIBCALL ContactInfoLabel PROTO((ContactInfoPtr cip, CharPtr buffer, Int2 buflen, Uint1 content));

/*****************************************************************************
*
*   Submit-block
*
*****************************************************************************/
typedef struct subblk {
	ContactInfoPtr contact;
	CitSubPtr cit;
	Boolean hup;      /* hold until published */
	DatePtr reldate;  /* release date */
	Uint1 subtype;    /* submission type */
	CharPtr tool,     /* tool that made this submission */
		user_tag,     /* user "handle" to identify this submission */
		comment;      /* instructions to submission staff */
} SubmitBlock, PNTR SubmitBlockPtr;

NLM_EXTERN SubmitBlockPtr LIBCALL SubmitBlockNew PROTO((void));
NLM_EXTERN SubmitBlockPtr LIBCALL SubmitBlockFree PROTO(( SubmitBlockPtr sbp));
NLM_EXTERN SubmitBlockPtr LIBCALL SubmitBlockAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean        LIBCALL SubmitBlockAsnWrite PROTO((SubmitBlockPtr sbp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Int2 LIBCALL SubmitBlockLabel PROTO((SubmitBlockPtr sbp, CharPtr buffer, Int2 buflen, Uint1 content));

/*****************************************************************************
*
*   Seq-submit
*   	datatype =
*   		0 = not-set (error)
*   		1 = Seq-entry(s) (data = SeqEntryPtr, possibly a chain)
*   		2 = Seq-annot(s) (data = SeqAnnotPtr, possibly a chain)
*   		3 = Seq-id(s) for delete (data = SeqIdPtr, possibly a chain)
*
*****************************************************************************/
typedef struct seqsub {
	SubmitBlockPtr sub;
	Uint1 datatype;
	Pointer data;
	GatherIndex idx;     /* internal gather/objmgr tracking fields */
} SeqSubmit, PNTR SeqSubmitPtr;

NLM_EXTERN SeqSubmitPtr LIBCALL SeqSubmitNew PROTO((void));
NLM_EXTERN SeqSubmitPtr LIBCALL SeqSubmitFree PROTO(( SeqSubmitPtr ssp));
NLM_EXTERN SeqSubmitPtr LIBCALL SeqSubmitAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean      LIBCALL SeqSubmitAsnWrite PROTO((SeqSubmitPtr ssp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Int2 LIBCALL SeqSubmitLabel PROTO((SeqSubmitPtr ssp, CharPtr buffer, Int2 buflen, Uint1 content));

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

