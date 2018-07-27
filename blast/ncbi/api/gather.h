/*   gather.h
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  gather.h
*
* Author:  Jim Ostell
*
* Version Creation Date:   10/7/94
*
* $Revision: 6.12 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: gather.h,v $
* Revision 6.12  2002/05/08 18:58:09  kans
* itemID is Uint4
*
* Revision 6.11  2001/11/15 18:34:52  kans
* added GetNextDescriptorUnindexed, requires AssignIDsInEntity be called first
*
* Revision 6.10  2000/01/06 00:54:50  kans
* added useSeqMgrIndexes field to GatherScope and InternalGCC structures, will implement indexed feature table exploration tomorrow
*
* Revision 6.9  1999/10/29 18:06:27  kans
* added GetPointerForIDs (with SW)
*
* Revision 6.8  1999/09/29 18:24:54  kans
* added DeleteMarkedObjects
*
* Revision 6.7  1999/09/28 18:10:16  kans
* added DeleteMarkedObjectsProc callback - not yet tested
*
* Revision 6.6  1999/09/28 12:10:25  kans
* finished implementing lightweight GatherObjectsInEntity
*
* Revision 6.5  1999/09/26 20:44:26  kans
* implemented most of VisitProc callbacks
*
* Revision 6.4  1999/09/26 00:17:14  kans
* VisitObjectsInEntity prototype added
*
* Revision 6.3  1999/09/07 17:59:53  kans
* AssignIDsInEntity takes datatype and dataptr for when entityID is 0, allowing unlinked components to be updated
*
* Revision 6.2  1999/09/07 17:00:43  kans
* added AssignIDsInEntity
*
* Revision 6.1  1999/01/13 23:34:20  kans
* added GatherSpecificProcLaunch
*
* Revision 6.0  1997/08/25 18:05:51  madden
* Revision changed to 6.0
*
* Revision 5.5  1997/06/19 18:37:53  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.4  1997/03/24 20:45:52  shavirin
* Added protection for usage with C++ compiler
*
 * Revision 5.3  1996/11/05  17:58:57  zjing
 * add a new member last to aligndata
 *
 * Revision 5.2  1996/08/29  01:18:34  ostell
 * added GatherAddExtraLoc for codebreak and trna.atncodon mapping
 *
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 4.11  1996/02/28  04:53:06  ostell
 * added ObjMgrHold suport
 *
 * Revision 4.9  1996/01/03  23:01:04  ostell
 * added GatherOverWrite() to support find/replace external to Gather
 *
 * Revision 4.8  1995/12/22  14:42:30  ostell
 * added do_not_reload_from_cache to GatherScope
 * modified calls to support it
 * changed default behavior of gather to load and reclock entities
 *
 * Revision 4.7  1995/12/20  19:19:39  ostell
 * added GatherContext.igccp field
 * added FocusSeqEntry() function
 *
 * Revision 4.6  1995/11/21  23:08:38  ostell
 * added support in GatherContext for gatherstack
 *
 * Revision 4.5  1995/11/06  22:18:38  kans
 * added prototypes for ReMapIntFuzz and SeqLocReMap
 *
 * Revision 4.4  1995/11/06  21:29:03  ostell
 * added newid and convert_loc to GatherScope, and new_loc to GatherContext
 * added functions ReMapIntFuzz and SeqLocReMap to support them
 * This allows SeqLocs on features to be copied into a remapped form by gather
 *
 * Revision 4.3  1995/10/06  19:25:24  ostell
 * added fields "ignore_top" and "stop_on_annot" to GatherScope
 * if "ignore_top" is TRUE, features on seglevel 0 are ignored
 * if "stop_on_annot" is TRUE, segments are traversed to a maximum depth
 * of gsp->seglevel, but traversing is stopped as soon as an annotation is
 * found.
 *
 * Revision 4.2  1995/09/30  03:38:31  ostell
 * Changed ObjMgrMessage functions to pass a structure
 * Added support for selecting regions
 * Added ability to remove entity when no more views on it
 *
 * Revision 4.1  1995/08/16  17:48:34  kans
 * add a chain parameter for gather Seq-align (jz)
 *
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 1.15  1995/07/10  15:51:59  kans
 * changes in gather_align_data (zjing)
 *
 * Revision 1.14  1995/06/01  21:53:55  kans
 * support for Seq-align (zjing)
 *
 * Revision 1.13  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
*
* ==========================================================================
*/

#ifndef _GATHER_
#define _GATHER_

#include <sequtil.h>


typedef struct gatherrange {
  Int4     left;
  Int4     right;
  Boolean  l_trunc;
  Boolean  r_trunc;
  Uint1    strand;
} GatherRange, PNTR GatherRangePtr;

typedef struct seqrange {
  Int4		start;
  Int4		stop;
  Uint1		strand;
 } SeqRange, PNTR SeqRangePtr;
 
typedef struct alignrange{
  Uint1    segtype;  /* codes for segments of alignments, _SEG below */
  GatherRange gr;
  SeqRange sr;
  struct alignrange PNTR next;
 } AlignRange, PNTR AlignRangePtr;
  
 
 typedef struct aligndata {
	Uint2 chain;		/*the current count of the dense-diag or std-seg chain*/
	Int4 master_pos;	/*the start position of the master sequence*/
	Int2 order;		/*order of sequence in ONE chain*/
 	SeqIdPtr sip;
 	GatherRange extremes;	/*two ends on the graph*/
 	SeqRange	seqends;	/*two ends on the sequence*/
 	AlignRangePtr arp, last;
  	struct aligndata PNTR next;
 }AlignData, PNTR AlignDataPtr;
 	

#define GAP_SEG 1	    /* a gap */
#define REG_SEG 2		 /* a segment */
#define INS_SEG 3		 /* an insertion */
#define STD_SEG	4		/*a segment from StdSeg*/
#define DIAG_SEG 5		/*a diagnol segment*/

/*****************************************************************************
*
*   GatherSeqEntry() and GatherItem() return a pointer to the GatherContext
*   below. entityID, itemID, and thistype are the 3 minimum elements required
*   for GatherItem to relocate the item.
*
*   The fields seglevel and propagated will NOT be set by GatherItem(). They
*   are ONLY set by GatherSeqEntry().
*
*****************************************************************************/
typedef struct gatherelement {
	Uint2 itemID, itemtype;
	Pointer thisitem;
	Boolean tempload;
} GatherElement, PNTR GatherElementPtr;

typedef struct gathercontext {
	Uint2  entityID ,
		  itemID;
	Pointer thisitem,
		  previtem,
		  parentitem;
	Uint2 thistype,
		  prevtype,
		  parenttype;
	Pointer PNTR prevlink;    /* used to remove data item from chains */
	SeqEntryPtr sep;          /* sep that points to Bioseq or BioseqSet in thisitem */
	Pointer userdata;
	Boolean tempload;		  /* this data temporarily loaded */
	Boolean hold;             /* ObjMgr is in a hold state (see ObjMgrSetHold, objmgr.h) */
	GatherRangePtr rdp;		  /* mapped intervals to global coordinates */
	AlignDataPtr adp;			/*for aligned data*/
	Int2 num_interval;		  /* number of mapped intervals */
	GatherRange extremes;       /* extreme left and right locations */
	Boolean propagated;       /* is this item propagated from out of scope? */
	Int2 seglevel;            /* how many segment levels are we? */
	Boolean product;          /* TRUE if match by SeqId on product */
	Int2 indent;              /* indent level in containing Seq-entry */
	SeqLocPtr new_loc;        /* SeqLoc converted to new coordinates if
									GatherScope.convert_loc == TRUE.
                                    WARNING:  USER MUST FREE new_loc */
	SeqLocPtr PNTR extra_loc;	/* converted SeqLocs for CodeBreak or tRNA.anticodon
								    filled in if GatherScope.convert_loc = TRUE
									WARNING:  USER MUST FREE extra_loc[i] */
	Int2 extra_loc_cnt,         /* number of cells in extra_loc containing converted
		                            locs */
		extra_loc_total;        /* size of extra_loc array */
	GatherElementPtr gatherstack;  /* thisitem is gcp->indent, parent gcp->indent - 1 */
	Int2 numstack;                 /* gatherstack total size */
	Pointer igccp;           /* really InternalGCCPtr. use with care */
} GatherContext, PNTR GatherContextPtr;

typedef Boolean (*GatherItemProc) (GatherContextPtr gcp);

typedef struct gatherscope {
	SeqEntryPtr scope;        /* NULL, or SeqEntry to limit data items to */
	Boolean get_feats_location,   /* get all features if location on Bioseq */
		get_feats_product;    /* get all features if product on Bioseq */
	SeqLocPtr target;            /* limit features to a range on the Bioseq */
	Int4 offset;              /* offset features this much */
	Boolean ignore[OBJ_MAX];	/* if TRUE, this GITEM does not call userfunc */
	Boolean nointervals;        /* if TRUE, don't populate GatherIntervals */
	Boolean mapinsert;		/*if TRUE, map the gaps in master as insertion*/
	Int2 seglevels,             /* number of segment levels to traverse */
		currlevel;              /* current segment depth */
	Boolean stop_on_annot;      /* if TRUE, stop traversing down when Seq-annot found */
	Boolean ignore_top;         /* if TRUE, ignore Seq-annot on top level segment */
	Boolean split_packed_pnt;   /* if TRUE, give itemID to each point */
	Boolean convert_loc;        /* if TRUE, create a copy of Seq-loc for features and
	                               put in GatherContext.new_loc. Only works if "target"
								   is not NULL and get_feats... is TRUE */
	SeqIdPtr newid;             /* SeqId to use if convert_loc is TRUE */
	Boolean do_not_reload_from_cache;  /* if TRUE, gather does not automatically reload cached entity */
	Boolean useSeqMgrIndexes;   /* explore targeted feature tables with feature indexing functions */
} GatherScope, PNTR GatherScopePtr;

typedef struct internalgcc {
	GatherContext gc;
	GatherScope scope;
	GatherItemProc userfunc;
	Boolean rev;                /* TRUE if scope->target is on complement */
	Int2 max_interval;
	BioseqPtr bsp;              /* for checking a range on a Bioseq */
	Int2 itemIDs[OBJ_MAX];
	SeqLocPtr segloc;           /* if target is segmented and seglevel > 0 */
	Int2 first_seg, last_seg;   /* see SeqLocCopyPart() in edutil.h */
	Int2 segcnt;                /* count of segments in segloc */
	Int4Ptr seglens;            /* length of each segment in seqloc */
	BoolPtr found_annot;     /* TRUE if annot found in this region of current Bioseq */
	                            /*    Not an indication of annot found in this segment */
	Uint1 locatetype;           /* itemtype to locate */
	Int2 locateID;              /* itemID to locate */
	Pointer locatePtr;          /* data item to locate */
	ObjMgrDataPtr omdp;         /* top level omdp in entity */
	Boolean reloaded;           /* TRUE if entity was reloaded from cache by IGCCBuild() */
	Boolean useSeqMgrIndexes;   /* explore targeted feature tables with feature indexing functions */
} InternalGCC, PNTR InternalGCCPtr;


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

NLM_EXTERN Boolean SeqLocOffset PROTO((SeqLocPtr seq_loc, SeqLocPtr sfp_loc, GatherRangePtr range, Int4 offset));
NLM_EXTERN Boolean get_align_ends PROTO((SeqAlignPtr align, SeqIdPtr id, Int4Ptr start, Int4Ptr stop, Uint1Ptr strand));

NLM_EXTERN AlignDataPtr FreeAlignData PROTO((AlignDataPtr adp));
NLM_EXTERN AlignDataPtr gather_align_data PROTO((SeqLocPtr m_slp, SeqAlignPtr align, Int4 offset, Boolean ck_interval, Boolean map));

NLM_EXTERN IntFuzzPtr ReMapIntFuzz PROTO((IntFuzzPtr ifp, Boolean rev, SeqLocPtr seq_loc, SeqLocPtr sfp_loc, Int4 offset));
NLM_EXTERN SeqLocPtr SeqLocReMap PROTO((SeqIdPtr newid, SeqLocPtr seq_loc, SeqLocPtr head, Int4 offset, Boolean rev));

/*****************************************************************************
*
*   GatherEntity (entityID, userdata, userproc, scope)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL GatherEntity PROTO((Uint2 entityID, Pointer userdata, GatherItemProc userfunc, GatherScopePtr scope));

/*****************************************************************************
*
*   GatherSeqEntry (sep, userdata, userproc, scope)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL GatherSeqEntry PROTO((SeqEntryPtr sep, Pointer userdata, GatherItemProc userfunc, GatherScopePtr scope));

/*****************************************************************************
*
*   FocusSeqEntry (sep, scope)
*      zeros out all fields in scope
*      sets scope.target, .scope,  appropriately for 
*         SeqEntry
*      if (Bioseq)
*         target is the bioseq
*         entityID is the containing set if any
*         scope.scope is null
*      if (BioseqSet)
*         target is NULL
*         entityID is the containing set if any
*         scope.scope is sep
*
*      return of FOCUS_INITIALIZED means scope is initialized
*      return of FOCUS_NOT_NEEDED means send NULL for GatherScope.. you
*        don't need to scope for this SeqEntryPtr
*      return of FOCUS_ERROR  means it could not be initialized
*
*      NOTE: If this function returns FOCUS_INITIALIZED, the CALLER MUST
*        call SeqLocFree(scope.target) to remove any target this function
*        had to install. WARNING!
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL FocusSeqEntry PROTO((SeqEntryPtr sep, GatherScopePtr scope));

#define FOCUS_ERROR 0
#define FOCUS_NOT_NEEDED 1
#define FOCUS_INITIALIZED 2

/*****************************************************************************
*
*   GatherItem (entityID, itemID, itemtype, userdata, userproc)
*      Get an item by entityID, itemID, itemtype
*      GatherContext.seglevel and GatherContext.propagated will not be
*        set on the callback.
*      itemtype is as defined in objmgr.h for OBJ_
*        only those included in GITEM_ defined above will be found
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL GatherItem PROTO((Uint2 entityID, Uint2 itemID, Uint2 itemtype,
                                   Pointer userdata, GatherItemProc userfunc));

/*****************************************************************************
*
*   GatherData (entityID, itemID, itemtype, userdata, userproc)
*      Get an item by entityID, itemtype, and a Pointer of itemtype
*      GatherContext.seglevel and GatherContext.propagated will not be
*        set on the callback.
*   
*      Sets in_scope to FALSE so that the callback is not called
*         Scope is NULL, so in_scope is never TRUE
*      Sets ignore TRUE for everything not needed to find item
*      Sets locatetype and locateID, which are checked in the traversal
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL GatherData PROTO((Uint2 entityID, Pointer dataptr, Uint2 itemtype,
                                   Pointer userdata, GatherItemProc userfunc));

/*****************************************************************************
*
*   GatherItemIDByData (entityID, itemtype, dataptr)
*      Looks in entityID for an element of itemtype that matches the pointer
*         dataptr.
*      if found, returns the itemID
*      else returns 0
*      itemtype is as defined in objmgr.h for OBJ_
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL GatherItemIDByData PROTO((Uint2 entityID, Uint2 itemtype, Pointer dataptr));


/****************************************************************************
*
*   GatherDataForProc(ompcp, sel)
*
*       fills in data, choice, and choictype in OMProcControlPtr
*         sets ompcp->whole_entity TRUE if appropriate
*       returns TRUE if it did it
*       if (sel == TRUE), fills in ompcp with data from ObjMgrGetSelect first.
*          returns FALSE if nothing selected.. Does ErrPostEx() for it
*
****************************************************************************/
NLM_EXTERN Boolean LIBCALL GatherDataForProc PROTO((OMProcControlPtr ompcp, Boolean sel));

/****************************************************************************
*
*   DetachDataForProc(ompcp, sel)
*
*       fills in data, choice, and choictype in OMProcControlPtr
*         sets ompcp->whole_entity TRUE if appropriate
*       returns TRUE if it did it
*       if (sel == TRUE), fills in ompcp with data from ObjMgrGetSelect first.
*          returns FALSE if nothing selected.. Does ErrPostEx() for it
*       Detaches data item from surrounding data if necessary
*
****************************************************************************/
NLM_EXTERN Boolean LIBCALL DetachDataForProc PROTO((OMProcControlPtr ompcp, Boolean sel));

/****************************************************************************
*
*   AttachDataForProc(ompcp, sel)
*
*       fills in data, choice, and choictype in OMProcControlPtr
*         sets ompcp->whole_entity TRUE if appropriate
*       returns TRUE if it did it
*       if (sel == TRUE), fills in ompcp with data from ObjMgrGetSelect first.
*          returns FALSE if nothing selected.. Does ErrPostEx() for it
*       Attaches data in output section of ompcp into the input section
*
****************************************************************************/
NLM_EXTERN Boolean LIBCALL AttachDataForProc PROTO((OMProcControlPtr ompcp, Boolean sel));

/****************************************************************************
*
*   CopyDataForProc(ompcp, sel)
*
*       fills in data, choice, and choictype in OMProcControlPtr
*         sets ompcp->whole_entity TRUE if appropriate
*       returns TRUE if it did it
*       if (sel == TRUE), fills in ompcp with data from ObjMgrGetSelect first.
*          returns FALSE if nothing selected.. Does ErrPostEx() for it
*       Attaches copy of data in output section of ompcp
*       adds to objmgr as necessary, but does NOT register copy
*
****************************************************************************/
NLM_EXTERN Boolean LIBCALL CopyDataForProc PROTO((OMProcControlPtr ompcp, Boolean sel));

/****************************************************************************
*
*   ReplaceDataForProc(ompcp, sel)
*
*       fills in data, choice, and choictype in OMProcControlPtr
*         sets ompcp->whole_entity TRUE if appropriate
*       returns TRUE if it did it
*       if (sel == TRUE), fills in ompcp with data from ObjMgrGetSelect first.
*          returns FALSE if nothing selected.. Does ErrPostEx() for it
*
*       Replaces data in input section of ompcp with the output section
*       Data in input section is deleted
*
*       This replaces all contained data!! Replacing a Bioseq replaces any
*         attached descriptors or annots or history as well, for example.
*
*       This does NOT send update messages to the ObjMgr for you.
*
****************************************************************************/
NLM_EXTERN Boolean LIBCALL ReplaceDataForProc PROTO((OMProcControlPtr ompcp, Boolean sel));


/*****************************************************************************
*
*   GatherProcLaunch(proctype, sel, entityID, itemID, itemtype,
*        inputtype, subinputtype, outputtype, suboutputtype)
*
*   	looks for proctype of highest priority that
*   		matches inputtype and outputtype
*     if (sel) then fills in entityID,itemID,itemtype with currently selected
*        item
*     else
*        uses the function arguments
*     locates the data pointer, determines the subtype (if any)
*     then finds the highest priority process
*   	0 on outputtype, inputsubtype, outputsubtype matches any
*     if subtype can be matched on input/output, takes in preference over
*        more general proc
*
*     USAGE:
*     1) To launch an editor for the currently selected item
*      GatherProcLaunch(OMPROC_EDIT,TRUE,0,0,0,0,0,0,0);
*     2) To launch an editor to create a new seq_descr of type pub
*      GatherProcLaunch(OMPROC_EDIT,FALSE,0,0,0,OBJ_SEQDESC,Seq_descr_pub,0,0);
*     3) To launch an editor for a specific seq_descr
*      GatherProcLaunch(OMPROC_EDIT,FALSE,2,1,4,0,0,0,0);
*        (where the 3 numbers identify the seq_descr)
*     4)To launch an editor which will create a new seq_descr and attach it
*        to the currently selected Bioseq
*      GatherProcLaunch(OMPROC_EDIT,TRUE,0,0,0,OBJ_SEQDESC,Seq_descr_pub,0,0)
*        Note in this case ompcp->input_entityid, .input_itemid, input_itemtype
*          well refer to a Bioseq. The editor should check the input_itemtype
*          and decide if it can attach it's output to it, or if it is an
*          input type mismatch error.
*     5) To launch an editor which will create a new seq_descr and attach to
*        a specific Bioseq
*         (Same as (4) but sel=FALSE, and entitid,itemid,itemtype filled in
*          for the Bioseq).
*
*     GENERAL RULES:
*
*     All this means the function will be called with OMProcControlPtr (ompcp)
*      fields filled in (input_entityid, input_itemid, input_itemtype) as:
*       1) Create new one, place in desktop = 0,0,0
*       2) Edit old one, non-zero values, with input_itemtype matching the type
*          of the editor.
*       3) Create a new one, attach it to something else, non-zero values,
*          with input_itemtype not matching the type of the editor.
*
*     Functions to install the returned values are for the cases above:
*       1) ObjMgrRegister()
*       2) GatherReplaceDataForProc()
*       3) GatherAttachDataForProc()
*
*     returns the return from the proc, or OM_MSG_RET_NOPROC if not found
*
*****************************************************************************/
NLM_EXTERN Int2 GatherProcLaunch PROTO((Uint2 proctype, Boolean sel, Uint2 entityID, Uint2 itemID,
                    Uint2 itemtype, Uint2 inputtype, Uint2 subinputtype, Uint2 outputtype, Uint2 suboutputtype));

/*****************************************************************************
*
*   GatherSpecificProcLaunch(procid, procname, proctype, sel, entityID, itemID, itemtype)
*
*   	Launches specific viewer or editor
*
*****************************************************************************/
NLM_EXTERN Int2 GatherSpecificProcLaunch PROTO((Uint2 procid, CharPtr procname, Uint2 proctype,
                                          Boolean sel, Uint2 entityID, Uint2 itemID, Uint2 itemtype));

/*****************************************************************
*
*  GatherOverWrite (oldptr, newptr, type)
*      type is OBJ_...
*      overwrites oldptr with contents of newptr
*      sets any "next" pointers contained in newptr to point to the same
*        chain as those in oldptr did
*      At the end, oldptr contains the data from newptr,
*                  newptr contains the data from oldptr
*                  newptr is removed from any chain
*                  oldptr remains in whatever chain it was in before
*                  CALLER must call appropriate free function for newptr
*      this function is used for making a varient copy of an object, then
*        replacing it in another object without changing points to or from
*        this object
*******************************************************************/
NLM_EXTERN Boolean LIBCALL GatherOverWrite PROTO((Pointer oldptr, Pointer newptr, Uint2 type));


/*****************************************************************************
*
*   AssignIDsInEntity (entityID, datatype, dataptr)
*   	Assigns entityID/itemID/itemtype, parent pointer, and prevlink to several
*       data objects.  If entityID is > 0 it looks up the registered datatype and
*       dataptr from the object manager.  Otherwise it uses the remaining parameters,
*       assigning entityID 0 to the unregistered components.
*
*   GatherObjectsInEntity (entityID, datatype, dataptr, callback, userdata, objMgrFilter)
*   	Calls callback for objects within entity.  If the objMgrFilter parameter is NULL,
*       every object type is visited, otherwise the array length should be OBJ_MAX, and
*       the elements are from the OBJ_ list.
*
*****************************************************************************/

typedef struct gatherobject {
  Uint2             entityID;
  Uint2             itemtype;
  Uint1             subtype;
  Uint2             parenttype;
  Uint4             itemID;
  Pointer           dataptr;
  Pointer           parentptr;
  Pointer PNTR      prevlink;
  Pointer           userdata;
} GatherObject, PNTR GatherObjectPtr;

typedef Boolean (*GatherObjectProc) (GatherObjectPtr gop);

NLM_EXTERN Boolean LIBCALL AssignIDsInEntity (Uint2 entityID, Uint2 datatype, Pointer dataptr);

NLM_EXTERN Boolean LIBCALL GatherObjectsInEntity (Uint2 entityID, Uint2 datatype, Pointer dataptr,
                                                  GatherObjectProc callback, Pointer userdata, BoolPtr objMgrFilter);

/*****************************************************************************
*
*   DeleteMarkedObjects (entityID, datatype, dataptr)
*   	Unlinks and removes all objects whose GatherIndex.deleteme flag is not 0.
*
*****************************************************************************/

NLM_EXTERN Boolean DeleteMarkedObjects (Uint2 entityID, Uint2 datatype, Pointer dataptr);

/*****************************************************************************
*
*   GetPointerForIDs (entityID, itemID, itemtype)
*   	Finds pointer of specified object (GatherItem replacement).
*
*****************************************************************************/

NLM_EXTERN Pointer LIBCALL GetPointerForIDs (Uint2 entityID, Uint2 itemID, Uint2 itemtype);

/*****************************************************************************
*
*   GetNextDescriptorUnindexed (bsp, choice, curr)
*   	After AssignIDsInEntity, gets next descriptor up the set hierarchy.
*
*****************************************************************************/

NLM_EXTERN SeqDescrPtr GetNextDescriptorUnindexed (
  BioseqPtr bsp,
  Uint1 choice,
  SeqDescrPtr curr
);


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
