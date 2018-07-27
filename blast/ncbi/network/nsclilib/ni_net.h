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
* File Name:    ni_net.h
*
* Author:       Beatty, Gish
*
* Version Creation Date:        1/1/92
*
* $Revision: 6.3 $
*
* File Description: 
*
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 05-12-92 Epstein     Converted tabs to spaces
* 01-21-94 Schuler     Added NETP_INET_MACTCP symbol
*
* ==========================================================================
*
*
* RCS Modification History:
* $Log: ni_net.h,v $
* Revision 6.3  2002/06/17 15:11:33  ivanov
* #include <arpa/inet.h> is absent on BeOS platform
*
* Revision 6.2  2001/04/05 02:36:22  juran
* Removed obsolete includes.
*
* Revision 6.1  1999/11/23 21:43:30  vakatov
* [WIN32] Always #define NETP_INET_WSOCK for Win32
*
* Revision 6.0  1997/08/25 18:39:09  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 14:11:55  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:56:32  ostell
 * force revision to 4.0
 *
 * Revision 1.18  1995/07/05  14:23:58  kans
 * move #include <sys/select.h> to ncbilcl.r6k
 *
 * Revision 1.17  1995/05/17  17:52:48  epstein
 * add RCS log revision history
 *
*/

#ifndef _NI_NET_

#if !defined(COMP_MPW) && !defined(OS_VMS) && !defined(COMP_METRO)
#include <fcntl.h>
#endif /* skip for COMP_MPW or OS_VMS or COMP_METRO */

#if defined(OS_MAC) && !defined(NETP_defined)
#define NETP_INET_MACTCP
#endif

#ifdef NETP_INET_MACTCP
#include <netdb.h>
#include <s_types.h>
#include <s_socket.h>
#include <neti_in.h>
#include <a_inet.h>
#define _NI_NET_
#endif /* NETP_INET_MACTCP */

#ifdef NETP_INET_NEWT
#undef TRUE
#undef FALSE
#undef NULL
#include <nmpcip.h>
#define _NI_NET_
#endif /* NETP_INET_NEWT */

#ifdef NETP_INET_PCNFS
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <windows.h>
#include <in_addr.h>
#include <sys/nfs_time.h>
#include <tk_errno.h>
#define _NI_NET_
#endif /* NETP_INET_PCNFS */

/* TGV, Inc. Multinet TCP/IP suite */
#ifdef NETP_INET_TGV
#include "Multinet_root:[multinet.include]netdb.h"
#include "Multinet_root:[multinet.include.sys]types.h"
#include "Multinet_root:[multinet.include.sys]socket.h"
#include "Multinet_root:[multinet.include.sys]ioctl.h"
#include "Multinet_root:[multinet.include.netinet]in.h"
#include "Multinet_root:[multinet.include.arpa]inet.h"
/*#include "Multinet_root:[multinet.include.sys]time.h"*/
#include "Multinet_root:[multinet.include]errno.h"
#define _NI_NET_
#endif /* NETP_INET_TGV */

/* The Wollongong Group, Inc. TCP/IP suite */
#ifdef NETP_INET_TWG
#include <netdb.h>
#include <types.h>
#include <socket.h>
/*#include <ioctl.h>*/
#include <in.h>
#include <inet.h>
#include <time.h>
#include <errno.h>
#define _NI_NET_
#endif /* NETP_INET_TWG */

/* The Wollongong PathWay API 1.0 suite for OpenVMS */
#ifdef NETP_INET_WPW
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <errno.h>
#define _NI_NET_
#endif /* NETP_INET_TWG */

/* DEC TCP/IP for OpenVMS */
#ifdef NETP_INET_UCX
#include <netdb.h>
#include <types.h>
#include <socket.h>
#include <in.h>
#include <inet.h>
#include <time.h>
#include <ucx$inetdef.h>
#include <iodef.h>
#define _NI_NET_
#endif /* NETP_INET_UCX */


#if defined(WIN32)  &&  !defined(_NI_NET_)  &&  !defined(NETP_INET_WSOCK)
#  define NETP_INET_WSOCK
#endif
#ifdef NETP_INET_WSOCK
#  undef NEAR
#  undef FAR
#  undef Beep
#  undef PASCAL
#  undef CDECL
#  undef TRUE
#  undef FALSE
#  define STRICT
#  include <windows.h>
#  include <winsock.h>
#  define _NI_NET_
#endif /* NETP_INET_WSOCK */

/* default */
#ifndef _NI_NET_
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#ifndef OS_UNIX_BEOS
#include <arpa/inet.h>
#endif
#include <sys/time.h>
#define _NI_NET_
#endif /* _NI_NET_ */

   /*** included from ni_lib.c *****/

#ifdef OS_UNIX
#include <signal.h>
#endif /* OS_UNIX */
#ifdef NETP_INET_MACTCP
#include <neterrno.h> /* include missing error numbers */
#endif /* NETP_INET_MACTCP */
#ifdef OS_VMS
#include <perror.h>
#endif /* OS_VMS */

#ifdef NETP_SOCKS
#include <socks.h>
#endif

  /*** included from ni_msg.c *****/

#ifdef OS_VMS
#include <perror.h>
#endif /* OS_VMS */

#endif

