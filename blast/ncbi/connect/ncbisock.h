#ifndef NCBISOCK__H
#define NCBISOCK__H

/* $Id: ncbisock.h,v 6.3 2004/04/01 13:43:06 lavr Exp $
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
 * $Log: ncbisock.h,v $
 * Revision 6.3  2004/04/01 13:43:06  lavr
 * Spell "occurred", "occurrence", and "occurring"
 *
 * Revision 6.2  2000/02/25 16:45:55  vakatov
 * Redesigned to really share "ncbi_*.[ch]" etc. between the C and
 * the C++ toolkits, and even to use them in a "standalone" fashion
 *
 * Revision 6.3  2000/02/17 21:51:34  vakatov
 * REDESIGN::
 *
 * Revision 6.2  2000/02/17 19:37:42  vakatov
 * REDESIGN::
 *
 * Revision 6.1  1999/10/18 15:39:05  vakatov
 * Initial revision
 * This is actually just an interface for the back compatibility with the
 * former "ncbisock.[ch]"; the real code is in "ncbi_socket.[ch]"
 * ===========================================================================
 */

#include <ncbistd.h>


#define LSOCK              Nlm_LSOCK
#define SOCK               Nlm_SOCK

#define ESOCK_ErrCode      Nlm_ESOCK_ErrCode
#define eSOCK_ESuccess     Nlm_eSOCK_Success
#define eSOCK_ETimeout     Nlm_eSOCK_Timeout
#define eSOCK_EClosed      Nlm_eSOCK_Closed
#define eSOCK_EUnknown     Nlm_eSOCK_Unknown

#define ESOCK_Mode         Nlm_ESOCK_Mode
#define eSOCK_OnRead       Nlm_eSOCK_OnRead
#define eSOCK_OnWrite      Nlm_eSOCK_OnWrite
#define eSOCK_OnReadWrite  Nlm_eSOCK_OnReadWrite

#define SOCK_ErrCodeStr    Nlm_SOCK_ErrCodeStr

#define SOCK_Initialize    Nlm_SOCK_Initialize
#define SOCK_Destroy       Nlm_SOCK_Destroy

#define LSOCK_Create       Nlm_LSOCK_Create
#define LSOCK_Accept       Nlm_LSOCK_Accept
#define LSOCK_Close        Nlm_LSOCK_Close
#define LSOCK_GetOSHandle  Nlm_LSOCK_GetOSHandle

#define SOCK_Create        Nlm_SOCK_Create
#define SOCK_SetTimeout    Nlm_SOCK_SetTimeout
#define SOCK_Select        Nlm_SOCK_Select
#define SOCK_Read          Nlm_SOCK_Read
#define SOCK_ReadPersist   Nlm_SOCK_ReadPersist
#define SOCK_Peek          Nlm_SOCK_Peek
#define SOCK_PushBack      Nlm_SOCK_PushBack
#define SOCK_Eof           Nlm_SOCK_Eof
#define SOCK_Write         Nlm_SOCK_Write
#define SOCK_Address       Nlm_SOCK_Address
#define SOCK_Reconnect     Nlm_SOCK_Reconnect
#define SOCK_Close         Nlm_SOCK_Close
#define SOCK_GetOSHandle   Nlm_SOCK_GetOSHandle

#define GetHostName        Nlm_GetHostName
#define Uint4toInaddr      Nlm_Uint4toInaddr

/* for ErrPost */
#define SOCK_ERRCODE  777


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif


/* Handle of a socket (see "ncbi_socket.[ch]")
 */
struct LSOCK_tag;                /* listening socket:  internal storage  */
typedef struct LSOCK_tag* LSOCK; /* listening socket:  handle */

struct SOCK_tag;                 /* socket:  internal storage  */
typedef struct SOCK_tag*  SOCK;  /* socket:  handle */


/* Error code
 */
typedef enum
{
  eSOCK_ESuccess = 0, /* everything is fine, no errors occurred         */
  eSOCK_ETimeout,     /* timeout expired before the data could be i/o'd */
  eSOCK_EClosed,      /* peer has closed the connection                 */

  eSOCK_EUnknown      /* unknown(most probably -- fatal) error          */
} ESOCK_ErrCode;


/* I/O direction
 */
typedef enum {
  eSOCK_OnRead,
  eSOCK_OnWrite,
  eSOCK_OnReadWrite
} ESOCK_Mode;


/* Return (const) verbal description for the passed error code
 */
NLM_EXTERN const char* SOCK_ErrCodeStr
(ESOCK_ErrCode err_code
 );



/******************************************************************************
 *  LISTENING SOCKET
 */


/* [SERVER-side]  Create and initialize the server-side(listening) socket
 * (socket() + bind() + listen())
 */
NLM_EXTERN ESOCK_ErrCode LSOCK_Create
(Nlm_Uint2  port,     /* [in]  the port to listen at            */
 Nlm_Uint2  n_listen, /* [in]  maximal # of pending connections */
 LSOCK*     lsock     /* [out] handle of the created listening socket  */
 );


/* [SERVER-side]  Accept connection from a client
 * NOTE: the "*timeout" is for this accept() only;  to set i/o timeout,
 *       use SOCK_Timeout(); (by default -- infinite)
 */
NLM_EXTERN ESOCK_ErrCode LSOCK_Accept
(LSOCK           lsock,    /* [in]  handle of a listening socket   */
 const STimeout* timeout,  /* [in]  timeout(infinite if NULL)      */
 SOCK*           sock      /* [out] handle of the created socket */
 );


/* [SERVER-side]  Close the listening socket, destroy relevant internal data
 */
NLM_EXTERN ESOCK_ErrCode LSOCK_Close
(LSOCK lsock
 );


/* Get an OS-dependent native socket handle to use by platform-specific API.
 * FYI:  on MS-Windows it will be "SOCKET", on other platforms -- "int".
 */
NLM_EXTERN ESOCK_ErrCode LSOCK_GetOSHandle
(LSOCK     lsock,
 void*     handle_buf,  /* pointer to a memory area to put the OS handle at */
 Nlm_Uint4 handle_size  /* exact(!) size of the expected OS handle */
 );



/******************************************************************************
 *  SOCKET
 */


/* [CLIENT-side]  Connect client to another(server-side, listening) socket
 * (socket() + connect() [+ select()])
 */
NLM_EXTERN ESOCK_ErrCode SOCK_Create
(const char*     host,    /* [in]  server host */
 Nlm_Uint2       port,    /* [in]  server port */
 const STimeout* timeout, /* [in]  the connect timeout */
 SOCK*           sock     /* [out] handle of the created socket */
);


/* [CLIENT-side]  Close the socket referred by "sock" and then connect
 * it to another "host:port";  fail if it takes more than "timeout".
 * (close() + connect() [+ select()])
 *
 * HINT: if "host" is NULL then connect to the same host address as before
 *       if "port" is zero then connect to the same port # as before
 * NOTE: "new" socket inherits the i/o timeouts,
 */
NLM_EXTERN ESOCK_ErrCode SOCK_Reconnect
(SOCK            sock,    /* handle of the socket to reconnect */
 const char*     host,    /* [in] server host */
 Nlm_Uint2       port,    /* [in] server port */
 const STimeout* timeout  /* [in] the connect timeout */
 );


/* [CLIENT-side]  Close the connection, destroy relevant internal data
 * NOTE: if write timeout is specified then it is lingering until either all
 *       unsent data are sent or until the timeout expires
 * NOTE: whatever error code is returned, this function cannot be
 *       called more than once for the same socket
 */
NLM_EXTERN ESOCK_ErrCode SOCK_Close
(SOCK sock
 );


/* Block on the socket until either read/write(dep. on "mode") is
 * available or timeout expires(if "timeout" is NULL then assume it infinite)
 */
NLM_EXTERN ESOCK_ErrCode SOCK_Select
(SOCK            sock,
 ESOCK_Mode      mode,
 const STimeout* timeout
 );


/* Specify timeout for the connection i/o (see SOCK_[Read|Write|Close] funcs).
 * NOTE: set the timeout to the maximum if "new_timeout" is NULL
 * NOTE: the default timeout is the maximum possible(wait "ad infinitum")
 */
#define SOCK_GET_TIMEOUT ((const STimeout*)~0)
NLM_EXTERN ESOCK_ErrCode SOCK_SetTimeout
(SOCK            sock,
 ESOCK_Mode      mode,
 const STimeout* new_timeout, /* (dont set if equal to SOCK_GET_TIMEOUT) */
 STimeout*       r_timeout,   /* if non-NULL, return previous read */
 STimeout*       w_timeout    /* and(or) write timeout values      */
 );


/* Read up to "size" bytes from "sock" to the mem.buffer pointed by "buf".
 * In "*n_read", return the number of succesfully read bytes.
 * If there is no data available to read and the timeout(see
 * SOCK_Timeout()) is expired then return eSOCK_ETimeout.
 * NOTE: Theoretically, eSOCK_Closed may indicate an empty message
 *       rather than a real closure of the connection...
 */
NLM_EXTERN ESOCK_ErrCode SOCK_Read
(SOCK       sock,
 void*      buf,
 Nlm_Uint4  size,
 Nlm_Uint4* n_read
 );


/* Operate just like SOCK_Read() but it persistently tries to read *exactly*
 * "size" bytes, and it reads again and again -- until timeout expiration or
 * error 
 */
NLM_EXTERN ESOCK_ErrCode SOCK_ReadPersist
(SOCK       sock,
 void*      buf,
 Nlm_Uint4  size,
 Nlm_Uint4* n_read
 );


/* Operate just like SOCK_Read() but dont remove the read data from the
 * input queue.
 */
NLM_EXTERN ESOCK_ErrCode SOCK_Peek
(SOCK       sock,
 void*      buf,
 Nlm_Uint4  size,
 Nlm_Uint4* n_read
 );


/* Push the specified data back to the socket input queue(to the socket's
 * internal read buffer). These can be any data, not necessarily the data
 * previously read from the socket.
 */
NLM_EXTERN ESOCK_ErrCode SOCK_PushBack
(SOCK        sock,
 const void* buf,
 Nlm_Uint4   size
 );


/* If the last input operation (Read, ReadPersist or Peek) hit EOF.
 * NOTE:  the input operations does not return SOCK_eClosed unless there
 *        is no more data to read/peek;  thus, in the case of Peek, this is
 *        the only "non-destructive" way to check whether it already hit
 *        the EOF or we can still expect more data to come.
 */
NLM_EXTERN Nlm_Boolean SOCK_Eof
(SOCK sock
 );


/* Write "size" bytes from the mem.buffer "buf" to "sock".
 * In "*n_written", return the number of successfully written bytes.
 * If cannot write all data and the timeout(see SOCK_Timeout()) is expired
 * then return eSOCK_ETimeout.
 */
NLM_EXTERN ESOCK_ErrCode SOCK_Write
(SOCK        sock,
 const void* buf,
 Nlm_Uint4   size,
 Nlm_Uint4*  n_written
 );


/* Get host address and port of the socket peer
 * If "network_byte_order" is true then return them in the network byte order
 * NOTE:  "host" or "port" can be NULL
 */
NLM_EXTERN void SOCK_Address
(SOCK        sock,
 Nlm_Uint4*  host,
 Nlm_Uint2*  port,
 Nlm_Boolean network_byte_order
 );


/* Get an OS-dependent native socket handle to use by platform-specific API.
 * FYI:  on MS-Windows it will be "SOCKET", on other platforms -- "int".
 */
NLM_EXTERN ESOCK_ErrCode SOCK_GetOSHandle
(SOCK      sock,
 void*     handle_buf,  /* pointer to a memory area to put the OS handle at */
 Nlm_Uint4 handle_size  /* exact(!) size of the expected OS handle */
 );



/******************************************************************************
 *  SHUTDOWN
 */

/* Destroy internal data used by this module
 * NOTE: no function from this API can be used after the call to SOCK_Destroy
 */
NLM_EXTERN ESOCK_ErrCode SOCK_Destroy(void);



/******************************************************************************
 *  AUXILIARY network-specific functions (added for the portability reasons)
 */


NLM_EXTERN Nlm_Boolean GetHostName
(char*      name,
 Nlm_Uint4  namelen
 );

NLM_EXTERN Nlm_Boolean Uint4toInaddr
(Nlm_Uint4  ui4_addr,  /* NOTE: must be in the network byte-order  */
 char*      buf,       /* to be filled by smth. like "123.45.67.89\0" */
 Nlm_Uint4  buf_len
 );

/* Total KLUDGE (dont use this beast, please) */
NLM_EXTERN Nlm_Uint4 Nlm_htonl(Nlm_Uint4 value);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* NCBISOCK__H */
