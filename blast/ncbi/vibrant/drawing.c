/*   drawing.c
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
* File Name:  drawing.c
*
* Author:  Jonathan Kans, Alex Smirnov, Jill Shermer, Denis Vakatov
*
* Version Creation Date:   11/13/92
*
* $Revision: 6.16 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: drawing.c,v $
* Revision 6.16  2003/03/18 17:00:10  kans
* Nlm_AddSilentSegRect does not absorb clicks, SegRect reverted to absorb clicks, so desktop click responsiveness is maintained
*
* Revision 6.15  2003/02/19 17:36:19  rsmith
* SegRect (borders around a SegmenT) can not longer absorb clicks.
*
* Revision 6.14  2001/12/13 14:51:31  kans
* DrawSegment calls SetPrimAttribute only if it will be drawing
*
* Revision 6.13  2000/10/25 20:47:17  vakatov
* Rollback R6.10 (still may need add. fix for MS-Windows) [w/ V.Chetvernin]
*
* Revision 6.12  1999/10/04 17:16:30  kans
* include ncbidraw.h instead of vibrant.h, a couple Nlm_ prefixes
*
* Revision 6.11  1998/07/01 18:27:39  vakatov
* Use "const" qualifier somewhere
*
* Revision 6.10  1998/06/25 18:41:12  vakatov
* s_DoRoundRect(): oval_[hc] -- multiply by 2 when using Paint/FrameRoundRect
*
* Revision 6.9  1998/06/15 22:07:44  vakatov
* RecDrawProc(), RoRecDrawProc() -- do not add extra pixel to the
* horiz. and vert. dimensions; thus get it in-sync with other primitives
*
* Revision 6.8  1998/06/01 17:29:18  vakatov
* Extended rounded rect. options -- let it roundup only two(adjacent)
* corners(see "flags" arg in Nlm_AddRoundedRectangle())
*
* Revision 6.7  1998/04/27 16:03:57  vakatov
* Added rounded rectangles
* Re-implemented the ovals -- dont emulate them by polygons anymore,
* use the "native" drawing functions instead
*
* Revision 6.6  1997/12/16 18:38:39  kans
* sentinel draw callback has draw info context parameter
*
* Revision 6.5  1997/12/15 18:10:46  kans
* added cleanup data function to sentinel
*
* Revision 6.4  1997/11/19 17:38:09  kans
* added ChangeSentinelRectangle
*
* Revision 6.3  1997/11/13 21:41:31  vakatov
* Added "sentinel rectangle" primitive[Nlm_AddSntRectangle] which
* calls its callback procedure[Nlm_SntOnDrawProc] on each redraw
*
* Revision 6.2  1997/10/30 20:01:40  vakatov
* Added Nlm_PrimitiveBox()
*
* Revision 6.1  1997/09/16 18:56:54  vakatov
* RecDrawProc():  setup WIDTH_ATT for non-filled rectangle
*
* Revision 5.4  1997/07/23 19:48:54  vakatov
* Use Nlm_PaintStringEx function where non-display HDC can be used
* (on MS-Win, MoveToEx()/GetCurrentPositionEx() don't work in this case)
*
* Revision 5.3  1997/05/08 14:05:21  vakatov
* [WIN_MSWIN] PaintStringWin() is obsolete; replaced by MoveTo + PaintString
*
 * Revision 5.2  1996/11/07  15:06:06  kans
 * extra break needed
 *
 * Revision 5.1  1996/11/05  22:30:11  vakatov
 * MakeAlignRect()/...Box():  Made the alignment be UPPER_LEFT by default
 *
 * Revision 4.7  1996/03/21  21:05:23  vakatov
 * Arrow-rectangle FRAME_PRIMITIVE highlighting now merely
 * draw a box embracing RECTANGLE or LINE primitive
 *
 * Revision 4.6  1996/03/15  19:49:52  vakatov
 * Arrow-rectangle FRAME_PRIMITIVE highlighting corrected
 *
 * Revision 4.5  1995/11/17  20:44:48  smirnov
 * Alex: fix DrawArc function.
 *
 * Revision 4.4  1995/11/09  16:35:55  kans
 * removed static from AddArc
 *
 * Revision 4.3  1995/11/07  23:21:53  kans
 * moved Nlm_DrawSegment from viewer.c to drawing.c (for GIF without Vibrant)
 *
 * Revision 4.1  1995/09/12  00:39:10  ostell
 * changes for text to appear in windows metafiles
 *
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

#ifndef _DRAWINGP_
#include <drawingp.h>
#endif

#ifndef _VIEWERP_
#include <viewerp.h>
#endif

/*****************************************************************************
*
*   DEFINES
*
*****************************************************************************/
#define ARROWWIDTH2 36

/*****************************************************************************
*
*   EXTERNAL VARIABLES
*
*****************************************************************************/
FonT  smallFont = NULL;
FonT  mediumFont = NULL;
FonT  largeFont = NULL;

AttPData blackAttPData = 
{ {0,0,0}, SOLID_LINE, SOLID_SHADING, STD_PEN_WIDTH, COPY_MODE };
AttPData whiteAttPData = 
{ {255,255,255}, SOLID_LINE, SOLID_SHADING, STD_PEN_WIDTH, COPY_MODE };

/*****************************************************************************
*
*   STATIC VARIABLES
*
*****************************************************************************/
static Int1 sin100[91] = {
0,2,3,5,7,9,10,12,14,16,17,19,21,22,24,26,28,29,31,33,34,36,37,39,41,42,
44,45,47,48,50,51,53,54,56,57,59,60,62,63,64,66,67,68,69,71,72,73,74,75,
77,78,79,80,81,82,83,84,85,86,87,87,88,89,90,91,91,92,93,93,94,95,95,96,
96,97,97,97,98,98,98,99,99,99,99,100,100,100,100,100,100 };

static Int1 atan100[101] = {
0,1,1,2,2,3,3,4,5,5,6,6,7,7,8,9,9,10,10,11,11,12,12,13,13,14,15,15,16,16,17,
17,18,18,19,19,20,20,21,21,22,22,23,23,24,24,25,25,26,26,27,27,27,28,28,29,
29,30,30,31,31,31,32,32,33,33,33,34,34,35,35,35,36,36,37,37,37,38,38,38,39,
39,39,40,40,40,41,41,41,42,42,42,43,43,43,44,44,44,44,45,45 };

static PoinT polyPoints[362];

static Uint1  thinNews [] = {
  0x11, 0x22, 0x44, 0x88, 0x11, 0x22, 0x44, 0x88
};
static Uint1  thinNwse [] = {
  0x88, 0x44, 0x22, 0x11, 0x88, 0x44, 0x22, 0x11
};
static Uint1  thickNews [] = {
  0x33, 0x66, 0xCC, 0x99, 0x33, 0x66, 0xCC, 0x99
};
static Uint1  thickNwse [] = {
  0x99, 0xCC, 0x66, 0x33, 0x99, 0xCC, 0x66, 0x33
};
static Uint1  thinHoriz [] = {
  0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00
};
static Uint1  thinVert [] = {
  0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
};
static Uint1  thickHoriz [] = {
  0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00
};
static Uint1  thickVert [] = {
  0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33
};

#ifdef WIN_MAC
static Int1 userArrowSize = 10;
#endif
#ifdef WIN_MSWIN
static Int1 userArrowSize = 7;
#endif
#ifdef WIN_MOTIF
static Int1 userArrowSize = 10;
#endif

/*****************************************************************************
*
*   SetXXXFont (void)
*       Creates fonts of various sizes.
*
*****************************************************************************/

FonT SetSmallFont (void)

{
  if (smallFont == NULL) {
#ifdef WIN_MAC
    smallFont = ParseFont ("Monaco,9");
#endif
#ifdef WIN_MSWIN
    smallFont = ParseFont ("Courier,9");
#endif
#ifdef WIN_MOTIF
    smallFont = ParseFont ("Courier,12");
#endif
  }
  return smallFont;
}

FonT SetMediumFont (void)

{
  if (mediumFont == NULL) {
#ifdef WIN_MAC
    mediumFont = ParseFont ("Monaco,12");
#endif
#ifdef WIN_MSWIN
    mediumFont = ParseFont ("Courier,12");
#endif
#ifdef WIN_MOTIF
    mediumFont = ParseFont ("Courier,14");
#endif
  }
  return mediumFont;
}

FonT SetLargeFont (void)

{
  if (largeFont == NULL) {
#ifdef WIN_MAC
    largeFont = ParseFont ("Monaco,18");
#endif
#ifdef WIN_MSWIN
    largeFont = ParseFont ("Courier,18");
#endif
#ifdef WIN_MOTIF
    largeFont = ParseFont ("Courier,18");
#endif
  }
  return largeFont;
}

/*****************************************************************************
*
*   DISPATCHERS FOR GENERIC PRIMITIVE OPERATIONS
*
*****************************************************************************/

/*****************************************************************************
*
*   GetPrimDrawAttribute (primdrawcontect, ...)
*      helper function for PrimDrawProc
*
*****************************************************************************/

void Nlm_GetPrimDrawAttribute (PrimitivE prim, 
                           Uint1Ptr color, Int1Ptr plinestyle,
                           Int1Ptr pshading, Int1Ptr ppenwidth,
                           Int1Ptr pmode, Int1Ptr phighlight)
{
  AttPData* atts;

  if (prim != NULL ) {
    atts = &(((GenPPtr)prim)->att);
    if (color != NULL) {
      color[0] = atts->color[0];
      color[1] = atts->color[1];
      color[2] = atts->color[2];
    }
    if (plinestyle != NULL) *plinestyle = atts->linestyle;
    if (pshading != NULL) *pshading = atts->shading;
    if (ppenwidth != NULL) *ppenwidth = atts->penwidth;
    if (pmode != NULL) *pmode = atts->mode;
    if (phighlight != NULL) *phighlight = ((GenPPtr)prim)->highlight;
  }
}

/*****************************************************************************
*
*    HELPER FUNCTIONS
*
*****************************************************************************/
Nlm_BoxPtr Nlm_GetWorldWindow (Nlm_PrimDrawContext pdc) {
  if ( pdc != NULL ){
    return ( &(((DrawInfoPtr)pdc)->scale.worldWindow) );
  }
  return NULL;
}

Int4 Nlm_GetScaleX (PrimDrawContext pdc){
  if ( pdc != NULL ){
    return ( ((DrawInfoPtr)pdc)->scale.scaleX );
  }
  return 0;
}

Int4 Nlm_GetScaleY (Nlm_PrimDrawContext pdc){
  if ( pdc != NULL ){
    return ( ((DrawInfoPtr)pdc)->scale.scaleY );
  }
  return 0;
}

Int4 Nlm_GetOffsetX (PrimDrawContext pdc){
  if ( pdc != NULL ){
    return ( ((DrawInfoPtr)pdc)->scale.offsetX );
  }
  return 0;
}

Int4 Nlm_GetOffsetY (PrimDrawContext pdc){
  if ( pdc != NULL ){
    return ( ((DrawInfoPtr)pdc)->scale.offsetY );
  }
  return 0;
}

Boolean Nlm_isPrimInWindow (PrimDrawContext pdc){
  if ( pdc != NULL ){
    return ( ((DrawInfoPtr)pdc)->checked );
  }
  return FALSE;
}

Int1 Nlm_GetPrimHligh (PrimDrawContext pdc){
  if ( pdc != NULL ){
    return ( ((DrawInfoPtr)pdc)->highlight );
  }
  return PLAIN_SEGMENT;
}

BoxPtr Nlm_GetHitWindow (PrimHitContext phc){
  if ( phc != NULL ){
    return ( &(((ScalePtr)phc)->worldWindow) );
  }
  return NULL;
}

void Nlm_SetPrimAttribute (PrimDrawContext pdc, Nlm_Uint1 flags ){
  AttPPtr atts;

  if (pdc != NULL) {
    atts = ((DrawInfoPtr)pdc)->primattrib;
    if ( flags & COLOR_ATT ) {
      SelectColor (atts->color [0], atts->color [1], atts->color [2]);
    }
    if ( flags & WIDTH_ATT ){
      WidePen (atts->penwidth);
    }
    if ( flags & STYLE_ATT ) {
      switch (atts->linestyle) {
        case SOLID_LINE :
          Solid ();
          break;
        case DOTTED_LINE :
          Dotted ();
          break;
        case DASHED_LINE :
          Dashed ();
      }
    }
    if ( flags & SHADING_ATT ) {
      switch (atts->shading) {
        case SOLID_SHADING :
          Solid ();
          break;
        case DARK_SHADING :
          Dark ();
          break;
        case MEDIUM_SHADING :
          Medium ();
          break;
        case LIGHT_SHADING :
          Light ();
          break;
        case EMPTY_SHADING :
          Empty ();
          break;
        case THIN_HORIZ_SHADING :
          SetPenPattern (thinHoriz);
          break;
        case THIN_VERT_SHADING :
          SetPenPattern (thinVert);
          break;
        case THICK_HORIZ_SHADING :
          SetPenPattern (thickHoriz);
          break;
        case THICK_VERT_SHADING :
          SetPenPattern (thickVert);
          break;
        case THIN_NESW_SHADING :
          SetPenPattern (thinNews);
          break;
        case THIN_NWSE_SHADING :
          SetPenPattern (thinNwse);
          break;
        case THICK_NESW_SHADING :
          SetPenPattern (thickNews);
          break;
        case THICK_NWSE_SHADING :
          SetPenPattern (thickNwse);
      }
    }
    if ( flags & MODE_ATT ) {
      switch (atts->mode) {
        case COPY_MODE :
          CopyMode ();
          break;
        case MERGE_MODE :
          MergeMode ();
          break;
        case INVERT_MODE :
          InvertMode ();
          break;
        case ERASE_MODE :
          EraseMode ();
      }
    }
  }
}

void Nlm_RestorePrimAttribute (PrimDrawContext pdc){
  Nlm_SetPrimAttribute (pdc, (Uint1)0xFF);
}

/*****************************************************************************
*
*     OPERATIONS ON PRIMITIVES
*
*****************************************************************************/
/*****************************************************************************
*
*   Set Arrow Size
*
*****************************************************************************/
void Nlm_SetDefArrowSize ( Int1 arrowSize )
{
  if ( arrowSize < 4 ) arrowSize = 4;
  if ( arrowSize > 16 ) arrowSize = 16;
  userArrowSize = arrowSize;
}

static void MakeAlignRect ( Int2 x, Int2 y, Int2 width, Int2 height, 
                            Int2 offset, Int2 align, RectPtr rPtr )
{
  RecT r;

  switch (align) {
    case UPPER_CENTER :
      r.left = x - (width>>1); r.top = y - height - offset;
      break;
    case UPPER_RIGHT :
      r.left = x + offset; r.top = y - height - offset;
      break;
    case MIDDLE_LEFT :
      r.left = x - width - offset; r.top = y - (height>>1);
      break;
    case MIDDLE_CENTER :
      r.left = x - (width>>1); r.top = y - (height>>1);
      break;
    case MIDDLE_RIGHT :
      r.left = x + offset; r.top = y - (height>>1);
      break;
    case LOWER_LEFT :
      r.left = x - width - offset; r.top = y + offset;
      break;
    case LOWER_CENTER :
      r.left = x - (width>>1); r.top = y + offset;
      break;
    case LOWER_RIGHT :
      r.left = x + offset; r.top = y + offset;
      break;
    case UPPER_LEFT :
    default:
      r.left = x - width - offset; r.top = y - height - offset;
      break;
  }
  r.right = r.left + width; r.bottom = r.top + height;
  *rPtr = r;
}

static void MakeAlignBox ( Int4 x, Int4 y, Int4 width, Int4 height, 
                           Int4 offsetX, Int4 offsetY, Int2 align, 
                           BoxPtr boxPtr )
{
  BoxInfo box;

  switch (align) {
    case UPPER_CENTER :
      box.left = x - (width>>1); box.top = y + height + offsetY;
      break;
    case UPPER_RIGHT :
      box.left = x + offsetX; box.top = y + height + offsetY;
      break;
    case MIDDLE_LEFT :
      box.left = x - width - offsetX; box.top = y + (height>>1);
      break;
    case MIDDLE_CENTER :
      box.left = x - (width>>1); box.top = y + (height>>1);
      break;
    case MIDDLE_RIGHT :
      box.left = x + offsetX; box.top = y + (height>>1);
      break;
    case LOWER_LEFT :
      box.left = x - width - offsetX; box.top = y - offsetY;
      break;
    case LOWER_CENTER :
      box.left = x - (width>>1); box.top = y - offsetY;
      break;
    case LOWER_RIGHT :
      box.left = x + offsetX; box.top = y - offsetY;
      break;
    case UPPER_LEFT :
    default:
      box.left = x - width - offsetX; box.top = y + height + offsetY;
      break;
  }
  box.right = box.left + width; box.bottom = box.top - height;
  *boxPtr = box;
}

/*****************************************************************************
*
*   DrawPrimitive (item, scale)
*       Draws a primitive
*
*****************************************************************************/

void Nlm_DrawPrimitive (BasePPtr item, DrawInfoPtr drawinfo)
{
  PrimDefPtr pdp;

  if (item != NULL && drawinfo != NULL) {
    switch (item->code) {
      case GENERIC :
        pdp = ((GenPPtr)item)->pdp;
        if ( pdp != NULL && pdp->draw != NULL) {
          pdp->draw ( &(((GenPPtr)item)->data), (PrimDrawContext)drawinfo);
        }
        break;
      default :
        Message (MSG_ERROR, "DrawPrimitive item unknown");
        break;
    }
  }
}

/*****************************************************************************
*
*   PrimitiveIsCloseToPoint (item, &id)
*
*****************************************************************************/

Boolean Nlm_PrimitiveIsCloseToPoint (BasePPtr item, ScalePtr scale )
{
  PrimDefPtr pdp;

  if (item != NULL && scale != NULL) {
    switch (item->code) {
      case GENERIC :
        pdp = ((GenPPtr)item)->pdp;
        if ( pdp != NULL && pdp->hittest != NULL) {
          return pdp->hittest ( &(((GenPPtr)item)->data), 
                                (PrimHitContext)scale);
        }
        break;
      default :
        Message (MSG_ERROR, "PrimitiveIsCloseToPoint item unknown");
        break;
    }
  }
  return FALSE;
}

/*****************************************************************************
*
*   TryHighlightPrimitive (item, highlight)
*
*****************************************************************************/

Boolean Nlm_TryHighlightPrimitive (BasePPtr item, Int1 highlight)
{
  if (item != NULL) {
    switch (item->code) {
      case GENERIC :
        ((GenPPtr)item)->highlight = highlight;
        return TRUE;
      default:
        Message (MSG_ERROR, "TryHighlightPrimitive item unknown");
        break;
    }
  }
  return FALSE;
}

/*****************************************************************************
*
*   TryOffsetPrimitive (item, deltaX, deltaY)
*
*****************************************************************************/

Boolean Nlm_TryOffsetPrimitive (BasePPtr item, Int4 deltaX, Int4 deltaY)
{
  PrimDefPtr pdp;

  if (item != NULL) {
    switch (item->code) {
      case GENERIC :
        pdp = ((GenPPtr)item)->pdp;
        if ( pdp != NULL && pdp->offset != NULL) {
          return pdp->offset ( &(((GenPPtr)item)->data), deltaX, deltaY);
        }
        break;
      default:
        Message (MSG_ERROR, "TryOffsetPrimitive item unknown");
        break;
    }
  }
  return FALSE;
}

/*****************************************************************************
*
* TryGetPrimitiveLimits (item, scaleX, scaleY, pLimits)
*
*****************************************************************************/
Nlm_Boolean Nlm_TryGetPrimitiveLimits (Nlm_BasePPtr item, Nlm_Int4 scaleX, 
                                       Nlm_Int4 scaleY, Nlm_BoxPtr pLimits)
{
  PrimDefPtr pdp;

  if (item != NULL) {
    switch (item->code) {
      case GENERIC :
        pdp = ((GenPPtr)item)->pdp;
        if ( pdp != NULL && pdp->getlimits != NULL) {
          return pdp->getlimits ( &(((GenPPtr)item)->data), scaleX, scaleY,
                                  pLimits );
        }
        break;
      default:
        Message (MSG_ERROR, "TryOffsetPrimitive item unknown");
        break;
    }
  }
  return FALSE;
}

extern Nlm_Boolean Nlm_PrimitiveBox(Nlm_PrimitivE prim,
                                    Nlm_Int4 scaleX, Nlm_Int4 scaleY,
                                    Nlm_BoxPtr pBox) {
  return Nlm_TryGetPrimitiveLimits((Nlm_BasePPtr)prim, scaleX, scaleY, pBox);
}


/*****************************************************************************
*
*   CleanupPrimitive (item)
*       Deleted all data, owned by primitive
*
*****************************************************************************/

void Nlm_CleanupPrimitive (BasePPtr item)
{
  PrimDefPtr pdp;

  if (item != NULL) {
    switch (item->code) {
      case GENERIC :
        pdp = ((GenPPtr)item)->pdp;
        if ( pdp != NULL && pdp->cleanup != NULL) {
          pdp->cleanup ( &(((GenPPtr)item)->data) );
        }
        break;
      default:
        Message (MSG_ERROR, "CleanupPrimitive item unknown");
        break;
    }
  }
}

/*****************************************************************************
*
*     PRIMITIVES
*
*****************************************************************************/

/*****************************************************************************
*
*     INVISIBLE FRAME
*
*****************************************************************************/

typedef struct frampdata {
  BoxInfo  box;
} FramPData, PNTR FramPDataPtr;

static Boolean FramHitTestProc ( FramPDataPtr pdata, PrimHitContext phc)
{
  register ScalePtr dInfoPtr;
 
  dInfoPtr = (ScalePtr)phc;
  if ( (pdata->box.left < dInfoPtr->worldWindow.right) && 
       (pdata->box.right > dInfoPtr->worldWindow.left) &&
       (pdata->box.top > dInfoPtr->worldWindow.bottom) &&
       (pdata->box.bottom < dInfoPtr->worldWindow.top) ) return TRUE; 
  return FALSE; 
}

static Boolean FramOffsetProc ( FramPDataPtr pdata, Int4 deltaX, 
                                Int4 deltaY )
{
  pdata->box.left += deltaX;
  pdata->box.right += deltaX;
  pdata->box.top += deltaY;
  pdata->box.bottom += deltaY;
  return TRUE; 
}

static Boolean FramGetLimitsProc ( FramPDataPtr pdata, Int4 scaleX,
                                   Int4 scaleY, BoxPtr pLimits )
{
  *pLimits = pdata->box;
  return TRUE; 
}

static PrimDef framPrimDef = {
  (PrimDrawProc) NULL,
  (PrimHitTestProc) FramHitTestProc,
  (PrimOffsetProc) FramOffsetProc,
  (PrimCleanupProc) NULL,
  (PrimGetLimitsProc) FramGetLimitsProc
};

/*****************************************************************************
*
*   AddInvFrame (parent, left, top, right, bottom, primID)
*
*****************************************************************************/

PrimitivE AddInvFrame  (SegmenT parent, Int4 left, Int4 top, Int4 right,
                        Int4 bottom, Uint2 primID)
{
  FramPData data;
  Int4 swap;

  if (left > right) {
    swap = left;
    left = right;
    right = swap;
  }
  if (bottom > top) {
    swap = bottom;
    bottom = top;
    top = swap;
  }
  data.box.left = left;
  data.box.top = top;
  data.box.right = right;
  data.box.bottom = bottom;
  return AddPrimitive (&framPrimDef, parent, primID, (VoidPtr)&data,
                       sizeof(FramPData));
}


/*****************************************************************************
*
*     RECTANGLE
*
*****************************************************************************/

typedef struct recpdata {
  BoxInfo  box;
  Int2     arrow;
  Int1     arrowSize;
  Boolean  fill;
} RecPData, PNTR RecPDataPtr;


static void RecDrawProc (RecPDataPtr pdata, PrimDrawContext pdc)
{
  register DrawInfoPtr dInfoPtr = (DrawInfoPtr)pdc;
  BoxInfo tmpBox;
  RecT    r;
  Int4    curScale;
  PoinT   pts [3];
  Int1    arrowSize;
  Boolean vis = TRUE;
  RecT    rect;

  tmpBox = pdata->box;
  if ( (tmpBox.left   > dInfoPtr->scale.worldWindow.right ) || 
       (tmpBox.right  < dInfoPtr->scale.worldWindow.left  ) || 
       (tmpBox.top    < dInfoPtr->scale.worldWindow.bottom) || 
       (tmpBox.bottom > dInfoPtr->scale.worldWindow.top   ) )
    return;
  
  if ( dInfoPtr->checked == FALSE ) {
    if ( tmpBox.left < dInfoPtr->scale.worldWindow16.left ) 
      tmpBox.left = dInfoPtr->scale.worldWindow16.left;
    if ( tmpBox.right > dInfoPtr->scale.worldWindow16.right ) 
      tmpBox.right = dInfoPtr->scale.worldWindow16.right;
    if ( tmpBox.top > dInfoPtr->scale.worldWindow16.top ) 
      tmpBox.top = dInfoPtr->scale.worldWindow16.top;
    if ( tmpBox.bottom < dInfoPtr->scale.worldWindow16.bottom ) 
      tmpBox.bottom = dInfoPtr->scale.worldWindow16.bottom;
  }

  arrowSize = pdata->arrowSize;
  curScale = dInfoPtr->scale.scaleX;
  r.left   = (Int2)((dInfoPtr->scale.offsetX + tmpBox.left )  / curScale);
  r.right  = (Int2)((dInfoPtr->scale.offsetX + tmpBox.right)  / curScale);
  curScale = dInfoPtr->scale.scaleY;
  r.top    = (Int2)((dInfoPtr->scale.offsetY - tmpBox.top   ) / curScale);
  r.bottom = (Int2)((dInfoPtr->scale.offsetY - tmpBox.bottom) / curScale);

  /* calculate vertexes */
  rect = r;

  switch ( pdata->arrow )
    {
      Int2 oldPoint;

    case LEFT_ARROW :
      oldPoint = r.left;
      if (r.right - r.left > (Int2)arrowSize) r.left += (Int2)arrowSize;
      else r.left += (Int2)(arrowSize>>1); 
      pts[0].x = r.left;   pts[0].y = r.bottom + (Int2)2;
      pts[1].x = oldPoint; pts[1].y = (r.bottom + r.top)>>1;
      pts[2].x = r.left;   pts[2].y = r.top - (Int2)2;
      if (r.right < r.left)
	{
	  vis = FALSE;
	  rect.left  = pts[1].x;
	  rect.right = pts[0].x;
	}
      break;

    case RIGHT_ARROW :
      oldPoint = r.right;
      if (r.right - r.left >= arrowSize) r.right -= (Int2)arrowSize;
      else r.right -= (Int2)(arrowSize>>1);
      if ( r.right < r.left ) vis = FALSE;
      pts[0].x = r.right;  pts[0].y = r.bottom + (Int2)2;
      pts[1].x = oldPoint; pts[1].y = (r.bottom + r.top)>>1;
      pts[2].x = r.right;  pts[2].y = r.top - (Int2)2;
      if (r.right < r.left)
	{
	  vis = FALSE;
	  rect.left  = pts[0].x;
	  rect.right = pts[1].x;
	}
      break;

    case UP_ARROW :
      oldPoint = r.top;
      if (r.bottom - r.top >= (Int2)arrowSize) r.top += (Int2)arrowSize;
      else r.top += (Int2)(arrowSize>>1);
      if ( r.bottom < r.top ) vis = FALSE;
      pts[0].x = r.right + (Int2)2;   pts[0].y = r.top;
      pts[1].x = (r.right+r.left)>>1; pts[1].y = oldPoint;
      pts[2].x = r.left - (Int2)2;    pts[2].y = r.top;
      if (r.right < r.left)
	{
	  vis = FALSE;
	  rect.top    = pts[1].y;
	  rect.bottom = pts[0].y;
	}
      break;

    case DOWN_ARROW :
      oldPoint = r.bottom;
      if (r.bottom - r.top >= (Int2)arrowSize) r.bottom -= (Int2)arrowSize;
      else r.bottom -= (Int2)(arrowSize>>1);
      if ( r.bottom < r.top ) vis = FALSE;
      pts[0].x = r.right + (Int2)2;   pts[0].y = r.bottom;
      pts[1].x = (r.right+r.left)>>1; pts[1].y = oldPoint;
      pts[2].x = r.left - (Int2)2;    pts[2].y = r.bottom;
      if (r.right < r.left)
	{
	  vis = FALSE;
	  rect.top    = pts[0].y;
	  rect.bottom = pts[1].y;
	}
      break;
    }

  SetPrimAttribute (pdc, COLOR_ATT|SHADING_ATT|MODE_ATT);
  if ( !pdata->fill )
    SetPrimAttribute (pdc, WIDTH_ATT);

  /* draw arrow */
  if (pdata->arrow != NO_ARROW)
    {
      if (pdata->fill)
	PaintPoly (3, pts);
      else
	FramePoly (3, pts);

      if (dInfoPtr->highlight == FILL_PRIMITIVE)
	PaintPoly(3, pts);
    }

  /* draw rectangle */
  if ( vis ) {
    if ( pdata->fill )
      PaintRect( &r );
    else
      FrameRect( &r );
  }

  /* highlighting */
  switch ( dInfoPtr->highlight )
    {
    case PLAIN_PRIMITIVE:
      break;
    case FRAME_PRIMITIVE:
    case OUTLINE_PRIMITIVE:
      Black();
      WidePen( 1 );
      InsetRect(&rect, -3, -3);
      FrameRect( &rect );
      InsetRect(&rect, +3, +3);
      break;
    case FILL_PRIMITIVE:
      InsetRect(&rect, -2, -2);
      PaintRect( &rect );
      InsetRect(&rect, +2, +2);
      break;
    }
}


static Boolean RecHitTestProc ( RecPDataPtr pdata, PrimHitContext phc)
{
  register ScalePtr dInfoPtr;
 
  dInfoPtr = (ScalePtr)phc;
  if ( (pdata->box.left < dInfoPtr->worldWindow.right) &&
       (pdata->box.right > dInfoPtr->worldWindow.left) &&
       (pdata->box.top > dInfoPtr->worldWindow.bottom) &&
       (pdata->box.bottom < dInfoPtr->worldWindow.top) ) return TRUE; 
  return FALSE; 
}

static Boolean RecOffsetProc ( RecPDataPtr pdata, Int4 deltaX, 
                                   Int4 deltaY )
{
  pdata->box.left += deltaX;
  pdata->box.right += deltaX;
  pdata->box.top += deltaY;
  pdata->box.bottom += deltaY;
  return TRUE; 
}

static Boolean RecGetLimitsProc ( RecPDataPtr pdata, Int4 scaleX,
                                  Int4 scaleY, BoxPtr pLimits )
{
  Int4 arrowSize;

  *pLimits = pdata->box;
  switch ( pdata->arrow ){
    case LEFT_ARROW :
      arrowSize = pdata->arrowSize*scaleX/2;
      if ( pLimits->right - pLimits->left < arrowSize )
        pLimits->right = pLimits->left + arrowSize;
      break;
    case RIGHT_ARROW :
      arrowSize = pdata->arrowSize*scaleX/2;
      if ( pLimits->right - pLimits->left < arrowSize )
        pLimits->left = pLimits->right - arrowSize;
      break;
    case UP_ARROW :
      arrowSize = pdata->arrowSize*scaleY/2;
      if ( pLimits->top - pLimits->bottom < arrowSize )
        pLimits->bottom = pLimits->top - arrowSize;
      break;
    case DOWN_ARROW :
      arrowSize = pdata->arrowSize*scaleY/2;
      if ( pLimits->top - pLimits->bottom < arrowSize )
        pLimits->top = pLimits->bottom + arrowSize;
  }
  OutsetBox ( pLimits, scaleX<<2, scaleY<<2 );
  return TRUE; 
}

static PrimDef recPrimDef = {
  (PrimDrawProc) RecDrawProc,
  (PrimHitTestProc) RecHitTestProc,
  (PrimOffsetProc) RecOffsetProc,
  (PrimCleanupProc) NULL,
  (PrimGetLimitsProc) RecGetLimitsProc
};

/*****************************************************************************
*
*   AddRectangle (parent, left, top, right, bottom, arrow, fill, primID)
*
*****************************************************************************/

PrimitivE AddRectangle (SegmenT parent, Int4 left, Int4 top, Int4 right,
                        Int4 bottom, Int2 arrow, Boolean fill, Uint2 primID)
{
  RecPData data;
  Int4 swap;

  if (left > right) {
    swap = left;
    left = right;
    right = swap;
  }
  if (bottom > top) {
    swap = bottom;
    bottom = top;
    top = swap;
  }
  data.box.left = left;
  data.box.top = top;
  data.box.right = right;
  data.box.bottom = bottom;
  data.fill = fill;
  data.arrow = arrow;
  data.arrowSize = userArrowSize;
  return AddPrimitive (&recPrimDef, parent, primID, (VoidPtr)&data,
                       sizeof(RecPData));
}


/*****************************************************************************
*
*     ROUNDED RECTANGLE
*
*****************************************************************************/

typedef struct {
  BoxInfo box;
  Int4    oval_w;
  Int4    oval_h;
  Boolean fill;
  Uint4   flags;
} RoRecPData, PNTR RoRecPDataPtr;


static void s_DoRoundRect(RectPtr r,
                          Int2 oval_w, Int2 oval_h, Boolean fill, Uint4 flags)
{
  Int2 W, dW, H, dH;
  RecT r1, r2, qr1, qr2;
  EQuadrant q1, q2;
  PoinT p1, p2;

  if (!flags  ||
      (flags & ROREC_TOP   &&  flags & ROREC_BOTTOM)  ||
      (flags & ROREC_LEFT  &&  flags & ROREC_RIGHT)) {
    if ( fill )
      PaintRoundRect(r, oval_w, oval_h);
    else
      FrameRoundRect(r, oval_w, oval_h);
    return;
  }

  W = r->right - r->left;
  H = r->bottom - r->top;
  LoadRect(&r1, 0, 0, 0, 0);
  LoadRect(&r2, 0, 0, 0, 0);

  if (flags & ROREC_TOP  ||  flags & ROREC_BOTTOM) {
    if (2 * oval_w > W)
      oval_w = W / 2;
    dW = W - 2 * oval_w;

    if (oval_h > H)
      oval_h = H;
    dH = H - oval_h;
  }
  else { /* ROREC_LEFT || ROREC_RIGHT */
    if (2 * oval_h > H)
      oval_h = H / 2;
    dH = H - 2 * oval_h;

    if (oval_w > W)
      oval_w = W;
    dW = W - oval_w;
  }

  if (oval_w < 2  ||  oval_h < 2) {
    if ( fill )
      PaintRect(r);
    else
      FrameRect(r);
    return;
  }


  if (flags & ROREC_TOP  ||  flags & ROREC_BOTTOM) {
    if (flags & ROREC_TOP) {
      if ( dW )
        LoadRect(&r1, (Int2)(r->left + oval_w), r->top,
                 (Int2)(r->right - oval_w), (Int2)(r->top + oval_h));
      if ( dH )
        LoadRect(&r2, r->left, (Int2)(r->top + oval_h),
                 r->right, r->bottom);
      q1 = eQ_LeftTop;
      LoadRect(&qr1, r->left, r->top,
               (Int2)(r->left + oval_w), (Int2)(r->top + oval_h));
      q2 = eQ_RightTop;
      LoadRect(&qr2, (Int2)(r->right - oval_w), r->top,
               r->right, (Int2)(r->top + oval_h));

      if ( !fill ) {
        p1.y = p2.y = r->bottom;  p1.x = r->left;  p2.x = r->right;
        DrawLine(p1, p2);
        if ( dW ) {
          p1.y = p2.y = r1.top;  p1.x = r1.left;  p2.x = r1.right;
          DrawLine(p1, p2);
        }
      }
    }
    else { /* ROREC_BOTTOM */
      if ( dW )
        LoadRect(&r1, (Int2)(r->left + oval_w), (Int2)(r->bottom - oval_h),
                 (Int2)(r->right - oval_w), r->bottom);
      if ( dH )
        LoadRect(&r2, r->left, r->top,
                 r->right, (Int2)(r->bottom - oval_h));
      q1 = eQ_LeftBottom;
      LoadRect(&qr1, r->left, r->bottom,
               (Int2)(r->left + oval_w), (Int2)(r->bottom - oval_h));
      q2 = eQ_RightBottom;
      LoadRect(&qr2, (Int2)(r->right - oval_w), r->bottom,
               r->right, (Int2)(r->bottom - oval_h));

      if ( !fill ) {
        p1.y = p2.y = r->top;  p1.x = r->left;  p2.x = r->right;
        DrawLine(p1, p2);
        if ( dW ) {
          p1.y = p2.y = r->bottom;  p1.x = r1.left;  p2.x = r1.right;
          DrawLine(p1, p2);
        }
      }
    }

    if (!fill  &&  dH) {
      p1.y = r2.top;  p2.y = r2.bottom;

      p1.x = p2.x = r->left;
      DrawLine(p1, p2);
      p1.x = p2.x = r->right;
      DrawLine(p1, p2);
    }
  }

  else { /* ROREC_LEFT | ROREC_RIGHT */
    if (flags & ROREC_LEFT) {
      if ( dH )
        LoadRect(&r1, r->left, (Int2)(r->top + oval_h),
                 (Int2)(r->left + oval_w), (Int2)(r->bottom - oval_h));
      if ( dW )
        LoadRect(&r2, (Int2)(r->left + oval_w), r->top,
                 r->right, r->bottom);
      q1 = eQ_LeftTop;
      LoadRect(&qr1, r->left, r->top,
          (Int2)(r->left + oval_w), (Int2)(r->top + oval_h));
      q2 = eQ_LeftBottom;
      LoadRect(&qr2, r->left, r->bottom,
               (Int2)(r->left + oval_w), (Int2)(r->bottom - oval_h));
      
      if ( !fill ) {
        p1.x = p2.x = r->right;  p1.y = r->top;  p2.y = r->bottom;
        DrawLine(p1, p2);
        if ( dW ) {
          p1.x = p2.x = r->left;  p1.y = r1.top;  p2.y = r1.bottom;
          DrawLine(p1, p2);
        }
      }
    }
    else { /* ROREC_RIGHT */
      if ( dH )
        LoadRect(&r1, (Int2)(r->right - oval_w), (Int2)(r->top + oval_h),
                 r->right, (Int2)(r->bottom - oval_h));
      if ( dW )
        LoadRect(&r2, r->left, r->top,
                 (Int2)(r->right - oval_w), r->bottom);
      q1 = eQ_RightTop;
      LoadRect(&qr1, (Int2)(r->right - oval_w), r->top,
               r->right, (Int2)(r->top + oval_h));
      q2 = eQ_RightBottom;
      LoadRect(&qr2, (Int2)(r->right - oval_w), r->bottom,
               r->right, (Int2)(r->bottom - oval_h));
      
      if ( !fill ) {
        p1.x = p2.x = r->left;  p1.y = r->top;  p2.y = r->bottom;
        DrawLine(p1, p2);
        if ( dW ) {
          p1.x = p2.x = r->right;  p1.y = r1.top;  p2.y = r1.bottom;
          DrawLine(p1, p2);
        }
      }
    }

    if (!fill  &&  dH) {
      p1.x = r2.left;  p2.x = r2.right;

      p1.y = p2.y = r->top;
      DrawLine(p1, p2);
      p1.y = p2.y = r->bottom;
      DrawLine(p1, p2);
    }
  }

  if ( fill ) {
    if ( !EmptyRect(&r1) )
      PaintRect(&r1);
    if ( !EmptyRect(&r2) )
      PaintRect(&r2);
    PaintQuadrant(&qr1, q1);
    PaintQuadrant(&qr2, q2);
  } else {
    FrameQuadrant(&qr1, q1);
    FrameQuadrant(&qr2, q2);
  }
}


static void RoRecDrawProc(RoRecPDataPtr pdata, PrimDrawContext pdc)
{
  register DrawInfoPtr dInfoPtr = (DrawInfoPtr)pdc;
  BoxInfo tmpBox;
  RecT    r;
  Int4    curScale;
  Int2    oval_w, oval_h;

  /* check limits */
  tmpBox = pdata->box;
  if ( (tmpBox.left   > dInfoPtr->scale.worldWindow.right ) || 
       (tmpBox.right  < dInfoPtr->scale.worldWindow.left  ) || 
       (tmpBox.top    < dInfoPtr->scale.worldWindow.bottom) || 
       (tmpBox.bottom > dInfoPtr->scale.worldWindow.top   ) )
    return;
  
  if ( dInfoPtr->checked == FALSE ) {
    if ( tmpBox.left < dInfoPtr->scale.worldWindow16.left ) 
      tmpBox.left = dInfoPtr->scale.worldWindow16.left;
    if ( tmpBox.right > dInfoPtr->scale.worldWindow16.right ) 
      tmpBox.right = dInfoPtr->scale.worldWindow16.right;
    if ( tmpBox.top > dInfoPtr->scale.worldWindow16.top ) 
      tmpBox.top = dInfoPtr->scale.worldWindow16.top;
    if ( tmpBox.bottom < dInfoPtr->scale.worldWindow16.bottom ) 
      tmpBox.bottom = dInfoPtr->scale.worldWindow16.bottom;
  }

  /* scale */
  curScale = dInfoPtr->scale.scaleX;
  r.left   = (Int2)((dInfoPtr->scale.offsetX + tmpBox.left )  / curScale);
  r.right  = (Int2)((dInfoPtr->scale.offsetX + tmpBox.right)  / curScale);
  oval_w   = (Int2)(pdata->oval_w / curScale);

  curScale = dInfoPtr->scale.scaleY;
  r.top    = (Int2)((dInfoPtr->scale.offsetY - tmpBox.top   ) / curScale);
  r.bottom = (Int2)((dInfoPtr->scale.offsetY - tmpBox.bottom) / curScale);
  oval_h   = (Int2)(pdata->oval_h / curScale);

  /* attributes */
  SetPrimAttribute(pdc, COLOR_ATT|SHADING_ATT|MODE_ATT);
  if ( !pdata->fill )
    SetPrimAttribute(pdc, WIDTH_ATT);

  /* draw */
  s_DoRoundRect(&r, oval_w, oval_h, pdata->fill, pdata->flags);

  /* highlight */
  switch ( dInfoPtr->highlight ) {
  case PLAIN_PRIMITIVE:
    break;
  case FRAME_PRIMITIVE:
  case OUTLINE_PRIMITIVE:
    Black();
    WidePen( 1 );
    InsetRect(&r, -3, -3);
    s_DoRoundRect(&r, (Int2)(oval_w + 3), (Int2)(oval_h + 3),
                  FALSE, pdata->flags);
    break;
  case FILL_PRIMITIVE:
    InsetRect(&r, -2, -2);
    s_DoRoundRect(&r, (Int2)(oval_w + 3), (Int2)(oval_h + 3),
                  TRUE, pdata->flags);
    break;
  }
}


static Boolean RoRecHitTestProc(RoRecPDataPtr pdata, PrimHitContext phc)
{
  ScalePtr dInfoPtr = (ScalePtr)phc;

  return (Boolean)
    ((pdata->box.left < dInfoPtr->worldWindow.right) &&
     (pdata->box.right > dInfoPtr->worldWindow.left) &&
     (pdata->box.top > dInfoPtr->worldWindow.bottom) &&
     (pdata->box.bottom < dInfoPtr->worldWindow.top));
}

static Boolean RoRecOffsetProc(RoRecPDataPtr pdata, Int4 deltaX, Int4 deltaY)
{
  pdata->box.left   += deltaX;
  pdata->box.right  += deltaX;
  pdata->box.top    += deltaY;
  pdata->box.bottom += deltaY;
  return TRUE; 
}

static Boolean RoRecGetLimitsProc(RoRecPDataPtr pdata,
                                  Int4 scaleX, Int4 scaleY, BoxPtr pLimits )
{
  *pLimits = pdata->box;
  OutsetBox(pLimits, scaleX<<2, scaleY<<2);
  return TRUE; 
}

static PrimDef rorecPrimDef = {
  (PrimDrawProc     ) RoRecDrawProc,
  (PrimHitTestProc  ) RoRecHitTestProc,
  (PrimOffsetProc   ) RoRecOffsetProc,
  (PrimCleanupProc  ) 0,
  (PrimGetLimitsProc) RoRecGetLimitsProc
};


/*****************************************************************************
*
*   AddRoundedRectangle
*
*****************************************************************************/

PrimitivE AddRoundedRectangle(SegmenT parent,
                              Int4 left, Int4 top, Int4 right, Int4 bottom,
                              Int4 oval_w, Int4 oval_h, Boolean fill,
                              Uint4 flags, Uint2 primID)
{
  RoRecPData data;
  Int4 tmp;

  if (left > right) {
    tmp = left;
    left = right;
    right = tmp;
  }
  if (bottom > top) {
    tmp = bottom;
    bottom = top;
    top = tmp;
  }

  data.box.left   = left;
  data.box.top    = top;
  data.box.right  = right;
  data.box.bottom = bottom;

  data.flags = flags;

  data.oval_w = oval_w;
  data.oval_h = oval_h;

  data.fill = fill;
  return AddPrimitive(&rorecPrimDef, parent, primID, (VoidPtr)&data,
                       sizeof(RoRecPData));
}


/*****************************************************************************
*
*   SEGMENT  RECTANGLE
*
*****************************************************************************/

typedef struct segrecpdata {
  BoxPtr   parentBox;
  Boolean  fill;
} SegRecPData, PNTR SegRecPDataPtr;

static void SegRecDrawProc (SegRecPDataPtr pdata, PrimDrawContext pdc)
{
  register DrawInfoPtr dInfoPtr;
  BoxInfo tmpBox;
  Int4    curScale;
  RecT    r;
 
  dInfoPtr = (DrawInfoPtr)pdc;
  tmpBox = *(pdata->parentBox);
  if ( (tmpBox.left > dInfoPtr->scale.worldWindow.right) || 
       (tmpBox.right < dInfoPtr->scale.worldWindow.left) || 
       (tmpBox.top < dInfoPtr->scale.worldWindow.bottom) || 
       (tmpBox.bottom > dInfoPtr->scale.worldWindow.top) ) return; 
  if ( dInfoPtr->checked == FALSE ) {
    if ( tmpBox.left < dInfoPtr->scale.worldWindow16.left ) 
      tmpBox.left = dInfoPtr->scale.worldWindow16.left;
    if ( tmpBox.right > dInfoPtr->scale.worldWindow16.right ) 
      tmpBox.right = dInfoPtr->scale.worldWindow16.right;
    if ( tmpBox.top > dInfoPtr->scale.worldWindow16.top ) 
      tmpBox.top = dInfoPtr->scale.worldWindow16.top;
    if ( tmpBox.bottom < dInfoPtr->scale.worldWindow16.bottom ) 
      tmpBox.bottom = dInfoPtr->scale.worldWindow16.bottom;
  }
  if (pdata->fill) {
    SetPrimAttribute (pdc, COLOR_ATT|SHADING_ATT|MODE_ATT);
  } else {
    SetPrimAttribute (pdc, COLOR_ATT|STYLE_ATT|WIDTH_ATT|MODE_ATT);
  }
  curScale = dInfoPtr->scale.scaleX;
  r.left = (Int2)((dInfoPtr->scale.offsetX + tmpBox.left) / curScale);
  r.right = (Int2)((dInfoPtr->scale.offsetX + tmpBox.right) / curScale);
  curScale = dInfoPtr->scale.scaleY;
  r.top = (Int2)((dInfoPtr->scale.offsetY - tmpBox.top) / curScale);
  r.bottom = (Int2)((dInfoPtr->scale.offsetY - tmpBox.bottom) / curScale);
  r.right++;  r.bottom++;
  if ( pdata->fill )  PaintRect (&r);
  else FrameRect (&r);
  r.right--; r.bottom--;
  switch ( dInfoPtr->highlight ) {
    case FRAME_PRIMITIVE:
    case OUTLINE_PRIMITIVE:
      SetPrimAttribute (pdc, STYLE_ATT|WIDTH_ATT);
      WidePen((Int2)(dInfoPtr->primattrib->penwidth + 1));
      r.right ++;  r.bottom ++;
      FrameRect (&r);
      InsetRect (&r, -1, -1 );
      FrameRect (&r);
      break;
    case FILL_PRIMITIVE:
      SetPrimAttribute (pdc, SHADING_ATT);
      r.right ++;  r.bottom ++;
      InsetRect (&r, -1, -1);
      PaintRect (&r);
  }
}

static Boolean SegRecHitTestProc ( SegRecPDataPtr pdata, PrimHitContext phc)
{
  register ScalePtr dInfoPtr;
  BoxInfo tmpBox;
 

  
  dInfoPtr = (ScalePtr)phc;
  tmpBox = *(pdata->parentBox);
  if ( (tmpBox.left < dInfoPtr->worldWindow.right) && 
       (tmpBox.right > dInfoPtr->worldWindow.left) &&
       (tmpBox.top > dInfoPtr->worldWindow.bottom) &&
       (tmpBox.bottom < dInfoPtr->worldWindow.top) ) return TRUE;
  return FALSE;
}

static Boolean SegRecOffsetProc ( SegRecPDataPtr pdata, Int4 deltaX, 
                                   Int4 deltaY )
{ 
  return FALSE; 
}

static Boolean SegRecGetLimitsProc ( SegRecPDataPtr pdata, Int4 scaleX,
Int4 scaleY, BoxPtr pLimits )
{
  *pLimits = *(pdata->parentBox);
  return TRUE; 
}

static PrimDef segrecPrimDef = {
  (PrimDrawProc) SegRecDrawProc,
  (PrimHitTestProc) SegRecHitTestProc,
  (PrimOffsetProc) SegRecOffsetProc,
  (PrimCleanupProc) NULL,
  (PrimGetLimitsProc) SegRecGetLimitsProc
};

Nlm_PrimitivE Nlm_AddSegRect ( SegmenT parent, Boolean fill, 
                              Uint2 primID )
{
  SegRecPData data;

  data.parentBox = &(((SegPPtr)parent)->seg.box);
  data.fill  = fill;
  return AddPrimitive (&segrecPrimDef, parent, primID, (VoidPtr)&data,
                       sizeof(SegRecPData));
}

static Boolean SilentSegRecHitTestProc ( SegRecPDataPtr pdata, PrimHitContext phc)
{
/* SilentSegRect's should never absorb clicks */
  return FALSE;
}

static PrimDef silentSegrecPrimDef = {
  (PrimDrawProc) SegRecDrawProc,
  (PrimHitTestProc) SilentSegRecHitTestProc,
  (PrimOffsetProc) SegRecOffsetProc,
  (PrimCleanupProc) NULL,
  (PrimGetLimitsProc) SegRecGetLimitsProc
};

Nlm_PrimitivE Nlm_AddSilentSegRect ( SegmenT parent, Boolean fill, 
                              Uint2 primID )
{
  SegRecPData data;

  data.parentBox = &(((SegPPtr)parent)->seg.box);
  data.fill  = fill;
  return AddPrimitive (&silentSegrecPrimDef, parent, primID, (VoidPtr)&data,
                       sizeof(SegRecPData));
}

/*****************************************************************************
*
*  H - LINE
*
*****************************************************************************/
typedef struct hlinepdata {
  Int4     x1;
  Int4     x2;
  Int4     y;
  Int1     fArrowPoint;
  Int1     arrowSize;
} HLinePData, PNTR HLinePDataPtr;

static void HLineDrawProc (HLinePDataPtr pdata, PrimDrawContext pdc)
{
  register DrawInfoPtr dInfoPtr;
  Int4    curScale;
  Int4    x1,x2,y;
  PoinT   pt1, pt2;
  PoinT   pts [3];
  Int2    oldPoint;
  Int1    arrowSize;
  Boolean vis = TRUE;
  RecT    rect;
  
  dInfoPtr = (DrawInfoPtr)pdc;
  x1 = pdata->x1;
  x2 = pdata->x2;
  y = pdata->y;
  if ( (y  > dInfoPtr->scale.worldWindow.top   ) ||
       (y  < dInfoPtr->scale.worldWindow.bottom) ||
       (x1 > dInfoPtr->scale.worldWindow.right ) ||
       (x2 < dInfoPtr->scale.worldWindow.left  ) )
    return;

  if ( dInfoPtr->checked == FALSE ) {
    curScale = dInfoPtr->scale.worldWindow16.left;
    if ( x1 < curScale ) x1 = curScale;
    curScale = dInfoPtr->scale.worldWindow16.right;
    if ( x2 > curScale ) x2 = curScale;
  }

  SetPrimAttribute (pdc, COLOR_ATT|STYLE_ATT|WIDTH_ATT|MODE_ATT);

  arrowSize = pdata->arrowSize;
  curScale = dInfoPtr->scale.scaleX;
  pt1.x = (Int2)((dInfoPtr->scale.offsetX + x1) / curScale);
  pt2.x = (Int2)((dInfoPtr->scale.offsetX + x2) / curScale);
  curScale = dInfoPtr->scale.scaleY;
  pt1.y = pt2.y = (Int2)((dInfoPtr->scale.offsetY - y) / curScale);

  if (dInfoPtr->highlight != PLAIN_PRIMITIVE)
    {
      rect.left  = pt1.x;
      rect.right = pt2.x;
      rect.top   = rect.bottom =  pt1.y;
    }

  switch ( pdata->fArrowPoint ) {
  case 0:
    oldPoint = pt2.x;
    if (pt2.x - pt1.x > (Int2)arrowSize) pt2.x -= (Int2)arrowSize;
    else pt2.x -= (Int2)(arrowSize>>1);
    if ( pt2.x < pt1.x ) vis = FALSE;
    pts[1].x = oldPoint; pts[1].y = pt1.y;
    oldPoint = (Int2)ARROWWIDTH2 * (Int2)arrowSize / (Int2)100;
    pts[0].x = pt2.x; pts[0].y = pt1.y + oldPoint;
    pts[2].x = pt2.x; pts[2].y = pt2.y - oldPoint ;
    break;
  case 1:
    oldPoint = pt1.x;
    if (pt2.x - pt1.x > (Int2)arrowSize) pt1.x += (Int2)arrowSize;
    else pt1.x += (Int2)(arrowSize>>1);
    if ( pt2.x < pt1.x ) vis = FALSE;
    pts[1].x = oldPoint; pts[1].y = pt1.y;
    oldPoint = (Int2)ARROWWIDTH2 * (Int2)arrowSize / (Int2)100;
    pts[0].x = pt1.x; pts[0].y = pt1.y - oldPoint;
    pts[2].x = pt1.x; pts[2].y = pt2.y + oldPoint ;
  }
  
  if ( vis ) DrawLine (pt1, pt2);
  if ( pdata->fArrowPoint != 0xF ){
    SetPrimAttribute (pdc, SHADING_ATT);
    PaintPoly (3, pts);
  }

  switch ( dInfoPtr->highlight )
    {
    case PLAIN_PRIMITIVE:
      break;
    case FRAME_PRIMITIVE:
    case OUTLINE_PRIMITIVE:
      Black();
      WidePen( 1 );
      InsetRect(&rect, -2, -2);
      rect.bottom++;
      FrameRect( &rect );
      break;
    case FILL_PRIMITIVE:
      SetPrimAttribute (pdc, STYLE_ATT|WIDTH_ATT);
      WidePen((Int2)(((DrawInfoPtr)pdc)->primattrib->penwidth + 2));
      if ( vis ) DrawLine (pt1, pt2);
      if ( pdata->fArrowPoint != 0xF ){
        FramePoly (3, pts);
        SetPrimAttribute (pdc, SHADING_ATT);
        PaintPoly (3, pts);
      }
      break;
    }
}


static Boolean HLineHitTestProc ( HLinePDataPtr pdata, PrimHitContext phc)
{
  register ScalePtr dInfoPtr;
 
  dInfoPtr = (ScalePtr)phc;
  if ( (pdata->x1 < dInfoPtr->worldWindow.right) && 
       (pdata->x2 > dInfoPtr->worldWindow.left) &&
       (pdata->y > dInfoPtr->worldWindow.bottom) &&
       (pdata->y < dInfoPtr->worldWindow.top) ) return TRUE;
  return FALSE; 
}

static Boolean HLineOffsetProc ( HLinePDataPtr pdata, Int4 deltaX, 
                                     Int4 deltaY )
{
  pdata->x1 += deltaX;
  pdata->x2 += deltaX;
  pdata->y += deltaY;
  return TRUE; 
}

static Boolean HLineGetLimitsProc ( HLinePDataPtr pdata, Int4 scaleX,
Int4 scaleY, BoxPtr pLimits )
{
  Int4 arrowSize;

  switch ( pdata->fArrowPoint ) {
    case 0:
      arrowSize = pdata->arrowSize*scaleX/2;
      if ( pdata->x2 - pdata->x1 < arrowSize ) 
        pLimits->left = pdata->x2 - arrowSize;
      else pLimits->left = pdata->x1;
      pLimits->right = pdata->x2;
      pLimits->top = pdata->y + arrowSize;
      pLimits->bottom = pdata->y - arrowSize;
      break;
    case 1:
      arrowSize = pdata->arrowSize*scaleX/2;
      if ( pdata->x2 - pdata->x1 < arrowSize ) 
        pLimits->right = pdata->x2 - arrowSize;
      else pLimits->right = pdata->x2;
      pLimits->left = pdata->x1;
      pLimits->top = pdata->y + arrowSize;
      pLimits->bottom = pdata->y - arrowSize;
      break;
    default:
      pLimits->left = pdata->x1;
      pLimits->right = pdata->x2;
      pLimits->top = pLimits->bottom = pdata->y;
  }
  OutsetBox ( pLimits, scaleX<<2, scaleY<<2 );
  return TRUE; 
}

static PrimDef hlinePrimDef = {
  (PrimDrawProc) HLineDrawProc,
  (PrimHitTestProc) HLineHitTestProc,
  (PrimOffsetProc) HLineOffsetProc,
  (PrimCleanupProc) NULL,
  (PrimGetLimitsProc) HLineGetLimitsProc
};

/*****************************************************************************
*
*   AddHLine (parent, x1, x2, y, arrow, primID)
*
*****************************************************************************/

static PrimitivE Nlm_AddHLine ( SegmenT parent, Int4 pnt1X, Int4 pnt2X, 
                                Int4 pntY, Boolean arrow, Uint2 primID )
{
  HLinePData data;

  if ( pnt1X == pnt2X ) arrow = FALSE;
  data.y = pntY;
  data.arrowSize = userArrowSize;
  if ( pnt2X >= pnt1X ) {
    data.x1 = pnt1X;
    data.x2 = pnt2X;
    data.fArrowPoint = 0;
  } else {
    data.x1 = pnt2X;
    data.x2 = pnt1X;
    data.fArrowPoint = 1;
  }
  if ( !arrow ) data.fArrowPoint = (Int1)0xF;
  return AddPrimitive (&hlinePrimDef, parent, primID, (VoidPtr)&data,
                       sizeof(HLinePData));
}

/*****************************************************************************
*
*  V - LINE
*
*****************************************************************************/
typedef struct vlinepdata {
  Int4     y1;
  Int4     y2;
  Int4     x;
  Int1     fArrowPoint;
  Int1     arrowSize;
} VLinePData, PNTR VLinePDataPtr;

static void VLineDrawProc (VLinePDataPtr pdata, PrimDrawContext pdc)
{
  register DrawInfoPtr dInfoPtr;
  Int4    curScale;
  Int4    y1,y2,x;
  PoinT   pt1, pt2;
  PoinT   pts [3];
  Int2    oldPoint;
  Int1    arrowSize;
  Boolean vis = TRUE;
 
  dInfoPtr = (DrawInfoPtr)pdc;
  y1 = pdata->y1;
  y2 = pdata->y2;
  x = pdata->x;
  if ( (y1 > dInfoPtr->scale.worldWindow.top) ||
       (y2 < dInfoPtr->scale.worldWindow.bottom) ||
       (x > dInfoPtr->scale.worldWindow.right) ||
       (x < dInfoPtr->scale.worldWindow.left) ) return;
  if ( dInfoPtr->checked == FALSE ) {
    curScale = dInfoPtr->scale.worldWindow16.bottom;
    if ( y1 < curScale ) y1 = curScale;
    curScale = dInfoPtr->scale.worldWindow16.top;
    if ( y2 > curScale ) y2 = curScale;
  }
  SetPrimAttribute (pdc, COLOR_ATT|STYLE_ATT|WIDTH_ATT|MODE_ATT);
  arrowSize = pdata->arrowSize;
  curScale = dInfoPtr->scale.scaleY;
  pt1.y = (Int2)((dInfoPtr->scale.offsetY - y1) / curScale);
  pt2.y = (Int2)((dInfoPtr->scale.offsetY - y2) / curScale);
  curScale = dInfoPtr->scale.scaleX;
  pt1.x = pt2.x = (Int2)((dInfoPtr->scale.offsetX + x) / curScale);
  switch ( pdata->fArrowPoint ) {
    case 0:
      oldPoint = pt2.y;
      if (pt1.y - pt2.y > (Int2)arrowSize) pt2.y += (Int2)arrowSize;
      else pt2.y += (Int2)(arrowSize>>1);
      if ( pt2.y > pt1.y ) vis = FALSE;
      pts[1].x = pt1.x; pts[1].y = oldPoint;
      oldPoint = (Int2)ARROWWIDTH2 * (Int2)arrowSize / (Int2)100;
      pts[0].x = pt1.x - oldPoint; pts[0].y = pt2.y;
      pts[2].x = pt1.x + oldPoint; pts[2].y = pt2.y;
      break;
    case 1:
      oldPoint = pt1.y;
      if (pt1.y - pt2.y > (Int2)arrowSize) pt1.y -= (Int2)arrowSize;
      else pt1.y -= (Int2)(arrowSize>>1);
      if ( pt2.y > pt1.y ) vis = FALSE;
      pts[1].x = pt1.x; pts[1].y = oldPoint;
      oldPoint = (Int2)ARROWWIDTH2 * (Int2)arrowSize / (Int2)100;
      pts[0].x = pt1.x + oldPoint; pts[0].y = pt1.y;
      pts[2].x = pt1.x - oldPoint; pts[2].y = pt1.y;
  }
  if ( vis ) DrawLine (pt1, pt2);
  if ( pdata->fArrowPoint != 0xF ){
    FramePoly (3, pts);
    SetPrimAttribute (pdc, SHADING_ATT);
    PaintPoly (3, pts);
  }
  if ( dInfoPtr->highlight != PLAIN_SEGMENT ) {
    SetPrimAttribute (pdc, STYLE_ATT|WIDTH_ATT);
    WidePen((Int2)(((DrawInfoPtr)pdc)->primattrib->penwidth + 2));
    if ( vis )  DrawLine (pt1, pt2);
    if ( pdata->fArrowPoint != 0xF ){
      SetPrimAttribute (pdc, SHADING_ATT);
      PaintPoly (3, pts);
    }
  }
}

static Boolean VLineHitTestProc ( VLinePDataPtr pdata, PrimHitContext phc)
{
  register ScalePtr dInfoPtr;
 
  dInfoPtr = (ScalePtr)phc;
  if ( (pdata->x < dInfoPtr->worldWindow.right) && 
       (pdata->x > dInfoPtr->worldWindow.left) &&
       (pdata->y1 < dInfoPtr->worldWindow.top) &&
       (pdata->y2 > dInfoPtr->worldWindow.bottom) ) return TRUE;
  return FALSE; 
}

static Boolean VLineOffsetProc ( VLinePDataPtr pdata, Int4 deltaX, 
                                     Int4 deltaY )
{
  pdata->x += deltaX;
  pdata->y1 += deltaY;
  pdata->y2 += deltaY;
  return TRUE; 
}

static Boolean VLineGetLimitsProc ( VLinePDataPtr pdata, Int4 scaleX,
Int4 scaleY, BoxPtr pLimits )
{
  Int4 arrowSize;

  switch ( pdata->fArrowPoint ) {
    case 0:
      arrowSize = pdata->arrowSize*scaleY/2;
      if ( pdata->y2 - pdata->y1 < arrowSize ) 
        pLimits->bottom = pdata->y2 + arrowSize;
      else pLimits->bottom = pdata->y1;
      pLimits->top = pdata->y2;
      pLimits->right = pdata->x + arrowSize;
      pLimits->left = pdata->x - arrowSize;
      break;
    case 1:
      arrowSize = pdata->arrowSize*scaleY/2;
      if ( pdata->y2 - pdata->y1 < arrowSize ) 
        pLimits->top = pdata->y1 - arrowSize;
      else pLimits->top = pdata->y2;
      pLimits->bottom = pdata->y1;
      pLimits->right = pdata->x + arrowSize;
      pLimits->left = pdata->x - arrowSize;
      break;
    default:
      pLimits->top = pdata->y2;
      pLimits->bottom = pdata->y1;
      pLimits->left = pLimits->right = pdata->x;
  }
  OutsetBox ( pLimits, scaleX<<2, scaleY<<2 );
  return TRUE; 
}

static PrimDef vlinePrimDef = {
  (PrimDrawProc) VLineDrawProc,
  (PrimHitTestProc) VLineHitTestProc,
  (PrimOffsetProc) VLineOffsetProc,
  (PrimCleanupProc) NULL,
  (PrimGetLimitsProc) VLineGetLimitsProc
};

/*****************************************************************************
*
*   AddVLine (parent, x, y1, y2, arrow, primID)
*
*****************************************************************************/

static PrimitivE Nlm_AddVLine ( SegmenT parent, Int4 pntX, Int4 pnt1Y, 
                                Int4 pnt2Y, Boolean arrow, Uint2 primID )
{
  VLinePData data;

  if ( pnt1Y == pnt2Y ) arrow = FALSE;
  data.x = pntX;
  data.arrowSize = userArrowSize;
  if ( pnt2Y >= pnt1Y ) {
    data.y1 = pnt1Y;
    data.y2 = pnt2Y;
    data.fArrowPoint = 0;
  } else {
    data.y1 = pnt2Y;
    data.y2 = pnt1Y;
    data.fArrowPoint = 1;
  }
  if ( !arrow ) data.fArrowPoint = (Int1)0xF;
  return AddPrimitive (&vlinePrimDef, parent, primID, (VoidPtr)&data,
                       sizeof(VLinePData));
}

/*****************************************************************************
*
*     LINE
*
*****************************************************************************/

typedef struct linepdata {
  BoxInfo  box;
  Int1     fPoint;
  Int1     fArrowPoint;
  Int1     arrowSize;
} LinePData, PNTR LinePDataPtr;

static Int2 MSin(Int4 a)
{
  if (a < 0)
    a += INT4_MAX / 360 * 360;
  a %= 360;

  if (a < 90)
    return sin100[a];
  if (a < 180)
    return sin100[180-a];
  if (a < 270)
    return -sin100[a-180];
  return -sin100[360-a];
}

/* (for future use)
static Int2 MCos(Int4 a)
{
  if (a < 0)
    a += INT4_MAX / 360 * 360;
  a %= 360;

  if (a < 90)
    return sin100[90 - a];
  if (a < 180)
    return -sin100[a - 90];
  if (a < 270)
    return -sin100[270 - a];
  return sin100[a-270];
}
*/

static void LineDrawProc (LinePDataPtr pdata, PrimDrawContext pdc)
{
  register DrawInfoPtr dInfoPtr;
  Int4    curScale;
  Int4    x1,y1,x2,y2;
  PoinT   pt1, pt2;
  PoinT   pts [3];
  Int2    angle;
  Int2    dangle = 20;
  Int2    arrowSize;
  Int2    arrowLen = 106;
  Int2    dx, dy;
  Boolean vis = TRUE;
 
  dInfoPtr = (DrawInfoPtr)pdc;
  arrowSize = (Int2)pdata->arrowSize;
  x1 = pdata->box.left;
  x2 = pdata->box.right;
  if ( pdata->fPoint == 0 ) {
    y1 = pdata->box.bottom;
    y2 = pdata->box.top;
  } else {
    y1 = pdata->box.top;
    y2 = pdata->box.bottom;
  }
  if ( pdata->fArrowPoint == 0xF ) {
    if ( (x1 > dInfoPtr->scale.worldWindow.right) ||
         (x2 < dInfoPtr->scale.worldWindow.left) ||
         (pdata->box.bottom > dInfoPtr->scale.worldWindow.top) ||
         (pdata->box.top < dInfoPtr->scale.worldWindow.bottom) ) return;
  } else {
    curScale = arrowSize * dInfoPtr->scale.scaleX / 2;
    if ( ((x1+curScale) > dInfoPtr->scale.worldWindow.right) ||
         ((x2-curScale) < dInfoPtr->scale.worldWindow.left) ) return;
    curScale = arrowSize * dInfoPtr->scale.scaleY / 2;
    if ( ((pdata->box.bottom+curScale) > dInfoPtr->scale.worldWindow.top) ||
         ((pdata->box.top+curScale) < dInfoPtr->scale.worldWindow.bottom) ) 
         return;
  }
  if ( dInfoPtr->checked == FALSE ) {
    if ( LineIntoVPort ( &x1, &y1, &x2, &y2,
         &(dInfoPtr->scale.worldWindow16)) == FALSE ) return;
  }
  SetPrimAttribute (pdc, COLOR_ATT|STYLE_ATT|WIDTH_ATT|MODE_ATT);
  curScale = dInfoPtr->scale.scaleX;
  pt1.x = (Int2)((dInfoPtr->scale.offsetX + x1) / curScale);
  pt2.x = (Int2)((dInfoPtr->scale.offsetX + x2) / curScale);
  curScale = dInfoPtr->scale.scaleY;
  pt1.y = (Int2)((dInfoPtr->scale.offsetY - y1) / curScale);
  pt2.y = (Int2)((dInfoPtr->scale.offsetY - y2) / curScale);
  if ( pdata->fArrowPoint != 0xF ){
    dx = pt2.x - pt1.x;
    dy = pt2.y - pt1.y; if ( dy < 0 ) dy = -dy;
    if ( (dx < 4) || ( dy < 4 ) ){
      x2 -= x1;
      y2 -= y1; if ( y2 < 0 ) y2 = -y2;
    } else {
      x2 = (Int4)dx; y2 = (Int4)dy;
    }
    x1 = x2*x2 + y2*y2;
    y1 = (Int4)arrowSize * (Int4)arrowSize;
    if ( x1 < y1 ) {
      dangle = 36;
      arrowLen = 61;
      if ( x1 < y1/4 ) vis = FALSE;
    }
    if ( x2 > y2 ) {
      if ( x2 > 0xFFFFFF ) angle = atan100[ y2/(x2/100) ] ;
      else angle = atan100[ y2*100/x2 ] ;
    } else {
      if ( y2 > 0xFFFFFF ) angle = 90 - atan100[ x2/(y2/100) ] ;
      angle = 90 - atan100[ x2*100/y2 ] ;
    }
    switch ( pdata->fArrowPoint ){
      case 0:
        pts[0] = pt2;
        angle += 180;
        break;
      case 1:
        pts[0] = pt2;
        angle = 180 - angle;
        break;
      case 2:
        pts[0] = pt1;
        break;
      default:
        pts[0] = pt1;
        angle = 360 - angle;
    }
    angle += dangle;
    pts[1].x = pts[0].x + MSin(angle+90)*arrowLen/100*arrowSize/100;
    pts[1].y = pts[0].y - MSin(angle)*arrowLen/100*arrowSize/100;
    angle -= 2*dangle;
    pts[2].x = pts[0].x + MSin(angle+90)*arrowLen/100*arrowSize/100;
    pts[2].y = pts[0].y - MSin(angle)*arrowLen/100*arrowSize/100;
  }
  if ( vis ) DrawLine (pt1, pt2);
  if ( pdata->fArrowPoint != 0xF ){
    FramePoly (3, pts);
    SetPrimAttribute (pdc, SHADING_ATT);
    PaintPoly (3, pts);
  }
  if ( dInfoPtr->highlight != PLAIN_SEGMENT ) {
    SetPrimAttribute (pdc, STYLE_ATT|WIDTH_ATT);
    WidePen((Int2)(((DrawInfoPtr)pdc)->primattrib->penwidth + 2));
    if ( vis ) DrawLine (pt1, pt2);
    if ( pdata->fArrowPoint != 0xF ){
      FramePoly (3, pts);
      SetPrimAttribute (pdc, SHADING_ATT);
      PaintPoly (3, pts);
    }
  }
}

static Boolean LineHitTestProc ( LinePDataPtr pdata, PrimHitContext phc)
{
  register ScalePtr dInfoPtr;
  Int4  x1,y1,x2,y2;
 
  dInfoPtr = (ScalePtr)phc;
  if ( (pdata->box.left < dInfoPtr->worldWindow.right) && 
       (pdata->box.right > dInfoPtr->worldWindow.left) &&
       (pdata->box.top > dInfoPtr->worldWindow.bottom) &&
       (pdata->box.bottom < dInfoPtr->worldWindow.top) ) {
    x1 = pdata->box.left;
    x2 = pdata->box.right;
    if ( pdata->fPoint == 0 ) {
      y1 = pdata->box.bottom;
      y2 = pdata->box.top;
    } else {
      y2 = pdata->box.bottom;
      y1 = pdata->box.top;
    }
    return IsLineInVPort ( x1, y1, x2, y2, &(dInfoPtr->worldWindow) );
  }
  return FALSE; 
}

static Boolean LineOffsetProc ( LinePDataPtr pdata, Int4 deltaX, 
                                    Int4 deltaY )
{
  pdata->box.left += deltaX;
  pdata->box.right += deltaX;
  pdata->box.top += deltaY;
  pdata->box.bottom += deltaY;
  return TRUE; 
}

static Boolean LineGetLimitsProc ( LinePDataPtr pdata, Int4 scaleX,
Int4 scaleY, BoxPtr pLimits )
{
  *pLimits = pdata->box;
  switch ( pdata->fArrowPoint ) {
    case 0:
      pLimits->right += pdata->arrowSize*scaleX/2;
      pLimits->top += pdata->arrowSize*scaleY/2;
      break;
    case 1:
      pLimits->right += pdata->arrowSize*scaleX/2;
      pLimits->bottom -= pdata->arrowSize*scaleY/2;
      break;
    case 2:
      pLimits->left -= pdata->arrowSize*scaleX/2;
      pLimits->bottom -= pdata->arrowSize*scaleY/2;
      break;
    case 3:
      pLimits->left -= pdata->arrowSize*scaleX/2;
      pLimits->top += pdata->arrowSize*scaleY/2;
  }
  OutsetBox ( pLimits, scaleX<<2, scaleY<<2 );
  return TRUE; 
}

static PrimDef linePrimDef = {
  (PrimDrawProc) LineDrawProc,
  (PrimHitTestProc) LineHitTestProc,
  (PrimOffsetProc) LineOffsetProc,
  (PrimCleanupProc) NULL,
  (PrimGetLimitsProc) LineGetLimitsProc
};

/*****************************************************************************
*
*   AddLine (parent, x1, y1, x2, y2, arrow, primID)
*
*****************************************************************************/

PrimitivE Nlm_AddLine ( SegmenT parent, Int4 pnt1X, Int4 pnt1Y, Int4 pnt2X, 
                        Int4 pnt2Y, Boolean arrow, Uint2 primID )
{
  LinePData data;
  GenPPtr   gpp;

  if ( pnt1Y == pnt2Y ){
    return Nlm_AddHLine ( parent, pnt1X, pnt2X, pnt2Y, arrow, primID );
  }
  if ( pnt1X == pnt2X ){
    return Nlm_AddVLine ( parent, pnt1X, pnt1Y, pnt2Y, arrow, primID );
  }
  if ( (pnt1X == pnt2X) || (pnt1Y == pnt2Y) ) arrow = FALSE;
  if ( pnt2X >= pnt1X ) {
    data.box.left = pnt1X;
    data.box.right = pnt2X;
    data.fArrowPoint = 0;
  } else {
    data.box.left = pnt2X;
    data.box.right = pnt1X;
    data.fArrowPoint = 2;
  }
  if ( pnt2Y >= pnt1Y ) {
    data.box.bottom = pnt1Y;
    data.box.top = pnt2Y;
    if ( data.fArrowPoint ) data.fArrowPoint = 3;
  } else {
    data.box.bottom = pnt2Y;
    data.box.top = pnt1Y;
    if ( data.fArrowPoint == 0 ) data.fArrowPoint = 1;
  }
  data.arrowSize = userArrowSize;
  if ( data.fArrowPoint & 0x1 ) data.fPoint = 1;
  else data.fPoint = 0;
  if ( arrow == FALSE ) data.fArrowPoint = (Int1)0xF;
  gpp = (GenPPtr)AddPrimitive (&linePrimDef, parent, primID, (VoidPtr)&data,
                               sizeof(LinePData));
  if ( gpp != NULL ){
    gpp->att.shading = SOLID_SHADING;
  }
  return (PrimitivE)gpp;
}

/*****************************************************************************
*
*     BITMAP & SYMBOL
*
*****************************************************************************/

typedef struct bmppdata {
  Uint1Ptr bits;
  PntInfo  pnt;
  Int2     width;
  Int2     height;
  Int2     align;
} BmpPData, PNTR BmpPDataPtr;

static void BmpDrawProc (BmpPDataPtr pdata, PrimDrawContext pdc)
{
  register DrawInfoPtr dInfoPtr;
  Int4     x, y;
  BoxInfo  tmpBox;
  RecT     r;
 
  dInfoPtr = (DrawInfoPtr)pdc;
  MakeAlignBox ( pdata->pnt.x, pdata->pnt.y, 
                 (Int4)pdata->width * dInfoPtr->scale.scaleX, 
                 (Int4)pdata->height * dInfoPtr->scale.scaleY, 
                  0, 0, pdata->align, &tmpBox );
  if ( (tmpBox.left > dInfoPtr->scale.worldWindow.right) || 
       (tmpBox.right < dInfoPtr->scale.worldWindow.left) || 
       (tmpBox.top < dInfoPtr->scale.worldWindow.bottom) || 
       (tmpBox.bottom > dInfoPtr->scale.worldWindow.top) ) return; 
  x = (dInfoPtr->scale.offsetX + pdata->pnt.x) / dInfoPtr->scale.scaleX;
  y = (dInfoPtr->scale.offsetY - pdata->pnt.y) / dInfoPtr->scale.scaleY;
  MakeAlignRect ( (Int2)x, (Int2)y, pdata->width, pdata->height, 
                  0, pdata->align, &r );
  SetPrimAttribute (pdc, COLOR_ATT|MODE_ATT);
  if (pdata->bits != NULL) CopyBits (&r, pdata->bits);
  switch ( dInfoPtr->highlight ) {
    case FRAME_PRIMITIVE:
    case FILL_PRIMITIVE:
    case OUTLINE_PRIMITIVE:
      r.right ++;  r.bottom ++;
      SetPrimAttribute (pdc, STYLE_ATT|WIDTH_ATT);
      FrameRect (&r);
      InsetRect (&r, -1, -1);
      FrameRect (&r);
  }
}

static Boolean BmpHitTestProc ( BmpPDataPtr pdata, PrimHitContext phc)
{
  register ScalePtr dInfoPtr;
  BoxInfo  box;
 
  dInfoPtr = (ScalePtr)phc;
  MakeAlignBox ( pdata->pnt.x, pdata->pnt.y, 
                 (Int4)pdata->width * dInfoPtr->scaleX,
                 (Int4)pdata->height * dInfoPtr->scaleY,
                 0, 0, pdata->align, &box );
  if ( (box.left < dInfoPtr->worldWindow.right) && 
       (box.right > dInfoPtr->worldWindow.left) &&
       (box.top > dInfoPtr->worldWindow.bottom) &&
       (box.bottom < dInfoPtr->worldWindow.top) ) return TRUE; 
  return FALSE; 
}

static Boolean BmpOffsetProc ( BmpPDataPtr pdata, Int4 deltaX, 
                               Int4 deltaY )
{
  pdata->pnt.x += deltaX;
  pdata->pnt.y += deltaY;
  return TRUE; 
}

static Boolean BmpGetLimitsProc ( BmpPDataPtr pdata, Int4 scaleX,
                                  Int4 scaleY, BoxPtr pLimits )
{
  MakeAlignBox ( pdata->pnt.x, pdata->pnt.y, 
                 (Int4)pdata->width * scaleX,
                 (Int4)pdata->height * scaleY,
                 0, 0, pdata->align, pLimits );
  OutsetBox ( pLimits, scaleX<<2, scaleY<<2 );
  return TRUE; 
}

static PrimDef bmpPrimDef = {
  (PrimDrawProc) BmpDrawProc,
  (PrimHitTestProc) BmpHitTestProc,
  (PrimOffsetProc) BmpOffsetProc,
  (PrimCleanupProc) NULL,
  (PrimGetLimitsProc) BmpGetLimitsProc
};

/*****************************************************************************
*
*   AddBitmap (parent, pntX, pntY, width, height, data, align, primID)
*
*****************************************************************************/

PrimitivE AddBitmap (SegmenT parent, Int4 pntX, Int4 pntY, Int2 width,
                     Int2 height, Uint1Ptr bits, Int2 align, Uint2 primID)
{
  BmpPData data;

  data.pnt.x = pntX;
  data.pnt.y = pntY;
  data.bits = bits;
  data.width = width;
  data.height = height;
  data.align = align;
  return AddPrimitive (&bmpPrimDef, parent, primID, (VoidPtr)&data,
                       sizeof(BmpPData));
}

#define SYMBOL_WIDTH   8
#define SYMBOL_HEIGHT  8

static Uint1 rectSym [] = {
  0xFE, 0x82, 0x82, 0x82, 0x82, 0x82, 0xFE, 0x00
};
static Uint1 diamondSym [] = {
  0x10, 0x28, 0x44, 0x82, 0x44, 0x28, 0x10, 0x00
};
static Uint1 ovalSym [] = {
  0x38, 0x44, 0x82, 0x82, 0x82, 0x44, 0x38, 0x00
};
static Uint1 leftTriSym [] = {
  0x06, 0x1A, 0x62, 0x82, 0x62, 0x1A, 0x06, 0x00
};
static Uint1 rightTriSym [] = {
  0xC0, 0xB0, 0x8C, 0x82, 0x8C, 0xB0, 0xC0, 0x00
};
static Uint1 upTriSym [] = {
  0x10, 0x28, 0x28, 0x44, 0x44, 0x82, 0xFE, 0x00
};
static Uint1 downTriSym [] = {
  0xFE, 0x82, 0x44, 0x44, 0x28, 0x28, 0x10, 0x00
};
static Uint1 rectFillSym [] = {
  0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x00
};
static Uint1 diamondFillSym [] = {
  0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x10, 0x00
};
static Uint1 ovalFillSym [] = {
  0x38, 0x7C, 0xFE, 0xFE, 0xFE, 0x7C, 0x38, 0x00
};
static Uint1 leftTriFillSym [] = {
  0x06, 0x1E, 0x7E, 0xFE, 0x7E, 0x1E, 0x06, 0x00
};
static Uint1 rightTriFillSym [] = {
  0xC0, 0xF0, 0xFC, 0xFE, 0xFC, 0xF0, 0xC0, 0x00
};
static Uint1 upTriFillSym [] = {
  0x10, 0x38, 0x38, 0x7C, 0x7C, 0xFE, 0xFE, 0x00
};
static Uint1 downTriFillSym [] = {
  0xFE, 0xFE, 0x7C, 0x7C, 0x38, 0x38, 0x10, 0x00
};

static Uint1Ptr symbolList [] = {
  rectSym, diamondSym, ovalSym, leftTriSym,
  rightTriSym, upTriSym, downTriSym,
  rectFillSym, diamondFillSym, ovalFillSym, leftTriFillSym,
  rightTriFillSym, upTriFillSym, downTriFillSym
};

/*****************************************************************************
*
*   AddSymbol (parent, pntX, pntY, symbol, fill, align, primID)
*
*****************************************************************************/

PrimitivE AddSymbol (SegmenT parent, Int4 pntX, Int4 pntY,
                     Int2 symbol, Boolean fill, Int2 align, Uint2 primID)

{
  Int2 index;

  if (symbol < RECT_SYMBOL || symbol > DOWN_TRIANGLE_SYMBOL) {
    Message (MSG_ERROR, "AddSymbol symbol out of range");
    return NULL;
  }
  index = symbol - (Int2)RECT_SYMBOL;
  if (fill) index += 7;
  return AddBitmap (parent, pntX, pntY, SYMBOL_WIDTH, SYMBOL_HEIGHT,
                    symbolList [index], align, primID);
}

/*****************************************************************************
*
*     MARKER
*
*****************************************************************************/
typedef struct markpdata {
  PntInfo  pnt;
  Int2     length;
  Int2     orient;
} MarkPData, PNTR MarkPDataPtr;

static void MarkDrawProc (MarkPDataPtr pdata, PrimDrawContext pdc)
{
  register DrawInfoPtr dInfoPtr;
  Int4     x1, y1, x2, y2;
  Int4     curScale;
  Int4     len;
 
  dInfoPtr = (DrawInfoPtr)pdc;
  curScale = dInfoPtr->scale.scaleX;
  len = (Int4)pdata->length * curScale;
  x1 = (dInfoPtr->scale.offsetX + pdata->pnt.x) / curScale;
  if ( ( (pdata->pnt.x+len) < dInfoPtr->scale.worldWindow.left) ||
       ( (pdata->pnt.x-len) > dInfoPtr->scale.worldWindow.right) ) return;
  curScale = dInfoPtr->scale.scaleY;
  len = (Int4)pdata->length * curScale;
  y1 = (dInfoPtr->scale.offsetY - pdata->pnt.y) / curScale;
  if ( ((pdata->pnt.y+len) < dInfoPtr->scale.worldWindow.bottom) ||
       ((pdata->pnt.y-len) > dInfoPtr->scale.worldWindow.top) ) return;
  switch (pdata->orient){
    case HORIZ_LEFT :
      x2 = x1; y2 = y1;
      x1 -= (Int4)pdata->length;
      break;
    case HORIZ_CENTER :
      x2 = x1+(Int4)pdata->length / 2; y2 = y1;
      x1 = x2 - (Int4)pdata->length;
      break;
    case HORIZ_RIGHT :
      x2 = x1 + (Int4)pdata->length; y2 = y1;
      break;
    case VERT_ABOVE :
      x2 = x1; y2 = y1 - (Int4)pdata->length;
      break;
    case VERT_MIDDLE :
      x2 = x1; y2 = y1 - (Int4)pdata->length / 2;
      y1 = y2 + (Int4)pdata->length;
      break;
    default:
      x2 = x1; y2 = y1;
      y1 += (Int4)pdata->length;
  }
  SetPrimAttribute (pdc, COLOR_ATT|STYLE_ATT|WIDTH_ATT|MODE_ATT);
  MoveTo ((Int2)x1, (Int2)y1);
  LineTo ((Int2)x2, (Int2)y2);
  if ( dInfoPtr->highlight != PLAIN_SEGMENT ) {
    WidePen((Int2)(((DrawInfoPtr)pdc)->primattrib->penwidth + 2));
    MoveTo ((Int2)x1, (Int2)y1);
    LineTo ((Int2)x2, (Int2)y2);
  }
}

static Boolean MarkHitTestProc ( MarkPDataPtr pdata, PrimHitContext phc)
{
  register ScalePtr dInfoPtr;
  Int4     curScale;
  Int4     lenXY;

  dInfoPtr = (ScalePtr)phc;
  switch (pdata->orient){
    case HORIZ_LEFT :
    case HORIZ_CENTER :
    case HORIZ_RIGHT :
      lenXY = pdata->pnt.y;
      if ( (lenXY < dInfoPtr->worldWindow.bottom) ||
           (lenXY > dInfoPtr->worldWindow.top )) return FALSE;
      curScale = dInfoPtr->scaleX;
      lenXY = (Int4)pdata->length * curScale;
      switch (pdata->orient){
        case HORIZ_LEFT :
          if ( (pdata->pnt.x < dInfoPtr->worldWindow.left) ||
               ((pdata->pnt.x - lenXY) > dInfoPtr->worldWindow.right)) 
            return FALSE;
          break;
        case HORIZ_CENTER :
          lenXY /= 2;
          if ( ((pdata->pnt.x + lenXY) < dInfoPtr->worldWindow.left) ||
               ((pdata->pnt.x - lenXY) > dInfoPtr->worldWindow.right))
            return FALSE;
          break;
        default:
          if ( ((pdata->pnt.x + lenXY) < dInfoPtr->worldWindow.left) ||
               ( pdata->pnt.x > dInfoPtr->worldWindow.right)) 
            return FALSE;
      }
      break;
    default:
      lenXY = pdata->pnt.x;
      if ( (lenXY < dInfoPtr->worldWindow.left) ||
           (lenXY > dInfoPtr->worldWindow.right )) return FALSE;
      curScale = dInfoPtr->scaleY;
      lenXY = (Int4)pdata->length * curScale;
      switch (pdata->orient){
        case VERT_ABOVE :
          if ( ((pdata->pnt.y + lenXY) < dInfoPtr->worldWindow.bottom) ||
               (pdata->pnt.y > dInfoPtr->worldWindow.top)) 
            return FALSE;
          break;
        case VERT_MIDDLE :
          lenXY /= 2;
          if ( ((pdata->pnt.y + lenXY) < dInfoPtr->worldWindow.bottom) ||
               ((pdata->pnt.y - lenXY) > dInfoPtr->worldWindow.top)) 
            return FALSE;
          break;
        default:
          if ( (pdata->pnt.y < dInfoPtr->worldWindow.bottom) ||
               ((pdata->pnt.y - lenXY) > dInfoPtr->worldWindow.top)) 
            return FALSE;
          break;
      }
  }
  return TRUE;
}

static Boolean MarkOffsetProc ( MarkPDataPtr pdata, Int4 deltaX, 
                                    Int4 deltaY )
{
  pdata->pnt.x += deltaX;
  pdata->pnt.y += deltaY;
  return TRUE; 
}

static Boolean MarkGetLimitsProc ( MarkPDataPtr pdata, Int4 scaleX,
Int4 scaleY, BoxPtr pLimits )
{
  LoadBox (pLimits, pdata->pnt.x, pdata->pnt.y, pdata->pnt.x, pdata->pnt.y);
  switch (pdata->orient){
    case HORIZ_LEFT :
      pLimits->left -= (Int4)pdata->length * scaleX;
      break;
    case HORIZ_CENTER :
      pLimits->left -= (Int4)pdata->length * scaleX / 2;
      pLimits->right += (Int4)pdata->length * scaleX / 2;
      break;
    case HORIZ_RIGHT :
      pLimits->right += (Int4)pdata->length * scaleX;
      break;
    case VERT_ABOVE :
      pLimits->top += (Int4)pdata->length * scaleY;
      break;
    case VERT_MIDDLE :
      pLimits->top += (Int4)pdata->length * scaleY / 2;
      pLimits->bottom -= (Int4)pdata->length * scaleY / 2;
      break;
    default:
      pLimits->bottom -= (Int4)pdata->length * scaleY;
  }
  OutsetBox ( pLimits, scaleX<<2, scaleY<<2 );
  return TRUE; 
}

static PrimDef markPrimDef = {
  (PrimDrawProc) MarkDrawProc,
  (PrimHitTestProc) MarkHitTestProc,
  (PrimOffsetProc) MarkOffsetProc,
  (PrimCleanupProc) NULL,
  (PrimGetLimitsProc) MarkGetLimitsProc
};

/*****************************************************************************
*
*   AddMarker (parent, pntX, pntY, length, orient, primID)
*
*****************************************************************************/

PrimitivE AddMarker (SegmenT parent, Int4 pntX, Int4 pntY,
                     Int2 length, Int2 orient, Uint2 primID)
{
  MarkPData data;

  data.pnt.x = pntX;
  data.pnt.y = pntY;
  data.length = length;
  data.orient = orient;
  return AddPrimitive (&markPrimDef, parent, primID, (VoidPtr)&data,
                       sizeof(MarkPData));
}

/*****************************************************************************
*
*     TEXT LABEL
*
*****************************************************************************/

typedef struct lblpdata {
  PntInfo  pnt;
  FonT     font;
  Int2     width;
  Int2     height;
  Int2     align;
  Int2     offset;
  Char     str;
} LblPData, PNTR LblPDataPtr;


static void LblDrawProc (LblPDataPtr pdata, PrimDrawContext pdc)
{
  register DrawInfoPtr dInfoPtr;
  BoxInfo  tmpBox;
  Int4     x, y;
  RecT     r;
 
  dInfoPtr = (DrawInfoPtr)pdc;
  MakeAlignBox ( pdata->pnt.x, pdata->pnt.y, 
                 (Int4)pdata->width * dInfoPtr->scale.scaleX, 
                 (Int4)pdata->height * dInfoPtr->scale.scaleY, 
                  pdata->offset * dInfoPtr->scale.scaleX,
                  pdata->offset * dInfoPtr->scale.scaleY,
                  pdata->align, &tmpBox );
  if ( (tmpBox.left > dInfoPtr->scale.worldWindow.right) || 
       (tmpBox.right < dInfoPtr->scale.worldWindow.left) || 
       (tmpBox.top < dInfoPtr->scale.worldWindow.bottom) || 
       (tmpBox.bottom > dInfoPtr->scale.worldWindow.top) ) return; 
  x = (dInfoPtr->scale.offsetX + pdata->pnt.x) / dInfoPtr->scale.scaleX;
  y = (dInfoPtr->scale.offsetY - pdata->pnt.y) / dInfoPtr->scale.scaleY;
  MakeAlignRect ( (Int2)x, (Int2)y, pdata->width, pdata->height, 
                  pdata->offset, pdata->align, &r );
  SelectFont (pdata->font);
  SetPrimAttribute (pdc, COLOR_ATT|MODE_ATT);
  PaintStringEx(&pdata->str, r.left, (Nlm_Int2)(r.top + Ascent()));
  if ( dInfoPtr->highlight == OUTLINE_PRIMITIVE ){
    r.right ++;  r.bottom ++;
    SetPrimAttribute (pdc, STYLE_ATT|WIDTH_ATT);
    FrameRect (&r);
    InsetRect (&r, -1, -1);
    FrameRect (&r);
  }
}

static Boolean LblHitTestProc (LblPDataPtr pdata, PrimHitContext phc )
{
  register ScalePtr dInfoPtr;
  BoxInfo  box;
 
  dInfoPtr = (ScalePtr)phc;
  MakeAlignBox ( pdata->pnt.x, pdata->pnt.y, 
                 (Int4)pdata->width * dInfoPtr->scaleX,
                 (Int4)pdata->height * dInfoPtr->scaleY,
                 pdata->offset * dInfoPtr->scaleX,
                 pdata->offset * dInfoPtr->scaleY,
                 pdata->align, &box );
  if ( (box.left < dInfoPtr->worldWindow.right) && 
       (box.right > dInfoPtr->worldWindow.left) &&
       (box.top > dInfoPtr->worldWindow.bottom) &&
       ((box.bottom + (dInfoPtr->scaleY<<1)) < dInfoPtr->worldWindow.top) )
    return TRUE; 
  return FALSE; 
}

static void LblCleanupProc (LblPDataPtr pdata)
{
  DeleteFont (pdata->font);
}

static Boolean LblOffsetProc (LblPDataPtr pdata, Int4 deltaX, Int4 deltaY)
{
  pdata->pnt.x += deltaX;
  pdata->pnt.y += deltaY;
  return TRUE;
}

static Boolean LblGetLimitsProc ( LblPDataPtr pdata, Int4 scaleX,
                                  Int4 scaleY, BoxPtr pLimits )
{
  MakeAlignBox ( pdata->pnt.x, pdata->pnt.y, 
                 (Int4)pdata->width * scaleX,
                 (Int4)pdata->height * scaleY,
                 pdata->offset * scaleX,
                 pdata->offset * scaleY,
                 pdata->align, pLimits );
  OutsetBox ( pLimits, scaleX<<2, scaleY<<2 );
  return TRUE; 
}

static PrimDef lblPrimDef = {
  (PrimDrawProc) LblDrawProc,
  (PrimHitTestProc) LblHitTestProc,
  (PrimOffsetProc) LblOffsetProc,
  (PrimCleanupProc) LblCleanupProc,
  (PrimGetLimitsProc) LblGetLimitsProc
};

/*****************************************************************************
*
*   AddTextLabel (parent, pntX, pntY, string, font, offset, align, primID)
*
*****************************************************************************/

PrimitivE Nlm_AddTextLabel (SegmenT parent, Int4 pntX, Int4 pntY,
                            const Char* string, FonT font, Int2 offset, 
                            Int2 align, Uint2 primID )
{
  size_t      data_size = sizeof(LblPData) + Nlm_StringLen(string) + 1;
  LblPDataPtr dataPtr   = (LblPDataPtr)MemNew( data_size );
  PrimitivE   txtLabel;

  dataPtr->pnt.x = pntX;
  dataPtr->pnt.y = pntY;
  Nlm_StringCpy(&(dataPtr->str), string);
  dataPtr->font = CopyFont (font);
  SelectFont (font);
  dataPtr->width = StringWidth (string) + 2;
  dataPtr->height = LineHeight () + 2;
  dataPtr->align = align;
  dataPtr->offset = offset;
  SelectFont (programFont);
  txtLabel = AddPrimitive (&lblPrimDef, parent, primID,
                           (VoidPtr)dataPtr, (Int2)data_size);
  MemFree ( dataPtr );
  return txtLabel;
}

/*****************************************************************************
*
*   AddLabel (parent, pntX, pntY, string, size, offset, align, primID)
*
*****************************************************************************/

PrimitivE AddLabel (SegmenT parent, Int4 pntX, Int4 pntY, const Char* string,
                    Int2 size, Int2 offset, Int2 align, Uint2 primID)
{
  FonT  font;
  switch (size) {
    case SMALL_TEXT:
      if (smallFont == NULL)   SetSmallFont ();
      font = smallFont;
      break;
    case MEDIUM_TEXT:
      if (mediumFont == NULL)  SetMediumFont ();
      font = mediumFont;
      break;
    case LARGE_TEXT:
      if (largeFont == NULL)   SetLargeFont ();
      font = largeFont;
      break;
    default :
      font = programFont;
      break;
  }
  return AddTextLabel (parent, pntX, pntY, string, font,
                       offset, align, primID);
}


/*****************************************************************************
*
*     OVAL
*
*****************************************************************************/

typedef struct {
  PntInfo pnt;
  Int4    radx;
  Int4    rady;
  Boolean fill;
} OvalPData, PNTR OvalPDataPtr;


static void OvalDrawProc(OvalPDataPtr pdata, PrimDrawContext pdc)
{
  DrawInfoPtr dInfoPtr = (DrawInfoPtr)pdc;
  Int4 rad;
  Int4 tmp4;
  Int2 radx, rady;
  Int2 cx, cy;
  RecT oval_rect;
 
  rad = pdata->radx;
  tmp4 = pdata->pnt.x;
  if ((tmp4-rad) > dInfoPtr->scale.worldWindow.right || 
      (tmp4+rad) < dInfoPtr->scale.worldWindow.left)
    return;

  rad = pdata->rady;
  tmp4 = pdata->pnt.y;
  if ((tmp4+rad) < dInfoPtr->scale.worldWindow.bottom  || 
      (tmp4-rad) > dInfoPtr->scale.worldWindow.top)
    return; 

  radx = (Int2)(pdata->radx / dInfoPtr->scale.scaleX);
  rady = (Int2)(pdata->rady / dInfoPtr->scale.scaleY);
  cx = (Int2)((dInfoPtr->scale.offsetX + pdata->pnt.x)
       / dInfoPtr->scale.scaleX);
  cy = (Int2)((dInfoPtr->scale.offsetY - pdata->pnt.y)
       / dInfoPtr->scale.scaleY);
  LoadRect(&oval_rect,
           (Int2)(cx - radx),     (Int2)(cy - rady),
           (Int2)(cx + radx + 1), (Int2)(cy + rady + 1));

  if ( pdata->fill ) {
    SetPrimAttribute(pdc, COLOR_ATT|SHADING_ATT|MODE_ATT);
    PaintOval(&oval_rect);
  }
  else {
    SetPrimAttribute(pdc, COLOR_ATT|STYLE_ATT|WIDTH_ATT|MODE_ATT);
    FrameOval(&oval_rect);
  }

  switch ( dInfoPtr->highlight ) {
  case PLAIN_SEGMENT:
    break;
  case FILL_PRIMITIVE:
    if ( !pdata->fill ) {
      SetPrimAttribute (pdc, SHADING_ATT);
      PaintOval(&oval_rect);
    }
    break;
  default :
    SetPrimAttribute (pdc, STYLE_ATT|WIDTH_ATT);
    FrameRect(&oval_rect);
    InsetRect(&oval_rect, -1, -1);
    FrameRect(&oval_rect);
  }
}


static Boolean OvalHitTestProc(OvalPDataPtr pdata, PrimHitContext phc)
{
  ScalePtr dInfoPtr = (ScalePtr)phc;
  Int4     x,y;
  Int4     dx,dy;
  Int4     deltax, deltay;
  Int2     angle;
 
  if ((pdata->pnt.x - pdata->radx) > dInfoPtr->worldWindow.right  ||
      (pdata->pnt.x + pdata->radx) < dInfoPtr->worldWindow.left   ||
      (pdata->pnt.y - pdata->rady) > dInfoPtr->worldWindow.top    ||
      (pdata->pnt.y + pdata->rady) < dInfoPtr->worldWindow.bottom) 
    return FALSE;

  x = (dInfoPtr->worldWindow.right + dInfoPtr->worldWindow.left) / 2;
  y = (dInfoPtr->worldWindow.top + dInfoPtr->worldWindow.bottom) / 2;
  deltax = dInfoPtr->worldWindow.right - dInfoPtr->worldWindow.left;
  deltay = dInfoPtr->worldWindow.top - dInfoPtr->worldWindow.bottom;
  dx = x - pdata->pnt.x;
  dy = y - pdata->pnt.y;
  if (!dx  &&  !dy)
    return TRUE;
  x = (dx > 0) ? dx : -dx;
  y = (dy > 0) ? dy : -dy;
  if (x > y)
    angle = atan100[y * 100 / x];
  else
    angle = 90 - atan100[x * 100 / y] ;
  dx = (Int4)MSin(90 + angle) * pdata->radx / 100L;
  dy = (Int4)MSin(     angle) * pdata->rady / 100L;

  return (Boolean)(dx + deltax > x  &&  dy + deltay > y);
}

static Boolean OvalOffsetProc(OvalPDataPtr pdata, Int4 deltaX, Int4 deltaY)
{
  pdata->pnt.x += deltaX;
  pdata->pnt.y += deltaY;
  return TRUE;
}

static Boolean OvalGetLimitsProc(OvalPDataPtr pdata,
                                 Int4 scaleX, Int4 scaleY, BoxPtr pLimits)
{
  Int4 xy = pdata->pnt.x;
  pLimits->left   = xy - pdata->radx;
  pLimits->right  = xy + pdata->radx;
  xy = pdata->pnt.y;
  pLimits->bottom = xy - pdata->rady;
  pLimits->top    = xy + pdata->rady;
  OutsetBox(pLimits, scaleX<<2, scaleY<<2);
  return TRUE;
}

static PrimDef ovalPrimDef = {
  (PrimDrawProc     ) OvalDrawProc,
  (PrimHitTestProc  ) OvalHitTestProc,
  (PrimOffsetProc   ) OvalOffsetProc,
  (PrimCleanupProc  ) NULL,
  (PrimGetLimitsProc) OvalGetLimitsProc
};

/*****************************************************************************
*
*   AddOval (parent, pntX, pntY, radius, fill, primID)
*
*****************************************************************************/

PrimitivE AddOval(SegmenT parent, Int4 pntX, Int4 pntY, Int4 radiusX,
                  Int4 radiusY, Boolean fill, Uint2 primID)
{
  OvalPData data;

  data.pnt.x = pntX;
  data.pnt.y = pntY;
  data.radx = radiusX;
  data.rady = radiusY;
  data.fill = fill;
  return AddPrimitive(&ovalPrimDef, parent, primID, (VoidPtr)&data,
                      sizeof(OvalPData));
}


/*****************************************************************************
*
*     ARC
*
*****************************************************************************/

typedef struct arcpdata {
  BoxInfo  box;
  PntInfo  pnt;
  Int4     radx;
  Int4     rady;
  Int4     start;
  Int4     end;
  Boolean  fill;
} ArcPData, PNTR ArcPDataPtr;

static void FrameArcProc ( Int2 pNum, PoinT PNTR points )
{
  Int2 i;

  MoveTo ( points->x, points->y );
  points++;
  for (i=1; i<pNum; i++ ){
    LineTo ( points->x, points->y );
    points++;
  }
}

static void ArcDrawProc (ArcPDataPtr pdata, PrimDrawContext pdc)
{
  register DrawInfoPtr dInfoPtr;
  BoxInfo tmpBox;
  Int4     curScale;
  Int2     radx, rady;
  Int2     cx, cy;
  Int2     i,j,pNum;
  Int2     start, end;
  Int2     step;
  RecT     r;
 
  dInfoPtr = (DrawInfoPtr)pdc;
  tmpBox = pdata->box;
  if ( (tmpBox.left > dInfoPtr->scale.worldWindow.right) || 
       (tmpBox.right < dInfoPtr->scale.worldWindow.left) || 
       (tmpBox.top < dInfoPtr->scale.worldWindow.bottom) || 
       (tmpBox.bottom > dInfoPtr->scale.worldWindow.top) ) return; 
  radx = (Int2)(pdata->radx / dInfoPtr->scale.scaleX);
  rady = (Int2)(pdata->rady / dInfoPtr->scale.scaleY);
  cx = (Int2)((dInfoPtr->scale.offsetX + pdata->pnt.x)
       / dInfoPtr->scale.scaleX);
  cy = (Int2)((dInfoPtr->scale.offsetY - pdata->pnt.y)
       / dInfoPtr->scale.scaleY);
  if ( radx < rady ) i = rady;
  else i = radx;
  if ( i < 10 ) step = 10;
  else if ( i < 30 ) step = 5;
  else if ( i < 60 ) step = 3;
  else step = 1;
  start = (Int2) (pdata->start / 1000);
  end = (Int2) (pdata->end / 1000);
  if ( end < start ) end += 360;
  polyPoints[0].x = cx;
  polyPoints[0].y = cy;
  pNum = 1;
  for (j=0; j<2; j++ ) {
    for ( i=MAX(0,start); (i<90) && (i<=end); i+= step){
      polyPoints[pNum].x = cx + 
        (Int2)(((Int4)radx * (Int4)sin100[90-i]) / (Int4)100);
      polyPoints[pNum++].y = cy - 
        (Int2)(((Int4)rady * (Int4)sin100[i]) / (Int4)100);
    }
    start -= 90; end -= 90;
    for ( i=MAX(0,start); (i<90) && (i<=end); i+= step){
      polyPoints[pNum].x = cx -
        (Int2)(((Int4)radx * (Int4)sin100[i]) / (Int4)100);
      polyPoints[pNum++].y = cy - 
        (Int2)(((Int4)rady * (Int4)sin100[90-i]) / (Int4)100);
    }
    start -= 90; end -= 90;
    for ( i=MAX(0,start); (i<90) && (i<=end); i+= step){
      polyPoints[pNum].x = cx -
        (Int2)(((Int4)radx * (Int4)sin100[90-i]) / (Int4)100);
      polyPoints[pNum++].y = cy + 
      (Int2)(((Int4)rady * (Int4)sin100[i]) / (Int4)100);
    }
    start -= 90; end -= 90;
    for ( i=MAX(0,start); (i<90) && (i<=end); i+= step){
      polyPoints[pNum].x = cx + 
        (Int2)(((Int4)radx * (Int4)sin100[i]) / (Int4)100);
      polyPoints[pNum++].y = cy + 
        (Int2)(((Int4)rady * (Int4)sin100[90-i]) / (Int4)100);
    }
    start -= 90; end -= 90;
  }
  if (pdata->fill) {
    SetPrimAttribute (pdc, COLOR_ATT|SHADING_ATT|MODE_ATT);
  } else {
    SetPrimAttribute (pdc, COLOR_ATT|STYLE_ATT|WIDTH_ATT|MODE_ATT);
  }
  if ( pdata->fill ) PaintPoly(pNum, polyPoints);
  else FrameArcProc ( (Int2)(pNum-1), &(polyPoints[1]));
  switch ( dInfoPtr->highlight ) {
    case PLAIN_SEGMENT:
      break;
    case FILL_PRIMITIVE:
      if ( pdata->fill == 0 ) {
        SetPrimAttribute (pdc, SHADING_ATT);
        PaintPoly(pNum, polyPoints);
      }
      break;
    default :
      curScale = dInfoPtr->scale.scaleX;
      r.left = (Int2)((dInfoPtr->scale.offsetX + tmpBox.left) / curScale);
      r.right = (Int2)((dInfoPtr->scale.offsetX + tmpBox.right) / curScale)+1;
      curScale = dInfoPtr->scale.scaleY;
      r.top = (Int2)((dInfoPtr->scale.offsetY - tmpBox.top) / curScale);
      r.bottom = (Int2)((dInfoPtr->scale.offsetY - tmpBox.bottom) / curScale)+1;
      SetPrimAttribute (pdc, STYLE_ATT|WIDTH_ATT);
      FrameRect (&r);
      InsetRect (&r, -1, -1);
      FrameRect (&r);
  }
}

static Boolean ArcHitTestProc ( ArcPDataPtr pdata, PrimHitContext phc)
{
  register ScalePtr dInfoPtr;
 
  dInfoPtr = (ScalePtr)phc;
  if ( (pdata->box.left < dInfoPtr->worldWindow.right) && 
       (pdata->box.right > dInfoPtr->worldWindow.left) &&
       (pdata->box.top > dInfoPtr->worldWindow.bottom) &&
       (pdata->box.bottom < dInfoPtr->worldWindow.top) ) return TRUE;
  return FALSE;
}

static Boolean ArcOffsetProc ( ArcPDataPtr pdata, Int4 deltaX, 
                               Int4 deltaY )
{
  pdata->pnt.x += deltaX;
  pdata->pnt.y += deltaY;
  pdata->box.left += deltaX;
  pdata->box.right += deltaX;
  pdata->box.bottom += deltaY;
  pdata->box.top += deltaY;
  return TRUE;
}

static Boolean ArcGetLimitsProc ( ArcPDataPtr pdata, Int4 scaleX,
                                  Int4 scaleY, BoxPtr pLimits )
{
  *pLimits = pdata->box;
  return TRUE;
}

static PrimDef arcPrimDef = {
  (PrimDrawProc) ArcDrawProc,
  (PrimHitTestProc) ArcHitTestProc,
  (PrimOffsetProc) ArcOffsetProc,
  (PrimCleanupProc) NULL,
  (PrimGetLimitsProc) ArcGetLimitsProc
};

/*****************************************************************************
*
*   AddArc (parent, pntX, pntY, radius, start, end, fill, primID)
*
*****************************************************************************/

PrimitivE AddArc (SegmenT parent, Int4 pntX, Int4 pntY, Int4 radiusX,
                  Int4 radiusY, Int4 start, Int4 end, Boolean fill, 
                  Uint2 primID)
{
  ArcPData data;

  data.pnt.x = pntX;
  data.pnt.y = pntY;
  data.radx = radiusX;
  data.rady = radiusY;
  data.start = start;
  data.end = end;
  data.fill = fill;
  data.box.left = pntX - radiusX;
  data.box.right = pntX + radiusX;
  data.box.top = pntY + radiusY;
  data.box.bottom = pntY - radiusY;
  return AddPrimitive (&arcPrimDef, parent, primID, (VoidPtr)&data,
                       sizeof(ArcPData));
}

/*****************************************************************************
*
*     POLYGON
*
*****************************************************************************/
typedef struct polpdata {
  BoxInfo  box;
  Int2     vernum;
  Int2     fill;
  PntInfo  pnt;
} PolyPData, PNTR PolyPDataPtr;

static void PolyDrawProc (PolyPDataPtr pdata, PrimDrawContext pdc)
{
  register DrawInfoPtr dInfoPtr;
  PntPtr   pPtr;
  BoxInfo  tmpBox;
  Int4     curScale;
  RecT     r;
  Int2     vernum;
  Int2     i;
 
  dInfoPtr = (DrawInfoPtr)pdc;
  tmpBox = pdata->box;
  if ( (tmpBox.left > dInfoPtr->scale.worldWindow.right) || 
       (tmpBox.right < dInfoPtr->scale.worldWindow.left) || 
       (tmpBox.top < dInfoPtr->scale.worldWindow.bottom) || 
       (tmpBox.bottom > dInfoPtr->scale.worldWindow.top) ) return; 
  if (pdata->fill) {
    SetPrimAttribute (pdc, COLOR_ATT|SHADING_ATT|MODE_ATT);
  } else {
    SetPrimAttribute (pdc, COLOR_ATT|STYLE_ATT|WIDTH_ATT|MODE_ATT);
  }
  vernum = pdata->vernum;
  pPtr = &(pdata->pnt);
  for ( i=0; i<vernum; i++ ){
    polyPoints[i].x = (Int2)((dInfoPtr->scale.offsetX + pPtr->x)
                      / dInfoPtr->scale.scaleX);
    polyPoints[i].y = (Int2)((dInfoPtr->scale.offsetY - pPtr->y)
                      / dInfoPtr->scale.scaleY);
    pPtr++;
  }
  if ( pdata->fill ) PaintPoly(vernum, polyPoints);
  else FramePoly(vernum, polyPoints);
  switch ( dInfoPtr->highlight ) {
    case PLAIN_SEGMENT:
      break;
    case FILL_PRIMITIVE:
      if ( pdata->fill == 0 ) {
        SetPrimAttribute (pdc, SHADING_ATT);
        PaintPoly(vernum, polyPoints);
      }
      break;
    default:
      curScale = dInfoPtr->scale.scaleX;
      r.left = (Int2)((dInfoPtr->scale.offsetX + tmpBox.left) / curScale);
      r.right = (Int2)((dInfoPtr->scale.offsetX + tmpBox.right) / curScale)+1;
      curScale = dInfoPtr->scale.scaleY;
      r.top = (Int2)((dInfoPtr->scale.offsetY - tmpBox.top) / curScale);
      r.bottom = (Int2)((dInfoPtr->scale.offsetY - tmpBox.bottom) / curScale)+1;
      SetPrimAttribute (pdc, STYLE_ATT|WIDTH_ATT);
      FrameRect (&r);
      InsetRect (&r, -1, -1);
      FrameRect (&r);
  }
}

static Boolean PolyHitTestProc ( PolyPDataPtr pdata, PrimHitContext phc)
{
  register ScalePtr dInfoPtr;
 
  dInfoPtr = (ScalePtr)phc;
  if ( (pdata->box.left < dInfoPtr->worldWindow.right) && 
       (pdata->box.right > dInfoPtr->worldWindow.left) &&
       (pdata->box.top > dInfoPtr->worldWindow.bottom) &&
       (pdata->box.bottom < dInfoPtr->worldWindow.top) ) return TRUE; 
  return FALSE; 
}

static Boolean PolyOffsetProc ( PolyPDataPtr pdata, Int4 deltaX, 
                                   Int4 deltaY )
{
  PntPtr   pPtr;
  Int2     vernum;
  Int2     i;

  pdata->box.left += deltaX;
  pdata->box.right += deltaX;
  pdata->box.top += deltaY;
  pdata->box.bottom += deltaY;
  vernum = pdata->vernum;
  pPtr = &(pdata->pnt);
  for ( i=0; i<vernum; i++ ){
    pPtr->x += deltaX;
    pPtr->y += deltaY;
    pPtr++;
  }
  return TRUE; 
}

static Boolean PolyGetLimitsProc ( PolyPDataPtr pdata, Int4 scaleX,
                                  Int4 scaleY, BoxPtr pLimits )
{
  *pLimits = pdata->box;
  OutsetBox ( pLimits, scaleX<<2, scaleY<<2 );
  return TRUE;
}

static PrimDef polyPrimDef = {
  (PrimDrawProc) PolyDrawProc,
  (PrimHitTestProc) PolyHitTestProc,
  (PrimOffsetProc) PolyOffsetProc,
  (PrimCleanupProc) NULL,
  (PrimGetLimitsProc) PolyGetLimitsProc
};

/*****************************************************************************
*
*   AddPolygon (parent, vernum, points, fill, primID)
*
*****************************************************************************/

PrimitivE AddPolygon (SegmenT parent, Int2 vernum, PntPtr points,
                     Boolean fill, Uint2 primID)
{
  PrimitivE    poly = NULL;
  PolyPDataPtr dataPtr;
  PntPtr       pPtr;
  Int2         i;

  if ( (vernum > 1) && (vernum <= 32) && (points != NULL) ){
    dataPtr = (PolyPDataPtr)MemNew(sizeof(PolyPData) + vernum*sizeof(PntInfo));
    dataPtr->vernum = vernum;
    dataPtr->fill = (Int2)fill;
    LoadBox ( &(dataPtr->box), INT4_MAX, INT4_MIN, INT4_MIN, INT4_MAX );
    pPtr = &(dataPtr->pnt);
    for ( i=0; i<vernum; i++ ){
      pPtr->x = points[i].x;
      if ( dataPtr->box.left > pPtr->x ) dataPtr->box.left = pPtr->x;
      if ( dataPtr->box.right < pPtr->x ) dataPtr->box.right = pPtr->x;
      pPtr->y = points[i].y;
      if ( dataPtr->box.top < pPtr->y ) dataPtr->box.top = pPtr->y;
      if ( dataPtr->box.bottom > pPtr->y ) dataPtr->box.bottom = pPtr->y;
      pPtr++;
    }
    poly = AddPrimitive (&polyPrimDef, parent, primID, (VoidPtr)dataPtr,
                         (Int2)(sizeof(PolyPData) + vernum*sizeof(PntInfo)));
    MemFree ( dataPtr );
  }
  return poly;
}

/*****************************************************************************
*
*   DrawSegment (seg, drawinfo )
*       Draws a segment, recursively tracking attributes
*
*****************************************************************************/

extern void Nlm_DrawSegment (SegPPtr seg, Nlm_DrawInfoPtr drawinfoPtr )
{
  BasePPtr  item;
  DrawInfo  drawinfo;
  RecT      r;
  Int1      highlight;
  Int1      highlightPrim;

  if (seg != NULL && drawinfoPtr != NULL) {
    drawinfo = *drawinfoPtr;
    drawinfo.primattrib = &blackAttPData;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      if (seg->seg.maxscale == 0 || 
          seg->seg.maxscale >= 
          MAX (drawinfo.scale.scaleX, drawinfo.scale.scaleY)) {
        if ( (seg->seg.box.left <= drawinfo.scale.worldWindow.right) &&
             (seg->seg.box.right >= drawinfo.scale.worldWindow.left) &&
             (seg->seg.box.top >= drawinfo.scale.worldWindow.bottom) &&
             (seg->seg.box.bottom <= drawinfo.scale.worldWindow.top) ){
          if (seg->seg.visible) {
             SetPrimAttribute ( (Nlm_PrimDrawContext)&drawinfo, (Uint1)0xFF );
            if ( !drawinfo.checked ) {
              if ((seg->seg.box.left >= drawinfo.scale.worldWindow16.left) &&
                (seg->seg.box.right <= drawinfo.scale.worldWindow16.right) &&
                (seg->seg.box.top <= drawinfo.scale.worldWindow16.top) &&
                (seg->seg.box.bottom >= drawinfo.scale.worldWindow16.bottom))
                drawinfo.checked = TRUE;
            }
            if ( seg->seg.highlight != PLAIN_SEGMENT ) {
              highlight = seg->seg.highlight;
            } else {
              highlight = drawinfo.highlight;
            }
            switch ( highlight ) {
              case FRAME_CONTENTS:
                highlightPrim = FRAME_PRIMITIVE;
                break;
              case FILL_CONTENTS:  
                highlightPrim = FILL_PRIMITIVE;
                break;
              case OUTLINE_CONTENTS:
                highlightPrim = OUTLINE_PRIMITIVE;
                break;
              default:
                highlightPrim = PLAIN_PRIMITIVE;
            }
            item = seg->seg.head;
            while (item != NULL) {
              switch (item->code) {
                case GENERIC :
                  drawinfo.primattrib = &(((Nlm_GenPPtr)item)->att);
                  drawinfo.highlight = ((Nlm_GenPPtr)item)->highlight;
                  if ( drawinfo.highlight == PLAIN_PRIMITIVE ) {
                    drawinfo.highlight = highlightPrim;
                  }
                  DrawPrimitive (item, &drawinfo );
                  break;
                case PICTURE :
                  Message (MSG_ERROR, "DrawSegment child is a picture");
                  break;
                case SEGMENT :
                  drawinfo.highlight = highlight;
                  Nlm_DrawSegment ((SegPPtr)item, &drawinfo);
                  break;
                default :
                  Message (MSG_ERROR, "DrawSegment child is unknown");
              }
              item = item->next;
            }
            if ( seg->seg.highlight != PLAIN_SEGMENT ) {
              drawinfo.primattrib = &blackAttPData;
				  SetPrimAttribute ( (Nlm_PrimDrawContext)&drawinfo, 0xFF );
              MapWorldBoxToRect ( &r, &(seg->seg.box), &(drawinfo.scale) );
              switch (seg->seg.highlight) {
                case FRAME_SEGMENT :
                  FrameRect (&r);
                  break;
                case FILL_SEGMENT :
                  PaintRect (&r);
                  break;
                case OUTLINE_SEGMENT :
                  InsetRect (&r, -4, -4);
                  FrameRect (&r);
                  InsetRect (&r, 1, 1);
                  FrameRect (&r);
                  InsetRect (&r, 2, 2);
                  drawinfo.primattrib = &whiteAttPData;
						SetPrimAttribute ( (Nlm_PrimDrawContext)&drawinfo, 0xFF );
                  FrameRect (&r);
              }
            }
          }
        }
      }
      drawinfoPtr->curattrib = drawinfo.curattrib;
    } else {
      Message (MSG_ERROR, "DrawSegment argument not a segment or picture");
    }
  }
}


/*****************************************************************************
*
*    SENTINEL RECTANGLE
*
*****************************************************************************/

typedef struct sntrecpdata {
  BoxInfo          box;
  SntOnDrawProc    callback;
  BigScalar        calldata;
  SntOnCleanupProc cleanup;
} SntRecPData, PNTR SntRecPDataPtr;


static void SntRecDrawProc(SntRecPDataPtr pdata, PrimDrawContext pdc)
{
  BoxPtr primBoxPtr = &pdata->box;
  BoxPtr drawBoxPtr = &((DrawInfoPtr)pdc)->scale.worldWindow;

  if ( (primBoxPtr->left   > drawBoxPtr->right ) || 
       (primBoxPtr->right  < drawBoxPtr->left  ) || 
       (primBoxPtr->top    < drawBoxPtr->bottom) || 
       (primBoxPtr->bottom > drawBoxPtr->top   ) )
    return;
  
  if ( pdata->callback )
    (*pdata->callback)( pdata->calldata, pdc );
}

static Boolean SntRecHitTestProc(SntRecPDataPtr pdata, PrimHitContext phc)
{
  BoxPtr primBoxPtr = &pdata->box;
  BoxPtr drawBoxPtr = &((ScalePtr)phc)->worldWindow;

  return (Boolean)((primBoxPtr->left   < drawBoxPtr->right )  &&
                   (primBoxPtr->right  > drawBoxPtr->left  )  &&
                   (primBoxPtr->top    > drawBoxPtr->bottom)  &&
                   (primBoxPtr->bottom < drawBoxPtr->top   ));
}

static void SntRecCleanupProc (SntRecPDataPtr pdata)
{
  if ( pdata->cleanup )
    (*pdata->cleanup)( pdata->calldata );
}

static Boolean SntRecOffsetProc(SntRecPDataPtr pdata, Int4 deltaX, Int4 deltaY)
{
  BoxPtr primBoxPtr = &pdata->box;

  primBoxPtr->left   += deltaX;
  primBoxPtr->right  += deltaX;
  primBoxPtr->top    += deltaY;
  primBoxPtr->bottom += deltaY;

  return TRUE; 
}

static Boolean SntRecGetLimitsProc(SntRecPDataPtr pdata,
                                   Int4 scaleX, Int4 scaleY, BoxPtr pLimits)
{
  *pLimits = pdata->box;
  OutsetBox(pLimits, scaleX<<2, scaleY<<2);
  return TRUE; 
}

extern void ChangeSentinelRectangle (PrimitivE prim, Int4 left, Int4 top, Int4 right, Int4 bottom)

{
  SntRecPDataPtr  pdata;

  if (prim == NULL) return;
  pdata = (SntRecPDataPtr) &(((GenPPtr) prim)->data);
  pdata->box.left = left;
  pdata->box.top = top;
  pdata->box.right = right;
  pdata->box.bottom = bottom;
}

static PrimDef sntrecPrimDef = {
  (PrimDrawProc)      SntRecDrawProc,
  (PrimHitTestProc)   SntRecHitTestProc,
  (PrimOffsetProc)    SntRecOffsetProc,
  (PrimCleanupProc)   SntRecCleanupProc,
  (PrimGetLimitsProc) SntRecGetLimitsProc
};

/*****************************************************************************
*
*   AddSntRectangle(parent, left,top,right,bottom, callback,calldata, primID)
*
*****************************************************************************/
extern PrimitivE AddSntRectangle(SegmenT parent,
                                 Int4 left, Int4 top, Int4 right, Int4 bottom,
                                 SntOnDrawProc callback, BigScalar calldata,
                                 SntOnCleanupProc cleanup, Uint2 primID)
{
  SntRecPData data;
  Int4 swap;

  if (left > right) {
    swap = left;
    left = right;
    right = swap;
  }
  if (bottom > top) {
    swap = bottom;
    bottom = top;
    top = swap;
  }
  data.box.left   = left;
  data.box.top    = top;
  data.box.right  = right;
  data.box.bottom = bottom;
  data.callback = callback;
  data.calldata = calldata;
  data.cleanup = cleanup;
  return AddPrimitive(&sntrecPrimDef, parent, primID, (VoidPtr)&data,
                      sizeof(SntRecPData));
}

