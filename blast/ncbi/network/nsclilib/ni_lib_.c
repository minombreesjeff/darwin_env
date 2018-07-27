/*  $RCSfile: ni_lib_.c,v $  $Revision: 4.13 $  $Date: 2004/11/18 15:19:25 $
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
 * $Log: ni_lib_.c,v $
 * Revision 4.13  2004/11/18 15:19:25  lavr
 * Retire obsoleted eNII_ interfaces and g_NII_ vtables
 *
 * Revision 4.12  2003/10/27 14:11:10  lavr
 * Old (Shavirin's) dispatchers disabled unconditionally
 *
 * Revision 4.11  2003/10/16 15:11:52  lavr
 * SRV_CONN_MODE made case-insensitive
 *
 * Revision 4.10  2002/04/16 21:34:20  lavr
 * Disable old dispatcher completely and unconditionally
 *
 * Revision 4.9  2001/02/21 22:09:27  lavr
 * SERVICE connector added
 *
 * Revision 4.8  1998/09/08 17:59:06  vakatov
 * Added WWW/Firewall network interface
 *
 * Revision 4.7  1998/05/05 22:45:37  vakatov
 * Added "eNII_Debug" network interface
 *
 * Revision 4.6  1998/04/15 20:01:20  vakatov
 * [OS_MAC, ALLOW_STATELESS]  Stateless mode now available on Mac
 *
 * Revision 4.5  1998/04/10 19:24:47  vakatov
 * NI_SetInterface():  return the overridden(old) interface value; check
 * for the validity of the new interface
 *
 * Revision 4.4  1998/04/03 22:54:46  vakatov
 * Support *stateless* WWW connection only if #ALLOW_STATELESS is defined
 * (thus, disable it by default)
 *
 * Revision 4.3  1998/03/31 00:27:04  kans
 * for Mac, define NI_WWW_SUPPORTED
 *
 * Revision 4.2  1998/03/30 17:50:17  vakatov
 * Ingrafted to the main NCBI CVS tree
 *
 * ==========================================================================
 */

#include <ncbinet.h>
#include <ncbithr.h>

/* Some interfaces are supported on selected platforms
 */
#if defined(OS_UNIX) || defined(OS_MSWIN) || defined(OS_MAC)
#  define NI_SERVICE_SUPPORTED
#  define NI_DEBUG_SUPPORTED
#endif /* OS_UNIX | OS_MSWIN | OS_MAC */

/* Override config-file value by the environment variable's value, if any */
#if defined(OS_UNIX) || defined(OS_MSWIN)
#  define USE_GETENV
#endif /* OS_UNIX | OS_MSWIN */


/* Hard-coded constants, environment parameter names & defaults
 */

#define DEF_CONFIG_FILE      "NCBI"
#define DEF_CONFIG_SECTION   "NET_SERV"
#define ENV_CONN_MODE        "SRV_CONN_MODE"
#define SERVICE_MODE         "SERVICE"
#define DEBUG_MODE           "DEBUG"


/* Enumerated array of the interface implementation function sets
 */

static const NIInterface **s_NII[eNII_Default] = {
  0, /* eNII_Dispatcher,  obsolete */
  0, /* eNII_WWW,         obsolete */
  0, /* eNII_WWWFirewall, obsolete */
  0, /* eNII_WWWDirect,   obsolete */

#ifdef NI_SERVICE_SUPPORTED
  &g_NII_Service,
#else
  0,
#endif

#ifdef NI_DEBUG_SUPPORTED
  &g_NII_Debug
#else
  0
#endif
};


/* Local typedefs
 */

typedef struct {
  ENIInterface interface;
  CharPtr      address;
} NIOptions;


/* Static functions
 */

static void s_NIOptionsTLS_Cleanup(TNlmTls TLS, VoidPtr old_value) 
{
  if ( old_value ) {
    NIOptions *nio = (NIOptions *)old_value;
    MemFree(nio->address);
    MemFree(nio);
  }
}

static NIOptions* s_GetNIOptions
(const Char *conf_file, const Char *conf_section)
{
  static TNlmTls s_NIOptionsTLS;

  NIOptions *nio;
  Char       conn_mode[64];

  if (NlmTlsGetValue(s_NIOptionsTLS, (VoidPtr *)&nio)
      &&  nio  &&  nio->interface != eNII_Default)
    return nio;

  if ( !nio )
    nio = (NIOptions *)MemNew(sizeof(NIOptions));

  NI_GetEnvParam(conf_file, conf_section, ENV_CONN_MODE,
                 conn_mode, sizeof(conn_mode), "");
  if (StringICmp(conn_mode, SERVICE_MODE) == 0)
    nio->interface = eNII_Service;
  else if (StringICmp(conn_mode, DEBUG_MODE) == 0)
    nio->interface = eNII_Debug;
  else
    nio->interface = NII_DEFAULT;

  if ( !NI_IsInterfaceSupported(nio->interface) )
    nio->interface = NII_DEFAULT;
  ASSERT ( NI_IsInterfaceSupported(nio->interface) );

  NlmTlsSetValue(&s_NIOptionsTLS, (VoidPtr)nio, s_NIOptionsTLS_Cleanup);
  return nio;
}


/*********************************
 * INTERNAL EXTERNALS(to be used inside the library only; not public)
 */

extern Uint4 NI_GetEnvParamEx
(const Char *conf_file, const Char *conf_section,
 const Char *env_name, const Char *conf_name,
 Char *buf, Uint4 bufsize, const Char *dflt)
{
  ASSERT ( (env_name  &&  *env_name)  ||  (conf_name  &&  *conf_name) );
  ASSERT ( buf  &&  bufsize );
  buf[0] = '\0';

#ifdef USE_GETENV
  if (env_name  &&  *env_name) {
    const Char *str = getenv(env_name);
    if (str  &&  *str) {
      Uint4 len = StrLen(str);
      if (len >= bufsize)
        return 0;
      StrCpy(buf, str);
      return len;
    }
  }
#endif

  if (conf_name  &&  *conf_name) {
    if ( !conf_file )
      conf_file = DEF_CONFIG_FILE;
    if ( !conf_section )
      conf_section = DEF_CONFIG_SECTION;

    return GetAppParam
      ((CharPtr)conf_file, (CharPtr)conf_section, (CharPtr)conf_name,
       (CharPtr)dflt, buf, (Int2)bufsize);
  }

  return 0;
}

extern Uint4 NI_GetEnvParam
(const Char *conf_file, const Char *conf_section,
 const Char *env_conf_name,
 Char *buf, Uint4 bufsize, const Char *dflt)
{
  return NI_GetEnvParamEx(conf_file, conf_section,
                          env_conf_name, env_conf_name,
                          buf, bufsize, dflt);
}


/*********************************
 * EXTERNALS
 */

NLM_EXTERN Boolean NI_IsInterfaceSupported(ENIInterface ni_interface)
{
  switch ( ni_interface ) {
  case eNII_Service:
  case eNII_Debug:
    return (Boolean)(s_NII[ni_interface] != 0);
  case eNII_Default:
    return TRUE;
  default:
    return FALSE;
  }
}


NLM_EXTERN void NI_SetAddress(const Char *address)
{
  NIOptions *nio = s_GetNIOptions(0,0);
  if ( nio->address )
    MemFree(nio->address);
  nio->address = StringSave(address);
}


NLM_EXTERN ENIInterface NI_SetInterface(ENIInterface ni_interface)
{
  NIOptions *nio = s_GetNIOptions(0,0);
  ENIInterface prev_interface = nio->interface;

  if ( NI_IsInterfaceSupported(ni_interface) )
    nio->interface = ni_interface;
  else {
    ErrPostEx(SEV_ERROR, 0, 0,
              "NI_SetInterface():  unsupported interface [#%d]",
              (int)ni_interface);
  }
  return prev_interface;
}


NLM_EXTERN NI_DispatcherPtr  NI_GenericInit
(CharPtr configFile, CharPtr configSection, Boolean showMonitor,
 CharPtr lastDispatcher, Int2 lastDispLen)
{
  return
    (*s_NII[s_GetNIOptions(configFile,configSection)->interface])->generic_init
    (configFile, configSection, showMonitor, lastDispatcher, lastDispLen);
}


NLM_EXTERN NI_DispatcherPtr  NI_SetDispatcher
(NI_DispatcherPtr disp, CharPtr host, CharPtr svc, int timeout,
 Int4 uniqueSeqNo, ValNodePtr encryption, Boolean useOutServ)
{
  ENIInterface nii = disp ? disp->interface : s_GetNIOptions(0,0)->interface;
  return (*s_NII[nii])->set_dispatcher
    (disp, host, svc, timeout, uniqueSeqNo, encryption, useOutServ);
}


NLM_EXTERN NI_HandPtr        NI_GenericGetService
(NI_DispatcherPtr disp, CharPtr configFile, CharPtr configSection,
 CharPtr defService, Boolean hasResource)
{
  return (*s_NII[disp->interface])->generic_get_service
    (disp, configFile, configSection, defService, hasResource);
}


NLM_EXTERN Int2              NI_EndServices
(NI_DispatcherPtr disp)
{
  return (*s_NII[disp->interface])->end_services
    (disp);
}

NLM_EXTERN Int2              NI_ServiceDisconnect
(NI_HandPtr mhp)
{
  return mhp ? (*s_NII[mhp->disp->interface])->disconnect_service(mhp) : 0;
}

/* EOF */
