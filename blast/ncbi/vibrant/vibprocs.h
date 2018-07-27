/*   vibprocs.h
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
* File Name:  vibprocs.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   7/1/91
*
* $Revision: 6.28 $
*
* File Description: 
*       Vibrant procedure definitions
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: vibprocs.h,v $
* Revision 6.28  2004/04/01 13:43:09  lavr
* Spell "occurred", "occurrence", and "occurring"
*
* Revision 6.27  2004/02/23 16:36:51  sinyakov
* Use Int4 instead of Int2 for cursor position and text selection in text boxes
*
* Revision 6.26  2004/02/12 20:05:26  bazhin
* Added callback function "Nlm_SetKeepCrNlTextFieldCallback(Nlm_TexT t)",
* which allows to copy-paste texts from multiple columns to single string.
*
* Revision 6.25  2004/02/05 16:25:06  kans
* revert to Nlm_GetTextVScrollBar and Nlm_GetTextHScrollBar being static, implement setOffset and getOffset for scroll text, since Windows scroll text has its own scroll bar
*
* Revision 6.24  2004/02/04 15:21:02  kans
* make GetTextVScrollBar and GetTextHScrollBar extern, to add search function to general text viewer
*
* Revision 6.23  2004/01/05 17:08:09  kans
* added functions to control use of dual screens
*
* Revision 6.22  2003/10/29 19:19:34  kans
* added prototype for Nlm_SetTextColor
*
* Revision 6.21  2002/04/30 18:25:12  bazhin
* Added function "Nlm_SetWindowConfigureCallback(WindoW w)", which
* allows to catch events, when window just moved without resizing.
* Fixed some "gcc -Wall -ansi" warnings.
*
* Revision 6.20  2002/01/09 15:23:54  kans
* added HasAquaMenuLayout
*
* Revision 6.19  2001/09/10 17:34:33  bazhin
* Added function Nlm_SetTextCursorBlinkRate(Nlm_TexT t, Nlm_Int2 msec).
*
* Revision 6.18  2000/08/01 15:57:32  kans
* separate lines for Nlm_HANDLE on all platforms, even if two are currently the same, and no space between # and define in case some compilers complain
*
* Revision 6.17  2000/08/01 15:23:29  vakatov
* Addition to R6.13:   error if neither of "WIN_***" is #defined
*
* Revision 6.16  2000/07/31 13:28:46  lewisg
* Nlm_GetExecPath
*
* Revision 6.15  2000/07/28 21:25:11  lewisg
* *** empty log message ***
*
* Revision 6.14  2000/07/28 21:05:54  lewisg
* more c++ fixes
*
* Revision 6.13  2000/07/28 15:36:34  kans
* for OS_UNIX, give error message if WIN_MOTIF not defined
*
* Revision 6.12  2000/07/25 21:19:00  lewisg
* eliminate redundant windows pathname code
*
* Revision 6.11  2000/07/25 19:18:36  lewisg
* get pathname for executable on windows
*
* Revision 6.10  2000/07/14 20:30:40  kans
* added Nlm_MSWin_OpenApplication (for launching Cn3D with a data file)
*
* Revision 6.9  1999/04/06 14:23:26  lewisg
* add opengl replacement for viewer3d
*
* Revision 6.8  1999/02/19 20:48:36  vakatov
* [WIN_MSWIN]  Added Nlm_MSWin_OpenDocument(also can be used to
* auto-launch IE or Netscape if passed an URL)
*
* Revision 6.7  1999/01/07 22:32:36  kans
* added Mac-specific Nlm_SendURLAppleEvent
*
* Revision 6.6  1998/07/07 23:03:34  vakatov
* Added Nlm_Execv() to spawn applications with cmd-line parameters;
* now implemented for [OS_MSWIN, OS_UNIX].
*
* Revision 6.5  1998/03/22 03:00:59  kans
* changed names to RegisterServiceProc and RegisterResultProc
*
* Revision 6.4  1998/03/22 02:33:17  kans
* added request proc, result proc, message handlers to support, and send open doc event, launch app now work with file names or signatures
*
* Revision 6.3  1998/03/17 21:07:59  kans
* added Nlm_SendOpenDocAppleEventEx, and private Nlm_LaunchAppEx
*
* Revision 6.2  1997/11/12 20:56:09  kans
* added SetMouseMoveCallback and SetMouseMoveRegion, implemented first on Mac
*
* Revision 6.1  1997/10/18 23:30:21  kans
* implemented Nlm_GetTextCursorPos (DV)
*
* Revision 6.0  1997/08/25 18:57:29  madden
* Revision changed to 6.0
*
* Revision 5.18  1997/08/04 14:15:11  vakatov
* Added Nlm_SetTextCursorPos() function
*
* Revision 5.17  1997/08/01 16:47:28  vakatov
* [WIN_MOTIF,WIN_MSWIN]  Added IconifyWindow() and IconicWindow()
*
* Revision 5.16  1997/07/21 21:52:37  vakatov
* [WIN_MOTIF] Added Nlm_[Un]RegisterIO() functions to wait for I/O
* events and call(via X) user-defined callback(s)
*
* Revision 5.15  1997/07/18 17:01:13  vakatov
* [WIN_MOTIF]  Removed "arg[cv]";  use "Nlm_GetArg[cv]()" instead
*
* Revision 5.14  1997/07/16 19:47:00  vakatov
* +Nlm_StrngPrintable() proto
*
* Revision 5.13  1997/06/23 21:18:46  vakatov
* Added Nlm_SetTextEditable() function to allow/prohibit text editing
*
* Revision 5.12  1997/05/27 21:50:55  vakatov
* Added Nlm_PopupParentWindow() to popup window w/o switching input focus
*
* Revision 5.11  1997/04/28 21:42:46  vakatov
* Added AUTO_FOCUS, FOCUS_NAVIG and DEFAULT_SLATE_POLICY defs & comments
*
 * Revision 5.10  1997/04/25  16:08:45  vakatov
 * Changed old and added new navigation and special key names.
 * Added Nlm_SetSlatePolicy() and definitions for now available slate
 * navigation key handling policies.
 * Added Nlm_Scroll() -- for one-line/one-page scrolling of scrollbars.
 *
 * Revision 5.9  1997/03/19  15:57:07  vakatov
 * Added:  Nlm_Boolean Nlm_QuittingProgram PROTO((void));
 *
 * Revision 5.8  1996/12/12  23:11:10  kans
 * added Nlm_ClipPrintingRect (DV)
 *
 * Revision 5.7  1996/09/26  00:29:23  kans
 * added SetWindowTimer
 *
 * Revision 5.6  1996/09/09  00:15:01  kans
 * AutonomousPanel4 uses ScrollBar4 to have > 32K positions
 *
 * Revision 5.5  1996/08/27  20:56:26  vakatov
 * Added prototypes of function for creating and handling Int2- and
 * Int4-range scrollbars homogeneously
 *
 * Revision 5.4  1996/07/25  18:49:02  vakatov
 * Added prototype of Nlm_GetBarMax()
 *
 * Revision 5.3  1996/06/19  20:30:42  vakatov
 * Added prototype for function Nlm_GetListItem()
 *
 * Revision 5.2  1996/06/14  14:26:46  vakatov
 * Nlm_SetColorCell() -- to immediately change the color appearence
 * [WIN_MOTIF]  Nlm_RestrictMotifColorsTo() -- to avoid Motif/3D-Viewer
 * color overlapping
 *
 * Revision 5.1  1996/05/31  20:11:14  vakatov
 * Added Nlm_ExtendedList() function to set the list selection policy
 * to EXTENDED_SELECTION -- as it was for Nlm_MultiList() since Revision 4.4.
 *
 * Revision 4.5  1996/04/24  21:25:17  vakatov
 *  Nlm_WaitForCondition(cond) modified; now it waits until the next X-event
 * rather than sleeping 100, 200, ..., 900 msec.
 *  Nlm_WaitForXEvent() function added for this reason.
 *
 * Revision 4.4  1996/03/27  23:09:15  vakatov
 * Function Nlm_SetCursorShape() added to let programmer
 * easily store/restore current cursor shape
 *
 * Revision 4.3  1996/03/11  20:39:19  epstein
 * add support for Drag & Drop
 *
 * Revision 4.2  1995/11/08  23:30:31  kans
 * removed edit block fields, which belong in the application
 *
 * Revision 4.1  1995/10/06  16:44:39  epstein
 * add Nlm_WaitForCondition macro
 *
 * Revision 2.56  1995/05/31  18:00:58  kans
 * added SetColorMap
*
* ==========================================================================
*/

#ifndef _VIBPROCS_
#define _VIBPROCS_

#ifdef __cplusplus
extern "C" {
#endif

/***  GLOBAL VARIABLES  ***/

/*
*  The fileDone flag indicates success or failure of the last
*  file operation, and can be used to test for end-of-file on
*  input.  The termCH character contains the character that
*  terminated the most recently read string, field, or line.
*  That character is not included in the string.
*/

extern  Nlm_Boolean  Nlm_fileDone;
extern  Nlm_Int2     Nlm_fileError;
extern  Nlm_Char     Nlm_termCH;

/*
*  The currentKey variable can be used by callback routines to
*  determine what key was most recently pressed by the user.
*/

extern  Nlm_Char  Nlm_currentKey;

/*
*  The cmmdKey, ctrlKey, optKey and shftKey flags reflect the
*  status of the command key, the control key, the option key,
*  and the shift key of the keyboard, respectively.  Macintosh,
*  IBM PC, and Unix machines may only have a subset of these
*  modifier keys on their keyboards.  The dblClick flag is set
*  if the user double clicked in a list selection box.
*/

extern  Nlm_Boolean  Nlm_cmmdKey;
extern  Nlm_Boolean  Nlm_ctrlKey;
extern  Nlm_Boolean  Nlm_optKey;
extern  Nlm_Boolean  Nlm_shftKey;
extern  Nlm_Boolean  Nlm_dblClick;

/*
*  The screenRect contains the size in pixels of the computer screen.
*/

extern  Nlm_RecT  Nlm_screenRect;

/*
*  Miscellaneous constants define the width and height of scroll bars.
*/

extern  Nlm_Int2  Nlm_hScrollBarHeight;
extern  Nlm_Int2  Nlm_vScrollBarWidth;

/*
*  Miscellaneous constants define the height of certain control objects.
*/

extern  Nlm_Int2  Nlm_popupMenuHeight;
extern  Nlm_Int2  Nlm_dialogTextHeight;


/***  PROCEDURES THAT CREATE INTERFACE OBJECTS  ***/

/***  WINDOW OBJECTS  ***/

/*
*  When positive, the left and top window parameters specify the pixel
*  location of the upper left hand corner of the window on the screen.
*  When negative, they specify the a relative percentage position, with
*  -50, -33 meaning centered horizontally and placed vertically one third
*  of the way down the screen.  When positive, the width and height
*  parameters indicate the pixel size of the window.  When negative, they
*  indicate that the window, when first made visible, will size itself to
*  fit around the objects it contains, with the absolute value of the
*  width being the number of pixels to use as a border around the internal
*  objects and the absolute value of the height being the spacing between
*  objects.
*/

/*
*  Document, Fixed, and Round windows may be dragged around the screen by
*  the user.  Document windows may also be resized by the user.  When a
*  Modal window is the front window, the user is prevented from bringing
*  other windows to the front or from making menu choices.  This should
*  be used only when extra information is essential in order to proceed,
*  or to alert the user to critical error conditions.  Modal windows should
*  be created when needed and destroyed immediately upon release.  If a
*  modal window is hidden, it may inhibit any user input.  Floating windows
*  float above other windows, and can be useful for selecting tools (such
*  as in a MacPaint drawing palette) without needing to change the active
*  window each time.  The close procedure is called when the user clicks in
*  the close box of a window.  The callback procedure will usually hide or
*  remove the window, or may display a modal window asking the user to
*  confirm the close.  The activate and deactivate procedures are executed
*  when a window is activated or deactivated, and the resize procedure is
*  called when the user changes the size of a window.
*/

#ifdef WIN_MOTIF
/* Useful when creating a top-level (shell) window containing
 * 3D-viewer -- as  the latter overwrites all colorcells from 32 to 255
 * This restriction is valid while creating the next(the only!)
 * top-level window, then expires */
void Nlm_RestrictMotifColorsTo  PROTO ( (Nlm_Uchar n_colorcells) );
#else
#define Nlm_RestrictMotifColorsTo(dummy) {;}
#endif

Nlm_WindoW  Nlm_DocumentWindow PROTO((Nlm_Int2 left, Nlm_Int2 top, Nlm_Int2 width, Nlm_Int2 height, Nlm_CharPtr title, Nlm_WndActnProc close, Nlm_WndActnProc resize));
Nlm_WindoW  Nlm_FixedWindow PROTO((Nlm_Int2 left, Nlm_Int2 top, Nlm_Int2 width, Nlm_Int2 height, Nlm_CharPtr title, Nlm_WndActnProc close));
Nlm_WindoW  Nlm_FrozenWindow PROTO((Nlm_Int2 left, Nlm_Int2 top, Nlm_Int2 width, Nlm_Int2 height, Nlm_CharPtr title, Nlm_WndActnProc close));
Nlm_WindoW  Nlm_RoundWindow PROTO((Nlm_Int2 left, Nlm_Int2 top, Nlm_Int2 width, Nlm_Int2 height, Nlm_CharPtr title, Nlm_WndActnProc close));
Nlm_WindoW  Nlm_AlertWindow PROTO((Nlm_Int2 left, Nlm_Int2 top, Nlm_Int2 width, Nlm_Int2 height, Nlm_WndActnProc close));
Nlm_WindoW  Nlm_ModalWindow PROTO((Nlm_Int2 left, Nlm_Int2 top, Nlm_Int2 width, Nlm_Int2 height, Nlm_WndActnProc close));
Nlm_WindoW  Nlm_FloatingWindow PROTO((Nlm_Int2 left, Nlm_Int2 top, Nlm_Int2 width, Nlm_Int2 height, Nlm_WndActnProc close));
Nlm_WindoW  Nlm_ShadowWindow PROTO((Nlm_Int2 left, Nlm_Int2 top, Nlm_Int2 width, Nlm_Int2 height, Nlm_WndActnProc close));
Nlm_WindoW  Nlm_PlainWindow PROTO((Nlm_Int2 left, Nlm_Int2 top, Nlm_Int2 width, Nlm_Int2 height, Nlm_WndActnProc close));

/* esl++ */
Nlm_WindoW  Nlm_MovableModalWindow PROTO((Nlm_Int2 left, Nlm_Int2 top, Nlm_Int2 width, Nlm_Int2 height, Nlm_CharPtr title, Nlm_WndActnProc close));
void        Nlm_SetModalWindowOwner PROTO((Nlm_WindoW w, Nlm_WindoW owner));
Nlm_Boolean Nlm_IsWindowModal PROTO((Nlm_WindoW w));

void        Nlm_SetClose PROTO((Nlm_WindoW w, Nlm_WndActnProc cls));
void        Nlm_SetActivate PROTO((Nlm_WindoW w, Nlm_WndActnProc act));
void        Nlm_SetDeactivate PROTO((Nlm_WindoW w, Nlm_WndActnProc deact));
void        Nlm_SetResize  PROTO((Nlm_WindoW w, Nlm_WndActnProc resiz));
void        Nlm_SetColorMap  PROTO((Nlm_WindoW w, Nlm_Uint2 totalColors,
                                    Nlm_Uint1Ptr red, Nlm_Uint1Ptr green,
                                    Nlm_Uint1Ptr blue));
void        Nlm_SetColorCell PROTO((Nlm_GraphiC w, Nlm_Uint1 pixel,
                                    Nlm_Uint1 red, Nlm_Uint1 green,
                                    Nlm_Uint1 blue));

void        Nlm_SetWindowExtra PROTO((Nlm_WindoW w, Nlm_VoidPtr data, Nlm_WndFreeProc cleanup));
Nlm_VoidPtr Nlm_GetWindowExtra PROTO((Nlm_WindoW w));

void        Nlm_SetWindowConfigureCallback PROTO((Nlm_WindoW w));

/* Dual screen functions control centering where the window left parameter is negative */

Nlm_Boolean Nlm_HasDualScreen (void);
void Nlm_UseFullScreen (void);
void Nlm_UseLeftScreen (void);
void Nlm_UseRightScreen (void);


/***  GROUPING OBJECT  ***/

/*
*  Groups are used for fine control of automatic positioning of objects
*  within a window.  When containing radio buttons, the integer value of
*  the group corresponds to the current button choice.  See SetValue and
*  GetValue under Object Title and State Manipulations Procedures.  When
*  both the width and height of a group are 0, objects within the group
*  are overlapped at the same location.  The default group margins are
*  3 horizontal and 2 vertical pixels.  This can be changed with the
*  SetGroupMargins procedure.
*/

Nlm_GrouP  Nlm_NormalGroup PROTO((Nlm_GrouP prnt, Nlm_Int2 width, Nlm_Int2 height, Nlm_CharPtr title, Nlm_FonT font, Nlm_GrpActnProc actn));
Nlm_GrouP  Nlm_HiddenGroup PROTO((Nlm_GrouP prnt,  Nlm_Int2 width, Nlm_Int2 height, Nlm_GrpActnProc actn));
void       Nlm_SetGroupMargins PROTO((Nlm_GrouP g, Nlm_Int2 xMargin, Nlm_Int2 yMargin));
void       Nlm_SetGroupSpacing PROTO((Nlm_GrouP g, Nlm_Int2 xSpacing, Nlm_Int2 ySpacing));

/*
*  In the following object creation procedures, whenever the parent
*  parameter specifies GrouP, a WindoW may also be used, because
*  both objects are handles to the same dummy structure.
*/

/***  CONTROL OBJECTS  ***/

/***  Button Objects  ***/

/*
*  Push buttons and Default buttons allow the user to trigger specific
*  callback functions.  Check boxes allow the user to specify boolean
*  yes/no flags.  The parent group containing radio buttons has an
*  integer value that corresponds to one of several mutually exclusive
*  choices that the user can make.  Radio buttons return handles only to
*  allow them to be individually disabled or hidden.  They should be used
*  in the same manner as list items and choice items.
*/

Nlm_ButtoN  Nlm_PushButton PROTO((Nlm_GrouP prnt, Nlm_CharPtr title, Nlm_BtnActnProc actn));
Nlm_ButtoN  Nlm_DefaultButton PROTO((Nlm_GrouP prnt, Nlm_CharPtr title, Nlm_BtnActnProc actn));
Nlm_ButtoN  Nlm_CheckBox PROTO((Nlm_GrouP prnt, Nlm_CharPtr title, Nlm_BtnActnProc actn));
Nlm_ButtoN  Nlm_RadioButton PROTO((Nlm_GrouP prnt, Nlm_CharPtr title));

/***  Scrolling Choice List Objects  ***/

/*
*  Single choice and Multiple choice lists functionally correspond to groups of
*  radio buttons or check boxes.  Because they have the appearance of scrolling
*  choice lists instead of buttons, additional items do not change the size of
*  the list object.  To prevent automatic refreshing of the screen after every
*  append use the Disable function.  Calling Enable will update the list.
*/

Nlm_LisT  Nlm_SingleList    PROTO((Nlm_GrouP prnt, Nlm_Int2 width, Nlm_Int2 height, Nlm_LstActnProc actn));
Nlm_LisT  Nlm_MultiList     PROTO((Nlm_GrouP prnt, Nlm_Int2 width, Nlm_Int2 height, Nlm_LstActnProc actn));
Nlm_LisT  Nlm_ExtendedList  PROTO((Nlm_GrouP prnt, Nlm_Int2 width, Nlm_Int2 height, Nlm_LstActnProc actn));
void      Nlm_ListItem      PROTO((Nlm_LisT l, Nlm_CharPtr title));
void      Nlm_GetListItem   PROTO((Nlm_LisT l, Nlm_Int2 item,
                                   Nlm_CharPtr title, size_t maxsize));

/***  Menu Objects  ***/

/*
*  A Pulldown or Apple menu can appear on the desktop (NULL window) or
*  in a menu bar within a given window.  Popup menus can appear anywhere
*  in a window.  Submenus can appear in any of the above menus, or in
*  other submenus.  Command items act exactly like push buttons, in that
*  they are used to trigger specific callback routines.  Status items
*  correspond to boolean check boxes.  Choice groups are mutually
*  exclusive groups of choices that function exactly like radio groups or
*  single choice lists.  The active choice has a check mark in front of
*  it.  A Popup list is a popup menu with a choice group that displays the
*  current value of the choice group on the window.  The DeskAccGroup
*  allows the user to display a Macintosh desk accessory menu (usually in
*  the Apple menu).  A Font group creates a menu choice group composed of
*  the available fonts in the System file.  The Separator item places a
*  dotted line in between groups of menu items.
*/

Nlm_MenU    Nlm_PulldownMenu PROTO((Nlm_WindoW w, Nlm_CharPtr title));
Nlm_MenU    Nlm_AppleMenu PROTO((Nlm_WindoW w));
Nlm_MenU    Nlm_PopupMenu PROTO((Nlm_GrouP prnt, Nlm_CharPtr title));
Nlm_MenU    Nlm_SubMenu PROTO((Nlm_MenU m, Nlm_CharPtr title));
Nlm_IteM    Nlm_CommandItem PROTO((Nlm_MenU m, Nlm_CharPtr title, Nlm_ItmActnProc actn));
Nlm_IteM    Nlm_StatusItem PROTO((Nlm_MenU m, Nlm_CharPtr title, Nlm_ItmActnProc actn));
Nlm_ChoicE  Nlm_ChoiceGroup PROTO((Nlm_MenU m, Nlm_ChsActnProc actn));
Nlm_IteM    Nlm_ChoiceItem PROTO((Nlm_ChoicE c, Nlm_CharPtr title));
Nlm_PopuP   Nlm_PopupList PROTO((Nlm_GrouP prnt, Nlm_Boolean macLike, Nlm_PupActnProc actn));
void        Nlm_PopupItem PROTO((Nlm_PopuP p, Nlm_CharPtr title));
void        Nlm_PopupItems PROTO((Nlm_PopuP p, Nlm_CharPtr PNTR titles));
void        Nlm_DeskAccGroup PROTO((Nlm_MenU m));
Nlm_ChoicE  Nlm_FontGroup PROTO((Nlm_MenU m));
void        Nlm_SeparatorItem PROTO((Nlm_MenU m));
Nlm_Boolean Nlm_HasAquaMenuLayout (void);

/***  Bar Object  ***/

/*
*  This scroll bar is a normal object that can be used for input of integers.
*  Width or height specify the number of standard character widths or standard
*  character line heights to make the scroll bar.
*  
*  CorrectBarValue, CorrectBarMax and CorrectBarPage will change
*  the scroll bar value/max/page without triggering the scroll bar's callback.
*
*  SetBarValue and GetBarValue functions allows one to set Int4 value to
*  the scrollbar -- unlike the generic SetValue and GetValue functions
*  which operate with Int2 values only.
*/

Nlm_BaR  Nlm_ScrollBar4 PROTO((Nlm_GrouP prnt, Nlm_Int2 width, Nlm_Int2 height, Nlm_BarScrlProc4 actn));
Nlm_BaR  Nlm_ScrollBar  PROTO((Nlm_GrouP prnt, Nlm_Int2 width, Nlm_Int2 height, Nlm_BarScrlProc  actn));

void     Nlm_SetBarValue     PROTO((Nlm_BaR b, Nlm_Int4 val));
void     Nlm_CorrectBarValue PROTO((Nlm_BaR b, Nlm_Int4 val));
Nlm_Int4 Nlm_GetBarValue     PROTO((Nlm_BaR b));

void     Nlm_SetBarMax       PROTO((Nlm_BaR b, Nlm_Int4 max));
void     Nlm_CorrectBarMax   PROTO((Nlm_BaR b, Nlm_Int4 max));
Nlm_Int4 Nlm_GetBarMax       PROTO((Nlm_BaR b));

void     Nlm_CorrectBarPage  PROTO((Nlm_BaR b, Nlm_Int4 pgUp, Nlm_Int4 pgDn));

typedef enum
{
  SCROLL_LINEUP = 0,
  SCROLL_LINEDN,
  SCROLL_PAGEUP,
  SCROLL_PAGEDN
}  Nlm_enumScrollEvent;
void     Nlm_Scroll          PROTO((Nlm_BaR b, Nlm_enumScrollEvent event));

/***  Repeat Object  ***/

/*
*  Repeat buttons are implemented as autonomous PaneLs.  They will repeatedly
*  trigger the callback in response to click and hold messages.
*/

Nlm_RepeaT  Nlm_RepeatButton PROTO((Nlm_GrouP prnt, Nlm_CharPtr title, Nlm_RptClckProc actn));

/***  Icon Object  ***/

/*
*  Icon buttons are implemented as autonomous PaneLs.  They are used for to display
*  arbitrary bitmaps.  The draw callback should get the value of the icon button to
*  determine what to display.  The click, drag, hold and release mouse callbacks (see
*  SetPanelClick) may set the value of the icon button to change its appearance.
*/

Nlm_IcoN    Nlm_IconButton PROTO((Nlm_GrouP prnt, Nlm_Int2 pixwidth, Nlm_Int2 pixheight, Nlm_IcnActnProc draw, Nlm_IcnChngProc inval,
				Nlm_IcnClckProc click, Nlm_IcnClckProc drag, Nlm_IcnClckProc hold, Nlm_IcnClckProc release));

/***  Switch Object  ***/

/*
*  The switch object is used as a miniature scroll bar, with up and down arrows but no
*  thumb or page areas.  It can optionally display a (non-editable) text representation
*  of the value (e.g., "5/15").  The value is manipulated with SetValue and GetValue.
*  The arrows will allow the user to set values from 1 through max.  SetSwitchParams will
*  set both the max and the value at once, to avoid flickering by multiple redraws.
*/

Nlm_SwitcH  Nlm_UpDownSwitch PROTO((Nlm_GrouP prnt, Nlm_Boolean text, Nlm_SwtChngProc actn));
Nlm_SwitcH  Nlm_LeftRightSwitch PROTO((Nlm_GrouP prnt, Nlm_Boolean text, Nlm_SwtChngProc actn));
void        Nlm_SetSwitchMax PROTO((Nlm_SwitcH s, Nlm_Int2 max));
Nlm_Int2    Nlm_GetSwitchMax PROTO((Nlm_SwitcH s));
void        Nlm_SetSwitchParams PROTO((Nlm_SwitcH s, Nlm_Int2 value, Nlm_Int2 max));

/***  TEXT OBJECTS  ***/

/***  Editable Text Objects  ***/

/*
*  A Dialog text item contains a single line of text, and pressing the
*  tab key will step between individual dialog text items in the front
*  window.  A Scroll text box can hold multiple lines of text.
*/

Nlm_TexT  Nlm_DialogText PROTO((Nlm_GrouP prnt, Nlm_CharPtr dfault, Nlm_Int2 charWidth, Nlm_TxtActnProc actn));
Nlm_TexT  Nlm_HiddenText PROTO((Nlm_GrouP prnt, Nlm_CharPtr dfault, Nlm_Int2 charWidth, Nlm_TxtActnProc actn, Nlm_TxtActnProc tabProc, Nlm_TxtActnProc rtnProc));
Nlm_TexT  Nlm_SpecialText PROTO((Nlm_GrouP prnt, Nlm_CharPtr dfault, Nlm_Int2 charWidth, Nlm_TxtActnProc actn, Nlm_TxtActnProc tabProc, Nlm_TxtActnProc rtnProc));
Nlm_TexT  Nlm_PasswordText PROTO((Nlm_GrouP prnt, Nlm_CharPtr dfault, Nlm_Int2 charWidth, Nlm_TxtActnProc actn));
Nlm_TexT  Nlm_ScrollText PROTO((Nlm_GrouP prnt, Nlm_Int2 width, Nlm_Int2 height, Nlm_FonT font, Nlm_Boolean wrap, Nlm_TxtActnProc actn));

void      Nlm_SetTextSelect PROTO((Nlm_TexT t, Nlm_TxtActnProc slct, Nlm_TxtActnProc dslct));

Nlm_TexT  Nlm_CurrentText PROTO((void));

/* return TRUE and selection range [begin,end] -- if there is a selection
 */
Nlm_Boolean Nlm_TextSelectionRange PROTO((Nlm_TexT t, Nlm_Int4Ptr begin, Nlm_Int4Ptr end));

void      Nlm_CutText PROTO((Nlm_TexT t));
void      Nlm_CopyText PROTO((Nlm_TexT t));
void      Nlm_PasteText PROTO((Nlm_TexT t));
void      Nlm_ClearText PROTO((Nlm_TexT t));

size_t    Nlm_TextLength PROTO((Nlm_TexT t));
void      Nlm_SelectText PROTO((Nlm_TexT t, Nlm_Int4 begin, Nlm_Int4 end));
void      Nlm_SetTextEditable PROTO((Nlm_TexT t, Nlm_Boolean editable));
void      Nlm_SetTextCursorBlinkRate PROTO((Nlm_TexT t, Nlm_Int2 msec));
void      Nlm_SetTextColor PROTO((Nlm_TexT t, Nlm_Uint4 r, Nlm_Uint4 g, Nlm_Uint4 b));
void      Nlm_SetKeepCrNlTextFieldCallback PROTO((Nlm_TexT t));

/* return the new insertion position in the text(it may differ from "pos"
 * if an error occurred or "pos" is out of limits)
 */
Nlm_Int4  Nlm_SetTextCursorPos PROTO((Nlm_TexT t, Nlm_Int4 pos));

Nlm_Int4  Nlm_GetTextCursorPos PROTO((Nlm_TexT t));



/***  Static Prompt Objects  ***/

/*
*  The static prompt is not editable, but is used to display status
*  information or instructions to the user.
*/

Nlm_PrompT  Nlm_StaticPrompt  PROTO((Nlm_GrouP prnt, Nlm_CharPtr title, Nlm_Int2 pixwidth, Nlm_Int2 pixheight, Nlm_FonT font, Nlm_Char just));

/***  Display Object  ***/

/*
*  The currently selected display object in the front window receives
*  the output that is sent to the terminal file.
*/

Nlm_DisplaY  Nlm_NormalDisplay PROTO((Nlm_GrouP prnt, Nlm_Int2 width, Nlm_Int2 height));

Nlm_DisplaY  Nlm_ScrollDisplay PROTO((Nlm_GrouP prnt, Nlm_Int2 width, Nlm_Int2 height));

/***  SIMPLE UNIVERSAL DRAWING OBJECT  ***/

/*
*  A panel object is a rectangular box on a window that provides a universal
*  drawing environment. Panels translate click actions and drawing requests
*  to instance-specific procedures.  The SimplePanel and AutonomousPanel objects
*  are autonomous in that they create a single object within a window or group.
*  (The more general and complex slate/panel combinations are discussed in the
*  next section.)  SetPanelClick assigns click, drag, hold and release callbacks
*  to a given panel.  SimplePanels have only the minimum parameters to specify a
*  panel, and it is expected that the specific application callbacks for click,
*  draw, etc., will handle the entire behavior of the panel.
*
*  AutonomousPanels allow the creation of higher-level graphical objects that
*  can function like built-in Vibrant control objects while allowing much fancier
*  graphical display and manipulation.  The extra parameter specifies the number
*  of extra bytes to be placed on top of the panel instance data, and the data
*  can be accessed with SetPanelExtra and GetPanelExtra.  The reset callback is
*  called via the Reset class function, and should be used to free any instance-
*  specific allocated memory that may be pointed to in the extra data.  In order
*  to override certain class functions (e.g., to allow SetTitle to apply to a
*  particular autonomous panel), the classPtr function can point to a GphPrcs
*  array (in static or heap memory).  Any function pointer that is not NULL will
*  override the standard function.
*
*  The purpose of providing separate slates and panels (see next section) is to
*  allow multiple independent panels to be placed in the same scrolling unit.
*  The slate handles scrolling, and the individual panel children have their
*  own click and draw callbacks.  Slates are distinguished internally from
*  autonomous panels because their click through reset procedures are all NULL.
*/

Nlm_PaneL  Nlm_SimplePanel PROTO((Nlm_GrouP prnt, Nlm_Int2 pixwidth, Nlm_Int2 pixheight, Nlm_PnlActnProc draw));

Nlm_PaneL  Nlm_AutonomousPanel4 PROTO((Nlm_GrouP prnt, Nlm_Int2 pixwidth, Nlm_Int2 pixheight, Nlm_PnlActnProc draw, Nlm_SltScrlProc4 vscrl, Nlm_SltScrlProc4 hscrl, Nlm_Int2 extra, Nlm_PnlActnProc reset, Nlm_GphPrcsPtr classPtr));
Nlm_PaneL  Nlm_AutonomousPanel PROTO((Nlm_GrouP prnt, Nlm_Int2 pixwidth, Nlm_Int2 pixheight, Nlm_PnlActnProc draw, Nlm_SltScrlProc vscrl, Nlm_SltScrlProc hscrl, Nlm_Int2 extra, Nlm_PnlActnProc reset, Nlm_GphPrcsPtr classPtr));


#ifdef _OPENGL
void Nlm_Set3DColorMap PROTO((Nlm_PaneL w, Nlm_Uint2 totalColors,
                             Nlm_Uint1Ptr red, Nlm_Uint1Ptr green,
                             Nlm_Uint1Ptr blue, void **display));


Nlm_PaneL  Nlm_Autonomous3DPanel PROTO((Nlm_GrouP prnt, Nlm_Int2 pixwidth,
                                       Nlm_Int2 pixheight, Nlm_PnlActnProc draw,
                                       Nlm_SltScrlProc vscrl, Nlm_SltScrlProc hscrl,
                                       Nlm_Int2 extra, Nlm_PnlActnProc reset,
                                       Nlm_GphPrcsPtr classPtr, Nlm_Boolean *IndexMode,
                                       void **display, void **visinfo));
#endif /* _OPENGL */



void       Nlm_SetPanelClick PROTO((Nlm_PaneL p, Nlm_PnlClckProc click, Nlm_PnlClckProc drag, Nlm_PnlClckProc hold, Nlm_PnlClckProc release));

void       Nlm_SetPanelExtra PROTO((Nlm_PaneL p, Nlm_VoidPtr sptr));
void       Nlm_GetPanelExtra PROTO((Nlm_PaneL p, Nlm_VoidPtr sptr));

/***  GENERALIZED SCROLLABLE DRAWING OBJECT  ***/

/*
*  A slate object is a rectangular box on a window that provides a universal
*  drawing environment to its panel object children. Panels translate click
*  actions and drawing requests to instance-specific procedures.  The parent
*  slate must be notified of individual row and column pixel offsets, and of
*  the corresponding row heights and column widths, in order for slate scroll
*  bars to work automatically.  The panel must be notified of the actual size
*  of its virtual rectangle.  The panel instance procedures should use the
*  GetOffset procedure to determine the pixel offset for purposes of drawing
*  and responding to clicks.  The VirtualSlate procedure allows specification
*  of virtual scroll positions before and after the real scroll positions in
*  a slate.  When the user drags the scroll bar thumb into a virtual area, the
*  virtual action callback should be used to load new data into the slate.
*  The GeneralSlate allows the specification of an alternative scroll bar
*  callback procedure for a scrolling slate, as well as extra instance space
*  and an overriding class function, in order to effectively have an autonomous
*  scrolling panel.  The default scroll bar procedure should be sufficient
*  for most situations.  Superimposed panels, or derivatives of panels, can be
*  used to build complicated behaviors using simpler objects.
*/

Nlm_SlatE  Nlm_ScrollSlate PROTO((Nlm_GrouP prnt, Nlm_Int2 width, Nlm_Int2 height));
Nlm_SlatE  Nlm_NormalSlate PROTO((Nlm_GrouP prnt, Nlm_Int2 width, Nlm_Int2 height));
Nlm_SlatE  Nlm_HiddenSlate PROTO((Nlm_GrouP prnt, Nlm_Int2 width, Nlm_Int2 height));
Nlm_SlatE  Nlm_GeneralSlate PROTO((Nlm_GrouP prnt, Nlm_Int2 width, Nlm_Int2 height, Nlm_SltScrlProc scrl, Nlm_Int2 extra, Nlm_GphPrcsPtr classPtr));
void       Nlm_VirtualSlate PROTO((Nlm_SlatE s, Nlm_Int2 before, Nlm_Int2 after, Nlm_SltScrlProc actn));

void       Nlm_RegisterRow PROTO((Nlm_SlatE s, Nlm_Int2 position, Nlm_Int2 height, Nlm_Int2 count));
void       Nlm_RegisterColumn PROTO((Nlm_SlatE s, Nlm_Int2 position, Nlm_Int2 width, Nlm_Int2 count));

Nlm_BaR    Nlm_GetSlateVScrollBar PROTO((Nlm_SlatE s));
Nlm_BaR    Nlm_GetSlateHScrollBar PROTO((Nlm_SlatE s));

Nlm_PaneL  Nlm_CustomPanel PROTO((Nlm_SlatE s, Nlm_PnlActnProc draw, Nlm_Int2 extra, Nlm_PnlActnProc reset));
Nlm_PaneL  Nlm_GeneralPanel PROTO((Nlm_SlatE s, Nlm_PnlActnProc draw, Nlm_Int2 extra, Nlm_PnlActnProc reset, Nlm_GphPrcsPtr classPtr));

void       Nlm_RegisterRect PROTO((Nlm_PaneL p, Nlm_RectPtr r));

void       Nlm_SetSlateChar PROTO((Nlm_SlatE s, Nlm_SltCharProc chr));
void       Nlm_CaptureSlateFocus  PROTO((Nlm_SlatE s));

void       Nlm_SetSlateBorder PROTO((Nlm_SlatE s, Nlm_Boolean turnon));  /* dgg -- let us turn this adornment on/off */

/* SLATE NAVIGATION AND FOCUS POLICY */
#define GET_VALUE   0x1  /* do nothing but return current slate polycy */
#define VERT_PAGE   0x2  /* control the slate scrollbar by PgUp/PgDn keys */
#define HORIZ_PAGE  0x4  /* control the slate scrollbar by Home/End  keys */
#define VERT_ARROW  0x8  /* control the slate scrollbar by vert. arrow keys */
#define HORIZ_ARROW 0x10 /* control the slate scrollbar by horiz.arrow keys */
#define AUTO_FOCUS  0x20 /* capture input focus when clicking on the slate */
#define FOCUS_NAVIG 0x40 /* handle navig.keys when the slate is having focus,
                            even if the slate has registered its own keyProc */
#define DEFAULT_SLATE_POLICY  (VERT_PAGE | HORIZ_PAGE | VERT_ARROW | HORIZ_ARROW | AUTO_FOCUS)

/* Set new, return previous policy */
Nlm_Int4   Nlm_SetSlatePolicy PROTO((Nlm_SlatE s, Nlm_Int4 flag));


/***  APPLICATION CHARACTER PROCEDURE  ***/

/*
*  The KeyboardView procedure specifies an application procedure to be
*  called whenever the user presses a key on the keyboard, regardless of
*  other action to be taken in response to that event.
*/

void  Nlm_KeyboardView PROTO((Nlm_KeyProc key));

/***  PRINTING PROCEDURES  ***/

Nlm_WindoW   Nlm_StartPrinting PROTO((void));
void         Nlm_EndPrinting PROTO((Nlm_WindoW w));

Nlm_Boolean  Nlm_StartPage PROTO((void));
Nlm_Boolean  Nlm_EndPage PROTO((void));

Nlm_Boolean  Nlm_PrintingRect PROTO((Nlm_RectPtr rpt));
Nlm_Boolean  Nlm_ClipPrintingRect PROTO((const Nlm_RecT PNTR rpt));

/***  PICTURE TO CLIPBOARD PROCEDURES  ***/

Nlm_WindoW  Nlm_StartPicture PROTO((Nlm_RectPtr r));
void        Nlm_EndPicture PROTO((Nlm_WindoW w));

/***  STRING TO AND FROM CLIPBOARD PROCEDURES  ***/

void         Nlm_StringToClipboard PROTO((Nlm_CharPtr str));
Nlm_CharPtr  Nlm_ClipboardToString PROTO((void));
Nlm_Boolean  Nlm_ClipboardHasString PROTO((void));

/***  FILE SPECIFICATION PROCEDURES  ***/

Nlm_Boolean  Nlm_GetInputFileName PROTO((Nlm_CharPtr fileName, size_t maxsize, Nlm_CharPtr extType, Nlm_CharPtr macType));
Nlm_Boolean  Nlm_GetOutputFileName PROTO((Nlm_CharPtr fileName, size_t maxsize, Nlm_CharPtr dfault));

/***  APPLICATION TIMER PROCEDURE  ***/

/*
*  The Metronome procedure specifies an application procedure to be called
*  18 or 20 times per second (on the PC and the Macintosh, respectively)
*  regardless of any action by the user.
*/

void  Nlm_Metronome PROTO((Nlm_VoidProc actn));

/*
*  Each window can have a timer associated with it, called right after
*  the metronome callback.
*/

void  Nlm_SetWindowTimer PROTO((Nlm_WindoW w, Nlm_WndActnProc actn));

/***  MOUSE MOVEMENT PROCEDURE  ***/

void Nlm_SetMouseMoveCallback (Nlm_ClckProc actn);

void Nlm_SetMouseMoveRegion (Nlm_RegioN rgn);

/***  MAIN EVENT LOOP PROCEDURE  ***/

/*
*  After creating all menus, windows, and window objects, the Nlm_Main
*  procedure should call ProcessEvents.  ProcessEvents will convert all
*  toolbox events or messages into calls to the appropriate object.
*/

void  Nlm_ProcessEvents PROTO((void));

/***  QUIT PROGRAM PROCEDURE  ***/

/*
*  When a callback function determines that the program is finished it
*  should call QuitProgram.  This will allow the ProcessEvents loop to
*  be exited.
*/

void  Nlm_QuitProgram PROTO((void));


/* Return TRUE if QuitProgram() has been called
 */
Nlm_Boolean Nlm_QuittingProgram PROTO((void));


/***  PROCEDURES THAT ATTACH CALLBACKS TO INTERFACE OBJECTS  ***/

/*
*  Given a handle to an object, these procedures will set callback
*  functions for various kinds of events.  The action procedure is
*  executed whenever the user does something like clicking the mouse
*  button on an object and then releasing the mouse button while
*  the mouse is still over the object.  The item parameter is 0 to
*  refer to a parent or individual object, and is >0 to refer to a
*  particular child of a group object.  Although only some of the
*  above procedures take an action as an explicit parameter, every
*  graphic object has an action procedure field, which can be set
*  or reset as desired.
*/

void  Nlm_SetAction PROTO((Nlm_Handle a, Nlm_ActnProc actn));

/***  PROCEDURES THAT MODIFY INTERFACE OBJECTS  ***/

/***  Object Position and Appearance Procedures  ***/

/*
*  When an object is added to a window or group, it is placed at the
*  next available location.  The parent then records the size and
*  position of the object and recalculates the next available location.
*  Advance will place the next object to the right of the previous one
*  (normally it would be placed below the previous object).  Break will
*  place the next object at the left margin underneath all previously
*  defined objects, and will reset the top margin so that Advance will
*  work properly.  SetNextPosition and GetNextPosition give programs
*  finer control over object positioning, including the ability to
*  overlap objects.  Creating a group with width and height equal to 0
*  will also result in overlapped objects.
*/

void  Nlm_Advance PROTO((Nlm_Handle a));
void  Nlm_Break PROTO((Nlm_Handle a));

void  Nlm_SetNextPosition PROTO((Nlm_Handle a, Nlm_PoinT nps));
void  Nlm_GetNextPosition PROTO((Nlm_Handle a, Nlm_PointPtr nps));

/***  Item Count Procedure  ***/

/*
*  CountItems returns the number of items in a parent object.
*/

Nlm_Int2  Nlm_CountItems PROTO((Nlm_Handle a));

/***  Object Visibility and Responsiveness Procedures  ***/

/*
*  Show makes any object visible (unless, for example, the parent of the
*  object happens to be invisible).  All objects except windows are shown
*  when they are created.  Hide will make any object invisible.  Enable
*  and Disable change the responsiveness of an object to mouse clicks and
*  key presses.  All objects are enabled when created.  Select is used to
*  bring windows to the front, to select a dialog text item, or to select
*  a particular display object for use as a console terminal display.
*/

Nlm_Boolean  Nlm_Enabled PROTO((Nlm_Handle a));
Nlm_Boolean  Nlm_Visible PROTO((Nlm_Handle a));
void         Nlm_ObjectRect PROTO((Nlm_Handle a, Nlm_RectPtr r));
void         Nlm_InvalObject PROTO((Nlm_Handle a));

void         Nlm_Show PROTO((Nlm_Handle a));
void         Nlm_Hide PROTO((Nlm_Handle a));
void         Nlm_Enable PROTO((Nlm_Handle a));
void         Nlm_Disable PROTO((Nlm_Handle a));
void         Nlm_Select PROTO((Nlm_Handle a));

/***  Object Title and State Manipulation Procedures  ***/

/*
*  The status of an object is polled with one of the following procedures.
*  Title refers to the title or prompt of an object.  Three kinds of values
*  are recognized:  Groups of radio buttons, single choice lists, and menu
*  choice groups have elements that are mutually exclusive, and thus have a
*  single integer as their value.  The SetValue and GetValue procedures are
*  appropriate for these objects.  Boolean objects (check boxes, status items)
*  use SetStatus and GetStatus.  GetNextItem will find the first set Boolean
*  item after prev, and will return 0 if there are no more set items.  Text
*  objects use SetTitle and GetTitle.  SetOffset and GetOffset will manipulate
*  the scroll bar offsets for any object.  SetPosition and GetPosition
*  manipulate the position of an object.  SetRange is used to set the page
*  increment and maximum values for a scroll bar.  Reset will clear the value
*  of an object or remove all children from a group.
*/

void         Nlm_SetTitle PROTO((Nlm_Handle a, Nlm_CharPtr title));
void         Nlm_GetTitle PROTO((Nlm_Handle a, Nlm_CharPtr title, size_t maxsize));
void         Nlm_SetValue PROTO((Nlm_Handle a, Nlm_Int2 value));
Nlm_Int2     Nlm_GetValue PROTO((Nlm_Handle a));
void         Nlm_SetStatus PROTO((Nlm_Handle a, Nlm_Boolean status));
Nlm_Boolean  Nlm_GetStatus PROTO((Nlm_Handle a));
void         Nlm_SetOffset PROTO((Nlm_Handle a, Nlm_Int2 horiz, Nlm_Int2 vert));
void         Nlm_GetOffset PROTO((Nlm_Handle a, Nlm_Int2Ptr horiz, Nlm_Int2Ptr vert));
void         Nlm_SetPosition PROTO((Nlm_Handle a, Nlm_RectPtr r));
void         Nlm_GetPosition PROTO((Nlm_Handle a, Nlm_RectPtr r));
void         Nlm_SetRange PROTO((Nlm_Handle a, Nlm_Int2 pgUp, Nlm_Int2 pgDn, Nlm_Int2 max));
void         Nlm_AdjustPrnt PROTO((Nlm_Handle a, Nlm_RectPtr r, Nlm_Boolean align));

void         Nlm_SetItemTitle PROTO((Nlm_Handle a, Nlm_Int2 item, Nlm_CharPtr title));
void         Nlm_GetItemTitle PROTO((Nlm_Handle a, Nlm_Int2 item, Nlm_CharPtr title, size_t maxsize));
void         Nlm_SetItemStatus PROTO((Nlm_Handle a, Nlm_Int2 item, Nlm_Boolean status));
Nlm_Boolean  Nlm_GetItemStatus PROTO((Nlm_Handle a, Nlm_Int2 item));

Nlm_Int2     Nlm_GetNextItem PROTO((Nlm_Handle a, Nlm_Int2 prev));

void         Nlm_Reset PROTO((Nlm_Handle a));

/***  Object Destruction Procedure  ***/

/*
*  Remove hides an object and then completely disposes of all memory structures
*  associated with that object.
*/

Nlm_Handle  Nlm_Remove PROTO((Nlm_Handle a));

/***  MISCELLANEOUS WINDOW AND OBJECT PROCEDURES  ***/

Nlm_Handle   Nlm_Parent PROTO((Nlm_Handle a));

void         Nlm_RealizeWindow PROTO((Nlm_WindoW w));
void         Nlm_IconifyWindow PROTO((Nlm_WindoW w));
Nlm_Boolean  Nlm_IconicWindow  PROTO((Nlm_WindoW w));
Nlm_WindoW   Nlm_WhichWindow PROTO((Nlm_PoinT mouseLoc));
Nlm_Boolean  Nlm_InWindow PROTO((Nlm_PoinT mouseLoc));
Nlm_WindoW   Nlm_FrontWindow PROTO((void));
Nlm_Boolean  Nlm_InFront PROTO((Nlm_WindoW w));
void         Nlm_UseWindow PROTO((Nlm_WindoW w));
Nlm_WindoW   Nlm_CurrentWindow PROTO((void));
Nlm_Boolean  Nlm_UsingWindow PROTO((Nlm_WindoW w));
Nlm_WindoW   Nlm_ActiveWindow PROTO((void));
void         Nlm_EraseWindow PROTO((Nlm_WindoW w));
Nlm_WindoW   Nlm_ParentWindow PROTO((Nlm_Handle a));
Nlm_WindoW   Nlm_SavePort PROTO((Nlm_Handle a));
void         Nlm_RestorePort PROTO((Nlm_WindoW w));
void         Nlm_Update PROTO((void));
Nlm_Boolean  Nlm_EventAvail PROTO((void));
void         Nlm_FlushEvents PROTO((void));
void         Nlm_ProcessAnEvent PROTO((void));
void         Nlm_ProcessEventOrIdle PROTO((void));
void         Nlm_ProcessExternalEvent PROTO((void));
Nlm_Boolean  Nlm_AllParentsEnabled PROTO((Nlm_Handle a));
Nlm_Boolean  Nlm_AllParentsVisible PROTO((Nlm_Handle a));
void         Nlm_RemoveDyingWindows PROTO((void));
void         Nlm_UnloadSegment PROTO((Nlm_VoidProc routineAddr));
Nlm_Boolean  Nlm_LaunchApp PROTO((Nlm_CharPtr fileName));
void         Nlm_SetBarAnomaly PROTO((Nlm_BaR b, Nlm_Boolean anomaly));
void         Nlm_CopyWindowImage PROTO((void));

void        Nlm_SetObjectExtra PROTO((Nlm_Handle a, Nlm_VoidPtr data, Nlm_FreeProc cleanup));
Nlm_VoidPtr Nlm_GetObjectExtra PROTO((Nlm_Handle a));

void        Nlm_ProcessUpdatesFirst  PROTO((Nlm_Boolean val));

/* Bring the parent window of object "a" to the top of Z order;
 *  if the parent window is already shown then(unlike the Nlm_Select())
 *  do not switch current input focus, otherwise just call Nlm_Show()
 */
void        Nlm_PopupParentWindow PROTO((Nlm_Handle a));

/* Run the specified application(executable located in "path").
 * The "argv" argument is an array of character pointers to
 * null-terminated strings.  These strings constitute the argument
 * list available to the new process image.
 * The "argv" argument vector is terminated by a null pointer.
 */
extern Nlm_Boolean Nlm_Execv PROTO((const Nlm_Char* path, Nlm_Char *const * argv));
#define Execv Nlm_Execv

/***  OBJECT ALIGNMENT PROCEDURE FOR USE WITHOUT GROUP REPOSITIONING  ***/

#if defined(WIN_MAC)
#define Nlm_HANDLE Nlm_Handle
#elif defined(WIN_MOTIF)
#define Nlm_HANDLE Nlm_Handle
#elif defined(WIN_MSWIN)
#define Nlm_HANDLE int
#elif !defined(WIN_GIF)
#error "Vibrant applications must be compiled with WIN_*** defined!!!"
#endif

#define ALIGN_LEFT      1
#define ALIGN_RIGHT     2
#define ALIGN_CENTER    3
#define ALIGN_JUSTIFY   4
#define ALIGN_UPPER     5
#define ALIGN_LOWER     6
#define ALIGN_MIDDLE    7
#define ALIGN_VERTICAL  8

void CDECL   Nlm_AlignObjects VPROTO((int align, ...));


/***  NAVIGATION AND SPECIAL KEY CODES  ***/

#define NLM_PREV    11
#define NLM_NEXT    12
#define NLM_END      4
#define NLM_HOME     1

#define NLM_DEL    127
#define NLM_INS      5
#define NLM_ESC     27

#define NLM_LEFT    28
#define NLM_RIGHT   29
#define NLM_UP      30
#define NLM_DOWN    31


/***  EDIT MENU DEFINES, TYPES AND PROCEDURES  ***/

#define UNDO_MENU_ITEM "Undo/Z"
#define CUT_MENU_ITEM "Cut/X"
#define COPY_MENU_ITEM "Copy/C"
#define PASTE_MENU_ITEM "Paste/V"
#define CLEAR_MENU_ITEM "Clear"
#define DELETE_MENU_ITEM "Delete"

extern Nlm_TexT Nlm_CurrentVisibleText (void);
extern void Nlm_StdCutTextProc (Nlm_IteM i);
extern void Nlm_StdCopyTextProc (Nlm_IteM i);
extern void Nlm_StdPasteTextProc (Nlm_IteM i);
extern void Nlm_StdDeleteTextProc (Nlm_IteM i);

/***  FILE HANDLING PROCEDURES  ***/

/*
*  Files do not use the same procedures as other kinds of (visual) objects, and
*  have separate procedures for creation, manipulation, and destruction.  The
*  fileDone flag is set if the file was successfully opened.  The fileDone flag
*  also indicates whether a read or write statement was successful, and can be
*  used to determine if the end of a file was detected by a read statement.
*/

void         Nlm_ReadText PROTO((FILE *f, Nlm_CharPtr str, size_t maxsize));
void CDECL   Nlm_WriteText VPROTO((FILE *f, char *format, ...));
void CDECL   Nlm_WriteLog VPROTO((char *format, ...));

Nlm_Char     Nlm_ReadChar PROTO((FILE *f));
void         Nlm_ReadString PROTO((FILE *f, Nlm_CharPtr str, size_t maxsize));
void         Nlm_ReadField PROTO((FILE *f, Nlm_CharPtr str, size_t maxsize));
void         Nlm_ReadLine PROTO((FILE *f, Nlm_CharPtr str, size_t maxsize));
Nlm_Uint2    Nlm_ReadCard PROTO((FILE *f));
Nlm_Int2     Nlm_ReadInt PROTO((FILE *f));
Nlm_Int4     Nlm_ReadLong PROTO((FILE *f));
Nlm_FloatLo  Nlm_ReadReal PROTO((FILE *f));
Nlm_FloatHi  Nlm_ReadDouble PROTO((FILE *f));
void         Nlm_WriteChar PROTO((FILE *f, Nlm_Char ch));
void         Nlm_WriteLn PROTO((FILE *f));
void         Nlm_WriteString PROTO((FILE *f, Nlm_CharPtr str));
void         Nlm_WriteCard PROTO((FILE *f, Nlm_Uint2 cardval, Nlm_Int2 length));
void         Nlm_WriteInt PROTO((FILE *f, Nlm_Int2 intval, Nlm_Int2 length));
void         Nlm_WriteLong PROTO((FILE *f, Nlm_Int4 longval, Nlm_Int2 length));
void         Nlm_WriteReal PROTO((FILE *f, Nlm_FloatLo realval, Nlm_Int2 length, Nlm_Int2 dec));
void         Nlm_WriteDouble PROTO((FILE *f, Nlm_FloatHi doubleval, Nlm_Int2 length, Nlm_Int2 dec));


/*** esl: FONT HANDLING PROCEDURES ***/

/* Interactive font selection (last param should be NULL for this version) */
Nlm_Boolean  Nlm_ChooseFont PROTO((Nlm_FontSpecPtr fsp, Nlm_Uint2 flags, Nlm_VoidPtr null));
#define CFF_READ_FSP    1
#define CFF_MONOSPACE   2

/* FontSpec/string conversions */
Nlm_Boolean  Nlm_StrToFontSpec PROTO((Nlm_CharPtr str, Nlm_FontSpecPtr fsp));
void         Nlm_FontSpecToStr PROTO((Nlm_FontSpecPtr fsp, Nlm_CharPtr str, size_t maxsize));


/***  STRING HANDLING PROCEDURES  ***/

/* To compose a printable text for multiline TexT objects for all platforms.
 * The resulting string will contain only printable characters(and '\n', '\t'
 * and (on non-WIN_MAC platforms) '\r').
 * On non-Mac platforms insert '\r' before '\n' symbols not preceeded
 * by '\r';  otherwise, remove all '\r' symbols and replace '\n' by '\015'.
 * NB: The caller is responsible to free the memory allocated for the result.
 */
Nlm_CharPtr Nlm_StrngPrintable PROTO((const Nlm_Char PNTR str));

/*
*  These string functions are intended to be both robust and portable
*  across all operating systems that the NCBI toolbox will support.
*  The maxsize parameter is usually passed sizeof (dest), and will
*  prevent characters from being written past the allocated memory of
*  a string.
*/

Nlm_Uint4    Nlm_StrngLen PROTO((Nlm_CharPtr string));
void         Nlm_StrngCat PROTO((Nlm_CharPtr dest, Nlm_CharPtr source, size_t maxsize));
void         Nlm_StrngCpy PROTO((Nlm_CharPtr dest, Nlm_CharPtr source, size_t maxsize));
Nlm_Boolean  Nlm_StrngPos PROTO((Nlm_CharPtr str, Nlm_CharPtr sub, Nlm_Uint4 start, Nlm_Boolean caseCounts, Nlm_Uint4Ptr match));
void         Nlm_StrngSeg PROTO((Nlm_CharPtr dest, Nlm_CharPtr source, Nlm_Uint4 start, Nlm_Uint4 length, size_t maxsize));
void         Nlm_StrngRep PROTO((Nlm_CharPtr dest, Nlm_CharPtr source, Nlm_Uint4 start, size_t maxsize));
Nlm_Boolean  Nlm_StrngEql PROTO((Nlm_CharPtr str1, Nlm_CharPtr str2, Nlm_Boolean caseCounts));
Nlm_Int2     Nlm_StrngCmp PROTO((Nlm_CharPtr str1, Nlm_CharPtr str2, Nlm_Boolean caseCounts));
Nlm_Int2     Nlm_SymblCmp PROTO((Nlm_CharPtr str1, Nlm_CharPtr str2, Nlm_Boolean caseCounts));

/***  STRING TO NUMBER CONVERSION PROCEDURES  ***/

/*
*  The boolean value returned by the String to Number functions indicates
*  whether the conversion was successful (meaning that the string was in
*  fact a legitimate number in the proper range).
*/

Nlm_Boolean  Nlm_StrToCard PROTO((Nlm_CharPtr str, Nlm_Uint2Ptr cardval));
Nlm_Boolean  Nlm_StrToInt PROTO((Nlm_CharPtr str, Nlm_Int2Ptr intval));
Nlm_Boolean  Nlm_StrToLong PROTO((Nlm_CharPtr str, Nlm_Int4Ptr longval));
Nlm_Boolean  Nlm_StrToPtr PROTO((Nlm_CharPtr str, Nlm_VoidPtr PNTR ptrval));
Nlm_Boolean  Nlm_StrToReal PROTO((Nlm_CharPtr str, Nlm_FloatLoPtr realval));
Nlm_Boolean  Nlm_StrToDouble PROTO((Nlm_CharPtr str, Nlm_FloatHiPtr doubleval));
void         Nlm_CardToStr PROTO((Nlm_Uint2 cardval, Nlm_CharPtr str, Nlm_Int2 length, size_t maxsize));
void         Nlm_IntToStr PROTO((Nlm_Int2 intval, Nlm_CharPtr str, Nlm_Int2 length, size_t maxsize));
void         Nlm_LongToStr PROTO((Nlm_Int4 longval, Nlm_CharPtr str, Nlm_Int2 length, size_t maxsize));
void         Nlm_PtrToStr PROTO((Nlm_VoidPtr ptrval, Nlm_CharPtr str, Nlm_Int2 length, size_t maxsize));
void         Nlm_RealToStr PROTO((Nlm_FloatLo realval, Nlm_CharPtr str, Nlm_Int2 length, Nlm_Int2 dec, size_t maxsize));
void         Nlm_DoubleToStr PROTO((Nlm_FloatHi doubleval, Nlm_CharPtr str, Nlm_Int2 length, Nlm_Int2 dec, size_t maxsize));

/***  SPECIALIZED MEMORY ALLOCATION PROCEDURES  ***/

/*
*  SetString and GetString are used to conveniently copy C strings to
*  and from relocatable memory allocated on the heap.
*/

Nlm_Handle  Nlm_SetString PROTO((Nlm_Handle h, Nlm_CharPtr str));
void        Nlm_GetString PROTO((Nlm_Handle h, Nlm_CharPtr str, size_t maxsize));


/***  MISCELLANEOUS PROCEDURES  ***/


/*
 *  The supported cursor shapes
 */

void         Nlm_ArrowCursor PROTO((void));
void         Nlm_CrossCursor PROTO((void));
void         Nlm_IBeamCursor PROTO((void));
void         Nlm_PlusCursor  PROTO((void));
void         Nlm_WatchCursor PROTO((void));

typedef enum
  {
    Nlm_CURSOR_ARROW = 0,
#define CURSOR_ARROW    Nlm_CURSOR_ARROW
#define CURSOR_DEFAULT  Nlm_CURSOR_ARROW
    Nlm_CURSOR_CROSS,
#define CURSOR_CROSS    Nlm_CURSOR_CROSS
    Nlm_CURSOR_IBEAM,
#define CURSOR_IBEAM    Nlm_CURSOR_IBEAM
    Nlm_CURSOR_PLUS,
#define CURSOR_PLUS     Nlm_CURSOR_PLUS
    Nlm_CURSOR_WATCH,
#define CURSOR_WATCH    Nlm_CURSOR_WATCH

    /* add new cursor shape IDs before this comment */

    Nlm_N_CURSORS,
#define N_CURSORS       Nlm_N_CURSORS
    Nlm_WHAT_CURSOR,
#define WHAT_CURSOR     Nlm_WHAT_CURSOR
    Nlm_BAD_CURSOR
#define BAD_CURSOR      Nlm_BAD_CURSOR
  }
  Nlm_enumCursorShape;
#define enumCursorShape Nlm_enumCursorShape

/*
 *  All new cursor shapes should be added within this function
 */
Nlm_Boolean Nlm_InitCursorShapes  PROTO( ( void ) );
#define InitCursorShapes Nlm_InitCursorShapes

/*
 *  on success, set current cursor shape to "cursor_shape" and
 *  return previously set cursor shape;
 *  on error, return BAD_CURSOR;
 *  if "cursor_shape" == WHAT_CURSOR then do nothing but
 *  return previously set cursor shape
 */
enumCursorShape Nlm_SetCursorShape  PROTO((enumCursorShape cursor_shape));
#define SetCursorShape Nlm_SetCursorShape


/*
 *  Etc
 */

void         Nlm_Version PROTO((Nlm_CharPtr vsn, size_t maxsize));

void         Nlm_MousePosition PROTO((Nlm_PointPtr pt));
Nlm_Boolean  Nlm_MouseButton PROTO((void));
Nlm_Int4     Nlm_ComputerTime PROTO((void));

#ifdef WIN_MSWIN
void         Nlm_DisableVibrant PROTO((void));
void         Nlm_EnableVibrant PROTO((void));
#endif /* WIN_MSWIN */

/* dgg++ added for use w/ DCLAP c++ library */
void Nlm_SetObject PROTO((Nlm_GraphiC a, Nlm_VoidPtr thisobject));
Nlm_VoidPtr Nlm_GetObject PROTO((Nlm_GraphiC a));

#ifdef WIN_MAC
extern void Nlm_SendOpenDocAppleEvent PROTO((Nlm_CharPtr datafile, Nlm_CharPtr sig));
extern void Nlm_SendOpenDocAppleEventEx PROTO((Nlm_CharPtr datafile, Nlm_CharPtr sig, Nlm_CharPtr prog, Nlm_Boolean wantReply));
extern void Nlm_SendURLAppleEvent (Nlm_CharPtr urlString, Nlm_CharPtr sig, Nlm_CharPtr prog);
#endif

#ifdef WIN_MSWIN
/* Call application registered to open the given type of document.
 * The doc type is recognized automagically using the file extention;
 * if the doc is URL then its name must explicitely start from "http://".
 */
extern Nlm_Boolean Nlm_MSWin_OpenDocument(const Nlm_Char* doc_name);

/* Call program (e.g., Cn3D) with parameters (e.g., data file name) */
extern Nlm_Boolean Nlm_MSWin_OpenApplication(const Nlm_Char* program, const Nlm_Char* parameters);
extern void Nlm_GetExecPath(char *filetype, char *buf, int buflen);
#endif

#ifdef WIN_MOTIF
extern void Nlm_SetMotifWindowName PROTO((Nlm_CharPtr nm));
#endif

extern Nlm_Boolean Nlm_ChooseColorDialog PROTO(( 
       Nlm_Uint1 PNTR redptr,Nlm_Uint1 PNTR greenptr, Nlm_Uint1 PNTR blueptr,
       Nlm_Boolean Initial ));

extern void LIBCALL Nlm_RegisterDropProc PROTO((Nlm_DropProc dropProc));
extern void LIBCALL Nlm_RegisterServiceProc PROTO((Nlm_ServiceProc serviceProc));
extern void LIBCALL Nlm_RegisterResultProc PROTO((Nlm_ResultProc resultProc));

#ifdef OS_UNIX
extern void Nlm_FineGranularitySleep PROTO((Int4 seconds, Int4 microseconds));
#endif

#ifdef WIN_MOTIF
extern void Nlm_WaitForXEvent  PROTO((void));
#define Nlm_WaitForCondition(cond) { \
  while (cond) { \
    Nlm_WaitForXEvent(); \
    Nlm_ProcessExternalEvent(); \
  } \
  }
#else
#define Nlm_WaitForCondition(cond) while (cond) { Nlm_ProcessExternalEvent(); }
#endif /* WIN_MOTIF */

#ifdef WIN_MOTIF
typedef enum
{
  Nlm_IO_READ,    /* file descriptor has data available    */
  Nlm_IO_WRITE,   /* file descriptor available for writing */
  Nlm_IO_ERROR,   /* I/O error(exception) occurred         */
  Nlm_IO_CLEANUP  /* when called from UnregisterIO()       */
} Nlm_IOtype;

struct Nlm_IOstruct;
typedef struct Nlm_IOstruct PNTR Nlm_IO;

/* IO callback function */
typedef void (*Nlm_IOcallback)(Nlm_IO io, int fd, Nlm_IOtype type,
                               Nlm_VoidPtr udata);

extern Nlm_IO Nlm_RegisterIO PROTO((int fd,
                                    Nlm_IOcallback ufunc, Nlm_VoidPtr udata,
                                    Nlm_IOtype type));
extern void Nlm_UnregisterIO PROTO((Nlm_IO io));
#endif /* WIN_MOTIF */


#ifdef __cplusplus
}
#endif

#endif

