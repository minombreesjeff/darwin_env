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
* File Name:    ni_defin.h
*
* Author:       Beatty, Gish
*
* Version Creation Date:        1/1/92
*
* $Revision: 6.2 $
*
* File Description:
*
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 5/12/92  Epstein     Converted tabs to spaces
* 5/06/92  Epstein     Added conditionally-compiled option for use with the
*                      "purify" tool
* 01-21-94 Schuler     Added NETP_INET_MACTCP symbol
*
* ==========================================================================
*
*
* RCS Modification History:
* $Log: ni_defin.h,v $
* Revision 6.2  2001/05/08 16:43:57  juran
* Use errno, not errno_long.
*
* Revision 6.1  1999/11/23 21:43:30  vakatov
* [WIN32] Always #define NETP_INET_WSOCK for Win32
*
* Revision 6.0  1997/08/25 18:38:29  madden
* Revision changed to 6.0
*
* Revision 5.2  1997/08/13 13:37:15  epstein
* remove stray semicolons from #defines
*
* Revision 5.1  1996/06/11 20:14:14  epstein
* use O_NONBLOCK instead of O_NDELAY per a suggestion by Sergei Shavirin
*
 * Revision 5.0  1996/05/28  14:11:55  ostell
 * Set to revision 5.0
 *
 * Revision 4.1  1995/12/18  21:58:06  epstein
 * correction for SGI build
 *
 * Revision 4.0  1995/07/26  13:56:32  ostell
 * force revision to 4.0
 *
 * Revision 1.24  1995/05/17  17:51:56  epstein
 * add RCS log revision history
 *
*/

#ifndef _NI_DEFIN_
#define _NI_DEFIN_

#define STDIN           0
#define STDOUT          1
#define STDERR          STDOUT + 1

/* if the "purify" tool is in use, it will seize the file descriptor which    */
/* we had wanted; this is O.K., but the difference must be taken into account */
/* here                                                                       */
#ifndef _PURIFY
#define STDPIPE         STDERR + 1
#else
#define STDPIPE         STDERR + 2
#endif

#define NI_Handle       MHandle                 /* for API use */
#define NI_HandPtr      MHandPtr                /* for API use */
#define NI_Request      Request                 /* for API use */
#define NI_ReqPtr       ReqPtr                  /* for API use */

#define ERRTEXT_BUFSIZ  512                     /* size of supplied error text buffer */
#define HOSTNAME_SIZ    64
#define INETADDR_SIZ    16                      /* of the form 255.255.255.255 */
#define SVC_HOST_SIZ    64                      /* size of addr or name of host */
#define SVC_NAME_SIZ    64                      /* size of name of service */
#define RES_NAME_SIZ    64                      /* size of name of resource */
#define RESTYP_NAME_SIZ 64                      /* size of type of resource */
#define USERNAME_SIZ    64                      /* kerberos principle */
#define GROUPNAME_SIZ   64                      /* kerberos instance */
#define DOMAINNAME_SIZ  64                      /* kerberos realm */
#define PASSWORD_SIZ    32                      /* must be encrypted */

#define MATCHES_ANY_TYPE  "*"                   /* string which matches any resource type */

#define NI_LAST_RESERVED_PORT   1024            /* ports above this MAY be legal */

#define NI_DEFAULT_TIMEOUT      60
#define NI_SELECT_TIMEOUT       60
#define NI_READ_TIMEOUT         NI_DEFAULT_TIMEOUT
#define NI_WRITE_TIMEOUT        NI_DEFAULT_TIMEOUT

#define PIPE_TOKEN      "\003"
#define PIPE_MSG_FMT    "%d\003%s\003"

#define READ_AIP                raip
#define WRITE_AIP               waip

#define NI_DEFAULT_DOMAIN       "ncbi.nlm.nih.gov"      /* default domain */
#define NI_DEFAULT_HOST         "dispatcher"            /* default host for dispatcher */
#define NI_DEFAULT_SERVICE      "disp_port"             /* default name of service (port) */
#define NI_DFLT_SVC_PORT        5557                    /* default port for service */

#define NI_CLIENT_PORT_LO_NAME  "ncbi_begin"            /* beginning of client port range */
#define NI_DFLT_CLILO_PORT      5601                    /* default beginning of client port range */
#define NI_CLIENT_PORT_HI_NAME  "ncbi_end"              /* end of client port range */
#define NI_DFLT_CLIHI_PORT      32767                   /* default end of client port range */


/* instrumentation is enabled by default */
#ifdef NI_NOSOCK_LOGGING
#define LOG_SOCKET(x,y)
#else
#define LOG_SOCKET(x,y) NI_LogSocket(x,y,__FILE__,__LINE__)
#endif /* NI_NOSOCK_LOGGING */

#if defined(WIN32)  &&  !defined(NETP_INET_WSOCK)
#  define NETP_INET_WSOCK
#endif

#ifdef NETP_INET_WSOCK
/* ** the following was extracted from <winsock.h> ** */
/*
 * Windows Sockets errors redefined as regular Berkeley error constants.
 * These are commented out in Windows NT to avoid conflicts with errno.h.
 * Use the WSA constants instead.
 */
#define EWOULDBLOCK             WSAEWOULDBLOCK
#define EINPROGRESS             WSAEINPROGRESS
#define EALREADY                WSAEALREADY
#define ENOTSOCK                WSAENOTSOCK
#define EDESTADDRREQ            WSAEDESTADDRREQ
#define EMSGSIZE                WSAEMSGSIZE
#define EPROTOTYPE              WSAEPROTOTYPE
#define ENOPROTOOPT             WSAENOPROTOOPT
#define EPROTONOSUPPORT         WSAEPROTONOSUPPORT
#define ESOCKTNOSUPPORT         WSAESOCKTNOSUPPORT
#define EOPNOTSUPP              WSAEOPNOTSUPP
#define EPFNOSUPPORT            WSAEPFNOSUPPORT
#define EAFNOSUPPORT            WSAEAFNOSUPPORT
#define EADDRINUSE              WSAEADDRINUSE
#define EADDRNOTAVAIL           WSAEADDRNOTAVAIL
#define ENETDOWN                WSAENETDOWN
#define ENETUNREACH             WSAENETUNREACH
#define ENETRESET               WSAENETRESET
#define ECONNABORTED            WSAECONNABORTED
#define ECONNRESET              WSAECONNRESET
#define ENOBUFS                 WSAENOBUFS
#define EISCONN                 WSAEISCONN
#define ENOTCONN                WSAENOTCONN
#define ESHUTDOWN               WSAESHUTDOWN
#define ETOOMANYREFS            WSAETOOMANYREFS
#define ETIMEDOUT               WSAETIMEDOUT
#define ECONNREFUSED            WSAECONNREFUSED
#define ELOOP                   WSAELOOP
//#define ENAMETOOLONG            WSAENAMETOOLONG
#define EHOSTDOWN               WSAEHOSTDOWN
#define EHOSTUNREACH            WSAEHOSTUNREACH
//#define ENOTEMPTY               WSAENOTEMPTY
#define EPROCLIM                WSAEPROCLIM
#define EUSERS                  WSAEUSERS
#define EDQUOT                  WSAEDQUOT
#define ESTALE                  WSAESTALE
#define EREMOTE                 WSAEREMOTE
#endif  /* NETP_INET_WSOCK */


/* MACROS */

/* BLOCKSIG, UNBLOCKSIG, and BZERO are for dispatcher and ncbid only */
#if defined(OS_UNIX_SYSV) && !defined(BSD_COMPAT)
/* for System V we cheat and use the caller's "mask" variable to store     */
/* the signal; note that these macros won't work if dealing with more than */
/* one signal                                                              */
#define NI_BLOCKSIG(_sig, _mask)                { _mask = _sig; sighold(_sig); }
#define NI_UNBLOCKSIG(_sig)                     sigrelse(_sig)
#define NI_BZERO(buf, bufsize)                  Nlm_MemFill(buf, 0, bufsize)
#else
#define NI_BLOCKSIG(_sig, _mask)                _mask = sigblock(sigmask(_sig))
#define NI_UNBLOCKSIG(_mask)                    sigsetmask(_mask)
#define NI_BZERO(buf, bufsize)                  bzero(buf, bufsize)
#endif


#ifdef NETP_INET_NEWT
#define NI_READSOCKET(rsok, rbuf, rlen)         recv(rsok, rbuf, rlen, 0)
#define NI_WRITESOCKET(wsok, wbuf, wlen)        send(wsok, wbuf, wlen, 0)
#define NI_CLOSESOCKET(sok)                     close_socket(sok)
#define NI_select(s, r, w, x, t)                NI_poll_select(s, r, w, x, t)
#define NI_SETBLOCKING(fd)                      ioctl(fd, FIONBIO, "1")
#define NI_SETNONBLOCKING(fd)                   ioctl(fd, FIONBIO, "0")
#endif /* NETP_INET_NEWT */

#ifdef NETP_INET_PCNFS
#define NI_READSOCKET(rsok, rbuf, rlen)         recv(rsok, rbuf, rlen, 0)
#define NI_WRITESOCKET(wsok, wbuf, wlen)        send(wsok, wbuf, wlen, 0)
#define NI_CLOSESOCKET(sok)                     close(sok)
#define NI_select(s, r, w, x, t)                select(s, r, w, x, t)
#define NI_SETBLOCKING(fd)                      NI_SetBlocking (fd)
#define NI_SETNONBLOCKING(fd)                   NI_SetNonBlocking (fd)
#define NI_BLOCK_WITH_FUNCTION
#endif /* NETP_INET_PCNFS */

/* Windows Sockets */
#ifdef NETP_INET_WSOCK
#define NI_READSOCKET(rsok, rbuf, rlen)         recv(rsok, rbuf, rlen, 0)
#define NI_WRITESOCKET(wsok, wbuf, wlen)        send(wsok, wbuf, wlen, 0)
#define NI_CLOSESOCKET(sok)                     closesocket(sok)
#define NI_select(s, r, w, x, t)                NI_poll_select(s, r, w, x, t)
#define NI_SETBLOCKING(fd)                      NI_SetBlocking (fd)
#define NI_SETNONBLOCKING(fd)                   NI_SetNonBlocking (fd)
#define errno                                   WSAGetLastError()
#define _INVALID_SOCKET_DEFINED_
#define NI_BLOCK_WITH_FUNCTION
#define SOCK_INDEX_ERRNO                        ((errno <= WSABASEERR || errno >= sys_nerr + WSABASEERR) ? 0 : (errno - WSABASEERR))
#endif /* NETP_INET_WINSOCK */

/* TGV's Multinet TCP/IP suite for OpenVMS */
#ifdef NETP_INET_TGV
#define NI_READSOCKET(rsok, rbuf, rlen)         recv(rsok, rbuf, rlen, 0)
#define NI_WRITESOCKET(wsok, wbuf, wlen)        send(wsok, wbuf, wlen, 0)
#define NI_CLOSESOCKET(sok)                     socket_close(sok)
#define NI_select(s, r, w, x, t)                select(s, r, w, x, t)
#define NI_SETBLOCKING(fd)                      NI_SetBlocking (fd)
#define NI_SETNONBLOCKING(fd)                   NI_SetNonBlocking (fd)
#define NI_BLOCK_WITH_FUNCTION
#define SOCK_ERRNO                              socket_errno
#endif

/* Wollongong TCP/IP suite for OpenVMS */
#ifdef NETP_INET_TWG
#define NI_READSOCKET(rsok, rbuf, rlen)         recv(rsok, rbuf, rlen, 0)
#define NI_WRITESOCKET(wsok, wbuf, wlen)        send(wsok, wbuf, wlen, 0)
#define NI_CLOSESOCKET(sok)                     netclose(sok)
#define NI_select(s, r, w, x, t)                select(s, r, w, x, t)
#define NI_SETBLOCKING(fd)                      NI_SetBlocking (fd)
#define NI_SETNONBLOCKING(fd)                   NI_SetNonBlocking (fd)
#define NI_BLOCK_WITH_FUNCTION
/* from UCX */
#define FD_SETSIZE                              32
#define fd_set                                  int
#define FD_SET(n, p)                            *(p) |= 1 << (n)
#define FD_CLR(n,p)                             *(p) &= ~(1 << (n))
#define FD_ISSET(n,p)                           ((*p) & (1 << (n)))
#define FD_ZERO(p)                              *(p) = 0
#endif

/* Wollongong PathWay API 1.0 suite for OpenVMS */
#ifdef NETP_INET_WPW
#define NI_READSOCKET(rsok, rbuf, rlen)         recv(rsok, rbuf, rlen, 0)
#define NI_WRITESOCKET(wsok, wbuf, wlen)        send(wsok, wbuf, wlen, 0)
#define NI_CLOSESOCKET(sok)                     netclose(sok)
#define NI_select(s, r, w, x, t)                select(s, r, w, x, t)
#define NI_SETBLOCKING(fd)                      NI_SetBlocking (fd)
#define NI_SETNONBLOCKING(fd)                   NI_SetNonBlocking (fd)
#define NI_BLOCK_WITH_FUNCTION
#endif

/* DEC TCP/IP for OpenVMS (UCX) */
#ifdef NETP_INET_UCX
#define NI_READSOCKET(rsok, rbuf, rlen)         recv(rsok, rbuf, rlen, 0)
#define NI_WRITESOCKET(wsok, wbuf, wlen)        send(wsok, wbuf, wlen, 0)
#define NI_CLOSESOCKET(sok)                     close(sok)
#define NI_select(s, r, w, x, t)                select(s, r, w, x, t)
#define NI_SETBLOCKING(fd)                      NI_SetBlocking (fd)
#define NI_SETNONBLOCKING(fd)                   NI_SetNonBlocking (fd)
#define NI_BLOCK_WITH_FUNCTION
#define FD_SETSIZE                              32
#define fd_set                                  int
#define FD_SET(n, p)                            *(p) |= 1 << (n)
#define FD_CLR(n,p)                             *(p) &= ~(1 << (n))
#define FD_ISSET(n,p)                           ((*p) & (1 << (n)))
#define FD_ZERO(p)                              *(p) = 0
#endif


/* Macintosh */
#if defined(OS_MAC) && !defined(NETP_defined)
#define NETP_INET_MACTCP
#endif

#ifdef NETP_INET_MACTCP
#define NI_READSOCKET(rsok, rbuf, rlen)         read(rsok, rbuf, rlen)
#define NI_WRITESOCKET(wsok, wbuf, wlen)        write(wsok, wbuf, wlen)
#define NI_CLOSESOCKET(sok)                     close(sok)
#define NI_select(s, r, w, x, t)                select(s, r, w, x, t)
#define NI_SETBLOCKING(fd)      fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) & ~O_NDELAY)       /* set blocking */
#define NI_SETNONBLOCKING(fd)   fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NDELAY)        /* set nonblocking */

/* The errno_long mechanism is used for interoperability between the           */
/* network services library and the NCSA socket library, which need not have   */
/* been compiled with the same integer size (2 byte vs. 4 byte)                */
#define SOCK_ERRNO errno
#endif /* NETP_INET_MACTCP */


#ifndef NI_READSOCKET /* for now, fall back on UNIX as default */
#define NI_READSOCKET(rsok, rbuf, rlen)         read(rsok, rbuf, rlen)
#define NI_WRITESOCKET(wsok, wbuf, wlen)        write(wsok, wbuf, wlen)
#define NI_CLOSESOCKET(sok)                     close(sok)
#define NI_select(s, r, w, x, t)                select(s, r, w, x, t)

#ifdef OS_UNIX_BSD
#define NI_SETBLOCKING(fd)      fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) & ~FNDELAY)       /* set blocking */
#define NI_SETNONBLOCKING(fd)   fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | FNDELAY)        /* set nonblocking */
#else
#define NI_SETBLOCKING(fd)      fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) & ~O_NONBLOCK)       /* set blocking */
#define NI_SETNONBLOCKING(fd)   fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK)        /* set nonblocking */
#endif /* OS_UNIX_BSD */

#endif /* NI_READSOCKET */

/* MSDOS  */

#ifdef COMP_MSC
#ifndef NETP_INET_MACTCP
#ifdef FD_SETSIZE
#undef FD_SETSIZE
#endif
#define FD_SETSIZE      32
#include <io.h>

#ifndef NETP_INET_NEWT
#ifndef NETP_INET_PCNFS
#ifndef NETP_INET_WSOCK
/* copied from Sun version of this file 6-11-91 and modified */
#define NFDBITS         32
#define FD_SET(n, p)    ((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#define FD_CLR(n, p)    ((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#define FD_ISSET(n, p)  ((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#define FD_ZERO(p)      MemFill((char *)(p), '\0', sizeof(*(p)))
#endif
#endif
#endif

#endif  /* NETP_INET_MACTCP */
#endif  /* COMP_MSC */

#ifndef SOCK_ERRNO
#define SOCK_ERRNO errno
#endif /* SOCK_ERRNO */

#ifndef SOCK_INDEX_ERRNO
#define SOCK_INDEX_ERRNO SOCK_ERRNO
#endif /* SOCK_INDEX_ERRNO */

#ifndef _INVALID_SOCKET_DEFINED_
#define INVALID_SOCKET -1
#endif /* INVALID_SOCKET */

#ifdef NETP_SOCKS
#define NI_ACCEPT(sok,addr,addrlen)         Raccept(sok,addr,addrlen)
#define NI_BIND(sok,name,namelen,bindaddr)  Rbind(sok,name,namelen,bindaddr)
#define NI_CONNECT(sok,name,namelen)        Rconnect(sok,name,namelen)
#define NI_GETSOCKNAME(sok,name,namelen)    Rgetsockname(sok,name,namelen)
#define NI_LISTEN(sok,backlog)              Rlisten(sok,backlog)
#else
#define NI_ACCEPT(sok,addr,addrlen)         accept(sok,addr,addrlen)
#define NI_BIND(sok,name,namelen,bindaddr)  bind(sok,name,namelen)
#define NI_CONNECT(sok,name,namelen)        connect(sok,name,namelen)
#define NI_GETSOCKNAME(sok,name,namelen)    getsockname(sok,name,namelen)
#define NI_LISTEN(sok,backlog)              listen(sok,backlog)
#endif


#endif  /* _NI_DEFIN_ */


