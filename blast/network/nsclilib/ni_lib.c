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
* File Name:    ni_lib.c
*
* Author:       Beatty, Gish, Epstein
*
* Version Creation Date:        1/1/92
*
* $Revision: 6.14 $
*
* File Description:
*   This file is a library of functions to be used by server application
*   and client software, using the NCBI "network services" paradigm.
*
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 4/27/92  Epstein     Added extensive in-line commentary, and removed all tabs.
* 5/11/92  Epstein     Removed unused function NI_SVCRequestGet(); added support
*                      for the connection ID to be written to a CONID file each
*                      time that the value of conid is updated; in practice,
*                      only dispatcher will update a CONID file.
* 6/22/92  Epstein     For UNIX signals, catch the SIGPIPE error which can
*                      occur when writing to a socket which is no longer
*                      connected.
* 7/06/92  Epstein     Changed sokselectw() to examine the SO_ERROR socket option
*                      after select()-ing a socket to which we were attempting a
*                      connection. This eliminates "false connects", i.e.,
*                      unsuccessful connection attempts which look successful
*                      because the select() call returns 1.
* 7/14/92  Epstein     Changed NI_SetDispatcher() and NI_InitServices() to use
*                      a configurable timeout parameter, and in the process
*                      also changed sokselectw() to have a timeout parameter,
* 1/21/93  Epstein     Add dispatcher-list support, and add dispatcher-list
*                      parameter to NI_InitServices().
* 2/12/93  Epstein     Use new boolean parameter to MsgMakeHandle(), indicating
*                      whether or not it should create a socket.  This was
*                      an attempted fix for a Mac problem ... it later
*                      turned out to be an incorrect problem-fix, but also
*                      does no harm.
* 2/24/93  Epstein     Fix long-standing Mac bug, by correctly destroying
*                      services handle and hence closing an open socket.
* 3/02/93  Epstein     Add functions to write dispatcher-configuration info
*                      to a config file.  This provides a standardized
*                      mechanisms which applications may use for net services
*                      configuration.  Also added platform functions, so
*                      that dispatcher/server complex can know what type
*                      of platform a client is running on, assuming that the
*                      client is telling the truth.
* 3/03/93  Epstein     Cleanup variable initialization.
* 3/08/93  Epstein     Improve error messages & cleanup to NI_InitServices,
*                      include reason in login failure message, and add
*                      client platform to service request.
* 3/09/93  Epstein     Add HaltServices() function to simplify cleanup.
* 3/22/93  Epstein     Fix typecast for getsockopt(), and, more importantly,
*                      remember to return the computed value in NI_GetPlatform.
* 3/23/93  Epstein     Support VMS/TGV, and add NETP_INET_ prefixes to 
*                      conditional-compilation symbols.
* 3/24/93  Epstein     Clear the caller's pointer in NI_SetDispConfig().
* 3/31/93  Epstein     Add dispatcher pointer as context for all network
*                      services operations; this allows an application
*                      to use more than one dispatcher at a time, at the
*                      expense of slightly greater complexity.  Also add
*                      a "Generic Init" function, which can be used by
*                      an application to obtain network-services in a
*                      simplified, standardized manner.
* 3/31/93  Epstein     Move debug and module variables to their correct home.
* 4/02/93  Epstein     Add WinSock support.
* 4/12/93  Schuler     Add MAKEWORD macro.
* 4/21/93  Schuler     Removed function prototypes for NI_AsnRead, NI_AsnWrite
* 5/07/93  Epstein     Move WSAStartup() code to a better place, add workaround
*                      for connection attempt on a non-blocking socket in PC-NFS
*                      4.0, add more platform definitions.
* 5/24/93  Epstein     Add separate error codes for TCP/IP initialization
*                      failure and inability to resolve local host name.
* 5/25/93  Epstein     Add configuration-file workaround for PC-NFS 5.0 bug,
*                      where NIS sometimes fails on the PC's own host name.
* 5/27/93  Epstein     Incorporate pragmatic "Gestalt" code for Vibrant
*                      scrolling workaround for WinSock under Windows 3.1,
*                      add add SOCK_INDEX_ERRNO macro to workaround another
*                      WinSock pecularity.
* 6/02/93  Schuler     Change "Handle" to "MonitorPtr" for Monitors.
* 6/07/93  Epstein     Added generic timer functions.
*                      Also add missing revision history, derived from
*                      RCS file.
* 6/09/93  Epstein     Added activity hook to report network activity back
*                      to an application.
* 6/14/93  Epstein     Changed "Generic" logic to cause UNIX/VMS loginname
*                      to override loginname, rather than vice versa.  Also
*                      setup DispatchConnect() logic to set client's declared
*                      IP address to 0.0.0.0, rather than causing an error,
*                      in the case where the client cannot resolve its own
*                      host name.  In this case, the dispatcher will set its
*                      own opinion of the client address based upon
*                      getpeername().
* 6/15/93  Epstein     Eliminate "Gestalt" code for Vibrant scrolling
*                      workaround for WinSock under Windows 3.1, since the
*                      solution for this problem does not require its use.
* 6/25/93  Epstein     Fix activity-hook action for service disconnection (had
*                      erroneously announced dispatcher-disconnection), and
*                      add logic to try to avoid getservbyname() by looking
*                      up dispatcher port # and (loport,hiport) in NCBI
*                      configuration file instead of in NIS.  As a last resort,
*                      look up the name in NIS if the entry in the NCBI
*                      config. file is non-numeric.  Also, change the client
*                      port lookup mechanism for Macintoshes to add a configured
*                      "delta" value to the low port number.  This results in
*                      allowing several Network Services applications to run
*                      concurrently on a Mac without port conflicts.
* 7/08/93  Epstein     Fix list traversal error in NI_ProcessTimers()
* 7/08/93  Epstein     Added a counter as a failsafe mechanism in
*                      NI_ProcessTimers(), since previous fix attempt failed.
* 7/09/93  Epstein     Changed a few #define names to avoid Alpha compilation
*                      warnings, and added reference count to dispatcher data
*                      structure.
* 8/09/93  Epstein     Improve diagnostics when a listen() call fails
* 8/23/93  Epstein     Add currentDisp variable so that the currently-attached
*                      dispatcher is used when the parameter to NI_SetDispatcher
*                      is NULL.
* 8/31/93  Epstein     Fix host vs. network order when comparing port numbers.
* 9/08/93  Epstein     Added new stackDescription variable, to be able to
*                      report to the dispatcher the identity of the vendor
*                      of the WinSock stack
* 9/09/93  Epstein     Fix use of currentDisp variable to correctly compare
*                      new dispatcher request again current dispatcher.
*11/24/93  Epstein     Added code to support standalone servers and clients
*                      which communicate with standalone servers or "service
*                      brokers", which listen on a specific port (to be
*                      augmented later).
*11/30/93  Epstein     Made standalone server code UNIX-only, to avoid
*                      possible compilation errors on other platforms.  However,
*                      it should be possible in principle to run and test a
*                      standalone server on a non-UNIX host.  Also, added
*                      limited security to standalone servers.
*12/08/93  Epstein     Fixed service connection activity hook, per discussion
*                      with Kyle Hart.
*01/19/94  Schuler     Post error (SEV_INFO) on WinSock initialization 
*                      failure showing WinSock's error code.
*01/28/94  Schuler     Replace "NETP_INET_MACTCP" with "NETP_INET_MACTCP"
*01/28/94  Schuler     Defined THIS_MODULE and THIS_FILE
*02/14/94  Epstein     Add preliminary RSAREF encryption support
*02/22/94  Epstein     Add DISP_RECONN_ACTION logic to allow users to breakout
*                      or quit if unable to contact primary dispatcher.
*02/24/94  Epstein     Make use of new NI_DupPubKey function, and insert
*                      newlines in macros to make editing easier.
*03/03/94  Epstein     Reduce memory leaks, suppress non-printing characters
*                      in winsock.dll.
*04/22/94  Epstein     Change error handling to use SEV_ERROR and SEV_WARNING
*                      (ErrPostEx).  Also do a better job of detecting
*                      inability to connect to dispatcher in DispatchConnect(),
*                      because under Solaris getsockopt() doesn't correctly
*                      detect an error.
*04/25/94  Epstein     Cosmetic change for error when NACK received from
*                      Dispatcher.
*05/04/94  Epstein     Add logic to allow a mixture of encrypted and
*                      unencrypted services, determined by ENCRYPTION=FALSE
*                      fields in the appropriate sections in the config.
*                      files.
*06/08/94  Epstein     Add SOCKS support (probably not correct yet) by
*                      asking Dispatcher to provide a SVC_PRE_RESPONSE message
*                      which contains the server's IP address.
*06/10/94  Epstein     More SOCKS refinement, plus added tracing for SOCKS
*06/15/94  Epstein     Produce working SOCKS version, by changing the protocol
*                      so that a SOCKSified client uses two service request
*                      messages; one to learn the IP address of the server
*                      to which it will be assigned, and the second "real"
*                      service request after it has bound the 'listen' port
*                      on the SOCKS daemon.
*07/01/94  Epstein     Determine at runtime whether or not to use SOCKS,
*                      methodology, based upon presence or absence of
*                      SOCKS_CONF file.
*07/07/94  Epstein     Updated commentary.
*09/22/94  Epstein     Improved standalone server code by using SO_REUSEADDR
*                      option, making it possible for identical servers to
*                      run consecutively without waiting for sockets to
*                      shutdown.
*12/02/94  Epstein     Changed NI_GenericGetService() to have environment
*                      variables override configuration file for SERVICE_NAME
*                      and RESOURCE_TYPE.  This is mostly just a convenience
*                      for internal NCBI use, allowing the use of a single
*                      config. file while using scripts to force the use
*                      of a different service.
*12/06/94  Epstein     Added connectDelay, adminInfo and motd fields.
*                      The client reports the time which it took to
*                      establish a connection in their connectDelay field.
*                      The Dispatcher provides the name of the Network
*                      administrator and a secondary message-of-the-day in
*                      the other two fields.
*12/21/94  Epstein     Added instrumentation for socket management
*01/11/95  Epstein     Change socket instrumentation to suppress some
*                      errors for WinSock 1.1.
*01/13/95  Epstein     Add new GetLAPType() function to report Mac clients'
*                      TCP/IP implementation ("Ethernet", "PPP", etc.) to
*                      the Dispatcher.
*03/20/95  Epstein     Fix InitServices/EndServices logic to correctly only
*                      establish a single Dispatcher connection for multiple
*                      services.
*03/29/95  Epstein     Reduce calls to Message() in favor of ErrPostEx().
*06/02/95  Epstein     Fix bindPort() to address host/network byte ordering;
*                      this should correct a byte-ordering problem on little-
*                      endian hosts which use SOCKS.
*06/06/95  Epstein     For UNIX, try to get the user's name from the USER
*                      environment variable before inquiring getlogin();
*                      this can save CPU time and contention on some systems,
*                      since access to utmp can be slow.
*06/12/95  Epstein     Another byte-ordering fix for SOCKS for the SOCKS
*                      proxy's port number.
*10/19/95  Epstein     Bug fix to accomodate logic correct in Nlm_StringCmp()
*
*11/15/95  Shavirin    Added new ability of clients to get direct connection
*                      to the server.
*                      Created new function: NI_DirectServiceRequest to connect
                       to the server directly and to send SVC_REQUEST
*                      Added new parameter in configuration file - DIRECT_SVC_CON
*                      This parameter updates new field in dispatcher stucture:
*                      disp->useOutServ. If this parameter set to FALSE - old style
*                      communication will be used. TRUE is set for direct
*                      connection.
*                      To handle direct connection protocol of the service request
*                      function was changed. In the case of old style everything
*                      remained the same (except reconnect switch option - to
*                      try direct connection if old style failed - currently disabled)
*                      In the case when disp->useOutServ is set client sends
*                      service request with field svcreq->want_ticket = TRUE.
*                      Dispatcher will sent back SVC_REQUEST with the prepared ticket
*                      or NACK in the case of error. Client will send SVC_REQUEST
*                      directly to the server and will wait until SVC_RESPONCE or
*                      NACK will be received from the dispatcher. After SVC_RESPONCE
*                      received communication transfers to the upper layers.
*01/17/96  Epstein     Add protection for DIRECT_SVC_CON mode; this comes into
*                      play with Mac OpenTransport (OT)
*02/02/96  Epstein     Add PROXY_SERV_OVERRIDE GetAppParam() flag for
*                      DIRECT_SVC_CON mode.  This makes it easier for a
*                      machine behind a firewall to get through the firewall,
*                      although it assumes a priori knowledge of the IP
*                      address of the server, and the port numbers on the
*                      server.
*02/21/96  Epstein     Fix brokered-services for little-endian clients
* ==========================================================================
*
*
* RCS Modification History:
* $Log: ni_lib.c,v $
* Revision 6.14  2001/01/04 22:35:02  vakatov
* NI_ServerACK(), NI_ServerNACK() -- to do nothing, always return 0.
*
* Revision 6.13  2000/10/30 18:12:01  beloslyu
* FreeBSD was added
*
* Revision 6.12  1999/07/30 19:11:08  vakatov
* Use "strerror()" instead of "sys_errlist[]"
*
* Revision 6.11  1999/06/07 18:31:43  beloslyu
* NetBSD port
*
* Revision 6.10  1998/04/10 19:24:45  vakatov
* NI_SetInterface():  return the overridden(old) interface value; check
* for the validity of the new interface
*
* Revision 6.9  1998/03/31 23:44:53  vakatov
* "interface" gets #defined in MS-Win headers; just #undef it
*
* Revision 6.8  1998/03/31 21:35:34  vakatov
* Added stub NI_IsInterfaceSupported(), NI_SetAddress() and
* NI_SetInterface() for the forward compatibility with "ni_lib_.h"
*
* Revision 6.7  1998/03/30 23:05:52  vakatov
* fixed minor typo
*
* Revision 6.6  1998/03/17 18:55:42  shavirin
* Rolling back to version 6.3
*
* Revision 6.3  1997/11/18 21:14:42  epstein
* identify Linux Alpha client
*
* Revision 6.2  1997/10/06 21:21:50  shavirin
* Fixed memory leak and uninitiolized memory read errors
*
* Revision 6.1  1997/09/11 18:08:24  epstein
*  add output of uname -a to Unix clients' self-identifying string
*
* Revision 6.0  1997/08/25 18:38:44  madden
* Revision changed to 6.0
*
* Revision 5.5  1997/07/01 19:12:49  vakatov
* [WIN32]  DLL'd "netcli.lib"
*
* Revision 5.4  1997/06/17 18:52:17  epstein
* fix numerous memory problems associated with timers
*
* Revision 5.3  1997/04/24 12:53:33  ostell
* fixed typo in call to AsnTypeStringToHex
*
 * Revision 5.2  1997/04/24  12:44:36  ostell
 * changed calls to AsnTypeStringToHex to match new arguments and returns
 *
 * Revision 5.1  1996/10/02  18:18:20  epstein
 * add function NI_FqdnToIpaddr() to simplify layering of netcnfg.c
 *
 * Revision 5.0  1996/05/28  14:11:55  ostell
 * Set to revision 5.0
 *
 * Revision 4.15  1996/05/22  15:12:54  epstein
 * adopt more reasonable semantics for brokered services: by default, only first service is brokered
 *
 * Revision 4.14  1996/04/29  15:29:19  epstein
 * add disp to NI_HandPtr so that service-handle can encapsulate greater context
 *
 * Revision 4.13  1996/02/21  15:50:07  epstein
 * Fix brokered-services for little-endian clients
 *
 * Revision 4.12  1996/02/02  14:53:41  epstein
 * add PROXY_SERV_OVERRIDE setting for DIRECT_SVC_CON mode
 *
 * Revision 4.11  1996/01/17  20:33:09  epstein
 * Add protection for DIRECT_SVC_CON mode; this comes into play with Mac OpenTransport
 *
 * Revision 4.10  1995/12/21  19:55:47  epstein
 * make socket non-blocking for brokered services
 *
 * Revision 4.9  1995/11/30  19:58:08  epstein
 * make NI_DirectServiceRequest() function static
 *
 * Revision 4.8  1995/11/29  17:50:24  epstein
 * fix byte-ordering for direct-service connection
 *
 * Revision 4.7  1995/11/28  21:39:06  epstein
 * remove unneeded sleep() call from non-UNIX platforms
 *
 * Revision 4.6  1995/11/28  20:18:41  epstein
 * fix leaky-socket problem for non-direct-connection services
 *
 * Revision 4.5  1995/11/27  20:59:17  epstein
 * add client support for direct-connection services
 *
 * Revision 4.2  95/10/19  20:29:06  epstein
 * Bug fix to accomodate logic correct in Nlm_StringCmp()
 * 
 * Revision 4.1  1995/08/01  13:48:33  epstein
 * remember to initialize someBrokered
 *
 * Revision 4.0  1995/07/26  13:56:32  ostell
 * force revision to 4.0
 *
 * Revision 1.74  1995/07/12  14:53:28  epstein
 * Another byte-ordering fix for SOCKS
 *
 * Revision 1.73  1995/06/06  10:04:52  epstein
 * use USER environment variable in lieu of getlogin()
 *
 * Revision 1.72  95/06/02  16:58:43  epstein
 * fix bindPort() using htonl() to correct byte-ordering problem on little-endian clients which use SOCKS
 * 
 * Revision 1.71  95/05/17  17:52:18  epstein
 * add RCS log revision history
 * 
*/

#define _NCBINET_LOCAL_VARS
#define THIS_MODULE g_nsclient_module
#define THIS_FILE _this_file
#define __NI_LIB__
#include "ncbinet.h"
#include "ni_lib.h"
#include "ni_msg.h"

char * g_nsclient_module = "nsclient";
static char *_this_file = __FILE__;

                                
#define ERR_KEY_MISMATCH "The public encryption key received from the dispatcher does\n\
not match what is on file.  There is a slight security risk\n\
that this key is being presented by a \"spoofer\" rather than\n\
the real dispatcher.  You may wish to contact the NCBI by\n\
other means to determine whether this new key is valid.  Do\n\
you wish to accept this as the new key and continue?"
#define ERR_KEY_NOPREVKEY "A public encryption key was just received from the dispatcher,\n\
but no key is currently on file.  There is a slight security\n\
risk that this key is being presented by a \"spoofer\" rather\n\
than the real dispatcher.  You may wish to contact the NCBI by\n\
other means to determine whether this new key is valid.  Do\n\
you wish to accept this as the new key and continue?"

#ifdef NETP_INET_NEWT

#define SIN_ADDR        sin_addr.S_un.S_addr
#define H_ADDR_TYPE     Uint4Ptr
#else
#define SIN_ADDR        sin_addr
#define H_ADDR_TYPE     struct in_addr *
#endif

#ifdef WIN16
#ifndef MAKEWORD
#define MAKEWORD(a,b)   ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) <<8)) 
#endif 
#endif

#define NULL_TIMER 0


typedef struct NI_Timer {
    time_t timeout;
    NI_TimeoutHook hook;
    Pointer hookParam;
} NI_Timer, PNTR NI_TimerPtr;


/* GLOBALS */
static FILE             *conid_fp = NULL;           /* File pointer for CONID */
static NodePtr timerHead = NULL;                    /* list of timers */
static NI_NetServHook activityHook = NULL;
static NI_DispatcherPtr currentDisp = NULL;
static CharPtr stackDescription = NULL;
static fd_set openfds;


#ifdef NETP_INET_WSOCK
static Int4 wsaStartupCount = 0;
#endif

NILoginPtr              NI_MakeMsgLogin PROTO((void));
static Int2             SetIdentity PROTO((NI_DispatcherPtr disp, CharPtr user, CharPtr group, CharPtr domain));
static void             HaltServices PROTO((NI_DispatcherPtr disp));
static NI_HandPtr       DispatchConnect PROTO((NI_DispatcherPtr disp, CharPtr host, CharPtr name, int timeout));
static Uint2            bindPort PROTO((int sok, struct sockaddr_in PNTR sokadr, Int2 loport, Int2 hiport, Uint4 remoteHost));
static Int2             CopyIdentity PROTO((NI_DispatcherPtr disp, NI_UidPtr uid));
static NI_HandPtr       NI_DirectServiceRequest PROTO((NIMsgPtr imp, NI_HandPtr sconnhp));


int                     sokselectr PROTO((int fd));
int                     sokselectw PROTO((int fd, int timeout));

int                     getAsnError PROTO((char * str));
void                    SetConFilePtr PROTO((FILE *fp));
void                    CloseConFile PROTO((void));


/* Added for the forward compatibility with "ni_lib_.h":
 *  NI_IsInterfaceSupported()
 *  NI_SetAddress()
 *  NI_SetInterface()
 */
NLM_EXTERN Boolean NI_IsInterfaceSupported(ENIInterface ni_interface) {
  return (Boolean)(ni_interface == eNII_Dispatcher);
}
NLM_EXTERN void NI_SetAddress(const Char *address) {;}
NLM_EXTERN ENIInterface NI_SetInterface(ENIInterface ni_interface) {
  return eNII_Dispatcher;
}




/*
 * Purpose:     Specify which dispatcher a client should try to connect to
 *
 * Parameters:
 *   disp           Usually NULL, the pointer to a pre-existing Dispatcher
 *                  structure
 *   host           Name of the host (Fully Qualified Domain Name) to use
 *   svc            Name of the "service" to try to use on that host
 *   dispserialnum  Serial number of dispatcher-list. Use -1 if no response
 *                  list is desired, or 0 if the serial number is not known.
 *
 *
 * Description:
 *              Set up the dispatcher name which should be used, and the
 *              name of the service on that dispatcher. If other parameters
 *              have been specified previously, free the memory associated
 *              with those names.
 *
 * Note:
 *              There are useful defaults for "svc". When in doubt, call
 *              this function with a second arguement of NULL.
 */

NLM_EXTERN NI_DispatcherPtr NI_SetDispatcher(NI_DispatcherPtr disp, CharPtr host, CharPtr svc, int timeout,
                 Int4 dispserialnum, ValNodePtr encryption, Boolean useOutServ)
{
    if (disp == NULL) {
        if (currentDisp != NULL && !currentDisp->someBrokered)
        { /* use current dispatcher if it matches what the caller wants */
            if ((svc == NULL ||
                 StringCmp(svc, currentDisp->dispServiceName) == 0 ) &&
                StringCmp(host, currentDisp->dispHostName) == 0) {
                return currentDisp;
            }
        }

        disp = (NI_DispatcherPtr) MemNew(sizeof(NI_Dispatcher));
        if (disp == NULL)
            return NULL;
        disp->useOutServ = useOutServ;       /* value from configfile */ 
        disp->reqResponse = NULL;
        disp->dispHostName = NULL;
        disp->dispServiceName = NULL;
        disp->dispSerialNo = 0;
        disp->localHostAddr[0] = '\0';
        disp->dispHP = NULL;
        disp->svcsHP = NULL;
        disp->clientPort = 0;
        disp->identity = NULL;
        disp->dispTimeout = 0;
        disp->referenceCount = 0;
        disp->someBrokered = FALSE;
        disp->brokeredDummy = FALSE;
        disp->encryptInfo = encryption;
        disp->useSocks = FALSE;
#ifdef SOCKS_CONF
        {
            FILE *fp;

            if ((fp = FileOpen(SOCKS_CONF, "r")) != NULL)
            {
                disp->useSocks = TRUE;
                FileClose(fp);
            }
        }
#endif /* SOCKS_CONF */
    }
    if (disp->dispHostName != NULL) {
        MemFree(disp->dispHostName);
        disp->dispHostName = NULL;
    }
    if (disp->dispServiceName != NULL) {
        MemFree(disp->dispServiceName);
        disp->dispServiceName = NULL;
    }
    if (host != NULL)
        disp->dispHostName = StringSave(host);
    if (svc != NULL)
        disp->dispServiceName = StringSave(svc);

    disp->dispSerialNo = dispserialnum;
    disp->dispTimeout = timeout;

    return disp;
} /* NI_SetDispatcher */



/*
 * Purpose:     Try to establish a connection to the dispatcher
 *
 * Parameters:
 *   disp           A pointer to the dispatcher structure
 *   user           User name to try on the dispatcher
 *   group          Group name to try on the dispatcher
 *   password       Password for this user name
 *   dip            A pointer to the caller's list of dispatchers; this should
 *                     be used by the caller to update its information
 *                     regarding which dispatchers to try in the future
 *                     (if dip == NULL, then no retries will be made to get
 *                      alternate dispatchers)
 *
 * Returns:
 *                 -1, if something failed (ni_errno indicates the nature of
 *                     the problem)
 *                 0, if everything was successful
 *                 1, if we are connected to the dispatcher which we requested,
 *                     but the list of current dispatchers has changed
 *                 2, if we are connected to a dispatcher, but not the one
 *                     which we requested
 *
 *
 * Description:
 *              Perform any WinSock and/or SOCKS initialization as necessary
 *              Connect to the dispatcher
 *              Set-up a socket for an incoming connection from a server
 *                  application process (non-SOCKS clients only)
 *              Send a LOGIN message to the dispatcher
 *              Wait for an ACK or NACK response from the dispatcher (or for
 *                  a timeout to occur)
 *              If the response was a NACK due to the dispatcher being a
 *                  backup dispatcher, then try the dispatcher which it
 *                  directs us to
 */

NLM_EXTERN Int2 NI_InitServices(NI_DispatcherPtr disp, CharPtr user, CharPtr group, CharPtr password, NI_DispInfoPtr PNTR dip)
{
    NIMsgPtr            mp, imp;
    NILoginPtr          loginp;
    struct sockaddr_in  svcsAddr;
    struct timeval      timeout;
    int                 ready;
    NIDispInfoPtr       dispinfo = NULL;
    Boolean             newDispToTry;
    Int2                altDispTries = 0;
    Int2                retval = 0;
    int                 status;
    fd_set              readfds;
    NI_PubKeyPtr        pubKey = NULL;
    Boolean             failed;
#ifdef NETP_INET_WSOCK
    WSADATA wsaData;
#endif /* NETP_INET_WSOCK */
#if defined(OS_MAC) && defined(NETP_INET_MACTCP)
    extern char * GetLAPType(void);
    char *lapType = GetLAPType();
#endif /* OS_MAC && NETP_INET_MACTCP */

    if (disp == NULL)
    {
        ni_errno = NIE_MISC;
        return -1;
    }

    if (disp->referenceCount > 0 && disp->dispHP != NULL)
    { /* already connected */
        disp->referenceCount++;
        return 0;
    }

#ifdef NETP_INET_WSOCK
    status = WSAStartup(MAKEWORD(1,1),&wsaData);
    if (status != 0)
        ErrPostEx(SEV_ERROR,0,0,"WinSock 1.1 initialization failure, code %d", status-WSABASEERR);
    /* Try WinSock 1.1 and 1.0 in that order of preference */
    if (status != 0 && (status = WSAStartup(MAKEWORD(1,0),&wsaData)) != 0)
    {
        ErrPostEx(SEV_ERROR,0,0,"WinSock 1.0 initialization failure, code %d", status-WSABASEERR);
        ni_errno = NIE_TCPINITFAIL;
        return -1;
    }
    TRACE("%s\n", wsaData.szDescription);
    if (stackDescription != NULL)
    {
        MemFree(stackDescription);
    }
    stackDescription = StringSave(wsaData.szDescription);
    for (status = StrLen(stackDescription) - 1; status >= 0; status--)
    {
        /* convert characters which are incompatible with VisibleString */
        if (stackDescription[status] < ' ' || stackDescription[status] > '~')
            stackDescription[status] = '#';
    }
    wsaStartupCount++;

#endif
#if defined(OS_MAC) && defined(NETP_INET_MACTCP)
    if (lapType != NULL)
    {
        stackDescription = StringSave(lapType);
    }
#endif /* OS_MAC && NETP_INET_MACTCP */
#ifdef OS_UNIX
    {
        FILE *fp;
        char buffer[128];
        int status;

        MemSet(buffer, 0, sizeof(buffer));

	if ((fp = popen("uname -a","r")) != NULL)
	{
	    FileRead (buffer, 1, sizeof (buffer), fp);
	    stackDescription = StringSave(buffer);
	    pclose(fp);
            for (status = StrLen(stackDescription) - 1; status >= 0; status--)
            {
                /* convert characters which are incompatible with VisibleString */
                if (stackDescription[status] < ' ' || stackDescription[status] > '~')
                stackDescription[status] = '#';
            }

        }
    }
#endif /* OS_UNIX */    
#ifdef NETP_SOCKS
    if (disp->useSocks)
    {
        char path[128];

        Nlm_ProgramPath(path, sizeof path);

        SOCKSinit(path);
        TRACE("Performed SOCKSinit(%s)\n", path);
    }
#endif /* NETP_SOCKS */

    if (disp->dispHostName == NULL)
        disp->dispHostName = StringSave(NI_DEFAULT_HOST);
    if (disp->dispServiceName == NULL)
        disp->dispServiceName = StringSave(NI_DEFAULT_SERVICE);

    do {
        newDispToTry = FALSE;
        disp->svcsHP = NULL;
        if ((disp->dispHP = DispatchConnect(disp, disp->dispHostName, disp->dispServiceName, disp->dispTimeout))
            == NULL) {
            NI_DestroyDispInfo(dispinfo);
            HaltServices (disp);
            ErrPostEx(SEV_WARNING,0,0, "NI_InitServices: Unable to connect to host <%s>, error <%s>", disp->dispHostName, ni_errlist[ni_errno]);
            return -1;      /* ni_errno remains set */
        }

        if ((disp->svcsHP = MsgMakeHandle(TRUE)) == NULL) {
            NI_DestroyDispInfo(dispinfo);
            HaltServices (disp);
            ErrPostEx (SEV_ERROR, CTX_NCBICORE, CORE_UNKNOWN, "NI_InitServices: Unable to allocate resources to communicate with %s", disp->dispHostName);
            return -1;
        }

        if (disp->dispTimeout > 0)
        {
            MsgSetReadTimeout(disp->svcsHP, disp->dispTimeout);
        }
       
        if (!disp->useOutServ)
        {  /* we need no listening if we are using direct connection */
            if ((disp->clientPort = bindPort(disp->svcsHP->sok, &svcsAddr, disp->loport, disp->hiport, 0)) == 0) {
                MsgDestroyHandle(disp->svcsHP);
                disp->svcsHP = NULL;
                ni_errno = NIE_NOBIND;                  /* can't bind a free application socket */
                NI_DestroyDispInfo(dispinfo);
                HaltServices (disp);
                ErrPostEx (SEV_ERROR, CTX_NCBICORE, CORE_UNKNOWN, "NI_InitServices: <%s>", ni_errlist[ni_errno]);
                return -1;
            }
            if ((status = NI_LISTEN(disp->svcsHP->sok, 5)) < 0) {
#ifdef NETP_INET_NEWT
                SOCK_ERRNO = ABS(status);
#endif
                StringCpy(ni_errtext, strerror(SOCK_INDEX_ERRNO));
                ni_errno = NIE_NOLISTEN;
                NI_DestroyDispInfo(dispinfo);
                HaltServices (disp);
                ErrPostEx (SEV_ERROR, CTX_NCBICORE, CORE_UNKNOWN, "NI_InitServices: <%s> <port %d, errno %d>", ni_errlist[ni_errno], (int) disp->clientPort, (int) SOCK_ERRNO);
                return -1;
            }
        }

        SetIdentity(disp, user, group, NI_DEFAULT_DOMAIN);

        loginp = NI_MakeMsgLogin();
        NI_DestroyUid(loginp->uid);
        loginp->uid = NI_MakeUid();
        loginp->seqno = disp->dispHP->seqno++;
        loginp->dispserialno = disp->dispSerialNo;
        loginp->connectDelay = disp->dispHP->connectDelay;
        if (disp->encryptInfo != NULL && NI_EncrAvailable())
        {
            loginp->encryptionDesired = TRUE;
            if (disp->encryptInfo->data.ptrvalue != NULL)
            {
                pubKey = (NI_PubKeyPtr) disp->encryptInfo->data.ptrvalue;
                loginp->pubKey = (NIPubKeyPtr) NI_PubKeyDup(pubKey);
            }
        }
        CopyIdentity(disp, loginp->uid);
        if (password != NULL)
            loginp->password = StringSave(password);
        mp = MsgBuild(NI_LOGIN, disp->dispHP->conid, (VoidPtr) loginp);

        if (MsgWrite(disp->dispHP, mp, FALSE) < 0) {
            if (getAsnError(ni_errtext) == ECONNRESET)
                ni_errno = NIE_MAXCONNS;
            else
                ni_errno = NIE_MSGWRITE;
            MsgDestroyHandle(disp->svcsHP);
            disp->svcsHP = NULL;
            NI_DestroyDispInfo(dispinfo);
            HaltServices (disp);
            ErrPostEx (SEV_ERROR, CTX_NCBICORE, CORE_UNKNOWN, "NI_InitServices: <%s>", ni_errlist[ni_errno]);
            return -1;
        }

        /* blocks until ACK or ERROR from dispatcher or TIMEOUT */

        timeout.tv_sec = (Uint4) NI_TIMEOUT_SECS;
        timeout.tv_usec = 0;
        FD_ZERO(&readfds);
        FD_SET(disp->dispHP->sok, &readfds);
        while ((ready = NI_select(FD_SETSIZE, &readfds, NULL, NULL, &timeout)) < 0) {
            if (SOCK_ERRNO == EINTR)
                ;                   /* repeat while interrupted */
            else {
                MsgDestroyHandle(disp->svcsHP);
                disp->svcsHP = NULL;
                ni_errno = NIE_SELECT;              /* select error */
                NI_DestroyDispInfo(dispinfo);
                ErrPostEx (SEV_ERROR, CTX_NCBICORE, CORE_UNKNOWN, "NI_InitServices: <%s>", ni_errlist[ni_errno]);
                return -1;
            }
        }

        if (FD_ISSET(disp->dispHP->sok, &readfds) != 0) {
            if ((imp = MsgRead(disp->dispHP, FALSE)) == NULL) {
                if (getAsnError(ni_errtext) == ECONNRESET)
                    ni_errno = NIE_MAXCONNS;
                else
                    ni_errno = NIE_MSGREAD;

                MsgDestroyHandle(disp->svcsHP);
                disp->svcsHP = NULL;
                NI_DestroyDispInfo(dispinfo);
                HaltServices (disp);
                ErrPostEx (SEV_ERROR, CTX_NCBICORE, CORE_UNKNOWN, "NI_InitServices: <%s>", ni_errlist[ni_errno]);
                return -1;
            }
            switch (imp->type) {
              case NI_ACK:
                /************************************************************/
                /* even though we connected successfully to the dispatcher, */
                /* it may have given us more up-to-date information on the  */
                /* latest list of dispatchers which should be tried; if so, */
                /* pass the updated list back to the caller                 */
                /************************************************************/
                if (imp->msun.ack->dispinfo != NULL) {
                    if (dispinfo != NULL)
                    {
                        NI_DestroyDispInfo(dispinfo);
                        dispinfo = NULL;
                    }
                    dispinfo = imp->msun.ack->dispinfo;
                    imp->msun.ack->dispinfo = NULL; /* for clean free */
                }
                if (disp->encryptInfo != NULL && NI_EncrAvailable())
                {
                    if (dispinfo != NULL && dispinfo->pubKey != NULL)
                    {
                        failed = FALSE;
                        if (pubKey == NULL)
                        {
                            failed = Message(MSG_YN, ERR_KEY_NOPREVKEY) ==
                                     ANS_NO;
                            ni_errno = NIE_NEWKEYNOTACCPT;
                        } else {
                            if (! NI_PubKeysEqual(pubKey, (NI_PubKeyPtr) dispinfo->pubKey))
                            {
                                failed = Message(MSG_YN, ERR_KEY_MISMATCH) ==
                                         ANS_NO;
                                ni_errno = NIE_NEWKEYMISMATCH;
                            }
                        }
                        if (failed)
                        {
                            HaltServices(disp);
                            MsgDestroy(imp);
                            return -1;
                        } else {
                            /* replace the key */
                            NI_DestroyPubKey((NIPubKeyPtr) pubKey);
                            pubKey = (NI_PubKeyPtr) dispinfo->pubKey;
                            dispinfo->pubKey = NULL;
                            disp->encryptInfo->data.ptrvalue = (Pointer) pubKey;
                        }
                    }
                }
                if (dispinfo != NULL && dip != NULL) {
                    if (*dip != NULL)
                        NI_DestroyDispInfo((NIDispInfoPtr) *dip);
                    *dip = (NI_DispInfoPtr) dispinfo;
                    dispinfo = NULL;
                    retval = 1;
                }
                else {
                    NI_DestroyDispInfo(dispinfo);
                }
                if (imp->msun.ack->motd != NULL &&
                    imp->msun.ack->motd[0] != NULLB)
                {
                    disp->motd = imp->msun.ack->motd;
                    imp->msun.ack->motd = NULL; /* for clean free */
                }
                if (imp->msun.ack->adminInfo != NULL &&
                    imp->msun.ack->adminInfo[0] != NULLB)
                {
                    disp->adminInfo = imp->msun.ack->adminInfo;
                    imp->msun.ack->adminInfo = NULL; /* for clean free */
                }
#ifdef OS_UNIX
                signal(SIGPIPE, SIG_IGN); /* catch socket errors */
#endif /* OS_UNIX */
                MsgDestroy(imp);
                disp->referenceCount++;
                if (currentDisp == NULL)
                {
                    currentDisp = disp;
                }
                return retval;   /* only good return */

              case NI_NACK:
                ni_errno = (enum ni_error) imp->msun.nack->code;
                if (imp->msun.nack->reason != NULL)
                {
                    StringCpy(ni_errtext, imp->msun.nack->reason);
                } else {
                    ni_errtext[0] = '\0';
                }
                if (dispinfo != NULL)
                {
                    NI_DestroyDispInfo(dispinfo);
                    dispinfo = NULL;
                }
                dispinfo = imp->msun.nack->dispinfo;
                imp->msun.nack->dispinfo = NULL; /* for clean free */
                if (ni_errno == NIE_BACKUPDISP && dispinfo != NULL &&
                    dispinfo->numdispatchers > 0 && dip != NULL &&
                    ++altDispTries < MAX_ALT_DISP_TRIES)
                {
                    MsgDestroy(imp);
                    HaltServices (disp);
                    NI_SetDispatcher(disp, dispinfo->displist[0], disp->dispServiceName,
                                     disp->dispTimeout, dispinfo->serialno, disp->encryptInfo,
                                     disp->useOutServ);
                    newDispToTry = TRUE;
                    retval = 2;
                    break;
                }
                MsgDestroy(imp);
                MsgDestroyHandle(disp->svcsHP);
                disp->svcsHP = NULL;
                NI_DestroyDispInfo(dispinfo);
                HaltServices (disp);
                ErrPostEx (SEV_ERROR, CTX_NCBICORE, CORE_UNKNOWN, "NI_InitServices: <%s>\n%s", ni_errlist[ni_errno], ni_errtext);
                return -1;

              default:
                MsgDestroy(imp);
                ni_errno = NIE_MSGUNK;
                MsgDestroyHandle(disp->svcsHP);
                disp->svcsHP = NULL;
                NI_DestroyDispInfo(dispinfo);
                HaltServices (disp);
                ErrPostEx (SEV_ERROR, CTX_NCBICORE, CORE_UNKNOWN, "NI_InitServices: <%s>", ni_errlist[ni_errno]);
                return -1;
            }
        }
    } while (newDispToTry);

    MsgDestroyHandle(disp->svcsHP);
    disp->svcsHP = NULL;
    ni_errno = NIE_LOGTIMEOUT;          /* TIMEOUT */
    NI_DestroyDispInfo(dispinfo);
    HaltServices (disp);
    ErrPostEx (SEV_ERROR, CTX_NCBICORE, CORE_UNKNOWN, "NI_InitServices: <%s>", ni_errlist[ni_errno]);
    return -1;
} /* NI_InitServices */


/*
 * Purpose:     Init network services based on information in config file
 *
 * Parameters:
 *   configFile     Name of NCBI-style configuration file.  If NULL, defaults
 *                     to "NCBI"
 *   configSection  Section with NCBI-style configuration file.  If NULL,
 *                     defaults to "NET_SERV"
 *   showMonitor    Boolean; if TRUE, display a monitor while re-trying
 *                     for an alternate dispatcher
 *   lastDispatcher Pointer to where this function should store the name
 *                  of the dispatcher which was actually used (may be NULL
 *                  if the caller does not care about this value)
 *   lastDispLen    Maximum length of lastDispatcher
 *
 * Returns:
 *                 NULL, if unable to contact dispatcher
 *                 a pointer to the Dispatcher structure, otherwise
 *
 *
 * Description:
 *              Extracts a dispatcher name and a user name from a configuration
 *              file.  If necessary, tries other dispatchers, in order, as
 *              listed in configuration file.  Also sets up encryption, if
 *              the client is encryption-capable and encryption is requested
 *              in the configuration file.
 *
 *
 * Note:
 *              This function is provided as a convenience to developers who
 *              wish to use Network Services.  Use of this function is not
 *              integral to the use of Network Services ... it is merely a
 *              convenience.
 */

NLM_EXTERN NI_DispatcherPtr NI_GenericInit (CharPtr configFile, CharPtr configSection, Boolean showMonitor, CharPtr lastDispatcher, Int2 lastDispLen)
{
    char *def_user;
    char username[64];
    char groupname[20];
    char password[20];
    char dispname[60];
    char disp_config[10];
    char disp_msg[110];
    char buf[60];
    Boolean more_disps;
    int alternate = 1;
    int disp_timeout;
    Int4 disp_serialno;
    Monitor *mon = NULL;
    NI_DispInfoPtr dip = NULL;
    NI_DispatcherPtr disp = NULL;
    Boolean someBrokered;
    Boolean useOutServ;
    ValNodePtr encryptInfo = NULL;
    NI_PubKeyPtr keyCopy = NULL;
    Boolean doEncr = FALSE;
    Boolean quitOnDispConnFailure = FALSE;
    Boolean showMessage = FALSE;
    Int4 numBrokeredServices;
#ifdef OS_UNIX
    char *getlogin PROTO((void));
#endif

    /******************* open the network connnection *********/
#define NI_DISP_NAME "dispatch1.nlm.nih.gov"
#define NI_USER_NAME "anonymous"
#define NI_GROUP_NAME "GUEST"

    def_user = NI_USER_NAME;

    if (configFile == NULL)
        configFile = "NCBI";
    if (configSection == NULL)
        configSection = "NET_SERV";

    GetAppParam(configFile, configSection, "DISP_USERNAME", NI_USER_NAME, username,
                sizeof username);
    /* the user's login name overrides the config file */
    /* for UNIX or VMS systems (or, for the future, any system where the      */
    /* user name can be determined), use the user's login name as the default */
    def_user = NULL;
#ifdef OS_UNIX
    if ((def_user = getenv("USER")) == NULL)
    {
        def_user = getlogin();
    }
#endif
#ifdef OS_VMS
    def_user = getenv("USER");
#endif
    if (def_user != NULL)
    {
        StrNCpy(username, def_user, sizeof username);
    }

    GetAppParam(configFile, configSection, "DISP_GROUPNAME", NI_GROUP_NAME, groupname,
                sizeof groupname);
    GetAppParam(configFile, configSection, "DISP_PASSWORD", "", password,
                sizeof password); /* default = NONE */

    GetAppParam(configFile, configSection, "DISP_TIMEOUT", "0", buf, sizeof buf);
    disp_timeout = atoi(buf);

    GetAppParam(configFile, configSection, "DISPSERIALNO", "0", buf, sizeof buf);
    disp_serialno = atoi(buf);

    GetAppParam(configFile, configSection, "DISPATCHER", NI_DISP_NAME, dispname,
                sizeof dispname);

    GetAppParam(configFile, configSection, "DIRECT_SVC_CON", "FALSE", buf,
                sizeof(buf));
    useOutServ = StrICmp(buf, "TRUE") == 0;

    GetAppParam(configFile, configSection, "SOME_BROKERED", "FALSE", buf, sizeof buf);
    someBrokered = StrICmp(buf, "TRUE") == 0;
    if (someBrokered) {
        GetAppParam(configFile, configSection, "BROKERED_COUNT", "1", buf, sizeof buf);
        numBrokeredServices = atoi(buf);
        if (numBrokeredServices <= 0) {
            someBrokered = FALSE;
        } else {
            sprintf (buf, "%ld", (long) numBrokeredServices - 1);
            TransientSetAppParam(configFile, configSection, "BROKERED_COUNT", buf);
        }
    }

    GetAppParam(configFile, configSection, "DISP_RECONN_ACTION", "CONT", buf, sizeof buf);
    showMessage = StrICmp(buf, "ASK") == 0;
    quitOnDispConnFailure = StrICmp(buf, "QUIT") == 0;

    GetAppParam(configFile, configSection, "ENCRYPTION_DESIRED", "FALSE", buf, sizeof buf);
    if (StrICmp(buf, "TRUE") == 0 && NI_EncrAvailable())
    {
        doEncr = TRUE;
        encryptInfo = ValNodeNew(NULL);
        encryptInfo->data.ptrvalue = (Pointer) NI_ReadPubKeyFromConfig();
        keyCopy = NI_PubKeyDup((NI_PubKeyPtr) encryptInfo->data.ptrvalue);
    }

    do {
        if (alternate == 2 && showMonitor)
        {
            mon = MonitorStrNew("Unable to contact primary dispatcher", 35);
        }
        if (alternate >= 2)
        {
            if (showMessage)
            {
                sprintf (disp_msg, "Unable to contact primary dispatcher.  Ready to try\ndispatcher #%d <", alternate);
                StrCat(disp_msg, dispname);
                StrCat(disp_msg, ">.  Continue?");
                if (Message(MSG_YN, disp_msg) == ANS_NO)
                    break;
            } else {
                sprintf(disp_msg, "Trying dispatcher #%d <", alternate);
                StrCat(disp_msg, dispname);
                StrCat(disp_msg, ">");
                if (showMonitor) {
                    MonitorStrValue(mon, disp_msg);
                }
            }
        }

        if (lastDispatcher != NULL) {
                StrNCpy(lastDispatcher, dispname, lastDispLen);
        }

        disp = NI_SetDispatcher (NULL, dispname, NULL, disp_timeout, disp_serialno, encryptInfo, useOutServ);
        
        if (someBrokered) {
            disp->brokeredDummy = TRUE;
            disp->someBrokered = TRUE;
            disp->referenceCount++;
            ValNodeFree (encryptInfo);
            return disp;
        }

        if (NI_InitServices(disp, username, groupname[0] == '\0' ? 0 : groupname,
                            password[0] == '\0' ? 0 : password, &dip) >= 0)
        {
            if (dip != NULL && dip->serialno != disp_serialno) {
                NI_SetDispConfig (&dip, dispname, sizeof dispname);
            }
            if (mon != NULL)
                MonitorFree(mon);
            if (disp->encryptInfo != NULL &&
                disp->encryptInfo->data.ptrvalue != NULL &&
                ! NI_PubKeysEqual(keyCopy,
                 (NI_PubKeyPtr) disp->encryptInfo->data.ptrvalue))
            {
                NI_WritePubKeyToConfig ((NI_PubKeyPtr) disp->encryptInfo->data.ptrvalue);
            }
            NI_DestroyPubKey ((NIPubKeyPtr) keyCopy);
            return disp;
        }
        ErrShow ();
        NI_EndServices (disp);
        sprintf(disp_config, "DISP_ALT_%d", alternate++);
        more_disps = GetAppParam(configFile, configSection, disp_config, "", dispname,
                                 sizeof dispname);
        if (doEncr)
        {
            encryptInfo = ValNodeNew(NULL);
            encryptInfo->data.ptrvalue = (Pointer)
                                          NI_PubKeyDup((NI_PubKeyPtr) keyCopy);
        }
    } while (more_disps && ! quitOnDispConnFailure);

    if (mon != NULL)
        MonitorFree(mon);

    ValNodeFree (encryptInfo);
    NI_DestroyPubKey ((NIPubKeyPtr) keyCopy);
    ErrPostEx(SEV_ERROR,0,0, "NI_InitServices: Unable to connect to any dispatcher");
    return NULL;
}


/*
 * Purpose:     Get a network service based on information in config file
 *
 * Parameters:
 *   disp           Pointer to the dispatcher structure obtained from a
 *                  previous call to NI_SetDispatcher or NI_GenericInit
 *   configFile     Name of NCBI-style configuration file.  If NULL, defaults
 *                     to "NCBI"
 *   defService     The default service/resource/resource-type name, if
 *                  not specified otherwise in configuration file.
 *   hasResource    Boolean; if TRUE, ask for a resource when requesting
 *                     service
 *
 * Returns:
 *                 NULL, if unable to obtain service
 *                 a pointer to the service-structure, otherwise
 *
 *
 * Description:
 *              Extracts a service name and other service data from a
 *              configuration file, and attempts to obtain that service.
 *              As a special case, handle communication with a "brokered
 *              server" (a server which is already listening on a port, where
 *              no communication needs to be performed with the dispatcher).
 *              Also disable data encryption for this service request, if
 *              explicitly specified in the configuration file.
 *
 *
 * Note:
 *              This function is provided as a convenience to developers who
 *              wish to use Network Services.  Use of this function is not
 *              integral to the use of Network Services ... it is merely a
 *              convenience.
 *
 *              For UNIX systems, environment variables can be used to
 *              override the config. file's values for SERVICE_NAME and
 *              RESOURCE_TYPE.
 */

NLM_EXTERN NI_HandPtr NI_GenericGetService (NI_DispatcherPtr disp, CharPtr configFile, CharPtr configSection, CharPtr defService, Boolean hasResource)
{
    char buf[40];
    char service[40];
    char resource[40];
    char res_type[40];
    int serv_min;
    int serv_max;
    int res_min;
    int res_max;
    char brokeredIpaddr[40];
    Uint2 port;
    NI_HandPtr result;
    ValNodePtr savEncrypt;
#ifdef OS_UNIX
    CharPtr envName = (CharPtr)MemNew(StrLen(configSection) + 20);
    CharPtr envValue;
#endif

    if (configFile == NULL)
        configFile = "NCBI";

    GetAppParam(configFile, configSection, "SERVICE_NAME", defService,
            service, sizeof service);
#ifdef OS_UNIX
    /* environment variable overrides config. file */
    sprintf (envName, "NI_SERVICE_NAME_%s", configSection);
    if ((envValue = getenv(envName)) != NULL)
    {
        StrCpy (service, envValue);
    }
#endif /* OS_UNIX */
    GetAppParam(configFile, configSection, "SERV_VERS_MIN", "1",
            buf, sizeof buf);
    serv_min = atoi(buf);
    GetAppParam(configFile, configSection, "SERV_VERS_MAX", "0",
            buf, sizeof buf);
    serv_max = atoi(buf);

    res_min = 1;
    res_max = 0;

    if (hasResource) {
        GetAppParam(configFile, configSection, "RESOURCE_NAME", defService,
                resource, sizeof resource);
        GetAppParam(configFile, configSection, "RESOURCE_TYPE", defService,
                res_type, sizeof res_type);
#ifdef OS_UNIX
        /* environment variable overrides config. file */
        sprintf (envName, "NI_RESOURCE_TYPE_%s", configSection);
        if ((envValue = getenv(envName)) != NULL)
        {
            StrCpy (res_type, envValue);
        }
#endif /* OS_UNIX */
        GetAppParam(configFile, configSection, "RES_VERS_MIN", "1",
                buf, sizeof buf);
        res_min = atoi(buf);
        GetAppParam(configFile, configSection, "RES_VERS_MAX", "0",
                buf, sizeof buf);
        res_max = atoi(buf);
    }

#ifdef OS_UNIX
    MemFree (envName);
#endif /* OS_UNIX */

    if (disp->someBrokered)
    {
        GetAppParam(configFile, configSection, "BROKERED_PORT", "0",
                    buf, sizeof buf);
        port = htons(atoi(buf));
        GetAppParam(configFile, configSection, "BROKERED_IPADDR", "",
                    brokeredIpaddr, sizeof brokeredIpaddr);
        if (port != 0 && brokeredIpaddr[0] != '\0')
        { /* simulate service request by connecting to that port */
            struct  sockaddr_in serv_addr;
            NI_HandPtr sHP;
            int timeout = 30;
            int status;

            MemFill((VoidPtr) &serv_addr, '\0', sizeof(serv_addr));
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_addr.s_addr = inet_addr(brokeredIpaddr);
            serv_addr.sin_port = port;

            if ((sHP = MsgMakeHandle(TRUE)) == NULL)
                return NULL;
            MsgSetLJError(sHP);
            sHP->hostname = StringSave(brokeredIpaddr);

            if (activityHook != NULL)
            {
                activityHook(sHP, NetServHook_svcreq, 0);
            }

          RETRY:
#ifndef NETP_INET_NEWT
            if ((status = NI_CONNECT(sHP->sok, (struct sockaddr PNTR) &serv_addr, sizeof(serv_addr))) < 0) { /* } */
#else
            if ((status = NI_CONNECT(sHP->sok, &serv_addr, sizeof(serv_addr))) < 0) {
                SOCK_ERRNO = ABS(status);
#endif
                switch (SOCK_ERRNO) {
                  case EINTR:
                    goto RETRY;

#ifdef NETP_INET_PCNFS
                  /* This is apparently a bug in PC-NFS 4.0 ... a connection attempt */
                  /* on a non-blocking socket yields errno == 0                      */
                  case 0:
#endif /* NETP_INET_PCNFS */
                  case EWOULDBLOCK:
                  case EINPROGRESS:
                    /* if the connect()ion is not established immediately, a         */
                    /* select() can be performed where the corresponding "write"     */
                    /* file descriptor will be enabled once the connect()ion has been*/
                    /* established                                                   */
                    if (sokselectw(sHP->sok, timeout) == 0) {
                        return sHP;
                    }
                    break;

                  default:
                    break;
                }
                MsgDestroyHandle(sHP);
                ni_errno = NIE_BROKSVCCONN;        /* can't connect to brokered service */
                return NULL;
            }
            {
                Char buf[16];
                Char key[8];
                Int2 len;
        
                if ((len = GetAppParam("NCBI", "NET_SERV", "DESKEY", "", buf, sizeof buf)) > 0 &&
                    (AsnTypeStringToHex(buf, len, key, NULL) == 0))
                    
                {
                    NI_SetupDESEncryption(sHP, (UcharPtr) key);
                }
            }
            return sHP;
        } else {
            if (disp->brokeredDummy)
            { /* JAE ... establish a true dispatcher connection here */
            }
        }
    }

    savEncrypt = disp->encryptInfo;
    if (GetAppParam(configFile, configSection, "ENCRYPTION", "TRUE",
                    buf, sizeof buf) > 0 && StrICmp(buf, "FALSE") == 0)
    {
        /* temporarily disable encryption */
        disp->encryptInfo = NULL;
    }
    result = NI_ServiceGet(disp, service, serv_min, serv_max,
                           hasResource ? resource : 0, res_type, res_min,
                           res_max);
    disp->encryptInfo = savEncrypt;

    return result;
}


/*
 * Purpose:     Write dispatcher-configuration information to a config file
 *
 * Parameters:
 *   dipp           A pointer to the caller's list of dispatchers, obtained
 *                     from NI_InitServices()
 *   dispatcher     The caller's dispatcher string
 *   dispLen        Length of the caller's dispatcher string
 *
 * Returns:
 *                 0, if bad parameters were provided
 *                 the dispatcher-list serial number, otherwise
 *
 *
 * Description:
 *              Sets up the "NCBI" configuration file with the following
 *              entries in the "NET_SERV" section:
 *              * DISPATCHER is the primary dispatcher name
 *              * DISP_ALT_n for every alternate dispatcher, 1 <= n, a smaller
 *                n indicates a higher priority alternate dispatcher
 *              * DISPSERIALNO is the serial number of the dispatcher list
 *                obtained from a remote dispatcher. This serial number should
 *                be unique for all time ... the dispatcher's serial number
 *                must be changed whenever the master list is modified.
 *
 * Note:
 *              This configuration mechanism is only _one_ recommended
 *              mechanism for network services dispatcher configuration. The
 *              application may perform this configuration in any manner
 *              deemed appropriate by the application programmer.
 *
 *              The value returned by this function is the recommended value
 *              for the dispserialno parameter in a subsequent call to
 *              NI_InitDispatcher().
 */

NLM_EXTERN Int4 NI_SetDispConfig(NI_DispInfoPtr PNTR dipp, CharPtr dispatcher, Int2 dispLen)
{
  Int2 num;
  Char dispConfig[20];
  char buf[10];
  Int4 retval;
  NI_DispInfoPtr dip;

  if (dipp == NULL || (dip = *dipp) == NULL)
  {
    if (dispatcher != NULL)
    {
      dispatcher[0] = '\0';
    }
    return 0;
  }

  if (dip->numdispatchers > 0 && dip->displist != NULL)
  {
    StringNCpy (dispatcher, dip->displist[0], dispLen);
    SetAppParam ("NCBI", "NET_SERV", "DISPATCHER", dip->displist[0]);
  }

  for (num = 1; num < dip->numdispatchers; num++)
  {
    sprintf (dispConfig, "DISP_ALT_%d", num);
    SetAppParam ("NCBI", "NET_SERV", dispConfig, dip->displist[num]);
  }

  /* wipe out any extraneous old configuration */
  for (num = dip->numdispatchers; num < 100; num++)
  {
    sprintf (dispConfig, "DISP_ALT_%d", num);
    if (GetAppParam("NCBI", "NET_SERV", dispConfig, "", buf, sizeof buf) <= 0)
    {
      break;
    }
    SetAppParam ("NCBI", "NET_SERV", dispConfig, NULL);
  }

  retval = dip->serialno;
  sprintf (buf, "%ld", (long) dip->serialno);
  SetAppParam ("NCBI", "NET_SERV", "DISPSERIALNO", buf);

  NI_DestroyDispInfo ((NIDispInfoPtr) dip);
  *dipp = NULL;

  return retval;
}


/*
 * Purpose:     End use of network services
 *
 * Parameters:
 *   disp           A pointer to the dispatcher structure
 *
 * Returns:
 *                 0 (always)
 *
 *
 * Description:
 *              Tear down the sockets and data structures associated with
 *              the dispatcher and a server, and free all memory associated
 *              with data structures.
 */

NLM_EXTERN Int2 NI_EndServices(NI_DispatcherPtr disp)
{
    Int2 openSockets;

    if (disp == NULL)
        return 0;

    if (disp->referenceCount > 0)
        disp->referenceCount--;

    if (disp->referenceCount <= 0)
    {
        if (disp == currentDisp)
        {
            currentDisp = NULL;
        }
    
        HaltServices (disp);
        NI_SetDispatcher(disp, NULL, NULL, 0, 0, NULL, FALSE);               /* free mem */
    
        if (stackDescription != NULL)
        {
            MemFree(stackDescription);
            stackDescription = NULL;
        }
        MemFree(disp->adminInfo);
        MemFree(disp->motd);
        MemFree(disp);

        /* For historical reasons pertaining to Network Entrez, a single open
           socket does not constitute an error in this context.  However, at
           program exit time, a single open socket does constitute a serious
           problem. */
        if ((openSockets = NI_SocketsOpen()) > 1)
        {
            ErrPostEx(SEV_WARNING,0,0, "At end-services time, %d sockets are still open", openSockets);
        }
    }

    return 0;
} /* NI_EndServices */



/*
 * Purpose:     Request a catalog from the dispatcher
 *
 * Parameters:
 *   disp           A pointer to the dispatcher structure
 *
 * Returns:
 *                 NULL, if unable to obtain the catalog
 *                 a pointer to the received catalog data structure, otherwise
 *
 *
 * Description:
 *              Send a request to the dispatcher, requesting a catalog, and
 *              wait (up to some timeout) for a response. The dispatcher's
 *              response should either be that catalog, or a NACK.
 */

NLM_EXTERN NICatalogPtr NI_GetCatalog(NI_DispatcherPtr disp)
{
    NICatalogPtr        catp;
    NIMsgPtr            mp, imp;
    NICmdPtr            cmdp;
    struct timeval      timeout;
    int                 ready;
    fd_set              readfds;

    if (disp == NULL)
        return NULL;

    cmdp = (NICmdPtr) NI_MakeMsgCmd();
    cmdp->seqno = disp->dispHP->seqno++;
    cmdp->code = NI_SEND_CATALOG;
    if ((mp = MsgBuild(NI_COMMAND, disp->dispHP->conid, (VoidPtr) cmdp)) == NULL) {
        ni_errno = NIE_MISC;    /* unable to alloc mem for Msg */
        return NULL;
    }
    if (MsgWrite(disp->dispHP, mp, FALSE) < 0) {
        ni_errno = NIE_MSGWRITE;
        return NULL;
    }

    /* blocks until response from dispatcher or TIMEOUT */

    timeout.tv_sec = (Uint4) NI_TIMEOUT_SECS;
    timeout.tv_usec = 0;
    FD_ZERO(&readfds);
    FD_SET(disp->dispHP->sok, &readfds);
    while ((ready = NI_select(FD_SETSIZE, &readfds, NULL, NULL, &timeout)) < 0) {
        if (SOCK_ERRNO == EINTR)
            ;                   /* repeat while interrupted */
        else {
            ni_errno = NIE_SELECT;              /* select error */
            return NULL;
        }
    }

    if (FD_ISSET(disp->dispHP->sok, &readfds) != 0) {
        if ((imp = MsgRead(disp->dispHP, FALSE)) == NULL) {
            LOG_SOCKET(disp->dispHP->sok, FALSE);
            NI_CLOSESOCKET(disp->dispHP->sok);
            ni_errno = NIE_MSGREAD;
            return NULL;
        }
        switch (imp->type) {
          case NI_CATALOG:
            catp = imp->msun.catalog;
            imp->msun.catalog = NULL;
            ni_errno = NIE_NO_ERROR;
            MsgDestroy(imp);
            return catp;

          case NI_NACK:
            ni_errno = (enum ni_error) imp->msun.nack->code;
            if (imp->msun.nack->reason != NULL)
                StringCpy(ni_errtext, imp->msun.nack->reason);
            else
                ni_errtext[0] = '\0';
            MsgDestroy(imp);
            return NULL;

          default:
            MsgDestroy(imp);
            ni_errno = NIE_MSGUNK;      /* Unknown MSG type */
            return NULL;
        }
    }
    ni_errno = NIE_CMDTIMEOUT;          /* TIMEOUT */
    return NULL;
} /* NI_GetCatalog */



/*
 * Purpose:     Create the data structure for a service request
 *
 * Parameters:
 *   disp           A pointer to the dispatcher structure
 *
 * Returns:
 *                 a pointer to the newly created data structure
 *
 *
 * Description:
 *              Allocate the memory for a service request data structure,
 *              and fill in some of the fields.
 * Note:
 *              There are two ways for a program to issue a service request:
 *              (1) Multi-step, general method (like IRS form 1040)
 *                * Build a request with NI_SVCRequestBuild()
 *                * Populate the request with a specific service request using
 *                  NI_RequestSetService()
 *                * Populate the request with zero or more resource requests
 *                  calling NI_RequestAddResource() once for every resource
 *                * Send the request with NI_ServiceRequest(), and (hopefully)
 *                  obtain a connection to a service provider
 *                * At some later time, delete the request (to save memory)
 *              (2) One-stop shopping, for simple requirement (like form 1040EZ)
 *                * Do everything for a service and up to one resource using
 *                  NI_ServiceGet()
 */

NLM_EXTERN NI_ReqPtr NI_SVCRequestBuild(NI_DispatcherPtr disp)
{
    NI_ReqPtr   reqp;

    if (disp == NULL)
        return NULL;

    reqp = (NI_ReqPtr) NI_MakeRequest();
    reqp->clientPort = (Uint2) disp->clientPort;
    if (disp->useSocks)
    {
        /* tell the Dispatcher that it should use getpeername() to determine */
        /* the IP address of the SOCKS daemon                                */
        reqp->clientAddr = StringSave("0.0.0.0");
    } else {
        reqp->clientAddr = StringSave(disp->localHostAddr);
    }
    reqp->dispatcher = disp; /* should not be freed when destroying Req */

    return  reqp;
} /* NI_SVCRequestBuild */



/*
 * Purpose:     Destroy a service request data structure
 *
 * Parameters:
 *   reqp         A pointer to the data structure to be destroyed
 *
 *
 * Description:
 *              Free all the resources associated with a service request
 */

NLM_EXTERN void NI_SVCRequestDestroy(NI_ReqPtr reqp)
{
    NI_DestroyRequest(reqp);
} /* NI_SVCRequestDestroy */



/*
 * Purpose:     Make a service request for a service and up to one resource
 *
 * Parameters:
 *   disp         A pointer to the dispatcher structure
 *   svc          Name of requested service
 *   svcvermin    Minimum version number requested for this service
 *   svcvermax    Maximum version number requested for this service
 *   res          Name of requested resource (possibly NULL)
 *   resvermin    Minimum version number requested for this resource
 *   resvermax    Maximum version number requested for this resource
 *
 * Returns:
 *                The result of the service request
 *
 *
 * Description:
 *              Create and issue a service request for the specified
 *              parameters.
 */

NLM_EXTERN NI_HandPtr NI_ServiceGet(NI_DispatcherPtr disp, CharPtr svc, Uint2 svcvermin, Uint2 svcvermax, CharPtr res, CharPtr restype, Uint2 resvermin, Uint2 resvermax)
{
    NI_ReqPtr   reqp;

    if (disp == NULL)
        return NULL;

    reqp = NI_SVCRequestBuild(disp);
    NI_RequestSetService(reqp, svc, svcvermin, svcvermax);
    if (res != NULL)
        NI_RequestAddResource(reqp, res, restype, resvermin, resvermax);

    return NI_ServiceRequest(reqp);
} /* NI_ServiceGet */



/*
 * Purpose:     Issue the specified service request
 *
 * Parameters:
 *   req          The pre-formatted service request
 *
 * Returns:
 *               A message handle to the server which is servicing our request,
 *                   if successful
 *               NULL, otherwise (ni_errno will indicate a more precise cause)
 *
 *
 * Description:               
 *              Create and issue a service request for the specified
 *              service request, as follows:
 *
 *              * Create a data structure to which the resulting service
 *                connection can be attached
 *              * Send the service request to the dispatcher
 *
 *                             old - style connection:
 *
 *              * Wait for the following two events, in either order:
 *                (1) A response from the dispatcher, which is either a
 *                    SVC_RESPONSE (good), or a NACK (bad) {or a timeout}
 *                (2) A connection request from the server, which we then
 *                    accept()
 *              * If both of the two events occur successfully, return with
 *                success, else, return with failure.
 *
 *                      old-style / direct connection switch 
 *                           (currently disabled)
 *
 *              * After NACK was received in old - style connection we don't 
 *                worry, set disp->useOutServ to TRUE and return to the beginning
 *                of the function. All service request will be repeated with
 *                direct connection.                      
 *
 *                             direct connection:
 *
 *              * Wait for the following events:
 *                (1) A response from the dispatcher, which is either a
 *                    SVC_REQUEST (good), or a NACK (bad) {or a timeout}
 *              * In the case of SVC_REQUEST we send this request directly
 *                to the server and wait for next event:
 *                (2) A response from the dispatcher, which is either a
 *                    SVC_RESPONSE (good), or a NACK (bad) {or a timeout} 
 *              * If both of the two events occur successfully, return with
 *                success, else, return with failure.
 *
 * Note:
 *              If the caller's Dispatcher data structure indicates that
 *              encryption should be performed, then a DES key is
 *              pseudorandomly generated prior to issuing the service request,
 *              and is encrypted using public-key encryption.  Following
 *              successful establishment of the client<->server session, the
 *              DES key is used to encrypt the ensuing session using
 *              cypher-block-chaining.
 *
 *              For SOCKSified clients, a different protocol is used where
 *              the client first sends a "pre-service-request", asking the
 *              IP address of the computer to which the Dispatcher will assign
 *              the request.  Upon receipt of a NI_SVC_PRE_RESPONSE message
 *              containing that IP address, the client performs a SOCKSified
 *              bind()ing indicating that the specified IP address will
 *              "call back".  Having determine the port number which has
 *              been bound on the SOCKS proxy, the client sends the "real"
 *              service request containing the SOCKS proxy's port number
 *              and a reminder as to which IP address the Dispatcher
 *              "promised" would be assigned.  After that, processing
 *              proceeds normally, with the server connecting-back (via
 *              the SOCKS proxy) and the Dispatcher sending a SVC_RESPONSE
 *              acknowledgement.
 *
 *              Note that SOCKS and encryption are completely orthogonal
 *              with respect to each other, and a client may use either, both,
 *              or neither.
 */

NLM_EXTERN NI_HandPtr NI_ServiceRequest(NI_ReqPtr req)
{
    NI_HandPtr          sconnhp;
#ifdef NETP_INET_MACTCP
    Int4                sconnlen;
#else
    int                 sconnlen;
#endif
    struct sockaddr_in  sconnaddr;
    NIMsgPtr            mp, imp;
    NISvcReqPtr         svcreqp;
    struct timeval      timeout;
    Monitor             *mon = NULL;
    int                 ready;
    Boolean             disp_contact = FALSE, serv_contact = FALSE;
    Uint4               this_req;
    fd_set              readfds;
    NI_DispatcherPtr    disp = req->dispatcher;
    Uchar               desKey[8];
#ifdef NETP_SOCKS
    struct sockaddr_in  svcsAddr;
    Int2                status;
#endif

NEXTTRY:
    ni_errtext[0] = '\0';
    if (disp->useOutServ) {
        if ((sconnhp = MsgMakeHandle(TRUE)) == NULL) {
            ni_errno = NIE_MAKEHAND;
            return NULL;
        }
    } else {
        if ((sconnhp = MsgMakeHandle(FALSE)) == NULL) {
            ni_errno = NIE_MAKEHAND;
            return NULL;
        }
    }

    svcreqp = NI_MakeMsgSvcreq();
    svcreqp->seqno = disp->dispHP->seqno++;
    svcreqp->platform = (Uint4) NI_GetPlatform();
    if (stackDescription != NULL)
    {
        svcreqp->applId = StringSave(stackDescription);
    }
    if (disp->encryptInfo != NULL && NI_EncrAvailable())
    {
        UcharPtr            encryptedDesKey;
        Int2                encryptedLen;

        NI_GenerateDESKey (desKey);
        encryptedLen = NI_PubKeyEncrypt((NI_PubKeyPtr) disp->encryptInfo->data.ptrvalue,
                         desKey, sizeof desKey, &encryptedDesKey);
        if (encryptedLen <= 0)
        {
            NI_DestroyRequest(req);
            MsgDestroyHandle(sconnhp);
            ni_errno = NIE_PUBKEYENCRFAIL;
            return NULL;
        }
        /* convert the DES key into a ByteStore */
        svcreqp->desKey = BSNew(encryptedLen);
        BSWrite (svcreqp->desKey, (VoidPtr) encryptedDesKey, encryptedLen);
        MemFree (encryptedDesKey);
    }
    this_req = svcreqp->seqno;
    CopyIdentity(disp, svcreqp->uid);
    NI_DestroyRequest(svcreqp->request);
    svcreqp->request = req;

    if (disp->useSocks)
    {
        svcreqp->wantPreResponse = TRUE;
    }
    /*  we want to get a ticket for the direct connection */
    if (disp->useOutServ) {
        svcreqp->want_ticket = TRUE;
    } 
    
    if ((mp = MsgBuild(NI_SVC_REQUEST, disp->dispHP->conid, (VoidPtr) svcreqp)) == NULL) {
        NI_DestroyRequest(req);
        MsgDestroyHandle(sconnhp);
        ni_errno = NIE_MISC;            /* unable to alloc mem for Msg */
        return NULL;
    }

    if (MsgWrite(disp->dispHP, mp, disp->useSocks) < 0) {
        MsgDestroyHandle(sconnhp);
        ni_errno = NIE_MSGWRITE;
        return NULL;
    }

    /* blocks until SVC_RESPONSE from dispatcher and service or NACK or TIMEOUT */
    while ( !disp_contact || !serv_contact) {
        timeout.tv_sec = (Uint4) NI_TIMEOUT_SECS;
        timeout.tv_usec = 0;
        FD_ZERO(&readfds);
        FD_SET(disp->dispHP->sok, &readfds);
        if (! disp->useOutServ)
            FD_SET(disp->svcsHP->sok, &readfds);
        while ((ready = NI_select(FD_SETSIZE, &readfds, NULL, NULL, &timeout)) < 0) {
            if (SOCK_ERRNO == EINTR)
                ;                                       /* repeat while interrupted */
            else {
                MsgDestroyHandle(sconnhp);
                ni_errno = NIE_SELECT;          /* select error */
                return NULL;
            }
        }
        if (ready == 0) {
            MsgDestroyHandle(sconnhp);
            ni_errno = NIE_DSPTIMEOUT;          /* TIMEOUT */
            return NULL;
        }
        if (FD_ISSET(disp->dispHP->sok, &readfds) != 0) {
            if ((imp = MsgRead(disp->dispHP, FALSE)) == NULL) {
                LOG_SOCKET(disp->dispHP->sok, FALSE);
                NI_CLOSESOCKET(disp->dispHP->sok);
                MsgDestroyHandle(sconnhp);
                ni_errno = NIE_MSGREAD;
                return NULL;
            }
            disp_contact = TRUE;
            switch (imp->type) {
              case NI_SVC_PRE_RESPONSE:
#ifdef NETP_SOCKS
                if (disp->useSocks)
                {
                    /* must defer binding and listening for SOCKS connection */
                    /* until server's IP address is known                    */
            
                    TRACE("Processing SOCKS SVC_PRE_RESPONSE\n");
                    /* SOCKS can't deal well with non-blocking connections */
                    NI_SETBLOCKING(disp->svcsHP->sok);
    
                    if ((disp->clientPort = bindPort(disp->svcsHP->sok, &svcsAddr, disp->loport, disp->hiport, imp->msun.preresp->server_ip)) == 0) {
                        TRACE("bindPort failed\n");
                        MsgDestroyHandle(sconnhp);
                        disp->svcsHP = NULL;
                        ni_errno = NIE_NOBIND;                  /* can't bind a free application socket */
                        ErrPost (CTX_NCBICORE, CORE_UNKNOWN, "NI_ServiceRequest: <%s>", ni_errlist[ni_errno]);
                        return NULL;
                    }
                    TRACE("bindPort succeeded, port = %d\n", disp->clientPort);
                    if (NI_GETSOCKNAME(disp->svcsHP->sok, &svcsAddr, &sconnlen) >= 0)
                        disp->clientPort = ntohs(svcsAddr.sin_port);
                    req->clientPort = disp->clientPort;
                    svcreqp->server_ip = imp->msun.preresp->server_ip;
                    svcreqp->wantPreResponse = FALSE;
                    if (MsgWrite(disp->dispHP, mp, FALSE) < 0) {
                        MsgDestroyHandle(sconnhp);
                        ni_errno = NIE_MSGWRITE;
                        return NULL;
                    }
                    disp_contact = FALSE; /* now waiting to hear one more msg */
    
                    TRACE("After GETSOCKNAME, port = %d\n", disp->clientPort);
                    if ((status = NI_LISTEN(disp->svcsHP->sok, 5)) < 0) {
#ifdef NETP_INET_NEWT
                        SOCK_ERRNO = ABS(status);
#endif
                        TRACE("Listen failed, errno = %d\n", SOCK_ERRNO);
                        MsgDestroyHandle(sconnhp);
                        StringCpy(ni_errtext, strerror(SOCK_INDEX_ERRNO));
                        ni_errno = NIE_NOLISTEN;
                        ErrPost (CTX_NCBICORE, CORE_UNKNOWN, "NI_ServiceRequest: <%s> <port %d, errno %d>", ni_errlist[ni_errno], (int) disp->clientPort, (int) SOCK_ERRNO);
                        return NULL;
                    }
                }
#endif
                /* non-SOCKS clients ignore this message, and should never receive it */
                TRACE("Listen succeeded\n");
                MsgDestroy(imp);
                break;

              case NI_SVC_RESPONSE:
                if (disp->useSocks)
                {
                    TRACE("Got SOCKS service response from Dispatcher\n");
                }
                ni_errno = NIE_NO_ERROR;
                NI_DestroyRequest(disp->reqResponse);
                disp->reqResponse = imp->msun.svcresp->request;
                sconnhp->hostname = StringSave(disp->reqResponse->clientAddr);
                imp->msun.svcresp->request = NULL;
                MsgDestroy(imp);
                if (mon != NULL) {
                    MonitorStrValue(mon, "Direct connection established");
#ifdef OS_UNIX
                    sleep(1);
#endif /* OS_UNIX */
                    MonitorFree(mon);
                }
                break;

              case NI_NACK:
                if (disp->useSocks)
                {
                    TRACE("Got SOCKS NACK from Dispatcher\n");
                }
                ni_errno = (enum ni_error) imp->msun.nack->code;
                if (imp->msun.nack->reason != NULL)
                    StringCpy(ni_errtext, imp->msun.nack->reason);
                else
                    ni_errtext[0] = '\0';
                MsgDestroy(imp);

                /* retry in the case when old style connection failed now is disabled */

                if (FALSE && !disp->useOutServ) {
                    disp->useOutServ = TRUE;
                    mon = MonitorStrNew("Old type of connection failed", 40);
                    MonitorStrValue(mon, "Trying to establish direct connection");
                    serv_contact = FALSE;
                    disp_contact = FALSE;
                    goto NEXTTRY;
                } 
                else { 
                    MsgDestroyHandle(sconnhp);
                    return NULL;
                }
                /* Message with direct connection ticket was received */
            case NI_SVC_REQUEST:
              if ((sconnhp = NI_DirectServiceRequest(imp, sconnhp)) == NULL) {
                  TRACE("Unable to establish direct connection\n");
                  return NULL;
              }
              if (mon != NULL)
                    MonitorStrValue(mon, "Received ticket for direct connection");
                  
              serv_contact = TRUE;
              disp_contact = FALSE;  /* we are looking for SVC_RESPONCE */
              break;


              default:
                if (disp->useSocks)
                {
                    TRACE("Got SOCKS unknown message type from Dispatcher\n");
                }
                MsgDestroy(imp);
                MsgDestroyHandle(sconnhp);
                ni_errno = NIE_MSGUNK;          /* Unknown MSG type */
                sprintf(ni_errtext, "%d", imp->type);
                return NULL;
            }
        }
        if (!disp->useOutServ && (FD_ISSET(disp->svcsHP->sok, &readfds) != 0) && !serv_contact) {
            sconnlen = sizeof(sconnaddr);
#ifdef NETP_INET_NEWT
            sconnhp->sok = NI_ACCEPT(disp->svcsHP->sok, &sconnaddr, &sconnlen);
#else
            sconnhp->sok = NI_ACCEPT(disp->svcsHP->sok, (struct sockaddr PNTR) &sconnaddr, &sconnlen);
#endif /* NETP_INET_NEWT */
            LOG_SOCKET(sconnhp->sok, TRUE);
#ifdef NETP_SOCKS
            if (disp->useSocks)
            {
                TRACE("Got connection from server, socket %d\n", sconnhp->sok);
                MsgDestroyHandle(disp->svcsHP);
                disp->svcsHP = MsgMakeHandle(TRUE); /* for next time */
            }
#endif /* NETP_SOCKS */
            if (sconnhp->sok < 0) {
#ifdef NETP_INET_NEWT
                SOCK_ERRNO = ABS(sconnhp->sok);
#endif
                MsgDestroyHandle(sconnhp);
                StringCpy(ni_errtext, strerror(SOCK_INDEX_ERRNO));
                ni_errno = NIE_NOACCEPT;        /* application accept error */
                return NULL;
            }
            serv_contact = TRUE;
        }
    }

    if (activityHook != NULL)
    {
       (*activityHook)(sconnhp, NetServHook_svcreq, 0);
    }

    if (disp->encryptInfo != NULL && NI_EncrAvailable())
    {
        NI_SetupDESEncryption(sconnhp, desKey);
    }

    if (sconnhp != NULL)
    {
        sconnhp->disp = disp;
    }

    return sconnhp;
} /* NI_ServiceRequest */



/*
 * Purpose:       Issue the specified direct service request
 *
 * Parameters:
 *
 *   imp          The pre-formatted service request
 *   sconnhp      server handle to connect to
 *
 * Returns:
 *
 *   sconnhp     A message handle to the server which is servicing our request,
 *               if successful
 *
 * Description:  This function connects directly to server specified by sconnhp
 *               and sends SVC_REQUEST message imp, formated and completed by
 *               dispatcher and received by NI_ServiceRequest function of the client
 *
 *               
 */
static NI_HandPtr
NI_DirectServiceRequest(NIMsgPtr imp, NI_HandPtr sconnhp)
{
    int timeout = 30;
    int status;
    struct sockaddr_in  sconnaddr;
    char buf[20];

    MemFill((VoidPtr) &sconnaddr, '\0', sizeof(sconnaddr));
    sconnaddr.sin_family = AF_INET;
    sconnaddr.sin_addr.s_addr = htonl(imp->msun.svcreq->server_ip);
    if (GetAppParam("NCBI", "NET_SERV", "PROXY_SERV_OVERRIDE", "", buf, sizeof buf) > 0)
    {
        sconnaddr.sin_addr.s_addr = inet_addr(buf);
    }
    sconnaddr.sin_port = htons(imp->msun.svcreq->server_port);
            
RETRY1:


#ifndef NETP_INET_NEWT
    status = NI_CONNECT(sconnhp->sok, (struct sockaddr PNTR) &sconnaddr, 
                        sizeof(sconnaddr));
#else      
    status = NI_CONNECT(sconnhp->sok, &sconnaddr, sizeof(sconnaddr));
#endif
    if (status < 0) {  
#ifdef NETP_INET_NEWT
        SOCK_ERRNO = ABS(status);
#endif
        switch (SOCK_ERRNO) {
        case EINTR:
            goto RETRY1;
          
#ifdef NETP_INET_PCNFS
        case 0:
#endif /* NETP_INET_PCNFS */
        case EWOULDBLOCK:
        case EINPROGRESS:
          /* if the connect()ion is not established immediately, a         */
          /* select() can be performed where the corresponding "write"     */
          /* file descriptor will be enabled once the connect()ion has been*/
          /* established                                                   */

            if (sokselectw(sconnhp->sok, timeout) != 0) {
                MsgDestroyHandle(sconnhp);  
                return NULL;
            }
            break;

        default:
            MsgDestroyHandle(sconnhp);
            ni_errno = NIE_DIRUNCONNECT;  /* cannot establish direct connection */
            return NULL;
        }
    }

    /* Now we are sending service request directly to the server */

    if (MsgWrite(sconnhp, imp, TRUE) < 0) {
        MsgDestroyHandle(sconnhp);
        ni_errno = NIE_DIRUNCONNECT;
        return NULL;
    }
    return sconnhp;
}



/*
 * Purpose:     Disconnect from a service provider
 *
 * Parameters:
 *   mhp          Message handle for the server
 *
 * Returns:
 *               0, always
 *
 *
 * Description:
 *              Disconnect from a service provider, essentially by just
 *              closing the communication socket to that service provider.
 */

NLM_EXTERN Int2 NI_ServiceDisconnect(NI_HandPtr mhp)
{
    if (activityHook != NULL)
    {
        activityHook(mhp, NetServHook_svcdisconn, 0);
    }

    MsgDestroyHandle(mhp);
    return 0;
} /* NI_ServiceDisconnect */



/*
 * Purpose:     Obtain the read file descriptor from a "message handle"
 *
 * Parameters:
 *   handp        Message handle
 *
 * Returns:
 *               Socket associated with message handle
 *
 *
 * Description:
 *              Get the read file desciptor from a message handle. This
 *              might be useful, for example, when wishing to perform
 *              "direct" I/O to the socket after a connection has been
 *              established with a server/client.
 */

NLM_EXTERN int NI_ServiceGetReadFd(NI_HandPtr handp)
{
    return handp->sok;
} /* NI_ServiceGetReadFd */



/*
 * Purpose:     Obtain the write file descriptor from a "message handle"
 *
 * Parameters:
 *   handp        Message handle
 *
 * Returns:
 *               Socket associated with message handle
 *
 *
 * Description:
 *              Get the write file desciptor from a message handle. This
 *              might be useful, for example, when wishing to perform
 *              "direct" I/O to the socket after a connection has been
 *              established with a server/client.
 */

NLM_EXTERN int NI_ServiceGetWriteFd(NI_HandPtr handp)
{
    return handp->sok;
} /* NI_ServiceGetWriteFd */



/*
 * Purpose:     Populate a service request with a service name and version #s
 *
 * Parameters:
 *   req          Service request
 *   name         Service name
 *   vermin       Minimum version number for this service
 *   vermax       Maximum version number for this service
 *
 * Returns:
 *               -1, if the name is a NULL pointer
 *               0, otherwise
 *
 *
 * Description:
 *              Populate the service request with the specified service name
 *              and version numbers, dynamically allocating space for the
 *              service name.
 */

NLM_EXTERN Int2 NI_RequestSetService(NI_ReqPtr req, CharPtr name, Uint2 vermin, Uint2 vermax)
{
    if (name == NULL) {
        ni_errno = NIE_INVAL;
        return -1;
    }
    req->service->name = StringSave(name);
    req->service->minVersion = vermin;
    req->service->maxVersion = vermax;
    req->service->typeL = NULL;
    return 0;
} /* NI_RequestSetService */



/*
 * Purpose:     Populate a service request with an additional resource
 *
 * Parameters:
 *   req          Service request
 *   name         Resource name
 *   type         Service type
 *   vermin       Minimum version number for this resource
 *   vermax       Maximum version number for this resource
 *
 * Returns:
 *               -1, if the name is a NULL pointer
 *               0, otherwise
 *
 *
 * Description:
 *              Insert the information for this resource into a list of
 *              resources associated with this service request. This
 *              function may be called one or more times (or, not at all) to
 *              populate a service request with one or more resources.
 */

NLM_EXTERN Int2 NI_RequestAddResource(NI_ReqPtr req, CharPtr name, CharPtr type, Uint2 vermin, Uint2 vermax)

{
    NIResPtr    resp;

    if (name == NULL) {
        ni_errno = NIE_INVAL;
        return -1;
    }
    resp = NI_MakeResource();
    resp->name = StringSave(name);
    if (type != NULL)
        resp->type = StringSave(type);
    resp->minVersion = vermin;
    resp->maxVersion = vermax;
    req->resourceL = ListInsertPrev((VoidPtr) resp, req->resourceL);    /* add to end of list */
    return 0;
} /* NI_RequestAddResource */



/* THESE FUNCTIONS NOT VISIBLE TO API USER */

/*
 * Purpose:     Partially halt Network Services
 *
 * Parameters:
 *   disp         A pointer to the dispatcher structure
 *
 * Description:
 *              Halt network services, except refrain from freeing the
 *              parameters which are set by NI_SetDispatcher().
 */

static void
HaltServices (NI_DispatcherPtr disp)
{
    if (disp == NULL)
        return;

    if (disp->referenceCount > 0)
        return;

    if (activityHook != NULL)
    {
        activityHook((NI_HandPtr) disp, NetServHook_dispdisconn, 0);
    }

    MsgDestroyHandle(disp->dispHP);
    MsgDestroyHandle(disp->svcsHP);
    NI_DestroyRequest(disp->reqResponse);
    if (disp->identity != NULL) {
        MemFree (disp->identity->username);
        MemFree (disp->identity->group);
        MemFree (disp->identity->domain);
        MemFree (disp->identity);
        disp->identity = NULL;
    }
    disp->dispHP = NULL;
    disp->svcsHP = NULL;
    disp->reqResponse = NULL;
    if (disp->encryptInfo != NULL)
    {
        if (disp->encryptInfo != NULL)
            NI_DestroyPubKey((NIPubKeyPtr) disp->encryptInfo->data.ptrvalue);
        ValNodeFree(disp->encryptInfo);
    }

#ifdef NETP_INET_WSOCK
    /* we have an obligation to perform one cleanup call for every Startup */
    while (wsaStartupCount-- > 0)
    {
        WSACleanup();
    }
#endif
}


/*
 * Purpose:     Lookup a port # in config file and possible NIS
 *
 * Parameters:
 *   service      Name of config. file entry
 *   networkOrder Boolean, indicates whether value should be returned in host
 *                order or network order.
 *
 * Description:
 *              Look up the specified entry in the NCBI config. file, and
 *              lookup in NIS the name obtained from the config file if it's
 *              non-numeric.
 *
 * Note:
 *              The intent of this function is that, in most cases, the
 *              GetAppParam() entry will not be present, and a default value
 *              will be used instead.  The getservbyname() call is intended
 *              to be a last resort, because this may be slow on some systems.
 */

static Uint2
GetByConfigOrServ(CharPtr service, Boolean networkOrder)
{
    struct servent PNTR portEntry;
    Char                buf[50];
    Uint2               port;

    if (GetAppParam("NCBI", "NET_SERV", service, "", buf, sizeof buf) <= 0)
    {
        port = 0;
    } else {
        if (StrSpn(buf, "0123456789") == StrLen(buf))
        { /* all numeric */
            port = atoi(buf);
            if (networkOrder)
               port = htons(port);
        } else {
            /* entry from configuration file is name to use in getservbyname */
            if ((portEntry = getservbyname(buf, "tcp")) == NULL)
            {
                port = 0;
            } else  {
                port = portEntry->s_port;
                if (! networkOrder)
                    port = ntohs(port);
            }
        }
    }

    return port;
}


/*
 * Purpose:     Connect to the dispatcher
 *
 * Parameters:
 *   disp         A pointer to the dispatcher structure
 *   host         Name of the host on which dispatcher resides
 *   service      Name of the "service" (i.e., port) to which we should connect
 *   timeout      How long to wait for dispatcher to respond, 0 ==> use default
 *
 * Returns:
 *               NULL, if the attempt to connect failed
 *               a pointer to the "Msg" structure for the dispatcher, otherwise
 *
 *
 * Description:
 *               Connect to the dispatcher on the specified hostname on the
 *               specified service (where a service maps to a port number).
 *               This is done by establishing a socket to the dispatcher,
 *               and then connect()ing to that socket; the dispatcher should
 *               be listen()ing on that socket, and should subsequently accept()
 *               the connection request.
 *
 *               While doing this, also obtain other useful information;
 *               namely, the dotted IP address of the local host, and the
 *               high and low port numbers to be used when attempting
 *               dispatcher connections. This global information is used
 *               elsewhere.
 */

#ifndef INADDR_NONE
#define INADDR_NONE             -1
#endif /* INADDR_NONE */

static NI_HandPtr
DispatchConnect(NI_DispatcherPtr disp, CharPtr host, CharPtr service, int timeout)
{
    struct hostent      PNTR dispHost, PNTR localHost;
    struct sockaddr_in  serv_addr;
    NI_HandPtr          dHP;
    Uint2               disp_port;
    Uint4               srvadd;
    Char                servInetAddr[INETADDR_SIZ], localHostName[SVC_HOST_SIZ];
    Char                t_service[64];
    int                 status;
    Int4                connectStartTime;

    if (disp == NULL)
        return NULL;


    serv_addr.sin_family = AF_INET;

    srvadd = inet_addr(host);
    if ((Int4)srvadd != INADDR_NONE)    /* malformed request */
        MemCopy((VoidPtr) &serv_addr.sin_addr, (VoidPtr) &srvadd, sizeof(srvadd));
    else {
        if ((dispHost = gethostbyname(host)) == NULL) {
            ni_errno = NIE_NOHOSTENT;
            return NULL;
        }
/*      MemCopy((VoidPtr)&serv_addr.sin_addr, (VoidPtr)(dispHost->h_addr), dispHost->h_length);*/
        MemCopy(&serv_addr.sin_addr, dispHost->h_addr, dispHost->h_length);
    }
    StringCpy(servInetAddr, inet_ntoa(serv_addr.SIN_ADDR));

    if ((disp_port = GetByConfigOrServ(service, TRUE)) == 0)
    {
        if (service)
            StringCpy(t_service, service);      /* because Windows barfs on the pointer */
        else
            t_service[0] = 0;
        if ((disp_port = htons(atoi(t_service))) == 0)
            disp_port = htons(NI_DFLT_SVC_PORT);
    }
    if (ntohs(disp_port) <= NI_LAST_RESERVED_PORT) {
        ni_errno = NIE_NOSERVENT;
        return NULL;
    }

    /* get the Internet address of the "local host" */
#ifdef NETP_INET_MACTCP
    /* simpler solution to avoid the hazards of gethostname() */
    {
        unsigned long localHostId;

        localHostId = gethostid();
        StringCpy(disp->localHostAddr, inet_ntoa(* (H_ADDR_TYPE) &localHostId));
    }
#else
    gethostname(localHostName, SVC_HOST_SIZ);
    if ((localHost = gethostbyname(localHostName)) == NULL) {
        /* GetAppParam() workaround for PC-NFS 5.0 bug */
        if (GetAppParam("NCBI", "NET_SERV", "HOST_ADDRESS", "",
                        disp->localHostAddr, sizeof(disp->localHostAddr)) <= 0)
        { /* use a bogus address which the dispatcher will try to fix */
            StringCpy(disp->localHostAddr, "0.0.0.0");

        }
    } else {
        StringCpy(disp->localHostAddr, inet_ntoa(* (H_ADDR_TYPE) localHost->h_addr));
    }
#endif /* NETP_INET_MACTCP */

    if ((disp->loport = GetByConfigOrServ(NI_CLIENT_PORT_LO_NAME, FALSE)) == 0)
    {
        if ((disp->loport = atoi(NI_CLIENT_PORT_LO_NAME)) == 0)
            disp->loport = NI_DFLT_CLILO_PORT;
    }
    if (disp->loport <= NI_LAST_RESERVED_PORT) {
        ni_errno = NIE_BADPORT;         /* bad low client port */
        return NULL;
    }

    if ((disp->hiport = GetByConfigOrServ(NI_CLIENT_PORT_HI_NAME, FALSE)) == 0)
    {
        if ((disp->hiport = atoi(NI_CLIENT_PORT_HI_NAME)) == 0)
            disp->hiport = NI_DFLT_CLIHI_PORT;
    }
    if (disp->hiport <= NI_LAST_RESERVED_PORT) {
        ni_errno = NIE_BADPORT;         /* bad high client port */
        return NULL;
    }

    MemFill((VoidPtr) &serv_addr, '\0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(servInetAddr);
    serv_addr.sin_port = disp_port;

    if ((dHP = MsgMakeHandle(TRUE)) == NULL)
        return NULL;
    MsgSetLJError(dHP);
    if (timeout > 0)
        MsgSetReadTimeout(dHP, timeout);

    if (activityHook != NULL)
    {
        activityHook((NI_HandPtr) disp, NetServHook_dispconn, 0);
    }

#ifdef NETP_SOCKS
    if (disp->useSocks)
    { /* SOCKS can't deal well with blocking connections */
        NI_SETBLOCKING(dHP->sok);
    }
#endif

    connectStartTime = Nlm_GetSecs();

  RETRY:
#ifndef NETP_INET_NEWT
    if ((status = NI_CONNECT(dHP->sok, (struct sockaddr PNTR) &serv_addr, sizeof(serv_addr))) < 0) { /* } */
#else
    if ((status = NI_CONNECT(dHP->sok, &serv_addr, sizeof(serv_addr))) < 0) {
        SOCK_ERRNO = ABS(status);
#endif
        switch (SOCK_ERRNO) {
          case EINTR:
            goto RETRY;

#ifdef NETP_INET_PCNFS
          /* This is apparently a bug in PC-NFS 4.0 ... a connection attempt */
          /* on a non-blocking socket yields errno == 0                      */
          case 0:
#endif /* NETP_INET_PCNFS */
          case EWOULDBLOCK:
          case EINPROGRESS:
            /* if the connect()ion is not established immediately, a         */
            /* select() can be performed where the corresponding "write"     */
            /* file descriptor will be enabled once the connect()ion has been*/
            /* established                                                   */
            status = sizeof(serv_addr);
            if (sokselectw(dHP->sok, timeout) == 0
#ifdef OS_UNIX
                && getpeername(dHP->sok,(struct sockaddr *) &serv_addr, &status) == 0
#endif
                ) {
                dHP->state = NI_CONNECTED;
                dHP->connectDelay = Nlm_GetSecs() - connectStartTime;
                return dHP;
            }
            break;

          default:
            break;
        }
        MsgDestroyHandle(dHP);
        ni_errno = NIE_DISPCONN;        /* can't connect to dispatcher */
        return NULL;
    }
    dHP->state = NI_CONNECTED;
    dHP->connectDelay = Nlm_GetSecs() - connectStartTime;
    return dHP;
} /* DispatchConnect */


/*
 * Purpose:     Convert an FQDN to an IP address
 *
 * Parameters:
 *   fqdn         A fully-qualified domain name, like "dispatch1.nlm.nih.gov"
 *   ipbuf        An output buffer for the dotted-decimal IP address
 *   ipbuflen     The length of ipbuf
 *
 * Returns:
 *               TRUE, if the address was resolved successfully, FALSE otherwise
 */

NLM_EXTERN Boolean NI_FqdnToIpaddr(CharPtr fqdn, CharPtr ipbuf, Int2 ipbuflen)
{
    struct hostent PNTR dispHost;
    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    if ((dispHost = gethostbyname(fqdn)) != NULL)
    {
         MemCopy (&serv_addr.sin_addr, dispHost->h_addr, dispHost->h_length);
         StringNCpy (ipbuf, inet_ntoa (serv_addr.SIN_ADDR), ipbuflen-1);
         return TRUE;
    } else {
         return FALSE;
    }
}




/*
 * Purpose:     Get the platform on which this client is running
 *
 * Parameters:
 *                none
 *
 * Returns:
 *               the client's platform, or NI_PLATFORM_UNKNOWN
 *
 *
 * Description:
 *               Calculate what platform this client is running on.
 *
 *
 * Note:
 *               Although the initial implementation of this function
 *               calculates the platform-type at compile-time, it is
 *               legitimate to perform some computation at run time, e.g.,
 *               to determine whether this client is using a particular
 *               low-level driver.
 *
 *               The dispatcher and servers should not rely on the
 *               information which is received for platform-type, because
 *               the client may be lying, either because of a coding error
 *               or malice on the part of a client developer.
 */

NLM_EXTERN Int2 NI_GetPlatform (void)
{
    static Boolean alreadyInited = FALSE;
    static Int2 retval;

    if (alreadyInited)
    {
      return retval;
    }

    alreadyInited = TRUE;

    retval = NI_PLATFORM_UNKNOWN;

#ifdef NETP_INET_MACTCP
    retval = NI_PLATFORM_MAC;
#endif

#ifdef OS_VMS
#ifdef NETP_INET_TGV
    retval = NI_PLATFORM_VMS_TGV;
#endif
#ifdef NETP_INET_TWG
    retval = NI_PLATFORM_VMS_TWG;
#endif
#ifdef NETP_INET_WPW
    retval = NI_PLATFORM_VMS_WPW;
#endif
#ifdef NETP_INET_UCX
    retval = NI_PLATFORM_VMS_UCX;
#endif
#ifdef OS_AXP_VMS
    retval = NI_PLATFORM_AXP_OPENVMS;
#endif
#endif /* OS_VMS */

#ifdef OS_UNIX
    retval = NI_PLATFORM_GENERIC_UNIX;
#ifdef PROC_IBM370
    retval = NI_PLATFORM_IBM370AIX;
#endif
#ifdef OS_UNIX_SUN
    retval = NI_PLATFORM_SUN;
#endif
#if defined(OS_UNIX_OSF1) && defined(PROC_ALPHA)
    retval = NI_PLATFORM_ALPHA_OSF1;
#endif
#ifdef COMP_AUX
    retval = NI_PLATFORM_AUX;
#endif
#if defined(COMP_CRAY) && defined(PROC_YMP)
    retval = NI_PLATFORM_CRAY;
#endif
#ifdef PROC_CONVEX
    retval = NI_PLATFORM_CONVEX;
#endif
#ifdef PROC_HPPA
    retval = NI_PLATFORM_HPUX;
#endif
#ifdef OS_UNIX_NEXT
    retval = NI_PLATFORM_NEXT;
#endif
#ifdef PROC_MIPS
    retval = NI_PLATFORM_SGI;
#endif
#ifdef OS_UNIX_ULTRIX
    retval = NI_PLATFORM_ULTRIX;
#endif
#if defined(OS_UNIX_SYSV) && defined(PROC_SPARC)
    retval = NI_PLATFORM_SYSV_ON_SPARC;
#endif
#ifdef OS_UNIX_AIX
    retval = NI_PLATFORM_AIX;
#endif
#ifdef OS_UNIX_LINUX
#ifdef PROC_ALPHA
    retval = NI_PLATFORM_LINUX_ALPHA;
#else
    retval = NI_PLATFORM_LINUX;
#endif
#endif
#ifdef OS_UNIX_NETBSD
	retval = NI_PLATFORM_NETBSD;
#endif
#ifdef OS_UNIX_FREEBSD
	retval = NI_PLATFORM_FREEBSD;
#endif
#endif /* OS_UNIX */

#ifdef OS_DOS
    retval = NI_PLATFORM_DOS;
#ifdef WIN16
    retval = NI_PLATFORM_WIN16;
#endif
#ifdef NETP_INET_NEWT
    retval = NI_PLATFORM_WIN_NEWT;
#endif
#ifdef NETP_INET_PCNFS
    retval = NI_PLATFORM_WIN_PCNFS;
#endif
#ifdef WINSOCK
    retval = NI_PLATFORM_WIN_WINSOCK;
#endif
#endif /* OS_DOS */

#ifdef OS_WINNT
    retval = NI_PLATFORM_WINNT;
#endif

    return retval;
}


/*
 * Purpose:     Set the "identity" of this client
 *
 * Parameters:
 *   disp         A pointer to the dispatcher structure
 *   user         New Username
 *   group        New Groupname
 *   domain       New DomainName
 *
 * Returns:
 *               0, always
 *
 *
 * Description:
 *               Allocate the space for the "UID" structure, if not already
 *               allocated, and populate it with the user name, group name,
 *               and domain name.
 */

static Int2
SetIdentity(NI_DispatcherPtr disp, CharPtr user, CharPtr group, CharPtr domain)
{
    if (disp == NULL)
        return 0;

    if (disp->identity == NULL)
        disp->identity = NI_MakeUid();

    if (disp->identity->username != NULL)
        MemFree(disp->identity->username);
    disp->identity->username = StringSave(user);
    if (disp->identity->group != NULL)
        MemFree(disp->identity->group);
    if (group != NULL)
        disp->identity->group = StringSave(group);
    else
        disp->identity->group = NULL;
    if (disp->identity->domain != NULL)
        MemFree(disp->identity->domain);
    disp->identity->domain = StringSave(domain);
    return 0;
} /* SetIdentity */



/*
 * Purpose:     Copy from the "identity" UID to the specified UID data struct
 *
 * Parameters:
 *   disp         A pointer to the dispatcher structure
 *   uid          UID structure to be copied into
 *
 * Returns:
 *               -1, if invalid arguments
 *               0, otherwise
 *
 *
 * Description:
 *              Copy fields from the "identity" UID data structure into the
 *              UID data structure provided by the caller.
 */

static Int2
CopyIdentity(NI_DispatcherPtr disp, NI_UidPtr uid)
{
    if (disp == NULL || disp->identity == NULL || uid == NULL)
        return -1;
    if  (uid->username != NULL)
        MemFree(uid->username);
    uid->username = StringSave(disp->identity->username);
    if  (uid->group != NULL)
        MemFree(uid->group);
    uid->group = StringSave(disp->identity->group);
    if  (uid->domain != NULL)
        MemFree(uid->domain);
    uid->domain = StringSave(disp->identity->domain);
    return 0;
} /* CopyIdentity */



/*
 * Purpose:     Select the next available port within the given range,
 *              and bind a socket to it.
 *
 * Parameters:
 *   sok          Socket to be bound to a port (INPUT)
 *   sokadr       Socket data structure to be populated (OUTPUT)
 *   loport       Minimum acceptable port number
 *   hiport       Maximum acceptable port number
 *
 * Returns:
 *               0, if unable to bind to a port
 *               the selected ("bound") port number, otherwise
 *
 *
 * Description:
 *              Iterate through the range of acceptable port numbers, until
 *              an unused port number can be selected to which the socket
 *              can be bound.
 */

static Uint2
bindPort(int sok, struct sockaddr_in PNTR sokadr, Int2 loport, Int2 hiport, Uint4 remoteHost)
{
    int                 status;
#ifdef NETP_INET_MACTCP
    int                 delta = 0;
    Char                buf[20];
#endif

    if (hiport == 0)
        hiport = loport;
    if (loport > hiport)
        return 0;

#ifdef NETP_INET_MACTCP
    /* use a hint from the configuration file to avoid port # conflicts */
    if (hiport > loport && GetAppParam("NCBI", "NET_SERV", "PORT_DELTA", "0",
        buf, sizeof buf) > 0)
    {
        delta = atoi(buf);
        loport += delta % (hiport - loport);
        sprintf (buf, "%d", delta + 1);
        SetAppParam("NCBI", "NET_SERV", "PORT_DELTA", buf);
    }
#endif

    MemFill((VoidPtr) sokadr, '\0', sizeof(struct sockaddr_in));
    sokadr->sin_family = AF_INET;
    sokadr->sin_addr.s_addr = INADDR_ANY;

    while (loport <= hiport) {
        sokadr->sin_port = htons(loport);
#ifdef NETP_INET_NEWT
        if ((status = NI_BIND(sok, sokadr, sizeof(struct sockaddr_in), htonl(remoteHost))) == 0)
#else
        if ((status = NI_BIND(sok, (struct sockaddr PNTR) sokadr, sizeof(struct sockaddr_in), htonl(remoteHost))) == 0)
#endif /* NETP_INET_NEWT */
            return (Uint2) ntohs(sokadr->sin_port);
        else {
#ifdef NETP_INET_NEWT
            SOCK_ERRNO = ABS(status);
#endif
            loport++;
        }
    }
    return 0;
} /* bindPort */



/* SERVER FUNCTIONS */

static int      writepipe PROTO((int fd, char *buf, int len));

/*
 * Purpose:     Write a message on the pipe from a child server application
 *              process to its parent NCBID.
 *
 * Parameters:
 *   fd           Pipe file descriptor
 *   buf          Buffer to be written
 *   len          Length of buffer
 *
 * Returns:
 *                0, if unable to write because the pipe is full
 *                number of bytes written, otherwise
 *
 *
 * Description:
 *              Write the specified number of bytes to a pipe, and handle
 *              multiple write attempts if necessary, to handle the case where
 *              a write() may be interrupted by a signal.
 *
 * Note:
 *              This routine is only used by a child process after it has been
 *              forked and before it has been execed.
 */

static int
writepipe(int fd, char *buf, int len)
{
    int         byteswrit;

  WriteAgain:
    if ((byteswrit = write(fd, buf, len)) < 0) {
        switch (errno) {
          case EINTR:
            goto WriteAgain;

          case EWOULDBLOCK:
          default:
            return 0;
        }
    }

    return byteswrit;
} /* writepipe */

static Int2 StandAlonePort(void)
{
    CharPtr env;

#ifdef OS_UNIX
    if ((env = getenv("NI_STANDALONE_SERVER")) == NULL)
    {
        return 0;
    }

    return atoi(env);
#else
    return 0;
#endif
}


/*
 * Purpose:     Send an "ACK" from a child server application process to its
 *              parent NCBID.
 *
 * Returns:
 *                0, if the ACK was sent successfully
 *                -1, otherwise
 *
 *
 * Description:
 *              Write an "ACK" from a child server application process to its
 *              parent NCBID, on the pipe connecting the two processes.
 *
 * Note:
 *              This routine should be called by a child process after it has
 *              determined that it has started successfully. At most one
 *              of NI_ServerACK() and NI_ServerNACK() may be called.
 */

#define TEMP_BUF_SIZ    256

NLM_EXTERN int NI_ServerACK(void)
{
#if 0
    int         wstat;
    Char        temp_buf[TEMP_BUF_SIZ];
    Int2        port;

    if ((port = StandAlonePort()) == 0)
    { /* not stand-alone */
        sprintf(temp_buf, PIPE_MSG_FMT, NIE_SERVACK, "OK");
        if ((wstat = writepipe(STDPIPE, temp_buf, strlen(temp_buf))) <= 0) {
            ni_errno = NIE_PIPEIO;
            strcpy(ni_errtext, (wstat == 0) ? "EWOULDBLOCK" : strerror(errno));
            return -1;
        }
    } else { /* stand-alone */
#ifdef OS_UNIX
        /* non-UNIX platforms currently experience compilation errors */
        struct sockaddr_in  soktAddr;
        NI_HandPtr hp;
        int sok;
        int status;
        struct sockaddr_in sockaddr;
        int soktLen;
        CharPtr security;
        int one = 1; /* for SO_REUSEADDR */

        hp = MsgMakeHandle(TRUE);
        NI_SETBLOCKING(hp->sok);

        MemFill(&sockaddr, '\0', sizeof(struct sockaddr_in));
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_addr.s_addr = INADDR_ANY;
        sockaddr.sin_port = htons(port);

        if (setsockopt(hp->sok, SOL_SOCKET, SO_REUSEADDR, (char *) &one,
                       sizeof(one)) < 0)
        {
            Message (MSG_ERROR, "Unable to set socket re-usability, errno = %d", errno);
        }

#ifdef NETP_INET_NEWT
        if ((status = bind(hp->sok, &sockaddr, sizeof(struct sockaddr_in))) == 0)
#else
        if ((status = bind(hp->sok, (struct sockaddr PNTR) &sockaddr, sizeof(struct sockaddr_in))) != 0)
#endif /* NETP_INET_NEWT */
        { /* error */
            ErrPostEx(SEV_FATAL,0,0, 
                      "Bind failed on socket %d, status = %d, errno = %d",
                      hp->sok, status, errno);
            return -1;
        }

        NI_LISTEN(hp->sok, 1);
        close(0); /* so that accept() will return 0 */
        soktLen = sizeof(soktAddr);

        /* accept the connection */
        if ((sok = NI_ACCEPT(hp->sok, (struct sockaddr *) &soktAddr, &soktLen)) < 0)
        { /* error */
            ErrPostEx(SEV_FATAL,0,0, "Accept returned bad file descriptor %d, errno = %d",
                    sok, errno);
            return -1;
        }
        LOG_SOCKET(sok, TRUE);
        NI_SETNONBLOCKING(sok);
        MsgDestroyHandle(hp);
        if ((security = getenv("NI_STANDALONE_SECURITY")) != NULL)
        { /* security must be substring of client address */
            if (StrNCmp(inet_ntoa(soktAddr.SIN_ADDR), security, StrLen(security)) != 0)
            {
                close(sok);
                ErrPostEx(SEV_FATAL,0,0, "Security violation from IP address %s, security = %s\n",
                        inet_ntoa(soktAddr.SIN_ADDR), security);
                return -1;
            }
        }
#endif /* OS_UNIX */
    }
#endif /* 0 */

    return 0;
} /* NI_ServerACK */



/*
 * Purpose:     Send an "NACK" from a child server application process to its
 *              parent NCBID.
 *
 * Returns:
 *                0, if the NACK was sent successfully
 *                -1, otherwise
 *
 *
 * Description:
 *              Write an "NACK" from a child server application process to its
 *              parent NCBID, on the pipe connecting the two processes.
 *
 * Note:
 *              This routine should be called by a child process after it has
 *              determined that it will be unable to start successfully. In
 *              the event that this routine is not called (or is unable to
 *              perform its function), a timeout mechanism must be relied
 *              upon for the NCBID to realize that a child has started
 *              unsuccessfully.
 *
 *              At most one of NI_ServerACK() and NI_ServerNACK() may be called.
 */

NLM_EXTERN int NI_ServerNACK(CharPtr err_text)
{
#if 0
    int         wstat;
    Char        temp_buf[TEMP_BUF_SIZ];

    sprintf(temp_buf, PIPE_MSG_FMT, NIE_SERVNACK, err_text);
    if (StandAlonePort() == 0)
    { /* not stand-alone */
        if ((wstat = writepipe(STDPIPE, temp_buf, strlen(temp_buf))) <= 0) {
            ni_errno = NIE_PIPEIO;
            strcpy(ni_errtext, (wstat == 0) ? "EWOULDBLOCK" : strerror(errno));
            return -1;
        }
    } else { /* stand-alone */
        ErrPostEx(SEV_FATAL,0,0, "Stand-alone server failed startup {%s}", temp_buf);
        return -1;
    }
#endif /* 0 */

    return 0;
} /* NI_ServerNACK */



/*
 * Purpose:     Open the stream to be used for ASN I/O between a server
 *              application process and its client.
 *
 * Returns:
 *                NULL, if something went wrong
 *                a pointer to the Msg structure, otherwise
 *
 *
 * Description:
 *              Create a "Msg" structure for ASN I/O, and associate the Msg's
 *              socket with the standard input file descriptor (STDIN), which is
 *              the communication socket between the server application process
 *              and its client.
 *
 * Note:
 *              This routine should only be called by a child application
 *              process (not by a client).
 */

NLM_EXTERN NI_HandPtr NI_OpenASNIO(void)
{
    NI_HandPtr  hp;

    if ((hp = MsgMakeHandle(FALSE)) == NULL)
        return NULL;

    MsgSetReadTimeout(hp, NI_SERV_LISTEN_TIMEOUT);      /* set default for servers to listen */

    if ((hp->sok = dup(STDIN)) == -1) {         /* STDOUT points to same socket */
        MsgDestroyHandle(hp);
        return NULL;
    }
    LOG_SOCKET(hp->sok, TRUE);
    {
        CharPtr buf;
        Char key[8];

        if ((buf = getenv("NI_DESKEY")) != NULL &&
            (AsnTypeStringToHex(buf, StrLen(buf), key, NULL) == 0))
        {
            NI_SetupDESEncryption(hp, (UcharPtr) key);
        }
    }
    return hp;
} /* NI_OpenASNIO */



/*
 * Purpose:     Close the ASN stream between a server application process and
 *              its client.
 *
 * Returns:
 *                -1 if something went wrong
 *                0, otherwise
 *
 *
 * Description:
 *              Close the stream by closing the socket and deleting the
 *              associated data structures.
 *
 * Note:
 *              This routine should only be called by a child application
 *              process (not by a client).
 */

NLM_EXTERN Int2 NI_CloseASNIO(NI_HandPtr hp)
{
    return MsgDestroyHandle(hp);
} /* NI_CloseANSIO */



/* MISC FUNCTIONS */

/* sokselectr and sokselectw are not prototyped in ni_lib.h */

/*
 * Purpose:     Wait for a "read" socket to become ready to read, or for
 *              a timeout to occur.
 *
 * Returns:
 *                -1 if something went wrong
 *                0, otherwise
 *
 *
 * Description:
 *              Wait for the indicated "read" socket to be marked as
 *              "selected" by a socket() call.
 *
 * Note:
 *              This routine is presently unused.
 *
 *              The timeout mechanism is not exactly enforced, because
 *              received signals could result in a longer timeout period.
 */

int sokselectr(int fd)
{
    fd_set      rfds;
    int         ready;
    struct timeval      timeout;

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);
    timeout.tv_sec = NI_SELECT_TIMEOUT;
    timeout.tv_usec = 0;
    while ((ready = select(fd+1, &rfds, NULL, NULL, &timeout)) == -1) {
        switch (SOCK_ERRNO) {
          case EINTR:
            continue;

          default:
            ni_errno = NIE_MISC;
            sprintf(ni_errtext, "%s", strerror(SOCK_INDEX_ERRNO));
            return -1;
        }
    }
    if (ready == 0) {
        strcpy(ni_errtext, ni_errlist[ni_errno]);
        ni_errno = NIE_TIMEOUT;
        return -1;
    }
    if (FD_ISSET(fd, &rfds))
        return 0;
    else
        return -1;
} /* sokselectr */



/*
 * Purpose:     Wait for a "write" socket to become ready to write, or for
 *              a timeout to occur.
 *
 * Returns:
 *                -1 if something went wrong
 *                0, otherwise
 *
 *
 * Description:
 *              Wait for the indicated "write" socket to be marked as
 *              "selected" by a socket() call.
 *
 * Note:
 *              This routine can be used when waiting for a connect() to go
 *              through successfully.
 *
 *              The timeout mechanism is not exactly enforced, because
 *              received signals could result in a longer timeout period.
 */

int sokselectw(int fd, int seconds)
{
    fd_set      wfds;
    int         ready;
    struct timeval      timeout;

    FD_ZERO(&wfds);
    FD_SET(fd, &wfds);
    timeout.tv_sec = NI_SELECT_TIMEOUT;
    if (seconds > 0) /* override default */
        timeout.tv_sec = seconds;
    timeout.tv_usec = 0;
    while ((ready = select(fd+1, NULL, &wfds, NULL, &timeout)) == -1) {
        switch (SOCK_ERRNO) {
          case EINTR:
            continue;

          default:
            ni_errno = NIE_MISC;
            sprintf(ni_errtext, "%s", strerror(SOCK_INDEX_ERRNO));
            return -1;
        }
    }
    if (ready == 0) {
        strcpy(ni_errtext, ni_errlist[ni_errno]);
        ni_errno = NIE_TIMEOUT;
        return -1;
    }
    if (FD_ISSET(fd, &wfds))
    {
#ifdef OS_UNIX
        int err;
        int optlen;

        optlen = sizeof(int);
        if (getsockopt(fd, SOL_SOCKET, SO_ERROR, (char *) &err, &optlen) >= 0 &&
            err != 0) /* check for an error */
            return -1; /* got some error */
#endif /* OS_UNIX */
        return 0;
    }
    else
        return -1;
} /* sokselectw */



/*
 * Purpose:     Parse the error number from an ASN error string which was
 *              formatted at a low level.
 *
 * Returns:
 *                -1, if unable to parse the string
 *                the parsed error number, otherwise
 *
 *
 * Description:
 *              Parse the error number from an ASN error string which was
 *              prepared by the ASN tools, and formatted at a low level.
 *
 * Note:
 *              The parsing mechanism is dependent upon any future format
 *              changes which may occur in the ASN tools.
 */

int getAsnError(char *str)
{
    int         errnum;

    if (sscanf(str, "%*s %*s %*s [-%d]", &errnum) < 1)
        errnum = -1;
    return errnum;
} /* getAsnError */


/*
 * Purpose:     Set the Connection ID file pointer
 *
 * Parameters:
 *   fp           The new value for the file descriptor
 *
 *
 * Description:
 *              Set the Connection ID file pointer. This is used to update
 *              the connection ID each time it is updated, to keep the
 *              value current.
 *
 * Note:
 *              In reality, this should only be called by the dispatcher.
 */

void SetConFilePtr (FILE *fp)
{
    conid_fp = fp;
}


/*
 * Purpose:     Update the connection ID file
 *
 * Parameters:
 *   conid        The new connection ID value
 *
 *
 * Description:
 *              Update the connection ID file, and be sure to flush the stream,
 *              to try to ensure that output really occurs.
 *
 * Note:
 *              Should be called every time the "next" connection ID is
 *              modified.
 */

NLM_EXTERN void WriteConFile (Uint4 conid)
{
    if (conid_fp != NULL) {
        (void) fseek(conid_fp, 0L, SEEK_SET);
        (void) FileWrite((CharPtr) &conid, 1, sizeof(conid), conid_fp);
        (void) fflush (conid_fp);
    }
}


/*
 * Purpose:     Close the connection ID file
 *
 *
 * Description:
 *              Close the connection ID file.
 *              to try to ensure that output really occurs.
 *
 * Note:
 *              In reality, this should only be called by the dispatcher.
 */

void CloseConFile (void)
{
    if (conid_fp != NULL) {
        fclose (conid_fp);
        conid_fp = NULL;
    }
}



/*
 * Purpose:     Check for expired timers
 *
 *
 * Description:
 *              For every expired timer, call the specified timer
 *              callback function, which is in turn responsible for cancelling
 *              the timer.
 *
 * Note:
 *              Timer checks only take place when this function is called.
 *              Therefore, it is the responsibility of an application to
 *              intermitently call this function.  This could be done, e.g.
 *              using the UNIX alarm clock mechanism, or inside of an event
 *              loop.
 *
 *              The order of operations is significant here, because the
 *              hook function must cancel the timer.  To perform the linked
 *              list traversal in a less careful manner could result in
 *              illegal memory accesses.
 *
 *              The timer list in managed in a very unsophisticated manner;
 *              if lots of timers were anticipated, the list would be
 *              maintained sorted by time, and all of the timer functions
 *              would need to maintain and traverse the timer list based
 *              upon this criterion.
 *
 *              A count is used as a failsafe mechanism against infinite loops.
 */

#define NI_MAX_TIMERS 1000

NLM_EXTERN void NI_ProcessTimers(void)
{
    NodePtr t;
    NodePtr tnew;
    NI_TimerPtr timer;
    NodePtr timersToBeFired = NULL;
    time_t curtime;
    int count = NI_MAX_TIMERS;

    if ((t = timerHead) == NULL)
    {
        return;
    }

    curtime = GetSecs();

    do {
        timer = (NI_TimerPtr) t->elem;
        tnew = ListGetNext(t);
        if (timer != NULL && timer->timeout != NULL_TIMER &&
            timer->timeout <= curtime)
        { /* note the timer to be fired */
            timersToBeFired = ListInsert(timer, timersToBeFired);
        }
        if (t == tnew)
        { /* data structure error, time to bail out */
            break;
        }
        t = tnew;
    } while (t != timerHead && t != NULL && --count > 0);

    if ((t = timersToBeFired) == NULL)
        return;

    count = NI_MAX_TIMERS;

    do {
        timer = (NI_TimerPtr) t->elem;
        tnew = ListGetNext(t);

        /* mark the timer so it won't fire again */
        timer->timeout = NULL_TIMER;
        if (timer->hook != NULL)
        {
            timer->hook(timer->hookParam);
        }
        t = tnew;
    } while (t != timersToBeFired && t != NULL && --count > 0);

    ListDelete(timersToBeFired);
}


/*
 * Purpose:     Return the time when the next timeout will occur
 *
 * Returns:     The time, in seconds, when the next scheduled timeout will
 *              occur, or NULL_TIMER, if there are no timers set.
 *
 * Description:
 *              Return the time when the next timer timeout will occur.
 *              This information is typically used with the select()
 *              system call, to ensure that a timeout parameter is passed
 *              to select() which is sufficiently short to ensure that
 *              the application will call NI_ProcessTimers() at an
 *              appropriate time.
 *
 * Note:
 *              The timer list in managed in a very unsophisticated manner;
 *              if lots of timers were anticipated, the list would be
 *              maintained sorted by time, and all of the timer functions
 *              would need to maintain and traverse the timer list based
 *              upon this criterion.
 */

NLM_EXTERN time_t NI_GetNextWakeup(void)
{
    time_t next_wakeup = NULL_TIMER;
    NodePtr t;
    NI_TimerPtr timer;

    NI_ProcessTimers();

    if ((t = timerHead) == NULL)
    {
        return NULL_TIMER;
    }

    do {
        t = ListGetNext(t);
        timer = (NI_TimerPtr) t->elem;
        if (next_wakeup == NULL_TIMER || (timer->timeout != NULL_TIMER &&
            timer->timeout < next_wakeup))
        {
            next_wakeup = timer->timeout;
        }
    } while (t != timerHead && t != NULL);

    return next_wakeup;
}


/*
 * Purpose:     Set a timer
 *
 * Parameters:
 *   timeout      The time in seconds when the timer should expire
 *   hook         Callback to be called when (if) the timer expires
 *   hookParam    Parameter to be passed to caller's hook when the timer expires
 *
 *
 * Returns:     The "timer ID", really a pointer to the timer data structure
 *
 *
 * Description:
 *              Sets a timer with the appropriate parameters.
 *
 * Note:
 *              The timer list in managed in a very unsophisticated manner;
 *              if lots of timers were anticipated, the list would be
 *              maintained sorted by time, and all of the timer functions
 *              would need to maintain and traverse the timer list based
 *              upon this criterion.
 *
 *              It is the responsibility of the application (usually the
 *              hook function) to cancel the timer.
 */

NodePtr
NI_SetTimer(time_t timeout, NI_TimeoutHook hook, Pointer hookParam)
{
    NodePtr t;
    NI_TimerPtr timer;

    timer = (NI_TimerPtr) MemNew(sizeof(NI_Timer));
    timer->timeout = timeout;
    timer->hook = hook;
    timer->hookParam = hookParam;
    t = ListInsert(timer, timerHead);
    timerHead = t;

    return t;
}


/*
 * Purpose:     Cancel a timer
 *
 * Parameters:
 *   timerID      The ID of the timer
 *
 *
 * Description:
 *              Cancel the specified timer by deleting the entry and its
 *              associated data structure.
 *
 * Note:
 *              The timer list in managed in a very unsophisticated manner;
 *              if lots of timers were anticipated, the list would be
 *              maintained sorted by time, and all of the timer functions
 *              would need to maintain and traverse the timer list based
 *              upon this criterion.
 */

NLM_EXTERN void NI_CancelTimer(NodePtr timerId)
{
    if (timerId != NULL)
    {
        MemFree (timerId->elem);
        timerHead = ListDelete(timerId);
    }
}


/*
 * Purpose:     Set an activity hook, to inform the application of key events
 *
 * Parameters:
 *   hook         The hook (callback function)
 *
 *
 * Description:
 *              Setup a hook function which will subsequently be used to
 *              inform the application of various events; these currently
 *              include:
 *                * Connection to dispatcher
 *                * Disconnection from dispatcher
 *                * Service connection
 *                * Service disconnection
 *                * Bytes written
 *                * Bytes read
 *
 * Note:
 *              This hook is global for the running application.
 */

NLM_EXTERN void NI_SetActivityHook (NI_NetServHook hook)
{
    activityHook = hook;
}


/*
 * Purpose:     Return the current activity hook
 *
 *
 * Description:
 *              Return the current activity hook.  This is only intended
 *              to be used internally by the Network Services library.
 *              This function is used to avoid making activityHook into a
 *              global variable.
 */

NLM_EXTERN NI_NetServHook NI_ActivityHook (void)
{
    return activityHook;
}


/*
 * Purpose:     Initialize socket management
 *
 * Description:
 *              If not already initialized, initialize the socket management
 *              data structures
 */

static void InitLogSocket(void)
{
    static Boolean inited = FALSE;

    if (! inited)
    {
        FD_ZERO(&openfds);
        inited = TRUE;
    }
}


/*
 * Purpose:     Count the number of open sockets
 */

NLM_EXTERN Int2 NI_SocketsOpen(void)
{
    int sok;
    int count = 0;

    InitLogSocket();
    for (sok = 0; sok < FD_SETSIZE; sok++)
    {
        if (FD_ISSET(sok, &openfds))
            count++;
    }
    return count;
}

/*
 * Purpose:     Log each socket transaction
 */

NLM_EXTERN void NI_LogSocket(int sok, Boolean opening, CharPtr filename, int lineno)
{
    int localsok;

    InitLogSocket();

    if (sok == INVALID_SOCKET || sok < 0 || sok >= FD_SETSIZE)
    {
#ifndef NETP_INET_WSOCK
        /* FD_SETSIZE doesn't accurately describe the socket range for
           WinSock applications, so don't generate misleading error msgs */
        ErrPostEx(SEV_WARNING,0,0, "Bad %s operation on socket %d at %s:%d",
                  opening ? "opening" : "closing", sok, filename, lineno);
#endif /* NETP_INET_WSOCK */
        return;
    }

    if (opening)
    {
        TRACE("Just opened socket %d at %s:%d\n", sok, filename, lineno);
        if (FD_ISSET(sok, &openfds))
        {
            ErrPostEx(SEV_ERROR,0,0, "Duplicate open of socket %d at %s:%d",
                      sok, filename, lineno);
        } else {
            FD_SET(sok, &openfds);
        }
    } else {
        TRACE("Trying to close socket %d at %s:%d\n", sok, filename, lineno);
        if (FD_ISSET(sok, &openfds))
        {
            FD_CLR(sok, &openfds);
        } else {
            ErrPostEx(SEV_ERROR,0,0, "Duplicate close of socket %d at %s:%d",
                      sok, filename, lineno);
        }
    }

#ifdef DEBUG
    for (localsok = 0; localsok < FD_SETSIZE; localsok++)
    {
        if (FD_ISSET(localsok, &openfds))
        {
            TRACE("Socket %d is currently open\n", localsok);
        }
    }
#endif /* DEBUG */
}

