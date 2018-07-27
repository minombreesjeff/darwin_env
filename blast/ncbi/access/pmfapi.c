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
* $Revision: 1.63 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
*
* ==========================================================================
*/

#include <ncbi.h>
#include <pmfapi.h>
#include <objmedli.h>
#include <objpubme.h>
#include <objsset.h>
#include <objmgr.h>
#include <sequtil.h>
#include <sqnutils.h>

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

  /* do not convert if too big for buffers */

  for (tmp = alp->names; tmp != NULL; tmp = tmp->next) {
    if (StringLen ((CharPtr) tmp->data.ptrvalue) > 70) return;
  }

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

NLM_EXTERN CONN PubMedFetchOpenConnection (
  Int4 uid
)

{
  Char  query [64];

  if (uid < 1) return NULL;

  /*
  sprintf (query, "db=PubMed&id=%ld&report=asn1&mode=text", (long) uid);
  return QUERY_OpenUrlQuery ("www.ncbi.nlm.nih.gov", 80, "/entrez/utils/pmfetch.fcgi",
                             query, "Entrez2Tool", 30, eMIME_T_NcbiData,
                             eMIME_AsnText, eENCOD_None, 0);
  */

  sprintf (query, "id=%ld", (long) uid);
  return QUERY_OpenServiceQuery ("PubFetch", query, 30);
}

NLM_EXTERN CONN PubSeqFetchOpenConnection (
  Int4 uid,
  Int2 retcode,
  Int4 flags
)

{
  Char  query [64];

  if (uid < 1) return NULL;
  if (retcode < 0 || retcode > 4) {
    retcode = 0;
  }
  if (flags < 0) {
    flags = 1;
  }

#ifdef PUB_SEQ_FETCH_DEBUG
  sprintf (query, "val=%ld&save=idf&view=1&maxplex=%d&extrafeat=%ld", (long) uid, (int) retcode, (long) flags);
  return QUERY_OpenUrlQuery ("www.ncbi.nlm.nih.gov", 80, "/entrez/viewer.fcgi",
                             query, "Entrez2Tool", 30, eMIME_T_NcbiData,
                             eMIME_AsnText, eENCOD_None, 0);
#endif

  sprintf (query, "val=%ld&maxplex=%d&extrafeat=%ld", (long) uid, (int) retcode, (long) flags);
  return QUERY_OpenServiceQuery ("SeqFetch", query, 30);
}

static CharPtr girevtxt = "cmd=seqid&txt=on&seqid=fasta&val=";

NLM_EXTERN CONN GiRevHistOpenConnection (
  Int4 uid,
  Int4 num,
  Int4Ptr uids
)

{
  CONN     conn;
  Int4     i;
  CharPtr  query;
  Char     tmp [16];

  if (uid > 0 && uids == NULL) {
    uids = &uid;
    num = 1;
  }
  if (uids == NULL || num < 1) return NULL;

  query = (CharPtr) MemNew (11 * num + StringLen (girevtxt) + 5);
  if (query == NULL) return NULL;
  /*
  StringCpy (query, girevtxt);
  */
  StringCpy (query, "val=");

  sprintf (tmp, "%ld", (long) uids [0]);
  StringCat (query, tmp);

  for (i = 1; i < num; i++) {
    sprintf (tmp, ",%ld", (long) uids [i]);
    StringCat (query, tmp);
  }

  /*
  conn = QUERY_OpenUrlQuery ("www.ncbi.nlm.nih.gov", 80, "/entrez/sutils/girevhist.cgi",
                             query, "Entrez2Tool", 30, eMIME_T_NcbiData,
                             eMIME_Plain, eENCOD_None, 0);
  */

  conn = QUERY_OpenServiceQuery ("Gi2Accn", query, 30);

  MemFree (query);
  return conn;
}

NLM_EXTERN CONN AccnRevHistOpenConnection (
  CharPtr accn
)

{
  Char  query [64];

  if (StringHasNoText (accn)) return NULL;
  
  /*
  sprintf (query, "val=%s&save=idf&view=gi&maxplex=0", accn);
  return QUERY_OpenUrlQuery ("www.ncbi.nlm.nih.gov", 80, "/entrez/viewer.fcgi",
                             query, "Entrez2Tool", 30, eMIME_T_NcbiData,
                             eMIME_Plain, eENCOD_None, 0);
  */

  sprintf (query, "val=%s", accn);
  return QUERY_OpenServiceQuery ("Accn2Gi", query, 30);
}

NLM_EXTERN CONN GiSeqIdSetOpenConnection (
  Int4 gi
)

{
  Char  query [64];

  if (gi < 1) return NULL;

  /*
  sprintf (query, "cmd=seqid&txt=on&val=%ld&seqid=asntext&os=PUBSEQ_OS", (long) gi);
  return QUERY_OpenUrlQuery ("www.ncbi.nlm.nih.gov", 80, "/entrez/sutils/girevhist.cgi",
                             query, "Entrez2Tool", 30, eMIME_T_NcbiData,
                             eMIME_Plain, eENCOD_None, 0);
  */

  sprintf (query, "val=%ld", (long) gi);
  return QUERY_OpenServiceQuery ("Gi2SeqIdSet", query, 30);
}

static CharPtr accnlsttxt = "cmd=seqid&txt=on&seqid=fastalong&val=";

NLM_EXTERN CONN AccnListOpenConnection (
  CharPtr PNTR accns
)

{
  CONN     conn;
  Int4     i;
  size_t   len;
  Int4     num;
  CharPtr  query;
  CharPtr  tmp;

  if (accns == NULL) return NULL;

  for (num = 0, len = 0; accns [num] != NULL; num++) {
    len += StringLen (accns [num]) + 1;
  }
  if (num < 1) return NULL;

  query = (CharPtr) MemNew (len + StringLen (accnlsttxt) + 5);
  if (query == NULL) return NULL;
  tmp = query;

  /*
  tmp = StringMove (tmp, accnlsttxt);
  */
  tmp = StringMove (tmp, "val=");

  tmp = StringMove (tmp, accns [0]);

  for (i = 1; accns [i] != NULL; i++) {
    tmp = StringMove (tmp, ",");
    tmp = StringMove (tmp, accns [i]);
  }

  /*
  conn = QUERY_OpenUrlQuery ("www.ncbi.nlm.nih.gov", 80, "/entrez/sutils/girevhist.cgi",
                             query, "Entrez2Tool", 30, eMIME_T_NcbiData,
                             eMIME_Plain, eENCOD_None, 0);
  */

  conn = QUERY_OpenServiceQuery ("AccnList", query, 30);

  MemFree (query);
  return conn;
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
  while ((status = CONN_Wait (conn, eIO_Read, &timeout)) == eIO_Timeout && max < 300) {
    currtime = GetSecs ();
    max = currtime - starttime;
#ifdef OS_MAC
    WaitNextEvent (0, &currEvent, 0, NULL);
#endif
  }

  return status;
}

NLM_EXTERN PubmedEntryPtr PubMedReadReply (
  CONN conn,
  EIO_Status status
)

{
  AsnIoConnPtr    aicp;
  PubmedEntryPtr  pep = NULL;

  if (conn != NULL && status == eIO_Success) {
    aicp = QUERY_AsnIoConnOpen ("r", conn);
    pep = PubmedEntryAsnRead (aicp->aip, NULL);
    QUERY_AsnIoConnClose (aicp);
  }

  if (pep != NULL) {
    ChangeMedlineAuthorsToISO ((MedlineEntryPtr) pep->medent);
  }

  return pep;
}

NLM_EXTERN PubmedEntryPtr PubMedWaitForReply (
  CONN conn
)

{
  PubmedEntryPtr  pep = NULL;

  if (conn == NULL) return NULL;

  if (CommonWaitForReply (conn) == eIO_Success) {
    pep = PubMedReadReply (conn, eIO_Success);
  }
  CONN_Close (conn);

  return pep;
}

#define PUB_SEQ_DEBUG_TEMP_FILE "pubseqfetchtempfile"

NLM_EXTERN SeqEntryPtr PubSeqReadReply (
  CONN conn,
  EIO_Status status
)

{
  AsnIoConnPtr  aicp;
  ErrSev        oldsev;
  SeqEntryPtr   sep = NULL;
#ifdef PUB_SEQ_FETCH_DEBUG
  AsnIoPtr      aip;
  FILE          *fp;
#endif

  if (conn != NULL && status == eIO_Success) {
#ifdef PUB_SEQ_FETCH_DEBUG
    fp = FileOpen (PUB_SEQ_DEBUG_TEMP_FILE, "w");
    QUERY_CopyResultsToFile (conn, fp);
    FileClose (fp);
    aip = AsnIoOpen (PUB_SEQ_DEBUG_TEMP_FILE, "r");
    sep = SeqEntryAsnRead (aip, NULL);
    AsnIoClose (aip);
    if (sep != NULL) {
      FileRemove (PUB_SEQ_DEBUG_TEMP_FILE);
    }
#else
    oldsev = ErrSetMessageLevel (SEV_MAX);
    aicp = QUERY_AsnIoConnOpen ("rb", conn);
    sep = SeqEntryAsnRead (aicp->aip, NULL);
    QUERY_AsnIoConnClose (aicp);
    ErrSetMessageLevel (oldsev);
#endif
  }
  return sep;
}

NLM_EXTERN SeqEntryPtr PubSeqWaitForReply (
  CONN conn
)

{
  SeqEntryPtr  sep = NULL;

  if (conn == NULL) return NULL;

  if (CommonWaitForReply (conn) == eIO_Success) {
    sep = PubSeqReadReply (conn, eIO_Success);
  }
  CONN_Close (conn);

  return sep;
}

NLM_EXTERN CharPtr GiRevHistReadReply (
  CONN conn,
  EIO_Status status
)

{
  AsnIoConnPtr  aicp;
  ByteStorePtr  bsp;
  Char          buf [512];
  size_t        n_read;
  CharPtr       str = NULL;

  if (conn != NULL && status == eIO_Success) {
    bsp = BSNew (512);
    aicp = QUERY_AsnIoConnOpen ("r", conn);
    while ((status = CONN_Read (aicp->conn, (Pointer) buf, sizeof (buf), &n_read, eIO_ReadPlain)) == eIO_Success) {
      BSWrite (bsp, buf, n_read);
    }
    str = BSMerge (bsp, NULL);
    BSFree (bsp);
    QUERY_AsnIoConnClose (aicp);
  }
  return str;
}

NLM_EXTERN CharPtr GiRevHistWaitForReply (
  CONN conn
)

{
  CharPtr  str = NULL;

  if (conn == NULL) return NULL;

  if (CommonWaitForReply (conn) == eIO_Success) {
    str = GiRevHistReadReply (conn, eIO_Success);
  }
  CONN_Close (conn);

  return str;
}

NLM_EXTERN Int4 AccnRevHistReadReply (
  CONN conn,
  EIO_Status status
)

{
  AsnIoConnPtr  aicp;
  Char          buf [32];
  Char          ch;
  Int4          gi = 0;
  size_t        n_read;
  CharPtr       ptr;
  long int      val;

  if (conn != NULL && status == eIO_Success) {
    aicp = QUERY_AsnIoConnOpen ("r", conn);
    status = CONN_Read (aicp->conn, (Pointer) buf, sizeof (buf), &n_read, eIO_ReadPlain);
    if (status == eIO_Success) {
      ptr = buf;
      ch = *ptr;
      while (ch != '\0' && ch != '\n' && ch != '\r') {
         ptr++;
         ch = *ptr;
      }
      *ptr = '\0';
      if (sscanf (buf, "%ld", &val) == 1 && val > 0) {
        gi = val;
      }
    }
    QUERY_AsnIoConnClose (aicp);
  }
  return gi;
}

NLM_EXTERN Int4 AccnRevHistWaitForReply (
  CONN conn
)

{
  Int4  gi = 0;

  if (conn == NULL) return 0;

  if (CommonWaitForReply (conn) == eIO_Success) {
    gi = AccnRevHistReadReply (conn, eIO_Success);
  }
  CONN_Close (conn);

  return gi;
}

NLM_EXTERN SeqIdPtr GiSeqIdSetReadReply (
  CONN conn,
  EIO_Status status
)

{
  AsnIoConnPtr  aicp;
  SeqIdPtr      last = NULL;
  ErrSev        sev;
  SeqIdPtr      sid;
  SeqIdPtr      sip = NULL;

  if (conn != NULL && status == eIO_Success) {
    aicp = QUERY_AsnIoConnOpen ("r", conn);
    sev = ErrSetLogLevel (SEV_WARNING);
    while ((sid = SeqIdAsnRead (aicp->aip, NULL)) != NULL) {
      if (sip == NULL) {
        sip = sid;
      } else if (last != NULL) {
        last->next = sid;
      }
      last = sid;
    }
    ErrSetLogLevel (sev);
    QUERY_AsnIoConnClose (aicp);
  }
  return sip;
}

NLM_EXTERN SeqIdPtr GiSeqIdSetWaitForReply (
  CONN conn
)

{
  SeqIdPtr  sip = NULL;

  if (conn == NULL) return NULL;

  if (CommonWaitForReply (conn) == eIO_Success) {
    sip = GiSeqIdSetReadReply (conn, eIO_Success);
  }
  CONN_Close (conn);

  return sip;
}

NLM_EXTERN CharPtr AccnListReadReply (
  CONN conn,
  EIO_Status status
)

{
  AsnIoConnPtr  aicp;
  ByteStorePtr  bsp;
  Char          buf [512];
  size_t        n_read;
  CharPtr       str = NULL;

  if (conn != NULL && status == eIO_Success) {
    bsp = BSNew (512);
    aicp = QUERY_AsnIoConnOpen ("r", conn);
    while ((status = CONN_Read (aicp->conn, (Pointer) buf, sizeof (buf), &n_read, eIO_ReadPlain)) == eIO_Success) {
      BSWrite (bsp, buf, n_read);
    }
    str = BSMerge (bsp, NULL);
    BSFree (bsp);
    QUERY_AsnIoConnClose (aicp);
  }
  return str;
}

NLM_EXTERN CharPtr AccnListWaitForReply (
  CONN conn
)

{
  CharPtr  str = NULL;

  if (conn == NULL) return NULL;

  if (CommonWaitForReply (conn) == eIO_Success) {
    str = AccnListReadReply (conn, eIO_Success);
  }
  CONN_Close (conn);

  return str;
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

  conn = PubMedFetchOpenConnection (uid);

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

typedef struct psconfirm {
  Int4  uid;
  Int4  gi;
} PsConfirm, PNTR PsConfirmPtr;

static void ConfirmGiInSep (
  BioseqPtr bsp,
  Pointer userdata
)

{
  Int4          gi;
  PsConfirmPtr  psp;
  SeqIdPtr      sip;

  if (bsp == NULL || userdata == NULL) return;
  psp = (PsConfirmPtr) userdata;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice != SEQID_GI) continue;
    gi = (Int4) sip->data.intvalue;
    if (psp->gi == 0 || gi == psp->uid) {
      psp->gi = gi;
    }
  }
}

NLM_EXTERN SeqEntryPtr PubSeqSynchronousQuery (
  Int4 uid,
  Int2 retcode,
  Int4 flags
)

{
  CONN         conn;
  PsConfirm    ps;
  SeqEntryPtr  sep;
#ifdef OS_UNIX
  Boolean      logtimes;
  clock_t      starttime;
  clock_t      stoptime;
  struct tms   timebuf;
#endif

  if (uid < 1) return NULL;

#ifdef OS_UNIX
  logtimes = (Boolean) ((getenv ("NCBI_LOG_SYNC_QUERY_TIMES")) != NULL);
#endif

  conn = PubSeqFetchOpenConnection (uid, retcode, flags);

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

  if (sep != NULL) {
    ps.uid = uid;
    ps.gi = 0;
    VisitBioseqsInSep (sep, (Pointer) &ps, ConfirmGiInSep);
    if (ps.gi != uid) {
      ErrPostEx (SEV_ERROR, 0, 0,
                 "PubSeqSynchronousQuery requested gi %ld but received gi %ld",
                 (long) uid, (long) ps.gi);
    }
  } else {
    ErrPostEx (SEV_ERROR, 0, 0, "PubSeqSynchronousQuery failed for gi %ld", (long) uid);
  }

  return sep;
}

NLM_EXTERN CharPtr GiRevHistSynchronousQuery (
  Int4 uid,
  Int4 num,
  Int4Ptr uids
)

{
  CONN        conn;
  CharPtr     str;
#ifdef OS_UNIX
  Boolean     logtimes;
  clock_t     starttime;
  clock_t     stoptime;
  struct tms  timebuf;
#endif

#ifdef OS_UNIX
  logtimes = (Boolean) ((getenv ("NCBI_LOG_SYNC_QUERY_TIMES")) != NULL);
#endif

  conn = GiRevHistOpenConnection (uid, num, uids);

  if (conn == NULL) return NULL;

  QUERY_SendQuery (conn);

#ifdef OS_UNIX
  if (logtimes) {
    starttime = times (&timebuf);
  }
#endif

  str = GiRevHistWaitForReply (conn);

#ifdef OS_UNIX
  if (logtimes) {
    stoptime = times (&timebuf);
    printf ("GiRevHistWaitForReply %ld\n", (long) (stoptime - starttime));
  }
#endif

  return str;
}

NLM_EXTERN Int4 AccnRevHistSynchronousQuery (
  CharPtr accn
)

{
  CONN        conn;
  Int4        gi;
#ifdef OS_UNIX
  Boolean     logtimes;
  clock_t     starttime;
  clock_t     stoptime;
  struct tms  timebuf;
#endif

#ifdef OS_UNIX
  logtimes = (Boolean) ((getenv ("NCBI_LOG_SYNC_QUERY_TIMES")) != NULL);
#endif

  conn = AccnRevHistOpenConnection (accn);

  if (conn == NULL) return 0;

  QUERY_SendQuery (conn);

#ifdef OS_UNIX
  if (logtimes) {
    starttime = times (&timebuf);
  }
#endif

  gi = AccnRevHistWaitForReply (conn);

#ifdef OS_UNIX
  if (logtimes) {
    stoptime = times (&timebuf);
    printf ("AccnRevHistWaitForReply %ld\n", (long) (stoptime - starttime));
  }
#endif

  return gi;
}

NLM_EXTERN SeqIdPtr GiSeqIdSetSynchronousQuery (
  Int4 gi
)

{
  CONN        conn;
  SeqIdPtr    sip;
#ifdef OS_UNIX
  Boolean     logtimes;
  clock_t     starttime;
  clock_t     stoptime;
  struct tms  timebuf;
#endif

#ifdef OS_UNIX
  logtimes = (Boolean) ((getenv ("NCBI_LOG_SYNC_QUERY_TIMES")) != NULL);
#endif

  conn = GiSeqIdSetOpenConnection (gi);

  if (conn == NULL) return NULL;

  QUERY_SendQuery (conn);

#ifdef OS_UNIX
  if (logtimes) {
    starttime = times (&timebuf);
  }
#endif

  sip = GiSeqIdSetWaitForReply (conn);

#ifdef OS_UNIX
  if (logtimes) {
    stoptime = times (&timebuf);
    printf ("GiSeqIdSetWaitForReply %ld\n", (long) (stoptime - starttime));
  }
#endif

  return sip;
}

NLM_EXTERN CharPtr AccnListSynchronousQuery (
  CharPtr PNTR accns
)

{
  CONN        conn;
  CharPtr     str;
#ifdef OS_UNIX
  Boolean     logtimes;
  clock_t     starttime;
  clock_t     stoptime;
  struct tms  timebuf;
#endif

#ifdef OS_UNIX
  logtimes = (Boolean) ((getenv ("NCBI_LOG_SYNC_QUERY_TIMES")) != NULL);
#endif

  conn = AccnListOpenConnection (accns);

  if (conn == NULL) return NULL;

  QUERY_SendQuery (conn);

#ifdef OS_UNIX
  if (logtimes) {
    starttime = times (&timebuf);
  }
#endif

  str = AccnListWaitForReply (conn);

#ifdef OS_UNIX
  if (logtimes) {
    stoptime = times (&timebuf);
    printf ("AccnListWaitForReply %ld\n", (long) (stoptime - starttime));
  }
#endif

  return str;
}

NLM_EXTERN Boolean PubMedAsynchronousQuery (
  Int4 uid,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
)

{
  CONN  conn;

  conn = PubMedFetchOpenConnection (uid);

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

NLM_EXTERN Boolean PubSeqAsynchronousQuery (
  Int4 uid,
  Int2 retcode,
  Int4 flags,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
)

{
  CONN  conn;

  conn = PubSeqFetchOpenConnection (uid, retcode, flags);

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

NLM_EXTERN Boolean GiRevHistAsynchronousQuery (
  Int4 uid,
  Int4 num,
  Int4Ptr uids,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
)

{
  CONN  conn;

  conn = GiRevHistOpenConnection (uid, num, uids);

  if (conn == NULL) return FALSE;

  QUERY_SendQuery (conn);

  QUERY_AddToQueue (queue, conn, resultproc, userdata, TRUE);

  return TRUE;
}

NLM_EXTERN Int4 GiRevHistCheckQueue (
  QUEUE* queue
)

{
  return QUERY_CheckQueue (queue);
}

NLM_EXTERN Boolean AccnRevHistAsynchronousQuery (
  CharPtr accn,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
)

{
  CONN  conn;

  conn = AccnRevHistOpenConnection (accn);

  if (conn == NULL) return FALSE;

  QUERY_SendQuery (conn);

  QUERY_AddToQueue (queue, conn, resultproc, userdata, TRUE);

  return TRUE;
}

NLM_EXTERN Int4 AccnRevHistCheckQueue (
  QUEUE* queue
)

{
  return QUERY_CheckQueue (queue);
}

NLM_EXTERN Boolean GiSeqIdSetAsynchronousQuery (
  Int4 gi,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
)

{
  CONN  conn;

  conn = GiSeqIdSetOpenConnection (gi);

  if (conn == NULL) return FALSE;

  QUERY_SendQuery (conn);

  QUERY_AddToQueue (queue, conn, resultproc, userdata, TRUE);

  return TRUE;
}

NLM_EXTERN Int4 GiSeqIdSetCheckQueue (
  QUEUE* queue
)

{
  return QUERY_CheckQueue (queue);
}

NLM_EXTERN Boolean AccnListAsynchronousQuery (
  CharPtr PNTR accns,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
)

{
  CONN  conn;

  conn = AccnListOpenConnection (accns);

  if (conn == NULL) return FALSE;

  QUERY_SendQuery (conn);

  QUERY_AddToQueue (queue, conn, resultproc, userdata, TRUE);

  return TRUE;
}

NLM_EXTERN Int4 AccnListCheckQueue (
  QUEUE* queue
)

{
  return QUERY_CheckQueue (queue);
}

/* object manager registerable fetch function */

static CharPtr pubseqfetchproc = "PubSeqBioseqFetch";
static CharPtr pubseqseqidtogi = "PubSeqSeqIdForGi";
static CharPtr pubseqgitoseqid = "PubSeqGiForSeqId";

static Int2 LIBCALLBACK PubSeqBioseqFetchFunc (Pointer data)

{
  BioseqPtr         bsp;
  Int4              flags = -1;
  Char              id [41];
  OMUserDataPtr     omdp = NULL;
  OMProcControlPtr  ompcp;
  ObjMgrProcPtr     ompp;
  Int2              retcode = 0;
  SeqEntryPtr       sep = NULL;
  SeqIdPtr          sid;
  SeqIdPtr          sip;
  Int4              uid = 0;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  ompp = ompcp->proc;
  if (ompp == NULL) return OM_MSG_RET_ERROR;
  sip = (SeqIdPtr) ompcp->input_data;
  if (sip == NULL) return OM_MSG_RET_ERROR;

  omdp = ObjMgrGetUserData (ompcp->input_entityID, ompp->procid, OMPROC_FETCH, 0);
  if (omdp != NULL) {
    uid = omdp->userdata.intvalue;
    if (uid == 0) return OM_MSG_RET_ERROR;
  } else if (sip->choice == SEQID_GI) {
    uid = sip->data.intvalue;
  } else if (sip->choice != SEQID_LOCAL) {
    sid = SeqIdDup (sip);
    SeqIdWrite (sid, id, PRINTID_FASTA_SHORT, sizeof (id) - 1);
    SeqIdFree (sid);
    uid = AccnRevHistSynchronousQuery (id);
  }

  if (uid == 0) return OM_MSG_RET_OK;

  sep = PubSeqSynchronousQuery (uid, retcode, flags);

  if (sep == NULL) return OM_MSG_RET_OK;
  bsp = BioseqFindInSeqEntry (sip, sep);
  ompcp->output_data = (Pointer) bsp;
  ompcp->output_entityID = ObjMgrGetEntityIDForChoice (sep);

  omdp = ObjMgrAddUserData (ompcp->output_entityID, ompp->procid, OMPROC_FETCH, 0);
  if (omdp != NULL) {
    omdp->userdata.intvalue = uid;
  }

  return OM_MSG_RET_DONE;
}

static Int2 LIBCALLBACK PubSeqSeqIdForGiFunc (Pointer data)

{
  Char              ch;
  OMProcControlPtr  ompcp;
  ObjMgrProcPtr     ompp;
  CharPtr           ptr;
  SeqIdPtr          sid;
  SeqIdPtr          sip;
  CharPtr           str;
  Int4              uid;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  ompp = ompcp->proc;
  if (ompp == NULL) return OM_MSG_RET_ERROR;
  sip = (SeqIdPtr) ompcp->input_data;
  if (sip == NULL) return OM_MSG_RET_ERROR;

  if (sip->choice != SEQID_GI) return OM_MSG_RET_ERROR;
  uid = (Int4) sip->data.intvalue;
  if (uid < 1) return OM_MSG_RET_ERROR;

  str = GiRevHistSynchronousQuery (uid, 0, NULL);
  if (str == NULL) return OM_MSG_RET_ERROR;

  ptr = str;
  ch = *ptr;
  while (ch != '\0' && ch != '\n' && ch != '\r') {
    ptr++;
    ch = *ptr;
  }
  *ptr = '\0';

  if (StringNCmp (str, "ERROR", 5) != 0) {
    sid = SeqIdParse (str);
  }
  MemFree (str);

  if (sid == NULL) return OM_MSG_RET_ERROR;

  ompcp->output_data = (Pointer) sid;
  return OM_MSG_RET_DONE;
}

static Int2 LIBCALLBACK PubSeqGiForSeqIdFunc (Pointer data)

{
  Char              buf [41];
  Int4              gi;
  OMProcControlPtr  ompcp;
  ObjMgrProcPtr     ompp;
  SeqIdPtr          sid;
  SeqIdPtr          sip;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL) return OM_MSG_RET_ERROR;
  ompp = ompcp->proc;
  if (ompp == NULL) return OM_MSG_RET_ERROR;
  sip = (SeqIdPtr) ompcp->input_data;
  if (sip == NULL) return OM_MSG_RET_ERROR;

  if (sip->choice == SEQID_GI) return OM_MSG_RET_ERROR;
  if (sip->choice == SEQID_LOCAL) return OM_MSG_RET_OK;
  SeqIdWrite (sip, buf, PRINTID_FASTA_SHORT, sizeof (buf));
  if (StringHasNoText (buf)) return OM_MSG_RET_ERROR;

  gi = AccnRevHistSynchronousQuery (buf);
  if (gi < 1) return OM_MSG_RET_ERROR;

  sid = ValNodeNew (NULL);
  if (sid == NULL) return OM_MSG_RET_ERROR;
  sid->choice = SEQID_GI;
  sid->data.intvalue = gi;
  sid->next = NULL;

  ompcp->output_data = (Pointer) sid;
  return OM_MSG_RET_DONE;
}

NLM_EXTERN Boolean PubSeqFetchEnable (void)

{
  ObjMgrProcLoad (OMPROC_FETCH, pubseqfetchproc, pubseqfetchproc,
                  OBJ_SEQID, 0, OBJ_BIOSEQ, 0, NULL,
                  PubSeqBioseqFetchFunc, PROC_PRIORITY_DEFAULT);

  ObjMgrProcLoad (OMPROC_FETCH, pubseqseqidtogi, pubseqseqidtogi,
                  OBJ_SEQID, SEQID_GI, OBJ_SEQID, 0, NULL,
                  PubSeqSeqIdForGiFunc, PROC_PRIORITY_DEFAULT);

  ObjMgrProcLoad (OMPROC_FETCH, pubseqgitoseqid, pubseqgitoseqid,
                  OBJ_SEQID, 0, OBJ_SEQID, SEQID_GI, NULL,
                  PubSeqGiForSeqIdFunc, PROC_PRIORITY_DEFAULT);

  SeqMgrSetPreCache (GiRevHistLookupFarSeqIDs);

  SeqMgrSetSeqIdSetFunc (GiRevHistLookupSeqIdSet);

  return TRUE;
}

NLM_EXTERN void PubSeqFetchDisable (void)

{
  ObjMgrPtr      omp;
  ObjMgrProcPtr  ompp;

  omp = ObjMgrGet ();
  ompp = ObjMgrProcFind (omp, 0, pubseqfetchproc, OMPROC_FETCH);
  if (ompp != NULL) {
    ObjMgrFreeUserData (0, ompp->procid, OMPROC_FETCH, 0);
  }
  ompp = ObjMgrProcFind (omp, 0, pubseqseqidtogi, OMPROC_FETCH);
  if (ompp != NULL) {
    ObjMgrFreeUserData (0, ompp->procid, OMPROC_FETCH, 0);
  }
  ompp = ObjMgrProcFind (omp, 0, pubseqgitoseqid, OMPROC_FETCH);
  if (ompp != NULL) {
    ObjMgrFreeUserData (0, ompp->procid, OMPROC_FETCH, 0);
  }

  SeqMgrSetPreCache (NULL);

  SeqMgrSetSeqIdSetFunc (NULL);
}

/* multiple Accession preload into cache or report function */

NLM_EXTERN Int4 CacheAccnList (
  CharPtr str,
  CacheAccnListProc userfunc
)

{
  SeqIdPtr  accn;
  Char      buf [41];
  Char      ch;
  Int4      count = 0;
  Int4      gi;
  Int4      i;
  SeqIdPtr  ids;
  CharPtr   ptr;
  SeqIdPtr  sip;
  CharPtr   tmp;

  if (str == NULL) return 0;

  /* parse output */

  i = 0;
  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    tmp = ptr;
    while (ch != '\0' && ch != '\n' && ch != '\r') {
      ptr++;
      ch = *ptr;
    }
    *ptr = '\0';
    while (ch == '\n' || ch == '\r') {
      ptr++;
      ch = *ptr;
    }
    if (StringNCmp (tmp, "ERROR", 5) != 0) {
      ids = SeqIdParse (tmp);
      if (ids != NULL) {
        gi = 0;
        accn = NULL;
        for (sip = ids; sip != NULL; sip = sip->next) {
          switch (sip->choice) {
            case SEQID_GI :
              gi = (Int4) sip->data.intvalue;
              break;
            case SEQID_GENBANK :
            case SEQID_EMBL :
            case SEQID_PIR :
            case SEQID_SWISSPROT :
            case SEQID_PATENT :
            case SEQID_OTHER :
            case SEQID_DDBJ :
            case SEQID_PRF :
            case SEQID_TPG :
            case SEQID_TPE :
            case SEQID_TPD :
              accn = sip;
              break;
            default :
              break;
          }
        }
        if (gi > 0 && accn != NULL) {

          if (userfunc != NULL) {

            SeqIdWrite (accn, buf, PRINTID_TEXTID_ACC_VER, sizeof (buf) - 1);
            if (! StringHasNoText (buf)) {
              userfunc (gi, buf);
              count++;
            }

          } else {

            /* safety check before recording in cache */

            if (! FetchFromSeqIdGiCache (gi, NULL)) {
              RecordInSeqIdGiCache (gi, accn);
              count++;
            }
          }
        }
      }
    }
    i++;
  }

  return count;
}

/* multiple SeqId preload into cache function */

static int LIBCALLBACK SortByInt4 (VoidPtr vp1, VoidPtr vp2)

{
  Int4Ptr ip1 = (Int4Ptr) vp1;
  Int4Ptr ip2 = (Int4Ptr) vp2;

  if (ip1 == NULL || ip2 == NULL) return 0;
  if (*ip1 > *ip2) return 1;
  if (*ip1 < *ip2) return -1;
  return 0;
}

static Int4 IntPreLoadSeqIdGiCache (
  Int4 num,
  Int4Ptr ids
)

{
  Char      ch;
  Int4      count = 0;
  Int4      gi;
  Int4      i;
  CharPtr   ptr;
  SeqIdPtr  sip;
  CharPtr   str;
  CharPtr   tmp;

  if (num < 1 || num > 100 || ids == NULL) return 0;

  /* perform multiple gi to SeqId query */

  str = GiRevHistSynchronousQuery (0, num, ids);
  if (str == NULL) return 0;

  /* parse output */

  i = 0;
  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    tmp = ptr;
    while (ch != '\0' && ch != '\n' && ch != '\r') {
      ptr++;
      ch = *ptr;
    }
    *ptr = '\0';
    while (ch == '\n' || ch == '\r') {
      ptr++;
      ch = *ptr;
    }
    if (StringNCmp (tmp, "ERROR", 5) != 0) {
      gi = ids [i];

      /* safety check before recording in cache */

      if (! FetchFromSeqIdGiCache (gi, NULL)) {
        sip = SeqIdParse (tmp);
        if (sip != NULL) {
          RecordInSeqIdGiCache (gi, sip);
          SeqIdFree (sip);
          count++;
        }
      }
    }
    i++;
  }

  MemFree (str);

  return count;
}

static Int4 FilterKnownGis (
  Int4 num,
  Int4Ptr uniq
)

{
  BioseqPtr  bsp;
  Int4       gi;
  Int4       i;
  Int4       ids [101];
  Int4       j;
  SeqIdPtr   sip;
  ValNode    vn;

  if (num < 1 || num > 100 || uniq == NULL) return 0;

  MemSet ((Pointer) &vn, 0, sizeof (ValNode));
  vn.choice = SEQID_GI;

  /* eliminate uids we already know, load ids list with remainder */

  for (i = 0, j = 0; i < num; i++) {
    gi = uniq [i];
    if (gi < 1) continue;

    vn.data.intvalue = gi;
    bsp = BioseqFindCore (&vn);
    if (bsp != NULL) continue; /* already loaded */

    sip = NULL;
    if (FetchFromSeqIdGiCache (gi, NULL)) continue; /* already cached */

    ids [j] = gi;
    j++;
  }

  if (j < 1) return 0;

  return IntPreLoadSeqIdGiCache (j, ids);
}

static Int4 UniqPreLoadList (
  Int4 num,
  Int4Ptr srted
)

{
  Int4  gi;
  Int4  i;
  Int4  j;
  Int4  last;
  Int4  uniq [101];

  if (num < 1 || num > 100 || srted == NULL) return 0;

  /* unique uid list */

  last = 0;
  for (i = 0, j = 0; i < num; i++) {
    gi = srted [i];
    if (gi != last) {
      uniq [j] = gi;
      j++;
    }
    last = gi;
  }
  num = j;

  return FilterKnownGis (num, uniq);
}

static Int4 SortPreLoadList (
  Int4 num,
  Int4Ptr raw
)

{
  Int4  i;
  Int4  uids [101];

  if (num < 1 || num > 100 || raw == NULL) return 0;

  /* copy raw uid list */

  for (i = 0; i < num; i++) {
    uids [i] = raw [i];
  }

  /* sort by gi value */

  HeapSort ((Pointer) uids, (size_t) num, sizeof (Int4), SortByInt4);

  return UniqPreLoadList (num, uids);
}

NLM_EXTERN Int4 GiRevHistPreLoadSeqIdGiCache (
  Int4 num,
  Int4Ptr uids
)

{
  Int4  count;
  Int4  offset;
  Int4  rsult = 0;

  /* split into queries of 100 or fewer uids at a time */

  if (num < 1 || uids == NULL) return 0;
  count = (Int4) MIN (num, 100L);
  offset = 0;
  while (count > 0 && num > 0) {
    rsult += SortPreLoadList (count, uids + offset);
    offset += count;
    num -= count;
    count = (Int4) MIN (num, 100L);
  }
  return rsult;
}

typedef struct fariddata {
  Int4  num;
  Int4  lastgi;
  Int4  total;
  Int4  uids [101];
} FarIdData, PNTR FarIDPtr;

static void LookupSegments (SeqLocPtr slp, SeqIdPtr sip, Pointer userdata)

{
  FarIDPtr   fip;
  Int4       gi;
  Int2       i;
  SeqLocPtr  loc;

  if (slp == NULL && sip == NULL) return;
  if (userdata == NULL) return;
  fip = (FarIDPtr) userdata;

  if (sip == NULL) {
    sip = SeqLocId (slp);
    if (sip == NULL) {
      loc = SeqLocFindNext (slp, NULL);
      if (loc != NULL) {
        sip = SeqLocId (loc);
      }
    }
  }
  if (sip == NULL) return;

  if (sip->choice != SEQID_GI) return;
  gi = sip->data.intvalue;
  if (gi < 1) return;

  /* do not add if already in list */

  if (gi == fip->lastgi) return;

  for (i = 0; i < fip->num; i++) {
    if (gi == fip->uids [i]) return;
  }

  if (fip->num > 99) {
    fip->total += GiRevHistPreLoadSeqIdGiCache (fip->num, fip->uids);
    fip->num = 0;
  }

  fip->uids [fip->num] = gi;
  (fip->num)++;
  fip->lastgi = gi;
}

static void LookupBioseqs (BioseqPtr bsp, Pointer userdata)

{
  DeltaSeqPtr  dsp;
  SeqLocPtr    slp = NULL;
  ValNode      vn;

  if (bsp == NULL) return;

  if (bsp->repr == Seq_repr_seg) {
    vn.choice = SEQLOC_MIX;
    vn.extended = 0;
    vn.data.ptrvalue = bsp->seq_ext;
    vn.next = NULL;
    while ((slp = SeqLocFindNext (&vn, slp)) != NULL) {
      if (slp != NULL && slp->choice != SEQLOC_NULL) {
        LookupSegments (slp, NULL, userdata);
      }
    }
  } else if (bsp->repr == Seq_repr_delta) {
    for (dsp = (DeltaSeqPtr) (bsp->seq_ext); dsp != NULL; dsp = dsp->next) {
      if (dsp->choice == 1) {
        slp = (SeqLocPtr) dsp->data.ptrvalue;
        if (slp != NULL && slp->choice != SEQLOC_NULL) {
          LookupSegments (slp, NULL, userdata);
        }
      }
    }
  }
}

static void LookupLocations (SeqFeatPtr sfp, Pointer userdata)

{
  SeqLocPtr  slp = NULL;

  if (sfp == NULL) return;

  while ((slp = SeqLocFindNext (sfp->location, slp)) != NULL) {
    if (slp != NULL && slp->choice != SEQLOC_NULL) {
      LookupSegments (slp, NULL, userdata);
    }
  }
}

static void LookupProducts (SeqFeatPtr sfp, Pointer userdata)

{
  SeqLocPtr  slp = NULL;

  if (sfp == NULL) return;

  while ((slp = SeqLocFindNext (sfp->product, slp)) != NULL) {
    if (slp != NULL && slp->choice != SEQLOC_NULL) {
      LookupSegments (slp, NULL, userdata);
    }
  }
}

static void LookupAlignments (SeqAlignPtr sap, Pointer userdata)

{
  DenseDiagPtr  ddp;
  DenseSegPtr   dsp;
  SeqIdPtr      sip;
  SeqLocPtr     slp = NULL;
  StdSegPtr     ssp;

  if (sap == NULL) return;

  LookupSegments (sap->bounds, NULL, userdata);
  if (sap->segs == NULL) return;

  switch (sap->segtype) {
    case SAS_DENDIAG :
      ddp = (DenseDiagPtr) sap->segs;
      if (ddp != NULL) {
        for (sip = ddp->id; sip != NULL; sip = sip->next) {
          LookupSegments (NULL, sip, userdata);
        }
      }
      break;
    case SAS_DENSEG :
      dsp = (DenseSegPtr) sap->segs;
      if (dsp != NULL) {
        for (sip = dsp->ids; sip != NULL; sip = sip->next) {
          LookupSegments (NULL, sip, userdata);
        }
      }
      break;
    case SAS_STD :
      ssp = (StdSegPtr) sap->segs;
      for (slp = ssp->loc; slp != NULL; slp = slp->next) {
        LookupSegments (slp, NULL, userdata);
      }
      break;
    case SAS_DISC :
      /* recursive */
      for (sap = (SeqAlignPtr) sap->segs; sap != NULL; sap = sap->next) {
        LookupAlignments (sap, userdata);
      }
      break;
    default :
      break;
  }
}

static void LookupHistory (BioseqPtr bsp, Pointer userdata)

{
  SeqHistPtr  hist;
  SeqIdPtr    sip;

  if (bsp == NULL) return;
  hist = bsp->hist;
  if (hist == NULL) return;
  if (hist->assembly != NULL) {
    LookupAlignments (hist->assembly, userdata);
  }
  for (sip = hist->replace_ids; sip != NULL; sip = sip->next) {
    LookupSegments (NULL, sip, userdata);
  }
  for (sip = hist->replaced_by_ids; sip != NULL; sip = sip->next) {
    LookupSegments (NULL, sip, userdata);
  }
}

NLM_EXTERN Int4 LIBCALLBACK GiRevHistLookupFarSeqIDs (
  SeqEntryPtr sep,
  Boolean components,
  Boolean locations,
  Boolean products,
  Boolean alignments,
  Boolean history
)

{
  FarIdData    fid;
  SeqEntryPtr  oldsep;

  if (sep == NULL) return 0;
  MemSet ((Pointer) &fid, 0, sizeof (FarIdData));

  oldsep = SeqEntrySetScope (sep);

  if (components) {
    VisitBioseqsInSep (sep, (Pointer) &fid, LookupBioseqs);
  }
  if (locations) {
    VisitFeaturesInSep (sep, (Pointer) &fid, LookupLocations);
  }
  if (products) {
    VisitFeaturesInSep (sep, (Pointer) &fid, LookupProducts);
  }
  if (alignments) {
    VisitAlignmentsInSep (sep, (Pointer) &fid, LookupAlignments);
  }
  if (history) {
    VisitBioseqsInSep (sep, (Pointer) &fid, LookupHistory);
  }

  if (fid.num > 0) {
    fid.total += GiRevHistPreLoadSeqIdGiCache (fid.num, fid.uids);
  }

  SeqEntrySetScope (oldsep);
  return fid.total;
}

NLM_EXTERN SeqIdPtr LIBCALLBACK GiRevHistLookupSeqIdSet (
  Int4 gi
)

{
  return GiSeqIdSetSynchronousQuery (gi);
}

/* PubMed fetch functions */

static PubmedEntryPtr LIBCALLBACK DoPubMedFetch (Int4 uid)

{
  return PubMedSynchronousQuery (uid);
}

NLM_EXTERN Boolean PubMedFetchEnable (
  void
)

{
  PubMedSetFetchFunc (DoPubMedFetch);
  return TRUE;
}

NLM_EXTERN void PubMedFetchDisable (
  void
)

{
  PubMedSetFetchFunc (NULL);
}

