/*  objmgr.h
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
* File Name:  objmgr.h
*
* Author:  James Ostell
*   
* Version Creation Date: 9/94
*
* $Revision: 6.37 $
*
* File Description:  Manager for Bioseqs and BioseqSets
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: objmgr.h,v $
* Revision 6.37  2004/04/01 13:43:05  lavr
* Spell "occurred", "occurrence", and "occurring"
*
* Revision 6.36  2002/07/30 14:41:45  kans
* removed omdp->rearranged
*
* Revision 6.35  2002/07/29 21:30:17  kans
* added rearranged flag to omdp
*
* Revision 6.34  2002/07/01 14:29:03  kans
* changed totobj, currobj to Uint4
*
* Revision 6.33  2002/05/31 21:53:27  yaschenk
* changing lookup by EntityID to array[][]
*
* Revision 6.32  2001/12/26 20:54:14  kans
* added defines for OM_OPT_RECORD_SUPPRESSED and OM_OPT_RECORD_DEAD
*
* Revision 6.31  2001/11/19 15:26:19  kans
* added ObjMgrDeleteAllInRecord, still need to bail in ObjMgrDelete if bulkIndexFree, then call from BioseqFree and BioseqSetFree
*
* Revision 6.30  2001/11/15 18:15:48  kans
* set bsp->omdp at creation, SeqMgrDeleteIndexesInRecord sets omdp->bulkIndexFree
*
* Revision 6.29  2001/05/31 22:58:25  kans
* added ObjMgrReapOne, DEFAULT_MAXOBJ, autoclean reaps and frees one entity at a time, as needed
*
* Revision 6.28  2001/05/31 22:33:03  kans
* added autoclean and maxobj to ObjMgr structure, ObjMgrAddFunc optionally calls ObjMgrReap and ObjMgrFreeCache to completely clear out least recently accessed objects if currobj >= maxobj
*
* Revision 6.27  2001/02/16 21:34:49  ostell
* changed GetSecs() to ObjMgrTouchCnt() to reduce system calls
*
* Revision 6.26  2000/11/28 22:59:02  kans
* omdp->lockcnt now a Uint2 to avoid overflow when locking accessions repeatedly referenced by genomic contig
*
* Revision 6.25  2000/11/28 21:43:55  kans
* added ObjMgrReportFunc for debugging
*
* Revision 6.24  2000/10/30 21:26:09  shavirin
* Changes and fixes for some MT-safety related problems.
*
* Revision 6.23  2000/04/07 15:01:24  durand
* added UPDATE_TYPE_NEWSEQ define
*
* Revision 6.22  2000/03/02 21:11:05  lewisg
* use bandalign for import sequence, make standalone ddv use viewmgr, make dialogs modal, send color update
*
* Revision 6.21  2000/02/10 17:05:52  kans
* added UPDATE_TYPE_CN3D
*
* Revision 6.20  2000/02/10 16:53:55  kans
* added UPDATE_TYPE_VIEWMGR define (presumably in the right place)
*
* Revision 6.19  1999/12/06 22:38:52  durand
* add UPDATE_TYPE_CARETPOS to be used by DDV
*
* Revision 6.18  1999/12/03 23:17:23  lewisg
* Patrick's new global update msg, argument passing when launching ddv, experimental editing
*
* Revision 6.17  1999/11/18 00:21:42  lewisg
* draw speedups and selection on mouseup
*
* Revision 6.16  1999/10/29 18:53:08  durand
* add DDVUpdateMSG typedef
*
* Revision 6.15  1999/10/15 20:56:40  lewisg
* append DDV_ColorGlobal as userdata.  free memory when cn3d terminates.
*
* Revision 6.14  1999/09/27 20:03:15  kans
* added rowID to OMMsgStruct, new ObjMgrSendRowMsg function
*
* Revision 6.13  1999/08/11 15:17:54  kans
* added ObjMgrFreeByEntityID
*
* Revision 6.12  1999/07/06 17:17:28  ywang
* add message OM_MSG_FLUSH
*
 * Revision 6.11  1998/09/28  19:54:12  kans
 * made ObjMgrDump debugging function public, no longer conditionally compiled
 *
* Revision 6.10  1998/07/01 19:11:51  kans
* added fromProcID, toProcID, OM_MSG_PROCESS, ObjMgrSendProcMsg, time of indexing, ObjMgrGetProcID, moved protFeat and cdsOrRnaFeat to seqmgr structure
*
* Revision 6.9  1998/06/30 21:40:52  kans
* added seqmgr feature indexed time stamp to omdp
*
* Revision 6.8  1998/06/29 20:35:53  kans
* added reap and reload extra functions
*
* Revision 6.7  1998/06/29 00:23:54  kans
* several changes to new indexing functions
*
* Revision 6.6  1998/06/27 22:23:44  kans
* improvements and further implementation of new indexing, exploration functions
*
* Revision 6.5  1998/06/26 22:36:19  kans
* initial work on tracking sorted features, and cds and prot links, for rapid collection
*
* Revision 6.4  1998/05/29 20:34:11  chappey
* New objmgr messages OM_MSG_HIDE, OM_MSG_SHOW
*
* Revision 6.3  1998/03/24 23:55:33  kans
* added OBJ_PROJECT define
*
* Revision 6.2  1997/11/19 22:14:39  ostell
* added support for multithreaded programs
*
* Revision 6.1  1997/09/11 15:55:49  ostell
* Added support for SetColor messages
*
* Revision 6.0  1997/08/25 18:06:49  madden
* Revision changed to 6.0
*
* Revision 5.2  1997/06/19 18:38:31  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.1  1997/06/02 17:04:14  kans
* added ObjMgrProcLoadEx
*
* Revision 5.0  1996/05/28 13:23:23  ostell
* Set to revision 5.0
*
 * Revision 4.10  1996/02/28  04:53:06  ostell
 * added ObjMgrHold suport
 *
 * Revision 4.8  1996/01/05  14:39:12  ostell
 * fix for control chars in comment
 *
 * Revision 4.6  1995/12/27  18:39:17  ostell
 * added OBJ_BIOSEQ_DELTA
 *
 * Revision 4.5  1995/12/22  14:45:45  ostell
 * added do_not_reload_from_cache to OmProcControl so that DataGet
 * functions from gather can control this behvior
 * added protection from recursion in ObjMgrDelete
 *
 * Revision 4.4  1995/12/06  14:58:12  ostell
 * added OBJ_SEQHIST_ALIGN
 *
 * Revision 4.3  1995/10/03  15:50:37  ostell
 * added support for selection by region.. now fully implemented
 *
 * Revision 4.2  1995/09/30  03:38:31  ostell
 * Changed ObjMgrMessage functions to pass a structure
 * Added support for selecting regions
 * Added ability to remove entity when no more views on it
 *
 * Revision 4.1  1995/09/25  18:06:41  ostell
 * added ObjMgrGenericAsnTextRead
 *
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 1.18  1995/07/08  15:21:01  ostell
 * Added ObjMgrSetOptions() and related functions
 * Added support for delete of entity when no more user data on it
 * Added support for DirtyFlag
 *
 * Revision 1.17  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#ifndef _NCBI_ObjMgr_
#define _NCBI_ObjMgr_

#ifndef _ASNTOOL_
#include <asn.h>
#endif

#ifndef _NCBI_Seqfeat_
#include <objfeat.h>
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
*   ObjMgr manipulates the "registry" of Objects in memory
*       assigns "EntityID" to Objects loaded in memory..
*   		only top level Object gets an EntityID
*   		caching and locking also done on top level Object
*
*   WARNING: This system currently only supports the predefined types here.
*       new types can be added with some limited recoding, but cannot be
*          added on the fly until additional work is done.
*
*****************************************************************************/

#define OBJ_ALL              0   /* object manager itself */
#define OBJ_SEQENTRY         1
#define OBJ_BIOSEQ		     2
#define OBJ_BIOSEQSET		 3
#define OBJ_SEQDESC			 4
#define OBJ_SEQANNOT		 5
#define OBJ_ANNOTDESC		 6
#define OBJ_SEQFEAT		     7
#define OBJ_SEQALIGN		 8
#define OBJ_SEQGRAPH		 9
#define OBJ_SEQSUB	        10	
#define OBJ_SUBMIT_BLOCK	11
#define OBJ_SEQSUB_CONTACT  12   /* used by gather.c */
#define OBJ_BIOSEQ_MAPFEAT  14   /* used by gather.c */
#define OBJ_BIOSEQ_SEG      15   /* used by gather.c */
#define OBJ_SEQHIST         16   /* used by gather.c */
#define OBJ_SEQHIST_ALIGN   17   /* used by gather.c */
#define OBJ_BIOSEQ_DELTA    18   /* used by gather.c */

#define OBJ_PUB             20
#define OBJ_SEQFEAT_CIT     21  /* used by gather.c */
#define OBJ_SEQSUB_CIT      22  /* used by gather.c */
#define OBJ_MEDLINE_ENTRY   23  /* converted to a Pub on input */

#define OBJ_PUB_SET         24

#define OBJ_SEQLOC          25
#define OBJ_SEQID           26

#define OBJ_SEQCODE         27
#define OBJ_SEQCODE_SET     28
#define OBJ_GENETIC_CODE    29
#define OBJ_GENETIC_CODE_SET 30

#define OBJ_TEXT_REPORT		31
#define OBJ_FASTA			32
#define OBJ_VIBRANT_PICTURE 33

#define OBJ_PROJECT         34

#define OBJ_MAX            35    /* number of predefined types including 0 */


/*****************************************************************************
*
*   OMNewFunc()
*   	allocates an object of type
*   	sets default values if any
*   	this will not necessarily be ready for AsnWrite (may be lacking
*            required elements)
*       this will be suitable for input to editors or OMFreeFunc()
*
*****************************************************************************/
typedef Pointer (LIBCALLBACK *OMNewFunc) PROTO((void));

/*****************************************************************************
*
*   OMFreeFunc(data)
*   	frees datapointer passed to it.
*   	supplied to several functions
*   	callback must know what type of pointer is expected
*
*****************************************************************************/
typedef Pointer (LIBCALLBACK *OMFreeFunc) PROTO((Pointer data));

/*****************************************************************************
*
*   OMLabelFunc(data, buffer, buflen, content)
*   	returns a short label of content=
*   		OM_LABEL_TYPE = definition of data type.
*   		OM_LABEL_CONTENT = short label based on content
*   		OM_LABEL_BOTH = TYPE: CONTENT
*   		OM_LABEL_SUMMARY = produce a one line summary of content
*   	fills supplied buffer up to buflen chars.
*   
*   NOTE:
*   	this means buf MUST be (buflen+1) chars long.
*   
*   	adds the terminating \0
*   	returns number of characters in buffer not counting the \0
*
*****************************************************************************/
typedef Int2 (LIBCALLBACK *OMLabelFunc) PROTO((Pointer data, CharPtr buffer, Int2 buflen, Uint1 content));

#define OM_LABEL_TYPE 0
#define OM_LABEL_CONTENT 1
#define OM_LABEL_BOTH    2
#define OM_LABEL_SUMMARY 3

/*****************************************************************************
*
*   OMSubTypeFunc(Pointer)
*   	returns the proper subtype value given the data pointer
*   	default is to return 0
*       This is used for procedures that support a particular subtype
*         (a certain feature type or descriptor type)
*
*****************************************************************************/
typedef Uint2 (LIBCALLBACK *OMSubTypeFunc) PROTO((Pointer ptr));


/*****************************************************************************
*
*   OMMessageFunc(message)
*   	when a proc registers a message function with an entity, it will
*   	 recieve all messages emenating from that entity.
*
*****************************************************************************/
typedef struct ommsgstruct {
	Int2 message;    /* the message code, defined below */
	Uint2 entityID,
		itemID,
		itemtype,
		rowID;
	Uint2 fromProcID,
		toProcID;         /* allows communication to and from particular procs */
	Pointer procmsgdata;  /* procedure specific data to be sent, assumes toProcID set */
	Pointer omuserdata;	  /* this is really an OMUserDataPtr */
	Pointer region;       /* this restricts the message to a region. See below */
	Uint1 regiontype;     /*  see define below */
	Uint1 rgb [3];        /* for set color message */
} OMMsgStruct, PNTR OMMsgStructPtr;

	/**** defines for regiontype (above) *****/

#define OM_REGION_SEQLOC	1    /* region is a SeqLocPtr of type SEQLOC_INT */

    /**** From of message callback supplied by user ******/

typedef Int2 (LIBCALLBACK *OMMessageFunc) PROTO((OMMsgStructPtr message));

	/**** defines for message function return values ******/

#define OM_MSG_RET_ERROR   0	  /* an error occurred handling this message */
#define OM_MSG_RET_OK      1      /* message handled ok */
#define OM_MSG_RET_DEL     2      /* due to message, remove this handler */
#define OM_MSG_RET_DONE    3      /* handled, do not send message further */
#define OM_MSG_RET_NOPROC  4      /* specified procedure was not found */

    /**** defines for message value (OMMsgStruct.message) *******/

#define OM_MSG_DEL         1      /* this item is being deleted */
#define OM_MSG_CREATE      2      /* this item was just created */
#define OM_MSG_UPDATE      3      /* this item was updated */
#define OM_MSG_SELECT      4      /* this item selected */
#define OM_MSG_DESELECT    5      /* this item deselected */
#define OM_MSG_CACHED      6      /* item was cached */
#define OM_MSG_UNCACHED    7      /* item reloaded from cache */
#define OM_MSG_TO_CLIPBOARD 8     /* object put in clipboard */
#define OM_MSG_CONNECT      9      /* registered entity being incorporated into
		                               another. */
#define OM_MSG_SETCOLOR    10     /* set color of region */
#define OM_MSG_HIDE        11     /* hide sequence */
#define OM_MSG_SHOW        12     /* show sequence */
#define OM_MSG_PROCESS     13     /* process-specific message */
#define OM_MSG_FLUSH       14     /* close all existed windows */
#define OM_MSG_MOUSEUP     15     /* mouse up on selection has occurred */

/*****************************************************************************
*
*  data type used to send additional information for an update msg
*
*****************************************************************************/
#define UPDATE_TYPE_LAYOUT       (1)
#define UPDATE_TYPE_COLOR        (2)
#define UPDATE_TYPE_EDIT_DELBSP  (3)
#define UPDATE_TYPE_CARETPOS     (4)
#define UPDATE_TYPE_VIEWMGR      (5)
#define UPDATE_TYPE_RECOLOR      (6)
#define UPDATE_TYPE_NEWSEQ       (7)

typedef struct ddvupdatemsg{
	Uint1   type;
	Pointer data;
	}DDVUpdateMSG, PNTR DDVUpdateMSGPtr;


/*****************************************************************************
*
*   ObjMgrSendMsg(msg, entityID, itemID, itemtype)
*       Directly invokes the objmgr messaging system
*       should be used cautiously as most objmgr calls do their own messaging
*
*
*   ObjMgrSendProcMsg(msg, entityID, itemID, itemtype, fromProcID, toProcID)
*       Allows messages to be sent to particular procs
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrSendMsg PROTO((Uint2 msg, Uint2 entityID, Uint2 itemID, Uint2 itemtype));

NLM_EXTERN Boolean LIBCALL ObjMgrSendProcMsg PROTO((Uint2 msg, Uint2 entityID, Uint2 itemID, Uint2 itemtype,
                                                    Uint2 fromProcID, Uint2 toProcID, Pointer procmsgdata));

NLM_EXTERN Boolean LIBCALL ObjMgrSendRowMsg PROTO((Uint2 msg, Uint2 entityID, Uint2 itemID, Uint2 itemtype, Uint2 rowID));

	                              /* user supplied data on entity */
typedef struct omuserdata {
	Uint2 procid,                 /* proc that put userdata here */
		proctype,                 /* OMPROC_... */
		userkey;                  /* user supplied key */
	DataVal userdata;             /* user supplied data */
	OMFreeFunc freefunc;      /* function to free user supplied data.ptrvalue */
	OMMessageFunc messagefunc;    /* function to recieve OM messages */
	struct omuserdata PNTR next;
} OMUserData, PNTR OMUserDataPtr;

typedef struct objmgrdata {
	Uint2 datatype;
	Pointer dataptr;
	Uint2 parenttype;
	Pointer parentptr;
	Uint2 choicetype;
	ValNodePtr choice;           /* CHOICE wrapper (if datatype=Bioseq or Set) */
	Uint2 lockcnt;             /* count of locks */
	Uint1 tempload;
	Uint4 touch;                /* last time this was unlocked */
	Uint2 EntityID;             /* arbitrary ID assigned to complete entity */
	Boolean clipboard;         /* is this in the clipboard? */
	Boolean dirty;             /* updated without a save? */
	Boolean being_freed;       /* in-process of being freed, set by ObjMgrDel */
	Boolean free;
	Boolean bulkIndexFree;     /* used to suppress individual SeqMgrDeleteFromBioseqIndex */
	OMUserDataPtr userdata;    /* for user supplied data */
	Uint2 options;             /* options set with bit flags, defined below */
	Uint2 lastDescrItemID;     /* for new seqmgr extra bioseq exploration functions */
	time_t indexed;            /* time that entity was seqmgr indexed */
	Pointer extradata;         /* used (initially) for extra bioseq feature indices */
	OMFreeFunc reapextra;      /* function to call when caching out, passed extradata */
	OMFreeFunc reloadextra;    /* function to call when reloading, passed extradata */
	OMFreeFunc freeextra;      /* function to call when deleting, passed extradata */
} ObjMgrData, PNTR ObjMgrDataPtr;

/** values for tempload (above) **/
#define TL_NOT_TEMP     0   /* not a temporarily loaded guy */
#define TL_LOADED       1   /* in memory (but temporarily)  */
#define TL_CACHED       2   /* out of memory, but cached somewhere */


/*****************************************************************************
*
*   A registered type provides:
*   	generic copy (to clipboard)
*   	generic match
*   	generic open
*   	generic free
*   	generic display (in asn.1)
*		cut (complete entity)
*   	paste (complete entity to desktop)
*
*   Procs must be provided for everything else
*   
*
*****************************************************************************/


typedef struct objtypedata {
	Uint2 datatype;             /* same as datatype above */
	CharPtr asnname,          	/* asn.1 name for this */
		label,                  /* a short label for this type */
		name;                   /* a descriptive name */
	AsnTypePtr atp;             /* the base type pointer */
	OMNewFunc newfunc;
	AsnReadFunc asnread;
	AsnWriteFunc asnwrite;
	OMFreeFunc freefunc;          /* generic free function */
	OMLabelFunc labelfunc;
	OMSubTypeFunc subtypefunc;    /* get subtype (if any) given dataptr of OBJ_ */
	Uint2 tochoicetype;           /* if this could be a choice, the OBJ_ for it */
	Uint1 tochoice;               /* value of vnp->choice if this is made a choice */
} ObjMgrType, PNTR ObjMgrTypePtr;


/*****************************************************************************
*
*   Procs provide additional functions by data type.
*   	If the type is left off either input or output, then assumes anytype
*
*   paste: inputtype = type to paste
*          outputtype = type to paste it into
*   cut:   inputtype = type to cut it from
*          outputtype = type that is cut out
*   delete: like cut
*   filter: inputtype = type that goes in the filter
*           outputtype = type that comes out
*   transform: this is a destructive filter (input type disappears)
*   edit: inputtype and output type are the same
*   view: inputtype is a datatype.
*
*****************************************************************************/


                /* general function. ompcp is really an ObjMgrProcControlPtr */
                /* return is an OM_MSG_RET */
typedef Int2 (LIBCALLBACK * ObjMgrGenFunc) PROTO ((Pointer ompcp));

#define NUM_OPT 20
typedef struct objprocdata {
	Uint2 proctype,             /* type of procedure (PROC_) */
		  subproctype,          /* used to distinguish procs */
		  procid;				/* unique id for this proc */
	CharPtr procname,			/* human readable name */
		proclabel,				/* short label */
		submenu;				/* optional submenu name */
	Uint2 inputtype,            /* input object (OBJ_) */
		  subinputtype,         /* for distinguishing CHOICEs */
		outputtype,             /* output object (OBJ_) */
		  suboutputtype;         /* for distinghishing CHOICEs  */
	Pointer procdata;           /* data used by proc */
	ObjMgrGenFunc func;         /* the function pointer */
	Int2 priority;              /* higher number = higher priority */
	DataVal options[NUM_OPT];   /* for function options */
} ObjMgrProc, PNTR ObjMgrProcPtr;

typedef struct omproccontrol {
    Boolean whole_entity;      /* TRUE if whole entity is selected */
	Pointer input_data,
			input_choice;      
	Uint2 input_entityID,
		input_itemID,
		input_itemtype,
		input_choicetype;
	Uint1 input_regiontype;   /* for region of entity */
	Pointer input_region;     
	Pointer output_data,
		output_choice;      
	Uint2 output_entityID,
		output_itemID,
		output_itemtype,
		output_choicetype;
	Uint1 output_regiontype;
	Pointer output_region;
	ObjMgrProcPtr proc;
	Boolean do_not_reload_from_cache;   /* default is for gather to reload cache */
} OMProcControl, PNTR OMProcControlPtr;

#define OMPROC_OPEN		1
#define OMPROC_DELETE	2
#define OMPROC_VIEW		3
#define OMPROC_EDIT		4
#define OMPROC_SAVE		5
#define OMPROC_CUT		6
#define OMPROC_COPY		7
#define OMPROC_PASTE	8
#define OMPROC_ANALYZE  9
#define OMPROC_FIND		10
#define OMPROC_REPLACE	11
#define OMPROC_FILTER	12
#define OMPROC_FETCH	13
#define OMPROC_COLORMGR 14

#define OMPROC_MAX      14


#define NUM_OMD 50

typedef struct omdata {
	ObjMgrData data[NUM_OMD];
	struct omdata PNTR next;
} OMData, PNTR OMDataPtr;

typedef struct omproc {
	ObjMgrProc data[NUM_OMD];
	struct omproc PNTR next;
} OMProc, PNTR OMProcPtr;

typedef struct omtype {
	ObjMgrType data[NUM_OMD];
	struct omtype PNTR next;
} OMType, PNTR OMTypePtr;

typedef struct selstruct {
	Uint2 entityID,
		  itemtype,
		  itemID;
	Pointer region;       /* this restricts the message to a region.
	                         currently only a SeqLocPtr of type SEQLOC_INT */
	Uint1 regiontype;     /* 0 = not set
	                         1 = SeqLocPtr of type SEQLOC_INT	*/							 
	struct selstruct PNTR prev,
		PNTR next;
} SelStruct, PNTR SelStructPtr;

typedef struct objmng {        /* functions for data management */
	Uint2 options;             /* options set with bit flags, defined below */

	OMTypePtr ncbitype;    /* types needed by NCBI functions */
	ObjMgrTypePtr PNTR typelist; /* sorted by type */
	Uint2 tottype,                 /* number of elements in ncbitype */
		 currtype,                /* number of elements in ncbitype occupied */
		HighestObjMgrType;          /* set to OBJ_MAX on startup */

	OMProcPtr ncbiproc;          /* NCBI functions */
	ObjMgrProcPtr PNTR proclist; /* sorted by proctype */
	Uint2 totproc,                 /* number of elements in ncbiproc */
		 currproc,                /* number of elements in ncbiproc occupied */
		HighestProcType,			  /* set to PROC_MAX on startup */
		HighestProcID,
		HighestUserKey;

	OMDataPtr ncbidata;    /* data needed by NCBI functions */
	ObjMgrDataPtr PNTR datalist; /* sorted by pointer */
	Uint4 totobj,                 /* number of elements in ncbidata */
		 currobj;                /* number of elements in ncbidata occupied */
	Uint2 HighestEntityID;
	ObjMgrDataPtr clipboard;
	SelStructPtr sel;               /* for currently selected data item(s) */

	Uint2 maxtemp,                /* maximum number of unlocked temp entries to keep */
		 tempcnt;                /* count of unlocked temp entries */
	Uint2 hold;                  /* if not 0, no reaping is done until it is 0 */
	Boolean reaping;             /* to protect against recursive calls to ObjMgrReap due to Msg */
	Boolean is_write_locked;     /* needed to unlock during messaging */

	OMUserDataPtr userdata;      /* for global messageing */

	Boolean autoclean;           /* if TRUE, reap and free cache if currobj > maxobj */
	Uint2 maxobj;                /* number of objects that tiggers autocleaning */

	ObjMgrDataPtr PNTR PNTR	entityID_index; /**** two-level array index on Entity ID *****/

} ObjMgr, PNTR ObjMgrPtr;

#define DEFAULT_MAXTEMP 5   /* default number of temp entries to keep */
#define DEFAULT_MAXOBJ 50000   /* default number of objects to keep for autocleaning */

/*****************************************************************************
*
*   Return the current ObjMgr
*   	Initialize if not done already
*
*****************************************************************************/
NLM_EXTERN ObjMgrPtr LIBCALL ObjMgrGet PROTO((void));
/*****************************************************************************
*
*   ObjMgrReadLock()
*   	Initialize if not done already
*       A thread can have only one read or write lock at a time
*       Many threads can have read locks
*       Only one thread can have a write lock
*       No other threads may have read locks if a write lock is granted
*       If another thread holds a write lock, this call blocks until write
*          is unlocked.
*
*****************************************************************************/
NLM_EXTERN ObjMgrPtr LIBCALL ObjMgrReadLock PROTO((void));

/*****************************************************************************
*
*   ObjMgrWriteLock()
*   	Initialize if not done already
*       A thread can have only one read or write lock at a time
*       Many threads can have read locks
*       Only one thread can have a write lock
*       No other threads may have read locks if a write lock is granted
*       If another thread holds a read or write lock, this call blocks until write
*          is unlocked.
*
*****************************************************************************/
NLM_EXTERN ObjMgrPtr LIBCALL ObjMgrWriteLock PROTO((void));
/*****************************************************************************
*
*  ObjMgrUnlock()
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrUnlock PROTO((void));


/*****************************************************************************
*
*   Options are set on the ObjMgr by setting 1 bit flags
*   Currently available options are defined below.
*   Options set on the ObjMgr itself (called with entityID = 0) become
*      defaults for every data object
*   Options set on a specific entity (called with entityID > 0) apply only
*      to that entity.
*
*****************************************************************************/
#define OM_OPT_FREE_IF_NO_VIEW   1  /* free entity if no more UserData
                                       structures are attached to it */
#define OM_OPT_RECORD_SUPPRESSED 2  /* removed from distribution, still
                                       available for retrieval */
#define OM_OPT_RECORD_DEAD       4

/*****************************************************************************
*
*   ObjMgrSetHold()
*     Increments hold count in ObjMgr
*       returns current hold count
*     if hold != 0, then no tempload records are cached out of memory
*     This is useful if you are doing a complex task involving repeated
*      access to many records and you want them to stay in memory while
*      the task is performed, even though they are loaded temporarily by
*      BioseqLock..
*     Be sure to call ObjMgrClearHold() when you are done so they can be
*      cached out.
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrSetHold PROTO((void));

/*****************************************************************************
*
*   ObjMgrCheckHold()
*       returns current hold count
*     if hold == 0, then tempload records will be cached out of memory
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrCheckHold PROTO((void));

/*****************************************************************************
*
*   ObjMgrClearHold()
*     Decrements hold count in ObjMgr
*       returns current hold count
*       will never decrement below 0
*     if hold == 0, then tempload records will be cached out of memory
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrClearHold PROTO((void));

/*****************************************************************************
*
*   ObjMgrSetOptions(Uint2 option)
*     Sets options on the ObjMgr. All Options are 1 bit flags set with
*       defines. Several Options may be ORed together.
*       returns current options.
*     if entityID != 0, only applies to that entityID
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrSetOptions PROTO((Uint2 option, Uint2 entityID));

/*****************************************************************************
*
*   ObjMgrClearOptions(Uint2 option)
*     Clears options on the ObjMgr. All Options are 1 bit flags set with
*       defines. Several Options may be ORed together.
*       returns current options.
*     if entityID != 0, only applies to that entityID
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrClearOptions PROTO((Uint2 option, Uint2 entityID));

/*****************************************************************************
*
*   ObjMgrGetOptions(void)
*     Returns options on the ObjMgr. All Options are 1 bit flags set with
*       defines. Several Options may be ORed together.
*     if entityID != 0, only test that entityID
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrGetOptions PROTO((Uint2 entityID));

/*****************************************************************************
*
*   ObjMgrTestOptions(Uint2 option)
*     Tests options on the ObjMgr. All Options are 1 bit flags set with
*       defines. Several Options may be ORed together.
*       returns TRUE if ALL flags in "option" are set in the ObjMgr
*     if entityID != 0, only test that entityID
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrTestOptions PROTO((Uint2 option, Uint2 entityID));


/*****************************************************************************
*
*   Management of Data Types
*
*****************************************************************************/

NLM_EXTERN Int4 LIBCALL ObjMgrTypeLookup PROTO((ObjMgrPtr omp, Uint2 type));

/*****************************************************************************
*
*   ObjMgrTypeFind(omp, type, asnname, name)
*   	returns the objmgrptr by looking for
*   		type: if type != 0
*   		asnname: if asnname != NULL
*   		name: if name != NULL
*       in that order of preference.
*   	returns NULL if can't match on highest priority key
*
*****************************************************************************/
NLM_EXTERN ObjMgrTypePtr LIBCALL ObjMgrTypeFind PROTO((ObjMgrPtr omp, Uint2 type, CharPtr asnname, CharPtr name));

NLM_EXTERN Uint2 LIBCALL ObjMgrTypeAdd PROTO((ObjMgrTypePtr omtdp));
NLM_EXTERN Uint2 LIBCALL ObjMgrTypeLoad PROTO (( Uint2 type, CharPtr asnname,
		CharPtr label, CharPtr name, AsnTypePtr atp, OMNewFunc newfunc,
		AsnReadFunc asnread, AsnWriteFunc asnwrite, OMFreeFunc freefunc,
		OMLabelFunc labelfunc, OMSubTypeFunc subtypefunc));
/**************************************************************************
*
* ObjMgrTypeFindNext(omp, omtp)
*    returns next ObjMgrType after omtp
*    Exhaustively traverses registered types if omtp starts as NULL
*
***************************************************************************/		
NLM_EXTERN ObjMgrTypePtr LIBCALL ObjMgrTypeFindNext PROTO((ObjMgrPtr omp, ObjMgrTypePtr omtp));

/*****************************************************************************
*
*   ObjMgrTypeSetLabelFunc(type, labelfunc)
*   	replaces the labelfunc for type with a new one
*   	can also set it for the first time
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrTypeSetLabelFunc PROTO((Uint2 type, OMLabelFunc labelfunc));

/*****************************************************************************
*
*   Management of Procedures
*
*****************************************************************************/

NLM_EXTERN Int4 LIBCALL ObjMgrProcLookup PROTO((ObjMgrPtr omp, Uint2 procID));

/*****************************************************************************
*
*   ObjMgrProcAdd(data, priority)
*   	adds an ObjMgrProc at given priority
*		priority must be > 0  and less than 32000
*       highest priority function is called first.
*       if priority == 0 (default)
*   		gets the next highest priority over previous procs of same type
*       if priority == PROC_PRIORITY_HIGHEST
*           is always the highest priority (first one wins)
*   	if priority == PROC_PRIORITY_LOWEST
*           is always the lowest priority
*
*****************************************************************************/
NLM_EXTERN Uint2  LIBCALL ObjMgrProcAdd PROTO((ObjMgrProcPtr data, Int2 priority));

#define PROC_PRIORITY_HIGHEST 32767
#define PROC_PRIORITY_LOWEST  -32765
#define PROC_PRIORITY_DEFAULT 0


/*****************************************************************************
*
*   ObjMgrProcLoad(proctype, procname, proclabel, inputtype, subinputtype,
*              outputtype, suboutputtype,
*   				data, func, priority)
*   	adds a new proceedure with these parameters
*   	returns the procid
*   	if a procedure of the same name and type are already loaded
*   		returns the procid of the loaded proc.. does not reload
*
*****************************************************************************/
NLM_EXTERN Uint2 ObjMgrProcLoad PROTO((Uint2 proctype, CharPtr procname, CharPtr proclabel,
                                       Uint2 inputtype, Uint2 subinputtype,
                                       Uint2 outputtype, Uint2 suboutputtype, Pointer userdata,
                                       ObjMgrGenFunc func, Int2 priority));

/*****************************************************************************
*
*   ObjMgrProcLoadEx(proctype, procname, proclabel, inputtype, subinputtype,
*              outputtype, suboutputtype,
*   				data, func, priority, submenu)
*   	adds a new proceedure with these parameters
*   	returns the procid
*   	if a procedure of the same name and type are already loaded
*   		returns the procid of the loaded proc.. does not reload
*
*****************************************************************************/
NLM_EXTERN Uint2 ObjMgrProcLoadEx PROTO((Uint2 proctype, CharPtr procname, CharPtr proclabel,
                                         Uint2 inputtype, Uint2 subinputtype,
                                         Uint2 outputtype, Uint2 suboutputtype, Pointer userdata,
                                         ObjMgrGenFunc func, Int2 priority, CharPtr submenu));

/*****************************************************************************
*
*   ObjMgrProcFind(omp, procid, procname, proctype)
*   	if procid != NULL looks for it
*   	else matches on procname and proctype
*   		proctype = 0, matches all proctypes
*
*****************************************************************************/
NLM_EXTERN ObjMgrProcPtr LIBCALL ObjMgrProcFind PROTO((ObjMgrPtr omp, Uint2 procid,
                                                       CharPtr procname, Uint2 proctype));

/*****************************************************************************
*
*   ObjMgrGetProcID(omp, procname, proctype)
*   	returns procid given procname and proctype
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrGetProcID PROTO((ObjMgrPtr omp, CharPtr procname, Uint2 proctype));

/*****************************************************************************
*
*   ObjMgrProcFindNext(omp, proctype, inputtype, outputtype, last)
*   	looks for proctype of highest priority that
*   		matches inputtype and outputtype
*   	0 on proctype or inputtype or outputtype matches any
*   	if last != NULL, then gets next after last
*
*****************************************************************************/
NLM_EXTERN ObjMgrProcPtr LIBCALL ObjMgrProcFindNext PROTO((ObjMgrPtr omp, Uint2 proctype,
						Uint2 inputtype, Uint2 outputtype, ObjMgrProcPtr last));

/*****************************************************************************
*
*   ObjMgrProc Calls to run classes of procs
*
*****************************************************************************/


NLM_EXTERN Int2 LIBCALL ObjMgrProcOpen PROTO((ObjMgrPtr omp, Uint2 outputtype));

/*****************************************************************************
*
*   OMGetNextUserKey returns a unique user key
*
*****************************************************************************/


NLM_EXTERN Uint2 LIBCALL OMGetNextUserKey PROTO((void));


/*****************************************************************************
*
*   Management of Data Objects
*
*****************************************************************************/

NLM_EXTERN ObjMgrDataPtr LIBCALL ObjMgrFindByData PROTO((ObjMgrPtr omp, Pointer ptr));
NLM_EXTERN Int4 LIBCALL ObjMgrLookup PROTO((ObjMgrPtr omp, Pointer data));
NLM_EXTERN Boolean LIBCALL ObjMgrAdd PROTO((Uint2 type, Pointer data));
NLM_EXTERN Boolean LIBCALL ObjMgrDelete PROTO((Uint2 type, Pointer data));
NLM_EXTERN Boolean LIBCALL ObjMgrDeleteAllInRecord (void);
NLM_EXTERN Boolean LIBCALL ObjMgrConnect PROTO((Uint2 type, Pointer data, Uint2 parenttype, Pointer parentdata));
NLM_EXTERN Boolean LIBCALL ObjMgrConnectFunc PROTO((ObjMgrPtr omp, Uint2 type, Pointer data, Uint2 parenttype, Pointer parentdata));

/*****************************************************************************
*
*   ObjMgrDetach (type, data)
*   	Removes parent info from element
*       Adds to objmgr if necessary
*       Does NOT register entity
*       (opposite of ObjMgrConnect)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrDetach PROTO((Uint2 type, Pointer data));
NLM_EXTERN Boolean LIBCALL ObjMgrDetachFunc PROTO((ObjMgrPtr omp, Uint2 type, Pointer data));

/*****************************************************************************
*
*   ObjMgrSetDirtyFlag (entityID, the_flag)
*     record that an entity has been changed but not yet saved.
*       the_flag sets or unsets this state
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrSetDirtyFlag PROTO((Uint2 entityID, Boolean the_flag));

/*****************************************************************************
*
*   ObjMgrGetDirtyFlag (entityID)
*     returns the state of the dirty flag for entityID
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrGetDirtyFlag PROTO((Uint2 entityID));

NLM_EXTERN Boolean LIBCALL ObjMgrSetChoice PROTO((Uint2 type, ValNodePtr choice, Pointer data));
NLM_EXTERN Int4 LIBCALL ObjMgrLock PROTO((Uint2 type, Pointer data, Boolean lockit));
NLM_EXTERN Boolean LIBCALL ObjMgrSetTempLoad PROTO((ObjMgrPtr omp, Pointer ptr));
NLM_EXTERN Boolean LIBCALL ObjMgrReap PROTO((ObjMgrPtr omp));
NLM_EXTERN Boolean LIBCALL ObjMgrReapOne PROTO((ObjMgrPtr omp));
NLM_EXTERN ObjMgrDataPtr LIBCALL ObjMgrFindTop PROTO((ObjMgrPtr omp, ObjMgrDataPtr smdp));
NLM_EXTERN ObjMgrDataPtr LIBCALL ObjMgrGetDataStruct PROTO((ObjMgrPtr omp,
														   Uint2 entityID));
NLM_EXTERN ObjMgrDataPtr LIBCALL ObjMgrGetData PROTO((Uint2 entityID));
NLM_EXTERN Uint2 LIBCALL ObjMgrAddEntityID PROTO((ObjMgrPtr omp, ObjMgrDataPtr omdp));


/*****************************************************************************
*
*   ObjMgrWholeEntity(omdp, itemID, itemtype)
*	returns TRUE if itemID, itemtype identify a complete entity omdp
*       returns FALSE if these are an internal part of the entity
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrWholeEntity PROTO((ObjMgrDataPtr omdp, Uint2 itemID, Uint2 itemtype));

/*****************************************************************************
*
*   ObjMgrRegister (datatype, data)
*   	datatype is the datatype of data to register
*   	if data is already registered in ObjMgr, returns entityID
*   	if not, is added to the ObjMgr, returns entityID
*
*   	if (datatype is a choice type, uses data as a ValNodePtr)
*   
*   	on failure returns 0
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrRegister PROTO((Uint2 datatype, Pointer data));

/*****************************************************************************
*
*   ObjMgrAddUserData(entityID, procid, proctype, userkey)
*   	creates a new OMUserData struct attached to entityID
*   	if entityID = 0, attaches to the desktop (all objects)
*   	Caller must fill in returned structure
*   	returns NULL on failure
*
*****************************************************************************/
NLM_EXTERN OMUserDataPtr LIBCALL ObjMgrAddUserData PROTO((Uint2 entityID, Uint2 procid, Uint2 proctype, Uint2 userkey));

/*****************************************************************************
*
*   ObjMgrGetUserData(entityID, procid, proctype, userkey)
*   	returns OMUserData attached to entityID by procid
*       if procid ==0, returns first OMUserData of proctype
*       if userkey == matches any userkey
*   	returns NULL on failure
*
*****************************************************************************/
NLM_EXTERN OMUserDataPtr LIBCALL ObjMgrGetUserData PROTO((Uint2 entityID, Uint2 procid, Uint2 proctype, Uint2 userkey));

/*****************************************************************************
*
*   ObjMgrFreeUserData(entityID, procid, proctype, userkey)
*   	frees OMUserData attached to entityID by procid
*       if procid ==0, frees all OMUserData of proctype
*   	if proctype ==0, matches any proctype
*       if userkey == matches any userkey
*       if ObjMgrGetOptions(OM_OPT_FREE_IF_NO_VIEWS) if TRUE
*            and this is the last UserData, will free the entity
*   	returns TRUE if any freed
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrFreeUserData PROTO((Uint2 entityID, Uint2 procid, Uint2 proctype, Uint2 userkey));


/*****************************************************************************
*
*   ObjMgrGetClipBoard()
*     returns ObjMgrDataPtr to current clipboard object or NULL if none
*
*****************************************************************************/
NLM_EXTERN ObjMgrDataPtr LIBCALL ObjMgrGetClipBoard PROTO((void));

/*****************************************************************************
*
*   ObjMgrAddToClipBoard(entityID, ptr)
*   	if entityID > 0, then uses it.
*   	else, looks up entityID using ptr
*       adds entityID if needed
*       sends OM_MSG_TO_CLIPBOARD
*
*   	Anything in the clipboard is deleted
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrAddToClipBoard PROTO((Uint2 entityID, Pointer ptr));

/*****************************************************************************
*
*   ObjMgrFreeClipBoard()
*     clears any data from the clipboard
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrFreeClipBoard PROTO((void));

/*****************************************************************************
*
*   ObjMgrFreeCache(type)
*   	Frees all cached objects of type and subtypes of type
*   		based on ObjMgrMatch()
*   	if type == 0, frees all cached objects
*   	returns TRUE if no errors occurred
*
*       if ObjMgrSetHold() has been called but not cleared, nothing will
*         be removed from cache, since it is held, but this function will
*         return FALSE to warn you.
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrFreeCache PROTO((Uint2 type));

/*****************************************************************************
*
*   ObjMgrMatch(type1, type2)
*   	returns 0 if no match
*   	1 if identical
*   	2 if 2 is a subset of 1   (e.g. 1=OBJ_SEQENTRY, 2=BIOSEQ)
*       current type1 that can have subtypes are:
*   		OBJ_SEQENTRY
*   		OBJ_PUB
*   		OBJ_SEQANNOT
*   		OBJ_SEQCODE_SET
*   		OBJ_GENETIC_CODE_SET
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL ObjMgrMatch PROTO((Uint2 type1, Uint2 type2));

/*****************************************************************************
*
*   Boolean ObjMgrIsTemp(data)
*   	returns TRUE if data is a temporarily loaded item
*       data must be BioseqPtr or BioseqSetPtr
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrIsTemp PROTO((Pointer data));

/*****************************************************************************
*
*   Boolean ObjMgrIsParent(parent, child)
*   	returns TRUE if child is a child of parent
*       if parent = NULL, returns TRUE if child has no parent
*       child must never be NULL
*       returns TRUE if they are the equal
*       data must be BioseqPtr or BioseqSetPtr
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrIsChild PROTO((Pointer parent, Pointer child));

/*****************************************************************************
*
*   ObjMgrDump(fp)
*       prints status of internal object manager data for debugging
*
*****************************************************************************/

NLM_EXTERN void LIBCALL ObjMgrDump PROTO((FILE * fp, CharPtr title));

/*****************************************************************************
*
*   ObjMgrGetChoiceForData(data)
*   	returns ValNodePtr for a BioseqPtr or BioseqSetPtr
*       choice must have been put in ObjMgr using ObjMgrSeqEntry
*       the Bioseq/BioseqSets it is a part of must also be in ObjMgr
*       returns NULL on failure.
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ObjMgrGetChoiceForData PROTO((Pointer data));

/*****************************************************************************
*
*   ObjMgrGetEntityIDForChoice(choice)
*   	returns the EntityID for a ValNodePtr
*       choice must have been put in ObjMgr using ObjMgrChoice
*       the Bioseq/BioseqSets it is a part of must also be in ObjMgr
*       This function will move up to the top of the Choice tree it may be
*          in. If top level EntityID is 0, one is assigned at this point.
*       If an element is moved under a different hierarchy, its EntityID will
*          change.
*
*       Either ObjMgrGetEntityIDForPointer() or ObjMgrGetEntityIDForChoice()
*   		MUST be called to have an OM_MSG_CREATE message sent to any
*           registered proceedures
*   
*       returns 0 on failure.
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrGetEntityIDForChoice PROTO((ValNodePtr choice));

/*****************************************************************************
*
*   ObjMgrGetEntityIDForPointer(data)
*   	returns the EntityID for any pointer, Choice or Data
*       This function will move up to the top of the tree it may be
*          in. If top level EntityID is 0, one is assigned at this point.
*       If an element is moved under a different hierarchy, its EntityID will
*          change.
*
*       Either ObjMgrGetEntityIDForPointer() or ObjMgrGetEntityIDForChoice()
*   		MUST be called to have an OM_MSG_CREATE message sent to any
*           registered proceedures
*   
*       returns 0 on failure.
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrGetEntityIDForPointer PROTO((Pointer ptr));

/*****************************************************************************
*
*   ObjMgrGetChoiceForEntityID (id)
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ObjMgrGetChoiceForEntityID PROTO((Uint2 id));

/*****************************************************************************
*
*   Selection Functions for data objects
*      See also SeqLoc specific selection in seqmgr.h
*
*****************************************************************************/

/*****************************************************************************
*
*   ObjMgrSelect(entityID, itemID, itemtype, regiontype, region)
*      if entityID == 0, just deselects everything
* if entityID,itemID, itemtype already selected, deselects everything else
*      if something else selected, deselects it first, then selects requested
*        item
*      if regiontype != 0, and restricts to region
*      use ObjMgrAlsoSelect() to avoid implicit deselection of other items
*      returns TRUE if item is now currently selected, FALSE if not
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrSelect PROTO((Uint2 entityID, Uint2 itemID,
                                               Uint2 itemtype, Uint1 regiontype, Pointer region));
NLM_EXTERN Boolean LIBCALL ObjMgrAlsoSelect PROTO((Uint2 entityID, Uint2 itemID,
                                                   Uint2 itemtype, Uint1 regiontype, Pointer region));

/*****************************************************************************
*
*   ObjMgrSetColor(entityID, itemID, itemtype, regiontype, region, rgb);
*      Sets color of object in displays
*      if regiontype != 0, and restricts to region
*      rgb is a pointer to a Uint1[3] array containing an RGB value.
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrSetColor PROTO((Uint2 entityID, Uint2 itemID,
                                        Uint2 itemtype, Uint1 regiontype,
                                        Pointer region, Uint1Ptr rgb));
  
/*****************************************************************************
*
*   ObjMgrDeSelect(entityID, itemID, itemtype)
*   	if this item was selected, then deselects and returns TRUE
*   	else returns FALSE
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrDeSelect PROTO((Uint2 entityID, Uint2 itemID,
						 Uint2 itemtype, Uint1 regiontype, Pointer region));
NLM_EXTERN Boolean LIBCALL ObjMgrDeSelectAll PROTO((void));
	
/*************************************************************************
*
*   ObjMgrGetSelected() returns a linked list of selected items or NULL
*   	if nothing is selected. Do not modify this list. List may be
*       changed by later calls, so do not keep it.
*
*************************************************************************/
NLM_EXTERN SelStructPtr LIBCALL ObjMgrGetSelected PROTO((void));


/*****************************************************************************
*
*   ObjMgrGenericAsnTextFileRead(filename, datatypeptr, entityIDptr)
*      reads an asn1 text file for any datatype registered with the ObjMgr.
*      filename may contain a path
*      scans the start of the file to figure out the type.
*
*      if it fails, it returns NULL
*      if it succeeds, it returns a Pointer to the loaded data object
*          if datatypeptr is not NULL, fills it in with the proper OBJ_...
*          if entityIDptr is not NULL,
*                it registers the data with the object manager
*                it fills in entityIDptr with the entityID of the loaded object
*
*****************************************************************************/
NLM_EXTERN Pointer LIBCALL ObjMgrGenericAsnTextFileRead PROTO((CharPtr filename,
                                 Uint2Ptr datatypeptr, Uint2Ptr entityIDptr));

/******************************************************************************
*
*  ObjMgrMemCopy(type, ptr)
*    Uses AsnIoMemCopy to make a copy of any ObjMgr supported type
*    ObjMgrType for "type" must have been previously loaded
*
******************************************************************************/
NLM_EXTERN Pointer LIBCALL ObjMgrMemCopy PROTO((Uint2 type, Pointer ptr));

/******************************************************************************
*
*  ObjMgrFree(type, ptr)
*    ObjMgrType for "type" must have been previously loaded
*
******************************************************************************/
NLM_EXTERN Pointer LIBCALL ObjMgrFree PROTO((Uint2 type, Pointer ptr));

/******************************************************************************
*
*  ObjMgrFreeByEntityID(entityID)
*    Obtains type and ptr from ObjMgrDataPtr, then calls ObjMgrFree
*
******************************************************************************/
NLM_EXTERN Pointer LIBCALL ObjMgrFreeByEntityID PROTO((Uint2 entityID));

NLM_EXTERN void LIBCALL ObjMgrResetAll PROTO((void));

/* debugging function */

NLM_EXTERN void LIBCALL ObjMgrReportFunc (CharPtr filename);


NLM_EXTERN void LIBCALL ObjMgrAddIndexOnEntityID PROTO((ObjMgrPtr omp,Uint2 entityID,ObjMgrDataPtr omdp));
NLM_EXTERN void LIBCALL ObjMgrDeleteIndexOnEntityID PROTO((ObjMgrPtr omp,Uint2 entityID));
NLM_EXTERN ObjMgrDataPtr LIBCALL ObjMgrLookupIndexOnEntityID PROTO((ObjMgrPtr omp,Uint2 entityID));

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

