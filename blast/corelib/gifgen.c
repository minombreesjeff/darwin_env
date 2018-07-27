/*   gifgen.c
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
 *
 * File Name:  gifgen.c
 *
 * Author:  Alex Smirnov, Denis Vakatov
 *
 * Version Creation Date:   10/20/95
 *
 * File Description: 
 *       GIF drawing functions
 *
 * $Log: gifgen.c,v $
 * Revision 6.15  2001/03/21 18:54:57  juran
 * Fix "for (...);" warnings by adding a space before the semicolon.
 *
 * Revision 6.14  2001/01/05 20:10:29  vakatov
 * s_ComposeEffClip() -- fixed a typo (".lt" -> ".rb")
 *
 * Revision 6.13  2000/04/06 17:12:54  chetvern
 * gdImageFilledPolygon
 * zero section width case fixed
 *
 * Revision 6.12  1999/10/20 19:48:39  vakatov
 * + gdImageGetDimensions()
 *
 * Revision 6.11  1999/08/13 21:23:08  vakatov
 * Renamed "gd[Set|Get]ImageColor()" to "gdImage[Set|Get]Color()"
 *
 * Revision 6.10  1999/08/13 20:57:40  vakatov
 * + gdSetImageColor()
 * + gdImageGetAutoCropRectangle(), gdImageCrop(), gdImageAutoCrop()  <in
 * a close collab. with S.Resenchuk & R.Tatusov>
 * + static s_*Rect() to handle "gdRect"
 *
 * Revision 6.9  1999/05/20 21:32:45  vakatov
 * s_DrawEllipse():  "ad hoc" patches for the case of zero radius.
 * [_DEBUG] s_DrawConic():  just do nothing(and dont crash) if obtained
 * a wrong octant
 *
 * Revision 6.8  1998/06/26 19:57:15  vakatov
 * gdImageRoundRectangle():  check and adjust the passed args
 *
 * Revision 6.7  1998/06/15 22:02:53  vakatov
 * Added gdImageSetClip() to allow clipping(rectangular area only)
 *
 * Revision 6.6  1998/05/08 21:25:29  vakatov
 * Fixed a typo in the IMAGE_BOUNDS_SAFE macro(Roman Tatusov)
 *
 * Revision 6.5  1998/04/27 15:52:53  vakatov
 * Added gdImageRoundRectangle() and gdImageQuadrant() to draw a rounded
 * rectangle and a quarter of an ellipse respectively
 *
 * Revision 6.4  1998/04/08 19:53:48  brandon
 * Added gdImageCopy() and gdImageCopyResized() from gd1.2
 *
 * Revision 6.3  1998/03/30 16:55:38  vakatov
 * Added s_DrawConic() -- the generic conic section drawing algorithm;
 * implemented gdImageArcEx() and gdImageEllipse()
 * NOTE:  "fill" argument of gdImageArcEx() intended to draw filled
 *        sector of an ellipse is not implemented yet
 *
 * Revision 6.2  1997/11/26 21:26:08  vakatov
 * Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
 *
 * Revision 6.1  1997/09/10 00:00:53  vakatov
 * Ignore current drawing pattern when drawing line-based primitives
 * (line, polygon and arc) -- use current line style only
 *
 * Revision 5.5  1997/08/19 19:26:43  vakatov
 * Added gdImageSelectPattern() to provide Nlm_SelectPattern()
 * functionality for GIFs
 *
 * Revision 5.4  1997/05/28 21:10:09  vakatov
 * gdGetImageColor() now accepts NULL as r,g,b w/out a crash
 *
 * Revision 5.3  1996/12/03 21:48:33  vakatov
 * Adopted for 32-bit MS-Windows DLLs
 *
 * Revision 5.2  1996/11/22  19:44:54  vakatov
 * Added auxiliary function gdGetImageColor()
 *
 * Revision 5.1  1996/11/12  15:38:17  vakatov
 * Added two routines for the vertical text printing:  gdImageCharV() and
 * gdImageStringV() -- [by R.Tatusov]
 *
 * Revision 1.4  1996/05/21  14:35:04  vakatov
 * "qsort()" function call has been replaced with the call of toolkit function
 * "HeapSort()" for the portability(VMS compiler) reason
 *
 * Revision 1.3  1996/04/23  21:33:07  kans
 * includes ncbi.h, uses Nlm_Malloc instead of malloc, etc.
 */


/**************************************************************************/
/* INCLUDE */
/**************************************************************************/
#include <ncbi.h>
#include <gifgen.h>


/**************************************************************************/
/* DEFINES */
/**************************************************************************/
#define costScale 1024
#define sintScale 1024

#define IMAGE_BOUNDS_SAFE(im, X, Y) \
((X) >= im->eff_clip.lt.x  &&  (Y) >= im->eff_clip.lt.y  && \
 (X) <  im->eff_clip.rb.x  &&  (Y) <  im->eff_clip.rb.y)


#define directSet(im, x, y, color) \
{ \
  if ( IMAGE_BOUNDS_SAFE(im, x, y) ) \
    im->pixels[x+y*im->sx] = (Uint1)color; \
}
   
#define dashedSet \
{ \
  dashStep++; \
  if (dashStep == gdDashSize) { \
    dashStep = 0; \
    on = !on; \
  } \
  if (on) { \
    directSet(im, x, y, color); \
  } \
}


/**************************************************************************/
/* TYPEDEFS */
/**************************************************************************/

/**************************************************************************/
/* STATIC VARIABLE */
/**************************************************************************/
static const int cost[] = {
  1024, 1023, 1023, 1022, 1021, 1020, 1018,
  1016, 1014, 1011, 1008, 1005, 1001, 997,
  993, 989, 984, 979, 973, 968, 962,
  955, 949, 942, 935, 928, 920, 912,
  904, 895, 886, 877, 868, 858, 848,
  838, 828, 817, 806, 795, 784, 772,
  760, 748, 736, 724, 711, 698, 685,
  671, 658, 644, 630, 616, 601, 587,
  572, 557, 542, 527, 512, 496, 480,
  464, 448, 432, 416, 400, 383, 366,
  350, 333, 316, 299, 282, 265, 247,
  230, 212, 195, 177, 160, 142, 124,
  107, 89, 71, 53, 35, 17, 0,
  -17, -35, -53, -71, -89, -107, -124,
  -142, -160, -177, -195, -212, -230, -247,
  -265, -282, -299, -316, -333, -350, -366,
  -383, -400, -416, -432, -448, -464, -480,
  -496, -512, -527, -542, -557, -572, -587,
  -601, -616, -630, -644, -658, -671, -685,
  -698, -711, -724, -736, -748, -760, -772,
  -784, -795, -806, -817, -828, -838, -848,
  -858, -868, -877, -886, -895, -904, -912,
  -920, -928, -935, -942, -949, -955, -962,
  -968, -973, -979, -984, -989, -993, -997,
  -1001, -1005, -1008, -1011, -1014, -1016, -1018,
  -1020, -1021, -1022, -1023, -1023, -1024, -1023,
  -1023, -1022, -1021, -1020, -1018, -1016, -1014,
  -1011, -1008, -1005, -1001, -997, -993, -989,
  -984, -979, -973, -968, -962, -955, -949,
  -942, -935, -928, -920, -912, -904, -895,
  -886, -877, -868, -858, -848, -838, -828,
  -817, -806, -795, -784, -772, -760, -748,
  -736, -724, -711, -698, -685, -671, -658,
  -644, -630, -616, -601, -587, -572, -557,
  -542, -527, -512, -496, -480, -464, -448,
  -432, -416, -400, -383, -366, -350, -333,
  -316, -299, -282, -265, -247, -230, -212,
  -195, -177, -160, -142, -124, -107, -89,
  -71, -53, -35, -17, 0, 17, 35,
  53, 71, 89, 107, 124, 142, 160,
  177, 195, 212, 230, 247, 265, 282,
  299, 316, 333, 350, 366, 383, 400,
  416, 432, 448, 464, 480, 496, 512,
  527, 542, 557, 572, 587, 601, 616,
  630, 644, 658, 671, 685, 698, 711,
  724, 736, 748, 760, 772, 784, 795,
  806, 817, 828, 838, 848, 858, 868,
  877, 886, 895, 904, 912, 920, 928,
  935, 942, 949, 955, 962, 968, 973,
  979, 984, 989, 993, 997, 1001, 1005,
  1008, 1011, 1014, 1016, 1018, 1020, 1021,
  1022, 1023, 1023 
};

static const int sint[] = { 
  0, 17, 35, 53, 71, 89, 107,
  124, 142, 160, 177, 195, 212, 230,
  247, 265, 282, 299, 316, 333, 350,
  366, 383, 400, 416, 432, 448, 464,
  480, 496, 512, 527, 542, 557, 572,
  587, 601, 616, 630, 644, 658, 671,
  685, 698, 711, 724, 736, 748, 760,
  772, 784, 795, 806, 817, 828, 838,
  848, 858, 868, 877, 886, 895, 904,
  912, 920, 928, 935, 942, 949, 955,
  962, 968, 973, 979, 984, 989, 993,
  997, 1001, 1005, 1008, 1011, 1014, 1016,
  1018, 1020, 1021, 1022, 1023, 1023, 1024,
  1023, 1023, 1022, 1021, 1020, 1018, 1016,
  1014, 1011, 1008, 1005, 1001, 997, 993,
  989, 984, 979, 973, 968, 962, 955,
  949, 942, 935, 928, 920, 912, 904,
  895, 886, 877, 868, 858, 848, 838,
  828, 817, 806, 795, 784, 772, 760,
  748, 736, 724, 711, 698, 685, 671,
  658, 644, 630, 616, 601, 587, 572,
  557, 542, 527, 512, 496, 480, 464,
  448, 432, 416, 400, 383, 366, 350,
  333, 316, 299, 282, 265, 247, 230,
  212, 195, 177, 160, 142, 124, 107,
  89, 71, 53, 35, 17, 0, -17,
  -35, -53, -71, -89, -107, -124, -142,
  -160, -177, -195, -212, -230, -247, -265,
  -282, -299, -316, -333, -350, -366, -383,
  -400, -416, -432, -448, -464, -480, -496,
  -512, -527, -542, -557, -572, -587, -601,
  -616, -630, -644, -658, -671, -685, -698,
  -711, -724, -736, -748, -760, -772, -784,
  -795, -806, -817, -828, -838, -848, -858,
  -868, -877, -886, -895, -904, -912, -920,
  -928, -935, -942, -949, -955, -962, -968,
  -973, -979, -984, -989, -993, -997, -1001,
  -1005, -1008, -1011, -1014, -1016, -1018, -1020,
  -1021, -1022, -1023, -1023, -1024, -1023, -1023,
  -1022, -1021, -1020, -1018, -1016, -1014, -1011,
  -1008, -1005, -1001, -997, -993, -989, -984,
  -979, -973, -968, -962, -955, -949, -942,
  -935, -928, -920, -912, -904, -895, -886,
  -877, -868, -858, -848, -838, -828, -817,
  -806, -795, -784, -772, -760, -748, -736,
  -724, -711, -698, -685, -671, -658, -644,
  -630, -616, -601, -587, -572, -557, -542,
  -527, -512, -496, -480, -464, -448, -432,
  -416, -400, -383, -366, -350, -333, -316,
  -299, -282, -265, -247, -230, -212, -195,
  -177, -160, -142, -124, -107, -89, -71,
  -53,  -35,  -17
};

/**************************************************************************/
/* STATIC FUNCTION */
/**************************************************************************/


static void s_NormalizeRect(gdRect* rect)
{
  if (rect->lt.x > rect->rb.x) {
    int x = rect->lt.x;
    rect->lt.x = rect->rb.x;
    rect->rb.x = x;
  }

  if (rect->lt.y > rect->rb.y) {
    int y = rect->lt.y;
    rect->lt.y = rect->rb.y;
    rect->rb.y = y;
  }
}


static void s_ExpandRect(gdRect* rect, int dX, int dY)
{
  s_NormalizeRect(rect);
  rect->lt.x -= dX;
  rect->rb.x += dX;
  rect->lt.y -= dY;
  rect->rb.y += dY;

  if (rect->lt.x > rect->rb.x)
    rect->lt.x = rect->rb.x = (rect->lt.x + rect->rb.x) / 2;
  if (rect->lt.y > rect->rb.y)
    rect->lt.y = rect->rb.y = (rect->lt.y + rect->rb.y) / 2;
}


static void s_CropRect(gdRect* rect, const gdRect* crop)
{
  s_NormalizeRect(rect);

  if (rect->lt.x < crop->lt.x)
    rect->lt.x = crop->lt.x;
  if (rect->rb.x > crop->rb.x)
    rect->rb.x = crop->rb.x;

  if (rect->lt.y < crop->lt.y)
    rect->lt.y = crop->lt.y;
  if (rect->rb.y > crop->rb.y)
    rect->rb.y = crop->rb.y;
  
}


static void s_GetRectImage(gdRect* rect, const gdImage* im)
{
  rect->lt.x = 0;
  rect->lt.y = 0;
  rect->rb.x = (im ? im->sx : INT2_MAX) - 1;
  rect->rb.y = (im ? im->sy : INT2_MAX) - 1;
}


static void s_CropRectImage(gdRect* rect, const gdImage* im)
{
  gdRect crop;
  s_GetRectImage(&crop, im);
  s_CropRect(rect, &crop);
}



static void 
/*FCN*/gdImageBrushApply(gdImagePtr im, int x, int y)
{
   int lx, ly;
   int hy;
   int hx;
   int x1, y1, x2, y2;
   int srcx, srcy;

   if (!im->brush) {
      return;
   }
   hy = gdImageSY(im->brush)/2;
   y1 = y - hy;
   y2 = y1 + gdImageSY(im->brush);   
   hx = gdImageSX(im->brush)/2;
   x1 = x - hx;
   x2 = x1 + gdImageSX(im->brush);
   srcy = 0;
   for (ly = y1; (ly < y2); ly++) {
      srcx = 0;
      for (lx = x1; (lx < x2); lx++) {
         int p;
         p = gdImageGetPixel(im->brush, srcx, srcy);
         /* Allow for non-square brushes! */
         if (p != gdImageGetTransparent(im->brush)) {
            gdImageSetPixel(im, lx, ly,
               im->brushColorMap[p]);
         }
         srcx++;
      }
      srcy++;
   }   
}      

static void 
/*FCN*/gdImageTileApply(gdImagePtr im, int x, int y)
{
   int srcx, srcy;
   int p;

   if (!im->tile) {
      return;
   }
   srcx = x % gdImageSX(im->tile);
   srcy = y % gdImageSY(im->tile);
   p = gdImageGetPixel(im->tile, srcx, srcy);
   /* Allow for transparency */
   if (p != gdImageGetTransparent(im->tile)) {
      gdImageSetPixel(im, x, y,
         im->tileColorMap[p]);
   }
}      

#ifdef __cplusplus
extern "C" {
  static int LIBCALLBACK gdCompareInt(Nlm_VoidPtr a, Nlm_VoidPtr b);
}
#endif

static 
/*FCN*/int LIBCALLBACK gdCompareInt(Nlm_VoidPtr a, Nlm_VoidPtr b)
{
   return (*(const int *)a) - (*(const int *)b);
}

/* s_DrawConic():
 *
 * CONIC  2D Bresenham-like conic drawer.
 *       CONIC(Sx,Sy, Ex,Ey, A,B,C,D,E,F) draws the conic specified
 *       by A x^2 + B x y + C y^2 + D x + E y + F = 0, between the
 *       start point (Sx, Sy) and endpoint (Ex,Ey).
 *
 * Author: Andrew W. Fitzgibbon (andrewfg@ed.ac.uk),
 *         Machine Vision Unit,
 *         Dept. of Artificial Intelligence,
 *         Edinburgh University,
 * 
 * Date: 31-Mar-94
 * Version 2: 6-Oct-95
 *      Bugfixes from Arne Steinarson <arst@ludd.luth.se>
 *      (ftp://ftp.dai.ed.ac.uk/pub/vision/src/conic-patch1.[ch])
 * Adopted and "beautified" for the NCBI toolkit by D.Vakatov, Feb-98;
 * and, s_GetOctant() rewritten to fix at the octant boundary points
 */


static int s_GetOctant(int gx, int gy)
{
  static const int OCTANT[3 /*gX?0*/][3 /*gY?0*/][3 /*|gX|?|gY|*/] = {
    /***********  gY < 0 *** gY = 0 *** gY > 0 */ 
    /* gX < 0 */ 8, 8, 7,   0, 0, 7,   5, 6, 6,
    /* gX = 0 */ 1, 0, 0,   0, 0, 0,   5, 0, 0,
    /* gX > 0 */ 1, 2, 2,   0, 0, 3,   4, 4, 3
  };

#define A_VS_B(a,b) (a < b) ? 0 : (a == b) ? 1 : 2
  int Xvs0 = A_VS_B(gx, 0);
  int Yvs0 = A_VS_B(gy, 0);
  if (gx < 0)
    gx = -gx;
  if (gy < 0)
    gy = -gy;

  /* ASSERT ( 1 <= OCTANT[Xvs0][Yvs0][A_VS_B(gx, gy)] ); */
  /* ASSERT ( 8 >= OCTANT[Xvs0][Yvs0][A_VS_B(gx, gy)] ); */
  return OCTANT[Xvs0][Yvs0][A_VS_B(gx, gy)];
}


typedef void (*Nlm_DrawPoint)(int x, int y, void *data);

static void s_DrawConic
(/* Axx + Bxy + Cyy + Dx + Ey + F = 0 */
 int A, int B, int C, int D, int E, int F,
 int xs, int ys, /* starting point */
 int xe, int ye, /* ending point   */
 /* call "draw_func" to draw a point */
 Nlm_DrawPoint draw_func, void *draw_data)
{
  static const int DIAGx[] = {999, 1,  1, -1, -1, -1, -1,  1,  1};
  static const int DIAGy[] = {999, 1,  1,  1,  1, -1, -1, -1, -1};
  static const int SIDEx[] = {999, 1,  0,  0, -1, -1,  0,  0,  1};
  static const int SIDEy[] = {999, 0,  1,  1,  0,  0, -1, -1,  0};

  int octant;
  int dxS, dyS, dxD, dyD;
  int d,u,v;
  int k1sign, k1, Bsign, k2, k3;
  int gxe, gye, gx, gy;
  int octantcount;
  int x, y;

  if (xs == xe  &&  ys == ye)
    return;

  A *= 4;  B *= 4;  C *= 4;  D *= 4;  E *= 4;  F *= 4;

  /* Translate start point to origin... */
  F = A*xs*xs + B*xs*ys + C*ys*ys + D*xs + E*ys + F;
  D = D + 2*A*xs + B*ys;
  E = E + B*xs + 2*C*ys;
  
  /* Work out starting octant */
  octant = s_GetOctant(D, E);
  
  dxS = SIDEx[octant]; 
  dyS = SIDEy[octant]; 
  dxD = DIAGx[octant];
  dyD = DIAGy[octant];

  switch ( octant ) {
  case 1:
    d = A + B/2 + C/4 + D + E/2 + F;
    u = A + B/2 + D;
    v = u + E;
    break;
  case 2:
    d = A/4 + B/2 + C + D/2 + E + F;
    u = B/2 + C + E;
    v = u + D;
    break;
  case 3:
    d = A/4 - B/2 + C - D/2 + E + F;
    u = -B/2 + C + E;
    v = u - D;
    break;
  case 4:
    d = A - B/2 + C/4 - D + E/2 + F;
    u = A - B/2 - D;
    v = u + E;
    break;
  case 5:
    d = A + B/2 + C/4 - D - E/2 + F;
    u = A + B/2 - D;
    v = u - E;
    break;
  case 6:
    d = A/4 + B/2 + C - D/2 - E + F;
    u = B/2 + C - E;
    v = u - D;
    break;
  case 7:
    d = A/4 - B/2 + C + D/2 - E + F;
    u =  -B/2 + C - E;
    v = u + D;
    break;
  case 8:
    d = A - B/2 + C/4 + D - E/2 + F;
    u = A - B/2 + D;
    v = u - E;
    break;
  default:
    /* ASSERT ( FALSE );  cannot be drawn by this algorithm */
    return;
  }
  
  k1sign = dyS * dyD;
  k1 = 2 * (A + k1sign * (C - A));
  Bsign = dxD * dyD;
  k2 = k1 + Bsign * B;
  k3 = 2 * (A + C + Bsign * B);

  /* Work out gradient at endpoint */
  gxe = xe - xs;
  gye = ye - ys;
  gx = 2*A*gxe +   B*gye + D;
  gy =   B*gxe + 2*C*gye + E;
  
  octantcount = s_GetOctant(gx, gy) - octant;
  if (octantcount < 0  ||
      ((octantcount == 0)  &&
       ((xs > xe  &&  dxD > 0)  ||  (ys > ye  &&  dyD > 0)  ||
        (xs < xe  &&  dxD < 0)  ||  (ys < ye  &&  dyD < 0))))
    octantcount += 8;

  x = xs;
  y = ys;
  
  while (octantcount > 0) {
    if (octant & 1) { /* Octant is odd */
      while (2*v <= k2) {
        draw_func(x, y, draw_data);
        
        if (d < 0) { /* inside */
          x += dxS;
          y += dyS;
          u += k1;
          v += k2;
          d += u;
        }
        else { /* outside */
          x += dxD;
          y += dyD;
          u += k2;
          v += k3;
          d += v;
        }
      }
    
      d = d - u + v/2 - k2/2 + 3*k3/8; 
      u = -u + v - k2/2 + k3/2;
      v = v - k2 + k3/2;
      k1 = k1 - 2*k2 + k3;
      k2 = k3 - k2;
      {{ int tmp = dxS;  dxS = -dyS;  dyS = tmp; }}
    }
    else { /* Octant is even */
      while (2*u < k2) {
        draw_func(x, y, draw_data);

        if (d > 0) { /* outside */
          x += dxS;
          y += dyS;
          u += k1;
          v += k2;
          d += u;
        }
        else { /* inside */
          x += dxD;
          y += dyD;
          u += k2;
          v += k3;
          d += v;
        }
      }

      {{
        int tmpdk = k1 - k2;
        d = d + u - v + tmpdk;
        v = 2*u - v + tmpdk;
        u = u + tmpdk;
        k3 = k3 + 4*tmpdk;
        k2 = k1 + tmpdk;
        {{ int tmp = dxD;  dxD = -dyD;  dyD = tmp; }}
      }}
    }
    
    octant = (octant & 7) + 1;
    octantcount--;
  }

  /* Draw final octant until we reach the endpoint */
  if (octant & 1) {  /* Octant is odd */
    while (2*v <= k2) {
      draw_func(x, y, draw_data);
      if (x == xe  &&  y == ye)
        break;

      if (d < 0) { /* inside */
        x += dxS;
        y += dyS;
        u += k1;
        v += k2;
        d += u;
      }
      else { /* outside */
        x += dxD;
        y += dyD;
        u += k2;
        v += k3;
        d += v;
      }
    }
  }
  else { /* Octant is even */
    while ((2*u < k2)) {
      draw_func(x, y, draw_data);
      if (x == xe  &&  y == ye)
        break;

      if (d > 0) { /* outside */
        x += dxS;
        y += dyS;
        u += k1;
        v += k2;
        d += u;
      }
      else { /* inside */
        x += dxD;
        y += dyD;
        u += k2;
        v += k3;
        d += v;
      }
    }
  }
}

typedef struct {
  gdImage *im;
  int color;
} SDrawPointArc;

static void s_DrawPointArc(int x, int y, void *data)
{
  SDrawPointArc *dpa = (SDrawPointArc *)data;
  directSet(dpa->im, x, y, dpa->color);
}


typedef struct {
  int x;
  int y;
} SPoint;

typedef struct {
  int xdim;
  int ydim;
  int n;
  SPoint *points;
} SStorePoint;

static void s_StorePoint(int x, int y, void *data)
{
  SStorePoint *sp = (SStorePoint *)data;
  if (x < 0  ||  y < 0  ||  sp->xdim <= x  ||  sp->ydim <= y)
    return;

  sp->points[sp->n].x = x;
  sp->points[sp->n].y = y;
  sp->n++;
}


typedef struct {
  gdImage *im;
  int color;
  int cx;
  int cy;
  int prev_y;    /* for fill only */
  int quadrant; /* 0 -- all quadrants;  1 -- right top;  2,3,4 -- clockwise */
} SDrawPointEllipse;


static void s_DrawPointEllipse(int x, int y, void *data)
{
  SDrawPointEllipse *dpe = (SDrawPointEllipse *)data;
  int xl = dpe->cx - x;
  int xr = dpe->cx + x;
  int yt = dpe->cy - y;
  int yb = dpe->cy + y;

  switch ( dpe->quadrant ) {
  case 0:
    if ( x ) {
      directSet(dpe->im, xl, yb, dpe->color);
      directSet(dpe->im, xr, yt, dpe->color);
    }
    if ( y ) {
      directSet(dpe->im, xl, yt, dpe->color);
      directSet(dpe->im, xr, yb, dpe->color);
    }
    break;
  case 1:
    directSet(dpe->im, xr, yt, dpe->color);
    break;
  case 2:
    directSet(dpe->im, xl, yt, dpe->color);
    break;
  case 3:
    directSet(dpe->im, xl, yb, dpe->color);
    break;
  case 4:
    directSet(dpe->im, xr, yb, dpe->color);
    break;
  default:
    ASSERT(0);
  }
}


static void s_ImageHLine(gdImagePtr im, int y, int x1, int x2, int color)
{
  int x;
  if (y < 0  ||  im->sy <= y)
    return;
  if (x1 > x2) {
    x = x1;  x1 = x2;  x2 = x;
  }
  if (x2 < 0  ||  im->sx <= x1)
    return;

  x1 = MAX(x1, 0);
  x2++;
  x2 = MIN(x2, im->sx);

  for (x = x1;  x < x2;  x++)
    gdImageSetPixel(im, x, y, color);
}


static void s_DrawPointFilledEllipse(int x, int y, void *data)
{
  SDrawPointEllipse *dpe = (SDrawPointEllipse *)data;
  if (y == dpe->prev_y)
    return;

  switch ( dpe->quadrant ) {
  case 0:
    s_ImageHLine(dpe->im, dpe->cy + y, dpe->cx - x, dpe->cx + x, dpe->color);
    if ( y )
      s_ImageHLine(dpe->im, dpe->cy - y, dpe->cx - x, dpe->cx + x, dpe->color);
    break;
  case 1:
    s_ImageHLine(dpe->im, dpe->cy - y, dpe->cx, dpe->cx + x, dpe->color);
    break;
  case 2:
    s_ImageHLine(dpe->im, dpe->cy - y, dpe->cx - x, dpe->cx, dpe->color);
    break;
  case 3:
    s_ImageHLine(dpe->im, dpe->cy + y, dpe->cx - x, dpe->cx, dpe->color);
    break;
  case 4:
    s_ImageHLine(dpe->im, dpe->cy + y, dpe->cx, dpe->cx + x, dpe->color);
    break;
  default:
    ASSERT(0);
  }
  
  dpe->prev_y = y;
}


static void s_ComposeEffClip(gdImagePtr im)
{
  if ( !im->use_clip ) {
    im->eff_clip.lt.x = im->eff_clip.lt.y = 0;
    im->eff_clip.rb.x = im->sx;
    im->eff_clip.rb.y = im->sy;
    return;
  }

  im->eff_clip.lt.x = MAX(im->clip.lt.x, 0);
  im->eff_clip.lt.y = MAX(im->clip.lt.y, 0);
  im->eff_clip.rb.x = MIN(im->clip.rb.x + 1, im->sx);
  im->eff_clip.rb.y = MIN(im->clip.rb.y + 1, im->sy);
}


/**************************************************************************/
/* GLOBAL FUNCTIONS */
/**************************************************************************/

NLM_EXTERN gdImagePtr 
/*FCN*/gdImageCreate(int sx, int sy)
{
   gdImagePtr im;

   im = (gdImage *) Nlm_Malloc(sizeof(gdImage));
   im->pixels = (unsigned char *) Nlm_Calloc( sx * sy, 1 );
   im->polyInts = 0;
   im->polyAllocated = 0;
   im->brush = 0;
   im->tile = 0;
   im->style = 0;
   im->sx = sx;
   im->sy = sy;
   im->colorsTotal = 0;
   im->transparent = (-1);
   im->interlace = 0;
   im->use_pattern = 0;
   im->use_clip = 0;
   s_ComposeEffClip(im);
   return im;
}

NLM_EXTERN void 
/*FCN*/gdImageDestroy(gdImagePtr im)
{
   Nlm_Free(im->pixels);
   if (im->polyInts) {
         Nlm_Free(im->polyInts);
   }
   if (im->style) {
      Nlm_Free(im->style);
   }
   Nlm_Free(im);
}

NLM_EXTERN int 
/*FCN*/gdImageColorClosest(gdImagePtr im, int r, int g, int b)
{
   int i;
   long rd, gd, bd;
   int ct = (-1);
   long mindist = 0;

   for (i=0; (i<(im->colorsTotal)); i++) {
      long dist;
      if (im->open[i]) {
         continue;
      }
      rd = (im->red[i] - r);   
      gd = (im->green[i] - g);
      bd = (im->blue[i] - b);
      dist = rd * rd + gd * gd + bd * bd;
      if ((i == 0) || (dist < mindist)) {
         mindist = dist;   
         ct = i;
      }
   }
   return ct;
}

NLM_EXTERN int 
/*FCN*/gdImageColorExact(gdImagePtr im, int r, int g, int b)
{
   int i;

   for (i=0; (i<(im->colorsTotal)); i++) {
      if (im->open[i]) {
         continue;
      }
      if ((im->red[i] == r) && 
         (im->green[i] == g) &&
         (im->blue[i] == b)) {
         return i;
      }
   }
   return -1;
}

NLM_EXTERN int 
/*FCN*/gdImageColorAllocate(gdImagePtr im, int r, int g, int b)
{
   int i;
   int ct = (-1);

   for (i=0; (i<(im->colorsTotal)); i++) {
      if (im->open[i]) {
         ct = i;
         break;
      }
   }   
   if (ct == (-1)) {
      ct = im->colorsTotal;
      if (ct == gdMaxColors) {
         return -1;
      }
      im->colorsTotal++;
   }
   im->red[ct] = r;
   im->green[ct] = g;
   im->blue[ct] = b;
   im->open[ct] = 0;
   return ct;
}

NLM_EXTERN void 
/*FCN*/gdImageColorDeallocate(gdImagePtr im, int color)
{
   im->open[color] = 1;
}

NLM_EXTERN int 
/*FCN*/gdImageGetColor(gdImagePtr im, int color, int *r, int *g, int *b)
{
  ASSERT ( color >= 0 );
  if (color < 0  ||  color >= im->colorsTotal  ||  im->open[color])
    return FALSE;

  if ( r )
    *r = im->red  [color];
  if ( g )
    *g = im->green[color];
  if ( b )
    *b = im->blue [color];
  return TRUE;
}


NLM_EXTERN int 
/*FCN*/gdImageSetColor(gdImagePtr im, int color, int r, int g, int b)
{
  ASSERT ( color >= 0 );
  if (color < 0  ||  color >= im->colorsTotal  ||  im->open[color])
    return FALSE;

  im->red  [color] = r;
  im->green[color] = g;
  im->blue [color] = b;
  return TRUE;
}


NLM_EXTERN void 
/*FCN*/gdImageColorTransparent(gdImagePtr im, int color)
{
   im->transparent = color;
}

NLM_EXTERN void 
/*FCN*/gdImageSetPixel(gdImagePtr im, int x, int y, int color)
{
   int p;

   switch(color) {
     case gdStyled:
      if (!im->style) {
         /* Refuse to draw if no style is set. */
         return;
      } else {
         p = im->style[im->stylePos++];
      }
      if (p != (gdTransparent)) {
         gdImageSetPixel(im, x, y, p);
      }
      im->stylePos = im->stylePos %  im->styleLength;
      break;
    case gdStyledBrushed:
      if (!im->style) {
         /* Refuse to draw if no style is set. */
         return;
      }
      p = im->style[im->stylePos++];
      if ((p != gdTransparent) && (p != 0)) {
         gdImageSetPixel(im, x, y, gdBrushed);
      }
      im->stylePos = im->stylePos %  im->styleLength;
      break;
    case gdBrushed:
      gdImageBrushApply(im, x, y);
      break;
    case gdTiled:
      gdImageTileApply(im, x, y);
      break;
    default:
      if (IMAGE_BOUNDS_SAFE(im, x, y)  &&
          (!im->use_pattern  ||  (char)(im->pattern[7-y%8] >> x%8 << 7)))
        im->pixels[x+y*im->sx] = (Uint1)color;
      break;
   }
}

NLM_EXTERN int 
/*FCN*/gdImageGetPixel(gdImagePtr im, int x, int y)
{
   if (IMAGE_BOUNDS_SAFE(im, x, y)) {
      return im->pixels[x+y*im->sx];
   } else {
      return 0;
   }
}

NLM_EXTERN void 
/*FCN*/gdImageLine(gdImagePtr im, int x1, int y1, int x2, int y2, int color)
{
   int dx, dy, incr1, incr2, d, x, y, xend, yend, xdirflag, ydirflag;

   dx = abs(x2-x1);
   dy = abs(y2-y1);
   if (dy <= dx) {
      d = 2*dy - dx;
      incr1 = 2*dy;
      incr2 = 2 * (dy - dx);
      if (x1 > x2) {
         x = x2;
         y = y2;
         ydirflag = (-1);
         xend = x1;
      } else {
         x = x1;
         y = y1;
         ydirflag = 1;
         xend = x2;
      }
      directSet(im, x, y, color);
      if (((y2 - y1) * ydirflag) > 0) {
         while (x < xend) {
            x++;
            if (d < 0) {
               d+=incr1;
            } else {
               y++;
               d+=incr2;
            }
            directSet(im, x, y, color);
         }
      } else {
         while (x < xend) {
            x++;
            if (d < 0) {
               d+=incr1;
            } else {
               y--;
               d+=incr2;
            }
            directSet(im, x, y, color);
         }
      }      
   } else {
      d = 2*dx - dy;
      incr1 = 2*dx;
      incr2 = 2 * (dx - dy);
      if (y1 > y2) {
         y = y2;
         x = x2;
         yend = y1;
         xdirflag = (-1);
      } else {
         y = y1;
         x = x1;
         yend = y2;
         xdirflag = 1;
      }
      directSet(im, x, y, color);
      if (((x2 - x1) * xdirflag) > 0) {
         while (y < yend) {
            y++;
            if (d < 0) {
               d+=incr1;
            } else {
               x++;
               d+=incr2;
            }
            directSet(im, x, y, color);
         }
      } else {
         while (y < yend) {
            y++;
            if (d < 0) {
               d+=incr1;
            } else {
               x--;
               d+=incr2;
            }
            directSet(im, x, y, color);
         }
      }
   }
}

NLM_EXTERN void 
/*FCN*/gdImageDashedLine(gdImagePtr im, int x1, int y1, int x2, int y2, int color)
{
   int dx, dy, incr1, incr2, d, x, y, xend, yend, xdirflag, ydirflag;
   int dashStep = 0;
   int on = 1;

   dx = abs(x2-x1);
   dy = abs(y2-y1);
   if (dy <= dx) {
      d = 2*dy - dx;
      incr1 = 2*dy;
      incr2 = 2 * (dy - dx);
      if (x1 > x2) {
         x = x2;
         y = y2;
         ydirflag = (-1);
         xend = x1;
      } else {
         x = x1;
         y = y1;
         ydirflag = 1;
         xend = x2;
      }
      dashedSet;
      if (((y2 - y1) * ydirflag) > 0) {
         while (x < xend) {
            x++;
            if (d <0) {
               d+=incr1;
            } else {
               y++;
               d+=incr2;
            }
            dashedSet;
         }
      } else {
         while (x < xend) {
            x++;
            if (d <0) {
               d+=incr1;
            } else {
               y--;
               d+=incr2;
            }
            dashedSet;
         }
      }      
   } else {
      d = 2*dx - dy;
      incr1 = 2*dx;
      incr2 = 2 * (dx - dy);
      if (y1 > y2) {
         y = y2;
         x = x2;
         yend = y1;
         xdirflag = (-1);
      } else {
         y = y1;
         x = x1;
         yend = y2;
         xdirflag = 1;
      }
      dashedSet;
      if (((x2 - x1) * xdirflag) > 0) {
         while (y < yend) {
            y++;
            if (d <0) {
               d+=incr1;
            } else {
               x++;
               d+=incr2;
            }
            dashedSet;
         }
      } else {
         while (y < yend) {
            y++;
            if (d <0) {
               d+=incr1;
            } else {
               x--;
               d+=incr2;
            }
            dashedSet;
         }
      }
   }
}

NLM_EXTERN int 
/*FCN*/gdImageBoundsSafe(gdImagePtr im, int x, int y)
{
   return IMAGE_BOUNDS_SAFE(im, x, y);
}

NLM_EXTERN void 
/*FCN*/gdImageChar(gdImagePtr im, gdFontPtr f, int x, int y, int c, int color)
{
   int cx, cy;
   int px, py;
   int fline;

   cx = 0;
   cy = 0;
   if ((c < f->offset) || (c >= (f->offset + f->nchars))) {
      return;
   }
   fline = (c - f->offset) * f->h * f->w;
   for (py = y; (py < (y + f->h)); py++) {
      for (px = x; (px < (x + f->w)); px++) {
         if (f->data[fline + cy * f->w + cx]) {
            gdImageSetPixel(im, px, py, color);   
         }
         cx++;
      }
      cx = 0;
      cy++;
   }
}

NLM_EXTERN void 
/*FCN*/gdImageCharV(gdImagePtr im, gdFontPtr f, int x, int y, int c, int color)
{
   int h, w;
   int px, py;
   int fline;
   char *da;

   h = f->h;
   w = f->w;
   fline = (c - f->offset) * h * w;
   da = f->data + fline;
   for (py = 0; py < h; py++) {
      for (px = 0; px < w; px++, da++) {
         if (*da) {
            gdImageSetPixel(im, x+py, y-px, color);   
         }
      }
   }
}

NLM_EXTERN void 
/*FCN*/gdImageString(gdImagePtr im, gdFontPtr f, int x, int y, char *s, int color)
{
   int i;
   int l;

   l = strlen(s);
   for (i=0; (i<l); i++) {
      gdImageChar(im, f, x, y, s[i], color);
      x += f->w;
   }
}

NLM_EXTERN void 
/*FCN*/gdImageStringV(gdImagePtr im, gdFontPtr f, int x, int y, char *s, int color)
{
   int i;
   int l;

   l = strlen(s);
   for (i=0; (i<l); i++) {
      gdImageCharV(im, f, x, y, s[i], color);
      y -= f->w;
   }
}


NLM_EXTERN void 
/*FCN*/gdImageArc(gdImagePtr im, int cx, int cy, int w, int h, int s, int e, int color)
{
   int i;
   int lx = 0, ly = 0;
   int w2, h2;

   w2 = w/2;
   h2 = h/2;
   while (e < s) {
      e += 360;
   }
   for (i=s; (i <= e); i++) {
      int x, y;
      x = ((long)cost[i % 360] * (long)w2 / costScale) + cx; 
      y = ((long)sint[i % 360] * (long)h2 / sintScale) + cy;
      if (i != s) {
         gdImageLine(im, lx, ly, x, y, color);   
      }
      lx = x;
      ly = y;
   }
}


NLM_EXTERN void
/*FCN*/gdImageArcEx(gdImagePtr im, int cx, int cy, int rx, int ry,
                    double s_angle, double e_angle, int color,
                    Nlm_Boolean fill)
{
  int A, B, C, D, E, F;
  int xs, ys, xe, ye;

  double s_cos = cos(s_angle);
  double s_sin = sin(s_angle);
  double e_cos = cos(e_angle);
  double e_sin = sin(e_angle);

  A = ry * ry;
  B = 0;
  C = rx * rx;
  D = -2 * A * cx;
  E = -2 * C * cy;
  F = A * cx * cx + C * cy * cy - A * C;

#define ROUNDER(x)  (x ? 0.5 : 0)
  xs = cx + (int)(rx * s_cos + ROUNDER(s_cos));
  ys = cy + (int)(ry * s_sin + ROUNDER(s_sin));
  xe = cx + (int)(rx * e_cos + ROUNDER(e_cos));
  ye = cy + (int)(ry * e_sin + ROUNDER(e_sin));

  if ( !fill )
    {
      /* plain arc */
      SDrawPointArc dpa;
      dpa.im = im;
      dpa.color = color;
      s_DrawConic(A, B, C, D, E, F, xs, ys, xe, ye, s_DrawPointArc, &dpa);
    }
  else
    {
      /* filled arc */
      SStorePoint sp_arc;
      sp_arc.xdim   = im->sx;
      sp_arc.ydim   = im->sy;
      sp_arc.n      = 0;
      sp_arc.points = (SPoint *)Nlm_Malloc(2 * (sp_arc.xdim + sp_arc.ydim));
      s_DrawConic(A, B, C, D, E, F, xs, ys, xe, ye, s_StorePoint, &sp_arc);
      if (!sp_arc.n  &&  (A + B + C + D + E + F > 0))
        return; /* point (1,1) to check if it belongs to the internal area */
    }
}


static void s_DrawEllipse(gdImagePtr im,
                          int cx, int cy, int rx, int ry, int color,
                          Nlm_Boolean fill, int quadrant)
{
  SDrawPointEllipse dpe;
  int A, B, C, D, E, F;
  int xs, ys, xe, ye;

  if (!rx  &&  !ry) {
    gdImageSetPixel(im, cx, cy, color);  
    return;
  }

  if ( !rx ) {
    ys = (quadrant == 0  ||  quadrant == 3  ||  quadrant == 4) ?
      cy + ry : cy;
    ye = (quadrant == 0  ||  quadrant == 1  ||  quadrant == 2) ?
      cy - ry : cy;
    gdImageLine(im, cx, ys, cx, ye, color);
    return;
  }

  if ( !ry ) {
    xs = (quadrant == 0  ||  quadrant == 1  ||  quadrant == 4) ?
      cx + rx : cx;
    xe = (quadrant == 0  ||  quadrant == 2  ||  quadrant == 3) ?
      cx - rx : cx;
    gdImageLine(im, xs, cy, xe, cy, color);
    return;
  }

  ASSERT( rx > 0  &&  ry > 0 );

  A = ry * ry;
  B = 0;
  C = rx * rx;
  D = 0;
  E = 0;
  F = - A * C;

  xs = rx;
  ys = 0;
  xe = 0;
  ye = ry;

  dpe.im = im;
  dpe.color = color;
  dpe.cx = cx;
  dpe.cy = cy;
  dpe.prev_y = 999;
  dpe.quadrant = quadrant;
  s_DrawConic(A, B, C, D, E, F, xs, ys, xe, ye,
              fill ? s_DrawPointFilledEllipse : s_DrawPointEllipse, &dpe);
}


NLM_EXTERN void
/*FCN*/gdImageEllipse(gdImagePtr im, int cx, int cy, int rx, int ry, int color,
                      Nlm_Boolean fill)
{
  s_DrawEllipse(im, cx, cy, rx, ry, color, fill, 0);
}

NLM_EXTERN void
/*FCN*/gdImageQuadrant(gdImagePtr im,
                       int cx, int cy, int rx, int ry, int color,
                       Nlm_Boolean fill, int quadrant)
{
  s_DrawEllipse(im, cx, cy, rx, ry, color, fill, quadrant);
}


NLM_EXTERN void 
/*FCN*/gdImageRectangle(gdImagePtr im,
                        int x1, int y1, int x2, int y2, int color)
{
   gdImageLine(im, x1, y1, x2, y1, color);      
   gdImageLine(im, x1, y2, x2, y2, color);      
   gdImageLine(im, x1, y1, x1, y2, color);
   gdImageLine(im, x2, y1, x2, y2, color);
}

NLM_EXTERN void 
/*FCN*/gdImageFilledRectangle(gdImagePtr im,
                              int x1, int y1, int x2, int y2, int color)
{
   int x, y;

   for (y=y1; (y<=y2); y++) {
      for (x=x1; (x<=x2); x++) {
         gdImageSetPixel(im, x, y, color);
      }
   }
}

NLM_EXTERN void
/*FCN*/gdImageRoundRectangle(gdImagePtr im,
                             int x1, int y1, int x2, int y2,
                             int oval_w, int oval_h,
                             int color, Nlm_Boolean fill)
{
  int X1 = MIN(x1, x2);
  int Y1 = MIN(y1, y2);
  int X2 = MAX(x1, x2);
  int Y2 = MAX(y1, y2);

  int w = x2 - x1 + 1;
  int h = y2 - y1 + 1;

  if (oval_w > w/2)
    oval_w = w/2;
  if (oval_h > h/2)
    oval_h = h/2;

  x1 = X1;  y1 = Y1;  x2 = X2;  y2 = Y2;  

  X1 = X1 + oval_w;  Y1 = Y1 + oval_h;
  X2 = X2 - oval_w;  Y2 = Y2 - oval_h;

  if ( fill ) {
    gdImageFilledRectangle(im, x1, Y1, x2, Y2, color);
    gdImageFilledRectangle(im, X1, y1, X2, Y1, color);
    gdImageFilledRectangle(im, X1, Y2, X2, y2, color);
  }
  else {
    gdImageLine(im, x1, Y1, x1, Y2, color);      
    gdImageLine(im, x2, Y1, x2, Y2, color);      
    gdImageLine(im, X1, y1, X2, y1, color);      
    gdImageLine(im, X1, y2, X2, y2, color);      
  }

  gdImageQuadrant(im, X2, Y1, oval_w, oval_h, color, fill, 1);
  gdImageQuadrant(im, X1, Y1, oval_w, oval_h, color, fill, 2);
  gdImageQuadrant(im, X1, Y2, oval_w, oval_h, color, fill, 3);
  gdImageQuadrant(im, X2, Y2, oval_w, oval_h, color, fill, 4);
}


NLM_EXTERN void 
/*FCN*/gdImagePolygon(gdImagePtr im, gdPointPtr p, int n, int c)
{
   int i;
   int lx, ly;

   if (!n) {
      return;
   }
   lx = p->x;
   ly = p->y;
   gdImageLine(im, lx, ly, p[n-1].x, p[n-1].y, c);
   for (i=1; (i < n); i++) {
      p++;
      gdImageLine(im, lx, ly, p->x, p->y, c);
      lx = p->x;
      ly = p->y;
   }
}   
   
NLM_EXTERN void 
/*FCN*/gdImageFilledPolygon(gdImagePtr im, gdPointPtr p, int n, int c)
{
   int i;
   int y;
   int y1, y2;
   size_t ints;

   if (!n) {
      return;
   }
   if (!im->polyAllocated) {
      im->polyInts = (int *) Nlm_Malloc(sizeof(int) * n);
      im->polyAllocated = n;
   }      
   if (im->polyAllocated < n) {
      while (im->polyAllocated < n) {
         im->polyAllocated *= 2;
      }   
      im->polyInts = (int *) realloc(im->polyInts,
         sizeof(int) * im->polyAllocated);
   }
   y1 = p[0].y;
   y2 = p[0].y;
   for (i=1; (i < n); i++) {
      if (p[i].y < y1) {
         y1 = p[i].y;
      }
      if (p[i].y > y2) {
         y2 = p[i].y;
      }
   }
   for (y=y1; (y <= y2); y++) {
      int interLast = 0;
      int dirLast = 0;
      int interFirst = 1;
      ints = 0;
      for (i=0; (i <= n); i++) {
         int x1, x2;
         int y1, y2;
         int dir;
         int ind1, ind2;
         int lastInd1 = 0;
         if ((i == n) || (!i)) {
            ind1 = n-1;
            ind2 = 0;
         } else {
            ind1 = i-1;
            ind2 = i;
         }
         y1 = p[ind1].y;
         y2 = p[ind2].y;
         if (y1 < y2) {
            y1 = p[ind1].y;
            y2 = p[ind2].y;
            x1 = p[ind1].x;
            x2 = p[ind2].x;
            dir = -1;
         } else if (y1 > y2) {
            y2 = p[ind1].y;
            y1 = p[ind2].y;
            x2 = p[ind1].x;
            x1 = p[ind2].x;
            dir = 1;
         } else {
            gdImageLine(im, 
               p[ind1].x, y1, 
               p[ind2].x, y1,
               c);
            continue;
         }
         if ((y >= y1) && (y <= y2)) {
            int inter;
            int tmp = y2 - y1;
            if ( x2 > x1 ){
              inter = ((y-y1) * (x2-x1) + tmp/2)/tmp + x1;
            } else{
              inter = ((y-y1) * (x2-x1) - tmp/2)/tmp + x1;
            }
            if (!interFirst) {
               if ((p[ind1].y == p[lastInd1].y) &&
                  (p[ind1].x != p[lastInd1].x)) {
                  if (dir == dirLast) {
                     if (inter > interLast) {
                        im->polyInts[ints] = inter;
                     }   
                     continue;
                  }
               }
               if (inter == interLast) {
                  if (dir == dirLast) {
                     continue;
                  }
               }
            } 
            if (i > 0) {
               im->polyInts[ints++] = inter;
            }
            lastInd1 = i;
            dirLast = dir;
            interLast = inter;
            interFirst = 0;
         }
      }

      if (ints > 1) {
        size_t j;
        HeapSort((Nlm_VoidPtr)im->polyInts, ints, sizeof(int), &gdCompareInt);
        for (j = 0;  j < (ints-1);  j += 2) {
          gdImageLine(im, im->polyInts[j], y, im->polyInts[j+1], y, c);
        }
      }
   }
}
   
NLM_EXTERN void 
/*FCN*/gdImageInterlace(gdImagePtr im, int interlaceArg)
{
   im->interlace = interlaceArg;
}

NLM_EXTERN void
/*FCN*/gdImageCopyBits ( gdImagePtr im, int x, int y, int w, int h,
                         char * ptr, int color )
{
  int i,j;
  unsigned char * uptr;
  int index;

  for ( i=0; i<w; i++ ){
    for ( j=0; j<h; j++ ){
      index = i+j*w;
      uptr = (unsigned char *)(ptr+index/8);
      index = 7 - index%8;
      index = (int)(*uptr >> index);
      if ( index & 0x1 ){
        gdImageSetPixel ( im, i+x, j+y, color );
      } else {
        gdImageSetPixel ( im, i+x, j+y, 0 );
      }
    }
  }
}

NLM_EXTERN void
/*FCN*/gdImageSelectPattern(gdImagePtr im, const unsigned char pattern[])
{
  size_t i;
  if ( !im )
    return;
  ASSERT ( sizeof(im->pattern) == 8 );

  im->use_pattern = 0;
  for (i = 0;  i < sizeof(im->pattern);  i++)
    {
      if ((im->pattern[i] = pattern[i]) != 0xFF)
        im->use_pattern = 1;
    }
}

NLM_EXTERN void 
/*FCN*/gdImageCopy(gdImagePtr dst, gdImagePtr src, int dstX, int dstY, int srcX, int srcY, int w, int h)
{
	int c;
	int x, y;
	int tox, toy;
	int i;
	int colorMap[gdMaxColors];
	for (i=0; (i<gdMaxColors); i++) {
		colorMap[i] = (-1);
	}
	toy = dstY;
	for (y=srcY; (y < (srcY + h)); y++) {
		tox = dstX;
		for (x=srcX; (x < (srcX + w)); x++) {
			int nc;
			c = gdImageGetPixel(src, x, y);
			/* Added 7/24/95: support transparent copies */
			if (gdImageGetTransparent(src) == c) {
				tox++;
				continue;
			}
			/* Have we established a mapping for this color? */
			if (colorMap[c] == (-1)) {
				/* If it's the same image, mapping is trivial */
				if (dst == src) {
					nc = c;
				} else { 
					/* First look for an exact match */
					nc = gdImageColorExact(dst,
						src->red[c], src->green[c],
						src->blue[c]);
				}	
				if (nc == (-1)) {
					/* No, so try to allocate it */
					nc = gdImageColorAllocate(dst,
						src->red[c], src->green[c],
						src->blue[c]);
					/* If we're out of colors, go for the
						closest color */
					if (nc == (-1)) {
						nc = gdImageColorClosest(dst,
							src->red[c], src->green[c],
							src->blue[c]);
					}
				}
				colorMap[c] = nc;
			}
			gdImageSetPixel(dst, tox, toy, colorMap[c]);
			tox++;
		}
		toy++;
	}
}			

NLM_EXTERN void 
/*FCN*/gdImageCopyResized(gdImagePtr dst, gdImagePtr src, int dstX, int dstY, int srcX, int srcY, int dstW, int dstH, int srcW, int srcH)
{
	int c;
	int x, y;
	int tox, toy;
	int ydest;
	int i;
	int colorMap[gdMaxColors];
	/* Stretch vectors */
	int *stx;
	int *sty;
	/* We only need to use floating point to determine the correct
		stretch vector for one line's worth. */
	double accum;
	stx = (int *) MemNew(sizeof(int) * srcW);
	sty = (int *) MemNew(sizeof(int) * srcH);
	accum = 0;
	for (i=0; (i < srcW); i++) {
		int got;
		accum += (double)dstW/(double)srcW;
		got = (int)floor(accum);
		stx[i] = got;
		accum -= got;
	}
	accum = 0;
	for (i=0; (i < srcH); i++) {
		int got;
		accum += (double)dstH/(double)srcH;
		got = (int)floor(accum);
		sty[i] = got;
		accum -= got;
	}
	for (i=0; (i<gdMaxColors); i++) {
		colorMap[i] = (-1);
	}
	toy = dstY;
	for (y=srcY; (y < (srcY + srcH)); y++) {
		for (ydest=0; (ydest < sty[y-srcY]); ydest++) {
			tox = dstX;
			for (x=srcX; (x < (srcX + srcW)); x++) {
				int nc;
				if (!stx[x - srcX]) {
					continue;
				}
				c = gdImageGetPixel(src, x, y);
				/* Added 7/24/95: support transparent copies */
				if (gdImageGetTransparent(src) == c) {
					tox += stx[x-srcX];
					continue;
				}
				/* Have we established a mapping for this color? */
				if (colorMap[c] == (-1)) {
					/* If it's the same image, mapping is trivial */
					if (dst == src) {
						nc = c;
					} else { 
						/* First look for an exact match */
						nc = gdImageColorExact(dst,
							src->red[c], src->green[c],
							src->blue[c]);
					}	
					if (nc == (-1)) {
						/* No, so try to allocate it */
						nc = gdImageColorAllocate(dst,
							src->red[c], src->green[c],
							src->blue[c]);
						/* If we're out of colors, go for the
							closest color */
						if (nc == (-1)) {
							nc = gdImageColorClosest(dst,
								src->red[c], src->green[c],
								src->blue[c]);
						}
					}
					colorMap[c] = nc;
				}
				for (i=0; (i < stx[x - srcX]); i++) {
					gdImageSetPixel(dst, tox, toy, colorMap[c]);
					tox++;
				}
			}
			toy++;
		}
	}
	free(stx);
	free(sty);
}


NLM_EXTERN int gdImageSetClip(gdImagePtr im,
                              const gdRect *clip, gdRect *old_clip)
{
  static const gdRect empty_clip = { {0, 0}, {0, 0} };

  int was_clip = im->use_clip;
  if ( old_clip ) {
    if ( im->use_clip )
      MemCpy(old_clip, &im->clip, sizeof(gdRect));
    else
      MemSet(old_clip, '\0', sizeof(gdRect));
  }

  if (clip  &&  MemCmp(&clip, &empty_clip, sizeof(gdRect))) {
    im->clip.lt.x = MIN(clip->lt.x, clip->rb.x);
    im->clip.lt.y = MIN(clip->lt.y, clip->rb.y);
    im->clip.rb.x = MAX(clip->lt.x, clip->rb.x);
    im->clip.rb.y = MAX(clip->lt.y, clip->rb.y);
    im->use_clip = 1;
  } else {
    im->use_clip = 0;
  }

  s_ComposeEffClip(im);

  return was_clip;
}


NLM_EXTERN void gdImageGetDimensions(gdImagePtr im, int* width, int* height)
{
  if ( width )
    *width = im->sx;
  if ( height )
    *height = im->sy;
}


NLM_EXTERN void gdImageGetAutoCropRectangle(gdImagePtr im, gdRect* rect)
{
  int width  = im->sx;
  int height = im->sy;
  int x, y, xb, yb, ye;
  Uint1 *base_ptr, *ptr;

  /* find upper colored pixel (CP);  background is zero */
  for (y = 0, base_ptr = im->pixels;  y < height;  y++, base_ptr += width) {
    for (x = 0, ptr = base_ptr;  x < width  &&  !*ptr;  x++, ptr++) ;
    if (x < width)
      break;
  }
  rect->lt.y = yb = y;

  /* Found no CPs at all? */
  if (y == height) {
    rect->lt.x = rect->lt.y = 0;
    rect->rb.x = MAX(im->sx-1, 0);
    rect->rb.y = MAX(im->sy-1, 0);
    return;
  }

  /* find lower CP */
  base_ptr = im->pixels + width * (height - 1);
  for (y = height-1;  y > yb;  y--, base_ptr -= width) {
    for (x = 0, ptr = base_ptr;  x < width  &&  !*ptr;  x++, ptr++) ;
    if (x < width)
      break;
  }
  rect->rb.y = ye = y;

  /* find left CP */
  base_ptr = im->pixels + width * yb;
  for (x = 0;  x < width;  x++, base_ptr++) {
    for (y = yb, ptr = base_ptr;  y <= ye  &&  !*ptr;  y++, ptr += width) ;
    if (y <= ye)
      break;
  }
  rect->lt.x = xb = x;

  /* find right CP */
  base_ptr = im->pixels + width * (ye + 1) - 1;
  for (x = width-1;  x > xb;  x--, base_ptr--) {
    for (y = ye, ptr = base_ptr;  y >= yb  &&  !*ptr;  y--, ptr -= width) ;
    if (y >= yb)
      break;
  }
  rect->rb.x = x;
}



NLM_EXTERN void gdImageCrop(gdImagePtr im, const gdRect* rect)
{
  Uint1* base_ptr = im->pixels;
  gdRect crop     = *rect;
  int    width    = im->sx;

  int    new_width;
  int    y;
  Uint1* ptr;
   
  /* adjust crop rectangle to fit image boundaries */
  s_CropRectImage(&crop, im);

  /* nothing to crop;  or crop without memcpy? */
  if (crop.lt.x == 0  &&  crop.lt.y == 0  &&  crop.rb.x == im->sx - 1) {
    im->sy = crop.rb.y + 1;
    return;
  }

  /* "crop" -- move the content of crop rectangle to (0, 0) */
  new_width = crop.rb.x - crop.lt.x + 1;
  ptr = base_ptr + crop.lt.y * width + crop.lt.x;
  for (y = crop.lt.y;  y <= crop.rb.y;  y++) {
    memcpy(base_ptr, ptr, new_width);
    base_ptr += new_width;
    ptr  += width;
  }

  /* alternate image width/height */
  im->sx = new_width;
  im->sy = crop.rb.y - crop.lt.y + 1;
}


NLM_EXTERN void gdImageAutoCrop(gdImagePtr im, int border)
{
  gdRect crop;
  if (border < 0)
    border = -border;

  gdImageGetAutoCropRectangle(im, &crop);
  s_ExpandRect(&crop, border, border);
  gdImageCrop(im, &crop);
}


/*EOF*/
