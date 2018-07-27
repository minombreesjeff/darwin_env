/*   viewer.c
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
* File Name:  viewer.c
*
* Author:  Jonathan Kans, Alex Smirnov, Jill Shermer
*
* Version Creation Date:   10/25/92
*
* $Revision: 6.10 $
*
* File Description:
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: viewer.c,v $
* Revision 6.10  2001/03/04 21:50:59  kans
* CorrectScrollBar numLines, visLines calculation changed slightly
*
* Revision 6.9  1999/08/06 19:28:17  vakatov
* Restored "NormalizeBox()" as "s_NormalizeBoxUpsideDown()" -- because
* this particular moduleby some reason assumes that:  top >= bottom
*
* Revision 6.8  1999/08/06 18:42:36  vakatov
* Moved "NormalizeBox()" from "viewer.c" to "mappingp.[ch]" & made it public
*
* Revision 6.7  1999/03/02 19:09:50  vakatov
* PrintAllViewer() -- fixed typos introduced in R6.6
*
* Revision 6.6  1998/06/12 20:10:36  kans
* fixed unix compiler warnings
*
* Revision 6.5  1997/12/18 16:49:38  vakatov
* cosmetics in DrawViewerProc()
*
* Revision 6.4  1997/12/08 14:53:25  kans
* PictureHasEnlarged does not invalidate
*
* Revision 6.3  1997/11/26 21:30:53  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.2  1997/11/18 00:41:05  kans
* PictureGrew is private extern
*
* Revision 6.1  1997/11/17 23:56:29  kans
* added PictureHasEnlarged, call on scroll if PictureGrew dynamically
*
* Revision 5.8  1997/08/21 18:50:10  vakatov
* PrintAllViewer() -- print starts from the left-top corner(was left-bottom)
*
* Revision 5.7  1997/07/24 20:33:46  vakatov
* [WIN_MSWIN]  CopyViewer() -- fix in the positioning of clipboard viewport
*
* Revision 5.6  1997/01/16 22:02:36  vakatov
* Added CopyAllViewer() -- to copy the whole picture to the clipboard
*
 * Revision 5.5  1996/12/13  21:40:35  vakatov
 * Rearranged to use Int4-ranged scrollbars, so that the viewer can
 * manage(scroll) very big pictures
 *
 * Revision 5.4  1996/12/13  17:13:14  vakatov
 * PrintViewer() functionality reverted to the "one-page printing";
 * the multipage printing is to be performed by PrintAllViewer().
 *
 * Revision 5.3  1996/12/12  23:11:10  kans
 * changes to allow multi-page printing of viewer pictures (DV)
 *
 * Revision 5.2  1996/09/09  20:13:47  vakatov
 * Added auxiliary function AlignPortToWorld_1D();
 * removed BoxInBox() -- replaced by (new) function AlignPortToWorld().
 * -- all that was to properly syncronize World/Port/Viewer coordinates
 * in CalculateScaling() and other functions.
 * Added auxiliary function CorrectScrollbar() responsible for setting
 * the viewer scrollbar range and position depending of the given world & port
 * box locations.
 * Modified AttachCommon() and ViewerWasResized() using(mentioned above) stuff.
 *
 * Revision 5.1  1996/09/03  18:32:22  vakatov
 * ViewerWasResized() function has been modified in order to provide proper
 * picture viewport and scrollbars positioning on the viewer resizing
 *
 * Revision 4.3  1996/01/31  18:44:50  smirnov
 * AS: Fix copy&paste bug
 *
 * Revision 4.2  1995/12/14  17:29:30  kans
 * moved SearchSegment from viewer.c to picture.c, made extern
 *
 * Revision 4.1  1995/11/07  23:21:53  kans
 * moved Nlm_DrawSegment from viewer.c to drawing.c (for GIF without Vibrant)
 *
 * Revision 1.58  1995/05/17  16:37:04  kans
 * fix uninitialized variable bug, world expands to encompass port (AS)
 *
* ==========================================================================
*/

#ifndef _VIBRANT_
#include <vibrant.h>
#endif

#ifndef _PICTURE_
#include <picture.h>
#endif

#ifndef _PICTUREP_
#include <picturep.h>
#endif

#ifndef _MAPPINGP_
#include <mappingp.h>
#endif

#ifndef _DRAWINGP_
#include <drawingp.h>
#endif

#ifndef _VIEWER_
#include <viewer.h>
#endif

#ifndef _VIEWERP_
#include <viewerp.h>
#endif

/*****************************************************************************
*
*   INTERNAL TYPE DEFINES
*
*****************************************************************************/

#define PROXIMITY    2


/*****************************************************************************
*
*   ViewerVScrollProc (sb, viewer, newval, oldval)
*
*****************************************************************************/

static void ViewerVScrollProc (BaR sb, SlatE viewer, Int4 newval, Int4 oldval)
{
  ViewPData  extra;
  Int4       height;
  RecT       r;
  Int4       visLines;

  if (viewer == NULL  ||  oldval == newval  ||
      !Visible( viewer )  ||  !AllParentsVisible( viewer ))
    return;

  GetPanelExtra((PaneL)viewer, &extra);
  ObjectRect(viewer, &r);
  InsetRect(&r, 4, 4);
  height = r.bottom - r.top;
  visLines = height / extra.scale.scrollY;

  Select( viewer );
  if ((newval > oldval  &&  newval - visLines + 1 <= oldval) ||
      (newval < oldval  &&  newval + visLines - 1 >= oldval))
    {
      Int4 pixels = (newval - oldval) * extra.scale.scrollY;
      if (ABS(pixels) < height)
        {
          ScrollRect(&r, 0, (Int2)(-pixels));
          if (pixels > 0)
            r.top    = (Int2)(r.bottom - pixels);
          else
            r.bottom = (Int2)(r.top    - pixels);
        }
    }
  InsetRect(&r, -1, -1);
  InvalRect( &r );

  {{
  Int4 offsetY= (extra.scale.scaleY * extra.scale.scrollY) * (newval - oldval);
  extra.scale.port.top    -= offsetY;
  extra.scale.port.bottom -= offsetY;
  }}
  SetPanelExtra((PaneL)viewer, &extra);
  Update();

  if (PictureGrew ((VieweR) viewer)) {
    PictureHasEnlarged ((VieweR) viewer);
  }

  if (extra.pan != NULL)
    extra.pan((VieweR)viewer, extra.picture);
}


/*****************************************************************************
*
*   ViewerHScrollProc (sb, viewer, newval, oldval)
*
*****************************************************************************/

static void ViewerHScrollProc (BaR sb, SlatE viewer, Int4 newval, Int4 oldval)
{
  ViewPData  extra;
  RecT       r;
  Int4       visLines;
  Int4       width;

  if (viewer == NULL  ||  oldval == newval  ||
      !Visible( viewer )  ||  !AllParentsVisible( viewer ))
    return;

  GetPanelExtra((PaneL)viewer, &extra);
  ObjectRect(viewer, &r);
  InsetRect(&r, 4, 4);
  width = r.right - r.left;
  visLines = width / extra.scale.scrollX;

  Select( viewer );
  if ((newval > oldval  &&  newval - visLines + 1 <= oldval) ||
      (newval < oldval  &&  newval + visLines - 1 >= oldval))
    {
       Int4 pixels = (newval - oldval) * extra.scale.scrollX;
       if (ABS(pixels) < width)
         {
           ScrollRect(&r, (Int2)(-pixels), 0);
           if (pixels > 0)
             r.left  = (Int2)(r.right - pixels);
           else
             r.right = (Int2)(r.left  - pixels);
         }
    }
  InsetRect(&r, -1, -1);
  InvalRect( &r );

  {{
  Int4 offsetX= (extra.scale.scaleX * extra.scale.scrollX) * (newval - oldval);
  extra.scale.port.left  += offsetX;
  extra.scale.port.right += offsetX;
  }}
  SetPanelExtra((PaneL)viewer, &extra);
  Update();

  if (PictureGrew ((VieweR) viewer)) {
    PictureHasEnlarged ((VieweR) viewer);
  }

  if (extra.pan != NULL)
    extra.pan((VieweR)viewer, extra.picture);
}


static void MakeScaleInfo ( Nlm_VScalePtr oldPtr, Nlm_ScalePtr newPtr )
{
  PntInfo worldP;
  PoinT   pixelP;

  newPtr->worldWindow = oldPtr->port;
  newPtr->scaleX = oldPtr->scaleX;
  newPtr->scaleY = oldPtr->scaleY;
  newPtr->offsetX = (Int4)oldPtr->view.left   * oldPtr->scaleX -
                    oldPtr->port.left;
  newPtr->offsetY = (Int4)oldPtr->view.bottom * oldPtr->scaleY +
                    oldPtr->port.bottom;
  pixelP.x = -1000;
  pixelP.y = -1000;
  MapPixelPointToWorld ( &worldP, &pixelP, newPtr );
  newPtr->worldWindow16.left = worldP.x;
  newPtr->worldWindow16.top  = worldP.y;
  pixelP.x = 4000;
  pixelP.y = 4000;
  MapPixelPointToWorld ( &worldP, &pixelP, newPtr );
  newPtr->worldWindow16.right  = worldP.x;
  newPtr->worldWindow16.bottom = worldP.y;
}


/*****************************************************************************
*
*   DrawViewerProc (viewer)
*       Draws the picture in a viewer panel.  Clipping is to the intersection
*       of the updateRgn (the portion of the panel exposed) and the inset area
*       of the viewer, leaving a four-pixel margin.
*
*****************************************************************************/
static void DrawViewerProc(PaneL viewer)
{
  RegioN       dst;
  ViewPData    extra;
  SegPPtr      pic;
  RegioN       src;
  Nlm_DrawInfo dInfo;
  RecT         rupd;
  RecT         rextra;
  RecT         rview;

  if ( !viewer )
    return;

  GetPanelExtra(viewer, &extra);
  pic = (SegPPtr)extra.picture;
  if ( !pic )
    return;

  if (pic->base.code != PICTURE) {
    Message (MSG_ERROR, "DrawViewerProc target is not a picture");
    return;
  }

  rupd   = updateRect;
  rextra = extra.scale.view;
  src = CreateRgn();
  dst = CreateRgn();
  LoadRectRgn(src, extra.scale.view.left,  extra.scale.view.top,
                   extra.scale.view.right, extra.scale.view.bottom);
  SectRgn(src, updateRgn, dst);
  ClipRgn(dst);
  DestroyRgn(src);
  DestroyRgn(dst);
  MakeScaleInfo(&extra.scale, &dInfo.scale);
  rview.left   = (Int2)(MAX(rextra.left,   rupd.left)   - 10);
  rview.top    = (Int2)(MAX(rextra.top,    rupd.top )   - 10);
  rview.right  = (Int2)(MIN(rextra.right,  rupd.right)  + 10);
  rview.bottom = (Int2)(MIN(rextra.bottom, rupd.bottom) + 10);

  MapRectToWorldBox(&dInfo.scale.worldWindow, &rview, &dInfo.scale);
  dInfo.checked = FALSE;
  dInfo.curattrib = blackAttPData;
  dInfo.highlight = 0;
  Nlm_DrawSegment(pic, &dInfo);
  if (extra.draw != NULL) {
    ResetDrawingTools ();
    extra.draw((VieweR)viewer, extra.picture);
  }
  ResetDrawingTools();
  ResetClip();
}


/*****************************************************************************
*
*   ViewerClickProc (viewer, pt)
*
*****************************************************************************/

static void ViewerClickProc (PaneL viewer, PoinT pt)

{
  ViewPData  extra;

  GetPanelExtra (viewer, &extra);
  if (extra.click != NULL && extra.picture != NULL) {
    extra.click ((VieweR) viewer, extra.picture, pt);
  }
}

/*****************************************************************************
*
*   ViewerDragProc (viewer, pt)
*
*****************************************************************************/

static void ViewerDragProc (PaneL viewer, PoinT pt)

{
  ViewPData  extra;

  GetPanelExtra (viewer, &extra);
  if (extra.drag != NULL && extra.picture != NULL) {
    extra.drag ((VieweR) viewer, extra.picture, pt);
  }
}

/*****************************************************************************
*
*   ViewerReleaseProc (viewer, pt)
*
*****************************************************************************/

static void ViewerReleaseProc (PaneL viewer, PoinT pt)

{
  ViewPData  extra;

  GetPanelExtra (viewer, &extra);
  if (extra.release != NULL && extra.picture != NULL) {
    extra.release ((VieweR) viewer, extra.picture, pt);
  }
}

/*****************************************************************************
*
*   ResetViewerProc (viewer)
*       Clears the panel extra data to remove references to the picture,
*       and calls the cleanup procedure to free any attached instance data
*
*****************************************************************************/

static void ResetViewerProc (PaneL viewer)

{
  ViewPData  extra;

  GetPanelExtra (viewer, &extra);
  if (extra.data != NULL && extra.cleanup != NULL) {
    extra.cleanup ((VieweR) viewer, extra.data);
  }
  extra.draw = NULL;
  extra.data = NULL;
  extra.cleanup = NULL;
  MemSet ((VoidPtr) (&extra), 0, sizeof (ViewPData));
  SetPanelExtra (viewer, &extra);
}

/*****************************************************************************
*
*   CreateViewer (prnt, width, height, vscoll, hscroll)
*       Creates a viewer panel
*
*****************************************************************************/

VieweR CreateViewer (GrouP prnt, Int2 width, Int2 height, Boolean vscroll, Boolean hscroll)

{
  ViewPData    extra;
  SltScrlProc4 hscrproc;
  Int2         margin;
  PaneL        viewer;
  SltScrlProc4 vscrproc;

  viewer = NULL;
  if (prnt != NULL) {
    margin = 8;
    if (vscroll) {
      vscrproc = ViewerVScrollProc;
      margin = 0;
    } else {
      vscrproc = NULL;
    }
    if (hscroll) {
      hscrproc = ViewerHScrollProc;
      margin = 0;
    } else {
      hscrproc = NULL;
    }
    viewer = AutonomousPanel4(prnt,
                              (Int2)(width + margin), (Int2)(height + margin),
                              DrawViewerProc, vscrproc, hscrproc,
                              sizeof(ViewPData), ResetViewerProc, NULL);
    if (viewer != NULL) {
      SetPanelClick (viewer, ViewerClickProc, ViewerDragProc, NULL, ViewerReleaseProc);
      MemSet ((VoidPtr) (&extra), 0, sizeof (ViewPData));
      extra.picture = NULL;
      SetPanelExtra (viewer, &extra);
    }
  }
  return (VieweR) viewer;
}

/*****************************************************************************
*
*   ResetViewer (viewer)
*       Resets a viewer panel
*
*****************************************************************************/

void ResetViewer (VieweR viewer)

{
  if (viewer != NULL) {
    Reset (viewer);
  }
}

/*****************************************************************************
*
*   DeleteViewer (viewer)
*       Removes a viewer panel
*
*****************************************************************************/

VieweR DeleteViewer (VieweR viewer)
{
  if ( viewer ) {
    viewer = (VieweR) Remove(viewer);
  }
  return viewer;
}

/*****************************************************************************
*
*   AlignPortToWorld (BoxPtr port, const Box PNTR world)
*       Forces "world" to contain "port" -- provided with:
*         "port"  can be moved   and cannot be resized
*
*****************************************************************************/

static void AlignPortToWorld_1D(Int4    world_min,   Int4    world_max,
                                Int4Ptr port_min,    Int4Ptr port_max,
                                Boolean align_to_min)
{
  ASSERT ( port_min != NULL  &&  port_max != NULL );
  ASSERT ( *port_min <= *port_max );
  ASSERT ( world_min <= world_max );

  if ( align_to_min )
    {
      Int4 offset = *port_max - world_max;
      if (offset > 0)
        {
          *port_min -= offset;
          *port_max -= offset;
        }

      offset = world_min - *port_min;
      if (offset > 0)
        {
          *port_min += offset;
          *port_max += offset;
        }
    }
  else
    {
      Int4 offset = world_min - *port_min;
      if (offset > 0)
        {
          *port_min += offset;
          *port_max += offset;
        }

      offset = *port_max - world_max;
      if (offset > 0)
        {
          *port_min -= offset;
          *port_max -= offset;
        }
    }
}

static void AlignPortToWorld (BoxPtr port, BoxPtr world)
{
  if (port == NULL  ||  world == NULL)
    return;

  AlignPortToWorld_1D(world->left,   world->right,
                      &port->left,   &port->right,  TRUE);
  AlignPortToWorld_1D(world->bottom, world->top,
                      &port->bottom, &port->top,    FALSE);
}


/*****************************************************************************
*
*   s_NormalizeBoxUpsideDown (box)
*       Ensures that left <= right and bottom <= top in world coordinates
*
*   NOTE:  dont confuse it with the "standard" public "NormalizeBox()"!!!
*****************************************************************************/

static void s_NormalizeBoxUpsideDown (BoxPtr box)
{
  Int4  swap;

  if (box != NULL) {
    if (box->left > box->right) {
      swap = box->left;
      box->left = box->right;
      box->right = swap;
    }
    if (box->bottom > box->top) {
      swap = box->bottom;
      box->bottom = box->top;
      box->top = swap;
    }
  }
}


/*****************************************************************************
*
*   CalculateScaling (viewer, picture, pntX, pntY, align,
*                     scaleX, scaleY, scalePtr, linHgtPtr,
*                     linWidPtr, scrolXPtr, scrolYPtr)
*       Performs scaling calculations in attaching a picture to a viewer
*
*****************************************************************************/

static void CalculateScaling (VieweR viewer, SegmenT picture,
                              Int4 pntX, Int4 pntY,
                              Int2 align, Int4 scaleX, Int4 scaleY,
                              VScalePtr scalePtr,
                              Int4Ptr linHgtPtr, Int4Ptr linWidPtr)
{
  PicPPtr    pic     = (PicPPtr) picture;
  Int2       scrollX = 10;
  Int2       scrollY = 10;
  BoxInfo    world;
  BoxInfo    port;
  RecT       view;
  Int4       width;
  Int4       height;
  Int4       lineWidth;
  Int4       lineHeight;

  if (viewer == NULL  ||  picture == NULL  ||
      pic->base.code != PICTURE  ||  scalePtr == NULL)
    return;

  SelectFont( systemFont );

  if (scaleX < 0)
    scaleX = -scaleX;
  if (scaleY < 0)
    scaleY = -scaleY;
  world.left   = pic->seg.box.left   - 10*scaleX;
  world.top    = pic->seg.box.top    + 10*scaleY + 1;
  world.right  = pic->seg.box.right  + 10*scaleX + 1;
  world.bottom = pic->seg.box.bottom - 10*scaleY;
  s_NormalizeBoxUpsideDown( &world );

  ObjectRect(viewer, &view);
  InsetRect(&view, 4, 4);

  if (scaleX == 0)
    {
      scaleX = (world.right - world.left + 1 + view.right - view.left) /
        (Int4)(view.right - view.left + 1);
      if (scaleX == 0)
        scaleX = 1;
    }
  if (scaleY == 0)
    {
      scaleY = (world.top - world.bottom + 1 + view.bottom - view.top) /
        (Int4)(view.bottom - view.top + 1);
      if (scaleY == 0)
        scaleY = 1;
    }

  lineWidth  = scaleX * scrollX;
  lineHeight = scaleY * scrollY;
  width  = (view.right - view.left + 1) * scaleX;
  height = (view.bottom - view.top + 1) * scaleY;

  if (pntX == INT4_MIN)
    pntX = world.left;
  else if (pntX == INT4_MAX)
    pntX = world.right;
  if (pntY == INT4_MIN)
    pntY = world.bottom;
  else if (pntY == INT4_MAX)
    pntY = world.top;

  switch ( align )
    {
    case MIDDLE_CENTER :
      port.left   = pntX - (width  / 2);
      port.top    = pntY + (height / 2);
      port.right  = port.left + width  - 1;
      port.bottom = port.top  - height + 1;
      break;
    case UPPER_LEFT :
      port.left   = pntX;
      port.top    = pntY;
      port.right  = pntX + width  - 1;
      port.bottom = pntY - height + 1;
      break;
    case LOWER_LEFT :
      port.left   = pntX;
      port.bottom = pntY;
      port.right  = pntX + width  - 1;
      port.top    = pntY + height - 1;
      break;
    case UPPER_RIGHT :
      port.right  = pntX;
      port.top    = pntY;
      port.left   = pntX - width  + 1;
      port.bottom = pntY - height + 1;
      break;
    case LOWER_RIGHT :
      port.right  = pntX;
      port.bottom = pntY;
      port.left   = pntX - width  + 1;
      port.top    = pntY + height - 1;
      break;
    case UPPER_CENTER :
      port.left   = pntX      - (width / 2);
      port.top    = pntY;
      port.right  = port.left + width  - 1;
      port.bottom = pntY      - height + 1;
      break;
    case LOWER_CENTER :
      port.left   = pntX      - (width / 2);
      port.bottom = pntY;
      port.right  = port.left + width  - 1;
      port.top    = pntY      + height - 1;
      break;
    case MIDDLE_LEFT :
      port.left   = pntX;
      port.top    = pntY     + (height / 2);
      port.right  = pntX     + width  - 1;
      port.bottom = port.top - height + 1;
      break;
    case MIDDLE_RIGHT :
      port.right  = pntX;
      port.top    = pntY     + (height / 2);
      port.left   = pntX     - width  + 1;
      port.bottom = port.top - height + 1;
      break;
    default :
      Message (MSG_ERROR, "AttachPicture align parameter is invalid");
      break;
    }
  s_NormalizeBoxUpsideDown( &port );
  AlignPortToWorld(&port, &world);

  scalePtr->world   = world;
  scalePtr->view    = view;
  scalePtr->port    = port;
  scalePtr->scaleX  = scaleX;
  scalePtr->scaleY  = scaleY;
  scalePtr->scrollX = scrollX;
  scalePtr->scrollY = scrollY;
  scalePtr->force   = FALSE;

  if (linHgtPtr != NULL)
    *linHgtPtr = lineHeight;
  if (linWidPtr != NULL)
    *linWidPtr = lineWidth;
}


/*****************************************************************************
*
*   EstimateScaling (viewer, picture, pntX, pntY, align, scaleX, scaleY)
*       Calculates maximum scaling factors in attaching a picture to a viewer
*
*****************************************************************************/

void EstimateScaling (VieweR viewer, SegmenT picture, Int4 pntX, Int4 pntY,
                      Int2 align, Int4Ptr scaleX, Int4Ptr scaleY)
{
  VScaleInfo scale;
  PicPPtr    pic = (PicPPtr)picture;

  if (viewer == NULL  ||  pic == NULL  ||  pic->base.code != PICTURE)
    return;

  SelectFont( systemFont );
  CalculateScaling(viewer, picture, pntX, pntY, align,
                   0, 0, &scale, NULL, NULL);

  if (scaleX != NULL)
    *scaleX = scale.scaleX;
  if (scaleY != NULL)
    *scaleY = scale.scaleY;
}


static void CorrectScrollbar(BaR sb, Boolean align_to_min, Int4 lineSize,
                             Int4 world_min, Int4 world_max,
                             Int4 port_min,  Int4 port_max)
{
  ASSERT ( world_min < world_max );
  ASSERT ( port_min  < port_max  );
  ASSERT ( lineSize  > 0 );

  if (sb == NULL)
    return;

  {{
    /*
    Int4 numLines = (world_max - world_min) / lineSize + 1;
    Int4 visLines = (port_max - port_min + 1) / lineSize;
    */
    Int4 numLines = (world_max - world_min + lineSize - 1) / lineSize;
    Int4 visLines = (port_max - port_min + lineSize - 1) / lineSize;
    Int4 max = MAX((numLines - visLines), 0);
    Int4 val = (align_to_min ? (port_min - world_min) : (world_max - port_max))
      / lineSize;

    CorrectBarMax  (sb, max);
    CorrectBarValue(sb, val);
    CorrectBarPage (sb, visLines - 1, visLines - 1);
  }}
}


/*****************************************************************************
*
*   AttachCommon (viewer, picture, pntX, pntY, align,
*                 scaleX, scaleY, draw, data, cleanup)
*       Connects a picture to a viewer, mapping world coordinate boundaries
*       onto viewer panel, calculating scaling and offset factors.  If the
*       scaleX or scaleY parameters are 0, the minimum scale for which no
*       scrolling is necessary is used.  Extra instance data can be attached
*
*****************************************************************************/

static void AttachCommon (VieweR viewer, SegmenT picture, Int4 pntX,
                          Int4 pntY, Int2 align, Int4 scaleX, Int4 scaleY,
                          VwrDrawProc draw, VoidPtr data, VwrFreeProc cleanup)

{
  ViewPData  extra;
  Int4       lineHeight;
  Int4       lineWidth;
  PicPPtr    pic = (PicPPtr)picture;

  if (viewer == NULL)
    return;

  SelectFont( systemFont );

  if (picture == NULL)
    {
      Reset (viewer);
      return;
    }

  if (pic->base.code != PICTURE)
    {
      Message (MSG_ERROR, "AttachPicture():  argument #2 is not a picture");
      return;
    }

  {{
    Int4 _scaleX = scaleX ? scaleX : 1;
    Int4 _scaleY = scaleY ? scaleY : 1;
    RecalculateSegment(picture, _scaleX, _scaleY);
  }}

  extra.viewer  = viewer;
  extra.picture = picture;
  CalculateScaling(viewer, picture, pntX, pntY, align, scaleX, scaleY,
                   &extra.scale, &lineHeight, &lineWidth);

  if (scaleX == 0  ||  scaleY == 0)
    {
      RecalculateSegment(picture, extra.scale.scaleX, extra.scale.scaleY);
      CalculateScaling(viewer, picture, pntX, pntY, align, scaleX, scaleY,
                       &extra.scale, &lineHeight, &lineWidth);
      RecalculateSegment(picture, extra.scale.scaleX, extra.scale.scaleY);
      CalculateScaling(viewer, picture, pntX, pntY, align, scaleX, scaleY,
                       &extra.scale, &lineHeight, &lineWidth);
      RecalculateSegment(picture, extra.scale.scaleX, extra.scale.scaleY);
      CalculateScaling(viewer, picture, pntX, pntY, align, scaleX, scaleY,
                       &extra.scale, &lineHeight, &lineWidth);
      RecalculateSegment(picture, extra.scale.scaleX, extra.scale.scaleY);
      CalculateScaling(viewer, picture, pntX, pntY, align,
                       extra.scale.scaleX, extra.scale.scaleY,
                       &extra.scale, &lineHeight, &lineWidth);
    }
  extra.click   = NULL;
  extra.drag    = NULL;
  extra.release = NULL;
  extra.pan     = NULL;
  extra.draw    = draw;
  extra.data    = data;
  extra.cleanup = cleanup;
  SetPanelExtra((PaneL)viewer, &extra);

  if (Visible( viewer )  &&  AllParentsVisible( viewer ))
    ResetClip();

  CorrectScrollbar(GetSlateHScrollBar( (SlatE)viewer ), TRUE, lineWidth,
                   extra.scale.world.left, extra.scale.world.right,
                   extra.scale.port.left,  extra.scale.port.right);

  CorrectScrollbar(GetSlateVScrollBar( (SlatE)viewer ), FALSE, lineHeight,
                   extra.scale.world.bottom, extra.scale.world.top,
                   extra.scale.port.bottom,  extra.scale.port.top);

  if (Visible( viewer )  &&  AllParentsVisible( viewer ))
    {
      WindoW tempPort = SavePort( viewer );
      Select( viewer );
      InvalRect( &extra.scale.view );
      RestorePort( tempPort );
    }
}


/*****************************************************************************
*
*   AttachPicture (viewer, picture, pntX, pntY, align, scaleX, scaleY, draw)
*       Connects a picture to a viewer
*
*****************************************************************************/

void AttachPicture (VieweR viewer, SegmenT picture, Int4 pntX, Int4 pntY,
                    Int2 align, Int4 scaleX, Int4 scaleY, VwrDrawProc draw)

{
  AttachCommon (viewer, picture, pntX, pntY, align,
                scaleX, scaleY, draw, NULL, NULL);
}

/*****************************************************************************
*
*   AttachInstance (viewer, picture, pntX, pntY, align,
*                   scaleX, scaleY, draw, data, cleanup)
*       Connects a picture to a viewer, with extra instance data
*
*****************************************************************************/

void AttachInstance (VieweR viewer, SegmenT picture, Int4 pntX, Int4 pntY,
                     Int2 align, Int4 scaleX, Int4 scaleY, VwrDrawProc draw,
                     VoidPtr data, VwrFreeProc cleanup)

{
  AttachCommon (viewer, picture, pntX, pntY, align,
                scaleX, scaleY, draw, data, cleanup);
}


/*****************************************************************************
*
*   ViewerWasResized (viewer)
*       Updates viewer internals after resizing
*
*****************************************************************************/

void ViewerWasResized (VieweR viewer)
{
  ViewPData  extra;
  RecT       r;

  if (viewer == NULL)
    return;

  GetPanelExtra((PaneL)viewer, &extra);
  if (extra.picture == NULL)
    return;

  ObjectRect(viewer, &r);
  InsetRect(&r, 4, 4);
  extra.scale.view = r;

  extra.scale.port.right = extra.scale.port.left +
    (extra.scale.view.right - extra.scale.view.left + 1) * extra.scale.scaleX;
  AlignPortToWorld_1D(extra.scale.world.left, extra.scale.world.right,
                      &extra.scale.port.left, &extra.scale.port.right, TRUE);

  extra.scale.port.bottom = extra.scale.port.top -
    (extra.scale.view.bottom - extra.scale.view.top + 1) * extra.scale.scaleY;
  AlignPortToWorld_1D(extra.scale.world.bottom, extra.scale.world.top,
                      &extra.scale.port.bottom, &extra.scale.port.top, FALSE);


  SetPanelExtra((PaneL)viewer, &extra);
  if (Visible( viewer )  &&  AllParentsVisible( viewer ))
    ResetClip ();

  CorrectScrollbar(GetSlateHScrollBar( (SlatE)viewer ), TRUE,
                   (extra.scale.scaleX * extra.scale.scrollX),
                   extra.scale.world.left, extra.scale.world.right,
                   extra.scale.port.left,  extra.scale.port.right);

  CorrectScrollbar(GetSlateVScrollBar( (SlatE)viewer ), FALSE,
                   (extra.scale.scaleY * extra.scale.scrollY),
                   extra.scale.world.bottom, extra.scale.world.top,
                   extra.scale.port.bottom,  extra.scale.port.top);

  if (Visible( viewer )  &&  AllParentsVisible( viewer ))
    {
      WindoW tempPort = SavePort( viewer );
      Select( viewer );
      InvalRect( &extra.scale.view );
      RestorePort( tempPort );
    }
}


/*****************************************************************************
*
*   PictureGrew (viewer)
*
*****************************************************************************/

Boolean PictureGrew (VieweR viewer)

{
  ViewPData  extra;
  PicPPtr    pic;
  Int4       scaleX;
  Int4       scaleY;
  BoxInfo    world;

  GetPanelExtra ((PaneL)viewer, &extra);
  if (extra.picture == NULL) return FALSE;
  pic = (PicPPtr) extra.picture;

  scaleX = extra.scale.scaleX;
  scaleY = extra.scale.scaleY;

  if (scaleX < 0)
    scaleX = -scaleX;
  if (scaleY < 0)
    scaleY = -scaleY;
  world.left   = pic->seg.box.left   - 10*scaleX;
  world.top    = pic->seg.box.top    + 10*scaleY + 1;
  world.right  = pic->seg.box.right  + 10*scaleX + 1;
  world.bottom = pic->seg.box.bottom - 10*scaleY;
  s_NormalizeBoxUpsideDown( &extra.scale.world );

  if (extra.scale.world.left != world.left ||
      extra.scale.world.top != world.top ||
      extra.scale.world.right != world.right ||
      extra.scale.world.bottom != world.bottom) {
    return TRUE;
  }

  return FALSE;
}

/*****************************************************************************
*
*   PictureHasEnlarged (viewer)
*       Updates viewer internals after picture enlarges dynamically
*
*****************************************************************************/

void PictureHasEnlarged (VieweR viewer)

{
  ViewPData  extra;
  PicPPtr    pic;
  Int4       scaleX;
  Int4       scaleY;

  if (viewer == NULL) return;

  GetPanelExtra ((PaneL) viewer, &extra);
  if (extra.picture == NULL) return;
  pic = (PicPPtr) extra.picture;

  scaleX = extra.scale.scaleX;
  scaleY = extra.scale.scaleY;

  if (scaleX < 0)
    scaleX = -scaleX;
  if (scaleY < 0)
    scaleY = -scaleY;
  extra.scale.world.left   = pic->seg.box.left   - 10*scaleX;
  extra.scale.world.top    = pic->seg.box.top    + 10*scaleY + 1;
  extra.scale.world.right  = pic->seg.box.right  + 10*scaleX + 1;
  extra.scale.world.bottom = pic->seg.box.bottom - 10*scaleY;
  s_NormalizeBoxUpsideDown( &extra.scale.world );

  SetPanelExtra((PaneL)viewer, &extra);
  if (Visible( viewer )  &&  AllParentsVisible( viewer ))
    ResetClip ();

  CorrectScrollbar(GetSlateHScrollBar( (SlatE)viewer ), TRUE,
                   (extra.scale.scaleX * extra.scale.scrollX),
                   extra.scale.world.left, extra.scale.world.right,
                   extra.scale.port.left,  extra.scale.port.right);

  CorrectScrollbar(GetSlateVScrollBar( (SlatE)viewer ), FALSE,
                   (extra.scale.scaleY * extra.scale.scrollY),
                   extra.scale.world.bottom, extra.scale.world.top,
                   extra.scale.port.bottom,  extra.scale.port.top);

  /*
  if (Visible( viewer )  &&  AllParentsVisible( viewer ))
    {
      WindoW tempPort = SavePort( viewer );
      Select( viewer );
      InvalRect( &extra.scale.view );
      RestorePort( tempPort );
    }
    */
}


/*****************************************************************************
*
*   SetViewerProcs (viewer, click, drag, release, pan)
*       Attaches program responsiveness callbacks to a viewer
*
*****************************************************************************/

void SetViewerProcs (VieweR viewer, VwrClckProc click, VwrClckProc drag,
                     VwrClckProc release, VwrPanProc pan)

{
  ViewPData  extra;

  if (viewer != NULL) {
    GetPanelExtra ((PaneL) viewer, &extra);
    extra.click = click;
    extra.drag = drag;
    extra.release = release;
    extra.pan = pan;
    SetPanelExtra ((PaneL) viewer, &extra);
  }
}

/*****************************************************************************
*
*   SetViewerData (viewer, data, cleanup)
*       Attaches instance data to a viewer
*
*****************************************************************************/

void SetViewerData (VieweR viewer, VoidPtr data, VwrFreeProc cleanup)

{
  ViewPData  extra;

  if (viewer != NULL) {
    GetPanelExtra ((PaneL) viewer, &extra);
    extra.data = data;
    extra.cleanup = cleanup;
    SetPanelExtra ((PaneL) viewer, &extra);
  }
}

/*****************************************************************************
*
*   GetViewerData (viewer)
*       Returns the instance data attached to a viewer
*
*****************************************************************************/

VoidPtr GetViewerData (VieweR viewer)

{
  ViewPData  extra;

  if (viewer != NULL) {
    GetPanelExtra ((PaneL) viewer, &extra);
    return (extra.data);
  } else {
    return NULL;
  }
}

/*****************************************************************************
*
*   ViewerBox (viewer, world, port, view, scaleX, scaleY)
*       Returns the world and port boxes, view rect, and scales of a viewer
*
*****************************************************************************/

void ViewerBox (VieweR viewer, BoxPtr world, BoxPtr port,
                RectPtr view, Int4Ptr scaleX, Int4Ptr scaleY)

{
  ViewPData  extra;

  if (viewer != NULL) {
    GetPanelExtra ((PaneL) viewer, &extra);
    if (world != NULL) {
      *world = extra.scale.world;
    }
    if (port != NULL) {
      *port = extra.scale.port;
    }
    if (view != NULL) {
      *view = extra.scale.view;
    }
    if (scaleX != NULL) {
      *scaleX = extra.scale.scaleX;
    }
    if (scaleY != NULL) {
      *scaleY = extra.scale.scaleY;
    }
  }
}

/*****************************************************************************
*
*   FindSegPrim (viewer, pt, segID, primID, prim)
*       Returns the deepest segment that contains the pt
*
*****************************************************************************/
SegmenT FindSegPrim (VieweR viewer, PoinT pt, Uint2Ptr segIDPtr,
                     Uint2Ptr primIDPtr, PrimitivE PNTR primPtr)
{
  PicPPtr    pic;
  SegmenT    found = NULL;
  PrimitivE  prim  = NULL;
  Uint2      segID = 0;
  Uint2      primID = 0;
  ScaleInfo  scale;
  ViewPData  extra;
  RecT       r;

  if (viewer != NULL) {
    GetPanelExtra ((PaneL) viewer, &extra);
    pic = (PicPPtr) extra.picture;
    if (pic->base.code == PICTURE) {
      MakeScaleInfo ( &(extra.scale) , &scale );
      LoadRect (&r, (Int2)(pt.x - PROXIMITY), (Int2)(pt.y - PROXIMITY),
                    (Int2)(pt.x + PROXIMITY), (Int2)(pt.y + PROXIMITY));
      MapRectToWorldBox ( &(scale.worldWindow), &r, &scale );
      found = SearchSegment ((SegmenT)pic, &scale, &prim );
    } else {
      Message (MSG_ERROR, "FindSegment argument not a picture");
    }
  }
  if (found != NULL) {
    segID = SegmentID (found);
    primID = ((GenPPtr)prim)->primID;
  }
  if (segIDPtr != NULL) {
    *segIDPtr = segID;
  }
  if (primIDPtr != NULL) {
    *primIDPtr = primID;
  }
  if ( primPtr != NULL) {
    *primPtr = prim;
  }
  return found;
}

/*****************************************************************************
*
*   FindSegment (viewer, pt, segID, primID, primCt)
*       Returns the deepest segment that contains the pt
*
*****************************************************************************/

SegmenT FindSegment (VieweR viewer, PoinT pt, Uint2Ptr segIDPtr,
                     Uint2Ptr primIDPtr, Uint2Ptr primCtPtr )
{
  SegmenT   found;
  PrimitivE prim;
  BasePPtr  item;
  SegPPtr   seg;
  Uint2     primCt = 0;

  found = FindSegPrim (viewer, pt, segIDPtr, primIDPtr, &prim );
  if ( found != NULL ) {
    seg = (SegPPtr) found;
    item = seg->seg.head;
    while (item != (BasePPtr)prim ) {
      item = item->next;
      primCt++;
    }
  }
  if ( primCtPtr != NULL ){
    *primCtPtr = (Int2)(primCt + 1);
  }
  return found;
}

/*****************************************************************************
*
*   ShowSegment (viewer, segment)
*       Makes a segment visible
*
*****************************************************************************/

void ShowSegment (VieweR viewer, SegmenT segment)

{
  RecT       d;
  ViewPData  extra;
  RecT       r;
  RecT       s;
  SegPPtr    seg;
  WindoW     tempPort;

  if (viewer != NULL && segment != NULL) {
    seg = (SegPPtr) segment;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      if (! seg->seg.visible) {
        seg->seg.visible = TRUE;
        GetPanelExtra ((PaneL) viewer, &extra);
        if (BoxInViewport (&r, &(seg->seg.box), &(extra.scale))) {
          InsetRect (&r, -1, -1);
          if (Visible (viewer) && AllParentsVisible (viewer)) {
            ObjectRect (viewer, &s);
            InsetRect (&s, 2, 2);
            SectRect (&r, &s, &d);
            tempPort = SavePort (viewer);
            Select (viewer);
            InvalRect (&d);
            RestorePort (tempPort);
          }
        }
      }
    } else {
      Message (MSG_ERROR, "ShowSegment argument not a segment or picture");
    }
  }
}

/*****************************************************************************
*
*   HideSegment (viewer, segment)
*       Maps a segment invisible
*
*****************************************************************************/

void HideSegment (VieweR viewer, SegmenT segment)

{
  RecT       d;
  ViewPData  extra;
  RecT       r;
  RecT       s;
  SegPPtr    seg;
  WindoW     tempPort;

  if (viewer != NULL && segment != NULL) {
    seg = (SegPPtr) segment;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      if (seg->seg.visible) {
        seg->seg.visible = FALSE;
        GetPanelExtra ((PaneL) viewer, &extra);
        if (BoxInViewport (&r, &(seg->seg.box), &(extra.scale))) {
          InsetRect (&r, -1, -1);
          if (Visible (viewer) && AllParentsVisible (viewer)) {
            ObjectRect (viewer, &s);
            InsetRect (&s, 2, 2);
            SectRect (&r, &s, &d);
            tempPort = SavePort (viewer);
            Select (viewer);
            InvalRect (&d);
            RestorePort (tempPort);
          }
        }
      }
    } else {
      Message (MSG_ERROR, "ShowSegment argument not a segment or picture");
    }
  }
}

/*****************************************************************************
*
*   HighlightSegment (viewer, segment, highlight)
*       Highlights a segment
*
*****************************************************************************/

void HighlightSegment (VieweR viewer, SegmenT segment, Int1 highlight)

{
  RecT       d;
  ViewPData  extra;
  RecT       r;
  RecT       s;
  SegPPtr    seg;
  WindoW     tempPort;

  if (viewer != NULL && segment != NULL) {
    seg = (SegPPtr) segment;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      if (seg->seg.highlight != highlight) {
        seg->seg.highlight = highlight;
        if (seg->seg.visible) {
          if (Visible (viewer) && AllParentsVisible (viewer)) {
            GetPanelExtra ((PaneL) viewer, &extra);
            if (BoxInViewport (&r, &(seg->seg.box), &(extra.scale))) {
              InsetRect (&r, -5, -5);
              ObjectRect (viewer, &s);
              InsetRect (&s, 2, 2);
              SectRect (&r, &s, &d);
              tempPort = SavePort (viewer);
              Select (viewer);
              InvalRect (&d);
              RestorePort (tempPort);
            }
          }
        }
      }
    } else {
      Message (MSG_ERROR, "HighlightSegment argument not a segment or picture");
    }
  }
}

/*****************************************************************************
*
*   HighlightPrimitive (viewer, segment, primitive, highlight)
*       Highlights a primitive
*
*****************************************************************************/

void HighlightPrimitive (VieweR viewer, SegmenT segment, PrimitivE primitive,
Int1 highlight)

{
  RecT       d;
  ViewPData  extra;
  BasePPtr   item;
  RecT       r;
  RecT       s;
  SegPPtr    seg;
  WindoW     tempPort;
  BoxInfo    pLimits;

  if (viewer != NULL && segment != NULL) {
    seg = (SegPPtr) segment;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      item = (BasePPtr) primitive;
      if (item != NULL && TryHighlightPrimitive (item, highlight)) {
        if (seg->seg.visible) {
          if (Visible (viewer) && AllParentsVisible (viewer)) {
            GetPanelExtra ((PaneL) viewer, &extra);
            TryGetPrimitiveLimits ( item, extra.scale.scaleX,
                                    extra.scale.scaleY, &pLimits );
            if (BoxInViewport (&r, &(pLimits), &(extra.scale))) {
              InsetRect (&r, -5, -5);
              ObjectRect (viewer, &s);
              InsetRect (&s, 2, 2);
              SectRect (&r, &s, &d);
              tempPort = SavePort (viewer);
              Select (viewer);
              InvalRect (&d);
              RestorePort (tempPort);
            }
          }
        }
      }
    } else {
      Message (MSG_ERROR, "HighlightPrimitive argument not a segment or picture");
    }
  }
}

/*****************************************************************************
*
*   ResetHighlight (viewer, segment)
*       Resets all highlighting within a segment
*
*****************************************************************************/

static void ResetSegmentHighlight (VieweR viewer, SegmenT segment)

{
  BasePPtr  item;
  SegPPtr    seg;

  if (viewer != NULL && segment != NULL) {
    seg = (SegPPtr) segment;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      seg->seg.highlight = PLAIN_SEGMENT;
      item = seg->seg.head;
      while (item != NULL) {
        switch (item->code) {
          case UNKNOWN :
            break;
          case PICTURE :
            seg->seg.highlight = PLAIN_SEGMENT;
            ResetSegmentHighlight (viewer, (SegmenT) item);
            break;
          case SEGMENT :
            seg->seg.highlight = PLAIN_SEGMENT;
            ResetSegmentHighlight (viewer, (SegmenT) item);
            break;
          case GENERIC :
            TryHighlightPrimitive (item, PLAIN_PRIMITIVE);
            break;
          default :
            break;
        }
        item = item->next;
      }
    }
  }
}

void ResetHighlight (VieweR viewer, SegmenT segment)

{
  RecT    r;
  WindoW  tempPort;

  if (viewer != NULL && segment != NULL) {
    ResetSegmentHighlight (viewer, segment);
    tempPort = CurrentWindow ();
    UseWindow (ParentWindow (viewer));
    ObjectRect (viewer, &r);
    InsetRect (&r, 3, 3);
    InvalRect (&r);
    UseWindow (tempPort);
  }
}


/*****************************************************************************
*
*   PrintViewer (viewer)
*       Prints the picture.
*
*****************************************************************************/
void PrintViewer (VieweR viewer)
{ /* print only those segments of the picture which are currently
     shown in the viewer -- and no more than one printer page */
  ViewPData    extra;
  PicPPtr      pic;
  WindoW       w;
  Nlm_DrawInfo dInfo;

  if (viewer == NULL)
    return;

  GetPanelExtra((PaneL) viewer, &extra);
  pic = (PicPPtr)extra.picture;
  if (pic == NULL)
    return;

  if (pic->base.code != PICTURE)
    {
      Message (MSG_ERROR, "PrintViewer target is not a picture");
      return;
    }

  w = StartPrinting();
  if (w == NULL)
    return;

  if ( StartPage() )
    {
      PrintingRect( &extra.scale.view );
      extra.scale.force = TRUE;
      MakeScaleInfo(&extra.scale, &dInfo.scale);
      MapRectToWorldBox(&dInfo.scale.worldWindow, &extra.scale.view,
                        &dInfo.scale);
      dInfo.checked = TRUE;
      Nlm_DrawSegment((SegPPtr)pic, &dInfo);
      EndPage();
    }

  EndPrinting( w );
}


void PrintAllViewer (VieweR viewer)
{ /* print the whole picture on as many pages as needed */
  ViewPData    extra;
  PicPPtr      pict;
  WindoW       w;
  Nlm_DrawInfo dInfo;
  Int4         pageWidth, pageHeight;
  Int4         pictWidth, pictHeight;
  Int4         x, y;
  Int4         dInfo_offsetY, dInfo_left_WC, dInfo_bottom_WC;
  Boolean      ok = TRUE;

  if (viewer == NULL)
    return;

  GetPanelExtra((PaneL)viewer, &extra);
  pict = (PicPPtr)extra.picture;
  if (pict == NULL)
    return;

  if (pict->base.code != PICTURE)
    Message(MSG_ERROR, "PrintViewer source is not a picture");

  w = StartPrinting();
  if (w == NULL)
    return;

  PrintingRect    ( &extra.scale.view );
  ClipPrintingRect( &extra.scale.view );
  pageWidth  = (extra.scale.view.right  - extra.scale.view.left) *
    extra.scale.scaleX;
  pageHeight = (extra.scale.view.bottom - extra.scale.view.top ) *
    extra.scale.scaleY;
  pictWidth  = pict->seg.box.right - pict->seg.box.left;
  pictHeight = pict->seg.box.top   - pict->seg.box.bottom;
  if ( pageHeight )
    pictHeight = (pictHeight + pageHeight - 1) / pageHeight * pageHeight;

  dInfo_left_WC   = extra.scale.port.left   = pict->seg.box.left;
  dInfo_bottom_WC = extra.scale.port.bottom = pict->seg.box.top - pictHeight;
  MakeScaleInfo(&extra.scale, &dInfo.scale);
  dInfo_offsetY   = dInfo.scale.offsetY;
  dInfo.checked = TRUE;

  for (x = 0;  ok  &&  x < pictWidth;   x += pageWidth)
    {
      dInfo.scale.offsetY = dInfo_offsetY;
      for (y = 0;  y < pictHeight;  y += pageHeight)
        {
          BoxInfo *print_port = &dInfo.scale.worldWindow;
          print_port->left   = dInfo_left_WC   + x;
          print_port->bottom = dInfo_bottom_WC + y;
          print_port->right  = print_port->left   + pageWidth  - 1;
          print_port->top    = print_port->bottom + pageHeight - 1;

          ok = StartPage();
          if ( !ok )
            break;

          Nlm_DrawSegment((SegPPtr)pict, &dInfo);

          ok = EndPage();
          if ( !ok )
            break;

          dInfo.scale.offsetY += pageHeight;
        }
      dInfo.scale.offsetX -= pageWidth;
    }

  Nlm_ResetClip();
  EndPrinting( w );
}


/*****************************************************************************
*
*   CopyViewer (viewer)
*       Saves the picture to the clipboard.
*
*****************************************************************************/

void CopyAllViewer (VieweR viewer)
{
  ViewPData    extra;
  PicPPtr      pic;
  WindoW       w;
  Nlm_DrawInfo dInfo;

  if (viewer == NULL)
    return;

  GetPanelExtra((PaneL)viewer, &extra);
  pic = (PicPPtr)extra.picture;

  if (pic == NULL)
    return;

  if (pic->base.code != PICTURE) {
    Message (MSG_ERROR, "CopyAllViewer target is not a picture");
    return;
  }

  MakeScaleInfo(&extra.scale, &dInfo.scale);
  MapWorldBoxToRect(&extra.scale.view, &pic->seg.box, &dInfo.scale);
  MapRectToWorldBox(&dInfo.scale.worldWindow, &extra.scale.view, &dInfo.scale);
  w = StartPicture( &extra.scale.view );
  if (w == NULL)
    return;

  extra.scale.force = TRUE;
  dInfo.checked = TRUE;
  Nlm_DrawSegment((SegPPtr)pic, &dInfo);
  EndPicture( w );
}


void CopyViewer (VieweR viewer)
{
  ViewPData    extra;
  PicPPtr      pic;
  WindoW       w;
  Nlm_DrawInfo dInfo;

  if (viewer == NULL)
    return;

  GetPanelExtra((PaneL)viewer, &extra);
  pic = (PicPPtr)extra.picture;

  if (pic == NULL)
    return;

  if (pic->base.code != PICTURE) {
    Message (MSG_ERROR, "CopyViewer target is not a picture");
    return;
  }

#ifdef WIN_MSWIN
  OffsetRect(&extra.scale.view,
             (Int2)(-extra.scale.view.left), (Int2)(-extra.scale.view.top));
#endif
  w = StartPicture( &extra.scale.view );
  if (w == NULL)
    return;

  extra.scale.force = TRUE;
  MakeScaleInfo(&extra.scale, &dInfo.scale);
  MapRectToWorldBox(&dInfo.scale.worldWindow, &extra.scale.view, &dInfo.scale);
  dInfo.checked = FALSE;
  Nlm_DrawSegment((SegPPtr)pic, &dInfo);
  EndPicture( w );
}


/*****************************************************************************
*
*   MapWorldToViewer (viewer, pnt, pt)
*       Maps a pnt in world coordinates to a point in viewer coordinates
*
*****************************************************************************/

void MapWorldToViewer (VieweR viewer, PntInfo pnt, PointPtr pt)

{
  ViewPData  extra;
  ScaleInfo  scale;

  if (viewer != NULL && pt != NULL) {
    GetPanelExtra ((PaneL) viewer, &extra);
    MakeScaleInfo ( &(extra.scale), &scale );
    MapWorldPointToPixel (pt, &pnt, &scale);
  }
}

/*****************************************************************************
*
*   MapViewerToWorld (viewer, pt, pnt)
*       Maps a point in viewer coordinates to a pnt in world coordinates
*
*****************************************************************************/

void MapViewerToWorld (VieweR viewer, PoinT pt, PntPtr pnt)

{
  ViewPData  extra;
  ScaleInfo  scale;

  if (viewer != NULL && pnt != NULL) {
    GetPanelExtra ((PaneL) viewer, &extra);
    MakeScaleInfo ( &(extra.scale), &scale );
    MapPixelPointToWorld (pnt, &pt, &scale);
  }
}

