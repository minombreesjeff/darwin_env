/* casn.h
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
* RCS $Id: casn.h,v 6.0 1997/08/25 18:12:43 madden Exp $
*
* Author:  Greg Schuler
*
* Version Creation Date: 9/23/92
*
* File Description:
	functions to decompress a compressed ASN,1 (CASN) file.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 06-28-93 Schuler     New function:  CASN_Seek()
* 10-05-94 Schuler     Prototype for CASN_NextBiostruc
* 05-16-94 Schuler     Added RCS Log directive.  Comments will henceforth
*                      be inserted automatically into the source at the
*                      time it is checked into the system.

$Log: casn.h,v $
Revision 6.0  1997/08/25 18:12:43  madden
Revision changed to 6.0

Revision 5.1  1997/06/26 21:55:23  vakatov
[PC] DLL'd "ncbicdr.lib", "ncbiacc.lib", "ncbinacc.lib" and "ncbicacc.lib"

Revision 5.0  1996/05/28 13:55:34  ostell
Set to revision 5.0

 * Revision 4.3  1995/08/28  23:20:47  kans
 * Biostruc_supported restored
 *
 * Revision 4.2  1995/08/24  13:13:00  schuler
 * Changed CASE_Type_ch to CASN_Type_ch
 *
 * Revision 4.1  1995/08/04  02:23:53  schuler
 * Added definition CASN_Type_ch (chromosome)
 *
 * Revision 4.0  1995/07/26  13:50:32  ostell
 * force revision to 4.0
 *
 * Revision 2.16  1995/07/25  18:47:57  kans
 * revert to no Biostruc_supported
 *
 * Revision 2.15  1995/06/26  18:07:13  kans
 * restored #define Biostruc_supported
 *
 * Revision 2.13  1995/06/23  16:02:43  kans
 * support for accmmdbs.c stub to resolve symbols without MMDB link
 *
 * Revision 2.12  1995/06/23  13:22:25  kans
 * Biostruc_CD_supported symbol needed for local MMDB access
 *
 * Revision 2.11  1995/06/22  21:21:22  kans
 * #define Biostruc_supported
 *
 * Revision 2.10  1995/05/16  14:12:46  schuler
 * Automatic comment insertion enabled
 *


* ==========================================================================
*/
#ifndef __CompressedASN1__
#define __CompressedASN1__

#ifndef Biostruc_supported
#define Biostruc_supported
#endif

#include <objsset.h>

#ifdef Biostruc_supported
#include <mmdbapi1.h>
#include <mmdbapi2.h>
#include <mmdbapi3.h>
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

enum CASN_Error	{
	CASN_ErrNone,			/* no error */
	CASN_ErrGeneric,		/* general error, not one listed below */
	CASN_ErrMemory,			/* memory allocation failed */
	CASN_ErrBadHandle,		/* CASN_Handle is invalid or corrupt */
	CASN_ErrFileOpen,		/* unable to open file for reading */
	CASN_ErrFileCreate,		/* unable to open file for writing */
	CASN_ErrFileRead,		/* unable to read from open file */
	CASN_ErrFileWrite,		/* unable to write to open file */
	CASN_ErrFileSeek,		/* either seek or tell failed */
	CASN_ErrFileFormat };	/* file format not recognized, or file corrupt */

typedef enum CASN_Type
{
	CASN_Type_ml,
	CASN_Type_aa,
	CASN_Type_nt,
	CASN_Type_st,
	CASN_Type_ch,
	CASN_TypeMed = -1,	/* MEDLINE record (ASN.1 type Medline-entry)*/
	CASN_TypeSeq = -2 	/* Sequence record (ASN.1 type Seq-entry)*/
}
CASN_Type;

struct casn_ioblock;
typedef struct casn_ioblock *CASN_Handle;


/* ----- high-level ----- */
NLM_EXTERN CASN_Handle   LIBCALL  CASN_Open PROTO((char *fname));
NLM_EXTERN void          LIBCALL  CASN_Close PROTO((CASN_Handle handle)); 
NLM_EXTERN AsnIo*        LIBCALL  CASN_GetAsnIoPtr PROTO((CASN_Handle handle));
NLM_EXTERN CASN_Type     LIBCALL  CASN_DocType PROTO((CASN_Handle handle));
NLM_EXTERN long          LIBCALL  CASN_DocCount PROTO((CASN_Handle handle));
NLM_EXTERN MedlineEntry* LIBCALL  CASN_NextMedlineEntry PROTO((CASN_Handle handle));
NLM_EXTERN SeqEntry*     LIBCALL  CASN_NextSeqEntry PROTO((CASN_Handle handle));
NLM_EXTERN int           LIBCALL  CASN_Seek PROTO((CASN_Handle,long offset,int origin));

#ifdef Biostruc_supported
NLM_EXTERN Biostruc*     LIBCALL  CASN_NextBiostruc PROTO((CASN_Handle handle));
#endif

/* ----- low-level ----- */
NLM_EXTERN CASN_Handle LIBCALL  CASN_New PROTO((CASN_Type doc_type, int huff_count));
NLM_EXTERN void        LIBCALL  CASN_Free PROTO((CASN_Handle));

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
