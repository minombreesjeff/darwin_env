/*   pictogif.c
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
* File Name:  pictogif
*
* Author:  Alex Smirnov
*
* Version Creation Date:   11/23/95
*
* $Revision: 6.2 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: pictogif.c,v $
* Revision 6.2  1999/10/20 23:14:00  vakatov
* + Nlm_Picture2gdImage() -- to draw in gdImage.
* Nlm_PictureToGIF() -- restore current GIF image (if any) when the
* dump is complete.
*
* Revision 6.1  1998/07/02 18:19:29  vakatov
* Cleaned the code & made it pass through the C++ compilation
*
* ==========================================================================
*/

#include <vibrant.h>
#include <picturep.h>
#include <mappingp.h>
#include <drawingp.h>
#include <ncbigif.h>
#include <gifgen.h>


/****************************************************************************/
/* DEFINES */
/****************************************************************************/
#define PROXIMITY    2


/****************************************************************************/
/* STATIC FUNCTIONS */
/****************************************************************************/

static Boolean Nlm_MakeScaleGIF ( SegmenT picture, Int2 gifWidth, 
                                 Int2 gifHeight, Int4 pntX, Int4 pntY,
                                 Int2 align, Int4 scaleX, Int4 scaleY,
                                 ScalePtr scale )
{
  Int2 pGifX, pGifY;

  if ( scaleX < 0 ) scaleX = -scaleX;
  if ( scaleY < 0 ) scaleY = -scaleY;
  SelectFont (systemFont);
  RecalculateSegment (picture, scaleX, scaleY);
  switch (align) {
    case MIDDLE_CENTER :
      pGifX = gifWidth / 2;
      pGifY = gifHeight / 2;
      break;
    case UPPER_LEFT :
      pGifX = 0;
      pGifY = 0;
      break;
    case LOWER_LEFT :
      pGifX = 0;
      pGifY = gifHeight-1;
      break;
    case UPPER_RIGHT :
      pGifX = gifWidth-1;
      pGifY = 0;
      break;
    case LOWER_RIGHT :
      pGifX = gifWidth-1;
      pGifY = gifHeight-1;
      break;
    case UPPER_CENTER :
      pGifX = gifWidth / 2;
      pGifY = 0;
      break;
    case LOWER_CENTER :
      pGifX = gifWidth / 2;
      pGifY = gifHeight-1;
      break;
    case MIDDLE_LEFT :
      pGifX = 0;
      pGifY = gifHeight / 2;
      break;
    case MIDDLE_RIGHT :
      pGifX = gifWidth-1;
      pGifY = gifHeight / 2;
      break;
    default :
      return FALSE;
  }
  scale->scaleX = scaleX;
  scale->scaleY = scaleY;
  scale->offsetX = (Int4)pGifX * scaleX - pntX;
  scale->offsetY = (Int4)pGifY * scaleY + pntY ;
  scale->worldWindow.left = - scale->offsetX;
  scale->worldWindow.right = (Int4)gifWidth * scaleX - scale->offsetX;
  scale->worldWindow.top = scale->offsetY;
  scale->worldWindow.bottom = - (Int4)gifHeight * scaleY + scale->offsetY;
  scale->worldWindow16.left = -1000 * scaleX - scale->offsetX;
  scale->worldWindow16.right =  4000 * scaleX - scale->offsetX;
  scale->worldWindow16.top = 1000 * scaleY + scale->offsetY;
  scale->worldWindow16.bottom = -4000 * scaleY + scale->offsetY;
  return TRUE;
}

/****************************************************************************/
/* GLOBAL FUNCTIONS */
/****************************************************************************/

NLM_EXTERN Boolean Nlm_PictureToGIF
(FILE* out, Int2 gifWidth, Int2 gifHeight,
 SegmenT picture, Int4 pntX, Int4 pntY,
 Int2 align, Int4 scaleX, Int4 scaleY,
 Boolean transparent)
{
  Nlm_DrawInfo dInfo;

  if (!out  ||  !picture  ||  !scaleX  ||  !scaleY  ||
      gifWidth <= 0  ||  gifHeight <= 0  ||
      ((PicPPtr) picture)->base.code != PICTURE)
    return FALSE;

  if ( !Nlm_MakeScaleGIF(picture, gifWidth, gifHeight, pntX, pntY,
                         align, scaleX, scaleY, &dInfo.scale) )
    return FALSE;
  
  {{ /* Draw */
    gdImage* im_prev = Nlm_SetCurrentGIF(0);
    if ( !Nlm_CreateGIF(gifWidth, gifHeight, transparent) ) {
      Nlm_SetCurrentGIF(im_prev);
      return FALSE;
    }
    dInfo.checked  = FALSE;
    dInfo.curattrib = blackAttPData;
    dInfo.highlight = 0;
    Nlm_DrawSegment((SegPPtr)picture, &dInfo);
    Nlm_SaveGIF(out);
    Nlm_DestroyGIF();
    Nlm_SetCurrentGIF(im_prev);
  }}

  return TRUE;
}


NLM_EXTERN Boolean Nlm_Picture2gdImage
(struct gdImageStruct* im, SegmenT picture,
 Int4 pntX, Int4 pntY, Int2 align, Int4 scaleX, Int4 scaleY)
{
  Nlm_DrawInfo dInfo;

  if (!im  ||  !picture  ||  !scaleX  ||  !scaleY  ||
      ((PicPPtr) picture)->base.code != PICTURE)
    return FALSE;

  {{ /* Scale and position of the picture in the GD image */
    int width, height;
    gdImageGetDimensions(im, &width, &height);
    if ( !Nlm_MakeScaleGIF(picture, (Int2)width, (Int2)height, pntX, pntY,
                           align, scaleX, scaleY, &dInfo.scale) )
      return FALSE;
  }}

  dInfo.checked   = FALSE;
  dInfo.curattrib = blackAttPData;
  dInfo.highlight = 0;

  {{ /* Draw */
    gdImage* im_prev = Nlm_SetCurrentGIF(im);
    Nlm_DrawSegment((SegPPtr) picture, &dInfo);
    Nlm_SetCurrentGIF(im_prev);
  }}
  return TRUE;
}


NLM_EXTERN SegmenT Nlm_FindPrimGIF
(SegmenT picture, PoinT pt,
 Int2 gifWidth, Int2 gifHeight,
 Int4 pntX, Int4 pntY,
 Int2 align, Int4 scaleX, Int4 scaleY,
 Uint2Ptr segIDPtr, 
 Uint2Ptr primIDPtr, PrimitivE PNTR primPtr)
{
  SegmenT      found = NULL;
  PrimitivE    prim  = NULL;
  PicPPtr      pic;
  ScaleInfo    scale;
  RecT         r;
  Uint2        segID = 0;
  Uint2        primID = 0;

  if ( picture != NULL && gifWidth > 0 && gifHeight > 0 
       && scaleX != 0 && scaleY != 0 ) {
    pic = (PicPPtr) picture;
    if (pic->base.code == PICTURE) {
      if ( !Nlm_MakeScaleGIF ( picture, gifWidth, gifHeight, pntX, pntY,
           align, scaleX, scaleY, &scale )) return NULL;
      LoadRect(&r,
               (Int2)(pt.x - PROXIMITY), (Int2)(pt.y - PROXIMITY), 
               (Int2)(pt.x + PROXIMITY), (Int2)(pt.y + PROXIMITY));
      MapRectToWorldBox ( &(scale.worldWindow), &r, &scale );
      found = SearchSegment (picture, &scale, &prim );
      if (found != NULL) {
        segID = SegmentID (found);
        primID = ((GenPPtr)prim)->primID;
      }
      if (segIDPtr != NULL) *segIDPtr = segID;
      if (primIDPtr != NULL) *primIDPtr = primID;
      if ( primPtr != NULL) *primPtr = prim;
      return found;
    }
  }
  return NULL;
}


NLM_EXTERN SegmenT Nlm_FindSegGIF
(SegmenT picture, PoinT pt,
 Int2 gifWidth, Int2 gifHeight,
 Int4 pntX, Int4 pntY,
 Int2 align, Int4 scaleX, Int4 scaleY,
 Uint2Ptr segIDPtr, 
 Uint2Ptr primIDPtr, Uint2Ptr primCtPtr)
{
  SegmenT   found;
  PrimitivE prim;
  BasePPtr  item;
  SegPPtr   seg;
  Uint2     primCt = 0;

  found = FindPrimGIF ( picture, pt, gifWidth, gifHeight,
                        pntX, pntY, align, scaleX, scaleY,
                        segIDPtr, primIDPtr, &prim );
  if ( found != NULL ) {
    seg = (SegPPtr) found;
    item = seg->seg.head;
    while (item != (BasePPtr)prim ) {
      item = item->next;
      primCt++;
    }
  }
  if ( primCtPtr != NULL ){
    *primCtPtr = primCt + 1;
  }
  return found;
}

/*EOF*/
