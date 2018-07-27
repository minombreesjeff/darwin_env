/*   vecscnapi.c
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
* File Name:  vecscnapi.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   6/13/00
*
* $Revision: 1.15 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
*
* ==========================================================================
*/

#include <ncbi.h>
#include <urlquery.h>
#include <vecscnapi.h>
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

NLM_EXTERN CONN VecScreenOpenConnection (
  CharPtr query
)

{
  /*
  return QUERY_OpenUrlQuery ("yar.ncbi.nlm.nih.gov", 6224,
                             "/VecScreen/vecscreenQB.cgi",
                             query, "vecscreenapp",
                             30, eMIME_T_NcbiData, eMIME_Fasta, eENCOD_Url,
                             fHCC_UrlDecodeInput | fHCC_UrlEncodeOutput);
  */

  StringCat (query, "\n");
  return QUERY_OpenServiceQuery ("VecScreen", query, 30);
}

NLM_EXTERN EIO_Status VecScreenWaitForReply (
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
  while ((status = CONN_Wait (conn, eIO_Read, &timeout)) == eIO_Timeout && max < 300) {
    currtime = GetSecs ();
    max = currtime - starttime;
#ifdef OS_MAC
    WaitNextEvent (0, &currEvent, 0, NULL);
#endif
  }

  return status;
}

typedef struct VQueueTag {
  Char                   rid [64];
  Char                   seqid [41];
  time_t                 estTime;
  time_t                 initialTime;
  time_t                 postedTime;
  Int2                   secondsToWait;
  VecScreenResultProc    resultproc;
  VecScreenAnnounceProc  announceproc;
  Nlm_VoidPtr            userdata;
  QUEUE                  connqueue;
  Boolean                done;
  struct VQueueTag*      next;
} VecScreenQueue, PNTR VQueuePtr;

static Boolean LIBCALLBACK SecondVecScreenCallback (
  CONN conn,
  Nlm_VoidPtr userdata,
  EIO_Status  status
)

{
  VQueuePtr  cqp;
  FILE       *fp;
  Char       line [256];
  Char       path [PATH_MAX];
  CharPtr    rid;
  CharPtr    sttus;
  CharPtr    str;
  Boolean    success = FALSE;
  Boolean    waiting = FALSE;

  /* look for waiting, failure, or success */

  cqp = (VQueuePtr) userdata;

  TmpNam (path);
  fp = FileOpen (path, "w");
  QUERY_CopyResultsToFile (conn, fp);
  FileClose (fp);

  fp = FileOpen (path, "r");
  str = ReadALine (line, sizeof (line), fp);
  while (str != NULL) {
    if (! StringHasNoText (line)) {
      if (line [0] == '>') {
        if (StringNICmp (line, ">Vector", 7) == 0) {
          rid = StringStr (line, "RID: ");
          if (rid != NULL) {
            rid += 5;
            sttus = StringStr (rid, " Status: ");
            if (sttus != NULL) {
              *sttus = '\0';
              sttus += 9;
              if (StringCmp (cqp->rid, rid) != 0) {
                ErrPostEx (SEV_ERROR, 0, 0, "RID mismatch '%s' vs '%s'", cqp->rid, rid);
                cqp->done = TRUE;
              } else if (StringStr (sttus, "FAILED") != NULL) {
                cqp->done = TRUE;
              } else if (StringStr (sttus, "unknown") != NULL) {
                ErrPostEx (SEV_ERROR, 0, 0, "RID unknown '%s'", rid);
                cqp->done = TRUE;
              } else if (StringStr (sttus, "SUCCESS") != NULL) {
                success = TRUE;
              } else if (StringStr (sttus, "WAITING") != NULL) {
                waiting = TRUE;
                /*
                Message (MSG_POST, "WAITING");
                */
              }
            }
          }
        } else if (StringNICmp (line, ">Message", 8) == 0) {
          str = ReadALine (line, sizeof (line), fp);
          while (str != NULL && StringNCmp (line, "//", 2) != 0) {
            Message (MSG_POST, "%s\n", str);
            if (StringStr (line, "FAILURE") != NULL) {
              if (! waiting) {
                cqp->done = TRUE;
              }
            }
            str = ReadALine (line, sizeof (line), fp);
          }
        }
      }
    }
    str = ReadALine (line, sizeof (line), fp);
  }
  FileClose (fp);

  if (success) {
    cqp->resultproc (path, cqp->userdata, cqp->rid, cqp->seqid, success);
    cqp->done = TRUE;
  } else if (cqp->done) {
    cqp->resultproc (NULL, cqp->userdata, cqp->rid, cqp->seqid, success);
  }

  FileRemove (path);

  return TRUE;
}

static Boolean LIBCALLBACK FirstVecScreenCallback (
  CONN conn,
  Nlm_VoidPtr userdata,
  EIO_Status  status
)

{
  VQueuePtr  cqp;
  FILE       *fp;
  Char       line [256];
  Char       path [PATH_MAX];
  CharPtr    rid;
  CharPtr    rtoe;
  CharPtr    str;
  long int   val;

  /* read rID or failure message */

  cqp = (VQueuePtr) userdata;

  TmpNam (path);
  fp = FileOpen (path, "w");
  QUERY_CopyResultsToFile (conn, fp);
  FileClose (fp);

  fp = FileOpen (path, "r");
  str = ReadALine (line, sizeof (line), fp);
  while (str != NULL) {
    if (! StringHasNoText (line)) {
      if (line [0] == '>') {
        if (StringNICmp (line, ">Vector", 7) == 0) {
          rid = StringStr (line, "RID: ");
          if (rid != NULL) {
            rid += 5;
            rtoe = StringStr (rid, " RTOE: ");
            if (rtoe != NULL) {
              *rtoe = '\0';
              rtoe += 7;
              StringNCpy_0 (cqp->rid, rid, sizeof (cqp->rid));
              if (sscanf (rtoe, "%ld", &val) == 1) {
                cqp->estTime = (time_t) val;
                cqp->secondsToWait = (Int2) val + 2;
              } else {
                cqp->secondsToWait = 15;
              }
              if (cqp->secondsToWait > 15) {
                cqp->secondsToWait = 15;
              }
              if (cqp->announceproc != NULL) {
                cqp->announceproc (rid, cqp->seqid, (Int2) val);
              }
            }
          } else if (StringStr (line, "FAILED") != NULL) {
            cqp->done = TRUE;
            if (cqp->resultproc != NULL) {
              cqp->resultproc (NULL, cqp->userdata, cqp->rid, cqp->seqid, FALSE);
            }
          }
        } else if (StringNICmp (line, ">Message", 8) == 0) {
          str = ReadALine (line, sizeof (line), fp);
          while (str != NULL && StringNCmp (line, "//", 2) != 0) {
            Message (MSG_POST, "%s\n", str);
            if (StringStr (line, "FAILURE") != NULL) {
              cqp->done = TRUE;
            }
            str = ReadALine (line, sizeof (line), fp);
          }
        }
      }
    }
    str = ReadALine (line, sizeof (line), fp);
  }
  FileClose (fp);

  FileRemove (path);

  return TRUE;
}

static void VecScreen_AddToQueue (
  VQUEUE * queue,
  VecScreenResultProc resultproc,
  VecScreenAnnounceProc announceproc,
  Nlm_VoidPtr userdata,
  CONN conn,
  BioseqPtr bsp
)

{
  VQueuePtr       cqp;
  VQueuePtr PNTR  qptr;
  VQueuePtr       tmp;

  if (queue == NULL || resultproc == NULL || conn == NULL || bsp == NULL) return;

  /* allocate queue element */

  cqp = (VQueuePtr) MemNew (sizeof (VecScreenQueue));
  if (cqp == NULL) return;

  cqp->rid [0] = '\0';
  SeqIdWrite (bsp->id, cqp->seqid, PRINTID_FASTA_LONG, 40);
  cqp->estTime = 0;
  cqp->initialTime = GetSecs ();
  cqp->postedTime = cqp->initialTime;
  cqp->secondsToWait = 15;
  cqp->resultproc = resultproc;
  cqp->announceproc = announceproc;
  cqp->userdata = userdata;
  cqp->connqueue = NULL;
  cqp->done = FALSE;

  /* add to polling queue */

  qptr = (VQueuePtr PNTR) queue;
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

  QUERY_AddToQueue (&(cqp->connqueue), conn, FirstVecScreenCallback, (Pointer) cqp, TRUE);
}

NLM_EXTERN Boolean VecScreenAsynchronousRequest (
  CharPtr database,
  BioseqPtr bsp,
  VQUEUE* queue,
  VecScreenResultProc resultproc,
  VecScreenAnnounceProc announceproc,
  VoidPtr userdata
)

{
  CONN  conn;
  FILE  *fp;
  Char  path [PATH_MAX];
  Char  str [128];

  if (bsp == NULL || queue == NULL || resultproc == NULL) return FALSE;

  if (StringHasNoText (database)) {
    database = "UniVec";
  }
  sprintf (str, "db=%s", database);
  conn = VecScreenOpenConnection (str);
  if (conn == NULL) return FALSE;

  TmpNam (path);

  fp = FileOpen (path, "w");
  BioseqToFasta (bsp, fp, ISA_na (bsp->mol));
  FileClose (fp);

  fp = FileOpen (path, "r");
  QUERY_CopyFileToQuery (conn, fp);
  FileClose (fp);

  QUERY_SendQuery (conn);

  VecScreen_AddToQueue (queue, resultproc, announceproc, userdata, conn, bsp);

  FileRemove (path);

  return TRUE;
}

static void VecScreen_RemoveFromQueue (
  VQUEUE* queue, VQueuePtr freeme
)

{
  VQueuePtr       curr;
  VQueuePtr       next;
  VQueuePtr PNTR  prev;
  VQueuePtr PNTR  qptr;

  qptr = (VQueuePtr PNTR) queue;
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

NLM_EXTERN Int4 VecScreenCheckQueue (
  VQUEUE* queue
)

{
  CONN            conn;
  Nlm_Int4        count = 0;
  VQueuePtr       curr;
  time_t          currtime;
  VQueuePtr       next;
  VQueuePtr PNTR  qptr;
  Char            str [128];

  qptr = (VQueuePtr PNTR) queue;
  if (qptr == NULL || *qptr == NULL) return 0;

  curr = *qptr;

  while (curr != NULL) {
    next = curr->next;

    /* check for return of rID, WAITING, of SUCCESS message */

    QUERY_CheckQueue (&(curr->connqueue));

    if (curr->done) {
      VecScreen_RemoveFromQueue (queue, curr);

    } else {
      if (curr->connqueue == NULL) {
        currtime = GetSecs ();
        if (currtime - curr->postedTime < curr->secondsToWait) {
        } else if (! StringHasNoText (curr->rid)) {

          /* estimated wait time has expired, so queue another check */

          if (curr->secondsToWait < 300) {
            curr->secondsToWait *= 2;
            if (curr->secondsToWait > 300) {
              curr->secondsToWait = 300;
            }
          }
          curr->postedTime = GetSecs ();

          sprintf (str, "req=%s", curr->rid);
          conn = VecScreenOpenConnection (str);
          QUERY_SendQuery (conn);
          QUERY_AddToQueue (&(curr->connqueue), conn, SecondVecScreenCallback, (Pointer) curr, TRUE);
        }
      }

      count++;
    }

    curr = next;
  }

  return count;
}

NLM_EXTERN Int4 PrintVecScreenQueue (
  VQUEUE* queue,
  FILE *fp
)

{
  Nlm_Int4        count = 0;
  VQueuePtr       curr;
  time_t          currtime;
  VQueuePtr       next;
  VQueuePtr PNTR  qptr;

  qptr = (VQueuePtr PNTR) queue;
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

