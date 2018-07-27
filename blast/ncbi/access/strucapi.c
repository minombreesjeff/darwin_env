/*   strucapi.c
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
* File Name:  strucapi.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   8/18/00
*
* $Revision: 1.17 $
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
#include <strucapi.h>

/* low-level connection functions */

NLM_EXTERN CONN StrucOpenConnection (
  Int4 uid
)

{
  Char        query [256];
  /*
  CONN        conn;
  size_t      n_written;
  EIO_Status  status;
  */

  if (uid < 1) return NULL;

  /*
  sprintf (query, "uid=%ld%s", (long) uid,
           "&save=asntext&form=6&db=t&Dopt=j&Complexity=Cn3D%20Subset");

  conn = QUERY_OpenUrlQuery ("www.ncbi.nlm.nih.gov", 80, "/Structure/mmdb/mmdbsrv.cgi",
                             NULL, "Entrez2Tool", 30, eMIME_T_NcbiData,
                             eMIME_WwwForm, eENCOD_Url, 0);
 
  status = CONN_Write (conn, (const void *) query, StringLen (query), &n_written);
  if (status != eIO_Success) {
    CONN_Close (conn);
    return NULL;
  }

  return conn;
  */

  sprintf (query, "uid=%ld%s", (long) uid,
           "&save=asntext&form=6&db=t&Dopt=j&Complexity=Cn3D%20Subset");
  return QUERY_OpenServiceQuery ("StrucFetch", query, 30);
}

NLM_EXTERN BiostrucSeqPtr StrucWaitForReply (
  CONN conn
)

{
  BiostrucSeqPtr  bsp = NULL;
  time_t          currtime, starttime;
  Int2            max = 0;
  EIO_Status      status;
  STimeout        timeout;

  if (conn == NULL) return NULL;

#ifdef OS_MAC
  timeout.sec = 0;
  timeout.usec = 0;
#else
  timeout.sec = 100;
  timeout.usec = 0;
#endif

  starttime = GetSecs ();
  while ((status = CONN_Wait (conn, eIO_Read, &timeout)) == eIO_Timeout && max < 300) {
    currtime = GetSecs ();
    max = currtime - starttime;
#ifdef OS_MAC
    QUERY_WaitForNextMacEvent ();
#endif
  }
  if (status == eIO_Success) {
    /*
    {
      FILE *fp = FileOpen ("dummy", "w");
      QUERY_CopyResultsToFile (conn, fp);
      FileClose (fp);
    }
    */
    bsp = StrucReadReply (conn, status);
  }
  CONN_Close (conn);

  return bsp;
}

/* high-level connection functions */

NLM_EXTERN BiostrucSeqPtr StrucSynchronousQuery (
  Int4 uid
)

{
  BiostrucSeqPtr  bsp;
  CONN            conn;

  conn = StrucOpenConnection (uid);

  if (conn == NULL) return NULL;

  QUERY_SendQuery (conn);

  bsp = StrucWaitForReply (conn);

  return bsp;
}

NLM_EXTERN Boolean StrucAsynchronousQuery (
  Int4 uid,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
)

{
  CONN  conn;

  conn = StrucOpenConnection (uid);

  if (conn == NULL) return FALSE;

  QUERY_SendQuery (conn);

  QUERY_AddToQueue (queue, conn, resultproc, userdata, TRUE);

  return TRUE;
}

NLM_EXTERN Int4 StrucCheckQueue (
  QUEUE* queue
)

{
  return QUERY_CheckQueue (queue);
}

NLM_EXTERN BiostrucSeqPtr StrucReadReply (
  CONN conn,
  EIO_Status status
)

{
  AsnIoConnPtr     aicp;
  BiostrucSeqPtr   bsp = NULL;
  NcbiMimeAsn1Ptr  nmap = NULL;

  if (conn != NULL && status == eIO_Success) {
    aicp = QUERY_AsnIoConnOpen ("r", conn);
    nmap = NcbiMimeAsn1AsnRead (aicp->aip, NULL);
    QUERY_AsnIoConnClose (aicp);
    if (nmap != NULL && nmap->choice == NcbiMimeAsn1_strucseq) {
      bsp = (BiostrucSeqPtr) nmap->data.ptrvalue;
    }
  }
  return bsp;
}

