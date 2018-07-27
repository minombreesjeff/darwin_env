/*   picture.c
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
* File Name:  picture.c
*
* Author:  Jonathan Kans, Alex Smirnov, Jill Shermer
*
* Version Creation Date:   10/23/92
*
* $Revision: 6.7 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: picture.c,v $
* Revision 6.7  2004/02/17 19:22:54  johnson
* make segment ID consistly unsigned
*
* Revision 6.6  2002/08/07 18:13:42  kans
* G/SetPrimitiveIDs, itemID is Uint4
*
* Revision 6.5  2002/03/07 15:53:45  kans
* added SetSegmentVisibleFlag to set/clear the visible flag before attaching to a viewer
*
* Revision 6.4  1999/10/13 17:45:46  kans
* added entityID, itemID, and itemtype to primitive internal structure, added Get and Set functions
*
* Revision 6.3  1999/10/04 17:16:33  kans
* include ncbidraw.h instead of vibrant.h, a couple Nlm_ prefixes
*
* Revision 6.2  1998/06/30 00:32:02  vakatov
* Nlm_AddAttribute():  pass "color" as constant
*
* Revision 6.1  1997/11/26 21:30:04  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.0  1997/08/25 18:56:22  madden
* Revision changed to 6.0
*
* Revision 5.2  1997/07/02 19:40:32  vakatov
* Fixed typo:  #DBUG --> #_DEBUG in DeletePrim()
*
* Revision 5.1  1997/07/02 18:35:51  vakatov
* Nlm_DeletePrim() redesigned/rewritten to guarantee prim. list consistency
*
* Revision 5.0  1996/05/28 13:45:08  ostell
* Set to revision 5.0
*
 * Revision 4.4  1996/04/01  16:07:10  hogue
 * Cast MemCpy to (void *)
 *
 * Revision 4.3  1996/03/18  20:42:05  vakatov
 * ChangeAddPrimOrder(...) function allows one to specify the order of adding
 * primitives(or segments) to a picture
 *
 * Revision 4.2  1995/12/14  17:29:30  kans
 * moved SearchSegment from viewer.c to picture.c, made extern
 *
 * Revision 4.1  1995/11/20  20:36:29  kans
 * line segments of circle now drawn with visible penwidth on Mac (AS)
 *
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 1.30  1995/05/17  15:15:14  kans
 * added Log line
 *
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

/*****************************************************************************
*
*   EXTERNAL VARIABLES
*
*****************************************************************************/

Uint1 BLACK_COLOR [3] = {0, 0, 0};
Uint1 WHITE_COLOR [3] = {255, 255, 255};
Uint1 RED_COLOR  [3] = {255, 0, 0};
Uint1 GREEN_COLOR [3] = {0, 255, 0};
Uint1 BLUE_COLOR [3] = {0, 0, 255};
Uint1 YELLOW_COLOR [3] = {255, 255, 0};
Uint1 CYAN_COLOR [3] = {0, 255, 255};
Uint1 MAGENTA_COLOR [3] = {255, 0, 255};

/*****************************************************************************
*
*   STATIC VARIABLES
*
*****************************************************************************/

static AttPData defAttPData = 
{ {0,0,0}, SOLID_LINE, SOLID_SHADING, STD_PEN_WIDTH, COPY_MODE };

/*****************************************************************************
*
*   CreatePicture (void)
*       Creates a Picture Record (same as Segment Record except for base.code)
*
*****************************************************************************/

SegmenT Nlm_CreatePicture (void)

{
  PicPPtr  pic;

  pic = (PicPPtr) MemNew (sizeof (PicPRec));
  if (pic != NULL) {
    pic->base.next = NULL;
    pic->base.prev = NULL;
    pic->base.code = PICTURE;
    pic->seg.box.left = INT4_MAX;
    pic->seg.box.top = INT4_MIN;
    pic->seg.box.right = INT4_MIN;
    pic->seg.box.bottom = INT4_MAX;
    pic->seg.head = NULL;
    pic->seg.tail = NULL;
    pic->seg.parent = NULL;
    pic->seg.visible = TRUE;
    pic->seg.maxscale = 0;
    pic->seg.penwidth = STD_PEN_WIDTH;
    pic->seg.highlight = PLAIN_SEGMENT;
    pic->seg.segID = 0;
    pic->attLast = defAttPData;
  }
  return (SegmenT) pic;
}

/*****************************************************************************
*
*   DeletePicture (picture)
*       Removes any previously existing segments, then frees picture node
*
*****************************************************************************/

SegmenT Nlm_DeletePicture (SegmenT picture)

{
  PicPPtr  pic;

  pic = NULL;
  if (picture != NULL) {
    pic = (PicPPtr) picture;
    if (pic->base.code == PICTURE) {
      ResetSegment ((SegmenT) pic);
      pic = (PicPPtr) MemFree(pic);
    } else {
      Message (MSG_ERROR, "DeletePicture argument not a picture");
    }
  }
  return (SegmenT) pic;
}

/*****************************************************************************
*
*   DeleteSegment (segment)
*       Removes any previously existing segments, then frees segment node
*
*****************************************************************************/

SegmenT Nlm_DeleteSegment (SegmenT segment)

{
  SegPPtr  seg;

  seg = NULL;
  if (segment != NULL) {
    seg = (SegPPtr) segment;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      ResetSegment ((SegmenT) seg);
      seg = (SegPPtr) MemFree(seg);
    } else {
      Message (MSG_ERROR, "DeleteSegment argument not a segment or picture");
    }
  }
  return (SegmenT) seg;
}

/*****************************************************************************
*
*   CreatePrimitive (parent, code, size)
*       Common routine creates any primitive type, then adds it to the
*       parent segment's child list, updating segment pointers
*
*****************************************************************************/

static Nlm_enumPrimAddOrder actualAddPrimOrder = ADD_TO_TAIL;
static Nlm_enumPrimAddOrder tempAddPrimOrder   = WHAT_ORDER;

Nlm_enumPrimAddOrder Nlm_ChangeAddPrimOrder(Nlm_enumPrimAddOrder prim_add_order)
{
  Nlm_enumPrimAddOrder prev_order = (tempAddPrimOrder == WHAT_ORDER) ?
    actualAddPrimOrder : tempAddPrimOrder;

  switch ( prim_add_order )
    {
    case WHAT_ORDER:
      break;
    case ADD_TO_TAIL:
    case ADD_TO_HEAD:
      actualAddPrimOrder = prim_add_order;
      tempAddPrimOrder   = WHAT_ORDER;
      break;
    case ADD_TO_TAIL_ONCE:
    case ADD_TO_HEAD_ONCE:
      tempAddPrimOrder = prim_add_order;
      break;
    }

  return prev_order;
}

static BasePPtr CreatePrimitive (SegmenT parent, Int1 code, size_t size)

{
  BasePPtr  prim;
  SegPPtr   prnt;

  prim = NULL;
  if (parent != NULL) {
    prnt = (SegPPtr) parent;
    if (prnt->base.code == SEGMENT || prnt->base.code == PICTURE) {
      prim = (BasePPtr) MemNew (size);
      if (prim != NULL) {
	Nlm_enumPrimAddOrder order = Nlm_ChangeAddPrimOrder( WHAT_ORDER );
	tempAddPrimOrder = WHAT_ORDER;

	prim->prev = NULL;
        prim->next = NULL;
        prim->code = code;
        if (prnt->seg.head != NULL && prnt->seg.tail != NULL) {
	  if (order == ADD_TO_HEAD  ||  order == ADD_TO_HEAD_ONCE)
	    {
	      prnt->seg.head->prev = prim;
	      prim->next = prnt->seg.head;
	      prnt->seg.head = prim;
	    }
	  else
	    {
	      prnt->seg.tail->next = prim;
	      prim->prev = prnt->seg.tail;
	      prnt->seg.tail = prim;
	    }
        } else if (prnt->seg.head == NULL && prnt->seg.tail == NULL) {
          prnt->seg.head = prim;
          prnt->seg.tail = prim;
        } else {
          Message (MSG_ERROR, "CreatePrimitive list integrity problem");
        }
      }
    } else {
      Message (MSG_ERROR, "CreatePrimitive parent not a segment or picture");
    }
  }
  return prim;
}

/*****************************************************************************
*
*   LinkSegment (parent, child)
*       Links existing segment into parent
*
*****************************************************************************/

void Nlm_LinkSegment (SegmenT parent, SegmenT child)

{
  BasePPtr  prev;
  BasePPtr  prim;
  SegPPtr   prnt;
  SegPPtr   seg;

   if (parent != NULL && child != NULL) {
    prnt = (SegPPtr) parent;
    if (prnt->base.code == SEGMENT || prnt->base.code == PICTURE) {
      prim = (BasePPtr) child;
      seg = (SegPPtr) prim;
      if (prim->code == SEGMENT) {
        if (seg->seg.parent == NULL) {
          seg->seg.parent = (BasePPtr) parent;
          if (prnt->seg.head != NULL && prnt->seg.tail != NULL) {
            prev = prnt->seg.tail;
            prev->next = prim;
            prim->prev = prev;
            prnt->seg.tail = prim;
          } else if (prnt->seg.head == NULL && prnt->seg.tail == NULL) {
            prnt->seg.head = prim;
            prnt->seg.tail = prim;
            prim->prev = NULL;
          } else {
            Message (MSG_ERROR, "LinkSegment list integrity problem");
          }
        } else {
          Message (MSG_ERROR, "LinkSegment child already linked");
        }
      } else {
        Message (MSG_ERROR, "LinkSegment child not a segment");
      }
    } else {
      Message (MSG_ERROR, "LinkSegment parent not a segment or picture");
    }
  }
}

/*****************************************************************************
*
*   UnlinkSegment (child)
*       Unlinks existing segment from parent
*
*****************************************************************************/

void Nlm_UnlinkSegment (SegmenT child)

{
  BasePPtr  item;
  BasePPtr  last;
  BasePPtr  next;
  BasePPtr  prim;
  SegPPtr   prnt;
  SegPPtr   seg;

  if (child != NULL) {
    prim = (BasePPtr) child;
    if (prim->code == SEGMENT) {
      seg = (SegPPtr) prim;
      prnt = (SegPPtr) seg->seg.parent;
      if (prnt != NULL) {
        last = NULL;
        item = prnt->seg.head;
        while (item != NULL) {
          if (item == prim) {
            seg->seg.parent = NULL;
            next = item->next;
            if (last != NULL) {
              last->next = next;
            }
            if ( next != NULL ) {
              next->prev = last;
            }
            item->next = NULL;
            item = NULL;
            if (prnt->seg.head == prim) {
              prnt->seg.head = next;
            }
            if (prnt->seg.tail == prim) {
              prnt->seg.tail = last;
            }
          } else {
            last = item;
            item = item->next;
          }
        }
      }
    } else {
      Message (MSG_ERROR, "UnlinkSegment child not a segment");
    }
  }
}

/*****************************************************************************
*
*   CreateSegment (parent, segID, maxScale)
*       Creates a segment primitive that can have its own list of children
*
*****************************************************************************/

SegmenT Nlm_CreateSegment (SegmenT parent, Uint2 segID, Int4 maxScale)
{
  BasePPtr  prim;
  SegPPtr   prnt;
  SegPPtr   seg;

  if (parent != NULL) {
    seg = (SegPPtr) CreatePrimitive (parent, SEGMENT, sizeof (SegPRec));
  } else {
    prim = (BasePPtr) MemNew (sizeof (SegPRec));
    if (prim != NULL) {
      prim->next = NULL;
      prim->prev = NULL;
      prim->code = SEGMENT;
    }
    seg = (SegPPtr) prim;
  }
  if (seg != NULL) {
    seg->seg.box.left = INT4_MAX;
    seg->seg.box.top = INT4_MIN;
    seg->seg.box.right = INT4_MIN;
    seg->seg.box.bottom = INT4_MAX;
    seg->seg.head = NULL;
    seg->seg.tail = NULL;
    seg->seg.parent = (BasePPtr) parent;
    seg->seg.visible = TRUE;
    seg->seg.maxscale = maxScale;
    prnt = (SegPPtr) parent;
    seg->seg.penwidth = STD_PEN_WIDTH;
    seg->attLast = defAttPData;
    if (prnt != NULL) {
      if (prnt->base.code == SEGMENT || prnt->base.code == PICTURE) {
        seg->seg.penwidth = prnt->seg.penwidth;
        seg->attLast = prnt->attLast;
      }
    }
    seg->seg.highlight = PLAIN_SEGMENT;
    seg->seg.segID = segID;
  }
  return (SegmenT) seg;
}

/*****************************************************************************
*
*   ResetSegment (segment)
*       Cleaves and deletes the segment's child list (recursively),
*       freeing item specific data, and leaving the segment itself
*       intact for repopulation
*
*****************************************************************************/

SegmenT Nlm_ResetSegment (SegmenT segment)

{
  BasePPtr  item;
  BasePPtr  next;
  SegPPtr   seg;

  seg = NULL;
  if (segment != NULL) {
    seg = (SegPPtr) segment;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      item = seg->seg.head;
      while (item != NULL) {
        next = item->next;
        switch (item->code) {
          case PICTURE :
          case SEGMENT :
            ResetSegment ((SegmenT) item);
            break;
          default :
            CleanupPrimitive (item);
            break;
        }
        MemFree (item);
        item = next;
      }
      seg->seg.box.left = INT4_MAX;
      seg->seg.box.top = INT4_MIN;
      seg->seg.box.right = INT4_MIN;
      seg->seg.box.bottom = INT4_MAX;
      seg->seg.head = NULL;
      seg->seg.tail = NULL;
      seg->seg.penwidth = STD_PEN_WIDTH;
      seg->seg.highlight = PLAIN_SEGMENT;
      seg->attLast = defAttPData;
    } else {
      Message (MSG_ERROR, "ResetSegment argument not a segment or picture");
    }
  }
  return (SegmenT) seg;
}

/*****************************************************************************
*
*   ParentSegment (segment)
*       Returns the parent of a segment
*
*****************************************************************************/

SegmenT Nlm_ParentSegment (SegmenT segment)

{
  SegmenT  parent;
  SegPPtr  seg;

  parent = NULL;
  if (segment != NULL) {
    seg = (SegPPtr) segment;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      parent = (SegmenT) seg->seg.parent;
      if (parent != NULL) {
        seg = (SegPPtr) parent;
        if (seg->base.code != SEGMENT && seg->base.code != PICTURE) {
          Message (MSG_ERROR, "ParentSegment parent not a segment or picture");
        }
      }
    } else {
      Message (MSG_ERROR, "ParentSegment argument not a segment or picture");
    }
  }
  return parent;
}

/*****************************************************************************
*
*   SegmentID (segment)
*       Returns the segment ID of a segment
*
*****************************************************************************/

Uint2 Nlm_SegmentID (SegmenT segment)

{
  SegPPtr  seg;
  Uint2     segID;

  segID = 0;
  if (segment != NULL) {
    seg = (SegPPtr) segment;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      segID = seg->seg.segID;
    } else {
      Message (MSG_ERROR, "SegmentID argument not a segment or picture");
    }
  }
  return segID;
}

/*****************************************************************************
*
*   SegmentVisible (segment)
*       Returns the visibility of a segment
*
*****************************************************************************/

Boolean Nlm_SegmentVisible (SegmenT segment)

{
  SegPPtr  seg;
  Boolean  vis;

  vis = FALSE;
  if (segment != NULL) {
    seg = (SegPPtr) segment;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      vis = seg->seg.visible;
    } else {
      Message (MSG_ERROR, "SegmentVisible argument not a segment or picture");
    }
  }
  return vis;
}

/*****************************************************************************
*
*   Nlm_SetSegmentVisibleFlag (segment, visible)
*       Sets the visibility of a segment
*
*****************************************************************************/

void Nlm_SetSegmentVisibleFlag (SegmenT segment, Boolean visible)

{
  SegPPtr  seg;

  if (segment != NULL) {
    seg = (SegPPtr) segment;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      seg->seg.visible = visible;
    } else {
      Message (MSG_ERROR, "Nlm_SetSegmentVisibleFlag argument not a segment or picture");
    }
  }
}

/*****************************************************************************
*
*   SegmentStyle (segment)
*       Returns the highlight style of a segment
*
*****************************************************************************/

Int1 Nlm_SegmentStyle (SegmenT segment)

{
  Int1     highlight;
  SegPPtr  seg;

  highlight = PLAIN_SEGMENT;
  if (segment != NULL) {
    seg = (SegPPtr) segment;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      highlight = seg->seg.highlight;
    } else {
      Message (MSG_ERROR, "SegmentStyle argument not a segment or picture");
    }
  }
  return highlight;
}

/*****************************************************************************
*
*   SegmentBox (segment, box)
*       Returns the bounding box of a segment
*
*****************************************************************************/

void Nlm_SegmentBox (SegmenT segment, BoxPtr box)

{
  SegPPtr  seg;

  if (segment != NULL) {
    seg = (SegPPtr) segment;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      if (box != NULL) {
        *box = seg->seg.box;
      }
    } else {
      Message (MSG_ERROR, "SegmentBox argument not a segment or picture");
    }
  }
}

/*****************************************************************************
*
*   GetPrimitive (segment, primCt)
*       Gets a primitive by index within a segment
*
*****************************************************************************/

PrimitivE Nlm_GetPrimitive (SegmenT segment, Uint2 primCt)

{
  BasePPtr  item;
  SegPPtr   seg;

  item = NULL;
  if (segment != NULL && primCt > 0) {
    seg = (SegPPtr) segment;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      item = seg->seg.head;
      while (item != NULL && primCt > 1) {
        item = item->next;
        primCt--;
      }
    } else {
      Message (MSG_ERROR, "FindPrimitive argument not a segment or picture");
    }
  }
  return (PrimitivE) item;
}

void Nlm_SetPrimitiveIDs (PrimitivE prim, Uint2 entityID, Uint4 itemID,
                          Uint2 itemtype, Uint2 primID)

{
  GenPPtr   gpp;
  BasePPtr  item;

  item = (BasePPtr) prim;
  if (item != NULL && item->code == GENERIC) {
    gpp = (GenPPtr) item;
    gpp->entityID = entityID;
    gpp->itemID = itemID;
    gpp->itemtype = itemtype;
    gpp->primID = primID;
  }
}

void Nlm_GetPrimitiveIDs (PrimitivE prim, Uint2Ptr entityIDPtr, Uint4Ptr itemIDPtr,
                          Uint2Ptr itemtypePtr, Uint2Ptr primIDPtr)

{
  GenPPtr   gpp;
  BasePPtr  item;
  item = (BasePPtr) prim;
  if (item != NULL && item->code == GENERIC) {
    gpp = (GenPPtr) item;
    if (entityIDPtr != NULL) {
      *entityIDPtr = gpp->entityID;
    }
    if (itemIDPtr != NULL) {
      *itemIDPtr = gpp->itemID;
    }
    if (itemtypePtr != NULL) {
      *itemtypePtr = gpp->itemtype;
    }
    if (primIDPtr != NULL) {
      *primIDPtr = gpp->primID;
    }
  }
}

/*****************************************************************************
*
*   AdjustParent (parent, left, top, right, bottom)
*       Recalculates segment boundaries, propagates up parent list
*
*****************************************************************************/

static void AdjustParent (SegmenT parent, Int4 left, Int4 top, Int4 right, Int4 bottom)

{
  SegPPtr  seg;

  if (parent != NULL) {
    seg = (SegPPtr) parent;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      if (seg->seg.box.left > left) {
        seg->seg.box.left = left;
      }
      if (seg->seg.box.top < top) {
        seg->seg.box.top = top;
      }
      if (seg->seg.box.right < right) {
        seg->seg.box.right = right;
      }
      if (seg->seg.box.bottom > bottom) {
        seg->seg.box.bottom = bottom;
      }
      if ( seg->base.code == SEGMENT ) {
        AdjustParent ((SegmenT) seg->seg.parent, left, top, right, bottom);
      }
    } else {
      Message (MSG_ERROR, "AdjustParent argument not a segment or picture");
    }
  }
}

/*****************************************************************************
*
*   AddAttribute (parent, flags, color, linestyle, shading, penwidth, mode)
*       Changes current attribute settings (applied only to lower levels)
*
*****************************************************************************/

void Nlm_AddAttribute(SegmenT parent, Uint1 flags, const Uint1* color,
                      Int1 linestyle, Int1 shading, Int1 penwidth, Int1 mode)

{
  AttPPtr  att;
  SegPPtr  prnt;

  prnt = (SegPPtr) parent;
  if (prnt != NULL) {
    prnt->seg.penwidth = penwidth;
    att = &(prnt->attLast);
    if (linestyle >= NO_LINE_STYLE && linestyle <= DASHED_LINE) {
      if (shading >= NO_SHADING && shading <= THICK_NWSE_SHADING) {
        if ( flags & COLOR_ATT ) {
          if (color != NULL) {
            att->color [0] = color [0];
            att->color [1] = color [1];
            att->color [2] = color [2];
          } else {
            att->color [0] = BLACK_COLOR [0];
            att->color [1] = BLACK_COLOR [1];
            att->color [2] = BLACK_COLOR [2];
          }
        }
        if ( flags & STYLE_ATT ) att->linestyle = linestyle;
        if ( flags & SHADING_ATT ) att->shading = shading;
        if ( flags & WIDTH_ATT ) {
          att->penwidth = MIN (penwidth, PEN_MAX);
          if ( att->penwidth <= 0 ) att->penwidth = 1;
        }
        if ( flags & MODE_ATT ) att->mode = mode;
      } else {
        Message (MSG_ERROR, "AddAttribute shading out of range");
      }
    } else {
      Message (MSG_ERROR, "AddAttribute line style out of range");
    }
  } else {
    Message (MSG_ERROR, "AddAttribute parent is NULL");
  }
}

/*****************************************************************************
*
*   AddPrimitive (pdp, parent, primID, pextra, extrasize )
*
*****************************************************************************/

PrimitivE Nlm_AddPrimitive (PrimDefPtr pdp, SegmenT parent, Uint2 primID, 
                        VoidPtr pextra, Int2 extrasize )
{
  GenPPtr  gpp = NULL;
  BoxInfo  pLimits;

  if (pdp == NULL) {
    Message (MSG_ERROR, "Primitive definition ptr is NULL");
  } else {
    gpp = (GenPPtr) CreatePrimitive (parent, GENERIC, 
                    sizeof (GenPRec) + MAX(0,extrasize-sizeof(double)));
    if (gpp != NULL) {
      gpp->primID = primID;
      gpp->pdp = pdp;
      if (pextra != NULL && extrasize > 0 ) {
        MemCpy ((void *)&gpp->data, (void *)pextra, (size_t)extrasize);
      }
      gpp->highlight = PLAIN_SEGMENT;
      gpp->att = ((SegPPtr)parent)->attLast;
      TryGetPrimitiveLimits ((BasePPtr)gpp, 1, 1, &pLimits );
      AdjustParent (parent,
                    pLimits.left, pLimits.top,
                    pLimits.right, pLimits.bottom);
    }
  }
  return (PrimitivE) gpp;
}

static void RecalSegment (SegPPtr seg, Int4 scaleX, Int4 scaleY )

{
  BasePPtr item;
  BoxInfo  pLimits;

  if (seg != NULL) {
    seg->seg.box.left = INT4_MAX;
    seg->seg.box.top = INT4_MIN;
    seg->seg.box.right = INT4_MIN;
    seg->seg.box.bottom = INT4_MAX;
    item = seg->seg.head;
    while (item != NULL) {
      switch (item->code) {
        case PICTURE :
        case SEGMENT :
          RecalSegment ((SegPPtr)item, scaleX, scaleY);
          break;
        case GENERIC :
          TryGetPrimitiveLimits ( item, scaleX, scaleY, &pLimits );
          AdjustParent ((SegmenT)seg,
                        pLimits.left, pLimits.top,
                        pLimits.right, pLimits.bottom);
      }
      item = item->next;
    }
  }
}

/*****************************************************************************
*
*   RecalculateSegment (segment, scaleX, scaleY)
*       Explores the segment's child list (recursively), recalculating segment
*       boundaries and scaled margins, propagating back up parent list
*
*****************************************************************************/

void Nlm_RecalculateSegment (SegmenT segment, Int4 scaleX, Int4 scaleY )

{
  SegPPtr     seg;

  if (segment != NULL) {
    seg = (SegPPtr) segment;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      RecalSegment (seg, scaleX, scaleY);
    } else {
      Message (MSG_ERROR, "RecalculateSegment argument not a segment or picture");
    }
  }
}

/*****************************************************************************
*
*   OffsetSegment (segment, deltaX, deltaY)
*
*****************************************************************************/

void Nlm_OffsetSegment (SegmenT segment, Int4 deltaX, Int4 deltaY)

{
  BasePPtr  item;
  SegPPtr   seg;

  if (segment != NULL) {
    item = (BasePPtr)segment;
    switch (item->code) {
      case PICTURE :
      case SEGMENT :
        seg = (SegPPtr) segment;
        seg->seg.box.left += deltaX;
        seg->seg.box.top += deltaY;
        seg->seg.box.right += deltaX;
        seg->seg.box.bottom += deltaY;
        item = seg->seg.head;
        while (item != NULL) {
          switch (item->code) {
            case PICTURE :
            case SEGMENT :
              Nlm_OffsetSegment ((SegmenT)item, deltaX, deltaY );
              break;
            case GENERIC :
              TryOffsetPrimitive (item, deltaX, deltaY);
          }
          item = item->next;
        }

    }
  }
}

/*****************************************************************************
*
*   ExploreSegment (segment, userdata, callback)
*       Explores the segment's child list (recursively)
*
*****************************************************************************/

static Boolean ExploreSegmentProc (SegmenT segment, VoidPtr userdata, SegmentExploreProc callback)

{
  Boolean   goOn;
  GenPPtr   gpp;
  BasePPtr  item;
  BasePPtr  next;
  Uint2     primCt;
  SegPPtr   seg;

  goOn = FALSE;
  if (segment != NULL && callback != NULL) {
    seg = (SegPPtr) segment;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      goOn = callback (segment, NULL, seg->seg.segID, 0, 0, userdata);
      item = seg->seg.head;
      primCt = 0;
      while (item != NULL && goOn) {
        primCt++;
        next = item->next;
        switch (item->code) {
          case PICTURE :
          case SEGMENT :
            goOn = ExploreSegmentProc ((SegmenT) item, userdata, callback);
            break;
          case GENERIC :
            gpp = (GenPPtr) item;
            goOn = callback (segment, (PrimitivE) item, seg->seg.segID,
                             gpp->primID, primCt, userdata);
            break;
          default :
            break;
        }
        item = next;
      }
    } else {
      Message (MSG_ERROR, "ExploreSegment argument not a segment or picture");
    }
  }
  return goOn;
}

void Nlm_ExploreSegment (SegmenT segment, VoidPtr userdata, SegmentExploreProc callback)

{
  ExploreSegmentProc (segment, userdata, callback);
}

/*****************************************************************************
*
*   SearchSegment (segment, pt, pnt, scale, prID, prCt)
*       Returns the deepest segment that contains the pnt in world coordinates
*
*****************************************************************************/

SegmenT SearchSegment (SegmenT segment, ScalePtr scalePtr, PrimitivE PNTR prPtr )
{
  SegmenT   foundSeg = NULL;
  BasePPtr  item;
  SegPPtr   seg;
  ScaleInfo scale;

  if (segment != NULL && scalePtr != NULL ) {
    seg = (SegPPtr) segment;
    if (seg->base.code == SEGMENT || seg->base.code == PICTURE) {
      scale = *scalePtr;
      if (seg->seg.maxscale == 0 || 
          seg->seg.maxscale >= MAX (scale.scaleX, scale.scaleY)) {
        if ( (seg->seg.box.left <= scale.worldWindow.right) &&
             (seg->seg.box.right >= scale.worldWindow.left) &&
             (seg->seg.box.top >= scale.worldWindow.bottom) &&
             (seg->seg.box.bottom <= scale.worldWindow.top) ){
          if (seg->seg.visible) {
            item = seg->seg.tail;
            while (item != NULL && foundSeg == NULL) {
              switch (item->code) {
                case GENERIC :
                  if (PrimitiveIsCloseToPoint (item, &scale)) {
                    foundSeg = (SegmenT) seg;
                    *prPtr = (PrimitivE)item;
                  }
                  break;
                case SEGMENT :
                  foundSeg = SearchSegment ((SegmenT)item, &scale, prPtr);
                  break;
                default : 
                  Message (MSG_ERROR, "FindSegment child is a picture");
              }
              item = item->prev;
            }
          }
        }
      }
    } else {
      Message (MSG_ERROR, "SearchSegment argument not a segment or picture");
    }
  }
  return foundSeg;
}

/*****************************************************************************
*
*  ChangePrimAttribute (prim, flags, color, linestyle, shading, penwidth, mode)
*
*****************************************************************************/

void Nlm_ChangePrimAttribute (PrimitivE prim, Uint1 flags, Uint1Ptr color, 
       Int1 linestyle, Int1 shading, Int1 penwidth, Int1 mode) 
{
  GenPPtr  gpp = (GenPPtr)prim;
  AttPPtr  att;

  if ( gpp ) {
    att = &(gpp->att);
    if (linestyle >= NO_LINE_STYLE && linestyle <= DASHED_LINE) {
      if (shading >= NO_SHADING && shading <= THICK_NWSE_SHADING) {
        if ( flags & COLOR_ATT ) {
          if (color != NULL) {
            att->color [0] = color [0];
            att->color [1] = color [1];
            att->color [2] = color [2];
          } else {
            att->color [0] = BLACK_COLOR [0];
            att->color [1] = BLACK_COLOR [1];
            att->color [2] = BLACK_COLOR [2];
          }
        }
        if ( flags & STYLE_ATT ) att->linestyle = linestyle;
        if ( flags & SHADING_ATT ) att->shading = shading;
        if ( flags & WIDTH_ATT ) att->penwidth = MIN (penwidth, PEN_MAX);
        if ( flags & MODE_ATT ) att->mode = mode;
      } else {
        Message (MSG_ERROR, "ChangePrimAttribute shading out of range");
      }
    } else {
      Message (MSG_ERROR, "ChangePrimAttribute line style out of range");
    }
  } else {
    Message (MSG_ERROR, "ChangePrimAttribute primitive is NULL");
  }
}

/*****************************************************************************
*
*   OffsetPrim ( prim, deltaX, deltaY)
*
*****************************************************************************/

void Nlm_OffsetPrim(PrimitivE prim, Int4 deltaX, Int4 deltaY)
{
  BasePPtr item = (BasePPtr)prim;

  if (item  &&  item->code == GENERIC)
    TryOffsetPrimitive(item, deltaX, deltaY);
}


/*****************************************************************************
*
*   DeletePrim ( prim )
*
*****************************************************************************/

void Nlm_DeletePrim(Nlm_SegmenT parent, Nlm_PrimitivE prim)
{
  BasePPtr      item = (BasePPtr)prim;
  Nlm_SegPData *seg  = &((SegPPtr)parent)->seg;

  if ( !item )
    return;

#ifdef _DEBUG
  {{
    BasePPtr temp = seg->head;
    for ( ;  temp  &&  temp != item;  temp = temp->next);
    ASSERT ( temp == item );
  }}
#endif

  if (item == seg->head)
    seg->head = item->next;
  if (item == seg->tail)
    seg->tail = item->prev;

  if ( item->prev )
    item->prev->next = item->next;
  if ( item->next )
    item->next->prev = item->prev;

  CleanupPrimitive( item );
  MemFree( item );

  /* It would be nice to call Nlm_RecalculateSegment() here, but:
   *  1) we don't know the scaling factors;
   *  2) it can be rather time-consuming if a lot of primitives get
   *     deleted at once.
   *  Ergo:  that's up to the user to recalc the seg. Take care...
   */
}

