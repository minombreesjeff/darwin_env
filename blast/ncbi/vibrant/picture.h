/*   picture.h
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
* File Name:  picture.h
*
* Author:  Jonathan Kans, Alex Smirnov, Jill Shermer
*
* Version Creation Date:   10/23/92
*
* $Revision: 6.15 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: picture.h,v $
* Revision 6.15  2004/02/17 19:22:54  johnson
* make segment ID consistly unsigned
*
* Revision 6.14  2003/03/18 17:00:10  kans
* Nlm_AddSilentSegRect does not absorb clicks, SegRect reverted to absorb clicks, so desktop click responsiveness is maintained
*
* Revision 6.13  2002/08/07 18:13:42  kans
* G/SetPrimitiveIDs, itemID is Uint4
*
* Revision 6.12  2002/03/07 15:53:45  kans
* added SetSegmentVisibleFlag to set/clear the visible flag before attaching to a viewer
*
* Revision 6.11  1999/10/13 17:45:46  kans
* added entityID, itemID, and itemtype to primitive internal structure, added Get and Set functions
*
* Revision 6.10  1999/10/04 19:39:09  kans
* include vibdefns.h
*
* Revision 6.9  1999/10/04 17:16:32  kans
* include ncbidraw.h instead of vibrant.h, a couple Nlm_ prefixes
*
* Revision 6.8  1998/07/01 18:27:43  vakatov
* Use "const" qualifier somewhere
*
* Revision 6.7  1998/06/30 00:32:03  vakatov
* Nlm_AddAttribute():  pass "color" as constant
*
* Revision 6.6  1998/06/01 17:29:20  vakatov
* Extended rounded rect. options -- let it roundup only two(adjacent)
* corners(see "flags" arg in Nlm_AddRoundedRectangle())
*
* Revision 6.5  1998/04/27 16:05:46  vakatov
* + AddRoundedRectangle()
*
* Revision 6.4  1997/12/16 18:38:36  kans
* sentinel draw callback has draw info context parameter
*
* Revision 6.3  1997/12/15 18:10:44  kans
* added cleanup data function to sentinel
*
* Revision 6.2  1997/11/13 21:41:34  vakatov
* Added "sentinel rectangle" primitive[Nlm_AddSntRectangle] which
* calls its callback procedure[Nlm_SntOnDrawProc] on each redraw
*
* Revision 6.1  1997/10/30 20:01:43  vakatov
* Added Nlm_PrimitiveBox()
*
* Revision 5.1  1997/07/02 18:35:53  vakatov
* Nlm_DeletePrim() redesigned/rewritten to guarantee prim. list consistency
*
* Revision 4.2  1996/03/18  20:41:36  vakatov
* ChangeAddPrimOrder(...) function allows one to specify the order of adding
* primitives(or segments) to a picture
*
* 022495   Alex
* ==========================================================================
*/

#ifndef _PICTURE_
#define _PICTURE_

#ifndef _NCBIDRAW_
#include <ncbidraw.h>
#endif

#ifndef _VIBDEFNS_
#include <vibdefns.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   STRUCTURE TYPEDEFS
*
*****************************************************************************/

typedef struct Nlm_segment {
  Nlm_VoidPtr  dummy;
} PNTR Nlm_SegmenT;

typedef struct Nlm_primitive {
  Nlm_VoidPtr  dummy;
} PNTR Nlm_PrimitivE;

/*  BoxInfo stores the world coordinates of a rectangle */

typedef struct Nlm_boxinfo {
  Nlm_Int4  left;
  Nlm_Int4  top;
  Nlm_Int4  right;
  Nlm_Int4  bottom;
} Nlm_BoxInfo, PNTR Nlm_BoxPtr;

/*  PntInfo stores the world coordinates of a point */

typedef struct Nlm_pntinfo {
  Nlm_Int4  x;
  Nlm_Int4  y;
} Nlm_PntInfo, PNTR Nlm_PntPtr;

/*****************************************************************************
*
*   GENERIC PRIMITIVE DEFS
*
*****************************************************************************/

typedef struct Nlm_primdrawctxt {
  Nlm_VoidPtr  dummy;
} PNTR Nlm_PrimDrawContext;

typedef struct Nlm_primhittestctxt {
  Nlm_VoidPtr  dummy;
} PNTR Nlm_PrimHitContext;

typedef void (*Nlm_PrimDrawProc) PROTO((Nlm_VoidPtr pextra, 
Nlm_PrimDrawContext pdc));
typedef Nlm_Boolean (*Nlm_PrimHitTestProc) PROTO((Nlm_VoidPtr pextra, 
Nlm_PrimHitContext phc));
typedef Nlm_Boolean (*Nlm_PrimOffsetProc) PROTO((Nlm_VoidPtr pextra, 
Nlm_Int4 deltaX, Nlm_Int4 deltaY));
typedef Nlm_Boolean (*Nlm_PrimGetLimitsProc) PROTO(( Nlm_VoidPtr pextra, 
Nlm_Int4 scaleX, Nlm_Int4 scaleY, Nlm_BoxPtr pLimits));
typedef void (*Nlm_PrimCleanupProc) PROTO((Nlm_VoidPtr pextra));

/* The following struct may be changed with the next version.
Use functions ResetPrimDef(), Nlm_SetPrimDrawProc(), etc, instead
of Nlm_PrimDef = { values } */

typedef struct Nlm_primdef {
  Nlm_PrimDrawProc      draw;
  Nlm_PrimHitTestProc   hittest;
  Nlm_PrimOffsetProc    offset;
  Nlm_PrimCleanupProc   cleanup;
  Nlm_PrimGetLimitsProc getlimits;
} Nlm_PrimDef, PNTR Nlm_PrimDefPtr;

extern void Nlm_ResetPrimDef PROTO((Nlm_PrimDefPtr)); 
extern void Nlm_SetPrimDrawProc PROTO((Nlm_PrimDrawProc)); 
extern void Nlm_SetPrimHitTestProc PROTO((Nlm_PrimHitTestProc));
extern void Nlm_SetPrimOffsetProc PROTO((Nlm_PrimOffsetProc));
extern void Nlm_SetPrimGetLimitsProc PROTO((Nlm_PrimGetLimitsProc));
extern void Nlm_SetPrimCleanupProc PROTO((Nlm_PrimCleanupProc));

/* helper functions for callbacks */

extern Nlm_BoxPtr  Nlm_GetWorldWindow PROTO((Nlm_PrimDrawContext pdc));
extern Nlm_Int4    Nlm_GetScaleX PROTO((Nlm_PrimDrawContext pdc));
extern Nlm_Int4    Nlm_GetScaleY PROTO((Nlm_PrimDrawContext pdc));
extern Nlm_Int4    Nlm_GetOffsetX PROTO((Nlm_PrimDrawContext pdc));
extern Nlm_Int4    Nlm_GetOffsetY PROTO((Nlm_PrimDrawContext pdc));
extern Nlm_Boolean Nlm_isPrimInWindow PROTO((Nlm_PrimDrawContext pdc));
extern Nlm_Int1    Nlm_GetPrimHligh PROTO((Nlm_PrimDrawContext pdc));
extern Nlm_BoxPtr  Nlm_GetHitWindow PROTO((Nlm_PrimHitContext phc));
extern void        Nlm_SetPrimAttribute PROTO((Nlm_PrimDrawContext pdc,
                                               Nlm_Uint1 flags ));
extern void        Nlm_RestorePrimAttribute PROTO((Nlm_PrimDrawContext pdc));

/*****************************************************************************
*
*   SYMBOL DEFINES
*
*****************************************************************************/

/*  Attribute flag bits */

#define NO_ATTS             0
#define COLOR_ATT           1
#define STYLE_ATT           2
#define SHADING_ATT         4
#define WIDTH_ATT           8
#define MODE_ATT           16

/*  Color specifications */

extern Uint1 BLACK_COLOR [3];
extern Uint1 WHITE_COLOR [3];
extern Uint1 RED_COLOR [3];
extern Uint1 GREEN_COLOR [3];
extern Uint1 BLUE_COLOR [3];
extern Uint1 YELLOW_COLOR [3];
extern Uint1 CYAN_COLOR [3];
extern Uint1 MAGENTA_COLOR [3];

/*  Line styles */

#define NO_LINE_STYLE          0
#define SOLID_LINE             1
#define DOTTED_LINE            2
#define DASHED_LINE            3

/*  Shading styles */

#define NO_SHADING             0
#define SOLID_SHADING          1
#define DARK_SHADING           2
#define MEDIUM_SHADING         3
#define LIGHT_SHADING          4
#define EMPTY_SHADING          5

#define THIN_HORIZ_SHADING     6
#define THIN_VERT_SHADING      7
#define THICK_HORIZ_SHADING    8
#define THICK_VERT_SHADING     9

#define THIN_NESW_SHADING     10
#define THIN_NWSE_SHADING     11
#define THICK_NESW_SHADING    12
#define THICK_NWSE_SHADING    13

/*  Pen width */

#define NO_PEN_WIDTH           0
#define STD_PEN_WIDTH          1
#define PEN_MAX                6

/*  Mode specifications */

#define NO_MODE                0
#define COPY_MODE              1
#define MERGE_MODE             2
#define INVERT_MODE            3
#define ERASE_MODE             4

/*  Rectangle arrow direction */

#define NO_ARROW               0
#define LEFT_ARROW             1
#define RIGHT_ARROW            2
#define UP_ARROW               3
#define DOWN_ARROW             4

/*  Symbol types */

#define RECT_SYMBOL            1
#define DIAMOND_SYMBOL         2
#define OVAL_SYMBOL            3
#define LEFT_TRIANGLE_SYMBOL   4
#define RIGHT_TRIANGLE_SYMBOL  5
#define UP_TRIANGLE_SYMBOL     6
#define DOWN_TRIANGLE_SYMBOL   7

/*  Alignment specifications, relative to point */

#define MIDDLE_CENTER          1
#define UPPER_LEFT             2
#define LOWER_LEFT             3
#define UPPER_RIGHT            4
#define LOWER_RIGHT            5
#define UPPER_CENTER           6
#define LOWER_CENTER           7
#define MIDDLE_LEFT            8
#define MIDDLE_RIGHT           9

/*  Orientation specifications, relative to point */

#define HORIZ_LEFT            11
#define HORIZ_CENTER          12
#define HORIZ_RIGHT           13
#define VERT_ABOVE            14
#define VERT_MIDDLE           15
#define VERT_BELOW            16

/*  Text sizes */

#define SMALL_TEXT             1
#define MEDIUM_TEXT            2
#define LARGE_TEXT             3

/*  Segment highlight style */

#define PLAIN_SEGMENT          0
#define FRAME_SEGMENT          1
#define FILL_SEGMENT           2
#define FRAME_CONTENTS         3
#define FILL_CONTENTS          4
#define OUTLINE_SEGMENT        5
#define OUTLINE_CONTENTS       6

/*  Primitive highlight style */

#define PLAIN_PRIMITIVE        0
#define FRAME_PRIMITIVE        1
#define FILL_PRIMITIVE         2
#define OUTLINE_PRIMITIVE      3

typedef Nlm_Boolean (*Nlm_SegmentExploreProc) PROTO((Nlm_SegmenT seg, 
Nlm_PrimitivE prim, Nlm_Uint2 segID, Nlm_Uint2 primID, Nlm_Uint2 primCt, 
Nlm_VoidPtr userdata));

/*****************************************************************************
*
*   FUNCTION PROTOTYPES
*
*****************************************************************************/

extern Nlm_SegmenT   Nlm_CreatePicture  PROTO((void));
extern Nlm_SegmenT   Nlm_DeletePicture  PROTO((Nlm_SegmenT picture));

extern Nlm_SegmenT   Nlm_CreateSegment  PROTO((Nlm_SegmenT parent, 
Uint2 segID, Int4 maxScale));
extern Nlm_SegmenT   Nlm_ResetSegment   PROTO((Nlm_SegmenT segment));
extern Nlm_SegmenT   Nlm_DeleteSegment  PROTO((Nlm_SegmenT segment));

extern Nlm_SegmenT   Nlm_ParentSegment  PROTO((Nlm_SegmenT segment));
extern Nlm_Uint2     Nlm_SegmentID      PROTO((Nlm_SegmenT segment));
extern Nlm_Boolean   Nlm_SegmentVisible PROTO((Nlm_SegmenT segment));
extern Nlm_Int1      Nlm_SegmentStyle   PROTO((Nlm_SegmenT segment));
extern void          Nlm_SegmentBox     PROTO((Nlm_SegmenT segment, 
Nlm_BoxPtr box));

extern void          Nlm_AddAttribute   PROTO((Nlm_SegmenT parent, 
Nlm_Uint1 flags, const Nlm_Uint1* color, Nlm_Int1 linestyle, Nlm_Int1 shading, 
Nlm_Int1 penwidth, Nlm_Int1 mode));
extern void          Nlm_SetDefArrowSize PROTO((Nlm_Int1 arrowSize));
extern Nlm_PrimitivE Nlm_AddPrimitive PROTO((Nlm_PrimDefPtr pdp,
Nlm_SegmenT parent, Nlm_Uint2 primID, Nlm_VoidPtr pextra, Nlm_Int2 extrasize));
extern Nlm_PrimitivE Nlm_AddInvFrame     PROTO((Nlm_SegmenT parent, 
Nlm_Int4 left, Nlm_Int4 top, Nlm_Int4 right, Nlm_Int4 bottom, Nlm_Uint2 primID));
extern Nlm_PrimitivE Nlm_AddRectangle   PROTO((Nlm_SegmenT parent, 
Nlm_Int4 left, Nlm_Int4 top, Nlm_Int4 right, Nlm_Int4 bottom, Nlm_Int2 arrow,
Nlm_Boolean fill, Nlm_Uint2 primID));

#define ROREC_TOP    0x1
#define ROREC_BOTTOM 0x2
#define ROREC_LEFT   0x4
#define ROREC_RIGHT  0x8
extern Nlm_PrimitivE Nlm_AddRoundedRectangle PROTO((Nlm_SegmenT parent, 
Nlm_Int4 left, Nlm_Int4 top, Nlm_Int4 right, Nlm_Int4 bottom,
Nlm_Int4 oval_w, Nlm_Int4 oval_h, Nlm_Boolean fill, Uint4 flags,
Nlm_Uint2 primID));

extern Nlm_PrimitivE Nlm_AddSegRect     PROTO((Nlm_SegmenT parent, 
Nlm_Boolean fill, Nlm_Uint2 primID));
extern Nlm_PrimitivE Nlm_AddSilentSegRect PROTO((Nlm_SegmenT parent, 
Nlm_Boolean fill, Nlm_Uint2 primID));
extern Nlm_PrimitivE Nlm_AddLine        PROTO((Nlm_SegmenT parent, 
Nlm_Int4 pnt1X, Nlm_Int4 pnt1Y, Nlm_Int4 pnt2X, Nlm_Int4 pnt2Y, 
Nlm_Boolean arrow, Nlm_Uint2 primID));
extern Nlm_PrimitivE Nlm_AddSymbol      PROTO((Nlm_SegmenT parent, 
Nlm_Int4 pntX, Nlm_Int4 pntY, Nlm_Int2 symbol, Nlm_Boolean fill, 
Nlm_Int2 align, Nlm_Uint2 primID));
extern Nlm_PrimitivE Nlm_AddBitmap      PROTO((Nlm_SegmenT parent, 
Nlm_Int4 pntX, Nlm_Int4 pntY, Nlm_Int2 width, Nlm_Int2 height, 
Nlm_Uint1Ptr data, Nlm_Int2 align, Nlm_Uint2 primID));
extern Nlm_PrimitivE Nlm_AddMarker      PROTO((Nlm_SegmenT parent, 
Nlm_Int4 pntX, Nlm_Int4 pntY, Nlm_Int2 length, Nlm_Int2 orient, 
Nlm_Uint2 primID));
extern Nlm_PrimitivE Nlm_AddLabel       PROTO((Nlm_SegmenT parent, 
Nlm_Int4 pntX, Nlm_Int4 pntY, const Nlm_Char* string, Nlm_Int2 size, 
Nlm_Int2 offset, Nlm_Int2 align, Nlm_Uint2 primID));
extern Nlm_PrimitivE Nlm_AddTextLabel   PROTO((Nlm_SegmenT parent, 
Nlm_Int4 pntX, Nlm_Int4 pntY, const Nlm_Char* string, Nlm_FonT font, 
Nlm_Int2 offset, Nlm_Int2 align, Nlm_Uint2 primID));
extern Nlm_PrimitivE Nlm_AddPolygon     PROTO((Nlm_SegmenT parent,
Nlm_Int2 vernum, Nlm_PntPtr points, Nlm_Boolean fill, Nlm_Uint2 primID));
extern Nlm_PrimitivE Nlm_AddArc         PROTO((Nlm_SegmenT parent, 
Nlm_Int4 pntX, Nlm_Int4 pntY, Nlm_Int4 radiusX, Nlm_Int4 radiusY, 
Nlm_Int4 start, Nlm_Int4 end, Nlm_Boolean fill, Nlm_Uint2 primID));
extern Nlm_PrimitivE AddOval            PROTO((Nlm_SegmenT parent, 
Nlm_Int4 pntX, Nlm_Int4 pntY, Nlm_Int4 radiusX, Nlm_Int4 radiusY, 
Nlm_Boolean fill, Nlm_Uint2 primID));

/* the pdc parameter in SntOnDrawProc is really Nlm_DrawInfoPtr from <mappingp.h> */

typedef void (*Nlm_SntOnDrawProc)(BigScalar calldata, Nlm_PrimDrawContext pdc);
typedef void (*Nlm_SntOnCleanupProc)(BigScalar calldata);
extern Nlm_PrimitivE Nlm_AddSntRectangle PROTO((Nlm_SegmenT parent,
 Nlm_Int4 left, Nlm_Int4 top, Nlm_Int4 right, Nlm_Int4 bottom,
 Nlm_SntOnDrawProc callback, BigScalar calldata, Nlm_SntOnCleanupProc cleanup,
 Nlm_Uint2 primID));


extern void          Nlm_RecalculateSegment PROTO((Nlm_SegmenT segment, 
Nlm_Int4 scaleX, Nlm_Int4 scaleY));
extern void          Nlm_OffsetSegment      PROTO((Nlm_SegmenT segment, 
Nlm_Int4 deltaX, Nlm_Int4 deltaY));
extern void          Nlm_LinkSegment        PROTO((Nlm_SegmenT parent, 
Nlm_SegmenT child));
extern void          Nlm_UnlinkSegment      PROTO((Nlm_SegmenT child));
extern void          Nlm_ExploreSegment     PROTO((Nlm_SegmenT segment, 
Nlm_VoidPtr userdata, Nlm_SegmentExploreProc callback));

/* SetSegmentVisibleFlag does not change the display in the viewer - use ShowSegment and HideSegment */
extern void          Nlm_SetSegmentVisibleFlag (Nlm_SegmenT segment, Nlm_Boolean visible);

/*Working with primitive */

extern Nlm_PrimitivE Nlm_GetPrimitive PROTO((Nlm_SegmenT segment, Nlm_Uint2 primCt));
extern void Nlm_SetPrimitiveIDs PROTO((Nlm_PrimitivE prim, Nlm_Uint2 entityID, Nlm_Uint4 itemID, Nlm_Uint2 itemtype, Nlm_Uint2 primID));
extern void Nlm_GetPrimitiveIDs PROTO((Nlm_PrimitivE prim, Nlm_Uint2Ptr entityIDPtr, Nlm_Uint4Ptr itemIDPtr, Nlm_Uint2Ptr itemtypePtr, Nlm_Uint2Ptr primIDPtr));
extern void Nlm_GetPrimDrawAttribute PROTO((Nlm_PrimitivE prim, 
Nlm_Uint1Ptr color, Nlm_Int1Ptr plinestyle, Nlm_Int1Ptr pshading, 
Nlm_Int1Ptr ppenwidth, Nlm_Int1Ptr pmode, Nlm_Int1Ptr phighlight));
extern void Nlm_ChangePrimAttribute PROTO((Nlm_PrimitivE prim, Nlm_Uint1 flags, Nlm_Uint1Ptr color, Nlm_Int1 linestyle, Nlm_Int1 shading, Nlm_Int1 penwidth, Nlm_Int1 mode));
extern void Nlm_OffsetPrim PROTO((Nlm_PrimitivE prim, Nlm_Int4 deltaX, Nlm_Int4 deltaY));
extern void Nlm_DeletePrim PROTO((Nlm_SegmenT parent, Nlm_PrimitivE prim));
extern Nlm_Boolean Nlm_PrimitiveBox PROTO((Nlm_PrimitivE prim, Nlm_Int4 scaleX, Nlm_Int4 scaleY, Nlm_BoxPtr pLimits));

/*
 *  The primitive adding order
 */
typedef enum
{
  /* check actual primitive adding order */
  WHAT_ORDER,
  /* add all subsequent new primitives to the end of primitive list */
  ADD_TO_TAIL,
  /* .......... to the head ..... */
  ADD_TO_HEAD,
  /* change the inserting order for the next added primitive only */
  ADD_TO_TAIL_ONCE,
  ADD_TO_HEAD_ONCE
}  Nlm_enumPrimAddOrder;

/*
 *  Reset the primitive adding order;  return previous adding order
 */
extern Nlm_enumPrimAddOrder Nlm_ChangeAddPrimOrder
  PROTO((Nlm_enumPrimAddOrder prim_add_order));
 


#define SegmenT Nlm_SegmenT
#define PrimitivE Nlm_PrimitivE
#define BoxInfo Nlm_BoxInfo
#define BoxPtr Nlm_BoxPtr
#define PntInfo Nlm_PntInfo
#define PntPtr Nlm_PntPtr
#define PrimDrawContext Nlm_PrimDrawContext
#define PrimHitContext Nlm_PrimHitContext
#define PrimDrawProc Nlm_PrimDrawProc
#define PrimPntHitTest Nlm_PrimPntHitTest
#define PrimHitTestProc Nlm_PrimHitTestProc
#define PrimOffsetProc Nlm_PrimOffsetProc
#define PrimGetLimitsProc Nlm_PrimGetLimitsProc
#define PrimCleanupProc Nlm_PrimCleanupProc
#define PrimDef Nlm_PrimDef
#define PrimDefPtr Nlm_PrimDefPtr
#define ResetPrimDef Nlm_ResetPrimDef
#define SetPrimDrawProc Nlm_SetPrimDrawProc
#define SetPrimHitTestProc Nlm_SetPrimHitTestProc
#define SetPrimOffsetProc Nlm_SetPrimOffsetProc
#define SetPrimGetLimitsProc Nlm_SetPrimGetLimitsProc
#define SetPrimCleanupProc Nlm_SetPrimCleanupProc
#define GetWorldWindow Nlm_GetWorldWindow
#define GetScaleX Nlm_GetScaleX
#define GetScaleY Nlm_GetScaleY
#define GetOffsetX Nlm_GetOffsetX
#define GetOffsetY Nlm_GetOffsetY
#define isPrimInWindow Nlm_isPrimInWindow
#define GetPrimHligh Nlm_GetPrimHligh
#define GetHitWindow Nlm_GetHitWindow
#define SetPrimAttribute Nlm_SetPrimAttribute
#define RestorePrimAttribute Nlm_RestorePrimAttribute
#define SegmentExploreProc Nlm_SegmentExploreProc
#define CreatePicture Nlm_CreatePicture
#define DeletePicture Nlm_DeletePicture
#define CreateSegment Nlm_CreateSegment
#define ResetSegment Nlm_ResetSegment
#define DeleteSegment Nlm_DeleteSegment
#define ParentSegment Nlm_ParentSegment
#define SegmentID Nlm_SegmentID
#define SegmentVisible Nlm_SegmentVisible
#define SegmentStyle Nlm_SegmentStyle
#define SegmentBox Nlm_SegmentBox
#define SetSegmentVisibleFlag Nlm_SetSegmentVisibleFlag
#define AddAttribute Nlm_AddAttribute
#define AddPrimitive Nlm_AddPrimitive
#define AddInvFrame Nlm_AddInvFrame
#define AddRectangle Nlm_AddRectangle
#define AddRoundedRectangle Nlm_AddRoundedRectangle
#define AddSegRect Nlm_AddSegRect
#define AddSilentSegRect Nlm_AddSilentSegRect
#define AddLine Nlm_AddLine
#define AddSymbol Nlm_AddSymbol
#define AddBitmap Nlm_AddBitmap
#define AddMarker Nlm_AddMarker
#define AddLabel Nlm_AddLabel
#define AddTextLabel Nlm_AddTextLabel
#define AddPolygon Nlm_AddPolygon
#define AddArc Nlm_AddArc
#define SntOnDrawProc Nlm_SntOnDrawProc
#define SntOnCleanupProc Nlm_SntOnCleanupProc
#define AddSntRectangle Nlm_AddSntRectangle
#define RecalculateSegment Nlm_RecalculateSegment
#define OffsetSegment Nlm_OffsetSegment
#define LinkSegment Nlm_LinkSegment
#define UnlinkSegment Nlm_UnlinkSegment
#define ExploreSegment Nlm_ExploreSegment
#define GetPrimitive Nlm_GetPrimitive
#define SetPrimitiveIDs Nlm_SetPrimitiveIDs
#define GetPrimitiveIDs Nlm_GetPrimitiveIDs
#define GetPrimDrawAttribute Nlm_GetPrimDrawAttribute
#define ChangePrimAttribute Nlm_ChangePrimAttribute
#define OffsetPrim Nlm_OffsetPrim
#define DeletePrim Nlm_DeletePrim
#define PrimitiveBox Nlm_PrimitiveBox
#define enumPrimAddOrder Nlm_enumPrimAddOrder
#define ChangeAddPrimOrder Nlm_ChangeAddPrimOrder

#ifdef __cplusplus
}
#endif

#endif
