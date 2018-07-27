/* $Id: ncbinet.h,v 6.6 2001/04/05 04:02:21 juran Exp $      
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
* File Name:    ncbinet.h
*
* Author:       Beatty, Gish
*
* Version Creation Date:        1/1/92
*
* $Revision: 6.6 $
*
* File Description: 
*
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: ncbinet.h,v $
* Revision 6.6  2001/04/05 04:02:21  juran
* Removed MacTCP-enabling preprocessor hacks.
*
* Revision 6.5  2000/07/08 20:44:05  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 6.4  1999/07/30 19:11:05  vakatov
* Use "strerror()" instead of "sys_errlist[]"
*
* Revision 6.3  1998/08/05 20:22:11  vakatov
* [OS_UNIX] Dont declare "errno" as extern if it's #define'd(e.g. on
* the MT platforms it can be #define'd to a function)
*
* Revision 6.2  1998/03/30 17:58:44  vakatov
* Included <ni_lib_.h> (some function protos were moved to there)
*
* Revision 3.1  1998/03/17 20:39:52  vakatov
* <save> [UNIX]  +compiled OK with "ni_www_.[ch]" and "ni_lib_.[ch]" and
* modified "ni_types.h" and "ncbinet.h"
*
* Revision 6.1  1997/11/18 21:14:12  epstein
* special handling for Linux/Alpha
*
* Revision 5.2  1997/07/01 19:12:41  vakatov
* [WIN32]  DLL'd "netcli.lib"
*
* Revision 5.1  1996/10/02 18:18:42  epstein
* add function NI_FqdnToIpaddr()
*
* Revision 4.1  1995/11/27  20:59:09  epstein
* add client support for direct-connection services
*
* 01/21/94 Schuler     Replaced
*/

#ifndef _NCBINET_
#define _NCBINET_

#include "ni_types.h"   /* include <ncbi.h> */
#include "ni_defin.h"
#include "ni_error.h"
#include "ni_encr.h"
#include <ni_lib_.h>

#ifdef NETP_INET_MACTCP

#include "sock_ext.h"
extern void bzero PROTO((CharPtr target, long numbytes));

#endif /* NETP_INET_MACTCP */


/* GLOBAL VARIABLES */
/* CLIENT FUNCTIONS */

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NETP_INET_MACTCP
#ifndef errno
extern int      errno;
#endif
extern long     errno_long;
#endif


NLM_EXTERN Boolean NI_FqdnToIpaddr PROTO((CharPtr fqdn, CharPtr ipbuf, Int2 ipbuflen));

NLM_EXTERN Int2             NI_InitServices PROTO((NI_DispatcherPtr disp, CharPtr user, CharPtr group, CharPtr password, NI_DispInfoPtr PNTR dip));

NLM_EXTERN Int4             NI_SetDispConfig PROTO((NI_DispInfoPtr PNTR dip, CharPtr dispatcher, Int2 dispLen));

NLM_EXTERN NICatalogPtr     NI_GetCatalog PROTO((NI_DispatcherPtr disp));

NLM_EXTERN ReqPtr           NI_SVCRequestBuild PROTO((NI_DispatcherPtr disp));

NLM_EXTERN void             NI_SVCRequestDestroy PROTO((ReqPtr reqp));

NLM_EXTERN NI_HandPtr       NI_ServiceGet PROTO((NI_DispatcherPtr disp, CharPtr svc, Uint2 svcvermin, Uint2 svcvermax, CharPtr res, CharPtr restype, Uint2 resvermin, Uint2 resvermax));

NLM_EXTERN NI_HandPtr       NI_ServiceRequest PROTO((ReqPtr req));

NLM_EXTERN int              NI_ServiceGetReadFd PROTO((NI_HandPtr mhp));

NLM_EXTERN int              NI_ServiceGetWriteFd PROTO((NI_HandPtr mhp));

NLM_EXTERN Int2             NI_RequestSetService PROTO((ReqPtr req, CharPtr name, Uint2 vermin, Uint2 vermax));

NLM_EXTERN Int2             NI_RequestAddResource PROTO((ReqPtr req, CharPtr name, CharPtr type, Uint2 vermin, Uint2 vermax));

NLM_EXTERN Int2             NI_GetPlatform PROTO((void));


/* SERVER FUNCTIONS */

NLM_EXTERN int              NI_ServerACK PROTO((void));

NLM_EXTERN int              NI_ServerNACK PROTO((CharPtr err_text));

NLM_EXTERN NI_HandPtr       NI_OpenASNIO PROTO((void));

NLM_EXTERN Int2             NI_CloseASNIO PROTO((NI_HandPtr hp));

/* TIMER MANAGEMENT */

NLM_EXTERN void             NI_ProcessTimers PROTO((void));
NLM_EXTERN time_t           NI_GetNextWakeup PROTO((void));
NLM_EXTERN NodePtr          NI_SetTimer PROTO((time_t timeout, NI_TimeoutHook hook, Pointer hookParam));
NLM_EXTERN void             NI_CancelTimer PROTO((NodePtr timerId));

/* MISC. FUNCTIONS */

NLM_EXTERN void             NI_SetActivityHook PROTO((NI_NetServHook hook));

/* for internal network services use only */
NLM_EXTERN NI_NetServHook   NI_ActivityHook PROTO((void));
NLM_EXTERN void             WriteConFile PROTO((Uint4 conid));

NLM_EXTERN void             NI_LogSocket PROTO((int sok, Boolean opening, CharPtr filename, int lineno));
NLM_EXTERN Int2             NI_SocketsOpen PROTO((void));

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#ifdef __cplusplus
}
#endif

#endif
