/*  valid.h
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
* File Name:  valid.h
*
* Author:  James Ostell
*   
* Version Creation Date: 1/1/94
*
* $Revision: 6.5 $
*
* File Description:  Sequence editing utilities
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: valid.h,v $
* Revision 6.5  2000/02/14 15:00:19  kans
* added vsp->farIDsInAlignments for use by alignment validator
*
* Revision 6.4  1999/12/24 01:21:06  kans
* added validateAlignments flag controlling call to ValidateSeqAlignWithinValidator
*
* Revision 6.3  1999/07/22 22:04:36  kans
* added suppressContext flag
*
* Revision 6.2  1999/05/03 20:06:36  kans
* if no pubs or no biosource, report only once, not once per bioseq
*
* Revision 6.1  1998/07/02 17:53:47  kans
* useSeqMgrIndexes field added to ValidStructPtr, validator can use either old (nested gathers) or new (SeqMgr indexing) method
*
* Revision 6.0  1997/08/25 18:08:29  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/06/19 18:39:54  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.0  1996/05/28 13:23:23  ostell
* Set to revision 5.0
*
 * Revision 4.1  1996/01/23  23:10:10  kans
 * added onlyspell and justwarnonspell to ValidStructPtr
 *
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 1.4  1995/06/03  13:45:47  ostell
 * changes made in valid to use gather functions and ErrPostItem instead
 * of previous custom functions
 *
 * Revision 1.3  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#ifndef _NCBI_Valid_
#define _NCBI_Valid_

/*****************************************************************************
*
*   valid.h
*   	values for cutoff are
*       0 INFO
*       1 WARN
*       2 ERROR
*       3 FATAL
*
*****************************************************************************/
#ifndef _NCBI_Seqport_
#include <seqport.h>
#endif

#ifndef _GATHER_
#include <gather.h>
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

typedef void (*SpellCallBackFunc) (char * str);
typedef int (* SpellCheckFunc) (char *String, SpellCallBackFunc);


#define SET_DEPTH 20

typedef struct validstruct {
	Int2 cutoff;                   /* lowest errmsg to show 0=default */
	Int2 errors[6];
	SeqEntryPtr sep;               /* top level SeqEntryPtr */
	BioseqSetPtr bssp;			   /* current bioseqset */
	BioseqPtr bsp;                 /* current bioseq */
	SeqFeatPtr sfp;                /* current feature */
	ValNodePtr descr;              /* current descriptor */
	Uint4 descrs [SET_DEPTH];      /* bit flags set by descriptor type */
	Int2 protcnt, nuccnt, segcnt;
	CharPtr errbuf;
	Boolean patch_seq;             /* repair invalid sequence residues? */
	Boolean non_ascii_chars;       /* non ascii chars found in read? */
	Boolean suppress_no_pubs;
	Boolean suppress_no_biosrc;
	SpellCheckFunc spellfunc;
	SpellCallBackFunc spellcallback;
	GatherContextPtr gcp;          /* used for reporting the errors */
								   /* this section used for checking Bioseqs */
	Uint2 bsp_partial_val;         /* return from SeqLocPartial on segmented SeqLoc */
	Boolean onlyspell;             /* only do spell check */
	Boolean justwarnonspell;       /* severity WARNING instead of ERROR on spell */
	Boolean useSeqMgrIndexes;      /* new style indexing to speed up validation */
	Boolean suppressContext;       /* suppress context part of message */
	Boolean validateAlignments;    /* call alignval test suite */
	Boolean farIDsInAlignments;    /* fetch to get far IDs in alignments */
} ValidStruct, PNTR ValidStructPtr;

NLM_EXTERN Boolean ValidateSeqEntry PROTO((SeqEntryPtr sep, ValidStructPtr vsp));
NLM_EXTERN void ValidStructClear (ValidStructPtr vsp);  /* 0 out a ValidStruct */
NLM_EXTERN ValidStructPtr ValidStructNew (void);
NLM_EXTERN ValidStructPtr ValidStructFree (ValidStructPtr vsp);
NLM_EXTERN void ReportNonAscii PROTO((ValidStructPtr vsp, SeqEntryPtr sep));
NLM_EXTERN void SpellCallBack (char * str);

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
