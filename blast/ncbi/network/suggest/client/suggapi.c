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
* File Name: suggapi.c
*
* Author:  Yuri Sadykov
*
* Version Creation Date:   08/14/95
*
* $Revision: 6.1 $
*
* File Description: 
*	Implementation of API for Suggest service
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
*/

#include <ncbinet.h>
#include "suggapi.h"

/*****************************************************************************
 * Global variables
 */
static NI_HandPtr	svcp = NULL;
static Boolean		num_attached = 0;
static NI_DispatcherPtr	dispatcher;
static AsnIoPtr	pAsnIn = NULL;
static AsnIoPtr	pAsnOut = NULL;
static Boolean	bSendFini = TRUE;

/*****************************************************************************
 * Function prototipes
 */
static Boolean
NetInit PROTO((void));

static SuggestResponsePtr
NetSuggestReadAsn PROTO((void));

static void
ErrorFromServer PROTO((SuggestErrorPtr pError));

static Boolean
NetFini PROTO((void));


/******************************************************************************\
 * Exported functionts which are in API
\******************************************************************************/

/*============================================================================*\
 * Function:
 *	SuggestInit
 *
 * Purpose:
 *	Initializes Suggest API
 *
 * Parameters:
 *	none
 *
 * Return:
 *	TRUE if successful, FALSE otherwise
 *
\*----------------------------------------------------------------------------*/
Boolean
SuggestInit(void)
{
    SuggestRequestPtr	pRequest;
    SuggestResponsePtr	pResponse;

/*    myNetInit = SuggestInit;
*/
    if (!NetInit())
        return FALSE;

    svcp = NI_GenericGetService(dispatcher, NULL, "SUGGEST", "Suggest", TRUE);
    if (svcp == NULL)
    {
        ErrPostEx(SEV_ERROR, 0, 0, "NI_ServiceGet [%s] (%s)",
		  ni_errlist[ni_errno], ni_errtext);
        SuggestFini();

        return FALSE;
    }

    pAsnIn = svcp->raip;
    pAsnOut = svcp->waip;

    pRequest = ValNodeNew(NULL);
    pRequest->choice = SuggestRequest_init;
    SuggestRequestAsnWrite (pRequest, pAsnOut, NULL);
    AsnIoReset(pAsnOut);
    SuggestRequestFree(pRequest);

    if ((pResponse = NetSuggestReadAsn()) == NULL)
    {
        return FALSE;
    }

    if (pResponse->choice == SuggestResponse_error) {
	ErrorFromServer(pResponse->data.ptrvalue);
	if (((SuggestErrorPtr)pResponse->data.ptrvalue)->level
	    == Suggest_error_level_fatal)
	    bSendFini = FALSE;
	SuggestFini();
	SuggestResponseFree(pResponse->data.ptrvalue);
	
	return FALSE;
    }
    
    pResponse->data.ptrvalue = NULL;
    SuggestResponseFree(pResponse);

    return TRUE;
}

/*============================================================================*\
 * Function:
 *	SuggestFini
 *
 * Purpose:
 *	Frees all resources allocated by the API.
 *
 * Parameters:
 *	none
 *
 * Return:
 *	TRUE if successful, FALSE otherwise
 *
\*----------------------------------------------------------------------------*/
Boolean
SuggestFini(void)
{
    short erract;
    ErrDesc err;
    Boolean retval = TRUE;

    ErrGetOpts(&erract, NULL);
    ErrSetOpts(ERR_IGNORE, 0);
    ErrFetch(&err);

    if (pAsnOut != NULL && pAsnIn != NULL && bSendFini)
    {
	SuggestRequestPtr	pReq;
	SuggestResponsePtr	pResp;

        pReq = ValNodeNew(NULL);
        pReq->choice = SuggestRequest_fini;
        SuggestRequestAsnWrite(pReq, pAsnOut, NULL);
        AsnIoReset(pAsnOut);
        SuggestRequestFree(pReq);

	if ((pResp = NetSuggestReadAsn()) == NULL) {
	    retval = FALSE;
	} else {
	    pResp->data.ptrvalue = NULL;
	    SuggestResponseFree(pResp);
	    retval = TRUE;
	}
    }

    NetFini();

    ErrSetOpts(erract, 0);
    ErrFetch(&err);

    return retval;
}

/*============================================================================*\
 * Function:
 *	SuggestFindIntervals
 *
 * Purpose:
 *
 *
 * Parameters:
 *	pIntervals	- pointer to SuggestIntervals structure which contains
 *		the data for processing;
 *
 * Return:
 *	pointer to SeqAnnot structure which will contain the result of
 * 	the processing or NULL if error occurred.
 *
 * Note:
 *	SuggestResponse structure can contain an error from Suggest Server, so
 *	if this function returns not NULL it does not mean that all right. The
 *	caller of this function should check the structure on errors.
 *
 *	This function does not free input structure pointed pIntervals.
 *
\*----------------------------------------------------------------------------*/
SeqAnnotPtr SuggestFindIntervals(SuggestIntervalsPtr pIntervals)
{
    SuggestRequestPtr	pReq;
    SuggestResponsePtr	pResp;
    SeqAnnotPtr		pSeqAnnot;

    if (pIntervals == NULL)
	return NULL;

    /* Create request and send it to the server
     */
    pReq = ValNodeNew(NULL);
    if (pReq == NULL)
	return NULL;
	
    pReq->choice = SuggestRequest_intervals;
    pReq->data.ptrvalue = pIntervals;
    SuggestRequestAsnWrite(pReq, pAsnOut, NULL);
    AsnIoReset(pAsnOut);
    
    pReq->data.ptrvalue = NULL;	/* The function doesn't free input data */
    SuggestRequestFree(pReq);

    /* Read response from the server
     */
    pResp = NetSuggestReadAsn();
    if (pResp == NULL)
	return NULL;

    if (pResp->choice == SuggestResponse_error) {
	/* Error occurred on server's side, report it
	 */
	ErrorFromServer(pResp->data.ptrvalue);
	SuggestResponseFree(pResp);

	return NULL;
    }

    pSeqAnnot = pResp->data.ptrvalue;
    pResp->data.ptrvalue = NULL;
    SuggestResponseFree(pResp);
    
    return pSeqAnnot;
}


/******************************************************************************\
 * Internal functions which specific to internal implementation of the API
\******************************************************************************/

/*============================================================================*\
 * Function:
 *	NetInit
 *
 * Purpose:
 *
 *
 * Parameters:
 *	none
 *
 * Return:
 *	TRUE if successful, FALSE otherwise
 *
\*----------------------------------------------------------------------------*/
static Boolean
NetInit(void)
{
    if (num_attached++ > 0)
        return TRUE;

    return ((dispatcher = NI_GenericInit(NULL, NULL, TRUE, NULL, 0)) != NULL);
}

/*============================================================================*\
 * Function:
 *	NetSuggestReadAsn
 *
 * Purpose:
 *	Reads SuggestResponse from network file.
 *
 * Parameters:
 *	none
 *
 * Return:
 *	Pointer to read SuggestResponse structure.
 *
\*----------------------------------------------------------------------------*/
static SuggestResponsePtr
NetSuggestReadAsn(void)
{
    SuggestResponsePtr pSuggResp;
    short erract;
    ErrDesc err;

    ErrGetOpts(&erract, NULL);
    ErrSetOpts(ERR_IGNORE, 0);
    ErrFetch(&err); /* clear any pending error */

    pSuggResp = SuggestResponseAsnRead(pAsnIn, NULL);

    if (ErrFetch(&err))
    {
        ErrPost(CTX_UNKNOWN, 1, "Null message read from server");
    }
    ErrSetOpts(erract, 0);

    return pSuggResp;
}

/*============================================================================*\
 * Function:
 *	ErrorFromServer
 *
 * Purpose:
 *	Reports message on error which was send by suggest server
 *
 * Parameters:
 *	pError	 - pointer on SuggestError structure which was sent
 *		by the server;
 *
 * Return:
 *	none
 *
\*----------------------------------------------------------------------------*/
static void
ErrorFromServer(SuggestErrorPtr pError)
{
    char	*pchErrorType;
    ErrSev	errSev;
    
    if (pError == NULL)
	return;

    switch (pError->level) {
      case Suggest_error_level_none:
	return;
	
      case Suggest_error_level_info:
	pchErrorType = "INFO";
	errSev = SEV_INFO;
	break;
	
      case Suggest_error_level_warn:
	pchErrorType = "WARNING";
	errSev = SEV_WARNING;
	break;
	
      case Suggest_error_level_error:
	pchErrorType = "ERROR";
	errSev = SEV_ERROR;
	break;
	
      case Suggest_error_level_fatal:
	pchErrorType = "FATAL";
	errSev = SEV_ERROR;
	break;
    }
		    
    ErrPostEx(errSev, 0, 0, "%s from Suggest Server: %s",
	      pchErrorType, pError->msg);
}

/*============================================================================*\
 * Function:
 *	NetFini
 *
 * Purpose:
 *	
 *
 * Parameters:
 *
 *
 * Return:
 *	TRUE if successful, FALSE otherwise
 *
\*----------------------------------------------------------------------------*/
static Boolean
NetFini(void)
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
