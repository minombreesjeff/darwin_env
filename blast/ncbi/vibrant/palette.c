/*   palette.c
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
* File Name:  palette.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   1/1/91
*
* $Revision: 6.1 $
*
* File Description:   Active graphics objects
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: palette.c,v $
* Revision 6.1  2003/11/07 16:02:30  rsmith
* Renamed static function NewPalette to Nlm_NewPalette to avoid a name conflict on Mac.
*
* Revision 6.0  1997/08/25 18:56:11  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 13:45:08  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 2.3  1995/05/17  15:15:14  kans
 * added Log line
 *
*
* ==========================================================================
*/

#include "panels.h"

typedef  struct  paletteData {
  Int2         drawCount;
  Handle       drawHandles;
  Int2         leftMargin;
  Int2         rightMargin;
  Int2         width;
  PaletteProc  action;
} PaletteData;

typedef  struct  drawRec {
  PaintProc    draw;
  PaletteProc  click;
  RecT         rect;
  Int2         value;
} DrawRec, PNTR DrawPtr;

static void DrawPalette (PaneL p)

{
  DrawPtr      dptr;
  PaintProc    drw;
  Handle       hdld;
  Int2         i;
  Int2         numd;
  Int2         off;
  PaletteData  pdata;
  RecT         r;
  SlatE        s;
  RecT         sr;

  s = (SlatE) Parent (p);
  GetPanelExtra (p, &pdata);
  numd = pdata.drawCount;
  hdld = pdata.drawHandles;
  ObjectRect (s, &sr);
  InsetRect (&sr, 4, 4);
  GetOffset (p, NULL, &off);
  if (hdld != NULL) {
    dptr = (DrawPtr) HandLock (hdld);
    i = 0;
    while (i < numd) {
      drw = dptr [i].draw;
      if (drw != NULL) {
        r = dptr [i].rect;
        OffsetRect (&r, 0, -off);
        if (RectInRect (&r, &sr) &&
            (updateRgn == NULL || RectInRgn (&r, updateRgn))) {
          drw ((PalettE) p, i + 1, &r);
        }
      }
      i++;
    }
    HandUnlock (hdld);
  }
}

extern void AppendPalette (PalettE p, RectPtr rct,
                           PaintProc draw, PaletteProc click,
                           Int2 value)

{
  Int2         chunk;
  DrawPtr      dptr;
  RecT         dr;
  Handle       hdld;
  Int2         numd;
  Int2         off;
  PaletteData  pdata;
  RecT         r;
  SlatE        s;
  RecT         sr;
  WindoW       tempPort;

  if (p != NULL && rct != NULL) {
    tempPort = SavePort (p);
    s = (SlatE) Parent (p);
    GetPanelExtra ((PaneL) p, &pdata);
    numd = pdata.drawCount;
    hdld = pdata.drawHandles;
    chunk = 128;
    if (hdld != NULL) {
      if (numd % chunk == 0) {
        hdld = HandMore (hdld, sizeof (DrawRec) *
                         (numd / chunk + 1) * chunk);
      }
    } else {
      hdld = HandNew (sizeof (DrawRec) * chunk);
    }
    numd++;
    pdata.drawHandles = hdld;
    pdata.drawCount = numd;
    SetPanelExtra ((PaneL) p, &pdata);
    dptr = (DrawPtr) HandLock (hdld);
    dptr [numd - 1].draw = draw;
    dptr [numd - 1].click = click;
    dptr [numd - 1].rect = *rct;
    dptr [numd - 1].value = value;
    HandUnlock (hdld);
    r = *rct;
    RegisterRect ((PaneL) p, &r);
    if (draw != NULL && Enabled (p) && AllParentsEnabled (p) &&
        Visible (p) && AllParentsVisible (p)) {
      ObjectRect (s, &sr);
      InsetRect (&sr, 4, 4);
      GetOffset (p, NULL, &off);
      OffsetRect (&r, 0, -off);
      SectRect (&r, &sr, &dr);
      if (RectInRect (&dr, &sr)) {
        Select (p);
        InvalRect (&dr);
      }
    }
    RestorePort (tempPort);
  }
}

extern void SetPaletteValue (PalettE p, Int2 num, Int2 value)

{
  DrawPtr      dptr;
  Handle       hdld;
  Int2         numd;
  Int2         off;
  PaletteData  pdata;
  RecT         r;
  SlatE        s;
  RecT         sr;
  WindoW       tempPort;

  if (p != NULL && num > 0) {
    tempPort = SavePort (p);
    num--;
    s = (SlatE) Parent (p);
    GetPanelExtra ((PaneL) p, &pdata);
    ObjectRect (s, &sr);
    InsetRect (&sr, 4, 4);
    GetOffset (p, NULL, &off);
    numd = pdata.drawCount;
    hdld = pdata.drawHandles;
    if (hdld != NULL && num < numd) {
      dptr = (DrawPtr) HandLock (hdld);
      if (dptr [num].value != value) {
        dptr [num].value = value;
        r = dptr [num].rect;
        OffsetRect (&r, 0, -off);
        if (dptr [num].draw != NULL && Enabled (p) && AllParentsEnabled (p) &&
            Visible (p) && AllParentsVisible (p) &&
            RectInRect (&r, &sr)) {
          Select (p);
          InvalRect (&r);
        }
      }
      HandUnlock (hdld);
    }
    RestorePort (tempPort);
  }
}

extern Int2 GetPaletteValue (PalettE p, Int2 num)

{
  DrawPtr      dptr;
  Handle       hdld;
  Int2         numd;
  PaletteData  pdata;
  Int2         rsult;

  rsult = 0;
  if (p != NULL && num > 0) {
    num--;
    GetPanelExtra ((PaneL) p, &pdata);
    numd = pdata.drawCount;
    hdld = pdata.drawHandles;
    if (hdld != NULL && num < numd) {
      dptr = (DrawPtr) HandLock (hdld);
      rsult = dptr [num].value;
      HandUnlock (hdld);
    }
  }
  return rsult;
}

extern Int2 PaletteNumItems (PalettE p)

{
  PaletteData  pdata;
  Int2         rsult;

  rsult = 0;
  if (p != NULL) {
    GetPanelExtra ((PaneL) p, &pdata);
    rsult = pdata.drawCount;
  }
  return rsult;
}

static void PaletteClick (PaneL p, PoinT pt)

{
  PaletteProc  actn;
  DrawPtr      dptr;
  Boolean      goOn;
  Handle       hdld;
  Int2         i;
  Int2         numd;
  Int2         off;
  PaletteData  pdata;
  RecT         r;
  SlatE        s;
  RecT         sr;

  if (p != NULL) {
    actn = NULL;
    s = (SlatE) Parent (p);
    GetPanelExtra (p, &pdata);
    ObjectRect (s, &sr);
    InsetRect (&sr, 4, 4);
    GetOffset (p, NULL, &off);
    numd = pdata.drawCount;
    hdld = pdata.drawHandles;
    if (hdld != NULL) {
      dptr = (DrawPtr) HandLock (hdld);
      i = 0;
      goOn = TRUE;
      while (i < numd && goOn) {
        actn = dptr [i].click;
        r = dptr [i].rect;
        OffsetRect (&r, 0, -off);
        if (dptr [i].draw != NULL) {
          if (RectInRect (&r, &sr) && PtInRect (pt, &r)) {
            goOn = FALSE;
          } else {
            i++;
          }
        } else {
          if (PtInRect (pt, &r) && PtInRect (pt, &sr)) {
            goOn = FALSE;
          } else {
            i++;
          }
        }
      }
      HandUnlock (hdld);
      if (i < numd) {
        if (actn != NULL) {
          actn ((PalettE) p, i + 1);
        }
        actn = pdata.action;
        if (actn != NULL) {
          actn ((PalettE) p, i + 1);
        }
      }
    }
  }
}

static void Nlm_NewPalette (PalettE p, Int2 minwid, PaletteProc actn)

{
  PoinT        npt;
  PaletteData  pdata;
  RecT         r;
  SlatE        s;

  SelectFont (systemFont);
  s = (SlatE) Parent (p);
  ObjectRect (s, &r);
  InsetRect (&r, 4, 4);
  GetNextPosition (p, &npt);
  pdata.drawCount = 0;
  pdata.drawHandles = NULL;
  pdata.leftMargin = npt.x;
  pdata.rightMargin = npt.x + minwid;
  pdata.width = pdata.rightMargin - pdata.leftMargin;
  pdata.action = actn;
  SetPanelExtra ((PaneL) p, &pdata);
  LoadRect (&r, npt.x, npt.y, pdata.rightMargin, npt.y);
  RegisterRect ((PaneL) p, &r);
  Break (p);
}

static void ResetPalette (PaneL p)

{
  PaletteData  pdata;

  GetPanelExtra (p, &pdata);
  if (pdata.drawHandles != NULL) {
    HandFree (pdata.drawHandles);
  }
  Nlm_NewPalette ((PalettE) p, pdata.width, pdata.action);
}

extern PalettE PalettePanel (SlatE s, Int2 pixwidth, PaletteProc actn)

{
  PalettE  p;
  WindoW   tempPort;

  p = NULL;
  if (s != NULL) {
    tempPort = SavePort (s);
    p = (PalettE) CustomPanel (s,  DrawPalette, sizeof (PaletteData), ResetPalette);
    if (p != NULL) {
      SetPanelClick ((PaneL) p, PaletteClick, NULL, NULL, NULL);
      Nlm_NewPalette (p, pixwidth, actn);
    }
    RestorePort (tempPort);
  }
  return p;
}
