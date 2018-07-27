/*   vibprmpt.c
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
* File Name:  vibprmpt.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   7/1/91
*
* $Revision: 6.1 $
*
* File Description: 
*       Vibrant prompt functions
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: vibprmpt.c,v $
* Revision 6.1  1997/11/26 21:30:33  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.0  1997/08/25 18:57:25  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/01/29 16:41:22  kans
* using StringNCpy_0
*
 * Revision 5.0  1996/05/28  13:45:08  ostell
 * Set to revision 5.0
 *
 * Revision 4.2  1996/03/02  22:36:38  kans
 * reduction of X traffic (DV)
 *
 * Revision 4.1  1996/02/13  17:24:07  kans
 * accelerated set position prior to realization (Denis Vakatov)
 *
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 2.12  1995/05/17  15:15:14  kans
 * added Log line
 *
*
* ==========================================================================
*/

#include <vibtypes.h>
#include <vibprocs.h>
#include <vibincld.h>

#ifdef WIN_MAC
#define Nlm_PromptTool Nlm_Handle
#endif

#ifdef WIN_MSWIN
#define Nlm_PromptTool HWND
#endif

#ifdef WIN_MOTIF
#define Nlm_PromptTool Widget
#endif

typedef  struct  Nlm_promptdata {
  Nlm_PromptTool  handle;
  Nlm_FonT        font;
  Nlm_Int2        height;
  Nlm_Char        just;
} Nlm_PromptData;

typedef  struct  Nlm_promptrec {
  Nlm_GraphicRec  graphicR;
  Nlm_PromptData  prompt;
} Nlm_PromptRec, PNTR Nlm_PptPtr;

static Nlm_GphPrcsPtr  gphprcsptr = NULL;

static Nlm_GphPrcsPtr  promptProcs;
static Nlm_GphPrcsPtr  staticPromptProcs;

static Nlm_PrompT      recentPrompt = NULL;
static Nlm_PromptData  recentPromptData;

static void Nlm_LoadPromptData (Nlm_PrompT p, Nlm_PromptTool hdl,
                                Nlm_FonT fnt, Nlm_Int2 hgt,
                                Nlm_Char jst)

{
  Nlm_PptPtr      pp;
  Nlm_PromptData  PNTR pptr;

  if (p != NULL) {
    pp = (Nlm_PptPtr) Nlm_HandLock (p);
    pptr = &(pp->prompt);
    pptr->handle = hdl;
    pptr->font = fnt;
    pptr->height = hgt;
    pptr->just = jst;
    Nlm_HandUnlock (p);
    recentPrompt = NULL;
  }
}

static void Nlm_GetPromptData (Nlm_PrompT p, Nlm_PromptData * pdata)

{
  Nlm_PptPtr  pp;

  if (p != NULL && pdata != NULL) {
    if (p == recentPrompt && NLM_RISKY) {
      *pdata = recentPromptData;
    } else {
      pp = (Nlm_PptPtr) Nlm_HandLock (p);
      *pdata = pp->prompt;
      Nlm_HandUnlock (p);
      recentPrompt = p;
      recentPromptData = *pdata;
    }
  }
}

#ifdef WIN_MAC
static void Nlm_SetPromptData (Nlm_PrompT p, Nlm_PromptData * pdata)
{
  Nlm_PptPtr  pp;

  if (p != NULL && pdata != NULL) {
    pp = (Nlm_PptPtr) Nlm_HandLock (p);
    pp->prompt = *pdata;
    Nlm_HandUnlock (p);
    recentPrompt = p;
    recentPromptData = *pdata;
  }
}

static void Nlm_SetPromptHandle (Nlm_PrompT p, Nlm_PromptTool hdl)
{
  Nlm_PromptData  pdata;

  Nlm_GetPromptData (p, &pdata);
  pdata.handle = hdl;
  Nlm_SetPromptData (p, &pdata);
}
#endif

static Nlm_PromptTool Nlm_GetPromptHandle (Nlm_PrompT p)

{
  Nlm_PromptData  pdata;

  Nlm_GetPromptData (p, &pdata);
  return pdata.handle;
}

#ifdef WIN_MAC
static void Nlm_DrawPrompt (Nlm_GraphiC p)

{
  Nlm_Boolean     gray;
  Nlm_PromptTool  h;
  Nlm_PromptData  pd;
  Nlm_RecT        r;
  Nlm_Char        str [256];

  if (Nlm_GetVisible (p) && Nlm_GetAllParentsVisible (p)) {
    Nlm_GetPromptData ((Nlm_PrompT) p, &pd);
    h = pd.handle;
    if (h != NULL) {
      if (pd.font != NULL) {
        Nlm_SelectFont (pd.font);
      }
      Nlm_GetString (h, str, sizeof (str));
      if (Nlm_StringLen (str) > 0) {
        if (Nlm_Enabled (p) && Nlm_AllParentsEnabled (p)) {
          gray = FALSE;
        } else {
          gray = TRUE;
        }
        Nlm_GetRect (p, &r);
        Nlm_DrawString (&r, str, pd.just, gray);
      }
      Nlm_SelectFont (Nlm_systemFont);
    }
  }
}
#endif

static void Nlm_ShowPrompt (Nlm_GraphiC p, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_PromptTool  h;
  Nlm_WindoW      tempPort;

  if (setFlag) {
    Nlm_SetVisible (p, TRUE);
  }
  if (Nlm_GetVisible (p) && Nlm_AllParentsButWindowVisible (p)) {
    tempPort = Nlm_SavePortIfNeeded (p, savePort);
    h = Nlm_GetPromptHandle ((Nlm_PrompT) p);
#ifdef WIN_MAC
    Nlm_DoDraw (p);
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

static void Nlm_HidePrompt (Nlm_GraphiC p, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_PromptTool  h;
  Nlm_WindoW      tempPort;
#ifdef WIN_MAC
  Nlm_RecT        r;
#endif

  if (setFlag) {
    Nlm_SetVisible (p, FALSE);
  }
  tempPort = Nlm_SavePortIfNeeded (p, savePort);
  h = Nlm_GetPromptHandle ((Nlm_PrompT) p);
#ifdef WIN_MAC
  if (Nlm_GetAllParentsVisible (p)) {
    Nlm_GetRect (p, &r);
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

static void Nlm_EnablePrompt (Nlm_GraphiC p, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_PromptTool  h;
  Nlm_WindoW      tempPort;
#ifdef WIN_MAC
  Nlm_RecT        r;
#endif

  if (setFlag) {
    Nlm_SetEnabled (p, TRUE);
  }
  if (Nlm_GetEnabled (p) && Nlm_GetAllParentsEnabled (p)) {
    tempPort = Nlm_SavePortIfNeeded (p, savePort);
    h = Nlm_GetPromptHandle ((Nlm_PrompT) p);
#ifdef WIN_MAC
    if (Nlm_GetVisible (p) && Nlm_GetAllParentsVisible (p)) {
      if (h != NULL) {
        Nlm_DrawPrompt (p);
      } else {
        Nlm_GetRect (p, &r);
        Nlm_InvalRect (&r);
      }
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

static void Nlm_DisablePrompt (Nlm_GraphiC p, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_PromptTool  h;
  Nlm_WindoW      tempPort;
#ifdef WIN_MAC
  Nlm_RecT        r;
#endif

  if (setFlag) {
    Nlm_SetEnabled (p, FALSE);
  }
  tempPort = Nlm_SavePortIfNeeded (p, savePort);
  h = Nlm_GetPromptHandle ((Nlm_PrompT) p);
#ifdef WIN_MAC
  if (Nlm_GetVisible (p) && Nlm_GetAllParentsVisible (p)) {
    if (h != NULL) {
      Nlm_DrawPrompt (p);
    } else {
      Nlm_GetRect (p, &r);
      Nlm_InvalRect (&r);
    }
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

static void Nlm_RemovePrompt (Nlm_GraphiC p, Nlm_Boolean savePort)

{
  Nlm_GraphicData  gdata;
  Nlm_PromptTool   h;
  Nlm_WindoW       tempPort;

  tempPort = Nlm_SavePortIfNeeded (p, savePort);
  h = Nlm_GetPromptHandle ((Nlm_PrompT) p);
  if (h != NULL) {
#ifdef WIN_MAC
    Nlm_HandFree (h);
#endif
#ifdef WIN_MSWIN
    DestroyWindow (h);
#endif
#ifdef WIN_MOTIF
    XtDestroyWidget (h);
#endif
  }
  Nlm_GetGraphicData (p, &gdata);
  if (gdata.data != NULL && gdata.cleanup != NULL) {
    gdata.cleanup ((Nlm_GraphiC) p, gdata.data);
  }
  Nlm_HandFree (p);
  recentPrompt = NULL;
  Nlm_RestorePort (tempPort);
}

static void Nlm_RemoveLinkedInPrompt (Nlm_GraphiC p, Nlm_Boolean savePort)

{
  Nlm_PromptTool  h;
  Nlm_WindoW      tempPort;

  tempPort = Nlm_SavePortIfNeeded (p, savePort);
  h = Nlm_GetPromptHandle ((Nlm_PrompT) p);
  if (h != NULL) {
#ifdef WIN_MAC
    Nlm_HandFree (h);
#endif
#ifdef WIN_MSWIN
    DestroyWindow (h);
#endif
#ifdef WIN_MOTIF
    XtDestroyWidget (h);
#endif
  }
  Nlm_RemoveLink (p);
  recentPrompt = NULL;
  Nlm_RestorePort (tempPort);
}

static void Nlm_SetPromptTitle (Nlm_GraphiC p, Nlm_Int2 item,
                                Nlm_CharPtr title, Nlm_Boolean savePort)

{
  Nlm_PromptTool  h;
  Nlm_WindoW      tempPort;
#ifdef WIN_MAC
  Nlm_RecT        r;
#endif
#ifdef WIN_MOTIF
  XmString        label;
#endif

  tempPort = Nlm_SavePortIfNeeded (p, savePort);
  h = Nlm_GetPromptHandle ((Nlm_PrompT) p);
#ifdef WIN_MAC
  h = Nlm_SetString (h, title);
  Nlm_SetPromptHandle ((Nlm_PrompT) p, h);
  if (Nlm_GetVisible (p) && Nlm_GetAllParentsVisible (p)) {
    if (h != NULL) {
      Nlm_DrawPrompt (p);
    } else {
      Nlm_GetRect (p, &r);
      Nlm_InvalRect (&r);
    }
  }
#endif
#ifdef WIN_MSWIN
  if (h != NULL) {
    SetWindowText (h, title);
    if (Nlm_GetVisible (p) && Nlm_GetAllParentsVisible (p)) {
      UpdateWindow (h);
    }
  }
#endif
#ifdef WIN_MOTIF
  if (h != NULL) {
    label = XmStringCreateSimple (title);
    XtVaSetValues (h, XmNlabelString, label, NULL);
    XmStringFree (label);
  }
#endif
  Nlm_RestorePort (tempPort);
}

static void Nlm_GetPromptTitle (Nlm_GraphiC p, Nlm_Int2 item,
                                Nlm_CharPtr title, size_t maxsize)

{
  Nlm_PromptTool  h;
#ifdef WIN_MSWIN
  Nlm_Char        temp [256];
#endif
#ifdef WIN_MOTIF
  XmString        label;
  char            *text;
#endif

  if (title != NULL) {
    h = Nlm_GetPromptHandle ((Nlm_PrompT) p);
    if (h != NULL) {
#ifdef WIN_MAC
      Nlm_GetString (h, title, maxsize);
#endif
#ifdef WIN_MSWIN
      GetWindowText (h, temp, sizeof (temp));
      Nlm_StringNCpy_0 (title, temp, maxsize);
#endif
#ifdef WIN_MOTIF
      XtVaGetValues (h, XmNlabelString, &label, NULL);
      title [0] = '\0';
      if (XmStringGetLtoR (label, XmSTRING_DEFAULT_CHARSET, &text)) {
        Nlm_StringNCpy_0 (title, text, maxsize);
        XtFree (text);
      }
#endif
    } else {
      Nlm_StringNCpy_0 (title, "", maxsize);
    }
  }
}

#ifndef WIN_MOTIF
static void Nlm_InvalPrompt (Nlm_GraphiC p)
{
  Nlm_RecT        r;
#ifdef WIN_MSWIN
  Nlm_RectTool    rtool;
  Nlm_WindowTool  wptr;
#endif

  if (Nlm_GetVisible (p) && Nlm_GetAllParentsVisible (p)) {
#ifdef WIN_MAC
    Nlm_GetRect (p, &r);
    Nlm_InsetRect (&r, -1, -1);
    Nlm_InvalRect (&r);
#endif
#ifdef WIN_MSWIN
    wptr = Nlm_ParentWindowPtr (p);
    Nlm_GetRect (p, &r);
    r.top = r.bottom - 1;
    r.bottom++;
    Nlm_RecTToRectTool (&r, &rtool);
    InvalidateRect (wptr, &rtool, TRUE);
    Nlm_GetRect (p, &r);
    r.left = r.right - 1;
    r.right++;
    Nlm_RecTToRectTool (&r, &rtool);
    InvalidateRect (wptr, &rtool, TRUE);
#endif
  }
}
#endif /* ndef WIN_MOTIF */

static void Nlm_SetPromptPosition (Nlm_GraphiC p, Nlm_RectPtr r,
				   Nlm_Boolean savePort, Nlm_Boolean force)
{
  Nlm_Int2        delta;
  Nlm_PromptTool  h;
  Nlm_Int2        height;
  Nlm_Int2        limit;
  Nlm_RecT        oldRect;
  Nlm_PromptData  pdata;
  Nlm_RecT        rct;
  Nlm_WindoW      tempPort;

  if (r == NULL)  return;

  if ( !Nlm_GetRealized( p ) )
    {
      Nlm_SetRect (p, r);
      return;
    }

  Nlm_DoGetPosition (p, &oldRect);
  if (!force  &&  Nlm_EqualRect (r, &oldRect))  return;

  tempPort = Nlm_SavePortIfNeeded (p, savePort);
  Nlm_GetPromptData ((Nlm_PrompT) p, &pdata);
  h = pdata.handle;
  rct = *r;
  limit = ABS (rct.bottom - rct.top);
  height = pdata.height;
  delta = limit - height;
  if (delta > 0) {
    rct.top += delta / 2;
    rct.bottom = rct.top + height;
  }

#ifdef WIN_MAC
  Nlm_InvalPrompt (p);
  Nlm_SetRect (p, r);
  Nlm_InvalPrompt (p);
#endif
#ifdef WIN_MSWIN
  Nlm_SetRect (p, r);
  if (h != NULL) {
    MoveWindow (h, rct.left, rct.top, rct.right - rct.left,
		rct.bottom - rct.top, TRUE);
    UpdateWindow (h);
  }
#endif
#ifdef WIN_MOTIF
  XtVaSetValues (h,
		 XmNx, (Position) rct.left,
		 XmNy, (Position) rct.top,
		 XmNwidth, (Dimension) (rct.right - rct.left),
		 XmNheight, (Dimension) (rct.bottom - rct.top), 
		 NULL);
  Nlm_SetRect (p, r);
#endif

  Nlm_RestorePort (tempPort);
}


static void Nlm_GetPromptPosition (Nlm_GraphiC p, Nlm_RectPtr r)

{
  if (r != NULL) {
    Nlm_GetRect (p, r);
  }
}


static void Nlm_NewPrompt (Nlm_PrompT p, Nlm_CharPtr title, Nlm_FonT font, Nlm_Char just)

{
  Nlm_Int2        delta;
  Nlm_PromptTool  h;
  Nlm_Int2        height;
  Nlm_Int2        limit;
  Nlm_Char        local [128];
  Nlm_RecT        r;
  Nlm_WindowTool  wptr;
#ifdef WIN_MSWIN
  Nlm_FntPtr      fntptr;
  Nlm_Int4        style;
#endif
#ifdef WIN_MOTIF
  Nlm_FntPtr      fntptr;
  XmFontList      fontlist;
  XmString        label;
  Cardinal        n;
  Arg             wargs [15];
#endif

  local [0] = '\0';
  Nlm_StringNCpy_0 (local, title, sizeof (local) - 1);
  Nlm_GetRect ((Nlm_GraphiC) p, &r);
  limit = ABS (r.bottom - r.top);
  height = Nlm_LineHeight ();
  delta = limit - height;
  if (delta > 0) {
    r.top += delta / 2;
    r.bottom = r.top + height;
  }
  wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) p);

#ifdef WIN_MAC
  h = Nlm_SetString (NULL, local);
#endif

#ifdef WIN_MSWIN
  switch (just) {
    case 'r':
      style = SS_RIGHT;
      break;
    case 'l':
      style = SS_LEFT;
      break;
    case 'c':
      style = SS_CENTER;
      break;
    default:
      style = SS_LEFT;
      break;
  }
  h = CreateWindow ("Static", local, WS_CHILD | SS_NOPREFIX | style,
                    r.left, r.top, r.right - r.left,
                    r.bottom - r.top, wptr, 0,
                    Nlm_currentHInst, NULL);
  if (font != NULL) {
    fntptr = (Nlm_FntPtr) Nlm_HandLock (font);
    if (fntptr != NULL && fntptr->handle != NULL) {
      SetWindowFont (h, fntptr->handle, FALSE);
    }
    Nlm_HandUnlock (font);
  }
#endif

#ifdef WIN_MOTIF
  fontlist = NULL;
  if (font != NULL) {
    fntptr = (Nlm_FntPtr) Nlm_HandLock (font);
    if (fntptr != NULL && fntptr->handle != NULL) {
      fontlist = XmFontListCreate (fntptr->handle, XmSTRING_DEFAULT_CHARSET);
    }
    Nlm_HandUnlock (font);
  }

  n = 0;
  label = XmStringCreateSimple (local[0] ? local : " ");
  XtSetArg (wargs [n], XmNlabelString, label); n++;
  XtSetArg (wargs [n], XmNx, (Position) r.left); n++;
  XtSetArg (wargs [n], XmNy, (Position) r.top); n++;
  XtSetArg (wargs [n], XmNwidth, (Dimension) (r.right - r.left)); n++;
  XtSetArg (wargs [n], XmNheight, (Dimension) (r.bottom - r.top)); n++;
  XtSetArg (wargs [n], XmNmarginHeight, 0); n++;
  XtSetArg (wargs [n], XmNmarginWidth, 0); n++;
  XtSetArg (wargs [n], XmNborderWidth, (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNrecomputeSize, FALSE); n++;
  XtSetArg (wargs [n], XmNfontList, fontlist); n++;
  switch (just) {
    case 'r':
      XtSetArg (wargs [n], XmNalignment, XmALIGNMENT_END); n++;
      break;
    case 'l':
      XtSetArg (wargs [n], XmNalignment, XmALIGNMENT_BEGINNING); n++;
      break;
    case 'c':
      XtSetArg (wargs [n], XmNalignment, XmALIGNMENT_CENTER); n++;
      break;
    default:
      XtSetArg (wargs [n], XmNalignment, XmALIGNMENT_BEGINNING); n++;
      break;
  }
  h = XmCreateLabel (wptr, (String) "", wargs, n);
  XmStringFree (label);
  XtManageChild( h );

  if (fontlist != NULL) {
    XmFontListFree (fontlist);
  }
#endif

  Nlm_LoadPromptData (p, h, font, height, just);
}


extern Nlm_PrompT Nlm_DependentPrompt (Nlm_GraphiC prnt, Nlm_RectPtr r,
                                       Nlm_CharPtr title, Nlm_FonT font,
                                       Nlm_Char just)

{
  Nlm_PrompT  p;
  Nlm_WindoW  tempPort;

  p = NULL;
  if (prnt != NULL && r != NULL) {
    tempPort = Nlm_SavePort (prnt);
    if (font != NULL) {
      Nlm_SelectFont (font);
    } else {
      Nlm_SelectFont (Nlm_systemFont);
    }
    p = (Nlm_PrompT) Nlm_HandNew (sizeof (Nlm_PromptRec));
    if (p != NULL) {
      Nlm_LoadGraphicData ((Nlm_GraphiC) p, NULL, (Nlm_GraphiC) prnt,
                           NULL, NULL, promptProcs, NULL, r, TRUE, FALSE, NULL, NULL);
      if (Nlm_nextIdNumber < 32767) {
        Nlm_nextIdNumber++;
      }
      Nlm_NewPrompt (p, title, font, just);
    }
    Nlm_SelectFont (Nlm_systemFont);
    Nlm_RestorePort (tempPort);
  }
  return p;
}

extern Nlm_PrompT Nlm_StaticPrompt  (Nlm_GrouP prnt, Nlm_CharPtr title,
                                     Nlm_Int2 pixwidth, Nlm_Int2 pixheight,
                                     Nlm_FonT font, Nlm_Char just)

{
  Nlm_PoinT   npt;
  Nlm_PrompT  p;
  Nlm_RecT    r;
  Nlm_WindoW  tempPort;

  p = NULL;
  if (prnt != NULL) {
    tempPort = Nlm_SavePort ((Nlm_GraphiC) prnt);
    Nlm_GetNextPosition ((Nlm_GraphiC) prnt, &npt);
    if (font != NULL) {
      Nlm_SelectFont (font);
    } else {
      Nlm_SelectFont (Nlm_systemFont);
    }
    if (pixwidth == 0) {
      pixwidth = Nlm_StringWidth (title) + 2;
    }
    if (pixheight == 0) {
      pixheight = Nlm_LineHeight ();
    }
    Nlm_LoadRect(&r, npt.x, npt.y,
                 (Nlm_Int2)(npt.x+pixwidth), (Nlm_Int2)(npt.y+pixheight));
    p = (Nlm_PrompT) Nlm_CreateLink ((Nlm_GraphiC) prnt, &r, sizeof (Nlm_PromptRec),
                                     staticPromptProcs);
    if (p != NULL) {
      Nlm_NewPrompt (p, title, font, just);
      Nlm_DoAdjustPrnt ((Nlm_GraphiC) p, &r, TRUE, FALSE);
      Nlm_DoShow ((Nlm_GraphiC) p, TRUE, FALSE);
    }
    Nlm_SelectFont (Nlm_systemFont);
    Nlm_RestorePort (tempPort);
  }
  return p;
}

extern void Nlm_FreePrompt (void)

{
  gphprcsptr = (Nlm_GphPrcsPtr) Nlm_MemFree (gphprcsptr);
}

extern void Nlm_InitPrompt (void)

{
  gphprcsptr = (Nlm_GphPrcsPtr) Nlm_MemNew (sizeof (Nlm_GphPrcs) * 2);

  promptProcs = &(gphprcsptr [0]);
#ifdef WIN_MAC
  promptProcs->draw = Nlm_DrawPrompt;
#endif
  promptProcs->show = Nlm_ShowPrompt;
  promptProcs->hide = Nlm_HidePrompt;
  promptProcs->enable = Nlm_EnablePrompt;
  promptProcs->disable = Nlm_DisablePrompt;
  promptProcs->remove = Nlm_RemovePrompt;
  promptProcs->setTitle = Nlm_SetPromptTitle;
  promptProcs->getTitle = Nlm_GetPromptTitle;
  promptProcs->setPosition = Nlm_SetPromptPosition;
  promptProcs->getPosition = Nlm_GetPromptPosition;

  staticPromptProcs = &(gphprcsptr [1]);
#ifdef WIN_MAC
  staticPromptProcs->draw = Nlm_DrawPrompt;
#endif
  staticPromptProcs->show = Nlm_ShowPrompt;
  staticPromptProcs->hide = Nlm_HidePrompt;
  staticPromptProcs->enable = Nlm_EnablePrompt;
  staticPromptProcs->disable = Nlm_DisablePrompt;
  staticPromptProcs->remove = Nlm_RemoveLinkedInPrompt;
  staticPromptProcs->setTitle = Nlm_SetPromptTitle;
  staticPromptProcs->getTitle = Nlm_GetPromptTitle;
  staticPromptProcs->setPosition = Nlm_SetPromptPosition;
  staticPromptProcs->getPosition = Nlm_GetPromptPosition;
}
