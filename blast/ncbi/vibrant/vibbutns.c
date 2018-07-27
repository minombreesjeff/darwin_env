/*   vibbutns.c
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
* File Name:  vibbutns.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   7/1/91
*
* $Revision: 6.8 $
*
* File Description: 
*       Vibrant button functions
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: vibbutns.c,v $
* Revision 6.8  2004/01/20 23:34:38  sinyakov
* [WIN_MSWIN]: fixed to display '&' character in button text
*
* Revision 6.7  2002/03/07 19:18:21  kans
* check box wid += 4 for Aqua
*
* Revision 6.6  2002/02/05 14:02:28  kans
* adjust push button margins in Mac OS X Aqua
*
* Revision 6.5  2002/01/09 23:22:02  juran
* Make buttons wider in Aqua.
*
* Revision 6.4  2000/02/07 20:17:35  lewisg
* minor bug fixes, use gui font for win32
*
* Revision 6.3  1999/10/19 12:48:19  thiessen
* minor hack to Nlm_RemoveDefaultButton to allow Cn3D to run under RH linux 6 with Lesstif library
*
* Revision 6.2  1997/12/12 21:08:28  kans
* a number of symbols changed in the latest CodeWarrior release, now using headers from Apple
*
* Revision 6.1  1997/11/26 21:30:11  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.0  1997/08/25 18:56:47  madden
* Revision changed to 6.0
*
* Revision 5.6  1997/08/18 14:47:30  vakatov
* [WIN_MOTIF]  Minor fix: Push/DefaultButton position adjustment
*
* Revision 5.5  1997/04/25 16:14:38  vakatov
* [WIN_MOTIF,WIN_MSWIN] Catch and render(DoSendFocus) navigation key events
*
 * Revision 5.4  1997/01/29  16:41:22  kans
 * using StringNCpy_0
 *
 * Revision 5.3  1996/07/03  18:55:16  vakatov
 * [WIN-MOTIF]  Let the MOTIF calculate the button's initial size automatically
 * (i.e. according to the MOTIF internal rules) rather than try to calculate
 * the button size using a vague guess on the used font size and the title length
 *
 * Revision 5.2  1996/06/11  16:06:10  vakatov
 * [WIN_MOTIF]  PushButton location/sizing adjustment
 *
 * Revision 5.1  1996/06/07  21:53:28  vakatov
 * [WIN_MOTIF] Default-/Push- buttons size alignment;  DefaultButton is not
 * mandatory the first button in the group
 *
 * Revision 5.0  1996/05/28  13:45:08  ostell
 * Set to revision 5.0
 *
 * Revision 4.8  1996/05/20  21:33:39  vakatov
 * [WIN_MOTIF]  All "Widget" / "NULL" comparisons and assignments replaced
 * by the "Widget" / "(Widget)0" these
 *
 * Revision 4.7  1996/05/06  19:20:51  vakatov
 * [WIN_MOTIF]  Nlm_SetButtonPosition() -- additional adjustment
 *
 * Revision 4.6  1996/05/06  19:06:07  kans
 * ifdef motif and prototype needed for newest functions
 *
 * Revision 4.5  1996/05/06  18:27:20  vakatov
 * Nlm_RemoveDefaultButton() corrected (set default button to NULL);
 * Nlm_SetButtonDefault() added to allow change button's "defaultBtn" flag;
 * [WIN_MOTIF] Nlm_PendingDefButton() trick-function added to avoid a strange
 * PushButton/DefaultButton size interference
 *
 * Revision 4.4  1996/04/30  21:50:03  vakatov
 * Since now push-buttons belonging to a group containing default button
 * have of the same size as if they were belonging to a group without a
 * default button (WIN_MOTIF feature only)
 *
 * Revision 4.3  1996/03/02  22:36:38  kans
 * reduction of X traffic (DV)
 *
 * Revision 4.2  1996/02/13  17:24:07  kans
 * accelerated set position prior to realization (Denis Vakatov)
 *
 * Revision 4.1  1995/12/25  16:03:50  kans
 * removed offset parameter for Motif push buttons
 *
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 2.13  1995/05/17  15:15:14  kans
 * added Log line
 *
*
* ==========================================================================
*/

#include <vibtypes.h>
#include <vibprocs.h>
#include <vibincld.h>

#ifdef WIN_MAC
#define Nlm_ControlTool ControlHandle
#endif

#ifdef WIN_MSWIN
#define Nlm_ControlTool HWND
#endif

#ifdef WIN_MOTIF
#define Nlm_ControlTool Widget
#endif

typedef  struct  Nlm_buttondata {
  Nlm_ControlTool  handle;
  Nlm_Int2         border;
  Nlm_Int2         offset;
  Nlm_Int2         shrinkX;
  Nlm_Int2         shrinkY;
  Nlm_Boolean      defaultBtn;
} Nlm_ButtonData;

typedef  struct  Nlm_buttonrec {
  Nlm_GraphicRec  graphicR;
  Nlm_ButtonData  button;
} Nlm_ButtonRec, PNTR Nlm_BtnPtr;

static Nlm_GphPrcsPtr  gphprcsptr = NULL;

static Nlm_GphPrcsPtr  pushProcs;
static Nlm_GphPrcsPtr  defaultProcs;
static Nlm_GphPrcsPtr  checkProcs;
static Nlm_GphPrcsPtr  radioProcs;

static Nlm_ButtoN      recentButton = NULL;
static Nlm_ButtonData  recentButtonData;

#ifdef WIN_MSWIN
static WNDPROC         lpfnNewButtonProc = NULL;
static WNDPROC         lpfnOldButtonProc = NULL;
static Nlm_Boolean     handlechar;
#endif

#ifdef WIN_MOTIF
static void NlmHint_ResetDefButtonE(Widget w, XtPointer client_data,
                                    XEvent *event,
                                    Boolean *continue_to_dispatch);
static void NlmHint_ResetDefButtonA(Widget w, XtPointer client_data,
                                    XtPointer call_data);
#endif

static void Nlm_LoadButtonData (Nlm_ButtoN b, Nlm_ControlTool hdl,
                                Nlm_Int2 bdr, Nlm_Int2 ofs,
                                Nlm_Int2 shkX, Nlm_Int2 shkY,
                                Nlm_Boolean dflt)

{
  Nlm_BtnPtr      bp;
  Nlm_ButtonData  PNTR ptr;

  if (b != NULL) {
    bp = (Nlm_BtnPtr) Nlm_HandLock (b);
    ptr = &(bp->button);
    ptr->handle = hdl;
    ptr->border = bdr;
    ptr->offset = ofs;
    ptr->shrinkX = shkX;
    ptr->shrinkY = shkY;
    ptr->defaultBtn = dflt;
    Nlm_HandUnlock (b);
    recentButton = NULL;
  }
}


static void Nlm_GetButtonData (Nlm_ButtoN b, Nlm_ButtonData * bdata)
{
  Nlm_BtnPtr  bp;

  if (b != NULL && bdata != NULL) {
    if (b == recentButton && NLM_RISKY) {
      *bdata = recentButtonData;
    } else {
      bp = (Nlm_BtnPtr) Nlm_HandLock (b);
      *bdata = bp->button;
      Nlm_HandUnlock (b);
      recentButton = b;
      recentButtonData = *bdata;
    }
  }
}

static Nlm_ControlTool Nlm_GetButtonHandle (Nlm_ButtoN b)

{
  Nlm_ButtonData  bdata;

  Nlm_GetButtonData (b, &bdata);
  return bdata.handle;
}


#ifdef WIN_MAC
static Nlm_Int2 Nlm_GetButtonBorder (Nlm_ButtoN b)
{
  Nlm_ButtonData  bdata;

  Nlm_GetButtonData (b, &bdata);
  return bdata.border;
}


static Nlm_Boolean Nlm_CommonPtInRect (Nlm_GraphiC b, Nlm_PoinT pt)

{
  Nlm_RecT  r;

  Nlm_GetRect (b, &r);
  return (Nlm_PtInRect (pt, &r));
}

static Nlm_Boolean Nlm_CommonButtonClick (Nlm_GraphiC b, Nlm_PoinT pt, Nlm_Int2 part)

{
  Nlm_ControlTool  c;
  Nlm_PointTool    ptool;

  c = Nlm_GetButtonHandle ((Nlm_ButtoN) b);
  Nlm_PoinTToPointTool (pt, &ptool);
  return (part == TrackControl (c, ptool, NULL));
}

static Nlm_Boolean Nlm_PushClick (Nlm_GraphiC b, Nlm_PoinT pt)

{
  Nlm_Boolean  rsult;

  rsult = FALSE;
  if (Nlm_CommonPtInRect (b, pt)) {
    if (Nlm_CommonButtonClick (b, pt, kControlButtonPart)) {
      Nlm_DoAction (b);
    }
    rsult = TRUE;
  }
  return rsult;
}

static Nlm_Boolean Nlm_DefaultClick (Nlm_GraphiC b, Nlm_PoinT pt)

{
  Nlm_Boolean  rsult;

  rsult = FALSE;
  if (Nlm_CommonPtInRect (b, pt)) {
    if (Nlm_CommonButtonClick (b, pt, kControlButtonPart)) {
      Nlm_DoAction (b);
    }
    rsult = TRUE;
  }
  return rsult;
}

static Nlm_Boolean Nlm_CheckClick (Nlm_GraphiC b, Nlm_PoinT pt)

{
  Nlm_Boolean  _bool_;
  Nlm_GraphiC  g;
  Nlm_Boolean  rsult;

  rsult = FALSE;
  if (Nlm_CommonPtInRect (b, pt)) {
    if (Nlm_CommonButtonClick (b, pt, kControlCheckBoxPart)) {
      _bool_ = Nlm_DoGetStatus (b, 0);
      if (_bool_) {
        Nlm_DoSetStatus (b, 0, FALSE, FALSE);
      } else {
        Nlm_DoSetStatus (b, 0, TRUE, FALSE);
      }
      Nlm_DoAction (b);
      g = Nlm_GetParent (b);
      Nlm_DoAction (g);
    }
    rsult = TRUE;
  }
  return rsult;
}

static Nlm_Boolean Nlm_RadioClick (Nlm_GraphiC b, Nlm_PoinT pt)

{
  Nlm_Boolean  _bool_;
  Nlm_GraphiC  g;
  Nlm_Boolean  rsult;

  rsult = FALSE;
  if (Nlm_CommonPtInRect (b, pt)) {
    if (Nlm_CommonButtonClick (b, pt, kControlCheckBoxPart)) {
      _bool_ = Nlm_DoGetStatus (b, 0);
      if (! _bool_) {
        g = Nlm_GetParent (b);
        Nlm_ClearItemsInGroup (g, b, FALSE);
        Nlm_DoSetStatus (b, 0, TRUE, FALSE);
        Nlm_DoAction (b);
        Nlm_DoAction (g);
      }
    }
    rsult = TRUE;
  }
  return rsult;
}
#endif

#ifdef WIN_MSWIN
static Nlm_Boolean Nlm_PushCommand (Nlm_GraphiC b)

{
  Nlm_DoAction (b);
  return TRUE;
}

static Nlm_Boolean Nlm_DefaultCommand (Nlm_GraphiC b)

{
  Nlm_DoAction (b);
  return TRUE;
}

static Nlm_Boolean Nlm_CheckCommand (Nlm_GraphiC b)

{
  Nlm_Boolean  _bool_;
  Nlm_GraphiC  g;

  _bool_ = Nlm_DoGetStatus (b, 0);
  if (_bool_) {
    Nlm_DoSetStatus (b, 0, FALSE, FALSE);
  } else {
    Nlm_DoSetStatus (b, 0, TRUE, FALSE);
  }
  Nlm_DoAction (b);
  g = Nlm_GetParent (b);
  Nlm_DoAction (g);
  return TRUE;
}

static Nlm_Boolean Nlm_RadioCommand (Nlm_GraphiC b)

{
  Nlm_Boolean  _bool_;
  Nlm_GraphiC  g;

  _bool_ = Nlm_DoGetStatus (b, 0);
  if (! _bool_) {
    g = Nlm_GetParent (b);
    Nlm_ClearItemsInGroup (g, b, FALSE);
    Nlm_DoSetStatus (b, 0, TRUE, FALSE);
    Nlm_DoAction (b);
    Nlm_DoAction (g);
  }
  return TRUE;
}
#endif

#ifdef WIN_MOTIF
static void Nlm_PushCallback (Nlm_GraphiC b)

{
  Nlm_DoAction (b);
}

static void Nlm_DefaultCallback (Nlm_GraphiC b)

{
  Nlm_DoAction (b);
}

static void Nlm_CheckCallback (Nlm_GraphiC b)

{
  Nlm_Boolean      _bool_;
  Nlm_ControlTool  c;
  Nlm_GraphiC      g;

  c = Nlm_GetButtonHandle ((Nlm_ButtoN) b);
  _bool_ = Nlm_DoGetStatus (b, 0);
  if (! _bool_) {
    Nlm_DoSetStatus (b, 0, FALSE, FALSE);
  } else {
    Nlm_DoSetStatus (b, 0, TRUE, FALSE);
  }
  Nlm_DoAction (b);
  g = Nlm_GetParent (b);
  Nlm_DoAction (g);
}

static void Nlm_RadioCallback (Nlm_GraphiC b)

{
  Nlm_Boolean      _bool_;
  Nlm_ControlTool  c;
  Nlm_GraphiC      g;

  c = Nlm_GetButtonHandle ((Nlm_ButtoN) b);
  _bool_ = Nlm_DoGetStatus (b, 0);
  if (_bool_) {
    g = Nlm_GetParent (b);
    Nlm_ClearItemsInGroup (g, b, FALSE);
    Nlm_DoSetStatus (b, 0, TRUE, FALSE);
    Nlm_DoAction (b);
    Nlm_DoAction (g);
  } else {
    Nlm_DoSetStatus (b, 0, TRUE, FALSE);
  }
}
#endif

#ifdef WIN_MAC
static Nlm_Boolean Nlm_DefaultKey (Nlm_GraphiC b, Nlm_Char ch)

{
  Nlm_ControlTool  c;
  unsigned long    res;
  Nlm_Boolean      rsult;

  rsult = FALSE;
  if (ch == '\15' || ch == '\3') {
    c = Nlm_GetButtonHandle ((Nlm_ButtoN) b);
    HiliteControl (c, 1);
    Delay (8, &res);
    HiliteControl (c, 0);
    Nlm_DoSetStatus (b, 0, TRUE, FALSE);
    Nlm_DoAction (b);
    rsult = TRUE;
  }
  return rsult;
}

static void Nlm_DrawButton (Nlm_GraphiC b)

{
  Nlm_ControlTool  c;
  Nlm_RecT         r;

  if (Nlm_GetVisible (b) && Nlm_GetAllParentsVisible (b)) {
    Nlm_GetRect (b, &r);
    if (Nlm_RectInRgn (&r, Nlm_updateRgn)) {
      c = Nlm_GetButtonHandle ((Nlm_ButtoN) b);
      Draw1Control (c);
    }
  }
}

static void Nlm_DrawDefault (Nlm_GraphiC b)

{
  Nlm_ControlTool  c;
  Nlm_RecT         r;

  if (Nlm_HasAquaMenuLayout ()) {
    Nlm_DrawButton (b);
    return;
  }
  if (Nlm_GetVisible (b) && Nlm_GetAllParentsVisible (b)) {
    Nlm_GetRect (b, &r);
    Nlm_InsetRect (&r, -4, -4);
    if (Nlm_RectInRgn (&r, Nlm_updateRgn)) {
      PenSize (3, 3);
      Nlm_FrameRoundRect (&r, 16, 16);
      PenNormal ();
      c = Nlm_GetButtonHandle ((Nlm_ButtoN) b);
      Draw1Control (c);
    }
  }
}
#endif

static void Nlm_ShowButton (Nlm_GraphiC b, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_ControlTool  c;
  Nlm_WindoW       tempPort;

  if (setFlag) {
    Nlm_SetVisible (b, TRUE);
  }
  if (Nlm_GetVisible (b) && Nlm_AllParentsButWindowVisible (b)) {
    tempPort = Nlm_SavePortIfNeeded (b, savePort);
    c = Nlm_GetButtonHandle ((Nlm_ButtoN) b);
#ifdef WIN_MAC
    ShowControl (c);
    Nlm_DoDraw (b);
#endif
#ifdef WIN_MSWIN
    ShowWindow (c, SW_SHOW);
    UpdateWindow (c);
#endif
#ifdef WIN_MOTIF
    XtManageChild (c);
#endif
    Nlm_RestorePort (tempPort);
  }
}

static void Nlm_HideButton (Nlm_GraphiC b, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_ControlTool  c;
  Nlm_WindoW       tempPort;
#ifdef WIN_MAC
  Nlm_Int2         border;
  Nlm_RecT         r;
#endif

  if (setFlag) {
    Nlm_SetVisible (b, FALSE);
  }
  tempPort = Nlm_SavePortIfNeeded (b, savePort);
  c = Nlm_GetButtonHandle ((Nlm_ButtoN) b);
#ifdef WIN_MAC
  border = Nlm_GetButtonBorder ((Nlm_ButtoN) b);
  HideControl (c);
  if (Nlm_GetAllParentsVisible (b)) {
    Nlm_GetRect (b, &r);
    Nlm_InsetRect (&r, -1 - border, -1 - border);
    Nlm_EraseRect (&r);
    Nlm_ValidRect (&r);
  }
#endif
#ifdef WIN_MSWIN
  ShowWindow (c, SW_HIDE);
  UpdateWindow (c);
#endif
#ifdef WIN_MOTIF
  XtUnmanageChild (c);
#endif
  Nlm_RestorePort (tempPort);
}

static void Nlm_EnableButton (Nlm_GraphiC b, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_ControlTool  c;
  Nlm_WindoW       tempPort;

  if (setFlag) {
    Nlm_SetEnabled (b, TRUE);
  }
  if (Nlm_GetEnabled (b) && Nlm_GetAllParentsEnabled (b)) {
    tempPort = Nlm_SavePortIfNeeded (b, savePort);
    c = Nlm_GetButtonHandle ((Nlm_ButtoN) b);
#ifdef WIN_MAC
    HiliteControl (c, 0);
#endif
#ifdef WIN_MSWIN
    EnableWindow (c, TRUE);
#endif
#ifdef WIN_MOTIF
    XtVaSetValues (c, XmNsensitive, TRUE, NULL);
#endif
    Nlm_RestorePort (tempPort);
  }
}

static void Nlm_DisableButton (Nlm_GraphiC b, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_ControlTool  c;
  Nlm_WindoW       tempPort;

  if (setFlag) {
    Nlm_SetEnabled (b, FALSE);
  }
  tempPort = Nlm_SavePortIfNeeded (b, savePort);
  c = Nlm_GetButtonHandle ((Nlm_ButtoN) b);
#ifdef WIN_MAC
  HiliteControl (c, 255);
#endif
#ifdef WIN_MSWIN
  EnableWindow (c, FALSE);
#endif
#ifdef WIN_MOTIF
  XtVaSetValues (c, XmNsensitive, FALSE, NULL);
#endif
  Nlm_RestorePort (tempPort);
}

static void Nlm_RemoveButton (Nlm_GraphiC b, Nlm_Boolean savePort)

{
  Nlm_ControlTool  c;
  Nlm_WindoW       tempPort;

  tempPort = Nlm_SavePortIfNeeded (b, savePort);
  c = Nlm_GetButtonHandle ((Nlm_ButtoN) b);
#ifdef WIN_MAC
  DisposeControl (c);
#endif
#ifdef WIN_MSWIN
  RemoveProp (c, (LPSTR) "Nlm_VibrantProp");
  DestroyWindow (c);
#endif
#ifdef WIN_MOTIF
  XtDestroyWidget (c);
#endif
  Nlm_RemoveLink (b);
  recentButton = NULL;
  Nlm_RestorePort (tempPort);
}

static void Nlm_RemoveDefaultButton (Nlm_GraphiC b, Nlm_Boolean savePort)

{
  Nlm_ControlTool  c;
  Nlm_WindoW       tempPort;
#ifdef WIN_MOTIF
  Nlm_WindoW       w;
  Nlm_WindowTool   wptr;
#endif

  tempPort = Nlm_SavePortIfNeeded (b, savePort);
  c = Nlm_GetButtonHandle ((Nlm_ButtoN) b);
#ifdef WIN_MAC
  DisposeControl (c);
#endif
#ifdef WIN_MSWIN
  RemoveProp (c, (LPSTR) "Nlm_VibrantProp");
  DestroyWindow (c);
#endif
#ifdef WIN_MOTIF
  w = Nlm_ParentWindow (b);
  wptr = Nlm_ParentWindowPtr (b);
  if (Nlm_GetWindowDefaultButton (w) == (Nlm_ButtoN) b) {
    Nlm_SetWindowDefaultButton (w, NULL);
#ifndef LESSTIF_VERSION
    /* causes segfault using LessTif (0.89) under RH Linux 6 */
    XtVaSetValues (wptr, XmNdefaultButton, NULL, NULL);
#endif
    {{
      Widget shell = Nlm_ParentWindowShell((Nlm_GraphiC) b);
      Atom atom =XmInternAtom(Nlm_currentXDisplay, "WM_TAKE_FOCUS", TRUE);
      if ( atom )
        XmRemoveWMProtocolCallback(shell, atom,
                                   NlmHint_ResetDefButtonA,
                                   (XtPointer) b);
      XtRemoveEventHandler(shell, FocusChangeMask, FALSE,
                           NlmHint_ResetDefButtonE, (XtPointer)b);
    }}
  }
  XtDestroyWidget (c);
#endif
  Nlm_RemoveLink (b);
  recentButton = NULL;
  Nlm_RestorePort (tempPort);
}

static void Nlm_SetButtonTitle (Nlm_GraphiC b, Nlm_Int2 item,
                                Nlm_CharPtr title, Nlm_Boolean savePort)

{
  Nlm_ControlTool  c;
  Nlm_Char         temp [256];
  Nlm_WindoW       tempPort;
#ifdef WIN_MOTIF
  XmString         label;
#endif

  tempPort = Nlm_SavePortIfNeeded (b, savePort);
  c = Nlm_GetButtonHandle ((Nlm_ButtoN) b);
  Nlm_StringNCpy_0 (temp, title, sizeof (temp));
#ifdef WIN_MAC
  Nlm_CtoPstr (temp);
  SetControlTitle (c, (StringPtr) temp);
#endif
#ifdef WIN_MSWIN
  SetWindowText (c, temp);
#endif
#ifdef WIN_MOTIF
  label = XmStringCreateSimple (temp);
  XtVaSetValues (c, XmNlabelString, label, NULL);
  XmStringFree (label);
#endif
  Nlm_RestorePort (tempPort);
}

static void Nlm_GetButtonTitle (Nlm_GraphiC b, Nlm_Int2 item,
                                Nlm_CharPtr title, size_t maxsize)

{
  Nlm_ControlTool  c;
  Nlm_Char         temp [256];
#ifdef WIN_MOTIF
  XmString         label;
  char             *text;
#endif

  c = Nlm_GetButtonHandle ((Nlm_ButtoN) b);
#ifdef WIN_MAC
  GetControlTitle (c, (StringPtr) temp);
  Nlm_PtoCstr (temp);
#endif
#ifdef WIN_MSWIN
  GetWindowText (c, temp, sizeof (temp));
#endif
#ifdef WIN_MOTIF
  XtVaGetValues (c, XmNlabelString, &label, NULL);
  temp [0] = '\0';
  if (XmStringGetLtoR (label, XmSTRING_DEFAULT_CHARSET, &text)) {
    Nlm_StringNCpy_0 (temp, text, sizeof (temp));
    XtFree (text);
  }
#endif
  Nlm_StringNCpy_0 (title, temp, maxsize);
}

static void Nlm_SetButtonStatus (Nlm_GraphiC b, Nlm_Int2 item,
                                 Nlm_Boolean set, Nlm_Boolean savePort)

{
  Nlm_ControlTool  c;
  Nlm_WindoW       tempPort;
  Nlm_Int2         val;

  tempPort = Nlm_SavePortIfNeeded (b, savePort);
  c = Nlm_GetButtonHandle ((Nlm_ButtoN) b);
  if (set) {
    val = 1;
  } else {
    val = 0;
  }
#ifdef WIN_MAC
  SetControlValue (c, val);
#endif
#ifdef WIN_MSWIN
  Button_SetCheck (c, val);
#endif
#ifdef WIN_MOTIF
  XmToggleButtonSetState (c, (Boolean) set, FALSE);
#endif
  Nlm_RestorePort (tempPort);
}

static Nlm_Boolean Nlm_GetButtonStatus (Nlm_GraphiC b, Nlm_Int2 item)

{
  Nlm_ControlTool  c;

  c = Nlm_GetButtonHandle ((Nlm_ButtoN) b);
#ifdef WIN_MAC
  return (GetControlValue (c) != 0);
#endif
#ifdef WIN_MSWIN
  return (Nlm_Boolean) (Button_GetCheck (c) != 0);
#endif
#ifdef WIN_MOTIF
  return (Nlm_Boolean) (XmToggleButtonGetState (c) != FALSE);
#endif
}

#ifdef WIN_MAC
static void Nlm_InvalButton (Nlm_GraphiC b, Nlm_Int2 border)

{
  Nlm_RecT  r;

  if (Nlm_GetVisible (b) && Nlm_GetAllParentsVisible (b)) {
    Nlm_GetRect (b, &r);
    Nlm_InsetRect (&r, -1 - border, -1 - border);
    Nlm_InvalRect (&r);
  }
}
#endif

static void Nlm_SetButtonPosition (Nlm_GraphiC b, Nlm_RectPtr r, Nlm_Boolean savePort, Nlm_Boolean force)

{
  Nlm_ButtonData   bdata;
  Nlm_ControlTool  c;
  Nlm_RecT         oldRect;
  Nlm_WindoW       tempPort;
#ifdef WIN_MAC
  Nlm_Int2         border;
#endif

  if (r == NULL)  return;

  if ( !Nlm_GetRealized( b ) )
    {
      Nlm_SetRect(b, r);
      return;
    }

  Nlm_DoGetPosition (b, &oldRect);
  if (!force  &&  Nlm_EqualRect(r, &oldRect))  return;

  tempPort = Nlm_SavePortIfNeeded (b, savePort);
  Nlm_GetButtonData ((Nlm_ButtoN) b, &bdata);
  c = bdata.handle;
#ifdef WIN_MAC
  border = bdata.border;
  Nlm_InvalButton (b, border);
  MoveControl (c, r->left, r->top);
  SizeControl (c, r->right - r->left - bdata.shrinkX, r->bottom - r->top - bdata.shrinkY);
  Nlm_SetRect (b, r);
  Nlm_InvalButton (b, border);
#endif
#ifdef WIN_MSWIN
  MoveWindow(c, r->left, r->top, r->right - r->left, r->bottom - r->top, TRUE);
  Nlm_SetRect (b, r);
  UpdateWindow (c);
#endif
#ifdef WIN_MOTIF
  {{
    Nlm_Int2 offset = bdata.offset;
    XtVaSetValues (c,
                   XmNx, (Position) (r->left - offset),
                   XmNy, (Position) (r->top  - offset),
                   XmNwidth,  (Dimension) (r->right  - r->left),
                   XmNheight, (Dimension) (r->bottom - r->top), 
                   NULL);
  }}
  Nlm_SetRect (b, r);
#endif

  Nlm_RestorePort (tempPort);
}

static void Nlm_GetButtonPosition (Nlm_GraphiC b, Nlm_RectPtr r)

{
  if (r != NULL) {
    Nlm_GetRect (b, r);
  }
}

static Nlm_GraphiC Nlm_DefaultGainFocus (Nlm_GraphiC b, Nlm_Char ch, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  return NULL;
#endif
#ifdef WIN_MSWIN
  Nlm_GraphiC   rsult;

  rsult = NULL;
  if (ch == '\n' || ch == '\r') {
    if (Nlm_GetVisible (b) && Nlm_GetAllParentsVisible (b) &&
        Nlm_GetEnabled (b) && Nlm_GetAllParentsEnabled (b)) {
      Nlm_DoAction (b);
      rsult = b;
    }
  }
  return rsult;
#endif
#ifdef WIN_MOTIF
  return NULL;
#endif
}


#ifdef WIN_MSWIN
static Nlm_Boolean Nlm_IsDefaultButton (Nlm_ButtoN b)
{
  Nlm_ButtonData  bdata;
  Nlm_GetButtonData (b, &bdata);
  return bdata.defaultBtn;
}

/* Message cracker functions */

static void MyCls_OnChar (HWND hwnd, UINT ch, int cRepeat)

{
  Nlm_ButtoN  b;

  b = (Nlm_ButtoN) GetProp (hwnd, (LPSTR) "Nlm_VibrantProp");
  handlechar = FALSE;
  if (ch == '\t') {
    Nlm_DoSendFocus ((Nlm_GraphiC) b, (Nlm_Char) ch);
  } else if (ch == '\n' || ch == '\r') {
    if (Nlm_IsDefaultButton (b)) {
      Nlm_DoGainFocus ((Nlm_GraphiC) b, (Nlm_Char) ch, FALSE);
    } else {
      Nlm_DoSendFocus ((Nlm_GraphiC) b, (Nlm_Char) ch);
    }
  } else {
    handlechar = TRUE;
  }
}

LRESULT CALLBACK EXPORT ButtonProc (HWND hwnd, UINT message,
                                    WPARAM wParam, LPARAM lParam)

{
  Nlm_ButtoN  b;
  BOOL        call_win_proc = TRUE;
  LRESULT     rsult = 0;
  HDC         tempHDC;
  HWND        tempHWnd;

  if (Nlm_VibrantDisabled ()) {
    return CallWindowProc (lpfnOldButtonProc, hwnd, message, wParam, lParam);
  }

  tempHWnd = Nlm_currentHWnd;
  tempHDC = Nlm_currentHDC;
  b = (Nlm_ButtoN) GetProp (hwnd, (LPSTR) "Nlm_VibrantProp");
  Nlm_theWindow = Nlm_GetParentWindow ((Nlm_GraphiC) b);
  Nlm_currentHWnd = GetParent (hwnd);
  Nlm_currentHDC = Nlm_ParentWindowPort ((Nlm_GraphiC) b);
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
      call_win_proc = !Nlm_ProcessKeydown((Nlm_GraphiC)b, wParam,
                               VERT_PAGE|VERT_ARROW|HORIZ_PAGE|HORIZ_ARROW);
      break;
    case WM_CHAR:
      HANDLE_WM_CHAR (hwnd, wParam, lParam, MyCls_OnChar);
      call_win_proc = handlechar;
      break;
  }

  if ( call_win_proc )
    rsult = CallWindowProc(lpfnOldButtonProc, hwnd, message, wParam, lParam);

  Nlm_currentHWnd = tempHWnd;
  Nlm_currentHDC  = tempHDC;
  Nlm_currentWindowTool = tempHWnd;
  return rsult;
}


static void Nlm_PrepareTitleMsWin(Nlm_CharPtr temp, Nlm_CharPtr title,
				  size_t siztemp)
{
    /* duplicate existing '&' */
    Nlm_Uint4 src_pos = 0;
    Nlm_Uint4 dest_pos = 0;

    for(src_pos = 0, dest_pos = 0;
	title[src_pos] != '\0' && dest_pos+1 < siztemp;
	++src_pos)
    {
	if(title[src_pos] == '&')
	{
	    if(dest_pos + 2 < siztemp)
	    {
		temp[dest_pos++] = '&';
		temp[dest_pos++] = title[src_pos];
	    }
	    else
		break;
	}
	else
	    temp[dest_pos++] = title[src_pos];
    }

    temp[dest_pos] = '\0';
}
#endif

#ifdef WIN_MOTIF
static void Nlm_ButtonCallback (Widget w, XtPointer client_data, XtPointer call_data)

{
  Nlm_GraphiC  b;

  b = (Nlm_GraphiC) client_data;
  Nlm_DoCallback (b);
}

extern void Nlm_MapDefaultButton (Nlm_WindoW w, Nlm_ButtoN b)

{
  Nlm_ControlTool  c;
  Nlm_WindowTool   wptr;

  if (w != NULL && b != NULL) {
    wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) b);
    c = Nlm_GetButtonHandle (b);
    Nlm_SetWindowDefaultButton (w, b);
    XtVaSetValues (wptr, XmNdefaultButton, c, NULL);
  }
}
#endif


#define PUSH_STYLE 1
#define DEFAULT_STYLE 2
#define CHECK_STYLE 3
#define RADIO_STYLE 4

#ifdef WIN_MOTIF
static void NlmHint_ResetDefButton(Nlm_ButtoN b)
{
  Nlm_WindowTool  wptr  = Nlm_ParentWindowPtr((Nlm_GraphiC) b);
  Nlm_ControlTool bptr  = Nlm_GetButtonHandle( b );

  XtVaSetValues(wptr, XmNdefaultButton, NULL, NULL);
  XtVaSetValues(wptr, XmNdefaultButton, bptr, NULL);
}

static void NlmHint_ResetDefButtonE(Widget w, XtPointer client_data,
                                   XEvent *event,
                                   Boolean *continue_to_dispatch )
{
  if (event->type == FocusIn)
    NlmHint_ResetDefButton( (Nlm_ButtoN)client_data );
}

static void NlmHint_ResetDefButtonA(Widget w, XtPointer client_data,
                                    XtPointer call_data)
{
  NlmHint_ResetDefButton( (Nlm_ButtoN)client_data );
}
#endif


static void Nlm_NewButton (Nlm_ButtoN b, Nlm_CharPtr title,
                           Nlm_Int2 type, Nlm_Int2 shrinkX,
                           Nlm_Int2 shrinkY, Nlm_BtnActnProc actn)
{
  Nlm_Int2         border;
  Nlm_ControlTool  c;
  Nlm_Boolean      dflt;
  Nlm_Int2         offset;
  Nlm_RecT         r;
  Nlm_Char         temp [256];
  Nlm_WindowTool   wptr;
#ifdef WIN_MAC
  Nlm_Int2         procID;
  Nlm_RectTool     rtool;
#endif
#ifdef WIN_MSWIN
  Nlm_Uint4        style;
  Nlm_FntPtr fntptr;
#endif
#ifdef WIN_MOTIF
  String           call;
  XmString         label;
  Cardinal         n;
  Arg              wargs [15];
#endif

  Nlm_GetRect ((Nlm_GraphiC) b, &r);
  wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) b);
#ifdef WIN_MSWIN
  Nlm_PrepareTitleMsWin (temp, title, sizeof (temp));
#else
  Nlm_StringNCpy_0 (temp, title, sizeof (temp));
#endif
  c = (Nlm_ControlTool) 0;
  border = 0;
  offset = 0;
  dflt = (Nlm_Boolean) (type == DEFAULT_STYLE);

#ifdef WIN_MAC
  Nlm_CtoPstr (temp);
  switch (type) {
    case PUSH_STYLE:
      procID = 0;
      r.right -= shrinkX;
      r.bottom -= shrinkY;
      break;
    case DEFAULT_STYLE:
      procID = 0;
      if (! Nlm_HasAquaMenuLayout ()) {
        border = 5;
      }
      r.right -= shrinkX;
      r.bottom -= shrinkY;
      break;
    case CHECK_STYLE:
      procID = 1;
      break;
    case RADIO_STYLE:
      procID = 2;
      break;
    default:
      procID = 0;
      break;
  }
  Nlm_RecTToRectTool (&r, &rtool);
  c = NewControl (wptr, &rtool, (StringPtr) temp, FALSE, 0, 0, 1, procID, 0);
  Nlm_LoadButtonData (b, c, border, offset, shrinkX, shrinkY, dflt);
#endif

#ifdef WIN_MSWIN
  switch (type) {
    case PUSH_STYLE:
      style = BS_PUSHBUTTON;
      break;
    case DEFAULT_STYLE:
      style = BS_DEFPUSHBUTTON;
      break;
    case CHECK_STYLE:
      style = BS_CHECKBOX;
      break;
    case RADIO_STYLE:
      style = BS_RADIOBUTTON;
      break;
    default:
      style = BS_PUSHBUTTON;
      break;
  }
  c = CreateWindow ("Button", temp, WS_CHILD | style,
                    r.left, r.top, r.right - r.left,
                    r.bottom - r.top, wptr, 0,
                    Nlm_currentHInst, NULL);
  if ( c ) {
    SetProp (c, (LPSTR) "Nlm_VibrantProp", (Nlm_HandleTool) b);
  }
  Nlm_LoadButtonData (b, c, border, offset, 0, 0, dflt);
  if (lpfnNewButtonProc == NULL) {
    lpfnNewButtonProc = (WNDPROC) MakeProcInstance ((FARPROC) ButtonProc, Nlm_currentHInst);
  }
  if (lpfnOldButtonProc == NULL) {
    lpfnOldButtonProc = (WNDPROC) GetWindowLong (c, GWL_WNDPROC);
  } else if (lpfnOldButtonProc != (WNDPROC) GetWindowLong (c, GWL_WNDPROC)) {
    Nlm_Message (MSG_ERROR, "ButtonProc subclass error");
  }
  SetWindowLong (c, GWL_WNDPROC, (LONG) lpfnNewButtonProc);
  fntptr = (Nlm_FntPtr) Nlm_HandLock (Nlm_systemFont);
  SetWindowFont(c, fntptr->handle, FALSE);
  Nlm_HandUnlock(Nlm_systemFont);
#endif

#ifdef WIN_MOTIF
  n = 0;
  label = XmStringCreateSimple (temp);
  XtSetArg (wargs [n], XmNlabelString, label); n++;

  if (r.right > r.left)
    {
      XtSetArg (wargs [n], XmNwidth, (Dimension) (r.right - r.left));
      n++;
    }

  if (r.bottom > r.top)
    {
      XtSetArg (wargs [n], XmNheight, (Dimension) (r.bottom - r.top));
      n++;
    }

  if (type == DEFAULT_STYLE  || type == PUSH_STYLE)
    {
      r.left -= 2;
      r.top  -= 4;
    }
  XtSetArg (wargs [n], XmNx, (Position) r.left - offset);  n++;
  XtSetArg (wargs [n], XmNy, (Position) r.top  - offset);  n++;
  XtSetArg (wargs [n], XmNborderWidth, (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNhighlightThickness, (Dimension) 0); n++;
  switch (type) {
    case DEFAULT_STYLE:
      Nlm_SetWindowDefaultButton(Nlm_ParentWindow((Nlm_GraphiC) b), b);
    case PUSH_STYLE:
      XtSetArg (wargs [n], XmNrecomputeSize, FALSE); n++;
      c = XmCreatePushButton (wptr, (String) "", wargs, n);
      XtVaSetValues (wptr, XmNdefaultButton, c, NULL);
      call = XmNactivateCallback;
      break;
    case CHECK_STYLE:
      XtSetArg (wargs [n], XmNindicatorType, XmN_OF_MANY); n++;
      XtSetArg (wargs [n], XmNalignment, XmALIGNMENT_BEGINNING); n++;
      c = XmCreateToggleButton (wptr, (String) "", wargs, n);
      call = XmNvalueChangedCallback;
      break;
    case RADIO_STYLE:
      XtSetArg (wargs [n], XmNindicatorType, XmONE_OF_MANY); n++;
      XtSetArg (wargs [n], XmNalignment, XmALIGNMENT_BEGINNING); n++;
      c = XmCreateToggleButton (wptr, (String) "", wargs, n);
      call = XmNvalueChangedCallback;
      break;
    default:
      break;
  }
  XmStringFree (label);
  XtAddCallback (c, call, Nlm_ButtonCallback, (XtPointer) b);
  Nlm_LoadButtonData (b, c, border, offset, 0, 0, dflt);
  Nlm_OverrideStdTranslations((Nlm_GraphiC)b, c,
                              VERT_PAGE|VERT_ARROW|HORIZ_PAGE|HORIZ_ARROW);
  XtManageChild( c );
  XtVaGetValues(c, XmNwidth, &r.right, XmNheight, &r.bottom, NULL);
  r.right  += r.left;
  r.bottom += r.top;
  Nlm_SetRect((Nlm_GraphiC)b, &r);
  switch ( type )
    {
    case DEFAULT_STYLE:
      {
        Atom atom = XmInternAtom(Nlm_currentXDisplay, "WM_TAKE_FOCUS",
                                 TRUE);
        Widget shell = Nlm_ParentWindowShell((Nlm_GraphiC) b);
        if ( atom )
          XmAddWMProtocolCallback(shell, atom,
                                  NlmHint_ResetDefButtonA, (XtPointer) b);
        XtAddEventHandler(shell,
                          FocusChangeMask, FALSE,
                          NlmHint_ResetDefButtonE, (XtPointer) b);
      }
    case PUSH_STYLE:
      XtVaSetValues(wptr, XmNdefaultButton, NULL, NULL);
      break;
    }
#endif

  Nlm_LoadAction ((Nlm_GraphiC) b, (Nlm_ActnProc) actn);
}


static Nlm_ButtoN Nlm_CommonButton (Nlm_GrouP prnt, Nlm_CharPtr title,
                                    Nlm_Int2 type, Nlm_BtnActnProc actn,
                                    Nlm_GphPrcsPtr classPtr)

{
  Nlm_ButtoN  b = NULL;
  Nlm_Int2    hgt;
  Nlm_PoinT   npt;
  Nlm_RecT    r;
  Nlm_Int2    shrinkX = 0;
  Nlm_Int2    shrinkY = 0;
  Nlm_WindoW  tempPort;
  Nlm_Int2    wid;

  if (prnt == NULL)  return NULL;

  tempPort = Nlm_SavePort ((Nlm_GraphiC) prnt);
  Nlm_GetNextPosition ((Nlm_GraphiC) prnt, &npt);
  Nlm_SelectFont (Nlm_systemFont);
  wid = Nlm_StringWidth (title);

#ifdef WIN_MAC
  hgt = Nlm_stdLineHeight;
  wid += 20;
  switch (type) {
    case PUSH_STYLE:
    case DEFAULT_STYLE:
      hgt += 3;
      break;
  }
  if (Nlm_HasAquaMenuLayout ()) {
    switch (type) {
      case PUSH_STYLE:
        wid += 10;
        hgt = 26;
        shrinkX = 6;
        shrinkY = 6;
        break;
      case DEFAULT_STYLE:
        wid += 10;
        hgt = 26;
        shrinkX = 6;
        shrinkY = 6;
        break;
      case CHECK_STYLE:
        wid += 4;
        break;
      case RADIO_STYLE:
        wid += 4;
        break;
      default:
        wid += 0;
        break;
    }
  }
#endif

#ifdef WIN_MSWIN
  switch (type) {
    case PUSH_STYLE:
      hgt = Nlm_stdLineHeight+8;
      wid += 20;
      break;
    case DEFAULT_STYLE:
      hgt = Nlm_stdLineHeight+8;
      wid += 20;
      break;
    case CHECK_STYLE:
      hgt = Nlm_stdLineHeight+3;
      wid += 20;
      break;
    case RADIO_STYLE:
      hgt = Nlm_stdLineHeight+3;
      wid += 20;
      break;
    default:
      hgt = Nlm_stdLineHeight;
      wid += 20;
      break;
  }
#endif

#ifdef WIN_MOTIF
  hgt = 0;
  wid = 0;
#endif

  Nlm_LoadRect(&r, npt.x, npt.y, (Nlm_Int2)(npt.x+wid), (Nlm_Int2)(npt.y+hgt));
  b = (Nlm_ButtoN) Nlm_CreateLink ((Nlm_GraphiC) prnt, &r, sizeof (Nlm_ButtonRec), classPtr);
  if (b != NULL) {
    Nlm_NewButton (b, title, type, shrinkX, shrinkY, actn);
    Nlm_GetRect ((Nlm_GraphiC) b, &r);
    Nlm_DoAdjustPrnt ((Nlm_GraphiC) b, &r, TRUE, FALSE);
    Nlm_DoShow ((Nlm_GraphiC) b, TRUE, FALSE);
  }
  Nlm_RestorePort (tempPort);

  return b;
}


extern Nlm_ButtoN Nlm_PushButton (Nlm_GrouP prnt, Nlm_CharPtr title,
                                  Nlm_BtnActnProc actn)

{
  Nlm_ButtoN  b;

  b = Nlm_CommonButton (prnt, title, PUSH_STYLE, actn, pushProcs);
  return b;
}

extern Nlm_ButtoN Nlm_DefaultButton (Nlm_GrouP prnt, Nlm_CharPtr title,
                                     Nlm_BtnActnProc actn)

{
  Nlm_ButtoN  b;

  b = Nlm_CommonButton (prnt, title, DEFAULT_STYLE, actn, defaultProcs);
  return b;
}

extern Nlm_ButtoN Nlm_CheckBox (Nlm_GrouP prnt, Nlm_CharPtr title,
                                Nlm_BtnActnProc actn)

{
  Nlm_ButtoN  b;

  b = Nlm_CommonButton (prnt, title, CHECK_STYLE, actn, checkProcs);
  return b;
}

extern Nlm_ButtoN Nlm_RadioButton (Nlm_GrouP prnt, Nlm_CharPtr title)

{
  Nlm_ButtoN  b;

  b = Nlm_CommonButton (prnt, title, RADIO_STYLE, NULL, radioProcs);
  return b;
}

extern void Nlm_FreeButtons (void)

{
  gphprcsptr = (Nlm_GphPrcsPtr) Nlm_MemFree (gphprcsptr);
}

extern void Nlm_InitButtons (void)

{
  gphprcsptr = (Nlm_GphPrcsPtr) Nlm_MemNew (sizeof (Nlm_GphPrcs) * 4);

  pushProcs = &(gphprcsptr [0]);
#ifdef WIN_MAC
  pushProcs->click = Nlm_PushClick;
  pushProcs->draw = Nlm_DrawButton;
#endif
#ifdef WIN_MSWIN
  pushProcs->command = Nlm_PushCommand;
#endif
#ifdef WIN_MOTIF
  pushProcs->callback = Nlm_PushCallback;
#endif
  pushProcs->show = Nlm_ShowButton;
  pushProcs->hide = Nlm_HideButton;
  pushProcs->enable = Nlm_EnableButton;
  pushProcs->disable = Nlm_DisableButton;
  pushProcs->remove = Nlm_RemoveButton;
  pushProcs->setTitle = Nlm_SetButtonTitle;
  pushProcs->getTitle = Nlm_GetButtonTitle;
  pushProcs->setPosition = Nlm_SetButtonPosition;
  pushProcs->getPosition = Nlm_GetButtonPosition;

  defaultProcs = &(gphprcsptr [1]);
#ifdef WIN_MAC
  defaultProcs->click = Nlm_DefaultClick;
  defaultProcs->key = Nlm_DefaultKey;
  defaultProcs->draw = Nlm_DrawDefault;
#endif
#ifdef WIN_MSWIN
  defaultProcs->command = Nlm_DefaultCommand;
#endif
#ifdef WIN_MOTIF
  defaultProcs->callback = Nlm_DefaultCallback;
#endif
  defaultProcs->show = Nlm_ShowButton;
  defaultProcs->hide = Nlm_HideButton;
  defaultProcs->enable = Nlm_EnableButton;
  defaultProcs->disable = Nlm_DisableButton;
  defaultProcs->remove = Nlm_RemoveDefaultButton;
  defaultProcs->setTitle = Nlm_SetButtonTitle;
  defaultProcs->getTitle = Nlm_GetButtonTitle;
  defaultProcs->setPosition = Nlm_SetButtonPosition;
  defaultProcs->getPosition = Nlm_GetButtonPosition;
  defaultProcs->gainFocus = Nlm_DefaultGainFocus;

  checkProcs = &(gphprcsptr [2]);
#ifdef WIN_MAC
  checkProcs->click = Nlm_CheckClick;
  checkProcs->draw = Nlm_DrawButton;
#endif
#ifdef WIN_MSWIN
  checkProcs->command = Nlm_CheckCommand;
#endif
#ifdef WIN_MOTIF
  checkProcs->callback = Nlm_CheckCallback;
#endif
  checkProcs->show = Nlm_ShowButton;
  checkProcs->hide = Nlm_HideButton;
  checkProcs->enable = Nlm_EnableButton;
  checkProcs->disable = Nlm_DisableButton;
  checkProcs->remove = Nlm_RemoveButton;
  checkProcs->setTitle = Nlm_SetButtonTitle;
  checkProcs->getTitle = Nlm_GetButtonTitle;
  checkProcs->setStatus = Nlm_SetButtonStatus;
  checkProcs->getStatus = Nlm_GetButtonStatus;
  checkProcs->setPosition = Nlm_SetButtonPosition;
  checkProcs->getPosition = Nlm_GetButtonPosition;

  radioProcs = &(gphprcsptr [3]);
#ifdef WIN_MAC
  radioProcs->click = Nlm_RadioClick;
  radioProcs->draw = Nlm_DrawButton;
#endif
#ifdef WIN_MSWIN
  radioProcs->command = Nlm_RadioCommand;
#endif
#ifdef WIN_MOTIF
  radioProcs->callback = Nlm_RadioCallback;
#endif
  radioProcs->show = Nlm_ShowButton;
  radioProcs->hide = Nlm_HideButton;
  radioProcs->enable = Nlm_EnableButton;
  radioProcs->disable = Nlm_DisableButton;
  radioProcs->remove = Nlm_RemoveButton;
  radioProcs->setTitle = Nlm_SetButtonTitle;
  radioProcs->getTitle = Nlm_GetButtonTitle;
  radioProcs->setStatus = Nlm_SetButtonStatus;
  radioProcs->getStatus = Nlm_GetButtonStatus;
  radioProcs->setPosition = Nlm_SetButtonPosition;
  radioProcs->getPosition = Nlm_GetButtonPosition;
}


extern void Nlm_SetButtonDefault (Nlm_ButtoN b, Nlm_Boolean dflt);
extern void Nlm_SetButtonDefault (Nlm_ButtoN b, Nlm_Boolean dflt)
{
  if (b != NULL)
    {
      Nlm_BtnPtr  bp = (Nlm_BtnPtr) Nlm_HandLock (b);
      bp->button.defaultBtn = dflt;
      Nlm_HandUnlock (b);
    }
}

