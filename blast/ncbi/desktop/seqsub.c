/*   seqsub.c
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
* File Name:  seqsub.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   1/22/95
*
* $Revision: 6.11 $
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

#include <seqsub.h>
#include <gather.h>

static ENUM_ALIST(name_suffix_alist)
  {" ",    0},
  {"Jr.",  1},
  {"Sr.",  2},
  {"II",   3},
  {"III",  4},
  {"IV",   5},
  {"V",    6},
  {"VI",   7},
END_ENUM_ALIST

typedef struct contactpage {
  DIALOG_MESSAGE_BLOCK
  TexT            firstname;
  TexT            middleinit;
  TexT            lastname;
  PopuP           suffix;
  DialoG          affil;
  GrouP           contactGrp [3];
} ContactPage, PNTR ContactPagePtr;

extern CharPtr NameStdPtrToAuthorSpreadsheetString (NameStdPtr nsp);

static void ContactInfoPtrToContactPage (DialoG d, Pointer data)

{
  AuthorPtr       ap;
  ContactInfoPtr  cip;
  ContactPagePtr  cpp;
  NameStdPtr      nsp;
  PersonIdPtr     pid;
  CharPtr         str;
  CharPtr         txt;

  cpp = (ContactPagePtr) GetObjectExtra (d);
  cip = (ContactInfoPtr) data;
  if (cpp != NULL) {
    SafeSetTitle (cpp->firstname, "");
    SafeSetTitle (cpp->lastname, "");
    PointerToDialog (cpp->affil, NULL);
    if (cip != NULL) {
      ap = cip->contact;
      if (ap != NULL) {
        pid = ap->name;
        if (pid != NULL && pid->choice == 2) {
          nsp = pid->data;
          if (nsp != NULL) {
            str = NameStdPtrToAuthorSpreadsheetString (nsp);
            if (str != NULL) {
              txt = ExtractTagListColumn (str, 0);
              SafeSetTitle (cpp->firstname, txt);
              MemFree (txt);
              txt = ExtractTagListColumn (str, 1);
              SafeSetTitle (cpp->middleinit, txt);
              MemFree (txt);
              txt = ExtractTagListColumn (str, 2);
              SafeSetTitle (cpp->lastname, txt);
              MemFree (txt);
              txt = ExtractTagListColumn (str, 3);
	      SafeSetValue (cpp->suffix, atoi(txt)+1);
              MemFree (str);
            }
          }
        }
        PointerToDialog (cpp->affil, ap->affil);
      }
    }
  }
}

extern NameStdPtr AuthorSpreadsheetStringToNameStdPtr (CharPtr txt);

static Pointer ContactPageToContactInfoPtr (DialoG d)

{
  AuthorPtr       ap;
  ContactInfoPtr  cip;
  ContactPagePtr  cpp;
  NameStdPtr      nsp;
  PersonIdPtr     pid;
  Char            str [128];
  CharPtr         txt;
  Char            suffix [32];
  Uint2           suffixVal;


  cip = NULL;
  cpp = (ContactPagePtr) GetObjectExtra (d);
  if (cpp != NULL) {
    cip = ContactInfoNew ();
    if (cip != NULL) {
      nsp = NULL;
      ap = AuthorNew ();
      cip->contact = ap;
      if (ap != NULL) {
        pid = PersonIdNew ();
        ap->name = pid;
        if (pid != NULL) {
          pid->choice = 2;
          str [0] = '\0';
          txt = SaveStringFromText (cpp->firstname);
          StringCat (str, txt);
          StringCat (str, "\t");
          MemFree (txt);
          txt = SaveStringFromText (cpp->middleinit);
          StringCat (str, txt);
          StringCat (str, "\t");
          MemFree (txt);
          txt = SaveStringFromText (cpp->lastname);
          StringCat (str, txt);
          StringCat (str, "\t");
          MemFree (txt);
          suffixVal = GetValue (cpp->suffix);
	  sprintf (suffix, "%d", (int) (suffixVal - 1));
          StringCat (str, suffix);
          StringCat (str, "\n");
          txt = StringSave (str);
          nsp = AuthorSpreadsheetStringToNameStdPtr (txt);
          MemFree (txt);
          pid->data = nsp;
        }
        ap->affil = DialogToPointer (cpp->affil);
        if (nsp == NULL || nsp->names [0] == NULL || ap->affil == NULL) {
          cip = ContactInfoFree (cip);
        }
      }
    }
  }
  return (Pointer) cip;
}

static Boolean ReadContactDialog (DialoG d, CharPtr filename)

{
  AsnIoPtr        aip;
  ContactInfoPtr  cip;
  ContactPagePtr  cpp;
  Char            path [PATH_MAX];

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  cpp = (ContactPagePtr) GetObjectExtra (d);
  if (cpp != NULL) {
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
      aip = AsnIoOpen (path, "r");
      if (aip != NULL) {
        cip = ContactInfoAsnRead (aip, NULL);
        AsnIoClose (aip);
        if (cip != NULL) {
          ContactInfoPtrToContactPage (cpp->dialog, cip);
          cip = ContactInfoFree (cip);
          Update ();
          Select (cpp->firstname);
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static Boolean WriteContactDialog (DialoG d, CharPtr filename)

{
  AsnIoPtr        aip;
  ContactInfoPtr  cip;
  ContactPagePtr  cpp;
  Char            path [PATH_MAX];
#ifdef WIN_MAC
  FILE            *f;
#endif

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  cpp = (ContactPagePtr) GetObjectExtra (d);
  if (cpp != NULL) {
    if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), NULL)) {
#ifdef WIN_MAC
      f = FileOpen (path, "r");
      if (f != NULL) {
        FileClose (f);
      } else {
        FileCreate (path, "TEXT", "ttxt");
      }
#endif
      aip = AsnIoOpen (path, "w");
      if (aip != NULL) {
        cip = ContactPageToContactInfoPtr (cpp->dialog);
        ContactInfoAsnWrite (cip, aip, NULL);
        AsnIoClose (aip);
        cip = ContactInfoFree (cip);
        return TRUE;
      }
    }
  }
  return FALSE;
}

static ValNodePtr TestContactDialog (DialoG d)

{
  ContactPagePtr  cpp;
  ValNodePtr      head;

  head = NULL;
  cpp = (ContactPagePtr) GetObjectExtra (d);
  if (cpp != NULL) {
    if (TextHasNoText (cpp->firstname)) {
      head = AddStringToValNodeChain (head, "You must specify a first name", 0);
    }
    if (TextHasNoText (cpp->lastname)) {
      head = AddStringToValNodeChain (head, "You must specify a last name", 0);
    }
    /*
    if (TextHasNoText (cpp->phone)) {
      head = AddStringToValNodeChain (head, "You must specify a phone number", 0);
    }
    if (TextHasNoText (cpp->email)) {
      head = AddStringToValNodeChain (head, "You must specify an e-mail address", 0);
    }
    */
  }
  return head;
}

static CharPtr contactTabs [] = {
  "Name", "Affiliation", "Contact", NULL
};

static void ChangeContactSubPage (VoidPtr data, Int2 newval, Int2 oldval)

{
  ContactPagePtr  cpp;

  cpp = (ContactPagePtr) data;
  if (cpp != NULL) {
    if (oldval >= 0 && oldval <= 2) {
      SafeHide (cpp->contactGrp [oldval]);
    }
    if (newval >= 0 && newval <= 2) {
      SafeShow (cpp->contactGrp [newval]);
    }
    Update ();
  }
}

extern DialoG CreateContactDialog (GrouP h, CharPtr title)

{
  ContactPagePtr  cpp;
  GrouP           g;
  GrouP           k;
  GrouP           m;
  GrouP           p;
  GrouP           s;
  DialoG          tbs;

  p = HiddenGroup (h, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  cpp = (ContactPagePtr) MemNew (sizeof (ContactPage));
  if (cpp != NULL) {

    SetObjectExtra (p, cpp, StdCleanupExtraProc);
    cpp->dialog = (DialoG) p;
    cpp->todialog = ContactInfoPtrToContactPage;
    cpp->fromdialog = ContactPageToContactInfoPtr;
    cpp->testdialog = TestContactDialog;
    cpp->importdialog = ReadContactDialog;
    cpp->exportdialog = WriteContactDialog;

    if (title != NULL && title [0] != '\0') {
      s = NormalGroup (p, 0, -2, title, systemFont, NULL);
    } else {
      s = HiddenGroup (p, 0, -2, NULL);
    }
    m = HiddenGroup (s, -1, 0, NULL);
    SetGroupSpacing (m, 10, 10);

    tbs = CreateFolderTabs (m, contactTabs, 0, 0, 0,
                            PROGRAM_FOLDER_TAB,
                            ChangeContactSubPage, (Pointer) cpp);
    k = HiddenGroup (m, 0, 0, NULL);

    cpp->contactGrp [0] = HiddenGroup (k, -1, 0, NULL);
    g = HiddenGroup (cpp->contactGrp [0], 4, 0, NULL);
    SetGroupSpacing (g, -1, 2);
    StaticPrompt (g, "First Name", 0, 0, programFont, 'c');
    StaticPrompt (g, "M.I.", 0, 0, programFont, 'c');
    StaticPrompt (g, "Last Name", 0, 0, programFont, 'c');
    StaticPrompt (g, "Sfx", 0, 0, programFont, 'c');
    cpp->firstname = DialogText (g, "", 8, NULL);
    cpp->middleinit = DialogText (g, "", 4, NULL);
    cpp->lastname = DialogText (g, "", 9, NULL);
    cpp->suffix = PopupList (g, TRUE, NULL);
    SetObjectExtra (cpp->suffix, cpp, NULL);
    InitEnumPopup (cpp->suffix, name_suffix_alist, NULL);
    SetEnumPopup (cpp->suffix, name_suffix_alist, 0);

    cpp->affil = CreateExtAffilDialog (k, NULL,
                                       &(cpp->contactGrp [1]),
                                       &(cpp->contactGrp [2]));

    AlignObjects (ALIGN_CENTER, (HANDLE) tbs, (HANDLE) g, (HANDLE) cpp->affil, NULL);
  }

  return (DialoG) p;
}

typedef struct citsubpage {
  DIALOG_MESSAGE_BLOCK
  DialoG          authors;
  TexT            consortium;
  DialoG          date;
  DialoG          affil;
  TexT            descr;
  GrouP           citsubGrp [5];
} CitsubPage, PNTR CitsubPagePtr;

static AuthListPtr AddConsortiumToAuthList (AuthListPtr alp, TexT consortium)

{
  AuthorPtr    ap;
  ValNodePtr   names;
  PersonIdPtr  pid;

  if (TextHasNoText (consortium)) return alp;
  if (alp == NULL) {
    alp = AuthListNew ();
    alp->choice = 1;
  }
  pid = PersonIdNew ();
  if (pid == NULL) return NULL;
  pid->choice = 5;
  pid->data = SaveStringFromText (consortium);
  ap = AuthorNew ();
  if (ap == NULL) return NULL;
  ap->name = pid;
  names = ValNodeAdd (&(alp->names));
  names->choice = 1;
  names->data.ptrvalue = ap;
  return alp;
}

static void AuthListToConsortium (AuthListPtr alp, TexT consortium)

{
  AuthorPtr    ap;
  ValNodePtr   names;
  PersonIdPtr  pid;
  CharPtr      str;

  if (alp == NULL || consortium == NULL) return;
  if (alp->choice != 1) return;
  for (names = alp->names; names != NULL; names = names->next) {
    ap = names->data.ptrvalue;
    if (ap == NULL) continue;
    pid = ap->name;
    if (pid == NULL || pid->choice != 5) continue;
    str = (CharPtr) pid->data;
    SafeSetTitle (consortium, str);
  }
}

static void CitSubPtrToCitsubPage (DialoG d, Pointer data)

{
  AuthListPtr    alp;
  CitsubPagePtr  cpp;
  CitSubPtr      csp;
  WindoW         tempPort;

  cpp = (CitsubPagePtr) GetObjectExtra (d);
  csp = (CitSubPtr) data;
  if (cpp != NULL) {
    PointerToDialog (cpp->authors, NULL);
    PointerToDialog (cpp->date, NULL);
    PointerToDialog (cpp->affil, NULL);
    SafeSetTitle (cpp->descr, "");
    tempPort = SavePort (cpp->authors);
    if (csp != NULL) {
      alp = csp->authors;
      PointerToDialog (cpp->authors, (Pointer) alp);
      AuthListToConsortium (alp, cpp->consortium);
      if (alp != NULL) {
        PointerToDialog (cpp->affil, alp->affil);
      }
      if (csp->date != NULL) {
        PointerToDialog (cpp->date, csp->date);
      } else if (csp->imp != NULL) {
        PointerToDialog (cpp->date, csp->imp->date);
      }
      SafeSetTitle (cpp->descr, csp->descr);
    }
    RestorePort (tempPort);
  }
}

static Pointer CitsubPageToCitSubPtr (DialoG d)

{
  AuthListPtr    alp;
  CitsubPagePtr  cpp;
  CitSubPtr      csp;

  csp = NULL;
  cpp = (CitsubPagePtr) GetObjectExtra (d);
  if (cpp != NULL) {
    csp = CitSubNew ();
    if (csp != NULL) {
      alp = (AuthListPtr) DialogToPointer (cpp->authors);
      alp = AddConsortiumToAuthList (alp, cpp->consortium);
      if (alp != NULL) {
        alp->affil = DialogToPointer (cpp->affil);
      }
      csp->authors = alp;
      csp->date = DialogToPointer (cpp->date);
      csp->descr = SaveStringFromTextAndStripNewlines (cpp->descr);
      if (alp == NULL) {
        csp = CitSubFree (csp);
      }
    }
  }
  return (Pointer) csp;
}

static Boolean ReadCitsubDialog (DialoG d, CharPtr filename)

{
  AsnIoPtr       aip;
  CitsubPagePtr  cpp;
  CitSubPtr      csp;
  Char           path [PATH_MAX];

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  cpp = (CitsubPagePtr) GetObjectExtra (d);
  if (cpp != NULL) {
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
      aip = AsnIoOpen (path, "r");
      if (aip != NULL) {
        csp = CitSubAsnRead (aip, NULL);
        AsnIoClose (aip);
        if (csp != NULL) {
          CitSubPtrToCitsubPage (cpp->dialog, csp);
          csp = CitSubFree (csp);
          Update ();
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static Boolean WriteCitsubDialog (DialoG d, CharPtr filename)

{
  AsnIoPtr       aip;
  CitsubPagePtr  cpp;
  CitSubPtr      csp;
  Char           path [PATH_MAX];
#ifdef WIN_MAC
  FILE            *f;
#endif

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  cpp = (CitsubPagePtr) GetObjectExtra (d);
  if (cpp != NULL) {
    if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), NULL)) {
#ifdef WIN_MAC
      f = FileOpen (path, "r");
      if (f != NULL) {
        FileClose (f);
      } else {
        FileCreate (path, "TEXT", "ttxt");
      }
#endif
      aip = AsnIoOpen (path, "w");
      if (aip != NULL) {
        csp = CitsubPageToCitSubPtr (cpp->dialog);
        CitSubAsnWrite (csp, aip, NULL);
        AsnIoClose (aip);
        csp = CitSubFree (csp);
        return TRUE;
      }
    }
  }
  return FALSE;
}

static ValNodePtr TestCitsubDialog (DialoG d)

{
  CitsubPagePtr  cpp;
  ValNodePtr     head;
  /*
  CharPtr        ptr;
  */

  head = NULL;
  cpp = (CitsubPagePtr) GetObjectExtra (d);
  if (cpp != NULL) {
    /*
    ptr = GetDocText (cpp->authors.doc, 0, 0, 0);
    if (StringHasNoText (ptr)) {
      head = AddStringToValNodeChain (head, "You must specify an author for the citation", 0);
    }
    MemFree (ptr);
    */
  }
  return head;
}

static CharPtr citsubTabs [] = {
  "Names", "Affiliation", "Contact", "Description", NULL, NULL
};

static void ChangeCitsubSubPage (VoidPtr data, Int2 newval, Int2 oldval)

{
  CitsubPagePtr  cpp;

  cpp = (CitsubPagePtr) data;
  if (cpp != NULL) {
    if (oldval >= 0 && oldval <= 4) {
      SafeHide (cpp->citsubGrp [oldval]);
    }
    if (newval >= 0 && newval <= 4) {
      SafeShow (cpp->citsubGrp [newval]);
    }
    Update ();
  }
}

extern DialoG CreateCitSubDialog (GrouP h, CharPtr title, CitSubPtr csp)

{
  CitsubPagePtr  cpp;
  GrouP          k;
  GrouP          m;
  GrouP          p;
  GrouP          q;
  GrouP          s;
  DialoG         tbs;

  p = HiddenGroup (h, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  cpp = (CitsubPagePtr) MemNew (sizeof (CitsubPage));
  if (cpp != NULL) {

    SetObjectExtra (p, cpp, StdCleanupExtraProc);
    cpp->dialog = (DialoG) p;
    cpp->todialog = CitSubPtrToCitsubPage;
    cpp->fromdialog = CitsubPageToCitSubPtr;
    cpp->testdialog = TestCitsubDialog;
    cpp->importdialog = ReadCitsubDialog;
    cpp->exportdialog = WriteCitsubDialog;

    if (title != NULL && title [0] != '\0') {
      s = NormalGroup (p, 0, -2, title, systemFont, NULL);
    } else {
      s = HiddenGroup (p, 0, -2, NULL);
    }
    m = HiddenGroup (s, -1, 0, NULL);
    SetGroupSpacing (m, 10, 10);

    citsubTabs [4] = NULL;
    if (csp != NULL) {
      if (csp->date != NULL) {
        citsubTabs [4] = "Date";
      } else if (csp->imp != NULL && csp->imp->date != NULL) {
        citsubTabs [4] = "Date";
      } else if (GetAppProperty ("InternalNcbiSequin") != NULL) {
        citsubTabs [4] = "Date";
      }
    }
    tbs = CreateFolderTabs (m, citsubTabs, 0, 0, 0,
                            PROGRAM_FOLDER_TAB,
                            ChangeCitsubSubPage, (Pointer) cpp);
    citsubTabs [4] = NULL;
    k = HiddenGroup (m, 0, 0, NULL);

    cpp->citsubGrp [0] = HiddenGroup (k, -1, 0, NULL);
    cpp->authors = CreateAuthorDialog (cpp->citsubGrp [0], 3, -1);
    q = HiddenGroup (cpp->citsubGrp [0], 2, 0, NULL);
    StaticPrompt (q, "Consortium", 0, stdLineHeight, programFont, 'l');
    cpp->consortium = DialogText (q, "", 16, NULL);

    cpp->affil = CreateExtAffilDialog (k, NULL,
                                       &(cpp->citsubGrp [1]),
                                       &(cpp->citsubGrp [2]));

    cpp->citsubGrp [3] = HiddenGroup (k, 0, 2, NULL);
    StaticPrompt (cpp->citsubGrp [3], "Description", 0, 0, programFont, 'c');
    cpp->descr = ScrollText (cpp->citsubGrp [3], 30, 4, programFont, TRUE, NULL);
    Hide (cpp->citsubGrp [3]);

    cpp->citsubGrp [4] = HiddenGroup (k, -1, 0, NULL);
    cpp->date = CreateDateDialog (cpp->citsubGrp [4], NULL);
    Hide (cpp->citsubGrp [4]);

    AlignObjects (ALIGN_CENTER, (HANDLE) tbs, (HANDLE) cpp->authors,
                  (HANDLE) q, (HANDLE) cpp->citsubGrp [3],
                  (HANDLE) cpp->date, (HANDLE) cpp->affil, NULL);
  }

  return (DialoG) p;
}

typedef struct submitpage {
  DIALOG_MESSAGE_BLOCK
  GrouP           subtype;
  GrouP           releaseGrp;
  GrouP           hup;
  GrouP           releaseDate;
  DialoG          date;
  TexT            userID;
  TexT            comment;
  Char            tool [64];
} SubmitPage, PNTR SubmitPagePtr;

static void ChangeSubtype (GrouP g)

{
  Boolean        update;
  SubmitPagePtr  spp;

  spp = (SubmitPagePtr) GetObjectExtra (g);
  if (spp != NULL && spp->subtype != NULL) {
    update = (Boolean) (GetValue (spp->subtype) == 2);
    if (update) {
      SafeHide (spp->releaseGrp);
    } else {
      SafeShow (spp->releaseGrp);
    }
  }
}

static void ChangeHup (GrouP g)

{
  Boolean        hup;
  SubmitPagePtr  spp;

  spp = (SubmitPagePtr) GetObjectExtra (g);
  if (spp != NULL) {
    hup = (Boolean) (GetValue (spp->hup) == 2);
    if (hup) {
      SafeShow (spp->releaseDate);
    } else {
      SafeHide (spp->releaseDate);
    }
  }
}

static void SubmitBlockPtrToSubmitPage (DialoG d, Pointer data)

{
  Char            ch;
  Int2            i;
  CharPtr         ptr;
  SubmitBlockPtr  sbp;
  SubmitPagePtr   spp;
  CharPtr         str;
  WindoW          tempPort;

  spp = (SubmitPagePtr) GetObjectExtra (d);
  sbp = (SubmitBlockPtr) data;
  if (spp != NULL) {
    tempPort = SavePort (spp->comment);
    SafeSetValue (spp->hup, 2);
    SafeSetValue (spp->subtype, 1);
    /*
    SafeSetTitle (spp->userID, "");
    */
    SafeSetTitle (spp->comment, "");
    PointerToDialog (spp->date, NULL);
    if (sbp != NULL) {
      if (sbp->hup) {
        SafeSetValue (spp->hup, 2);
      } else {
        SafeSetValue (spp->hup, 1);
      }
      i = sbp->subtype;
      if (i > 3 || i < 0) {
        i = 4;
      }
      SafeSetValue (spp->subtype, i);
      /*
      SafeSetTitle (spp->userID, sbp->user_tag);
      */
      str = StringSave (sbp->comment);
      ptr = str;
      if (ptr != NULL) {
        ch = *ptr;
        while (ch != '\0') {
          if (ch == '~') {
#ifdef WIN_MAC
            *ptr = '\015';
#else
            *ptr = '\n';
#endif
          }
          ptr++;
          ch = *ptr;
        }
      }
      SafeSetTitle (spp->comment, str);
      MemFree (str);
      PointerToDialog (spp->date, sbp->reldate);
      spp->tool [0] = '\0';
      if (! StringHasNoText (sbp->tool)) {
        StringNCpy_0 (spp->tool, sbp->tool, sizeof (spp->tool));
      }
    }
    ChangeSubtype (spp->subtype);
    ChangeHup (spp->hup);
    RestorePort (tempPort);
  }
}

static Pointer SubmitPageToSubmitBlockPtr (DialoG d)

{
  Char            ch;
  Int2            i;
  CharPtr         os;
  CharPtr         ptr;
  SubmitBlockPtr  sbp;
  CharPtr         sequin_app_version;
  SubmitPagePtr   spp;
  Char            tmp [64];

  sbp = NULL;
  spp = (SubmitPagePtr) GetObjectExtra (d);
  if (spp != NULL) {
    sbp = SubmitBlockNew ();
    if (sbp != NULL) {
      sbp->hup = (Boolean) (GetValue (spp->hup) == 2);
      if (spp->subtype != NULL) {
        i = GetValue (spp->subtype);
        if (i > 3 || i < 0) {
          i = 255;
        }
        sbp->subtype = i;
      } else {
        sbp->subtype = 1;
      }
      /*
      sbp->user_tag = SaveStringFromText (spp->userID);
      */
      sbp->comment = SaveStringFromTextAndStripNewlines (spp->comment);
      ptr = sbp->comment;
      if (ptr != NULL) {
        ch = *ptr;
        while (ch != '\0') {
          if (ch < ' ' || ch > '~') {
            *ptr = '~';
          }
          ptr++;
          ch = *ptr;
        }
      }
      if (StringHasNoText (spp->tool)) {
        sequin_app_version = (CharPtr) GetAppProperty ("SequinAppVersion");
        if (sequin_app_version != NULL) {
          os = GetOpSysString ();
          if (os != NULL) {
            sprintf (tmp, "Sequin %s - %s", sequin_app_version, os);
          } else {
            sprintf (tmp, "Sequin %s", sequin_app_version);
          }
        } else {
          StringCpy (tmp, "Sequin");
        }
        sbp->tool = StringSave (tmp);
      } else {
        sbp->tool = StringSave (spp->tool);
      }
      sbp->reldate = DialogToPointer (spp->date);
    }
  }
  return (Pointer) sbp;
}

static ValNodePtr TestSubmitPage (DialoG d)

{
  DatePtr        dp;
  SubmitPagePtr  spp;
  ValNodePtr     head;

  head = NULL;
  spp = (SubmitPagePtr) GetObjectExtra (d);
  if (spp != NULL) {
    switch (GetValue (spp->subtype)) {
      case 1 :
        switch (GetValue (spp->hup)) {
          case 0 :
            head = AddStringToValNodeChain (head,
              "You must state whether your data can be released immediately or not", 0);
            break;
          case 1 :
            break;
          case 2 :
            dp = DialogToPointer (spp->date);
            if (dp == NULL) {
              head = AddStringToValNodeChain (head, "You must specify a date for release", 0);
            }
            DateFree (dp);
            break;
          default :
            break;
        }
        break;
      case 2 :
        break;
      default :
        break;
    }
  }
  return head;
}

extern DialoG CreateSubmitDataDialog (GrouP h, CharPtr title, Boolean newOnly, Boolean defaultAsUpdate)

{
  GrouP          g;
  GrouP          k;
  GrouP          n;
  GrouP          p;
  GrouP          s;
  GrouP          t;
  SubmitPagePtr  spp;

  p = HiddenGroup (h, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  spp = (SubmitPagePtr) MemNew (sizeof (SubmitPage));
  if (spp != NULL) {
    SetObjectExtra (p, spp, StdCleanupExtraProc);
    spp->dialog = (DialoG) p;
    spp->todialog = SubmitBlockPtrToSubmitPage;
    spp->fromdialog = SubmitPageToSubmitBlockPtr;
    spp->testdialog = TestSubmitPage;

    if (title != NULL && title [0] != '\0') {
      s = NormalGroup (p, 0, -2, title, systemFont, NULL);
    } else {
      s = HiddenGroup (p, 0, -2, NULL);
    }
    n = HiddenGroup (s, -1, 0, NULL);
    SetGroupSpacing (n, 3, 15);

    t = NULL;
    if (! newOnly) {
      t = HiddenGroup (n, 3, 0, NULL);
      SetGroupSpacing (t, 3, 10);
      StaticPrompt (t, "Submission Type", 0, stdLineHeight, programFont, 'l');
      spp->subtype = HiddenGroup (t, 4, 0, ChangeSubtype);
      SetObjectExtra (spp->subtype, spp, NULL);
      RadioButton (spp->subtype, "New");
      RadioButton (spp->subtype, "Update");
      if (defaultAsUpdate) {
        SetValue (spp->subtype, 2);
      } else {
        SetValue (spp->subtype, 1);
      }
    }

    k = HiddenGroup (n, 0, 0, NULL);

    spp->releaseGrp = HiddenGroup (k, -1, 0, NULL);
    g = HiddenGroup (spp->releaseGrp, 3, 0, NULL);
    SetGroupSpacing (g, 3, 10);
    StaticPrompt (g, "May we release this record before publication?",
                  0, stdLineHeight, programFont, 'l');
    spp->hup = HiddenGroup (g, 3, 0, ChangeHup);
    SetObjectExtra (spp->hup, spp, NULL);
    RadioButton (spp->hup, "Yes");
    RadioButton (spp->hup, "No");
    SetValue (spp->hup, 1);
    spp->releaseDate = HiddenGroup (spp->releaseGrp, 10, 0, NULL);
    StaticPrompt (spp->releaseDate, "Release Date: ", 0, popupMenuHeight, programFont, 'l');
    spp->date = CreateDateDialog (spp->releaseDate, NULL);
    AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) spp->releaseDate, NULL);
    Hide (spp->releaseDate);
    if (GetValue (spp->subtype) == 2) {
      SafeHide (spp->releaseGrp);
    }

    g = NULL;
    if (! newOnly) {
      g = HiddenGroup (n, 0, 2, NULL);
      StaticPrompt (g, "Special Instructions to Database Staff", 0, 0, programFont, 'c');
      spp->comment = ScrollText (g, 30, 4, programFont, TRUE, NULL);
    }

    AlignObjects (ALIGN_CENTER, (HANDLE) spp->releaseGrp,
                  (HANDLE) t, (HANDLE) g, NULL);
  }

  return (DialoG) p;
}

#define SUBMISSION_PAGE   0
#define CONTACT_PAGE      1
#define CITATION_PAGE     2

typedef struct submitform {
  FORM_MESSAGE_BLOCK
  GrouP           pages [3];
  Boolean         visited [3];

  Int2            currentPage;
  Boolean         firstTime;
  Boolean         contactSeen;

  DialoG          submit;
  DialoG          contact;
  DialoG          citsub;

  ButtoN          process;
} SubmitForm, PNTR SubmitFormPtr;

static void SubmitBlockPtrToSubmitForm (ForM f, Pointer data)

{
  SubmitFormPtr   sbfp;
  SubmitBlockPtr  sbp;

  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  sbp = (SubmitBlockPtr) data;
  if (sbfp != NULL) {
    sbfp->firstTime = FALSE;
    if (sbp) {
      PointerToDialog (sbfp->submit, (Pointer) sbp);
      PointerToDialog (sbfp->contact, (Pointer) sbp->contact);
      PointerToDialog (sbfp->citsub, (Pointer) sbp->cit);
    } else {
      PointerToDialog (sbfp->submit, NULL);
      PointerToDialog (sbfp->contact, NULL);
      PointerToDialog (sbfp->citsub, NULL);
    }
  }
}

static Pointer SubmitFormToSubmitBlockPtr (ForM f)

{
  CitSubPtr       csp;
  SubmitFormPtr   sbfp;
  SubmitBlockPtr  sbp;

  sbp = NULL;
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    sbp = (SubmitBlockPtr) DialogToPointer (sbfp->submit);
    if (sbp != NULL) {
      sbp->contact = (ContactInfoPtr) DialogToPointer (sbfp->contact);
      sbp->cit = (CitSubPtr) DialogToPointer (sbfp->citsub);
      if (sbp->contact == NULL || sbp->cit == NULL) {
        sbp = SubmitBlockFree (sbp);
      }
      csp = sbp->cit;
      if (csp->date == NULL) {
        csp->date = DateCurr ();
      }
    }
  }
  return (Pointer) sbp;
}

static ValNodePtr TestSubmitForm (ForM f)

{
  ValNodePtr     head;
  SubmitFormPtr  sbfp;
  ValNodePtr     vnp;

  head = NULL;

  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {

    vnp = TestDialog (sbfp->contact);
    if (vnp != NULL) {
      head = AddStringToValNodeChain (head, "Contact error messages:", 0);
      ValNodeLink (&head, vnp);
    }

    vnp = TestDialog (sbfp->submit);
    if (vnp != NULL) {
      head = AddStringToValNodeChain (head, "Submit error messages:", 0);
      ValNodeLink (&head, vnp);
    }

    vnp = TestDialog (sbfp->citsub);
    if (vnp != NULL) {
      head = AddStringToValNodeChain (head, "Citation error messages:", 0);
      ValNodeLink (&head, vnp);
    }

  }
  return head;
}

static Boolean ReadSubmitBlock (ForM f, CharPtr filename)

{
  Pointer         dataptr;
  Uint2           datatype;
  Uint2           entityID;
  SubmitFormPtr   sbfp;
  SubmitBlockPtr  sbp;
  SeqSubmitPtr    ssp;
  Char            path [PATH_MAX];

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
      dataptr = ObjMgrGenericAsnTextFileRead (path, &datatype, &entityID);
      if (dataptr != NULL && entityID > 0) {
        sbp = NULL;
        switch (datatype) {
          case OBJ_SUBMIT_BLOCK :
            sbp = (SubmitBlockPtr) dataptr;
            break;
          case OBJ_SEQSUB :
            ssp = (SeqSubmitPtr) dataptr;
            if (ssp != NULL) {
              sbp = ssp->sub;
            }
            break;
          default :
            break;
        }
        if (sbp != NULL) {
          SubmitBlockPtrToSubmitForm (f, sbp);
        }
        ObjMgrDelete (datatype, dataptr);
        if (sbp != NULL) {
          Update ();
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static Boolean WriteSubmitBlock (ForM f, CharPtr filename)

{
  AsnIoPtr        aip;
  SubmitFormPtr   sbfp;
  SubmitBlockPtr  sbp;
  Char            path [PATH_MAX];
#ifdef WIN_MAC
  FILE            *fp;
#endif

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
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
        sbp = SubmitFormToSubmitBlockPtr (f);
        SubmitBlockAsnWrite (sbp, aip, NULL);
        AsnIoClose (aip);
        sbp = SubmitBlockFree (sbp);
        return TRUE;
      }
    }
  }
  return FALSE;
}

static Boolean ImportSubmitForm (ForM f, CharPtr filename)

{
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    switch (sbfp->currentPage) {
      case SUBMISSION_PAGE :
        return ReadSubmitBlock (f, filename);
      case CONTACT_PAGE :
        return ImportDialog (sbfp->contact, filename);
      case CITATION_PAGE :
        return ImportDialog (sbfp->citsub, filename);
      default :
        break;
    }
  }
  return FALSE;
}

static Boolean ExportSubmitForm (ForM f, CharPtr filename)

{
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    switch (sbfp->currentPage) {
      case SUBMISSION_PAGE :
        return WriteSubmitBlock (f, filename);
      case CONTACT_PAGE :
        return ExportDialog (sbfp->contact, filename);
      case CITATION_PAGE :
        return ExportDialog (sbfp->citsub, filename);
      default :
        break;
    }
  }
  return FALSE;
}

static void CopyContactToCitAuthors (SubmitFormPtr sbfp)

{
  AuthListPtr     alp;
  AuthorPtr       ap;
  ContactInfoPtr  cip;
  CitsubPagePtr   cpp;
  ValNodePtr      names;

  if (sbfp == NULL) return;
  cpp = (CitsubPagePtr) GetObjectExtra (sbfp->citsub);
  if (cpp == NULL) return;
  cip = (ContactInfoPtr) DialogToPointer (sbfp->contact);
  if (cip == NULL) return;
  ap = NULL;
  if (cip->contact != NULL) {
    ap = (AuthorPtr) AsnIoMemCopy (cip->contact,
                                   (AsnReadFunc) AuthorAsnRead,
                                   (AsnWriteFunc) AuthorAsnWrite);
  }
  ContactInfoFree (cip);
  if (ap == NULL) return;
  alp = AuthListNew ();
  if (alp != NULL) {
    alp->choice = 1;
    names = ValNodeNew (NULL);
    alp->choice = 1;
    alp->names = names;
    if (names != NULL) {
      names->choice = 1;
      names->data.ptrvalue = ap;
    }
    if (ap == NULL) {
      alp = AuthListFree (alp);
    }
    if (alp != NULL) {
       PointerToDialog (cpp->authors, (Pointer) alp);
    }
  }
  alp = AuthListFree (alp);
}

static void SetSubmitBlockImportExportItems (SubmitFormPtr sbfp)

{
  CitSubPtr  csp;
  IteM       exportItm;
  IteM       importItm;

  if (sbfp != NULL) {
    importItm = FindFormMenuItem ((BaseFormPtr) sbfp, VIB_MSG_IMPORT);
    exportItm = FindFormMenuItem ((BaseFormPtr) sbfp, VIB_MSG_EXPORT);
    switch (sbfp->currentPage) {
      case SUBMISSION_PAGE :
        SafeSetTitle (importItm, "Import Submitter Info...");
        SafeSetTitle (exportItm, "Export Submitter Info...");
        SafeEnable (importItm);
        SafeEnable (exportItm);
        break;
      case CONTACT_PAGE :
        SafeSetTitle (importItm, "Import Contact...");
        SafeSetTitle (exportItm, "Export Contact...");
        SafeEnable (importItm);
        SafeEnable (exportItm);
        break;
      case CITATION_PAGE :
        csp = (CitSubPtr) DialogToPointer (sbfp->citsub);
        if (csp == NULL) {
          CopyContactToCitAuthors (sbfp);
        }
        CitSubFree (csp);
        SafeSetTitle (importItm, "Import Citation...");
        SafeSetTitle (exportItm, "Export Citation...");
        SafeEnable (importItm);
        SafeEnable (exportItm);
        break;
      default :
        break;
    }
  }
}

static void ChangeSubmitBlockPage (VoidPtr data, Int2 newval, Int2 oldval)

{
  /*
  ContactInfoPtr  cip;
  CitSubPtr       csp;
  */
  Int2            i;
  SubmitFormPtr   sbfp;
  Int2            sum;

  sbfp = (SubmitFormPtr) data;
  if (sbfp != NULL) {
    sbfp->currentPage = newval;
    SafeHide (sbfp->pages [oldval]);
    Update ();
    if (sbfp->firstTime && sbfp->contactSeen && newval == CITATION_PAGE) {
      /*
      cip = (ContactInfoPtr) DialogToPointer (sbfp->contact);
      if (cip != NULL) {
        csp = CitSubFromContactInfo (cip);
        if (csp != NULL) {
          PointerToDialog (sbfp->citsub, (Pointer) csp);
          CitSubFree (csp);
        }
        ContactInfoFree (cip);
      }
      */
      sbfp->firstTime = FALSE;
    }
    SafeShow (sbfp->pages [newval]);
    sbfp->visited [sbfp->currentPage] = TRUE;
    if (newval == CONTACT_PAGE) {
      sbfp->contactSeen = TRUE;
    }
    sum = 0;
    for (i = 0; i < 3; i++) {
      if (sbfp->visited [i]) {
        sum++;
      }
    }
    if (sum >= 3) {
      SafeEnable (sbfp->process);
    }
    SetSubmitBlockImportExportItems (sbfp);
    Update ();
  }
}

static CharPtr  submitFormTabs [] = {
  "Submission", "Contact", "Citation", NULL
};

static void SubmitBlockFormMessage (ForM f, Int2 mssg)

{
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    switch (mssg) {
      case VIB_MSG_IMPORT :
        ImportSubmitForm (f, NULL);
        break;
      case VIB_MSG_EXPORT :
        ExportSubmitForm (f, NULL);
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
        if (sbfp->appmessage != NULL) {
          sbfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void SubmitBlockFormActivate (WindoW w)

{
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) GetObjectExtra (w);
  if (sbfp != NULL) {
    if (sbfp->activate != NULL) {
      sbfp->activate (w);
    }
    SetSubmitBlockImportExportItems (sbfp);
  }
}

static void AcceptSubmitBlockFormButtonProc (ButtoN b)

{
  Boolean         failed;
  SubmitFormPtr   sbfp;
  SubmitBlockPtr  sbp;

  sbp = NULL;
  failed = FALSE;
  sbfp = (SubmitFormPtr) GetObjectExtra (b);
  if (sbfp == NULL) return;
  sbp = (SubmitBlockPtr) DialogToPointer (sbfp->submit);
  if (sbp != NULL) {
    sbp->contact = (ContactInfoPtr) DialogToPointer (sbfp->contact);
    sbp->cit = (CitSubPtr) DialogToPointer (sbfp->citsub);
    if (sbp->contact == NULL && sbp->cit == NULL) {
      failed = TRUE;
      Message (MSG_OK, "Requires contact and citation information");
    } else if (sbp->contact == NULL) {
      failed = TRUE;
      Message (MSG_OK, "Requires contact information");
    } else if (sbp->cit == NULL) {
      failed = TRUE;
      Message (MSG_OK, "Requires citation information");
    } else if (sbp->hup && sbp->reldate == NULL) {
      failed = TRUE;
      Message (MSG_OK, "Requires release date");
    }
  }
  sbp = SubmitBlockFree (sbp);
  if (failed) return;
  Hide (sbfp->form);
  if (b != NULL) {
    if (sbfp != NULL && sbfp->form != NULL && sbfp->actproc != NULL) {
      (sbfp->actproc) (sbfp->form);
    }
  }
  Update ();
  Remove (sbfp->form);
}

static void SubmitBlockFormCleanupProc (GraphiC g, VoidPtr data)

{
  SubmitFormPtr  sbfp;
  Uint2          userkey;

  sbfp = (SubmitFormPtr) data;
  if (sbfp != NULL) {
    if (sbfp->input_entityID > 0 && sbfp->userkey > 0) {
      userkey = sbfp->userkey;
      sbfp->userkey = 0;
      ObjMgrFreeUserData (sbfp->input_entityID, sbfp->procid, sbfp->proctype, userkey);
    }
  }
  StdCleanupExtraProc (g, data);
}

extern ForM CreateSubmitBlockForm (Int2 left, Int2 top, CharPtr title,
                                   Boolean newOnly, Boolean defaultAsUpdate, SubmitBlockPtr sbp,
                                   BtnActnProc cnclproc, FormActnFunc actproc)

{
  ButtoN             b;
  GrouP              c;
  CitSubPtr          csp;
  GrouP              h;
  Int2               i;
  GrouP              j;
  GrouP              q;
  SubmitFormPtr      sbfp;
  StdEditorProcsPtr  sepp;
  DialoG             tbs;
  WindoW             w;

  w = NULL;
  sbfp = MemNew (sizeof (SubmitForm));
  if (sbfp != NULL) {
    if (cnclproc == NULL) {
      w = FixedWindow (left, top, -10, -10, title, StdCloseWindowProc);
    } else {
      w = FixedWindow (left, top, -10, -10, title, NULL);
    }
    SetObjectExtra (w, sbfp, SubmitBlockFormCleanupProc);
    sbfp->form = (ForM) w;
    sbfp->actproc = actproc;
    sbfp->toform = SubmitBlockPtrToSubmitForm;
    sbfp->fromform = SubmitFormToSubmitBlockPtr;
    sbfp->testform = TestSubmitForm;
    sbfp->importform = ImportSubmitForm;
    sbfp->exportform = ExportSubmitForm;
    sbfp->formmessage = SubmitBlockFormMessage;

#ifndef WIN_MAC
    CreateStdEditorFormMenus (w);
#endif

    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      sbfp->activate = sepp->activateForm;
      sbfp->appmessage = sepp->handleMessages;
    }
    SetActivate (w, SubmitBlockFormActivate);

    j = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (j, 10, 10);

    tbs = CreateFolderTabs (j, submitFormTabs, 0, 0, 0,
                            SYSTEM_FOLDER_TAB,
                            ChangeSubmitBlockPage, (Pointer) sbfp);
    sbfp->currentPage = SUBMISSION_PAGE;
    sbfp->firstTime = TRUE;
    sbfp->contactSeen = FALSE;
    for (i = 0; i < 3; i++) {
      sbfp->visited [i] = FALSE;
    }
    sbfp->visited [sbfp->currentPage] = TRUE;

    h = HiddenGroup (w, 0, 0, NULL);

    q = HiddenGroup (h, -1, 0, NULL);
    SetGroupSpacing (q, 10, 20);
    sbfp->submit = CreateSubmitDataDialog (q, "", newOnly, defaultAsUpdate);
    sbfp->pages [SUBMISSION_PAGE] = q;
    Hide (sbfp->pages [SUBMISSION_PAGE]);

    q = HiddenGroup (h, -1, 0, NULL);
    SetGroupSpacing (q, 10, 20);
    sbfp->contact = CreateContactDialog (q, "");
    sbfp->pages [CONTACT_PAGE] = q;
    Hide (sbfp->pages [CONTACT_PAGE]);

    q = HiddenGroup (h, -1, 0, NULL);
    SetGroupSpacing (q, 10, 20);
    csp = NULL;
    if (sbp != NULL) {
      csp = sbp->cit;
    }
    sbfp->citsub = CreateCitSubDialog (q, "", csp);
    sbfp->pages [CITATION_PAGE] = q;
    Hide (sbfp->pages [CITATION_PAGE]);

    c = HiddenGroup (w, 2, 0, NULL);
    SetGroupSpacing (c, 10, 2);
    sbfp->process = PushButton (c, "Accept", AcceptSubmitBlockFormButtonProc);
    SetObjectExtra (sbfp->process, sbfp, NULL);
    if (cnclproc == NULL) {
      cnclproc = StdCancelButtonProc;
    }
    b = PushButton (c, "Cancel", cnclproc);
    SetObjectExtra (b, sbfp, NULL);

    AlignObjects (ALIGN_CENTER, (HANDLE) sbfp->pages [SUBMISSION_PAGE],
                  (HANDLE) sbfp->pages [CONTACT_PAGE],
                  (HANDLE) sbfp->pages [CITATION_PAGE],
                  (HANDLE) j, (HANDLE) c, NULL);

    RealizeWindow (w);

    Show (sbfp->pages [sbfp->currentPage]);
  }
  return (ForM) w;
}

static void SubmitBlockFormActnProc (ForM f)

{
  OMProcControl   ompc;
  SubmitFormPtr   sbfp;
  SubmitBlockPtr  sbp;

  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    MemSet ((Pointer) &ompc, 0, sizeof (OMProcControl));
    ompc.input_entityID = sbfp->input_entityID;
    ompc.input_itemID = sbfp->input_itemID;
    ompc.input_itemtype = sbfp->input_itemtype;
    ompc.output_itemtype = sbfp->input_itemtype;
    sbp = (SubmitBlockPtr) FormToPointer (sbfp->form);
    if (sbp != NULL) {
      ompc.output_data = (Pointer) sbp;
      if (ompc.input_entityID == 0) {
        if (! ObjMgrRegister (OBJ_SUBMIT_BLOCK, (Pointer) sbp)) {
          Message (MSG_ERROR, "ObjMgrRegister failed");
        }
      } else if (ompc.input_itemtype != OBJ_SUBMIT_BLOCK) {
        ompc.output_itemtype = OBJ_SUBMIT_BLOCK;
        if (! AttachDataForProc (&ompc, FALSE)) {
          Message (MSG_ERROR, "AttachDataForProc failed");
        }
        ObjMgrSendMsg (OM_MSG_UPDATE, sbfp->input_entityID,
                       sbfp->input_itemID, sbfp->input_itemtype);
      } else {
        if (! ReplaceDataForProc (&ompc, FALSE)) {
          Message (MSG_ERROR, "ReplaceDataForProc failed");
        }
        ObjMgrSendMsg (OM_MSG_UPDATE, sbfp->input_entityID,
                       sbfp->input_itemID, sbfp->input_itemtype);
      }
    }
  }
}

extern Int2 LIBCALLBACK SubmitBlockGenFunc (Pointer data)

{
  ObjMgrDataPtr     omdp;
  OMProcControlPtr  ompcp;
  OMUserDataPtr     omudp;
  SubmitFormPtr     sbfp;
  SubmitBlockPtr    sbp = NULL;
  SeqSubmitPtr      ssp;
  WindoW            w;

  ompcp = (OMProcControlPtr) data;
  sbp = NULL;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_SUBMIT_BLOCK :
      sbp = (SubmitBlockPtr) ompcp->input_data;
      break;
    case OBJ_SEQSUB :
      break;
    case OBJ_SEQSUB_CIT :
      omdp = ObjMgrGetData (ompcp->input_entityID);
      if (omdp != NULL && omdp->datatype == OBJ_SEQSUB) {
        ssp = (SeqSubmitPtr) omdp->dataptr;
        if (ssp != NULL && ssp->datatype == 1) {
          sbp = ssp->sub;
        }
       }
      break;
    case 0 :
      break;
    default :
      return OM_MSG_RET_ERROR;
  }
  omudp = ItemAlreadyHasEditor (ompcp->input_entityID, ompcp->input_itemID,
                                ompcp->input_itemtype, ompcp->proc->procid);
  if (omudp != NULL) {
    sbfp = (SubmitFormPtr) omudp->userdata.ptrvalue;
    if (sbfp != NULL) {
      Select (sbfp->form);
    }
    return OM_MSG_RET_DONE;
  }
  w = (WindoW) CreateSubmitBlockForm (-50, -33,
                                      "Submission Instructions",
                                      FALSE, FALSE, sbp, NULL,
                                      SubmitBlockFormActnProc);
  sbfp = (SubmitFormPtr) GetObjectExtra (w);
  if (sbfp != NULL) {
    sbfp->input_entityID = ompcp->input_entityID;
    sbfp->input_itemID = ompcp->input_itemID;
    sbfp->input_itemtype = ompcp->input_itemtype;
    sbfp->this_itemtype = OBJ_SUBMIT_BLOCK;
    sbfp->this_subtype = 0;
    if (ompcp->input_itemtype == OBJ_SEQSUB_CIT && ompcp->input_itemID == 1) {
      sbfp->input_itemtype = OBJ_SUBMIT_BLOCK;
    }
    sbfp->procid = ompcp->proc->procid;
    sbfp->proctype = ompcp->proc->proctype;
    sbfp->userkey = OMGetNextUserKey ();
    omudp = ObjMgrAddUserData (ompcp->input_entityID, ompcp->proc->procid,
	                           OMPROC_EDIT, sbfp->userkey);

    if (omudp != NULL) {
      omudp->userdata.ptrvalue = (Pointer) sbfp;
      omudp->messagefunc = StdVibrantEditorMsgFunc;
    }
    if (sbp != NULL) {
      PointerToForm (sbfp->form, (Pointer) sbp);
    }
  }
  Show (w);
  Select (w);
  return OM_MSG_RET_DONE;
}

static AuthListPtr NameToAuthList (CharPtr first, CharPtr last)

{
  AuthListPtr  alp;
  AuthorPtr    ap;
  ValNodePtr   names;
  NameStdPtr   nsp;
  PersonIdPtr  pid;

  alp = NULL;
  if (last != NULL && last [0] != '\0') {
    alp = AuthListNew ();
    if (alp != NULL) {
      alp->choice = 1;
      names = ValNodeNew (NULL);
      alp->names = names;
      if (names != NULL) {
        ap = AuthorNew ();
        names->choice = 1;
        names->data.ptrvalue = ap;
        if (ap != NULL) {
          pid = PersonIdNew ();
          ap->name = pid;
          if (pid != NULL) {
            pid->choice = 2;
            nsp = NameStdNew ();
            pid->data = nsp;
            if (nsp != NULL) {
              nsp->names [0] = StringSave (last);
              nsp->names [1] = StringSave (first);
            }
          }
        }
      }
    }
  }
  return alp;
}

extern CitSubPtr CitSubFromContactInfo (ContactInfoPtr cip)

{
  AuthListPtr  alp;
  AuthorPtr    ap;
  CitSubPtr    csp;
  NameStdPtr   nsp;
  PersonIdPtr  pid;

  csp = NULL;
  if (cip != NULL) {
    csp = CitSubNew ();
    if (csp != NULL) {
      alp = NULL;
      ap = cip->contact;
      if (ap != NULL) {
        pid = ap->name;
        if (pid != NULL && pid->choice == 2) {
          nsp = pid->data;
          if (nsp != NULL) {
            alp = NameToAuthList (nsp->names [1], nsp->names [0]);
            if (alp != NULL) {
              alp->affil = AsnIoMemCopy (ap->affil,
                                         (AsnReadFunc) AffilAsnRead,
                                         (AsnWriteFunc) AffilAsnWrite);
            }
          }
        }
      }
      csp->authors = alp;
      csp->date = DateCurr ();
    }
  }
  return csp;
}

