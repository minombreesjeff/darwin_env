/*   ncbiport.h
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
* File Name:  ncbiport.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   1/1/91
*
* $Revision: 6.0 $
*
* File Description: 
*       Vibrant drawing port specification definitions
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: ncbiport.h,v $
* Revision 6.0  1997/08/25 18:56:09  madden
* Revision changed to 6.0
*
* Revision 5.2  1997/07/10 21:49:27  vakatov
* [WIN_X]  Now able to manage windows having different depths(and
* different visuals and GC).
*
* Revision 5.1  1997/06/18 19:46:32  vakatov
* [WIN_GIF]  Do not use WIN_MAC/MSWIN/MOTIF-specific fields of the
* Nlm_FontRec struct.
*
* Revision 5.0  1996/05/28 13:45:08  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 2.6  1995/05/17  15:15:14  kans
 * added Log line
 *
*
* ==========================================================================
*/

#ifndef _NCBIPORT_
#define _NCBIPORT_

#ifndef _NCBI_
#include <ncbi.h>
#endif
#ifndef _NCBIDRAW_
#include <ncbidraw.h>
#endif
#ifndef _NCBIWIN_
#include <ncbiwin.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN_MAC
#define Nlm_PointTool Point
#define Nlm_RectTool Rect
#define Nlm_RgnTool RgnHandle
#define Nlm_FontTool Nlm_Handle /* esl: not used */
#endif

#ifdef WIN_MSWIN
#define Nlm_PointTool POINT
#define Nlm_RectTool RECT
#define Nlm_RgnTool HRGN
#define Nlm_FontTool HFONT
#endif

#ifdef WIN_X
#define Nlm_PointTool XPoint
#define Nlm_RectTool XRectangle
#define Nlm_RgnTool Region
#define Nlm_FontTool XFontStruct*
#endif

/* The font structure is created using a handle.  The FntPtr is
*  returned by a call to Nlm_HandLock (font).  Be sure to call
*  Nlm_HandUnlock (font) after obtaining the font handle.
*/

/* esl: includes refcnt and fontspec instead of name/size/bld... */
typedef  struct  Nlm_fontrec {
  Nlm_FonT      next;
  Nlm_Int4      refcnt;  
  Nlm_FontSpec  fontspec;
#ifndef WIN_GIF
#ifdef WIN_MAC
  Nlm_Int2      number;
  Nlm_Int2      size;
  Nlm_Int2      style;
#elif defined(WIN_MSWIN) || defined(WIN_X)
  Nlm_FontTool  handle;
#endif
#endif
  Nlm_FonT      print;
} Nlm_FontRec, Nlm_FontData, PNTR Nlm_FntPtr;

extern Nlm_Boolean  Nlm_nowPrinting;

#ifdef WIN_MAC
extern  RGBColor     Nlm_RGBforeColor;
extern  RGBColor     Nlm_RGBbackColor;
extern  Nlm_Boolean  Nlm_hasColorQD;
#endif

#ifdef WIN_MSWIN
extern  HWND  Nlm_currentHWnd;
extern  HDC   Nlm_currentHDC;
#endif

#ifdef WIN_X
extern  Display      *Nlm_currentXDisplay;
extern  int          Nlm_currentXScreen;
extern  Window       Nlm_currentXWindow;
extern  GC           Nlm_currentXGC;
extern  Nlm_Uint4    Nlm_XbackColor;
extern  Nlm_Uint4    Nlm_XforeColor;
extern  Nlm_Int2     Nlm_XOffset;
extern  Nlm_Int2     Nlm_YOffset;
extern  Nlm_RegioN   Nlm_clpRgn;
extern  Nlm_Boolean  Nlm_hasColor;
#endif

#ifdef WIN_MAC
void  Nlm_SetPort PROTO((GrafPtr grafptr));
#endif

#ifdef WIN_MSWIN
void  Nlm_SetPort PROTO((HWND hwnd, HDC hdc));
extern void Nlm_FontSpecToLOGFONT PROTO((Nlm_FontSpecPtr fsp, LOGFONT *lfp)); /* esl */
#endif

#ifdef __cplusplus
}
#endif

#endif
