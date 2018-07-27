/*   ncbidraw.c
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
* File Name:  ncbidraw.c
*
* Author:  Jonathan Kans, Denis Vakatov
*
* Version Creation Date:   1/1/91
*
* $Revision: 6.29 $
*
* File Description: 
*       Vibrant drawing functions.
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: ncbidraw.c,v $
* Revision 6.29  2001/08/29 21:14:10  juran
* Move Carbon forward-compatibility to MoreCarbonAccessors.h.
* Call InvalRgn instead of InvalWindowRegion.
*
* Revision 6.28  2001/04/05 19:45:50  juran
* Carbon fixes.
*
* Revision 6.27  2000/02/07 20:17:35  lewisg
* minor bug fixes, use gui font for win32
*
* Revision 6.26  2000/01/24 16:11:13  lewisg
* speed up seqid comparison in color manager, make fast windows version of SetColor()
*
* Revision 6.25  2000/01/13 23:37:13  beloslyu
* changes because of port to HP-UX 11.0
*
* Revision 6.24  1999/12/21 18:04:23  kans
* removed MPW/THINKC conditional code, starting upgrade to Carbon compatibility - Churchill
*
* Revision 6.23  1999/10/20 23:08:10  vakatov
* + Nlm_SetCurrentGIF() -- to set Nlm_currentGIF directly.
* + Nlm_DestroyGIF() -- to destroy Nlm_currentGIF.
* Nlm_CreateGIF() and Nlm_SaveGIF() -- do not destroy Nlm_currentGIF.
*
* Revision 6.22  1999/10/04 18:44:02  vakatov
* [WIN_GIF] Nlm_SelectFont() -- fixed the way the font gets chosen
*
* Revision 6.21  1999/03/17 15:10:47  vakatov
* + Nlm_XLoadStandardFont() to find a "last-resort" font
*
* Revision 6.20  1998/08/12 14:52:26  vakatov
* [MSWIN] Nlm_DrawLine():  draw at least one point(fix to R6.8)
*
* Revision 6.19  1998/07/27 16:49:23  vakatov
* [WIN_X] AddNewFont():  try "-*-" in addition to "--" in font name spec.
*
* Revision 6.18  1998/07/24 21:43:14  vakatov
* [WIN_MSWIN]  It looks like the metafile driver does not support
* extended userstyle pens, so use plain pens when writing to metafile
*
* Revision 6.17  1998/07/14 16:44:24  vakatov
* Added VibrantIsGUI() and <internal> Nlm_VibrantSetGUI()
*
* Revision 6.16  1998/07/01 22:07:16  vakatov
* s_AdjustRoundRect():  one-pixel fix
*
* Revision 6.15  1998/07/01 18:27:41  vakatov
* Use "const" qualifier somewhere
*
* Revision 6.14  1998/06/26 20:00:17  vakatov
* Added s_AdjustRoundRect() to adjust the rounding sizes in Nlm_*RoundRect()
*
* Revision 6.13  1998/06/25 18:36:29  vakatov
* [WIN_MSWIN]  Minor 1-pixel adjustments to synchronize graphic primitives
*
* Revision 6.12  1998/06/12 16:40:12  kans
* fixed warnings detected by unix compiler
*
* Revision 6.11  1998/06/01 17:20:11  vakatov
* Added code to draw/fill 90-degree arc/pie (quadrants)
*
* Revision 6.10  1998/04/27 15:58:46  vakatov
* Implemented the drawing of rounded rectangles and ovals for GIF and X11
*
* Revision 6.9  1998/02/06 16:10:18  vakatov
* [WIN_X] SetUpDrawingTools(): fixed mem.leak(default font info)
*
* Revision 6.8  1998/02/05 20:07:08  vakatov
* [X11,MSWIN]  Made DrawLine() always draw the last and the first point
* (to be in-sync with GIF and MAC)
*
* Revision 6.7  1997/12/12 21:08:20  kans
* a number of symbols changed in the latest CodeWarrior release, now using headers from Apple
*
* Revision 6.6  1997/11/26 21:29:59  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.5  1997/11/04 23:32:49  vakatov
* [WIN_X] #ULTRA_SPARC_X_SERVER_BUG -- workaround to set foreground color
* when the stippling is in effect -- see Nlm_XSetForeground() for details.
*
* Revision 6.4  1997/09/16 20:34:15  vakatov
* Added Nlm_FitStringWidth();  for now, it is implemented as native
* for [WIN_X] and generic for all other platforms.
* [WIN_X]  Fixed bug in Nlm_AddNewFont() which disabled the font
* matching algorithm
*
* Revision 6.3  1997/09/11 00:00:38  vakatov
* [WIN_X] Nlm_EraseRect():  use XGetGCValues() to retrieve curr. fill style
*
* Revision 6.2  1997/09/10 18:23:35  vakatov
* [WIN_X] Nlm_EraseRect():  temp. switch fill style to FillSolid
*
* Revision 6.1  1997/09/09 23:53:36  vakatov
* [WIN_GIF]  Nlm_Solid() -- reset the drawing pattern to solid
*
* Revision 5.22  1997/08/19 19:28:01  vakatov
* [WIN_GIF]  Implemented Nlm_SelectPattern()-related features
*
* Revision 5.21  1997/07/23 19:42:17  vakatov
* Added Nlm_PaintStringEx function(text + position)
*
* Revision 5.20  1997/07/18 15:18:39  vakatov
* [!WIN_GIF] +Dummy Nlm_CreateGIF() and Nlm_SaveGIF();  /return FALSE/
*
* Revision 5.19  1997/07/10 21:49:21  vakatov
* [WIN_X]  Now able to manage windows having different depths(and
* different visuals and GC).
*
* Revision 5.18  1997/06/18 19:46:27  vakatov
* [WIN_GIF]  Do not use WIN_MAC/MSWIN/MOTIF-specific fields of the
* Nlm_FontRec struct.
*
* Revision 5.17  1997/06/09 18:54:33  vakatov
* [WIN_X]  Nlm_XAllocColor() moved to "vibwndws.c";  Nlm_SetupColor()
* removed at all;  use Nlm_VibrantDefaultColormap() and Nlm_XAllocColor()
* instead of 0 and XAllocColor() in most cases in order to use
* standard colormap rather than the default one.
* [ALL]  Renamed("Local__" - prefixed) static coordination transformation funcs
* whose name conflicts with the global ones;  +other code cleaning...
*
* Revision 5.16  1997/06/04 16:12:05  vakatov
* [WIN_MSWIN] PaintStringWin() has been removed again(see also R5.14)
*
* Revision 5.15  1997/06/04 00:05:12  kans
* support for Japanese by Tomo Koike of DDBJ
*
* Revision 5.14  1997/05/08 14:05:27  vakatov
* [WIN_MSWIN] PaintStringWin() is obsolete; replaced by MoveTo + PaintString
*
 * Revision 5.13  1997/05/06  20:03:44  vakatov
 * [WIN_GIF] Nlm_CreateGIF(): Boolean --> Nlm_Boolean (by J.Kuzio)
 *
 * Revision 5.12  1997/05/06  18:39:52  vakatov
 * [WIN32] Nlm_SetPenDash() -- added #WIN_MSWIN cpp condition
 *
 * Revision 5.11  1997/04/22  17:32:28  vakatov
 * [WIN_X] Nlm_DrawLine() -- correction of R5.6 for p1.[xy] > p2.[xy]
 *
 * Revision 5.10  1997/04/04  22:46:42  vakatov
 * [WIN_MSWIN]  Added static HFONT2Font() function;  since now,
 * "Nlm_systemFont" and "Nlm_programFont" are to be printed using
 * the matching logical fonts obtained with HFONT2Font().
 *
 * Revision 5.9  1997/01/29  16:41:22  kans
 * using StringNCpy_0
 *
 * Revision 5.8  1996/12/11  22:29:19  vakatov
 * [WIN_MSWIN]  Nlm_ResetDrawingTools():  reset prevPenHDC to ensure
 * proper brush pattern setting in some exotic cases
 *
 * Revision 5.7  1996/12/04  21:17:19  vakatov
 * [WIN_X] Nlm_InvertRect():  use GXinvert both on b/w and color displays
 *
 * Revision 5.6  1996/12/04  16:12:08  vakatov
 * [WIN_X]  Made Nlm_DrawLine() always draw the last point;  Nlm_MoveTo() now
 * implemented via a call to Nlm_DrawLine().
 *
 * Revision 5.5  1996/11/21  22:26:22  vakatov
 * [WIN_X]  Made Nlm_LineTo() and Nlm_DrawLine() draw last point
 *
 * Revision 5.4  1996/11/21  18:50:59  vakatov
 * [WIN_MOTIF]  Rectangle and Polygon drawing sync. to other platforms
 *
 * Revision 5.3  1996/10/28  19:33:04  vakatov
 * [WIN_MOTIF]  Use Nlm_VibrantDefaultColormap() instead of the display
 * default colormap everywhere in the color handling code.
 *
 * Revision 5.2  1996/10/01  18:17:37  vakatov
 * [WIN_X]  Added auxiliary static routine Nlm_XAllocColor() in order to
 * provide a uniform way of the color allocating;  on the color
 * allocation error, it now returns "Black" pixel rather than "White".
 *
 * Revision 5.1  1996/09/30  19:54:08  vakatov
 * [WIN_X, WIN_NT]  Added Nlm_SetPenDash() to allow advanced line dashing
 * [WIN_MSWIN]  Nlm_PaintRect() -- synchronize it with Nlm_FrameRect(),
 * Nlm_FrameOval(), etc. (left + 1, bottom + 1)
 *
 * Revision 5.0  1996/05/28  13:45:08  ostell
 * Set to revision 5.0
 *
 * Revision 4.13  1996/05/10  21:12:33  vakatov
 * Added UpdateColorTable() function to allow the user to read his color
 * preferencies from an external config.-file in format:
 * <index> <red> <green> <blue> [<name>];
 * <name> is optional and it is only used in the WIN_MOTIF-based
 * applications to get hardware-independent named colors, if possible.
 *
 * Revision 4.12  1996/04/15  19:31:56  tatiana
 * add Boolean transparent to Nlm_CreateGIF()
 *
 * Revision 4.11  1996/02/01  22:42:01  smirnov
 * AS: problem with symbols in clipboard is fixed
 *
 * Revision 4.10  1996/02/01  18:42:53  smirnov
 * AS: fix bug: draw a small rectangle
 *
 * Revision 4.9  1996/01/31  18:47:32  smirnov
 * fix copy&paste bug
 *
 * Revision 4.8  1995/12/22  19:42:33  smirnov
 * AS: Fixed bug in GIF version of DrawText()
 *
 * Revision 4.7  1995/11/27  15:47:55  kans
 * fixed FrameArc (VL + JK)
 *
 * Revision 4.6  1995/11/27  15:13:41  kans
 * corrections to font height, etc. functions (VL)
 *
 * Revision 4.4  1995/11/14  13:42:05  kans
 * fixes to AddNewFont (VL)
 *
 * Revision 4.3  1995/11/07  15:47:22  smirnov
 * add GIF generator (Alex Smirnov)
 *
 * Revision 4.2  1995/09/12  17:59:35  ostell
 * fixes for MS windows text to metafile
 *
 * Revision 4.1  1995/09/12  00:39:10  ostell
 * changes for text to appear in windows metafiles
 *
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 2.41  1995/07/14  17:48:26  kans
 * new CopyPixmap (AS)
 *
 * Revision 2.40  1995/06/07  21:23:38  kans
 * RecreateBrushes problem fixed (AS)
* ==========================================================================
*/

#include <vibtypes.h>
#include <vibincld.h>

#ifdef VAR_ARGS
#  include <varargs.h>
#else
#  include <stdarg.h>
#endif

#ifdef WIN_GIF
#  include <gifgen.h>
#  ifdef Nlm_RgnTool
#    undef Nlm_RgnTool 
#  endif
#  ifdef Nlm_RectTool
#    undef Nlm_RectTool 
#  endif
#  define Nlm_RectTool Nlm_RecT
#  ifdef WIN_MOTIF
#    undef WIN_MOTIF
#    ifdef WIN_X
#      undef WIN_X
#    endif
#    define Nlm_RgnTool Nlm_VoidPtr
#  endif
#  ifdef WIN_MAC
#    undef WIN_MAC
#    define Nlm_RgnTool Handle
#  endif
#  ifdef WIN_MSWIN
#    undef WIN_MSWIN
#    define Nlm_RgnTool HANDLE
#  endif
#else  /* WIN_GIF */
typedef struct gdImageStruct { void* dummy; } gdImage;
#endif /* WIN_GIF */

#if defined(WIN_X) && !defined(__hpux)
#include <X11/Xmu/Drawing.h>
#endif

Nlm_Boolean  Nlm_nowPrinting = FALSE;

#ifdef WIN_MAC
# include "MoreCarbonAccessors.h"
#endif

#ifdef WIN_MAC
RGBColor     Nlm_RGBforeColor;
RGBColor     Nlm_RGBbackColor;
Nlm_Boolean  Nlm_hasColorQD = FALSE;
#endif

#ifdef WIN_MSWIN
#define ATT_PENSTYLE  1
#define ATT_PENWIDTH  2
#define ATT_PATTERN   4
HWND         Nlm_currentHWnd;
HDC          Nlm_currentHDC;
#endif

#ifdef WIN_X
Display      *Nlm_currentXDisplay;
int          Nlm_currentXScreen;
Window       Nlm_currentXWindow;
GC           Nlm_currentXGC;
Nlm_Uint4    Nlm_XbackColor;
Nlm_Uint4    Nlm_XforeColor;
Nlm_Int2     Nlm_XOffset;
Nlm_Int2     Nlm_YOffset;
Nlm_RegioN   Nlm_clpRgn;
Nlm_Boolean  Nlm_hasColor = FALSE;
#endif

#ifdef WIN_GIF
#define      GIF_SOLID   0
#define      GIF_DASHED  1
static gdImagePtr  Nlm_currentGIF  = NULL;
static int         Nlm_curGIFColor = 1;
static int         Nlm_curGIFLType = GIF_SOLID;
static gdFontPtr   Nlm_curGIFFont  = NULL;
static Nlm_PoinT   Nlm_curGIFPoint = {0,0};
#endif

Nlm_RegioN   Nlm_updateRgn;
Nlm_RecT     Nlm_updateRect;

Nlm_Int2     Nlm_stdAscent;
Nlm_Int2     Nlm_stdDescent;
Nlm_Int2     Nlm_stdLeading;
Nlm_Int2     Nlm_stdFontHeight;
Nlm_Int2     Nlm_stdLineHeight;
Nlm_Int2     Nlm_stdCharWidth;

Nlm_FonT     Nlm_systemFont = NULL;
Nlm_FonT     Nlm_programFont = NULL;

#define COPY_MODE   1
#define MERGE_MODE  2
#define INVERT_MODE 3
#define ERASE_MODE  4

static Nlm_Int2    currentMode = COPY_MODE;

static Nlm_FonT    Nlm_fontList = NULL;
static Nlm_FonT    Nlm_fontInUse = NULL;

#ifndef WIN_GIF
static Nlm_RegioN  Nlm_scrollRgn;
#endif

#ifdef WIN_MAC
static Nlm_Byte  whitePat [] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static Nlm_Byte  ltGrayPat [] = {0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22};
static Nlm_Byte  grayPat [] = {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55};
static Nlm_Byte  dkGrayPat [] = {0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD};
static Nlm_Byte  blackPat [] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

static Nlm_Byte  dotPat [] = {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55};
static Nlm_Byte  dashPat [] = {0xCC, 0xCC, 0x33, 0x33, 0xCC, 0xCC, 0x33, 0x33};
#endif

#ifdef WIN_MSWIN
static DWORD       dash_gap[2];
static Nlm_Int2    currentPenStyle = PS_SOLID;
static Nlm_Int2    currentPenWidth = 1;
static void        *currentPattern = NULL;
static COLORREF    prevColor = 12346;
static Nlm_Int2    prevPenStyle = -1;
static Nlm_Int2    prevPenWidth = -1;
static void        *prevPattern = NULL;
static HDC         prevPenHDC = NULL;

static COLORREF    blackColor;
static COLORREF    redColor;
static COLORREF    greenColor;
static COLORREF    blueColor;
static COLORREF    cyanColor;
static COLORREF    magentaColor;
static COLORREF    yellowColor;
static COLORREF    whiteColor;

static Nlm_Uint2   blackPat [] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static Nlm_Uint2   dkGrayPat [] = {0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22};
static Nlm_Uint2   grayPat [] = {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55};
static Nlm_Uint2   ltGrayPat [] = {0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD};
static Nlm_Uint2   whitePat [] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

static Nlm_Uint2   dotPat [] = {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55};
static Nlm_Uint2   dashPat [] = {0xCC, 0xCC, 0x33, 0x33, 0xCC, 0xCC, 0x33, 0x33};

static HPEN        hBlackPen;
static HPEN        hNullPen;
static HPEN        hWhitePen;
static HBRUSH      hBlackBrush;
static HBRUSH      hDkGrayBrush;
static HBRUSH      hGrayBrush;
static HBRUSH      hHollowBrush;
static HBRUSH      hLtGrayBrush;
static HBRUSH      hNullBrush;
static HBRUSH      hWhiteBrush;
static HFONT       hAnsiFixedFont;
static HFONT       hAnsiVarFont;
static HFONT       hDeviceDefaultFont;
static HFONT       hOemFixedFont;
static HFONT       hSystemFont;
static HFONT       hSystemFixedFont;
static HFONT       hDefaultGuiFont;

static TEXTMETRIC  textMetrics;

static COLORREF    winTextColor;
static COLORREF    winBkColor;

#endif

#ifdef WIN_X
static XFontStruct  *currentFont;
static Pixmap       currentPixmap = 0;
static Nlm_PoinT    currentPoint;
static Nlm_Uint4    currentBkColor;
static Nlm_Uint4    currentFgColor;
static int          currentFunction = GXcopy;
static int          currentFillStyle = FillOpaqueStippled;

static Nlm_Uint4    blackColor;
static Nlm_Uint4    redColor;
static Nlm_Uint4    greenColor;
static Nlm_Uint4    blueColor;
static Nlm_Uint4    cyanColor;
static Nlm_Uint4    magentaColor;
static Nlm_Uint4    yellowColor;
static Nlm_Uint4    whiteColor;

static XFontStruct  fontInfo;
static Nlm_Uint1    flip [256];

static Nlm_RgnTool  emptyRgn;

#define ULTRA_SPARC_X_SERVER_BUG
#ifdef ULTRA_SPARC_X_SERVER_BUG
/* Attention! This looks as an X-server(not X-client) bug;  it means
 * that all X applications intended to run on an Ultra-SPARC's X-server
 * must be compiled with this option, no matter where the X-client
 * was built and run on(e.g. SGI, Linux, etc.)
 */
void Nlm_XSetForeground(Display *display, GC gc, unsigned long color) {
  if (currentFillStyle == FillOpaqueStippled) {
    XSetFillStyle(Nlm_currentXDisplay, Nlm_currentXGC, FillSolid);
    XSetFillStyle(Nlm_currentXDisplay, Nlm_currentXGC, FillOpaqueStippled);
  }
  XSetForeground(display, gc, color);
}
#define XSetForeground Nlm_XSetForeground
#endif

#endif /* WIN_X */

#if defined(WIN_X) || defined(WIN_GIF)
static Nlm_Uint1 whitePat []= {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static Nlm_Uint1 ltGrayPat[]= {0x88, 0x22, 0x88, 0x22, 0x88, 0x22, 0x88, 0x22};
static Nlm_Uint1 grayPat  []= {0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55};
static Nlm_Uint1 dkGrayPat[]= {0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD, 0x77, 0xDD};
static Nlm_Uint1 blackPat []= {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
#endif /* WIN_X | WIN_GIF */


#ifdef WIN_MAC
extern  void  Nlm_SetPort (GrafPtr grafptr)
{
  SetPort (grafptr);
}
#endif

#ifdef WIN_MSWIN
extern  void  Nlm_SetPort (HWND hwnd, HDC hdc)
{
  Nlm_currentHWnd = hwnd;
  Nlm_currentHDC = hdc;
}
#endif

#ifdef WIN_X

/* for internal Vibrant use only */
extern  XFontStruct * Nlm_XLoadQueryFont (Display *d, Nlm_CharPtr fDescr, Nlm_Boolean critical)
{
  XFontStruct *f;

  if ((f = XLoadQueryFont(d, fDescr)) == NULL && critical)
  {
    fprintf (stderr, "Vibrant: Unable to load critical font <%s>\n", fDescr);
    exit (1);
  }

  return f;
}
#endif


extern Nlm_Boolean Nlm_CreateGIF(Nlm_Int2 width, Nlm_Int2 height, Nlm_Boolean transparent)
{
#ifdef WIN_GIF
  gdImage* im = gdImageCreate((int)width, (int)height);
  if ( im ) {
    int white_color = gdImageColorAllocate(im, 255, 255, 255);
    if ( transparent )
      gdImageColorTransparent(im, white_color);
    gdImageColorAllocate(im, 0, 0, 0);

    Nlm_SetCurrentGIF(im);
    return TRUE;
  }
#endif /* WIN_GIF */
  return FALSE;
}


extern Nlm_Boolean Nlm_SaveGIF(FILE* out)
{
#ifdef WIN_GIF
  if (Nlm_currentGIF  &&  out) {
    gdImageGif(Nlm_currentGIF, out);
    return TRUE;
  }
#endif /* WIN_GIF */
  return FALSE;
}


extern void Nlm_DestroyGIF(void)
{
#ifdef WIN_GIF
  gdImage* im = Nlm_SetCurrentGIF(0);
  if ( im )
    gdImageDestroy(im);
#endif /* WIN_GIF */
}


extern struct gdImageStruct* Nlm_SetCurrentGIF(struct gdImageStruct* im)
{
#ifdef WIN_GIF
  gdImage* im_prev = Nlm_currentGIF;
  if (im == im_prev)
    return im;

  Nlm_curGIFColor   = 1;
  Nlm_curGIFLType   = GIF_SOLID;
  Nlm_curGIFFont    = gdFont7X13b;
  Nlm_curGIFPoint.x = 0;
  Nlm_curGIFPoint.y = 0;
  Nlm_currentGIF    = im;
  return im_prev;
#else
  return 0;
#endif /* WIN_GIF */
}


static void Nlm_SetFontData (Nlm_FonT f, Nlm_FontData * fdata)

{
  Nlm_FntPtr  fp;

  if (f != NULL && fdata != NULL) {
    fp = (Nlm_FntPtr) Nlm_HandLock (f);
    *fp = *fdata;
    Nlm_HandUnlock (f);
  }
}

static void Nlm_GetFontData (Nlm_FonT f, Nlm_FontData * fdata)

{
  Nlm_FntPtr  fp;

  if (f != NULL && fdata != NULL) {
    fp = (Nlm_FntPtr) Nlm_HandLock (f);
    *fdata = *fp;
    Nlm_HandUnlock (f);
  }
}


#ifdef WIN_MSWIN
static Nlm_Boolean Nlm_NotAStockPen (HPEN pen)
{
  return (Nlm_Boolean) (pen != hBlackPen && pen != hNullPen && pen != hWhitePen);
}

static Nlm_Boolean Nlm_NotAStockBrush (HBRUSH brush)
{
  return (Nlm_Boolean) (brush != hBlackBrush && brush != hDkGrayBrush &&
                        brush != hGrayBrush && brush != hHollowBrush &&
                        brush != hLtGrayBrush && hNullBrush &&
                        brush != hWhiteBrush);
}

static Nlm_Boolean Nlm_NotAStockFont (HFONT font)
{
  return (Nlm_Boolean) (font != hAnsiFixedFont && font != hAnsiVarFont &&
                        font != hDeviceDefaultFont && font != hOemFixedFont &&
                        font != hSystemFont && font != hSystemFixedFont);
}
#endif


extern void Nlm_SetPenDash(Nlm_Uint1 offset, Nlm_Uint1 dash, Nlm_Uint1 gap)
{
  if (dash == 0)
    dash = 1;
  if (gap == 0)
    gap = 1;

#ifdef WIN_X
  if (Nlm_currentXDisplay != NULL  &&  Nlm_currentXGC != NULL)
    {
      XGCValues gcv;
      char dash_gap[2];
      dash_gap[0] = dash;
      dash_gap[1] = gap;
      gcv.line_style = LineOnOffDash;
      XChangeGC(Nlm_currentXDisplay, Nlm_currentXGC,
                GCLineStyle, &gcv);
      XSetDashes(Nlm_currentXDisplay, Nlm_currentXGC,
                 (int)offset, dash_gap, sizeof(dash_gap));
    }

#elif defined(WIN32)  &&  defined(WIN_MSWIN)
  dash_gap[0] = dash;
  dash_gap[1] = gap;

  if ( Nlm_GetPicWinHDC() ) {
    Nlm_Dashed(); /* metafile-dumping driver does not support ext.pens */
    return;
  }

  {{
  HPEN     newPen, oldPen;
  LOGBRUSH newBrush;

  newBrush.lbStyle = BS_SOLID;
  newBrush.lbColor = winTextColor;
  newBrush.lbHatch = 0;
  if ((newPen = ExtCreatePen(PS_GEOMETRIC|PS_USERSTYLE|PS_ENDCAP_SQUARE,
                             1, &newBrush, 2, dash_gap)) == NULL  ||
      (oldPen = SelectObject(Nlm_currentHDC, newPen)) == NULL)
    {
      if ( newPen )
        DeleteObject( newPen );
      Nlm_Dashed();
      return;
    }

  prevColor = winTextColor;
  if ( Nlm_NotAStockPen( oldPen ) )
    DeleteObject( oldPen );
  }}

#else
  Nlm_Dashed();
#endif
}


#ifdef WIN_MAC
static void Nlm_ChooseColor (Nlm_Int4 color)
{
  ForeColor (color);
}
#endif


#ifdef WIN_MSWIN
static void Nlm_RecreateBrushes (void)
{
  COLORREF  color;
  HBITMAP   hBitmap;
  HBRUSH    newBrush;
  HPEN      newPen;
  HBRUSH    oldBrush;
  HPEN      oldPen;

  if (Nlm_currentHDC != NULL) {
    color = winTextColor;
    if ( (prevColor != color)||(prevPenStyle != currentPenStyle)||
         (prevPenWidth != currentPenWidth) ||
         (prevPenHDC != Nlm_currentHDC) ){

#ifdef WIN32
      if (dash_gap[0] != 0) {
        if (prevColor != winTextColor)
          Nlm_SetPenDash(0, (Nlm_Uint1)dash_gap[0], (Nlm_Uint1)dash_gap[1]);
        return;
      }

      if ( Nlm_GetPicWinHDC() ) /* metafile driver does not support ext.pens */
        newPen = CreatePen(currentPenStyle, currentPenWidth, color);
      else {
        LOGBRUSH brush;
        brush.lbStyle = BS_SOLID;
        brush.lbColor = color;
        brush.lbHatch = 0;
        newPen = ExtCreatePen(currentPenStyle|PS_GEOMETRIC|PS_ENDCAP_SQUARE,
                              currentPenWidth, &brush, 0, NULL);
      }
#else
      newPen = CreatePen(currentPenStyle, currentPenWidth, color);
#endif /* else!WIN32 */

      if (newPen != NULL) {
        oldPen = SelectObject (Nlm_currentHDC, newPen);
        if (oldPen == NULL) {
          OutputDebugString ("Cannot select pen\n");
          DeleteObject (newPen);
        } else {
          prevColor = color;
          prevPenStyle = currentPenStyle;
          prevPenWidth = currentPenWidth;
          if (Nlm_NotAStockPen (oldPen)) DeleteObject (oldPen);
        }
      }
    }
    if (currentPattern == NULL) {
      currentPattern = whitePat;
    }
    if ( (currentPattern != prevPattern) ||
         (prevPenHDC != Nlm_currentHDC) ){
      hBitmap = CreateBitmap (8, 8, 1, 1, (LPSTR) currentPattern);
      newBrush = CreatePatternBrush (hBitmap);
      if (newBrush != NULL) {
        oldBrush = SelectObject (Nlm_currentHDC, newBrush);
        if (oldBrush == NULL) {
          OutputDebugString ("Cannot select brush\n");
          DeleteObject (newBrush);
        } else {
          prevPenHDC = Nlm_currentHDC;
          prevPattern = currentPattern;
          if ( Nlm_NotAStockBrush(oldBrush) )
            DeleteObject (oldBrush);
        }
      }
      DeleteObject (hBitmap);
    }
  }
}

static void Nlm_SelectPattern (Nlm_Int2 style, Nlm_Int2 width, void * pat,
                               Nlm_Int1 flags )

{
  if ( flags & ATT_PENSTYLE ) currentPenStyle = style;
  if ( flags & ATT_PENWIDTH ) currentPenWidth = width;
  if ( flags & ATT_PATTERN ) currentPattern = pat;
#ifdef WIN32
  dash_gap[0] = 0;
#endif
  Nlm_RecreateBrushes ();
}

static void Nlm_ChooseColor (Nlm_Int4 color)

{
  if (Nlm_currentHDC != NULL) {
    SetTextColor (Nlm_currentHDC, color);
    winTextColor = color;
    Nlm_RecreateBrushes ();
  }
}

static Nlm_Boolean Nlm_GetTextMetrics (void)

{
  HDC          hDC;
  Nlm_Boolean  success;

  success = FALSE;
  if (Nlm_currentHDC != NULL) {
    success = (Nlm_Boolean) GetTextMetrics (Nlm_currentHDC, &textMetrics);
  } else {
    hDC = CreateIC ("DISPLAY", NULL, NULL, NULL);
    success = (Nlm_Boolean) GetTextMetrics (hDC, &textMetrics);
    DeleteDC (hDC);
  }
  return success;
}


static HBRUSH GetBackgroundBrush (HWND hwnd)

{
#ifndef WIN32
  return (HBRUSH) GetClassWord (hwnd, GCW_HBRBACKGROUND);
#else
  return (HBRUSH) GetClassLong (hwnd, GCL_HBRBACKGROUND);
#endif
}
#endif

#ifdef WIN_X
static void Nlm_SelectPattern (void * pat)

{
  if (Nlm_currentXDisplay != NULL && Nlm_currentXWindow != 0) {
    if (currentPixmap != 0) {
      XFreePixmap (Nlm_currentXDisplay, currentPixmap);
    }
    currentPixmap = XCreateBitmapFromData (Nlm_currentXDisplay, Nlm_currentXWindow,
                                           (char *) pat, 8, 8);
    if (currentPixmap != 0 && Nlm_currentXGC != NULL) {
      XSetStipple (Nlm_currentXDisplay, Nlm_currentXGC, currentPixmap);
    }
  }
}

static void Nlm_ChooseColor (Nlm_Uint4 color)

{
  if (Nlm_currentXDisplay != NULL && Nlm_currentXGC != NULL) {
    XSetForeground (Nlm_currentXDisplay, Nlm_currentXGC, color);
    currentFgColor = color;
  }
}

static Nlm_Boolean Nlm_GetTextMetrics (void)

{
  Nlm_Boolean  success;

  success = FALSE;
  if (Nlm_currentXDisplay != NULL && Nlm_currentXGC != NULL) {
    if (currentFont != NULL) {
      fontInfo = *currentFont;
      success = TRUE;
    }
  }
  return success;
}
#endif


extern void Nlm_ResetDrawingTools (void)
{
#ifdef WIN_MAC
  PenNormal ();
  PenMode (patCopy);
  TextMode (srcOr);
  if (Nlm_hasColorQD) {
    RGBForeColor (&Nlm_RGBforeColor);
    RGBBackColor (&Nlm_RGBbackColor);
  } else {
    ForeColor (blackColor);
    BackColor (whiteColor);
  }
#endif
#ifdef WIN_MSWIN
  if (Nlm_currentHDC != NULL) {
    SetROP2 (Nlm_currentHDC, R2_COPYPEN);
    SelectObject(Nlm_currentHDC, GetStockObject(SYSTEM_FONT));
    winTextColor = GetSysColor (COLOR_WINDOWTEXT);
    winBkColor = GetSysColor (COLOR_WINDOW);
    SetTextColor (Nlm_currentHDC, winTextColor);
    SetBkColor (Nlm_currentHDC, winBkColor);
    SetBkMode (Nlm_currentHDC, TRANSPARENT);
    Nlm_SelectPattern (PS_SOLID, 1, blackPat, 
                       ATT_PENSTYLE|ATT_PENWIDTH|ATT_PATTERN );
    prevPenHDC = NULL;
  }
#endif
#ifdef WIN_X
  if (Nlm_currentXDisplay != NULL && Nlm_currentXGC != NULL) {
    currentMode = COPY_MODE;
    currentFunction = GXcopy;
    currentFillStyle = FillOpaqueStippled;
    XSetFunction (Nlm_currentXDisplay, Nlm_currentXGC, currentFunction);
    XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, currentFillStyle);
    if (Nlm_hasColor) {
      XSetBackground (Nlm_currentXDisplay, Nlm_currentXGC, Nlm_XbackColor);
      XSetForeground (Nlm_currentXDisplay, Nlm_currentXGC, Nlm_XforeColor);
      currentBkColor = Nlm_XbackColor;
      currentFgColor = Nlm_XforeColor;
    } else {
      XSetBackground (Nlm_currentXDisplay, Nlm_currentXGC, whiteColor);
      XSetForeground (Nlm_currentXDisplay, Nlm_currentXGC, blackColor);
      currentBkColor = whiteColor;
      currentFgColor = blackColor;
    }
    XSetLineAttributes (Nlm_currentXDisplay, Nlm_currentXGC,
                        1, LineSolid, CapProjecting, JoinMiter);
    Nlm_SelectPattern (blackPat);
  }
#endif
#ifdef WIN_GIF
  Nlm_SelectColor (0,0,0);
  Nlm_curGIFLType = GIF_SOLID;
  Nlm_curGIFFont = gdFont7X13b;
  gdImageSelectPattern(Nlm_currentGIF, blackPat);
#endif
  currentMode = COPY_MODE;
}

/* The following functions:
 *   Local__PointToolToPoinT
 *   Local__PoinTToPointTool
 *   Local__RectToolToRecT
 *   Local__RecTToRectTool
 * are the local versions of relevant global Nlm_... functions
 * declared in "vibincld.h" and defined in "vibutils.c" 
 */

#ifdef WIN_MAC
static void Local__PointToolToPoinT(Nlm_PointTool src, Nlm_PointPtr dst)
{
  if ( !dst )
    return;

  dst->x = src.h;
  dst->y = src.v;
}
#endif /* WIN_MAC */

#ifndef WIN_GIF
static void Local__PoinTToPointTool(Nlm_PoinT src, Nlm_PointTool PNTR dst)
{
  if ( !dst )
    return;

#ifdef WIN_MAC
  dst->h = src.x;
  dst->v = src.y;
#endif
#if defined(WIN_MSWIN) || defined(WIN_X)
  dst->x = src.x;
  dst->y = src.y;
#endif
}


static void Local__RectToolToRecT(Nlm_RectTool PNTR src, Nlm_RectPtr dst)
{
  if (!dst  ||  !src)
    return;

#if defined(WIN_MAC) || defined(WIN_MSWIN)
  dst->left   = (Nlm_Int2)src->left;
  dst->top    = (Nlm_Int2)src->top;
  dst->right  = (Nlm_Int2)src->right;
  dst->bottom = (Nlm_Int2)src->bottom;
#endif
#ifdef WIN_X
  dst->left   = src->x;
  dst->top    = src->y;
  dst->right  = src->x + src->width;
  dst->bottom = src->y + src->height;
#endif
}
#endif /* !WIN_GIF */

static void Local__RecTToRectTool(Nlm_RectPtr src, Nlm_RectTool PNTR dst)
{
  if (!dst  ||  !src)
    return;

#if defined(WIN_MAC) || defined(WIN_MSWIN) || defined(WIN_GIF)
  dst->left   = MIN(src->left, src->right );
  dst->top    = MIN(src->top,  src->bottom);
  dst->right  = MAX(src->left, src->right );
  dst->bottom = MAX(src->top,  src->bottom);
#endif
#ifdef WIN_X
  dst->x = MIN(src->left, src->right);
  dst->y = MIN(src->top, src->bottom);
  dst->width  = ABS(src->right - src->left);
  dst->height = ABS(src->bottom - src->top);
#endif
}


extern void Nlm_CopyMode (void)
{
#ifdef WIN_MAC
  PenMode (patCopy);
  TextMode (srcOr);
#endif
#ifdef WIN_MSWIN
  if (Nlm_currentHDC != NULL) {
    SetROP2 (Nlm_currentHDC, R2_COPYPEN);
  }
#endif
#ifdef WIN_X
  currentFunction = GXcopy;
  currentFillStyle = FillOpaqueStippled;
  if (Nlm_currentXDisplay != NULL && Nlm_currentXGC != NULL) {
    XSetFunction (Nlm_currentXDisplay, Nlm_currentXGC, currentFunction);
    XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, currentFillStyle);
  }
#endif
  currentMode = COPY_MODE;
}

extern void Nlm_MergeMode (void)

{
#ifdef WIN_MAC
  PenMode (patOr);
  TextMode (srcOr);
#endif
#ifdef WIN_MSWIN
  if (Nlm_currentHDC != NULL) {
    SetROP2 (Nlm_currentHDC, R2_MASKPEN);
  }
#endif
#ifdef WIN_X
  if (Nlm_hasColor) {
    currentFunction = GXand;
  } else {
    currentFunction = GXor;
  }
  currentFillStyle = FillStippled;
  if (Nlm_currentXDisplay != NULL && Nlm_currentXGC != NULL) {
    XSetFunction (Nlm_currentXDisplay, Nlm_currentXGC, currentFunction);
    XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, currentFillStyle);
  }
#endif
  currentMode = MERGE_MODE;
}

extern void Nlm_InvertMode (void)

{
#ifdef WIN_MAC
  PenMode (patXor);
  TextMode (srcXor);
#endif
#ifdef WIN_MSWIN
  if (Nlm_currentHDC != NULL) {
    SetROP2 (Nlm_currentHDC, R2_NOTXORPEN);
  }
#endif
#ifdef WIN_X
  if (Nlm_hasColor) {
    currentFunction = GXequiv;
  } else {
    currentFunction = GXinvert;
  }
  currentFillStyle = FillStippled;
  if (Nlm_currentXDisplay != NULL && Nlm_currentXGC != NULL) {
    XSetFunction (Nlm_currentXDisplay, Nlm_currentXGC, currentFunction);
    XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, currentFillStyle);
  }
#endif
  currentMode = INVERT_MODE;
}

extern void Nlm_EraseMode (void)


{
#ifdef WIN_MAC
  PenMode (patBic);
  TextMode (srcBic);
#endif
#ifdef WIN_MSWIN
  if (Nlm_currentHDC != NULL) {
    SetROP2 (Nlm_currentHDC, R2_MERGENOTPEN);
  }
#endif
#ifdef WIN_X
  if (Nlm_hasColor) {
    currentFunction = GXorInverted;
  } else {
    currentFunction = GXand;
  }
  currentFillStyle = FillStippled;
  if (Nlm_currentXDisplay != NULL && Nlm_currentXGC != NULL) {
    XSetFunction (Nlm_currentXDisplay, Nlm_currentXGC, currentFunction);
    XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, currentFillStyle);
  }
#endif
  currentMode = ERASE_MODE;
}

extern void Nlm_Black (void)

{
#ifdef WIN_MAC
  if (Nlm_hasColorQD) {
    Nlm_SelectColor (0, 0, 0);
  } else {
    Nlm_ChooseColor (blackColor);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_ChooseColor (blackColor);
#endif
#ifdef WIN_X
  Nlm_ChooseColor (blackColor);
#endif
#ifdef WIN_GIF
  Nlm_SelectColor (0, 0, 0);
#endif
}

extern void Nlm_Red (void)

{
#ifdef WIN_MAC
  if (Nlm_hasColorQD) {
    Nlm_SelectColor (255, 0, 0);
  } else {
    Nlm_ChooseColor (redColor);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_ChooseColor (redColor);
#endif
#ifdef WIN_X
  Nlm_ChooseColor (redColor);
#endif
#ifdef WIN_GIF
  Nlm_SelectColor (255, 0, 0);
#endif
}

extern void Nlm_Green (void)

{
#ifdef WIN_MAC
  if (Nlm_hasColorQD) {
    Nlm_SelectColor (0, 255, 0);
  } else {
    Nlm_ChooseColor (greenColor);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_ChooseColor (greenColor);
#endif
#ifdef WIN_X
  Nlm_ChooseColor (greenColor);
#endif
#ifdef WIN_GIF
  Nlm_SelectColor (0, 255, 0);
#endif
}

extern void Nlm_Blue (void)

{
#ifdef WIN_MAC
  if (Nlm_hasColorQD) {
    Nlm_SelectColor (0, 0, 255);
  } else {
    Nlm_ChooseColor (blueColor);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_ChooseColor (blueColor);
#endif
#ifdef WIN_X
  Nlm_ChooseColor (blueColor);
#endif
#ifdef WIN_GIF
  Nlm_SelectColor (0, 0, 255);
#endif
}

extern void Nlm_Cyan (void)

{
#ifdef WIN_MAC
  if (Nlm_hasColorQD) {
    Nlm_SelectColor (0, 255, 255);
  } else {
    Nlm_ChooseColor (cyanColor);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_ChooseColor (cyanColor);
#endif
#ifdef WIN_X
  Nlm_ChooseColor (cyanColor);
#endif
#ifdef WIN_GIF
  Nlm_SelectColor (0, 255, 255);
#endif
}

extern void Nlm_Magenta (void)

{
#ifdef WIN_MAC
  if (Nlm_hasColorQD) {
    Nlm_SelectColor (255, 0, 255);
  } else {
    Nlm_ChooseColor (magentaColor);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_ChooseColor (magentaColor);
#endif
#ifdef WIN_X
  Nlm_ChooseColor (magentaColor);
#endif
#ifdef WIN_GIF
  Nlm_SelectColor (255, 0, 255);
#endif
}

extern void Nlm_Yellow (void)

{
#ifdef WIN_MAC
  if (Nlm_hasColorQD) {
    Nlm_SelectColor (255, 255, 0);
  } else {
    Nlm_ChooseColor (yellowColor);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_ChooseColor (yellowColor);
#endif
#ifdef WIN_X
  Nlm_ChooseColor (yellowColor);
#endif
#ifdef WIN_GIF
  Nlm_SelectColor (255, 255, 0);
#endif
}

extern void Nlm_White (void)

{
#ifdef WIN_MAC
  if (Nlm_hasColorQD) {
    Nlm_SelectColor (255, 255, 255);
  } else {
    Nlm_ChooseColor (whiteColor);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_ChooseColor (whiteColor);
#endif
#ifdef WIN_X
  Nlm_ChooseColor (whiteColor);
#endif
#ifdef WIN_GIF
  Nlm_SelectColor (255, 255, 255);
#endif
}

extern void Nlm_Gray (void)

{
  Nlm_SelectColor (127, 127, 127);
}

extern void Nlm_LtGray (void)

{
  Nlm_SelectColor (191, 191, 191);
}

extern void Nlm_DkGray (void)

{
  Nlm_SelectColor (63, 63, 63);
}


extern void Nlm_SelectColor (Nlm_Uint1 red, Nlm_Uint1 green, Nlm_Uint1 blue)
{
#ifdef WIN_MAC
  RGBColor   color;
  Nlm_Uint2  bl;
  Nlm_Uint2  gn;
  Nlm_Uint2  rd;

  if (Nlm_hasColorQD) {
    rd = (Nlm_Uint2) red;
    gn = (Nlm_Uint2) green;
    bl = (Nlm_Uint2) blue;
    color.red = rd << 8 | rd;
    color.green = gn << 8 | gn;
    color.blue = bl << 8 | bl;
    RGBForeColor (&color);
  } else if ((int) red + (int) green + (int) blue < 192) {
    Nlm_ChooseColor (blackColor);
  } else {
    Nlm_ChooseColor (whiteColor);
  }
#endif
#ifdef WIN_MSWIN
  COLORREF   color;
  Nlm_Uint2  bl;
  Nlm_Uint2  gn;
  Nlm_Uint2  rd;

  rd = (Nlm_Uint2) red;
  gn = (Nlm_Uint2) green;
  bl = (Nlm_Uint2) blue;
  color = RGB (rd, gn, bl);
  Nlm_ChooseColor (color);
#endif
#ifdef WIN_X
  XColor xcolor;
  Nlm_XAllocColor(&xcolor, Nlm_VibrantDefaultColormap(), red, green, blue);
  Nlm_ChooseColor( xcolor.pixel );
#endif
#ifdef WIN_GIF
  Nlm_curGIFColor = (int)Nlm_GetColorRGB ( red, green, blue );
#endif
}


extern Nlm_Uint4 Nlm_GetColorRGB (Nlm_Uint1 red, Nlm_Uint1 green,
                                  Nlm_Uint1 blue)
{
#ifdef WIN_MAC
  Nlm_Uint1  colors [4];

  colors [0] = 0;
  colors [1] = red;
  colors [2] = green;
  colors [3] = blue;
  return *((Nlm_Int4Ptr) colors);
#endif
#ifdef WIN_MSWIN
  Nlm_Uint2  bl;
  Nlm_Uint2  gn;
  Nlm_Uint2  rd;

  rd = (Nlm_Uint2) red;
  gn = (Nlm_Uint2) green;
  bl = (Nlm_Uint2) blue;
  return (Nlm_Uint4)(RGB(rd, gn, bl));
#endif
#ifdef WIN_X
  XColor xcolor;
  Nlm_XAllocColor(&xcolor, Nlm_VibrantDefaultColormap(), red, green, blue);
  return xcolor.pixel;
#endif
#ifdef WIN_GIF
  int i;

  if ( Nlm_currentGIF != NULL ){
    i = gdImageColorExact ( Nlm_currentGIF, (int)red, (int)green, (int)blue );
    if ( i == -1 ){
      i = gdImageColorAllocate ( Nlm_currentGIF, (int)red, (int)green, 
                                                 (int)blue );
      if ( i == -1 ){
        i = gdImageColorClosest ( Nlm_currentGIF, (int)red, (int)green, 
                                                  (int)blue );
      }
    }
  }
  return (Nlm_Uint4)i;
#endif
}


extern Nlm_Uint4 Nlm_GetColor (void)
{
#ifdef WIN_MAC
  Nlm_Uint1  colors [4];
  RGBColor   foreColor;
  Nlm_Int4   fgColor;

  fgColor = 0;
  if (Nlm_hasColorQD) {
    GetForeColor (&foreColor);
    colors [0] = 0;
    colors [1] = (Nlm_Uint1) (foreColor.red >> 8);
    colors [2] = (Nlm_Uint1) (foreColor.green >> 8);
    colors [3] = (Nlm_Uint1) (foreColor.blue >> 8);
    fgColor = *((Nlm_Int4Ptr) colors);
#if !OPAQUE_TOOLBOX_STRUCTS
  } else {
  	GrafPtr port;
    GetPort (&port);
    if (port != NULL) {
      fgColor = port->fgColor;
    }
#endif
  }
  return (Nlm_Uint4) fgColor;
#endif
#ifdef WIN_MSWIN
  Nlm_Int4  fgColor;

  fgColor = 0;
  if (Nlm_currentHDC != NULL) {
    fgColor = GetTextColor (Nlm_currentHDC);
  }
  return (Nlm_Uint4) fgColor;
#endif
#ifdef WIN_X
  return currentFgColor;
#endif
#ifdef WIN_GIF
  return (Nlm_Uint4)Nlm_curGIFColor;
#endif
}

/*
Used to set the color of text or foreground color.  This function is the same
as Nlm_SetColor except on Windows, where it does not call the
extremely expensive Nlm_RecreateBrushes()
*/

extern void Nlm_SetColorEx (Nlm_Uint4 color)
{
#ifdef WIN_MAC
    Nlm_SetColor (color);
#endif
#ifdef WIN_MSWIN
    if (Nlm_currentHDC != NULL) {
        SetTextColor (Nlm_currentHDC, (Nlm_Int4) color);
        winTextColor = color;
    }
#endif
#ifdef WIN_X
    Nlm_SetColor (color);
#endif
}


extern void Nlm_SetColor (Nlm_Uint4 color)

{
#ifdef WIN_MAC
  Nlm_Uint2  bl;
  Nlm_Uint2  gn;
  Nlm_Uint2  rd;
  Nlm_Uint1  colors [4];
  RGBColor   foreColor;
  GrafPtr    port;

  if (Nlm_hasColorQD) {
    *((Nlm_Int4Ptr) colors) = color;
    rd = (Nlm_Uint2) colors [1];
    gn = (Nlm_Uint2) colors [2];
    bl = (Nlm_Uint2) colors [3];
    foreColor.red = rd << 8 | rd;
    foreColor.green = gn << 8 | gn;
    foreColor.blue = bl << 8 | bl;
    RGBForeColor (&foreColor);
  } else {
    GetPort (&port);
    if (port != NULL) {
      ForeColor ((Nlm_Int4) color);
    }
  }
#endif
#ifdef WIN_MSWIN
  if (Nlm_currentHDC != NULL) {
    SetTextColor (Nlm_currentHDC, (Nlm_Int4) color);
    winTextColor = color;
    Nlm_RecreateBrushes ();
  }
#endif
#ifdef WIN_X
  XSetForeground (Nlm_currentXDisplay, Nlm_currentXGC, color);
  currentFgColor = color;
#endif
#ifdef WIN_GIF
  Nlm_curGIFColor = (int)color;
#endif
}

extern void Nlm_InvertColors (void)

{
#ifdef WIN_MAC
  RGBColor  backColor;
  RGBColor  foreColor;

  if (Nlm_hasColorQD) {
    GetForeColor (&foreColor);
    GetBackColor (&backColor);
    RGBForeColor (&backColor);
    RGBBackColor (&foreColor);
#if !OPAQUE_TOOLBOX_STRUCTS
  } else {
    Nlm_Int4  bkColor;
    Nlm_Int4  fgColor;
  	GrafPtr port;
    GetPort (&port);
    if (port != NULL) {
      fgColor = port->fgColor;
      bkColor = port->bkColor;
      ForeColor (bkColor);
      BackColor (fgColor);
    }
#endif
  }
#endif
#ifdef WIN_MSWIN
  Nlm_Int4  bkColor;
  Nlm_Int4  fgColor;

  if (Nlm_currentHDC != NULL) {
    fgColor = GetTextColor (Nlm_currentHDC);
    bkColor = GetBkColor (Nlm_currentHDC);
    SetTextColor (Nlm_currentHDC, bkColor);
    winTextColor = bkColor;
    SetBkColor (Nlm_currentHDC, fgColor);
    winBkColor  = fgColor;
    Nlm_RecreateBrushes ();
  }
#endif
#ifdef WIN_X
  Nlm_Uint4  newBkColor;
  Nlm_Uint4  newFgColor;

  newBkColor = currentFgColor;
  newFgColor = currentBkColor;
  XSetBackground (Nlm_currentXDisplay, Nlm_currentXGC, newBkColor);
  XSetForeground (Nlm_currentXDisplay, Nlm_currentXGC, newFgColor);
  currentBkColor = newBkColor;
  currentFgColor = newFgColor;
#endif
#ifdef WIN_GIF
#endif
}

extern void Nlm_DecodeColor (Nlm_Uint4 color, Nlm_Uint1Ptr red,
                             Nlm_Uint1Ptr green, Nlm_Uint1Ptr blue)

{
#ifdef WIN_MAC
  Nlm_Uint1Ptr  colors;
  Nlm_Uint1     bl;
  Nlm_Uint1     gn;
  Nlm_Uint1     rd;

  rd = 0;
  gn = 0;
  bl = 0;
  if (Nlm_hasColorQD) {
    colors = (Nlm_Uint1Ptr) (&color);
    rd = (Nlm_Uint1) colors [1];
    gn = (Nlm_Uint1) colors [2];
    bl = (Nlm_Uint1) colors [3];
  } else if (color == whiteColor) {
    rd = 255;
    gn = 255;
    bl = 255;
  }
  if (red != NULL) {
    *red = (Nlm_Uint1) rd;
  }
  if (green != NULL) {
    *green = (Nlm_Uint1) gn;
  }
  if (blue != NULL) {
    *blue = (Nlm_Uint1) bl;
  }
#endif
#ifdef WIN_MSWIN
  Nlm_Uint1  bl;
  Nlm_Uint1  gn;
  Nlm_Uint1  rd;

  rd = GetRValue (color);
  gn = GetGValue (color);
  bl = GetBValue (color);
  if (red != NULL) {
    *red = (Nlm_Uint1) rd;
  }
  if (green != NULL) {
    *green = (Nlm_Uint1) gn;
  }
  if (blue != NULL) {
    *blue = (Nlm_Uint1) bl;
  }
#endif
#ifdef WIN_X
  Nlm_Uint2  bl;
  Nlm_Uint2  gn;
  Nlm_Uint2  rd;
  XColor     xcolor;

  rd = 0;
  gn = 0;
  bl = 0;
  if (Nlm_hasColor) {
    xcolor.pixel = color;
    if (Nlm_currentXDisplay != NULL) {
      XQueryColor(Nlm_currentXDisplay, Nlm_VibrantDefaultColormap(), &xcolor);
      rd = xcolor.red >> 8;
      gn = xcolor.green >> 8;
      bl = xcolor.blue >> 8;
    }
  } else if (color == whiteColor) {
    rd = 255;
    gn = 255;
    bl = 255;
  }
  if (red != NULL) {
    *red = (Nlm_Uint1) rd;
  }
  if (green != NULL) {
    *green = (Nlm_Uint1) gn;
  }
  if (blue != NULL) {
    *blue = (Nlm_Uint1) bl;
  }
#endif
#ifdef WIN_GIF
#endif
}

extern void Nlm_Dark (void)
{
#ifdef WIN_MAC
  PenPat ((ConstPatternParam) dkGrayPat);
#endif
#ifdef WIN_MSWIN
  Nlm_SelectPattern (PS_SOLID, 1, dkGrayPat, ATT_PATTERN );
#endif
#ifdef WIN_X
  Nlm_SelectPattern (dkGrayPat);
#endif
#ifdef WIN_GIF
  gdImageSelectPattern(Nlm_currentGIF, dkGrayPat);
#endif
}

extern void Nlm_Medium (void)
{
#ifdef WIN_MAC
  PenPat ((ConstPatternParam) grayPat);
#endif
#ifdef WIN_MSWIN
  Nlm_SelectPattern (PS_SOLID, 1, grayPat, ATT_PATTERN );
#endif
#ifdef WIN_X
  Nlm_SelectPattern (grayPat);
#endif
#ifdef WIN_GIF
  gdImageSelectPattern(Nlm_currentGIF, grayPat);
#endif
}

extern void Nlm_Light (void)
{
#ifdef WIN_MAC
  PenPat ((ConstPatternParam) ltGrayPat);
#endif
#ifdef WIN_MSWIN
  Nlm_SelectPattern (PS_SOLID, 1, ltGrayPat, ATT_PATTERN);
#endif
#ifdef WIN_X
  Nlm_SelectPattern (ltGrayPat);
#endif
#ifdef WIN_GIF
  gdImageSelectPattern(Nlm_currentGIF, ltGrayPat);
#endif
}

extern void Nlm_Empty (void)
{
#ifdef WIN_MAC
  PenPat ((ConstPatternParam) whitePat);
#endif
#ifdef WIN_MSWIN
  Nlm_SelectPattern (PS_SOLID, 1, whitePat, ATT_PATTERN);
#endif
#ifdef WIN_X
  Nlm_SelectPattern (whitePat);
#endif
#ifdef WIN_GIF
  gdImageSelectPattern(Nlm_currentGIF, whitePat);
#endif
}

extern void Nlm_SetPenPattern (Nlm_VoidPtr pattern)
{
#ifdef WIN_MAC
  Nlm_Int2      i;
  Nlm_Byte      pat [8];
  Nlm_BytePtr   ptr;
#endif
#ifdef WIN_MSWIN
  Nlm_Int2      i;
  Nlm_Uint2     pat [8];
  Nlm_Uint1Ptr  ptr;
  Nlm_Uint1Ptr  q;
#endif
#ifdef WIN_X
  Nlm_Int2      i;
  Nlm_Uint1     pat [8];
  Nlm_Uint1Ptr  ptr;
  Nlm_Uint1Ptr  q;
#endif

  if (pattern != NULL) {
#ifdef WIN_MAC
    ptr = (Nlm_BytePtr) pattern;
    for (i = 0; i < 8; i++) {
      pat [i] = *ptr;
      ptr++;
    }
    PenPat ((ConstPatternParam) pat);
#endif
#ifdef WIN_MSWIN
    ptr = (Nlm_Uint1Ptr) pattern;
    q = (Nlm_Uint1Ptr) pat;
    for (i = 0; i < 8; i++) {
      *q = (Nlm_Uint1) ~(*ptr);
      ptr++;
      q++;
      *q = 0;
      q++;
    }
    Nlm_SelectPattern (PS_SOLID, 1, pat, ATT_PATTERN);
#endif
#ifdef WIN_X
    ptr = (Nlm_Uint1Ptr) pattern;
    q = (Nlm_Uint1Ptr) pat;
    for (i = 0; i < 8; i++) {
      *q = flip [*ptr];
      ptr++;
      q++;
    }
    Nlm_SelectPattern (pat);
#endif
#ifdef WIN_GIF
    gdImageSelectPattern(Nlm_currentGIF, (const unsigned char*)pattern);
#endif
  }
}

extern void Nlm_Solid (void)
{ /* Reset *both* line stile and drawing pattern to SOLID */
#ifdef WIN_MAC
  PenPat ((ConstPatternParam) blackPat);
#endif
#ifdef WIN_MSWIN
  Nlm_SelectPattern (PS_SOLID, 1, blackPat, ATT_PATTERN|ATT_PENSTYLE );
#endif
#ifdef WIN_X
  Nlm_SelectPattern( blackPat );
  if (Nlm_currentXDisplay  &&  Nlm_currentXGC) {
    XGCValues values;
    values.line_style = LineSolid;
    XChangeGC (Nlm_currentXDisplay, Nlm_currentXGC, GCLineStyle, &values);
  }
#endif
#ifdef WIN_GIF
  Nlm_SetPenPattern( blackPat );
  Nlm_curGIFLType = GIF_SOLID;
#endif
}

extern void Nlm_Dotted (void)
{
#ifdef WIN_MAC
  PenPat ((ConstPatternParam) dotPat);
#endif
#ifdef WIN_MSWIN
  Nlm_SelectPattern (PS_DOT, 1, dotPat, ATT_PENSTYLE|ATT_PATTERN);
#endif
#ifdef WIN_X
  if (Nlm_currentXDisplay  &&  Nlm_currentXGC) {
    XGCValues values;
    values.line_style = LineOnOffDash;
    XChangeGC (Nlm_currentXDisplay, Nlm_currentXGC, GCLineStyle, &values);
  }
#endif
#ifdef WIN_GIF
  Nlm_curGIFLType = GIF_DASHED;
#endif
}

extern void Nlm_Dashed (void)
{
#ifdef WIN_MAC
  PenPat ((ConstPatternParam) dashPat);
#endif
#ifdef WIN_MSWIN
  Nlm_SelectPattern (PS_DASH, 1, dashPat, ATT_PENSTYLE|ATT_PATTERN);
#endif
#ifdef WIN_X
  if (Nlm_currentXDisplay  &&  Nlm_currentXGC) {
    XGCValues values;
    values.line_style = LineOnOffDash;
    XChangeGC (Nlm_currentXDisplay, Nlm_currentXGC, GCLineStyle, &values);
  }
#endif
#ifdef WIN_GIF
  Nlm_curGIFLType = GIF_DASHED;
#endif
}

extern void Nlm_WidePen (Nlm_Int2 width)
{
#ifdef WIN_MAC
  PenPat ((ConstPatternParam) blackPat);
  PenSize (width, width);
#endif
#ifdef WIN_MSWIN
  Nlm_SelectPattern (PS_SOLID, width, blackPat, ATT_PENWIDTH);
#endif
#ifdef WIN_X
  XGCValues  values;

  if (Nlm_currentXDisplay != NULL && Nlm_currentXGC != NULL) {
    values.line_width = width;
    XChangeGC (Nlm_currentXDisplay, Nlm_currentXGC, GCLineWidth, &values);
  }
#endif
#ifdef WIN_GIF
#endif
}

/* esl: changed to work with new FontData format */
static void Nlm_LoadFontData (Nlm_FonT font,
                              Nlm_FonT next,
                              Nlm_Int4 refcnt,
                              Nlm_FontSpecPtr fsp,
#ifdef WIN_MAC
                              Nlm_Int2 num,
                              Nlm_Int2 size,
                              Nlm_Int2 styl,
#else
                              Nlm_FontTool  hdl,
#endif
                              Nlm_FonT print)
{ /* load font data */
  Nlm_FontData fdata;
  if (font == NULL) return;
  fdata.next = next;
  fdata.refcnt = refcnt;
  if (fsp != NULL) fdata.fontspec = *fsp;
  else Nlm_MemSet (&fdata.fontspec, 0, sizeof (Nlm_FontSpec));
#ifdef WIN_MAC
  fdata.number = num;
  fdata.size = size;
  fdata.style = styl;
#elif defined(WIN_MSWIN) || defined(WIN_X)
  fdata.handle = hdl;
#endif
  fdata.print = print;
  Nlm_SetFontData (font, &fdata);
}

/* esl: public proc for Windows font mapping (used also in ChooseFont) */
#ifdef WIN_MSWIN
extern void Nlm_FontSpecToLOGFONT (Nlm_FontSpecPtr fsp, LOGFONT *lfp)
{
  if (fsp == NULL || lfp == NULL) return;

  memset (lfp, 0, sizeof (LOGFONT));

  { /* height & width */
    HDC hDC = GetDC (NULL);
    lfp->lfHeight = - MulDiv (fsp->size, GetDeviceCaps (hDC, LOGPIXELSY), 72);
    ReleaseDC (NULL, hDC);
    lfp->lfWidth = 0;
  }

  /* weight & style */
  lfp->lfWeight = ((fsp->style & STYLE_BOLD) != 0) ? FW_BOLD : FW_DONTCARE; /*FW_NORMAL?*/
  lfp->lfItalic = (BYTE)((fsp->style & STYLE_ITALIC) != 0);
  lfp->lfUnderline = (BYTE)((fsp->style & STYLE_UNDERLINE) != 0);
  lfp->lfStrikeOut = (BYTE)((fsp->style & 128) != 0); /* Windows specific */

  /* character set */
  switch (fsp->charset) {
    case CHARSET_SYMBOL: lfp->lfCharSet = SYMBOL_CHARSET; break;
    case CHARSET_KANJI: lfp->lfCharSet = SHIFTJIS_CHARSET; break;
    default: lfp->lfCharSet = ANSI_CHARSET; break;
 }

  /* precisions & quality */
  lfp->lfOutPrecision = OUT_DEFAULT_PRECIS;
  lfp->lfClipPrecision = CLIP_DEFAULT_PRECIS;
  lfp->lfQuality = DEFAULT_QUALITY;

  { /* pitch & family */
    BYTE pitch, family;
    switch (fsp->pitch) {
      case PITCH_FIXED: pitch = FIXED_PITCH; break;
      case PITCH_VARIABLE: pitch = VARIABLE_PITCH; break;
      default: pitch = DEFAULT_PITCH; break;
    }
    switch (fsp->family) {
      case FAMILY_ROMAN: family = FF_ROMAN; break;
      case FAMILY_SWISS: family = FF_SWISS; break;
      case FAMILY_MODERN: family = FF_MODERN; break;
      case FAMILY_SCRIPT: family = FF_SCRIPT; break;
      case FAMILY_DECORATIVE: family = FF_DECORATIVE; break;
      default: family = FF_DONTCARE; break;
    }
    lfp->lfPitchAndFamily = (BYTE)(pitch | family);
  }

  /* face name */
  Nlm_StringNCpy_0 (lfp->lfFaceName, fsp->name, LF_FACESIZE - 1);
}
#endif /* WIN_MSWIN */

/* VL */
#ifdef WIN_X  
static Nlm_CharPtr Xi[]={
  "times",
  "palatino",
  "utopia",
  "new century schoolbook",
  "lucidabright",
  "lucida",
  "charter"
};

extern XFontStruct *Nlm_XLoadStandardFont(void)
{
  static char* s_StdXFontName[] = {
    "-*-helvetica-bold-r-*--14-*",
    "-*-helvetica-bold-r-*--*-140-*",
    "-*-helvetica-bold-r-*-*-*-140-*",
    "-*-fixed-medium-r-*--*-120-*",
    "-*-courier-medium-r-*--*-120-*",
    "8x13",
    "9x15",
    "fixed"
  };

  int i;
  for (i = 0;  i < DIM(s_StdXFontName);  i++) {
    XFontStruct* font = Nlm_XLoadQueryFont(Nlm_currentXDisplay,
                                           s_StdXFontName[i], FALSE);
    if ( font )
      return font;
  }

  /* the last-chance font */
  return Nlm_XLoadQueryFont(Nlm_currentXDisplay, "fixed", TRUE);
}
#endif


/* esl: main internal procedure to create fonts */
static Nlm_FonT Nlm_AddNewFont (Nlm_FontSpecPtr fsp, Nlm_Boolean residentp)
{
  Nlm_FonT rsult;
#ifdef WIN_MAC
  Nlm_Int2 num = 0;
  Nlm_Int2 styl = 0;
#else /* WIN_MSWIN | WIN_X */
  Nlm_FontTool hdl = NULL;
#endif

  if (fsp == NULL)
    return NULL;
  if ( (rsult = (Nlm_FonT)Nlm_HandNew( sizeof(Nlm_FontRec) )) == NULL )
    return NULL;

#ifdef WIN_MAC
  {{
    Nlm_Char temp[256];
    if (fsp->name[0] != '\0') {
      Nlm_StringNCpy_0 (temp, fsp->name, sizeof (temp));
      Nlm_CtoPstr (temp);
      GetFNum ((StringPtr) temp, &num);
    }
    if (num == 0) { /* use standard fonts */
      if (fsp->charset == CHARSET_KANJI) {
        if (fsp->pitch == PITCH_FIXED) num = 0x4034;     /* osaka(fixed) */
        else num = 0x4000;                               /* osaka */
      }
      else if (fsp->charset == CHARSET_SYMBOL) num = 23; /* symbol */
      else if (fsp->family == FAMILY_ROMAN) {
        if (fsp->pitch == PITCH_FIXED) num = 22;         /* courier */
        else num = 2;                                    /* new york */
      } else if (fsp->family == FAMILY_SWISS) {
        if (fsp->pitch == PITCH_FIXED) num = 4;          /* monaco */
        else num = 3;                                    /* geneva */
      } else if (fsp->family == FAMILY_MODERN) num = 22; /* courier */
      else if (fsp->family == FAMILY_SCRIPT) num = 12;   /* los angeles */
      else if (fsp->pitch == PITCH_FIXED) num = 4;       /* monaco */
      else num = 0;                                      /* system */
    }
    if ((fsp->style & STYLE_BOLD) != 0) styl += bold;
    if ((fsp->style & STYLE_ITALIC) != 0) styl += italic;
    if ((fsp->style & STYLE_UNDERLINE) != 0)  styl += underline;
    /* other Mac-specific styles??? */
    if ((fsp->style &  8) != 0)  styl += outline;
    if ((fsp->style & 16) != 0)  styl += shadow;
    if ((fsp->style & 32) != 0)  styl += condense;
    if ((fsp->style & 64) != 0)  styl += extend;
  }}
#endif /* WIN_MAC */
#ifdef WIN_MSWIN
  {{
    LOGFONT lf;
    Nlm_FontSpecToLOGFONT (fsp, &lf);
    hdl = CreateFontIndirect (&lf);
  }}
#endif /* WIN_MSWIN */
#ifdef WIN_X
  if ( !Nlm_currentXDisplay )
    return (Nlm_FonT) Nlm_MemFree(rsult);
    
  {{
    Nlm_Char fspec[256];
    Nlm_CharPtr bs = ((fsp->style & STYLE_BOLD) != 0) ? "bold" : "medium";
    Nlm_CharPtr is = "r";
    int sz = (int)fsp->size;

    if (fsp->style & STYLE_ITALIC) {
      size_t i;
      is="o";
      for (i=0; i<DIM(Xi); i++)
        if ( !Nlm_StringICmp(fsp->name, Xi[i]) ) {
          is="i";
          break;
        }
    }
    if ( *fsp->name ) { /* try the whole data */
      sprintf (fspec, "-*-%s-%s-%s-*--*-%d-*-*-*-*-*-*",
               fsp->name, bs, is, sz*10);
      hdl = Nlm_XLoadQueryFont (Nlm_currentXDisplay, fspec, FALSE);
      if ( ! hdl ) {
        sprintf (fspec, "-*-%s-%s-%s-*-*-*-%d-*-*-*-*-*-*",
                 fsp->name, bs, is, sz*10);
        hdl = Nlm_XLoadQueryFont (Nlm_currentXDisplay, fspec, FALSE);
      }
    }
    if (!hdl  &&  *fsp->name) { /* try the name only */
      sprintf (fspec, "-*-%s-*", fsp->name);
      hdl = Nlm_XLoadQueryFont (Nlm_currentXDisplay, fspec, FALSE);
    }
    if (hdl == NULL) { /* try charset/pitch/family */
      Nlm_CharPtr ns;
      if      (fsp->charset == CHARSET_SYMBOL) ns = "symbol";
      else if (fsp->family  == FAMILY_ROMAN  ) ns = "times";
      else if (fsp->family  == FAMILY_SWISS  ) ns = "helvetica";
      else if (fsp->family  == FAMILY_MODERN ) ns = "courier";
      else if (fsp->pitch   == PITCH_FIXED   ) ns = "fixed";
      else ns = "helvetica";

      sprintf(fspec, "-*-%s-%s-%s-*--*-%d-*", ns, bs, is, sz*10);
      hdl = Nlm_XLoadQueryFont (Nlm_currentXDisplay, fspec, FALSE);
      if ( !hdl ) {
        sprintf(fspec, "-*-%s-%s-%s-*-*-*-%d-*", ns, bs, is, sz*10);
        hdl = Nlm_XLoadQueryFont(Nlm_currentXDisplay, fspec, FALSE);
      }
    }

    if ( !hdl ) /* last resort:  try "standard" font */
      hdl = Nlm_XLoadStandardFont();
  }}
#endif /* WIN_X */

  Nlm_LoadFontData (rsult,
                    Nlm_fontList,
                    residentp ? -1 : 1,
                    fsp,
#ifdef WIN_MAC
                    num,
                    fsp->size,
                    styl,
#else /* WIN_MSWIN | WIN_X */
                    hdl,
#endif
                    NULL);

  Nlm_fontList = rsult;
  return rsult;
}


/* esl: public procedure to compare FontSpecs */
extern Nlm_Boolean Nlm_EqualFontSpec (Nlm_FontSpecPtr fsp1,
                                      Nlm_FontSpecPtr fsp2)
{
  if (fsp1 == NULL || fsp2 == NULL) return FALSE;
  return (Nlm_Boolean) (
    (Nlm_StrNICmp (fsp1->name, fsp2->name, FONT_NAME_SIZE) == 0) &&
    fsp1->size == fsp2->size &&
    fsp1->style == fsp2->style &&
    fsp1->charset == fsp2->charset &&
    fsp1->pitch == fsp2->pitch &&
    fsp1->family == fsp2->family
  );
}

/* esl: internal procedure to find existing fonts */
static Nlm_FonT Nlm_FindFont (Nlm_FontSpecPtr fsp)
{
  if (fsp != NULL) {
    Nlm_FonT font = Nlm_fontList;
    while (font != NULL) {
      Nlm_FntPtr fp = (Nlm_FntPtr) Nlm_HandLock (font);
      Nlm_Boolean found = Nlm_EqualFontSpec (fsp, &fp->fontspec);
      Nlm_FonT next = fp->next;
      Nlm_HandUnlock (font);
      if (found) return font;
      font = next;
    }
  }
  return NULL;
}

/* esl: internal procedure to increase refcnt */
static void Nlm_ReuseFont (Nlm_FonT font, Nlm_Boolean residentp)
{
  if (font != NULL) {
    Nlm_FntPtr fp = (Nlm_FntPtr) Nlm_HandLock (font);
    /* ASSERT (fp->refcnt != 0) */
    if (fp->refcnt > 0) {
      /* font is temporary: make it resident or increment refcnt */
      fp->refcnt = residentp ? -1 : fp->refcnt + 1;
    } /* else font is resident: just use it */
    Nlm_HandUnlock (font);
  }
}

/* esl: internal procedure to create resident/temporary fonts */
static Nlm_FonT Nlm_CreateFontIndirect (Nlm_FontSpecPtr fsp,
                                        Nlm_Boolean residentp)
{
  if (fsp != NULL) {
    if (fsp->family == 255) { /* magic number! */
      /* conventional specification of the standard fonts */
      return (fsp->pitch == PITCH_FIXED) ? Nlm_programFont : Nlm_systemFont;
    } else {
      Nlm_FonT font = Nlm_FindFont (fsp);
      if (font == NULL) {
        /* create new font and add it to the font list */
        return  Nlm_AddNewFont (fsp, residentp);
      } else {
        /* reuse existing font */
        Nlm_ReuseFont (font, residentp);
        return font;
      }
    }
  }
  return NULL;
}

#ifdef WIN_MSWIN
/* Creates(if yet no one) FonT based on the properties of "screen_font".
 * Use this function to choose a font corresponding to a stock font.
 */
static Nlm_FonT HFONT2Font(HFONT screen_font)
{
  HDC hDC;
  if ( !screen_font )
    return NULL;

  hDC = GetDC( NULL ); /* CreateIC("DISPLAY", NULL, NULL, NULL); */
  if ( !hDC )
    return NULL;

  {{
    TEXTMETRIC tMetrics;

    BOOL ok = SelectObject(hDC, screen_font)  &&
              GetTextMetrics(hDC, &tMetrics);
    int height = MulDiv(tMetrics.tmHeight, 72, GetDeviceCaps(hDC, LOGPIXELSY));

    ReleaseDC(NULL, hDC); /* DeleteDC( hDC ); */
    if ( !ok )
      return NULL;

    {{
      Nlm_FontSpec fs;
      fs.name[0] = '\0';
      fs.size    = (Nlm_Int2)height;
      fs.style   = 0;
      if (tMetrics.tmWeight == FW_BOLD)
        fs.style |= STYLE_BOLD;
      if ( tMetrics.tmItalic )
        fs.style |= STYLE_ITALIC;
      if ( tMetrics.tmUnderlined )
        fs.style |= STYLE_UNDERLINE;
      fs.charset = CHARSET_NULL;
      fs.pitch   = (Nlm_Uint1)((tMetrics.tmPitchAndFamily & TMPF_FIXED_PITCH) ?
                                 PITCH_VARIABLE : PITCH_FIXED);
      switch (tMetrics.tmPitchAndFamily & 0xF0)
        {
        case FF_DECORATIVE: fs.family = FAMILY_DECORATIVE;  break;
        case FF_MODERN:     fs.family = FAMILY_MODERN;      break;
        case FF_ROMAN:      fs.family = FAMILY_ROMAN;       break;
        case FF_SCRIPT:     fs.family = FAMILY_SCRIPT;      break;
        case FF_SWISS:      fs.family = FAMILY_SWISS;       break;
        default:
          fs.family = FAMILY_NULL;
        }

      return Nlm_CreateFontIndirect(&fs, TRUE);
    }}
  }}
}
#endif /* WIN_MSWIN */

/* esl: public procedures to create resident/temporary fonts */
extern Nlm_FonT Nlm_CreateFont (Nlm_FontSpecPtr fsp)
{
  return Nlm_CreateFontIndirect (fsp, FALSE);
}
extern Nlm_FonT Nlm_GetResidentFont (Nlm_FontSpecPtr fsp)
{
  return Nlm_CreateFontIndirect (fsp, TRUE);
}

/* esl: public procedure to "copy" font (actually to increment refcnt) */
extern Nlm_FonT Nlm_CopyFont (Nlm_FonT font)
{
  Nlm_ReuseFont (font, FALSE); /* do not make it resident */
  return font;
}

/* esl: fonts created this way are resident for compatibility */
extern Nlm_FonT Nlm_GetFont (Nlm_CharPtr name, Nlm_Int2 size,
                             Nlm_Boolean bld, Nlm_Boolean itlc,
                             Nlm_Boolean undrln, Nlm_CharPtr fmly)
{
  Nlm_Uint1 style, charset, pitch, family;

  /* style */
  style = 0;
  if (bld) style |= STYLE_BOLD;
  if (itlc) style |= STYLE_ITALIC;
  if (undrln) style |= STYLE_UNDERLINE;

  /* charset */
  charset = CHARSET_NULL;

  /* pitch */
  pitch = PITCH_NULL;

  /* family */
  if (fmly == NULL || fmly [0] != '\0') family = FAMILY_NULL;
  else if (Nlm_StringICmp (fmly, "Roman") == 0) family = FAMILY_ROMAN;
  else if (Nlm_StringICmp (fmly, "Swiss") == 0) family = FAMILY_SWISS;
  else if (Nlm_StringICmp (fmly, "Modern") == 0) family = FAMILY_MODERN;
  else if (Nlm_StringICmp (fmly, "Script") == 0) family = FAMILY_SCRIPT;
  else if (Nlm_StringICmp (fmly, "Decorative") == 0) family = FAMILY_DECORATIVE;
  else family = FAMILY_NULL;

  /* create resident font */
  { Nlm_FontSpec fs;
    Nlm_MemSet (fs.name, 0, FONT_NAME_SIZE);
    if (name != NULL) Nlm_StringNCpy_0 (fs.name, name, FONT_NAME_SIZE - 1);
    fs.size = size;
    fs.style = style;
    fs.charset = charset;
    fs.pitch = pitch;
    fs.family = family;
    return Nlm_CreateFontIndirect (&fs, TRUE);
  }
}

static Nlm_FonT Nlm_ParseFontSpec (Nlm_CharPtr spec)
{
  Nlm_Boolean  bold;
  Nlm_CharPtr  fmly;
  Nlm_Boolean  ital;
  Nlm_Char     name [128];
  Nlm_CharPtr  p;
  Nlm_CharPtr  q;
  Nlm_CharPtr  r;
  Nlm_FonT     rsult;
  Nlm_Int2     size;
  Nlm_Char     temp [128];
  Nlm_Boolean  undr;
  int          val;

  rsult = NULL;
  if (spec != NULL && spec [0] != '\0') {
    fmly = NULL;
    bold = FALSE;
    ital = FALSE;
    undr = FALSE;
    Nlm_StringNCpy_0 (name, spec, sizeof (name) - 1);
    p = Nlm_StringChr (name, ',');
    if (p != NULL) {
      *p = '\0';
      p++;
      q = Nlm_StringChr (p, ',');
      if (q != NULL) {
        *q = '\0';
        q++;
      }
      r = Nlm_StringChr (q, 'B');
      if (r == NULL) {
        r = Nlm_StringChr (q, 'b');
      }
      if (r != NULL) {
        bold = TRUE;
      }
      r = Nlm_StringChr (q, 'I');
      if (r == NULL) {
        r = Nlm_StringChr (q, 'i');
      }
      if (r != NULL) {
        ital = TRUE;
      }
      r = Nlm_StringChr (q, 'U');
      if (r == NULL) {
        r = Nlm_StringChr (q, 'u');
      }
      if (r != NULL) {
        undr = TRUE;
      }
      Nlm_StringNCpy_0 (temp, p, sizeof (temp) - 1);
      if (sscanf (temp, "%d", &val) != EOF) {
        size = (Nlm_Int2) val;
        rsult = Nlm_GetFont (name, size, bold, ital, undr, fmly);
      }
    }
  }
  return rsult;
}

/* esl: fonts created this way are resident for compatibility */
extern Nlm_FonT Nlm_ParseFont (Nlm_CharPtr spec)

{
  Nlm_Char     name [128];
  Nlm_CharPtr  p;
  Nlm_FonT     prtr;
  Nlm_CharPtr  q;
  Nlm_FonT     rsult;

  rsult = NULL;
  if (spec != NULL && spec [0] != '\0') {
    Nlm_StringNCpy_0 (name, spec, sizeof (name) - 1);
    p = Nlm_StringChr (name, '|');
    if (p != NULL) {
      *p = '\0';
      p++;
      while (*p == ' ') {
        p++;
      }
      q = name;
      while (*q == ' ') {
        q++;
      }
      rsult = Nlm_ParseFontSpec (q);
      prtr = Nlm_ParseFontSpec (p);
      Nlm_AssignPrinterFont (rsult, prtr);
    } else {
      q = name;
      while (*q == ' ') {
        q++;
      }
      rsult = Nlm_ParseFontSpec (q);
    }
  }
  return rsult;
}

extern Nlm_FonT Nlm_GetFontEx (Nlm_CharPtr name, Nlm_Int2 size,
                             Nlm_Boolean bld, Nlm_Boolean itlc,
                             Nlm_Boolean undrln, Nlm_CharPtr fmly,
							 Nlm_CharPtr chset, Nlm_Boolean fixed)
{
  Nlm_Uint1 style, charset, pitch, family;

  /* style */
  style = 0;
  if (bld) style |= STYLE_BOLD;
  if (itlc) style |= STYLE_ITALIC;
  if (undrln) style |= STYLE_UNDERLINE;

  /* charset */
  if (chset == NULL || chset [0] == '\0') charset = CHARSET_NULL;
  else if (Nlm_StringICmp (chset, "Ansi") == 0) charset = CHARSET_ANSI;
  else if (Nlm_StringICmp (chset, "Symbol") == 0) charset = CHARSET_SYMBOL;
  else if (Nlm_StringICmp (chset, "Kanji") == 0) charset = CHARSET_KANJI;
  else if (Nlm_StringICmp (chset, "Hangul") == 0) charset = CHASET_HANGUL;
  else charset = CHARSET_NULL;

  /* pitch */
  if (fixed) pitch = PITCH_FIXED;
  else pitch = PITCH_NULL;

  /* family */
  if (fmly == NULL || fmly [0] == '\0') family = FAMILY_NULL;
  else if (Nlm_StringICmp (fmly, "Roman") == 0) family = FAMILY_ROMAN;
  else if (Nlm_StringICmp (fmly, "Swiss") == 0) family = FAMILY_SWISS;
  else if (Nlm_StringICmp (fmly, "Modern") == 0) family = FAMILY_MODERN;
  else if (Nlm_StringICmp (fmly, "Script") == 0) family = FAMILY_SCRIPT;
  else if (Nlm_StringICmp (fmly, "Decorative") == 0) family = FAMILY_DECORATIVE;
  else family = FAMILY_NULL;

  /* create resident font */
  { Nlm_FontSpec fs;
    Nlm_MemSet (fs.name, 0, FONT_NAME_SIZE);
    if (name != NULL) Nlm_StringNCpy_0 (fs.name, name, FONT_NAME_SIZE - 1);
    fs.size = size;
    fs.style = style;
    fs.charset = charset;
    fs.pitch = pitch;
    fs.family = family;
    return Nlm_CreateFontIndirect (&fs, TRUE);
  }
}

static Nlm_FonT Nlm_ParseFontSpecEx (Nlm_CharPtr spec)
{
  Nlm_Boolean  bold;
  Nlm_CharPtr  fmly;
  Nlm_Boolean  ital;
  Nlm_Char     name [128];
  Nlm_CharPtr  p;
  Nlm_CharPtr  q;
  Nlm_CharPtr  r;
  Nlm_CharPtr  s;
  Nlm_FonT     rsult;
  Nlm_Int2     size;
  Nlm_Char     temp [128];
  Nlm_Boolean  undr;
  int          val;
  Nlm_Char     chst [128];
  Nlm_Boolean  fixd;

  rsult = NULL;
  if (spec != NULL && spec [0] != '\0') {
    fmly = NULL;
    bold = FALSE;
    ital = FALSE;
    undr = FALSE;
	fixd = FALSE;
	chst [0] = '\0';
    Nlm_StringNCpy_0 (name, spec, sizeof (name) - 1);
    p = Nlm_StringChr (name, ',');
    if (p != NULL) {
      *p = '\0';
      p++;
      q = Nlm_StringChr (p, ',');
      if (q != NULL) {
        *q = '\0';
        q++;
      }
      s = Nlm_StringChr (q, ',');
      if (s != NULL) {
        *s = '\0';
        s++;
      }
      r = Nlm_StringChr (q, 'B');
      if (r == NULL) {
        r = Nlm_StringChr (q, 'b');
      }
      if (r != NULL) {
        bold = TRUE;
      }
      r = Nlm_StringChr (q, 'I');
      if (r == NULL) {
        r = Nlm_StringChr (q, 'i');
      }
      if (r != NULL) {
        ital = TRUE;
      }
      r = Nlm_StringChr (q, 'U');
      if (r == NULL) {
        r = Nlm_StringChr (q, 'u');
      }
      if (r != NULL) {
        undr = TRUE;
      }
      r = Nlm_StringChr (q, 'F');
      if (r == NULL) {
        r = Nlm_StringChr (q, 'f');
      }
      if (r != NULL) {
        fixd = TRUE;
      }
      if (s != NULL) {
        Nlm_StringNCpy_0 (chst, s, sizeof (chst) - 1);
      }
      Nlm_StringNCpy_0 (temp, p, sizeof (temp) - 1);
      if (sscanf (temp, "%d", &val) != EOF) {
        size = (Nlm_Int2) val;
        rsult = Nlm_GetFontEx (name, size, bold, ital, undr, fmly, chst, fixd);
      }
    }
  }
  return rsult;
}

extern Nlm_FonT Nlm_ParseFontEx (Nlm_CharPtr scrSpec, Nlm_CharPtr prtSpec)
{
  Nlm_CharPtr  p;
  Nlm_FonT     prtr;
  Nlm_CharPtr  q;
  Nlm_FonT     rsult;

  rsult = NULL;
  if (scrSpec != NULL && scrSpec [0] != '\0') {
  	q = scrSpec;
  	while (*q == ' ') {
  		q++;
  	}
  	rsult = Nlm_ParseFontSpecEx(q);
  }
  prtr = NULL;
  if (prtSpec != NULL && prtSpec [0] != '\0') {
  	p = prtSpec;
  	while (*p == ' ') {
  		p++;
  	}
  	prtr = Nlm_ParseFontSpecEx(p);
    Nlm_AssignPrinterFont (rsult, prtr);
  }
  return rsult;
}

/* esl: public procedures to get specifications for common fonts */
/* ToDo: add platform-dependent names? */

static Nlm_FontSpec Nlm_commonFontSpec;
extern Nlm_FontSpecPtr Nlm_Helvetica (Nlm_Int2 size, Nlm_Uint1 style)
{
  Nlm_commonFontSpec.name[0] = '\0';
  Nlm_commonFontSpec.size = size;
  Nlm_commonFontSpec.style = style;
  Nlm_commonFontSpec.charset = CHARSET_ANSI;
  Nlm_commonFontSpec.pitch = PITCH_VARIABLE;
  Nlm_commonFontSpec.family = FAMILY_SWISS;
  return &Nlm_commonFontSpec;
}

extern Nlm_FontSpecPtr Nlm_Times (Nlm_Int2 size, Nlm_Uint1 style)
{
  Nlm_commonFontSpec.name[0] = '\0';
  Nlm_commonFontSpec.size = size;
  Nlm_commonFontSpec.style = style;
  Nlm_commonFontSpec.charset = CHARSET_ANSI;
  Nlm_commonFontSpec.pitch = PITCH_VARIABLE;
  Nlm_commonFontSpec.family = FAMILY_ROMAN;
  return &Nlm_commonFontSpec;
}

extern Nlm_FontSpecPtr Nlm_Courier (Nlm_Int2 size, Nlm_Uint1 style)
{
  Nlm_commonFontSpec.name[0] = '\0';
  Nlm_commonFontSpec.size = size;
  Nlm_commonFontSpec.style = style;
  Nlm_commonFontSpec.charset = CHARSET_ANSI;
  Nlm_commonFontSpec.pitch = PITCH_FIXED;
  Nlm_commonFontSpec.family = FAMILY_MODERN;
  return &Nlm_commonFontSpec;
}

extern Nlm_FontSpecPtr Nlm_Symbol (Nlm_Int2 size, Nlm_Uint1 style)
{
  Nlm_commonFontSpec.name[0] = '\0';
  Nlm_commonFontSpec.size = size;
  Nlm_commonFontSpec.style = style;
  Nlm_commonFontSpec.charset = CHARSET_SYMBOL; /* should be enough */
  Nlm_commonFontSpec.pitch = PITCH_NULL;
  Nlm_commonFontSpec.family = FAMILY_NULL;
  return &Nlm_commonFontSpec;
}

extern Nlm_FontSpecPtr Nlm_Gothic (Nlm_Int2 size, Nlm_Uint1 style)

{
#ifdef WIN_MAC
  strncpy(Nlm_commonFontSpec.name, "Osaka", sizeof(Nlm_commonFontSpec.name));
#endif
#ifdef WIN_MSWIN
  strncpy(Nlm_commonFontSpec.name, "\x82\x6c\x82\x72\x20\x82\x6f\x83\x53\x83\x56\x83\x62\x83\x4e", sizeof(Nlm_commonFontSpec.name));
#endif
#ifdef WIN_MOTIF
  strncpy(Nlm_commonFontSpec.name, "gothic", sizeof(Nlm_commonFontSpec.name));
#endif
  Nlm_commonFontSpec.size = size;
  Nlm_commonFontSpec.style = style;
  Nlm_commonFontSpec.charset = CHARSET_KANJI;
  Nlm_commonFontSpec.pitch = PITCH_NULL;
  Nlm_commonFontSpec.family = FAMILY_GOTHIC;
  return &Nlm_commonFontSpec;
}

extern Nlm_FontSpecPtr Nlm_Minchou (Nlm_Int2 size, Nlm_Uint1 style)

{
#ifdef WIN_MAC
  strncpy(Nlm_commonFontSpec.name, "Osaka", sizeof(Nlm_commonFontSpec.name));
#endif
#ifdef WIN_MSWIN
  strncpy(Nlm_commonFontSpec.name, "\x82\x6c\x82\x72\x20\x82\x6f\x96\xbe\x92\xa9", sizeof(Nlm_commonFontSpec.name));
#endif
#ifdef WIN_MOTIF
  strncpy(Nlm_commonFontSpec.name, "minchou", sizeof(Nlm_commonFontSpec.name));
#endif
  Nlm_commonFontSpec.size = size;
  Nlm_commonFontSpec.style = style;
  Nlm_commonFontSpec.charset = CHARSET_KANJI;
  Nlm_commonFontSpec.pitch = PITCH_NULL;
  Nlm_commonFontSpec.family = FAMILY_MINCHOU;
  return &Nlm_commonFontSpec;
}

extern Nlm_FontSpecPtr Nlm_GothicFixed (Nlm_Int2 size, Nlm_Uint1 style)

{
  Nlm_Gothic(size, style);
#ifdef WIN_MAC
  strncpy(Nlm_commonFontSpec.name, "Osaka\x81\x7c\x93\x99\x95\x9d", sizeof(Nlm_commonFontSpec.name));
#endif
#ifdef WIN_MSWIN
  strncpy(Nlm_commonFontSpec.name, "\x82\x6c\x82\x72\x20\x83\x53\x83\x56\x83\x62\x83\x4e", sizeof(Nlm_commonFontSpec.name));
#endif
  Nlm_commonFontSpec.pitch = PITCH_FIXED;
  return &Nlm_commonFontSpec;
}
extern Nlm_FontSpecPtr Nlm_MinchouFixed (Nlm_Int2 size, Nlm_Uint1 style)
{
  Nlm_Minchou(size, style);
#ifdef WIN_MAC
  strncpy(Nlm_commonFontSpec.name, "Osaka\x81\x7c\x93\x99\x95\x9d", sizeof(Nlm_commonFontSpec.name));
#endif
#ifdef WIN_MSWIN
  strncpy(Nlm_commonFontSpec.name, "\x82\x6c\x82\x72\x20\x96\xbe\x92\xa9", sizeof(Nlm_commonFontSpec.name));
#endif
  Nlm_commonFontSpec.pitch = PITCH_FIXED;
  return &Nlm_commonFontSpec;
}


/* esl: public procedure to delete temporary fonts */
/* ToDo: track associated fdata.print font! */
extern Nlm_FonT Nlm_DeleteFont (Nlm_FonT font)
{
  if (font != NULL) {
    Nlm_FontData fdata;
    Nlm_GetFontData (font, &fdata);
    /* ASSERT(fdata.refcnt != 0) */
    if (fdata.refcnt > 0) {
      if (fdata.refcnt > 1) {
        /* still in use somewhere else: decrement refcnt */
        fdata.refcnt--;
        Nlm_SetFontData (font, &fdata);
      } else {
        /* last reference should be lost: remove from the list and delete */
        Nlm_FonT prev = NULL;
        Nlm_FonT curr = Nlm_fontList;
        while (curr != NULL) {
          if (curr == font) { /* font is found! */
            Nlm_GetFontData (curr, &fdata);
            curr = fdata.next;
            if (prev != NULL) { /* remove in the middle */
              Nlm_GetFontData (prev, &fdata);
              fdata.next = curr;
              Nlm_SetFontData (prev, &fdata);
            } else { /* remove first */
              Nlm_fontList = curr;
            }
          } else { /* check next! */
            prev = curr;
            Nlm_GetFontData (curr, &fdata);
            curr = fdata.next;
          }
        }
        /* if font is selected, unselect it before deletion! */
        if (font == Nlm_fontInUse) Nlm_SelectFont (Nlm_systemFont);
        /* delete it! */
        Nlm_GetFontData (font, &fdata);
#ifdef WIN_MSWIN
        if (fdata.handle != NULL)
          DeleteObject (fdata.handle);
#endif
#ifdef WIN_MOTIF
        if (fdata.handle != NULL)
          XFreeFont (Nlm_currentXDisplay, fdata.handle);
#endif
        Nlm_HandFree (font);
      }
    } /* else font is resident: leave it as it is */
  }
  return NULL;
}

/* esl: procedure to enumerate resident fonts */
extern Nlm_FonT Nlm_FindNextResidentFont (Nlm_FonT font)
{
  Nlm_FonT curr;
  Nlm_FontData fdata;
  if (font == NULL) {
    curr = Nlm_fontList;
  } else {
    Nlm_GetFontData (font, &fdata);
    curr = fdata.next;
  }
  while (curr != NULL) { /* skip temporary fonts */
    Nlm_GetFontData (curr, &fdata);
    if (fdata.refcnt < 0) return curr; else curr = fdata.next;
  }
  return NULL;
}

/* esl: procedure to extract specification from font */
extern Nlm_Boolean Nlm_GetFontSpec (Nlm_FonT font, Nlm_FontSpecPtr fsp)
{
  if (font == NULL || fsp == NULL) return FALSE;
  /* use conventional specification for system/program fonts */
  if (font == Nlm_systemFont || font == Nlm_programFont) {
  /*alexs: copy fontspec for System and Program font */
    Nlm_FntPtr fp = (Nlm_FntPtr) Nlm_HandLock (font);
    *fsp = fp->fontspec;
    Nlm_HandUnlock (font);
    fsp->family = 255; /* magic number! */
    fsp->pitch = (Nlm_Uint1)((font == Nlm_programFont) ?
                               PITCH_FIXED : PITCH_NULL);
  } else {
    Nlm_FntPtr fp = (Nlm_FntPtr) Nlm_HandLock (font);
    *fsp = fp->fontspec;
    Nlm_HandUnlock (font);
  }
  return TRUE;
}

extern void Nlm_SelectFont (Nlm_FonT f)

{
  Nlm_FontData  fdata;

  if (f != NULL) {
    Nlm_GetFontData (f, &fdata);
    if (fdata.print != NULL && Nlm_nowPrinting) {
      f = fdata.print;
      Nlm_GetFontData (f, &fdata);
    }
#ifdef WIN_MAC
    TextFont (fdata.number);
    TextSize (fdata.size);
    TextFace (fdata.style);
#endif
#ifdef WIN_MSWIN
    if (Nlm_currentHDC != NULL) {
      SelectObject (Nlm_currentHDC, fdata.handle);
    }
#endif
#ifdef WIN_X
    if (Nlm_currentXDisplay != NULL && Nlm_currentXGC != NULL) {
      if (fdata.handle != NULL) {
        XSetFont (Nlm_currentXDisplay, Nlm_currentXGC, fdata.handle->fid);
        currentFont = fdata.handle;
      }
    }
#endif
#ifdef WIN_GIF
    if (fdata.fontspec.size >= 16) {
      Nlm_curGIFFont = gdFont8X16;
    } else if (fdata.fontspec.size >= 15) {
      Nlm_curGIFFont = gdFont9X15b;
    } else if (fdata.fontspec.size >= 13) {
      Nlm_curGIFFont = gdFont7X13b;
    } else if (fdata.fontspec.size >= 10) {
      Nlm_curGIFFont = gdFont6X12;
    } else {
      Nlm_curGIFFont = gdFont5X8;
    }
#endif
    Nlm_fontInUse = f;
  }
}

/* esl ToDo: increment refcnt for fdata.print font? */
extern void Nlm_AssignPrinterFont (Nlm_FonT scrnFont, Nlm_FonT prtrFont)
{
  if (scrnFont != NULL) {
    Nlm_FntPtr fp = (Nlm_FntPtr) Nlm_HandLock (scrnFont);
    fp->print = prtrFont;
    Nlm_HandUnlock (scrnFont);
  }
}

extern Nlm_Int2 Nlm_CharWidth (Nlm_Char ch)

{
#ifdef WIN_MAC
  return (CharWidth (ch));
#endif
#ifdef WIN_MSWIN
  Nlm_Char  str [4];

  str [0] = ch;
  str [1] = '\0';
  return (Nlm_TextWidth (str, 1));
#endif
#ifdef WIN_X
  Nlm_Char  str [4];

  str [0] = ch;
  str [1] = '\0';
  return (Nlm_TextWidth (str, 1));
#endif
#ifdef WIN_GIF
 return (Nlm_Int2)Nlm_curGIFFont->w;
#endif
}

extern Nlm_Int2 Nlm_StringWidth(const Nlm_Char* text)
{
  return Nlm_TextWidth(text, Nlm_StringLen(text));
}

extern Nlm_Int2 Nlm_TextWidth(const Nlm_Char* text, size_t len)
{
  if (text  &&  len) {{
#if defined(WIN_MAC)
    return TextWidth(text, 0, (Nlm_Int2)len);

#elif defined(WIN_MSWIN)
    SIZE tag;
    if ( Nlm_currentHDC )
      GetTextExtentPoint(Nlm_currentHDC, text, (int)len, &tag);
    else {
      HDC hDC = CreateIC("DISPLAY", NULL, NULL, NULL);
      GetTextExtentPoint (hDC, text, (int)len, &tag);
      DeleteDC(hDC);
    }
    return (Nlm_Int2)tag.cx;

#elif defined(WIN_X)
    if ( Nlm_GetTextMetrics() )
      return (Nlm_Int2)XTextWidth(&fontInfo, text, (int)len);

#elif defined(WIN_GIF)
    return (Nlm_Int2)(Nlm_curGIFFont->w * len);
#endif
  }}

 return 0;
}


extern Nlm_Int2 Nlm_Ascent (void)

{
#ifdef WIN_MAC
  FontInfo  fontinfo;

  GetFontInfo (&fontinfo);
  return (fontinfo.ascent);
#endif
#ifdef WIN_MSWIN
  Nlm_Int2  rsult;

  rsult = 0;
  if (Nlm_GetTextMetrics ()) {
    rsult = (Nlm_Int2) textMetrics.tmAscent;
  }
  return rsult;
#endif
#ifdef WIN_X
  Nlm_Int2  rsult;

  rsult = 0;
  if (Nlm_GetTextMetrics ()) {
    rsult = MAX (fontInfo.ascent, fontInfo.max_bounds.ascent);
  }
  return rsult;
#endif
#ifdef WIN_GIF
 return (Nlm_Int2)Nlm_curGIFFont->h - (Nlm_Int2)Nlm_curGIFFont->d;
#endif
}

extern Nlm_Int2 Nlm_Descent (void)

{
#ifdef WIN_MAC
  FontInfo  fontinfo;

  GetFontInfo (&fontinfo);
  return (fontinfo.descent);
#endif
#ifdef WIN_MSWIN
  Nlm_Int2  rsult;

  rsult = 0;
  if (Nlm_GetTextMetrics ()) {
    rsult = (Nlm_Int2) textMetrics.tmDescent;
  }
  return rsult;
#endif
#ifdef WIN_X
  Nlm_Int2  rsult;

  rsult = 0;
  if (Nlm_GetTextMetrics ()) {
    rsult = MAX (fontInfo.descent, fontInfo.max_bounds.descent);
  }
  return rsult;
#endif
#ifdef WIN_GIF
 return (Nlm_Int2)Nlm_curGIFFont->d;
#endif
}

extern Nlm_Int2 Nlm_Leading (void)

{
#ifdef WIN_MAC
  FontInfo  fontinfo;

  GetFontInfo (&fontinfo);
  return (fontinfo.leading);
#endif
#ifdef WIN_MSWIN
  Nlm_Int2  rsult;

  rsult = 0;
  if (Nlm_GetTextMetrics ()) {
    rsult = (Nlm_Int2) textMetrics.tmExternalLeading;
  }
  return rsult;
#endif
#ifdef WIN_X
  return 0;
#endif
#ifdef WIN_GIF
 return 0;
#endif
}

extern Nlm_Int2 Nlm_FontHeight (void)

{
#ifdef WIN_MAC
  FontInfo  fontinfo;

  GetFontInfo (&fontinfo);
  return (fontinfo.ascent + fontinfo.descent);
#endif
#ifdef WIN_MSWIN
  Nlm_Int2  rsult;

  rsult = 0;
  if (Nlm_GetTextMetrics ()) {
    rsult = (Nlm_Int2) textMetrics.tmHeight;
  }
  return rsult;
#endif
#ifdef WIN_X
  Nlm_Int2  rsult;

  rsult = 0;
  if (Nlm_GetTextMetrics ()) {
    rsult = (MAX (fontInfo.ascent, fontInfo.max_bounds.ascent) +
             MAX (fontInfo.descent, fontInfo.max_bounds.descent));
  }
  return rsult;
#endif
#ifdef WIN_GIF
 return (Nlm_Int2)Nlm_curGIFFont->h;
#endif
}

extern Nlm_Int2 Nlm_LineHeight (void)

{
#ifdef WIN_MAC
  FontInfo  fontinfo;

  GetFontInfo (&fontinfo);
  return (fontinfo.ascent + fontinfo.descent + fontinfo.leading);
#endif
#ifdef WIN_MSWIN
  Nlm_Int2  rsult;

  rsult = 0;
  if (Nlm_GetTextMetrics ()) {
    rsult = (Nlm_Int2) (textMetrics.tmHeight + textMetrics.tmExternalLeading);
  }
  return rsult;
#endif
#ifdef WIN_X
  Nlm_Int2  rsult;

  rsult = 0;
  if (Nlm_GetTextMetrics ()) {
    rsult = (MAX (fontInfo.ascent, fontInfo.max_bounds.ascent) +
             MAX (fontInfo.descent, fontInfo.max_bounds.descent));
  }
  return rsult;
#endif
#ifdef WIN_GIF
 return (Nlm_Int2)(Nlm_curGIFFont->h + 1);
#endif
}

extern Nlm_Int2 Nlm_MaxCharWidth (void)

{
#ifdef WIN_MAC
  FontInfo  fontinfo;

  GetFontInfo (&fontinfo);
  return (fontinfo.widMax);
#endif
#ifdef WIN_MSWIN
  Nlm_Int2  rsult;

  rsult = 0;
  if (Nlm_GetTextMetrics ()) {
    rsult = (Nlm_Int2) textMetrics.tmMaxCharWidth;
  }
  return rsult;
#endif
#ifdef WIN_X
  Nlm_Int2  rsult;

  rsult = 0;
  if (Nlm_GetTextMetrics ()) {
    rsult = fontInfo.max_bounds.width;
  }
  return rsult;
#endif
#ifdef WIN_GIF
 return (Nlm_Int2)Nlm_curGIFFont->w;
#endif
}


size_t Nlm_FitStringWidth(const Nlm_Char PNTR str, Nlm_Int4 max_width)
{
  size_t len;
  if (!str  ||  !*str  ||  max_width < 1)
    return 0;

#if defined(WIN_X)
  {{
    if ( !Nlm_GetTextMetrics() )
      return 0;

    if (fontInfo.min_byte1  ||  fontInfo.max_byte1)
      return 0; /* two-byte font */

    if ( !fontInfo.per_char ) /* non-proportional font */
      return (fontInfo.max_bounds.width > 0 ?
              max_width / fontInfo.max_bounds.width : 0);

    {{
      Nlm_Int4 width = 0;
      unsigned min_char = fontInfo.min_char_or_byte2;
      unsigned max_char = fontInfo.max_char_or_byte2;
      XCharStruct *per_char = fontInfo.per_char;
      int def_char_width =
        (min_char <= fontInfo.default_char  &&
         fontInfo.default_char <= max_char) ?
        per_char[fontInfo.default_char - min_char].width : 0;
      const unsigned char *ustr = (const unsigned char *) str;

      for (len = 0;  width <= max_width  &&  *ustr;  len++, ustr++)
        {
          if (min_char <= *ustr  &&  *ustr <= max_char) {
            int w = per_char[*ustr - min_char].width;
            width += w ? w : def_char_width;
          }
          else
            width += def_char_width;
        }

      return (width > max_width ? len-1 : len);
    }}
  }}
#else
  {{ /* platform-independent algorithm */
    Nlm_Int2 max_char_width = Nlm_MaxCharWidth();
    if (max_char_width < 1)
      return 0;

    ASSERT ( max_width > 0 );
    if (Nlm_StringWidth( (Nlm_CharPtr)str ) <= max_width)
      return Nlm_StringLen( str );

    len = (size_t)(max_width / max_char_width);
    ASSERT ( len < StringLen( str ) );

    while (str[len] != '\0'  &&
           Nlm_TextWidth((Nlm_CharPtr)str, len) <= max_width)
      len++;

    return (len ? len-1 : 0);
  }}
#endif
}


extern void Nlm_SetPen (Nlm_PoinT pt)
{
#ifdef WIN_MAC
  MoveTo (pt.x, pt.y);
#endif
#ifdef WIN_MSWIN
  if (Nlm_currentHDC != NULL) {
    MoveToEx (Nlm_currentHDC, pt.x, pt.y, NULL);
  }
#endif
#ifdef WIN_X
  currentPoint = pt;
#endif
#ifdef WIN_GIF
  Nlm_curGIFPoint = pt;
#endif
}

extern void Nlm_GetPen (Nlm_PointPtr pt)

{
#ifdef WIN_MAC
  Nlm_PointTool  ptool;

  if (pt != NULL) {
    GetPen (&ptool);
    Local__PointToolToPoinT (ptool, pt);
  }
#endif
#ifdef WIN_MSWIN
  POINT  pos;

  if (pt != NULL && Nlm_currentHDC != NULL) {
    GetCurrentPositionEx (Nlm_currentHDC, &pos);
    pt->x = (Nlm_Int2) pos.x;
    pt->y = (Nlm_Int2) pos.y;
  }
#endif
#ifdef WIN_X
  if (pt != NULL) {
    *pt = currentPoint;
  }
#endif
#ifdef WIN_GIF
  *pt = Nlm_curGIFPoint;
#endif
}

extern void Nlm_PaintChar (Nlm_Char ch)

{
#ifdef WIN_MAC
  DrawChar (ch);
#endif
#ifdef WIN_MSWIN
  Nlm_PoinT  pt;
  Nlm_Char   str [2];

  if (Nlm_currentHDC != NULL) {
    str [0] = ch;
    str [1] = '\0';
    Nlm_GetPen (&pt);
    TextOut (Nlm_currentHDC, pt.x, pt.y - Nlm_Ascent (), str, 1);
    pt.x += Nlm_CharWidth (ch);
    Nlm_MoveTo (pt.x, pt.y);
  }
#endif
#ifdef WIN_X
  Nlm_PoinT  pt;
  Nlm_Char   str [2];

  if (Nlm_currentXDisplay != NULL && Nlm_currentXWindow != 0 &&
      Nlm_currentXGC != NULL) {
    str [0] = ch;
    str [1] = '\0';
    Nlm_GetPen (&pt);
    XDrawString (Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
                 pt.x - Nlm_XOffset, pt.y - Nlm_YOffset, str, 1);
    pt.x += Nlm_CharWidth (ch);
    Nlm_MoveTo (pt.x, pt.y);
  }
#endif
#ifdef WIN_GIF
  if ( Nlm_currentGIF != NULL ){
    gdImageChar ( Nlm_currentGIF, Nlm_curGIFFont, Nlm_curGIFPoint.x, 
                                  Nlm_curGIFPoint.y - Nlm_Ascent(), 
                                  (int)ch, Nlm_curGIFColor );
    Nlm_curGIFPoint.x += Nlm_CharWidth (ch);
  }
#endif
}


extern void Nlm_PaintStringEx(Nlm_CharPtr text, Nlm_Int2 x, Nlm_Int2 y)
{
#ifdef WIN_MSWIN
  if (!text  ||  !*text  ||  !Nlm_currentHDC)
    return;

  TextOut(Nlm_currentHDC, x, y - Nlm_Ascent(),
          text, (Nlm_Int2)Nlm_StringLen( text ));
  Nlm_MoveTo((Nlm_Int2)(x + Nlm_StringWidth(text)), y);
#else
  Nlm_MoveTo(x, y);
  Nlm_PaintString( text );
#endif
}
extern void Nlm_PaintString (Nlm_CharPtr text)
{
#ifdef WIN_MAC
  Nlm_PoinT  pt;
  Nlm_Char   str [256];

  if (text != NULL) {
    Nlm_GetPen (&pt);
    if (Nlm_StringLen (text) > 0) {
      Nlm_StringNCpy_0 (str, text, sizeof (str));
      Nlm_CtoPstr (str);
      DrawString ((StringPtr) str);
    }
    pt.x += Nlm_StringWidth (text);
    Nlm_MoveTo (pt.x, pt.y);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_PoinT pt;
  Nlm_GetPen( &pt );
  Nlm_PaintStringEx(text, pt.x, pt.y);
#endif
#ifdef WIN_X
  Nlm_Int2   len;
  Nlm_PoinT  pt;

  if (text != NULL && Nlm_currentXDisplay != NULL &&
      Nlm_currentXWindow != 0 && Nlm_currentXGC != NULL) {
    len = (Nlm_Int2) Nlm_StringLen (text);
    Nlm_GetPen (&pt);
    if (len > 0) {
      XDrawString (Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
                   pt.x - Nlm_XOffset, pt.y - Nlm_YOffset, text, len);
    }
    pt.x += Nlm_StringWidth (text);
    Nlm_MoveTo (pt.x, pt.y);
  }
#endif
#ifdef WIN_GIF
  if ( text != NULL && Nlm_currentGIF != NULL ){
    gdImageString ( Nlm_currentGIF, Nlm_curGIFFont, Nlm_curGIFPoint.x, 
                    Nlm_curGIFPoint.y - Nlm_Ascent(),               
                    text, Nlm_curGIFColor );
    Nlm_curGIFPoint.x += Nlm_StringWidth (text);
  }
#endif
}

#ifdef VAR_ARGS
void CDECL Nlm_PaintText (format, va_alist)
  char *format;
  va_dcl
#else
void CDECL Nlm_PaintText (char *format, ...)
#endif

{
#ifdef WIN_MAC
  va_list    args;
  Nlm_PoinT  pt;
  Nlm_Char   str [256];

  if (format != NULL) {
#ifdef VAR_ARGS
    va_start(args);
#else
    va_start(args, format);
#endif
    vsprintf(str, format, args);
    va_end(args);
    Nlm_GetPen (&pt);
    if (Nlm_StringLen (str) > 0) {
      Nlm_CtoPstr (str);
      DrawString ((StringPtr) str);
    }
    pt.y += Nlm_LineHeight ();
    Nlm_MoveTo (pt.x, pt.y);
  }
#endif
#ifdef WIN_MSWIN
  va_list    args;
  Nlm_Int2   len;
  Nlm_PoinT  pt;
  Nlm_Char   str [256];

  if (format != NULL && Nlm_currentHDC != NULL) {
#ifdef VAR_ARGS
    va_start(args);
#else
    va_start(args, format);
#endif
    vsprintf(str, format, args);
    va_end(args);
    len = (Nlm_Int2) Nlm_StringLen (str);
    Nlm_GetPen (&pt);
    if (len > 0) {
      TextOut (Nlm_currentHDC, pt.x, pt.y - Nlm_Ascent (), str, len);
    }
    pt.y += Nlm_LineHeight ();
    Nlm_MoveTo (pt.x, pt.y);
  }
#endif
#ifdef WIN_X
  va_list    args;
  Nlm_Int2   len;
  Nlm_PoinT  pt;
  Nlm_Char   str [256];

  if (format != NULL && Nlm_currentXDisplay != NULL &&
      Nlm_currentXWindow != 0 && Nlm_currentXGC != NULL) {
#ifdef VAR_ARGS
    va_start(args);
#else
    va_start(args, format);
#endif
    vsprintf(str, format, args);
    va_end(args);
    len = (Nlm_Int2) Nlm_StringLen (str);
    Nlm_GetPen (&pt);
    if (len > 0) {
      XDrawString (Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
                   pt.x - Nlm_XOffset, pt.y - Nlm_YOffset, str, len);
    }
    pt.y += Nlm_LineHeight ();
    Nlm_MoveTo (pt.x, pt.y);
  }
#endif
#ifdef WIN_GIF
  va_list    args;
  Nlm_Char   str [256];

  if ( format != NULL && Nlm_currentGIF != NULL ){
#ifdef VAR_ARGS
    va_start(args);
#else
    va_start(args, format);
#endif
    vsprintf(str, format, args);
    va_end(args);
    gdImageString ( Nlm_currentGIF, Nlm_curGIFFont, Nlm_curGIFPoint.x, 
                    Nlm_curGIFPoint.y - Nlm_Ascent(), 
                    str, Nlm_curGIFColor );
    Nlm_curGIFPoint.x += Nlm_StringWidth (str);
  }
#endif
}

extern void Nlm_DrawString (Nlm_RectPtr r, Nlm_CharPtr text,
                            Nlm_Char jst, Nlm_Boolean gray)

{
  Nlm_DrawText (r, text, Nlm_StringLen (text), jst, gray);
}

extern void Nlm_DrawText (Nlm_RectPtr r, Nlm_CharPtr text,
                          size_t len, Nlm_Char jst,
                          Nlm_Boolean gray)

{
#ifdef WIN_MAC
  Nlm_Int2      delta;
  Nlm_Int2      height;
  Nlm_Int2      just;
  Nlm_Int2      limit;
  PenState      pnState;
  Nlm_RectTool  rtool;

  if (r != NULL) {
    Nlm_EraseRect (r);
    if (text != NULL && len > 0) {
      Local__RecTToRectTool (r, &rtool);
      limit = ABS (r->bottom - r->top);
      height = Nlm_LineHeight ();
      delta = limit - height;
      if (delta > 0) {
        rtool.top += delta / 2;
        rtool.bottom = rtool.top + height;
      }
      switch (jst) {
        case 'r':
          just = -1;
          break;
        case 'l':
          just = 0;
          break;
        case 'c':
          just = 1;
          break;
        default:
          just = 0;
          break;
      }
      TETextBox (text, len, &rtool, just);
      if (gray) {
        GetPenState (&pnState);
        PenMode (patBic);
        PenPat ((ConstPatternParam) grayPat);
        PaintRect (&rtool);
        SetPenState (&pnState);
      }
    }
  }
#endif
#ifdef WIN_MSWIN
  Nlm_Int2      format;
  Nlm_Int4      oldcolor;
  Nlm_RectTool  rtool;
  Nlm_FontData  fdata;
  SIZE          tsize;
  HDC           hdc;

  if (r != NULL && Nlm_currentHDC != NULL) {
    Local__RecTToRectTool (r, &rtool);
    if (Nlm_currentHWnd != NULL) {
      Nlm_EraseRect (r);
    } else {
      FillRect (Nlm_currentHDC, &rtool, hWhiteBrush);
    }
    if (text != NULL && len > 0) {
      switch (jst) {
        case 'r':
          format = DT_RIGHT;
          break;
        case 'l':
          format = DT_LEFT;
          break;
        case 'c':
          format = DT_CENTER;
          break;
        default:
          format = DT_LEFT;
          break;
      }
      if (gray) {
        winTextColor = GetSysColor (COLOR_GRAYTEXT);
        oldcolor = SetTextColor (Nlm_currentHDC, winTextColor);
      }
      hdc = Nlm_GetPicWinHDC();
      if ( hdc == NULL ){
        /* esl: DT_VCENTER must be combined with DT_SINGLELINE!  */
        DrawText (Nlm_currentHDC, text, (Nlm_Int2) len, &rtool,
                  format | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
      } else {
        if ( Nlm_fontInUse != NULL ){
          Nlm_GetFontData (Nlm_fontInUse, &fdata);
          SelectObject (hdc, fdata.handle);
        }
        GetTextExtentPoint ( hdc, text, len, &tsize );
        tsize.cy = (r->top + r->bottom)/2 - tsize.cy/2;
        switch ( format ){
          case DT_LEFT:
            tsize.cx = r->left; break;
          case DT_RIGHT:
            tsize.cx = r->right - tsize.cx; break;
          default:
            tsize.cx = (r->left + r->right)/2 - tsize.cx/2;
        }
        Nlm_PaintStringEx(text, (Nlm_Int2)tsize.cx, (Nlm_Int2)tsize.cy);
      }
      if (gray) {
        SetTextColor (Nlm_currentHDC, oldcolor);
        winTextColor = oldcolor;
      }
    }
  }
#endif
#ifdef WIN_X
  Nlm_Int2   delta;
  Nlm_Int2   height;
  Nlm_Int2   limit;
  Pixmap     pix = 0;
  Nlm_PoinT  pt;
  Nlm_Int2   width;

  if (r != NULL && Nlm_currentXDisplay != NULL &&
      Nlm_currentXWindow != 0 && Nlm_currentXGC != NULL) {
    Nlm_EraseRect (r);
    if (text != NULL && len > 0) {
      pt.x = r->left;
      pt.y = r->top;
      limit = ABS (r->right - r->left);
      width = Nlm_TextWidth (text, len);
      while (len > 0 && width > limit) {
        len--;
        width = Nlm_TextWidth (text, len);
      }
      delta = limit - width;
      switch (jst) {
        case 'r':
          pt.x += delta;
          break;
        case 'l':
          break;
        case 'c':
          pt.x += delta / 2;
          break;
        default:
          break;
      }
      limit = ABS (r->bottom - r->top);
      height = Nlm_LineHeight ();
      delta = limit - height;
      if (delta > 0) {
        pt.y += delta / 2;
      }
      if (limit >= height) {
        if (gray) {
          pix = XCreateBitmapFromData (Nlm_currentXDisplay, Nlm_currentXWindow,
                                       (char *) grayPat, 8, 8);
          if (pix != 0 && Nlm_currentXGC != NULL) {
            XSetStipple (Nlm_currentXDisplay, Nlm_currentXGC, pix);
          }
        }
        XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, FillStippled);
        XDrawString (Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
                     pt.x - Nlm_XOffset, pt.y + Nlm_Ascent () - Nlm_YOffset,
                     text, (int) len);
        if (gray && pix != 0) {
          XFreePixmap (Nlm_currentXDisplay, pix);
          if (Nlm_currentXGC != NULL) {
            XSetStipple (Nlm_currentXDisplay, Nlm_currentXGC, currentPixmap);
          }
        }
        XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, currentFillStyle);
      }
    }
  }
#endif
#ifdef WIN_GIF
  Nlm_Int2   delta;
  Nlm_Int2   height;
  Nlm_Int2   limit;
  Nlm_PoinT  pt;
  Nlm_Int2   width;

  if (r != NULL && Nlm_currentGIF != NULL ) {
    Nlm_EraseRect (r);
    if (text != NULL && len > 0) {
      pt.x = r->left;
      pt.y = r->top;
      limit = ABS (r->right - r->left);
      width = Nlm_TextWidth (text, len);
      while (len > 0 && width > limit) {
        len--;
        width = Nlm_TextWidth (text, len);
      }
      delta = limit - width;
      switch (jst) {
        case 'r':
          pt.x += delta;
          break;
        case 'l':
          break;
        case 'c':
          pt.x += delta / 2;
          break;
        default:
          break;
      }
      limit = ABS (r->bottom - r->top);
      height = Nlm_LineHeight ();
      delta = limit - height;
      if (delta > 0) {
        pt.y += delta / 2;
      }
      if (limit >= height) {
        char save;
        save = text[len];
        text[len] = 0;
        gdImageString ( Nlm_currentGIF, Nlm_curGIFFont, 
                        pt.x, pt.y,
                        text, Nlm_curGIFColor );
        text[len] = save;
      }
    }
  }
#endif
}

extern void Nlm_MoveTo (Nlm_Int2 x, Nlm_Int2 y)

{
#ifdef WIN_MAC
  MoveTo (x, y);
#endif
#ifdef WIN_MSWIN
  if (Nlm_currentHDC != NULL) {
    MoveToEx (Nlm_currentHDC, x, y, NULL);
  }
#endif
#ifdef WIN_X
  currentPoint.x = x;
  currentPoint.y = y;
#endif
#ifdef WIN_GIF
  Nlm_curGIFPoint.x = x;
  Nlm_curGIFPoint.y = y;
#endif
}


extern void Nlm_LineTo (Nlm_Int2 x, Nlm_Int2 y)

{
#ifdef WIN_MAC
  LineTo (x, y);
#endif
#ifdef WIN_MSWIN
  if (Nlm_currentHDC != NULL) {
    LineTo (Nlm_currentHDC, x, y);
  }
#endif
#ifdef WIN_X
  Nlm_PoinT to_point;
  to_point.x = x;
  to_point.y = y;
  Nlm_DrawLine(currentPoint, to_point);
#endif
#ifdef WIN_GIF
  if ( Nlm_currentGIF != NULL ){
    if ( Nlm_curGIFLType == GIF_DASHED ) {
      gdImageDashedLine ( Nlm_currentGIF, Nlm_curGIFPoint.x, Nlm_curGIFPoint.y,
                          x, y, Nlm_curGIFColor );
    } else {
      gdImageLine ( Nlm_currentGIF, Nlm_curGIFPoint.x, Nlm_curGIFPoint.y,
                    x, y, Nlm_curGIFColor );
    }
    Nlm_MoveTo ( x,y );
  }
#endif
}

extern void Nlm_DrawLine (Nlm_PoinT pt1, Nlm_PoinT pt2)
{
#ifdef WIN_MAC
  MoveTo (pt1.x, pt1.y);
  LineTo (pt2.x, pt2.y);
#endif
#ifdef WIN_MSWIN
  if (Nlm_currentHDC != NULL) {
    if (pt1.x == pt2.x  &&  pt1.y == pt2.y) {
      SetPixel(Nlm_currentHDC, pt1.x, pt1.y, winTextColor); /* just a dot */
    } else {
      MoveToEx(Nlm_currentHDC, pt1.x, pt1.y, NULL);
      LineTo  (Nlm_currentHDC, pt2.x, pt2.y);
    }
  }
#endif
#ifdef WIN_X
  if (Nlm_currentXDisplay != NULL && Nlm_currentXWindow != 0 &&
      Nlm_currentXGC != NULL)
    {
      XDrawLine(Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
                pt1.x - Nlm_XOffset, pt1.y - Nlm_YOffset,
                pt2.x - Nlm_XOffset, pt2.y - Nlm_YOffset);
      currentPoint.x = pt2.x;
      currentPoint.y = pt2.y;
    }
#endif
#ifdef WIN_GIF
  if ( Nlm_currentGIF != NULL ){
    if ( Nlm_curGIFLType == GIF_DASHED ) {
      gdImageDashedLine ( Nlm_currentGIF, pt1.x, pt1.y, pt2.x, pt2.y, 
                          Nlm_curGIFColor );
    } else {
      gdImageLine ( Nlm_currentGIF, pt1.x, pt1.y, pt2.x, pt2.y, 
                          Nlm_curGIFColor );
    }
    Nlm_MoveTo ( pt2.x, pt2.y );
  }
#endif
}

#ifdef WIN_GIF
static void Nlm_GIFRgnToRect ( Nlm_RgnTool t, Nlm_RectPtr r )
{
  Nlm_RectPtr rTool;

  if ( t != NULL && r != NULL ){
    rTool = (Nlm_RectPtr)HandLock(t);
    *r = *rTool;
    HandUnlock(rTool);
  }
}

static void Nlm_RectToGIFRgn ( Nlm_RectPtr r, Nlm_RgnTool t )
{
  Nlm_RectPtr rTool;

  if ( t != NULL && r != NULL ){
    rTool = (Nlm_RectPtr)HandLock(t);
    *rTool = *r;
    HandUnlock(rTool);
  }
}
#endif

extern void Nlm_LoadPt (Nlm_PointPtr pt, Nlm_Int2 x, Nlm_Int2 y)

{
  if (pt != NULL) {
    pt->x = x;
    pt->y = y;
  }
}

extern void Nlm_AddPt (Nlm_PoinT src, Nlm_PointPtr dst)

{
  if (dst != NULL) {
    dst->x += src.x;
    dst->y += src.y;
  }
}

extern void Nlm_SubPt (Nlm_PoinT src, Nlm_PointPtr dst)

{
  if (dst != NULL) {
    dst->x -= src.x;
    dst->y -= src.y;
  }
}

extern Nlm_Boolean Nlm_EqualPt (Nlm_PoinT p1, Nlm_PoinT p2)

{
  return (Nlm_Boolean) (p1.x == p2.x && p1.y == p2.y);
}

extern Nlm_Boolean Nlm_PtInRect (Nlm_PoinT pt, Nlm_RectPtr r)

{
  return (Nlm_Boolean) (r != NULL && pt.x >= r->left && pt.x < r->right &&
                        pt.y >= r->top && pt.y < r->bottom);
}

extern Nlm_Boolean Nlm_PtInRgn (Nlm_PoinT pt, Nlm_RegioN rgn)

{
  Nlm_RgnTool    ntool;
  Nlm_Boolean    rsult;
#ifdef WIN_MAC
  Nlm_PointTool  ptool;
#endif
#ifdef WIN_GIF
  Nlm_RecT r;
#endif

  rsult = FALSE;
  if (rgn != NULL) {
    ntool = (Nlm_RgnTool) rgn;
#ifdef WIN_MAC
    Local__PoinTToPointTool (pt, &ptool);
    rsult = PtInRgn (ptool, ntool);
#endif
#ifdef WIN_MSWIN
    rsult = (Nlm_Boolean) PtInRegion (ntool, pt.x, pt.y);
#endif
#ifdef WIN_X
    rsult = (XPointInRegion (ntool, pt.x, pt.y) != 0);
#endif
#ifdef WIN_GIF
    Nlm_GIFRgnToRect ( ntool, &r );
    rsult = Nlm_PtInRect (pt, &r);
#endif
  }
  return rsult;
}

extern void Nlm_LoadRect (Nlm_RectPtr r, Nlm_Int2 lf,
                          Nlm_Int2 tp, Nlm_Int2 rt,
                          Nlm_Int2 bt)

{
  if (r != NULL) {
    r->left = lf;
    r->top = tp;
    r->right = rt;
    r->bottom = bt;
  }
}

extern void Nlm_UpsetRect (Nlm_RectPtr r, Nlm_Int2 lf,
                           Nlm_Int2 tp, Nlm_Int2 rt,
                           Nlm_Int2 bt)

{
  if (r != NULL) {
    r->left += lf;
    r->top += tp;
    r->right -= rt;
    r->bottom -= bt;
  }
}

extern void Nlm_OffsetRect (Nlm_RectPtr r, Nlm_Int2 dx, Nlm_Int2 dy)

{
  if (r != NULL) {
    r->left += dx;
    r->top += dy;
    r->right += dx;
    r->bottom += dy;
  }
}

extern void Nlm_InsetRect (Nlm_RectPtr r, Nlm_Int2 dx, Nlm_Int2 dy)

{
  if (r != NULL) {
    r->left += dx;
    r->top += dy;
    r->right -= dx;
    r->bottom -= dy;
  }
}


static void Nlm_LoadNormalized (Nlm_RectPtr dst, Nlm_RectPtr src)
{
  if (!src  ||  !dst)
    return;

  Nlm_LoadRect(dst,
               (Nlm_Int2)MIN(src->left, src->right),
               (Nlm_Int2)MIN(src->top, src->bottom),
               (Nlm_Int2)MAX(src->left, src->right),
               (Nlm_Int2)MAX(src->top, src->bottom));
}


extern Nlm_Boolean Nlm_SectRect(Nlm_RectPtr src1, Nlm_RectPtr src2,
                                Nlm_RectPtr dst)
{
#if defined(WIN_MSWIN) || defined(WIN_MAC)
  Nlm_Boolean   rsult;
  Nlm_RectTool  rtool1;
  Nlm_RectTool  rtool2;
  Nlm_RectTool  rtool3;
#elif defined(WIN_X) || defined(WIN_GIF)
  Nlm_RecT      rect1;
  Nlm_RecT      rect2;
#endif

  if (!src1  ||  !src2  ||  !dst)
    return FALSE;

#if defined(WIN_MSWIN) || defined(WIN_MAC)
  Local__RecTToRectTool(src1, &rtool1);
  Local__RecTToRectTool(src2, &rtool2);
#ifdef WIN_MSWIN
  rsult = (Nlm_Boolean)IntersectRect(&rtool3, &rtool1, &rtool2);
#else
  rsult = SectRect(&rtool1, &rtool2, &rtool3);
#endif
  Local__RectToolToRecT(&rtool3, dst);
  return rsult;

#elif defined(WIN_X) || defined(WIN_GIF)
    Nlm_LoadNormalized(&rect1, src1);
    Nlm_LoadNormalized(&rect2, src2);
    dst->left   = MAX(rect1.left,   rect2.left  );
    dst->right  = MIN(rect1.right,  rect2.right );
    dst->top    = MAX(rect1.top,    rect2.top   );
    dst->bottom = MIN(rect1.bottom, rect2.bottom);
    if (dst->left > dst->right  ||  dst->top > dst->bottom)
      {
        Nlm_LoadRect(dst, 0, 0, 0, 0);
        return FALSE;
      }
    return TRUE;
#endif
}


extern Nlm_Boolean Nlm_UnionRect(Nlm_RectPtr src1, Nlm_RectPtr src2,
                                 Nlm_RectPtr dst)
{
#if defined(WIN_MSWIN) || defined(WIN_MAC)
  Nlm_Boolean   rsult;
  Nlm_RectTool  rtool1;
  Nlm_RectTool  rtool2;
  Nlm_RectTool  rtool3;
#elif defined(WIN_X) || defined(WIN_GIF)
  Nlm_RecT      rect1;
  Nlm_RecT      rect2;
#endif

  if (!src1  ||  !src2  ||  !dst)
    return FALSE;

#if defined(WIN_MSWIN) || defined(WIN_MAC)
  Local__RecTToRectTool(src1, &rtool1);
  Local__RecTToRectTool(src2, &rtool2);
#ifdef WIN_MSWIN
  rsult = (Nlm_Boolean)UnionRect(&rtool3, &rtool1, &rtool2);
#else
  UnionRect(&rtool1, &rtool2, &rtool3);
  rsult = EmptyRect(&rtool3);
#endif
  Local__RectToolToRecT(&rtool3, dst);
  return rsult;

#elif defined(WIN_X) || defined(WIN_GIF)
  Nlm_LoadNormalized (&rect1, src1);
  Nlm_LoadNormalized (&rect2, src2);
  dst->left   = MIN(rect1.left,   rect2.left  );
  dst->right  = MAX(rect1.right,  rect2.right );
  dst->top    = MIN(rect1.top,    rect2.top   );
  dst->bottom = MAX(rect1.bottom, rect2.bottom);
  return TRUE;
#endif
}


extern Nlm_Boolean Nlm_EqualRect (Nlm_RectPtr r1, Nlm_RectPtr r2)

{
  return (Nlm_Boolean) (r1 != NULL && r2 != NULL && r1->left == r2->left &&
                        r1->top == r2->top && r1->right == r2->right &&
                        r1->bottom == r2->bottom);
}

extern Nlm_Boolean Nlm_EmptyRect (Nlm_RectPtr r)

{
  return (Nlm_Boolean) ! (r != NULL && r->bottom > r->top && r->right > r->left);
}

extern Nlm_Boolean Nlm_RectInRect (Nlm_RectPtr r1, Nlm_RectPtr r2)

{
  Nlm_Boolean  rsult;

  rsult = FALSE;
  if (r1 != NULL && r2 != NULL &&
    r1->top >= r2->top && r1->bottom <= r2->bottom &&
    r1->left >= r2->left && r1->right <= r2->right) {
    rsult = TRUE;
  }
  return rsult;
}

extern Nlm_Boolean Nlm_RectInRgn (Nlm_RectPtr r, Nlm_RegioN rgn)

{
  Nlm_RgnTool   ntool;
  Nlm_Boolean   rsult;
  Nlm_RectTool  rtool;
#ifdef WIN_GIF
  Nlm_RecT      rect;
#endif

  rsult = FALSE;
  if (r != NULL && rgn != NULL) {
    Local__RecTToRectTool (r, &rtool);
    ntool = (Nlm_RgnTool) rgn;
#ifdef WIN_MAC
    rsult = RectInRgn (&rtool, ntool);
#endif
#ifdef WIN_MSWIN
    rsult = (Nlm_Boolean) RectInRegion (ntool, &rtool);
#endif
#ifdef WIN_X
    rsult = (XRectInRegion (ntool, rtool.x, rtool.y, rtool.width+1, rtool.height+1) != 0);
#endif
#ifdef WIN_GIF
    Nlm_GIFRgnToRect ( ntool, &rect );
    rsult = Nlm_RectInRect (r, &rect) ;
#endif
  }
  return rsult;
}


extern void Nlm_EraseRect (Nlm_RectPtr r)
{
  Nlm_RectTool rtool;
  if ( !r )
    return;

  Local__RecTToRectTool(r, &rtool);
    
#ifdef WIN_MAC
  EraseRect (&rtool);
#endif
#ifdef WIN_MSWIN
  if (Nlm_currentHDC  &&  Nlm_currentHWnd) {
    Nlm_Int4 bkColor    = GetBkColor (Nlm_currentHDC);
    HBRUSH   hBackBrush = CreateSolidBrush (bkColor);
    FillRect (Nlm_currentHDC, &rtool, hBackBrush);
    DeleteObject (hBackBrush);
  }
#endif
#ifdef WIN_X
  if (Nlm_currentXDisplay  &&  Nlm_currentXWindow) {
    XGCValues values;
    XGetGCValues(Nlm_currentXDisplay, Nlm_currentXGC, GCFillStyle, &values);
    XSetForeground (Nlm_currentXDisplay, Nlm_currentXGC, currentBkColor);
    XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, FillSolid);
    XFillRectangle (Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
                    rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
                    rtool.width+1, rtool.height+1);
    XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, values.fill_style);
    XSetForeground (Nlm_currentXDisplay, Nlm_currentXGC, currentFgColor);
  }
#endif
#ifdef WIN_GIF
  if ( Nlm_currentGIF )
    gdImageFilledRectangle(Nlm_currentGIF, rtool.left, rtool.top, 
                           rtool.right, rtool.bottom, 0);
#endif
}


extern void Nlm_FrameRect (Nlm_RectPtr r)

{
#ifdef WIN_MAC
  Nlm_RectTool  rtool;

  if (r != NULL) {
    Local__RecTToRectTool (r, &rtool);
    if (rtool.right == rtool.left) {
      rtool.right = rtool.left + 1;
    }
    if (rtool.bottom == rtool.top) {
      rtool.bottom = rtool.top + 1;
    }
    FrameRect (&rtool);
  }
#endif
#ifdef WIN_MSWIN
  HBRUSH        oldBrush;
  POINT         pos;
  Nlm_RectTool  rtool;

  if (r != NULL && Nlm_currentHDC != NULL) {
    oldBrush = SelectObject (Nlm_currentHDC, GetStockObject (NULL_BRUSH));
    Local__RecTToRectTool (r, &rtool);
    if (rtool.right == rtool.left) {
      rtool.right = rtool.left + 1;
    }
    if (rtool.bottom == rtool.top) {
      rtool.bottom = rtool.top + 1;
    }
    if ( (rtool.bottom == (rtool.top+1)) &&
         (rtool.right == (rtool.left+1)) ){
      GetCurrentPositionEx (Nlm_currentHDC, &pos);
      MoveToEx (Nlm_currentHDC, rtool.left, rtool.top, NULL);
      LineTo (Nlm_currentHDC, rtool.left+1, rtool.top);
      MoveToEx (Nlm_currentHDC, pos.x, pos.y, NULL);
    } else {
      Rectangle (Nlm_currentHDC, rtool.left, rtool.top, rtool.right+1, rtool.bottom+1);
    }
    if (oldBrush != NULL) {
      SelectObject (Nlm_currentHDC, oldBrush);
    }
  }
#endif
#ifdef WIN_X
  Nlm_RectTool  rtool;

  if (r != NULL && Nlm_currentXDisplay != NULL &&
      Nlm_currentXWindow != 0 && Nlm_currentXGC != NULL) {
    Local__RecTToRectTool (r, &rtool);
    if ( !rtool.width  ) rtool.width++;
    if ( !rtool.height ) rtool.height++;
    XDrawRectangle (Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
                    rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
                    rtool.width, rtool.height);
  }
#endif
#ifdef WIN_GIF
  Nlm_RectTool  rtool;

  if ( r != NULL && Nlm_currentGIF != NULL ){
    Local__RecTToRectTool (r, &rtool);
    gdImageRectangle (Nlm_currentGIF, rtool.left, rtool.top, 
                      rtool.right, rtool.bottom, Nlm_curGIFColor );
  }
#endif
}

extern void Nlm_PaintRect(Nlm_RectPtr r)

{
  Nlm_RectTool rtool;
  if ( !r )
    return;
  Local__RecTToRectTool(r, &rtool);

#if defined(WIN_MAC)
  if (rtool.right == rtool.left) {
    rtool.right = rtool.left + 1;
  }
  if (rtool.bottom == rtool.top) {
    rtool.bottom = rtool.top + 1;
  }
  PaintRect(&rtool);

#elif defined(WIN_MSWIN)
  if ( Nlm_currentHDC ) {
    HPEN oldPen = SelectObject (Nlm_currentHDC, GetStockObject (NULL_PEN));
    Rectangle(Nlm_currentHDC,
              rtool.left, rtool.top, rtool.right+2, rtool.bottom+2);
    if ( oldPen )
      SelectObject(Nlm_currentHDC, oldPen);
  }

#elif defined(WIN_X)
  if (Nlm_currentXDisplay  &&  Nlm_currentXWindow  &&  Nlm_currentXGC) {
    XFillRectangle(Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
                   rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
                   rtool.width + 1, rtool.height + 1);
  }
#elif defined(WIN_GIF)
  if ( Nlm_currentGIF ) {
    gdImageFilledRectangle(Nlm_currentGIF, rtool.left, rtool.top, 
                           rtool.right, rtool.bottom, Nlm_curGIFColor);
  }
#endif
}

extern void Nlm_InvertRect (Nlm_RectPtr r)
{
#ifndef WIN_GIF
  Nlm_RectTool  rtool;
#endif

  if (r == NULL)
    return;

#ifdef WIN_MAC
  Local__RecTToRectTool (r, &rtool);
  InvertRect (&rtool);
#endif
#ifdef WIN_MSWIN
  if (Nlm_currentHDC == NULL)
    return;

  Local__RecTToRectTool (r, &rtool);
  InvertRect (Nlm_currentHDC, &rtool);
#endif
#ifdef WIN_X
  if (Nlm_currentXDisplay == NULL  ||
      Nlm_currentXWindow == 0  ||  Nlm_currentXGC == NULL)
    return;

  Local__RecTToRectTool (r, &rtool);
  XSetFunction  (Nlm_currentXDisplay, Nlm_currentXGC, GXinvert);
  XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, FillStippled);
  XFillRectangle(Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
                 rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
                 rtool.width + 1, rtool.height + 1);
  XSetFunction  (Nlm_currentXDisplay, Nlm_currentXGC, currentFunction);
  XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, currentFillStyle);
#endif
}


extern void Nlm_ScrollRect (Nlm_RectPtr r, Nlm_Int2 dx, Nlm_Int2 dy)

{
#ifdef WIN_MAC
  Nlm_RectTool  rtool;

  if (r != NULL) {
    Local__RecTToRectTool (r, &rtool);
    ScrollRect (&rtool, dx, dy, (Nlm_RgnTool) Nlm_scrollRgn);
    InvalRgn ((Nlm_RgnTool) Nlm_scrollRgn);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_RectTool  rtool;

  if (r != NULL && Nlm_currentHDC != NULL) {
    SetRectRgn ((Nlm_RgnTool) Nlm_scrollRgn, 0, 0, 0, 0);
    Local__RecTToRectTool (r, &rtool);
    ScrollDC (Nlm_currentHDC, dx, dy, &rtool, &rtool,
              (Nlm_RgnTool) Nlm_scrollRgn, NULL);
    if (Nlm_currentHWnd != NULL && Nlm_scrollRgn != NULL) {
      FillRgn (Nlm_currentHDC, (Nlm_RgnTool) Nlm_scrollRgn,
               GetBackgroundBrush (Nlm_currentHWnd));
    }
    InvalidateRgn (Nlm_currentHWnd, (Nlm_RgnTool) Nlm_scrollRgn, TRUE);
  }
#endif
#ifdef WIN_X
  XEvent        event;
  unsigned int  height;
  Nlm_RecT      rct;
  Nlm_RectTool  rtool;
  unsigned int  width;
  unsigned int  dstX;
  unsigned int  dstY;
  unsigned int  srcX;
  unsigned int  srcY;

  if (r != NULL && Nlm_currentXDisplay != NULL &&
      Nlm_currentXGC != NULL && Nlm_currentXWindow != 0) {
    height = ABS (r->bottom - r->top) - ABS (dy);
    width = ABS (r->right - r->left) - ABS (dx);
    if (dx > 0) {
      srcX = r->left - Nlm_XOffset;
      dstX = r->left - Nlm_XOffset + dx;
    } else if (dx < 0) {
      srcX = r->left - Nlm_XOffset - dx;
      dstX = r->left - Nlm_XOffset;
    } else {
      srcX = r->left - Nlm_XOffset;
      dstX = r->left - Nlm_XOffset;
    }
    if (dy > 0) {
      srcY = r->top - Nlm_YOffset;
      dstY = r->top - Nlm_YOffset + dy;
    } else if (dy < 0) {
      srcY = r->top - Nlm_YOffset - dy;
      dstY = r->top - Nlm_YOffset;
    } else {
      srcY = r->top - Nlm_YOffset;
      dstY = r->top - Nlm_YOffset;
    }
    if (Nlm_hasColor) {
      XSetForeground (Nlm_currentXDisplay, Nlm_currentXGC, currentBkColor);
    }
    XCopyArea (Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXWindow,
               Nlm_currentXGC, srcX, srcY, width, height, dstX, dstY);
    XSync (Nlm_currentXDisplay, FALSE);
    if (! XCheckTypedWindowEvent (Nlm_currentXDisplay,
                                  Nlm_currentXWindow, NoExpose, &event)) {
      while (XCheckTypedWindowEvent (Nlm_currentXDisplay,
             Nlm_currentXWindow, GraphicsExpose, &event)) {
        XClearArea (Nlm_currentXDisplay, Nlm_currentXWindow,
                    event.xgraphicsexpose.x, event.xgraphicsexpose.y,
                    event.xgraphicsexpose.width, event.xgraphicsexpose.height,
                    TRUE);
      }
    }
    if (dx > 0) {
      rct = *r;
      rct.right = rct.left + dx;
      Local__RecTToRectTool (&rct, &rtool);
      XClearArea (Nlm_currentXDisplay, Nlm_currentXWindow,
                  rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
                  rtool.width, rtool.height, TRUE);
    } else if (dx < 0) {
      rct = *r;
      rct.left = rct.right + dx;
      Local__RecTToRectTool (&rct, &rtool);
      XClearArea (Nlm_currentXDisplay, Nlm_currentXWindow,
                  rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
                  rtool.width, rtool.height, TRUE);
    }
    if (dy > 0) {
      rct = *r;
      rct.bottom = rct.top + dy;
      Local__RecTToRectTool (&rct, &rtool);
      XClearArea (Nlm_currentXDisplay, Nlm_currentXWindow,
                  rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
                  rtool.width, rtool.height, TRUE);
    } else if (dy < 0) {
      rct = *r;
      rct.top = rct.bottom + dy;
      Local__RecTToRectTool (&rct, &rtool);
      XClearArea (Nlm_currentXDisplay, Nlm_currentXWindow,
                  rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
                  rtool.width, rtool.height, TRUE);
    }
    if (Nlm_hasColor) {
      XSetForeground (Nlm_currentXDisplay, Nlm_currentXGC, currentFgColor);
    }
  }
#endif
#ifdef WIN_GIF
#endif
}

extern void Nlm_EraseOval (Nlm_RectPtr r)
{
  Nlm_RectTool rtool;
  if ( !r )
    return;
  Local__RecTToRectTool(r, &rtool);

#if   defined(WIN_MAC)
  EraseOval(&rtool);

#elif defined(WIN_MSWIN)
  if (Nlm_currentHDC  &&  Nlm_currentHWnd) {
    HPEN   xPen   = SelectObject(Nlm_currentHDC, GetStockObject(NULL_PEN));
    HBRUSH xBrush = SelectObject(Nlm_currentHDC,
                                 GetBackgroundBrush(Nlm_currentHWnd));
    Ellipse(Nlm_currentHDC,
            rtool.left, rtool.top, rtool.right + 2, rtool.bottom + 2);
    if ( xPen )
      SelectObject(Nlm_currentHDC, xPen);
    if ( xBrush )
      SelectObject(Nlm_currentHDC, xBrush);
  }

#elif defined(WIN_X)
  if (Nlm_currentXDisplay  &&  Nlm_currentXWindow) {
    XGCValues values;
    XGetGCValues  (Nlm_currentXDisplay, Nlm_currentXGC, GCFillStyle, &values);
    XSetForeground(Nlm_currentXDisplay, Nlm_currentXGC, currentBkColor);
    XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, FillSolid);
    XFillArc(Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
             rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
             rtool.width, rtool.height, 0, 23040);
    XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, values.fill_style);
    XSetForeground(Nlm_currentXDisplay, Nlm_currentXGC, currentFgColor);
  }

#elif defined(WIN_GIF)
  gdImageEllipse(Nlm_currentGIF,
                 (rtool.left + rtool.right) /2, (rtool.top + rtool.bottom) /2,
                 (rtool.right - rtool.left) /2, (rtool.bottom - rtool.top) /2,
                 0, TRUE);
#endif
}

extern void Nlm_FrameOval(Nlm_RectPtr r)
{
  Nlm_RectTool rtool;
  if ( !r )
    return;
  Local__RecTToRectTool(r, &rtool);

#if   defined(WIN_MAC)
  FrameOval(&rtool);

#elif defined(WIN_MSWIN)
  if ( Nlm_currentHDC ) {
    HBRUSH xBrush = SelectObject(Nlm_currentHDC, GetStockObject(NULL_BRUSH));
    Ellipse(Nlm_currentHDC, rtool.left, rtool.top, rtool.right, rtool.bottom);
    if ( xBrush )
      SelectObject(Nlm_currentHDC, xBrush);
  }

#elif defined(WIN_X)
  if ( Nlm_currentXDisplay  &&  Nlm_currentXWindow  &&  Nlm_currentXGC) {
    XDrawArc(Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
             rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
             rtool.width - 1, rtool.height - 1, 0, 23040);
  }

#elif defined(WIN_GIF)
  gdImageEllipse(Nlm_currentGIF,
                 (rtool.left + rtool.right) /2, (rtool.top + rtool.bottom) /2,
                 (rtool.right - rtool.left) /2, (rtool.bottom - rtool.top) /2,
                 Nlm_curGIFColor, FALSE);
#endif
}

extern void Nlm_PaintOval(Nlm_RectPtr r)
{
  Nlm_RectTool rtool;
  if ( !r )
    return;
  Local__RecTToRectTool(r, &rtool);

#if   defined(WIN_MAC)
  PaintOval(&rtool);

#elif defined(WIN_MSWIN)
  if ( Nlm_currentHDC ) {
    HPEN xPen = SelectObject(Nlm_currentHDC, GetStockObject(NULL_PEN));
    Ellipse(Nlm_currentHDC,
            rtool.left, rtool.top, rtool.right+1, rtool.bottom+1);
    if ( xPen )
      SelectObject(Nlm_currentHDC, xPen);
  }

#elif defined(WIN_X)
  if (Nlm_currentXDisplay  &&  Nlm_currentXWindow  &&  Nlm_currentXGC) {
    XFillArc(Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
             rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
             rtool.width, rtool.height, 0, 23040);
  }

#elif defined(WIN_GIF)
  gdImageEllipse(Nlm_currentGIF,
                 (rtool.left + rtool.right) /2, (rtool.top + rtool.bottom) /2,
                 (rtool.right - rtool.left) /2, (rtool.bottom - rtool.top) /2,
                 Nlm_curGIFColor, TRUE);
#endif
}


extern void Nlm_InvertOval(Nlm_RectPtr r)
{
  Nlm_RectTool rtool;
  if ( !r )
    return;
  Local__RecTToRectTool(r, &rtool);

#if   defined(WIN_MAC)
  InvertOval (&rtool);

#elif defined(WIN_MSWIN)
  if ( Nlm_currentHDC ) {
    HPEN   xPen   = SelectObject(Nlm_currentHDC, GetStockObject(BLACK_PEN  ));
    HBRUSH xBrush = SelectObject(Nlm_currentHDC, GetStockObject(BLACK_BRUSH));
    int    xMode  = GetROP2(Nlm_currentHDC);
    SetROP2(Nlm_currentHDC, R2_NOTXORPEN);
    Ellipse(Nlm_currentHDC,
            rtool.left, rtool.top, rtool.right+1, rtool.bottom+1);
    if ( xPen )
      SelectObject(Nlm_currentHDC, xPen);
    if ( xBrush )
      SelectObject(Nlm_currentHDC, xBrush);
    SetROP2(Nlm_currentHDC, xMode);
  }

#elif defined(WIN_X)
  if (Nlm_currentXDisplay  &&  Nlm_currentXWindow  &&  Nlm_currentXGC) {
    XSetFunction (Nlm_currentXDisplay, Nlm_currentXGC, GXinvert);
    XSetFillStyle(Nlm_currentXDisplay, Nlm_currentXGC, FillStippled);
    XFillArc(Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
             rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
             rtool.width, rtool.height, 0, 23040);
    XSetFunction (Nlm_currentXDisplay, Nlm_currentXGC, currentFunction);
    XSetFillStyle(Nlm_currentXDisplay, Nlm_currentXGC, currentFillStyle);
  }
#elif defined(WIN_GIF)
#endif
}


static void s_AdjustRoundRect(const Nlm_RecT* r, Nlm_Int2 *w, Nlm_Int2 *h)
{
  Nlm_Int2 w2, h2;

  w2 = r->right - r->left;  w2 = ABS(w2) / 2;
  if (*w > w2)
    *w = w2;

  h2 = r->bottom - r->top;  h2 = ABS(h2) / 2;
  if (*h > h2)
    *h = h2;
}


extern void Nlm_EraseRoundRect(Nlm_RectPtr r, Nlm_Int2 ovlWid, Nlm_Int2 ovlHgt)
{
  Nlm_RectTool rtool;
  if ( !r )
    return;
  Local__RecTToRectTool(r, &rtool);
  s_AdjustRoundRect(r, &ovlWid, &ovlHgt);

#if defined(WIN_MAC)
  EraseRoundRect(&rtool, ovlWid, ovlHgt);

#elif defined(WIN_MSWIN)
  if (Nlm_currentHDC  &&  Nlm_currentHWnd) {
    HPEN   xPen   = SelectObject(Nlm_currentHDC, GetStockObject(NULL_PEN));
    HBRUSH xBrush = SelectObject(Nlm_currentHDC,
                                 GetBackgroundBrush(Nlm_currentHWnd));
    RoundRect(Nlm_currentHDC, rtool.left, rtool.top,
              rtool.right + 2, rtool.bottom + 2, ovlWid, ovlHgt);
    if ( xPen )
      SelectObject(Nlm_currentHDC, xPen);
    if ( xBrush )
      SelectObject(Nlm_currentHDC, xBrush);
  }

#elif defined(WIN_X)
  if (Nlm_currentXDisplay  &&  Nlm_currentXWindow  &&  Nlm_currentXGC) {
    XGCValues values;
    XGetGCValues  (Nlm_currentXDisplay, Nlm_currentXGC, GCFillStyle, &values);
    XSetForeground(Nlm_currentXDisplay, Nlm_currentXGC, currentBkColor);
    XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, FillSolid);
    XmuFillRoundedRectangle(Nlm_currentXDisplay, Nlm_currentXWindow,
                            Nlm_currentXGC,
                            rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
                            rtool.width+1, rtool.height+1, ovlWid, ovlHgt);
    XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, values.fill_style);
    XSetForeground(Nlm_currentXDisplay, Nlm_currentXGC, currentFgColor);
  }

#elif defined(WIN_GIF)
  if ( Nlm_currentGIF ) {
    gdImageRoundRectangle(Nlm_currentGIF,
                          rtool.left, rtool.top, rtool.right, rtool.bottom,
                          ovlWid, ovlHgt, 0, TRUE);
  }
#endif
}


extern void Nlm_FrameRoundRect(Nlm_RectPtr r, Nlm_Int2 ovlWid, Nlm_Int2 ovlHgt)
{
  Nlm_RectTool rtool;
  if ( !r )
    return;
  Local__RecTToRectTool(r, &rtool);
  s_AdjustRoundRect(r, &ovlWid, &ovlHgt);

#if   defined(WIN_MAC)
  FrameRoundRect(&rtool, ovlWid, ovlHgt);

#elif defined(WIN_MSWIN)
  if ( Nlm_currentHDC ) {
    HBRUSH xBrush = SelectObject(Nlm_currentHDC, GetStockObject(NULL_BRUSH));
    RoundRect(Nlm_currentHDC, rtool.left, rtool.top,
              rtool.right + 1, rtool.bottom + 1, ovlWid, ovlHgt);
    if ( xBrush )
      SelectObject(Nlm_currentHDC, xBrush);
  }

#elif defined(WIN_X)
  if (Nlm_currentXDisplay  &&  Nlm_currentXWindow  &&  Nlm_currentXGC) {
    XmuDrawRoundedRectangle(Nlm_currentXDisplay, Nlm_currentXWindow,
                            Nlm_currentXGC,
                            rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
                            rtool.width, rtool.height, ovlWid, ovlHgt);
  }

#elif defined(WIN_GIF)
  if ( Nlm_currentGIF ) {
    gdImageRoundRectangle(Nlm_currentGIF,
                          rtool.left, rtool.top, rtool.right, rtool.bottom,
                          ovlWid, ovlHgt, Nlm_curGIFColor, FALSE);
  }
#endif
}


extern void Nlm_PaintRoundRect(Nlm_RectPtr r, Nlm_Int2 ovlWid, Nlm_Int2 ovlHgt)
{
  Nlm_RectTool rtool;
  if ( !r )
    return;
  Local__RecTToRectTool(r, &rtool);
  s_AdjustRoundRect(r, &ovlWid, &ovlHgt);

#if   defined(WIN_MAC)
  PaintRoundRect(&rtool, ovlWid, ovlHgt);

#elif defined(WIN_MSWIN)
  if ( Nlm_currentHDC ) {
    HPEN xPen = SelectObject(Nlm_currentHDC, GetStockObject(NULL_PEN));
    RoundRect(Nlm_currentHDC, rtool.left, rtool.top,
              rtool.right + 2, rtool.bottom + 2, ovlWid, ovlHgt);
    if ( xPen )
      SelectObject(Nlm_currentHDC, xPen);
  }

#elif defined(WIN_X)
  if (Nlm_currentXDisplay  &&  Nlm_currentXWindow  &&  Nlm_currentXGC) {
    XmuFillRoundedRectangle(Nlm_currentXDisplay, Nlm_currentXWindow,
                            Nlm_currentXGC,
                            rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
                            rtool.width+1, rtool.height+1, ovlWid, ovlHgt);
  }

#elif defined(WIN_GIF)
  if ( Nlm_currentGIF ) {
    gdImageRoundRectangle(Nlm_currentGIF,
                          rtool.left, rtool.top, rtool.right, rtool.bottom,
                          ovlWid, ovlHgt, Nlm_curGIFColor, TRUE);
  }
#endif
}


extern void Nlm_InvertRoundRect(Nlm_RectPtr r,
                                Nlm_Int2 ovlWid, Nlm_Int2 ovlHgt)
{
  Nlm_RectTool rtool;
  if ( !r )
    return;
  Local__RecTToRectTool(r, &rtool);
  s_AdjustRoundRect(r, &ovlWid, &ovlHgt);

#if   defined(WIN_MAC)
  InvertRoundRect (&rtool, ovlWid, ovlHgt);

#elif defined(WIN_MSWIN)
  if ( Nlm_currentHDC ) {
    HPEN   xPen   = SelectObject(Nlm_currentHDC, GetStockObject(BLACK_PEN  ));
    HBRUSH xBrush = SelectObject(Nlm_currentHDC, GetStockObject(BLACK_BRUSH));
    int    xMode  = GetROP2(Nlm_currentHDC);
    SetROP2(Nlm_currentHDC, R2_NOTXORPEN);
    RoundRect(Nlm_currentHDC, rtool.left, rtool.top,
              rtool.right + 2, rtool.bottom + 2, ovlWid, ovlHgt);
    if ( xPen )
      SelectObject(Nlm_currentHDC, xPen);
    if ( xBrush )
      SelectObject(Nlm_currentHDC, xBrush);
    SetROP2(Nlm_currentHDC, xMode);
  }

#elif defined(WIN_X)
  if (Nlm_currentXDisplay  &&  Nlm_currentXWindow  &&  Nlm_currentXGC) {
    XSetFunction  (Nlm_currentXDisplay, Nlm_currentXGC, GXinvert);
    XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, FillStippled);
    XmuFillRoundedRectangle(Nlm_currentXDisplay, Nlm_currentXWindow,
                            Nlm_currentXGC,
                            rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
                            rtool.width+1, rtool.height+1, ovlWid, ovlHgt);
    XSetFunction  (Nlm_currentXDisplay, Nlm_currentXGC, currentFunction);
    XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, currentFillStyle);
  }
#elif defined(WIN_GIF)
#endif
}


#ifdef WIN_X
static int Nlm_PtToAngle (Nlm_RectPtr r, Nlm_PoinT pt)

{
  int     rsult;
  double  val;
  double  x;
  double  y;

  x = pt.x - (r->right + r->left) / 2;
  y = (r->bottom + r->top) / 2 - pt.y;
  if (x == 0) {
    rsult = 5760;
  } else if (y == 0) {
    rsult = 0;
  } else {
    val = atan2 (ABS (y), ABS (x));
    rsult = (int)(val * 11520.0 / 3.14159);
  }
  if (x < 0) {
    if (y < 0) {
      rsult = 11520 + rsult;
    } else {
      rsult = 11520 - rsult;
    }
  } else if (y < 0) {
    rsult = 23040 - rsult;
  }
  return rsult;
}
#endif


extern void Nlm_EraseArc(Nlm_RectPtr r, Nlm_PoinT start, Nlm_PoinT end)
{
  Nlm_RectTool rtool;
  if ( !r )
    return;
  Local__RecTToRectTool(r, &rtool);

#if   defined(WIN_MAC)
  {{
    Nlm_Int2      angle1;
    Nlm_Int2      angle2;
    Nlm_Int2      arcAngle;
    Nlm_PointTool ptool1;
    Nlm_PointTool ptool2;
    Nlm_RectTool  rtool;

    Local__RecTToRectTool(r, &rtool);
    Local__PoinTToPointTool(start, &ptool1);
    Local__PoinTToPointTool(  end, &ptool2);
    PtToAngle(&rtool, ptool1, &angle1);
    PtToAngle(&rtool, ptool2, &angle2);
    if (angle2 > angle1)
      arcAngle = angle2 - angle1;
    else
      arcAngle = 360 - angle1 + angle2;

    EraseArc(&rtool, angle1, arcAngle);
  }}

#elif defined(WIN_MSWIN)
  if (Nlm_currentHDC  &&  Nlm_currentHWnd) {
    HPEN   xPen   = SelectObject(Nlm_currentHDC, GetStockObject(NULL_PEN));
    HBRUSH xBrush = SelectObject(Nlm_currentHDC,
                                 GetBackgroundBrush(Nlm_currentHWnd));
    Pie(Nlm_currentHDC,
        rtool.left, rtool.top, rtool.right + 2, rtool.bottom + 2,
        end.x, end.y, start.x, start.y);
    if ( xPen )
      SelectObject(Nlm_currentHDC, xPen);
    if ( xBrush )
      SelectObject(Nlm_currentHDC, xBrush);
  }

#elif defined(WIN_X)
  if (Nlm_currentXDisplay  &&  Nlm_currentXWindow) {
    XGCValues values;
    int angle1 = Nlm_PtToAngle(r, start);
    int angle2 = Nlm_PtToAngle(r, end);
    int arcAngle;

    if (angle1 > angle2)
      arcAngle = angle1 - angle2;
    else
      arcAngle = 23040 - angle2 + angle1;

    XGetGCValues  (Nlm_currentXDisplay, Nlm_currentXGC, GCFillStyle, &values);
    XSetForeground(Nlm_currentXDisplay, Nlm_currentXGC, currentBkColor);
    XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, FillSolid);
    XFillArc(Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
             rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
             rtool.width, rtool.height, angle1, -arcAngle);
    XSetFillStyle (Nlm_currentXDisplay, Nlm_currentXGC, values.fill_style);
    XSetForeground(Nlm_currentXDisplay, Nlm_currentXGC, currentFgColor);
  }
#elif defined(WIN_GIF)
#endif
}

extern void Nlm_FrameArc(Nlm_RectPtr r, Nlm_PoinT start, Nlm_PoinT end)
{
  Nlm_RectTool rtool;
  if ( !r )
    return;
  Local__RecTToRectTool(r, &rtool);

#if   defined(WIN_MAC)
  {{
    Nlm_Int2      angle1;
    Nlm_Int2      angle2;
    Nlm_Int2      arcAngle;
    Nlm_PointTool ptool1;
    Nlm_PointTool ptool2;

    Local__PoinTToPointTool(start, &ptool1);
    Local__PoinTToPointTool(  end, &ptool2);
    PtToAngle(&rtool, ptool1, &angle1);
    PtToAngle(&rtool, ptool2, &angle2);
    if (angle2 > angle1) {
      arcAngle = angle2 - angle1;
    } else {
      arcAngle = 360 - angle1 + angle2;
    }
    FrameArc (&rtool, angle1, arcAngle);
  }}

#elif defined(WIN_MSWIN)
  if ( Nlm_currentHDC ) {
    HBRUSH xBrush = SelectObject(Nlm_currentHDC, GetStockObject(NULL_BRUSH));
    Arc(Nlm_currentHDC, rtool.left, rtool.top, rtool.right+1, rtool.bottom+1,
        end.x, end.y, start.x, start.y);
    if ( xBrush )
      SelectObject(Nlm_currentHDC, xBrush);
  }

#elif defined(WIN_X)
  if (Nlm_currentXDisplay  &&  Nlm_currentXWindow  &&  Nlm_currentXGC) {
    int angle1 = Nlm_PtToAngle(r, start);
    int angle2 = Nlm_PtToAngle(r, end);
    int arcAngle = angle1 > angle2 ? angle1 - angle2 : 23040 - angle2 + angle1;
    XDrawArc(Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
             rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
             rtool.width - 1, rtool.height - 1, angle1, -arcAngle);
  }

#elif defined(WIN_GIF)
  {{
    int cx = (rtool.left + rtool.right) / 2;
    int cy = (rtool.top + rtool.bottom) / 2;
    double s_angle = atan2(start.x - cx, start.y - cy); 
    double e_angle = atan2(end.x   - cx, end.y   - cy); 
    gdImageArcEx(Nlm_currentGIF, cx, cy,
                 (rtool.right - rtool.left) /2, (rtool.bottom - rtool.top) /2,
                 s_angle, e_angle, Nlm_curGIFColor, FALSE);
  }}
#endif
}

extern void Nlm_PaintArc(Nlm_RectPtr r, Nlm_PoinT start, Nlm_PoinT end)
{
  Nlm_RectTool rtool;
  if ( !r )
    return;
  Local__RecTToRectTool(r, &rtool);

#if   defined(WIN_MAC)
  {{
    Nlm_Int2       angle1;
    Nlm_Int2       angle2;
    Nlm_Int2       arcAngle;
    Nlm_PointTool  ptool1;
    Nlm_PointTool  ptool2;

    Local__PoinTToPointTool(start, &ptool1);
    Local__PoinTToPointTool(end, &ptool2);
    PtToAngle(&rtool, ptool1, &angle1);
    PtToAngle(&rtool, ptool2, &angle2);
    if (angle2 > angle1)
      arcAngle = angle2 - angle1;
    else
      arcAngle = 360 - angle1 + angle2;

    PaintArc(&rtool, angle1, arcAngle);
  }}

#elif defined(WIN_MSWIN)
  if ( Nlm_currentHDC ) {
    HPEN xPen = SelectObject(Nlm_currentHDC, GetStockObject(NULL_PEN));
    Pie(Nlm_currentHDC,
        rtool.left, rtool.top, rtool.right + 2, rtool.bottom + 2,
        end.x, end.y, start.x, start.y);
    if ( xPen )
      SelectObject(Nlm_currentHDC, xPen);
  }

#elif defined(WIN_X)
  if (Nlm_currentXDisplay  &&  Nlm_currentXWindow  &&  Nlm_currentXGC) {
    int angle1 = Nlm_PtToAngle(r, start);
    int angle2 = Nlm_PtToAngle(r, end);
    int arcAngle;

    if (angle1 > angle2)
      arcAngle = angle1 - angle2;
    else
      arcAngle = 23040 - angle2 + angle1;

    XFillArc(Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
             rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
             rtool.width, rtool.height, angle1, -arcAngle);
  }

#elif defined(WIN_GIF)
  {{
    int cx = (rtool.left + rtool.right) / 2;
    int cy = (rtool.top + rtool.bottom) / 2;
    double s_angle = atan2(start.x - cx, start.y - cy); 
    double e_angle = atan2(end.x   - cx, end.y   - cy); 
    gdImageArcEx(Nlm_currentGIF, cx, cy,
                 (rtool.right - rtool.left) /2, (rtool.bottom - rtool.top) /2,
                 s_angle, e_angle, Nlm_curGIFColor, TRUE);
  }}
#endif
}

extern void Nlm_InvertArc(Nlm_RectPtr r, Nlm_PoinT start, Nlm_PoinT end)
{
  Nlm_RectTool rtool;
  if ( !r )
    return;
  Local__RecTToRectTool(r, &rtool);

#if   defined(WIN_MAC)
  {{
    Nlm_Int2      angle1;
    Nlm_Int2      angle2;
    Nlm_Int2      arcAngle;
    Nlm_PointTool ptool1;
    Nlm_PointTool ptool2;
    Nlm_RectTool  rtool;

    Local__RecTToRectTool(r, &rtool);
    Local__PoinTToPointTool(start, &ptool1);
    Local__PoinTToPointTool(end,   &ptool2);
    PtToAngle(&rtool, ptool1, &angle1);
    PtToAngle(&rtool, ptool2, &angle2);
    if (angle2 > angle1)
      arcAngle = angle2 - angle1;
    else
      arcAngle = 360 - angle1 + angle2;

    InvertArc(&rtool, angle1, arcAngle);
  }}

#elif defined(WIN_MSWIN)
  if ( Nlm_currentHDC ) {
    HPEN   xPen   = SelectObject(Nlm_currentHDC, GetStockObject(BLACK_PEN  ));
    HBRUSH xBrush = SelectObject(Nlm_currentHDC, GetStockObject(BLACK_BRUSH));
    int    xMode  = GetROP2(Nlm_currentHDC);
    SetROP2(Nlm_currentHDC, R2_NOTXORPEN);
    Pie(Nlm_currentHDC,
        rtool.left, rtool.top, rtool.right + 2, rtool.bottom + 2,
        end.x, end.y, start.x, start.y);
    if ( xPen )
      SelectObject(Nlm_currentHDC, xPen);
    if ( xBrush )
      SelectObject(Nlm_currentHDC, xBrush);
    SetROP2(Nlm_currentHDC, xMode);
  }

#elif defined(WIN_X)
  if (Nlm_currentXDisplay  &&  Nlm_currentXWindow  &&  Nlm_currentXGC) {
    int angle1 = Nlm_PtToAngle(r, start);
    int angle2 = Nlm_PtToAngle(r, end);
    int arcAngle;

    if (angle1 > angle2)
      arcAngle = angle1 - angle2;
    else
      arcAngle = 23040 - angle2 + angle1;

    XSetFunction (Nlm_currentXDisplay, Nlm_currentXGC, GXinvert);
    XSetFillStyle(Nlm_currentXDisplay, Nlm_currentXGC, FillStippled);
    XFillArc(Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
             rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
             rtool.width, rtool.height, angle1, -arcAngle);
    XSetFunction (Nlm_currentXDisplay, Nlm_currentXGC, currentFunction);
    XSetFillStyle(Nlm_currentXDisplay, Nlm_currentXGC, currentFillStyle);
  }
#elif defined(WIN_GIF)
#endif
}

typedef enum {
  eDM_Erase = 0,
  eDM_Frame,
  eDM_Paint,
  eDM_Invert
} EDrawMethod;


/*********************************
 *  QUADRANT
 */

static void s_DoQuadrant(Nlm_RectPtr r, Nlm_EQuadrant quadrant,
                         EDrawMethod method)
{
  Nlm_RecT rr;
  Nlm_Int2 rx, ry;

  Nlm_LoadNormalized(&rr, r);
  rx = rr.right  - rr.left;
  ry = rr.bottom - rr.top;
  
#ifdef WIN_GIF
  {{
    int cx = 0, cy = 0;

    switch ( quadrant ) {
    case eQ_RightTop:
      cx = rr.left;   cy = rr.bottom;  break;
    case eQ_LeftTop:
      cx = rr.right;  cy = rr.bottom;  break;
    case eQ_LeftBottom:
      cx = rr.right;  cy = rr.top;     break;
    case eQ_RightBottom:
      cx = rr.left;   cy = rr.top;     break;
    }

    gdImageQuadrant(Nlm_currentGIF, cx, cy, rx, ry,
                    (method == eDM_Erase) ? 0 : Nlm_curGIFColor,
                    (method != eDM_Frame), (int)quadrant);
  }}
#else
  {{
    Nlm_PoinT start, stop;
    switch ( quadrant ) {
    case eQ_RightTop:
      start.x = rr.left;   start.y = rr.top;
      stop.x  = rr.right;  stop.y  = rr.bottom;
      rr.left -= rx;  rr.bottom += ry;
      break;
    case eQ_LeftTop:
      start.x = rr.left;   start.y  = rr.bottom;
      stop.x  = rr.right;  stop.y = rr.top;
      rr.right += rx;  rr.bottom += ry;
      break;
    case eQ_LeftBottom:
      start.x = rr.right;  start.y = rr.bottom;
      stop.x  = rr.left;   stop.y  = rr.top;
      rr.right += rx;  rr.top -= ry;
      break;
    case eQ_RightBottom:
      start.x = rr.right;  start.y = rr.top;
      stop.x  = rr.left;   stop.y  = rr.bottom;
      rr.left -= rx;  rr.top -= ry;
      break;
    }

    switch ( method ) {
    case eDM_Erase:
      Nlm_EraseArc(&rr, start, stop);  break;
    case eDM_Frame:
      Nlm_FrameArc(&rr, start, stop);  break;
    case eDM_Paint:
      Nlm_PaintArc(&rr, start, stop);  break;
    case eDM_Invert:
      Nlm_InvertArc(&rr, start, stop); break;
    }
  }}
#endif
}

void Nlm_EraseQuadrant(Nlm_RectPtr r, Nlm_EQuadrant quadrant) {
  s_DoQuadrant(r, quadrant, eDM_Erase);
}
void Nlm_FrameQuadrant(Nlm_RectPtr r, Nlm_EQuadrant quadrant) {
  s_DoQuadrant(r, quadrant, eDM_Frame);
}
void Nlm_PaintQuadrant(Nlm_RectPtr r, Nlm_EQuadrant quadrant) {
  s_DoQuadrant(r, quadrant, eDM_Paint);
}
void Nlm_InvertQuadrant(Nlm_RectPtr r, Nlm_EQuadrant quadrant) {
  s_DoQuadrant(r, quadrant, eDM_Invert);
}


/*********************************
 *  POLYGON
 */

#ifdef WIN_MAC
static PolyHandle Nlm_CreatePoly (Nlm_Int2 num, Nlm_PointPtr pts)

{
  Nlm_PoinT   firstPt;
  Nlm_Int2    i;
  Nlm_PoinT   pt;
  PolyHandle  rsult;

  rsult = NULL;
  if (pts != NULL && num > 0) {
    rsult = OpenPoly ();
    firstPt = pts [0];
    MoveTo (firstPt.x, firstPt.y);
    for (i = 1; i < num; i++) {
      pt = pts [i];
      LineTo (pt.x, pt.y);
    }
    if (! Nlm_EqualPt (pt, firstPt)) {
      LineTo (firstPt.x, firstPt.y);
    }
    ClosePoly ();
  }
  return rsult;
}

static void Nlm_DestroyPoly (PolyHandle ply)

{
  if (ply != NULL) {
    KillPoly (ply);
  }
}
#endif

#ifdef WIN_MSWIN
static LPPOINT Nlm_CreatePoly (Nlm_Int2 num, Nlm_PointPtr pts)

{
  Nlm_PoinT      firstPt;
  Nlm_Int2       i;
  Nlm_PoinT      pt;
  Nlm_PointTool  ptool;
  LPPOINT        rsult;

  rsult = NULL;
  if (pts != NULL && num > 0) {
    rsult = (LPPOINT) Nlm_MemNew ((size_t) ((num + 1) * sizeof (POINT)));
    if (rsult != NULL) {
      firstPt = pts [0];
      for (i = 0; i < num; i++) {
        pt = pts [i];
        Local__PoinTToPointTool (pt, &ptool);
        rsult [i] = ptool;
      }
      if (! Nlm_EqualPt (pt, firstPt)) {
        Local__PoinTToPointTool (firstPt, &ptool);
        rsult [i] = ptool;
      }
    }
  }
  return rsult;
}

static void Nlm_DestroyPoly (LPPOINT ply)

{
  if (ply != NULL) {
    Nlm_MemFree (ply);
  }
}
#endif

#ifdef WIN_X
static XPoint *Nlm_CreatePoly (Nlm_Int2 num, Nlm_PointPtr pts)

{
  Nlm_PoinT      firstPt;
  Nlm_Int2       i;
  Nlm_PoinT      pt;
  Nlm_PointTool  ptool;
  XPoint         *rsult;

  rsult = NULL;
  if (pts != NULL && num > 0) {
    rsult = (XPoint *) Nlm_MemNew ((size_t) ((num + 1) * sizeof (XPoint)));
    if (rsult != NULL) {
      firstPt = pts [0];
      firstPt.x -= Nlm_XOffset;
      firstPt.y -= Nlm_YOffset;
      for (i = 0; i < num; i++) {
        pt = pts [i];
        pt.x -= Nlm_XOffset;
        pt.y -= Nlm_YOffset;
        Local__PoinTToPointTool (pt, &ptool);
        rsult [i] = ptool;
      }
      if (! Nlm_EqualPt (pt, firstPt)) {
        Local__PoinTToPointTool (firstPt, &ptool);
        rsult [i] = ptool;
      }
    }
  }
  return rsult;
}

static void Nlm_DestroyPoly (XPoint *ply)

{
  if (ply != NULL) {
    Nlm_MemFree (ply);
  }
}
#endif

extern void Nlm_ErasePoly (Nlm_Int2 num, Nlm_PointPtr pts)

{
  if (pts != NULL && num > 1) {
#ifdef WIN_MAC
  PolyHandle   ply;

  ply = Nlm_CreatePoly (num, pts);
  if (ply != NULL) {
    ErasePoly (ply);
  }
  Nlm_DestroyPoly (ply);
#endif
#ifdef WIN_MSWIN
#endif
#ifdef WIN_X
#endif
#ifdef WIN_GIF
#endif
  }
}


extern void Nlm_FramePoly(Nlm_Int2 num, Nlm_PointPtr pts)
{
  if (pts != NULL && num > 1) {
#ifdef WIN_MAC
  PolyHandle   ply;

  ply = Nlm_CreatePoly (num, pts);
  if (ply != NULL) {
    FramePoly (ply);
  }
  Nlm_DestroyPoly (ply);
#endif
#ifdef WIN_MSWIN
  LPPOINT  ply;

  ply = Nlm_CreatePoly (num, pts);
  if (Nlm_currentHDC != NULL && ply != NULL) {
    if (! Nlm_EqualPt (pts [0], pts [num - 1])) {
      num++;
    }
    Polyline (Nlm_currentHDC, ply, num);
  }
  Nlm_DestroyPoly (ply);
#endif
#ifdef WIN_X
  XPoint  *ply;

  ply = Nlm_CreatePoly (num, pts);
  if (Nlm_currentXDisplay != NULL && Nlm_currentXWindow != 0 &&
      Nlm_currentXGC != NULL && ply != NULL) {
    if (! Nlm_EqualPt (pts [0], pts [num - 1])) {
      num++;
    }
    XDrawLines (Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
                ply, num, CoordModeOrigin);
  }
  Nlm_DestroyPoly (ply);
#endif
#ifdef WIN_GIF
  gdPointPtr pPtr;
  int        i;

  if (Nlm_currentGIF != NULL && pts != NULL) {
    pPtr = (gdPointPtr)Nlm_MemNew(num * sizeof(gdPoint));
    for ( i=0; i<num; i++ ){
      pPtr[i].x = pts[i].x;
      pPtr[i].y = pts[i].y;
    }
    gdImagePolygon ( Nlm_currentGIF,  pPtr, num, Nlm_curGIFColor );
    MemFree (pPtr);
  }
#endif
  }
}

extern void Nlm_PaintPoly (Nlm_Int2 num, Nlm_PointPtr pts)

{
  if (pts != NULL && num > 1) {
#ifdef WIN_MAC
  PolyHandle   ply;

  ply = Nlm_CreatePoly (num, pts);
  if (ply != NULL) {
    PaintPoly (ply);
  }
  Nlm_DestroyPoly (ply);
#endif
#ifdef WIN_MSWIN
  LPPOINT  ply;

  ply = Nlm_CreatePoly (num, pts);
  if (Nlm_currentHDC != NULL && ply != NULL) {
    if (! Nlm_EqualPt (pts [0], pts [num - 1])) {
      num++;
    }
    SetPolyFillMode (Nlm_currentHDC, ALTERNATE);
    Polygon (Nlm_currentHDC, ply, num);
  }
  Nlm_DestroyPoly (ply);
#endif
#ifdef WIN_X
  XPoint  *ply = Nlm_CreatePoly (num, pts);
  if (Nlm_currentXDisplay != NULL && Nlm_currentXWindow != 0 &&
      Nlm_currentXGC != NULL && ply != NULL)
    {
      int i;
      short left=32767, top=32767; 
      for (i = 0;  i < num;  i++)
        {
          if (ply[i].x < left)
            left = ply[i].x;
          if (ply[i].y < top)
            top  = ply[i].y;
        }
      for (i = 0;  i < num;  i++)
        {
          if (ply[i].x != left)
            ply[i].x++;
          if (ply[i].y != top)
            ply[i].y++;
        }

      XSetFillRule (Nlm_currentXDisplay, Nlm_currentXGC, EvenOddRule);
      XFillPolygon (Nlm_currentXDisplay, Nlm_currentXWindow, Nlm_currentXGC,
                    ply, num, Complex, CoordModeOrigin);
    }
  Nlm_DestroyPoly (ply);
#endif
#ifdef WIN_GIF
  gdPointPtr pPtr;
  int        i;

  if (Nlm_currentGIF != NULL && pts != NULL) {
    pPtr = (gdPointPtr)Nlm_MemNew(num * sizeof(gdPoint));
    for ( i=0; i<num; i++ ){
      pPtr[i].x = pts[i].x;
      pPtr[i].y = pts[i].y;
    }
    gdImageFilledPolygon ( Nlm_currentGIF,  pPtr, num, Nlm_curGIFColor );
    MemFree (pPtr);
  }
#endif
  }
}

extern void Nlm_InvertPoly (Nlm_Int2 num, Nlm_PointPtr pts)

{
  if (pts != NULL && num > 1) {
#ifdef WIN_MAC
  PolyHandle   ply;

  ply = Nlm_CreatePoly (num, pts);
  if (ply != NULL) {
    InvertPoly (ply);
  }
  Nlm_DestroyPoly (ply);
#endif
#ifdef WIN_MSWIN
#endif
#ifdef WIN_X
#endif
#ifdef WIN_GIF
#endif
  }
}

extern Nlm_RegioN Nlm_CreateRgn (void)

{
  Nlm_RgnTool  ntool;

#ifdef WIN_MAC
  ntool = NewRgn ();
#endif
#ifdef WIN_MSWIN
  ntool = CreateRectRgn (0, 0, 0, 0);
#endif
#ifdef WIN_X
  ntool = XCreateRegion ();
#endif
#ifdef WIN_GIF
  ntool = HandNew ( sizeof(Nlm_RecT) );
#endif
  return (Nlm_RegioN) ntool;
}

extern Nlm_RegioN Nlm_DestroyRgn (Nlm_RegioN rgn)

{
  Nlm_RgnTool  ntool;

  if (rgn != NULL) {
    ntool = (Nlm_RgnTool) rgn;
#ifdef WIN_MAC
    DisposeRgn (ntool);
#endif
#ifdef WIN_MSWIN
    DeleteObject (ntool);
#endif
#ifdef WIN_X
    XDestroyRegion (ntool);
#endif
#ifdef WIN_GIF
    HandFree ( ntool );
#endif
  }
  return NULL;
}

extern void Nlm_ClearRgn (Nlm_RegioN rgn)

{
  Nlm_RgnTool  ntool;
#ifdef WIN_MSWIN
  Nlm_RgnTool  temp;
#endif

  if (rgn != NULL) {
    ntool = (Nlm_RgnTool) rgn;
#ifdef WIN_MAC
    SetEmptyRgn (ntool);
#endif
#ifdef WIN_MSWIN
    temp = CreateRectRgn (0, 0, 0, 0);
    CombineRgn (ntool, temp, temp, RGN_COPY);
    DeleteObject (temp);
#endif
#ifdef WIN_X
    XUnionRegion (emptyRgn, emptyRgn, ntool);
#endif
  } 
}

extern void Nlm_LoadRectRgn (Nlm_RegioN rgn, Nlm_Int2 lf,
                             Nlm_Int2 tp, Nlm_Int2 rt,
                             Nlm_Int2 bt)

{
  Nlm_RgnTool   ntool;
#ifdef WIN_X
  Nlm_RecT      rct;
  Nlm_RectTool  rtool;
#endif
#ifdef WIN_GIF
  Nlm_RecT      rct;
#endif

  if (rgn != NULL) {
    ntool = (Nlm_RgnTool) rgn;
#ifdef WIN_MAC
    SetRectRgn (ntool, lf, tp, rt, bt);
#endif
#ifdef WIN_MSWIN
    SetRectRgn (ntool, lf, tp, rt, bt);
#endif
#ifdef WIN_X
    Nlm_LoadRect (&rct, lf, tp, rt, bt);
    Local__RecTToRectTool (&rct, &rtool);
    XUnionRectWithRegion (&rtool, emptyRgn, ntool);
#endif
#ifdef WIN_GIF
    Nlm_LoadRect (&rct, lf, tp, rt, bt);
    Nlm_RectToGIFRgn ( &rct, ntool );
#endif
  }
}

extern void Nlm_OffsetRgn (Nlm_RegioN rgn, Nlm_Int2 dx, Nlm_Int2 dy)

{
  Nlm_RgnTool  ntool;
#ifdef WIN_GIF
  Nlm_RecT rTool;
#endif

  if (rgn != NULL) {
    ntool = (Nlm_RgnTool) rgn;
#ifdef WIN_MAC
    OffsetRgn (ntool, dx, dy);
#endif
#ifdef WIN_MSWIN
    OffsetRgn (ntool, dx, dy);
#endif
#ifdef WIN_X
    XOffsetRegion (ntool, dx, dy);
#endif
#ifdef WIN_GIF
    Nlm_GIFRgnToRect ( ntool, &rTool );
    Nlm_OffsetRect ( &rTool, dx, dy );
    Nlm_RectToGIFRgn ( &rTool, ntool );
#endif
  }
}

extern void Nlm_SectRgn (Nlm_RegioN src1, Nlm_RegioN src2, Nlm_RegioN dst)

{
  Nlm_RgnTool  ntool1;
  Nlm_RgnTool  ntool2;
  Nlm_RgnTool  ntool3;
#ifdef WIN_X
  Nlm_RgnTool  temp;
#endif
#ifdef WIN_GIF
  Nlm_RecT     rsrc1;
  Nlm_RecT     rsrc2;
  Nlm_RecT     rdst;
#endif

  if (src1 != NULL && src2 != NULL && dst != NULL) {
    ntool1 = (Nlm_RgnTool) src1;
    ntool2 = (Nlm_RgnTool) src2;
    ntool3 = (Nlm_RgnTool) dst;
#ifdef WIN_MAC
    SectRgn (ntool1, ntool2, ntool3);
#endif
#ifdef WIN_MSWIN
    CombineRgn (ntool3, ntool1, ntool2, RGN_AND);
#endif
#ifdef WIN_X
    temp = XCreateRegion ();
    XIntersectRegion (ntool1, ntool2, temp);
    XUnionRegion (temp, emptyRgn, ntool3);
    XDestroyRegion (temp);
#endif
#ifdef WIN_GIF
    Nlm_GIFRgnToRect ( ntool1, &rsrc1 );
    Nlm_GIFRgnToRect ( ntool2, &rsrc2 );
    Nlm_SectRect ( &rsrc1, &rsrc2, &rdst );
    Nlm_RectToGIFRgn ( &rdst, ntool3 );
#endif
  }
}

extern void Nlm_UnionRgn (Nlm_RegioN src1, Nlm_RegioN src2, Nlm_RegioN dst)

{
  Nlm_RgnTool  ntool1;
  Nlm_RgnTool  ntool2;
  Nlm_RgnTool  ntool3;
#ifdef WIN_X
  Nlm_RgnTool  temp;
#endif
#ifdef WIN_GIF
  Nlm_RecT     rsrc1;
  Nlm_RecT     rsrc2;
  Nlm_RecT     rdst;
#endif

  if (src1 != NULL && src2 != NULL && dst != NULL) {
    ntool1 = (Nlm_RgnTool) src1;
    ntool2 = (Nlm_RgnTool) src2;
    ntool3 = (Nlm_RgnTool) dst;
#ifdef WIN_MAC
    UnionRgn (ntool1, ntool2, ntool3);
#endif
#ifdef WIN_MSWIN
    CombineRgn (ntool3, ntool1, ntool2, RGN_OR);
#endif
#ifdef WIN_X
    temp = XCreateRegion ();
    XUnionRegion (ntool1, ntool2, temp);
    XUnionRegion (temp, emptyRgn, ntool3);
    XDestroyRegion (temp);
#endif
#ifdef WIN_GIF
    Nlm_GIFRgnToRect ( ntool1, &rsrc1 );
    Nlm_GIFRgnToRect ( ntool2, &rsrc2 );
    Nlm_UnionRect ( &rsrc1, &rsrc2, &rdst );
    Nlm_RectToGIFRgn ( &rdst, ntool3 );
#endif
  }
}

extern void Nlm_DiffRgn (Nlm_RegioN src1, Nlm_RegioN src2, Nlm_RegioN dst)

{
  Nlm_RgnTool  ntool1;
  Nlm_RgnTool  ntool2;
  Nlm_RgnTool  ntool3;
#ifdef WIN_X
  Nlm_RgnTool  temp;
#endif

  if (src1 != NULL && src2 != NULL && dst != NULL) {
    ntool1 = (Nlm_RgnTool) src1;
    ntool2 = (Nlm_RgnTool) src2;
    ntool3 = (Nlm_RgnTool) dst;
#ifdef WIN_MAC
    DiffRgn (ntool1, ntool2, ntool3);
#endif
#ifdef WIN_MSWIN
    CombineRgn (ntool3, ntool1, ntool2, RGN_DIFF);
#endif
#ifdef WIN_X
    temp = XCreateRegion ();
    XSubtractRegion (ntool1, ntool2, temp);
    XUnionRegion (temp, emptyRgn, ntool3);
    XDestroyRegion (temp);
#endif
  }
}

extern void Nlm_XorRgn (Nlm_RegioN src1, Nlm_RegioN src2, Nlm_RegioN dst)

{
  Nlm_RgnTool  ntool1;
  Nlm_RgnTool  ntool2;
  Nlm_RgnTool  ntool3;
#ifdef WIN_X
  Nlm_RgnTool  temp;
#endif

  if (src1 != NULL && src2 != NULL && dst != NULL) {
    ntool1 = (Nlm_RgnTool) src1;
    ntool2 = (Nlm_RgnTool) src2;
    ntool3 = (Nlm_RgnTool) dst;
#ifdef WIN_MAC
    XorRgn (ntool1, ntool2, ntool3);
#endif
#ifdef WIN_MSWIN
    CombineRgn (ntool3, ntool1, ntool2, RGN_XOR);
#endif
#ifdef WIN_X
    temp = XCreateRegion ();
    XXorRegion (ntool1, ntool2, temp);
    XUnionRegion (temp, emptyRgn, ntool3);
    XDestroyRegion (temp);
#endif
  }
}

extern Nlm_Boolean Nlm_EqualRgn (Nlm_RegioN rgn1, Nlm_RegioN rgn2)

{
  Nlm_RgnTool  ntool1;
  Nlm_RgnTool  ntool2;
  Nlm_Boolean  rsult;

  rsult = FALSE;
  if (rgn1 != NULL && rgn2 != NULL) {
    ntool1 = (Nlm_RgnTool) rgn1;
    ntool2 = (Nlm_RgnTool) rgn2;
#ifdef WIN_MAC
    rsult = EqualRgn (ntool1, ntool2);
#endif
#ifdef WIN_MSWIN
    rsult = (Nlm_Boolean) EqualRgn (ntool1, ntool2);
#endif
#ifdef WIN_X
    rsult = (XEqualRegion (ntool1, ntool2) != 0);
#endif
  }
  return rsult;
}

extern Nlm_Boolean Nlm_EmptyRgn (Nlm_RegioN rgn)

{
  Nlm_RgnTool   ntool;
  Nlm_Boolean   rsult;
#ifdef WIN_MSWIN
  Nlm_RectTool  rtool;
#endif

  rsult = FALSE;
  if (rgn != NULL) {
    ntool = (Nlm_RgnTool) rgn;
#ifdef WIN_MAC
    rsult = EmptyRgn (ntool);
#endif
#ifdef WIN_MSWIN
    rsult = (Nlm_Boolean) (GetRgnBox (ntool, &rtool) == NULLREGION);
#endif
#ifdef WIN_X
    rsult = (XEmptyRegion (ntool) != 0);
#endif
  }
  return rsult;
}

extern void Nlm_EraseRgn (Nlm_RegioN rgn)

{
#ifdef WIN_MAC
  Nlm_RgnTool  ntool;

  if (rgn != NULL) {
    ntool = (Nlm_RgnTool) rgn;
    EraseRgn (ntool);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_RgnTool  ntool;

  if (rgn != NULL && Nlm_currentHDC != NULL && Nlm_currentHWnd != NULL) {
    ntool = (Nlm_RgnTool) rgn;
    FillRgn (Nlm_currentHDC, ntool, GetBackgroundBrush (Nlm_currentHWnd));
  }
#endif
#ifdef WIN_X
#endif
}

extern void Nlm_FrameRgn (Nlm_RegioN rgn)

{
#ifdef WIN_MAC
  Nlm_RgnTool  ntool;

  if (rgn != NULL) {
    ntool = (Nlm_RgnTool) rgn;
    FrameRgn (ntool);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_RgnTool  ntool;

  if (rgn != NULL && Nlm_currentHDC != NULL) {
    ntool = (Nlm_RgnTool) rgn;
    FrameRgn (Nlm_currentHDC, ntool, GetStockObject (BLACK_BRUSH), 1, 1);
  }
#endif
#ifdef WIN_X
#endif
}

extern void Nlm_PaintRgn (Nlm_RegioN rgn)

{
#ifdef WIN_MAC
  Nlm_RgnTool  ntool;

  if (rgn != NULL) {
    ntool = (Nlm_RgnTool) rgn;
    PaintRgn (ntool);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_RgnTool  ntool;
  HBRUSH       oldBrush;

  if (rgn != NULL && Nlm_currentHDC != NULL) {
    ntool = (Nlm_RgnTool) rgn;
    oldBrush = SelectObject (Nlm_currentHDC, GetStockObject (BLACK_BRUSH));
    if (oldBrush != NULL) {
      SelectObject (Nlm_currentHDC, oldBrush);
      FillRgn (Nlm_currentHDC, ntool, oldBrush);
    }
  }
#endif
#ifdef WIN_X
#endif
}

extern void Nlm_InvertRgn (Nlm_RegioN rgn)

{
#ifdef WIN_MAC
  Nlm_RgnTool  ntool;

  if (rgn != NULL) {
    ntool = (Nlm_RgnTool) rgn;
    InvertRgn (ntool);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_RgnTool  ntool;

  if (rgn != NULL && Nlm_currentHDC != NULL) {
    ntool = (Nlm_RgnTool) rgn;
    InvertRgn (Nlm_currentHDC, ntool);
  }
#endif
#ifdef WIN_X
#endif
}

extern void Nlm_ClipRect (Nlm_RectPtr r)

{
#ifdef WIN_MAC
  Nlm_RectTool  rtool;

  if (r != NULL) {
    Local__RecTToRectTool (r, &rtool);
    ClipRect (&rtool);
  }
#endif
#ifdef WIN_MSWIN
  HRGN  hRgnClip;

  if (r != NULL && Nlm_currentHDC != NULL) {
    hRgnClip = CreateRectRgn (r->left, r->top, r->right, r->bottom);
    SelectClipRgn (Nlm_currentHDC, hRgnClip);
    DeleteObject (hRgnClip);
  }
#endif
#ifdef WIN_X
  Nlm_RectTool  rtool;

  if (r != NULL && Nlm_currentXDisplay != NULL && Nlm_currentXGC != NULL) {
    Local__RecTToRectTool (r, &rtool);
    rtool.x -= Nlm_XOffset;
    rtool.y -= Nlm_YOffset;
    XSetClipRectangles (Nlm_currentXDisplay, Nlm_currentXGC, 0, 0, &rtool, 1, Unsorted);
    if (Nlm_clpRgn != NULL) {
      XDestroyRegion ((Nlm_RgnTool) Nlm_clpRgn);
      Nlm_clpRgn = NULL;
    }
    Nlm_clpRgn = (Nlm_RegioN) XCreateRegion ();
    XUnionRectWithRegion (&rtool, (Nlm_RgnTool) Nlm_clpRgn, (Nlm_RgnTool) Nlm_clpRgn);
    XOffsetRegion ((Nlm_RgnTool) Nlm_clpRgn, Nlm_XOffset, Nlm_YOffset);
  }
#endif
}

extern void Nlm_ClipRgn (Nlm_RegioN rgn)

{
#ifdef WIN_MAC
  Nlm_RgnTool  ntool;

  if (rgn != NULL) {
    ntool = (Nlm_RgnTool) rgn;
    SetClip (ntool);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_RgnTool  ntool;

  if (rgn != NULL && Nlm_currentHDC != NULL) {
    ntool = (Nlm_RgnTool) rgn;
    SelectClipRgn (Nlm_currentHDC, ntool);
  }
#endif
#ifdef WIN_X
  Nlm_RgnTool  ntool1;
  Nlm_RgnTool  ntool2;

  if (rgn != NULL && Nlm_currentXDisplay != NULL && Nlm_currentXGC != NULL) {
    ntool1 = XCreateRegion ();
    ntool2 = XCreateRegion ();
    XUnionRegion ((Nlm_RgnTool) rgn, ntool1, ntool2);
    XOffsetRegion (ntool2, -Nlm_XOffset, -Nlm_YOffset);
    XSetRegion (Nlm_currentXDisplay, Nlm_currentXGC, ntool2);
    if (Nlm_clpRgn != NULL) {
      XDestroyRegion ((Nlm_RgnTool) Nlm_clpRgn);
      Nlm_clpRgn = NULL;
    }
    Nlm_clpRgn = (Nlm_RegioN) XCreateRegion ();
    XUnionRegion ((Nlm_RgnTool) rgn, ntool1, (Nlm_RgnTool) Nlm_clpRgn);
    XOffsetRegion ((Nlm_RgnTool) Nlm_clpRgn, -Nlm_XOffset, -Nlm_YOffset);
    XDestroyRegion (ntool1);
    XDestroyRegion (ntool2);
  }
#endif
}

extern void Nlm_ResetClip (void)

{
#ifdef WIN_MAC
  Nlm_RecT      r;
  Nlm_RectTool  rtool;

  Nlm_LoadRect (&r, -32767, -32767, 32767, 32767);
  Local__RecTToRectTool (&r, &rtool);
  ClipRect (&rtool);
#endif
#ifdef WIN_MSWIN
  if (Nlm_currentHDC != NULL) {
    SelectClipRgn (Nlm_currentHDC, NULL);
  }
#endif
#ifdef WIN_X
  if (Nlm_currentXDisplay != NULL && Nlm_currentXGC != NULL) {
    XSetClipMask (Nlm_currentXDisplay, Nlm_currentXGC, None);
    if (Nlm_clpRgn != NULL) {
      XDestroyRegion ((Nlm_RgnTool) Nlm_clpRgn);
      Nlm_clpRgn = NULL;
    }
  }
#endif
}

extern void Nlm_ValidRect (Nlm_RectPtr r)

{
#ifdef WIN_MAC
  Nlm_RectTool  rtool;

  if (r != NULL) {
    Local__RecTToRectTool (r, &rtool);
    ValidRect (&rtool);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_RectTool  rtool;

  if (r != NULL && Nlm_currentHWnd != NULL) {
    Local__RecTToRectTool (r, &rtool);
    ValidateRect (Nlm_currentHWnd, &rtool);
  }
#endif
#ifdef WIN_X
#endif
}

extern void Nlm_InvalRect (Nlm_RectPtr r)

{
#ifdef WIN_MAC
  Nlm_RectTool  rtool;

  if (r != NULL) {
    Local__RecTToRectTool (r, &rtool);
    InvalRect (&rtool);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_RectTool  rtool;

  if (r != NULL && Nlm_currentHWnd != NULL) {
    Local__RecTToRectTool (r, &rtool);
    InvalidateRect (Nlm_currentHWnd, &rtool, TRUE);
  }
#endif
#ifdef WIN_X
  Nlm_RectTool  rtool;

  if (r != NULL && Nlm_currentXDisplay != NULL && Nlm_currentXWindow != 0) {
    Local__RecTToRectTool (r, &rtool);
    XClearArea (Nlm_currentXDisplay, Nlm_currentXWindow,
                rtool.x - Nlm_XOffset, rtool.y - Nlm_YOffset,
                rtool.width, rtool.height, TRUE);
  }
#endif
}

extern void Nlm_ValidRgn (Nlm_RegioN rgn)

{
#ifdef WIN_MAC
  if (rgn != NULL) {
    ValidRgn ((Nlm_RgnTool) rgn);
  }
#endif
#ifdef WIN_MSWIN
  if (rgn != NULL && Nlm_currentHWnd != NULL) {
    ValidateRgn (Nlm_currentHWnd, (Nlm_RgnTool) rgn);
  }
#endif
#ifdef WIN_X
#endif
}

extern void Nlm_InvalRgn (Nlm_RegioN rgn)

{
#ifdef WIN_MAC
  if (rgn != NULL) {
    InvalRgn ((Nlm_RgnTool) rgn);
  }
#endif
#ifdef WIN_MSWIN
  if (rgn != NULL && Nlm_currentHWnd != NULL) {
    InvalidateRgn (Nlm_currentHWnd, (Nlm_RgnTool) rgn, TRUE);
  }
#endif
#ifdef WIN_X
  Nlm_RgnTool   ntool1;
  Nlm_RgnTool   ntool2;
  Nlm_RectTool  rtool;

  if (rgn != NULL && Nlm_currentXDisplay != NULL &&
      Nlm_currentXGC != NULL && Nlm_currentXWindow != 0) {
    ntool1 = XCreateRegion ();
    ntool2 = XCreateRegion ();
    XUnionRegion ((Nlm_RgnTool) rgn, ntool1, ntool2);
    XOffsetRegion (ntool2, -Nlm_XOffset, -Nlm_YOffset);
    XSetRegion (Nlm_currentXDisplay, Nlm_currentXGC, ntool2);
    XClipBox (ntool2, &rtool);
    XClearArea (Nlm_currentXDisplay, Nlm_currentXWindow, rtool.x,
                rtool.y, rtool.width, rtool.height, TRUE);
    XDestroyRegion (ntool1);
    XDestroyRegion (ntool2);
    if (Nlm_clpRgn != NULL) {
      ntool1 = XCreateRegion ();
      ntool2 = XCreateRegion ();
      XUnionRegion ((Nlm_RgnTool) Nlm_clpRgn, ntool1, ntool2);
      XOffsetRegion (ntool2, -Nlm_XOffset, -Nlm_YOffset);
      XSetRegion (Nlm_currentXDisplay, Nlm_currentXGC, ntool2);
      XDestroyRegion (ntool1);
      XDestroyRegion (ntool2);
    } else {
      XSetClipMask (Nlm_currentXDisplay, Nlm_currentXGC, None);
    }
  }
#endif
}

extern void Nlm_CopyBits (Nlm_RectPtr r, Nlm_VoidPtr source)

{
#ifdef WIN_MAC
  const BitMap *dstBitsPtr;
  Nlm_Int2  mode;
  PenState  pnState;
  GrafPtr   port;
  Rect      rect;
  BitMap    srcBits;
  Rect      srcRect;

  if (r != NULL && source != NULL) {
    GetPort (&port);
    GetPenState (&pnState);
    switch (pnState.pnMode) {
      case patCopy:
        mode = srcCopy;
        break;
      case patOr:
        mode = srcOr;
        break;
      case patXor:
        mode = srcXor;
        break;
      case patBic:
        mode = srcBic;
        break;
      default:
        mode = srcCopy;
        break;
    }
    Local__RecTToRectTool (r, &rect);
    srcRect = rect;
    OffsetRect (&srcRect, -rect.left, -rect.top);
    srcBits.baseAddr = (Ptr) source;
    srcBits.rowBytes = (rect.right - rect.left - 1) / 8 + 1;
    srcBits.bounds = srcRect;
#if OPAQUE_TOOLBOX_STRUCTS
    dstBitsPtr = GetPortBitMapForCopyBits(port);
#else
    dstBitsPtr = &port->portBits;
#endif
    CopyBits (&srcBits, dstBitsPtr, &srcRect, &rect, mode, NULL);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_Int2      cols;
  HBITMAP       hBitmap;
  HBITMAP       hOldBitmap;
  HDC           hMemoryDC;
  int           rop2;
  Nlm_Int2      i;
  Nlm_Int2      j;
  Nlm_Int4      mode;
  Nlm_Int4      num;
  Nlm_Boolean   odd;
  Nlm_Uint1Ptr  p;
  Nlm_Uint1Ptr  ptr;
  Nlm_Uint1Ptr  q;
  Nlm_Int2      rows;

  if (r != NULL && source != NULL && Nlm_currentHDC != NULL) {
    rows = (Nlm_Int2)((r->right - r->left - 1) / 8 + 1);
    odd  = (Nlm_Boolean) ((rows & 1) != 0);
    cols = (Nlm_Int2)(r->bottom - r->top);
    num = rows * cols;
    if (odd) {
      num += cols;
    }
    ptr = (Nlm_Uint1Ptr) Nlm_MemNew ((size_t) num * sizeof (Nlm_Uint1));
    if (ptr != NULL) {
      p = source;
      q = ptr;
      for (i = 0; i < cols; i++) {
        j = 0;
        while (j < rows) {
          *q = *p;
          p++;
          q++;
          j++;
        }
        if (odd) {
          *q = 0;
          q++;
        }
      }
      q = ptr;
      while (num > 0) {
        *q = (Nlm_Uint1) ~(*q);
        q++;
        num--;
      }
      hBitmap = CreateBitmap (r->right - r->left, r->bottom - r->top,
                              1, 1, (LPSTR) ptr);
      hMemoryDC = CreateCompatibleDC (Nlm_currentHDC);
      if ( hMemoryDC == NULL ) {
        hMemoryDC = CreateCompatibleDC (NULL);
        hOldBitmap = SelectObject (hMemoryDC, hBitmap);
        mode = SRCCOPY;
        BitBlt (Nlm_currentHDC, r->left, r->top,
                r->right - r->left, r->bottom - r->top,
                hMemoryDC, 0, 0, mode);
        SelectObject (hMemoryDC, hOldBitmap);
      } else {
        hOldBitmap = SelectObject (hMemoryDC, hBitmap);
        if (hOldBitmap != NULL) {
          rop2 = GetROP2( Nlm_currentHDC );
          switch (rop2) {
            case R2_COPYPEN:
              mode = SRCCOPY;
              break;
            case R2_MASKPEN:
              mode = SRCAND;
              break;
            case R2_NOTXORPEN:
              mode = 0x00990066;
              break;
            case R2_MERGENOTPEN:
              mode = MERGEPAINT;
              break;
            default:
              mode = WHITENESS;
              break;
          }
          BitBlt (Nlm_currentHDC, r->left, r->top,
                  r->right - r->left, r->bottom - r->top,
                  hMemoryDC, 0, 0, mode);
          SelectObject (hMemoryDC, hOldBitmap);
        }
      }
      Nlm_MemFree (ptr);
      DeleteDC (hMemoryDC);
      DeleteObject (hBitmap);
    }
  }
#endif
#ifdef WIN_X
  Nlm_Int2      cols;
  Nlm_Int2      height;
  Nlm_Int4      num;
  Pixmap        pixmap;
  Nlm_Uint1Ptr  ptr;
  Nlm_Uint1Ptr  q;
  Nlm_Int2      rows;
  Nlm_Int2      width;

  if (r != NULL && source != NULL && Nlm_currentXDisplay != NULL &&
      Nlm_currentXWindow != 0 && Nlm_currentXGC != NULL) {
    rows = (r->right - r->left - 1) / 8 + 1;
    cols = r->bottom - r->top;
    num = rows * cols;
    ptr = (Nlm_Uint1Ptr) Nlm_MemNew ((size_t) num * sizeof (Nlm_Uint1));
    if (ptr != NULL) {
      Nlm_MemCopy (ptr, source, (size_t) num);
      q = ptr;
      while (num > 0) {
        *q = flip [*q];
        q++;
        num--;
      }
      width = r->right - r->left;
      height = r->bottom - r->top;
      pixmap = XCreateBitmapFromData (Nlm_currentXDisplay, Nlm_currentXWindow,
                                      (char *) ptr, width, height);
      XSetGraphicsExposures (Nlm_currentXDisplay, Nlm_currentXGC, FALSE);
      if (currentMode != MERGE_MODE) {
        XCopyPlane (Nlm_currentXDisplay, pixmap, Nlm_currentXWindow,
                    Nlm_currentXGC, 0, 0, width, height,
                    r->left - Nlm_XOffset, r->top - Nlm_YOffset, 1);
      } else {
        XSetClipOrigin (Nlm_currentXDisplay, Nlm_currentXGC,
                        r->left - Nlm_XOffset, r->top - Nlm_YOffset);
        XSetClipMask (Nlm_currentXDisplay, Nlm_currentXGC, pixmap);
        XSetFunction (Nlm_currentXDisplay, Nlm_currentXGC, GXcopy);
        XCopyPlane (Nlm_currentXDisplay, pixmap, Nlm_currentXWindow,
                    Nlm_currentXGC, 0, 0, width, height,
                    r->left - Nlm_XOffset, r->top - Nlm_YOffset, 1);
        XSetFunction (Nlm_currentXDisplay, Nlm_currentXGC, GXand);
        XSetClipOrigin (Nlm_currentXDisplay, Nlm_currentXGC, 0, 0);
        if (Nlm_clpRgn != NULL) {
          XSetRegion (Nlm_currentXDisplay, Nlm_currentXGC, (Nlm_RgnTool) Nlm_clpRgn);
        } else {
          XSetClipMask (Nlm_currentXDisplay, Nlm_currentXGC, None);
        }
      }
      XSetGraphicsExposures (Nlm_currentXDisplay, Nlm_currentXGC, TRUE);
      XFreePixmap (Nlm_currentXDisplay, pixmap);
      Nlm_MemFree (ptr);
    }
  }
#endif
#ifdef WIN_GIF
  if (r != NULL && source != NULL && Nlm_currentGIF != NULL ){
    gdImageCopyBits ( Nlm_currentGIF, r->left, r->top,
                      r->right - r->left, r->bottom - r->top,
                      (char*)source, Nlm_curGIFColor );
  }
#endif
}

extern void Nlm_CopyPixmap ( Nlm_RectPtr r, Nlm_Int1Ptr source, 
                             Nlm_Uint1 totalColors, 
                             Nlm_RGBColoRPtr colorTable )
{
#ifdef WIN_MSWIN
  BITMAPINFO       PNTR bInfoPtr;
  RGBQUAD          PNTR quadPtr;
  BITMAPINFOHEADER PNTR bmpHeader;
  HBITMAP               pixMap;
  HDC                   hMemDC;
#endif
#ifdef WIN_MAC
  PixMap           PNTR pixelMap;
  Rect                  rectSrc;
  Rect                  rectDst;
  GrafPtr               port;
  CTabHandle            tabHandle;
  CTabPtr               tabPtr;
#endif
  Nlm_Int2              width;
  Nlm_Int2              height;
#ifndef WIN_GIF
  Nlm_Int2              i;
#endif

  if ( (r==NULL)||(source==NULL)||(totalColors==0)||
       (colorTable==NULL) ) return;

  width  = (Nlm_Int2)(r->right - r->left);
  height = (Nlm_Int2)(r->bottom - r->top);

#ifdef WIN_MSWIN
  bInfoPtr = (BITMAPINFO*)MemNew ( sizeof(BITMAPINFOHEADER) +
                                   totalColors*sizeof(RGBQUAD) );
  bInfoPtr->bmiHeader.biClrUsed = totalColors;
  for( i=0; i<(Nlm_Int2)totalColors; i++ ) {
    quadPtr = &(bInfoPtr->bmiColors[i]);
    quadPtr->rgbRed = colorTable[i].red;
    quadPtr->rgbGreen = colorTable[i].green;
    quadPtr->rgbBlue = colorTable[i].blue;
    quadPtr->rgbReserved = 0;
  }
  bmpHeader = &(bInfoPtr->bmiHeader);
  bmpHeader->biWidth = width;
  bmpHeader->biHeight = height;
  bmpHeader->biSize = sizeof(BITMAPINFOHEADER);
  bmpHeader->biCompression = BI_RGB;
  bmpHeader->biXPelsPerMeter = 2000;
  bmpHeader->biYPelsPerMeter = 2000;
  bmpHeader->biClrImportant = 0;
  bmpHeader->biSizeImage = 0;
  bmpHeader->biBitCount = 8;
  bmpHeader->biPlanes = 1;
  pixMap = CreateDIBitmap( Nlm_currentHDC,
                           (BITMAPINFOHEADER*)bInfoPtr, 
                           CBM_INIT, source, bInfoPtr, 
                           DIB_RGB_COLORS );
  if ( pixMap != NULL ){
    hMemDC = CreateCompatibleDC ( Nlm_currentHDC );
    if ( hMemDC != NULL ){
      SelectObject( hMemDC, pixMap );
      StretchBlt ( Nlm_currentHDC, r->left, r->top+height, width, -height, 
                   hMemDC, 0, 0, width, height, SRCCOPY );

/*      BitBlt( Nlm_currentHDC, r->left, r->top, width, height, hMemDC, 0, 0,
              SRCCOPY );*/
      DeleteDC( hMemDC );
    }
    DeleteObject( pixMap );
  }
  MemFree(bInfoPtr);
#endif
#ifdef WIN_MAC
  pixelMap = (PixMap*)MemNew(sizeof(PixMap));
  pixelMap->hRes = 72;
  pixelMap->vRes = 72;
  pixelMap->bounds.left = 0;
  pixelMap->bounds.top = 0;
  pixelMap->cmpSize = 8;
// 2001-03-22:  Joshua Juran
// Evidently these two members don't exist in Carbon.  So don't set them.
#if !TARGET_API_MAC_CARBON
  pixelMap->planeBytes = 0;
  pixelMap->pmReserved = 0;
#endif
  pixelMap->pmVersion = 0;
  pixelMap->packType = 0;
  pixelMap->packSize = 0;
  pixelMap->pixelSize = 8;
  pixelMap->pixelType = 0;
  pixelMap->cmpCount = 1;
  pixelMap->rowBytes = width | 0x8000;
  pixelMap->bounds.right = width;
  pixelMap->bounds.bottom = height;
  pixelMap->pmTable = tabHandle = GetCTable(72);
  if ( tabHandle != NULL ) {
    HLock ( (Handle)tabHandle );
    tabPtr = *tabHandle;
    for ( i=0; i<(Nlm_Int2)totalColors; i++ ){
      tabPtr->ctTable[i].rgb.red =   (Nlm_Uint2)colorTable[i].red<<8 | 
                                     (Nlm_Uint2)colorTable[i].red;
      tabPtr->ctTable[i].rgb.green = (Nlm_Uint2)colorTable[i].green<<8 | 
                                     (Nlm_Uint2)colorTable[i].green;
      tabPtr->ctTable[i].rgb.blue =  (Nlm_Uint2)colorTable[i].blue<<8 | 
                                     (Nlm_Uint2)colorTable[i].blue; 
      if (i >= 254) break;
    }
    tabPtr->ctSeed = GetCTSeed();
    HUnlock((Handle)tabHandle );
    pixelMap->baseAddr = (Ptr)source;
    rectSrc.top = 0;   rectSrc.bottom = height;
    rectSrc.left = 0;  rectSrc.right = width;
    rectDst.top = r->top;   rectDst.bottom = r->bottom;
    rectDst.left = r->left;  rectDst.right = r->right;
    GetPort(&port);
#ifdef CopyBits
#undef CopyBits
#endif
    CopyBits((BitMap*)pixelMap,
             GetPortBitMapForCopyBits(port),
             &rectSrc, &rectDst, srcCopy, NULL);
    DisposeCTable(tabHandle);
  }
  MemFree(pixelMap);
#endif

#ifdef WIN_MOTIF
  {{
  XVisualInfo visinfo;
  if (XMatchVisualInfo(Nlm_currentXDisplay, Nlm_currentXScreen,
                       8, PseudoColor, &visinfo) ||
      XMatchVisualInfo(Nlm_currentXDisplay, Nlm_currentXScreen,
                       8, GrayScale,   &visinfo) )
    {
      Visual*      vis    = visinfo.visual;
      Nlm_Uint1Ptr nSource = (Nlm_Uint1Ptr)MemNew(width * height);
      Nlm_Uint1Ptr iMap    = (Nlm_Uint1Ptr)MemNew( totalColors );
      XImage*      imageX11;
      Nlm_Uint1Ptr curPtr;
      Nlm_Uint1Ptr endPtr;

      for (i = 0;  i < (Nlm_Int2)totalColors;  i++)
        {
          XColor colorCell;
          Nlm_XAllocColor(&colorCell, Nlm_VibrantDefaultColormap(),
                          colorTable[i].red, colorTable[i].green,
                          colorTable[i].blue);
          iMap[i] = (Nlm_Uint1)colorCell.pixel;
        }

      curPtr = (Nlm_Uint1Ptr)nSource;
      endPtr = curPtr + width * height;
      while ( curPtr != endPtr )
        *curPtr++ = iMap[*source++];

      imageX11 = XCreateImage(Nlm_currentXDisplay, vis, 8, ZPixmap, 0, 
                              (char*)nSource, width, height, 8 , 0 );  
      XPutImage(Nlm_currentXDisplay, Nlm_currentXWindow, 
                Nlm_currentXGC, imageX11, 0, 0, r->left-Nlm_XOffset, 
                r->top-Nlm_YOffset, width, height );
      XFlush ( Nlm_currentXDisplay );
      XDestroyImage( imageX11 );
      MemFree( nSource );
      MemFree( iMap );
    }
  }}
#endif
}


extern void Nlm_SetUpDrawingTools (void)
{
#ifdef WIN_MAC
  Rect bounds;
  Nlm_FontSpec fsp;
  long       gval;
  char       tmpFontName[256];


  Nlm_scrollRgn = (Nlm_RegioN) (NewRgn ());

  Nlm_updateRgn = (Nlm_RegioN) (NewRgn ());
  SetRectRgn ((Nlm_RgnTool) Nlm_updateRgn, -32768, -32768, 32767, 32767);
  //HLock ((Handle) Nlm_updateRgn);
  GetRegionBounds(Nlm_updateRgn, &bounds);
  Local__RectToolToRecT (&bounds, &Nlm_updateRect);
  //HUnlock ((Handle) Nlm_updateRgn);

  Nlm_fontList = NULL;
  Nlm_fontInUse = NULL;
  Nlm_systemFont = (Nlm_FonT) Nlm_HandNew (sizeof (Nlm_FontRec));
  /* esl: LoadFontData changed to work with new FontData format */
  /* alexs get font name */
  memset ( &fsp, 0, sizeof(Nlm_FontSpec));
  GetFontName ( GetSysFont(), (StringPtr) tmpFontName );
  Nlm_PtoCstr ( tmpFontName );
  Nlm_StringNCpy_0 (fsp.name, tmpFontName, FONT_NAME_SIZE - 1);
  fsp.name[FONT_NAME_SIZE - 1] = 0;
  fsp.size = 12;
  Nlm_LoadFontData (Nlm_systemFont, NULL, -1, &fsp, 0, 12, 0, NULL);
  Nlm_programFont = (Nlm_FonT) Nlm_HandNew (sizeof (Nlm_FontRec));
  /* esl: LoadFontData changed to work with new FontData format */
  Nlm_StrCpy (fsp.name, "Monaco");
  fsp.size = 9;
  Nlm_LoadFontData (Nlm_programFont, NULL, -1, &fsp, 4, 9, 0, NULL);
  Nlm_fontList = NULL;
  Nlm_fontInUse = Nlm_systemFont;

  Nlm_stdAscent = Nlm_Ascent ();
  Nlm_stdDescent = Nlm_Descent ();
  Nlm_stdLeading = Nlm_Leading ();
  Nlm_stdFontHeight = Nlm_FontHeight ();
  Nlm_stdLineHeight = Nlm_LineHeight ();
  Nlm_stdCharWidth = Nlm_MaxCharWidth ();
  /* gestalt for quickdraw features are defined as bits in a bitfield
     for example gestaltHasColor = 0, thus we need to test for lsb set
   */
  if( Gestalt( gestaltQuickdrawFeatures, &gval) == noErr){
      Nlm_hasColorQD = (gval && (1 << gestaltHasColor));
  }
  if (Nlm_hasColorQD) {
    Nlm_RGBforeColor.red = 0;
    Nlm_RGBforeColor.green = 0;
    Nlm_RGBforeColor.blue = 0;
    Nlm_RGBbackColor.red = 65535;
    Nlm_RGBbackColor.green = 65535;
    Nlm_RGBbackColor.blue = 65535;
  }
#endif
#ifdef WIN_MSWIN
  Nlm_scrollRgn = (Nlm_RegioN)CreateRectRgn(0, 0, 0, 0);
  Nlm_updateRgn = (Nlm_RegioN)CreateRectRgn(-32767, -32767, 32767, 32767);

  {{
  Nlm_RectTool  rtool;
  GetRgnBox((Nlm_RgnTool)Nlm_updateRgn, &rtool);
  Local__RectToolToRecT(&rtool, &Nlm_updateRect);
  }}

  /* Stock fonts */
  hAnsiFixedFont     = GetStockObject( ANSI_FIXED_FONT     );
  hAnsiVarFont       = GetStockObject( ANSI_VAR_FONT       );
  hDeviceDefaultFont = GetStockObject( DEVICE_DEFAULT_FONT );
  hOemFixedFont      = GetStockObject( OEM_FIXED_FONT      );
  hSystemFont        = GetStockObject( SYSTEM_FONT         );
  hSystemFixedFont   = GetStockObject( SYSTEM_FIXED_FONT   );
  hDefaultGuiFont    = GetStockObject( DEFAULT_GUI_FONT    );

  Nlm_systemFont  = (Nlm_FonT) Nlm_HandNew( sizeof(Nlm_FontRec) );
  Nlm_LoadFontData(Nlm_systemFont,  NULL, -1, NULL, hDefaultGuiFont,
                   HFONT2Font( hDefaultGuiFont ));

  Nlm_fontList  = NULL;
  Nlm_fontInUse = Nlm_systemFont;

  Nlm_stdAscent     = Nlm_Ascent();
  Nlm_stdDescent    = Nlm_Descent();
  Nlm_stdLeading    = Nlm_Leading();
  Nlm_stdFontHeight = Nlm_FontHeight();
  Nlm_stdLineHeight = Nlm_LineHeight();
  Nlm_stdCharWidth  = Nlm_MaxCharWidth();

  Nlm_programFont = (Nlm_FonT) Nlm_HandNew( sizeof(Nlm_FontRec) );
  Nlm_LoadFontData(Nlm_programFont, NULL, -1, NULL, hAnsiFixedFont,
                   HFONT2Font( hAnsiFixedFont ));

  blackColor   = RGB(  0,   0,   0);
  redColor     = RGB(255,   0,   0);
  greenColor   = RGB(  0, 255,   0);
  blueColor    = RGB(  0,   0, 255);
  cyanColor    = RGB(  0, 255, 255);
  magentaColor = RGB(255,   0, 255);
  yellowColor  = RGB(255, 255,   0);
  whiteColor   = RGB(255, 255, 255);

  hBlackPen = GetStockObject( BLACK_PEN );
  hNullPen  = GetStockObject( NULL_PEN  );
  hWhitePen = GetStockObject( WHITE_PEN );

  hBlackBrush  = GetStockObject( BLACK_BRUSH  );
  hDkGrayBrush = GetStockObject( DKGRAY_BRUSH );
  hGrayBrush   = GetStockObject( GRAY_BRUSH   );
  hHollowBrush = GetStockObject( HOLLOW_BRUSH );
  hLtGrayBrush = GetStockObject( LTGRAY_BRUSH );
  hNullBrush   = GetStockObject( NULL_BRUSH   );
  hWhiteBrush  = GetStockObject( WHITE_BRUSH  );
#endif
#ifdef WIN_X
  XFontStruct   *f;
  Nlm_Int2      i;
  Nlm_Uint2     inv;
  Nlm_Int2      j;
  XFontStruct   *p;
  Nlm_RecT      r;
  Nlm_RectTool  rtool;
  Nlm_Uint2     val;
  Nlm_FontSpec  fsp;
  Nlm_Char      fSpecName[64];


  Nlm_scrollRgn = (Nlm_RegioN) (XCreateRegion ());

  Nlm_updateRgn = (Nlm_RegioN) (XCreateRegion ());
  Nlm_LoadRect (&r, -32767, -32767, 32767, 32767);
  Local__RecTToRectTool (&r, &rtool);
  XUnionRectWithRegion (&rtool, (Nlm_RgnTool) Nlm_updateRgn, (Nlm_RgnTool) Nlm_updateRgn);
  Local__RectToolToRecT (&rtool, &Nlm_updateRect);

  emptyRgn = XCreateRegion ();

  Nlm_fontList = NULL;
  Nlm_fontInUse = NULL;
  {{
    XFontStruct *F =XQueryFont(Nlm_currentXDisplay,
                               XGContextFromGC(DefaultGC(Nlm_currentXDisplay,
                                                         Nlm_currentXScreen)));
    i = F->ascent + F->descent;
    XFreeFontInfo(NULL, F, 1);
  }}
  sprintf ( fSpecName, "-*-helvetica-bold-r-*--%d-*-*", i );
  f = Nlm_XLoadQueryFont (Nlm_currentXDisplay, fSpecName, FALSE);
  if ( f == NULL ){
    i++;
    sprintf ( fSpecName, "-*-helvetica-bold-r-*--%d-*-*", i );
    f = Nlm_XLoadQueryFont (Nlm_currentXDisplay, fSpecName, FALSE);
  }
  if ( f == NULL ){
    i--; i--;
    sprintf ( fSpecName, "-*-helvetica-bold-r-*--%d-*-*", i );
    f = Nlm_XLoadQueryFont (Nlm_currentXDisplay, fSpecName, FALSE);
  }
  if ( f == NULL ){
    f = Nlm_XLoadQueryFont (Nlm_currentXDisplay, "-*-helvetica-bold-r-*--*-140-*", FALSE);
    i = 14;
  }

  memset(&fsp, 0, sizeof(Nlm_FontSpec));
  if ( f ) {
    Nlm_StrCpy(fsp.name, "helvetica");
    fsp.style = STYLE_BOLD;
  } else {
    f = Nlm_XLoadStandardFont();
    fsp.name[0] = '\0';
    fsp.style = STYLE_REGULAR;
    i = f->ascent + f->descent;
  }
  fsp.size = i;
  
  Nlm_systemFont = (Nlm_FonT) Nlm_HandNew (sizeof (Nlm_FontRec));
  Nlm_LoadFontData (Nlm_systemFont, NULL, -1, &fsp, f, NULL);

  sprintf ( fSpecName, "-*-fixed-medium-r-*--%d-*-*", i );
  p = Nlm_XLoadQueryFont (Nlm_currentXDisplay, fSpecName, FALSE);
  Nlm_StrCpy ( fsp.name, "fixed" );
  fsp.size = i;
  fsp.style = STYLE_REGULAR;
  if (p == NULL) {
    p = Nlm_XLoadQueryFont (Nlm_currentXDisplay, "-*-fixed-medium-r-*--*-120-*", FALSE);
    Nlm_StrCpy ( fsp.name, "fixed" );
    fsp.size = 12;
  }
  if (p == NULL) {
    p = Nlm_XLoadQueryFont (Nlm_currentXDisplay,
                            "-*-courier-medium-r-*--*-120-*", FALSE);
    Nlm_StrCpy ( fsp.name, "courier" );
    fsp.size = 12;
  }
  if ( !p ) {
    p = Nlm_XLoadStandardFont();
    fsp.name[0] = '\0';
    fsp.size = p->ascent + p->descent;
  }
  Nlm_programFont = (Nlm_FonT) Nlm_HandNew (sizeof (Nlm_FontRec));
  /* esl: LoadFontData changed to work with new FontData format */
  Nlm_LoadFontData (Nlm_programFont, NULL, -1, &fsp, p, NULL);
  Nlm_fontList = NULL;
  Nlm_fontInUse = Nlm_systemFont;

  XSetFont (Nlm_currentXDisplay, Nlm_currentXGC, f->fid);
  currentFont = f;
  Nlm_stdAscent = Nlm_Ascent ();
  Nlm_stdDescent = Nlm_Descent ();
  Nlm_stdLeading = Nlm_Leading ();
  Nlm_stdFontHeight = Nlm_FontHeight ();
  Nlm_stdLineHeight = Nlm_LineHeight ();
  Nlm_stdCharWidth = Nlm_MaxCharWidth ();

  Nlm_hasColor = (Nlm_currentXDisplay != NULL  &&
                  XDisplayCells(Nlm_currentXDisplay, Nlm_currentXScreen) > 2);

  if ( Nlm_hasColor )
    {
      whiteColor   = Nlm_GetColorRGB(255, 255, 255);
      blackColor   = Nlm_GetColorRGB(  0,   0,   0);
      redColor     = Nlm_GetColorRGB(255,   0,   0);
      greenColor   = Nlm_GetColorRGB(  0, 255,   0);
      blueColor    = Nlm_GetColorRGB(  0,   0, 255);
      cyanColor    = Nlm_GetColorRGB(  0, 255, 255);
      magentaColor = Nlm_GetColorRGB(255,   0, 255);
      yellowColor  = Nlm_GetColorRGB(255, 255,   0);
    }
  else
    {
      whiteColor   = WhitePixel(Nlm_currentXDisplay, Nlm_currentXScreen);
      blackColor   = BlackPixel(Nlm_currentXDisplay, Nlm_currentXScreen);
      redColor     = BlackPixel(Nlm_currentXDisplay, Nlm_currentXScreen);
      greenColor   = BlackPixel(Nlm_currentXDisplay, Nlm_currentXScreen);
      blueColor    = BlackPixel(Nlm_currentXDisplay, Nlm_currentXScreen);
      cyanColor    = BlackPixel(Nlm_currentXDisplay, Nlm_currentXScreen);
      magentaColor = BlackPixel(Nlm_currentXDisplay, Nlm_currentXScreen);
      yellowColor  = BlackPixel(Nlm_currentXDisplay, Nlm_currentXScreen);
    }

  fontInfo.fid = 0;
  for (i = 0; i < 256; i++) {
    inv = 0;
    val = (Nlm_Uint2) i;
    for (j = 0; j < 8; j++) {
      inv = (inv << 1);
      inv += (val % 2);
      val = (val >> 1);
    }
    flip [i] = inv;
  }

  Nlm_XbackColor = whiteColor;
  Nlm_XforeColor = blackColor;
  Nlm_XOffset = 0;
  Nlm_YOffset = 0;
  Nlm_clpRgn = NULL;
#endif
#ifdef WIN_GIF
  Nlm_curGIFColor = 1;
  Nlm_curGIFLType = GIF_SOLID;
  Nlm_curGIFFont = gdFont7X13b;
  Nlm_curGIFPoint.x = 0;
  Nlm_curGIFPoint.y = 0;
#endif
}

extern void Nlm_CleanUpDrawingTools (void)

{
  Nlm_FonT      f;
  Nlm_FontData  fdata;

  Nlm_ResetDrawingTools ();
#ifdef WIN_MOTIF
  Nlm_GetFontData (Nlm_systemFont, &fdata);
  if (fdata.handle != NULL) {
    XFreeFont (Nlm_currentXDisplay, fdata.handle);
  }
  Nlm_GetFontData (Nlm_programFont, &fdata);
  if (fdata.handle != NULL) {
    XFreeFont (Nlm_currentXDisplay, fdata.handle);
  }
#endif
  Nlm_HandFree (Nlm_systemFont);
  Nlm_HandFree (Nlm_programFont);
  f = Nlm_fontList;
  while (f != NULL) {
    Nlm_GetFontData (f, &fdata);
#ifdef WIN_MSWIN
    if (fdata.handle != NULL) {
      DeleteObject (fdata.handle);
    }
#endif
#ifdef WIN_MOTIF
    if (fdata.handle != NULL) {
      XFreeFont (Nlm_currentXDisplay, fdata.handle);
    }
#endif
    Nlm_HandFree (f);
    f = fdata.next;
  }

#ifdef WIN_MOTIF
  XDestroyRegion (emptyRgn);
#endif
}


size_t UpdateColorTable(Nlm_RGBColoR table[], size_t table_size,
                        const Nlm_Char PNTR filename)
{
  size_t n_done = 0;
  FILE *file;
  Nlm_Char str[128];

  if (table_size < 1  ||  filename == NULL)  return 0;

  file = Nlm_FileOpen(filename, "r");
  if (file == NULL)
    {
      Nlm_ErrLogPrintf("\n\
Warning:  Cannot open the user's color description file \"%s\"\n",
                       filename);
      return 0;
    }

  while (Nlm_FileGets(str, sizeof(str), file) != NULL)
    {
      int index;
      int red, green, blue;
      int n_char_read;
      if (sscanf(str, "%i %i %i %i %n",
                 &index, &red, &green, &blue, &n_char_read) != 4)
        {
          Nlm_ErrLogPrintf("\n\
[%s] Warning:\n\
Cannot extract <index> <red> <green> <blue> from the stroke:\n\
\"%s\"\n",
                           filename, str);
          continue;
        }

      if (index < 0  ||  table_size <= (size_t)index)
        {
          Nlm_ErrLogPrintf("\n\
[%s] Warning:\n\
The color index is out of range = %d  (must be:  0 <= index <= %l)\n",
                           filename, index, (long)(table_size - 1));
          continue;
        }

#ifdef WIN_MOTIF
      {{
        Nlm_CharPtr name, s;
        for (name=str+n_char_read;  *name != '\0' && !isalnum(*name);  name++);
        for (s   =name;             *s    != '\0' &&  isalnum(*s   );  s++   );
        *s = '\0';

        if (*name != '\0')
          {
            XColor rgb_db_def, hardware_def;
            if ( XLookupColor(Nlm_currentXDisplay,
                              Nlm_VibrantDefaultColormap(),
                              name,
                              &rgb_db_def, &hardware_def) )
              {
                red   = (int)(hardware_def.red   >> 8);
                green = (int)(hardware_def.green >> 8);
                blue  = (int)(hardware_def.blue  >> 8);
              }
            else
              {
                Nlm_ErrLogPrintf("\n\
[%s] Warning:\n\
Cannot find color of name \"%s\" in the X11 color database",
                                 filename, name);
                continue;
              }
          }
      }}
#endif

      if (red   < 0  ||  255 < red   ||
          green < 0  ||  255 < green ||
          blue  < 0  ||  255 < blue)
        {
          Nlm_ErrLogPrintf("\n\
[%s] Warning:\n\
The color component values are out of range = (%d, %d, %d)\n\
(must be:  0 <= value <= 255)\n",
                           filename, red, green, blue);
          continue;
        }

      table[index].red   = (Nlm_Uint1) red;
      table[index].green = (Nlm_Uint1) green;
      table[index].blue  = (Nlm_Uint1) blue;
      n_done++;
    }

  Nlm_FileClose( file );
  return n_done;
}


static Nlm_Boolean s_VibrantIsGUI = FALSE;
extern Nlm_Boolean Nlm_VibrantIsGUI(void) {
  return s_VibrantIsGUI;
}
extern void Nlm_VibrantSetGUI(void) {
  s_VibrantIsGUI = TRUE;
}
