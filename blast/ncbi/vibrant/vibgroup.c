/*   vibgroup.c
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
* File Name:  vibgroup.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   7/1/91
*
* $Revision: 6.2 $
*
* File Description: 
*       Vibrant group functions
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: vibgroup.c,v $
* Revision 6.2  2004/02/03 23:32:25  sinyakov
* WIN_MSWIN: use InvalidateRect() to redraw group box inner space after it was moved
* instead of UpdateWindow() which redraws only group box frame
* Nlm_GroupGainFocus(): call Nlm_GetNext() before calling Nlm_DoGainFocus()
*
* Revision 6.1  1997/11/26 21:30:20  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.0  1997/08/25 18:57:05  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/01/29 16:41:22  kans
* using StringNCpy_0
*
 * Revision 5.0  1996/05/28  13:45:08  ostell
 * Set to revision 5.0
 *
 * Revision 4.3  1996/03/26  22:35:37  vakatov
 * Wrong positioning of the group label caused by the fact that the group
 * label is not registered as a child of the group -- fixed.
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
 * Revision 2.18  1995/06/08  20:54:43  kans
 * added Nlm_WindowFrameGroup for Mac, to assist with screen dump figures
 *
 * Revision 2.17  1995/05/17  15:15:14  kans
 * added Log line
 *
*
* ==========================================================================
*/

#include <vibtypes.h>
#include <vibprocs.h>
#include <vibincld.h>

#ifdef WIN_MAC
#define Nlm_GroupTool Nlm_PrompT
#define Nlm_LineTool  Nlm_Handle
#endif

#ifdef WIN_MSWIN
#define Nlm_GroupTool HWND
#define Nlm_LineTool  Nlm_Handle
#endif

#ifdef WIN_MOTIF
#define Nlm_GroupTool Nlm_PrompT
#define Nlm_LineTool  Widget
#endif

typedef  struct  Nlm_groupdata {
  Nlm_GroupTool  handle;
  Nlm_LineTool   left;
  Nlm_LineTool   top;
  Nlm_LineTool   right;
  Nlm_LineTool   bottom;
  Nlm_Int2       style;
} Nlm_GroupData;

typedef  struct  Nlm_grouprec {
  Nlm_BoxRec     boxR;
  Nlm_GroupData  group;
} Nlm_GroupRec, PNTR Nlm_GrpPtr;

static Nlm_GphPrcsPtr  gphprcsptr = NULL;

static Nlm_GphPrcsPtr  groupProcs;

static Nlm_GrouP       recentGroup = NULL;
static Nlm_GroupData   recentGroupData;

static void Nlm_LoadGroupData (Nlm_GrouP g, Nlm_GroupTool hdl,
                               Nlm_LineTool lf, Nlm_LineTool tp,
                               Nlm_LineTool rt, Nlm_LineTool bt,
                               Nlm_Int2 styl)

{
  Nlm_GrpPtr     gp;
  Nlm_GroupData  PNTR gptr;

  if (g != NULL) {
    gp = (Nlm_GrpPtr) Nlm_HandLock (g);
    gptr = &(gp->group);
    gptr->handle = hdl;
    gptr->left = lf;
    gptr->top = tp;
    gptr->right = rt;
    gptr->bottom = bt;
    gptr->style = styl;
    Nlm_HandUnlock (g);
    recentGroup = NULL;
  }
}


static void Nlm_GetGroupData (Nlm_GrouP g, Nlm_GroupData * gdata)

{
  Nlm_GrpPtr  gp;

  if (g != NULL && gdata != NULL) {
    if (g == recentGroup && NLM_RISKY) {
      *gdata = recentGroupData;
    } else {
      gp = (Nlm_GrpPtr) Nlm_HandLock (g);
      *gdata = gp->group;
      Nlm_HandUnlock (g);
      recentGroup = g;
      recentGroupData = *gdata;
    }
  }
}

static Nlm_GroupTool Nlm_GetGroupHandle (Nlm_GrouP g)

{
  Nlm_GroupData  gdata;

  Nlm_GetGroupData (g, &gdata);
  return gdata.handle;
}

#ifdef WIN_MAC
typedef struct revItem {
  Nlm_GraphiC          graphic;
  struct revItem  PNTR next;
} Nlm_RevItem, PNTR Nlm_RevPtr;

static Nlm_Boolean Nlm_GroupClick (Nlm_GraphiC a, Nlm_PoinT pt)

{
  Nlm_GraphiC  g;
  Nlm_RevPtr   next;
  Nlm_Boolean  notInside;
  Nlm_RecT     r;
  Nlm_Boolean  rsult;
  Nlm_RevPtr   this;
  Nlm_RevPtr   top;

  rsult = FALSE;
  Nlm_GetRect (a, &r);
  if (Nlm_PtInRect (pt, &r)) {
    g = Nlm_GetChild (a);
    notInside = TRUE;
    /*
    while (g != NULL && notInside) {
      n = Nlm_GetNext (g);
      if (Nlm_GetEnabled (g) && Nlm_GetVisible (g) && Nlm_DoClick (g, pt)) {
        notInside = FALSE;
      }
      g = n;
    }
    */
    if (g != NULL && notInside) {
      top = NULL;
      while (g != NULL) {
        this = Nlm_MemNew (sizeof (Nlm_RevItem));
        if (this != NULL) {
          this->graphic = g;
          this->next = top;
          top = this;
        }
        g = Nlm_GetNext (g);
      }
      this = top;
      while (this != NULL && notInside) {
        next = this->next;
        g = this->graphic;
        if (Nlm_GetEnabled (g) && Nlm_GetVisible (g) && Nlm_DoClick (g, pt)) {
          notInside = FALSE;
        }
        this = next;
      }
      this = top;
      while (this != NULL) {
        next = this->next;
        Nlm_MemFree (this);
        this = next;
      }
    }
    rsult = (! notInside);
  }
  return rsult;
}

static Nlm_Boolean Nlm_GroupKey (Nlm_GraphiC a, Nlm_Char ch)

{
  Nlm_GraphiC  g;
  Nlm_GraphiC  n;
  Nlm_Boolean  notInside;

  g = Nlm_GetChild (a);
  notInside = TRUE;
  while (g != NULL && notInside) {
    n = Nlm_GetNext (g);
    if (Nlm_GetEnabled (g) && Nlm_GetVisible (g) && Nlm_DoKey (g, ch)) {
      notInside = FALSE;
    }
    g = n;
  }
  return (! notInside);
}

static void Nlm_DrawGroup (Nlm_GraphiC a)

{
  Nlm_GraphiC    g;
  Nlm_GroupData  gdata;
  Nlm_GroupTool  h;
  Nlm_Int2       i;
  Nlm_RecT       r;

  if (Nlm_GetVisible (a) && Nlm_GetAllParentsVisible (a)) {
    Nlm_GetRect (a, &r);
    Nlm_InsetRect (&r, -4, -4);
    if (Nlm_RectInRgn (&r, Nlm_updateRgn)) {
      Nlm_InsetRect (&r, 4, 4);
      Nlm_GetGroupData ((Nlm_GrouP) a, &gdata);
      h = gdata.handle;
      if (h != NULL) {
        if (gdata.style == 0) {
          r.top += 6;
          Nlm_FrameRect (&r);
          Nlm_DoDraw ((Nlm_GraphiC) h);
        } else {
          Nlm_FrameRect (&r);
          r.bottom++;
          r.right++;
          Nlm_FrameRect (&r);
          r.bottom--;
          r.right--;
          r.bottom = r.top + 19;
          if (Nlm_hasColorQD) {
            Nlm_SelectColor (127, 127, 127);
          }
          for (i = 4; i < 16; i += 2) {
            Nlm_MoveTo (r.left + 2, r.top + i);
            Nlm_LineTo (r.right - 3, r.top + i);
          }
          if (Nlm_hasColorQD) {
            Nlm_Black ();
          }
          Nlm_DoDraw ((Nlm_GraphiC) h);
          if (Nlm_hasColorQD) {
            /*
            Nlm_SelectColor (191, 191, 191);
            */
            Nlm_SelectColor (191, 191, 255);
          }
          Nlm_MoveTo (r.left + 1, r.top + 1);
          Nlm_LineTo (r.right - 2, r.top + 1);
          Nlm_MoveTo (r.left + 1, r.top + 1);
          Nlm_LineTo (r.left + 1, r.bottom - 2);
          if (Nlm_hasColorQD) {
            /*
            Nlm_SelectColor (159, 159, 159);
            */
            Nlm_SelectColor (159, 159, 223);
          }
          Nlm_MoveTo (r.left + 1, r.bottom - 2);
          Nlm_LineTo (r.right - 2, r.bottom - 2);
          Nlm_MoveTo (r.right - 2, r.top + 1);
          Nlm_LineTo (r.right - 2, r.bottom - 2);
          if (Nlm_hasColorQD) {
            Nlm_Black ();
          }
          Nlm_FrameRect (&r);
        }
      }
      g = Nlm_GetChild (a);
      while (g != NULL) {
        Nlm_DoDraw (g);
        g = Nlm_GetNext (g);
      }
    }
  }
}

static Nlm_Boolean Nlm_IdleGroup (Nlm_GraphiC a, Nlm_PoinT pt)

{
  Nlm_GraphiC  g;

  if (Nlm_GetVisible (a) && Nlm_GetEnabled (a)) {
    g = Nlm_GetChild (a);
    while (g != NULL) {
      Nlm_DoIdle (g, pt);
      g = Nlm_GetNext (g);
    }
  }
  return TRUE;
}
#endif

static void Nlm_ShowGroup (Nlm_GraphiC a, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_GraphiC    g;
  Nlm_GroupTool  h;
  Nlm_WindoW     tempPort;
#ifdef WIN_MAC
  Nlm_RecT       r;
#endif
#ifdef WIN_MOTIF
  Nlm_GroupData  gdata;
#endif

  tempPort = Nlm_SavePortIfNeeded (a, savePort);
  if (setFlag) {
    Nlm_SetVisible (a, TRUE);
  }
  if (Nlm_GetVisible (a) && Nlm_AllParentsButWindowVisible (a)) {
    h = Nlm_GetGroupHandle ((Nlm_GrouP) a);
    if (h != NULL) {
#ifdef WIN_MAC
      if (Nlm_GetAllParentsVisible (a)) {
        Nlm_GetRect (a, &r);
        r.top += 6;
        Nlm_FrameRect (&r);
      }
      Nlm_DoShow ((Nlm_GraphiC) h, TRUE, FALSE);
#endif
#ifdef WIN_MSWIN
      ShowWindow (h, SW_SHOW);
      UpdateWindow (h);
#endif
#ifdef WIN_MOTIF
      Nlm_DoShow ((Nlm_GraphiC) h, TRUE, FALSE);
      Nlm_GetGroupData ((Nlm_GrouP) a, &gdata);
      XtManageChild (gdata.left);
      XtManageChild (gdata.top);
      XtManageChild (gdata.right);
      XtManageChild (gdata.bottom);
#endif
    }
    g = Nlm_GetChild (a);
    while (g != NULL) {
      Nlm_DoShow (g, FALSE, FALSE);
      g = Nlm_GetNext (g);
    }
  }
  Nlm_RestorePort (tempPort);
}

static void Nlm_HideGroup (Nlm_GraphiC a, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_GraphiC    g;
  Nlm_GroupTool  h;
  Nlm_WindoW     tempPort;
#ifdef WIN_MAC
  Nlm_RecT       r;
#endif
#ifdef WIN_MOTIF
  Nlm_GroupData  gdata;
#endif

  tempPort = Nlm_SavePortIfNeeded (a, savePort);
  if (Nlm_AllParentsButWindowVisible (a)) {
    h = Nlm_GetGroupHandle ((Nlm_GrouP) a);
#ifdef WIN_MAC
    if (h != NULL) {
      Nlm_DoHide ((Nlm_GraphiC) h, TRUE, FALSE);
    }
    if (Nlm_GetAllParentsVisible (a)) {
      Nlm_GetRect (a, &r);
      Nlm_InsetRect (&r, -1, -1);
      /*
      Nlm_InvalRect (&r);
      */
      Nlm_EraseRect (&r);
      Nlm_ValidRect (&r);
    }
#endif
    g = Nlm_GetChild (a);
    while (g != NULL) {
      Nlm_DoHide (g, FALSE, FALSE);
      g = Nlm_GetNext (g);
    }
#ifdef WIN_MSWIN
    if (h != NULL) {
      ShowWindow (h, SW_HIDE);
      UpdateWindow (h);
    }
#endif
#ifdef WIN_MOTIF
    if (h != NULL) {
      Nlm_DoHide ((Nlm_GraphiC) h, TRUE, FALSE);
      Nlm_GetGroupData ((Nlm_GrouP) a, &gdata);
      XtUnmanageChild (gdata.left);
      XtUnmanageChild (gdata.top);
      XtUnmanageChild (gdata.right);
      XtUnmanageChild (gdata.bottom);
    }
#endif
  }
  if (setFlag) {
    Nlm_SetVisible (a, FALSE);
  }
  Nlm_RestorePort (tempPort);
}

static void Nlm_EnableGroup (Nlm_GraphiC a, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_GraphiC    g;
  Nlm_GroupTool  h;
  Nlm_WindoW     tempPort;

  tempPort = Nlm_SavePortIfNeeded (a, savePort);
  if (setFlag) {
    Nlm_SetEnabled (a, TRUE);
  }
  if (Nlm_GetEnabled (a) && Nlm_GetAllParentsEnabled (a)) {
    h = Nlm_GetGroupHandle ((Nlm_GrouP) a);
    if (h != NULL) {
#ifdef WIN_MAC
      Nlm_DoEnable ((Nlm_GraphiC) h, TRUE, FALSE);
#endif
#ifdef WIN_MSWIN
      EnableWindow (h, TRUE);
#endif
#ifdef WIN_MOTIF
      Nlm_DoEnable ((Nlm_GraphiC) h, TRUE, FALSE);
#endif
    }
    g = Nlm_GetChild (a);
    while (g != NULL) {
      Nlm_DoEnable (g, FALSE, FALSE);
      g = Nlm_GetNext (g);
    }
  }
  Nlm_RestorePort (tempPort);
}

static void Nlm_DisableGroup (Nlm_GraphiC a, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_GraphiC    g;
  Nlm_GroupTool  h;
  Nlm_WindoW     tempPort;

  tempPort = Nlm_SavePortIfNeeded (a, savePort);
  if (setFlag) {
    Nlm_SetEnabled (a, FALSE);
  }
  h = Nlm_GetGroupHandle ((Nlm_GrouP) a);
  if (h != NULL) {
#ifdef WIN_MAC
    Nlm_DoDisable ((Nlm_GraphiC) h, TRUE, FALSE);
#endif
#ifdef WIN_MSWIN
    EnableWindow (h, FALSE);
#endif
#ifdef WIN_MOTIF
    Nlm_DoDisable ((Nlm_GraphiC) h, TRUE, FALSE);
#endif
  }
  g = Nlm_GetChild (a);
  while (g != NULL) {
    Nlm_DoDisable (g, FALSE, FALSE);
    g = Nlm_GetNext (g);
  }
  Nlm_RestorePort (tempPort);
}

static void Nlm_ActivateGroup (Nlm_GraphiC a, Nlm_Boolean savePort)

{
  Nlm_GraphiC  g;

  g = Nlm_GetChild (a);
  while (g != NULL) {
    Nlm_DoActivate (g, FALSE);
    g = Nlm_GetNext (g);
  }
}

static void Nlm_DeactivateGroup (Nlm_GraphiC a, Nlm_Boolean savePort)

{
  Nlm_GraphiC  g;

  g = Nlm_GetChild (a);
  while (g != NULL) {
    Nlm_DoDeactivate (g, FALSE);
    g = Nlm_GetNext (g);
  }
}

static void Nlm_ResetGroup (Nlm_GraphiC a, Nlm_Boolean savePort)

{
  Nlm_GraphiC  g;
  Nlm_GraphiC  n;
  Nlm_WindoW   tempPort;

  tempPort = Nlm_SavePortIfNeeded (a, savePort);
  g = Nlm_GetChild (a);
  while (g != NULL) {
    n = Nlm_GetNext (g);
    Nlm_DoReset (g, FALSE);
    g = n;
  }
  Nlm_RestorePort (tempPort);
}

static void Nlm_RemoveGroup (Nlm_GraphiC a, Nlm_Boolean savePort)

{
  Nlm_GraphiC    g;
  Nlm_GroupTool  h;
  Nlm_GraphiC    n;
  Nlm_WindoW     tempPort;
#ifdef WIN_MOTIF
  Nlm_GroupData  gdata;
#endif

  tempPort = Nlm_SavePortIfNeeded (a, savePort);
  g = Nlm_GetChild (a);
  while (g != NULL) {
    n = Nlm_GetNext (g);
    Nlm_DoRemove (g, FALSE);
    g = n;
  }
  h = Nlm_GetGroupHandle ((Nlm_GrouP) a);
  if (h != NULL) {
#ifdef WIN_MAC
    Nlm_DoRemove ((Nlm_GraphiC) h, FALSE);
#endif
#ifdef WIN_MSWIN
    DestroyWindow (h);
#endif
#ifdef WIN_MOTIF
    Nlm_DoRemove ((Nlm_GraphiC) h, FALSE);
    Nlm_GetGroupData ((Nlm_GrouP) a, &gdata);
    XtDestroyWidget (gdata.left);
    XtDestroyWidget (gdata.top);
    XtDestroyWidget (gdata.right);
    XtDestroyWidget (gdata.bottom);
#endif
  }
  Nlm_RemoveLink (a);
  recentGroup = NULL;
  Nlm_RestorePort (tempPort);
}

static void Nlm_SetGroupTitle (Nlm_GraphiC a, Nlm_Int2 item,
                               Nlm_CharPtr title, Nlm_Boolean savePort)

{
  Nlm_GraphiC    g;
  Nlm_GroupTool  h;
  Nlm_WindoW     tempPort;

  tempPort = Nlm_SavePortIfNeeded (a, savePort);
  if (item > 0) {
    g = Nlm_FindItem (a, item);
    if (g != NULL) {
      Nlm_DoSetTitle (g, 0, title, FALSE);
    }
  } else {
    h = Nlm_GetGroupHandle ((Nlm_GrouP) a);
    if (h != NULL) {
#ifdef WIN_MAC
      Nlm_DoSetTitle ((Nlm_GraphiC) h, 0, title, FALSE);
#endif
#ifdef WIN_MSWIN
      SetWindowText (h, title);
#endif
#ifdef WIN_MOTIF
#endif
    }
  }
  Nlm_RestorePort (tempPort);
}

static void Nlm_GetGroupTitle (Nlm_GraphiC a, Nlm_Int2 item,
                               Nlm_CharPtr title, size_t maxsize)

{
  Nlm_GraphiC    g;
  Nlm_GroupTool  h;
#ifdef WIN_MSWIN
  Nlm_Char       temp [256];
#endif

  if (item > 0) {
    g = Nlm_FindItem (a, item);
    if (g != NULL) {
      Nlm_DoGetTitle (g, 0, title, maxsize);
    }
  } else {
    h = Nlm_GetGroupHandle ((Nlm_GrouP) a);
    if (h != NULL) {
#ifdef WIN_MAC
      Nlm_DoGetTitle ((Nlm_GraphiC) h, 0, title, maxsize);
#endif
#ifdef WIN_MSWIN
      GetWindowText (h, temp, sizeof (temp));
      Nlm_StringNCpy_0 (title, temp, maxsize);
#endif
#ifdef WIN_MOTIF
#endif
    }
  }
}

static void Nlm_SetGroupValue (Nlm_GraphiC a, Nlm_Int2 value, Nlm_Boolean savePort)

{
  Nlm_GraphiC  g;
  Nlm_WindoW   tempPort;

  tempPort = Nlm_SavePortIfNeeded (a, savePort);
  if (value > 0) {
    g = Nlm_FindItem (a, value);
    Nlm_ClearItemsInGroup (a, g, FALSE);
    if (g != NULL) {
      Nlm_DoSetStatus (g, 0, TRUE, FALSE);
    }
  } else {
    Nlm_ClearItemsInGroup (a, NULL, FALSE);
  }
  Nlm_RestorePort (tempPort);
}

static Nlm_Int2 Nlm_GetGroupValue (Nlm_GraphiC a)

{
  Nlm_Boolean  found;
  Nlm_GraphiC  g;
  Nlm_Int2     i;
  Nlm_Int2     rsult;

  rsult = FALSE;
  g = Nlm_GetChild (a);
  i = 0;
  found = FALSE;
  while (g != NULL && ! found) {
    i++;
    found = Nlm_DoGetStatus (g, 0);
    g = Nlm_GetNext (g);
  }
  if (found) {
    rsult = i;
  }
  return rsult;
}

static void Nlm_SetGroupStatus (Nlm_GraphiC a, Nlm_Int2 item,
                                Nlm_Boolean value, Nlm_Boolean savePort)

{
  Nlm_GraphiC  g;
  Nlm_WindoW   tempPort;

  tempPort = Nlm_SavePortIfNeeded (a, savePort);
  if (item > 0) {
    g = Nlm_FindItem (a, item);
    if (g != NULL) {
      Nlm_DoSetStatus (g, 0, value, FALSE);
    }
  }
  Nlm_RestorePort (tempPort);
}

static Nlm_Boolean Nlm_GetGroupStatus (Nlm_GraphiC a, Nlm_Int2 item)

{
  Nlm_GraphiC  g;
  Nlm_Boolean  rsult;

  rsult = FALSE;
  if (item > 0) {
    g = Nlm_FindItem (a, item);
    if (g != NULL) {
      rsult = Nlm_DoGetStatus (g, 0);
    }
  }
  return rsult;
}

#ifdef WIN_MAC
static void Nlm_InvalItem (Nlm_GraphiC a)

{
  Nlm_RecT  r;

  if (Nlm_GetVisible (a) && Nlm_GetAllParentsVisible (a)) {
    Nlm_GetRect (a, &r);
    Nlm_InsetRect (&r, -1, -1);
    Nlm_InvalRect (&r);
  }
}
#endif

static void Nlm_InvalGroup (Nlm_GraphiC a)

{
#ifndef WIN_MOTIF
  Nlm_RecT        r;
#endif
#ifdef WIN_MSWIN
  Nlm_RectTool    rtool;
  Nlm_WindowTool  wptr;
#endif

  if (Nlm_GetVisible (a) && Nlm_GetAllParentsVisible (a)) {
#ifdef WIN_MAC
    Nlm_GetRect (a, &r);
    Nlm_InsetRect (&r, -1, -1);
    Nlm_InvalRect (&r);
#endif
#ifdef WIN_MSWIN
    wptr = Nlm_ParentWindowPtr (a);
    Nlm_GetRect (a, &r);
    r.top = r.bottom - 1;
    r.bottom++;
    Nlm_RecTToRectTool (&r, &rtool);
    InvalidateRect (wptr, &rtool, TRUE);
    Nlm_GetRect (a, &r);
    r.left = r.right - 1;
    r.right++;
    Nlm_RecTToRectTool (&r, &rtool);
    InvalidateRect (wptr, &rtool, TRUE);
#endif
  }
}


static void Nlm_SetGroupPosition (Nlm_GraphiC a, Nlm_RectPtr r, Nlm_Boolean savePort, Nlm_Boolean force)

{
  Nlm_GraphiC    g;
  Nlm_RecT       gr;
  Nlm_GroupTool  h;
  Nlm_RecT       oldRect;
  Nlm_WindoW     tempPort;
  Nlm_Int2       deltaX;
  Nlm_Int2       deltaY;
#if defined(WIN_MAC) || defined(WIN_MOTIF)
  Nlm_Int2       deltaH;
  Nlm_Int2       deltaW;
#endif
#ifdef WIN_MOTIF
  Nlm_GroupData  gdata;
#endif
  Nlm_Boolean is_realized;

  if (r == NULL)  return;

  Nlm_DoGetPosition (a, &oldRect);
  if (!force  &&  Nlm_EqualRect(r, &oldRect))  return;

  is_realized = Nlm_GetRealized( a );
  tempPort = Nlm_SavePortIfNeeded(a, savePort);

  Nlm_GetRect(a, &oldRect);
  gr = *r;
  deltaX = gr.left - oldRect.left;
  deltaY = gr.top  - oldRect.top;
#if defined(WIN_MAC) || defined(WIN_MOTIF)
  deltaW =  (gr.right  - gr.left) - (oldRect.right  - oldRect.left);
  deltaH =  (gr.bottom - gr.top ) - (oldRect.bottom - oldRect.top );
#endif

  if ( is_realized )
    Nlm_InvalGroup( a );

  Nlm_SetRect(a, &gr);

  h = Nlm_GetGroupHandle((Nlm_GrouP) a);
  if (h != NULL)
    {
#if defined(WIN_MAC) || defined(WIN_MOTIF)
      Nlm_GetRect ((Nlm_GraphiC) h, &gr);
      Nlm_UpsetRect (&gr, deltaX, deltaY, -deltaX, -deltaY);
      if (force  &&
	  is_realized  &&  !Nlm_GetRealized((Nlm_GraphiC) h )  &&
	  !deltaX  &&  !deltaY  &&  !deltaW  &&  !deltaH)
	Nlm_SetRealized((Nlm_GraphiC) h, TRUE);

      Nlm_DoSetPosition ((Nlm_GraphiC) h, &gr, FALSE, force);
#endif

      if ( is_realized )
	{
#ifdef WIN_MAC
	  Nlm_InvalGroup( a );
#endif
#ifdef WIN_MSWIN
	  MoveWindow (h,  gr.left,  gr.top,
		      gr.right - gr.left,  gr.bottom - gr.top,  TRUE);
#endif
#ifdef WIN_MOTIF
	  Nlm_GetRect (a, &gr);
	  gr.top += Nlm_stdLineHeight / 2;
	  Nlm_GetGroupData ((Nlm_GrouP) a, &gdata);
	  XtVaSetValues (gdata.left,
			 XmNx, (Position) gr.left,
			 XmNy, (Position) gr.top,
			 XmNwidth, (Dimension) 1,
			 XmNheight, (Dimension) (gr.bottom - gr.top), 
			 NULL);
	  XtVaSetValues (gdata.top,
			 XmNx, (Position) gr.left,
			 XmNy, (Position) gr.top,
			 XmNwidth, (Dimension) (gr.right - gr.left),
			 XmNheight, (Dimension) 1, 
			 NULL);
	  XtVaSetValues (gdata.right,
			 XmNx, (Position) gr.right,
			 XmNy, (Position) gr.top,
			 XmNwidth, (Dimension) 1,
			 XmNheight, (Dimension) (gr.bottom - gr.top), 
			 NULL);
	  XtVaSetValues (gdata.bottom,
			 XmNx, (Position) gr.left,
			 XmNy, (Position) gr.bottom,
			 XmNwidth, (Dimension) (gr.right - gr.left + 1),
			 XmNheight, (Dimension) 1, 
			 NULL);
#endif
	}
    }

  g = Nlm_GetChild( a );
  while (g != NULL) {
    Nlm_DoGetPosition (g, &gr);
    Nlm_OffsetRect (&gr, deltaX, deltaY);
    Nlm_DoSetPosition (g, &gr, FALSE, force);
    g = Nlm_GetNext( g );
  }

#ifdef WIN_MSWIN
  if (is_realized  &&  (h != NULL))
  {
    Nlm_RecT        r;
    Nlm_RectTool    rtool;
    Nlm_WindowTool  wptr;
    wptr = Nlm_ParentWindowPtr (a);
    Nlm_GetRect (a, &r);
    Nlm_RecTToRectTool (&r, &rtool);
    InvalidateRect (wptr, &rtool, TRUE);
  }
#endif

  Nlm_RestorePort( tempPort );
}


static void Nlm_GetGroupPosition (Nlm_GraphiC a, Nlm_RectPtr r)

{
  Nlm_RecT  gr;

  Nlm_GetRect (a, &gr);
  if (r != NULL) {
    *r = gr;
  }
}

static Nlm_GraphiC Nlm_GroupGainFocus (Nlm_GraphiC a, Nlm_Char ch, Nlm_Boolean savePort)

{
  Nlm_GraphiC  p;
  Nlm_GraphiC  q;
  Nlm_GraphiC  n;

  q = NULL;
  if (Nlm_GetVisible (a) && Nlm_GetEnabled (a)) {
    p = Nlm_GetChild (a);
    while (p != NULL && q == NULL) {
      n = Nlm_GetNext (p);
      q = Nlm_DoGainFocus (p, ch, savePort);
      p = n;
    }
  }
  return q;
}

static void Nlm_GroupLoseFocus (Nlm_GraphiC a, Nlm_GraphiC excpt, Nlm_Boolean savePort)

{
  Nlm_GraphiC  p;

  p = Nlm_GetChild (a);
  while (p != NULL) {
    Nlm_DoLoseFocus (p, excpt, savePort);
    p = Nlm_GetNext (p);
  }
}

static void Nlm_RepositionHorizItems (Nlm_GraphiC a, Nlm_Int2 count,
                                      Nlm_Int2 width, Nlm_Boolean align)

{
  Nlm_BoxData  bdata;
  Nlm_Int2     col;
  Nlm_Int2     column;
  Nlm_Int2     curWdt;
  Nlm_GrouP    g;
  Nlm_Int2     i;
  Nlm_Int2     newWdt;
  Nlm_Int2     offset;
  Nlm_GraphiC  p;
  Nlm_RecT     r;
  Nlm_RecT     s;
  Nlm_GraphiC  top;
  Nlm_RecT     topRect;
  Nlm_Int2     topWdt;

  column = (count - 1) % width + 1;
  g = (Nlm_GrouP) Nlm_GetParent (a);
  top = Nlm_FindItem ((Nlm_GraphiC) g, column);
  Nlm_DoGetPosition (a, &r);
  Nlm_DoGetPosition (top, &topRect);
  curWdt = r.right - r.left;
  topWdt = topRect.right - topRect.left;
  newWdt = MAX (curWdt, topWdt);
  if (curWdt < newWdt) {
    r.right = r.left + newWdt;
    Nlm_DoSetPosition (a, &r, FALSE, FALSE);
    curWdt = newWdt;
  }
  if (newWdt > topWdt) {
    offset = newWdt - topWdt;
    i = 1;
    p = Nlm_GetChild ((Nlm_GraphiC) g);
    while (p != NULL) {
      col = (i - 1) % width + 1;
      if (col == column) {
        Nlm_DoGetPosition (p, &r);
        r.right = r.left + newWdt;
        Nlm_DoSetPosition (p, &r, FALSE, FALSE);
        Nlm_RecordRect ((Nlm_GraphiC) g, &r);
      } else if (col > column) {
        Nlm_DoGetPosition (p, &r);
        Nlm_OffsetRect (&r, offset, 0);
        Nlm_DoSetPosition (p, &r, FALSE, FALSE);
        Nlm_RecordRect ((Nlm_GraphiC) g, &r);
      }
      i++;
      p = Nlm_GetNext (p);
    }
  }
  Nlm_GetRect ((Nlm_GraphiC) g, &r);
  Nlm_GetRect ((Nlm_GraphiC) g, &s);
  Nlm_GetBoxData ((Nlm_BoX) g, &bdata);
  s.right = MAX (r.right, bdata.limitPoint.x + bdata.xMargin);
  s.bottom = bdata.limitPoint.y + bdata.yMargin;
  Nlm_SetRect ((Nlm_GraphiC) g, &s);
  r.right = bdata.limitPoint.x + bdata.xMargin;
  r.bottom = bdata.limitPoint.y + bdata.yMargin;
  Nlm_DoAdjustPrnt ((Nlm_GraphiC) g, &r, align, FALSE);
  Nlm_DoGetPosition (a, &r);
  bdata.nextCol = r.right + bdata.xSpacing;
  Nlm_SetBoxData ((Nlm_BoX) g, &bdata);
}

static void Nlm_RepositionVertItems (Nlm_GraphiC a, Nlm_Int2 count,
                                     Nlm_Int2 height, Nlm_Boolean align)

{
  Nlm_BoxData  bdata;
  Nlm_Int2     column;
  Nlm_Int2     curWdt;
  Nlm_GrouP    g;
  Nlm_Int2     newWdt;
  Nlm_GraphiC  p;
  Nlm_RecT     r;
  Nlm_RecT     s;
  Nlm_GraphiC  top;
  Nlm_RecT     topRect;
  Nlm_Int2     topWdt;

  column = (count - 1) / height + 1;
  g = (Nlm_GrouP) Nlm_GetParent (a);
  top = Nlm_FindItem ((Nlm_GraphiC) g, (column - 1) * height + 1);
  Nlm_DoGetPosition (a, &r);
  Nlm_DoGetPosition (top, &topRect);
  curWdt = r.right - r.left;
  topWdt = topRect.right - topRect.left;
  newWdt = MAX (curWdt, topWdt);
  if (curWdt < newWdt) {
    r.right = r.left + newWdt;
    Nlm_DoSetPosition (a, &r, FALSE, FALSE);
    Nlm_RecordRect ((Nlm_GraphiC) g, &r);
    curWdt = newWdt;
  }
  if (newWdt > topWdt) {
    p = top;
    while (p != NULL && p != a) {
      Nlm_DoGetPosition (p, &r);
      r.right = r.left + newWdt;
      Nlm_DoSetPosition (p, &r, FALSE, FALSE);
      Nlm_RecordRect ((Nlm_GraphiC) g, &r);
      p = Nlm_GetNext (p);
    }
  }
  Nlm_GetRect ((Nlm_GraphiC) g, &r);
  Nlm_GetRect ((Nlm_GraphiC) g, &s);
  Nlm_GetBoxData ((Nlm_BoX) g, &bdata);
  s.right = MAX (r.right, bdata.limitPoint.x + bdata.xMargin);
  s.bottom = bdata.limitPoint.y + bdata.yMargin;
  Nlm_SetRect ((Nlm_GraphiC) g, &s);
  r.right = bdata.limitPoint.x + bdata.xMargin;
  r.bottom = bdata.limitPoint.y + bdata.yMargin;
  Nlm_DoAdjustPrnt ((Nlm_GraphiC) g, &r, align, FALSE);
  Nlm_DoGetPosition (a, &r);
  bdata.nextCol = r.right + bdata.xSpacing;
  Nlm_SetBoxData ((Nlm_BoX) g, &bdata);
}

static void Nlm_FinishItemPositioning (Nlm_GraphiC a, Nlm_RectPtr subRect,
                                       Nlm_Boolean align, Nlm_Boolean savePort)

{
  Nlm_BoxData    bdata;
  Nlm_Int2       count;
  Nlm_GrouP      g;
  Nlm_Int2       height;
  Nlm_RecT       r;
  Nlm_RecT       s;
  Nlm_WindoW     tempPort;
  Nlm_Int2       width;
#ifdef WIN_MAC
  Nlm_GroupData  gdata;
  Nlm_GroupTool  h;
  Nlm_RecT       hr;
  Nlm_Int2       mid;
  Nlm_Int2       wid;
#endif
#ifdef WIN_MSWIN
  Nlm_GroupTool  h;
#endif
#ifdef WIN_MOTIF
  Nlm_GroupData  gdata;
  Nlm_RecT       gr;
  Nlm_GroupTool  h;
#endif

  tempPort = Nlm_SavePortIfNeeded (a, savePort);
  g = (Nlm_GrouP) Nlm_GetParent (a);
  Nlm_InvalGroup ((Nlm_GraphiC) g);
  Nlm_RecordRect ((Nlm_GraphiC) g, subRect);
  if (align) {
    Nlm_GetBoxData ((Nlm_BoX) g, &bdata);
    if (bdata.boxWidth != 0 || bdata.boxHeight != 0) {
      Nlm_NextPosition ((Nlm_GraphiC) g, subRect);
    }
  }
  Nlm_GetRect ((Nlm_GraphiC) g, &r);
  Nlm_GetRect ((Nlm_GraphiC) g, &s);
  Nlm_GetBoxData ((Nlm_BoX) g, &bdata);
  s.right = MAX (r.right, bdata.limitPoint.x + bdata.xMargin);
  s.bottom = bdata.limitPoint.y + bdata.yMargin;
  Nlm_SetRect ((Nlm_GraphiC) g, &s);
  r.right = bdata.limitPoint.x + bdata.xMargin;
  r.bottom = bdata.limitPoint.y + bdata.yMargin;
  Nlm_DoAdjustPrnt ((Nlm_GraphiC) g, &r, align, FALSE);
#ifdef WIN_MAC
  Nlm_GetRect ((Nlm_GraphiC) g, &r);
  Nlm_GetGroupData ((Nlm_GrouP) g, &gdata);
  if (gdata.style > 0) {
    h = gdata.handle;
    if (h != NULL) {
      Nlm_GetPosition ((Nlm_GraphiC) h, &hr);
      wid = hr.right - hr.left;
      mid = (r.left + r.right) / 2;
      hr.left = mid - (wid / 2);
      hr.right = hr.left + wid;
      Nlm_SetPosition ((Nlm_GraphiC) h, &hr);
    }
  }
#endif
  if (align) {
    count = Nlm_CountGroupItems ((Nlm_GraphiC) g);
    width = bdata.boxWidth;
    height = bdata.boxHeight;
    if (width != 0) {
      if (width > 0 && count > width) {
        Nlm_RepositionHorizItems (a, count, width, align);
      }
      Nlm_Advance ((Nlm_GraphiC) g);
      if ((count %  ABS (width)) == 0) {
        Nlm_Break ((Nlm_GraphiC) g);
      }
    } else if (height != 0) {
      if (height > 0 && count > 1) {
        Nlm_RepositionVertItems (a, count, height, align);
      }
      if ((count % ABS (height)) == 0) {
        Nlm_Advance ((Nlm_GraphiC) g);
      }
    }
  }
#ifdef WIN_MAC
  Nlm_InvalGroup ((Nlm_GraphiC) g);
  Nlm_InvalItem (a);
#endif
#ifdef WIN_MSWIN
  Nlm_GetRect ((Nlm_GraphiC) g, &r);
  h = Nlm_GetGroupHandle (g);
  if (h != NULL) {
    MoveWindow (h, r.left, r.top, r.right - r.left, r.bottom - r.top, TRUE);
    UpdateWindow (h);
  }
#endif
#ifdef WIN_MOTIF
  h = Nlm_GetGroupHandle (g);
  if (h != NULL) {
    Nlm_GetRect ((Nlm_GraphiC) g, &gr);
    gr.top += Nlm_stdLineHeight / 2;
    Nlm_GetGroupData ((Nlm_GrouP) g, &gdata);
    XtVaSetValues (gdata.left,
                   XmNx, (Position) gr.left,
                   XmNy, (Position) gr.top,
                   XmNwidth, (Dimension) 1,
                   XmNheight, (Dimension) (gr.bottom - gr.top), 
                   NULL);
    XtVaSetValues (gdata.top,
                   XmNx, (Position) gr.left,
                   XmNy, (Position) gr.top,
                   XmNwidth, (Dimension) (gr.right - gr.left),
                   XmNheight, (Dimension) 1, 
                   NULL);
    XtVaSetValues (gdata.right,
                   XmNx, (Position) gr.right,
                   XmNy, (Position) gr.top,
                   XmNwidth, (Dimension) 1,
                   XmNheight, (Dimension) (gr.bottom - gr.top), 
                   NULL);
    XtVaSetValues (gdata.bottom,
                   XmNx, (Position) gr.left,
                   XmNy, (Position) gr.bottom,
                   XmNwidth, (Dimension) (gr.right - gr.left + 1),
                   XmNheight, (Dimension) 1, 
                   NULL);
  }
#endif
  Nlm_RestorePort (tempPort);
}


#ifdef WIN_MOTIF
static Nlm_LineTool Nlm_CreateSeparator (Nlm_GrouP g, Nlm_RectPtr gr, Nlm_Int2 which)

{
  Nlm_LineTool    h;
  Dimension       height;
  Cardinal        n;
  unsigned char   orient;
  Nlm_RecT        r;
  unsigned char   separat;
  Arg             wargs [20];
  Dimension       width;
  Nlm_WindowTool  wptr;

  r = *gr;
  r.top += Nlm_stdLineHeight / 2;
  switch (which) {
    case 1:
      orient = XmVERTICAL;
      separat = XmSINGLE_LINE;
      r.right = r.left;
      width = (Dimension) 1;
      height = (Dimension) (r.bottom - r.top);
      break;
    case 2:
      orient = XmHORIZONTAL;
      separat = XmSINGLE_LINE;
      r.bottom = r.top;
      width = (Dimension) (r.right - r.left);
      height = (Dimension) 1;
      break;
    case 3:
      orient = XmVERTICAL;
      separat = XmSINGLE_LINE;
      r.left = r.right;
      width = (Dimension) 1;
      height = (Dimension) (r.bottom - r.top);
      break;
    case 4:
      orient = XmHORIZONTAL;
      separat = XmSINGLE_LINE;
      r.top = r.bottom;
      width = (Dimension) (r.right - r.left + 1);
      height = (Dimension) 1;
      break;
    default:
      width = (Dimension) (r.right - r.left);
      height = (Dimension) (r.bottom - r.top);
      orient = XmHORIZONTAL;
      separat = XmSINGLE_LINE;
      break;
  }
  n = 0;
  XtSetArg (wargs [n], XmNx, (Position) r.left); n++;
  XtSetArg (wargs [n], XmNy, (Position) r.top); n++;
  XtSetArg (wargs [n], XmNwidth, (Dimension) width); n++;
  XtSetArg (wargs [n], XmNheight, (Dimension) height); n++;
  XtSetArg (wargs [n], XmNmarginHeight, 0); n++;
  XtSetArg (wargs [n], XmNmarginWidth, 0); n++;
  XtSetArg (wargs [n], XmNborderWidth, (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNmargin, (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNhighlightThickness, (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNshadowThickness, (Dimension) 0); n++;
  XtSetArg (wargs [n], XmNorientation, orient); n++;
  XtSetArg (wargs [n], XmNseparatorType, separat); n++;
  wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) g);
  h = XmCreateSeparator (wptr, (String) "", wargs, n);
  XtManageChild (h);
  return h;
}
#endif


static void Nlm_NewGroup (Nlm_GrouP g, Nlm_CharPtr title,
                          Nlm_Int2 width, Nlm_Int2 height,
                          Nlm_Boolean visible, Nlm_RectPtr pr,
                          Nlm_FonT font, Nlm_Int2 styl,
                          Nlm_GrpActnProc actn)

{
  Nlm_LineTool    bt;
  Nlm_GroupTool   h;
  Nlm_LineTool    lf;
  Nlm_PoinT       pt;
  Nlm_RecT        r;
  Nlm_LineTool    rt;
  Nlm_LineTool    tp;
#ifdef WIN_MSWIN
  Nlm_FntPtr      fntptr;
  Nlm_WindowTool  wptr;
#endif

  lf = NULL;
  tp = NULL;
  rt = NULL;
  bt = NULL;
  Nlm_GetRect ((Nlm_GraphiC) g, &r);
  if (styl > 0) {
    pt.x = r.left + 3;
    pt.y = r.top + Nlm_LineHeight () + 13;
  } else if (visible) {
    pt.x = r.left + 3;
    pt.y = r.top + Nlm_LineHeight () + 2;
  } else {
    pt.x = r.left;
    pt.y = r.top;
  }
  if (styl > 0) {
    Nlm_LoadBoxData ((Nlm_BoX) g, pt, pt, pt, pt.y, pt.x, 12, 12, 3, 2, width, height);
  } else if (visible) {
    Nlm_LoadBoxData ((Nlm_BoX) g, pt, pt, pt, pt.y, pt.x, 3, 3, 3, 2, width, height);
  } else {
    Nlm_LoadBoxData ((Nlm_BoX) g, pt, pt, pt, pt.y, pt.x, 0, 0, 3, 2, width, height);
  }
  h = NULL;
  if (visible) {
#ifdef WIN_MAC
    if (styl > 0) {
      h = Nlm_DependentPrompt ((Nlm_GraphiC) g, pr, title, font, 'c');
    } else {
      h = Nlm_DependentPrompt ((Nlm_GraphiC) g, pr, title, font, 'l');
    }
#endif
#ifdef WIN_MSWIN
    wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) g);
    h = CreateWindow ("Button", title, WS_CHILD | BS_GROUPBOX,
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
    h = Nlm_DependentPrompt ((Nlm_GraphiC) g, pr, title, font, 'l');
    lf = Nlm_CreateSeparator (g, &r, 1);
    tp = Nlm_CreateSeparator (g, &r, 2);
    rt = Nlm_CreateSeparator (g, &r, 3);
    bt = Nlm_CreateSeparator (g, &r, 4);
#endif
  }
  Nlm_LoadGroupData (g, h, lf, tp, rt, bt, styl);
  Nlm_LoadAction ((Nlm_GraphiC) g, (Nlm_ActnProc) actn);
}

static Nlm_GrouP Nlm_CreateGroup (Nlm_GrouP prnt, Nlm_Int2 recordSize,
                                  Nlm_Int2 width, Nlm_Int2 height,
                                  Nlm_Boolean visible, Nlm_CharPtr title,
                                  Nlm_FonT font, Nlm_Int2 styl,
                                  Nlm_GrpActnProc actn)

{
  Nlm_GrouP   g;
  Nlm_Int2    lf;
  Nlm_Int2    mid;
  Nlm_PoinT   npt;
  Nlm_RecT    pr;
  Nlm_RecT    r;
  Nlm_WindoW  tempPort;
  Nlm_Int2    top;
  Nlm_Int2    wid;

  g = NULL;
  if (prnt != NULL) {
    tempPort = Nlm_SavePort ((Nlm_GraphiC) prnt);
    Nlm_GetNextPosition ((Nlm_GraphiC) prnt, &npt);
    if (font == NULL) {
      font = Nlm_systemFont;
    }
    Nlm_SelectFont (font);
    wid = Nlm_StringWidth (title);
#ifdef WIN_MAC
    top = Nlm_LineHeight ();
#endif
#ifdef WIN_MSWIN
    top = Nlm_LineHeight ();
#endif
#ifdef WIN_MOTIF
    top = Nlm_LineHeight ();
#endif
    if (visible) {
      Nlm_LoadRect (&r, npt.x, npt.y, npt.x + wid + 20, npt.y + 23);
    } else {
      Nlm_LoadRect (&r, npt.x, npt.y, npt.x, npt.y);
    }
    g = (Nlm_GrouP) Nlm_CreateLink ((Nlm_GraphiC) prnt, &r, recordSize, groupProcs);
    if (g != NULL) {
      Nlm_GetRect ((Nlm_GraphiC) g, &r);
      if (styl == 0) {
        Nlm_LoadRect (&pr, r.left + 4, r.top, r.left + wid + 6, r.top + top);
      } else {
        mid = (r.left + r.right) / 2;
        wid += 10;
        lf = mid - (wid / 2) - 1;
        Nlm_LoadRect (&pr, lf, r.top + 2, lf + wid, r.top + top + 2);
      }
      Nlm_NewGroup (g, title, width, height, visible, &pr, font, styl, actn);
      Nlm_DoAdjustPrnt ((Nlm_GraphiC) g, &r, TRUE, FALSE);
      Nlm_DoShow ((Nlm_GraphiC) g, TRUE, FALSE);
    }
    Nlm_RestorePort (tempPort);
  }
  return g;
}

extern Nlm_GrouP Nlm_NormalGroup (Nlm_GrouP prnt, Nlm_Int2 width,
                                  Nlm_Int2 height, Nlm_CharPtr title,
                                  Nlm_FonT font, Nlm_GrpActnProc actn)

{
  Nlm_GrouP  g;

  g = Nlm_CreateGroup (prnt, sizeof (Nlm_GroupRec), width,
                       height, TRUE, title, font, 0, actn);
  return g;
}

extern Nlm_GrouP Nlm_HiddenGroup (Nlm_GrouP prnt, Nlm_Int2 width,
                                  Nlm_Int2 height, Nlm_GrpActnProc actn)

{
  Nlm_GrouP  g;

  g = Nlm_CreateGroup (prnt, sizeof (Nlm_GroupRec), width,
                       height, FALSE, "", NULL, 0, actn);
  return g;
}

extern Nlm_GrouP Nlm_WindowFrameGroup (Nlm_GrouP prnt, Nlm_Int2 width,
                                       Nlm_Int2 height, Nlm_CharPtr title,
                                       Nlm_FonT font, Nlm_GrpActnProc actn);
extern Nlm_GrouP Nlm_WindowFrameGroup (Nlm_GrouP prnt, Nlm_Int2 width,
                                       Nlm_Int2 height, Nlm_CharPtr title,
                                       Nlm_FonT font, Nlm_GrpActnProc actn)

{
  Nlm_GrouP  g;

  g = Nlm_CreateGroup (prnt, sizeof (Nlm_GroupRec), width,
                       height, TRUE, title, font, 1, actn);
  return g;
}

extern void Nlm_SetGroupMargins (Nlm_GrouP g, Nlm_Int2 xMargin, Nlm_Int2 yMargin)

{
  Nlm_BoxData    bdata;
  Nlm_GroupTool  h;
  Nlm_PoinT      pt;
  Nlm_RecT       r;

  if (g != NULL) {
    Nlm_GetRect ((Nlm_GraphiC) g, &r);
    Nlm_GetBoxData ((Nlm_BoX) g, &bdata);
    bdata.xMargin = xMargin;
    bdata.yMargin = yMargin;
    h = Nlm_GetGroupHandle (g);
    if (h != NULL) {
      pt.x = r.left + xMargin;
      pt.y = r.top + Nlm_stdLineHeight + yMargin;
    } else {
      pt.x = r.left + xMargin;
      pt.y = r.top + yMargin;
    }
    bdata.nextPoint = pt;
    bdata.limitPoint = pt;
    bdata.resetPoint = pt;
    bdata.nextCol = pt.x;
    bdata.topRow = pt.y;
    Nlm_SetBoxData ((Nlm_BoX) g, &bdata);
  }
}

extern void Nlm_SetGroupSpacing (Nlm_GrouP g, Nlm_Int2 xSpacing, Nlm_Int2 ySpacing)

{
  Nlm_BoxData  bdata;

  if (g != NULL) {
    Nlm_GetBoxData ((Nlm_BoX) g, &bdata);
    bdata.xSpacing = xSpacing;
    bdata.ySpacing = ySpacing;
    Nlm_SetBoxData ((Nlm_BoX) g, &bdata);
  }
}

extern void Nlm_FreeGroup (void)

{
  gphprcsptr = (Nlm_GphPrcsPtr) Nlm_MemFree (gphprcsptr);
}

extern void Nlm_InitGroup (void)

{
  gphprcsptr = (Nlm_GphPrcsPtr) Nlm_MemNew (sizeof (Nlm_GphPrcs) * 1);

  groupProcs = &(gphprcsptr [0]);
#ifdef WIN_MAC
  groupProcs->click = Nlm_GroupClick;
  groupProcs->key = Nlm_GroupKey;
  groupProcs->draw = Nlm_DrawGroup;
  groupProcs->idle = Nlm_IdleGroup;
#endif
  groupProcs->show = Nlm_ShowGroup;
  groupProcs->hide = Nlm_HideGroup;
  groupProcs->enable = Nlm_EnableGroup;
  groupProcs->disable = Nlm_DisableGroup;
  groupProcs->activate = Nlm_ActivateGroup;
  groupProcs->deactivate = Nlm_DeactivateGroup;
  groupProcs->remove = Nlm_RemoveGroup;
  groupProcs->reset = Nlm_ResetGroup;
  groupProcs->countItems = Nlm_CountGroupItems;
  groupProcs->linkIn = Nlm_LinkIn;
  groupProcs->adjustPrnt = Nlm_FinishItemPositioning;
  groupProcs->setTitle = Nlm_SetGroupTitle;
  groupProcs->getTitle = Nlm_GetGroupTitle;
  groupProcs->setValue = Nlm_SetGroupValue;
  groupProcs->getValue = Nlm_GetGroupValue;
  groupProcs->setStatus = Nlm_SetGroupStatus;
  groupProcs->getStatus = Nlm_GetGroupStatus;
  groupProcs->setPosition = Nlm_SetGroupPosition;
  groupProcs->getPosition = Nlm_GetGroupPosition;
  groupProcs->gainFocus = Nlm_GroupGainFocus;
  groupProcs->loseFocus = Nlm_GroupLoseFocus;
}
