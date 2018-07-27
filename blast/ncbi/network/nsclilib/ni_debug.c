/* $Id: ni_debug.c,v 6.6 2004/11/18 19:39:23 kans Exp $
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
 * Author:  Denis Vakatov
 *
 * File Description:
 *   Basic client code for the debug NCBI client that connects
 *   directly to the standalone server (bypasses NCBI dispatchers)
 *
 * --------------------------------------------------------------------------
 * $Log: ni_debug.c,v $
 * Revision 6.6  2004/11/18 19:39:23  kans
 * calls to SOCK_Read and SOCK_Write needed to pass size_t pointer in order to compile under CodeWarrior
 *
 * Revision 6.5  2004/11/18 14:46:18  lavr
 * Use newer ncbi_socket.h API
 *
 * Revision 6.4  2003/10/27 14:11:10  lavr
 * Old (Shavirin's) dispatchers disabled unconditionally
 *
 * Revision 6.3  2001/02/21 22:09:27  lavr
 * SERVICE connector added
 *
 * Revision 6.2  1999/03/11 15:20:22  vakatov
 * Added "timeout" arg to SOCK_Create() and SOCK_Reconnect()
 *
 * Revision 6.1  1998/05/05 22:23:35  vakatov
 * Initial revision
 *
 * ==========================================================================
 */

#include <ncbi.h>
#include <ncbinet.h>
#include <connect/ncbi_socket.h>


/*********************************
 * INTERNALS
 */

/* Hard-coded constants, environment parameter names & defaults
 */

#define ENV_DEBUG_HOST      "NI_DEBUG_HOST"
#define ENV_DEBUG_PORT      "NI_DEBUG_PORT"

#define ENV_DEBUG_TRY       "NI_DEBUG_TRY"
#define DEF_DEBUG_TRY       2


/* Static functions
 */

static Int2 LIBCALLBACK s_AsnRead(Pointer p, CharPtr buff, Uint2 len)
{
  size_t n_read = 0;
  SOCK_Read((SOCK)p, buff, len, &n_read, eIO_ReadPlain);
  return (Int2)n_read;
}


static Int2 LIBCALLBACK s_AsnWrite(Pointer p, CharPtr buff, Uint2 len)
{
  size_t n_written = 0;
  SOCK_Write((SOCK)p, buff, len, &n_written, eIO_WritePlain);
  return (Int2)n_written;
}


static void LIBCALLBACK s_AsnErrorFunc(Int2 type, CharPtr message)
{
  ErrPostEx(SEV_ERROR, 77, type, message);
}


/* The interface implementaion functions
 */

static NI_DispatcherPtr s_GenericInit
(CharPtr configFile, CharPtr configSection, Boolean showMonitor,
 CharPtr lastDispatcher, Int2 lastDispLen)
{
  NI_DispatcherPtr disp = (NI_DispatcherPtr)MemNew(sizeof(NI_Dispatcher));
  disp->interface = eNII_Debug;

  if ( lastDispatcher )
    StringNCpy_0(lastDispatcher, "Debug NCBI Network Client", lastDispLen);

  disp->motd = StringSave("Makes a direct connection to the specified server");
  disp->adminInfo = StringSave("Denis V. Vakatov "
                               "(vakatov@ncbi.nlm.nih.gov)");
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
  SOCK       sock;
  Char       srv_host[64];
  Uint2      srv_port;
  Uint4      conn_try;

  /* get the server host name */
  NI_GetEnvParam(configFile, configSection, ENV_DEBUG_HOST,
                 srv_host, sizeof(srv_host), "");
  if ( !srv_host[0] ) {
    ErrPostEx(SEV_ERROR, 0, 0, ENV_DEBUG_HOST ": undefined");
    return 0;
  }

  {{ /* get the server port */
    Char str[32];
    int  val;
    NI_GetEnvParam(configFile, configSection, ENV_DEBUG_PORT,
                   str, sizeof(str), "");
    val = atoi(str);
    if (val <= 0) {
      ErrPostEx(SEV_ERROR, 0, 0, ENV_DEBUG_PORT ": bad(%d) or undefined", val);
      return 0;
    }
    srv_port = (Uint2)val;
  }}

  {{ /* alternate the max. number of attemts to establish a connection */
    Char str[32];
    int  val;
    NI_GetEnvParam(configFile, configSection, ENV_DEBUG_TRY,
                   str, sizeof(str), "");
    val = atoi(str);
    conn_try = (Uint4)((val > 0) ? val : DEF_DEBUG_TRY);
  }}

  /* establish connection to the server */
  for (sock = 0;  !sock  &&  conn_try;  conn_try--) {
    if (SOCK_Create(srv_host, srv_port, 0, &sock) != eIO_Success) {
      ErrPostEx(SEV_WARNING, 0, 1,
                "[Debug NI Client]  Cannot connect to host \"%s\", port %d;",
                srv_host, (int)srv_port);
    }
  }
  if ( !sock ) {
    ErrPostEx(SEV_ERROR, 0, 1,
              "[Debug NI Client]  Failed to connect to host \"%s\", port %d;",
              srv_host, (int)srv_port);
    return 0;
  }

  /* open ASN i/o, etc. */
  result = (NI_HandPtr)MemNew(sizeof(NI_Handle));
  result->extra_proc_info = sock;
  result->raip = AsnIoNew((ASNIO_BIN | ASNIO_IN),  (FILE *)0,
                          (void *)sock, s_AsnRead,  (IoFuncType)0);
  result->waip = AsnIoNew((ASNIO_BIN | ASNIO_OUT), (FILE *)0,
                          (void *)sock, (IoFuncType)0, s_AsnWrite);

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
  SOCK_Close((SOCK)mhp->extra_proc_info);
  AsnIoClose(mhp->raip);
  AsnIoClose(mhp->waip);
  MemFree(mhp->hostname);
  MemFree(mhp);
  return 0;
} 


/* Exported table of interface functions
 */

static const NIInterface s_NII_Debug = {
  s_GenericInit,
  s_SetDispatcher,
  s_GenericGetService,
  s_ServiceDisconnect,
  s_EndServices
};
const NIInterface *g_NII_Debug = &s_NII_Debug;

/* EOF */
