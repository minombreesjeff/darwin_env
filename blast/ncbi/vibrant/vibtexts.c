/*   vibtexts.c
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
* File Name:  vibtexts.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   7/1/91
*
* $Revision: 6.28 $
*
* File Description: 
*       Vibrant edit text functions
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: vibtexts.c,v $
* Revision 6.28  2004/07/19 13:36:43  bollin
* replaced obsolete XmFontListCreate function to get rid of run-time warnings
*
* Revision 6.27  2004/06/24 15:34:48  bollin
* added lines from Yoon Choi to make ctrl-A select all text in TextMode in
* Sequin
*
* Revision 6.26  2004/03/17 16:09:14  sinyakov
* WIN_MSWIN: fixed text box activation and text selection logic,
* do not reset currentText when text window loses focus (to match Motif version),
* removed scroll text size limit,
* in Nlm_SelectAText(): scroll selection into view for scroll text,
* in Nlm_TextGainFocus(), when focus changes to a single-line edit box,
* do not highlight all text, instead just set the cursor pos to beginning,
* in MyCls_OnChar(), multiline edit boxes accept tabs
* as input characters and not as focus change commands.
*
* Revision 6.25  2004/02/23 16:36:52  sinyakov
* Use Int4 instead of Int2 for cursor position and text selection in text boxes
*
* Revision 6.24  2004/02/17 16:01:15  bollin
* implemented Nlm_SetScrollTextOffset for Motif
*
* Revision 6.23  2004/02/12 20:29:09  kans
* put ifdef WIN_MOTIF wrapper around Nlm_KeepCrNlTextFieldCallback
*
* Revision 6.22  2004/02/12 20:05:10  bazhin
* Added callback function "Nlm_SetKeepCrNlTextFieldCallback(Nlm_TexT t)",
* which allows to copy-paste texts from multiple columns to single string.
*
* Revision 6.21  2004/02/09 17:46:27  bollin
* added fix from Yoon Choi for copy-paste from Windows clipboard into Unix application
*
* Revision 6.20  2004/02/06 19:27:33  bollin
* Use PostMessage to communicate with scrolling function on TextViewer window
*
* Revision 6.19  2004/02/05 16:25:05  kans
* revert to Nlm_GetTextVScrollBar and Nlm_GetTextHScrollBar being static, implement setOffset and getOffset for scroll text, since Windows scroll text has its own scroll bar
*
* Revision 6.18  2004/02/04 15:21:03  kans
* make GetTextVScrollBar and GetTextHScrollBar extern, to add search function to general text viewer
*
* Revision 6.17  2003/11/17 17:03:30  kans
* changed C++ style comments to C comments
*
* Revision 6.16  2003/11/03 21:51:34  sinyakov
* WIN_MSWIN: bugfix: strlen(title) moved out of loop condition
*
* Revision 6.15  2003/10/29 19:10:08  bazhin
* Added function Nlm_SetTextColor(Nlm_TexT t, Nlm_Uint4 r, Nlm_Uint4 g,
*                                 Nlm_Uint4 b).
*
* Revision 6.14  2003/07/30 13:51:42  johnson
* MSWIN: multi-line text boxes now respond to 'enter' key
*
* Revision 6.13  2003/03/28 21:27:18  rsmith
* on Mac OS took out ClearCurrentScrap after Cut/Copy. It only erased whatever was copied.
*
* Revision 6.12  2003/01/07 15:08:11  shomrat
* Change condition in for loop in Nlm_SetScrollText
*
* Revision 6.11  2002/12/04 22:35:11  johnson
* WIN_MSWIN: replace UNIX '\n' with DOS "\r\n" in Nlm_SetScrollText
*
* Revision 6.10  2001/09/10 17:58:28  bazhin
* Removed odd lines from SetTextCursorBlinkRate() function.
*
* Revision 6.9  2001/09/10 17:34:10  bazhin
* Added function Nlm_SetTextCursorBlinkRate(Nlm_TexT t, Nlm_Int2 msec).
*
* Revision 6.8  2001/05/14 20:29:04  juran
* Redesign Mac clipboard support.
*
* Revision 6.7  2000/05/02 22:02:15  vakatov
* Nlm_TextCallback():  get rid of an extra condition
*
* Revision 6.6  2000/03/31 19:20:03  thiessen
* fix recursion bug
*
* Revision 6.5  2000/02/07 20:17:36  lewisg
* minor bug fixes, use gui font for win32
*
* Revision 6.4  1998/07/02 18:24:35  vakatov
* Cleaned the code & made it pass through the C++ compilation
*
* Revision 6.3  1997/12/19 18:01:45  vakatov
* [X11,MSWIN]  When pasting into a single-line text field, replace all
* non-printable characters by spaces
*
* Revision 6.2  1997/11/26 21:30:42  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.1  1997/10/18 23:30:16  kans
* implemented Nlm_GetTextCursorPos (DV)
*
* Revision 6.0  1997/08/25 18:57:44  madden
* Revision changed to 6.0
*
* Revision 5.22  1997/08/07 16:46:39  vakatov
* [WIN_MOTIF] Nlm_PasswordCallback() -- process <DEL> at the first text pos
*
* Revision 5.21  1997/08/07 13:38:32  kans
* password text does action callback on delete (Mac)
*
* Revision 5.20  1997/08/05 16:42:26  kans
* allowTextCallback not declared in Mac, so ifdefs changed in Nlm_SetTextCursorPos
*
* Revision 5.19  1997/08/04 14:15:14  vakatov
* Added Nlm_SetTextCursorPos() function
*
* Revision 5.18  1997/07/21 18:49:09  vakatov
* [WIN_MAC] SetPasswordText() -- moved "r" and "GetRect()"(sorry, missed
* that!) back to the if(Visible... scope
*
* Revision 5.17  1997/07/21 18:39:42  kans
* SetPasswordText r in Mac version at top scope
*
* Revision 5.16  1997/07/21 18:24:36  vakatov
* SetPasswordText() -- show all symbols as '*'
*
* Revision 5.15  1997/07/17 15:31:47  vakatov
* [WIN_MSWIN]  Emulate non-editable text(crack WM_CHAR event in TextProc
* as the WinSDK's Edit_SetReadOnly lead to invalidation bug for ScrollText)
*
* Revision 5.14  1997/07/16 13:47:20  kans
* non-editable scroll text now scrolls on Mac
*
* Revision 5.12  1997/07/13 23:03:05  kans
* ScrollText height is in stdLineHeight units
*
* Revision 5.11  1997/07/08 13:42:06  kans
* ttool to rtool fix in Nlm_DrawScrollText/DCLAP
*
* Revision 5.10  1997/06/24 21:30:37  kans
* implemented SetTextEditable on Mac
*
* Revision 5.9  1997/06/24 19:10:00  vakatov
* Nlm_ScrollText() -- calculate the text box limits using the systemFont
* (not the current font!) dimensions
*
* Revision 5.8  1997/06/23 21:21:05  vakatov
* Added Nlm_SetTextEditable() function to allow/prohibit text editing
* [WIN_MOTIF] Made text widgets to fit the specified number of lines
*
* Revision 5.7  1997/04/25 16:14:06  vakatov
* [WIN_MOTIF,WIN_MSWIN] Catch and render(DoSendFocus) navigation key events
* Extensive code cleaning and type casting
*
 * Revision 5.6  1997/02/12  17:47:28  vakatov
 * [WIN_MOTIF]  Do not reset selection when activating a window containing
 * a text or when the text is empty
 *
 * Revision 5.5  1997/01/29  17:53:59  kans
 * minor changes due to StringNCpy_0 change
 *
 * Revision 5.4  1997/01/29  16:41:22  kans
 * using StringNCpy_0
 *
 * Revision 5.3  1996/07/23  21:07:10  epstein
 * Vakatov/Epstein avoid trying to clear an area on an unrealized window
 *
 * Revision 5.2  1996/06/18  15:44:56  vakatov
 * Do not assign last (maxsize-1) symbol of output buffer
 * in Nlm_GetScrollText() and Nlm_GetDialogText() to '\0'.
 *
 * Revision 5.1  1996/06/17  19:28:17  vakatov
 * [WIN_MOTIF]  Positioning of scrolled text postponed until its realization
 *
 * Revision 4.9  1996/05/08  20:42:52  vakatov
 * [WIN_MOTIF]  Nlm_PasswordCallback() function -- rewritten to avoid stack
 * corruption on the case of too long(>30 symbols) user input and
 * to make the password text field more convenient to edit;
 * [ALL]  Nlm_GetPasswordLength() function added (other than
 * Nlm_TextLength());  a lot of other tiny changes and corrections
 *
 * Revision 4.8  1996/05/08  13:43:47  vakatov
 * [WIN_MOTIF]  Modified Nlm_SetPasswordText() to reset the password
 *
 * Revision 4.7  1996/05/03  17:02:08  kans
 * added a cast to SelectText call
 *
 * Revision 4.6  1996/03/02  22:36:38  kans
 * reduction of X traffic (DV)
 *
 * Revision 4.5  1996/02/13  17:24:07  kans
 * accelerated set position prior to realization (Denis Vakatov)
 *
 * Revision 4.4  1995/11/27  16:04:00  kans
 * ScrollText now handles killfocus and setfocus messages (VL)
 *
 * Revision 4.3  1995/11/27  15:13:41  kans
 * allow select and deselect callbacks for DialogText and ScrollText (VL)
 *
 * Revision 4.2  1995/11/08  23:30:31  kans
 * removed edit block fields, which belong in the application
 *
 * Revision 4.1  1995/10/10  15:51:53  kans
 * fully implemented scroll text resizing
 *
 * Revision 2.52  1995/07/14  17:48:26  kans
 * forces dialog, hidden text to use systemFont (AS)
 *
 * Revision 2.51  1995/05/20  18:27:38  kans
 * corrected accidental prevention of passing returns to other objects
 *
*
* ==========================================================================
*/

#include <vibtypes.h>
#include <vibprocs.h>
#include <vibincld.h>

#ifdef WIN_MAC
#define Nlm_TextTool TEHandle
#endif

#ifdef WIN_MSWIN
#define Nlm_TextTool HWND
#endif

#ifdef WIN_MOTIF
#define Nlm_TextTool Widget
#endif

#define HSCROLL_POSITIONS 100

typedef  struct  Nlm_textdata {
  Nlm_TextTool     handle;
  Nlm_BaR          vScrollBar;
  Nlm_BaR          hScrollBar;
  Nlm_Boolean      wrap;
  Nlm_FonT         font;
  Nlm_Int2         texthght;
  Nlm_Boolean      active;
  Nlm_Boolean      changed;
  Nlm_Boolean      hidden;
  Nlm_Boolean      special;
  Nlm_Int2         visLines;
  Nlm_TxtActnProc  select;
  Nlm_TxtActnProc  deselect;
  Nlm_TxtActnProc  tabnotify;
  Nlm_TxtActnProc  returnnotify;
  Nlm_Boolean      editable;
} Nlm_TextData;

typedef  struct  Nlm_textrec {
  Nlm_GraphicRec  graphicR;
  Nlm_TextData    text;
} Nlm_TextRec, PNTR Nlm_TxtPtr;

#define MAX_PASSWORD 32
typedef  struct  Nlm_passwdrec {
  Nlm_TextRec  textR;
  Nlm_Char     password [MAX_PASSWORD];
} Nlm_PasswdRec, PNTR Nlm_PwdPtr;

Nlm_Boolean Nlm_textScrapFull = FALSE;

static Nlm_GphPrcsPtr  gphprcsptr = NULL;

static Nlm_GphPrcsPtr  dialogTextProcs;
static Nlm_GphPrcsPtr  hiddenTextProcs;
static Nlm_GphPrcsPtr  specialTextProcs;
static Nlm_GphPrcsPtr  passwordTextProcs;
static Nlm_GphPrcsPtr  scrollTextProcs;

static Nlm_TexT        recentText = NULL;
static Nlm_TextData    recentTextData;

static Nlm_TexT        currentText = NULL;

#ifdef WIN_MSWIN
static WNDPROC         lpfnNewTextProc = NULL;
static WNDPROC         lpfnOldTextProc = NULL;
static Nlm_Boolean     handlechar;
#endif

#if defined(WIN_MOTIF) || defined(WIN_MSWIN)
static Nlm_Boolean     allowTextCallback = TRUE;
#endif

static void Nlm_LoadTextData (Nlm_TexT t, Nlm_TextTool hdl,
                              Nlm_BaR vbar, Nlm_BaR hbar,
                              Nlm_Boolean wrp, Nlm_FonT fnt,
                              Nlm_Int2 hght, Nlm_Boolean actv,
                              Nlm_Boolean chgd, Nlm_Boolean hidn,
                              Nlm_Boolean spcl, Nlm_Int2 visl,
                              Nlm_TxtActnProc slct, Nlm_TxtActnProc dslct,
                              Nlm_TxtActnProc tabn, Nlm_TxtActnProc rtnn,
                              Nlm_Boolean edtbl)

{
  Nlm_TextData  PNTR tdptr;
  Nlm_TxtPtr   tp;

  if (t != NULL) {
    tp = (Nlm_TxtPtr) Nlm_HandLock (t);
    tdptr = &(tp->text);
    tdptr->handle = hdl;
    tdptr->vScrollBar = vbar;
    tdptr->hScrollBar = hbar;
    tdptr->wrap = wrp;
    tdptr->font = fnt;
    tdptr->texthght = hght;
    tdptr->active = actv;
    tdptr->changed = chgd;
    tdptr->hidden = hidn;
    tdptr->special = spcl;
    tdptr->visLines = visl;
    tdptr->select = slct;
    tdptr->deselect = dslct;
    tdptr->tabnotify = tabn;
    tdptr->returnnotify = rtnn;
    tdptr->editable = edtbl;
    Nlm_HandUnlock (t);
    recentText = NULL;
  }
}

static void Nlm_SetTextData (Nlm_TexT t, Nlm_TextData * tdata)

{
  Nlm_TxtPtr  tp;

  if (t != NULL && tdata != NULL) {
    tp = (Nlm_TxtPtr) Nlm_HandLock (t);
    tp->text = *tdata;
    Nlm_HandUnlock (t);
    recentText = t;
    recentTextData = *tdata;
  }
}

static void Nlm_GetTextData (Nlm_TexT t, Nlm_TextData * tdata)

{
  Nlm_TxtPtr  tp;

  if (t != NULL && tdata != NULL) {
    if (t == recentText && NLM_RISKY) {
      *tdata = recentTextData;
    } else {
      tp = (Nlm_TxtPtr) Nlm_HandLock (t);
      *tdata = tp->text;
      Nlm_HandUnlock (t);
      recentText = t;
      recentTextData = *tdata;
    }
  }
}

static Nlm_TextTool Nlm_GetTextHandle (Nlm_TexT t)

{
  Nlm_TextData  tdata;

  Nlm_GetTextData (t, &tdata);
  return tdata.handle;
}

static Nlm_BaR Nlm_GetTextVScrollBar (Nlm_TexT t)

{
  Nlm_TextData  tdata;

  Nlm_GetTextData (t, &tdata);
  return tdata.vScrollBar;
}

static Nlm_BaR Nlm_GetTextHScrollBar (Nlm_TexT t)

{
  Nlm_TextData  tdata;

  Nlm_GetTextData (t, &tdata);
  return tdata.hScrollBar;
}

static Nlm_Boolean Nlm_GetTextWrap (Nlm_TexT t)

{
  Nlm_TextData  tdata;

  Nlm_GetTextData (t, &tdata);
  return tdata.wrap;
}

#ifdef WIN_MAC
static Nlm_Int2 Nlm_GetFontHeight (Nlm_TexT t)
{
  Nlm_TextData  tdata;
  Nlm_GetTextData (t, &tdata);
  return tdata.texthght;
}
#endif

static void Nlm_SetActive (Nlm_TexT t, Nlm_Boolean act)

{
  Nlm_TextData  tdata;

  Nlm_GetTextData (t, &tdata);
  tdata.active = act;
  Nlm_SetTextData (t, &tdata);
}

static Nlm_Boolean Nlm_GetActive (Nlm_TexT t)

{
  Nlm_TextData  tdata;

  Nlm_GetTextData (t, &tdata);
  return tdata.active;
}

#ifdef WIN_MAC
static void Nlm_SetChanged (Nlm_TexT t, Nlm_Boolean chd)
{
  Nlm_TextData  tdata;
  Nlm_GetTextData (t, &tdata);
  tdata.changed = chd;
  Nlm_SetTextData (t, &tdata);
}
#endif


static Nlm_Boolean Nlm_IsHiddenText (Nlm_TexT t)

{
  Nlm_TextData  tdata;

  Nlm_GetTextData (t, &tdata);
  return tdata.hidden;
}

#ifndef WIN_MOTIF
static Nlm_Boolean Nlm_IsHiddenOrSpecialText (Nlm_TexT t)
{
  Nlm_TextData  tdata;
  Nlm_GetTextData (t, &tdata);
  return (Nlm_Boolean) (tdata.hidden || tdata.special);
}

static Nlm_Int2 Nlm_GetVisLines (Nlm_TexT t)
{
  Nlm_TextData  tdata;
  Nlm_GetTextData (t, &tdata);
  return tdata.visLines;
}
#endif

extern void Nlm_SetTextSelect (Nlm_TexT t, Nlm_TxtActnProc slct, Nlm_TxtActnProc dslct)

{
  Nlm_TextData  tdata;

  if (t != NULL) {
    Nlm_GetTextData (t, &tdata);
    tdata.select = slct;
    tdata.deselect = dslct;
    Nlm_SetTextData (t, &tdata);
  }
}


static void Nlm_SetPassword (Nlm_TexT t, Nlm_CharPtr passwd)
{
  if (t != NULL  &&  passwd != NULL)
    {
      Nlm_PwdPtr pp = (Nlm_PwdPtr) Nlm_HandLock (t);
      Nlm_StringNCpy_0((Nlm_CharPtr)pp->password, passwd, MAX_PASSWORD);
      Nlm_HandUnlock (t);
    }
}


static void Nlm_GetPassword (Nlm_TexT t, Nlm_CharPtr passwd,
                             size_t maxsize)
{
  if (t != NULL  &&  passwd != NULL)
    {
      Nlm_PwdPtr pp = (Nlm_PwdPtr) Nlm_HandLock (t);
      Nlm_StringNCpy_0(passwd, (Nlm_CharPtr)pp->password, maxsize);
      Nlm_HandUnlock (t);
    }
}


static size_t Nlm_GetPasswordLength (Nlm_TexT t)
{
  Nlm_PwdPtr pp = (Nlm_PwdPtr) Nlm_HandLock (t);
  size_t len = Nlm_StringLen( pp->password );
  Nlm_HandUnlock (t);
  return len;
}


static Nlm_Boolean Nlm_GetTextEditable (Nlm_TexT t)

{
  Nlm_TextData  tdata;

  Nlm_GetTextData (t, &tdata);
  return tdata.editable;
}


#ifdef WIN_MAC
static Nlm_Int2 Nlm_GetTextLines (Nlm_TexT t)

{
  Nlm_TextTool  h;
  Nlm_Int2      lines;
  TEPtr         tptr;

  h = Nlm_GetTextHandle (t);
  HLock ((Handle) h);
  tptr = (TEPtr) *((Handle) h);
  lines = tptr->nLines;
  HUnlock ((Handle) h);
  return lines;
}

static Nlm_Int2 Nlm_GetLineHeight (Nlm_TexT t)

{
  Nlm_TextTool  h;
  Nlm_Int2      height;
  TEPtr         tptr;

  h = Nlm_GetTextHandle (t);
  HLock ((Handle) h);
  tptr = (TEPtr) *((Handle) h);
  height = tptr->lineHeight;
  HUnlock ((Handle) h);
  return height;
}

static Nlm_Int2 Nlm_GetInsertionStartLine (Nlm_TexT t)

{
  Nlm_TextTool  h;
  Nlm_Int2      i;
  short         *lines;
  Nlm_Int2      numLines;
  Nlm_Int2      startLoc;
  TEPtr         tptr;

  h = Nlm_GetTextHandle (t);
  HLock ((Handle) h);
  tptr = (TEPtr) *((Handle) h);
  numLines = tptr->nLines;
  startLoc = tptr->selStart;
  i = 0;
  lines = &(tptr->lineStarts[0]);
  while (i < numLines && startLoc > lines [i]) {
    i++;
  }
  HUnlock ((Handle) h);
  return i;
}

static void Nlm_ScrollToInsertionPoint (Nlm_TexT t)

{
  Nlm_BaR   sb;
  Nlm_Int2  start;
  Nlm_Int2  val;
  Nlm_Int2  vis;

  sb = Nlm_GetTextVScrollBar (t);
  if (sb != NULL) {
    start = Nlm_GetInsertionStartLine (t);
    vis = Nlm_GetVisLines (t);
    val = Nlm_DoGetValue ((Nlm_GraphiC) sb);
    if (val + vis < start) {
      Nlm_DoSetValue ((Nlm_GraphiC) sb, start - (vis / 2), TRUE);
    } else if (val > start) {
      Nlm_DoSetValue ((Nlm_GraphiC) sb, start - (vis / 2), TRUE);
    }
  }
}

static void Nlm_UpdateScrollBar (Nlm_TexT t)

{
  Nlm_Int2  lines;
  Nlm_Int2  newval;
  Nlm_BaR   sb;
  Nlm_Int2  start;
  Nlm_Int2  vis;

  sb = Nlm_GetTextVScrollBar (t);
  if (sb != NULL) {
    lines = Nlm_GetTextLines (t);
    start = Nlm_GetInsertionStartLine (t);
    vis = Nlm_GetVisLines (t);
    newval = 0;
    if (lines > vis) {
      newval = lines - vis;
    }
    if (newval == 0) {
      if (Nlm_DoGetValue ((Nlm_GraphiC) sb) > newval) {
        Nlm_DoSetValue ((Nlm_GraphiC) sb, newval, FALSE);
      }
      Nlm_DoReset ((Nlm_GraphiC) sb, FALSE);
    } else {
      Nlm_DoSetRange ((Nlm_GraphiC) sb, vis - 1, vis - 1, newval, FALSE);
      if (Nlm_DoGetValue ((Nlm_GraphiC) sb) > newval) {
        Nlm_DoSetValue ((Nlm_GraphiC) sb, newval, FALSE);
      }
    }
  }
  sb = Nlm_GetTextHScrollBar (t);
  if (sb != NULL) {
    Nlm_CorrectBarPage (sb, 10, 10);
    Nlm_CorrectBarMax (sb, HSCROLL_POSITIONS);
  }
}

static void Nlm_DeactivateBoxesInList (Nlm_TexT t)
{
  Nlm_WindoW  w;
  w = Nlm_GetParentWindow ((Nlm_GraphiC) t);
  Nlm_DoLoseFocus ((Nlm_GraphiC) w, (Nlm_GraphiC) t, FALSE);
}
#endif

static void Nlm_DoTextSelect (Nlm_TexT t)

{
  Nlm_TxtActnProc  sel;
  Nlm_TextData     tdata;

  if (t != NULL) {
    Nlm_GetTextData (t, &tdata);
    sel = tdata.select;
    if (sel != NULL && currentText != t) {
      currentText = t;
      sel (t);
    }
  }
  currentText = t;
}

static void Nlm_DoTextDeselect (Nlm_TexT t)

{
  Nlm_TxtActnProc  desel;
  Nlm_TextData     tdata;

  if (t != NULL) {
    Nlm_GetTextData (t, &tdata);
    desel = tdata.deselect;
    if (desel != NULL) {
      desel (t);
    }
  }
}

static void Nlm_DoTabCallback (Nlm_TexT t)

{
  Nlm_TextData  tdata;

  Nlm_GetTextData (t, &tdata);
  if (tdata.tabnotify != NULL) {
    tdata.tabnotify (t);
  }
}

static Nlm_Boolean Nlm_DoReturnCallback (Nlm_TexT t)

{
  Nlm_TextData  tdata;

  Nlm_GetTextData (t, &tdata);
  if (tdata.returnnotify != NULL) {
    tdata.returnnotify (t);
    return TRUE;
  }
  return FALSE;
}

static void Nlm_SelectAText (Nlm_TexT t, Nlm_Int4 begin, Nlm_Int4 end)

{
#ifdef WIN_MAC
  Nlm_TextTool  h;
  TEPtr         hp;
  short         len;
  Nlm_Int4      selStart;
  Nlm_Int4      selEnd;

  Nlm_DeactivateBoxesInList (t);
  h = Nlm_GetTextHandle (t);
  HLock ((Handle) h);
  hp = (TEPtr) *((Handle) h);
  len = hp->teLength;
  HUnlock ((Handle) h);
  selStart = begin;
  selEnd = end;
  if (selEnd > (Nlm_Int4) len) {
    selEnd = (Nlm_Int4) len;
  }
  if (Nlm_Visible (t) && Nlm_AllParentsVisible (t)) {
    TESetSelect (selStart, selEnd, h);
  }
  Nlm_SetActive (t, TRUE);
  Nlm_DoActivate ((Nlm_GraphiC) t, FALSE);
  Nlm_DoTextSelect (t);
#endif
#ifdef WIN_MSWIN
  Nlm_TextTool  h;

  h = Nlm_GetTextHandle (t);
  Edit_SetSel (h, begin, end);
  Edit_ScrollCaret (h);
  Nlm_SetActive (t, TRUE);
  Nlm_DoActivate ((Nlm_GraphiC) t, FALSE);
  Nlm_DoTextSelect (t);
#endif
#ifdef WIN_MOTIF
  Nlm_TextTool    h;
  XmTextPosition  max;

  if (Nlm_WindowHasBeenShown (Nlm_ParentWindow (t))) {
    allowTextCallback = FALSE;
    h = Nlm_GetTextHandle (t);
    XmProcessTraversal (h, XmTRAVERSE_CURRENT);
    max = XmTextGetLastPosition (h);
    if (max > (XmTextPosition) end) {
      max = (XmTextPosition) end;
    }
    if (max > 0)
      {
        XmTextSetSelection (h, (XmTextPosition) begin,
                            (XmTextPosition) max, (Time) 0);
        XmTextSetHighlight (h, (XmTextPosition) begin,
                            (XmTextPosition) max, XmHIGHLIGHT_SELECTED);
      }
    allowTextCallback = TRUE;
  }
  Nlm_DoTextSelect (t);
#endif
}

extern Nlm_Boolean Nlm_TextSelectionRange(Nlm_TexT t,
                                          Nlm_Int4Ptr begin, Nlm_Int4Ptr end)
{
  Nlm_Int4 x_begin, x_end;
  Nlm_TextTool h = Nlm_GetTextHandle( t );
  if ( !h )
    return FALSE;

  {{
#if defined(WIN_MAC)
    TEPtr hp;

    HLock( (Handle)h );
    hp = (TEPtr) *( (Handle)h );
    x_begin = hp->selStart;
    x_end   = hp->selEnd;
    HUnlock( (Handle)h );

#elif defined(WIN_MSWIN)
    DWORD dwBegin, dwEnd;
    SNDMSG(h, EM_GETSEL, (WPARAM)(&dwBegin), (LPARAM)(&dwEnd));
    
    x_begin = dwBegin;
    x_end   = dwEnd;

#elif defined(WIN_MOTIF)
    XmTextPosition left;
    XmTextPosition right;

    if (!Nlm_WindowHasBeenShown( Nlm_ParentWindow(t) )  ||
        !XmTextGetSelectionPosition(h, &left, &right))
      return FALSE;
    x_begin = left;
    x_end   = right;

#else
    return FALSE;
#endif
  }}

#if !defined(WIN_MAC)
  /* MAC implmementation uses Nlm_TextSelectionRange()
     to get current cursor position even when selection is empty */
  if (x_begin == x_end)
    return FALSE;
#endif

  if ( begin )
    *begin = x_begin;
  if ( end )
    *end   = x_end;

#if defined(WIN_MAC)
  /* MAC implmementation uses Nlm_TextSelectionRange()
     to get current cursor position even when selection is empty */
  if (x_begin == x_end)
    return FALSE;
#endif

  return TRUE;
}

#ifdef WIN_MAC
static Nlm_Boolean Nlm_DialogTextClick (Nlm_GraphiC t, Nlm_PoinT pt)

{
  Nlm_TextTool   h;
  Nlm_PointTool  ptool;
  Nlm_RecT       r;
  Nlm_Boolean    rsult;
  Nlm_WindoW     w;

  rsult = FALSE;
  if (! Nlm_GetTextEditable ((Nlm_TexT) t)) return FALSE;
  Nlm_GetRect (t, &r);
  if (Nlm_PtInRect (pt, &r)) {
    Nlm_DeactivateBoxesInList ((Nlm_TexT) t);
    h = Nlm_GetTextHandle ((Nlm_TexT) t);
    Nlm_GetRect (t, &r);
    Nlm_PoinTToPointTool (pt, &ptool);
    TEClick (ptool, Nlm_shftKey, h);
    Nlm_SetActive ((Nlm_TexT) t, TRUE);
    w = Nlm_GetParentWindow (t);
    Nlm_DoActivate (t, FALSE);
    do {
    } while (Nlm_MouseButton ());
    Nlm_DoTextSelect ((Nlm_TexT) t);
    rsult = TRUE;
  }
  return rsult;
}

static Nlm_Boolean Nlm_ScrollTextClick (Nlm_GraphiC t, Nlm_PoinT pt)

{
  Nlm_TextTool   h;
  Nlm_BaR        hsb;
  Nlm_PointTool  ptool;
  Nlm_RecT       r;
  Nlm_Boolean    rsult;
  Nlm_BaR        vsb;
  Nlm_WindoW     w;
  Nlm_Boolean    wrap;

  rsult = FALSE;
  Nlm_GetRect (t, &r);
  wrap = Nlm_GetTextWrap ((Nlm_TexT) t);
  r.right += Nlm_vScrollBarWidth;
  if (! wrap) {
    r.bottom += Nlm_hScrollBarHeight;
  }
  if (Nlm_PtInRect (pt, &r)) {
    vsb = Nlm_GetTextVScrollBar ((Nlm_TexT) t);
    hsb = Nlm_GetTextHScrollBar ((Nlm_TexT) t);
    if (vsb != NULL && Nlm_DoClick ((Nlm_GraphiC) vsb, pt)) {
    } else if (hsb != NULL && Nlm_DoClick ((Nlm_GraphiC) hsb, pt)) {
    } else {
      if (! Nlm_GetTextEditable ((Nlm_TexT) t)) return FALSE;
      Nlm_DeactivateBoxesInList ((Nlm_TexT) t);
      h = Nlm_GetTextHandle ((Nlm_TexT) t);
      r.right -= Nlm_vScrollBarWidth;
      if (! wrap) {
        r.bottom -= Nlm_hScrollBarHeight;
      }
      Nlm_PoinTToPointTool (pt, &ptool);
      TEClick (ptool, Nlm_shftKey, h);
      Nlm_SetActive ((Nlm_TexT) t, TRUE);
      w = Nlm_GetParentWindow (t);
      Nlm_DoActivate (t, FALSE);
      do {
      } while (Nlm_MouseButton ());
      Nlm_DoTextSelect ((Nlm_TexT) t);
      rsult = TRUE;
    }
  }
  return rsult;
}

static Nlm_Boolean Nlm_TextKey (Nlm_GraphiC t, Nlm_Char ch)

{
  Nlm_Boolean   act;
  Nlm_TextTool  h;
  Nlm_Boolean   rsult;

  rsult = FALSE;
  if (! Nlm_GetTextEditable ((Nlm_TexT) t)) return FALSE;
  act = Nlm_GetActive ((Nlm_TexT) t);
  if (act && ! Nlm_cmmdKey) {
    if (ch != '\0') {
      h = Nlm_GetTextHandle ((Nlm_TexT) t);
      Nlm_ScrollToInsertionPoint ((Nlm_TexT) t);
      TEKey (ch, h);
      TECalText (h);
      Nlm_UpdateScrollBar ((Nlm_TexT) t);
      Nlm_ScrollToInsertionPoint ((Nlm_TexT) t);
      Nlm_DoAction (t);
      Nlm_SetChanged ((Nlm_TexT) t, TRUE);
      rsult = TRUE;
    }
  }
  return rsult;
}

static Nlm_Boolean Nlm_DialogKey (Nlm_GraphiC t, Nlm_Char ch)

{
  Nlm_Boolean  act;
  Nlm_Boolean  rsult;

  rsult = FALSE;
  if (! Nlm_GetTextEditable ((Nlm_TexT) t)) return FALSE;
  act = Nlm_GetActive ((Nlm_TexT) t);
  if (act && ! Nlm_cmmdKey) {
    if (ch == '\t' && Nlm_IsHiddenOrSpecialText ((Nlm_TexT) t)) {
      Nlm_DoTabCallback ((Nlm_TexT) t);
      rsult = TRUE;
    } else if (ch == '\t') {
      Nlm_DoSendFocus (t, ch);
      rsult = TRUE;
    } else if (ch == '\n'  || ch == '\r' || ch == '\3') {
      rsult = Nlm_DoReturnCallback ((Nlm_TexT) t);
    } else if (ch != '\0') {
      rsult = Nlm_TextKey (t, ch);
    }
  }
  return rsult;
}

static Nlm_Boolean Nlm_PasswordKey (Nlm_GraphiC t, Nlm_Char ch)

{
  Nlm_Boolean   act;
  Nlm_TextTool  h;
  TEPtr         hp;
  Nlm_Int2      len;
  Nlm_Char      password [MAX_PASSWORD];
  Nlm_RecT      r;
  Nlm_RectTool  rtool;
  Nlm_Boolean   rsult;
  Nlm_Int4      selStart;
  Nlm_Int4      selEnd;

  rsult = FALSE;
  if (! Nlm_GetTextEditable ((Nlm_TexT) t)) return FALSE;
  act = Nlm_GetActive ((Nlm_TexT) t);
  if (act && ! Nlm_cmmdKey) {
    if (ch == '\t') {
      Nlm_DoSendFocus (t, ch);
      rsult = TRUE;
    } else if (ch == '\n'  || ch == '\r' || ch == '\3') {
    } else if (ch == '\b') {
      rsult = TRUE;
      Nlm_SetPassword ((Nlm_TexT) t, "");
      h = Nlm_GetTextHandle ((Nlm_TexT) t);
      selStart = 0;
      HLock ((Handle) h);
      hp = (TEPtr) *((Handle) h);
      selEnd = hp->teLength;
      HUnlock ((Handle) h);
      Nlm_GetRect (t, &r);
      if (Nlm_GetVisible (t) && Nlm_GetAllParentsVisible (t)) {
        TESetSelect (selStart, selEnd, h);
        TEDelete (h);
        Nlm_InsetRect (&r, 2, 2);
        Nlm_EraseRect (&r);
        Nlm_RecTToRectTool (&r, &rtool);
        TEUpdate (&rtool, h);
      }
      Nlm_DoAction (t);
    } else if (ch != '\0') {
      Nlm_GetPassword ((Nlm_TexT) t, password, sizeof (password));
      len = (Nlm_Int2) Nlm_StringLen (password);
      if (len < sizeof (password) - 2) {
        password [len] = ch;
        password [len + 1] = '\0';
      }
      Nlm_SetPassword ((Nlm_TexT) t, password);
      rsult = Nlm_TextKey (t, '*');
    }
  }
  return rsult;
}

static void Nlm_DrawDialogText (Nlm_GraphiC t)

{
  Nlm_TextTool  h;
  Nlm_RecT      r;
  Nlm_RectTool  ttool;

  if (Nlm_GetVisible (t) && Nlm_GetAllParentsVisible (t)) {
    Nlm_GetRect (t, &r);
    if (Nlm_RectInRgn (&r, Nlm_updateRgn)) {
      Nlm_EraseRect (&r);
      if (Nlm_GetEnabled (t) && Nlm_GetAllParentsEnabled (t)) {
        h = Nlm_GetTextHandle ((Nlm_TexT) t);
        if (Nlm_GetActive ((Nlm_TexT) t)) {
          TEActivate (h);
        } else {
          TEDeactivate (h);
        }
        TECalText (h);
        Nlm_InsetRect (&r, 2, 2);
        Nlm_RecTToRectTool (&r, &ttool);
        Nlm_SelectFont (Nlm_systemFont);
        TEUpdate (&ttool, h);
        Nlm_InsetRect (&r, -2, -2);
        Nlm_FrameRect (&r);
      } else {
#ifdef DCLAP
        h = Nlm_GetTextHandle ((Nlm_TexT) t);
        TEDeactivate (h);
        TECalText (h);
        Nlm_InsetRect (&r, 2, 2);
        Nlm_RecTToRectTool (&r, &ttool);
        Nlm_SelectFont (Nlm_systemFont);
        TEUpdate (&ttool, h);
        Nlm_InsetRect (&r, -2, -2);
#endif
        Nlm_Dotted ();
        Nlm_FrameRect (&r);
        Nlm_Solid ();
      }
    }
  }
}

static void Nlm_DrawHiddenText (Nlm_GraphiC t)

{
  Nlm_TextTool  h;
  Nlm_RecT      r;
  Nlm_RectTool  ttool;

  if (Nlm_GetVisible (t) && Nlm_GetAllParentsVisible (t)) {
    Nlm_GetRect (t, &r);
    if (Nlm_RectInRgn (&r, Nlm_updateRgn)) {
      Nlm_EraseRect (&r);
      if (Nlm_GetEnabled (t) && Nlm_GetAllParentsEnabled (t)) {
        h = Nlm_GetTextHandle ((Nlm_TexT) t);
        if (Nlm_GetActive ((Nlm_TexT) t)) {
          TEActivate (h);
        } else {
          TEDeactivate (h);
        }
        TECalText (h);
        Nlm_RecTToRectTool (&r, &ttool);
        Nlm_SelectFont (Nlm_systemFont);
        TEUpdate (&ttool, h);
      }
    }
  }
}

static void Nlm_DrawScrollText (Nlm_GraphiC t)

{
  Nlm_TextTool  h;
  Nlm_BaR       hsb;
  Nlm_RecT      r;
  Nlm_RectTool  rtool;
  Nlm_BaR       vsb;
  Nlm_Boolean   wrap;

  if (Nlm_GetVisible (t) && Nlm_GetAllParentsVisible (t)) {
    Nlm_GetRect (t, &r);
    r.right += Nlm_vScrollBarWidth;
    wrap = Nlm_GetTextWrap ((Nlm_TexT) t);
    if (! wrap) {
      r.bottom += Nlm_hScrollBarHeight;
    }
    if (Nlm_RectInRgn (&r, Nlm_updateRgn)) {
      if (Nlm_GetEnabled (t) && Nlm_GetAllParentsEnabled (t)) {
        Nlm_FrameRect (&r);
        vsb = Nlm_GetTextVScrollBar ((Nlm_TexT) t);
        if (vsb != NULL) {
          Nlm_DoDraw ((Nlm_GraphiC) vsb);
        }
        hsb = Nlm_GetTextHScrollBar ((Nlm_TexT) t);
        if (hsb != NULL) {
          Nlm_DoDraw ((Nlm_GraphiC) hsb);
        }
        r.right -= Nlm_vScrollBarWidth;
        if (! wrap) {
          r.bottom -= Nlm_hScrollBarHeight;
        }
        Nlm_InsetRect (&r, 4, 2);
        r.bottom = r.top + Nlm_GetVisLines ((Nlm_TexT) t) * Nlm_GetFontHeight ((Nlm_TexT) t);
        h = Nlm_GetTextHandle ((Nlm_TexT) t);
        if (Nlm_GetActive ((Nlm_TexT) t)) {
          TEActivate (h);
        } else {
          TEDeactivate (h);
        }
        TECalText (h);
        Nlm_RecTToRectTool (&r, &rtool);
        Nlm_SelectFont (Nlm_systemFont);
        TEUpdate (&rtool, h);
      } else {
#ifdef DCLAP
        h = Nlm_GetTextHandle ((Nlm_TexT) t);
        TEDeactivate (h);
        TECalText (h);
        Nlm_RecTToRectTool (&r, &rtool);
        Nlm_SelectFont (Nlm_systemFont);
        TEUpdate (&rtool, h);
#else
        Nlm_EraseRect (&r);
#endif
        Nlm_Dotted ();
        Nlm_FrameRect (&r);
        Nlm_Solid ();
      }
    }
  }
}

static Nlm_Boolean Nlm_IdleText (Nlm_GraphiC t, Nlm_PoinT pt)

{  Nlm_TextTool  h;

  if (Nlm_GetVisible (t) && Nlm_GetEnabled (t) && Nlm_GetActive ((Nlm_TexT) t)) {
    h = Nlm_GetTextHandle ((Nlm_TexT) t);
    TEIdle (h);
  }
  return TRUE;
}
#endif

#ifdef WIN_MSWIN
static Nlm_Boolean Nlm_DialogTextCommand (Nlm_GraphiC t)

{
  if (Nlm_currentCode == EN_CHANGE && allowTextCallback) {
    Nlm_DoAction (t);
  }
  return TRUE;
}

static Nlm_Boolean Nlm_PasswordTextCommand (Nlm_GraphiC t)

{
  Nlm_TextTool  h;
  Nlm_Char      password [MAX_PASSWORD];

  if (Nlm_currentCode == EN_CHANGE && allowTextCallback) {
    h = Nlm_GetTextHandle ((Nlm_TexT) t);
    GetWindowText (h, password, sizeof (password) - 2);
    Nlm_SetPassword ((Nlm_TexT) t, password);
    Nlm_DoAction (t);
  }
  return TRUE;
}

static Nlm_Boolean Nlm_ScrollTextCommand (Nlm_GraphiC t)

{
 if (allowTextCallback)
   switch (Nlm_currentCode) {
     case EN_CHANGE:
       Nlm_DoAction (t);
       break;
     case EN_KILLFOCUS:
       Nlm_DoLoseFocus (t, NULL, TRUE);
       break;
     case EN_SETFOCUS:
       Nlm_DoGainFocus (t, 0 /* '\t' */, TRUE);
       /* Nlm_SelectText ((Nlm_TexT) t, 0, 0); */
       break;
   }
  return TRUE;
}
#endif


static void Nlm_ShowText (Nlm_GraphiC t, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_TextTool  h;
  Nlm_WindoW    tempPort;

  if (setFlag) {
    Nlm_SetVisible (t, TRUE);
  }
  if (Nlm_GetVisible (t) && Nlm_AllParentsButWindowVisible (t)) {
    tempPort = Nlm_SavePortIfNeeded (t, savePort);
    h = Nlm_GetTextHandle ((Nlm_TexT) t);
#ifdef WIN_MAC
    Nlm_DoDraw (t);
#endif
#ifdef WIN_MSWIN
    ShowWindow (h, SW_SHOW);
    UpdateWindow (h);
#endif
#ifdef WIN_MOTIF
    XtManageChild (h);
#endif
    Nlm_RestorePort (tempPort);
  }
}

static void Nlm_ShowScrollText (Nlm_GraphiC t, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_TextTool  h;
  Nlm_BaR       sb;
  Nlm_WindoW    tempPort;

  if (setFlag) {
    Nlm_SetVisible (t, TRUE);
  }
  if (Nlm_GetVisible (t) && Nlm_AllParentsButWindowVisible (t)) {
    tempPort = Nlm_SavePortIfNeeded (t, savePort);
    sb = Nlm_GetTextVScrollBar ((Nlm_TexT) t);
    if (sb != NULL) {
      Nlm_DoShow ((Nlm_GraphiC) sb, TRUE, FALSE);
    }
    sb = Nlm_GetTextHScrollBar ((Nlm_TexT) t);
    if (sb != NULL) {
      Nlm_DoShow ((Nlm_GraphiC) sb, TRUE, FALSE);
    }
    h = Nlm_GetTextHandle ((Nlm_TexT) t);
#ifdef WIN_MAC
    Nlm_DoDraw (t);
#endif
#ifdef WIN_MSWIN
    ShowWindow (h, SW_SHOW);
    UpdateWindow (h);
#endif
#ifdef WIN_MOTIF
    XtManageChild (XtParent (h));
#endif
    Nlm_RestorePort (tempPort);
  }
}

static void Nlm_HideText (Nlm_GraphiC t, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_TextTool  h;
  Nlm_WindoW    tempPort;
#ifdef WIN_MAC
  Nlm_RecT      r;
#endif

  if (setFlag) {
    Nlm_SetVisible (t, FALSE);
  }
  tempPort = Nlm_SavePortIfNeeded (t, savePort);
  h = Nlm_GetTextHandle ((Nlm_TexT) t);
#ifdef WIN_MAC
  if (Nlm_GetAllParentsVisible (t)) {
    Nlm_GetRect (t, &r);
    Nlm_InsetRect (&r, -1, -1);
    Nlm_EraseRect (&r);
    Nlm_ValidRect (&r);
  }
#endif
#ifdef WIN_MSWIN
  ShowWindow (h, SW_HIDE);
  UpdateWindow (h);
#endif
#ifdef WIN_MOTIF
  XtUnmanageChild (h);
#endif
  Nlm_RestorePort (tempPort);
}

static void Nlm_HideScrollText (Nlm_GraphiC t, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_TextTool  h;
  Nlm_BaR       sb;
  Nlm_WindoW    tempPort;
#ifdef WIN_MAC
  Nlm_RecT      r;
#endif

  if (setFlag) {
    Nlm_SetVisible (t, FALSE);
  }
  tempPort = Nlm_SavePortIfNeeded (t, savePort);
  sb = Nlm_GetTextVScrollBar ((Nlm_TexT) t);
  if (sb != NULL) {
    Nlm_DoHide ((Nlm_GraphiC) sb, TRUE, FALSE);
  }
  sb = Nlm_GetTextHScrollBar ((Nlm_TexT) t);
  if (sb != NULL) {
    Nlm_DoHide ((Nlm_GraphiC) sb, TRUE, FALSE);
  }
  h = Nlm_GetTextHandle ((Nlm_TexT) t);
#ifdef WIN_MAC
  if (Nlm_GetAllParentsVisible (t)) {
    Nlm_GetRect (t, &r);
    if (sb != NULL) {
      r.right += Nlm_vScrollBarWidth;
    }
    Nlm_InsetRect (&r, -1, -1);
    Nlm_EraseRect (&r);
    Nlm_ValidRect (&r);
  }
#endif
#ifdef WIN_MSWIN
  ShowWindow (h, SW_HIDE);
  UpdateWindow (h);
#endif
#ifdef WIN_MOTIF
  XtUnmanageChild (XtParent (h));
#endif
  Nlm_RestorePort (tempPort);
}

static void Nlm_EnableText (Nlm_GraphiC t, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_TextTool  h;
  Nlm_BaR       sb;
  Nlm_WindoW    tempPort;

  if (setFlag) {
    Nlm_SetEnabled (t, TRUE);
  }
  if (Nlm_GetEnabled (t) && Nlm_GetAllParentsEnabled (t)) {
    tempPort = Nlm_SavePortIfNeeded (t, savePort);
    sb = Nlm_GetTextVScrollBar ((Nlm_TexT) t);
    if (sb != NULL) {
      Nlm_DoEnable ((Nlm_GraphiC) sb, TRUE, FALSE);
    }
    sb = Nlm_GetTextHScrollBar ((Nlm_TexT) t);
    if (sb != NULL) {
      Nlm_DoEnable ((Nlm_GraphiC) sb, TRUE, FALSE);
    }
    h = Nlm_GetTextHandle ((Nlm_TexT) t);
#ifdef WIN_MAC
    if (Nlm_GetVisible (t) && Nlm_GetAllParentsVisible (t)) {
      Nlm_DoDraw (t);
    }
#endif
#ifdef WIN_MSWIN
    EnableWindow (h, TRUE);
#endif
#ifdef WIN_MOTIF
    XtVaSetValues (h, XmNsensitive, TRUE, NULL);
#endif
    Nlm_RestorePort (tempPort);
  }
}

static void Nlm_DisableText (Nlm_GraphiC t, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_TextTool  h;
  Nlm_BaR       sb;
  Nlm_WindoW    tempPort;

  if (setFlag) {
    Nlm_SetEnabled (t, FALSE);
  }
  tempPort = Nlm_SavePortIfNeeded (t, savePort);
  sb = Nlm_GetTextVScrollBar ((Nlm_TexT) t);
  if (sb != NULL) {
    Nlm_DoDisable ((Nlm_GraphiC) sb, TRUE, FALSE);
  }
  sb = Nlm_GetTextHScrollBar ((Nlm_TexT) t);
  if (sb != NULL) {
    Nlm_DoDisable ((Nlm_GraphiC) sb, TRUE, FALSE);
  }
  h = Nlm_GetTextHandle ((Nlm_TexT) t);
#ifdef WIN_MAC
  if (Nlm_GetVisible (t) && Nlm_GetAllParentsVisible (t)) {
    Nlm_DoDraw (t);
  }
#endif
#ifdef WIN_MSWIN
  EnableWindow (h, FALSE);
#endif
#ifdef WIN_MOTIF
  XtVaSetValues (h, XmNsensitive, FALSE, NULL);
#endif
  Nlm_RestorePort (tempPort);
}

static void Nlm_ActivateText (Nlm_GraphiC t, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  Nlm_TextTool  h;
  Nlm_RecT      r;
  Nlm_RectTool  rtool;
#endif

  if (Nlm_GetVisible (t) && Nlm_GetAllParentsVisible (t)) {
    if (Nlm_GetEnabled (t) && Nlm_GetAllParentsEnabled (t) && Nlm_GetActive ((Nlm_TexT) t)) {
#ifdef WIN_MAC
      h = Nlm_GetTextHandle ((Nlm_TexT) t);
      Nlm_GetRect (t, &r);
      Nlm_InsetRect (&r, 2, 2);
      TEActivate (h);
      TECalText (h);
      Nlm_RecTToRectTool (&r, &rtool);
      TEUpdate (&rtool, h);
#endif
#ifdef WIN_MSWIN
      Nlm_TextTool h = Nlm_GetTextHandle ((Nlm_TexT) t);
      if (t != NULL && h != NULL) {
        if (Nlm_Visible (t) && Nlm_AllParentsVisible (t)) {
          SetFocus (h);
        }
      }
#endif
#ifdef WIN_MOTIF
#endif
    }
  }
}

static void Nlm_ActivateHiddenText (Nlm_GraphiC t, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  Nlm_TextTool  h;
  Nlm_RecT      r;
  Nlm_RectTool  rtool;
#endif
#ifdef WIN_MSWIN
  Nlm_TextTool  h;
#endif

  if (Nlm_GetVisible (t) && Nlm_GetAllParentsVisible (t)) {
    if (Nlm_GetEnabled (t) && Nlm_GetAllParentsEnabled (t) && Nlm_GetActive ((Nlm_TexT) t)) {
#ifdef WIN_MAC
      h = Nlm_GetTextHandle ((Nlm_TexT) t);
      Nlm_GetRect (t, &r);
      TEActivate (h);
      TECalText (h);
      Nlm_RecTToRectTool (&r, &rtool);
      TEUpdate (&rtool, h);
#endif
#ifdef WIN_MSWIN
      h = Nlm_GetTextHandle ((Nlm_TexT) t);
      if (t != NULL && h != NULL) {
        if (Nlm_Visible (t) && Nlm_AllParentsVisible (t)) {
          SetFocus (h);
        }
      }
#endif
    }
  }
}

static void Nlm_ActivateScrollText (Nlm_GraphiC t, Nlm_Boolean savePort)

{
#ifndef WIN_MOTIF
  Nlm_TextTool  h;
#endif
#ifdef WIN_MAC
  Nlm_RecT      r;
  Nlm_RectTool  rtool;
#endif

  if (Nlm_GetVisible (t) && Nlm_GetAllParentsVisible (t)) {
    if (Nlm_GetEnabled (t) && Nlm_GetAllParentsEnabled (t) && Nlm_GetActive ((Nlm_TexT) t)) {
#ifdef WIN_MAC
      h = Nlm_GetTextHandle ((Nlm_TexT) t);
      Nlm_GetRect (t, &r);
      Nlm_InsetRect (&r, 4, 2);
      r.bottom = r.top + Nlm_GetVisLines ((Nlm_TexT) t) * Nlm_GetFontHeight ((Nlm_TexT) t);
      TEActivate (h);
      TECalText (h);
      Nlm_RecTToRectTool (&r, &rtool);
      TEUpdate (&rtool, h);
#endif
#ifdef WIN_MSWIN
      h = Nlm_GetTextHandle ((Nlm_TexT) t);
      if (t != NULL && h != NULL) {
        if (Nlm_Visible (t) && Nlm_AllParentsVisible (t)) {
          SetFocus (h);
        }
      }
#endif
    }
  }
}

static void Nlm_DeactivateText (Nlm_GraphiC t, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  Nlm_TextTool  h;
  Nlm_RecT      r;
  Nlm_RectTool  rtool;

  if (Nlm_GetVisible (t) && Nlm_GetAllParentsVisible (t)) {
    if (Nlm_GetEnabled (t) && Nlm_GetAllParentsEnabled (t)) {
      h = Nlm_GetTextHandle ((Nlm_TexT) t);
      Nlm_GetRect (t, &r);
      Nlm_InsetRect (&r, 2, 2);
      TEDeactivate (h);
      TECalText (h);
      Nlm_RecTToRectTool (&r, &rtool);
      TEUpdate (&rtool, h);
    }
  }
#endif
}

static void Nlm_DeactivateScrollText (Nlm_GraphiC t, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  Nlm_TextTool  h;
  Nlm_RecT      r;
  Nlm_RectTool  rtool;

  if (Nlm_GetVisible (t) && Nlm_GetAllParentsVisible (t)) {
    if (Nlm_GetEnabled (t) && Nlm_GetAllParentsEnabled (t)) {
      h = Nlm_GetTextHandle ((Nlm_TexT) t);
      Nlm_GetRect (t, &r);
      Nlm_InsetRect (&r, 4, 2);
      r.bottom = r.top + Nlm_GetVisLines ((Nlm_TexT) t) * Nlm_GetFontHeight ((Nlm_TexT) t);
      TEDeactivate (h);
      TECalText (h);
      Nlm_RecTToRectTool (&r, &rtool);
      TEUpdate (&rtool, h);
    }
  }
#endif
}

static void Nlm_ResetText (Nlm_GraphiC t, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  Nlm_Int2      delta;
  Nlm_TextTool  h;
  Nlm_Int2      height;
  Nlm_BaR       sb;
  Nlm_WindoW    tempPort;
  Nlm_Int2      width;

  tempPort = Nlm_SavePortIfNeeded (t, savePort);
  h = Nlm_GetTextHandle ((Nlm_TexT) t);
  TESetSelect (0, 32767, h);
  TEDelete (h);
  sb = Nlm_GetTextVScrollBar ((Nlm_TexT) t);
  if (sb != NULL) {
    delta = Nlm_DoGetValue ((Nlm_GraphiC) sb);
    height = Nlm_GetLineHeight ((Nlm_TexT) t);
    TEScroll (0, delta * height, h);
    Nlm_DoReset ((Nlm_GraphiC) sb, FALSE);
  }
  sb = Nlm_GetTextHScrollBar ((Nlm_TexT) t);
  if (sb != NULL) {
    delta = Nlm_DoGetValue ((Nlm_GraphiC) sb);
    width = Nlm_stdCharWidth;
    TEScroll (delta * width, 0, h);
    Nlm_DoReset ((Nlm_GraphiC) sb, FALSE);
  }
  Nlm_RestorePort (tempPort);
#endif
#ifdef WIN_MSWIN
  Nlm_TextTool  h;

  h = Nlm_GetTextHandle ((Nlm_TexT) t);
  allowTextCallback = FALSE;
  SetWindowText (h, "");
  allowTextCallback = TRUE;
#endif
#ifdef WIN_MOTIF
  Nlm_TextTool  h = Nlm_GetTextHandle ((Nlm_TexT) t);

  allowTextCallback = FALSE;
  XmTextSetString (h, NULL);
  XmTextShowPosition (h, 0);
  allowTextCallback = TRUE;
#endif
}

static void Nlm_RemoveText (Nlm_GraphiC t, Nlm_Boolean savePort)

{
  Nlm_TextTool  h;
  Nlm_WindoW    tempPort;
#ifdef WIN_MAC
  Nlm_BaR       sb;
#endif

  tempPort = Nlm_SavePortIfNeeded (t, savePort);
  if (currentText == (Nlm_TexT) t) {
    currentText = NULL;
  }
  h = Nlm_GetTextHandle ((Nlm_TexT) t);
#ifdef WIN_MAC
  TEDispose (h);
  sb = Nlm_GetTextVScrollBar ((Nlm_TexT) t);
  if (sb != NULL) {
    Nlm_DoRemove ((Nlm_GraphiC) sb, FALSE);
  }
  sb = Nlm_GetTextHScrollBar ((Nlm_TexT) t);
  if (sb != NULL) {
    Nlm_DoRemove ((Nlm_GraphiC) sb, FALSE);
  }
#endif
#ifdef WIN_MSWIN
  RemoveProp (h, (LPSTR) "Nlm_VibrantProp");
  DestroyWindow (h);
#endif
#ifdef WIN_MOTIF
  XtDestroyWidget (h);
#endif
  Nlm_RemoveLink (t);
  recentText = NULL;
  Nlm_RestorePort (tempPort);
}

static void Nlm_TextSelectProc (Nlm_GraphiC t, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  Nlm_Int2      end;
  Nlm_TextTool  h;
  TEPtr         hp;
  Nlm_WindoW    tempPort;

  if (t != NULL) {
    tempPort = Nlm_SavePortIfNeeded (t, savePort);
    h = Nlm_GetTextHandle ((Nlm_TexT) t);
    HLock ((Handle) h);
    hp = (TEPtr) *((Handle) h);
    end = hp->teLength;
    HUnlock ((Handle) h);
    Nlm_SelectAText ((Nlm_TexT) t, 0, end);
    Nlm_RestorePort (tempPort);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_WindoW  tempPort;

  if (t != NULL) {
    size_t len;
    tempPort = Nlm_SavePortIfNeeded (t, savePort);
    len = Nlm_TextLength ((Nlm_TexT) t);
    Nlm_SelectAText ((Nlm_TexT) t, 0, len);
    Nlm_RestorePort (tempPort);
  }
#endif
#ifdef WIN_MOTIF
  Nlm_WindoW  tempPort;

  if (t != NULL) {
    size_t len;
    tempPort = Nlm_SavePortIfNeeded (t, savePort);
    len = Nlm_TextLength ((Nlm_TexT) t);
    Nlm_SelectAText ((Nlm_TexT) t, 0, len);
    Nlm_RestorePort (tempPort);
  }
#endif
}

extern size_t Nlm_TextLength (Nlm_TexT t)

{
  Nlm_TextTool  h;
  size_t        len;
#ifdef WIN_MAC
  TEPtr         hp;
#endif
#ifdef WIN_MOTIF
  Nlm_CharPtr   ptr;
#endif

  len = 0;
  if (t != NULL) {
    h = Nlm_GetTextHandle (t);
#ifdef WIN_MAC
    HLock ((Handle) h);
    hp = (TEPtr) *((Handle) h);
    len = hp->teLength;
    HUnlock ((Handle) h);
#endif
#ifdef WIN_MSWIN
    len = (size_t) GetWindowTextLength (h);
#endif
#ifdef WIN_MOTIF
    ptr = XmTextGetString (h);
    if ( ptr )
      {
        len = Nlm_StringLen (ptr);
        XtFree (ptr);
      }
#endif
  }
  return len;
}


extern void Nlm_SelectText(Nlm_TexT t, Nlm_Int4 begin, Nlm_Int4 end)
{
  Nlm_WindoW tempPort;
  if ( !t )
    return;

  tempPort = Nlm_SavePortIfNeeded((Nlm_GraphiC)t, TRUE);
  if (begin < 0)
    begin = 0;
  if (end < 0)
    end = 0;
  Nlm_SelectAText((Nlm_TexT)t, begin, end);
  Nlm_RestorePort( tempPort );
}


static void Nlm_SetDialogText (Nlm_GraphiC t, Nlm_Int2 item,
                               Nlm_CharPtr title, Nlm_Boolean savePort)

{
  Nlm_TextTool  h;
  Nlm_WindoW    tempPort;
#ifdef WIN_MAC
  Nlm_Uint4     len;
  Nlm_RecT      r;
#endif
#ifdef WIN_MOTIF
  Nlm_CharPtr   str;
#endif

  tempPort = Nlm_SavePortIfNeeded (t, savePort);
  h = Nlm_GetTextHandle ((Nlm_TexT) t);
#ifdef WIN_MAC
  len = Nlm_StringLen (title);
  TESetText (title, len, h);
  TECalText (h);
  Nlm_GetRect (t, &r);
  if (Nlm_GetVisible (t) && Nlm_GetAllParentsVisible (t)) {
    Nlm_InsetRect (&r, 1, 1);
    Nlm_InvalRect (&r);
  }
  Nlm_UpdateScrollBar ((Nlm_TexT) t);
#endif
#ifdef WIN_MSWIN
  allowTextCallback = FALSE;
  SetWindowText (h, title);
  allowTextCallback = TRUE;
#endif
#ifdef WIN_MOTIF
  allowTextCallback = FALSE;
  str = Nlm_StringSave (title);
  XmTextSetString (h, str);
  Nlm_MemFree (str);
  XmTextShowPosition (h, 0);
  allowTextCallback = TRUE;
#endif
  Nlm_RestorePort (tempPort);
}


static void Nlm_SetPasswordText (Nlm_GraphiC t, Nlm_Int2 item,
                                 Nlm_CharPtr title, Nlm_Boolean savePort)
{
  Nlm_TextTool  h        = Nlm_GetTextHandle( (Nlm_TexT)t );
  Nlm_WindoW    tempPort = Nlm_SavePortIfNeeded(t, savePort);
  Nlm_CharPtr   actual_title;
  size_t        len;

  if (title == NULL)
    title = "";
  Nlm_SetPassword((Nlm_TexT)t, title);
  len = Nlm_GetPasswordLength( (Nlm_TexT)t );
  actual_title = (Nlm_CharPtr) Nlm_MemNew(len + 1);
  Nlm_MemSet(actual_title, '*', len);

#ifdef WIN_MAC
  TESetText (actual_title, len, h);
  TECalText (h);
  if (Nlm_GetVisible (t) && Nlm_GetAllParentsVisible (t)) {
    Nlm_RecT r;
    Nlm_GetRect (t, &r);
    Nlm_InsetRect (&r, 1, 1);
    Nlm_InvalRect (&r);
  }
  Nlm_UpdateScrollBar ((Nlm_TexT) t);
#endif
#ifdef WIN_MSWIN
  allowTextCallback = FALSE;
  SetWindowText (h, actual_title);
  allowTextCallback = TRUE;
#endif
#ifdef WIN_MOTIF
  allowTextCallback = FALSE;
  XmTextSetString    (h, (*actual_title ? actual_title : 0));
  XmTextShowPosition         (h, len);
  XmTextSetInsertionPosition (h, len);
  allowTextCallback = TRUE;
#endif

  Nlm_MemFree( actual_title );
  Nlm_RestorePort( tempPort );
}


static void Nlm_SetScrollText (Nlm_GraphiC t, Nlm_Int2 item,
                               Nlm_CharPtr title, Nlm_Boolean savePort)

{
  Nlm_TextTool  h;
  Nlm_WindoW    tempPort;
#ifdef WIN_MAC
  Nlm_Uint4     len;
  Nlm_RecT      r;
#endif
#ifdef WIN_MSWIN
  Nlm_Uint4	count;
  Nlm_Uint4     len;
  Nlm_CharPtr	tmp, newTitle;
#endif

  tempPort = Nlm_SavePortIfNeeded (t, savePort);
  h = Nlm_GetTextHandle ((Nlm_TexT) t);
#ifdef WIN_MAC
  len = Nlm_StringLen (title);
  TESetText (title, len, h);
  TECalText (h);
  Nlm_GetRect (t, &r);
  Nlm_InsetRect (&r, 4, 2);
  r.bottom = r.top + Nlm_GetVisLines ((Nlm_TexT) t) * Nlm_GetFontHeight ((Nlm_TexT) t);
  if (Nlm_GetVisible (t) && Nlm_GetAllParentsVisible (t)) {
    Nlm_InsetRect (&r, -1, -1);
    Nlm_InvalRect (&r);
  }
  Nlm_UpdateScrollBar ((Nlm_TexT) t);
#endif
#ifdef WIN_MSWIN
  allowTextCallback = FALSE;
  
  /* count number of newlines */
  tmp = title;
  count = 0;
  for (tmp = title; tmp != NULL && *tmp; ++tmp) {
      if (*tmp == '\n')
	  ++count;
  }

  if (count == 0)
    SetWindowText (h, title);
  else { /* replace UNIX <lf> with DOS <cr><lf> */
    len = Nlm_StringLen (title);
    newTitle = (Nlm_CharPtr) MemNew(len+count+1);
    tmp = newTitle;
    for (count=0; count < len; ++count) {
	if (title[count] == '\n' && (count == 0 || title[count-1] != '\r'))
	    *tmp++ = '\r';
	*tmp++ = title[count];
    }
    *tmp = '\0';
    SetWindowText (h, newTitle);
    MemFree(newTitle);
  }

  allowTextCallback = TRUE;
#endif
#ifdef WIN_MOTIF
  allowTextCallback = FALSE;
  /*
     The gcc compiler puts static strings in read only memory.  Motif 1.1 dies
     at this point for text objects (Dialog, Hidden and Password) that use the
     modifyVerify callback.  In those cases, the strings are copied.
  */
  XmTextSetString (h, title);
  allowTextCallback = TRUE;
#endif
  Nlm_RestorePort (tempPort);
}

static void Nlm_GetDialogText (Nlm_GraphiC t, Nlm_Int2 item,
                               Nlm_CharPtr title, size_t maxsize)

{
  Nlm_TextTool  h;
#ifdef WIN_MAC
  Nlm_Char      **chars;
  TEPtr         hp;
  Nlm_Int2      i;
  Nlm_Int2      length;
  Nlm_Char      *ptr;
#endif
#ifdef WIN_MOTIF
  Nlm_CharPtr   ptr;
#endif

  if (title != NULL && maxsize > 0) {
    h = Nlm_GetTextHandle ((Nlm_TexT) t);
#ifdef WIN_MAC
    i = 0;
    HLock ((Handle) h);
    hp = (TEPtr) *((Handle) h);
    chars = hp->hText;
    length = hp->teLength;
    if (length > maxsize - 1) {
      length = maxsize - 1;
    }
    HUnlock ((Handle) h);
    if (chars != NULL) {
      HLock ((Handle) chars);
      ptr = (Nlm_Char *) *((Handle) chars);
      Nlm_StringNCpy (title, ptr, length); /* remains StringNCpy, not _0 */
      title [length] = '\0';
      HUnlock ((Handle) chars);
    }
#endif
#ifdef WIN_MSWIN
    GetWindowText (h, title, maxsize);
/*    title [maxsize - 1] = '\0';*/
#endif
#ifdef WIN_MOTIF
    ptr = XmTextGetString (h);
    Nlm_StringNCpy_0(title, ptr, maxsize);
    XtFree (ptr);
#endif
  }
}

static void Nlm_GetPasswordText (Nlm_GraphiC t, Nlm_Int2 item,
                                 Nlm_CharPtr title, size_t maxsize)

{
  Nlm_GetPassword ((Nlm_TexT) t, title, maxsize);
}

static void Nlm_GetScrollText (Nlm_GraphiC t, Nlm_Int2 item,
                               Nlm_CharPtr title, size_t maxsize)

{
  Nlm_TextTool  h;
#ifdef WIN_MAC
  Nlm_Char      **chars;
  TEPtr         hp;
  Nlm_Int2      i;
  Nlm_Int2      length;
  Nlm_Char      *ptr;
#endif
#ifdef WIN_MOTIF
  Nlm_CharPtr   ptr;
#endif

  if (title != NULL && maxsize > 0) {
    h = Nlm_GetTextHandle ((Nlm_TexT) t);
#ifdef WIN_MAC
    i = 0;
    HLock ((Handle) h);
    hp = (TEPtr) *((Handle) h);
    chars = hp->hText;
    length = hp->teLength;
    if (length > maxsize - 1) {
      length = maxsize - 1;
    }
    HUnlock ((Handle) h);
    if (chars != NULL) {
      HLock ((Handle) chars);
      ptr = (Nlm_Char *) *((Handle) chars);
      Nlm_StringNCpy (title, ptr, length); /* remains StringNCpy, not _0 */
      title [length] = '\0';
      HUnlock ((Handle) chars);
    }
#endif
#ifdef WIN_MSWIN
    GetWindowText (h, title, maxsize);
/*    title [maxsize - 1] = '\0'; */
#endif
#ifdef WIN_MOTIF
    ptr = XmTextGetString (h);
    Nlm_StringNCpy_0(title, ptr, maxsize);
    XtFree (ptr);
#endif
  }
}

extern Nlm_TexT Nlm_CurrentText (void)

{
  return currentText;
}

#ifdef WIN_MAC

static void Clipboard_TEToDeskScrap()
{
	OSErr err;
	/* Copy the TE scrap to the desk scrap. */
	err = TEToScrap();
}

static void Clipboard_TECut(TEHandle inTE)
{
	/* Cut the text into the TE scrap. */
	TECut(inTE);
	/* Update the desk scrap. */
	Clipboard_TEToDeskScrap();
}

static void Clipboard_TECopy(TEHandle inTE)
{
	/* Copy the text into the TE scrap. */
	TECopy(inTE);
	/* Update the desk scrap. */
	Clipboard_TEToDeskScrap();
}

static void Clipboard_TEPaste(TEHandle inTE)
{
    /* #if TARGET_API_MAC_CARBON */
	/* ScrapRef scrap; */
	/* OSStatus status = GetCurrentScrap(&scrap); */
	OSErr err = TEFromScrap();
	TEPaste(inTE);
}

#endif  /* WIN_MAC */

extern void Nlm_CutText (Nlm_TexT t)

{
  Nlm_TextTool  h;

  if (t != NULL) {
    h = Nlm_GetTextHandle (t);
#ifdef WIN_MAC
    Clipboard_TECut(h);
#endif
#ifdef WIN_MSWIN
    allowTextCallback = FALSE;
    SendMessage (h, WM_CUT, 0, 0);
    allowTextCallback = TRUE;
#endif
#ifdef WIN_MOTIF
    allowTextCallback = FALSE;
    XmTextCut (h, CurrentTime);
    allowTextCallback = TRUE;
#endif
    Nlm_DoAction ((Nlm_GraphiC) t);
    Nlm_textScrapFull = TRUE;
  }
}

extern void Nlm_CopyText (Nlm_TexT t)

{
  Nlm_TextTool  h;

  if (t != NULL) {
    h = Nlm_GetTextHandle (t);
#ifdef WIN_MAC
    Clipboard_TECopy(h);
#endif
#ifdef WIN_MSWIN
    allowTextCallback = FALSE;
    SendMessage (h, WM_COPY, 0, 0);
    allowTextCallback = TRUE;
#endif
#ifdef WIN_MOTIF
    allowTextCallback = FALSE;
    XmTextCopy (h, CurrentTime);
    allowTextCallback = TRUE;
#endif
    Nlm_textScrapFull = TRUE;
  }
}

extern void Nlm_PasteText (Nlm_TexT t)

{
  Nlm_TextTool  h;

  if (t != NULL) {
    if (! Nlm_GetTextEditable (t)) return;
    h = Nlm_GetTextHandle (t);
#ifdef WIN_MAC
    Clipboard_TEPaste(h);
#endif
#ifdef WIN_MSWIN
    allowTextCallback = FALSE;
    SendMessage (h, WM_PASTE, 0, 0);
    allowTextCallback = TRUE;
#endif
#ifdef WIN_MOTIF
    allowTextCallback = FALSE;
    XmTextPaste (h);
    allowTextCallback = TRUE;
#endif
    Nlm_DoAction ((Nlm_GraphiC) t);
  }
}

extern void Nlm_ClearText (Nlm_TexT t)

{
  Nlm_TextTool  h;

  if (t != NULL) {
    if (! Nlm_GetTextEditable (t)) return;
    h = Nlm_GetTextHandle (t);
#ifdef WIN_MAC
    TEDelete (h);
#endif
#ifdef WIN_MSWIN
    allowTextCallback = FALSE;
    SendMessage (h, WM_CLEAR, 0, 0);
    allowTextCallback = TRUE;
#endif
#ifdef WIN_MOTIF
    allowTextCallback = FALSE;
    XmTextClearSelection (h, CurrentTime);
    allowTextCallback = TRUE;
#endif
    Nlm_DoAction ((Nlm_GraphiC) t);
    Nlm_textScrapFull = TRUE;
  }
}

extern Nlm_TexT Nlm_CurrentVisibleText (void)

{
  Nlm_TexT  t;

  t = Nlm_CurrentText ();
  if (t != NULL && Nlm_Visible (t) && Nlm_AllParentsVisible (t)) {
    return t;
  } else {
    return NULL;
  }
}

extern void Nlm_StdCutTextProc (Nlm_IteM i)

{
  Nlm_CutText (Nlm_CurrentVisibleText ());
}

extern void Nlm_StdCopyTextProc (Nlm_IteM i)

{
  Nlm_CopyText (Nlm_CurrentVisibleText ());
}

extern void Nlm_StdPasteTextProc (Nlm_IteM i)

{
  Nlm_PasteText (Nlm_CurrentVisibleText ());
}

extern void Nlm_StdDeleteTextProc (Nlm_IteM i)

{
  Nlm_ClearText (Nlm_CurrentVisibleText ());
}

#ifdef WIN_MSWIN
/* Message cracker functions */

static void MyCls_OnChar (HWND hwnd, UINT ch, int cRepeat)

{
  Nlm_Boolean  ishidden;
  Nlm_Boolean  iseditable;
  Nlm_TexT     t;
  Nlm_TextTool h;

  t = (Nlm_TexT) GetProp (hwnd, (LPSTR) "Nlm_VibrantProp");
  h = Nlm_GetTextHandle(t);
  handlechar = FALSE;
  ishidden = Nlm_IsHiddenOrSpecialText ((Nlm_TexT) t);
  iseditable = Nlm_GetTextEditable(t);
  if (ch == '\t' && ishidden) {
    Nlm_DoTabCallback (t);
  } else if (ch == '\1') {
    /* control-A should select all text */
    if (h) {
      SNDMSG(h, EM_SETSEL, (WPARAM)(0), (LPARAM)(MAKELONG(0, 0xffff)));
    }
  } else if (ch == '\t'  && !Nlm_ctrlKey && iseditable && (GetWindowLong(hwnd, GWL_STYLE) & ES_MULTILINE)) {
    /* editable multiline boxes accept tab as input character not as focus change to stay 
       consistent with behavior on MOTIF */
    handlechar = TRUE;
  } else if (ch == '\t') {
    Nlm_DoSendFocus ((Nlm_GraphiC) t, (Nlm_Char) ch);
  } else if ((ch == '\n' || ch == '\r') && ishidden) {
    Nlm_DoReturnCallback (t);
  } else if ((ch == '\n' || ch == '\r') && iseditable) {
    if (GetWindowLong(hwnd, GWL_STYLE) & ES_MULTILINE) {
	/* multiline edit box */
	if (Nlm_ctrlKey)
	    /* Ctrl-Enter goes to dialog box buttons */
    	    Nlm_DoSendFocus ((Nlm_GraphiC) t, (Nlm_Char) ch);
	else
	    /* plain Enter goes as character to the edit field */
	    handlechar = TRUE;
    }
    else
	Nlm_DoSendFocus ((Nlm_GraphiC) t, (Nlm_Char) ch);
  } else if (!iseditable && ch == '\3') { /* pass Ctrl-C ("Copy") to default handler */
    handlechar = TRUE;
  } else if (iseditable) {
    handlechar = TRUE;
  }
}

static void MyCls_OnSetFocus (HWND hwnd, HWND hwndOldFocus)

{
  Nlm_TexT  t;

  t = (Nlm_TexT) GetProp (hwnd, (LPSTR) "Nlm_VibrantProp");
  Nlm_SetActive (t, TRUE);
  Nlm_DoTextSelect (t);
}

static void MyCls_OnKillFocus (HWND hwnd, HWND hwndNewFocus)

{
  Nlm_TexT  t;

  t = (Nlm_TexT) GetProp (hwnd, (LPSTR) "Nlm_VibrantProp");
  if (Nlm_GetActive (t)) {
    Nlm_DoTextDeselect (t);
  }
  /*
  currentText = NULL;
  */
}

LRESULT CALLBACK EXPORT TextProc (HWND hwnd, UINT message,
                                  WPARAM wParam, LPARAM lParam)

{
  Nlm_TexT  t;
  BOOL      call_win_proc = TRUE;
  LRESULT   rsult = 0;
  HDC       tempHDC;
  HWND      tempHWnd;

  if ( Nlm_VibrantDisabled() )
    return CallWindowProc (lpfnOldTextProc, hwnd, message, wParam, lParam);

  tempHWnd = Nlm_currentHWnd;
  tempHDC = Nlm_currentHDC;
  t = (Nlm_TexT) GetProp (hwnd, (LPSTR) "Nlm_VibrantProp");
  Nlm_theWindow = Nlm_GetParentWindow ((Nlm_GraphiC) t);
  Nlm_currentHWnd = GetParent (hwnd);
  Nlm_currentHDC = Nlm_ParentWindowPort ((Nlm_GraphiC) t);
  Nlm_currentWindowTool = hwnd;
  Nlm_currentKey = '\0';
  Nlm_currentWParam = wParam;
  Nlm_currentLParam = lParam;
  Nlm_cmmdKey = FALSE;
  Nlm_optKey = FALSE;
  Nlm_ctrlKey = (Nlm_Boolean) ((GetKeyState (VK_CONTROL) & 0x8000) != 0);
  Nlm_shftKey = (Nlm_Boolean) ((GetKeyState (VK_SHIFT)   & 0x8000) != 0);
  Nlm_dblClick = FALSE;

  switch ( message ) {
    case WM_KEYDOWN:
      if(!Nlm_GetTextEditable(t) && Nlm_KeydownToChar( wParam ) == NLM_DEL)
      {
	  call_win_proc = FALSE;
	  break;
      }
      call_win_proc = !(Nlm_GetVisLines(t) == 1  &&
                        Nlm_ProcessKeydown((Nlm_GraphiC)t,
                                           wParam, VERT_PAGE|VERT_ARROW));
      break;
    case WM_CHAR:
      HANDLE_WM_CHAR(hwnd, wParam, lParam, MyCls_OnChar);
      call_win_proc = handlechar;
      break;
    case WM_SETFOCUS:
      HANDLE_WM_SETFOCUS (hwnd, wParam, lParam, MyCls_OnSetFocus);
      break;
    case WM_KILLFOCUS:
      HANDLE_WM_KILLFOCUS (hwnd, wParam, lParam, MyCls_OnKillFocus);
      break;
    case WM_PASTE:
      if ( !Nlm_GetTextEditable(t) )
      {
	  call_win_proc = FALSE;
	  break;
      }
      if ( !(GetWindowLong(hwnd, GWL_STYLE) & ES_MULTILINE) ) {
        LPSTR text = NULL, str;
        HANDLE h_text;
        VERIFY ( OpenClipboard(hwnd) );
        if (IsClipboardFormatAvailable(CF_TEXT)  &&
            (h_text = GetClipboardData(CF_TEXT)) != NULL) {
          str = GlobalLock(h_text);
          if (str  &&  *str  &&  (text = malloc(strlen(str)+1)) != NULL)
            strcpy(text, str);
          GlobalUnlock(h_text);
        }
        VERIFY ( CloseClipboard() );
        if ( !text )
          break;  /* no suitable data found in the clipboard */

        for (str = text;  *str;  str++)
          if ( !isprint(*str) ) {
            *str = ' ';
            call_win_proc = FALSE;
          }
        if ( !call_win_proc ) { /* the pasted text contains non-print. chars */
          Edit_ReplaceSel(hwnd, text);
        }
        free(text);
      }
  } /* switch (message) */

  if ( call_win_proc )
    rsult = CallWindowProc(lpfnOldTextProc, hwnd, message, wParam, lParam);

  Nlm_currentHWnd = tempHWnd;
  Nlm_currentHDC = tempHDC;
  Nlm_currentWindowTool = tempHWnd;
  return rsult;
}
#endif

static Nlm_GraphiC Nlm_TextGainFocus (Nlm_GraphiC t, Nlm_Char ch, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  Nlm_TextTool  h;
  TEPtr         hp;
  Nlm_Int2      len;
  Nlm_GraphiC   rsult;

  rsult = NULL;
  if (ch == '\t' && Nlm_GetVisible (t) && Nlm_GetEnabled (t)) {
    h = Nlm_GetTextHandle ((Nlm_TexT) t);
    HLock ((Handle) h);
    hp = (TEPtr) *((Handle) h);
    len = hp->teLength;
    HUnlock ((Handle) h);
    Nlm_SetActive ((Nlm_TexT) t, TRUE);
    Nlm_SelectAText ((Nlm_TexT) t, 0, len);
    rsult = t;
  }
  return rsult;
#endif
#ifdef WIN_MSWIN
  Nlm_GraphiC   rsult;

  rsult = NULL;
  if (ch == '\t' && Nlm_GetVisible (t) && Nlm_GetEnabled (t)) {
    Nlm_TextTool h = Nlm_GetTextHandle ((Nlm_TexT) t);
    Nlm_SetActive ((Nlm_TexT) t, TRUE);
    if (GetWindowLong(h, GWL_STYLE) & ES_MULTILINE) {
	/* multiline edit box */
	Nlm_Int4 begin = 0, end = 0;
	Nlm_TextSelectionRange((Nlm_TexT) t, &begin, &end);
	Nlm_SelectAText ((Nlm_TexT) t, begin, end);
    } else {
        /* singleline edit box - make it consistent with MOTIF by setting len = 0 
           which just sets the cursor pos to the beginning and does not highlight 
           the text
        */
        size_t len = 0;  /* Nlm_TextLength ((Nlm_TexT) t); */
	Nlm_SelectAText ((Nlm_TexT) t, 0, len);
    }
    rsult = t;
  }
  return rsult;
#endif
#ifdef WIN_MOTIF
  Nlm_GraphiC   rsult;

  rsult = NULL;
  if (ch == '\t' && Nlm_GetVisible (t) && Nlm_GetEnabled (t)) {
    size_t len;
    Nlm_SetActive ((Nlm_TexT) t, TRUE);
    len = Nlm_TextLength ((Nlm_TexT) t);
    Nlm_SelectAText ((Nlm_TexT) t, 0, len);
    rsult = t;
  }
  return rsult;
#endif
}

static void Nlm_TextLoseFocus (Nlm_GraphiC t, Nlm_GraphiC excpt, Nlm_Boolean savePort)

{
  Nlm_WindoW  tempPort;

  if (t != excpt) {
    if (Nlm_GetActive ((Nlm_TexT) t)) {
      Nlm_DoTextDeselect ((Nlm_TexT) t);
    }
    Nlm_SetActive ((Nlm_TexT) t, FALSE);
    tempPort = Nlm_SavePortIfNeeded (t, savePort);
    Nlm_DeactivateText (t, FALSE);
    Nlm_RestorePort (tempPort);
  }
}

#ifdef WIN_MAC
static void Nlm_InvalText (Nlm_GraphiC t)

{
  Nlm_RecT  r;

  if (Nlm_GetVisible (t) && Nlm_GetAllParentsVisible (t)) {
    Nlm_GetRect (t, &r);
    Nlm_InsetRect (&r, -1, -1);
    Nlm_InvalRect (&r);
  }
}

static void Nlm_InvalScrollText (Nlm_GraphiC t)

{
  Nlm_RecT  r;

  if (Nlm_GetVisible (t) && Nlm_GetAllParentsVisible (t)) {
    Nlm_GetRect (t, &r);
    r.right += Nlm_vScrollBarWidth;
    if (! Nlm_GetTextWrap ((Nlm_TexT) t)) {
      r.bottom += Nlm_hScrollBarHeight;
    }
    Nlm_InsetRect (&r, -1, -1);
    Nlm_InvalRect (&r);
  }
}
#endif


static void Nlm_SetTextPosition (Nlm_GraphiC t, Nlm_RectPtr r,
				 Nlm_Boolean savePort, Nlm_Boolean force)
{
  Nlm_TextTool  h;
  Nlm_RecT      oldRect;
  Nlm_WindoW    tempPort;
  Nlm_RecT      tr;
#ifdef WIN_MAC
  TEPtr         hp;
  Nlm_RectTool  rtool;
#endif

  if (r == NULL)  return;

  if ( !Nlm_GetRealized( t ) )
    {
      Nlm_SetRect (t, r);
      return;
    }

  Nlm_DoGetPosition (t, &oldRect);
  if (!force  &&  Nlm_EqualRect(r, &oldRect))  return;

  tempPort = Nlm_SavePortIfNeeded (t, savePort);
  h = Nlm_GetTextHandle ((Nlm_TexT) t);
#ifdef WIN_MAC
  Nlm_InvalText (t);
#endif
  tr = *r;
#ifdef WIN_MAC
  Nlm_InsetRect (&tr, 2, 2);
  Nlm_RecTToRectTool (&tr, &rtool);
  HLock ((Handle) h);
  hp = (TEPtr) *((Handle) h);
  hp->destRect = rtool;
  hp->viewRect = rtool;
  HUnlock ((Handle) h);
  Nlm_SetRect (t, r);
  Nlm_InvalText (t);
#endif
#ifdef WIN_MSWIN
  MoveWindow (h, tr.left, tr.top, tr.right - tr.left, tr.bottom - tr.top, TRUE);
  Nlm_SetRect (t, r);
  UpdateWindow (h);
#endif
#ifdef WIN_MOTIF
  allowTextCallback = FALSE;
  XtVaSetValues (h,
		 XmNx, (Position) (tr.left + 1),
		 XmNy, (Position) (tr.top  + 1),
		 XmNwidth,  (Dimension) (tr.right - tr.left),
		 XmNheight, (Dimension) (tr.bottom - tr.top), 
		 NULL);
  Nlm_SetRect (t, r);
  allowTextCallback = TRUE;
#endif

  Nlm_RestorePort (tempPort);
}


static void Nlm_SetHiddenTextPosition (Nlm_GraphiC t, Nlm_RectPtr r,
				       Nlm_Boolean savePort, Nlm_Boolean force)
{
  Nlm_TextTool  h;
  Nlm_RecT      oldRect;
  Nlm_WindoW    tempPort;
  Nlm_RecT      tr;
#ifdef WIN_MAC
  TEPtr         hp;
  Nlm_RectTool  rtool;
#endif

  if (r == NULL)  return;

  if ( !Nlm_GetRealized( t ) )
    {
      Nlm_SetRect (t, r);
      return;
    }

  Nlm_DoGetPosition (t, &oldRect);
  if (!force  &&  Nlm_EqualRect(r, &oldRect))  return;

  tempPort = Nlm_SavePortIfNeeded (t, savePort);
  h = Nlm_GetTextHandle ((Nlm_TexT) t);
#ifdef WIN_MAC
  Nlm_InvalText (t);
#endif
  tr = *r;
#ifdef WIN_MAC
  Nlm_RecTToRectTool (&tr, &rtool);
  HLock ((Handle) h);
  hp = (TEPtr) *((Handle) h);
  hp->destRect = rtool;
  hp->viewRect = rtool;
  HUnlock ((Handle) h);
  Nlm_SetRect (t, r);
  Nlm_InvalText (t);
#endif
#ifdef WIN_MSWIN
  MoveWindow (h, tr.left, tr.top, tr.right - tr.left, tr.bottom - tr.top, TRUE);
  Nlm_SetRect (t, r);
  UpdateWindow (h);
#endif
#ifdef WIN_MOTIF
  allowTextCallback = FALSE;
  XtVaSetValues (h,
		 XmNx, (Position) tr.left + 1,
		 XmNy, (Position) tr.top  + 1,
		 XmNwidth,  (Dimension) (tr.right - tr.left),
		 XmNheight, (Dimension) (tr.bottom - tr.top), 
		 NULL);
  Nlm_SetRect (t, r);
  allowTextCallback = TRUE;
#endif

  Nlm_RestorePort (tempPort);
}


static void Nlm_ResetVisLines (Nlm_TexT t)

{
  Nlm_RecT      r;
  Nlm_TextData  tdata;

  Nlm_GetTextData (t, &tdata);
  Nlm_GetRect ((Nlm_GraphiC) t, &r);
  Nlm_InsetRect (&r, 4, 2);
  if (tdata.texthght > 0) {
    tdata.visLines = (Nlm_Int2)((r.bottom - r.top + 1) / tdata.texthght);
  } else {
    tdata.visLines = 0;
  }
  Nlm_SetTextData (t, &tdata);
}


static void Nlm_SetScrollTextOffset (Nlm_GraphiC t, Nlm_Int2 horiz,
                                     Nlm_Int2 vert, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  Nlm_BaR  sb;

  sb = Nlm_GetTextVScrollBar ((Nlm_TexT) t);
  if (sb != NULL) {
    Nlm_DoSetValue ((Nlm_GraphiC) sb, vert, savePort);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_TextTool  h;

  h = Nlm_GetTextHandle ((Nlm_TexT) t);
  PostMessage (h, WM_VSCROLL, MAKEWPARAM (SB_THUMBPOSITION, vert), 0L);


#endif
#ifdef WIN_MOTIF
  Nlm_Int4 value, slider_size, increment, page_increment;
  Nlm_TextTool  h;
  Widget        scrolled_window;
  Widget        vscroll;

  h = Nlm_GetTextHandle ((Nlm_TexT) t);
  value = 0;
  slider_size = 0;
  increment = 0;
  page_increment = 0;
  vscroll = NULL;

  scrolled_window = XtParent (h);
  if (scrolled_window == NULL) return;

  XtVaGetValues (scrolled_window, XmNverticalScrollBar, &vscroll, NULL);
  if (vscroll == NULL) return;

  XmScrollBarGetValues (vscroll, &value, &slider_size, &increment, &page_increment);
  XmScrollBarSetValues (vscroll, vert, slider_size, increment, page_increment, TRUE);

#endif
}

static void Nlm_GetScrollTextOffset (Nlm_GraphiC t, Nlm_Int2Ptr horiz, Nlm_Int2Ptr vert)

{
#ifdef WIN_MAC
  Nlm_BaR   sb;
  Nlm_Int2  rsult;

  sb = Nlm_GetTextVScrollBar ((Nlm_TexT) t);
  rsult = 0;
  if (sb != NULL) {
    rsult = Nlm_DoGetValue ((Nlm_GraphiC) sb);
  }
  if (vert != NULL) {
    *vert = rsult;
  }
  if (horiz != NULL) {
    *horiz = 0;
  }
#endif
#ifdef WIN_MSWIN
  Nlm_TextTool  h;

  h = Nlm_GetTextHandle ((Nlm_TexT) t);
  if (vert != NULL) {
    *vert = (Nlm_Int2) GetScrollPos (h, SB_VERT);
  }
  if (horiz != NULL) {
    *horiz = 0;
  }
#endif
#ifdef WIN_MOTIF
  if (vert != NULL) {
    *vert = 0;
  }
  if (horiz != NULL) {
    *horiz = 0;
  }
#endif
}

static void Nlm_SetScrollTextPosition (Nlm_GraphiC t, Nlm_RectPtr r,
				       Nlm_Boolean savePort, Nlm_Boolean force)
{
  Nlm_TextTool  h;
  Nlm_RecT      oldRect;
  Nlm_WindoW    tempPort;
  Nlm_RecT      tr;
  Nlm_Boolean   wrap;
#ifdef WIN_MAC
  Nlm_Int2      deltax;
  Nlm_Int2      deltay;
  Nlm_RectTool  dtool;
  TEPtr         hp;
  Nlm_RectTool  rtool;
  Nlm_BaR       sb;
#endif
  Nlm_Boolean   is_realized;

  if (r == NULL)  return;

  Nlm_DoGetPosition (t, &oldRect);
  if (!force  &&  Nlm_EqualRect(r, &oldRect))  return;

  h = Nlm_GetTextHandle ((Nlm_TexT) t);
  if (h == NULL)  return;

  tr = *r;
  wrap = Nlm_GetTextWrap ((Nlm_TexT) t);
  is_realized = Nlm_GetRealized( t );
  if (is_realized)
    {
      tempPort = Nlm_SavePortIfNeeded (t, savePort);
#ifdef WIN_MAC
      Nlm_InvalScrollText (t);
#endif
#ifdef WIN_MSWIN
      MoveWindow (h,  tr.left,  tr.top,
		  tr.right - tr.left,  tr.bottom - tr.top,  TRUE);
#endif
#ifdef WIN_MOTIF
      allowTextCallback = FALSE;
      XtVaSetValues (XtParent (h),
		     XmNx, (Position) tr.left,
		     XmNy, (Position) tr.top,
		     XmNwidth,  (Dimension)(tr.right - tr.left),
		     XmNheight, (Dimension)(tr.bottom - tr.top),
		     NULL);
#endif
    }

  tr.right -= Nlm_vScrollBarWidth;
  if ( !wrap )
    tr.bottom -= Nlm_hScrollBarHeight;
  Nlm_SetRect (t, &tr);

  if ( is_realized )
    {
#ifdef WIN_MAC
      Nlm_ResetVisLines ((Nlm_TexT) t);
      Nlm_InsetRect (&tr, 4, 2);
      tr.bottom = tr.top + Nlm_GetVisLines ((Nlm_TexT) t) * Nlm_GetFontHeight ((Nlm_TexT) t);
      HLock ((Handle) h);
      hp = (TEPtr) *((Handle) h);
      deltax = hp->destRect.left - hp->viewRect.left;
      deltay = hp->destRect.top - hp->viewRect.top;
      Nlm_RecTToRectTool (&tr, &rtool);
      Nlm_OffsetRect (&tr, deltax, deltay);
      Nlm_RecTToRectTool (&tr, &dtool);
      if (! wrap) {
	dtool.right += HSCROLL_POSITIONS * Nlm_stdCharWidth;
      }
      hp->destRect = dtool;
      hp->viewRect = rtool;
      HUnlock ((Handle) h);
      Nlm_InvalScrollText (t);
    }

  sb = Nlm_GetTextVScrollBar ((Nlm_TexT) t);
  if (sb != NULL) {
    Nlm_GetRect (t, &tr);
    tr.left = tr.right;
    tr.right += Nlm_vScrollBarWidth;
    Nlm_DoSetPosition ((Nlm_GraphiC) sb, &tr, FALSE, force);
  }
  sb = Nlm_GetTextHScrollBar ((Nlm_TexT) t);
  if (sb != NULL) {
    Nlm_GetRect (t, &tr);
    tr.top = tr.bottom;
    tr.bottom += Nlm_hScrollBarHeight;
    Nlm_DoSetPosition ((Nlm_GraphiC) sb, &tr, FALSE, force);
  }

  if ( is_realized )
    {
      Nlm_UpdateScrollBar ((Nlm_TexT) t);
#endif
#ifdef WIN_MSWIN
      Nlm_ResetVisLines ((Nlm_TexT) t);
      UpdateWindow (h);
#endif
#ifdef WIN_MOTIF
      Nlm_ResetVisLines ((Nlm_TexT) t);
      allowTextCallback = TRUE;
#endif

      Nlm_RestorePort (tempPort);
    }
}


static void Nlm_GetTextPosition (Nlm_GraphiC t, Nlm_RectPtr r)

{
  if (r != NULL) {
    Nlm_GetRect (t, r);
  }
}

static void Nlm_GetScrollTextPosition (Nlm_GraphiC t, Nlm_RectPtr r)

{
  if (r != NULL) {
    Nlm_GetRect (t, r);
    r->right += Nlm_vScrollBarWidth;
    if (! Nlm_GetTextWrap ((Nlm_TexT) t)) {
      r->bottom += Nlm_hScrollBarHeight;
    }
  }
}

#ifdef WIN_MAC
static void Nlm_VScrollAction (Nlm_BaR sb, Nlm_GraphiC t,
                               Nlm_Int2 newval, Nlm_Int2 oldval)

{
  Nlm_Int2      delta;
  Nlm_TextTool  h;
  Nlm_Int2      height;
  Nlm_RecT      r;

  h = Nlm_GetTextHandle ((Nlm_TexT) t);
  height = Nlm_GetLineHeight ((Nlm_TexT) t);
  delta = oldval - newval;
  if (oldval != newval) {
    Nlm_SelectFont (Nlm_systemFont);
    TEScroll (0, delta * height, h);
  } else if (Nlm_GetVisible (t) && Nlm_GetAllParentsVisible (t)) {
    Nlm_GetRect (t, &r);
    Nlm_InsetRect (&r, 2, 1);
    Nlm_InvalRect (&r);
  }
  Nlm_Update ();
}

static void Nlm_HScrollAction (Nlm_BaR sb, Nlm_GraphiC t,
                               Nlm_Int2 newval, Nlm_Int2 oldval)

{
  Nlm_Int2      delta;
  Nlm_TextTool  h;
  Nlm_Int2      width;
  Nlm_RecT      r;

  h = Nlm_GetTextHandle ((Nlm_TexT) t);
  width = Nlm_stdCharWidth;
  delta = oldval - newval;
  if (oldval != newval) {
    Nlm_SelectFont (Nlm_systemFont);
    TEScroll (delta * width, 0, h);
  } else if (Nlm_GetVisible (t) && Nlm_GetAllParentsVisible (t)) {
    Nlm_GetRect (t, &r);
    Nlm_InsetRect (&r, 2, 1);
    Nlm_InvalRect (&r);
  }
  Nlm_Update ();
}
#endif

#ifdef WIN_MOTIF
static void Nlm_ReturnCallback(Widget w,
                               XtPointer client_data, XtPointer call_data)
{
  XmAnyCallbackStruct *cbs = (XmAnyCallbackStruct *)call_data;

  if (cbs->event != NULL)
    Nlm_DoSendFocus((Nlm_GraphiC)client_data, '\r');
}


static void Nlm_TextCallback(Widget w,
                             XtPointer client_data, XtPointer call_data)
{
  XmAnyCallbackStruct *cbs = (XmAnyCallbackStruct *)call_data;

  if ( allowTextCallback )
    Nlm_DoAction( (Nlm_GraphiC)client_data );
}


static void Nlm_FocusCallback(Widget w, XtPointer client_data, XtPointer call_data)
{
  Nlm_TexT t = (Nlm_TexT) client_data;
  Nlm_SetActive (t, TRUE);
  Nlm_DoTextSelect (t);
}


static void Nlm_LoseFocusCallback (Widget w, XtPointer client_data, XtPointer call_data)
{
  XmAnyCallbackStruct  *cbs;
  Nlm_TexT             t;

  cbs = (XmAnyCallbackStruct *) call_data;
  t = (Nlm_TexT) client_data;
  if (Nlm_GetActive (t)) {
    Nlm_DoTextDeselect (t);
  }
  /*
  currentText = NULL;
  */
}

static void Nlm_RefreshCallback (Widget w, XtPointer client_data, XtPointer call_data)

{
  XmTextVerifyCallbackStruct  *cbs;
  Nlm_RecT                    r;
  Nlm_TexT                    t;
  Window                      ww;

  cbs = (XmTextVerifyCallbackStruct *) call_data;
  t = (Nlm_TexT) client_data;
  if (cbs->text->ptr == NULL) {
    Nlm_ObjectRect ((Nlm_GraphiC) t, &r);
    Nlm_OffsetRect (&r, -r.left, -r.top);
    if (! Nlm_IsHiddenText (t)) {
      Nlm_InsetRect (&r, 4, 4);
#ifdef OS_VMS
      Nlm_InsetRect (&r, 5, 3);
#else
      Nlm_InsetRect (&r, 5, 5);
#endif
    }
    if (Nlm_currentXDisplay != NULL && (ww = XtWindow(w)) != 0)
    {
        XClearArea (Nlm_currentXDisplay, ww, r.left, r.top,
                    r.right - r.left, r.bottom - r.top, TRUE);
    }
  }
}


static void Nlm_TextFieldCallback(Widget w,
                                  XtPointer client_data, XtPointer call_data)
{
  XmTextVerifyCallbackStruct *cbs = (XmTextVerifyCallbackStruct *)call_data;

  int i;

  /* recalculate the correct length by detecting the null byte */
  for (i = 0;  i < cbs->text->length;  i++) {
    if (cbs->text->ptr[i] == '\0') {
      cbs->text->length = i + 1;
      break;
    }
  }

  /* Don't convert carriage returns and linefeeds into space */
  for (i = 0;  i < cbs->text->length;  i++) {
    if ( !isprint(cbs->text->ptr[i])  &&  !isspace (cbs->text->ptr[i]) )
      cbs->text->ptr[i] = ' ';
  }
}


static void Nlm_PasswordCallback (Widget w,
                                  XtPointer client_data,
                                  XtPointer call_data)
{
  Nlm_GraphiC                 t    = (Nlm_GraphiC) client_data;
  XmTextVerifyCallbackStruct  *cbs = (XmTextVerifyCallbackStruct *)
    call_data;
  Nlm_Boolean                 IsSpecSym = (cbs->text->length == 0);
  Nlm_Char                    password [MAX_PASSWORD];

  /*  the Widget/password synchronization is under the caller's control */
  if (cbs->event == NULL)
    return;

  /*  X-event processing  -- all the Widget/password sync. is here */
  if ((!IsSpecSym  &&  cbs->currInsert >= MAX_PASSWORD - 1)  ||
      cbs->text->length > 1  ||
      (IsSpecSym  &&  cbs->currInsert == 0  &&
      Nlm_GetInputChar(&cbs->event->xkey) != NLM_DEL))
    { /*  min/max password length achieved  */
      cbs->doit = FALSE;
      return;
    }

  Nlm_GetPassword ((Nlm_TexT)t, password, sizeof(password));
  if ( !IsSpecSym )
    {
      cbs->startPos = cbs->currInsert;
      if (cbs->startPos + cbs->text->length  >=  MAX_PASSWORD)
        cbs->text->length = MAX_PASSWORD - cbs->startPos - 1;
      Nlm_MemCpy(password + cbs->startPos, cbs->text->ptr,
                 (size_t)cbs->text->length);
      Nlm_MemSet(cbs->text->ptr, '*', (size_t)cbs->text->length);
    }

  cbs->endPos = XmTextGetLastPosition( w );
  password[cbs->startPos + (size_t)cbs->text->length] = '\0';
  Nlm_SetPassword ((Nlm_TexT)t, password);
}


static void Nlm_TabCallback (Widget w, XEvent *ev, String *args, Cardinal *num_args)

{
  XKeyEvent  *event;
  XtPointer  ptr;
  Nlm_TexT   t;

  event = (XKeyEvent *) ev;
  XtVaGetValues (w, XmNuserData, &ptr, NULL);
  t = (Nlm_TexT) ptr;
  Nlm_DoTabCallback (t);
}

static void Nlm_HiddenReturnCallback (Widget w, XEvent *ev, String *args, Cardinal *num_args)

{
  XKeyEvent  *event;
  XtPointer  ptr;
  Nlm_TexT   t;

  event = (XKeyEvent *) ev;
  XtVaGetValues (w, XmNuserData, &ptr, NULL);
  t = (Nlm_TexT) ptr;
  Nlm_DoReturnCallback (t);
}

static void Nlm_HiddenActivateCallback (Widget w, XtPointer client_data, XtPointer call_data)

{
  XmAnyCallbackStruct  *cbs;
  Nlm_TexT             t;

  cbs = (XmAnyCallbackStruct *) call_data;
  t = (Nlm_TexT) client_data;
  Nlm_DoReturnCallback (t);
}
#endif


static void Nlm_NewDialogText (Nlm_TexT t, Nlm_CharPtr dfault, Nlm_TxtActnProc actn)

{
  Nlm_TextTool    h;
  Nlm_Char        local [128];
  Nlm_RecT        r;
  Nlm_WindowTool  wptr;
#ifdef WIN_MAC
  Nlm_RectTool    rtool;
#endif
#ifdef WIN_MSWIN
  Nlm_Uint4       style;
  Nlm_FntPtr      fntptr;
#endif
#ifdef WIN_MOTIF
  XmFontList      fontlist;
  Nlm_FntPtr      fntptr;
  Cardinal        n;
  Arg             wargs [20];
  XmFontListEntry font_entry;
#endif

  Nlm_StringNCpy_0(local, dfault, sizeof(local));

  Nlm_GetRect ((Nlm_GraphiC) t, &r);
  wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) t);

#ifdef WIN_MAC
  Nlm_InsetRect (&r, 2, 2);
  Nlm_RecTToRectTool (&r, &rtool);
  h = TENew (&rtool, &rtool);
  Nlm_LoadTextData (t, h, NULL, NULL, FALSE, NULL, 0, FALSE,
                    FALSE, FALSE, FALSE, 1, NULL, NULL, NULL, NULL, TRUE);
  Nlm_SetDialogText ((Nlm_GraphiC) t, 0, local, FALSE);
  if (h != NULL) {
    TEAutoView (TRUE, h);
  }
#endif

#ifdef WIN_MSWIN
  allowTextCallback = FALSE;
  style = WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_LEFT;
  h = CreateWindow ("Edit", local, style,
                    r.left, r.top, r.right - r.left,
                    r.bottom - r.top, wptr, 0,
                    Nlm_currentHInst, NULL);
  if (h != NULL) {
    SetProp (h, (LPSTR) "Nlm_VibrantProp", (Nlm_HandleTool) t);
  }
  Nlm_LoadTextData (t, h, NULL, NULL, FALSE, NULL, 0, FALSE,
                    FALSE, FALSE, FALSE, 1, NULL, NULL, NULL, NULL, TRUE);
  if (lpfnNewTextProc == NULL) {
    lpfnNewTextProc = (WNDPROC) MakeProcInstance ((FARPROC) TextProc, Nlm_currentHInst);
  }
  if (lpfnOldTextProc == NULL) {
    lpfnOldTextProc = (WNDPROC) GetWindowLong (h, GWL_WNDPROC);
  } else if (lpfnOldTextProc != (WNDPROC) GetWindowLong (h, GWL_WNDPROC)) {
    Nlm_Message (MSG_ERROR, "TextProc subclass error");
  }
  SetWindowLong (h, GWL_WNDPROC, (LONG) lpfnNewTextProc);
  allowTextCallback = TRUE;
  fntptr = (Nlm_FntPtr) Nlm_HandLock (Nlm_systemFont);
  SetWindowFont(h, fntptr->handle, FALSE);
  Nlm_HandUnlock(Nlm_systemFont);
#endif

#ifdef WIN_MOTIF
  allowTextCallback = FALSE;
  fntptr = (Nlm_FntPtr) Nlm_HandLock (Nlm_systemFont);
  font_entry = XmFontListEntryCreate (XmFONTLIST_DEFAULT_TAG,
                                      XmFONT_IS_FONT,
                                      fntptr->handle);
  fontlist = XmFontListAppendEntry (NULL, font_entry);
  Nlm_HandUnlock (Nlm_systemFont);

  n = 0;
  XtSetArg (wargs [n], XmNx, (Position) r.left); n++;
  XtSetArg (wargs [n], XmNy, (Position) r.top); n++;
  XtSetArg (wargs [n], XmNwidth,  (Dimension) (r.right - r.left + 1)); n++;
  XtSetArg (wargs [n], XmNheight, (Dimension) (r.bottom - r.top + 1)); n++;
  XtSetArg (wargs [n], XmNfontList, fontlist); n++;
  XtSetArg (wargs [n], XmNmarginHeight, (Dimension) 1); n++;
  XtSetArg (wargs [n], XmNmarginWidth,  (Dimension) 2); n++;
  XtSetArg (wargs [n], XmNborderWidth,  (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNeditMode, XmSINGLE_LINE_EDIT); n++;
  XtSetArg (wargs [n], XmNhighlightThickness, 0); n++;
  XtSetArg (wargs [n], XmNautoShowCursorPosition, TRUE); n++;
  h = XmCreateText(wptr, (String) "", wargs, n);
  Nlm_OverrideStdTranslations((Nlm_GraphiC)t, h, VERT_PAGE|VERT_ARROW);
  Nlm_LoadTextData (t, h, NULL, NULL, FALSE, NULL, 0, FALSE,
                    FALSE, FALSE, FALSE, 1, NULL, NULL, NULL, NULL, TRUE);
  XmTextSetString (h, local);
  XmTextShowPosition (h, 0);
  XtAddCallback (h, XmNmodifyVerifyCallback, Nlm_TextFieldCallback, (XtPointer)t);
  XtAddCallback (h, XmNvalueChangedCallback, Nlm_TextCallback, (XtPointer) t);
  XtAddCallback (h, XmNactivateCallback, Nlm_ReturnCallback, (XtPointer) t);
  XtAddCallback (h, XmNfocusCallback, Nlm_FocusCallback, (XtPointer) t);
  XtAddCallback (h, XmNlosingFocusCallback, Nlm_LoseFocusCallback, (XtPointer) t);
  XtManageChild( h );

  if (fontlist != NULL) XmFontListFree (fontlist);
  allowTextCallback = TRUE;
#endif

  Nlm_LoadAction ((Nlm_GraphiC) t, (Nlm_ActnProc) actn);
}


static void Nlm_NewPasswordText (Nlm_TexT t, Nlm_CharPtr dfault, Nlm_TxtActnProc actn)

{
  Nlm_TextTool    h;
  Nlm_Char        local [128];
  Nlm_RecT        r;
  Nlm_WindowTool  wptr;
#ifdef WIN_MAC
  Nlm_RectTool    rtool;
#endif
#ifdef WIN_MSWIN
  Nlm_Uint4       style;
#endif
#ifdef WIN_MOTIF
  XmFontList      fontlist;
  Nlm_FntPtr      fntptr;
  Cardinal        n;
  Arg             wargs [20];
  XmFontListEntry font_entry;
#endif

  Nlm_StringNCpy_0(local, dfault, sizeof(local));

  Nlm_GetRect ((Nlm_GraphiC) t, &r);
  wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) t);

#ifdef WIN_MAC
  Nlm_InsetRect (&r, 2, 2);
  Nlm_RecTToRectTool (&r, &rtool);
  h = TENew (&rtool, &rtool);
  Nlm_LoadTextData (t, h, NULL, NULL, FALSE, NULL, 0, FALSE,
                    FALSE, FALSE, FALSE, 1, NULL, NULL, NULL, NULL, TRUE);
  Nlm_SetPasswordText ((Nlm_GraphiC) t, 0, local, FALSE);
#endif

#ifdef WIN_MSWIN
  style = WS_CHILD | WS_BORDER |  ES_AUTOVSCROLL | ES_LEFT | ES_PASSWORD;
  h = CreateWindow ("Edit", local, style,
                    r.left, r.top, r.right - r.left,
                    r.bottom - r.top, wptr, 0,
                    Nlm_currentHInst, NULL);
  if (h != NULL) {
    SetProp (h, (LPSTR) "Nlm_VibrantProp", (Nlm_HandleTool) t);
  }
  Nlm_LoadTextData (t, h, NULL, NULL, FALSE, NULL, 0, FALSE,
                    FALSE, FALSE, FALSE, 1, NULL, NULL, NULL, NULL, TRUE);
  if (lpfnNewTextProc == NULL) {
    lpfnNewTextProc = (WNDPROC) MakeProcInstance ((FARPROC) TextProc, Nlm_currentHInst);
  }
  if (lpfnOldTextProc == NULL) {
    lpfnOldTextProc = (WNDPROC) GetWindowLong (h, GWL_WNDPROC);
  } else if (lpfnOldTextProc != (WNDPROC) GetWindowLong (h, GWL_WNDPROC)) {
    Nlm_Message (MSG_ERROR, "TextProc subclass error");
  }
  SetWindowLong (h, GWL_WNDPROC, (LONG) lpfnNewTextProc);
  Nlm_SetPassword (t, local);
#endif

#ifdef WIN_MOTIF
  allowTextCallback = FALSE;
  fntptr = (Nlm_FntPtr) Nlm_HandLock (Nlm_systemFont);
  font_entry = XmFontListEntryCreate (XmFONTLIST_DEFAULT_TAG,
                                      XmFONT_IS_FONT,
                                      fntptr->handle);
  fontlist = XmFontListAppendEntry (NULL, font_entry);
  Nlm_HandUnlock (Nlm_systemFont);
  Nlm_InsetRect (&r, 3, 3);

  n = 0;
  XtSetArg (wargs [n], XmNx, (Position) r.left); n++;
  XtSetArg (wargs [n], XmNy, (Position) r.top); n++;
  XtSetArg (wargs [n], XmNwidth,  (Dimension) (r.right - r.left + 1)); n++;
  XtSetArg (wargs [n], XmNheight, (Dimension) (r.bottom - r.top + 1)); n++;
  XtSetArg (wargs [n], XmNmarginHeight, (Dimension) 1); n++;
  XtSetArg (wargs [n], XmNmarginWidth,  (Dimension) 2); n++;
  XtSetArg (wargs [n], XmNborderWidth,  (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNeditMode, XmSINGLE_LINE_EDIT); n++;
  XtSetArg (wargs [n], XmNfontList, fontlist); n++;
  XtSetArg (wargs [n], XmNhighlightThickness, 0); n++;
  XtSetArg (wargs [n], XmNautoShowCursorPosition, TRUE); n++;
  h = XmCreateText (wptr, (String) "", wargs, n);
  Nlm_OverrideStdTranslations((Nlm_GraphiC)t, h, VERT_PAGE|VERT_ARROW);
  Nlm_LoadTextData (t, h, NULL, NULL, FALSE, NULL, 0, FALSE,
                    FALSE, FALSE, FALSE, 1, NULL, NULL, NULL, NULL, TRUE);
  XtAddCallback (h, XmNmodifyVerifyCallback, Nlm_PasswordCallback, (XtPointer) t);
  XtAddCallback (h, XmNvalueChangedCallback, Nlm_TextCallback, (XtPointer) t);
  XtAddCallback (h, XmNactivateCallback, Nlm_ReturnCallback, (XtPointer) t);
  XtAddCallback (h, XmNfocusCallback, Nlm_FocusCallback, (XtPointer) t);
  XtAddCallback (h, XmNlosingFocusCallback, Nlm_LoseFocusCallback, (XtPointer) t);
  Nlm_SetPasswordText((Nlm_GraphiC)t, 0, local, FALSE);
  XtManageChild( h );

  if (fontlist != NULL) XmFontListFree (fontlist);
  allowTextCallback = TRUE;
#endif

  Nlm_LoadAction ((Nlm_GraphiC) t, (Nlm_ActnProc) actn);
}


static void Nlm_NewHiddenText (Nlm_TexT t, Nlm_CharPtr dfault,
                               Nlm_TxtActnProc actn, Nlm_TxtActnProc tabProc,
                               Nlm_TxtActnProc rtnProc)

{
  Nlm_TextTool    h;
  Nlm_Char        local [128];
  Nlm_RecT        r;
  Nlm_WindowTool  wptr;
#ifdef WIN_MAC
  Nlm_RectTool    rtool;
#endif
#ifdef WIN_MSWIN
  Nlm_Uint4       style;
  Nlm_FntPtr fntptr;
#endif
#ifdef WIN_MOTIF
  Cardinal        n;
  Arg             wargs [20];
  XmFontList      fontlist;
  Nlm_FntPtr      fntptr;
  XmFontListEntry font_entry;
#endif

  Nlm_StringNCpy_0(local, dfault, sizeof(local));

  Nlm_GetRect ((Nlm_GraphiC) t, &r);
  wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) t);

#ifdef WIN_MAC
  Nlm_RecTToRectTool (&r, &rtool);
  h = TENew (&rtool, &rtool);
  Nlm_LoadTextData (t, h, NULL, NULL, FALSE, NULL, 0, FALSE,
                    FALSE, TRUE, FALSE, 1, NULL, NULL, tabProc, rtnProc, TRUE);
  Nlm_SetDialogText ((Nlm_GraphiC) t, 0, local, FALSE);
  if (h != NULL) {
    TEAutoView (TRUE, h);
  }
#endif

#ifdef WIN_MSWIN
  style = WS_CHILD | ES_MULTILINE | ES_AUTOVSCROLL | ES_LEFT;
  h = CreateWindow ("Edit", local, style,
                    r.left, r.top, r.right - r.left,
                    r.bottom - r.top, wptr, 0,
                    Nlm_currentHInst, NULL);
  if (h != NULL) {
    SetProp (h, (LPSTR) "Nlm_VibrantProp", (Nlm_HandleTool) t);
  }
  Nlm_LoadTextData (t, h, NULL, NULL, FALSE, NULL, 0, FALSE,
                    FALSE, TRUE, FALSE, 1, NULL, NULL, tabProc, rtnProc, TRUE);
  if (lpfnNewTextProc == NULL) {
    lpfnNewTextProc = (WNDPROC) MakeProcInstance ((FARPROC) TextProc, Nlm_currentHInst);
  }
  if (lpfnOldTextProc == NULL) {
    lpfnOldTextProc = (WNDPROC) GetWindowLong (h, GWL_WNDPROC);
  } else if (lpfnOldTextProc != (WNDPROC) GetWindowLong (h, GWL_WNDPROC)) {
    Nlm_Message (MSG_ERROR, "TextProc subclass error");
  }
  SetWindowLong (h, GWL_WNDPROC, (LONG) lpfnNewTextProc);
  fntptr = (Nlm_FntPtr) Nlm_HandLock (Nlm_systemFont);
  SetWindowFont(h, fntptr->handle, FALSE);
  Nlm_HandUnlock(Nlm_systemFont);
#endif

#ifdef WIN_MOTIF
  allowTextCallback = FALSE;
  fntptr = (Nlm_FntPtr) Nlm_HandLock (Nlm_systemFont);
  font_entry = XmFontListEntryCreate (XmFONTLIST_DEFAULT_TAG,
                                      XmFONT_IS_FONT,
                                      fntptr->handle);
  fontlist = XmFontListAppendEntry (NULL, font_entry);
  Nlm_HandUnlock (Nlm_systemFont);

  n = 0;
  XtSetArg (wargs [n], XmNx, (Position) r.left); n++;
  XtSetArg (wargs [n], XmNy, (Position) r.top); n++;
  XtSetArg (wargs [n], XmNwidth,  (Dimension) (r.right - r.left + 1)); n++;
  XtSetArg (wargs [n], XmNheight, (Dimension) (r.bottom - r.top + 1)); n++;
  XtSetArg (wargs [n], XmNfontList, fontlist); n++;
  XtSetArg (wargs [n], XmNmarginHeight, 0); n++;
  XtSetArg (wargs [n], XmNmarginWidth,  0); n++;
  XtSetArg (wargs [n], XmNborderWidth, (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNshadowThickness, (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg (wargs [n], XmNhighlightThickness, 0); n++;
  XtSetArg (wargs [n], XmNautoShowCursorPosition, TRUE); n++;
  XtSetArg (wargs [n], XmNverifyBell, FALSE); n++;
  h = XmCreateText (wptr, (String) "", wargs, n);
  XtVaSetValues (h, XmNuserData, (XtPointer) t, NULL);
  Nlm_LoadTextData (t, h, NULL, NULL, FALSE, NULL, 0, FALSE,
                    FALSE, TRUE, FALSE, 1, NULL, NULL, tabProc, rtnProc, TRUE);
  XmTextSetString (h, local);
  XmTextShowPosition (h, 0);
  XtAddCallback (h, XmNmodifyVerifyCallback, Nlm_RefreshCallback, (XtPointer) t);
  XtAddCallback (h, XmNvalueChangedCallback, Nlm_TextCallback, (XtPointer) t);
  XtAddCallback (h, XmNactivateCallback, Nlm_HiddenActivateCallback, (XtPointer) t);
  XtAddCallback (h, XmNfocusCallback, Nlm_FocusCallback, (XtPointer) t);
  XtAddCallback (h, XmNlosingFocusCallback, Nlm_LoseFocusCallback, (XtPointer) t);
  XtManageChild( h );

  if (fontlist != NULL) XmFontListFree (fontlist);
  allowTextCallback = TRUE;
#endif

  Nlm_LoadAction ((Nlm_GraphiC) t, (Nlm_ActnProc) actn);
}


static void Nlm_NewSpecialText (Nlm_TexT t, Nlm_CharPtr dfault,
                                Nlm_TxtActnProc actn, Nlm_TxtActnProc tabProc,
                                Nlm_TxtActnProc rtnProc)

{
  Nlm_TextTool    h;
  Nlm_Char        local [128];
  Nlm_RecT        r;
  Nlm_WindowTool  wptr;
#ifdef WIN_MAC
  Nlm_RectTool    rtool;
#endif
#ifdef WIN_MSWIN
  Nlm_Uint4       style;
  Nlm_FntPtr fntptr;
#endif
#ifdef WIN_MOTIF
  XmFontList      fontlist;
  Nlm_FntPtr      fntptr;
  Cardinal        n;
  Arg             wargs[20];
  String          trans =
    "<Key>Tab:     do_tab()  \n\
     <Key>Return:  do_return()";
  XmFontListEntry font_entry;
#endif

  Nlm_StringNCpy_0(local, dfault, sizeof(local));

  Nlm_GetRect ((Nlm_GraphiC) t, &r);
  wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) t);

#ifdef WIN_MAC
  Nlm_InsetRect (&r, 2, 2);
  Nlm_RecTToRectTool (&r, &rtool);
  h = TENew (&rtool, &rtool);
  Nlm_LoadTextData (t, h, NULL, NULL, FALSE, NULL, 0, FALSE,
                    FALSE, FALSE, TRUE, 1, NULL, NULL, tabProc, rtnProc, TRUE);
  Nlm_SetDialogText ((Nlm_GraphiC) t, 0, local, FALSE);
  if (h != NULL) {
    TEAutoView (TRUE, h);
  }
#endif

#ifdef WIN_MSWIN
  allowTextCallback = FALSE;
  style = WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | ES_LEFT;
  h = CreateWindow ("Edit", local, style,
                    r.left, r.top, r.right - r.left,
                    r.bottom - r.top, wptr, 0,
                    Nlm_currentHInst, NULL);
  if (h != NULL) {
    SetProp (h, (LPSTR) "Nlm_VibrantProp", (Nlm_HandleTool) t);
  }
  Nlm_LoadTextData (t, h, NULL, NULL, FALSE, NULL, 0, FALSE,
                    FALSE, FALSE, TRUE, 1, NULL, NULL, tabProc, rtnProc, TRUE);
  if (lpfnNewTextProc == NULL) {
    lpfnNewTextProc = (WNDPROC) MakeProcInstance ((FARPROC) TextProc, Nlm_currentHInst);
  }
  if (lpfnOldTextProc == NULL) {
    lpfnOldTextProc = (WNDPROC) GetWindowLong (h, GWL_WNDPROC);
  } else if (lpfnOldTextProc != (WNDPROC) GetWindowLong (h, GWL_WNDPROC)) {
    Nlm_Message (MSG_ERROR, "TextProc subclass error");
  }
  SetWindowLong (h, GWL_WNDPROC, (LONG) lpfnNewTextProc);
  allowTextCallback = TRUE;
  fntptr = (Nlm_FntPtr) Nlm_HandLock (Nlm_systemFont);
  SetWindowFont(h, fntptr->handle, FALSE);
  Nlm_HandUnlock(Nlm_systemFont);
#endif

#ifdef WIN_MOTIF
  allowTextCallback = FALSE;
  fntptr = (Nlm_FntPtr) Nlm_HandLock (Nlm_systemFont);
  font_entry = XmFontListEntryCreate (XmFONTLIST_DEFAULT_TAG,
                                      XmFONT_IS_FONT,
                                      fntptr->handle);
  fontlist = XmFontListAppendEntry (NULL, font_entry);
  Nlm_HandUnlock (Nlm_systemFont);
  Nlm_InsetRect (&r, 3, 3);

  n = 0;
  XtSetArg (wargs [n], XmNx, (Position) r.left); n++;
  XtSetArg (wargs [n], XmNy, (Position) r.top); n++;
  XtSetArg (wargs [n], XmNwidth,  (Dimension) (r.right - r.left + 1)); n++;
  XtSetArg (wargs [n], XmNheight, (Dimension) (r.bottom - r.top + 1)); n++;
  XtSetArg (wargs [n], XmNmarginHeight, (Dimension) 1); n++;
  XtSetArg (wargs [n], XmNmarginWidth,  (Dimension) 2); n++;
  XtSetArg (wargs [n], XmNborderWidth,  (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  XtSetArg (wargs [n], XmNfontList, fontlist); n++;
  XtSetArg (wargs [n], XmNhighlightThickness, 0); n++;
  XtSetArg (wargs [n], XmNautoShowCursorPosition, TRUE); n++;
  XtSetArg (wargs [n], XmNverifyBell, FALSE); n++;
  h = XmCreateText (wptr, (String) "", wargs, n);
  XtOverrideTranslations (h, XtParseTranslationTable (trans));
  XtVaSetValues (h, XmNuserData, (XtPointer) t, NULL);
  Nlm_LoadTextData (t, h, NULL, NULL, FALSE, NULL, 0, FALSE,
                    FALSE, FALSE, TRUE, 1, NULL, NULL, tabProc, rtnProc, TRUE);
  XmTextSetString (h, local);
  XmTextShowPosition (h, 0);
  XtAddCallback (h, XmNmodifyVerifyCallback, Nlm_RefreshCallback, (XtPointer) t);
  XtAddCallback (h, XmNvalueChangedCallback, Nlm_TextCallback, (XtPointer) t);
  XtAddCallback (h, XmNactivateCallback, Nlm_HiddenActivateCallback, (XtPointer) t);
  XtAddCallback (h, XmNfocusCallback, Nlm_FocusCallback, (XtPointer) t);
  XtAddCallback (h, XmNlosingFocusCallback, Nlm_LoseFocusCallback, (XtPointer) t);
  XtManageChild( h );

  if (fontlist != NULL) XmFontListFree (fontlist);
  allowTextCallback = TRUE;
#endif

  Nlm_LoadAction ((Nlm_GraphiC) t, (Nlm_ActnProc) actn);
}


static void Nlm_NewScrollText (Nlm_TexT t, Nlm_Int2 height,
                               Nlm_FonT font, Nlm_Int2 fnthgt,
                               Nlm_Boolean wrap, Nlm_TxtActnProc actn)

{
  Nlm_TextTool    h;
  Nlm_BaR         hsb;
  Nlm_RecT        r;
  Nlm_BaR         vsb;
  Nlm_WindowTool  wptr;
#ifdef WIN_MAC
  Nlm_RectTool    dtool;
  Nlm_RectTool    rtool;
  Nlm_Int2        width;
#endif
#ifdef WIN_MSWIN
  Nlm_FntPtr      fntptr;
  HFONT           oldFont;
  Nlm_Uint4       style;
#endif
#ifdef WIN_MOTIF
  Nlm_FntPtr      fntptr;
  XmFontList      fontlist;
  Cardinal        n;
  Arg             wargs [15];
  XmFontListEntry font_entry;
#endif

  Nlm_GetRect ((Nlm_GraphiC) t, &r);
  wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) t);

#ifdef WIN_MAC
  vsb = NULL;
  hsb = NULL;
  r.left = r.right;
  r.right += Nlm_vScrollBarWidth;
  vsb = Nlm_VertScrollBar ((Nlm_GraphiC) t, &r, Nlm_VScrollAction);
  if (! wrap) {
    Nlm_GetRect ((Nlm_GraphiC) t, &r);
    r.top = r.bottom;
    r.bottom += Nlm_hScrollBarHeight;
    hsb = Nlm_HorizScrollBar ((Nlm_GraphiC) t, &r, Nlm_HScrollAction);
  }
  Nlm_GetRect ((Nlm_GraphiC) t, &r);
  Nlm_InsetRect (&r, 4, 2);
  if (fnthgt > 0) {
    height = (r.bottom - r.top + 1) / fnthgt;
  } else {
    height = 0;
  }
  Nlm_DoSetRange ((Nlm_GraphiC) vsb, height - 1, height - 1, 0, FALSE);
  r.bottom = r.top + height * fnthgt;
  Nlm_RecTToRectTool (&r, &rtool);
  if (! wrap) {
    width = (r.right - r.left + 1) / Nlm_stdCharWidth;
    Nlm_DoSetRange ((Nlm_GraphiC) hsb, width - 1, width - 1, 0, FALSE);
    r.right += HSCROLL_POSITIONS * Nlm_stdCharWidth;
  }
  Nlm_RecTToRectTool (&r, &dtool);
  h = TENew (&dtool, &rtool);
  Nlm_LoadTextData (t, h, vsb, hsb, wrap, font, fnthgt, FALSE,
                    FALSE, FALSE, FALSE, height, NULL, NULL, NULL, NULL, TRUE);
#endif

#ifdef WIN_MSWIN
  vsb = NULL;
  hsb = NULL;
  r.right += Nlm_vScrollBarWidth;
  allowTextCallback = FALSE;
  style = WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL | ES_LEFT;
  if (! wrap) {
    style |= WS_HSCROLL | ES_AUTOHSCROLL;
    r.bottom += Nlm_hScrollBarHeight;
  }
  h = CreateWindow ("Edit", "", style,
                    r.left, r.top, r.right - r.left,
                    r.bottom - r.top, wptr, (Nlm_HandleTool) t,
                    Nlm_currentHInst, NULL);
  if (h != NULL) {
    SetProp (h, (LPSTR) "Nlm_VibrantProp", (Nlm_HandleTool) t);
  }
  Edit_LimitText (h, 0);
  oldFont = NULL;
  if (font != NULL) {
    fntptr = (Nlm_FntPtr) Nlm_HandLock (font);
    if (fntptr != NULL && fntptr->handle != NULL) {
      oldFont = SelectObject (Nlm_currentHDC, fntptr->handle);
    }
    Nlm_HandUnlock (font);
  } else {
    oldFont = SelectObject (Nlm_currentHDC, GetStockObject (ANSI_FIXED_FONT));
  }
  if (oldFont != NULL) {
    SelectObject (Nlm_currentHDC, oldFont);
    SetWindowFont (h, oldFont, FALSE);
  }
  Nlm_LoadTextData (t, h, vsb, hsb, wrap, font, fnthgt, FALSE,
                    FALSE, FALSE, FALSE, height, NULL, NULL, NULL, NULL, TRUE);
  if (lpfnNewTextProc == NULL) {
    lpfnNewTextProc = (WNDPROC) MakeProcInstance ((FARPROC) TextProc, Nlm_currentHInst);
  }
  if (lpfnOldTextProc == NULL) {
    lpfnOldTextProc = (WNDPROC) GetWindowLong (h, GWL_WNDPROC);
  } else if (lpfnOldTextProc != (WNDPROC) GetWindowLong (h, GWL_WNDPROC)) {
    Nlm_Message (MSG_ERROR, "TextProc subclass error");
  }
  SetWindowLong (h, GWL_WNDPROC, (LONG) lpfnNewTextProc);
  allowTextCallback = TRUE;
#endif

#ifdef WIN_MOTIF
  allowTextCallback = FALSE;
  vsb = NULL;
  hsb = NULL;
  r.right += Nlm_vScrollBarWidth;
  if (! wrap) {
    r.bottom += Nlm_hScrollBarHeight;
  }
  fontlist = NULL;
  if (font != NULL) {
    fntptr = (Nlm_FntPtr) Nlm_HandLock (font);
    if (fntptr != NULL && fntptr->handle != NULL) {
      font_entry = XmFontListEntryCreate (XmFONTLIST_DEFAULT_TAG,
                                          XmFONT_IS_FONT,
                                          fntptr->handle);
      fontlist = XmFontListAppendEntry (NULL, font_entry);
    }
    Nlm_HandUnlock (font);
  }

  n = 0;
  XtSetArg (wargs [n], XmNx, (Position) r.left); n++;
  XtSetArg (wargs [n], XmNy, (Position) r.top); n++;
  XtSetArg (wargs [n], XmNmarginHeight, (Dimension) 1); n++;
  XtSetArg (wargs [n], XmNmarginWidth,  (Dimension) 2); n++;
  XtSetArg (wargs [n], XmNborderWidth,  (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNeditMode, XmMULTI_LINE_EDIT); n++;
  if (wrap) {
    XtSetArg (wargs [n], XmNscrollHorizontal, FALSE); n++;
    XtSetArg (wargs [n], XmNwordWrap, TRUE); n++;
  }
  XtSetArg (wargs [n], XmNfontList, fontlist); n++;
  XtSetArg (wargs [n], XmNhighlightThickness, 0); n++;
  h = XmCreateScrolledText (wptr, (String) "", wargs, n);
  Nlm_LoadTextData (t, h, vsb, hsb, wrap, font, fnthgt, FALSE,
                    FALSE, FALSE, FALSE, height, NULL, NULL, NULL, NULL, TRUE);
  XtAddCallback (h, XmNvalueChangedCallback, Nlm_TextCallback, (XtPointer) t);
  XtAddCallback (h, XmNfocusCallback, Nlm_FocusCallback, (XtPointer) t);
  XtAddCallback (h, XmNlosingFocusCallback, Nlm_LoseFocusCallback, (XtPointer) t);
  XtAddCallback (h, XmNmodifyVerifyCallback, Nlm_TextFieldCallback, (XtPointer)t);
  XtManageChild (h);

  allowTextCallback = TRUE;
  if (fontlist != NULL) {
    XmFontListFree (fontlist);
  }
#endif

  Nlm_LoadAction ((Nlm_GraphiC) t, (Nlm_ActnProc) actn);
}


extern Nlm_TexT Nlm_DialogText (Nlm_GrouP prnt, Nlm_CharPtr dfault,
                                Nlm_Int2 charWidth, Nlm_TxtActnProc actn)

{
  Nlm_Int2    cwid;
  Nlm_Int2    hbounds;
  Nlm_PoinT   npt;
  Nlm_RecT    r;
  Nlm_TexT    t;
  Nlm_WindoW  tempPort;
  Nlm_Int2    vbounds;

  t = NULL;
  if (prnt != NULL) {
    tempPort = Nlm_SavePort ((Nlm_GraphiC) prnt);
    Nlm_GetNextPosition ((Nlm_GraphiC) prnt, &npt);
    Nlm_SelectFont (Nlm_systemFont);
    if (charWidth == 0) {
      cwid = Nlm_StringWidth (dfault);
    } else {
      cwid = (Nlm_Int2)(charWidth * Nlm_stdCharWidth);
    }
#ifdef WIN_MAC
    hbounds = 2;
    vbounds = 2;
#endif
#ifdef WIN_MSWIN
    hbounds = 3;
    vbounds = (Nlm_Int2)((Nlm_stdFontHeight * 3 / 2 - Nlm_stdLineHeight) / 2);
#endif
#ifdef WIN_MOTIF
    hbounds = 3;
    vbounds = 3;
#endif
    r.left = npt.x;
    r.top  = npt.y;
    r.right  = (Nlm_Int2)(r.left + cwid + 2 + hbounds * 2);
    r.bottom = (Nlm_Int2)(r.top + Nlm_stdLineHeight + vbounds * 2);
    t = (Nlm_TexT) Nlm_CreateLink ((Nlm_GraphiC) prnt, &r,
                                   sizeof (Nlm_TextRec), dialogTextProcs);
    if (t != NULL) {
      Nlm_NewDialogText (t, dfault, actn);
#ifdef WIN_MAC
      Nlm_DoSelect ((Nlm_GraphiC) t, FALSE);
#endif
      Nlm_DoAdjustPrnt ((Nlm_GraphiC) t, &r, TRUE, FALSE);
      Nlm_DoShow ((Nlm_GraphiC) t, TRUE, FALSE);
    }
    Nlm_RestorePort (tempPort);
  }
  return t;
}

extern Nlm_TexT Nlm_HiddenText (Nlm_GrouP prnt, Nlm_CharPtr dfault,
                                Nlm_Int2 charWidth, Nlm_TxtActnProc actn,
                                Nlm_TxtActnProc tabProc, Nlm_TxtActnProc rtnProc)

{
  Nlm_Int2    cwid;
  Nlm_Int2    hbounds;
  Nlm_PoinT   npt;
  Nlm_RecT    r;
  Nlm_TexT    t;
  Nlm_WindoW  tempPort;
  Nlm_Int2    vbounds;

  t = NULL;
  if (prnt != NULL) {
    tempPort = Nlm_SavePort ((Nlm_GraphiC) prnt);
    Nlm_GetNextPosition ((Nlm_GraphiC) prnt, &npt);
    Nlm_SelectFont (Nlm_systemFont);
    if (charWidth == 0) {
      cwid = Nlm_StringWidth (dfault);
    } else {
      cwid = (Nlm_Int2)(charWidth * Nlm_stdCharWidth);
    }
    hbounds = 0;
    vbounds = 0;
    r.left = npt.x;
    r.top  = npt.y;
    r.right  = (Nlm_Int2)(r.left + cwid + 2 + hbounds * 2);
    r.bottom = (Nlm_Int2)(r.top + Nlm_stdLineHeight + vbounds * 2);
    t = (Nlm_TexT) Nlm_CreateLink ((Nlm_GraphiC) prnt, &r,
                                   sizeof (Nlm_TextRec), hiddenTextProcs);
    if (t != NULL) {
      Nlm_NewHiddenText (t, dfault, actn, tabProc, rtnProc);
#ifdef WIN_MAC
      Nlm_DoSelect ((Nlm_GraphiC) t, FALSE);
#endif
      Nlm_DoAdjustPrnt ((Nlm_GraphiC) t, &r, TRUE, FALSE);
      Nlm_DoShow ((Nlm_GraphiC) t, TRUE, FALSE);
    }
    Nlm_RestorePort (tempPort);
  }
  return t;
}

extern Nlm_TexT Nlm_SpecialText (Nlm_GrouP prnt, Nlm_CharPtr dfault,
                                 Nlm_Int2 charWidth, Nlm_TxtActnProc actn,
                                 Nlm_TxtActnProc tabProc, Nlm_TxtActnProc rtnProc)

{
  Nlm_Int2    cwid;
  Nlm_Int2    hbounds;
  Nlm_PoinT   npt;
  Nlm_RecT    r;
  Nlm_TexT    t;
  Nlm_WindoW  tempPort;
  Nlm_Int2    vbounds;

  t = NULL;
  if (prnt != NULL) {
    tempPort = Nlm_SavePort ((Nlm_GraphiC) prnt);
    Nlm_GetNextPosition ((Nlm_GraphiC) prnt, &npt);
    Nlm_SelectFont (Nlm_systemFont);
    if (charWidth == 0) {
      cwid = Nlm_StringWidth (dfault);
    } else {
      cwid = (Nlm_Int2)(charWidth * Nlm_stdCharWidth);
    }
#ifdef WIN_MAC
    hbounds = 2;
    vbounds = 2;
#endif
#ifdef WIN_MSWIN
    hbounds = 3;
    vbounds = (Nlm_Int2)((Nlm_stdFontHeight * 3 / 2 - Nlm_stdLineHeight) / 2);
#endif
#ifdef WIN_MOTIF
    hbounds = 3;
    vbounds = 3;
#endif
    r.left = npt.x;
    r.top  = npt.y;
    r.right  = (Nlm_Int2)(r.left + cwid + 2 + hbounds * 2);
    r.bottom = (Nlm_Int2)(r.top + Nlm_stdLineHeight + vbounds * 2);
    t = (Nlm_TexT) Nlm_CreateLink ((Nlm_GraphiC) prnt, &r,
                                   sizeof (Nlm_TextRec), specialTextProcs);
    if (t != NULL) {
      Nlm_NewSpecialText (t, dfault, actn, tabProc, rtnProc);
#ifdef WIN_MAC
      Nlm_DoSelect ((Nlm_GraphiC) t, FALSE);
#endif
      Nlm_DoAdjustPrnt ((Nlm_GraphiC) t, &r, TRUE, FALSE);
      Nlm_DoShow ((Nlm_GraphiC) t, TRUE, FALSE);
    }
    Nlm_RestorePort (tempPort);
  }
  return t;
}

extern Nlm_TexT Nlm_PasswordText (Nlm_GrouP prnt, Nlm_CharPtr dfault,
                                  Nlm_Int2 charWidth, Nlm_TxtActnProc actn)

{
  Nlm_Int2    cwid;
  Nlm_Int2    hbounds;
  Nlm_PoinT   npt;
  Nlm_RecT    r;
  Nlm_TexT    t;
  Nlm_WindoW  tempPort;
  Nlm_Int2    vbounds;

  t = NULL;
  if (prnt != NULL) {
    tempPort = Nlm_SavePort ((Nlm_GraphiC) prnt);
    Nlm_GetNextPosition ((Nlm_GraphiC) prnt, &npt);
    Nlm_SelectFont (Nlm_systemFont);
    if (charWidth == 0) {
      cwid = Nlm_StringWidth (dfault);
    } else {
      cwid = (Nlm_Int2)(charWidth * Nlm_stdCharWidth);
    }
#ifdef WIN_MAC
    hbounds = 2;
    vbounds = 2;
#endif
#ifdef WIN_MSWIN
    hbounds = 3;
    vbounds = (Nlm_Int2)((Nlm_stdFontHeight * 3 / 2 - Nlm_stdLineHeight) / 2);
#endif
#ifdef WIN_MOTIF
    hbounds = 3;
    vbounds = 3;
#endif
    r.left = npt.x;
    r.top  = npt.y;
    r.right  = (Nlm_Int2)(r.left + cwid + 2 + hbounds * 2);
    r.bottom = (Nlm_Int2)(r.top + Nlm_stdLineHeight + vbounds * 2);
    t = (Nlm_TexT) Nlm_CreateLink ((Nlm_GraphiC) prnt, &r,
                                   sizeof (Nlm_PasswdRec), passwordTextProcs);
    if (t != NULL) {
      Nlm_NewPasswordText (t, dfault, actn);
#ifdef WIN_MAC
      Nlm_DoSelect ((Nlm_GraphiC) t, FALSE);
#endif
      Nlm_DoAdjustPrnt ((Nlm_GraphiC) t, &r, TRUE, FALSE);
      Nlm_DoShow ((Nlm_GraphiC) t, TRUE, FALSE);
    }
    Nlm_RestorePort (tempPort);
  }
  return t;
}

extern Nlm_TexT Nlm_ScrollText (Nlm_GrouP prnt, Nlm_Int2 width,
                                Nlm_Int2 height, Nlm_FonT font,
                                Nlm_Boolean wrap, Nlm_TxtActnProc actn)

{
  Nlm_Int2    dwid;
  Nlm_Int2    fnthgt;
  Nlm_PoinT   npt;
  Nlm_RecT    r;
  Nlm_TexT    t;
  Nlm_WindoW  tempPort;
  Nlm_Int2    thgt;

  t = NULL;
  if (prnt != NULL) {
    tempPort = Nlm_SavePort ((Nlm_GraphiC) prnt);
    Nlm_GetNextPosition ((Nlm_GraphiC) prnt, &npt);
    fnthgt = Nlm_stdLineHeight;
    if ( !font )
      font = Nlm_programFont;
    Nlm_SelectFont (font);
    dwid = (Nlm_Int2)(width  * Nlm_stdCharWidth);
    thgt = (Nlm_Int2)(height * fnthgt);
    Nlm_LoadRect(&r, npt.x, npt.y,
                 (Nlm_Int2)(npt.x + dwid + 8), (Nlm_Int2)(npt.y + thgt + 4));
    t = (Nlm_TexT) Nlm_CreateLink ((Nlm_GraphiC) prnt, &r, sizeof (Nlm_TextRec), scrollTextProcs);
    if (t != NULL) {
      Nlm_NewScrollText (t, height, font, fnthgt, wrap, actn);
      r.right += Nlm_vScrollBarWidth;
      if (! wrap) {
        r.bottom += Nlm_hScrollBarHeight;
      }
      Nlm_DoAdjustPrnt ((Nlm_GraphiC) t, &r, TRUE, FALSE);
      Nlm_SelectFont (Nlm_systemFont);
      Nlm_DoShow ((Nlm_GraphiC) t, TRUE, FALSE);
    }
    Nlm_RestorePort (tempPort);
  }
  return t;
}


extern void Nlm_SetTextEditable(Nlm_TexT t, Nlm_Boolean editable)
{
#if defined(WIN_MOTIF)
  XtVaSetValues(Nlm_GetTextHandle(t), XmNeditable, (Boolean)editable, NULL);
#else
  Nlm_TextData  tdata;
  Nlm_GetTextData (t, &tdata);
  tdata.editable = editable;
  Nlm_SetTextData (t, &tdata);
#endif
}


extern Nlm_Int4 Nlm_SetTextCursorPos(Nlm_TexT t, Nlm_Int4 pos)
{
  Nlm_Int4 actual_pos = 0;
#if defined(WIN_MOTIF) || defined(WIN_MSWIN)
  Nlm_TextTool h = Nlm_GetTextHandle( t );
  allowTextCallback = FALSE;
#endif
#if defined(WIN_MAC)
  Nlm_SelectText(t, pos, pos);
  Nlm_TextSelectionRange(t, &actual_pos, NULL);
#elif defined(WIN_MOTIF) 
  XmTextSetSelection(h, (XmTextPosition)0, (XmTextPosition)0, (Time)0);
  XmTextShowPosition(h, pos);
  XmTextSetInsertionPosition(h, pos);
  actual_pos = XmTextGetInsertionPosition( h );
#elif defined(WIN_MSWIN)
  Edit_SetSel(h, pos, pos);
#ifdef WIN32
  Edit_ScrollCaret( h );
#endif
  {{
  DWORD dwBegin = 0;
  SNDMSG(h, EM_GETSEL, (WPARAM)(&dwBegin), 0L);
  actual_pos = dwBegin;
  }}
#endif
#if defined(WIN_MOTIF) || defined(WIN_MSWIN)
  allowTextCallback = TRUE;
#endif
  return actual_pos;
}


extern Nlm_Int4 Nlm_GetTextCursorPos(Nlm_TexT t)
{
  Nlm_Int4     pos = 0;
  Nlm_TextTool h   = Nlm_GetTextHandle( t );
  if ( !h )
    return 0;

#if   defined(WIN_MAC)
  Nlm_TextSelectionRange(t, &pos, NULL);
#elif defined(WIN_MOTIF) 
  pos = (Nlm_Int4)XmTextGetInsertionPosition( h );
#elif defined(WIN_MSWIN)
  {{
  DWORD dwBegin = 0;
  SNDMSG(h, EM_GETSEL, (WPARAM)&dwBegin, 0L);
  pos = dwBegin;
  }}
#endif

  return pos;
}


extern void Nlm_PassPanelClickToText (Nlm_PaneL p, Nlm_TexT t, Nlm_PoinT pt)

{
#ifdef WIN_MSWIN
  Nlm_TextTool  h;
  UINT          message;
  WPARAM        wParam;
#endif
#ifdef WIN_MOTIF
  Nlm_TextTool  h;
#endif

  if (t != NULL) {
    Nlm_KillSlateTimer ();
#ifdef WIN_MAC
    Nlm_DoClick ((Nlm_GraphiC) t, pt);
#endif
#ifdef WIN_MSWIN
    ReleaseCapture ();
    h = Nlm_GetTextHandle (t);
    if (Nlm_dblClick) {
      message = WM_LBUTTONDBLCLK;
    } else {
      message = WM_LBUTTONDOWN;
    }
    if (Nlm_shftKey) {
      wParam = MK_SHIFT;
    } else {
      wParam = 0;
    }
/*
    Edit_SetSel (h, 0, 0);
    if (Nlm_Visible (t) && Nlm_AllParentsVisible (t)) {
      SetFocus (h);
      Nlm_DoTextSelect (t);
    }
*/
    SendMessage (h, message, Nlm_currentWParam, Nlm_currentLParam);
#endif
#ifdef WIN_MOTIF
  if (Nlm_WindowHasBeenShown (Nlm_ParentWindow ((Nlm_Handle) t))) {
    allowTextCallback = FALSE;
    h = Nlm_GetTextHandle (t);
    XmTextSetSelection (h, (XmTextPosition) 0,
                        (XmTextPosition) 0, (Time) 0);
    XmProcessTraversal (h, XmTRAVERSE_CURRENT);
    XmTextSetHighlight (h, (XmTextPosition) 0,
                        (XmTextPosition) 0, XmHIGHLIGHT_SELECTED);
    allowTextCallback = TRUE;
  }
  Nlm_DoTextSelect (t);
#endif
  }
}

#ifdef WIN_MAC
extern Nlm_Boolean Nlm_RegisterTexts (void)

{
  return TRUE;
}
#endif

#ifdef WIN_MSWIN
extern Nlm_Boolean Nlm_RegisterTexts (void)

{
  return TRUE;
}
#endif

#ifdef WIN_MOTIF
extern Nlm_Boolean Nlm_RegisterTexts (void)
{
  XtActionsRec  actions;

  actions.string = "do_tab";
  actions.proc = Nlm_TabCallback;
  XtAppAddActions (Nlm_appContext, &actions, 1);
  actions.string = "do_return";
  actions.proc = Nlm_HiddenReturnCallback;
  XtAppAddActions (Nlm_appContext, &actions, 1);
  return TRUE;
}
#endif

extern void Nlm_FreeTexts (void)

{
  gphprcsptr = (Nlm_GphPrcsPtr) Nlm_MemFree (gphprcsptr);
}

extern void Nlm_InitTexts (void)

{
  gphprcsptr = (Nlm_GphPrcsPtr) Nlm_MemNew (sizeof (Nlm_GphPrcs) * 5);

  dialogTextProcs = &(gphprcsptr [0]);
#ifdef WIN_MAC
  dialogTextProcs->click = Nlm_DialogTextClick;
  dialogTextProcs->key = Nlm_DialogKey;
  dialogTextProcs->draw = Nlm_DrawDialogText;
  dialogTextProcs->idle = Nlm_IdleText;
#endif
#ifdef WIN_MSWIN
  dialogTextProcs->command = Nlm_DialogTextCommand;
#endif
#ifdef WIN_MOTIF
#endif
  dialogTextProcs->show = Nlm_ShowText;
  dialogTextProcs->hide = Nlm_HideText;
  dialogTextProcs->enable = Nlm_EnableText;
  dialogTextProcs->disable = Nlm_DisableText;
  dialogTextProcs->activate = Nlm_ActivateText;
  dialogTextProcs->deactivate = Nlm_DeactivateText;
  dialogTextProcs->remove = Nlm_RemoveText;
  dialogTextProcs->reset = Nlm_ResetText;
  dialogTextProcs->select = Nlm_TextSelectProc;
  dialogTextProcs->setTitle = Nlm_SetDialogText;
  dialogTextProcs->getTitle = Nlm_GetDialogText;
  dialogTextProcs->setPosition = Nlm_SetTextPosition;
  dialogTextProcs->getPosition = Nlm_GetTextPosition;
  dialogTextProcs->gainFocus = Nlm_TextGainFocus;
  dialogTextProcs->loseFocus = Nlm_TextLoseFocus;

  hiddenTextProcs = &(gphprcsptr [1]);
#ifdef WIN_MAC
  hiddenTextProcs->click = Nlm_DialogTextClick;
  hiddenTextProcs->key = Nlm_DialogKey;
  hiddenTextProcs->draw = Nlm_DrawHiddenText;
  hiddenTextProcs->idle = Nlm_IdleText;
#endif
#ifdef WIN_MSWIN
  hiddenTextProcs->command = Nlm_DialogTextCommand;
#endif
#ifdef WIN_MOTIF
#endif
  hiddenTextProcs->show = Nlm_ShowText;
  hiddenTextProcs->hide = Nlm_HideText;
  hiddenTextProcs->enable = Nlm_EnableText;
  hiddenTextProcs->disable = Nlm_DisableText;
  hiddenTextProcs->activate = Nlm_ActivateHiddenText;
  hiddenTextProcs->deactivate = Nlm_DeactivateText;
  hiddenTextProcs->remove = Nlm_RemoveText;
  hiddenTextProcs->reset = Nlm_ResetText;
  hiddenTextProcs->select = Nlm_TextSelectProc;
  hiddenTextProcs->setTitle = Nlm_SetDialogText;
  hiddenTextProcs->getTitle = Nlm_GetDialogText;
  hiddenTextProcs->setPosition = Nlm_SetHiddenTextPosition;
  hiddenTextProcs->getPosition = Nlm_GetTextPosition;
  hiddenTextProcs->gainFocus = Nlm_TextGainFocus;
  hiddenTextProcs->loseFocus = Nlm_TextLoseFocus;

  specialTextProcs = &(gphprcsptr [2]);
#ifdef WIN_MAC
  specialTextProcs->click = Nlm_DialogTextClick;
  specialTextProcs->key = Nlm_DialogKey;
  specialTextProcs->draw = Nlm_DrawDialogText;
  specialTextProcs->idle = Nlm_IdleText;
#endif
#ifdef WIN_MSWIN
  specialTextProcs->command = Nlm_DialogTextCommand;
#endif
#ifdef WIN_MOTIF
#endif
  specialTextProcs->show = Nlm_ShowText;
  specialTextProcs->hide = Nlm_HideText;
  specialTextProcs->enable = Nlm_EnableText;
  specialTextProcs->disable = Nlm_DisableText;
  specialTextProcs->activate = Nlm_ActivateHiddenText;
  specialTextProcs->deactivate = Nlm_DeactivateText;
  specialTextProcs->remove = Nlm_RemoveText;
  specialTextProcs->reset = Nlm_ResetText;
  specialTextProcs->select = Nlm_TextSelectProc;
  specialTextProcs->setTitle = Nlm_SetDialogText;
  specialTextProcs->getTitle = Nlm_GetDialogText;
  specialTextProcs->setPosition = Nlm_SetTextPosition;
  specialTextProcs->getPosition = Nlm_GetTextPosition;
  specialTextProcs->gainFocus = Nlm_TextGainFocus;
  specialTextProcs->loseFocus = Nlm_TextLoseFocus;

  passwordTextProcs = &(gphprcsptr [3]);
#ifdef WIN_MAC
  passwordTextProcs->click = Nlm_DialogTextClick;
  passwordTextProcs->key = Nlm_PasswordKey;
  passwordTextProcs->draw = Nlm_DrawDialogText;
  passwordTextProcs->idle = Nlm_IdleText;
#endif
#ifdef WIN_MSWIN
  passwordTextProcs->command = Nlm_PasswordTextCommand;
#endif
#ifdef WIN_MOTIF
#endif
  passwordTextProcs->show = Nlm_ShowText;
  passwordTextProcs->hide = Nlm_HideText;
  passwordTextProcs->enable = Nlm_EnableText;
  passwordTextProcs->disable = Nlm_DisableText;
  passwordTextProcs->activate = Nlm_ActivateText;
  passwordTextProcs->deactivate = Nlm_DeactivateText;
  passwordTextProcs->remove = Nlm_RemoveText;
  passwordTextProcs->reset = Nlm_ResetText;
  passwordTextProcs->select = Nlm_TextSelectProc;
  passwordTextProcs->setTitle = Nlm_SetPasswordText;
  passwordTextProcs->getTitle = Nlm_GetPasswordText;
  passwordTextProcs->setPosition = Nlm_SetTextPosition;
  passwordTextProcs->getPosition = Nlm_GetTextPosition;
  passwordTextProcs->gainFocus = Nlm_TextGainFocus;
  passwordTextProcs->loseFocus = Nlm_TextLoseFocus;

  scrollTextProcs = &(gphprcsptr [4]);
#ifdef WIN_MAC
  scrollTextProcs->click = Nlm_ScrollTextClick;
  scrollTextProcs->key = Nlm_TextKey;
  scrollTextProcs->draw = Nlm_DrawScrollText;
  scrollTextProcs->idle = Nlm_IdleText;
#endif
#ifdef WIN_MSWIN
  scrollTextProcs->command = Nlm_ScrollTextCommand;
#endif
#ifdef WIN_MOTIF
#endif
  scrollTextProcs->show = Nlm_ShowScrollText;
  scrollTextProcs->hide = Nlm_HideScrollText;
  scrollTextProcs->enable = Nlm_EnableText;
  scrollTextProcs->disable = Nlm_DisableText;
  scrollTextProcs->activate = Nlm_ActivateScrollText;
  scrollTextProcs->deactivate = Nlm_DeactivateScrollText;
  scrollTextProcs->remove = Nlm_RemoveText;
  scrollTextProcs->reset = Nlm_ResetText;
  scrollTextProcs->select = Nlm_TextSelectProc;
  scrollTextProcs->setTitle = Nlm_SetScrollText;
  scrollTextProcs->getTitle = Nlm_GetScrollText;
  scrollTextProcs->setOffset = Nlm_SetScrollTextOffset;
  scrollTextProcs->getOffset = Nlm_GetScrollTextOffset;
  scrollTextProcs->setPosition = Nlm_SetScrollTextPosition;
  scrollTextProcs->getPosition = Nlm_GetScrollTextPosition;
  scrollTextProcs->gainFocus = Nlm_TextGainFocus;
  scrollTextProcs->loseFocus = Nlm_TextLoseFocus;
}

extern void Nlm_SetTextCursorBlinkRate(Nlm_TexT t, Nlm_Int2 msec)
{
#ifdef WIN_MOTIF
    Nlm_TextTool h;
    Arg          args[2];

    if(t == NULL || msec < 0)
        return;
    h = Nlm_GetTextHandle(t);
    if(h == NULL)
        return;
    XtSetArg(args[0], XmNblinkRate, msec);
    XtSetValues(h, args, 1);
#endif
    return;
}

extern void Nlm_SetTextColor(Nlm_TexT t, Nlm_Uint4 r, Nlm_Uint4 g, Nlm_Uint4 b)
{
#ifdef WIN_MOTIF
    Nlm_TextTool h;
    Arg          args[2];

    if(t == NULL)
        return;
    h = Nlm_GetTextHandle(t);
    if(h == NULL)
        return;
    XtSetArg(args[0], XmNforeground, Nlm_GetColorRGB(r, g, b));
    XtSetValues(h, args, 1);
#endif
    return;
}

#ifdef WIN_MOTIF
static void Nlm_KeepCrNlTextFieldCallback(Widget w,
                                          XtPointer client_data,
                                          XtPointer call_data)
{
    XmTextVerifyCallbackStruct *cbs = (XmTextVerifyCallbackStruct *) call_data;
    char *p;
    int i;

    /* recalculate the correct length by detecting the null byte
     */
    p = cbs->text->ptr;
    for(i = 0; i < cbs->text->length; i++)
    {
        if(p[i] == '\0')
        {
            cbs->text->length = i + 1;
            break;
        }
    }

    for(i = 0; i < cbs->text->length; i++)
        if((!isprint(p[i]) && !isspace(p[i])) || p[i] == '\r' || p[i] == '\n')
            p[i] = ' ';
}
#endif

extern void Nlm_SetKeepCrNlTextFieldCallback(Nlm_TexT t)
{
#ifdef WIN_MOTIF
    Nlm_TextTool h;

    if(t == NULL)
        return;
    h = Nlm_GetTextHandle(t);
    if(h == NULL)
        return;
    XtAddCallback(h, XmNmodifyVerifyCallback,
                  Nlm_KeepCrNlTextFieldCallback, (XtPointer) t);
#endif
    return;
}
