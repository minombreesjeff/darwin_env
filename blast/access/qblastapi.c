/*   qblastapi.c
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
* File Name:  qblastapi.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   6/28/00
*
* $Revision: 1.16 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
*
* $Log: qblastapi.c,v $
* Revision 1.16  2001/02/25 21:50:41  kans
* changes from Uint4 to size_t due to connection type changes
*
* Revision 1.15  2001/02/21 22:07:34  lavr
* Changes for use new CONN interface
*
* Revision 1.14  2000/12/06 17:53:31  madden
* make QBBioseqToFasta non-static, fix UMR
*
* Revision 1.13  2000/12/04 18:07:31  madden
* BLASTGetBOByRID uses Blast.cgi
*
* Revision 1.12  2000/11/07 21:54:52  shavirin
* Added new function BLASTGetQueryBioseqByRID().
*
* Revision 1.10  2000/08/30 14:55:08  kans
* parentheses around cast of currtime - curr->initialTime in printf
*
* Revision 1.9  2000/07/19 15:16:12  shavirin
* Added new function CharPtr BLASTGetBOByRID(CharPtr RID);
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <urlquery.h>
#include <qblastapi.h>
#include <tofasta.h>
#include <sqnutils.h>

#ifdef OS_MAC
#include <Events.h>
#endif

static CharPtr ReadALine (CharPtr str, size_t size, FILE *fp)

{
  Char     ch;
  CharPtr  ptr;
  CharPtr  rsult;

  if (str == NULL || size < 1 || fp == NULL) return NULL;
  *str = '\0';
  rsult = fgets (str, size, fp);
  if (rsult != NULL) {
    ptr = str;
    ch = *ptr;
    while (ch != '\0' && ch != '\n' && ch != '\r') {
      ptr++;
      ch = *ptr;
    }
    *ptr = '\0';
  }
  return rsult;
}

NLM_EXTERN CONN QBlastOpenConnection (
  void
)

{
 return QUERY_OpenUrlQuery ("www.ncbi.nlm.nih.gov", 80, "/blast/Blast.cgi",
                             NULL, "qblastapp", 30, eMIME_T_Application,
                             eMIME_WwwForm, eENCOD_Url, 0);
}

NLM_EXTERN EIO_Status QBlastWaitForReply (
  CONN conn
)

{
  time_t           currtime, starttime;
  Int2             max = 0;
  EIO_Status       status;
  STimeout         timeout;
#ifdef OS_MAC
  EventRecord      currEvent;
#endif

  if (conn == NULL) return eIO_Unknown;

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

  return status;
}

typedef struct QBQueueTag {
  Char                rid [64];
  Char                seqid [41];
  time_t              estTime;
  time_t              initialTime;
  time_t              postedTime;
  Int2                secondsToWait;
  QBlastResultProc    resultproc;
  QBlastAnnounceProc  announceproc;
  Nlm_VoidPtr         userdata;
  QUEUE               connqueue;
  Boolean             ready;
  Boolean             fetching;
  Boolean             done;
  struct QBQueueTag*  next;
} QBlastQueue, PNTR QBQueuePtr;

static Boolean LIBCALLBACK ThirdQBlastCallback (
  CONN conn,
  Nlm_VoidPtr userdata,
  EIO_Status  status
)

{
  QBQueuePtr  cqp;
  FILE        *fp;
  Char        path [PATH_MAX];

  cqp = (QBQueuePtr) userdata;

  TmpNam (path);
  fp = FileOpen (path, "w");
  QUERY_CopyResultsToFile (conn, fp);
  FileClose (fp);

  cqp->resultproc (path, cqp->userdata, cqp->rid, cqp->seqid, TRUE);
  cqp->done = TRUE;

  FileRemove (path);

  return TRUE;
}

static Boolean LIBCALLBACK SecondQBlastCallback (
  CONN conn,
  Nlm_VoidPtr userdata,
  EIO_Status  status
)

{
  QBQueuePtr  cqp;
  FILE        *fp;
  Char        line [256];
  Char        path [PATH_MAX];
  CharPtr     sttus;
  CharPtr     str;
  Boolean     waiting = FALSE;

  /* look for waiting, failure, or success */

  cqp = (QBQueuePtr) userdata;

  TmpNam (path);
  fp = FileOpen (path, "w");
  QUERY_CopyResultsToFile (conn, fp);
  FileClose (fp);

  fp = FileOpen (path, "r");
  str = ReadALine (line, sizeof (line), fp);
  while (str != NULL) {
    if (! StringHasNoText (line)) {
      sttus = StringStr (line, "Status=");
      if (sttus != NULL) {
        sttus += 7;
        if (StringStr (sttus, "WAITING") != NULL) {
          waiting = TRUE;
        } else if (StringStr (sttus, "READY") != NULL) {
          cqp->ready = TRUE;
        /* } else if (StringStr (sttus, "FAILED") != NULL) {
          cqp->done = TRUE; */
        }
      }
    }
    str = ReadALine (line, sizeof (line), fp);
  }
  FileClose (fp);

  if (cqp->done) {
    cqp->resultproc (NULL, cqp->userdata, cqp->rid, cqp->seqid, FALSE);
  }

  FileRemove (path);

  return TRUE;
}

static Boolean LIBCALLBACK FirstQBlastCallback (
  CONN conn,
  Nlm_VoidPtr userdata,
  EIO_Status  status
)

{
  QBQueuePtr  cqp;
  FILE        *fp;
  Char        line [256];
  Char        path [PATH_MAX];
  CharPtr     ptr;
  Char        rid [64];
  time_t      rtoe;
  CharPtr     str;
  long int    val;

  /* read rID or failure message */

  cqp = (QBQueuePtr) userdata;

  TmpNam (path);
  fp = FileOpen (path, "w");
  QUERY_CopyResultsToFile (conn, fp);
  FileClose (fp);

  rid [0] = '\0';
  rtoe = 15;

  fp = FileOpen (path, "r");
  str = ReadALine (line, sizeof (line), fp);
  while (str != NULL) {
    if (! StringHasNoText (line)) {
      ptr = StringStr (line, "RID = ");
      if (ptr != NULL) {
        ptr += 6;
        StringNCpy_0 (rid, ptr, sizeof (rid));
      }
      ptr = StringStr (line, "RTOE = ");
      if (ptr != NULL) {
        ptr += 7;
        if (sscanf (ptr, "%ld", &val) == 1) {
          rtoe = (time_t) val;
        }
      }
    }
    str = ReadALine (line, sizeof (line), fp);
  }
  FileClose (fp);

  FileRemove (path);

  if (! StringHasNoText (rid)) {
    StringNCpy_0 (cqp->rid, rid, sizeof (cqp->rid));
    cqp->estTime = (time_t) rtoe;
    cqp->secondsToWait = (Int2) rtoe + 2;
    if (cqp->secondsToWait > 15) {
      cqp->secondsToWait = 15;
    }
    if (cqp->announceproc != NULL) {
      cqp->announceproc (rid, cqp->seqid, (Int2) val);
    }

  } else {

    cqp->done = TRUE;
    if (cqp->resultproc != NULL) {
      cqp->resultproc (NULL, cqp->userdata, cqp->rid, cqp->seqid, FALSE);
    }
  }

  return TRUE;
}

static void QBlast_AddToQueue (
  QBQUEUE * queue,
  QBlastResultProc resultproc,
  QBlastAnnounceProc announceproc,
  Nlm_VoidPtr userdata,
  CONN conn,
  BioseqPtr bsp,
  CharPtr rid
)

{
  QBQueuePtr       cqp;
  QBQueuePtr PNTR  qptr;
  QBQueuePtr       tmp;

  if (queue == NULL || resultproc == NULL || conn == NULL) return;

  /* allocate queue element */

  cqp = (QBQueuePtr) MemNew (sizeof (QBlastQueue));
  if (cqp == NULL) return;

  cqp->rid [0] = '\0';
  if (rid != NULL) {
    StringNCpy_0 (cqp->rid, rid, sizeof (cqp->rid));
  }
  cqp->seqid [0] = '\0';
  if (bsp != NULL) {
    SeqIdWrite (bsp->id, cqp->seqid, PRINTID_FASTA_LONG, 40);
  }

  cqp->estTime = 0;
  cqp->initialTime = GetSecs ();
  cqp->postedTime = cqp->initialTime;
  cqp->secondsToWait = 15;
  cqp->resultproc = resultproc;
  cqp->announceproc = announceproc;
  cqp->userdata = userdata;
  cqp->connqueue = NULL;
  cqp->ready = FALSE;
  cqp->fetching = FALSE;
  cqp->done = FALSE;

  /* add to polling queue */

  qptr = (QBQueuePtr PNTR) queue;
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

  /* queue the request for a rID */

  if (bsp != NULL) {

    QUERY_AddToQueue (&(cqp->connqueue), conn, FirstQBlastCallback, (Pointer) cqp, TRUE);

  } else if (rid != NULL) {

    QUERY_AddToQueue (&(cqp->connqueue), conn, SecondQBlastCallback, (Pointer) cqp, TRUE);

  }
}

/* print just the sequence with no punctuation */

static Boolean QBlastFastaFileFunc (BioseqPtr bsp, Int2 key, CharPtr buf,
                                    Uint4 buflen, Pointer data)
{
  FILE * fp;
  CharPtr bufr;
  size_t len, bufsize;
  size_t src_read, dst_written;

  fp = (FILE *)data;

  switch (key) {
    case FASTA_ID:
      len = StringLen (buf);
      if (len > 0) {
        bufsize = len * 3 + 2;
        bufr = MemNew (bufsize);
        if (bufr != NULL) {
          URL_Encode (buf, len, &src_read, bufr, bufsize, &dst_written);
          fprintf (fp, "%s", "%3E");
          fprintf(fp, "%s+", bufr);
        }
        MemFree (bufr);
      }
      /* fprintf(fp, ">%s ", buf); */
      break;
    case FASTA_DEFLINE:
      len = StringLen (buf);
      if (len > 0) {
        bufsize = len * 3 + 2;
        bufr = MemNew (bufsize);
        if (bufr != NULL) {
          URL_Encode (buf, len, &src_read, bufr, bufsize, &dst_written);
          fprintf(fp, "%s", bufr);
          fprintf (fp, "%s", "%0D%0A");
        }
        MemFree (bufr);
      }
      /* fprintf(fp, "%s\n", buf); */
      break;
    case FASTA_SEQLINE:
      fprintf(fp, "%s", buf); /* no newline */
      break;
    case FASTA_EOS:   /* end of sequence */
      break;
    default:
      break;
  }
  return TRUE;
}

#define QB_FASTA_BUFFER_LEN 400

Boolean QBBioseqToFasta (BioseqPtr bsp, FILE *fp, Boolean is_na)

{
  MyFsa mfa;
  Char buf [QB_FASTA_BUFFER_LEN + 1];

  if (bsp == NULL || fp == NULL) return FALSE;

  MemSet(&mfa, 0, sizeof(MyFsa));

  mfa.buf = buf;
  mfa.buflen = QB_FASTA_BUFFER_LEN;
  mfa.seqlen = 80;
  mfa.mydata = (Pointer)fp;
  mfa.myfunc = QBlastFastaFileFunc;
  mfa.bad_asn1 = FALSE;
  mfa.order = 0;
  mfa.accession = NULL;
  mfa.organism = NULL;
  mfa.do_virtual = FALSE;
  mfa.tech = 0;
  mfa.no_sequence = FALSE;
  mfa.formatdb = FALSE;

  return BioseqToFastaX (bsp, &mfa, is_na);
}

NLM_EXTERN Boolean QBlastAsynchronousRequest (
  CharPtr database,
  CharPtr program,
  BioseqPtr bsp,
  QBQUEUE* queue,
  QBlastResultProc resultproc,
  QBlastAnnounceProc announceproc,
  VoidPtr userdata
)

{
  CONN    conn;
  FILE    *fp;
  size_t  n_written;
  Char    path [PATH_MAX];
  Char    str [256];

  if (bsp == NULL || queue == NULL || resultproc == NULL) return FALSE;

  if (StringHasNoText (database)) {
    database = "nr";
  }
  if (StringHasNoText (program)) {
    program = "blastn";
  }
  sprintf (str, "DATABASE=%s&PROGRAM=%s&CMD=Put&QUERY_BELIEVE_DEFLINE=yes&QUERY=",
           database, program);
  conn = QBlastOpenConnection ();
  if (conn == NULL) return FALSE;

  TmpNam (path);

  fp = FileOpen (path, "w");
  QBBioseqToFasta (bsp, fp, ISA_na (bsp->mol));
  FileClose (fp);

  fp = FileOpen (path, "r");
  CONN_Write (conn, (const void *) str, StringLen (str), &n_written);
  QUERY_CopyFileToQuery (conn, fp);
  FileClose (fp);

  QUERY_SendQuery (conn);

  QBlast_AddToQueue (queue, resultproc, announceproc, userdata, conn, bsp, NULL);

  FileRemove (path);

  return TRUE;
}

static void QBlast_RemoveFromQueue (
  QBQUEUE* queue, QBQueuePtr freeme
)

{
  QBQueuePtr       curr;
  QBQueuePtr       next;
  QBQueuePtr PNTR  prev;
  QBQueuePtr PNTR  qptr;

  qptr = (QBQueuePtr PNTR) queue;
  if (qptr == NULL || *qptr == NULL || freeme == NULL) return;

  prev = qptr;
  curr = *qptr;

  while (curr != NULL) {
    next = curr->next;
    if (curr == freeme) {
      *(prev) = next;
      curr->next = NULL;
      MemFree (curr);
    } else {
      prev = &(curr->next);
    }
    curr = next;
  }
}

NLM_EXTERN Int4 QBlastCheckQueue (
  QBQUEUE* queue
)

{
  CONN             conn;
  Nlm_Int4         count = 0;
  QBQueuePtr       curr;
  time_t           currtime;
  QBQueuePtr       next;
  size_t           n_written;
  QBQueuePtr PNTR  qptr;
  Char             str [80];

  qptr = (QBQueuePtr PNTR) queue;
  if (qptr == NULL || *qptr == NULL) return 0;

  curr = *qptr;

  /* first loop through actual url requests, call first-third callbacks */

  while (curr != NULL) {
    next = curr->next;

    /* check for return of rID, WAITING, of SUCCESS message */

    QUERY_CheckQueue (&(curr->connqueue));

    if (curr->done) {
      QBlast_RemoveFromQueue (queue, curr);
    }

    curr = next;
  }

  /* now send new url queries if appropriate */

  curr = *qptr;
  if (curr == NULL) return 0;

  while (curr != NULL) {
    next = curr->next;

    if (curr->done) {
      QBlast_RemoveFromQueue (queue, curr);

    } else {

      if ((! curr->fetching) && (! curr->ready) && curr->connqueue == NULL) {
        currtime = GetSecs ();
        if (currtime - curr->postedTime < curr->secondsToWait) {
        } else if (! StringHasNoText (curr->rid)) {

          /* estimated wait time has expired, so queue another check */

          if (curr->secondsToWait < 60) {
            curr->secondsToWait *= 2;
          }
          curr->postedTime = GetSecs ();

          sprintf (str, "RID=%s&CMD=Get\n", curr->rid);
          conn = QBlastOpenConnection ();
          if (conn != NULL) {
            CONN_Write (conn, (const void *) str, StringLen (str), &n_written);
            QUERY_SendQuery (conn);
            QUERY_AddToQueue (&(curr->connqueue), conn, SecondQBlastCallback, (Pointer) curr, TRUE);
          } else {
            ErrPostEx (SEV_ERROR, 0, 0, "Unable to open QBlast connection");
          }
        }
      }

      count++;
    }

    if ((! curr->done) && curr->ready && (! curr->fetching) && curr->connqueue == NULL) {
      curr->fetching = TRUE;
      sprintf (str, "CMD=Get&RID=%s&FORMAT_OBJECT=Alignment&FORMAT_TYPE=ASN.1\n", curr->rid);
      conn = QBlastOpenConnection ();
      if (conn != NULL) {
        CONN_Write (conn, (const void *) str, StringLen (str), &n_written);
        QUERY_SendQuery (conn);
        QUERY_AddToQueue (&(curr->connqueue), conn, ThirdQBlastCallback, (Pointer) curr, TRUE);
      } else {
        ErrPostEx (SEV_ERROR, 0, 0, "Unable to open QBlast connection");
      }
    }

    curr = next;
  }

  return count;
}

NLM_EXTERN Boolean QBlastCheckRequest (
  CharPtr rid,
  QBQUEUE* queue,
  QBlastResultProc resultproc,
  QBlastAnnounceProc announceproc,
  VoidPtr userdata
)

{
  CONN    conn;
  size_t  n_written;
  Char    str [256];

  if (StringHasNoText (rid) || queue == NULL || resultproc == NULL) return FALSE;

  sprintf (str, "RID=%s&CMD=Get\n", rid);
  conn = QBlastOpenConnection ();
  if (conn == NULL) return FALSE;
  CONN_Write (conn, (const void *) str, StringLen (str), &n_written);
  QUERY_SendQuery (conn);

  QBlast_AddToQueue (queue, resultproc, announceproc, userdata, conn, NULL, rid);

  return TRUE;
}

NLM_EXTERN void QBlastCloseQueue (
  QBQUEUE* queue
)

{
  QBQueuePtr       curr;
  QBQueuePtr       next;
  QBQueuePtr PNTR  qptr;

  qptr = (QBQueuePtr PNTR) queue;
  if (qptr == NULL || *qptr == NULL) return;

  curr = *qptr;

  while (curr != NULL) {
    next = curr->next;

    if (curr->connqueue != NULL) {
      QUERY_CloseQueue (&(curr->connqueue));
    }
    QBlast_RemoveFromQueue (queue, curr);

    curr = next;
  }
}

NLM_EXTERN Int4 PrintQBlastQueue (
  QBQUEUE* queue,
  FILE *fp
)

{
  Nlm_Int4         count = 0;
  QBQueuePtr       curr;
  time_t           currtime;
  QBQueuePtr       next;
  QBQueuePtr PNTR  qptr;

  qptr = (QBQueuePtr PNTR) queue;
  if (qptr == NULL || *qptr == NULL || fp == NULL) return 0;

  currtime = GetSecs ();
  curr = *qptr;

  while (curr != NULL) {
    next = curr->next;

    if (! curr->done) {

      fprintf (fp, "%s\t%s\t%d\t%d\n", curr->rid, curr->seqid,
               (int) (currtime - curr->initialTime), (int) curr->estTime);
      count++;
    }

    curr = next;
  }

  return count;
}

static void LIBCALLBACK AsnIoErrorFunc(Int2 type, CharPtr message)
{
    return;
}

/* Function to get SeqAnnot for RID. We suupose, that search already
   finished and results are exists on the Qblast repository */

NLM_EXTERN SeqAnnotPtr BLASTGetSeqAnnotByRID(CharPtr RID)
{
    Char         query_string[256];
    CONN         conn;
    SeqAnnotPtr  annot;
    size_t       n_written;
    Int4         length;
    AsnIoConnPtr aicp;
    EIO_Status   status;

    /* sprintf(query_string, "RID=%s&ALIGNMENT_VIEW=11", RID); */
    
    sprintf(query_string, "FORMAT_TYPE=ASN.1&CMD=Get&RID=%s&"
            "FORMAT_OBJECT=Alignment", RID);
    
    /*    conn = QUERY_OpenUrlQuery ("www.ncbi.nlm.nih.gov", 80,  */
    conn = QUERY_OpenUrlQuery ("www.ncbi.nlm.nih.gov", 80, 
                               "/blast/Blast.cgi", NULL,  
                               "BLASTGetSeqAnnotByRID()", 
                               30, eMIME_T_Application, 
                               eMIME_WwwForm, eENCOD_Url, 0);
    
    length = StringLen(query_string)+1;
    status = CONN_Write (conn, query_string, length , &n_written);    
    QUERY_SendQuery (conn);
    
    aicp = QUERY_AsnIoConnOpen ("r", conn);
    AsnIoSetErrorMsg(aicp->aip, AsnIoErrorFunc);

    annot = SeqAnnotAsnRead (aicp->aip, NULL);
    QUERY_AsnIoConnClose (aicp);

    CONN_Close(conn);
    return annot;
}
/* Function to get SeqAnnot for RID. We suupose, that search already
   finished and results are exists on the Qblast repository */

NLM_EXTERN BioseqPtr BLASTGetQueryBioseqByRID(CharPtr RID)
{
    Char         query_string[256];
    CONN         conn;
    BioseqPtr    bsp;
    size_t       n_written;
    Int4         length;
    AsnIoConnPtr aicp;
    EIO_Status   status;
    
    /* sprintf(query_string, "RID=%s&ALIGNMENT_VIEW=11", RID); */
    
    sprintf(query_string, "CMD=Get&RID=%s&FORMAT_OBJECT=Bioseq&"
            "FORMAT_TYPE=ASN.1", RID);
    
    /* conn = QUERY_OpenUrlQuery ("www.ncbi.nlm.nih.gov", 80,  */
    
    conn = QUERY_OpenUrlQuery ("www.ncbi.nlm.nih.gov", 80, 
                               "/blast/Blast.cgi", NULL, 
                               "BLASTGetQueryBioseqByRID()", 
                               30, eMIME_T_Application, 
                               eMIME_WwwForm, eENCOD_Url, 0);

    length = StringLen(query_string);
    status = CONN_Write (conn, query_string, length , &n_written);    
    QUERY_SendQuery (conn);
    
    aicp = QUERY_AsnIoConnOpen ("r", conn);
    AsnIoSetErrorMsg(aicp->aip, AsnIoErrorFunc);

    bsp = BioseqAsnRead (aicp->aip, NULL);
    QUERY_AsnIoConnClose (aicp);

    CONN_Close(conn);
    return bsp;
}

/* Function to get BlastObject for RID. We suupose, that search already
   finished and results are exists on the Qblast repository. Blast Object
   ASN.1 will be returned as CharPtr buffer*/
NLM_EXTERN CharPtr BLASTGetBOByRID(CharPtr RID)
{
    Char         query_string[256];
    CONN         conn;
    size_t       n_written;
    size_t       bytes;
    Int4         length, buff_len, new_size;
    EIO_Status   status;
    CharPtr      in_buff;
    
/*
    sprintf(query_string, "RID=%s&ALIGNMENT_VIEW=13", RID);
*/
    sprintf(query_string, "CMD=Get&RID=%s&FORMAT_OBJECT=BlastObject&FORMAT_TYPE=ASN.1", RID);
    
    conn = QUERY_OpenUrlQuery ("www.ncbi.nlm.nih.gov", 80, 
                               "/blast/Blast.cgi", NULL, 
                               "BLASTGetBOByRID()", 
                               30, eMIME_T_Application, 
                               eMIME_WwwForm, eENCOD_Url, 0);

    length = StringLen(query_string) + 1;
    status = CONN_Write (conn, query_string, length , &n_written);    
    QUERY_SendQuery (conn);
    
    new_size = 1024;   
    if((in_buff = (CharPtr)MemNew(new_size)) == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Error in allocating memory\n");
        return NULL;
    }

    buff_len = 0;
    while ((status = CONN_Read(conn, in_buff + buff_len, 
                               1024, &bytes, eIO_Plain)) == eIO_Success) {
        
        if(bytes == 0) 
            break;

        new_size += bytes;
        buff_len += bytes;
        
        if ((in_buff = (CharPtr)Realloc(in_buff, new_size)) == NULL) {
            ErrPostEx(SEV_ERROR, 0, 0, "Error in reallocating memory\n");
            return NULL;
        }
    }
    
    in_buff[buff_len] = NULLB;
    
    CONN_Close(conn);
    return in_buff;
}
