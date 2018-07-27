/*   pubdesc.c
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
* File Name:  pubdesc.c
*
* Author:  John Kuzio, Jonathan Kans
*
* Version Creation Date:   7/28/95
*
* $Revision: 6.23 $
*
* File Description:
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: pubdesc.c,v $
* Revision 6.23  2001/11/12 19:54:53  kans
* do not allocate year, protect against buffer overflow by -1 value
*
* Revision 6.22  2001/11/08 14:26:38  kans
* some protection in relaxed lookup, but server is not handling maxuid parameter in EntrezCreateBooleanRequest
*
* Revision 6.21  2001/10/23 18:57:38  kans
* EntrezAddToBooleanRequest has new key parameter
*
* Revision 6.20  2001/09/28 15:55:17  kans
* for docsum, extract first author, separate year from month
*
* Revision 6.19  2001/09/10 19:20:06  kans
* fixed for new entrez2 spec
*
* Revision 6.18  2001/08/03 23:01:50  kans
* import/export primary author dialog
*
* Revision 6.17  2001/05/30 15:10:53  kans
* removed medarch and medutil includes
*
* Revision 6.16  2001/05/29 16:31:11  kans
* added LaunchRelaxedQuery, written by Hanzhen Sun
*
* Revision 6.15  1999/05/10 23:13:26  kans
* separate lookup by muid and pmid in case both are present
*
* Revision 6.14  1999/05/06 19:00:58  kans
* enable lookup by pmid button
*
* Revision 6.13  1998/12/14 22:36:21  kans
* trim spaces around muid string
*
* Revision 6.12  1998/11/30 17:31:13  kans
* kludge to save authors if lookup by article
*
* Revision 6.11  1998/10/20 15:33:59  kans
* citsub now allows affil phone, fax, e-mail to be set
*
* Revision 6.10  1998/06/12 00:24:52  kans
* fixed problems detected by unix compiler
*
* Revision 6.9  1998/06/02 17:51:02  kans
* StdVibrantEditorMsgFunc responds to OM_MSG_DEL
*
* Revision 6.8  1998/04/13 21:07:22  kans
* PubdescAcceptFormButtonProc tries to copy ASN.1, on failure aborts attempted save
*
* Revision 6.7  1997/12/01 15:05:03  kuzio
* continuing bug fixes for proceedings citation
*
* Revision 6.6  1997/11/26 20:40:11  kuzio
* typo
*
* Revision 6.5  1997/11/26 15:49:25  kuzio
* 1st fix of proceedings entry bugs
*
* Revision 6.4  1997/11/13 21:33:50  kans
* implemented proceedings, proceedings chapter (Kuzio)
*
* Revision 6.3  1997/11/11 23:24:46  kans
* Import clears ppp->flagPubDelta
*
* Revision 6.2  1997/10/21 23:02:12  kans
* standard remark text popup for indexers
*
* Revision 6.1  1997/09/15 19:05:53  kans
* editors call ObjMgrAddUserData to support ItemAlreadyHasEditor
*
* Revision 6.0  1997/08/25 18:22:59  madden
* Revision changed to 6.0
*
* Revision 5.23  1997/08/01 15:07:20  kans
* ExtendGeneFeatIfOnMRNA only called by indexer version
*
* Revision 5.22  1997/07/30 20:27:31  kans
* use of ExtendGeneFeatIfOnMRNA
*
* Revision 5.21  1997/07/22 21:51:15  kans
* citbookptr was not being objtained properly from citartptr
*
* Revision 5.20  1997/07/14 13:17:51  kans
* CreateIntervalEditorDialogEx takes callback to synchronize feature partial checkbox to 5prime and 3prime partial boxes
*
* Revision 5.19  1997/05/05 15:51:35  kans
* no replace authors on new pub
*
 * Revision 5.18  1997/05/05  02:07:26  kans
 * replace authors button in initial form
 *
 * Revision 5.17  1997/05/02  21:27:32  kans
 * replace authors implemented
 *
 * Revision 5.16  1997/05/02  19:18:33  kans
 * calls SetClosestParentIfDuplicating
 *
 * Revision 5.15  1997/04/02  22:48:56  kans
 * changed title_rank to be iso-jta, ml-jta, jta, coden, issn, name, trans
 *
 * Revision 5.14  1997/04/02  21:19:43  kans
 * title_old set to 0 at beginning, so cit-art with lower ranked titles
 * coming after cit-gen with higher ranked titles don't override
 *
 * Revision 5.13  1997/01/29  19:33:43  kans
 * changed to StringNCpy_0
 *
 * Revision 5.12  1996/12/31  21:24:06  kans
 * calls GetRidOfEmptyFeatsDescStrings
 *
 * Revision 5.11  1996/12/12  21:20:19  kans
 * unpublished citations don't force current date if none set
 *
 * Revision 5.10  1996/12/04  18:17:05  kans
 * internal indexer version start up on journal page
 *
 * Revision 5.9  1996/11/01  20:56:00  kans
 * create dummy date in pub_sub
 *
 * Revision 5.8  1996/10/25  18:31:42  kans
 * one more place to make fake imp->date
 *
 * Revision 5.7  1996/09/12  13:25:43  kans
 * added cast to memset
 *
 * Revision 5.6  1996/09/05  16:23:50  kans
 * cit-sub date initialized to current date
 *
 * Revision 5.5  1996/07/18  19:46:11  kans
 * activate slot now in forms structure in vibforms.h
 *
 * Revision 5.4  1996/07/17  21:52:31  kans
 * SaveStringFromTextAndStripNewlines for saving from most scroll text boxes
 *
 * Revision 5.3  1996/07/17  20:53:58  kans
 * multiple citgen would miss "unpublished" indication
 *
 * Revision 5.2  1996/06/21  18:38:12  kans
 * allows switching of reftype in existing pub if descriptor
 *
 * Revision 5.1  1996/06/13  18:22:32  kans
 * GetATSub needed to check for csp->imp not NULL before double dereferencing
 *
 * Revision 5.0  1996/05/28  13:39:11  ostell
 * Set to revision 5.0
 *
 * Revision 1.47  1996/05/22  21:50:59  kans
 * changed OM_MSG_RET_OK to OM_MSG_RET_DONE
 *
 * Revision 1.46  1996/05/21  20:18:23  kans
 * don't read or write reftype on import/export
 *
 * Revision 1.45  1996/05/21  20:05:34  kans
 * implement import/export of pubdesc, seqloc
 *
 * Revision 1.44  1996/05/21  18:50:57  kans
 * put title page(s) before author page, preparation for import/export
 *
 * Revision 1.43  1996/04/24  19:23:27  kans
 * internal sequin always shows serial number control
 *
 * Revision 1.42  1996/04/09  21:37:10  kans
 * fixed thesis (Kuzio)
 *
 * Revision 1.41  1996/04/09  16:33:20  kans
 * added help scrolling, and new pub has accept button instead of replace
 *
 * Revision 1.40  1996/04/08  14:40:22  kans
 * added initial support for PubMed IDs (Kuzio)
 *
 * Revision 1.39  1996/03/21  16:06:18  kans
 * SetNewFeatureDefaultInterval now used
 *
 * Revision 1.38  1996/03/12  00:39:39  kans
 * added support for CitRetract in journal imprint
 *
 * Revision 1.37  1996/02/29  17:47:28  kans
 * internal sequin allows new submission citation to be added
 *
 * Revision 1.36  1996/02/15  23:03:43  kans
 * remove contact sub-dialog
 *
 * Revision 1.35  1996/02/14  18:15:48  kans
 * publication features don't have citations on feature requirement
 *
 * Revision 1.34  1996/02/07  16:15:20  kans
 * new pub disables buttons that would allow unimplemented feat/desc switch
 *
 * Revision 1.33  1996/01/24  22:11:46  kans
 * if no imp->date, it creates a dummy one to pass the object loader
 *
 * Revision 1.32  1996/01/24  21:46:30  kans
 * convert cap to cgp truncated and corrupted title
 *
 * Revision 1.31  1996/01/02  21:35:24  kans
 * changed Process to Proceed for initial form button title
 *
 * Revision 1.30  1996/01/02  19:48:35  kans
 * uses fallback message callback
 *
 * Revision 1.29  1996/01/02  17:15:31  kans
 * replaceThis is in the app property structure, newfeat reftype set properly
 *
 * Revision 1.28  1995/12/31  02:55:36  kans
 * REPLACE_THIS needed for separate replace all and replace this buttons
 *
 * Revision 1.27  1995/12/30  19:50:01  kans
 * message handler now uses switch statement
 *
 * Revision 1.26  1995/12/30  04:05:48  kans
 * editors handle cut/copy/paste/delete etc. messages from menu items
 *
 * Revision 1.25  1995/12/30  00:51:49  kans
 * init, enter, reset, redraw dialog/form uses message function
 *
 * Revision 1.24  1995/12/26  14:57:21  kans
 * added NULL clearProc parameter to CreateStdEditorFormMenus
 *
 * Revision 1.23  1995/12/25  21:48:12  kans
 * calls CreateStdEditorFormMenus
 *
 * Revision 1.22  1995/12/24  15:58:54  kans
 * SetSerialNumber was testing cgp->serial_number instead of serial_number
 *
 * Revision 1.21  1995/12/23  21:18:11  kans
 * moved MedArch pre- and post-processing into pubdesc
 *
 * Revision 1.20  1995/12/21  20:40:39  kans
 * accept buttons now update minimal cits on feats
 *
 * Revision 1.19  1995/12/20  22:22:37  kans
 * code center cleanup (Kuzio)
 *
 * Revision 1.18  1995/12/20  21:01:53  kans
 * support for remote MedArch services
 *
 * Revision 1.17  1995/12/20  20:22:08  kans
 * implements most of replace all function (except minimal cits on feats)
 *
 * Revision 1.16  1995/12/20  16:04:10  kans
 * Update called when switching folder tabs
 *
 * Revision 1.15  1995/12/19  22:16:57  kans
 * *** empty log message ***
 *
 * Revision 1.14  1995/12/15  00:51:53  kans
 * all author pages now use subfolders for names, affil, and contact
 *
 * Revision 1.13  1995/12/12  23:34:22  kans
 * uses extra folder tab appearance parameters
 *
 * Revision 1.12  1995/12/12  22:36:32  kans
 * cannot change feat/desc type of existing pubdesc (Kuzio)
 *
 * Revision 1.11  1995/12/09  01:55:16  kans
 * changes to cit-sub and cit-pat (the other JAK)
 *
 * Revision 1.10  1995/12/08  03:22:26  kans
 * second round of cosmetic changes
 *
 * Revision 1.9  1995/12/08  01:48:08  kans
 * first round of cosmetic changes
 *
 * Revision 1.8  1995/12/07  22:51:39  kans
 * maximum folder tab per line now set for all publication types
 *
 * Revision 1.7  1995/12/07  01:00:33  kans
 * new parameters to CreateFolderTabs
 *
 * Revision 1.6  1995/12/05  22:55:05  kans
 * major internal simplification (Kuzio)
 *
 * Revision 1.5  1995/12/02  23:36:38  kans
 * functional on existing features and descriptors
 *
 * Revision 1.4  1995/12/01  23:38:35  kans
 * *** empty log message ***
 *
 * Revision 1.3  1995/11/08  23:37:52  kans
 * edit block moved out of vibrant
 *
 * Revision 1.2  1995/09/25  21:13:26  kans
 * *** empty log message ***
 *
 * Revision 1.1  1995/07/28  18:01:31  kans
 * Initial revision
 *
*
* ==========================================================================
*/

#include <pubdesc.h>
#include <gather.h>
#include <utilpub.h>

#define FIRST_PAGE      0

#define PUB_UNPUB       0
#define PUB_JOURNAL     1
#define PUB_BOOKCHPTR   2
#define PUB_BOOK        3
#define PUB_THESIS      4
#define PUB_PROCCHPTR   5
#define PUB_PROC        6
#define PUB_PATENT      7
#define PUB_SUB         8

#define ART_JOURNAL     1
#define ART_BOOK        2
#define ART_PROC        3

#define CIT_BOOK        0
#define CIT_PROC        1
#define CIT_LET         2

#define NUM_TABS 12

typedef struct pubinitform {
  FEATURE_FORM_BLOCK

  SeqEntryPtr   sep;
  SeqFeatPtr    sfp;
  ValNodePtr    sdp;

  Boolean       flagPubDelta;
  Boolean       flagSerial;

  GrouP         pub_status;
  GrouP         pub_choice;
  GrouP         pub_reftype;
  Int2          pub_choice_init;
  Uint1         reftype;
} PubinitForm, PNTR PubinitFormPtr;

typedef struct pubdescform {
  FEATURE_FORM_BLOCK

  SeqEntryPtr   sep;

  GrouP         pages [NUM_TABS];
  DialoG        foldertabs;
  Int2          currentPage;
  Int2          tabnumber;
  Int2          pub_choice;
  Int2          Author_Page;
  Int2          Attribute_Page;
  Int2          Location_Page;
  Boolean       is_feat;

  Boolean       replaceAll;
} PubdescForm, PNTR PubdescFormPtr;

typedef struct pubdescpage {
  DIALOG_MESSAGE_BLOCK

  TexT          title_box;
  DialoG        author_list;
  DialoG        author_affil;
  AuthListPtr   originalAuthList;

  TexT          journal;
  TexT          volume;
  TexT          issue;

  TexT          year;
  PopuP         month;
  TexT          day;

  TexT          booktitle;
  DialoG        editor_list;
  DialoG        editor_affil;

  TexT          xa_info;
  DialoG        proc_affil;
  TexT          pryear;
  PopuP         prmonth;
  TexT          prday;

  DialoG        publisher;
  TexT          cpryear;
  PopuP         cprmonth;
  TexT          cprday;

  TexT          pages;

  GrouP         medium;
  TexT          comment;
  PopuP         retractType;
  GrouP         retractGrp;
  TexT          retractExp;

/* year/month - date of issue; cpryear/cprmonth - date of application */
  TexT          pat_country;
  TexT          pat_doc_type;
  TexT          pat_number;
  TexT          pat_app_number;
  DialoG        pat_applicant;
  DialoG        pat_app_affil;
  DialoG        pat_assignee;
  DialoG        pat_ass_affil;
  TexT          pat_abs;

  GrouP         AuthGroup[3];
  GrouP         EditGroup[3];
  GrouP         AppGroup[3];
  GrouP         AssGroup[3];

  Uint1         reftype;
  Uint1         pub_status;
  Int2          pub_choice;
  Boolean       flagPubDelta;

  TexT          pmid, muid, serial;

  PubEquivLookupProc  lookupArticle;
  LookupJournalProc   lookupJournal;
} PubdescPage, PNTR PubdescPagePtr;

/*
 * folder tab names
 * note: names have to be added to pubdescFormTabs array
 *       tabcounter has to reflect tab number
 */

static CharPtr a1[] = {
  "Title", "Authors", "Remark",
                   NULL, NULL, NULL};
static CharPtr a2[] = {
  "Title", "Authors", "Journal", "Remark",
                   NULL, NULL, NULL};
static CharPtr a3[] = {
  "Chapter Title", "Book Title", "Authors",
      "Editors", "Publisher", "Remark",
                   NULL, NULL, NULL};
static CharPtr a4[] = {
  "Book Title", "Authors", "Publisher", "Remark",
                   NULL, NULL, NULL};
static CharPtr a5[] = {
  "Title", "Authors", "Publisher", "Remark",
                   NULL, NULL, NULL};
static CharPtr a6[] = {
  "Abstract Tile", "Proceedings", "Presenters",
      "Chairpersons", "Locale", "Publisher", "Remark",
                   NULL, NULL, NULL};
static CharPtr a7[] = {
  "Proceedings", "Conveners", "Locale", "Publisher", "Remark",
                   NULL, NULL, NULL};
static CharPtr a8[] = {
  "Title", "Authors", "Patent", "Applicants", "Assignees",
      "Abstract", "Remark",
                   NULL, NULL, NULL};
static CharPtr a9[] = {
  "Description", "Authors", "Remark",
                   NULL, NULL, NULL};

static CharPtr PNTR pubdescFormTabs[] = {
  a1, a2, a3, a4, a5, a6, a7, a8, a9};

static Int2 tabcounter[] = {
   3,  4,  6,  4,  4,  7,  5,  7,  3};

static Int2 featTabsPerLine[] = {
   5,  6,  4,  6,  6,  5,  5,  5,  5};

static Int2 descTabsPerLine[] = {
   3,  4,  6,  4,  4,  4,  5,  4,  3};

static ValNodePtr NewPMuidFromText (TexT idtext, Int2 choice)
{
  ValNodePtr    idnode;
  Char          str[256];
  Int4          id;

  idnode = NULL;
  GetTitle (idtext, str, sizeof (str));
  TrimSpacesAroundString (str);
  if (str[0] != '\0')
  {
    StrToLong (str, &id);
    if (id > 0)
    {
      idnode = ValNodeNew (NULL);
      if (idnode != NULL)
      {
        idnode->choice = (Uint1) choice;
        idnode->data.intvalue = id;
      }
    }
  }
  return idnode;
}

static ValNodePtr NewSerialFromText (TexT serialtext)
{
  ValNodePtr    pubgennode;
  CitGenPtr     cgp;
  Char          str[256];
  Int2          serial;

/* this is just to keep serial numbers alive */
  pubgennode = NULL;
  GetTitle (serialtext, str, sizeof (str));
  if (str[0] != '\0')
  {
    StrToInt (str, &serial);
    if (serial > 0)
    {
      pubgennode = ValNodeNew (NULL);
      if (pubgennode != NULL)
      {
        pubgennode->choice = PUB_Gen;
        cgp = CitGenNew ();
        if (cgp != NULL)
        {
          pubgennode->data.ptrvalue = cgp;
          cgp->serial_number = serial;
        }
        else
        {
          pubgennode = ValNodeFree (pubgennode);
        }
      }
    }
  }
  return pubgennode;
}

static CitPatPtr PutATPat (PubdescPagePtr ppp)
{
  CitPatPtr     cpp;
  AuthListPtr   alp;

  cpp = NULL;
  if (ppp != NULL)
  {
    cpp = CitPatNew ();
    if (cpp != NULL)
    {
      cpp->title = SaveStringFromTextAndStripNewlines (ppp->title_box);
      alp = (AuthListPtr) DialogToPointer (ppp->author_list);
      if (alp != NULL)
      {
        alp->affil = DialogToPointer (ppp->author_affil);
      }
      cpp->authors = alp;
    }
  }
  return cpp;
}

static CitBookPtr PutATBook (PubdescPagePtr ppp)
{
  CitBookPtr    cbp;
  ValNodePtr    ttl;
  AuthListPtr   alp;
  ImprintPtr    imp;
  DatePtr       dp;

  cbp = NULL;
  if (ppp != NULL)
  {
    ttl = ValNodeNew (NULL);
    if (ttl != NULL)
    {
      ttl->choice = 1;              /* OhOh - assume name for book title */
      if (ppp->pub_choice == PUB_BOOK || ppp->pub_choice == PUB_BOOKCHPTR)
        ttl->data.ptrvalue = SaveStringFromTextAndStripNewlines (ppp->booktitle);
      else      /* thesis */
        ttl->data.ptrvalue = SaveStringFromTextAndStripNewlines (ppp->title_box);
    }

    if (ppp->pub_choice == PUB_BOOK || ppp->pub_choice == PUB_BOOKCHPTR)
      alp = (AuthListPtr) DialogToPointer (ppp->editor_list);
    else        /* thesis */
      alp = (AuthListPtr) DialogToPointer (ppp->author_list);
    if (alp != NULL)
    {
      if (ppp->pub_choice == PUB_BOOK || ppp->pub_choice == PUB_BOOKCHPTR)
        alp->affil = DialogToPointer (ppp->editor_affil);
      else      /* thesis */
        alp->affil = DialogToPointer (ppp->author_affil);
    }

    imp = ImprintNew ();
    if (imp != NULL)
    {
      imp->pub = DialogToPointer (ppp->publisher);
      imp->date = VibrantToDatePtr (ppp->month, ppp->day,
                                    ppp->year);
      if (imp->date == NULL) {
        dp = DateNew ();
        imp->date = dp;
        if (dp != NULL) {
          dp->data [0] = 0;
          dp->str = StringSave ("?");
        }
      }
      imp->cprt = VibrantToDatePtr (ppp->cprmonth, ppp->cprday,
                                    ppp->cpryear);
      if (ppp->pub_choice == PUB_BOOKCHPTR)
        imp->pages = SaveStringFromText (ppp->pages);
      imp->prepub = ppp->pub_status;
    }

    cbp = CitBookNew ();
    if (cbp != NULL)
    {
      cbp->title = ttl;
      cbp->authors = alp;
      cbp->othertype = CIT_BOOK;
      cbp->imp = imp;
    }
    else
    {
      if (ttl != NULL)
        ttl = ValNodeFree (ttl);
      if (alp != NULL)
        alp = AuthListFree (alp);
      if (imp != NULL)
        imp = ImprintFree (imp);
    }
  }
  return cbp;
}

static CitBookPtr PutATProc (PubdescPagePtr ppp)
{
  CitBookPtr    cbp;
  ValNodePtr    ttl;
  AuthListPtr   alp;
  ImprintPtr    imp;
  DatePtr       dp;
  AffilPtr      ap;
  ValNodePtr    vnphead, vnp;

  cbp = NULL;
  if (ppp != NULL)
  {
    ttl = ValNodeNew (NULL);
    if (ttl != NULL)
    {
      ttl->choice = 1;              /* OhOh - assume name for book title */
      ttl->data.ptrvalue = SaveStringFromTextAndStripNewlines (ppp->booktitle);
    }

    alp = (AuthListPtr) DialogToPointer (ppp->editor_list);
    if (alp == NULL)
    {
      alp = AuthListNew ();
    }
    if (alp != NULL)
    {
      alp->affil = DialogToPointer (ppp->editor_affil);
    }

    imp = ImprintNew ();
    if (imp != NULL)
    {
      imp->pub = DialogToPointer (ppp->publisher);
      imp->date = VibrantToDatePtr (ppp->month, ppp->day,
                                    ppp->year);
      if (imp->date == NULL) {
        dp = DateNew ();
        imp->date = dp;
        if (dp != NULL) {
          dp->data [0] = 0;
          dp->str = StringSave ("?");
        }
      }
/*
      imp->cprt = VibrantToDatePtr (ppp->cprmonth, ppp->cprday,
                                    ppp->cpryear);
*/
      if (ppp->pub_choice == PUB_PROCCHPTR)
        imp->pages = SaveStringFromText (ppp->pages);
      imp->prepub = ppp->pub_status;
    }

    vnphead = vnp = ValNodeNew (NULL);
    if (vnp != NULL)
    {
      vnp->choice = 1;
      vnp->data.ptrvalue = SaveStringFromTextAndStripNewlines (ppp->xa_info);
    }
    vnp = ValNodeNew (vnphead);
    if (vnp != NULL)
    {
      vnp->choice = 2;
      vnp->data.ptrvalue = (Pointer) VibrantToDatePtr (ppp->month, ppp->day,
                                                       ppp->year);
      if (vnp->data.ptrvalue == NULL) {
        dp = DateNew ();
        vnp->data.ptrvalue = (Pointer) dp;
        if (dp != NULL) {
          dp->data [0] = 0;
          dp->str = StringSave ("?");
        }
      }
    }
    ap = DialogToPointer (ppp->proc_affil);
    if (ap == NULL)
    {
      ap = AffilNew ();
    }
    vnp = ValNodeNew (vnphead);
    if (vnp != NULL)
    {
      vnp->choice = 3;
      vnp->data.ptrvalue = (Pointer) ap;
    }

    cbp = CitBookNew ();
    if (cbp != NULL)
    {
      cbp->title = ttl;
      cbp->authors = alp;
      cbp->othertype = CIT_PROC;
      cbp->imp = imp;
      cbp->otherdata = (Pointer) vnphead;
    }
    else
    {
      if (ttl != NULL)
        ttl = ValNodeFree (ttl);
      if (alp != NULL)
        alp = AuthListFree (alp);
      if (vnphead != NULL)
        vnphead = ValNodeFree (vnphead);
    }
  }
  return cbp;
}

static CitArtPtr PutATArt (ValNodePtr vnp, PubdescPagePtr ppp)
{
  CitArtPtr     cap;
  ValNodePtr    ttl;
  AuthListPtr   alp;

  cap = NULL;
  if (vnp != NULL && ppp != NULL)
  {
    cap = CitArtNew ();
    if (cap != NULL)
    {
      vnp->data.ptrvalue = cap;
      ttl = ValNodeNew (NULL);
      if (ttl != NULL)
      {
        cap->title = ttl;
        ttl->choice = 1;      /* OhOh - assume name */
        ttl->data.ptrvalue = SaveStringFromTextAndStripNewlines (ppp->title_box);
      }
      alp = (AuthListPtr) DialogToPointer (ppp->author_list);
      if (alp != NULL)
      {
        alp->affil = DialogToPointer (ppp->author_affil);
      }
      cap->authors = alp;
    }
  }
  return cap;
}

static Pointer PubdescPageToPubdescPtr (DialoG d)
{
  DatePtr               dp;
  PubdescPtr            pdp;
  PubdescPagePtr        ppp;
  ValNodePtr            vnp, vnpt, ttl;
  CitArtPtr             cap;
  CitBookPtr            cbp;
  CitGenPtr             cgp;
  CitJourPtr            cjp;
  CitPatPtr             cpp;
  CitSubPtr             csp;
  AuthListPtr           alp;
  ImprintPtr            imp;

  Char                  str[256];
  Int2                  serial;
  Int2                  val;
  CitRetractPtr         crp;

  pdp = NULL;
  ppp = (PubdescPagePtr) GetObjectExtra (d);
  if (ppp != NULL)
  {
    pdp = PubdescNew ();
    if (pdp != NULL)
    {
/* reftype - desc, feat, site */
      pdp->reftype = ppp->reftype;
/* comments */
      pdp->comment = SaveStringFromTextAndStripNewlines (ppp->comment);
      vnp = ValNodeNew (NULL);
      pdp->pub = vnp;
      switch (ppp->pub_choice)
      {
      case PUB_UNPUB:
        cgp = CitGenNew ();
        if (cgp != NULL)
        {
          vnp->choice = PUB_Gen;
          vnp->data.ptrvalue = cgp;
          cgp->cit = StringSave ("unpublished");
          alp = (AuthListPtr) DialogToPointer (ppp->author_list);
          if (alp != NULL)
          {
            alp->affil = DialogToPointer (ppp->author_affil);
          }
          cgp->authors = alp;
          cgp->title = SaveStringFromTextAndStripNewlines (ppp->title_box);
          cgp->date = VibrantToDatePtr (ppp->month, ppp->day, ppp->year);
          GetTitle (ppp->serial, str, sizeof (str));
          if (str[0] != '\0')
          {
            StrToInt (str, &serial);
            if (serial > 0)
            {
              cgp->serial_number = serial;
            }
          }
        }
        break;
      case PUB_JOURNAL:
        cap = PutATArt (vnp, ppp);
        if (cap != NULL)
        {
          vnp->choice = PUB_Article;
          cjp = CitJourNew ();
          if (cjp != NULL)
          {
            cap->from = ART_JOURNAL;
            cap->fromptr = cjp;
            ttl = ValNodeNew (NULL);
            if (ttl != NULL)
            {
              cjp->title = ttl;
              ttl->choice = 5;      /* OhOh - assume ISO_JTA for journal */
              ttl->data.ptrvalue = SaveStringFromText (ppp->journal);
            }
            imp = ImprintNew ();
            if (imp != NULL)
            {
              cjp->imp = imp;
              imp->volume = SaveStringFromText (ppp->volume);
              imp->issue = SaveStringFromText (ppp->issue);
              imp->pages = SaveStringFromText (ppp->pages);
              imp->date = VibrantToDatePtr
                          (ppp->month, ppp->day, ppp->year);
              if (imp->date == NULL) {
                dp = DateNew ();      /* Kludge to allow lookup by muid */
                imp->date = dp;
                if (dp != NULL) {
                  dp->data [0] = 0;
                  dp->str = StringSave ("?");
                }
              }
              imp->prepub = ppp->pub_status;
              val = GetValue (ppp->retractType);
              if (val > 1) {
                crp = CitRetractNew ();
                if (crp != NULL) {
                  crp->type = (Uint1) (val - 1);
                  crp->exp = SaveStringFromText (ppp->retractExp);
                }
                imp->retract = crp;
              }
            }
          }
        }
        break;
/* note: for Cit-book's there is cbp->otherdata which could be ValNodes */
      case PUB_BOOKCHPTR:
        cap = PutATArt (vnp, ppp);
        if (cap != NULL)
        {
          vnp->choice = PUB_Article;
          cbp = PutATBook (ppp);
          if (cbp != NULL)
          {
            cap->from = ART_BOOK;
            cap->fromptr = (Pointer) cbp;
          }
        }
        break;
      case PUB_BOOK:
        cbp = PutATBook (ppp);
        if (cbp != NULL)
        {
          vnp->choice = PUB_Book;
          vnp->data.ptrvalue = (Pointer) cbp;
        }
        break;
      case PUB_PROCCHPTR:
        cap = PutATArt (vnp, ppp);
        if (cap != NULL)
        {
          vnp->choice = PUB_Article;
          cbp = PutATProc (ppp);
          if (cbp != NULL)
          {
            cap->from = ART_PROC;
            cap->fromptr = (Pointer) cbp;
          }
        }
        break;
      case PUB_PROC:
        cbp = PutATProc (ppp);
        if (cbp != NULL)
        {
          vnp->choice = PUB_Proc;
          vnp->data.ptrvalue = (Pointer) cbp;
        }
        break;
      case PUB_THESIS:
        cbp = PutATBook (ppp);
        if (cbp != NULL)
        {
          vnp->choice = PUB_Man;
          vnp->data.ptrvalue = (Pointer) cbp;
          cbp->othertype = CIT_LET;   /* 2=Cit-let */
          cbp->let_type = 3;          /* 3=thesis */
        }
        break;
      case PUB_PATENT:
        cpp = PutATPat (ppp);
        if (cpp != NULL)
        {
          vnp->choice = PUB_Patent;
          vnp->data.ptrvalue = (Pointer) cpp;

          cpp->country = SaveStringFromText (ppp->pat_country);
          cpp->doc_type = SaveStringFromText (ppp->pat_doc_type);
          cpp->number = SaveStringFromText (ppp->pat_number);
          cpp->date_issue = VibrantToDatePtr
                            (ppp->month, ppp->day, ppp->year);
          cpp->app_number = SaveStringFromText (ppp->pat_app_number);
          cpp->app_date = VibrantToDatePtr
                          (ppp->cprmonth, ppp->cprday, ppp->cpryear);

          alp = (AuthListPtr) DialogToPointer (ppp->pat_applicant);
          cpp->applicants = alp;
          if (alp != NULL)
          {
            alp->affil = DialogToPointer (ppp->pat_app_affil);
          }
          alp = (AuthListPtr) DialogToPointer (ppp->pat_assignee);
          cpp->assignees = alp;
          if (alp != NULL)
          {
            alp->affil = DialogToPointer (ppp->pat_ass_affil);
          }
          cpp->abstract = SaveStringFromTextAndStripNewlines (ppp->pat_abs);
        }
        break;
      case PUB_SUB:
        csp = CitSubNew ();
        if (csp != NULL)
        {
          vnp->choice = PUB_Sub;
          vnp->data.ptrvalue = csp;
          alp = (AuthListPtr) DialogToPointer (ppp->author_list);
          if (alp != NULL)
          {
            alp->affil = DialogToPointer (ppp->author_affil);
          }
          csp->authors = alp;
          csp->descr = SaveStringFromTextAndStripNewlines (ppp->title_box);
          csp->date = VibrantToDatePtr (ppp->month, ppp->day, ppp->year);
          if (csp->date == NULL) {
                dp = DateNew ();
                csp->date = dp;
                if (dp != NULL) {
                  dp->data [0] = 0;
                  dp->str = StringSave ("?");
                }
          }

          csp->medium = (Uint1) GetValue (ppp->medium);
          if (csp->medium == 5)
            csp->medium = 255;        /* 255 = other */
        }
        break;
      default:
        break;
      } /* end switch (pub_choice) */

/* pubmed */
      vnpt = NewPMuidFromText (ppp->pmid, PUB_PMid);
      if (vnpt != NULL)
      {
        vnp->next = vnpt;
        vnp = vnpt;
      }
/* medline */
      vnpt = NewPMuidFromText (ppp->muid, PUB_Muid);
      if (vnpt != NULL)
      {
        vnp->next = vnpt;
        vnp = vnpt;
      }

      if (ppp->pub_choice != PUB_UNPUB)
      {
        vnpt = NewSerialFromText (ppp->serial);
        if (vnpt != NULL)
        {
          vnp->next = vnpt;
          vnp = vnpt;
        }
      }
    }
  }
  return (Pointer) pdp;
}

static void GetATSub (CitSubPtr csp, PubdescPagePtr ppp)
{
  AuthListPtr   alp;
  CharPtr       tmp;

  if (csp != NULL && ppp != NULL)
  {
    tmp = StringSave (csp->descr);
    SetTitle (ppp->title_box, tmp);
    MemFree (tmp);
    alp = csp->authors;
    if (alp != NULL)
    {
      PointerToDialog (ppp->author_list, (Pointer) alp);
      ppp->originalAuthList = AuthListFree (ppp->originalAuthList);
      ppp->originalAuthList = AsnIoMemCopy (alp,
                                            (AsnReadFunc) AuthListAsnRead,
                                            (AsnWriteFunc) AuthListAsnWrite);
      if (alp->affil != NULL)
      {
        PointerToDialog (ppp->author_affil, (Pointer) alp->affil);
      }
      else if (csp->imp != NULL)      /* go fishing */
      {
        PointerToDialog (ppp->author_affil, (Pointer) csp->imp->pub);
      }
    }
  }
}

static void GetATPat (CitPatPtr cpp, PubdescPagePtr ppp)
{
  AuthListPtr   alp;
  CharPtr       tmp;

  if (cpp != NULL && ppp != NULL)
  {
    tmp = StringSave (cpp->title);
    SetTitle (ppp->title_box, tmp);
    MemFree (tmp);
    alp = cpp->authors;
    if (alp != NULL)
    {
      PointerToDialog (ppp->author_list, (Pointer) alp);
      ppp->originalAuthList = AuthListFree (ppp->originalAuthList);
      ppp->originalAuthList = AsnIoMemCopy (alp,
                                            (AsnReadFunc) AuthListAsnRead,
                                            (AsnWriteFunc) AuthListAsnWrite);
      PointerToDialog (ppp->author_affil, (Pointer) alp->affil);
    }
  }
}

static void GetATBook (CitBookPtr cbp, PubdescPagePtr ppp)
{
  ValNodePtr    ttl;
  AuthListPtr   alp;
  ImprintPtr    imp;

  CharPtr       tmp;

  if (cbp != NULL && ppp != NULL)
  {
    ttl = cbp->title;
    while (ttl != NULL)
    {
      if (ttl->choice == 1 || ttl->choice == 3)
      {
        tmp = StringSave ((CharPtr) ttl->data.ptrvalue);
        if (ppp->pub_choice != PUB_THESIS)
          SetTitle (ppp->booktitle, tmp);
        else
          SetTitle (ppp->title_box, tmp);
        MemFree (tmp);
        break;
      }
      ttl = ttl->next;
    }
    alp = cbp->authors;
    if (alp != NULL)
    {
      if (ppp->pub_choice != PUB_THESIS)
      {
        PointerToDialog (ppp->editor_list, (Pointer) alp);
        PointerToDialog (ppp->editor_affil, (Pointer) alp->affil);
      }
      else
      {
        PointerToDialog (ppp->author_list, (Pointer) alp);
        ppp->originalAuthList = AuthListFree (ppp->originalAuthList);
        ppp->originalAuthList = AsnIoMemCopy (alp,
                                              (AsnReadFunc) AuthListAsnRead,
                                              (AsnWriteFunc) AuthListAsnWrite);
        PointerToDialog (ppp->author_affil, (Pointer) alp->affil);
      }
    }
    imp = cbp->imp;
    if (imp != NULL)
    {
      PointerToDialog (ppp->publisher, (Pointer) imp->pub);
      SetTitle ((TexT) ppp->pages, (CharPtr) imp->pages);
      DatePtrToVibrant (imp->date,
                        ppp->month, ppp->day, ppp->year);
      DatePtrToVibrant (imp->cprt,
                        ppp->cprmonth, ppp->cprday, ppp->cpryear);
    }
  }
}

static void GetATProc (CitBookPtr cbp, PubdescPagePtr ppp)
{
  ValNodePtr    ttl;
  AuthListPtr   alp;
  ImprintPtr    imp;
  ValNodePtr    vnp;

  CharPtr       tmp;

  if (cbp != NULL && ppp != NULL)
  {
    ttl = cbp->title;
    while (ttl != NULL)
    {
      if (ttl->choice == 1 || ttl->choice == 3)
      {
        tmp = StringSave ((CharPtr) ttl->data.ptrvalue);
        SetTitle (ppp->booktitle, tmp);
        MemFree (tmp);
        break;
      }
      ttl = ttl->next;
    }
    alp = cbp->authors;
    if (alp != NULL)
    {
      PointerToDialog (ppp->editor_list, (Pointer) alp);
      PointerToDialog (ppp->editor_affil, (Pointer) alp->affil);
    }
    imp = cbp->imp;
    if (imp != NULL)
    {
      PointerToDialog (ppp->publisher, (Pointer) imp->pub);
      SetTitle ((TexT) ppp->pages, (CharPtr) imp->pages);
      DatePtrToVibrant (imp->date,
                        ppp->month, ppp->day, ppp->year);
    }
    vnp = (ValNodePtr) cbp->otherdata;
    while (vnp != NULL)
    {
      switch (vnp->choice)
      {
       case 1:
        SetTitle ((TexT) ppp->xa_info, (CharPtr) vnp->data.ptrvalue);
        break;
       case 2:
        DatePtrToVibrant ((DatePtr) vnp->data.ptrvalue,
                          ppp->month, ppp->day, ppp->year);
        break;
       case 3:
        PointerToDialog (ppp->proc_affil, (Pointer) vnp->data.ptrvalue);
        break;
       default:
        break;
      }
      vnp = vnp->next;
    }
  }
}

static void GetATArt (CitArtPtr cap, PubdescPagePtr ppp)
{
  ValNodePtr    ttl;
  AuthListPtr   alp;

  CharPtr       tmp;

  if (cap != NULL && ppp != NULL)
  {
    ttl = cap->title;
    while (ttl != NULL)
    {
      if (ttl->choice == 1 || ttl->choice == 3)
      {
        tmp = StringSave ((CharPtr) ttl->data.ptrvalue);
        SetTitle (ppp->title_box, tmp);
        MemFree (tmp);
        break;
      }
      ttl = ttl->next;
    }
    alp = cap->authors;
    if (alp != NULL)
    {
      PointerToDialog (ppp->author_list, (Pointer) alp);
      ppp->originalAuthList = AuthListFree (ppp->originalAuthList);
      ppp->originalAuthList = AsnIoMemCopy (alp,
                                            (AsnReadFunc) AuthListAsnRead,
                                            (AsnWriteFunc) AuthListAsnWrite);
      PointerToDialog (ppp->author_affil, (Pointer) alp->affil);
    }
  }
}

static void PubdescPtrToPubdescPage (DialoG d, Pointer data)
{
  PubdescPtr            pdp;
  PubdescPagePtr        ppp;

  DatePtr       dp;

  ValNodePtr    vnp, ttl;
  AuthListPtr   alp;
  CitArtPtr     cap;
  CitBookPtr    cbp;
  CitGenPtr     cgp;
  CitJourPtr    cjp;
  CitPatPtr     cpp;
  CitSubPtr     csp;
  ImprintPtr    imp;
  Int2          title_new, title_old;
  Int2          title_rank[9];
/* codecenter fix */
/*  Int2          title_rank[] = {0, 2, 0, 1, 6, 7, 5, 4, 3}; */
/*   title spec                   0  1  2  3  4  5  6  7  8  */
  Int4          muid, pmid;
  Char          str[256];
  Uint1         medium;

  Boolean       flagGoodPub;

  CitRetractPtr  crp;

/* codecenter fix */
  title_rank[0] = 0;
  title_rank[1] = 2;
  title_rank[2] = 0;
  title_rank[3] = 1;
  title_rank[4] = 5;
  title_rank[5] = 7;
  title_rank[6] = 6;
  title_rank[7] = 4;
  title_rank[8] = 3;

  ppp = (PubdescPagePtr) GetObjectExtra (d);
  pdp = (PubdescPtr) data;
  title_old = 0;
  if (ppp != NULL)
  {
    if (pdp != NULL)
    {
      vnp = pdp->pub;
      flagGoodPub = FALSE;
      while (vnp != NULL)
      {
        switch (vnp->choice)
        {
/*
 * the strategy here is:
 * if CitGen then check if things like Author/Title are filled
 * if they are empty, fill them
 * if a CitArt/CitBook/etc are found these will be filled regardless
 * this will move towards getting rid of CitGen except for "unpublished"
 * same strategy for MUIDs buried in a CitGen
 * if a separate CitMuid is found it will overwrite any CitGen Muid
 * if a separate CitMuid is found first, the CitGen Muid will be ignored
 */
          case PUB_Gen:
            cgp = (CitGenPtr) vnp->data.ptrvalue;
            if (cgp != NULL)
            {
              if (!flagGoodPub)
              {
/* authors ? */
                alp = cgp->authors;
                if (alp != NULL)
                {
                  PointerToDialog (ppp->author_list, (Pointer) alp);
                  ppp->originalAuthList = AuthListFree (ppp->originalAuthList);
                  ppp->originalAuthList = AsnIoMemCopy (alp,
                                            (AsnReadFunc) AuthListAsnRead,
                                            (AsnWriteFunc) AuthListAsnWrite);
                  PointerToDialog (ppp->author_affil, (Pointer) alp->affil);
                }
/* title ? */
                SetTitle (ppp->title_box, cgp->title);
/* somekind of publication ? */
                if (cgp->journal != NULL)
                {
                  ttl = cgp->journal;
                  while (ttl != NULL)
                  {
                    if ((ttl->choice == 1) ||
                     (ttl->choice > 2 && ttl->choice < 9))
                    {
                      title_new = title_rank[ttl->choice];
                      if (title_new > title_old)
                      {
                        title_old = title_new;
                        SetTitle (ppp->journal,
                                  (CharPtr) ttl->data.ptrvalue);
                      }
                    }
                    ttl = ttl->next;
                  }
                }
                dp = cgp->date;
                if (dp != NULL)
                {
                  DatePtrToVibrant (dp, ppp->month, ppp->day, ppp->year);
                }
              }
/* pubmed uid */
              GetTitle (ppp->pmid, str, sizeof (str));
              if (str[0] == '\0')
              {
                pmid = cgp->pmid;
                if (pmid > 0)
                {
                  LongToStr (pmid, str, 0, sizeof (str));
                  SetTitle (ppp->pmid, str);
                }
              }
/* medline uid */
              GetTitle (ppp->muid, str, sizeof (str));
              if (str[0] == '\0')
              {
                muid = cgp->muid;
                if (muid > 0)
                {
                  LongToStr (muid, str, 0, sizeof (str));
                  SetTitle (ppp->muid, str);
                }
              }
/* always pull a serial number if it is there */
              if (cgp->serial_number > 0)
              {
                IntToStr (cgp->serial_number, str, 0, sizeof (str));
                SetTitle (ppp->serial, str);
              }
            }
            break;
          case PUB_Sub:
            csp = (CitSubPtr) vnp->data.ptrvalue;
            if (csp != NULL)
            {
              flagGoodPub = TRUE;
              GetATSub (csp, ppp);
              dp = NULL;
              if (csp->imp != NULL)     /* WARNING: imp is obsolete */
                dp = csp->imp->date;
              if (dp == NULL)
                dp = csp->date;
              if (dp != NULL)
              {
                DatePtrToVibrant (dp, ppp->month, ppp->day, ppp->year);
              }
              medium = csp->medium;
              if (medium == 0 || medium == 255)
                medium = 5;
              SetValue (ppp->medium, medium);
            }
            break;
          case PUB_PMid:
            pmid = vnp->data.intvalue;
            if (pmid > 0)
            {
              LongToStr (pmid, str, 0, sizeof (str));
              SetTitle (ppp->pmid, str);
            }
            break;
          case PUB_Muid:
            muid = vnp->data.intvalue;
            if (muid > 0)
            {
              LongToStr (muid, str, 0, sizeof (str));
              SetTitle (ppp->muid, str);
            }
            break;
          case PUB_Article:
            cap = (CitArtPtr) vnp->data.ptrvalue;
            if (cap != NULL)
            {
              flagGoodPub = TRUE;
              GetATArt (cap, ppp);
              if (ppp->flagPubDelta)
                break;
              switch (cap->from)
              {
                case ART_JOURNAL:
                  cjp = (CitJourPtr) cap->fromptr;
                  ttl = cjp->title;
                  while (ttl != NULL)
                  {
                    if ((ttl->choice == 1) ||
                     (ttl->choice > 2 && ttl->choice < 9))
                    {
                      title_new = title_rank[ttl->choice];
                      if (title_new > title_old)
                      {
                        title_old = title_new;
                        SetTitle (ppp->journal, (CharPtr) ttl->data.ptrvalue);
                      }
                    }
                    ttl = ttl->next;
                  }
                  imp = cjp->imp;
                  if (imp != NULL)
                  {
                    SetTitle ((TexT) ppp->volume, (CharPtr) imp->volume);
                    SetTitle ((TexT) ppp->issue, (CharPtr) imp->issue);
                    SetTitle ((TexT) ppp->pages, (CharPtr) imp->pages);
                    dp = imp->date;
                    if (dp != NULL)
                    {
                      DatePtrToVibrant (dp, ppp->month, ppp->day, ppp->year);
                    }
                    crp = imp->retract;
                    if (crp != NULL) {
                      SetValue (ppp->retractType, crp->type + 1);
                      SetTitle (ppp->retractExp, crp->exp);
                      SafeEnable (ppp->retractExp);
                    }
                  }
                  break;
                case ART_BOOK:
                  cbp = (CitBookPtr) cap->fromptr;
                  if (cbp != NULL)
                  {
                    GetATBook (cbp, ppp);       /* book chapter */
                  }
                  break;
                case ART_PROC:
                  cbp = (CitBookPtr) cap->fromptr;
                  if (cbp != NULL)
                  {
                    GetATProc (cbp, ppp);       /* proceedings chapter */
                  }
                  break;
                default:
                  break;
              }
            }
            break;
          case PUB_Book:
            cbp = (CitBookPtr) vnp->data.ptrvalue;
            if (cbp != NULL)
            {
              flagGoodPub = TRUE;
              GetATBook (cbp, ppp);     /* book */
            }
            break;
          case PUB_Proc:
            cbp = (CitBookPtr) vnp->data.ptrvalue;
            if (cbp != NULL)
            {
              flagGoodPub = TRUE;
              GetATProc (cbp, ppp);     /* proceedings */
            }
            break;
          case PUB_Man:
            cbp = (CitBookPtr) vnp->data.ptrvalue;
            if (cbp != NULL)
            {
              if (cbp->othertype == 2)    /* 2=Cit-let */
              {
                if (cbp->let_type == 3)   /* 3=thesis */
                {
                  flagGoodPub = TRUE;
                  GetATBook (cbp, ppp);
                }
              }
            }
            break;
          case PUB_Patent:
            cpp = (CitPatPtr) vnp->data.ptrvalue;
            if (cpp != NULL)
            {
              flagGoodPub = TRUE;
              GetATPat (cpp, ppp);
              SetTitle (ppp->pat_country, cpp->country);
              SetTitle (ppp->pat_doc_type, cpp->doc_type);
              SetTitle (ppp->pat_number, cpp->number);
              SetTitle (ppp->pat_app_number, cpp->app_number);
              dp = cpp->date_issue;
              if (dp != NULL)
              {
                DatePtrToVibrant (dp, ppp->month, ppp->day, ppp->year);
              }
              dp = cpp->app_date;
              if (dp != NULL)
              {
                DatePtrToVibrant (dp, ppp->cprmonth,
                                  ppp->cprday, ppp->cpryear);
              }
              alp = cpp->applicants;
              if (alp != NULL)
              {
                PointerToDialog (ppp->pat_applicant, (Pointer) alp);
                PointerToDialog (ppp->pat_app_affil, (Pointer) alp->affil);
              }
              alp = cpp->assignees;
              if (alp != NULL)
              {
                PointerToDialog (ppp->pat_assignee, (Pointer) alp);
                PointerToDialog (ppp->pat_ass_affil, (Pointer) alp->affil);
              }
              SetTitle (ppp->pat_abs, cpp->abstract);
            }
            break;
          default:
            break;
        }
        vnp = vnp->next;
      }
      if (!ppp->flagPubDelta)
        SetTitle (ppp->comment, pdp->comment);
    } /* end if (pdp) */
  } /* end if (ppp) */
}

static GrouP CreateBookPublisherPage (GrouP m, PubdescPagePtr ppp,
                                      Uint1 pchoice, Boolean flagSerial)
{
  GrouP pg, g1, g2;

  pg = NULL;
  if (ppp != NULL)
  {
    pg = HiddenGroup (m, -1, 0, NULL);
    if (pg != NULL)
    {
      g1 = HiddenGroup (pg, -1, 0, NULL);
      SetGroupSpacing (g1, 10, 10);
      ppp->publisher = CreatePublisherAffilDialog (g1, NULL);
      g2 = HiddenGroup (g1, -6, 0, NULL);
/* pages for book chapter only */
      if (pchoice == PUB_BOOKCHPTR)
      {
        StaticPrompt (g2, "Pages",
                      0, dialogTextHeight, programFont, 'l');
        ppp->pages = (Pointer) DialogText (g2, "", 8, NULL);
      }
      StaticPrompt (g2, "Publication Year",
                    0, dialogTextHeight, programFont, 'l');
      ppp->year = DialogText(g2, "", 4, NULL);
      if (pchoice != PUB_PROCCHPTR)
      {
        StaticPrompt (g2, "(Copyright Year)",
                      0, dialogTextHeight, programFont, 'l');
        ppp->cpryear = DialogText(g2, "", 4, NULL);
      }
      AlignObjects (ALIGN_CENTER, (HANDLE) g1, (HANDLE) g2, NULL);
    }
  }
  return pg;
}

static GrouP CreateLocalePage (GrouP m, PubdescPagePtr ppp, Uint1 pchoice,
                               Boolean flagSerial)
{
  GrouP pg, g1, g2, g3, n;

  pg = NULL;
  if (ppp != NULL)
  {
    pg = HiddenGroup (m, -1, 0, NULL);
    if (pg != NULL)
    {
      g1 = HiddenGroup (pg, -1, 0, NULL);
      SetGroupSpacing (g1, 10, 10);

      ppp->proc_affil = CreateProceedingsDialog (g1, NULL);

      g2 = HiddenGroup (g1, -6, 0, NULL);
      StaticPrompt (g2, "Proceedings Number (ie. 4th Ann or IV )",
                    0, dialogTextHeight, programFont, 'l');
      ppp->xa_info = (Pointer) DialogText (g2, "", 16, NULL);

      n = NormalGroup (g1, -1, 0, "Date of Meeting",
                       programFont, NULL);
      g3 = HiddenGroup (n, -6, 0, NULL);
      StaticPrompt (g3, "Year", 0, dialogTextHeight, programFont, 'l');
      ppp->pryear = DialogText (g3, "", 6, NULL);
      StaticPrompt (g3, "Month ", 0, popupMenuHeight, programFont, 'l');
      ppp->prmonth = PopupList (g3, TRUE, NULL);
      InitEnumPopup (ppp->prmonth, months_alist, NULL);
      StaticPrompt (g3, "Day", 0, dialogTextHeight, programFont, 'l');
      ppp->prday = DialogText (g3, "", 6, NULL);

      AlignObjects (ALIGN_CENTER, (HANDLE) g1, (HANDLE) g2, (HANDLE) n,
                    NULL);
    }
  }
  return pg;
}

static CharPtr  labels1 [] = {
  "Journal", "Volume", "Month", "muid", "pmid", NULL};
static CharPtr  labels2 [] = {
  "Issue", "Day", NULL};
static CharPtr  labels3 [] = {
  "Pages", "Year", NULL};
static CharPtr AuthTabs[] = {
  "Names", "Affiliation", NULL};
static CharPtr CitSubAuthTabs[] = {
  "Names", "Affiliation", "Contact", NULL};

static void ChangeAuthPage (VoidPtr data, Int2 newval, Int2 oldval)
{
  PubdescPagePtr        ppp;
  ppp = (PubdescPagePtr) data;
  if (ppp != NULL)
  {
    SafeHide (ppp->AuthGroup[oldval]);
    SafeShow (ppp->AuthGroup[newval]);
    Update ();
  }
}
static void ChangeEditPage (VoidPtr data, Int2 newval, Int2 oldval)
{
  PubdescPagePtr        ppp;
  ppp = (PubdescPagePtr) data;
  if (ppp != NULL)
  {
    SafeHide (ppp->EditGroup[oldval]);
    SafeShow (ppp->EditGroup[newval]);
    Update ();
  }
}
static void ChangeAppPage (VoidPtr data, Int2 newval, Int2 oldval)
{
  PubdescPagePtr        ppp;
  ppp = (PubdescPagePtr) data;
  if (ppp != NULL)
  {
    SafeHide (ppp->AppGroup[oldval]);
    SafeShow (ppp->AppGroup[newval]);
    Update ();
  }
}
static void ChangeAssPage (VoidPtr data, Int2 newval, Int2 oldval)
{
  PubdescPagePtr        ppp;
  ppp = (PubdescPagePtr) data;
  if (ppp != NULL)
  {
    SafeHide (ppp->AssGroup[oldval]);
    SafeShow (ppp->AssGroup[newval]);
    Update ();
  }
}

static CitGenPtr ConvertCitArtToCitGen (CitArtPtr cap)

{
  CitGenPtr   cgp;
  CitJourPtr  cjp;
  ImprintPtr  imp;
  Char        str [128];
  Int2        title_new;
  Int2        title_old;
  Int2        title_rank [9];
  ValNodePtr  ttl;
  CharPtr     txt;
  ValNodePtr  vnp;

  cgp = NULL;
  if (cap != NULL) {
    title_rank[0] = 0;
    title_rank[1] = 2;
    title_rank[2] = 0;
    title_rank[3] = 1;
    title_rank[4] = 6;
    title_rank[5] = 7;
    title_rank[6] = 5;
    title_rank[7] = 4;
    title_rank[8] = 3;
    cgp = CitGenNew ();
    if (cgp != NULL) {
      if (cap->from == 1) {
        cgp->authors = AsnIoMemCopy (cap->authors,
                                     (AsnReadFunc) AuthListAsnRead,
                                     (AsnWriteFunc) AuthListAsnWrite);
        cjp = (CitJourPtr) cap->fromptr;
        if (cjp != NULL) {
          str [0] = '\0';
          ttl = cjp->title;
          title_old = 0;
          while (ttl != NULL) {
            if (ttl->choice == 1 || (ttl->choice > 2 && ttl->choice < 9)) {
              title_new = title_rank[ttl->choice];
              if (title_new > title_old) {
                title_old = title_new;
                StringNCpy_0 (str, (CharPtr) ttl->data.ptrvalue, sizeof (str));
              }
            }
            ttl = ttl->next;
          }
          if (str [0] != '\0') {
            vnp = ValNodeNew (NULL);
            if (vnp != NULL) {
              vnp->choice = 5;
              vnp->data.ptrvalue = StringSave (str);
              cgp->journal = vnp;
            }
          }
          imp = cjp->imp;
          if (imp != NULL) {
            cgp->volume = StringSave (imp->volume);
            cgp->issue = StringSave (imp->issue);
            cgp->pages = StringSave (imp->pages);
            if (imp->date != NULL) {
              cgp->date = DateDup (imp->date);
            }
          }
        }
        txt = NULL;
        ttl = cap->title;
        while (ttl != NULL) {
          if (ttl->choice == 1 || ttl->choice == 3) {
            txt = (CharPtr) ttl->data.ptrvalue;
          }
          ttl = ttl->next;
        }
        if (txt != NULL && *txt != '\0') {
          cgp->title = StringSave (txt);
        }
      }
    }
  }
  return cgp;
}


static ValNodePtr LookupAnArticle (PubEquivLookupProc lookup, ValNodePtr oldpep, Boolean byMuid)

{
  CitArtPtr   cap;
  CitArtPtr   cap2;
  CitGenPtr   cgp;
  MsgAnswer   msg;
  Int2        num;
  ValNodePtr  pep;
  ValNodePtr  pub;
  ValNodePtr  vnp;

  pub = NULL;
  if (lookup != NULL && oldpep != NULL) {
    pep = AsnIoMemCopy (oldpep, (AsnReadFunc) PubEquivAsnRead,
                        (AsnWriteFunc) PubEquivAsnWrite);
    if (pep != NULL) {
      if (byMuid) {
        if (pep->choice == PUB_Article) {
          cap = (CitArtPtr) pep->data.ptrvalue;
          if (cap != NULL) {
            cgp = ConvertCitArtToCitGen (cap);
            if (cgp != NULL) {
              pep->choice = PUB_Gen;
              pep->data.ptrvalue = cgp;
              CitArtFree (cap);
            }
          }
        }
      }
      pub = lookup (pep);
      PubEquivFree (pep);
      if (pub != NULL) {
        if (! byMuid) {
          pep = AsnIoMemCopy (oldpep, (AsnReadFunc) PubEquivAsnRead,
                              (AsnWriteFunc) PubEquivAsnWrite);
          if (pep != NULL && pep->choice == PUB_Article) {
            cap = (CitArtPtr) pep->data.ptrvalue;
            if (cap != NULL) {
              cgp = ConvertCitArtToCitGen (cap);
              if (cgp != NULL) {
                pep->choice = PUB_Gen;
                pep->data.ptrvalue = cgp;
                CitArtFree (cap);
              }
            }
            for (vnp = pep; vnp->next != NULL; vnp = vnp->next) continue;
            vnp->next = pub;
            pub = pep;
          }
        }
        cap = NULL;
        cap2 = NULL;
        cgp = NULL;
        for (vnp = pub; vnp != NULL; vnp = vnp->next) {
          switch (vnp->choice) {
            case PUB_Gen :
              if (cgp == NULL || cgp->authors == NULL) {
                cgp = (CitGenPtr) vnp->data.ptrvalue;
              }
              break;
            case PUB_Article :
              if (cap == NULL || cap->authors == NULL) {
                cap = (CitArtPtr) vnp->data.ptrvalue;
              } else {
                cap2 = (CitArtPtr) vnp->data.ptrvalue;
              }
              break;
            default :
              break;
          }
        }
        if (cap != NULL && cap2 != NULL && cap->authors != NULL && cap2->authors != NULL) {
          num = 0;
          for (vnp = cap2->authors->names; vnp != NULL; vnp = vnp->next) {
            num++;
          }
          if (num > 10) {
            msg = Message (MSG_YN, "Retain original %d authors?", (int) num);
            if (msg == ANS_YES) {
              AuthListFree (cap->authors);
              cap2->authors = cap2->authors;
              cap2->authors = NULL;
            } else if (msg == ANS_CANCEL) {
              pub = PubEquivFree (pub);
            } else {
              AuthListFree (cap2->authors);
              cap2->authors = NULL;
            }
          } else {
            AuthListFree (cap2->authors);
            cap2->authors = NULL;
          }
        } else if (cap != NULL && cgp != NULL && cap->authors != NULL && cgp->authors != NULL) {
          num = 0;
          for (vnp = cgp->authors->names; vnp != NULL; vnp = vnp->next) {
            num++;
          }
          if (num > 10) {
            msg = Message (MSG_YN, "Retain original %d authors?", (int) num);
            if (msg == ANS_YES) {
              AuthListFree (cap->authors);
              cap->authors = cgp->authors;
              cgp->authors = NULL;
            } else if (msg == ANS_CANCEL) {
              pub = PubEquivFree (pub);
            } else {
              AuthListFree (cgp->authors);
              cgp->authors = NULL;
            }
          } else {
            AuthListFree (cgp->authors);
            cgp->authors = NULL;
          }
        }
      }
    }
  }
  return pub;
}

static void LookupCommonProc (ButtoN b, Boolean byMuid, Boolean byPmid)

{
  PubdescPtr      pdp;
  ValNodePtr      pep;
  PubdescPagePtr  ppp;
  ValNodePtr      vnp;

  ppp = (PubdescPagePtr) GetObjectExtra (b);
  if (ppp != NULL && ppp->lookupArticle != NULL) {
    pdp = DialogToPointer (ppp->dialog);
    if (pdp != NULL) {
      if (byMuid) {
        if (byPmid) {
          vnp = ValNodeFindNext (pdp->pub, NULL, PUB_Muid);
          if (vnp != NULL && ValNodeFindNext (pdp->pub, NULL, PUB_PMid) != NULL) {
            ValNodeExtract (&(pdp->pub), PUB_Muid);
          }
        } else {
          vnp = ValNodeFindNext (pdp->pub, NULL, PUB_PMid);
          if (vnp != NULL && ValNodeFindNext (pdp->pub, NULL, PUB_Muid) != NULL) {
            ValNodeExtract (&(pdp->pub), PUB_PMid);
          }
        }
      }
      pep = LookupAnArticle (ppp->lookupArticle, pdp->pub, byMuid);
      if (pep != NULL) {
        pdp->pub = PubEquivFree (pdp->pub);
        pdp->pub = pep;
        PointerToDialog (ppp->dialog, (Pointer) pdp);
      }
      PubdescFree (pdp);
      Select (ParentWindow (b));
      Update ();
    }
  }
}

static void LookupArticleProc (ButtoN b)

{
  LookupCommonProc (b, FALSE, FALSE);
}

static void LookupByMuidProc (ButtoN b)

{
  LookupCommonProc (b, TRUE, FALSE);
}

static void LookupByPmidProc (ButtoN b)

{
  LookupCommonProc (b, TRUE, TRUE);
}

static void LaunchRelaxedQuery PROTO((PubdescPtr pdp, Pointer userdata));

static void LookupRelaxedProc (ButtoN b)

{
  PubdescPtr      pdp;
  PubdescPagePtr  ppp;

  ppp = (PubdescPagePtr) GetObjectExtra (b);
  if (ppp == NULL) return;

  pdp = DialogToPointer (ppp->dialog);
  if (pdp == NULL) return;

  LaunchRelaxedQuery (pdp, (Pointer) ppp);
  PubdescFree (pdp);
}

static void LookupISOJournalProc (ButtoN b)

{
  Int1            jtaType;
  PubdescPagePtr  ppp;
  Char            str [256];

  ppp = (PubdescPagePtr) GetObjectExtra (b);
  if (ppp != NULL && ppp->lookupJournal != NULL) {
    GetTitle (ppp->journal, str, sizeof (str) - 1);
    if (! StringHasNoText (str)) {
      if ((ppp->lookupJournal) (str, sizeof (str) - 1, &jtaType)) {
        SetTitle (ppp->journal, str);
        Update ();
      }
    }
  }
}

static void ChangeRetractType (PopuP p)

{
  PubdescPagePtr  ppp;
  Int2            val;

  ppp = (PubdescPagePtr) GetObjectExtra (p);
  if (ppp != NULL) {
    val = GetValue (p);
    if (val < 2) {
      SafeDisable (ppp->retractExp);
    } else {
      SafeEnable (ppp->retractExp);
    }
  }
}

static void SetCitSubTitle (PopuP p)

{
  PubdescPagePtr  ppp;
  Int2            val;

  ppp = (PubdescPagePtr) GetObjectExtra (p);
  if (ppp != NULL) {
    val = GetValue (p);
    switch (val) {
      case 1 :
        SetTitle (ppp->title_box, "");
        break;
      case 2 :
        SetTitle (ppp->title_box, "Amino acid sequence updated by submitter");
        break;
      case 3 :
        SetTitle (ppp->title_box, "Nucleotide sequence updated by submitter");
        break;
      case 4 :
        SetTitle (ppp->title_box, "Nucleotide and amino acid sequences updated by submitter");
        break;
      case 5 :
        SetTitle (ppp->title_box, "Amino acid sequence updated by database staff");
        break;
      case 6 :
        SetTitle (ppp->title_box, "Nucleotide sequence updated by database staff");
        break;
      case 7 :
        SetTitle (ppp->title_box, "Nucleotide and amino acid sequences updated by database staff");
        break;
      default :
        SetTitle (ppp->title_box, "");
        break;
    }
  }
}

static void CleanupPubdescDialog (GraphiC g, VoidPtr data)

{
  PubdescPagePtr  ppp;

  ppp = (PubdescPagePtr) data;
  if (ppp != NULL) {
    ppp->originalAuthList = AuthListFree (ppp->originalAuthList);
  }
  StdCleanupExtraProc (g, data);
}

static DialoG CreatePubdescDialog (GrouP h, CharPtr title, GrouP PNTR pages,
                        Uint1 reftype,
                        Uint1 pub_status, Int2 pub_choice,
                        Boolean flagPubDelta,
                        Boolean flagSerial,
                        PubdescEditProcsPtr pepp,
                        PubdescFormPtr pfp)
{
  ButtoN                b;
  GrouP                 c;
  GrouP                 g1, g2, g3, g4, g5, g6, g7, g8, g9, g10;
  GrouP                 g11, g12, g13, g14, g15;
  GrouP                 g16, g17, g18, g19, g20, g21, g22;
  ButtoN                lkp;
  GrouP                 m, m1, m2, m3, m4, m5, m6, m7, m8, m9;
  GrouP                 n1, n2, n3, n4;
  GrouP                 p;
  PrompT                p2, p3, p4, p5;
  PopuP                 pp;
  PubdescPagePtr        ppp;
  GrouP                 s;
  Int2                  Remarknumber, thispage;
  Int2                  wid1, wid2, wid3;

  p = HiddenGroup (h, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  ppp = (PubdescPagePtr) MemNew (sizeof (PubdescPage));
  if (ppp != NULL && pages != NULL)
  {
    if (pepp == NULL)
    {
      pepp = (PubdescEditProcsPtr) GetAppProperty ("PubdescEditForm");
    }

    SetObjectExtra (p, ppp, CleanupPubdescDialog);
    ppp->dialog = (DialoG) p;
    ppp->todialog = PubdescPtrToPubdescPage;
    ppp->fromdialog = PubdescPageToPubdescPtr;
    ppp->testdialog = NULL;

/* copy a bunch of flags and defaults got from the init form */
    ppp->reftype = reftype;
    ppp->pub_status = pub_status;
    ppp->pub_choice = pub_choice;
    ppp->flagPubDelta = flagPubDelta;

    if (pepp != NULL && pub_choice == PUB_JOURNAL) {
      ppp->lookupArticle = pepp->lookupArticle;
      ppp->lookupJournal = pepp->lookupJournal;
    }

    if (title != NULL && title [0] != '\0')
    {
      s = NormalGroup (p, 0, -2, title, systemFont, NULL);
    }
    else
    {
      s = HiddenGroup (p, 0, -2, NULL);
    }
    SetGroupSpacing (s, 10, 10);

    m = HiddenGroup (s, 0, 0, NULL);

    thispage = 0;

    pages[thispage] = HiddenGroup (m, -1, 0, NULL);
    g3 = HiddenGroup (pages[thispage], -1, 0, NULL);

    if (ppp->pub_choice != PUB_SUB)
      StaticPrompt (g3, "Title", (Int2) (28 * stdCharWidth),
                    0, programFont, 'c');
    else
      StaticPrompt (g3, "Description", (Int2) (28 * stdCharWidth),
                    0, programFont, 'c');

    if (ppp->pub_choice != PUB_BOOK && ppp->pub_choice != PUB_PROC)
    {
      if (ppp->pub_choice != PUB_SUB) {
        ppp->title_box = ScrollText (g3, 28, 5, programFont, TRUE, NULL);
      } else {
        ppp->title_box = ScrollText (g3, 25, 9, programFont, TRUE, NULL);
        g22 = NULL;
        if (GetAppProperty ("InternalNcbiSequin") != NULL) {
          g22 = HiddenGroup (g3, 0, 2, NULL);
          StaticPrompt (g22, "Add standard remark text", 0, 0, programFont, 'c');
          pp = PopupList (g22, TRUE, SetCitSubTitle);
          PopupItem (pp, " ");
          PopupItem (pp, "AA by submitter");
          PopupItem (pp, "NT by submitter");
          PopupItem (pp, "NT and AA by submitter");
          PopupItem (pp, "AA by database");
          PopupItem (pp, "NT by database");
          PopupItem (pp, "NT and AA by database");
          SetValue (pp, 0); /* so blank value (1) can be chosen to clear text */
          SetObjectExtra (pp, (Pointer) ppp, NULL);
        }
        AlignObjects (ALIGN_CENTER, (HANDLE) ppp->title_box, (HANDLE) g22, NULL);
      }
    }
    else
    {
      ppp->booktitle = ScrollText (g3, 28, 5, programFont, TRUE, NULL);
    }
    Hide (pages[thispage]);

    if (ppp->pub_choice == PUB_BOOKCHPTR || ppp->pub_choice == PUB_PROCCHPTR)
    {
      thispage++;
      pages[thispage] = HiddenGroup (m, -1, 0, NULL);
      g3 = HiddenGroup (pages[thispage], -1, 0, NULL);
      StaticPrompt (g3, "Title", (Int2) (28 * stdCharWidth), 0,
                    programFont, 'c');
      ppp->booktitle = ScrollText (g3, 28, 5, programFont,
                                  TRUE, NULL);
      Hide (pages[thispage]);
    }

    thispage++;
    pages[thispage] = HiddenGroup (m, -1, 0, NULL);
    SetGroupSpacing (pages[thispage], 10, 10);

    g1 = HiddenGroup (pages[thispage], -1, 0, NULL);
    SetGroupSpacing (g1, 10, 10);
    if (ppp->pub_choice == PUB_SUB)
    {
      m1 = (GrouP) CreateFolderTabs (g1, CitSubAuthTabs, 0, 0, 0,
                         PROGRAM_FOLDER_TAB,
                         ChangeAuthPage, (Pointer) ppp);
    }
    else if (ppp->pub_choice != PUB_BOOK && ppp->pub_choice != PUB_PROC)
    {
      m1 = (GrouP) CreateFolderTabs (g1, AuthTabs, 0, 0, 0,
                         PROGRAM_FOLDER_TAB,
                         ChangeAuthPage, (Pointer) ppp);
    }
    else
    {
      m1 = (GrouP) CreateFolderTabs (g1, AuthTabs, 0, 0, 0,
                         PROGRAM_FOLDER_TAB,
                         ChangeEditPage, (Pointer) ppp);
    }
    g2 = HiddenGroup (g1, 0, 0, NULL);
    m2 = HiddenGroup (g2, -1, 0, NULL);
    if (ppp->pub_choice != PUB_BOOK && ppp->pub_choice != PUB_PROC)
    {
      ppp->AuthGroup[0] = m2;
      ppp->author_list = CreateAuthorDialog (m2, 3, -1);
      pfp->Author_Page = thispage;
    }
    else
    {
      ppp->EditGroup[0] = m2;
      ppp->editor_list = CreateAuthorDialog (m2, 3, -1);
    }
    Show (m2);
    m3 = HiddenGroup (g2, -1, 0, NULL);
    if (ppp->pub_choice != PUB_BOOK && ppp->pub_choice != PUB_PROC)
    {
      ppp->author_affil = CreateExtAffilDialog (m3, NULL, &(ppp->AuthGroup[1]),
                                                &(ppp->AuthGroup[2]));
    }
    else
    {
      ppp->editor_affil = CreateExtAffilDialog (m3, NULL, &(ppp->EditGroup[1]),
                                                &(ppp->EditGroup[2]));
    }

    AlignObjects (ALIGN_CENTER, (HANDLE) m1, (HANDLE) m2, (HANDLE) m3,
                  NULL);

    Hide (pages[thispage]);

    switch (pub_choice)
    {
      case PUB_JOURNAL:

        SelectFont (programFont);
        wid1 = MaxStringWidths (labels1);
        wid2 = MaxStringWidths (labels2);
        wid3 = MaxStringWidths (labels3);
        SelectFont (systemFont);

        pages[2] = HiddenGroup (m, -1, 0, NULL);
        SetGroupSpacing (pages[2], 3, 10);
        g4 = HiddenGroup (pages[2], -1, 0, NULL);

        g5 = HiddenGroup (g4, -3, 0, NULL);
        StaticPrompt (g5, "Journal", wid1, dialogTextHeight, programFont, 'l');
        ppp->journal = DialogText (g5, "", 24, NULL);

        g6 = HiddenGroup (g4, -6, 0, NULL);
        StaticPrompt (g6, "Volume", wid1, dialogTextHeight, programFont, 'l');
        ppp->volume = DialogText (g6, "", 2, NULL);
        p2 = StaticPrompt (g6, "Issue", wid2, dialogTextHeight, programFont,
                           'l');
        ppp->issue = DialogText (g6, "", 2, NULL);
        p3 = StaticPrompt (g6, "Pages", wid3, dialogTextHeight, programFont,
                           'l');
        ppp->pages = DialogText (g6, "", 2, NULL);

        g8 = HiddenGroup (g4, -6, 0, NULL);
        StaticPrompt (g8, "Month", wid1, popupMenuHeight, programFont, 'l');
        ppp->month = PopupList (g8, TRUE, NULL);
        InitEnumPopup (ppp->month, months_alist, NULL);
        SetValue (ppp->month, 1);
        p4 = StaticPrompt (g8, "Day", wid2, dialogTextHeight, programFont,
                           'l');
        ppp->day = DialogText (g8, "", 4, NULL);
        p5 = StaticPrompt (g8, "Year", wid3, dialogTextHeight, programFont,
                           'l');
        ppp->year = DialogText (g8, "", 6, NULL);

        if (ppp->lookupJournal != NULL) {
          g9 = HiddenGroup (g4, -3, 0, NULL);
        } else {
          g9 = HiddenGroup (g4, -2, 0, NULL);
        }
/* muid's are a journal-thing */
/* for now PubMedid's (pmid's) are a journal thing too */
/* this will change with a new layout design */
        StaticPrompt (g9, "muid", wid1, dialogTextHeight, programFont, 'l');
        ppp->muid = DialogText (g9, "", 6, NULL);
        lkp = NULL;
        if (ppp->lookupJournal != NULL) {
          lkp = PushButton (g9, "Lookup ISO JTA", LookupISOJournalProc);
          SetObjectExtra (lkp, ppp, NULL);
        }
        StaticPrompt (g9, "pmid", wid1, dialogTextHeight, programFont, 'l');
        ppp->pmid = DialogText (g9, "", 6, NULL);
        if (ppp->lookupJournal != NULL) {
          StaticPrompt (g9, "", 0, dialogTextHeight, programFont, 'l');
        }

        if (flagSerial && ppp->serial == NULL)
        {
          StaticPrompt (g9, "Serial Number",
                        0, dialogTextHeight, programFont, 'l');
          ppp->serial = DialogText (g9, "", 6, NULL);
          if (ppp->lookupJournal != NULL) {
            StaticPrompt (g9, "", 0, dialogTextHeight, programFont, 'l');
          }
        } else if (GetAppProperty ("InternalNcbiSequin") != NULL) {
          StaticPrompt (g9, "Serial Number",
                        0, dialogTextHeight, programFont, 'l');
          ppp->serial = DialogText (g9, "", 6, NULL);
          if (ppp->lookupJournal != NULL) {
            StaticPrompt (g9, "", 0, dialogTextHeight, programFont, 'l');
          }
        }

        g20 = HiddenGroup (pages[2], -2, 0, NULL);
        g21 = HiddenGroup (g20, 0, 2, NULL);
        StaticPrompt (g21, "Erratum", 0, 0, programFont, 'c');
        ppp->retractType = PopupList (g21, TRUE, ChangeRetractType);
        SetObjectExtra (ppp->retractType, ppp, NULL);
        PopupItem (ppp->retractType, " ");
        PopupItem (ppp->retractType, "Retracted");
        PopupItem (ppp->retractType, "Notice");
        PopupItem (ppp->retractType, "In-Error");
        PopupItem (ppp->retractType, "Erratum");
        ppp->retractGrp = HiddenGroup (g20, 0, 2, NULL);
        StaticPrompt (ppp->retractGrp, "Explanation", 0, 0, programFont, 'c');
        ppp->retractExp = DialogText (ppp->retractGrp, "", 18, NULL);
        Disable (ppp->retractExp);

        c = HiddenGroup (pages[2], -4, 0, NULL);
        SetGroupSpacing (c, 10, 2);
        if (ppp->lookupArticle != NULL) {
          b = PushButton (c, "Lookup Article", LookupArticleProc);
          SetObjectExtra (b, ppp, NULL);
          b = PushButton (c, "Lookup By muid", LookupByMuidProc);
          SetObjectExtra (b, ppp, NULL);
          b = PushButton (c, "Lookup By pmid", LookupByPmidProc);
          SetObjectExtra (b, ppp, NULL);
          /* Disable (b); */
          b = PushButton (c, "Lookup Relaxed", LookupRelaxedProc);
          SetObjectExtra (b, ppp, NULL);
        }

        AlignObjects (ALIGN_RIGHT, (HANDLE) ppp->pages, (HANDLE) ppp->year,
                      (HANDLE) ppp->journal, (HANDLE) lkp, NULL);
        AlignObjects (ALIGN_LEFT, (HANDLE) ppp->pages, (HANDLE) ppp->year,
                      (HANDLE) lkp, NULL);

        AlignObjects (ALIGN_JUSTIFY, (HANDLE) ppp->issue,
                      (HANDLE) ppp->day, NULL);
        AlignObjects (ALIGN_JUSTIFY, (HANDLE) ppp->volume,
                      (HANDLE) ppp->month, NULL);
        AlignObjects (ALIGN_JUSTIFY, (HANDLE) p2, (HANDLE) p4, NULL);
        AlignObjects (ALIGN_JUSTIFY, (HANDLE) ppp->issue,
                      (HANDLE) ppp->day, NULL);
        AlignObjects (ALIGN_RIGHT, (HANDLE) ppp->day,
                      (HANDLE) ppp->issue, (HANDLE) ppp->muid,
                      (HANDLE) ppp->pmid, (HANDLE) ppp->serial, NULL);
        AlignObjects (ALIGN_JUSTIFY, (HANDLE) p3, (HANDLE) p5, NULL);
        AlignObjects (ALIGN_CENTER, (HANDLE) g4, (HANDLE) g20, (HANDLE) c, NULL);

        Hide (pages[2]);

        break;

      case PUB_BOOKCHPTR:
      case PUB_PROCCHPTR:

        pages[3] = HiddenGroup (m, -1, 0, NULL);
        SetGroupSpacing (pages[3], 10, 10);
        g4 = HiddenGroup (pages[3], -1, 0, NULL);
        SetGroupSpacing (g4, 10, 10);
        m4 = (GrouP) CreateFolderTabs (g4, AuthTabs, 0, 0, 0,
                                       PROGRAM_FOLDER_TAB,
                                       ChangeEditPage, (Pointer) ppp);
        g5 = HiddenGroup (g4, 0, 0, NULL);
        m5 = HiddenGroup (g5, -1, 0, NULL);
        ppp->EditGroup[0] = m5;
        ppp->editor_list = CreateAuthorDialog (m5, 3, -1);
        Show (m5);
        m6 = HiddenGroup (g5, -1, 0, NULL);
        ppp->editor_affil = CreateExtAffilDialog (m6, NULL,
                                                  &(ppp->EditGroup[1]),
                                                  &(ppp->EditGroup[2]));

        AlignObjects (ALIGN_CENTER, (HANDLE) m4, (HANDLE) m5, (HANDLE) m6,
                      NULL);

        Hide (pages[3]);

        if (pub_choice == PUB_BOOKCHPTR)
        {
          pages[4] = CreateBookPublisherPage (m, ppp, PUB_BOOKCHPTR,
                                              flagSerial);
          Hide (pages[4]);
        }
        else
        {
          pages[4] = CreateLocalePage (m, ppp, PUB_PROCCHPTR, flagSerial);
          Hide (pages[4]);
          pages[5] = CreateBookPublisherPage (m, ppp, PUB_BOOKCHPTR,
                                              flagSerial);
          Hide (pages[5]);
        }
        break;

      case PUB_BOOK:
      case PUB_PROC:
      case PUB_THESIS:
        if (pub_choice == PUB_BOOK || pub_choice == PUB_THESIS)
        {
          pages[2] = CreateBookPublisherPage (m, ppp, PUB_BOOK, flagSerial);
          Hide (pages[2]);
        }
        else
        {
          pages[2] = CreateLocalePage (m, ppp, PUB_PROC, flagSerial);
          Hide (pages[2]);
          pages[3] = CreateBookPublisherPage (m, ppp, PUB_BOOK, flagSerial);
          Hide (pages[3]);
        }
        break;

      case PUB_PATENT:
        pages[2] = HiddenGroup (m, -1, 0, NULL);
        g3 = HiddenGroup (pages[2], -1, 0, NULL);
/* assorted stuff */
        g4 = HiddenGroup (g3, -2, 0, NULL);
        StaticPrompt (g4, "Country", 0, dialogTextHeight, programFont, 'l');
        ppp->pat_country = DialogText (g4, "", 18, NULL);
        StaticPrompt (g4, "Document Type", 0, dialogTextHeight,
                      programFont, 'l');
        ppp->pat_doc_type = DialogText (g4, "", 18, NULL);
/* the number */
        StaticPrompt (g3, "", 0, dialogTextHeight, programFont, 'l');
        n1 = NormalGroup (g3, -1, 0, "Patent Number",
                          programFont, NULL);
        g13 = HiddenGroup (n1, -1, 0, NULL);
        ppp->pat_number = DialogText (g13, "", 10, NULL);
        AlignObjects (ALIGN_CENTER, (HANDLE) n1, (HANDLE) g13, NULL);
/* issue date */
        n2 = NormalGroup (g3, -1, 0, "Patent Issue Date",
                          programFont, NULL);
        g5 = HiddenGroup (n2, -6, 0, NULL);
        StaticPrompt (g5, "Year", 0, dialogTextHeight, programFont, 'l');
        ppp->year = DialogText (g5, "", 6, NULL);
        StaticPrompt (g5, "Month", 0, popupMenuHeight, programFont, 'l');
        ppp->month = PopupList (g5, TRUE, NULL);
        InitEnumPopup (ppp->month, months_alist, NULL);
        StaticPrompt (g5, "Day", 0, dialogTextHeight, programFont, 'l');
        ppp->day = DialogText (g5, "", 6, NULL);
        AlignObjects (ALIGN_CENTER, (HANDLE) n2, (HANDLE) g5, NULL);
/* application number */
        StaticPrompt (g3, "", 0, dialogTextHeight, programFont, 'l');
        n3 = NormalGroup (g3, -1, 0, "Application Number",
                          programFont, NULL);
        g6 = HiddenGroup (n3, -2, 0, NULL);
        ppp->pat_app_number = DialogText (g6, "", 10, NULL);
        AlignObjects (ALIGN_CENTER, (HANDLE) n3, (HANDLE) g6, NULL);
/* application date */
        n4 = NormalGroup (g3, -1, 0, "Application Date",
                          programFont, NULL);
        g7 = HiddenGroup (n4, -6, 0, NULL);
        StaticPrompt (g7, "Year", 0, dialogTextHeight, programFont, 'l');
        ppp->cpryear = DialogText (g7, "", 6, NULL);
        StaticPrompt (g7, "Month ", 0, popupMenuHeight, programFont, 'l');
        ppp->cprmonth = PopupList (g7, TRUE, NULL);
        InitEnumPopup (ppp->cprmonth, months_alist, NULL);
        StaticPrompt (g7, "Day", 0, dialogTextHeight, programFont, 'l');
        ppp->cprday = DialogText (g7, "", 6, NULL);
        AlignObjects (ALIGN_CENTER, (HANDLE) n4, (HANDLE) g7, NULL);

        Hide (pages[2]);

        pages[3] = HiddenGroup (m, -1, 0, NULL);
        SetGroupSpacing (pages[3], 10, 10);
        g8 = HiddenGroup (pages[3], -1, 0, NULL);
        SetGroupSpacing (g8, 10, 10);
        m4 = (GrouP) CreateFolderTabs (g8, AuthTabs, 0, 0, 0,
                                      PROGRAM_FOLDER_TAB,
                                      ChangeAppPage, (Pointer) ppp);
        g17 = HiddenGroup (g8, 0, 0, NULL);
        m5 = HiddenGroup (g17, -1, 0, NULL);
        ppp->AppGroup[0] = m5;
        ppp->pat_applicant = CreateAuthorDialog (m5, 3, -1);
        Show (m5);
        m6 = HiddenGroup (g17, -1, 0, NULL);
        ppp->pat_app_affil = CreateExtAffilDialog (m6, NULL,
                                                   &(ppp->AppGroup[1]),
                                                   &(ppp->AppGroup[2]));

        AlignObjects (ALIGN_CENTER, (HANDLE) m4, (HANDLE) m5, (HANDLE) m6,
                      NULL);

        Hide (pages[3]);

        pages[4] = HiddenGroup (m, -1, 0, NULL);
        SetGroupSpacing (pages[4], 10, 10);
        g18 = HiddenGroup (pages[4], -1, 0, NULL);
        SetGroupSpacing (g18, 10, 10);

        m7 = (GrouP) CreateFolderTabs (g18, AuthTabs, 0, 0, 0,
                                       PROGRAM_FOLDER_TAB,
                                       ChangeAssPage, (Pointer) ppp);
        g19 = HiddenGroup (g18, 0, 0, NULL);
        m8 = HiddenGroup (g19, -1, 0, NULL);
        ppp->AssGroup[0] = m8;
        ppp->pat_assignee = CreateAuthorDialog (m8, 3, -1);
        Show (m8);
        m9 = HiddenGroup (g19, -1, 0, NULL);
        ppp->pat_ass_affil = CreateExtAffilDialog (m9, NULL,
                                                   &(ppp->AssGroup[1]),
                                                   &(ppp->AssGroup[2]));

        AlignObjects (ALIGN_CENTER, (HANDLE) m7, (HANDLE) m8, (HANDLE) m9,
                      NULL);

        Hide (pages[4]);

        pages[5] = HiddenGroup (m, -1, 0, NULL);
        SetGroupSpacing (pages[5], 10, 10);
        g12 = HiddenGroup (pages[5], -1, 0, NULL);
        StaticPrompt (g12, "Abstract",
                      (Int2) (25 * stdCharWidth), 0,
                      programFont, 'c');
        ppp->pat_abs = ScrollText (g12, 25, 9, programFont, TRUE, NULL);

        Hide (pages[5]);

        break;

      case PUB_SUB:
      default:
        break;
    }

    Remarknumber = tabcounter[pub_choice]-1;
    pages[Remarknumber] = HiddenGroup (m, -1, 0, NULL);

    g10 = HiddenGroup (pages[Remarknumber], -1, 0, NULL);
    StaticPrompt (g10, "Remark", (Int2) (25 * stdCharWidth), 0,
                  programFont, 'c');
    ppp->comment = ScrollText (g10, 25, 5, programFont, TRUE, NULL);

    g11 = HiddenGroup (g10, -6, 0, NULL);
    if (pub_choice == PUB_UNPUB || pub_choice == PUB_SUB)
    {
      StaticPrompt (g11, "Year", 0, dialogTextHeight, programFont, 'l');
      ppp->year = DialogText (g11, "", 6, NULL);
      StaticPrompt (g11, "Month", 0, popupMenuHeight, programFont, 'l');
      ppp->month = PopupList (g11, TRUE, NULL);
      InitEnumPopup (ppp->month, months_alist, NULL);
      StaticPrompt (g11, "Day", 0, dialogTextHeight, programFont, 'l');
      ppp->day = DialogText (g11, "", 6, NULL);
      if (pub_choice == PUB_SUB) {
        DatePtr  dp;
        dp = DateCurr ();
        DatePtrToVibrant (dp, ppp->month, ppp->day, ppp->year);
        DateFree (dp);
      }
    }

    g16 = HiddenGroup (g10, -2, 0, NULL);
    if (pub_choice != PUB_JOURNAL)
    {
      if (flagSerial && ppp->serial == NULL)
      {
        StaticPrompt (g16, "Serial Number",
                      0, dialogTextHeight, programFont, 'l');
        ppp->serial = DialogText (g16, "", 6, NULL);
      } else if (GetAppProperty ("InternalNcbiSequin") != NULL) {
        StaticPrompt (g16, "Serial Number",
                      0, dialogTextHeight, programFont, 'l');
        ppp->serial = DialogText (g16, "", 6, NULL);
      }
    }

    g14 = NULL;
    if (pub_choice == PUB_SUB)
    {
      g14 = NormalGroup (g10, -1, 0, "Submission medium", programFont, NULL);
      g15 = HiddenGroup (g14, -4, 0, NULL);
      ppp->medium = g15;
      RadioButton (g15, "Paper");
      RadioButton (g15, "Tape");
      RadioButton (g15, "Floppy");
      RadioButton (g15, "Email");
      RadioButton (g15, "Other");
      AlignObjects (ALIGN_CENTER, (HANDLE) g14, (HANDLE) g15, NULL);
    }

    if (pub_choice != PUB_SUB)
      AlignObjects (ALIGN_CENTER, (HANDLE) g11, (HANDLE) g16, NULL);
    else
      AlignObjects (ALIGN_CENTER, (HANDLE) g11, (HANDLE) g16,
                      (HANDLE) g14, NULL);

    Hide (pages[Remarknumber]);

/* see NUM_TABS defined */
    AlignObjects (ALIGN_CENTER, (HANDLE) pages [0],
            (HANDLE) pages [1], (HANDLE) pages [2],
            (HANDLE) pages [3], (HANDLE) pages [4],
            (HANDLE) pages [5], (HANDLE) pages [6],
            (HANDLE) pages [7], (HANDLE) pages [8],
            (HANDLE) pages [9], (HANDLE) pages [10],
            (HANDLE) pages [11], NULL);
  }
  return (DialoG) p;
}

static void SetPubdescImportExportItems (PubdescFormPtr pfp)

{
  IteM  exportItm;
  IteM  importItm;

  if (pfp != NULL) {
    importItm = FindFormMenuItem ((BaseFormPtr) pfp, VIB_MSG_IMPORT);
    exportItm = FindFormMenuItem ((BaseFormPtr) pfp, VIB_MSG_EXPORT);
    if (pfp->currentPage == FIRST_PAGE) {
      SafeSetTitle (importItm, "Import Pubdesc...");
      SafeSetTitle (exportItm, "Export Pubdesc...");
      SafeEnable (importItm);
      SafeEnable (exportItm);
    } else if (pfp->currentPage == pfp->Author_Page) {
      SafeSetTitle (importItm, "Import Authors...");
      SafeSetTitle (exportItm, "Export Authors...");
      SafeEnable (importItm);
      SafeEnable (exportItm);
    } else if (pfp->is_feat && pfp->currentPage == pfp->Location_Page) {
      SafeSetTitle (importItm, "Import SeqLoc...");
      SafeSetTitle (exportItm, "Export SeqLoc...");
      SafeEnable (importItm);
      SafeEnable (exportItm);
    } else {
      SafeSetTitle (importItm, "Import...");
      SafeSetTitle (exportItm, "Export...");
      SafeDisable (importItm);
      SafeDisable (exportItm);
    }
  }
}

static void ChangePubdescPage (VoidPtr data, Int2 newval, Int2 oldval)
{
  PubdescFormPtr  pfp;

  pfp = (PubdescFormPtr) data;
  if (pfp != NULL)
  {
    pfp->currentPage = newval;
    SafeHide (pfp->pages [oldval]);
    SafeShow (pfp->pages [newval]);
    if (pfp->is_feat)
    {
      if (newval == pfp->Location_Page)
      {
        SendMessageToDialog (pfp->location, VIB_MSG_ENTER);
      }
    }
    SetPubdescImportExportItems (pfp);
    Update ();
  }
}

static void PubdescAcceptFormButtonProc (ButtoN b)

{
  PubdescPtr      copy;
  ErrSev          oldErrSev;
  PubdescPtr      pdp;
  PubdescFormPtr  pfp;

  pfp = (PubdescFormPtr) GetObjectExtra (b);
  if (pfp == NULL) return;
  ErrClear ();
  oldErrSev = ErrSetMessageLevel (SEV_ERROR);
  pdp = (PubdescPtr) DialogToPointer (pfp->data);
  copy = AsnIoMemCopy ((Pointer) pdp,
                       (AsnReadFunc) PubdescAsnRead,
                       (AsnWriteFunc) PubdescAsnWrite); 
  ErrSetMessageLevel (oldErrSev);
  ErrShow ();
  ErrClear ();
  PubdescFree (pdp);
  if (copy == NULL) {
    Message (MSG_OK, "Illegal ASN.1, unable to continue");
    return;
  }
  PubdescFree (copy);
  Hide (pfp->form);
  if (pfp->actproc != NULL) {
    (pfp->actproc) (pfp->form);
  }
  Update ();
  Remove (pfp->form);
}

static void ReplaceAllFormButtonProc (ButtoN b)

{
  PubdescFormPtr  pfp;

  pfp = (PubdescFormPtr) GetObjectExtra (b);
  if (pfp != NULL) {
    pfp->replaceAll = TRUE;
  }
  PubdescAcceptFormButtonProc (b);
}

static Boolean ImportPubdescForm (ForM f, CharPtr filename)

{
  AsnIoPtr        aip;
  Char            path [PATH_MAX];
  PubdescPtr      pdp;
  PubdescPagePtr  ppp;
  PubdescFormPtr  pfp;

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  pfp = (PubdescFormPtr) GetObjectExtra (f);
  if (pfp != NULL) {
    if (pfp->currentPage == FIRST_PAGE) {
      if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
        aip = AsnIoOpen (path, "r");
        if (aip != NULL) {
          pdp = PubdescAsnRead (aip, NULL);
          AsnIoClose (aip);
          if (pdp != NULL) {
            ppp = (PubdescPagePtr) GetObjectExtra (pfp->data);
            if (ppp != NULL) {
              pdp->reftype = ppp->reftype;
              ppp->flagPubDelta = FALSE; /* otherwise it ignores some fields */
            }
            PointerToDialog (pfp->data, (Pointer) pdp);
            pdp = PubdescFree (pdp);
            Update ();
            return TRUE;
          }
        }
      }
    } else if (pfp->currentPage == pfp->Author_Page) {
      ppp = (PubdescPagePtr) GetObjectExtra (pfp->data);
      if (ppp == NULL) return FALSE;
      if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
        return ImportDialog (ppp->author_list, path);
      }
    } else if (pfp->is_feat && pfp->currentPage == pfp->Location_Page) {
      return ImportDialog (pfp->location, filename);
    }
  }
  return FALSE;
}

static Boolean ExportPubdescForm (ForM f, CharPtr filename)

{
  AsnIoPtr        aip;
  Char            path [PATH_MAX];
  PubdescPtr      pdp;
  PubdescFormPtr  pfp;
  PubdescPagePtr  ppp;
#ifdef WIN_MAC
  FILE            *fp;
#endif

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  pfp = (PubdescFormPtr) GetObjectExtra (f);
  if (pfp != NULL) {
    if (pfp->currentPage == FIRST_PAGE) {
      if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), NULL)) {
#ifdef WIN_MAC
        fp = FileOpen (path, "r");
        if (fp != NULL) {
          FileClose (fp);
        } else {
          FileCreate (path, "TEXT", "ttxt");
        }
#endif
        aip = AsnIoOpen (path, "w");
        if (aip != NULL) {
          pdp = DialogToPointer (pfp->data);
          pdp->reftype = 0;
          PubdescAsnWrite (pdp, aip, NULL);
          AsnIoClose (aip);
          pdp = PubdescFree (pdp);
          return TRUE;
        }
      }
    } else if (pfp->currentPage == pfp->Author_Page) {
      ppp = (PubdescPagePtr) GetObjectExtra (pfp->data);
      if (ppp == NULL) return FALSE;
      if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), NULL)) {
        return ExportDialog (ppp->author_list, path);
      }
    } else if (pfp->is_feat && pfp->currentPage == pfp->Location_Page) {
      return ExportDialog (pfp->location, filename);
    }
  }
  return FALSE;
}

static void PubdescDescFormMessage (ForM f, Int2 mssg)

{
  PubdescFormPtr  pfp;

  pfp = (PubdescFormPtr) GetObjectExtra (f);
  if (pfp != NULL) {
    switch (mssg) {
      case VIB_MSG_IMPORT :
        ImportPubdescForm (f, NULL);
        break;
      case VIB_MSG_EXPORT :
        ExportPubdescForm (f, NULL);
        break;
      case VIB_MSG_PRINT :
        break;
      case VIB_MSG_CLOSE :
        Remove (f);
        break;
      case VIB_MSG_CUT :
        StdCutTextProc (NULL);
        break;
      case VIB_MSG_COPY :
        StdCopyTextProc (NULL);
        break;
      case VIB_MSG_PASTE :
        StdPasteTextProc (NULL);
        break;
      case VIB_MSG_DELETE :
        StdDeleteTextProc (NULL);
        break;
      default :
        if (pfp->appmessage != NULL) {
          pfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void PubdescFormActivate (WindoW w)

{
  PubdescFormPtr  pfp;

  pfp = (PubdescFormPtr) GetObjectExtra (w);
  if (pfp != NULL) {
    if (pfp->activate != NULL) {
      pfp->activate (w);
    }
    SetPubdescImportExportItems (pfp);
  }
}

extern ForM CreatePubdescDescForm (Int2 left, Int2 top,
                    CharPtr title,
                    Uint1 reftype, Uint1 pub_status,
                    Int2 pub_choice,
                    Boolean flagPubDelta,
                    Boolean flagSerial,
                    ValNodePtr sdp, SeqEntryPtr sep,
                    FormActnFunc actproc,
                    PubdescEditProcsPtr pepp)
{
  ButtoN                b;
  GrouP                 c;
  GrouP                 h1;
  Int2                  initPage;
  Int2                  j;
  CharPtr PNTR          labels;
  PubdescFormPtr        pfp;
  StdEditorProcsPtr     sepp;
  Int2                  tabnumber;
  CharPtr               tabs [NUM_TABS];
  WindoW                w;

  w = NULL;
  pfp = (PubdescFormPtr) MemNew (sizeof (PubdescForm));
  if (pfp != NULL) {
    w = FixedWindow (left, top, -10, -10, title, StdCloseWindowProc);
    SetObjectExtra (w, pfp, StdDescFormCleanupProc);
    pfp->form = (ForM) w;
    pfp->actproc = actproc;
    pfp->toform = NULL;
    pfp->fromform = NULL;
    pfp->testform = NULL;
    pfp->formmessage = PubdescDescFormMessage;
    pfp->importform = ImportPubdescForm;
    pfp->exportform = ExportPubdescForm;

    if (pepp == NULL)
    {
      pepp = (PubdescEditProcsPtr) GetAppProperty ("PubdescEditForm");
    }

#ifndef WIN_MAC
    CreateStdEditorFormMenus (w);
#endif

    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      pfp->activate = sepp->activateForm;
      pfp->appmessage = sepp->handleMessages;
    }
    SetActivate (w, PubdescFormActivate);

    tabnumber = tabcounter[pub_choice];
    pfp->tabnumber = tabnumber;
    pfp->pub_choice = pub_choice;
    pfp->is_feat = FALSE;
    pfp->replaceAll = FALSE;
    labels = pubdescFormTabs[pub_choice];
    for (j = 0; j < NUM_TABS; j++)
    {
      tabs [j] = NULL;
    }
    for (j = 0; j < NUM_TABS && labels [j] != NULL; j++)
    {
      tabs [j] = labels [j];
    }

    h1 = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (h1, 3, 10);

    initPage = FIRST_PAGE;
    if (GetAppProperty ("InternalNcbiSequin") != NULL) {
      if (pub_choice == PUB_JOURNAL) {
        initPage = 2;
      }
    }
    pfp->foldertabs = CreateFolderTabs (h1, tabs, initPage,
                         descTabsPerLine [pub_choice], 10,
                         SYSTEM_FOLDER_TAB,
                         ChangePubdescPage, (Pointer) pfp);
    pfp->currentPage = initPage;
    pfp->data = CreatePubdescDialog (h1, NULL, pfp->pages,
          reftype,
          pub_status,
          pub_choice,
          flagPubDelta, flagSerial,
          pepp, pfp);
    pfp->Attribute_Page = 0;
    pfp->Location_Page = 0;

    c = HiddenGroup (h1, 4, 0, NULL);
    if (sdp == NULL) {
      b = PushButton (c, "Accept", ReplaceAllFormButtonProc);
      SetObjectExtra (b, pfp, NULL);
    } else if (pepp != NULL && pepp->replaceThis) {
      b = PushButton (c, "Replace All", ReplaceAllFormButtonProc);
      SetObjectExtra (b, pfp, NULL);
      b = PushButton (c, "Replace This", PubdescAcceptFormButtonProc);
      SetObjectExtra (b, pfp, NULL);
    } else {
      b = PushButton (c, "Accept", ReplaceAllFormButtonProc);
      SetObjectExtra (b, pfp, NULL);
    }
    PushButton (c, "Cancel", StdCancelButtonProc);

    AlignObjects (ALIGN_CENTER, (HANDLE) pfp->foldertabs, (HANDLE) pfp->data,
               (HANDLE) c, NULL);
    RealizeWindow (w);

    SendMessageToDialog (pfp->data, VIB_MSG_INIT);
    ChangePubdescPage ((Pointer) pfp, pfp->currentPage, 0);
    /*
    Show (pfp->pages [pfp->currentPage]);
    SendMessageToDialog (pfp->data, VIB_MSG_ENTER);
    */
    Update ();
  }
  return (ForM) w;
}

static void PubdescFeatFormMessage (ForM f, Int2 mssg)

{
  PubdescFormPtr  pfp;

  pfp = (PubdescFormPtr) GetObjectExtra (f);
  if (pfp != NULL) {
    switch (mssg) {
      case VIB_MSG_INIT :
        StdInitFeatFormProc (f);
        break;
      case VIB_MSG_IMPORT :
        ImportPubdescForm (f, NULL);
        break;
      case VIB_MSG_EXPORT :
        ExportPubdescForm (f, NULL);
        break;
      case VIB_MSG_PRINT :
        break;
      case VIB_MSG_CLOSE :
        Remove (f);
        break;
      case VIB_MSG_CUT :
        StdCutTextProc (NULL);
        break;
      case VIB_MSG_COPY :
        StdCopyTextProc (NULL);
        break;
      case VIB_MSG_PASTE :
        StdPasteTextProc (NULL);
        break;
      case VIB_MSG_DELETE :
        StdDeleteTextProc (NULL);
        break;
      default :
        if (pfp->appmessage != NULL) {
          pfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

extern ForM CreatePubdescFeatForm (Int2 left, Int2 top,
                    CharPtr title,
                    Uint1 reftype, Uint1 pub_status,
                    Int2 pub_choice,
                    Boolean flagPubDelta,
                    Boolean flagSerial,
                    SeqFeatPtr sfp, SeqEntryPtr sep,
                    FormActnFunc actproc,
                    PubdescEditProcsPtr pepp)
{
  ButtoN                b;
  GrouP                 c;
  GrouP                 g;
  GrouP                 h;
  Int2                  initPage;
  Int2                  j;
  CharPtr PNTR          labels;
  PubdescFormPtr        pfp;
  GrouP                 s;
  StdEditorProcsPtr     sepp;
  Int2                  tabnumber;
  CharPtr               tabs [NUM_TABS];
  WindoW                w;
  Int2                  Attribute_Page, Location_Page;

  w = NULL;
  pfp = (PubdescFormPtr) MemNew (sizeof (PubdescForm));
  if (pfp != NULL) {
    w = FixedWindow (left, top, -10, -10, title, StdCloseWindowProc);
    SetObjectExtra (w, pfp, StdFeatFormCleanupProc);
    pfp->form = (ForM) w;
    pfp->actproc = actproc;
    pfp->toform = StdSeqFeatPtrToFeatFormProc;
    pfp->fromform = NULL;
    pfp->testform = NULL;
    pfp->formmessage = PubdescFeatFormMessage;
    pfp->importform = ImportPubdescForm;
    pfp->exportform = ExportPubdescForm;

    if (pepp == NULL)
    {
      pepp = (PubdescEditProcsPtr) GetAppProperty ("PubdescEditForm");
    }

#ifndef WIN_MAC
    CreateStdEditorFormMenus (w);
#endif

    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      pfp->activate = sepp->activateForm;
      pfp->appmessage = sepp->handleMessages;
    }
    SetActivate (w, PubdescFormActivate);

    tabnumber = tabcounter[pub_choice];
    pfp->tabnumber = tabnumber;
    pfp->pub_choice = pub_choice;
    pfp->is_feat = TRUE;
    pfp->replaceAll = FALSE;
    labels = pubdescFormTabs[pub_choice];
    for (j = 0; j < NUM_TABS; j++)
    {
      tabs [j] = NULL;
    }
    for (j = 0; j < NUM_TABS && labels [j] != NULL; j++)
    {
      tabs [j] = labels [j];
    }
    tabs [tabnumber+0] = "Properties";
    tabs [tabnumber+1] = "Location";

    g = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (g, 3, 10);

    initPage = FIRST_PAGE;
    if (GetAppProperty ("InternalNcbiSequin") != NULL) {
      if (pub_choice == PUB_JOURNAL) {
        initPage = 2;
      }
    }
    pfp->foldertabs = CreateFolderTabs (g, tabs, initPage,
                         featTabsPerLine [pub_choice], 10,
                         SYSTEM_FOLDER_TAB,
                         ChangePubdescPage, (Pointer) pfp);
    pfp->currentPage = initPage;

    h = HiddenGroup (g, 0, 0, NULL);

    pfp->data = CreatePubdescDialog (h, NULL, pfp->pages,
          reftype,
          pub_status,
          pub_choice,
          flagPubDelta, flagSerial,
          pepp, pfp);

    s = HiddenGroup (h, -1, 0, NULL);
    CreateCommonFeatureGroup (s, (FeatureFormPtr) pfp, sfp, FALSE, FALSE);
    Attribute_Page = tabnumber+0;
    Location_Page = tabnumber+1;
    pfp->pages [Attribute_Page] = s;
    Hide (pfp->pages [Attribute_Page]);

    s = HiddenGroup (h, -1, 0, NULL);
    pfp->location = CreateIntervalEditorDialogEx (s, NULL, 4, 2, sep, TRUE, TRUE,
                                                  TRUE, TRUE, FALSE,
                                                  (FeatureFormPtr) pfp,
                                                  StdFeatIntEdPartialCallback);
    pfp->pages [Location_Page] = s;
    Hide (pfp->pages [Location_Page]);

    pfp->Attribute_Page = Attribute_Page;
    pfp->Location_Page = Location_Page;

    c = HiddenGroup (g, 4, 0, NULL);
    if (sfp == NULL) {
      b = PushButton (c, "Accept", ReplaceAllFormButtonProc);
      SetObjectExtra (b, pfp, NULL);
    } else if (pepp != NULL && pepp->replaceThis) {
      b = PushButton (c, "Replace All", ReplaceAllFormButtonProc);
      SetObjectExtra (b, pfp, NULL);
      b = PushButton (c, "Replace This", PubdescAcceptFormButtonProc);
      SetObjectExtra (b, pfp, NULL);
    } else {
      b = PushButton (c, "Accept", ReplaceAllFormButtonProc);
      SetObjectExtra (b, pfp, NULL);
    }
    PushButton (c, "Cancel", StdCancelButtonProc);

    AlignObjects (ALIGN_CENTER, (HANDLE) pfp->foldertabs, (HANDLE) pfp->data,
               (HANDLE) pfp->pages [Attribute_Page],
               (HANDLE) pfp->pages [Location_Page],
               (HANDLE) c, NULL);
    RealizeWindow (w);

    SendMessageToDialog (pfp->data, VIB_MSG_INIT);
    SendMessageToDialog (pfp->location, VIB_MSG_INIT);
    ChangePubdescPage ((Pointer) pfp, pfp->currentPage, 0);
    /*
    Show (pfp->pages [pfp->currentPage]);
    SendMessageToDialog (pfp->data, VIB_MSG_ENTER);
    */
    Update ();
  }
  return (ForM) w;
}

static Boolean GetCurrentPubdesc (GatherContextPtr gcp)

{
  PubdescPtr      pdp;
  PubdescPtr PNTR pdpp;
  ValNodePtr      sdp;
  SeqFeatPtr      sfp;

  pdpp = (PubdescPtr PNTR) gcp->userdata;
  if (pdpp != NULL) {
    if (gcp->thistype == OBJ_SEQDESC) {
      sdp = (ValNodePtr) gcp->thisitem;
      if (sdp != NULL) {
        pdp = (PubdescPtr) AsnIoMemCopy (sdp->data.ptrvalue,
                                         (AsnReadFunc) PubdescAsnRead,
                                         (AsnWriteFunc) PubdescAsnWrite);
        *pdpp = pdp;
        return FALSE;
      }
    } else if (gcp->thistype == OBJ_SEQFEAT) {
      sfp = (SeqFeatPtr) gcp->thisitem;
      if (sfp != NULL) {
        pdp = (PubdescPtr) AsnIoMemCopy (sfp->data.value.ptrvalue,
                                        (AsnReadFunc) PubdescAsnRead,
                                        (AsnWriteFunc) PubdescAsnWrite);
        *pdpp = pdp;
        return FALSE;
      }
    }
  }
  return TRUE;
}

static Int2  GetSerialNumber (ValNodePtr pep)

{
  CitGenPtr  cgp;
  Int2       serial_number;

  serial_number = -1;
  while (pep != NULL) {
    if (pep->choice == PUB_Gen) {
      cgp = (CitGenPtr) pep->data.ptrvalue;
      if (cgp != NULL) {
        if (cgp->serial_number != -1) {
          serial_number = cgp->serial_number;
        }
      }
    }
    pep = pep->next;
  }
  return serial_number;
}

static void SetSerialNumber (ValNodePtr pep, Int2 serial_number)

{
  CitGenPtr  cgp;

  if (serial_number != -1) {
    while (pep != NULL) {
      if (pep->choice == PUB_Gen) {
        cgp = (CitGenPtr) pep->data.ptrvalue;
        if (cgp != NULL) {
          cgp->serial_number = serial_number;
        }
      }
      pep = pep->next;
    }
  }
}

typedef struct replacealldata {
  Boolean     changeAll;
  PubdescPtr  deleteThis;
  PubdescPtr  replaceWith;
} ReplaceAllData, PNTR ReplaceAllDataPtr;

static Boolean ReplaceAllCallback (GatherContextPtr gcp)

{
  PubdescPtr         pdp;
  ValNodePtr         pep;
  ValNodePtr         ppr;
  ReplaceAllDataPtr  radp;
  ValNodePtr         sdp;
  Int2               serial_number;
  SeqFeatPtr         sfp;
  ValNodePtr         tmp;
  ValNodePtr         vnp;

  radp = (ReplaceAllDataPtr) gcp->userdata;
  if (radp != NULL) {
    pdp = NULL;
    if (gcp->thistype == OBJ_SEQDESC) {
      sdp = (ValNodePtr) gcp->thisitem;
      if (sdp != NULL && sdp->choice == Seq_descr_pub) {
        pdp = (PubdescPtr) sdp->data.ptrvalue;
      }
    } else if (gcp->thistype == OBJ_SEQFEAT) {
      sfp = (SeqFeatPtr) gcp->thisitem;
      if (sfp != NULL && sfp->data.choice == SEQFEAT_PUB) {
        pdp = (PubdescPtr) sfp->data.value.ptrvalue;
      }
    } else if (gcp->thistype == OBJ_SEQFEAT_CIT) {
      vnp = (ValNodePtr) gcp->thisitem;
      tmp = ValNodeNew (NULL);
      if (tmp != NULL) {
        tmp->choice = PUB_Equiv;
        tmp->data.ptrvalue = radp->deleteThis->pub;
        if (PubLabelMatch (tmp, vnp) == 0) {
          pep = ValNodeNew (NULL);
          if (pep != NULL) {
            tmp->choice = PUB_Equiv;
            tmp->data.ptrvalue = radp->replaceWith->pub;
            ppr = MinimizePub (tmp);
            pep->choice = PUB_Equiv;
            pep->data.ptrvalue = ppr;
            pep->next = vnp->next;
            *(gcp->prevlink) = pep;
            vnp->next = NULL;
            PubFree (vnp);
          }
        }
      }
      ValNodeFree (tmp);
      return TRUE;
    }
    if (pdp != NULL && radp->changeAll) {
      if (PubEquivMatch (pdp->pub, radp->deleteThis->pub) == 0) {
        serial_number = GetSerialNumber (pdp->pub);
        pdp->pub = PubEquivFree (pdp->pub);
        pdp->pub = AsnIoMemCopy (radp->replaceWith->pub, (AsnReadFunc) PubEquivAsnRead,
                                 (AsnWriteFunc) PubEquivAsnWrite);
        SetSerialNumber (pdp->pub, serial_number);
      }
    }
  }
  return TRUE;
}

static void SetupRAD (ReplaceAllDataPtr radp, PubdescFormPtr pfp)

{
  if (radp != NULL && pfp != NULL) {
    radp->changeAll = pfp->replaceAll;
    radp->deleteThis = NULL;
    radp->replaceWith = NULL;
    if (pfp->input_entityID != 0 &&
        (pfp->input_itemtype == OBJ_SEQDESC || pfp->input_itemtype == OBJ_SEQFEAT)) {
      radp->replaceWith = DialogToPointer (pfp->data);
      GatherItem (pfp->input_entityID, pfp->input_itemID, pfp->input_itemtype,
                  (Pointer) &(radp->deleteThis), GetCurrentPubdesc);
    }
  }
}

static void UpdateRAD (ReplaceAllDataPtr radp, PubdescFormPtr pfp)

{
  GatherScope  gs;

  if (radp != NULL && pfp != NULL) {
    if (radp->deleteThis != NULL && radp->replaceWith != NULL) {
      MemSet ((Pointer) &gs, 0, sizeof (GatherScope));
      gs.get_feats_location = TRUE;
      gs.seglevels = 1;
      GatherEntity (pfp->input_entityID, radp, ReplaceAllCallback, &gs);
    }
  }
}

static void CleanupRAD (ReplaceAllDataPtr radp)

{
  if (radp != NULL) {
    PubdescFree (radp->deleteThis);
    PubdescFree (radp->replaceWith);
  }
}

static void PubdescDescFormActnProc (ForM f)

{
  PubdescFormPtr  pfp;
  ReplaceAllData  rad;

  pfp = (PubdescFormPtr) GetObjectExtra (f);
  if (pfp != NULL) {
    SetupRAD (&rad, pfp);
    if (DescFormReplaceWithoutUpdateProc (f)) {
      UpdateRAD (&rad, pfp);
      GetRidOfEmptyFeatsDescStrings (pfp->input_entityID, NULL);
      if (GetAppProperty ("InternalNcbiSequin") != NULL) {
        ExtendGeneFeatIfOnMRNA (pfp->input_entityID, NULL);
      }
      ObjMgrSendMsg (OM_MSG_UPDATE, pfp->input_entityID,
                     pfp->input_itemID, pfp->input_itemtype);
    }
    CleanupRAD (&rad);
  }
}

static void PubdescFeatFormActnProc (ForM f)

{
  PubdescFormPtr  pfp;
  ReplaceAllData  rad;

  pfp = (PubdescFormPtr) GetObjectExtra (f);
  if (pfp != NULL) {
    SetupRAD (&rad, pfp);
    if (FeatFormReplaceWithoutUpdateProc (f)) {
      UpdateRAD (&rad, pfp);
      GetRidOfEmptyFeatsDescStrings (pfp->input_entityID, NULL);
      if (GetAppProperty ("InternalNcbiSequin") != NULL) {
        ExtendGeneFeatIfOnMRNA (pfp->input_entityID, NULL);
      }
      ObjMgrSendMsg (OM_MSG_UPDATE, pfp->input_entityID,
                     pfp->input_itemID, pfp->input_itemtype);
    }
    CleanupRAD (&rad);
  }
}

static void ProcessCitProc (ButtoN b)
{
  OMUserDataPtr         omudp;
  PubinitFormPtr        pifp;
  PubdescFormPtr        pfp;
  Uint1                 rf_value;
  Uint1                 st_value;
  Int2                  pb_value;
  Int2                  descfeat;
  WindoW                w;

  w = NULL;

  WatchCursor ();
  if (b != NULL)
  {
    pifp = (PubinitFormPtr) GetObjectExtra (b);
    if (pifp != NULL && pifp->form != NULL)
    {
      descfeat = GetValue (pifp->pub_reftype);
      switch (descfeat)
      {
        case 1:                 /* desc */
          pifp->reftype = 0;    /* sequence */
          break;
        case 2:                 /* feature */
          pifp->reftype = 0;    /* sequence */
          break;
        case 3:                 /* desc */
          pifp->reftype = 2;    /* feature */
          break;
        case 4:                 /* desc */
          pifp->reftype = 1;    /* site */
          break;
        default:
          pifp->reftype = 0;
          break;
      }
      rf_value = pifp->reftype;
      st_value = (Uint1) GetValue (pifp->pub_status);
      if (st_value > 2)
        st_value = 0;
      pb_value = GetValue (pifp->pub_choice);
      if (descfeat != 2)
      {
        w = (WindoW) CreatePubdescDescForm (-50, -33, "Citation Information",
                    rf_value, st_value, pb_value,
                    pifp->flagPubDelta,
                    pifp->flagSerial,
                    pifp->sdp, pifp->sep, PubdescDescFormActnProc,
                    NULL);
      }
      else
      {
        w = (WindoW) CreatePubdescFeatForm (-50, -33, "Citation Information",
                    rf_value, st_value, pb_value,
                    pifp->flagPubDelta,
                    pifp->flagSerial,
                    pifp->sfp, pifp->sep, PubdescFeatFormActnProc,
                    NULL);
      }
      pfp = GetObjectExtra (w);
      if (pfp != NULL)
      {
        pfp->input_entityID = pifp->input_entityID;
        pfp->input_itemID = pifp->input_itemID;
        pfp->input_itemtype = pifp->input_itemtype;
        pfp->this_itemtype = pifp->this_itemtype;
        pfp->this_subtype = pifp->this_subtype;
        pfp->procid = pifp->procid;
        pfp->proctype = pifp->proctype;
        pfp->userkey = pifp->userkey;
        omudp = ObjMgrAddUserData (pfp->input_entityID, pfp->procid,
                                   OMPROC_EDIT, pfp->userkey);

        if (omudp != NULL) {
          omudp->userdata.ptrvalue = (Pointer) pfp;
          omudp->messagefunc = StdVibrantEditorMsgFunc;
        }

        SendMessageToForm (pfp->form, VIB_MSG_INIT);
        if (pifp->sdp != NULL) {
          PointerToDialog (pfp->data, (Pointer) pifp->sdp->data.ptrvalue);
        } else if (pifp->sfp != NULL) {
          PointerToForm (pfp->form, (Pointer) pifp->sfp);
        } else if (pifp->this_itemtype == OBJ_SEQFEAT) {
          SetNewFeatureDefaultInterval ((FeatureFormPtr) pfp);
        }
      }
    }
  }
  Update ();
  Remove (ParentWindow (b));
  if (w != NULL)
  {
    Show (w);
    Select (w);
  }
  ArrowCursor ();
}

static void ChangePubStat (GrouP g)
{
  Int2                  pubstat;
  PubinitFormPtr        pifp;

  pifp = (PubinitFormPtr) GetObjectExtra (g);
  if (pifp != NULL)
  {
    pubstat = GetValue (pifp->pub_status);
    if (pubstat == 1)   /* unpublished */
    {
      SetValue (pifp->pub_choice, 0);
      Disable (pifp->pub_choice);
    }
    if (pubstat == 2 || pubstat == 3)   /* in press or published */
    {
      Enable (pifp->pub_choice);
      if (GetValue (pifp->pub_choice) == 0)
      {
        SetValue (pifp->pub_choice, 1);
      }
    }
  }
}

static void ChangePublication (GrouP g)
{
  PubinitFormPtr        pifp;
  Int2                  pb_choice;

  pifp = (PubinitFormPtr) GetObjectExtra (g);
  if (pifp != NULL)
  {
    pb_choice = GetValue (pifp->pub_choice);
    if (pb_choice != pifp->pub_choice_init)
    {
      pifp->flagPubDelta = TRUE;
    }
    else
    {
      pifp->flagPubDelta = FALSE;
    }
  }
}

static void PubdescInitPtrToPubdescInitForm (ForM w, Pointer d)
{
  PubinitFormPtr        pifp;
  PubdescPtr            pdp;
  ValNodePtr            vnp;
  CitArtPtr             cap;
  CitJourPtr            cjp;
  CitBookPtr            cbp;
  ImprintPtr            imp;
  CitGenPtr             cgp;
  Int2                  publication;
  Boolean               forcetounpub;

  pifp = (PubinitFormPtr) GetObjectExtra (w);
  pdp = (PubdescPtr) d;

  if (pifp != NULL && pdp != NULL)
  {
/* reftype */
    if (pdp->reftype <= 2)
    {
      pifp->reftype = pdp->reftype;
    }
/* publication type */
    vnp = pdp->pub;
    imp = NULL;
/* publication can be set independant of value in all but CitGen */
    forcetounpub = FALSE;
    publication = PUB_UNPUB;
    while (vnp != NULL)
    {
      switch (vnp->choice)
      {
        case PUB_Gen:
          cgp = (CitGenPtr) vnp->data.ptrvalue;
          if (cgp != NULL)
          {
/* if have to extract a serial number */
            if (cgp->serial_number > 0)
            {
              pifp->flagSerial = TRUE;
            }
/* if nothing else is available assume its a journal */
            if (publication == PUB_UNPUB)  /* if zero then set */
            {
              publication = PUB_JOURNAL;
            }
/* check for explicit unpublished */
            if (cgp->cit != NULL && cgp->cit[0] != '\0')
            {
              if (StringICmp (cgp->cit, "unpublished") == 0)
             {
               publication = PUB_UNPUB;    /* back to zero */
               forcetounpub = FALSE;
             }
            }
       } /* end if cgp */
       break;
     case PUB_Sub:
       publication = PUB_SUB;
       break;
     case PUB_Article:
       cap = (CitArtPtr) vnp->data.ptrvalue;
       switch (cap->from)
       {
         case ART_JOURNAL:
           publication = PUB_JOURNAL;
           cjp = (CitJourPtr) cap->fromptr;
           if (cjp != NULL)
           {
             imp = (ImprintPtr) cjp->imp;
           }
           break;
         case ART_BOOK:
           publication = PUB_BOOKCHPTR;
           cbp = (CitBookPtr) cap->fromptr;
           if (cbp != NULL)
           {
             imp = (ImprintPtr) cbp->imp;
           }
           break;
         case ART_PROC:
           publication = PUB_PROCCHPTR;
           cbp = (CitBookPtr) cap->fromptr;
           if (cbp != NULL)
           {
             imp = (ImprintPtr) cbp->imp;
           }
           break;
         default:
           break;
       }
       break;
     case PUB_Book:
       publication = PUB_BOOK;
       cbp = (CitBookPtr) vnp->data.ptrvalue;
       if (cbp != NULL)
       {
         imp = (ImprintPtr) cbp->imp;
       }
       break;
     case PUB_Proc:
       publication = PUB_PROC;
       cbp = (CitBookPtr) vnp->data.ptrvalue;
       if (cbp != NULL)
       {
         imp = (ImprintPtr) cbp->imp;
       }
       break;
     case PUB_Man:
       cbp = (CitBookPtr) vnp->data.ptrvalue;
       if (cbp != NULL)
       {
         if (cbp->othertype == 2)    /* 2=Cit-let */
         {
           if (cbp->let_type == 3)   /* 3=thesis */
           {
             publication = PUB_THESIS;
           }
         }
         imp = (ImprintPtr) cbp->imp;
       }
       break;
     case PUB_Patent:
       publication = PUB_PATENT;
       break;
     default:
       break;
      } /* end switch (vnp->choice) */
      vnp = vnp->next;
    } /* end while */

    if (forcetounpub) {
      publication = PUB_UNPUB;    /* back to zero */
    }

/* unpublished inpress published only */
/* can have Auth/Title/Jour (submitted) but is NOT published or inpress */
/* need an imprint to define status */
    pifp->pub_choice_init = publication;
    SetValue (pifp->pub_choice, publication);
    if (publication == PUB_SUB)
    {
      SetValue (pifp->pub_status, 1);           /* unpublished for sure */
      Disable (pifp->pub_status);               /* can't switch nuthin */
      Disable (pifp->pub_reftype);
    }
    if (publication > 0 && publication != PUB_SUB)
    {
      if (imp != NULL)
      {
        Enable (pifp->pub_choice);
        if (imp->prepub == 0)                   /* published */
          SetValue (pifp->pub_status, 3);
        if (imp->prepub == 2)                   /* inpress */
          SetValue (pifp->pub_status, 2);
      }
      if (publication == PUB_PATENT)    /* patents have to be published? */
      {
        Enable (pifp->pub_choice);
        SetValue (pifp->pub_status, 3);
      }
    }
  } /* end if (pifp) */
  return;
}

typedef struct repauthdata {
  AuthListPtr     replaceThis;
  AuthListPtr     withThis;
} RepAuthData, PNTR RepAuthPtr;

static void ReplaceOneAuthList (AuthListPtr PNTR alpp, RepAuthPtr rap)

{
  AuthListPtr  alp;
  AffilPtr     ap;

  if (alpp == NULL || *alpp == NULL || rap == NULL) return;
  if (rap->replaceThis == NULL || rap->withThis == NULL) return;
  if (AuthListMatch (*alpp, rap->replaceThis, TRUE) != 0) return;
  alp = *alpp;
  ap = alp->affil;
  alp->affil = NULL;
  alp = AuthListFree (alp);
  alp = AsnIoMemCopy (rap->withThis,
                      (AsnReadFunc) AuthListAsnRead,
                      (AsnWriteFunc) AuthListAsnWrite);
  alp->affil = ap;
  *alpp = alp;
}

static void ReplaceOneAuthListPub (ValNodePtr vnp, RepAuthPtr rap)

{
  CitArtPtr   cap;
  CitBookPtr  cbp;
  CitGenPtr   cgp;
  CitPatPtr   cpp;
  CitSubPtr   csp;

  if (vnp == NULL) return;
  if (vnp->choice == PUB_PMid || vnp->choice == PUB_Muid) return;
  if (vnp->data.ptrvalue == NULL) return;
  switch (vnp->choice) {
    case PUB_Gen :
      cgp = (CitGenPtr) vnp->data.ptrvalue;
      ReplaceOneAuthList (&(cgp->authors), rap);
      break;
    case PUB_Sub :
      csp = (CitSubPtr) vnp->data.ptrvalue;
      ReplaceOneAuthList (&(csp->authors), rap);
      break;
    case PUB_Article :
      cap = (CitArtPtr) vnp->data.ptrvalue;
      ReplaceOneAuthList (&(cap->authors), rap);
      break;
    case PUB_Book :
      cbp = (CitBookPtr) vnp->data.ptrvalue;
      ReplaceOneAuthList (&(cbp->authors), rap);
      break;
    case PUB_Proc :
      cbp = (CitBookPtr) vnp->data.ptrvalue;
      ReplaceOneAuthList (&(cbp->authors), rap);
      break;
    case PUB_Man :
      cbp = (CitBookPtr) vnp->data.ptrvalue;
      if (cbp->othertype == 2 && cbp->let_type == 3) {
        ReplaceOneAuthList (&(cbp->authors), rap);
      }
      break;
    case PUB_Patent :
      cpp = (CitPatPtr) vnp->data.ptrvalue;
      ReplaceOneAuthList (&(cpp->authors), rap);
      ReplaceOneAuthList (&(cpp->applicants), rap);
      ReplaceOneAuthList (&(cpp->assignees), rap);
      break;
    default :
      break;
  }
}

static Boolean ReplaceAuthList (GatherContextPtr gcp)

{
  PubdescPtr  pdp;
  RepAuthPtr  rap;
  ValNodePtr  sdp;
  SeqFeatPtr  sfp;
  ValNodePtr  vnp;

  if (gcp == NULL) return TRUE;

  rap = (RepAuthPtr) gcp->userdata;
  if (rap == NULL ) return TRUE;

  pdp = NULL;
  if (gcp->thistype == OBJ_SEQFEAT) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp != NULL && sfp->data.choice == SEQFEAT_PUB) {
      pdp = (PubdescPtr) sfp->data.value.ptrvalue;
    }
  } else if (gcp->thistype == OBJ_SEQDESC) {
    sdp = (ValNodePtr) gcp->thisitem;
    if (sdp != NULL && sdp->choice == Seq_descr_pub) {
      pdp = (PubdescPtr) sdp->data.ptrvalue;
    }
  }
  if (pdp == NULL) return TRUE;
  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    ReplaceOneAuthListPub (vnp, rap);
  }
  return TRUE;
}

static void ReplaceAuthors (ButtoN b)

{
  AuthListPtr     alp;
  GatherScope     gs;
  PubdescFormPtr  pfp;
  PubdescPagePtr  ppp;
  RepAuthData     rad;

  pfp = (PubdescFormPtr) GetObjectExtra (b);
  if (pfp == NULL) return;
  ppp = (PubdescPagePtr) GetObjectExtra (pfp->data);
  if (ppp == NULL) return;
  alp = (AuthListPtr) DialogToPointer (ppp->author_list);
  if (alp == NULL) return;
  if (ppp->originalAuthList == NULL) return;
  Hide (pfp->form);
  rad.replaceThis = ppp->originalAuthList;
  rad.withThis = alp;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  MemSet((Pointer) (gs.ignore), (int) (TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQDESC] = FALSE;
  GatherEntity (pfp->input_entityID, (Pointer) (&rad), ReplaceAuthList, &gs);
  ObjMgrSetDirtyFlag (pfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, pfp->input_entityID, 0, 0);
  Remove (pfp->form);
  Update ();
}

static void MakeReplaceAuthorsForm (ButtoN b)

{
  ButtoN               b1;
  GrouP                c;
  GrouP                p;
  PubdescEditProcsPtr  pepp;
  PubdescFormPtr       pfp;
  PubinitFormPtr       pifp;
  PubdescPagePtr       ppp;
  StdEditorProcsPtr    sepp;
  WindoW               w;

  pifp = (PubinitFormPtr) GetObjectExtra (b);
  if (pifp == NULL) return;

  pfp = (PubdescFormPtr) MemNew (sizeof (PubdescForm));
  if (pfp == NULL) return;

  Hide (ParentWindow (b));
  Update ();

  w = FixedWindow (-50, -33, -10, -10, "Global Author Replace", StdCloseWindowProc);
  SetObjectExtra (w, pfp, StdCleanupFormProc);
  pfp->form = (ForM) w;
  pfp->actproc = NULL;
  pfp->toform = NULL;
  pfp->fromform = NULL;
  pfp->testform = NULL;
  pfp->formmessage = PubdescDescFormMessage;
  pfp->importform = NULL;
  pfp->exportform = NULL;

  pepp = (PubdescEditProcsPtr) GetAppProperty ("PubdescEditForm");

#ifndef WIN_MAC
  CreateStdEditorFormMenus (w);
#endif

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    pfp->activate = sepp->activateForm;
    pfp->appmessage = sepp->handleMessages;
  }
  SetActivate (w, PubdescFormActivate);

  p = HiddenGroup (w, 2, 0, NULL);
  ppp = (PubdescPagePtr) MemNew (sizeof (PubdescPage));
  if (ppp != NULL) {
    SetObjectExtra (p, ppp, CleanupPubdescDialog);
    ppp->dialog = (DialoG) p;
    ppp->todialog = PubdescPtrToPubdescPage;
    ppp->fromdialog = PubdescPageToPubdescPtr;
    ppp->testdialog = NULL;
    ppp->author_list = CreateAuthorDialog (p, 3, -1);
  }
  pfp->data = (DialoG) p;

  c = HiddenGroup (w, 2, 0, NULL);
  b1 = PushButton (c, "Accept", ReplaceAuthors);
  SetObjectExtra (b1, pfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) pfp->data, (HANDLE) c, NULL);
  RealizeWindow (w);

  pfp->input_entityID = pifp->input_entityID;
  pfp->input_itemID = pifp->input_itemID;
  pfp->input_itemtype = pifp->input_itemtype;
  pfp->this_itemtype = pifp->this_itemtype;
  pfp->this_subtype = pifp->this_subtype;

  SendMessageToForm (pfp->form, VIB_MSG_INIT);
  if (pifp->sdp != NULL) {
    PointerToDialog (pfp->data, (Pointer) pifp->sdp->data.ptrvalue);
  } else if (pifp->sfp != NULL) {
    PointerToDialog (pfp->data, (Pointer) pifp->sfp->data.value.ptrvalue);
  }

  Remove (ParentWindow (b));
  Update ();
  Show (w);
  Select (w);
}

extern ForM CreatePubdescInitForm (Int2 left, Int2 top, CharPtr title,
                         ValNodePtr sdp, SeqFeatPtr sfp,
                         SeqEntryPtr sep, Uint2 itemtype,
                         FormActnFunc actproc,
                         PubdescEditProcsPtr pepp)
{
  ButtoN                b, b1, b2, b3, b4;
  GrouP                 c, c1;
  GrouP                 h1;
  GrouP                 g1, g2, g3, g4, g5, g7;
  PubdescPtr            pdp;
  PubinitFormPtr        pifp;
  WindoW                w;
  ValNodePtr            vnp;

  w = NULL;
  pifp = (PubinitFormPtr) MemNew (sizeof (PubinitForm));
  pifp->flagPubDelta = FALSE;
  pifp->flagSerial = FALSE;
  if (pifp != NULL)
  {
    w = FixedWindow (left, top, -10, -10, title, StdCloseWindowProc);
    SetObjectExtra (w, pifp, StdCleanupFormProc);
    pifp->form = (ForM) w;
    pifp->actproc = actproc;
    pifp->toform = PubdescInitPtrToPubdescInitForm;
    pifp->fromform = NULL;
    pifp->testform = NULL;

    pifp->sep = sep;
    pifp->sdp = sdp;
    pifp->sfp = sfp;

    pdp = NULL;
    if (sdp != NULL)
    {
      pdp = (PubdescPtr) sdp->data.ptrvalue;
    }
    else if (sfp != NULL)
    {
      pdp = (PubdescPtr) sfp->data.value.ptrvalue;
    }

    h1 = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (h1, 10, 20);
    g2 = NormalGroup (h1, -1, 0, "Status", programFont, NULL);
    g4 = HiddenGroup (g2, -4, 0, ChangePubStat);
    pifp->pub_status = g4;
    SetObjectExtra (g4, pifp, NULL);
    RadioButton (g4, "Unpublished");
    RadioButton (g4, "In Press");
    RadioButton (g4, "Published");
    SetValue (g4, 1);           /* unpublished */
    g1 = NormalGroup (h1, -1, 0, "Class", programFont, NULL);
    g5 = HiddenGroup (g1, -4, 0, ChangePublication);
    pifp->pub_choice = g5;
    SetObjectExtra (g5, pifp, NULL);

    RadioButton (g5, "Journal");
    RadioButton (g5, "Book Chapter");
    RadioButton (g5, "Book");
    RadioButton (g5, "Thesis/Monograph");
    RadioButton (g5, "Proceedings Chapter");
    RadioButton (g5, "Proceedings");
    RadioButton (g5, "Patent");
    if (GetAppProperty ("InternalNcbiSequin") != NULL) {
      RadioButton (g5, "Submission");
    } else if (pdp != NULL)
    {
      vnp = pdp->pub;
      while (vnp != NULL)
      {
        if (vnp->choice == PUB_Sub)
        {
          RadioButton (g5, "Submission");
          break;
        }
        vnp = vnp->next;
      }
    }
    Disable (g5);               /* publications disabled */
    g3 = NormalGroup (h1, -1, 0, "Scope", programFont, NULL);
    g7 = HiddenGroup (g3, -1, 0, NULL);
    pifp->pub_reftype = g7;
    SetObjectExtra (g7, pifp, NULL);
    b1 = RadioButton (g7, "Refers to the entire sequence");
    b2 = RadioButton (g7, "Refers to part of the sequence");
    b3 = RadioButton (g7, "Cites a feature on the sequence");

    b4 = NULL;
    if (pdp != NULL && pdp->reftype == 1)
    {
      b4 = RadioButton (g7, "Citation history lost");
    }

    if (pdp != NULL)
    {
      /* Disable (g7); */     /* can't switch reftypes of existing pubdescs */
      if (sdp != NULL)  /* seqdesc */
      {
        switch (pdp->reftype) {
          case 0:
            SetValue (g7, 1);
            break;
          case 1:
            SetValue (g7, 4);
            break;
          case 2:
            SetValue (g7, 3);
            break;
          default:
            SetValue (g7, 1);
            break;
        }
        Disable (b2);
      } else if (sfp != NULL) { /* seqfeat */
        SetValue (g7, 2);
        Disable (b1);
        Disable (b3);
        Disable (b4);
      } else {
        SetValue (g7, 1);
        Disable (g7);
      }
    } else if (itemtype == OBJ_SEQFEAT) {
      SetValue (g7, 2);
      Disable (b1);
      Disable (b3);
      Disable (b4);
    } else {
      SetValue (g7, 1);
      Disable (b2);
    }

    c = HiddenGroup (h1, 2, 0, NULL);
    b = PushButton (c, "Proceed", ProcessCitProc);
    SetObjectExtra (b, pifp, NULL);
    PushButton (c, "Cancel", StdCancelButtonProc);

    c1 = NULL;
    if (pdp != NULL) {
      if (pepp == NULL) {
        pepp = (PubdescEditProcsPtr) GetAppProperty ("PubdescEditForm");
      }
      if (pepp != NULL && pepp->replaceThis) {
        c1 = HiddenGroup (h1, 2, 0, NULL);
        b = PushButton (c1, "Replace Authors", MakeReplaceAuthorsForm);
        SetObjectExtra (b, pifp, NULL);
      }
    }

    /*
    AlignObjects (ALIGN_CENTER, (HANDLE) g2,
                    (HANDLE) pifp->pub_status, NULL);
    AlignObjects (ALIGN_CENTER, (HANDLE) g1,
                    (HANDLE) pifp->pub_choice, NULL);
    AlignObjects (ALIGN_CENTER, (HANDLE) g3,
                    (HANDLE) pifp->pub_reftype, NULL);
    */
    AlignObjects (ALIGN_CENTER, (HANDLE) g2, (HANDLE) g1, (HANDLE) g3,
                    (HANDLE) c, (HANDLE) c1, NULL);
    RealizeWindow (w);
  }
  return (ForM) w;
}

extern Int2 LIBCALLBACK PubdescGenFunc (Pointer data)

{
  HelpMessageFunc       helpfunc;
  Uint2                 itemtype;
  OMProcControlPtr      ompcp;
  OMUserDataPtr         omudp;
  PubinitFormPtr        pifp;
  PubdescFormPtr        pfp;
  ObjMgrProcPtr         proc;
  ValNodePtr            sdp;
  SeqEntryPtr           sep;
  SeqFeatPtr            sfp;
  Uint2                 subtype;
  WindoW                w;

  ompcp = (OMProcControlPtr) data;
  w = NULL;
  sdp = NULL;
  sfp = NULL;
  sep = NULL;
  itemtype = 0;
  subtype = 0;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  proc = ompcp->proc;
  switch (ompcp->input_itemtype)
  {
    case OBJ_SEQFEAT :
      sfp = (SeqFeatPtr) ompcp->input_data;
      if (sfp != NULL && sfp->data.choice != SEQFEAT_PUB)
      {
        return OM_MSG_RET_ERROR;
      }
      itemtype = OBJ_SEQFEAT;
      subtype = FEATDEF_PUB;
      break;
    case OBJ_SEQDESC :
      sdp = (ValNodePtr) ompcp->input_data;
      if (sdp != NULL && sdp->choice != Seq_descr_pub)
      {
        return OM_MSG_RET_ERROR;
      }
      itemtype = OBJ_SEQDESC;
      subtype = Seq_descr_pub;
      break;
    case OBJ_BIOSEQ :
      break;
    case OBJ_BIOSEQSET :
      break;
    case 0 :
      break;
    default :
      return OM_MSG_RET_ERROR;
  }
  omudp = ItemAlreadyHasEditor (ompcp->input_entityID, ompcp->input_itemID,
                                ompcp->input_itemtype, ompcp->proc->procid);
  if (omudp != NULL) {
    pfp = (PubdescFormPtr) omudp->userdata.ptrvalue;
    if (pfp != NULL) {
      Select (pfp->form);
    }
    return OM_MSG_RET_DONE;
  }
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  if (sfp != NULL)
  {
    w = (WindoW) CreatePubdescInitForm (-50, -33, "Citation on Feature",
          sdp, sfp, sep, itemtype, PubdescFeatFormActnProc, NULL);
  }
  else if (sdp != NULL)
  {
    w = (WindoW) CreatePubdescInitForm (-50, -33, "Citation on Entry",
          sdp, sfp, sep, itemtype, PubdescDescFormActnProc, NULL);
  }
  else
  {
    itemtype = proc->inputtype;
    subtype = proc->subinputtype;
    if (itemtype == OBJ_SEQFEAT && subtype == FEATDEF_PUB)
    {
      w = (WindoW) CreatePubdescInitForm (-50, -33, "Citation on Feature",
          sdp, sfp, sep, itemtype, PubdescFeatFormActnProc, NULL);
    }
    else if (itemtype == OBJ_SEQDESC && subtype == Seq_descr_pub)
    {
      w = (WindoW) CreatePubdescInitForm (-50, -33, "Citation on Entry",
          sdp, sfp, sep, itemtype, PubdescDescFormActnProc, NULL);
    }
    else
    {
      return OM_MSG_RET_ERROR;
    }
  }

  pifp = (PubinitFormPtr) GetObjectExtra (w);
  if (pifp != NULL)
  {
    pifp->input_entityID = ompcp->input_entityID;
    pifp->input_itemID = ompcp->input_itemID;
    pifp->input_itemtype = ompcp->input_itemtype;
    pifp->this_itemtype = itemtype;
    pifp->this_subtype = subtype;
    pifp->procid = ompcp->proc->procid;
    pifp->proctype = ompcp->proc->proctype;
    pifp->userkey = OMGetNextUserKey ();

    SendMessageToForm (pifp->form, VIB_MSG_INIT);
    if (sdp != NULL)
    {
      PointerToForm (pifp->form, (Pointer) sdp->data.ptrvalue);
      SetClosestParentIfDuplicating ((BaseFormPtr) pifp);
    }
    else if (sfp != NULL)
    {
      PointerToForm (pifp->form, (Pointer) sfp->data.value.ptrvalue);
      SetClosestParentIfDuplicating ((BaseFormPtr) pifp);
    }
  }
  Show (w);
  Select (w);
  helpfunc = (HelpMessageFunc) GetAppProperty ("HelpMessageProc");
  if (helpfunc != NULL) {
    helpfunc ("Publications", NULL);
  }
  return OM_MSG_RET_DONE;
}

/* RELAXED QUERY SECTION - Hanzhen Sun */

#include <sequtil.h>
#include <ent2api.h>
#include <vibrant.h>
#include <document.h>
#include <asn.h>
/*  not sure this is needed*/
#include <pmfapi.h>
#include <dlogutil.h>


/****** global structures for the program***************/

typedef struct citart_inpress_struct {

  FILE *logfile;	
  Boolean error;	
      
  CharPtr   f_last_name, f_first_name, l_last_name, l_first_name; 
  CharPtr  jour_title, jour_volume,  jour_page, art_title;

  Int2 year;
  Int1 rank;

  ByteStorePtr uids_bs;  /*pmids*/

} CitArtInPress, PNTR CitArtInPressPtr;

static ParData txtParFmt = {FALSE, TRUE, TRUE, TRUE, TRUE, 0, 0};
static ColData txtColFmt = {0, 0, 80, 4, NULL, 'l', TRUE, FALSE, FALSE, FALSE, TRUE};

typedef struct citationupdateform {
  FORM_MESSAGE_BLOCK

  TexT            count;

  ButtoN          First_Author;
  ButtoN          Last_Author;
  ButtoN          Journal;
  ButtoN          Volume;
  ButtoN          Page;
  ButtoN          Year;

  TexT            f_auth_text;
  TexT            l_auth_text;
  TexT            journal_text;
  TexT            volume_text;
  TexT            page_text;

  TexT            year_text;

  TexT            art_title_text;
  ButtoN          Extra_Term;
  TexT            extra_term_text;

  PopuP           new_query;
  DoC             rdoc;          
  ButtoN          action;           

  Pointer         userdata;

  CitArtInPressPtr caipp;

} CitationUpdateForm, PNTR CitationUpdateFormPtr;



/****** prototypes for GUI setup***************/

static void PopulateWindow PROTO(( WindoW w, PubdescPtr pdp));
static void UpdateWindow PROTO(( CitationUpdateFormPtr cufp , CitArtInPressPtr caipp));
static void Quit PROTO((ButtoN b));



/****** prototypes for user action CB***************/

void Accept PROTO((ButtoN b));

static void  SendQuery PROTO((ButtoN b));
static void  CreateDocSum  PROTO((ByteStorePtr uids_bs, DoC doc, TexT count));
static void  DisplayDocSum PROTO((CharPtr docSum, DoC dp));
static void  MyNotify PROTO((DoC d, Int2 item, Int2 row, Int2 col, Boolean dblclick));


/*****************************************************************************
*
*  Function:   GetUidListFromE2Request
*              executes entrez2 query  passed in,
               and get back the uids from entrezReplyPtr. 
********************************
*  Argument:   CitArtInPressPtr
*
*  Returns:    void
*
*****************************************************************************/
static void GetUidListFromE2Request (CitArtInPressPtr caipp, Entrez2RequestPtr e2rp, DoC rdoc) {

  Entrez2ReplyPtr         e2ry;
  Entrez2BooleanReplyPtr  e2br;
  Entrez2IdListPtr        e2idlist;
 
  /*  feed back to user, in case it takes long*/
  Reset (rdoc);
  Update ();                 

  AppendText (rdoc, "Query submitted to Entrez2 server, waiting for reply\n\n", NULL, NULL, NULL);

  InvalDocument (rdoc);   /*Invalidates visible area of a document ??*/
  ArrowCursor ();
  Update ();


  /*submit query, extract reply and get the uids*/
  if (e2rp == NULL) return;
  e2ry = EntrezSynchronousQuery (e2rp);

  e2rp = Entrez2RequestFree (e2rp);
  if (e2ry == NULL) return;
  e2br = EntrezExtractBooleanReply (e2ry);      /* get the reply part of it*/
  if (e2br == NULL) return;


  if (e2br->count > 0 &&  e2br->uids != NULL) {
    e2idlist = e2br->uids;
    if (e2idlist != NULL && e2idlist->num > 0 &&  e2idlist->uids != NULL) {

      /* here, uids are Pmids, not Muids.*/
      caipp->uids_bs = e2idlist->uids;
    }
  }
  /*here, need to update uids_bs, if the count is 0 ***/
  else { caipp->uids_bs = NULL; }
}


/*****************************************************************************
*  Function:     DisplayDocSum
*  Description:	
*  Argument:     CharPtr, Doc
*
*  Returns:	 void
*
*****************************************************************************

    really, should be implemented inside CreateDocSum for efficiency.???

*****************************************************************************/
static void DisplayDocSum(CharPtr docSum, DoC dp)
{
  Int2  lineheight;

  SelectFont (programFont);
  lineheight = LineHeight ();
  SelectFont (systemFont);

  SetDocDefaults (dp, &txtParFmt, &txtColFmt, programFont);
  
  SetDocNotify (dp, MyNotify);
  SetDocAutoAdjust (dp, FALSE);


  if (docSum == NULL) {
    AppendText (dp, "NO Docment Summary returned for this citation", NULL, NULL, NULL);
  }
  AppendText (dp, docSum, &txtParFmt, &txtColFmt, programFont );

  AdjustDocScroll (dp);
}


/*****************************************************************************
*
*  Function:   CreateDocSum
*
*              create docSums on a list of Uids, call display funcion for them.
*              displays count of Uids into the textfield.
* 
********************************
*  Argument:   ByteStorePtr, DoC,  TexT
*
*  Returns:   
*
***************************************************************************/
static void  CreateDocSum  (ByteStorePtr uids_bs, DoC doc, TexT count_text) 
{ 

  Uint4                   uid = 0;
  Int4                    i = 0;   
  ByteStorePtr bs = uids_bs;
  Int2             count = BSLen(bs)/ sizeof(uid); 
  CharPtr count_str, sumStr = NULL;

  Entrez2DocsumPtr      dsp;
  Entrez2DocsumListPtr  e2dl;
  Entrez2RequestPtr     e2rp;
  Entrez2ReplyPtr       e2ry;
  Entrez2DocsumDataPtr  e2ddp;

  CharPtr author="No Author Available",
          title="No Title Available",
          source="No Source Available",
          volume="No Volume Available",
          pages="No Page Available",
          year_str="No Year Available",
          tmp = NULL;
  Int2 size = 0, year = -1;

  /* no citation hit returned */
  if  (uids_bs == NULL) {
    Reset (doc);
    Update ();                 
    
    AppendText (doc, "No match was found, hints for a modified query:\n Leave out one author\n Leave out year field (or increase it by 1) \n Leave out Volume field \n Modify the fields as you deem sensible", NULL, NULL, systemFont);
    
    InvalDocument (doc);   
    ArrowCursor ();
    Update ();

    SafeSetTitle (count_text, "0");
    
    return;
  }

  BSSeek (bs, 0, SEEK_SET);

  count_str = MemNew(5);
  sprintf(count_str, "%d", count);  
  SafeSetTitle (count_text, count_str);
 
  Reset (doc);
  Update ();

  for (i = 0; i <count; i++) {
    /*read one uid*/
    BSRead (bs, &uid, sizeof (Uint4));

    /*not familiar with this function yet.*/
    e2rp = EntrezCreateDocSumRequest ("PubMed", uid, 0, NULL, NULL);
    
    /*  e2rp = EntrezCreateDocSumRequest ("PubMed", uid, num, uids, NULL); */
    if (e2rp == NULL) return;
    
    e2ry =  EntrezSynchronousQuery(e2rp);
    e2rp = Entrez2RequestFree(e2rp);
    e2dl = EntrezExtractDocsumReply (e2ry);
    
    if (e2dl == NULL) return;
    
    for (dsp = e2dl->list; dsp != NULL; dsp = dsp->next)
      {
        for (e2ddp = dsp->docsum_data; e2ddp != NULL; e2ddp = e2ddp->next) {
          if (StringHasNoText (e2ddp->field_value)) continue;
          if (StringICmp (e2ddp->field_name, "Authors") == 0) {
            author = e2ddp->field_value;
            if (author != NULL) {
              tmp = StringChr (author, ',');
              if (tmp != NULL) {
                *tmp = '\0';
              }
            }
          } else if (StringICmp (e2ddp->field_name, "Title") == 0) {
            title = e2ddp->field_value;
          } else if (StringICmp (e2ddp->field_name, "Source") == 0) {
            source = e2ddp->field_value;
          } else if (StringICmp (e2ddp->field_name, "Volume") == 0) {
            volume = e2ddp->field_value;
          } else if (StringICmp (e2ddp->field_name, "Pages") == 0) {
            pages = e2ddp->field_value;
          } else if (StringICmp (e2ddp->field_name, "PubDate") == 0) {
            year_str = e2ddp->field_value;
            if (year_str != NULL) {
              tmp = StringChr (year_str, ' ');
              if (tmp != NULL) {
                *tmp = '\0';
              }
            }
          }
        }

	size =  StringLen(author)+ StringLen(title)+ StringLen(source)+  StringLen(volume)+ StringLen(pages);

	/* account for 4 new line char.s, PMID, etc!!*/
	sumStr = MemNew(size + 40);
	       
 	sprintf(sumStr, "%s\n%s\n%s. %s; %s:%s\nPMID: %d \n\n", author, title, source,year_str, volume, pages, uid);

	/*add each sumStr to DoC object by AppendText.*/
	DisplayDocSum (sumStr, doc);
	MemFree(sumStr);
      }   
  }
 
  InvalDocument (doc);   /*Invalidates visible area of a document ??*/
  ArrowCursor ();
  Update ();
  /*update Highlighted item (none) */
  SetDocHighlight (doc, 0, 0);
    
  Entrez2DocsumListFree (e2dl);
 
  BSSeek (bs, 0, SEEK_SET); /* reset bs read position*/      
}




/*non-PROTOtyped local functions*/
static  void AddAuthor(Entrez2RequestPtr e2rp, CharPtr term, Boolean is_1st)
{
  if (StringHasNoText (term)) return;
  if (!is_1st) {
    EntrezAddToBooleanRequest (e2rp, NULL, ENTREZ_OP_AND, NULL, NULL, NULL, 0, 0, NULL, NULL, FALSE, FALSE);  
  }
  EntrezAddToBooleanRequest (e2rp, NULL, 0, "AUTH", term, NULL, 0, 0, NULL, NULL, FALSE, FALSE);
}

static  void AddJournal(Entrez2RequestPtr e2rp, CharPtr term, Boolean is_1st)
{
  if (StringHasNoText (term)) return;
  if (!is_1st) {
    EntrezAddToBooleanRequest (e2rp, NULL, ENTREZ_OP_AND, NULL, NULL, NULL, 0, 0, NULL, NULL, FALSE, FALSE);  
  }
  EntrezAddToBooleanRequest (e2rp, NULL, 0, "JOUR", term, NULL, 0, 0, NULL, NULL, FALSE, FALSE);
}

static  void AddVolume(Entrez2RequestPtr e2rp, CharPtr term, Boolean is_1st)
{
  if (StringHasNoText (term)) return;
  if (!is_1st) {
    EntrezAddToBooleanRequest (e2rp, NULL, ENTREZ_OP_AND, NULL, NULL, NULL, 0, 0, NULL, NULL, FALSE, FALSE);  
  }
  EntrezAddToBooleanRequest (e2rp, NULL, 0, "VOLUME",term, NULL, 0, 0, NULL, NULL, FALSE, FALSE);
}

static  void AddPage(Entrez2RequestPtr e2rp, CharPtr term, Boolean is_1st)
{
  if (StringHasNoText (term)) return;
  if (!is_1st) {
    EntrezAddToBooleanRequest (e2rp, NULL, ENTREZ_OP_AND, NULL, NULL, NULL, 0, 0, NULL, NULL, FALSE, FALSE);  
  }
  EntrezAddToBooleanRequest (e2rp, NULL, 0, "PAGE",term, NULL, 0, 0, NULL, NULL, FALSE, FALSE);
}

static  void AddYear(Entrez2RequestPtr e2rp, CharPtr term, Boolean is_1st)
{
  if (StringHasNoText (term)) return;
  if (!is_1st) {
    EntrezAddToBooleanRequest (e2rp, NULL, ENTREZ_OP_AND, NULL, NULL, NULL, 0, 0, NULL, NULL, FALSE, FALSE);  
  }
  EntrezAddToBooleanRequest (e2rp, NULL, 0, "DP", term, NULL, 0, 0, NULL, NULL, FALSE, FALSE);
}

static  void AddAll(Entrez2RequestPtr e2rp, CharPtr term, Boolean is_1st)
{
  if (StringHasNoText (term)) return;
  if (!is_1st) {
    EntrezAddToBooleanRequest (e2rp, NULL, ENTREZ_OP_AND, NULL, NULL, NULL, 0, 0, NULL, NULL, FALSE, FALSE);  
  }
  EntrezAddToBooleanRequest (e2rp, NULL, 0, "ALL",term, NULL, 0, 0, NULL, NULL, FALSE, FALSE);
}




/*****************************************************************************
*
*  Function:   FormDefaultQuery
*              constructs entrez2 query from information passed in, execute
*              the query, and get back the uids from entrezReplyPtr. 
********************************pass uids back as bs of caipp. returns void.
*  Argument:   CitArtInPressPtr
*
*  Returns:    void
*
*****************************************************************************/
static void FormDefaultQuery (Entrez2RequestPtr e2rp, CitArtInPressPtr caipp) {

  Char  year [16];
 
  if (caipp->year != -1) {
    sprintf(year, "%d", (int) caipp->year); 
  }

  /* add all available fields as indicated by rank*/
  if (caipp->rank == 10) {      /* add title, page, vol */

    AddJournal(e2rp,  caipp->jour_title, TRUE);  /* add first term*/
    AddVolume(e2rp,  caipp->jour_volume, FALSE);  
    AddPage(e2rp,  caipp->jour_page, FALSE); 
  }
  
  else {    
    
    if (caipp->rank > 4) {    /* add author in all cases*/

      /***** term  can be capital letter or lower case,  (ENTREZ_OP_NONE is 0)  */
      AddAuthor(e2rp,  caipp->f_last_name, TRUE);  /* add first term */

      if(StringCmp(caipp->l_last_name, "null") && StringCmp(caipp->l_last_name, caipp->f_last_name)) {
      AddAuthor(e2rp,  caipp->l_last_name, FALSE); 
      }
    }
    
    if (caipp->rank == 6) {  /* add year, takes StringSave(year-1900)?? */   
      AddYear(e2rp,  year, FALSE);        
    }     

    if (caipp->rank > 6) {  /* add jour_title */
      AddJournal(e2rp,  caipp->jour_title, FALSE);  /* NOT first term*/
    }
    
    if (caipp->rank == 9) {       /* add vol and year */
      AddVolume(e2rp,  caipp->jour_volume, FALSE);  
      AddYear(e2rp,  year, FALSE); 
    }

    if (caipp->rank == 8) {  /* add year or volume */
      
      if (caipp->year != -1) {
	AddYear(e2rp,  year, FALSE); 	
      }
      else {
	AddVolume(e2rp,  caipp->jour_volume, FALSE);
      }		       
    }   /* end rank 8*/ 		     
  }  
}	



/*****************************************************************************
*  Function:     SendQuery
*
*  Description:  SendQuery CB, form query according to the choice made on the
*                popup menu, call DisplayEntrezReply.
*
********************************
*  Argument:     ButtoN b
*  
*  Returns:      void
*
***************************************************************************/
static void SendQuery (ButtoN b)

{
  Int2               choice;     
  CitationUpdateFormPtr  cufp;
  CitArtInPressPtr caipp;        /* local caipp, make deref coding easier*/

  Entrez2RequestPtr  e2rp;


  Char               f_auth [64];
  Char               l_auth [64];
  Char               journal [64];
  Char               volume [64];
  Char               page [64];
  Char               year [64];
  Char               new_term [256];

  Boolean            is_first_term = TRUE;


  cufp = (CitationUpdateFormPtr) GetObjectExtra (b);
  if (cufp == NULL) return;

  caipp = cufp->caipp;
  if (caipp == NULL) return;

  Reset (cufp->rdoc);
  Update ();                 

  choice = GetValue (cufp->new_query);

  /*** trick:  must make sure to add Op : AND, after the first term, can't
   add one before!! */

  if (choice == 1) {               /* precess default query*/

    e2rp = EntrezCreateBooleanRequest (TRUE, FALSE, "PubMed", NULL, 0, 0, NULL, 20, 0);

    FormDefaultQuery (e2rp,  caipp);
    

    /* add exta term (user supplied) as ALL, no specific field */
    if ( GetStatus (cufp->Extra_Term) ) {
      GetTitle (cufp->extra_term_text, new_term, sizeof (f_auth));
      AddAll(e2rp, new_term, FALSE);
      is_first_term = FALSE;
    }
  }
     
  if (choice == 2) {        /*user specify query*/

    /* for new term in query, add to [all], then automatically go through Term mapping*/
    /* add exta term (user supplied) as ALL, no specific field */
    /* debug:  assuming this is a well-formed query string, let the add func. parse it*/
    if ( GetStatus (cufp->Extra_Term) ) {
      GetTitle (cufp->extra_term_text, new_term, sizeof (f_auth));
      e2rp = EntrezCreateBooleanRequest (TRUE, FALSE, "PubMed", new_term, 0, 0, NULL, 20, 0); 
      is_first_term = FALSE;
    }
    else{
      e2rp = EntrezCreateBooleanRequest (TRUE, FALSE, "PubMed", NULL, 0, 0, NULL, 20, 0);
    }

    if ( GetStatus (cufp->First_Author) ) {
      GetTitle (cufp->f_auth_text, f_auth, sizeof (f_auth));
      AddAuthor(e2rp, f_auth, is_first_term);
      is_first_term = FALSE;
    }

    if ( GetStatus (cufp->Last_Author) ) {
      GetTitle (cufp->l_auth_text, l_auth, sizeof (l_auth));
      AddAuthor(e2rp, l_auth, is_first_term);    
      is_first_term = FALSE;
    }

    if ( GetStatus (cufp->Journal) ) {
      GetTitle (cufp->journal_text, journal, sizeof (journal));
      AddJournal(e2rp, journal, is_first_term);
      is_first_term = FALSE;
    }

    if ( GetStatus (cufp->Volume) ) {
      GetTitle (cufp->volume_text, volume, sizeof (volume));
      AddVolume(e2rp, volume, is_first_term);
      is_first_term = FALSE;
    }

    if ( GetStatus (cufp->Page) ) {
      GetTitle (cufp->page_text, volume, sizeof (page));
      AddPage(e2rp, page, is_first_term);
      is_first_term = FALSE;
    }

    if ( GetStatus (cufp->Year) ) {
      GetTitle (cufp->year_text, year, sizeof (year));
      AddYear(e2rp, year, is_first_term);
      is_first_term = FALSE;
    }      
  }

  SetValue (cufp->new_query, 2); /* reset the popup list to display customize item*/


  /* process query, get Uids, populate the caipp field*/
  GetUidListFromE2Request (caipp, e2rp, cufp->rdoc);

  /*here, creating report from the Uids obtained in GetUidList() */
  CreateDocSum (caipp->uids_bs, cufp->rdoc, cufp->count);

  return;
}


/*****************************************************************************
*  Function:     Accept 
*
*  Description:	
*****************
*  Argument:  
*
*  Returns:	int, or call a CB to return uid.
*
*            need to change from Update to Accept, 

*****************************************************************************/

static void  Accept (ButtoN b) {

  Int2               curr_item = 0, max;     
  Char max_str[6], pmid_str[15];
  CitationUpdateFormPtr  cufp;
  CitArtInPressPtr caipp; 
  WindoW w;
  PubdescPagePtr ppp;

  Uint4 pmid;

  cufp = (CitationUpdateFormPtr) GetObjectExtra (b);
  if (cufp == NULL) return;
  caipp = cufp->caipp;
  if (caipp == NULL) return;
  ppp = (PubdescPagePtr) cufp->userdata;
  if (ppp == NULL) return;

  GetDocHighlight (cufp->rdoc, &curr_item, NULL);

  GetTitle (cufp->count, max_str, sizeof (max_str));
  max = atoi(max_str);

  /*debug*/
  if (curr_item == 0 || curr_item > max) {
    AppendText (cufp->rdoc, "\n      Please select a valid citation for update!!", NULL, NULL, systemFont);

    InvalDocument (cufp->rdoc);  
    ArrowCursor ();
    Update ();
    return;
  }

  BSSeek (caipp->uids_bs, sizeof(Uint4)*(curr_item-1), SEEK_SET);
  BSRead (caipp->uids_bs, &pmid, sizeof (Uint4));

  Reset (cufp->rdoc);
  Update ();                  

  w = ParentWindow (b);
  Hide (w);
  sprintf(pmid_str, "%d", pmid); 
  /* fill in pmid in text box of original window */
  SetTitle (ppp->pmid, pmid_str);
  ArrowCursor ();
  Update ();
  /* have this button now point to ppp needed for automatic LookupbyPmidProc */
  SetObjectExtra (b, (Pointer) ppp, NULL);
  LookupByPmidProc (b);
  Remove (w);
}


static void MyNotify (DoC d, Int2 item, Int2 row, Int2 col, Boolean dblclick)
{
  Int2  curr = 0, max;
  CharPtr  str;
   
  CitationUpdateFormPtr  cufp;
  CitArtInPressPtr caipp; 

  Uint4 pmid;
  Char max_str[6];


 if (dblclick) {

   cufp = (CitationUpdateFormPtr) GetObjectExtra (d);
   if (cufp == NULL) return;
   caipp = cufp->caipp;
   if (caipp == NULL) return;

   GetDocHighlight (d, &curr, NULL);

   GetTitle (cufp->count, max_str, sizeof (max_str));
   max = atoi(max_str);

   
   /*debug*/
   if (curr == 0 || curr > max) {
     AppendText (cufp->rdoc, "\n      Please select a valid citation!!", NULL, NULL, systemFont);
     
    InvalDocument (cufp->rdoc);  
    ArrowCursor ();
    Update ();
    return;
   }

  BSSeek (caipp->uids_bs, sizeof(Uint4)*(curr-1), SEEK_SET);
  BSRead (caipp->uids_bs, &pmid, sizeof (Uint4));

  if (item > 0 && col > 0) {
    str = GetDocText (d, item, 1, 0);     /* could get last row (pmid)?? */


    /* can launch separate editor here, display Abstract....
    Message (MSG_OK, "New Window, query with pmid: %d", pmid);
    */

    LaunchEntrezURL ("PubMed", pmid, "Abstract");
 
    MemFree (str);
  } else {
    Beep ();
  }
 }
  else {
    GetDocHighlight (d, &curr, NULL);
    SetDocHighlight (d, item, item);
    
    UpdateDocument(d, curr, 0);
    UpdateDocument(d, item, 0);
  }
}


/* call back for new Cancel button*/
static void Quit (ButtoN b)

{
  QuitProgram ();
}



/*****************************************************************************
*  Function:     CreateCitationUpdateWindow_detail
*  Description:	 create the empty GUI, no caipp is passed in to populate the text areas.
*  Argument:     CitArtInPressPtr
*   
*  Returns:	void
*
****************************************************************************/

/*debug  need to reset initial text fields to "" */

static WindoW CreateCitationUpdateWindow_detail (Pointer userdata)
{

  CitationUpdateFormPtr  cufp; 

  /*cufp attached to b (and other action items, to which CB are attached) by SetOjectExtra*/
  
  ButtoN             b;    
  GrouP              e, g, h, i, j, k, l;
  GrouP              cit_fields, authors, journal, imprint, title;
  PopuP              p;
  WindoW             w;

  cufp = (CitationUpdateFormPtr) MemNew (sizeof (CitationUpdateForm));
  if (cufp == NULL) return NULL;
  w = FixedWindow (-50, -33, -10, -10, "In-press citation update", StdCloseWindowProc);
  SetObjectExtra (w, cufp, StdCleanupFormProc);

  cufp->form = (ForM) w;
  cufp->userdata = userdata;

  /* start setup of GUI */
  h = HiddenGroup (w, -1, 0, NULL);   /*verticle layout, top level group*/

  e = HiddenGroup (h, 1, 0, NULL);
  SetGroupSpacing (e, 10, 3);
  StaticPrompt (e, " ", 0, 10, programFont, 'l');


  l = HiddenGroup (h, 2, 0, NULL);
  SetGroupSpacing (l, 20, 3);

  StaticPrompt (l, "Number of potential matches (Max 20):", 0, dialogTextHeight, systemFont, 'l');
  cufp->count = DialogText (l, "", 6, NULL);

  StaticPrompt (l, " ", 0, 10, programFont, 'l');
  StaticPrompt (l, " ", 0, 10, programFont, 'l');


  /********* group together all cit-fields with a normal group.
   * inside the normal group, control display with hidden groups */
  cit_fields = NormalGroup (h, 0, 4, "Availble information on this in-press citation:", systemFont, NULL);

  
  /* add authors*/
  authors = HiddenGroup (cit_fields, 4, 0, NULL);
  SetGroupSpacing (authors, 5, 3);

  cufp->First_Author  = CheckBox (authors, "First Author", NULL);
  cufp->f_auth_text = DialogText (authors,  "", 12, NULL);

  cufp->Last_Author = CheckBox (authors, "Last Author", NULL);
  cufp->l_auth_text = DialogText (authors,  "", 12, NULL);

  journal = HiddenGroup (cit_fields, 2, 0, NULL);
  SetGroupSpacing (journal, 5, 3);

  cufp->Journal = CheckBox (journal, "Journal", NULL);
  cufp->journal_text = DialogText (journal,  "", 30, NULL);

  imprint = HiddenGroup (cit_fields, 6, 0, NULL);
  SetGroupSpacing (imprint, 5, 3);

  cufp->Year = CheckBox (imprint, "Year", NULL);
  cufp->year_text = DialogText (imprint, "", 6, NULL);
  cufp->Volume = CheckBox (imprint, "Volume", NULL);
  cufp->volume_text  = DialogText (imprint, "", 6, NULL);
  cufp->Page = CheckBox (imprint, "Page", NULL);
  cufp->page_text = DialogText (imprint, "", 6, NULL);


  title = HiddenGroup (cit_fields, 6, 0, NULL);
  SetGroupSpacing (title, 5, 3);

  StaticPrompt (title, "Article Title:", 0, dialogTextHeight, systemFont, 'l');
  cufp->art_title_text = ScrollText (title, 30, 1, systemFont, TRUE, NULL);
  SafeSetTitle (cufp->art_title_text, "");

  i = HiddenGroup (h, 2, 0, NULL);     /*3rd vertical item in h*/
  SetGroupSpacing (i, 10, 3);


  StaticPrompt (i, " ", 0, popupMenuHeight, programFont, 'l');
  StaticPrompt (i, " ", 0, popupMenuHeight, programFont, 'l');

  cufp->Extra_Term = CheckBox (i, "Add these terms to new query", NULL);
  cufp->extra_term_text = DialogText (i, "", 25, NULL);


  j = HiddenGroup (h, 3, 0, NULL);
  SetGroupSpacing (j, 5, 3);

  StaticPrompt (j, " ", 0, 10, programFont, 'l');
  StaticPrompt (j, " ", 0, 10, programFont, 'l');
  StaticPrompt (j, " ", 0, 10, programFont, 'l');


  StaticPrompt (j, "New query option:", 0, popupMenuHeight, systemFont, 'l');

  p = PopupList (j, TRUE, NULL);

  PopupItem (p, "default: use all fields");
  PopupItem (p, "customize:  use checked fields");

  SetValue (p, 1);   /* after 1st default query, set value to 2*/
  cufp->new_query = p;

  b = DefaultButton (j, "Send Modified Query", SendQuery);   
  SetObjectExtra (b, cufp, NULL);

  cufp->action = b;

  StaticPrompt (j, " ", 0, 10, programFont, 'l');
  StaticPrompt (j, " ", 0, 10, programFont, 'l');
  StaticPrompt (j, " ", 0, 10, programFont, 'l');

  g = HiddenGroup (h, 1, 0, NULL);
  SetGroupSpacing (g, 10, 3);

  StaticPrompt (g, "Summary of potential matches, highlight one to use in update", 0, dialogTextHeight, systemFont, 'l');

  cufp->rdoc = DocumentPanel (g, 650, 300);        /*here, pixel width and height*/
  SetObjectExtra (cufp->rdoc, cufp, NULL);

  /*debug, hard-code pixWidth, need to specify with Window left/right coord. in 
   *the case of resizable window*/
  txtColFmt.pixWidth = 650;
  txtColFmt.pixInset = 8;



  /* k, group of buttons for user control ,   last row in h*/
  k = HiddenGroup (h, 2, 0, NULL);
  SetGroupSpacing (k, 20, 3);

  /*spacing*/
  StaticPrompt (k, " ", 0, popupMenuHeight, programFont, 'l');
  StaticPrompt (k, " ", 0, popupMenuHeight, programFont, 'l'); 

  b = PushButton (k, " Accept ", Accept);   
  SetObjectExtra (b, cufp, NULL);

  b = PushButton (k, "  Cancel ", StdCancelButtonProc);
  SetObjectExtra (b, cufp, NULL);

  StaticPrompt (k, " ", 0, 10, programFont, 'l');
  StaticPrompt (k, " ", 0, 10, programFont, 'l');

  AlignObjects (ALIGN_CENTER, (Nlm_HANDLE) g, (Nlm_HANDLE) j, (Nlm_HANDLE) cit_fields, (Nlm_HANDLE) k, (Nlm_HANDLE) cufp->rdoc,  NULL);


  RealizeWindow (w);
  Select (cufp->extra_term_text);       /*set default active field*/

  /*SetWindowTimer (w, HandleQuery);     what for?? timeout on query??  */

  return w;
}

/*****************************************************************************
*  Function:     PopulateWindow
*  Description:	 use pdp to populate a caipp structure. No default query!!
*                calls UpdateWindow to populate the text areas 
*  Argument:     WindoW
*   
*  Returns:	void
**************************************************************************
*
*   do not   call SendQuery()  !!!
*    
*
****************************************************************************/
static void PopulateWindow( WindoW w, PubdescPtr pdp)
{
  CitationUpdateFormPtr  cufp;

  CitArtInPressPtr caipp;       /* keep populate this for use in SendQuery*/
  PubdescPtr pubdesc;		/* pubdesc node in seqdesc */
  ValNodePtr pub;		/* pub-equiv chain in pubdesc */
  CitArtPtr cap;		/* article pulled from pub-equiv */
  CitJourPtr cjp;
  ImprintPtr ImpPtr;
  AuthListPtr alp;  
  AuthorPtr f_author_p, l_author_p;  /* first and last authors*/
  PersonIdPtr pid_p;
  NameStdPtr nsp;

  ValNodePtr title_vnp, auth_vnp, art_title_vnp;

  /*could get rid of full names*/
  CharPtr f_first_name, f_last_name, l_first_name, l_last_name;
  CharPtr jour_title, jour_volume, jour_page, art_title;
  
  Int2 year;
  Int1 rank;

  Boolean has_author, has_title, has_volume, has_page, has_year, is_article;
  has_author =  has_title = has_volume = has_page = has_year = is_article = FALSE;

  cufp = (CitationUpdateFormPtr) GetObjectExtra (w);
  if (cufp == NULL) return;


  if ( (caipp = MemNew(sizeof(CitArtInPress))) == NULL) {
    return;
  }

  pubdesc = pdp;      /* don't really need this could use pubdesc*/   
  if(pubdesc == NULL) return;

  /*loop through set of pub-equiv Value Nodes, find cit-art */
  if (pubdesc->pub == NULL) return;

  for (pub=pubdesc->pub, cap=NULL; pub; pub=pub->next) {
    if (pub->choice == PUB_Article) {

      is_article = TRUE;
      cap = (CitArtPtr) pub->data.ptrvalue;
      if (cap == NULL) return; 
      
      /* look for cit-art from journals only*/
      if (cap->from ==1) {
	
	cjp = (CitJourPtr) cap->fromptr;
	if (cjp == NULL) return; 
	
	ImpPtr =(ImprintPtr) cjp->imp;	      
	if(ImpPtr == NULL) return; 
	      

	  /* get article title */
	  if( !cap->title ) { art_title = StringSave("No title was available"); }
	  for (art_title_vnp=cap->title; art_title_vnp; art_title_vnp=art_title_vnp->next) {
	    /*could combine the four*/
	    if((art_title_vnp->choice == Cit_title_name) || (art_title_vnp->choice == Cit_title_tsub)|| (art_title_vnp->choice == Cit_title_trans)) {
	      art_title = art_title_vnp->data.ptrvalue;		    
	    }
	  }	 

	  /* get journal title */
	  if( !cjp->title ) { jour_title = StringSave("null"); }
	  for (title_vnp=cjp->title; title_vnp; title_vnp=title_vnp->next) {
	    /*could combine the four*/
	    if (title_vnp->choice == Cit_title_iso_jta) {
	      jour_title = title_vnp->data.ptrvalue;		    
	    }
	    else if (title_vnp->choice == Cit_title_ml_jta) {
	      jour_title = title_vnp->data.ptrvalue;
	    }
	    else if (title_vnp->choice == Cit_title_jta) {
	      jour_title = title_vnp->data.ptrvalue;
	    }
	    else if (title_vnp->choice == Cit_title_name) {
	      jour_title = title_vnp->data.ptrvalue;
	      /* break;            don't need break ??? */
	    }
	    else { 
	      jour_title = StringSave("this journal got a WEIRD title");
	    }
	    has_title = TRUE;  
	  }
	  
	  
	  if (ImpPtr->volume) {
	    jour_volume =  ImpPtr->volume;
	    has_volume = TRUE;  
	  }
	  else {  jour_volume = StringSave("null"); }	 
	  
	  if (ImpPtr->pages) { 
	    jour_page =  ImpPtr->pages;
	    has_page = TRUE;  
	  }
	  else {  jour_page = StringSave("null"); }
	
	  
	  /* not ideal behavior, although date is required in imp*/
	  if ((DatePtr)ImpPtr->date == NULL) { return; }
	  
		year = -1;
		DateRead (ImpPtr->date, &year, NULL, NULL, NULL);
		if (year != -1) { has_year = TRUE; }				
		
		alp = (AuthListPtr) cap->authors; 
		if (alp == NULL) {
		  
		  if (caipp->logfile) {                   
 
		    fprintf(caipp->logfile,"|this in-press cit-art has no author| %s | %s| %s | %d\n", jour_title, jour_volume, jour_page, year);
		  }
		   return;
		}		
		 		 			
		/*get ptr to both 1st and last author*/	 
		auth_vnp = (ValNodePtr) alp->names; /*get the first node */ 
		
		f_author_p = (AuthorPtr)auth_vnp->data.ptrvalue;
		
		/* get the node for the last author*/
		while (auth_vnp->next) {
		  auth_vnp = auth_vnp->next;
		}
		l_author_p =(AuthorPtr)auth_vnp->data.ptrvalue;
		
		/* should be analyzing the value of alp->choice,  not alp->names->choice*/
		if (alp->choice == 1) {   
		  /*could make this (getting name from author_p) a function */		   
		   pid_p = (PersonIdPtr) f_author_p->name;
		   
		   if (pid_p->choice ==2) { 			 
		     
		     nsp = (NameStdPtr) pid_p -> data;  
		     
		     /* why didnot this work?? * 			 
			if (!( nsp->names[0])) */		    
		     
		     if (nsp->names[0] != NULL ) {
		       
		       f_last_name =  nsp->names[0];	
		       has_author = TRUE;		       
		       
		       if (nsp->names[1]!= NULL) {
			 f_first_name =  nsp->names[1];	
		       }
		       else { f_first_name = StringSave("null"); }		       	
		       
		     }
		     /* set to null!! */
		     else {  f_last_name = StringSave("null"); 
		     f_first_name = StringSave("null");			     
		     }
		   }
		   
		   
		   /*now do the last author */
		   
		   if (l_author_p->name == f_author_p->name) {  /*single author*/
		     l_first_name = f_first_name;
		     l_last_name =f_last_name;
		   }
		   else {
		     
		     pid_p = (PersonIdPtr) l_author_p->name;
		     if (pid_p->choice ==2) { 			 
		       nsp = (NameStdPtr) pid_p -> data;  
		       
		       if (nsp->names[0] != NULL ) {
			 
			 l_last_name =  nsp->names[0];	
			 if (nsp->names[1]!= NULL) {
			   l_first_name =  nsp->names[1];	
			 }
			 else { l_first_name = StringSave("null"); }
		       }		      
		       else {  l_last_name = StringSave("null"); 
		       l_first_name = StringSave("null");	
		       }
		     }
		   } /*end of last author */
		 }  /* end alp-> choice is std */
		 
		 
		 /* here, if author list is not std names */ 			      
		 else if (alp->choice == 2) {  
		   
		   /* full name as last name*/	       
		   f_last_name = (CharPtr)f_author_p;
		   f_first_name = StringSave ("null");
		   has_author = TRUE;	

		   l_last_name = (CharPtr)l_author_p;
		   l_first_name = StringSave ("null");
		 }
		 else {  
		   /* full name as last name*/	       
		   f_last_name = (CharPtr)f_author_p;
		   f_first_name = StringSave ("null");
		   has_author = TRUE;	

		   l_last_name = (CharPtr)l_author_p;
		   l_first_name = StringSave ("null");
		 }	      		     		   
		


		rank = 0;
		 /*caculate rank*/
	
		if (has_page && has_volume && has_title) {
		   rank = 10;
		}
		else if (has_author) {
		  if (has_title) {
		    if(has_year && has_volume) { rank = 9; }
		    else if (has_year || has_volume) { rank = 8; }
		     else { rank = 7; }                /* auth + journal_title only*/
		  }
		  else if (has_year) { rank = 6; }   /* auth + year only*/
		  else { rank = 5; } /* author  but no journal_title case */
		}		 
		else { rank = 0; }  /*ignore all other combo*/	     


		rank = 0;
		 /*caculate rank*/
		/*   better set flags as key individual fields are collected!! 
		 *    avoid many calls to StringCmp */
		/* must have first author, or (journal_title, volume and page)*/
		
		if (has_page && has_volume && has_title) {
		   rank = 10;
		}
		else if (has_author) {
		  if (has_title) {
		    if(has_year && has_volume) { rank = 9; }
		    else if (has_year || has_volume) { rank = 8; }
		     else { rank = 7; }                /* auth + journal_title only*/
		  }
		  else if (has_year) { rank = 6; }   /* auth + year only*/
		  else { rank = 5; } /* author  but no journal_title case */
		}		 
		else { rank = 0; }  /*ignore all other combo*/	     


		  /*  safe practice to save a new copy if the struct my go away. */     
		  caipp->jour_title =  StringSave(jour_title);
		  caipp->f_last_name = StringSave(f_last_name);
		  caipp->f_first_name =StringSave(f_first_name);
		  caipp->l_last_name = StringSave(l_last_name);
		  caipp->l_first_name = StringSave(l_first_name);
		  caipp->jour_volume = StringSave(jour_volume);
		  caipp->jour_page = StringSave(jour_page);
		  caipp->year = year;
		  caipp->art_title = StringSave(art_title);
		  caipp->rank = rank;
			
	/*}       end if prepub*/  	
      }        /*end if cit-art*/      
    }          /*end if Pub_article*/    
  }  /* end for, looped through all valnodes of pub*/

  if (!is_article) {
    AppendText (cufp->rdoc, "\n ONLY article citation are looked up here, please click Cancel to get back!!", NULL, NULL, systemFont);
    
    InvalDocument (cufp->rdoc);  
    ArrowCursor ();
    Update ();

    MemFree(caipp);
    return;
  }
  
  cufp->caipp = caipp;

  UpdateWindow(cufp, caipp);

  /*debug:  don't need the default query? */

  /* SendQuery(cufp->action); */
}


/*****************************************************************************
*  Function:     UpdateWindow
*  Description:	 use current node of caipp to populate the text areas. reset caipp
*  Argument:     CitArtInPressPtr, WindoW
*   
*  Returns:	void
**************************************************************************
*
*     No call to SendQuery()
*
****************************************************************************/
static void UpdateWindow( CitationUpdateFormPtr cufp, CitArtInPressPtr caipp)
{

  CharPtr             year;    /* convert int year to CharPtr */

  if (cufp == NULL) return;

  if (caipp == NULL) return;

  year = MemNew(5);
  sprintf(year, "%d", caipp->year); 

  SafeSetTitle (cufp->f_auth_text,caipp->f_last_name);
  SafeSetTitle (cufp->l_auth_text,caipp->l_last_name);
  SafeSetTitle (cufp->journal_text,caipp->jour_title);
  SafeSetTitle (cufp->year_text, year);
  SafeSetTitle (cufp->volume_text,caipp->jour_volume);
  SafeSetTitle (cufp->page_text,caipp->jour_page);

  SafeSetTitle (cufp->art_title_text, caipp->art_title);
  SafeSetTitle (cufp->extra_term_text, "");

  SetValue (cufp->new_query, 1); /* reset the popup to default query after an update*/
}


/*****************************************************************************
*  Function:     LaunchRelaxedQuery 
*  Description:	 call back function for visitPubDesc  (or, CB for Ssequin button.
*  Argument:     userdata could be a FormPtr of some sort.
*   
*  Returns:	 void
*
****************************************************************************/
static void LaunchRelaxedQuery (PubdescPtr pdp, Pointer userdata)
{

  /*debug, will need to get userdata, pass somefield into PopulateWindow to hold uid.*/

  WindoW w;

  w = CreateCitationUpdateWindow_detail(userdata);	
  
  if (w == NULL) return;
  PopulateWindow(w, pdp);
  Show (w);
  Select (w);
}

