/*  objmgr.c
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
* File Name:  objmgr.c
*
* Author:  James Ostell
*   
* Version Creation Date: 9/94
*
* $Revision: 6.43 $
*
* File Description:  Manager for Bioseqs and BioseqSets
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: objmgr.c,v $
* Revision 6.43  2001/12/13 13:59:14  kans
* ObjMgrSendMsg clears feature indexes if OM_MSG_UPDATE
*
* Revision 6.42  2001/11/30 12:19:47  kans
* ObjMgrDelete bails if omdp->bulkIndexFree
*
* Revision 6.41  2001/11/19 15:27:52  kans
* minor fix to ObjMgrDeleteAllInRecord
*
* Revision 6.40  2001/11/19 15:26:19  kans
* added ObjMgrDeleteAllInRecord, still need to bail in ObjMgrDelete if bulkIndexFree, then call from BioseqFree and BioseqSetFree
*
* Revision 6.39  2001/11/15 18:15:48  kans
* set bsp->omdp at creation, SeqMgrDeleteIndexesInRecord sets omdp->bulkIndexFree
*
* Revision 6.38  2001/10/02 12:22:57  kans
* ObjMgrRecordOmdpByEntityID for quick access to top omdp by entity
*
* Revision 6.37  2001/05/31 22:58:25  kans
* added ObjMgrReapOne, DEFAULT_MAXOBJ, autoclean reaps and frees one entity at a time, as needed
*
* Revision 6.36  2001/05/31 22:33:02  kans
* added autoclean and maxobj to ObjMgr structure, ObjMgrAddFunc optionally calls ObjMgrReap and ObjMgrFreeCache to completely clear out least recently accessed objects if currobj >= maxobj
*
* Revision 6.35  2001/05/24 21:54:34  kans
* check for incrementing totobj or currobj above UINT2_MAX, reducing currobj below 0
*
* Revision 6.34  2001/02/16 21:34:49  ostell
* changed GetSecs() to ObjMgrTouchCnt() to reduce system calls
*
* Revision 6.33  2000/11/28 22:58:08  kans
* removed omdp->lockcnt == 0 debugging breakpoint statement
*
* Revision 6.32  2000/11/28 21:43:54  kans
* added ObjMgrReportFunc for debugging
*
* Revision 6.31  2000/10/30 21:26:08  shavirin
* Changes and fixes for some MT-safety related problems.
*
* Revision 6.30  2000/03/27 23:10:23  kans
* initial readlock/lookup/unlock removed from ObjMgrDelete by request of EY
*
* Revision 6.29  2000/03/23 17:45:44  madden
* Use ObjMgrWriteLock instead of ObjMgrReadLock in ObjMgrFreeCache
*
* Revision 6.28  2000/03/20 23:38:40  aleksey
* Finally submitted the changes which have been made by serge bazhin
* and been kept in my local directory.
*
* These changes allow to establish user callback functions
* in 'Asn2ffJobPtr' structure which are called within
* 'SeqEntryToFlatAjp' function call.
* The new members are:
* user_data       - pointer to a user context for passing data
* ajp_count_index - user defined function
* ajp_print_data  - user defined function
* ajp_print_index - user defined function
*
* Revision 6.27  1999/10/20 17:17:37  lewisg
* delete rest of userdata when using OM_OPT_FREE_IF_NO_VIEW in ObjMgrFreeUserDataFunc
*
* Revision 6.26  1999/10/18 20:58:47  lewisg
* add rowID to ObjMgrSendMsgFunc
*
* Revision 6.25  1999/09/27 20:03:14  kans
* added rowID to OMMsgStruct, new ObjMgrSendRowMsg function
*
* Revision 6.24  1999/08/13 19:43:35  kans
* ObjMgrDelete and ObjMgrGetEntityIDForPointer now multithread-safe (EY)
*
* Revision 6.23  1999/08/11 15:17:53  kans
* added ObjMgrFreeByEntityID
*
* Revision 6.22  1999/06/11 16:42:39  kans
* remove from recycle just shrinks stack if removing last element
*
* Revision 6.21  1999/06/10 21:10:28  kans
* added ObjMgrRemoveEntityIDFromRecycle, called from RestoreSeqEntryObjMgrData
*
* Revision 6.20  1999/06/01 21:06:48  ywang
* get rid of (sip1->strand == sip2->strand) in ObjMgrRegionMatch
*
* Revision 6.19  1999/05/17 02:11:23  chappey
* remove the temporary fix
*
* Revision 6.18  1999/05/17 00:41:54  chappey
* temporary fix in ObjMgrRegionComp when SeqLocStop==APPEND_RESIDUE
*
* Revision 6.17  1999/02/19 19:24:34  chappey
* Possible deselection of a region included in a selected region
*
* Revision 6.16  1999/01/22 16:19:51  chappey
* AlsoSelect does not Deselect anymore when selected region of an entityID is already selected. Only one selection remains
*
* Revision 6.15  1999/01/21 19:45:32  chappey
* bug fixed in ObjMgrSelectFunc2, CheckRedondantSelect, ObjMgrRegionComp
*
* Revision 6.14  1998/12/02 01:56:24  kans
* when recycling entityID, check to see if it is already in list, do not add second copy - will see why this is happening later
*
* Revision 6.13  1998/11/30 20:59:01  egorov
* Add ObjMgrNextAvailableEntityID() to avoid overflowing
*
* Revision 6.12  1998/10/07 22:25:43  kans
* reapextra, like freeextra, takes omdp, not omdp->extradata
*
* Revision 6.11  1998/10/07 21:20:14  kans
* ObjMgrAlsoSelect changes (CC)
*
* Revision 6.10  1998/09/28 19:54:10  kans
* made ObjMgrDump debugging function public, no longer conditionally compiled
*
* Revision 6.9  1998/08/18 14:57:02  kans
* protect against nulls in ObjMgrRegionMatch
*
* Revision 6.8  1998/07/01 19:11:45  kans
* added fromProcID, toProcID, OM_MSG_PROCESS, ObjMgrSendProcMsg, time of indexing, ObjMgrGetProcID, moved protFeat and cdsOrRnaFeat to seqmgr structure
*
* Revision 6.7  1998/06/26 23:01:24  kans
* ObjMgrDelete frees extra data
*
* Revision 6.6  1998/06/02 16:53:27  kans
* fixed bug that prevented userdata from being freed, and decrement highest entity if attaching highest to something else
*
* Revision 6.5  1997/12/11 14:37:28  ostell
* corrected a deadlock with DeSelectRegion
*
* Revision 6.4  1997/12/05 16:42:42  ostell
* added objmgrunlock() around "Free if no views"
*
* Revision 6.3  1997/11/22 16:49:57  ostell
* fixed deadlock in ObjMgrReap
*
* Revision 6.2  1997/11/19 22:14:35  ostell
* added support for multithreaded programs
*
* Revision 6.1  1997/09/11 15:55:45  ostell
* Added support for SetColor messages
*
* Revision 6.0  1997/08/25 18:06:43  madden
* Revision changed to 6.0
*
* Revision 5.9  1997/08/01 14:12:19  ostell
* put check for time_t being signed int back into ObjMgrReap for Solaris
*
* Revision 5.8  1997/07/31 16:06:07  kans
* ObjMgrReap lowest time uses UINT4_MAX because of overflow of the number of seconds since 1900
*
* Revision 5.7  1997/07/25 21:31:20  ostell
* free if no view now checks that omudp->messagefunc != NULL instead of checking
* all of them.
*
* Revision 5.6  1997/06/19 18:38:27  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.5  1997/06/02 17:04:09  kans
* added ObjMgrProcLoadEx
*
* Revision 5.4  1997/04/29 16:11:30  kans
* send deselect message AFTER removing item from list of selected items
*
 * Revision 5.3  1997/01/22  05:29:46  ostell
 * made objmgrget static
 * made seqmgrget static
 *
 * Revision 5.2  1996/07/30  21:18:15  chappey
 * don't return FALSE in ObjMgrSetTempLoad when index == 0
 *
 * Revision 5.1  1996/06/26  20:07:48  chappey
 * ObjMgrRegionMatch now returs TRUE when the regions are identical
 *
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 4.14  1996/05/22  21:46:59  kans
 * don't return on OBJ_MSG_RET_DONE in ObjMgrSendStructMsgFunc
 *
 * Revision 4.13  1996/03/05  19:15:01  kans
 * fixed bug in ObjMgrSetOptions and ObjMgrClearOptions
 *
 * Revision 4.12  1996/02/28  04:53:06  ostell
 * added ObjMgrHold suport
 *
 * Revision 4.10  1996/01/22  13:28:11  kans
 * cast first parameter of MemSet to (Pointer) for SunOS compiler
 *
 * Revision 4.9  1996/01/05  14:39:12  ostell
 * fix for control chars in comment
 *
 * Revision 4.7  1995/12/22  20:12:01  ostell
 * added lock in ObjMgrTempLoad to protect new record from ObjMgrReap
 *
 * Revision 4.6  1995/12/22  14:45:45  ostell
 * added do_not_reload_from_cache to OmProcControl so that DataGet
 * functions from gather can control this behvior
 * added protection from recursion in ObjMgrDelete
 *
 * Revision 4.5  1995/12/15  02:47:01  ostell
 * fix to ObjMgrReap so highest time_t is INT4_MAX not UINT4_MAX to compensate
 * for buggy include files from vendors
 *
 * Revision 4.4  1995/10/03  15:50:37  ostell
 * added support for selection by region.. now fully implemented
 *
 * Revision 4.3  1995/10/02  14:36:55  ostell
 * fixed ObjMgrFreeUserData to get options from omp or omdp
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
 * Revision 1.25  1995/07/20  18:30:32  kans
 * changed SeqIdPrint to SeqIdWrite
 *
 * Revision 1.24  1995/07/08  15:21:01  ostell
 * Added ObjMgrSetOptions() and related functions
 * Added support for delete of entity when no more user data on it
 * Added support for DirtyFlag
 *
 * Revision 1.23  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

/** for ErrPostEx() ****/

static char *this_module = "ncbiapi";
#define THIS_MODULE this_module
static char *this_file = __FILE__;
#define THIS_FILE this_file

/**********************/

#include <objmgr.h>		   /* the interface */
#include <objsset.h>       /* temporary for caching functions */
#include <ncbithr.h>       /* for thread safe functions */
#include <sequtil.h>
#include <explore.h>       /* for SeqMgrClearFeatureIndexes */

/***
#define DEBUG_OBJMGR
***/


/*****************************************************************************
*
*   Data Object local functions
*
*****************************************************************************/
static Uint4 NEAR ObjMgrTouchCnt PROTO((void));
static Boolean NEAR ObjMgrExtend PROTO((ObjMgrPtr omp));
static ObjMgrDataPtr NEAR ObjMgrFindByEntityID PROTO((ObjMgrPtr omp, Uint2 entityID, ObjMgrDataPtr PNTR prev));
static Boolean NEAR ObjMgrFreeClipBoardFunc PROTO((ObjMgrPtr omp));
static Boolean NEAR ObjMgrFreeCacheFunc PROTO((ObjMgrPtr omp, Uint2 type, Uint2Ptr rettype, VoidPtr PNTR retval));
static Boolean NEAR ObjMgrAddFunc PROTO((ObjMgrPtr omp, Uint2 type, Pointer data));
static Boolean NEAR ObjMgrFreeUserDataFunc PROTO((ObjMgrPtr omp, Uint2 entityID,
								Uint2 procid, Uint2 proctype, Uint2 userkey));
static Int4 NEAR ObjMgrLockFunc (ObjMgrPtr omp, Uint2 type, Pointer data, Boolean lockit);
/*****************************************************************************
*
*   Procedure Management local Functions
*
*****************************************************************************/
static Boolean NEAR ObjMgrProcExtend PROTO((ObjMgrPtr omp));

/*****************************************************************************
*
*   Type Management Local functions
*
*****************************************************************************/
static Boolean NEAR ObjMgrTypeExtend PROTO((ObjMgrPtr omp));

/*****************************************************************************
*
*   Messaging/Selection local functions
*
*****************************************************************************/
static Boolean NEAR ObjMgrSendMsgFunc PROTO((ObjMgrPtr omp, ObjMgrDataPtr omdp,
				Int2 msg, Uint2 entityID, Uint2 itemID,	Uint2 itemtype, Uint2 rowID,
				Uint2 fromProcID, Uint2 toProcID, Pointer procmsgdata));
static Boolean NEAR ObjMgrSendStructMsgFunc PROTO((ObjMgrPtr omp, ObjMgrDataPtr omdp,
				OMMsgStructPtr ommsp));
static SelStructPtr NEAR ObjMgrAddSelStruct PROTO((ObjMgrPtr omp,
	 Uint2 entityID, Uint2 itemID, Uint2 itemtype, Uint1 regiontype, Pointer region));
static Boolean NEAR ObjMgrSelectFunc PROTO((ObjMgrPtr omp, Uint2 entityID, Uint2 itemID, Uint2 itemtype,
									Uint1 regiontype, Pointer region));
static Boolean NEAR ObjMgrDeSelectFunc PROTO((ObjMgrPtr omp, Uint2 entityID, Uint2 itemID, Uint2 itemtype,
									Uint1 regiontype, Pointer region));
static Boolean NEAR ObjMgrDeSelectStrucFunc PROTO((ObjMgrPtr omp, SelStructPtr ssp));
static Boolean NEAR ObjMgrDeSelectAllFunc PROTO((ObjMgrPtr omp));
static Boolean NEAR ObjMgrRegionMatch PROTO((Uint1 regiontype1, Pointer region1,
						Uint1 regiontype2, Pointer region2));
static Pointer NEAR ObjMgrRegionCopy PROTO((ObjMgrPtr omp, Uint1 regiontype, Pointer region));
static Pointer NEAR ObjMgrRegionFree PROTO((ObjMgrPtr omp, Uint1 regiontype, Pointer region));
static Pointer NEAR ObjMgrMemCopyFunc PROTO((ObjMgrPtr omp, Uint2 type, Pointer ptr, Boolean unlock));
static Pointer NEAR ObjMgrFreeFunc PROTO((ObjMgrPtr omp, Uint2 type, Pointer ptr, Boolean unlock));

/*****************************************************************************
*
*   ObjMgr Functions
*
*****************************************************************************/
static TNlmMutex omctr_mutex = NULL;
static Uint4 NEAR ObjMgrTouchCnt (void)
{
	static Uint4 ObjMgrTouchCtr = 1; /* start at 1 to avoid rollover signal */
	Int4 ret, imax, i;
	ObjMgrPtr omp;
	ObjMgrDataPtr PNTR omdpp;
	Uint4 tmpctr;

	if (! ObjMgrTouchCtr)  /* rolled over */
	{
		ret = NlmMutexLockEx(&omctr_mutex); /* protect this section */
		if (ret)  /* error */
		{
			ErrPostEx(SEV_FATAL,0,0,"ObjMgrTouchCnt failed [%ld]", (long)ret);
			return ObjMgrTouchCtr;
		}
		
		/**** reset the touch values *****/
		/** all calls to this function should already have ObjMgrWriteLock **/

		if (! ObjMgrTouchCtr) /* check that this was not a stalled thread */
		{
			tmpctr = 0;
			omp = ObjMgrGet();
			imax = omp->currobj;
			omdpp = omp->datalist;
			for (i = 0; i < imax; i++)
			{
				if (omdpp[i]->parentptr == NULL)
					omdpp[i]->touch = ++tmpctr;
			}
			ObjMgrTouchCtr = tmpctr;
		}
		NlmMutexUnlock(omctr_mutex);
	}

	return ++ObjMgrTouchCtr;

}

NLM_EXTERN ObjMgrDataPtr LIBCALL ObjMgrGetData (Uint2 entityID)
{
	ObjMgrDataPtr prev, retval=NULL;
	ObjMgrPtr omp;

	omp = ObjMgrReadLock();
	retval = ObjMgrFindByEntityID (omp, entityID, &prev);
	ObjMgrUnlock();
	return retval;
}

NLM_EXTERN ObjMgrDataPtr LIBCALL ObjMgrGetDataStruct (ObjMgrPtr omp, Uint2 entityID)
{
	ObjMgrDataPtr prev;

	return ObjMgrFindByEntityID (omp, entityID, &prev);
}

/* cache to avoid linear search of all objects in ObjMgrFindByEntityID */

#define TOP_ID_STACK_SIZE  100

static Uint2 topEntityIDs [TOP_ID_STACK_SIZE];
static ObjMgrDataPtr topOMDPs [TOP_ID_STACK_SIZE];
static Int2  topIDStackPt = 0;

extern void ObjMgrRecordOmdpByEntityID (Uint2 entityID, ObjMgrDataPtr omdp);
extern void ObjMgrRecordOmdpByEntityID (Uint2 entityID, ObjMgrDataPtr omdp)

{
	Int2  i;

	if (entityID < 1) return;

	/* check for preexisting entry, update */

	for (i = 0; i < topIDStackPt; i++) {
		if (entityID == topEntityIDs [i]) {
			topOMDPs [i] = omdp; /* record in stack */
			if (omdp == NULL) { /* remove if null */
				if (topIDStackPt > i + 1) {
					topIDStackPt--;
					topEntityIDs [i] = topEntityIDs [topIDStackPt];
					topOMDPs [i] = topOMDPs [topIDStackPt];
				} else {
					topIDStackPt--;
				}
			}
			return;
		}
	}

	/* add if not found */

	if (omdp == NULL) return;
	if (topIDStackPt < TOP_ID_STACK_SIZE) {
		topEntityIDs [topIDStackPt] = entityID;
		topOMDPs [topIDStackPt] = omdp;
		topIDStackPt++;
	}
}

static ObjMgrDataPtr NEAR ObjMgrFindByEntityID (ObjMgrPtr omp, Uint2 entityID, ObjMgrDataPtr PNTR prev)
{
	ObjMgrDataPtr omdp, prevptr=NULL;
	ObjMgrDataPtr PNTR omdpp;
	Int4 i, imax;

	/* check cache first to avoid linear search through all objects */

	for (i = 0; i < topIDStackPt; i++) {
		if (entityID == topEntityIDs [i]) {
			omdp = topOMDPs [i];
			if (omdp != NULL && omdp->parentptr == NULL && omdp->EntityID == entityID) {
				if (prev != NULL)
					*prev = prevptr;
				return omdp;
			}
		}
	}

	imax = omp->currobj;
	omdpp = omp->datalist;
	for (i = 0; i < imax; i++)
	{
		omdp = omdpp[i];    /* emptys always at end */
		if (omdp->parentptr == NULL)
		{
			if (omdp->EntityID == entityID)
			{
				if (prev != NULL)
					*prev = prevptr;
				return omdp;
			}
		}
	}
	return NULL;
}

NLM_EXTERN ObjMgrDataPtr LIBCALL ObjMgrFindByData (ObjMgrPtr omp, Pointer ptr)
{
	ObjMgrDataPtr omdp;
	ObjMgrDataPtr PNTR omdpp;
	Int4 i, imax;
	
	if (ptr == NULL) return NULL;

	imax = omp->currobj;
	omdpp = omp->datalist;

	i = ObjMgrLookup(omp, ptr);   /* find by binary search on dataptr? */
	if (i >= 0)
		return omdpp[i];          /* found it */

	for (i = 0; i < imax; i++)
	{
		omdp = omdpp[i];    /* emptys always at end */
		if ((Pointer)(omdp->choice) == ptr)
		{
			return omdp;
		}
	}
	return NULL;
}

#define ENTITY_ID_STACK_SIZE  100

static Uint2 recycledEntityIDs [ENTITY_ID_STACK_SIZE];
static Int4  recycledIDStackPt = 0;

extern void ObjMgrRemoveEntityIDFromRecycle (Uint2 entityID, ObjMgrPtr omp);
extern void ObjMgrRemoveEntityIDFromRecycle (Uint2 entityID, ObjMgrPtr omp)

{
	Int4  i;

	if (entityID < 1) return;
	if (omp != NULL) {
		for (i = 0; i < recycledIDStackPt; i++) {
			if (entityID == recycledEntityIDs [i]) {
				recycledEntityIDs [i] = 0; /* remove from recycle list */
				if (recycledIDStackPt > i + 1) {
					recycledIDStackPt--;
					recycledEntityIDs [i] = recycledEntityIDs [recycledIDStackPt];
				} else {
					recycledIDStackPt--;
				}
			}
		}
	}
}

static Uint2 ObjMgrNextAvailEntityID (ObjMgrPtr omp)

{
	Uint2      entityID = 0;
	if (omp != NULL) {
		if (recycledIDStackPt > 0) {
			recycledIDStackPt--;
			entityID = recycledEntityIDs [recycledIDStackPt];
		} else {
			entityID = ++(omp->HighestEntityID);
		}
	}
	return entityID;
}

static void ObjMgrRecycleEntityID (Uint2 entityID, ObjMgrPtr omp)

{

	Int4  i;

	if (entityID < 1) return;
	if (omp != NULL) {
		/* check to see if entity is already on stack (e.g., entity 1), abort if so */
		for (i = 0; i < recycledIDStackPt; i++) {
			if (entityID == recycledEntityIDs [i]) return;
		}
		if (recycledIDStackPt < ENTITY_ID_STACK_SIZE) {
			recycledEntityIDs [recycledIDStackPt] = entityID;
			recycledIDStackPt++;
		}
	}
}

NLM_EXTERN Uint2 LIBCALL ObjMgrAddEntityID (ObjMgrPtr omp, ObjMgrDataPtr omdp)
{
	if (omdp == NULL) return 0;

	if (omdp->EntityID)
	   return omdp->EntityID;

	/* omdp->EntityID = ++(omp->HighestEntityID); */
	omdp->EntityID = ObjMgrNextAvailEntityID (omp);
	ObjMgrRecordOmdpByEntityID (omdp->EntityID, omdp);

#ifdef DEBUG_OBJMGR
	ObjMgrDump(NULL, "ObjMgrAddEntityID-A");
#endif

	ObjMgrSendMsgFunc(omp, omdp, OM_MSG_CREATE, omdp->EntityID, 0, 0, 0, 0, 0, NULL);

#ifdef DEBUG_OBJMGR
	ObjMgrDump(NULL, "ObjMgrAddEntityID-B");
#endif

	return omdp->EntityID;
}
static Boolean NEAR ObjMgrSendMsgFunc (ObjMgrPtr omp, ObjMgrDataPtr omdp,
				Int2 msg, Uint2 entityID, Uint2 itemID,	Uint2 itemtype, Uint2 rowID,
				Uint2 fromProcID, Uint2 toProcID, Pointer procmsgdata)
{
	OMMsgStruct omms;

	MemSet((Pointer)(&omms), 0, sizeof(OMMsgStruct));

	omms.message = msg;
	omms.entityID = entityID;
	omms.itemID = itemID;
	omms.itemtype = itemtype;
	omms.fromProcID = fromProcID;
	omms.toProcID = toProcID;
	omms.procmsgdata = procmsgdata;
    omms.rowID = rowID;
	
	return ObjMgrSendStructMsgFunc (omp, omdp, &omms);
}

/*********************************************************************
*
*   ObjMgrSendStructMsgFunc()
*   This function actually sends the messages to registered proceedures.
*   It has to unlock the objmgr so the proceedures can read or write the
*   objmgr. This is risky, but I am not sure how to do this safer. It does
*   keep an array of messages to send before it unlocks anything, in case
*   the message list changes from a callback. This imposes a fixed limit to 
*   the number of messages that can be sent at once to MAXMSG. This may become
*   a problem.
*
*********************************************************************/
static Boolean NEAR ObjMgrSendStructMsgFunc (ObjMgrPtr omp, ObjMgrDataPtr omdp,
				OMMsgStructPtr ommsp)
{
#define MAXMSG 250  /* max number of messages to save */
	OMUserDataPtr prev=NULL, curr, next, PNTR root, msgs[MAXMSG], omudp;
	Int2 retval;
	Int2 ctr, nummsg, i;
	Boolean is_write_locked, too_many, done;

	is_write_locked = omp->is_write_locked;   /* remember state */
	nummsg = 0;      /* no messages known yet */
	  
	                  /*****************************************
					  *  First find all messages to send while locked
					  *****************************************/
	curr = omp->userdata;
	root = &(omp->userdata);
	ctr = 2;
	too_many = FALSE;
	while ((ctr) && (! too_many))
	{
		prev = NULL;
		while ((curr != NULL) && (! too_many))
		{
			next = curr->next;
			if (curr->messagefunc != NULL)
			{
				if (ommsp->toProcID == 0 || ommsp->toProcID == curr->procid) {
					if (nummsg < MAXMSG)
					{
						msgs[nummsg] = curr;
						nummsg++;
					}
					else
						too_many = TRUE;
				}
			}
			curr = next;
		}

		if (omdp == NULL)
			ctr -= 2;
		else if (ctr == 2)
		{
			ctr--;
			curr = omdp->userdata;
			root = &(omdp->userdata);
		}
		else
			ctr--;
	}

	if (too_many)
	{
		ErrPostEx(SEV_ERROR,0,0,"ObjMgrSendMessage: more than %d messages",
			(int)nummsg);
	}



                      /*****************************************
					  *  now send the messages confirming that each
					  *    still exists
					  *****************************************/

	for (i = 0; i < nummsg; i++)
	{
		omudp = msgs[i];
		done = FALSE;
	curr = omp->userdata;
	root = &(omp->userdata);
	ctr = 2;
	while ((ctr) && (! done))
	{
		prev = NULL;
		while ((curr != NULL) && (! done))
		{
			next = curr->next;
			if (curr == omudp)
			{
			  done = TRUE;   /* found this one */
			  if (curr->messagefunc != NULL)
			  {
				ommsp->omuserdata = (Pointer)curr;

				ObjMgrUnlock();   /* have to allow message to react */
				retval = (* (curr->messagefunc))(ommsp);
				if (is_write_locked)
					ObjMgrWriteLock();
				else
					ObjMgrReadLock();

				if (retval == OM_MSG_RET_ERROR)
					ErrShow();
				else if (retval == OM_MSG_RET_DEL)
				{
					if (prev == NULL)
						*root = next;
					else
						prev->next = next;
					if (curr->freefunc != NULL)
						(* (curr->freefunc))(curr->userdata.ptrvalue);
					MemFree(curr);
				}
			  }
			}
			curr = next;
		}

		if (omdp == NULL)
			ctr -= 2;
		else if (ctr == 2)
		{
			ctr--;
			curr = omdp->userdata;
			root = &(omdp->userdata);
		}
		else
			ctr--;
	  }
	}
	return TRUE;

}
/*****************************************************************************
*
*   Return the current ObjMgr
*   	ObjMgrGet is obsolete
*       ObjMgrReadLock, ReadUnlock, WriteLock, WriteUnlock are thread safe
*
*****************************************************************************/
static TNlmMutex omp_mutex = NULL;
static ObjMgrPtr global_omp = NULL;
static TNlmRWlock omp_RWlock = NULL;

/*****************************************************************************
*
*   Return the current ObjMgr
*   	Initialize if not done already
*       This function will become obsolete
*
*****************************************************************************/
NLM_EXTERN ObjMgrPtr LIBCALL ObjMgrGet (void)
{
	Int4 ret;
	ObjMgrPtr omp;

	if (global_omp != NULL)
		return global_omp;

	ret = NlmMutexLockEx(&omp_mutex);  /* protect this section */
	if (ret)  /* error */
	{
		ErrPostEx(SEV_FATAL,0,0,"ObjMgrGet failed [%ld]", (long)ret);
		return NULL;
	}

	if (global_omp == NULL)  /* check again after mutex */
	{
	                             /*** have to initialize it **/
		omp = (ObjMgrPtr) MemNew (sizeof(ObjMgr));
		omp->maxtemp = DEFAULT_MAXTEMP;
		omp->maxobj = DEFAULT_MAXOBJ;
		omp_RWlock = NlmRWinit();  /* initialize RW lock */
		global_omp = omp;       /* do this last for mutex safety */
	}

	NlmMutexUnlock(omp_mutex);

	return global_omp;
}

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
NLM_EXTERN ObjMgrPtr LIBCALL ObjMgrReadLock (void)
{
	ObjMgrPtr omp;
	Int4 ret;

	omp = ObjMgrGet();  /* ensure initialization */

        ret = NlmRWrdlock(omp_RWlock); 
	/* ret = NlmRWwrlock(omp_RWlock); */

	if (ret != 0)
	{
		ErrPostEx(SEV_ERROR,0,0,"ObjMgrReadLock: RWrdlock error [%ld]",
			(long)ret);
		return NULL;
	}

	omp->is_write_locked = TRUE;

	return omp;
}

/*****************************************************************************
*
*   ObjMgrWriteLock
*   	Initialize if not done already
*       A thread can have only one read or write lock at a time
*       Many threads can have read locks
*       Only one thread can have a write lock
*       No other threads may have read locks if a write lock is granted
*       If another thread holds a read or write lock, this call blocks until write
*          is unlocked.
*
*****************************************************************************/
NLM_EXTERN ObjMgrPtr LIBCALL ObjMgrWriteLock (void)
{
	ObjMgrPtr omp;
	Int4 ret;

	omp = ObjMgrGet();  /* ensure initialization */

	ret = NlmRWwrlock(omp_RWlock);
	if (ret != 0)
	{
		ErrPostEx(SEV_ERROR,0,0,"ObjMgrWriteLock: RWwrlock error [%ld]",
			(long)ret);
		return NULL;
	}
	omp->is_write_locked = TRUE;
	return omp;
}


/*****************************************************************************
*
*  ObjMgrUnlock()
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrUnlock (void)
{
	ObjMgrPtr omp;
	Int4 ret;

	omp = ObjMgrGet();  /* ensure initialization */

	ret = NlmRWunlock(omp_RWlock);
	if (ret != 0)
	{
		ErrPostEx(SEV_ERROR,0,0,"ObjMgrUnlock: RWunlock error [%ld]",
			(long)ret);
		return FALSE;
	}
	omp->is_write_locked = FALSE;  /* can't be write locked */
	return TRUE;
}




/*****************************************************************************
*
*   ObjMgrSetOptions(Uint2 option, Uint2 entityID)
*     Sets options on the ObjMgr. All Options are 1 bit flags set with
*       defines. Several Options may be ORed together.
*       returns current options.
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrSetOptions (Uint2 option, Uint2 entityID)
{
	ObjMgrPtr omp;
	Uint2Ptr optionptr;
	ObjMgrDataPtr omdp, prev;

	omp = ObjMgrGet();
	if (entityID == 0)
		optionptr = &(omp->options);
	else
	{
		omdp = ObjMgrFindByEntityID(omp, entityID, &prev);
		if (omdp == NULL)
			return 0;
		optionptr = &(omdp->options);
	} 
	*optionptr |= (option & 0xFFFF);
	return *optionptr;
}

/*****************************************************************************
*
*   ObjMgrClearOptions(Uint2 option, Uint2 entityID)
*     Clears options on the ObjMgr. All Options are 1 bit flags set with
*       defines. Several Options may be ORed together.
*       returns current options.
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrClearOptions (Uint2 option, Uint2 entityID)
{
	ObjMgrPtr omp;
 	Uint2Ptr optionptr;
	ObjMgrDataPtr omdp, prev;


	omp = ObjMgrGet();
	if (entityID == 0)
		optionptr = &(omp->options);
	else
	{
		omdp = ObjMgrFindByEntityID(omp, entityID, &prev);
		if (omdp == NULL)
			return 0;
		optionptr = &(omdp->options);
	} 
	*optionptr &= ~(option & 0xFFFF);
	return *optionptr;
}

/*****************************************************************************
*
*   ObjMgrGetOptions(entityID)
*     Returns options on the ObjMgr. All Options are 1 bit flags set with
*       defines. Several Options may be ORed together.
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrGetOptions (Uint2 entityID)
{
	ObjMgrPtr omp;
	ObjMgrDataPtr omdp, prev;
	Uint2 retval = 0;

	omp = ObjMgrReadLock();
	if (entityID == 0)
		 retval = omp->options;
	else
	{
		omdp = ObjMgrFindByEntityID(omp, entityID, &prev);
		if (omdp != NULL)
			retval = omdp->options;
	}
	ObjMgrUnlock();
	return retval; 
}

/*****************************************************************************
*
*   ObjMgrTestOptions(Uint2 option)
*     Tests options on the ObjMgr. All Options are 1 bit flags set with
*       defines. Several Options may be ORed together.
*       returns TRUE if ANY flags in "option" are set in the ObjMgr.
*       Normally one would supply only 1 option to test. If many were to
*       be tested, use ObjMgrGetOptions()
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrTestOptions (Uint2 option, Uint2 entityID)
{
	ObjMgrPtr omp;
	Uint2Ptr optionptr;
	ObjMgrDataPtr omdp, prev;
	Boolean retval = FALSE;

	omp = ObjMgrReadLock();
	if (entityID == 0)
		optionptr = &(omp->options);
	else
	{
		omdp = ObjMgrFindByEntityID(omp, entityID, &prev);
		if (omdp == NULL)
			goto erret;
		optionptr = &(omdp->options);
	}
	
	if (*optionptr & (option & 0xFFFF))
		retval = TRUE;
erret:

	ObjMgrUnlock();

	return retval;
}

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
NLM_EXTERN Uint2 LIBCALL ObjMgrSetHold (void)
{
	ObjMgrPtr omp;

	omp = ObjMgrWriteLock();

	if (omp->hold < UINT2_MAX)
		omp->hold++;
	else
		ErrPostEx(SEV_ERROR,0,0, "ObjMgrSetHold: hold > UINT2_MAX");
	ObjMgrUnlock();
	return omp->hold;
}

/*****************************************************************************
*
*   ObjMgrClearHold()
*     Decrements hold count in ObjMgr
*       returns current hold count
*       will never decrement below 0
*     if hold == 0, then tempload records will be cached out of memory
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrClearHold (void)
{
	ObjMgrPtr omp;

	omp = ObjMgrWriteLock();

	if (omp->hold)
	{
		omp->hold--;
		if (! (omp->hold))  /* down to 0 */
			ObjMgrReap(omp);
	}
	else
		ErrPostEx(SEV_ERROR,0,0, "ObjMgrClearHold: hold = 0");
	ObjMgrUnlock();
	return omp->hold;
}

/*****************************************************************************
*
*   ObjMgrCheckHold()
*       returns current hold count
*     if hold == 0, then tempload records will be cached out of memory
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrCheckHold (void)
{
	ObjMgrPtr omp;

	omp = ObjMgrReadLock();
	ObjMgrUnlock();
	return omp->hold;
}



/*****************************************************************************
*
*   Data Management Functions
*
*****************************************************************************/


/*****************************************************************************
*
*   ObjMgrExtend(omp)
*
*****************************************************************************/
static Boolean NEAR ObjMgrExtend (ObjMgrPtr omp)
{
	Boolean result = FALSE;
	OMDataPtr omdp, prev=NULL;
	ObjMgrDataPtr PNTR tmp;
	Int4 i, j;

	for (omdp = omp->ncbidata; omdp != NULL; omdp = omdp->next)
		prev = omdp;

	omdp = (OMDataPtr)MemNew(sizeof(OMData));
	if (omdp == NULL) return result;
	tmp = (ObjMgrDataPtr PNTR)MemNew((size_t)(sizeof(ObjMgrDataPtr) * (omp->totobj + NUM_OMD)));
	if (tmp == NULL)
	{
		MemFree(omdp);
		return result;
	}

	if (prev != NULL)
	{
		prev->next = omdp;
		MemMove(tmp, omp->datalist, (size_t)(sizeof(ObjMgrDataPtr) * omp->totobj));
		MemFree(omp->datalist);
	}
	else
		omp->ncbidata = omdp;

	j = omp->totobj;

	for (i = 0; i < NUM_OMD; i++, j++)
		tmp[j] = &(omdp->data[i]);

	if (omp->totobj < UINT2_MAX - NUM_OMD)
		omp->totobj += NUM_OMD;
	else
		ErrPostEx(SEV_ERROR, 0,0, "ObjMgrExtend: incrementing totobj above UINT2_MAX");
	omp->datalist = tmp;

	result = TRUE;
	return result;
}
/*****************************************************************************
*
*   ObjMgrAdd(type, data)
*   	adds a pointer (data) of type (type) to the sequence manager
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrAdd (Uint2 type, Pointer data)
{
	ObjMgrPtr omp;
	Boolean retval = FALSE;

	omp = ObjMgrWriteLock();
	retval = ObjMgrAddFunc(omp, type, data);
	ObjMgrUnlock();
	return retval;
}


/*****************************************************************************
*
*   ObjMgrAddFunc(omp, type, data)
*   	adds a pointer (data) of type (type) to the sequence manager
*
*****************************************************************************/
static Boolean NEAR ObjMgrAddFunc (ObjMgrPtr omp, Uint2 type, Pointer data)
{
	ObjMgrDataPtr omdp;
	ObjMgrDataPtr PNTR omdpp;
	Int4 i, imin, imax;
	Boolean retval = FALSE;
	unsigned long tmp, datai;
	BioseqPtr bsp;
#ifdef DEBUG_OBJMGR
	FILE * fp;

	fp = FileOpen("ObjMgr.log", "a");
#endif

	/* if autoclean is set and above maxobj, remove least recently accessed objects */
	if (omp->autoclean && omp->currobj >= omp->maxobj) {
		ObjMgrReapOne (omp);
		ObjMgrFreeCache (0);
	}

	if (omp->currobj >= omp->totobj)
	{
		if (! ObjMgrExtend(omp))
			goto erret;
	}

	i = omp->currobj;
	omdpp = omp->datalist;
	omdp = omdpp[i];    /* emptys always at end */
	omdp->options = omp->options;   /* set default options */

	imin = 0;                   /* find where it goes */
	imax = omp->currobj-1;

	datai = (unsigned long)data;

	if ((i) && (datai < (unsigned long)(omdpp[imax]->dataptr)))
	{
		i = (imax + imin) / 2;
		while (imax > imin)
		{
			tmp = (unsigned long)(omdpp[i]->dataptr);
#ifdef DEBUG_OBJMGR
			fprintf(fp, "Sort: i=%d tmp=%ld data=%ld imax=%d imin=%d\n",
				(int)i, (long)tmp, (long)data, (int)imax, (int)imin);
#endif
			if (tmp > datai)
				imax = i - 1;
			else if (tmp < datai)
				imin = i + 1;
			else
				break;
			i = (imax + imin)/2;
		}

#ifdef DEBUG_OBJMGR
			fprintf(fp, "End: i=%d tmp=%ld data=%ld imax=%d imin=%d\n",
				(int)i, (long)tmp, (long)data, (int)imax, (int)imin);
#endif
		if (datai > (unsigned long)(omdpp[i]->dataptr)) /* check for off by 1 */
		{
			i++;
#ifdef DEBUG_OBJMGR
			fprintf(fp, "Adjust: i=%d\n", (int)i);
#endif
		}


		imax = omp->currobj - 1;	 /* open the array */
		while (imax >= i)
		{
			omdpp[imax+1] = omdpp[imax];
			imax--;
		}
	}

	omdpp[i] = omdp;    /* put in the pointer in order */
	if (omp->currobj < UINT2_MAX)
		omp->currobj++;     /* got one more */
	else
		ErrPostEx(SEV_ERROR, 0,0, "ObjMgrAddFunc: incrementing currobj above UINT2_MAX");

	omdp->dataptr = data;  /* fill in the values */
	omdp->datatype = type;
	omdp->touch = ObjMgrTouchCnt();   /* stamp with time */

	if (type == OBJ_BIOSEQ) {
		bsp = (BioseqPtr) data;
		if (bsp != NULL) {
			/* used for feature indexing, rapid delete from SeqID index */
			bsp->omdp = (Pointer) omdp;
		}
	}

#ifdef DEBUG_OBJMGR
	FileClose(fp);
	ObjMgrDump(NULL, "ObjMgrAdd");
#endif
	retval = TRUE;
erret: 
	return retval;
}

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
NLM_EXTERN Uint2 LIBCALL ObjMgrRegister (Uint2 datatype, Pointer data)
{
	Uint2 dtype, retval = 0;
	ObjMgrPtr omp;
	ObjMgrDataPtr omdp;
	ObjMgrTypePtr omtp;
	Pointer ptr;


	if (data == NULL) return retval;

	dtype = datatype;  /* sets the default state */
	ptr = data;

	omp = ObjMgrWriteLock();

	omdp = ObjMgrFindByData(omp, data);  /* already have it? */
	if (omdp != NULL)
	{
		if (! omdp->EntityID)
		{
			if (omdp->parentptr != NULL)
				ErrPostEx(SEV_ERROR,0,0, "ObjMgrRegister: parent != NULL");

			ObjMgrAddEntityID(omp, omdp);
		}

		retval = omdp->EntityID;
		goto erret;
	}
								/* have to add it to ObjMgr */
	omtp = ObjMgrTypeFind(omp, datatype, NULL, NULL);
	if (omtp == NULL)
	{
		ErrPostEx(SEV_ERROR,0,0, "ObjMgrRegister: invalid data type [%d]", (int)datatype);
		goto erret;
	}
      
      /*** this was to register things like SeqEntry.. currently we register data.ptrvalue directly **
      
	if (omtp->fromchoicefunc == NULL)
	{
		dtype = datatype;
		ptr = data;
	}
	else
	{
		vnp = (ValNodePtr)data;
		dtype = (*(omtp->fromchoicefunc))(vnp);
		ptr = vnp->data.ptrvalue;
	}

      ***********************************************************************************************/
      
	ObjMgrAddFunc(omp, dtype, ptr);
	omdp = ObjMgrFindByData(omp, ptr);
	if (omdp == NULL) goto erret;

	ObjMgrAddEntityID(omp, omdp);
	retval = omdp->EntityID;
erret:
	ObjMgrUnlock();

	return retval;
}


/*****************************************************************************
*
*   ObjMgrAddUserData(entityID, procid, proctype, userkey)
*   	creates a new OMUserData struct attached to entityID
*   	if entityID = 0, attaches to the desktop (all objects)
*   	Caller must fill in returned structure
*   	returns NULL on failure
*
*****************************************************************************/
NLM_EXTERN OMUserDataPtr LIBCALL ObjMgrAddUserData (Uint2 entityID, Uint2 procid, Uint2 proctype, Uint2 userkey)
{
	OMUserDataPtr omudp=NULL, tmp;
	ObjMgrPtr omp;
	ObjMgrDataPtr omdp;

	omp = ObjMgrWriteLock();

	omudp = MemNew(sizeof(OMUserData));
	omudp->procid = procid;
	omudp->proctype = proctype;
	omudp->userkey = userkey;

	if (entityID == 0)
	{
		if (omp->userdata == NULL)
			omp->userdata = omudp;
		else
		{
			for (tmp = omp->userdata; tmp->next != NULL; tmp = tmp->next)
				continue;
			tmp->next = omudp;
		}
	}
	else
	{
		omdp = ObjMgrFindByEntityID(omp, entityID, NULL);
		if (omdp == NULL)
			omudp = MemFree(omudp);
		else
		{
			if (omdp->userdata == NULL)
				omdp->userdata = omudp;
			else
			{
				for (tmp = omdp->userdata; tmp->next != NULL; tmp = tmp->next)
					continue;
				tmp->next = omudp;

			}
		}
	}

	ObjMgrUnlock();
	return omudp;
}

/*****************************************************************************
*
*   ObjMgrGetUserData(entityID, procid, proctype, userkey)
*   	Finds an OMUserData struct attached to entityID
*   	if entityID = 0, gets from the desktop
*   	returns NULL on failure
*
*****************************************************************************/
NLM_EXTERN OMUserDataPtr LIBCALL ObjMgrGetUserData (Uint2 entityID, Uint2 procid, Uint2 proctype, Uint2 userkey)
{
	OMUserDataPtr omudp=NULL;
	ObjMgrPtr omp;
	ObjMgrDataPtr omdp;

	omp = ObjMgrReadLock();

	if (entityID == 0)
	{
		omudp = omp->userdata;
	}
	else
	{
		omdp = ObjMgrFindByEntityID(omp, entityID, NULL);
		if (omdp != NULL)
			omudp = omdp->userdata;
	}

	while (omudp != NULL)
	{
		if ((omudp->procid == procid) || (! procid))
		{
			if ((omudp->proctype == proctype) || (! proctype))
			{
				if ((omudp->userkey == userkey) || (! userkey))
					break;
			}
		}
		omudp = omudp->next;
	}

	ObjMgrUnlock();
	return omudp;
}
/*****************************************************************************
*
*   ObjMgrFreeUserData(entityID, procid, proctype, userkey)
*   	frees OMUserData attached to entityID by procid
*       if procid ==0, frees all OMUserData of proctype
*   	if proctype ==0, matches any proctype
*       if userkey == matches any userkey
*   	returns TRUE if any freed
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrFreeUserData (Uint2 entityID, Uint2 procid, Uint2 proctype, Uint2 userkey)
{
	ObjMgrPtr omp;
	Boolean retval = FALSE;

	omp = ObjMgrWriteLock();
	retval = ObjMgrFreeUserDataFunc(omp, entityID, procid, proctype, userkey);
	ObjMgrUnlock();
	return retval;
}

/*****************************************************************************
*
*   ObjMgrFreeUserDataFunc(omp, entityID, procid, proctype, userkey)
*   	frees OMUserData attached to entityID by procid
*       if procid ==0, frees all OMUserData of proctype
*   	if proctype ==0, matches any proctype
*       if userkey == matches any userkey
*   	returns TRUE if any freed
*
*****************************************************************************/
static Boolean NEAR ObjMgrFreeUserDataFunc (ObjMgrPtr omp, Uint2 entityID,
								Uint2 procid, Uint2 proctype, Uint2 userkey)
{
	OMUserDataPtr omudp=NULL, prev, next;
	ObjMgrDataPtr omdp;
	Boolean got_one = FALSE, view_left;
	ObjMgrTypePtr omtp;
	Uint2 type, options;
	Pointer ptr;
	Boolean is_write_locked;

	options = omp->options;

	if (entityID == 0)
	{
		omudp = omp->userdata;
	}
	else
	{
		omdp = ObjMgrFindByEntityID(omp, entityID, NULL);
		if (omdp != NULL)
		{
			omudp = omdp->userdata;
			options = omdp->options;
		}
	}

	prev = NULL;
	view_left = FALSE;  /* assume nothing left to receive messages */
	while (omudp != NULL)
	{
		next = omudp->next;
		if ((omudp->procid == procid) || (! procid))
		{
			if ((omudp->proctype == proctype) || (! proctype))
			{
				if ((omudp->userkey == userkey) || (! userkey))
				{
					got_one = TRUE;
					if (prev == NULL)
					{
						if (entityID == 0)
							omp->userdata = next;
						else
							omdp->userdata = next;
					}
					else
						prev->next = next;
					if (omudp->freefunc != NULL)
						(* (omudp->freefunc))(omudp->userdata.ptrvalue);
					omudp = MemFree(omudp);
				}
			}
		}
		if (omudp != NULL)
		{
			prev = omudp;
			if (omudp->messagefunc != NULL) /* gets messages */
				view_left = TRUE;
		}
		omudp = next;
	}

	if (options & OM_OPT_FREE_IF_NO_VIEW)  /* free data if no more views */
	{
		if ((got_one) && (! view_left) && (omdp != NULL))   /* no more views */
		{
			if (! omdp->being_freed)  /* being freed by ObjMgrDelete */
			{
				if (omdp->choice != NULL)
				{
					type = omdp->choicetype;
					ptr = omdp->choice;
				}
				else
				{
					type = omdp->datatype;
					ptr = omdp->dataptr;
				}

				omtp = ObjMgrTypeFind(omp, type, NULL, NULL);
				if (omtp != NULL)
				{
					omdp->being_freed = TRUE; /* flag not to call this func */
                    /* get rid of extra user data */
                    ObjMgrFreeUserDataFunc(omp, entityID, 0, 0, 0);
					is_write_locked = omp->is_write_locked;
					ObjMgrUnlock();
					(*(omtp->freefunc))(ptr);
					if (is_write_locked)
						ObjMgrWriteLock();
					else
						ObjMgrReadLock();
				}
			}
		}
	}
	return got_one;
}



/*****************************************************************************
*
*   ObjMgrLookup(omp, data)
*   	Binary lookup of data in omp->datalist
*   	returns index (>=0) if found
*       returns -1 if not found
*
*****************************************************************************/
NLM_EXTERN Int4 LIBCALL ObjMgrLookup(ObjMgrPtr omp, Pointer data)
{
	Int4 imin, imax, i;
	ObjMgrDataPtr PNTR omdpp;
	unsigned long tmp, datai;

	imin = 0;
	imax = omp->currobj - 1;
	omdpp = omp->datalist;

	datai = (unsigned long)data;

	while (imax >= imin)
	{
		i = (imax + imin)/2;
		tmp = (unsigned long)(omdpp[i]->dataptr);
		if (tmp > datai)
			imax = i - 1;
		else if (tmp < datai)
			imin = i + 1;
		else
			return i;
	}

	return (Int4)(-1);
}

/*****************************************************************************
*
*   ObjMgrDelete(type, data)
*   	deletes a pointer (data) of type (type) to the sequence manager
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrDelete (Uint2 type, Pointer data)
{
	ObjMgrPtr omp;
	ObjMgrDataPtr omdp;
	ObjMgrDataPtr PNTR omdpp, PNTR to, PNTR from;
	Int4 i, j;
	Boolean retval = FALSE;

	omp = ObjMgrWriteLock();  /* really remove the entity */
	i = ObjMgrLookup(omp, data);  /* make sure it is still current */
	if (i < 0)  /* not found */
	{
	   /***	may not be registered with objmgr ***
		ErrPostEx(SEV_ERROR, 0,0, "ObjMgrDelete: pointer [%ld] type [%d] not found",
			(long)data, (int)type);
		***/
		goto erret;
	}

	omdpp = omp->datalist;
	omdp = omdpp[i];    /* emptys always at end */

	if (omdp == NULL) goto erret;

	if (omdp->EntityID != 0)
	{
		ObjMgrSendMsgFunc(omp, omdp, OM_MSG_DEL, omdp->EntityID, 0, 0, 0, 0, 0, NULL);
		ObjMgrDeSelectFunc(omp, omdp->EntityID,0,0,0,NULL);
		if (! omdp->being_freed)  /* ObjMgrFreeUserData can call delete */
		{
			omdp->being_freed = TRUE;  /* break cycle */
			ObjMgrFreeUserDataFunc(omp, omdp->EntityID, 0, 0, 0);
		}
	}

	if (omdp->clipboard)    /* update the clipboard */
		omp->clipboard = NULL;

	if (omdp->lockcnt)
		ErrPostEx(SEV_ERROR, 0,0,"ObjMgrDelete: deleting locked element");
	else if (omdp->tempload == TL_LOADED)
	{
		if (omp->tempcnt)
			omp->tempcnt--;
		else
			ErrPostEx(SEV_ERROR, 0,0, "ObjMgrDelete: reducing tempcnt below 0");
	}

	/* if (omdp->EntityID != 0 && omdp->EntityID == omp->HighestEntityID)
		omp->HighestEntityID--; */
	if (omdp->EntityID != 0)
		ObjMgrRecycleEntityID (omdp->EntityID, omp);

	if (omdp->extradata != NULL && omdp->freeextra != NULL) {
		omdp->freeextra ((Pointer) omdp);
	}

	if (omdp->bulkIndexFree) {
		ObjMgrUnlock(); /* if bulk free, delete at end of BioseqFree or BioseqSetFree */
		return TRUE;
	}

	MemSet((Pointer)omdp, 0, sizeof(ObjMgrData));
	if (omp->currobj)
		omp->currobj--;
	else
		ErrPostEx(SEV_ERROR, 0,0, "ObjMgrDelete: reducing currobj below 0");
	j = omp->currobj - i;
	if (j)
	{
		to = omdpp + i;
		from = to + 1;
		MemMove(to, from, (size_t)(sizeof(ObjMgrDataPtr) * j));
	}
	omdpp[omp->currobj] = omdp;  /* put in pointer to empty data space */

#ifdef DEBUG_OBJMGR
	ObjMgrDump(NULL, "ObjMgrDelete");
#endif

	retval = TRUE;
erret:
	ObjMgrUnlock();
	return retval;
}

/*****************************************************************************
*
*   ObjMgrDeleteAllInRecord()
*   	deletes all omdp entries in a record
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrDeleteAllInRecord (
  void
)

{
  Int4                i, j, k, num;
  ObjMgrDataPtr       omdp;
  ObjMgrDataPtr PNTR  omdpp;
  ObjMgrPtr           omp;
  ObjMgrDataPtr PNTR  tmp;

  omp = ObjMgrWriteLock ();

  if (omp != NULL) {
    omdpp = omp->datalist;
    if (omdpp != NULL) {

      num = omp->currobj;
      tmp = (ObjMgrDataPtr PNTR) MemNew (sizeof (ObjMgrDataPtr) * (size_t) (num + 1));
      if (tmp != NULL) {

        for (i = 0, j = 0, k = 0; i < num; i++) {
          omdp = omdpp [i];
          if (omdp != NULL && omdp->bulkIndexFree) {
            MemSet ((Pointer) omdp, 0, sizeof (ObjMgrData));
            tmp [k] = omdp;
            k++;
          } else {
            omdpp [j] = omdpp [i];
            j++;
          }
        }
        omp->currobj = j;
        MemMove (omdpp + j, tmp, sizeof (ObjMgrDataPtr) * (size_t) k);
      }

      MemFree (tmp);
    }
  }

  ObjMgrUnlock ();

  return TRUE;
}

/*****************************************************************************
*
*   ObjMgrGetClipBoard()
*     returns ObjMgrDataPtr to current clipboard object or NULL if none
*
*****************************************************************************/
NLM_EXTERN ObjMgrDataPtr LIBCALL ObjMgrGetClipBoard (void)
{
	ObjMgrPtr omp;
	
	omp = ObjMgrReadLock();
	ObjMgrUnlock();
	return omp->clipboard;
}

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
NLM_EXTERN Boolean LIBCALL ObjMgrAddToClipBoard (Uint2 entityID, Pointer ptr)
{
	ObjMgrPtr omp;
	ObjMgrDataPtr omdp;
	Boolean retval = FALSE;

	omp = ObjMgrWriteLock();
	if (! entityID)
	{
		omdp = ObjMgrFindByData(omp, ptr);
		if (omdp != NULL)
		{
			if (omdp->parentptr != NULL)
			{
				ErrPostEx(SEV_ERROR,0,0,"AddToClipBoard: ParentPtr != NULL");
				goto erret;
			}
			/* if (omdp->EntityID == 0)
				omdp->EntityID = ++(omp->HighestEntityID); */
			if (omdp->EntityID == 0) {
				omdp->EntityID = ObjMgrNextAvailEntityID (omp);
				ObjMgrRecordOmdpByEntityID (omdp->EntityID, omdp);
			}
		}
	}
	else
	{
		omdp = ObjMgrFindByEntityID(omp, entityID, NULL);
	}

	if (omdp == NULL)
	{
		ErrPostEx(SEV_ERROR,0,0,"AddToClipBoard: data not found");
		goto erret;
	}

	ObjMgrFreeClipBoardFunc(omp);

	omdp->clipboard = TRUE;
	omp->clipboard = omdp;

	ObjMgrSendMsgFunc(omp, omdp, OM_MSG_TO_CLIPBOARD, omdp->EntityID, 0, 0, 0, 0, 0, NULL);

	retval = TRUE;
erret:
	ObjMgrUnlock();

	return retval;
}

/*****************************************************************************
*
*   ObjMgrFreeClipBoard()
*     clears any data from the clipboard
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrFreeClipBoard (void)
{
	ObjMgrPtr omp;
	Boolean retval;
	
	omp = ObjMgrWriteLock();
	retval = ObjMgrFreeClipBoardFunc(omp);
	ObjMgrUnlock();
	return retval;
}

/*****************************************************************************
*
*   ObjMgrFreeClipBoardFunc()
*     clears any data from the clipboard
*
*****************************************************************************/
static Boolean NEAR ObjMgrFreeClipBoardFunc (ObjMgrPtr omp)
{
	ObjMgrTypePtr omtp;
	ObjMgrDataPtr omdp;
	Uint2 type;
	Pointer ptr;
	
	omdp = omp->clipboard;
	if (omdp == NULL) return TRUE;

	if (omdp->choice != NULL)
	{
		type = omdp->choicetype;
		ptr = omdp->choice;
	}
	else
	{
		type = omdp->datatype;
		ptr = omdp->dataptr;
	}

	omtp = ObjMgrTypeFind(omp, type, NULL, NULL);
	if (omtp == NULL)
	{
		ErrPostEx(SEV_ERROR,0,0,"ObjMgrFreeClipBoard: cant find type [%d]", (int)type);
		return FALSE;
	}
	else
		(*(omtp->freefunc))(ptr);
	ObjMgrDelete(omdp->datatype, omdp->dataptr);
	omp->clipboard = NULL;
	return TRUE;
}
/*****************************************************************************
*
*   ObjMgrConnect (type, data, parenttype, parentdata)
*   	Adds parent info to element
*       Updates EntityID
*   		if both are 0, assigns it by incrementing HighestEntityID
*   		if one is 0, assigns it the other
*           if neither is 0 and not the same ID
*                assigns parent to child (and cascades to its children)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrConnect (Uint2 type, Pointer data, Uint2 parenttype, Pointer parentdata)
{
	ObjMgrPtr omp;
	Boolean retval = FALSE;
	
	omp = ObjMgrWriteLock();
	retval = ObjMgrConnectFunc(omp, type, data, parenttype, parentdata);
	ObjMgrUnlock();
	return retval;
}

/*****************************************************************************
*
*   ObjMgrConnectFunc (type, data, parenttype, parentdata)
*   	Adds parent info to element
*       Updates EntityID
*   		if both are 0, assigns it by incrementing HighestEntityID
*   		if one is 0, assigns it the other
*           if neither is 0 and not the same ID
*                assigns parent to child (and cascades to its children)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrConnectFunc (ObjMgrPtr omp, Uint2 type, Pointer data, Uint2 parenttype, Pointer parentdata)
{
	Int4 i;
	ObjMgrDataPtr omdp;
	Boolean retval = FALSE;
	
	i = ObjMgrLookup(omp, data);
	if (i < 0)  /* not found */
	{
		ErrPostEx(SEV_ERROR, 0,0, "ObjMgrConnect: pointer [%ld] type [%d] not found",
			(long)data, (int)type);
		goto erret;
	}

	omdp = omp->datalist[i];

	if ((omdp->EntityID != 0) && (parentdata != NULL))
	{                         /* registered type, inform any attached procs */

		ObjMgrSendMsgFunc(omp, omdp, OM_MSG_CONNECT, omdp->EntityID, 0, 0, 0, 0, 0, NULL);
		ObjMgrDeSelectFunc(omp, omdp->EntityID,0,0,0,NULL);
		ObjMgrFreeUserDataFunc(omp, omdp->EntityID, 0, 0, 0);

		/* if (omp->HighestEntityID == omdp->EntityID) {
			(omp->HighestEntityID)--;
		} */
		ObjMgrRecycleEntityID (omdp->EntityID, omp);
		omdp->EntityID = 0;   /* reset.. now has a parent */
	}
	omdp->parenttype = parenttype;
	omdp->parentptr = parentdata;

	if (parentdata != NULL)
	{
		i = ObjMgrLookup(omp, parentdata);
		if (i < 0)  /* not found */
		{
			ErrPostEx(SEV_ERROR, 0,0, "ObjMgrConnect: parent pointer [%ld] type [%d] not found",
				(long)parentdata, (int)parenttype);
			goto erret;
		}

		
	}

#ifdef DEBUG_OBJMGR
	ObjMgrDump(NULL, "ObjMgrConnect");
#endif
	retval = TRUE;
erret:
	return retval;
}
/*****************************************************************************
*
*   ObjMgrDetach (type, data)
*   	Removes parent info from element
*       Adds to objmgr if necessary
*       Does NOT register entity
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrDetach (Uint2 type, Pointer data)
{
	ObjMgrPtr omp;
	Boolean retval = FALSE;
	
	omp = ObjMgrWriteLock();
	retval = ObjMgrDetachFunc(omp, type, data);
	ObjMgrUnlock();
	return retval;
}

/*****************************************************************************
*
*   ObjMgrDetach (type, data)
*   	Removes parent info from element
*       Adds to objmgr if necessary
*       Does NOT register entity
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrDetachFunc (ObjMgrPtr omp, Uint2 type, Pointer data)
{
	Int4 i;
	ObjMgrDataPtr omdp;
	Boolean retval = FALSE;
	
	i = ObjMgrLookup(omp, data);
	if (i < 0)  /* not found */
	{
		retval = ObjMgrAddFunc(omp, type, data);
		goto erret;
	}

	omdp = omp->datalist[i];
	if (omdp->parentptr == NULL)  /* not connected anyway */
		return TRUE;

	omdp->parenttype = 0;
	omdp->parentptr = NULL;
	omdp->EntityID = 0;   /* reset.. now has a parent */

	retval = TRUE;
erret:
	return retval;
}

/*****************************************************************************
*
*   ObjMgrSetDirtyFlag (entityID, the_flag)
*     record that an entity has been changed but not yet saved.
*       the_flag sets or unsets this state
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrSetDirtyFlag (Uint2 entityID, Boolean the_flag)
{
	ObjMgrPtr omp;
	ObjMgrDataPtr omdp;
	Boolean retval = FALSE;

	if (entityID <= 0) return FALSE;

	omp = ObjMgrWriteLock();
	omdp = ObjMgrFindByEntityID(omp, entityID, NULL);
	if (omdp != NULL)
	{
		omdp->dirty = the_flag;
		retval = TRUE;
	}
	ObjMgrUnlock();
	return retval;
}

/*****************************************************************************
*
*   ObjMgrGetDirtyFlag (entityID)
*     returns the state of the dirty flag for entityID
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrGetDirtyFlag (Uint2 entityID)
{
	ObjMgrPtr omp;
	ObjMgrDataPtr omdp;
	Boolean retval = FALSE;

	if (entityID <= 0) return retval;

	omp = ObjMgrReadLock();
	omdp = ObjMgrFindByEntityID(omp, entityID, NULL);
	if (omdp != NULL)
		retval = omdp->dirty;
	ObjMgrUnlock();
	return retval;
}

/*****************************************************************************
*
*   ObjMgrSetChoice(type, choice, data)
*   	Adds the ValNodePtr pointing directly to this Bioseq or BioseqSet
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrSetChoice (Uint2 type, ValNodePtr choice, Pointer data)
{
	ObjMgrPtr omp;
	Int4 i;
	ObjMgrDataPtr omdp;
	Boolean retval = FALSE;
	
	omp = ObjMgrWriteLock();

	i = ObjMgrLookup(omp, data);
	if (i < 0)  /* not found */
	{
		ErrPostEx(SEV_ERROR, 0,0, "ObjMgrChoice: pointer [%ld] type [%d] not found",
			(long)data, (int)type);
		goto erret;
	}

	omdp = omp->datalist[i];
	omdp->choicetype = type;
	omdp->choice = choice;
	retval = TRUE;
erret:
	ObjMgrUnlock();

#ifdef DEBUG_OBJMGR
	ObjMgrDump(NULL, "ObjMgrChoice");
#endif

	return retval;
}

/*****************************************************************************
*
*   ObjMgrFindTop(omp, omdp)
*   	finds the highest parent of omdp
*       returns a ObjMgrDataPtr to the top
*
*****************************************************************************/
NLM_EXTERN ObjMgrDataPtr LIBCALL ObjMgrFindTop (ObjMgrPtr omp, ObjMgrDataPtr omdp)
{
	Int4 i;

	if ((omp == NULL) || (omdp == NULL)) return NULL;

	while (omdp->parentptr != NULL)
	{
		i = ObjMgrLookup(omp, omdp->parentptr);
		if (i < 0)  /* not found */
		{
			ErrPostEx(SEV_ERROR, 0,0, "ObjMgrFindTop: parentptr [%ld] not found",
				(long)(omdp->parentptr));
			return NULL;
		}
		omdp = omp->datalist[i];
	}

	return omdp;
}

/*****************************************************************************
*
*   ObjMgrWholeEntity(omdp, itemID, itemtype)
*		returns TRUE if itemID, itemtype identify a complete entity omdp
*       returns FALSE if these are an internal part of the entity
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrWholeEntity (ObjMgrDataPtr omdp, Uint2 itemID, Uint2 itemtype)
{
	if (omdp == NULL)
		return FALSE;

	if ((itemID == 0) && (itemtype == 0))
		return TRUE;

	if ((itemID == 1) && (itemtype == omdp->datatype))
		return TRUE;

	return FALSE;

}

/*****************************************************************************
*
*   ObjMgrFreeCache(type)
*   	Frees all cached objects of type and subtypes of type
*   		based on ObjMgrMatch()
*   	if type == 0, frees all cached objects
*   	returns TRUE if no errors occured
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrFreeCache (Uint2 type)
{
	ObjMgrPtr omp;
	Boolean more_to_go = TRUE;
	Uint2 rettype;
	VoidPtr retval;

	while (more_to_go)
        {
           omp = ObjMgrWriteLock();
/*
                omp = ObjMgrReadLock ();
*/
                more_to_go = ObjMgrFreeCacheFunc(omp, type, &rettype, &retval);
		if (more_to_go)
		{
			switch (rettype)
			{
			case OBJ_SEQENTRY:
				SeqEntryFree((SeqEntryPtr)retval);
				break;
			case OBJ_BIOSEQ:
				BioseqFree((BioseqPtr)retval);
				break;
			case OBJ_BIOSEQSET:
				BioseqSetFree((BioseqSetPtr)retval);
				break;
			default:
				break;
			}
		}
		ObjMgrUnlock();
	}

        /*        if(type == 0)
                  omp->HighestEntityID = 0; */
        
	return TRUE;
}

/*****************************************************************************
*
*   ObjMgrFreeCacheFunc(type)
*   	Finds next cached objects of type and subtypes of type
*   		based on ObjMgrMatch()
*   	if type == 0, frees all cached objects
*   	returns TRUE if no more found
*
*****************************************************************************/
static Boolean NEAR ObjMgrFreeCacheFunc (ObjMgrPtr omp, Uint2 type, Uint2Ptr rettype, VoidPtr PNTR retval)
{
    Int4 i, num;
	ObjMgrDataPtr PNTR omdpp, omdp;

	if (omp->hold)   /* things are being held */
		return FALSE;   /* nothing to do, but send a warning */

	omdpp = omp->datalist;
	num = omp->currobj;
    for (i = 0; i < num; i++)
    {
		omdp = omdpp[i];
		if ((omdp->parentptr == NULL) &&     /* top level */
			(omdp->tempload == TL_CACHED))   /* cached */
		{
			if ((! type) ||
				(ObjMgrMatch(type, omdp->datatype)) ||
				(ObjMgrMatch(type, omdp->choicetype)))
			{
				/** here is where the freefunc should be called **/

				if (omdp->choice != NULL)
				{
					switch (omdp->choicetype)
					{
						case OBJ_SEQENTRY:
							*rettype = omdp->choicetype;
							*retval = omdp->choice;
							return TRUE;
						default:
							ErrPostEx(SEV_ERROR,0,0, "ObjMgrFreeCache: unsupported choicetype[%d]",
								(int)(omdp->choicetype));
							break;
					}
				}
				else
				{
					switch(omdp->datatype)
					{
						case OBJ_BIOSEQ:
						case OBJ_BIOSEQSET:
							*rettype = omdp->datatype;
							*retval = omdp->dataptr;
							return TRUE;
						default:
							ErrPostEx(SEV_ERROR,0,0,"ObjMgrFreeCache: usupported datatype [%d]",
								(int)(omdp->datatype));
							break;
					}
				}
			}
		}
    }
    return FALSE;
	
}


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
NLM_EXTERN Int2 LIBCALL ObjMgrMatch (Uint2 type1, Uint2 type2)
{
	if (type1 == type2)
		return 1;

	switch (type1)
	{
		case OBJ_SEQENTRY:
			switch (type2)
			{
				case OBJ_BIOSEQ:
				case OBJ_BIOSEQSET:
					return 2;
			}
			break;
		case OBJ_SEQANNOT:
			switch (type2)
			{
				case OBJ_SEQFEAT:
				case OBJ_SEQALIGN:
				case OBJ_SEQGRAPH:
					return 2;
			}
			break;
		case OBJ_SEQCODE_SET:
			switch (type2)
			{
				case OBJ_SEQCODE:
					return 2;
			}
			break;
		case OBJ_GENETIC_CODE_SET:
			switch (type2)
			{
				case OBJ_GENETIC_CODE:
					return 2;
			}
			break;
		default:
			break;
	}

	return 0;
}


/*****************************************************************************
*
*   ObjMgrGetChoiceForData(data)
*   	returns ValNodePtr for a BioseqPtr or BioseqSetPtr
*       choice must have been put in ObjMgr using ObjMgrChoice
*       the Bioseq/BioseqSets it is a part of must also be in ObjMgr
*       returns NULL on failure.
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ObjMgrGetChoiceForData (Pointer data)
{
	ObjMgrPtr omp;
	Int4 i;
	ValNodePtr choice = NULL;

	if (data == NULL) return choice;
	omp = ObjMgrReadLock();
	i = ObjMgrLookup(omp, data);
	if (i < 0)
	{
		ErrPostEx(SEV_ERROR, 0,0, "ChoiceGetChoiceForData: data not in ObjMgr");
	}
	else
	{
		choice = omp->datalist[i]->choice;
	}
	ObjMgrUnlock();

	return choice;
}

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
*       returns 0 on failure.
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrGetEntityIDForChoice (ValNodePtr choice)
{
	Pointer data;

	if (choice == NULL) return 0;
	data = choice->data.ptrvalue;
	if (data == NULL) return 0;

	return ObjMgrGetEntityIDForPointer (data);
}

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
NLM_EXTERN Uint2 LIBCALL ObjMgrGetEntityIDForPointer (Pointer ptr)
{
	ObjMgrPtr omp;
	ObjMgrDataPtr omdp;
	Uint2 retval = 0;

	if (ptr == NULL)
		return retval;

	omp = ObjMgrWriteLock(); /* (EY) */
	omdp = ObjMgrFindByData(omp, ptr);

	if (omdp == NULL) goto erret;

	omdp = ObjMgrFindTop(omp, omdp);
	if (omdp == NULL) goto erret;

	if (omdp->EntityID == 0)     /* need to assign it */
		ObjMgrAddEntityID(omp, omdp);

	retval = omdp->EntityID;
erret:
	ObjMgrUnlock();

	return retval;
}

/*****************************************************************************
*
*   ObjMgrGetChoiceForEntityID (id)
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL ObjMgrGetChoiceForEntityID (Uint2 id)
{
	ObjMgrPtr omp;
	ObjMgrDataPtr omdp;
	ValNodePtr retval = NULL;

	if (id <= 0) return retval;

	omp = ObjMgrReadLock();
	omdp = ObjMgrFindByEntityID(omp, id, NULL);
	if (omdp != NULL)
		retval = omdp->choice;
	ObjMgrUnlock();
	return retval;
}

/*****************************************************************************
*
*   ObjMgrLock(type, data, lockit)
*   	if lockit=TRUE, locks the element
*       else unlocks it
*       returns the current lock count or -1 on failure
*
*****************************************************************************/
NLM_EXTERN Int4 LIBCALL ObjMgrLock (Uint2 type, Pointer data, Boolean lockit)
{
	ObjMgrPtr omp;
	Int4 retval = -1;
	
	omp = ObjMgrWriteLock();
	if (omp != NULL)
		retval = ObjMgrLockFunc(omp, type, data, lockit);
	ObjMgrUnlock();
	return retval;
}

static Int4 NEAR ObjMgrLockFunc (ObjMgrPtr omp, Uint2 type, Pointer data, Boolean lockit)
{
	Int4 i, lockcnt = -1;
	ObjMgrDataPtr omdp;

#ifdef DEBUG_OBJMGR
	Char buf[80];
#endif

	i = ObjMgrLookup(omp, data);
	if (i < 0)  /* not found */
	{
		ErrPostEx(SEV_ERROR, 0,0, "ObjMgrLock: pointer [%ld] type [%d] not found",
			(long)data, (int)type);
		return lockcnt;
	}

	omdp = ObjMgrFindTop(omp, omp->datalist[i]);

	if (lockit) {
		omdp->lockcnt++;

		/******** desktop can do this *******
		if (omdp->tempload == TL_CACHED)
		{
			ErrPostEx(SEV_ERROR, 0,0,"ObjMgrLock: locking a cached entity");
		}
		*************************************/
	}
	else
	{
		if (omdp->lockcnt) {
			omdp->lockcnt--;
		}
		else
		{
			ErrPostEx(SEV_ERROR, 0,0,"ObjMgrLock: unlocking 0 lockcnt");
			return lockcnt;
		}
	}

	lockcnt = omdp->lockcnt;

	if (! lockit)     /* check for automatic delete */
	{
		if ((omdp->tempload != TL_NOT_TEMP) && (! omdp->lockcnt))
		{
			omdp->touch = ObjMgrTouchCnt();   /* stamp with time */
			/*
			omp->tempcnt++;
			*/
			ObjMgrReap(omp);
		}
	}


#ifdef DEBUG_OBJMGR
	if (lockit)
		sprintf(buf, "ObjMgrLock   Lock [%d]", (int)i);
	else
		sprintf(buf, "ObjMgrLock   Unlock [%d]", (int)i);
	ObjMgrDump(NULL, buf);
#endif

	return lockcnt;
}

/*****************************************************************************
*
*   Boolean ObjMgrSetTempLoad (ObjMgrPtr omp, Pointer ptr)
*   Sets a Loaded element to "temporary"
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrSetTempLoad (ObjMgrPtr omp, Pointer ptr)
{
	Int4 index;
	ObjMgrDataPtr omdp;

	index = ObjMgrLookup(omp, ptr);
	if (index < 0) return FALSE;

	omdp = ObjMgrFindTop(omp, omp->datalist[index]);
	if (omdp == NULL) return FALSE;

	if (omdp->tempload == TL_NOT_TEMP)
	{
		omdp->tempload = TL_LOADED;
		omp->tempcnt++;
	}
	omdp->touch = ObjMgrTouchCnt();

	omdp->lockcnt++;    /* protect against reaping this one */
	ObjMgrReap (omp);   /* check to see if we need to reap */
	omdp->lockcnt--;    /* reset to previous state */

	return TRUE;
}

/*****************************************************************************
*
*   ObjMgrReap(omp)
*   	Checks to see if memory needs to be cleared, and does it
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrReap (ObjMgrPtr omp)
{
	Uint4 lowest;
	Int4 num, j;
	ObjMgrDataPtr tmp, ditch, PNTR omdpp;
	Boolean is_write_locked, did_one = FALSE;

	if (omp->hold)      /* keep all tempload records around while hold is on */
		return FALSE;

	if (omp->reaping)   /* protect against recursion caused by ObjMgrSendMsg */
		return FALSE;

	while (omp->tempcnt > omp->maxtemp)   /* time to reap */
	{
		lowest = UINT4_MAX;
		
		num = omp->currobj;
		omdpp = omp->datalist;
		ditch = NULL;
		for (j = 0; j < num; j++, omdpp++)
		{
			tmp = *omdpp;
			if ((tmp->tempload == TL_LOADED) && (! tmp->lockcnt))
			{
				if (lowest > tmp->touch)
				{
					lowest = tmp->touch;
					ditch = tmp;
				}
			}
		}
		if (ditch == NULL)    /* nothing to free */
			return FALSE;

		omp->reaping = TRUE;
		ditch->tempload = TL_CACHED;
		ObjMgrSendMsgFunc(omp, ditch, OM_MSG_CACHED, ditch->EntityID, 0, 0, 0, 0, 0, NULL);
		omp->tempcnt--;
		is_write_locked = omp->is_write_locked;

		/* null out feature pointers in seqmgr feature indices via reap function */

		if (ditch->extradata != NULL && ditch->reapextra != NULL) {
			ditch->reapextra ((Pointer) ditch);
		}

		if (ditch->choice != NULL)
		{
			switch (ditch->choicetype)
			{
				case OBJ_SEQENTRY:
					did_one = TRUE;
					ObjMgrUnlock();
					SeqEntryFreeComponents(ditch->choice);
					break;
			}
		}
		else
		{
			switch (ditch->datatype)
			{
				case OBJ_BIOSEQ:
					did_one = TRUE;
					ObjMgrUnlock();
					BioseqFreeComponents((BioseqPtr)(ditch->dataptr));
					break;
				case OBJ_BIOSEQSET:
					did_one = TRUE;
					ObjMgrUnlock();
					BioseqSetFreeComponents((BioseqSetPtr)(ditch->dataptr), FALSE);
					break;
				default:
					ErrPostEx(SEV_ERROR,0,0,"ObjMgrUnlock: ditching unknown type");
					break;
			}
		}

		if (did_one)
		{
			if (is_write_locked)
				omp = ObjMgrWriteLock();
			else
				omp = ObjMgrReadLock();
		}
		
		omp->reaping = FALSE;

#ifdef DEBUG_OBJMGR
	ObjMgrDump(NULL, "ObjMgrReap");
#endif

	}

	return TRUE;
}

/*****************************************************************************
*
*   ObjMgrReapOne(omp)
*   	Reaps the single least recently accessed entity
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrReapOne (ObjMgrPtr omp)
{
	Uint4 lowest;
	Int4 num, j;
	ObjMgrDataPtr tmp, ditch, PNTR omdpp;
	Boolean is_write_locked, did_one = FALSE;

	if (omp->hold)      /* keep all tempload records around while hold is on */
		return FALSE;

	if (omp->reaping)   /* protect against recursion caused by ObjMgrSendMsg */
		return FALSE;

		lowest = UINT4_MAX;
		
		num = omp->currobj;
		omdpp = omp->datalist;
		ditch = NULL;
		for (j = 0; j < num; j++, omdpp++)
		{
			tmp = *omdpp;
			if ((tmp->tempload == TL_LOADED) && (! tmp->lockcnt))
			{
				if (lowest > tmp->touch)
				{
					lowest = tmp->touch;
					ditch = tmp;
				}
			}
		}
		if (ditch == NULL)    /* nothing to free */
			return FALSE;

		omp->reaping = TRUE;
		ditch->tempload = TL_CACHED;
		ObjMgrSendMsgFunc(omp, ditch, OM_MSG_CACHED, ditch->EntityID, 0, 0, 0, 0, 0, NULL);
		omp->tempcnt--;
		is_write_locked = omp->is_write_locked;

		/* null out feature pointers in seqmgr feature indices via reap function */

		if (ditch->extradata != NULL && ditch->reapextra != NULL) {
			ditch->reapextra ((Pointer) ditch);
		}

		if (ditch->choice != NULL)
		{
			switch (ditch->choicetype)
			{
				case OBJ_SEQENTRY:
					did_one = TRUE;
					ObjMgrUnlock();
					SeqEntryFreeComponents(ditch->choice);
					break;
			}
		}
		else
		{
			switch (ditch->datatype)
			{
				case OBJ_BIOSEQ:
					did_one = TRUE;
					ObjMgrUnlock();
					BioseqFreeComponents((BioseqPtr)(ditch->dataptr));
					break;
				case OBJ_BIOSEQSET:
					did_one = TRUE;
					ObjMgrUnlock();
					BioseqSetFreeComponents((BioseqSetPtr)(ditch->dataptr), FALSE);
					break;
				default:
					ErrPostEx(SEV_ERROR,0,0,"ObjMgrUnlock: ditching unknown type");
					break;
			}
		}

		if (did_one)
		{
			if (is_write_locked)
				omp = ObjMgrWriteLock();
			else
				omp = ObjMgrReadLock();
		}
		
		omp->reaping = FALSE;

#ifdef DEBUG_OBJMGR
	ObjMgrDump(NULL, "ObjMgrReapOne");
#endif

	return TRUE;
}

/*****************************************************************************
*
*   Boolean ObjMgrIsTemp(data)
*   	returns TRUE if data is a temporarily loaded item
*       data must be BioseqPtr or BioseqSetPtr
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrIsTemp (Pointer data)
{
	ObjMgrPtr omp;
	Int4 i;
	ObjMgrDataPtr omdp;
	Boolean retval = FALSE;
	
	omp = ObjMgrReadLock();

	i = ObjMgrLookup(omp, data);
	if (i < 0)  /* not found */
	{
		ErrPostEx(SEV_ERROR, 0,0, "ObjMgrIsTemp: pointer [%ld] not found",
			(long)data);
		goto erret;
	}

	omdp = ObjMgrFindTop(omp, omp->datalist[i]);
	if (omdp == NULL) goto erret;

	if (omdp->tempload != TL_NOT_TEMP)
		retval = TRUE;
erret:
	ObjMgrUnlock();
	return retval;
}

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
NLM_EXTERN Boolean LIBCALL ObjMgrIsChild (Pointer parent, Pointer child)
{
	ObjMgrPtr omp;
	Int4 i;
	ObjMgrDataPtr omdp, PNTR omdpp;
	Boolean retval = FALSE;

	if (child == NULL) return FALSE;
	if (parent == child) return TRUE;

	omp = ObjMgrReadLock();
	omdpp = omp->datalist;

	i = ObjMgrLookup(omp, child);
	if (i < 0)  /* not found */
	{
		ErrPostEx(SEV_ERROR, 0,0, "ObjMgrIsChild: pointer [%ld] not found",
			(long)child);
		goto erret;
	}

	omdp = omdpp[i];
	if (parent == NULL)
	{
		if (omdp->parentptr == NULL)
			retval = TRUE;
		goto erret;
	}

	while (omdp->parentptr != NULL)
	{
		if (omdp->parentptr == parent)
		{
			retval = TRUE;
			goto erret;
		}
		i = ObjMgrLookup(omp, omdp->parentptr);
		if (i < 0)
			goto erret;
		omdp = omdpp[i];
	}
erret:
	ObjMgrUnlock();
	return retval;
}

/*****************************************************************************
*
*   ObjMgrDump(fp)
*
*****************************************************************************/

NLM_EXTERN void LIBCALL ObjMgrDump (FILE * fp, CharPtr title)
{
	ObjMgrPtr omp;
	ObjMgrDataPtr omdp;
	Int4 i;
	Char buf[80];
	BioseqPtr bsp;
	Boolean close_it = FALSE;
	
	if (fp == NULL)
	{
		fp = FileOpen("ObjMgr.log", "a");
		close_it = TRUE;
	}

	omp = ObjMgrGet();
	fprintf(fp, "\n%s currobj=%d tempcnt=%d\n", title, (int)(omp->currobj),
		(int)(omp->tempcnt));
	for (i = 0; i < (Int4)(omp->currobj); i++)
	{
		omdp = omp->datalist[i];
		fprintf(fp, "[%d] [%d %d %ld] [%d %ld] %ld (%d) %uld\n", (int)i,
		    (int)omdp->EntityID, (int)(omdp->datatype),
			(long)(omdp->dataptr), (int)(omdp->parenttype),
			(long)(omdp->parentptr), (long)(omdp->choice), (int)(omdp->lockcnt),
			(unsigned long)(omdp->touch));
		if ((omdp->datatype == OBJ_BIOSEQ) && (omdp->dataptr != NULL))
		{
			bsp = (BioseqPtr)(omdp->dataptr);
			SeqIdWrite(bsp->id, buf, PRINTID_FASTA_LONG, sizeof (buf));
			fprintf(fp, "[%s %ld]\n", buf, (long)(bsp));
		}
	}
	
	if (close_it)
		FileClose(fp);
	return;
}

/*****************************************************************************
*
*   Procedure Management Functions
*
*****************************************************************************/


/*****************************************************************************
*
*   ObjMgrProcExtend(omp)
*
*****************************************************************************/
static Boolean NEAR ObjMgrProcExtend (ObjMgrPtr omp)
{
	Boolean result = FALSE;
	OMProcPtr omdp, prev=NULL;
	ObjMgrProcPtr PNTR tmp;
	Int4 i, j;

	for (omdp = omp->ncbiproc; omdp != NULL; omdp = omdp->next)
		prev = omdp;

	omdp = (OMProcPtr)MemNew(sizeof(OMProc));
	if (omdp == NULL) return result;
	tmp = (ObjMgrProcPtr PNTR)MemNew((size_t)(sizeof(ObjMgrProcPtr) * (omp->totproc + NUM_OMD)));
	if (tmp == NULL)
	{
		MemFree(omdp);
		return result;
	}

	if (prev != NULL)
	{
		prev->next = omdp;
		MemMove(tmp, omp->proclist, (size_t)(sizeof(ObjMgrProcPtr) *omp->totproc));
		MemFree(omp->proclist);
	}
	else
		omp->ncbiproc = omdp;

	j = omp->totproc;

	for (i = 0; i < NUM_OMD; i++, j++)
		tmp[j] = &(omdp->data[i]);

	omp->totproc += NUM_OMD;
	omp->proclist = tmp;

	result = TRUE;
	return result;
}

/*****************************************************************************
*
*   ObjMgrProcAdd(data, priority)
*   	adds an ObjMgrProc at given priority
*		priority must be > 0
*       highest priority function is called first.
*       if priority == 0 (default)
*   		gets the next highest priority over previous procs of same type
*       if priority == PROC_PRIORITY_HIGHEST
*           is always the highest priority (first one wins)
*   	if priority == PROC_PRIORITY_LOWEST
*           is always the lowest priority
*
*****************************************************************************/
NLM_EXTERN Uint2  LIBCALL ObjMgrProcAdd (ObjMgrProcPtr data, Int2 priority)
{
	ObjMgrPtr omp;
	ObjMgrProcPtr omdp;
	ObjMgrProcPtr PNTR omdpp;
	Int4 i;
	Uint2 procID;
	Uint2 retval = 0;

	omp = ObjMgrWriteLock();

	if (priority == 0)   /* set to next highest */
	{
		omdp = ObjMgrProcFindNext(omp, data->proctype, data->inputtype,
								data->outputtype, NULL);
		if (omdp != NULL)
			priority = omdp->priority + 10;
	}


	if (omp->currproc >= omp->totproc)
	{
		if (! ObjMgrProcExtend(omp))
			goto erret;
	}

	i = omp->currproc;
	omdpp = omp->proclist;
	omdp = omdpp[i];    /* emptys always at end */

	omp->currproc++;     /* got one more */
	procID = ++(omp->HighestProcID);

	MemMove(omdp, data, sizeof(ObjMgrProc));
	omdp->priority = priority;
	omdp->procid = procID;  /* fill in the values */
	omdp->proclabel = StringSave(data->proclabel);
	omdp->procname = StringSave(data->procname);
	omdp->submenu = StringSave(data->submenu);
	retval = procID;
erret:
	ObjMgrUnlock();

	return retval;
}

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
NLM_EXTERN Uint2 ObjMgrProcLoadEx (Uint2 proctype, CharPtr procname, CharPtr proclabel,
							Uint2 inputtype, Uint2 subinputtype,
							Uint2 outputtype, Uint2 suboutputtype, Pointer userdata,
							ObjMgrGenFunc func, Int2 priority, CharPtr submenu)
{
	ObjMgrPtr omp;
	ObjMgrProcPtr ompp;
	ObjMgrProc ompd;

	omp = ObjMgrWriteLock();
	ompp = ObjMgrProcFind(omp, 0, procname, proctype);
	ObjMgrUnlock();
	if (ompp != NULL)  /* already enabled */
	{
		return ompp->procid;
	}

	ompp = &ompd;
	MemSet((Pointer)ompp, 0, sizeof(ObjMgrProc));
	
	ompp->proctype = proctype;
	ompp->procname = procname;
	ompp->proclabel = proclabel;
	ompp->inputtype = inputtype;
	ompp->subinputtype = subinputtype;
	ompp->outputtype = outputtype;
	ompp->suboutputtype = suboutputtype;
	ompp->procdata = userdata;
	ompp->func = func;
	ompp->submenu = submenu;

	return ObjMgrProcAdd(ompp, priority); /* order determines priority */
	
}

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
NLM_EXTERN Uint2 ObjMgrProcLoad (Uint2 proctype, CharPtr procname, CharPtr proclabel,
							Uint2 inputtype, Uint2 subinputtype,
							Uint2 outputtype, Uint2 suboutputtype, Pointer userdata,
							ObjMgrGenFunc func, Int2 priority)

{
	return ObjMgrProcLoadEx (proctype, procname, proclabel, inputtype, subinputtype,
							outputtype, suboutputtype, userdata, func, priority, NULL);
}

/*****************************************************************************
*
*   ObjMgrProcLookup(omp, procid)
*   	these are currently just stored in order
*   	returns index (>=0) if found
*       returns -1 if not found
*
*****************************************************************************/
NLM_EXTERN Int4 LIBCALL ObjMgrProcLookup(ObjMgrPtr omp, Uint2 procid)
{
	if (omp == NULL)
		return (Int4)(-1);

	if (procid)
		return (Int4)(procid - 1);
	else
		return (Int4)(-1);
}

/*****************************************************************************
*
*   ObjMgrProcFind(omp, procid, procname, proctype)
*   	if procid != NULL looks for it
*   	else matches on procname and proctype
*   		proctype = 0, matches all proctypes
*
*****************************************************************************/
NLM_EXTERN ObjMgrProcPtr LIBCALL ObjMgrProcFind(ObjMgrPtr omp, Uint2 procid,
										CharPtr procname, Uint2 proctype)
{
	ObjMgrProcPtr ompp=NULL, PNTR omppp, tmp;
	Int4 i, imax;

	omppp = omp->proclist;
	imax = omp->currproc;

	if (procid)   /* procid lookup is different */
	{
		i = ObjMgrProcLookup(omp, procid);
		if (i >= 0)
			ompp = omppp[i];
		return ompp;
	}

	for (i = 0; i < imax; i++)
	{
		tmp = omppp[i];
		if ((! proctype) || (proctype == tmp->proctype))
		{
			if (! StringCmp(procname, tmp->procname))
				return tmp;
		}
	}
	return ompp;
}

/*****************************************************************************
*
*   ObjMgrGetProcID(omp, procname, proctype)
*   	returns procid given procname and proctype
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrGetProcID (ObjMgrPtr omp, CharPtr procname, Uint2 proctype)

{
	ObjMgrProcPtr  ompp;

	ompp = ObjMgrProcFind (omp, 0, procname, proctype);
	if (ompp == NULL) return 0;
	return ompp->procid;
}

/*****************************************************************************
*
*   ObjMgrProcFindNext(omp, proctype, inputtype, outputtype, last)
*   	looks for proctype of highest priority that
*   		matches inputtype and outputtype
*   	0 on proctype or inputtype or outputtype matches any
*   	if last != NULL, then gets next after last
*       if omp == NULL, will ObjMgrReadLock() and Unlock() within the function
*
*****************************************************************************/
NLM_EXTERN ObjMgrProcPtr LIBCALL ObjMgrProcFindNext (ObjMgrPtr tomp, Uint2 proctype,
						Uint2 inputtype, Uint2 outputtype, ObjMgrProcPtr last)
{
	ObjMgrPtr omp;
	ObjMgrProcPtr best = NULL, tmp, retval = NULL;
	Int4 i, bestpriority=-32766, imax, maxpriority=32767;
	ObjMgrProcPtr PNTR omppp;
	Boolean unlock = FALSE;

	if (tomp == NULL)
	{
		omp = ObjMgrReadLock();
		unlock = TRUE;
	}
	else
		omp = tomp;

	omppp = omp->proclist;
	imax = omp->currproc;

	if (last != NULL)
	{
		maxpriority = last->priority;
		i = ObjMgrProcLookup(omp, last->procid);
		i++;
		while (i < imax)  /* find next of equal priority */
		{
			tmp = omppp[i];
			if ((! proctype) || (tmp->proctype == proctype))
			{
				if ((! inputtype) || (tmp->inputtype == inputtype))
				{
					if ((! outputtype) || (tmp->outputtype == outputtype))
					{
						if (tmp->priority == maxpriority)
						{
							retval = tmp;
							goto erret;
						}
					}
				}
			}
			i++;
		}
	}

	for (i = 0; i < imax; i++)  /* find the highest priority less than any previous */
	{
		tmp = omppp[i];
		if ((! proctype) || (tmp->proctype == proctype))
		{
			if ((! inputtype) || (tmp->inputtype == inputtype))
			{
				if ((! outputtype) || (tmp->outputtype == outputtype))
				{
					if ((tmp->priority > bestpriority) &&
						(tmp->priority < maxpriority))
					{
						best = tmp;
						bestpriority = tmp->priority;
					}
				}
			}
		}

	}

	retval = best;
erret:
	if (unlock)
		ObjMgrUnlock();
	return retval;
}

NLM_EXTERN Int2 LIBCALL ObjMgrProcOpen (ObjMgrPtr omp, Uint2 outputtype)
{
	ObjMgrProcPtr ompp=NULL, currp=NULL;
	Int2 retval;
	Boolean did_one = FALSE;
	OMProcControl ompc;

	MemSet((Pointer)(&ompc), 0, sizeof(OMProcControl));
	ompc.output_itemtype = outputtype;

	while ((currp = ObjMgrProcFindNext(omp, OMPROC_OPEN, 0, outputtype, currp)) != NULL)
	{
		ompc.proc = currp;
		retval = (*(currp->func)) (&ompc);
		did_one = TRUE;
		if (retval == OM_MSG_RET_DONE)
			break;
	}

	if (! did_one)
	{
		ErrPostEx(SEV_ERROR,0,0, "No OPEN function found");
		retval = OM_MSG_RET_ERROR;
	}
	return retval;
}

NLM_EXTERN Uint2 LIBCALL OMGetNextUserKey (void)

{
  ObjMgrPtr  omp;
  Uint2 retval = 0;

  omp = ObjMgrWriteLock ();
  if (omp == NULL) return retval;
  (omp->HighestUserKey)++;
  if (omp->HighestUserKey == 0) {
    (omp->HighestUserKey)++;
  }
  retval = omp->HighestUserKey;
  ObjMgrUnlock();
  return retval;
}

/*****************************************************************************
*
*   Data Type Functions
*
*****************************************************************************/

static Int2 LIBCALLBACK ObjMgrDefaultLabelFunc ( Pointer data, CharPtr buffer, Int2 buflen, Uint1 content)
{
	ObjMgrPtr omp;
	ObjMgrDataPtr omdp;
	ObjMgrTypePtr omtp;
	CharPtr label=NULL;
	static CharPtr defaultlabel="NoLabel";
	Int2 retval = 0;

	label = defaultlabel;
	omp = ObjMgrReadLock();
	omdp = ObjMgrFindByData(omp, data);
	if (omdp != NULL)
	{
		omtp = ObjMgrTypeFind(omp, omdp->datatype, NULL, NULL);
		if (omtp != NULL)
		{
			if (omtp->label != NULL)
				label = omtp->label;
			else if (omtp->name != NULL)
				label = omtp->name;
			else
				label = omtp->asnname;
		}
	}

	retval = LabelCopy(buffer, label, buflen);
	ObjMgrUnlock();
	return retval;
}

static Uint2 LIBCALLBACK ObjMgrDefaultSubTypeFunc (Pointer ptr)
{
	return 0;
}

/*****************************************************************************
*
*   ObjMgrTypeExtend(omp)
*
*****************************************************************************/
static Boolean NEAR ObjMgrTypeExtend (ObjMgrPtr omp)
{
	Boolean result = FALSE;
	OMTypePtr omdp, prev=NULL;
	ObjMgrTypePtr PNTR tmp;
	Int4 i, j;

	for (omdp = omp->ncbitype; omdp != NULL; omdp = omdp->next)
		prev = omdp;

	omdp = (OMTypePtr)MemNew(sizeof(OMType));
	if (omdp == NULL) return result;
	tmp = (ObjMgrTypePtr PNTR)MemNew((size_t)(sizeof(ObjMgrTypePtr) * (omp->tottype + NUM_OMD)));
	if (tmp == NULL)
	{
		MemFree(omdp);
		return result;
	}

	if (prev != NULL)
	{
		prev->next = omdp;
		MemMove(tmp, omp->typelist, (size_t)(sizeof(ObjMgrTypePtr) * omp->tottype));
		MemFree(omp->typelist);
	}
	else
		omp->ncbitype = omdp;

	j = omp->tottype;

	for (i = 0; i < NUM_OMD; i++, j++)
		tmp[j] = &(omdp->data[i]);

	omp->tottype += NUM_OMD;
	omp->typelist = tmp;

	result = TRUE;
	return result;
}

/*****************************************************************************
*
*   ObjMgrTypeAddFunc(omp, type, data)
*   	adds a pointer (data) of type (type) to the sequence manager
*       Currently this MUST be predefined and < OBJ_MAX
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrTypeAdd (ObjMgrTypePtr data)
{
	ObjMgrPtr omp;
	ObjMgrTypePtr omdp;
	ObjMgrTypePtr PNTR omdpp;
	Int4 i, imin, imax;
	Uint2 tmp, type,retval = 0;

	if (data == NULL) return retval;

	omp = ObjMgrWriteLock();

	type = data->datatype;
	if ((! type) || (type >= OBJ_MAX))
	{
		/***
		type = ++(omp->HighestObjMgrType);
	    ***/

		ErrPostEx(SEV_ERROR, 0,0, "ObjMgrTypeAdd: Can't register new object types yet");
		goto erret;
	}

	if (omp->currtype >= omp->tottype)
	{
		if (! ObjMgrTypeExtend(omp))
			goto erret;
	}

	i = omp->currtype;
	omdpp = omp->typelist;
	omdp = omdpp[i];    /* emptys always at end */

	imin = 0;                   /* find where it goes */
	imax = omp->currtype-1;

	if ((i) && (type < omdpp[imax]->datatype))
	{
		i = (imax + imin) / 2;
		while (imax > imin)
		{
			tmp = omdpp[i]->datatype;
			if (tmp > type)
				imax = i - 1;
			else if (tmp < type)
				imin = i + 1;
			else
				break;
			i = (imax + imin)/2;
		}

		if (type > omdpp[i]->datatype)     /* check for off by 1 */
			i++;

		imax = omp->currtype - 1;	 /* open the array */
		while (imax >= i)
		{
			omdpp[imax+1] = omdpp[imax];
			imax--;
		}
	}

	omdpp[i] = omdp;    /* put in the pointer in order */
	omp->currtype++;     /* got one more */

	MemMove(omdp, data, sizeof(ObjMgrType));
	omdp->datatype = type;  /* fill in the values */
	omdp->asnname = StringSave(data->asnname);
	omdp->label = StringSave(data->label);
	omdp->name = StringSave(data->name);
	if (omdp->labelfunc == NULL)
		omdp->labelfunc = ObjMgrDefaultLabelFunc;
	if (omdp->subtypefunc == NULL)
		omdp->subtypefunc = ObjMgrDefaultSubTypeFunc;
	retval = type;
erret:
	ObjMgrUnlock();
	return retval;
}

/*****************************************************************************
*
*   ObjMgrTypeLoad(args)
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL ObjMgrTypeLoad ( Uint2 type, CharPtr asnname,
		CharPtr label, CharPtr name, AsnTypePtr atp, OMNewFunc newfunc,
		AsnReadFunc asnread, AsnWriteFunc asnwrite, OMFreeFunc freefunc,
		OMLabelFunc labelfunc, OMSubTypeFunc subtypefunc)
{
	ObjMgrType omt;
	Uint2 newtype;

	MemSet((Pointer)(&omt), 0, sizeof(ObjMgrType));
	omt.datatype = type;
	omt.asnname = asnname;
	omt.label = label;
	omt.name = name;
	omt.atp = atp;
	omt.newfunc = newfunc;
	omt.asnread = asnread;
	omt.asnwrite = asnwrite;
	omt.freefunc = freefunc;
	omt.labelfunc = labelfunc;
	omt.subtypefunc = subtypefunc;

	newtype = ObjMgrTypeAdd(&omt);

	return newtype;
}

/*****************************************************************************
*
*   ObjMgrTypeLookup(omp, data)
*   	Binary lookup of data in omp->typelist
*   	returns index (>=0) if found
*       returns -1 if not found
*
*****************************************************************************/
NLM_EXTERN Int4 LIBCALL ObjMgrTypeLookup(ObjMgrPtr omp, Uint2 data)
{
	Int4 imin, imax, i;
	ObjMgrTypePtr PNTR omdpp;
	Uint2 tmp;

	imin = 0;
	imax = omp->currtype - 1;
	omdpp = omp->typelist;

	while (imax >= imin)
	{
		i = (imax + imin)/2;
		tmp = omdpp[i]->datatype;
		if (tmp > data)
			imax = i - 1;
		else if (tmp < data)
			imin = i + 1;
		else
			return i;
	}

	return (Int4)(-1);
}

/*****************************************************************************
*
*   ObjMgrTypeFind(omp, type, asnname, name)
*   	returns the objmgrptr by looking for
*   		type: if type != 0
*   		asnname: if asnname != NULL
*   		name: if name != NULL
*       in that order of preference.
*       if omp == NULL, then does temporary read lock in this function
*   	returns NULL if can't match on highest priority key
*
*****************************************************************************/
NLM_EXTERN ObjMgrTypePtr LIBCALL ObjMgrTypeFind (ObjMgrPtr tomp, Uint2 type, CharPtr asnname, CharPtr name)
{
	ObjMgrTypePtr omtp = NULL;
	ObjMgrTypePtr PNTR omdpp;
	Int4 i, imax, result;
	ObjMgrPtr omp;
	Boolean unlock = FALSE;

	if (tomp == NULL)
	{
		omp = ObjMgrReadLock();
		unlock = TRUE;
	}
	else
		omp = tomp;

	omdpp = omp->typelist;
	imax = omp->currtype;
	if (type)
	{
		i = ObjMgrTypeLookup(omp, type);
		if (i >= 0)
			omtp = omdpp[i];
	}
	else
	{
		for (i = 0; i < imax; i++)
		{
			if (asnname != NULL)
				result = StringCmp(omdpp[i]->asnname, asnname);
			else
				result = StringCmp(omdpp[i]->name, name);
			if (! result)
			{
				omtp = omdpp[i];
				break;
			}
		}
	}

	if (unlock)
		ObjMgrUnlock();

	return omtp;
}

/*****************************************************************************
*
*   ObjMgrTypeSetLabelFunc(type, labelfunc)
*   	replaces the labelfunc for type with a new one
*   	can also set it for the first time
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrTypeSetLabelFunc(Uint2 type, OMLabelFunc labelfunc)
{
	ObjMgrPtr omp;
	ObjMgrTypePtr omtp;
	Boolean retval = FALSE;

	omp = ObjMgrWriteLock();
	omtp = ObjMgrTypeFind(omp, type, NULL, NULL);
	if (omtp != NULL)
	{
		omtp->labelfunc = labelfunc;
		retval = TRUE;
	}
	ObjMgrUnlock();
	return retval;
}

/**************************************************************************
*
* ObjMgrTypeFindNext(omp, omtp)
*    returns next ObjMgrType after omtp
*    Exhaustively traverses registered types if omtp starts as NULL
*
***************************************************************************/		
NLM_EXTERN ObjMgrTypePtr LIBCALL ObjMgrTypeFindNext (ObjMgrPtr omp, ObjMgrTypePtr last)
{
	ObjMgrTypePtr PNTR omdpp;
	Int4 i, imax;
	Boolean got_it = FALSE;

	omdpp = omp->typelist;
	imax = omp->currtype;
	if (last == NULL)   /* take the first one */
		got_it = TRUE;
	for (i = 0; i < imax; i++)
	{
		if (got_it)
			return omdpp[i];
			
		if (omdpp[i] == last)
			got_it = TRUE;
	}
	
	return NULL;
}

/*****************************************************************************
*
*   Selection Functions for data objects
*
*****************************************************************************/
static Boolean NEAR ObjMgrSendSelMsg (ObjMgrPtr omp, Uint2 entityID,
         Uint2 itemID, Uint2 itemtype, Uint1 regiontype, Pointer region)
{
        ObjMgrDataPtr omdp;
        OMMsgStruct ommds;

        omdp = ObjMgrFindByEntityID(omp, entityID, NULL);
        if (omdp == NULL)
           return FALSE;

        MemSet((Pointer)(&ommds), 0, sizeof(OMMsgStruct));
        ommds.message = OM_MSG_SELECT;
        ommds.entityID = entityID;
        ommds.itemtype = itemtype;
        ommds.itemID = itemID;
        ommds.regiontype = regiontype;
        ommds.region = region;

        ObjMgrSendStructMsgFunc(omp, omdp, &ommds);
	return TRUE;
}

static Boolean NEAR ObjMgrSendDeSelMsg (ObjMgrPtr omp, Uint2 entityID,
         Uint2 itemID, Uint2 itemtype, Uint1 regiontype, Pointer region)
{
        ObjMgrDataPtr omdp;
        OMMsgStruct ommds;

        omdp = ObjMgrFindByEntityID(omp, entityID, NULL);
        if (omdp == NULL)
           return FALSE;

        MemSet((Pointer)(&ommds), 0, sizeof(OMMsgStruct));
        ommds.message = OM_MSG_DESELECT;
        ommds.entityID = entityID;
        ommds.itemtype = itemtype;
        ommds.itemID = itemID;
        ommds.regiontype = regiontype;
        ommds.region = region;

	ObjMgrSendStructMsgFunc(omp, omdp, &ommds);
	return TRUE;
}

static SelStructPtr NEAR ObjMgrAddSelStruct (ObjMgrPtr omp, Uint2 entityID,
		 Uint2 itemID, Uint2 itemtype, Uint1 regiontype, Pointer region)
{
	SelStructPtr ssp, tmp;

	tmp = omp->sel;

	if (tmp != NULL)
	{
		while (tmp->next != NULL)
			tmp = tmp->next;
	}

	ssp = MemNew(sizeof(SelStruct));
	if (tmp != NULL)
	{
		tmp->next = ssp;
		ssp->prev = tmp;
	}
	else
		omp->sel = ssp;

	ssp->entityID = entityID;
	ssp->itemID = itemID;
	ssp->itemtype = itemtype;
	ssp->regiontype = regiontype;
	ssp->region = region;

	return ssp;
}


static Boolean NEAR ObjMgrDeSelectStructFunc (ObjMgrPtr omp, SelStructPtr ssp)
{
	SelStructPtr next, prev;

	if (ssp == NULL) 
		return FALSE;

	next = ssp->next;
	prev = ssp->prev;
	if (prev != NULL)
		prev->next = next;
	else
		omp->sel = next;
	if (next != NULL)
		next->prev = prev;

        ObjMgrSendDeSelMsg (omp, ssp->entityID, ssp->itemID, ssp->itemtype, ssp->regiontype, ssp->region);

	ObjMgrRegionFree(omp, ssp->regiontype, ssp->region); /* free any region */

	MemFree(ssp);

	return TRUE;
}

static Boolean NEAR ObjMgrDeSelectAllFunc (ObjMgrPtr omp)
{
	SelStructPtr tmp, prev;

	tmp = omp->sel;
	if (tmp == NULL)
		return TRUE;

	prev = NULL;

	while (tmp->next != NULL)
	{
		prev = tmp;
		tmp = tmp->next;
	}

	while (tmp != NULL)
	{
		ObjMgrDeSelectStructFunc(omp, tmp);
		tmp = prev;
		if (tmp != NULL)
			prev = tmp->prev;
	}

	return TRUE;
}

static Boolean NEAR ObjMgrRegionMatch (Uint1 regiontype1, Pointer region1,
					Uint1 regiontype2, Pointer region2)
{
	SeqLocPtr slp;
	SeqIntPtr sip1, sip2;

	if (regiontype1 != regiontype2) return FALSE;
	if (! regiontype1) return FALSE;
	if ((region1 == NULL) || (region2 == NULL))	 return FALSE;

	switch (regiontype1)
	{
		case OM_REGION_SEQLOC:    /* SeqLocs of type SeqInt */
			slp = (SeqLocPtr)region1;
			sip1 = (SeqIntPtr)(slp->data.ptrvalue);
			slp = (SeqLocPtr)(region2);
			sip2 = (SeqIntPtr)(slp->data.ptrvalue);
			if (sip1 == NULL || sip2 == NULL) return FALSE;
			if ( ((sip1->from == sip2->from) &&
				(sip1->to == sip2->to)))
                       /* get rid of (sip1->strand == sip2->strand) */
					return TRUE;
			break;
		default:
			break;
	}
	return FALSE;	
}

static Pointer NEAR ObjMgrRegionCopy (ObjMgrPtr omp, Uint1 regiontype, Pointer region)
{
	Pointer newregion = NULL;

	if (region != NULL)
	{
		switch (regiontype)
		{
			case OM_REGION_SEQLOC:
				newregion = ObjMgrMemCopyFunc(omp, OBJ_SEQLOC, region, FALSE);
				break;
			default:
				break;
		}
	}
	return newregion;
}

static Pointer NEAR ObjMgrRegionFree (ObjMgrPtr omp, Uint1 regiontype, Pointer region)
{
	if (region != NULL)
	{
		switch (regiontype)
		{
			case OM_REGION_SEQLOC:
				ObjMgrFreeFunc(omp, OBJ_SEQLOC, region, FALSE);
				break;
			default:
				break;
		}
	}

	return NULL;
}

/*****************************************************************************
*
*   ObjMgrSelect(entityID, itemID, itemtype)
*      if entityID == 0, just deselects everything
*      if entityID,itemID, itemtype already selected, deselects everything else
*      if something else selected, deselects it first, then selects requested
*        item
*      returns TRUE if item is now currently selected, FALSE if not
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrSelect (Uint2 entityID, Uint2 itemID, Uint2 itemtype,
									Uint1 regiontype, Pointer region)
{
	ObjMgrPtr omp;
	Boolean retval = FALSE;

	omp = ObjMgrWriteLock();
	retval = ObjMgrSelectFunc(omp, entityID, itemID, itemtype,
		        regiontype, region);
	ObjMgrUnlock();
	return retval;
}


/*****************************************************************************
*
*   ObjMgrSelect(entityID, itemID, itemtype)
*      if entityID == 0, just deselects everything
*      if entityID,itemID, itemtype already selected, deselects everything else
*      if something else selected, deselects it first, then selects requested
*        item
*      returns TRUE if item is now currently selected, FALSE if not
*
*****************************************************************************/
static Boolean NEAR ObjMgrSelectFunc (ObjMgrPtr omp, Uint2 entityID, Uint2 itemID, Uint2 itemtype,
									Uint1 regiontype, Pointer region)
{
	SelStructPtr ssp, next;
	Boolean was_selected = FALSE, is_selected, remove_this, retval = TRUE;


	if (entityID == 0)    /* desktop */
	{
		ObjMgrDeSelectAllFunc(omp);
		goto erret;
	}
	else
	{
		ssp = omp->sel;
		while (ssp != NULL)
		{
			remove_this = TRUE;
			next = ssp->next;
			if (ssp->entityID == entityID)
			{
				if ((ssp->itemID == itemID) || (! itemID))
				{
					if ((ssp->itemtype == itemtype) || (! itemtype))
					{
						if ((ssp->regiontype == regiontype) || (! regiontype))
						{
							is_selected = TRUE;
							if (regiontype)
								is_selected = ObjMgrRegionMatch(ssp->regiontype,
									ssp->region, regiontype, region);
							if (is_selected)
							{
								remove_this = FALSE;
								was_selected = TRUE;
							}
						}
					}
				}
			}
			if (remove_this)
				ObjMgrDeSelectStructFunc(omp, ssp);

			ssp = next;
		}
	}

	if (was_selected)  /* no more action needed */
	{
		retval = TRUE;
	}
	else
	{
		ssp = ObjMgrAddSelStruct(omp, entityID, itemID, itemtype, regiontype, region);

		if (ssp != NULL)
			retval = ObjMgrSendSelMsg(omp, entityID, itemID, itemtype, regiontype, region);;
	}

erret:
	return retval;

}

static SeqLocPtr SeqLocChangeIntervalle (SeqLocPtr slp, Int4 start, Int4 stop)
{
        SeqIntPtr sip;

        sip = (SeqIntPtr)(slp->data.ptrvalue);
        if (start > -1)
                sip->from = start;
        if (stop > -1)
                sip->to = stop;
        return slp;
}

static Int2 NEAR ObjMgrRegionComp (Pointer region1,Pointer region2, Boolean direction2_plus)
{
        SeqLocPtr slp1, slp2;
        Int2      res;

        if ((region1 == NULL) || (region2 == NULL))      
           return FALSE;

        slp1 = (SeqLocPtr)region1;
        slp2 = (SeqLocPtr)region2;
        res = SeqLocCompare (slp1,slp2);
        if (res == SLC_NO_MATCH)
        {
           if (SeqLocStop(slp1) == SeqLocStart(slp2)-1)
              return 2;
           if (SeqLocStart(slp1) == SeqLocStop(slp2)+1)
              return 3;
           return 0;
        }
        if (res == SLC_A_EQ_B)
           return 1;
        if (res == SLC_B_IN_A)
        {
	   if (SeqLocStart(slp1)==SeqLocStart(slp2))
              return 5;
           if (SeqLocStop(slp1)==SeqLocStop(slp2))
              return 6;
           if (direction2_plus)
              return 5;
           else
              return 6;
        }
        if (res == SLC_A_IN_B)
           return 4;
        if (res == SLC_A_OVERLAP_B)
        {
           if (SeqLocStart(slp1) < SeqLocStart(slp2))
              return 2;
           if (SeqLocStart(slp1) > SeqLocStart(slp2))
              return 3;
        }
        return 0;
}


static Boolean NEAR ObjMgrSelectFunc2 
(ObjMgrPtr omp, Uint2 entityID, Uint2 itemID, Uint2 itemtype, Uint1 regiontype, Pointer region)
{
        SelStructPtr ssp, next;
        Boolean      retval = FALSE;
	SeqLocPtr    tmp = NULL,
                     tmp2 = NULL;
        Int2         res=0;
	Boolean direction_plus = TRUE;

        if (entityID == 0)    /* desktop */
        {
                ObjMgrDeSelectAllFunc(omp);
                goto erret;
        }
        else if (region)
        {
/***** Colombe Patrick ****/
/** send Seq_strand_minus seqloc of selection when the cursor moves to the left **/
/** the Seq_strand_minus is now change to Seq_strand_plus     for ever **/

           if (SeqLocStrand(region)==Seq_strand_minus)
	   {
SeqIntPtr sit;
SeqLocPtr slp;
      	      slp = (SeqLocPtr)region;
	      sit = (SeqIntPtr)(slp->data.ptrvalue);
	      sit->strand = Seq_strand_plus;
              direction_plus=FALSE;
	   }
/***** Colombe Patrick ****/
                ssp = omp->sel;
                while (ssp != NULL && res == 0)
                {
                        next = ssp->next;
                        if (ssp->entityID == entityID)
                        {
                                if ((ssp->itemID == itemID) || (! itemID))
                                {
                                        if ((ssp->itemtype == itemtype) || (! itemtype))
                                        {
                                                if (ssp->regiontype && regiontype)
                                                {
                                                        res = ObjMgrRegionComp (ssp->region, region, direction_plus);
							if (res>0)
								break;
                                                }
                                        }
                                }
                        }
                        ssp = next;
                }
        }
        if (res==2)  /* extend ssp->region to right */
        {
                SeqLocChangeIntervalle ((SeqLocPtr)ssp->region, -1, SeqLocStop(region));
                retval=ObjMgrSendSelMsg (omp, entityID, itemID, itemtype, ssp->regiontype, ssp->region);
        }
        else if (res==3)   /* extend ssp->region to left */
        {
                SeqLocChangeIntervalle ((SeqLocPtr)ssp->region, SeqLocStart(region), -1);
                retval=ObjMgrSendSelMsg (omp, entityID, itemID, itemtype, ssp->regiontype, ssp->region);
        }
        else if (res==4)   /* extend ssp->regiontype both sides */
        {
                SeqLocChangeIntervalle ((SeqLocPtr)ssp->region, SeqLocStart(region), SeqLocStop(region));
                retval=ObjMgrSendSelMsg (omp, entityID, itemID, itemtype, ssp->regiontype, ssp->region);
        }
        else if (res==5)  /* shrink ssp->region from right */
        {
                tmp = SeqLocIntNew (SeqLocStart((SeqLocPtr)ssp->region), SeqLocStop((SeqLocPtr)region), 0, SeqLocId(region));
                tmp2 = SeqLocIntNew (SeqLocStop((SeqLocPtr)region)+1, SeqLocStop((SeqLocPtr)ssp->region), 0, SeqLocId(region));
                SeqLocChangeIntervalle ((SeqLocPtr)ssp->region, -1, (Int4)(SeqLocStop(region)));
		ValNodeFree (tmp);
                retval=ObjMgrSendDeSelMsg (omp, entityID, itemID, itemtype, OM_REGION_SEQLOC, tmp2);

		ValNodeFree (tmp2);
        }
        else if (res==6) /* shrink ssp->region from left */
        {
                tmp = SeqLocIntNew (SeqLocStart((SeqLocPtr)region), SeqLocStop((SeqLocPtr)ssp->region), 0, SeqLocId(region));
                tmp2 = SeqLocIntNew (SeqLocStart((SeqLocPtr)ssp->region), SeqLocStart((SeqLocPtr)region)-1, 0, SeqLocId(region));
                SeqLocChangeIntervalle ((SeqLocPtr)ssp->region, (Int4)(SeqLocStart(region)), -1);
		ValNodeFree (tmp);
                retval=ObjMgrSendDeSelMsg (omp, entityID, itemID, itemtype, OM_REGION_SEQLOC, tmp2);
		ValNodeFree (tmp2);
        }
        else if (res==0)
        {
                ssp = ObjMgrAddSelStruct(omp, entityID, itemID, itemtype, regiontype, region);
                if (ssp != NULL)
                        retval=ObjMgrSendSelMsg (omp, entityID, itemID, itemtype, regiontype, region);
        }

erret:
        return (Boolean)(res>0);

}

static Boolean NEAR CheckRedondantSelect (ObjMgrPtr omp)
{
        SelStructPtr ssp1, 
        		ssp2,
        		next1,
        		next2, pre2;
        SeqLocPtr    tmp;
        Boolean      retval = FALSE;
        Int2         res=0;

        if (omp!=NULL)   
        {
                ssp1 = omp->sel;
                while (ssp1 != NULL && res==0)
                {
                   next1 = ssp1->next;
                   ssp2=next1;
                   pre2=ssp1;
                   while (ssp2!=NULL && res==0)
                   {
                   	next2 = ssp2->next;
                        if (ssp1->entityID == ssp2->entityID)
                        {
                                if (ssp1->itemID == ssp2->itemID)
                                {
                                        if (ssp1->itemtype == ssp2->itemtype)
                                        {
                                                if (ssp1->regiontype && ssp2->regiontype)
                                                {
                                                        res = ObjMgrRegionComp (ssp1->region, ssp2->region, TRUE);
							if (res>0)
								break;
                                                }
                                        }
                                }
                        }
                        ssp2 = next2;
                   }
                   if (res==0)
			ssp1 = next1;
                }
        }
	if (res==1)
	{
       		pre2->next=next2;
       		ssp2=MemFree(ssp2);
                ObjMgrSendSelMsg (omp, ssp1->entityID, ssp1->itemID, ssp1->itemtype, 0, 0);
                retval=TRUE;
	}
        else if (res==2)  /* extend ssp->regiontype to right */
        {
                SeqLocChangeIntervalle ((SeqLocPtr)ssp1->region, -1, SeqLocStop(ssp2->region));
       		pre2->next=next2;
       		ssp2=MemFree(ssp2);                
                ObjMgrSendSelMsg (omp, ssp1->entityID, ssp1->itemID, ssp1->itemtype, 0, 0);
                retval=TRUE;
        }
        else if (res==3)   /* extend ssp->regiontype to left */
        {
                SeqLocChangeIntervalle ((SeqLocPtr)ssp1->region, SeqLocStart(ssp2->region), -1);
       		pre2->next=next2;
       		ssp2=MemFree(ssp2);                
                ObjMgrSendSelMsg (omp, ssp1->entityID, ssp1->itemID, ssp1->itemtype, 0, 0);
                retval=TRUE;
        }
        else if (res==4)   /* extend ssp->regiontype both sides */
        {
                SeqLocChangeIntervalle ((SeqLocPtr)ssp1->region, SeqLocStart(ssp2->region), SeqLocStop(ssp2->region));
       		pre2->next=next2;
       		ssp2=MemFree(ssp2);                
                ObjMgrSendSelMsg (omp, ssp1->entityID, ssp1->itemID, ssp1->itemtype, 0, 0);
                retval=TRUE;
        }
        else if (res==5)  /* shrink ssp->region from right */
        {
		tmp = SeqLocIntNew (SeqLocStart((SeqLocPtr)ssp1->region), SeqLocStop(ssp2->region), 0, SeqLocId(ssp2->region));
                SeqLocChangeIntervalle ((SeqLocPtr)ssp1->region, -1, SeqLocStop(ssp2->region));
       		pre2->next=next2;
       		ssp2=MemFree(ssp2);                
                ObjMgrSendDeSelMsg (omp, ssp1->entityID, ssp1->itemID, ssp1->itemtype, 0, 0);
		ValNodeFree (tmp);
                retval=TRUE;
        }
        else if (res==6) /* shrink ssp->region from left */
        {
                tmp = SeqLocIntNew (SeqLocStart(ssp2->region), SeqLocStop((SeqLocPtr)ssp1->region), 0, SeqLocId(ssp2->region));
                SeqLocChangeIntervalle ((SeqLocPtr)ssp1->region, SeqLocStart(ssp2->region), -1);
       		pre2->next=next2;
       		ssp2=MemFree(ssp2);                
                ObjMgrSendDeSelMsg (omp, ssp1->entityID, ssp1->itemID, ssp1->itemtype, 0, 0);
		ValNodeFree (tmp);
                retval=TRUE;
        }
        return retval;
}



/*****************************************************************************
*
*   ObjMgrSetColor(entityID, itemID, itemtype, regiontype, region, rgb);
*      Sets color of object in displays
*      if regiontype != 0, and restricts to region
*      rgb is a pointer to a Uint1[3] array containing an RGB value.
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrSetColor (Uint2 entityID, Uint2 itemID,
                                        Uint2 itemtype, Uint1 regiontype,
                                        Pointer region, Uint1Ptr rgb)
{
	ObjMgrPtr omp;
	ObjMgrDataPtr omdp;
	OMMsgStruct ommds;
	Boolean retval = FALSE;

	omp = ObjMgrReadLock();

	if (rgb == NULL) goto erret;

	omdp = ObjMgrFindByEntityID(omp, entityID, NULL);
	if (omdp == NULL) goto erret;

	MemSet((Pointer)(&ommds), 0, sizeof(OMMsgStruct));
	ommds.message = OM_MSG_SETCOLOR;
	ommds.entityID = entityID;
	ommds.itemtype = itemtype;
	ommds.itemID = itemID;
	ommds.regiontype = regiontype;
	ommds.region = region;
        ommds.rgb[0] = rgb[0];
        ommds.rgb[1] = rgb[1];
        ommds.rgb[2] = rgb[2];

                                /* send the message */
	ObjMgrSendStructMsgFunc(omp, omdp, &ommds);
                                /* free the region */
	retval = TRUE;
erret:
	ObjMgrUnlock();

	return retval;
}
/*****************************************************************************
*
*   ObjMgrDeSelect(entityID, itemID, itemtype)
*   	if this item was selected, then deselects and returns TRUE
*   	else returns FALSE
*
*****************************************************************************/

NLM_EXTERN Boolean LIBCALL ObjMgrDeSelect (Uint2 entityID, Uint2 itemID, Uint2 itemtype,
									Uint1 regiontype, Pointer region)
{
	ObjMgrPtr omp;
	Boolean retval=FALSE;

	omp = ObjMgrWriteLock();
	retval = ObjMgrDeSelectFunc (omp, entityID, itemID, itemtype,
				regiontype, region);
	ObjMgrUnlock();
	return retval;
}


/*****************************************************************************
*
*   ObjMgrDeSelect(entityID, itemID, itemtype)
*   	if this item was selected, then deselects and returns TRUE
*   	else returns FALSE
*
*****************************************************************************/

static Boolean NEAR ObjMgrDeSelectFunc (ObjMgrPtr omp, Uint2 entityID, Uint2 itemID, Uint2 itemtype,
									Uint1 regiontype, Pointer region)
{
	SelStructPtr tmp, next;
	Boolean retval=FALSE, tret, do_it;
	SeqLocPtr slp;

	if (entityID == 0)
	{
		retval = ObjMgrDeSelectAllFunc(omp);
		goto erret;
	}

	tmp = omp->sel;
	while (tmp != NULL)
	{
		next = tmp->next;
		do_it = FALSE;
		if (tmp->entityID == entityID)
		{
			if ((! itemtype) || (itemtype == tmp->itemtype))
			{
				if ((! itemID) || (itemID == tmp->itemID))
				{
					if ((tmp->regiontype == regiontype) || (! regiontype))
					{
						do_it = TRUE;
						if (regiontype)
							do_it = ObjMgrRegionMatch(tmp->regiontype,
								tmp->region, regiontype, region);
						if (do_it)
						{
							tret = ObjMgrDeSelectStructFunc(omp, tmp);
							if (tret)
								retval = TRUE;
						}
                                                else if (SeqLocCompare(tmp->region, region)==SLC_B_IN_A)
						{
							if (SeqLocStart(tmp->region)==SeqLocStart(region))
							{
                						SeqLocChangeIntervalle ((SeqLocPtr)tmp->region, (Int4)(SeqLocStop(region)+1), -1);
                						retval=ObjMgrSendDeSelMsg (omp, entityID, itemID, itemtype, OM_REGION_SEQLOC, region);
							}
							else if (SeqLocStop(tmp->region)==SeqLocStop(region))
							{
								SeqLocChangeIntervalle ((SeqLocPtr)tmp->region, -1, (Int4)(SeqLocStart(region)-1));
								retval=ObjMgrSendDeSelMsg (omp, entityID, itemID, itemtype, OM_REGION_SEQLOC, region);
							}
							else {
								slp=SeqLocIntNew(SeqLocStop(region)+1, SeqLocStop((SeqLocPtr)tmp->region), 0, SeqLocId(tmp->region));
				 				SeqLocChangeIntervalle ((SeqLocPtr)tmp->region, -1, (Int4)(SeqLocStart(region)-1));
                						ObjMgrAddSelStruct(omp, entityID, itemID, itemtype, regiontype, slp);
                                                                retval=ObjMgrSendDeSelMsg (omp, entityID, itemID, itemtype, OM_REGION_SEQLOC, region);

							}
						}
					}
				}
			}
		}
		tmp = next;
	}

erret:
	return retval;
}

NLM_EXTERN Boolean LIBCALL ObjMgrAlsoSelect (Uint2 entityID, Uint2 itemID,
					 Uint2 itemtype, Uint1 regiontype, Pointer region)
{
	ObjMgrPtr omp;
	Boolean retval;
				/* if already selected, just deselect */
/**
	if (ObjMgrDeSelect(entityID, itemID, itemtype, regiontype, region))
		return FALSE;
**/
	omp = ObjMgrWriteLock();

	retval = ObjMgrSelectFunc2 (omp, entityID, itemID, itemtype, regiontype, region);
	if (retval) {
		while (retval)
			retval=CheckRedondantSelect (omp);
	}
	ObjMgrUnlock();

        return retval;

}

NLM_EXTERN Boolean LIBCALL ObjMgrDeSelectAll (void)
{
	ObjMgrPtr omp;
	Boolean retval = FALSE;

	omp = ObjMgrWriteLock();
	retval = ObjMgrDeSelectAllFunc(omp);
	ObjMgrUnlock();
	return retval;
}

NLM_EXTERN SelStructPtr LIBCALL ObjMgrGetSelected (void)
{
	ObjMgrPtr omp;
	SelStructPtr sel;

	omp = ObjMgrReadLock();
	sel = omp->sel;
	ObjMgrUnlock();
	return sel;
}

/*****************************************************************************
*
*   ObjMgrSendMsg(msg, entityID, itemID, itemtype)
*       Directly invokes the objmgr messaging system
*       should be used cautiously
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL ObjMgrSendMsg(Uint2 msg, Uint2 entityID, Uint2 itemID, Uint2 itemtype)
{
	ObjMgrPtr omp;
	ObjMgrDataPtr omdp;
	Boolean retval = FALSE;

	if (msg == OM_MSG_UPDATE) {
		SeqMgrClearFeatureIndexes (entityID, NULL);
	}
	omp = ObjMgrReadLock();
	omdp = ObjMgrFindByEntityID(omp, entityID, NULL);
	if (omdp != NULL)
	{
		ObjMgrSendMsgFunc(omp, omdp, msg, entityID, itemID, itemtype, 0, 0, 0, NULL);
		retval = TRUE;
	}
	ObjMgrUnlock();
	return retval;
}

NLM_EXTERN Boolean LIBCALL ObjMgrSendProcMsg(Uint2 msg, Uint2 entityID, Uint2 itemID, Uint2 itemtype,
                                             Uint2 fromProcID, Uint2 toProcID, Pointer procmsgdata)
{
	ObjMgrPtr omp;
	ObjMgrDataPtr omdp;
	Boolean retval = FALSE;

	omp = ObjMgrReadLock();
	omdp = ObjMgrFindByEntityID(omp, entityID, NULL);
	if (omdp != NULL)
	{
		ObjMgrSendMsgFunc(omp, omdp, msg, entityID, itemID, itemtype, 0, fromProcID, toProcID, procmsgdata);
		retval = TRUE;
	}
	ObjMgrUnlock();
	return retval;
}

NLM_EXTERN Boolean LIBCALL ObjMgrSendRowMsg(Uint2 msg, Uint2 entityID, Uint2 itemID, Uint2 itemtype, Uint2 rowID)
{
	ObjMgrPtr omp;
	ObjMgrDataPtr omdp;
	Boolean retval = FALSE;

	omp = ObjMgrReadLock();
	omdp = ObjMgrFindByEntityID(omp, entityID, NULL);
	if (omdp != NULL)
	{
		ObjMgrSendMsgFunc(omp, omdp, msg, entityID, itemID, itemtype, rowID, 0, 0, NULL);
		retval = TRUE;
	}
	ObjMgrUnlock();
	return retval;
}

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
*                 it registers the data with the object manager
*                 it fills in entityIDptr with the entityID of the loaded object
*
*****************************************************************************/
NLM_EXTERN Pointer LIBCALL ObjMgrGenericAsnTextFileRead (CharPtr filename,
                                   Uint2Ptr datatypeptr, Uint2Ptr entityIDptr)
{
	Char line[255];
	FILE * fp;
	AsnIoPtr aip;
	Pointer ptr = NULL;
	Int2 ct, i;
	ObjMgrPtr omp;
	ObjMgrTypePtr omtp = NULL;

	if (filename == NULL)
		return ptr;

	if (datatypeptr != NULL) *datatypeptr = 0;
	if (entityIDptr != NULL) *entityIDptr = 0;

	fp = FileOpen(filename, "r");
	ct = FileRead(line, 1, 255, fp);
	for (i = 0; i < ct; i++)
	{
		if (line[i] == ':')
		{
			if ((line[i+1] == ':') && (line[i+2] == '='))
			{
				line[i] = '\0';
				i--;
				while ((i >= 0) && (IS_WHITESP(line[i])))
				{
					line[i] = '\0';
					i--;
				}
				while ((i >= 0) && (! IS_WHITESP(line[i])))
					i--;
				omp = ObjMgrReadLock();
				omtp = ObjMgrTypeFind(omp, 0, &(line[i+1]), NULL);
				ObjMgrUnlock();
				if (omtp == NULL)
				{
					FileClose(fp);
					ErrPostEx(SEV_ERROR, 0,0, "Can't read ASN.1 type [%s]",
						&(line[i+1]));
					return ptr;
				}
				break;
			}
		}
	}
	
	if (omtp == NULL)
	{
		FileClose(fp);
		ErrPostEx(SEV_ERROR,0,0,"Don't know how to read file [%s]", filename);
		return ptr;
	}
	
	fseek(fp, (long)(i+1), SEEK_SET);
	aip = AsnIoNew(ASNIO_TEXT_IN, fp, NULL, NULL, NULL);
	if (aip != NULL)
		aip->fname = StringSave(filename);
	ptr = (*(omtp->asnread))(aip, NULL);
	AsnIoClose(aip);
	if (ptr == NULL)
	{
		ErrPostEx(SEV_ERROR,0,0,"Couldn't read [%s], type [%s]", filename, omtp->asnname);
	}
	else
	{
		if (datatypeptr != NULL)
			*datatypeptr = omtp->datatype;
		if (entityIDptr != NULL)
			*entityIDptr = ObjMgrRegister(omtp->datatype, ptr);
	}
	
	return ptr;
}

/***********************************************************************************
*
*  ObjMgrMemCopy(type, ptr)
*    Uses AsnIoMemCopy to make a copy of any ObjMgr supported type
*    ObjMgrType for "type" must have been previously loaded
*
***********************************************************************************/
NLM_EXTERN Pointer LIBCALL ObjMgrMemCopy (Uint2 type, Pointer ptr)
{
	Pointer newptr = NULL;
	ObjMgrPtr omp;

	if (ptr == NULL) return newptr;

 	omp = ObjMgrReadLock();
	newptr = ObjMgrMemCopyFunc(omp, type, ptr, TRUE);

	return newptr;
}

static Pointer NEAR ObjMgrMemCopyFunc (ObjMgrPtr omp, Uint2 type, Pointer ptr, Boolean unlock)
{
	Pointer newptr = NULL;
	ObjMgrTypePtr omtp;

	if (ptr == NULL) {

            if (unlock)
		ObjMgrUnlock();
            
            return newptr;
        }

	omtp = ObjMgrTypeFind(omp, type, NULL, NULL);
	if (unlock)
		ObjMgrUnlock();
	if (omtp == NULL) return newptr;

	newptr = AsnIoMemCopy(ptr, omtp->asnread, omtp->asnwrite);

	return newptr;
}

/***********************************************************************************
*
*  ObjMgrFree(type, ptr)
*    ObjMgrType for "type" must have been previously loaded
*
***********************************************************************************/
NLM_EXTERN Pointer LIBCALL ObjMgrFree (Uint2 type, Pointer ptr)
{
	Pointer newptr = NULL;
	ObjMgrPtr omp;

	if (ptr == NULL) return newptr;

 	omp = ObjMgrReadLock();
	newptr = ObjMgrFreeFunc(omp, type, ptr, TRUE);

	return newptr;
}

NLM_EXTERN Pointer LIBCALL ObjMgrFreeByEntityID (Uint2 entityID)
{
	ObjMgrDataPtr omdp;
	Uint2 type;
	Pointer ptr;

	if (entityID < 1) return NULL;
	omdp = ObjMgrGetData (entityID);
	if (omdp == NULL) return NULL;

	if (omdp->choice != NULL) {
		type = omdp->choicetype;
		ptr = omdp->choice;
	} else {
		type = omdp->datatype;
		ptr = omdp->dataptr;
	}

	return ObjMgrFree (type, ptr);
}

static Pointer NEAR ObjMgrFreeFunc (ObjMgrPtr omp, Uint2 type, Pointer ptr, Boolean unlock)
{
	Pointer newptr = NULL;
	ObjMgrTypePtr omtp;

	if (ptr == NULL) {
            if (unlock)
		ObjMgrUnlock();
            return newptr;
        }

	omtp = ObjMgrTypeFind(omp, type, NULL, NULL);

	if (unlock)
		ObjMgrUnlock();
	if (omtp == NULL) return newptr;
	(*(omtp->freefunc))(ptr);

	return newptr;
}

NLM_EXTERN void LIBCALL ObjMgrResetAll (void)
{
    Int4 ret;
    
    if (global_omp == NULL)
        return;
    
    ret = NlmMutexLockEx(&omp_mutex);  /* protect this section */
    if (ret) { /* error */
        ErrPostEx(SEV_FATAL,0,0,"ObjMgrResetAll failed [%ld]", (long)ret);
        return;
    }
    
    global_omp = (ObjMgrPtr) MemFree(global_omp);
    NlmRWdestroy(omp_RWlock);
    omp_RWlock = NULL;
    
    NlmMutexUnlock(omp_mutex);
    
    return;
}

static CharPtr objmgrtypestrs [] = {
  "OBJ_ALL", "OBJ_SEQENTRY", "OBJ_BIOSEQ", "OBJ_BIOSEQSET", "OBJ_SEQDESC",
  "OBJ_SEQANNOT", "OBJ_ANNOTDESC", "OBJ_SEQFEAT", "OBJ_SEQALIGN", "OBJ_SEQGRAPH",
  "OBJ_SEQSUB", "OBJ_SUBMIT_BLOCK", "OBJ_SEQSUB_CONTACT", "13", "OBJ_BIOSEQ_MAPFEAT",
  "OBJ_BIOSEQ_SEG", "OBJ_SEQHIST", "OBJ_SEQHIST_ALIGN", "OBJ_BIOSEQ_DELTA", "19",
  "OBJ_PUB", "OBJ_SEQFEAT_CIT", "OBJ_SEQSUB_CIT", "OBJ_MEDLINE_ENTRY", "OBJ_PUB_SET",
  "OBJ_SEQLOC", "OBJ_SEQID", "OBJ_SEQCODE", "OBJ_SEQCODE_SET", "OBJ_GENETIC_CODE",
  "OBJ_GENETIC_CODE_SET", "OBJ_TEXT_REPORT", "OBJ_FASTA", "OBJ_VIBRANT_PICTURE", "OBJ_PROJECT"
};

static CharPtr temploadstrs [] = {
  "TL_NOT_TEMP", "TL_LOADED", "TL_CACHED"
};

static CharPtr proctypestrs [] = {
  "0", "OMPROC_OPEN", "OMPROC_DELETE", "OMPROC_VIEW", "OMPROC_EDIT",
  "OMPROC_SAVE", "OMPROC_CUT", "OMPROC_COPY", "OMPROC_PASTE", "OMPROC_ANALYZE",
  "OMPROC_FIND", "OMPROC_REPLACE", "OMPROC_FILTER", "OMPROC_FETCH",
};

static void PrintABool (FILE *fp, CharPtr str, Boolean val)

{
  if (val) {
    fprintf (fp, "%s TRUE\n", str);
  } else {
    fprintf (fp, "%s FALSE\n", str);
  }
}

static void ReportOnEntity (ObjMgrDataPtr omdp, ObjMgrPtr omp, Boolean selected,
                            Uint2 itemID, Uint2 itemtype, Int2 index, FILE *fp)

{
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  Char           buf [50];
  OMUserDataPtr  omudp;

  if (omdp == NULL || fp == NULL) return;
  if (selected) {
    fprintf (fp, "Data Element\n\n");
    fprintf (fp, "  EntityID %d selected\n", (int) omdp->EntityID);
    fprintf (fp, "  ItemID %d, Itemtype %d\n", (int) itemID, (int) itemtype);
  } else if (omdp->parentptr == NULL) {
    fprintf (fp, "Top Data Element %d\n\n", (int) index);
    fprintf (fp, "  EntityID %d\n", (int) omdp->EntityID);
  } else {
    fprintf (fp, "Inner Data Element %d\n\n", (int) index);
    fprintf (fp, "  EntityID %d\n", (int) omdp->EntityID);
  }
  if (omdp->datatype < OBJ_MAX) {
    fprintf (fp, "  Datatype %s", objmgrtypestrs [omdp->datatype]);
    if (omdp->datatype == OBJ_BIOSEQ) {
      bsp = (BioseqPtr) omdp->dataptr;
      if (bsp != NULL) {
        SeqIdWrite (bsp->id, buf, PRINTID_FASTA_LONG, sizeof (buf) - 1);
        fprintf (fp, " %s, length %ld", buf, (long) bsp->length);
      }
    } else if (omdp->datatype == OBJ_BIOSEQSET) {
      bssp = (BioseqSetPtr) omdp->dataptr;
      if (bssp != NULL) {
        fprintf (fp, " class %d", (int) bssp->_class);
      }
    }
    fprintf (fp, "\n");
  } else {
    fprintf (fp, "  Unregistered datatype %d\n", (int) omdp->datatype);
  }
  fprintf (fp, "  Lockcnt %d\n", (int) omdp->lockcnt);
  if (omdp->tempload < 3) {
    fprintf (fp, "  Tempload %s\n", temploadstrs [omdp->tempload]);
  } else {
    fprintf (fp, "  Unrecognized tempload %d\n", (int) omdp->tempload);
  }
  PrintABool (fp, "  Clipboard", omdp->clipboard);
  PrintABool (fp, "  Dirty", omdp->dirty);
  PrintABool (fp, "  Being_freed", omdp->being_freed);
  PrintABool (fp, "  Free", omdp->free);
  fprintf (fp, "\n");
  for (omudp = omdp->userdata; omudp != NULL; omudp = omudp->next) {
    if (omudp->proctype <= OMPROC_MAX) {
      fprintf (fp, "    Proctype %s\n", proctypestrs [omudp->proctype]);
    } else {
      fprintf (fp, "    Unrecognized proctype %d\n", (int) omudp->proctype);
    }
    fprintf (fp, "    Procid %d\n", (int) omudp->procid);
    fprintf (fp, "    Userkey %d\n", (int) omudp->userkey);
    fprintf (fp, "\n");
  }
}

NLM_EXTERN void LIBCALL ObjMgrReportFunc (CharPtr filename)

{
  FILE           *fp;
  Int2           j;
  Int2           num;
  ObjMgrPtr      omp;
  ObjMgrDataPtr  omdp;
  ObjMgrDataPtr  PNTR omdpp;

  omp = ObjMgrGet ();
  if (omp == NULL) return;
  fp = FileOpen (filename, "w");
  fprintf (fp, "Object Manager\n\n");
  fprintf (fp, "  HighestEntityID %d\n", (int) omp->HighestEntityID);
  fprintf (fp, "  Totobj %d\n", (int) omp->totobj);
  fprintf (fp, "  Currobj %d\n", (int) omp->currobj);
  fprintf (fp, "  Maxtemp %d\n", (int) omp->maxtemp);
  fprintf (fp, "  Tempcnt %d\n", (int) omp->tempcnt);
  fprintf (fp, "  Hold %d\n", (int) omp->hold);
  PrintABool (fp, "  Reaping", omp->reaping);
  PrintABool (fp, "  Is_write_locked", omp->is_write_locked);
  fprintf (fp, "\n");
  num = omp->currobj;
  for (j = 0, omdpp = omp->datalist; j < num && omdpp != NULL; j++, omdpp++) {
    omdp = *omdpp;
    if (omdp->parentptr == NULL) {
      ReportOnEntity (omdp, omp, FALSE, 0, 0, j + 1, fp);
    }
  }
  for (j = 0, omdpp = omp->datalist; j < num && omdpp != NULL; j++, omdpp++) {
    omdp = *omdpp;
    if (omdp->parentptr != NULL) {
      ReportOnEntity (omdp, omp, FALSE, 0, 0, j + 1, fp);
    }
  }
  FileClose (fp);
}

