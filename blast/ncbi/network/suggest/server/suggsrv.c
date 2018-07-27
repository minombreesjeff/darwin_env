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
* File Name: suggsrv.c
*
* Author:  Yuri Sadykov
*
* Version Creation Date:   08/14/95
*
* $Revision: 6.1 $
*
* File Description: 
*       Implementation of Suggest server. Here it is only server specific
*       part, mainly related with reading of requests from a client and writing
*       responces. The implementaion of the processing of data is in suggest.c
*       and adjust.c
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
* $Log: suggsrv.c,v $
* Revision 6.1  2004/04/01 13:43:07  lavr
* Spell "occurred", "occurrence", and "occurring"
*
* Revision 6.0  1997/08/25 18:40:59  madden
* Revision changed to 6.0
*
* Revision 1.6  1996/07/09 21:40:29  epstein
* add kludge Nlm_Main() to allow program to be built with shared libs
*
 * Revision 1.5  1995/12/12  18:52:54  kans
 * now uses functions in suggslp.c
 *
 * Revision 1.4  1995/11/27  21:03:40  sad
 * Fixed error related with initializing global structure;
 *
 * Fixed error while writing interval's fields from and to, which had
 * values less then required;
 *
 * Added writing strand values in output intervals;
 *
 * Now all intervals with the same product are within single feature;
 *
*/

#include <ncbinet.h>
#include <seqport.h>
#include <suggslp.h>
#include <suggen.h>

/*****************************************************************************
 * External function prototipes
 */

/*****************************************************************************
 * Function prototipes
 */
static Boolean
ServeClient(AsnIoPtr pAsnIn, AsnIoPtr pAsnOut);

static SuggestErrorPtr
GetLastSuggestError(void);

static MsgAnswer LIBCALLBACK
SuggestMessageHook(MsgKey msgKey, ErrSev errSev,
                   const char *pchCaption, const char *pchMessage);

static void
SetSuggestMessageHook(void);

static void
RestoreMessageHook(void);

static SuggestErrorPtr
GetLastSuggestError(void);

static void
ClearSuggestError(void);

/*============================================================================*\
 * Function:
 *      main
 *
 * Purpose:
 *      Entry point. This function is for suggest if it is built as a server. 
 *
 * Parameters:
 *      Only one option to the application is acceptable "-d" which means that
 * the application must running without network capability and input and output 
 * of data are through the local files. It's very convinient for debugging.
 *
 * Return:
 *      Error code
\*----------------------------------------------------------------------------*/
int
main(int argc, char *argv[])
{
    int         iArg;
    int         iReadTimeout;
    Boolean     bDebug = FALSE;
    AsnIoPtr    pAsnIoIn;
    AsnIoPtr    pAsnIoOut;
    Char        achBuff[100];
    NI_HandPtr  pHand;
    
    if (argc > 1) {
        iArg = 1;
        if (StrCmp(argv[1], "-d") == 0) {
            iArg++;
            bDebug = TRUE;
        }
    }

    if (bDebug) {
        /* For debugging of the server only.
         * Read and write local files only, no communication
         * with clients.
         */
        pAsnIoIn = AsnIoOpen("suggsrv.in", "r");
        pAsnIoOut = AsnIoOpen("suggsrv.out", "w");
    } else {
        /* Send acknowledgment--all right
         */
        NI_ServerACK();

        /* Create connection with client, i.e. input and output
         */
        pHand = NI_OpenASNIO();

        /* this read-timeout is effectively an idle timeout for
         * the server process; the process will terminate upon
         * read-timeout
         */
        GetAppParam("NCBI", "NET_SRV", "SERV_INACT_TIMER",
                    "10", achBuff, sizeof(achBuff));
        iReadTimeout = atoi(achBuff) * 60;      /* param is minutes */
        MsgSetReadTimeout(pHand, iReadTimeout);
        pAsnIoIn = pHand->raip;
        pAsnIoOut = pHand->waip;
    }

    suggestOut.bServer = TRUE;

    /* Serve a client
     */
    while (ServeClient(pAsnIoIn, pAsnIoOut))
        ;

    AsnIoClose(pAsnIoIn);
    AsnIoClose(pAsnIoOut);
}

/*****************************************************************************\
 * Function:
 *      ServeClient
 *
 * Purpose:
 *      Reads one request from a client and sends to it the result of
 *      processing.
 *
 * Parameter:
 *      pAsnIoIn        - input file from which the request are read;
 *      pAsnIoOut       - output file to which 
 *
 * Return:
 *      FALSE if nothing more to process or error occurred and farther
 *      processing is not possible, otherwise TRUE.
 *
\*****************************************************************************/
static Boolean
ServeClient(AsnIoPtr pAsnIn, AsnIoPtr pAsnOut)
{
    short       iErrAct;
    ErrDesc     err;
    Boolean     bOnceMore = TRUE;
    Uint2       errLevel;
    SuggestErrorPtr     pSError;
    SuggestRequestPtr   pReq;
    SuggestResponsePtr  pResp;
    SuggestIntervalsPtr pSIntervals;

    /* Encountering  EOF on reading is a "normal" occurrence,
     * and does not merit an error message
     */
    ErrGetOpts(&iErrAct, NULL);
    ErrSetOpts(ERR_IGNORE, 0);
    ErrFetch(&err);     /* clear any pending error, which can be ignored */

    pReq = SuggestRequestAsnRead (pAsnIn, NULL);

    if (ErrFetch(&err)) {
        ErrPostEx(SEV_ERROR, 1, 1, "Error encountered on AsnReadId %d", err);
        
        return FALSE;
    }
    
    ErrSetOpts(iErrAct, 0);
    
    if (pReq == NULL) {
        ErrPostEx(SEV_ERROR, 1, 1, "Null AsnReadId");

        return FALSE;
    }

    switch (pReq->choice) {
      case SuggestRequest_init: /* Initializing */
        pResp = ValNodeNew(NULL);
        SetSuggestMessageHook();
        if (InitSuggest()) {
            /* Init successful, send OK.
             */
            pResp->choice = SuggestResponse_init;
            pResp->data.ptrvalue = NULL;
        } else {
            /* Error while init, send error
             */
            pResp->choice = SuggestResponse_error;
            pResp->data.ptrvalue = (Pointer)GetLastSuggestError();
            bOnceMore = FALSE;  /* Cancel farther processing */
        }
        SuggestResponseAsnWrite(pResp, pAsnOut, NULL);
        RestoreMessageHook();
        pResp->data.ptrvalue = NULL;    /* It's not necessary to free
                                         * SuggestError structure.
                                         */
        SuggestResponseFree(pResp);
        pReq->data.ptrvalue = NULL;
        break;
        
      case SuggestRequest_intervals:    /* What this server does */
        pSIntervals = pReq->data.ptrvalue;
        
        pResp = ValNodeNew(NULL);
        SetSuggestMessageHook();
        
        /* The main task of this application, process the data
         */
        SuggestClientService (pSIntervals);
        
        /* Check for error while the processing
         */
        pSError = GetLastSuggestError();
        errLevel = pSError->level;
        
        if (errLevel == Suggest_error_level_error ||
            errLevel == Suggest_error_level_fatal)
        {
            /* Fatal error occurs, send error message to athe client
             * and flags that the application must exit
             */
            pResp->choice = SuggestResponse_error;
            pResp->data.ptrvalue = GetLastSuggestError();
            if (errLevel == Suggest_error_level_fatal)
                bOnceMore = FALSE;

/****       SeqAnnotFree(suggestOut.out.pSeqAnnot); */
        } else {
            pResp->choice = SuggestResponse_intervals;
            pResp->data.ptrvalue = suggestOut.out.pSeqAnnot;
        }
            
        SuggestResponseAsnWrite(pResp, pAsnOut, NULL);
        RestoreMessageHook();
        SuggestResponseFree(pResp);
        suggestOut.out.pSeqAnnot = NULL;
        break;
        
      case SuggestRequest_fini: /* Deinitializing */
        pResp = ValNodeNew(NULL);
        pResp->choice = SuggestResponse_fini;
        pResp->data.ptrvalue = NULL;
        SuggestResponseAsnWrite(pResp, pAsnOut, NULL);
        SuggestResponseFree(pResp);
        pReq->data.ptrvalue = NULL;
        bOnceMore = FALSE;
        break;

      default:
        /* All request types must be processed.
         * No default processing.
         */
        ASSERT(FALSE);
    }

    AsnIoReset(pAsnOut);
    SuggestRequestFree(pReq);   /* free read request */

    return bOnceMore;
}

static MsgAnswer LIBCALLBACK
SuggestMessageHook(MsgKey msgKey, ErrSev errSev,
                   const char *pchCaption, const char *pchMessage)
{
    switch (errSev) {
      case MSG_ERROR:
        suggestError.level = Suggest_error_level_error;
        break;
        
      case MSG_FATAL:
        suggestError.level = Suggest_error_level_fatal;
        break;
    }

    if (suggestError.msg != NULL)
        MemFree(suggestError.msg);

    suggestError.msg = StringSave(pchMessage);
    
    return ANS_NONE;
}

/*============================================================================*\
 * Function:
 *      SetSuggesstMessageHook
 *
 * Purpose:
 *      Sets hook function for intercepting all calls to Message
 *
 * Parameters:
 *      None
 *
 * Return:
 *      None
 *
\*----------------------------------------------------------------------------*/
static void
SetSuggestMessageHook(void)
{
    fOldMsgHook = SetMessageHook(SuggestMessageHook);
    ClearSuggestError();
}

/*============================================================================*\
 * Function:
 *      RestoreMessageHook
 *
 * Purpose:
 *      Resotore old hook function.
 *
 * Parameters:
 *      none
 *
 * Return:
 *      none
 *
\*----------------------------------------------------------------------------*/
static void
RestoreMessageHook(void)
{
    fOldMsgHook = SetMessageHook(fOldMsgHook);
    ClearSuggestError();
}

/*============================================================================*\
 * Function:
 *      GetLastSuggestError
 *
 * Purpose:
 *      
 *
 * Parameters:
 *      none
 *
 * Return:
 *      pointer to SuggestError structure, which contains information about
 * last error.
 *
\*----------------------------------------------------------------------------*/
static SuggestErrorPtr
GetLastSuggestError(void)
{
    return &suggestError;
}

/*============================================================================*\
 * Function:
 *      ClearSuggestError
 *
 * Purpose:
 *      Sets last error to a state which corresponds to no error.
 *
 * Parameters:
 *      none
 *
 * Return:
 *      none
 *
\*----------------------------------------------------------------------------*/
static void
ClearSuggestError(void)
{
    suggestError.level = Suggest_error_level_none;
    if (suggestError.msg != NULL)
        MemFree(suggestError.msg);

    suggestError.msg = NULL;
}

Int2
Nlm_Main(void)
{
    fprintf(stderr,"Kludge main executed in file %s", __FILE__);
    exit (1);
}
