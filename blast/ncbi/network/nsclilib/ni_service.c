/*  $RCSfile: ni_service.c,v $  $Revision: 6.22 $  $Date: 2005/04/20 14:30:55 $
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
 * $Log: ni_service.c,v $
 * Revision 6.22  2005/04/20 14:30:55  lavr
 * Allow zero timeout -- final
 *
 * Revision 6.21  2005/04/19 19:03:15  lavr
 * Allow zero timeout -- correction
 *
 * Revision 6.20  2005/04/19 18:57:57  lavr
 * Allow zero timeout
 *
 * Revision 6.19  2004/07/12 16:34:05  lavr
 * Add "SRV_CONN_MODE" for alternate FIREWALL switching
 * (Thanks to Svetlana Karamycheva for reporting this..)
 *
 * Revision 6.18  2004/02/23 17:00:39  lavr
 * Fix n_written variable name typo
 *
 * Revision 6.17  2004/02/23 15:30:21  lavr
 * New (last) parameter "how" added in CONN_Write() API call
 *
 * Revision 6.16  2003/10/27 14:10:49  lavr
 * Better flow control in s_AsnWrite()
 *
 * Revision 6.15  2003/10/16 15:12:38  lavr
 * Better flow control in s_AsnWrite()
 *
 * Revision 6.14  2003/10/10 19:23:58  lavr
 * Perform persistent ASN.1 write to connection in a tight loop
 * (work around bug in AsnIoWriteBlock that otherwise generates err 79)
 *
 * Revision 6.13  2002/11/26 17:00:11  lavr
 * Recognize "SOME" and "DATA" as keyword values of "SRV_DEBUG_PRINTOUT"
 *
 * Revision 6.12  2002/10/04 15:33:42  lavr
 * Few cosmetic changes
 *
 * Revision 6.11  2002/08/16 20:36:19  lavr
 * Do not override net params which are not defined by old env.interface
 *
 * Revision 6.10  2002/08/08 02:45:28  lavr
 * Minor change
 *
 * Revision 6.9  2002/08/07 18:45:21  lavr
 * Change from deprecated to current EIO_ReadMethod enums
 *
 * Revision 6.8  2002/08/06 20:36:22  lavr
 * Uppercase environment variable name to look for an alternate service name
 *
 * Revision 6.7  2002/05/06 19:24:38  lavr
 * Translate service name AFTER creation of SConnNetInfo
 *
 * Revision 6.6  2002/04/23 17:57:54  lavr
 * Recognize "INFINITE" as a timeout from registry/environment
 *
 * Revision 6.5  2002/04/16 21:58:06  lavr
 * Few fixes after test compilation and run
 *
 * Revision 6.4  2002/04/16 21:33:24  lavr
 * Add compatibility for service parameters taken as for WWW disp
 *
 * Revision 6.3  2002/03/23 04:21:04  lavr
 * Typo corrected
 *
 * Revision 6.2  2002/03/22 22:22:45  lavr
 * Try to do the best in setting up proper timeouts
 *
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

/* Hard-coded constants, environment parameter names & defaults
 * NOTE:: These are taken from ni_www.c for backward compatibility.
 *        Their use will eventually get deprecated in the future...
 */

#define SRV_SECTION         "NET_SERV"

#define ENV_CONN_MODE       "SRV_CONN_MODE"
#define ENV_ENGINE_HOST     "SRV_ENGINE_HOST"
#define ENV_ENGINE_PORT     "SRV_ENGINE_PORT"
#define ENV_ENGINE_URL      "SRV_ENGINE_URL"
#define ENV_TIMEOUT         "SRV_CONN_TIMEOUT"
#define ENV_CONN_TRY        "SRV_CONN_TRY"
#define ENV_HTTP_PROXY_HOST "SRV_HTTP_PROXY_HOST"
#define ENV_HTTP_PROXY_PORT "SRV_HTTP_PROXY_PORT"
#define ENV_PROXY_HOST      "SRV_PROXY_HOST"
#define ENV_DEBUG_PRINTOUT  "SRV_DEBUG_PRINTOUT"
#define ENV_NO_LB_DIRECT    "SRV_NO_LB_DIRECT"


/* Static functions
 */

static Int2 LIBCALLBACK s_AsnRead(Pointer p, CharPtr buf, Uint2 len)
{
    size_t n_read;
    CONN_Read((CONN) p, buf, len, &n_read, eIO_ReadPlain);
    return (Int2) n_read;
}


static Int2 LIBCALLBACK s_AsnWrite(Pointer p, CharPtr buf, Uint2 len)
{
    size_t n_written;
    CONN_Write((CONN) p, buf, len, &n_written, eIO_WritePersist);
    return (Int2) n_written;
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
    
    disp->motd = StringSave("Load-Balanced Service Mapping Facility");
    disp->adminInfo = StringSave("Anton Lavrentiev <lavr@ncbi.nlm.nih.gov>");
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
    SConnNetInfo *net_info, *def_info;
    Char          str[64];
    NI_HandPtr    result;
    double        valf;
    CONN          conn;
    int           val;
    CONNECTOR     c;

    if (!(net_info = ConnNetInfo_Create(defService))) {
        ErrPostEx(SEV_ERROR, 0, 1, "[Service NI Client] "
                  " Cannot set parameters for service \"%s\"", defService);
        return 0;
    }
    if (!(def_info = ConnNetInfo_Clone(net_info))) {
        ErrPostEx(SEV_ERROR, 0, 1, "[Service NI Client] "
                  " Cannot create reserve copy of network info");
        ConnNetInfo_Destroy(net_info);
        return 0;
    }
    /* Now override default parameters with legacy parameters
     * of older WWW service dispatcher -- should go obsolete soon... */

    /* alternate firewall mode request */
    NI_GetEnvParam(configFile, SRV_SECTION, ENV_CONN_MODE,
                   str, sizeof(str), "");
    if (*str  &&  StringICmp(str, "FIREWALL") == 0)
        net_info->firewall = 1/*true*/;

    /* alternate dispatcher's host name & port */
    NI_GetEnvParam(configFile, SRV_SECTION, ENV_ENGINE_HOST,
                   net_info->host, sizeof(net_info->host), def_info->host);

    NI_GetEnvParam(configFile, SRV_SECTION, ENV_ENGINE_PORT,
                   str, sizeof(str), "");
    if (*str  &&  (val = atoi(str)) != 0)
        net_info->port = val;

    /* alternate the dispatcher's CGI path */
    NI_GetEnvParam(configFile, SRV_SECTION, ENV_ENGINE_URL,
                   net_info->path, sizeof(net_info->path), def_info->path);

    /* alternate HTTP proxy host & port */
    NI_GetEnvParam(configFile, SRV_SECTION, ENV_HTTP_PROXY_HOST,
                   net_info->http_proxy_host,
                   sizeof(net_info->http_proxy_host),
                   def_info->http_proxy_host);
    NI_GetEnvParam(configFile, SRV_SECTION, ENV_HTTP_PROXY_PORT,
                   str, sizeof(str), "");
    if (*str  &&  (val = atoi(str)) != 0)
        net_info->http_proxy_port = val;

    /* alternate non-transparent CERN-like firewall proxy server */
    NI_GetEnvParam(configFile, SRV_SECTION, ENV_PROXY_HOST,
                   net_info->proxy_host, sizeof(net_info->proxy_host),
                   def_info->proxy_host);

    /* alternate connection timeout */
    NI_GetEnvParam(configFile, SRV_SECTION, ENV_TIMEOUT,
                   str, sizeof(str), "");
    if (*str) {
        if (strlen(str) > 2 && StringNICmp(str, "infinite", strlen(str)) == 0){
            net_info->timeout = 0;
        } else {
            net_info->timeout = &net_info->tmo;
            if ((valf = atof(str)) < 0.0)
                valf = DEF_CONN_TIMEOUT;
            net_info->timeout->sec  = (unsigned int) valf;
            net_info->timeout->usec = (unsigned int)
                ((valf - net_info->timeout->sec) * 1000000);
        }
    }

    /* alternate max. number of attempts to establish connection */
    NI_GetEnvParam(configFile, SRV_SECTION, ENV_CONN_TRY,
                   str, sizeof(str), "");
    if (*str  &&  (val = atoi(str)) != 0)
        net_info->max_try = val;

    /* alternate debug printout request */
    NI_GetEnvParam(configFile, SRV_SECTION, ENV_DEBUG_PRINTOUT,
                   str, sizeof(str), "");
    if (*str  &&  (StringICmp(str, "1"   ) == 0 ||
                   StringICmp(str, "true") == 0 ||
                   StringICmp(str, "yes" ) == 0 ||
                   StringICmp(str, "some") == 0)) {
        net_info->debug_printout = eDebugPrintout_Some;
    }
    if (*str  &&   StringICmp(str, "data") == 0)
        net_info->debug_printout = eDebugPrintout_Data;

    /* whether to prohibit the use of local LBSMD */
    NI_GetEnvParam(configFile, SRV_SECTION, ENV_NO_LB_DIRECT,
                   str, sizeof(str), "");
    if (*str  &&  (StringICmp(str, "0"    ) != 0 &&
                   StringICmp(str, "false") != 0 &&
                   StringICmp(str, "no"   ) != 0)) {
        net_info->lb_disable = 1/*true*/;
    }

    {{ /* alternate service name */
        static const Char ENV_PREFIX[] = "NI_SERVICE_NAME_";
        CharPtr envName = (CharPtr)MemNew(sizeof(ENV_PREFIX) +
                                          StringLen(configSection));
        StringCpy(envName, ENV_PREFIX);
        StringCat(envName, configSection);
        StringUpper(envName);
        NI_GetEnvParamEx(configFile, configSection, envName, "SERVICE_NAME",
                         str, sizeof(str), defService);
        MemFree(envName);
    }}

    ConnNetInfo_Destroy(def_info);

    /* establish connection to the server */
    if (!(c = SERVICE_CreateConnectorEx(str, fSERV_Any, net_info, 0)) ||
        CONN_Create(c, &conn) != eIO_Success) {
        ErrPostEx(SEV_ERROR, 0, 1, "[Service NI Client] "
                  " Service \"%s\" %s", str, c ? "unusable" : "not found");
        ConnNetInfo_Destroy(net_info);
        return 0;
    }
    CONN_SetTimeout(conn, eIO_Open,      net_info->timeout);
    CONN_SetTimeout(conn, eIO_ReadWrite, net_info->timeout);
    CONN_SetTimeout(conn, eIO_Close,     net_info->timeout);

    /* open ASN i/o, etc. */
    if ((result = (NI_HandPtr) MemNew(sizeof(NI_Handle))) != 0) {
        result->extra_proc_info = conn;
        result->raip = AsnIoNew(ASNIO_BIN | ASNIO_IN,  (FILE*) 0,
                                (void*) conn, s_AsnRead,  (IoFuncType) 0);
        result->waip = AsnIoNew(ASNIO_BIN | ASNIO_OUT, (FILE*) 0,
                                (void*) conn, (IoFuncType) 0, s_AsnWrite);
        AsnIoSetErrorMsg(result->raip, s_AsnErrorFunc);
        AsnIoSetErrorMsg(result->waip, s_AsnErrorFunc);
        result->hostname = StringSave(net_info->client_host);
        result->disp = disp;
        disp->referenceCount++;
    } else
        CONN_Close(conn);
    ConnNetInfo_Destroy(net_info);

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
