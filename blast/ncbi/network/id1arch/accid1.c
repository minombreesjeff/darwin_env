#include <seqmgr.h>
#include  <id1arch.h>

#define ID1BFS_DISABLE 0     /* ID1BioseqFetch not in use (default) */
#define ID1BFS_INIT    1     /* ID1BioseqFetchEnable called, but not ID1Init */
#define ID1BFS_READY   2     /* ID1Init has been called */

#define ID1_NUM_RETRY 5

static CharPtr procname[3] = {
        "ID1BioseqFetch",
        "ID1SeqIdForGI",
        "ID1GIForSeqId" };
static Int2 LIBCALLBACK s_ID1BioseqFetchFunc PROTO((Pointer data));
static Int2 LIBCALLBACK s_ID1SeqIdForGIFunc PROTO((Pointer data));
static Int2 LIBCALLBACK s_ID1GIForSeqIdFunc PROTO((Pointer data));

typedef struct ID1Fetch_struct {
        Uint1 	state;
        CharPtr app_name;
	Uint2 	ctr;        /* counts iterations of enable disable */
} ID1FetchStruct, PNTR ID1FetchStructPtr;


Boolean 
ID1Init(void)
{
	return ID1ArchInit();
}

void 
ID1Fini(void)
{
	ID1ArchFini();
}


/**** Look Up a Uid from a SeqId using ID1 lookup ****/
Int4 
ID1FindSeqId(SeqIdPtr sip)
{
	return ID1ArchGIGet(sip);
}

/**** Look Up the source SeqId given a GI ****************/
SeqIdPtr
ID1SeqIdForGI(Int4 gi)
{
	return ID1ArchSeqIdsGet(gi,NULL);
}


SeqEntryPtr 
ID1SeqEntryGet(Int4 gi, Int2 retcode)
{
	SeqEntryPtr sep = NULL;
	ErrStrId errorID;
	char errorString[32];
	Int4 status=0,i;
   
	if (!gi) 
		return NULL;
	sprintf(errorString, "(ID1SeqEntryGet: %d)",gi);
        errorID = Nlm_ErrUserInstall(errorString, 0);

	for(i=0;i<ID1_NUM_RETRY;i++){
		if((sep=ID1ArchSeqEntryGet(gi,NULL,0,&status,retcode)) != NULL){
			break;
		} else {
			if(status & GI_IS_OVERRIDEN /** override **/){
				ErrPostEx(SEV_INFO,0,0,"Sequence is withdrawn");
				goto DONE;
			} else if(status &  GI_IS_CONFIDENTIAL /*** confidential ***/){
				ErrPostEx(SEV_INFO,0,0,"Sequence is not yet available");
                                goto DONE;
			} else {
				ErrPostEx(SEV_INFO,0,0,"Network failure... Reconecting");
			}
		}
	}
	if(sep==NULL){
		ErrPostEx(SEV_WARNING,0,0,"Unable to get the sequence ");
	}

DONE:
	Nlm_ErrUserDelete(errorID);
	return sep;
}

Boolean 
ID1BioseqFetchEnable(CharPtr progname, Boolean now)
{
	Boolean result;
	ID1FetchStructPtr ifsp;
	ObjMgrPtr omp;
	ObjMgrProcPtr ompp;

	/* check if already enabled ***/

        omp = ObjMgrGet();
        ompp = ObjMgrProcFind(omp, 0, procname[0], OMPROC_FETCH);
        if(ompp != NULL){   /* already initialized */
		ifsp = (ID1FetchStructPtr)(ompp->procdata);
	} else {
		ifsp = MemNew(sizeof(ID1FetchStruct));
		ifsp->app_name = StringSave(progname);

                ObjMgrProcLoad(OMPROC_FETCH,procname[0],procname[0],OBJ_SEQID,0,OBJ_BIOSEQ,0,
                        (Pointer)ifsp,s_ID1BioseqFetchFunc, PROC_PRIORITY_DEFAULT);

		ObjMgrProcLoad(OMPROC_FETCH, procname[1],procname[1],OBJ_SEQID,SEQID_GI,OBJ_SEQID,0,
                        (Pointer)ifsp,s_ID1SeqIdForGIFunc,PROC_PRIORITY_DEFAULT);

                ObjMgrProcLoad(OMPROC_FETCH, procname[2],procname[2],OBJ_SEQID,0,OBJ_SEQID,SEQID_GI,
                        (Pointer)ifsp,s_ID1GIForSeqIdFunc,PROC_PRIORITY_DEFAULT);
	}
	ifsp->ctr++;    /* count number of enables */
	if(ifsp->state == ID1BFS_READY)  /* nothing more to do */
		return TRUE;
        if(now){
                if ((result = ID1Init())==FALSE){
                        return result;
                }
                ifsp->state = ID1BFS_READY;
        } else {
		ifsp->state = ID1BFS_INIT;
	}
        return TRUE;
}
void
ID1BioseqFetchDisable(void)
{
	ObjMgrPtr		omp;
	ObjMgrProcPtr		ompp;
	ID1FetchStructPtr	ifsp;

	omp = ObjMgrGet();
	ompp = ObjMgrProcFind(omp,0,procname[0],OMPROC_FETCH);
	if(ompp == NULL)   /* not initialized */
		return;

	ifsp = (ID1FetchStructPtr)(ompp->procdata);
	if(!ifsp || !ifsp->ctr)   /* no enables active */
		return;
	ifsp->ctr--;
	if(ifsp->ctr)   /* connection still pending */
		return;
	if(ifsp->state == ID1BFS_READY)
		ID1Fini();
	ifsp->state = ID1BFS_DISABLE;
        return;
}
/**** Static Functions *****/

/*****************************************************************************
*
*   s_ID1SeqIdForGIFunc(data)
*       callback for ID1SeqIdForGI
*
*****************************************************************************/
static Int2 LIBCALLBACK
s_ID1SeqIdForGIFunc(Pointer data)
{
	OMProcControlPtr ompcp;
	ObjMgrProcPtr ompp;
	ID1FetchStructPtr ifsp;
	SeqIdPtr sip, sip2;

	ompcp = (OMProcControlPtr)data;
	ompp = ompcp->proc;
	ifsp = ompp->procdata;
	if(!ifsp || ifsp->state == ID1BFS_DISABLE){ /* shut off */
		return OM_MSG_RET_OK;    /* not done, go on to next */
        } else if(ifsp->state == ID1BFS_INIT){
                if (!ID1Init())
			return OM_MSG_RET_ERROR;
                ifsp->state = ID1BFS_READY;
        }

        if(ifsp->state != ID1BFS_READY)
		return OM_MSG_RET_ERROR;

        sip = (SeqIdPtr)(ompcp->input_data);
        if(sip == NULL || sip->choice != SEQID_GI)
		return OM_MSG_RET_ERROR;

        if((sip2 = ID1SeqIdForGI(sip->data.intvalue))==NULL)
		return OM_MSG_RET_OK;

        ompcp->output_data = (Pointer)sip2; /* not registering right now */
        return OM_MSG_RET_DONE;   /* data found, don't call further functions */
}

/*****************************************************************************
*
*   s_ID1GIForSeqIdFunc(data)
*       callback for ID1GIForSeqId
*
*****************************************************************************/
static Int2 LIBCALLBACK
s_ID1GIForSeqIdFunc (Pointer data)
{
	OMProcControlPtr	ompcp;
	ObjMgrProcPtr		ompp;
	ID1FetchStructPtr	ifsp;
	SeqIdPtr		sip, sip2;
	Int4			gi;

	ompcp = (OMProcControlPtr)data;
	ompp = ompcp->proc;
        ifsp = ompp->procdata;

	if(!ifsp || ifsp->state == ID1BFS_DISABLE){  /* shut off */
		return OM_MSG_RET_OK;    /* not done, go on to next */
	} else if(ifsp->state == ID1BFS_INIT) {
                if (!ID1Init())
			return OM_MSG_RET_ERROR;
		ifsp->state = ID1BFS_READY;
        }

        if(ifsp->state != ID1BFS_READY)
		return OM_MSG_RET_ERROR;

        if((sip = (SeqIdPtr)ompcp->input_data) == NULL)
		return OM_MSG_RET_ERROR;

	if (sip->choice == SEQID_GI)
		 return OM_MSG_RET_ERROR;

	if((gi = ID1FindSeqId(sip))<=1)
		return OM_MSG_RET_OK;

	sip2 = ValNodeNew(NULL);
	sip2->choice = SEQID_GI;
	sip2->data.intvalue = gi;

        ompcp->output_data = (Pointer)sip2; /* not registering right now */

        return OM_MSG_RET_DONE;   /* data found, don't call further functions */
}

/*****************************************************************************
*
*   s_ID1BioseqFetchFunc(data)
*       callback for ID1SeqEntryGet
*
*****************************************************************************/

static Int2 LIBCALLBACK
s_ID1BioseqFetchFunc(Pointer data)
{
	OMProcControlPtr	ompcp;
	ObjMgrProcPtr		ompp;
	OMUserDataPtr		omdp;
	ID1FetchStructPtr	ifsp;
	SeqEntryPtr		sep;
	BioseqPtr		bsp;
	SeqIdPtr		sip;
	Int4			gi=0;
	Int4			ent=0;
	CharPtr			sat=NULL;
	Int2			retcode=0;

	ompcp = (OMProcControlPtr)data;
	ompp = ompcp->proc;
        ifsp = ompp->procdata;

	if(!ifsp || ifsp->state == ID1BFS_DISABLE){  /* shut off */
		return OM_MSG_RET_OK;    /* not done, go on to next */
	} else if(ifsp->state == ID1BFS_INIT) {
                if (!ID1Init())
			return OM_MSG_RET_ERROR;
		ifsp->state = ID1BFS_READY;
        }

        if(ifsp->state != ID1BFS_READY)
		return OM_MSG_RET_ERROR;

	/* check for cached gi */
        if (ompcp->input_entityID){
                omdp = ObjMgrGetUserData(ompcp->input_entityID,ompp->procid, OMPROC_FETCH, 0);
                if (omdp != NULL) {
			gi = omdp->userdata.intvalue;
		}
        }
        if(!gi){          /* not cached, try input id */
                sip = (SeqIdPtr)(ompcp->input_data);
                if (sip == NULL)
                        return OM_MSG_RET_ERROR;

                if(sip->choice != SEQID_GI){
			if(sip->choice==SEQID_GENERAL){
				Dbtag *db = (Dbtag*)sip->data.ptrvalue;
				sat=db->db;
				gi=ent=db->tag->str?atoi(db->tag->str):db->tag->id;
				sep=ID1ArchSeqEntryGet(gi,sat,ent,0,0);
				if(sep) goto GOTIT;
			}
			gi = ID1FindSeqId(sip);
		} else {
                        gi = sip->data.intvalue;
		}
        }

        if(!gi) return OM_MSG_RET_OK;  /* no error but call next proc */

        sep = ID1SeqEntryGet(gi,0);/*** always get a whole blob in anticipation of following requests ***/
        if(sep == NULL) 
		return OM_MSG_RET_ERROR;
GOTIT:
        sip = (SeqIdPtr)(ompcp->input_data);
        bsp = BioseqFindInSeqEntry(sip, sep);
        ompcp->output_data = (Pointer)bsp;
        ompcp->output_entityID = ObjMgrGetEntityIDForChoice(sep);

	/* store the cache info */
        omdp = ObjMgrAddUserData(ompcp->output_entityID, ompp->procid, OMPROC_FETCH, 0);
        omdp->userdata.intvalue = gi;

        return OM_MSG_RET_DONE;   /* data found, don't call further functions */
}
