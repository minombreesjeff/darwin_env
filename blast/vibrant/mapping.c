/*   mapping.c
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
* File Name:  mapping.c
*
* Author:  Jonathan Kans, Alex Smirnov, Jill Shermer
*
* Version Creation Date:   1/19/93
*
* $Revision: 6.4 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: mapping.c,v $
* Revision 6.4  1999/10/04 17:16:31  kans
* include ncbidraw.h instead of vibrant.h, a couple Nlm_ prefixes
*
* Revision 6.3  1999/08/06 19:29:45  vakatov
* "NormalizeBox()":  bottom >= top!
*
* Revision 6.2  1999/08/06 18:42:32  vakatov
* Moved "NormalizeBox()" from "viewer.c" to "mappingp.[ch]" & made it public
*
* Revision 6.1  1998/06/12 16:40:24  kans
* fixed warnings detected by unix compiler
* ==========================================================================
*/

#ifndef _NCBIDRAW_
#include <ncbidraw.h>
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


/*****************************************************************************
*
*   LoadBox (box, left, top, right, bottom)
*       Initializes a box structure with the specified parameters
*
*****************************************************************************/

extern void Nlm_LoadBox
(Nlm_BoxPtr box, Int4 left, Int4 top, Int4 right, Int4 bottom)
{
  if (box != NULL) {
    box->left = left;
    box->top = top;
    box->right = right;
    box->bottom = bottom;
  }
}

extern void Nlm_OutsetBox
(Nlm_BoxPtr box, Int4 dX, Int4 dY )
{
  register BoxPtr boxP;

  boxP = box;
  boxP->left -= dX;
  boxP->top += dY;
  boxP->right += dX;
  boxP->bottom -= dY;
}

/*****************************************************************************
*
*   MapWorldPointToPixel (pt, pnt, scale)
*       Maps a world coordinates pnt into a viewer coordinates point
*
*****************************************************************************/

extern void Nlm_MapWorldPointToPixel
(Nlm_PointPtr pt, Nlm_PntPtr pnt, Nlm_ScalePtr scale)
{
  if (pt != NULL && pnt != NULL && scale != NULL) {
    pt->x = (Nlm_Int2)((scale->offsetX + pnt->x) / scale->scaleX);
    pt->y = (Nlm_Int2)((scale->offsetY - pnt->y) / scale->scaleY);
  }
}

/*****************************************************************************
*
*   MapPixelPointToWorld (pnt, pt, scale)
*       Maps a point in viewer coordinates to a pnt in world coordinates
*
*****************************************************************************/

extern void Nlm_MapPixelPointToWorld
(Nlm_PntPtr pnt, Nlm_PointPtr pt, Nlm_ScalePtr scale)
{
  if (pnt != NULL && pt != NULL && scale != NULL) {
    pnt->x = (Int4)pt->x * scale->scaleX - scale->offsetX;
    pnt->y = scale->offsetY - (Int4)pt->y * scale->scaleY;
  }
}

extern void Nlm_MapWorldBoxToRect
(Nlm_RectPtr r, Nlm_BoxPtr box, Nlm_ScalePtr scale )
{
  Int4 curScale;

  if (r != NULL && box != NULL && scale != NULL) {
    curScale = scale->scaleX;
    r->left = (Int2)((scale->offsetX + box->left) / curScale);
    r->right = (Int2)((scale->offsetX + box->right) / curScale);
    curScale = scale->scaleY;
    r->top = (Int2)((scale->offsetY - box->top) / curScale);
    r->bottom = (Int2)((scale->offsetY - box->bottom) / curScale);
  }
}

extern void Nlm_MapRectToWorldBox
(Nlm_BoxPtr box, Nlm_RectPtr r, Nlm_ScalePtr scale)
{
  Int4 curScale;

  if (r != NULL && box != NULL && scale != NULL) {
    curScale = scale->scaleX;
    box->left = (Int4)r->left * scale->scaleX - scale->offsetX;
    box->right = (Int4)r->right * scale->scaleX - scale->offsetX;
    curScale = scale->scaleY;
    box->top = scale->offsetY - (Int4)r->top * scale->scaleY;
    box->bottom = scale->offsetY - (Int4)r->bottom * scale->scaleY;
  }
}

/*****************************************************************************
*
*   MapX (pntX, scale)
*       Maps a horizontal world coordinate to an Int4 in viewer coordinates
*
*****************************************************************************/

static Int4 MapX (Int4 pntX, VScalePtr scale)
{
  return (Int4) scale->view.left + (pntX - scale->port.left) / scale->scaleX;
}

/*****************************************************************************
*
*   MapY (pntY, scale)
*       Maps a vertical world coordinate to an Int4 in viewer coordinates
*
*****************************************************************************/

static Int4 MapY (Int4 pntY, VScalePtr scale)
{
  return (Int4)scale->view.bottom - (pntY - scale->port.bottom)/scale->scaleY;
}

/*****************************************************************************
*
*   BoxInViewport (rct, box, scale)
*       Determines whether a box is visible in the viewport
*
*****************************************************************************/

extern Boolean BoxInViewport
(Nlm_RectPtr rct, Nlm_BoxPtr box, VScalePtr scale)
{
  if (!box  ||  !scale  ||
      MAX(box->left, scale->port.left) > MIN(box->right, scale->port.right)  ||
      MAX(box->bottom, scale->port.bottom) > MIN(box->top, scale->port.top))
    return FALSE;

  if ( !rct )
    return TRUE;

  rct->left   = (Int2)
    MAX (MapX(box->left,   scale),  (Int4)(scale->view.left   - 20));
  rct->top    = (Int2)
    MAX (MapY(box->top,    scale),  (Int4)(scale->view.top    - 20));
  rct->right  = (Int2)
    MIN (MapX(box->right,  scale),  (Int4)(scale->view.right  + 20));
  rct->bottom = (Int2)
    MIN (MapY(box->bottom, scale),  (Int4)(scale->view.bottom + 20));
  return TRUE;
}


/*****************************************************************************
*
*   LineIntoVPort (x1,y1,x2,y2,worldWindow)
*
*****************************************************************************/
extern Boolean LineIntoVPort
(Int4Ptr x1, Int4Ptr y1, Int4Ptr x2, Int4Ptr y2, Nlm_BoxPtr worldWindow)
{
  register Int4 ax;
  register Int4 bx;
  Int4     xx1,xx2,yy1,yy2;
  BoxInfo  winW;
  Int2     status1 = 0;
  Int2     status2 = 0;

  winW = *worldWindow;
  ax = *x1;
  xx1 = ax;
  if ( ax < winW.left ) {
    status1 = 1;
  } else if ( ax > winW.right ) {
    status1 = 2;
  }
  ax = *x2;
  xx2 = ax;
  if ( ax < winW.left ) {
    status2 = 1;
  } else if ( ax > winW.right ) {
    status2 = 2;
  }
  ax = *y1;
  yy1 = ax;
  if ( ax < winW.bottom ) {             /* 7 6 8 */
    status1 += 3;                       /* 1 0 2 */
  } else if ( ax > winW.top ) {         /* 4 3 5 */
    status1 += 6;
  }
  ax = *y2;
  yy2 = ax;
  if ( ax < winW.bottom ) {
    status2 += 3;
  } else if ( ax > winW.top ) {
    status2 += 6;
  }
  ax = xx1;
  bx = yy1;
  switch ( status1 ){
  case 0: 
    break;
  case 1:
    ax = bx + (yy2 - bx) * (winW.left - ax)/(xx2 - ax);
    if ( (ax < winW.bottom) || (ax > winW.top) ) return FALSE;
    *y1 = ax;
    *x1 = winW.left;
    break;
  case 2:
    ax = bx + (yy2 - bx) * (winW.right - ax)/(xx2 - ax);
    if ( (ax < winW.bottom) || (ax > winW.top) ) return FALSE;
    *y1 = ax;
    *x1 = winW.right;
    break;
  case 3:
    ax = ax + (xx2 - ax) * (winW.bottom - bx)/(yy2 - bx);
    if ( (ax < winW.left) || (ax > winW.right) ) return FALSE;
    *x1 = ax;
    *y1 = winW.bottom;
    break;
  case 4:
    ax = bx + (yy2 - bx) * (winW.left - ax)/(xx2 - ax);
    if ( (ax < winW.bottom) || (ax > winW.top) ) {
      ax = xx1;
      ax = ax + (xx2 - ax) * (winW.bottom - bx)/(yy2 - bx);
      if ( (ax < winW.left) || (ax > winW.right) ) return FALSE;
      *x1 = ax;
      *y1 = winW.bottom;
    } else {
      *y1 = ax;
      *x1 = winW.left;
    }
    break;
  case 5:
    ax = bx + (yy2 - bx) * (winW.right - ax)/(xx2 - ax);
    if ( (ax < winW.bottom) || (ax > winW.top) ) {
      ax = xx1;
      ax = ax + (xx2 - ax) * (winW.bottom - bx)/(yy2 - bx);
      if ( (ax < winW.left) || (ax > winW.right) ) return FALSE;
      *x1 = ax;
      *y1 = winW.bottom;
    } else {
      *y1 = ax;
      *x1 = winW.right;
    }
    break;
  case 6:
    ax = ax + (xx2 - ax) * (winW.top - bx)/(yy2 - bx);
    if ( (ax < winW.left) || (ax > winW.right) ) return FALSE;
    *x1 = ax;
    *y1 = winW.top;
    break;
  case 7:
    ax = bx + (yy2 - bx) * (winW.left - ax)/(xx2 - ax);
    if ( (ax < winW.bottom) || (ax > winW.top) ) {
      ax = xx1;
      ax = ax + (xx2 - ax) * (winW.top - bx)/(yy2 - bx);
      if ( (ax < winW.left) || (ax > winW.right) ) return FALSE;
      *x1 = ax;
      *y1 = winW.top;
    } else {
      *y1 = ax;
      *x1 = winW.left;
    }
    break;
  default:
    ax = bx + (yy2 - bx) * (winW.right - ax)/(xx2 - ax);
    if ( (ax < winW.bottom) || (ax > winW.top) ) {
      ax = xx1;
      ax = ax + (xx2 - ax) * (winW.top - bx)/(yy2 - bx);
      if ( (ax < winW.left) || (ax > winW.right) ) return FALSE;
      *x1 = ax;
      *y1 = winW.top;
    } else {
      *y1 = ax;
      *x1 = winW.right;
    }
  }
  ax = xx1;
  bx = yy1;
  switch ( status2 ){
  case 0: 
    break;
  case 1:
    ax = bx + (yy2 - bx) * (winW.left - ax)/(xx2 - ax);
    if ( (ax < winW.bottom) || (ax > winW.top) ) return FALSE;
    *y2 = ax;
    *x2 = winW.left;
    break;
  case 2:
    ax = bx + (yy2 - bx) * (winW.right - ax)/(xx2 - ax);
    if ( (ax < winW.bottom) || (ax > winW.top) ) return FALSE;
    *y2 = ax;
    *x2 = winW.right;
    break;
  case 3:
    ax = ax + (xx2 - ax) * (winW.bottom - bx)/(yy2 - bx);
    if ( (ax < winW.left) || (ax > winW.right) ) return FALSE;
    *x2 = ax;
    *y2 = winW.bottom;
    break;
  case 4:
    ax = bx + (yy2 - bx) * (winW.left - ax)/(xx2 - ax);
    if ( (ax < winW.bottom) || (ax > winW.top) ) {
      ax = xx1;
      ax = ax + (xx2 - ax) * (winW.bottom - bx)/(yy2 - bx);
      if ( (ax < winW.left) || (ax > winW.right) ) return FALSE;
      *x2 = ax;
      *y2 = winW.bottom;
    } else {
      *y2 = ax;
      *x2 = winW.left;
    }
    break;
  case 5:
    ax = bx + (yy2 - bx) * (winW.right - ax)/(xx2 - ax);
    if ( (ax < winW.bottom) || (ax > winW.top) ) {
      ax = xx1;
      ax = ax + (xx2 - ax) * (winW.bottom - bx)/(yy2 - bx);
      if ( (ax < winW.left) || (ax > winW.right) ) return FALSE;
      *x2 = ax;
      *y2 = winW.bottom;
    } else {
      *y2 = ax;
      *x2 = winW.right;
    }
    break;
  case 6:
    ax = ax + (xx2 - ax) * (winW.top - bx)/(yy2 - bx);
    if ( (ax < winW.left) || (ax > winW.right) ) return FALSE;
    *x2 = ax;
    *y2 = winW.top;
    break;
  case 7:
    ax = bx + (yy2 - bx) * (winW.left - ax)/(xx2 - ax);
    if ( (ax < winW.bottom) || (ax > winW.top) ) {
      ax = xx1;
      ax = ax + (xx2 - ax) * (winW.top - bx)/(yy2 - bx);
      if ( (ax < winW.left) || (ax > winW.right) ) return FALSE;
      *x2 = ax;
      *y2 = winW.top;
    } else {
      *y2 = ax;
      *x2 = winW.left;
    }
    break;
  default:
    ax = bx + (yy2 - bx) * (winW.right - ax)/(xx2 - ax);
    if ( (ax < winW.bottom) || (ax > winW.top) ) {
      ax = xx1;
      ax = ax + (xx2 - ax) * (winW.top - bx)/(yy2 - bx);
      if ( (ax < winW.left) || (ax > winW.right) ) return FALSE;
      *x2 = ax;
      *y2 = winW.top;
    } else {
      *y2 = ax;
      *x2 = winW.right;
    }
  }
  return TRUE;
}

/*****************************************************************************
*
*   IsLineInVPort (x1,y1,x2,y2,worldWindow)
*
*****************************************************************************/
extern Boolean IsLineInVPort
(Int4 x1, Int4 y1, Int4 x2, Int4 y2, Nlm_BoxPtr worldWindow) 
{
  register Int4 ax;
  BoxInfo  winW;
  Int2     status1 = 0;

  winW = *worldWindow;
  ax = x1;
  if ( ax < winW.left ) {
    status1 = 1;
  } else if ( ax > winW.right ) {
    status1 = 2;
  } 
  ax = y1;                              /* 7 6 8 */
  if ( ax < winW.bottom ) {             /* 1 0 2 */
    status1 += 3;                       /* 4 3 5 */
  } else if ( ax > winW.top ) {
    status1 += 6;
  }
  switch ( status1 ) {
  case 0:
    return TRUE;
  case 1:
    if ( y2 == y1 ) return TRUE;
    ax = y1 + (y2 - y1) * (winW.left - x1) / (x2 - x1);
    if ( (ax >= winW.bottom) && (ax <= winW.top) ) return TRUE;
    break;
  case 2:
    if ( y2 == y1 ) return TRUE;
    ax = y1 + (y2 - y1) * (winW.right - x1) / (x2 - x1);
    if ( (ax >= winW.bottom) && (ax <= winW.top) ) return TRUE;
    break;
  case 3:
    if ( x2 == x1 ) return TRUE;
    ax = x1 + (x2 - x1) * (winW.bottom - y1) / (y2 - y1);
    if ( (ax >= winW.left) && (ax <= winW.right) ) return TRUE;
    break;
  case 4:
    ax = x1 + (x2 - x1) * (winW.bottom - y1) / (y2 - y1);
    if ( (ax >= winW.left) && (ax <= winW.right) ) return TRUE;
    ax = y1 + (y2 - y1) * (winW.left - x1) / (x2 - x1);
    if ( (ax >= winW.bottom) && (ax <= winW.top) ) return TRUE;
    break;
  case 5:
    ax = x1 + (x2 - x1) * (winW.bottom - y1) / (y2 - y1);
    if ( (ax >= winW.left) && (ax <= winW.right) ) return TRUE;
    ax = y1 + (y2 - y1) * (winW.right - x1) / (x2 - x1);
    if ( (ax >= winW.bottom) && (ax <= winW.top) ) return TRUE;
    break;
  case 6:
    if ( x2 == x1 ) return TRUE;
    ax = x1 + (x2 - x1) * (winW.top - y1) / (y2 - y1);
    if ( (ax >= winW.left) && (ax <= winW.right) ) return TRUE;
    break;
  case 7:
    ax = y1 + (y2 - y1) * (winW.left - x1) / (x2 - x1);
    if ( (ax >= winW.bottom) && (ax <= winW.top) ) return TRUE;
    ax = x1 + (x2 - x1) * (winW.top - y1) / (y2 - y1);
    if ( (ax >= winW.left) && (ax <= winW.right) ) return TRUE;
    break;
  default:
    ax = x1 + (x2 - x1) * (winW.top - y1) / (y2 - y1);
    if ( (ax >= winW.left) && (ax <= winW.right) ) return TRUE;
    ax = y1 + (y2 - y1) * (winW.right - x1) / (x2 - x1);
    if ( (ax >= winW.bottom) && (ax <= winW.top) ) return TRUE;
  }
  return FALSE;
}


/*****************************************************************************
*
*   NormalizeBox (box)
*       Ensures that left <= right and top <= bottom in world coordinates
*
*****************************************************************************/

extern void NormalizeBox
(Nlm_BoxInfo* box)
{
  Int4  swap;
  if (box->left > box->right) {
    swap = box->left;
    box->left = box->right;
    box->right = swap;
  }
  if (box->top > box->bottom) {
    swap = box->bottom;
    box->bottom = box->top;
    box->top = swap;
  }
}

