/*   cdentrez.h
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
* RCS $Id: cdentrez.h,v 6.0 1997/08/25 18:12:58 madden Exp $
*
* Author:  Ostell, Kans
*
* Version Creation Date:   10/15/91
*
* File Description: 
*   	entrez index access library for Entrez CDROM
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* July'94  Schuler     Multiple changes to support new CD-ROM formats
* 1/24/95  Kans        Define _NEW_CdEntrez_
* 05-16-94 Schuler     Added RCS Log directive.  Comments will henceforth
*                      be inserted automatically into the source at the
*                      time it is checked into the system.

$Log: cdentrez.h,v $
Revision 6.0  1997/08/25 18:12:58  madden
Revision changed to 6.0

Revision 5.2  1997/06/26 21:55:35  vakatov
[PC] DLL'd "ncbicdr.lib", "ncbiacc.lib", "ncbinacc.lib" and "ncbicacc.lib"

Revision 5.1  1996/08/14 19:57:08  epstein
add APIs for fetching pieces of biostruc annots

 * Revision 5.0  1996/05/28  13:55:34  ostell
 * Set to revision 5.0
 *
 * Revision 4.3  1996/03/29  18:52:30  epstein
 * add support for structure alignments
 *
 * Revision 4.2  1995/10/02  02:35:42  epstein
 * add range-checking
 *
 * Revision 4.1  1995/08/11  20:26:11  epstein
 * add max-models support for biostrucs
 *
 * Revision 4.0  1995/07/26  13:50:32  ostell
 * force revision to 4.0
 *
 * Revision 2.24  1995/06/29  15:57:13  epstein
 * added Complexity argument when fetching structures
 *
 * Revision 2.23  95/06/23  16:02:43  kans
 * support for accmmdbs.c stub to resolve symbols without MMDB link
 * 
 * Revision 2.22  1995/06/23  13:22:25  kans
 * Biostruc_CD_supported symbol needed for local MMDB access
 *
 * Revision 2.21  1995/05/16  14:12:46  schuler
 * Automatic comment insertion enabled
 *



* ==========================================================================
*/

#ifndef _CDENTREZ_
#define _CDENTREZ_

#ifndef _NEW_CdEntrez_
/* only use new symbol */
#define _NEW_CdEntrez_
#endif

#if !defined(_NEW_CdEntrez_) && !defined(_OLD_CdEntrez_)
/* if neither symbol defined, default to using both */
#define _OLD_CdEntrez_
#define _NEW_CdEntrez_
#endif

#ifndef _ACCENTR_
#include <accentr.h>
#endif
#ifndef _CDROMLIB_
#include <cdromlib.h>
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

/**** Initialize and close session *********************/

NLM_EXTERN Boolean  CdEntrezInit PROTO((Boolean no_warnings));
NLM_EXTERN void  CdEntrezFini PROTO((void));

/**** Get Types and Terms ******************************/
/**** prototypes are in cdromlib.h *********************/

NLM_EXTERN EntrezInfo* CdEntrezGetInfo PROTO((void));
NLM_EXTERN char* CdEntrezDetailedInfo PROTO((void));
/*#define CdEntrezGetInfo CdGetInfo*/
/*#define CdEntrezDetailedInfo CdDetailedInfo*/

/**** Get Links and Neighbors **************************/

NLM_EXTERN Int4 CdEntGetMaxLinks PROTO((void));

NLM_EXTERN LinkSetPtr  CdUidLinks PROTO((DocType type, DocUid uid, DocType link_to_type));
NLM_EXTERN Int2  CdLinkUidList PROTO((LinkSetPtr PNTR result, DocType type,
		DocType link_to_type, Int2 numuid, Int4Ptr uids, Boolean mark_missing));

/**** Get Summaries ************************************/

NLM_EXTERN DocSumPtr  CdDocSum PROTO((DocType type, DocUid uid));
NLM_EXTERN Int2  CdEntMlSumListGet PROTO((DocSumPtr PNTR result, Int2 numuid, Int4Ptr uids));
NLM_EXTERN Int2  CdEntSeqSumListGet PROTO((DocSumPtr PNTR result, Int2 numuid, DocType db, Int4Ptr uids));
#ifdef _NEW_CdEntrez_
NLM_EXTERN int  CdDocSumListGet PROTO((DocSum **result, int numuid, DocType type, const DocUid *uids));
#endif

/**** Get Term List ************************************/

NLM_EXTERN Int2  CdTermListByPage PROTO((DocType type, DocField field,
				Int2 page, Int2 numpage, TermListProc proc));
NLM_EXTERN Int2  CdTermListByTerm PROTO((DocType type, DocField field,
				CharPtr term, Int2 numterms, TermListProc proc, Int2Ptr first_page));

NLM_EXTERN Boolean  CdEntrezFindTerm PROTO((DocType type, DocField field, CharPtr term, Int4Ptr spcl, Int4Ptr totl));

/**** Creates a term node from the uid parameter ********/
NLM_EXTERN void  CdEntrezCreateNamedUidList PROTO((CharPtr term, DocType type, DocField field, Int4 num, DocUidPtr uids));
NLM_EXTERN void  CdEntrezCreateNamedUidListX PROTO((CharPtr term, DocType type, DocField field, ByteStorePtr bsp));

/**** Look up terms with Boolean operations ************/

NLM_EXTERN ValNodePtr  CdEntTLNew PROTO((DocType type));
NLM_EXTERN ValNodePtr  CdEntTLAddTerm PROTO((ValNodePtr elst, CharPtr term, DocType type, DocField field, Boolean special, CharPtr highRange));
NLM_EXTERN ValNodePtr  CdEntTLFree PROTO((ValNodePtr elst));
NLM_EXTERN LinkSetPtr  CdEntTLEval PROTO((ValNodePtr elst));
NLM_EXTERN Int4  CdEntTLEvalCount PROTO((ValNodePtr elst));

NLM_EXTERN ByteStorePtr  CdEntTLEvalX PROTO((ValNodePtr elst));

/**** Retrieve Data ***********************************/

NLM_EXTERN Int2  CdEntMedlineEntryListGet PROTO((MedlineEntryPtr PNTR result, Int2 numuid,
		Int4Ptr uids, Boolean mark_missing));
NLM_EXTERN Int2  CdEntSeqEntryListGet PROTO((SeqEntryPtr PNTR result, Int2 numuid,
		Int4Ptr uids, Int2 retcode, Boolean mark_missing));

/* may be called by Network Entrez server */
NLM_EXTERN DocSumPtr CdSeqSumAsnRead PROTO((AsnIoPtr aip, DocUid uid));

/**** Get a SeqId for a GI ***************************/

NLM_EXTERN SeqIdPtr CdSeqIdForGI PROTO((Int4 gi));
NLM_EXTERN Int4  CdEntrezFindSeqId PROTO((SeqIdPtr sip));

/**** get a 3-d structure *********************/
#ifdef Biostruc_supported
NLM_EXTERN BiostrucPtr CdEntrezBiostrucGet PROTO((DocUid uid, Int4 mdlLvl, Int4 maxModels));
NLM_EXTERN BiostrucAnnotSetPtr CdEntrezBiostrucAnnotSetGet PROTO((DocUid uid));
NLM_EXTERN LinkSetPtr LIBCALL CdEntrezBiostrucFeatIds PROTO((DocUid mmdbid, Int2 feature_type, Int4 feature_set_id));
NLM_EXTERN BiostrucAnnotSetPtr LIBCALL CdEntrezBiostrucAnnotSetGetByFid PROTO((DocUid mmdbid, Int4 feature_id, Int4 feature_set_id));
#endif /* Biostruc_supported */

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


