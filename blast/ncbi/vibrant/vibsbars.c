/*   vibsbars.c
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
* File Name:  vibsbars.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   7/1/91
*
* $Revision: 6.3 $
*
* File Description:
*       Vibrant scroll bar functions
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: vibsbars.c,v $
* Revision 6.3  2001/04/05 03:23:07  juran
* Carbon UPP fix and removed pre-UPP support.
*
* Revision 6.2  1997/12/12 21:08:43  kans
* a number of symbols changed in the latest CodeWarrior release, now using headers from Apple
*
* Revision 6.1  1997/11/26 21:30:36  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.0  1997/08/25 18:57:35  madden
* Revision changed to 6.0
*
* Revision 5.10  1997/04/25 16:12:46  vakatov
* Process all line/page scrolling actions uniformly -- with the
* (new) Nlm_Scroll() function
*
 * Revision 5.9  1996/09/25  23:43:17  kans
 * setPos calls invalPos BEFORE user's callback, otherwise the scroll bar
 * thumb can have a noticable delay, e.g., a doc object pulling items from
 * the net to calculate the new bar maximum
 *
 * Revision 5.8  1996/09/24  14:21:57  kans
 * restored call to invalPos after call to user's callback, which was used
 * only to synchronize appearance of scroll bar with its internal state, not
 * to affect the position returned by GetValue
 *
 * Revision 5.7  1996/09/24  12:31:10  kans
 * ResetBar calls setMax with doCallback FALSE, to avoid an unwanted redraw,
 * and setPos calls invalPos before calling the user's callback, to ensure
 * that GetValue will be correct inside the callback.
 *
 * Revision 5.6  1996/09/19  18:51:39  vakatov
 * [WIN_MOTIF]  Set the scrollbar location and dimensions when creating
 * the new scrollbar
 *
 * Revision 5.5  1996/09/10  15:31:03  vakatov
 * [WIN_MOTIF]  Syncronized the scrollbar slider size with the page
 * size and maximum scrollbar value
 *
 * Revision 5.4  1996/09/06  17:26:16  kans
 * when wrapping with saveportifneeded, pass FALSE instead of savePort inside
 *
 * Revision 5.3  1996/09/06  15:07:54  vakatov
 * setPos():  Saved port before executing the scrollbar callback routine
 *
 * Revision 5.2  1996/08/27  20:48:10  vakatov
 * Rearranged and partially rewritten in order to expand maximum scrollbar
 * range from [0, INT2_MAX] to [0, INT4_MAX]
 *
 * Revision 5.1  1996/07/25  18:48:05  vakatov
 * Made Nlm_GetBarMax() external (it was static)
 *
 * Revision 5.0  1996/05/28  13:45:08  ostell
 * Set to revision 5.0
 *
 * Revision 4.3  1996/03/02  22:36:38  kans
 * reduction of X traffic (DV)
 *
 * Revision 4.2  1996/02/13  19:49:04  kans
 * scroll bar "realized" if force is TRUE, to handle cases where bar is
 * owned by a panel or text, not in the normal vibrant child list
 *
 * Revision 4.1  1996/02/13  17:24:07  kans
 * accelerated set position prior to realization (Denis Vakatov)
 *
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 2.18  1995/05/17  15:15:14  kans
 * added Log line
 *
*
* ==========================================================================
*/

#include <vibtypes.h>
#include <vibprocs.h>
#include <vibincld.h>


/*********************************************************************
 *  PRIVATE
 *********************************************************************/

/* Note that the scroll bar tool type is also duplicated in vibwndws.c */

#ifdef WIN_MAC
#define Nlm_BarTool ControlHandle
#endif

#ifdef WIN_MSWIN
#define Nlm_BarTool HWND
#endif

#ifdef WIN_MOTIF
#define Nlm_BarTool Widget
#endif

typedef  struct  Nlm_bardata
{
  Nlm_BarTool     handle;
  Nlm_Int4        pageUp;
  Nlm_Int4        pageDn;
  Nlm_Int4        delta;
  Nlm_Int4        delta_max;
  Nlm_Boolean     anomaly;
  Nlm_GraphiC     slave;

  Nlm_BarScrlProc4 action4;
  Nlm_BarScrlProc  action;

  Nlm_Int2        value;
  Nlm_Int2        maximum;

  Nlm_Int4        factor;
  Nlm_Int4        rem;
  Nlm_Int4        max;
  Nlm_Int4        pos;
}  Nlm_BarData;


typedef  struct  Nlm_barrec
{
  Nlm_GraphicRec  graphicR;
  Nlm_BarData     bar;
} Nlm_BarRec, PNTR Nlm_BarPtr;

static Nlm_BaR      recentBar = NULL;
static Nlm_BarData  recentBarData;


static void Nlm_SetBarData (Nlm_BaR b, Nlm_BarData *bdata)
{
  Nlm_BarPtr bp = (Nlm_BarPtr)Nlm_HandLock( b );
  if (bp == NULL)
    return;

  bp->bar = *bdata;
  Nlm_HandUnlock( b );

  recentBar = b;
  recentBarData = *bdata;
}

static void Nlm_GetBarData (Nlm_BaR b, Nlm_BarData *bdata)
{
  if (b == NULL)
    return;

  if (b == recentBar)
    *bdata = recentBarData;
  else
    {
      Nlm_BarPtr bp = (Nlm_BarPtr)Nlm_HandLock( b );
      *bdata = bp->bar;
      Nlm_HandUnlock( b );
      recentBar = b;
      recentBarData = *bdata;
    }
}


static void invalPos(Nlm_BaR b, Nlm_Boolean savePort)
{
  Nlm_BarData  bdata;
  Nlm_WindoW   tempPort;

  if (b == NULL)
    return;

  Nlm_GetBarData((Nlm_BaR)b, &bdata);
  if (bdata.delta == 0 /*  ||  !Nlm_GetEnabled( (Nlm_GraphiC)b ) */ )
    return;

  tempPort = Nlm_SavePortIfNeeded((Nlm_GraphiC)b, savePort);

#ifdef WIN_MAC
  SetControlValue(bdata.handle, bdata.value);
#endif
#ifdef WIN_MSWIN
  SetScrollPos(bdata.handle, SB_CTL, bdata.value, TRUE);
#endif
#ifdef WIN_MOTIF
  XtVaSetValues(bdata.handle, XmNvalue, (int)bdata.value, NULL);
#endif

  bdata.delta = 0;
  Nlm_SetBarData(b, &bdata);
  Nlm_RestorePort( tempPort );
}


static void setPos(Nlm_BaR b, Nlm_Int4 pos,
                   Nlm_Boolean savePort, Nlm_Boolean doCallback)
{
  Nlm_BarData  bdata;
  Nlm_Int4     prev_pos;
  Nlm_WindoW   tempPort;

  if (b == NULL)
    return;

  Nlm_GetBarData(b, &bdata);

  if (pos < 0)
    pos = 0;
  else if (pos > bdata.max)
    pos = bdata.max;

  if (pos == bdata.pos)
    return;
  
  prev_pos     = bdata.pos;
  bdata.delta += pos - bdata.pos;
  bdata.pos    = pos;
  bdata.rem    = pos % bdata.factor;
  bdata.value  = (Nlm_Int2)(pos / bdata.factor);
  ASSERT ( bdata.value <= bdata.maximum );
#if defined(TEST_MODULE_VIBSBARS)  &&  defined(OS_UNIX) 
fprintf(stderr, "dpos=%ld, newpos=%ld, newval=%ld\n",
        (long)bdata.delta, (long)bdata.pos, (long)bdata.value);
#endif
  Nlm_SetBarData(b, &bdata);

  tempPort = Nlm_SavePortIfNeeded((Nlm_GraphiC)b, savePort);
  invalPos(b, FALSE);

  if ( doCallback )
    {

      if (bdata.action4 != NULL)
        (*bdata.action4)(b, bdata.slave, bdata.pos, prev_pos);

      if (bdata.action != NULL)
        {
          Nlm_Int2 curr_pos_2 = (Nlm_Int2)
            ((bdata.pos < INT2_MAX) ? bdata.pos : INT2_MAX);
          Nlm_Int2 prev_pos_2 = (Nlm_Int2)
            ((prev_pos  < INT2_MAX) ? prev_pos  : INT2_MAX);

          (*bdata.action)(b, bdata.slave, curr_pos_2, prev_pos_2);
        }

    }

  Nlm_RestorePort( tempPort );
}


static Nlm_Int4 getPos(Nlm_BaR b)
{
  Nlm_BarData  bdata;
  if (b == NULL)
    return 0;

  Nlm_GetBarData(b, &bdata);

  return bdata.pos;
}


static void setValue(Nlm_BaR b, Nlm_Int2 value, Nlm_Boolean savePort)
{
  Nlm_BarData  bdata;
  Nlm_GetBarData(b, &bdata);

  setPos(b, value * bdata.factor, savePort, TRUE);
}


static void invalRange(Nlm_BaR b, Nlm_Boolean savePort)
{
  Nlm_BarData  bdata;
  Nlm_WindoW   tempPort;

  if (b == NULL  /* ||  !Nlm_GetEnabled( (Nlm_GraphiC)b ) */ )
    return;

  Nlm_GetBarData(b, &bdata);
  if (bdata.delta_max == 0  &&  bdata.max != 0)
    return;

  bdata.delta_max = 0;
  Nlm_SetBarData(b, &bdata);

  if (bdata.maximum < 0)
    return;

  tempPort = Nlm_SavePortIfNeeded((Nlm_GraphiC)b, savePort);

#ifdef WIN_MAC
  SetControlMaximum(bdata.handle, bdata.maximum);
#endif
#ifdef WIN_MSWIN
  SetScrollRange(bdata.handle, SB_CTL, 0, bdata.maximum, TRUE);
#endif
#ifdef WIN_MOTIF
  {{
    int page = ((bdata.pageUp + bdata.pageDn) / 2) / bdata.factor;
    page = MAX(1, page);
    page = MIN(page, (INT2_MAX - bdata.maximum - 1));
    XtVaSetValues(bdata.handle,
                  XmNmaximum, (int)(bdata.maximum + page), 
                  XmNpageIncrement, page,
                  XmNsliderSize,    page,
                  NULL);
  }}
#endif

  Nlm_RestorePort( tempPort );
}


static void setMax(Nlm_BaR b, Nlm_Int4 max,
                   Nlm_Boolean savePort, Nlm_Boolean doCallback)
{
  Nlm_BarData  bdata;
  Nlm_WindoW   tempPort;

  Nlm_GetBarData(b, &bdata);

  if (b == NULL)
    return;

  if (max < 0)
    max = 0;
  bdata.delta_max = max - bdata.max;
  bdata.max       = max;
  bdata.factor    = max / INT2_MAX + 1;
  bdata.maximum   = (Nlm_Int2)((max + bdata.factor - 1) / bdata.factor);
  Nlm_SetBarData(b, &bdata);
#if defined(TEST_MODULE_VIBSBARS)  &&  defined(OS_UNIX) 
fprintf(stderr, "dmax=%ld, newmax=%ld, newmaxval=%ld\n",
        (long)bdata.delta_max, (long)bdata.max, (long)bdata.maximum);
#endif
  tempPort = Nlm_SavePortIfNeeded((Nlm_GraphiC)b, savePort);
  if (bdata.delta_max < 0)
    setPos(b, bdata.pos, FALSE, doCallback);
  invalRange(b, FALSE);
  Nlm_RestorePort( tempPort );
}



static Nlm_GphPrcsPtr  gphprcsptr = NULL;

static Nlm_GphPrcsPtr  barProcs;
static Nlm_GphPrcsPtr  linkedBarProcs;

#ifdef WIN_MSWIN
static WNDPROC         lpfnNewBarProc = NULL;
static WNDPROC         lpfnOldBarProc = NULL;
static Nlm_Boolean     handlechar;
#endif


static void Nlm_LoadBarData (Nlm_BaR b, Nlm_BarTool hdl,
                             Nlm_Int4 pgUp, Nlm_Int4 pgDn,
                             Nlm_Int4 dlta, Nlm_Int4 val,
                             Nlm_Int4 max, Nlm_Boolean anomaly,
                             Nlm_GraphiC slv,
                             Nlm_BarScrlProc4 actn4, Nlm_BarScrlProc actn)
{
  Nlm_BarPtr       bp    = (Nlm_BarPtr)Nlm_HandLock( b );
  Nlm_BarData PNTR bdptr = &bp->bar;
  bdptr->handle    = hdl;
  bdptr->pageUp    = pgUp;
  bdptr->pageDn    = pgDn;
  bdptr->delta     = dlta;
  bdptr->delta_max = 0;
  bdptr->anomaly   = anomaly;
  bdptr->slave     = slv;
  bdptr->action4   = actn4;
  bdptr->action    = actn;
  setMax(b, max, FALSE, FALSE);
  setPos(b, val, FALSE, FALSE);
  Nlm_HandUnlock( b );
  recentBar = NULL;
}


static Nlm_BarTool Nlm_GetBarHandle (Nlm_BaR b)
{
  Nlm_BarData  bdata;

  Nlm_GetBarData(b, &bdata);
  return bdata.handle;
}


static Nlm_Int4 Nlm_GetBarPageUp (Nlm_BaR b)
{
  Nlm_BarData  bdata;

  Nlm_GetBarData (b, &bdata);
  return bdata.pageUp;
}


static Nlm_Int4 Nlm_GetBarPageDown (Nlm_BaR b)
{
  Nlm_BarData  bdata;

  Nlm_GetBarData (b, &bdata);
  return bdata.pageDn;
}


#ifdef WIN_MAC
pascal void ScrollAction PROTO((Nlm_BarTool c, Nlm_Int2 part));
pascal void ScrollAction(Nlm_BarTool c, Nlm_Int2 part)
{
  Nlm_enumScrollEvent event;

  switch ( part )
    {
    case kControlUpButtonPart:
      event = SCROLL_LINEUP;  break;
    case kControlDownButtonPart:
      event = SCROLL_LINEDN;  break;
    case kControlPageUpPart:
      event = SCROLL_PAGEUP;  break;
    case kControlPageDownPart:
      event = SCROLL_PAGEDN;  break;
    default:
      return;
    }

  Nlm_Scroll((Nlm_BaR)GetControlReference( c ), event);
}


static Nlm_Boolean Nlm_ScrollClick (Nlm_GraphiC b, Nlm_PoinT pt)
{
  Nlm_BarTool       c;
  Nlm_Int2          part;
  Nlm_PointTool     ptool;
  Nlm_RecT          r;

  Nlm_GetRect (b, &r);
  if ( !Nlm_PtInRect(pt, &r) )
    return FALSE;

  c = Nlm_GetBarHandle ( (Nlm_BaR)b );
  Nlm_PoinTToPointTool (pt, &ptool);

  part = TestControl(c, ptool);
  if (part == kControlIndicatorPart)
    {
      TrackControl(c, ptool, NULL);
      setValue((Nlm_BaR)b, GetControlValue(c), TRUE);
    }
  else
    {
      ControlActionUPP proc = NewControlActionUPP(ScrollAction);
      TrackControl(c, ptool, proc);
      DisposeControlActionUPP( proc );
    }

  return TRUE;
}
#endif


#ifdef WIN_MSWIN
static Nlm_Boolean Nlm_ScrollCommand(Nlm_GraphiC g)
{
  Nlm_enumScrollEvent event;

  switch ( Nlm_currentCode )
    {
    case SB_LINEUP:
      event = SCROLL_LINEUP;  break;
    case SB_LINEDOWN:
      event = SCROLL_LINEDN;  break;
    case SB_PAGEUP:
      event = SCROLL_PAGEUP;  break;
    case SB_PAGEDOWN:
      event = SCROLL_PAGEDN;  break;
    case SB_THUMBPOSITION:
      setValue((Nlm_BaR)g, (Nlm_Int2)Nlm_currentPos, TRUE);
      return TRUE;
    default:
      return FALSE;
    }

  Nlm_Scroll((Nlm_BaR)g, event);
  return TRUE;
}
#endif


#ifdef WIN_MAC
static void Nlm_DrawBar (Nlm_GraphiC b)
{
  Nlm_BarData  bdata;
  Nlm_BarTool  c;
  Nlm_RecT     r;

  if (!Nlm_GetVisible( b )  ||  !Nlm_GetAllParentsVisible( b ))
    return;

  Nlm_GetBarData ((Nlm_BaR) b, &bdata);
  Nlm_GetRect (b, &r);
  if ( !Nlm_RectInRgn(&r, Nlm_updateRgn) )
    return;

  c = bdata.handle;

  if (bdata.maximum != GetControlMaximum( c ))
    SetControlMaximum(c, bdata.maximum);
  if (bdata.value != GetControlValue( c ))
    SetControlValue(c, bdata.value);

  Draw1Control( c );
}
#endif


static void Nlm_ShowBar (Nlm_GraphiC b, Nlm_Boolean setFlag, Nlm_Boolean savePort)
{
  Nlm_BarTool  c;
  Nlm_WindoW   tempPort;

  if ( setFlag )
    Nlm_SetVisible(b, TRUE);

  if (!Nlm_GetVisible( b )  ||  !Nlm_AllParentsButWindowVisible( b ))
    return;

  tempPort = Nlm_SavePortIfNeeded(b, savePort);
  c = Nlm_GetBarHandle( (Nlm_BaR)b );

#ifdef WIN_MAC
  ShowControl( c );
  Nlm_DoDraw( b );
#endif
#ifdef WIN_MSWIN
  ShowWindow(c, SW_SHOW);
  UpdateWindow( c );
#endif
#ifdef WIN_MOTIF
  XtManageChild( c );
#endif

  Nlm_RestorePort( tempPort );
}


static void Nlm_HideBar (Nlm_GraphiC b, Nlm_Boolean setFlag, Nlm_Boolean savePort)
{
  Nlm_BarTool  c = Nlm_GetBarHandle( (Nlm_BaR)b );
  Nlm_WindoW   tempPort = Nlm_SavePortIfNeeded(b, savePort);
#ifdef WIN_MAC
  Nlm_RecT     r;
#endif

  if ( setFlag )
    Nlm_SetVisible(b, FALSE);

#ifdef WIN_MAC
  HideControl( c );
  if ( Nlm_GetAllParentsVisible(b) )
    {
      Nlm_GetRect (b, &r);
      Nlm_InsetRect (&r, -1, -1);
      Nlm_EraseRect (&r);
      Nlm_ValidRect (&r);
    }
#endif
#ifdef WIN_MSWIN
  ShowWindow(c, SW_HIDE);
  UpdateWindow( c );
#endif
#ifdef WIN_MOTIF
  XtUnmanageChild( c );
#endif

  Nlm_RestorePort( tempPort );
}


static void Nlm_EnableBar (Nlm_GraphiC b, Nlm_Boolean setFlag, Nlm_Boolean savePort)
{
  Nlm_BarData  bdata;
  Nlm_WindoW   tempPort;

  if ( setFlag )
    Nlm_SetEnabled(b, TRUE);

  if (!Nlm_GetEnabled( b )  || !Nlm_GetAllParentsEnabled( b ))
    return;

  Nlm_GetBarData ((Nlm_BaR) b, &bdata);
  tempPort = Nlm_SavePortIfNeeded(b, savePort);

#ifdef WIN_MSWIN
  EnableWindow(bdata.handle, TRUE);
#endif

  invalPos  ((Nlm_BaR)b, savePort);
  invalRange((Nlm_BaR)b, savePort);

#ifdef WIN_MAC
  HiliteControl(bdata.handle, 0);
#endif
#ifdef WIN_MOTIF
  XtVaSetValues(bdata.handle, XmNsensitive, TRUE, NULL);
#endif

  Nlm_RestorePort (tempPort);
}


static void Nlm_DisableBar (Nlm_GraphiC b, Nlm_Boolean setFlag, Nlm_Boolean savePort)
{
  Nlm_BarTool  c        = Nlm_GetBarHandle( (Nlm_BaR)b );
  Nlm_WindoW   tempPort = Nlm_SavePortIfNeeded(b, savePort);

  if ( setFlag )
    Nlm_SetEnabled(b, FALSE);

#ifdef WIN_MAC
  HiliteControl(c, 255);
#endif
#ifdef WIN_MSWIN
  EnableWindow(c, FALSE);
#endif
#ifdef WIN_MOTIF
  XtVaSetValues(c, XmNsensitive, FALSE, NULL);
#endif

  Nlm_RestorePort( tempPort );
}


static void Nlm_ResetBar (Nlm_GraphiC b, Nlm_Boolean savePort)
{
  setMax((Nlm_BaR)b, 0, savePort, FALSE);
}


static void Nlm_RemoveBar (Nlm_GraphiC b, Nlm_Boolean savePort)
{
  Nlm_BarTool      c        = Nlm_GetBarHandle( (Nlm_BaR)b );
  Nlm_WindoW       tempPort = Nlm_SavePortIfNeeded(b, savePort);
  Nlm_GraphicData  gdata;

#ifdef WIN_MAC
  DisposeControl( c );
#endif
#ifdef WIN_MSWIN
  RemoveProp(c, (LPSTR)"Nlm_VibrantProp");
  DestroyWindow( c );
#endif
#ifdef WIN_MOTIF
  XtDestroyWidget( c );
#endif

  Nlm_GetGraphicData(b, &gdata);
  if (gdata.data != NULL  &&  gdata.cleanup != NULL)
    gdata.cleanup(b, gdata.data);

  Nlm_HandFree( b );
  recentBar = NULL;
  Nlm_RestorePort( tempPort );
}


static void Nlm_RemoveLinkedInBar (Nlm_GraphiC b, Nlm_Boolean savePort)
{
  Nlm_BarTool  c        = Nlm_GetBarHandle( (Nlm_BaR)b );
  Nlm_WindoW   tempPort = Nlm_SavePortIfNeeded(b, savePort);

#ifdef WIN_MAC
  DisposeControl( c );
#endif
#ifdef WIN_MSWIN
  RemoveProp(c, (LPSTR)"Nlm_VibrantProp");
  DestroyWindow( c );
#endif
#ifdef WIN_MOTIF
  XtDestroyWidget( c );
#endif

  Nlm_RemoveLink( b );
  recentBar = NULL;
  Nlm_RestorePort( tempPort );
}


static void Nlm_SetBarValue2(Nlm_GraphiC b, Nlm_Int2 val, Nlm_Boolean savePort)
{
  setPos((Nlm_BaR)b, (Nlm_Int4)val, savePort, TRUE);
}


static Nlm_Int2 Nlm_GetBarValue2 (Nlm_GraphiC b)
{
  Nlm_Int4 pos = getPos( (Nlm_BaR)b );
  if (pos > INT2_MAX)
    pos = INT2_MAX;

  return (Nlm_Int2)pos;
}


#ifdef WIN_MAC
static void Nlm_InvalBar (Nlm_GraphiC b)
{
  Nlm_RecT  r;
  if (!Nlm_GetVisible( b )  ||  !Nlm_GetAllParentsVisible( b ))
    return;

  Nlm_GetRect(b, &r);
  Nlm_InsetRect(&r, -1, -1);
  Nlm_InvalRect( &r );
}
#endif


static void Nlm_SetBarPosition (Nlm_GraphiC b, Nlm_RectPtr r,
				Nlm_Boolean savePort, Nlm_Boolean force)
{
  Nlm_BarTool      c;
  Nlm_GraphicData  gdata;
  Nlm_WindoW       tempPort;

  if (r == NULL)
    return;

  tempPort = Nlm_SavePortIfNeeded(b, savePort);
  if (force)
    {
      Nlm_GetGraphicData(b, &gdata);
      if ( !gdata.realized )
        {
          gdata.realized = TRUE;
          Nlm_SetGraphicData(b, &gdata);
        }
      invalRange((Nlm_BaR)b, FALSE);
      invalPos  ((Nlm_BaR)b, FALSE);
    }

  if ( !Nlm_GetRealized( b ) )
    {
      Nlm_SetRect (b, r);
      Nlm_RestorePort( tempPort );
      return;
    }

  c = Nlm_GetBarHandle( (Nlm_BaR)b );

#ifdef WIN_MAC
  Nlm_InvalBar( b );
  MoveControl(c, r->left, r->top);
  SizeControl(c, r->right - r->left, r->bottom - r->top);
  Nlm_SetRect(b, r);
  Nlm_InvalBar( b );
#endif
#ifdef WIN_MSWIN
  Nlm_SetRect(b, r);
  MoveWindow(c, r->left, r->top, r->right - r->left, r->bottom - r->top, TRUE);
  UpdateWindow( c );
#endif
#ifdef WIN_MOTIF
  XtVaSetValues (c,
		 XmNx,      (Position)  r->left,
		 XmNy,      (Position)  r->top,
		 XmNwidth,  (Dimension) (r->right - r->left),
		 XmNheight, (Dimension) (r->bottom - r->top), 
		 NULL);
  Nlm_SetRect(b, r);
#endif

  Nlm_RestorePort( tempPort );
}


static void Nlm_GetBarPosition (Nlm_GraphiC b, Nlm_RectPtr r)
{
  if (r != NULL)
    Nlm_GetRect(b, r);
}



static void Nlm_SetBarRange2(Nlm_GraphiC b, Nlm_Int2 pgUp,
                             Nlm_Int2 pgDn, Nlm_Int2 max,
                             Nlm_Boolean savePort)
{
  Nlm_CorrectBarPage((Nlm_BaR)b, (Nlm_Int4)pgUp, (Nlm_Int4)pgDn);
  if (max > 0) {
    setMax((Nlm_BaR)b, max, savePort, FALSE);
  }
}


#ifdef WIN_MSWIN
/* Message cracker functions */
static void MyCls_OnChar (HWND hwnd, UINT ch, int cRepeat)

{
  Nlm_BaR  b = (Nlm_BaR)GetProp(hwnd, (LPSTR)"Nlm_VibrantProp");

  if (ch == '\t'  ||  ch == '\n'  ||  ch == '\r')
    {
      handlechar = FALSE;
      Nlm_DoSendFocus ((Nlm_GraphiC) b, (Nlm_Char) ch);
    }
  else
    handlechar = TRUE;
}


LRESULT CALLBACK EXPORT BarProc (HWND hwnd, UINT message,
                                 WPARAM wParam, LPARAM lParam)
{
  Nlm_BaR  b;
  LRESULT  rsult = 0;
  BOOL     call_win_proc = TRUE;
  HDC      tempHDC;
  HWND     tempHWnd;

  if ( Nlm_VibrantDisabled() )
    return CallWindowProc(lpfnOldBarProc, hwnd, message, wParam, lParam);

  tempHWnd = Nlm_currentHWnd;
  tempHDC  = Nlm_currentHDC;

  b = (Nlm_BaR)GetProp(hwnd, (LPSTR)"Nlm_VibrantProp");

  Nlm_theWindow         = Nlm_GetParentWindow( (Nlm_GraphiC)b );
  Nlm_currentHWnd       = GetParent( hwnd );
  Nlm_currentHDC        = Nlm_ParentWindowPort( (Nlm_GraphiC)b );
  Nlm_currentWindowTool = hwnd;

  Nlm_currentWParam = wParam;
  Nlm_currentLParam = lParam;

  Nlm_currentKey    = '\0';
  Nlm_cmmdKey  = FALSE;
  Nlm_ctrlKey  = FALSE;
  Nlm_optKey   = FALSE;
  Nlm_shftKey  = FALSE;
  Nlm_dblClick = FALSE;

  switch ( message )
    {
    case WM_KEYDOWN:
      call_win_proc = !Nlm_ProcessKeydown((Nlm_GraphiC)b, wParam,
        VERT_PAGE|VERT_ARROW|HORIZ_PAGE|HORIZ_ARROW);
      break;
    case WM_CHAR:
      HANDLE_WM_CHAR(hwnd, wParam, lParam, MyCls_OnChar);
      call_win_proc = handlechar;
      break;
  }

  if ( call_win_proc )
    rsult = CallWindowProc(lpfnOldBarProc, hwnd, message, wParam, lParam);

  Nlm_currentHWnd       = tempHWnd;
  Nlm_currentHDC        = tempHDC;
  Nlm_currentWindowTool = tempHWnd;
  return rsult;
}
#endif


#ifdef WIN_MOTIF
static void Nlm_LineUpCallback (Widget w,
                                XtPointer client_data, XtPointer call_data)
{
  Nlm_Scroll((Nlm_BaR)client_data, SCROLL_LINEUP);
}

static void Nlm_LineDnCallback (Widget w,
                                XtPointer client_data, XtPointer call_data)
{
  Nlm_Scroll((Nlm_BaR)client_data, SCROLL_LINEDN);
}

static void Nlm_PageUpCallback (Widget w,
                                XtPointer client_data, XtPointer call_data)
{
  Nlm_Scroll((Nlm_BaR)client_data, SCROLL_PAGEUP);
}

static void Nlm_PageDnCallback (Widget w,
                                XtPointer client_data, XtPointer call_data)
{
  Nlm_Scroll((Nlm_BaR)client_data, SCROLL_PAGEDN);
}

static void Nlm_ThumbCallback (Widget w,
                               XtPointer client_data, XtPointer call_data)
{
  XmScrollBarCallbackStruct  *cbs  = (XmScrollBarCallbackStruct *)call_data;
  setValue((Nlm_BaR)client_data, (Nlm_Int2)cbs->value, TRUE);
}

static void Nlm_TopCallback (Widget w,
                             XtPointer client_data, XtPointer call_data)
{
  setValue((Nlm_BaR)client_data, 0, TRUE);
}

static void Nlm_BottomCallback (Widget w,
                                XtPointer client_data, XtPointer call_data)
{
  setValue((Nlm_BaR)client_data, (Nlm_Int2)INT2_MAX, TRUE);
}
#endif


static void Nlm_NewBarX (Nlm_BaR b, Nlm_GraphiC slv,
                         Nlm_Boolean vert,
                         Nlm_BarScrlProc4 actn4, Nlm_BarScrlProc actn)
{
  Nlm_BarTool     c;
  Nlm_RecT        r;
  Nlm_WindowTool  wptr = Nlm_ParentWindowPtr( (Nlm_GraphiC)b );
#ifdef WIN_MAC
  Nlm_Int4        ref;
  Nlm_RectTool    rtool;
#endif
#ifdef WIN_MSWIN
  Nlm_Int4        style = vert ? SBS_VERT : SBS_HORZ;
#endif
#ifdef WIN_MOTIF
  Cardinal        n;
  Arg             wargs [20];
#endif

  Nlm_GetRect((Nlm_GraphiC)b, &r);

#ifdef WIN_MAC
  Nlm_RecTToRectTool(&r, &rtool);
  ref = (Nlm_Int4)b;
  c = NewControl (wptr, &rtool, (StringPtr) "", FALSE, 0, 0, 0, 16, ref);
  Nlm_LoadBarData(b, c, 10, 10, 0, 0, 0, FALSE, slv, actn4, actn);
#endif

#ifdef WIN_MSWIN
  c = CreateWindow("Scrollbar", "", WS_CHILD | style,
                   r.left, r.top, r.right - r.left, r.bottom - r.top,
                   wptr, 0, Nlm_currentHInst, NULL);
  if (c != NULL)
    SetProp(c, (LPSTR)"Nlm_VibrantProp", (Nlm_HandleTool)b);

  Nlm_LoadBarData(b, c, 10, 10, 0, 0, 0, FALSE, slv, actn4, actn);
  if (lpfnNewBarProc == NULL)
    lpfnNewBarProc = (WNDPROC)MakeProcInstance((FARPROC) BarProc,
                                               Nlm_currentHInst);

  if (lpfnOldBarProc == NULL)
    lpfnOldBarProc = (WNDPROC)GetWindowLong(c, GWL_WNDPROC);
  else if (lpfnOldBarProc != (WNDPROC)GetWindowLong(c, GWL_WNDPROC))
    Nlm_Message(MSG_ERROR, "BarProc subclass error");

  SetWindowLong(c, GWL_WNDPROC, (LONG)lpfnNewBarProc);
#endif

#ifdef WIN_MOTIF
  n = 0;
  XtSetArg(wargs [n], XmNx,      (Position)  r.left);  n++;
  XtSetArg(wargs [n], XmNy,      (Position)  r.top );  n++;
  XtSetArg(wargs [n], XmNwidth,  (Dimension) (r.right - r.left));  n++;
  XtSetArg(wargs [n], XmNheight, (Dimension) (r.bottom - r.top));  n++;
  XtSetArg(wargs [n], XmNborderWidth, (Dimension)0);  n++;
  XtSetArg(wargs [n], XmNmarginHeight,  0);  n++;
  XtSetArg(wargs [n], XmNmarginWidth,   0);  n++;
  XtSetArg(wargs [n], XmNsliderSize,    1);  n++;

  if (vert) {
    XtSetArg(wargs [n], XmNorientation,         XmVERTICAL     );  n++;
    XtSetArg(wargs [n], XmNprocessingDirection, XmMAX_ON_BOTTOM);  n++;
  } else {
    XtSetArg(wargs [n], XmNorientation,         XmHORIZONTAL   );  n++;
    XtSetArg(wargs [n], XmNprocessingDirection, XmMAX_ON_RIGHT );  n++;
  }

  c = XmCreateScrollBar(wptr, (String) "", wargs, n);

  {{
  XtPointer b_ptr = (XtPointer)b;
  XtAddCallback(c, XmNincrementCallback,     Nlm_LineDnCallback, b_ptr);
  XtAddCallback(c, XmNdecrementCallback,     Nlm_LineUpCallback, b_ptr);
  XtAddCallback(c, XmNpageIncrementCallback, Nlm_PageDnCallback, b_ptr);
  XtAddCallback(c, XmNpageDecrementCallback, Nlm_PageUpCallback, b_ptr);
  XtAddCallback(c, XmNvalueChangedCallback,  Nlm_ThumbCallback,  b_ptr);
  XtAddCallback(c, XmNtoTopCallback,         Nlm_BottomCallback, b_ptr);
  XtAddCallback(c, XmNtoBottomCallback,      Nlm_TopCallback,    b_ptr);
  }}

  XtManageChild( c );

  Nlm_LoadBarData(b, c, 10, 10, 0, 0, 0, FALSE, slv, actn4, actn);
#endif
}


static Nlm_BaR Nlm_CommonScrollBarX (Nlm_GraphiC slave, Nlm_RectPtr r,
                                     Nlm_BarScrlProc4 actn4,
                                     Nlm_BarScrlProc  actn,
                                     Nlm_Boolean vert)
{
  Nlm_BaR     b;
  Nlm_WindoW  tempPort;

  if (slave == NULL  ||  r == NULL)
    return NULL;

  b = (Nlm_BaR)Nlm_HandNew( sizeof(Nlm_BarRec) );
  if (b == NULL)
    return NULL;

  tempPort = Nlm_SavePort( slave );
  Nlm_LoadGraphicData((Nlm_GraphiC)b, NULL, (Nlm_GraphiC)slave,
                      NULL, NULL, barProcs, NULL, r, TRUE, FALSE, NULL, NULL);
  if (Nlm_nextIdNumber < 32767)
    Nlm_nextIdNumber++;
  Nlm_NewBarX(b, slave, vert, actn4, actn);

  Nlm_RestorePort( tempPort );
  return b;
}


static Nlm_BaR Nlm_ScrollBarX (Nlm_GrouP prnt,
                               Nlm_Int2 width, Nlm_Int2 height,
                               Nlm_BarScrlProc4 actn4, Nlm_BarScrlProc actn)
{
  Nlm_BaR      b;
  Nlm_PoinT    npt;
  Nlm_RecT     r;
  Nlm_WindoW   tempPort;
  Nlm_Boolean  vert;

  if (prnt == NULL)
    return NULL;

  tempPort = Nlm_SavePort( (Nlm_GraphiC)prnt );
  Nlm_GetNextPosition((Nlm_GraphiC)prnt, &npt);
  if (width > 0) 
    {
      vert = FALSE;
      width *= Nlm_stdCharWidth;
      Nlm_LoadRect(&r, npt.x, npt.y,
                   (Nlm_Int2)(npt.x + width),
                   (Nlm_Int2)(npt.y + Nlm_hScrollBarHeight));
    }
  else
    {
      vert = TRUE;
      height *= Nlm_stdLineHeight;
      Nlm_LoadRect(&r, npt.x, npt.y,
                   (Nlm_Int2)(npt.x + Nlm_vScrollBarWidth),
                   (Nlm_Int2)(npt.y + height));
    }
  b = (Nlm_BaR)Nlm_CreateLink((Nlm_GraphiC)prnt,
                              &r, sizeof(Nlm_BarRec), linkedBarProcs);
  if (b == NULL)
    return NULL;

  Nlm_NewBarX(b, (Nlm_GraphiC)prnt, vert, actn4, actn);
  Nlm_DoAdjustPrnt((Nlm_GraphiC)b, &r, TRUE, FALSE);
  Nlm_DoShow((Nlm_GraphiC)b, TRUE, FALSE);
  Nlm_RestorePort( tempPort );

  return b;
}



/*********************************************************************
 *  PUBLIC
 *********************************************************************/

/*
 *  Creation
 */

extern Nlm_BaR Nlm_HorizScrollBar4 (Nlm_GraphiC slave,
                                    Nlm_RectPtr r, Nlm_BarScrlProc4 actn)
{
  return Nlm_CommonScrollBarX(slave, r, actn, NULL, FALSE);
}

extern Nlm_BaR Nlm_HorizScrollBar  (Nlm_GraphiC slave,
                                    Nlm_RectPtr r, Nlm_BarScrlProc actn)
{
  return Nlm_CommonScrollBarX(slave, r, NULL, actn, FALSE);
}


extern Nlm_BaR Nlm_VertScrollBar4 (Nlm_GraphiC slave,
                                   Nlm_RectPtr r, Nlm_BarScrlProc4 actn)
{
  return Nlm_CommonScrollBarX(slave, r, actn, NULL, TRUE);
}

extern Nlm_BaR Nlm_VertScrollBar  (Nlm_GraphiC slave,
                                   Nlm_RectPtr r, Nlm_BarScrlProc actn)
{
  return Nlm_CommonScrollBarX(slave, r, NULL, actn, TRUE);
}


extern Nlm_BaR Nlm_ScrollBar4 (Nlm_GrouP prnt, Nlm_Int2 width,
                               Nlm_Int2 height, Nlm_BarScrlProc4 actn)
{
  return Nlm_ScrollBarX(prnt, width, height, actn, NULL);
}

extern Nlm_BaR Nlm_ScrollBar  (Nlm_GrouP prnt, Nlm_Int2 width,
                               Nlm_Int2 height, Nlm_BarScrlProc  actn)
{
  return Nlm_ScrollBarX(prnt, width, height, NULL, actn);
}


/*
 *  Methods
 */

extern void Nlm_SetBarValue(Nlm_BaR b, Nlm_Int4 val)
{
  setPos(b, val, TRUE, TRUE);
}

extern void Nlm_CorrectBarValue (Nlm_BaR b, Nlm_Int4 val)
{
  setPos(b, val, TRUE, FALSE);
}

extern Nlm_Int4 Nlm_GetBarValue (Nlm_BaR b)
{
  return getPos( b );
}


extern void Nlm_SetBarMax (Nlm_BaR b, Nlm_Int4 max)
{
  setMax(b, max, TRUE, TRUE);
}

extern void Nlm_CorrectBarMax (Nlm_BaR b, Nlm_Int4 max)
{
  setMax(b, max, TRUE, FALSE);
}

extern Nlm_Int4 Nlm_GetBarMax (Nlm_BaR b)
{
  Nlm_BarData  bdata;

  Nlm_GetBarData (b, &bdata);
  return bdata.max;
}


extern void Nlm_CorrectBarPage(Nlm_BaR b, Nlm_Int4 pgUp, Nlm_Int4 pgDn)
{
  Nlm_BarData  bdata;

  if (b == NULL)
    return;

  Nlm_GetBarData(b, &bdata);
  if ((bdata.pageUp == pgUp  &&  bdata.pageDn == pgDn)  ||
      (pgUp <= 0  &&  pgDn <= 0))
    return;

  if (pgUp > 0)
    bdata.pageUp = pgUp;
  if (pgDn > 0)
    bdata.pageDn = pgDn;
#ifdef WIN_MOTIF
  bdata.delta_max = INT4_MAX;
#endif
  Nlm_SetBarData(b, &bdata);
#ifdef WIN_MOTIF
  invalRange(b, TRUE);
#endif
}


extern void Nlm_Scroll(Nlm_BaR b, Nlm_enumScrollEvent event)
{
  switch ( event )
    {
    case SCROLL_LINEUP:
      setPos(b, getPos(b) - 1, TRUE, TRUE);
      break;
    case SCROLL_LINEDN:
      setPos(b, getPos(b) + 1, TRUE, TRUE);
      break;
    case SCROLL_PAGEUP:
      setPos(b, getPos(b) - Nlm_GetBarPageUp(b),   TRUE, TRUE);
      break;
    case SCROLL_PAGEDN:
      setPos(b, getPos(b) + Nlm_GetBarPageDown(b), TRUE, TRUE);
      break;
    default:
      AbnormalExit( 1 );
    }
}



/*********************************************************************
 *  PROTECTED
 *********************************************************************/

extern void Nlm_SetBarAnomaly (Nlm_BaR b, Nlm_Boolean anomaly)

{
  Nlm_BarData  bdata;

  if (b == NULL)
    return;

  Nlm_GetBarData(b, &bdata);
  bdata.anomaly = anomaly;
  Nlm_SetBarData(b, &bdata);
}


extern void Nlm_FreeBars (void)
{
  gphprcsptr = (Nlm_GphPrcsPtr)Nlm_MemFree( gphprcsptr );
}


extern void Nlm_InitBars (void)
{
  barProcs = gphprcsptr = (Nlm_GphPrcsPtr)Nlm_MemNew(2 * sizeof(Nlm_GphPrcs));
  if (barProcs == NULL)
    return;

#ifdef WIN_MAC
  barProcs->click       = Nlm_ScrollClick;
  barProcs->draw        = Nlm_DrawBar;
#endif
#ifdef WIN_MSWIN
  barProcs->command     = Nlm_ScrollCommand;
#endif
  barProcs->show        = Nlm_ShowBar;
  barProcs->hide        = Nlm_HideBar;
  barProcs->enable      = Nlm_EnableBar;
  barProcs->disable     = Nlm_DisableBar;
  barProcs->remove      = Nlm_RemoveBar;
  barProcs->reset       = Nlm_ResetBar;
  barProcs->setValue    = Nlm_SetBarValue2;
  barProcs->getValue    = Nlm_GetBarValue2;
  barProcs->setPosition = Nlm_SetBarPosition;
  barProcs->getPosition = Nlm_GetBarPosition;
  barProcs->setRange    = Nlm_SetBarRange2;

  linkedBarProcs = gphprcsptr + 1;
#ifdef WIN_MAC
  linkedBarProcs->click       = Nlm_ScrollClick;
  linkedBarProcs->draw        = Nlm_DrawBar;
#endif
#ifdef WIN_MSWIN
  linkedBarProcs->command     = Nlm_ScrollCommand;
#endif
#ifdef WIN_MOTIF
#endif
  linkedBarProcs->show        = Nlm_ShowBar;
  linkedBarProcs->hide        = Nlm_HideBar;
  linkedBarProcs->enable      = Nlm_EnableBar;
  linkedBarProcs->disable     = Nlm_DisableBar;
  linkedBarProcs->remove      = Nlm_RemoveLinkedInBar;
  linkedBarProcs->reset       = Nlm_ResetBar;
  linkedBarProcs->setValue    = Nlm_SetBarValue2;
  linkedBarProcs->getValue    = Nlm_GetBarValue2;
  linkedBarProcs->setPosition = Nlm_SetBarPosition;
  linkedBarProcs->getPosition = Nlm_GetBarPosition;
  linkedBarProcs->setRange    = Nlm_SetBarRange2;
}


/*********************************************************************
 *  TEST-ONLY
 *********************************************************************/

#ifdef TEST_MODULE_VIBSBARS
#ifdef OS_UNIX
#include <stdio.h>
#endif

static void scroll4CB(Nlm_BaR b, Nlm_GraphiC slave,
                      Nlm_Int4 prev_pos, Nlm_Int4 curr_pos);
static void scroll2CB(Nlm_BaR b, Nlm_GraphiC slave,
                      Nlm_Int2 prev_pos, Nlm_Int2 curr_pos);
static void QuitCB(Nlm_WindoW w);


extern Nlm_Int2 Nlm_Main( void )
{
  Nlm_WindoW www =
    Nlm_FixedWindow(-30, -50, -10, -10, "TEST_MODULE_VIBSBARS", QuitCB);
  
  {{
  Nlm_BaR bar4 = Nlm_ScrollBar4(www, 0, 20, scroll4CB);
  Nlm_CorrectBarMax(bar4, 100000);
  }}

  {{
  Nlm_BaR bar2 = Nlm_ScrollBar (www, 0, 20, scroll2CB);
  Nlm_CorrectBarMax(bar2, 1000);
  }}

  Nlm_Show( www );
  Nlm_ProcessEvents();
  return 0;
}


static void scroll4CB(Nlm_BaR b, Nlm_GraphiC slave,
                      Nlm_Int4 curr_pos, Nlm_Int4 prev_pos)
{
  Nlm_Int4 pos = Nlm_GetBarValue( b );
  Nlm_Int4 max = Nlm_GetBarMax  ( b );

#ifdef OS_UNIX
  fprintf(stderr, "Scroll4CB():  prev=%ld curr=%ld  val=%ld  max=%ld\n",
          (long)prev_pos, (long)curr_pos,
          (long)pos, (long)max);
#endif
return;

  if (prev_pos + 5 < curr_pos)
    Nlm_CorrectBarMax(b, max - 1000);
  else
    Nlm_CorrectBarMax(b, max + 1000);
}


static void scroll2CB(Nlm_BaR b, Nlm_GraphiC slave,
                      Nlm_Int2 curr_pos, Nlm_Int2 prev_pos)
{
  Nlm_Int4 pos = Nlm_GetBarValue( b );
  Nlm_Int4 max = Nlm_GetBarMax  ( b );

  static Nlm_Int4 div = 1;
  Nlm_Int4 page;

  if (curr_pos > prev_pos)
    {
      page = max / div;
      if (div < max)
        div *= 2;
    }
  else
    {
      if (div > 1)
        div /= 2;
      page = max / div;
    }
#ifdef OS_UNIX
  fprintf(stderr,
          "Scroll2CB():  prev=%ld curr=%ld  val=%ld  max=%ld  page=%ld\n",
          (long)prev_pos, (long)curr_pos,
          (long)pos, (long)max, (long)page);
#endif
  Nlm_CorrectBarPage(b, page, page);
}


static void QuitCB(Nlm_WindoW w)
{
  Nlm_QuitProgram();
}

#endif /* TEST_MODULE_VIBSBARS */
