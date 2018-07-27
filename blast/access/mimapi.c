/*   mimapi.c
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
* File Name:  mimapi.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   8/16/00
*
* $Revision: 1.3 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <urlquery.h>
#include <mimapi.h>

/* low-level connection functions */

NLM_EXTERN CONN MimOpenConnection (
  Int4 uid
)

{
  Char  query [64];

  if (uid < 1) return NULL;

  sprintf (query, "cmd=ASN.1&id=%ld", (long) uid);
  return QUERY_OpenUrlQuery ("www.ncbi.nlm.nih.gov", 80, "/entrez/dispomim.cgi",
                             query, "Entrez2Tool", 30, eMIME_T_NcbiData,
                             eMIME_AsnText, eENCOD_None, 0);
}

#ifdef OS_MAC
#include <Events.h>
#endif

NLM_EXTERN MimEntryPtr MimWaitForReply (
  CONN conn
)

{
  AsnIoConnPtr  aicp;
  time_t        currtime, starttime;
  Int2          max = 0;
  MimEntryPtr   mep = NULL;
  EIO_Status    status;
  STimeout      timeout;
#ifdef OS_MAC
  EventRecord   currEvent;
#endif

  if (conn == NULL) return NULL;

#ifdef OS_MAC
  timeout.sec = 0;
  timeout.usec = 0;
#else
  timeout.sec = 100;
  timeout.usec = 0;
#endif

  starttime = GetSecs ();
  while ((status = CONN_Wait (conn, eIO_Read, &timeout)) != eIO_Success && max < 300) {
    currtime = GetSecs ();
    max = currtime - starttime;
#ifdef OS_MAC
    WaitNextEvent (0, &currEvent, 0, NULL);
#endif
  }
  if (status == eIO_Success) {
    aicp = QUERY_AsnIoConnOpen ("r", conn);
    mep = MimEntryAsnRead (aicp->aip, NULL);
    QUERY_AsnIoConnClose (aicp);
  }
  CONN_Close (conn);

  return mep;
}

/* high-level connection functions */

NLM_EXTERN MimEntryPtr MimSynchronousQuery (
  Int4 uid
)

{
  CONN         conn;
  MimEntryPtr  mep;

  conn = MimOpenConnection (uid);

  if (conn == NULL) return NULL;

  QUERY_SendQuery (conn);

  mep = MimWaitForReply (conn);

  return mep;
}

NLM_EXTERN Boolean MimAsynchronousQuery (
  Int4 uid,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
)

{
  CONN  conn;

  conn = MimOpenConnection (uid);

  if (conn == NULL) return FALSE;

  QUERY_SendQuery (conn);

  QUERY_AddToQueue (queue, conn, resultproc, userdata, TRUE);

  return TRUE;
}

NLM_EXTERN Int4 MimCheckQueue (
  QUEUE* queue
)

{
  return QUERY_CheckQueue (queue);
}

NLM_EXTERN MimEntryPtr MimReadReply (
  CONN conn,
  EIO_Status status
)

{
  AsnIoConnPtr  aicp;
  MimEntryPtr   mep = NULL;

  if (conn != NULL && status == eIO_Success) {
    aicp = QUERY_AsnIoConnOpen ("r", conn);
    mep = MimEntryAsnRead (aicp->aip, NULL);
    QUERY_AsnIoConnClose (aicp);
  }
  return mep;
}

