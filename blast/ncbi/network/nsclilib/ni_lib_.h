#ifndef NI_LIB___H
#define NI_LIB___H

/*  $Id: ni_lib_.h,v 4.9 2004/11/18 15:18:36 lavr Exp $
* ==========================================================================
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
* ==========================================================================
*
* Author:  Denis Vakatov
*
* File Description:
*   Wraparound the old NCBI network client API.
*   Now provides new mechanisms to connect clients to the NCBI network 
*   services
*
* --------------------------------------------------------------------------
* $Log: ni_lib_.h,v $
* Revision 4.9  2004/11/18 15:18:36  lavr
* Retire obsoleted g_NII_ vtables
*
* Revision 4.8  2002/04/16 21:33:50  lavr
* Change "ni_serv.c" -> "ni_service.c"
*
* Revision 4.7  2001/02/21 22:09:27  lavr
* SERVICE connector included
*
* Revision 4.6  2000/09/27 21:05:08  vakatov
* [WWW/LB-specific]  NI_WWW_SetNcbidPort() to set port of the NCBID to connect
*
* Revision 4.5  1998/09/08 17:59:06  vakatov
* Added WWW/Firewall network interface
*
* Revision 4.4  1998/05/05 22:45:38  vakatov
* Added "eNII_Debug" network interface
*
* Revision 4.3  1998/04/10 19:24:47  vakatov
* NI_SetInterface():  return the overridden(old) interface value; check
* for the validity of the new interface
*
* Revision 4.2  1998/03/30 17:50:18  vakatov
* Ingrafted to the main NCBI CVS tree
*
* ==========================================================================
*/

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* To check whether an interface is supported
 */
NLM_EXTERN Boolean NI_IsInterfaceSupported(ENIInterface ni_interface);


/* Setup the interface to be used for the dispatcher created in the
 * next call to NI_GenericInitWWW()
 * NOTE 1:  this interface will be used for this thread only and for
 *          all subsequent calls to NI_GenericInitWWW()
 * NOTE 2:  use "interface" == eNII_Default to try to retrieve the
 *          interface from the environment and config files; if it
 *          cannot be found there as well, then the application will
 *          use "interface" == NII_DEFAULT
 */
NLM_EXTERN ENIInterface NI_SetInterface(ENIInterface ni_interface);


/* Setup the client host address to be used in the next call to
 * NI_GenericGetService()
 * NOTE:  the address will be valid for this thread only and for
 *        the only one call to NI_GenericGetService()
 */
NLM_EXTERN void NI_SetAddress(const Char *address);

/* WWW/LB-specific;  set port of the NCBID to connect (for this thread only).
 * Return previously set port.
 * HINT:  call it with zero "port" to get current port, without changing it.
 */
NLM_EXTERN Uint2 NI_WWW_SetNcbidPort(Uint2 port);


/* The set of functions that are capable to work with different
 * network interfaces(according to NI_SetInterface)
 */
NLM_EXTERN NI_DispatcherPtr  NI_GenericInit
(CharPtr configFile, CharPtr configSection, Boolean showMonitor,
 CharPtr lastDispatcher, Int2 lastDispLen);
NLM_EXTERN NI_DispatcherPtr  NI_SetDispatcher
(NI_DispatcherPtr disp, CharPtr host, CharPtr svc, int timeout,
 Int4 uniqueSeqNo, ValNodePtr encryption, Boolean useOutServ);
NLM_EXTERN NI_HandPtr        NI_GenericGetService
(NI_DispatcherPtr disp, CharPtr configFile, CharPtr configSection,
 CharPtr defService, Boolean hasResource);
NLM_EXTERN Int2              NI_EndServices
(NI_DispatcherPtr disp);
NLM_EXTERN Int2              NI_ServiceDisconnect
(NI_HandPtr mhp);


/*******************************************************************
 * The stuff below is for the NCBI in-library(internal) use
 *******************************************************************/

/* Type definition of functions comprising an interface
 */

typedef NI_DispatcherPtr (*F_NIGenericInit)
     (CharPtr configFile, CharPtr configSection, Boolean showMonitor,
      CharPtr lastDispatcher, Int2 lastDispLen);
typedef NI_DispatcherPtr (*F_NISetDispatcher)
     (NI_DispatcherPtr disp, CharPtr host, CharPtr svc, int timeout,
      Int4 dispserialnum, ValNodePtr encryption, Boolean useOutServ);
typedef NI_HandPtr (*F_NIGenericGetService)
     (NI_DispatcherPtr disp, CharPtr configFile, CharPtr configSection,
      CharPtr defService, Boolean hasResource);
typedef Int2 (*F_NIServiceDisconnect)
     (NI_HandPtr mhp);
typedef Int2 (*F_NIEndServices)
     (NI_DispatcherPtr disp);


/* Set of functions describing an interface
 */
typedef struct {
  F_NIGenericInit       generic_init;
  F_NISetDispatcher     set_dispatcher;
  F_NIGenericGetService generic_get_service;
  F_NIServiceDisconnect disconnect_service;
  F_NIEndServices       end_services;
} NIInterface;


/* These are the only clues exported by the modules that implement
 * connection interfaces
 * NOTE: although these are global, however they must *NOT* be referenced
 * anywhere but in this module!
 */
extern const NIInterface *g_NII_Service;     /* "ni_service.c" */
extern const NIInterface *g_NII_Debug;       /* "ni_debug.c"   */


/* Try to extract the specified parameter from the environment or
 * configuration fife
 */
extern Uint4 NI_GetEnvParam
(const Char *conf_file, const Char *conf_section,
 const Char *env_conf_name,
 Char *buf, Uint4 bufsize, const Char *dflt);

extern Uint4 NI_GetEnvParamEx
(const Char *conf_file, const Char *conf_section,
 const Char *env_name, const Char *conf_name,
 Char *buf, Uint4 bufsize, const Char *dflt);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* NI_LIB___H */
