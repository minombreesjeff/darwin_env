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
* File Name:    ni_error.h
*
* Author:       Beatty, Gish
*
* Version Creation Date:        1/1/92
*
* $Revision: 6.4 $
*
* File Description: 
*
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 5/12/92  Epstein     Converted tabs to spaces
*
* ==========================================================================
* $Log: ni_error.h,v $
* Revision 6.4  2000/10/30 18:12:01  beloslyu
* FreeBSD was added
*
* Revision 6.3  2000/07/08 20:44:05  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 6.2  1999/06/07 18:20:16  beloslyu
* NetBSD port
*
* Revision 6.1  1997/11/18 21:14:35  epstein
* identify Linux Alpha client
*
* Revision 6.0  1997/08/25 18:38:40  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/07/01 19:12:46  vakatov
* [WIN32]  DLL'd "netcli.lib"
*
* Revision 5.0  1996/05/28 14:11:55  ostell
* Set to revision 5.0
*
* Revision 4.1  1995/11/27  20:59:13  epstein
* add client support for direct-connection services
*/

#ifndef _NI_ERROR_
#define _NI_ERROR_

#include "ncbinet.h"

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define NI_Platform enum NI_platform
NI_Platform {
  NI_PLATFORM_UNKNOWN = 0,
  NI_PLATFORM_MAC,
  NI_PLATFORM_VMS_TGV,
  NI_PLATFORM_AXP_OPENVMS,
  NI_PLATFORM_GENERIC_UNIX,
  NI_PLATFORM_IBM370AIX,
  NI_PLATFORM_SUN,
  NI_PLATFORM_ALPHA_OSF1,
  NI_PLATFORM_AUX,
  NI_PLATFORM_CRAY,
  NI_PLATFORM_CONVEX,
  NI_PLATFORM_HPUX,
  NI_PLATFORM_NEXT,
  NI_PLATFORM_SGI,
  NI_PLATFORM_ULTRIX,
  NI_PLATFORM_DOS,
  NI_PLATFORM_WIN16,
  NI_PLATFORM_WIN_NEWT,
  NI_PLATFORM_WIN_PCNFS,
  NI_PLATFORM_WIN_WINSOCK,
  NI_PLATFORM_WINNT,
  NI_PLATFORM_SYSV_ON_SPARC,
  NI_PLATFORM_VMS_UCX,
  NI_PLATFORM_VMS_TWG,
  NI_PLATFORM_VMS_WPW,
  NI_PLATFORM_AIX,
  NI_PLATFORM_LINUX,
  NI_PLATFORM_LINUX_ALPHA,
  NI_PLATFORM_NETBSD,
  NI_PLATFORM_FREEBSD,

  NI_N_PLATFORMS
#define NI_MAX_PLATFORMS (NI_N_PLATFORMS - 1)
};

#define NI_ErrLevel enum ni_errlevel
NI_ErrLevel {
  NIL_INFORMATION = 1,   /* informational */
  NIL_WARNING,           /* small problem */
  NIL_SEVERE,            /* recoverable */
  NIL_FATAL              /* not recoverable */
};

#define NI_Error enum ni_error
NI_Error {
  NIE_NO_ERROR = 0,   /* no error */
  NIE_MISC,           /* error */
  NIE_NOUSERENT,      /* no entry for user */
  NIE_NOHOSTENT,      /* no entry for host */
  NIE_NOSERVENT,      /* no entry for service */
  NIE_BADHOST,        /* host address invalid */
  NIE_BADPORT,        /* port number invalid */
  NIE_SOCKOPEN,       /* can't open socket */
  NIE_NOBIND,         /* can't bind socket */
  NIE_NOACCEPT,       /* accept socket failed */
  NIE_NOLISTEN,       /* listen on socket failed */
  NIE_DISPCONN,       /* can't connect to dispatcher */
  NIE_CLICONN,        /* can't connect to client */
  NIE_APPCONN,        /* can't connect to application */
  NIE_MAXCONNS,       /* maximum number of connections in use */
  NIE_SELECT,         /* select statement failed */
  NIE_MSGWRITE,       /* message write failed */
  NIE_MSGREAD,        /* message read failed */
  NIE_MSGUNK,         /* message type unknown */
  NIE_FORK,           /* fork failed */
  NIE_SETUID,         /* unable to set user ID */
  NIE_EXEC,           /* execute failed */
  NIE_SERVUNK,        /* unknown service */
  NIE_FILOPEN,        /* can't open file */
  NIE_CONFIG,         /* error in configuration file */
  NIE_SVCENT,         /* error parsing service entry */
  NIE_RESENT,         /* error parsing resource entry */
  NIE_SVCUNAVL,       /* service is unavailable */
  NIE_RESUNAVL,       /* resource is unavailable */
  NIE_DSPDCONN,       /* dispatcher disconnected */
  NIE_SVCDCONN,       /* service disconnected */
  NIE_BADLOGIN,       /* login failed */
  NIE_LOGTIMEOUT,     /* login timed out */
  NIE_DSPTIMEOUT,     /* dispatcher timed out */
  NIE_SVCTIMEOUT,     /* service timed out */
  NIE_CMDTIMEOUT,     /* command timed out */
  NIE_TIMEOUT,        /* timed out */
  NIE_MAKEHAND,       /* unable to make message handle */
  NIE_NOTINGRP,       /* user not in group required for access */
  NIE_BADVERS,        /* version requested not in valid range */
  NIE_BADNAME,        /* name requested was not found */
  NIE_BADREQID,       /* requested ID was not found */
  NIE_BADSELFD,       /* select returned unrequested fd */
  NIE_NISERROR,       /* error using NIS service */
  NIE_INVAL,          /* invalid argument */
  NIE_BADEXIT,        /* child process exited abnormally */
  NIE_SEMBUSY,        /* already running on this machine */
  NIE_PIPE,           /* attempt to set up pipe failed */
  NIE_PIPEIO,         /* i/o error on pipe */
  NIE_SERVACK,        /* spawned server OK */
  NIE_SERVNACK,       /* spawned server failed */
  NIE_PARSE,          /* unable to parse string */
  NIE_TYPEMISM,       /* service/resource type mismatch */
  NIE_BACKUPDISP,     /* dispatcher is acting as backup */
  NIE_NOTGUEST,       /* client is not an authorized guest user */
  NIE_TCPINITFAIL,    /* TCP stack initialization failure */
  NIE_NOLOCALHOSTENT, /* no entry for local host */
  NIE_ASN1SPECFAIL,   /* ASN.1 specification load failure */
  NIE_SVCNOTENCR,     /* service does not support encryption */
  NIE_BROKSVCCONN,    /* can't connect to brokered service */
  NIE_PUBKEYENCRFAIL, /* failure trying to perform public-key encryption */
  NIE_PUBKEYDECRFAIL, /* failure trying to perform public-key decryption */
  NIE_NEWKEYNOTACCPT, /* new encryption key was not accepted by user */
  NIE_NEWKEYMISMATCH, /* mismatched new encryption key not accepted by user */
  NIE_DIREXPT,        /* expired ticket */
  NIE_DIRINVIPADDR,   /* ticket used from invalid IP address */
  NIE_DIRBADCHECKSUM, /* bad ticket checksum */
  NIE_DIRINVSERVER,   /* ticket used at invalid server */
  NIE_DIRUNCONNECT,   /* unable to connect to server */      

  NIE_UNKNOWN,        /* unknown error */
  NIE_N_ERRORS        /* do not enumerate after this line! */
};

/* number of errors */
NLM_EXTERN Uint2 *x_ni_nerr PROTO((void));
#define ni_nerr (*x_ni_nerr())

/* error level - NOT SET on error */
NLM_EXTERN NI_ErrLevel *x_ni_errlev PROTO((void));
#define ni_errlev (*x_ni_errlev())

/* error number set by failing function */
NLM_EXTERN NI_Error *x_ni_errno PROTO((void));
#define ni_errno (*x_ni_errno())

/* list of error messages */
NLM_EXTERN Char ***x_ni_errlist PROTO((void));
#define ni_errlist (*x_ni_errlist())

/* additional error text buffer */
NLM_EXTERN Char **x_ni_errtext PROTO((void));
#define ni_errtext (*x_ni_errtext())

/* list of platform names messages */
NLM_EXTERN Char ***x_ni_platform PROTO((void));
#define ni_platform (*x_ni_platform())

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif  /* _NI_ERROR_ */
