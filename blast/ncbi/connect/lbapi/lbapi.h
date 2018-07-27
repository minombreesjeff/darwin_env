#include <ncbilcl.h>

#if defined(LB_DIRECT) && !defined(OS_UNIX_SOL) && !defined(OS_UNIX_IRIX) && !defined(OS_UNIX_LINUX)
#undef LB_DIRECT
#endif

#ifndef LB_DIRECT
#define LBAPI__H
#endif

#ifndef LBAPI__H
#define LBAPI__H

/* $Id: lbapi.h,v 1.10 2001/06/07 21:09:33 beloslyu Exp $
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
* File Name:  $RCSfile: lbapi.h,v $
*
* Author:  Sergei Shavirin
*
* Initial Version Creation Date: 03/24/1997
*
* $Revision: 1.10 $
*
* File Description:
*        Definitions for the load-balancing API.
*
* $Log: lbapi.h,v $
* Revision 1.10  2001/06/07 21:09:33  beloslyu
* we really need this for linux
*
* Revision 1.9  1999/01/22 22:02:38  vakatov
* Added important NOTE for LBGetIPAddress()
*
* Revision 1.8  1998/05/08 15:26:58  vakatov
* [LB_DIRECT]  now can skip IP addresses(for the dispatcher hosts) which
* were already offered by the load-balancing daemon but failed by some reason
*
* Revision 1.7  1998/04/30 19:57:22  vakatov
* included <ncbilcl.h> to get #OS_UNIX_SOL and #OS_UNIX_IRIX
*
* Revision 1.6  1998/04/30 16:27:14  vakatov
* [IRIX] fixed a typo that(erroneously) did not allow LB_DIRECT on IRIX
*
* Revision 1.5  1998/04/07 14:40:32  vakatov
* added missing #endif
*
* Revision 1.4  1998/04/07 14:17:28  vakatov
* Skip all code(#ifdef'd) if LB_DIRECT is not defined or if compiled on
* any platform but Solaris and IRIX
*
* Revision 1.3  1998/04/03 21:16:48  vakatov
* Cleaned up the code and prepared "lbapi.[ch]" to be moved to "netcli" lib
*
* Revision 1.2  1998/04/02 17:36:06  yaschenk
* Adding dispd-ncbid shortcut when they are on the same server, adding preference to same server in stateless mode
*
* Revision 1.1  1998/03/06 18:55:01  shavirin
* Initial revision
* ==========================================================================
*/

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NUM_HOSTS       100
#define NUM_LOAD_PARAMETERS 4
#define MAX_NUM_SERVICES    16
#define MAX_SERVICE_LEN     32
#define LB_SHARED_KEY       34523
#define LB_SEMA_KEY         8766564

typedef struct LB_Message {
    unsigned int address; /* actually, char[4], but for UNIXes... it is okay */
    int load[NUM_LOAD_PARAMETERS];
    char service[MAX_NUM_SERVICES][MAX_SERVICE_LEN];
} LB_Message, *LB_MessagePtr;

extern int LBPrintTable(void);

/* Using the info from the local load-balancing daemon,
 * get IP address of the host where the specified "service" is running.
 * Return "preferred_ip" if the specified "service" is available on the
 * host with IP address = "preferred_ip".
 * Do not consider first "n_skip" hosts enlisted in the "skip_ip" array.
 * On any error, return zero.
 * NOTE:  all IP addresses here are in the network byte order
 */
extern unsigned LBGetIPAddress
(const char* service,
 unsigned    preferred_ip,
 unsigned*   skip_ip,
 size_t      n_skip
 );

#ifdef __cplusplus
}
#endif

#endif /* LBAPI__H */
