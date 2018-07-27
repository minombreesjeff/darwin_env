/*  $Id: sock_srv.c,v 6.0 1999/04/01 22:12:32 vakatov Exp $
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
*  A simple test program to listen on a socket and just bounce all
*  incoming data(e.g., can be used to test "con_sock.[ch]")
*
* --------------------------------------------------------------------------
* $Log: sock_srv.c,v $
* Revision 6.0  1999/04/01 22:12:32  vakatov
* Initial revision
*
* ==========================================================================
*/

#ifndef _DEBUG
#define _DEBUG
#endif

#include <ncbi.h>
#include <ncbisock.h>


/* Listen on the specified "port".
 * Accept connections and bounce the incoming data back to the client
 * (only one client at a time).
 * Perform up to "n_cycle" sessions, then exit.
 */
static void s_DoServer(Uint2 port, Uint4 n_cycle)
{
  LSOCK         lsock;
  ESOCK_ErrCode err_code;

  ErrPostEx(SEV_INFO, 0, 0,
            "DoServer(port = %hu, n_cycle = %lu)",
            (unsigned short)port, (unsigned long)n_cycle);

  /* Create listening socket */
  err_code = LSOCK_Create(port, 1, &lsock);
  ASSERT(err_code == eSOCK_ESuccess);

  /* Accept connections from clients and run test sessions */
  while ( n_cycle-- ) {
    SOCK      sock;
    Nlm_Uint4 n_io, n_io_done;
    Nlm_Char  buf[1024];
    STimeout  timeout;

    /* Accept connection */
    err_code = LSOCK_Accept(lsock, 0, &sock);
    ASSERT(err_code == eSOCK_ESuccess);

    /* Set i/o timeouts for the accepted connection */
    timeout.sec  = 100;
    timeout.usec = 888;
    err_code = SOCK_SetTimeout(sock, eSOCK_OnRead,  &timeout, 0, 0);
    ASSERT(err_code == eSOCK_ESuccess);
    err_code = SOCK_SetTimeout(sock, eSOCK_OnWrite, &timeout, 0, 0);
    ASSERT(err_code == eSOCK_ESuccess);

    /* Bounce all incoming data back to the client */
    while ((err_code = SOCK_Read(sock, buf, sizeof(buf), &n_io))
           == eSOCK_ESuccess) {
      err_code = SOCK_Write(sock, buf, n_io, &n_io_done);
      if (err_code != eSOCK_ESuccess) {
        ErrPostEx(SEV_WARNING, 0, 0,
                  "Failed to write -- DoServer(n_cycle = %lu): %s",
                  (unsigned long)n_cycle, SOCK_ErrCodeStr(err_code));
        break;
      }
    }
    ASSERT(err_code == eSOCK_EClosed);

    /* Close connection */
    err_code = SOCK_Close(sock);
    ASSERT(err_code == eSOCK_ESuccess || err_code == eSOCK_EClosed);
  }

  /* Close listening socket */
  err_code = LSOCK_Close(lsock);
  ASSERT(err_code == eSOCK_ESuccess);
}


extern Nlm_Int2 Nlm_Main(void)
{
#define MIN_PORT 5001
  Nlm_Int4     argc = Nlm_GetArgc();
  Nlm_CharPtr* argv = Nlm_GetArgv();
  int port    = 0;
  int n_cycle = 100;

#ifdef WIN16
  {{ /* a kludge to make sure the "vibwndws.c"(MainProc) get linked */
    extern void Nlm_Metronome(Nlm_VoidPtr actn);  Nlm_Metronome(0);
  }}
#endif

  Nlm_ErrSetOpts(ERR_TEE, ERR_LOG_ON);
  ErrSetLogLevel(SEV_INFO);
  ErrSetMessageLevel(SEV_INFO);
  VERIFY ( Nlm_ErrSetLog("sock_srv.log") );

  switch ( argc ) {
  case 3:
    if (sscanf(argv[2], "%d", &n_cycle) != 1  ||
        n_cycle <= 0  ||  n_cycle > UINT4_MAX)
      break;

  case 2:
    if (sscanf(argv[1], "%d", &port) != 1  ||
        port < MIN_PORT  ||  port > UINT2_MAX)
      break;
  }

  if (port == 0) {
    ErrPostEx(SEV_ERROR, 0, 0,
              "Usage: %s <port> [n_cycle]\n where <port> not less than %hd",
              argv[0], (short)MIN_PORT);
    return 1;
  }

  s_DoServer((Uint2)port, (Uint4)n_cycle);

  VERIFY ( SOCK_Destroy() == eSOCK_ESuccess );
  return 0;
}
