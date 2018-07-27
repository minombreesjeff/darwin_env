/*  $RCSfile: ni_serv.c,v $  $Revision: 6.1 $  $Date: 2001/02/21 22:09:15 $
 * ==========================================================================
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
 * ==========================================================================
 *
 * Author:  Anton Lavrentiev
 *
 * File Description:
 *   NCBI Named Service Client (based on SERVICE connector)
 *
 * --------------------------------------------------------------------------
 * $Log: ni_serv.c,v $
 * Revision 6.1  2001/02/21 22:09:15  lavr
 * Initial revision
 *
 * ==========================================================================
 */

#include <ncbi.h>
#include <ncbinet.h>
#include <connect/ncbi_connection.h>
#include <connect/ncbi_service_connector.h>


/*********************************
 * INTERNALS
 */


/* Static functions
 */

static Int2 LIBCALLBACK s_AsnRead(Pointer p, CharPtr buff, Uint2 len)
{
    size_t n_read = 0;
    CONN_Read((CONN)p, buff, len, &n_read, eIO_Plain);
    return (Int2)n_read;
}


static Int2 LIBCALLBACK s_AsnWrite(Pointer p, CharPtr buff, Uint2 len)
{
    size_t n_written = 0;
    CONN_Write((CONN)p, buff, len, &n_written);
    return (Int2)n_written;
}


static void LIBCALLBACK s_AsnErrorFunc(Int2 type, CharPtr message)
{
    ErrPostEx(SEV_ERROR, 88, type, message);
}


/* The interface implementaion functions
 */

static NI_DispatcherPtr s_GenericInit
(CharPtr configFile, CharPtr configSection, Boolean showMonitor,
 CharPtr lastDispatcher, Int2 lastDispLen)
{
    NI_DispatcherPtr disp = (NI_DispatcherPtr)MemNew(sizeof(NI_Dispatcher));
    disp->interface = eNII_Service;
    
    if ( lastDispatcher )
        StringNCpy_0(lastDispatcher, "NCBI Named Service", lastDispLen);
    
    disp->motd = StringSave("New load-balanced service resolution facility");
    disp->adminInfo = StringSave("Anton Lavrentiev (lavr@ncbi.nlm.nih.gov)");
    disp->referenceCount = 1;
    return disp;
}


static NI_DispatcherPtr s_SetDispatcher
(NI_DispatcherPtr disp, CharPtr host, CharPtr svc, int timeout,
 Int4 dispserialnum, ValNodePtr encryption, Boolean useOutServ)
{
    return s_GenericInit(0, 0, 0, 0, 0);
}


static NI_HandPtr s_GenericGetService
(NI_DispatcherPtr disp, CharPtr configFile, CharPtr configSection,
 CharPtr defService, Boolean hasResource)
{
    NI_HandPtr result;
    CONN       conn;
    CONNECTOR  c;
    
    /* establish connection to the server */
    if (!(c = SERVICE_CreateConnector(defService)) ||
        CONN_Create(c, &conn) != eIO_Success) {
        ErrPostEx(SEV_ERROR, 0, 1,
                  "[Service NI Client]  Cannot use service \"%s\"",
                  defService);
        return 0;
    }
    
    /* open ASN i/o, etc. */
    result = (NI_HandPtr)MemNew(sizeof(NI_Handle));
    result->extra_proc_info = conn;
    result->raip = AsnIoNew((ASNIO_BIN | ASNIO_IN),  (FILE *)0,
                            (void *)conn, s_AsnRead,  (IoFuncType)0);
    result->waip = AsnIoNew((ASNIO_BIN | ASNIO_OUT), (FILE *)0,
                            (void *)conn, (IoFuncType)0, s_AsnWrite);
    
    AsnIoSetErrorMsg(result->raip, s_AsnErrorFunc);
    AsnIoSetErrorMsg(result->waip, s_AsnErrorFunc);
    result->hostname = StringSave("");
    result->disp = disp;
    disp->referenceCount++;
    
    return result;
}


static Int2 s_EndServices(NI_DispatcherPtr disp)
{
    ASSERT ( disp->referenceCount > 0 );
    if (--disp->referenceCount == 0) {
        MemFree(disp->adminInfo);
        MemFree(disp->motd);
        MemFree(disp);
    }
    return 0;
}


static Int2 s_ServiceDisconnect(NI_HandPtr mhp)
{
    s_EndServices(mhp->disp);
    CONN_Close((CONN)mhp->extra_proc_info);
    AsnIoClose(mhp->raip);
    AsnIoClose(mhp->waip);
    MemFree(mhp->hostname);
    MemFree(mhp);
    return 0;
} 


/* Exported table of interface functions
 */

static const NIInterface s_NII_Service = {
    s_GenericInit,
    s_SetDispatcher,
    s_GenericGetService,
    s_ServiceDisconnect,
    s_EndServices
};
const NIInterface *g_NII_Service = &s_NII_Service;

/* EOF */
