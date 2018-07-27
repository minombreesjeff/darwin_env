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
* File Name:    ni_error.c
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
* $Log: ni_error.c,v $
* Revision 6.3  2000/10/30 18:12:01  beloslyu
* FreeBSD was added
*
* Revision 6.2  1999/06/07 18:28:26  beloslyu
* NetBSD port
*
* Revision 6.1  1997/11/18 21:14:30  epstein
* identify Linux Alpha client
*
* Revision 6.0  1997/08/25 18:38:38  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/07/01 19:12:45  vakatov
* [WIN32]  DLL'd "netcli.lib"
*
* Revision 5.0  1996/05/28 14:11:55  ostell
* Set to revision 5.0
*
 * Revision 4.1  1995/11/27  20:59:11  epstein
 * add client support for direct-connection services
 *
 * Revision 4.0  95/07/26  13:56:32  ostell
 * force revision to 4.0
 * 
 * Revision 1.12  1995/05/17  17:52:10  epstein
 * add RCS log revision history
 *
*/

#include "ni_error.h"
    
static Char *STATIC__ni_errlist[NIE_N_ERRORS] =
{
  "no error",
  "error",
  "no entry for user",
  "no entry for host",
  "no entry for service",
  "host address invalid",
  "port number invalid",
  "can't open socket",
  "can't bind socket",
  "accept socket failed",
  "listen on socket failed",
  "can't connect to dispatcher",
  "can't connect to client",
  "can't connect to application",
  "maximum number of connections in use",
  "select statement failed",
  "message write failed",
  "message read failed",
  "message type unknown",
  "fork failed",
  "unable to set user ID",
  "execute failed",
  "unknown service",
  "can't open file",
  "error in configuration file",
  "error parsing service entry",
  "error parsing resource entry",
  "service is unavailable",
  "resource is unavailable",
  "dispatcher disconnected",
  "service disconnected",
  "login failed",
  "login timed out",
  "dispatcher timed out",
  "service timed out",
  "command timed out",
  "timed out",
  "unable to make message handle",
  "user not in group required for access",
  "version requested not in valid range",
  "name requested was not found",
  "requested ID was not found",
  "select returned unrequested fd",
  "error using NIS service",
  "invalid argument",
  "child process exited abnormally",
  "already running on this machine",
  "attempt to set up pipe failed",
  "i/o error on pipe",
  "spawned server OK",
  "spawned server failed",
  "unable to parse string",
  "service/resource type mismatch",
  "dispatcher is acting as backup",
  "client is not an authorized guest user",
  "TCP stack initialization failure",
  "no entry for local host",
  "ASN.1 specification load failure",
  "service does not support encryption",
  "can't connect to brokered service",
  "failure trying to perform public-key encryption",
  "failure trying to perform public-key decryption",
  "new encryption key was not accepted by user",
  "mismatched new encryption key not accepted by user",
  "expired ticket",
  "ticket used from invalid IP address",
  "bad ticket checksum",
  "ticket used at invalid server",
  "unable to connect to server",
  "unknown error"
};
static Char      **STATIC__ni_errlist_ptr = &STATIC__ni_errlist[0];
NLM_EXTERN Char ***x_ni_errlist(void)  { return &STATIC__ni_errlist_ptr; }


static Char *STATIC__ni_platform[NI_N_PLATFORMS] =
{
  "unknown",           /* NI_PLATFORM_UNKNOWN */
  "Macintosh",         /* NI_PLATFORM_MAC */
  "VMS (TGV)",         /* NI_PLATFORM_VMS_TGV */
  "AXP/OpenVMS",       /* NI_PLATFORM_AXP_OPENVMS */
  "Generic UNIX",      /* NI_PLATFORM_GENERIC_UNIX */
  "IBM370 AIX",        /* NI_PLATFORM_IBM370AIX */
  "Sun",               /* NI_PLATFORM_SUN */
  "Alpha+OSF/1",       /* NI_PLATFORM_ALPHA_OSF1 */
  "Mac AU/X",          /* NI_PLATFORM_AUX */
  "Cray",              /* NI_PLATFORM_CRAY */
  "Convex",            /* NI_PLATFORM_CONVEX */
  "HP/UX",             /* NI_PLATFORM_HPUX */
  "NEXT",              /* NI_PLATFORM_NEXT */
  "SGI (MIPS)",        /* NI_PLATFORM_SGI */
  "ULTRIX",            /* NI_PLATFORM_ULTRIX */
  "DOS",               /* NI_PLATFORM_DOS */
  "WIN16",             /* NI_PLATFORM_WIN16 */
  "NEWT",              /* NI_PLATFORM_WIN_NEWT */
  "PC-NFS",            /* NI_PLATFORM_WIN_PCNFS */
  "WINSOCK",           /* NI_PLATFORM_WIN_WINSOCK */
  "WIN NT",            /* NI_PLATFORM_WINNT */
  "System V on Sparc", /* NI_PLATRFOM_SYSV_ON_SPARC */
  "VMS (UCX)",         /* NI_PLATFORM_VMS_UCX */
  "VMS (TWG)",         /* NI_PLATFORM_VMS_TWG */
  "VMS (WPW)",         /* NI_PLATFORM_VMS_WPW */
  "AIX",               /* NI_PLATFORM_AIX */
  "LINUX",             /* NI_PLATFORM_LINUX */
  "LINUX on ALPHA",    /* NI_PLATFORM_LINUX_ALPHA */
  "NetBSD",            /* NI_PLATFORM_NETBSD */
  "FreeBSD",           /* NI_PLATFORM_FREEBSD */
};
static Char       **STATIC__ni_platform_ptr = &STATIC__ni_platform[0];
NLM_EXTERN Char  ***x_ni_platform(void)   { return &STATIC__ni_platform_ptr; }


/* number of errors */
static Uint2            STATIC__ni_nerr = (Uint2)NIE_UNKNOWN;
NLM_EXTERN Uint2       *x_ni_nerr(void)      { return &STATIC__ni_nerr; }

/* error level - NOT SET on error */
static NI_ErrLevel      STATIC__ni_errlev;
NLM_EXTERN NI_ErrLevel *x_ni_errlev(void)  { return &STATIC__ni_errlev; }

/* error number set by failing function */
static NI_Error       STATIC__ni_errno;
NLM_EXTERN NI_Error  *x_ni_errno(void)     { return &STATIC__ni_errno; }

/* additional error text buffer */
static Char        STATIC__ni_errtext[ERRTEXT_BUFSIZ];
static Char       *STATIC__ni_errtext_ptr = &STATIC__ni_errtext[0];
NLM_EXTERN Char  **x_ni_errtext(void)   { return &STATIC__ni_errtext_ptr; }

