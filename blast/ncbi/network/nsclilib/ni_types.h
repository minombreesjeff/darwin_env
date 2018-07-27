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
* File Name:    ni_types.h
*
* Author:       Beatty, Gish
*
* Version Creation Date:        1/1/92
*
* $Revision: 6.7 $
*
* File Description: 
*
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: ni_types.h,v $
* Revision 6.7  2004/11/19 14:11:03  lavr
* Reinstate OBSOLETED eNII_ constants which still may be in use in some DEAD code in the toolkit
*
* Revision 6.5  2002/08/08 01:52:28  lavr
* Default dispatcher set to SERVICE
*
* Revision 6.4  2001/02/21 22:09:27  lavr
* SERVICE connector included
*
* Revision 6.3  1998/09/08 17:59:07  vakatov
* Added WWW/Firewall network interface
*
* Revision 6.2  1998/05/05 22:45:39  vakatov
* Added "eNII_Debug" network interface
*
* Revision 6.1  1998/03/30 17:56:06  vakatov
* Added ENIInterface enumerator definition and added "interface" field
* to the NI_Dispatcher struct
*
* Revision 5.3  1997/01/28 21:24:33  epstein
* move NodePtr definition to ncbimisc.h
*
* Revision 5.2  1996/06/28  17:14:39  epstein
* add job-penalty
*
* Revision 5.1  1996/06/27  17:18:17  epstein
* add load-balancing
*
* Revision 4.2  1996/04/29  15:29:10  epstein
* add disp to NI_HandPtr so that service-handle can encapsulate greater context
*
* Revision 4.1  1995/11/27  20:59:31  epstein
* add client support for direct-connection services
* 
* Revision 1.19  1995/05/24  12:09:04  epstein
* add support for tracking of how many times a client IP has used a service within a time interval
*/

#ifndef _NI_TYPES_
#define _NI_TYPES_

#include <ncbi.h>
#include <asn.h>

#define NI_Handle       MHandle    /* for API use */
#define NI_HandPtr      MHandPtr   /* for API use */
#define NI_Request      Request    /* for API use */
#define NI_ReqPtr       ReqPtr     /* for API use */
                                  
#define INETADDR_SIZ    16         /* of the form 255.255.255.255 */

#define NI_ConnState enum NIConState
NI_ConnState {
        NI_ERROR_COND = 0,
        NI_CREATED,
        NI_BOUND,
        NI_CONNECTED,
        NI_DISCARD,             /* disconnect pending (requested) */
        NI_DISCONNECTED
};

typedef Int4 (LIBCALLBACK *NI_ReadFilt) PROTO((VoidPtr mh, CharPtr buf, int bytesread, int len, CharPtr PNTR extra_buf, Int4Ptr extra_buf_len));
typedef Int4 (LIBCALLBACK *NI_WriteFilt) PROTO((VoidPtr mh, CharPtr buf, int len, CharPtr tmpbuf));
typedef void (LIBCALLBACK *NI_TimeoutHook) PROTO((Pointer p));

typedef struct encr {
        VoidPtr         desWriteContext; /* really a DES_CBC_CTX structure */
        VoidPtr         desReadContext;  /* really a DES_CBC_CTX structure */
        Uint1           encrType;
        Int1            state;
        Int2            numDeferredBytes;
        Int2            realDataLeft;
        Int2            bytesToRead;
        Uchar           deferredData[15];
        NI_WriteFilt    write_filter;    /* output filter routine */
        NI_ReadFilt     read_filter;     /* input filter routine */
} NI_EncrData, PNTR NI_EncrDataPtr;


typedef struct MHandle {
        CharPtr         hostname;       /* name of peer machine */
        Uint4           conid;          /* unique connection id number */
        Uint4           seqno;          /* unique message number */
        NI_ConnState    state;          /* of connection - bound, connected, etc. */
        int             sok;            /* the socket or file descriptor */
        int             r_timeout;      /* read timeout value in seconds */
        int             w_timeout;      /* write timeout value in seconds */
        Uint4           peer;           /* Internet address of peer */
        AsnIoPtr        raip;           /* ASNtool IO read pointer */
        AsnIoPtr        waip;           /* ASNtool IO write pointer */
        Boolean         longjump;       /* TRUE if OK to use longjump (servers) */
        time_t          access_time;    /* time stamp of last access */
        Boolean         unblocked_mode; /* is this interface "unblocked" ? */
        Boolean         have_blocked;   /* have we blocked reading this msg? */
        int             num_queued_bytes; /* amt of queued data for this msg */
        int             cur_index;      /* current index for reading queue */
        NodePtr         queued_data_list; /* queued data for this msg */
        VoidPtr         extra_proc_info; /* extra processing info, used externally */
        NI_WriteFilt    write_filter;    /* output filter routine */
        NI_ReadFilt     read_filter;     /* input filter routine */
        Boolean         write_filt_pass_thru; /* write filter is pass-thru */
        Boolean         read_filt_pass_thru; /* read filter is pass-thru */
        NI_TimeoutHook  readTimeoutHook; /* hook for read timeout expiration */
        NI_TimeoutHook  writeTimeoutHook; /* hook for write tmout expiration */
        NodePtr         readTimer;
        NodePtr         writeTimer;
        Boolean         isBrokered;
        Uint2           brokeredPort;
        NI_EncrDataPtr  encryption;
        Uint2           connectDelay;
        struct NI_Dispatcher PNTR disp;
} MHandle, PNTR MHandPtr;

typedef enum _NetServHookCode_ {
        NetServHook_dispconn = 1,
        NetServHook_svcreq,
        NetServHook_svcdisconn,
        NetServHook_dispdisconn,
        NetServHook_read,
        NetServHook_write
} NI_NetServHookCode;

typedef void (LIBCALLBACK *NI_NetServHook) PROTO((NI_HandPtr, NI_NetServHookCode, int));


typedef struct NICatalog {
        Uint4           seqno;
        CharPtr         motd;
        NodePtr         toolsetL;
} NICatalog, PNTR NICatalogPtr;

typedef struct NIStatus {
        FloatHi         load;
        FloatHi         power;
        FloatHi         lightThresh;
        FloatHi         heavyThresh;
        FloatHi         jobPenalty;
} NIStatus, PNTR NIStatusPtr;

typedef struct NIRegion {
        CharPtr         regionName;
        Uint2           priorityDelta; /* incentive to clients in that region */
} NIRegion, PNTR NIRegionPtr;

typedef struct NIToolset {
        CharPtr         host;
        CharPtr         motd;
        NodePtr         services;
        NodePtr         resources;
        NodePtr         regions;
} NIToolset, PNTR NIToolsetPtr;

typedef struct NIService {
        CharPtr         name;           /* name of service */
        Uint2           minVersion;
        Uint2           maxVersion;     /* NULL if infinite */
        Uint2           id;             /* ID that is unique on host */
        Uint2           priority;       /* priority of service */
        CharPtr         group;          /* group with access to resource */
        CharPtr         descrip;        /* description of service */
        NodePtr         typeL;          /* list of res types supported by svc */
        NodePtr         subSetList;     /* svcs of which this supports a subset */
        NodePtr         superSetList;   /* svcs of which this supports a superset */
        Uint2           priorityTimeout; /* timeout by which dispd must hear  */
        Uint2           priorityPenalty; /* resp from ncbid or impose penalty */
        Boolean         encryptionSupported; /* does this service support encryption? */
        Uint2           trackingPeriod; /* time over which Dispatcher track(s/ed) this service */
        Uint4           trackingCount; /* # service requests for this service&IP during tracking-period */
} NIService, PNTR NISvcPtr;

typedef struct NIResource {
        CharPtr         name;
        CharPtr         type;           /* same as a service name ? */
        Uint2           minVersion;
        Uint2           maxVersion;
        Uint2           id;             /* ID that is unique on host */
        CharPtr         group;          /* group with access to resource */
        CharPtr         descrip;        /* description of resource */
} NIResource, PNTR NIResPtr;

typedef struct NI_Uid {
        CharPtr         username;       /* kerberos principle */
        CharPtr         group;          /* kerberos instance */
        CharPtr         domain;         /* kerberos realm */
} NI_Uid, PNTR NI_UidPtr;


/* The available connection interfaces
 */
typedef enum {
  /* Refer to "s_NII" in "ni_lib_.c" when changing the enumerator ordering
   * or adding new interfaces */
  eNII_Dispatcher = 0,  /* old-fashioned NCBI dispatched-based connection  | OBSOLETE */
  eNII_WWW,             /* WWW-based connection                            | OBSOLETE */
  eNII_WWWFirewall,     /* eNII_WWW + pass through the NCBI firewall daemon| OBSOLETE */
  eNII_WWWDirect,       /* WWW-based stateless connection                  | OBSOLETE */
  eNII_Service,         /* SERVICE-based connection                         */
  eNII_Debug,           /* direct client-server connection                  */

  /* FEATURE: add new interfaces *above* this point(i.e. above eNII_Default),
   * so that eNII_Default be equal to the number of avail. interfaces */
  eNII_Default      /* let program try environment and config files */

/* NII_DEFAULT will be used if user did not explicitly specify the interface
 * and if application failed to find it in environment and config files */
#define NII_DEFAULT eNII_Service
} ENIInterface;


typedef struct NI_Dispatcher {
        ENIInterface     interface;
        struct Request PNTR reqResponse;     /* ptr to request struct with actual services/resources used */
        CharPtr          dispHostName;
        CharPtr          dispServiceName;
        Int4             dispSerialNo;
        Char             localHostAddr[INETADDR_SIZ];  /* Internet address of client */
        NI_HandPtr       dispHP;             /* handle for dispatcher */
        NI_HandPtr       svcsHP;             /* handle for incoming services */
        Uint2            clientPort;
        NI_UidPtr        identity;           /* client identity struct */
        int              dispTimeout;        /* timeout when communicating w/dispatcher */
        Int2             loport;             /* range of client ports to bind */
        Int2             hiport;             /* range of client ports to bind */
        Int2             referenceCount;     /* # of services connected via this dispatcher */
        Boolean          someBrokered;       /* some services are brokered */
        Boolean          brokeredDummy;      /* indicates that this is a dummy */
        ValNodePtr       encryptInfo;        /* encryption information, or NULL */
        Boolean          useSocks;           /* client should use SOCKS */
        CharPtr          adminInfo;          /* info. regarding administrator */
        CharPtr          motd;               /* message of the day for user */
        Boolean          useOutServ;         /* indicates that client will use direct connection */ 
} NI_Dispatcher, PNTR NI_DispatcherPtr;

typedef struct Request {
        CharPtr         clientAddr;
        Uint2           clientPort;
        NISvcPtr        service;        /* ptr to a service (NOT a node) */
        NodePtr         resourceL;      /* ptr to list of resources */
        NI_DispatcherPtr dispatcher;    /* dispatcher on which req is made */
} Request, PNTR ReqPtr;

typedef struct NI_PubKey {
        Int2            bits;           /* bits used in modulus */
        ByteStorePtr    modulus;
        ByteStorePtr    exponent;
} NI_PubKey, PNTR NI_PubKeyPtr;


typedef struct NI_DispInfo {
        Int4            serialno;       /* unique dispatcher serial # */
        Boolean         isalternatelist;/* is this an alternate disp-list ? */
        int             numdispatchers; /* number of dispachers in disp-list */
        CharPtr PNTR    displist;       /* list of dispatchers */
        NI_PubKeyPtr    pubKey;
} NI_DispInfo, PNTR NI_DispInfoPtr;


#endif
