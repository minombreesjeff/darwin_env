/*   vibextra.c
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
* File Name:  vibextra.c
*
* Author:  Jonathan Kans, Alex Smirnov
*
* Version Creation Date:   3/9/93
*
* $Revision: 6.2 $
*
* File Description: 
*       Vibrant miscellaneous extensions
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: vibextra.c,v $
* Revision 6.2  2001/03/19 19:15:17  juran
* Change "for (...);" to "for (...) ;" (added space before semicolon) to silence trying-to-be-helpful compiler warning.
*
* Revision 6.1  1997/11/26 21:30:14  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.0  1997/08/25 18:56:53  madden
* Revision changed to 6.0
*
* Revision 5.2  1996/10/28 19:33:47  vakatov
* [WIN_MOTIF]  Use Nlm_VibrantDefaultColormap() instead of the display
* default colormap everywhere in the color handling code.
* Made a lot of type castings to get rid of all compiler warnings.
*
 * Revision 5.1  1996/07/25  19:48:09  vakatov
 * [WIN_X]  Added #VOID_PIXEL to serve as the tmpColorPixel's void value
 *
 * Revision 5.0  1996/05/28  13:45:08  ostell
 * Set to revision 5.0
 *
 * Revision 4.2  1996/04/24  21:20:45  vakatov
 * Nlm_ChooseColorDialog(): Nlm_WaitForCondition() to wait for the modal
 * dialog finishing
 *
 * Revision 4.1  1996/02/27  16:31:13  kans
 * subclass folder tabs to allow SetValue (but with 0, not 1, as first value)
 *
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 1.20  1995/05/20  18:27:38  kans
 * replaced ; accidentally removed when log message was added
 *
 * Revision 1.19  1995/05/17  15:15:14  kans
 * added Log line
 *
*
* ==========================================================================
*/

#include <vibtypes.h>
#include <vibprocs.h>
#include <vibincld.h>
#include <ncbiport.h>

#ifdef WIN_MSWIN
#include<commdlg.h>
#include<colordlg.h>
#endif

#ifdef VAR_ARGS
#include <varargs.h>
#else
#include <stdarg.h>
#endif

typedef  struct  Nlm_repeatdata {
  Nlm_Handle       title;
  Nlm_RptClckProc  action;
} Nlm_RepeatData;

typedef struct Nlm_switchdata {
  Nlm_Int2         max;
  Nlm_Int2         val;
  Nlm_Boolean      text;
  Nlm_Boolean      vert;
  Nlm_Boolean      upActv;
  Nlm_Boolean      dnActv;
  Nlm_SwtChngProc  actn;
} Nlm_SwitchData;

typedef  struct  Nlm_icondata {
  Nlm_IcnChngProc  inval;
  Nlm_Int2         value;
  Nlm_Boolean      status;
  Nlm_Handle       title;
} Nlm_IconData;

static Nlm_GphPrcsPtr  gphprcsptr = NULL;

static Nlm_GphPrcsPtr  repeatProcs;
static Nlm_GphPrcsPtr  switchProcs;
static Nlm_GphPrcsPtr  iconProcs;

extern Nlm_GphPrcsPtr  Nlm_folderTabProcs;
Nlm_GphPrcsPtr  Nlm_folderTabProcs = NULL;

static Nlm_Boolean     inRepeatButton;

static Nlm_Uint1 upFillArrow [] = {
  0x10, 0x38, 0x38, 0x7C, 0x7C, 0xFE, 0xFE, 0x00
};

static Nlm_Uint1 downFillArrow [] = {
  0xFE, 0xFE, 0x7C, 0x7C, 0x38, 0x38, 0x10, 0x00
};

static Nlm_Uint1 leftFillArrow [] = {
  0x06, 0x1E, 0x7E, 0xFE, 0x7E, 0x1E, 0x06, 0x00
};

static Nlm_Uint1 rightFillArrow [] = {
  0xC0, 0xF0, 0xFC, 0xFE, 0xFC, 0xF0, 0xC0, 0x00
};

static Nlm_Uint1 upOpenArrow [] = {
  0x10, 0x28, 0x28, 0x44, 0x44, 0x82, 0xFE, 0x00
};

static Nlm_Uint1 downOpenArrow [] = {
  0xFE, 0x82, 0x44, 0x44, 0x28, 0x28, 0x10, 0x00
};

static Nlm_Uint1 leftOpenArrow [] = {
  0x06, 0x1A, 0x62, 0x82, 0x62, 0x1A, 0x06, 0x00
};

static Nlm_Uint1 rightOpenArrow [] = {
  0xC0, 0xB0, 0x8C, 0x82, 0x8C, 0xB0, 0xC0, 0x00
};

#ifdef VAR_ARGS
extern void CDECL Nlm_AlignObjects (align, va_alist)
int align;
va_dcl
#else
extern void CDECL Nlm_AlignObjects (int align, ...)
#endif

{
  va_list     args;
  Nlm_Int2    delta;
  Nlm_Int2    maxX;
  Nlm_Int2    maxY;
  Nlm_Int2    minX;
  Nlm_Int2    minY;
  Nlm_Handle  obj;
  Nlm_RecT    r;

#ifdef VAR_ARGS
  va_start (args);
#else
  va_start (args, align);
#endif
  minX = 0;
  minY = 0;
  maxX = 0;
  maxY = 0;
  obj = (Nlm_Handle) va_arg (args, Nlm_HANDLE);
  while (obj != NULL) {
    Nlm_GetPosition (obj, &r);
    minX = MAX (minX, r.left);
    minY = MAX (minY, r.top);
    maxX = MAX (maxX, r.right);
    maxY = MAX (maxY, r.bottom);
    obj = (Nlm_Handle) va_arg (args, Nlm_HANDLE);
  }
  va_end(args);

#ifdef VAR_ARGS
  va_start (args);
#else
  va_start (args, align);
#endif
  obj = (Nlm_Handle) va_arg (args, Nlm_HANDLE);
  while (obj != NULL) {
    Nlm_GetPosition (obj, &r);
    switch (align) {
      case ALIGN_LEFT:
        if (r.left < minX) {
          r.left = minX;
          Nlm_SetPosition (obj, &r);
          Nlm_DoAdjustPrnt ((Nlm_GraphiC) obj, &r, FALSE, TRUE);
        }
        break;
      case ALIGN_RIGHT:
        if (r.right < maxX) {
          r.right = maxX;
          Nlm_SetPosition (obj, &r);
          Nlm_DoAdjustPrnt ((Nlm_GraphiC) obj, &r, FALSE, TRUE);
        }
        break;
      case ALIGN_CENTER:
        delta = maxX - r.right;
        if (delta > 0) {
          Nlm_OffsetRect (&r, (Nlm_Int2)(delta/2), 0);
          Nlm_SetPosition (obj, &r);
          Nlm_DoAdjustPrnt ((Nlm_GraphiC) obj, &r, FALSE, TRUE);
        }
        break;
      case ALIGN_JUSTIFY:
        if (r.left < minX || r.right < maxX) {
          r.left = minX;
          r.right = maxX;
          Nlm_SetPosition (obj, &r);
          Nlm_DoAdjustPrnt ((Nlm_GraphiC) obj, &r, FALSE, TRUE);
        }
        break;
      case ALIGN_UPPER:
        if (r.top < minY) {
          r.top = minY;
          Nlm_SetPosition (obj, &r);
          Nlm_DoAdjustPrnt ((Nlm_GraphiC) obj, &r, FALSE, TRUE);
        }
        break;
      case ALIGN_LOWER:
        if (r.bottom < maxY) {
          r.bottom = maxY;
          Nlm_SetPosition (obj, &r);
          Nlm_DoAdjustPrnt ((Nlm_GraphiC) obj, &r, FALSE, TRUE);
        }
        break;
      case ALIGN_MIDDLE:
        delta = maxY - r.bottom;
        if (delta > 0) {
          Nlm_OffsetRect (&r, 0, (Nlm_Int2)(delta/2));
          Nlm_SetPosition (obj, &r);
          Nlm_DoAdjustPrnt ((Nlm_GraphiC) obj, &r, FALSE, TRUE);
        }
        break;
      case ALIGN_VERTICAL:
        if (r.top < minY || r.bottom < maxY) {
          r.top = minY;
          r.bottom = maxY;
          Nlm_SetPosition (obj, &r);
          Nlm_DoAdjustPrnt ((Nlm_GraphiC) obj, &r, FALSE, TRUE);
        }
        break;
      default:
        break;
    }
    obj = (Nlm_Handle) va_arg (args, Nlm_HANDLE);
  }
  va_end(args);
}

static void Nlm_DrawSwitchProc (Nlm_PaneL s)

{
  Nlm_RecT        dn;
  Nlm_SwitchData  extra;
  Nlm_Int2        mid;
  Nlm_RecT        r;
  Nlm_Char        str [32];
  Nlm_RecT        tx;
  Nlm_RecT        up;

  Nlm_GetPanelExtra (s, &extra);
  Nlm_ObjectRect (s, &r);
  if (extra.text && extra.max > 0 && extra.val > 0) {
    sprintf (str, "%d/%d", (int) extra.val, (int) extra.max);
    Nlm_SelectFont (Nlm_programFont);
    if (extra.vert) {
      Nlm_LoadRect (&tx, (Nlm_Int2)(r.left  +  1), (Nlm_Int2)(r.top    + 1),
                         (Nlm_Int2)(r.right - 11), (Nlm_Int2)(r.bottom - 1));
    } else {
      Nlm_LoadRect (&tx, (Nlm_Int2)(r.left  +  1), (Nlm_Int2)(r.top    + 1),
                         (Nlm_Int2)(r.right - 20), (Nlm_Int2)(r.bottom - 1));
    }
    Nlm_DrawString (&tx, str, 'r', FALSE);
    Nlm_SelectFont (Nlm_systemFont);
  }
  Nlm_FrameRect (&r);
  if (extra.vert) {
    Nlm_LoadRect (&up, (Nlm_Int2)(r.right - 9), (Nlm_Int2)(r.top + 2),
                       (Nlm_Int2)(r.right - 2), (Nlm_Int2)(r.top + 9));
    if (extra.upActv) {
      Nlm_CopyBits (&up, upFillArrow);
    } else {
      Nlm_CopyBits (&up, upOpenArrow);
    }
  } else {
    mid = (r.top + r.bottom) / 2;
    Nlm_LoadRect (&up, (Nlm_Int2)(r.right - 9), (Nlm_Int2)(mid - 3),
                       (Nlm_Int2)(r.right - 2), (Nlm_Int2)(mid + 4));
    if (extra.upActv) {
      Nlm_CopyBits (&up, rightFillArrow);
    } else {
      Nlm_CopyBits (&up, rightOpenArrow);
    }
  }
  if (extra.vert) {
    Nlm_LoadRect (&dn, (Nlm_Int2)(r.right - 9), (Nlm_Int2)(r.bottom - 9),
                       (Nlm_Int2)(r.right - 2), (Nlm_Int2)(r.bottom - 2));
    if (extra.dnActv) {
      Nlm_CopyBits (&dn, downFillArrow);
    } else {
      Nlm_CopyBits (&dn, downOpenArrow);
    }
  } else {
    mid = (r.top + r.bottom) / 2;
    Nlm_LoadRect (&dn, (Nlm_Int2)(r.right - 18), (Nlm_Int2)(mid - 3),
                       (Nlm_Int2)(r.right - 11), (Nlm_Int2)(mid + 4));
    if (extra.dnActv) {
      Nlm_CopyBits (&dn, leftFillArrow);
    } else {
      Nlm_CopyBits (&dn, leftOpenArrow);
    }
  }
}

static void Nlm_SwitchClickProc (Nlm_PaneL s, Nlm_PoinT pt)

{
  Nlm_RecT        dn;
  Nlm_SwitchData  extra;
  Nlm_Int2        mid;
  Nlm_RecT        r;
  Nlm_RecT        up;

  Nlm_GetPanelExtra (s, &extra);
  Nlm_ObjectRect (s, &r);
  if (extra.vert) {
    Nlm_LoadRect (&up, (Nlm_Int2)(r.right - 9), (Nlm_Int2)(r.top + 2),
                       (Nlm_Int2)(r.right - 2), (Nlm_Int2)(r.top + 9));
    Nlm_LoadRect (&dn, (Nlm_Int2)(r.right - 9), (Nlm_Int2)(r.bottom - 9),
                       (Nlm_Int2)(r.right - 2), (Nlm_Int2)(r.bottom - 2));
  } else {
    mid = (r.top + r.bottom) / 2;
    Nlm_LoadRect (&up, (Nlm_Int2)(r.right - 9), (Nlm_Int2)(mid - 3),
                       (Nlm_Int2)(r.right - 2), (Nlm_Int2)(mid + 4));
    Nlm_LoadRect (&dn, (Nlm_Int2)(r.right - 18), (Nlm_Int2)(mid - 3),
                       (Nlm_Int2)(r.right - 11), (Nlm_Int2)(mid + 4));
  }
  if (Nlm_PtInRect (pt, &up)) {
    if (extra.val < extra.max) {
      Nlm_DoSetValue ((Nlm_GraphiC) s, (Nlm_Int2)(extra.val + 1), FALSE);
      if (extra.actn != NULL) {
        extra.actn ((Nlm_SwitcH) s, (Nlm_Int2)(extra.val + 1), extra.val);
      }
    }
  } else if (Nlm_PtInRect (pt, &dn)) {
    if (extra.val > 1) {
      Nlm_DoSetValue ((Nlm_GraphiC) s, (Nlm_Int2)(extra.val - 1), FALSE);
      if (extra.actn != NULL) {
        extra.actn ((Nlm_SwitcH) s, (Nlm_Int2)(extra.val - 1), extra.val);
      }
    }
  }
}

static Nlm_SwitcH Nlm_CommonSwitch (Nlm_GrouP prnt, Nlm_Boolean text,
                                    Nlm_Boolean vert, Nlm_SwtChngProc actn)

{
  Nlm_SwitchData  extra;
  Nlm_Int2        height;
  Nlm_PaneL       s;
  Nlm_Int2        width;

  s = NULL;
  if (prnt != NULL) {
    Nlm_SelectFont (Nlm_programFont);
    if (vert) {
      height = 20;
      width = 11;
    } else {
      height = 11;
      width = 20;
    }
    if (text) {
      width += Nlm_StringWidth ("99/99") + 4;
      height = MAX (height, Nlm_LineHeight () + 2);
    }
    Nlm_SelectFont (Nlm_systemFont);
    s = Nlm_AutonomousPanel (prnt, width, height, Nlm_DrawSwitchProc, NULL,
                             NULL, sizeof (Nlm_SwitchData), NULL, switchProcs);
    if (s != NULL) {
      Nlm_SetPanelClick (s, Nlm_SwitchClickProc, NULL, NULL, NULL);
      Nlm_MemSet ((Nlm_VoidPtr) (&extra), 0, sizeof (Nlm_SwitchData));
      extra.max = 0;
      extra.val = 0;
      extra.text = text;
      extra.vert = vert;
      extra.upActv = FALSE;
      extra.dnActv = FALSE;
      extra.actn = actn;
      Nlm_SetPanelExtra (s, &extra);
    }
  }
  return (Nlm_SwitcH) s;
}

extern Nlm_SwitcH Nlm_UpDownSwitch (Nlm_GrouP prnt, Nlm_Boolean text, Nlm_SwtChngProc actn)

{
  return Nlm_CommonSwitch (prnt, text, TRUE, actn);
}

extern Nlm_SwitcH Nlm_LeftRightSwitch (Nlm_GrouP prnt, Nlm_Boolean text, Nlm_SwtChngProc actn)

{
  return Nlm_CommonSwitch (prnt, text, FALSE, actn);
}

static void Nlm_UpdateSwitch (Nlm_SwitcH s)

{
  Nlm_RecT        dn;
  Nlm_SwitchData  extra;
  Nlm_Int2        mid;
  Nlm_RecT        r;
  Nlm_RecT        tx;
  Nlm_RecT        up;

  Nlm_GetPanelExtra ((Nlm_PaneL) s, &extra);
  if (Nlm_Visible (s) && Nlm_AllParentsVisible (s)) {
    Nlm_ObjectRect (s, &r);
    if (extra.vert) {
      Nlm_LoadRect (&up, (Nlm_Int2)(r.right - 9), (Nlm_Int2)(r.top + 2),
                         (Nlm_Int2)(r.right - 2), (Nlm_Int2)(r.top + 9));
      Nlm_LoadRect (&dn, (Nlm_Int2)(r.right - 9), (Nlm_Int2)(r.bottom - 9),
                         (Nlm_Int2)(r.right - 2), (Nlm_Int2)(r.bottom - 2));
    } else {
      mid = (r.top + r.bottom) / 2;
      Nlm_LoadRect (&up, (Nlm_Int2)(r.right - 9), (Nlm_Int2)(mid - 3),
                         (Nlm_Int2)(r.right - 2), (Nlm_Int2)(mid + 4));
      Nlm_LoadRect (&dn, (Nlm_Int2)(r.right - 18), (Nlm_Int2)(mid - 3),
                         (Nlm_Int2)(r.right - 11), (Nlm_Int2)(mid + 4));
    }
    Nlm_Select (s);
    if (extra.upActv != (Nlm_Boolean) (extra.val < extra.max)) {
      Nlm_InsetRect (&up, -1, -1);
      Nlm_InvalRect (&up);
    }
    if (extra.dnActv != (Nlm_Boolean) (extra.val > 1)) {
      Nlm_InsetRect (&dn, -1, -1);
      Nlm_InvalRect (&dn);
    }
    if (extra.text) {
      if (extra.vert) {
        Nlm_LoadRect (&tx, (Nlm_Int2)(r.left  +  1), (Nlm_Int2)(r.top    + 1),
                           (Nlm_Int2)(r.right - 10), (Nlm_Int2)(r.bottom - 1));
      } else {
        Nlm_LoadRect (&tx, (Nlm_Int2)(r.left  +  1), (Nlm_Int2)(r.top + 1),
                           (Nlm_Int2)(r.right - 19), (Nlm_Int2)(r.bottom - 1));
      }
      Nlm_InvalRect (&tx);
    }
  }
  extra.upActv = (Nlm_Boolean) (extra.val < extra.max);
  extra.dnActv = (Nlm_Boolean) (extra.val > 1);
  Nlm_SetPanelExtra ((Nlm_PaneL) s, &extra);
}

static void Nlm_SetSwitchValue (Nlm_GraphiC s, Nlm_Int2 value, Nlm_Boolean savePort)

{
  Nlm_SwitchData  extra;
  Nlm_Int2        oldval;
  Nlm_WindoW      tempPort;

  tempPort = Nlm_SavePortIfNeeded (s, savePort);
  if (s != NULL) {
    Nlm_GetPanelExtra ((Nlm_PaneL) s, &extra);
    oldval = extra.val;
    extra.val = value;
    Nlm_SetPanelExtra ((Nlm_PaneL) s, &extra);
    if (oldval != value) {
      Nlm_UpdateSwitch ((Nlm_SwitcH) s);
    }
  }
  Nlm_RestorePort (tempPort);
}

static Nlm_Int2 Nlm_GetSwitchValue (Nlm_GraphiC s)

{
  Nlm_SwitchData  extra;
  Nlm_Int2        value;

  value = 0;
  if (s != NULL) {
    Nlm_GetPanelExtra ((Nlm_PaneL) s, &extra);
    value = extra.val;
  }
  return value;
}

extern void Nlm_SetSwitchMax (Nlm_SwitcH s, Nlm_Int2 max)

{
  Nlm_SwitchData  extra;
  Nlm_Int2        oldmax;
  Nlm_WindoW      tempPort;

  tempPort = Nlm_SavePort (s);
  if (s != NULL) {
    Nlm_GetPanelExtra ((Nlm_PaneL) s, &extra);
    oldmax = extra.max;
    extra.max = max;
    if (max == 0) {
      extra.val = 0;
    }
    Nlm_SetPanelExtra ((Nlm_PaneL) s, &extra);
    if (oldmax != max) {
      Nlm_UpdateSwitch (s);
    }
  }
  Nlm_RestorePort (tempPort);
}

extern Nlm_Int2 Nlm_GetSwitchMax (Nlm_SwitcH s)

{
  Nlm_SwitchData  extra;
  Nlm_Int2        max;

  max = 0;
  if (s != NULL) {
    Nlm_GetPanelExtra ((Nlm_PaneL) s, &extra);
    max = extra.max;
  }
  return max;
}

extern void Nlm_SetSwitchParams (Nlm_SwitcH s, Nlm_Int2 value, Nlm_Int2 max)

{
  Nlm_SwitchData  extra;
  Nlm_Int2        oldmax;
  Nlm_Int2        oldval;
  Nlm_WindoW      tempPort;

  tempPort = Nlm_SavePort (s);
  if (s != NULL) {
    Nlm_GetPanelExtra ((Nlm_PaneL) s, &extra);
    oldmax = extra.max;
    extra.max = max;
    if (value > max) {
      value = max;
    }
    oldval = extra.val;
    extra.val = value;
    Nlm_SetPanelExtra ((Nlm_PaneL) s, &extra);
    if (oldval != value || oldmax != max) {
      Nlm_UpdateSwitch (s);
    }
  }
  Nlm_RestorePort (tempPort);
}

static void Nlm_DoRepeatAction (Nlm_RepeaT rb, Nlm_PoinT pt)

{
  Nlm_RptClckProc  actn;
  Nlm_RepeatData   rdata;

  Nlm_GetPanelExtra ((Nlm_PaneL) rb, &rdata);
  actn = rdata.action;
  if (actn != NULL) {
    actn (rb, pt);
  }
}

static void Nlm_RepeatClick (Nlm_PaneL rb, Nlm_PoinT pt)

{
  Nlm_RecT  r;

  Nlm_GetRect ((Nlm_GraphiC) rb, &r);
  if (Nlm_PtInRect (pt, &r)) {
    inRepeatButton = TRUE;
    Nlm_InsetRect (&r, 2, 2);
    Nlm_InvertRect (&r);
    Nlm_DoRepeatAction ((Nlm_RepeaT) rb, pt);
  }
}

static void Nlm_RepeatPress (Nlm_PaneL rb, Nlm_PoinT pt)

{
  Nlm_RecT  r;

  Nlm_GetRect ((Nlm_GraphiC) rb, &r);
  if (Nlm_PtInRect (pt, &r)) {
    if (! inRepeatButton) {
      inRepeatButton = TRUE;
      Nlm_InsetRect (&r, 2, 2);
      Nlm_InvertRect (&r);
    }
    Nlm_DoRepeatAction ((Nlm_RepeaT) rb, pt);
  } else if (inRepeatButton) {
    inRepeatButton = FALSE;
    Nlm_InsetRect (&r, 2, 2);
    Nlm_InvertRect (&r);
  }
}

static void Nlm_RepeatRelease (Nlm_PaneL rb, Nlm_PoinT pt)

{
  Nlm_RecT  r;

  Nlm_GetRect ((Nlm_GraphiC) rb, &r);
  if (inRepeatButton) {
    inRepeatButton = FALSE;
    Nlm_InsetRect (&r, 2, 2);
    Nlm_InvertRect (&r);
  }
}

static void Nlm_DrawRepeat (Nlm_PaneL rb)

{
  Nlm_Handle      h;
  Nlm_RecT        r;
  Nlm_RepeatData  rdata;
  Nlm_Char        str [64];

  if (Nlm_GetVisible ((Nlm_GraphiC) rb) && Nlm_GetAllParentsVisible ((Nlm_GraphiC) rb)) {
    Nlm_GetRect ((Nlm_GraphiC) rb, &r);
    Nlm_FrameRect (&r);
    Nlm_InsetRect (&r, 1, 1);
    Nlm_FrameRect (&r);
    Nlm_InsetRect (&r, 1, 1);
    Nlm_GetPanelExtra (rb, &rdata);
    h = rdata.title;
    if (h != NULL) {
      Nlm_SelectFont (Nlm_systemFont);
      Nlm_GetString (h, str, sizeof (str));
      if (Nlm_StringLen (str) > 0) {
        Nlm_DrawString (&r, str, 'c', FALSE);
      }
    }
  }
}

static void Nlm_SetRepeatTitle (Nlm_GraphiC rb, Nlm_Int2 item,
                                Nlm_CharPtr title, Nlm_Boolean savePort)

{
  Nlm_Handle      h;
  Nlm_RecT        r;
  Nlm_RepeatData  rdata;
  Nlm_WindoW      tempPort;

  tempPort = Nlm_SavePortIfNeeded (rb, savePort);
  Nlm_GetPanelExtra ((Nlm_PaneL) rb, &rdata);
  h = rdata.title;
  h = Nlm_SetString (h, title);
  rdata.title = h;
  Nlm_SetPanelExtra ((Nlm_PaneL) rb, &rdata);
  if (Nlm_GetVisible (rb) && Nlm_GetAllParentsVisible (rb)) {
    Nlm_GetRect (rb, &r);
    Nlm_InvalRect (&r);
  }
  Nlm_RestorePort (tempPort);
}

static void Nlm_GetRepeatTitle (Nlm_GraphiC rb, Nlm_Int2 item,
                                Nlm_CharPtr title, size_t maxsize)

{
  Nlm_Handle      h;
  Nlm_RepeatData  rdata;

  if (title != NULL) {
    Nlm_GetPanelExtra ((Nlm_PaneL) rb, &rdata);
    h = rdata.title;
    Nlm_GetString (h, title, maxsize);
  }
}

static void Nlm_ResetRepeat (Nlm_PaneL rb)

{
  Nlm_RepeatData  rdata;

  Nlm_GetPanelExtra (rb, &rdata);
  if (rdata.title != NULL) {
    rdata.title = Nlm_HandFree (rdata.title);
  }
  Nlm_SetPanelExtra (rb, &rdata);
}

extern Nlm_RepeaT Nlm_RepeatButton (Nlm_GrouP prnt, Nlm_CharPtr title,
                                    Nlm_RptClckProc actn)

{
  Nlm_RepeaT      rb;
  Nlm_RepeatData  rdata;
  Nlm_Int2        vbounds;
  Nlm_Int2        width;

  rb = NULL;
  if (prnt != NULL) {
    Nlm_SelectFont (Nlm_systemFont);
    width = Nlm_StringWidth (title);
#ifdef WIN_MAC
    vbounds = 2;
#endif
#ifdef WIN_MSWIN
    vbounds = 4;
#endif
#ifdef WIN_MOTIF
    vbounds = 2;
#endif
    rb = (Nlm_RepeaT) Nlm_AutonomousPanel (prnt,
      (Nlm_Int2)(width + 8), (Nlm_Int2)(Nlm_stdLineHeight + vbounds*2),
      Nlm_DrawRepeat, NULL, NULL,
      sizeof (Nlm_RepeatData), Nlm_ResetRepeat, repeatProcs);
    if (rb != NULL) {
      Nlm_SetPanelClick ((Nlm_PaneL) rb, Nlm_RepeatClick, NULL,
                         Nlm_RepeatPress, Nlm_RepeatRelease);
      rdata.title = NULL;
      rdata.action = actn;
      Nlm_SetPanelExtra ((Nlm_PaneL) rb, &rdata);
      Nlm_SetRepeatTitle ((Nlm_GraphiC) rb, 0, title, FALSE);
    }
  }
  return rb;
}

static void Nlm_SetIconValue (Nlm_GraphiC i, Nlm_Int2 value, Nlm_Boolean savePort)

{
  Nlm_IconData  extra;
  Nlm_Int2      oldval;
  Nlm_RecT      r;
  Nlm_WindoW    tempPort;

  tempPort = Nlm_SavePortIfNeeded (i, savePort);
  if (i != NULL) {
    Nlm_GetPanelExtra ((Nlm_PaneL) i, &extra);
    oldval = extra.value;
    extra.value = value;
    Nlm_SetPanelExtra ((Nlm_PaneL) i, &extra);
    if (oldval != value) {
      if (Nlm_Visible (i) && Nlm_AllParentsVisible (i)) {
        Nlm_Select (i);
        if (extra.inval != NULL) {
          extra.inval ((Nlm_IcoN) i, value, oldval);
        } else {
          Nlm_ObjectRect (i, &r);
          Nlm_InsetRect (&r, -1, -1);
          Nlm_InvalRect (&r);
        }
      }
    }
  }
  Nlm_RestorePort (tempPort);
}

static Nlm_Int2 Nlm_GetIconValue (Nlm_GraphiC i)

{
  Nlm_IconData  extra;
  Nlm_Int2      value;

  value = 0;
  if (i != NULL) {
    Nlm_GetPanelExtra ((Nlm_PaneL) i, &extra);
    value = extra.value;
  }
  return value;
}

static void Nlm_SetIconStatus (Nlm_GraphiC i, Nlm_Int2 item,
                               Nlm_Boolean set, Nlm_Boolean savePort)

{
  Nlm_IconData  extra;
  Nlm_Boolean   oldstat;
  Nlm_RecT      r;
  Nlm_WindoW    tempPort;

  tempPort = Nlm_SavePortIfNeeded (i, savePort);
  if (i != NULL) {
    Nlm_GetPanelExtra ((Nlm_PaneL) i, &extra);
    oldstat = extra.status;
    extra.status = set;
    Nlm_SetPanelExtra ((Nlm_PaneL) i, &extra);
    if (oldstat != set) {
      if (Nlm_Visible (i) && Nlm_AllParentsVisible (i)) {
        Nlm_Select (i);
        if (extra.inval != NULL) {
          extra.inval ((Nlm_IcoN) i, extra.value, extra.value);
        } else {
          Nlm_ObjectRect (i, &r);
          Nlm_InsetRect (&r, -1, -1);
          Nlm_InvalRect (&r);
        }
      }
    }
  }
  Nlm_RestorePort (tempPort);
}

static Nlm_Boolean Nlm_GetIconStatus (Nlm_GraphiC i, Nlm_Int2 item)

{
  Nlm_IconData  extra;
  Nlm_Boolean   status;

  status = 0;
  if (i != NULL) {
    Nlm_GetPanelExtra ((Nlm_PaneL) i, &extra);
    status = extra.status;
  }
  return status;
}

static void Nlm_SetIconTitle (Nlm_GraphiC i, Nlm_Int2 item,
                              Nlm_CharPtr title, Nlm_Boolean savePort)

{
  Nlm_IconData  extra;
  Nlm_Handle    h;
  Nlm_RecT      r;
  Nlm_WindoW    tempPort;

  tempPort = Nlm_SavePortIfNeeded (i, savePort);
  Nlm_GetPanelExtra ((Nlm_PaneL) i, &extra);
  h = extra.title;
  h = Nlm_SetString (h, title);
  extra.title = h;
  Nlm_SetPanelExtra ((Nlm_PaneL) i, &extra);
  if (Nlm_GetVisible (i) && Nlm_GetAllParentsVisible (i)) {
    Nlm_GetRect (i, &r);
    Nlm_InvalRect (&r);
  }
  Nlm_RestorePort (tempPort);
}

static void Nlm_GetIconTitle (Nlm_GraphiC i, Nlm_Int2 item,
                              Nlm_CharPtr title, size_t maxsize)

{
  Nlm_IconData  extra;
  Nlm_Handle    h;

  if (title != NULL) {
    Nlm_GetPanelExtra ((Nlm_PaneL) i, &extra);
    h = extra.title;
    Nlm_GetString (h, title, maxsize);
  }
}

static void Nlm_ResetIcon (Nlm_PaneL i)

{
  Nlm_IconData  extra;

  Nlm_GetPanelExtra (i, &extra);
  if (extra.title != NULL) {
    extra.title = Nlm_HandFree (extra.title);
  }
  Nlm_SetPanelExtra (i, &extra);
}

extern Nlm_IcoN Nlm_IconButton (Nlm_GrouP prnt, Nlm_Int2 pixwidth, Nlm_Int2 pixheight,
                                Nlm_IcnActnProc draw, Nlm_IcnChngProc inval,
                                Nlm_IcnClckProc click, Nlm_IcnClckProc drag,
                                Nlm_IcnClckProc hold, Nlm_IcnClckProc release)

{
  Nlm_IcoN      ic;
  Nlm_IconData  idata;

  ic = NULL;
  if (prnt != NULL) {
    Nlm_SelectFont (Nlm_systemFont);
    ic = (Nlm_IcoN) Nlm_AutonomousPanel (prnt, pixwidth, pixheight,
                                         (Nlm_PnlActnProc) draw, NULL, NULL,
                                         sizeof (Nlm_IconData), Nlm_ResetIcon,
                                         iconProcs);
    if (ic != NULL) {
      Nlm_SetPanelClick ((Nlm_PaneL) ic, (Nlm_PnlClckProc) click, (Nlm_PnlClckProc) drag,
                         (Nlm_PnlClckProc) hold, (Nlm_PnlClckProc) release);
      idata.inval = inval;
      idata.value = 0;
      idata.status = FALSE;
      idata.title = NULL;
      Nlm_SetPanelExtra ((Nlm_PaneL) ic, &idata);
    }
  }
  return ic;
}

extern void Nlm_SetFolderTabValue (Nlm_GraphiC a, Nlm_Int2 value, Nlm_Boolean savePort);
extern void Nlm_SetFolderTabSubclass (Nlm_GrouP g);
extern void Nlm_SetFolderTabSubclass (Nlm_GrouP g)

{
  Nlm_GraphicData  gdata;

  if (Nlm_folderTabProcs != NULL) {
    Nlm_folderTabProcs->setValue = Nlm_SetFolderTabValue;
    Nlm_GetGraphicData ((Nlm_GraphiC) g, &gdata);
    Nlm_folderTabProcs->ancestor = gdata.classptr;
    gdata.classptr = Nlm_folderTabProcs;
    Nlm_SetGraphicData ((Nlm_GraphiC) g, &gdata);
  }
}

extern void Nlm_FreeExtras (void)

{
  gphprcsptr = (Nlm_GphPrcsPtr) Nlm_MemFree (gphprcsptr);
}

extern void Nlm_InitExtras (void)

{
  gphprcsptr = (Nlm_GphPrcsPtr) Nlm_MemNew (sizeof (Nlm_GphPrcs) * 4);

  repeatProcs = &(gphprcsptr [0]);
  repeatProcs->setTitle = Nlm_SetRepeatTitle;
  repeatProcs->getTitle = Nlm_GetRepeatTitle;

  switchProcs = &(gphprcsptr [1]);
  switchProcs->setValue = Nlm_SetSwitchValue;
  switchProcs->getValue = Nlm_GetSwitchValue;

  iconProcs = &(gphprcsptr [2]);
  iconProcs->setValue = Nlm_SetIconValue;
  iconProcs->getValue = Nlm_GetIconValue;
  iconProcs->setStatus = Nlm_SetIconStatus;
  iconProcs->getStatus = Nlm_GetIconStatus;
  iconProcs->setTitle = Nlm_SetIconTitle;
  iconProcs->getTitle = Nlm_GetIconTitle;

  Nlm_folderTabProcs = &(gphprcsptr [3]);
}

/* ---------------------- Start ------------------------- */
/* --------------------MS-Windows------------------------ */
#ifdef WIN_MSWIN

extern HWND Nlm_currentHWnd;

static DWORD win_CustomColors[16];

extern Nlm_Boolean Nlm_ChooseColorDialog ( 
       Nlm_Uint1 PNTR redptr,
       Nlm_Uint1 PNTR greenptr, 
       Nlm_Uint1 PNTR blueptr, 
       Nlm_Boolean Initial ){
  CHOOSECOLOR chc;
  Nlm_Boolean res;

  chc.lStructSize = sizeof (CHOOSECOLOR);
  chc.hwndOwner = Nlm_currentHWnd;
  chc.lpCustColors = win_CustomColors;
  chc.Flags = CC_FULLOPEN;
  if ( Initial ) {
    chc.Flags |= CC_RGBINIT;
    chc.rgbResult = RGB ( *redptr, *greenptr, *blueptr );
  }
  res = (Nlm_Boolean)ChooseColor ( &chc );
  if ( res ){
    if ( redptr != NULL ) *redptr = GetRValue (chc.rgbResult);
    if ( greenptr != NULL ) *greenptr = GetGValue (chc.rgbResult);
    if ( blueptr != NULL ) *blueptr = GetBValue (chc.rgbResult);
  }
  return res;
}
/* ---------------------End MS-Windows--------------------- */
#else
/* --------------------- Non MS-Window -------------------- */
/* --------------------- Defines -------------------------- */
/* Sizes */
#define NLM_CHCOLOR_BOXW   26
#define NLM_CHCOLOR_BOXH   20
#define NLM_CHCOLOR_SHLW   180
#define NLM_CHCOLOR_SHLH   20
#define NLM_CHCOLOR_PTRH   10
#define NLM_CHCOLOR_PTRW   16
#define NLM_CHCOLOR_PTRW2  8
#define NLM_CHCOLOR_EXW    160
#define NLM_CHCOLOR_EXH    40
/* Owners */
#define NLM_CHCOLOR_OBC    0
#define NLM_CHCOLOR_OCC    1
#define NLM_CHCOLOR_OSP    2
#define NLM_CHCOLOR_OST    3
#define NLM_CHCOLOR_OHP    4
#define NLM_CHCOLOR_OHT    5
#define NLM_CHCOLOR_OLP    6
#define NLM_CHCOLOR_OLT    7
#define NLM_CHCOLOR_ORT    8
#define NLM_CHCOLOR_OGT    9
#define NLM_CHCOLOR_OBT    10

/* --------------------- Typedefs ------------------------- */

typedef struct {
  Nlm_Uint1  red;
  Nlm_Uint1  green;
  Nlm_Uint1  blue;
} NLMINTERNALCOLOR;

/* -------------------Global variable---------------------- */

#ifdef WIN_X
extern Display      *Nlm_currentXDisplay;
extern int          Nlm_currentXScreen;
extern Window       Nlm_currentXWindow;
extern GC           Nlm_currentXGC;
extern Nlm_Uint4    Nlm_XbackColor;
extern Nlm_Uint4    Nlm_XforeColor;
extern Nlm_Int2     Nlm_XOffset;
extern Nlm_Int2     Nlm_YOffset;
extern Nlm_RegioN   Nlm_clpRgn;
extern Nlm_Boolean  Nlm_hasColor;
#endif

/* -------------------Static variable---------------------- */

static Nlm_Boolean  colorDlgUp;
static Nlm_Boolean  colorDlgOK;
static Nlm_PaneL    pBasicColor;
static Nlm_PaneL    pCustomColor;
static Nlm_PaneL    pSColor;
static Nlm_PaneL    pSPtrColor;
static Nlm_PaneL    pHColor;
static Nlm_PaneL    pHPtrColor;
static Nlm_PaneL    pLColor;
static Nlm_PaneL    pLPtrColor;
static Nlm_PaneL    pRetColor;
static Nlm_TexT     tColorRed;
static Nlm_TexT     tColorGreen;
static Nlm_TexT     tColorBlue;
static Nlm_TexT     tColorHue;
static Nlm_TexT     tColorSat;
static Nlm_TexT     tColorLum;
static Nlm_Int2     stColorRGB[3];
static Nlm_Int2     stColorHSL[3];
static Nlm_Int2     stColorCurCustom;
static Nlm_Int2     stColorModel;
#ifdef WIN_X
#define VOID_PIXEL ((unsigned long)(-1))
static unsigned long tmpColorPixel = VOID_PIXEL;
#endif

static NLMINTERNALCOLOR theCustomColors[16];

static NLMINTERNALCOLOR theBasicColors[48] = {
{255,128,128},{255,255,232},{128,255,128},{0,255,128},
{128,255,255},{0,128,255},  {255,128,192},{255,128,255},
{255,0,0},    {255,255,128},{128,255,0},  {0,255,64},
{0,255,255},  {0,128,192},  {128,128,192},{255,0,255},
{128,64,64},  {255,255,0},  {0,255,0},    {0,128,128},
{0,64,128},   {128,128,255},{128,0,64},   {255,0,128},
{128,0,0},    {255,128,0},  {0,128,0},    {0,128,64},
{0,0,255},    {0,0,160},    {128,0,128},  {128,0,255},
{64,0,0},     {128,64,0},   {0,64,0},     {0,64,64,},
{0,0,128},    {0,0,64},     {64,0,64},    {64,0,128},
{0,0,0},      {128,128,0},  {128,128,64}, {128,128,128},
{64,128,128}, {192,192,192},{130,130,130},{255,255,255} };

static Nlm_Uint1 thePointer[] = {   /*16 X 10*/
0x01, 0x80, 0x03, 0xC0, 0x07, 0xE0,
0x0F, 0xF0, 0x1F, 0xF8, 0x3F, 0xFC,
0x7F, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF };


/* ----------------Utility functions ---------------------- */
static void Nlm_SafeSetTitleExtra (Nlm_Handle a, Nlm_CharPtr title)

{
  Nlm_Char  str [256];

  Nlm_GetTitle (a, str, sizeof (str));
  if (Nlm_StringCmp (title, str) != 0) {
    Nlm_SetTitle (a, title);
  }
}

static int Nlm_GetColorVal ( Nlm_TexT t ) {
  Nlm_Char cValue[64];
  Nlm_Char PNTR cValuePtr;
  int      iValue = -1;
  int      update = 0;

  Nlm_GetTitle( t, cValue, 64 );
  cValue[63] = 0;
  for ( cValuePtr = cValue; IS_DIGIT(*cValuePtr); cValuePtr++ ) ;
  if ( *cValuePtr != '\0' ) {
    *cValuePtr = 0;
    update = 1;
  }
  sscanf ( cValue, "%d", &iValue );
  if ( iValue <= 0 ) {
    iValue = 0;
    update = 2;
  }  
  if ( iValue > 255 ) {
    iValue = 255 ;
    update = 1;
  } 
  if ( update ==2 ){ 
    Nlm_SetTitle ( t, "" );
  } else if ( update ) {
    sprintf ( cValue, "%d", iValue );
    Nlm_SafeSetTitleExtra ( t, cValue );
  }
  return iValue;
}

static void Nlm_HSL2RGB ( Nlm_Int2 PNTR rgb, Nlm_Int2 PNTR hsl ){
  int      i, min;
  double   maxVal, minVal;
  double   sumDoub, difDoub;

  /* S = (maxRGB-minRGB)/(maxRGB+minRGB)*256 */
  /* L = (maxRGB+minRGB)/2 */
  /* H = (maxRGB-minRGB)/(maxRGB+midRGB-2*minRGB)*(256/3) + C */
  sumDoub = (double)hsl[2] * 2;
  difDoub = (double)hsl[1] * sumDoub / 256.0;
  maxVal = (sumDoub + difDoub) / 2.0;
  if ( maxVal > 255 ) maxVal = 255;
  minVal = sumDoub - maxVal;
  if ( (double)hsl[0] > (256.0*2.0/3.0) ) {
    min = 1; difDoub = (double)hsl[0] - (256.0*2.0/3.0);
  } else if ( (double)hsl[0] > (256.0*1.0/3.0) ){
    min = 0; difDoub = (double)hsl[0] - (256.0*1.0/3.0);
  } else {
    min = 2; difDoub = (double)hsl[0];
  }
  rgb[min] = (Nlm_Int2)minVal;
  difDoub *= 3.0/256.0;
  if ( difDoub < 0.5 ){
    i = min + 1; if ( i > 2 ) i=0;
    rgb[i] = (Nlm_Int2)maxVal;
    i++; if ( i > 2 ) i=0;
    difDoub = 1.0 - difDoub;
  }else{
    i = min - 1; if ( i < 0 ) i=2;
    rgb[i] = (Nlm_Int2)maxVal;
    i --; if ( i < 0 ) i=2;
  }
  if ( (maxVal-minVal) < 1.0E-12 ) {
    rgb[i] = (Nlm_Int2)minVal;
  } else {
    rgb[i] = (Nlm_Int2)((maxVal-minVal)/difDoub - maxVal + 2*minVal);
  }
}

static void Nlm_RGB2HSL ( Nlm_Int2 PNTR hsl, Nlm_Int2 PNTR rgb ){
  int      i, min;
  double   maxVal, minVal;
  double   sumDoub, difDoub;
  
  /* S = (maxRGB-minRGB)/(maxRGB+minRGB)*256 */
  /* L = (maxRGB+minRGB)/2 */
  /* H = (maxRGB-minRGB)/(maxRGB+midRGB-2*minRGB)*(256/3) + C */
  maxVal = -1;
  minVal = 256;
  for ( i=0; i<3; i++ ){
    if ( rgb[i] < minVal ) {
      minVal = (double)rgb[i];
      min = i;
    }
    if ( rgb[i] > maxVal ) maxVal = (double)rgb[i];
  }
  sumDoub = minVal + maxVal;
  difDoub = maxVal - minVal;
  if ( sumDoub < 1.0E-10 ) hsl[1] = 255;
  else hsl[1] = (Nlm_Int2)( difDoub * 256.0/sumDoub );
  if ( hsl[1] > 255 ) hsl[1] = 255;
  hsl[2] = (Nlm_Int2)(sumDoub/ 2.0);
  if ( hsl[2] > 255 ) hsl[2] = 255;
  if ( (rgb[0] == rgb[1]) && (rgb[0] == rgb[2]) ) hsl[0] = 0;
  else {
    i = (min + 1); if ( i > 2 ) i=0;
    difDoub = (double)rgb[i] - minVal;
    sumDoub = (double)rgb[i];
    i++; if ( i > 2 ) i=0;
    sumDoub += (double)rgb[i] - 2*minVal;
    i = min + 1; if ( i > 2 ) i=0;
    hsl[0] = (Nlm_Int2)( 256.0/3.0 *((double)i + (1.0-difDoub/sumDoub)) );
    hsl[0] += 1;
    if ( hsl[0] > 255 ) hsl[0] = 0;
  }
}

static void Nlm_UpdColorPanel ( Nlm_PaneL p ){
  Nlm_RecT   rp;
  Nlm_WindoW tmpPort;

  tmpPort = Nlm_SavePort (p);
  Nlm_ObjectRect (p, &rp);
  Nlm_Select (p); 
  Nlm_InvalRect ( &rp );
  Nlm_RestorePort (tmpPort);
}

static void Nlm_UpdColorText ( Nlm_TexT t, Nlm_Int2 iVal ){
  Nlm_Char cValue[64];

  sprintf ( cValue, "%d", iVal );
  Nlm_SafeSetTitleExtra ( t, cValue );
}

static void Nlm_DrawColorPtr ( Nlm_Int2 x, Nlm_Int2 y ){
  Nlm_RecT r;

  Nlm_LoadRect ( &r, x-NLM_CHCOLOR_PTRW2, y, 
                     x+NLM_CHCOLOR_PTRW2, y+NLM_CHCOLOR_PTRH );
  Nlm_CopyBits ( &r, (Nlm_VoidPtr)thePointer );
}

/* -------------------Draw functions ---------------------- */

static void Nlm_BasicColorDraw ( Nlm_PaneL p ){
  int        x, y, xx, yy;
  Nlm_RecT   rp;
  Nlm_RecT   rdraw;

  Nlm_ObjectRect (pBasicColor, &rp);
  for ( y=0; y<6; y++ ){
    for ( x=0; x<8; x++ ){
      xx = rp.left + x*NLM_CHCOLOR_BOXW;
      yy = rp.top + y*NLM_CHCOLOR_BOXH;
      Nlm_LoadRect ( &rdraw, xx + 2, yy + 2,
                     xx + NLM_CHCOLOR_BOXW - 2, yy + NLM_CHCOLOR_BOXH - 2 );
      Nlm_FrameRect ( &rdraw );
    }
  }
  for ( y=0; y<6; y++ ){
    for ( x=0; x<8; x++ ){
      xx = rp.left + x*NLM_CHCOLOR_BOXW;
      yy = rp.top + y*NLM_CHCOLOR_BOXH;
      Nlm_LoadRect ( &rdraw, xx + 3, yy + 3,
                     xx + NLM_CHCOLOR_BOXW - 3, yy + NLM_CHCOLOR_BOXH - 3 );
      Nlm_SelectColor ( theBasicColors[y*8+x].red,
                        theBasicColors[y*8+x].green,
                        theBasicColors[y*8+x].blue );
      Nlm_PaintRect ( &rdraw );
    }
  }
}

static void Nlm_CustomColorDraw ( Nlm_PaneL p ){
  int      x,y,xx,yy;
  Nlm_RecT rp;
  Nlm_RecT rdraw;

  Nlm_ObjectRect (pCustomColor, &rp);
  if ( p != NULL ){
    y = stColorCurCustom / 8;
    x = stColorCurCustom - y*8;
    xx = rp.left + x*NLM_CHCOLOR_BOXW;
    yy = rp.top + y*NLM_CHCOLOR_BOXH;
    Nlm_LoadRect ( &rdraw, xx, yy, 
                   xx + NLM_CHCOLOR_BOXW, yy + NLM_CHCOLOR_BOXH );
    Nlm_InvertMode();
    Nlm_FrameRect ( &rdraw );
    Nlm_CopyMode();
  }
  for ( x=0; x<8; x++ ){
    for ( y=0; y<2; y++ ){
      xx = rp.left + x*NLM_CHCOLOR_BOXW;
      yy = rp.top + y*NLM_CHCOLOR_BOXH;
      Nlm_LoadRect ( &rdraw, xx + 2, yy + 2, 
                     xx + NLM_CHCOLOR_BOXW - 2, yy + NLM_CHCOLOR_BOXH - 2 );
      Nlm_FrameRect ( &rdraw );
    }
  }
  for ( x=0; x<8; x++ ){
    for ( y=0; y<2; y++ ){
      xx = rp.left + x*NLM_CHCOLOR_BOXW;
      yy = rp.top + y*NLM_CHCOLOR_BOXH;
      Nlm_LoadRect ( &rdraw, xx + 3, yy + 3, 
                     xx + NLM_CHCOLOR_BOXW - 3, yy + NLM_CHCOLOR_BOXH - 3 );
      Nlm_SelectColor ( theCustomColors[y*8+x].red,
                        theCustomColors[y*8+x].green,
                        theCustomColors[y*8+x].blue );
      Nlm_PaintRect ( &rdraw );
    }
  }
}

static void Nlm_HColorDraw ( Nlm_PaneL p ){
  Nlm_RecT rp;
  int      i;
  int      x0, y0, xcur;

  Nlm_ObjectRect (pHColor, &rp);
  rp.left += NLM_CHCOLOR_PTRW2;
  rp.right -= NLM_CHCOLOR_PTRW2;
  Nlm_FrameRect ( &rp );
  x0 = xcur = rp.left + 1;
  y0 = rp.top + 1;
  if ( stColorModel == 1 ) {
    for ( i=0; i<6; i++ ){
      Nlm_LoadRect ( &rp, xcur, y0,
          x0 + (NLM_CHCOLOR_SHLW*(i+1))/18, y0+NLM_CHCOLOR_SHLH-2);
      Nlm_SelectColor ( (Nlm_Uint1)(255-(i*255)/6),
                        (Nlm_Uint1)((i*255)/6), 0 );
      xcur = x0 + (NLM_CHCOLOR_SHLW*(i+1))/18;
      Nlm_PaintRect ( &rp );
    }
    for ( i=0; i<6; i++ ){
      Nlm_LoadRect ( &rp, xcur, y0,
          x0 + (NLM_CHCOLOR_SHLW*(i+7))/18, y0+NLM_CHCOLOR_SHLH-2);
      Nlm_SelectColor ( 0, (Nlm_Uint1)(255-(i*255)/6),
                        (Nlm_Uint1)((i*255)/6) );
      xcur = x0 + (NLM_CHCOLOR_SHLW*(i+7))/18;
      Nlm_PaintRect ( &rp );
    }
    for ( i=0; i<6; i++ ){
      if ( i == 5 ) {
        Nlm_LoadRect ( &rp, xcur, y0,
            x0 + NLM_CHCOLOR_SHLW - 2, y0+NLM_CHCOLOR_SHLH-2);
      } else {
        Nlm_LoadRect ( &rp, xcur, y0,
            x0 + (NLM_CHCOLOR_SHLW*(i+13))/18, y0+NLM_CHCOLOR_SHLH-2);
      }
      Nlm_SelectColor ( (Nlm_Uint1)((i*255)/6), 0,
                        (Nlm_Uint1)(255-(i*255)/6) );
      xcur = x0 + (NLM_CHCOLOR_SHLW*(i+13))/18;
      Nlm_PaintRect ( &rp );
    }
  } else {
    for ( i=0; i<6; i++ ){
      if ( i == 5 ){
        Nlm_LoadRect ( &rp, xcur, y0,
            x0 + NLM_CHCOLOR_SHLW - 2, y0+NLM_CHCOLOR_SHLH-2);
      } else {
        Nlm_LoadRect ( &rp, xcur, y0,
            x0 + (NLM_CHCOLOR_SHLW*(i+1))/6, y0+NLM_CHCOLOR_SHLH-2);
      }
      Nlm_SelectColor ( (Nlm_Uint1)(((i+1)*255)/7), 0, 0 );
      xcur = x0 + (NLM_CHCOLOR_SHLW*(i+1))/6;
      Nlm_PaintRect ( &rp );
    }
  }
}

static void Nlm_HPtrColorDraw ( Nlm_PaneL p ){
  Nlm_RecT rp;
  Nlm_Int2 val;

  Nlm_ObjectRect (pHPtrColor, &rp);
  if ( stColorModel == 1 ) {
    val = stColorHSL[0];
  } else {
    val = stColorRGB[0];
  }
  Nlm_DrawColorPtr ( rp.left + NLM_CHCOLOR_PTRW2 +
    (Nlm_Int2)((long)val*(long)NLM_CHCOLOR_SHLW/(long)256),
    rp.top );
}

static void Nlm_SColorDraw ( Nlm_PaneL p ){
  Nlm_RecT rp;
  int      i;
  int      x0, y0, xcur;

  Nlm_ObjectRect (pSColor, &rp);
  rp.left += NLM_CHCOLOR_PTRW2;
  rp.right -= NLM_CHCOLOR_PTRW2;
  Nlm_FrameRect ( &rp );
  x0 = xcur = rp.left + 1;
  y0 = rp.top + 1;
  for ( i=0; i<6; i++ ){
    if ( i == 5 ){
      Nlm_LoadRect ( &rp, xcur, y0,
          x0 + NLM_CHCOLOR_SHLW - 2, y0+NLM_CHCOLOR_SHLH-2);
    } else {
      Nlm_LoadRect ( &rp, xcur, y0,
          x0 + (NLM_CHCOLOR_SHLW*(i+1))/6, y0+NLM_CHCOLOR_SHLH-2);
    }
    if ( stColorModel == 1 ) {
      Nlm_SelectColor ( (Nlm_Uint1)(((7-i)*255)/10),
                        (Nlm_Uint1)((255*(14+i)/21)),
                        (Nlm_Uint1)(((7-i)*255)/10) );
    } else {
      Nlm_SelectColor ( 0, (Nlm_Uint1)(((i+1)*255)/7), 0 );
    }
    xcur = x0 + (NLM_CHCOLOR_SHLW*(i+1))/6;
    Nlm_PaintRect ( &rp );
  }
}

static void Nlm_SPtrColorDraw ( Nlm_PaneL p ){
  Nlm_RecT rp;
  Nlm_Int2 val;

  Nlm_ObjectRect (pSPtrColor, &rp);
  if ( stColorModel == 1 ) {
    val = stColorHSL[1];
  } else {
    val = stColorRGB[1];
  }
  Nlm_DrawColorPtr ( rp.left + NLM_CHCOLOR_PTRW2 +
    (Nlm_Int2)((long)val*(long)NLM_CHCOLOR_SHLW/(long)256),
    rp.top );
}

static void Nlm_LColorDraw ( Nlm_PaneL p ){
  Nlm_RecT rp;
  int      i;
  int      x0, y0, xcur;

  Nlm_ObjectRect (pLColor, &rp);
  rp.left += NLM_CHCOLOR_PTRW2;
  rp.right -= NLM_CHCOLOR_PTRW2;
  Nlm_FrameRect ( &rp );
  x0 = xcur = rp.left + 1;
  y0 = rp.top + 1;
  if ( stColorModel == 1 ) {
    for ( i=0; i<8; i++ ){
      if ( i == 7 ){
        Nlm_LoadRect ( &rp, xcur, y0,
            x0 + NLM_CHCOLOR_SHLW - 2, y0+NLM_CHCOLOR_SHLH-2);
      } else {
        Nlm_LoadRect ( &rp, xcur, y0,
            x0 + (NLM_CHCOLOR_SHLW*(i+1))/8, y0+NLM_CHCOLOR_SHLH-2);
      }
      Nlm_SelectColor ( (Nlm_Uint1)((i*255)/7),
                        (Nlm_Uint1)((i*255)/7),
                        (Nlm_Uint1)((i*255)/7) );
      xcur = x0 + (NLM_CHCOLOR_SHLW*(i+1))/8;
      Nlm_PaintRect ( &rp );
    }
  } else {
    for ( i=0; i<6; i++ ){
      if ( i == 5 ){
        Nlm_LoadRect ( &rp, xcur, y0,
            x0 + NLM_CHCOLOR_SHLW - 2, y0+NLM_CHCOLOR_SHLH-2);
      } else {
        Nlm_LoadRect ( &rp, xcur, y0,
            x0 + (NLM_CHCOLOR_SHLW*(i+1))/6, y0+NLM_CHCOLOR_SHLH-2);
      }
      Nlm_SelectColor ( 0, 0, (Nlm_Uint1)(((i+1)*255)/7) );
      xcur = x0 + (NLM_CHCOLOR_SHLW*(i+1))/6;
      Nlm_PaintRect ( &rp );
    }
  }
}

static void Nlm_LPtrColorDraw ( Nlm_PaneL p ){
  Nlm_RecT rp;
  Nlm_Int2 val;

  Nlm_ObjectRect (pLPtrColor, &rp);
  if ( stColorModel == 1 ) {
    val = stColorHSL[2];
  } else {
    val = stColorRGB[2];
  }
  Nlm_DrawColorPtr ( rp.left + NLM_CHCOLOR_PTRW2 +
    (Nlm_Int2)((long)val*(long)NLM_CHCOLOR_SHLW/(long)256),
    rp.top );
}

static void Nlm_SelectDynColor (Nlm_Uint1 red, 
                                Nlm_Uint1 green,
                                Nlm_Uint1 blue) {
#ifdef WIN_X
  if (Nlm_hasColor  &&  (tmpColorPixel != VOID_PIXEL))
    {
      Nlm_Uint2 rd = (Nlm_Uint2)red;
      Nlm_Uint2 gn = (Nlm_Uint2)green;
      Nlm_Uint2 bl = (Nlm_Uint2)blue;
      XColor xcolor;
      xcolor.red   = rd << 8 | rd;
      xcolor.green = gn << 8 | gn;
      xcolor.blue  = bl << 8 | bl;
      xcolor.pixel = tmpColorPixel;
      xcolor.flags = DoRed|DoGreen|DoBlue;
      XStoreColor(Nlm_currentXDisplay,  Nlm_VibrantDefaultColormap(), &xcolor);
      XSetForeground (Nlm_currentXDisplay, Nlm_currentXGC, tmpColorPixel);
    }
  else
    Nlm_SelectColor(red, green, blue);
#else
  Nlm_SelectColor(red, green, blue);
#endif
}

static void Nlm_RetColorDraw ( Nlm_PaneL p ){
  Nlm_RecT  rp;

  Nlm_ObjectRect (pRetColor, &rp);
  Nlm_FrameRect ( &rp );
  Nlm_InsetRect ( &rp, 1, 1 );
  Nlm_SelectDynColor ( (Nlm_Uint1)stColorRGB[0],
                       (Nlm_Uint1)stColorRGB[1],
                       (Nlm_Uint1)stColorRGB[2] );
  Nlm_PaintRect ( &rp );
}

/* -------------------Button callbacks--------------------- */

static void Nlm_OKChooseColor ( Nlm_ButtoN bn ){
  colorDlgUp = FALSE;
  colorDlgOK = TRUE;
}

static void Nlm_CancelChooseColor ( Nlm_ButtoN bn ){
  colorDlgUp = FALSE;
}

static void Nlm_SaveCustomColor ( Nlm_ButtoN bn ){
  Nlm_WindoW tmpPort;

  theCustomColors[stColorCurCustom].red = (Nlm_Uint1)stColorRGB[0];
  theCustomColors[stColorCurCustom].green = (Nlm_Uint1)stColorRGB[1];
  theCustomColors[stColorCurCustom].blue = (Nlm_Uint1)stColorRGB[2];
  tmpPort = Nlm_SavePort (pCustomColor);
  Nlm_Select (pCustomColor); 
  Nlm_CustomColorDraw ( NULL );
  Nlm_RestorePort (tmpPort);
}

/* -------------------Update dialog------------------------ */
static void Nlm_UpdateColorDialog ( Nlm_Int2 owner ){
  Nlm_WindoW tmpPort;

  Nlm_ResetClip ();
  switch ( owner ){
    case NLM_CHCOLOR_OSP:
    case NLM_CHCOLOR_OHP:
    case NLM_CHCOLOR_OLP:
      if ( stColorModel == 1 ) {
        Nlm_HSL2RGB ( stColorRGB, stColorHSL );
      } else {
        Nlm_RGB2HSL ( stColorHSL, stColorRGB );
      }
      break;
    case NLM_CHCOLOR_OST:
    case NLM_CHCOLOR_OHT:
    case NLM_CHCOLOR_OLT:
      Nlm_HSL2RGB ( stColorRGB, stColorHSL );
      break;
    default:
      Nlm_RGB2HSL ( stColorHSL, stColorRGB );
  }
  tmpPort = Nlm_SavePort (pRetColor);
  Nlm_Select (pRetColor); 
  Nlm_RetColorDraw ( pRetColor );
  Nlm_RestorePort (tmpPort);
  switch ( owner ){
    case NLM_CHCOLOR_OHP:
    case NLM_CHCOLOR_OHT:
    case NLM_CHCOLOR_OLP:
    case NLM_CHCOLOR_OLT:
      break;
    default:
      Nlm_UpdColorPanel ( pSPtrColor );
  }
  switch ( owner ){
    case NLM_CHCOLOR_OSP:
    case NLM_CHCOLOR_OST:
    case NLM_CHCOLOR_OLP:
    case NLM_CHCOLOR_OLT:
      break;
    default:
      Nlm_UpdColorPanel ( pHPtrColor );
  }
  switch ( owner ){
    case NLM_CHCOLOR_OSP:
    case NLM_CHCOLOR_OST:
    case NLM_CHCOLOR_OHP:
    case NLM_CHCOLOR_OHT:
      break;
    default:
      Nlm_UpdColorPanel ( pLPtrColor );
  }
  if ( owner != NLM_CHCOLOR_OHT ) Nlm_UpdColorText ( tColorHue, stColorHSL[0] );
  if ( owner != NLM_CHCOLOR_OST ) Nlm_UpdColorText ( tColorSat, stColorHSL[1] );
  if ( owner != NLM_CHCOLOR_OLT ) Nlm_UpdColorText ( tColorLum, stColorHSL[2] );
  if ( owner != NLM_CHCOLOR_ORT ) Nlm_UpdColorText ( tColorRed, stColorRGB[0] );
  if ( owner != NLM_CHCOLOR_OGT ) Nlm_UpdColorText ( tColorGreen, stColorRGB[1] );
  if ( owner != NLM_CHCOLOR_OBT ) Nlm_UpdColorText ( tColorBlue, stColorRGB[2] );
}

/* -------------------Text callbacks----------------------- */

static void Nlm_EditColorRGB ( Nlm_TexT t ){
  int      ival;
  Nlm_Int2 owner;

  ival = Nlm_GetColorVal ( t );
  if ( t == tColorRed ){
    stColorRGB[0] = ival; 
    owner = NLM_CHCOLOR_ORT;
  } else if ( t == tColorGreen ){
    stColorRGB[1] = ival; 
    owner = NLM_CHCOLOR_OGT;
  } else {
    stColorRGB[2] = ival; 
    owner = NLM_CHCOLOR_OBT;
  }
  Nlm_UpdateColorDialog (owner);
}

static void Nlm_EditColorHSL ( Nlm_TexT t ){
  int      ival;
  Nlm_Int2 owner;

  ival = Nlm_GetColorVal ( t );
  if ( t == tColorHue ){
    stColorHSL[0] = ival; 
    owner = NLM_CHCOLOR_OHT;
  } else if ( t == tColorSat ){
    stColorHSL[1] = ival; 
    owner = NLM_CHCOLOR_OST;
  } else {
    stColorHSL[2] = ival; 
    owner = NLM_CHCOLOR_OLT;
  }
  Nlm_UpdateColorDialog (owner);
}

/* -------------------Click callback----------------------- */

static void Nlm_BasicColorClick ( Nlm_PaneL p, Nlm_PoinT pt ){
  Nlm_RecT   rp;
  int        colorNum;

  Nlm_ObjectRect (pBasicColor, &rp);
  pt.x -= rp.left;
  pt.y -= rp.top;
  pt.x /= NLM_CHCOLOR_BOXW;
  pt.y /= NLM_CHCOLOR_BOXH;
  colorNum = pt.y*8+pt.x;
  if ( (colorNum >= 0) && (colorNum < 48) ){
     stColorRGB[0] = theBasicColors[colorNum].red;
     stColorRGB[1] = theBasicColors[colorNum].green;
     stColorRGB[2] = theBasicColors[colorNum].blue;
     Nlm_UpdateColorDialog(NLM_CHCOLOR_OBC);
  }
}

static void Nlm_CustomColorClick ( Nlm_PaneL p, Nlm_PoinT pt ){
  Nlm_RecT   rp;
  Nlm_RecT   rdraw;
  int        colorNum,i,x,y;

  Nlm_ObjectRect (pCustomColor, &rp);
  pt.x -= rp.left;
  pt.y -= rp.top;
  pt.x /= NLM_CHCOLOR_BOXW;
  pt.y /= NLM_CHCOLOR_BOXH;
  colorNum = pt.y*8+pt.x;
  if ( (colorNum >= 0) && (colorNum < 16) ){
     stColorRGB[0] = theCustomColors[colorNum].red;
     stColorRGB[1] = theCustomColors[colorNum].green;
     stColorRGB[2] = theCustomColors[colorNum].blue;
     Nlm_InvertMode();
     for ( i=0;i<2;i++){
       y = stColorCurCustom / 8;
       x = stColorCurCustom - y*8;
       x = rp.left + x*NLM_CHCOLOR_BOXW;
       y = rp.top + y*NLM_CHCOLOR_BOXH;
       Nlm_LoadRect ( &rdraw, x, y, 
                      x + NLM_CHCOLOR_BOXW, y + NLM_CHCOLOR_BOXH );
       Nlm_FrameRect ( &rdraw );
       stColorCurCustom = colorNum;
     }
     Nlm_CopyMode();
     Nlm_UpdateColorDialog (NLM_CHCOLOR_OCC);
  }
}

static void Nlm_HColorClick ( Nlm_PaneL p, Nlm_PoinT pt ){
  Nlm_RecT   rp;
  Nlm_Int2   val;

  Nlm_ObjectRect (pHColor, &rp);
  pt.x -= rp.left + NLM_CHCOLOR_PTRW2;
  if ( (pt.x >= 0) && (pt.x <= NLM_CHCOLOR_SHLW) ){
    val = (Nlm_Int2)(((long)pt.x * (long)256)/(long)NLM_CHCOLOR_SHLW);
    if ( val > 255 ) val = 255;                
    if ( stColorModel == 1 ) {
      stColorHSL[0] = val;
    } else {
      stColorRGB[0] = val;
    }
    Nlm_UpdateColorDialog (NLM_CHCOLOR_OHP);
  }
}

static void Nlm_SColorClick ( Nlm_PaneL p, Nlm_PoinT pt ){
  Nlm_RecT   rp;
  Nlm_Int2   val;

  Nlm_ObjectRect (pSColor, &rp);
  pt.x -= rp.left + NLM_CHCOLOR_PTRW2;
  if ( (pt.x >= 0) && (pt.x <= NLM_CHCOLOR_SHLW) ){
    val = (Nlm_Int2)(((long)pt.x * (long)256)/(long)NLM_CHCOLOR_SHLW);
    if ( val > 255 ) val = 255;                
    if ( stColorModel == 1 ) {
      stColorHSL[1] = val;
    } else {
      stColorRGB[1] = val;
    }
    Nlm_UpdateColorDialog (NLM_CHCOLOR_OSP);
  }
}

static void Nlm_LColorClick ( Nlm_PaneL p, Nlm_PoinT pt ){
  Nlm_RecT   rp;
  Nlm_Int2   val;

  Nlm_ObjectRect (pLColor, &rp);
  pt.x -= rp.left + NLM_CHCOLOR_PTRW2;
  if ( (pt.x >= 0) && (pt.x <= NLM_CHCOLOR_SHLW) ){
    val = (Nlm_Int2)(((long)pt.x * (long)256)/(long)NLM_CHCOLOR_SHLW);
    if ( val > 255 ) val = 255;                
    if ( stColorModel == 1 ) {
      stColorHSL[2] = val;
    } else {
      stColorRGB[2] = val;
    }
    Nlm_UpdateColorDialog (NLM_CHCOLOR_OLP);
  }
}

/* ---------------Radio Button callback-------------------- */

static void Nlm_ChangeColorModel ( Nlm_GrouP g ){
  Nlm_RecT   rp;
  Nlm_PaneL  p[6];
  Nlm_Int2   i;
  Nlm_WindoW tmpPort;

  if ( Nlm_GetValue(g) != stColorModel ){
    stColorModel = Nlm_GetValue(g);
    p[0] = pSColor;
    p[1] = pSPtrColor;
    p[2] = pHColor;
    p[3] = pHPtrColor;
    p[4] = pLColor;
    p[5] = pLPtrColor;
    for (i=0; i<6; i++ ) {
      Nlm_ObjectRect (p[i], &rp);
      tmpPort = Nlm_SavePort (p[i]);
      Nlm_Select (p[i]); 
      Nlm_InvalRect (&rp);
      Nlm_RestorePort (tmpPort);
    }
  }
}

/* ---------------Main dialog function--------------------- */

extern Nlm_Boolean Nlm_ChooseColorDialog ( 
       Nlm_Uint1 PNTR redptr,Nlm_Uint1 PNTR greenptr, Nlm_Uint1 PNTR blueptr,
       Nlm_Boolean Initial ){
  Nlm_WindoW w;
  Nlm_GrouP  gw, gleft, gright, g, gtmp;
#ifdef WIN_X
  unsigned long  plane_masks[1];
#endif

  w = Nlm_MovableModalWindow (-50, -20, -20, -20, "Color", NULL);
  gw = Nlm_HiddenGroup (w, 2,0, NULL);
  Nlm_SetGroupMargins (gw, 1, 1);
  Nlm_SetGroupSpacing (gw, 10, 1);
  gleft = Nlm_HiddenGroup (gw, 1, 0, NULL);
  Nlm_SetGroupSpacing (gleft, 1, 10 );
  Nlm_StaticPrompt (gleft, " Basic colors:", 0, 0, Nlm_systemFont, 'l');
  pBasicColor = Nlm_SimplePanel ( gleft, NLM_CHCOLOR_BOXW * 8, 
                                  NLM_CHCOLOR_BOXH * 6, Nlm_BasicColorDraw );
  Nlm_SetPanelClick ( pBasicColor, Nlm_BasicColorClick, NULL, NULL, NULL );
  Nlm_StaticPrompt (gleft, " Custom colors:", 0, 0, Nlm_systemFont, 'l');
  pCustomColor = Nlm_SimplePanel ( gleft, NLM_CHCOLOR_BOXW * 8,
                                   NLM_CHCOLOR_BOXH * 2, Nlm_CustomColorDraw );
  Nlm_SetPanelClick ( pCustomColor, Nlm_CustomColorClick, NULL, NULL, NULL );
  pRetColor = Nlm_SimplePanel ( gleft, NLM_CHCOLOR_EXW, 
                                NLM_CHCOLOR_EXH, Nlm_RetColorDraw );
  gright = Nlm_HiddenGroup (gw, 1, 0, NULL);
  Nlm_SetGroupSpacing (gright, 1, 1);
  g = Nlm_HiddenGroup (gright, 2, 0, Nlm_ChangeColorModel );
  Nlm_SetGroupMargins (g, 20, 0);
  Nlm_RadioButton ( g, "HSL" );
  Nlm_RadioButton ( g, "RGB" );
  Nlm_SetValue(g,1);
  stColorModel = 1;
  g = Nlm_HiddenGroup (gright, 1, 0, NULL);
  Nlm_SetGroupMargins (g, 1, 20);
  Nlm_SetGroupSpacing (g, 5, 10);
  gtmp = Nlm_HiddenGroup (g, 1, 0, NULL);
  Nlm_SetGroupSpacing (gtmp, 0, 0);
  pHColor = Nlm_SimplePanel ( gtmp, NLM_CHCOLOR_SHLW + NLM_CHCOLOR_PTRW,
                              NLM_CHCOLOR_SHLH, Nlm_HColorDraw );
  Nlm_SetPanelClick ( pHColor, Nlm_HColorClick, Nlm_HColorClick, NULL, NULL );
  pHPtrColor = Nlm_SimplePanel ( gtmp, NLM_CHCOLOR_SHLW + NLM_CHCOLOR_PTRW, 
                                 NLM_CHCOLOR_PTRH, Nlm_HPtrColorDraw );
  Nlm_SetPanelClick ( pHPtrColor, Nlm_HColorClick, Nlm_HColorClick, NULL, NULL );
  Nlm_AlignObjects (ALIGN_CENTER, (Nlm_HANDLE)pHColor,
                                  (Nlm_HANDLE)pHPtrColor, NULL);
  gtmp = Nlm_HiddenGroup (g, 1, 0, NULL);
  Nlm_SetGroupSpacing (gtmp, 0, 0);
  pSColor = Nlm_SimplePanel ( gtmp, NLM_CHCOLOR_SHLW + NLM_CHCOLOR_PTRW,
                              NLM_CHCOLOR_SHLH, Nlm_SColorDraw );
  Nlm_SetPanelClick ( pSColor, Nlm_SColorClick, Nlm_SColorClick, NULL, NULL );
  pSPtrColor = Nlm_SimplePanel ( gtmp, NLM_CHCOLOR_SHLW + NLM_CHCOLOR_PTRW, 
                                 NLM_CHCOLOR_PTRH, Nlm_SPtrColorDraw );
  Nlm_SetPanelClick ( pSPtrColor, Nlm_SColorClick, Nlm_SColorClick, NULL, NULL );
  Nlm_AlignObjects (ALIGN_CENTER, (Nlm_HANDLE)pSColor,
                                  (Nlm_HANDLE)pSPtrColor, NULL );
  gtmp = Nlm_HiddenGroup (g, 1, 0, NULL);
  Nlm_SetGroupSpacing (gtmp, 0, 0);
  pLColor = Nlm_SimplePanel ( gtmp, NLM_CHCOLOR_SHLW + NLM_CHCOLOR_PTRW,
                              NLM_CHCOLOR_SHLH, Nlm_LColorDraw );
  Nlm_SetPanelClick ( pLColor, Nlm_LColorClick, Nlm_LColorClick, NULL, NULL );
  pLPtrColor = Nlm_SimplePanel ( gtmp, NLM_CHCOLOR_SHLW + NLM_CHCOLOR_PTRW, 
                                 NLM_CHCOLOR_PTRH, Nlm_LPtrColorDraw );
  Nlm_SetPanelClick ( pLPtrColor, Nlm_LColorClick, Nlm_LColorClick, NULL, NULL );
  Nlm_AlignObjects (ALIGN_CENTER, (Nlm_HANDLE)pLColor,
                                  (Nlm_HANDLE)pLPtrColor, NULL);
  g = Nlm_HiddenGroup (gright, 2, 0, NULL);
  Nlm_SetGroupMargins (g, 10, 2);
  gtmp = Nlm_HiddenGroup (g, 2, 0, NULL);
  Nlm_StaticPrompt (gtmp, "Hue:", 0, 0, Nlm_systemFont, 'r');
  tColorHue = Nlm_DialogText ( gtmp, NULL, 3, Nlm_EditColorHSL );
  Nlm_StaticPrompt (gtmp, "Sat:", 0, 0, Nlm_systemFont, 'r');
  tColorSat = Nlm_DialogText ( gtmp, NULL, 3, Nlm_EditColorHSL );
  Nlm_StaticPrompt (gtmp, "Lum:", 0, 0, Nlm_systemFont, 'r');
  tColorLum = Nlm_DialogText ( gtmp, NULL, 3, Nlm_EditColorHSL );
  gtmp = Nlm_HiddenGroup (g, 2, 0, NULL);
  Nlm_StaticPrompt (gtmp, "Red:", 0, 0, Nlm_systemFont, 'r');
  tColorRed = Nlm_DialogText ( gtmp, NULL, 3, Nlm_EditColorRGB );
  Nlm_StaticPrompt (gtmp, "Green:", 0, 0, Nlm_systemFont, 'r');
  tColorGreen = Nlm_DialogText ( gtmp, NULL, 3, Nlm_EditColorRGB );
  Nlm_StaticPrompt (gtmp, "Blue:", 0, 0, Nlm_systemFont, 'r');
  tColorBlue = Nlm_DialogText ( gtmp, NULL, 3, Nlm_EditColorRGB );
  g = Nlm_HiddenGroup (w, 3,0, NULL);
  Nlm_SetGroupSpacing (g, 15, 1);
  Nlm_SetGroupMargins (g, 60,1);
  Nlm_DefaultButton (g, "   OK   ", Nlm_OKChooseColor );
  Nlm_PushButton (g, "Cancel", Nlm_CancelChooseColor );
  Nlm_PushButton (g, "Save Custom Color", Nlm_SaveCustomColor );
/*  Nlm_DoShow ((Nlm_GraphiC) w, TRUE, TRUE); */
  Nlm_RealizeWindow (w);
  Nlm_UpdateColorDialog (NLM_CHCOLOR_OBC);
  Nlm_Show ((Nlm_GraphiC) w); 
  Nlm_ArrowCursor ();
  colorDlgUp = TRUE;
  colorDlgOK = FALSE;
#ifdef WIN_X
  if (tmpColorPixel == VOID_PIXEL  &&
      Nlm_currentXDisplay != NULL  &&
      XAllocColorCells(Nlm_currentXDisplay,
                       Nlm_VibrantDefaultColormap(),
                       FALSE, plane_masks, 0, &tmpColorPixel, 1) == 0)
    {
      tmpColorPixel = VOID_PIXEL;
    }
#endif
  Nlm_WaitForCondition( colorDlgUp );
  Nlm_ProcessAnEvent ();
/*  Nlm_DoRemove ((Nlm_GraphiC) w, TRUE); */
  Nlm_Remove ((Nlm_GraphiC) w);
  if ( colorDlgOK ){
    *redptr = (Nlm_Uint1)stColorRGB[0];
    *greenptr = (Nlm_Uint1)stColorRGB[1];
    *blueptr = (Nlm_Uint1)stColorRGB[2];
    return TRUE;
  }
  return FALSE;
}

#endif
/* -------------------End Non MS-Window ------------------- */

