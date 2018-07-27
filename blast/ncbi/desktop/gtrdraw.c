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
* File Name: gtrdraw.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.4 $
*
* File Description: sentinal trees
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: gtrdraw.c,v $
* Revision 6.4  1999/10/04 17:46:23  kans
* include vibrant.h
*
* Revision 6.3  1998/10/01 16:37:10  kuzio
* cast
*
* Revision 6.2  1998/09/16 19:00:36  kuzio
* cvs logs
*
* ==========================================================================
*/

#include <vibrant.h>
#include <drawingp.h>
#include <gtrdraw.h>

/* common */

static void CleanGSP (BigScalar calldata)
{
  GraphTreeSentPtr gtp;

  if ((gtp = (GraphTreeSentPtr) calldata) == NULL)
    return;
  MemFree (gtp->epa);
  MemFree (gtp->offsetX);
  MemFree (gtp->offsetY);
  ValNodeFreeData (gtp->vnpnames);
  MemFree (gtp);
  return;
}

static void PlotTheTreePlot (Int4Ptr epa, Int4 numval, ValNodePtr vnp,
                             Int2 fontsize, Boolean flagLabelNodes,
                             AttPData PNTR curattrib,
                             Int2 scrolloffsetX, Int2 scrolloffsetY)
{
  Int4    i;
  PoinT   point1, point2;
  Int2    width, height;
  CharPtr name;
  FonT    f, fx;
  Uint1   curR, curG, curB;

  if (flagLabelNodes)
  {
    curR = curattrib->color[0];
    curG = curattrib->color[1];
    curB = curattrib->color[2];
    fx = FindNextResidentFont (NULL);
    f = CreateFont (Helvetica (fontsize, STYLE_BOLD));
    SelectFont (f);
  }

  for (i = 0; i < numval; i++)
  {
    point1.x = scrolloffsetX + (Int2) *epa++;
    point1.y = scrolloffsetY + (Int2) *epa++;
    point2.x = scrolloffsetX + (Int2) *epa++;
    point2.y = scrolloffsetY + (Int2) *epa++;
    DrawLine (point1, point2);
    if (flagLabelNodes && vnp->data.ptrvalue != NULL)
    {
      SelectColor (255, 0, 0);
      name = (CharPtr) vnp->data.ptrvalue;
      if (StrLen (name) > 8)
        name[8] = '\0';
      width = StringWidth (name);
      height = FontHeight ();
      PaintStringEx (name,(Int2)(point2.x-(width/2)),(Int2)(point2.y+height));
      SelectColor (curR, curG, curB);
    }
    vnp = vnp->next;
  }

  if (flagLabelNodes)
  {
    SelectFont (fx);
    DeleteFont (f);
  }

  return;
}

static void TreeSentProc (BigScalar calldata, PrimDrawContext pdc)
{
  GraphTreeSentPtr gtp;
  DrawInfoPtr      dInfoPtr;
  ScaleInfo        sInfo;

  if ((gtp = (GraphTreeSentPtr) calldata) == NULL)
    return;
  if (gtp->epa == NULL)
    return;

  dInfoPtr = (DrawInfoPtr) pdc;
  sInfo    = dInfoPtr->scale;
  if (gtp->offsetX == NULL)
  {
    gtp->offsetX = (Int4Ptr) MemNew (sizeof (Int4));
    *(gtp->offsetX) = sInfo.offsetX;
    gtp->offsetY = (Int4Ptr) MemNew (sizeof (Int4));
    *(gtp->offsetY) = sInfo.offsetY;
  }
  PlotTheTreePlot (gtp->epa, gtp->numval, gtp->vnpnames,
                   gtp->fontsize, gtp->flagLabelNodes,
                   &(dInfoPtr->curattrib),
                   (Int2) (sInfo.offsetX - (*(gtp->offsetX))),
                   (Int2) (sInfo.offsetY - (*(gtp->offsetY))));
  return;
}

static GraphTreeSentPtr AddTreeSentinelToPicture (SegmenT seg,
                                                  TreeNodePtr ptrNode,
                                                  Int2 fontsize,
                                                  Boolean flagLabelNodes)
{
  GraphTreeSentPtr gtp;
  Int4Ptr          epap;
  Int4             numval, nwidth, xwidth, ndepth, xdepth;
  TreeNodePtr      ptrNodeNext;
  Boolean          flagTakeNext;
  ValNodePtr       vnp;

  if ((gtp = (GraphTreeSentPtr) MemNew (sizeof (GraphTreeSentData))) == NULL)
    return NULL;
  if ((gtp->epa = (Int4Ptr) MemNew (sizeof (Int4) * 262144)) == NULL)
    return NULL;
  gtp->offsetX = NULL;
  gtp->offsetY = NULL;
  gtp->flagIsGUI = VibrantIsGUI ();
  gtp->vnpnames = NULL;
  gtp->fontsize = fontsize;
  gtp->flagLabelNodes = flagLabelNodes;

  epap = gtp->epa;
  numval = 0;
  ptrNodeNext = NULL;
  flagTakeNext = FALSE;
  while ((ptrNodeNext = ExploreTree (ptrNode, ptrNodeNext,
          &flagTakeNext)) != NULL)
  {
    if (ptrNodeNext->ptrChildLeft != NULL)
    {
      *epap++ = ptrNodeNext->xcoord;
      *epap++ = ptrNodeNext->ycoord;
      *epap++ = ptrNodeNext->ptrChildLeft->xcoord;
      *epap++ = ptrNodeNext->ptrChildLeft->ycoord;
      vnp = ValNodeNew (gtp->vnpnames);
      if (gtp->vnpnames == NULL)
        gtp->vnpnames = vnp;
      if ((ptrNodeNext->ptrChildLeft->ptrChildLeft == NULL) &&
          (ptrNodeNext->ptrChildLeft->ptrChildRight == NULL))
      {
        vnp->data.ptrvalue = (Pointer) StringSave
                              (ptrNodeNext->ptrChildLeft->name);
      }
      numval++;
    }
    if (ptrNodeNext->ptrChildRight != NULL)
    {
      *epap++ = ptrNodeNext->xcoord;
      *epap++ = ptrNodeNext->ycoord;
      *epap++ = ptrNodeNext->ptrChildRight->xcoord;
      *epap++ = ptrNodeNext->ptrChildRight->ycoord;
      vnp = ValNodeNew (gtp->vnpnames);
      if (gtp->vnpnames == NULL)
        gtp->vnpnames = vnp;
      if ((ptrNodeNext->ptrChildRight->ptrChildLeft == NULL) &&
          (ptrNodeNext->ptrChildRight->ptrChildRight == NULL))
      {
        vnp->data.ptrvalue = (Pointer) StringSave
                              (ptrNodeNext->ptrChildRight->name);
      }
      numval++;
    }
    flagTakeNext = FALSE;
  }
  gtp->numval = numval;

  nwidth = GetMinimumTreeXcoord (ptrNode);
  ndepth = GetMinimumTreeYcoord (ptrNode);
  xwidth = GetMaximumTreeXcoord (ptrNode);
  xdepth = GetMaximumTreeYcoord (ptrNode);
  xwidth += 32;
  xdepth += 16;
  gtp->snt = AddSntRectangle (seg, nwidth, ndepth, xwidth, xdepth,
                              TreeSentProc, (BigScalar) gtp,
                              CleanGSP, 0);
  return gtp;
}

extern SegmenT DrawTree (TreeNodePtr ptrNode)
{
  SegmenT seg;

  seg = CreatePicture ();
  AddTreeSentinelToPicture (seg, ptrNode, 8, TRUE);
  return seg;
}
