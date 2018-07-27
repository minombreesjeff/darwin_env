/*   vibmenus.c
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
* File Name:  vibmenus.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   7/1/91
*
* $Revision: 6.13 $
*
* File Description: 
*       Vibrant menu functions
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: vibmenus.c,v $
* Revision 6.13  2001/05/17 22:04:26  kans
* separator item in mswin creates a fake status item link, so GetChildPosition is correct, and thus later submenus have the correct position for enabling and disabling
*
* Revision 6.12  2001/04/03 22:10:19  juran
* Carbon fixes.
*
* Revision 6.11  2001/01/24 18:43:00  kans
* remove artifact bar caused by popup label on Solaris (TF)
*
* Revision 6.10  2000/02/07 20:17:35  lewisg
* minor bug fixes, use gui font for win32
*
* Revision 6.9  1999/12/30 16:47:09  kans
* Carbon changes (Churchill)
*
* Revision 6.8  1999/04/22 15:18:57  vakatov
* Call XtUnrealizeWidget() before XtDestroyWidget() to make sure no
* "post-mortem" callbacks(registered by XtAddEventHandler()) get
* triggered for the destroyed widget. Reason: the widget may not be
* immediately destroyed if XtDestroyWidget() was called in a nested
* event dispatch loop.
*
* Revision 6.7  1999/03/09 17:55:48  vakatov
* [WIN_MOTIF]  Fixed for the ">63 items menu overflow" in
*              Nlm_AppendChoice() and Nlm_AppendOnePopListChoice()
*
* Revision 6.6  1999/02/08 15:46:55  vakatov
* [WIN_MOTIF]  Nlm_SetPopListValue() catch(and just ignore) attempts to
* se a value larger than 63
*
* Revision 6.5  1997/12/12 21:08:35  kans
* a number of symbols changed in the latest CodeWarrior release, now using headers from Apple
*
* Revision 6.4  1997/11/26 21:30:26  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.3  1997/10/08 18:43:58  vakatov
* [WIN_MOTIF] Nlm_ResetMenu() -- remove separator items too
*
* Revision 6.2  1997/10/01 17:08:04  vakatov
* [WIN_MSWIN, WIN_MOTIF]  RemoveSubMenu() -- set parent's "submenu" to NULL
*
* Revision 6.1  1997/09/30 16:39:04  vakatov
* [WIN_MOTIF] ResetMenu() -- check for NULL(ch.group) item;;  destroy
* items in RemoveChoiceGroup()
*
* Revision 5.28  1997/07/10 21:49:32  vakatov
* [WIN_X]  Now able to manage windows having different depths(and
* different visuals and GC).
*
* Revision 5.27  1997/06/04 18:25:22  vakatov
* [WIN_MOTIF] Added accelerators to PopupList items
*
* Revision 5.26  1997/05/12 19:01:57  vakatov
* Disable and gray out the PopupList's cascade button properly;
* [WIN_MOTIF] disable accelerators and mnemonics in the PopupList menu
* when the menu is disabled
*
* Revision 5.25  1997/04/25 19:52:22  kans
* mac submenus call prepare title
*
 * Revision 5.24  1997/04/25  16:13:27  vakatov
 * [WIN_MOTIF,WIN_MSWIN] Catch and render(DoSendFocus) navigation key events
 * occured in PopupList
 *
 * Revision 5.23  1997/04/18  22:17:49  vakatov
 * New way of specifying mnemonics/accelerators in the item(or menu) title:
 * use 1st sybbol after '/' for WIN_MAC;  2nd -- for WIN_MOTIF & WIN_MSWIN
 * mnemonic; 3rd -- for WIN_MOTIF accelerator.
 * [WIN_MOTIF] Added mnemonics for PopupList items.
 *
 * Revision 5.22  1997/04/10  21:35:38  vakatov
 * [WIN_MOTIF]  Implemented the mnemonic and accelerator key functionality
 * for menus and popup lists.
 * [WIN_MSWIN]  Impl. mnemonic keys for submenu cascade items. +typecasting.
 *
 * Revision 5.21  1997/02/13  22:32:59  vakatov
 * [WIN_MOTIF]  In addition to R5.17 -- (1) do not destroy CascadeButtons
 * at all;  (2) do these workarounds only if the preprocessor
 * variable NLM_MOTIF_CASCADEB_BUG is set (see in <vibincld.h>)
 *
 * Revision 5.20  1997/01/28  19:07:39  vakatov
 * [WIN_MOTIF] Nlm_ResetMenu() -- got rid of an unallocated memory read
 *
 * Revision 5.19  1997/01/27  20:13:41  vakatov
 * [WIN_MOTIF] Nlm_RemovePopupList():  remove colormap-change event handler --
 * to ignore events coming from the dangling shell(see "vibwndws.c" R5.38)
 *
 * Revision 5.18  1997/01/14  22:02:07  vakatov
 * Fixed inaccurate string copying -- <mostly potential> 1-byte exceeding of
 * the string size by StringNCat;  missing terminating '\0' by StringNCpy.
 * Removed some unused variables;  set preprocessor conditions more
 * thoroughly to exclude some functions from the compilation on some platforms.
 *
 * Revision 5.17  1997/01/03  21:45:37  vakatov
 * [WIN_MOTIF]  Unmanage(rather than destroy completely) menubar and its
 * cascade buttons to workaround the MOTIF/X11 "double memory freeing" quirk
 * (a "pure" simplest non-Vibrant MOTIF/X11 code also goes to the same bug).
 *
 * Revision 5.16  1996/11/18  20:52:41  vakatov
 * [WIN_MSWIN]  Allowed enabling/disabling of submenus and removing
 * of(top-level, menubar-attached only!) pulldown menus
 *
 * Revision 5.15  1996/11/08  16:54:07  vakatov
 * [WIN_MAC] RemoveMenuItemButNotSub(): now we can remove the submenu(J.Kans);
 * The problems were caused by a bug in "vibutils":Nlm_RemoveLink(), now
 * fixed(in Revision 5.7)
 *
 * Revision 5.14  1996/10/28  19:34:30  vakatov
 * [WIN_MOTIF]  Use Nlm_VibrantDefaultColormap() instead of the display
 * default colormap everywhere in the color handling code.
 * Made a lot of type castings to get rid of all compiler warnings.
 *
 * Revision 5.13  1996/10/10  20:22:17  vakatov
 * [WIN_MOTIF]  Rewrote Nlm_ResetPopList() function to avoid unexpected
 * displaying of popup-list on reset;  also got rid of the unnecessary
 * destroying and further re-creating of underlying widgets and data
 *
 * Revision 5.12  1996/10/04  23:38:19  kans
 * added Nlm_RemoveMenuItemButNotSub to prevent double removal of submenus
 * in windows.  this is tricky code, since submenus must really be installed
 * in the desktop menu bar, but attached to a menu item with the label.  this
 * change fixes a crash when a window containing a window submenu was removed,
 * but there may be uncollected garbage.
 *
 * Revision 5.11  1996/09/04  18:05:54  vakatov
 * [WIN_MOTIF]  PopupList & its Items::  prohibited automatic(Motif-initiated)
 * size change;  PopupList cascade button oversize:: increased from 48 to 52
 * pixels
 *
 * Revision 5.10  1996/07/26  18:12:28  kans
 * Colormap is Motif only
 *
 * Revision 5.9  1996/07/26  18:05:44  vakatov
 * [WIN_MOTIF]  Explicitly set colormaps for all pulldown menus;
 * changed the colormap switching method for the option menus
 * (to fit the LINUX/MOO-TIFF "features")
 *
 * Revision 5.8  1996/07/23  21:20:14  vakatov
 * [WIN_MOTIF]  Do not delay the pop-up(/down) menus realization as it
 * results in problems on SUN_OS and SGI4
 *
 * Revision 5.7  1996/07/09  20:29:10  vakatov
 * [WIN_MOTIF] [OS_UNIX_SUN] (see Revision 4.7 log) -- the same quirk is
 * for the XmPulldownMenu -- fixed
 *
 * Revision 5.6  1996/07/03  20:23:04  kans
 * don't extend margins everywhere - popups got enlarged improperly
 *
 * Revision 5.5  1996/07/03  19:42:37  kans
 * set margin height and width now that resize is automatic
 *
 * Revision 5.4  1996/07/03  16:57:13  vakatov
 * Menu items are allowed to change their size to fit a new title
 *
 * Revision 5.3  1996/06/20  18:19:14  kans
 * submenu before pulldown menu prevented notJustSubMenu from being set
 *
 * Revision 5.2  1996/06/20  17:03:52  kans
 * [WIN_MAC] if desktop menu bar created by submenu only, won't look for
 * command key accelerator (MenuKey call with no menus corrupted system)
 *
 * Revision 5.1  1996/06/14  14:31:24  vakatov
 * [WIN_MOTIF]  Added focus on/off event handler(new function MenuShellFocusCB)
 * to the OptionList's(ad hoc) menu shell -- to keep fixed colormap there
 *
 * Revision 4.9  1996/04/25  16:42:05  kans
 * removed Nlm_WindowMenuBarKey, which caused a crash
 *
 * Revision 4.8  1996/04/25  14:13:50  kans
 * removed Nlm_PopListKey, which caused a crash if no menus in program
 *
 * Revision 4.7  1996/03/27  19:29:51  vakatov
 * OS_UNIX_SUN quirk causing incorrect delayed realization of
 * XmOptionMenu, is fixed
 *
 * Revision 4.6  1996/03/05  19:59:08  epstein
 * fix call to XtSetSensitive()
 *
 * Revision 4.5  1996/03/02  22:36:38  kans
 * reduction of X traffic (DV)
 *
 * Revision 4.4  1996/02/13  17:24:07  kans
 * accelerated set position prior to realization (Denis Vakatov)
 *
 * Revision 4.3  1996/01/10  18:25:01  smirnov
 * AS: add Enable / Disable function for X11 popup.
 *
 * Revision 4.2  1995/12/25  16:03:50  kans
 * popup menu height now always Nlm_popupMenuHeight
 *
 * Revision 4.1  1995/11/27  15:13:41  kans
 * ResetMenu implementation for Windows and Motif (VL)
 *
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 2.29  1995/07/19  20:42:23  kans
 * window rectangle takes Motif menu bar height into account
 *
* ==========================================================================
*/

#include <vibtypes.h>
#include <vibprocs.h>
#include <vibincld.h>

#ifdef WIN_MAC
# include "MoreCarbonAccessors.h"
#define Nlm_MenuTool   MenuHandle
#define Nlm_PopupTool  Nlm_Handle
#define Nlm_ItemTool   Nlm_Handle
#endif

#ifdef WIN_MSWIN
#define Nlm_MenuTool   HMENU
#define Nlm_PopupTool  HWND
#define Nlm_ItemTool   Nlm_Handle
#endif

#ifdef WIN_MOTIF
#define Nlm_MenuTool   Widget
#define Nlm_PopupTool  Widget
#define Nlm_ItemTool   Widget
#define POPUP_LOCATION_ADJUSTMENT -2
#endif

typedef  struct  Nlm_menudata {
  Nlm_MenuTool   handle;
  Nlm_PrompT     prompt;
  Nlm_Int2       menuTag;
  Nlm_PopupTool  popup;
} Nlm_MenuData;

typedef  struct  Nlm_menurec {
  Nlm_GraphicRec  graphicR;
  Nlm_MenuData    menu;
} Nlm_MenuRec, PNTR Nlm_MenPtr;

typedef  struct  Nlm_itemdata {
  Nlm_MenU      subMenu;
  Nlm_Int2      firstItem;
  Nlm_ItemTool  handle;
} Nlm_ItemData;

typedef  struct  Nlm_itemrec {
  Nlm_GraphicRec  graphicR;
  Nlm_ItemData    item;
} Nlm_ItemRec, PNTR Nlm_ItmPtr;

typedef  struct  Nlm_choicedata {
  Nlm_Int2     numItems;
  Nlm_Boolean  isAPopupList;
  Nlm_Int2     popupValue;
  Nlm_ItemTool PNTR handles;
} Nlm_ChoiceData;

typedef  struct  Nlm_choicerec {
  Nlm_ItemRec     itemR;
  Nlm_ChoiceData  choice;
} Nlm_ChoiceRec, PNTR Nlm_ChsPtr;

typedef  struct  Nlm_menubardata {
  Nlm_MenuTool  handle;
  Nlm_Boolean   notJustSubMenu;
} Nlm_MenuBarData;

typedef  struct  Nlm_menubarrec {
  Nlm_BoxRec       boxR;
  Nlm_MenuBarData  menubar;
} Nlm_MenuBarRec, PNTR Nlm_MbrPtr;

static Nlm_GphPrcsPtr   gphprcsptr = NULL;

#ifdef WIN_MAC
static Nlm_GphPrcsPtr   desktopMenuBarProcs;
static Nlm_GphPrcsPtr   desktopPulldownProcs;
#endif
static Nlm_GphPrcsPtr   windowMenuBarProcs;
static Nlm_GphPrcsPtr   windowPulldownProcs;
static Nlm_GphPrcsPtr   popupProcs;
static Nlm_GphPrcsPtr   subMenuProcs;
static Nlm_GphPrcsPtr   subItemProcs;
static Nlm_GphPrcsPtr   commandItemProcs;
static Nlm_GphPrcsPtr   statusItemProcs;
static Nlm_GphPrcsPtr   choiceGroupProcs;
static Nlm_GphPrcsPtr   choiceItemProcs;
static Nlm_GphPrcsPtr   macPopListProcs;
static Nlm_GphPrcsPtr   msPopListProcs;
static Nlm_GphPrcsPtr   popUpChoiceProcs;
static Nlm_GphPrcsPtr   daProcs;

static Nlm_MenU         recentMenu = NULL;
static Nlm_MenuData     recentMenuData;

static Nlm_IteM         recentItem = NULL;
static Nlm_ItemData     recentItemData;

static Nlm_ChoicE       recentChoice = NULL;
static Nlm_ChoiceData   recentChoiceData;

static Nlm_MenuBaR      recentMenuBar = NULL;
static Nlm_MenuBarData  recentMenuBarData;

static Nlm_Int2         nextMenuNum = 1;

#ifdef WIN_MAC
static Nlm_Int2         currentItemNum = 0;
static Nlm_Int2         currentMenuNum = 0;
static Nlm_Int2         currentMenuTag = 0;
#endif

#ifdef WIN_MSWIN
static Nlm_Int2         currentItemID = 0;
static WNDPROC          lpfnNewPopupProc = NULL;
static WNDPROC          lpfnOldPopupProc = NULL;
static Nlm_Boolean      handlechar;
#endif

#ifdef WIN_MOTIF
static Widget           choiceWidget = NULL;
#endif

static void Nlm_LoadMenuData (Nlm_MenU m, Nlm_MenuTool hdl,
			      Nlm_PrompT ppt, Nlm_Int2 tag,
			      Nlm_PopupTool pup)

{
  Nlm_MenuData  PNTR mdptr;
  Nlm_MenPtr    mp;

  if (m != NULL) {
    mp = (Nlm_MenPtr) Nlm_HandLock (m);
    mdptr = &(mp->menu);
    mdptr->handle = hdl;
    mdptr->prompt = ppt;
    mdptr->menuTag = tag;
    mdptr->popup = pup;
    Nlm_HandUnlock (m);
    recentMenu = NULL;
  }
}


static void Nlm_GetMenuData (Nlm_MenU m, Nlm_MenuData * mdata)

{
  Nlm_MenPtr  mp;

  if (m != NULL && mdata != NULL) {
    if (m == recentMenu && NLM_RISKY) {
      *mdata = recentMenuData;
    } else {
      mp = (Nlm_MenPtr) Nlm_HandLock (m);
      *mdata = mp->menu;
      Nlm_HandUnlock (m);
      recentMenu = m;
      recentMenuData = *mdata;
    }
  }
}

static void Nlm_LoadItemData (Nlm_IteM i, Nlm_MenU sub,
			      Nlm_Int2 frst, Nlm_ItemTool hdl)

{
  Nlm_ItemData  PNTR idptr;
  Nlm_ItmPtr   ip;

  if (i != NULL) {
    ip = (Nlm_ItmPtr) Nlm_HandLock (i);
    idptr = &(ip->item);
    idptr->subMenu = sub;
    idptr->firstItem = frst;
    idptr->handle = hdl;
    Nlm_HandUnlock (i);
    recentItem = NULL;
  }
}


static void Nlm_SetItemData (Nlm_IteM i, Nlm_ItemData * idata)
{
  Nlm_ItmPtr  ip;

  if (i != NULL && idata != NULL) {
    ip = (Nlm_ItmPtr) Nlm_HandLock (i);
    ip->item = *idata;
    Nlm_HandUnlock (i);
    recentItem = i;
    recentItemData = *idata;
  }
}


static void Nlm_GetItemData (Nlm_IteM i, Nlm_ItemData * idata)

{
  Nlm_ItmPtr  ip;

  if (i != NULL && idata != NULL) {
    if (i == recentItem && NLM_RISKY) {
      *idata = recentItemData;
    } else {
      ip = (Nlm_ItmPtr) Nlm_HandLock (i);
      *idata = ip->item;
      Nlm_HandUnlock (i);
      recentItem = i;
      recentItemData = *idata;
    }
  }
}

static void Nlm_LoadChoiceData (Nlm_ChoicE c, Nlm_Int2 num,
				Nlm_Boolean plist, Nlm_Int2 pval,
				Nlm_ItemTool PNTR hdls)

{
  Nlm_ChoiceData  PNTR cdptr;
  Nlm_ChsPtr      cp;

  if (c != NULL) {
    cp = (Nlm_ChsPtr) Nlm_HandLock (c);
    cdptr = &(cp->choice);
    cdptr->numItems = num;
    cdptr->isAPopupList = plist;
    cdptr->popupValue = pval;
    cdptr->handles = hdls;
    Nlm_HandUnlock (c);
    recentChoice = NULL;
  }
}

static void Nlm_SetChoiceData (Nlm_ChoicE c, Nlm_ChoiceData * cdata)

{
  Nlm_ChsPtr  cp;

  if (c != NULL && cdata != NULL) {
    cp = (Nlm_ChsPtr) Nlm_HandLock (c);
    cp->choice = *cdata;
    Nlm_HandUnlock (c);
    recentChoice = c;
    recentChoiceData = *cdata;
  }
}

static void Nlm_GetChoiceData (Nlm_ChoicE c, Nlm_ChoiceData * cdata)

{
  Nlm_ChsPtr  cp;

  if (c != NULL && cdata != NULL) {
    if (c == recentChoice && NLM_RISKY) {
      *cdata = recentChoiceData;
    } else {
      cp = (Nlm_ChsPtr) Nlm_HandLock (c);
      *cdata = cp->choice;
      Nlm_HandUnlock (c);
      recentChoice = c;
      recentChoiceData = *cdata;
    }
  }
}

static void Nlm_LoadMenuBarData (Nlm_MenuBaR mb, Nlm_MenuTool hdl)

{
  Nlm_MenuBarData  PNTR mbptr;
  Nlm_MbrPtr       mbp;

  if (mb != NULL) {
    mbp = (Nlm_MbrPtr) Nlm_HandLock (mb);
    mbptr = &(mbp->menubar);
    mbptr->handle = hdl;
    Nlm_HandUnlock (mb);
    recentMenuBar = NULL;
  }
}

#ifdef WIN_MAC
static void Nlm_SetMenuBarData (Nlm_MenuBaR mb, Nlm_MenuBarData * mbdata)
{
  Nlm_MbrPtr  mbp;

  if (mb != NULL && mbdata != NULL) {
    mbp = (Nlm_MbrPtr) Nlm_HandLock (mb);
    mbp->menubar = *mbdata;
    Nlm_HandUnlock (mb);
    recentMenuBar = mb;
    recentMenuBarData = *mbdata;
  }
}
#endif

static void Nlm_GetMenuBarData (Nlm_MenuBaR mb, Nlm_MenuBarData * mbdata)

{
  Nlm_MbrPtr  mbp;

  if (mb != NULL && mbdata != NULL) {
    if (mb == recentMenuBar && NLM_RISKY) {
      *mbdata = recentMenuBarData;
    } else {
      mbp = (Nlm_MbrPtr) Nlm_HandLock (mb);
      *mbdata = mbp->menubar;
      Nlm_HandUnlock (mb);
      recentMenuBar = mb;
      recentMenuBarData = *mbdata;
    }
  }
}

static Nlm_MenuTool Nlm_GetMenuHandle (Nlm_MenU m)

{
  Nlm_MenuData  mdata;

  Nlm_GetMenuData (m, &mdata);
  return mdata.handle;
}

static Nlm_PrompT Nlm_GetMenuPrompt (Nlm_MenU m)

{
  Nlm_MenuData  mdata;

  Nlm_GetMenuData (m, &mdata);
  return mdata.prompt;
}

#ifdef WIN_MAC
static Nlm_Int2 Nlm_GetMenuTag (Nlm_MenU m)
{
  Nlm_MenuData  mdata;

  Nlm_GetMenuData (m, &mdata);
  return mdata.menuTag;
}
#endif


static Nlm_PopupTool Nlm_GetMenuPopup (Nlm_MenU m)
{
  Nlm_MenuData  mdata;

  Nlm_GetMenuData (m, &mdata);
  return mdata.popup;
}


static void Nlm_SetSubMenu (Nlm_IteM i, Nlm_MenU sub)
{
  Nlm_ItemData  idata;

  Nlm_GetItemData (i, &idata);
  idata.subMenu = sub;
  Nlm_SetItemData (i, &idata);
}


static Nlm_MenU Nlm_GetSubMenu (Nlm_IteM i)

{
  Nlm_ItemData  idata;

  Nlm_GetItemData (i, &idata);
  return idata.subMenu;
}

static Nlm_Int2 Nlm_GetFirstItem (Nlm_IteM i)

{
  Nlm_ItemData  idata;

  Nlm_GetItemData (i, &idata);
  return idata.firstItem;
}

static Nlm_ItemTool Nlm_GetItemHandle (Nlm_IteM i)

{
  Nlm_ItemData  idata;

  Nlm_GetItemData (i, &idata);
  return idata.handle;
}


#ifdef WIN_MSWIN
static Nlm_Int2 Nlm_GetChildPosition(Nlm_GraphiC child)
{
  Nlm_Int2    pos    = 1;
  Nlm_GraphiC parent = Nlm_GetParent( child );
  Nlm_GraphiC next_child;
  for (next_child = Nlm_GetChild( parent );
       next_child != NULL  &&  next_child != child;
       next_child = Nlm_GetNext( next_child ), pos++);
  return (Nlm_Int2)(next_child == NULL ? 0 : pos);
}
#endif


static void Nlm_SetNumItems (Nlm_ChoicE c, Nlm_Int2 num)

{
  Nlm_ChoiceData  cdata;

  Nlm_GetChoiceData (c, &cdata);
  cdata.numItems = num;
  Nlm_SetChoiceData (c, &cdata);
}

static Nlm_Int2 Nlm_GetNumItems (Nlm_ChoicE c)

{
  Nlm_ChoiceData  cdata;

  Nlm_GetChoiceData (c, &cdata);
  return cdata.numItems;
}

static Nlm_Boolean Nlm_IsItAPopupList (Nlm_ChoicE c)

{
  Nlm_ChoiceData  cdata;

  Nlm_GetChoiceData (c, &cdata);
  return cdata.isAPopupList;
}

static void Nlm_SetPopupValue (Nlm_ChoicE c, Nlm_Int2 pval)

{
  Nlm_ChoiceData  cdata;

  Nlm_GetChoiceData (c, &cdata);
  cdata.popupValue = pval;
  Nlm_SetChoiceData (c, &cdata);
}

static Nlm_Int2 Nlm_GetPopupValue (Nlm_ChoicE c)

{
  Nlm_ChoiceData  cdata;

  Nlm_GetChoiceData (c, &cdata);
  return cdata.popupValue;
}

static void Nlm_SetChoiceHandles (Nlm_ChoicE c, Nlm_ItemTool PNTR hdls)

{
  Nlm_ChoiceData  cdata;

  Nlm_GetChoiceData (c, &cdata);
  cdata.handles = hdls;
  Nlm_SetChoiceData (c, &cdata);
}

static Nlm_ItemTool PNTR Nlm_GetChoiceHandles (Nlm_ChoicE c)

{
  Nlm_ChoiceData  cdata;

  Nlm_GetChoiceData (c, &cdata);
  return cdata.handles;
}

static Nlm_MenuTool Nlm_GetMenuBarHandle (Nlm_MenuBaR mb)

{
  Nlm_MenuBarData  mbdata;

  Nlm_GetMenuBarData (mb, &mbdata);
  return mbdata.handle;
}


#ifdef WIN_MSWIN
static Nlm_Int2 Nlm_ItemToID (Nlm_MenU m, Nlm_Int2 index)
{
  Nlm_Int2      rsult = 0;
  Nlm_MenuTool  h;

  if (m != NULL && index > 0) {
    h = Nlm_GetMenuHandle (m);
    rsult = GetMenuItemID (h, index - 1);
  }

  return rsult;
}
#endif


static Nlm_Boolean Nlm_IsMenuItemChecked (Nlm_MenU m, Nlm_ItemTool itool, Nlm_Int2 item)

{
  Nlm_MenuTool  h;
#ifdef WIN_MAC
  Nlm_Char      mark;
  Nlm_Int2      temp;
#endif
#ifdef WIN_MSWIN
  Nlm_Int2      id;
#endif

  h = Nlm_GetMenuHandle (m);
#ifdef WIN_MAC
  GetItemMark (h, item, &temp);
  mark = (Nlm_Char) temp;
  return (mark != '\0');
#endif
#ifdef WIN_MSWIN
  id = GetMenuItemID (h, item - 1);
  return (Nlm_Boolean) ((LOBYTE (GetMenuState (h, id, MF_BYCOMMAND)) & MF_CHECKED) != 0);
#endif
#ifdef WIN_MOTIF
  return (Nlm_Boolean) (XmToggleButtonGetState (itool) != FALSE);
#endif
}

static void Nlm_CheckMenuItem (Nlm_MenU m, Nlm_ItemTool itool,
			       Nlm_Int2 item, Nlm_Boolean check)

{
  Nlm_MenuTool  h;
#ifdef WIN_MAC
  Nlm_Char      ch;
#endif
#ifdef WIN_MSWIN
  Nlm_Int2      id;
#endif

  h = Nlm_GetMenuHandle (m);
#ifdef WIN_MAC
  if (check) {
    ch = '\22';
  }
  else {
    ch = '\0';
  }
  SetItemMark (h, item, ch);
#endif
#ifdef WIN_MSWIN
  id = GetMenuItemID (h, item - 1);
  if (check) {
    CheckMenuItem (h, id, MF_CHECKED);
  } else {
    CheckMenuItem (h, id, MF_UNCHECKED);
  }
#endif
#ifdef WIN_MOTIF
  XmToggleButtonSetState (itool, (Boolean) check, FALSE);
#endif
}

#ifdef WIN_MAC
static void Nlm_DeskAccProc (Nlm_ChoicE c)

{
#if TARGET_API_MAC_CARBON >= 1
    // Carbon will create, fill, and handle everything in the Apple menu
    return;
#else
  Nlm_MenuTool  h;
  Nlm_MenU      m;
  Nlm_Char      temp [256];

  m = (Nlm_MenU) Nlm_GetParent ((Nlm_GraphiC) c);
  h = Nlm_GetMenuHandle (m);
  GetMenuItemText (h, currentItemNum, (StringPtr) temp);
  OpenDeskAcc ((StringPtr) temp);
#endif
}
#endif

static void Nlm_ActivateInnerMenus (Nlm_MenU m)

{
  Nlm_IteM      i;
  Nlm_MenU      s;
#ifdef WIN_MAC
  Nlm_MenuTool  h;
#endif

  i = (Nlm_IteM) Nlm_GetChild ((Nlm_GraphiC) m);
  while (i != NULL) {
    s = Nlm_GetSubMenu (i);
    if (s != NULL) {
      if (Nlm_GetEnabled ((Nlm_GraphiC) s)) {
	Nlm_DoEnable ((Nlm_GraphiC) i, FALSE, FALSE);
#ifdef WIN_MAC
	h = Nlm_GetMenuHandle (s);
	InsertMenu (h, -1);
#endif
	Nlm_ActivateInnerMenus (s);
      }
    }
    i = (Nlm_IteM) Nlm_GetNext ((Nlm_GraphiC) i);
  }
}

static void Nlm_DeactivateInnerMenus (Nlm_MenU m)

{
  Nlm_IteM  i;
  Nlm_MenU  s;
#ifdef WIN_MAC
  Nlm_Int2  tag;
#endif

  i = (Nlm_IteM) Nlm_GetChild ((Nlm_GraphiC) m);
  while (i != NULL) {
    s = Nlm_GetSubMenu (i);
    if (s != NULL) {
      Nlm_DoDisable ((Nlm_GraphiC) i, FALSE, FALSE);
#ifdef WIN_MAC
      tag = Nlm_GetMenuTag (s);
      DeleteMenu (tag);
#endif
      Nlm_DeactivateInnerMenus (s);
    }
    i = (Nlm_IteM) Nlm_GetNext ((Nlm_GraphiC) i);
  }
}

#ifdef WIN_MAC
static Nlm_Boolean Nlm_DesktopMenuBarClick (Nlm_GraphiC mb, Nlm_PoinT pt)

{
  Nlm_Int4        choice;
  Nlm_PoinT       global;
  Nlm_MenU        m;
  Nlm_MenU        n;
  Nlm_Boolean     notInGraphic;
  Nlm_PointTool   ptool;
  Nlm_Boolean     rsult;
  Nlm_Int2        windowLoc;
  Nlm_WindowTool  wptr;

  rsult = FALSE;
  currentMenuNum = 0;
  currentItemNum = 0;
  global = pt;
  Nlm_LocalToGlobal (&global);
  Nlm_PoinTToPointTool (global, &ptool);
  windowLoc = FindWindow (ptool, &wptr);
  if (windowLoc == inMenuBar) {
    choice = MenuSelect (ptool);
    currentMenuNum = HiWord (choice);
    currentItemNum = LoWord (choice);
    m = (Nlm_MenU) Nlm_GetChild (mb);
    notInGraphic = TRUE;
    while (m != NULL && notInGraphic) {
      n = (Nlm_MenU) Nlm_GetNext ((Nlm_GraphiC) m);
      currentMenuTag = Nlm_GetMenuTag (m);
      if (Nlm_GetEnabled ((Nlm_GraphiC) m) && Nlm_GetVisible ((Nlm_GraphiC) m) &&
	  Nlm_DoClick ((Nlm_GraphiC) m, pt)) {
	notInGraphic = FALSE;
      }
      m = n;
    }
    rsult = TRUE;
  }
  return rsult;
}

static Nlm_Boolean Nlm_WindowMenuBarClick (Nlm_GraphiC mb, Nlm_PoinT pt)

{
  Nlm_MenU     m;
  Nlm_MenU     n;
  Nlm_Boolean  notInside;

  m = (Nlm_MenU) Nlm_GetChild (mb);
  notInside = TRUE;
  while (m != NULL && notInside) {
    n = (Nlm_MenU) Nlm_GetNext ((Nlm_GraphiC) m);
    currentMenuTag = Nlm_GetMenuTag (m);
    if (Nlm_GetVisible ((Nlm_GraphiC) m) && Nlm_DoClick ((Nlm_GraphiC) m, pt)) {
      notInside = FALSE;
    }
    m = n;
  }
  return (! notInside);
}

static Nlm_Boolean Nlm_DesktopMenuClick (Nlm_GraphiC m, Nlm_PoinT pt)

{
  Nlm_IteM     i;
  Nlm_Boolean  notInMenu;
  Nlm_IteM     n;
  Nlm_Boolean  rsult;
  Nlm_Int2     tag;

  rsult = FALSE;
  tag = Nlm_GetMenuTag ((Nlm_MenU) m);
  if (currentMenuNum > 0 && currentMenuNum == tag) {
    i = (Nlm_IteM) Nlm_GetChild (m);
    notInMenu = TRUE;
    while (i != NULL && notInMenu) {
      n = (Nlm_IteM) Nlm_GetNext ((Nlm_GraphiC) i);
      if (Nlm_GetEnabled ((Nlm_GraphiC) i) && Nlm_GetVisible ((Nlm_GraphiC) i) &&
	  Nlm_DoClick ((Nlm_GraphiC) i, pt)) {
	notInMenu = FALSE;
      }
      i = n;
    }
    HiliteMenu (0);
    currentMenuNum = 0;
    rsult = TRUE;
  }
  return rsult;
}

static void Nlm_PopupAPopupList (Nlm_GraphiC m, Nlm_PoinT pt,
				 Nlm_RectPtr r, Nlm_PoinT pop,
				 Nlm_Int2 item)

{
  Nlm_Int4      choice;
  Nlm_MenuTool  h;
  Nlm_IteM      i;
  Nlm_IteM      n;
  Nlm_WindoW    newwindow;
  Nlm_Boolean   notInMenu;
  Nlm_WindoW    oldwindow;

  oldwindow = Nlm_CurrentWindow ();
  newwindow = Nlm_GetParentWindow (m);
  Nlm_InvertRect (r);
  h = Nlm_GetMenuHandle ((Nlm_MenU) m);
  choice = PopUpMenuSelect (h, pop.y, pop.x, item);
  currentMenuNum = HiWord (choice);
  currentItemNum = LoWord (choice);
  currentMenuTag = Nlm_GetMenuTag ((Nlm_MenU) m);
  i = (Nlm_IteM) Nlm_GetChild (m);
  notInMenu = TRUE;
  while (i != NULL && notInMenu) {
    n = (Nlm_IteM) Nlm_GetNext ((Nlm_GraphiC) i);
    if (Nlm_GetEnabled ((Nlm_GraphiC) i) && Nlm_GetVisible ((Nlm_GraphiC) i) &&
	Nlm_DoClick ((Nlm_GraphiC) i, pt)) {
      notInMenu = FALSE;
    }
    i = n;
  }
  currentMenuNum = 0;
  Nlm_UseWindow (newwindow);
  Nlm_EraseRect (r);
  Nlm_InvalRect (r);
  Nlm_Update ();
  Nlm_UseWindow (oldwindow);
}

static Nlm_Boolean Nlm_PopupClick (Nlm_GraphiC m, Nlm_PoinT pt)

{
  Nlm_MenuTool  h;
  Nlm_PrompT    p;
  Nlm_PoinT     pop;
  Nlm_RecT      r;
  Nlm_Boolean   rsult;
  Nlm_Int2      tag;

  rsult = FALSE;
  p = Nlm_GetMenuPrompt ((Nlm_MenU) m);
  Nlm_GetRect ((Nlm_GraphiC) p, &r);
  if (Nlm_PtInRect (pt, &r)) {
    h = Nlm_GetMenuHandle ((Nlm_MenU) m);
    InsertMenu (h, -1);
    currentMenuNum = 0;
    currentItemNum = 0;
    pop.x = r.left;
    pop.y = r.bottom + 1;
    Nlm_LocalToGlobal (&pop);
    Nlm_PopupAPopupList (m, pt, &r, pop, 0);
    tag = Nlm_GetMenuTag ((Nlm_MenU) m);
    DeleteMenu (tag);
    rsult = TRUE;
  }
  return rsult;
}

static Nlm_Boolean Nlm_PulldownClick (Nlm_GraphiC m, Nlm_PoinT pt)

{
  Nlm_MenuTool  h;
  Nlm_PoinT     pop;
  Nlm_RecT      r;
  Nlm_Boolean   rsult;
  Nlm_Int2      tag;

  rsult = FALSE;
  Nlm_GetRect (m, &r);
  Nlm_InsetRect (&r, 0, 2);
  if (Nlm_PtInRect (pt, &r)) {
    h = Nlm_GetMenuHandle ((Nlm_MenU) m);
    InsertMenu (h, -1);
    currentMenuNum = 0;
    currentItemNum = 0;
    Nlm_GetRect (m, &r);
    Nlm_InsetRect (&r, -6, 0);
    r.top++;
    pop.x = r.left + 1;
    pop.y = r.bottom;
    Nlm_LocalToGlobal (&pop);
    Nlm_PopupAPopupList (m, pt, &r, pop, 0);
    tag = Nlm_GetMenuTag ((Nlm_MenU) m);
    DeleteMenu (tag);
    rsult = TRUE;
  }
  return rsult;
}

static Nlm_Boolean Nlm_MacPopListClick (Nlm_GraphiC m, Nlm_PoinT pt)

{
  Nlm_ChoicE    c;
  Nlm_MenuTool  h;
  Nlm_Int2      item;
  Nlm_PoinT     pop;
  Nlm_RecT      r;
  Nlm_Boolean   rsult;
  Nlm_Int2      tag;

  rsult = FALSE;
  Nlm_GetRect (m, &r);
  Nlm_InsetRect (&r, 2, 3);
  if (Nlm_PtInRect (pt, &r)) {
    h = Nlm_GetMenuHandle ((Nlm_MenU) m);
    InsertMenu (h, -1);
    currentMenuNum = 0;
    currentItemNum = 0;
    item = 0;
    c = (Nlm_ChoicE) Nlm_GetChild (m);
    if (c != NULL) {
      item = Nlm_DoGetValue ((Nlm_GraphiC) c);
    }
    Nlm_GetRect (m, &r);
    pop.x = r.left + 1;
    pop.y = r.top + 1;
    Nlm_UpsetRect (&r, 1, 1, 2, 2);
    Nlm_LocalToGlobal (&pop);
    Nlm_PopupAPopupList (m, pt, &r, pop, item);
    tag = Nlm_GetMenuTag ((Nlm_MenU) m);
    DeleteMenu (tag);
    rsult = TRUE;
  }
  return rsult;
}

static Nlm_Boolean Nlm_MSPopListClick (Nlm_GraphiC m, Nlm_PoinT pt)

{
  Nlm_MenuTool  h;
  Nlm_PoinT     pop;
  Nlm_RecT      r;
  Nlm_Boolean   rsult;
  Nlm_Int2      tag;

  rsult = FALSE;
  Nlm_GetRect (m, &r);
  Nlm_InsetRect (&r, 2, 2);
  if (Nlm_PtInRect (pt, &r)) {
    h = Nlm_GetMenuHandle ((Nlm_MenU) m);
    InsertMenu (h, -1);
    currentMenuNum = 0;
    currentItemNum = 0;
    Nlm_GetRect (m, &r);
    pop.x = r.left + 1;
    pop.y = r.bottom - 1;
    Nlm_UpsetRect (&r, 1, 1, 2, 2);
    Nlm_LocalToGlobal (&pop);
    Nlm_PopupAPopupList (m, pt, &r, pop, 0);
    tag = Nlm_GetMenuTag ((Nlm_MenU) m);
    DeleteMenu (tag);
    rsult = TRUE;
  }
  return rsult;
}

static Nlm_Boolean Nlm_SubItemClick (Nlm_GraphiC i, Nlm_PoinT pt)

{
  Nlm_IteM     c;
  Nlm_IteM     n;
  Nlm_Boolean  notInMenu;
  Nlm_Int2     oldMenuTag;
  Nlm_Boolean  rsult;
  Nlm_MenU     s;

  rsult = FALSE;
  s = Nlm_GetSubMenu ((Nlm_IteM) i);
  if (s != NULL) {
    oldMenuTag = currentMenuTag;
    currentMenuTag = Nlm_GetMenuTag (s);
    c = (Nlm_IteM) Nlm_GetChild ((Nlm_GraphiC) s);
    notInMenu = TRUE;
    while (c != NULL && notInMenu) {
      n = (Nlm_IteM) Nlm_GetNext ((Nlm_GraphiC) c);
      if (Nlm_GetEnabled ((Nlm_GraphiC) c) && Nlm_GetVisible ((Nlm_GraphiC) c) &&
	  Nlm_DoClick ((Nlm_GraphiC) c, pt)) {
	notInMenu = FALSE;
      }
      c = n;
    }
    currentMenuTag = oldMenuTag;
    rsult = (! notInMenu); 
  }
  return rsult;
}

static Nlm_Boolean Nlm_CommItemClick (Nlm_GraphiC i, Nlm_PoinT pt)

{
  Nlm_Int2     index;
  Nlm_Boolean  rsult;

  rsult = FALSE;
  index = Nlm_GetFirstItem ((Nlm_IteM) i);
  if (currentItemNum == index && currentMenuTag == currentMenuNum) {
    Nlm_DoAction (i);
    rsult = TRUE;
  }
  return rsult;
}

static Nlm_Boolean Nlm_StatItemClick (Nlm_GraphiC i, Nlm_PoinT pt)

{
  Nlm_Int2     index;
  Nlm_Boolean  rsult;
  Nlm_Boolean  val;

  rsult = FALSE;
  index = Nlm_GetFirstItem ((Nlm_IteM) i);
  if (currentItemNum == index && currentMenuTag == currentMenuNum) {
    val = Nlm_DoGetStatus (i, 0);
    Nlm_DoSetStatus (i, 0, (! val), FALSE);
    Nlm_DoAction (i);
    rsult = TRUE;
  }
  return rsult;
}

static Nlm_Boolean Nlm_ChoiceGroupClick (Nlm_GraphiC c, Nlm_PoinT pt)

{
  Nlm_Int2     first;
  Nlm_Int2     num;
  Nlm_Boolean  rsult;
  Nlm_Int2     val;

  rsult = FALSE;
  first = Nlm_GetFirstItem ((Nlm_IteM) c);
  num = Nlm_GetNumItems ((Nlm_ChoicE) c);
  val = Nlm_DoGetValue (c);
  if (currentItemNum >= first && currentItemNum < first + num &&
      currentMenuTag == currentMenuNum) {
    if (val != currentItemNum - first + 1) {
      Nlm_DoSetValue (c, currentItemNum - first + 1, FALSE);
      Nlm_DoAction (c);
    }
    rsult = TRUE;
  }
  return rsult;
}

static Nlm_Boolean Nlm_PopupChoiceGroupClick (Nlm_GraphiC c, Nlm_PoinT pt)

{
  return Nlm_ChoiceGroupClick (c, pt);
}

static Nlm_Boolean Nlm_DAClick (Nlm_GraphiC c, Nlm_PoinT pt)

{
  Nlm_Int2     first;
  Nlm_Int2     num;
  Nlm_Boolean  rsult;

  rsult = FALSE;
  first = Nlm_GetFirstItem ((Nlm_IteM) c);
  num = Nlm_GetNumItems ((Nlm_ChoicE) c);
  if (currentItemNum >= first && currentItemNum < first + num &&
      currentMenuTag == currentMenuNum) {
    Nlm_DeskAccProc ((Nlm_ChoicE) c);
    rsult = TRUE;
  }
  return rsult;
}
#endif

#ifdef WIN_MSWIN
static Nlm_Boolean Nlm_WindowMenuBarCommand (Nlm_GraphiC mb)

{
  Nlm_MenU     m;
  Nlm_MenU     n;
  Nlm_Boolean  notInside;

  notInside = TRUE;
  currentItemID = Nlm_currentId;
  m = (Nlm_MenU) Nlm_GetChild (mb);
  while (m != NULL && notInside) {
    n = (Nlm_MenU) Nlm_GetNext ((Nlm_GraphiC) m);
    if (Nlm_GetVisible ((Nlm_GraphiC) m) && Nlm_DoCommand ((Nlm_GraphiC) m)) {
      notInside = FALSE;
    }
    m = n;
  }
  return (Nlm_Boolean) (! notInside);
}

static Nlm_Boolean Nlm_PulldownCommand (Nlm_GraphiC m)

{
  Nlm_IteM     i;
  Nlm_IteM     n;
  Nlm_Boolean  notInside;


  notInside = TRUE;
  i = (Nlm_IteM) Nlm_GetChild (m);
  while (i != NULL && notInside) {
    n = (Nlm_IteM) Nlm_GetNext ((Nlm_GraphiC) i);
    if (Nlm_GetEnabled ((Nlm_GraphiC) i) && Nlm_GetVisible ((Nlm_GraphiC) i) &&
	Nlm_DoCommand ((Nlm_GraphiC) i)) {
      notInside = FALSE;
    }
    i = n;
  }
  return (Nlm_Boolean) (! notInside);
}

static Nlm_Boolean Nlm_MSPopListCommand (Nlm_GraphiC m)

{
  Nlm_ChoicE   c;
  Nlm_Boolean  rsult;

  if (Nlm_currentCode == CBN_SELCHANGE) {
    c = (Nlm_ChoicE) Nlm_GetChild (m);
    Nlm_DoAction ((Nlm_GraphiC) c);
  }
  rsult = TRUE;
  return rsult;
}

static Nlm_Boolean Nlm_MacPopListCommand (Nlm_GraphiC m)

{
  Nlm_Boolean  rsult;

  rsult = Nlm_MSPopListCommand (m);
  return rsult;
}

static Nlm_Boolean Nlm_SubItemCommand (Nlm_GraphiC prnt)

{
  Nlm_IteM     i;
  Nlm_MenU     m;
  Nlm_IteM     n;
  Nlm_Boolean  notInside;


  notInside = TRUE;
  m = Nlm_GetSubMenu ((Nlm_IteM) prnt);
  i = (Nlm_IteM) Nlm_GetChild ((Nlm_GraphiC) m);
  while (i != NULL && notInside) {
    n = (Nlm_IteM) Nlm_GetNext ((Nlm_GraphiC) i);
    if (Nlm_GetEnabled ((Nlm_GraphiC) i) && Nlm_GetVisible ((Nlm_GraphiC) i) &&
	Nlm_DoCommand ((Nlm_GraphiC) i)) {
      notInside = FALSE;
    }
    i = n;
  }
  return (Nlm_Boolean) (! notInside);
}

static Nlm_Boolean Nlm_CommItemCommand (Nlm_GraphiC i)

{
  Nlm_Int2     id;
  Nlm_Int2     index;
  Nlm_MenU     m;
  Nlm_Boolean  rsult;

  rsult = FALSE;
  index = Nlm_GetFirstItem ((Nlm_IteM) i);
  m = (Nlm_MenU) Nlm_GetParent (i);
  id = Nlm_ItemToID (m, index);
  if (currentItemID == id) {
    Nlm_DoAction (i);
    rsult = TRUE;
  }
  return rsult;
}

static Nlm_Boolean Nlm_StatItemCommand (Nlm_GraphiC i)

{
  Nlm_Int2     id;
  Nlm_Int2     index;
  Nlm_MenU     m;
  Nlm_Boolean  rsult;

  rsult = FALSE;
  index = Nlm_GetFirstItem ((Nlm_IteM) i);
  m = (Nlm_MenU) Nlm_GetParent (i);
  id = Nlm_ItemToID (m, index);
  if (currentItemID == id) {
    if (Nlm_DoGetStatus (i, 0)) {
      Nlm_DoSetStatus (i, 0, FALSE, FALSE);
    } else {
      Nlm_DoSetStatus (i, 0, TRUE, FALSE);
    }
    Nlm_DoAction (i);
    rsult = TRUE;
  }
  return rsult;
}

static Nlm_Boolean Nlm_ChoiceGroupCommand (Nlm_GraphiC c)

{
  Nlm_Int2     first;
  Nlm_Int2     id;
  Nlm_Int2     index;
  Nlm_MenU     m;
  Nlm_Int2     num;
  Nlm_Boolean  rsult;
  Nlm_Int2     val;

  rsult = FALSE;
  first = Nlm_GetFirstItem ((Nlm_IteM) c);
  num = Nlm_GetNumItems ((Nlm_ChoicE) c);
  val = Nlm_DoGetValue (c);
  index = first;
  m = (Nlm_MenU) Nlm_GetParent (c);
  while (num > 0) {
    id = Nlm_ItemToID (m, index);
    if (currentItemID == id) {
      if (val != index - first + 1) {
	Nlm_DoSetValue (c, (Nlm_Int2)(index - first + 1), FALSE);
	Nlm_DoAction (c);
      }
      num = 0;
      rsult = TRUE;
    } else {
      index++;
      num--;
    }
  }
  return rsult;
}
#endif

#ifdef WIN_MOTIF
static void Nlm_CommItemCallback (Nlm_GraphiC i)

{
  Nlm_DoAction (i);
}

static void Nlm_StatItemCallback (Nlm_GraphiC i)

{
  Nlm_DoAction (i);
}

static void Nlm_ChoiceGroupCallback (Nlm_GraphiC c)

{
  Nlm_ItemTool  PNTR hdls;
  Nlm_Int2      i;
  Nlm_Int2      j;
  Nlm_Int2      num;

  hdls = Nlm_GetChoiceHandles ((Nlm_ChoicE) c);
  num = Nlm_GetNumItems ((Nlm_ChoicE) c);
  if (hdls != NULL && num > 0) {
    i = 0;
    while (i < num && hdls [i] != (Nlm_ItemTool) choiceWidget) {
      i++;
    }
    if (i < num) {
      if (XmToggleButtonGetState (hdls [i]) == FALSE) {
	XmToggleButtonSetState (hdls [i], TRUE, FALSE);
      } else {
	for (j = 0; j < num; j++) {
	  if (j != i) {
	    XmToggleButtonSetState (hdls [j], FALSE, FALSE);
	  }
	}
	Nlm_DoAction (c);
      }
    }
  }
}

static void Nlm_PopupChoiceCallback (Nlm_GraphiC c)

{
  Nlm_ItemTool  PNTR hdls;
  Nlm_Int2      i;
  Nlm_Int2      num;
  Nlm_Int2      val;

  hdls = Nlm_GetChoiceHandles ((Nlm_ChoicE) c);
  num = Nlm_GetNumItems ((Nlm_ChoicE) c);
  if (hdls != NULL && num > 0) {
    i = 0;
    while (i < num && hdls [i] != (Nlm_ItemTool) choiceWidget) {
      i++;
    }
    if (i < num) {
      i++;
      val = Nlm_GetPopupValue ((Nlm_ChoicE) c);
      if (i != val) {
	Nlm_SetPopupValue ((Nlm_ChoicE) c, i);
	XtUnmanageChild (hdls [63]);
	Nlm_DoAction (c);
      }
    }
  }
}
#endif

#ifdef WIN_MAC
static Nlm_Boolean Nlm_DesktopMenuBarKey (Nlm_GraphiC mb, Nlm_Char ch)

{
  Nlm_Int4         choice;
  Nlm_MenU         m;
  Nlm_MenuBarData  mbdata;
  Nlm_MenU         n;
  Nlm_Boolean      notInside;
  Nlm_Boolean      rsult;

  rsult = FALSE;
  currentMenuNum = 0;
  currentItemNum = 0;
  if (Nlm_cmmdKey) {
    Nlm_GetMenuBarData ((Nlm_MenuBaR) mb, &mbdata);
    if (mbdata.notJustSubMenu) {
      choice = MenuKey (ch);
      currentMenuNum = HiWord (choice);
      currentItemNum = LoWord (choice);
      m = (Nlm_MenU) Nlm_GetChild (mb);
      notInside = TRUE;
      while (m != NULL && notInside) {
	n = (Nlm_MenU) Nlm_GetNext ((Nlm_GraphiC) m);
	currentMenuTag = Nlm_GetMenuTag (m);
	if (Nlm_GetEnabled ((Nlm_GraphiC) m) && Nlm_GetVisible ((Nlm_GraphiC) m) &&
	    Nlm_DoKey ((Nlm_GraphiC) m, ch)) {
	  notInside = FALSE;
	}
	m = n;
      }
    rsult = TRUE;
    }
  }
  return rsult;
}

static Nlm_Boolean Nlm_DesktopMenuKey (Nlm_GraphiC m, Nlm_Char ch)

{
  Nlm_IteM     i;
  Nlm_IteM     n;
  Nlm_Boolean  notInMenu;
  Nlm_Boolean  rsult;
  Nlm_Int2     tag;

  rsult = FALSE;
  tag = Nlm_GetMenuTag ((Nlm_MenU) m);
  if (currentMenuNum > 0 && currentMenuNum == tag) {
    i = (Nlm_IteM) Nlm_GetChild (m);
    notInMenu = TRUE;
    while (i != NULL && notInMenu) {
      n = (Nlm_IteM) Nlm_GetNext ((Nlm_GraphiC) i);
      if (Nlm_GetEnabled ((Nlm_GraphiC) i) && Nlm_GetVisible ((Nlm_GraphiC) i) &&
	  Nlm_DoKey ((Nlm_GraphiC) i, ch)) {
	notInMenu = FALSE;
      }
      i = n;
    }
    HiliteMenu (0);
    currentMenuNum = 0;
    rsult = TRUE;
  }
  return rsult;
}

static Nlm_Boolean Nlm_SubMenuKey (Nlm_GraphiC m, Nlm_Char ch)

{
  Nlm_IteM     i;
  Nlm_Boolean  rsult;

  rsult = FALSE;
  i = (Nlm_IteM) Nlm_GetParent (m);
  if (i != NULL) {
    rsult = Nlm_DesktopMenuKey (m, ch);
  }
  return rsult;
}

static Nlm_Boolean Nlm_PopupKey (Nlm_GraphiC m, Nlm_Char ch)

{
  Nlm_Int4      choice;
  Nlm_MenuTool  h;
  Nlm_IteM      i;
  Nlm_IteM      n;
  Nlm_Boolean   notInMenu;
  Nlm_RecT      r;
  Nlm_Boolean   rsult;
  Nlm_Int2      tag;

  rsult = FALSE;
  if (Nlm_cmmdKey) {
    tag = Nlm_GetMenuTag ((Nlm_MenU) m);
    h = Nlm_GetMenuHandle ((Nlm_MenU) m);
    InsertMenu (h, -1);
    currentMenuNum = 0;
    currentItemNum = 0;
    choice = MenuKey (ch);
    currentMenuNum = HiWord (choice);
    currentItemNum = LoWord (choice);
    if (currentMenuNum > 0 && currentMenuNum == tag) {
      Nlm_GetRect (m, &r);
      Nlm_InvertRect (&r);
      i = (Nlm_IteM) Nlm_GetChild (m);
      notInMenu = TRUE;
      while (i != NULL && notInMenu) {
	n = (Nlm_IteM) Nlm_GetNext ((Nlm_GraphiC) i);
      if (Nlm_GetEnabled ((Nlm_GraphiC) i) && Nlm_GetVisible ((Nlm_GraphiC) i) &&
	  Nlm_DoKey ((Nlm_GraphiC) i, ch)) {
	  notInMenu = FALSE;
	}
	i = n;
      }
      currentMenuNum = 0;
      DeleteMenu (tag);
      Nlm_DoDraw (m);
      rsult = TRUE;
    } else {
      currentMenuNum = 0;
      DeleteMenu (tag);
    }
  }
  return rsult;
}

static Nlm_Boolean Nlm_CommItemKey (Nlm_GraphiC i, Nlm_Char ch)

{
  Nlm_Int2     index;
  Nlm_Boolean  rsult;

  rsult = FALSE;
  index = Nlm_GetFirstItem ((Nlm_IteM) i);
  if (currentItemNum == index) {
    Nlm_DoAction (i);
    rsult = TRUE;
  }
  return rsult;
}

static Nlm_Boolean Nlm_StatItemKey (Nlm_GraphiC i, Nlm_Char ch)

{
  Nlm_MenuTool  h;
  Nlm_Int2      index;
  Nlm_MenU      m;
  Nlm_Char      mark;
  Nlm_Boolean   rsult;
  Nlm_Int2      temp;

  rsult = FALSE;
  index = Nlm_GetFirstItem ((Nlm_IteM) i);
  if (currentItemNum == index) {
    m = (Nlm_MenU) Nlm_GetParent (i);
    h = Nlm_GetMenuHandle (m);
    GetItemMark (h, currentItemNum, &temp);
    mark = (Nlm_Char) temp;
    Nlm_CheckMenuItem (m, NULL, currentItemNum, (mark == '\0'));
    Nlm_DoAction (i);
    rsult = TRUE;
  }
  return rsult;
}

static Nlm_Boolean Nlm_ChoiceGroupKey (Nlm_GraphiC c, Nlm_Char ch)

{
  Nlm_Int2     first;
  Nlm_Int2     num;
  Nlm_Boolean  rsult;
  Nlm_Int2     val;

  rsult = FALSE;
  first = Nlm_GetFirstItem ((Nlm_IteM) c);
  num = Nlm_GetNumItems ((Nlm_ChoicE) c);
  val = Nlm_DoGetValue (c);
  if (currentItemNum >= first && currentItemNum < first + num) {
    if (val != currentItemNum - first + 1) {
      Nlm_DoSetValue (c, currentItemNum - first + 1, FALSE);
      Nlm_DoAction (c);
      rsult = TRUE;
    }
  }
  return rsult;
}

static Nlm_Boolean Nlm_DAKey (Nlm_GraphiC c, Nlm_Char ch)

{
  Nlm_Int2     first;
  Nlm_Int2     num;
  Nlm_Boolean  rsult;

  rsult = FALSE;
  first = Nlm_GetFirstItem ((Nlm_IteM) c);
  num = Nlm_GetNumItems ((Nlm_ChoicE) c);
  if (currentItemNum >= first && currentItemNum < first + num) {
    Nlm_DeskAccProc ((Nlm_ChoicE) c);
    rsult = TRUE;
  }
  return rsult;
}
#endif

#ifdef WIN_MAC
static void Nlm_DrawWindowMenuBar (Nlm_GraphiC mb)

{
  Nlm_MenU        m;
  Nlm_RecT        r;
  Nlm_WindowTool  wptr;
  Rect bounds;

  wptr = Nlm_ParentWindowPtr (mb);
  GetPortBounds(GetWindowPort(wptr), &bounds);
  Nlm_RectToolToRecT (&bounds, &r);
  r.top = 19;
  r.bottom = 21;
  if (Nlm_RectInRgn (&r, Nlm_updateRgn)) {
    MoveTo (0, 20);
    LineTo (r.right, 20);
  }
  m = (Nlm_MenU) Nlm_GetChild (mb);
  while (m != NULL) {
    Nlm_DoDraw ((Nlm_GraphiC) m);
    m = (Nlm_MenU) Nlm_GetNext ((Nlm_GraphiC) m);
  }
}

static void Nlm_DrawDesktopMenu (Nlm_GraphiC m)

{
  DrawMenuBar ();
}

static void Nlm_DrawWindowMenu (Nlm_GraphiC m)

{
  Nlm_PrompT  p;
  Nlm_RecT    r;

  if (Nlm_GetVisible (m) && Nlm_GetAllParentsVisible (m)) {
    Nlm_GetRect (m, &r);
    if (Nlm_RectInRgn (&r, Nlm_updateRgn)) {
      p = Nlm_GetMenuPrompt ((Nlm_MenU) m);
      if (p != NULL) {
	Nlm_DoDraw ((Nlm_GraphiC) p);
	Nlm_SelectFont (Nlm_systemFont);
      }
    }
  }
}

static void Nlm_DrawPopup (Nlm_GraphiC m)

{
  Nlm_PrompT  p;
  Nlm_RecT    r;

  if (Nlm_GetVisible (m) && Nlm_GetAllParentsVisible (m)) {
    Nlm_GetRect (m, &r);
    if (Nlm_RectInRgn (&r, Nlm_updateRgn)) {
      p = Nlm_GetMenuPrompt ((Nlm_MenU) m);
      if (p != NULL) {
	Nlm_GetRect ((Nlm_GraphiC) p, &r);
	Nlm_InsetRect (&r, -1, -1);
	Nlm_FrameRect (&r);
	Nlm_OffsetRect (&r, 1, 1);
	Nlm_FrameRect (&r);
	Nlm_DoDraw ((Nlm_GraphiC) p);
	Nlm_SelectFont (Nlm_systemFont);
      }
    }
  }
}

static void Nlm_DrawPopList (Nlm_GraphiC m)

{
  Nlm_ChoicE  c;
  Nlm_RecT    pr;
  Nlm_RecT    r;
  Nlm_Char    temp [256];
  Nlm_Int2    value;

  if (Nlm_GetVisible (m) && Nlm_GetAllParentsVisible (m)) {
    Nlm_GetRect (m, &r);
    if (Nlm_RectInRgn (&r, Nlm_updateRgn)) {
      Nlm_SelectFont (Nlm_systemFont);
      Nlm_UpsetRect (&r, 1, 1, 2, 2);
      pr = r;
      Nlm_InsetRect (&r, -1, -1);
      Nlm_FrameRect (&r);
      Nlm_OffsetRect (&r, 1, 1);
      Nlm_FrameRect (&r);
      Nlm_EraseRect (&pr);
      c = (Nlm_ChoicE) Nlm_GetChild (m);
      if (c != NULL) {
	value = Nlm_DoGetValue ((Nlm_GraphiC) c);
	if (value > 0) {
	  Nlm_DoGetTitle ((Nlm_GraphiC) c, value, temp, sizeof (temp));
	  pr.left += 13;
	  Nlm_DrawString (&pr, temp, 'l', FALSE);
	}
      }
    }
  }
}
#endif


static void Nlm_ShowWindowMenuBar (Nlm_GraphiC mb, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_WindoW      tempPort;
#ifdef WIN_MSWIN
  Nlm_WindowTool  wptr;
#endif
#ifdef WIN_MOTIF
  Nlm_MenuTool    h;
#endif

  tempPort = Nlm_SavePortIfNeeded (mb, savePort);
  Nlm_SetVisible (mb, TRUE);
#ifdef WIN_MAC
  Nlm_DoDraw (mb);
#endif
#ifdef WIN_MSWIN
  wptr = Nlm_ParentWindowPtr (mb);
  DrawMenuBar (wptr);
#endif
#ifdef WIN_MOTIF
  h = Nlm_GetMenuBarHandle ((Nlm_MenuBaR) mb);
  XtManageChild (h);
#endif

  Nlm_RestorePort (tempPort);
}


#ifdef WIN_MAC
static void Nlm_ShowDesktopMenu (Nlm_GraphiC m, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_MenuTool  h;
  Nlm_WindoW    tempPort;

  tempPort = Nlm_SavePortIfNeeded (m, savePort);
  Nlm_SetVisible (m, TRUE);
  h = Nlm_GetMenuHandle ((Nlm_MenU) m);
  InsertMenu (h, 0);
  Nlm_ActivateInnerMenus ((Nlm_MenU) m);
  Nlm_DoDraw (m);
  Nlm_RestorePort (tempPort);
}
#endif

static void Nlm_ShowPopup (Nlm_GraphiC m, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_PrompT      p;
  Nlm_WindoW      tempPort;
#ifdef WIN_MSWIN
  Nlm_WindowTool  wptr;
#endif

  if (setFlag) {
    Nlm_SetVisible (m, TRUE);
  }
  if (Nlm_GetVisible (m) && Nlm_AllParentsButWindowVisible (m)) {
    tempPort = Nlm_SavePortIfNeeded (m, savePort);
    p = Nlm_GetMenuPrompt ((Nlm_MenU) m);
    if (p != NULL) {
      Nlm_DoShow ((Nlm_GraphiC) p, TRUE, FALSE);
    }
#ifdef WIN_MAC
    Nlm_DoDraw (m);
#endif
#ifdef WIN_MSWIN
    wptr = Nlm_ParentWindowPtr (m);
    DrawMenuBar (wptr);
#endif
#ifdef WIN_MOTIF
#endif
    Nlm_RestorePort (tempPort);
  }
}

static void Nlm_ShowPopListMenu (Nlm_GraphiC m, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_WindoW     tempPort;
#ifdef WIN_MSWIN
  Nlm_PopupTool  u;
#endif
#ifdef WIN_MOTIF
  Nlm_PopupTool  u;
#endif

  if (setFlag) {
    Nlm_SetVisible (m, TRUE);
  }
  if (Nlm_GetVisible (m) && Nlm_AllParentsButWindowVisible (m)) {
    tempPort = Nlm_SavePortIfNeeded (m, savePort);
#ifdef WIN_MAC
    Nlm_DoDraw (m);
#endif
#ifdef WIN_MSWIN
    u = Nlm_GetMenuPopup ((Nlm_MenU) m);
    if (u != NULL) {
      ShowWindow (u, SW_SHOW);
      UpdateWindow (u);
    }
#endif
#ifdef WIN_MOTIF
    u = Nlm_GetMenuPopup ((Nlm_MenU) m);
    XtManageChild (u);
#endif
    Nlm_RestorePort (tempPort);
  }
}

static void Nlm_ShowPopListChoice (Nlm_GraphiC c, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_MenU  m;

  if (setFlag) {
    Nlm_SetVisible (c, TRUE);
  }
  m = (Nlm_MenU) Nlm_GetParent (c);
  Nlm_DoShow ((Nlm_GraphiC) m, setFlag, savePort);
}

#ifdef WIN_MAC
static void Nlm_HideDesktopMenu (Nlm_GraphiC m, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_Int2    tag;
  Nlm_WindoW  tempPort;

  tempPort = Nlm_SavePortIfNeeded (m, savePort);
  Nlm_SetVisible (m, FALSE);
  tag = Nlm_GetMenuTag ((Nlm_MenU) m);
  DeleteMenu (tag);
  Nlm_DeactivateInnerMenus ((Nlm_MenU) m);
  Nlm_DoDraw (m);
  Nlm_RestorePort (tempPort);
}
#endif

static void Nlm_HidePulldown (Nlm_GraphiC m, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  Nlm_RecT    r;
  Nlm_WindoW  tempPort;

  tempPort = Nlm_SavePortIfNeeded (m, savePort);
  Nlm_SetVisible (m, FALSE);
  Nlm_GetRect (m, &r);
  Nlm_EraseRect (&r);
  Nlm_ValidRect (&r);
  Nlm_RestorePort (tempPort);
#endif
}

static void Nlm_HidePopup (Nlm_GraphiC m, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  Nlm_RecT    r;
  Nlm_WindoW  tempPort;

  if (setFlag) {
    Nlm_SetVisible (m, FALSE);
  }
  if (Nlm_GetAllParentsVisible (m)) {
    tempPort = Nlm_SavePortIfNeeded (m, savePort);
    Nlm_GetRect (m, &r);
    /*
    Nlm_InsetRect (&r, -2, -2);
    Nlm_OffsetRect (&r, 1, 1);
    */
    Nlm_InsetRect (&r, -1, -1);
    Nlm_EraseRect (&r);
    Nlm_ValidRect (&r);
    Nlm_RestorePort (tempPort);
  }
#endif
}

static void Nlm_HidePopListMenu (Nlm_GraphiC m, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_WindoW     tempPort;
#ifdef WIN_MAC
  Nlm_RecT       r;
#endif
#ifdef WIN_MSWIN
  Nlm_PopupTool  u;
#endif
#ifdef WIN_MOTIF
  Nlm_PopupTool  u;
#endif

  if (setFlag) {
    Nlm_SetVisible (m, FALSE);
  }
  tempPort = Nlm_SavePortIfNeeded (m, savePort);
#ifdef WIN_MAC
  if (Nlm_GetAllParentsVisible (m)) {
    Nlm_GetRect (m, &r);
    /*
    Nlm_InsetRect (&r, -2, -2);
    Nlm_OffsetRect (&r, 1, 1);
    */
    Nlm_InsetRect (&r, -1, -1);
    Nlm_EraseRect (&r);
    Nlm_ValidRect (&r);
  }
#endif
#ifdef WIN_MSWIN
  u = Nlm_GetMenuPopup ((Nlm_MenU) m);
  if (u != NULL) {
    ShowWindow (u, SW_HIDE);
    UpdateWindow (u);
  }
#endif
#ifdef WIN_MOTIF
    u = Nlm_GetMenuPopup ((Nlm_MenU) m);
    XtUnmanageChild (u);
#endif
  Nlm_RestorePort (tempPort);
}


static void Nlm_HidePopListChoice(Nlm_GraphiC c, Nlm_Boolean setFlag,
                                  Nlm_Boolean savePort)
{
  if ( setFlag )
    Nlm_SetVisible(c, FALSE);

  Nlm_DoHide(Nlm_GetParent( c ), setFlag, savePort);
}


#ifdef WIN_MAC
static void Nlm_EnableDesktopMenu (Nlm_GraphiC m, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_MenuTool  h;
  Nlm_WindoW    tempPort;

  tempPort = Nlm_SavePortIfNeeded (m, savePort);
  Nlm_SetEnabled (m, TRUE);
  h = Nlm_GetMenuHandle ((Nlm_MenU) m);
  EnableMenuItem (h, 0);
  if (Nlm_GetVisible (m)) {
    DrawMenuBar ();
  }
  Nlm_RestorePort (tempPort);
}
#endif


static void Nlm_EnablePulldown (Nlm_GraphiC m, Nlm_Boolean setFlag, Nlm_Boolean savePort)
{
#ifdef WIN_MAC
  Nlm_MenuTool  h;
  Nlm_PrompT    p;

  if (setFlag) {
    Nlm_SetEnabled (m, TRUE);
  }
  if (Nlm_GetEnabled (m) && Nlm_GetAllParentsEnabled (m)) {
    if (Nlm_GetVisible (m) && Nlm_GetAllParentsVisible (m)) {
      Nlm_WindoW tempPort = Nlm_SavePortIfNeeded (m, savePort);
      h = Nlm_GetMenuHandle ((Nlm_MenU) m);
      EnableMenuItem (h, 0);
      Nlm_DoDraw (m);
      p = Nlm_GetMenuPrompt ((Nlm_MenU) m);
      Nlm_DoEnable ((Nlm_GraphiC) p, TRUE, FALSE);
      Nlm_RestorePort (tempPort);
    }
  }

#else

  if ( setFlag )
    Nlm_SetEnabled(m, TRUE);

  if (Nlm_GetEnabled( m )  &&  Nlm_GetAllParentsEnabled( m ))
    {
      Nlm_WindoW tempPort = Nlm_SavePortIfNeeded (m, savePort);
#ifdef WIN_MSWIN
      Nlm_GraphiC g = Nlm_GetChild( m );
      while (g != NULL) {
        Nlm_DoEnable(g, FALSE, FALSE);
        g = Nlm_GetNext( g );
      }
      DrawMenuBar( Nlm_ParentWindowPtr(m) );
#endif
#ifdef WIN_MOTIF
      Nlm_PopupTool u = Nlm_GetMenuPopup( (Nlm_MenU)m );
      if (u != NULL)
        XtVaSetValues(u, XmNsensitive, TRUE, NULL);
#endif
      Nlm_RestorePort( tempPort );
    }
#endif
}


static void Nlm_EnablePopup (Nlm_GraphiC m, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  Nlm_MenuTool  h;
  Nlm_WindoW    tempPort;

  if (setFlag) {
    Nlm_SetEnabled (m, TRUE);
  }
  if (Nlm_GetEnabled (m) && Nlm_GetAllParentsEnabled (m)) {
    if (Nlm_GetVisible (m) && Nlm_GetAllParentsVisible (m)) {
      tempPort = Nlm_SavePortIfNeeded (m, savePort);
      h = Nlm_GetMenuHandle ((Nlm_MenU) m);
      EnableMenuItem (h, 0);
      Nlm_DoDraw (m);
      Nlm_RestorePort (tempPort);
    }
  }
#endif
}

static void Nlm_EnablePopList (Nlm_GraphiC m, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_WindoW     tempPort;
#ifdef WIN_MAC
  Nlm_MenuTool   h;
#endif
#ifdef WIN_MSWIN
  Nlm_PopupTool  u;
#endif
#ifdef WIN_MOTIF
    Nlm_PopupTool  c;
#endif

  if (setFlag) {
    Nlm_SetEnabled (m, TRUE);
  }
  if (Nlm_GetEnabled (m) && Nlm_GetAllParentsEnabled (m)) {
    tempPort = Nlm_SavePortIfNeeded (m, savePort);
#ifdef WIN_MAC
    h = Nlm_GetMenuHandle ((Nlm_MenU) m);
    EnableMenuItem (h, 0);
    if (Nlm_GetVisible (m) && Nlm_GetAllParentsVisible (m)) {
      Nlm_DoDraw (m);
    }
#endif
#ifdef WIN_MSWIN
    u = Nlm_GetMenuPopup ((Nlm_MenU) m);
    if (u != NULL) {
      EnableWindow (u, TRUE);
    }
#endif
#ifdef WIN_MOTIF
    c = Nlm_GetMenuPopup ((Nlm_MenU) m); 
    XtSetSensitive(XmOptionButtonGadget(c), TRUE);
    XtVaSetValues(c, XmNpopupEnabled, TRUE, NULL);
#endif
    Nlm_RestorePort (tempPort);
  }
}

static void Nlm_EnableMenuItem (Nlm_GraphiC i, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_MenuTool  h;
  Nlm_Int2      itm;
  Nlm_MenU      m;
  Nlm_WindoW    tempPort;
#ifdef WIN_MSWIN
  Nlm_Int2      id;
#endif
#ifdef WIN_MOTIF
  Nlm_ItemTool  itool;
#endif

  tempPort = Nlm_SavePortIfNeeded (i, savePort);
  Nlm_SetEnabled (i, TRUE);
  m = (Nlm_MenU) Nlm_GetParent (i);
  h = Nlm_GetMenuHandle (m);
  itm = Nlm_GetFirstItem ((Nlm_IteM) i);
#ifdef WIN_MAC
  EnableMenuItem (h, itm);
#endif
#ifdef WIN_MSWIN
  id = Nlm_ItemToID (m, itm);
  EnableMenuItem (h, id, MF_ENABLED);
#endif
#ifdef WIN_MOTIF
  itool = Nlm_GetItemHandle ((Nlm_IteM) i);
  if ( !XtIsSensitive( itool ) )
    XtSetSensitive(itool, TRUE);
#endif
  Nlm_RestorePort (tempPort);
}


static void Nlm_EnableSubMenu (Nlm_GraphiC m, Nlm_Boolean setFlag, Nlm_Boolean savePort)
{
  Nlm_WindoW   tempPort = Nlm_SavePortIfNeeded(m, savePort);
  Nlm_IteM     i        = (Nlm_IteM)Nlm_GetParent( m );

  Nlm_SetEnabled (m, TRUE);

  if (i != NULL)
    {
#ifdef WIN_MAC
      Nlm_MenuTool h;
      Nlm_EnableMenuItem ((Nlm_GraphiC) i, FALSE, FALSE);
      h = Nlm_GetMenuHandle ((Nlm_MenU) m);
      InsertMenu (h, -1);
#endif
#ifdef WIN_MSWIN
      Nlm_MenU     prnt   = (Nlm_MenU)Nlm_GetParent( (Nlm_GraphiC)i );
      Nlm_MenuTool prnt_h = Nlm_GetMenuHandle( prnt );
      Nlm_Int2     pos    = Nlm_GetChildPosition( (Nlm_GraphiC)i );
      EnableMenuItem(prnt_h, pos-1, MF_BYPOSITION|MF_ENABLED);
#endif
#ifdef WIN_MOTIF
      Nlm_PopupTool pop = Nlm_GetMenuPopup ((Nlm_MenU) m);
      XtVaSetValues (pop, XmNsensitive, TRUE, NULL);
#endif
      Nlm_ActivateInnerMenus ((Nlm_MenU) m);
    }

  Nlm_RestorePort( tempPort );
}


static void Nlm_EnableChoice (Nlm_GraphiC c, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_MenuTool  h;
  Nlm_Int2      itm;
  Nlm_MenU      m;
  Nlm_Int2      num;
  Nlm_WindoW    tempPort;
#ifdef WIN_MSWIN
  Nlm_Int2      id;
#endif
#ifdef WIN_MOTIF
  Nlm_ItemTool  PNTR hdls;
#endif

  tempPort = Nlm_SavePortIfNeeded (c, savePort);
  Nlm_SetEnabled (c, TRUE);
  m = (Nlm_MenU) Nlm_GetParent (c);
  itm = Nlm_GetFirstItem ((Nlm_IteM) c);
  num = Nlm_GetNumItems ((Nlm_ChoicE) c);
  h = Nlm_GetMenuHandle (m);
  while (num > 0) {
#ifdef WIN_MAC
    EnableMenuItem (h, itm);
#endif
#ifdef WIN_MSWIN
    if (! Nlm_IsItAPopupList ((Nlm_ChoicE) c)) {
      id = Nlm_ItemToID (m, itm);
      EnableMenuItem (h, id, MF_ENABLED);
    }
#endif
#ifdef WIN_MOTIF
    hdls = Nlm_GetChoiceHandles ((Nlm_ChoicE) c);
    XtVaSetValues (hdls [num - 1], XmNsensitive, TRUE, NULL);
#endif
    itm++;
    num--;
  }
  Nlm_RestorePort (tempPort);
}


static void Nlm_EnablePopListChoice(Nlm_GraphiC c, Nlm_Boolean setFlag,
                                  Nlm_Boolean savePort)
{
  Nlm_EnableChoice(c, setFlag, savePort);
  Nlm_DoEnable(Nlm_GetParent(c), setFlag, savePort);
}


static void Nlm_EnableChoiceGroup (Nlm_GraphiC c, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_MenuTool  h;
  Nlm_Int2      itm;
  Nlm_MenU      m;
  Nlm_Int2      num;
  Nlm_WindoW    tempPort;
#ifdef WIN_MSWIN
  Nlm_Int2      id;
#endif
#ifdef WIN_MOTIF
  Nlm_ItemTool  PNTR hdls;
#endif

  tempPort = Nlm_SavePortIfNeeded (c, savePort);
  Nlm_SetEnabled (c, TRUE);
  m = (Nlm_MenU) Nlm_GetParent (c);
  itm = Nlm_GetFirstItem ((Nlm_IteM) c);
  num = Nlm_GetNumItems ((Nlm_ChoicE) c);
  h = Nlm_GetMenuHandle (m);
  while (num > 0) {
#ifdef WIN_MAC
    EnableMenuItem (h, itm);
#endif
#ifdef WIN_MSWIN
    if (! Nlm_IsItAPopupList ((Nlm_ChoicE) c)) {
      id = Nlm_ItemToID (m, itm);
      EnableMenuItem (h, id, MF_ENABLED);
    }
#endif
#ifdef WIN_MOTIF
    hdls = Nlm_GetChoiceHandles ((Nlm_ChoicE) c);
    XtVaSetValues (hdls [num - 1], XmNsensitive, TRUE, NULL);
#endif
    itm++;
    num--;
  }
  Nlm_RestorePort (tempPort);
}

static void Nlm_EnableChoiceItem (Nlm_GraphiC i, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_ChoicE    c;
  Nlm_MenuTool  h;
  Nlm_Int2      index;
  Nlm_Int2      itm;
  Nlm_MenU      m;
  Nlm_WindoW    tempPort;
#ifdef WIN_MSWIN
  Nlm_Int2      id;
#endif
#ifdef WIN_MOTIF
  Nlm_ItemTool  PNTR hdls;
#endif

  tempPort = Nlm_SavePortIfNeeded (i, savePort);
  Nlm_SetEnabled (i, TRUE);
  c = (Nlm_ChoicE) Nlm_GetParent (i);
  index = Nlm_GetFirstItem ((Nlm_IteM) i);
  if (index > 0 && index <= Nlm_GetNumItems ((Nlm_ChoicE) c)) {
    m = (Nlm_MenU) Nlm_GetParent ((Nlm_GraphiC) c);
    h = Nlm_GetMenuHandle (m);
    itm = (Nlm_Int2)(Nlm_GetFirstItem( (Nlm_IteM)c ) + index - 1);
#ifdef WIN_MAC
    EnableMenuItem (h, itm);
#endif
#ifdef WIN_MSWIN
    if (! Nlm_IsItAPopupList ((Nlm_ChoicE) c)) {
      id = Nlm_ItemToID (m, itm);
      EnableMenuItem (h, id, MF_ENABLED);
    }
#endif
#ifdef WIN_MOTIF
    hdls = Nlm_GetChoiceHandles ((Nlm_ChoicE) c);
    XtVaSetValues (hdls [index - 1], XmNsensitive, TRUE, NULL);
#endif
  }
  Nlm_RestorePort (tempPort);
}

#ifdef WIN_MAC
static void Nlm_DisableDesktopMenu (Nlm_GraphiC m, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_MenuTool  h;
  Nlm_WindoW    tempPort;

  tempPort = Nlm_SavePortIfNeeded (m, savePort);
  Nlm_SetEnabled (m, FALSE);
  h = Nlm_GetMenuHandle ((Nlm_MenU) m);
  DisableMenuItem (h, 0);
  if (Nlm_GetVisible (m)) {
    DrawMenuBar ();
  }
  Nlm_RestorePort (tempPort);
}
#endif

static void Nlm_DisablePulldown (Nlm_GraphiC m, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  Nlm_MenuTool  h;
  Nlm_PrompT    p;
  Nlm_WindoW    tempPort;

  if (setFlag) {
    Nlm_SetEnabled (m, FALSE);
  }
  if (Nlm_GetVisible (m) && Nlm_GetAllParentsVisible (m)) {
    tempPort = Nlm_SavePortIfNeeded (m, savePort);
    h = Nlm_GetMenuHandle ((Nlm_MenU) m);
    DisableMenuItem (h, 0);
    Nlm_DoDraw (m);
    p = Nlm_GetMenuPrompt ((Nlm_MenU) m);
    Nlm_DoDisable ((Nlm_GraphiC) p, TRUE, FALSE);
    Nlm_RestorePort (tempPort);
  }

#else

  if ( setFlag )
    Nlm_SetEnabled(m, FALSE);

  if (Nlm_GetVisible (m) && Nlm_GetAllParentsVisible (m))
    {
      Nlm_WindoW tempPort = Nlm_SavePortIfNeeded (m, savePort);
#ifdef WIN_MSWIN
      Nlm_GraphiC g = Nlm_GetChild (m);
      while (g != NULL) {
        Nlm_DoDisable (g, FALSE, FALSE);
        g = Nlm_GetNext (g);
      }
      DrawMenuBar( Nlm_ParentWindowPtr(m) );
#endif
#ifdef WIN_MOTIF
      Nlm_PopupTool u = Nlm_GetMenuPopup ((Nlm_MenU) m);
      if (u != NULL)
        XtVaSetValues (u, XmNsensitive, FALSE, NULL);
#endif
      Nlm_RestorePort (tempPort);
    }
#endif
}

static void Nlm_DisablePopup (Nlm_GraphiC m, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  Nlm_MenuTool  h;
  Nlm_WindoW    tempPort;

  if (setFlag) {
    Nlm_SetEnabled (m, FALSE);
  }
  if (Nlm_GetVisible (m) && Nlm_GetAllParentsVisible (m)) {
    tempPort = Nlm_SavePortIfNeeded (m, savePort);
    h = Nlm_GetMenuHandle ((Nlm_MenU) m);
    DisableMenuItem (h, 0);
    Nlm_DoDraw (m);
    Nlm_RestorePort (tempPort);
  }
#endif
}

static void Nlm_DisablePopList (Nlm_GraphiC m, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_WindoW     tempPort;
#ifdef WIN_MAC
  Nlm_MenuTool   h;
#endif
#ifdef WIN_MSWIN
  Nlm_PopupTool  u;
#endif
#ifdef WIN_MOTIF
    Nlm_PopupTool  c;
#endif

  if (setFlag) {
    Nlm_SetEnabled (m, FALSE);
  }
  tempPort = Nlm_SavePortIfNeeded (m, savePort);
#ifdef WIN_MAC
  if (Nlm_GetVisible (m) && Nlm_GetAllParentsVisible (m)) {
    h = Nlm_GetMenuHandle ((Nlm_MenU) m);
    DisableMenuItem (h, 0);
    Nlm_DoDraw (m);
  }
#endif
#ifdef WIN_MSWIN
  u = Nlm_GetMenuPopup ((Nlm_MenU) m);
  if (u != NULL) {
    EnableWindow (u, FALSE);
  }
#endif
#ifdef WIN_MOTIF
  c = Nlm_GetMenuPopup ((Nlm_MenU) m); 
  XtSetSensitive(XmOptionButtonGadget(c), FALSE);
  XtVaSetValues(c, XmNpopupEnabled, FALSE, NULL);
#endif
  Nlm_RestorePort (tempPort);
}

static void Nlm_DisableMenuItem (Nlm_GraphiC i, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_MenuTool  h;
  Nlm_Int2      itm;
  Nlm_MenU      m;
  Nlm_WindoW    tempPort;
#ifdef WIN_MSWIN
  Nlm_Int2      id;
#endif
#ifdef WIN_MOTIF
  Nlm_ItemTool  itool;
#endif

  tempPort = Nlm_SavePortIfNeeded (i, savePort);
  Nlm_SetEnabled (i, FALSE);
  m = (Nlm_MenU) Nlm_GetParent (i);
  h = Nlm_GetMenuHandle (m);
  itm = Nlm_GetFirstItem ((Nlm_IteM) i);
#ifdef WIN_MAC
  DisableMenuItem (h, itm);
#endif
#ifdef WIN_MSWIN
  id = Nlm_ItemToID (m, itm);
  EnableMenuItem (h, id, MF_GRAYED);
#endif
#ifdef WIN_MOTIF
  itool = Nlm_GetItemHandle ((Nlm_IteM) i);
  XtVaSetValues (itool, XmNsensitive, FALSE, NULL);
#endif
  Nlm_RestorePort (tempPort);
}


static void Nlm_DisableSubMenu (Nlm_GraphiC m, Nlm_Boolean setFlag, Nlm_Boolean savePort)
{
  Nlm_WindoW tempPort = Nlm_SavePortIfNeeded(m, savePort);
  Nlm_IteM   i        = (Nlm_IteM)Nlm_GetParent( m );

  Nlm_SetEnabled(m, FALSE);

  if (i != NULL)
    {
#ifdef WIN_MAC
      Nlm_Int2 tag;
      Nlm_DisableMenuItem ((Nlm_GraphiC) i, FALSE, FALSE);
      tag = Nlm_GetMenuTag ((Nlm_MenU) m);
      DeleteMenu (tag);
#endif
#ifdef WIN_MSWIN
      Nlm_MenU     prnt   = (Nlm_MenU)Nlm_GetParent( (Nlm_GraphiC)i);
      Nlm_MenuTool prnt_h = Nlm_GetMenuHandle( prnt );
      Nlm_Int2     pos    = Nlm_GetChildPosition( (Nlm_GraphiC)i );
      EnableMenuItem(prnt_h, pos-1, MF_BYPOSITION|MF_GRAYED);
#endif
#ifdef WIN_MOTIF
      Nlm_PopupTool  pop;
      Nlm_DisableMenuItem ((Nlm_GraphiC) i, FALSE, FALSE);
      pop = Nlm_GetMenuPopup ((Nlm_MenU) m);
      XtVaSetValues (pop, XmNsensitive, FALSE, NULL);
#endif
      Nlm_DeactivateInnerMenus ((Nlm_MenU) m);
    }
  Nlm_RestorePort( tempPort );
}


static void Nlm_DisableChoice (Nlm_GraphiC c, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_MenuTool  h;
  Nlm_Int2      itm;
  Nlm_MenU      m;
  Nlm_Int2      num;
  Nlm_WindoW    tempPort;
#ifdef WIN_MSWIN
  Nlm_Int2      id;
#endif
#ifdef WIN_MOTIF
  Nlm_ItemTool  PNTR hdls;
#endif

  tempPort = Nlm_SavePortIfNeeded (c, savePort);
  Nlm_SetEnabled (c, FALSE);
  m = (Nlm_MenU) Nlm_GetParent (c);
  itm = Nlm_GetFirstItem ((Nlm_IteM) c);
  num = Nlm_GetNumItems ((Nlm_ChoicE) c);
  h = Nlm_GetMenuHandle (m);
  while (num > 0) {
#ifdef WIN_MAC
    DisableMenuItem (h, itm);
#endif
#ifdef WIN_MSWIN
    if (! Nlm_IsItAPopupList ((Nlm_ChoicE) c)) {
      id = Nlm_ItemToID (m, itm);
      EnableMenuItem (h, id, MF_GRAYED);
    }
#endif
#ifdef WIN_MOTIF
    hdls = Nlm_GetChoiceHandles ((Nlm_ChoicE) c);
    XtVaSetValues (hdls [num - 1], XmNsensitive, FALSE, NULL);
#endif
    itm++;
    num--;
  }
  Nlm_RestorePort (tempPort);
}


static void Nlm_DisablePopListChoice(Nlm_GraphiC c, Nlm_Boolean setFlag,
                                     Nlm_Boolean savePort)
{
  Nlm_DisableChoice(c, setFlag, savePort);
  Nlm_DoDisable(Nlm_GetParent(c), setFlag, savePort);
}

#ifdef NOT_USED_XXXXX
static void Nlm_DisableChoiceGroup (Nlm_GraphiC c, Nlm_Boolean setFlag, Nlm_Boolean savePort)
{
  Nlm_MenuTool  h;
  Nlm_Int2      itm;
  Nlm_MenU      m;
  Nlm_Int2      num;
  Nlm_WindoW    tempPort;
#ifdef WIN_MSWIN
  Nlm_Int2      id;
#endif
#ifdef WIN_MOTIF
  Nlm_ItemTool  PNTR hdls;
#endif

  tempPort = Nlm_SavePortIfNeeded (c, savePort);
  Nlm_SetEnabled (c, FALSE);
  m = (Nlm_MenU) Nlm_GetParent (c);
  itm = Nlm_GetFirstItem ((Nlm_IteM) c);
  num = Nlm_GetNumItems ((Nlm_ChoicE) c);
  h = Nlm_GetMenuHandle (m);
  while (num > 0) {
#ifdef WIN_MAC
    DisableMenuItem (h, itm);
#endif
#ifdef WIN_MSWIN
    if (! Nlm_IsItAPopupList ((Nlm_ChoicE) c)) {
      id = Nlm_ItemToID (m, itm);
      EnableMenuItem (h, id, MF_GRAYED);
    }
#endif
#ifdef WIN_MOTIF
    hdls = Nlm_GetChoiceHandles ((Nlm_ChoicE) c);
    XtVaSetValues (hdls [num - 1], XmNsensitive, FALSE, NULL);
#endif
    itm++;
    num--;
  }
  Nlm_RestorePort (tempPort);
}
#endif /* NOT_USED_XXXXX */

static void Nlm_DisableChoiceItem (Nlm_GraphiC i, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_ChoicE    c;
  Nlm_MenuTool  h;
  Nlm_Int2      index;
  Nlm_Int2      itm;
  Nlm_MenU      m;
  Nlm_WindoW    tempPort;
#ifdef WIN_MSWIN
  Nlm_Int2      id;
#endif
#ifdef WIN_MOTIF
  Nlm_ItemTool  PNTR hdls;
#endif

  tempPort = Nlm_SavePortIfNeeded (i, savePort);
  Nlm_SetEnabled (i, FALSE);
  c = (Nlm_ChoicE) Nlm_GetParent (i);
  index = Nlm_GetFirstItem ((Nlm_IteM) i);
  if (index > 0 && index <= Nlm_GetNumItems ((Nlm_ChoicE) c)) {
    m = (Nlm_MenU) Nlm_GetParent ((Nlm_GraphiC) c);
    h = Nlm_GetMenuHandle (m);
    itm = (Nlm_Int2)(Nlm_GetFirstItem( (Nlm_IteM)c ) + index - 1);
#ifdef WIN_MAC
    DisableMenuItem (h, itm);
#endif
#ifdef WIN_MSWIN
    if (! Nlm_IsItAPopupList ((Nlm_ChoicE) c)) {
      id = Nlm_ItemToID (m, itm);
      EnableMenuItem (h, id, MF_GRAYED);
    }
#endif
#ifdef WIN_MOTIF
    hdls = Nlm_GetChoiceHandles ((Nlm_ChoicE) c);
    XtVaSetValues (hdls [index - 1], XmNsensitive, FALSE, NULL);
#endif
  }
  Nlm_RestorePort (tempPort);
}

#ifdef WIN_MAC
static void Nlm_RemoveDesktopMenuBar (Nlm_GraphiC mb, Nlm_Boolean savePort)

/*
*  A submenu is linked into the desktop menu bar immediately after the menu
*  that contains it is created.  Thus, removal of desktop menus cannot safely
*  get the next child in the list of menus right before removing a link, since
*  that next child, if a submenu, will immediately become a dangling pointer.
*  Removal of desktop menus must instead repeatedly remove the first child of
*  the desktop menu bar.
*/

{
  Nlm_MenU    m;
  Nlm_WindoW  tempPort;

  tempPort = Nlm_SavePortIfNeeded (mb, savePort);
  m = (Nlm_MenU) Nlm_GetChild (mb);
  while (m != NULL) {
    Nlm_DeactivateInnerMenus (m);
    Nlm_DoRemove ((Nlm_GraphiC) m, FALSE);
    m = (Nlm_MenU) Nlm_GetChild (mb);
  }
  Nlm_HandFree (mb);
  recentMenuBar = NULL;
  Nlm_RestorePort (tempPort);
}
#endif

static void Nlm_RemoveWindowMenuBar (Nlm_GraphiC mb, Nlm_Boolean savePort)

{
  Nlm_MenU      m;
  Nlm_MenU      n;
  Nlm_WindoW    tempPort;
  Nlm_WindoW    w;
#ifdef WIN_MSWIN
  Nlm_MenuTool  h;
#endif
#ifdef WIN_MOTIF
  Nlm_MenuTool  h;
#endif

  tempPort = Nlm_SavePortIfNeeded (mb, savePort);
  m = (Nlm_MenU) Nlm_GetChild (mb);
  while (m != NULL) {
    n = (Nlm_MenU) Nlm_GetNext ((Nlm_GraphiC) m);
    Nlm_DeactivateInnerMenus (m);
    m = n;
  }
  m = (Nlm_MenU) Nlm_GetChild (mb);
  while (m != NULL) {
    n = (Nlm_MenU) Nlm_GetNext ((Nlm_GraphiC) m);
    Nlm_DoRemove ((Nlm_GraphiC) m, FALSE);
    m = n;
  }
  w = Nlm_GetParentWindow ((Nlm_GraphiC) mb);
  Nlm_SetWindowMenuBar (w, NULL);
#ifdef WIN_MSWIN
  h = Nlm_GetMenuBarHandle ((Nlm_MenuBaR) mb);
  DestroyMenu (h);
#endif
#ifdef WIN_MOTIF
  h = Nlm_GetMenuBarHandle ((Nlm_MenuBaR) mb);
  XtUnrealizeWidget(h);
#ifndef NLM_MOTIF_CASCADEB_BUG
  XtDestroyWidget( h );
#endif
#endif
  Nlm_HandFree (mb);
  recentMenuBar = NULL;
  Nlm_RestorePort (tempPort);
}

#ifdef WIN_MAC
static void Nlm_RemoveDesktopMenu (Nlm_GraphiC m, Nlm_Boolean savePort)

{
  Nlm_IteM      i;
  Nlm_MenuTool  h;
  Nlm_IteM      n;
  Nlm_WindoW    tempPort;

  tempPort = Nlm_SavePortIfNeeded (m, savePort);
  i = (Nlm_IteM) Nlm_GetChild (m);
  while (i != NULL) {
    n = (Nlm_IteM) Nlm_GetNext ((Nlm_GraphiC) i);
    Nlm_DoRemove ((Nlm_GraphiC) i, FALSE);
    i = n;
  }
  h = Nlm_GetMenuHandle ((Nlm_MenU) m);
  DisposeMenu (h);
  DrawMenuBar ();
  Nlm_RemoveLink (m);
  recentMenu = NULL;
  Nlm_RestorePort (tempPort);
}
#endif

static void Nlm_RemoveSubMenu (Nlm_GraphiC m, Nlm_Boolean savePort)

{
  Nlm_IteM         i;
  Nlm_MenuTool     h;
  Nlm_IteM         n;
  Nlm_WindoW       tempPort;
#ifdef WIN_MAC
  Nlm_MenuBaR      mb;
#else
  Nlm_GraphicData  gdata;
#endif

  tempPort = Nlm_SavePortIfNeeded (m, savePort);
  Nlm_DoDisable (m, TRUE, FALSE);
  i = (Nlm_IteM) Nlm_GetChild (m);
  while (i != NULL) {
    n = (Nlm_IteM) Nlm_GetNext ((Nlm_GraphiC) i);
    Nlm_DoRemove ((Nlm_GraphiC) i, FALSE);
    i = n;
  }
  h = Nlm_GetMenuHandle ((Nlm_MenU) m);
#ifdef WIN_MAC
  DisposeMenu (h);
#endif
#ifdef WIN_MSWIN
  DestroyMenu (h);
#endif

  i = (Nlm_IteM) Nlm_GetParent (m);
  Nlm_SetSubMenu (i, NULL);
#ifdef WIN_MAC
  mb = Nlm_GetWindowMenuBar (Nlm_desktopWindow);
  Nlm_SetParent (m, (Nlm_GraphiC) mb);
  Nlm_RemoveLink (m);
#else
  Nlm_GetGraphicData (m, &gdata);
  if (gdata.data != NULL && gdata.cleanup != NULL) {
    gdata.cleanup (m, gdata.data);
  }
  Nlm_HandFree (m);
#endif
  recentMenu = NULL;
  Nlm_RestorePort (tempPort);
}

static void Nlm_RemovePopupMenu (Nlm_GraphiC m, Nlm_Boolean savePort)

{
  Nlm_IteM       i;
  Nlm_IteM       n;
  Nlm_PrompT     p;
  Nlm_WindoW     tempPort;
#ifdef WIN_MAC
  Nlm_MenuTool   h;
#endif
#ifdef WIN_MSWIN
  Nlm_PopupTool  u;
#endif
#ifdef WIN_MOTIF
  Nlm_MenuTool   h;
  Nlm_PopupTool  pop;
#endif

  tempPort = Nlm_SavePortIfNeeded (m, savePort);
  i = (Nlm_IteM) Nlm_GetChild (m);
  while (i != NULL) {
    n = (Nlm_IteM) Nlm_GetNext ((Nlm_GraphiC) i);
    Nlm_DoRemove ((Nlm_GraphiC) i, FALSE);
    i = n;
  }
#ifdef WIN_MAC
  h = Nlm_GetMenuHandle ((Nlm_MenU) m);
  DisposeMenu (h);
#endif
#ifdef WIN_MSWIN
  u = Nlm_GetMenuPopup ((Nlm_MenU) m);
  if (u != NULL) { /* Floating Popup [see NewFloatingPopup()] */
    ASSERT ( Nlm_GetMenuHandle( (Nlm_MenU)m ) == NULL );
    RemoveProp (u, (LPSTR) "Nlm_VibrantProp");
    DestroyWindow (u);
  }
  else { /* Pulldown [see NewPopup()] */
    Nlm_MenuBaR  mb  = (Nlm_MenuBaR)Nlm_GetParent( m );
    Nlm_MenuTool mbh = Nlm_GetMenuBarHandle( mb );
    Nlm_Int2     pos = Nlm_GetChildPosition( m );
    VERIFY ( DeleteMenu(mbh, pos-1, MF_BYPOSITION) );
    DrawMenuBar( Nlm_ParentWindowPtr((Nlm_GraphiC)mb) );
  }
#endif
#ifdef WIN_MOTIF
  h = Nlm_GetMenuHandle ((Nlm_MenU) m);
  if (h != NULL) {
    XtUnrealizeWidget(h);
    /*
      This code will, for some unknown reason, crash on the next
      update if you run a program from outside the window manager.
    */
    /*
    XtDestroyWidget (h);
    */
  }
  pop = Nlm_GetMenuPopup ((Nlm_MenU) m);
  if (pop != NULL) {
#ifdef NLM_MOTIF_CASCADEB_BUG
    XtUnmanageChild( pop );
#else
    XtDestroyWidget( pop );
#endif
  }
#endif
  p = Nlm_GetMenuPrompt ((Nlm_MenU) m);
  if (p != NULL) {
    Nlm_DoRemove ((Nlm_GraphiC) p, FALSE);
  }
  Nlm_RemoveLink (m);
  recentMenu = NULL;
  Nlm_RestorePort (tempPort);
}

static void Nlm_RemovePopListMenu (Nlm_GraphiC m, Nlm_Boolean savePort)

{
  Nlm_IteM       i;
  Nlm_IteM       n;
  Nlm_PrompT     p;
  Nlm_WindoW     tempPort;
#ifdef WIN_MAC
  Nlm_MenuTool   h;
#endif
#ifdef WIN_MSWIN
  Nlm_PopupTool  u;
#endif

  tempPort = Nlm_SavePortIfNeeded (m, savePort);
  i = (Nlm_IteM) Nlm_GetChild (m);
  while (i != NULL) {
    n = (Nlm_IteM) Nlm_GetNext ((Nlm_GraphiC) i);
    Nlm_DoRemove ((Nlm_GraphiC) i, FALSE);
    i = n;
  }
#ifdef WIN_MAC
  h = Nlm_GetMenuHandle ((Nlm_MenU) m);
  DisposeMenu (h);
#endif
#ifdef WIN_MSWIN
  u = Nlm_GetMenuPopup ((Nlm_MenU) m);
  if (u != NULL) {
    RemoveProp (u, (LPSTR) "Nlm_VibrantProp");
    DestroyWindow (u);
  }
#endif
#ifdef WIN_MOTIF
#endif
  p = Nlm_GetMenuPrompt ((Nlm_MenU) m);
  if (p != NULL) {
    Nlm_DoRemove ((Nlm_GraphiC) p, FALSE);
  }
  Nlm_RemoveLink (m);
  recentMenu = NULL;
  Nlm_RestorePort (tempPort);
}


#ifdef WIN_MOTIF
static void MenuShellColormapCB(Widget w, XtPointer client_data,
				XEvent *event, Boolean *cont)
{
  Colormap cMap;
  if ( !Nlm_GetFixedColormap((Nlm_GraphiC)(client_data), &cMap, NULL) )
    return;

  if (event->type               == ColormapNotify  &&
      event->xcolormap.state    == ColormapUninstalled  &&
      event->xcolormap.colormap != None  &&
      w != NULL  &&  XtIsManaged(w)  &&
      event->xcolormap.window   == XtWindow(w))
    XInstallColormap(Nlm_currentXDisplay, cMap);
}
#endif


static void Nlm_RemovePopupList (Nlm_GraphiC c, Nlm_Boolean savePort)
/*
*  If the user calls Remove (c) to remove the popup list, then
*  savePort will be TRUE, and the implicitly created parent menu
*  is also removed.  If the user calls Remove (w) to remove the
*  parent window, then RemovePopupList will be called by the
*  implicitly created parent menu just prior to its removal, and
*  an additional attempt to remove the menu is both unnecessary
*  and dangerous.
*/
{
  Nlm_WindoW  tempPort = Nlm_SavePortIfNeeded(c, savePort);
  Nlm_MenU    m        = (Nlm_MenU)Nlm_GetParent( c );

#ifdef WIN_MOTIF
  Nlm_ItemTool  PNTR hdls;
  XtRemoveEventHandler(Nlm_GetMenuHandle( m ), ColormapChangeMask, FALSE,
                       MenuShellColormapCB, (XtPointer)m);
  hdls = Nlm_GetChoiceHandles ((Nlm_ChoicE) c);
  if (hdls != NULL) {
    Nlm_MemFree (hdls);
  }
#endif
  Nlm_RemoveLink (c);
  recentItem = NULL;
  recentChoice = NULL;
  if (savePort) {
    Nlm_DoRemove ((Nlm_GraphiC) m, FALSE);
  }
  Nlm_RestorePort (tempPort);
}

static void Nlm_RemoveMenuItem (Nlm_GraphiC i, Nlm_Boolean savePort)

{
  Nlm_MenU    sub;
  Nlm_WindoW  tempPort;

  tempPort = Nlm_SavePortIfNeeded (i, savePort);
  sub = Nlm_GetSubMenu ((Nlm_IteM) i);
  if (sub != NULL) {
    Nlm_DoRemove ((Nlm_GraphiC) sub, FALSE);
  }
  Nlm_RemoveLink (i);
  recentItem = NULL;
  recentChoice = NULL;
  Nlm_RestorePort (tempPort);
}

#ifdef WIN_MAC
static void Nlm_RemoveMenuItemButNotSub (Nlm_GraphiC i, Nlm_Boolean savePort)

{
  Nlm_MenU    sub;
  Nlm_WindoW  tempPort;

  tempPort = Nlm_SavePortIfNeeded (i, savePort);
  sub = Nlm_GetSubMenu ((Nlm_IteM) i);
  if (sub != NULL) {
    Nlm_SetSubMenu ((Nlm_IteM) i, NULL);
    /*Nlm_SetParent ((Nlm_GraphiC) sub, NULL);*/
    Nlm_DoRemove ((Nlm_GraphiC) sub, FALSE);
  }
  Nlm_RemoveLink (i);
  recentItem = NULL;
  recentChoice = NULL;
  Nlm_RestorePort (tempPort);
}
#endif

static void Nlm_RemoveChoiceGroup (Nlm_GraphiC c, Nlm_Boolean savePort)

{
  Nlm_IteM    i;
  Nlm_IteM    n;
  Nlm_MenU    sub;
  Nlm_WindoW  tempPort;
#ifdef WIN_MOTIF
  Nlm_ItemTool  PNTR hdls;
#endif

  tempPort = Nlm_SavePortIfNeeded (c, savePort);
  sub = Nlm_GetSubMenu ((Nlm_IteM) c);
  if (sub != NULL) {
    Nlm_DoRemove ((Nlm_GraphiC) sub, FALSE);
  }
#ifdef WIN_MOTIF
  hdls = Nlm_GetChoiceHandles ((Nlm_ChoicE) c);
  if (hdls != NULL) {
    Nlm_Int2 item = Nlm_GetNumItems( (Nlm_ChoicE)c );
    ASSERT ( item >= 0 );
    while ( item-- ) {
      if ( hdls[item] ) {
#ifdef NLM_MOTIF_CASCADEB_BUG
        if ( XmIsCascadeButton( hdls[item] ) )
          XtUnmanageChild( hdls[item] );
        else
#endif
          XtDestroyWidget( hdls[item] );
      }
    }
    Nlm_MemFree( hdls );
  }
#endif
  i = (Nlm_IteM) Nlm_GetChild (c);
  while (i != NULL) {
    n = (Nlm_IteM) Nlm_GetNext ((Nlm_GraphiC) i);
    Nlm_DoRemove ((Nlm_GraphiC) i, FALSE);
    i = n;
  }
  Nlm_RemoveLink (c);
  recentItem = NULL;
  recentChoice = NULL;
  Nlm_RestorePort (tempPort);
}

static void Nlm_RemoveChoiceItem (Nlm_GraphiC i, Nlm_Boolean savePort)

{
  Nlm_WindoW  tempPort;

  tempPort = Nlm_SavePortIfNeeded (i, savePort);
  Nlm_RemoveLink (i);
  recentItem = NULL;
  recentChoice = NULL;
  Nlm_RestorePort (tempPort);
}


static void Nlm_ResetMenu (Nlm_GraphiC m, Nlm_Boolean savePort)
{
  Nlm_MenuTool h        = Nlm_GetMenuHandle( (Nlm_MenU)m );
  Nlm_WindoW   tempPort = Nlm_SavePortIfNeeded(m, savePort);

  {{ /* reset the Vibrant menu structure */
    Nlm_GraphiC  item = Nlm_GetChild( m );
    while ( item ) {
      Nlm_GraphiC next_item = Nlm_GetNext( item );
      Nlm_DoRemove(item, FALSE);
      item = next_item;
    }
  }}

  {{ /* reset platform-dependent menu structures */
#if   defined(WIN_MAC)
    Nlm_Int2 cnt = CountMenuItems( h );
    while (cnt-- > 0)
      DeleteMenuItem(h, cnt);
#elif defined(WIN_MSWIN)
    Nlm_WindowTool wptr = Nlm_ParentWindowPtr( m );
    int cnt = GetMenuItemCount( h );
    while (cnt-- > 0)
      DeleteMenu(h, cnt, MF_BYPOSITION);
    DrawMenuBar( wptr );
#elif defined(WIN_MOTIF)
    Cardinal   cnt;
    WidgetList children;
    XtVaGetValues(h,  XtNnumChildren, &cnt,  XtNchildren, &children,  NULL);
    if ( children ) {
      while (cnt-- > 0) {
        ASSERT ( children[cnt] );
#ifdef NLM_MOTIF_CASCADEB_BUG
        if ( XmIsCascadeButton( children[cnt] ) )
          XtUnmanageChild( children[cnt] );
        else
#endif
          XtDestroyWidget( children[cnt] );
      }
    }
#endif
  }}

  Nlm_RestorePort( tempPort );
}


#ifndef WIN_MOTIF
static void Nlm_ResetPopList (Nlm_GraphiC c, Nlm_Boolean savePort)

{
  Nlm_Int2        hbounds;
  Nlm_MenU        m;
  Nlm_RecT        r;
  Nlm_WindoW      tempPort;
#ifdef WIN_MAC
  Nlm_Int2        cnt;
  Nlm_MenuTool    h;
#endif
#ifdef WIN_MSWIN
  Nlm_PopupTool   u;
#endif

  tempPort = Nlm_SavePortIfNeeded (c, savePort);
  m = (Nlm_MenU) Nlm_GetParent (c);
#ifdef WIN_MAC
  h = Nlm_GetMenuHandle (m);
  cnt = CountMenuItems (h);
  while (cnt > 0) {
    DeleteMenuItem (h, cnt);
    cnt--;
  }
#endif
#ifdef WIN_MSWIN
  u = Nlm_GetMenuPopup (m);
  if (u != NULL) {
    ComboBox_ResetContent (u);
  }
#endif
  Nlm_LoadItemData ((Nlm_IteM) c, NULL, 1, NULL);
  Nlm_LoadChoiceData ((Nlm_ChoicE) c, 0, TRUE, 0, NULL);
#ifdef WIN_MAC
  hbounds = 13;
#endif
#ifdef WIN_MSWIN
  hbounds = 13;
#endif
  Nlm_GetRect ((Nlm_GraphiC) m, &r);
  r.right  = (Nlm_Int2)(r.left + hbounds * 2);
  r.bottom = (Nlm_Int2)(r.top + Nlm_popupMenuHeight);
  Nlm_DoSetPosition ((Nlm_GraphiC) c, &r, FALSE, FALSE);
  Nlm_RestorePort (tempPort);
}
#endif


#ifdef WIN_MOTIF
static void Nlm_ResetPopList (Nlm_GraphiC c, Nlm_Boolean savePort)
{
  Nlm_ItemTool     *hdls = Nlm_GetChoiceHandles( (Nlm_ChoicE)c );
  Nlm_MenU         m     = (Nlm_MenU)Nlm_GetParent( c );
  Nlm_PopupTool    pop   = Nlm_GetMenuPopup( m );
  Nlm_Int2         i;

  Nlm_DoSetValue(c, 0, FALSE);
  for (i = 0;  i < 63;  i++)
    {
      if (hdls[i] != NULL)
	{
	  XtDestroyWidget( hdls[i] );
	  hdls[i] = NULL;
	}
    }

  Nlm_LoadItemData((Nlm_IteM)c, m, 1, pop);
  Nlm_LoadChoiceData((Nlm_ChoicE)c, 0, TRUE, 0, hdls);
}
#endif

static Nlm_Int2 Nlm_CountMenuItems (Nlm_GraphiC m)

{
  Nlm_MenuTool  h;
  Nlm_Int2      rsult;

  rsult = 0;
  h = Nlm_GetMenuHandle ((Nlm_MenU) m);
#ifdef WIN_MAC
  rsult = CountMenuItems (h);
#endif
#ifdef WIN_MSWIN
  rsult = (Nlm_Int2)GetMenuItemCount( h );
#endif
#ifdef WIN_MOTIF
#endif
  return rsult;
}

static Nlm_Int2 Nlm_CountChoiceItems (Nlm_GraphiC c)

{
  return (Nlm_GetNumItems ((Nlm_ChoicE) c));
}

static void Nlm_SetMenuItem (Nlm_MenU m, Nlm_ItemTool itool,
			     Nlm_Int2 item, Nlm_CharPtr itemName)

{
#ifdef WIN_MAC
  Nlm_MenuTool  h;
  Nlm_Char      temp [256];

  h = Nlm_GetMenuHandle ((Nlm_MenU) m);
  if (item > 0) {
    if (Nlm_StringLen (itemName) == 0) {
      temp [0] = '(';
      temp [1] = '\0';
      Nlm_CtoPstr (temp);
      SetMenuItemText (h, item, (StringPtr) temp);
    } else {
      Nlm_StringNCpy_0(temp, itemName, sizeof(temp));
      Nlm_CtoPstr (temp);
      SetMenuItemText (h, item, (StringPtr) temp);
    }
  }
#endif
#ifdef WIN_MSWIN
  Nlm_MenuTool    h;
  Nlm_Int2        id;
  Nlm_Char        temp [256];
  Nlm_WindowTool  wptr;

  h = Nlm_GetMenuHandle ((Nlm_MenU) m);
  id = Nlm_ItemToID ((Nlm_MenU) m, item);
  Nlm_StringNCpy_0(temp, itemName, sizeof(temp));
  ModifyMenu (h, id, /*MF_CHANGE |*/ MF_BYCOMMAND | MF_STRING, item, temp);
  wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) m);
  DrawMenuBar (wptr);
#endif
#ifdef WIN_MOTIF
  XmString  label;
  Nlm_Char  temp [256];

  if (itool != NULL) {
    Nlm_StringNCpy_0(temp, itemName, sizeof(temp));
    label = XmStringCreateSimple (temp);
    XtVaSetValues (itool, XmNlabelString, label, NULL);
    XmStringFree (label);
  }
#endif
}

static void Nlm_SetSingleTitle (Nlm_GraphiC i, Nlm_Int2 item,
				Nlm_CharPtr title, Nlm_Boolean savePort)

{
  Nlm_Int2      itm;
  Nlm_ItemTool  itool;
  Nlm_MenU      m;
  Nlm_WindoW    tempPort;

  tempPort = Nlm_SavePortIfNeeded (i, savePort);
  m = (Nlm_MenU) Nlm_GetParent (i);
  itm = Nlm_GetFirstItem ((Nlm_IteM) i);
  itool = Nlm_GetItemHandle ((Nlm_IteM) i);
  Nlm_SetMenuItem (m, itool, itm, title);
  Nlm_RestorePort (tempPort);
}

static void Nlm_SetChoiceTitle (Nlm_GraphiC c, Nlm_Int2 item,
				Nlm_CharPtr title, Nlm_Boolean savePort)

{
  Nlm_Int2      first;
  Nlm_ItemTool  itool;
  Nlm_MenU      m;
  Nlm_Int2      num;
  Nlm_PrompT    p;
  Nlm_WindoW    tempPort;

  tempPort = Nlm_SavePortIfNeeded (c, savePort);
  first = Nlm_GetFirstItem ((Nlm_IteM) c);
  num = Nlm_GetNumItems ((Nlm_ChoicE) c);
  m = (Nlm_MenU) Nlm_GetParent (c);
  itool = NULL;
  if (item > 0 && item <= num) {
    Nlm_SetMenuItem (m, itool, (Nlm_Int2)(first + item - 1), title);
  } else if (item == 0) {
    p = Nlm_GetMenuPrompt (m);
    if (p != NULL) {
      Nlm_DoSetTitle ((Nlm_GraphiC) p, 0, title, FALSE);
    }
  }
  Nlm_RestorePort (tempPort);
}

static void Nlm_SetChoiceGroupTitle (Nlm_GraphiC c, Nlm_Int2 item,
				     Nlm_CharPtr title, Nlm_Boolean savePort)

{
  Nlm_MenU    m;
  Nlm_PrompT  p;
  Nlm_WindoW  tempPort;

  tempPort = Nlm_SavePortIfNeeded (c, savePort);
  m = (Nlm_MenU) Nlm_GetParent (c);
  p = Nlm_GetMenuPrompt (m);
  if (p != NULL) {
    Nlm_DoSetTitle ((Nlm_GraphiC) p, 0, title, FALSE);
  }
  Nlm_RestorePort (tempPort);
}

static void Nlm_SetChoiceItemTitle (Nlm_GraphiC i, Nlm_Int2 item,
				    Nlm_CharPtr title, Nlm_Boolean savePort)

{
  Nlm_ChoicE    c;
  Nlm_Int2      first;
  Nlm_Int2      index;
  Nlm_ItemTool  itool;
  Nlm_MenU      m;
  Nlm_WindoW    tempPort;

  tempPort = Nlm_SavePortIfNeeded (i, savePort);
  c = (Nlm_ChoicE) Nlm_GetParent (i);
  index = Nlm_GetFirstItem ((Nlm_IteM) i);
  first = Nlm_GetFirstItem ((Nlm_IteM) c);
  m = (Nlm_MenU) Nlm_GetParent ((Nlm_GraphiC) c);
  itool = NULL;
  Nlm_SetMenuItem (m, itool, (Nlm_Int2)(first + index - 1), title);
  Nlm_RestorePort (tempPort);
}

static void Nlm_SetPopListTitle (Nlm_GraphiC c, Nlm_Int2 item,
				 Nlm_CharPtr title, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  Nlm_SetChoiceTitle (c, item, title, savePort);
#endif
}

static void Nlm_SetSubmenuTitle (Nlm_GraphiC m, Nlm_Int2 item,
				 Nlm_CharPtr title, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  Nlm_IteM    i;
  Nlm_WindoW  tempPort;

  tempPort = Nlm_SavePortIfNeeded (m, savePort);
  i = (Nlm_IteM) Nlm_GetParent (m);
  if (i != NULL) {
    Nlm_SetSingleTitle ((Nlm_GraphiC) i, item, title, FALSE);
  }
  Nlm_RestorePort (tempPort);
#endif
}

static void Nlm_GetMenuItem (Nlm_GraphiC m, Nlm_ItemTool itool,
			     Nlm_Int2 item, Nlm_CharPtr itemName,
			     size_t maxsize)

{
  Nlm_MenuTool  h;
  Nlm_Char      temp [256];
#ifdef WIN_MSWIN
  Nlm_Int2      id;
#endif
#ifdef WIN_MOTIF
  XmString      label;
  char          *text;
#endif

  h = Nlm_GetMenuHandle ((Nlm_MenU) m);
  temp [0] = '\0';
  if (item > 0) {
#ifdef WIN_MAC
    GetMenuItemText (h, item, (StringPtr) temp);
    Nlm_PtoCstr (temp);
#endif
#ifdef WIN_MSWIN
    id = Nlm_ItemToID ((Nlm_MenU) m, item);
    GetMenuString (h, id, temp, sizeof (temp), MF_BYCOMMAND);
#endif
#ifdef WIN_MOTIF
    if (itool != NULL) {
      XtVaGetValues (itool, XmNlabelString, &label, NULL);
      if (XmStringGetLtoR (label, XmSTRING_DEFAULT_CHARSET, &text)) {
	Nlm_StringNCpy_0(temp, text, sizeof(temp));
	XtFree (text);
      }
    }
#endif
  }
  Nlm_StringNCpy_0(itemName, temp, maxsize);
}

static void Nlm_GetSingleTitle (Nlm_GraphiC i, Nlm_Int2 item,
				Nlm_CharPtr title, size_t maxsize)

{
  Nlm_Int2      itm;
  Nlm_ItemTool  itool;
  Nlm_MenU      m;

  m = (Nlm_MenU) Nlm_GetParent (i);
  itm = Nlm_GetFirstItem ((Nlm_IteM) i);
  itool = Nlm_GetItemHandle ((Nlm_IteM) i);
  Nlm_GetMenuItem ((Nlm_GraphiC) m, itool, itm, title, maxsize);
}

extern void Nlm_GetChoiceTitle (Nlm_GraphiC c, Nlm_Int2 item,
				Nlm_CharPtr title, size_t maxsize)

{
  Nlm_Int2      first;
  Nlm_ItemTool  itool;
  Nlm_MenU      m;
  Nlm_Int2      num;
  Nlm_PrompT    p;

  first = Nlm_GetFirstItem ((Nlm_IteM) c);
  num = Nlm_GetNumItems ((Nlm_ChoicE) c);
  m = (Nlm_MenU) Nlm_GetParent (c);
  itool = NULL;
  if (item > 0 && item <= num) {
    Nlm_GetMenuItem ((Nlm_GraphiC) m, itool, (Nlm_Int2)(first + item - 1),
		     title, maxsize);
  } else if (item == 0) {
    p = Nlm_GetMenuPrompt (m);
    if (p != NULL) {
      Nlm_DoGetTitle ((Nlm_GraphiC) p, 0, title, maxsize);
    }
  }
}

static void Nlm_GetChoiceGroupTitle (Nlm_GraphiC c, Nlm_Int2 item,
				     Nlm_CharPtr title, size_t maxsize)

{
  Nlm_MenU    m;
  Nlm_PrompT  p;

  m = (Nlm_MenU) Nlm_GetParent (c);
  p = Nlm_GetMenuPrompt (m);
  if (p != NULL) {
    Nlm_DoGetTitle ((Nlm_GraphiC) p, 0, title, maxsize);
  }
}

static void Nlm_GetChoiceItemTitle (Nlm_GraphiC i, Nlm_Int2 item,
				    Nlm_CharPtr title, size_t maxsize)

{
  Nlm_ChoicE    c;
  Nlm_Int2      first;
  Nlm_Int2      index;
  Nlm_ItemTool  itool;
  Nlm_MenU      m;

  c = (Nlm_ChoicE) Nlm_GetParent (i);
  index = Nlm_GetFirstItem ((Nlm_IteM) i);
  first = Nlm_GetFirstItem ((Nlm_IteM) c);
  m = (Nlm_MenU) Nlm_GetParent ((Nlm_GraphiC) c);
  itool = NULL;
  Nlm_GetMenuItem ((Nlm_GraphiC) m, itool, (Nlm_Int2)(first + index - 1),
		   title, maxsize);
}

static void Nlm_GetPopListTitle (Nlm_GraphiC c, Nlm_Int2 item,
				 Nlm_CharPtr title, size_t maxsize)

{
  Nlm_MenU       m;
#ifdef WIN_MSWIN
  Nlm_Char       temp [256];
  Nlm_PopupTool  u;
#endif

  m = (Nlm_MenU) Nlm_GetParent (c);
#ifdef WIN_MAC
  Nlm_GetChoiceTitle (c, item, title, maxsize);
#endif
#ifdef WIN_MSWIN
  u = Nlm_GetMenuPopup (m);
  if (u != NULL) {
    if (ComboBox_GetLBTextLen (u, item - 1) < sizeof (temp)) {
      ComboBox_GetLBText (u, item - 1, temp);
    } else {
      temp [0] = '\0';
    }
    Nlm_StringNCpy_0(title, temp, maxsize);
  }
#endif
#ifdef WIN_MOTIF
#endif
}

static void Nlm_GetSubmenuTitle (Nlm_GraphiC m, Nlm_Int2 item,
				 Nlm_CharPtr title, size_t maxsize)

{
#ifdef WIN_MAC
  Nlm_IteM  i;

  i = (Nlm_IteM) Nlm_GetParent (m);
  if (i != NULL) {
    Nlm_GetSingleTitle ((Nlm_GraphiC) i, item, title, maxsize);
  }
#endif
#ifdef WIN_MSWIN
  if (title != NULL && maxsize > 0) {
    title [0] = '\0';
  }
#endif
#ifdef WIN_MOTIF
  if (title != NULL && maxsize > 0) {
    title [0] = '\0';
  }
#endif
}

static void Nlm_SetStatValue (Nlm_GraphiC i, Nlm_Int2 item,
			      Nlm_Boolean value, Nlm_Boolean savePort)

{
  Nlm_Int2      itm;
  Nlm_ItemTool  itool;
  Nlm_MenU      m;
  Nlm_WindoW    tempPort;

  tempPort = Nlm_SavePortIfNeeded (i, savePort);
  m = (Nlm_MenU) Nlm_GetParent (i);
  itm = Nlm_GetFirstItem ((Nlm_IteM) i);
  itool = Nlm_GetItemHandle ((Nlm_IteM) i);
  Nlm_CheckMenuItem (m, itool, itm, value);
  Nlm_RestorePort (tempPort);
}

static void Nlm_SetChoiceValue (Nlm_GraphiC c, Nlm_Int2 value, Nlm_Boolean savePort)

{
  Nlm_Int2      first;
  Nlm_ItemTool  PNTR hdls;
  Nlm_Int2      index;
  Nlm_ItemTool  itool;
  Nlm_MenU      m;
  Nlm_Int2      num;
  Nlm_WindoW    tempPort;

  tempPort = Nlm_SavePortIfNeeded (c, savePort);
  first = Nlm_GetFirstItem ((Nlm_IteM) c);
  num = Nlm_GetNumItems ((Nlm_ChoicE) c);
  m = (Nlm_MenU) Nlm_GetParent (c);
  index = 1;
  hdls = Nlm_GetChoiceHandles ((Nlm_ChoicE) c);
  while (index <= num) {
    if (hdls != NULL) {
      itool = hdls [index - 1];
    } else {
      itool = NULL;
    }
    Nlm_CheckMenuItem (m, itool, (Nlm_Int2)(first + index - 1), FALSE);
    index++;
  }
  if (value > 0 && value <= num) {
    if (hdls != NULL) {
      itool = hdls [value - 1];
    } else {
      itool = NULL;
    }
    Nlm_CheckMenuItem (m, itool, (Nlm_Int2)(first + value - 1), TRUE);
  }
  Nlm_RestorePort (tempPort);
}

static void Nlm_SetChoiceGroupValue (Nlm_GraphiC c, Nlm_Int2 value, Nlm_Boolean savePort)

{
  Nlm_Int2      first;
  Nlm_ItemTool  PNTR hdls;
  Nlm_Int2      index;
  Nlm_ItemTool  itool;
  Nlm_MenU      m;
  Nlm_Int2      num;
  Nlm_WindoW    tempPort;

  tempPort = Nlm_SavePortIfNeeded (c, savePort);
  first = Nlm_GetFirstItem ((Nlm_IteM) c);
  num = Nlm_GetNumItems ((Nlm_ChoicE) c);
  m = (Nlm_MenU) Nlm_GetParent (c);
  index = 1;
  hdls = Nlm_GetChoiceHandles ((Nlm_ChoicE) c);
  while (index <= num) {
    if (hdls != NULL) {
      itool = hdls [index - 1];
    } else {
      itool = NULL;
    }
    Nlm_CheckMenuItem (m, itool, (Nlm_Int2)(first + index - 1), FALSE);
    index++;
  }
  if (value > 0 && value <= num) {
    if (hdls != NULL) {
      itool = hdls [value - 1];
    } else {
      itool = NULL;
    }
    Nlm_CheckMenuItem (m, itool, (Nlm_Int2)(first + value - 1), TRUE);
  }
  Nlm_RestorePort (tempPort);
}

static void Nlm_SetPopListValue (Nlm_GraphiC c, Nlm_Int2 value, Nlm_Boolean savePort)

{
  Nlm_MenU       m;
  Nlm_WindoW     tempPort;
#ifdef WIN_MAC
  Nlm_RecT       r;
#endif
#ifdef WIN_MSWIN
  Nlm_PopupTool  u;
#endif
#ifdef WIN_MOTIF
  Nlm_ItemTool   PNTR hdls;
  Nlm_PopupTool  u;
#endif

  tempPort = Nlm_SavePortIfNeeded (c, savePort);
  m = (Nlm_MenU) Nlm_GetParent (c);
#ifdef WIN_MAC
  Nlm_SetChoiceValue (c, value, FALSE);
  if (Nlm_GetVisible (c) && Nlm_GetAllParentsVisible (c)) {
    Nlm_GetRect ((Nlm_GraphiC) m, &r);
    Nlm_UpsetRect (&r, 1, 1, 2, 2);
    Nlm_InvalRect (&r);
  }
#endif
#ifdef WIN_MSWIN
  u = Nlm_GetMenuPopup (m);
  if (u != NULL) {
    if (value > 0) {
      ComboBox_SetCurSel (u, value - 1);
    } else {
      ComboBox_SetCurSel (u, -1);
    }
  }
#endif
#ifdef WIN_MOTIF
  Nlm_SetPopupValue ((Nlm_ChoicE) c, value);
  u = Nlm_GetMenuPopup (m);
  if (u != NULL) {
    hdls = Nlm_GetChoiceHandles ((Nlm_ChoicE) c);
    if (value > 0  &&  value < 64) {
      XtVaSetValues (u, XmNmenuHistory, hdls [value - 1], NULL);
      XtUnmanageChild (hdls [63]);
    } else {
      XtVaSetValues (u, XmNmenuHistory, hdls [63], NULL);
      XtManageChild (hdls [63]);
    }
  }
#endif
  Nlm_RestorePort (tempPort);
}

static Nlm_Boolean Nlm_GetStatValue (Nlm_GraphiC i, Nlm_Int2 item)

{
  Nlm_Int2      itm;
  Nlm_ItemTool  itool;
  Nlm_MenU      m;

  m = (Nlm_MenU) Nlm_GetParent (i);
  itm = Nlm_GetFirstItem ((Nlm_IteM) i);
  itool = Nlm_GetItemHandle ((Nlm_IteM) i);
  return (Nlm_IsMenuItemChecked (m, itool, itm));
}

static Nlm_Int2 Nlm_GetChoiceValue (Nlm_GraphiC c)

{
  Nlm_Int2      first;
  Nlm_ItemTool  PNTR hdls;
  Nlm_Int2      itm;
  Nlm_ItemTool  itool;
  Nlm_MenU      m;
  Nlm_Int2      num;
  Nlm_Int2      rsult;

  rsult = 0;
  first = Nlm_GetFirstItem ((Nlm_IteM) c);
  num = Nlm_GetNumItems ((Nlm_ChoicE) c);
  m = (Nlm_MenU) Nlm_GetParent (c);
  itm = 1;
  hdls = Nlm_GetChoiceHandles ((Nlm_ChoicE) c);
  while (itm <= num && rsult == 0) {
    if (hdls != NULL) {
      itool = hdls [itm - 1];
    } else {
      itool = NULL;
    }
    if (Nlm_IsMenuItemChecked (m, itool, (Nlm_Int2)(first + itm - 1))) {
      rsult = itm;
    }
    itm++;
  }
  return rsult;
}

static Nlm_Int2 Nlm_GetChoiceGroupValue (Nlm_GraphiC c)

{
  Nlm_Int2      first;
  Nlm_ItemTool  PNTR hdls;
  Nlm_Int2      itm;
  Nlm_ItemTool  itool;
  Nlm_MenU      m;
  Nlm_Int2      num;
  Nlm_Int2      rsult;

  rsult = 0;
  first = Nlm_GetFirstItem ((Nlm_IteM) c);
  num = Nlm_GetNumItems ((Nlm_ChoicE) c);
  m = (Nlm_MenU) Nlm_GetParent (c);
  itm = 1;
  hdls = Nlm_GetChoiceHandles ((Nlm_ChoicE) c);
  while (itm <= num && rsult == 0) {
    if (hdls != NULL) {
      itool = hdls [itm - 1];
    } else {
      itool = NULL;
    }
    if (Nlm_IsMenuItemChecked (m, itool, (Nlm_Int2)(first + itm - 1))) {
      rsult = itm;
    }
    itm++;
  }
  return rsult;
}

static Nlm_Int2 Nlm_GetPopListValue (Nlm_GraphiC c)

{
  Nlm_Int2       rsult;
#ifdef WIN_MSWIN
  Nlm_MenU       m;
  Nlm_PopupTool  u;
#endif

  rsult = 0;
#ifdef WIN_MAC
  rsult = Nlm_GetChoiceValue (c);
#endif
#ifdef WIN_MSWIN
  m = (Nlm_MenU) Nlm_GetParent (c);
  u = Nlm_GetMenuPopup (m);
  if (u != NULL) {
    rsult = (Nlm_Int2)(ComboBox_GetCurSel( u ) + 1);
  }
#endif
#ifdef WIN_MOTIF
  rsult = Nlm_GetPopupValue ((Nlm_ChoicE) c);
#endif
  return rsult;
}

#ifdef WIN_MOTIF
static void Nlm_MenuCallback (Widget w, XtPointer client_data, XtPointer call_data)

{
  Nlm_GraphiC  i;

  choiceWidget = w;
  i = (Nlm_GraphiC) client_data;
  Nlm_DoCallback (i);
}
#endif


#ifndef WIN_MAC
static Nlm_Boolean Nlm_TitleGetAccel(Nlm_CharPtr  title,
                                     Nlm_CharPtr  accel,
                                     Nlm_CharPtr  mnemo,
                                     Nlm_Uint4Ptr mnemo_pos)
{
  static Nlm_Char letter[2];
  Nlm_Uint4 slash_pos, x_pos;

  if ( !Nlm_StrngPos(title, "/", 0, FALSE, &slash_pos)  ||
       title[slash_pos+1] == '\0'  ||  title[slash_pos+2] == '\0')
    return FALSE;


  if ( accel )
    *accel = IS_ALPHANUM( title[slash_pos+3] ) ?  title[slash_pos+3] : '\0';

  letter[0] = title[slash_pos + 2];

  if (!mnemo  &&  !mnemo_pos)
    return TRUE;

  /* Look for the letter;  try to match the case, if possible... */
  if (IS_ALPHANUM( letter[0] )  && 
      ((Nlm_StrngPos(title, letter, 0, TRUE,  &x_pos) && x_pos < slash_pos)  ||
       (Nlm_StrngPos(title, letter, 0, FALSE, &x_pos) && x_pos < slash_pos)))
    {
      if ( mnemo )
        *mnemo = title[x_pos];
      if ( mnemo_pos )
        *mnemo_pos = x_pos;
    }
  else
    {
      if ( mnemo )
        *mnemo = '\0';
    }

  return TRUE;
}
#endif /* !WIN_MAC */


static void Nlm_StripTitleAccel(Nlm_CharPtr title)
{
  Nlm_Uint4 pos;
  if ( Nlm_StrngPos(title, "/", 0, FALSE, &pos) )
    title[pos] = '\0';
}


#ifdef WIN_MOTIF
#define NO_MNEMO       0x1
#define NO_ACCEL       0x2
#define NO_ACCEL_DESCR 0x4

static void Nlm_AddTitleAccel(Widget w, Nlm_CharPtr title, Nlm_Int4 flags)
{
  Nlm_Char accel, mnemo;

  if (((flags & 0x3) ^ 0x3)  &&
      Nlm_TitleGetAccel(title, &accel, &mnemo, NULL) )
    {
      if (!(flags & NO_ACCEL)  &&  accel)
        {
          static char accel_key[] = "Ctrl<Key> ";
          accel_key[sizeof(accel_key) - 2] = accel;
          XtVaSetValues(w, XmNaccelerator, accel_key, NULL);

          if ( !(flags & NO_ACCEL_DESCR) )
            {
              XmString accel_text;
              static char accel_descr[] = "Ctrl- ";
              accel_descr[sizeof(accel_descr) - 2] = accel;

              accel_text = XmStringCreateSimple( accel_descr );
              XtVaSetValues(w, XmNacceleratorText, accel_text, NULL);
              XmStringFree( accel_text );
            }
        }
          
      if (!(flags & NO_MNEMO)  &&  mnemo)
        XtVaSetValues(w, XmNmnemonic, mnemo, NULL);
    }

  {{
    XmString label;
    Nlm_StripTitleAccel( title );
    label = XmStringCreateSimple( title );
    XtVaSetValues(w, XmNlabelString, label, NULL);
    XmStringFree( label );
  }}
}
#endif /* WIN_MOTIF */


static void Nlm_PrepareTitle(Nlm_CharPtr temp, Nlm_CharPtr title,
                             size_t siztemp, Nlm_Boolean isMenu)
{
  Nlm_StringNCpy_0(temp, title, siztemp);

#ifdef WIN_MAC
  {{
    if ( isMenu  )
      Nlm_StripTitleAccel( temp );
  }}
#endif

#ifdef WIN_MSWIN
  {{
    Nlm_Char  mnemo;
    Nlm_Uint4 pos;

    if (Nlm_TitleGetAccel(temp, NULL, &mnemo, &pos)  &&  mnemo)
      { /* temp := title[ 0..poslet-1 ] + '&' + title[ poslet..END ] */
        Nlm_StringNCpy_0(temp+pos+1, title+pos, siztemp-pos-1);
        temp[pos] = '&';
      }

    Nlm_StripTitleAccel( temp );
  }}
#endif
}


#define COMMAND_ITEM  1
#define STATUS_ITEM   2
#define CHOICE_ITEM   3
#define POPUP_ITEM    4
#define CASCADE_ITEM  5

static Nlm_ItemTool Nlm_AppendItems(Nlm_MenU m, Nlm_IteM i,
                                    Nlm_CharPtr itemNames,
                                    Nlm_Int2 type, Nlm_Boolean savePort)
{
  Nlm_MenuTool  h;
  Nlm_ItemTool  item = NULL;
  Nlm_Char      temp[256];
  Nlm_WindoW    tempPort;
#ifdef WIN_MOTIF
  Cardinal      n;
  Arg           wargs[10];
#endif

  Nlm_PrepareTitle(temp, itemNames, sizeof(temp), FALSE);
  if (temp[0] == '\0')
    return NULL;

  tempPort = Nlm_SavePortIfNeeded((Nlm_GraphiC)m, savePort);
  h = Nlm_GetMenuHandle( (Nlm_MenU)m );


#ifdef WIN_MAC
  Nlm_CtoPstr( temp );
  AppendMenu(h, (StringPtr)temp);
#endif

#ifdef WIN_MSWIN
  AppendMenu(h, MF_ENABLED, nextMenuNum, temp);
  if (nextMenuNum < 32767)
    nextMenuNum++;
#endif

#ifdef WIN_MOTIF
  n = 0;
  XtSetArg (wargs [n], XmNfontList, Nlm_XfontList); n++;
  if (type != POPUP_ITEM) {
    XtSetArg (wargs [n], XmNmarginHeight, (Dimension)5); n++;
    XtSetArg (wargs [n], XmNmarginWidth,  (Dimension)5); n++;
  }
  XtSetArg (wargs [n], XmNborderWidth, (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNheight, (Dimension)(Nlm_stdLineHeight+10)); n++;
  switch ( type )
    {
    case COMMAND_ITEM:
      item = XmCreatePushButton(h, (String)"", wargs, n);
      XtAddCallback(item, XmNactivateCallback,
                    Nlm_MenuCallback, (XtPointer)i);
      break;
    case STATUS_ITEM:
      XtSetArg(wargs[n], XmNindicatorType, XmN_OF_MANY);  n++;
      item = XmCreateToggleButton(h, (String)"", wargs, n);
      XtAddCallback(item, XmNvalueChangedCallback,
                    Nlm_MenuCallback, (XtPointer)i);
      break;
    case CHOICE_ITEM:
      XtSetArg(wargs[n], XmNindicatorType, XmONE_OF_MANY);  n++;
      item = XmCreateToggleButton(h, (String)"", wargs, n);
      XtAddCallback(item, XmNvalueChangedCallback,
                    Nlm_MenuCallback, (XtPointer)i);
      break;
    case POPUP_ITEM:
      XtSetArg(wargs[n], XmNrecomputeSize, (Boolean)FALSE);  n++;
      item = XmCreatePushButton(h, (String)temp, wargs, n);
      XtAddCallback(item, XmNactivateCallback,
                    Nlm_MenuCallback, (XtPointer)i);
      break;
    default:
      break;
    }

  if ( item )
    Nlm_AddTitleAccel(item, temp, (type == POPUP_ITEM ? NO_ACCEL_DESCR : 0));
#endif

  Nlm_RestorePort( tempPort );
  return item;
}


#ifdef WIN_MAC
static void Nlm_AppendResources (Nlm_MenU m, ResType res, Nlm_Boolean savePort)

{
  Nlm_MenuTool  h;
  Nlm_WindoW    tempPort;

  tempPort = Nlm_SavePortIfNeeded ((Nlm_GraphiC) m, savePort);
  h = Nlm_GetMenuHandle (m);
  AppendResMenu (h, res);
  Nlm_RestorePort (tempPort);
}
#endif

static void Nlm_StripParens (Nlm_CharPtr to, Nlm_CharPtr from, size_t max)

{
  Nlm_Char  ch;
  Nlm_Int2  i;
  Nlm_Int2  j;

  if (to != NULL && max > 0) {
    *to = '\0';
    if (from != NULL) {
      i = 0;
      j = 0;
      ch = from [i];
      while (ch != '\0' && j < (Nlm_Int2) (max - 1)) {
	if (ch == '(') {
	  to [j] = '[';
	  i++;
	  j++;
	} else if (ch == ')') {
	  to [j] = ']';
	  i++;
	  j++;
	} else {
	  to [j] = ch;
	  i++;
	  j++;
	}
	ch = from [i];
      }
      to [j] = '\0';
    }
  }
}

static Nlm_IteM Nlm_AppendChoice (Nlm_ChoicE c, Nlm_CharPtr title,
				  Nlm_Boolean poplist, Nlm_Boolean savePort)

{
  Nlm_IteM      i;
  Nlm_ItemTool  itool;
  Nlm_MenU      m;
  Nlm_Int2      num = Nlm_GetNumItems(c);
  Nlm_RecT      r;
  Nlm_Char      temp [256];
  Nlm_WindoW    tempPort;
#ifdef WIN_MOTIF
  Nlm_ItemTool  *hdls;
  if (num < 0  ||  63 <= num)
    return 0;
#endif

  i = NULL;
  tempPort = Nlm_SavePortIfNeeded ((Nlm_GraphiC) c, savePort);
  Nlm_StripParens (temp, title, sizeof (temp));
  m = (Nlm_MenU) Nlm_GetParent ((Nlm_GraphiC) c);
  if (poplist) {
    itool = Nlm_AppendItems (m, (Nlm_IteM) c, temp, POPUP_ITEM, FALSE);
  } else {
    itool = Nlm_AppendItems (m, (Nlm_IteM) c, temp, CHOICE_ITEM, FALSE);
    Nlm_LoadRect (&r, 0, 0, 0, 0);
    i = (Nlm_IteM) Nlm_CreateLink ((Nlm_GraphiC) c, &r, sizeof (Nlm_ItemRec), choiceItemProcs);
    if (i != NULL) {
      Nlm_SetVisible ((Nlm_GraphiC) i, TRUE);
    }
  }

  num++;
  Nlm_LoadItemData (i, NULL, num, NULL);
#ifdef WIN_MOTIF
  hdls = Nlm_GetChoiceHandles (c);
  if (hdls == NULL) {
    hdls = (Nlm_ItemTool *) Nlm_MemNew(64 * sizeof (Nlm_ItemTool));
    Nlm_SetChoiceHandles (c, hdls);
  }
  if ( hdls ) {
    Nlm_SetNumItems(c, num);
    hdls[num - 1] = itool;
    XtManageChild(itool);
  }
#else
  Nlm_SetNumItems (c, num);
#endif

  Nlm_RestorePort (tempPort);
  return i;
}


static void Nlm_AppendPopList (Nlm_ChoicE c, Nlm_CharPtr title, Nlm_Boolean savePort)
{
  Nlm_MenU       m;
  Nlm_RecT       r;
  Nlm_Int2       swid;
  Nlm_Char       temp[256];
  Nlm_WindoW     tempPort;
  Nlm_Int4       wid;
#ifdef WIN_MSWIN
  Nlm_Int2       num;
  Nlm_PopupTool  u;
#endif
#ifdef WIN_MOTIF
  Nlm_Boolean    done;
  Nlm_ItemTool  *hdls;
  Nlm_ItemTool   itool;
  XmString       label;
  Nlm_Int2       num;
  Nlm_PopupTool  u;
#endif

  tempPort = Nlm_SavePortIfNeeded ((Nlm_GraphiC) c, savePort);
  Nlm_StripParens(temp, title, sizeof(temp));
  m = (Nlm_MenU) Nlm_GetParent ((Nlm_GraphiC) c);
#ifdef WIN_MAC
  Nlm_StripTitleAccel( temp );
  Nlm_AppendChoice (c, temp, TRUE, FALSE);
#endif
#ifdef WIN_MSWIN
  Nlm_StripTitleAccel( temp );
  u = Nlm_GetMenuPopup (m);
  if (u != NULL) {
    ComboBox_AddString (u, temp);
    num = Nlm_GetNumItems ((Nlm_ChoicE) c);
    num++;
    Nlm_SetNumItems (c, num);
  }
#endif
#ifdef WIN_MOTIF
  done = FALSE;
  hdls = Nlm_GetChoiceHandles (c);
  if (hdls != NULL) {
    num = Nlm_GetNumItems ((Nlm_ChoicE) c);
    num++;
    if (num > 0 && num < 64) {
      itool = hdls [num - 1];
      if (itool != NULL) {
	label = XmStringCreateSimple (temp);
	XtVaSetValues (itool, XmNlabelString, label, NULL);
	XmStringFree (label);
	XtManageChild (itool);
	Nlm_SetNumItems ((Nlm_ChoicE) c, num);
	done = TRUE;
      }
    }
  }
  if (! done) {
    Nlm_AppendChoice (c, temp, TRUE, FALSE);
  }
#endif
  Nlm_SelectFont (Nlm_systemFont);
  swid = Nlm_StringWidth (temp);
  Nlm_GetRect ((Nlm_GraphiC) m, &r);
#ifdef WIN_MAC
  wid = r.right - r.left - 26;
  if (swid > wid) {
    r.right = r.left + swid + 26;
    Nlm_SetRect ((Nlm_GraphiC) m, &r);
    Nlm_SetRect ((Nlm_GraphiC) c, &r);
  }
  if (swid > wid) {
    if (Nlm_GetVisible ((Nlm_GraphiC) c) && Nlm_GetAllParentsVisible ((Nlm_GraphiC) c)) {
      Nlm_InsetRect (&r, -2, -2);
      Nlm_OffsetRect (&r, 1, 1);
      Nlm_InvalRect (&r);
    }
  }
#endif
#ifdef WIN_MSWIN
  wid = r.right - r.left - 26;
  if (swid > wid) {
    r.right = (Nlm_Int2)(r.left + swid + 26);
    Nlm_SetRect ((Nlm_GraphiC) m, &r);
    Nlm_SetRect ((Nlm_GraphiC) c, &r);
  }
  u = Nlm_GetMenuPopup (m);
  if (u != NULL) {
    if (num > 10) {
      num = 10;
    }
    MoveWindow (u, r.left, r.top, r.right - r.left,
		r.bottom - r.top + (num + 1) * Nlm_stdLineHeight, TRUE);
    UpdateWindow (u);
  }
#endif
#ifdef WIN_MOTIF
  wid = r.right - r.left - 52;
  if (swid > wid) {
    r.right = r.left + swid + 52;
    Nlm_SetRect ((Nlm_GraphiC) m, &r);
    Nlm_SetRect ((Nlm_GraphiC) c, &r);
  }
  u = Nlm_GetMenuPopup (m);
  if (u != NULL) {
    XtVaSetValues (u,
		   XmNx, (Position) r.left,
		   XmNy, (Position) r.top,
		   XmNwidth,  (Dimension) (r.right - r.left),
		   XmNheight, (Dimension) (r.bottom - r.top), 
		   NULL);
  }
#endif
  Nlm_GetRect ((Nlm_GraphiC) m, &r);
  Nlm_DoAdjustPrnt ((Nlm_GraphiC) m, &r, TRUE, FALSE);
  Nlm_RestorePort (tempPort);
}


static Nlm_ItemTool Nlm_AppendOnePopListItem (Nlm_MenU m, Nlm_IteM i,
					      Nlm_CharPtr itemNames)

{
  Nlm_MenuTool  h;
  Nlm_ItemTool  rsult;
  Nlm_Char      temp [256];
#ifdef WIN_MOTIF
  XmString      label;
  Cardinal      n;
  Arg           wargs [10];
#endif

  rsult = NULL;
  h = Nlm_GetMenuHandle ((Nlm_MenU) m);
  Nlm_StringNCpy_0(temp, itemNames, sizeof(temp));
  if (temp [0] != '\0') {
#ifdef WIN_MAC
    Nlm_CtoPstr (temp);
    AppendMenu (h, (StringPtr) temp);
#endif
#ifdef WIN_MSWIN
    AppendMenu (h, MF_ENABLED, nextMenuNum, itemNames);
    if (nextMenuNum < 32767) {
      nextMenuNum++;
    }
#endif
#ifdef WIN_MOTIF
    label = XmStringCreateSimple (temp);
    n = 0;
    XtSetArg (wargs [n], XmNlabelString, label); n++;
    XtSetArg (wargs [n], XmNfontList, Nlm_XfontList); n++;
    XtSetArg (wargs [n], XmNborderWidth, (Dimension) 0); n++;
    XtSetArg (wargs [n], XmNheight, (Dimension)(Nlm_stdLineHeight+10)); n++;
    XtSetArg (wargs [n], XmNrecomputeSize, (Boolean)FALSE); n++;
    rsult = XmCreatePushButton (h, (String) "", wargs, n);
    XtAddCallback (rsult, XmNactivateCallback, Nlm_MenuCallback, (XtPointer) i);
    XmStringFree (label);
#endif
  }
  return rsult;
}


static Nlm_IteM Nlm_AppendOnePopListChoice (Nlm_ChoicE c, Nlm_CharPtr title)

{
  Nlm_IteM      i;
  Nlm_ItemTool  itool;
  Nlm_MenU      m;
  Nlm_Int2      num;
  Nlm_Char      temp [256];
#ifdef WIN_MOTIF
  Nlm_ItemTool  *hdls;
#endif

  i = NULL;
  Nlm_StripParens (temp, title, sizeof (temp));
  m = (Nlm_MenU) Nlm_GetParent ((Nlm_GraphiC) c);
  itool = Nlm_AppendOnePopListItem (m, (Nlm_IteM) c, temp);
  num = Nlm_GetNumItems (c);
  num++;
  Nlm_LoadItemData (i, NULL, num, NULL);
#ifdef WIN_MOTIF
  hdls = Nlm_GetChoiceHandles (c);
  if (hdls == NULL) {
    hdls = (Nlm_ItemTool *) Nlm_MemNew (64 * sizeof (Nlm_ItemTool));
    Nlm_SetChoiceHandles (c, hdls);
  }
  if (hdls != NULL && num > 0 && num < 64) {
    hdls [num - 1] = itool;
    Nlm_SetNumItems (c, num);
    XtManageChild (itool);
  }
#else
  Nlm_SetNumItems (c, num);
#endif

  return i;
}


static void Nlm_MultiAppendPopList (Nlm_ChoicE c, Nlm_CharPtr PNTR titles, Nlm_Boolean savePort)

{
  Nlm_Int2       i;
  Nlm_MenU       m;
  Nlm_RecT       r;
  Nlm_Int4       swid;
  Nlm_Char       temp [256];
  Nlm_WindoW     tempPort;
  Nlm_Int4       wid;
#ifdef WIN_MSWIN
  Nlm_Int2       num;
  Nlm_PopupTool  u;
#endif
#ifdef WIN_MOTIF
  Nlm_Boolean    done;
  Nlm_ItemTool  *hdls;
  Nlm_ItemTool   itool;
  XmString       label;
  Nlm_Int2       num;
  Nlm_PopupTool  u;
#endif

  if (titles == NULL) return;
  tempPort = Nlm_SavePortIfNeeded ((Nlm_GraphiC) c, savePort);
  m = (Nlm_MenU) Nlm_GetParent ((Nlm_GraphiC) c);
  swid = 0;
  i = 0;
  while (titles [i] != NULL) {
    Nlm_StripParens (temp, titles [i], sizeof (temp));
#ifdef WIN_MAC
    Nlm_AppendOnePopListChoice (c, temp);
#endif
#ifdef WIN_MSWIN
    u = Nlm_GetMenuPopup (m);
    if (u != NULL) {
      ComboBox_AddString (u, temp);
      num = Nlm_GetNumItems ((Nlm_ChoicE) c);
      num++;
      Nlm_SetNumItems (c, num);
    }
#endif
#ifdef WIN_MOTIF
    done = FALSE;
    hdls = Nlm_GetChoiceHandles (c);
    if (hdls != NULL) {
      num = Nlm_GetNumItems ((Nlm_ChoicE) c);
      num++;
      if (num > 0 && num < 64) {
	itool = hdls [num - 1];
	if (itool != NULL) {
	  label = XmStringCreateSimple (temp);
	  XtVaSetValues (itool, XmNlabelString, label, NULL);
	  XmStringFree (label);
	  XtManageChild (itool);
	  Nlm_SetNumItems ((Nlm_ChoicE) c, num);
	  done = TRUE;
	}
      }
    }
    if (! done) {
      Nlm_AppendOnePopListChoice (c, temp);
    }
#endif
    Nlm_SelectFont (Nlm_systemFont);
    wid = Nlm_StringWidth (temp);
    if (wid > swid) {
      swid = wid;
    }
    i++;
  }
  Nlm_GetRect ((Nlm_GraphiC) m, &r);
#ifdef WIN_MAC
  wid = r.right - r.left - 26;
  if (swid > wid) {
    r.right = r.left + swid + 26;
    Nlm_SetRect ((Nlm_GraphiC) m, &r);
    Nlm_SetRect ((Nlm_GraphiC) c, &r);
  }
  if (swid > wid) {
    if (Nlm_GetVisible ((Nlm_GraphiC) c) && Nlm_GetAllParentsVisible ((Nlm_GraphiC) c)) {
      Nlm_InsetRect (&r, -2, -2);
      Nlm_OffsetRect (&r, 1, 1);
      Nlm_InvalRect (&r);
    }
  }
#endif
#ifdef WIN_MSWIN
  wid = r.right - r.left - 26;
  if (swid > wid) {
    r.right = (Nlm_Int2)(r.left + swid + 26);
    Nlm_SetRect ((Nlm_GraphiC) m, &r);
    Nlm_SetRect ((Nlm_GraphiC) c, &r);
  }
  u = Nlm_GetMenuPopup (m);
  if (u != NULL) {
    if (num > 10) {
      num = 10;
    }
    MoveWindow (u, r.left, r.top, r.right - r.left,
		r.bottom - r.top + (num + 1) * Nlm_stdLineHeight, TRUE);
    UpdateWindow (u);
  }
#endif
#ifdef WIN_MOTIF
  wid = r.right - r.left - 52;
  if (swid > wid) {
    r.right = r.left + swid + 52;
    Nlm_SetRect ((Nlm_GraphiC) m, &r);
    Nlm_SetRect ((Nlm_GraphiC) c, &r);
  }
  u = Nlm_GetMenuPopup (m);
  if (u != NULL) {
    XtVaSetValues (u,
		   XmNx, (Position) r.left,
		   XmNy, (Position) r.top,
		   XmNwidth, (Dimension) (r.right - r.left),
		   XmNheight, (Dimension) (r.bottom - r.top), 
		   NULL);
  }
#endif
  Nlm_GetRect ((Nlm_GraphiC) m, &r);
  Nlm_DoAdjustPrnt ((Nlm_GraphiC) m, &r, TRUE, FALSE);
  Nlm_RestorePort (tempPort);
}

extern void Nlm_PopupItems (Nlm_PopuP p, Nlm_CharPtr PNTR titles)

{
  Nlm_ChoicE  c;

  c = (Nlm_ChoicE) p;
  if (c != NULL && titles != NULL) {
    if (Nlm_IsItAPopupList (c)) {
      Nlm_MultiAppendPopList (c, titles, TRUE);
    }
  }
}



#ifdef WIN_MAC
static void Nlm_InvalPopup (Nlm_GraphiC m)

{
  Nlm_RecT  r;

  if (Nlm_GetVisible (m) && Nlm_GetAllParentsVisible (m)) {
    Nlm_GetRect (m, &r);
    Nlm_InsetRect (&r, -1, -1);
    Nlm_InvalRect (&r);
  }
}
#endif


static void Nlm_SetPopupPosition (Nlm_GraphiC m, Nlm_RectPtr r,
				  Nlm_Boolean savePort, Nlm_Boolean force)
{
#ifdef WIN_MAC
  Nlm_RecT    oldRect;
  Nlm_WindoW  tempPort;

  if (r == NULL)  return;

  if ( !Nlm_GetRealized( m ) )  
    {
      Nlm_SetRect(m, r);
      return;
    }

  Nlm_DoGetPosition (m, &oldRect);
  if (!force  &&  Nlm_EqualRect(r, &oldRect))  return;

  tempPort = Nlm_SavePortIfNeeded (m, savePort);
  Nlm_InvalPopup (m);
  Nlm_SetRect (m, r);
  Nlm_InvalPopup (m);
  Nlm_RestorePort (tempPort);
#endif
}


static void Nlm_GetPopupPosition (Nlm_GraphiC m, Nlm_RectPtr r)

{
#ifdef WIN_MAC
  if (r != NULL) {
    Nlm_GetRect (m, r);
  }
#endif
}


static void Nlm_SetPopListPosition (Nlm_GraphiC m, Nlm_RectPtr r,
				    Nlm_Boolean savePort, Nlm_Boolean force)
{
  Nlm_ChoicE     c;
  Nlm_RecT       oldRect;
  Nlm_WindoW     tempPort;
#ifdef WIN_MSWIN
  Nlm_Int2       num;
  Nlm_PopupTool  u;
#endif
#ifdef WIN_MOTIF
  Nlm_PopupTool  u;
#endif

  if (r == NULL)  return;

  Nlm_DoGetPosition (m, &oldRect);
  if (!force  &&  Nlm_EqualRect(r, &oldRect))  return;

  if ( Nlm_GetRealized( m ) )
    {
      tempPort = Nlm_SavePortIfNeeded (m, savePort);
#ifdef WIN_MAC
      Nlm_InvalPopup (m);
#endif
    }

  Nlm_SetRect (m, r);
  c = (Nlm_ChoicE) Nlm_GetChild (m);
  Nlm_SetRect ((Nlm_GraphiC) c, r);

  if ( !Nlm_GetRealized( m ) )  return;

#ifdef WIN_MAC
  Nlm_InvalPopup (m);
#endif
#ifdef WIN_MSWIN
  u = Nlm_GetMenuPopup ((Nlm_MenU) m);
  if (u != NULL) {
    c = (Nlm_ChoicE) Nlm_GetChild (m);
    num = Nlm_GetNumItems (c);
    if (num > 10) {
      num = 10;
    }
    MoveWindow (u, r->left, r->top, r->right - r->left,
		r->bottom - r->top + (num + 1) * Nlm_stdLineHeight, TRUE);
    UpdateWindow (u);
  }
#endif
#ifdef WIN_MOTIF

  /* Adjust the popup slightly to the left in */
  /* order to adjust for the space that it    */
  /* leaves for the empty label.              */

  r->left += POPUP_LOCATION_ADJUSTMENT;

  /* Set the Popup's size and position */

  u = Nlm_GetMenuPopup ((Nlm_MenU) m);
  XtVaSetValues (u,
		 XmNx, (Position) r->left,
		 XmNy, (Position) r->top,
		 XmNwidth, (Dimension) (r->right - r->left),
		 XmNheight, (Dimension) (r->bottom - r->top), 
		 NULL);
#endif

  Nlm_RestorePort (tempPort);
}


static void Nlm_GetPopListPosition (Nlm_GraphiC m, Nlm_RectPtr r)

{
  if (r != NULL) {
    Nlm_GetRect (m, r);
  }
}


static void Nlm_SetPopupChoicePosition(Nlm_GraphiC p, Nlm_RectPtr r,
				       Nlm_Boolean savePort, Nlm_Boolean force)
{
  Nlm_GraphiC  m;

  if (r != NULL) {
    m = Nlm_GetParent (p);
    Nlm_DoSetPosition (m, r, savePort, force);
  }
}


static void Nlm_GetPopupChoicePosition (Nlm_GraphiC p, Nlm_RectPtr r)

{
  Nlm_GraphiC  m;

  if (r != NULL) {
    m = Nlm_GetParent (p);
    Nlm_GetRect (m, r);
  }
}

#ifdef WIN_MSWIN
/* Message cracker functions */

static void MyCls_OnChar (HWND hwnd, UINT ch, int cRepeat)

{
  Nlm_PopuP  p;

  p = (Nlm_PopuP) GetProp (hwnd, (LPSTR) "Nlm_VibrantProp");
  handlechar = FALSE;
  if (ch == '\t') {
    Nlm_DoSendFocus ((Nlm_GraphiC) p, (Nlm_Char) ch);
  } else if (ch == '\n' || ch == '\r') {
    Nlm_DoSendFocus ((Nlm_GraphiC) p, (Nlm_Char) ch);
  } else {
    handlechar = TRUE;
  }
}

LRESULT CALLBACK EXPORT PopupProc (HWND hwnd, UINT message,
				   WPARAM wParam, LPARAM lParam)
{
  Nlm_PopuP  p;
  BOOL       call_win_proc = TRUE;
  LRESULT    rsult = 0;
  HDC        tempHDC;
  HWND       tempHWnd;

  if (Nlm_VibrantDisabled ()) {
    return CallWindowProc (lpfnOldPopupProc, hwnd, message, wParam, lParam);
  }

  tempHWnd = Nlm_currentHWnd;
  tempHDC  = Nlm_currentHDC;
  p = (Nlm_PopuP) GetProp (hwnd, (LPSTR) "Nlm_VibrantProp");
  Nlm_theWindow = Nlm_GetParentWindow ((Nlm_GraphiC) p);
  Nlm_currentHWnd = GetParent (hwnd);
  Nlm_currentHDC = Nlm_ParentWindowPort ((Nlm_GraphiC) p);
  Nlm_currentWindowTool = hwnd;
  Nlm_currentKey = '\0';
  Nlm_currentWParam = wParam;
  Nlm_currentLParam = lParam;
  Nlm_cmmdKey  = FALSE;
  Nlm_ctrlKey  = FALSE;
  Nlm_optKey   = FALSE;
  Nlm_shftKey  = FALSE;
  Nlm_dblClick = FALSE;

  switch (message) {
    case WM_KEYDOWN:
      call_win_proc = !Nlm_ProcessKeydown((Nlm_GraphiC)p, wParam,
                                          VERT_PAGE|VERT_ARROW);
      break;
    case WM_CHAR:
      HANDLE_WM_CHAR (hwnd, wParam, lParam, MyCls_OnChar);
      call_win_proc = handlechar;
      break;
  }

  if ( call_win_proc )
    rsult = CallWindowProc(lpfnOldPopupProc, hwnd, message, wParam, lParam);

  Nlm_currentHWnd = tempHWnd;
  Nlm_currentHDC  = tempHDC;
  Nlm_currentWindowTool = tempHWnd;
  return rsult;
}
#endif

extern Nlm_Int2  Nlm_internalMenuBarHeight;
Nlm_Int2  Nlm_internalMenuBarHeight = 0;

static void Nlm_NewMenuBar (Nlm_MenuBaR mb)

{
  Nlm_PoinT       npt;
  Nlm_RecT        r;
#ifdef WIN_MSWIN
  Nlm_MenuTool    h;
  Nlm_WindowTool  wptr;
#endif
#ifdef WIN_MOTIF
  Nlm_MenuTool    h;
  Cardinal        n;
  Nlm_MainTool    man;
  Arg             wargs [10];
#endif

  Nlm_GetRect ((Nlm_GraphiC) mb, &r);
  Nlm_LoadPt (&npt, (Nlm_Int2)(r.left + 15), r.top);
  Nlm_LoadBoxData ((Nlm_BoX) mb, npt, npt, npt, npt.y, npt.x, 7, 7, 7, 7, 0, 0);
#ifdef WIN_MSWIN
  h = CreateMenu ();
  wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) mb);
  SetMenu (wptr, h);
  Nlm_LoadMenuBarData (mb, h);
#endif
#ifdef WIN_MOTIF
  man = Nlm_ParentWindowMain ((Nlm_GraphiC) mb);
  n = 0;
  XtSetArg (wargs [n], XmNborderWidth, (Dimension) 0); n++;
  h = XmCreateMenuBar (man, (String) "", wargs, n);

  Nlm_LoadMenuBarData (mb, h);
#endif
}


#ifdef WIN_MAC
static void Nlm_NewDesktopMenu (Nlm_MenU m, Nlm_CharPtr title)

{
  Nlm_MenuTool  h;
  Nlm_Char      temp [256];

/* M.I */
  Nlm_PrepareTitle (temp, title, sizeof (temp), TRUE);
/* M.I */
  Nlm_CtoPstr (temp);
  h = NewMenu (nextMenuNum, (StringPtr) temp);
  Nlm_LoadMenuData (m, h, NULL, nextMenuNum, NULL);
  if (nextMenuNum < 32767) {
    nextMenuNum++;
  }
}
#endif


#ifdef WIN_MOTIF
static void Nlm_SetupCmapVis(Nlm_GraphiC g, Arg *wargs, Cardinal *n)
{
  Colormap cMap;
  Visual  *visual;
  if ( !Nlm_GetFixedColormap(g, &cMap, &visual) )
    {
      cMap = Nlm_VibrantDefaultColormap();
      visual = Nlm_VibrantDefaultVisual();
    }

  XtSetArg(wargs[*n], XmNcolormap, cMap);  (*n)++;
  XtSetArg(wargs[*n], XmNvisual, visual);  (*n)++;
}
#endif


static void Nlm_NewPopup (Nlm_MenU m, Nlm_CharPtr title, Nlm_RectPtr r)
{
  Nlm_MenuTool   h;
  Nlm_PrompT     p;
  Nlm_PopupTool  pop;
  Nlm_Char       temp [256];
#ifdef WIN_MSWIN
  Nlm_MenuBaR    mb;
  Nlm_MenuTool   mbh;
#endif
#ifdef WIN_MOTIF
  Dimension      height;
  Nlm_MenuBaR    mb;
  Nlm_MenuTool   mbh;
  Cardinal       n;
  Arg            wargs[16];
#endif

  h = NULL;
  p = NULL;
  pop = NULL;
/* M.I */
  Nlm_PrepareTitle (temp, title, sizeof (temp), TRUE);
/* M.I */

#ifdef WIN_MAC
  h = NewMenu (nextMenuNum, (StringPtr) "");
  p = Nlm_DependentPrompt ((Nlm_GraphiC) m, r, temp, Nlm_systemFont, 'c');
#endif

#ifdef WIN_MSWIN
  p   = NULL;
  mb  = (Nlm_MenuBaR) Nlm_GetParent ((Nlm_GraphiC) m);
  mbh = Nlm_GetMenuBarHandle( mb );
  h = CreateMenu();
  AppendMenu(mbh, MF_POPUP | MF_ENABLED, (UINT)h, temp);
#endif

#ifdef WIN_MOTIF
  p = NULL;
  mb = (Nlm_MenuBaR) Nlm_GetParent ((Nlm_GraphiC) m);
  mbh = Nlm_GetMenuBarHandle (mb);

  n = 0;
  XtSetArg (wargs [n], XmNborderWidth, (Dimension) 0); n++;
  Nlm_SetupCmapVis((Nlm_GraphiC)m, wargs, &n);
  h = XmCreatePulldownMenu (mbh, (String) "", wargs, n);
  XtRealizeWidget( h );

  n = 0;
  XtSetArg (wargs [n], XmNsubMenuId, h); n++;
  XtSetArg (wargs [n], XmNborderWidth, (Dimension) 0); n++;
  Nlm_SetupCmapVis((Nlm_GraphiC)m, wargs, &n);
  pop = XmCreateCascadeButton (mbh, (String) "", wargs, n);
  Nlm_AddTitleAccel(pop, temp, 0);
  XtManageChild (pop);

  if (Nlm_internalMenuBarHeight == 0) {
    XtVaGetValues (mbh, XmNborderWidth, &height, NULL);
    Nlm_internalMenuBarHeight += (Nlm_Int2) height;
    XtVaGetValues (mbh, XmNmarginHeight, &height, NULL);
    Nlm_internalMenuBarHeight += (Nlm_Int2) height;
    XtVaGetValues (mbh, XmNhighlightThickness, &height, NULL);
    Nlm_internalMenuBarHeight += (Nlm_Int2) height;
    XtVaGetValues (pop, XmNheight, &height, NULL);
    Nlm_internalMenuBarHeight += (Nlm_Int2) height;
  }
#endif

  Nlm_LoadMenuData (m, h, p, nextMenuNum, pop);
  if (nextMenuNum < 32767) {
    nextMenuNum++;
  }
}


static void Nlm_NewFloatingPopup (Nlm_MenU m, Nlm_CharPtr title, Nlm_RectPtr mr)

{
#ifdef WIN_MAC
  Nlm_MenuTool    h;
  Nlm_PrompT      p;
  Nlm_Char        temp [256];
#endif
#ifdef WIN_MSWIN
  Nlm_PopupTool   c;
  Nlm_RecT        r;
  Nlm_WindowTool  wptr;
  Nlm_FntPtr      fntptr;
#endif

#ifdef WIN_MAC
/* M.I */
  Nlm_PrepareTitle (temp, title, sizeof (temp), TRUE);
/* M.I */
  h = NewMenu (nextMenuNum, (StringPtr) "");
  p = Nlm_DependentPrompt ((Nlm_GraphiC) m, mr, temp, Nlm_systemFont, 'c');
  Nlm_LoadMenuData (m, h, p, nextMenuNum, NULL);
  if (nextMenuNum < 32767) {
    nextMenuNum++;
  }
#endif
#ifdef WIN_MSWIN
  r = *mr;
  wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) m);
  c = CreateWindow ("Combobox", title,
		     WS_CHILD | CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED,
		     r.left, r.top, r.right - r.left,
		     r.bottom - r.top, wptr, 0,
		     Nlm_currentHInst, NULL);
  if (c != NULL) {
    SetProp (c, (LPSTR) "Nlm_VibrantProp", (Nlm_HandleTool) m);
  }
  Nlm_LoadMenuData (m, NULL, NULL, nextMenuNum, c);
  if (nextMenuNum < 32767) {
    nextMenuNum++;
  }
  fntptr = (Nlm_FntPtr) Nlm_HandLock (Nlm_systemFont);
  SetWindowFont(c, fntptr->handle, FALSE);
  Nlm_HandUnlock(Nlm_systemFont);

#endif
#ifdef WIN_MOTIF
#endif
}

#ifdef WIN_MOTIF
static void Nlm_NewPopupList (Nlm_MenU m, Nlm_ChoicE c, Nlm_PupActnProc actn)

{
  Nlm_MenuTool    h;
  Nlm_ItemTool    PNTR hdls;
  Nlm_ItemTool    itool;
  Nlm_PopupTool   pop;
  Nlm_RecT        r;
  Nlm_WindowTool  wptr;
  Pixel           color;
  XmString        label;
  Widget          lbl;
  Cardinal        n;
  Arg             wargs [15];

  Nlm_GetRect ((Nlm_GraphiC) m, &r);
  wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) m);

  n = 0;
  Nlm_SetupCmapVis((Nlm_GraphiC)m, wargs, &n);
  h = XmCreatePulldownMenu (wptr, (String) "", wargs, n);
  XtRealizeWidget( h );
  XtAddEventHandler(h, ColormapChangeMask, FALSE,
		    MenuShellColormapCB, (XtPointer) m);

  n = 0;
  XtSetArg (wargs [n], XmNx, (Position) r.left); n++;
  XtSetArg (wargs [n], XmNy, (Position) r.top ); n++;
  XtSetArg (wargs [n], XmNwidth,  (Dimension) (r.right  - r.left)); n++;
  XtSetArg (wargs [n], XmNheight, (Dimension) (r.bottom - r.top )); n++;
  XtSetArg (wargs [n], XmNborderWidth, (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNhighlightThickness, 0); n++;
  XtSetArg (wargs [n], XmNsubMenuId, h); n++;
  /*
  XtSetArg (wargs [n], XmNorientation, XmVERTICAL); n++;
  */

  pop = XmCreateOptionMenu (wptr, (String) "", wargs, n);
  Nlm_OverrideStdTranslations((Nlm_GraphiC)m, pop,
                              VERT_PAGE|VERT_ARROW|HORIZ_PAGE|HORIZ_ARROW);

  /*
  lbl = XmOptionLabelGadget (pop);
  XtVaGetValues (lbl, XmNforeground, &color, 0);

  n = 0;
  label = XmStringCreateSimple ("");
  XtSetArg (wargs [n], XmNlabelString, label); n++;
  XtSetArg (wargs [n], XmNwidth, (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNheight, (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNborderWidth, (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNhighlightThickness, (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNbackground, (Pixel) color); n++;
  XtSetValues (lbl, wargs, n);
  XmStringFree (label);
  */

  XtManageChild (pop);

  Nlm_LoadMenuData (m, h, NULL, nextMenuNum, pop);
  if (nextMenuNum < 32767) {
    nextMenuNum++;
  }

  n = 0;
  label = XmStringCreateSimple (" ");
  XtSetArg (wargs [n], XmNlabelString, label); n++;
  XtSetArg (wargs [n], XmNfontList, Nlm_XfontList); n++;
  XtSetArg (wargs [n], XmNmarginHeight, 5); n++;
  XtSetArg (wargs [n], XmNmarginWidth, 5); n++;
  XtSetArg (wargs [n], XmNborderWidth, (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNheight, Nlm_stdLineHeight+10); n++;
  XtSetArg (wargs [n], XmNrecomputeSize, FALSE); n++;
  itool = XmCreatePushButton (h, (String) "", wargs, n);
  XtAddCallback (itool, XmNactivateCallback, Nlm_MenuCallback, (XtPointer) c);
  XmStringFree (label);
  XtManageChild (itool);
  hdls = (Nlm_ItemTool *) Nlm_MemNew (64 * sizeof (Nlm_ItemTool));
  hdls [63] = itool;
  XtVaSetValues (pop, XmNmenuHistory, hdls [63], NULL);
  Nlm_LoadItemData ((Nlm_IteM) c, m, 1, pop);
  Nlm_LoadChoiceData (c, 0, TRUE, 0, hdls);
  Nlm_LoadAction ((Nlm_GraphiC) c, (Nlm_ActnProc) actn);
}
#endif  /* WIN_MOTIF */


#if defined(WIN_MSWIN) || defined(WIN_MAC) 
static void Nlm_NewPopListMenu (Nlm_MenU m)
{
  Nlm_PopupTool   c;
  Nlm_MenuTool    h;
#ifdef WIN_MSWIN
  Nlm_RecT        r;
  Nlm_WindowTool  wptr;
  Nlm_FntPtr      fntptr;
#endif

#ifdef WIN_MAC
  h = NewMenu (nextMenuNum, (StringPtr) "");
  c = NULL;
  Nlm_LoadMenuData (m, h, NULL, nextMenuNum, c);
#endif
#ifdef WIN_MSWIN
  Nlm_GetRect ((Nlm_GraphiC) m, &r);
  h = NULL;
  wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) m);
  c = CreateWindow ("Combobox", "", WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
		     r.left, r.top, r.right - r.left,
		     r.bottom - r.top, wptr, 0,
		     Nlm_currentHInst, NULL);
  if (c != NULL) {
    SetProp (c, (LPSTR) "Nlm_VibrantProp", (Nlm_HandleTool) m);
  }
  Nlm_LoadMenuData (m, h, NULL, nextMenuNum, c);
  if (lpfnNewPopupProc == NULL) {
    lpfnNewPopupProc = (WNDPROC) MakeProcInstance ((FARPROC) PopupProc, Nlm_currentHInst);
  }
  if (lpfnOldPopupProc == NULL) {
    lpfnOldPopupProc = (WNDPROC) GetWindowLong (c, GWL_WNDPROC);
  } else if (lpfnOldPopupProc != (WNDPROC) GetWindowLong (c, GWL_WNDPROC)) {
    Nlm_Message (MSG_ERROR, "PopupProc subclass error");
  }
  SetWindowLong (c, GWL_WNDPROC, (LONG) lpfnNewPopupProc);
  fntptr = (Nlm_FntPtr) Nlm_HandLock (Nlm_systemFont);
  SetWindowFont(c, fntptr->handle, FALSE);
  Nlm_HandUnlock(Nlm_systemFont);

#endif

  if (nextMenuNum < 32767)
    nextMenuNum++;
}
#endif  /* WIN_MSWIN | WIN_MAC */


#ifdef WIN_MAC
static void Nlm_NewSubMenu (Nlm_MenU m)

{
  Nlm_MenuTool  h;

  h = NewMenu (nextMenuNum, (StringPtr) "");
  Nlm_LoadMenuData (m, h, NULL, nextMenuNum, NULL);
  if (nextMenuNum < 32767) {
    nextMenuNum++;
  }
}

static void Nlm_NewSubmenuItem (Nlm_IteM i, Nlm_MenU sub, Nlm_CharPtr title)

{
  Nlm_MenuTool  h;
  Nlm_Int2      index;
  Nlm_MenU      m;

  m = (Nlm_MenU) Nlm_GetParent ((Nlm_GraphiC) i);
  Nlm_AppendItems (m, i, title, CASCADE_ITEM, FALSE);
  index = Nlm_CountMenuItems ((Nlm_GraphiC) m);
  Nlm_LoadItemData (i, sub, index, NULL);
  h = Nlm_GetMenuHandle (sub);
  InsertMenu (h, -1);
}
#endif

#ifdef WIN_MSWIN
static void Nlm_NewSubMenuAndItem (Nlm_IteM i, Nlm_MenU sub, Nlm_CharPtr title)
{
  Nlm_MenuTool  h;
  Nlm_Int2      index;
  Nlm_MenU      m;
  Nlm_MenuTool  mh;

  m = (Nlm_MenU) Nlm_GetParent ((Nlm_GraphiC) i);
  mh = Nlm_GetMenuHandle (m);
  h = CreateMenu ();
  AppendMenu (mh, MF_POPUP | MF_ENABLED, (UINT) h, title);
  Nlm_LoadMenuData (sub, h, NULL, nextMenuNum, NULL);
  if (nextMenuNum < 32767) {
    nextMenuNum++;
  }
  index = Nlm_CountMenuItems ((Nlm_GraphiC) m);
  Nlm_LoadItemData (i, sub, index, NULL);
}
#endif


#ifdef WIN_MOTIF
static void Nlm_NewCascadingMenu (Nlm_MenU m, Nlm_IteM i,
				  Nlm_MenU sub, Nlm_CharPtr title)

{
  Nlm_MenuTool   h;
  Nlm_MenuTool   mh;
  Nlm_PopupTool  pop;
  Cardinal       n;
  Arg            wargs [10];

  mh = Nlm_GetMenuHandle (m);

  n = 0;
  Nlm_SetupCmapVis((Nlm_GraphiC)m, wargs, &n);
  h = XmCreatePulldownMenu (mh, (String) "", wargs, n);
  XtRealizeWidget( h );

  n = 0;
  XtSetArg (wargs [n], XmNfontList, Nlm_XfontList); n++;
  XtSetArg (wargs [n], XmNmarginHeight, 5); n++;
  XtSetArg (wargs [n], XmNmarginWidth, 5); n++;
  XtSetArg (wargs [n], XmNborderWidth, (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNsubMenuId, h); n++;
/*  XtSetArg (wargs [n], XmNrecomputeSize, FALSE); n++;*/
  pop = XmCreateCascadeButton (mh, (String) "", wargs, n);
  Nlm_AddTitleAccel(pop, title, 0);
  XtManageChild (pop);
  
  Nlm_LoadMenuData (sub, h, NULL, nextMenuNum, pop);
  Nlm_LoadItemData (i, sub, 0, pop);
}
#endif



static void Nlm_NewComm (Nlm_IteM i, Nlm_CharPtr title, Nlm_ItmActnProc actn)

{
  Nlm_Int2      index;
  Nlm_ItemTool  itool;
  Nlm_MenU      m;
  Nlm_Char      temp [256];

  m = (Nlm_MenU) Nlm_GetParent ((Nlm_GraphiC) i);
/* M.I */
  Nlm_PrepareTitle (temp, title, sizeof (temp), FALSE);
/* M.I */
  itool = Nlm_AppendItems (m, i, temp, COMMAND_ITEM, FALSE);
  index = Nlm_CountMenuItems ((Nlm_GraphiC) m);
  Nlm_LoadItemData (i, NULL, index, itool);
  Nlm_LoadAction ((Nlm_GraphiC) i, (Nlm_ActnProc) actn);
#ifdef WIN_MOTIF
  XtManageChild (itool);
#endif
}


static void Nlm_NewStat (Nlm_IteM i, Nlm_CharPtr title, Nlm_ItmActnProc actn)

{
  Nlm_Int2      index;
  Nlm_ItemTool  itool;
  Nlm_MenU      m;
  Nlm_Char      temp [256];

  m = (Nlm_MenU) Nlm_GetParent ((Nlm_GraphiC) i);
/* M.I */
  Nlm_PrepareTitle (temp, title, sizeof (temp), FALSE);
/* M.I */
  itool = Nlm_AppendItems (m, i, temp, STATUS_ITEM, FALSE);
  index = Nlm_CountMenuItems ((Nlm_GraphiC) m);
  Nlm_LoadItemData (i, NULL, index, itool);
  Nlm_LoadAction ((Nlm_GraphiC) i, (Nlm_ActnProc) actn);
#ifdef WIN_MOTIF
  XtManageChild (itool);
#endif
}


static void Nlm_NewChoice (Nlm_ChoicE c, Nlm_ChsActnProc actn)

{
  Nlm_Int2  index;
  Nlm_MenU  m;

  m = (Nlm_MenU) Nlm_GetParent ((Nlm_GraphiC) c);
  index = Nlm_CountMenuItems ((Nlm_GraphiC) m);
  Nlm_LoadItemData ((Nlm_IteM) c, NULL, (Nlm_Int2)(index + 1), NULL);
  Nlm_LoadChoiceData (c, 0, FALSE, 0, NULL);
  Nlm_LoadAction ((Nlm_GraphiC) c, (Nlm_ActnProc) actn);
}


#if defined(WIN_MAC)  ||  defined(WIN_MSWIN)
static void Nlm_NewPopListChoice (Nlm_ChoicE c, Nlm_PupActnProc actn)
{
  Nlm_Int2  index;
  Nlm_MenU  sub;
#ifdef WIN_MAC
  Nlm_MenU  m;
#endif

  sub = NULL;
  index = 0;
#ifdef WIN_MAC
  m = (Nlm_MenU) Nlm_GetParent ((Nlm_GraphiC) c);
  index = Nlm_CountMenuItems ((Nlm_GraphiC) m);
#endif
#ifdef WIN_MSWIN
  index = 0;
#endif
  Nlm_LoadItemData ((Nlm_IteM) c, sub, (Nlm_Int2)(index + 1), NULL);
  Nlm_LoadChoiceData (c, 0, TRUE, 0, NULL);
  Nlm_LoadAction ((Nlm_GraphiC) c, (Nlm_ActnProc) actn);
}
#endif  /* WIN_MAC | WIN_MSWIN */


#ifdef WIN_MAC
static void Nlm_NewDeskAcc (Nlm_ChoicE c)
{
  Nlm_Int2  frst;
  Nlm_Int2  index;
  Nlm_MenU  m;

  m = (Nlm_MenU) Nlm_GetParent ((Nlm_GraphiC) c);
  index = Nlm_CountMenuItems ((Nlm_GraphiC) m);
  frst = index + 1;
  Nlm_AppendResources (m, 'DRVR', FALSE);
  index = Nlm_CountMenuItems ((Nlm_GraphiC) m);
  Nlm_LoadItemData ((Nlm_IteM) c, NULL, frst, NULL);
  Nlm_LoadChoiceData (c, index + 1 - frst, FALSE, 0, NULL);
}
#endif  /* WIN_MAC */


static void Nlm_NewFontGroup (Nlm_ChoicE c)

{
#ifdef WIN_MAC
  Nlm_MenU  m;
  Nlm_Int2  newval;
  Nlm_Int2  oldval;

  m = (Nlm_MenU) Nlm_GetParent ((Nlm_GraphiC) c);
  oldval = Nlm_CountMenuItems ((Nlm_GraphiC) m);
  Nlm_AppendResources (m, 'FOND', FALSE);
  newval = Nlm_CountMenuItems ((Nlm_GraphiC) m);
  Nlm_LoadItemData ((Nlm_IteM) c, NULL, oldval + 1, NULL);
  Nlm_LoadChoiceData (c, newval - oldval, FALSE, 0, NULL);
#endif
#ifdef WIN_MSWIN
  /*
  WNDPROC   inst;
  */
  Nlm_MenU  m;
  Nlm_Int2  newval;
  Nlm_Int2  oldval;

  m = (Nlm_MenU) Nlm_GetParent ((Nlm_GraphiC) c);
  oldval = Nlm_CountMenuItems ((Nlm_GraphiC) m);
  /*
  inst = (WNDPROC) MakeProcInstance (EnumAllFaces, Nlm_currentHInst);
  FreeProcInstance (inst);
  */
  newval = Nlm_CountMenuItems ((Nlm_GraphiC) m);
  Nlm_LoadItemData ((Nlm_IteM) c, NULL, (Nlm_Int2)(oldval + 1), NULL);
  Nlm_LoadChoiceData (c, (Nlm_Int2)(newval - oldval), FALSE, 0, NULL);
#endif
#ifdef WIN_MOTIF
#endif
}

#ifdef WIN_MAC
static Nlm_MenuBaR Nlm_CreateDesktopMenuBar (Nlm_WindoW w)

{
  Nlm_MenuBaR      mb;
  Nlm_MenuBarData  mbdata;
  Nlm_RecT         r;

  Nlm_LoadRect (&r, 0, 0, 0, 0);
  mb = (Nlm_MenuBaR) Nlm_HandNew (sizeof (Nlm_MenuBarRec));
  if (mb != NULL) {
    Nlm_LoadGraphicData ((Nlm_GraphiC) mb, NULL, (Nlm_GraphiC) w, NULL, NULL,
			 desktopMenuBarProcs, NULL, &r, TRUE, FALSE, NULL, NULL);
    if (Nlm_nextIdNumber < 32767) {
      Nlm_nextIdNumber++;
    }
    Nlm_SetWindowMenuBar (w, mb);
    Nlm_SetVisible ((Nlm_GraphiC) mb, TRUE);
    Nlm_GetMenuBarData (mb, &mbdata);
    mbdata.notJustSubMenu = FALSE;
    Nlm_SetMenuBarData (mb, &mbdata);
  }
  return mb;
}
#endif

static Nlm_MenuBaR Nlm_CreateWindowMenuBar (Nlm_WindoW w)

{
  Nlm_MenuBaR  mb;
  Nlm_RecT     r;
#ifdef WIN_MAC
  Nlm_BoxData  bdata;
#endif

  Nlm_LoadRect (&r, 0, 0, 0, 20);
  mb = (Nlm_MenuBaR) Nlm_HandNew (sizeof (Nlm_MenuBarRec));
  if (mb != NULL) {
    Nlm_LoadGraphicData ((Nlm_GraphiC) mb, NULL, (Nlm_GraphiC) w, NULL, NULL,
			 windowMenuBarProcs, NULL, &r, TRUE, FALSE, NULL, NULL);
    if (Nlm_nextIdNumber < 32767) {
      Nlm_nextIdNumber++;
    }
  }
  if (mb != NULL) {
#ifdef WIN_MAC
    Nlm_DoAdjustPrnt ((Nlm_GraphiC) mb, &r, TRUE, FALSE);
    Nlm_GetBoxData ((Nlm_BoX) w, &bdata);
    bdata.topRow = bdata.limitPoint.y + bdata.yMargin;
    bdata.nextCol = bdata.resetPoint.x;
    bdata.nextPoint.y = bdata.topRow;
    bdata.nextPoint.x = bdata.nextCol;
    Nlm_SetBoxData ((Nlm_BoX) w, &bdata);
#endif
    Nlm_SetWindowMenuBar (w, mb);
    Nlm_NewMenuBar (mb);
    Nlm_DoShow ((Nlm_GraphiC) mb, TRUE, FALSE);
  }
  return mb;
}

extern Nlm_MenU Nlm_PulldownMenu (Nlm_WindoW w, Nlm_CharPtr title)

{
  Nlm_MenU         m;
  Nlm_MenuBaR      mb;
  Nlm_RecT         r;
  Nlm_WindoW       tempPort;
#ifdef WIN_MAC
  Nlm_MenuBarData  mbdata;
  Nlm_PoinT        npt;
#endif
#ifdef WIN_MSWIN
  Nlm_PoinT        npt;
#endif

  m = NULL;
  if (w != NULL && title != NULL && w == Nlm_GetParentWindow ((Nlm_GraphiC) w)) {
    tempPort = Nlm_SavePort ((Nlm_GraphiC) w);
    Nlm_SelectFont (Nlm_systemFont);
    mb = Nlm_GetWindowMenuBar (w);
    if (mb == NULL) {
      mb = Nlm_CreateWindowMenuBar (w);
    }
#ifdef WIN_MAC
    Nlm_GetNextPosition ((Nlm_GraphiC) mb, &npt);
    r.top = npt.y;
    r.left = npt.x;
    r.bottom = r.top + 20;
    r.right = r.left + Nlm_StringWidth (title) + 5;
    Nlm_RecordRect ((Nlm_GraphiC) mb, &r);
    Nlm_Advance ((Nlm_GraphiC) mb);
#endif
#ifdef WIN_MSWIN
    Nlm_GetNextPosition ((Nlm_GraphiC) mb, &npt);
    r.top = npt.y;
    r.left = npt.x;
    r.bottom = r.top;
    r.right = (Nlm_Int2)(r.left + Nlm_StringWidth(title) + 50);
    Nlm_DoAdjustPrnt ((Nlm_GraphiC) mb, &r, TRUE, FALSE);
    Nlm_Advance ((Nlm_GraphiC) mb);
#endif
#ifdef WIN_MOTIF
    Nlm_LoadRect (&r, 0, 0, 0, 0);
#endif
    m = (Nlm_MenU) Nlm_CreateLink ((Nlm_GraphiC) mb, &r, sizeof (Nlm_MenuRec),
				   windowPulldownProcs);
    if (m != NULL) {
      r.top += 3;
      Nlm_NewPopup (m, title, &r);
      Nlm_DoShow ((Nlm_GraphiC) m, TRUE, FALSE);
    }
    Nlm_RestorePort (tempPort);
  } else if (title != NULL) {
#ifdef WIN_MAC
    w = Nlm_desktopWindow;
    mb = Nlm_GetWindowMenuBar (w);
    if (mb == NULL) {
      mb = Nlm_CreateDesktopMenuBar (w);
    }
    if (mb != NULL) {
      Nlm_GetMenuBarData (mb, &mbdata);
      mbdata.notJustSubMenu = TRUE;
      Nlm_SetMenuBarData (mb, &mbdata);
    }
    Nlm_LoadRect (&r, 0, 0, 0, 0);
    m = (Nlm_MenU) Nlm_CreateLink ((Nlm_GraphiC) mb, &r, sizeof (Nlm_MenuRec),
				   desktopPulldownProcs);
    if (m != NULL) {
      Nlm_NewDesktopMenu (m, title);
      Nlm_DoShow ((Nlm_GraphiC) m, TRUE, TRUE);
    }
#endif
  }
  return m;
}

extern Nlm_MenU Nlm_AppleMenu (Nlm_WindoW w)

{
  Nlm_MenU  m;
#ifdef WIN_MAC
  Nlm_Char  appleStr [2] = {'\24', '\0'};
#endif

  m = NULL;
#ifdef WIN_MAC
  m = Nlm_PulldownMenu (w, appleStr);
#endif
  return m;
}

extern Nlm_MenU Nlm_PopupMenu (Nlm_GrouP prnt, Nlm_CharPtr title)

{
  Nlm_MenU    m;
  Nlm_PoinT   npt;
  Nlm_RecT    p;
  Nlm_RecT    r;
  Nlm_WindoW  tempPort;
  Nlm_Int2    wid;

  m = NULL;
  if (prnt != NULL && title != NULL) {
    tempPort = Nlm_SavePort ((Nlm_GraphiC) prnt);
    Nlm_GetNextPosition ((Nlm_GraphiC) prnt, &npt);
    Nlm_SelectFont (Nlm_systemFont);
    wid = Nlm_StringWidth (title);
    r.left = npt.x;
    r.top = npt.y;
    r.right  = (Nlm_Int2)(r.left + wid + 10);
    r.bottom = (Nlm_Int2)(r.top + Nlm_stdLineHeight + 8);
    m = (Nlm_MenU) Nlm_CreateLink ((Nlm_GraphiC) prnt, &r, sizeof (Nlm_MenuRec), popupProcs);
    if (m != NULL) {
      p = r;
      Nlm_UpsetRect (&p, 1, 1, 2, 2);
      Nlm_NewFloatingPopup (m, title, &p);
      Nlm_DoAdjustPrnt ((Nlm_GraphiC) m, &r, TRUE, FALSE);
      Nlm_DoShow ((Nlm_GraphiC) m, TRUE, FALSE);
    }
    Nlm_RestorePort (tempPort);
  }
  return m;
}

extern Nlm_MenU Nlm_SubMenu (Nlm_MenU m, Nlm_CharPtr title)
/*
*  WARNING -- Sub menus on the Macintosh link into the desktop window menu
*  bar in addition to their own parent items, so removing windows with
*  sub menus in them is dangerous.
*/
{
  Nlm_IteM     i;
  Nlm_RecT     r;
  Nlm_MenU     sub = NULL;
  Nlm_Char     temp [256];
#ifdef WIN_MAC
  Nlm_Int2     len;
  Nlm_MenuBaR  mb;
  Nlm_WindoW   w;
  Nlm_Int2     tag;
#endif

  if (!m  || !title)
    return NULL;

#ifdef WIN_MAC
  w = Nlm_desktopWindow;
  mb = Nlm_GetWindowMenuBar (w);
  if (mb == NULL) {
    mb = Nlm_CreateDesktopMenuBar (w);
  }
  Nlm_LoadRect (&r, 0, 0, 0, 0);
  sub = (Nlm_MenU) Nlm_CreateLink ((Nlm_GraphiC) mb, &r, sizeof (Nlm_MenuRec), subMenuProcs);
  if ( !sub )
    return NULL;

  Nlm_NewSubMenu (sub);
  Nlm_SetVisible ((Nlm_GraphiC) sub, TRUE);
  Nlm_PrepareTitle (temp, title, sizeof (temp), TRUE);
  len = (Nlm_Int2) Nlm_StringLen (temp);
  tag = Nlm_GetMenuTag (sub);
  if (len < 250) {
    temp [len] = '/';
    temp [len + 1] = '\33';
    temp [len + 2] = '!';
    temp [len + 3] = (Nlm_Char) tag;
    temp [len + 4] = '\0';
  }
  i = (Nlm_IteM) Nlm_CreateLink ((Nlm_GraphiC) m, &r, sizeof (Nlm_ItemRec), subItemProcs);
  if (i != NULL) {
    Nlm_NewSubmenuItem (i, sub, temp);
    Nlm_SetParent ((Nlm_GraphiC) sub, (Nlm_GraphiC) i);
    Nlm_SetVisible ((Nlm_GraphiC) i, TRUE);
  }
#endif

#if defined(WIN_MSWIN) || defined(WIN_MOTIF)
  sub = (Nlm_MenU) Nlm_HandNew( sizeof(Nlm_MenuRec) );
  if ( !sub )
    return NULL;

  Nlm_LoadRect(&r, 0, 0, 0, 0);
  Nlm_LoadGraphicData((Nlm_GraphiC)sub, NULL, (Nlm_GraphiC)m, NULL, NULL,
                      subMenuProcs, NULL, &r, TRUE, FALSE, NULL, NULL);
  if (Nlm_nextIdNumber < 32767)
    Nlm_nextIdNumber++;
  Nlm_PrepareTitle(temp, title, sizeof(temp), TRUE);
  i = (Nlm_IteM)Nlm_CreateLink((Nlm_GraphiC)m,
                               &r, sizeof(Nlm_ItemRec), subItemProcs);
  if (i != NULL) {
#ifdef WIN_MSWIN
    Nlm_NewSubMenuAndItem(i, sub, temp);
#else /* WIN_MOTIF */
    Nlm_NewCascadingMenu(m, i, sub, temp);
#endif
    Nlm_SetParent ((Nlm_GraphiC) sub, (Nlm_GraphiC)i);
    Nlm_SetVisible((Nlm_GraphiC) sub, TRUE);
    Nlm_SetVisible((Nlm_GraphiC)   i, TRUE);
  }
#endif

  return sub;
}

extern Nlm_IteM Nlm_CommandItem (Nlm_MenU m, Nlm_CharPtr title, Nlm_ItmActnProc actn)

{
  Nlm_IteM  i;
  Nlm_RecT  r;

  i = NULL;
  if (m != NULL && title != NULL) {
    Nlm_LoadRect (&r, 0, 0, 0, 0);
    i = (Nlm_IteM) Nlm_CreateLink ((Nlm_GraphiC) m, &r, sizeof (Nlm_ItemRec), commandItemProcs);
    if (i != NULL) {
      Nlm_NewComm (i, title, actn);
      Nlm_SetVisible ((Nlm_GraphiC) i, TRUE);
    }
  }
  return i;
}

extern Nlm_IteM Nlm_StatusItem (Nlm_MenU m, Nlm_CharPtr title, Nlm_ItmActnProc actn)

{
  Nlm_IteM  i;
  Nlm_RecT  r;

  i = NULL;
  if (m != NULL && title != NULL) {
    Nlm_LoadRect (&r, 0, 0, 0, 0);
    i = (Nlm_IteM) Nlm_CreateLink ((Nlm_GraphiC) m, &r, sizeof (Nlm_ItemRec), statusItemProcs);
    if (i != NULL) {
      Nlm_NewStat (i, title, actn);
      Nlm_SetVisible ((Nlm_GraphiC) i, TRUE);
    }
  }
  return i;
}


extern void Nlm_SeparatorItem (Nlm_MenU m)

{
  Nlm_MenuTool  h;
#ifdef WIN_MAC
  Nlm_Char      temp [256];
#endif
#ifdef WIN_MSWIN
  Nlm_IteM      i;
  Nlm_RecT      r;
#endif
#ifdef WIN_MOTIF
  Nlm_ItemTool  itool;
  Cardinal      n;
  Arg           wargs [10];
#endif

  if (m != NULL) {
    h = Nlm_GetMenuHandle (m);
#ifdef WIN_MAC
    Nlm_StringNCpy_0(temp, "(-", sizeof(temp));
    Nlm_CtoPstr (temp);
    AppendMenu (h, (StringPtr) temp);
#endif
#ifdef WIN_MSWIN
    Nlm_LoadRect (&r, 0, 0, 0, 0);
    i = (Nlm_IteM) Nlm_CreateLink ((Nlm_GraphiC) m, &r, sizeof (Nlm_ItemRec), statusItemProcs);
    if (i != NULL) {
      Nlm_NewStat (i, NULL, NULL);
      Nlm_SetVisible ((Nlm_GraphiC) i, TRUE);
    }
    AppendMenu (h, MF_SEPARATOR, 0, NULL);
#endif
#ifdef WIN_MOTIF
    n = 0;
    XtSetArg (wargs [n], XmNborderWidth, (Dimension) 0); n++;
    itool = XmCreateSeparator (h, (String) "", wargs, n);
    XtManageChild (itool);
#endif
  }
}


extern Nlm_ChoicE Nlm_ChoiceGroup (Nlm_MenU m, Nlm_ChsActnProc actn)

{
  Nlm_ChoicE  c;
  Nlm_RecT    r;

  c = NULL;
  if (m != NULL) {
    Nlm_LoadRect (&r, 0, 0, 0, 0);
    c = (Nlm_ChoicE) Nlm_CreateLink ((Nlm_GraphiC) m, &r, sizeof (Nlm_ChoiceRec),
				     choiceGroupProcs);
    if (c != NULL) {
      Nlm_NewChoice (c, actn);
      Nlm_SetVisible ((Nlm_GraphiC) c, TRUE);
    }
  }
  return c;
}

extern Nlm_PopuP Nlm_PopupList (Nlm_GrouP prnt, Nlm_Boolean macLike,
				Nlm_PupActnProc actn)

{
  Nlm_ChoicE   c;
  Nlm_GphPrcs  PNTR classPtr;
  Nlm_Int2     hbounds;
  Nlm_MenU     m;
  Nlm_PoinT    npt;
  Nlm_RecT     r;
  Nlm_WindoW   tempPort;

  c = NULL;
  if (prnt != NULL) {
    tempPort = Nlm_SavePort ((Nlm_GraphiC) prnt);
    Nlm_GetNextPosition ((Nlm_GraphiC) prnt, &npt);
    Nlm_SelectFont (Nlm_systemFont);
    if (macLike) {
      classPtr = macPopListProcs;
    } else {
      classPtr = msPopListProcs;
    }
#ifdef WIN_MAC
    hbounds = 13;
#endif
#ifdef WIN_MSWIN
    hbounds = 13;
#endif
#ifdef WIN_MOTIF
    hbounds = 26;
#endif
    r.left = npt.x;
    r.top  = npt.y;
    r.right  = (Nlm_Int2)(r.left + hbounds * 2);
    r.bottom = (Nlm_Int2)(r.top + Nlm_popupMenuHeight);
#ifdef WIN_MAC
    m = (Nlm_MenU) Nlm_CreateLink ((Nlm_GraphiC) prnt, &r, sizeof (Nlm_MenuRec), classPtr);
    if (m != NULL) {
      Nlm_NewPopListMenu (m);
      Nlm_DoAdjustPrnt ((Nlm_GraphiC) m, &r, TRUE, FALSE);
      Nlm_DoShow ((Nlm_GraphiC) m, TRUE, FALSE);
      c = (Nlm_ChoicE) Nlm_CreateLink ((Nlm_GraphiC) m, &r, sizeof (Nlm_ChoiceRec),
				       popUpChoiceProcs);
      if (c != NULL) {
	Nlm_NewPopListChoice (c, actn);
	Nlm_SetVisible ((Nlm_GraphiC) c, TRUE);
      }
    }
#endif
#ifdef WIN_MSWIN
    m = (Nlm_MenU) Nlm_CreateLink ((Nlm_GraphiC) prnt, &r, sizeof (Nlm_MenuRec), classPtr);
    if (m != NULL) {
      Nlm_NewPopListMenu (m);
      Nlm_DoAdjustPrnt ((Nlm_GraphiC) m, &r, TRUE, FALSE);
      Nlm_DoShow ((Nlm_GraphiC) m, TRUE, FALSE);
      c = (Nlm_ChoicE) Nlm_CreateLink ((Nlm_GraphiC) m, &r, sizeof (Nlm_ChoiceRec),
				       popUpChoiceProcs);
      if (c != NULL) {
	Nlm_NewPopListChoice (c, actn);
	Nlm_SetVisible ((Nlm_GraphiC) c, TRUE);
      }
    }
#endif
#ifdef WIN_MOTIF
    m = (Nlm_MenU) Nlm_CreateLink ((Nlm_GraphiC) prnt, &r, sizeof (Nlm_MenuRec), classPtr);
    if (m != NULL) {
      c = (Nlm_ChoicE) Nlm_CreateLink ((Nlm_GraphiC) m, &r, sizeof (Nlm_ChoiceRec),
				       popUpChoiceProcs);
      if (c != NULL) {
	Nlm_NewPopupList (m, c, actn);
	Nlm_DoAdjustPrnt ((Nlm_GraphiC) m, &r, TRUE, FALSE);
	Nlm_SetVisible ((Nlm_GraphiC) c, TRUE);
	Nlm_DoShow ((Nlm_GraphiC) m, TRUE, FALSE);
      }
    }
#endif
    Nlm_RestorePort (tempPort);
  }
  return (Nlm_PopuP) c;
}

extern Nlm_IteM Nlm_ChoiceItem (Nlm_ChoicE c, Nlm_CharPtr title)

{
  Nlm_IteM  i;

  i = NULL;
  if (c != NULL && title != NULL) {
    if (! Nlm_IsItAPopupList (c)) {
      i = Nlm_AppendChoice (c, title, FALSE, FALSE);
    }
  }
  return i;
}

extern void Nlm_PopupItem (Nlm_PopuP p, Nlm_CharPtr title)

{
  Nlm_ChoicE  c;

  c = (Nlm_ChoicE) p;
  if (c != NULL && title != NULL) {
    if (Nlm_IsItAPopupList (c)) {
      Nlm_AppendPopList (c, title, TRUE);
    }
  }
}

extern void Nlm_DeskAccGroup (Nlm_MenU m)

{
#ifdef WIN_MAC
  Nlm_ChoicE  c;
  Nlm_RecT    r;

  c = NULL;
  if (m != NULL) {
    Nlm_LoadRect (&r, 0, 0, 0, 0);
    c = (Nlm_ChoicE) Nlm_CreateLink ((Nlm_GraphiC) m, &r, sizeof (Nlm_ChoiceRec), daProcs);
    if (c != NULL) {
      Nlm_NewDeskAcc (c);
      Nlm_SetVisible ((Nlm_GraphiC) c, TRUE);
    }
  }
#endif
}

extern Nlm_ChoicE Nlm_FontGroup (Nlm_MenU m)

{
  Nlm_ChoicE  c;
  Nlm_RecT    r;

  c = NULL;
  if (m != NULL) {
    Nlm_LoadRect (&r, 0, 0, 0, 0);
    c = (Nlm_ChoicE) Nlm_CreateLink ((Nlm_GraphiC) m, &r, sizeof (Nlm_ChoiceRec),
				     choiceGroupProcs);
    if (c != NULL) {
      Nlm_NewFontGroup (c);
      Nlm_SetVisible ((Nlm_GraphiC) c, TRUE);
    }
  }
  return c;
}

extern void Nlm_FreeMenus (void)

{
  gphprcsptr = (Nlm_GphPrcsPtr) Nlm_MemFree (gphprcsptr);
}

extern void Nlm_InitMenus (void)

{
  gphprcsptr = (Nlm_GphPrcsPtr) Nlm_MemNew (sizeof (Nlm_GphPrcs) * 15);

#ifdef WIN_MAC
  desktopMenuBarProcs = &(gphprcsptr [0]);
  desktopMenuBarProcs->click = Nlm_DesktopMenuBarClick;
  desktopMenuBarProcs->key = Nlm_DesktopMenuBarKey;
  desktopMenuBarProcs->remove = Nlm_RemoveDesktopMenuBar;
  desktopMenuBarProcs->linkIn = Nlm_LinkIn;

  desktopPulldownProcs = &(gphprcsptr [1]);
  desktopPulldownProcs->click = Nlm_DesktopMenuClick;
  desktopPulldownProcs->key = Nlm_DesktopMenuKey;
  desktopPulldownProcs->draw = Nlm_DrawDesktopMenu;
  desktopPulldownProcs->show = Nlm_ShowDesktopMenu;
  desktopPulldownProcs->hide = Nlm_HideDesktopMenu;
  desktopPulldownProcs->enable = Nlm_EnableDesktopMenu;
  desktopPulldownProcs->disable = Nlm_DisableDesktopMenu;
  desktopPulldownProcs->remove = Nlm_RemoveDesktopMenu;
  desktopPulldownProcs->reset = Nlm_ResetMenu;
  desktopPulldownProcs->countItems = Nlm_CountMenuItems;
  desktopPulldownProcs->linkIn = Nlm_LinkIn;
#endif

  windowMenuBarProcs = &(gphprcsptr [2]);
#ifdef WIN_MAC
  windowMenuBarProcs->click = Nlm_WindowMenuBarClick;
  windowMenuBarProcs->key = NULL;
  windowMenuBarProcs->draw = Nlm_DrawWindowMenuBar;
#endif
#ifdef WIN_MSWIN
  windowMenuBarProcs->command = Nlm_WindowMenuBarCommand;
#endif
  windowMenuBarProcs->show = Nlm_ShowWindowMenuBar;
  windowMenuBarProcs->remove = Nlm_RemoveWindowMenuBar;
  windowMenuBarProcs->linkIn = Nlm_LinkIn;

  windowPulldownProcs = &(gphprcsptr [3]);
#ifdef WIN_MAC
  windowPulldownProcs->click = Nlm_PulldownClick;
  windowPulldownProcs->key = Nlm_PopupKey;
  windowPulldownProcs->draw = Nlm_DrawWindowMenu;
#endif
#ifdef WIN_MSWIN
  windowPulldownProcs->command = Nlm_PulldownCommand;
#endif
  windowPulldownProcs->show = Nlm_ShowPopup;
  windowPulldownProcs->hide = Nlm_HidePulldown;
  windowPulldownProcs->enable = Nlm_EnablePulldown;
  windowPulldownProcs->disable = Nlm_DisablePulldown;
  windowPulldownProcs->remove = Nlm_RemovePopupMenu;
  windowPulldownProcs->reset = Nlm_ResetMenu;
  windowPulldownProcs->countItems = Nlm_CountMenuItems;
  windowPulldownProcs->linkIn = Nlm_LinkIn;

  popupProcs = &(gphprcsptr [4]);
#ifdef WIN_MAC
  popupProcs->click = Nlm_PopupClick;
  popupProcs->key = Nlm_PopupKey;
  popupProcs->draw = Nlm_DrawPopup;
#endif
  popupProcs->show = Nlm_ShowPopup;
  popupProcs->hide = Nlm_HidePopup;
  popupProcs->enable = Nlm_EnablePopup;
  popupProcs->disable = Nlm_DisablePopup;
  popupProcs->remove = Nlm_RemovePopupMenu;
  popupProcs->reset = Nlm_ResetMenu;
  popupProcs->countItems = Nlm_CountMenuItems;
  popupProcs->linkIn = Nlm_LinkIn;
  popupProcs->setPosition = Nlm_SetPopupPosition;
  popupProcs->getPosition = Nlm_GetPopupPosition;

  subMenuProcs = &(gphprcsptr [5]);
#ifdef WIN_MAC
  subMenuProcs->click = Nlm_DesktopMenuClick;
  subMenuProcs->key = Nlm_SubMenuKey;
#endif
  subMenuProcs->enable = Nlm_EnableSubMenu;
  subMenuProcs->disable = Nlm_DisableSubMenu;
  subMenuProcs->remove = Nlm_RemoveSubMenu;
  subMenuProcs->reset = Nlm_ResetMenu;
  subMenuProcs->countItems = Nlm_CountMenuItems;
  subMenuProcs->linkIn = Nlm_LinkIn;
  subMenuProcs->setTitle = Nlm_SetSubmenuTitle;
  subMenuProcs->getTitle = Nlm_GetSubmenuTitle;

  subItemProcs = &(gphprcsptr [6]);
#ifdef WIN_MAC
  subItemProcs->click = Nlm_SubItemClick;
#endif
#ifdef WIN_MSWIN
  subItemProcs->command = Nlm_SubItemCommand;
#endif
  subItemProcs->enable = Nlm_EnableMenuItem;
  subItemProcs->disable = Nlm_DisableMenuItem;
#ifdef WIN_MAC
  subItemProcs->remove = Nlm_RemoveMenuItemButNotSub;
#else
  subItemProcs->remove = Nlm_RemoveMenuItem;
#endif
  subItemProcs->linkIn = Nlm_LinkIn;
  subItemProcs->setTitle = Nlm_SetSingleTitle;
  subItemProcs->getTitle = Nlm_GetSingleTitle;

  commandItemProcs = &(gphprcsptr [7]);
#ifdef WIN_MAC
  commandItemProcs->click = Nlm_CommItemClick;
  commandItemProcs->key = Nlm_CommItemKey;
#endif
#ifdef WIN_MSWIN
  commandItemProcs->command = Nlm_CommItemCommand;
#endif
#ifdef WIN_MOTIF
  commandItemProcs->callback = Nlm_CommItemCallback;
#endif
  commandItemProcs->enable = Nlm_EnableMenuItem;
  commandItemProcs->disable = Nlm_DisableMenuItem;
  commandItemProcs->remove = Nlm_RemoveMenuItem;
  commandItemProcs->setTitle = Nlm_SetSingleTitle;
  commandItemProcs->getTitle = Nlm_GetSingleTitle;

  statusItemProcs = &(gphprcsptr [8]);
#ifdef WIN_MAC
  statusItemProcs->click = Nlm_StatItemClick;
  statusItemProcs->key = Nlm_StatItemKey;
#endif
#ifdef WIN_MSWIN
  statusItemProcs->command = Nlm_StatItemCommand;
#endif
#ifdef WIN_MOTIF
  statusItemProcs->callback = Nlm_StatItemCallback;
#endif
  statusItemProcs->enable = Nlm_EnableMenuItem;
  statusItemProcs->disable = Nlm_DisableMenuItem;
  statusItemProcs->remove = Nlm_RemoveMenuItem;
  statusItemProcs->setTitle = Nlm_SetSingleTitle;
  statusItemProcs->getTitle = Nlm_GetSingleTitle;
  statusItemProcs->setStatus = Nlm_SetStatValue;
  statusItemProcs->getStatus = Nlm_GetStatValue;

  choiceGroupProcs = &(gphprcsptr [9]);
#ifdef WIN_MAC
  choiceGroupProcs->click = Nlm_ChoiceGroupClick;
  choiceGroupProcs->key = Nlm_ChoiceGroupKey;
#endif
#ifdef WIN_MSWIN
  choiceGroupProcs->command = Nlm_ChoiceGroupCommand;
#endif
#ifdef WIN_MOTIF
  choiceGroupProcs->callback = Nlm_ChoiceGroupCallback;
#endif
  choiceGroupProcs->enable = Nlm_EnableChoiceGroup;
  choiceGroupProcs->disable = Nlm_DisableChoice;
  choiceGroupProcs->remove = Nlm_RemoveChoiceGroup;
  choiceGroupProcs->linkIn = Nlm_LinkIn;
  choiceGroupProcs->countItems = Nlm_CountChoiceItems;
  choiceGroupProcs->setTitle = Nlm_SetChoiceGroupTitle;
  choiceGroupProcs->getTitle = Nlm_GetChoiceGroupTitle;
  choiceGroupProcs->setValue = Nlm_SetChoiceGroupValue;
  choiceGroupProcs->getValue = Nlm_GetChoiceGroupValue;

  choiceItemProcs = &(gphprcsptr [10]);
  choiceItemProcs->enable = Nlm_EnableChoiceItem;
  choiceItemProcs->disable = Nlm_DisableChoiceItem;
  choiceItemProcs->remove = Nlm_RemoveChoiceItem;
  choiceItemProcs->setTitle = Nlm_SetChoiceItemTitle;
  choiceItemProcs->getTitle = Nlm_GetChoiceItemTitle;
  choiceItemProcs->setStatus = NULL;
  choiceItemProcs->getStatus = NULL;

  macPopListProcs = &(gphprcsptr [11]);
#ifdef WIN_MAC
  macPopListProcs->click = Nlm_MacPopListClick;
  macPopListProcs->key = NULL;
  macPopListProcs->draw = Nlm_DrawPopList;
#endif
#ifdef WIN_MSWIN
  macPopListProcs->command = Nlm_MacPopListCommand;
#endif
  macPopListProcs->show = Nlm_ShowPopListMenu;
  macPopListProcs->hide = Nlm_HidePopListMenu;
  macPopListProcs->enable = Nlm_EnablePopList;
  macPopListProcs->disable = Nlm_DisablePopList;
  macPopListProcs->remove = Nlm_RemovePopListMenu;
  macPopListProcs->reset = Nlm_ResetMenu;
  macPopListProcs->countItems = Nlm_CountMenuItems;
  macPopListProcs->linkIn = Nlm_LinkIn;
  macPopListProcs->setPosition = Nlm_SetPopListPosition;
  macPopListProcs->getPosition = Nlm_GetPopListPosition;

  msPopListProcs = &(gphprcsptr [12]);
#ifdef WIN_MAC
  msPopListProcs->click = Nlm_MSPopListClick;
  msPopListProcs->key = NULL;
  msPopListProcs->draw = Nlm_DrawPopList;
#endif
#ifdef WIN_MSWIN
  msPopListProcs->command = Nlm_MSPopListCommand;
#endif
  msPopListProcs->show = Nlm_ShowPopListMenu;
  msPopListProcs->hide = Nlm_HidePopListMenu;
  msPopListProcs->enable = Nlm_EnablePopList;
  msPopListProcs->disable = Nlm_DisablePopList;
  msPopListProcs->remove = Nlm_RemovePopListMenu;
  msPopListProcs->reset = Nlm_ResetMenu;
  msPopListProcs->countItems = Nlm_CountMenuItems;
  msPopListProcs->linkIn = Nlm_LinkIn;
  msPopListProcs->setPosition = Nlm_SetPopListPosition;
  msPopListProcs->getPosition = Nlm_GetPopListPosition;

  popUpChoiceProcs = &(gphprcsptr [13]);
#ifdef WIN_MAC
  popUpChoiceProcs->click = Nlm_PopupChoiceGroupClick;
  popUpChoiceProcs->key = Nlm_ChoiceGroupKey;
#endif
#ifdef WIN_MOTIF
  popUpChoiceProcs->callback = Nlm_PopupChoiceCallback;
#endif
  popUpChoiceProcs->show = Nlm_ShowPopListChoice;
  popUpChoiceProcs->hide = Nlm_HidePopListChoice;
  popUpChoiceProcs->enable = Nlm_EnablePopListChoice;
  popUpChoiceProcs->disable = Nlm_DisablePopListChoice;
  popUpChoiceProcs->remove = Nlm_RemovePopupList;
  popUpChoiceProcs->reset = Nlm_ResetPopList;
  popUpChoiceProcs->countItems = Nlm_CountChoiceItems;
  popUpChoiceProcs->setTitle = Nlm_SetPopListTitle;
  popUpChoiceProcs->getTitle = Nlm_GetPopListTitle;
  popUpChoiceProcs->setValue = Nlm_SetPopListValue;
  popUpChoiceProcs->getValue = Nlm_GetPopListValue;
  popUpChoiceProcs->setPosition = Nlm_SetPopupChoicePosition;
  popUpChoiceProcs->getPosition = Nlm_GetPopupChoicePosition;

  daProcs = &(gphprcsptr [14]);
#ifdef WIN_MAC
  daProcs->click = Nlm_DAClick;
  daProcs->key = Nlm_DAKey;
#endif
  daProcs->enable = Nlm_EnableChoice;
  daProcs->disable = Nlm_DisableChoice;
  daProcs->remove = Nlm_RemoveMenuItem;
  daProcs->countItems = Nlm_CountChoiceItems;
  daProcs->setTitle = Nlm_SetChoiceTitle;
  daProcs->getTitle = Nlm_GetChoiceTitle;
  daProcs->setValue = Nlm_SetChoiceValue;
  daProcs->getValue = Nlm_GetChoiceValue;
}

