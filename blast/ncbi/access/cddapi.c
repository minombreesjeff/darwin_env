/*   cddapi.c
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
* File Name:  cddapi.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   8/16/00
*
* $Revision: 1.11 $
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
#include <cddapi.h>
#include <seqport.h>
#include <sqnutils.h>

/* low-level connection functions */

NLM_EXTERN CONN CddOpenConnection (
  BioseqPtr bsp,
  FloatHi expectValue,
  Boolean findBest,
  Boolean doFilter,
  Boolean makeFeats,
  CharPtr dataLib,
  Boolean precalc
)

{
  CONN        conn;
  Char        expect [32];
  Char        feats [16];
  Char        filter [8];
  Char        graph [8];
  Char        id [42];
  Char        inputtype [32];
  size_t      len;
  CharPtr     query;
  SeqIdPtr    sip;
  CharPtr     str;
  /*
  size_t      n_written;
  EIO_Status  status;
  */

  if (bsp == NULL) return NULL;
  if (! ISA_aa (bsp->mol)) return NULL;

  str = GetSequenceByBsp (bsp);
  if (StringHasNoText (str)) return NULL;
  len = StringLen (str);
  query = (CharPtr) MemNew (len + 128);
  if (query == NULL) {
    MemFree (str);
    return NULL;
  }
  sprintf (expect, "%7.4lf", (double) expectValue);
  TrimSpacesAroundString (expect);

  if (findBest) {
    StringCpy (graph, "1");
  } else {
    StringCpy (graph, "2");
  }

  if (doFilter) {
    StringCpy (filter, "T");
  } else {
    StringCpy (filter, "F");
  }

  if (makeFeats) {
    StringCpy (feats, "AONLY");
  } else {
    StringCpy (feats, "LONLY");
  }

  if (StringHasNoText (dataLib)) {
    dataLib = "cdd";
  }

  if (precalc) {
    StringCpy (inputtype, "INPUT_TYPE=precalc&FULL&");
  } else {
    StringCpy (inputtype, "");
  }

  sip = SeqIdFindBest (bsp->id, 0);
  if (precalc) {
    SeqIdWrite (sip, id, PRINTID_REPORT, sizeof (id) - 1);
  } else {
    SeqIdWrite (sip, id, PRINTID_FASTA_SHORT, sizeof (id) - 1);
  }
  if (StringHasNoText (id)) {
    StringCpy (id, "tmpseq_0");
  }

  /*
  sprintf (query, "%s&NOHTML&DATALIB=%s&EXPECT=%s&FILTER=%s&GRAPH=%s&SEQUENCE=>%s\n%s", feats, dataLib, expect, filter, graph, id, str);
  conn = QUERY_OpenUrlQuery ("www.ncbi.nlm.nih.gov", 80, "/Structure/cdd/wrpsb.cgi",
                             NULL, "Entrez2Tool", 30, eMIME_T_NcbiData,
                             eMIME_Plain, eENCOD_None, 0);
  status = CONN_Write (conn, (const void *) query, StringLen (query), &n_written);
  if (status != eIO_Success) {
    CONN_Close (conn);
    MemFree (str);
    MemFree (query);
    return NULL;
  }
  */

  if (precalc) {
    sprintf (query, "%s&NOHTML&DATALIB=%s&EXPECT=%s&FILTER=%s&GRAPH=%s&%sSEQUENCE=%s",
             feats, dataLib, expect, filter, graph, inputtype, id);
  } else {
    sprintf (query, "%s&NOHTML&DATALIB=%s&EXPECT=%s&FILTER=%s&GRAPH=%s&%sSEQUENCE=>%s\n%s",
             feats, dataLib, expect, filter, graph, inputtype, id, str);
  }
  conn = QUERY_OpenServiceQuery ("CddSearch", query, 30);

  MemFree (str);
  MemFree (query);
  return conn;
}

NLM_EXTERN SeqAnnotPtr CddReadReply (
  CONN conn,
  EIO_Status status
)

{
  AsnIoConnPtr  aicp;
  SeqAnnotPtr   sap = NULL;

  if (conn != NULL && status == eIO_Success) {
    aicp = QUERY_AsnIoConnOpen ("r", conn);
    if (aicp == NULL || aicp->aip == NULL) return NULL;

    /*
    aicp->aip->scan_for_start = TRUE;
    */

    sap = SeqAnnotAsnRead (aicp->aip, NULL);
    QUERY_AsnIoConnClose (aicp);

    if (sap != NULL && sap->data == NULL) {
      sap = SeqAnnotFree (sap);
    }
  }
  return sap;
}

#ifdef OS_MAC
#include <Events.h>
#endif

NLM_EXTERN SeqAnnotPtr CddWaitForReply (
  CONN conn
)

{
  time_t        currtime, starttime;
  Int2          max = 0;
  SeqAnnotPtr   sap = NULL;
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
  while ((status = CONN_Wait (conn, eIO_Read, &timeout)) == eIO_Timeout && max < 300) {
    currtime = GetSecs ();
    max = currtime - starttime;
#ifdef OS_MAC
    WaitNextEvent (0, &currEvent, 0, NULL);
#endif
  }
  if (status == eIO_Success) {
    sap = CddReadReply (conn, status);
  }
  CONN_Close (conn);

  return sap;
}

/* high-level connection functions */

NLM_EXTERN SeqAnnotPtr CddSynchronousQuery (
  BioseqPtr bsp,
  FloatHi expectValue,
  Boolean findBest,
  Boolean doFilter,
  Boolean makeFeats,
  CharPtr dataLib,
  Boolean precalc
)

{
  CONN         conn;
  SeqAnnotPtr  sap;

  conn = CddOpenConnection (bsp, expectValue, findBest, doFilter, makeFeats, dataLib, precalc);

  if (conn == NULL) return NULL;

  QUERY_SendQuery (conn);

  sap = CddWaitForReply (conn);

  return sap;
}

NLM_EXTERN Boolean CddAsynchronousQuery (
  BioseqPtr bsp,
  FloatHi expectValue,
  Boolean findBest,
  Boolean doFilter,
  Boolean makeFeats,
  CharPtr dataLib,
  Boolean precalc,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
)

{
  CONN  conn;

  conn = CddOpenConnection (bsp, expectValue, findBest, doFilter, makeFeats, dataLib, precalc);

  if (conn == NULL) return FALSE;

  QUERY_SendQuery (conn);

  QUERY_AddToQueue (queue, conn, resultproc, userdata, TRUE);

  return TRUE;
}

NLM_EXTERN Int4 CddCheckQueue (
  QUEUE* queue
)

{
  return QUERY_CheckQueue (queue);
}

/* utility functions */

static void PromoteSeqId (SeqIdPtr sip, Pointer userdata)

{
  SeqIdPtr  bestid, newid, oldid;

  if (sip == NULL) return;

  /* change lcl|tmpseq_0 returned from CGI, or gi if precalcualted */

  if (sip->choice != SEQID_LOCAL && sip->choice != SEQID_GI) return;

  bestid = (SeqIdPtr) userdata;

  newid = SeqIdDup (bestid);
  if (newid == NULL) return;

  oldid = ValNodeNew (NULL);
  if (oldid == NULL) return;

  MemCopy (oldid, sip, sizeof (ValNode));
  oldid->next = NULL;

  sip->choice = newid->choice;
  sip->data.ptrvalue = newid->data.ptrvalue;

  SeqIdFree (oldid);
  ValNodeFree (newid);

  SeqIdStripLocus (sip);
}

static void CorrectFeatureSeqIds (
  SeqFeatPtr sfp,
  Pointer userdata
)

{
  VisitSeqIdsInSeqLoc (sfp->location, userdata, PromoteSeqId);
}

static void CorrectAlignmentSeqIds (
  SeqAlignPtr sap,
  Pointer userdata
)

{
  VisitSeqIdsInSeqAlign (sap, userdata, PromoteSeqId);
}

NLM_EXTERN void CddCorrectIDs (
  BioseqPtr bsp,
  SeqAnnotPtr sap
)

{
  SeqEntryPtr  scope;
  SeqIdPtr     sip;

  if (bsp == NULL || sap == NULL) return;

  sip = SeqIdFindBest (bsp->id, 0);
  scope = SeqEntrySetScope (NULL);
  VisitFeaturesOnSap (sap, (Pointer) sip, CorrectFeatureSeqIds);
  VisitAlignmentsOnSap (sap, (Pointer) sip, CorrectAlignmentSeqIds);
  SeqEntrySetScope (scope);
}

