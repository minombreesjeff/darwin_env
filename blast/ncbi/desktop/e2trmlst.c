/*   e2trmlst.c
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
* File Name:  e2trmlst.c
*
* Author:  Jonathan Kans, Greg Schuler, Jonathan Epstein, Tim Ford
*
* Version Creation Date:   10/30/01
*
* $Revision: 6.38 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
*
* ==========================================================================
*/

#include <vibrant.h>
#include <document.h>
#include <asn.h>
#include <ent2api.h>
#include <urlquery.h>
#include <dlogutil.h>
#include <medview.h>
#include <bspview.h>
#include <accutils.h>

#include <entrez2.h>

/*-------------------*/
/* Defined constants */
/*-------------------*/

#define MAX_TAXONOMY_TREE_DEPTH  1024
#define TL_TERM_DELIMITER        ':'
#define E2_STR_BUFF_SIZE         256
#define E2_EXPLODE               FALSE
#define E2_DO_NOT_EXPLODE        TRUE
#define E2_TRANSLATE             FALSE
#define E2_DO_NOT_TRANSLATE      TRUE
#define E2_TERM_COL              1
#define E2_COUNT_COL             2
#define E2_LIST_MODIFIED         TRUE
#define E2_LIST_NOT_MODIFIED     FALSE

#define MAX_MODES         11

#define STATE_OFF         0
#define STATE_ON          1

#define E2_RANGE_FROM     1
#define E2_RANGE_TO       2

#define GROUP_NONE        0
#define GROUP_SINGLE      1
#define GROUP_FIRST       2
#define GROUP_MIDDLE      3
#define GROUP_LAST        4

#define TERMLIST_OPERATOR             2
#define TERMLIST_TERM                 3

#define ADV_QUERY_INVALID_STATE       0
#define ADV_QUERY_EVALUATE_STATE      1
#define ADV_QUERY_RETRIEVE_STATE      2

#define AVAIL_WINDOW_ROWS             7
#define ADV_TEXT_MAX_LENGTH           8192
#define ADV_TEXT_WIDTH                20
#define ADV_TEXT_HEIGHT               7

#define LEXCHAR_LPAREN                1
#define LEXCHAR_RPAREN                2
#define LEXCHAR_LBRACKET              3
#define LEXCHAR_RBRACKET              4
#define LEXCHAR_QUOTE                 5
#define LEXCHAR_AND                   6
#define LEXCHAR_OR                    7
#define LEXCHAR_NOT                   8
#define LEXCHAR_COMMA                 9
#define LEXCHAR_ATSIGN               10
#define LEXCHAR_BACKSLASH            11
#define LEXCHAR_WHITESPACE           12
#define LEXCHAR_SEMICOLON            13
#define LEXCHAR_COLON                14
#define LEXCHAR_EOL                  15
#define LEXCHAR_NULL                 16
#define LEXCHAR_OTHER                17

#define LEXSTATE_IDLE                 0
#define LEXSTATE_BACKSLASHED          1
#define LEXSTATE_INQUOTE              2
#define LEXSTATE_INQUOTE_AFTERBSLASH  3
#define LEXSTATE_INSTRING             4
#define LEXSTATE_ERROR                5

#define LEXTOK_LPAREN                 1
#define LEXTOK_RPAREN                 2
#define LEXTOK_LBRACKET               3
#define LEXTOK_RBRACKET               4
#define LEXTOK_AND                    5
#define LEXTOK_OR                     6
#define LEXTOK_NOT                    7
#define LEXTOK_COMMA                  8
#define LEXTOK_ATSIGN                 9
#define LEXTOK_STRING                10
#define LEXTOK_RANGE                 11

#define ERR_CD_LEX                   17

/*------------------------------*/
/* Defines for enumerated lists */
/*------------------------------*/

typedef enum {
  SELECTION_MODE = 1,
  AUTOMATIC_MODE,
  WILD_CARD_MODE,
  MESH_TREE_MODE,
  TAXONOMY_MODE,
  RANGE_MODE,
  LOOKUP_ACCN_MODE,
  VIEW_ACCN_MODE,
  LOOKUP_UID_MODE,
  VIEW_UID_MODE,
  TRANSLATE_MODE
} ModeChoice;

static ENUM_ALIST(mult_alist)
  {"Multiple",            AUTOMATIC_MODE},
  {"Selection",           SELECTION_MODE},
  {"Wild Card",           WILD_CARD_MODE},
END_ENUM_ALIST

static ENUM_ALIST(auto_alist)
  {"Automatic",           TRANSLATE_MODE},
  {"Multiple",            AUTOMATIC_MODE},
  {"Selection",           SELECTION_MODE},
  {"Wild Card",           WILD_CARD_MODE},
END_ENUM_ALIST

static ENUM_ALIST(accn_alist)
  {"Lookup",              LOOKUP_ACCN_MODE},
  {"Range",               RANGE_MODE},
  {"Selection",           SELECTION_MODE},
  {"Wild Card",           WILD_CARD_MODE},
  {"View",                VIEW_ACCN_MODE},
END_ENUM_ALIST

static ENUM_ALIST(mesh_alist)
  {"MeSH Tree",           MESH_TREE_MODE},
  {"Selection",           SELECTION_MODE},
  {"Wild Card",           WILD_CARD_MODE},
END_ENUM_ALIST

static ENUM_ALIST(tax_alist)
  {"Selection",           SELECTION_MODE},
  {"Taxonomy",            TAXONOMY_MODE},
  {"Wild Card",           WILD_CARD_MODE},
END_ENUM_ALIST

static ENUM_ALIST(default_alist)
  {"Range",               RANGE_MODE},
  {"Selection",           SELECTION_MODE},
  {"Wild Card",           WILD_CARD_MODE},
END_ENUM_ALIST

static ENUM_ALIST(range_alist)
  {"Range",               RANGE_MODE},
  {"Selection",           SELECTION_MODE},
END_ENUM_ALIST

static ENUM_ALIST(trunc_alist)
  {"Selection",           SELECTION_MODE},
  {"Wild Card",           WILD_CARD_MODE},
END_ENUM_ALIST

static ENUM_ALIST(uid_alist)
  {"Lookup",              LOOKUP_UID_MODE},
  {"View",                VIEW_UID_MODE},
END_ENUM_ALIST

static EnumFieldAssocPtr mode_alists [] = {
  mult_alist,
  auto_alist,
  accn_alist,
  mesh_alist,
  tax_alist,
  default_alist,
  range_alist,
  trunc_alist,
  uid_alist,
  NULL
};

typedef enum {
  POPUP_MULT = 0,
  POPUP_AUTO,
  POPUP_ACCN,
  POPUP_MESH,
  POPUP_TAX,
  POPUP_DEFAULT,
  POPUP_RANGE,
  POPUP_TRUNC,
  POPUP_UID
} ModeIndex;

/*-----------------------*/
/* Structure definitions */
/*-----------------------*/

typedef struct trmstatedata {
  CharPtr                   term;
  CharPtr                   field;
  Int4                      count;
  Int2                      db;
  Int2                      fld;
  Int2                      group;
  Int2                      above;
  Int2                      below;
  Int2                      state;
  Int4Ptr                   uids;
  CharPtr                   key;
  struct trmstatedata PNTR  prev;
  struct trmstatedata PNTR  next;
} StateData, PNTR StateDataPtr;

typedef struct {
  FORM_MESSAGE_BLOCK
  E2RetrieveDocsProc  retrieveDocsProc;
  E2RetrieveUidProc   retrieveUidProc;
  GrouP               stdQueryGroup;
  GrouP               advQueryGroup;
  ButtoN              advEvaluateButton;
  ButtoN              advRetrieveButton;
  Int2                advQueryState;
  ButtoN              advResetButton;
  TexT                advQueryText;
  Int2                advQueryLexPos;
  CharPtr             advQueryLexStr;
  Int2                advQueryLexState;
  ValNodePtr          advQueryNextToken;
  ValNode             advQueryNextRealNode;
  Boolean             advQueryNewGroup;
  PopuP PNTR          fieldsPopup;
  PopuP               modesPopup [MAX_MODES];
  GrouP               termGroup;
  GrouP               rangeGroup;
  IteM                advancedQueryItem;
  IteM                explodeItem;
  ModeChoice          currMode;
  Int2                currField;
  Int2                currDb;
  DoC                 availDoc;
  Int2                availItem;
  Int2                availRow;
  Int2                availClickItem;
  Int2                availClickRow;
  Int4                availNumTerms;
  Int4                availPageSize;
  Int2                availNumPages;
  Int2                availCurrentPage;
  Boolean             textChanged;
  Int2                okayToAccept;
  TexT                termText;
  TexT                fromText;
  TexT                toText;
  Boolean             isValidFrom;
  Boolean             isValidTo;
  Boolean             isFromTextChanged;
  Boolean             isToTextChanged;
  Int2                currRangeField;
  ButtoN              acceptButton;
  PopuP               taxLineagePopup;
  ValNodePtr          taxStrings;
  PopuP               databasePopup;
  ButtoN              retrieveButton;
  ButtoN              resetButton;
  Boolean             wasDoubleClick;
  DoC                 chosenDoc;
  Int2                chosenItem;
  Int2                chosenClickItem;
  Int2                chosenClickRow;
  Int2                chosenClickCol;
  Boolean             chosenInAboveBox;
  Boolean             chosenInBelowBox;
  Int2                chosenNumLines;
  RecT                trackRect;
  PoinT               trackPt;
  StateDataPtr        termList;
} FormInfo, PNTR FormInfoPtr;

/*----------------------------*/
/* File-wide static variables */
/*----------------------------*/

static ChoicE         s_showASNItem;
static Entrez2InfoPtr s_masterE2ip = NULL;

static ParData  availParFmt = { FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0 };
static ColData  availColFmt [] = {
  {0, 5, 70, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE},  /* term  */
  {0, 5, 10, 0, NULL, 'r', FALSE, TRUE, FALSE, FALSE, FALSE},   /* count */
  {0, 5, 10, 0, NULL, 'r', FALSE, FALSE, FALSE, FALSE, TRUE}    /* leaf  */
};

static ParData  chosenParFmt = { FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0 };
static ColData  chosenColFmt [] = {
  {0, 23, 70, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE}, /* term  */
  {0, 5, 10, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE},  /* field */
  {0, 5, 10, 0, NULL, 'r', FALSE, TRUE, FALSE, FALSE, TRUE}     /* count */
};

/*---------------------*/
/* Function prototpyes */
/*---------------------*/

static Boolean RemoveTermFromList (
  FormInfoPtr pFormInfo,
  StateDataPtr sdp,
  Boolean goingDown,
  Boolean lastDraggedDown
);
static Boolean Query_TranslateAndAddBoolTerm (
  ForM f,
  Int2 currDb,
  Int2 currFld,
  CharPtr strs,
  Int2 state,
  Int4 num
);

static Boolean LoadAvailList (FormInfoPtr pFormInfo, CharPtr str);

/*==================================================================*/
/*                                                                  */
/*  ShowASN () -  Returns the current setting of the s_showASN      */
/*               static variable.                                   */
/*                                                                  */
/*==================================================================*/

NLM_EXTERN Boolean ShowASN (void)

{
  Int2  val;

  /*
  return GetStatus (s_showASNItem);
  */
  val = GetValue (s_showASNItem);
  if (val >= 11) return TRUE;
  if (val < 2) return FALSE;
  SetValue (s_showASNItem, val - 1);
  if (val < 2) return FALSE;
  return TRUE;
}

static void LogOrLaunch (CharPtr path, CharPtr title)

{
  Char    buf [256];
  size_t  ct;
  FILE    *ifp;
  FILE    *ofp;
  Int2    val;

  val = GetValue (s_showASNItem);
  if (val == 12) {
    ifp = FileOpen (path, "r");
    ofp = FileOpen ("entrez2.log", "a");
    if (ifp != NULL && ofp != NULL) {
      while ((ct = FileRead (buf, 1, sizeof (buf), ifp)) > 0) {
        FileWrite (buf, 1, ct, ofp);
      }
    }
    fprintf (ofp, "\n");
    FileClose (ofp);
    FileClose (ifp);
  } else {
    LaunchGeneralTextViewer (path, title);
  }
}

/*==================================================================*/
/*                                                                  */
/*  DisplayEntrezRequest () - Displays an Entrez2 request in a      */
/*                           pop-up window.                         */
/*                                                                  */
/*==================================================================*/

NLM_EXTERN void DisplayEntrezRequest (Entrez2RequestPtr e2rq)

{
  AsnIoPtr  aip;
  Char      path [PATH_MAX];

  if (e2rq == NULL) return;
  TmpNam (path);
  aip = AsnIoOpen (path, "w");
  if (aip == NULL) return;
  Entrez2RequestAsnWrite (e2rq, aip, NULL);
  AsnIoClose (aip);
  LogOrLaunch (path, "Entrez2RequestAsnWrite");
  FileRemove (path);
}

/*==================================================================*/
/*                                                                  */
/*  DisplayEntrezReply () - Displays an Entrez2 reply in a pop-up   */
/*                         window.                                  */
/*                                                                  */
/*==================================================================*/

NLM_EXTERN void DisplayEntrezReply (Entrez2ReplyPtr e2ry)

{
  AsnIoPtr  aip;
  Char      path [PATH_MAX];

  if (e2ry == NULL) return;
  TmpNam (path);
  aip = AsnIoOpen (path, "w");
  if (aip == NULL) return;
  Entrez2ReplyAsnWrite (e2ry, aip, NULL);
  AsnIoClose (aip);
  LogOrLaunch (path, "Entrez2ReplyAsnWrite");
  FileRemove (path);
}

/* text version of entrez2 query for debugging purposes */

static Entrez2ReplyPtr EntrezTextWaitForReply (
  CONN conn
)

{
  AsnIoConnPtr     aicp;
  AsnIoPtr         aip;
  Char             buffer [1025];
  time_t           currtime, starttime;
  Entrez2ReplyPtr  e2ry = NULL;
  Boolean          first;
  FILE *           fp;
  Int2             max = 0;
  size_t           n_read;
  Boolean          special_read = FALSE;
  EIO_Status       status;
  STimeout         timeout;
  Char             tmp [PATH_MAX];

  if (conn == NULL) return NULL;

  timeout.sec = 100;
  timeout.usec = 0;

  starttime = GetSecs ();
  while ((status = CONN_Wait (conn, eIO_Read, &timeout)) == eIO_Timeout && max < 300) {
    currtime = GetSecs ();
    max = currtime - starttime;
  }
  if (status == eIO_Success) {
#ifdef OS_UNIX
    if (getenv ("ENTREZ2_RECORD_QUERY") != 0) {
      special_read = TRUE;
    }
#endif
    if (special_read) {
      TmpNam (tmp);
#ifdef OS_UNIX
printf ("TmpNam '%s'\n", tmp);
#endif
      fp = FileOpen (tmp, "w");
      if (fp != NULL) {

        first = TRUE;
        do {
          status = CONN_Read (conn, buffer, sizeof (buffer) -1, &n_read,
                              first ? eIO_ReadPersist : eIO_ReadPlain);
          buffer [n_read] = '\0';
          if (first) {
            if (StringStr (buffer, "Entrez2-reply") == NULL) {
              ErrPostEx (SEV_ERROR, 0, 0, "EntrezReadReply failed on '%s'", buffer);
              FileClose (fp);
              /*
              FileRemove (tmp);
              */
              return NULL;
            }
            first = FALSE;
          }
          fprintf (fp, "%s", buffer);
        } while (status == eIO_Success);
        FileClose (fp);

        aip = AsnIoOpen (tmp, "r");
        e2ry = Entrez2ReplyAsnRead (aip, NULL);
        AsnIoClose (aip);
        /*
        FileRemove (tmp);
        */
      }
    } else {
      aicp = QUERY_AsnIoConnOpen ("r", conn);
      e2ry = Entrez2ReplyAsnRead (aicp->aip, NULL);
      QUERY_AsnIoConnClose (aicp);
    }
  }
  CONN_Close (conn);

  return e2ry;
}

static Entrez2ReplyPtr EntrezSpecialSynchronousQuery (
  Entrez2RequestPtr e2rq,
  Boolean textmode
)

{
  AsnIoConnPtr     aicp;
  CONN             conn;
  Entrez2ReplyPtr  e2ry;
  CharPtr          tempcookie = NULL;
  CharPtr          host_machine = NULL;
  Uint2            host_port = 0;
  CharPtr          host_path = NULL;
  CharPtr          env_machine = NULL;
  CharPtr          env_path = NULL;
  CharPtr          env_port = NULL;
  CharPtr          env_service = NULL;
  int              val = 0;

  if (e2rq == NULL) return NULL;

#ifdef OS_UNIX
  env_machine = (CharPtr) getenv ("NCBI_ENTREZ2_HOST_MACHINE");
  if (! StringHasNoText (env_machine)) {
    host_machine = env_machine;
  }
#endif
  if (StringHasNoText (host_machine)) {
    host_machine = "www.ncbi.nlm.nih.gov";
  }

#ifdef OS_UNIX
  env_port = (CharPtr) getenv ("NCBI_ENTREZ2_HOST_PORT");
  if (! StringHasNoText (env_port)) {
    if (sscanf (env_port, "%d", &val) == 1) {
      host_port = (Uint2) val;
    }
  }
#endif
  if (host_port == 0) {
    host_port = 80;
  }

#ifdef OS_UNIX
  env_path = (CharPtr) getenv ("NCBI_ENTREZ2_HOST_PATH");
  if (! StringHasNoText (env_path)) {
    host_path = env_path;
  }
#endif
  if (StringHasNoText (host_path)) {
    host_path = "/entrez/eutils/entrez2server.fcgi";
  }

  if (textmode) {
    conn = QUERY_OpenUrlQuery (host_machine, host_port, host_path, NULL,
                               "Entrez2Text", 30, eMIME_T_NcbiData,
                               eMIME_AsnText, eENCOD_None, 0);

    aicp = QUERY_AsnIoConnOpen ("w", conn);
  } else {
    conn = QUERY_OpenUrlQuery (host_machine, host_port, host_path, NULL,
                               "Entrez2Text", 30, eMIME_T_NcbiData,
                               eMIME_AsnBinary, eENCOD_None, 0);

    aicp = QUERY_AsnIoConnOpen ("wb", conn);
  }

  tempcookie = e2rq->cookie;

  Entrez2RequestAsnWrite (e2rq, aicp->aip, NULL);

  e2rq->cookie = tempcookie;

  AsnIoFlush (aicp->aip);
  QUERY_AsnIoConnClose (aicp);

  QUERY_SendQuery (conn);

  if (textmode) {
    e2ry = EntrezTextWaitForReply (conn);
  } else {
    e2ry = EntrezWaitForReply (conn);
  }

  return e2ry;
}

extern Entrez2ReplyPtr SpecialEntrezSynchronousQuery (
  Entrez2RequestPtr e2rq
);

extern Entrez2ReplyPtr SpecialEntrezSynchronousQuery (
  Entrez2RequestPtr e2rq
)

{
#ifdef OS_UNIX
  if (getenv ("ENTREZ2_TEXT_QUERY") != 0) {
    return EntrezSpecialSynchronousQuery (e2rq, TRUE);
  }
  if (getenv ("ENTREZ2_BINARY_QUERY") != 0) {
    return EntrezSpecialSynchronousQuery (e2rq, FALSE);
  }
#endif
  return EntrezSynchronousQuery (e2rq);
}

/*==================================================================*/
/*                                                                  */
/*  DBGetInfo () - Given a database ID, return the DB pointer.      */
/*                                                                  */
/*==================================================================*/

static Entrez2DbInfoPtr DBGetInfo (Int2 dbId)

{
  Int2              count;
  Entrez2InfoPtr    e2ip;
  Entrez2DbInfoPtr  e2db;

  /*------------------*/
  /* Check parameters */
  /*------------------*/

  if (dbId < 0) return NULL;

  /*----------------------------------*/
  /* Get information on the databases */
  /*----------------------------------*/

  e2ip = Query_GetInfo ();

  /*-----------------------*/
  /* Find the requested DB */
  /*-----------------------*/

  for (e2db = e2ip->db_info, count = 0; e2db != NULL; e2db = e2db->next, count++) {
    if (count == dbId) return e2db;
  }

  return NULL;
}

/*==================================================================*/
/*                                                                  */
/*  DBGetNameFromID () - Given a DB ID, returns its name.           */
/*                                                                  */
/*==================================================================*/

NLM_EXTERN CharPtr DBGetNameFromID (Int2 dbId)

{
  Int2              count;
  Entrez2InfoPtr    e2ip;
  Entrez2DbInfoPtr  e2db;

  /*-----------------*/
  /* Check parameter */
  /*-----------------*/

  if (dbId < 0) return NULL;

  /*----------------------------------*/
  /* Get information on the databases */
  /*----------------------------------*/

  e2ip = Query_GetInfo ();

  /*--------------------------*/
  /* Find to the requested DB */
  /*--------------------------*/

  for (e2db = e2ip->db_info, count = 0; e2db != NULL; e2db = e2db->next, count++) {
    if (count == dbId) return e2db->db_name;
  }

  return NULL;
}

/*==================================================================*/
/*                                                                  */
/*  DBGetIDFromName () - Given a DB name, returns its unique ID.    */
/*                                                                  */
/*==================================================================*/

NLM_EXTERN Int2 DBGetIDFromName (CharPtr dbName)

{
  Int2              count;
  Entrez2InfoPtr    e2ip;
  Entrez2DbInfoPtr  e2db;

  /*-----------------*/
  /* Check parameter */
  /*-----------------*/

  if (dbName == NULL) return -1;

  /*----------------------------------*/
  /* Get information on the databases */
  /*----------------------------------*/

  e2ip = Query_GetInfo ();

  /*---------------------------*/
  /* Look for the requested DB */
  /*---------------------------*/

  for (e2db = e2ip->db_info, count = 0; e2db != NULL; e2db = e2db->next, count++) {
    if (StrICmp (e2db->db_name, dbName) == 0) return count;
  }

  return -1;
}

/*==================================================================*/
/*                                                                  */
/*  FieldGetInfo () - Given a database ID and a field ID, return    */
/*                    the field name.                               */
/*                                                                  */
/*==================================================================*/

static Entrez2FieldInfoPtr FieldGetInfo (Int2 dbId, Int2 fieldId)

{
  Int2                 count;
  Entrez2DbInfoPtr     e2db;
  Entrez2FieldInfoPtr  e2fd;

  /*------------------*/
  /* Check parameters */
  /*------------------*/

  if (dbId < 0 || fieldId < 0) return NULL;

  /*----------------------------------*/
  /* Find the requested DB            */
  /*----------------------------------*/

  e2db = DBGetInfo (dbId);
  if (e2db == NULL) return NULL;

  /*----------------*/
  /* Get field info */
  /*----------------*/

  for (e2fd = e2db->fields, count = 0; e2fd != NULL; e2fd = e2fd->next, count++) {
    if (count == fieldId) return e2fd;
  }

  return NULL;
}

/*==================================================================*/
/*                                                                  */
/*  IsValidFieldName () - Given a database ID and a field name,     */
/*                        check to see if it is as valid name.      */
/*                                                                  */
/*==================================================================*/

static Boolean IsValidFieldName (Int2 dbId, CharPtr fieldName)

{
  Entrez2DbInfoPtr     e2db;
  Entrez2FieldInfoPtr  e2fd;

  /*------------------*/
  /* Check parameters */
  /*------------------*/

  if (dbId < 0 || fieldName == NULL) return FALSE;

  /*-----------------------*/
  /* Find the requested DB */
  /*-----------------------*/

  e2db = DBGetInfo (dbId);
  if (e2db == NULL) return FALSE;

  /*----------------*/
  /* Get field info */
  /*----------------*/

  for (e2fd = e2db->fields; e2fd != NULL; e2fd = e2fd->next) {
    if (StrICmp (e2fd->field_name, fieldName) == 0)
      return TRUE;
  }

  return FALSE;
}

/*===================================================================*/
/*                                                                   */
/*  FieldGetNameFromMenuName () - Given a field menu name and a      */
/*                               database ID, return the field name. */
/*                                                                   */
/*===================================================================*/

static CharPtr FieldGetNameFromMenuName (Int2 dbId, CharPtr menuName)

{
  Entrez2DbInfoPtr     e2db;
  Entrez2FieldInfoPtr  e2fd;

  /*------------------*/
  /* Check parameters */
  /*------------------*/

  if (menuName == NULL || dbId < 0) return NULL;

  /*-----------------------*/
  /* Find the requested DB */
  /*-----------------------*/

  e2db = DBGetInfo (dbId);
  if (e2db == NULL) return NULL;

  /*--------------------------*/
  /* Find the requested field */
  /*--------------------------*/

  for (e2fd = e2db->fields; e2fd != NULL; e2fd = e2fd->next) {
    if (StrICmp (e2fd->field_menu, menuName) == 0) return e2fd->field_name;
  }

  return NULL;
}

/*==================================================================*/
/*                                                                  */
/*  FieldGetIDFromName () - Given a field name and a database ID,   */
/*                         return the field ID.                     */
/*                                                                  */
/*==================================================================*/

static Int2 FieldGetIDFromName (Int2 dbId, CharPtr fieldName)

{
  Int2                 count;
  Entrez2DbInfoPtr     e2db;
  Entrez2FieldInfoPtr  e2fd;

  /*------------------*/
  /* Check parameters */
  /*------------------*/

  if (fieldName == NULL || dbId < 0) return -1;

  /*-----------------------*/
  /* Find the requested DB */
  /*-----------------------*/

  e2db = DBGetInfo (dbId);
  if (e2db == NULL) return -1;

  /*--------------------------*/
  /* Find the requested field */
  /*--------------------------*/

  for (e2fd = e2db->fields, count = 0; e2fd != NULL; e2fd = e2fd->next, count++) {
    if (StrICmp (e2fd->field_name, fieldName) == 0) return count;
  }

  return -1;
}

/*==================================================================*/
/*                                                                  */
/*  FieldGetInfoFromName () - This is desigend to get field info    */
/*                           given only a field name. It is useful  */
/*                           for a getting information on a field   */
/*                           in a sorted alist of fields.           */
/*                                                                  */
/*==================================================================*/

static Entrez2FieldInfoPtr FieldGetInfoFromName (CharPtr fieldName)

{
  Int2                 count;
  Entrez2InfoPtr       e2ip;
  Entrez2DbInfoPtr     e2db;
  Entrez2FieldInfoPtr  e2fd;

  /*------------------*/
  /* Check parameters */
  /*------------------*/

  if (fieldName == NULL) return NULL;

  /*----------------------------------*/
  /* Get information on the databases */
  /*----------------------------------*/

  e2ip = Query_GetInfo ();

  /*-------------------------------*/
  /* Find the requested field info */
  /*-------------------------------*/

  for (e2db = e2ip->db_info, count = 0; e2db != NULL; e2db = e2db->next, count++) {
    for (e2fd = e2db->fields; e2fd != NULL; e2fd = e2fd->next) {
      if (StrICmp (e2fd->field_name, fieldName) == 0) return e2fd;
    }
  }

  return NULL;
}

/*==================================================================*/
/*                                                                  */
/* FieldGetModePopup () -                                           */
/*                                                                  */
/*==================================================================*/

static Int2 FieldGetModePopup (Int2 dbId, Int2 fieldId, EnumFieldAssocPtr fieldAList, FormInfoPtr pFormInfo)

{
  Entrez2DbInfoPtr     dbInfo;
  Entrez2FieldInfoPtr  fieldInfo;
  Boolean              is_rangable;
  Boolean              is_truncatable;
  ModeIndex            modeId = POPUP_DEFAULT;

  /*-----------------------------------------*/
  /* Find the requested field, and determine */
  /* its corresponding mode list popup.      */
  /*-----------------------------------------*/

  dbInfo = DBGetInfo (dbId);
  if (dbInfo == NULL) return -1;

  fieldInfo = FieldGetInfo (dbId, fieldId);
  if (fieldInfo == NULL) return -1;

  /* is_rangable = fieldInfo->is_rangable; */
  is_rangable = (Boolean) (fieldInfo->is_date || fieldInfo->is_numerical);
  /* is_truncatable = fieldInfo->is_truncatable; */
  is_truncatable = (Boolean) (! is_rangable);

  if (StringICmp (dbInfo->db_name, "PubMed") == 0 &&
      StringICmp (fieldInfo->field_name, "ALL") == 0) {
    modeId = POPUP_AUTO;
  } else if (StringICmp (fieldInfo->field_name, "ALL") == 0) {
    modeId = POPUP_MULT;
  } else if ((StringICmp (fieldInfo->field_name, "TITL") == 0) ||
             (StringICmp (fieldInfo->field_name, "WORD") == 0) ||
             (StringICmp (fieldInfo->field_name, "TIAB") == 0)) {
    modeId = POPUP_MULT;
  } else if (StringICmp (fieldInfo->field_name, "ACCN") == 0 ||
             StringICmp (fieldInfo->field_name, "PACC") == 0) {
    modeId = POPUP_ACCN;
  } else if (StringICmp (fieldInfo->field_name, "ORGN") == 0) {
    modeId = POPUP_TAX;
  } else if (StringICmp (fieldInfo->field_name, "MESH") == 0 ||
             StringICmp (fieldInfo->field_name, "MAJR") == 0) {
    modeId = POPUP_MESH;
  } else if (StringICmp (fieldInfo->field_name, "UID") == 0) {
    modeId = POPUP_UID;
  } else if (is_rangable && is_truncatable) {
    modeId = POPUP_DEFAULT;
  } else if (is_rangable) {
    modeId = POPUP_RANGE;
  } else if (is_truncatable) {
    modeId = POPUP_TRUNC;
  } else {
    modeId = POPUP_DEFAULT;
  }

  pFormInfo->currField = FieldGetIDFromName (dbId, fieldInfo->field_name);
  return modeId;
}

/*==================================================================*/
/*                                                                  */
/*  CreateDatabaseAlist () - Returns alist of database names        */
/*                                                                  */
/*==================================================================*/

EnumFieldAssocPtr CreateDatabaseAlist (Entrez2InfoPtr e2ip)

{
  EnumFieldAssocPtr  alist;
  Uint1              choice;
  Entrez2DbInfoPtr   e2db;
  ValNodePtr         head = NULL;

  if (e2ip == NULL || e2ip->db_count < 1) return NULL;

  for (e2db = e2ip->db_info; e2db != NULL; e2db = e2db->next) {
    choice = (Uint1) DBGetIDFromName (e2db->db_name);
    ValNodeCopyStr (&head, choice, e2db->db_menu);
  }

  alist = MakeEnumFieldAlistFromValNodeList (head);
  ValNodeFreeData (head);

  return alist;
}

/*==================================================================*/
/*                                                                  */
/*  CreateFieldAlist () - Returns sorted alist of field names for   */
/*                       the given database.                        */
/*                                                                  */
/*==================================================================*/

EnumFieldAssocPtr CreateFieldAlist (Entrez2DbInfoPtr e2db)

{
  EnumFieldAssocPtr    alist;
  Uint1                choice;
  Entrez2FieldInfoPtr  fieldInfo;
  Int2                 dbId;
  ValNodePtr           head = NULL;

  if (e2db == NULL || e2db->field_count < 1) return NULL;

  dbId = DBGetIDFromName (e2db->db_name);
  for (fieldInfo = e2db->fields; fieldInfo != NULL; fieldInfo = fieldInfo->next) {
    choice = (Uint1) FieldGetIDFromName (dbId, fieldInfo->field_name);
    ValNodeCopyStr (&head, choice, fieldInfo->field_menu);
  }

  head = ValNodeSort (head, SortVnpByString);

  alist = MakeEnumFieldAlistFromValNodeList (head);
  ValNodeFreeData (head);

  return alist;
}

/*==================================================================*/
/*                                                                  */
/*  CreateAllFieldsAlist () - Returns sorted uniqued alist of all   */
/*                           field names.                           */
/*                                                                  */
/*==================================================================*/

static EnumFieldAssocPtr CreateAllFieldsAlist (Entrez2InfoPtr e2ip)

{
  EnumFieldAssocPtr    alist;
  Uint1                choice;
  Entrez2DbInfoPtr     e2db;
  Entrez2FieldInfoPtr  fieldInfo;
  Int2                 dbId;
  ValNodePtr           head = NULL;

  for (e2db = e2ip->db_info; e2db != NULL; e2db = e2db->next) {
    dbId = DBGetIDFromName (e2db->db_name);
    for (fieldInfo = e2db->fields; fieldInfo != NULL; fieldInfo = fieldInfo->next) {
      choice = (Uint1) FieldGetIDFromName (dbId, fieldInfo->field_name);
      ValNodeCopyStr (&head, choice, fieldInfo->field_name);
    }
  }

  head = ValNodeSort (head, SortVnpByString);
  head = UniqueValNode (head);

  alist = MakeEnumFieldAlistFromValNodeList (head);
  ValNodeFreeData (head);

  return alist;
}

/*==================================================================*/
/*                                                                  */
/*  GetUidFromAccn () -                                             */
/*                                                                  */
/*==================================================================*/

static Uint4 GetUidFromAccn (CharPtr dbName, CharPtr accn)

{
  Entrez2BooleanReplyPtr  e2br;
  Entrez2IdListPtr        e2id;
  Entrez2RequestPtr       e2rq;
  Entrez2ReplyPtr         e2ry;
  Char                    ch;
  CharPtr                 ptr;
  Char                    str [61];
  Uint4                   uid = 0;

  if (StringHasNoText (dbName) || StringHasNoText (accn)) return 0;

  StringNCpy_0 (str, accn, sizeof (str));
  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    if (ch == '|' || ch == '.') {
      *ptr = ' ';
    }
    ptr++;
    ch = *ptr;
  }
  TrimSpacesAroundString (str);
  if (StringStr (str, "[ACCN]") == NULL) {
    StringCat (str, "[ACCN]");
  }
  e2rq = EntrezCreateBooleanRequest (TRUE, FALSE, dbName, str, 0, 0, NULL, 1, 0);

  if (e2rq == NULL) return 0;
  e2ry = SpecialEntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (e2ry == NULL) return 0;
  e2br = EntrezExtractBooleanReply (e2ry);
  if (e2br == NULL) return 0;

  if (e2br->count > 0) {
    e2id = e2br->uids;
    if (e2id != NULL && e2id->num > 0 && e2id->uids != NULL) {
      BSSeek (e2id->uids, 0, SEEK_SET);
      uid = Nlm_BSGetUint4 (e2id->uids);
    }
  }

  Entrez2BooleanReplyFree (e2br);

  return uid;
}

/*==================================================================*/
/*                                                                  */
/*  ProcessDirectLookup () -                                        */
/*                                                                  */
/*==================================================================*/

static void ProcessDirectLookup (FormInfoPtr pFormInfo, CharPtr str)

{
  CharPtr  dbName;
  Int4     uid;

  uid = 0;

  /*----------------------------------------*/
  /* Convert the given string to a UID and */
  /* check to see if it is a valid one.    */
  /*---------------------------------------*/

  if ((pFormInfo->currMode == LOOKUP_ACCN_MODE) || (pFormInfo->currMode == VIEW_ACCN_MODE)) {
    if (StringLen (str) > 0) {
      dbName = DBGetNameFromID (pFormInfo->currDb);
      uid = GetUidFromAccn (dbName, str);
    }
  } else if ((pFormInfo->currMode == LOOKUP_UID_MODE) || (pFormInfo->currMode == VIEW_UID_MODE)) {
    if (! StrToLong (str, &uid)) {
      Message (MSG_OK, "You must enter an integer");
      return;
    }
  } else
    return;

  if (uid <= 0) {
    if (pFormInfo->currMode == LOOKUP_ACCN_MODE || pFormInfo->currMode == VIEW_ACCN_MODE) {
      Message (MSG_OK, "This accession is not present in the database");
    } else {
      Message (MSG_OK, "This UID is not present in the database");
    }
    return;
  }

  /*-------------------------------------*/
  /* Lookup the record for the given UID */
  /* and display it in a DocSum window.  */
  /*-------------------------------------*/

  if (pFormInfo->currMode == LOOKUP_ACCN_MODE || pFormInfo->currMode == LOOKUP_UID_MODE) {
    dbName = DBGetNameFromID (pFormInfo->currDb);
    pFormInfo->retrieveUidProc (pFormInfo->form, uid, dbName);
  } else {
    LaunchRecViewer (pFormInfo->form, uid, 1, &uid, pFormInfo->currDb, 1);
  }
}

/*==================================================================*/
/*                                                                  */
/*  DoResetAvail () - Clears out the Term List object.              */
/*                                                                  */
/*==================================================================*/

static void DoResetAvail (FormInfoPtr pFormInfo, Boolean clearTerm)

{
  Int2  i;

  if (clearTerm) {
    SafeSetTitle (pFormInfo->termText, "");
    SafeSetTitle (pFormInfo->toText, "");
    SafeSetTitle (pFormInfo->fromText, "");
    SafeDisable (pFormInfo->acceptButton);
  } else if (pFormInfo->currMode == RANGE_MODE) {
    if (TextHasNoText (pFormInfo->fromText) && TextHasNoText (pFormInfo->toText))
      SafeDisable (pFormInfo->acceptButton);
    else
      SafeEnable (pFormInfo->acceptButton);
  } else {
    if (TextHasNoText (pFormInfo->termText))
      SafeDisable (pFormInfo->acceptButton);
    else
      SafeEnable (pFormInfo->acceptButton);
  }

  pFormInfo->availItem = 0;
  pFormInfo->availRow = 0;
  Reset (pFormInfo->availDoc);
  SetDocCache (pFormInfo->availDoc, NULL, NULL, NULL);

  for (i = 0; i < 7; i++)
    AppendText (pFormInfo->availDoc, " \n", &availParFmt, availColFmt, systemFont);

  InvalDocument (pFormInfo->availDoc);
  SafeHide (pFormInfo->taxLineagePopup);
  Reset (pFormInfo->taxLineagePopup);
  pFormInfo->taxStrings = ValNodeFreeData (pFormInfo->taxStrings);
  pFormInfo->availNumTerms = 0;
  pFormInfo->availPageSize = 0;
  pFormInfo->availNumPages = 0;
}

/*==================================================================*/
/*                                                                  */
/* FreeTerm () - Frees a given term from the term list.             */
/*                                                                  */
/*==================================================================*/

static void FreeTerm (StateDataPtr termPtr)

{
  MemFree (termPtr->term);
  MemFree (termPtr->field);
  MemFree (termPtr);
}

/*==================================================================*/
/*                                                                  */
/* FreeTermList () - Frees all the terms in the current query       */
/*                    term list.                                    */
/*                                                                  */
/*==================================================================*/

static StateDataPtr FreeTermList (FormInfoPtr pFormInfo)

{
  StateDataPtr  next;

  while (pFormInfo->termList != NULL) {
    next = pFormInfo->termList->next;
    FreeTerm (pFormInfo->termList);
    pFormInfo->termList = next;
  }

  return NULL;
}

/*==================================================================*/
/*                                                                  */
/*  ResetChosen () - Clears out the Query Refinement object.        */
/*                                                                  */
/*==================================================================*/

static void ResetChosen (FormInfoPtr pFormInfo)

{
  /*--------------*/
  /* Clear it out */
  /*--------------*/

  Reset (pFormInfo->chosenDoc);
  FreeTermList (pFormInfo);
  SetDocCache (pFormInfo->chosenDoc, NULL, NULL, NULL);

  InvalDocument (pFormInfo->chosenDoc);
  pFormInfo->chosenNumLines = 0;

  SafeSetTitle (pFormInfo->retrieveButton, "Retrieve 0 Documents");
  SafeDisable (pFormInfo->retrieveButton);
}

/*==================================================================*/
/*                                                                  */
/*  Reset_Callback () - Called when reset button is pushed. Clears  */
/*                     the Term List and Query Refinement windows.  */
/*                                                                  */
/*==================================================================*/

static void Reset_Callback (ButtoN b)

{
  FormInfoPtr  pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (b);
  DoResetAvail (pFormInfo, TRUE);
  ResetChosen (pFormInfo);
  Reset (pFormInfo->advQueryText);
  pFormInfo->isValidFrom = FALSE;
  pFormInfo->isValidTo = FALSE;
}

/*==================================================================*/
/*                                                                  */
/*  StateDataNew() - Creates a new Query term structure and adds    */
/*                     it to the query term list.                   */
/*                                                                  */
/*==================================================================*/

static StateDataPtr StateDataNew (FormInfoPtr pFormInfo)

{
  StateDataPtr  newNode;
  StateDataPtr  tempNode;

  /*-------------------*/
  /* Create a new node */
  /*-------------------*/

  newNode = (StateDataPtr) MemNew (sizeof (StateData));
  if (newNode == NULL) return NULL;

  /*-------------------------------*/
  /* Add it to the end of the list */
  /*-------------------------------*/

  tempNode = pFormInfo->termList;
  if (tempNode != NULL) {
    while (tempNode->next != NULL)
      tempNode = tempNode->next;
    tempNode->next = newNode;
    newNode->prev = tempNode;
  } else {                      /* Empty list -- this is first term */

    newNode->prev = NULL;
    newNode->next = NULL;
    pFormInfo->termList = newNode;
  }

  /*---------------------*/
  /* Return successfully */
  /*---------------------*/

  return newNode;
}

/*===================================================================*/
/*                                                                   */
/*  CreateTerm () - Creates a new boolean term using the given       */
/*                       data and adds it to the linked list of      */
/*                       terms.                                      */
/*                                                                   */
/*===================================================================*/

static StateDataPtr CreateTerm (
  ForM    f,
  Int2    currDb,
  Int2    currFld,
  CharPtr strs,
  Int2    state,
  Int4    num,
  Boolean allowDuplicates
)

{
  StateDataPtr         prev;
  StateDataPtr         sdp;
  Entrez2FieldInfoPtr  fieldInfo;
  FormInfoPtr          pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (f);

  /*----------------------------------*/
  /* Make sure we're don't try to add */
  /* the same string twice.           */
  /*----------------------------------*/

  if (! allowDuplicates) {
    for (sdp = pFormInfo->termList; sdp != NULL; sdp = sdp->next) {
      if (MeshStringICmp (sdp->term, strs) == 0 &&
          sdp->db == currDb && sdp->fld == currFld) return NULL;
    }
  }

  /*------------------------------*/
  /* Create a term for the string */
  /* on the end of the list.      */
  /*------------------------------*/

  sdp = StateDataNew (pFormInfo);
  if (pFormInfo->termList == NULL)
    pFormInfo->termList = sdp;

  if (sdp == NULL) return NULL;

  (pFormInfo->chosenNumLines)++;

  sdp->term = StringSave (strs);
  sdp->count = num;
  sdp->db = currDb;
  sdp->fld = currFld;
  sdp->uids = NULL;
  sdp->key = NULL;
  fieldInfo = FieldGetInfo (currDb, currFld);
  if (fieldInfo == NULL) return NULL;
  sdp->field = StringSave (fieldInfo->field_name);
  if (sdp->field == NULL)
    sdp->field = StringSave ("----");
  sdp->group = GROUP_SINGLE;
  sdp->above = ENTREZ_OP_NONE;

  /*-------------------------------------*/
  /* Add the term to the current Query's */
  /* linked list of terms.               */
  /*-------------------------------------*/

  prev = sdp->prev;
  if (prev != NULL) {
    sdp->above = ENTREZ_OP_AND;
    prev->below = ENTREZ_OP_AND;
  }
  sdp->below = ENTREZ_OP_NONE;
  sdp->state = state;

  /*------------------------------*/
  /* Return a ptr to the new term */
  /*------------------------------*/

  return sdp;
}

/*==================================================================*/
/*                                                                  */
/*  DisplayTerm () -                                                */
/*                                                                  */
/*==================================================================*/

static void DisplayTerm (FormInfoPtr pFormInfo, CharPtr term, CharPtr fieldName, Int4 total)

{
  RecT  r;
  BaR   sb;
  Char  strn [256];

  /*------------------------------------------*/
  /* Create a display string for the term and */
  /* add it to the 'chosen' window.           */
  /*------------------------------------------*/

  sprintf (strn, "%s\t [%s]\t%ld\n", term, fieldName, (long) total);
  AppendText (pFormInfo->chosenDoc, strn, &chosenParFmt, chosenColFmt, systemFont);

  /*---------------------------------*/
  /* Display our new query string in */
  /* Query Refinement window.        */
  /*---------------------------------*/

  InvalDocRows (pFormInfo->chosenDoc, pFormInfo->chosenNumLines, 1, 1);
  AdjustDocScroll (pFormInfo->chosenDoc);
  sb = GetSlateVScrollBar ((SlatE) pFormInfo->chosenDoc);
  ResetClip ();
  SetBarValue (sb, MAX (pFormInfo->chosenNumLines - 7, 0));
  ObjectRect (pFormInfo->chosenDoc, &r);
  InsetRect (&r, 4, 4);
  r.right = r.left + 16;
  InsetRect (&r, -1, -1);
  Select (pFormInfo->chosenDoc);
  InvalRect (&r);

  Update ();
}

/*==================================================================*/
/*                                                                  */
/*  Query_AddBoolTerm () - Adds a boolean term to the linked list   */
/*                        of terms that form the query.             */
/*                                                                  */
/*==================================================================*/

static StateDataPtr Query_AddBoolTerm (ForM f, Int2 currDb, Int2 currFld, CharPtr strs, Int2 state, Int4 num)

{
  StateDataPtr  sdp;
  FormInfoPtr   pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (f);

  /*-----------------------------*/
  /* Add the term and display it */
  /*-----------------------------*/

  sdp = CreateTerm (f, currDb, currFld, strs, state, num, FALSE);
  if (sdp != NULL) {
    DisplayTerm (pFormInfo, strs, sdp->field, num);
  }

  /*------------------------------*/
  /* Return a ptr to the new term */
  /*------------------------------*/

  return sdp;
}

/*==================================================================*/
/*                                                                  */
/*  ProcessMeshTerm () -                                            */
/*                                                                  */
/*==================================================================*/

static void ProcessMeshTerm (FormInfoPtr pFormInfo, CharPtr str)

{
  Int4     iTermCount;
  CharPtr  ptr;
  CharPtr  text;

  /*---------------------------*/
  /* Trim off extra spaces and */
  /* curly-bracketed info      */
  /*---------------------------*/

  ptr = str;
  while (*ptr != '\0' && *ptr != '{') {
    ptr++;
  }
  *ptr = '\0';

  /*-----------------------------*/
  /* Get the term count from the */
  /* term selection window.      */
  /*-----------------------------*/

  text = GetDocText (pFormInfo->availDoc, pFormInfo->availItem, pFormInfo->availRow, E2_COUNT_COL);
  TrimSpacesAroundString (text);
  StrToLong (text, &iTermCount);
  MemFree (text);

  /*-------------------------------*/
  /* Load the chosen term into the */
  /* query refinement window.      */
  /*-------------------------------*/

  Query_AddBoolTerm (pFormInfo->form, pFormInfo->currDb, pFormInfo->currField, str, STATE_ON, iTermCount);
}

/*==================================================================*/
/*                                                                  */
/*  ValNodeMerge () -                                               */
/*                                                                  */
/*==================================================================*/

static CharPtr ValNodeMerge (ValNodePtr head)

{
  size_t      len;
  CharPtr     str;
  CharPtr     to;
  ValNodePtr  vnp;

  if (head == NULL) return NULL;

  for (vnp = head, len = 0; vnp != NULL; vnp = vnp->next)
    len += StringLen ((CharPtr) vnp->data.ptrvalue);

  str = (CharPtr) MemNew (len + 4);
  if (str == NULL) return NULL;

  for (vnp = head, to = str; vnp != NULL; vnp = vnp->next)
    to = StringMove (to, (CharPtr) vnp->data.ptrvalue);

  return str;
}

/*==================================================================*/
/*                                                                  */
/*  FetchFromTermList () -                                          */
/*                                                                  */
/*==================================================================*/

static CharPtr FetchFromTermList (DoC d, Int2 item, Pointer ptr)

{
  FormInfoPtr          pFormInfo;
  CharPtr              db, field;
  Entrez2RequestPtr    e2rq;
  Entrez2ReplyPtr      e2ry;
  Entrez2TermListPtr   e2tl;
  Entrez2TermPtr       e2tp;
  Entrez2FieldInfoPtr  fieldInfo;
  Int4                 firstPos, numTerms, page;
  ValNodePtr           head = NULL;
  CharPtr              rsult = NULL;
  Char                 str [256], tmp [16];

  pFormInfo = (FormInfoPtr) GetObjectExtra (d);

  if (item < 1) return NULL;

  page = item - 1;

  db = DBGetNameFromID (pFormInfo->currDb);
  fieldInfo = FieldGetInfo (pFormInfo->currDb, pFormInfo->currField);
  field = fieldInfo->field_name;
  if (StringHasNoText (db) || StringHasNoText (field)) return NULL;

  firstPos = pFormInfo->availPageSize * page;
  numTerms = pFormInfo->availPageSize;
  if (firstPos + numTerms > pFormInfo->availNumTerms)
    numTerms = pFormInfo->availNumTerms - firstPos;

  e2rq = EntrezCreateGetTermListRequest (db, field, firstPos, numTerms);
  if (e2rq == NULL) return NULL;

  if (ShowASN () == TRUE)
    DisplayEntrezRequest (e2rq);

  e2ry = SpecialEntrezSynchronousQuery (e2rq);
  if (e2ry != NULL) {
    if (ShowASN () == TRUE)
      DisplayEntrezReply (e2ry);

    e2tl = EntrezExtractTermListReply (e2ry);
    if (e2tl != NULL)
      for (e2tp = e2tl->list; e2tp != NULL; e2tp = e2tp->next) {
        StringNCpy_0 (str, e2tp->term, sizeof (str) - 16);
        sprintf (tmp, "\t%ld\n", (long) e2tp->count);
        StringCat (str, tmp);
        ValNodeCopyStr (&head, 0, str);
      }
  }
  Entrez2RequestFree (e2rq);

  rsult = ValNodeMerge (head);
  ValNodeFreeData (head);

  return rsult;
}

/*==================================================================*/
/*                                                                  */
/*  GetTermPage () -                                                */
/*                                                                  */
/*==================================================================*/

static Int4 GetTermPage (FormInfoPtr pFormInfo, CharPtr str)

{
  CharPtr              db;
  CharPtr              field;
  Entrez2RequestPtr    e2rq;
  Entrez2ReplyPtr      e2ry;
  Int4                 pagesize = -1;
  Int4                 pos = -1;
  Int4                 rsult = -1;
  Entrez2FieldInfoPtr  fieldInfo;

  db = DBGetNameFromID (pFormInfo->currDb);
  fieldInfo = FieldGetInfo (pFormInfo->currDb, pFormInfo->currField);
  field = fieldInfo->field_name;

  if (StringHasNoText (db) || StringHasNoText (field)) return rsult;

  e2rq = EntrezCreateGetTermPositionRequest (db, field, str);
  if (e2rq == NULL) return rsult;

  if (ShowASN () == TRUE)
    DisplayEntrezRequest (e2rq);

  e2ry = SpecialEntrezSynchronousQuery (e2rq);
  if (e2ry != NULL) {
    if (ShowASN () == TRUE)
      DisplayEntrezReply (e2ry);

    pos = EntrezExtractTermPosReply (e2ry);
    pagesize = pFormInfo->availPageSize;
    if (pagesize > 0)
      rsult = (pos - 1) / pagesize;
  }
  Entrez2RequestFree (e2rq);

  if (pos == -1) return -1;

  return rsult;
}

/*==================================================================*/
/*                                                                  */
/*  ChangeUnderscoreToSpace () -                                    */
/*                                                                  */
/*==================================================================*/

static void ChangeUnderscoreToSpace (CharPtr str)

{
  Char     ch;
  CharPtr  ptr;

  if (str == NULL) return;
  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    if (ch == '_')
      *ptr = ' ';
    ptr++;
    ch = *ptr;
  }
}

/*==================================================================*/
/*                                                                  */
/*  ChangePeriodToSpace () -                                    */
/*                                                                  */
/*==================================================================*/

static void ChangePeriodToSpace (CharPtr str)

{
  Char     ch;
  CharPtr  ptr;

  if (str == NULL) return;
  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    if (ch == '.')
      *ptr = ' ';
    ptr++;
    ch = *ptr;
  }
}

/*==================================================================*/
/*                                                                  */
/*  ChangeMeshSlashSymbol () -                                      */
/*                                                                  */
/*==================================================================*/

static void ChangeMeshSlashSymbol (CharPtr str)

{
  Char     ch;
  CharPtr  ptr;

  if (str == NULL) return;

  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    if (ch == '/')
      *ptr = '\31';
    ptr++;
    ch = *ptr;
  }
}

/*==================================================================*/
/*                                                                  */
/*  FindLineOfText () - Assumes a string of text separated by       */
/*                     newline, characters, and terminated by a     */
/*                     newline                                      */
/*                                                                  */
/*==================================================================*/

static Int2 FindLineOfText (CharPtr text, CharPtr str)

{
  Char          ch;
  Int2          compare, idx, left, mid, numLines, right;
  CharPtr PNTR  index;
  CharPtr       lookfor, ptr;
  size_t        len;

  /* Check parameters */

  if (StringLen (text) == 0 || StringLen (str) == 0) return 0;

  /* Count the number of lines to be searched */

  mid = 0;
  lookfor = StringSave (str);
  ChangeMeshSlashSymbol (lookfor);
  numLines = 0;
  ptr = text;
  ch = *ptr;
  while (ch != '\0') {
    if (ch == '\n')
      numLines++;
    ptr++;
    ch = *ptr;
  }

  if (numLines <= 0) {
    MemFree (lookfor);
    return 0;
  }

  /* Create an array of pointers to the lines */

  index = MemNew (sizeof (CharPtr) * (size_t) (numLines + 3));
  if (index != NULL) {
    idx = 0;
    ptr = text;
    ch = *ptr;
    index [idx] = ptr;
    while (ch != '\0') {
      if (ch == '\n') {
        idx++;
        *ptr = '\0';
        ptr++;
        ch = *ptr;
        index [idx] = ptr;
      } else {
        ptr++;
        ch = *ptr;
      }
    }
  }

  /* Do a binary search for the search string */

  left = 1;
  right = numLines;
  while (left <= right) {
    mid = (left + right) / 2;
    compare = StringICmp (lookfor, index [mid - 1]);
    if (compare <= 0)
      right = mid - 1;
    if (compare >= 0)
      left = mid + 1;
  }

  if (left <= right + 1) {
    len = StringLen (lookfor);
    compare = StringNICmp (lookfor, index [mid - 1], len);
    if (compare > 0) {
      mid++;
      /*
      if (mid <= numLines) {
	compare = StringNICmp (lookfor, index [mid - 1], len);
	if (compare > 0)
	  mid = 0;
      } else
	mid = 0;
      */
    }
    /*
    else
      mid = 0;
    */
  }
  
  /* Clean up and return */

  MemFree (index);
  MemFree (lookfor);
  return mid;
}

/*==================================================================*/
/*                                                                  */
/*  ScrollToText () -                                               */
/*                                                                  */
/*==================================================================*/

static void ScrollToText (FormInfoPtr pFormInfo,
			  CharPtr     str,
			  Int2        page,
			  Boolean     hard,
			  Boolean     exact)

{
  Int2     compare, oldItem, oldRow, perfect, row;
  size_t   len;
  Int4     numLines, startsAt;
  BaR      sb;
  Char     temp [256];
  CharPtr  text;

  /* Check parameters */

  if (StringHasNoText (str) || page == 0) return;

  /* Clean up the text string */

  StringNCpy_0 (temp, str, sizeof (temp));
  ChangeUnderscoreToSpace (temp);
  ChangePeriodToSpace (temp);
  ChangeMeshSlashSymbol (temp);

  /* Get the information on the current page of data */

  text = GetDocText (pFormInfo->availDoc, page, 0, 1);
  GetItemParams4 (pFormInfo->availDoc, page, &startsAt, NULL, NULL, NULL, NULL);
  GetDocParams4 (pFormInfo->availDoc, NULL, &numLines);
  ChangeMeshSlashSymbol (text);

  /* Search for the text string in the current page of data */

  row = FindLineOfText (text, temp);
  MemFree (text);

  /* If the text string is not on the page */
  /* then go to the top of the page.       */

  if (row < 1 || row > numLines)
    /*
    return;
    */
    row = 1;

  /* If the term is not on the current page then */
  /* we have to load and search the new page.    */

  /*
  if (row <= 1 || row >= numLines) {
    LoadAvailList (pFormInfo, str);
    return;
  }
  */

  /* Redisplay the page with the text string highlighted */
  
  startsAt += row - 1;
  sb = GetSlateVScrollBar ((SlatE) pFormInfo->availDoc);
  CorrectBarMax (sb, numLines - 7);
  if (!RowIsVisible (pFormInfo->availDoc, page, row, NULL, NULL)) {
    ForceFormat (pFormInfo->availDoc, page);    /* forces UpdateLineStarts */
    GetItemParams4 (pFormInfo->availDoc, page, &startsAt, NULL, NULL,
		    NULL, NULL);
    GetDocParams4 (pFormInfo->availDoc, NULL, &numLines);
    startsAt += row - 1;
    sb = GetSlateVScrollBar ((SlatE) pFormInfo->availDoc);
    CorrectBarMax (sb, numLines - 7);
    if (hard)
      SetBarValue (sb, startsAt);
    else
      CorrectBarValue (sb, startsAt);
  }
  text = GetDocText (pFormInfo->availDoc, page, row, 1);
  ChangeMeshSlashSymbol (text);
  perfect = StringICmp (text, temp);
  len = StringLen (temp);
  compare = StringNICmp (text, temp, len);
  MemFree (text);
  if (compare == 0) {
    oldItem = pFormInfo->availItem;
    oldRow = pFormInfo->availRow;
    if (oldItem != page || oldRow != row) {
      pFormInfo->availItem = page;
      pFormInfo->availRow = row;
      InvalDocRows (pFormInfo->availDoc, oldItem, oldRow, oldRow);
      InvalDocRows (pFormInfo->availDoc, pFormInfo->availItem,
		    pFormInfo->availRow, pFormInfo->availRow);
    }
    if (exact) {
      if (perfect == 0)
        pFormInfo->textChanged = FALSE;
    } else
      pFormInfo->textChanged = FALSE;
  } else {
    pFormInfo->availItem = 0;
    pFormInfo->availRow = 0;
  }
}

/*==================================================================*/
/*                                                                  */
/*  LoadAvailList () -                                              */
/*                                                                  */
/*==================================================================*/

static Boolean LoadAvailList (FormInfoPtr pFormInfo, CharPtr str)

{
  Int4                 count;
  Entrez2FieldInfoPtr  fieldInfo;
  Int4                 pagesize;
  Int2                 numpages = 0;
  Int2                 page = 0;

  pFormInfo->availItem = 0;
  pFormInfo->availRow = 0;
  Reset (pFormInfo->availDoc);
  SetDocCache (pFormInfo->availDoc, NULL, NULL, NULL);
  Update ();
  if (str [0] == '\0') return FALSE;

  fieldInfo = FieldGetInfo (pFormInfo->currDb, pFormInfo->currField);
  count = fieldInfo->term_count;
  if (count == 0) return FALSE;

  pFormInfo->availNumTerms = count;
  pagesize = 1;
  while (count > pagesize) {
    count /= 2;
    pagesize *= 2;
  }

  while (count < 16000 && pagesize > 128) {
    count *= 2;
    pagesize /= 2;
  }
  pFormInfo->availPageSize = pagesize;
  numpages = (Int2) ((pFormInfo->availNumTerms + pagesize - 1) / pagesize);
  pFormInfo->availNumPages = numpages;

  BulkAppendItem (pFormInfo->availDoc, numpages, FetchFromTermList, pagesize, &availParFmt, availColFmt, systemFont);
  AppendText (pFormInfo->availDoc, "\n\n\n\n\n\n\n\n\n\n\n\n \n", &availParFmt, availColFmt, systemFont);

  SetDocCache (pFormInfo->availDoc, StdPutDocCache, StdGetDocCache, StdResetDocCache);

  page = GetTermPage (pFormInfo, str);
  pFormInfo->availCurrentPage = page + 1;
  if (page != -1) {
    ScrollToText (pFormInfo, str, pFormInfo->availCurrentPage, FALSE, FALSE);
  }

  InvalDocument (pFormInfo->availDoc);
  Update ();
  AdjustDocScroll (pFormInfo->availDoc);

  return TRUE;
}

/*==================================================================*/
/*                                                                  */
/*  ProcessSelectionTerm () -                                       */
/*                                                                  */
/*==================================================================*/

static void ProcessSelectionTerm (FormInfoPtr pFormInfo, CharPtr str)

{
  Int4     iTermCount;
  Char     termText [E2_STR_BUFF_SIZE];
  CharPtr  text;

  /* If a new term has been typed into the */
  /* term text entry box, then do a lookup */
  /* of that term.                         */

  if (pFormInfo->textChanged) {
    LoadAvailList (pFormInfo, str);
    pFormInfo->textChanged = FALSE;
    pFormInfo->okayToAccept = FALSE;
  }

  /* Process the highlighted term from */
  /* the term selection box            */

  else if (pFormInfo->okayToAccept &&
	   pFormInfo->availItem > 0 &&
	   pFormInfo->availRow > 0) {

    text = GetDocText (pFormInfo->availDoc, pFormInfo->availItem,
		       pFormInfo->availRow, E2_TERM_COL);
    if (NULL == text)
      return;

    GetTitle (pFormInfo->termText, termText, E2_STR_BUFF_SIZE);

    /* If the higlighted term is the same as */
    /* in the text entry box, then add the   */
    /* term to the Query Refinement list.    */

    if (StringICmp (termText, text) == 0) {
      if (StringICmp (str, text) == 0) {
	MemFree (text);
	text = GetDocText (pFormInfo->availDoc, pFormInfo->availItem,
			   pFormInfo->availRow, E2_COUNT_COL);
	TrimSpacesAroundString (text);
	StrToLong (text, &iTermCount);
	MemFree (text);
	Query_AddBoolTerm (pFormInfo->form, pFormInfo->currDb,
			   pFormInfo->currField, str, STATE_ON,
			   iTermCount);
	Select (pFormInfo->termText);
	pFormInfo->okayToAccept = FALSE;
      }
    }

    /* Otherwise, just move the higlighted */
    /* term to the text box.               */

    else {
      TrimSpacesAroundString (text);
      SafeSetTitle (pFormInfo->termText, text);
      Select (pFormInfo->termText);
      Update ();
      StringNCpy_0 (str, text, E2_STR_BUFF_SIZE);
    }

  }
}

/*==================================================================*/
/*                                                                  */
/*  ProcessTaxonomyTerm () -                                        */
/*                                                                  */
/*==================================================================*/

static void ProcessTaxonomyTerm (FormInfoPtr pFormInfo, CharPtr str)

{
  Int4     iTermCount;
  CharPtr  text;

  /*----------------------*/
  /* Get the term's count */
  /*----------------------*/

  text = GetDocText (pFormInfo->availDoc, pFormInfo->availItem, pFormInfo->availRow, E2_COUNT_COL);
  TrimSpacesAroundString (text);
  StrToLong (text, &iTermCount);
  MemFree (text);

  /*-----------------------------------*/
  /* Add the term to the current query */
  /*-----------------------------------*/

  Query_AddBoolTerm (pFormInfo->form, pFormInfo->currDb, pFormInfo->currField, str, STATE_ON, iTermCount);
  Select (pFormInfo->termText);
  pFormInfo->okayToAccept = FALSE;
}

/*===================================================================*/
/*                                                                   */
/*  ReadAFew () - Fetches the count for a given term and for several */
/*               following terms and displays them in the term list  */
/*               panel.                                              */
/*                                                                   */
/*===================================================================*/

static Boolean ReadAFew (FormInfoPtr pFormInfo, CharPtr str, CharPtr actual, size_t maxsize, Int4Ptr count)

{
  Char                 displayStr [256];
  Boolean              found;
  Entrez2TermListPtr   e2TermListPtr;
  Entrez2FieldInfoPtr  fieldInfo;
  CharPtr              dbName;
  CharPtr              fieldName;
  Int2                 row;
  Entrez2TermPtr       termPtr;

  /*------------------------------*/
  /* Get the names of the current */
  /* db and the current field.    */
  /*------------------------------*/

  found = FALSE;
  dbName = DBGetNameFromID (pFormInfo->currDb);
  fieldInfo = FieldGetInfo (pFormInfo->currDb, pFormInfo->currField);
  fieldName = fieldInfo->field_name;
  if (StringHasNoText (dbName) || StringHasNoText (fieldName)) return FALSE;

  /*----------------------------------------*/
  /* Fetch a count for the requested string */
  /* and for several strings after it.      */
  /*----------------------------------------*/

  e2TermListPtr = Query_FetchSeveralCounts (dbName, fieldName, str, AVAIL_WINDOW_ROWS);

  if (e2TermListPtr == NULL) return FALSE;

  /*-------------------------------*/
  /* Did we find the desired term? */
  /*-------------------------------*/

  if (StringICmp (e2TermListPtr->list->term, str) == 0) {
    found = TRUE;
  } else {
    found = FALSE;
    StringCpy (actual, e2TermListPtr->list->term);
  }

  /*----------------------------*/
  /* Send the count back to the */
  /* calling function.          */
  /*----------------------------*/

  *count = e2TermListPtr->list->count;

  /*-------------------------------------*/
  /* Display the term and it's neighbors */
  /* in the term list window.            */
  /*-------------------------------------*/

  termPtr = e2TermListPtr->list;

  for (row = 1, termPtr = e2TermListPtr->list; row <= *count, termPtr != NULL; row++, termPtr = termPtr->next) {
    sprintf (displayStr, "%s\t%ld\t%d\n", termPtr->term, (long) (termPtr->count), (int) (termPtr->is_leaf_node ? 1 : 0));
    AppendText (pFormInfo->availDoc, displayStr, &availParFmt, availColFmt, systemFont);
  }

  if (found) {
    pFormInfo->availItem = 1;
    pFormInfo->availRow = 1;
  }
  InvalDocument (pFormInfo->availDoc);

  /*---------------------*/
  /* Return successfully */
  /*---------------------*/

  return found;
}

/*==================================================================*/
/*                                                                  */
/*  ProcessMultipleTerms () -                                       */
/*                                                                  */
/*==================================================================*/

static void ProcessMultipleTerms (FormInfoPtr pFormInfo, CharPtr strs)

{
  Char     actual [256];
  Char     ch;
  Int4     count;
  Boolean  found;
  Int2     i;
  Int2     j;
  Int2     k;
  Char     str [256];

  i = 0;
  while (StringLen (strs + i) > 0) {

    /*--------------------------------*/
    /* Parse a term out of the string */
    /*--------------------------------*/

    StringNCpy_0 (str, strs + i, sizeof (str));
    k = 0;
    ch = str [k];
    while (ch == ' ') {
      k++;
      ch = str [k];
    }
    j = 0;
    if (ch == '"') {
      k++;
      ch = str [j + k];
      while (ch != '\0' && ch != '"') {
        j++;
        ch = str [j + k];
      }
      if (ch == '"') {
        str [j + k] = '\0';
        i += j + k + 1;
      } else
        i += j + k;
    } else {
      while (ch != '\0' && ch != ' ') {
        j++;
        ch = str [j + k];
      }
      if (ch == ' ') {
        str [j + k] = '\0';
        i += j + k + 1;
      } else
        i += j + k;
    }

    /*-----------------------------------*/
    /* If we successfully parsed a term, */
    /* look it up and then add it to the */
    /* boolean query if found.           */
    /*-----------------------------------*/

    if (StringLen (str + k) > 0) {
      pFormInfo->availItem = 0;
      pFormInfo->availRow = 0;
      Reset (pFormInfo->availDoc);
      actual [0] = '\0';
      found = ReadAFew (pFormInfo, str + k, actual, sizeof (actual), &count);
      Update ();
      if (found) {
        Query_AddBoolTerm (pFormInfo->form, pFormInfo->currDb, pFormInfo->currField, str + k, STATE_ON, count);
      } else {
        Query_AddBoolTerm (pFormInfo->form, pFormInfo->currDb, pFormInfo->currField, actual, STATE_OFF, count);
      }
    }
  }

  /*-------------------------------------*/
  /* Clear out the term selection window */
  /* and the term entry fields.          */
  /*-------------------------------------*/

  pFormInfo->availItem = 0;
  pFormInfo->availRow = 0;
  Reset (pFormInfo->availDoc);

  InvalDocument (pFormInfo->availDoc);
  SafeSetTitle (pFormInfo->termText, "");
  SafeSetTitle (pFormInfo->fromText, "");
  SafeSetTitle (pFormInfo->toText, "");
  SafeDisable (pFormInfo->acceptButton);
  if (Visible (pFormInfo->termText)) {
    Select (pFormInfo->termText);
  }
}

/*==================================================================*/
/*                                                                  */
/*  RecalculateChosen () -                                          */
/*                                                                  */
/*==================================================================*/

static void RecalculateChosen (FormInfoPtr pFormInfo)

{
  Int4  count;
  Char  tmp [32];

  /*-------------------------------*/
  /* Get a count of documents that */
  /* satisfy the current query.    */
  /*-------------------------------*/

  count = Query_FetchCount (pFormInfo->form);

  /*-----------------------------------*/
  /* Update the retrieve message count */
  /*-----------------------------------*/

  if (count < 1)
    sprintf (tmp, "Retrieve 0 Documents");
  else if (count > 1)
    sprintf (tmp, "Retrieve %ld Documents", (long) count);
  else
    sprintf (tmp, "Retrieve %ld Document", (long) count);

  SafeSetTitle (pFormInfo->retrieveButton, tmp);
  SafeSetTitle (pFormInfo->advRetrieveButton, tmp);

  if (count < 1 || count > 32000) {
    SafeDisable (pFormInfo->retrieveButton);
    SafeDisable (pFormInfo->advRetrieveButton);
  } else {
    SafeEnable (pFormInfo->retrieveButton);
    SafeEnable (pFormInfo->advRetrieveButton);
  }
}

/*==================================================================*/
/*                                                                  */
/*  Query_TruncateCount () -                                        */
/*                                                                  */
/*==================================================================*/

static Int4 Query_TruncateCount (FormInfoPtr pFormInfo, CharPtr str)

{
  CharPtr                 dbName;
  Boolean                 doNotExplode;
  Boolean                 doNotTranslate;
  Int4                    count;
  Entrez2RequestPtr       e2RequestPtr;
  Entrez2ReplyPtr         e2ReplyPtr;
  Entrez2BooleanReplyPtr  e2BooleanPtr;
  Entrez2FieldInfoPtr     fieldInfo;

  if (pFormInfo == NULL || StringHasNoText (str)) return 0;

  /*--------------------------------*/
  /* Get the name of the current DB */
  /*--------------------------------*/

  dbName = DBGetNameFromID (pFormInfo->currDb);
  if (StringHasNoText (dbName)) return 0;

  fieldInfo = FieldGetInfo (pFormInfo->currDb, pFormInfo->currField);
  if (fieldInfo == NULL) return 0;

  /*---------------------------------*/
  /* Create an empty Boolean request */
  /*---------------------------------*/

  e2RequestPtr = EntrezCreateBooleanRequest (FALSE, FALSE, dbName, NULL, 0, 0, NULL, 0, 0);
  if (e2RequestPtr == NULL) return 0;

  /*--------------------------------------------------*/
  /* Send truncated term count request to the server. */
  /*--------------------------------------------------*/

  doNotTranslate = FALSE;
  doNotExplode = !GetStatus (pFormInfo->explodeItem);
  EntrezAddToBooleanRequest (e2RequestPtr, NULL, 0, fieldInfo->field_name,
                             str, NULL, 0, 0, NULL, NULL,
                             doNotExplode, doNotTranslate);

  if (ShowASN () == TRUE)
    DisplayEntrezRequest (e2RequestPtr);

  e2ReplyPtr = SpecialEntrezSynchronousQuery (e2RequestPtr);

  if (ShowASN () == TRUE)
    DisplayEntrezReply (e2ReplyPtr);

  if (e2ReplyPtr == NULL) return 0;

  /*----------------------------------*/
  /* Parse the count out of the reply */
  /*----------------------------------*/

  e2BooleanPtr = EntrezExtractBooleanReply (e2ReplyPtr);
  count = e2BooleanPtr->count;

  /*----------------------------------*/
  /* Clean up and return successfully */
  /*----------------------------------*/

  Entrez2BooleanReplyFree (e2BooleanPtr);
  Entrez2RequestFree (e2RequestPtr);

  return count;
}

/*==================================================================*/
/*                                                                  */
/*  Query_FetchRangeCount () -                                      */
/*                                                                  */
/*==================================================================*/

static Int4 Query_FetchRangeCount (FormInfoPtr pFormInfo, CharPtr str)

{
  CharPtr                 dbName;
  Int4                    count;
  Entrez2RequestPtr       e2RequestPtr;
  Entrez2ReplyPtr         e2ReplyPtr;
  Entrez2BooleanReplyPtr  e2BooleanPtr;
  Entrez2FieldInfoPtr     fieldInfo;
  Boolean                 doNotTranslate;

  if (pFormInfo == NULL || StringHasNoText (str)) return 0;

  /*--------------------------------*/
  /* Get the name of the current DB */
  /*--------------------------------*/

  dbName = DBGetNameFromID (pFormInfo->currDb);
  if (StringHasNoText (dbName)) return 0;

  fieldInfo = FieldGetInfo (pFormInfo->currDb, pFormInfo->currField);
  if (fieldInfo == NULL) return 0;

  /*---------------------------------*/
  /* Create an empty Boolean request */
  /*---------------------------------*/

  e2RequestPtr = EntrezCreateBooleanRequest (FALSE, FALSE, dbName, NULL, 0, 0, NULL, 0, 0);
  if (e2RequestPtr == NULL) return 0;

  /*--------------------------------------------------*/
  /* Send truncated term count request to the server. */
  /*--------------------------------------------------*/

  doNotTranslate = FALSE;
  EntrezAddToBooleanRequest (e2RequestPtr, NULL, 0, fieldInfo->field_name, str, NULL, 0, 0, NULL, NULL, FALSE, doNotTranslate);

  if (ShowASN () == TRUE)
    DisplayEntrezRequest (e2RequestPtr);

  e2ReplyPtr = SpecialEntrezSynchronousQuery (e2RequestPtr);

  if (ShowASN () == TRUE)
    DisplayEntrezReply (e2ReplyPtr);

  if (e2ReplyPtr == NULL) return 0;

  /*----------------------------------*/
  /* Parse the count out of the reply */
  /*----------------------------------*/

  e2BooleanPtr = EntrezExtractBooleanReply (e2ReplyPtr);
  if (e2BooleanPtr != NULL)
    count = e2BooleanPtr->count;
  else {
    Entrez2RequestFree (e2RequestPtr);
    return 0;
  }
    

  /*----------------------------------*/
  /* Clean up and return successfully */
  /*----------------------------------*/

  Entrez2BooleanReplyFree (e2BooleanPtr);
  Entrez2RequestFree (e2RequestPtr);

  return count;
}

/*===================================================================*/
/*                                                                   */
/* ProcessRangeTerms ()                                              */
/*                                                                   */
/*===================================================================*/

static void ProcessRangeTerms (FormInfoPtr pFormInfo)
{
  Char          fromStr [E2_STR_BUFF_SIZE];
  Char          toStr [E2_STR_BUFF_SIZE];
  Char          rangeStr [2 * E2_STR_BUFF_SIZE + 1];
  StateDataPtr  sdp;
  Int4          count;
  CharPtr       text;

  /* If a new term has been typed into the */
  /* 'To' text box, then do a lookup of    */
  /* that term.                            */

  if (pFormInfo->isToTextChanged) {
    GetTitle (pFormInfo->toText, toStr, E2_STR_BUFF_SIZE);
    pFormInfo->isValidTo = LoadAvailList (pFormInfo, toStr);
    pFormInfo->isToTextChanged = FALSE;
  }
  /* If we have valid 'From' and 'To' terms then */
  /* create a range term in the query window.    */

  else if ((pFormInfo->isValidFrom) && (pFormInfo->isValidTo)) {

    /* Get the 'from' and 'to' strings */

    GetTitle (pFormInfo->toText, toStr, E2_STR_BUFF_SIZE);
    GetTitle (pFormInfo->fromText, fromStr, E2_STR_BUFF_SIZE);
    sprintf(rangeStr, "%s:%s", fromStr, toStr);
    
    /* Get a count for the range */
    
    count = Query_FetchRangeCount (pFormInfo, rangeStr);
    
    /*  Add a term to the linked list */
    
    sdp = CreateTerm (pFormInfo->form, pFormInfo->currDb,
                      pFormInfo->currField, rangeStr, STATE_ON, count,
                      FALSE);
    if (NULL == sdp)
      return;
    
    /* Display the range as one term */
    
    DisplayTerm (pFormInfo, rangeStr, sdp->field, sdp->count);
    
    Select (pFormInfo->fromText);
    Reset (pFormInfo->availDoc);
    pFormInfo->okayToAccept = FALSE;
  }

  /* If a new term has been typed into the */
  /* 'From' text box, then do a lookup of  */
  /* that term.                            */

  else if (pFormInfo->isFromTextChanged) {
    GetTitle (pFormInfo->fromText, fromStr, E2_STR_BUFF_SIZE);
    pFormInfo->isValidFrom = LoadAvailList (pFormInfo, fromStr);
    pFormInfo->isFromTextChanged = FALSE;
  }
    
  else if (pFormInfo->okayToAccept &&
           pFormInfo->availItem > 0 &&
           pFormInfo->availRow > 0) {
    text = GetDocText (pFormInfo->availDoc, pFormInfo->availItem,
                       pFormInfo->availRow, E2_TERM_COL);
    if (text != NULL) {
      TrimSpacesAroundString (text);
      if (pFormInfo->currRangeField == E2_RANGE_FROM) {
        SafeSetTitle (pFormInfo->fromText, text);
        Select (pFormInfo->fromText);
        pFormInfo->isValidFrom = TRUE;
        pFormInfo->isFromTextChanged = FALSE;
      }
      else {
        SafeSetTitle (pFormInfo->toText, text);
        Select (pFormInfo->toText);
        pFormInfo->isValidTo = TRUE;
        pFormInfo->isToTextChanged = FALSE;
      }
      Update ();
    }
    MemFree (text);
  }

  /* Return successfully */
  
  return;
}

/*===================================================================*/
/*                                                                   */
/*  Accept_Callback () - Called when the accept button is pressed,   */
/*                      if fetches a term list or adds term to query */
/*                      list.                                        */
/*                                                                   */
/*===================================================================*/

static void Accept_Callback (ButtoN b)

{
  Int4          count;
  FormInfoPtr   pFormInfo;
  StateDataPtr  sdp;
  Char          str [E2_STR_BUFF_SIZE];

  pFormInfo = (FormInfoPtr) GetObjectExtra (b);

  str [0] = '\0';
  if (pFormInfo->currMode == RANGE_MODE)
    GetTitle (CurrentText (), str, E2_STR_BUFF_SIZE);
  else
    GetTitle (pFormInfo->termText, str, E2_STR_BUFF_SIZE);
  if (str [0] == '\0') return;

  WatchCursor ();
  switch (pFormInfo->currMode) {
    case TRANSLATE_MODE:
      Query_TranslateAndAddBoolTerm (pFormInfo->form, pFormInfo->currDb,
				     pFormInfo->currField, str, STATE_ON, 0);
      SafeSetTitle (pFormInfo->termText, "");
      if (Visible (pFormInfo->termText))
        Select (pFormInfo->termText);
      pFormInfo->availItem = 0;
      pFormInfo->availRow = 0;
      Reset (pFormInfo->availDoc);
      break;
    case SELECTION_MODE:
      ProcessSelectionTerm (pFormInfo, str);
      /*
      pFormInfo->availCurrentPage = 0;
      */
      break;
    case AUTOMATIC_MODE:
      ProcessMultipleTerms (pFormInfo, str);
      break;
    case WILD_CARD_MODE:
      StringCat (str, "*");
      count = Query_TruncateCount (pFormInfo, str);
      sdp = CreateTerm (pFormInfo->form, pFormInfo->currDb,
                        pFormInfo->currField, str, STATE_ON, count, FALSE);
      if (sdp != NULL) {
        DisplayTerm (pFormInfo, str, sdp->field, count);
      }
      SafeSetTitle (pFormInfo->termText, "");
      if (Visible (pFormInfo->termText))
        Select (pFormInfo->termText);
      pFormInfo->availItem = 0;
      pFormInfo->availRow = 0;
      Reset (pFormInfo->availDoc);
      break;
    case MESH_TREE_MODE:
      ProcessMeshTerm (pFormInfo, str);
      break;
    case TAXONOMY_MODE:
      ProcessTaxonomyTerm (pFormInfo, str);
      break;
    case RANGE_MODE:
      ProcessRangeTerms (pFormInfo);
      break;
    case LOOKUP_ACCN_MODE:
    case LOOKUP_UID_MODE:
    case VIEW_ACCN_MODE:
    case VIEW_UID_MODE:
      ProcessDirectLookup (pFormInfo, str);
      ArrowCursor ();
      Update ();
      return;
    default:
      break;
  }
  Update ();

  RecalculateChosen (pFormInfo);

  ArrowCursor ();
  Update ();
}

/*===================================================================*/
/*                                                                   */
/*  EvaluateRetrieve_Callback () - This is the callback for the      */
/*                                Evaluate/Retrieve button in        */
/*                                advanced query mode.               */
/*                                                                   */
/*===================================================================*/

static void EvaluateRetrieve_Callback (ButtoN evaluateRetrieveButton)

{
  FormInfoPtr  pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (evaluateRetrieveButton);

  if (pFormInfo->advQueryState == ADV_QUERY_EVALUATE_STATE) {
    WatchCursor ();
    Update ();
    RecalculateChosen (pFormInfo);
    pFormInfo->advQueryState = ADV_QUERY_RETRIEVE_STATE;
    ArrowCursor ();
  } else if (pFormInfo->advQueryState == ADV_QUERY_RETRIEVE_STATE) {
    pFormInfo->retrieveDocsProc (evaluateRetrieveButton);
    pFormInfo->advQueryState = ADV_QUERY_INVALID_STATE;
  }
}

/*===================================================================*/
/*                                                                   */
/*  DatabaseView_Callback () and FieldView_Callback () -             */
/*     Callback functions for items in Help menu.                    */
/*                                                                   */
/*===================================================================*/

static void DatabaseView_Callback (IteM i)

{
  Entrez2DbInfoPtr  e2db;
  Entrez2InfoPtr    e2ip;
  FILE              *fp;
  Int2              len;
  Int2              max_menu = 0;
  Int2              max_name = 0;
  Char              path [PATH_MAX];

  e2ip = Query_GetInfo ();
  if (e2ip == NULL) return;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp == NULL) return;
  for (e2db = e2ip->db_info; e2db != NULL; e2db = e2db->next) {
    len = (Int2) StringLen (e2db->db_name);
    max_name = MAX (len, max_name);
    len = (Int2) StringLen (e2db->db_menu);
    max_menu = MAX (len, max_menu);
  }
  max_name += 2;
  max_menu += 2;
  for (e2db = e2ip->db_info; e2db != NULL; e2db = e2db->next) {
    len = (Int2) StringLen (e2db->db_name);
    max_name = MAX (len, max_name);
    len = (Int2) StringLen (e2db->db_menu);
    max_menu = MAX (len, max_menu);
  }
  for (e2db = e2ip->db_info; e2db != NULL; e2db = e2db->next) {
    len = (Int2) StringLen (e2db->db_name);
    fprintf (fp, "%s", e2db->db_name);
    while (len < max_name) {
      fprintf (fp, " ");
      len++;
    }
    len = (Int2) StringLen (e2db->db_menu);
    fprintf (fp, "%s", e2db->db_menu);
    while (len < max_menu) {
      fprintf (fp, " ");
      len++;
    }
    fprintf (fp, "%s", e2db->db_descr);
    fprintf (fp, "\n");
  }
  FileClose (fp);
  LaunchGeneralTextViewer (path, "Database Summary");
  FileRemove (path);
}

static void FieldView_Callback (IteM i)

{
  Entrez2InfoPtr       e2ip;
  EnumFieldAssocPtr    fieldAlist;
  Entrez2FieldInfoPtr  fieldInfo;
  FILE                 *fp;
  Int2                 j;
  Int2                 len;
  Int2                 max_menu = 0;
  Int2                 max_name = 0;
  Char                 path [PATH_MAX];

  e2ip = Query_GetInfo ();
  if (e2ip == NULL) return;
  fieldAlist = CreateAllFieldsAlist (e2ip);
  if (fieldAlist == NULL) return;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp == NULL) return;
  for (j = 0; fieldAlist [j].name != NULL; j++) {
    fieldInfo = FieldGetInfoFromName (fieldAlist [j].name);
    len = (Int2) StringLen (fieldInfo->field_name);
    max_name = MAX (len, max_name);
    len = (Int2) StringLen (fieldInfo->field_menu);
    max_menu = MAX (len, max_menu);
  }
  max_name += 2;
  max_menu += 2;
  for (j = 0; fieldAlist [j].name != NULL; j++) {
    fieldInfo = FieldGetInfoFromName (fieldAlist [j].name);
    len = (Int2) StringLen (fieldInfo->field_name);
    fprintf (fp, "%s", fieldInfo->field_name);
    while (len < max_name) {
      fprintf (fp, " ");
      len++;
    }
    len = (Int2) StringLen (fieldInfo->field_menu);
    fprintf (fp, "%s", fieldInfo->field_menu);
    while (len < max_menu) {
      fprintf (fp, " ");
      len++;
    }
    fprintf (fp, "%s", fieldInfo->field_descr);
    fprintf (fp, "\n");
  }
  FileClose (fp);
  LaunchGeneralTextViewer (path, "Field Summary");
  FileRemove (path);
  FreeEnumFieldAlist (fieldAlist);
}

static CharPtr modeSummary [] = {
  "Automatic  Terms are processed through PubMed query engine",
  "Lookup     Accession looked up directly into Document window",
  "MeSH Tree  MeSH hierarchy above current level displayed in popup",
  "Multiple   Terms extracted and processed one at a time",
  "Range      From and To values entered as a composite term",
  "Selection  Available terms are displayed in Term Selection box",
  "Taxonomy   Organism hierarchy above current level displayed in popup",
  "View       Accession looked up directly into Viewer window",
  "Wild Card  Term appended with asterisk and processed",
  NULL
};

static void ModeView_Callback (IteM i)

{
  FILE  *fp;
  Int2  j;
  Char  path [PATH_MAX];

  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp == NULL) return;
  for (j = 0; modeSummary [j] != NULL; j++) {
    fprintf (fp, "%s\n", modeSummary [j]);
  }
  FileClose (fp);
  LaunchGeneralTextViewer (path, "Mode Summary");
  FileRemove (path);
}

/*==================================================================*/
/*                                                                  */
/*  AddPopupItem () - Add an item to a popup list. This is used to  */
/*                   create the lineage popup list in taxonomy      */
/*                   mode.                                          */
/*                                                                  */
/*==================================================================*/

static void AddPopupItem (PopuP p, CharPtr str, Int2 maxwid)

{
  Char     ch;
  Int2     hbounds;
  Int2     i;
  CharPtr  ptr;
  Char     title [256];
  Int2     wid;

  if (p != NULL && str != NULL) {
    StringNCpy_0 (title, str, sizeof (title));
    i = StringLen (title);
    title [i] = '\0';
    ptr = title;
    ch = *ptr;
    while (ch != '\0') {
      if (ch == '/') {
        *ptr = '-';
      }
      ptr++;
      ch = *ptr;
    }
#ifdef WIN_MAC
    hbounds = 13;
#endif
#ifdef WIN_MSWIN
    hbounds = 13;
#endif
#ifdef WIN_MOTIF
    hbounds = 24;
#endif
    maxwid -= StringWidth ("...") + hbounds * 2 + StringWidth (" ");
    wid = 0;
    i = 0;
    ch = title [i];
    while (ch != '\0' && wid <= maxwid) {
      wid += CharWidth (ch);
      i++;
      ch = title [i];
    }
    title [i] = '\0';
    if (ch != '\0' && i <= (Int2) StringLen (str)) {
      StringCat (title, "...");
    }
    PopupItem (p, title);
  }
}

/*==================================================================*/
/*                                                                  */
/*  AvailGetColWidth () - Returns the pixel width of the given      */
/*                       column.                                    */
/*                                                                  */
/*==================================================================*/

static Int2 AvailGetColWidth (Int2 columnNum)

{
  return availColFmt [columnNum].pixWidth;
}

/*==================================================================*/
/*                                                                  */
/*  RepopulateTaxonomy () - Given a term and a database, will       */
/*                         look up the term in the taxonomy tree    */
/*                         and populate the Term Selection window   */
/*                         with the term's 'children' in the tree.  */
/*                                                                  */
/*==================================================================*/

static Boolean RepopulateTaxonomy (FormInfoPtr pFormInfo, CharPtr taxterm)

{
  CharPtr              aLineage [MAX_TAXONOMY_TREE_DEPTH];
  Int2                 delta;
  CharPtr              dbName;
  CharPtr              fieldName;
  Int2                 i;
  Int2                 maxwidth;
  RecT                 r;
  RecT                 s;
  Char                 str [256];
  Int4                 taxId;
  Int2                 wid;
  Entrez2TermPtr       childPtr;
  Entrez2RequestPtr    e2Request;
  Entrez2ReplyPtr      e2Reply;
  Entrez2HierNodePtr   e2TermNode;
  Entrez2TermPtr       currentLineage;
  Entrez2FieldInfoPtr  fieldInfo;

  /*--------------------------*/
  /* Build an Entrez2 request */
  /*--------------------------*/

  taxId = 0;
  dbName = DBGetNameFromID (pFormInfo->currDb);
  fieldInfo = FieldGetInfo (pFormInfo->currDb, pFormInfo->currField);
  fieldName = fieldInfo->field_name;
  if (StringHasNoText (dbName) || StringHasNoText (fieldName)) return FALSE;

  e2Request = EntrezCreateGetTermHierarchyRequest (dbName, fieldName, taxterm, taxId);

  /*----------------------------------*/
  /* Send off the request and extract */
  /* the resulting reply.             */
  /*----------------------------------*/

  if (ShowASN () == TRUE)
    DisplayEntrezRequest (e2Request);

  e2Reply = SpecialEntrezSynchronousQuery (e2Request);

  if (ShowASN () == TRUE)
    DisplayEntrezReply (e2Reply);

  if (e2Reply == NULL) {
    Show (pFormInfo->taxLineagePopup);
    Update ();
    return FALSE;
  }

  e2TermNode = EntrezExtractHierNodeReply (e2Reply);
  if (e2TermNode == NULL) {
    Show (pFormInfo->taxLineagePopup);
    Update ();
    return FALSE;
  }

  Entrez2RequestFree (e2Request);

  /*-------------------------*/
  /* Clear any old terms out */
  /* of the Avail window.    */
  /*-------------------------*/

  SafeSetTitle (pFormInfo->termText, taxterm);
  Hide (pFormInfo->taxLineagePopup);
  Reset (pFormInfo->taxLineagePopup);
  pFormInfo->taxStrings = ValNodeFreeData (pFormInfo->taxStrings);
  pFormInfo->availItem = 0;
  pFormInfo->availRow = 0;
  Reset (pFormInfo->availDoc);
  SetDocCache (pFormInfo->availDoc, NULL, NULL, NULL);
  Update ();
  maxwidth = AvailGetColWidth (0);

  /*------------------------------------------------*/
  /* Create a selection list of a term's "lineage"  */
  /*    NOTE: The lineage is provided in the        */
  /*          reverse order from how we want to     */
  /*          add it to the list box, so they must  */
  /*          be put into a random-access structure */
  /*          and then reverse added.               */
  /*------------------------------------------------*/

  for (i = 0; i < e2TermNode->lineage_count; i++) {
    if (i == 0)
      currentLineage = e2TermNode->lineage;
    else
      currentLineage = currentLineage->next;

    if (currentLineage == NULL)
      break;

    aLineage [i] = currentLineage->term;
  }

  for (i = e2TermNode->lineage_count - 1; i >= 0; i--) {
    AddPopupItem (pFormInfo->taxLineagePopup, aLineage [i], maxwidth);
    ValNodeCopyStr (&(pFormInfo->taxStrings), 0, aLineage [i]);
  }

  /*----------------------------------------------*/
  /* Adjust the size of the Pull Down list box to */
  /* accomodate the lineage list just created.    */
  /*----------------------------------------------*/

  SetValue (pFormInfo->taxLineagePopup, e2TermNode->lineage_count);
  ObjectRect (pFormInfo->taxLineagePopup, &r);
  ObjectRect (pFormInfo->availDoc, &s);
  InsetRect (&s, 4, 4);
  wid = r.right - r.left;
  delta = (maxwidth - wid) / 2;
  r.left = s.left + delta;
  r.right = r.left + wid;
  SetPosition (pFormInfo->taxLineagePopup, &r);

  /*---------------------------------*/
  /* Add the children of the current */
  /* term to the Selection Box.      */
  /*---------------------------------*/

  childPtr = e2TermNode->children;

  for (i = 0; i < e2TermNode->child_count; i++) {
    sprintf (str, "%s\t%ld\t%d\n", childPtr->term, (long) (childPtr->count), (int) (childPtr->is_leaf_node ? 1 : 0));
    AppendText (pFormInfo->availDoc, str, &availParFmt, availColFmt, systemFont);
    childPtr = childPtr->next;
  }

  for (i = e2TermNode->child_count; i < 7; i++)
    AppendText (pFormInfo->availDoc, " \n", &availParFmt, availColFmt, systemFont);

  /*-------------------------*/
  /* Redraw the avail window */
  /* with the new terms.     */
  /*-------------------------*/

  InvalDocument (pFormInfo->availDoc);
  Show (pFormInfo->taxLineagePopup);
  Update ();
  AdjustDocScroll (pFormInfo->availDoc);

  return TRUE;
}

/*==================================================================*/
/*                                                                  */
/*  RepopulateTaxonomyRoot () -                                     */
/*                                                                  */
/*==================================================================*/

static void RepopulateTaxonomyRoot (FormInfoPtr pFormInfo)

{
  CharPtr  dbName;

  dbName = DBGetNameFromID (pFormInfo->currDb);

  if (StringICmp (dbName, "PubMed") == 0) {
    RepopulateTaxonomy (pFormInfo, "All MeSH Categories");
  } else {
    RepopulateTaxonomy (pFormInfo, "root");
  }
}

/*===================================================================*/
/*                                                                   */
/*  ChangeTaxParents_Callback () - Called when a 'Parent' term is    */
/*                                selected from the taxonomy lineage */
/*                                popup list.                        */
/*                                                                   */
/*                                Makes the selected term the active */
/*                                one and re-queries for it.         */
/*                                                                   */
/*===================================================================*/

static void ChangeTaxParents_Callback (PopuP p)

{
  FormInfoPtr  pFormInfo;
  Int2         val;
  ValNodePtr   vnp;

  pFormInfo = (FormInfoPtr) GetObjectExtra (p);

  val = GetValue (p);
  if (val <= 0) return;

  vnp = pFormInfo->taxStrings;
  while (val > 1 && vnp != NULL) {
    val--;
    vnp = vnp->next;
  }
  if (vnp != NULL) {
    WatchCursor ();
    RepopulateTaxonomy (pFormInfo, (CharPtr) vnp->data.ptrvalue);
    ArrowCursor ();
  }
}

/*===================================================================*/
/*                                                                   */
/*  TextAction () - Called when a key is type into the "Term:" text  */
/*                  entry box.  Enables/disables the accept button   */
/*                  and scroll to text.                              */
/*                                                                   */
/*===================================================================*/

static void TextAction (TexT t)

{
  Int2         i;
  FormInfoPtr  pFormInfo;
  Char         str [256];

  pFormInfo = (FormInfoPtr) GetObjectExtra (t);

  /* Enable or disable the 'Accept' button based */
  /* on the mode and the current text.           */

  if (pFormInfo->currMode == RANGE_MODE) {
    if (TextHasNoText (pFormInfo->fromText) &&
        TextHasNoText (pFormInfo->toText))
      SafeDisable (pFormInfo->acceptButton);
    else
      SafeEnable (pFormInfo->acceptButton);
  } else {
    GetTitle (pFormInfo->termText, str, sizeof (str));
    if (StringHasNoText (str)) {
      if (pFormInfo->currMode == SELECTION_MODE && str [0] == ' ')
        SafeEnable (pFormInfo->acceptButton);
      else
        SafeDisable (pFormInfo->acceptButton);
    } else
      SafeEnable (pFormInfo->acceptButton);
  }

  /* Get the current text */

  GetTitle (t, str, sizeof (str));

  /* If there is no text then clear */
  /* the 'Term Selection' window.   */

  if (str [0] == '\0') {
    pFormInfo->availItem = 0;
    pFormInfo->availRow = 0;
    Reset (pFormInfo->availDoc);
    for (i = 0; i < 7; i++)
      AppendText (pFormInfo->availDoc, " \n", &availParFmt, availColFmt,
		  systemFont);
    InvalDocument (pFormInfo->availDoc);
    pFormInfo->textChanged = FALSE;
    pFormInfo->okayToAccept = FALSE;
    pFormInfo->availCurrentPage = 0;
  }

  /* If there is text, then attempt to scroll */
  /* to it in the 'Term Selection' window.    */

  else {
    pFormInfo->textChanged = TRUE;
    pFormInfo->okayToAccept = FALSE;
    if (pFormInfo->availCurrentPage > 0 &&
	pFormInfo->currMode == SELECTION_MODE)
      ScrollToText (pFormInfo, str, pFormInfo->availCurrentPage, TRUE, TRUE);
    Update ();
  }
}

/*==================================================================*/
/*                                                                   */
/*  ToTextAction () -                                                */
/*                                                                   */
/*===================================================================*/

static void ToTextAction (TexT t)

{
  Int2         i;
  FormInfoPtr  pFormInfo;
  Char         str [256];

  pFormInfo = (FormInfoPtr) GetObjectExtra (t);

  pFormInfo->currRangeField = E2_RANGE_TO;

  if (TextHasNoText (pFormInfo->fromText) &&
      TextHasNoText (pFormInfo->toText))
    SafeDisable (pFormInfo->acceptButton);
  else
    SafeEnable (pFormInfo->acceptButton);
  
  GetTitle (t, str, sizeof (str));
  if (str [0] == '\0') {
    pFormInfo->availItem = 0;
    pFormInfo->availRow = 0;
    Reset (pFormInfo->availDoc);
    for (i = 0; i < 7; i++)
      AppendText (pFormInfo->availDoc, " \n", &availParFmt, availColFmt, systemFont);
    InvalDocument (pFormInfo->availDoc);
    pFormInfo->isToTextChanged = FALSE;
    pFormInfo->isValidTo = FALSE;
    pFormInfo->availCurrentPage = 0;
  } else {
    pFormInfo->isToTextChanged = TRUE;
    pFormInfo->isValidTo = TRUE;
    if (pFormInfo->availCurrentPage > 0)
      ScrollToText (pFormInfo, str, pFormInfo->availCurrentPage, TRUE, TRUE);
    Update ();
  }
}

/*==================================================================*/
/*                                                                   */
/*  FromTextAction () -                                              */
/*                                                                   */
/*===================================================================*/

static void FromTextAction (TexT t)

{
  Int2         i;
  FormInfoPtr  pFormInfo;
  Char         str [256];

  pFormInfo = (FormInfoPtr) GetObjectExtra (t);

  pFormInfo->currRangeField = E2_RANGE_FROM;

  if (TextHasNoText (pFormInfo->fromText) &&
      TextHasNoText (pFormInfo->toText))
    SafeDisable (pFormInfo->acceptButton);
  else
    SafeEnable (pFormInfo->acceptButton);

  GetTitle (t, str, sizeof (str));
  if (str [0] == '\0') {
    pFormInfo->availItem = 0;
    pFormInfo->availRow = 0;
    Reset (pFormInfo->availDoc);
    for (i = 0; i < 7; i++)
      AppendText (pFormInfo->availDoc, " \n", &availParFmt,
                  availColFmt, systemFont);
    InvalDocument (pFormInfo->availDoc);
    pFormInfo->isFromTextChanged = FALSE;
    pFormInfo->isValidFrom = FALSE;
    pFormInfo->availCurrentPage = 0;
  } else {
    pFormInfo->isFromTextChanged = TRUE;
    pFormInfo->isValidFrom = TRUE;
    if (pFormInfo->availCurrentPage > 0)
      ScrollToText (pFormInfo, str, pFormInfo->availCurrentPage, TRUE, TRUE);
    Update ();
  }
}

/*==================================================================*/
/*                                                                  */
/*  ChangeMode () -                                                 */
/*                                                                  */
/*==================================================================*/

static void ChangeMode (PopuP p)

{
  AlistDialogPtr     adp;
  Char               ch;
  Int2               i;
  EnumFieldAssocPtr  modeSet;
  ModeChoice         menuMode;
  ModeChoice         selectedMode;
  CharPtr            ptr, text;
  FormInfoPtr        pFormInfo;


  /*------------------------------------------*/
  /* Convert the menu choice into a mode type */
  /*------------------------------------------*/

  adp = (AlistDialogPtr) GetObjectExtra (p);
  pFormInfo = adp->userdata;
  modeSet = adp->alist;
  menuMode = (ModeChoice) (GetValue (p) - 1);
  selectedMode = (ModeChoice) modeSet [menuMode].value;

  pFormInfo->currMode = selectedMode;

  /* handle special modes, populate termlist */
  if (selectedMode == TAXONOMY_MODE || selectedMode == MESH_TREE_MODE) {
    SafeHide (pFormInfo->termGroup);
    SafeHide (pFormInfo->rangeGroup);
    text = SaveStringFromText (pFormInfo->termText);
    WatchCursor ();
    Update ();
    if (text != NULL) {
      ptr = text;
      ch = *ptr;
      while (ch != '\0' && ch >= ' ') {
        ptr++;
        ch = *ptr;
      }
      *ptr = '\0';
      if ((*text == '\0') || (! RepopulateTaxonomy (pFormInfo, text))) {
        RepopulateTaxonomyRoot (pFormInfo);
      }
    } else {
      RepopulateTaxonomyRoot (pFormInfo);
    }
    ArrowCursor ();
    MemFree (text);
    SafeShow (pFormInfo->taxLineagePopup);
    pFormInfo->textChanged = FALSE;
    pFormInfo->okayToAccept = TRUE;
  } else if (selectedMode == RANGE_MODE) {
    SafeHide (pFormInfo->termGroup);
    SafeHide (pFormInfo->taxLineagePopup);
    SafeShow (pFormInfo->rangeGroup);
    TextAction (pFormInfo->fromText);
    pFormInfo->availItem = 0;
    pFormInfo->availRow = 0;
    Reset (pFormInfo->availDoc);
    SetDocCache (pFormInfo->availDoc, NULL, NULL, NULL);
    for (i = 0; i < 7; i++) {
      AppendText (pFormInfo->availDoc, " \n", &availParFmt, availColFmt, systemFont);
    }
    InvalDocument (pFormInfo->availDoc);
    Select (pFormInfo->fromText);
  } else {
    SafeHide (pFormInfo->rangeGroup);
    SafeHide (pFormInfo->taxLineagePopup);
    SafeShow (pFormInfo->termGroup);
    pFormInfo->availItem = 0;
    pFormInfo->availRow = 0;
    pFormInfo->availCurrentPage = 0;
    TextAction (pFormInfo->termText);
    Reset (pFormInfo->availDoc);
    SetDocCache (pFormInfo->availDoc, NULL, NULL, NULL);
    for (i = 0; i < 7; i++) {
      AppendText (pFormInfo->availDoc, " \n", &availParFmt, availColFmt, systemFont);
    }
    InvalDocument (pFormInfo->availDoc);
    Select (pFormInfo->termText);
  }

  Update ();
}

/*==================================================================*/
/*                                                                  */
/*  ChangeMode_Callback () -                                        */
/*                                                                  */
/*==================================================================*/

static void ChangeMode_Callback (PopuP p)

{
  ChangeMode (p);
}

/*==================================================================*/
/*                                                                  */
/*  ChangeField () - Sets the current field to the one selected in  */
/*                  the given PopuP object, then selects the        */
/*                  corresponding modes PopuP that contains the     */
/*                  possible modes for this field.                  */
/*                                                                  */
/*==================================================================*/

static void ChangeField (PopuP p)

{
  AlistDialogPtr     adp;
  EnumFieldAssocPtr  fieldAList;
  Int2               fieldId;
  ModeIndex          md, mode;
  Int2               menuChoice;
  FormInfoPtr        pFormInfo;

  /*------------------------*/
  /* Get the selected field */
  /*------------------------*/

  adp = (AlistDialogPtr) GetObjectExtra (p);
  pFormInfo = adp->userdata;
  fieldAList = adp->alist;

  menuChoice = GetValue (p) - 1;
  if (menuChoice < 0) return;

  fieldId = fieldAList [menuChoice].value;

  /*--------------------------------*/
  /* Select and make active the set */
  /* of modes for this field.       */
  /*--------------------------------*/

  mode = (ModeIndex) FieldGetModePopup (pFormInfo->currDb, fieldId, fieldAList, pFormInfo);
  if (mode < 0) return;

  for (md = POPUP_MULT; md <= POPUP_UID; md++)
    if (md != mode)
      SafeHide (pFormInfo->modesPopup [md]);

  SafeShow (pFormInfo->modesPopup [mode]);
  ChangeMode (pFormInfo->modesPopup [mode]);
}

/*==================================================================*/
/*                                                                  */
/*  ChangeField_Callback () -                                       */
/*                                                                  */
/*==================================================================*/

static void ChangeField_Callback (PopuP p)

{
  ChangeField (p);
}

/*==================================================================*/
/*                                                                  */
/*  ChangeDatabase () -                                             */
/*                                                                  */
/*==================================================================*/

static void ChangeDatabase (PopuP p)

{
  FormInfoPtr     pFormInfo;
  Int2            db;
  Int2            dbase;
  UIEnum          val;
  Entrez2InfoPtr  e2ip;

  pFormInfo = (FormInfoPtr) GetObjectExtra (p);
  if (pFormInfo == NULL || pFormInfo->fieldsPopup == NULL) return;

  e2ip = Query_GetInfo ();

  val = GetValue (p) - 1;

  for (db = 0; db < e2ip->db_count; db++) {
    if (db != (Int2) val) {
      SafeHide (pFormInfo->fieldsPopup [db]);
    }
  }

  dbase = (Int2) val;
  pFormInfo->currDb = dbase;
  SafeShow (pFormInfo->fieldsPopup [dbase]);
  ChangeField (pFormInfo->fieldsPopup [dbase]);

  DoResetAvail (pFormInfo, FALSE);
  ResetChosen (pFormInfo);

  if (pFormInfo->currMode == TAXONOMY_MODE || pFormInfo->currMode == MESH_TREE_MODE) {
    SafeHide (pFormInfo->termGroup);
    SafeHide (pFormInfo->rangeGroup);
    SafeShow (pFormInfo->taxLineagePopup);
  } else if (pFormInfo->currMode == RANGE_MODE) {
    SafeHide (pFormInfo->termGroup);
    SafeHide (pFormInfo->taxLineagePopup);
    SafeShow (pFormInfo->rangeGroup);
    Select (pFormInfo->fromText);
  } else {
    SafeHide (pFormInfo->rangeGroup);
    SafeHide (pFormInfo->taxLineagePopup);
    SafeShow (pFormInfo->termGroup);
    Select (pFormInfo->termText);
  }

  Reset (pFormInfo->advQueryText);
}

/*==================================================================*/
/*                                                                  */
/*  ChangeDatabase_Callback () -                                    */
/*                                                                  */
/*==================================================================*/

static void ChangeDatabase_Callback (PopuP p)

{
  ChangeDatabase (p);
}

/*==================================================================*/
/*                                                                  */
/*  EditMessage_Callback () -                                       */
/*                                                                  */
/*==================================================================*/

static void EditMessage_Callback (ForM f, Int2 mssg)

{
  FormInfoPtr  pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (f);

  switch (mssg) {
    case VIB_MSG_CUT:
      StdCutTextProc (NULL);
      break;
    case VIB_MSG_COPY:
      StdCopyTextProc (NULL);
      break;
    case VIB_MSG_PASTE:
      StdPasteTextProc (NULL);
      break;
    case VIB_MSG_DELETE:
      StdDeleteTextProc (NULL);
      break;
    default:
      if (pFormInfo->appmessage != NULL) {
        pFormInfo->appmessage (f, mssg);
      }
      break;
  }
}

/*==================================================================*/
/*                                                                  */
/*  TermListActivate_Callback () -                                  */
/*                                                                  */
/*==================================================================*/

static void TermListActivate_Callback (WindoW w)

{
  FormInfoPtr  pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (w);

  if (pFormInfo->activate != NULL)
    pFormInfo->activate (w);
}

/*==================================================================*/
/*                                                                  */
/*  TermListCleanup_Callback () -                                   */
/*                                                                  */
/*==================================================================*/

static void TermListCleanup_Callback (GraphiC g, VoidPtr data)

{
  FormInfoPtr  pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (g);

  MemFree (pFormInfo->fieldsPopup);
  StdCleanupFormProc (g, data);
}

/*==================================================================*/
/*                                                                  */
/*  Quit_Callback () -                                              */
/*                                                                  */
/*==================================================================*/

static void Quit_Callback (IteM i)

{
  QuitProgram ();
}

/*==================================================================*/
/*                                                                  */
/*  StripNewLine () - Strip the newline character from the end of   */
/*                   a string.                                      */
/*                                                                  */
/*==================================================================*/

static void StripNewLine (CharPtr str)

{
  CharPtr  chptr;

  if (StringHasNoText (str))
    return;
  chptr = StringRChr (str, '\n');
  if (chptr != NULL) {
    *chptr = '\0';
  }
  chptr = StringRChr (str, '\r');
  if (chptr != NULL) {
    *chptr = '\0';
  }
}

/*==================================================================*/
/*                                                                  */
/*  Query_AddUidsTerm () -                                          */
/*                                                                  */
/*==================================================================*/

static Boolean Query_AddUidsTerm (ForM f, CharPtr uidListName, Int4 iUidCount, Int4Ptr uids, Int2 db)

{
  CharPtr            dbName;
  Entrez2RequestPtr  e2rq;
  Entrez2ReplyPtr    e2ry;
  StateDataPtr       sdp;
  StateDataPtr       prev;
  Char               displayStr [256];
  RecT               r;
  BaR                sb;
  FormInfoPtr        pFormInfo;
  Boolean            doNotTranslate;

  pFormInfo = (FormInfoPtr) GetObjectExtra (f);

  /*-----------------------------------------*/
  /* Make sure we don't add a Uid list twice */
  /*-----------------------------------------*/

  for (sdp = pFormInfo->termList; sdp != NULL; sdp = sdp->next)
    if (MeshStringICmp (sdp->term, uidListName) == 0) return FALSE;

  /*--------------------------------*/
  /* Create a term for the UID list */
  /*--------------------------------*/

  sdp = StateDataNew (pFormInfo);
  if (pFormInfo->termList == NULL)
    pFormInfo->termList = sdp;

  if (sdp == NULL) return FALSE;

  (pFormInfo->chosenNumLines)++;

  sdp->field = StringSave ("----");
  /* sdp->term = StringSave (uidListName); */
  sdp->count = iUidCount;
  /* sdp->uids = uids; */
  sdp->group = GROUP_SINGLE;
  sdp->above = ENTREZ_OP_NONE;

  dbName = DBGetNameFromID (pFormInfo->currDb);
  e2rq = EntrezCreateBooleanRequest (FALSE, FALSE, dbName, NULL, 0, 0, NULL, 0, 0);
  if (e2rq == NULL) return FALSE;

  doNotTranslate = FALSE;
  EntrezAddToBooleanRequest (e2rq, NULL, 0, NULL, NULL, NULL, 0, iUidCount, uids, NULL, FALSE, doNotTranslate);
  EntrezSetUseHistoryFlag (e2rq);

  if (ShowASN () == TRUE)
    DisplayEntrezRequest (e2rq);

  e2ry = SpecialEntrezSynchronousQuery (e2rq);
  if (e2ry == NULL) return FALSE;

  if (ShowASN () == TRUE)
    DisplayEntrezReply (e2ry);

  e2rq = Entrez2RequestFree (e2rq);
  sdp->key = StringSave (e2ry->key);
  sdp->term = StringSave (e2ry->key);
  Entrez2ReplyFree (e2ry);

  /*-------------------------------------*/
  /* Add the term to the current Query's */
  /* linked list of terms.               */
  /*-------------------------------------*/

  prev = sdp->prev;
  if (prev != NULL) {
    sdp->above = ENTREZ_OP_AND;
    prev->below = ENTREZ_OP_AND;
  }
  sdp->below = ENTREZ_OP_NONE;
  sdp->state = STATE_ON;

  /*---------------------------------------------*/
  /* Add the term to the query refinement window */
  /*---------------------------------------------*/

  sprintf (displayStr, "%s\t [%s]\t%ld\n", sdp->term, sdp->field, (long) iUidCount);
  AppendText (pFormInfo->chosenDoc, displayStr, &chosenParFmt, chosenColFmt, systemFont);

  /*---------------------------------*/
  /* Display our new query string in */
  /* Query Refinement window.        */
  /*---------------------------------*/

  InvalDocRows (pFormInfo->chosenDoc, pFormInfo->chosenNumLines, 1, 1);
  AdjustDocScroll (pFormInfo->chosenDoc);
  sb = GetSlateVScrollBar ((SlatE) pFormInfo->chosenDoc);
  ResetClip ();
  SetBarValue (sb, MAX (pFormInfo->chosenNumLines - 7, 0));
  ObjectRect (pFormInfo->chosenDoc, &r);
  InsetRect (&r, 4, 4);
  r.right = r.left + 16;
  InsetRect (&r, -1, -1);
  Select (pFormInfo->chosenDoc);
  InvalRect (&r);

  Update ();

  return TRUE;
}

/*==================================================================*/
/*                                                                  */
/*  ImportUIDs_Callback () -                                        */
/*                                                                  */
/*==================================================================*/

static void ImportUIDs_Callback (IteM i)

{
  Entrez2InfoPtr    e2ip;
  Entrez2DbInfoPtr  e2dbInfo;
  FILE              *fp;
  FormInfoPtr       pFormInfo;
  ByteStorePtr      bsp;
  Int4              iUidCount;
  Char              fullName [PATH_MAX];
  CharPtr           baseName;
  Char              str [32];
  Int4              uid;
  Int4Ptr           uids = NULL;
  WindoW            w;

  /*----------------------------*/
  /* Get the Term Form info for */
  /* the parent window.         */
  /*----------------------------*/

#ifdef WIN_MAC
  w = FrontWindow ();
#else
  w = ParentWindow (i);
#endif

  pFormInfo = (FormInfoPtr) GetObjectExtra (w);

  /*------------------------------*/
  /* Get the name of the UID file */
  /* and then open it.            */
  /*------------------------------*/

  if (! GetInputFileName (fullName, sizeof (fullName), "", "TEXT")) return;

  fp = FileOpen (fullName, "r");
  if (fp == NULL) return;

  /*-------------------------------*/
  /* Create a ByteStore Pointer to */
  /* store the UIDs in.            */
  /*-------------------------------*/

  bsp = BSNew (128);
  if (bsp == NULL) {
    FileClose (fp);
    return;
  }

  /*-------------------------------------*/
  /* Read the first line of the file and */
  /* verify that it contains a valid     */
  /* database name.                      */
  /*                                     */
  /* NOTE - Special case for medline     */
  /*        needs to me removed once     */
  /*        it is fixed on the server.   */
  /*                                     */
  /*-------------------------------------*/

  if (!FileGets (str, sizeof (str), fp)) {
    BSFree (bsp);
    FileClose (fp);
    return;
  }

  StripNewLine (str);

  e2ip = Query_GetInfo ();

  for (e2dbInfo = e2ip->db_info; e2dbInfo != NULL; e2dbInfo = e2dbInfo->next)
    if (StringICmp (e2dbInfo->db_menu, &str [1]) == 0) break;

  if (e2dbInfo == NULL) {
    Message (MSG_POSTERR, "First line must be in the form >DbName");
    BSFree (bsp);
    FileClose (fp);
    return;
  }

  /*-----------------------------------*/
  /* Read the rest of the lines in and */
  /* add each UID to the byteStore.    */
  /*-----------------------------------*/

  while (FileGets (str, sizeof (str), fp)) {
    StripNewLine (str);
    if (str [0] != '\0' && StrToLong (str, &uid))
      BSWrite (bsp, &uid, sizeof (DocUid));
  }

  BSSeek (bsp, 0L, 0);
  iUidCount = (Int4) ((BSLen (bsp)) / sizeof (Int4));
  uids = (Int4Ptr) BSMerge (bsp, NULL);

  /*--------------------------------*/
  /* Add the list of UIDs as a term */
  /* in the current query.          */
  /*--------------------------------*/

  baseName = FileNameFind (fullName);
  Query_AddUidsTerm (pFormInfo->form, baseName, iUidCount, uids, pFormInfo->currDb);
  Select (pFormInfo->termText);

  /*---------------------------*/
  /* Get a count of terms that */
  /* satisfy this query.       */
  /*---------------------------*/

  Update ();
  RecalculateChosen (pFormInfo);
  ArrowCursor ();
  Update ();

  /*----------------------------------*/
  /* Clean up and return successfully */
  /*----------------------------------*/

  BSFree (bsp);
  MemFree (uids);
  FileClose (fp);
}

/*==================================================================*/
/*                                                                  */
/*  ExportUIDs_Callback () -                                        */
/*                                                                  */
/*==================================================================*/

static void ExportUIDs_Callback (IteM i)

{
  CharPtr                 dbName;
  Entrez2BooleanReplyPtr  e2BooleanPtr;
  Entrez2IdListPtr        e2UidList;
  FILE                    *fp;
  FormInfoPtr             pFormInfo;
  Char                    path [PATH_MAX];
  Char                    headerStr [32];
  Int4Ptr                 uids;
  Int4                    uidNum;
  Char                    uidStr [32];
  WindoW                  w;

  /*----------------------------*/
  /* Get the Term Form info for */
  /* the parent window.         */
  /*----------------------------*/

#ifdef WIN_MAC
  w = FrontWindow ();
#else
  w = ParentWindow (i);
#endif

  pFormInfo = (FormInfoPtr) GetObjectExtra (w);

  /*-------------------------------*/
  /* Get a file name from the user */
  /* and open that file.           */
  /*-------------------------------*/

  if (! GetOutputFileName (path, sizeof (path), NULL)) return;

#ifdef WIN_MAC
  FileCreate (path, "TEXT", "ttxt");
#endif

  fp = FileOpen (path, "w");
  if (fp == NULL) return;

  WatchCursor ();

  /*-----------------------------------*/
  /* Write the header line to the file */
  /*-----------------------------------*/

  dbName = DBGetNameFromID (pFormInfo->currDb);
  sprintf (headerStr, ">%s\n", dbName);
  FilePuts (headerStr, fp);

  /*-----------------------*/
  /* Create a list of UIDS */
  /*-----------------------*/

  e2BooleanPtr = Query_FetchUIDs (pFormInfo->form);
  e2UidList = e2BooleanPtr->uids;

  uids = (Int4Ptr) BSMerge (e2UidList->uids, NULL);

  /*----------------------------*/
  /* Write the uids to the file */
  /*----------------------------*/

  for (uidNum = 0; uidNum < e2UidList->num; uidNum++) {
    sprintf (uidStr, "%ld\n", (long) uids [uidNum]);
    FilePuts (uidStr, fp);
  }

  /*----------------------------------*/
  /* Clean up and return successfully */
  /*----------------------------------*/

  FileClose (fp);
  free (uids);
  ArrowCursor ();
  Entrez2BooleanReplyFree (e2BooleanPtr);
}

/*==================================================================*/
/*                                                                  */
/*  AdvQueryInit () - Initialize the lexical static variables for   */
/*                     parsing a new string.                        */
/*                                                                  */
/*==================================================================*/

static Int2 AdvQueryInit (FormInfoPtr pFormInfo, CharPtr newStr)

{
  /*-----------------*/
  /* Check parameter */
  /*-----------------*/

  if (newStr == NULL) return -1;

  /*-----------------------------------------*/
  /* Initialize the lexical static variables */
  /* using the new string.                   */
  /*-----------------------------------------*/

  MemFree (pFormInfo->advQueryLexStr);
  pFormInfo->advQueryLexStr = StringSave (newStr);
  pFormInfo->advQueryLexPos = 0;
  pFormInfo->advQueryLexState = LEXSTATE_IDLE;

  /*-------------------------------------*/
  /* Free the linked list of query terms */
  /* so that we can build a new one with */
  /* the given string.                   */
  /*-------------------------------------*/

  ResetChosen (pFormInfo);

  /*---------------------*/
  /* Return successfully */
  /*---------------------*/

  return 0;
}

/*==================================================================*/
/*                                                                  */
/*  LexClassifyChar () -                                            */
/*                                                                  */
/*     NOTE -- Taken from LexClassifyChar() in accutils.c           */
/*                                                                  */
/*==================================================================*/

static Int2 LexClassifyChar (Char c)

{
  Int2  retval;

  switch (c) {
    case '(':
      retval = LEXCHAR_LPAREN;
      break;
    case ')':
      retval = LEXCHAR_RPAREN;
      break;
    case '[':
      retval = LEXCHAR_LBRACKET;
      break;
    case ']':
      retval = LEXCHAR_RBRACKET;
      break;
    case '"':
      retval = LEXCHAR_QUOTE;
      break;
    case '&':
      retval = LEXCHAR_AND;
      break;
    case '|':
      retval = LEXCHAR_OR;
      break;
    case '-':
      retval = LEXCHAR_NOT;
      break;
    case ',':
      retval = LEXCHAR_COMMA;
      break;
    case '@':
      retval = LEXCHAR_ATSIGN;
      break;
    case '\\':
      retval = LEXCHAR_BACKSLASH;
      break;
    case ' ':
    case '\t':
      retval = LEXCHAR_WHITESPACE;
      break;
    case ';':
      retval = LEXCHAR_SEMICOLON;
      break;
    case ':':
      retval = LEXCHAR_COLON;
      break;
    case '\0':
      retval = LEXCHAR_NULL;
      break;
    case '\r':
    case '\n':
      retval = LEXCHAR_EOL;
      break;
    default:
      retval = LEXCHAR_OTHER;
      break;
  }

  return retval;
}

/*==================================================================*/
/*                                                                  */
/*  AdvQueryGetNextToken() -                                        */
/*                                                                  */
/*        Returns:                                                  */
/*                                                                  */
/*            StartPos - >0 : The position in the static string     */
/*                            pFormInfo->advQueryLexStr where the   */
/*                            token starts.                         */
/*                                                                  */
/*                       -1 : No token found.                       */
/*                                                                  */
/*        Sets:                                                     */
/*                                                                  */
/*            pFormInfo->advQueryNextToken : Contains an            */
/*                            identifier specifying the type of     */
/*                            the token, and if the token is a      */
/*                            string also contains a pointer to     */
/*                            the string.                           */
/*                                                                  */
/*                            Set to NULL if no token found.        */
/*                                                                  */
/*==================================================================*/

static Int2 AdvQueryGetNextToken (FormInfoPtr pFormInfo)

{
  Int2            startPos;
  Int2            classChar;
  Int2            token = 0;
  Boolean         done;
  Char            c;
  CharPtr         lexToken = NULL;
  CharPtr         lexTokenStart;
  Int2            len;
  Int2            pos;

  /*------------------*/
  /* Check conditions */
  /*------------------*/

  if (pFormInfo->advQueryLexStr == NULL) {
    pFormInfo->advQueryNextToken = NULL;
    return -1;
  }

  pFormInfo->advQueryNextToken = &pFormInfo->advQueryNextRealNode;

  /*--------------------------------------*/
  /* Find the beginning of the next token */
  /* and malloc space for it.             */
  /*--------------------------------------*/

  len = StringLen (pFormInfo->advQueryLexStr);
  startPos = pFormInfo->advQueryLexPos;

  if (pFormInfo->advQueryLexPos >= len) {
    pFormInfo->advQueryLexState = LEXSTATE_ERROR;
    token = -1;
    lexToken = MemNew (1);
  } else {
    pos = pFormInfo->advQueryLexPos;
    lexToken = MemNew (StringLen (&pFormInfo->advQueryLexStr [pos]) + 1);
  }

  lexTokenStart = lexToken;

  /*------------------*/
  /*------------------*/

  for (done = FALSE; !done && pFormInfo->advQueryLexPos <= len; pFormInfo->advQueryLexPos++) {
    c = pFormInfo->advQueryLexStr [pFormInfo->advQueryLexPos];
    classChar = LexClassifyChar (c);
    switch (pFormInfo->advQueryLexState) {
      case LEXSTATE_IDLE:
        switch (classChar) {
          case LEXCHAR_LPAREN:
            token = LEXTOK_LPAREN;
            done = TRUE;
            break;
          case LEXCHAR_RPAREN:
            token = LEXTOK_RPAREN;
            done = TRUE;
            break;
          case LEXCHAR_LBRACKET:
            token = LEXTOK_LBRACKET;
            done = TRUE;
            break;
          case LEXCHAR_RBRACKET:
            token = LEXTOK_RBRACKET;
            done = TRUE;
            break;
          case LEXCHAR_AND:
            token = LEXTOK_AND;
            done = TRUE;
            break;
          case LEXCHAR_OR:
            token = LEXTOK_OR;
            done = TRUE;
            break;
          case LEXCHAR_NOT:
            token = LEXTOK_NOT;
            done = TRUE;
            break;
          case LEXCHAR_COMMA:
            token = LEXTOK_COMMA;
            done = TRUE;
            break;
          case LEXCHAR_ATSIGN:
            token = LEXTOK_ATSIGN;
            done = TRUE;
            break;
          case LEXCHAR_COLON:
            token = LEXTOK_RANGE;
            done = TRUE;
            break;
          case LEXCHAR_QUOTE:
            pFormInfo->advQueryLexState = LEXSTATE_INQUOTE;
            break;
          case LEXCHAR_BACKSLASH:
            pFormInfo->advQueryLexState = LEXSTATE_BACKSLASHED;
            break;
          case LEXCHAR_EOL:
          case LEXCHAR_WHITESPACE:
            startPos = pFormInfo->advQueryLexPos + 1;
            break;
          case LEXCHAR_SEMICOLON:
          case LEXCHAR_NULL:
            pFormInfo->advQueryLexState = LEXSTATE_ERROR;
            done = TRUE;
            break;
          case LEXCHAR_OTHER:
          default:
            pFormInfo->advQueryLexState = LEXSTATE_INSTRING;
            *lexToken++ = c;
            break;
        }
        break;
      case LEXSTATE_BACKSLASHED:
        switch (classChar) {
          case LEXCHAR_NULL:
          case LEXCHAR_EOL:
            *lexToken++ = '\0';
            done = TRUE;
            pFormInfo->advQueryLexState = LEXSTATE_IDLE;
            break;
          case LEXCHAR_LPAREN:
          case LEXCHAR_RPAREN:
          case LEXCHAR_LBRACKET:
          case LEXCHAR_RBRACKET:
          case LEXCHAR_QUOTE:
          case LEXCHAR_AND:
          case LEXCHAR_OR:
          case LEXCHAR_NOT:
          case LEXCHAR_COMMA:
          case LEXCHAR_ATSIGN:
          case LEXCHAR_BACKSLASH:
          case LEXCHAR_WHITESPACE:
          case LEXCHAR_SEMICOLON:
          case LEXCHAR_COLON:
          case LEXCHAR_OTHER:
          default:
            pFormInfo->advQueryLexState = LEXSTATE_INSTRING;
            *lexToken++ = c;
            break;
        }
        break;
      case LEXSTATE_INQUOTE:
        switch (classChar) {
          case LEXCHAR_QUOTE:
            token = LEXTOK_STRING;
            *lexToken++ = '\0';
            done = TRUE;
            pFormInfo->advQueryLexState = LEXSTATE_IDLE;
            break;
          case LEXCHAR_BACKSLASH:
            pFormInfo->advQueryLexState = LEXSTATE_INQUOTE_AFTERBSLASH;
            break;
          case LEXCHAR_NULL:
          case LEXCHAR_EOL:
            pFormInfo->advQueryLexState = LEXSTATE_ERROR;
            done = TRUE;
            break;
          default:
            *lexToken++ = c;
            break;
        }
        break;
      case LEXSTATE_INQUOTE_AFTERBSLASH:
        switch (classChar) {
          case LEXCHAR_NULL:
          case LEXCHAR_EOL:
            pFormInfo->advQueryLexState = LEXSTATE_ERROR;
            done = TRUE;
            break;
          default:
            pFormInfo->advQueryLexState = LEXSTATE_INQUOTE;
            *lexToken++ = c;
            break;
        }
        break;
      case LEXSTATE_INSTRING:
        switch (classChar) {
          case LEXCHAR_WHITESPACE:
          case LEXCHAR_SEMICOLON:
          case LEXCHAR_NULL:
          case LEXCHAR_EOL:
            token = LEXTOK_STRING;
            *lexToken++ = '\0';
            done = TRUE;
            pFormInfo->advQueryLexState = LEXSTATE_IDLE;
            break;
          case LEXCHAR_BACKSLASH:
            pFormInfo->advQueryLexState = LEXSTATE_BACKSLASHED;
            break;
          case LEXCHAR_QUOTE:
            pFormInfo->advQueryLexState = LEXSTATE_INQUOTE;
            break;
          case LEXCHAR_OTHER:
            *lexToken++ = c;
            break;
          default:
            token = LEXTOK_STRING;
            *lexToken++ = '\0';
            done = TRUE;
            pFormInfo->advQueryLexState = LEXSTATE_IDLE;
            pFormInfo->advQueryLexPos--;        /* push back last character */
            break;
        }
        break;
      case LEXSTATE_ERROR:
        done = TRUE;
        break;
    }
  }

  pFormInfo->advQueryNextToken->choice = (Uint1) token;
  pFormInfo->advQueryNextToken->data.ptrvalue = NULL;

  if (token == LEXTOK_STRING)
    pFormInfo->advQueryNextToken->data.ptrvalue = lexTokenStart;
  else
    MemFree (lexTokenStart);

  if (pFormInfo->advQueryLexState == LEXSTATE_ERROR) {
    pFormInfo->advQueryNextToken = NULL;
    return -1;
  } else
    return startPos;
}

/*===================================================================*/
/*                                                                   */
/*  CloseGroup () -                                                  */
/*                                                                   */
/*===================================================================*/

static Boolean CloseGroup (FormInfoPtr pFormInfo)

{
  StateDataPtr  pLastTerm;

  /*--------------*/
  /* Sanity check */
  /*--------------*/

  if (pFormInfo == NULL || pFormInfo->termList == NULL) return FALSE;

  /*--------------------------------*/
  /* Find the last term in the list */
  /*--------------------------------*/

  pLastTerm = pFormInfo->termList;
  while (pLastTerm->next != NULL)
    pLastTerm = pLastTerm->next;

  /*----------------------*/
  /* Set its group status */
  /*----------------------*/

  switch (pLastTerm->group) {
    case GROUP_SINGLE:
    case GROUP_LAST:
      break;
    case GROUP_FIRST:
      pLastTerm->group = GROUP_SINGLE;
      break;
    case GROUP_MIDDLE:
      pLastTerm->group = GROUP_LAST;
      break;
    default:
      break;
  }

  /*---------------------*/
  /* Return successfully */
  /*---------------------*/

  return TRUE;
}

/*===================================================================*/
/*                                                                   */
/*  ParseANDedTerms () -                                             */
/*                                                                   */
/*     NOTE -- Brought over from accutils.c                          */
/*                                                                   */
/*===================================================================*/

/* prototype for recursive function */

static Boolean ParseTermList (FormInfoPtr pFormInfo, Int2 db, Int2 fld, Int2 currOp);

static Boolean ParseANDedTerms (FormInfoPtr pFormInfo, Int2 db, Int2 fld, Int2 currOp)

{
  if (! ParseTermList (pFormInfo, db, fld, currOp)) return FALSE;

  while (pFormInfo->advQueryNextToken != NULL && pFormInfo->advQueryNextToken->choice == LEXTOK_AND) {
    AdvQueryGetNextToken (pFormInfo);
    if (! ParseTermList (pFormInfo, db, fld, LEXCHAR_AND)) return FALSE;
  }

  return TRUE;
}

/*===================================================================*/
/*                                                                   */
/*  ParseORedTerms () -                                              */
/*                                                                   */
/*     NOTE -- Brought over from accutils.c                          */
/*                                                                   */
/*===================================================================*/

static Boolean ParseORedTerms (FormInfoPtr pFormInfo, Int2 db, Int2 fld, Int2 currOp)

{
  if (! ParseANDedTerms (pFormInfo, db, fld, currOp)) return FALSE;

  while (pFormInfo->advQueryNextToken != NULL && pFormInfo->advQueryNextToken->choice == LEXTOK_OR) {
    AdvQueryGetNextToken (pFormInfo);
    if (! ParseANDedTerms (pFormInfo, db, fld, LEXCHAR_OR)) return FALSE;
  }

  return TRUE;
}

/*===================================================================*/
/*                                                                   */
/*  ParseNOTedTerms () -                                             */
/*                                                                   */
/*     NOTE -- This function brought over from accutils.c            */
/*                                                                   */
/*===================================================================*/

static Boolean ParseNOTedTerms (FormInfoPtr pFormInfo, Int2 db, Int2 fld, Int2 currOp)

{
  if (! ParseORedTerms (pFormInfo, db, fld, currOp)) return FALSE;

  while ((pFormInfo->advQueryNextToken != NULL) && (pFormInfo->advQueryNextToken->choice == LEXTOK_NOT)) {
    AdvQueryGetNextToken (pFormInfo);
    if (! ParseORedTerms (pFormInfo, db, fld, LEXCHAR_NOT)) return FALSE;
  }

  return TRUE;
}

/*===================================================================*/
/*                                                                   */
/*  ParseTermList () -                                               */
/*                                                                   */
/*===================================================================*/

static Boolean ParseTermList (FormInfoPtr pFormInfo, Int2 db, Int2 fld, Int2 currOp)

{
  CharPtr       term;
  CharPtr       fldStr;
  CharPtr       highRangeStr = NULL;
  StateDataPtr  newTermPtr;
  Int2          nOperator;

  /*---------------------------------------------*/
  /* Check for proper conditions to run function */
  /*---------------------------------------------*/

  if (pFormInfo->advQueryNextToken == NULL) {
    ErrPostEx (SEV_WARNING, ERR_CD_LEX, 0, "Null factor");
    return FALSE;
  }

  /*------------------------------------------*/
  /* If we have a left paren, then recursivly */
  /* parse what follows it as a term list.    */
  /*------------------------------------------*/

  if (pFormInfo->advQueryNextToken->choice == LEXTOK_LPAREN) {
    pFormInfo->advQueryNewGroup = TRUE;
    AdvQueryGetNextToken (pFormInfo);
    if (! ParseNOTedTerms (pFormInfo, db, fld, currOp))
      return FALSE;

      /*---------------------------*/
    /* If we have a right paren, */
    /* close out group.          */
      /*---------------------------*/

    if (pFormInfo->advQueryNextToken != NULL && pFormInfo->advQueryNextToken->choice == LEXTOK_RPAREN) {
      CloseGroup (pFormInfo);
      AdvQueryGetNextToken (pFormInfo);
    } else {
      ErrPostEx (SEV_WARNING, ERR_CD_LEX, 0, "Missing right paren");
      return FALSE;
    }

    return TRUE;
  }

  /*--------------------------*/
  /* Otherwise, grab the term */
  /*--------------------------*/

  if (pFormInfo->advQueryNextToken->choice != LEXTOK_STRING) {
    ErrPostEx (SEV_WARNING, ERR_CD_LEX, 0, "invalid token");
    return FALSE;
  }

  term = (CharPtr) (pFormInfo->advQueryNextToken->data.ptrvalue);
  AdvQueryGetNextToken (pFormInfo);

  /*---------------------------------------*/
  /* If it's a range, grab the second term */
  /*---------------------------------------*/

  if ((pFormInfo->advQueryNextToken != NULL) && (pFormInfo->advQueryNextToken->choice == LEXTOK_RANGE)) {
    AdvQueryGetNextToken (pFormInfo);
    if ((pFormInfo->advQueryNextToken == NULL) || (pFormInfo->advQueryNextToken->choice != LEXTOK_STRING)) {
      ErrPostEx (SEV_WARNING, ERR_CD_LEX, 0, "missing second half of range");
      MemFree (term);
      return FALSE;
    }
    highRangeStr = (CharPtr) (pFormInfo->advQueryNextToken->data.ptrvalue);
    AdvQueryGetNextToken (pFormInfo);
  }

  /*----------------------*/
  /* Parse the field name */
  /*----------------------*/

  if ((pFormInfo->advQueryNextToken != NULL) && (pFormInfo->advQueryNextToken->choice == LEXTOK_LBRACKET)) {

      /*----------------------*/
    /* Get the field string */
      /*----------------------*/

    AdvQueryGetNextToken (pFormInfo);
    if ((pFormInfo->advQueryNextToken == NULL) || (pFormInfo->advQueryNextToken->choice != LEXTOK_STRING)) {
      ErrPostEx (SEV_WARNING, ERR_CD_LEX, 0, "missing field id after bracket");
      MemFree (term);
      MemFree (highRangeStr);
      return FALSE;
    }

    fldStr = (CharPtr) (pFormInfo->advQueryNextToken->data.ptrvalue);

      /*-------------------------------------*/
    /* Convert string to field ID, mapping */
    /* wildcard '*' to 'ALL'.              */
      /*-------------------------------------*/

    if (fldStr != NULL && StrCmp (fldStr, "*") == 0)
      fld = FieldGetIDFromName (db, "ALL");
    else
      fld = FieldGetIDFromName (db, fldStr);

    MemFree (pFormInfo->advQueryNextToken->data.ptrvalue);
    if (fld < 0) {
      ErrPostEx (SEV_WARNING, ERR_CD_LEX, 0, "bad field identifier");
      MemFree (term);
      MemFree (highRangeStr);
      return FALSE;
    }

      /*----------------------------------*/
    /* Check to see if the fields are a */
    /* comma-seperated range.           */
      /*----------------------------------*/

    AdvQueryGetNextToken (pFormInfo);
    if ((pFormInfo->advQueryNextToken == NULL) ||
        (pFormInfo->advQueryNextToken->choice != LEXTOK_COMMA && pFormInfo->advQueryNextToken->choice != LEXTOK_RBRACKET)) {
      ErrPostEx (SEV_WARNING, ERR_CD_LEX, 0, "missing right bracket");
      MemFree (term);
      MemFree (highRangeStr);
      return FALSE;
    }
    if (pFormInfo->advQueryNextToken->choice == LEXTOK_COMMA) {
      AdvQueryGetNextToken (pFormInfo);
      if ((pFormInfo->advQueryNextToken == NULL) ||
          (pFormInfo->advQueryNextToken->choice != LEXTOK_STRING) || StringCmp (pFormInfo->advQueryNextToken->data.ptrvalue, "S") != 0) {
        ErrPostEx (SEV_WARNING, ERR_CD_LEX, 0, "field qualifier error");
        MemFree (term);
        MemFree (highRangeStr);
        return FALSE;
      }
      MemFree (pFormInfo->advQueryNextToken->data.ptrvalue);
      AdvQueryGetNextToken (pFormInfo);
      if ((pFormInfo->advQueryNextToken == NULL) || (pFormInfo->advQueryNextToken->choice != LEXTOK_RBRACKET)) {
        ErrPostEx (SEV_WARNING, ERR_CD_LEX, 0, "missing right bracket");
        MemFree (term);
        MemFree (highRangeStr);
        return FALSE;
      }
    }
    AdvQueryGetNextToken (pFormInfo);
  }

  /*-------------------------------------*/
  /* Add in the high range, if it exists */
  /*-------------------------------------*/

  if (highRangeStr != NULL) {
    StringCat (term, ":");
    StringCat (term, highRangeStr);
  }

  /*-----------------------------------------------*/
  /* Create a new term and add it to the term list */
  /*-----------------------------------------------*/

  newTermPtr = Query_AddBoolTerm (pFormInfo->form, db, fld, term, STATE_ON, 0);

  if (newTermPtr == NULL) return FALSE;

  /*---------------------------------------*/
  /* Select the proper operator with which */
  /* to connect it to the previous term.   */
  /*---------------------------------------*/

  switch (currOp) {
    case LEXCHAR_OR:
      nOperator = ENTREZ_OP_OR;
      break;
    case LEXCHAR_AND:
      nOperator = ENTREZ_OP_AND;
      break;
    case LEXCHAR_NOT:
      nOperator = ENTREZ_OP_BUTNOT;
      break;
    default:
      nOperator = ENTREZ_OP_NONE;
      break;
  }
  newTermPtr->above = nOperator;
  if (newTermPtr->prev != NULL)
    newTermPtr->prev->below = nOperator;

  /*-------------------------------*/
  /* Set its group status properly */
  /*-------------------------------*/

  if (pFormInfo->advQueryNewGroup == TRUE) {
    newTermPtr->group = GROUP_FIRST;
    pFormInfo->advQueryNewGroup = FALSE;
  } else {
    switch (newTermPtr->prev->group) {
      case GROUP_SINGLE:
        newTermPtr->group = GROUP_SINGLE;
        break;
      case GROUP_FIRST:
      case GROUP_MIDDLE:
        newTermPtr->group = GROUP_MIDDLE;
        break;
      default:
        newTermPtr->group = GROUP_SINGLE;
        break;
    }
  }


/*----------------------------------*/
/* Clean up and return successfully */
/*----------------------------------*/

  MemFree (term);
  MemFree (highRangeStr);

  return TRUE;
}

/*===================================================================*/
/*                                                                   */
/*  Query_ParseString () -                                           */
/*                                                                   */
/*     NOTE -- This function is based on EntrezPMTLParseString()     */
/*             from accutil.c                                        */
/*                                                                   */
/*===================================================================*/

static Boolean Query_ParseString (ForM f, CharPtr str, Int2 db, Int2 fld)

{
  Boolean      retval;
  FormInfoPtr  pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (f);

  /*---------------------------*/
  /* Check parameters, setting */
  /* defaults if necessary.    */
  /*---------------------------*/

  if (str == NULL || *str == '\0') return FALSE;

  /*------------------------------------*/
  /* Clear out the existing query terms */
  /*------------------------------------*/

  AdvQueryInit (pFormInfo, str);

  /*------------------------------------*/
  /* Recursively parse the query string */
  /*------------------------------------*/

  AdvQueryGetNextToken (pFormInfo);

  if ((ParseNOTedTerms (pFormInfo, db, fld, 0) == TRUE) && (pFormInfo->advQueryNextToken == NULL))
    retval = TRUE;
  else {
    ResetChosen (pFormInfo);
    retval = FALSE;
  }

  /*---------------------*/
  /* Return successfully */
  /*---------------------*/

  return retval;
}

/*==================================================================*/
/*                                                                  */
/*  AdvancedQueryText_Callback () -                                 */
/*                                                                  */
/*==================================================================*/

static void AdvancedQueryText_Callback (TexT queryTextBox)

{
  CharPtr      curstr;
  FormInfoPtr  pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (queryTextBox);

  curstr = SaveStringFromText (pFormInfo->advQueryText);

  if (Query_ParseString (pFormInfo->form, curstr, pFormInfo->currDb, -1)) {

    SafeEnable (pFormInfo->advRetrieveButton);
    pFormInfo->advQueryState = ADV_QUERY_EVALUATE_STATE;
    SafeSetTitle (pFormInfo->advRetrieveButton, "Evaluate");
  } else {
    SafeDisable (pFormInfo->advRetrieveButton);
    pFormInfo->advQueryState = ADV_QUERY_INVALID_STATE;
  }

  MemFree (curstr);
}

/*==================================================================*/
/*                                                                  */
/*  SetAdvancedText () -                                            */
/*                                                                  */
/*==================================================================*/

static void SetAdvancedText (TexT advancedText, CharPtr str)

{
  SetTitle (advancedText, str);
}

/*==================================================================*/
/*                                                                  */
/*  RepopulateChosen () -                                           */
/*                                                                  */
/*==================================================================*/

static void RepopulateChosen (FormInfoPtr pFormInfo)

{
  Int4          off;
  BaR           sb;
  StateDataPtr  sdp;
  Char          strn [256];

  sb = GetSlateVScrollBar ((SlatE) pFormInfo->chosenDoc);
  off = GetBarValue (sb);
  Reset (pFormInfo->chosenDoc);

  InvalDocument (pFormInfo->chosenDoc);
  pFormInfo->chosenNumLines = 0;
  for (sdp = pFormInfo->termList; sdp != NULL; sdp = sdp->next) {
    (pFormInfo->chosenNumLines)++;
    if (sdp->field == NULL)
      sprintf (strn, "%s\t [----]\t%ld\n", sdp->term, (long) sdp->count);
    else
      sprintf (strn, "%s\t [%s]\t%ld\n", sdp->term, sdp->field, (long) sdp->count);
    AppendText (pFormInfo->chosenDoc, strn, &chosenParFmt, chosenColFmt, systemFont);
  }
  InvalDocument (pFormInfo->chosenDoc);
  AdjustDocScroll (pFormInfo->chosenDoc);
  CorrectBarValue (sb, off);
  Update ();
}

/*==================================================================*/
/*                                                                  */
/*  Query_ConvertToString () - Convert the current linked list of   */
/*                            terms into a string that can be       */
/*                            displayed in the advanced query win.  */
/*                                                                  */
/*==================================================================*/

static CharPtr Query_ConvertToString (ForM f)

{
  Entrez2FieldInfoPtr  fieldInfo;
  Int2                 group;
  Int2                 last;
  CharPtr              ptr;
  StateDataPtr         sdp;
  CharPtr              tmp;
  CharPtr              advQueryString;
  FormInfoPtr          pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (f);

  if (pFormInfo->termList == NULL) return NULL;

  if (pFormInfo->chosenNumLines < 1) return NULL;

  advQueryString = MemNew (1000);
  advQueryString [0] = '\0';
  group = 0;
  last = 0;
  for (sdp = pFormInfo->termList; sdp != NULL; sdp = sdp->next) {
    if (sdp->group == GROUP_SINGLE || sdp->group == GROUP_FIRST)
      group++;

    if (sdp->state == STATE_ON) {
      if (last == 0) {
        StrCat (advQueryString, "( ");
        StrCat (advQueryString, "( ");
      } else if (last == group) {
        StrCat (advQueryString, " | ");
      } else if (sdp->above == ENTREZ_OP_BUTNOT) {
        StrCat (advQueryString, " )");
        StrCat (advQueryString, " )");
        StrCat (advQueryString, " - ");
        StrCat (advQueryString, "( ");
        StrCat (advQueryString, "( ");
      } else {
        StrCat (advQueryString, " )");
        StrCat (advQueryString, " & ");
        StrCat (advQueryString, "( ");
      }
      StringCat (advQueryString, "\"");
      tmp = StringSave (sdp->term);
      ptr = StringChr (tmp, ':');
      if (ptr != NULL) {
        *ptr = '\0';
        ptr++;
        TrimSpacesAroundString (tmp);
        TrimSpacesAroundString (ptr);
        StringCat (advQueryString, tmp);
        StringCat (advQueryString, "\" : \"");
        StringCat (advQueryString, ptr);
      } else
        StringCat (advQueryString, tmp);
      MemFree (tmp);
      StringCat (advQueryString, "\"[");
      fieldInfo = FieldGetInfo (sdp->db, sdp->fld);
      StringCat (advQueryString, fieldInfo->field_name);
      StringCat (advQueryString, "]");
      last = group;
    }
  }

  if (group > 0 && last > 0) {
    StrCat (advQueryString, " )");
    StrCat (advQueryString, " )");
  }

  return advQueryString;
}

/*==================================================================*/
/*                                                                  */
/*  AdvancedQueryToggle_Callback () - Called when the "Advanced     */
/*                                   Queries" menu option is        */
/*                                   selected.                      */
/*                                                                  */
/*==================================================================*/

static void AdvancedQueryToggle_Callback (IteM i)

{
  FormInfoPtr  pFormInfo;
  CharPtr      str;

  pFormInfo = (FormInfoPtr) GetObjectExtra (i);

  /*-----------------------*/
  /* Advanced ==> Standard */
  /*-----------------------*/

  if (! GetStatus (pFormInfo->advancedQueryItem)) {
    SafeHide (pFormInfo->advQueryGroup);
    Reset (pFormInfo->advQueryText);
    RepopulateChosen (pFormInfo);
    SafeShow (pFormInfo->stdQueryGroup);
  }

  /*-----------------------*/
  /* Standard ==> Advanced */
  /*-----------------------*/

  else {
    str = Query_ConvertToString (pFormInfo->form);
    SetAdvancedText (pFormInfo->advQueryText, str);
    MemFree (str);
    SafeHide (pFormInfo->stdQueryGroup);
    SafeShow (pFormInfo->advQueryGroup);
  }
}

/*==================================================================*/
/*                                                                  */
/*  ExplodeTermsToggle_Callback () - Called when the "Explode       */
/*                                   Terms" menu option is          */
/*                                   selected.                      */
/*                                                                  */
/*==================================================================*/

static void ExplodeTermsToggle_Callback (IteM i)

{
  FormInfoPtr  pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (i);
  if (pFormInfo == NULL) return;

  WatchCursor ();
  Update ();
  RecalculateChosen (pFormInfo);
  ArrowCursor ();
  Update ();
}

/*===================================================================*/
/*                                                                   */
/*  TermList_UnselectAll () -- Marks all terms in the term list as   */
/*                            unselected.                            */
/*                                                                   */
/*===================================================================*/

NLM_EXTERN Boolean TermList_UnselectAll (ForM f)

{
  Int2          count = 0;
  Int2          item;
  FormInfoPtr   pFormInfo;
  StateDataPtr  sdp;

  pFormInfo = (FormInfoPtr) GetObjectExtra (f);

  sdp = pFormInfo->termList;
  while (sdp != NULL) {
    sdp->state = STATE_OFF;
    sdp = sdp->next;
    count++;
  }

  ResetClip ();
  WatchCursor ();
  for (item = 1; item <= count; item++) {
    InvalDocCols (pFormInfo->chosenDoc, item, 3, 3);
  }
  Update ();
  RecalculateChosen (pFormInfo);
  ArrowCursor ();

  return TRUE;
}

/*==================================================================*/
/*                                                                  */
/*  AlphabetizeGroups () -                                          */
/*                                                                  */
/*==================================================================*/

static void AlphabetizeGroups (FormInfoPtr pFormInfo)

{
  Int2          compare;
  Boolean       keepGoing;
  StateDataPtr  next;
  StateDataPtr  sdp;
  StateData     tmp;

  if (pFormInfo->termList == NULL) return;

  keepGoing = TRUE;
  while (keepGoing) {
    keepGoing = FALSE;
    for (sdp = pFormInfo->termList; sdp->next != NULL; sdp = sdp->next) {
      next = sdp->next;
      if (sdp->group == GROUP_FIRST || sdp->group == GROUP_MIDDLE) {
        compare = MeshStringICmp (sdp->term, next->term);
        if (compare > 0) {
          tmp.term = next->term;
          tmp.field = next->field;
          tmp.count = next->count;
          tmp.db = next->db;
          tmp.fld = next->fld;
          next->term = sdp->term;
          next->field = sdp->field;
          next->count = sdp->count;
          next->db = sdp->db;
          next->fld = sdp->fld;
          sdp->term = tmp.term;
          sdp->field = tmp.field;
          sdp->count = tmp.count;
          sdp->db = tmp.db;
          sdp->fld = tmp.fld;
          keepGoing = TRUE;
        }
      }
    }
  }
}

/*==================================================================*/
/*                                                                  */
/*  Query_ClearUnusedTerms () - Remove any terms that have been     */
/*                              turned off.                         */
/*                                                                  */
/*==================================================================*/

static void Query_ClearUnusedTerms (IteM i)

{
  StateDataPtr       next;
  StateDataPtr       sdp;
  FormInfoPtr        pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (i);

  WatchCursor ();
  Update ();

  /* Iterate through the list, removing all */
  /* terms that are marked as unused.       */

  sdp = pFormInfo->termList;

  while (sdp != NULL) {
    next = sdp->next;
    if (sdp->state == STATE_OFF) {
      RemoveTermFromList (pFormInfo, sdp, FALSE, FALSE);
      FreeTerm (sdp);
    }
    sdp = next;
  }

  /* Adjust the display accordingly */

  AlphabetizeGroups (pFormInfo);
  RepopulateChosen (pFormInfo);
  RecalculateChosen (pFormInfo);
  ArrowCursor ();
}

/*==================================================================*/
/*                                                                  */
/*  SetupMenus () - Create and initialize the pulldown menus.       */
/*                                                                  */
/*==================================================================*/

static void SetupMenus (WindoW w, FormInfoPtr pFormInfo, Boolean explodeToggle, Boolean advancedQueryToggle)

{
  IteM  clearUnusedItem;
  MenU  m;
  MenU  s;

  /*-----------*/
  /* File menu */
  /*-----------*/

  m = PulldownMenu (w, "File");
  CommandItem (m, "Import UID List.../I", ImportUIDs_Callback);
  CommandItem (m, "Export UID List.../E", ExportUIDs_Callback);
  SeparatorItem (m);
  CommandItem (m, "Quit/Q", Quit_Callback);

  /*-----------*/
  /* Edit menu */
  /*-----------*/

  m = PulldownMenu (w, "Edit");
  FormCommandItem (m, CUT_MENU_ITEM, (BaseFormPtr) pFormInfo, VIB_MSG_CUT);
  FormCommandItem (m, COPY_MENU_ITEM, (BaseFormPtr) pFormInfo, VIB_MSG_COPY);
  FormCommandItem (m, PASTE_MENU_ITEM, (BaseFormPtr) pFormInfo, VIB_MSG_PASTE);
  FormCommandItem (m, CLEAR_MENU_ITEM, (BaseFormPtr) pFormInfo, VIB_MSG_DELETE);

  /*--------------*/
  /* Options menu */
  /*--------------*/

  m = PulldownMenu (w, "Options");

  clearUnusedItem = CommandItem (m, "Clear Unused Query Terms", Query_ClearUnusedTerms);
  SetObjectExtra (clearUnusedItem, pFormInfo, NULL);
  pFormInfo->advancedQueryItem = StatusItem (m, "Advanced Queries", AdvancedQueryToggle_Callback);
  SetStatus (pFormInfo->advancedQueryItem, advancedQueryToggle);
  SetObjectExtra (pFormInfo->advancedQueryItem, pFormInfo, NULL);

  pFormInfo->explodeItem = StatusItem (m, "Explode Terms", ExplodeTermsToggle_Callback);
  SetStatus (pFormInfo->explodeItem, explodeToggle);
  SetObjectExtra (pFormInfo->explodeItem, pFormInfo, NULL);

  s = SubMenu (m, "Show ASN.1");
  s_showASNItem = ChoiceGroup (s, NULL);
  ChoiceItem (s_showASNItem, "OFF");
  ChoiceItem (s_showASNItem, "1");
  ChoiceItem (s_showASNItem, "2");
  ChoiceItem (s_showASNItem, "3");
  ChoiceItem (s_showASNItem, "4");
  ChoiceItem (s_showASNItem, "5");
  ChoiceItem (s_showASNItem, "6");
  ChoiceItem (s_showASNItem, "7");
  ChoiceItem (s_showASNItem, "8");
  ChoiceItem (s_showASNItem, "9");
  ChoiceItem (s_showASNItem, "ON");
  ChoiceItem (s_showASNItem, "LOG");
  SetValue (s_showASNItem, 1);

  /*------------*/
  /* Help menu */
  /*------------*/

  m = PulldownMenu (w, "Help");
  CommandItem (m, "Databases...", DatabaseView_Callback);
  CommandItem (m, "Fields...", FieldView_Callback);
  CommandItem (m, "Modes...", ModeView_Callback);
}

/*==================================================================*/
/*                                                                  */
/*  SetupAdvQueryGroup () - Create and initialize the Advanced      */
/*                          Query Form.                             */
/*                                                                  */
/*==================================================================*/

static GrouP SetupAdvQueryGroup (FormInfoPtr pFormInfo, GrouP mainGroup)

{
  GrouP   advQueryGroup;
  GrouP   buttonGroup;
  GrouP   instructionGroup;
  Char    tempStr [256];

  advQueryGroup = HiddenGroup (mainGroup, -1, 0, NULL);
  SetGroupSpacing (advQueryGroup, 5, 5);

  /*----------------------------------*/
  /* Set up alternative boolean query */
  /* refinement text edit box.        */
  /*----------------------------------*/

  pFormInfo->advQueryText = ScrollText (advQueryGroup, ADV_TEXT_WIDTH, ADV_TEXT_HEIGHT, programFont, TRUE, AdvancedQueryText_Callback);
  SetObjectExtra (pFormInfo->advQueryText, pFormInfo, NULL);

  /*------------------------------------------*/
  /* Create a group for the Retrieve/Evaluate */
  /* combined button and the Reset button.    */
  /*------------------------------------------*/

  buttonGroup = HiddenGroup (advQueryGroup, 5, 0, NULL);
  SetGroupSpacing (buttonGroup, 10, 10);

  pFormInfo->advRetrieveButton = PushButton (buttonGroup, "Retrieve 000000000 Documents", EvaluateRetrieve_Callback);
  SetObjectExtra (pFormInfo->advRetrieveButton, pFormInfo, NULL);
  SetTitle (pFormInfo->advRetrieveButton, "Evaluate");
  Disable (pFormInfo->advRetrieveButton);
  pFormInfo->advQueryState = ADV_QUERY_INVALID_STATE;

  pFormInfo->advResetButton = PushButton (buttonGroup, "Reset", Reset_Callback);
  SetObjectExtra (pFormInfo->advResetButton, pFormInfo, NULL);

  /*----------------------------------------------*/
  /* Create instructions for using advanced query */
  /*----------------------------------------------*/

  instructionGroup = HiddenGroup (advQueryGroup, 1, 0, NULL);
  StaticPrompt (instructionGroup, "Operators:", 0, 0, programFont, 'l');
  StaticPrompt (instructionGroup, "", 0, 0, programFont, 'l');
  StaticPrompt (instructionGroup, "    & (and)", 0, 0, programFont, 'l');
  StaticPrompt (instructionGroup, "    | (or)", 0, 0, programFont, 'l');
  StaticPrompt (instructionGroup, "    - (butnot)", 0, 0, programFont, 'l');
  StaticPrompt (instructionGroup, "    : (range)", 0, 0, programFont, 'l');
  StaticPrompt (instructionGroup, "", 0, 0, programFont, 'l');
  StaticPrompt (instructionGroup, "Example:", 0, 0, programFont, 'l');
  StaticPrompt (instructionGroup, "", 0, 0, programFont, 'l');
  tempStr [0] = '\0';
  StringCat (tempStr, "    ((\"glucagon\" [WORD] | \"insulin\" ");
  StringCat (tempStr, "[MESH]) & (\"1995\" : \"1996\" [PDAT]))");
  StaticPrompt (instructionGroup, tempStr, 0, 0, programFont, 'l');
  StaticPrompt (instructionGroup, "", 0, 0, programFont, 'l');
  StaticPrompt (instructionGroup, "Use [*] to search all fields.", 0, 0, programFont, 'l');

  /*--------------------------------*/
  /* Return the newly created group */
  /*--------------------------------*/

  return advQueryGroup;
}

/*==================================================================*/
/*                                                                  */
/*  GetValueFromField () -                                          */
/*                                                                  */
/*==================================================================*/

static Int4 GetValueFromField (DoC d, Int2 item, Int2 row, Int2 col)

{
  CharPtr  str;
  Int4     value;

  value = -1;
  str = GetDocText (d, item, row, col);
  if (str != NULL) {
    if (! StrToLong (str, &value)) {
      value = -1;
    }
  }

  MemFree (str);
  return value;
}

/*==================================================================*/
/*                                                                  */
/*  DrawAvailLeaf () -                                              */
/*                                                                  */
/*==================================================================*/

static void DrawAvailLeaf (DoC d, RectPtr r, Int2 item, Int2 frst)

{
  RecT  q;
  Int2  value;

  if (r == NULL || frst != 0) return;

  value = GetValueFromField (d, item, 1, 3);
  if (value != 1) return;

  q = *r;
  q.left++;
  q.right = q.left + 4;
  q.top += stdLineHeight / 2 - 2;
  q.bottom = q.top + 4;
  value = GetValueFromField (d, item, 1, 3);
  if (value == STATE_ON) {
    /*
       InvertColors ();
    */
    EraseRect (&q);
    PaintRect (&q);
    /*
       InvertColors ();
    */
  } else
    PaintRect (&q);
}

/*==================================================================*/
/*                                                                  */
/*  HighlightAvail () -                                             */
/*                                                                  */
/*==================================================================*/

static Boolean HighlightAvail (DoC d, Int2 item, Int2 row, Int2 col)

{
  FormInfoPtr  pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (d);

  if (item == pFormInfo->availItem && row == pFormInfo->availRow) return TRUE;

  return FALSE;
}

/*==================================================================*/
/*                                                                  */
/*  ClickAvail () -                                                 */
/*                                                                  */
/*==================================================================*/

static void ClickAvail (DoC d, PoinT pt)

{
  Int2         item;
  Int2         row;
  FormInfoPtr  pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (d);

  MapDocPoint (d, pt, &item, &row, NULL, NULL);
  pFormInfo->availClickItem = item;
  pFormInfo->availClickRow = row;
  pFormInfo->wasDoubleClick = dblClick;

  SafeEnable (pFormInfo->acceptButton);
}

/*==================================================================*/
/*                                                                  */
/*  ReleaseAvail () - This is called whenever the mouse button is   */
/*                   realeased in the term selection window.        */
/*                                                                  */
/*==================================================================*/

static void ReleaseAvail (DoC d, PoinT pt)

{
  Char         ch;
  Int2         item;
  Int2         olditem;
  Int2         oldrow;
  CharPtr      ptr;
  Int2         row;
  CharPtr      text;
  Int4         iTermCount;
  CharPtr      sTermCount;
  FormInfoPtr  pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (d);

  /*------------------------------------*/
  /* Convert the screen location of the */
  /* release into an item and a row.    */
  /*------------------------------------*/

  MapDocPoint (d, pt, &item, &row, NULL, NULL);

  /* ---------------------------------------- */
  /* There's no dragging in the avail window */
  /* ---------------------------------------- */

  if (pFormInfo->availClickItem != item || pFormInfo->availClickRow != row) return;

  text = GetDocText (pFormInfo->availDoc, item, row, 1);
  if (StringHasNoText (text)) {
    text = MemFree (text);
  }

  olditem = pFormInfo->availItem;
  oldrow = pFormInfo->availRow;
  if (text != NULL) {
    pFormInfo->availItem = item;
    pFormInfo->availRow = row;
  } else {
    pFormInfo->availItem = 0;
    pFormInfo->availRow = 0;
  }
  if (olditem > 0 && oldrow > 0)
    InvalDocRows (pFormInfo->availDoc, olditem, oldrow, oldrow);
  if (text != NULL && item > 0 && row > 0)
    InvalDocRows (pFormInfo->availDoc, item, row, row);

  if (text != NULL) {
    ptr = text;
    ch = *ptr;
    while (ch != '\0' && ch >= ' ') {
      ptr++;
      ch = *ptr;
    }
    *ptr = '\0';
    if (pFormInfo->currMode == RANGE_MODE) {
      if (CurrentText () == pFormInfo->fromText) {
        SafeSetTitle (pFormInfo->fromText, text);
        Select (pFormInfo->fromText);
      } else if (CurrentText () == pFormInfo->toText) {
        SafeSetTitle (pFormInfo->toText, text);
        Select (pFormInfo->toText);
      }
    } else {
      SafeSetTitle (pFormInfo->termText, text);
      Select (pFormInfo->termText);
    }
    Update ();
  } else {
    if (pFormInfo->currMode == RANGE_MODE) {
      SafeSetTitle (pFormInfo->fromText, "");
      SafeSetTitle (pFormInfo->toText, "");
      Select (pFormInfo->fromText);
    } else {
      SafeSetTitle (pFormInfo->termText, "");
      Select (pFormInfo->termText);
    }
    Update ();
  }

  if (text != NULL && pFormInfo->wasDoubleClick) {
    WatchCursor ();
    Update ();

    if ((pFormInfo->currMode == TAXONOMY_MODE) || (pFormInfo->currMode == MESH_TREE_MODE)) {
      Update ();
      ResetClip ();             /* clipped to panel, need to update popup */
      RepopulateTaxonomy (pFormInfo, text);
    } else if (pFormInfo->currMode == RANGE_MODE) {
      ResetClip ();
      sTermCount = GetDocText (pFormInfo->availDoc, pFormInfo->availItem,
                               pFormInfo->availRow, E2_COUNT_COL);
      TrimSpacesAroundString (sTermCount);
      iTermCount = atoi (sTermCount);
      MemFree (sTermCount);
      Query_AddBoolTerm (pFormInfo->form, pFormInfo->currDb,
                         pFormInfo->currField, text, STATE_ON, iTermCount);
      Update ();
      RecalculateChosen (pFormInfo);
    } else {
      ResetClip ();
      sTermCount = GetDocText (pFormInfo->availDoc, pFormInfo->availItem,
                               pFormInfo->availRow, E2_COUNT_COL);
      TrimSpacesAroundString (sTermCount);
      iTermCount = atoi (sTermCount);
      MemFree (sTermCount);
      Query_AddBoolTerm (pFormInfo->form, pFormInfo->currDb,
                         pFormInfo->currField, text, STATE_ON, iTermCount);
      Update ();
      RecalculateChosen (pFormInfo);
    }

    ArrowCursor ();
  }
  MemFree (text);
}

/*==================================================================*/
/*                                                                  */
/*  AvailTimerProc () -                                             */
/*                                                                  */
/*==================================================================*/

static void AvailTimerProc (WindoW w)

{
  FormInfoPtr  pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (w);
  pFormInfo->okayToAccept = TRUE;
}

/*==================================================================*/
/*                                                                  */
/*  InitAvailPanel () - Set up the Term List Panel.                 */
/*                                                                  */
/*==================================================================*/

static void InitAvailPanel (FormInfoPtr pFormInfo)

{
  RecT  r;

  pFormInfo->availItem = 0;
  pFormInfo->availRow = 0;
  SetDocShade (pFormInfo->availDoc, DrawAvailLeaf, NULL, HighlightAvail, NULL);
  SetDocProcs (pFormInfo->availDoc, ClickAvail, NULL, ReleaseAvail, NULL);
  SetDocCache (pFormInfo->availDoc, NULL, NULL, NULL);

  ObjectRect (pFormInfo->availDoc, &r);
  InsetRect (&r, 4, 4);
  SelectFont (systemFont);

  availColFmt [1].pixWidth = StringWidth ("0000000") + 10;
  availColFmt [0].pixWidth = (r.right - r.left) - availColFmt [1].pixWidth;
  availColFmt [2].pixWidth = 0;

  SetDocAutoAdjust (pFormInfo->availDoc, FALSE);

  InvalDocument (pFormInfo->availDoc);
}

/*==================================================================*/
/*                                                                  */
/*  DrawChosenBrackets () - This is the draw callback for the       */
/*                         Chosen document panel. It gets called    */
/*                         every time the panel is drawn.           */
/*                                                                  */
/*==================================================================*/

static void DrawChosenBrackets (DoC d, RectPtr r, Int2 item, Int2 frst)

{
  FormInfoPtr   pFormInfo;
  RecT          s;
  StateDataPtr  sdp;
  static Uint1  andsign [] = { 0x30, 0x48, 0x50, 0x20, 0x50, 0x8A, 0x84, 0x8A, 0x71, 0x00 };
  static Uint1  notsign [] = { 0x00, 0x00, 0xFF, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00 };

  pFormInfo = (FormInfoPtr) GetObjectExtra (d);

  sdp = pFormInfo->termList;
  while (sdp != NULL && item > 1) {
    sdp = sdp->next;
    item--;
  }
  if (sdp == NULL) return;

  switch (sdp->group) {
    case GROUP_SINGLE:
      break;
    case GROUP_FIRST:
      MoveTo (r->left + 16 + 1, r->top + 1);
      LineTo (r->left + 16 + 3, r->top + 1);
      MoveTo (r->left + 16 + 1, r->top + 2);
      LineTo (r->left + 16 + 3, r->top + 2);
      MoveTo (r->left + 16 + 1, r->top + 1);
      LineTo (r->left + 16 + 1, r->bottom);
      break;
    case GROUP_MIDDLE:
      MoveTo (r->left + 16 + 1, r->top);
      LineTo (r->left + 16 + 1, r->bottom);
      break;
    case GROUP_LAST:
      MoveTo (r->left + 16 + 1, r->top);
      LineTo (r->left + 16 + 1, r->bottom - 1);
      MoveTo (r->left + 16 + 1, r->bottom - 2);
      LineTo (r->left + 16 + 3, r->bottom - 2);
      MoveTo (r->left + 16 + 1, r->bottom - 1);
      LineTo (r->left + 16 + 3, r->bottom - 1);
      break;
    default:
      break;
  }

  switch (sdp->above) {
    case ENTREZ_OP_NONE:
      break;
    case ENTREZ_OP_AND:
      LoadRect (&s, r->left + 6, r->top, r->left + 14, r->top + 5);
      CopyBits (&s, andsign + 5);
      break;
    case ENTREZ_OP_BUTNOT:
      LoadRect (&s, r->left, r->top, r->left + 8, r->top + 5);
      CopyBits (&s, notsign + 5);
      break;
    default:
      break;
  }

  switch (sdp->below) {
    case ENTREZ_OP_NONE:
      break;
    case ENTREZ_OP_AND:
      LoadRect (&s, r->left + 6, r->bottom - 5, r->left + 14, r->bottom);
      CopyBits (&s, andsign);
      break;
    case ENTREZ_OP_BUTNOT:
      LoadRect (&s, r->left, r->bottom - 5, r->left + 8, r->bottom);
      CopyBits (&s, notsign);
      break;
    default:
      break;
  }
}

/*==================================================================*/
/*                                                                  */
/*  HighlightChosen () -                                            */
/*                                                                  */
/*==================================================================*/

static Boolean HighlightChosen (DoC d, Int2 item, Int2 row, Int2 col)

{
  StateDataPtr  sdp;
  FormInfoPtr   pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (d);

  if (col == 1 || col == 2) {
    if (pFormInfo->chosenInAboveBox || pFormInfo->chosenInBelowBox) return FALSE;
    /* if (item == pFormInfo->chosenItem) return TRUE; */
    return FALSE;
  } else if (col == 3) {
    sdp = pFormInfo->termList;
    while (sdp != NULL && item > 1) {
      sdp = sdp->next;
      item--;
    }
    if (sdp == NULL) return FALSE;
    return (Boolean) (sdp->state == STATE_ON);
  } else
    return FALSE;
}

/*==================================================================*/
/*                                                                  */
/*  FrameChosen () -                                                */
/*                                                                  */
/*==================================================================*/

static void FrameChosen (FormInfoPtr pFormInfo)

{
  RecT  sr;

  ObjectRect (pFormInfo->chosenDoc, &sr);
  InsetRect (&sr, 4, 4);

  if (RectInRect (&(pFormInfo->trackRect), &sr)) {
    Dotted ();
    FrameRect (&(pFormInfo->trackRect));
  }
}

/*==================================================================*/
/*                                                                  */
/*  ClickChosen () -                                                */
/*                                                                  */
/*==================================================================*/

static void ClickChosen (DoC d, PoinT pt)

{
  Int2         col;
  Int2         item;
  RecT         r;
  Int2         row;
  RecT         s;
  FormInfoPtr  pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (d);

  MapDocPoint (d, pt, &item, &row, &col, &r);
  if (item > 0 && row == 0 && col > 0)
    row = 1;

  pFormInfo->chosenClickItem = item;
  pFormInfo->chosenClickRow = row;
  pFormInfo->chosenClickCol = col;
  pFormInfo->wasDoubleClick = dblClick;
  pFormInfo->chosenInAboveBox = FALSE;
  pFormInfo->chosenInBelowBox = FALSE;

  if ((col == 1 || col == 2) && (item > 0) && (item <= pFormInfo->chosenNumLines) && (row > 0)) {
    pFormInfo->chosenItem = item;
    LoadRect (&s, r.left, r.top, r.left + 16, r.top + 5);
    if (PtInRect (pt, &s))
      pFormInfo->chosenInAboveBox = TRUE;
    LoadRect (&s, r.left, r.bottom - 5, r.left + 16, r.bottom);
    if (PtInRect (pt, &s))
      pFormInfo->chosenInBelowBox = TRUE;
    if (pFormInfo->chosenInAboveBox || pFormInfo->chosenInBelowBox)
      return;
    InvalDocCols (pFormInfo->chosenDoc, item, 1, 2);
    Update ();
    r.right = r.left + chosenColFmt [0].pixWidth + chosenColFmt [1].pixWidth;
    LoadRect (&(pFormInfo->trackRect), r.left + 22, r.top, r.right - 2, r.bottom);
    pFormInfo->trackPt = pt;
    InvertMode ();
    FrameChosen (pFormInfo);
  } else
    pFormInfo->chosenItem = 0;
}

/*==================================================================*/
/*                                                                  */
/*  DragChosen () -                                                 */
/*                                                                  */
/*==================================================================*/

static void DragChosen (DoC d, PoinT pt)

{
  Int4         off;
  BaR          sb;
  RecT         sr;
  FormInfoPtr  pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (d);

  if (pFormInfo->chosenItem == 0) return;
  if (pFormInfo->chosenInAboveBox || pFormInfo->chosenInBelowBox) return;
  InvertMode ();
  FrameChosen (pFormInfo);
  Update ();
  ObjectRect (pFormInfo->chosenDoc, &sr);
  InsetRect (&sr, 4, 4);
  if (PtInRect (pt, &sr)) {
    OffsetRect (&(pFormInfo->trackRect), 0, pt.y - pFormInfo->trackPt.y);
    pFormInfo->trackPt = pt;
  }
  sb = GetSlateVScrollBar ((SlatE) pFormInfo->chosenDoc);
  off = GetBarValue (sb);
  ResetClip ();
  if (pt.y < sr.top)
    SetBarValue (sb, off - 1);
  else if (pt.y > sr.bottom)
    SetBarValue (sb, off + 1);
  Update ();
  InvertMode ();
  FrameChosen (pFormInfo);
  Update ();
}

/*==================================================================*/
/*                                                                  */
/*  FlipOperatorBelow () -                                          */
/*                                                                  */
/*==================================================================*/

static void FlipOperatorBelow (FormInfoPtr pFormInfo, Int2 item)

{
  StateDataPtr  next;
  RecT          r;
  StateDataPtr  sdp;

  sdp = pFormInfo->termList;
  while (item > 1 && sdp != NULL) {
    item--;
    sdp = sdp->next;
  }
  if (sdp != NULL) {
    next = sdp->next;
    if (next != NULL) {
      if (sdp->below != next->above)
        Beep ();
      if (sdp->below == ENTREZ_OP_AND) {
        sdp->below = ENTREZ_OP_BUTNOT;
        next->above = ENTREZ_OP_BUTNOT;
      } else if (sdp->below == ENTREZ_OP_BUTNOT) {
        sdp->below = ENTREZ_OP_AND;
        next->above = ENTREZ_OP_AND;
      }
      ObjectRect (pFormInfo->chosenDoc, &r);
      InsetRect (&r, 4, 4);
      r.right = r.left + 16;
      Select (pFormInfo->chosenDoc);
      InvalRect (&r);
      ResetClip ();
      WatchCursor ();
      Update ();
      RecalculateChosen (pFormInfo);
      ArrowCursor ();
    }
  }
}

/*==================================================================*/
/*                                                                  */
/*  RemoveTermFromList () - Removes the given term from the given   */
/*                           doubly-linked list of terms, and       */
/*                           adjusts the surrounding terms          */
/*                           accordingly.                           */
/*                                                                  */
/*==================================================================*/

static Boolean RemoveTermFromList (FormInfoPtr pFormInfo, StateDataPtr sdp, Boolean goingDown, Boolean lastDraggedDown)

{
  StateDataPtr  prev;
  StateDataPtr  next;
  Int2          op;

  /*-----------------------------*/
  /* Sanity check - can't remove */
  /* term from an empty list.    */
  /*-----------------------------*/

  if (sdp == NULL) return E2_LIST_NOT_MODIFIED;

  /*------------------------------------*/
  /* Detach term from surrounding terms */
  /*------------------------------------*/

  next = sdp->next;
  prev = sdp->prev;
  switch (sdp->group) {
    case GROUP_SINGLE:
      if (lastDraggedDown)
        return E2_LIST_NOT_MODIFIED;
      break;
    case GROUP_FIRST:
      if (next != NULL) {
        if (next->group == GROUP_MIDDLE)
          next->group = GROUP_FIRST;
        else if (next->group == GROUP_LAST)
          next->group = GROUP_SINGLE;
      }
      break;
    case GROUP_MIDDLE:
      break;
    case GROUP_LAST:
      if (prev != NULL) {
        if (prev->group == GROUP_MIDDLE)
          prev->group = GROUP_LAST;
        else if (prev->group == GROUP_FIRST)
          prev->group = GROUP_SINGLE;
      }
      break;
    default:
      break;
  }

  /*-------------------------*/
  /* Adjust the state of the */
  /* surrounding terms.      */
  /*-------------------------*/

  sdp->prev = NULL;
  sdp->next = NULL;
  if (prev != NULL)
    prev->next = next;
  if (next != NULL)
    next->prev = prev;
  if (prev == NULL)
    pFormInfo->termList = next;
  op = ENTREZ_OP_NONE;
  switch (sdp->group) {
    case GROUP_SINGLE:
      if (goingDown) {
        if (prev != NULL)
          op = prev->below;
      } else {
        if (next != NULL)
          op = next->above;
      }
      if (prev != NULL)
        prev->below = op;
      if (next != NULL)
        next->above = op;
      break;
    case GROUP_FIRST:
      if (prev != NULL)
        op = prev->below;
      if (next != NULL)
        next->above = op;
      break;
    case GROUP_MIDDLE:
      break;
    case GROUP_LAST:
      if (next != NULL)
        op = next->above;
      if (prev != NULL)
        prev->below = op;
      break;
    default:
      break;
  }

  /*---------------------*/
  /* Return successfully */
  /*---------------------*/

  return E2_LIST_MODIFIED;
}

/*====================================================================*/
/*                                                                    */
/*  AddTermToList () - Adds a given term to the given doubly-linked   */
/*                      list of terms, and adjusts the surrounding    */
/*                      terms accordingly.                            */
/*                                                                    */
/*====================================================================*/

static void AddTermToList (FormInfoPtr pFormInfo, StateDataPtr newTerm, Int2 item, Boolean merge)

{
  StateDataPtr  prev;
  StateDataPtr  next;
  StateDataPtr  currTerm;
  Int2          op;

  /*-------------------------------------*/
  /* Add the new term into the term list */
  /*-------------------------------------*/

  currTerm = pFormInfo->termList;
  if (currTerm == NULL)
    pFormInfo->termList = newTerm;
  else {
    while (item > 1 && currTerm->next != NULL) {
      item--;
      currTerm = currTerm->next;
    }
    next = currTerm->next;
    prev = currTerm->prev;
    currTerm->next = newTerm;
    newTerm->next = next;
    newTerm->prev = currTerm;
    if (next != NULL)
      next->prev = newTerm;
    op = ENTREZ_OP_NONE;
    if (prev != NULL)
      op = prev->below;
    currTerm->above = op;
    newTerm->below = currTerm->below;
    currTerm->below = ENTREZ_OP_NONE;
    newTerm->above = ENTREZ_OP_NONE;
  }

  /*----------------------------------*/
  /* Do adding to group, if necessary */
  /*----------------------------------*/

  newTerm->group = GROUP_SINGLE;
  if (merge) {
    switch (currTerm->group) {
      case GROUP_SINGLE:
        currTerm->group = GROUP_FIRST;
        newTerm->group = GROUP_LAST;
        break;
      case GROUP_FIRST:
        newTerm->group = GROUP_MIDDLE;
        break;
      case GROUP_MIDDLE:
        newTerm->group = GROUP_MIDDLE;
        break;
      case GROUP_LAST:
        currTerm->group = GROUP_MIDDLE;
        newTerm->group = GROUP_LAST;
        break;
      default:
        break;
    }
  }

  /*-----------------------------------------*/
  /* If it's not in a group, then by default */
  /* it's AND'd with the neighboring terms.  */
  /*-----------------------------------------*/

  else {
    prev = newTerm->prev;
    if (prev != NULL) {
      prev->below = ENTREZ_OP_AND;
      newTerm->above = ENTREZ_OP_AND;
    }
  }
}

/*==================================================================*/
/*                                                                  */
/*  ReleaseChosen () - This is the callback function that gets      */
/*                    when the mouse button is released after       */
/*                    dragging a term in the Query Refinement       */
/*                    panel.                                        */
/*                                                                  */
/*==================================================================*/

static void ReleaseChosen (DoC d, PoinT pt)

{
  Int2          col;
  Boolean       goingDown;
  Int2          inval;
  Int2          item;
  Boolean       lastDraggedDown;
  Boolean       merge;
  Int2          oldItem;
  RecT          r;
  Int2          row;
  RecT          s;
  StateDataPtr  sdp;
  RecT          sr;
  FormInfoPtr   pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (d);

  /*---------------------------------------*/
  /* Map the current cursor position to an */
  /* item in the Query Refinement panel.   */
  /*---------------------------------------*/

  MapDocPoint (d, pt, &item, &row, &col, &r);
  if (item > 0 && row == 0 && col > 0)
    row = 1;

  /*-------------*/
  /*-------------*/

  if (pFormInfo->chosenInAboveBox || pFormInfo->chosenInBelowBox) {
    LoadRect (&s, r.left, r.top, r.left + 16, r.top + 5);
    if (PtInRect (pt, &s)) {
      if ((pFormInfo->chosenInAboveBox) && (item == pFormInfo->chosenItem))
        FlipOperatorBelow (pFormInfo, pFormInfo->chosenItem - 1);
      else if ((pFormInfo->chosenInBelowBox) && (item == pFormInfo->chosenItem - 1))
        FlipOperatorBelow (pFormInfo, pFormInfo->chosenItem - 1);
    }
    LoadRect (&s, r.left, r.bottom - 5, r.left + 16, r.bottom);
    if (PtInRect (pt, &s)) {
      if ((pFormInfo->chosenInBelowBox) && (item == pFormInfo->chosenItem))
        FlipOperatorBelow (pFormInfo, pFormInfo->chosenItem);
      else if ((pFormInfo->chosenInAboveBox) && (item == pFormInfo->chosenItem + 1))
        FlipOperatorBelow (pFormInfo, pFormInfo->chosenItem);
    }
    return;
  }

  /*-----------------------*/
  /*-----------------------*/

  if (pFormInfo->chosenItem > 0) {

    oldItem = pFormInfo->chosenItem;
    pFormInfo->chosenItem = 0;
    InvertMode ();
    FrameChosen (pFormInfo);
    Update ();
    InvalDocCols (pFormInfo->chosenDoc, oldItem, 1, 2);
    Update ();
    if (item == oldItem) return;
    if (pFormInfo->chosenNumLines < 2) return;

      /*-----------------------------------------------*/
    /* Are we dragging the last item 'past' the end? */
      /*-----------------------------------------------*/

    lastDraggedDown = FALSE;
    if (oldItem == pFormInfo->chosenNumLines && item > pFormInfo->chosenNumLines)
      lastDraggedDown = TRUE;

      /*-----------*/
      /*-----------*/

    merge = TRUE;
    ObjectRect (pFormInfo->chosenDoc, &sr);
    InsetRect (&sr, 4, 4);
    /*
       if (item > pFormInfo->chosenNumLines && PtInRect (pt, &sr))
    */
    if (item == 0 && PtInRect (pt, &sr)) {
      merge = FALSE;
      item = INT2_MAX;
    }

      /*-------*/
      /*-------*/

    if ((item <= 0) || (pFormInfo->termList == NULL))
      return;

      /*--------------------------------------*/
    /* Are we moving an item down the list? */
      /*--------------------------------------*/

    goingDown = FALSE;
    if (item > oldItem) {
      item--;
      goingDown = TRUE;
    }

      /*--------------------------------*/
    /* Find the item being dragged in */
    /* the state list.                */
      /*--------------------------------*/

    sdp = pFormInfo->termList;
    while (oldItem > 1 && sdp != NULL) {
      oldItem--;
      sdp = sdp->next;
    }

    /*----------------------------------------*/
    /* Remove the term from the term list and */
    /* then re-add it in it's new position.   */
    /*----------------------------------------*/

    if (RemoveTermFromList (pFormInfo, sdp, goingDown, lastDraggedDown) ==
        E2_LIST_NOT_MODIFIED) return;

    AddTermToList (pFormInfo, sdp, item, merge);

    /*---------------------------*/
    /* Recalculate the new query */
    /* and redisplay it.         */
    /*---------------------------*/

    ResetClip ();
    WatchCursor ();
    Update ();
    AlphabetizeGroups (pFormInfo);
    RepopulateChosen (pFormInfo);
    RecalculateChosen (pFormInfo);
    ArrowCursor ();
  }

  /*------------------------------------*/
  /* If the count column was clicked on */
  /* then toggle rows state on or off.  */
  /*------------------------------------*/

  if ((pFormInfo->chosenClickItem == item) &&
      (pFormInfo->chosenClickRow == row)   &&
      (pFormInfo->chosenClickCol == col)   &&
      (col == 3)) {
    inval = item;
    sdp = pFormInfo->termList;
    while (sdp != NULL && item > 1) {
      sdp = sdp->next;
      item--;
    }
    if (sdp == NULL) return;
    switch (sdp->state) {
      case STATE_OFF:
        sdp->state = STATE_ON;
        break;
      case STATE_ON:
        sdp->state = STATE_OFF;
        break;
      default:
        sdp->state = STATE_OFF;
        break;
    }
    ResetClip ();
    WatchCursor ();
    InvalDocCols (pFormInfo->chosenDoc, inval, 3, 3);
    Update ();
    RecalculateChosen (pFormInfo);
    ArrowCursor ();
  }
}

/*==================================================================*/
/*                                                                  */
/*  InitChosenPanel () -                                            */
/*                                                                  */
/*==================================================================*/

static void InitChosenPanel (FormInfoPtr pFormInfo)

{
  Entrez2InfoPtr       e2ip;
  Entrez2DbInfoPtr     e2db;
  Entrez2FieldInfoPtr  e2fd;
  Int2                 maxWidth = 0;
  RecT                 r;
  Char                 str [32];

  SetDocProcs (pFormInfo->chosenDoc, ClickChosen, DragChosen, ReleaseChosen, NULL);
  SetDocShade (pFormInfo->chosenDoc, DrawChosenBrackets, NULL, HighlightChosen, NULL);
  pFormInfo->chosenNumLines = 0;

  ObjectRect (pFormInfo->chosenDoc, &r);
  InsetRect (&r, 4, 4);

  SelectFont (systemFont);
  chosenColFmt [2].pixWidth = StringWidth ("0000000") + 10;
  chosenColFmt [1].pixWidth = StringWidth ("0000000")  + 10;

  /* more accurate calculation of chosenColFmt [1].pixWidth */

  e2ip = Query_GetInfo ();
  if (e2ip != NULL) {
    for (e2db = e2ip->db_info; e2db != NULL; e2db = e2db->next) {
      for (e2fd = e2db->fields; e2fd != NULL; e2fd = e2fd->next) {
        if (e2fd->field_name == NULL || StringLen (e2fd->field_name) > 20) continue;
        sprintf (str, " [%s]", e2fd->field_name);
        maxWidth = MAX (maxWidth, StringWidth (str));
      }
    }
    chosenColFmt [1].pixWidth = maxWidth  + 10;
  }

  chosenColFmt [0].pixWidth = (r.right - r.left) - chosenColFmt [1].pixWidth - chosenColFmt [2].pixWidth;
  SetDocAutoAdjust (pFormInfo->chosenDoc, FALSE);

  InvalDocument (pFormInfo->chosenDoc);
}

/*==================================================================*/
/*                                                                  */
/*  SetupStdQueryGroup () - Create and initialize the Standard      */
/*                          Query Form.                             */
/*                                                                  */
/*==================================================================*/

static GrouP SetupStdQueryGroup (FormInfoPtr pFormInfo, GrouP mainGroup)

{
  Entrez2DbInfoPtr   e2db;
  Entrez2InfoPtr     e2ip;
  GrouP              stdQueryGroup;
  GrouP              enumlistGroup;
  EnumFieldAssocPtr  alist;
  Int2               idx;
  Int2               db;
  Int2               wid;
  GrouP              c;
  GrouP              j;
  GrouP              q1;
  GrouP              q2;
  GrouP              q3;
  PrompT             ppt1;
  PrompT             ppt2;
  ModeChoice         mc;
  ModeIndex          md;
  RecT               r;

  /*---------------------------------*/
  /* Get info about the DB server(s) */
  /*---------------------------------*/

  e2ip = Query_GetInfo ();
  if (e2ip == NULL) return NULL;

  /*---------------------------------*/
  /* Create the Standard Query Group */
  /*---------------------------------*/

  stdQueryGroup = HiddenGroup (mainGroup, -1, 0, NULL);
  SetGroupSpacing (stdQueryGroup, 5, 5);

  /*---------------------------------*/
  /* Create a group for the pulldown */
  /* lists and the accept button.    */
  /*---------------------------------*/

  enumlistGroup = HiddenGroup (stdQueryGroup, 0, 2, NULL);
  SetGroupSpacing (enumlistGroup, 15, 2);

  /*-------------------------------*/
  /* Set up database pulldown list */
  /*-------------------------------*/

  StaticPrompt (enumlistGroup, "Database:", 0, 0, programFont, 'l');
  q1 = HiddenGroup (enumlistGroup, 0, 0, NULL);
  alist = CreateDatabaseAlist (e2ip);
  idx = DBGetIDFromName ("PubMed");

  pFormInfo->databasePopup = CreateEnumPopupDialog (q1, TRUE, ChangeDatabase_Callback, alist, (UIEnum) idx, NULL);
  SetObjectExtra (pFormInfo->databasePopup, pFormInfo, NULL);
  FreeEnumFieldAlist (alist);
  pFormInfo->currDb = idx;

  /*-------------------------------*/
  /* Set up field pulldown list(s) */
  /*-------------------------------*/

  StaticPrompt (enumlistGroup, "Field:", 0, 0, programFont, 'l');
  q2 = HiddenGroup (enumlistGroup, 0, 0, NULL);
  pFormInfo->fieldsPopup = MemNew (sizeof (PopuP) * (e2ip->db_count + 2));
  idx = FieldGetIDFromName (pFormInfo->currDb, "ALL");
  for (e2db = e2ip->db_info; e2db != NULL; e2db = e2db->next) {
    db = DBGetIDFromName (e2db->db_name);
    alist = CreateFieldAlist (e2db);
    pFormInfo->fieldsPopup [db] = CreateEnumPopupDialog (q2, TRUE, ChangeField_Callback, alist, (UIEnum) idx, pFormInfo);
    FreeEnumFieldAlist (alist);
    Hide (pFormInfo->fieldsPopup [db]);
  }
  pFormInfo->currField = idx;
  db = DBGetIDFromName ("PubMed");
  Show (pFormInfo->fieldsPopup [db]);

  /*---------------------------*/
  /* Set up mode pulldown list */
  /*---------------------------*/

  StaticPrompt (enumlistGroup, "Mode:", 0, 0, programFont, 'l');
  q3 = HiddenGroup (enumlistGroup, 0, 0, NULL);
  for (md = POPUP_MULT; md <= POPUP_UID; md++) {
    alist = mode_alists [md];
    mc = SELECTION_MODE;
    if (md == POPUP_MULT)
      mc = AUTOMATIC_MODE;
    else if (md == POPUP_AUTO)
      mc = TRANSLATE_MODE;
    else if (md == POPUP_UID)
      mc = LOOKUP_UID_MODE;
    pFormInfo->modesPopup [md] = CreateEnumPopupDialog (q3, TRUE, ChangeMode_Callback, alist, (UIEnum) mc, pFormInfo);
    Hide (pFormInfo->modesPopup [md]);
  }
  pFormInfo->currMode = AUTOMATIC_MODE;
  md = POPUP_MULT;
  Show (pFormInfo->modesPopup [md]);

  /*----------------------*/
  /* Set up accept button */
  /*----------------------*/

  StaticPrompt (enumlistGroup, "", 0, 0, programFont, 'l');
  pFormInfo->acceptButton = DefaultButton (enumlistGroup, "Accept", Accept_Callback);
  SetObjectExtra (pFormInfo->acceptButton, pFormInfo, NULL);
  Disable (pFormInfo->acceptButton);

  AlignObjects (ALIGN_MIDDLE, (HANDLE) q1, (HANDLE) q2, (HANDLE) q3, (HANDLE) pFormInfo->acceptButton, NULL);

  /*------------------------------*/
  /* Create a group to contain:   */
  /*                              */
  /*    Term entry                */
  /*    From/To Range entry       */
  /*    Taxonomy 'Lineage' popup  */
  /*                              */
  /* These are mutually exclusive */
  /* and only one of these will   */
  /* be visible at a time.        */
  /*------------------------------*/

  c = HiddenGroup (stdQueryGroup, 0, 0, NULL);

  GetPosition (pFormInfo->acceptButton, &r);
  SelectFont (programFont);
  wid = r.right - (StringWidth ("From:") + StringWidth ("To:")) - 30;
  SelectFont (systemFont);
  wid /= 2 * stdCharWidth;

  pFormInfo->termGroup = HiddenGroup (c, 2, 0, NULL);
  StaticPrompt (pFormInfo->termGroup, "Term:", 0, dialogTextHeight, programFont, 'l');
  pFormInfo->termText = DialogText (pFormInfo->termGroup, "", 20, TextAction);
  SetObjectExtra (pFormInfo->termText, pFormInfo, NULL);

  pFormInfo->rangeGroup = HiddenGroup (c, 4, 0, NULL);
  StaticPrompt (pFormInfo->rangeGroup, "From:", 0, dialogTextHeight, programFont, 'l');
  pFormInfo->fromText = DialogText (pFormInfo->rangeGroup, "", wid, FromTextAction);
  pFormInfo->isValidFrom = FALSE;
  SetObjectExtra (pFormInfo->fromText, pFormInfo, NULL);

  StaticPrompt (pFormInfo->rangeGroup, "To:", 0, dialogTextHeight, programFont, 'l');
  pFormInfo->toText = DialogText (pFormInfo->rangeGroup, "", wid, ToTextAction);
  pFormInfo->isValidTo = FALSE;
  SetObjectExtra (pFormInfo->toText, pFormInfo, NULL);
  Hide (pFormInfo->rangeGroup);

  pFormInfo->taxLineagePopup = PopupList (c, TRUE, ChangeTaxParents_Callback);
  Hide (pFormInfo->taxLineagePopup);
  SetObjectExtra (pFormInfo->taxLineagePopup, pFormInfo, NULL);
  pFormInfo->taxStrings = NULL;

  /*------------------------------------*/
  /* Create the Avail and Chosen panels */
  /*------------------------------------*/

  c = HiddenGroup (stdQueryGroup, -1, 0, NULL);
  ppt1 = StaticPrompt (c, "Term Selection", 0, 0, programFont, 'c');
  pFormInfo->availDoc = DocumentPanel (c, 10 * stdCharWidth, 7 * stdLineHeight);
  SetObjectExtra (pFormInfo->availDoc, pFormInfo, NULL);

  ppt2 = StaticPrompt (c, "Query Refinement", 0, 0, programFont, 'c');
  j = HiddenGroup (c, 0, 0, NULL);
  pFormInfo->chosenDoc = DocumentPanel (j, 10 * stdCharWidth, 7 * stdLineHeight);
  SetObjectExtra (pFormInfo->chosenDoc, pFormInfo, NULL);

  /*-------------------*/
  /* Align the objects */
  /*-------------------*/

  AlignObjects (ALIGN_CENTER, (HANDLE) pFormInfo->taxLineagePopup, (HANDLE) pFormInfo->termGroup, NULL);
  AlignObjects (ALIGN_MIDDLE, (HANDLE) pFormInfo->taxLineagePopup, (HANDLE) pFormInfo->termGroup, (HANDLE) pFormInfo->rangeGroup, NULL);
  AlignObjects (ALIGN_RIGHT,
                (HANDLE) pFormInfo->acceptButton,
                (HANDLE) pFormInfo->termText,
                (HANDLE) pFormInfo->availDoc, (HANDLE) pFormInfo->chosenDoc, (HANDLE) pFormInfo->toText, (HANDLE) ppt1, (HANDLE) ppt2, NULL);

  /*---------------------------------*/
  /* Create a group for the Retrieve */
  /* and Reset buttons.              */
  /*---------------------------------*/

  c = HiddenGroup (stdQueryGroup, 5, 0, NULL);
  SetGroupSpacing (c, 10, 10);

  pFormInfo->retrieveButton = PushButton (c, "Retrieve 000000000 Documents", pFormInfo->retrieveDocsProc);
  SetObjectExtra (pFormInfo->retrieveButton, pFormInfo, NULL);
  SetTitle (pFormInfo->retrieveButton, "Retrieve 0 Documents");
  Disable (pFormInfo->retrieveButton);

  pFormInfo->resetButton = PushButton (c, "Reset", Reset_Callback);
  SetObjectExtra (pFormInfo->resetButton, pFormInfo, NULL);

  /*----------------------------------------*/
  /* Initialize the Avail and Chosen panels */
  /*----------------------------------------*/

  InitChosenPanel (pFormInfo);
  InitAvailPanel (pFormInfo);

  /*---------------------*/
  /* Return successfully */
  /*---------------------*/

  return stdQueryGroup;
}

/*==================================================================*/
/*                                                                  */
/*  CreateTermlistForm() -                                          */
/*                                                                  */
/*     The object hierarchy is:                                     */
/*                                                                  */
/*        MainWindow                                                */
/*           MainGroup                                              */
/*                                                                  */
/*              PulldownMenus                                       */
/*                                                                  */
/*              StdQueryGroup                                       */
/*                 EnumListGroup                                    */
/*                    DbPopup                                       */
/*                    FieldPopup                                    */
/*                    ModePopup(s)                                  */
/*                    AcceptButton                                  */
/*                 MiscGroup                                        */
/*                    TermDialogText                                */
/*                    FromDialogText                                */
/*                    ToDialogText                                  */
/*                    TaxLineagePopup                               */
/*                 AvailGroup                                       */
/*                    AvailPrompt                                   */
/*                    AvailPanel                                    */
/*                 ChosenGroup                                      */
/*                    ChosenPrompt                                  */
/*                    ChosenPanel                                   */
/*                 ButtonGroup                                      */
/*                    RetrieveButton                                */
/*                    ResetButton                                   */
/*                                                                  */
/*              AdvQueryGroup                                       */
/*                 AdvQueryTextBox                                  */
/*                 ButtonGroup                                      */
/*                    EvaluateButton                                */
/*                    RetrieveButton                                */
/*                    ResetButton                                   */
/*                                                                  */
/*==================================================================*/

NLM_EXTERN ForM CreateTermlistForm (
  Int2 left,
  Int2 top,
  CharPtr title,
  WndActnProc activateCallback,
  FormMessageFunc messagesCallback,
  E2RetrieveDocsProc retrieveCallback,
  E2RetrieveUidProc retrieveUidCallback,
  Boolean explodeToggle,
  Boolean advancedQueryToggle
)

{
  WindoW       mainWindow;
  GrouP        mainGroup;
  FormInfoPtr  pFormInfo;

  /*-----------------------------------*/
  /* Create a formInfo structure to be */
  /* filled in by this function.       */
  /*-----------------------------------*/

  pFormInfo = (FormInfoPtr) MemNew (sizeof (FormInfo));
  if (pFormInfo == NULL) return NULL;

  /*-----------------------------*/
  /* Fill in some default values */
  /*-----------------------------*/

  pFormInfo->advQueryLexPos = 0;
  pFormInfo->advQueryLexStr = NULL;
  pFormInfo->advQueryLexState = LEXSTATE_IDLE;
  pFormInfo->advQueryNextToken = NULL;
  pFormInfo->form = NULL;
  pFormInfo->termList = NULL;

  /*----------------------------*/
  /* Create the Main Window and */
  /* set up basic callbacks.    */
  /*----------------------------*/

  mainWindow = FixedWindow (left, top, -10, -10, title, StdSendCloseWindowMessageProc);
  if (mainWindow == NULL) return NULL;

  pFormInfo->formmessage = EditMessage_Callback;
  pFormInfo->appmessage = messagesCallback;
  pFormInfo->activate = activateCallback;
  pFormInfo->retrieveDocsProc = retrieveCallback;
  pFormInfo->retrieveUidProc = retrieveUidCallback;
  SetActivate (mainWindow, TermListActivate_Callback);

  /*---------------------------------------*/
  /* Set up the menus and an overall group */
  /* for everything in the termlist window */
  /*---------------------------------------*/

  SetupMenus (mainWindow, pFormInfo, explodeToggle, advancedQueryToggle);

  mainGroup = HiddenGroup (mainWindow, 0, 0, NULL);

  pFormInfo->stdQueryGroup = SetupStdQueryGroup (pFormInfo, mainGroup);
  pFormInfo->advQueryGroup = SetupAdvQueryGroup (pFormInfo, mainGroup);

  AlignObjects (ALIGN_RIGHT, (HANDLE) pFormInfo->availDoc, (HANDLE) pFormInfo->advQueryText, NULL);

  /*----------------------------------*/
  /* Show the advanced query group if */
  /* we're in advanced query mode.    */
  /*----------------------------------*/

  if (advancedQueryToggle == TRUE)
    SafeHide (pFormInfo->stdQueryGroup);
  else
    SafeHide (pFormInfo->advQueryGroup);

  /*--------------------------------------*/
  /* Select the default starting database */
  /*--------------------------------------*/

  ChangeDatabase (pFormInfo->databasePopup);

  /*--------------------------------*/
  /* Fill in the FormInfo structure */
  /*--------------------------------*/

  pFormInfo->form = (ForM) mainWindow;

  /*---------------------*/
  /* Return successfully */
  /*---------------------*/

  SetObjectExtra ((ForM) mainWindow, (BaseFormPtr) pFormInfo, TermListCleanup_Callback);

  RealizeWindow (mainWindow);
  SetWindowTimer (mainWindow, AvailTimerProc);

  return pFormInfo->form;
}

/*==================================================================*/
/*                                                                  */
/* Query_GetInfo () - Connect to the Entrez2 server and get DB      */
/*                   info -- db names, field names, etc.            */
/*                                                                  */
/*==================================================================*/

NLM_EXTERN Entrez2InfoPtr Query_GetInfo (void)

{
  Entrez2RequestPtr  e2rq;
  Entrez2ReplyPtr    e2ry;
  FILE               *fp;
  ValNodePtr         head = NULL;
  Char               path [PATH_MAX];
  CharPtr            str;
  ValNodePtr         vnp;

  /*---------------------------------*/
  /* Only query the server once, use */
  /* stored version afterwards.      */
  /*---------------------------------*/

  if (s_masterE2ip != NULL) return s_masterE2ip;

  /*----------------------------------*/
  /* Request the data from the server */
  /*----------------------------------*/

  e2rq = EntrezCreateGetInfoRequest ();
  if (e2rq == NULL) return NULL;

  if (ShowASN () == TRUE)
    DisplayEntrezRequest (e2rq);

  e2ry = SpecialEntrezSynchronousQuery (e2rq);
  if (e2ry == NULL) return NULL;

  if (ShowASN () == TRUE)
    DisplayEntrezReply (e2ry);

  s_masterE2ip = EntrezExtractInfoReply (e2ry);

  Entrez2RequestFree (e2rq);

  /*------------------------------*/
  /* Validate EntrezInfo contents */
  /*------------------------------*/

  if (! ValidateEntrez2InfoPtr (s_masterE2ip, &head)) {
    TmpNam (path);
    fp = FileOpen (path, "w");
    if (fp != NULL) {
      fprintf (fp, "Entrez2Info Validation Errors\n\n");
      for (vnp = head; vnp != NULL; vnp = vnp->next) {
        str = (CharPtr) vnp->data.ptrvalue;
        if (str == NULL) continue;
        fprintf (fp, "%s\n", str);
      }
      FileClose (fp);
      LaunchGeneralTextViewer (path, "ValidateEntrez2InfoPtr");
    }
    FileRemove (path);
    ValNodeFreeData (head);
  }

  /*---------------------*/
  /* Return successfully */
  /*---------------------*/

  return s_masterE2ip;
}

/*==================================================================*/
/*                                                                  */
/*  TermlistToRequestCloseGroup () - Builds a boolean request from  */
/*                           the info in the given TermFormPtr.     */
/*                                                                  */
/*==================================================================*/

static Boolean TermlistToRequestCloseGroup (FormInfoPtr pFormInfo, Entrez2RequestPtr e2RequestPtr)

{
  Int2                 group;
  Int2                 last;
  StateDataPtr         sdp;
  Boolean              isEmpty = TRUE;
  Boolean              doNotExplode;
  Boolean              doNotTranslate;
  CharPtr              rangeStr;
  CharPtr              fromTerm;
  CharPtr              toTerm;
  Entrez2FieldInfoPtr  fieldInfo;
  CharPtr              dbName;

  group = 0;
  last = 0;
  doNotExplode = ! GetStatus (pFormInfo->explodeItem);
  sdp = pFormInfo->termList;

  /* Loop through all the terms in the linked list */

  for (sdp = pFormInfo->termList; sdp != NULL; sdp = sdp->next) {

    /* Do not translate if DB is PubMed */
    /* and the field is ALL.            */

    fieldInfo = FieldGetInfo (sdp->db, sdp->fld);
    dbName = DBGetNameFromID (sdp->db);
    
    if ((StrICmp(dbName, "PubMed") == 0) && 
         (StrICmp(fieldInfo->field_name, "ALL") == 0))
      doNotTranslate = doNotExplode; /* doNotTranslate = TRUE; */
    else
      doNotTranslate = doNotExplode;

    if (sdp->group == GROUP_SINGLE || sdp->group == GROUP_FIRST)
      group++;
    if (sdp->state == STATE_ON) {
      isEmpty = FALSE;

      /* Add opening parens at beginning */

      if (last == 0) {
        EntrezAddToBooleanRequest (e2RequestPtr, NULL, ENTREZ_OP_LEFT_PAREN,
                                   NULL, NULL, NULL, 0, 0, NULL, NULL,
                                   doNotExplode, doNotTranslate);
        EntrezAddToBooleanRequest (e2RequestPtr, NULL, ENTREZ_OP_LEFT_PAREN,
                                   NULL, NULL, NULL, 0, 0, NULL, NULL,
                                   doNotExplode, doNotTranslate);
      }

      /* Put an 'OR' operator between groups */

      else if (last == group) {
        EntrezAddToBooleanRequest (e2RequestPtr, NULL, ENTREZ_OP_OR,
                                   NULL, NULL, NULL, 0, 0, NULL, NULL,
                                   doNotExplode, doNotTranslate);
      }

      /* Put 'BUTNOT' operator where requested */

      else if (sdp->above == ENTREZ_OP_BUTNOT) {
        EntrezAddToBooleanRequest (e2RequestPtr, NULL, ENTREZ_OP_RIGHT_PAREN,
                                   NULL, NULL, NULL, 0, 0, NULL, NULL,
                                   doNotExplode, doNotTranslate);
        EntrezAddToBooleanRequest (e2RequestPtr, NULL, ENTREZ_OP_RIGHT_PAREN,
                                   NULL, NULL, NULL, 0, 0, NULL, NULL,
                                   doNotExplode, doNotTranslate);
        EntrezAddToBooleanRequest (e2RequestPtr, NULL, ENTREZ_OP_BUTNOT,
                                   NULL, NULL, NULL, 0, 0, NULL, NULL,
                                   doNotExplode, doNotTranslate);
        EntrezAddToBooleanRequest (e2RequestPtr, NULL, ENTREZ_OP_LEFT_PAREN,
                                   NULL, NULL, NULL, 0, 0, NULL, NULL,
                                   doNotExplode, doNotTranslate);
        EntrezAddToBooleanRequest (e2RequestPtr, NULL, ENTREZ_OP_LEFT_PAREN,
                                   NULL, NULL, NULL, 0, 0, NULL, NULL,
                                   doNotExplode, doNotTranslate);
      }

      /* Otherwise default operator is 'AND' */

      else {
        EntrezAddToBooleanRequest (e2RequestPtr, NULL, ENTREZ_OP_RIGHT_PAREN,
                                   NULL, NULL, NULL, 0, 0, NULL, NULL,
                                   doNotExplode, doNotTranslate);
        EntrezAddToBooleanRequest (e2RequestPtr, NULL, ENTREZ_OP_AND,
                                   NULL, NULL, NULL, 0, 0, NULL, NULL,
                                   doNotExplode, doNotTranslate);
        EntrezAddToBooleanRequest (e2RequestPtr, NULL, ENTREZ_OP_LEFT_PAREN,
                                   NULL, NULL, NULL, 0, 0, NULL, NULL,
                                   doNotExplode, doNotTranslate);
      }

      /* Add in the term itself */

      if (sdp->key == NULL) {

        /* If it is a range, then split out the to and from */

        if (StringChr (sdp->term, ':') != 0) {
          rangeStr = (CharPtr) MemNew (strlen (sdp->term) + 1);
          StringCpy (rangeStr, sdp->term);
          fromTerm = StringTokMT(rangeStr, ":", &rangeStr);
          toTerm = StringTokMT(rangeStr, ":", &rangeStr);
          if ((fromTerm != NULL) && (toTerm != NULL)) {
            EntrezAddToBooleanRequest (e2RequestPtr, NULL, 0, sdp->field,
                                       fromTerm, NULL, 0, 0, NULL, NULL,
                                       doNotExplode, doNotTranslate);
            EntrezAddToBooleanRequest (e2RequestPtr, NULL, ENTREZ_OP_RANGE,
                                       NULL, NULL, NULL, 0, 0, NULL, NULL,
                                       doNotExplode, doNotTranslate);
            EntrezAddToBooleanRequest (e2RequestPtr, NULL, 0, sdp->field,
                                       toTerm, NULL, 0, 0, NULL, NULL,
                                       doNotExplode, doNotTranslate);
          }
          MemFree (rangeStr);
        }
        else
          EntrezAddToBooleanRequest (e2RequestPtr, NULL, 0, sdp->field,
                                     sdp->term, NULL, 0, 0, NULL, NULL,
                                     doNotExplode, doNotTranslate);
      } else {
        EntrezAddToBooleanRequest (e2RequestPtr, NULL, 0, NULL, NULL,
                                   sdp->key, 0, 0, NULL, NULL,
                                   doNotExplode, doNotTranslate);
        EntrezSetUseHistoryFlag (e2RequestPtr);
      }
      last = group;
    }
  }

  if (isEmpty == TRUE) return FALSE;

  /* Add on the closing parens */

  if (group > 0 && last > 0) {
    EntrezAddToBooleanRequest (e2RequestPtr, NULL, ENTREZ_OP_RIGHT_PAREN,
                                   NULL, NULL, NULL, 0, 0, NULL, NULL,
                                   doNotExplode, doNotTranslate);
    EntrezAddToBooleanRequest (e2RequestPtr, NULL, ENTREZ_OP_RIGHT_PAREN,
                                   NULL, NULL, NULL, 0, 0, NULL, NULL,
                                   doNotExplode, doNotTranslate);
  }

  /* Return successfully */

  return TRUE;
}

/*==================================================================*/
/*                                                                  */
/*  Query_FetchUIDs () - Use the linked list of terms stored in     */
/*                      the TermFormPtr to generate a query that    */
/*                      returns all matching UIDs.                  */
/*                                                                  */
/*==================================================================*/

NLM_EXTERN Entrez2BooleanReplyPtr Query_FetchUIDs (ForM f)

{
  CharPtr                 dbName;
  Entrez2RequestPtr       e2RequestPtr;
  Entrez2ReplyPtr         e2ReplyPtr;
  Entrez2BooleanReplyPtr  e2BooleanPtr;
  FormInfoPtr             pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (f);

  /*------------------------------------*/
  /* Make sure that we have at hand all */
  /* the ingredients for the query.     */
  /*------------------------------------*/

  if (pFormInfo->termList == NULL || pFormInfo->chosenNumLines < 1 || pFormInfo->termList->db < 0) return 0;

  /*--------------------------------*/
  /* Get the name of the current DB */
  /*--------------------------------*/

  dbName = DBGetNameFromID (pFormInfo->currDb);
  if (StringHasNoText (dbName)) return 0;

  /*---------------------------------*/
  /* Create an empty Boolean request */
  /*---------------------------------*/

  e2RequestPtr = EntrezCreateBooleanRequest (TRUE, FALSE, dbName, NULL, 0, 0, NULL, 0, 0);
  if (e2RequestPtr == NULL) return 0;

  /*------------------------------------*/
  /* Convert the linked list of boolean */
  /* terms into a boolean request and   */
  /* send the request to the server.    */
  /*------------------------------------*/

  if (! TermlistToRequestCloseGroup (pFormInfo, e2RequestPtr)) return 0;

  if (ShowASN () == TRUE)
    DisplayEntrezRequest (e2RequestPtr);

  e2ReplyPtr = SpecialEntrezSynchronousQuery (e2RequestPtr);
  if (e2ReplyPtr == NULL) return 0;

  if (ShowASN () == TRUE)
    DisplayEntrezReply (e2ReplyPtr);

  /*----------------------------------*/
  /* Parse the count out of the reply */
  /*----------------------------------*/

  e2BooleanPtr = EntrezExtractBooleanReply (e2ReplyPtr);

  /*----------------------------------*/
  /* Clean up and return successfully */
  /*----------------------------------*/

  Entrez2RequestFree (e2RequestPtr);

  return e2BooleanPtr;
}

/*==================================================================*/
/*                                                                  */
/*  Query_FetchSeveralCounts () -                                   */
/*                                                                  */
/*==================================================================*/

NLM_EXTERN Entrez2TermListPtr Query_FetchSeveralCounts (CharPtr dbName, CharPtr fieldName, CharPtr searchStr, Int2 count)

{
  Entrez2RequestPtr   e2RequestPtr;
  Entrez2ReplyPtr     e2ReplyPtr;
  Entrez2TermListPtr  e2TermListPtr;
  Int4                termPos;

  /*-----------------------------------------*/
  /* Find the position of the requested term */
  /*-----------------------------------------*/

  e2RequestPtr = EntrezCreateGetTermPositionRequest (dbName, fieldName, searchStr);
  if (ShowASN () == TRUE)
    DisplayEntrezRequest (e2RequestPtr);

  e2ReplyPtr = SpecialEntrezSynchronousQuery (e2RequestPtr);

  if (ShowASN () == TRUE)
    DisplayEntrezReply (e2ReplyPtr);

  if (e2ReplyPtr == NULL) return NULL;

  termPos = EntrezExtractTermPosReply (e2ReplyPtr);

  /*---------------------------------*/
  /* Retrieve the requested term and */
  /* several following terms.        */
  /*---------------------------------*/

  e2RequestPtr = EntrezCreateGetTermListRequest (dbName, fieldName, termPos, count);
  if (ShowASN () == TRUE)
    DisplayEntrezRequest (e2RequestPtr);

  e2ReplyPtr = SpecialEntrezSynchronousQuery (e2RequestPtr);

  if (ShowASN () == TRUE)
    DisplayEntrezReply (e2ReplyPtr);

  if (e2ReplyPtr == NULL) return NULL;

  e2TermListPtr = EntrezExtractTermListReply (e2ReplyPtr);

  /*---------------------*/
  /* Return successfully */
  /*---------------------*/

  return e2TermListPtr;
}

/*==================================================================*/
/*                                                                  */
/*  Query_FetchCount () -                                           */
/*                                                                  */
/*==================================================================*/

NLM_EXTERN Int4 Query_FetchCount (ForM f)

{
  CharPtr                 dbName;
  Int4                    count;
  Entrez2RequestPtr       e2RequestPtr;
  Entrez2ReplyPtr         e2ReplyPtr;
  Entrez2BooleanReplyPtr  e2BooleanPtr;
  FormInfoPtr             pFormInfo;

  pFormInfo = (FormInfoPtr) GetObjectExtra (f);

  /*------------------------------------*/
  /* Make sure that we have at hand all */
  /* the ingredients for the query.     */
  /*------------------------------------*/

  if (pFormInfo->termList == NULL || pFormInfo->chosenNumLines < 1 || pFormInfo->termList->db < 0) return 0;

  /*--------------------------------*/
  /* Get the name of the current DB */
  /*--------------------------------*/

  dbName = DBGetNameFromID (pFormInfo->currDb);
  if (StringHasNoText (dbName)) return 0;

  /*---------------------------------*/
  /* Create an empty Boolean request */
  /*---------------------------------*/

  e2RequestPtr = EntrezCreateBooleanRequest (FALSE, FALSE, dbName, NULL, 0, 0, NULL, 0, 0);
  if (e2RequestPtr == NULL) return 0;

  /*------------------------------------*/
  /* Convert the linked list of boolean */
  /* terms into a boolean request and   */
  /* send the request to the server.    */
  /*------------------------------------*/

  if (! TermlistToRequestCloseGroup (pFormInfo, e2RequestPtr)) return 0;

  if (ShowASN () == TRUE)
    DisplayEntrezRequest (e2RequestPtr);

  e2ReplyPtr = SpecialEntrezSynchronousQuery (e2RequestPtr);

  if (ShowASN () == TRUE)
    DisplayEntrezReply (e2ReplyPtr);

  if (e2ReplyPtr == NULL) return 0;

  /*----------------------------------*/
  /* Parse the count out of the reply */
  /*----------------------------------*/

  e2BooleanPtr = EntrezExtractBooleanReply (e2ReplyPtr);
  if (e2BooleanPtr == NULL) return 0;

  count = e2BooleanPtr->count;

  /*----------------------------------*/
  /* Clean up and return successfully */
  /*----------------------------------*/

  Entrez2BooleanReplyFree (e2BooleanPtr);
  Entrez2RequestFree (e2RequestPtr);

  return count;
}

/*==================================================================*/
/*                                                                  */
/*  RemoveExtraQuotes () -                                          */
/*                                                                  */
/*==================================================================*/

static CharPtr RemoveExtraQuotes (CharPtr origString)

{
  Int2     charNum = 0;
  Int2     length;
  CharPtr  newString;
  Int2     newCharNum = 0;

  length = StringLen (origString);
  newString = MemNew (length + 1);
  for (charNum = 0; charNum < length; charNum++) {
    if (origString [charNum] != '"') {
      newString [newCharNum] = origString [charNum];
      newCharNum++;
    }
  }

  return newString;
}

/*==================================================================*/
/*                                                                  */
/*  Query_FetchParsedCount () -                                     */
/*                                                                  */
/*==================================================================*/

NLM_EXTERN Int4 Query_FetchParsedCount (ForM f)

{
  CharPtr                 dbName;
  Entrez2RequestPtr       e2RequestPtr;
  Entrez2ReplyPtr         e2ReplyPtr;
  Int4                    count;
  Entrez2BooleanReplyPtr  e2BooleanPtr;
  FormInfoPtr             pFormInfo;
  Entrez2BooleanTermPtr   tmpTerm;
  StateDataPtr            currentTerm;
  Boolean                 found;
  ValNodePtr              valNodeTermList;
  CharPtr                 cleanedUpTerm;

  pFormInfo = (FormInfoPtr) GetObjectExtra (f);

  /*------------------------------------*/
  /* Make sure that we have at hand all */
  /* the ingredients for the query.     */
  /*------------------------------------*/

  if (pFormInfo->termList == NULL ||
      pFormInfo->chosenNumLines < 1 ||
      pFormInfo->termList->db < 0)
    return -1;

  /*--------------------------------*/
  /* Get the name of the current DB */
  /*--------------------------------*/

  dbName = DBGetNameFromID (pFormInfo->currDb);
  if (StringHasNoText (dbName)) return -1;

  /*---------------------------------*/
  /* Create an empty Boolean request */
  /*---------------------------------*/

  e2RequestPtr = EntrezCreateBooleanRequest (FALSE, TRUE, dbName, NULL,
                                             0, 0, NULL, 0, 0);
  if (e2RequestPtr == NULL) return -1;

  /*------------------------------------*/
  /* Convert the linked list of boolean */
  /* terms into a boolean request and   */
  /* send the request to the server.    */
  /*------------------------------------*/

  if (! TermlistToRequestCloseGroup (pFormInfo, e2RequestPtr)) return -1;

  if (ShowASN () == TRUE)
    DisplayEntrezRequest (e2RequestPtr);

  e2ReplyPtr = SpecialEntrezSynchronousQuery (e2RequestPtr);

  if (ShowASN () == TRUE)
    DisplayEntrezReply (e2ReplyPtr);

  if (e2ReplyPtr == NULL) return -1;

  /*--------------------------------*/
  /* Parse the counts for each term */
  /* out of the reply.              */
  /*--------------------------------*/

  e2BooleanPtr = EntrezExtractBooleanReply (e2ReplyPtr);
  if (e2BooleanPtr == NULL) return -1;

  count = e2BooleanPtr->count;

  if (e2BooleanPtr->query != NULL) {
    valNodeTermList = e2BooleanPtr->query->exp;

    while (valNodeTermList != NULL) {
      if (valNodeTermList->choice == 3) {
        tmpTerm = (Entrez2BooleanTermPtr) valNodeTermList->data.ptrvalue;
        cleanedUpTerm = RemoveExtraQuotes (tmpTerm->term);
        StringCpy (tmpTerm->term, cleanedUpTerm);
        currentTerm = pFormInfo->termList;
        found = FALSE;
        while ((currentTerm != NULL) && (! found)) {
          if ((StrICmp (currentTerm->field, tmpTerm->field) == 0) &&
              (StrICmp (currentTerm->term, tmpTerm->term) == 0)) {
            currentTerm->count = tmpTerm->term_count;
            found = TRUE;
          }
          currentTerm = currentTerm->next;
        }
      }
      valNodeTermList = valNodeTermList->next;
    }
  }

  /*----------------------------------*/
  /* Clean up and return successfully */
  /*----------------------------------*/

  Entrez2BooleanReplyFree (e2BooleanPtr);
  Entrez2RequestFree (e2RequestPtr);

  return count;
}

/*==================================================================*/
/*                                                                  */
/*  Query_GetTranslatedCount () -                                   */
/*                                                                  */
/*==================================================================*/

static Int4 Query_GetTranslatedTermCount (FormInfoPtr pFormInfo, CharPtr dbName, CharPtr fieldName, CharPtr term)

{
  Char                displayStr [256];
  Entrez2RequestPtr   e2RequestPtr;
  Entrez2ReplyPtr     e2ReplyPtr;
  Entrez2TermListPtr  e2TermListPtr;
  Int2                row;
  Int4                termPos;
  Entrez2TermPtr      termPtr;

  /*-----------------------------------------*/
  /* Find the position of the requested term */
  /*-----------------------------------------*/

  e2RequestPtr = EntrezCreateGetTermPositionRequest (dbName, fieldName, term);

  if (ShowASN () == TRUE)
    DisplayEntrezRequest (e2RequestPtr);

  e2ReplyPtr = SpecialEntrezSynchronousQuery (e2RequestPtr);

  if (ShowASN () == TRUE)
    DisplayEntrezReply (e2ReplyPtr);

  if (e2ReplyPtr == NULL) return -1;

  termPos = EntrezExtractTermPosReply (e2ReplyPtr);

  /*---------------------------------*/
  /* Retrieve the requested term and */
  /* several following terms.        */
  /*---------------------------------*/

  e2RequestPtr = EntrezCreateGetTermListRequest (dbName, fieldName, termPos, AVAIL_WINDOW_ROWS);

  if (ShowASN () == TRUE)
    DisplayEntrezRequest (e2RequestPtr);

  e2ReplyPtr = SpecialEntrezSynchronousQuery (e2RequestPtr);

  if (ShowASN () == TRUE)
    DisplayEntrezReply (e2ReplyPtr);

  if (e2ReplyPtr == NULL) return -1;

  e2TermListPtr = EntrezExtractTermListReply (e2ReplyPtr);

  if (e2TermListPtr == NULL) return -1;

  pFormInfo->availItem = 0;
  pFormInfo->availRow = 0;
  Reset (pFormInfo->availDoc);
  SetDocCache (pFormInfo->availDoc, NULL, NULL, NULL);
  Update ();

  for (row = 1, termPtr = e2TermListPtr->list; row <= AVAIL_WINDOW_ROWS && termPtr != NULL; row++, termPtr = termPtr->next) {
    sprintf (displayStr, "%s\t%ld\t%d\n", termPtr->term, (long) (termPtr->count), (int) (termPtr->is_leaf_node ? 1 : 0));
    AppendText (pFormInfo->availDoc, displayStr, &availParFmt, availColFmt, systemFont);
  }

  InvalDocument (pFormInfo->availDoc);
  Update ();

  if (StringICmp (e2TermListPtr->list->term, term) == 0) return e2TermListPtr->list->count;

  return -1;
}

/*==================================================================*/
/*                                                                  */
/*  Query_TranslateAndAddBoolTerm () -                              */
/*                      Have the server translate a single term     */
/*                      into multiple terms, each with their own    */
/*                      count. Then add each of these terms to the  */
/*                      the termlist (in an OR'd group) and display */
/*                      them.                                       */
/*                                                                  */
/*==================================================================*/

static Boolean Query_TranslateAndAddBoolTerm (
  ForM    f,
  Int2    currDb,
  Int2    currFld,
  CharPtr strs,
  Int2    state,
  Int4    num
)

{
  StateDataPtr            sdp = NULL;
  Entrez2RequestPtr       e2RequestPtr;
  Entrez2ReplyPtr         e2ReplyPtr;
  Entrez2BooleanReplyPtr  e2BooleanPtr;
  CharPtr                 dbName;
  Entrez2BooleanTermPtr   tmpTerm;
  ValNodePtr              valNodeTermList;
  CharPtr                 cleanedUpTerm;
  Boolean                 firstTerm;
  StateDataPtr            prev = NULL;
  CharPtr                 fieldName;
  Int2                    fieldId;
  Entrez2FieldInfoPtr     fieldInfo;
  FormInfoPtr             pFormInfo;
  Int2                    nextOperator;
  Int2                    nextGroup;
  Int2                    tmpOp;
  Boolean                 allowDuplicates;
  Boolean                 doNotTranslate;

  pFormInfo = (FormInfoPtr) GetObjectExtra (f);

  /*-------------------------------------------*/
  /* Get the names of the current db and field */
  /*-------------------------------------------*/

  dbName = DBGetNameFromID (currDb);
  if (StringHasNoText (dbName)) return FALSE;

  fieldInfo = FieldGetInfo (currDb, currFld);
  if (fieldInfo == NULL) return FALSE;

  /*------------------------------------------*/
  /* Allow duplicates if the DB is PubMed and */
  /* the field is All.                        */
  /*------------------------------------------*/

  if ((StringICmp (dbName, "PubMed") == 0) &&
      (StringICmp (fieldInfo->field_name, "All") == 0))
    allowDuplicates = TRUE;
  else
    allowDuplicates = FALSE;

  /*-----------------------------------*/
  /* Create and send a boolean request */
  /* to return a translated list       */
  /* of terms for the given term.      */
  /*-----------------------------------*/

  doNotTranslate = TRUE;
  e2RequestPtr = EntrezCreateBooleanRequest (FALSE, TRUE, dbName, NULL, 0, 0, NULL, 0, 0);
  EntrezAddToBooleanRequest (e2RequestPtr, strs, 0, NULL, NULL, NULL, 0, 0, NULL, NULL, TRUE, doNotTranslate);
  if (ShowASN () == TRUE)
    DisplayEntrezRequest (e2RequestPtr);

  e2ReplyPtr = SpecialEntrezSynchronousQuery (e2RequestPtr);

  if (ShowASN () == TRUE)
    DisplayEntrezReply (e2ReplyPtr);

  if (e2ReplyPtr == NULL) return FALSE;

  e2BooleanPtr = EntrezExtractBooleanReply (e2ReplyPtr);
  if (e2BooleanPtr == NULL) return FALSE;

  if (e2BooleanPtr->query == NULL) return FALSE;

  /*------------------------------------------------*/
  /* Parse the resulting terms out of the query and */
  /* add them one at a time to the current list of  */
  /* terms and to the chosen window.                */
  /*------------------------------------------------*/

  valNodeTermList = e2BooleanPtr->query->exp;

  firstTerm = TRUE;
  nextGroup = GROUP_SINGLE;
  while (valNodeTermList != NULL) {
    if (valNodeTermList->choice == TERMLIST_OPERATOR) {
      tmpOp = (Int2) valNodeTermList->data.intvalue;
      switch (tmpOp) {
        case ENTREZ_OP_AND:
          if (sdp != NULL) {
            if (sdp->group == GROUP_MIDDLE) {
              sdp->group = GROUP_LAST;
            } else if (sdp->group == GROUP_FIRST) {
              sdp->group = GROUP_SINGLE;
            }
          }
          nextOperator = tmpOp;
          nextGroup = GROUP_SINGLE;
          break;
        case ENTREZ_OP_OR:
        case ENTREZ_OP_BUTNOT:
          nextOperator = tmpOp;
          break;
        case ENTREZ_OP_LEFT_PAREN:
          nextGroup = GROUP_FIRST;
          break;
        case ENTREZ_OP_RIGHT_PAREN:
          break;
      }
    } else if (valNodeTermList->choice == TERMLIST_TERM) {

      /*---------------------*/
      /* Parse the term info */
      /*---------------------*/

      tmpTerm = (Entrez2BooleanTermPtr) valNodeTermList->data.ptrvalue;
      cleanedUpTerm = RemoveExtraQuotes (tmpTerm->term);
      StringCpy (tmpTerm->term, cleanedUpTerm);
      if (IsValidFieldName (currDb, tmpTerm->field) == FALSE)
        fieldName = FieldGetNameFromMenuName (currDb, tmpTerm->field);
      else
        fieldName = tmpTerm->field;

      fieldId = FieldGetIDFromName (currDb, fieldName);

      /*-----------------------------------*/
      /* Replace -1 with actual term count */
      /*  Note -- this also creates the    */
      /*    desired 'flashing' effect in   */
      /*    the termlist window.           */
      /*-----------------------------------*/

      tmpTerm->term_count = Query_GetTranslatedTermCount (pFormInfo, dbName, fieldName, tmpTerm->term);

      /*-------------------------------*/
      /* Add the term to the term list */
      /*-------------------------------*/

      if (NULL != sdp)
        prev = sdp;
      sdp = CreateTerm (f, currDb, fieldId, tmpTerm->term, state,
                        tmpTerm->term_count, allowDuplicates);
      if (NULL == sdp) {
        valNodeTermList = valNodeTermList->next;
        sdp = prev;
        continue;
      }

      /*-------------------------------*/
      /* Make all the translated terms */
      /* an or'd group of terms.       */
      /*-------------------------------*/

      switch (nextGroup) {
        case GROUP_SINGLE:
          sdp->group = GROUP_SINGLE;
          break;
        case GROUP_FIRST:
          sdp->group = GROUP_FIRST;
          nextGroup = GROUP_MIDDLE;
          break;
        case GROUP_MIDDLE:
          sdp->group = GROUP_MIDDLE;
          break;
        case GROUP_LAST:
          sdp->group = GROUP_LAST;
          break;
      }

      if (firstTerm == TRUE) {
        prev = sdp->prev;
        if (prev != NULL) {
          sdp->above = ENTREZ_OP_AND;
          prev->below = ENTREZ_OP_AND;
        }
        firstTerm = FALSE;
      } else {
        prev = sdp->prev;
        sdp->above = nextOperator;
        prev->below = nextOperator;
      }

      /*--------------------------------------*/
      /* Display the term in the chosen panel */
      /*--------------------------------------*/

      DisplayTerm (pFormInfo, tmpTerm->term, fieldName, tmpTerm->term_count);
    }
    valNodeTermList = valNodeTermList->next;
  }

  /*---------------------------*/
  /* Mark the last term as the */
  /* last in the group.        */
  /*---------------------------*/

  if (NULL != sdp)
    if (sdp->group != GROUP_SINGLE) {
      if (sdp->group == GROUP_FIRST)
        sdp->group = GROUP_SINGLE;
      else
        sdp->group = GROUP_LAST;
    }

  /*--------------------------------*/
  /* Flatten out nested 'OR' groups */
  /*       (A OR (B OR C))          */
  /*   becomes                      */
  /*       (A OR B OR C)            */
  /*--------------------------------*/

  sdp = pFormInfo->termList;
  while (sdp != NULL) {
    if ((sdp->group == GROUP_FIRST) &&
        (sdp->above == ENTREZ_OP_OR) &&
        (sdp->below == ENTREZ_OP_OR))
      sdp->group = GROUP_MIDDLE;
    sdp = sdp->next;
  }

  /*------------------------*/
  /* Recalculate and redraw */
  /*------------------------*/

  AlphabetizeGroups (pFormInfo);
  RepopulateChosen (pFormInfo);
  RecalculateChosen (pFormInfo);

  /*---------------------*/
  /* Return successfully */
  /*---------------------*/

  return TRUE;
}

/*==================================================================*/
/*                                                                  */
/*  RefineUIDs () -                                                 */
/*                                                                  */
/*==================================================================*/

NLM_EXTERN Boolean RefineUIDs (ForM f, CharPtr term, Int4 num, Int4Ptr uids, Int2 db)

{
  CharPtr            advQueryStr;
  EnumFieldAssocPtr  alist;
  Entrez2InfoPtr     e2ip;
  FormInfoPtr        pFormInfo;
  Char               str [64];
  WindoW             tempPort;

  pFormInfo = (FormInfoPtr) GetObjectExtra (f);

  /*-------------------------------*/
  /* Set the query window settings */
  /*-------------------------------*/

  e2ip = Query_GetInfo ();
  if (e2ip == NULL) return FALSE;

  alist = CreateDatabaseAlist (e2ip);
  if (pFormInfo->currDb != db) {
    SetEnumPopup (pFormInfo->databasePopup, alist, (UIEnum) db);
    ChangeDatabase (pFormInfo->databasePopup);
  } else
    DoResetAvail (pFormInfo, TRUE);

  SafeSetTitle (pFormInfo->termText, "");
  SafeSetTitle (pFormInfo->fromText, "");
  SafeSetTitle (pFormInfo->toText, "");
  SafeDisable (pFormInfo->acceptButton);
  if (StringHasNoText (term))
    term = "*Current_Documents";
  if (term [0] == '*')
    term++;
  StringCpy (str, "*");
  StringNCpy (str + 1, term, sizeof (str) - 3);

  /*-----------------------------------*/
  /* Add the term to the current query */
  /* in the query window.              */
  /*-----------------------------------*/

  tempPort = SavePort (pFormInfo->chosenDoc);
  Query_AddUidsTerm (pFormInfo->form, str, num, uids, db);
  RecalculateChosen (pFormInfo);
  RestorePort (tempPort);

  if (! GetStatus (pFormInfo->advancedQueryItem)) {
    Show (pFormInfo->termText);
    Select (pFormInfo->termText);
  } else {
    advQueryStr = Query_ConvertToString (pFormInfo->form);
    SetAdvancedText (pFormInfo->advQueryText, advQueryStr);
    MemFree (advQueryStr);
    SafeShow (pFormInfo->advQueryGroup);
  }

  /*---------------------*/
  /* Return successfully */
  /*---------------------*/

  return TRUE;
}

