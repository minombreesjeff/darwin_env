/*   ncbidraw.h
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
* File Name:  ncbidraw.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   1/1/91
*
* $Revision: 6.9 $
*
* File Description: 
*       Vibrant drawing procedure definitions
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: ncbidraw.h,v $
* Revision 6.9  2000/07/08 20:44:13  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 6.8  2000/01/24 16:11:13  lewisg
* speed up seqid comparison in color manager, make fast windows version of SetColor()
*
* Revision 6.7  1999/10/20 23:10:04  vakatov
* + Nlm_SetCurrentGIF(), + Nlm_DestroyGIF() proto.
* Get rid of the (now obsolete) PROTO macro; added more comments.
*
* Revision 6.6  1999/10/04 17:16:32  kans
* include ncbidraw.h instead of vibrant.h, a couple Nlm_ prefixes
*
* Revision 6.5  1998/07/14 16:44:26  vakatov
* Added VibrantIsGUI() and <internal> Nlm_VibrantSetGUI()
*
* Revision 6.4  1998/07/01 18:27:42  vakatov
* Use "const" qualifier somewhere
*
* Revision 6.3  1998/06/10 22:05:26  lewisg
* added PROTO() around *Quadrant()
*
* Revision 6.2  1998/06/01 17:20:13  vakatov
* Added code to draw/fill 90-degree arc/pie (quadrants)
*
* Revision 6.1  1997/09/16 20:24:53  vakatov
* Added Nlm_FitStringWidth()
*
* Revision 5.8  1997/07/23 19:42:22  vakatov
* Added Nlm_PaintStringEx function(text + position)
*
* Revision 5.7  1997/07/18 15:18:45  vakatov
* [!WIN_GIF] +Dummy Nlm_CreateGIF() and Nlm_SaveGIF();  /return FALSE/
*
* Revision 5.6  1997/06/18 22:10:10  vakatov
* [WIN_GIF] Moved Create/SaveGIF() decl. from "ncbigif.h" to "ncbidraw.h"
*
* Revision 5.2  1997/06/04 00:05:20  kans
* support for Japanese by Tomo Koike of DDBJ
*
* Revision 5.1  1996/09/30 19:56:00  vakatov
* + Nlm_SetPenDash PROTO and synopsis
*
* Revision 4.1  1996/05/10  21:14:21  vakatov
* Added UpdateColorTable() function to allow the user to read his color
* preferencies from an external config.-file
*
* Revision 2.12  1995/07/14  17:48:26  kans
* new CopyPixmap (AS)
* ==========================================================================
*/

#ifndef _NCBIDRAW_
#define _NCBIDRAW_

#ifndef _NCBI_
#include <ncbi.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

/***  PORTABLE GRAPHIC PRIMITIVE OBJECT TYPES  ***/

/*
*  Platform independent point and rectangle data structures.
*/

typedef  struct  Nlm_point {
  Nlm_Int2  x;
  Nlm_Int2  y;
} Nlm_PoinT, PNTR Nlm_PointPtr;

typedef  struct  Nlm_rect {
  Nlm_Int2  left;
  Nlm_Int2  top;
  Nlm_Int2  right;
  Nlm_Int2  bottom;
} Nlm_RecT, PNTR Nlm_RectPtr;

typedef  Nlm_Handle  Nlm_RegioN;

typedef  struct  Nlm_font {
  Nlm_VoidPtr  dummy;
} HNDL Nlm_FonT;

/***  GLOBAL VARIABLES  ***/

/*
*  The update region contains the update region on drawing requests,
*  and contains at least the visible region during other callbacks.
*  Clipping is set to the update region on drawing events.  The
*  update rectangle has the minimum rectangular boundary of the
*  update region.  These variables are kept up to date by the Vibrant
*  event loop.
*/

extern  Nlm_RegioN  Nlm_updateRgn;
extern  Nlm_RecT    Nlm_updateRect;

/*
*  The standard systemFont and programFont variables are used to
*  specify fonts for dialog objects and scrolling text objects.
*/

extern  Nlm_FonT  Nlm_systemFont;
extern  Nlm_FonT  Nlm_programFont;

/*
*  Miscellaneous constants for pixel sizes of the standard font.
*/

extern  Nlm_Int2  Nlm_stdAscent;
extern  Nlm_Int2  Nlm_stdDescent;
extern  Nlm_Int2  Nlm_stdLeading;
extern  Nlm_Int2  Nlm_stdFontHeight;
extern  Nlm_Int2  Nlm_stdLineHeight;
extern  Nlm_Int2  Nlm_stdCharWidth;

/***  DRAWING PROCEDURES  ***/

void         Nlm_SetUpDrawingTools (void);
void         Nlm_CleanUpDrawingTools(void);

/*
*  It is not necessary to create a new font when switching colors.  The
*  family for GetFont can be Roman, Swiss, Modern, Script, or Decorative.
*/

/*
*  ScrollRect will erase and invalidate all invalid regions.
*/

void         Nlm_ResetDrawingTools(void);

void         Nlm_CopyMode(void);
void         Nlm_MergeMode(void);
void         Nlm_InvertMode(void);
void         Nlm_EraseMode(void);

void         Nlm_Black(void);
void         Nlm_Red(void);
void         Nlm_Green(void);
void         Nlm_Blue(void);
void         Nlm_Cyan(void);
void         Nlm_Magenta(void);
void         Nlm_Yellow(void);
void         Nlm_White(void);
void         Nlm_Gray(void);
void         Nlm_LtGray(void);
void         Nlm_DkGray(void);
void         Nlm_SelectColor(Nlm_Uint1 red, Nlm_Uint1 green, Nlm_Uint1 blue);
Nlm_Uint4    Nlm_GetColorRGB(Nlm_Uint1 red, Nlm_Uint1 green, Nlm_Uint1 blue);
Nlm_Uint4    Nlm_GetColor(void);
void         Nlm_SetColorEx (Nlm_Uint4 color);
void         Nlm_SetColor(Nlm_Uint4 color);
void         Nlm_InvertColors(void);
void         Nlm_DecodeColor(Nlm_Uint4 color, Nlm_Uint1Ptr red, Nlm_Uint1Ptr green, Nlm_Uint1Ptr blue);

void         Nlm_Solid(void);
void         Nlm_Dark(void);
void         Nlm_Medium(void);
void         Nlm_Light(void);
void         Nlm_Empty(void);
void         Nlm_SetPenPattern(Nlm_VoidPtr pattern);
void         Nlm_Dotted(void);
void         Nlm_Dashed(void);
void         Nlm_WidePen(Nlm_Int2 width);

/* Under X11       -- pen offset, dash length and gap length
 * Under Win-NT    -- parameter "offset" ignored(always zero)
 * Other platforms -- exactly analogous to "Nlm_Dotted()"
 */
void Nlm_SetPenDash(Nlm_Uint1 offset, Nlm_Uint1 dash, Nlm_Uint1 gap);

/* esl++ */
/***  FONT HANDLING PROCEDURES  ***/

/* font styles */
#define STYLE_REGULAR       0
#define STYLE_BOLD          1
#define STYLE_ITALIC        2
#define STYLE_BOLD_ITALIC   3
#define STYLE_UNDERLINE     4
/* (other bits are used for platform-specific styles) */

/* font charset */
#define CHARSET_NULL        0
#define CHARSET_ANSI        1
#define CHARSET_SYMBOL      2
#define CHARSET_KANJI		3	/* Japanese Kanji */
#define CHASET_HANGUL		4	/* Korean character set */

/* font pitch */
#define PITCH_NULL          0
#define PITCH_FIXED         1
#define PITCH_VARIABLE      2

/* font family */
#define FAMILY_NULL         0
#define FAMILY_ROMAN        1
#define FAMILY_SWISS        2
#define FAMILY_MODERN       3
#define FAMILY_SCRIPT       4
#define FAMILY_DECORATIVE   5
#define FAMILY_GOTHIC		6	/* Japanese Kanji or Korean Hangle */
#define FAMILY_MINCHOU		7	/* Japanese Kanji */

#define FONT_NAME_SIZE      32

typedef struct Nlm_fontspec {
  Nlm_Char name [FONT_NAME_SIZE];
  Nlm_Int2 size;
  Nlm_Uint1 style;
  Nlm_Uint1 charset;
  Nlm_Uint1 pitch;
  Nlm_Uint1 family;
} Nlm_FontSpec, PNTR Nlm_FontSpecPtr;

Nlm_FonT     Nlm_CreateFont(Nlm_FontSpecPtr fsp);
Nlm_FonT     Nlm_GetResidentFont(Nlm_FontSpecPtr fsp);
Nlm_FonT     Nlm_CopyFont(Nlm_FonT font);
Nlm_FonT     Nlm_DeleteFont(Nlm_FonT font);
Nlm_FonT     Nlm_FindNextResidentFont(Nlm_FonT font);
Nlm_Boolean  Nlm_GetFontSpec(Nlm_FonT font, Nlm_FontSpecPtr fsp);
Nlm_Boolean  Nlm_EqualFontSpec(Nlm_FontSpecPtr fsp1, Nlm_FontSpecPtr fsp2);

/*
*  The functions below return the specifications for common fonts.
*  The return value points to the static buffer that is owerwritten
*  by next call to any of these functions.
*  Example: FonT f = CreateFont (Times (24, STYLE_BOLD_ITALIC);
*/
extern Nlm_FontSpecPtr Nlm_Helvetica(Nlm_Int2 size, Nlm_Uint1 style);
extern Nlm_FontSpecPtr Nlm_Times(Nlm_Int2 size, Nlm_Uint1 style);
extern Nlm_FontSpecPtr Nlm_Courier(Nlm_Int2 size, Nlm_Uint1 style);
extern Nlm_FontSpecPtr Nlm_Symbol(Nlm_Int2 size, Nlm_Uint1 style);
extern Nlm_FontSpecPtr Nlm_Gothic(Nlm_Int2 size, Nlm_Uint1 style);
extern Nlm_FontSpecPtr Nlm_Minchou(Nlm_Int2 size, Nlm_Uint1 style);
extern Nlm_FontSpecPtr Nlm_GothicFixed(Nlm_Int2 size, Nlm_Uint1 style);
extern Nlm_FontSpecPtr Nlm_MinchouFixed(Nlm_Int2 size, Nlm_Uint1 style);

/* esl++ end */

Nlm_FonT     Nlm_GetFont(Nlm_CharPtr name, Nlm_Int2 size, Nlm_Boolean bld, Nlm_Boolean itlc, Nlm_Boolean undrln, Nlm_CharPtr fmly);
Nlm_FonT     Nlm_GetFontEx(Nlm_CharPtr name, Nlm_Int2 size, Nlm_Boolean bld, Nlm_Boolean itlc, Nlm_Boolean undrln, Nlm_CharPtr fmly, Nlm_CharPtr chset, Nlm_Boolean fixed);
Nlm_FonT     Nlm_ParseFont(Nlm_CharPtr spec);
Nlm_FonT     Nlm_ParseFontEx(Nlm_CharPtr scrSpec, Nlm_CharPtr prtSpec);
void         Nlm_SelectFont(Nlm_FonT f);
void         Nlm_AssignPrinterFont(Nlm_FonT scrnFont, Nlm_FonT prtrFont);

Nlm_Int2     Nlm_CharWidth(Nlm_Char ch);
Nlm_Int2     Nlm_StringWidth(const Nlm_Char* text);
Nlm_Int2     Nlm_TextWidth(const Nlm_Char* text, size_t len);
Nlm_Int2     Nlm_Ascent(void);
Nlm_Int2     Nlm_Descent(void);
Nlm_Int2     Nlm_Leading(void);
Nlm_Int2     Nlm_FontHeight(void);
Nlm_Int2     Nlm_LineHeight(void);
Nlm_Int2     Nlm_MaxCharWidth(void);

/* Calculate(based on the presently active font) and return maximum
 * number of characters from the string "str" which can be fit into
 * "*max_width" pixels.
 * Return 0 if the "str" is NULL or empty or if the curr.font is NULL.
 */
size_t Nlm_FitStringWidth(const Nlm_Char PNTR str, Nlm_Int4 max_width);

void         Nlm_SetPen(Nlm_PoinT pt);
void         Nlm_GetPen(Nlm_PointPtr pt);

void         Nlm_PaintChar(Nlm_Char ch);
void         Nlm_PaintString(Nlm_CharPtr text);
void         Nlm_PaintStringEx(Nlm_CharPtr text, Nlm_Int2 x, Nlm_Int2 y);
void CDECL   Nlm_PaintText(char *format, ...);

void         Nlm_DrawString(Nlm_RectPtr r, Nlm_CharPtr text, Nlm_Char jst, Nlm_Boolean gray);
void         Nlm_DrawText(Nlm_RectPtr r, Nlm_CharPtr text, size_t len, Nlm_Char jst, Nlm_Boolean gray);

void         Nlm_MoveTo(Nlm_Int2 x, Nlm_Int2 y);
void         Nlm_LineTo(Nlm_Int2 x, Nlm_Int2 y);
void         Nlm_DrawLine(Nlm_PoinT pt1, Nlm_PoinT pt2);

void         Nlm_LoadPt(Nlm_PointPtr pt, Nlm_Int2 x, Nlm_Int2 y);
void         Nlm_AddPt(Nlm_PoinT src, Nlm_PointPtr dst);
void         Nlm_SubPt(Nlm_PoinT src, Nlm_PointPtr dst);
Nlm_Boolean  Nlm_EqualPt(Nlm_PoinT p1, Nlm_PoinT p2);
Nlm_Boolean  Nlm_PtInRect(Nlm_PoinT pt, Nlm_RectPtr r);
Nlm_Boolean  Nlm_PtInRgn(Nlm_PoinT pt, Nlm_RegioN rgn);

void         Nlm_LoadRect(Nlm_RectPtr r, Nlm_Int2 lf, Nlm_Int2 tp, Nlm_Int2 rt, Nlm_Int2 bt);
void         Nlm_UpsetRect(Nlm_RectPtr r, Nlm_Int2 lf, Nlm_Int2 tp, Nlm_Int2 rt, Nlm_Int2 bt);
void         Nlm_OffsetRect(Nlm_RectPtr r, Nlm_Int2 dx, Nlm_Int2 dy);
void         Nlm_InsetRect(Nlm_RectPtr r, Nlm_Int2 dx, Nlm_Int2 dy);
Nlm_Boolean  Nlm_SectRect(Nlm_RectPtr src1, Nlm_RectPtr src2, Nlm_RectPtr dst);
Nlm_Boolean  Nlm_UnionRect(Nlm_RectPtr src1, Nlm_RectPtr src2, Nlm_RectPtr dst);
Nlm_Boolean  Nlm_EqualRect(Nlm_RectPtr r1, Nlm_RectPtr r2);
Nlm_Boolean  Nlm_EmptyRect(Nlm_RectPtr r);
Nlm_Boolean  Nlm_RectInRect(Nlm_RectPtr r1, Nlm_RectPtr r2);
Nlm_Boolean  Nlm_RectInRgn(Nlm_RectPtr r, Nlm_RegioN rgn);

void         Nlm_EraseRect(Nlm_RectPtr r);
void         Nlm_FrameRect(Nlm_RectPtr r);
void         Nlm_PaintRect(Nlm_RectPtr r);
void         Nlm_InvertRect(Nlm_RectPtr r);
void         Nlm_ScrollRect(Nlm_RectPtr r, Nlm_Int2 dx, Nlm_Int2 dy);

void         Nlm_EraseOval(Nlm_RectPtr r);
void         Nlm_FrameOval(Nlm_RectPtr r);
void         Nlm_PaintOval(Nlm_RectPtr r);
void         Nlm_InvertOval(Nlm_RectPtr r);

void         Nlm_EraseRoundRect(Nlm_RectPtr r, Nlm_Int2 ovlWid, Nlm_Int2 ovlHgt);
void         Nlm_FrameRoundRect(Nlm_RectPtr r, Nlm_Int2 ovlWid, Nlm_Int2 ovlHgt);
void         Nlm_PaintRoundRect(Nlm_RectPtr r, Nlm_Int2 ovlWid, Nlm_Int2 ovlHgt);
void         Nlm_InvertRoundRect(Nlm_RectPtr r, Nlm_Int2 ovlWid, Nlm_Int2 ovlHgt);

void         Nlm_EraseArc(Nlm_RectPtr r, Nlm_PoinT start, Nlm_PoinT end);
void         Nlm_FrameArc(Nlm_RectPtr r, Nlm_PoinT start, Nlm_PoinT end);
void         Nlm_PaintArc(Nlm_RectPtr r, Nlm_PoinT start, Nlm_PoinT end);
void         Nlm_InvertArc(Nlm_RectPtr r, Nlm_PoinT start, Nlm_PoinT end);

/* Special case of an arc(90 grad) */
typedef enum {
  eQ_RightTop    = 1,
  eQ_LeftTop     = 2,
  eQ_LeftBottom  = 3,
  eQ_RightBottom = 4
} Nlm_EQuadrant;
#define EQuadrant Nlm_EQuadrant

void         Nlm_EraseQuadrant(Nlm_RectPtr r, Nlm_EQuadrant quadrant);
void         Nlm_FrameQuadrant(Nlm_RectPtr r, Nlm_EQuadrant quadrant);
void         Nlm_PaintQuadrant(Nlm_RectPtr r, Nlm_EQuadrant quadrant);
void         Nlm_InvertQuadrant(Nlm_RectPtr r, Nlm_EQuadrant quadrant);


void         Nlm_ErasePoly(Nlm_Int2 num, Nlm_PointPtr pts);
void         Nlm_FramePoly(Nlm_Int2 num, Nlm_PointPtr pts);
void         Nlm_PaintPoly(Nlm_Int2 num, Nlm_PointPtr pts);
void         Nlm_InvertPoly(Nlm_Int2 num, Nlm_PointPtr pts);

Nlm_RegioN   Nlm_CreateRgn(void);
Nlm_RegioN   Nlm_DestroyRgn(Nlm_RegioN rgn);
void         Nlm_ClearRgn(Nlm_RegioN rgn);
void         Nlm_LoadRectRgn(Nlm_RegioN rgn, Nlm_Int2 lf, Nlm_Int2 tp, Nlm_Int2 rt, Nlm_Int2 bt);
void         Nlm_OffsetRgn(Nlm_RegioN rgn, Nlm_Int2 dx, Nlm_Int2 dy);
void         Nlm_SectRgn(Nlm_RegioN src1, Nlm_RegioN src2, Nlm_RegioN dst);
void         Nlm_UnionRgn(Nlm_RegioN src1, Nlm_RegioN src2, Nlm_RegioN dst);
void         Nlm_DiffRgn(Nlm_RegioN src1, Nlm_RegioN src2, Nlm_RegioN dst);
void         Nlm_XorRgn(Nlm_RegioN src1, Nlm_RegioN src2, Nlm_RegioN dst);
Nlm_Boolean  Nlm_EqualRgn(Nlm_RegioN rgn1, Nlm_RegioN rgn2);
Nlm_Boolean  Nlm_EmptyRgn(Nlm_RegioN rgn);

void         Nlm_EraseRgn(Nlm_RegioN rgn);
void         Nlm_FrameRgn(Nlm_RegioN rgn);
void         Nlm_PaintRgn(Nlm_RegioN rgn);
void         Nlm_InvertRgn(Nlm_RegioN rgn);

void         Nlm_ClipRect(Nlm_RectPtr r);
void         Nlm_ClipRgn(Nlm_RegioN rgn);
void         Nlm_ResetClip(void);

void         Nlm_ValidRect(Nlm_RectPtr r);
void         Nlm_InvalRect(Nlm_RectPtr r);
void         Nlm_ValidRgn(Nlm_RegioN rgn);
void         Nlm_InvalRgn(Nlm_RegioN rgn);

void         Nlm_CopyBits(Nlm_RectPtr r, Nlm_VoidPtr source);

typedef struct {
  Nlm_Uint1 red;
  Nlm_Uint1 green;
  Nlm_Uint1 blue;
} Nlm_RGBColoR, PNTR Nlm_RGBColoRPtr;

void         Nlm_CopyPixmap(Nlm_RectPtr r, Nlm_Int1Ptr source, 
                            Nlm_Uint1 totalColors, 
                            Nlm_RGBColoRPtr colorTable);

/* Try to read alternate color set from the user-specified file;
 * <table_index> <red> <green> <blue> [<name>].
 * Return number of updated colors
 */ 
size_t Nlm_UpdateColorTable(Nlm_RGBColoR table[], size_t table_size,
                            const Nlm_Char PNTR filename);
#define UpdateColorTable Nlm_UpdateColorTable


/* [WIN_GIF] Create a GIF image with the given attributes, and set it as
 * the current drawable.
 */
Nlm_Boolean Nlm_CreateGIF(Nlm_Int2 width, Nlm_Int2 height,
                          Nlm_Boolean transparent);
#define CreateGIF Nlm_CreateGIF

/* [WIN_GIF] Save current GIF drawable to output stream "out".
 */
Nlm_Boolean Nlm_SaveGIF(FILE* out);
#define SaveGIF Nlm_SaveGIF

/* [WIN_GIF] Destroy current GIF drawable (if any).
 */
void Nlm_DestroyGIF(void);
#define DestroyGIF Nlm_DestroyGIF

/* [WIN_GIF] Make GIF image "im" be the current drawable.
 * Return the previous one.
 */
struct gdImageStruct* Nlm_SetCurrentGIF(struct gdImageStruct* im);
#define SetCurrentGIF Nlm_SetCurrentGIF


/* If the application is using GUI or just drawing(picture) functionality */
Nlm_Boolean Nlm_VibrantIsGUI(void);
#define VibrantIsGUI Nlm_VibrantIsGUI

#ifdef __cplusplus
}
#endif

#endif

