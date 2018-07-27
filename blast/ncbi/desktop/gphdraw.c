/*
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
* File Name: gphdraw.c
*
* Author(s): Jonathan Kans, John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.10 $
*
* File Description: sentinel graphs
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: gphdraw.c,v $
* Revision 6.10  2000/07/14 19:47:38  kans
* allow Phred Quality along with Phrap Quality
*
* Revision 6.9  1999/10/04 17:46:23  kans
* include vibrant.h
*
* Revision 6.8  1999/03/12 14:23:34  kuzio
* cast
*
* Revision 6.7  1998/10/13 17:13:56  kuzio
* colored superimposable graphs
*
* Revision 6.6  1998/09/16 19:00:35  kuzio
* cvs logs
*
* ==========================================================================
*/

#include <vibrant.h>
#include <sequtil.h>
#include <drawingp.h>
#include <gphdraw.h>

static void PlotTheWorkingArray (Int4Ptr xp, Int4 len, Int4 scaleX,
                                 Int4 gmin, RecT r, Uint1 uR, Uint1 uG,
                                 Uint1 uB, AttPData PNTR curattrib)
{
  PoinT          pt;
  Int4           i;
  Int4           j;
  Int4           max;
  Int4           min;
  Int4           val;
  Uint1          curR, curG, curB;

  if (curattrib != NULL)
  {
    curR = curattrib->color[0];
    curG = curattrib->color[1];
    curB = curattrib->color[2];
  }
  else
  {
    curR = 0;
    curG = 0;
    curB = 0;
  }

  SelectColor (uR, uG, uB);
  pt.x = r.left;
  i = 0;
  val = (Int4) *xp;
  pt.y = (Int2) (r.bottom - (Int2) (val - gmin));
  if (pt.y < r.top) {
    pt.y = r.top;
  }
  MoveTo (pt.x, pt.y);
  for (; i < len - scaleX; i += scaleX) {
    val = (Int4) *xp;
    min = (Int4) val;
    max = (Int4) val;
    for (j = 1; j < scaleX; j++) {
      xp++;
      val = (Int4) *xp;
      min = MIN (min, (Int4) val);
      max = MAX (max, (Int4) val);
    }
    xp++;
    pt.y = (Int2) (r.bottom - (Int2) (min - gmin));
    if (pt.y < r.top) {
      pt.y = r.top;
    }
    LineTo (pt.x, pt.y);
    pt.y = (Int2) (r.bottom - (Int2) (max - gmin));
    if (pt.y < r.top) {
      pt.y = r.top;
    }
    LineTo (pt.x, pt.y);
    (pt.x)++;
  }
  SelectColor (curR, curG, curB);
  return;
}

static Int4Ptr MakeWorkingSeqGraphInt4Array (Pointer data, Uint1 type,
                                             Int4 pos, Int4 len,
                                             Boolean usescaleflags,
                                             FloatHi a, FloatHi b)
{
  Int4          i;
  Int4Ptr       xpoints, xp;
  FloatHi       fval;
  Int4          ival;
  Byte          bval;
  ByteStorePtr  bs;
  BytePtr       bp;

  xpoints = (Int4Ptr) MemNew ((size_t) (sizeof (Int4) * (len + 10)));

  if (xpoints == NULL)
    return xpoints;

  xp = xpoints;
  switch (type) {
   default:
   case 1:
    for (i = 0; i < len; i++, pos++, xp++) {
      if (usescaleflags) {
        fval = a * (FloatHi) (((FloatHiPtr) data)[pos]) + b;
      } else {
        fval = (FloatHi) (((FloatHiPtr) data)[pos]);
      }
      *xp = (Int4) fval;
    }
    break;
   case 2:
    for (i = 0; i < len; i++, pos++, xp++) {
      if (usescaleflags) {
        ival = (Int4) (a * (FloatHi) (((Int4Ptr) data)[pos]) + b);
      } else {
        ival = (Int4) (((Int4Ptr) data)[pos]);
      }
      *xp = (Int4) ival;
    }
    break;
   case 3:
    bp = MemNew (sizeof (Byte) * (len + 10));
    bs = (ByteStorePtr) data;
    BSSeek (bs, pos, SEEK_SET);
    len = BSRead (bs, (Pointer) bp, len * sizeof (Byte));
    for (i = 0; i < len; i++, pos++, xp++) {
      if (usescaleflags) {
        bval = (Byte) (a * (FloatHi) (bp [i]) + b);
      } else {
        bval = (Byte) (bp [i]);
      }
      *xp = (Int4) bval;
    }
    MemFree (bp);
    break;
  }
  return xpoints;
}

static void GraphSentProc (BigScalar calldata, PrimDrawContext pdc)
{
  BioseqPtr      bsp;
  Int4           curScale;
  DrawInfoPtr    dInfoPtr;
  GraphSentPtr   gsp;
  Int4           left;
  Int4           len, pos;
  RecT           r;
  Int4           scaleX;
  Int4           scaleY;
  SeqGraphPtr    sgp;
  BoxInfo        tmpBox;
  Int4Ptr        xpoints;
  Int4           tmpscaleX;

  gsp = (GraphSentPtr) calldata;
  if (gsp == NULL) return;
  sgp = gsp->sgp;
  if (sgp == NULL || sgp->values == NULL) return;

  dInfoPtr = (DrawInfoPtr) pdc;
  tmpBox = gsp->box;
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
  scaleX = dInfoPtr->scale.scaleX;
  scaleY = dInfoPtr->scale.scaleY;

  curScale = dInfoPtr->scale.scaleX;
  r.left   = (Int2)((dInfoPtr->scale.offsetX + tmpBox.left )  / curScale);
  r.right  = (Int2)((dInfoPtr->scale.offsetX + tmpBox.right)  / curScale);
  curScale = dInfoPtr->scale.scaleY;
  r.top    = (Int2)((dInfoPtr->scale.offsetY - tmpBox.top   ) / curScale);
  r.bottom = (Int2)((dInfoPtr->scale.offsetY - tmpBox.bottom) / curScale);

  bsp = BioseqFind (SeqLocId (sgp->loc));
  left = GetOffsetInBioseq (sgp->loc, bsp, SEQLOC_LEFT_END);

  if (gsp->flagIsGUI)
  {
    len = tmpBox.right - tmpBox.left;
    pos = tmpBox.left - left;
  }
  else
  {
/* for non-GUI, plot all the sgp values */
    len = sgp->numval;
    pos = 0;
  }

  xpoints = MakeWorkingSeqGraphInt4Array (sgp->values, sgp->flags[2],
                                          pos, len,
                                          (Boolean) (sgp->flags[1] != 0),
                                          gsp->a, gsp->b);
  if (!gsp->flagIsGUI)
  {
    tmpscaleX = sgp->numval / (Int4) (tmpBox.right - tmpBox.left);
    if (tmpscaleX > scaleX)
      scaleX = tmpscaleX;
  }

  PlotTheWorkingArray (xpoints, len, scaleX, gsp->min, r,
                       gsp->red, gsp->green, gsp->blue,
                       &(dInfoPtr->curattrib));

  MemFree (xpoints);
}

static void CleanGSP (BigScalar calldata)
{
  GraphSentPtr  gsp;

  gsp = (GraphSentPtr) calldata;
  MemFree (gsp);
}

extern GraphSentPtr AddGraphSentinelToPicture (SeqGraphPtr sgp, BioseqPtr bsp,
                                               SegmenT pict, Int4 scaleX,
                                               Int4 top, Int2 start,
                                               Uint1Ptr uRGB)
{
  Int4          axis;
  GraphSentPtr  gsp;
  Int4          i;
  Boolean       is_phrap;
  Int4          max;
  Int4          min;
  SegmenT       seg;
  Char          str [32];
  Int4          leftoff, rightoff;

  if (sgp == NULL || bsp == NULL || pict == NULL)
    return NULL;
  gsp = MemNew (sizeof (GraphSentData));
  if (gsp == NULL)
    return NULL;
  gsp->flagIsGUI = VibrantIsGUI ();
  if (uRGB != NULL)
  {
    gsp->red   = uRGB[0];
    gsp->green = uRGB[1];
    gsp->blue  = uRGB[2];
  }
  else
  {
    gsp->red   = 0;
    gsp->green = 0;
    gsp->blue  = 0;
  }

  leftoff = GetOffsetInBioseq (sgp->loc, bsp, SEQLOC_LEFT_END);
  rightoff = GetOffsetInBioseq (sgp->loc, bsp, SEQLOC_RIGHT_END);
  if (!gsp->flagIsGUI)
  {
    leftoff /= scaleX;
    rightoff /= scaleX;
  }
  gsp->box.left = leftoff + start;
  gsp->box.right = rightoff - 1 + start;

  gsp->sgp = sgp;
  gsp->a = sgp->a;
  gsp->b = sgp->b;
  is_phrap = (Boolean) (StringICmp (sgp->title, "Phrap Quality") == 0 ||
                        StringICmp (sgp->title, "Phred Quality") == 0);
  switch (sgp->flags [2]) {
    case 1 :
      min = (Int4) sgp->min.realvalue;
      max = (Int4) sgp->max.realvalue;
      axis = (Int4) sgp->axis.realvalue;
      if (sgp->flags [1] != 0) {
        min = (Int4) (sgp->a * ((FloatHi) min) + sgp->b);
        max = (Int4) (sgp->a * ((FloatHi) max) + sgp->b);
      }
      break;
    case 2 :
      min = (Int4) sgp->min.intvalue;
      max = (Int4) sgp->max.intvalue;
      axis = (Int4) sgp->axis.intvalue;
      if (sgp->flags [1] != 0) {
        min = (Int4) (sgp->a * ((FloatHi) min) + sgp->b);
        max = (Int4) (sgp->a * ((FloatHi) max) + sgp->b);
      }
      break;
    case 3 :
      min = (Int4) sgp->min.intvalue;
      max = (Int4) sgp->max.intvalue;
      if (is_phrap) {
        min = MIN (0, min);
        max = MAX (100, max);
      }
      axis = (Int4) sgp->axis.intvalue;
      if (sgp->flags [1] != 0) {
        min = (Int4) (sgp->a * ((FloatHi) min) + sgp->b);
        max = (Int4) (sgp->a * ((FloatHi) max) + sgp->b);
      }
      break;
    default :
      min = (Int4) 0;
      max = (Int4) 100;
      axis = (Int4) 0;
      break;
  }
  gsp->seg = seg = CreateSegment (pict, 0, 0);
  gsp->bottom = top - (max - min) - 20;
  AddSegRect (seg, FALSE, 0);

  if (sgp->title != NULL)  /* StringHasNoText -- vibforms */
  {
    if (StrLen (sgp->title) > 0)
    {
      AddLabel (seg, (gsp->box.left + gsp->box.right) / 2, top,
                sgp->title, SMALL_TEXT, 0, MIDDLE_CENTER, 0);
    }
  }

  top -= 10;
  gsp->box.top = top;
  gsp->box.bottom = gsp->bottom;
  gsp->min = min;
  gsp->max = max;
  gsp->axis = axis;
  gsp->bottom += 10;

  if (is_phrap)
  {
    for (i = 0; i <=100; i += 20) {
      sprintf (str, "%ld", (long) i);
      AddLabel (seg, gsp->box.left, gsp->bottom + i, str,
                SMALL_TEXT, 5, MIDDLE_LEFT, 0);
    }
  }
  else
  {
    sprintf (str, "%ld", (long) max);
    AddLabel (seg, gsp->box.left, top-10, str,
              SMALL_TEXT, 5, MIDDLE_LEFT, 0);
    sprintf (str, "%ld", (long) min);
    AddLabel (seg, gsp->box.left, gsp->bottom-10, str,
              SMALL_TEXT, 5, MIDDLE_LEFT, 0);
    if (min < 0 && max > 0)
    {
      sprintf (str, "%ld", 0L);
      AddLabel (seg, gsp->box.left, gsp->bottom-min-10, str,
                SMALL_TEXT, 5, MIDDLE_LEFT, 0);
    }
  }

  gsp->snt = AddSntRectangle (seg, gsp->box.left, gsp->box.top,
                              gsp->box.right, gsp->box.bottom,
                              GraphSentProc, (BigScalar) gsp, CleanGSP, 0);
  return gsp;
}

extern SegmenT DrawSeqGraphSegment (SeqGraphPtr sgp, BioseqPtr bsp,
                                    Int4 xlen, Int4 top, Int2 start,
                                    Int4 leftend, Int4 rightend,
                                    SegmenT pict)
{
  GraphSentPtr gsp;
  Int4         scaleX;
  Char         str[32];

  if (sgp == NULL || bsp == NULL)
    return NULL;

  if (pict == NULL)
    pict = CreatePicture ();
  scaleX = sgp->numval / xlen;
  if ((sgp->numval % xlen) != 0)
    scaleX++;

  if ((gsp = AddGraphSentinelToPicture (sgp, bsp, pict, scaleX, top, start,
                                        NULL))
      != NULL)
  {
    if (gsp->seg != NULL)
    {
      sprintf (str, "%ld", (long) leftend);
      AddLabel (gsp->seg, gsp->box.left, gsp->bottom-20,
                str, SMALL_TEXT, 0, MIDDLE_CENTER, 0);
      sprintf (str, "%ld", (long) rightend);
      AddLabel (gsp->seg, gsp->box.left+(sgp->numval/scaleX), gsp->bottom-20,
                str, SMALL_TEXT, 0, MIDDLE_CENTER, 0);
    }
  }
  return pict;
}

extern void DrawGraphToPanel (SeqGraphPtr sgp, Int4 Xscale,
                              Int4 Yscale, Int4 Ylength, RecT PNTR rp,
                              Int4 leftend, Int4 rightend)
{
  Int4           len, max, min;
  Int4Ptr        xpoints, xp;
  Char           numberbuffer[32];

  switch (sgp->flags[2])
  {
   case 1:
    min = (Int4) sgp->min.realvalue;
    max = (Int4) sgp->max.realvalue;
    if (sgp->flags[1] != 0)
    {
      min = (Int4) (sgp->a * ((FloatHi) min) + sgp->b);
      max = (Int4) (sgp->a * ((FloatHi) max) + sgp->b);
    }
    break;
   case 2:
   case 3:
    min = (Int4) sgp->min.intvalue;
    max = (Int4) sgp->max.intvalue;
    if (sgp->flags[1] != 0)
    {
      min = (Int4) (sgp->a * ((FloatHi) min) + sgp->b);
      max = (Int4) (sgp->a * ((FloatHi) max) + sgp->b);
    }
    break;
   default:
    min = (Int4) 0;
    max = (Int4) Ylength;
    break;
  }
  max /= Yscale;
  min /= Yscale;
  len = sgp->numval;

  if (sgp->title != NULL)  /* StringHasNoText -- vibforms */
  {
    if (StrLen (sgp->title) > 0)
    {
      PaintStringEx (sgp->title, rp->left, (Int2) (rp->bottom-Ylength-30));
    }
  }

  sprintf (numberbuffer, "%ld", (long) max);
  PaintStringEx (numberbuffer, (Int2) (rp->left+((len/Xscale)+10)),
                 (Int2) (rp->bottom-Ylength));
  sprintf (numberbuffer, "%ld", (long) min);
  PaintStringEx (numberbuffer, (Int2) (rp->left+((len/Xscale)+10)),
                 rp->bottom);
  sprintf (numberbuffer, "%ld", (long) leftend);
  PaintStringEx (numberbuffer, rp->left,
                 (Int2) (rp->bottom+20));
  sprintf (numberbuffer, "%ld", (long) rightend);
  PaintStringEx (numberbuffer, (Int2) (rp->left+((len/Xscale)-20)),
                 (Int2) (rp->bottom+20));
  if (min < 0 && max > 0)
  {
    sprintf (numberbuffer, "%ld", 0L);
    PaintStringEx (numberbuffer, (Int2) (rp->left+((len/Xscale)+10)),
                   (Int2) (rp->bottom+min));
  }

  xp = xpoints = MakeWorkingSeqGraphInt4Array (sgp->values, sgp->flags[2],
                                               0, len, FALSE, sgp->a, sgp->b);
  PlotTheWorkingArray (xpoints, len, Xscale, (Int4) sgp->min.realvalue, *rp,
                       0, 0, 0, NULL);
  MemFree (xpoints);
  return;
}
