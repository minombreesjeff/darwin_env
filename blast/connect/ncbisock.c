/* $Id: ncbisock.c,v 6.5 2001/04/03 22:17:05 juran Exp $
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
 *   Plain portable TCP/IP socket API for:  UNIX, MS-Win, MacOS
 *
 *   This is just a back-compatibility interface("Nlm_*") to the real
 *   SOCK API located in "ncbi_socket.[ch]".
 *   Unlike the "real" SOCK API, this API uses:
 *    a) "Nlm_" name prefix for structures, types and functions;
 *    b) "Nlm_*" fixed size integer types like "Nlm_Uint4";
 *    c) "Nlm_Boolean" rather than a native "int" for the boolean type;
 *    d) [MSWIN] "NLM_EXTERN" rather than just "extern" to ease the compilation
 *       for MSWIN DLL.
 *
 * ---------------------------------------------------------------------------
 * $Log: ncbisock.c,v $
 * Revision 6.5  2001/04/03 22:17:05  juran
 * Backout previous change.
 *
 * Revision 6.3  2000/12/07 19:16:59  vakatov
 * Use new NCBI_SOCKET API imported from the C++ Toolkit
 *
 * Revision 6.2  2000/02/25 16:45:54  vakatov
 * Redesigned to really share "ncbi_*.[ch]" etc. between the C and
 * the C++ toolkits, and even to use them in a "standalone" fashion
 *
 * Revision 6.3  2000/02/18 23:50:41  vakatov
 * REDESIGN
 *
 * Revision 6.2  2000/02/17 21:51:15  vakatov
 * REDESIGN::
 *
 * Revision 6.1  1999/10/18 15:39:05  vakatov
 * Initial revision
 * This is actually just an interface for the back compatibility with the
 * former "ncbisock.[ch]"; the real code is in "ncbi_socket.[ch]"
 * ===========================================================================
 */

#include <ncbithr.h>
#include <ncbisock.h>
#include <ncbierr.h>


/* Undefine some "Nlm_SOCK_*" to clear the access to <ncbi_socket.h> API
 */
#undef LSOCK
#undef SOCK

#undef LSOCK_Create
#undef LSOCK_Accept
#undef LSOCK_Close
#undef LSOCK_GetOSHandle

#undef SOCK_Create
#undef SOCK_SetTimeout
#undef SOCK_Read
#undef SOCK_ReadPersist
#undef SOCK_Peek
#undef SOCK_PushBack
#undef SOCK_Eof
#undef SOCK_Write
#undef SOCK_Reconnect
#undef SOCK_Close
#undef SOCK_GetOSHandle


/* This is the only place where both <ncbibuf.h> and <ncbi_buffer.h> are
 * allowed to be #include'd in the same source module!
 */
#undef NCBISOCK__H

#include <connect/ncbi_socket.h>
#include <connect/ncbi_util.h>


/* EIO_Status <--> ESOCK_ErrCode
 */
static ESOCK_ErrCode s_ESOCK_ErrCode[eIO_Unknown + 1] = {
  eSOCK_ESuccess,
  eSOCK_ETimeout,
  eSOCK_EClosed,
  eSOCK_EUnknown,
  eSOCK_EUnknown,
  eSOCK_EUnknown
};
#define S2E(status)  s_ESOCK_ErrCode[(int) status]

static EIO_Status s_EIO_Status[eSOCK_EUnknown + 1] = {
  eIO_Success,
  eIO_Timeout,
  eIO_Closed,
  eIO_Unknown
};
#define E2S(err_code)  s_EIO_Status[(int) err_code]


/* ESOCK_Mode -> EIO_Event
 */
static EIO_Event s_EIO_Event[3] = {
  eIO_Read,
  eIO_Write,
  eIO_ReadWrite
};
#define M2E(mode) s_EIO_Event[(int) mode]


/***********************************************************************
 *  INITIALIZATION  -- to provide MT-safety and error posting
 */

#if defined(__cplusplus)
extern "C" {
  static int/*bool*/ s_MT_LOCK_Handler(void* user_data, EMT_Lock how);
  static void        s_MT_LOCK_Cleanup(void* user_data);
  static void s_LOG_Handler(void* user_data, SLOG_Handler* call_data);
}
#endif /* __cplusplus */


static int/*bool*/ s_MT_LOCK_Handler(void* user_data, EMT_Lock how)
{
  TNlmRWlock rw_lock = (TNlmRWlock) user_data;
  Int4 ret_code;
  switch ( how ) {
  case eMT_Lock:
    ret_code = NlmRWwrlock(rw_lock);
    break;
  case eMT_LockRead:
    ret_code = NlmRWrdlock(rw_lock);
    break;
  case eMT_Unlock:
    ret_code = NlmRWunlock(rw_lock);
    break;
  default:
    ASSERT(0);
    ret_code = 1/*bad*/;
  }

  return (ret_code == 0);
}

static void s_MT_LOCK_Cleanup(void* user_data)
{
  NlmRWdestroy((TNlmRWlock) user_data);
}


static void s_LOG_Handler(void* user_data, SLOG_Handler* call_data)
{
  static ErrSev s_Lev2Sev[eLOG_Fatal + 1] = {
    SEV_INFO,
    SEV_INFO,
    SEV_WARNING,
    SEV_ERROR,
    SEV_REJECT,
    SEV_FATAL
  };

  if (Nlm_ErrSetContext(call_data->module, call_data->file, call_data->line,
                        0, 0, 0, 0) == 0) {
    ErrSev sev = s_Lev2Sev[(int) call_data->level];
    Nlm_ErrPostStr(sev, SOCK_ERRCODE, 0, call_data->message);
  }
}


static int/*bool*/ s_Initialized = 0/*false*/;
static TNlmMutex   s_InitMutex;

static int/*fake*/ s_Initialize(void)
{
  NlmMutexLockEx(&s_InitMutex);
  if ( !s_Initialized ) {
    /* MT safety */
    if ( NlmThreadsAvailable() ) {
      TNlmRWlock rw_lock = NlmRWinit();
      if ( !rw_lock ) {
        ASSERT(0);
      } else {
        MT_LOCK lk =
          MT_LOCK_Create(rw_lock, s_MT_LOCK_Handler, s_MT_LOCK_Cleanup);
        ASSERT(lk);
        CORE_SetLOCK(lk);
      }
    }

    /* Error posting */
    CORE_SetLOG( LOG_Create(0, s_LOG_Handler, 0, 0) );

    /* API initialization */
    {{
      if (SOCK_InitializeAPI() == eIO_Success)
        s_Initialized = 1/*true*/;
      else
        ASSERT(0);
    }}
  }
  NlmMutexUnlock(s_InitMutex);
  return 1;
}

#define INITIALIZE (void)(s_Initialized ? 1 : s_Initialize())



/******************************************************************************
 *  SHUTDOWN
 */

NLM_EXTERN ESOCK_ErrCode SOCK_Destroy
(void)
{
  ESOCK_ErrCode err_code;
  TNlmMutex x_InitMutex;

  NlmMutexLockEx(&s_InitMutex);

  if ( s_Initialized ) {
    err_code = S2E( SOCK_ShutdownAPI() );
    s_Initialized = 0/*false*/;
  } else {
    err_code = eSOCK_ESuccess;
  }

  x_InitMutex = s_InitMutex;
  s_InitMutex = 0;
  NlmMutexUnlock(x_InitMutex);
  NlmMutexDestroy(x_InitMutex);

  return err_code;
}



/***********************************************************************
 *  MISC
 */

NLM_EXTERN const char* SOCK_ErrCodeStr
(ESOCK_ErrCode err_code)
{
  return IO_StatusStr( E2S(err_code) );
}



/***********************************************************************
 *  LSOCK
 */

NLM_EXTERN ESOCK_ErrCode Nlm_LSOCK_Create
(Nlm_Uint2  port,
 Nlm_Uint2  n_listen,
 LSOCK*     lsock)
{
  INITIALIZE;
  return S2E( LSOCK_Create(port, n_listen, lsock) );
}


NLM_EXTERN ESOCK_ErrCode Nlm_LSOCK_Accept
(LSOCK           lsock,
 const STimeout* timeout,
 SOCK*           sock)
{
  return S2E( LSOCK_Accept(lsock, timeout, sock) );
}


NLM_EXTERN ESOCK_ErrCode Nlm_LSOCK_Close
(LSOCK lsock)
{
  return S2E( LSOCK_Close(lsock) );
}


NLM_EXTERN ESOCK_ErrCode Nlm_LSOCK_GetOSHandle
(LSOCK     lsock,
 void*     handle_buf,
 Nlm_Uint4 handle_size)
{
  return S2E( LSOCK_GetOSHandle(lsock, handle_buf, (size_t) handle_size) );
}



/***********************************************************************
 *  SOCK
 */

NLM_EXTERN ESOCK_ErrCode Nlm_SOCK_Create
(const char*     host,
 Nlm_Uint2       port,
 const STimeout* timeout,
 SOCK*           sock)
{
  INITIALIZE;
  return S2E( SOCK_Create(host, port, timeout, sock) );
}


NLM_EXTERN ESOCK_ErrCode Nlm_SOCK_Reconnect
(SOCK            sock,
 const char*     host,
 Nlm_Uint2       port,
 const STimeout* timeout)
{
  return S2E( SOCK_Reconnect(sock, host, port, timeout) );
}


NLM_EXTERN ESOCK_ErrCode Nlm_SOCK_Close
(SOCK sock)
{
  return S2E( SOCK_Close(sock) );
}


NLM_EXTERN ESOCK_ErrCode Nlm_SOCK_Select
(SOCK            sock,
 Nlm_ESOCK_Mode  mode,
 const STimeout* timeout)
{
  return S2E( SOCK_Wait(sock, M2E(mode), timeout) );
}


NLM_EXTERN ESOCK_ErrCode Nlm_SOCK_SetTimeout
(SOCK            sock,
 Nlm_ESOCK_Mode  mode,
 const STimeout* new_timeout,
 STimeout*       r_timeout,
 STimeout*       w_timeout)
{
  /* retrieve R and/or W timeouts, if requested */
  static const STimeout s_Infinite = { 99999999, 999999 };
  if ( r_timeout ) {
    const STimeout* x_timeout = SOCK_GetTimeout(sock, eIO_Read);
    *r_timeout = x_timeout ? *x_timeout : s_Infinite;
  }
  if ( w_timeout ) {
    const STimeout* x_timeout = SOCK_GetTimeout(sock, eIO_Write);
    *w_timeout = x_timeout ? *x_timeout : s_Infinite;
  }

  /* special case -- do not change the timeout(s) */
  if (new_timeout == SOCK_GET_TIMEOUT)
    return Nlm_eSOCK_Success;

  /* change C(close) timeout when W timeout is changed */
  if (mode == eSOCK_OnWrite  ||  mode == eSOCK_OnReadWrite) {
    VERIFY(SOCK_SetTimeout(sock, eIO_Close, new_timeout) == eIO_Success);
  }

  /* change R and/or W timeouts */
  return S2E( SOCK_SetTimeout(sock, M2E(mode), new_timeout) );
}


NLM_EXTERN ESOCK_ErrCode Nlm_SOCK_Read
(SOCK       sock,
 void*      buf,
 Nlm_Uint4  size,
 Nlm_Uint4* n_read)
{
  size_t x_read;
  ESOCK_ErrCode err_code =
    S2E( SOCK_Read(sock, buf, (size_t) size, &x_read, eIO_Plain) );
  *n_read = (Nlm_Uint4) x_read;
  return err_code;
}


NLM_EXTERN ESOCK_ErrCode Nlm_SOCK_ReadPersist
(SOCK       sock,
 void*      buf,
 Nlm_Uint4  size,
 Nlm_Uint4* n_read)
{
  size_t x_read;
  ESOCK_ErrCode err_code =
    S2E( SOCK_Read(sock, buf, (size_t) size, &x_read, eIO_Persist) );
  *n_read = (Nlm_Uint4) x_read;
  return err_code;
}


NLM_EXTERN ESOCK_ErrCode Nlm_SOCK_Peek
(SOCK       sock,
 void*      buf,
 Nlm_Uint4  size,
 Nlm_Uint4* n_read)
{
  size_t x_read;
  ESOCK_ErrCode err_code =
    S2E( SOCK_Read(sock, buf, (size_t) size, &x_read, eIO_Peek) );
  *n_read = (Nlm_Uint4) x_read;
  return err_code;
}


NLM_EXTERN ESOCK_ErrCode Nlm_SOCK_PushBack
(SOCK        sock,
 const void* buf,
 Nlm_Uint4   size)
{
  return S2E( SOCK_PushBack(sock, buf, (size_t) size) );
}


NLM_EXTERN Nlm_Boolean Nlm_SOCK_Eof(SOCK sock)
{
    return (SOCK_Status(sock, eIO_Read) == eIO_Closed);
}


NLM_EXTERN ESOCK_ErrCode Nlm_SOCK_Write
(SOCK        sock,
 const void* buf,
 Nlm_Uint4   size,
 Nlm_Uint4*  n_written)
{
  size_t x_written;
  ESOCK_ErrCode err_code =
    S2E( SOCK_Write(sock, buf, (size_t) size, &x_written) );
  if ( n_written )
    *n_written = (Nlm_Uint4) x_written;
  return err_code;
}


NLM_EXTERN void Nlm_SOCK_Address
(SOCK         sock,
 Nlm_Uint4*   host,
 Nlm_Uint2*   port,
 Nlm_Boolean  network_byte_order)
{
  if ( host ) {
    unsigned int x_host;
    SOCK_GetAddress(sock, &x_host, 0, network_byte_order ? 1 : 0);
    *host = x_host;
  }
  if ( port ) {
    unsigned short x_port;
    SOCK_GetAddress(sock, 0, &x_port, network_byte_order ? 1 : 0);
    *port = x_port;
  }
}


NLM_EXTERN ESOCK_ErrCode Nlm_SOCK_GetOSHandle
(SOCK      sock,
 void*     handle_buf,
 Nlm_Uint4 handle_size)
{
  return S2E( SOCK_GetOSHandle(sock, handle_buf, (size_t) handle_size) );
}



/***********************************************************************
 *  AUXILIARY
 */

NLM_EXTERN Nlm_Boolean Nlm_GetHostName
(char*     name,
 Nlm_Uint4 namelen)
{
  INITIALIZE;
  return SOCK_gethostname(name, (size_t) namelen) ? FALSE : TRUE;
}


NLM_EXTERN Nlm_Boolean Nlm_Uint4toInaddr
(Nlm_Uint4 ui4_addr,
 char*     buf,
 Nlm_Uint4 buf_len)
{
  INITIALIZE;
  return SOCK_ntoa((unsigned int) ui4_addr, buf, (size_t) buf_len) ?
    FALSE : TRUE;
}


NLM_EXTERN Nlm_Uint4 Nlm_htonl(Nlm_Uint4 value)
{
  return (Nlm_Uint4) SOCK_htonl((Nlm_Uint4) value);
}
