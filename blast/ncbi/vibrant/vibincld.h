/*   vibincld.h
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
* File Name:  vibincld.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   7/1/91
*
* $Revision: 6.5 $
*
* File Description: 
*       Vibrant inclusion of underlying windowing system toolbox functions,
*       definition of common internal data types, and miscellaneous functions
*       for internal use only.  Applications should avoid including this file.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: vibincld.h,v $
* Revision 6.5  2002/06/13 16:15:13  kans
* fix includes for OS_UNIX_DARWIN with WIN_MAC (EN) - still bug in vibutils.c file dialog
*
* Revision 6.4  2000/07/08 20:44:14  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 6.3  1999/06/22 15:14:54  lewisg
* fix image library so that works on linux with > 8 bits
*
* Revision 6.2  1999/04/06 14:23:25  lewisg
* add opengl replacement for viewer3d
*
* Revision 6.1  1998/07/14 16:44:27  vakatov
* Added VibrantIsGUI() and <internal> Nlm_VibrantSetGUI()
*
* Revision 6.0  1997/08/25 18:57:08  madden
* Revision changed to 6.0
*
* Revision 5.9  1997/07/23 19:36:12  vakatov
* [WIN_MSWIN] +Nlm_GetPicWinHDC proto
*
* Revision 5.8  1997/07/10 21:49:30  vakatov
* [WIN_X]  Now able to manage windows having different depths(and
* different visuals and GC).
*
* Revision 5.7  1997/06/09 18:07:39  vakatov
* Removed "Nlm_fontList" declaration(it's now static in "ncbidraw.c")
* [WIN_MOTIF]  Added Nlm_XAllocColor() prototype
*
* Revision 5.6  1997/04/25 16:09:33  vakatov
* [WIN_MOTIF,WIN_MSWIN]  Added functions to catch, convert(to Mac keycodes)
* and process navigation and special keys
*
 * Revision 5.5  1997/04/17  16:17:50  kans
 * InitForms and FreeForms added
 *
 * Revision 5.4  1997/02/13  21:34:37  vakatov
 * [WIN_MOTIF]  #define NLM_MOTIF_CASCADEB_BUG -- workaround the Motif bug
 *
 * Revision 5.3  1997/01/28  22:03:51  kans
 * changed <GestaltEqu.h> to <Gestalt.h> for CodeWarrior
 *
 * Revision 5.2  1996/10/28  19:31:19  vakatov
 * [WIN_MOTIF]  Added prototypes for functions Nlm_VibrantDefaultColormap()
 * and Nlm_XrmGetResource()
 *
 * Revision 5.1  1996/08/27  20:52:53  vakatov
 * Added Nlm_HorizScrollBar4() and Nlm_VertScrollBar4() prototypes -- to
 * create Int4-range scrollbars
 *
 * Revision 5.0  1996/05/28  13:45:08  ostell
 * Set to revision 5.0
 *
 * Revision 4.2  1996/03/12  22:34:37  epstein
 * add shellapi for Drag-And-Drop support
 *
 * Revision 4.1  1996/02/13  17:24:07  kans
 * accelerated set position prior to realization (Denis Vakatov)
 *
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 2.21  1995/07/17  22:02:19  kans
 * Motif clipboard cut and paste supported (AS)
 *
 * Revision 2.20  1995/05/31  18:00:58  kans
 * added <Palettes.h>
 *
 * Revision 2.19  1995/05/17  15:15:14  kans
 * added Log line
 *
*
* ==========================================================================
*/

#ifndef _VIBINCLD_
#define _VIBINCLD_

#ifndef _NCBIWIN_
#include <ncbiwin.h>
#endif
#ifndef _NCBIPORT_
#include <ncbiport.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***  UNDERLYING TOOLBOX GRAPHIC PRIMITIVE OBJECT TYPES  ***/

#ifdef WIN_MAC
#define Nlm_HandleTool Handle
#define Nlm_PointTool Point
#define Nlm_RectTool Rect
#define Nlm_RgnTool RgnHandle
#endif

#ifdef WIN_MSWIN
#define Nlm_HandleTool HANDLE
#define Nlm_PointTool POINT
#define Nlm_RectTool RECT
#define Nlm_RgnTool HRGN
#endif

#ifdef WIN_MOTIF
#define Nlm_HandleTool Nlm_VoidPtr
#define Nlm_PointTool XPoint
#define Nlm_RectTool XRectangle
#define Nlm_RgnTool Region
#endif


/*
*  WARNING -- The various GetObjectData procedures use the
*  appropriate recentObject and recentObjectData variables,
*  to speed access by eliminating handle locking, copying,
*  and unlocking upon repeated access of an object.  The
*  remove and unlink procedures NULL out the variables, but
*  it could become a problem if some procedure destroys an
*  object other than by calling Remove.  This risky method
*  of data acquisition is controlled by the "NLM_RISKY" flag.
*  Defining NLM_RISKY as FALSE will prevent this caching and
*  force these procedures to always get a fresh copy of the
*  data from the object.
*/

#define NLM_RISKY TRUE

/*
*  WARNING -- The X windowing system is designed for use over
*  a network.  Sending queries during construction of Vibrant
*  windows can be very time consuming over long distances.
*  Defining the "NLM_QUIET" flag to TRUE will only realize
*  Motif widgets at creation time if their parent window has
*  already been shown.  Otherwise, it will realize all widgets
*  from the shell on down as soon as the window is first shown.
*/

#define NLM_QUIET TRUE



/* Workaround the bad memory management resulting in the double
 * memory deallocation when destroying cascade buttons or shells
 * containing those (some versions of Motif library)
 */
#ifdef WIN_MOTIF
#define NLM_MOTIF_CASCADEB_BUG
#endif


/***  ADDITIONAL INTERNAL INTERFACE OBJECT TYPES  ***/

typedef  struct  Nlm_menubar {
  Nlm_VoidPtr  dummy;
} HNDL Nlm_MenuBaR;

typedef  struct  Nlm_box {
  Nlm_VoidPtr  dummy;
} HNDL Nlm_BoX;

/***  MISCELLANEOUS DRAWING OBJECTS FOR INTERNAL USE  ***/

/*
*  This prompt object is for internal use only.  It does not link
*  in as a child of its parent in the normal way, so on the
*  Macintosh the parent must pass draw and click messages to it.
*  The parent also explicitly positions it, and must show, hide,
*  and remove it as necessary.
*/

Nlm_PrompT  Nlm_DependentPrompt PROTO((Nlm_GraphiC prnt, Nlm_RectPtr r, Nlm_CharPtr title, Nlm_FonT font, Nlm_Char just));

/*
*  These scroll bar objects are for internal use only.  They do not
*  link in as a child of its parent in the normal way, so on the
*  Macintosh the parent must pass draw and click messages to them.
*  The parent also explicitly positions them, and must show, hide,
*  and remove them as necessary.  The action callback procedure is
*  triggered by SetValue if the setting of the scroll bar changes.
*/

Nlm_BaR  Nlm_HorizScrollBar4 PROTO((Nlm_GraphiC slave, Nlm_RectPtr r, Nlm_BarScrlProc4 actn));
Nlm_BaR  Nlm_VertScrollBar4  PROTO((Nlm_GraphiC slave, Nlm_RectPtr r, Nlm_BarScrlProc4 actn));

Nlm_BaR  Nlm_HorizScrollBar PROTO((Nlm_GraphiC slave, Nlm_RectPtr r, Nlm_BarScrlProc actn));
Nlm_BaR  Nlm_VertScrollBar  PROTO((Nlm_GraphiC slave, Nlm_RectPtr r, Nlm_BarScrlProc actn));

/***  STRUCTURE DEFINITIONS FOR INTERFACE OBJECTS  ***/

/*
*  The GraphiC object is the base of all other objects.  It contains
*  linkage information, the class and callback pointers, a position
*  rectangle, and enabled and visible flags.
*/

typedef  struct  Nlm_graphicrec {
  Nlm_GraphiC   next;
  Nlm_GraphiC   parent;
  Nlm_GraphiC   children;
  Nlm_GraphiC   lastChild;
  Nlm_GphPrcs   PNTR classptr;
  Nlm_ActnProc  action;
  Nlm_RecT      rect;
  Nlm_Boolean   enabled;
  Nlm_Boolean   visible;
  Nlm_VoidPtr   data;
  Nlm_FreeProc  cleanup;
  Nlm_Boolean   realized;
  Nlm_VoidPtr   thisobject; /* <<<add to hold DObjectPtr dgg++ */
} Nlm_GraphicRec, Nlm_GraphicData, PNTR Nlm_GphPtr;

/*
*  The BoX object has positioning parameters to nicely organize
*  internal objects.
*/

typedef  struct  Nlm_boxdata {
  Nlm_PoinT  nextPoint;
  Nlm_PoinT  limitPoint;
  Nlm_PoinT  resetPoint;
  Nlm_Int2   topRow;
  Nlm_Int2   nextCol;
  Nlm_Int2   xMargin;
  Nlm_Int2   yMargin;
  Nlm_Int2   xSpacing;
  Nlm_Int2   ySpacing;
  Nlm_Int2   boxWidth;
  Nlm_Int2   boxHeight;
} Nlm_BoxData;

typedef  struct  Nlm_boxrec {
  Nlm_GraphicRec  graphicR;
  Nlm_BoxData     box;
} Nlm_BoxRec, PNTR Nlm_BoxPtr;



#ifdef WIN_MAC
#define Nlm_WindowTool  WindowPtr
#define Nlm_PortTool    GrafPtr
#define Nlm_ShellTool   Nlm_Handle
#define Nlm_MainTool    Nlm_Handle
#define Nlm_ColorMTool  PaletteHandle
#endif

#ifdef WIN_MSWIN
#define Nlm_WindowTool  HWND
#define Nlm_PortTool    HDC
#define Nlm_ShellTool   Nlm_Handle
#define Nlm_MainTool    Nlm_Handle
#define Nlm_ColorMTool  HPALETTE
#endif

#ifdef WIN_MOTIF
#define Nlm_WindowTool  Widget
#define Nlm_PortTool    GC
#define Nlm_ShellTool   Widget
#define Nlm_MainTool    Widget
#define Nlm_ColorMTool  Colormap
#endif

extern  Nlm_WindoW     Nlm_desktopWindow;
extern  Nlm_WindoW     Nlm_systemWindow;

extern  Nlm_WindoW     Nlm_theWindow;
extern  Nlm_WindowTool Nlm_currentWindowTool;

extern  Nlm_Boolean    Nlm_processUpdatesFirstVal;

#ifdef WIN_MAC
extern  EventRecord    Nlm_currentEvent;
#endif

#ifdef WIN_MSWIN
extern  HCURSOR        Nlm_currentCursor;
extern  WPARAM         Nlm_currentWParam;
extern  HINSTANCE      Nlm_currentHInst;
extern  LPARAM         Nlm_currentLParam;
extern  MSG            Nlm_currentMssg;
extern  int            Nlm_currentId;
extern  HWND           Nlm_currentHwndCtl;
extern  UINT           Nlm_currentCode;
extern  int            Nlm_currentPos;
#endif

#ifdef WIN_MOTIF
extern  Cursor         Nlm_currentCursor;
extern  XEvent         Nlm_currentEvent;
extern  XtAppContext   Nlm_appContext;
extern  XmFontList     Nlm_XfontList;
extern  Widget         Nlm_fileDialogShell;
#endif

void            Nlm_ProcessTimerEvent PROTO((void));
Nlm_WindowTool  Nlm_ParentWindowPtr PROTO((Nlm_GraphiC a));
Nlm_PortTool    Nlm_ParentWindowPort PROTO((Nlm_GraphiC a));
Nlm_MainTool    Nlm_ParentWindowMain PROTO((Nlm_GraphiC a));
Nlm_ShellTool   Nlm_ParentWindowShell PROTO((Nlm_GraphiC a));
Nlm_WindoW      Nlm_SavePortIfNeeded PROTO((Nlm_GraphiC a, Nlm_Boolean savePort));

void            Nlm_SetWindowCharDisplay PROTO((Nlm_WindoW w, Nlm_DisplaY d));
Nlm_DisplaY     Nlm_GetWindowCharDisplay PROTO((Nlm_WindoW w));
void            Nlm_SetWindowMenuBar PROTO((Nlm_WindoW w, Nlm_MenuBaR mb));
Nlm_MenuBaR     Nlm_GetWindowMenuBar PROTO((Nlm_WindoW w));
void            Nlm_SetWindowDefaultButton PROTO((Nlm_WindoW w, Nlm_ButtoN b));
Nlm_ButtoN      Nlm_GetWindowDefaultButton PROTO((Nlm_WindoW w));
Nlm_Boolean     Nlm_WindowHasBeenShown PROTO((Nlm_WindoW w));
Nlm_Boolean     Nlm_IsWindowDying PROTO((Nlm_WindoW w));
void            Nlm_PassPanelClickToText PROTO((Nlm_PaneL p, Nlm_TexT t, Nlm_PoinT pt));
void            Nlm_KillSlateTimer PROTO((void));

void            Nlm_GetChoiceTitle PROTO((Nlm_GraphiC c, Nlm_Int2 item, Nlm_CharPtr title, size_t maxsize));
Nlm_ShellTool   Nlm_GetWindowShell PROTO((Nlm_WindoW w));

#ifdef WIN_MAC
void            Nlm_SetUpdateRegion PROTO((WindowPtr wptr));
#endif
#ifdef WIN_MOTIF
Colormap        Nlm_VibrantDefaultColormap PROTO(( void ));
Visual         *Nlm_VibrantDefaultVisual   PROTO((void));
int             Nlm_VibrantDefaultDepth    PROTO(( void ));
Nlm_Boolean     Nlm_GetFixedColormap PROTO((Nlm_GraphiC w,
                                            Colormap *cMap, Visual **visual));
void            Nlm_XAllocColor PROTO((XColor *xcolor, Colormap colormap,
                                       Nlm_Uint1 red,
                                       Nlm_Uint1 green,
                                       Nlm_Uint1 blue));
void            Nlm_MapDefaultButton PROTO((Nlm_WindoW w, Nlm_ButtoN b));

/* Get resource value(in the string representation) defined as X11-resources
   having form:
      <application>.<_resource>:  <resource_value>
      Vibrant.<_resource>:        <resource_value>

   where <application> is the running application name without path and
   extention, lowercased(!).  The "<application>." value overrides the
   "Vibrant." one.
   On error, return NULL;  deallocation of the value returned on success
   is the user's responsibility(use "Nlm_MemFree()").
*/
Nlm_CharPtr     Nlm_XrmGetResource PROTO((const Nlm_Char PNTR _resource));
#endif

extern  Nlm_Int2    Nlm_nextIdNumber;

extern  Nlm_PoinT   Nlm_globalMouse;
extern  Nlm_PoinT   Nlm_localMouse;

extern  Nlm_Boolean Nlm_textScrapFull;

void        Nlm_InitVibrantHooks PROTO((void));

void        Nlm_InitBars PROTO((void));
void        Nlm_InitButtons PROTO((void));
void        Nlm_InitForms PROTO((void));
void        Nlm_InitExtras PROTO((void));
void        Nlm_InitGroup PROTO((void));
void        Nlm_InitLists PROTO((void));
void        Nlm_InitMenus PROTO((void));
void        Nlm_InitPrompt PROTO((void));
void        Nlm_InitSlate PROTO((void));
void        Nlm_InitTexts PROTO((void));
void        Nlm_InitWindows PROTO((void));

void        Nlm_FreeBars PROTO((void));
void        Nlm_FreeButtons PROTO((void));
void        Nlm_FreeForms PROTO((void));
void        Nlm_FreeExtras PROTO((void));
void        Nlm_FreeGroup PROTO((void));
void        Nlm_FreeLists PROTO((void));
void        Nlm_FreeMenus PROTO((void));
void        Nlm_FreePrompt PROTO((void));
void        Nlm_FreeSlate PROTO((void));
void        Nlm_FreeTexts PROTO((void));
void        Nlm_FreeWindows PROTO((void));

Nlm_Boolean Nlm_RegisterWindows PROTO((void));
Nlm_Boolean Nlm_RegisterSlates PROTO((void));
Nlm_Boolean Nlm_RegisterTexts PROTO((void));

void        Nlm_ClearItemsInGroup PROTO((Nlm_GraphiC a, Nlm_GraphiC excpt, Nlm_Boolean savePort));
Nlm_Int2    Nlm_CountGroupItems PROTO((Nlm_GraphiC a));

Nlm_GraphiC Nlm_CreateLink PROTO((Nlm_GraphiC prnt, Nlm_RectPtr r, Nlm_Int2 recordSize, Nlm_GphPrcs PNTR classPtr));
void        Nlm_RemoveLink PROTO((Nlm_GraphiC a));
void        Nlm_RecordRect PROTO((Nlm_GraphiC a, Nlm_RectPtr r));
void        Nlm_NextPosition PROTO((Nlm_GraphiC a, Nlm_RectPtr r));
Nlm_GphPrcsPtr Nlm_GetClassPtr PROTO((Nlm_GraphiC a));
#ifdef WIN_MAC
Nlm_Boolean Nlm_DoClick PROTO((Nlm_GraphiC a, Nlm_PoinT pt));
Nlm_Boolean Nlm_DoKey PROTO((Nlm_GraphiC a, Nlm_Char ch));
void        Nlm_DoDraw PROTO((Nlm_GraphiC a));
Nlm_Boolean Nlm_DoIdle PROTO((Nlm_GraphiC a, Nlm_PoinT pt));
#endif
#ifdef WIN_MSWIN
Nlm_Boolean Nlm_DoCommand PROTO((Nlm_GraphiC a));
#endif
#ifdef WIN_MOTIF
void        Nlm_DoCallback PROTO((Nlm_GraphiC a));
#endif
void        Nlm_DoShow PROTO((Nlm_GraphiC a, Nlm_Boolean setFlag, Nlm_Boolean savePort));
void        Nlm_DoHide PROTO((Nlm_GraphiC a, Nlm_Boolean setFlag, Nlm_Boolean savePort));
void        Nlm_DoEnable PROTO((Nlm_GraphiC a, Nlm_Boolean setFlag, Nlm_Boolean savePort));
void        Nlm_DoDisable PROTO((Nlm_GraphiC a, Nlm_Boolean setFlag, Nlm_Boolean savePort));
void        Nlm_DoActivate PROTO((Nlm_GraphiC a, Nlm_Boolean savePort));
void        Nlm_DoDeactivate PROTO((Nlm_GraphiC a, Nlm_Boolean savePort));
Nlm_Handle  Nlm_DoRemove PROTO((Nlm_GraphiC a, Nlm_Boolean savePort));
void        Nlm_DoReset PROTO((Nlm_GraphiC a, Nlm_Boolean savePort));
void        Nlm_DoSelect PROTO((Nlm_GraphiC a, Nlm_Boolean savePort));
void        Nlm_DoSetTitle PROTO((Nlm_GraphiC a, Nlm_Int2 item, Nlm_CharPtr title, Nlm_Boolean savePort));
void        Nlm_DoGetTitle PROTO((Nlm_GraphiC a, Nlm_Int2 item, Nlm_CharPtr title, size_t maxsize));
void        Nlm_DoSetValue PROTO((Nlm_GraphiC a, Nlm_Int2 value, Nlm_Boolean savePort));
Nlm_Int2    Nlm_DoGetValue PROTO((Nlm_GraphiC a));
void        Nlm_DoSetStatus PROTO((Nlm_GraphiC a, Nlm_Int2 item, Nlm_Boolean status, Nlm_Boolean savePort));
Nlm_Boolean Nlm_DoGetStatus PROTO((Nlm_GraphiC a, Nlm_Int2 item));
void        Nlm_DoSetOffset PROTO((Nlm_GraphiC a, Nlm_Int2 horiz, Nlm_Int2 vert, Nlm_Boolean savePort));
void        Nlm_DoGetOffset PROTO((Nlm_GraphiC a, Nlm_Int2Ptr horiz, Nlm_Int2Ptr vert));
Nlm_GraphiC Nlm_DoLinkIn PROTO((Nlm_GraphiC a, Nlm_GraphiC prnt));
void        Nlm_DoAdjustPrnt PROTO((Nlm_GraphiC a, Nlm_RectPtr r, Nlm_Boolean align, Nlm_Boolean savePort));
void        Nlm_DoSetPosition PROTO((Nlm_GraphiC a, Nlm_RectPtr r, Nlm_Boolean savePort, Nlm_Boolean force));
void        Nlm_DoGetPosition PROTO((Nlm_GraphiC a, Nlm_RectPtr r));
void        Nlm_DoSetRange PROTO((Nlm_GraphiC a, Nlm_Int2 pgUp, Nlm_Int2 pgDn, Nlm_Int2 max, Nlm_Boolean savePort));
Nlm_GraphiC Nlm_DoGainFocus PROTO((Nlm_GraphiC a, Nlm_Char ch, Nlm_Boolean savePort));
void        Nlm_DoLoseFocus PROTO((Nlm_GraphiC a, Nlm_GraphiC excpt, Nlm_Boolean savePort));
void        Nlm_DoSendChar PROTO((Nlm_GraphiC a, Nlm_Char ch, Nlm_Boolean savePort));
void        Nlm_DoSendFocus (Nlm_GraphiC a, Nlm_Char ch);
void        Nlm_DoAction PROTO((Nlm_GraphiC a));

Nlm_GraphiC Nlm_FindItem PROTO((Nlm_GraphiC a, Nlm_Int2 item));
Nlm_Int2    Nlm_GetItemIndex PROTO((Nlm_GraphiC a));

Nlm_WindoW  Nlm_GetParentWindow PROTO((Nlm_GraphiC a));
Nlm_Boolean Nlm_GetAllParentsEnabled PROTO((Nlm_GraphiC a));
Nlm_Boolean Nlm_GetAllParentsVisible PROTO((Nlm_GraphiC a));
Nlm_Boolean Nlm_AllParentsButWindowVisible PROTO((Nlm_GraphiC a));

void        Nlm_SetNext PROTO((Nlm_GraphiC a, Nlm_GraphiC nxt));
Nlm_GraphiC Nlm_GetNext PROTO((Nlm_GraphiC a));
void        Nlm_SetParent PROTO((Nlm_GraphiC a, Nlm_GraphiC prnt));
Nlm_GraphiC Nlm_GetParent PROTO((Nlm_GraphiC a));
void        Nlm_SetChild PROTO((Nlm_GraphiC a, Nlm_GraphiC chld));
Nlm_GraphiC Nlm_GetChild PROTO((Nlm_GraphiC a));
void        Nlm_SetRect PROTO((Nlm_GraphiC a, Nlm_RectPtr r));
void        Nlm_GetRect PROTO((Nlm_GraphiC a, Nlm_RectPtr r));
void        Nlm_SetEnabled PROTO((Nlm_GraphiC a, Nlm_Boolean enabld));
Nlm_Boolean Nlm_GetEnabled PROTO((Nlm_GraphiC a));
void        Nlm_SetVisible PROTO((Nlm_GraphiC a, Nlm_Boolean visibl));
Nlm_Boolean Nlm_GetVisible PROTO((Nlm_GraphiC a));
void        Nlm_SetRealized PROTO((Nlm_GraphiC a, Nlm_Boolean realizd));
Nlm_Boolean Nlm_GetRealized PROTO((Nlm_GraphiC a));

Nlm_GraphiC Nlm_LinkIn PROTO((Nlm_GraphiC a, Nlm_GraphiC prnt));

void        Nlm_LoadGraphicData PROTO((Nlm_GraphiC a, Nlm_GraphiC nxt, Nlm_GraphiC prnt, Nlm_GraphiC chld, Nlm_GraphiC lstchd, Nlm_GphPrcs PNTR classPtr, Nlm_ActnProc actnProc, Nlm_RectPtr r, Nlm_Boolean enabl, Nlm_Boolean vis, Nlm_VoidPtr dat, Nlm_FreeProc cln));
void        Nlm_SetGraphicData PROTO((Nlm_GraphiC a, Nlm_GraphicData PNTR gdata));
void        Nlm_GetGraphicData PROTO((Nlm_GraphiC a, Nlm_GraphicData PNTR gdata));
void        Nlm_LoadAction PROTO((Nlm_GraphiC a, Nlm_ActnProc actnProc));

void        Nlm_LoadBoxData PROTO((Nlm_BoX a, Nlm_PoinT nxt, Nlm_PoinT lmt, Nlm_PoinT rst, Nlm_Int2 top, Nlm_Int2 ncol, Nlm_Int2 xMrg, Nlm_Int2 yMrg, Nlm_Int2 xSpc, Nlm_Int2 ySpc, Nlm_Int2 wid, Nlm_Int2 hgt));
void        Nlm_SetBoxData PROTO((Nlm_BoX a, Nlm_BoxData PNTR bdata));
void        Nlm_GetBoxData PROTO((Nlm_BoX a, Nlm_BoxData PNTR bdata));

void        Nlm_SetExtraData PROTO((Nlm_GraphiC a, Nlm_VoidPtr dptr, Nlm_Int2 start, Nlm_Int2 extra));
void        Nlm_GetExtraData PROTO((Nlm_GraphiC a, Nlm_VoidPtr dptr, Nlm_Int2 start, Nlm_Int2 extra));

void        Nlm_PointToolToPoinT PROTO((Nlm_PointTool src, Nlm_PointPtr dst));
void        Nlm_PoinTToPointTool PROTO((Nlm_PoinT src, Nlm_PointTool PNTR dst));
void        Nlm_RectToolToRecT PROTO((Nlm_RectTool PNTR src, Nlm_RectPtr dst));
void        Nlm_RecTToRectTool PROTO((Nlm_RectPtr src, Nlm_RectTool PNTR dst));

void        Nlm_LocalToGlobal PROTO((Nlm_PointPtr pt));
void        Nlm_GlobalToLocal PROTO((Nlm_PointPtr pt));

void        Nlm_VibrantSetGUI PROTO((void));

#ifdef WIN_MSWIN
extern  Nlm_Boolean Nlm_VibrantDisabled PROTO((void));
extern  HDC         Nlm_GetPicWinHDC    PROTO((void));
#endif


#ifdef WIN_MOTIF
extern Nlm_Char Nlm_GetInputChar PROTO((XKeyEvent *event));
extern void Nlm_RegisterStdTranslations PROTO(( void ));
extern void Nlm_OverrideStdTranslations  PROTO((Nlm_GraphiC g, Widget w,
                                                Nlm_Int4 filter));
#endif
#ifdef WIN_MSWIN
extern Nlm_Char Nlm_KeydownToChar PROTO((WPARAM wParam));
extern Nlm_Boolean Nlm_ProcessKeydown PROTO((Nlm_GraphiC g, WPARAM wParam,
                                             Nlm_Int4 filter));
#endif

    /** includes needed by vibslate.c **/

#ifdef WIN_MOTIF
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#endif

   /** includes needed by vibwndws.c ***/

#ifdef WIN_MAC
#include <Resources.h>
#include <Scrap.h>
#include <AppleEvents.h>
#include <Gestalt.h>
#endif

#ifdef WIN_MSWIN
#include <shellapi.h>
#endif

#ifdef WIN_MOTIF
#include <Xm/DialogS.h>
#endif

  /** includes needed by vibutils.c ***/

#ifdef WIN_MAC
#ifndef OS_UNIX_DARWIN
#include <Printing.h>
#endif
#include <Processes.h>
#include <Gestalt.h>
#include <Scrap.h>
#include <AppleEvents.h>
#include <Palettes.h>
#endif

#ifdef WIN_MSWIN
#include <commdlg.h>
#endif

#ifdef WIN_MOTIF
#include <Xm/FileSB.h>
#include <Xm/CutPaste.h>

/*
 * Test X windows to see what kind of color buffer is available.
 * Necessary because Linux supports only the type available
 * from the hardware. Code in vibwndws.c. Returns TRUE if match and visinfo.
 */

extern Nlm_Boolean Nlm_CheckX(XVisualInfo *visinfo);


#ifdef __cplusplus
}
#endif


#ifdef OS_UNIX
#include <unistd.h>
#endif
#ifdef OS_VMS
#endif
#endif

#endif

