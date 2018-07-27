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
* File Name:    ni_msg.c
*
* Author:       Beatty, Gish, Epstein
*
* Version Creation Date:        1/1/92
*
* $Revision: 6.4 $
*
* File Description: 
*   This file consists mostly of functions for creating, destroying, reading,
*   and writing message structures. It also contains the high-level functions
*   for reading or writing a message, as well as the lowest-level functions
*   which are read, write, and error 'hooks' for the ASN library.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 5/01/92  Epstein     Added extensive in-line commentary, and removed all tabs
* 5/01/92  Epstein     Modified to support un-blocked reads, by storing state
*                      information and returning to the caller.
* 5/11/92  Epstein     Changed MsgBuild() to free memory in the event of
*                      an invalid message type; fixed MsgDestroy() to
*                      return -1 rather than NULL (NULL is the wrong type);
*                      added logic to allow the dispatcher to update the
*                      CONID file when the value of "conid" is incremented
* 6/18/92  Epstein     Added AsnIoSetBufsize() for write socket, to improve
*                      network throughput.
* 6/22/92  Epstein     Changed all NI_AsnRead() and NI_AsnWrite() error return
*                      codes to be the negative of the absolute value of errno,
*                      because errno's can be negative on Macintoshes.
* 7/10/92  Epstein     Changed MsgSaveData() to set the current index
*                      (mh->cur_index) to match the number of queued bytes;
*                      this avoids unintentional re-reading of queued data
*                      unless a read fails.
* 2/12/93  Epstein     Add an argument to MsgMakeHandle() to indicate whether
*                      or not a socket should be created.
* 2/19/93  Epstein     Add PC-NFS 4.0 support.
* 2/24/93  Epstein     Fix memory leak
* 3/08/93  Epstein     Add optional client platform to service request
* 3/11/93  Epstein     Add "is-alternate-list" boolean to dispatcher-list,
*                      to make it easier for a dispatcher-monitor-client
*                      to distinguish between primary and backup dispatchers.
* 3/23/93  Epstein     Change platform conditional-compilation to include
*                      the NETP_INET_ prefix, add VMS/TGV support.
* 4/02/93  Epstein     Remember to initialize conid to a known value ... this
*                      avoids clients connecting with all sorts of wild conid
*                      values.
* 4/02/93  Epstein     Add preliminary WinSock support.
* 4/13/93  Epstein     Add subSet fields to toolset to support catalog
*                      filtering by dispatcher.
* 4/21/93  Schuler     Removed (IoFuncType) typecast on calls to AsnIoNew()
*                      in order to enable compile-time type checking.
* 4/21/93  Schuler     Changed implementation of NI_AsnNew(), NI_AsnWrite(), and
*                      NI_ASNIOError() to use LIBCALLBACK calling convention.
* 4/21/93  Epstein     Fix MsgBuild() return error (was incorrectly returning
*                      FALSE instead of NULL).
* 5/07/93  Epstein     Fixup NI_SetBlocking() and NI_SetNonBlocking(), which
*                      had been backwards.  Also add Wollongong (TWG) support,
*                      and use INVALID_SOCKET #define, per WinSock spec.
* 5/25/93  Epstein     Add regional support, and add application ID to
*                      service request.
* 5/27/93  Epstein     Provide separate error code for ASN.1 dynamic object
*                      loader failure, try to do separate PeekMessage, etc.
*                      code for WinSock in the never-ending battle to fix
*                      scrollbar behavior under WinSock.
* 5/28/93  Epstein     Re-work Disable-vibrant code to use new pragmatic
*                      "Gestalt" functions, rather than making explicit
*                      calls into Vibrant.  This decouples Network Services
*                      from Vibrant.
* 6/02/93  Epstein     Fixup sys_errlist references to correctly use new
*                      SOCK_INDEX_ERRNO macro, to accomodate yet another
*                      WinSock peculiarity.
* 6/07/93  Epstein     Use new timer mechanism to support "hung timeout";
*                      also add new function MsgSetTimeoutHooks().
*                      Also add missing revision history, derived from
*                      RCS file.
* 6/09/93  Epstein     Added activity hook to report network activity back
*                      to an application.
* 6/15/93  Epstein     Fix long-standing WinSock scrolling problem, by changing
*                      NI_poll_select() to refrain from processing
*                      WM_LBUTTONUP messages.  For some reason, reading these
*                      messages off the queue and processing them resulted
*                      in a failure to correctly interpret the WM_LBUTTONUP
*                      message as an "end scrolling" message.
*                      Also clean-up Windows compilation warnings.
* 6/17/93  Epstein     Temporarily change NI_SetNonBlocking() to provide
*                      incorrect (blocking!) semantics for TWG.  This must
*                      be fixed at a later date.
* 6/22/93  Epstein     Support UCX, although NI_SetNonBlocking() and
*                      NI_SetBlocking() are currently completely unsupported.
*                      This should be fixed at a later date if it turns out
*                      that UCX actually supports these features.
* 8/09/93  Epstein     Remove annoying cursor modification, except for NEWT,
*                      where it may be necessary.
*01/21/94  Schuler     Calls to Yield() bracketed by #ifdef WIN61/#endif
*01/28/94  Schuler     Replaced "OS_MAC" with "NETP_INET_MACTCP"
*01/28/94  Schuler     Defined THIS_MODULE and THIS_FILE
*02/14/94  Epstein     Added encryption support
*02/16/94  Epstein     Replaced Gestalt calls with SetAppProperty()
*02/24/94  Epstein     Move des-key from dispinfo to login to allow ncbid
*                      to send a DES key to dispatcher.  Also performed some
*                      cleanup.
*06/08/94  Epstein     Add messages needed for SOCKS support
*06/15/94  Epstein     Add parameter to MsgWrite(), and add server-ip to
*                      service request, both to accomodate SOCKS.
*12/06/94  Epstein     Added connectDelay, adminInfo and motd field to
*                      client<->Dispatcher communication.  Also populated
*                      the priority field in the Catalog.
*12/21/94  Epstein     Make use of new socket instrumentation
* ==========================================================================
*
*
* RCS Modification History:
* $Log: ni_msg.c,v $
* Revision 6.4  2004/04/01 13:43:07  lavr
* Spell "occurred", "occurrence", and "occurring"
*
* Revision 6.3  2002/12/17 23:01:36  kans
* support for OS_UNIX_DARWIN for Mach-O executables (RGS)
*
* Revision 6.2  2001/08/29 18:00:01  juran
* Under Carbon, define missing POSIX macros in terms of Mac counterparts.
*
* Revision 6.1  1999/07/30 19:11:09  vakatov
* Use "strerror()" instead of "sys_errlist[]"
*
* Revision 6.0  1997/08/25 18:39:01  madden
* Revision changed to 6.0
*
* Revision 5.3  1997/07/01 19:12:55  vakatov
* [WIN32]  DLL'd "netcli.lib"
*
* Revision 5.2  1996/06/28 17:14:21  epstein
* add job-penalty
*
 * Revision 5.1  1996/06/27  17:17:54  epstein
 * add load-balancing
 *
 * Revision 5.0  1996/05/28  14:11:55  ostell
 * Set to revision 5.0
 *
 * Revision 4.5  1996/04/30  16:06:54  kans
 * UsingOpenTransport checks gestalt (JAE)
 *
 * Revision 4.4  1996/04/02  13:21:08  epstein
 * avoid a rare, unexplained condition by zapping a file descriptor
 *
 * Revision 4.3  1995/11/28  16:55:14  kans
 * changed return NULL to return FALSE
 *
 * Revision 4.2  1995/11/27  20:59:24  epstein
 * add client support for direct-connection services
 *
 * Revision 4.0  95/07/26  13:56:32  ostell
 * force revision to 4.0
 * 
 * Revision 1.37  1995/05/24  12:08:54  epstein
 * add support for tracking of how many times a client IP has used a service within a time interval
 *
 * Revision 1.36  95/05/17  17:52:39  epstein
 * add RCS log revision history
 * 
*/

extern char * g_nsclient_module;
#define THIS_MODULE g_nsclient_module
static char *  _this_file = __FILE__;
#define THIS_FILE _this_file

#include <ncbi.h>
#include "ni_msg.h"
#include "ni_asn.h"   /* produced by ASNTOOL */

#if TARGET_API_MAC_CARBON && !defined(O_NDELAY)
#define O_NDELAY kO_NDELAY
#endif

/* macros */
#if defined(NETP_INET_NEWT) || defined(NETP_INET_WSOCK)
/* cooperation may be required with Vibrant to avoid runaway scrollbars */
#define DisabVibrant() { Nlm_SetAppProperty("disable_vibrant", "1"); SetHourGlass(); }
#define EnabVibrant()  { Nlm_RemoveAppProperty("disable_vibrant"); PopHourGlass(); }
#else
#define DisabVibrant() 
#define EnabVibrant()
#endif /* NETP_INET_NEWT */

static NIAckPtr                 readACK PROTO((void));
static void                     writeACK PROTO((NIAckPtr ackp));
static NINackPtr                readNACK PROTO((void));
static void                     writeNACK PROTO((NINackPtr nackp));
static NILoginPtr               readLOGIN PROTO ((void));
static void                     writeLOGIN PROTO((NILoginPtr loginp));
static NISvcListPtr             readSVC_LIST PROTO ((void));
static void                     writeSVC_LIST PROTO((NISvcListPtr svclistp));
static NISvcReqPtr              readSVC_REQUEST PROTO ((void));
static void                     writeSVC_REQUEST PROTO((NISvcReqPtr svcreqp));
static NISvcRespPtr             readSVC_RESPONSE PROTO ((void));
static void                     writeSVC_RESPONSE PROTO((NISvcRespPtr svcrespp));
static NICmdPtr                 readCOMMAND PROTO ((void));
static void                     writeCOMMAND PROTO((NICmdPtr cmdp));
static NIPreRespPtr             readPRE_RESPONSE PROTO ((void));
static void                     writePRE_RESPONSE PROTO((NIPreRespPtr prp));

extern NITicketPtr              NI_DestroyTicket PROTO ((NITicketPtr));
static NITicketPtr              NI_ReadTicket PROTO ((void));
static Boolean                  NI_WriteTicket PROTO ((NITicketPtr)); 

static NIAcctPtr                readACCT PROTO ((void));
static void                     writeACCT PROTO((NIAcctPtr accp));
static NICatalogPtr             readCATALOG PROTO ((void));
static void                     writeCATALOG PROTO((NICatalogPtr catp));

static NIStatusPtr              readSTATUS PROTO ((void));
static void                     writeSTATUS PROTO((NIStatusPtr statp));

static int                      readRequest PROTO ((ReqPtr rp));
static void                     writeRequest PROTO((ReqPtr reqp));
static int                      readUid PROTO ((NI_UidPtr uid));
static void                     writeUid PROTO((NI_UidPtr uidp));
static int                      readService PROTO ((NISvcPtr svcp));
static void                     writeService PROTO((NISvcPtr svcp));
static int                      readResource PROTO ((NIResPtr resp));
static void                     writeResource PROTO((NIResPtr resp));
static int                      readRegion PROTO ((NIRegionPtr regp));
static void                     writeRegion PROTO((NIRegionPtr regp));
static int                      readToolset PROTO ((NIToolsetPtr tsp));
static void                     writeToolset PROTO((NIToolsetPtr tsp));
static int                      readPubKey PROTO ((NIPubKeyPtr pubkey));
static void                     writePubKey PROTO((NIPubKeyPtr pubkey));
static int                      readDispInfo PROTO ((NIDispInfoPtr dip));
static void                     writeDispInfo PROTO((NIDispInfoPtr dip));

static Boolean                  InitMsg PROTO((void));
static void                     WriteCleanup PROTO((CharPtr tmpbuf));
static Boolean                  MsgHaveSavedData PROTO((MHandPtr mh));
static void                     MsgSaveData PROTO((MHandPtr mh, CharPtr buf,
                                                   Uint2 len));
static int                      MsgReadSavedData PROTO((MHandPtr mh,
                                                   CharPtr buf, Uint2 len));
static void                     MsgFreeSavedData PROTO((MHandPtr mh));
static void                     SetHourGlass PROTO((void));
static void                     PopHourGlass PROTO((void));


/* VARS */

Uint4           conid = 1;      /* globally unique connection ID counter */
Jumpbuf         ni_env;         /* saved environment for LongJump (servers only) */

static AsnTypePtr       atp;
static AsnIoPtr         aip;



/* FUNCTIONS */

/******************************************************************************/
/*                                                                            */
/* Functions for creating, destroying, reading, and writing message structures*/
/*                                                                            */
/******************************************************************************/




/************************************ ACK *************************************/

NLM_EXTERN NIAckPtr NI_MakeMsgAck(void)
{
    NIAckPtr    ap;
    
    ap = (NIAckPtr) MemNew(sizeof(NIAck));
    ap->seqno = 0;
    ap->dispinfo = NULL;
    ap->adminInfo = NULL;
    ap->motd = NULL;
    return ap;
} /* NI_MakeMsgAck */


NLM_EXTERN Int2 NI_DestroyMsgAck(NIAckPtr mp)
{
    if (mp == NULL)
        return 1;
    if (mp->dispinfo != NULL)
    {
        NI_DestroyDispInfo(mp->dispinfo);
    }
    if (mp->adminInfo != NULL)
    {
        MemFree(mp->adminInfo);
    }
    if (mp->motd != NULL)
    {
        MemFree(mp->motd);
    }
    MemFree(mp);
    return 0;
} /* NI_MakeMsgAck */


static NIAckPtr 
readACK(void)
{
    DataVal     value;
    NIAckPtr    ackp;
    
    ackp = NI_MakeMsgAck();
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* seqno */
      goto AckFail;
    if (AsnReadVal(aip, atp, &value) < 0)
      goto AckFail;
    ackp->seqno = value.intvalue;
    atp = AsnReadId(aip, amp, atp);
    if (atp == MSG_ACK_disp_info) {
      if (AsnReadVal(aip, atp, &value) < 0)
        goto AckFail;
      ackp->dispinfo = NI_MakeDispInfo();
      if (readDispInfo(ackp->dispinfo) < 0)
            goto AckFail;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto AckFail;
    }
    if (atp == MSG_ACK_admin_info) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto AckFail;
        ackp->adminInfo = (CharPtr) value.ptrvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto AckFail;
    }
    if (atp == MSG_ACK_motd) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto AckFail;
        ackp->motd = (CharPtr) value.ptrvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto AckFail;
    }
    if (atp != MESSAGE_ack)
        goto AckFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto AckFail;
    return ackp;

  AckFail:
    NI_DestroyMsgAck(ackp);
    return NULL;
} /* readACK */


static void 
writeACK(NIAckPtr ackp)
{
    DataVal     value;

    AsnStartStruct(aip, MESSAGE_ack);
    value.intvalue = (Int4) ackp->seqno;
    AsnWrite(aip, MSG_ACK_seqno, &value);
    if (ackp->dispinfo != NULL) {
        AsnStartStruct(aip, MSG_ACK_disp_info);
        writeDispInfo(ackp->dispinfo);
        AsnEndStruct(aip, MSG_ACK_disp_info);
    }
    if (ackp->adminInfo != NULL) {
        value.ptrvalue = (Pointer) ackp->adminInfo;
        AsnWrite(aip, MSG_ACK_admin_info, &value);
    }
    if (ackp->motd != NULL) {
        value.ptrvalue = (Pointer) ackp->motd;
        AsnWrite(aip, MSG_ACK_motd, &value);
    }

    AsnEndStruct(aip, MESSAGE_ack);
} /* writeACK */


/************************************ NACK ***********************************/

NLM_EXTERN NINackPtr NI_MakeMsgNack(void)
{
    NINackPtr   np;
    
    np = (NINackPtr) MemNew(sizeof(NINack));
    np->seqno = 0;
    np->reason = NULL;
    np->dispinfo = NULL;
    return np;
} /* NI_MakeMsgNack */


NLM_EXTERN Int2 NI_DestroyMsgNack(NINackPtr mp)
{
    if (mp == NULL)
        return 1;
    if (mp->reason != NULL)
        MemFree(mp->reason);
    if (mp->dispinfo != NULL)
        NI_DestroyDispInfo(mp->dispinfo);
    MemFree(mp);
    return 0;
} /* NI_DestroyMsgNack */


static NINackPtr 
readNACK(void)
{
    DataVal     value;
    NINackPtr   nackp;
    
    nackp = NI_MakeMsgNack();
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* seqno */
        goto NackFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto NackFail;
    nackp->seqno = value.intvalue;
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* code */
        goto NackFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto NackFail;
    nackp->code = (NI_Error) value.intvalue;
    
    if ((atp = AsnReadId(aip, amp, atp)) == MSG_NACK_reason) {  /* reason */
        if (AsnReadVal(aip, atp, &value) < 0)
            goto NackFail;
        nackp->reason = (CharPtr) value.ptrvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto NackFail;
    }
    if (atp == MSG_NACK_disp_info) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto NackFail;
        nackp->dispinfo = NI_MakeDispInfo();
        if (readDispInfo(nackp->dispinfo) < 0)
            goto NackFail;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto NackFail;
    }
    if (atp != MESSAGE_nack)
        goto NackFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto NackFail;
    return nackp;

  NackFail:
    NI_DestroyMsgNack(nackp);
    return NULL;
} /* readNACK */


static void 
writeNACK(NINackPtr nackp)
{
    DataVal     value;

    AsnStartStruct(aip, MESSAGE_nack);
    value.intvalue = (Int4) nackp->seqno;
    AsnWrite(aip, MSG_NACK_seqno, &value);
    value.intvalue = (Int4) nackp->code;
    AsnWrite(aip, MSG_NACK_code, &value);
    
    if (nackp->reason != NULL) {
        value.ptrvalue = (Pointer) nackp->reason;
        AsnWrite(aip, MSG_NACK_reason, &value);
    }
    if (nackp->dispinfo != NULL) {
        AsnStartStruct(aip, MSG_NACK_disp_info);
        writeDispInfo(nackp->dispinfo);
        AsnEndStruct(aip, MSG_NACK_disp_info);
    }
    AsnEndStruct(aip, MESSAGE_nack);
}


/************************************ LOGIN ***********************************/

NLM_EXTERN NILoginPtr NI_MakeMsgLogin(void)
{
    NILoginPtr  lp;
    
    lp = (NILoginPtr) MemNew(sizeof(NILogin));
    lp->seqno = 0;
    lp->uid = NI_MakeUid();
    lp->password = NULL;
    lp->dispserialno = -1;
    lp->encryptionDesired = FALSE;
    lp->pubKey = NULL;
    lp->desKey = NULL;
    lp->connectDelay = -1;
    lp->server_port = 0;
    return lp;
} /* NI_MakeMsgLogin() */


NLM_EXTERN Int2 NI_DestroyMsgLogin(NILoginPtr lp)
{
    if (lp == NULL)
        return 1;
    if (lp->uid != NULL)
        NI_DestroyUid(lp->uid);
    if (lp->password != NULL)
        MemFree(lp->password);
    if (lp->pubKey != NULL)
        NI_DestroyPubKey(lp->pubKey);
    if (lp->desKey != NULL)
        BSFree(lp->desKey);
    MemFree(lp);
    return 0;
} /* NI_DestroyMsgLogin() */


static NILoginPtr 
readLOGIN(void)
{
    DataVal     value;
    NILoginPtr  loginp;
    
    loginp = NI_MakeMsgLogin();
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)       /* seqno */
        goto LoginFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto LoginFail;
    loginp->seqno = value.intvalue;
    
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)       /* MSG_LOGIN_uid */
        goto LoginFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto LoginFail;
    if (readUid(loginp->uid) < 0)
        goto LoginFail;
    
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)       /* password ? */
        goto LoginFail;

    if (atp == MSG_LOGIN_password) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto LoginFail;
        loginp->password = (CharPtr) value.ptrvalue;

    if ((atp = AsnReadId(aip, amp, atp)) == NULL)       /* disp_serial_no ? */
        goto LoginFail;
    }
    
    if (atp == MSG_LOGIN_disp_serial_no) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto LoginFail;
        loginp->dispserialno = value.intvalue;
 
   if ((atp = AsnReadId(aip, amp, atp)) == NULL)    /* encryption desired ? */
        goto LoginFail;
   }
    
    if (atp == MSG_LOGIN_encryption_desired) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto LoginFail;
        loginp->encryptionDesired = value.boolvalue;

    if ((atp = AsnReadId(aip, amp, atp)) == NULL)       /* pub_key ? */
        goto LoginFail;

    }
    
    if (atp == MSG_LOGIN_pub_key) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto LoginFail;
        loginp->pubKey = NI_MakePubKey();
        if (readPubKey(loginp->pubKey) < 0)
            goto LoginFail;
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)       /* des_key ? */
        goto LoginFail;
   }
    
    if (atp == MSG_LOGIN_des_key) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto LoginFail;
        loginp->desKey = (ByteStorePtr) value.ptrvalue;

    if ((atp = AsnReadId(aip, amp, atp)) == NULL)       /* connect_delay ? */
        goto LoginFail;
    }
    
    if (atp == MSG_LOGIN_connect_delay) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto LoginFail;
        loginp->connectDelay = value.intvalue;

    if ((atp = AsnReadId(aip, amp, atp)) == NULL)       /* server_port ? */
        goto LoginFail;
    }
    
    if (atp == MSG_LOGIN_server_port) {  
        if (AsnReadVal(aip, atp, &value) < 0)
            goto LoginFail;

        loginp->server_port = value.intvalue;

    if ((atp = AsnReadId(aip, amp, atp)) == NULL)       /* last message ? */
        goto LoginFail;
    }
    if (atp != MESSAGE_login)
        goto LoginFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto LoginFail;
    return loginp;

  LoginFail:
    NI_DestroyMsgLogin(loginp);
    return NULL;
} /* readLOGIN */


static void 
writeLOGIN(NILoginPtr loginp)
{
    DataVal     value;

    AsnStartStruct(aip, MESSAGE_login);
    value.intvalue = (Int4) loginp->seqno;
    AsnWrite(aip, MSG_LOGIN_seqno, &value);
    AsnStartStruct(aip, MSG_LOGIN_uid);
    writeUid(loginp->uid);
    AsnEndStruct(aip, MSG_LOGIN_uid);
    
    if (loginp->password != NULL) {
        value.ptrvalue = (Pointer) loginp->password;
        AsnWrite(aip, MSG_LOGIN_password, &value);
    }
    if (loginp->dispserialno >= 0) {
        value.intvalue = loginp->dispserialno;
        AsnWrite(aip, MSG_LOGIN_disp_serial_no, &value);
    }
    if (loginp->encryptionDesired) {
        value.boolvalue = loginp->encryptionDesired;
        AsnWrite(aip, MSG_LOGIN_encryption_desired, &value);
    }
    if (loginp->pubKey != NULL) {
        AsnStartStruct(aip, MSG_LOGIN_pub_key);
        writePubKey(loginp->pubKey);
        AsnEndStruct(aip, MSG_LOGIN_pub_key);
    }
    if (loginp->desKey != NULL) {
        value.ptrvalue = (Pointer) loginp->desKey;
        AsnWrite(aip, MSG_LOGIN_des_key, &value);
    }
    if (loginp->connectDelay >= 0) {
        value.intvalue = loginp->connectDelay;
        AsnWrite(aip, MSG_LOGIN_connect_delay, &value);
    }

    if (loginp->server_port > 0) {
        value.intvalue = loginp->server_port;
        AsnWrite(aip, MSG_LOGIN_server_port, &value);
    }


    AsnEndStruct(aip, MESSAGE_login);
} /* writeLOGIN */


/************************************ SVC_LIST ********************************/

NLM_EXTERN NISvcListPtr NI_MakeMsgSvclist(void)
{
    NISvcListPtr        sp;
    
    sp = (NISvcListPtr) MemNew(sizeof(NISvcList));
    sp->seqno = 0;
    sp->toolset = NI_MakeToolset();
    sp->knowsTracking = FALSE;
    return sp;
} /* NI_MakeMsgSvclist */


NLM_EXTERN Int2 NI_DestroyMsgSvclist(NISvcListPtr sp)
{
    if (sp == NULL)
        return 1;
    if (sp->toolset != NULL)
        NI_DestroyToolset(sp->toolset);
    MemFree(sp);
    return 0;
} /* NI_DestroyMsgSvclist */


static NISvcListPtr 
readSVC_LIST(void)
{
    DataVal     value;
    NISvcListPtr        svclistp;
    
    svclistp = NI_MakeMsgSvclist();
    
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* MSG_SVC_LIST_seqno */
        goto SvcListFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto SvcListFail;
    svclistp->seqno = value.intvalue;
    
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* MSG_SVC_LIST_toollist */
        goto SvcListFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto SvcListFail;
    if (readToolset(svclistp->toolset) < 0)
        goto SvcListFail;
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)
        goto SvcListFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto SvcListFail;
    
    if (atp == MSG_SVC_LIST_knows_tracking)
    {
        svclistp->knowsTracking = value.boolvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto SvcListFail;
        if (AsnReadVal(aip, atp, &value) < 0)
            goto SvcListFail;
    }

    if (atp != MESSAGE_svc_list)
        goto SvcListFail;
    return svclistp;

  SvcListFail:
    NI_DestroyMsgSvclist(svclistp);
    return NULL;
} /* readSVC_LIST */


static void 
writeSVC_LIST(NISvcListPtr svclistp)
{
    DataVal     value;

    AsnStartStruct(aip, MESSAGE_svc_list);
    value.intvalue = (Int4) svclistp->seqno;
    AsnWrite(aip, MSG_SVC_LIST_seqno, &value);
    
    AsnStartStruct(aip, MSG_SVC_LIST_toollist);
    writeToolset(svclistp->toolset);
    AsnEndStruct(aip, MSG_SVC_LIST_toollist);
    if (svclistp->knowsTracking)
    {
        value.boolvalue = svclistp->knowsTracking;
        AsnWrite(aip, MSG_SVC_LIST_knows_tracking, &value);
    }
    
    AsnEndStruct(aip, MESSAGE_svc_list);
} /* writeSVC_LIST */


/************************************ SVC_REQUEST *****************************/

NLM_EXTERN NISvcReqPtr NI_MakeMsgSvcreq(void)
{
    NISvcReqPtr sp;
    
    sp = (NISvcReqPtr) MemNew(sizeof(NISvcReq));
    sp->seqno = 0;
    sp->uid = NI_MakeUid();
    sp->request = NI_MakeRequest();
    sp->platform = NI_PLATFORM_UNKNOWN;
    sp->applId = NULL;
    sp->desKey = NULL;
    sp->wantPreResponse = FALSE;
    sp->server_ip = 0;
    sp->server_port = 0;
    sp->want_ticket = FALSE;
    sp->ticket = NULL;
    return sp;
} /* NI_MakeMsgSvcreq() */


NLM_EXTERN Int2 NI_DestroyMsgSvcreq(NISvcReqPtr sp)
{
    if (sp == NULL)
        return 1;
    if (sp->uid != NULL)
        NI_DestroyUid(sp->uid);
    if (sp->request != NULL)
        NI_DestroyRequest(sp->request);
    if (sp->applId != NULL)
        MemFree(sp->applId);
    if (sp->desKey != NULL)
        BSFree (sp->desKey);
    if (sp->ticket != NULL)
        NI_DestroyTicket (sp->ticket);
    MemFree(sp);
    return 0;
} /* NI_DestroyMsgSvcreq() */ 


static NISvcReqPtr 
readSVC_REQUEST(void)
{
    DataVal     value;
    NISvcReqPtr svcreqp;
    
    svcreqp = NI_MakeMsgSvcreq();
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)       /* MSG_SVC_REQUEST_seqno */
        goto SvcReqFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto SvcReqFail;
    svcreqp->seqno = value.intvalue;
    
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)       /* MSG_SVC_REQUEST_conid */
        goto SvcReqFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto SvcReqFail;
    svcreqp->conid = value.intvalue;
    
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)       /* MSG_SVC_REQUEST_uid */
        goto SvcReqFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto SvcReqFail;
    if (readUid(svcreqp->uid) < 0)
        goto SvcReqFail;

    if ((atp = AsnReadId(aip, amp, atp)) == NULL)       /* MSG_SVC_REQUEST_request */
        goto SvcReqFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto SvcReqFail;
    if (readRequest(svcreqp->request) < 0)
        goto SvcReqFail;

    if ((atp = AsnReadId(aip, amp, atp)) == MSG_SVC_REQUEST_platform) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto SvcReqFail;
        svcreqp->platform = (Uint4) value.intvalue;
        atp = AsnReadId(aip, amp, atp);
    }

    if (atp == MSG_SVC_REQUEST_appl_id) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto SvcReqFail;
        svcreqp->applId = (CharPtr) value.ptrvalue;
        atp = AsnReadId(aip, amp, atp);
    }
    
    if (atp == MSG_SVC_REQUEST_des_key) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto SvcReqFail;
        svcreqp->desKey = (ByteStorePtr) value.ptrvalue;
        atp = AsnReadId(aip, amp, atp);
    }

    if (atp == SVC_REQUEST_want_pre_response) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto SvcReqFail;
        svcreqp->wantPreResponse = value.boolvalue;
        atp = AsnReadId(aip, amp, atp);
    }

    if (atp == MSG_SVC_REQUEST_server_ip) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto SvcReqFail;
        svcreqp->server_ip = value.intvalue;
        atp = AsnReadId(aip, amp, atp);
    }
    if (atp == MSG_SVC_REQUEST_server_port) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto SvcReqFail;
        svcreqp->server_port = value.intvalue;
        atp = AsnReadId(aip, amp, atp);
    }
    if (atp == MSG_SVC_REQUEST_want_ticket) {
         if (AsnReadVal(aip, atp, &value) < 0)
            goto SvcReqFail;
        svcreqp->want_ticket = value.boolvalue;
        atp = AsnReadId(aip, amp, atp);
    }
    if (svcreqp->want_ticket) {

        if (atp == MSG_SVC_REQUEST_ticket) {
            if (AsnReadVal(aip, atp, &value) < 0)
                goto SvcReqFail;
            if((svcreqp->ticket = NI_ReadTicket()) ==  0)
                goto SvcReqFail;
            atp = AsnReadId(aip, amp, atp);
        }
    }

    if (atp != MESSAGE_svc_request)
        goto SvcReqFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto SvcReqFail;
    return svcreqp;

  SvcReqFail:
    NI_DestroyMsgSvcreq(svcreqp);
    return NULL;
} /* readSVC_REQUEST */


static void 
writeSVC_REQUEST(NISvcReqPtr svcreqp)
{
    DataVal     value;

    AsnStartStruct(aip, MESSAGE_svc_request);
    value.intvalue = (Int4) svcreqp->seqno;
    AsnWrite(aip, MSG_SVC_REQUEST_seqno, &value);
    value.intvalue = (Int4) svcreqp->conid;
    AsnWrite(aip, MSG_SVC_REQUEST_conid, &value);
    AsnStartStruct(aip, MSG_SVC_REQUEST_uid);
    writeUid(svcreqp->uid);
    AsnEndStruct(aip, MSG_SVC_REQUEST_uid);
    AsnStartStruct(aip, MSG_SVC_REQUEST_request);
    writeRequest(svcreqp->request);
    AsnEndStruct(aip, MSG_SVC_REQUEST_request);
    value.intvalue = (Int4) svcreqp->platform; 
    AsnWrite(aip, MSG_SVC_REQUEST_platform, &value);

    if (svcreqp->applId != NULL)
    {
        value.ptrvalue = (Pointer) svcreqp->applId;
        AsnWrite(aip, MSG_SVC_REQUEST_appl_id, &value);
    }
    
    if (svcreqp->desKey != NULL)
    {
        value.ptrvalue = (Pointer) svcreqp->desKey;
        AsnWrite(aip, MSG_SVC_REQUEST_des_key, &value);
    }

    if (svcreqp->wantPreResponse)
    {
        value.boolvalue = svcreqp->wantPreResponse;
        AsnWrite(aip, SVC_REQUEST_want_pre_response, &value);
    }

    if (svcreqp->server_ip != 0)
    {
        value.intvalue = svcreqp->server_ip;
        AsnWrite(aip, MSG_SVC_REQUEST_server_ip, &value);
    }

    if (svcreqp->server_port != 0)
    {
        value.intvalue = svcreqp->server_port;
        AsnWrite(aip, MSG_SVC_REQUEST_server_port, &value);
    }

    if (svcreqp->want_ticket) {
    
        value.boolvalue = svcreqp->want_ticket;
        AsnWrite(aip, MSG_SVC_REQUEST_want_ticket, &value);

        if (svcreqp->ticket != NULL) {
            AsnStartStruct(aip, MSG_SVC_REQUEST_ticket);
            NI_WriteTicket(svcreqp->ticket);  
            AsnEndStruct(aip, MSG_SVC_REQUEST_ticket);
        }
    }
    AsnEndStruct(aip, MESSAGE_svc_request);
} /* writeSVC_REQUEST */

/********************************* TICKET *************************************/

NLM_EXTERN NITicketPtr NI_MakeTicket(void)
{
    NITicketPtr ptr = MemNew((size_t) sizeof(NITicket));
    return ptr;
}

extern NITicketPtr NI_DestroyTicket(NITicketPtr ptr)
{
    if (ptr == NULL) {
        return NULL;
  }
    BSFree(ptr -> confounding_rand_num);
    BSFree(ptr -> client_ip_1);
    BSFree(ptr -> client_ip_2);
    BSFree(ptr -> server_ip);
    BSFree(ptr -> client_des_key);
    BSFree(ptr -> ticket_expiration);
    BSFree(ptr -> checksum);
    return MemFree(ptr);
}

static NITicketPtr
NI_ReadTicket(void)
{
    DataVal av;
    NITicketPtr ptr;
    ptr = NI_MakeTicket();

   if ((atp = AsnReadId(aip,amp, atp)) == NULL)
     goto TicketFail;

    if (atp == TICKET_seqno) {
        if ( AsnReadVal(aip, atp, &av) < 0) {
            goto TicketFail;
        }
        ptr -> seqno = av.intvalue;
     if ((atp = AsnReadId(aip,amp, atp)) == NULL)
         goto TicketFail;
    }
    if (atp == TICKET_confounding_rand_num) {
        if ( AsnReadVal(aip, atp, &av) < 0) {
            goto TicketFail;
        }
        ptr -> confounding_rand_num = (ByteStorePtr) av.ptrvalue;
        atp = AsnReadId(aip,amp, atp);
    }
    if (atp == TICKET_client_ip_1) {
        if ( AsnReadVal(aip, atp, &av) < 0) {
            goto TicketFail;
        }
        ptr -> client_ip_1 = (ByteStorePtr) av.ptrvalue;
        atp = AsnReadId(aip,amp, atp);
    }
    if (atp == TICKET_client_ip_2) {
        if ( AsnReadVal(aip, atp, &av) < 0) {
            goto TicketFail;
        }
        ptr -> client_ip_2 = (ByteStorePtr) av.ptrvalue;
        atp = AsnReadId(aip,amp, atp);
    }
    if (atp == TICKET_server_ip) {
        if ( AsnReadVal(aip, atp, &av) < 0) {
            goto TicketFail;
        }
        ptr -> server_ip = (ByteStorePtr) av.ptrvalue;
        atp = AsnReadId(aip,amp, atp);
    }
    if (atp == TICKET_client_des_key) {
        if ( AsnReadVal(aip, atp, &av) < 0) {
            goto TicketFail;
        }
        ptr -> client_des_key = (ByteStorePtr) av.ptrvalue;
        atp = AsnReadId(aip,amp, atp);
    }
    if (atp == TICKET_ticket_expiration) {
        if ( AsnReadVal(aip, atp, &av) < 0) {
            goto TicketFail;
        }
        ptr -> ticket_expiration = (ByteStorePtr) av.ptrvalue;
        atp = AsnReadId(aip,amp, atp);
    }
    if (atp == TICKET_checksum) {
        if ( AsnReadVal(aip, atp, &av) < 0) {
            goto TicketFail;
        }
        ptr -> checksum = (ByteStorePtr) av.ptrvalue;
        atp = AsnReadId(aip,amp, atp);
    }

    if (AsnReadVal(aip, atp, &av) < 0) {
        goto TicketFail;
    }
    /* end struct */

    return ptr;

TicketFail:

    NI_DestroyTicket(ptr);
    return NULL;
}

static Boolean 
NI_WriteTicket(NITicketPtr ptr)
{
    DataVal av;
    Boolean retval = FALSE;

    av.intvalue = ptr -> seqno;
    retval = AsnWrite(aip, TICKET_seqno,  &av);
    if (ptr -> confounding_rand_num != NULL) {
        av.ptrvalue = (Pointer) ptr -> confounding_rand_num;
        retval = AsnWrite(aip, TICKET_confounding_rand_num,  &av);
    }
    if (ptr -> client_ip_1 != NULL) {
        av.ptrvalue = (Pointer) ptr -> client_ip_1;
        retval = AsnWrite(aip, TICKET_client_ip_1,  &av);
    }
    if (ptr -> client_ip_2 != NULL) {
        av.ptrvalue = (Pointer) ptr -> client_ip_2;
        retval = AsnWrite(aip, TICKET_client_ip_2,  &av);
    }
    if (ptr -> server_ip != NULL) {
        av.ptrvalue = (Pointer) ptr -> server_ip;
        retval = AsnWrite(aip, TICKET_server_ip,  &av);
    }
    if (ptr -> client_des_key != NULL) {
        av.ptrvalue = (Pointer) ptr -> client_des_key;
        retval = AsnWrite(aip, TICKET_client_des_key,  &av);
    }
    if (ptr -> ticket_expiration != NULL) {
        av.ptrvalue = (Pointer) ptr -> ticket_expiration;
        retval = AsnWrite(aip, TICKET_ticket_expiration,  &av);
    }
    if (ptr -> checksum != NULL) {
        av.ptrvalue = (Pointer) ptr -> checksum;
        retval = AsnWrite(aip, TICKET_checksum,  &av);
    }

    if (!retval)
        goto TicketFail;
    return retval;

TicketFail:

    NI_DestroyTicket(ptr);
    return FALSE;

}



/********************************* SVC_RESPONSE *******************************/

NLM_EXTERN NISvcRespPtr NI_MakeMsgSvcresp(void)
{
    NISvcRespPtr        sp;
    
    sp = (NISvcRespPtr) MemNew(sizeof(NISvcResp));
    sp->seqno = 0;
    sp->request = NI_MakeRequest();
    return sp;
} /* NI_MakeMsgSvcresp() */


NLM_EXTERN Int2 NI_DestroyMsgSvcresp(NISvcRespPtr sp)
{
    if (sp == NULL)
        return 1;
    if (sp->request != NULL)
        NI_DestroyRequest(sp->request);
    MemFree(sp);
    return 0;
} /* NI_DestroyMsgSvcresp() */ 


static NISvcRespPtr 
readSVC_RESPONSE(void)
{
    DataVal             value;
    NISvcRespPtr        svcrespp;
    
    svcrespp = NI_MakeMsgSvcresp();
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* MSG_SVC_RESPONSE_seqno */
        goto SvcRespFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto SvcRespFail;
    svcrespp->seqno = value.intvalue;
    
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* MSG_SVC_RESPONSE_request */
        goto SvcRespFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto SvcRespFail;
    if (readRequest(svcrespp->request) < 0)
        goto SvcRespFail;
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* MSG_SVC_RESPONSE_request */
        goto SvcRespFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto SvcRespFail;
    
    if (atp != MESSAGE_svc_response)
        goto SvcRespFail;
    return svcrespp;

  SvcRespFail:
    NI_DestroyMsgSvcresp(svcrespp);
    return NULL;
} /* readSVC_RESPONSE */


static void 
writeSVC_RESPONSE(NISvcRespPtr svcrespp)
{
    DataVal     value;

    AsnStartStruct(aip, MESSAGE_svc_response);
    value.intvalue = (Int4) svcrespp->seqno;
    AsnWrite(aip, MSG_SVC_RESPONSE_seqno, &value);
    AsnStartStruct(aip, MSG_SVC_RESPONSE_request);
    writeRequest(svcrespp->request);
    AsnEndStruct(aip, MSG_SVC_RESPONSE_request);
    AsnEndStruct(aip, MESSAGE_svc_response);
} /* writeSVC_RESPONSE */


/************************************ COMMAND *********************************/

NLM_EXTERN NICmdPtr NI_MakeMsgCmd(void)
{
    NICmdPtr    cp;
    
    cp = (NICmdPtr) MemNew(sizeof(NICmd));
    cp->seqno = 0;
    return cp;
} /* NI_MakeMsgCmd() */


NLM_EXTERN Int2 NI_DestroyMsgCmd(NICmdPtr cp)
{
    if (cp == NULL)
        return 1;
    MemFree(cp);
    return 0;
} /* NI_DestroyMsgCmd() */ 


static NICmdPtr 
readCOMMAND(void)
{
    DataVal     value;
    NICmdPtr    cmdp;
    
    cmdp = NI_MakeMsgCmd();
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* seqno */
        goto CmdFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto CmdFail;
    cmdp->seqno = value.intvalue;
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* command */
        goto CmdFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto CmdFail;
    cmdp->code = (MsgCommand) value.intvalue;
    if ((atp = AsnReadId(aip, amp, atp)) != MESSAGE_command)
        goto CmdFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto CmdFail;
    return cmdp;

  CmdFail:
    NI_DestroyMsgCmd(cmdp);
    return NULL;
} /* readCOMMAND */


static void 
writeCOMMAND(NICmdPtr cmdp)
{
    DataVal     value;

    AsnStartStruct(aip, MESSAGE_command);
    value.intvalue = (Int4) cmdp->seqno;
    AsnWrite(aip, MSG_CMD_seqno, &value);
    value.intvalue = (Int4) cmdp->code;
    AsnWrite(aip, MSG_CMD_command, &value);
    AsnEndStruct(aip, MESSAGE_command);
} /* writeCOMMAND */


/************************************ PRE-RESPONSE *********************************/

NLM_EXTERN NIPreRespPtr NI_MakeMsgPreResp(void)
{
    NIPreRespPtr    cp;
    
    cp = (NIPreRespPtr) MemNew(sizeof(NIPreResp));
    cp->seqno = 0;
    return cp;
} /* NI_MakeMsgPreResp() */


NLM_EXTERN Int2 NI_DestroyMsgPreResp(NIPreRespPtr cp)
{
    if (cp == NULL)
        return 1;
    MemFree(cp);
    return 0;
} /* NI_DestroyMsgPreResp() */ 


static NIPreRespPtr 
readPRE_RESPONSE(void)
{
    DataVal     value;
    NIPreRespPtr    prp;
    
    prp = NI_MakeMsgPreResp();
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* seqno */
        goto PrFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto PrFail;
    prp->seqno = value.intvalue;
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* server IP */
        goto PrFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto PrFail;
    prp->server_ip = value.intvalue;
    if ((atp = AsnReadId(aip, amp, atp)) != MESSAGE_svc_pre_response)
        goto PrFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto PrFail;
    return prp;

  PrFail:
    NI_DestroyMsgPreResp(prp);
    return NULL;
} /* readPRE_RESPONSE */


static void 
writePRE_RESPONSE(NIPreRespPtr prp)
{
    DataVal     value;

    AsnStartStruct(aip, MESSAGE_svc_pre_response);
    value.intvalue = (Int4) prp->seqno;
    AsnWrite(aip, MSG_SVC_PRE_RESPONSE_seqno, &value);
    value.intvalue = (Int4) prp->server_ip;
    AsnWrite(aip, MSG_SVC_PRE_RESPONSE_server_ip, &value);
    AsnEndStruct(aip, MESSAGE_svc_pre_response);
} /* writePRE_RESPONSE */

/************************************ ACCT ************************************/

NLM_EXTERN NIAcctPtr NI_MakeMsgAcct(void)
{
    NIAcctPtr   ap;
    
    ap = (NIAcctPtr) MemNew(sizeof(NIAcct));
    ap->seqno = 0;
    ap->jobname = NULL;
    return ap;
} /* NI_MakeMsgAcct() */


NLM_EXTERN Int2 NI_DestroyMsgAcct(NIAcctPtr ap)
{
    if (ap == NULL)
        return 1;
    if (ap->jobname != NULL)
        MemFree(ap->jobname);
    MemFree(ap);
    return 0;
} /* NI_DestroyMsgAcct() */


static NIAcctPtr 
readACCT(void)
{
    DataVal     value;
    NIAcctPtr   accp;
    
    accp = NI_MakeMsgAcct();
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* seqno */
        goto AcctFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto AcctFail;
    accp->seqno = value.intvalue;
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* conid */
        goto AcctFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto AcctFail;
    accp->conid = value.intvalue;
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* jobname */
        goto AcctFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto AcctFail;
    accp->jobname = (CharPtr) value.ptrvalue;
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* usertime */
        goto AcctFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto AcctFail;
    accp->usertime = value.intvalue;
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* systemtime */
        goto AcctFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto AcctFail;
    accp->systemtime = value.intvalue;
    if ((atp = AsnReadId(aip, amp, atp)) != MESSAGE_acct)
        goto AcctFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto AcctFail;
    return accp;

  AcctFail:
    NI_DestroyMsgAcct(accp);
    return NULL;
} /* NIreadACCT */


static void 
writeACCT(NIAcctPtr accp)
{
    DataVal     value;

    AsnStartStruct(aip, MESSAGE_acct);
    value.intvalue = (Int4) accp->seqno;
    AsnWrite(aip, MSG_ACCT_seqno, &value);
    value.intvalue = (Int4) accp->conid;
    AsnWrite(aip, MSG_ACCT_conid, &value);
    value.ptrvalue = (Pointer) accp->jobname;
    AsnWrite(aip, MSG_ACCT_jobname, &value);
    value.intvalue = (Int4) accp->usertime;
    AsnWrite(aip, MSG_ACCT_usertime, &value);
    value.intvalue = (Int4) accp->systemtime;
    AsnWrite(aip, MSG_ACCT_systemtime, &value);
    AsnEndStruct(aip, MESSAGE_acct);
} /* NIwriteACCT */


/************************************ CATALOG *********************************/

NLM_EXTERN NICatalogPtr NI_MakeMsgCatalog(void)
{
    NICatalogPtr        cp;
    
    cp = (NICatalogPtr) MemNew(sizeof(NICatalog));
    cp->motd = NULL;
    cp->toolsetL = NULL;
    return cp;
} /* NI_MakeCatalog */


NLM_EXTERN Int2 NI_DestroyMsgCatalog(NICatalogPtr cp)
{
    NodePtr             np;
    
    
    if (cp == NULL)
        return 1;
    if (cp->motd != NULL)
        MemFree(cp->motd);
    if ((np = cp->toolsetL) != NULL) {
        do {
            NI_DestroyToolset((NIToolsetPtr) np->elem);
            np = ListDelete(np);
        } while (np != NULL);
    }   
    MemFree(cp);
    return 0;
} /* NI_DestroyMsgCatalog */


static NICatalogPtr 
readCATALOG(void)
{
    DataVal     value;
    NICatalogPtr        catp;
    NIToolsetPtr        tsp;
    
    catp = NI_MakeMsgCatalog();
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* MSG_CATALOG_seqno */
        goto CatalogFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto CatalogFail;
    catp->seqno = value.intvalue;
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)
        goto CatalogFail;
    
    if (atp == MSG_CATALOG_motd) {              /* MSG_CATALOG_motd ? */
        if (AsnReadVal(aip, atp, &value) < 0)
            goto CatalogFail;
        catp->motd = (CharPtr) value.ptrvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto CatalogFail;
    }
    
    if (atp == MSG_CATALOG_toollists) {         /* MSG_CATALOG_toollists ? */
        if (AsnReadVal(aip, atp, &value) < 0)
            goto CatalogFail;
        while ((atp = AsnReadId(aip, amp, atp)) == MSG_CATALOG_toollists_E) {
            if (AsnReadVal(aip, atp, &value) < 0)
                goto CatalogFail;
            tsp = NI_MakeToolset();
            if (readToolset(tsp) < 0)
                goto CatalogFail;
            ListBreakRing(tsp->services);
            ListBreakRing(tsp->resources);
            catp->toolsetL = ListInsert((VoidPtr) tsp, catp->toolsetL); /* end of list */
        }
        if (atp == NULL)
            goto CatalogFail;
        catp->toolsetL = catp->toolsetL->next;          /* point to first */
        if (AsnReadVal(aip, atp, &value) < 0)           /* MSG_CATALOG_toollists */
            goto CatalogFail;
        ListBreakRing(catp->toolsetL);
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto CatalogFail;
    }
    
    if (atp != MESSAGE_catalog)
        goto CatalogFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto CatalogFail;
    return catp;

  CatalogFail:
    NI_DestroyMsgCatalog(catp);
    return NULL;
} /* readCATALOG */


static void 
writeCATALOG(NICatalogPtr catp)
{
    DataVal     value;
    NodePtr     np, lastnode;
    
    AsnStartStruct(aip, MESSAGE_catalog);
    value.intvalue = (Int4) catp->seqno;
    AsnWrite(aip, MSG_CATALOG_seqno, &value);
    if (catp->motd != NULL) {
        value.ptrvalue = (Pointer) catp->motd;
        AsnWrite(aip, MSG_CATALOG_motd, &value);
    }
    
    np = catp->toolsetL;
    if (np != NULL) {
        AsnStartStruct(aip, MSG_CATALOG_toollists);
        lastnode = np->last;
        while (np != NULL) {
            AsnStartStruct(aip, MSG_CATALOG_toollists_E);
            writeToolset((NIToolsetPtr) np->elem);
            AsnEndStruct(aip, MSG_CATALOG_toollists_E);
            if (np == lastnode)
                break;
            np = np->next;
        }
        AsnEndStruct(aip, MSG_CATALOG_toollists);
    }
    
    AsnEndStruct(aip, MESSAGE_catalog);
} /* writeCatalog */


/************************************ LOAD STATUS *********************************/

NLM_EXTERN NIStatusPtr NI_MakeMsgStatus(void)
{
    NIStatusPtr        sp;
    
    sp = (NIStatusPtr) MemNew(sizeof(NIStatus));
    sp->load = 0.0;
    sp->power = 0.0;
    sp->lightThresh = 0.0;
    sp->heavyThresh = 0.0;
    sp->jobPenalty = 0.0;
    return sp;
} /* NI_MakeStatus */


NLM_EXTERN Int2 NI_DestroyMsgStatus(NIStatusPtr sp)
{
    NodePtr             np;
    
    
    if (sp == NULL)
        return 1;
    MemFree(sp);
    return 0;
} /* NI_DestroyMsgStatus */

static NIStatusPtr 
readSTATUS(void)
{
    DataVal     value;
    NIStatusPtr        statusp;
    
    statusp = NI_MakeMsgStatus();
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* MSG_LOAD_STATUS_load */
        goto StatusFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto StatusFail;
    statusp->load = value.realvalue;

    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* MSG_LOAD_STATUS_power */
        goto StatusFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto StatusFail;
    statusp->power = value.realvalue;

    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* MSG_LOAD_STATUS_light_thresh */
        goto StatusFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto StatusFail;
    statusp->lightThresh = value.realvalue;

    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* MSG_LOAD_STATUS_heavy_thresh */
        goto StatusFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto StatusFail;
    statusp->heavyThresh = value.realvalue;

    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* MSG_LOAD_STATUS_job_penalty */
        goto StatusFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto StatusFail;
    statusp->jobPenalty = value.realvalue;

    if ((atp = AsnReadId(aip, amp, atp)) == NULL)
        goto StatusFail;
    
    if (atp != MESSAGE_load_status)
        goto StatusFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto StatusFail;
    return statusp;

  StatusFail:
    NI_DestroyMsgStatus(statusp);
    return NULL;
} /* readSTATUS */


static void 
writeSTATUS(NIStatusPtr statp)
{
    DataVal     value;
    
    AsnStartStruct(aip, MESSAGE_load_status);

    value.realvalue = (FloatLo) statp->load;
    AsnWrite(aip, MSG_LOAD_STATUS_load, &value);
    value.realvalue = (FloatLo) statp->power;
    AsnWrite(aip, MSG_LOAD_STATUS_power, &value);
    value.realvalue = (FloatLo) statp->lightThresh;
    AsnWrite(aip, MSG_LOAD_STATUS_light_thresh, &value);
    value.realvalue = (FloatLo) statp->heavyThresh;
    AsnWrite(aip, MSG_LOAD_STATUS_heavy_thresh, &value);
    value.realvalue = (FloatLo) statp->jobPenalty;
    AsnWrite(aip, MSG_LOAD_STATUS_job_penalty, &value);
    
    AsnEndStruct(aip, MESSAGE_load_status);
} /* writeStatus */


/************************************ REQUEST *********************************/

NLM_EXTERN ReqPtr NI_MakeRequest(void)
{
    ReqPtr              rp;
    
    rp = (ReqPtr) MemNew(sizeof(Request));
    rp->clientAddr = NULL;
    rp->clientPort = 0;
    rp->service = NI_MakeService();
    rp->resourceL = NULL;
    return(rp);
} /* NI_MakeRequest() */


NLM_EXTERN Int2 NI_DestroyRequest(ReqPtr rp)
{
    if (rp == NULL)
        return 1;
    if (rp->clientAddr != NULL)
        MemFree(rp->clientAddr);
    if (rp->service != NULL)
        NI_DestroyService(rp->service);
    if (rp->resourceL != NULL) {        /* destroy list of nodes and resources they point to */
        while (rp->resourceL != NULL) {
            NI_DestroyResource((NIResPtr)rp->resourceL->elem);
            rp->resourceL = ListDelete(rp->resourceL);
        }
    }
    MemFree(rp);
    return 0;
} /* NI_DestroyRequest() */


static int
readRequest(ReqPtr reqp)
{
    DataVal     value;
    NIResPtr    resp;
    
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)
        goto RequestFail;
    if (atp == REQUEST_address) {               /* REQUEST_address ? */
        if (AsnReadVal(aip, atp, &value) < 0)
            goto RequestFail;
        reqp->clientAddr = (CharPtr) value.ptrvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto RequestFail;
    }
    if (atp == REQUEST_port) {                  /* REQUEST_port ? */
        if (AsnReadVal(aip, atp, &value) < 0)
            goto RequestFail;
        reqp->clientPort = (Uint2) value.intvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto RequestFail;
    }
    if (AsnReadVal(aip, atp, &value) < 0)       /* REQUEST_svcentry */
        goto RequestFail;
    if (readService(reqp->service) < 0)
        goto RequestFail;
    
    if ((atp = AsnReadId(aip, amp, atp)) == REQUEST_resentry) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto RequestFail;

        while ((atp = AsnReadId(aip, amp, atp)) == REQUEST_resentry_E) {
            if (AsnReadVal(aip, atp, &value) < 0)
                goto RequestFail;
            resp = NI_MakeResource();
            if (readResource(resp) < 0) {
                NI_DestroyResource(resp);
                goto RequestFail;
            }
            reqp->resourceL = ListInsert((VoidPtr) resp, reqp->resourceL);      /* end of list */
        }
        if (atp == NULL)
            goto RequestFail;
        reqp->resourceL = reqp->resourceL->next;        /* point to first */
        if (AsnReadVal(aip, atp, &value) < 0)           /* REQUEST_resentry */
            goto RequestFail;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto RequestFail;
    }
    if (atp == NULL)
        goto RequestFail;

    if (AsnReadVal(aip, atp, &value) < 0)
        goto RequestFail;
    return 0;

  RequestFail:
    if (reqp->resourceL != NULL) {      /* destroy list of nodes and resources they point to */
        while (reqp->resourceL != NULL) {
            NI_DestroyResource((NIResPtr)reqp->resourceL->elem);
            reqp->resourceL = ListDelete(reqp->resourceL);
        }
    }
    return -1;
} /* readrequest */


static void 
writeRequest(ReqPtr reqp)
{
    DataVal     value;
    NodePtr     np, lastnode;
    
    if (reqp->clientAddr != NULL) {
        value.ptrvalue = (Pointer) reqp->clientAddr;
        AsnWrite(aip, REQUEST_address, &value);
    }
    if (reqp->clientPort != 0) {
        value.intvalue = (Int4) reqp->clientPort;
        AsnWrite(aip, REQUEST_port, &value);
    }
    AsnStartStruct(aip, REQUEST_svcentry);
    writeService(reqp->service);
    AsnEndStruct(aip, REQUEST_svcentry);
    
    np = reqp->resourceL;
    if (np != NULL) {
        AsnStartStruct(aip, REQUEST_resentry);
        lastnode = np->last;
        while (np != NULL) {
            AsnStartStruct(aip, REQUEST_resentry_E);
            writeResource((NIResPtr) np->elem);
            AsnEndStruct(aip, REQUEST_resentry_E);
            if (np == lastnode)
                break;
            np = np->next;
        }
        AsnEndStruct(aip, REQUEST_resentry);
    }
} /* writeRequest */


/************************************ UID *************************************/

NLM_EXTERN NI_UidPtr NI_MakeUid(void)
{
    NI_UidPtr   up;
    
    up = (NI_UidPtr) MemNew(sizeof(NI_Uid));
    up->username = NULL;
    up->group = NULL;
    up->domain = NULL;
    return up;
} /* NI_MakeUid */


NLM_EXTERN Int2 NI_DestroyUid(NI_UidPtr up)
{
    if (up == NULL)
        return 1;
    if (up->username != NULL)
        MemFree(up->username);
    if (up->group != NULL)
        MemFree(up->group);
    if (up->domain != NULL)
        MemFree(up->domain);
    MemFree(up);
    return 0;
} /* NI_DestroyUid */


static int 
readUid(NI_UidPtr uidptr)
{
    DataVal     value;

    if ((atp = AsnReadId(aip, amp, atp)) == NULL || atp != IDENTITY_username)           /* username */
        goto UidFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto UidFail;
    uidptr->username = (CharPtr) value.ptrvalue;
    
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* group ? */
        goto UidFail;
    if (atp == IDENTITY_group) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto UidFail;
        uidptr->group = (CharPtr) value.ptrvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)           /* domain ? */
            goto UidFail;
    }
    if (atp == IDENTITY_domain) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto UidFail;
        uidptr->domain = (CharPtr) value.ptrvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto UidFail;
    }
    if (AsnReadVal(aip, atp, &value) < 0)
        goto UidFail;
    return 0;

  UidFail:
    return -1;
} /* readUid */


static void 
writeUid(NI_UidPtr uidptr)
{
    DataVal     value;
    
    value.ptrvalue = (Pointer) uidptr->username;
    AsnWrite(aip, IDENTITY_username, &value);
    
    if (uidptr->group != NULL) {
        value.ptrvalue = (Pointer) uidptr->group;
        AsnWrite(aip, IDENTITY_group, &value);
    }
    if (uidptr->domain != NULL) {
        value.ptrvalue = (Pointer) uidptr->domain;
        AsnWrite(aip, IDENTITY_domain, &value);
    }
} /* writeUid */


/************************************ SERVICE *********************************/

NLM_EXTERN NISvcPtr NI_MakeService(void)
{
    NISvcPtr    sp;
    
    sp = (NISvcPtr) MemNew(sizeof(NIService));
    sp->name = NULL;
    sp->minVersion = 0;
    sp->maxVersion = 0;
    sp->id = 0;
    sp->priority = 0;
    sp->priorityTimeout = 0;
    sp->priorityPenalty = 0;
    sp->group = NULL;
    sp->descrip = NULL;
    sp->typeL = NULL;
    sp->encryptionSupported = FALSE;
    sp->trackingPeriod = 0;
    sp->trackingCount = 0;
    return sp;
} /* NI_MakeService() */


NLM_EXTERN Int2 NI_DestroyService(NISvcPtr sp)
{
    if (sp == NULL)
        return 1;
    if (sp->name != NULL)
        MemFree(sp->name);
    if (sp->group != NULL)
        MemFree(sp->group);
    if (sp->descrip != NULL)
        MemFree(sp->descrip);
    if (sp->typeL != NULL) { /* destroy list of service types */
        ListStrDel(sp->typeL);
    }
    if (sp->subSetList != NULL) {
        ListStrDel(sp->subSetList);
    }
    if (sp->superSetList != NULL) {
        ListStrDel(sp->superSetList);
    }

    MemFree(sp);
    return 0;
} /* NI_DestroyService() */


static int 
readService(NISvcPtr svcptr)
{
    DataVal     value;
    CharPtr     svctype;

    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* SVC_ENTRY_name */
        goto ServiceFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto ServiceFail;
    svcptr->name = (CharPtr) value.ptrvalue;
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* SVC_ENTRY_minvers */
        goto ServiceFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto ServiceFail;
    svcptr->minVersion = (Uint2) value.intvalue;
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* SVC_ENTRY_maxvers */
        goto ServiceFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto ServiceFail;
    svcptr->maxVersion = (Uint2) value.intvalue;
    
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)
        goto ServiceFail;
    if (atp == SVC_ENTRY_id) {                          /* SVC_ENTRY_id ? */
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ServiceFail;
        svcptr->id = (Uint2) value.intvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto ServiceFail;
    }
    if (atp == SVC_ENTRY_priority) {                    /* SVC_ENTRY_priority ? */
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ServiceFail;
        svcptr->priority = (Uint2) value.intvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto ServiceFail;
    }
    if (atp == SVC_ENTRY_group) {                       /* SVC_ENTRY_group ? */
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ServiceFail;
        svcptr->group = (CharPtr) value.ptrvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto ServiceFail;
    }
    if (atp == SVC_ENTRY_description) {                 /* SVC_ENTRY_description ? */
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ServiceFail;
        svcptr->descrip = (CharPtr) value.ptrvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto ServiceFail;
    }
    if (atp == SVC_ENTRY_types) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ServiceFail;

        /* read the list of service types */
        while ((atp = AsnReadId(aip, amp, atp)) == SVC_ENTRY_types_E)
        {
            if (AsnReadVal(aip, atp, &value) < 0)
                goto ServiceFail;
            svctype = value.ptrvalue;
            svcptr->typeL = ListInsert((VoidPtr) svctype, svcptr->typeL);
        }
        if (atp == NULL)
            goto ServiceFail;
        svcptr->typeL = svcptr->typeL->next;            /* point to first */
        if (AsnReadVal(aip, atp, &value) < 0)           /* SVC_ENTRY_types */
            goto ServiceFail;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto ServiceFail;
    }

    if (atp == SVC_ENTRY_priority_timeout) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ServiceFail;
        svcptr->priorityTimeout = (Uint2) value.intvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto ServiceFail;
    }

    if (atp == SVC_ENTRY_priority_penalty) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ServiceFail;
        svcptr->priorityPenalty = (Uint2) value.intvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto ServiceFail;
    }
    
    if (atp == SVC_ENTRY_encryption_supported) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ServiceFail;
        svcptr->encryptionSupported = value.boolvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto ServiceFail;
    }
        
    if (atp == SVC_ENTRY_tracking_period) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ServiceFail;
        svcptr->trackingPeriod = value.intvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto ServiceFail;
    }
        
    if (atp == SVC_ENTRY_tracking_count) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ServiceFail;
        svcptr->trackingCount = value.intvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto ServiceFail;
    }
        
    if (atp == NULL)
        goto ServiceFail;

    if (AsnReadVal(aip, atp, &value) < 0)
        goto ServiceFail;
    return 0;

  ServiceFail:
    return -1;
} /* readService */


static void 
writeService(NISvcPtr svcptr)
{
    DataVal     value;
    NodePtr     np;
    NodePtr     lastnode;

    value.ptrvalue = (Pointer) svcptr->name;
    AsnWrite(aip, SVC_ENTRY_name, &value);
    value.intvalue = (Int4) svcptr->minVersion;
    AsnWrite(aip, SVC_ENTRY_minvers, &value);
    value.intvalue = (Int4) svcptr->maxVersion;
    AsnWrite(aip, SVC_ENTRY_maxvers, &value);
    
    if (svcptr->id != 0) {
        value.intvalue = (Int4) svcptr->id;
        AsnWrite(aip, SVC_ENTRY_id, &value);
    }
    if (svcptr->priority != 0) {
        value.intvalue = (Int4) svcptr->priority;
        AsnWrite(aip, SVC_ENTRY_priority, &value);
    }
    if (svcptr->group != NULL) {
        value.ptrvalue = (Pointer) svcptr->group;
        AsnWrite(aip, SVC_ENTRY_group, &value);
    }
    if (svcptr->descrip != NULL) {
        value.ptrvalue = (Pointer) svcptr->descrip;
        AsnWrite(aip, SVC_ENTRY_description, &value);
    }

    np = svcptr->typeL;
    if (np != NULL) {
        AsnStartStruct(aip, SVC_ENTRY_types);
        lastnode = np->last;
        while (np != NULL) {
            value.ptrvalue = (Pointer) np->elem;
            AsnWrite(aip, SVC_ENTRY_types_E, &value);
            if (np == lastnode)
                break;
            np = np->next;
        }
        AsnEndStruct(aip, SVC_ENTRY_types);
    }

    if (svcptr->priorityTimeout != 0)
    {
        value.intvalue = svcptr->priorityTimeout;
        AsnWrite(aip, SVC_ENTRY_priority_timeout, &value);
    }

    if (svcptr->priorityPenalty != 0)
    {
        value.intvalue = svcptr->priorityPenalty;
        AsnWrite(aip, SVC_ENTRY_priority_penalty, &value);
    }

    if (svcptr->encryptionSupported)
    {
        value.boolvalue = svcptr->encryptionSupported;
        AsnWrite(aip, SVC_ENTRY_encryption_supported, &value);
    }

    if (svcptr->trackingPeriod != 0)
    {
        value.intvalue = svcptr->trackingPeriod;
        AsnWrite(aip, SVC_ENTRY_tracking_period, &value);
    }

    if (svcptr->trackingCount != 0)
    {
        value.intvalue = svcptr->trackingCount;
        AsnWrite(aip, SVC_ENTRY_tracking_count, &value);
    }
} /* writeService */


/************************************ RESOURCE ********************************/

NLM_EXTERN NIResPtr NI_MakeResource(void)
{
    NIResPtr    rp;
    
    rp = (NIResPtr) MemNew(sizeof(NIResource));
    rp->name = NULL;
    rp->type = NULL;
    rp->minVersion = 0;
    rp->maxVersion = 0;
    rp->id = 0;
    rp->group = NULL;
    rp->descrip = NULL;
    return rp;
} /* NI_MakeResource() */


NLM_EXTERN Int2 NI_DestroyResource(NIResPtr rp)
{
    if (rp == NULL)
        return 1;
    if (rp->name != NULL)
        MemFree(rp->name);
    if (rp->type != NULL)
        MemFree(rp->type);
    if (rp->group != NULL)
        MemFree(rp->group);
    if (rp->descrip != NULL)
        MemFree(rp->descrip);
    MemFree(rp);
    return 0;
} /* NI_DestroyResource */


static int
readResource(NIResPtr resptr)
{
    DataVal     value;

    if ((atp = AsnReadId(aip, amp, atp)) == NULL)       /* RES_ENTRY_name */
        goto ResourceFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto ResourceFail;
    resptr->name = (CharPtr) value.ptrvalue;
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)       /* RES_ENTRY_type */
        goto ResourceFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto ResourceFail;
    resptr->type = (CharPtr) value.ptrvalue;
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)       /* RES_ENTRY_minvers */
        goto ResourceFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto ResourceFail;
    resptr->minVersion = (Uint2) value.intvalue;
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)       /* RES_ENTRY_maxvers */
        goto ResourceFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto ResourceFail;
    resptr->maxVersion = (Uint2) value.intvalue;
    
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)
        goto ResourceFail;
    if (atp == RES_ENTRY_id) {                          /* RES_ENTRY_id */
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ResourceFail;
        resptr->id = (Uint2) value.intvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto ResourceFail;
    }
    if (atp == RES_ENTRY_group) {                       /* RES_ENTRY_group */
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ResourceFail;
        resptr->group = (CharPtr) value.ptrvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto ResourceFail;
    }
    if (atp == RES_ENTRY_description) {                 /* RES_ENTRY_description */
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ResourceFail;
        resptr->descrip = (CharPtr) value.ptrvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto ResourceFail;
    }
    if (AsnReadVal(aip, atp, &value) < 0)
        goto ResourceFail;
    return 0;

  ResourceFail:
    return -1;
} /* readResource */


static void 
writeResource(NIResPtr resptr)
{
    DataVal     value;

    value.ptrvalue = (Pointer) resptr->name;
    AsnWrite(aip, RES_ENTRY_name, &value);
    value.ptrvalue = (Pointer) resptr->type;
    AsnWrite(aip, RES_ENTRY_type, &value);
    value.intvalue = (Int4) resptr->minVersion;
    AsnWrite(aip, RES_ENTRY_minvers, &value);
    value.intvalue = (Int4) resptr->maxVersion;
    AsnWrite(aip, RES_ENTRY_maxvers, &value);
    
    if (resptr->id != 0) {
        value.intvalue = (Int4) resptr->id;
        AsnWrite(aip, RES_ENTRY_id, &value);
    }
    if (resptr->group != NULL) {
        value.ptrvalue = (Pointer) resptr->group;
        AsnWrite(aip, RES_ENTRY_group, &value);
    }
    if (resptr->descrip != NULL) {
        value.ptrvalue = (Pointer) resptr->descrip;
        AsnWrite(aip, RES_ENTRY_description, &value);
    }
} /* writeResource */


/************************************ REGION *********************************/

NLM_EXTERN NIRegionPtr NI_MakeRegion(void)
{
    NIRegionPtr    rp;
    
    rp = (NIRegionPtr) MemNew(sizeof(NIRegion));
    rp->regionName = NULL;
    rp->priorityDelta = 0;
    return rp;
} /* NI_MakeRegion() */


NLM_EXTERN Int2 NI_DestroyRegion(NIRegionPtr rp)
{
    if (rp == NULL)
        return 1;
    if (rp->regionName != NULL)
        MemFree(rp->regionName);

    MemFree(rp);
    return 0;
} /* NI_DestroyRegion() */


static int 
readRegion(NIRegionPtr regptr)
{
    DataVal     value;

    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* REGION_DESCR_region_name */
        goto RegionFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto RegionFail;
    regptr->regionName = (CharPtr) value.ptrvalue;
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* REGION_DESCR_priority_delta */
        goto RegionFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto RegionFail;
    regptr->priorityDelta = (Uint2) value.intvalue;

    if ((atp = AsnReadId(aip, amp, atp)) == NULL)
        goto RegionFail;

    if (AsnReadVal(aip, atp, &value) < 0)
        goto RegionFail;
    return 0;

  RegionFail:
    return -1;
} /* readRegion */


static void 
writeRegion(NIRegionPtr regptr)
{
    DataVal     value;

    value.ptrvalue = (Pointer) regptr->regionName;
    AsnWrite(aip, REGION_DESCR_region_name, &value);
    value.intvalue = (Int4) regptr->priorityDelta;
    AsnWrite(aip, REGION_DESCR_priority_delta, &value);
} /* writeRegion */



/************************************ PubKey ********************************/

NLM_EXTERN NIPubKeyPtr NI_MakePubKey(void)
{
    NIPubKeyPtr pubkey;

    pubkey = (NIPubKeyPtr) MemNew(sizeof(NIPubKey));
    pubkey->bits = 0;
    pubkey->modulus = NULL;
    pubkey->exponent = NULL;
    return pubkey;
} /* NI_MakePubKey */


NLM_EXTERN Int2 NI_DestroyPubKey(NIPubKeyPtr pubkey)
{
    if (pubkey == NULL)
        return 1;
    if (pubkey->modulus != NULL)
        BSFree (pubkey->modulus);
    if (pubkey->exponent != NULL)
        BSFree (pubkey->exponent);
    MemFree(pubkey);
    return 0;
}


static int
readPubKey(NIPubKeyPtr pubkey)
{
    DataVal     value;

    if ((atp = AsnReadId(aip, amp, atp)) == NULL || atp != RSA_PUBKEY_bits)
        goto PubKeyFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto PubKeyFail;
    pubkey->bits = (Int2) value.intvalue;

    /* modulus */
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)
        goto PubKeyFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto PubKeyFail;
    pubkey->modulus = (ByteStorePtr) value.ptrvalue;

    /* exponent */
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)
        goto PubKeyFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto PubKeyFail;
    pubkey->exponent = (ByteStorePtr) value.ptrvalue;

    /* end struct */
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)
        goto PubKeyFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto PubKeyFail;
    return 0;

  PubKeyFail:
    return -1;
}

static void
writePubKey(NIPubKeyPtr pubkey)
{
    DataVal     value;

    if (pubkey == NULL)
        return;
    value.intvalue = pubkey->bits;
    AsnWrite(aip, RSA_PUBKEY_bits, &value);
    value.ptrvalue = pubkey->modulus;
    AsnWrite(aip, RSA_PUBKEY_modulus, &value);
    value.ptrvalue = pubkey->exponent;
    AsnWrite(aip, RSA_PUBKEY_exponent, &value);
}

NLM_EXTERN int NI_ReadPubKey(AsnIoPtr extaip, AsnTypePtr extatp, NIPubKeyPtr pubkey)
{
    AsnIoPtr      savaip = aip;
    AsnTypePtr    savatp = atp;
    DataVal       value;
    int           retval = -1;

    if (! InitMsg())
        return 0;
    aip = extaip;
    atp = extatp;
    if ((atp = AsnReadId(aip, amp, atp)) == RSA_PUBKEY)
    {
        if (AsnReadVal(aip, atp, &value) >= 0)
        {
            retval = readPubKey(pubkey);
        }
   }
   aip = savaip;
   atp = savatp;
   return retval;
}

NLM_EXTERN void NI_WritePubKey(AsnIoPtr extaip, AsnTypePtr extatp, NIPubKeyPtr pubkey)
{
    AsnIoPtr savaip = aip;
    AsnTypePtr savatp = atp;

    if (! InitMsg())
        return;
    aip = extaip;
    atp = extatp;
    if (extatp != NULL)
        AsnStartStruct(extaip, extatp);
    writePubKey(pubkey);
    if (extatp != NULL)
        AsnEndStruct(extaip, extatp);
    aip = savaip;
    atp = savatp;
}



/************************************ DispInfo ********************************/

NLM_EXTERN NIDispInfoPtr NI_MakeDispInfo(void)
{
    NIDispInfoPtr   dip;
    
    dip = (NIDispInfoPtr) MemNew(sizeof(NIDispInfo));
    dip->serialno = -1;
    dip->isalternatelist = FALSE;
    dip->numdispatchers = 0;
    dip->displist = NULL;
    dip->pubKey = NULL;
    return dip;
} /* NI_MakeDispInfo */


NLM_EXTERN Int2 NI_DestroyDispInfo(NIDispInfoPtr dip)
{
    Int2 num;

    if (dip == NULL)
        return 1;
    if (dip->displist != NULL)
    {
        for (num = 0; num < dip->numdispatchers; num++)
        {
            dip->displist[num] = (CharPtr) MemFree(dip->displist[num]);
        }
        MemFree(dip->displist);
    }
    if (dip->pubKey != NULL)
    {
        NI_DestroyPubKey(dip->pubKey);
    }
    MemFree(dip);
    return 0;
} /* NI_DestroyDispInfo */


static int 
readDispInfo(NIDispInfoPtr dip)
{
    DataVal     value;
    int         num;

    if ((atp = AsnReadId(aip, amp, atp)) == NULL || atp != DISPATCHER_INFO_serial_no)
        goto DispInfoFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto DispInfoFail;
    dip->serialno = value.intvalue;
    
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* is altlist?*/
        goto DispInfoFail;
    if (atp == INFO_is_alternate_list) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto DispInfoFail;
        dip->isalternatelist = value.boolvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)           /* num disps ?*/
            goto DispInfoFail;
    }
    
    if (atp == DISPATCHER_INFO_num_dispatchers) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto DispInfoFail;
        dip->numdispatchers = (int) value.intvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)           /* disps ? */
            goto DispInfoFail;
    }
    if (atp == DISPATCHER_INFO_disp_list) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto DispInfoFail;
        dip->displist = (CharPtr PNTR) MemNew(sizeof(CharPtr) * dip->numdispatchers);
        atp = AsnReadId(aip, amp, atp);
        for (num = 0; num < dip->numdispatchers &&
             atp == DISPATCHER_INFO_disp_list_E; num++)
        {
            if (AsnReadVal(aip, atp, &value) <= 0)
                goto DispInfoFail;
            dip->displist[num] = (CharPtr) value.ptrvalue;
            atp = AsnReadId(aip, amp, atp);
        }
        if (AsnReadVal(aip, atp, &value) <= 0)
            goto DispInfoFail;
        atp = AsnReadId(aip, amp, atp);
    }
    if (atp == DISPATCHER_INFO_pub_key) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto DispInfoFail;
        dip->pubKey = NI_MakePubKey();
        if (readPubKey(dip->pubKey) < 0)
            goto DispInfoFail;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto DispInfoFail;
    }

    if (AsnReadVal(aip, atp, &value) < 0)
        goto DispInfoFail;
    return 0;

  DispInfoFail:
    return -1;
} /* readDispInfo */


static void 
writeDispInfo(NIDispInfoPtr dip)
{
    DataVal     value;
    int         num;
    
    if (dip == NULL)
        return;
    value.intvalue = dip->serialno;
    AsnWrite(aip, DISPATCHER_INFO_serial_no, &value);
    value.boolvalue = dip->isalternatelist;
    AsnWrite(aip, INFO_is_alternate_list, &value);
    value.intvalue = dip->numdispatchers;
    AsnWrite(aip, DISPATCHER_INFO_num_dispatchers, &value);
    if (dip->displist != NULL) {
        AsnStartStruct (aip, DISPATCHER_INFO_disp_list);
        for (num = 0; num < dip->numdispatchers; num++) {
            value.ptrvalue = dip->displist[num];
            AsnWrite (aip, DISPATCHER_INFO_disp_list_E, &value);
        }
        AsnEndStruct (aip, DISPATCHER_INFO_disp_list);
    }
    if (dip->pubKey != NULL) {
        AsnStartStruct (aip, DISPATCHER_INFO_pub_key);
        writePubKey (dip->pubKey);
        AsnEndStruct (aip, DISPATCHER_INFO_pub_key);
    }
} /* writeDispInfo */

NLM_EXTERN int NI_ReadDispInfo(AsnIoPtr extaip, AsnTypePtr extatp, NIDispInfoPtr dip)
{
    AsnIoPtr     savaip = aip;
    AsnTypePtr   savatp = atp;
    DataVal      value;
    int          retval = -1;

    if (! InitMsg())
        return 0;
    aip = extaip;
    atp = extatp;
    if ((atp = AsnReadId(aip, amp, atp)) == DISPATCHER_INFO)
    {
        if (AsnReadVal(aip, atp, &value) >= 0)
        {
            retval = readDispInfo(dip);
        }
    }
    aip = savaip;
    atp = savatp;
    return retval;
}

NLM_EXTERN void NI_WriteDispInfo(AsnIoPtr extaip, AsnTypePtr extatp, NIDispInfoPtr dip)
{
    AsnIoPtr savaip = aip;
    AsnTypePtr savatp = atp;

    if (! InitMsg())
        return;
    aip = extaip;
    atp = extatp;
    if (extatp != NULL)
        AsnStartStruct(extaip, extatp);
    writeDispInfo(dip);
    if (extatp != NULL)
        AsnEndStruct(extaip, extatp);
    aip = savaip;
    atp = savatp;
}


/************************************ TOOLSET *********************************/

NLM_EXTERN NIToolsetPtr NI_MakeToolset(void)
{
    NIToolsetPtr        tsp;
    
    tsp = (NIToolsetPtr) MemNew(sizeof(NIToolset));
    tsp->host = NULL;
    tsp->motd = NULL;
    tsp->services = NULL;
    tsp->resources = NULL;
    tsp->regions = NULL;
    return tsp;
} /* NI_MakeToolset */


NLM_EXTERN Int2 NI_DestroyToolset(NIToolsetPtr tsp)
{
    NodePtr             np;
    
    if (tsp == NULL)
        return 1;
    if (tsp->host != NULL)
        MemFree(tsp->host);
    if (tsp->motd != NULL)
        MemFree(tsp->motd);
    
    if ((np = tsp->services) != NULL) {
        do {
            NI_DestroyService((NISvcPtr)np->elem);
            np = ListDelete(np);
        } while (np != NULL);
    }   
    
    if ((np = tsp->resources) != NULL) {
        do {
            NI_DestroyResource((NIResPtr)np->elem);
            np = ListDelete(np);
        } while (np != NULL);
    }   
    
    if ((np = tsp->regions) != NULL) {
        do {
            NI_DestroyRegion((NIRegionPtr)np->elem);
            np = ListDelete(np);
        } while (np != NULL);
    }   
    
    MemFree(tsp);
    return 0;
} /* NI_DestroyToolset */


NLM_EXTERN NIToolsetPtr NI_GetCatToolset(NIToolsetPtr tsp)
{
    NIToolsetPtr        dtsp;
    NISvcPtr            svcp, tsvcp;
    NIResPtr            resp, tresp;
    NodePtr             np;
    
    dtsp = NI_MakeToolset();
    
    if (tsp->host != NULL)
        dtsp->host = StringSave(tsp->host);
    if (tsp->motd != NULL)
        dtsp->motd = StringSave(tsp->motd);
    
    if (tsp->services != NULL) {
        np = tsp->services->last;
        do {
            np = ListGetNext(np);
            svcp = (NISvcPtr) np->elem;
            /* note that services which are a "subset" of another listed */
            /* service are filtered-out here                             */
            if (svcp != NULL && svcp->subSetList == NULL) {
                tsvcp = NI_MakeService();
                if (svcp->name != NULL)
                    tsvcp->name = StringSave(svcp->name);

                if (svcp->typeL != NULL)
                { /* make a copy of the list */
                    tsvcp->typeL = ListStrCopy(svcp->typeL);
                }

                tsvcp->minVersion = svcp->minVersion;
                tsvcp->maxVersion = svcp->maxVersion;
                tsvcp->priority = svcp->priority;
                tsvcp->id = 0;          /* these two not sent in catalog */
                tsvcp->group = NULL;
                if (svcp->descrip != NULL)
                    tsvcp->descrip = StringSave(svcp->descrip);
                dtsp->services = ListInsert((VoidPtr) tsvcp, dtsp->services);
            }
        } while (np != tsp->services->last);
        if (dtsp->services != NULL)
        {
            dtsp->services = dtsp->services->next;  /* point to first */
        }
    }
    
    if (tsp->resources != NULL) {
        np = tsp->resources->last;
        do {
            np = ListGetNext(np);
            resp = (NIResPtr) np->elem;
            if (resp != NULL) {
                tresp = NI_MakeResource();
                if (resp->name != NULL)
                    tresp->name = StringSave(resp->name);
                if (resp->type != NULL)
                    tresp->type = StringSave(resp->type);
                tresp->minVersion = resp->minVersion;
                tresp->maxVersion = resp->maxVersion;
                tresp->id = 0;          /* these two not sent in catalog */
                tresp->group = NULL;
                if (resp->descrip != NULL)
                    tresp->descrip = StringSave(resp->descrip);
                dtsp->resources = ListInsert((VoidPtr) tresp, dtsp->resources);
            }
        } while (np != tsp->resources->last);
        if (dtsp->resources != NULL)
        {
            dtsp->resources = dtsp->resources->next;        /* point to first */
        }
    }
    
    return dtsp;
} /* NI_GetCatToolset */


static int
readToolset(NIToolsetPtr tsp)
{
    DataVal     value;
    NISvcPtr    svcp;
    NIResPtr    resp;
    NIRegionPtr reg;
    
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)       /* TOOLSET_host */
        goto ToolsetFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto ToolsetFail;
    tsp->host = (CharPtr) value.ptrvalue;
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)
        goto ToolsetFail;
    
    if (atp == TOOLSET_motd) {                  /* TOOLSET_motd ? */
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ToolsetFail;
        tsp->motd = (CharPtr) value.ptrvalue;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto ToolsetFail;
    }
    if (atp == TOOLSET_services) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ToolsetFail;
        while ((atp = AsnReadId(aip, amp, atp)) == TOOLSET_services_E) {
            if (AsnReadVal(aip, atp, &value) < 0)
                goto ToolsetFail;
            svcp = NI_MakeService();
            readService(svcp);
            tsp->services = ListInsert((VoidPtr) svcp, tsp->services);
        }
        if (atp == NULL)
            goto ToolsetFail;
        tsp->services = tsp->services->next;    /* point to first */
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ToolsetFail;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto ToolsetFail;
    }
    if (atp == TOOLSET_resources) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ToolsetFail;
        while ((atp = AsnReadId(aip, amp, atp)) == TOOLSET_resources_E) {
            if (AsnReadVal(aip, atp, &value) < 0)
                goto ToolsetFail;
            resp = NI_MakeResource();
            readResource(resp);
            tsp->resources = ListInsert((VoidPtr) resp, tsp->resources);
        }
        if (atp == NULL)
            goto ToolsetFail;
        tsp->resources = tsp->resources->next;  /* point to first */
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ToolsetFail;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto ToolsetFail;
    }
    if (atp == TOOLSET_regions) {
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ToolsetFail;
        while ((atp = AsnReadId(aip, amp, atp)) == TOOLSET_regions_E) {
            if (AsnReadVal(aip, atp, &value) < 0)
                goto ToolsetFail;
            reg = NI_MakeRegion();
            readRegion(reg);
            tsp->regions = ListInsert((VoidPtr) reg, tsp->regions);
        }
        if (atp == NULL)
            goto ToolsetFail;
        tsp->regions = tsp->regions->next;  /* point to first */
        if (AsnReadVal(aip, atp, &value) < 0)
            goto ToolsetFail;
        if ((atp = AsnReadId(aip, amp, atp)) == NULL)
            goto ToolsetFail;
    }

    if (AsnReadVal(aip, atp, &value) < 0)
        goto ToolsetFail;
    return 0;

  ToolsetFail:
    return -1;
} /* readToolset */

static void 
writeToolset(NIToolsetPtr tsp)
{
    DataVal     value;
    NodePtr     np, lastnode;
    
    if (tsp->host != NULL)
        value.ptrvalue = (Pointer) tsp->host;
    else
        value.ptrvalue = (Pointer) StringSave("N/A");
    AsnWrite(aip, TOOLSET_host, &value);
    
    if (tsp->motd != NULL) {
        value.ptrvalue = (Pointer) tsp->motd;
        AsnWrite(aip, TOOLSET_motd, &value);
    }
    np = tsp->services;
    if (np != NULL) {
        AsnStartStruct(aip, TOOLSET_services);
        lastnode = np->last;
        while (np != NULL) {
            AsnStartStruct(aip, TOOLSET_services_E);
            writeService((NISvcPtr) np->elem);
            AsnEndStruct(aip, TOOLSET_services_E);
            if (np == lastnode)
                break;
            np = np->next;
        }
        AsnEndStruct(aip, TOOLSET_services);
    }
    np = tsp->resources;
    if (np != NULL) {
        AsnStartStruct(aip, TOOLSET_resources);
        lastnode = np->last;
        while (np != NULL) {
            AsnStartStruct(aip, TOOLSET_resources_E);
            writeResource((NIResPtr) np->elem);
            AsnEndStruct(aip, TOOLSET_resources_E);
            if (np == lastnode)
                break;
            np = np->next;
        }
        AsnEndStruct(aip, TOOLSET_resources);
    }
    np = tsp->regions;
    if (np != NULL) {
        AsnStartStruct(aip, TOOLSET_regions);
        lastnode = np->last;
        while (np != NULL) {
            AsnStartStruct(aip, TOOLSET_regions_E);
            writeRegion((NIRegionPtr) np->elem);
            AsnEndStruct(aip, TOOLSET_regions_E);
            if (np == lastnode)
                break;
            np = np->next;
        }
        AsnEndStruct(aip, TOOLSET_regions);
    }
} /* writeToolset */


/******************************************************************************/
/*                                                                            */
/*      Message and Handle Functions                                          */
/*                                                                            */
/******************************************************************************/

/*
 * Purpose:     "High level" message read for ASN.1 messages
 *
 * Parameters:
 *   hp           "Message handle", describing parameters of input environment
 *   unblocked    Flag indicating whether input should "block" if unable
 *                to read a complete message
 *
 * Returns:
 *                NULL, if unable to read a message ... if hp->have_blocked is
 *                  set, then this reflects an inability to read a complete
 *                  message at this time
 *                a pointer to the message which was just read, otherwise
 *
 *
 * Description:
 *              Try to read a message from the socket specified by the hp
 *              data structure. A message must begin with the "MESSAGE" 
 *              identifier, followed by the type of the message. Once the
 *              type of the message has been determined, the remainder of
 *              the message is read using the corresponding readXXX()
 *              function.
 *
 *              Matters are more complex when the "unblocked" parameter
 *              is TRUE. In this case, if a failure occurs on a message read,
 *              an attempt is made to see whether any bytes of data were
 *              read from the socket upon this invocation. If so, the
 *              have_blocked flag is set for the caller, and AsnIoReset() is
 *              called to deal with a future attempt to read from this socket
 *              when more data becomes available.
 *
 * Note:
 *              This is the highest level read function which should be
 *              called by an application program. All the other "static"
 *              functions called here should not be called directly by an
 *              application program.
 *
 *              The ASN.1 error handling is performing using the setjmp()/
 *              longjmp() paradigm, where a function can return to an earlier
 *              context if an error occurs.
 *
 *              Currently, it is not possible to distinguish
 *              between an ASN.1 format error, and an ASN failure due to
 *              inability to read the requested quantity of data. For this
 *              reason, if the "unblocked" option is selected, it will take
 *              hp->r_timeout seconds to detect either of these
 *              conditions. In the best of all possible worlds, it would
 *              be possible to distinguish between "data not YET available"
 *              and "invalid data" conditions.
 */

NLM_EXTERN NIMsgPtr MsgRead(MHandPtr hp, Boolean unblocked)
{
    DataVal     value;
    NIMsgPtr    mp = NULL;
    int         start_byte_count = hp->num_queued_bytes;
    NodePtr     timer = NULL;
    NodePtr     localTimer = NULL;

    ni_errno = NIE_MSGREAD;

    hp->unblocked_mode = unblocked;
    hp->have_blocked = FALSE;

    /* for unblocked I/O, set a "hangup" timeout which can be detected at    */
    /* at later time, if we are unable to read a complete or valid message   */
    if (unblocked && hp->readTimeoutHook != NULL)
        timer = NI_SetTimer(time(NULL) + hp->r_timeout, hp->readTimeoutHook,
                            (VoidPtr) hp);
    hp->readTimer = timer;
        
    if (hp->longjump == TRUE) {
        if (SetJump(ni_env)) {
            /* return from LongJump (error handler) */
            goto MsgReadFail;
        }
    }

    StringCpy(ni_errtext, "AsnReadId or AsnReadVal returned bad value");
    aip = hp->raip;
    atp = MESSAGE;
    if ((atp = AsnReadId(aip, amp, atp)) != MESSAGE)    /* Must read a MESSAGE */
        goto MsgReadFail;       /* don't have to read val as Null return is always fatal to channel */
    if (AsnReadVal(aip, atp, &value) < 0)
        goto MsgReadFail;
    
    if ((atp = AsnReadId(aip, amp, atp)) == NULL)               /* type of message */
        goto MsgReadFail;
    if (AsnReadVal(aip, atp, &value) < 0)
        goto MsgReadFail;
    
    mp = (NIMsgPtr) MemNew(sizeof(NIMessage));
    mp->conid = hp->conid;
    mp->type = NI_UNKNOWN;
    
    if (atp == MESSAGE_ack) {
        if ((mp->msun.ack = readACK()) != NULL)
            mp->type = NI_ACK;
        goto gotmessage;
    }
    
    if (atp == MESSAGE_nack) {
        if ((mp->msun.nack = readNACK()) != NULL)
            mp->type = NI_NACK;
        goto gotmessage;
    }
    
    if (atp == MESSAGE_login) {
        if ((mp->msun.login = readLOGIN()) != NULL)
            mp->type = NI_LOGIN;
        goto gotmessage;
    }
    
    if (atp == MESSAGE_svc_list) {
        if ((mp->msun.svclist = readSVC_LIST()) != NULL)
            mp->type = NI_SVC_LIST;
        goto gotmessage;
    }
    
    if (atp == MESSAGE_svc_request) {
        if ((mp->msun.svcreq = readSVC_REQUEST()) != NULL)
            mp->type = NI_SVC_REQUEST;
        goto gotmessage;
    }
    
    if (atp == MESSAGE_svc_response) {
        if ((mp->msun.svcresp = readSVC_RESPONSE()) != NULL)
            mp->type = NI_SVC_RESPONSE;
        goto gotmessage; 
    }
    
    if (atp == MESSAGE_command) {
        if ((mp->msun.command = readCOMMAND()) != NULL)
            mp->type = NI_COMMAND;
        goto gotmessage;
    }
    
    if (atp == MESSAGE_svc_pre_response) {
        if ((mp->msun.preresp = readPRE_RESPONSE()) != NULL)
            mp->type = NI_SVC_PRE_RESPONSE;
        goto gotmessage;
    }
    
    if (atp == MESSAGE_acct) {
        if ((mp->msun.acct = readACCT()) != NULL)
            mp->type = NI_ACCT;
        goto gotmessage;
    }
    
    if (atp == MESSAGE_catalog) {
        if ((mp->msun.catalog = readCATALOG()) != NULL)
            mp->type = NI_CATALOG;
        goto gotmessage;
    }
    
    if (atp == MESSAGE_load_status) {
        if ((mp->msun.status = readSTATUS()) != NULL)
            mp->type = NI_STATUS;
        goto gotmessage;
    }

  gotmessage:
    if (mp->type == NI_UNKNOWN) {
        StringCpy(ni_errtext, "unknown msg type");
        goto MsgReadFail;
    }
    else { /* got good stuff */
        MsgFreeSavedData(hp); /* free associated data */
        hp->unblocked_mode = FALSE;
        hp->have_blocked = FALSE;
        localTimer = hp->readTimer;
        hp->readTimer = NULL;
        NI_CancelTimer(localTimer);
        return mp;
    }
    
  MsgReadFail:
    if (mp != NULL)
        MsgDestroy(mp);

    /* if ( things didn't go well, but there was more data read from the     */
    /*      socket ) then                                                    */
    if (start_byte_count < hp->num_queued_bytes)
        hp->have_blocked = TRUE; /* set things up to try again */

    if (hp->have_blocked)
    { /* failed due to data unavailability */
        hp->cur_index = 0; /* reset queued read pointer */
        AsnIoReset (aip);  /* reset ASN input stream */
        /* caller should try again later, based upon detecting hp->have_blocked */
    }
    hp->unblocked_mode = FALSE;
    return NULL;
} /* MsgRead */



/*
 * Purpose:     "High level" message write for ASN.1 messages
 *
 * Parameters:
 *   hp           "Message handle", describing parameters of I/O environment
 *   mp           Message to be written
 *   noDestroy    If set, refrain from destroying the message
 *
 * Returns:
 *                -1, if unable to write the message
 *                0, otherwise
 *
 *
 * Description:
 *              Try to write a message from the socket specified by the hp
 *              data structure. The writing of the message is performed
 *              using the writeXXX() function which corresponds to the type
 *              of the message to be written.
 */

NLM_EXTERN Int2 MsgWrite(MHandPtr hp, NIMsgPtr mp, Boolean noDestroy)
{
    DataVal     value;

    ni_errno = NIE_MSGWRITE;

    if (hp->longjump == TRUE) {
        if (SetJump(ni_env)) {
            /* return from LongJump (error handler) */
            if (! noDestroy)
                MsgDestroy(mp);             /* failing, so destroy it */    
            return -1;
        }
    }
    
    aip = hp->waip;
    AsnWrite(aip, MESSAGE, &value);
    
    switch (mp->type) {
      case NI_ACK:
        writeACK(mp->msun.ack);
        break;
        
      case NI_NACK:
        writeNACK(mp->msun.nack);
        break;
        
      case NI_LOGIN:
        writeLOGIN(mp->msun.login);
        break;
        
      case NI_SVC_LIST:
        writeSVC_LIST(mp->msun.svclist);
        break;
        
      case NI_SVC_REQUEST:
        writeSVC_REQUEST(mp->msun.svcreq);
        break;
        
      case NI_SVC_RESPONSE:
        writeSVC_RESPONSE(mp->msun.svcresp);
        break;
        
      case NI_COMMAND:
        writeCOMMAND(mp->msun.command);
        break;
        
      case NI_SVC_PRE_RESPONSE:
        writePRE_RESPONSE(mp->msun.preresp);
        break;
        
      case NI_ACCT:
        writeACCT(mp->msun.acct);
        break;
        
      case NI_CATALOG:
        writeCATALOG(mp->msun.catalog);
        break;
        
      case NI_STATUS:
        writeSTATUS(mp->msun.status);
        break;
        
      default:
        ni_errno = NIE_MSGUNK;
        StringCpy(ni_errtext, "unable to write msg");
        if (! noDestroy)
            MsgDestroy(mp);
        return -1;
    }
    
    AsnIoFlush(aip);
    if (! noDestroy)
        MsgDestroy(mp);
    return 0;
} /* MsgWrite */



/*
 * Purpose:     Build a message of the specified type, populating it with
 *              a pointer to the actual message data, and the connection ID.
 *
 * Parameters:
 *   type         The type of message to be built
 *   conn         Connection ID associated with this session
 *   stp          Pointer to the actual message data
 *
 * Returns:
 *                NULL, if unable to allocate memory for the message, or if
 *                      the specified message type is invalid
 *                a pointer to the newly-build message, otherwise
 *
 *
 * Description:
 *              Create a message data structure, and populate it as indicated.
 */

NLM_EXTERN NIMsgPtr MsgBuild(MsgType type, Uint4 conn, VoidPtr stp)
{
    NIMsgPtr            mp;
    
    if (! InitMsg())
        return NULL;
    mp = (NIMsgPtr) MemNew(sizeof(NIMessage));
    if (mp == NULL)
        return NULL;
    mp->type = type;
    mp->conid = conn;
    
    switch (mp->type) {
      case NI_ACK:
        mp->msun.ack = (NIAckPtr) stp;
        break;
        
      case NI_NACK:
        mp->msun.nack = (NINackPtr) stp;
        break;
        
      case NI_LOGIN:
        mp->msun.login = (NILoginPtr) stp;
        break;
        
      case NI_SVC_LIST:
        mp->msun.svclist = (NISvcListPtr) stp;
        break;
        
      case NI_SVC_REQUEST:
        mp->msun.svcreq = (NISvcReqPtr) stp;
        break;
        
      case NI_SVC_RESPONSE:
        mp->msun.svcresp = (NISvcRespPtr) stp;
        break;
        
      case NI_COMMAND:
        mp->msun.command = (NICmdPtr) stp;
        break;
        
      case NI_SVC_PRE_RESPONSE:
        mp->msun.preresp = (NIPreRespPtr) stp;
        break;
        
      case NI_ACCT:
        mp->msun.acct = (NIAcctPtr) stp;
        break;
        
      case NI_CATALOG:
        mp->msun.catalog = (NICatalogPtr) stp;
        break;
        
      case NI_STATUS:
        mp->msun.status = (NIStatusPtr) stp;
        break;
        
      default:
        ni_errno = NIE_MSGUNK;
        StringCpy(ni_errtext, "unable to build");
        MemFree (mp);
        return NULL;
    }
    return mp;
} /* MsgBuild */



/*
 * Purpose:     Destroy the specified message
 *
 * Parameters:
 *   mp           A pointer to the message structure to be destroyed
 *
 * Returns:
 *                -1, if the specified pointer is NULL, or if the message
 *                    is of an unknown type
 *                0, otherwise
 *
 *
 * Description:
 *              Destroy the specified message, using the appropriate message
 *              destructor function.
 */

NLM_EXTERN Int2 MsgDestroy(NIMsgPtr mp)
{
    if (mp == NULL)
        return -1;
    
    switch (mp->type) {
      case NI_ACK:
        NI_DestroyMsgAck(mp->msun.ack);
        break;
        
      case NI_NACK:
        NI_DestroyMsgNack(mp->msun.nack);
        break;
        
      case NI_LOGIN:
        NI_DestroyMsgLogin(mp->msun.login);
        break;
        
      case NI_SVC_LIST:
        NI_DestroyMsgSvclist(mp->msun.svclist);
        break;
        
      case NI_SVC_REQUEST:
        NI_DestroyMsgSvcreq(mp->msun.svcreq);
        break;
        
      case NI_SVC_RESPONSE:
        NI_DestroyMsgSvcresp(mp->msun.svcresp);
        break;
        
      case NI_COMMAND:
        NI_DestroyMsgCmd(mp->msun.command);
        break;
        
      case NI_SVC_PRE_RESPONSE:
        NI_DestroyMsgPreResp(mp->msun.preresp);
        break;
        
      case NI_ACCT:
        NI_DestroyMsgAcct(mp->msun.acct);
        break;
        
      case NI_CATALOG:
        NI_DestroyMsgCatalog(mp->msun.catalog);
        break;
        
      case NI_STATUS:
        NI_DestroyMsgStatus(mp->msun.status);
        break;
        
      default:
        ni_errno = NIE_MSGUNK;
        StringCpy(ni_errtext, "unable to destroy msg");
        MemFree(mp);
        return -1;
    }

    MemFree(mp);
    return 0;
} /* MsgDestroy */



/*
 * Purpose:     Create a message handle
 *
 * Returns:
 *                NULL, if unable to allocate the required memory, open
 *                    a socket, set the socket to non-blocking mode, or
 *                    initialize the ASN.1 I/O streams successfully
 *                a pointer to the new message handle, otherwise
 *
 *
 * Description:
 *              Create a "message" handle, which is the basic unit of message
 *              I/O. A message handle includes, among other things, a logical
 *              connection ID, a socket used for both input and output, ASN.1
 *              I/O stream pointers, and a queue of input data which has
 *              already been read from the socket, but has not yet been
 *              processed successfully using ASN.1.
 *
 * Note:
 *              This is currently the only place where conid is incremented,
 *              and therefore, the only place where conid is written to the
 *              Conid file.
 */

#ifdef OS_MAC
    /* workaround for weaknesses of Mac OpenTransport */
    Boolean UsingOpenTransport(void);
#endif

NLM_EXTERN MHandPtr MsgMakeHandle(Boolean createSocket)
{
    MHandPtr    mh;
    Boolean     dontset = FALSE;
    
    if (! InitMsg()) {
        StringCpy(ni_errtext, "Network services ASN.1 initialization failed");
        ni_errno = NIE_ASN1SPECFAIL;
        return NULL;
    }
    if ((mh = (MHandPtr) MemNew(sizeof(MHandle))) == NULL) {
        StringCpy(ni_errtext, "unable to allocate new memory");
        return NULL;
    }
    mh->hostname = NULL;
    mh->conid = conid++;
    WriteConFile(conid);
    mh->seqno = 1;
    mh->sok = -1;

    if (createSocket) {
        if ((mh->sok = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) ==
            INVALID_SOCKET) {
#ifdef NETP_INET_NEWT
            SOCK_ERRNO = ABS(mh->sok);
#endif
            StringCpy(ni_errtext, strerror(SOCK_INDEX_ERRNO));
            MemFree(mh);
            return NULL;
        }
        LOG_SOCKET(mh->sok, TRUE);
        if (NI_SETNONBLOCKING(mh->sok) == -1) {
            StringCpy(ni_errtext, strerror(SOCK_INDEX_ERRNO));
            MemFree(mh);
            return NULL;
        }
    }
    mh->r_timeout = NI_READ_TIMEOUT;
    mh->w_timeout = NI_WRITE_TIMEOUT;
    mh->state = NI_CREATED;
    mh->extra_proc_info = NULL;
    mh->read_filter = NULL;
    mh->write_filter = NULL;
    mh->write_filt_pass_thru = TRUE;
    mh->read_filt_pass_thru = TRUE;
    mh->readTimeoutHook = NULL;
    mh->writeTimeoutHook = NULL;
    mh->readTimer = NULL;
    mh->writeTimer = NULL;
    mh->encryption = NULL;
    mh->isBrokered = FALSE;
    mh->brokeredPort = 0;
    mh->peer = 0;
    if ((mh->raip = AsnIoNew((ASNIO_BIN | ASNIO_IN), (FILE *) NULL, (Pointer) mh, NI_AsnRead, NI_AsnWrite)) == NULL) {
        LOG_SOCKET(mh->sok, FALSE);
        NI_CLOSESOCKET(mh->sok);
        StringCpy(ni_errtext, "unable to allocate new AsnIoPtr (raip)");
        MemFree(mh);
        return NULL;
    }
    if ((mh->waip = AsnIoNew((ASNIO_BIN | ASNIO_OUT), (FILE *) NULL, (Pointer) mh, NI_AsnRead, NI_AsnWrite)) == NULL) {
        AsnIoClose(mh->raip);
        LOG_SOCKET(mh->sok, FALSE);
        NI_CLOSESOCKET(mh->sok);
        StringCpy(ni_errtext, "unable to allocate new AsnIoPtr (waip)");
        MemFree(mh);
        return NULL;
    }
#ifdef OS_MAC
    /* workaround for weaknesses of Mac OpenTransport */
    if (UsingOpenTransport())
    {
        AsnIoSetBufsize(mh->waip, NI_BLOCKSIZE / 2);
        dontset = TRUE;
    }
#endif
    if (! dontset)
    {
        AsnIoSetBufsize(mh->waip, NI_BLOCKSIZE);
    }
    mh->longjump = FALSE;
    mh->access_time = time(NULL);
    mh->have_blocked = FALSE;
    mh->num_queued_bytes = 0;
    mh->cur_index = 0;
    mh->queued_data_list = NULL;
    mh->unblocked_mode = FALSE;
    return mh; 
} /* MsgMakeHandle */



/*
 * Purpose:     Destroy the specified message handle
 *
 * Parameters:
 *   hp           A pointer to the message handle to be destroyed
 *
 * Returns:
 *                -1, if the specified pointer is NULL
 *                0, otherwise
 *
 *
 * Description:
 *              Destroy the specified message handle, close its socket,
 *              close the ASN streams, and free any associated queued data.
 */

NLM_EXTERN Int2 MsgDestroyHandle(MHandPtr hp)
{
    if (hp == NULL)
        return -1;
    if (hp->sok != INVALID_SOCKET)
    {
        LOG_SOCKET(hp->sok, FALSE);
        NI_CLOSESOCKET(hp->sok);
    }
    if (hp->hostname != NULL)
        MemFree(hp->hostname);
    NI_CancelTimer(hp->readTimer);
    NI_CancelTimer(hp->writeTimer);
    /* avoid a rare, unexplained condition by zapping the file descriptor */
    if (hp->raip != NULL && hp->raip->fp == (FILE *) -1)
        hp->raip->fp = NULL;
    if (hp->waip != NULL && hp->waip->fp == (FILE *) -1)
        hp->waip->fp = NULL;
    AsnIoClose(hp->raip);
    AsnIoClose(hp->waip);
    if (hp->encryption != NULL)
    {
        NI_DestroyEncrStruct(hp->encryption);
    }
    MsgFreeSavedData(hp);
    MemFree(hp);
    return 0;
} /* MsgDestroyHandle */



/*
 * Purpose:     Set "longjump" error mechanism
 *
 * Parameters:
 *   mh           A pointer to the message handle for which the "longjump"
 *                error is to be set.
 *
 *
 * Description:
 *              Set the ASN I/O error handler to be a function which will
 *              simply "longjump", and hence return control to the place at
 *              which setjmp() was last called. The "longjump" flag should
 *              be check by a setjmp() caller, prior to calling setjmp(), to
 *              assure that the error handling mechanism is in place.
 */

NLM_EXTERN void MsgSetLJError(MHandPtr mh)
{
    AsnIoSetErrorMsg(mh->raip, NI_ASNIOError);
    AsnIoSetErrorMsg(mh->waip, NI_ASNIOError);
    mh->longjump = TRUE;
} /* MsgSetLJError */



/*
 * Purpose:     Set the "read" timeout for this message handle
 *
 * Parameters:
 *   mh           A pointer to the message handle for which the read timeout
 *                is to be set.
 *
 *
 * Description:
 *              Set the "read" timeout for this message handle. A default
 *              value is set-up at the time when the message handle is created.
 *
 * Note:
 *              This timeout is also used for the "hung" timeout.
 */

NLM_EXTERN void MsgSetReadTimeout(MHandPtr mh, int t)
{
    mh->r_timeout = t;
} /* MsgSetReadTimeout */


/*
 * Purpose:     Set the "write" timeout for this message handle
 *
 * Parameters:
 *   mh           A pointer to the message handle for which the write timeout
 *                is to be set.
 *
 *
 * Description:
 *              Set the "write" timeout for this message handle. A default
 *              value is set-up at the time when the message handle is created.
 */

NLM_EXTERN void MsgSetWriteTimeout(MHandPtr mh, int t)
{
    mh->w_timeout = t;
} /* MsgSetWriteTimeout */


/*
 * Purpose:     Set the filter parameters for this message handle
 *
 * Parameters:
 *   mh           A pointer to the message handle for which the parameters
 *                are to be set.
 *
 *   ex_proc      A pointer to an optional user-defined data structure to be
 *                used by the read and write filters.
 *
 *   wfilt        A write filter function, which may be used, e.g., to collect
 *                statistics, or to compress the data to be written.
 *
 *   rfilt        A read filter function, which may be used, e.g., to collect
 *                statistics, or to decompress the data which was read.
 *
 *   wfilt_pass   Indicates whether wfilt needs to modify the data buffer (e.g.,
 *                this will be FALSE if doing statistics only, but TRUE if
 *                doing data compression).
 *
 *   rfilt_pass   Indicates whether rfilt needs to return a modified data
 *                buffer (e.g., this will be FALSE if doing statistics only,
 *                but TRUE if doing data decompression).
 *
 *
 * Description:
 *              Set the I/O filters for this message handle. A default
 *              value (no filter) is set up at the time when the message
 *              handle is created.
 *
 * Note:
 *              It is the responsibility of the higher-level software (i.e.,
 *              not this module) to free any data associated with ex_proc.
 */

NLM_EXTERN void MsgSetFilters(MHandPtr mh, VoidPtr ex_proc, NI_WriteFilt wfilt,
              NI_ReadFilt rfilt, Boolean wfilt_pass, Boolean rfilt_pass)
{
    if (mh == NULL)
        return;

    mh->extra_proc_info = ex_proc;
    mh->write_filter = wfilt;
    mh->read_filter = rfilt;
    mh->write_filt_pass_thru = wfilt_pass;
    mh->read_filt_pass_thru = rfilt_pass;
}


/*
 * Purpose:     Set the timeout hooks for this message handle
 *
 * Parameters:
 *   mh           A pointer to the message handle for which the parameters
 *                are to be set.
 *
 *   rhook        A hook to be called when the read timeout expires
 *
 *   whook        A hook to be called when the write timeout expires
 *
 *
 * Description:
 *              Set the read and write timeout hooks for this message handle.
 *              A default value (no hook) is set up at the time when the
 *              message handle is created.
 *
 * Note:
 *              The write hook is currently usused (6/2/93).
 */

NLM_EXTERN void MsgSetTimeoutHooks(MHandPtr mh, NI_TimeoutHook rhook, NI_TimeoutHook whook)
{
    if (mh == NULL)
        return;

    mh->readTimeoutHook = rhook;
    mh->writeTimeoutHook = whook;
}

/*
 * Purpose:     Initialize the ASN.1 object loader for this module
 *
 * Parameters:
 *   none
 *
 *
 * Description:
 *              Dynamically load the ASN.1 static header, if necessary.
 */

static Boolean
InitMsg (void)
{
    static Boolean loaded = FALSE;

    if (loaded)
        return TRUE;
    if (! AsnLoad()) 
        return FALSE;
    loaded = TRUE;
    return TRUE;
}


/******************************************************************************/
/*                                                                            */
/*      Functions used by AsnTool for socket IO (readfunc and writefunc)      */
/*                                                                            */
/******************************************************************************/

/*
 * Purpose:     Read some data on behalf of the ASN.1 library
 *
 * Parameters:
 *   p            A pointer to the message handle structure
 *   buf          The buffer into which the data should be read
 *   len          Maximum number of bytes to be read
 *
 * Returns:
 *                0, if operating in "unblocked mode", and are unable to
 *                   read the requested amount of data
 *                -ETIMEOUT, if blocked, waiting for data to be available, but
 *                   none arrived before the read timeout expired
 *                -errno [ system error number ], for other errors
 *                the number of bytes read, otherwise
 *
 *
 * Description:
 *              This is the 'readfunc' function used by ASN.1 to read some
 *              data from the specified ASN.1 I/O stream.
 *
 *              First, try to read some data from the "queued data", i.e.,
 *              data which was read from the socket on a previous iteration,
 *              but ASN.1 processing failed because there was insufficient
 *              data.
 *
 *              Subsequently, try to read from the socket. If the data is
 *              unavailable, and running in "blocked" mode, wait for either
 *              the data to appear or for the read timeout to expire.
 *
 *              In the case where data is read successfully, store it in
 *              the "queued data" area.
 *
 *              Once data has been read successfully, optionally post-process
 *              it through a filter routine. This routine may, e.g., collect
 *              statistics or provide a de-compression mechanism. If it is
 *              a de-compression mechanism, any data which would not fit in
 *              the original buffer is stored in the "queued data" area, to
 *              be re-read by the next call to this function.
 */

NLM_EXTERN Int2 LIBCALLBACK NI_AsnRead(Pointer p, CharPtr buf, Uint2 len)
{
    MHandPtr        mh;
    int     bytesread;
    int     ready;
    time_t  secs0, secs1;
    fd_set  rfds;
    struct timeval  timeout;
    CharPtr extra_buf;
    Int4 extra_buf_len;
    CharPtr extra_encr_buf;
    Int4 extra_encr_buf_len;
    NI_NetServHook activityHook;

    mh = (MHandPtr) p;

    /* always provide caller with queued data, if available */
    if (MsgHaveSavedData(mh))
    {
        return MsgReadSavedData(mh, buf, len);
    }

    mh->access_time = time(NULL);

    DisabVibrant();

    while ((bytesread = NI_READSOCKET(mh->sok, buf, len)) <= 0) {
#ifndef NETP_INET_PCNFS
        if (bytesread == 0) {
            break;
        }
#else
        SOCK_ERRNO = tk_geterrno (mh->sok);
#endif
#ifdef NETP_INET_NEWT
        SOCK_ERRNO = ABS(bytesread);
#endif
        switch (SOCK_ERRNO) {
          case EINTR:
            continue;

          case EAGAIN:
#if EAGAIN != EWOULDBLOCK
          case EWOULDBLOCK:
#endif /* EAGAIN != EWOULDBLOCK */
            if (mh->unblocked_mode)
            {
                mh->have_blocked = TRUE;
                EnabVibrant();
                return 0;
            }
            FD_ZERO(&rfds);
            FD_SET(mh->sok, &rfds);
            secs0 = secs1 = time(NULL);

              RepeatSelect:
            /* a simple poll is effected when timeout.tv_sec == 0 */
            timeout.tv_sec = mh->r_timeout - (secs1 - secs0);
            timeout.tv_usec = 0;
            ready = NI_select(mh->sok + 1, &rfds, NULL, NULL, &timeout);
            if (ready > 0)
                continue;
            if (ready == 0)
            {
                EnabVibrant();
                return (-ABS(ETIMEDOUT));
            }
            switch (SOCK_ERRNO) {
              case EINTR:
                /* update the timeout because select() does not */
                secs1 = time(NULL);
                if (secs1 - secs0 > (time_t) mh->r_timeout)
                secs1 = secs0 + mh->r_timeout;
                goto RepeatSelect;

              default:
                break;
            }
            EnabVibrant();
            return (-ABS(SOCK_ERRNO));

          default:
            EnabVibrant();
            return (-ABS(SOCK_ERRNO));
        }
#ifdef WIN16
#ifdef NETP_INET_WSOCK
        {
            MSG msg;

            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
#else
        Yield();
#endif
#endif
    }

    EnabVibrant();

    extra_buf = NULL;
    extra_encr_buf = NULL;
    if (mh->read_filter != NULL)
    {
        if (mh->read_filt_pass_thru)
        {
            mh->read_filter (mh, buf, bytesread, len, NULL, NULL);
        }
        else {
            bytesread = (int) mh->read_filter (mh, buf, bytesread, len,
                                               &extra_buf, &extra_buf_len);
        }
    }

    if (mh->encryption != NULL && mh->encryption->read_filter != NULL)
    {
        bytesread = (int) mh->encryption->read_filter (mh, buf, bytesread,
                                                       len, &extra_encr_buf,
                                                       &extra_encr_buf_len);
    }

    if ((activityHook = NI_ActivityHook()) != NULL)
    {
        activityHook (mh, NetServHook_read, bytesread);
    }

    /* save what we just read */
    MsgSaveData (mh, buf, bytesread);

    if (extra_buf != NULL)
    { /* there was some extra data which we didn't get to read */
       MsgSaveData (mh, extra_buf, (Uint2) extra_buf_len);
       mh->cur_index -= extra_buf_len; /* enqueue data for next read */
       MemFree (extra_buf);
       extra_buf = NULL;
    }

    if (extra_encr_buf != NULL)
    { /* there was some extra encrypted data which we didn't get to read */
       MsgSaveData (mh, extra_encr_buf, (Uint2) extra_encr_buf_len);
       mh->cur_index -= extra_encr_buf_len; /* enqueue data for next read */
       MemFree (extra_encr_buf);
       extra_encr_buf = NULL;
    }

    return bytesread;
} /* NI_AsnRead */



/*
 * Purpose:     Write some data on behalf of the ASN.1 library
 *
 * Parameters:
 *   p            A pointer to the message handle structure
 *   buf          The buffer into which the data should be written
 *   len          Number of bytes to be written
 *
 * Returns:
 *                -ETIMEOUT, if unable to write the data before the write
 *                   timeout expired
 *                -errno [ system error number ], for other errors
 *                the number of bytes written, otherwise
 *
 *
 * Description:
 *              This is the 'writefunc' function used by ASN.1 to write some
 *              data to the specified ASN.1 I/O stream.
 *
 *              If unable to write the data immediately, block until it is
 *              possible to write the data, or until the write timeout expires.
 *
 *              The data to be output is optionally passed through a filter
 *              routine. This routine may, for example, collect statistics
 *              or compress the data to be written.
 *
 * Note:
 *              It is relatively unlikely that "writing" will block, thus
 *              causing the calling application to block. For "writing" to
 *              block, there would need to be flow control imposed in the
 *              TCP/IP protocol suite, which is an unlikely occurrence when
 *              passing small quantities of data.
 */

NLM_EXTERN Int2 LIBCALLBACK NI_AsnWrite(Pointer p, CharPtr buf, Uint2 len)
{
    MHandPtr        mh;
    Int2    byteswrit, bytesleft;
    int     ready;
    fd_set  wfds;
    time_t  secs0, secs1 = 0;
    struct timeval  timeout;
    CharPtr newbuf;
    Uint2 newlen;
    CharPtr tmpbuf = NULL;
    NI_NetServHook activityHook;
    
    mh = (MHandPtr) p;
    mh->access_time = time(NULL);

    newbuf = buf;
    newlen = len;
    if (mh->write_filter != NULL)
    {
        if (mh->write_filt_pass_thru)
        { /* write filter is "pass-through only" */
            mh->write_filter (mh, buf, len, NULL);
        }
        else { /* write filter modifies buffer to be written */
            tmpbuf = (CharPtr) MemNew(len + 5);
            newlen = (Uint2) mh->write_filter(mh, buf, len, tmpbuf);
            newbuf = tmpbuf;
        }
    }
    if (mh->encryption != NULL && mh->encryption->write_filter != NULL)
    {    
        tmpbuf = (CharPtr) MemNew(len + 9);
        newlen = (Uint2) mh->encryption->write_filter(mh, newbuf, newlen, tmpbuf);
        newbuf = tmpbuf;
    }

    if ((activityHook = NI_ActivityHook()) != NULL)
    {
        activityHook (mh, NetServHook_write, newlen);
    }

    DisabVibrant();
    
    for (bytesleft = newlen; bytesleft > 0; bytesleft -= byteswrit,
         newbuf += byteswrit) {
        while ((byteswrit = NI_WRITESOCKET(mh->sok, newbuf, bytesleft)) <= 0) {
        if (byteswrit == 0)
        {
#ifdef NETP_INET_NEWT
            /* this is a real kludge to deal with the fact that NEWT          */
            /* sometimes returns 0 on a send() for no good reason; we simulate*/
            /* that we were told that the send() would block                  */
            byteswrit = -EWOULDBLOCK;
#else
            WriteCleanup(tmpbuf);
            return 0;
#endif /* NETP_INET_NEWT */
        }

#ifdef NETP_INET_NEWT
        SOCK_ERRNO = ABS(byteswrit);
#endif
#ifdef NETP_INET_PCNFS
        SOCK_ERRNO = tk_geterrno (mh->sok);
#endif

        byteswrit = 0; /* don't allow byte count to become messed up */

        switch (SOCK_ERRNO) {
          case EINTR:
            continue;
            
          case EAGAIN:
#if EAGAIN != EWOULDBLOCK
            
          case EWOULDBLOCK:
#endif
#ifdef NETP_INET_NEWT
            /* work-around for bug in version 2.00 of NEWT; a select()*/
            /* will never recognize when it's O.K. to write to a      */
            /* socket                                                 */
            if (secs1 == 0)
            {
                secs1 = secs0 = time(NULL);
            }
            else {
                secs1 = time(NULL);
                if (secs1 - secs0 > (time_t) mh->w_timeout)
                {
                    WriteCleanup(tmpbuf);
                    return (-ABS(ETIMEDOUT));
                }
            }
            break;
#endif
            FD_ZERO(&wfds);
            FD_SET(mh->sok, &wfds);
            secs0 = secs1 = time(NULL);

          RepeatSelect:
            /* a simple poll is effected when timeout.tv_sec == 0 */
            timeout.tv_sec = mh->w_timeout - (secs1 - secs0);
            timeout.tv_usec = 0;
            ready = NI_select(mh->sok + 1, NULL, &wfds, NULL, &timeout);
            if (ready > 0)
                continue;
            if (ready == 0)
            {
                WriteCleanup(tmpbuf);
                return (-ABS(ETIMEDOUT));
            }
            switch (SOCK_ERRNO) {
              case EINTR:
                /* update the timeout because select() does not */
                secs1 = time(NULL);
                if (secs1 - secs0 > (time_t) mh->w_timeout)
                    secs1 = secs0 + mh->w_timeout;
                goto RepeatSelect;
              default:
                break;
            }
            WriteCleanup(tmpbuf);
            return (-ABS(SOCK_ERRNO));

          default:
            WriteCleanup(tmpbuf);
            return (-ABS(SOCK_ERRNO));
        }
#ifdef WIN16
#ifdef NETP_INET_WSOCK
        {
            MSG msg;

            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
#else
        Yield();
#endif
#endif
        }
    }

    WriteCleanup(tmpbuf);
    return (Int2)len;
} /* NI_AsnWrite */


/*
 * Purpose:     Clean-up for NI_AsnWrite
 *
 * Parameters:
 *   tmpbuf       Temporary buffer to be freed, if non-NULL
 *
 *
 * Description:
 *              Frees temporary buffer, and re-enables Vibrant for user
 *              input. Used because there are so many return paths from
 *              NI_AsnWrite.
 *
 */

static void
WriteCleanup (CharPtr tmpbuf)
{
    if (tmpbuf != NULL)
    {
        MemFree (tmpbuf);
    }
    EnabVibrant();
}


/*
 * Purpose:     ASN.1 Error handling
 *
 * Parameters:
 *   level        Error code to be returned
 *   str          Error text generated by ASN.1
 *
 *
 * Description:
 *              This is the error handling function used by ASN.1; it is called
 *              when any ASN.1 I/O error occurs.
 *
 *              When an ASN.1 I/O error occurs, this function stores the
 *              error text in a safe place, and "longjumps", returning
 *              control to a point where setjmp() was last called. This point
 *              should occur in a relatively high-level function (above the
 *              ASN.1 library).
 *
 * Note:
 *              Due to the implementation of setjmp()/longjmp(), if level is
 *              0, then the setjmp() caller will see the value 1; this
 *              mechanism is used to allow a setjmp() caller to distinguish
 *              between the case when they have just "set" the jump environment
 *              (return value == 0), and all other cases.
 */

NLM_EXTERN void LIBCALLBACK NI_ASNIOError(Int2 level, CharPtr str)
{
    StringCpy(ni_errtext, str);
    LongJump(ni_env, level);
} /* NI_ASNIOError */



#if defined(NETP_INET_NEWT) || defined(NETP_INET_WSOCK)

/*
 * Purpose:     Implementation of blocked select() for use with NEWT and WSOCK
 *
 * Parameters:
 *   width        "width" of file descriptor bits maps
 *   rfds         Read file descriptor bit maps
 *   wfds         Write file descriptor bit maps
 *   xfds         Exception file descriptor bit maps
 *
 * Returns:
 *                0, if the timeout period elapses
 *                -1, if an error occurred
 *                the number of available file descriptors (selected file
 *                  descriptors which are available for processing), otherwise
 *
 *
 * Description:
 *              Provide a "blocked" select() mechanism, because NEWT does
 *              not provide one.
 *
 * Note:
 *              When calling select(), the width of the file descriptor
 *              maps is increased by one, to handle an off-by-one error
 *              associated with the NEWT library.
 *
 *              This function is used in WinSock (rather than WinSock's
 *              select function) to ensure that message removal/dispatching
 *              is done in a manner which does not interfere with scrollbar
 *              management.
 */

NLM_EXTERN int NI_poll_select(int width, fd_set PNTR rfds, fd_set PNTR wfds, fd_set PNTR xfds, struct timeval PNTR timeout)
{
    fd_set      trfds, twfds, txfds;
    int         rdy;
    time_t      t_end;
    struct timeval tmout;
    
    if (rfds != NULL)
        Nlm_MemCopy(&trfds, rfds, sizeof(trfds));
    if (wfds != NULL)
        Nlm_MemCopy(&twfds, wfds, sizeof(twfds));
    if (xfds != NULL)
        Nlm_MemCopy(&txfds, xfds, sizeof(txfds));
    
    t_end = timeout->tv_sec + Nlm_GetSecs();
    tmout.tv_sec = 0;
    tmout.tv_usec = 0;
#ifdef NETP_INET_WSOCK
    width--;
#endif
    while  (Nlm_GetSecs() < t_end) {
        if (rfds != NULL)
            Nlm_MemCopy(rfds, &trfds, sizeof(trfds));
        if (wfds != NULL)
            Nlm_MemCopy(wfds, &twfds, sizeof(twfds));
        if (xfds != NULL)
            Nlm_MemCopy(xfds, &txfds, sizeof(txfds));
        
#ifdef NETP_INET_WSOCK
        {
            MSG msg;
            Boolean done = FALSE;

            /* get all the available messages, avoiding WM_LBUTTONUP since  */
            /* this will cause scroll bars to hang, for some unknown reason */
            while (! done)
            {
                done = TRUE;
                if (PeekMessage(&msg, NULL, 0, WM_LBUTTONUP-1, PM_REMOVE))
                {
                    done = FALSE;
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                if (PeekMessage(&msg, NULL, WM_LBUTTONUP+1, 0xffff, PM_REMOVE))
                {
                    done = FALSE;
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
        }
#else
#ifdef WIN16
        Yield();
#endif
#endif
        /* select with (width + 1) because NetManage boundary condition error */
        
        if ((rdy = select((width + 1), rfds, wfds, xfds, &tmout)) != 0) {
            if (rdy < 0) {              /* error */
#ifdef NETP_INET_NEWT
                errno = ABS(rdy);
#endif
                rdy = -1;
            }
            return rdy;
        }

    }
    return 0;   /* timed out */
} /* NI_poll_select */

#endif


#ifdef NI_BLOCK_WITH_FUNCTION


/*
 * Purpose:     Implementation of setting non-blocked-socket mode for platforms
 *              for which it is difficult to perform this functionality within
 *              a macro.
 *
 * Parameters:
 *   fd           Socket file descriptor
 *
 */

NLM_EXTERN int NI_SetNonBlocking (int fd)
{
  int mode = 1;

#ifdef NETP_INET_WSOCK
  u_long lmode = 1;
  return ioctlsocket (fd, FIONBIO, &lmode);
#endif

#ifdef NETP_INET_PCNFS
  return tk_ioctl (fd, FIONBIO, &mode);
#endif
#ifdef NETP_INET_TGV
  return socket_ioctl (fd, FIONBIO, &mode); 
#endif
#ifdef NETP_INET_WPW
  long lmode = 0;
  return ioctl (fd, FIONBIO, &mode); 
#endif
#ifdef NETP_INET_TWG
  /**************************************************************************/
  /* WARNING: THIS IS CURRENTLY NOT IMPLEMENTED PROPERLY BECAUSE WE (W.     */
  /*          GILBERT/J. EPSTEIN) ARE NOT ABLE TO MAKE NETWORK SERVICES     */
  /*          OPERATE PROPERLY IN NON-BLOCKING MODE.  AT SOME FUTURE DATE,  */
  /*          THE REMAINDER OF NETWORK SERVICES SHOULD BE DEBUGGED AND THE  */
  /*          FOLLOWING LINE SHOULD READ: lmode = 1                         */
  /*                                          17 June 1993                  */
  /**************************************************************************/
  /*
  long lmode = 0;
  return ioctl (fd, FIONBIO, &lmode);
 */
  return 0;
#endif
#ifdef NETP_INET_UCX
  /**************************************************************************/
  /* WARNING: THIS IS CURRENTLY UNIMPLEMENTED BECAUSE IT IS NOT CLEAR THAT  */
  /*          IT IS POSSIBLE TO SPECIFY BLOCKING/NONBLOCKING MODE FOR UCX   */
  /*                                          22 June 1993                  */
  /**************************************************************************/
  return 0;
#endif
}

/*
 * Purpose:     Implementation of setting blocked-socket mode for platforms
 *              for which it is difficult to perform this functionality within
 *              a macro.
 *
 * Parameters:
 *   fd           Socket file descriptor
 *
 */

NLM_EXTERN int NI_SetBlocking (int fd)
{
  int mode = 0;

#ifdef NETP_INET_WSOCK
  u_long lmode = 0;
  return ioctlsocket (fd, FIONBIO, &lmode);
#endif

#ifdef NETP_INET_PCNFS
  return tk_ioctl (fd, FIONBIO, &mode);
#endif
#ifdef NETP_INET_TGV
  return socket_ioctl (fd, FIONBIO, &mode);
#endif
#ifdef NETP_INET_WPW
  long lmode = 0;
  return ioctl (fd, FIONBIO, &lmode);
#endif
#ifdef NETP_INET_TWG
  /*
  long lmode = 0;
  return ioctl (fd, FIONBIO, &lmode);
*/
  return 0;
#endif
#ifdef NETP_INET_UCX
  /**************************************************************************/
  /* WARNING: THIS IS CURRENTLY UNIMPLEMENTED BECAUSE IT IS NOT CLEAR THAT  */
  /*          IT IS POSSIBLE TO SPECIFY BLOCKING/NONBLOCKING MODE FOR UCX   */
  /*                                          22 June 1993                  */
  /**************************************************************************/
  return 0;
#endif
}

#endif /* BLOCK_WITH_FUNCTION */


/*
 * Purpose:     Test as to whether there's available data already enqueued
 *
 * Parameters:
 *   mh           Pointer to "message handle" data structure
 *
 * Returns:
 *                FALSE, if there is no data already enqueued
 *                TRUE, otherwise
 *
 *
 * Description:
 *              Determine whether there is currently enqueued data on a
 *              "message handle" data structure.
 */

static Boolean
MsgHaveSavedData(MHandPtr mh)
{
    if (!mh->unblocked_mode)
        return FALSE;
    if (mh->queued_data_list == NULL)
        return FALSE;
    if (mh->num_queued_bytes <= 0)
        return FALSE;
    return (mh->cur_index < mh->num_queued_bytes);
}


/*
 * Purpose:     Read data which was previously stored in this "message handle"
 *
 * Parameters:
 *   mh           Pointer to "message handle" data structure
 *   buf          Buffer where data should be stored
 *   len          Maximum number of bytes to read out
 *
 * Returns:
 *                the number of bytes read from the enqueued data list
 *
 *
 * Description:
 *              Return up to len bytes of enqueued data, by traversing
 *              the enqueued data structure, beginning with the location
 *              where the "current byte" pointer is currently pointing.
 *
 * Note:
 *              The enqueued data is stored in a list of chained blocks, where
 *              at all times, all the blocks are full, except (possibly) the
 *              last block.
 */

#define MSG_READ_BLOCK_SIZE 1024

static int
MsgReadSavedData(MHandPtr mh, CharPtr buf, Uint2 len)
{
    NodePtr startpt;
    NodePtr blk;
    int     addl_queue_data;
    int     local_index;
    int     bytes_copied;
    int     data_in_this_block;
    int     bytes_to_copy;

    if (mh->queued_data_list == NULL || len <= 0 || mh->num_queued_bytes == 0)
        return 0;
    blk = startpt = mh->queued_data_list;
    addl_queue_data = mh->num_queued_bytes - mh->cur_index;
    local_index = 0;
    bytes_copied = 0;

    /* traverse the portion of the list which is necessary to get past */
    /* the current "file" pointer, and then copy up to len bytes of    */
    /* data from the remaining blocks                                  */
    do {
        /* if ( this is the last block ) then */
        if (mh->num_queued_bytes - local_index < MSG_READ_BLOCK_SIZE)
            data_in_this_block = mh->num_queued_bytes % MSG_READ_BLOCK_SIZE;
        else
            data_in_this_block = MSG_READ_BLOCK_SIZE;

        /* if ( there is more data to be skipped over ) then */
        if (local_index < mh->cur_index)
        { /* there is more data to be skipped over */
            if (local_index + data_in_this_block <= mh->cur_index)
            { /* skip this block */
                local_index += data_in_this_block;
                continue;
            }
            else { /* skip a portion of this block, and copy the remainder */
                data_in_this_block -= (mh->cur_index - local_index);
                local_index = mh->cur_index;
            }
        }

        bytes_to_copy = MIN((int) len, data_in_this_block);
        MemCopy(buf, blk->elem, bytes_to_copy);
        buf += bytes_to_copy;
        len -= bytes_to_copy;
        bytes_copied += bytes_to_copy;
        mh->cur_index += bytes_to_copy;
        local_index += bytes_to_copy;
    } while ( (blk = ListGetNext(blk)) != startpt && len > 0);

    return bytes_copied;
}



/*
 * Purpose:     Store len bytes of data from a buffer, for future use
 *
 * Parameters:
 *   mh           Pointer to "message handle" data structure
 *   buf          Buffer of data to be copied
 *   len          Number of bytes to save
 *
 *
 * Description:
 *              Store len bytes of data at the end of a list of "enqueued data."
 *              After finding the current end of the queue, data is added
 *              to the final block (up to its capacity), and then, if needed,
 *              additional blocks are allocated to store the remaining data.
 */

static void
MsgSaveData(MHandPtr mh, CharPtr buf, Uint2 len)
{
    NodePtr tail;
    int     bytes_to_copy;
    int     starting_byte;
    CharPtr newdata;

    if (len == 0 || !mh->unblocked_mode)
        return;

    /* get to the correct place in the list, and then start storing */

    tail = mh->queued_data_list;

    /* point to tail of list; in a ring, the element before the head is the */
    /* tail                                                                 */
    if (tail != NULL)
        tail = tail->last;

    /* if ( more room in the current element ) then */
    if ((starting_byte = mh->num_queued_bytes % MSG_READ_BLOCK_SIZE) != 0)
    {
        bytes_to_copy = MIN(MSG_READ_BLOCK_SIZE - starting_byte, (int) len);
        newdata = (CharPtr) tail->elem;
        MemCopy (&newdata[starting_byte], buf, bytes_to_copy);
        len -= bytes_to_copy;
        buf += len;
        mh->num_queued_bytes += bytes_to_copy;
    }

    /* while ( there is more data to be copied ) */
    while (len > 0)
    { /* create a new element, and copy data into that element */

        newdata = MemNew(MSG_READ_BLOCK_SIZE);
        bytes_to_copy = MIN(MSG_READ_BLOCK_SIZE, len);
        MemCopy (newdata, buf, bytes_to_copy);
        buf += bytes_to_copy;
        len -= bytes_to_copy;
        mh->num_queued_bytes += bytes_to_copy;
        tail = ListInsert(newdata, tail);
    }

    mh->cur_index = mh->num_queued_bytes;

    if (tail == NULL)
        mh->queued_data_list = NULL;
    else
        mh->queued_data_list = tail->next;
}



/*
 * Purpose:     Free data associated with an "enqueued data list"
 *
 * Parameters:
 *   mh           Pointer to "message handle" data structure
 *
 *
 * Description:
 *              Free all the message elements associated with a list of
 *              enqueued data, and set other message handle elements to
 *              appropriate values such that software accessing those structures
 *              will not, erroneously, think that there is valid enqueued data
 *              in the structure.
 */

static void
MsgFreeSavedData (MHandPtr mh)
{
    NodePtr np;
    CharPtr elem;

    mh->unblocked_mode = FALSE;
    mh->have_blocked = FALSE;
    mh->num_queued_bytes = 0;
    mh->cur_index = 0;

    np = mh->queued_data_list;

    mh->queued_data_list = NULL;

    while (np != NULL)
    {
        elem = (CharPtr) np->elem;
        if (elem != NULL)
            MemFree (elem);
        np = ListDelete(np);
    }
}


/*
 * Purpose:     Set and pop the hour-glass cursor
 *
 * Parameters:
 *   none
 *
 * Description:
 *              According to the NEWT/NetManage documentation, it is
 *              beneficial to make the cursor into an hourglass to block-out
 *              unwanted events during some socket calls.
 */

#if defined(NETP_INET_NEWT) || defined(NETP_INET_WSOCK)
static HCURSOR saveCursor = NULL;

static void SetHourGlass(void)
{
#ifdef NETP_INET_NEWT
   static HCURSOR hourCursor;

   hourCursor = LoadCursor(NULL, IDC_WAIT);
   saveCursor = SetCursor(hourCursor);
#endif
}

static void PopHourGlass(void)
{
#ifdef NETP_INET_NEWT
   if (saveCursor != NULL)
      SetCursor(saveCursor);
#endif
}
#endif /* NETP_INET_NEWT */

