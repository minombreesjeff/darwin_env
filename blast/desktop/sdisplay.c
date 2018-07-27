/*   sdisplay.c
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
* File Name:  sdisplay.c
*
* Author:  Sergei Egorov
*
* Version Creation Date:   9/23/94
*
* $Revision: 6.0 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include <vibrant.h>
#include <drawingp.h>
#include <gather.h>
#include <sdisplay.h>
#include <fstyle.h>

extern void SeqLayoutInit (SeqLayoutPtr lptr, SegmenT pict)

{
  if (lptr != NULL) {
    lptr->pict = pict;
    lptr->scales = NULL;
    lptr->showPartsLabels = FALSE;
    lptr->currentTop = 0;
    lptr->currentBottom = 0;
    lptr->currentBspLen = 0;
  }
}

                              
#define MINDIST 50


static void AddASequenceToPicture (ValNodePtr vnp, SeqLayoutPtr lptr,
                                   Int2 fntHeight, Uint1 repr, Uint1 mol)
{
  Int4         bottom;
  Uint1        c;
  Int4         curpos;
  Boolean      first;
  FeatNodePtr  fnp;
  FonT         fnt;
  Int2         i;
  IvalNodePtr  inp;
  ValNodePtr   ivl;
  Int4         labelHere;
  Int4         lastStop;
  Uint2        lastType;
  Uint1        lightGrayColor [3];
  SeqScalePtr  scale;
  SegmenT      seg;
  Int4         seglen;
  Int4         top;
  SegmenT      type;
  ValNodePtr   vnpsc;
  FeatStylePtr fsp;
  Int2         labelHeight;

  if (vnp != NULL && vnp->data.ptrvalue != NULL &&
      lptr != NULL && lptr->pict != NULL) {
    fnp = (FeatNodePtr) vnp->data.ptrvalue;
    if (fnp == NULL || fnp->thistype != OBJ_BIOSEQ) {
      return;
    }
    lastType = 0;
    type = NULL;
    seg = NULL;

    fsp = GetSequenceStyle (repr, mol);
    if (fsp == NULL) return;
    labelHere = lptr->currentTop;
    if (repr == Seq_repr_seg) {
      fnt = CreateFeatureLabelFont (fsp);
      SelectFont (fnt);
      labelHeight = LineHeight () + 2;
      SelectFont (systemFont);
      DeleteFont (fnt);
      for (i = 0; i < 3; i++) {
        c = BLACK_COLOR [i];
        lightGrayColor [i] = (Uint1) (127 + c / 2);
      }
      if (lptr->showPartsLabels) {
        lptr->currentTop -= labelHeight;
      }
    }

    top = lptr->currentTop;
    bottom = lptr->currentTop - SEQUENCE_HEIGHT;
    if (repr != 0) {
      first = TRUE;
      ivl = fnp->ivals;
      while (ivl != NULL) {
        inp = (IvalNodePtr) ivl->data.ptrvalue;
        ivl = ivl->next;
        if (inp != NULL) {
          if (lastType != inp->thistype || type == NULL) {
            lastType = inp->thistype;
            type = CreateSegment (lptr->pict, lastType, 0);
          }
          curpos = inp->start;
          seglen = inp->stop - curpos;
          seg = CreateSegment (type, fnp->entityID, 0);
          if (repr != Seq_repr_virtual && inp->label != NULL && lptr->showPartsLabels) {
            AddSequenceLabel (seg, curpos + seglen / 2, top, inp->label, 
                              fsp, inp->itemID);
          }
          if (inp->subtype == Seq_repr_virtual) {
            AddFeatureGap (seg, curpos, top, curpos + seglen, bottom, 
                           fsp, inp->itemID);
          } else {
            AddSequenceIval (seg, curpos, top, curpos + seglen, bottom, 
                             fsp, inp->itemID);
          }
          lastStop = inp->stop;
          first = FALSE;
          curpos += seglen;
        }
      }
    }
    if (repr == Seq_repr_seg) {
      if (lastType != OBJ_BIOSEQ || type == NULL) {
        lastType = OBJ_BIOSEQ;
        type = CreateSegment (lptr->pict, lastType, 0);
      }
      seg = CreateSegment (type, fnp->entityID, 0);
      AddAttribute (seg, COLOR_ATT | WIDTH_ATT | STYLE_ATT, lightGrayColor,
                    SOLID_LINE, SOLID_SHADING, SEQPENWIDTH, NO_MODE);
      AddRectangle (seg, fnp->left, labelHere + 3 * labelHeight /4,
                    fnp->right, labelHere + labelHeight /4,
                    NO_ARROW, TRUE, fnp->itemID);
    }
#if 0
    AddAttribute (seg, COLOR_ATT | WIDTH_ATT | STYLE_ATT | SHADING_ATT, BLACK_COLOR,
                  SOLID_LINE, SOLID_SHADING, SEQPENWIDTH, NO_MODE);
#endif
    if (fnp->label != NULL) {
      if (lastType != OBJ_BIOSEQ || type == NULL) {
        lastType = OBJ_BIOSEQ;
        type = CreateSegment (lptr->pict, lastType, 0);
      }
      seg = CreateSegment (type, fnp->entityID, 0);
      AddSequenceLabel (seg, (fnp->left + fnp->right) / 2, labelHere, 
                        fnp->label, fsp, fnp->itemID);
    }
    if (lptr->currentBottom > bottom) {
      lptr->currentBottom = bottom;
    }
    vnpsc = ValNodeNew (lptr->scales);
    if (lptr->scales == NULL) {
      lptr->scales = vnpsc;
    }
    if (vnpsc != NULL) {
      scale = MemNew (sizeof (SeqScaleData));
      vnpsc->data.ptrvalue = scale;
      if (scale != NULL) {
        scale->rulerNotCalculated = TRUE;
        scale->ruler = 0;
        scale->currentSeqLength = lptr->currentBspLen;
        scale->top = lptr->currentTop;
      }
    }
    lptr->currentTop -= SEQUENCE_SPACING;
#ifdef WIN_MSWIN
    lptr->currentTop -= 5;
#endif
  }
}

extern void AddSequenceToPicture (ValNodePtr vnp, SeqLayoutPtr lptr, Uint1 repr, Uint1 mol)

{
  FeatNodePtr  fnp;
  FonT         fnt;
  Int2         fntHeight;

  if (vnp != NULL && lptr != NULL && lptr->pict != NULL) {
    fnt = SetSmallFont ();
    SelectFont (fnt);
    fntHeight = LineHeight ();
    SelectFont (systemFont);
    lptr->currentBspLen = 0;
    fnp = (FeatNodePtr) vnp->data.ptrvalue;
    if (fnp != NULL) {
      lptr->currentBspLen = fnp->right - fnp->left;
    }
    AddASequenceToPicture (vnp, lptr, fntHeight, repr, mol);
  }
}




static void DrawScale (VieweR vwr, SegmenT picture)

{
  Int4         curpos;
  FloatHi      logDist;
  FloatHi      minDist;
  Int4         mod;
  FloatHi      nextPower;
  PntInfo      pnt1;
  PntInfo      pnt2;
  BoxInfo      port;
  PoinT        pt;
  RecT         r;
  RecT         s;
  SeqScalePtr  scale;
  ValNodePtr   scales;
  Int4         scaleX;
  Int4         scaleY;
  Int4         start;
  Int4         stop;
  Char         str [16];
  RecT         view;
  ValNodePtr   vnp;
  Int2         width;

  scales = GetViewerData (vwr);
  if (scales != NULL) {
    scale = NULL;
    vnp = scales;
    while (vnp != NULL) {
      scale = (SeqScalePtr) vnp->data.ptrvalue;
      ObjectRect (vwr, &r);
      ViewerBox (vwr, NULL, &port, &view, &scaleX, &scaleY);
      InsetRect (&r, 4, 4);
      LoadPt (&pt, r.left, r.top);
      MapViewerToWorld (vwr, pt, &pnt1);
      LoadPt (&pt, r.right, r.top);
      MapViewerToWorld (vwr, pt, &pnt2);
      start = pnt1.x;
      stop = pnt2.x;
      start = MAX (start, 0);
      if (scale != NULL) {
        if (scale->rulerNotCalculated) {
          minDist = (double) (MINDIST * scaleX);
          logDist = log10 (minDist);
          nextPower = (exp (ceil (logDist) * NCBIMATH_LN10));
          if (minDist < nextPower / 5.0) {
            scale->ruler = (Int4) ((nextPower + 0.5) / 5.0);
          } else if (minDist < nextPower / 4) {
            scale->ruler = (Int4) ((nextPower + 0.5) / 4.0);
          } else if (minDist < nextPower / 2) {
            scale->ruler = (Int4) ((nextPower + 0.5) / 2.0);
          } else {
            scale->ruler = (Int4) (nextPower + 0.5);
          }
          scale->rulerNotCalculated = FALSE;
        }
        mod = start % scale->ruler;
        if (mod != 0) {
          start -= mod;
        }
        stop = MIN (stop, scale->currentSeqLength);
        mod = stop % scale->ruler;
        if (mod != 0) {
          stop += scale->ruler - mod;
        }
        stop = MIN (stop, scale->currentSeqLength);
        SelectFont (programFont);
        for (curpos = start; curpos <= stop; curpos += scale->ruler) {
          pnt1.x = curpos;
          pnt1.y = scale->top - SEQUENCE_HEIGHT;
          MapWorldToViewer (vwr, pnt1, &pt);
          MoveTo (pt.x, pt.y + 2);
          LineTo (pt.x, pt.y + 10);
          LoadRect (&s, pt.x, pt.y + 12, pt.x, pt.y + 12 + stdLineHeight);
          sprintf (str, "%ld", (long) curpos);
          width = StringWidth (str);
          InsetRect (&s, -(width / 2), 0);
          InsetRect (&s, -2, 0);
          DrawString (&s, str, 'c', FALSE);
        }
      }
      vnp = vnp->next;
    }
    SelectFont (systemFont);
  }
}


static void CleanUp (VieweR vwr, VoidPtr data)

{
  ValNodeFreeData ((ValNodePtr) data);
}


extern void AttachSeqPictureToViewer (VieweR vwr, SeqLayoutPtr lptr, Int4 scaleX)

{
  SeqScalePtr  scale;
  Int4         scaleY;
  ValNodePtr   vnp;

  if (vwr != NULL && lptr != NULL && lptr->pict != NULL) {
    vnp = lptr->scales;
    while (vnp != NULL) {
      scale = (SeqScalePtr) vnp->data.ptrvalue;
      if (scale != NULL) {
        scale->rulerNotCalculated = TRUE;
        scale->ruler = 0;
      }
      vnp = vnp->next;
    }
    scaleY = 1;
    AttachInstance (vwr, lptr->pict, INT4_MIN, INT4_MAX, UPPER_LEFT, scaleX,
                      scaleY, DrawScale, (VoidPtr) lptr->scales, CleanUp);
    lptr->scales = NULL;
  }
}

