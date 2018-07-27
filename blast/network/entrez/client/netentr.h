/*   netentr.h
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
* File Name:  netentr.h
*
* Author:  Ostell, Kans, Epstein
*
* Version Creation Date:   10/15/91
*
* $Revision: 6.2 $
*
* File Description: 
*       entrez index access library for Entrez network version
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 8-16-94  Brylawski   Added declarations for on-the-fly neighboring,
*                      medline explosion, and mesh tree browsing.
*
* 11-20-94 Brylawski   Modified NetEntDoNeighborText to pass user preferences.
*
* ==========================================================================
* $Log: netentr.h,v $
* Revision 6.2  2000/07/08 20:44:03  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 6.1  1998/02/21 22:34:38  kans
* NetEntHierarchyGet passes type and field
*
* Revision 6.0  1997/08/25 18:34:57  madden
* Revision changed to 6.0
*
* Revision 5.2  1997/07/29 21:24:13  vakatov
* [WIN32,DLL]  DLL'zation of "netentr.lib"
*
* Revision 5.1  1996/08/14 19:47:49  epstein
* add annot get by feat ids, and also add date filtering support
*
 * Revision 5.0  1996/05/28  14:10:21  ostell
 * Set to revision 5.0
 *
 * Revision 4.5  1996/03/30  15:10:43  ostell
 * changed LIBCALL to CDECL in NetEntrezBiostructAnnotSetGet
 *
 * Revision 4.4  1996/03/29  18:55:49  epstein
 * add support for structure alignments
 *
 * Revision 4.3  1995/10/02  15:29:05  epstein
 * support range-checking
 *
 * Revision 4.2  1995/08/22  19:35:27  epstein
 * add clustering support
 *
 * Revision 4.1  1995/08/11  20:28:03  epstein
 * add max-models support for biostrucs
 *
 * Revision 4.0  1995/07/26  13:54:59  ostell
 * force revision to 4.0
 *
 * Revision 1.18  1995/06/29  16:36:46  epstein
 * add biostruc-complexity
*/

#ifndef _NETENTREZ_
#define _NETENTREZ_

#ifndef _ENTREZ_
#include <accentr.h>
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

#define TERM_MAX 80

struct termresp {
    CharPtr term;
    Int4    special_count;
    Int4    total_count;
};

typedef struct {
    Int4    num_terms;
    Int4    first_page;
    Int4    num_pages_read;
    struct termresp PNTR termresp;
} TermResp, PNTR TermRespPtr;

typedef struct {
    DocType   type;
    DocField  field;
    CharPtr   term;
    CharPtr   highRange;
} TermData, PNTR TermDataPtr;


/**** Initialize and close session *********************/

NLM_EXTERN Boolean CDECL NetEntrezInit PROTO((CharPtr appl_id, Boolean no_warnings));

NLM_EXTERN void CDECL NetEntrezFini PROTO((void));

NLM_EXTERN EntrezInfoPtr NetEntrezGetInfo PROTO((void));
NLM_EXTERN CharPtr NetEntrezDetailedInfo PROTO((void));

/**** Get Types and Terms ******************************/
/**** prototypes are in netlib.h *********************/

/**** Get Links and Neighbors **************************/

NLM_EXTERN Int4 NetEntGetMaxLinks PROTO((void));

NLM_EXTERN LinkSetPtr CDECL NetUidLinks PROTO((DocType type, DocUid uid, DocType link_to_type));
NLM_EXTERN Int2 CDECL NetLinkUidList PROTO((LinkSetPtr PNTR result, DocType type,
DocType link_to_type, Int2 numuid, DocUidPtr uids, Boolean mark_missing));

/**** Get Summaries ************************************/

NLM_EXTERN Int2 CDECL NetDocSumListGet PROTO((DocSumPtr PNTR result, Int2 numuid, DocType type, DocUidPtr uids, Int2 defer_count));
NLM_EXTERN DocSumPtr CDECL NetDocSum PROTO((DocType type, DocUid uid));

/**** Get Term List ************************************/

NLM_EXTERN Int2 CDECL NetTermListByPage PROTO((DocType type, DocField field, Int2 page, Int2 numpage, TermListProc proc));
NLM_EXTERN Int2 CDECL NetTermListByTerm PROTO((DocType type, DocField field, CharPtr term, Int2 numterms, TermListProc proc, Int2Ptr first_page));

NLM_EXTERN Boolean CDECL NetEntrezFindTerm PROTO((DocType type, DocField field, CharPtr term, Int4Ptr spcl, Int4Ptr totl));

/**** Creates a term node from the uid parameter ********/
NLM_EXTERN void CDECL NetEntrezCreateNamedUidList PROTO((CharPtr term, DocType type, DocField field, Int4 num, DocUidPtr uids));
NLM_EXTERN void CDECL NetEntrezCreateNamedUidListX PROTO((CharPtr term, DocType type, DocField field, ByteStorePtr bsp));


/**** Look up terms with Boolean operations ************/

NLM_EXTERN ValNodePtr CDECL NetEntTLNew PROTO((DocType type));
NLM_EXTERN ValNodePtr CDECL NetEntTLAddTerm PROTO((ValNodePtr elst, CharPtr term, DocType type, DocField field, Boolean special, CharPtr highRange));
NLM_EXTERN ValNodePtr CDECL NetEntTLFree PROTO((ValNodePtr elst));
NLM_EXTERN LinkSetPtr CDECL NetEntTLEval PROTO((ValNodePtr elst));
NLM_EXTERN ByteStorePtr CDECL NetEntTLEvalX PROTO((ValNodePtr elst));
NLM_EXTERN Int4 CDECL NetEntTLEvalCount PROTO((ValNodePtr elst));

/**** Get the Data **************************************/

NLM_EXTERN Int2 CDECL NetEntMedlineEntryListGet PROTO((MedlineEntryPtr PNTR result, Int2 numuid, DocUidPtr uids, Boolean mark_missing));

NLM_EXTERN Int2 CDECL NetEntSeqEntryListGet PROTO((SeqEntryPtr PNTR result, Int2 numuid,
        DocUidPtr uids, Int2 retcode, Boolean mark_missing));

NLM_EXTERN Int2 CDECL NetEntrezBiostrucListGet PROTO((BiostrucPtr PNTR result, Int4 mdlLvl, Int4 maxModels, Int2 numuid, DocUidPtr uids, Boolean mark_missing));
NLM_EXTERN BiostrucAnnotSetPtr CDECL NetEntrezBiostrucAnnotSetGet PROTO((DocUid uid));
NLM_EXTERN LinkSetPtr CDECL NetEntrezBiostrucFeatIds PROTO((DocUid mmdbid, Int2 feature_type, Int4 feature_set_id));
NLM_EXTERN BiostrucAnnotSetPtr CDECL NetEntrezBiostrucAnnotSetGetByFid  PROTO((DocUid mmdbid, Int4 feature_id, Int4 feature_set_id));



/**** Get a SeqId for a GI ***************************/

NLM_EXTERN SeqIdPtr NetSeqIdForGI PROTO((Int4 gi));
NLM_EXTERN Int4 NetEntrezFindSeqId PROTO((SeqIdPtr sip));
NLM_EXTERN EntrezHierarchyPtr NetEntMeshHierarchyGet PROTO((CharPtr term));
NLM_EXTERN EntrezHierarchyPtr NetEntHierarchyGet PROTO((CharPtr term, DocType type, DocField field));

/**** Neighbor some text on-the-fly ! ************/
NLM_EXTERN LinkSetPtr CDECL NetEntDoNeighborText PROTO((EntrezNeighborTextPtr entp));
NLM_EXTERN Boolean CDECL NetEntCanNeighborText PROTO((void));

/**** BLAST a sequence on-the-fly ! ************/
NLM_EXTERN Boolean CDECL NetEntCanBlast PROTO((void));
NLM_EXTERN LinkSetPtr LIBCALL NetEntBlastBioseq PROTO((BioseqPtr bsp, DocType db, CharPtr program, CharPtr database, CharPtr options, Boolean usemonitor));



NLM_EXTERN Boolean CDECL NetEntExpandedMedlineFeatures PROTO((void));
NLM_EXTERN Int4 LIBCALL NetEntClusterAnalysis PROTO((DocUidPtr uids, Int4 numuids, DocField fld, Int4 minCluster, Int4 maxCluster, Int4 maxTerms, CharPtr *terms, Int4Ptr termTotals));

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif


/* # of retries to get a server */
#define ENT_SERV_RETRIES 2


#ifndef _NETROMLIB_
#include <netlib.h>
#endif

#endif





