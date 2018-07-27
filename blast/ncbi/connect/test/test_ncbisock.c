/* $Id: test_ncbisock.c,v 6.1 1999/10/18 15:40:50 vakatov Exp $
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
 *   Test suite for "ncbisock.[ch]"
 *
 * ---------------------------------------------------------------------------
 * $Log: test_ncbisock.c,v $
 * Revision 6.1  1999/10/18 15:40:50  vakatov
 * Initial revision
 *
 * ===========================================================================
 */

#include <ncbilcl.h>
#if defined(OS_UNIX)
#  include <unistd.h>
#  define X_SLEEP(x) ((void) sleep(x))
#elif defined(OS_MSWIN)
#  include <windows.h>
#  define X_SLEEP(x) ((void) Sleep(1000 * x))
#else
#  define X_SLEEP(x) ((void) 0)
#endif

#include <ncbi.h>
#include <ncbisock.h>


#define TEST_BUFSIZE 8192

/* exit server before sending the data expected by client(Test 1) */
/*#define TEST_SRV1_SHUTDOWN*/

#ifndef TEST_SRV1_SHUTDOWN
/* exit server immediately after its first client is served(Test 1) */
/*#define TEST_SRV1_ONCE*/
#endif


#define ASS_RET(expr,retcode) \
if ( !(expr) ) { ASSERT ( 0 );  return retcode; } else {;}


/* The simplest randezvous(plain request-reply) test functions
 *      "TEST__client_1(SOCK sock)"
 *      "TEST__server_1(SOCK sock)"
 */

static const char s_C1[] = "C1";
static const char s_S1[] = "S1";

#define N_SUB_BLOB    10
#define SUB_BLOB_SIZE 7000
#define BIG_BLOB_SIZE (N_SUB_BLOB * SUB_BLOB_SIZE)


static Int2 TEST__client_1(SOCK sock)
{ /* reserved ret.codes [110-119] */
  ESOCK_ErrCode err_code;
  Uint4         n_io, n_io_done;
  char          buf[TEST_BUFSIZE];

  ErrPostEx(SEV_INFO, SOCK_ERRCODE, 110, "TC1()");

  /* Send a short string */
  n_io = StrLen(s_C1) + 1;
  err_code = SOCK_Write(sock, s_C1, n_io, &n_io_done);
  ASS_RET((err_code == eSOCK_ESuccess  &&  n_io == n_io_done), 101);

  n_io = StrLen(s_S1) + 1;
  err_code = SOCK_Read(sock, buf, n_io, &n_io_done);
  if (err_code == eSOCK_EClosed) {
    ErrPostEx(SEV_WARNING, SOCK_ERRCODE, 103, "TC1(): connection closed");
    return 103;
  }
  ASS_RET((err_code == eSOCK_ESuccess  &&  n_io == n_io_done), 104);
  ASS_RET((StrCmp(buf, s_S1) == 0), 105);
  ASS_RET(SOCK_PushBack(sock, buf, n_io_done) == eSOCK_ESuccess, 106);
  MemSet(buf, '\xFF', n_io_done);
  ASS_RET(SOCK_Read(sock, buf, n_io_done, &n_io_done) == eSOCK_ESuccess, 107);
  ASS_RET((StrCmp(buf, s_S1) == 0), 108);

  /* Send a very big binary blob */
  {{
    size_t i;
    char* blob = (char*) MemNew(BIG_BLOB_SIZE);
    for (i = 0;  i < BIG_BLOB_SIZE;  blob[i] = (char)i, i++)
      continue;
    for (i = 0;  i < 10;  i++) {
      err_code = SOCK_Write(sock, blob + i * SUB_BLOB_SIZE, SUB_BLOB_SIZE,
                            &n_io_done);
      ASS_RET((err_code == eSOCK_ESuccess  &&  n_io_done==SUB_BLOB_SIZE), 109);
    }
    MemFree(blob);
  }}

  return 0;
}


static Int2 TEST__server_1(SOCK sock)
{ /* reserved ret.codes [210-219] */
  ESOCK_ErrCode err_code;
  Uint4         n_io, n_io_done;
  char          buf[TEST_BUFSIZE];

  ErrPostEx(SEV_INFO, SOCK_ERRCODE, 210, "TS1()");

  /* Receive and send back a short string */
  n_io = StrLen(s_C1) + 1;
  err_code = SOCK_Read(sock, buf, n_io, &n_io_done);
  ASS_RET((err_code == eSOCK_ESuccess  &&  n_io == n_io_done), 210);
  ASS_RET((StrCmp(buf, s_C1) == 0), 211);

#ifdef TEST_SRV1_SHUTDOWN
  return 212;
#endif

  n_io = StrLen(s_S1) + 1;
  err_code = SOCK_Write(sock, s_S1, n_io, &n_io_done);
  ASS_RET((err_code == eSOCK_ESuccess  &&  n_io == n_io_done), 213);

  /* Receive a very big binary blob, and check its content */
  {{
    char* blob = (char*)MemNew(BIG_BLOB_SIZE);
    err_code = SOCK_ReadPersist(sock, blob, BIG_BLOB_SIZE, &n_io_done);
    ASS_RET((err_code == eSOCK_ESuccess  &&  n_io_done == BIG_BLOB_SIZE), 214);
    for (n_io = 0;  n_io < BIG_BLOB_SIZE;  n_io++)
      ASSERT( blob[n_io] == (char)n_io );
    MemFree(blob);
  }}

  return 0;
}


/* More complicated randezvous test functions
 *      "TEST__client_2(SOCK sock)"
 *      "TEST__server_2(SOCK sock)"
 */

static void s_DoubleTimeout(STimeout *to) {
  if (!to->sec  &&  !to->usec) {
    to->usec = 1;
  } else {
    to->sec   = 2 * to->sec + (2 * to->usec) / 1000000;
    to->usec = (2 * to->usec) % 1000000;
  }
}

static Int2 TEST__client_2(SOCK sock)
{ /* reserved ret.codes [120-139] */
#define W_FIELD  10
#define N_FIELD  1000
#define N_REPEAT 10
#define N_RECONNECT 3
  ESOCK_ErrCode err_code;
  Uint4         n_io, n_io_done, i;
  char          buf[W_FIELD * N_FIELD + 1];

  ErrPostEx(SEV_INFO, SOCK_ERRCODE, 110, "TC2()");

  /* fill out a buffer to send to server */
  MemSet(buf, '\0', sizeof(buf));
  for (i = 0;  i < N_FIELD;  i++) {
    sprintf(buf + i * W_FIELD, "%10lu", (unsigned long)i);
  }

  /* send the buffer to server, then get it back */
  for (i = 0;  i < N_REPEAT;  i++)
    {
      char     buf1[sizeof(buf)];
      STimeout w_to, r_to;
      Boolean  w_timeout_on = (Boolean) (i%2 != 0); /* if to start from     */
      Boolean  r_timeout_on = (Boolean) (i%3 != 0); /* zero or inf. timeout */
      char*    x_buf;

      /* set timeout */
      w_to.sec  = 0;
      w_to.usec = 0;
      err_code = SOCK_SetTimeout(sock, eSOCK_OnWrite,
                                 (w_timeout_on ? &w_to : 0), 0, 0);
      ASS_RET((err_code == eSOCK_ESuccess), 111);

#ifdef DO_RECONNECT
      /* reconnect */
      if ((i % N_RECONNECT) == 0) {
        Uint4 j = i / N_RECONNECT;
        do {
          err_code = SOCK_Reconnect(sock, 0, 0, 0);
          ErrPostEx(SEV_INFO, SOCK_ERRCODE, 117,
                    "TC2:reconnect: i=%d, err_code=%d",
                    (int)i, (int)err_code);
          ASS_RET((err_code == eSOCK_ESuccess), 117);
          ASSERT( !SOCK_Eof(sock) );

          /* give a break to let server to reset the listening socket */
          X_SLEEP(1);
        } while ( j-- );
      }
#endif

      /* send */
      x_buf = buf;
      n_io = sizeof(buf);
      do {
        X_SLEEP(1);
        err_code = SOCK_Write(sock, x_buf, n_io, &n_io_done);
        if (err_code == eSOCK_EClosed) {
          ErrPostEx(SEV_ERROR, SOCK_ERRCODE, 112,
                    "TC2:write: connection closed");
          return 112;
        }
        ErrPostEx(SEV_INFO, SOCK_ERRCODE, 113,
                  "TC2:write: i=%d, err_code=%d, n_io=%5lu, n_io_done=%5lu"
                  "\ntimeout(%d): %5lu sec, %6lu msec",
                  (int)i, (int)err_code,
                  (unsigned long)n_io, (unsigned long)n_io_done,
                  (int)w_timeout_on,
                  (unsigned long)w_to.sec, (unsigned long)w_to.usec);
        if ( !w_timeout_on ) {
          ASS_RET((err_code == eSOCK_ESuccess  &&  n_io_done == n_io), 113);
        } else {
          STimeout x_to;
          ASS_RET((err_code == eSOCK_ESuccess  ||  err_code == eSOCK_ETimeout),
                  114);
          ASS_RET((SOCK_SetTimeout(sock, eSOCK_OnWrite, SOCK_GET_TIMEOUT,
                                   0, &x_to) == eSOCK_ESuccess  &&
                   w_to.sec == x_to.sec  &&  w_to.usec == x_to.usec), 115);
        }
        n_io  -= n_io_done;
        x_buf += n_io_done;
        if (err_code == eSOCK_ETimeout)
          s_DoubleTimeout(&w_to);
        err_code = SOCK_SetTimeout(sock, eSOCK_OnWrite, &w_to, 0, 0);
        ASS_RET((err_code == eSOCK_ESuccess), 116);
        w_timeout_on = TRUE;
      } while ( n_io );

      /* get back the just sent data */
      r_to.sec  = 0;
      r_to.usec = 0;
      err_code = SOCK_SetTimeout(sock, eSOCK_OnRead,
                                 (r_timeout_on ? &r_to : 0), 0, 0);
      ASS_RET((err_code == eSOCK_ESuccess), 121);

      x_buf = buf1;
      n_io = sizeof(buf1);
      do {
        if (i%2 == 0)
          { /* peek a little piece twice and compare */
            char  xx_buf1[128], xx_buf2[128];
            Uint4 xx_io_done1, xx_io_done2;
            if (SOCK_Peek(sock, xx_buf1, sizeof(xx_buf1), &xx_io_done1)
                == eSOCK_ESuccess  &&
                SOCK_Peek(sock, xx_buf2, xx_io_done1, &xx_io_done2)
                == eSOCK_ESuccess) {
              ASSERT ( xx_io_done1 >= xx_io_done2 );
              VERIFY ( !MemCmp(xx_buf1, xx_buf2, xx_io_done2) );
            }
          }
        err_code = SOCK_Read(sock, x_buf, n_io, &n_io_done);
        if (err_code == eSOCK_EClosed) {
          ErrPostEx(SEV_ERROR, SOCK_ERRCODE, 122,
                    "TC2:read: connection closed");
          ASSERT( SOCK_Eof(sock) );
          return 122;
        }
        ErrPostEx(SEV_INFO, SOCK_ERRCODE, 123,
                  "TC2:read: i=%d, err_code=%d, n_io=%5lu, n_io_done=%5lu"
                  "\ntimeout(%d): %5lu sec, %6lu msec",
                  (int)i, (int)err_code,
                  (unsigned long)n_io, (unsigned long)n_io_done,
                  (int)r_timeout_on,
                  (unsigned long)r_to.sec, (unsigned long)r_to.usec);
        if ( !r_timeout_on ) {
          ASS_RET((err_code == eSOCK_ESuccess  &&  n_io_done > 0), 124);
        } else {
          STimeout x_to;
          ASS_RET((err_code == eSOCK_ESuccess  ||  err_code == eSOCK_ETimeout),
                  125);
          ASS_RET((SOCK_SetTimeout(sock, eSOCK_OnRead, SOCK_GET_TIMEOUT,
                                   &x_to, 0) == eSOCK_ESuccess  &&
                   r_to.sec == x_to.sec  &&  r_to.usec == x_to.usec), 126);
        }

        n_io  -= n_io_done;
        x_buf += n_io_done;
        if (err_code == eSOCK_ETimeout)
          s_DoubleTimeout(&r_to);
        err_code = SOCK_SetTimeout(sock, eSOCK_OnRead, &r_to, 0, 0);
        ASS_RET((err_code == eSOCK_ESuccess), 127);
        r_timeout_on = TRUE;
      } while ( n_io );

      ASS_RET((!MemCmp(buf, buf1, sizeof(buf))), 120); 
    }

  return 0;
}


static Int2 TEST__server_2(SOCK sock, LSOCK lsock)
{ /* reserved ret.codes [220-229] */
  ESOCK_ErrCode err_code;
  Uint4         n_io, n_io_done;
  char          buf[TEST_BUFSIZE];
  STimeout      r_to, w_to;
  Uint4         i;

  ErrPostEx(SEV_INFO, SOCK_ERRCODE, 220, "TS2()");

  r_to.sec  = 0;
  r_to.usec = 0;
  w_to = r_to;

 /* goto */
 l_reconnect: /* reconnection loopback */

  err_code = SOCK_SetTimeout(sock, eSOCK_OnRead,  &r_to, 0, 0);
  ASS_RET((err_code == eSOCK_ESuccess), 220);
  err_code = SOCK_SetTimeout(sock, eSOCK_OnWrite, &w_to, 0, 0);
  ASS_RET((err_code == eSOCK_ESuccess), 221);

  for (i = 0;  ;  i++) {
    char* x_buf;

    /* read data from socket */
    n_io = sizeof(buf);
    err_code = SOCK_Read(sock, buf, n_io, &n_io_done);
    switch ( err_code )
      {
      case eSOCK_ESuccess:
        ErrPostEx(SEV_INFO, SOCK_ERRCODE, 222,
                  "TS2:read:[%lu], err_code=%d, n_io=%5lu, n_io_done=%5lu",
                  (unsigned long)i, (int)err_code,
                  (unsigned long)n_io, (unsigned long)n_io_done);
        ASS_RET((n_io_done > 0), 222);
        break;

      case eSOCK_EClosed:
        ErrPostEx(SEV_INFO, SOCK_ERRCODE, 223,
                  "TS2:read: connection closed");
        ASSERT( SOCK_Eof(sock) );

        /* reconnect */
        if ( !lsock )
          return 0;

        ErrPostEx(SEV_INFO, SOCK_ERRCODE, 223, "TS2:reconnect");
        SOCK_Close(sock);
        err_code = LSOCK_Accept(lsock, NULL, &sock);
        ASS_RET((err_code == eSOCK_ESuccess), 229);
        ASSERT( !SOCK_Eof(sock) );
        /* !!! */ 
        goto l_reconnect;

      case eSOCK_ETimeout:
        ErrPostEx(SEV_INFO, SOCK_ERRCODE, 224,
                  "TS2:read:[%lu] timeout expired: %5lu sec, %6lu msec",
                  (unsigned long)i,
                  (unsigned long)r_to.sec, (unsigned long)r_to.usec);
        ASS_RET((n_io_done == 0), 224);
        s_DoubleTimeout(&r_to);
        err_code = SOCK_SetTimeout(sock, eSOCK_OnRead, &r_to, 0, 0);
        ASS_RET((err_code == eSOCK_ESuccess), 225);
        ASSERT( !SOCK_Eof(sock) );
        break;

      default:
        ASS_RET(0, 226);
      }

    /* write(just the same) data back to client */
    n_io  = n_io_done;
    x_buf = buf;
    while ( n_io ) {
      err_code = SOCK_Write(sock, buf, n_io, &n_io_done);
      switch ( err_code )
        {
        case eSOCK_ESuccess:
          ErrPostEx(SEV_INFO, SOCK_ERRCODE, 231,
                    "TS2:write:[%lu], err_code=%d, n_io=%5lu, n_io_done=%5lu",
                    (unsigned long)i, (int)err_code,
                    (unsigned long)n_io, (unsigned long)n_io_done);
          ASS_RET((n_io_done > 0), 231);
          break;
        case eSOCK_EClosed:
          ErrPostEx(SEV_ERROR, SOCK_ERRCODE, 232,
                    "TS2:write: connection closed");
          return 230;
        case eSOCK_ETimeout:
          ErrPostEx(SEV_INFO, SOCK_ERRCODE, 233,
                    "TS2:write:[%lu] timeout expired: %5lu sec, %6lu msec",
                    (unsigned long)i,
                    (unsigned long)w_to.sec, (unsigned long)w_to.usec);
          ASS_RET((n_io_done == 0), 233);
          s_DoubleTimeout(&w_to);
          err_code = SOCK_SetTimeout(sock, eSOCK_OnWrite, &w_to, 0, 0);
          ASS_RET((err_code == eSOCK_ESuccess), 234);
          break;
        default:
          ASS_RET(0, 235);
        }

      n_io  -= n_io_done;
      x_buf += n_io_done;
    }
  }

  return 0;
}


/* Skeletons for the socket i/o test:
 *   TEST__client(...)
 *   TEST__server(...)
 *   establish and close connection;  call test i/o functions like
 *     TEST__[client|server]_[1|2|...] (...)
 */
static Int2 TEST__client(const char*     server_host,
                         Uint2           server_port,
                         const STimeout* timeout)
{ /* reserved ret.codes [100-109] */
  SOCK          sock;
  ESOCK_ErrCode err_code;
  Int2      ret_code;

  ErrPostEx(SEV_INFO, SOCK_ERRCODE, 101,
            "TEST__client(host = \"%s\", port = %u)",
            server_host, (unsigned)server_port);

  /* Connect to server */
  err_code = SOCK_Create(server_host, server_port, timeout, &sock);
  ASS_RET((err_code == eSOCK_ESuccess), 100);

  /* Test the simplest randezvous(plain request-reply)
   * The two peer functions are:
   *      "TEST__[client|server]_1(SOCK sock)"
   */
  ret_code = TEST__client_1(sock);
  ASS_RET((ret_code == 0), 101);

  /* Test a more complex case
   * The two peer functions are:
   *      "TEST__[client|server]_2(SOCK sock)"
   */
  ret_code = TEST__client_2(sock);
  ASS_RET((ret_code == 0), 102);

  /* Close connection and exit */
  err_code = SOCK_Close(sock);
  ASS_RET((err_code == eSOCK_ESuccess  ||  err_code == eSOCK_EClosed), 109);
  return 0;
}


static Int2 TEST__server(Uint2 port)
{ /* reserved ret.codes [200-209] */
  LSOCK lsock;
  ESOCK_ErrCode err_code;

  ErrPostEx(SEV_INFO, SOCK_ERRCODE, 201,
            "TEST__server(port = %u)", (unsigned)port);

  /* Create listening socket */
  err_code = LSOCK_Create(port, 1, &lsock);
  ASS_RET((err_code == eSOCK_ESuccess), 200);

  /* Accept connections from clients and run test sessions */
  for (;;)
    {
      Int2 ret_code;

      /* Accept connection */
      SOCK sock;
      err_code = LSOCK_Accept(lsock, NULL, &sock);
      ASS_RET((err_code == eSOCK_ESuccess), 208);

      /* Test the simplest randezvous(plain request-reply)
       * The two peer functions are:
       *      "TEST__[client|server]_1(SOCK sock)"
       */
      ret_code = TEST__server_1(sock);
      ASS_RET((ret_code == 0), 201);

      /* Test a more complex case
       * The two peer functions are:
       *      "TEST__[client|server]_2(SOCK sock)"
       */
#ifdef DO_RECONNECT
      ret_code = TEST__server_2(sock, lsock);
#else
      ret_code = TEST__server_2(sock, 0);
#endif
      ASS_RET((ret_code == 0), 202);

      /* Close connection */
      err_code = SOCK_Close(sock);
      ASS_RET((err_code == eSOCK_ESuccess || err_code == eSOCK_EClosed), 209);

#ifdef TEST_SRV1_ONCE
      /* finish after the first session */
      break;
#endif
    }

  /* Close listening socket */
  err_code = LSOCK_Close(lsock);
  ASS_RET((err_code == eSOCK_ESuccess), 204);
  return 0;
}


/* Main function
 * Parse command-line options, initialize and cleanup API internals;
 * run client or server test
 */
extern Int2 Main(void)
{
#define MIN_PORT 5001
  Int4   argc = GetArgc();
  char** argv = GetArgv();

  ErrSetOpts(ERR_TEE, ERR_LOG_ON);
  ErrSetLogLevel(SEV_INFO);
  ErrSetMessageLevel(SEV_INFO);
  VERIFY ( ErrSetLog("ncbisock.log") );

  {{
    char local_host[64];
    VERIFY ( GetHostName(local_host, sizeof(local_host)) );
    ErrPostEx(SEV_INFO, SOCK_ERRCODE, 200,
              "\nRunning NCBISOCK test on host \"%s\"", local_host);
  }}

#ifdef DO_SERVER
  argc = 2;
#endif
#ifdef DO_CLIENT
  argc = 3;
#endif

  switch ( argc )
    {
    case 2:
      { /* Server */
#ifdef DO_SERVER
        short port = 5555;
#else
        short port;
        if (sscanf(argv[1], "%hd", &port) != 1  ||
            port < MIN_PORT)
          break;
#endif
        ErrPostEx(SEV_INFO, SOCK_ERRCODE, 200,
                  "Starting NCBISOCK server test...");
        VERIFY ( ErrSetLog("ncbisock.srv") );

        {{
          Int2 ret_code = TEST__server((Uint2)port);
          VERIFY ( SOCK_Destroy() == eSOCK_ESuccess );
          return ret_code;
        }}
      }

    case 3:
    case 4:
      { /* Client */
        STimeout* timeout = 0;
        STimeout  x_timeout;
#ifdef DO_CLIENT
        char* server_host = "localhost";
        short server_port = 5555;
        x_timeout.sec = x_timeout.usec = 999999;
#else
        /* host */
        char* server_host = argv[1];

        /* port */
        short server_port;
        if (sscanf(argv[2], "%hd", &server_port) != 1  ||
            server_port < MIN_PORT)
          break;

        /* timeout */
        if (argc == 4) {
          double tm_out = atof(argv[3]);
          if (tm_out < 0)
            break;
          x_timeout.sec  = (Uint4) tm_out;
          x_timeout.usec = (Uint4) ((tm_out - x_timeout.sec) * 1000000);
          timeout = &x_timeout;
        } else {
          x_timeout.sec = x_timeout.usec = 999999;
        }
#endif
        ErrPostEx(SEV_INFO, SOCK_ERRCODE, 100,
                  "Starting NCBISOCK client test...\n"
                  "%s:%d, timeout=%lu.%06lu\n",
                  server_host, (int)server_port,
                  (unsigned long)x_timeout.sec, (unsigned long)x_timeout.usec);
        VERIFY ( ErrSetLog("ncbisock.cli") );

        {{
          Int2 ret_code = TEST__client(server_host, (Uint2)server_port,
                                       timeout);
          VERIFY ( SOCK_Destroy() == eSOCK_ESuccess );
          return ret_code;
        }}
      }
    }

  /* Bad cmd-line arguments;  Usage */
  ErrPostEx(SEV_ERROR, SOCK_ERRCODE, 666,
            "Usage:\n"
            "  Client: %s <srv_host> <port> [conn_timeout]\n"
            "  Server: %s <port>\n"
            " where <port> not less than %hd,  and [conn_timeout] is double",
            argv[0], argv[0], (short)MIN_PORT);
  return 1;
}
