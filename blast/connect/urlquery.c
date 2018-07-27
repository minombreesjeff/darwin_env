/*   urlquery.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  urlquery.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   4/16/98
*
* $Revision: 6.18 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: urlquery.c,v $
* Revision 6.18  2001/09/25 13:20:56  lavr
* SERVICE_CreateConnectorEx() - number of args adjusted
*
* Revision 6.17  2001/08/16 18:07:04  kans
* protect against calling CONN_Read or CONN_Write with NULL conn parameter
*
* Revision 6.16  2001/06/07 20:17:34  kans
* in QUERY_OpenServiceQuery, pass service to ConnNetInfo_Create
*
* Revision 6.15  2001/06/07 20:07:41  kans
* added QUERY_OpenServiceQuery
*
* Revision 6.14  2001/04/25 15:14:27  lavr
* SConnNetInfo::timeout is now a pointer
*
* Revision 6.13  2001/02/25 21:42:27  kans
* changed several Uint4s to size_t due to new prototypes
*
* Revision 6.12  2001/02/21 22:02:04  lavr
* Changes for use new CONN interface
*
* Revision 6.11  2000/08/18 19:08:58  kans
* added QUERY_WaitForNextMacEvent, otherwise QuickDraw collides with mmdbapi
*
* Revision 6.10  2000/06/30 18:16:09  kans
* protect against reentrant calls if resultproc is GUI and processes timer - showed up on PC/Windows, not Mac or UNIX version of Sequin
*
* Revision 6.9  2000/06/30 12:46:10  kans
* added QUERY_CloseQueue
*
* Revision 6.8  2000/06/29 18:27:10  kans
* QUERY_OpenUrlQuery has new EMIME_Type type and EMIME_Encoding encoding parameters
*
* Revision 6.7  2000/06/13 12:58:14  kans
* added closeConn parameter to QUERY_AddToQueue
*
* Revision 6.6  1999/07/28 21:09:15  vakatov
* Multiple fixes in QUERY_OpenUrlQuery() to make it work with a generic
* URL server;  also, pass arguments in the cmd.-line
*
* ==========================================================================
*/

#include "asnbuild.h"
#include <urlquery.h>

#ifdef OS_MAC
#include <Events.h>

NLM_EXTERN void QUERY_WaitForNextMacEvent (void)

{
  EventRecord  currEvent;

  WaitNextEvent (0, &currEvent, 0, NULL);
}
#endif

NLM_EXTERN CONN QUERY_OpenUrlQuery (
  Nlm_CharPtr host_machine, Nlm_Uint2 host_port,
  Nlm_CharPtr host_path, Nlm_CharPtr arguments,
  Nlm_CharPtr appName, Nlm_Uint4 timeoutsec,
  EMIME_Type type, EMIME_SubType subtype,
  EMIME_Encoding encoding, THCC_Flags flags)
{
  CONN            conn = 0;
  CONNECTOR       connector;
  Nlm_Char        contentType[MAX_CONTENT_TYPE_LEN];
  SConnNetInfo*   info;
  Nlm_Char        path [PATH_MAX];
  EIO_Status      status;
  Nlm_CharPtr     userAgentName = NULL;
  Nlm_Char        user_header[sizeof(contentType) + 256];

  if (StringHasNoText (host_machine) || StringHasNoText (host_path))
    return NULL;

  /* allow the user to specify a prog. name, otherwise get from ProgramPath */
  if (! StringHasNoText (appName)) {
    userAgentName = appName;
  } else {
    Nlm_ProgramPath (path, sizeof (path));
    userAgentName = StringRChr (path, DIRDELIMCHR);
    if (userAgentName != NULL) {
      userAgentName++;
    }
  }
  if (StringHasNoText (userAgentName)) {
    userAgentName = "?";
  }

  /* was set content type from parameters - now explicit parameter */
  /*
  if ((flags & fHCC_UrlEncodeOutput) != 0) {
    encoding = eENCOD_Url;
  }
  */
  VERIFY( MIME_ComposeContentTypeEx(type, subtype, encoding,
                                  contentType, sizeof(contentType)) );

  /* set HTML header with program name as user agent */
  sprintf (user_header, "%sUser-Agent: %s\r\n", contentType, userAgentName);

  /* fill in connection info fields and create the connection */
  info = ConnNetInfo_Create(0);
  ASSERT( info );

  if ( host_machine ) {
      StringNCpy_0(info->host, host_machine, sizeof(info->host));
  }
  info->port = host_port;
  if ( !StringHasNoText(arguments) ) {
      StringNCpy_0(info->args, arguments, sizeof(info->args));
  }
  StringNCpy_0(info->path, host_path, sizeof(info->path));

  if ( info->timeout ) {
      info->timeout->sec  = timeoutsec;
      info->timeout->usec = 0;
  }

  connector = HTTP_CreateConnector(info, user_header, flags);
  status = CONN_Create(connector, &conn);
  if (status != eIO_Success) {
    ErrPostEx(SEV_ERROR, 0, 0, "QUERY_OpenUrlQuery failed in CONN_Create");
  }

  /* cleanup & return */
  ConnNetInfo_Destroy(info);
  return conn;
}


NLM_EXTERN CONN QUERY_OpenServiceQuery (
  Nlm_CharPtr service, Nlm_CharPtr arguments, Nlm_Uint4 timeoutsec
)

{
  CONN            conn = 0;
  CONNECTOR       connector;
  SConnNetInfo*   info;
  EIO_Status      status;

  /* fill in connection info fields and create the connection */
  info = ConnNetInfo_Create (service);
  ASSERT( info );

  if ( !StringHasNoText (arguments) ) {
      StringNCpy_0 (info->args, arguments, sizeof(info->args));
  }

  if ( info->timeout ) {
      info->timeout->sec  = timeoutsec;
      info->timeout->usec = 0;
  }

  connector = SERVICE_CreateConnectorEx (service, fSERV_Any, info, 0);
  status = CONN_Create (connector, &conn);
  if (status != eIO_Success) {
    ErrPostEx (SEV_ERROR, 0, 0, "QUERY_OpenUrlQuery failed in CONN_Create");
  }

  /* cleanup & return */
  ConnNetInfo_Destroy (info);
  return conn;
}


NLM_EXTERN void QUERY_SendQuery (
  CONN conn
)

{
  STimeout  timeout;

  if (conn == 0) return;

  /* flush buffer, sending query, without waiting for response */

  timeout.sec  = 0;
  timeout.usec = 0;
  CONN_Wait (conn, eIO_Read, &timeout);
}

#define URL_QUERY_BUFLEN  4096

NLM_EXTERN void QUERY_CopyFileToQuery (
  CONN conn, FILE *fp
)

{
  Nlm_CharPtr  buffer;
  size_t       ct;
  size_t       n_written;
  EIO_Status   status;

  if (conn == NULL || fp == NULL) return;

  buffer = (Nlm_CharPtr) MemNew(URL_QUERY_BUFLEN + 1);
  if (buffer != NULL) {
    while ((ct = FileRead (buffer, 1, URL_QUERY_BUFLEN, fp)) > 0) {
      status = CONN_Write (conn, (const void *) buffer, ct, &n_written);
    }
  }
  MemFree (buffer);
}

NLM_EXTERN void QUERY_CopyResultsToFile (
  CONN conn, FILE *fp
)

{
  Nlm_CharPtr  buffer;
  size_t       n_read;
  EIO_Status   status;

  if (conn == NULL || fp == NULL) return;

  buffer = (Nlm_CharPtr) MemNew(URL_QUERY_BUFLEN + 1);
  if (buffer != NULL) {
      while ((status = CONN_Read (conn, buffer, URL_QUERY_BUFLEN, &n_read,
                                  eIO_Plain)) == eIO_Success) {
          FileWrite (buffer, 1, n_read, fp);
      }
  }
  MemFree (buffer);
}

static Nlm_Int2 LIBCALL AsnIoConnWrite (Pointer ptr, Nlm_CharPtr buf, Nlm_Uint2 count)

{
	size_t        bytes;
	AsnIoConnPtr  aicp;

	aicp = (AsnIoConnPtr) ptr;
	if (aicp == NULL || aicp->conn == NULL) return 0;
	CONN_Write (aicp->conn, (const void *) buf, (size_t) count, &bytes);
	return (Nlm_Int2) bytes;
}

static Nlm_Int2 LIBCALL AsnIoConnRead (Pointer ptr, CharPtr buf, Nlm_Uint2 count)

{
	size_t        bytes;
	AsnIoConnPtr  aicp;

	aicp = (AsnIoConnPtr) ptr;
	if (aicp == NULL || aicp->conn == NULL) return 0;
	CONN_Read (aicp->conn, (Pointer) buf, (Int4) count, &bytes, eIO_Plain);
	return (Nlm_Int2) bytes;
}

NLM_EXTERN AsnIoConnPtr QUERY_AsnIoConnOpen (Nlm_CharPtr mode, CONN conn)

{
  Int1          type;
  AsnIoConnPtr  aicp;

  if (! StringCmp(mode, "r"))
    type = (ASNIO_IN | ASNIO_TEXT);
  else if (! StringCmp(mode, "rb"))
    type = (ASNIO_IN | ASNIO_BIN);
  else if (! StringCmp(mode, "w"))
    type = (ASNIO_OUT | ASNIO_TEXT);
  else if (! StringCmp(mode, "wb"))
    type = (ASNIO_OUT | ASNIO_BIN);
  else
  {
    AsnIoErrorMsg (NULL, 81, mode);
    return NULL;
  }

  aicp = (AsnIoConnPtr) MemNew (sizeof (AsnIoConn));
  aicp->aip = AsnIoNew (type, NULL, (Pointer) aicp, AsnIoConnRead, AsnIoConnWrite);
  aicp->conn = conn;
  return aicp;
}

NLM_EXTERN AsnIoConnPtr QUERY_AsnIoConnClose (AsnIoConnPtr aicp)

{
  if (aicp == NULL) return NULL;
  AsnIoClose (aicp->aip);
  return (AsnIoConnPtr) MemFree (aicp);
}

typedef struct SQueueTag {
  CONN                conn;
  QueryResultProc     resultproc;
  Nlm_VoidPtr         userdata;
  Nlm_Boolean         closeConn;
  Nlm_Boolean         protect;
  struct SQueueTag*   next;
} SConnQueue, PNTR QueuePtr;

NLM_EXTERN void QUERY_AddToQueue (
  QUEUE* queue, CONN conn, QueryResultProc resultproc, Nlm_VoidPtr userdata, Nlm_Boolean closeConn
)

{
  QueuePtr       cqp;
  QueuePtr PNTR  qptr;
  QueuePtr       tmp;

  if (conn == NULL || resultproc == NULL) return;

  /* allocate queue element */

  cqp = (QueuePtr) MemNew (sizeof (SConnQueue));
  if (cqp == NULL) return;

  cqp->conn = conn;
  cqp->resultproc = resultproc;
  cqp->userdata = userdata;
  cqp->closeConn = closeConn;
  cqp->protect = FALSE;

  /* add to polling queue */

  qptr = (QueuePtr PNTR) queue;
  if (qptr != NULL) {
    if (*qptr != NULL) {
      tmp = *qptr;
      if (tmp != NULL) {
        while (tmp->next != NULL) {
          tmp = tmp->next;
        }
        tmp->next = cqp;
      }
    } else {
      *qptr = cqp;
    }
  }
}

static void QUERY_RemoveFromQueue (
  QUEUE* queue, CONN conn
)

{
  QueuePtr       curr;
  QueuePtr       next;
  QueuePtr PNTR  prev;
  QueuePtr PNTR  qptr;

  qptr = (QueuePtr PNTR) queue;
  if (qptr == NULL || *qptr == NULL || conn == NULL) return;

  prev = qptr;
  curr = *qptr;

  while (curr != NULL) {
    next = curr->next;
    if (curr->conn == conn) {
      *(prev) = next;
      curr->next = NULL;
      MemFree (curr);
    } else {
      prev = &(curr->next);
    }
    curr = next;
  }
}

NLM_EXTERN Nlm_Int4 QUERY_CheckQueue (
  QUEUE* queue
)

{
  Nlm_Int4       count = 0;
  QueuePtr       curr;
  QueuePtr       next;
  QueuePtr PNTR  qptr;
  EIO_Status     status;
  STimeout       timeout;

  qptr = (QueuePtr PNTR) queue;
  if (qptr == NULL || *qptr == NULL) return 0;

  curr = *qptr;

  while (curr != NULL) {
    next = curr->next;

    if (curr->conn != NULL && (! curr->protect)) {
      timeout.sec  = 0;
      timeout.usec = 0;
      status = CONN_Wait (curr->conn, eIO_Read, &timeout);

      if (status == eIO_Success) {
        /* protect against reentrant calls if resultproc is GUI and processes timer */
        curr->protect = TRUE;
        if (curr->resultproc != NULL) {
          /* result could eventually be used to reconnect on timeout */
          curr->resultproc (curr->conn, curr->userdata, status);
        }
        if (curr->closeConn) {
          CONN_Close (curr->conn);
        }
        QUERY_RemoveFromQueue (queue, curr->conn);

      } else {
        count++;
      }
    }

    curr = next;
  }

  return count;
}

NLM_EXTERN void QUERY_CloseQueue (
  QUEUE* queue
)

{
  QueuePtr       curr;
  QueuePtr       next;
  QueuePtr PNTR  qptr;

  qptr = (QueuePtr PNTR) queue;
  if (qptr == NULL || *qptr == NULL) return;

  curr = *qptr;

  while (curr != NULL) {
    next = curr->next;

    if (curr->conn != NULL) {
      CONN_Close (curr->conn);
      QUERY_RemoveFromQueue (queue, curr->conn);
    }

    curr = next;
  }
}

