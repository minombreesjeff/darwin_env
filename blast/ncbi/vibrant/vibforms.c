/*   vibforms.c
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
* File Name:  vibforms.c
*
* Author:  Jonathan Kans, Sergei Egorov (EnumPopup code)
*
* Version Creation Date:   1/22/95
*
* $Revision: 6.16 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: vibforms.c,v $
* Revision 6.16  2005/04/05 13:09:32  bollin
* avoid SetEnumPopup error when resetting a TagList with a Popup control for
* which the alist has no 0 value.
*
* Revision 6.15  2005/03/30 21:09:33  bollin
* added function UpdateTagListPopupChoices so that if the user changes the
* values in the alist array passed to CreateTagListDialog, the changes can
* be reflected in the popup displayed
*
* Revision 6.14  2004/03/06 20:00:38  kans
* SetEnumPopupEx and SetEnumPopupByNameEx set popup to 0 if desired choice not found
*
* Revision 6.13  2004/01/16 22:35:37  kans
* added WidestString and WidestAlist
*
* Revision 6.12  2002/01/09 15:11:23  kans
* SetEnumPopupEx and SetEnumPopupByNameEx to suppress error if 0 or NULL initial value, called by dialog creators
*
* Revision 6.11  1999/10/21 16:50:40  kans
* use SafeSetValue for enum lists and spreadsheets
*
* Revision 6.10  1999/10/20 22:09:44  kans
* AlistDialogData public, has userdata field, convenience functions set userdata, initial value
*
* Revision 6.9  1999/10/18 21:47:39  kans
* MakeEnumFieldAlistFromValNodeList strips paren and changes slash to dash
*
* Revision 6.8  1999/10/18 15:33:03  kans
* added MakeEnumFieldAlistFromValNodeList
*
* Revision 6.7  1999/08/20 15:57:07  kans
* CreateEnumListDialog calculates width if passed in 0
*
* Revision 6.6  1999/06/17 18:05:14  kans
* fixed EnumPopupDialogToUIEnumPtr
*
* Revision 6.5  1999/06/16 20:55:02  kans
* added CreateEnumListDialog using SingleList
*
* Revision 6.4  1999/06/16 19:03:25  kans
* added CreateEnumPopupDialog
*
* Revision 6.3  1999/06/16 17:44:53  kans
* added DuplicateEnumFieldAlist
*
* Revision 6.2  1999/03/11 16:10:25  kans
* StringHasNoText and TrimSpacesAroundString moved to ncbistr
*
* Revision 6.1  1997/11/26 21:30:17  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.0  1997/08/25 18:56:59  madden
* Revision changed to 6.0
*
* Revision 5.15  1997/06/04 00:06:19  kans
* support for Japanese by Tomo Koike of DDBJ
*
* Revision 5.14  1997/05/05 15:01:28  kans
* implemented MultiLinePromptEx
*
 * Revision 5.13  1997/04/17  17:16:05  kans
 * needed total_vib_msg variable
 *
 * Revision 5.12  1997/04/17  16:17:50  kans
 * added RegisterFormMenuItemName (assigns unique number to string, to avoid
 * need to hard code form item numbers beyond that supplied by Vibrant)
 *
 * Revision 5.11  1997/03/05  20:05:01  kans
 * made an Int2 variable to negate from a Uint2 parameter
 *
 * Revision 5.10  1997/03/05  17:35:26  kans
 * RepeatProcOnHandles is not static
 *
 * Revision 5.9  1997/03/05  16:24:22  kans
 * added EnumAlist convenience functions, RepeatProcOnHandles
 *
 * Revision 5.8  1997/01/29  17:53:59  kans
 * minor changes due to StringNCpy_0 change
 *
 * Revision 5.7  1997/01/29  16:41:22  kans
 * using StringNCpy_0
 *
 * Revision 5.6  1996/11/25  22:59:35  kans
 * added noExtend field to TagList, and noExtend param to
 * CreateTagListDialogEx, to prevent spreadsheet from being enlarged by user
 *
 * Revision 5.5  1996/09/09  19:43:05  kans
 * prompt style spreadsheet column needed to be taller for minimal scroll bar
 *
 * Revision 5.4  1996/08/30  18:44:09  kans
 * added TAGLIST_PROMPT choice to make read-only spreadsheet column
 *
 * Revision 5.3  1996/07/18  19:44:33  kans
 * added userdataptr, cleanupuser, activate slots in form structure
 *
 * Revision 5.2  1996/07/05  15:16:54  kans
 * set folder tabs now selects initial tab, not tab group, for invalidating
 *
 * Revision 5.1  1996/07/01  22:46:22  kans
 * spreadsheet makes copy of types list (some applications change the list)
 *
 * Revision 5.0  1996/05/28  13:45:08  ostell
 * Set to revision 5.0
 *
 * Revision 4.26  1996/05/16  20:11:11  kans
 * prevent list popup element from having horizontal scroll bar (Motif)
 *
 * Revision 4.25  1996/03/21  00:46:49  kans
 * added TAGLIST_LIST to TAGLIST_TEXT and TAGLIST_POPUP
 *
 * Revision 4.24  1996/03/20  20:57:22  kans
 * create folder tab doesn't trash return value, set folder tab value
 * calls changeView application callback
 *
 * Revision 4.23  1996/02/27  16:31:13  kans
 * subclass folder tabs to allow SetValue (but with 0, not 1, as first value)
 *
 * Revision 4.22  1996/02/19  21:27:48  kans
 * form message menus on the Mac can now work on menus in windows
 *
 * Revision 4.21  1996/02/08  14:05:13  kans
 * *** empty log message ***
 *
 * Revision 4.20  1996/02/08  00:34:47  kans
 * new SetFormMenuItem function for registration (can cast in a MenU handle)
 *
 * Revision 4.19  1996/02/04  20:09:35  kans
 * tag list now accepts ENTER message, selects first text component
 *
 * Revision 4.18  1996/01/30  22:52:55  kans
 * spreadsheet sets bar page scrolling
 *
 * Revision 4.17  1996/01/29  18:02:30  kans
 * added filepath to base form structure (for Save menu item)
 *
 * Revision 4.16  1996/01/24  18:32:59  kans
 * CreateTagListDialogEx implemented (hidden from public view for now)
 *
 * Revision 4.15  1996/01/14  22:43:47  kans
 * added VIB_MSG_ACCEPT, three new send message standard procs
 *
 * Revision 4.14  1995/12/30  19:41:45  kans
 * added CreateTextTabs, FormCommandItem, FindFormMenuItem, and VIB_MSG_CLOSE
 *
 * Revision 4.13  1995/12/30  00:46:29  kans
 * new messages for enter, init, reset, redraw dialog/form
 *
 * Revision 4.12  1995/12/29  23:15:05  kans
 * added send message dialog and form functions
 *
 * Revision 4.11  1995/12/26  15:07:44  kans
 * TagRtn now opens an empty space below the current spreadsheet row
 *
 * Revision 4.10  1995/12/22  20:44:56  kans
 * implemented TagRtnProc to insert in the middle of a spreadsheet
 *
 * Revision 4.9  1995/12/20  16:00:33  kans
 * font always set before calculating folder tab widths
 *
 * Revision 4.8  1995/12/12  23:32:31  kans
 * extra parameters to customize folder tab
 *
 * Revision 4.7  1995/12/07  23:17:57  kans
 * implemented tab offset for subsequent lines
 *
 * Revision 4.6  1995/12/07  00:57:41  kans
 * added maxPerLine and indentNextLine parameters to CreateFolderTabs
 *
 * Revision 4.5  1995/11/27  21:12:46  kans
 * SaveStringFromText returns NULL if string has no text
 *
 * Revision 4.4  1995/11/12  21:35:08  kans
 * added Import/Export Dialog/Form function slots
 *
 * Revision 4.3  1995/11/08  23:30:31  kans
 * removed edit block fields, which belong in the application
 *
 * Revision 4.2  1995/08/01  17:52:55  kans
 * MultiLinePrompt doesn't add CharWidth of newline or return
 *
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 1.9  1995/07/20  19:30:46  kans
 * *** empty log message ***
 *
 * Revision 1.8  1995/07/20  17:31:56  kans
 * added TrimSpacesAroundString function
 *
 * Revision 1.7  1995/06/07  23:05:58  kans
 * fixed MultiLinePrompt, which was dropping last character
 *
 * Revision 1.6  1995/06/04  19:32:02  kans
 * *** empty log message ***
 *
 * Revision 1.5  1995/05/24  21:06:19  kans
 * add initial page parameter to folder tabs
 *
 * Revision 1.4  1995/05/17  21:04:51  kans
 * removed unused variables
 *
 * Revision 1.3  1995/05/17  15:15:14  kans
 * added Log line
 *
*
* ==========================================================================
*/

#include <vibrant.h>

#ifdef VAR_ARGS
#include <varargs.h>
#else
#include <stdarg.h>
#endif

#ifdef WIN_MAC
Pointer Nlm_currentFormDataPtr = NULL;
Nlm_IteM PNTR Nlm_globalMenuItemList = NULL;
Nlm_Int2 Nlm_globalMenuListSize = 0;
#endif

/* enum value -> name */
CharPtr GetEnumName (UIEnum val, EnumFieldAssocPtr al)
{
  for (; al->name != NULL; al++)
     if (al->value == val) return al->name;
  Message(MSG_POSTERR, "GetEnumName: %ld", (long)val);
  return NULL;
}

/* enum field <-> popup list UI */
Boolean InitEnumPopup (PopuP lst, EnumFieldAssocPtr al, UIEnumPtr pdef) 
{
  Int2 i, ii;
  EnumFieldAssocPtr efap;
  CharPtr PNTR titles;
  if (al == NULL) {
    Message(MSG_POSTERR, "in InitEnumPopup");
    return FALSE;
  }
  /*
  for (i = 1, ii = -1; al->name != NULL; i++, al++) {
     if (i == 1) { 
       if (pdef != NULL) *pdef = al->value; 
       ii = 1; 
     }
     PopupItem (lst, al->name); 
  }
  */
  efap = al;
  for (i = 1, ii = -1; efap->name != NULL; i++, efap++) {
     if (i == 1) { 
       if (pdef != NULL) *pdef = efap->value; 
       ii = 1; 
     }
  }
  titles = (CharPtr PNTR) MemNew (sizeof (CharPtr) * i + 1);
  if (titles != NULL) {
    efap = al;
    for (i = 0; efap->name != NULL; i++, efap++) {
      titles [i] = efap->name;
    }
    Nlm_PopupItems (lst, titles);
    MemFree (titles);
  }
  if (ii > 0) {
    SafeSetValue (lst, ii);
    return TRUE;
  } else
    return FALSE;
} 

Boolean GetEnumPopup (PopuP lst, EnumFieldAssocPtr al, UIEnumPtr pval)
{
  Int2 i; Int2 is = GetValue (lst);
  for (i = 1; al->name != NULL; i++, al++)
     if (i == is) { *pval = al->value; return TRUE; }
  return FALSE;
} 

static void SetEnumPopupEx (PopuP lst, EnumFieldAssocPtr al, UIEnum val, Boolean zeroOkay) 
{
  Int2 i;
  for (i = 1; al->name != NULL; i++, al++)
     if (al->value == val) { SafeSetValue (lst, i); return; }
  if (zeroOkay && val == 0) return;
  Message(MSG_POSTERR, "SetEnumPopup: %ld", (long)val);
  SafeSetValue (lst, 0);
} 

void SetEnumPopup (PopuP lst, EnumFieldAssocPtr al, UIEnum val) 
{
  SetEnumPopupEx (lst, al, val, FALSE);
} 

CharPtr GetEnumPopupByName (PopuP lst, EnumFieldAssocPtr al)
{
  Int2 i; Int2 is = GetValue (lst);
  for (i = 1; al->name != NULL; i++, al++)
     if (i == is) { return StringSaveNoNull (al->name); }
  return NULL;
} 

static void SetEnumPopupByNameEx (PopuP lst, EnumFieldAssocPtr al, CharPtr name, Boolean zeroOkay)

{
  Int2 i;
  for (i = 1; al->name != NULL; i++, al++)
     if (StringICmp (al->name, name) == 0) { SafeSetValue (lst, i); return; }
  if (zeroOkay && StringHasNoText (name)) return;
  Message(MSG_POSTERR, "SetEnumPopupByName: %s", name);
  SafeSetValue (lst, 0);
}

void SetEnumPopupByName (PopuP lst, EnumFieldAssocPtr al, CharPtr name)

{
  SetEnumPopupByNameEx (lst, al, name, FALSE);
}

Boolean WhereInEnumPopup (EnumFieldAssocPtr al, CharPtr name, UIEnumPtr pval)
{
  for ( ; al->name != NULL; al++)
     if (StringICmp (al->name, name) == 0) { *pval = al->value; return TRUE; }
  return FALSE;
} 

static int LIBCALLBACK CompareAlists (VoidPtr ptr1, VoidPtr ptr2)

{
  EnumFieldAssocPtr  ap1;
  EnumFieldAssocPtr  ap2;

  if (ptr1 != NULL && ptr2 != NULL) {
    ap1 = (EnumFieldAssocPtr) ptr1;
    ap2 = (EnumFieldAssocPtr) ptr2;
    if (ap1 != NULL && ap2 != NULL) {
      return StringICmp (ap1->name, ap2->name);
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

void SortEnumFieldAlist (EnumFieldAssocPtr alist)

{
  EnumFieldAssocPtr  ap;
  size_t             count;

  if (alist == NULL) return;
  ap = alist;
  count = 0;
  while (ap->name != NULL) {
    ap++;
    count++;
  }
  if (count < 2) return;
  HeapSort (alist, count, sizeof (EnumFieldAssoc), CompareAlists);
}

EnumFieldAssocPtr DuplicateEnumFieldAlist (EnumFieldAssocPtr alist)

{
  EnumFieldAssocPtr  ap1, ap2;
  size_t             count;
  EnumFieldAssocPtr  newap;

  if (alist == NULL) return NULL;
  ap1 = alist;
  count = 0;
  while (ap1->name != NULL) {
    ap1++;
    count++;
  }
  if (count < 1) return NULL;
  newap = MemNew (sizeof (EnumFieldAssoc) * (count + 1));
  if (newap == NULL) return NULL;
  ap1 = alist;
  ap2 = newap;
  while (ap1->name != NULL) {
    ap2->name = StringSaveNoNull (ap1->name);
    ap2->value = ap1->value;
    ap1++;
    ap2++;
  }
  ap2->name = NULL;
  return newap;
}

EnumFieldAssocPtr FreeEnumFieldAlist (EnumFieldAssocPtr alist)

{
  Int2  j;

  if (alist != NULL) {
    for (j = 0; alist [j].name != NULL; j++) {
      MemFree (alist [j].name);
    }
    MemFree (alist);
  }
  return NULL;
}

EnumFieldAssocPtr MakeEnumFieldAlistFromValNodeList (ValNodePtr vlist)

{
  EnumFieldAssocPtr  ap, newap;
  Char               ch;
  size_t             count, i;
  CharPtr            ptr;
  ValNodePtr         vnp;

  if (vlist == NULL) return NULL;
  count = ValNodeLen (vlist);
  if (count < 1) return NULL;

  newap = MemNew (sizeof (EnumFieldAssoc) * (count + 1));
  if (newap == NULL) return NULL;

  for (ap = newap, vnp = vlist, i = 0;
       vnp != NULL && i < count;
       vnp = vnp->next, ap++, i++) {
    ap->name = StringSaveNoNull ((CharPtr) vnp->data.ptrvalue);
    ptr = StringStr (ap->name, " (");
    if (ptr != NULL) {
      *ptr = '\0';
    }
    if (ap->name != NULL) {
      ptr = ap->name;
      ch = *ptr;
      while (ch != '\0') {
        if (ch == '/') {
          *ptr = '-';
        }
        ptr++;
        ch = *ptr;
      }
    }
    ap->value = (UIEnum) vnp->choice;
  }

  ap->name = NULL;
  ap->value = 0;

  return newap;
}

PopuP CreateEnumPopupListInitVal (GrouP prnt, Boolean macLike, PupActnProc actn,
                                  VoidPtr data, EnumFieldAssocPtr al, UIEnum val)

{
  PopuP  p;

  p = PopupList (prnt, macLike, actn);
  SetObjectExtra (p, data, NULL);
  InitEnumPopup (p, al, NULL);
  SetEnumPopup (p, al, val);
  return p;
}

PopuP CreateEnumPopupListInitName (GrouP prnt, Boolean macLike, PupActnProc actn,
                                   VoidPtr data, EnumFieldAssocPtr al, CharPtr name)

{
  PopuP  p;

  p = PopupList (prnt, macLike, actn);
  SetObjectExtra (p, data, NULL);
  InitEnumPopup (p, al, NULL);
  SetEnumPopupByName (p, al, name);
  return p;
}

/* popup list autonomous dialog - copies alist, frees on cleanup */

static void UIEnumPtrToEnumPopupDialog (DialoG d, Pointer data)

{
  AlistDialogPtr  adp;
  Int4Ptr         intptr;
  Int4            val;

  adp = (AlistDialogPtr) GetObjectExtra (d);
  intptr = (Int4Ptr) data;
  if (adp != NULL && intptr != NULL) {
    val = *intptr;
    SetEnumPopup (adp->pop, adp->alist, (UIEnum) val);
  }
}

static Pointer EnumPopupDialogToUIEnumPtr (DialoG d)

{
  AlistDialogPtr  adp;
  Int4Ptr         intptr;
  UIEnum          val;

  intptr = NULL;
  adp = (AlistDialogPtr) GetObjectExtra (d);
  if (adp != NULL) {
    if (GetEnumPopup (adp->pop, adp->alist, &val)) {
      adp->intvalue = (Int4) val;
      intptr = (&adp->intvalue);
    }
  }
  return (Pointer) intptr;
}

static void ClearEnumPopupDialog (GraphiC g, VoidPtr data)

{
  AlistDialogPtr  adp;

  adp = (AlistDialogPtr) data;
  if (adp != NULL) {
    FreeEnumFieldAlist (adp->alist);
  }
  MemFree (data);
}

PopuP CreateEnumPopupDialog (GrouP prnt, Boolean macLike, PupActnProc actn,
                             EnumFieldAssocPtr al, UIEnum val, Pointer userdata)

{
  AlistDialogPtr  adp;
  PopuP           p;

  if (prnt == NULL || al == NULL) return NULL;
  adp = (AlistDialogPtr) MemNew (sizeof (AlistDialogData));
  if (adp == NULL) return NULL;
  p = PopupList (prnt, macLike, actn);
  if (p == NULL) {
    MemFree (adp);
    return NULL;
  }
  SetObjectExtra (p, adp, ClearEnumPopupDialog);
  adp->dialog = (DialoG) p;
  adp->todialog = UIEnumPtrToEnumPopupDialog;
  adp->fromdialog = EnumPopupDialogToUIEnumPtr;
  adp->pop = p;
  adp->alist = DuplicateEnumFieldAlist (al);
  adp->userdata = userdata;
  InitEnumPopup (p, adp->alist, NULL);
  SetEnumPopupEx (p, adp->alist, val, TRUE);
  return p;
}

LisT CreateEnumListDialog (GrouP prnt, Int2 width, Int2 height, LstActnProc actn,
                           EnumFieldAssocPtr al, UIEnum val, Pointer userdata)

{
  AlistDialogPtr     adp;
  EnumFieldAssocPtr  ap;
  Int2               i;
  Int2               len;
  LisT               p;

  if (prnt == NULL || al == NULL) return NULL;
  adp = (AlistDialogPtr) MemNew (sizeof (AlistDialogData));
  if (adp == NULL) return NULL;
  if (width == 0) {
    SelectFont (systemFont);
    for (ap = al, i = 1; ap->name != NULL; i++, ap++) {
      len = StringWidth (ap->name);
      if (len > width) {
        width = len;
      }
    }
    width += stdCharWidth - 1;
    width /= stdCharWidth;
    width += 1;
  }
  p = SingleList (prnt, width, height, actn);
  if (p == NULL) {
    MemFree (adp);
    return NULL;
  }
  SetObjectExtra (p, adp, ClearEnumPopupDialog);
  adp->dialog = (DialoG) p;
  adp->todialog = UIEnumPtrToEnumPopupDialog;
  adp->fromdialog = EnumPopupDialogToUIEnumPtr;
  adp->pop = (PopuP) p;
  adp->alist = DuplicateEnumFieldAlist (al);
  adp->userdata = userdata;
  for (ap = adp->alist; ap->name != NULL; ap++) {
    ListItem (p, ap->name);
  }
  SetEnumPopupEx ((PopuP) p, adp->alist, val, TRUE);
  return p;
}

#ifdef VAR_ARGS
void CDECL RepeatProcOnHandles (proc, va_alist)
HandleActnProc proc;
va_dcl
#else
void CDECL RepeatProcOnHandles (HandleActnProc proc, ...)
#endif

{
  va_list     args;
  Nlm_Handle  obj;

#ifdef VAR_ARGS
  va_start (args);
#else
  va_start (args, proc);
#endif
  obj = (Nlm_Handle) va_arg (args, Nlm_HANDLE);
  while (obj != NULL) {
    if (proc != NULL) {
      proc (obj);
    }
    obj = (Nlm_Handle) va_arg (args, Nlm_HANDLE);
  }
  va_end(args);
}

extern void PointerToDialog (DialoG d, Pointer ptr)

{
  BaseDialogPtr  bdp;

  if (d != NULL) {
    bdp = (BaseDialogPtr) GetObjectExtra (d);
    if (bdp != NULL && bdp->todialog != NULL) {
      (bdp->todialog) (d, ptr);
    }
  }
}

extern Pointer DialogToPointer (DialoG d)

{
  BaseDialogPtr  bdp;

  if (d != NULL) {
    bdp = (BaseDialogPtr) GetObjectExtra (d);
    if (bdp != NULL && bdp->fromdialog != NULL) {
      return (bdp->fromdialog) (d);
    }
  }
  return NULL;
}

extern ValNodePtr TestDialog (DialoG d)

{
  BaseDialogPtr  bdp;

  if (d != NULL) {
    bdp = (BaseDialogPtr) GetObjectExtra (d);
    if (bdp != NULL && bdp->testdialog != NULL) {
      return (bdp->testdialog) (d);
    }
  }
  return NULL;
}

extern void SendMessageToDialog (DialoG d, Int2 mssg)

{
  BaseDialogPtr  bdp;

  if (d != NULL) {
    bdp = (BaseDialogPtr) GetObjectExtra (d);
    if (bdp != NULL && bdp->dialogmessage != NULL) {
      (bdp->dialogmessage) (d, mssg);
    }
  }
}

extern Boolean ImportDialog (DialoG d, CharPtr filename)

{
  BaseDialogPtr  bdp;

  if (d != NULL) {
    bdp = (BaseDialogPtr) GetObjectExtra (d);
    if (bdp != NULL && bdp->importdialog != NULL) {
      return (bdp->importdialog) (d, filename);
    }
  }
  return FALSE;
}

extern Boolean ExportDialog (DialoG d, CharPtr filename)

{
  BaseDialogPtr  bdp;

  if (d != NULL) {
    bdp = (BaseDialogPtr) GetObjectExtra (d);
    if (bdp != NULL && bdp->exportdialog != NULL) {
      return (bdp->exportdialog) (d, filename);
    }
  }
  return FALSE;
}

extern void StdCloseWindowProc (WindoW w)

{
  Remove (w);
}

extern void StdCancelButtonProc (ButtoN b)

{
  Remove (ParentWindow (b));
}

extern void StdSendCloseWindowMessageProc (WindoW w)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (w);
  if (bfp == NULL) return;
  SendMessageToForm (bfp->form, VIB_MSG_CLOSE);
}

extern void StdSendCancelButtonMessageProc (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (ParentWindow (b));
  if (bfp == NULL) return;
  SendMessageToForm (bfp->form, VIB_MSG_CLOSE);
}

extern void StdSendAcceptButtonMessageProc (ButtoN b)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (ParentWindow (b));
  if (bfp == NULL) return;
  SendMessageToForm (bfp->form, VIB_MSG_ACCEPT);
}

extern void PointerToForm (ForM f, Pointer ptr)

{
  BaseFormPtr  bfp;

  if (f != NULL) {
    bfp = (BaseFormPtr) GetObjectExtra (f);
    if (bfp != NULL && bfp->toform != NULL) {
      (bfp->toform) (f, ptr);
    }
  }
}

extern Pointer FormToPointer (ForM f)

{
  BaseFormPtr  bfp;

  if (f != NULL) {
    bfp = (BaseFormPtr) GetObjectExtra (f);
    if (bfp != NULL && bfp->fromform != NULL) {
      return (bfp->fromform) (f);
    }
  }
  return NULL;
}

extern ValNodePtr TestForm (ForM f)

{
  BaseFormPtr  bfp;

  if (f != NULL) {
    bfp = (BaseFormPtr) GetObjectExtra (f);
    if (bfp != NULL && bfp->testform != NULL) {
      return (bfp->testform) (f);
    }
  }
  return NULL;
}

extern void SendMessageToForm (ForM f, Int2 mssg)

{
  BaseFormPtr  bfp;

  if (f != NULL) {
    bfp = (BaseFormPtr) GetObjectExtra (f);
    if (bfp != NULL && bfp->formmessage != NULL) {
      (bfp->formmessage) (f, mssg);
    }
  }
}

extern Boolean ImportForm (ForM f, CharPtr filename)

{
  BaseFormPtr  bfp;

  if (f != NULL) {
    bfp = (BaseFormPtr) GetObjectExtra (f);
    if (bfp != NULL && bfp->importform != NULL) {
      return (bfp->importform) (f, filename);
    }
  }
  return FALSE;
}

extern Boolean ExportForm (ForM f, CharPtr filename)

{
  BaseFormPtr  bfp;

  if (f != NULL) {
    bfp = (BaseFormPtr) GetObjectExtra (f);
    if (bfp != NULL && bfp->exportform != NULL) {
      return (bfp->exportform) (f, filename);
    }
  }
  return FALSE;
}

extern void SetFormActnProc (ForM f, FormActnFunc actproc)

{
  BaseFormPtr  bfp;

  if (f != NULL) {
    bfp = (BaseFormPtr) GetObjectExtra (f);
    if (bfp != NULL) {
      bfp->actproc = actproc;
    }
  }
}

extern void StdAcceptFormButtonProc (ButtoN b)

{
  BaseFormPtr  bfp;

  Hide (ParentWindow (b));
  if (b != NULL) {
    bfp = (BaseFormPtr) GetObjectExtra (b);
    if (bfp != NULL && bfp->form != NULL && bfp->actproc != NULL) {
      (bfp->actproc) (bfp->form);
    }
  }
  Update ();
  Remove (ParentWindow (b));
}

extern void StdCleanupExtraProc (GraphiC g, VoidPtr data)

{
  MemFree (data);
}

extern void StdCleanupFormProc (GraphiC g, VoidPtr data)

{
  BaseFormPtr  bfp;

  if (data) {
    bfp = (BaseFormPtr) data;
    MemFree (bfp->menuitemlist);
    MemFree (bfp->filepath);
    if (bfp->userDataPtr != NULL && bfp->cleanupuser != NULL) {
      (bfp->cleanupuser) (g, (VoidPtr) bfp->userDataPtr);
    }
  }
  MemFree (data);
}

typedef struct itemobjectdata {
  Int2         mssg;
  IteM         item;
  BaseFormPtr  bfp;
} ItemObjectData, PNTR ItemObjectPtr;

static void FormCommandItemProc (IteM i)

{
  BaseFormPtr    bfp;
  ItemObjectPtr  iop;

  iop = (ItemObjectPtr) GetObjectExtra (i);
  if (iop == NULL) return;
#ifdef WIN_MAC
  bfp = iop->bfp;
  if (bfp == NULL) {
    bfp = (BaseFormPtr) currentFormDataPtr;
  }
#else
  bfp = iop->bfp;
#endif
  if (bfp == NULL) return;
  SendMessageToForm (bfp->form, iop->mssg);
}

#define MNULSTCHUNK 32

static Int2  Nlm_total_vib_msg = NUM_VIB_MSG;

extern void SetFormMenuItem (BaseFormPtr bfp, Int2 mssg, IteM itm)

{
  Int2       len;
  IteM PNTR  menuitemlist;
  Int2       menulistsize;
  IteM PNTR  olditemlist;
  Int2       oldlistsize;

  len = (Int2) MAX ((Int2) ((mssg / MNULSTCHUNK + 1) * MNULSTCHUNK), (Int2) Nlm_total_vib_msg);
  menuitemlist = NULL;
  menulistsize = 0;
#ifdef WIN_MAC
  if (bfp != NULL) {
    menuitemlist = bfp->menuitemlist;
    menulistsize = bfp->menulistsize;
  } else {
    menuitemlist = globalMenuItemList;
    menulistsize = globalMenuListSize;
  }
#else
  if (bfp == NULL) return;
  menuitemlist = bfp->menuitemlist;
  menulistsize = bfp->menulistsize;
#endif
  if (menuitemlist == NULL) {
    menuitemlist = (IteM *) MemNew((size_t) len * sizeof (IteM));
    menulistsize = len;
  } else if (menulistsize < len) {
    olditemlist = menuitemlist;
    oldlistsize = menulistsize;
    menuitemlist = (IteM *) MemNew ((size_t) len * sizeof (IteM));
    menulistsize = len;
    MemMove ((void *) menuitemlist, (void *) olditemlist, (size_t) oldlistsize * sizeof (IteM));
    MemFree (olditemlist);
  }
#ifdef WIN_MAC
  if (bfp != NULL) {
    bfp->menuitemlist = menuitemlist;
    bfp->menulistsize = menulistsize;
  } else {
    globalMenuItemList = menuitemlist;
    globalMenuListSize = menulistsize;
  }
#else
  bfp->menuitemlist = menuitemlist;
  bfp->menulistsize = menulistsize;
#endif
  if (menuitemlist != NULL && mssg >= 0 && mssg < Nlm_total_vib_msg) {
    menuitemlist [mssg] = itm;
  }
}

extern IteM FormCommandItem (MenU m, CharPtr title,
                             BaseFormPtr bfp, Int2 mssg)

{
  IteM           i;
  ItemObjectPtr  iop;

  i = CommandItem (m, title, FormCommandItemProc);
  iop = (ItemObjectPtr) MemNew (sizeof (ItemObjectData));
  if (iop != NULL) {
    iop->bfp = bfp;
    iop->item = i;
    iop->mssg = mssg;
  }
  SetObjectExtra (i, (Pointer) iop, StdCleanupExtraProc);
  SetFormMenuItem (bfp, mssg, i);
  return i;
}

extern IteM FindFormMenuItem (BaseFormPtr bfp, Int2 mssg)

{
  IteM PNTR  menuitemlist;
  Int2       menulistsize;

  menuitemlist = NULL;
  menulistsize = 0;
#ifdef WIN_MAC
  menuitemlist = globalMenuItemList;
  menulistsize = globalMenuListSize;
#else
  if (bfp != NULL) {
    menuitemlist = bfp->menuitemlist;
    menulistsize = bfp->menulistsize;
  }
#endif
  if (menuitemlist != NULL && mssg >= 0 && mssg < menulistsize) {
    return menuitemlist [mssg];
  }
  return NULL;
}

static ValNodePtr  Nlm_vibFormsNamedMenuItemList = NULL;

extern Int2 RegisterFormMenuItemName (CharPtr title)

{
  Int2        mssg;
  ValNodePtr  vnp;

  mssg = NUM_VIB_MSG + 1;
  if (StringHasNoText (title)) return mssg;
  vnp = Nlm_vibFormsNamedMenuItemList;
  while (vnp != NULL) {
    if (StringICmp (title, (CharPtr) vnp->data.ptrvalue) == 0) {
      return mssg;
    }
    vnp = vnp->next;
    mssg++;
  }
  ValNodeCopyStr (&Nlm_vibFormsNamedMenuItemList, 0, title);
  Nlm_total_vib_msg = mssg + 1;
  return mssg;
}

extern void Nlm_FreeForms (void);
extern void Nlm_InitForms (void);

void Nlm_InitForms (void)

{
  Nlm_vibFormsNamedMenuItemList = NULL;
  Nlm_total_vib_msg = NUM_VIB_MSG;
}

void Nlm_FreeForms (void)

{
  Nlm_vibFormsNamedMenuItemList = ValNodeFreeData (Nlm_vibFormsNamedMenuItemList);
}

extern void SafeShow (Handle a)

{
  if (! Visible (a)) {
    Show (a);
  }
}

extern void SafeHide (Handle a)

{
  if (Visible (a)) {
    Hide (a);
  }
}

extern void SafeEnable (Handle a)

{
  if (! Enabled (a)) {
    Enable (a);
  }
}

extern void SafeDisable (Handle a)

{
  if (Enabled (a)) {
    Disable (a);
  }
}

extern void SafeSetValue (Handle a, Int2 value)

{
  Int2 oldval;

  oldval = GetValue (a);
  if (oldval != value) {
    SetValue (a, value);
  }
}

extern void SafeSetStatus (Handle a, Boolean status)

{
  Boolean  oldstat;

  oldstat = GetStatus (a);
  if (oldstat != status) {
    SetStatus (a, status);
  }
}

extern void SafeSetTitle (Handle a, CharPtr title)

{
  Char  str [256];

  GetTitle (a, str, sizeof (str));
  if (title == NULL || StringCmp (title, str) != 0) {
    SetTitle (a, title);
  }
}

extern CharPtr SaveStringFromText (TexT t)

{
  size_t   len;
  CharPtr  str;

  len = TextLength (t);
  if (len > 0) {
    str = (CharPtr) MemNew(len + 1);
    if (str != NULL) {
      GetTitle (t, str, len + 1);
      TrimSpacesAroundString (str);
      if (StringHasNoText (str)) {
        str = (CharPtr) MemFree(str);
      }
      return str;
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
}

extern ValNodePtr SetTextFromVnp (TexT t, ValNodePtr vnp)

{
  if (vnp != NULL) {
    SafeSetTitle (t, (CharPtr)vnp->data.ptrvalue);
    vnp = vnp->next;
  } else {
    SafeSetTitle (t, "");
  }
  return vnp;
}

extern ValNodePtr GetVnpFromText (TexT t, ValNodePtr vnp, Boolean last)

{
  Char  str [256];

  if (vnp != NULL) {
    GetTitle (t, str, sizeof (str));
    vnp->data.ptrvalue = StringSave (str);
    if (! last) {
      vnp = ValNodeNew (vnp);
    }
  }
  return vnp;
}

extern Boolean TextHasNoText (TexT t)

{
  CharPtr  ptr;
  Boolean  rsult;

  if (t != NULL) {
    ptr = SaveStringFromText (t);
    rsult = StringHasNoText (ptr);
    MemFree (ptr);
    return rsult;
  }
  return TRUE;
}

extern Int2 MaxStringWidths (CharPtr PNTR strs)

{
  Int2  i;
  Int2  len;
  Int2  max;

  max = 0;
  if (strs != NULL) {
    i = 0;
    while (strs [i] != NULL) {
      len = StringWidth (strs [i]);
      if (len > max) {
        max = len;
      }
      i++;
    }
  }
  return (max + 2);
}

extern CharPtr WidestString (CharPtr PNTR strs)

{
  Int2     i;
  Int2     len;
  Int2     max;
  CharPtr  str;

  str = NULL;
  max = 0;
  if (strs != NULL) {
    i = 0;
    while (strs [i] != NULL) {
      len = StringWidth (strs [i]);
      if (len > max) {
        max = len;
        str = strs [i];
      }
      i++;
    }
  }
  return str;
}

extern Int2 MaxAlistWidths (EnumFieldAssocPtr al)

{
  Int2  i;
  Int2  len;
  Int2  max;

  max = 0;
  if (al != NULL) {
    for (i = 1; al->name != NULL; i++, al++) {
      len = StringWidth (al->name);
      if (len > max) {
        max = len;
      }
    }
#ifdef WIN_MAC
    max += 26;
#endif
#ifdef WIN_MSWIN
    max += 26;
#endif
#ifdef WIN_MOTIF
    max += 48;
#endif
  }
  return max;
}

extern CharPtr WidestAlist (EnumFieldAssocPtr al)

{
  Int2     i;
  Int2     len;
  Int2     max;
  CharPtr  str;

  str = NULL;
  max = 0;
  if (al != NULL) {
    for (i = 1; al->name != NULL; i++, al++) {
      len = StringWidth (al->name);
      if (len > max) {
        max = len;
        str = al->name;
      }
    }
  }
  return str;
}

extern GrouP MultiLinePromptEx (GrouP prnt, CharPtr text, Int2 maxWidth, FonT font, Boolean stripSpaces)

{
  Boolean  cansplit;
  Char     ch;
  CharPtr  buf;
  GrouP    g;
  Int2     i;
  Int2     j;
  Int2     k;
  size_t   len;
  Int2     width;

  g = NULL;
  if (prnt != NULL) {
    g = HiddenGroup (prnt, 1, 0, NULL);
    len = MIN (StringLen (text), (size_t) 2048);
    buf = (CharPtr) MemNew (len + 20);
    if (buf != NULL) {
      if (font == NULL) {
        font = programFont;
      }
      StringNCpy_0 (buf, text, len + 2);
      i = 0;
      while (StringLen (buf + i) > 0) {
        SelectFont (font);
        width = 0;
        j = 0;
        if (stripSpaces) {
          while (buf [i + j] == ' ') {
            i++;
          }
        }
        cansplit = FALSE;
        ch = buf [i + j];
        while (ch != '\0' && ch != '\n' && ch != '\r' &&
               width <= maxWidth && j < 125) {
          width += CharWidth (ch);
          if (ch == ' ' || ch == '-') {
            cansplit = TRUE;
          }
          j++;
          ch = buf [i + j];
        }
        if (width > maxWidth) {
          if (cansplit) {
            ch = buf [i + j];
            while (j > 0 && ch != ' ' && ch != '-') {
              j--;
              ch = buf [i + j];
            }
          } else {
            j--;
            ch = buf [i + j];
          }
        } else if (j >= 125) {
          k = j;
          ch = buf [i + k];
          while (k > 0 && ch != ' ' && ch != '-') {
            k--;
            ch = buf [i + k];
          }
          if (k > 80) {
            j = k;
          }
          ch = buf [i + j];
        }
        if (ch == '\n' || ch == '\r') {
          buf [i + j] = '\0';
          Nlm_StaticPrompt (g, buf + i, 0, 0, font, 'l');
          i += j + 1;
        } else if (j > 0) {
          buf [i + j] = '\0';
          Nlm_StaticPrompt (g, buf + i, 0, 0, font, 'l');
          buf [i + j] = ch;
          i += j;
        } else {
          i++;
        }
      }
      MemFree (buf);
      SelectFont (systemFont);
    }
  }
  return g;
}

extern GrouP MultiLinePrompt (GrouP prnt, CharPtr text, Int2 maxWidth, FonT font)

{
  return MultiLinePromptEx (prnt, text, maxWidth, font, TRUE);
}

static ValNodePtr GetNthValNode (TagListPtr tlp, Int2 index)

{
  Int2        i;
  CharPtr     str;
  ValNodePtr  vnp;

  vnp = NULL;
  if (tlp != NULL) {
    if (tlp->vnp == NULL) {
      tlp->vnp = ValNodeNew (NULL);
    }
    vnp = tlp->vnp;
    while (vnp->next != NULL && index > 0) {
      vnp = vnp->next;
      index--;
    }
    while (index > 0) {
      vnp = ValNodeNew (vnp);
      index--;
    }
    if (vnp != NULL && vnp->data.ptrvalue == NULL) {
      if (tlp->cols > 0 && tlp->cols < 100) {
        str = (CharPtr) MemNew(sizeof (Char) * tlp->cols + 2);
        if (str != NULL) {
          for (i = 0; i < tlp->cols; i++) {
            str [i] = '\t';
          }
          str [tlp->cols] = '\0';
        }
        vnp->data.ptrvalue = str;
      }
    }
  }
  return vnp;
}

static CharPtr ReplaceColumn (CharPtr source, CharPtr str, Int2 col)

{
  Char     ch;
  CharPtr  dst;
  CharPtr  ptr;
  CharPtr  tmp;

  if (col < 0) return NULL;

  tmp = (CharPtr) MemNew(StringLen (source) + StringLen (str) + 3);
  if (tmp == NULL) return NULL;
  dst = tmp;

  ptr = source;
  ch = *ptr;
  while (col > 0 && ch != '\n' && ch != '\0') {
    while (ch != '\t' && ch != '\n' && ch != '\0') {
      *dst = ch;
      dst++;
      ptr++;
      ch = *ptr;
    }
    if (ch == '\t') {
      *dst = ch;
      dst++;
      ptr++;
      ch = *ptr;
    }
    col--;
  }

  if (str != NULL) {
    ch = *str;
    while (ch != '\t' && ch != '\n' && ch != '\0') {
      *dst = ch;
      dst++;
      str++;
      ch = *str;
    }
  }

  ch = *ptr;
  while (ch != '\t' && ch != '\n' && ch != '\0') {
    ptr++;
    ch = *ptr;
  }

  while (ch != '\0') {
    *dst = ch;
    dst++;
    ptr++;
    ch = *ptr;
  }

  return tmp;
}

extern CharPtr ExtractTagListColumn (CharPtr source, Int2 col)

{
  Char     ch;
  size_t   count;
  CharPtr  ptr;
  CharPtr  str;

  if (source == NULL || source [0] == '\0' || col < 0) return NULL;

  ptr = source;
  ch = *ptr;
  while (col > 0 && ch != '\n' && ch != '\0') {
    while (ch != '\t' && ch != '\n' && ch != '\0') {
      ptr++;
      ch = *ptr;
    }
    if (ch == '\t') {
      ptr++;
      ch = *ptr;
    }
    col--;
  }

  count = 0;
  ch = ptr [count];
  while (ch != '\t' && ch != '\n' && ch != '\0') {
    count++;
    ch = ptr [count];
  }
  str = (CharPtr) MemNew(count + 1);
  if (str != NULL) {
    MemCpy (str, ptr, count);
  }
  return str;
}

static Handle GetTagListControl (TagListPtr tlp, Int2 i, Int2 j)

{
  if (tlp == NULL) return NULL;

  return tlp->control [i * MAX_TAGLIST_COLS + j];
}

static void RedrawTagList (DialoG d)

{
  Int2        i;
  Int2        j;
  Int2        num;
  CharPtr     ptr;
  TagListPtr  tlp;
  Int2        val;
  ValNodePtr  vnp;

  tlp = (TagListPtr) GetObjectExtra (d);
  if (tlp != NULL) {
    val = GetValue (tlp->bar);
    for (i = 0, vnp = tlp->vnp; i < val; i++, vnp = vnp->next) {
    }
    for (i = 0; i < tlp->rows && vnp != NULL; i++, vnp = vnp->next) {
      for (j = 0; j < tlp->cols; j++) {
        ptr = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, j);
        switch (tlp->types [j]) {
          case TAGLIST_TEXT :
          case TAGLIST_PROMPT :
            SafeSetTitle (GetTagListControl (tlp, i, j), ptr);
            break;
          case TAGLIST_POPUP :
          case TAGLIST_LIST :
            if (tlp->alists != NULL) {
              if (StrToInt (ptr, &num)) {
                SetEnumPopup ((PopuP) GetTagListControl (tlp, i, j), tlp->alists [j], (UIEnum) num);
              } else {
                SetEnumPopup ((PopuP) GetTagListControl (tlp, i, j), tlp->alists [j], (UIEnum) 0);
              }
            } else {
              SafeSetValue (GetTagListControl (tlp, i, j), 0);
            }
            break;
          default :
            break;
        }
        MemFree (ptr);
      }
    }
    for (; i < tlp->rows; i++) {
      for (j = 0; j < tlp->cols; j++) {
        switch (tlp->types [j]) {
          case TAGLIST_TEXT :
          case TAGLIST_PROMPT :
            SafeSetTitle (GetTagListControl (tlp, i, j), "");
            break;
          case TAGLIST_POPUP :
          case TAGLIST_LIST :
            if (tlp->alists != NULL) {
              SetEnumPopup ((PopuP) GetTagListControl (tlp, i, j), tlp->alists [j], (UIEnum) 0);
            } else {
              SafeSetValue (GetTagListControl (tlp, i, j), 0);
            }
            break;
          default :
            break;
        }
      }
    }
  }
}

static void CheckExtendTag (TagListPtr tlp)

{
  Boolean     extend;
  Int2        j;
  UIEnum      num;
  Int2        val;
  ValNodePtr  vnp;

  if (tlp != NULL) {
    if (tlp->noExtend) return;
    val = GetValue (tlp->bar);
    if (val == tlp->max) {
      extend = FALSE;
      for (j = 0; j < tlp->cols; j++) {
        switch (tlp->types [j]) {
          case TAGLIST_TEXT :
          /*case TAGLIST_PROMPT :*/
            if ( !TextHasNoText((TexT)GetTagListControl(tlp, tlp->rows-1, j)) )
              extend = TRUE;
            break;
          case TAGLIST_POPUP :
          case TAGLIST_LIST :
            if (tlp->alists != NULL &&
                GetEnumPopup ((PopuP)GetTagListControl (tlp, tlp->rows - 1, j),
                              tlp->alists [j], &num) &&
                (Int2) num > 0) {
              extend = TRUE;
            }
            break;
          default :
            break;
        }
      }
      if (extend) {
        (tlp->max)++;
        CorrectBarMax (tlp->bar, tlp->max);
        CorrectBarPage (tlp->bar, tlp->rows - 1, tlp->rows - 1);
        vnp = ValNodeNew (tlp->vnp);
        if (tlp->vnp == NULL) {
          tlp->vnp = vnp;
        }
      }
    }
  }
}

static void ScrollTagProc (BaR b, GraphiC g, Int2 _new, Int2 _old)

{
  TagListPtr  tlp;

  tlp = (TagListPtr) GetObjectExtra (b);
  if (tlp != NULL) {
    SendMessageToDialog (tlp->dialog, VIB_MSG_REDRAW);
    Update ();
    CheckExtendTag (tlp);
  }
}

static void PopupTagProc (PopuP p)

{
  Int2        i;
  Int2        j;
  UIEnum      num;
  CharPtr     ptr;
  Char        str [16];
  TagListPtr  tlp;
  Int2        val;
  ValNodePtr  vnp;

  tlp = (TagListPtr) GetObjectExtra (p);
  if (tlp != NULL) {
    val = GetValue (tlp->bar);
    for (i = 0; i < tlp->rows; i++) {
      for (j = 0; j < tlp->cols; j++) {
        if (p == GetTagListControl (tlp, i, j) && tlp->types [j] == TAGLIST_POPUP) {
          vnp = GetNthValNode (tlp, i + val);
          if (vnp != NULL) {
            if (tlp->alists != NULL && GetEnumPopup (p, tlp->alists [j], &num)) {
              IntToStr ((Int2) num, str, 0, sizeof (str));
              ptr = ReplaceColumn ((CharPtr)vnp->data.ptrvalue, str, j);
              vnp->data.ptrvalue = MemFree (vnp->data.ptrvalue);
              vnp->data.ptrvalue = ptr;
            }
          }
        }
      }
    }
    CheckExtendTag (tlp);
  }
}

static void ListTagProc (LisT l)

{
  Int2        i;
  Int2        j;
  UIEnum      num;
  CharPtr     ptr;
  Char        str [16];
  TagListPtr  tlp;
  Int2        val;
  ValNodePtr  vnp;

  tlp = (TagListPtr) GetObjectExtra (l);
  if (tlp != NULL) {
    val = GetValue (tlp->bar);
    for (i = 0; i < tlp->rows; i++) {
      for (j = 0; j < tlp->cols; j++) {
        if (l == GetTagListControl (tlp, i, j) && tlp->types [j] == TAGLIST_LIST) {
          vnp = GetNthValNode (tlp, i + val);
          if (vnp != NULL) {
            if (tlp->alists != NULL && GetEnumPopup ((PopuP) l, tlp->alists [j], &num)) {
              IntToStr ((Int2) num, str, 0, sizeof (str));
              ptr = ReplaceColumn ((CharPtr)vnp->data.ptrvalue, str, j);
              vnp->data.ptrvalue = MemFree (vnp->data.ptrvalue);
              vnp->data.ptrvalue = ptr;
            }
          }
        }
      }
    }
    CheckExtendTag (tlp);
  }
}

static void TextTagProc (TexT t)

{
  Int2        i;
  Int2        j;
  CharPtr     ptr;
  CharPtr     str;
  TagListPtr  tlp;
  Int2        val;
  ValNodePtr  vnp;

  tlp = (TagListPtr) GetObjectExtra (t);
  if (tlp != NULL) {
    val = GetValue (tlp->bar);
    for (i = 0; i < tlp->rows; i++) {
      for (j = 0; j < tlp->cols; j++) {
        if (t == GetTagListControl (tlp, i, j) && tlp->types [j] == TAGLIST_TEXT) {
          vnp = GetNthValNode (tlp, i + val);
          if (vnp != NULL) {
            str = SaveStringFromText (t);
            ptr = ReplaceColumn ((CharPtr)vnp->data.ptrvalue, str, j);
            vnp->data.ptrvalue = MemFree (vnp->data.ptrvalue);
            vnp->data.ptrvalue = ptr;
            MemFree (str);
          }
        }
      }
    }
    CheckExtendTag (tlp);
  }
}

static TexT FindNextText (TagListPtr tlp, Int2 i, Int2 j)

{
  TexT  nxt;
  Int2  val;

  nxt = NULL;
  if (tlp != NULL) {
    j++;
    while (nxt == NULL && j < tlp->cols) {
      if (tlp->types [j] == TAGLIST_TEXT) {
        nxt = (TexT)GetTagListControl (tlp, i, j);
      }
      j++;
    }
    if (nxt == NULL) {
      if (i < tlp->rows - 1) {
        i++;
        for (j = 0; nxt == NULL && j < tlp->cols; j++) {
          if (tlp->types [j] == TAGLIST_TEXT) {
            nxt = (TexT)GetTagListControl (tlp, i, j);
          }
        }
      } else {
        for (j = 0; nxt == NULL && j < tlp->cols; j++) {
          if (tlp->types [j] == TAGLIST_TEXT) {
            nxt = (TexT)GetTagListControl (tlp, i, j);
          }
        }
        val = GetValue (tlp->bar);
        if (val < tlp->max) {
          SafeSetValue (tlp->bar, val + 1);
        } else {
          CheckExtendTag (tlp);
          SafeSetValue (tlp->bar, tlp->max);
        }
      }
    }
  }
  return nxt;
}

static void TagTabProc (TexT t)

{
  Int2        i;
  Int2        j;
  TexT        nxt;
  TagListPtr  tlp;

  tlp = (TagListPtr) GetObjectExtra (t);
  if (tlp != NULL) {
    for (i = 0; i < tlp->rows; i++) {
      for (j = 0; j < tlp->cols; j++) {
        if (t == GetTagListControl (tlp, i, j) && tlp->types [j] == TAGLIST_TEXT) {
          nxt = FindNextText (tlp, i, j);
          Select (nxt);
        }
      }
    }
  }
}

static void TagRtnProc (TexT t)

{
  Int2        i;
  Int2        j;
  ValNodePtr  last;
  CharPtr     str;
  TagListPtr  tlp;
  Int2        val;
  ValNodePtr  vnp;

  tlp = (TagListPtr) GetObjectExtra (t);
  if (tlp != NULL) {
    if (tlp->noExtend) return;
    val = GetValue (tlp->bar);
    for (i = 0; i < tlp->rows; i++) {
      for (j = 0; j < tlp->cols; j++) {
        if (t == GetTagListControl (tlp, i, j) && tlp->types [j] == TAGLIST_TEXT) {
          vnp = ValNodeNew (NULL);
          if (vnp != NULL) {
            i += val + 1;
            if (i > 0) {
              last = NULL;
              if (i > 1)  {
                last = GetNthValNode (tlp, i - 1);
              } else {
                last = tlp->vnp;
              }
              if (last != NULL) {
                vnp->next = last->next;
                last->next = vnp;
              }
            } else {
              vnp->next = tlp->vnp;
              tlp->vnp = vnp;
            }
            if (tlp->cols > 0 && tlp->cols < 100) {
              str = (CharPtr)MemNew (sizeof (Char) * tlp->cols + 2);
              if (str != NULL) {
                for (j = 0; j < tlp->cols; j++) {
                  str [j] = '\t';
                }
                str [tlp->cols] = '\0';
              }
              vnp->data.ptrvalue = str;
              SendMessageToDialog (tlp->dialog, VIB_MSG_REDRAW);
              Update ();
              CheckExtendTag (tlp);
            }
          }
          return;
        }
      }
    }
  }
}

static UIEnum GetDefaultEnumValue (EnumFieldAssocPtr alist)
{
  EnumFieldAssocPtr eap;
  
  if (alist == NULL)
  {
    return 0;
  }
  
  eap = alist;
  while (eap != NULL && eap->name != NULL)
  {
    if (eap->value == 0)
    {
      return 0;
    }
    eap++;
  }
  return alist->value;
}

static void ResetTagList (DialoG d)

{
  Int2        i;
  Int2        j;
  TagListPtr  tlp;

  tlp = (TagListPtr) GetObjectExtra (d);
  if (tlp != NULL) {
    for (i = 0; i < tlp->rows; i++) {
      for (j = 0; j < tlp->cols; j++) {
        switch (tlp->types [j]) {
          case TAGLIST_TEXT :
          case TAGLIST_PROMPT :
            SafeSetTitle (GetTagListControl (tlp, i, j), "");
            break;
          case TAGLIST_POPUP :
          case TAGLIST_LIST :
            if (tlp->alists != NULL) {
              SetEnumPopup ((PopuP) GetTagListControl (tlp, i, j), tlp->alists [j],
                                                       GetDefaultEnumValue (tlp->alists [j]));
            } else {
              SafeSetValue (GetTagListControl (tlp, i, j), 0);
            }
            break;
          default :
            break;
        }
      }
    }
    Reset (tlp->bar);
    CorrectBarPage (tlp->bar, tlp->rows - 1, tlp->rows - 1);
    tlp->max = 0;
    tlp->vnp = ValNodeFreeData (tlp->vnp);
  }
}

static void CleanupTagList (GraphiC g, VoidPtr data)

{
  TagListPtr  tlp;

  tlp = (TagListPtr) data;
  if (tlp != NULL) {
    ValNodeFreeData (tlp->vnp);
    MemFree (tlp->types);
  }
  MemFree (data);
}

static void TagListMessage (DialoG d, Int2 mssg)

{
  Int2        j;
  TagListPtr  tlp;

  tlp = (TagListPtr) GetObjectExtra (d);
  if (tlp != NULL) {
    if (mssg == VIB_MSG_ENTER) {
      for (j = 0; j < tlp->cols; j++) {
        if (tlp->types [j] == TAGLIST_TEXT) {
          Select (GetTagListControl (tlp, 0, j));
          return;
        }
      }
    } else if (mssg == VIB_MSG_RESET) {
      ResetTagList (d);
    } else if (mssg == VIB_MSG_REDRAW) {
      RedrawTagList (d);
    }
  }
}

extern DialoG CreateTagListDialogEx (GrouP h, Uint2 rows, Uint2 cols,
                                     Int2 spacing, Uint2Ptr types,
                                     Uint2Ptr textWidths, EnumFieldAssocPtr PNTR alists,
                                     Boolean useBar, Boolean noExtend,
                                     ToDialogFunc tofunc, FromDialogFunc fromfunc);

extern DialoG CreateTagListDialogEx (GrouP h, Uint2 rows, Uint2 cols,
                                     Int2 spacing, Uint2Ptr types,
                                     Uint2Ptr textWidths, EnumFieldAssocPtr PNTR alists,
                                     Boolean useBar, Boolean noExtend,
                                     ToDialogFunc tofunc, FromDialogFunc fromfunc)

{
  EnumFieldAssocPtr  al;
  Int2               col;
  GrouP              g;
  Int2               i;
  Int2               j;
  Int2               k;
  Int2               len;
  LisT               lst;
  GrouP              p;
  GrouP              s;
  TagListPtr         tlp;
  Int2               wid;

  if (cols < 1 || types == NULL) return NULL;

  p = HiddenGroup (h, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  tlp = (TagListPtr) MemNew (sizeof (TagList));
  if (tlp != NULL) {

    SetObjectExtra (p, tlp, CleanupTagList);
    tlp->dialog = (DialoG) p;
    tlp->todialog = tofunc;
    tlp->fromdialog = fromfunc;
    tlp->dialogmessage = TagListMessage;
    tlp->testdialog = NULL;

    tlp->rows = (Int2) rows;
    tlp->cols = (Int2) cols;
    tlp->types = (Uint2Ptr) MemNew (sizeof (Int2) * MAX_TAGLIST_COLS);
    for (j = 0; j < (Int2) cols && j < MAX_TAGLIST_COLS; j++) {
      tlp->types [j] = types [j];
    }
    tlp->alists = alists;
    tlp->noExtend = noExtend;

    s = HiddenGroup (p, 2, 0, NULL);

    col = (Int2) cols;
    g = HiddenGroup (s, -col, 0, NULL);
    SetGroupSpacing (g, spacing, spacing);
    for (i = 0; i < tlp->rows; i++) {
      for (j = 0; j < tlp->cols; j++) {
        switch (types [j]) {
          case TAGLIST_TEXT :
            if (textWidths != NULL) {
              tlp->control [i * MAX_TAGLIST_COLS + j] =
                 (Handle) SpecialText (g, "", textWidths [j], TextTagProc,
                                       TagTabProc, TagRtnProc);
              SetObjectExtra (GetTagListControl (tlp, i, j), tlp, NULL);
            }
            break;
          case TAGLIST_PROMPT :
            if (textWidths != NULL) {
              tlp->control [i * MAX_TAGLIST_COLS + j] =
                 (Handle) StaticPrompt (g, "", stdCharWidth * textWidths [j] + 2,
                                        MAX (dialogTextHeight, popupMenuHeight),
                                        systemFont, 'l');
              SetObjectExtra (GetTagListControl (tlp, i, j), tlp, NULL);
            }
            break;
          case TAGLIST_POPUP :
            if (alists != NULL) {
              tlp->control [i * MAX_TAGLIST_COLS + j] = (Handle) PopupList (g, TRUE, PopupTagProc);
              SetObjectExtra (GetTagListControl (tlp, i, j), tlp, NULL);
              InitEnumPopup ((PopuP) GetTagListControl (tlp, i, j), alists [j], NULL);
            }
            break;
          case TAGLIST_LIST :
            if (alists != NULL) {
              wid = 0;
              al = alists [j];
              if (al != NULL) {
                for (k = 1; al->name != NULL; k++, al++) {
                  len = StringLen (al->name);
                  if (len > wid) {
                    wid = len;
                  }
                }
              }
              if (wid < 1) {
                wid = 7;
              }
              wid++;
              tlp->control [i * MAX_TAGLIST_COLS + j] = (Handle) SingleList (g, wid, 1, ListTagProc);
              SetObjectExtra (GetTagListControl (tlp, i, j), tlp, NULL);
              al = alists [j];
              if (al == NULL) {
                Message(MSG_POSTERR, "in InitEnumList");
              } else {
                lst = (LisT) GetTagListControl (tlp, i, j);
                for (k = 1; al->name != NULL; k++, al++) {
                  ListItem (lst, al->name);
                }
              }
            }
            break;
          default :
            break;
        }
      }
    }

    if (useBar) {
      tlp->bar = ScrollBar (s, 0, rows, ScrollTagProc);
      SetObjectExtra (tlp->bar, tlp, NULL);
      CorrectBarPage (tlp->bar, tlp->rows - 1, tlp->rows - 1);
    }
    tlp->max = 0;
    tlp->vnp = NULL;
    AlignObjects (ALIGN_LOWER, (HANDLE) g, (HANDLE) tlp->bar, NULL);
  }

  return (DialoG) p;
}

extern DialoG CreateTagListDialog (GrouP h, Uint2 rows, Uint2 cols, Int2 spacing,
                                   Uint2Ptr types, Uint2Ptr textWidths,
                                   EnumFieldAssocPtr PNTR alists,
                                   ToDialogFunc tofunc, FromDialogFunc fromfunc)

{
  return CreateTagListDialogEx (h, rows, cols, spacing, types, textWidths,
                                alists, TRUE, FALSE, tofunc, fromfunc);
}

extern void UpdateTagListPopupChoices (DialoG d, Int4 column);

extern void UpdateTagListPopupChoices (DialoG d, Int4 column)
{
  Int4              i, j;
  TagListPtr        tlp;
  PopuP             p;

  tlp = (TagListPtr) GetObjectExtra (d);
  if (tlp == NULL || tlp->alists == NULL)
  {
    return;
  }
  
  for (i = 0; i < tlp->rows; i++) {
    for (j = 0; j < tlp->cols; j++) {
      if (j != column && column > 0)
      {
        continue;
      }
      if (tlp->types [j] != TAGLIST_POPUP)
      {
        continue;
      }
      p = (PopuP) tlp->control [i * MAX_TAGLIST_COLS + j];
      Reset (p);
      InitEnumPopup (p, tlp->alists[j], NULL);
    }
  }
}

extern void JustInvalObject (Nlm_Handle a)

{
  RecT  r;

  if (a != NULL) {
    ObjectRect ((Nlm_GraphiC) a, &r);
    InsetRect (&r, -1, -1);
    InvalRect (&r);
  }
}

#define MAX_TABS  32

typedef struct foldertab {
  DIALOG_MESSAGE_BLOCK
  CharPtr         titles [MAX_TABS];
  PaneL           tabs [MAX_TABS];
  FonT            font;
  Int2            horizMargin;
  Int2            vertMargin;
  Int2            spaceBtwn;
  Int2            cornerTaper;
  Int2            endExt;
  Int2            currentPage;
  Int2            oldPage;
  Int2            numPages;
  TabActnProc     changeView;
  PnlActnProc     flipProc;
  PnlActnProc     changeProc;
  Pointer         userdata;
} FolderTabs, PNTR FolderTabsPtr;

static void DrawFolderTabs (PaneL p)

{
  FolderTabsPtr  ftp;
  Int2           i;
  RecT           r;
  Int2           wid;

  ftp = (FolderTabsPtr) GetObjectExtra (p);
  if (ftp != NULL) {
    i = 0;
    while (i < MAX_TABS && ftp->tabs [i] != NULL && ftp->tabs [i] != p) {
      i++;
    }
    if (i < MAX_TABS && ftp->tabs [i] != NULL && ftp->tabs [i] == p) {
      SelectFont (ftp->font);
      ObjectRect (p, &r);
      MoveTo (r.left + ftp->spaceBtwn, r.bottom - 1);
      LineTo (r.left + ftp->spaceBtwn, r.top + ftp->cornerTaper);
      LineTo (r.left + ftp->spaceBtwn + ftp->cornerTaper, r.top);
      LineTo (r.right - ftp->spaceBtwn - ftp->cornerTaper, r.top);
      LineTo (r.right - ftp->spaceBtwn, r.top + ftp->cornerTaper);
      LineTo (r.right - ftp->spaceBtwn, r.bottom - 1);
      if (ftp->currentPage == i) {
        MoveTo (r.left, r.bottom - 1);
        LineTo (r.left + ftp->spaceBtwn, r.bottom - 1);
        MoveTo (r.right - ftp->spaceBtwn, r.bottom - 1);
        LineTo (r.right, r.bottom - 1);
      } else {
        MoveTo (r.left, r.bottom - 1);
        LineTo (r.right, r.bottom - 1);
      }
      ObjectRect (p, &r);
      wid = StringWidth (ftp->titles [i]) + 2;
      /*
      MoveTo (r.left + ftp->horizMargin, r.top + ftp->vertMargin + Ascent ());
      */
      MoveTo ((r.left + r.right - wid) / 2, r.top + ftp->vertMargin + Ascent ());
      PaintText ("%s", ftp->titles [i]);
      SelectFont (systemFont);
    }
  }
}

static void DrawBottomLineLeft (PaneL p)

{
  FolderTabsPtr  ftp;
  RecT           r;

  ftp = (FolderTabsPtr) GetObjectExtra (p);
  if (ftp != NULL) {
    ObjectRect (p, &r);
    MoveTo (r.right - ftp->endExt, r.bottom - 1);
    LineTo (r.right, r.bottom - 1);
  }
}

static void DrawBottomLineRight (PaneL p)

{
  FolderTabsPtr  ftp;
  RecT           r;

  ftp = (FolderTabsPtr) GetObjectExtra (p);
  if (ftp != NULL) {
    ObjectRect (p, &r);
    MoveTo (r.left, r.bottom - 1);
    LineTo (r.left + ftp->endExt, r.bottom - 1);
  }
}

static Boolean  insideTab;
static Boolean  newChoice;

static void FlipTabFrame (PaneL p)

{
  FolderTabsPtr  ftp;
  RecT           r;

  ftp = (FolderTabsPtr) GetObjectExtra (p);
  if (ftp != NULL) {
    ObjectRect (p, &r);
    InvertMode ();
    MoveTo (r.left + ftp->spaceBtwn + 1, r.bottom - 2);
    LineTo (r.left + ftp->spaceBtwn + 1, r.top + ftp->cornerTaper + 1);
    MoveTo (r.left + ftp->spaceBtwn + 1, r.top + ftp->cornerTaper);
    LineTo (r.left + ftp->spaceBtwn + ftp->cornerTaper, r.top + 1);
    MoveTo (r.left + ftp->spaceBtwn + ftp->cornerTaper + 1, r.top + 1);
    LineTo (r.right - ftp->spaceBtwn - ftp->cornerTaper - 1, r.top + 1);
    MoveTo (r.right - ftp->spaceBtwn - ftp->cornerTaper, r.top + 1);
    LineTo (r.right - ftp->spaceBtwn - 1, r.top + ftp->cornerTaper);
    MoveTo (r.right - ftp->spaceBtwn - 1, r.top + ftp->cornerTaper + 1);
    LineTo (r.right - ftp->spaceBtwn - 1, r.bottom - 2);
    MoveTo (r.right - ftp->spaceBtwn - 2, r.bottom - 2);
    LineTo (r.left + ftp->spaceBtwn + 2, r.bottom - 2);
    CopyMode ();
  }
}

static void ClickFolderTabs (PaneL p, PoinT pt)

{
  FolderTabsPtr  ftp;
  Int2           i;

  insideTab = TRUE;
  newChoice = TRUE;
  ftp = (FolderTabsPtr) GetObjectExtra (p);
  if (ftp != NULL) {
    i = 0;
    while (ftp->tabs [i] != NULL && ftp->tabs [i] != p) {
      i++;
    }
    if (ftp->currentPage == i) {
      newChoice = FALSE;
    } else {
      if (ftp->tabs [i] != NULL && ftp->tabs [i] == p) {
        if (ftp->flipProc != NULL) {
          ftp->flipProc (p);
        }
      }
    }
  }
}

static void DragFolderTabs (PaneL p, PoinT pt)

{
  FolderTabsPtr  ftp;
  Int2        i;
  RecT        r;

  ftp = (FolderTabsPtr) GetObjectExtra (p);
  if (ftp != NULL && newChoice) {
    i = 0;
    while (ftp->tabs [i] != NULL && ftp->tabs [i] != p) {
      i++;
    }
    if (ftp->tabs [i] != NULL && ftp->tabs [i] == p) {
      ObjectRect (p, &r);
      InsetRect (&r, 1, 1);
      if (PtInRect (pt, &r)) {
        if (! insideTab) {
          if (ftp->flipProc != NULL) {
            ftp->flipProc (p);
          }
          insideTab = TRUE;
        }
      } else {
        if (insideTab) {
          if (ftp->flipProc != NULL) {
            ftp->flipProc (p);
          }
          insideTab = FALSE;
        }
      }
    }
  }
}

static void FolderTabChanging (PaneL p)

{
  FolderTabsPtr  ftp;
  Int2           old;
  RecT           r;


  ftp = (FolderTabsPtr) GetObjectExtra (p);
  if (ftp != NULL) {
    ObjectRect (p, &r);
    r.top = r.bottom - 1;
    InsetRect (&r, -1, -1);
    InvalRect (&r);
    old = ftp->oldPage;
    p = ftp->tabs [old];
    if (p != NULL) {
      Select (p);
      ObjectRect (p, &r);
      r.top = r.bottom - 1;
      InsetRect (&r, -1, -1);
      InvalRect (&r);
    }
  }
}

static void ReleaseFolderTabs (PaneL p, PoinT pt)

{
  FolderTabsPtr  ftp;
  Int2           i;
  RecT           r;

  ftp = (FolderTabsPtr) GetObjectExtra (p);
  if (ftp != NULL && newChoice) {
    i = 0;
    while (ftp->tabs [i] != NULL && ftp->tabs [i] != p) {
      i++;
    }
    if (ftp->tabs [i] != NULL && ftp->tabs [i] == p) {
      ObjectRect (p, &r);
      InsetRect (&r, 1, 1);
      if (PtInRect (pt, &r)) {
        if (insideTab) {
          if (ftp->flipProc != NULL) {
            ftp->flipProc (p);
          }
          insideTab = TRUE;
        }
      } else {
        if (insideTab) {
          if (ftp->flipProc != NULL) {
            ftp->flipProc (p);
          }
          insideTab = FALSE;
        }
      }
      if (PtInRect (pt, &r)) {
        ftp->oldPage = ftp->currentPage;
        ftp->currentPage = i;
        ResetClip ();
        if (ftp->changeProc != NULL) {
          ftp->changeProc (p);
        }
        Update ();
        if (ftp->changeView != NULL) {
          ftp->changeView (ftp->userdata, i, ftp->oldPage);
        }
      }
    }
  }
  insideTab = TRUE;
  newChoice = TRUE;
}

extern Nlm_GphPrcsPtr  Nlm_folderTabProcs;
extern WindoW Nlm_SavePortIfNeeded (GraphiC a, Boolean savePort);
extern void Nlm_SetFolderTabSubclass (Nlm_GrouP g);
extern void Nlm_SetFolderTabValue (GraphiC a, Int2 value, Boolean savePort);
extern void Nlm_SetFolderTabValue (GraphiC a, Int2 value, Boolean savePort)

{
  FolderTabsPtr  ftp;
  WindoW         tempPort;

  ftp = (FolderTabsPtr) GetObjectExtra (a);
  if (ftp != NULL) {
    if (ftp->currentPage != value && value >= 0 && value < ftp->numPages) {
      tempPort = Nlm_SavePortIfNeeded (a, savePort);
      a = (GraphiC) ftp->tabs [value];
      Select (a);
      ftp->oldPage = ftp->currentPage;
      ftp->currentPage = value;
      ResetClip ();
      if (ftp->changeProc != NULL) {
        ftp->changeProc ((PaneL) a);
      }
      Update ();
      if (ftp->changeView != NULL) {
        ftp->changeView (ftp->userdata, value, ftp->oldPage);
      }
      RestorePort (tempPort);
    }
  }
}

static void CleanupFolderTabs (GraphiC g, VoidPtr data)

{
  FolderTabsPtr  ftp;
  Int2           i;

  ftp = (FolderTabsPtr) data;
  if (ftp != NULL) {
    for (i = 0; i < MAX_TABS; i++) {
      ftp->titles [i] = (CharPtr) MemFree(ftp->titles [i]);
    }
  }
  MemFree (data);
}

extern DialoG CreateFolderTabs (GrouP h, CharPtr PNTR titles, Int2 initPage,
                                Int2 maxPerLine, Int2 indentNextLine,
                                FonT font, Int2 horizMargin, Int2 vertMargin,
                                Int2 spaceBtwn, Int2 cornerTaper, Int2 endExt,
                                TabActnProc changeView, Pointer userdata)

{
  FolderTabsPtr  ftp;
  Int2           grpWid;
  Int2           i;
  Int2           indent;
  Int2           j;
  Int2           lineHeight;
  GrouP          p;
  RecT           r;
  Handle         tabs [MAX_TABS * 2];
  Int2           total;
  Int2           wid;
  GrouP          x;

  p = HiddenGroup (h, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  ftp = (FolderTabsPtr) MemNew (sizeof (FolderTabs));
  if (ftp != NULL) {

    SetObjectExtra (p, ftp, CleanupFolderTabs);
    ftp->dialog = (DialoG) p;
    ftp->todialog = NULL;
    ftp->fromdialog = NULL;
    ftp->testdialog = NULL;

    ftp->currentPage = initPage;
    ftp->numPages = 0;
    for (i = 0; i < MAX_TABS; i++) {
      ftp->titles [i] = NULL;
      ftp->tabs [i] = NULL;
    }
    for (i = 0; i < MAX_TABS && titles [i] != NULL; i++) {
      ftp->titles [i] = StringSave (titles [i]);
    }
    for (j = 0; j < MAX_TABS * 2; j++) {
      tabs [j] = NULL;
    }

    ftp->font = font;
    ftp->horizMargin = horizMargin;
    ftp->vertMargin = vertMargin;
    ftp->spaceBtwn = spaceBtwn;
    ftp->cornerTaper = cornerTaper;
    ftp->endExt = endExt;
    ftp->flipProc = FlipTabFrame;
    ftp->changeProc = FolderTabChanging;

    if (maxPerLine == 0) {
      grpWid = 8;
    } else {
      grpWid = ABS (maxPerLine);
      if (grpWid > 8) {
        grpWid = 8;
      }
    }

    if (maxPerLine < 0) {
      x = HiddenGroup (p, -grpWid - 2, 0, NULL);
    } else {
      x = HiddenGroup (p, grpWid + 2, 0, NULL);
    }
    SetGroupSpacing (x, 0, 2);
    SelectFont (font);
    lineHeight = LineHeight ();
    SelectFont (systemFont);
    for (i = 0, j = 0; ftp->titles [i] != NULL && i < MAX_TABS && j < MAX_TABS * 2; i++) {
      if ((i % grpWid) == 0) {
        tabs [j] = SimplePanel (x, ftp->endExt, lineHeight * 1 + 2 * ftp->vertMargin, DrawBottomLineLeft);
        SetObjectExtra (tabs [j], ftp, NULL);
        j++;
      }
      SelectFont (font);
      wid = StringWidth (ftp->titles [i]);
      SelectFont (systemFont);
      ftp->tabs [i] = SimplePanel (x, wid + 2 + 2 * ftp->horizMargin,
                                   lineHeight * 1 + 2 * ftp->vertMargin,
                                   DrawFolderTabs);
      tabs [j] = ftp->tabs [i];
      SetObjectExtra (ftp->tabs [i], ftp, NULL);
      SetPanelClick (ftp->tabs [i], ClickFolderTabs, DragFolderTabs,
                     NULL, ReleaseFolderTabs);
      (ftp->numPages)++;
      j++;
      if ((i % grpWid) == (grpWid - 1) || ftp->titles [i + 1] == NULL || i + 1 >= MAX_TABS) {
        tabs [j] = SimplePanel (x, ftp->endExt, lineHeight * 1 + 2 * ftp->vertMargin, DrawBottomLineRight);
        SetObjectExtra (tabs [j], ftp, NULL);
        j++;
      }
    }
    ftp->changeView = changeView;
    ftp->userdata = userdata;
    if (indentNextLine > 0) {
      total = j;
      indent = 0;
      grpWid += 2;
      for (j = 0; j < total; j++) {
        if (indent > 0) {
          GetPosition (tabs [j], &r);
          OffsetRect (&r, indent, 0);
          SetPosition (tabs [j], &r);
          AdjustPrnt (tabs [j], &r, FALSE);
        }
        if ((j % grpWid) == (grpWid - 1)) {
          indent += indentNextLine;
        }
      }
    }
    Nlm_SetFolderTabSubclass ((GrouP) p);
  }

  return (DialoG) p;
}

static void DrawTextToolBar (PaneL p)

{
  FolderTabsPtr  ftp;
  Int2           i;
  RecT           r;
  Int2           wid;

  ftp = (FolderTabsPtr) GetObjectExtra (p);
  if (ftp != NULL) {
    i = 0;
    while (i < MAX_TABS && ftp->tabs [i] != NULL && ftp->tabs [i] != p) {
      i++;
    }
    if (i < MAX_TABS && ftp->tabs [i] != NULL && ftp->tabs [i] == p) {
      SelectFont (ftp->font);
      ObjectRect (p, &r);
      InsetRect (&r, ftp->spaceBtwn + 2, ftp->spaceBtwn + 2);
      FrameRect (&r);
      if (ftp->currentPage == i) {
        InsetRect (&r, -1, -1);
        FrameRect (&r);
      }
      ObjectRect (p, &r);
      wid = StringWidth (ftp->titles [i]) + 2;
      MoveTo ((r.left + r.right - wid) / 2, r.top + ftp->vertMargin + Ascent ());
      PaintText ("%s", ftp->titles [i]);
      SelectFont (systemFont);
    }
  }
}

static void FlipTextToolBarFrame (PaneL p)

{
  FolderTabsPtr  ftp;
  RecT           r;

  ftp = (FolderTabsPtr) GetObjectExtra (p);
  if (ftp != NULL) {
    ObjectRect (p, &r);
    InvertMode ();
    InsetRect (&r, ftp->spaceBtwn + 3, ftp->spaceBtwn + 3);
    FrameRect (&r);
    CopyMode ();
  }
}

static void TextToolBarChanging (PaneL p)

{
  FolderTabsPtr  ftp;
  Int2           old;
  RecT           r;


  ftp = (FolderTabsPtr) GetObjectExtra (p);
  if (ftp != NULL) {
    ObjectRect (p, &r);
    InsetRect (&r, -1, -1);
    InvalRect (&r);
    old = ftp->oldPage;
    p = ftp->tabs [old];
    if (p != NULL) {
      Select (p);
      ObjectRect (p, &r);
      InsetRect (&r, -1, -1);
      InvalRect (&r);
    }
  }
}

extern DialoG CreateTextTabs (GrouP h, CharPtr PNTR titles, Int2 initPage,
                              Int2 maxPerLine, Int2 indentNextLine,
                              FonT font, Int2 horizMargin, Int2 vertMargin,
                              Int2 spaceBtwn, TabActnProc changeView,
                              Pointer userdata)

{
  FolderTabsPtr  ftp;
  Int2           grpWid;
  Int2           i;
  Int2           indent;
  Int2           j;
  Int2           lineHeight;
  GrouP          p;
  RecT           r;
  Handle         tabs [MAX_TABS * 2];
  Int2           total;
  Int2           wid;

  p = HiddenGroup (h, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  ftp = (FolderTabsPtr) MemNew (sizeof (FolderTabs));
  if (ftp != NULL) {

    SetObjectExtra (p, ftp, CleanupFolderTabs);
    ftp->dialog = (DialoG) p;
    ftp->todialog = NULL;
    ftp->fromdialog = NULL;
    ftp->testdialog = NULL;

    ftp->currentPage = initPage;
    ftp->numPages = 0;
    for (i = 0; i < MAX_TABS; i++) {
      ftp->titles [i] = NULL;
      ftp->tabs [i] = NULL;
    }
    for (i = 0; i < MAX_TABS && titles [i] != NULL; i++) {
      ftp->titles [i] = StringSave (titles [i]);
    }
    for (j = 0; j < MAX_TABS * 2; j++) {
      tabs [j] = NULL;
    }

    ftp->font = font;
    ftp->horizMargin = horizMargin;
    ftp->vertMargin = vertMargin;
    ftp->spaceBtwn = spaceBtwn;
    ftp->cornerTaper = 0;
    ftp->endExt = 0;
    ftp->flipProc = FlipTextToolBarFrame;
    ftp->changeProc = TextToolBarChanging;

    if (maxPerLine == 0) {
      grpWid = 8;
    } else {
      grpWid = ABS (maxPerLine);
      if (grpWid > 8) {
        grpWid = 8;
      }
    }

    if (maxPerLine < 0) {
      p = HiddenGroup (h, -grpWid, 0, NULL);
    } else {
      p = HiddenGroup (h, grpWid, 0, NULL);
    }
    SetGroupSpacing (p, 0, 2);
    SelectFont (font);
    lineHeight = LineHeight ();
    SelectFont (systemFont);
    for (i = 0, j = 0; ftp->titles [i] != NULL && i < MAX_TABS && j < MAX_TABS * 2; i++) {
      SelectFont (font);
      wid = StringWidth (ftp->titles [i]);
      SelectFont (systemFont);
      ftp->tabs [i] = SimplePanel (p, wid + 2 + 2 * ftp->horizMargin,
                                   lineHeight * 1 + 2 * ftp->vertMargin,
                                   DrawTextToolBar);
      tabs [j] = ftp->tabs [i];
      SetObjectExtra (ftp->tabs [i], ftp, NULL);
      SetPanelClick (ftp->tabs [i], ClickFolderTabs, DragFolderTabs,
                     NULL, ReleaseFolderTabs);
      (ftp->numPages)++;
      j++;
    }
    ftp->changeView = changeView;
    ftp->userdata = userdata;
    if (indentNextLine > 0) {
      total = j;
      indent = 0;
      for (j = 0; j < total; j++) {
        if (indent > 0) {
          GetPosition (tabs [j], &r);
          OffsetRect (&r, indent, 0);
          SetPosition (tabs [j], &r);
          AdjustPrnt (tabs [j], &r, FALSE);
        }
        if ((j % grpWid) == (grpWid - 1)) {
          indent += indentNextLine;
        }
      }
    }
    Nlm_SetFolderTabSubclass ((GrouP) p);
  }

  return (DialoG) p;
}



