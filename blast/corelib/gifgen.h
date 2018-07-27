/*   gifgen.h
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
*/


/* +-------------------------------------------------------------------+ */
/* | Copyright 1990, 1991, 1993, David Koblas.  (koblas@netcom.com)    | */
/* |   Permission to use, copy, modify, and distribute this software   | */
/* |   and its documentation for any purpose and without fee is hereby | */
/* |   granted, provided that the above copyright notice appear in all | */
/* |   copies and that both that copyright notice and this permission  | */
/* |   notice appear in supporting documentation.  This software is    | */
/* |   provided "as is" without express or implied warranty.           | */
/* +-------------------------------------------------------------------+ */

/*
 * File Name:  gifgen.c
 *
 * Author:  Alex Smirnov
 *
 * Version Creation Date:   10/20/95
 *
 * File Description:
 *       GIF generator header file
 *
 * $Log: gifgen.h,v $
 * Revision 6.9  1999/10/26 15:47:35  vakatov
 * + gdImageGifEx(), gdFWrite -- to specify an alternative GIF write func
 * (with V.Chetvernin)
 *
 * Revision 6.8  1999/10/20 19:49:21  vakatov
 * + gdImageGetDimensions()
 *
 * Revision 6.7  1999/08/13 21:23:09  vakatov
 * Renamed "gd[Set|Get]ImageColor()" to "gdImage[Set|Get]Color()"
 *
 * Revision 6.6  1999/08/13 20:55:28  vakatov
 * + gdSetImageColor()
 * + gdImageGetAutoCropRectangle(), gdImageCrop(), gdImageAutoCrop()  <in
 * a close collab. with S.Resenchuk & R.Tatusov>
 *
 * Revision 6.5  1998/06/15 22:02:54  vakatov
 * Added gdImageSetClip() to allow clipping(rectangular area only)
 *
 * Revision 6.4  1998/04/27 15:52:54  vakatov
 * Added gdImageRoundRectangle() and gdImageQuadrant() to draw a rounded
 * rectangle and a quarter of an ellipse respectively
 *
 * Revision 6.3  1998/04/08 19:53:51  brandon
 * Added gdImageCopy() and gdImageCopyResized() from gd1.2
 *
 * Revision 6.2  1998/03/30 18:40:51  kans
 * needs to include ncbistd.h to define Nlm_Boolean
 *
 * Revision 6.1  1998/03/30 16:52:15  vakatov
 * Added gdImageArcEx() and gdImageEllipse()
 *
 * Revision 6.0  1997/08/25 18:15:08  madden
 * Revision changed to 6.0
 *
 * Revision 5.5  1997/08/19 19:26:46  vakatov
 * Added gdImageSelectPattern() to provide Nlm_SelectPattern()
 * functionality for GIFs
 *
 * Revision 5.4  1997/03/18 20:10:50  shavirin
 * Added protection for usage with C++
 *
 * Revision 5.3  1996/12/03  21:48:33  vakatov
 * Adopted for 32-bit MS-Windows DLLs
 *
 * Revision 5.2  1996/11/22  19:45:25  vakatov
 * Added prototypes for gdImageCreateFromGif() and gdGetImageColor()
 *
 * Revision 5.1  1996/11/12  15:39:33  vakatov
 * Added prototypes of the vertical text printing routines:  gdImageCharV()
 * and gdImageStringV() -- [by R.Tatusov]
 *
 * Revision 5.0  1996/05/28  13:18:57  ostell
 * Set to revision 5.0
 *
 * Revision 1.2  1996/04/19  14:17:56  vakatov
 * Inserted non-NCBI copyright headers.
 * Added VC log.
 *
 *
 * ==========================================================================
 */


#ifndef GIFGEN_H
#define GIFGEN_H 1

#include <ncbistd.h>
#include <stdio.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************************/
/* DEFINES */
/**************************************************************************/
#define gdMaxColors     256
#define gdDashSize      4
#define gdStyled        (-2)
#define gdBrushed       (-3)
#define gdStyledBrushed (-4)
#define gdTiled         (-5)
#define gdTransparent   (-6)

#define gdImageSX(im)             ((im)->sx)
#define gdImageSY(im)             ((im)->sy)
#define gdImageColorsTotal(im)    ((im)->colorsTotal)
#define gdImageRed(im,c)          ((im)->red[(c)])
#define gdImageGreen(im,c)        ((im)->green[(c)])
#define gdImageBlue(im,c)         ((im)->blue[(c)])
#define gdImageGetTransparent(im) ((im)->transparent)
#define gdImageGetInterlaced(im)  ((im)->interlace)

/**************************************************************************/
/* TYPEDEFS */
/**************************************************************************/
typedef struct {
  int    nchars;  /* # of characters in font */
  int    offset;  /* First character is numbered... (usually 32 = space) */
  int    w;       /* Character width  */
  int    h;       /* Character height */
  int    d;       /* Character descent */
  char * data;    /* Font data; array of characters, one row after another.
                     Easily included in code, also easily loaded from
                     data files. */
} gdFont, *gdFontPtr;

typedef struct {
  int x, y;
} gdPoint, *gdPointPtr;

typedef struct {
  gdPoint lt;
  gdPoint rb;
} gdRect;


typedef struct gdImageStruct {
  unsigned char * pixels;
  int             sx;
  int             sy;
  int             colorsTotal;
  int             red[gdMaxColors];
  int             green[gdMaxColors];
  int             blue[gdMaxColors];
  int             open[gdMaxColors];
  int             transparent;
  int           * polyInts;
  int             polyAllocated;
  struct gdImageStruct * brush;
  struct gdImageStruct * tile;
  int             brushColorMap[gdMaxColors];
  int             tileColorMap[gdMaxColors];
  int             styleLength;
  int             stylePos;
  int           * style;
  int             interlace;
  int             use_pattern;
  unsigned char   pattern[8];
  int             use_clip;
  gdRect          clip, eff_clip;
} gdImage,  *gdImagePtr;


/* Functions to manipulate images. */
NLM_EXTERN gdImagePtr gdImageCreate         (int sx, int sy);
NLM_EXTERN gdImagePtr gdImageCreateFromGif  (FILE *fd);
NLM_EXTERN void       gdImageDestroy        (gdImagePtr im);
NLM_EXTERN void       gdImageSetPixel       (gdImagePtr im,
                                             int x, int y, int color);
NLM_EXTERN int        gdImageGetPixel       (gdImagePtr im, int x, int y);
NLM_EXTERN void       gdImageLine           (gdImagePtr im, int x1, int y1,
                                             int x2, int y2, int color);
NLM_EXTERN void       gdImageDashedLine     (gdImagePtr im, int x1, int y1,
                                             int x2, int y2, int color);
NLM_EXTERN void       gdImageRectangle      (gdImagePtr im, int x1, int y1,
                                             int x2, int y2, int color);
NLM_EXTERN void       gdImageFilledRectangle(gdImagePtr im, int x1, int y1,
                                             int x2, int y2, int color);
NLM_EXTERN void       gdImageRoundRectangle (gdImagePtr im, int x1, int y1,
                                             int x2, int y2,
                                             int oval_w, int oval_h,
                                             int color, Nlm_Boolean fill);
NLM_EXTERN int        gdImageBoundsSafe     (gdImagePtr im, int x, int y);
NLM_EXTERN void       gdImageChar           (gdImagePtr im, gdFontPtr f,
                                             int x, int y,
                                             int c, int color);
NLM_EXTERN void       gdImageCharV          (gdImagePtr im, gdFontPtr f,
                                             int x, int y,
                                             int c, int color);
NLM_EXTERN void       gdImageString         (gdImagePtr im, gdFontPtr f,
                                             int x, int y,
                                             char *s, int color);
NLM_EXTERN void       gdImageStringV        (gdImagePtr im, gdFontPtr f,
                                             int x, int y,
                                             char *s, int color);
NLM_EXTERN void       gdImagePolygon        (gdImagePtr im,
                                             gdPointPtr p, int n, int c);
NLM_EXTERN void       gdImageFilledPolygon  (gdImagePtr im,
                                             gdPointPtr p, int n, int c);
NLM_EXTERN int        gdImageColorAllocate  (gdImagePtr im,
                                             int r, int g, int b);

/* Do nothing and return FALSE if the "color" is bad or not allocated
 */
#define gdGetImageColor gdImageGetColor
NLM_EXTERN int        gdImageGetColor       (gdImagePtr im, int color,
                                             int *r, int *g, int *b);

/* Do nothing and return FALSE if the "color" is bad or not allocated
 */
NLM_EXTERN int        gdImageSetColor       (gdImagePtr im, int color,
                                             int r, int g, int b);

NLM_EXTERN int        gdImageColorClosest   (gdImagePtr im,
                                             int r, int g, int b);
NLM_EXTERN int        gdImageColorExact     (gdImagePtr im,
                                             int r, int g, int b);
NLM_EXTERN void       gdImageColorDeallocate   (gdImagePtr im, int color);
NLM_EXTERN void       gdImageColorTransparent  (gdImagePtr im, int color);

/* Write out "size" bytes starting from address "buf".
 * Return the # of succesfully written bytes. */
typedef size_t (*gdFWrite)(const void* buf, size_t size, void* userdata);

/* Write in GIF format using write callback "func"
 */
NLM_EXTERN void       gdImageGifEx          (gdImagePtr im,
                                             gdFWrite   func,
                                             void*      userdata);

/* Write the image to file stream "out" in GIF format
 */
NLM_EXTERN void       gdImageGif            (gdImagePtr im, FILE* out);


NLM_EXTERN void       gdImageArc            (gdImagePtr im, int cx, int cy,
                                             int w, int h,
                                             int s, int e, int color);
NLM_EXTERN void       gdImageArcEx          (gdImagePtr im, int cx, int cy,
                                             int rx, int ry,
                                             double s_angle, double e_angle,
                                             int color, Nlm_Boolean fill);
NLM_EXTERN void       gdImageEllipse        (gdImagePtr im, int cx, int cy,
                                             int rx, int ry, int color,
                                             Nlm_Boolean fill);
NLM_EXTERN void       gdImageQuadrant       (gdImagePtr im, int cx, int cy,
                                             int rx, int ry, int color,
                                             Nlm_Boolean fill, int quadrant);
NLM_EXTERN void       gdImageInterlace      (gdImagePtr im, int interlaceArg);
NLM_EXTERN void       gdImageCopyBits       (gdImagePtr im, int x, int y,
                                             int w, int h,
                                             char * ptr, int color);
NLM_EXTERN void       gdImageSelectPattern  (gdImagePtr im,
                                             const unsigned char pattern[]);

NLM_EXTERN void       gdImageCopy           (gdImagePtr dst,
                                             gdImagePtr src,
                                             int dstX, int dstY,
                                             int srcX, int srcY,
                                             int w, int h);
NLM_EXTERN void       gdImageCopyResized    (gdImagePtr dst,
                                             gdImagePtr src,
                                             int dstX, int dstY,
                                             int srcX, int srcY,
                                             int dstW, int dstH,
                                             int srcW, int srcH);

/* (return non-zero value if the image was clipped before)
 */
NLM_EXTERN int        gdImageSetClip        (gdImagePtr im,
                                             const gdRect *clip,
                                             gdRect *old_clip);


/* Get the image's width and height. ("width" and "height" can be NULLs).
 */
NLM_EXTERN void gdImageGetDimensions(gdImagePtr im, int* width, int* height);


/* Determine a minimal rectangular area "rect" inside image "im" such that
 * the area outside the "rect" contains only background pixels.
 * NOTE: if the whole image contains only background pixels then
 *       just return the image's boundaries.
 */
NLM_EXTERN void gdImageGetAutoCropRectangle(gdImagePtr im, gdRect* rect);


/* Inside the image "im", move the rectangular area "rect" to the left
 * top corner.
 * NOTE:  "rect" will be automagically normalized and fit the image boundaries.
 */
NLM_EXTERN void gdImageCrop(gdImagePtr im, const gdRect* rect);


/* Apply gdImageGetAutoCropRectangle(), then add no more than "border" pixels
 * to each side of the resultant rectangular area, and do gdImageCrop().
 */
NLM_EXTERN void gdImageAutoCrop(gdImagePtr im, int border);



/**************************************************************************/
/* GLOBAL VARIABLE */
/**************************************************************************/

extern gdFontPtr gdFont8X16;
extern gdFontPtr gdFont9X15b;
extern gdFontPtr gdFont7X13b;
extern gdFontPtr gdFont6X12;
extern gdFontPtr gdFont5X8;


#ifdef __cplusplus
}
#endif


#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* GIFGEN_H */
