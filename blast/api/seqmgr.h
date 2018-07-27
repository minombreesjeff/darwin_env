/*  seqmgr.h
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
* File Name:  seqmgr.h
*
* Author:  James Ostell
*   
* Version Creation Date: 9/94
*
* $Revision: 6.41 $
*
* File Description:  Manager for Bioseqs and BioseqSets
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: seqmgr.h,v $
* Revision 6.41  2001/11/15 18:15:48  kans
* set bsp->omdp at creation, SeqMgrDeleteIndexesInRecord sets omdp->bulkIndexFree
*
* Revision 6.40  2001/10/02 21:46:51  kans
* added SeqEntryAsnOut (JO)
*
* Revision 6.39  2001/10/01 11:30:38  kans
* added LockFarComponentsEx, which allows far feature products and locations to be locked as well as far seg or delta components
*
* Revision 6.38  2001/09/12 18:26:08  kans
* added bspIndex to bspextra, made SeqMgrGetBioseqContext much more efficient for large sets
*
* Revision 6.37  2001/04/26 19:48:47  kans
* added SeqMgrGetFeatureByLabel, bspextra->featsByLabel
*
* Revision 6.36  2001/03/29 22:31:12  kans
* added fields for descriptor indexing
*
* Revision 6.35  2000/11/03 00:13:01  kans
* added from and to parameters to GenomePartToSegmentMap, needed because delta contig can refer to the same accession multiple times
*
* Revision 6.34  2000/09/28 20:57:08  kans
* added LockFarComponents and UnlockFarComponents
*
* Revision 6.33  2000/07/10 23:28:49  kans
* GenomePartToSegmentMap only takes two Bioseq parameters
*
* Revision 6.32  2000/07/10 15:17:08  kans
* added GenomePartToSegmentMap for acc->contig remapping
*
* Revision 6.31  2000/05/24 21:13:49  ostell
* added SeqMgrHoldIndexing() support
*
* Revision 6.30  2000/01/07 22:58:04  kans
* added dnaStop field to map sig_peptide, etc., onto DNA coordinates to choose proper flatfile segment for display
*
* Revision 6.29  2000/01/07 02:32:31  kans
* added SeqMgrGetSfpProductList for access to bspextra->prodlisthead
*
* Revision 6.28  2000/01/07 02:17:45  kans
* removed featsBySap, added prodlisthead, to support gather by get_feats_product
*
* Revision 6.27  2000/01/05 19:16:46  kans
* added featsBySap field to be used to speed up targeted feature gather
*
* Revision 6.26  1999/12/27 19:57:45  kans
* binary search needed precomputed index to handle overlapping gene (mRNA, CDS, etc.) features, so after binary search it can back up to the appropriate first feature to check in a linear scan
*
* Revision 6.25  1999/11/05 19:53:15  kans
* SeqMgrIndexAlignments called by SeqMgrIndexFeatures, but can now be called separately even if you do not index features
*
* Revision 6.24  1999/08/25 22:08:00  kans
* made MakeReversedSeqIdString public
*
* Revision 6.23  1999/07/21 23:53:56  kans
* added SeqMgrFindSeqAlignByID
*
* Revision 6.22  1999/04/01 20:44:14  kans
* Int2 lengths to Int4 to allow CountGapsInDeltaSeq with buffer > 32K
*
* Revision 6.21  1998/11/24 22:21:25  kans
* index mRNA and CDS by position, allow arbitrary sorted feature array index
*
* Revision 6.20  1998/10/22 23:41:51  kans
* feat context has bsp, partial flags, far location flag, GetDesired functions can work on entity entity if using itemID
*
* Revision 6.19  1998/10/22 16:05:56  kans
* removed labeltype parameter from SeqMgrIndexFeatures, changed index parameter/field to Uint2
*
* Revision 6.18  1998/09/22 16:55:53  kans
* added SeqMgrGetDesiredFeature and position index field
*
* Revision 6.17  1998/09/01 19:25:26  kans
* context parameter in get best protein, get cds/rna given product
*
* Revision 6.16  1998/08/21 20:19:01  kans
* added SeqMgrExploreSegments, indexing features on segmented bioseq
*
* Revision 6.15  1998/08/14 15:40:39  kans
* SeqMgrMapPartToSegmentedBioseq neede LIBCALL, speeded up function by adding map up on part if fetched
*
* Revision 6.14  1998/08/13 22:31:47  kans
* SeqMgrMapPartToSegmentedBioseq to speed up GetOffsetInBioseq, start of indexing segments, also index biosource by location for binary search (Wheelan)
*
* Revision 6.13  1998/08/12 21:10:38  kans
* added parts index to speed segmented bioseq mapping
*
* Revision 6.12  1998/07/23 00:40:41  kans
* added SeqMgrGetOverlappingPub, second gene and pub if spanning origin
*
* Revision 6.11  1998/07/06 15:30:21  kans
* scope on index explore, added SeqMgrExploreBioseqs
*
* Revision 6.10  1998/07/01 19:13:21  kans
* SMFeatBlock.data is allocated array of reasonable size
*
* Revision 6.9  1998/06/30 12:56:50  kans
* code fixes, public functions moved to explore.h
*
* Revision 6.8  1998/06/29 23:37:42  kans
* added context structure for all explores, index every bioseq in an entity
*
* Revision 6.7  1998/06/29 01:33:31  kans
* added SeqMgrGetNextDescriptor and SeqMgrGetNextFeature
*
* Revision 6.6  1998/06/29 00:23:57  kans
* several changes to new indexing functions
*
* Revision 6.5  1998/06/28 02:38:20  kans
* simplified filters, finished best gene, explore functions
*
* Revision 6.4  1998/06/27 22:23:47  kans
* improvements and further implementation of new indexing, exploration functions
*
* Revision 6.3  1998/06/26 22:36:28  kans
* initial work on tracking sorted features, and cds and prot links, for rapid collection
*
* Revision 6.2  1997/11/19 22:14:45  ostell
* added support for multithreaded programs
*
* Revision 6.1  1997/09/11 15:55:43  ostell
* Added support for SetColor messages
*
* Revision 6.0  1997/08/25 18:07:09  madden
* Revision changed to 6.0
*
* Revision 5.7  1997/07/28 13:29:45  ostell
* Moved GetUniGeneIDForSeqId() to seqmgr.c
*
* Revision 5.6  1997/06/19 18:38:47  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.5  1997/01/23 22:37:14  ostell
* minor change to seqmgr.h for new indexing
*
 * Revision 5.3  1997/01/08  22:49:22  tatiana
 * buf and buflen arguments added to CountGapsInDeltaSeq()
 *
 * Revision 5.2  1996/07/25  02:32:26  ostell
 * added CountGapsInDeltaSeq()
 *
 * Revision 5.1  1996/07/19  22:13:13  ostell
 * added SpreadGapsInDeltaSeq()
 *
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 4.3  1995/12/22  14:43:59  ostell
 * added reload code to BioseqLockById
 * break out relad from cache code to be used as part of gather locking
 * with BioseqReload
 *
 * Revision 4.2  1995/12/04  21:40:05  ostell
 * added GetSeqIdForGI() and GetGIForSeqId()
 *
 * Revision 4.1  1995/09/30  03:38:31  ostell
 * Changed ObjMgrMessage functions to pass a structure
 * Added support for selecting regions
 * Added ability to remove entity when no more views on it
 *
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 1.10  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#ifndef _NCBI_SeqMgr_
#define _NCBI_SeqMgr_

#ifndef _NCBI_ObjMgr_
#include <objmgr.h>		   /* the object manager interface */
#endif

#ifndef _NCBI_Seqset_
#include <objsset.h>		   /* the object loader interface */
#endif

#ifndef __NLM_THR__
#include <ncbithr.h>
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
*   Sequence Management Functions
*
*****************************************************************************/
	/** callbacks for data management **/

/*****************************************************************************
*
*   SeqMgr manipulates the "registry" of Bioseqs in memory
*       assigns "EntityID" to SeqEntrys loaded in memory..
*   		only top level SeqEntry gets an EntityID
*   		caching and locking also done on top level SeqEntry
*
*****************************************************************************/

#define BSF_TEMP 1      /* for BioseqFetch functions */

/* smp is really a SeqMgrPtr, but had to be Pointer to satisfy compiler */

typedef BioseqPtr (LIBCALLBACK * BSFetchTop)
		PROTO((SeqIdPtr sip, Uint1 ld_type));

typedef BioseqPtr (LIBCALLBACK * BSFetch) PROTO((SeqIdPtr sip, Pointer data));

typedef struct seqidindexelement {
	CharPtr str;               /* PRINTID_FASTA_SHORT string */
	ObjMgrDataPtr omdp;             /* the omdp containing the Bioseq */
} SeqIdIndexElement, PNTR SeqIdIndexElementPtr;

typedef struct seqidindexblock {
	SeqIdIndexElement sid[100];
	struct seqidindexblock PNTR next;
} SeqIdIndexBlock, PNTR SeqIdIndexBlockPtr;

typedef struct smscope {       /* for setting scope by thread */
	TNlmThread thr;            /* the thread the scope is valid for */
	SeqEntryPtr SEscope;       /* scope for that thread */
} SMScope, PNTR SMScopePtr;

typedef struct seqmng {        /* functions for sequence data management */
	SMScopePtr scope;
	Int2 total_scope,              /* sizeof scope */
		num_scope;                 /* current number */
	BSFetchTop bsfetch;            /* BioseqFetch into memory */
	Pointer TopData;               /* user data for BSFetchTop function */
	Boolean fetch_on_lock;         /* call fetch when locking? */
	Int4 NonIndexedBioseqCnt,      /* number of Bioseqs in NonIndexedBioseq */
		NonIndexedBioseqNum;       /* size of NonIndexedBioseq */
	BioseqPtr PNTR NonIndexedBioseq; /* Bioseqs waiting for SeqId indexing */
	Int4 BioseqIndexCnt,           /* number of elements used in BioseqIndex */
		BioseqIndexNum;            /* size of BioseqIndex */
	SeqIdIndexElementPtr PNTR BioseqIndex;  /* pointers to index elements */
	SeqIdIndexBlockPtr BioseqIndexData;    /* what BioseqIndex points to */
	Boolean is_write_locked;
	Int4 hold_indexing;      /* set by SeqMgrHoldIndexing */
} SeqMgr, PNTR SeqMgrPtr;

/**** All replaced in Object Manager ************/
/************************************************/
#define SM_BIOSEQ OBJ_BIOSEQ
#define SM_BIOSEQSET OBJ_BIOSEQSET

#define SeqMgrConnect(a,b,c,d) ObjMgrConnect(a,b,c,d)

/*****************************************************************************
*
*   SeqMgrAdd(type, data)
*   	adds a Bioseq or BioseqSet to the sequence manager
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrAdd PROTO((Uint2 type, Pointer data));
/*****************************************************************************
*
*   SeqMgrDelete(type, data)
*   	deletes a Bioseq or BioseqSet from the sequence manager
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrDelete PROTO((Uint2 type, Pointer data));

/*****************************************************************************
*
*   SeqMgrHoldIndexing(Boolean hold)
*       stops sequence indexing to allow bulk loading if hold = TRUE
*       starts it when hold = FALSE;
*       uses a counter so you must call it the same number of times
*        with TRUE as with FALSE
*       when the counter decrements to 0, it will index what it has.
*
*****************************************************************************/
NLM_EXTERN void LIBCALL SeqMgrHoldIndexing PROTO((Boolean hold));

/*****************************************************************************
*
*   SeqMgrAddToBioseqIndex(bsp)
*   	Indexes a BioseqPtr by SeqId(s)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrAddToBioseqIndex PROTO((BioseqPtr bsp));

/*****************************************************************************
*
*   SeqMgrDeleteDeleteFromBioseqIndex(bsp)
*   	Removes index on BioseqPtr SeqIds
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrDeleteFromBioseqIndex PROTO((BioseqPtr bsp));

/*****************************************************************************
*
*   SeqMgrReplaceInBioseqIndex(bsp)
*   	Replaces index on BioseqPtr SeqIds
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrReplaceInBioseqIndex PROTO((BioseqPtr bsp));

/*****************************************************************************
*
*   SeqMgrDeleteIndexesInRecord (sep)
*   	Bulk removal of SeqId index on entire entity prior to its deletion
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrDeleteIndexesInRecord (SeqEntryPtr sep);



NLM_EXTERN Boolean LIBCALL SeqMgrSeqEntry PROTO((Uint1 type, Pointer data, SeqEntryPtr sep));
NLM_EXTERN SeqEntryPtr LIBCALL SeqMgrGetSeqEntryForData PROTO((Pointer data));
NLM_EXTERN Int2 LIBCALL SeqMgrGetEntityIDForSeqEntry PROTO((SeqEntryPtr sep));
NLM_EXTERN SeqEntryPtr LIBCALL SeqMgrGetSeqEntryForEntityID PROTO((Int2 id));

/*****************************************************************************
*
*   SeqIdFetch functions
*     Convert between id types
*        Look first in memory
*        Then call registered OBJ_SEQID,OBJ_SEQID proceedures to satisfy
*        EntrezBioseqFetchEnable supports these for the Entrez Interface
*
*****************************************************************************/

/*****************************************************************************
*
*   GetSeqIdForGI(Int4)
*     returns the SeqId for a GI
*     returns NULL if can't find it
*     The returned SeqId is allocated. Caller must free it.
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr LIBCALL GetSeqIdForGI PROTO((Int4 gi));


/*****************************************************************************
*
*   GetGIForSeqId(SeqIdPtr)
*     returns the GI for a SeqId
*     returns 0 if can't find it
*
*****************************************************************************/
NLM_EXTERN Int4 LIBCALL GetGIForSeqId PROTO((SeqIdPtr sid));

/*****************************************************************************
*
*   MakeReversedSeqIdString(sid, buf, len)
*     Prints FASTA_SHORT style in upper case reverse order for fast binary searches
*
*****************************************************************************/
NLM_EXTERN Boolean MakeReversedSeqIdString (SeqIdPtr sid, CharPtr buf, size_t len);

/*****************************************************************************
*
*   SeqMgrLinkSeqEntry(sep, parenttype, parentptr)
*      connects all component seq-entries by traversing the linked list
*        all calling SeqMgrConnect and SeqMgrSeqEntry appropriately
*        if parenttype != 0, then assumes seqentry is contained in parentptr
*           and should be connected to it
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrLinkSeqEntry PROTO((SeqEntryPtr sep, Uint2 parenttype, Pointer parentptr));

/*****************************************************************************
*
*   SeqMgrFreeCache()
*   	frees all cached SeqEntrys
*   	returns FALSE if any errors occurred
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrFreeCache PROTO((void));

/********************************************************************************
*
*   BioseqReload (omdp, lockit)
*     reloads the cached SeqEntry at top of omdp
*     if (lockit) locks the record
*
*     returns NULL on failure
*     returns omdp of (possibly new) top level ObjMgrData containing the reloaded
*      data from the old omdp. Also returns NULL if omdp does not have a Bioseq
*      fetch function attached to it for reload.
*
*********************************************************************************/
NLM_EXTERN ObjMgrDataPtr LIBCALL BioseqReload PROTO((ObjMgrDataPtr omdp, Boolean lockit));

/*****************************************************************************
*
*   Selection Functions for data objects based on SeqLoc
*      See also general selection in objmgr.h
*
*****************************************************************************/

/*****************************************************************************
*
*   SeqMgrSelect(region)
*      region is a SeqLocPtr
*          It can only apply to one Bioseq
*          selected area will be extreme left and right ends
*          fuzziness is ignored
*      if something else selected, deselects it first, then selects requested
*        item
*      to select without deselecting something else, use SeqMgrAlsoSelect()
*      returns TRUE if item is now currently selected, FALSE if not
*      "region" is always copied. Caller is responsible for managment of
*         SeqLoc that is passed in.
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrSelect PROTO((SeqLocPtr region));
NLM_EXTERN Boolean LIBCALL SeqMgrAlsoSelect PROTO((SeqLocPtr region));

/*****************************************************************************
*
*   SeqMgrDeSelect(region)
*   	if this item was selected, then deselects and returns TRUE
*   	else returns FALSE
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrDeSelect PROTO((SeqLocPtr region));

/*****************************************************************************
*
*   SeqMgrSetColor(region, rgb)
*      region is a SeqLocPtr
*          It can only apply to one Bioseq
*          colored area will be extreme left and right ends
*          fuzziness is ignored
*      "region" is always copied. Caller is responsible for managment of
*         SeqLoc that is passed in.
*      rgb is a Uint1[3] array with RGB values
*         it is always copied so Caller is responsible for memory management
*         of passed in object
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrSetColor PROTO((SeqLocPtr region, Uint1Ptr rgb));

/************************************************/
/************************************************/


/*****************************************************************************
*
*   Return the current SeqMgr
*   	Initialize if not done already
*       This function will become obsolete
*
*****************************************************************************/
NLM_EXTERN SeqMgrPtr LIBCALL SeqMgrGet (void);

/*****************************************************************************
*
*   SeqMgrReadLock()
*   	Initialize if not done already
*       A thread can have only one read or write lock at a time
*       Many threads can have read locks
*       Only one thread can have a write lock
*       No other threads may have read locks if a write lock is granted
*       If another thread holds a write lock, this call blocks until write
*          is unlocked.
*
*****************************************************************************/
NLM_EXTERN SeqMgrPtr LIBCALL SeqMgrReadLock (void);

/*****************************************************************************
*
*   SeqMgrWriteLock
*   	Initialize if not done already
*       A thread can have only one read or write lock at a time
*       Many threads can have read locks
*       Only one thread can have a write lock
*       No other threads may have read locks if a write lock is granted
*       If another thread holds a read or write lock, this call blocks until write
*          is unlocked.
*
*****************************************************************************/
NLM_EXTERN SeqMgrPtr LIBCALL SeqMgrWriteLock (void);

/*****************************************************************************
*
*  SeqMgrUnlock()
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrUnlock (void);

/*****************************************************************************
*
*   SeqMgrSetBSFetchTop (fetch, data)
*   	sets the BSFetchTop routine to "fetch"
*       "data" pointer will be sent to function
*       returns previous value
*       set to NULL to turn off all fetching for that type
*
*       Current value can be called directly as BioseqFetch();
*   	Default is
*   		1) looks in memory
*   		2) looks locally if LocalBSFetch is set
*			3) looks remotely if RemoteBSFetch is set
*
*****************************************************************************/
NLM_EXTERN BSFetchTop LIBCALL SeqMgrSetBSFetchTop PROTO((BSFetchTop fetch, Pointer data));

/*****************************************************************************
*
*   SeqMgrSetFetchOnLock(value)
*   	if value = TRUE, manager will try to fetch the bioseq if not in
*          memory, when BioseqLock is called
*   	if FALSE, BioseqLock will only look in memory
*       returns previous value of fetch_on_lock
*       default is to fetch_on_lock
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrSetFetchOnLock PROTO((Boolean value));



NLM_EXTERN BioseqPtr LIBCALL BioseqLock PROTO((BioseqPtr bsp));
NLM_EXTERN Boolean LIBCALL BioseqUnlock PROTO((BioseqPtr bsp));

/*****************************************************************************
*
*   BioseqFind(sid)
*   	Finds a Bioseq in memory based on SeqId
*   	Will also restore a Bioseq that has been cached out by SeqMgr
*
*****************************************************************************/
NLM_EXTERN BioseqPtr LIBCALL BioseqFind PROTO((SeqIdPtr sip));

/*****************************************************************************
*
*   BioseqFindCore(sid)
*   	Finds a Bioseq in memory based on SeqId when only "core" elements needed
*   	Will NOT restore a Bioseq that has been cached out by SeqMgr
*       This function is for use ONLY by functions that only need the parts
*         of the Bioseq left when cached out. This includes the SeqId chain,
*         and non-pointer components of the Bioseq.
*
*****************************************************************************/
NLM_EXTERN BioseqPtr LIBCALL BioseqFindCore PROTO((SeqIdPtr sip));

/*****************************************************************************
*
*   BioseqFindEntity(sid, itemIDptr)
*   	Finds a Bioseq in memory based on SeqId
*   	Will NOT restore a Bioseq that has been cached out by SeqMgr
*       returns EntityID if found, otherwise 0
*       itemIDptr is set to the value for itemID in ObjMgr functions
*       itemtype is OBJ_BIOSEQ of course
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL BioseqFindEntity PROTO((SeqIdPtr sip, Uint2Ptr itemIDptr));


/*****************************************************************************
*
*   BioseqLockById(sid)
*   	Like BioseqFind, except will also try to fetch the bioseq from
*         outside storage if not in memory already. Will cache out data
*   	  loaded this way if memory gets too full
*         Calls BioseqFetch to do the fetch
*
*****************************************************************************/
NLM_EXTERN BioseqPtr LIBCALL BioseqLockById PROTO((SeqIdPtr sid));
NLM_EXTERN Boolean LIBCALL BioseqUnlockById PROTO((SeqIdPtr sid));

NLM_EXTERN BioseqPtr LIBCALL BioseqFetch PROTO((SeqIdPtr sid, Uint1 ld_type));

#define BSFETCH_TEMP 1	   /* load called by software.. temporary use */
#define BSFETCH_STD  0	   /* load called by user, must be freed by user */

/*****************************************************************************
*
*   SeqEntry Management Functions
*
*****************************************************************************/
/*****************************************************************************
*
*   SeqEntrySetScope(sep)
*   	scopes global seqentry searches to sep
*       setting sep=NULL, opens scope to all seqentries in memory
*       returns the current scope
*
*****************************************************************************/
NLM_EXTERN SeqEntryPtr LIBCALL SeqEntrySetScope PROTO((SeqEntryPtr sep));

/*****************************************************************************
*
*   SeqEntryGetScope(sep)
*       returns the current scope or NULL if none set
*
*****************************************************************************/
NLM_EXTERN SeqEntryPtr LIBCALL SeqEntryGetScope PROTO((void));

NLM_EXTERN SeqEntryPtr LIBCALL SeqEntryFind PROTO((SeqIdPtr sip));

/*****************************************************************************
*
*   Context routines for Bioseqs in Seq-entrys
*      Context is the chain of Seqentries leading to the bioseq.
*      context[count-1] is SeqEntry for bsp itself
*      If Bioseq not in a Seqentry, count is 0 and bcp->se may be used
*        if a fake Seqentry is convenient.
*
*****************************************************************************/
#define BIOSEQCONTEXTMAX 20

typedef struct bioseqcontxt {
	BioseqPtr bsp;           /* the Bioseq in question */
	Int2 count;              /* number of elements in context */
	Boolean hit;             /* used by BioseqContextNew and ..GetSeqFeat */
	SeqEntryPtr context[BIOSEQCONTEXTMAX];  /* array of SeqEntryPtr (last is count -1) */
	ValNode se;             /* used for a tempory SeqEntryPtr when only a Bioseq */
	SeqFeatPtr sfp;          /* current sfp */
	SeqAnnotPtr sap;         /* current sap */
	Int2 sftype,             /* SeqFeat type to look for */
		in;					 /* 0=location, 1=product, 2=either */
} BioseqContext, PNTR BioseqContextPtr;

NLM_EXTERN BioseqContextPtr LIBCALL BioseqContextNew PROTO((BioseqPtr bsp));
NLM_EXTERN BioseqContextPtr LIBCALL BioseqContextFree PROTO((BioseqContextPtr bcp));
/*****************************************************************************
*
*   BioseqContextGetSeqDescr(bcp, type, curr, SeqEntryPtr PNTR sep)
*       returns pointer to the next SeqDescr of this type
*       type gives type of Seq-descr
*       if (type == 0)
*          get them all
*       curr is NULL or previous node of this type found
*       moves up from bsp
*		if (sep != NULL) sep set to SeqEntryPtr containing the SeqDescr.
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL BioseqContextGetSeqDescr PROTO((BioseqContextPtr bcp, Int2 type, ValNodePtr curr, SeqEntryPtr PNTR the_sep));
NLM_EXTERN CharPtr LIBCALL BioseqContextGetTitle PROTO((BioseqContextPtr bcp));
/*****************************************************************************
*
*   BioseqContextGetSeqFeat(bcp, type, curr, sapp, in)
*       returns pointer to the next Seq-feat of this type
*       type gives type of Seq-descr
*       if (type == 0)
*          get them all
*       curr is NULL or previous node of this type found
*       moves up from bsp
*   	if (sapp != NULL) is filled with SeqAnnotPtr containing the SeqFeat
*   	in:
*   		0 = sfp->location only
*   		1 = sfp->product only
*   		2 = either of above
*
*****************************************************************************/
NLM_EXTERN SeqFeatPtr LIBCALL BioseqContextGetSeqFeat PROTO((BioseqContextPtr bcp, Int2 type, SeqFeatPtr curr, SeqAnnotPtr PNTR sapp, Int2 in));

/*** works like BioseqContextGetSeqFeat, but a SeqEntryPtr and (optionally)
     a Bioseq will do ****************************************************/

/*****************************************************************************
*
*   SeqEntryGetSeqFeat(sep, type, curr, sapp)
*       returns pointer to the next Seq-feat of this type
*       type gives type of SeqFeat
*       if (type == 0)
*          get them all
*       curr is NULL or previous node of this type found
*       moves up from bsp
*   	if (sapp != NULL) is filled with SeqAnnotPtr containing the SeqFeat
*       if (bsp != NULL) then for that Bioseq match on location by "in"
*   	in:
*   		0 = sfp->location only
*   		1 = sfp->product only
*   		2 = either of above
*
*****************************************************************************/
NLM_EXTERN SeqFeatPtr LIBCALL SeqEntryGetSeqFeat PROTO((SeqEntryPtr sep, Int2 type, SeqFeatPtr curr, SeqAnnotPtr PNTR sapp, Int2 in, BioseqPtr bsp));

/*****************************************************************************
*
*   SpreadGapsInDeltaSeq(BioseqPtr bsp)
*      bsp must be a delta seq
*      function counts deltas with known lengths ( = known_len)
*               counts deltas which are gaps of unknown length ( = unk_count)
*                  these can delta of length 0, delta with fuzz = lim (unk),
*                    or SEQLOC_NULL
*               converts all unknown gaps to delta with fuzz = lim(unk)
*               sets length of all unknown gaps to
*                  (bsp->length - known_len)/unk_count
*                  any reminder spread over first few gaps
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SpreadGapsInDeltaSeq PROTO((BioseqPtr bsp));

/*****************************************************************************
*
*   CountGapsInDeltaSeq(BioseqPtr bsp, &num_segs, &num_gaps, &known_residues, &num_gaps_faked, CharPtr buf, Int2 buflen)
*      bsp must be a delta seq
*      function counts deltas and returns a profile
*          num_segs = total number of segments
*          num_gaps = total number of segments representing gaps
*          known_residues = number of real residues in the sequence (not gaps)
*          num_gaps_faked = number of gaps where real length is not known, but where
*                           a length was guessed by spreading the total gap length
*                           out over all gaps evenly.
*
*      NOTE: any of these pointers except bsp can be NULL
*
*      returns TRUE if values in argument were set.
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL CountGapsInDeltaSeq PROTO((BioseqPtr bsp, Int4Ptr num_segs, Int4Ptr num_gaps, Int4Ptr known_residues, Int4Ptr num_gaps_faked, CharPtr buf, Int4 buflen));

/*****************************************************************************
*
*   GetUniGeneIDForSeqId(SeqIdPtr)
*     returns the UniGene ID for a SeqId
*     returns 0 if can't find it, or not a legal unigene id
*     This only applies to genomes division of entrez
*     These serve as temporary placeholders until NCBI establishes
*       a stable long-term ID system for these sequence clusters
*
*     The clusters begin with 1,000,000 and are grouped by organism
*
*****************************************************************************/

NLM_EXTERN Int4 LIBCALL GetUniGeneIDForSeqId PROTO((SeqIdPtr sip));


/*****************************************************************************
*
*   BioseqExtra extensions to object manager to preindex features for rapid retrieval
*
*   Public functions moved to explore.h
*
*****************************************************************************/

/* the following structures are not frequently used directly by applications */

typedef struct smfeatitem {
  SeqFeatPtr   sfp;      /* freed when TL_CACHED, later will implement reassignment when reloaded */
  SeqAnnotPtr  sap;      /* SeqAnnot containing SeqFeat, same reap/reload criteria as above */
  BioseqPtr    bsp;      /* Bioseq on which this feature is indexed */
  CharPtr      label;    /* featdef content label */
  Int4         left;     /* extreme left on bioseq (first copy spanning origin is < 1) */
  Int4         right;    /* extreme right on bioseq (second copy spanning origin is > length) */
  Int4Ptr      ivals;    /* array of start/stop pairs */
  Int2         numivals; /* number of start/stop pairs in ivals array */
  Int4         dnaStop;  /* last stop on protein mapped to DNA coordinate for flatfile */
  Boolean      partialL; /* left end is partial */
  Boolean      partialR; /* right end is partial */
  Boolean      farloc;   /* location has an accession not packaged in entity */
  Uint1        strand;   /* strand (mapped to segmented bioseq if segmented) */
  Uint1        subtype;  /* featdef subtype */
  Uint2        itemID;   /* storing itemID so no need to gather again */
  Boolean      ignore;   /* ignore this second copy of a feature spanning the origin */
  Uint2        index;    /* position index needed for SeqMgrGetDesiredFeature */
  Int4         overlap;  /* for xxxByPos, index of leftmost candidate that overlaps this */
} SMFeatItem, PNTR SMFeatItemPtr;

typedef struct smfeatblock {
  struct smfeatblock PNTR  next;   /* pointer to next block of chunks */
  Uint2                    index;  /* latest offset within this block */
  SMFeatItemPtr            data;   /* allocated block for this chunk */
} SMFeatBlock, PNTR SMFeatBlockPtr;

typedef struct segpartsmap {
  struct segpartsmap PNTR  next;           /* pointer to next block of chunks */
  SeqLocPtr                slp;            /* allocated copy of seqLoc for part */
  CharPtr                  seqIdOfPart;    /* reverse upper case string of seqID of part */
  BioseqPtr                parentBioseq;   /* bioseq pointer for segmented parent */
  Int4                     cumOffset;      /* offset of part in segmented bioseq */
  Int4                     from;
  Int4                     to;
  Uint1                    strand;
  Uint2                    itemID;         /* OBJ_BIOSEQ_SEG itemID */
} SMSeqIdx, PNTR SMSeqIdxPtr;

typedef struct smdescitem {
  SeqDescrPtr  sdp;         /* freed when TL_CACHED, later will implement reassignment when reloaded */
  SeqEntryPtr  sep;         /* SeqEntry containing SeqDescr, same reap/reload criteria as above */
  Uint2        itemID;      /* storing itemID so no need to gather again */
  Uint2        index;       /* position index needed for SeqMgrGetDesiredDescriptor */
  Uint2        level;       /* packaging level - 0 is on Bioseq itself */
  Uint1        seqdesctype; /* seqdesc subtype */
} SMDescItem, PNTR SMDescItemPtr;

typedef struct bioseqextra {
  BioseqPtr           bsp;
  ObjMgrDataPtr       omdp;
  SeqFeatPtr          protFeat;       /* protein feature on whole protein bioseq gives name */
  SeqFeatPtr          cdsOrRnaFeat;   /* cds or rna whose product points to this bioseq */
  ValNodePtr          prodlisthead;   /* all features whose product points to this bioseq */

  SMFeatBlockPtr      featlisthead;   /* linked list of SMFeatItem chunks, arrays point to elements */
  SMFeatBlockPtr      featlisttail;   /* current block in linked list of SMFeatItem chunks */

  ValNodePtr          desclisthead;   /* linked list of ValNodes pointing to SMDescItem structures */

  SMDescItemPtr PNTR  descrsByID;     /* array of all descriptors on bioseq in original itemID order */
  SMDescItemPtr PNTR  descrsBySdp;    /* array of all features on bioseq sorted by SeqDescrPtr */
  SMDescItemPtr PNTR  descrsByIndex;  /* array of all features on bioseq sorted by order of presentation */

  SeqAlignPtr PNTR    alignsByID;     /* array of all alignments (on entity) in original itemID order */

  SMFeatItemPtr PNTR  featsByID;      /* array of all features on bioseq in original itemID order */
  SMFeatItemPtr PNTR  featsBySfp;     /* array of all features on bioseq sorted by SeqFeatPtr */
  SMFeatItemPtr PNTR  featsByPos;     /* array of all features on bioseq sorted by location */
  SMFeatItemPtr PNTR  featsByLabel;   /* array of all features on bioseq sorted by label */

  SMFeatItemPtr PNTR  genesByPos;     /* subset of featsByPos array containing only gene features */
  SMFeatItemPtr PNTR  mRNAsByPos;     /* subset of featsByPos array containing only mRNA features */
  SMFeatItemPtr PNTR  CDSsByPos;      /* subset of featsByPos array containing only CDS features */
  SMFeatItemPtr PNTR  pubsByPos;      /* subset of featsByPos array containing only publication features */
  SMFeatItemPtr PNTR  orgsByPos;      /* subset of featsByPos array containing only biosource features */

  BioseqPtr           parentBioseq;   /* segmented parent of this raw part all packaged together */
  SMSeqIdxPtr         segparthead;    /* linked list to speed mapping from parts to segmented bioseq */

  SMSeqIdxPtr PNTR    partsByLoc;     /* array of parts on segmented bioseq sorted by location */
  SMSeqIdxPtr PNTR    partsBySeqId;   /* array of parts on segmented bioseq sorted by reverse uppercase seqID */

  Int4                numdescs;       /* number of elements in descrsByID, descrsBySdp, and descrsByIndex arrays */
  Int4                numaligns;      /* number of elements in alignsByID array */
  Int4                numfeats;       /* number of elements in featsByID, featsBySfp and featsByPos arrays */
  Int4                numgenes;       /* number of elements in genesByPos array */
  Int4                nummRNAs;       /* number of elements in mRNAsByPos array */
  Int4                numCDSs;        /* number of elements in CDSsByPos array */
  Int4                numpubs;        /* number of elements in pubsByPos array */
  Int4                numorgs;        /* number of elements in orgsByPos array */

  Int4                numsegs;        /* number of segments in partslist array */

  Int4                min;            /* used for finding best protein feature */
  Uint2               bspItemID;      /* for bioseq explore functions */
  Uint2               bspIndex;       /* for bioseq explore functions */
  Int2                blocksize;      /* size of SMFeatBlock.data array to avoid wasting space */
                                      /* additional fields to map between genome record and parts,
                                         genomic DNA and mRNA, and mRNA and protein */
} BioseqExtra, PNTR BioseqExtraPtr;

/* the following functions are not frequently called by applications */

/*****************************************************************************
*
*   Bioseq extra functions for reapextra, reloadextra, and freeextra take an
*     ObjMgrDataPtr as a parameter, and are only called by the object manager,
*     not the application program
*
*****************************************************************************/

NLM_EXTERN Pointer LIBCALLBACK SeqMgrReapBioseqExtraFunc PROTO((Pointer data));
NLM_EXTERN Pointer LIBCALLBACK SeqMgrReloadBioseqExtraFunc PROTO((Pointer data));
NLM_EXTERN Pointer LIBCALLBACK SeqMgrFreeBioseqExtraFunc PROTO((Pointer data));

/*****************************************************************************
*
*   SeqMgrFindSMFeatItemPtr and SeqMgrFindSMFeatItemByID return SMFeatItemPtr
*     to access internal fields, passing entityID and not bsp uses list attached
*     to top of entity containing index to all feature itemIDs regardless of
*     what bioseq they are indexed on
*   SeqMgrGetDesiredFeature in explore.h is the preferred public function
*   SeqMgrGetSfpProductList returns linked list of features whose sfp->product
*     points to the given bioseq
*
*****************************************************************************/

NLM_EXTERN SMFeatItemPtr LIBCALL SeqMgrFindSMFeatItemPtr PROTO((SeqFeatPtr sfp));
NLM_EXTERN SMFeatItemPtr LIBCALL SeqMgrFindSMFeatItemByID PROTO((Uint2 entityID, BioseqPtr bsp, Uint2 itemID));
NLM_EXTERN ValNodePtr LIBCALL SeqMgrGetSfpProductList (BioseqPtr bsp);

/*****************************************************************************
*
*   SeqMgrMapPartToSegmentedBioseq can speed up sequtil's CheckPointInBioseq
*     for indexed part bioseq to segmented bioseq mapping
*
*****************************************************************************/

NLM_EXTERN Int4 LIBCALL SeqMgrMapPartToSegmentedBioseq PROTO((BioseqPtr in, Int4 pos, BioseqPtr bsp, SeqIdPtr sip));

/*****************************************************************************
*
*   GenomePartToSegmentMap used for mapping of part positions not used on a given contig
*
*****************************************************************************/

NLM_EXTERN SMSeqIdxPtr GenomePartToSegmentMap (BioseqPtr in, BioseqPtr bsp, Int4 from, Int4 to);

/*****************************************************************************
*
*   SeqMgrIndexAlignments called by SeqMgrIndexFeatures, can be called separately
*
*****************************************************************************/

NLM_EXTERN void LIBCALL SeqMgrIndexAlignments (Uint2 entityID);

/*****************************************************************************
*
*   SeqMgrFindSeqAlignByID uses new index to speed lookup of SeqAlignPtr
*
*****************************************************************************/

NLM_EXTERN SeqAlignPtr LIBCALL SeqMgrFindSeqAlignByID PROTO((Uint2 entityID, Uint2 itemID));

/*****************************************************************************
*
*   LockFarComponents takes a top SeqEntryPtr and locks the far Bioseq components of
*     any segmented or delta sequences.  It turns a ValNode list of locked BioseqPtrs.
*   LockFarComponentsEx takes a top SeqEntryPtr and locks far Bioseqs that are either
*     components of any segmented or delta sequences, pointed to by feature locations,
*     or pointed to by feature products.  It turns a ValNode list of locked BioseqPtrs.
*   UnlockFarComponents takes the ValNode list of locked BioseqPtrs, unlocks each
*     Bioseq, frees the ValNode list, and returns NULL.
*
*****************************************************************************/

NLM_EXTERN ValNodePtr LockFarComponents (SeqEntryPtr sep);

NLM_EXTERN ValNodePtr LockFarComponentsEx (SeqEntryPtr sep, Boolean components, Boolean locations, Boolean products);

NLM_EXTERN ValNodePtr UnlockFarComponents (ValNodePtr bsplist);

/*****************************************************************************
*
*   SeqEntryAsnOut (SeqEntryPtr sep, SeqIdPtr sip, Int2 retcode, AsnIoPtr aipout)
*
*      Takes top level SeqEntryPtr (sep) in memory, finds the Bioseq for sip,
*         then writes the relevant part of the SeqEntry into aipout depending
*         on retcode.. The SeqEntry in memory is unchanged.
*
*      retcode sets maximum complexity to return by values:
*         0 = return the whole blob
*         1 = return just the Bioseq and relevant descriptors and features
*         2 = return containing Seg-set if any
*         3 = return containing Nuc-prot set if any
*         4 = return containing Pub-set if any (this no longer used really)
*
******************************************************************************/
NLM_EXTERN Boolean SeqEntryAsnOut (SeqEntryPtr sep, SeqIdPtr sip,
                                    Int2 retcode, AsnIoPtr aipout);

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

