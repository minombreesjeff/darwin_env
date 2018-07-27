/*   txcdproc.c
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
* File Name:  txcdproc.c
*
* Author:  Vladimir Soussov
*
* Version Creation Date:   07/15/97
*
* $Revision: 1.12 $
*
* File Description: 
*       API for Taxonomy service
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*
*
* RCS Modification History:
* $Log: txcdproc.c,v $
* Revision 1.12  2001/09/28 15:53:15  soussov
* tax1e_maxTaxId() added
*
* Revision 1.11  2001/09/18 16:55:38  soussov
* switching to the new ASN
*
* Revision 1.10  2001/09/18 14:42:26  soussov
* Fixes error processing in case of reconnection
*
* Revision 1.9  2001/09/17 22:27:34  soussov
* ErrPost call added if nothing was read from the service
*
* Revision 1.8  2001/06/13 21:23:19  soussov
* makes purify happy
*
* Revision 1.7  1999/12/20 17:05:16  soussov
* taxid4gi added
*
* Revision 1.6  1999/10/21 19:20:57  soussov
* time tracking added
*
* Revision 1.5  1998/07/27 16:54:04  soussov
* bug in txc_getChildren fixed
*
* Revision 1.4  1998/06/11 20:42:11  soussov
* kill some warnings
*
* Revision 1.3  1998/04/01 17:34:02  soussov
* changed tp include <>
*
* Revision 1.2  1998/03/31 22:54:42  kans
* codewarrior can tell null from 0, and unsigned charptr needed cast to CharPtr
*
* Revision 1.1  1998/02/10 20:12:02  soussov
* taxon2 related soft
*
 *
*/

/** for ErrPostEx() ****/

static char *this_module = "txcdproc";

#ifdef THIS_MODULE
#undef THIS_MODULE
#endif

#define THIS_MODULE this_module
static char *this_file = __FILE__;
#define THIS_FILE this_file

#include <ncbinet.h>
#include <objall.h>
#include <objfeat.h>
#include <objtax1.h>
#include <txclient.h>
#include <taxinc.h>
#include <time.h>

#define TAXARCH_SERV_RETRIES 4

static Taxon1RespPtr NetTaxArchReadAsn PROTO((void));
static Boolean ReestablishNetTaxArch PROTO((void));
static Boolean NetInit PROTO((void));
static Boolean ForceNetInit PROTO((void));
static Boolean NetFini PROTO((void));
static Boolean GenericReestablishNet PROTO((CharPtr svcName, Boolean showErrs));
static Boolean TaxServFini (void);

static NI_HandPtr svcp = NULL;
static AsnIoPtr   asnin = NULL;
static AsnIoPtr   asnout = NULL;
static Boolean num_attached = 0;
static Boolean reallyFinal = TRUE;
static NI_DispatcherPtr dispatcher;
static Boolean (*myNetInit) PROTO((void));

static int tax_track= 0;
static clock_t tax_time1;

static void report_service_error(CharPtr proc_name, Taxon1RespPtr taxbp)
{
    if(taxbp == NULL) {
	ErrPostEx(SEV_ERROR, 0, 0, "%s got NULL responce from service");
    }
    else if(taxbp->choice == Taxon1Resp_error) {
	Taxon1ErrorPtr tep= taxbp->data.ptrvalue;
	if(tep != NULL) {
	    ErrPostEx(tep->level, 0, 0, "%s got %s", proc_name, tep->msg? tep->msg : "Empty message");
	}
    }
}

/*****************************************************************************
*
*   NetTaxArchReadAsn ()
*
*****************************************************************************/

static Taxon1RespPtr NetTaxArchReadAsn(void)
{
    Taxon1RespPtr taxbp;
    short erract;
    ErrDesc err;

    ErrGetOpts(&erract, NULL);
    ErrSetOpts(ERR_CONTINUE, 0);
    ErrFetch(&err); /* clear any pending error */

    taxbp = Taxon1RespAsnRead(asnin, NULL);

    if ((taxbp == NULL) || ErrFetch(&err))
    {
        ErrPost (CTX_UNKNOWN, 1, "Null message read from server");
    }
    ErrSetOpts(erract, 0);

    return taxbp;
}

/*****************************************************************************
*
*   txc_connect2server()
*
*   all arguments are optional just to keep os interface
*****************************************************************************/

static Boolean TaxServInit(void)
{
    DataVal    av;
    Taxon1ReqPtr taxrp;
    Taxon1RespPtr taxbp;

    myNetInit = TaxServInit;

    if (!NetInit()) return FALSE;

    svcp= NI_GenericGetService(dispatcher, NULL, "TAXONOMY", "TaxService", TRUE);

    if (svcp == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "NI_ServiceGet [%s] (%s)", ni_errlist[ni_errno], ni_errtext);
        TaxServFini();
        return FALSE;
    }

    asnin= svcp->raip;
    asnout= svcp->waip;

    if(getenv("TAX_TRACK_TIME") != NULL) {
	tax_track= 1;
    }

    /**********************************************************/

    taxrp= ValNodeNew(NULL);
    taxrp->choice = Taxon1Req_init;
    if(tax_track) tax_time1= clock();
    Taxon1ReqAsnWrite(taxrp, asnout, NULL);
    AsnIoReset(asnout);
    Taxon1ReqFree (taxrp);

    if((taxbp = NetTaxArchReadAsn()) == NULL) {
        return FALSE;
    }
    else  {
        taxbp->data.ptrvalue = NULL;
        Taxon1RespFree (taxbp);
	if(tax_track) {
	    double diff= (double)(clock() - tax_time1)/CLOCKS_PER_SEC;
	    printf("tax1_init() %f\n", diff);
	}
	    
        return TRUE;
    }
}

Boolean txc_connect2Server(CharPtr server, CharPtr user, CharPtr passwd, CharPtr module)
{
    return TaxServInit();
}


/*****************************************************************************
*
*   Tax0Fini ()
*
*****************************************************************************/

static Boolean s_TaxArchFini(void)
{
    Boolean retval = TRUE;
    Taxon1ReqPtr taxrp;
    Taxon1RespPtr taxbp;

    
    if (asnout != NULL && asnin != NULL)  {
        taxrp = ValNodeNew(NULL);
        taxrp->choice = Taxon1Req_fini;
        Taxon1ReqAsnWrite (taxrp, asnout, NULL);
        AsnIoReset(asnout);
        Taxon1ReqFree (taxrp);

        if ((taxbp = NetTaxArchReadAsn()) == NULL) {
            retval = FALSE;
        }
        else {
            taxbp->data.ptrvalue = NULL;
            Taxon1RespFree (taxbp);
        }
    }

    NetFini();

    return retval;
}

/* the only thing done here is to suppress errors */

static Boolean TaxServFini (void)
{
    short erract;
    ErrDesc err;
    Boolean retval;

    ErrGetOpts(&erract, NULL);
    ErrSetOpts(ERR_IGNORE, 0);
    ErrFetch(&err);

    retval = s_TaxArchFini();

    ErrSetOpts(erract, 0);
    ErrFetch(&err);

    return retval;
}

void txc_close(void)
{
    TaxServFini();
}


static Taxon1NamePtr s_findname(CharPtr sname)
{
    Taxon1ReqPtr taxrp;
    Taxon1RespPtr taxbp;
    Taxon1NamePtr tnp;

    if((taxrp= ValNodeNew(NULL)) == NULL) return NULL;
    taxrp->choice= Taxon1Req_findname;
    taxrp->data.ptrvalue= sname;
    Taxon1ReqAsnWrite(taxrp, asnout, NULL);
    AsnIoReset(asnout);
    taxrp->data.ptrvalue= NULL;
    Taxon1ReqFree(taxrp);

    if((taxbp= NetTaxArchReadAsn()) == NULL) return NULL;
    
    if(taxbp->choice != Taxon1Resp_findname) {
	report_service_error("tax_findByName", taxbp);
	Taxon1RespFree(taxbp);
	return NULL;
    }

    tnp= taxbp->data.ptrvalue;
    taxbp->data.ptrvalue= NULL;
    Taxon1RespFree(taxbp);

    return tnp;
}

static Taxon1NamePtr findname(CharPtr sname)
{
    Int4 i;
    short erract;
    ErrDesc err;
    Taxon1NamePtr tnp = NULL;

    if(tax_track) tax_time1= clock();
    for (i= TAXARCH_SERV_RETRIES; i >= 0; --i) {
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);

        tnp = s_findname(sname);

        ErrSetOpts(erract, 0);
        if (!ErrFetch(&err))  break; /* success */
	
	if(!ReestablishNetTaxArch()) break;
    }
    if(tax_track) {
	double diff= (double)(clock() - tax_time1)/CLOCKS_PER_SEC;
	printf("tax1_findByName() %f\n", diff);
    }
	

    return(tnp);
}

Int4 tax_findByName(CharPtr sname, int mode, TaxNamePtr* res_name)
{
    TaxNamePtr o_name;
    Int4 i, n= 0;
    Taxon1NamePtr tnp, tnp_list= ((sname == NULL) || (*sname == '\0'))? NULL : findname(sname);
    

    if((res_name == NULL) || (tnp_list == NULL)) return 0;
    for(tnp= tnp_list; tnp != NULL; tnp= tnp->next) n++;

    *res_name= o_name= MemNew(n*sizeof(TaxName));
    if(o_name == NULL) {
	for(i= 0; i < n; i++) {
	    tnp= tnp_list->next;
	    Taxon1NameFree(tnp_list);
	    tnp_list= tnp;
	}
	return 0;
    }
    
    for(i= 0; i < n; i++) {
	tnp= tnp_list->next;
	o_name[i].tax_id= tnp_list->taxid;
	o_name[i].class_cde= tnp_list->cde;
	o_name[i].name_txt= tnp_list->oname;
	o_name[i].unique_name= tnp_list->uname;
	MemFree(tnp_list);
	tnp_list= tnp;
    }

    return n;
}
		
static Taxon1NamePtr s_getorgnames(Int4 tax_id)
{
    Taxon1ReqPtr taxrp;
    Taxon1RespPtr taxbp;
    Taxon1NamePtr tnp;

    if((taxrp= ValNodeNew(NULL)) == NULL) return NULL;
    taxrp->choice= Taxon1Req_getorgnames;
    taxrp->data.intvalue= tax_id;
    Taxon1ReqAsnWrite(taxrp, asnout, NULL);
    AsnIoReset(asnout);
    Taxon1ReqFree(taxrp);

    if((taxbp= NetTaxArchReadAsn()) == NULL) return NULL;
    
    if(taxbp->choice != Taxon1Resp_getorgnames) {
	report_service_error("tax_getOrgNames", taxbp);
	Taxon1RespFree(taxbp);
	return NULL;
    }

    tnp= taxbp->data.ptrvalue;
    taxbp->data.ptrvalue= NULL;
    Taxon1RespFree(taxbp);

    return tnp;
}


static Taxon1NamePtr getorgnames(Int4 tax_id)
{
    Int4 i;
    short erract;
    ErrDesc err;
    Taxon1NamePtr tnp = NULL;

    for (i= TAXARCH_SERV_RETRIES; i >= 0; --i) {
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);

        tnp = s_getorgnames(tax_id);

        ErrSetOpts(erract, 0);
        if (!ErrFetch(&err))  break; /* success */
	
	if(!ReestablishNetTaxArch()) break;
    }

    return(tnp);
}

Int4 tax_getOrgNames(Int4 tax_id, TaxNamePtr* res_name)
{
    TaxNamePtr o_name;
    Int4 i, n= 0;
    Taxon1NamePtr tnp, tnp_list= (tax_id > 0)? getorgnames(tax_id) : NULL;
    

    if((res_name == NULL) || (tnp_list == NULL)) return 0;
    for(tnp= tnp_list; tnp != NULL; tnp= tnp->next) n++;

    *res_name= o_name= MemNew(n*sizeof(TaxName));
    if(o_name == NULL) {
	for(i= 0; i < n; i++) {
	    tnp= tnp_list->next;
	    Taxon1NameFree(tnp_list);
	    tnp_list= tnp;
	}
	return 0;
    }
    
    for(i= 0; i < n; i++) {
	tnp= tnp_list->next;
	o_name[i].tax_id= tnp_list->taxid;
	o_name[i].class_cde= tnp_list->cde;
	o_name[i].name_txt= tnp_list->oname;
	o_name[i].unique_name= tnp_list->uname;
	MemFree(tnp_list);
	tnp_list= tnp;
    }

    return n;
}

static Int4 s_getdesignator(CharPtr sname)
{
    Taxon1ReqPtr taxrp;
    Taxon1RespPtr taxbp;
    Int4 tax_id;

    if((taxrp= ValNodeNew(NULL)) == NULL) return 0;
    taxrp->choice= Taxon1Req_getdesignator;
    taxrp->data.ptrvalue= sname;
    Taxon1ReqAsnWrite(taxrp, asnout, NULL);
    AsnIoReset(asnout);
    taxrp->data.ptrvalue= NULL;
    Taxon1ReqFree(taxrp);

    if((taxbp= NetTaxArchReadAsn()) == NULL) return 0;
    
    if(taxbp->choice != Taxon1Resp_getdesignator) {
	report_service_error("tax_getDesignator", taxbp);
	Taxon1RespFree(taxbp);
	return 0;
    }

    tax_id= taxbp->data.intvalue;
    Taxon1RespFree(taxbp);

    return tax_id;
}

static Int4 getdesignator(CharPtr sname)
{
    Int4 i;
    short erract;
    ErrDesc err;
    Int4 tax_id= 0;

    for (i= TAXARCH_SERV_RETRIES; i >= 0; --i) {
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);

        tax_id = s_getdesignator(sname);

        ErrSetOpts(erract, 0);
        if (!ErrFetch(&err))  break; /* success */
	
	if(!ReestablishNetTaxArch()) break;
    }

    return tax_id;
}

Int4 tax_getDesignator(char* sname)
{
    return getdesignator(sname);
}

static Int4 s_getunique(CharPtr sname)
{
    Taxon1ReqPtr taxrp;
    Taxon1RespPtr taxbp;
    Int4 tax_id;

    if((taxrp= ValNodeNew(NULL)) == NULL) return 0;
    taxrp->choice= Taxon1Req_getunique;
    taxrp->data.ptrvalue= sname;
    Taxon1ReqAsnWrite(taxrp, asnout, NULL);
    AsnIoReset(asnout);
    taxrp->data.ptrvalue= NULL;
    Taxon1ReqFree(taxrp);

    if((taxbp= NetTaxArchReadAsn()) == NULL) return 0;
    
    if(taxbp->choice != Taxon1Resp_getunique) {
	report_service_error("tax_uniqueName", taxbp);
	Taxon1RespFree(taxbp);
	return 0;
    }

    tax_id= taxbp->data.intvalue;
    Taxon1RespFree(taxbp);

    return tax_id;
}

static Int4 getunique(CharPtr sname)
{
    Int4 i;
    short erract;
    ErrDesc err;
    Int4 tax_id= 0;

    for (i= TAXARCH_SERV_RETRIES; i >= 0; --i) {
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);

        tax_id = s_getunique(sname);

        ErrSetOpts(erract, 0);
        if (!ErrFetch(&err))  break; /* success */
	
	if(!ReestablishNetTaxArch()) break;
    }

    return tax_id;
}

Int4 tax_uniqueName(CharPtr sname, Int4 id)
{
    Int4 tax_id= getunique(sname);
    return (tax_id == id)? 0 : tax_id;
}

static Int4 s_getidbyorg(OrgRefPtr orgRef)
{
    Taxon1ReqPtr taxrp;
    Taxon1RespPtr taxbp;
    Int4 tax_id;

    if((taxrp= ValNodeNew(NULL)) == NULL) return 0;
    taxrp->choice= Taxon1Req_getidbyorg;
    taxrp->data.ptrvalue= orgRef;
    Taxon1ReqAsnWrite(taxrp, asnout, NULL);
    AsnIoReset(asnout);
    taxrp->data.ptrvalue= NULL;
    Taxon1ReqFree(taxrp);

    if((taxbp= NetTaxArchReadAsn()) == NULL) return 0;
    
    if(taxbp->choice != Taxon1Resp_getidbyorg) {
	report_service_error("txc_getTaxIdByOrgRef", taxbp);
	Taxon1RespFree(taxbp);
	return 0;
    }

    tax_id= taxbp->data.intvalue;
    Taxon1RespFree(taxbp);

    return tax_id;
}

static Int4 getidbyorg(OrgRefPtr orgRef)
{
    Int4 i;
    short erract;
    ErrDesc err;
    Int4 tax_id= 0;

    if(tax_track) tax_time1= clock();
    for (i= TAXARCH_SERV_RETRIES; i >= 0; --i) {
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);

        tax_id = s_getidbyorg(orgRef);

        ErrSetOpts(erract, 0);
        if (!ErrFetch(&err))  break; /* success */
	
	if(!ReestablishNetTaxArch()) break;
    }

    if(tax_track) {
	double diff= (double)(clock() - tax_time1)/CLOCKS_PER_SEC;
	printf("tax1_getTaxIdByOrgRef() %f\n", diff);
    }
    

    return tax_id;
}

Int4 txc_getTaxIdByOrgRef(OrgRefPtr orgRef)
{
    return getidbyorg(orgRef);
}

Int4 tax_getIdByName(char* sname, char* qualif, Int1 id)
{
    OrgRefPtr orgRef= OrgRefNew();
    Int4 tax_id;
    
    orgRef->taxname= sname;
    if(qualif != NULL) {
	orgRef->orgname= OrgNameNew();
	orgRef->orgname->mod= OrgModNew();
	orgRef->orgname->mod->subtype= id;
	orgRef->orgname->mod->subname= qualif;
    }
    
    tax_id= getidbyorg(orgRef);
    orgRef->taxname= NULL;
    if(qualif != NULL) {
	orgRef->orgname->mod->subname=NULL;
    }
    OrgRefFree(orgRef);
    return tax_id;
}

static Taxon1NamePtr s_getlineage(Int4 tax_id)
{
    Taxon1ReqPtr taxrp;
    Taxon1RespPtr taxbp;
    Taxon1NamePtr tnp;

    if((taxrp= ValNodeNew(NULL)) == NULL) return NULL;
    taxrp->choice= Taxon1Req_taxalineage;
    taxrp->data.intvalue= tax_id;
    Taxon1ReqAsnWrite(taxrp, asnout, NULL);
    AsnIoReset(asnout);
    Taxon1ReqFree(taxrp);

    if((taxbp= NetTaxArchReadAsn()) == NULL) return NULL;
    
    if(taxbp->choice != Taxon1Resp_taxalineage) {
	report_service_error("txc_getLineage", taxbp);
	Taxon1RespFree(taxbp);
	return NULL;
    }

    tnp= taxbp->data.ptrvalue;
    taxbp->data.ptrvalue= NULL;
    Taxon1RespFree(taxbp);

    return tnp;
}


static Taxon1NamePtr getlineage(Int4 tax_id)
{
    Int4 i;
    short erract;
    ErrDesc err;
    Taxon1NamePtr tnp = NULL;

    if(tax_track) tax_time1= clock();
    for (i= TAXARCH_SERV_RETRIES; i >= 0; --i) {
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);

        tnp = s_getlineage(tax_id);

        ErrSetOpts(erract, 0);
        if (!ErrFetch(&err))  break; /* success */
	
	if(!ReestablishNetTaxArch()) break;
    }

    if(tax_track) {
	double diff= (double)(clock() - tax_time1)/CLOCKS_PER_SEC;
	printf("tax1_getLineage() %f\n", diff);
    }

    return(tnp);
}

TXC_TreeNodePtr* txc_getLineage(Int4 lin_id, Int4Ptr lin_len)
{
    TXC_TreeNodePtr* lin;
    Int4 n= 0, i, k, k1;
    Taxon1NamePtr tnp, tnp_list= (lin_id > 0)? getlineage(lin_id) : NULL;

    if(lin_len != NULL) *lin_len= 0;

    if(tnp_list == NULL) return NULL;

    for(tnp= tnp_list; tnp != NULL; tnp= tnp->next) n++;

    if((lin= MemNew(n*sizeof(TXC_TreeNodePtr))) == NULL) {
	/* no memory, just free tnp_list */
	for(i= 0; i < n; i++) {
	    tnp= tnp_list->next;
	    Taxon1NameFree(tnp_list);
	    tnp_list= tnp;
	}
	return NULL;
    }

    for(i= 0; i < n; i++) {
	tnp= tnp_list->next;
	k= StringLen(tnp_list->oname);
	k1= tnp_list->uname? (StringLen(tnp_list->uname) + 2) : 0;
	if((lin[i]= MemNew(k+k1+10)) != NULL) {
	    lin[i]->tax_id= tnp_list->taxid;
	    lin[i]->flags= tnp_list->cde | TXC_STHIDE;
	    memcpy(lin[i]->node_label, tnp_list->oname, k+1);
	    if(k1) {
		memcpy(lin[i]->node_label + (k+1), tnp_list->uname, k1 - 1);
		lin[i]->flags^= TXC_STHIDE;
	    }
	}
	Taxon1NameFree(tnp_list);
	tnp_list= tnp;
    }
	    
    if(lin_len != NULL) *lin_len= n;
    return lin;
}
		
static Taxon1NamePtr s_getchildren(Int4 tax_id)
{
    Taxon1ReqPtr taxrp;
    Taxon1RespPtr taxbp;
    Taxon1NamePtr tnp;

    if((taxrp= ValNodeNew(NULL)) == NULL) return NULL;
    taxrp->choice= Taxon1Req_taxachildren;
    taxrp->data.intvalue= tax_id;
    Taxon1ReqAsnWrite(taxrp, asnout, NULL);
    AsnIoReset(asnout);
    Taxon1ReqFree(taxrp);

    if((taxbp= NetTaxArchReadAsn()) == NULL) return NULL;
    
    if(taxbp->choice != Taxon1Resp_taxachildren) {
	report_service_error("txc_getChildren", taxbp);
	Taxon1RespFree(taxbp);
	return NULL;
    }

    tnp= taxbp->data.ptrvalue;
    taxbp->data.ptrvalue= NULL;
    Taxon1RespFree(taxbp);

    return tnp;
}


static Taxon1NamePtr getchildren(Int4 tax_id)
{
    Int4 i;
    short erract;
    ErrDesc err;
    Taxon1NamePtr tnp = NULL;

    for (i= TAXARCH_SERV_RETRIES; i >= 0; --i) {
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);

        tnp = s_getchildren(tax_id);

        ErrSetOpts(erract, 0);
        if (!ErrFetch(&err))  break; /* success */
	
	if(!ReestablishNetTaxArch()) break;
    }

    return(tnp);
}

TXC_TreeNodePtr* txc_getChildren(Int4 node_id, Int4Ptr nof_children)
{
    TXC_TreeNodePtr* lin;
    Int4 n= 0, i, k, k1;
    Taxon1NamePtr tnp, tnp_list= getchildren(node_id);

    if(nof_children != NULL) *nof_children= 0;

    if(tnp_list == NULL) return NULL;

    for(tnp= tnp_list; tnp != NULL; tnp= tnp->next) n++;

    if((lin= MemNew(n*sizeof(TXC_TreeNodePtr))) == NULL) {
	/* no memory, just free tnp_list */
	for(i= 0; i < n; i++) {
	    tnp= tnp_list->next;
	    Taxon1NameFree(tnp_list);
	    tnp_list= tnp;
	}
	return NULL;
    }

    for(i= 0; i < n; i++) {
	tnp= tnp_list->next;
	if(tnp_list->cde == 0) {
	    k= 0;
	    k1= 0;
	}
	else {
	    k= StringLen(tnp_list->oname);
	    k1= tnp_list->uname? (StringLen(tnp_list->uname) + 2) : 0;
	}
	if((lin[i]= MemNew(k+k1+10)) != NULL) {
	    lin[i]->tax_id= tnp_list->taxid;
	    lin[i]->flags= k? (tnp_list->cde | TXC_STHIDE) : 0;
	    if(k) {
		memcpy(lin[i]->node_label, tnp_list->oname, k+1);
		if(k1) {
		    memcpy(lin[i]->node_label + (k+1), tnp_list->uname, k1 - 1);
		    lin[i]->flags^= TXC_STHIDE;
		}
	    }
	    
	}
	Taxon1NameFree(tnp_list);
	tnp_list= tnp;
    }
	    
    if(nof_children != NULL) *nof_children= n;
    return lin;
}

static Taxon1InfoPtr s_getcde(void)
{
    Taxon1ReqPtr taxrp;
    Taxon1RespPtr taxbp;
    Taxon1InfoPtr tnp;

    if((taxrp= ValNodeNew(NULL)) == NULL) return NULL;
    taxrp->choice= Taxon1Req_getcde;
    taxrp->data.ptrvalue= NULL;
    Taxon1ReqAsnWrite(taxrp, asnout, NULL);
    AsnIoReset(asnout);
    Taxon1ReqFree(taxrp);

    if((taxbp= NetTaxArchReadAsn()) == NULL) return NULL;
    
    if(taxbp->choice != Taxon1Resp_getcde) {
	report_service_error("txc_loadNameClasses", taxbp);
	Taxon1RespFree(taxbp);
	return NULL;
    }

    tnp= taxbp->data.ptrvalue;
    taxbp->data.ptrvalue= NULL;
    Taxon1RespFree(taxbp);

    return tnp;
}


static Taxon1InfoPtr getcde(void)
{
    Int4 i;
    short erract;
    ErrDesc err;
    Taxon1InfoPtr tnp = NULL;

    for (i= TAXARCH_SERV_RETRIES; i >= 0; --i) {
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);

        tnp = s_getcde();

        ErrSetOpts(erract, 0);
        if (!ErrFetch(&err))  break; /* success */
	
	if(!ReestablishNetTaxArch()) break;
    }

    return(tnp);
}

Boolean txc_loadNameClasses(void)
{
    Taxon1InfoPtr tnp, tnp_list= getcde();

    if(tnp_list == NULL) return FALSE;

    while(tnp_list != NULL) {
	tax_addNameClass(tnp_list->ival1, tnp_list->sval, tnp_list->ival2);
	tnp= tnp_list->next;
	Taxon1InfoFree(tnp_list);
	tnp_list= tnp;
    }
    return TRUE;
}


static Taxon1InfoPtr s_getranks(void)
{
    Taxon1ReqPtr taxrp;
    Taxon1RespPtr taxbp;
    Taxon1InfoPtr tnp;

    if((taxrp= ValNodeNew(NULL)) == NULL) return NULL;
    taxrp->choice= Taxon1Req_getranks;
    taxrp->data.ptrvalue= NULL;
    Taxon1ReqAsnWrite(taxrp, asnout, NULL);
    AsnIoReset(asnout);
    Taxon1ReqFree(taxrp);

    if((taxbp= NetTaxArchReadAsn()) == NULL) return NULL;
    
    if(taxbp->choice != Taxon1Resp_getranks) {
	report_service_error("txc_loadRanks", taxbp);
	Taxon1RespFree(taxbp);
	return NULL;
    }

    tnp= taxbp->data.ptrvalue;
    taxbp->data.ptrvalue= NULL;
    Taxon1RespFree(taxbp);

    return tnp;
}


static Taxon1InfoPtr getranks(void)
{
    Int4 i;
    short erract;
    ErrDesc err;
    Taxon1InfoPtr tnp = NULL;

    for (i= TAXARCH_SERV_RETRIES; i >= 0; --i) {
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);

        tnp = s_getranks();

        ErrSetOpts(erract, 0);
        if (!ErrFetch(&err))  break; /* success */
	
	if(!ReestablishNetTaxArch()) break;
    }

    return(tnp);
}

Boolean txc_loadRanks(void)
{
    Taxon1InfoPtr tnp, tnp_list= getranks();

    if(tnp_list == NULL) return FALSE;

    while(tnp_list != NULL) {
	tax_addRank(tnp_list->ival1, tnp_list->sval);
	tnp= tnp_list->next;
	Taxon1InfoFree(tnp_list);
	tnp_list= tnp;
    }
    return TRUE;
}

static Taxon1InfoPtr s_getdivs(void)
{
    Taxon1ReqPtr taxrp;
    Taxon1RespPtr taxbp;
    Taxon1InfoPtr tnp;

    if((taxrp= ValNodeNew(NULL)) == NULL) return NULL;
    taxrp->choice= Taxon1Req_getdivs;
    taxrp->data.ptrvalue= NULL;
    Taxon1ReqAsnWrite(taxrp, asnout, NULL);
    AsnIoReset(asnout);
    Taxon1ReqFree(taxrp);

    if((taxbp= NetTaxArchReadAsn()) == NULL) return NULL;
    
    if(taxbp->choice != Taxon1Resp_getdivs) {
	report_service_error("txc_loadDivisions", taxbp);
	Taxon1RespFree(taxbp);
	return NULL;
    }

    tnp= taxbp->data.ptrvalue;
    taxbp->data.ptrvalue= NULL;
    Taxon1RespFree(taxbp);

    return tnp;
}


static Taxon1InfoPtr getdivs(void)
{
    Int4 i;
    short erract;
    ErrDesc err;
    Taxon1InfoPtr tnp = NULL;

    for (i= TAXARCH_SERV_RETRIES; i >= 0; --i) {
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);

        tnp = s_getdivs();

        ErrSetOpts(erract, 0);
        if (!ErrFetch(&err))  break; /* success */
	
	if(!ReestablishNetTaxArch()) break;
    }

    return(tnp);
}

Boolean txc_loadDivisions(void)
{
    Taxon1InfoPtr tnp, tnp_list= getdivs();
    Uint4 w;
    int i;
    unsigned char buff[8];

    if(tnp_list == NULL) return FALSE;

    while(tnp_list != NULL) {
	w= tnp_list->ival2;
	for(i= 0; i < 3; i++) {
	    buff[i]= (w >> (8*(3-i))) & 0xFF;
	}
	buff[3]= w & 0xFF;
	buff[4]= '\0';
	tax_addDivision(tnp_list->ival1, (CharPtr) buff, tnp_list->sval);
	tnp= tnp_list->next;
	Taxon1InfoFree(tnp_list);
	tnp_list= tnp;
    }
    return TRUE;
}

static Taxon1InfoPtr s_getgcs(void)
{
    Taxon1ReqPtr taxrp;
    Taxon1RespPtr taxbp;
    Taxon1InfoPtr tnp;

    if((taxrp= ValNodeNew(NULL)) == NULL) return NULL;
    taxrp->choice= Taxon1Req_getgcs;
    taxrp->data.ptrvalue= NULL;
    Taxon1ReqAsnWrite(taxrp, asnout, NULL);
    AsnIoReset(asnout);
    Taxon1ReqFree(taxrp);

    if((taxbp= NetTaxArchReadAsn()) == NULL) return NULL;
    
    if(taxbp->choice != Taxon1Resp_getgcs) {
	report_service_error("txc_loadGCs", taxbp);
	Taxon1RespFree(taxbp);
	return NULL;
    }

    tnp= taxbp->data.ptrvalue;
    taxbp->data.ptrvalue= NULL;
    Taxon1RespFree(taxbp);

    return tnp;
}


static Taxon1InfoPtr getgcs(void)
{
    Int4 i;
    short erract;
    ErrDesc err;
    Taxon1InfoPtr tnp = NULL;

    for (i= TAXARCH_SERV_RETRIES; i >= 0; --i) {
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);

        tnp = s_getgcs();

        ErrSetOpts(erract, 0);
        if (!ErrFetch(&err))  break; /* success */
	
	if(!ReestablishNetTaxArch()) break;
    }

    return(tnp);
}

Boolean txc_loadGCs(void)
{
    Taxon1InfoPtr tnp, tnp_list= getgcs();

    if(tnp_list == NULL) return FALSE;

    while(tnp_list != NULL) {
	tax_addGC(tnp_list->ival1, tnp_list->sval);
	tnp= tnp_list->next;
	Taxon1InfoFree(tnp_list);
	tnp_list= tnp;
    }
    return TRUE;
}


/*****************************************************************************
*
*   ReestablishNetTaxArch ()
*
*****************************************************************************/

static Boolean ReestablishNetTaxArch(void)
{
    return GenericReestablishNet("Taxon2", FALSE);
}

/*****************************************************************************
*
*   GenericReestablishNet ()
*
*****************************************************************************/

static Boolean GenericReestablishNet(CharPtr svcName, Boolean showErrs)
{
    MonitorPtr mon = NULL;
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

    if (!myNetInit())
    {
	if(mon) {
	    sprintf (buf, "%s get failed; re-contacting dispatcher", svcName);
	    MonitorStrValue(mon, buf);
	}
        retval = FALSE;
        if (ForceNetInit())
        { /* successfully established contact w/dispatcher */
	    if(mon) {
		sprintf (buf, "%s get failed; re-requesting %s service",
			 svcName, svcName);
		MonitorStrValue(mon, buf);
	    }
	    retval = myNetInit();
	}
        else {
            ErrPost(CTX_UNKNOWN, 1, "Unable to re-contact dispatcher");
            if (showErrs) {
                ErrShow();
            }
        }
    }

    if(mon) MonitorFree(mon);

    if (! retval )
    {
        sprintf (buf, "Unable to re-establish %s service", svcName);
        ErrPost(CTX_UNKNOWN, 1, buf);
        if (showErrs) {
            ErrShow();
        }
    }

    MemFree(buf);
    return retval;
}

/*****************************************************************************
*
*   NetInit ()
*
*****************************************************************************/

static Boolean NetInit(void)
{
    if (num_attached++ > 0)
        return TRUE;

    return ((dispatcher = NI_GenericInit(NULL, NULL, TRUE, NULL, 0)) != NULL);
}


/*****************************************************************************
*
*   ForceNetInit ()
*
*****************************************************************************/

static Boolean ForceNetInit(void)
{
    Boolean retval;

    reallyFinal = FALSE;
    num_attached = 0; /* force re-attempt to contact dispatcher */
    retval = NetInit();
    reallyFinal = TRUE;

    return retval;
}

/*****************************************************************************
*
*   NetFini ()
*
*****************************************************************************/

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


static _subspecPtr s_SSget(Int4 tax_id, _subspecPtr ss)
{
    Taxon1ReqPtr taxrp;
    Taxon1RespPtr taxbp;
    Taxon1InfoPtr tax_info;

    if((taxrp= ValNodeNew(NULL)) == NULL) return NULL;
    taxrp->choice= Taxon1Req_getorgmod;
    taxrp->data.ptrvalue= tax_info= Taxon1InfoNew();
    if(tax_info == NULL) {
	MemFree(taxrp);
	return NULL;
    }
    tax_info->ival1= tax_id;
    if(ss != NULL) {
	tax_info->ival2= ss->stype;
	tax_info->sval= ss->sname;
    }
    Taxon1ReqAsnWrite(taxrp, asnout, NULL);
    AsnIoReset(asnout);
    tax_info->sval= NULL;
    Taxon1ReqFree(taxrp);

    if((taxbp= NetTaxArchReadAsn()) == NULL) return NULL;
    
    if(taxbp->choice != Taxon1Resp_getorgmod) {
	report_service_error("tax_SSget", taxbp);
	Taxon1RespFree(taxbp);
	return NULL;
    }

    if(ss != NULL) {
	tax_info= taxbp->data.ptrvalue;
	if(tax_info != NULL) {	    
	    ss->r_id= tax_info->ival1;
	    ss->rtype= tax_info->ival2;
	    ss->rname= tax_info->sval;
	    tax_info->sval= NULL;
	}
	else ss= NULL;
	Taxon1RespFree(taxbp);
	return ss;
    }
    
    Taxon1RespFree(taxbp);

    return NULL;
}

_subspecPtr tax_SSget(Int4 tax_id, _subspecPtr ss)
{
    Int4 i;
    short erract;
    ErrDesc err;
    _subspecPtr res= NULL;

    if(tax_track) tax_time1= clock();
    for (i= TAXARCH_SERV_RETRIES; i >= 0; --i) {
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);

        res= s_SSget(tax_id, ss);

        ErrSetOpts(erract, 0);
        if (!ErrFetch(&err))  break; /* success */
	
	if(!ReestablishNetTaxArch()) break;
    }
    if(tax_track) {
	double diff= (double)(clock() - tax_time1)/CLOCKS_PER_SEC;
	printf("tax1_SSget() %f\n", diff);
    }

    return res;
}

static Int4 s_getTaxId4GI(Int4 gi)
{
    Taxon1ReqPtr taxrp;
    Taxon1RespPtr taxbp;
    Int4 tax_id;

    if((taxrp= ValNodeNew(NULL)) == NULL) return 0;
    taxrp->choice= Taxon1Req_id4gi;
    taxrp->data.intvalue= gi;
    Taxon1ReqAsnWrite(taxrp, asnout, NULL);
    AsnIoReset(asnout);
    Taxon1ReqFree(taxrp);

    if((taxbp= NetTaxArchReadAsn()) == NULL) return 0;
    
    if(taxbp->choice != Taxon1Resp_id4gi) {
	report_service_error("tax_getTaxId4GI", taxbp);
	Taxon1RespFree(taxbp);
	return 0;
    }

    tax_id= taxbp->data.intvalue;
    Taxon1RespFree(taxbp);

    return tax_id;
}

static Int4 getTaxId4GI(Int4 gi)
{
    Int4 i;
    short erract;
    ErrDesc err;
    Int4 tax_id= 0;

    for (i= TAXARCH_SERV_RETRIES; i >= 0; --i) {
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);

        tax_id = s_getTaxId4GI(gi);

        ErrSetOpts(erract, 0);
        if (!ErrFetch(&err))  break; /* success */
	
	if(!ReestablishNetTaxArch()) break;
    }

    return tax_id;
}

Int4 tax_getTaxId4GI(Int4 gi)
{
    return getTaxId4GI(gi);
}

static Int4 s_maxTaxId()
{
    Taxon1ReqPtr taxrp;
    Taxon1RespPtr taxbp;
    Int4 tax_id;

    if((taxrp= ValNodeNew(NULL)) == NULL) return 0;
    taxrp->choice= Taxon1Req_maxtaxid;
    Taxon1ReqAsnWrite(taxrp, asnout, NULL);
    AsnIoReset(asnout);
    Taxon1ReqFree(taxrp);

    if((taxbp= NetTaxArchReadAsn()) == NULL) return 0;
    
    if(taxbp->choice != Taxon1Resp_maxtaxid) {
	report_service_error("tax1e_maxTaxId", taxbp);
	Taxon1RespFree(taxbp);
	return 0;
    }

    tax_id= taxbp->data.intvalue;
    Taxon1RespFree(taxbp);

    return tax_id;
}

Int4 tax1e_maxTaxId()
{
    Int4 i;
    short erract;
    ErrDesc err;
    Int4 tax_id= 0;

    for (i= TAXARCH_SERV_RETRIES; i >= 0; --i) {
        ErrGetOpts(&erract, NULL);
        ErrSetOpts(ERR_IGNORE, 0);
        ErrFetch(&err);

        tax_id = s_maxTaxId();

        ErrSetOpts(erract, 0);
        if (!ErrFetch(&err))  break; /* success */
	
	if(!ReestablishNetTaxArch()) break;
    }

    return tax_id;
}
