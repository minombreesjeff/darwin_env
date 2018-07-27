/*  objneten.h
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
* File Name:  objneten.h
*
* Author:  Epstein
*   
* Version Creation Date: 6/3/92
*
* $Revision: 6.0 $
*
* File Description:  Object loader interface for module NCBI-Entrez
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 8-16-94  Brylawski   Added declarations for EntrezNeighborText and
*                      EntrezHierarchy loaders and structures.
*
* 11-20-94 Brylawski   Moved the EntrezNeighborText structure to accentr.h
*                      to permit its use in chosen.c and netentr.c .
* ==========================================================================
*
*
* RCS Modification History:
* $Log: objneten.h,v $
* Revision 6.0  1997/08/25 18:35:13  madden
* Revision changed to 6.0
*
* Revision 5.2  1997/07/29 21:24:27  vakatov
* [WIN32,DLL]  DLL'zation of "netentr.lib"
*
* Revision 5.1  1996/08/14 19:48:21  epstein
* add annot get by feat ids, and also add date filtering support
*
 * Revision 5.0  1996/05/28  14:10:21  ostell
 * Set to revision 5.0
 *
 * Revision 4.2  1995/08/22  19:35:04  epstein
 * add clustering support
 *
 * Revision 4.1  1995/08/08  15:03:11  epstein
 * fix NetEntAsnLoad prototype per J. Zhang
 *
 * Revision 4.0  1995/07/26  13:54:59  ostell
 * force revision to 4.0
 *
 * Revision 1.14  1995/07/11  12:30:30  epstein
 * change CDECLs to LIBCALLs
 *
 * Revision 1.13  1995/07/10  19:39:04  epstein
 * add docsumX
 *
 * Revision 1.12  1995/06/23  13:21:16  kans
 * include <accentr.h> to pull in Biostruc_supported symbol
 *
 * Revision 1.11  1995/05/17  17:53:35  epstein
 * add RCS log revision history
 *
*/

#ifndef _NCBI_NetEntrez_
#define _NCBI_NetEntrez_

#ifndef _ASNTOOL_
#include <asn.h>
#endif

/* accentr.h is included to define Biostruc_supported */
#ifndef _ACCENTR_
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

/*****************************************************************************
*
*   loader
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL NetEntAsnLoad PROTO((void));

/*****************************************************************************
*
*   internal structures for NCBI-Entrez objects
*
*****************************************************************************/

/*****************************************************************************
*
*   Entrez-ids
*
*****************************************************************************/


typedef struct {
	Int4 numid;
	DocUidPtr ids;
} EntrezIds, PNTR EntrezIdsPtr;

NLM_EXTERN EntrezIdsPtr LIBCALL EntrezIdsNew PROTO((void));
NLM_EXTERN EntrezIdsPtr LIBCALL EntrezIdsFree PROTO(( EntrezIdsPtr ufp));
NLM_EXTERN EntrezIdsPtr LIBCALL EntrezIdsAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean LIBCALL EntrezIdsAsnWrite PROTO((EntrezIdsPtr ufp, AsnIoPtr aip, AsnTypePtr atp));


/*****************************************************************************
*
*   Marked-link-set
*
*****************************************************************************/
typedef struct {
	LinkSetPtr link_set;
	Int4 uids_processed;
	EntrezIdsPtr  marked_missing;
} MarkedLinkSet, PNTR MarkedLinkSetPtr;


NLM_EXTERN MarkedLinkSetPtr LIBCALL MarkedLinkSetNew PROTO((void));
NLM_EXTERN MarkedLinkSetPtr LIBCALL MarkedLinkSetFree PROTO(( MarkedLinkSetPtr ufp));
NLM_EXTERN MarkedLinkSetPtr LIBCALL MarkedLinkSetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean LIBCALL MarkedLinkSetAsnWrite PROTO((MarkedLinkSetPtr ufp, AsnIoPtr aip, AsnTypePtr atp));


/*****************************************************************************
*
*   Named-List
*
*****************************************************************************/

typedef struct {
	DocType type;
	DocField fld;
	CharPtr term;
	EntrezIdsPtr uids;
} NamedList, PNTR NamedListPtr;

NLM_EXTERN NamedListPtr LIBCALL NamedListNew PROTO((void));
NLM_EXTERN NamedListPtr LIBCALL NamedListFree PROTO(( NamedListPtr ufp));
NLM_EXTERN NamedListPtr LIBCALL NamedListAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean LIBCALL NamedListAsnWrite PROTO((NamedListPtr ufp, AsnIoPtr aip, AsnTypePtr atp));


/*****************************************************************************
*
*   Term-by-Page
*
*****************************************************************************/

typedef struct {
	DocType type;
	DocField fld;
	Int4 page;
	Int4 num_pages;
} TermByPage, PNTR TermByPagePtr;

NLM_EXTERN TermByPagePtr LIBCALL TermByPageNew PROTO((void));
NLM_EXTERN TermByPagePtr LIBCALL TermByPageFree PROTO(( TermByPagePtr ufp));
NLM_EXTERN TermByPagePtr LIBCALL TermByPageAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean LIBCALL TermByPageAsnWrite PROTO((TermByPagePtr ufp, AsnIoPtr aip, AsnTypePtr atp));


/*****************************************************************************
*
*   Term-by-Term
*
*****************************************************************************/

typedef struct {
	DocType type;
	DocField fld;
	CharPtr term;
	Int4 num_terms;
} TermByTerm, PNTR TermByTermPtr;

NLM_EXTERN TermByTermPtr LIBCALL TermByTermNew PROTO((void));
NLM_EXTERN TermByTermPtr LIBCALL TermByTermFree PROTO(( TermByTermPtr ufp));
NLM_EXTERN TermByTermPtr LIBCALL TermByTermAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean LIBCALL TermByTermAsnWrite PROTO((TermByTermPtr ufp, AsnIoPtr aip, AsnTypePtr atp));

/*****************************************************************************
*
*   Term-Lookup
*
*****************************************************************************/

typedef struct {
	DocType type;
	DocField fld;
	CharPtr term;
} TermLookup, PNTR TermLookupPtr;

NLM_EXTERN TermLookupPtr LIBCALL TermLookupNew PROTO((void));
NLM_EXTERN TermLookupPtr LIBCALL TermLookupFree PROTO(( TermLookupPtr ufp));
NLM_EXTERN TermLookupPtr LIBCALL TermLookupAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean LIBCALL TermLookupAsnWrite PROTO((TermLookupPtr ufp, AsnIoPtr aip, AsnTypePtr atp));


/*****************************************************************************
*
*   Term-Page-Info
*
*****************************************************************************/

typedef struct {
	CharPtr term;
	Int4 spec_count;
	Int4 tot_count;
} TermPageInfo, PNTR TermPageInfoPtr;

NLM_EXTERN TermPageInfoPtr LIBCALL TermPageInfoNew PROTO((void));
NLM_EXTERN TermPageInfoPtr LIBCALL TermPageInfoFree PROTO(( TermPageInfoPtr ufp));
NLM_EXTERN TermPageInfoPtr LIBCALL TermPageInfoAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean LIBCALL TermPageInfoAsnWrite PROTO((TermPageInfoPtr ufp, AsnIoPtr aip, AsnTypePtr atp));


/*****************************************************************************
*
*   Term-Counts
*
*****************************************************************************/

typedef struct {
	Boolean found;
	Int4 spec_count;
	Int4 tot_count;
} TermCounts, PNTR TermCountsPtr;

NLM_EXTERN TermCountsPtr LIBCALL TermCountsNew PROTO((void));
NLM_EXTERN TermCountsPtr LIBCALL TermCountsFree PROTO(( TermCountsPtr ufp));
NLM_EXTERN TermCountsPtr LIBCALL TermCountsAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean LIBCALL TermCountsAsnWrite PROTO((TermCountsPtr ufp, AsnIoPtr aip, AsnTypePtr atp));

/*****************************************************************************
*
*   Link-SetGet
*
*****************************************************************************/

typedef struct {
	Int4 max;      /* maximum Ids to return */
	DocType link_to_cls;
	DocType query_cls;
	Boolean mark_missing;
	Int4 query_size;
	DocUidPtr query;
} LinkSetGet, PNTR LinkSetGetPtr;

NLM_EXTERN LinkSetGetPtr LIBCALL LinkSetGetNew PROTO((void));
NLM_EXTERN LinkSetGetPtr LIBCALL LinkSetGetFree PROTO(( LinkSetGetPtr ufp));
NLM_EXTERN LinkSetGetPtr LIBCALL LinkSetGetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean LIBCALL LinkSetGetAsnWrite PROTO((LinkSetGetPtr ufp, AsnIoPtr aip, AsnTypePtr atp));

NLM_EXTERN DocSumPtr LIBCALL MlSumNew PROTO((void));
NLM_EXTERN DocSumPtr LIBCALL MlSumFree PROTO(( DocSumPtr ufp));
NLM_EXTERN DocSumPtr LIBCALL MlSumAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean LIBCALL MlSumAsnWrite PROTO((DocSumPtr ufp, AsnIoPtr aip, AsnTypePtr atp));

NLM_EXTERN DocSumPtr LIBCALL SeqSumNew PROTO((void));
NLM_EXTERN DocSumPtr LIBCALL SeqSumFree PROTO(( DocSumPtr ufp));
NLM_EXTERN DocSumPtr LIBCALL SeqSumAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean LIBCALL SeqSumAsnWrite PROTO((DocSumPtr ufp, AsnIoPtr aip, AsnTypePtr atp));



/*****************************************************************************
*
*   EntrezDocGet
*
*****************************************************************************/


typedef struct {
	DocType cls;
	Boolean mark_missing;
	EntrezIdsPtr ids;
	Int4 defer_count;
} EntrezDocGet, PNTR EntrezDocGetPtr;

NLM_EXTERN EntrezDocGetPtr LIBCALL EntrezDocGetNew PROTO((void));
NLM_EXTERN EntrezDocGetPtr LIBCALL EntrezDocGetFree PROTO(( EntrezDocGetPtr ufp));
NLM_EXTERN EntrezDocGetPtr LIBCALL EntrezDocGetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean LIBCALL EntrezDocGetAsnWrite PROTO((EntrezDocGetPtr ufp, AsnIoPtr aip, AsnTypePtr atp));


/*****************************************************************************
*
*   Medline-list
*
*****************************************************************************/
typedef struct {
	Int4 num;
	MedlineEntryPtr PNTR data; /* vector */
	EntrezIdsPtr marked_missing; /* marked UIDs, optional */
} MedlineEntryList, PNTR MedlineEntryListPtr;


NLM_EXTERN MedlineEntryListPtr LIBCALL MedlineEntryListNew PROTO((void));
NLM_EXTERN MedlineEntryListPtr LIBCALL MedlineEntryListFree PROTO(( MedlineEntryListPtr ufp));
NLM_EXTERN MedlineEntryListPtr LIBCALL MedlineEntryListAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean LIBCALL MedlineEntryListAsnWrite PROTO((MedlineEntryListPtr ufp, AsnIoPtr aip, AsnTypePtr atp));

/*****************************************************************************
*
*   Ml-summary-list
*
*****************************************************************************/
typedef struct {
	Int4 num;
	DocSumPtr PNTR data; /* vector */
} MlSummaryList, PNTR MlSummaryListPtr;


NLM_EXTERN MlSummaryListPtr LIBCALL MlSummaryListNew PROTO((void));
NLM_EXTERN MlSummaryListPtr LIBCALL MlSummaryListFree PROTO(( MlSummaryListPtr ufp));
NLM_EXTERN MlSummaryListPtr LIBCALL MlSummaryListAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean LIBCALL MlSummaryListAsnWrite PROTO((MlSummaryListPtr ufp, AsnIoPtr aip, AsnTypePtr atp));

/*****************************************************************************
*
*   SeqEntryGet
*
*****************************************************************************/

typedef struct {
	EntrezIdsPtr ids;
	Int4 retype;
	Boolean mark_missing;
} EntrezSeqGet, PNTR EntrezSeqGetPtr;

NLM_EXTERN EntrezSeqGetPtr LIBCALL EntrezSeqGetNew PROTO((void));
NLM_EXTERN EntrezSeqGetPtr LIBCALL EntrezSeqGetFree PROTO(( EntrezSeqGetPtr ufp));
NLM_EXTERN EntrezSeqGetPtr LIBCALL EntrezSeqGetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean LIBCALL EntrezSeqGetAsnWrite PROTO((EntrezSeqGetPtr ufp, AsnIoPtr aip, AsnTypePtr atp));

/*****************************************************************************
*
*   Seq-summary-list
*
*****************************************************************************/
typedef struct {
	Int4 num;
	DocSumPtr PNTR data; /* vector */
} SeqSummaryList, PNTR SeqSummaryListPtr;


NLM_EXTERN SeqSummaryListPtr LIBCALL SeqSummaryListNew PROTO((void));
NLM_EXTERN SeqSummaryListPtr LIBCALL SeqSummaryListFree PROTO(( SeqSummaryListPtr ufp));
NLM_EXTERN SeqSummaryListPtr LIBCALL SeqSummaryListAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean LIBCALL SeqSummaryListAsnWrite PROTO((SeqSummaryListPtr ufp, AsnIoPtr aip, AsnTypePtr atp));


/*****************************************************************************
*
*   Seq-entry-list
*
*****************************************************************************/
typedef struct {
	Int4 num;
	SeqEntryPtr PNTR data; /* vector */
	EntrezIdsPtr marked_missing; /* marked UIDs, optional */
} PNTR SeqEntryListPtr;
/* note that SeqEntryList is not provided because the name conflicted with */
/* the name of a function on sequtil.c; the typedef was not needed anyhow  */


NLM_EXTERN SeqEntryListPtr LIBCALL SeqEntryListNew PROTO((void));
NLM_EXTERN SeqEntryListPtr LIBCALL SeqEntryListFree PROTO(( SeqEntryListPtr ufp));
NLM_EXTERN SeqEntryListPtr LIBCALL SeqEntryListAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean LIBCALL SeqEntryListAsnWrite PROTO((SeqEntryListPtr ufp, AsnIoPtr aip, AsnTypePtr atp));



#ifdef Biostruc_supported

/*****************************************************************************
*
*   Biostruc-list
*
*****************************************************************************/
typedef struct {
	Int4 num;
	BiostrucPtr PNTR data; /* vector */
	EntrezIdsPtr marked_missing; /* marked UIDs, optional */
} BiostrucList, PNTR BiostrucListPtr;


NLM_EXTERN BiostrucListPtr LIBCALL BiostrucListNew PROTO((void));
NLM_EXTERN BiostrucListPtr LIBCALL BiostrucListFree PROTO(( BiostrucListPtr ufp));
NLM_EXTERN BiostrucListPtr LIBCALL BiostrucListAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean LIBCALL BiostrucListAsnWrite PROTO((BiostrucListPtr ufp, AsnIoPtr aip, AsnTypePtr atp));

#endif /* Biostruc_supported */

/**************************************************
*
*    EntrezNeighborText
*
**************************************************/


NLM_EXTERN EntrezNeighborTextPtr LIBCALL EntrezNeighborTextAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL EntrezNeighborTextAsnWrite PROTO (( EntrezNeighborTextPtr , AsnIoPtr, AsnTypePtr));


/**************************************************
*
*    ChildLink
*
*************************************************/
typedef struct struct_Child_Link {
   struct struct_Child_Link PNTR next;
   CharPtr   name;
   Boolean   isLeafNode;
   Int4   special;
   Int4   total;
} ChildLink, PNTR ChildLinkPtr;



NLM_EXTERN EntrezHierarchyChildPtr LIBCALL EntrezHierarchyChildFree PROTO ((EntrezHierarchyChildPtr ));
NLM_EXTERN EntrezHierarchyChildPtr LIBCALL EntrezHierarchyChildNew PROTO (( void ));
NLM_EXTERN EntrezHierarchyChildPtr LIBCALL EntrezHierarchyChildAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL EntrezHierarchyChildAsnWrite PROTO (( EntrezHierarchyChildPtr ,AsnIoPtr, AsnTypePtr));

NLM_EXTERN EntrezHierarchyPtr LIBCALL EntrezHierarchyFree PROTO ((EntrezHierarchyPtr ));
NLM_EXTERN EntrezHierarchyPtr LIBCALL EntrezHierarchyNew PROTO (( void ));
NLM_EXTERN EntrezHierarchyPtr LIBCALL EntrezHierarchyAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL EntrezHierarchyAsnWrite PROTO (( EntrezHierarchyPtr, AsnIoPtr, AsnTypePtr));


/**************************************************
*
*    EntrezBlastreq
*
**************************************************/
typedef struct struct_Entrez_blastreq {
   Uint4 OBbits__;
   BioseqPtr   bsp;
   DocType bsp_database;
   CharPtr   program;
   CharPtr   database;
   CharPtr   options;
   Uint1     showprogress;
} EntrezBlastreq, PNTR EntrezBlastreqPtr;


NLM_EXTERN EntrezBlastreqPtr LIBCALL EntrezBlastreqFree PROTO ((EntrezBlastreqPtr ));
NLM_EXTERN EntrezBlastreqPtr LIBCALL EntrezBlastreqNew PROTO (( void ));
NLM_EXTERN EntrezBlastreqPtr LIBCALL EntrezBlastreqAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL EntrezBlastreqAsnWrite PROTO (( EntrezBlastreqPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    EntrezExtraInfo
*
**************************************************/
typedef struct struct_Entrez_extra_info {
   Uint4 OBbits__;
   Int4   maxlinks;
   Uint1   canneighbortext;
   Uint1   expanded_medline;
   Uint1   canblast;
} EntrezExtraInfo, PNTR EntrezExtraInfoPtr;


NLM_EXTERN EntrezExtraInfoPtr LIBCALL EntrezExtraInfoFree PROTO ((EntrezExtraInfoPtr ));
NLM_EXTERN EntrezExtraInfoPtr LIBCALL EntrezExtraInfoNew PROTO (( void ));
NLM_EXTERN EntrezExtraInfoPtr LIBCALL EntrezExtraInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL EntrezExtraInfoAsnWrite PROTO (( EntrezExtraInfoPtr , AsnIoPtr, AsnTypePtr));

typedef struct struct_New_summary_List {
   Int4   num;
   DocType type;
   DocSumPtr PNTR data;
} NewSummaryList, PNTR NewSummaryListPtr;

NLM_EXTERN NewSummaryListPtr LIBCALL NewSummaryListFree PROTO ((NewSummaryListPtr ));
NLM_EXTERN NewSummaryListPtr LIBCALL NewSummaryListNew PROTO (( void ));
NLM_EXTERN NewSummaryListPtr LIBCALL NewSummaryListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL NewSummaryListAsnWrite PROTO (( NewSummaryListPtr , AsnIoPtr, AsnTypePtr));


/**************************************************
*
*    ClusterArticles
*
**************************************************/
typedef struct struct_Cluster_articles {
   Uint4 OBbits__;
   EntrezIdsPtr ids;
   Int4   fld;
   Int4   min_cluster;
   Int4   max_cluster;
   Int4   max_terms;
} ClusterArticles, PNTR ClusterArticlesPtr;


NLM_EXTERN ClusterArticlesPtr LIBCALL ClusterArticlesFree PROTO ((ClusterArticlesPtr ));
NLM_EXTERN ClusterArticlesPtr LIBCALL ClusterArticlesNew PROTO (( void ));
NLM_EXTERN ClusterArticlesPtr LIBCALL ClusterArticlesAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ClusterArticlesAsnWrite PROTO (( ClusterArticlesPtr , AsnIoPtr, AsnTypePtr));




/**************************************************
*
*    ClusterResp
*
**************************************************/
typedef struct struct_Cluster_resp {
   Uint4 OBbits__;
   Int4   count;
   ValNodePtr   terms;
   ValNodePtr   term_counts;
} ClusterResp, PNTR ClusterRespPtr;


NLM_EXTERN ClusterRespPtr LIBCALL ClusterRespFree PROTO ((ClusterRespPtr ));
NLM_EXTERN ClusterRespPtr LIBCALL ClusterRespNew PROTO (( void ));
NLM_EXTERN ClusterRespPtr LIBCALL ClusterRespAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ClusterRespAsnWrite PROTO (( ClusterRespPtr , AsnIoPtr, AsnTypePtr));
 
 

/**************************************************
*
*    DateConstraints
*
**************************************************/
typedef struct struct_Date_constraints {
   Int4   count;
   struct struct_Date_vector PNTR   date_vec;
} DateConstraints, PNTR DateConstraintsPtr;


NLM_EXTERN DateConstraintsPtr LIBCALL DateConstraintsFree PROTO ((DateConstraintsPtr ));
NLM_EXTERN DateConstraintsPtr LIBCALL DateConstraintsNew PROTO (( void ));
NLM_EXTERN DateConstraintsPtr LIBCALL DateConstraintsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL DateConstraintsAsnWrite PROTO (( DateConstraintsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    DateVector
*
**************************************************/
typedef struct struct_Date_vector {
   struct struct_Date_vector PNTR next;
   DatePtr   begin_date;
   DatePtr   end_date;
   CharPtr   field_abbr;
} DateVector, PNTR DateVectorPtr;


NLM_EXTERN DateVectorPtr LIBCALL DateVectorFree PROTO ((DateVectorPtr ));
NLM_EXTERN DateVectorPtr LIBCALL DateVectorNew PROTO (( void ));
NLM_EXTERN DateVectorPtr LIBCALL DateVectorAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL DateVectorAsnWrite PROTO (( DateVectorPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    GetByFid
*
**************************************************/
typedef struct struct_Get_by_fid {
   Int4   mmdbid;
   Int4   feature_id;
   Int4   feature_set_id;
} GetByFid, PNTR GetByFidPtr;


NLM_EXTERN GetByFidPtr LIBCALL GetByFidFree PROTO ((GetByFidPtr ));
NLM_EXTERN GetByFidPtr LIBCALL GetByFidNew PROTO (( void ));
NLM_EXTERN GetByFidPtr LIBCALL GetByFidAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL GetByFidAsnWrite PROTO (( GetByFidPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    GetFeatIds
*
**************************************************/
typedef struct struct_Get_feat_ids {
   Int4   mmdbid;
   Int4   feature_type;
   Int4   feature_set_id;
} GetFeatIds, PNTR GetFeatIdsPtr;


NLM_EXTERN GetFeatIdsPtr LIBCALL GetFeatIdsFree PROTO ((GetFeatIdsPtr ));
NLM_EXTERN GetFeatIdsPtr LIBCALL GetFeatIdsNew PROTO (( void ));
NLM_EXTERN GetFeatIdsPtr LIBCALL GetFeatIdsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL GetFeatIdsAsnWrite PROTO (( GetFeatIdsPtr , AsnIoPtr, AsnTypePtr));

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
