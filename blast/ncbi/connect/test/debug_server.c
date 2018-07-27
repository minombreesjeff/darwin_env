/*  $Id: debug_server.c,v 6.9 2004/04/01 13:43:06 lavr Exp $
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
*  Listen on the specified socket; on connection, run the specified server
*  with its standard i/o attached to the socket.
*  Log all client/server traffic.
*
*  Requirements:
*    OS:         UNIX only! -- tested for Solaris, IRIX, OSF1, Linux
*    Modules:    "ncbi_socket.[ch]", "ncbi_buffer.[ch]" with -DNCBI_OS_UNIX
*    Libraries:  -lsocket -lnsl
*  Build example:
*    cc -o debug_server -g debug_server.c ../ncbi_socket.c ../ncbi_buffer.c \
*       -DNCBI_OS_UNIX -I.. -lsocket -lnsl
*    NOTE:  some platforms dont need "-lsocket" and/or "-lnsl"
*
* --------------------------------------------------------------------------
* $Log: debug_server.c,v $
* Revision 6.9  2004/04/01 13:43:06  lavr
* Spell "occurred", "occurrence", and "occurring"
*
* Revision 6.8  2002/08/07 18:45:19  lavr
* Change from deprecated to current EIO_ReadMethod enums
*
* Revision 6.7  2001/01/02 21:17:47  vakatov
* Keep in-sync with the CONNECT API:  SOCK_SetLOG --> CORE_SetLOG
*
* Revision 6.6  2000/04/07 15:49:17  vakatov
* Fixed a typo in the USAGE info
*
* Revision 6.5  2000/02/25 16:45:57  vakatov
* Redesigned to really share "ncbi_*.[ch]" etc. between the C and
* the C++ toolkits, and even to use them in a "standalone" fashion
*
* Revision 6.4  1999/10/19 19:18:15  vakatov
* Switched to the new low-level "ncbi_socket.[ch]" API (independent of
* the NCBI C toolkit).
* Portability fixes; tested for Solaris, IRIX, OSF1, Linux.
*
* Revision 6.3  1999/08/16 21:58:03  vakatov
* Dont set $HTTP_USER_AGENT if it's already set in the environment
*
* Revision 6.2  1999/08/13 22:01:07  vakatov
* Use the SOCK_GET_NATIVE_HANDLE macro to get "native" socket handle
* ==========================================================================
*/

#ifndef __EXTENSIONS__
#  define __EXTENSIONS__
#endif

#include <connect/ncbi_socket.h>
#include <connect/ncbi_util.h>

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>
#include <sys/time.h>
#include <sys/poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>
#include <assert.h>



/*****************************************************************************
 *  Static / Auxiliary
 *****************************************************************************/

/* Program name
 */
static const char* s_Progname = 0;


/* Usage info printout
 */
static void s_Usage(const char* message)
{
  if ( message )
    fprintf(stderr, "\nERROR:  %s\n", message);

  fprintf(stderr, "\
\nUSAGE:  %s <logname> <listening_port> <server_path> <arg1> <arg2> ...\
\n  logname:         [string]   log file base name(no if empty)\
\n                              *.inp, *.out, *.err, *.inf\
\n  listening_port:  [1..32767] port to listen at\
\n  server_path:     [string]   path to the server to launch\
\n  argN:            [string]   server cmd.-line arguments\n",
          s_Progname);

  fprintf(stderr, "\
\nEXAMPLE:  %s foo 5111 `which cat` -\
\n  Listen on port 5111, and on connect it will bounce back all incoming\
\n  data (that's what \"cat -\" does).\
\n\
\n  You can test it e.g. using 'telnet localhost 5111', then typing in some\
\n  data;  you then can finish 'telnet' session by entering '^]' and look\
\n  into 'foo.inf' file and into other logfiles('foo.*').\
\n\
\n  The '%s' itself can be forcibly (yet peacefully!) terminated\
\n  by SIGINT(^C).\n",
          s_Progname, s_Progname);

  fprintf(stderr, "\
\nHINT:  A regular NCBI network client can be easily configured to\
\n       connect to the specified port e.g. this way:\
\n  setenv SRV_CONN_MODE DEBUG\
\n  setenv NI_DEBUG_HOST <the server host name here, e.g. \"localhost\">\
\n  setenv NI_DEBUG_PORT 5111\n");

  exit(1);
}


/* Verify status returned by a SOCK function
 */
#define SOCK_VERIFY(status)  do { \
  if (status != eIO_Success) {\
    perror( IO_StatusStr(status) ); \
    exit(99); } \
} while(0)


/* Catching signals to enforce an early termination
 */
#ifdef __cplusplus
extern "C" {
#endif
static void s_SigChildHandler(int sig);
static void s_SigIntHandler(int sig);
#ifdef __cplusplus
}
#endif

static int s_ServerExit = 0 /* false */;
static void s_SigChildHandler(int sig)
{
  s_ServerExit = 1 /* true */;
}

static int s_DoExit = 0 /* false */;
static void s_SigIntHandler(int sig)
{
  s_DoExit = 1 /* true */;
}



/*****************************************************************************
 *  STATIC
 *****************************************************************************/


/* Server PID
 */
static pid_t s_ServerPID = 0;


/* The triggered i/o event in one of the monitored i/o streams
 */
typedef enum {
  /* socket to/from the client */
  eIOE_FromClient = 0,
  eIOE_ToClient,

  /* pipes to/from the server */
  eIOE_ToStdin,
  eIOE_FromStdout,
  eIOE_FromStderr,

  /* total # */
  eIOE_SizeOf
} EIOEvent;


/* Monitored i/o streams
 */
static struct pollfd s_IOpoll[eIOE_SizeOf];


/* Data buffers
 */
#define DATABUF_SIZE 4096
typedef struct {
  /* n_total_stored >= n_total_flush */
  size_t n_total_store;  /* total # of bytes ever stored  in   the buffer */
  size_t n_total_flush;  /* total # of bytes ever flushed from the buffer */
 
  /* 0 <= n_skip <= size <= DATABUF_SIZE */
  /* {n_skip == size}  <==>  {n_skip == 0, size == 0} */
  size_t n_skip;  /* ignore the first "n_skip" bytes (they are read already) */
  size_t size;    /* total # of bytes (including "n_skip") */
  char   buf[DATABUF_SIZE];
} SBuffer;

static SBuffer s_CS;  /* to store Client-to-Server data */
static SBuffer s_SC;  /* to store Server-to-Client data */

static int s_ClientHup = 0;  /* non-zero if cannot i/o to the Client */
static int s_StdinHup  = 0;  /* non-zero if cannot write  to Server STDIN  */
static int s_StdoutHup = 0;  /* non-zero if cannot read from Server STDOUT */
static int s_StderrHup = 0;  /* non-zero if cannot read from Server STDERR */

static size_t s_ErrTotal = 0;  /* # of bytes read from the server STDERR */


/* Status of the triggered i/o event
 */
typedef enum {
  eIOS_Success,  /* available for i/o */
  eIOS_Timeout,  /* timeout expired */
  eIOS_Error     /* error occurred */
} EIOStatus;


/* Socket to the client -- NCBI socket handle
 */
static SOCK sock = 0;


/* Log files
 */
static int   log_inp_fd = -1;  /* to log data coming to   the server STDIN */
static int   log_out_fd = -1;  /* to log data coming from the server STDOUT */
static int   log_err_fd = -1;  /* to log data coming from the server STDERR */
static FILE* log_inf_fp =  0;  /* to log all i/o events */



/* Monitor i/o events
 */
static void s_MonitorIO(void)
{
  int n_poll;

  /* check */
  assert(s_CS.n_total_store >= s_CS.n_total_flush);
  assert(s_CS.size <= sizeof(s_CS.buf));
  assert(s_CS.size > s_CS.n_skip  ||  (!s_CS.size  &&  !s_CS.n_skip)); 
  assert(s_SC.n_total_store >= s_SC.n_total_flush);
  assert(s_SC.size <= sizeof(s_SC.buf));
  assert(s_SC.size > s_SC.n_skip  ||  (!s_SC.size  &&  !s_SC.n_skip));
  assert(!s_ClientHup  ||  !s_StdinHup  ||  !s_StdoutHup  ||  !s_StderrHup);

  /* prepare for "poll()" */
  s_IOpoll[eIOE_FromClient].revents = 0;
  s_IOpoll[eIOE_ToClient].revents   = 0;
  s_IOpoll[eIOE_ToStdin].revents    = 0;
  s_IOpoll[eIOE_FromStdout].revents = 0;
  s_IOpoll[eIOE_FromStderr].revents = 0;

  if ( s_ClientHup ) {
    s_IOpoll[eIOE_FromClient].fd = -1;
    s_IOpoll[eIOE_ToClient].fd   = -1;
    s_IOpoll[eIOE_FromClient].events = 0;
    s_IOpoll[eIOE_ToClient].events   = 0;
  } else {
    s_IOpoll[eIOE_FromClient].events = s_CS.size ? 0 : POLLIN;
    s_IOpoll[eIOE_ToClient].events   = s_SC.size ? POLLOUT : 0;
  }

  if ( s_StdinHup ) {
    s_IOpoll[eIOE_ToStdin].fd     = -1;
    s_IOpoll[eIOE_ToStdin].events =  0;
  } else {
    s_IOpoll[eIOE_ToStdin].events    = s_CS.size ? POLLOUT : 0;
  }

  if ( s_StdoutHup ) {
    s_IOpoll[eIOE_FromStdout].fd     = -1;
    s_IOpoll[eIOE_FromStdout].events =  0;
  } else {
    s_IOpoll[eIOE_FromStdout].events = s_SC.size ? 0 : POLLIN;
  }

  if ( s_StderrHup ) {
    s_IOpoll[eIOE_FromStderr].fd     = -1;
    s_IOpoll[eIOE_FromStderr].events =  0;
  } else {
    s_IOpoll[eIOE_FromStderr].events = POLLIN;
  }

  /* poll */
#define TEN_SECOND 10000
  while ((n_poll = poll(s_IOpoll, eIOE_SizeOf, TEN_SECOND)) == -1  &&
         (errno == EAGAIN  ||  errno == EINTR)) {
    if ( s_DoExit )
      return;
    continue;
  }

  if (n_poll == 0) {
    fprintf(stderr, "\
Note:  no i/o events for more than 10 seconds... continue\n");
  } else if (n_poll < 0) {
    perror("s_DispatchIO:  Unhandled IO error on poll()");
    s_DoExit = 1 /* true */;
  }
}


/* Perform Client-Server and Server-Client i/o according to the i/o
 * events caught by s_MonitorIO().
 * Also, log the traffic.
 */
static void s_PerformIO(void)
{
  assert( !s_DoExit );
  assert(!s_ClientHup  ||  !s_StdinHup  ||  !s_StdoutHup  ||  !s_StderrHup);

  /* FromClient */
  if ( s_IOpoll[eIOE_FromClient].revents ) {
    if (s_IOpoll[eIOE_FromClient].revents & POLLIN) {
      EIO_Status status;
      size_t n_read;
      assert( !s_CS.size );
      if ((status =
           SOCK_Read(sock, s_CS.buf, sizeof(s_CS.buf), &n_read, eIO_ReadPlain))
          != eIO_Success)
        s_ClientHup = 1 /* true */;

      if ( log_inf_fp ) {
        if ( n_read )
          fprintf(log_inf_fp, "\
FROM_CLIENT [%lu:%lu]\n",
                  (unsigned long)s_CS.n_total_store,
                  (unsigned long)(s_CS.n_total_store + n_read - 1));
        if ( s_ClientHup ) {
          fprintf(log_inf_fp, "\
### FROM_CLIENT(READ): %s\n", IO_StatusStr(status));
        }
      }
      if (log_inp_fd != -1  &&
          write(log_inp_fd, s_CS.buf, n_read) != (ssize_t)n_read) {
        if ( log_inf_fp )
          fprintf(log_inf_fp, "\
### FROM_CLIENT(LOG): %s\n", strerror(errno));
        close(log_inp_fd);
        log_inp_fd = -1;
      }
      s_CS.size           = n_read;
      s_CS.n_total_store += n_read;
    } else if (s_IOpoll[eIOE_FromClient].revents & POLLOUT) {
      static int s_POLLOUT = 0;
      if ( !s_POLLOUT++ )
        fprintf(stderr, "Warning:    a poor implementation of poll()\n");
    } else {
      if ( log_inf_fp )
        fprintf(log_inf_fp, "\
### FROM_CLIENT(POLL@%lu): %s\n",
                (unsigned long)s_CS.n_total_store, strerror(errno));
      s_ClientHup = 1 /* true */;
    }

    if ( s_ClientHup )
      fprintf(stderr, "Attention:  CLIENT hangup!\n");
  }


  /* ToStdin */
  if ( s_IOpoll[eIOE_ToStdin].revents ) {
    if (s_IOpoll[eIOE_ToStdin].revents & POLLOUT) {
      ssize_t n_written;
      size_t  n_write = s_CS.size - s_CS.n_skip;
      assert( s_CS.size );

      while ((n_written = write(s_IOpoll[eIOE_ToStdin].fd, 
                                s_CS.buf + s_CS.n_skip, n_write)) < 0  &&
             errno == EINTR)
        continue;

      if (n_written > 0) {
        if ( log_inf_fp ) {
          fprintf(log_inf_fp, "\
TO_STDIN    [%lu:%lu]\n",
                  (unsigned long)s_CS.n_total_flush,
                  (unsigned long)(s_CS.n_total_flush + n_written - 1));
        }
        if (n_written == (ssize_t)n_write)
          s_CS.size = s_CS.n_skip = 0;
        else
          s_CS.n_skip += n_written;
        s_CS.n_total_flush += n_written;
      } else {
        if ( log_inf_fp )
          fprintf(log_inf_fp, "\
### TO_STDIN(WRITE@%lu): %s\n",
                  (unsigned long)s_CS.n_total_flush, strerror(errno));
        s_StdinHup = 1 /* true */;
      }
    } else {
      if ( log_inf_fp )
        fprintf(log_inf_fp, "\
### TO_STDIN(POLL@%lu): %s\n",
                (unsigned long)s_CS.n_total_flush, strerror(errno));
      s_StdinHup = 1 /* true */;
    }

    if ( s_StdinHup )
      fprintf(stderr, "Attention:  Server STDIN  hangup!\n");
  }


  /* FromStdout */
  if ( s_IOpoll[eIOE_FromStdout].revents ) {
    if (s_IOpoll[eIOE_FromStdout].revents & POLLIN) {
      ssize_t n_read;
      assert( !s_SC.size );
      while ((n_read = read(s_IOpoll[eIOE_FromStdout].fd,
                            s_SC.buf, sizeof(s_SC.buf))) <= 0  &&
             errno == EINTR)
        continue;

      if (n_read > 0) {
        if ( log_inf_fp ) {
          fprintf(log_inf_fp, "\
\tFROM_STDOUT [%lu:%lu]\n",
                  (unsigned long)s_SC.n_total_store,
                  (unsigned long)(s_SC.n_total_store + n_read - 1));
        }
        if (log_out_fd != -1  &&
            write(log_out_fd, s_SC.buf, n_read) != n_read) {
          if ( log_inf_fp )
            fprintf(log_inf_fp, "\
### FROM_STDOUT(LOG): %s\n", strerror(errno));
          close(log_out_fd);
          log_out_fd = -1;
        }
        s_SC.size           = n_read;
        s_SC.n_total_store += n_read;
      } else {
        if ( log_inf_fp )
          fprintf(log_inf_fp, "\
### FROM_STDOUT(READ@%lu): %s\n",
                  (unsigned long)s_SC.n_total_store, strerror(errno));
        s_StdoutHup = 1 /* true */;
      }
    } else {
      if ( log_inf_fp )
        fprintf(log_inf_fp, "\
### FROM_STDOUT(POLL@%lu): %s\n",
                (unsigned long)s_SC.n_total_store, strerror(errno));
      s_StdoutHup = 1 /* true */;
    }

    if ( s_StdoutHup )
      fprintf(stderr, "Attention:  Server STDOUT hangup!\n");
  }


  /* FromStderr */
  if ( s_IOpoll[eIOE_FromStderr].revents ) {
    char x_err_buf[DATABUF_SIZE];
    assert( !s_StderrHup );

    if (s_IOpoll[eIOE_FromStderr].revents & POLLIN) {
      ssize_t n_read;
      while ((n_read = read(s_IOpoll[eIOE_FromStderr].fd,
                            x_err_buf, sizeof(x_err_buf))) <= 0  &&
             errno == EINTR)
        continue;

      if (n_read > 0) {
        if ( log_inf_fp ) {
          fprintf(log_inf_fp, "\
\t\tFROM_STDERR [%lu:%lu]\n",
                  (unsigned long)s_ErrTotal,
                  (unsigned long)(s_ErrTotal + n_read - 1));
        }
        if (log_err_fd != -1  &&
            write(log_err_fd, x_err_buf, n_read) <= 0) {
          if ( log_inf_fp )
            fprintf(log_inf_fp, "\
### FROM_STDERR(LOG): %s\n", strerror(errno));
          close(log_err_fd);
          log_err_fd = -1;
        }
        s_ErrTotal += n_read;
      } else {
        if ( log_inf_fp )
          fprintf(log_inf_fp, "\
### FROM_STDERR(READ@%lu): %s\n",
                  (unsigned long)s_ErrTotal, strerror(errno));
        s_StderrHup = 1 /* true */;
      }
    } else {
      if ( log_inf_fp )
        fprintf(log_inf_fp, "\
### FROM_STDERR(POLL@%lu): %s\n",
                (unsigned long)s_ErrTotal, strerror(errno));
      s_StderrHup = 1 /* true */;
    }

    if ( s_StderrHup )
      fprintf(stderr, "Attention:  Server STDERR hangup!\n");
  }


  /* ToClient */
  if ( s_IOpoll[eIOE_ToClient].revents ) {
    if (s_IOpoll[eIOE_ToClient].revents & POLLOUT) {
      EIO_Status status;
      size_t n_written;
      size_t n_write = s_SC.size - s_SC.n_skip;
      assert( s_SC.size );

      if ((status = SOCK_Write(sock, s_SC.buf + s_SC.n_skip, n_write,
                               &n_written)) != eIO_Success)
        s_ClientHup = 1 /* true */;


      if ( log_inf_fp ) {
        if ( n_written )
          fprintf(log_inf_fp, "\
\tTO_CLIENT   [%lu:%lu]\n",
                  (unsigned long)s_SC.n_total_flush,
                  (unsigned long)(s_SC.n_total_flush + n_written - 1));
        if ( s_ClientHup ) {
          fprintf(log_inf_fp, "\
### TO_CLIENT(WRITE): %s\n", IO_StatusStr(status));
        }
      }

      if (n_written == n_write)
        s_SC.size = s_SC.n_skip = 0;
      else
        s_SC.n_skip += n_written;
      s_SC.n_total_flush += n_written;
    } else if (s_IOpoll[eIOE_ToClient].revents & POLLIN) {
      static int s_POLLIN = 0;
      if ( !s_POLLIN++ )
        fprintf(stderr, "Warning:    a poor implementation of poll()\n");
    } else {
      fprintf(log_inf_fp, "\
### TO_CLIENT(POLL@%lu): %s\n",
              (unsigned long)s_SC.n_total_flush, strerror(errno));
      s_ClientHup = 1 /* true */;
    }

    if ( s_ClientHup )
      fprintf(stderr, "Attention:  CLIENT hangup!\n");
  }
}


/* Flush and close
 */
static void s_Cleanup(void)
{
  /* Log files */
  if ( log_inf_fp )
    fclose(log_inf_fp);
  if (log_inp_fd != -1)
    close(log_inp_fd);
  if (log_out_fd != -1)
    close(log_out_fd);
  if (log_err_fd != -1)
    close(log_err_fd);

  /* Pipes to the server */
  if (s_IOpoll[eIOE_ToStdin].fd != -1)
    close(s_IOpoll[eIOE_ToStdin].fd);
  if (s_IOpoll[eIOE_FromStdout].fd != -1)
    close(s_IOpoll[eIOE_FromStdout].fd);
  if (s_IOpoll[eIOE_FromStderr].fd != -1)
    close(s_IOpoll[eIOE_FromStderr].fd);

  /* Socket to the client */
  if ( !s_ClientHup )
    SOCK_Close(sock);

  /* Kill the server */
  fprintf(stderr, "\n");
  if ( !s_ServerExit ) {
    fprintf(stderr, "\
----- The server (pid=%ld) is still alive;  send SIGINT and wait 5 sec\n",
            (long)s_ServerPID);
    kill(s_ServerPID, SIGINT);
    sleep(5);
  }
  if ( !s_ServerExit ) {
    fprintf(stderr, "\
----- The server (pid=%ld) is still alive;  send SIGKILL\n",
            (long)s_ServerPID);
    kill(s_ServerPID, SIGKILL);
  }

  /* Wait for the server to exit */
  fprintf(stderr, "\
----- Waiting for the server (pid=%ld) to exit... ", (long)s_ServerPID);
  if (s_ServerExit  ||  wait(0) == s_ServerPID)
    fprintf(stderr, "done\n");
  else
    perror("failed");
}


/* Printout the on-exit i/o statistics
 */
static void s_OnExitStat(void)
{
  if ( !log_inf_fp )
    return;

  if ( s_DoExit )
    fprintf(log_inf_fp, "\
\n\n!!!!!!!!!!!!!!!! INTERRUPTED BY USER  !!!!!!!!!!!!!!!!\n");

  fprintf(log_inf_fp, "\
\n\n================  ON EXIT -- I/O STATISTICS  ================\n\n");
  fprintf(log_inf_fp, "\
*** CLIENT  IO:  %s\n",    s_ClientHup  ? "hangup" : "active");
  fprintf(log_inf_fp, "\
*** SERVER APP:  %s\n\n",  s_ServerExit ? "exited" : "running");
  fprintf(log_inf_fp, "\
*** SERVER Stdin:   %s\n", s_StdinHup   ? "hangup" : "active");
  fprintf(log_inf_fp, "\
*** SERVER Stdout:  %s\n", s_StdoutHup  ? "hangup" : "active");
  fprintf(log_inf_fp, "\
*** SERVER Stderr:  %s\n", s_StderrHup  ? "hangup" : "active");

  fprintf(log_inf_fp, "\n");
  fprintf(log_inf_fp, "\
*** STDIN:   %5lu received from client (%lu sent to server)\n",
          (unsigned long)s_CS.n_total_store,
          (unsigned long)s_CS.n_total_flush);
  fprintf(log_inf_fp, "\
*** STDOUT:  %5lu received from server (%lu sent to client)\n",
          (unsigned long)s_SC.n_total_store,
          (unsigned long)s_SC.n_total_flush);
  fprintf(log_inf_fp, "\
*** STDERR:  %5lu received from server\n",
          (unsigned long)s_ErrTotal);
  fflush(log_inf_fp);
}


static void s_PumpAndLog(void)
{
  if ( log_inf_fp )
    fprintf(log_inf_fp, "\
\n\n================  RUNNING -- I/O TRAFFIC  ================\n\n");

  /* Regular i/o loop */
  while (!s_ClientHup  ||  !s_StdinHup  ||  !s_StdoutHup  ||  !s_StderrHup) {
    s_MonitorIO();
    if ( s_DoExit )
      break;

    s_PerformIO();
    if ( log_inf_fp )
      fflush(log_inf_fp);
    if ( s_DoExit )
      break;
  }

  /* Printout the on-exit i/o statistics */
  s_OnExitStat();
}



/*****************************************************************************
 *  MAIN
 *****************************************************************************/


extern int main(int argc, char* argv[], char* envp[])
{
  const char kUserAgent[]     = "HTTP_USER_AGENT";
  const char kFakeUserAgent[] = "HTTP_USER_AGENT=FakeUserAgent_DebugServer";
  int        x_has_user_agent = getenv(kUserAgent) != 0;

  /* cmd.-line args */
  const char*    logname   = (argc > 1) ? argv[1] : "";
  const char*    port_str  = (argc > 2) ? argv[2] : "";
  unsigned short port      = *port_str ? (unsigned short)atoi(port_str) : 0;
  const char*    server    = (argc > 3) ? argv[3] : "";

  /* traffic logs */
  char x_logname[PATH_MAX];  /* base log file name derived from "logname " */

  /* pipes to the server std streams */
  int inp_pipe[2];
  int out_pipe[2];
  int err_pipe[2];


  /* Errors and warnings go to STDERR
   */
  {{
    LOG lg = LOG_Create(0, 0, 0, 0);
    LOG_ToFILE(lg, stderr, 0/*false*/);
    CORE_SetLOG(lg);
  }}


  /* Store program name
   */
  s_Progname = argv[0];


  /* Compose file base name (strip spaces)
   */
  {{
    const char* str = logname;
    while (*str  &&  isspace((int)*str))
      str++;
    if ( *str ) {
      size_t len = strlen(str);
      while ( isspace((int)str[len-1]) )
        len--;
      if (len > PATH_MAX - 5)
        s_Usage("Too long log file name");
      memcpy(x_logname, str, len);
      x_logname[len] = '\0';
    } else {
      *x_logname = '\0';  /* dont log the traffic */
    }
  }}


  /* Printout the run info
   */
  {{
    /* get local host name */
    char local_host[1024];
    if (SOCK_gethostname(local_host, sizeof(local_host)) != 0)
      perror("Cannot get local host name");

    /* printout the parsed cmd.-line arguments */
    fprintf(stderr, "\
\nRUNNING \"%s\":\n   \
host:     %s\n   \
port:     %d (\"%s\")\n   \
logname:  \"%s\" (\"%s\")\n\n   \
SERVER:   '%s'\
",
            argv[0], local_host, (int)port, port_str, x_logname, logname,
            server);

    /* printout args to be passed to the server program */
    {{
      int i;
      for (i = 4;  i < argc;  i++)
        fprintf(stderr, " '%s'", argv[i]);
      fprintf(stderr, "\n\n");
    }}
  }}


  /* Create log files
   */
  fprintf(stderr, "----- Create log files... ");
  if ( *x_logname ) {
    char log_inf_name[PATH_MAX];
    char log_inp_name[PATH_MAX];
    char log_out_name[PATH_MAX];
    char log_err_name[PATH_MAX];

    strcpy(log_inf_name, x_logname);  strcat(log_inf_name, ".inf");
    strcpy(log_inp_name, x_logname);  strcat(log_inp_name, ".inp");
    strcpy(log_out_name, x_logname);  strcat(log_out_name, ".out");
    strcpy(log_err_name, x_logname);  strcat(log_err_name, ".err");

    log_inf_fp = fopen(log_inf_name, "w");
    log_inp_fd = creat(log_inp_name, 0666);
    log_out_fd = creat(log_out_name, 0666);
    log_err_fd = creat(log_err_name, 0666);

    if (log_inp_fd < 0  ||  log_out_fd < 0  ||  log_err_fd < 0  ||
        !log_inf_fp) {
      perror("Cannot open at least one of log files for writing");
      exit(22);
    }
  }
  fprintf(stderr, "done\n");


  /* Log the run info
   */
  {{
    if ( log_inf_fp ) {
      int i;

      fprintf(log_inf_fp, "LOGNAME: %s\n", x_logname);

      fprintf(log_inf_fp, "\
\n================  COMMAND LINE  ================\n\n");
      for (i = 3;  i < argc;  i++)
        fprintf(log_inf_fp, " '%s'", argv[i]);
      fprintf(log_inf_fp, "\n");
     

      fprintf(log_inf_fp, "\
\n\n================  ENVIRONMENT  ================\n\n");
      for (i = 0;  envp[i];  i++)
        fprintf(log_inf_fp, "%s\n", envp[i]);

      if ( !x_has_user_agent )
        fprintf(log_inf_fp, "%s\n", kFakeUserAgent);
    }
  }}


  /* Check cmd.-line args (roughly)
   */
  if ( !port )
    s_Usage("Bad port #");
  if (!server  ||  !*server)
    s_Usage("Empty server name");
  fprintf(stderr, "\n");


  /* Accept connection from a client (use NCBI socket library)
   */
  fprintf(stderr, "----- Accept connection from a client... ");
  {{
    LSOCK    lsock;
    STimeout timeout;
    SOCK_VERIFY( LSOCK_Create(port, 1, &lsock) );
    SOCK_VERIFY( LSOCK_Accept(lsock, 0, &sock) );
    SOCK_VERIFY( LSOCK_Close(lsock) );
    timeout.sec  = 0;
    timeout.usec = 0;
#if defined(NDEBUG)
    SOCK_VERIFY( SOCK_SetTimeout(sock, eIO_ReadWrite, &timeout) );
#else
    SOCK_VERIFY( SOCK_SetTimeout(sock, eIO_ReadWrite, 0) );
#endif
  }}
  fprintf(stderr, "done\n");


  /* Create pipes to connect to the server's standard i/o
   */
  if (pipe(inp_pipe) != 0  ||
      pipe(out_pipe) != 0  ||
      pipe(err_pipe) != 0) {
    perror("Failed to open a pipe");
    exit(2);
  }


  /* To catch when the server is exited, or C-Break
   */
  signal(SIGCHLD, s_SigChildHandler);
  signal(SIGINT,  s_SigIntHandler);


  /* Fork and launch the server, arrange server-side pipes
   */
  fprintf(stderr, "----- Launch the server... ");
  {{
    s_ServerPID = fork();
    if (s_ServerPID < 0) {
      perror("Failed to fork(%s)");
      exit(3);
    }

    /*** CHILD -- BEGIN ***/
    if (s_ServerPID == 0) {
      /* Redirect standard i/o to the pipes */
      if (close(inp_pipe[1]) != 0  ||
          dup2(inp_pipe[0], 0) != 0  ||  close(inp_pipe[0]) != 0) {
        perror("CHILD: Failed to redirect STDIN");   exit(101);
      }
      if (close(out_pipe[0]) != 0  ||
          dup2(out_pipe[1], 1) != 1  ||  close(out_pipe[1]) != 0) {
        perror("CHILD: Failed to redirect STDOUT");  exit(102);
      }
      if (close(err_pipe[0]) != 0  ||
          dup2(err_pipe[1], 2) != 2  ||
          close(err_pipe[1]) != 0) {
        perror("CHILD: Failed to redirect STDERR");  exit(103);
      }

      /* An ugly feature for the compatibility with the NCBI server lib...
       */
      if (!x_has_user_agent  &&  putenv((char*) kFakeUserAgent) != 0) {
        perror("Cannot set HTTP_USER_AGENT env. variable");
        exit(105);
      }

      /* Launch */
      execv(server, argv + 3);
      perror("CHILD: Failed to execv");
      exit(110);
    }
    /*** CHILD -- END ***/
  }}


  /*** PARENT ***/
  fprintf(stderr, "done (pid=%ld)\n", (long)s_ServerPID);


  /* Close unused pipes
   */
  if (close(inp_pipe[0]) != 0  ||
      close(out_pipe[1]) != 0  ||
      close(err_pipe[1]) != 0)
    perror("Failed to close unused pipes");


  /* Set pipes to non-blocking mode
   */
  if (fcntl(inp_pipe[1], F_SETFL,
            fcntl(inp_pipe[1], F_GETFL, 0) | O_NONBLOCK) == -1  ||
      fcntl(out_pipe[0], F_SETFL,                      
            fcntl(out_pipe[0], F_GETFL, 0) | O_NONBLOCK) == -1  ||
      fcntl(err_pipe[0], F_SETFL,                      
            fcntl(err_pipe[0], F_GETFL, 0) | O_NONBLOCK) == -1) {
    perror("Cannot set pipes to non-blocking mode");
    exit(10);
  }


  /* Prepare the i/o to be monitored by s_DispatchIO()
   */
  SOCK_VERIFY( SOCK_GetOSHandle(sock, &s_IOpoll[eIOE_FromClient].fd,
                                sizeof(s_IOpoll[eIOE_FromClient].fd)) );
  s_IOpoll[eIOE_ToClient].fd = s_IOpoll[eIOE_FromClient].fd;

  s_IOpoll[eIOE_ToStdin].fd    = inp_pipe[1];
  s_IOpoll[eIOE_FromStdout].fd = out_pipe[0];
  s_IOpoll[eIOE_FromStderr].fd = err_pipe[0];


  /* Pump the data to/from the server std i/o
   * Log the traffic
   */
  s_PumpAndLog();


  /* Cleanup
   */
  s_Cleanup();


  /* Exit
   */
  return 0;
}
