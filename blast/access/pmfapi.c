/*   pmfapi.c
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
* File Name:  pmfapi.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   5/5/00
*
* $Revision: 1.11 $
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
#include <pmfapi.h>
#include <objmedli.h>
#include <objpubme.h>
#include <objsset.h>
#include <objmgr.h>
#include <sequtil.h>
#include <ent2api.h>

#ifdef OS_MAC
#include <Events.h>
#endif

#ifdef OS_UNIX
#include <sys/times.h>
#include <limits.h>
#endif

/* ml to std code modified from original in medutil.c */

static Boolean AllUpperCase (
  CharPtr p
)

{
  Char  ch;

  if (p == NULL) return FALSE;
  ch = *p;
  while (ch != '\0') {
    if (! IS_UPPER (ch)) return FALSE;
    p++;
    ch = *p;
  }
  return TRUE;
}

static void SplitMLAuthorName (
  CharPtr name,
  CharPtr last,
  CharPtr initials,
  CharPtr suffix
)

{
  CharPtr  p, p2;
  Int2     i;
  Char     sbuf [20], ibuf [20];

  /* Clear the ibuf field and transfer the entire name to 'last',
  excluding leading and trailing spaces */

  if (name == NULL) return;

  ibuf [0] = '\0';
  sbuf [0] = '\0';
  last [0] = '\0';
  initials [0] = '\0';
  suffix [0] = '\0';
  while (*name <= ' ') {
    name++;
    if (*name == '\0') return;
  }
  StringCpy( last, name );

  for (i=StringLen (last) - 1; ((i >= 0) && (last [i] <= ' ')); i--) {
    last[i] = '\0';
  }

  /* Strip off the last token (initials or name suffix (Jr, Sr, suffix.) */

  p = StringRChr (last, (int) ' ');
  if (p != NULL) { /* more than just last name */

    /* Separate the token from the last name */

    p2 = p + 1;
    while ((p > last) && (*p == ' ')) {
      *p = '\0';
      p--;
    }

    /* If the last token is not all upper case, and there are more than
    two tokens, see if the next to the last are initials (upper case) */

    if (! AllUpperCase (p2) && (p = StringRChr (last, (int) ' ' )) != NULL) {

      /* We have at least three tokens, is the next to last initials? */

      if (AllUpperCase (p + 1)) {

        /* Yes - concatenate the last two tokens as initials */

        StringCpy (ibuf, p + 1);
        StringCpy (sbuf, p2);
        while (p > last && (*p == ' ')) {
          *p = '\0';
          p--;
        }
      }
    }
    
    if (ibuf [0] == '\0') { /* Only the last token goes in ibuf */
      StringCpy (ibuf, p2);
    }
  }

  /* now add periods to ibuf and convert suffix */

  for (p = initials, p2 = ibuf; *p2 != '\0'; p2++, p++) {
    *p = *p2;
    if (! IS_LOWER(*(p2 + 1))) { /* watch out for foreign names */
      p++;
      *p = '.';
    }
  }
  *p = '\0';

  if (sbuf [0]) {
    if (StringCmp (sbuf, "1d") == 0)
      p = StringMove (suffix, "I.");
    else if (StringCmp (sbuf, "2d") == 0)
      p = StringMove (suffix, "II.");
    else if (StringCmp (sbuf, "3d") == 0)
      p = StringMove (suffix, "III.");
    else if (StringCmp (sbuf, "4th") == 0)
      p = StringMove (suffix, "IV.");
    else if (StringCmp (sbuf, "5th") == 0)
      p = StringMove (suffix, "V.");
    else if (StringCmp (sbuf, "6th") == 0)
      p = StringMove (suffix, "VI.");
    else if (StringCmp (sbuf, "Sr") == 0)
      p = StringMove (suffix, "Sr.");
    else if (StringCmp (sbuf, "Jr") == 0)
      p = StringMove (suffix, "Jr.");
    else
      p = StringMove (suffix, sbuf);
  }
}

static ValNodePtr ConvertMLtoSTD (
  CharPtr token
)

{
  AuthorPtr   aup;
  CharPtr     eptr;
  Char        last [80], initials [20], suffix [20];
  NameStdPtr  nsp;
  PersonIdPtr pid;
  ValNodePtr  vnp;

  if (token == NULL) return NULL;
  for (eptr = token + StringLen (token) - 1;
       eptr > token && *eptr == ' ';
       eptr--) continue;

  SplitMLAuthorName (token, last, initials, suffix);

  nsp = NameStdNew ();
  if (nsp == NULL) return NULL;
  nsp->names [0] = StringSave (last);
  if (initials [0] != '\0') {
    nsp->names[4] = StringSave (initials);
  }
  if (suffix[0] != '\0') {
    nsp->names[5] = StringSave (suffix);
  }
  if (nsp->names[0] != NULL) {
    pid = PersonIdNew ();
    pid->choice = 2; /* name */
    pid->data = nsp;
    aup = AuthorNew ();
    aup->name = pid;
    vnp = ValNodeNew (NULL);
    vnp->data.ptrvalue = (Pointer) aup;
    return vnp;
  }
  return NULL;
}

static void ChangeMedlineAuthorsToISO (
  MedlineEntryPtr mep
)

{
  AuthListPtr  alp;
  CitArtPtr    cap;
  ValNodePtr   curr, oldnames, tmp, v;

  if (mep == NULL) return;
  cap = mep->cit;
  if (cap == NULL) return;
  alp = cap->authors;
  if (alp == NULL || alp->choice != 2) return;

  oldnames = alp->names;
  alp->names = NULL;
  alp->choice = 1; /* make std names */

  for (tmp = oldnames; tmp != NULL; tmp = tmp->next) {
    curr = ConvertMLtoSTD ((CharPtr) tmp->data.ptrvalue);
    if (alp->names == NULL) {
      alp->names = curr;
    } else {
      for (v = alp->names; v->next != NULL; v = v->next) continue;
      v->next = curr;
    }
  }

  ValNodeFreeData (oldnames);
}

NLM_EXTERN CONN PMFetchOpenConnection (
  CharPtr db,
  Int4 uid
)

{
  Char  query [64];

  if (StringHasNoText (db) || uid < 1) return NULL;
  if (StringICmp (db, "PubMed") != 0 &&
      StringICmp (db, "Protein") != 0&&
      StringICmp (db, "Nucleotide") != 0&&
      StringICmp (db, "Popset") != 0) {
    ErrPostEx (SEV_ERROR, 0, 0,  "Unrecognized database %s", db);
    return NULL;
  }
  sprintf (query, "db=%s&id=%ld&report=asn1&mode=text", db, (long) uid);
  return QUERY_OpenUrlQuery ("www.ncbi.nlm.nih.gov", 80, "/entrez/utils/pmfetch.fcgi",
                             query, "Entrez2Tool", 30, eMIME_T_NcbiData,
                             eMIME_AsnText, eENCOD_None, 0);
}

static EIO_Status CommonWaitForReply (
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

NLM_EXTERN PubmedEntryPtr PubMedWaitForReply (
  CONN conn
)

{
  AsnIoConnPtr    aicp;
  PubmedEntryPtr  pep = NULL;

  if (conn == NULL) return NULL;

  if (CommonWaitForReply (conn) == eIO_Success) {
    aicp = QUERY_AsnIoConnOpen ("r", conn);
    pep = PubmedEntryAsnRead (aicp->aip, NULL);
    QUERY_AsnIoConnClose (aicp);
  }
  CONN_Close (conn);

  ChangeMedlineAuthorsToISO ((MedlineEntryPtr) pep->medent);

  return pep;
}

NLM_EXTERN SeqEntryPtr PubSeqWaitForReply (
  CONN conn
)

{
  AsnIoConnPtr  aicp;
  SeqEntryPtr   sep = NULL;

  if (conn == NULL) return NULL;

  if (CommonWaitForReply (conn) == eIO_Success) {
    aicp = QUERY_AsnIoConnOpen ("r", conn);
    sep = SeqEntryAsnRead (aicp->aip, NULL);
    QUERY_AsnIoConnClose (aicp);
  }
  CONN_Close (conn);

  return sep;
}

NLM_EXTERN PubmedEntryPtr PubMedSynchronousQuery (
  Int4 uid
)

{
  CONN            conn;
  PubmedEntryPtr  pep;
#ifdef OS_UNIX
  Boolean         logtimes;
  clock_t         starttime;
  clock_t         stoptime;
  struct tms      timebuf;
#endif

  if (uid < 1) return NULL;

#ifdef OS_UNIX
  logtimes = (Boolean) ((getenv ("NCBI_LOG_SYNC_QUERY_TIMES")) != NULL);
#endif

  conn = PMFetchOpenConnection ("PubMed", uid);

  if (conn == NULL) return NULL;

  QUERY_SendQuery (conn);

#ifdef OS_UNIX
  if (logtimes) {
    starttime = times (&timebuf);
  }
#endif

  pep = PubMedWaitForReply (conn);

#ifdef OS_UNIX
  if (logtimes) {
    stoptime = times (&timebuf);
    printf ("PubMedWaitForReply %ld\n", (long) (stoptime - starttime));
  }
#endif

  return pep;
}

NLM_EXTERN SeqEntryPtr PubSeqSynchronousQuery (
  CharPtr db,
  Int4 uid
)

{
  CONN         conn;
  SeqEntryPtr  sep;
#ifdef OS_UNIX
  Boolean      logtimes;
  clock_t      starttime;
  clock_t      stoptime;
  struct tms   timebuf;
#endif

  if (StringHasNoText (db) || uid < 1) return NULL;

#ifdef OS_UNIX
  logtimes = (Boolean) ((getenv ("NCBI_LOG_SYNC_QUERY_TIMES")) != NULL);
#endif

  conn = PMFetchOpenConnection (db, uid);

  if (conn == NULL) return NULL;

  QUERY_SendQuery (conn);

#ifdef OS_UNIX
  if (logtimes) {
    starttime = times (&timebuf);
  }
#endif

  sep = PubSeqWaitForReply (conn);

#ifdef OS_UNIX
  if (logtimes) {
    stoptime = times (&timebuf);
    printf ("PubSeqWaitForReply %ld\n", (long) (stoptime - starttime));
  }
#endif

  return sep;
}

NLM_EXTERN Boolean PubMedAsynchronousQuery (
  Int4 uid,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
)

{
  CONN  conn;

  conn = PMFetchOpenConnection ("PubMed", uid);

  if (conn == NULL) return FALSE;

  QUERY_SendQuery (conn);

  QUERY_AddToQueue (queue, conn, resultproc, userdata, TRUE);

  return TRUE;
}

NLM_EXTERN Int4 PubMedCheckQueue (
  QUEUE* queue
)

{
  return QUERY_CheckQueue (queue);
}

NLM_EXTERN PubmedEntryPtr PubMedReadReply (
  CONN conn,
  EIO_Status status
)

{
  AsnIoConnPtr    aicp;
  PubmedEntryPtr  pep = NULL;

  if (conn != NULL && status == eIO_Success) {
    aicp = QUERY_AsnIoConnOpen ("rb", conn);
    pep = PubmedEntryAsnRead (aicp->aip, NULL);
    QUERY_AsnIoConnClose (aicp);
  }
  return pep;
}

NLM_EXTERN Boolean PubSeqAsynchronousQuery (
  CharPtr db,
  Int4 uid,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
)

{
  CONN  conn;

  conn = PMFetchOpenConnection (db, uid);

  if (conn == NULL) return FALSE;

  QUERY_SendQuery (conn);

  QUERY_AddToQueue (queue, conn, resultproc, userdata, TRUE);

  return TRUE;
}

NLM_EXTERN Int4 PubSeqCheckQueue (
  QUEUE* queue
)

{
  return QUERY_CheckQueue (queue);
}

NLM_EXTERN SeqEntryPtr PubSeqReadReply (
  CONN conn,
  EIO_Status status
)

{
  AsnIoConnPtr  aicp;
  SeqEntryPtr   sep = NULL;

  if (conn != NULL && status == eIO_Success) {
    aicp = QUERY_AsnIoConnOpen ("rb", conn);
    sep = SeqEntryAsnRead (aicp->aip, NULL);
    QUERY_AsnIoConnClose (aicp);
  }
  return sep;
}

/* object manager registerable fetch function */

static CharPtr pubseqfetchproc = "PubSeqBioseqFetch";

static Int2 LIBCALLBACK PubSeqBioseqFetchFunc (Pointer data)

{
  BioseqPtr         bsp;
  Char              id [41];
  OMProcControlPtr  ompcp;
  ObjMgrProcPtr     ompp;
  SeqEntryPtr       sep = NULL;
  SeqIdPtr          sid;
  SeqIdPtr          sip;
  Int4              uid;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  ompp = ompcp->proc;
  if (ompp == NULL) return OM_MSG_RET_ERROR;
  sip = (SeqIdPtr) ompcp->input_data;
  if (sip == NULL) return OM_MSG_RET_ERROR;

  if (sip->choice == SEQID_GI) {

    uid = sip->data.intvalue;
    if (uid == 0) return OM_MSG_RET_ERROR;

    sep = PubSeqSynchronousQuery ("nucleotide", uid);
    if (sep == NULL) {
      sep = PubSeqSynchronousQuery ("protein", uid);
    }

  } else {

    sid = SeqIdDup (sip);
    SeqIdWrite (sid, id, PRINTID_FASTA_LONG, sizeof (id) - 1);
    SeqIdFree (sid);

    uid = EntrezGetUIDforSeqIdString ("nucleotide", id);
    if (uid != 0) {
      sep = PubSeqSynchronousQuery ("nucleotide", uid);
    } else {
      uid = EntrezGetUIDforSeqIdString ("protein", id);
      if (uid == 0) return OM_MSG_RET_ERROR;
      sep = PubSeqSynchronousQuery ("protein", uid);
    }
  }

  if (sep == NULL) return OM_MSG_RET_ERROR;
  bsp = BioseqFindInSeqEntry (sip, sep);
  ompcp->output_data = (Pointer) bsp;
  ompcp->output_entityID = ObjMgrGetEntityIDForChoice (sep);
  return OM_MSG_RET_DONE;
}

NLM_EXTERN Boolean PubSeqFetchEnable (void)

{
  ObjMgrProcLoad (OMPROC_FETCH, pubseqfetchproc, pubseqfetchproc,
                  OBJ_SEQID, 0, OBJ_BIOSEQ, 0, NULL,
                  PubSeqBioseqFetchFunc, PROC_PRIORITY_DEFAULT);
  return TRUE;
}

NLM_EXTERN void PubSeqFetchDisable (void)

{
  ObjMgrPtr      omp;
  ObjMgrProcPtr  ompp;

  omp = ObjMgrGet ();
  ompp = ObjMgrProcFind (omp, 0, pubseqfetchproc, OMPROC_FETCH);
  if (ompp == NULL) return;
  ObjMgrFreeUserData (0, ompp->procid, OMPROC_FETCH, 0);
}

