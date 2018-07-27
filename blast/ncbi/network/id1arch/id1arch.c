/*
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
* File Name:  id1arch.c
*
* Author:  Eugene Yaschenko
*/

#include <sequtil.h>
#include <ncbinet.h>
#include <objsset.h>
#include <objsub.h>
#include <id1gen.h>
#include <id1arch.h>


static ID1serverBackPtr NetID1servReadAsn PROTO((void));
static Boolean ReestablishNetID1Arch PROTO((void));
static Boolean NetInit PROTO((void));
static Boolean ForceNetInit PROTO((void));
static Boolean NetFini PROTO((void));
static Boolean GenericReestablishNet PROTO((CharPtr svcName, Boolean showErrs));
static SeqIdPtr s_ID1ArchSeqIdsGet PROTO((Uint4 gi,AsnIoPtr aiopr,Boolean PNTR not_found));
static SeqEntryPtr s_ID1ArchSeqEntryGet PROTO((Int4 uid, CharPtr db, Int4 ent,Int4Ptr status, Int2 maxplex));
static Int4 s_ID1ArcgGIStateGet PROTO((Uint4 gi));
static ID1SeqHistPtr s_ID1ArchGIHistGet PROTO((Uint4 gi,Boolean rev,AsnIoPtr aiopr,Boolean PNTR not_found));

static NI_HandPtr svcp = NULL;
static AsnIoPtr   asnin = NULL;
static AsnIoPtr   asnout = NULL;
static Boolean num_attached = 0;
static Boolean reallyFinal = TRUE;
static NI_DispatcherPtr dispatcher;
static Boolean (*myNetInit) PROTO((void));


/*****************************************************************************
*
*   ID1ArchInit ()
*
*****************************************************************************/

Boolean ID1ArchInit (void)

{
    

    myNetInit = ID1ArchInit;

    if (! NetInit())
        return FALSE;

    svcp = NI_GenericGetService(dispatcher, NULL, "ID1", "ID1", TRUE);
    if (svcp == NULL)
    {
        ErrPost(CTX_UNKNOWN, 1, "NI_ServiceGet [%s] (%s)", ni_errlist[ni_errno], ni_errtext);
        ID1ArchFini();
        return FALSE;
    }

    asnin = svcp->raip;
    AsnIoSetBufsize(asnin,4390);
    asnout = svcp->waip;
    return TRUE;
}

/*****************************************************************************
*
*   ID1ArchFini ()
*
*****************************************************************************/

static Boolean s_ID1ArchFini (void)

{
    NetFini();
    return TRUE;
}

/* the only thing done here is to suppress errors */
Boolean ID1ArchFini (void)

{
    return  s_ID1ArchFini();
}

static SeqEntryPtr
s_ID1ArchSeqEntryGet (Int4 uid, CharPtr db, Int4 ent,Int4Ptr status, Int2 maxplex)

{
    ID1serverRequestPtr id1reqp;
    SeqEntryPtr sep;
    ID1serverMaxcomplexPtr mcp;
    ID1serverBackPtr id1bp;


    if(status)
	*status=0;

    id1reqp = ValNodeNew(NULL);
    id1reqp->choice = ID1serverRequest_getsefromgi;
		mcp = ID1serverMaxcomplexNew();
    id1reqp->data.ptrvalue = mcp;
		mcp -> gi = uid;
		mcp -> maxplex = maxplex;
		if(ent > 0){
			mcp->ent = ent;
			if(db){
				mcp->sat=StringSave(db);
			}
		}
    ID1serverRequestAsnWrite (id1reqp, asnout, NULL);
    AsnIoReset(asnout);
    ID1serverRequestFree (id1reqp);

    SeqMgrHoldIndexing(TRUE);
    id1bp = NetID1servReadAsn();
    SeqMgrHoldIndexing(FALSE);

    if (id1bp == NULL)
        return NULL;

    if (id1bp->choice != ID1serverBack_gotseqentry &&
			id1bp->choice !=  ID1serverBack_gotdeadseqentry)
    {
	if(status && id1bp->choice == ID1serverBack_error){
		*status = id1bp->data.intvalue;
	}
        ID1serverBackFree (id1bp);
        return NULL;
    }
		if (status != NULL){
			if (id1bp->choice == ID1serverBack_gotdeadseqentry){
				* status = 3;
			}else{
				* status = 0;
			}
		}
    sep = (SeqEntryPtr) (id1bp->data.ptrvalue);
    id1bp->data.ptrvalue = NULL; /* for clean free */
    ID1serverBackFree (id1bp);

    return sep;
}

SeqEntryPtr
ID1ArchSeqEntryGet (Int4 gi,CharPtr db, Int4 ent, Int4Ptr status, Int2 maxplex)
{
	Int4 i;
	SeqEntryPtr sep=NULL;
	Int4 retval = 0;
#if 0 
	if (maxplex < 0 || maxplex > 4){
		ErrPost(CTX_UNKNOWN, 2, "ID1ArchSeqEntryGet: maxplex(%d) out of range (0-4)", maxplex);
		return NULL;
	}
#endif
	for (i = 0; i < ID_SERV_RETRIES; i++){
		if (i > 0){
			if (! ReestablishNetID1Arch())
			break;
		}
		sep = s_ID1ArchSeqEntryGet (gi, db, ent, status, maxplex);
		if (sep) break; /* success */
		if(status && *status > 0 ) break; /* valid error - no retry */
	}
	return sep;
}

static SeqIdPtr
s_ID1ArchSeqIdsGet (Uint4 gi,AsnIoPtr aiopr, Boolean PNTR not_found)
{
    ID1serverRequestPtr id1reqp;
    ID1serverBackPtr id1bp;
    SeqIdPtr sip;

    *not_found=FALSE;

    id1reqp = ValNodeNew(NULL);
    id1reqp->choice = ID1serverRequest_getseqidsfromgi;
    id1reqp->data.intvalue = gi;
    ID1serverRequestAsnWrite (id1reqp, asnout, NULL);
    AsnIoReset(asnout);
    ID1serverRequestFree (id1reqp);

    if ((id1bp = NetID1servReadAsn()) == NULL)
        return NULL;

    if (id1bp->choice != ID1serverBack_ids) {
        ID1serverBackFree (id1bp);
        return NULL;
    }
    if(aiopr){
        ID1serverBackAsnWrite(id1bp,aiopr,NULL);
        AsnIoReset(aiopr);
    }
    sip = (id1bp->data.ptrvalue);
    id1bp->data.ptrvalue = NULL;
    ID1serverBackFree (id1bp);
    if(sip==NULL) *not_found=TRUE;
    return sip;
}

SeqIdPtr
ID1ArchSeqIdsGet(Uint4 gi,AsnIoPtr aiopr)
{
	Int4  i;
	SeqIdPtr sip = NULL;
	Boolean	not_found=FALSE;

	for (i = 0; i < ID_SERV_RETRIES; i++) {
		if (i > 0) { if (! ReestablishNetID1Arch()) break; }

		sip = s_ID1ArchSeqIdsGet(gi,aiopr,&not_found);
		if (sip) break; /* success */
		if (not_found) break; /* not found */
	}
	return sip;
}

static ID1SeqHistPtr
s_ID1ArchGIHistGet(Uint4 gi,Boolean rev,AsnIoPtr aiopr,Boolean PNTR not_found)
{
    ID1serverRequestPtr id1reqp;
    ID1serverBackPtr id1bp;

    ID1SeqHistPtr ishp;

    *not_found=FALSE;
    id1reqp = ValNodeNew(NULL);
    id1reqp->choice = rev? ID1serverRequest_getgirev : ID1serverRequest_getgihist;
    id1reqp->data.intvalue = gi;
    ID1serverRequestAsnWrite (id1reqp, asnout, NULL);
    AsnIoReset(asnout);
    ID1serverRequestFree (id1reqp);

    if ((id1bp = NetID1servReadAsn()) == NULL)
        return NULL;

    if (id1bp->choice != ID1serverBack_girevhist && id1bp->choice != ID1serverBack_gihist)
    {
        ID1serverBackFree (id1bp);
        return NULL;
    }
    if(aiopr){
	ID1serverBackAsnWrite(id1bp,aiopr,NULL);
	AsnIoReset(aiopr);
    }
    ishp = (id1bp->data.ptrvalue);
    id1bp->data.ptrvalue = NULL;
    ID1serverBackFree (id1bp);
    if(ishp==NULL) *not_found=TRUE;
    return ishp;
}

ID1SeqHistPtr
ID1ArchGIHistGet(Uint4 gi,Boolean rev,AsnIoPtr aiopr)
{
    Int4  i;
    ID1SeqHistPtr ishp = NULL;
    Boolean not_found=FALSE;

          for (i = 0; i < ID_SERV_RETRIES; i++)
          {
                        if (i > 0)
                        {
                                 if (! ReestablishNetID1Arch())
                                          break;
                        }

                        ishp = s_ID1ArchGIHistGet(gi,rev,aiopr,&not_found);
                        if (ishp) break; /* success */
			if(not_found) break; /* not found */
          }
    return ishp;
}


static Int4
s_ID1ArcgGIStateGet(Uint4 gi)
{
    ID1serverRequestPtr id1reqp;
    ID1serverBackPtr id1bp;

    Int4 state;

    id1reqp = ValNodeNew(NULL);
    id1reqp->choice = ID1serverRequest_getgistate;
    id1reqp->data.intvalue = gi;
    ID1serverRequestAsnWrite (id1reqp, asnout, NULL);
    AsnIoReset(asnout);
    ID1serverRequestFree (id1reqp);

    if ((id1bp = NetID1servReadAsn()) == NULL)
        return -1;

    if (id1bp->choice != ID1serverBack_gistate)
    {
        ID1serverBackFree (id1bp);
        return -1;
    }
    state = id1bp->data.intvalue;
    ID1serverBackFree (id1bp);
    return state;
}

Int4    
ID1ArcgGIStateGet(Uint4 gi)
{
	Int4 state=0,i;

	for (i = 0; i < ID_SERV_RETRIES; i++)
	{
		if (i > 0 && !ReestablishNetID1Arch()) break; 
		state = s_ID1ArcgGIStateGet(gi);
		if (state >=0 ) break; /* success */
	}
	return state;
}

static ID1serverBackPtr NetID1servReadAsn(void)
{
    ID1serverBackPtr id1bp=NULL;

    id1bp = ID1serverBackAsnRead(asnin, NULL);

    if (!id1bp)
    {
        ErrPostEx(SEV_ERROR, 0, 0, "Null message read from server");
    }
    return id1bp;
}

static Boolean ReestablishNetID1Arch(void)
{
    return GenericReestablishNet("ID1Arch", TRUE);
}

static Boolean GenericReestablishNet(CharPtr svcName, Boolean showErrs)
{
    Monitor *mon = NULL;
    Boolean retval;
    CharPtr buf;

    buf = MemNew(2 * StrLen(svcName) + 60);

    if (showErrs) {
        sprintf (buf, "Re-establishing %s Service", svcName);
        mon = MonitorStrNew(buf, 40);
        sprintf (buf, "Requesting %s service", svcName);
        MonitorStrValue(mon, buf);
    }
    NetFini();
    retval = TRUE;

    if (! myNetInit())
    {
        sprintf (buf, "%s get failed; re-contacting dispatcher", svcName);
        MonitorStrValue(mon, buf);
        retval = FALSE;
        if (ForceNetInit())
        { /* successfully established contact w/dispatcher */
            sprintf (buf, "%s get failed; re-requesting %s service",
                     svcName, svcName);
            MonitorStrValue(mon, buf);
            retval = myNetInit();
        }
        else {
            ErrPost(CTX_UNKNOWN, 1, "Unable to re-contact dispatcher");
            if (showErrs) {
                ErrShow();
            }
        }
    }

    MonitorFree(mon);

    if (! retval )
    {
        sprintf (buf, "Unable to re-establish %s service", svcName);
        ErrPost(CTX_UNKNOWN, 1, buf);
        if (showErrs) {
            ErrShow();
        }
    }

    MemFree(buf);
    AsnIoSetBufsize(asnin,4390);
    return retval;
}

static Boolean
NetInit(void)
{
    if (num_attached++ > 0)
        return TRUE;

    return ((dispatcher = NI_GenericInit(NULL, NULL, TRUE, NULL, 0)) != NULL);
}


static Boolean ForceNetInit(void)
{
    Boolean retval;

    reallyFinal = FALSE;
    num_attached = 0; /* force re-attempt to contact dispatcher */
    retval = NetInit();
    reallyFinal = TRUE;

    return retval;
}

static Boolean NetFini(void)
{
    if (num_attached > 0)
        num_attached--;

    if (num_attached == 0)
    {
        NI_ServiceDisconnect(svcp);
        svcp = NULL;
        NI_EndServices (dispatcher);
        dispatcher = NULL;
    }

    return TRUE;
}


static Int4
s_ID1ArchGIGet (SeqIdPtr sip)
{
    ID1serverRequestPtr id1reqp;
    ID1serverBackPtr id1bp;
    Int4 gi;

    id1reqp = ValNodeNew(NULL);
    id1reqp->choice = ID1serverRequest_getgi;
    id1reqp->data.ptrvalue = sip;
    ID1serverRequestAsnWrite (id1reqp, asnout, NULL);
    AsnIoReset(asnout);
    id1reqp->data.ptrvalue = NULL;
    ID1serverRequestFree (id1reqp);

    if ((id1bp = NetID1servReadAsn()) == NULL)
        return -1;

    if (id1bp->choice != ID1serverBack_gotgi)
    {
        ID1serverBackFree (id1bp);
        return 0;
    }
    gi = (id1bp->data.intvalue);
    ID1serverBackFree (id1bp);

    return gi;
}

Int4
ID1ArchGIGet (SeqIdPtr sip)
{
    Int4 gi=-1, i;
    Int4 retval = 0;

          for (i = 0; i < ID_SERV_RETRIES; i++)
          {
                        if (i > 0)
                        {
                                 if (! ReestablishNetID1Arch())
                                          break;
                        }

                        gi = s_ID1ArchGIGet (sip);
                        if (gi >= 0)
                                 break; /* success */
          }
        return gi;
}



void    SeqHistPrintTable (ID1SeqHistPtr ishp,FILE PNTR fp)
{
        Char    repl_date[20];
        SeqHistPtr shp;
	SeqIdPtr   sip;
	Uint4      old_gi=0,ent=0;
	CharPtr	   sat=NULL;
#ifdef IDFETCH_HTML_OUTPUT
	Boolean    checked=FALSE;
	if(!ishp){
		fprintf(fp,"<HR><h2>Gi is not found</h2>");
		goto END;
	}
        fprintf(fp,"<PRE><FORM ACTION=\"http://%s:%s%s\"METHOD=POST" ">%c",
                getenv("SERVER_NAME"),
                getenv("SERVER_PORT"),
                getenv("SCRIPT_NAME"),
                10);
#endif
        if(ishp){
#ifdef IDFETCH_HTML_OUTPUT
		fprintf(fp,"<b>");
                fprintf(fp,"%-20s%-20s%-20s\n","GI","Loaded","Select");
		fprintf(fp,"%-20s%-20s%-20s\n","--","------","------");
		fprintf(fp,"</b>");
#else
		fprintf(fp,"%-20s%-20s%-10s%-20s\n","GI","Loaded","DB","Retrieval No.");
		fprintf(fp,"%-20s%-20s%-10s%-20s\n","--","------","--","------------");

#endif
		
                for(;ishp;ishp=ishp->next){
                        shp=(SeqHistPtr)ishp->hist;
			for(sip=shp->replace_ids;sip;sip=sip->next){
				if(sip->choice == SEQID_GI ){
					if(old_gi != sip->data.intvalue){  
						old_gi = sip->data.intvalue;
					}
				}else if(sip->choice == SEQID_LOCAL){
					ent = ((ObjectIdPtr)sip->data.ptrvalue)->id;
					sat = "ID";
				} else if(sip->choice == SEQID_GENERAL) {
					ent = ((DbtagPtr)sip->data.ptrvalue)->tag->id;
					sat = ((DbtagPtr)sip->data.ptrvalue)->db;
				}
			}
                        if(shp->replace_date){
                                if(shp->replace_date->data[0]){
                                        sprintf(repl_date,"%0.2d/%0.2d/%0.4d",
                                                        shp->replace_date->data[2],
                                                        shp->replace_date->data[3],
                                                        1900+shp->replace_date->data[1]);
                                }else{
                                        sprintf(repl_date,"%-20s",shp->replace_date->str);
                                }
                        }else{
                                sprintf(repl_date,"N/A");
                        }
                        fprintf(fp,"%-20d%-20s%",old_gi,repl_date);
#ifdef IDFETCH_HTML_OUTPUT
			fprintf(fp,"<INPUT TYPE=radio NAME=in_gientlist VALUE=\"%d|%s|%d\" %s>     ",
				old_gi,repl_date,ent,checked?"":"CHECKED");
			fprintf(fp,"<INPUT TYPE=radio NAME=in_gientlist_2 VALUE=\"%d|%s|%d\" %s>\n",
                                old_gi,repl_date,ent,checked?"":"CHECKED");
			checked = TRUE;
#else
			fprintf(fp,"%-10s%-20d\n",sat,ent);
#endif
                }
	}
#ifdef IDFETCH_HTML_OUTPUT
	fprintf(fp,"</PRE>");
	fprintf(fp,"<HR>\n<h3>View Sequence:</h3>\n");
	fprintf(fp,"<b>Print Form:</b>&nbsp;&nbsp;"
		   "<select NAME=in_format>  "
		   "<option> GenBank"
		   "<option> GenPept"
		   "<option> FASTA"
                   "<option> ASN.1"
		   "<option> GbDiff"
		   "</select>");
	fprintf(fp,"<BR><INPUT TYPE=\"checkbox\" NAME=in_full VALUE=\"checked\">"
         " &nbsp; &nbsp;<b>Get full record (including other gis</b>)"
         "<BR><BR>");
	fprintf(fp,"<INPUT TYPE = \"submit\" VALUE = \"Display\">");
	fprintf(fp,"</FORM>");
END:
	1==1;
#endif
}
void   id_print_gi_state(Int4 state,CharPtr buf,Uint1 len)
{
        Int4    dlen;
        switch(state & 0xff){
         case 0:
                strncpy(buf,"NOT EXIST ",len-1);
                break;
         case 10:
                strncpy(buf,"DELETED ",len-1);
                break;
         case 20:
                strncpy(buf,"REPLACED ",len-1);
                break;
         case 40:
                strncpy(buf,"LIVE ",len-1);
                break;
         default:
                strncpy(buf,"UNKNOWN",len-1);
                break;
        }
        dlen = strlen(buf);
        if(state & GI_IS_SUPPRESSED){
                strncpy(buf+dlen,"|SUPPRESSED",len - dlen - 1);
        }
        dlen = strlen(buf);
        if(state & GI_IS_OVERRIDEN){
                strncpy(buf+dlen,"|WITHDRAWN",len - dlen - 1);
        }
        dlen = strlen(buf);
        if(state & GI_IS_CONFIDENTIAL){
                strncpy(buf+dlen,"|CONFIDENTIAL",len - dlen - 1);
        }
}
