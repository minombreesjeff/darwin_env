/*  $RCSfile: ncbicli.c,v $  $Revision: 4.20 $  $Date: 2000/12/01 23:06:07 $
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
* Author:  Denis Vakatov
*
* File Description:
*   API to open(using HTTPD-based dispatcher) and handle socket-based
*   connection to an NCBI service
*
* --------------------------------------------------------------------------
* $Log: ncbicli.c,v $
* Revision 4.20  2000/12/01 23:06:07  vakatov
* s_SendHeaderWWW() -- added missing \r before \n (HTTP requirement)
*
* Revision 4.19  2000/02/09 15:54:31  vakatov
* NIC_GetService() -- Changed error message format
*
* Revision 4.18  1999/06/24 17:59:35  vakatov
* Untie the HTTP and regular proxy servers: SRV_HTTP_PROXY_***, SRV_PROXY_HOST
*
* Revision 4.17  1999/03/12 22:38:23  vakatov
* More detailed debug printout in "s_ProcessReply_WWWClient()"
*
* Revision 4.16  1999/03/11 19:26:30  vakatov
* s_ProcessReply_WWWClient():  Use nic->server_host(Int4) rather then
* nic->server_host(unsigned long) in the SOCK_Address()
*
* Revision 4.15  1999/03/11 15:20:20  vakatov
* Added "timeout" arg to SOCK_Create() and SOCK_Reconnect()
*
* Revision 4.14  1999/01/22 22:04:58  vakatov
* Uint4toInaddr() to take address in the network byte order
*
* Revision 4.13  1998/12/15 17:26:53  vakatov
* + Handle the case of connecting through a CERN-like non-transparent proxy
*
* Revision 4.12  1998/10/13 20:51:14  vakatov
* Added to HTTP header:  &platform=<Nlm_PlatformName()> -- to report
* the client's platform
*
* Revision 4.11  1998/09/08 17:59:03  vakatov
* Added WWW/Firewall network interface
*
* Revision 4.10  1998/08/05 20:19:17  vakatov
* [eNIC_WWWClient]  NIC_GetService():  send the bytestore contents to
* server rather than to dispatcher
*
* Revision 4.9  1998/05/08 15:22:33  vakatov
* NIC_GetService():  added more detailed error diagnostics
*
* Revision 4.8  1998/05/05 22:31:08  vakatov
* Redesigned some code for the sake of simplicity
*
* Revision 4.7  1998/04/30 22:07:25  vakatov
* NIC_GetService():  set the regular connection timeout to infinite
*
* Revision 4.6  1998/04/03 16:08:37  vakatov
* s_NIC_DispConnectRequest(): minor fix in retrieving client host name
*
* Revision 4.5  1998/04/02 20:21:09  vakatov
* Added possibility to printout service reply headers
*
* Revision 4.4  1998/03/30 17:50:08  vakatov
* Ingrafted to the main NCBI CVS tree
*
* ==========================================================================
*/

#include <ncbi.h>
#include <ncbicli.h>

#define ENIC_DEFAULT eNIC_WWWClient


/***********************************************************************
 *  INTERNAL
 ***********************************************************************/

typedef struct Nlm_NICtag
{
  SOCK    sock;
  CharPtr client_host;
  Uint4   server_host;  /* local   byte-order */
  Uint2   server_port;  /* local   byte-order */
  Uint4   ticket;       /* network byte-order */
} Nlm_NICstruct;


/* Compose and send an HTTP header-request
 */
static Boolean s_SendHeaderWWW
(SOCK        sock,
 const Char* service_name,
 const Char* disp_path,
 const Char* client_agent,
 const Char* client_host,
 Uint4       content_length)
{
  static char X_POST_1[] = "POST ";
  static char X_POST_2[] = "?service=";
  static char X_POST_3[] = "&address=";
  static char X_POST_4[] = "&platform=";
  static char X_POST_E[] = " HTTP/1.0\r\n";
  static char X_AGENT_1[] = "User-Agent: ";
  static char X_AGENT_2[] = " from ";

  Char buffer[128];
  if (SOCK_Write(sock, (const void *)X_POST_1,  StrLen(X_POST_1 ), 0)
      != eSOCK_ESuccess  ||
      SOCK_Write(sock, (const void *)disp_path,
                 StrLen(disp_path), 0) != eSOCK_ESuccess  ||
      SOCK_Write(sock, (const void *)X_POST_2,  StrLen(X_POST_2 ), 0)
      != eSOCK_ESuccess  ||
      SOCK_Write(sock, (const void *)service_name,
                 StrLen(service_name), 0) != eSOCK_ESuccess  ||
      SOCK_Write(sock, (const void *)X_POST_3,  StrLen(X_POST_3 ), 0)
      != eSOCK_ESuccess  ||
      SOCK_Write(sock, (const void *)client_host,
                 StrLen(client_host), 0) != eSOCK_ESuccess  ||
      SOCK_Write(sock, (const void *)X_POST_4,  StrLen(X_POST_4 ), 0)
      != eSOCK_ESuccess  ||
      SOCK_Write(sock, (const void *)Nlm_PlatformName(),
                 StrLen(Nlm_PlatformName()), 0) != eSOCK_ESuccess  ||
      SOCK_Write(sock, (const void *)X_POST_E,  StrLen(X_POST_E ), 0)
      != eSOCK_ESuccess  ||
      SOCK_Write(sock, (const void *)X_AGENT_1, StrLen(X_AGENT_1), 0)
      != eSOCK_ESuccess  ||
      SOCK_Write(sock, (const void *)client_agent,
                 StrLen(client_agent), 0)
      != eSOCK_ESuccess  ||
      SOCK_Write(sock, (const void *)X_AGENT_2, StrLen(X_AGENT_2), 0)
      != eSOCK_ESuccess  ||
      SOCK_Write(sock, (const void *)client_host,
                 StrLen(client_host), 0)
      != eSOCK_ESuccess  ||
      sprintf(buffer, "\r\nContent-type: application/x-www-form-urlencoded"
              "\r\nContent-Length: %lu\r\n\r\n", (unsigned long)content_length)
      <= 0  ||
      SOCK_Write(sock, (const void *)buffer,    StrLen(buffer   ), 0)
      != eSOCK_ESuccess) {
    ErrPostEx(SEV_ERROR, NIC_ERROR, 4,
              "[NIC_GetService]  Error sending HTTP header");
    return FALSE;
  }
  return TRUE;
}


/* Send the bytestore content
 */
static Boolean s_SendBS(SOCK sock, const ByteStore *service_query)
{
  const Nlm_BSUnit *bsup;
  for (bsup = service_query->chain;  bsup  &&  bsup->len;  bsup = bsup->next) {
    const void *ptr = Nlm_HandLock(bsup->str);
    ESOCK_ErrCode err_code = SOCK_Write(sock, ptr, (Uint4)bsup->len, 0);
    Nlm_HandUnlock(bsup->str);
    if (err_code != eSOCK_ESuccess) {
      ErrPostEx(SEV_ERROR, NIC_ERROR, 5,
                "[NIC_GetService]  Error sending service query (%s)",
                SOCK_ErrCodeStr(err_code));
      return FALSE;
    }
  }
  return TRUE;
}


/* Skip "n_skip" bytes in the socket;
 * on error, return non-zero value(number of bytes that could not be skipped)
 */
static Uint4 s_NIC_SockSkip(SOCK sock, Uint4 n_skip)
{
  ESOCK_ErrCode err_code = eSOCK_ESuccess;
  while (n_skip  &&  err_code == eSOCK_ESuccess) {
    Char  buffer[1024];
    Uint4 n_io, n_io_done;

    n_io = MIN(sizeof(buffer), n_skip);
    err_code = SOCK_Read(sock, buffer, n_io, &n_io_done);
    n_skip -= n_io_done;
  }

  if ( n_skip )
    ErrPostEx(SEV_ERROR, NIC_ERROR, 0,
              "[s_NIC_SockSkip] Cannot dummy-read requested # of bytes");

  return n_skip;
}


/* Get and process reply from dispatcher(WWW Special-Client agent)
 */
static Boolean s_ProcessReply_WWWClient(NIC nic, const STimeout *timeout,
                                        const Char *proxy_host,
                                        Boolean debug_printout)
{
  Uint4         buf_read;
  Char          buffer[1024];
  ESOCK_ErrCode err_code;

  buf_read = 0;
  for (;;) {{ /* TRY */
    static const Char X_TAG[] = "SERVICE_ENGINE_HOST=";
    Char  *str;
    Uint4 n_read;

    /* read the next piece of data */
    err_code = SOCK_Read(nic->sock, buffer + buf_read,
                         sizeof(buffer) - buf_read - 1, &n_read);
    if (err_code != eSOCK_ESuccess) {
      ErrPostEx(SEV_WARNING, NIC_ERROR, 6,
                "[WWW Special] Error in reading reply from the dispatcher"
                " (%s)", SOCK_ErrCodeStr(err_code));
      break;
    }
    buf_read += n_read;
    ASSERT( buf_read < sizeof(buffer) );
    buffer[buf_read] = '\0';

    if ( debug_printout ) {
      fprintf(stderr, "[WWW Special Client, reply %lu:%lu]: \"%s\"\n",
              (unsigned long)(buf_read - n_read), (unsigned long)buf_read,
              buffer + buf_read - n_read);
    }

    /* parse for the reply tag and read info on the "real" server */
    str = StrStr(buffer, X_TAG);
    if ( str )  { /* read info after the tag */
      unsigned long server_host, server_port, ticket, dummy;
      if (sscanf(str + StrLen(X_TAG), "%lx %lx %lx %lx",
                 &server_host, &server_port, &ticket, &dummy) != 4) {
        buf_read = StrLen(str);
        MemMove(buffer, str, (size_t)(buf_read + 1));
        continue; /* yet incomplete... -- continue reading */
      }

      /* the dispatcher's reply has been parsed successfully */
      nic->server_host = server_host;
      nic->server_port = (Uint2)server_port;
      nic->ticket      = Nlm_htonl((Uint4)ticket);
      break;
    }

    /* avoid overflow in the read buffer */
#define SHIFT_SIZE (sizeof(buffer) / 2)
    if (buf_read > SHIFT_SIZE + sizeof(X_TAG)) {
      buf_read -= SHIFT_SIZE;
      MemMove(buffer, buffer + SHIFT_SIZE, (size_t)(buf_read + 1));
    }
#undef  SHIFT_SIZE 
  }} /* end-of-TRY */

  SOCK_Close(nic->sock); /* dont need the dispatcher anymore... */
  nic->sock = 0;

  if ( !nic->server_host )
    return FALSE; /* i.e. the reply is bad or missing */

  {{ /* Connect to the "real" server and send back the ticket */
    if (proxy_host  &&  *proxy_host) {
      StringNCpy_0(buffer, proxy_host, sizeof(buffer));
    } else {
      VERIFY( Uint4toInaddr(Nlm_htonl(nic->server_host),
                            buffer, sizeof(buffer)) );
    }

    if (SOCK_Create(buffer, nic->server_port, timeout, &nic->sock)
        != eSOCK_ESuccess) {
      ErrPostEx(SEV_ERROR, NIC_ERROR, 7,
                "[WWW Special] Cannot connect to server \"%s\", port %u",
                buffer, (unsigned int)nic->server_port);
      return FALSE;
    }
    if ( debug_printout ) {
      fprintf(stderr,
              "[WWW Special Client] Connected to server \"%s\", port %u\n",
              buffer, (unsigned int)nic->server_port);
    }

    /* store the server host */
    SOCK_Address(nic->sock, &nic->server_host, 0, FALSE);


    VERIFY( SOCK_SetTimeout(nic->sock, eSOCK_OnReadWrite, timeout, 0, 0)
            == eSOCK_ESuccess );

    if (SOCK_Write(nic->sock, (const void*)&nic->ticket, sizeof(nic->ticket),
                   0) != eSOCK_ESuccess) {
      ErrPostEx(SEV_ERROR, NIC_ERROR, 8,
                "[WWW Special] Cannot send the ticket back to server");
      return FALSE;
    }
    if ( debug_printout ) {
      fprintf(stderr, "[WWW Special Client] Sent ticket back to the server\n");
    }
  }}

  return TRUE;
}


/* Get and process reply from dispatcher(WWW Direct agent for stateless conn.)
 */
static Boolean s_ProcessReply_WWWDirect(NIC nic, Boolean debug_printout)
{
  /* (just skip all data up to the first '\0') */
  for (;;) {  
    Char  buffer[1024];
    Uint4 n_read, i_null;
    Uint4 n_total = 0;
    ESOCK_ErrCode err_code;

    err_code = SOCK_Peek(nic->sock, buffer, sizeof(buffer)-1, &n_read);
    if (err_code != eSOCK_ESuccess) {
      ErrPostEx(SEV_WARNING, NIC_ERROR, 6,
                "[WWW Direct] Error in reading reply from the dispatcher"
                " (%s)", SOCK_ErrCodeStr(err_code));
      return FALSE;
    }
    buffer[n_read] = '\0';

    if ( debug_printout ) {
      fprintf(stderr,
              "[WWW Direct Client, reply %lu:%lu]: \"%s\"",
              (unsigned long)n_total, (unsigned long)(n_total + n_read),
              buffer);
    }
    n_total += n_read;

    if ( !n_read ) {
      ASSERT( 0 );
      return FALSE;
    }

    for (i_null = 0;  buffer[i_null]  &&  i_null < n_read;  i_null++)
      continue;
    
    if (i_null == n_read)
      i_null = n_read - 1;

    if (s_NIC_SockSkip(nic->sock, i_null+1) != 0)
        return FALSE;

    if ( !buffer[i_null] )
      break; /* success */
  }

  return TRUE;
}



/***********************************************************************
 *  EXTERNAL
 ***********************************************************************/


NLM_EXTERN NIC NIC_GetService
(const Char      *service_name,
 const Char      *disp_host,
 Uint2            disp_port,
 const Char      *disp_path,
 const STimeout  *timeout,
 ENIC_Agent       client_agent,
 const Char      *client_host,
 const Char      *proxy_host,
 const ByteStore *service_query,
 TNIC_Flags       flags)
{
  NIC  nic = (NIC)MemNew(sizeof(Nlm_NICstruct));

  /* use the default agent, if specified */
  if (client_agent == eNIC_Default)
    client_agent = ENIC_DEFAULT;

  for (;;) {{ /* TRY to establish a connection */
    /* connect to dispatcher(or server) */
    if (SOCK_Create(disp_host, disp_port, timeout, &nic->sock)
        != eSOCK_ESuccess) {
      ErrPostEx(SEV_ERROR, NIC_ERROR, 1,
                "[NIC_GetService]  Cannot connect to host \"%s\", port %d;",
                disp_host, (int)disp_port);
      break;
    }

    /* retrieve the local host name */
    if (client_host  &&  *client_host) {
      nic->client_host = StringSave(client_host);
    } else {
      Char buffer[64];
      if ( !GetHostName(buffer, sizeof(buffer)) ) {
        ErrPostEx(SEV_WARNING, NIC_ERROR, 3,
                  "[NIC_GetService] Cannot get the local host name");
        buffer[0] = '\0';
      }
      nic->client_host = StringSave(buffer);
    }

    /* setup the connection i/o timeout(for the handshake stage only) */
    if (SOCK_SetTimeout(nic->sock, eSOCK_OnReadWrite, timeout, 0, 0)
        != eSOCK_ESuccess) {
      ErrPostEx(SEV_ERROR, NIC_ERROR, 2,
                "[NIC_GetService]  Cannot setup timeout for the connection"
                " handshake with host \"%s\", port %d",
                disp_host, (int)disp_port);
      break;
    }

    /* send HTTP header, get reply from dispatcher and process it */
    if (client_agent == eNIC_WWWClient) {
      /* send HTTP header */
      if ( !s_SendHeaderWWW(nic->sock, service_name, disp_path,
                            (flags & NIC_FIREWALL) ?
                            "NCBISpecialFWClient" : "NCBISpecialClient",
                            nic->client_host, 0) )
        break;

      /* process reply from the dispatcher */
      if ( !s_ProcessReply_WWWClient
           (nic, timeout, proxy_host,
            (Boolean)(flags & NIC_DEBUG_PRINTOUT)) )
        break;
    } else { /* i.e. eNIC_WWWDirect */
      /* send HTTP header(along with the bytestore content, if any) */
      if ( !s_SendHeaderWWW(nic->sock, service_name, disp_path,
                            "NCBIDirectClient", nic->client_host,
                            (Uint4)BSLen((ByteStorePtr)service_query)) )
        break;

      if (service_query  &&  !s_SendBS(nic->sock, service_query))
        break;

      /* process reply from the dispatcher */
      if (!s_ProcessReply_WWWDirect
          (nic, (Boolean)(flags & NIC_DEBUG_PRINTOUT)) )
        break;
    }

    /* set back to infinite timeout for the regular connection i/o */
    if (SOCK_SetTimeout(nic->sock, eSOCK_OnReadWrite, 0, 0, 0)
        != eSOCK_ESuccess) {
      ErrPostEx(SEV_ERROR, NIC_ERROR, 9,
                "[NIC_GetService]  Cannot set an infinite timeout for the"
                " regular client-server socket connection");
      break;
    }

    /* for "eNIC_WWWClient", send the bytestore content(query) now */
    if (client_agent == eNIC_WWWClient  &&  service_query  &&
        !s_SendBS(nic->sock, service_query))
      break;

    /* success */
    return nic;
  }} /* end-of-TRY */

  /* error */
  ErrPostEx(SEV_ERROR, NIC_ERROR, 10,
            "[NIC_GetService]  Failed to establish connection to dispatcher"
            " at \"%s:%d\", path \"%s\", for service \"%s\"",
            disp_host, (int)disp_port, disp_path, service_name);
  NIC_CloseService(nic);
  return 0;
}


NLM_EXTERN SOCK NIC_GetSOCK(NIC nic)
{
  return nic->sock;
}


NLM_EXTERN Boolean NIC_CloseService(NIC nic)
{
  if (nic->sock  &&  SOCK_Close(nic->sock) != eSOCK_ESuccess)
    return FALSE;

  MemFree(nic->client_host);
  MemFree(nic);
  return TRUE;
}



#ifdef TEST_MODULE__NCBICLI
/***********************************************************************
 *  TEST
 ***********************************************************************/

#define TEST_ENGINE_HOST     "www.ncbi.nlm.nih.gov"
#define TEST_ENGINE_PORT     80
#define TEST_ENGINE_URL      "/Service/nph-dispd.cgi"

#define ASS_RET(expr,retcode) \
if ( !(expr) ) { ASSERT( 0 );  return retcode; } else {;}


static Nlm_Int2 TEST__ncbicli_1(SOCK sock)
{
#ifdef __TODO
  ESOCK_ErrCode err_code;
  Char buf[1234];
  Uint4 n_io = sizeof(buf);
  Uint4 n_io_done;

  err_code = SOCK_Write(sock, buf, n_io, &n_io_done);
  ASS_RET((err_code == eSOCK_ESuccess), 11);

  err_code = SOCK_Read(sock, buf, n_io, &n_io_done);
  ASS_RET((err_code == eSOCK_ESuccess), 12);
#endif /* __TODO */

  return 0;
}

static Nlm_Int2 TEST__ncbicli(ENIC_Agent agent)
{
  static Char service_name[]  = "Entrez";
  Char        *service_query = service_name;

  NIC          nic;
  SOCK         sock;
  STimeout     timeout;
  Int2         ret_code;
  ByteStorePtr service_query_bsp;

  timeout.sec  = 10;
  timeout.usec =  0;

  service_query_bsp = BSNew(StrLen(service_query) + 1);
  BSWrite(service_query_bsp, service_query, StrLen(service_query)+1);
  nic = NIC_GetService(service_name,
                       TEST_ENGINE_HOST, TEST_ENGINE_PORT, TEST_ENGINE_URL,
                       &timeout, agent, 0, 0, service_query_bsp,
                       NIC_DEBUG_PRINTOUT);
  ASS_RET(nic, 1);
  BSFree(service_query_bsp);

  sock = NIC_GetSOCK(nic);
  ASS_RET(sock, 2);

  ret_code = TEST__ncbicli_1(sock);
  ASS_RET((ret_code == 0), ret_code);

  ASS_RET(NIC_CloseService(nic), 9);
  return 0;
}


extern Nlm_Int2 Nlm_Main(void)
{
  Int2 status;

#ifdef WIN16
  {{ /* a kludge to make sure the "vibwndws.c"(MainProc) get linked */
    extern void Nlm_Metronome(Nlm_VoidPtr actn);  Nlm_Metronome(0);
  }}
#endif

  Nlm_ErrSetOpts(ERR_TEE, ERR_LOG_ON);
  ErrSetLogLevel(SEV_INFO);
  ErrSetMessageLevel(SEV_INFO);
  VERIFY( Nlm_ErrSetLog("ncbicli.log") );

  Nlm_ErrPostEx(SEV_INFO, 2, 3, "NCBICLI test started...\n");

  status = TEST__ncbicli(eNIC_WWWClient);
  ASS_RET((status == 0), 1);

  status = TEST__ncbicli(eNIC_WWWDirect);
  ASS_RET((status == 0), 2);

  status = TEST__ncbicli(eNIC_Default);
  ASS_RET((status == 0), 3);

  VERIFY( SOCK_Destroy() == eSOCK_ESuccess );

  return status;
}

#endif /* TEST_MODULE__NCBICLI */

/* EOF */
