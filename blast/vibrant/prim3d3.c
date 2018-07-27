/*   prim3d3.c
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
* File Name:  prim3d3.c
*
* Author:  Alex Smirnov
*
* Version Creation Date:   04/03/95
*
* $Revision: 6.1 $
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

#ifndef _VIBRANT_
#include <vibrant.h>
#endif

#ifndef _PPICT3D_
#include <ppict3d.h>
#endif

#ifndef _PDIAGNOS_
#include <pdiagnos.h>
#endif

/*****************************************************************************
*
*   DEFINES
*
*****************************************************************************/
#define SETARCPIXEL(zptr,imageptr,z) \
{long offset; \
 long y_cur; long x_cur; \
 long reg1_save; long reg1_save_save; long zptr_save; long im_save; \
 if ( needChk){ \
   if ( (arcBMinX <= arc_xmax) && (arcBMaxX >= arc_xmin) && \
        (arcBMinY <= arc_ymax) && (arcBMaxY >= arc_ymin) ) { \
     if ( (arcBMinX > arc_xmin) && (arcBMaxX < arc_xmax) && \
          (arcBMinY > arc_ymin) && (arcBMaxY < arc_ymax) ) { \
       reg1_save = reg1; \
       reg1 = (Nlm_stCon.zmaxPersp1-z); \
       im_save = imageptr; \
       imageptr = arcBTotal; \
       while ( imageptr > 0 ){ \
         imageptr--; \
         zptr_save = zptr; \
         offset = (arcBufferY[imageptr]/reg1) * Nlm_stCon.width + \
                  arcBufferX[imageptr]/reg1; \
         zptr +=  offset<<1; \
         reg1_save_save = reg1; \
         reg1 = (arcBufferZ[imageptr])/reg1 + z; \
         if( (Uint2)reg1>=*((Uint2Ptr)zptr)) {\
           *((Uint2Ptr)zptr) = (Uint2)reg1; \
           *((Uint1Ptr)(im_save+offset)) = \
              (Uint1)(color + ((arcBufferC[imageptr]*z)>>9)); \
         } \
         reg1 = reg1_save_save; \
         zptr = zptr_save; \
       } \
       imageptr = im_save; \
       reg1 = reg1_save; \
     } else { \
       reg1_save = reg1; \
       reg1 = (Nlm_stCon.zmaxPersp1-z); \
       im_save = imageptr; \
       imageptr = arcBTotal; \
       while ( imageptr > 0 ){ \
         imageptr--; \
         y_cur = arcBufferY[imageptr]/reg1; \
         x_cur = arcBufferX[imageptr]/reg1; \
         if ( (x_cur>=arc_xmin)&&(x_cur<=arc_xmax)&& \
              (y_cur>=arc_ymin)&&(y_cur<=arc_ymax) ){ \
           offset = y_cur * Nlm_stCon.width + x_cur; \
           zptr_save = zptr; \
           zptr +=  offset<<1; \
           reg1_save_save = reg1; \
           reg1 = (arcBufferZ[imageptr])/reg1 + z; \
           if( (Uint2)reg1>=*((Uint2Ptr)zptr)) {\
             *((Uint2Ptr)zptr) = (Uint2)reg1; \
             *((Uint1Ptr)(im_save+offset)) = \
              (Uint1)(color + ((arcBufferC[imageptr]*z)>>9)); \
           } \
           reg1 = reg1_save_save; \
           zptr = zptr_save; \
         } \
       } \
       imageptr = im_save; \
       reg1 = reg1_save; \
     } \
   } \
 } else { \
   reg1_save = reg1; \
   reg1 = (Nlm_stCon.zmaxPersp1-z); \
   im_save = imageptr; \
   imageptr = arcBTotal; \
   while ( imageptr > 0 ){ \
     imageptr--; \
     zptr_save = zptr; \
     offset = (arcBufferY[imageptr]/reg1) * Nlm_stCon.width + \
              arcBufferX[imageptr]/reg1; \
     zptr +=  offset<<1; \
     reg1_save_save = reg1; \
     reg1 = (arcBufferZ[imageptr])/reg1 + z; \
     if( (Uint2)reg1>=*((Uint2Ptr)zptr)) {\
       *((Uint2Ptr)zptr) = (Uint2)reg1; \
       *((Uint1Ptr)(im_save+offset)) = \
          (Uint1)(color + ((arcBufferC[imageptr]*z)>>9)); \
     } \
     reg1 = reg1_save_save; \
     zptr = zptr_save; \
   } \
   imageptr = im_save; \
   reg1 = reg1_save; \
 } \
} 

#define SETARCPIXELID(zptr,imageptr,z,curID) \
{long offset; \
 long y_cur; long x_cur; \
 long reg1_save; long reg1_save_save; long zptr_save; long im_save; \
 reg1_save = reg1; \
 reg1 = (Nlm_stCon.zmaxPersp1-z); \
 im_save = imageptr; \
 imageptr = arcBTotal; \
 while ( imageptr > 0 ){ \
   imageptr--; \
   y_cur = arcBufferY[imageptr]/reg1; \
   x_cur = arcBufferX[imageptr]/reg1; \
   if ( (x_cur>=arc_xmin)&&(x_cur<=arc_xmax)&& \
        (y_cur>=arc_ymin)&&(y_cur<=arc_ymax) ){ \
     offset = (y_cur * Nlm_stCon.widthCur + x_cur); \
     zptr_save = zptr; \
     zptr += (offset << 1); \
     reg1_save_save = reg1; \
     reg1 = (arcBufferZ[imageptr])/reg1 + z; \
     if( (Uint2)reg1>=*((Uint2Ptr)zptr)) {\
       *((Uint2Ptr)zptr) = (Uint2)reg1; \
       *((VoidPtr*)(im_save+(offset * sizeof(Nlm_stCon.idBuffer[0])))) =curID;\
     } \
     reg1 = reg1_save_save; \
     zptr = zptr_save; \
   } \
 } \
 imageptr = im_save; \
 reg1 = reg1_save; \
} 

/*****************************************************************************
*
*   TYPEDEFS
*
*****************************************************************************/

/*****************************************************************************
*
*   GLOBAL VARIABLE
*
*****************************************************************************/
extern Nlm_Context3D Nlm_stCon;
extern Uint1         Nlm_Isqrt[129*129];

/*****************************************************************************
*
*   STATIC VARIABLE
*
*****************************************************************************/
static long arcBufferX[256*3];
static long arcBufferY[256*3];
static long arcBufferZ[256*3];
static long arcBufferC[256*3];
static long arcBTotal;
static long arcBMinX,arcBMaxX,arcBMinY,arcBMaxY;


/*****************************************************************************
*
*   FUNCTIONS
*
*****************************************************************************/
static void Nlm_Empty3D ( Nlm_VoidPtr p )
{
}

/*****************************************************************************
*
*   CYLINDER
*
*****************************************************************************/
typedef struct Nlm_cylin3d {
  Nlm_Base3D  base;
  long        x1,y1,z1;
  long        x2,y2,z2;
  long        radius;
} Nlm_Cylin3D, PNTR Nlm_Cylin3DPtr;

static void Nlm_Cylin3DDraw ( Nlm_Cylin3DPtr p )
{
  register long reg1;
  register long reg2;
  register long reg3;
  register long reg4;
  double        arc_clenXY;
  Uint1Ptr      radiusF;
  long          c_x1,c_x2;
  long          c_y1,c_y2;
  long          c_z1,c_z2;
  long          c_dx,c_dy,c_dz;
  long          rad;
  long          needChk;
  long          colorW;
  long          ystep;
  long          zystep;
  long          arc_x, arc_y;
  long          arc_porErr, arc_stepErr;
  long          arc_errX, arc_errY;
  long          arcN_x, arcN_y, arc_z;
  long          arcN_rMax, arcN_rMax2, arcN_rMin2;
  long          arcN_errX, arcN_errY, arcN_errBase;
  long          arc_stepSin, arc_stepCos;
  long          xx1,xx2;
  long          yy1,yy2;
  long          zz1,zz2;
  long          status2;
  long          orient;
  long          endPtr;
  long          colorScale;
  long          color;
  long          offset1;
  long          offset2;
  long          zrate;
  long          dx, dy;
  long          curz;
  long          arc_scale1;
  long          arc_offset1;
  long          arc_xmin, arc_ymin, arc_xmax, arc_ymax;

  reg1 = (long)p;

  /* Rotate and check */
  reg2 = Nlm_stCon.a[0][0];
  reg3 = ((Nlm_Cylin3DPtr)reg1)->x1 / (long)reg2;
  reg4 = ((Nlm_Cylin3DPtr)reg1)->x2 / (long)reg2;
  reg2 = Nlm_stCon.a[0][1];
  reg3 += ((Nlm_Cylin3DPtr)reg1)->y1 / (long)reg2;
  reg4 += ((Nlm_Cylin3DPtr)reg1)->y2 / (long)reg2;
  reg2 = Nlm_stCon.a[0][2];
  reg3 += ((Nlm_Cylin3DPtr)reg1)->z1 / (long)reg2;
  reg4 += ((Nlm_Cylin3DPtr)reg1)->z2 / (long)reg2;
  reg2 = Nlm_stCon.c[0];
  reg3 += reg2;
  reg4 += reg2;
  c_x1 = reg3; c_x2 = reg4;

  reg2 = Nlm_stCon.a[1][0];
  reg3 = ((Nlm_Cylin3DPtr)reg1)->x1 / (long)reg2;
  reg4 = ((Nlm_Cylin3DPtr)reg1)->x2 / (long)reg2;
  reg2 = Nlm_stCon.a[1][1];
  reg3 += ((Nlm_Cylin3DPtr)reg1)->y1 / (long)reg2;
  reg4 += ((Nlm_Cylin3DPtr)reg1)->y2 / (long)reg2;
  reg2 = Nlm_stCon.a[1][2];
  reg3 += ((Nlm_Cylin3DPtr)reg1)->z1 / (long)reg2;
  reg4 += ((Nlm_Cylin3DPtr)reg1)->z2 / (long)reg2;
  reg2 = Nlm_stCon.c[1];
  reg3 += reg2;
  reg4 += reg2;
  c_y1 = reg3; c_y2 = reg4;

  reg2 = Nlm_stCon.a[2][0];
  reg3 = ((Nlm_Cylin3DPtr)reg1)->x1 / (long)reg2;
  reg4 = ((Nlm_Cylin3DPtr)reg1)->x2 / (long)reg2;
  reg2 = Nlm_stCon.a[2][1];
  reg3 += ((Nlm_Cylin3DPtr)reg1)->y1 / (long)reg2;
  reg4 += ((Nlm_Cylin3DPtr)reg1)->y2 / (long)reg2;
  reg2 = Nlm_stCon.a[2][2];
  reg3 += ((Nlm_Cylin3DPtr)reg1)->z1 / (long)reg2;
  reg4 += ((Nlm_Cylin3DPtr)reg1)->z2 / (long)reg2;
  reg2 = Nlm_stCon.c[2];
  reg3 += reg2;
  reg4 += reg2;
  c_z1 = reg3; c_z2 = reg4; c_dz = reg4-reg3;
  if ( c_dz > 0 ){
    reg2 = c_z1; c_z1 = c_z2; c_z2 = reg2;
    reg2 = c_y1; c_y1 = c_y2; c_y2 = reg2;
    reg2 = c_x1; c_x1 = c_x2; c_x2 = reg2;
    c_dz = -c_dz;
  }
  reg2 = ((Nlm_Cylin3DPtr)reg1)->radius / Nlm_stCon.scale;
  reg2 = (reg2*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-c_z1);
  if ( reg2 > 127 ) return;
  if ( reg2 == 0 ) reg2 = 1;
  radiusF = &(Nlm_Isqrt[reg2*(reg2+1)]);
  rad = reg2;
  needChk = 0;
  reg2 = Nlm_stCon.width>>1;
  reg3 = c_x1-reg2;
  c_x1 = reg2 + (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-c_z1);
  reg3 = c_x2-reg2;
  reg4 = reg2 + (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-c_z2);
  c_x2 = reg4;
  reg3 = c_x1;
  c_dx = reg4-reg3;
  if ( reg3 < reg4 ) {
    reg3 -= rad; reg4 += rad;
    if ( (reg4 < Nlm_stCon.xmin)||(reg3 > Nlm_stCon.xmax) ) return;
    reg3 -= 4; reg4 += 4;
    if ( (reg4 > Nlm_stCon.xmax)||(reg3 < Nlm_stCon.xmin) ) needChk = 1;
  } else {
    reg3 += rad; reg4 -= rad;
    if ( (reg3 < Nlm_stCon.xmin)||(reg4 > Nlm_stCon.xmax) ) return;
    reg3 += 4; reg4 -= 4;
    if ( (reg3 > Nlm_stCon.xmax)||(reg4 < Nlm_stCon.xmin) ) needChk = 1;
  }
  reg2 = Nlm_stCon.height>>1;
  reg3 = c_y1-reg2;
  c_y1 = reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-c_z1);
  reg3 = c_y2-reg2;
  reg4 = reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-c_z2);
  c_y2 = reg4;
  reg3 = c_y1;
  c_dy = reg4-reg3;
  if ( reg3 < reg4 ) {
    reg3 -= rad; reg4 += rad;
    if ( (reg4 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) return;
    reg3 -= 4; reg4 += 4;
    if ( (reg4 > Nlm_stCon.ymax)||(reg3 < Nlm_stCon.ymin) ) needChk = 1;
  } else {
    reg3 += rad; reg4 -= rad;
    if ( (reg3 < Nlm_stCon.ymin)||(reg4 > Nlm_stCon.ymax) ) return;
    reg3 += 4; reg4 -= 4;
    if ( (reg3 > Nlm_stCon.ymax)||(reg4 < Nlm_stCon.ymin) ) needChk = 1;
  }
  reg3 = c_z1;
  reg3 = (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-reg3);
  reg4 = c_z2;
  reg4 = (reg4*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-reg4);
  c_z1 = reg3; c_z2 = reg4; c_dz = reg4-reg3;

  /* c_x1,c_y1,c_z1 - first (top) point    
     c_x2,c_y2,c_z2 - second point
     c_dx,c_dy,c_dz - vector 
     rad            - radius
     needChk        - 1 if need */

  if ( (c_dx==0) && (c_dy==0) ) return;
  colorW = Nlm_stCon.colorOffset +
    ((Nlm_Cylin3DPtr)reg1)->base.color * Nlm_stCon.colorStep + 1;
  color = (Nlm_stCon.colorStep*141*(c_z1+Nlm_stCon.zmax))/
    (628*Nlm_stCon.zmax) + colorW;
  colorScale = (((rad+1)*314)<<8)/
    (Nlm_stCon.colorStep*(50+(50*c_z1)/Nlm_stCon.zmax));
  colorScale *= 50+(50*c_z1);
  ystep = Nlm_stCon.width;
  zystep = ystep<<1;

  /* Elips - start */
  reg2 = c_dx*c_dx + c_dy*c_dy;
  arc_clenXY = sqrt ( (double)reg2 );
  reg3 = c_dz*c_dz;
  reg2 += reg3;
  reg4 = reg2/16000 + 1;
  reg2 /= reg4;
  reg3 /= reg4;
  if ( reg3 == 0 ) {
    reg2 = 16000; reg3 = 1;
  }
  arcN_rMax2 = reg2;
  arcN_rMin2 = reg3;
  reg2 = rad;
  arc_stepCos = arc_x = (long)((double)(-c_dy*reg2)/arc_clenXY); 
  arc_stepSin = arc_y = (long)((double)(c_dx*reg2)/arc_clenXY);
  arcN_rMax = arc_stepErr = reg2<<2;
  arc_porErr = reg2<<1;
  arcN_x= -arcN_rMax; arcN_y = 0;
  arcN_errBase = 0;
  arc_x--; 
  arc_errX = arc_stepErr; arc_errY = 0;
  arcBTotal = 0;
  arcBMinX = arcBMinY = -1000;
  arcBMaxX = arcBMaxY = 1000;
  arc_scale1 = (Nlm_stCon.zmaxPersp1-c_z1);
  arc_offset1 = arc_scale1>>1;
  do {
    for ( ;; ){
      reg1 = arc_porErr;
      reg3 = arc_errX; reg4 = arc_errY;
      if ( reg3 > reg1 ){
        arc_x ++; reg3 -= arc_stepErr;
      } else if ( reg3 < -reg1 ){
        arc_x --; reg3 += arc_stepErr;
      } else if ( reg4 > reg1 ){
        arc_y ++; reg4 -= arc_stepErr;
      } else if ( reg4 < -reg1 ){
        arc_y --; reg4 += arc_stepErr;
      } else break;
      arc_errX = reg3; arc_errY = reg4;
      /* we have arc_x, arc_y, rad and radiusF */
      reg1 = arc_y;
      if ( reg1 < 0 ) reg1 = -reg1;
      reg1 = (long)radiusF[reg1];
      if ( arc_x > reg1 ) {
        /*arc_x = reg1;*/
        arc_z = 0;
      } else if ( arc_x < -reg1 ) {
        /*arc_x = -reg1;*/
        arc_z = 0;
      } else {
        arc_z = Nlm_Isqrt[reg1*(reg1+1)+arc_x];
      }
      /* we have arc_x, arc_y, arc_z and rad */
      arcBufferC[arcBTotal] = ((50*(arc_z-arc_x Y_PLUS arc_y))<<17)/colorScale;
      arcBufferX[arcBTotal] = arc_x*arc_scale1+arc_offset1;
      arcBufferY[arcBTotal] = arc_y*arc_scale1+arc_offset1;
      arcBufferZ[arcBTotal++] = arc_z*arc_scale1+arc_offset1;
      if ( needChk ) {
        if ( arcBMinX > arc_x ) arcBMinX = arc_x;
        if ( arcBMaxX < arc_x ) arcBMaxX = arc_x;
        if ( arcBMinY > arc_y ) arcBMinY = arc_y;
        if ( arcBMaxY < arc_y ) arcBMaxY = arc_y;
      }
    }
    if ( arcN_x < 0 ){
      arcN_errX = arcN_errBase + (arcN_x<<1) + 1;
      arcN_errY = arcN_errBase + (arcN_rMax2*((arcN_y<<1) + 1))/arcN_rMin2;
      if ( arcN_errX <= 0 ){
        if ( (arcN_errY <= 0) && (arcN_errY > arcN_errX) ){
          arcN_y++; arcN_errBase = arcN_errY; 
          arc_errX += arc_stepSin; arc_errY -= arc_stepCos;
        } else {
          arcN_x++; arcN_errBase = arcN_errX; 
          arc_errX -= arc_stepCos; arc_errY -= arc_stepSin;
        }
      } else {
        arcN_y++; arcN_errBase = arcN_errY; 
        arc_errX += arc_stepSin; arc_errY -= arc_stepCos;
      }
    } else {
      arcN_errX = arcN_errBase + (arcN_x<<1) + 1;
      arcN_errY = arcN_errBase + (arcN_rMax2*(-(arcN_y<<1) + 1))/arcN_rMin2;
      if ( (arcN_errY <= 0) && (arcN_y > 0) ){
        if ( (arcN_errX <= 0) && (arcN_errX > arcN_errY) ){
          arcN_x++; arcN_errBase = arcN_errX; 
          arc_errX -= arc_stepCos; arc_errY -= arc_stepSin;
        } else {
          arcN_y--; arcN_errBase = arcN_errY; 
          arc_errX -= arc_stepSin; arc_errY += arc_stepCos;
        }
      } else {
        arcN_x++; arcN_errBase = arcN_errX; 
        arc_errX -= arc_stepCos; arc_errY -= arc_stepSin;
      }
    }
  } while ( (arcN_x <= arcN_rMax)&&(arcN_y >= 0) );
  reg1 = rad;
  Nlm_stCon.xmin -= reg1;
  Nlm_stCon.xmax += reg1; 
  Nlm_stCon.ymin -= reg1;
  Nlm_stCon.ymax += reg1;
  if ( needChk ) {
    /* Calculate status1(reg1) and status2(reg2) */
    reg1 = 0;
    reg2 = 0;
    reg3 = c_x1;
    reg4 = Nlm_stCon.xmin;
    if ( reg3 < reg4 ) reg1 = 1;
    else if ( reg3 > Nlm_stCon.xmax ) reg1 = 2;
    reg3 = c_x2;
    if ( reg3 < reg4 ) reg2 = 1;
    else if ( reg3 > Nlm_stCon.xmax ) reg2 = 2;

    reg3 = c_y1;
    reg4 = Nlm_stCon.ymin;
    if ( reg3 < reg4 ) reg1 += 6;
    else if ( reg3 > Nlm_stCon.ymax ) reg1 += 3;
    reg3 = c_y2;
    if ( reg3 < reg4 ) reg2 += 6;
    else if ( reg3 > Nlm_stCon.ymax ) reg2 += 3;

    /* Save status2*/
    status2 = reg2;

    /* Calculate the first point (xx1,yy1,zz1)*/
    switch ( reg1 ) {
      case 0: 
        xx1 = c_x1; yy1=c_y1; zz1=c_z1;
        break;
      case 1:
        reg1 = c_x1; reg2 = Nlm_stCon.xmin;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy1 = reg3;
        xx1 = reg2;
        zz1 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 2:
        reg1 = c_x1; reg2 = Nlm_stCon.xmax;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy1 = reg3;
        xx1 = reg2;
        zz1 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 3:
        reg1 = c_y1; reg2 = Nlm_stCon.ymax;
        reg4 = c_y2-reg1;
        reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
        xx1 = reg3;
        yy1 = reg2;
        zz1 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 4:
        reg1 = c_x1; reg2 = Nlm_stCon.xmin;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy1 = reg3;
        xx1 = reg2;
        if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
          reg1 = c_y1; reg2 = Nlm_stCon.ymax;
          reg4 = c_y2-reg1;
          reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
          xx1 = reg3;
          yy1 = reg2;
        }
        zz1 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 5:
        reg1 = c_x1; reg2 = Nlm_stCon.xmax;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy1 = reg3;
        xx1 = reg2;
        if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
          reg1 = c_y1; reg2 = Nlm_stCon.ymax;
          reg4 = c_y2-reg1;
          reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
          xx1 = reg3;
          yy1 = reg2;
        }
        zz1 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 6:
        reg1 = c_y1; reg2 = Nlm_stCon.ymin;
        reg4 = c_y2-reg1;
        reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
        xx1 = reg3;
        yy1 = reg2;
        zz1 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 7:
        reg1 = c_x1; reg2 = Nlm_stCon.xmin;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy1 = reg3;
        xx1 = reg2;
        if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
          reg1 = c_y1; reg2 = Nlm_stCon.ymin;
          reg4 = c_y2-reg1;
          reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
          xx1 = reg3;
          yy1 = reg2;
        }
        zz1 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      default:
        reg1 = c_x1; reg2 = Nlm_stCon.xmax;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy1 = reg3;
        xx1 = reg2;
        if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
          reg1 = c_y1; reg2 = Nlm_stCon.ymin;
          reg4 = c_y2-reg1;
          reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
          xx1 = reg3;
          yy1 = reg2;
        }
        zz1 = c_z1 + c_dz*(reg2-reg1)/reg4;
    }

    /* Calculate the second point (xx2,yy2,zz2)*/
    switch ( status2 ) {
      case 0: 
        xx2 = c_x2; yy2=c_y2; zz2=c_z2;
        break;
      case 1:
        reg1 = c_x1; reg2 = Nlm_stCon.xmin;
        reg4 = c_x2-reg1;
        yy2 = c_y1 + c_dy*(reg2-reg1)/reg4;
        xx2 = reg2;
        zz2 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 2:
        reg1 = c_x1; reg2 = Nlm_stCon.xmax;
        reg4 = c_x2-reg1;
        yy2 = c_y1 + c_dy*(reg2-reg1)/reg4;
        xx2 = reg2;
        zz2 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 3:
        reg1 = c_y1; reg2 = Nlm_stCon.ymax;
        reg4 = c_y2-reg1;
        xx2 = c_x1 + c_dx*(reg2-reg1)/reg4;
        yy2 = reg2;
        zz2 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 4:
        reg1 = c_x1; reg2 = Nlm_stCon.xmin;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy2 = reg3;
        xx2 = reg2;
        if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
          reg1 = c_y1; reg2 = Nlm_stCon.ymax;
          reg4 = c_y2-reg1;
          reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
          xx2 = reg3;
          yy2 = reg2;
        }
        zz2 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 5:
        reg1 = c_x1; reg2 = Nlm_stCon.xmax;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy2 = reg3;
        xx2 = reg2;
        if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
          reg1 = c_y1; reg2 = Nlm_stCon.ymax;
          reg4 = c_y2-reg1;
          reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
          xx2 = reg3;
          yy2 = reg2;
        }
        zz2 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 6:
        reg1 = c_y1; reg2 = Nlm_stCon.ymin;
        reg4 = c_y2-reg1;
        xx2 = c_x1 + c_dx*(reg2-reg1)/reg4;
        yy2 = reg2;
        zz2 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 7:
        reg1 = c_x1; reg2 = Nlm_stCon.xmin;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy2 = reg3;
        xx2 = reg2;
        if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
          reg1 = c_y1; reg2 = Nlm_stCon.ymin;
          reg4 = c_y2-reg1;
          reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
          xx2 = reg3;
          yy2 = reg2;
        }
        zz2 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      default:
        reg1 = c_x1; reg2 = Nlm_stCon.xmax;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy2 = reg3;
        xx2 = reg2;
        if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
          reg1 = c_y1; reg2 = Nlm_stCon.ymin;
          reg4 = c_y2-reg1;
          reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
          xx2 = reg3;
          yy2 = reg2;
        }
        zz2 = c_z1 + c_dz*(reg2-reg1)/reg4;
    }
    if ( (xx1==xx2) && (yy1==yy2) ) {
      reg1 = rad;
      Nlm_stCon.xmin += reg1;
      Nlm_stCon.xmax -= reg1; 
      Nlm_stCon.ymin += reg1;
      Nlm_stCon.ymax -= reg1; 
      return;
    }
  } else {
    yy1 = c_y1; yy2 = c_y2;
    xx1 = c_x1; xx2= c_x2;
    zz1= c_z1; zz2 = c_z2;
  }
  reg3 = xx1;
  reg4 = xx2;
  if ( reg3 < reg4 ) {
    if ( (reg3 < Nlm_stCon.xmin)||(reg4 > Nlm_stCon.xmax) ) {
      reg1 = rad;
      Nlm_stCon.xmin += reg1;
      Nlm_stCon.xmax -= reg1; 
      Nlm_stCon.ymin += reg1;
      Nlm_stCon.ymax -= reg1; 
      return;
    }
    reg1 = 0;
  } else {
    if ( (reg4 < Nlm_stCon.xmin)||(reg3 > Nlm_stCon.xmax) ) {
      reg1 = rad;
      Nlm_stCon.xmin += reg1;
      Nlm_stCon.xmax -= reg1; 
      Nlm_stCon.ymin += reg1;
      Nlm_stCon.ymax -= reg1;
      return;
    }
    reg1 = 1;
  }
  reg3 = yy1;
  reg4 = yy2;
  if ( reg3 < reg4 ) {
    if ( (reg3 < Nlm_stCon.ymin)||(reg4 > Nlm_stCon.ymax) ) {
      reg1 = rad;
      Nlm_stCon.xmin += reg1;
      Nlm_stCon.xmax -= reg1; 
      Nlm_stCon.ymin += reg1;
      Nlm_stCon.ymax -= reg1; 
      return;
    }
  } else {
    if ( (reg4 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
      reg1 = rad;
      Nlm_stCon.xmin += reg1;
      Nlm_stCon.xmax -= reg1; 
      Nlm_stCon.ymin += reg1;
      Nlm_stCon.ymax -= reg1; 
      return;
    }
    reg1 |= 0x2;
  }
  orient = reg1;
  reg1 = rad;
  Nlm_stCon.xmin += reg1;
  Nlm_stCon.xmax -= reg1; 
  Nlm_stCon.ymin += reg1;
  Nlm_stCon.ymax -= reg1;

  /* Calculate offsets and pointers */
  reg1 = yy1 * Nlm_stCon.width + xx1;
  offset1 = reg1;
  reg3 = (long)(Nlm_stCon.image + reg1);
  reg2 = yy2 * Nlm_stCon.width + xx2;
  offset2 = reg2;
  reg4 = (long)(Nlm_stCon.image + reg2);
  /* reg3 - statrt reg4 - end */

  /* Will be:
     reg1 - last orientation
     reg2 - zz1
     reg3 - start
     reg4 - zPtr
     endPtr saved in endPtr */
  reg1 = orient;
  switch ( reg1 ){
    case 0:
      dy = yy2 - yy1;
      reg2 = xx2 - xx1;
      dx = reg2;
      if ( reg2>dy ) {
        zrate = ((zz2 - zz1)<<10)/reg2;
      } else {
        zrate = ((zz2 - zz1)<<10)/dy;
        reg1 = 2;
      }
      reg2 = zz1;
      endPtr = reg4;
      arc_xmin = Nlm_stCon.xmin - xx1;
      arc_xmax = Nlm_stCon.xmax - xx1;
      arc_ymin = Nlm_stCon.ymin - yy1;
      arc_ymax = Nlm_stCon.ymax - yy1;
      reg4 = (long)(Nlm_stCon.zBuffer + offset1);
      break;
    case 1:
      dy = yy2 - yy1;
      reg2 = xx1 - xx2;
      if ( reg2 > dy ) {
        zrate = ((zz1 - zz2)<<10)/reg2;
      } else {
        zrate = ((zz1 - zz2)<<10)/dy;
        reg1 = 3;
      }
      dx = reg2;
      reg2 = reg3; reg3 = reg4; reg4 = reg2;
      reg2 = zz2;
      endPtr = reg4;
      arc_xmin = Nlm_stCon.xmin - xx2;
      arc_xmax = Nlm_stCon.xmax - xx2;
      arc_ymin = Nlm_stCon.ymin - yy2;
      arc_ymax = Nlm_stCon.ymax - yy2;
      reg4 = (long)(Nlm_stCon.zBuffer + offset2);
      break;
    case 2:
      dy = yy1 - yy2;
      reg2 = xx2 - xx1;
      if ( reg2 > dy ){
        zrate = ((zz2 - zz1)<<10)/reg2;
        reg1 = 1;
      } else {
        zrate = ((zz2 - zz1)<<10)/dy;
        reg1 = 3;
      }
      dx = reg2;
      reg2 = zz1;
      endPtr = reg4;
      arc_xmin = Nlm_stCon.xmin - xx1;
      arc_xmax = Nlm_stCon.xmax - xx1;
      arc_ymin = Nlm_stCon.ymin - yy1;
      arc_ymax = Nlm_stCon.ymax - yy1;
      reg4 = (long)(Nlm_stCon.zBuffer + offset1);
      break;
    default:
      dy = yy1 - yy2;
      reg2 = xx1 - xx2;
      if ( reg2 > dy ){
        zrate = ((zz1 - zz2)<<10)/reg2;
        reg1 = 0;
      } else {
        zrate = ((zz1 - zz2)<<10)/dy;
        reg1 = 2;
      }
      dx = reg2;
      reg2 = reg3; reg3 = reg4; reg4 = reg2;
      reg2 = zz2;
      endPtr = reg4;
      arc_xmin = Nlm_stCon.xmin - xx2;
      arc_xmax = Nlm_stCon.xmax - xx2;
      arc_ymin = Nlm_stCon.ymin - yy2;
      arc_ymax = Nlm_stCon.ymax - yy2;
      reg4 = (long)(Nlm_stCon.zBuffer + offset2);
  }

  SETARCPIXEL(reg4,reg3,reg2);
  reg2 <<= 10;
  /* Will be: reg1 - err */
  switch ( reg1 ){
    case 0:
      reg1 = -(dx>>1);
      do {
        reg1 += dy;
        if ( reg1 > 0 ){
           reg3 += ystep; 
           arc_ymin--;
           arc_ymax--;
           reg4 += zystep; 
           reg1 -= dx; 
        }
        reg3++; reg4++; reg4++;
        arc_xmin--;
        arc_xmax--;
        reg2 += zrate; 
        curz = reg2;
        reg2 >>= 10;
        SETARCPIXEL(reg4,reg3,reg2);
        reg2 = curz;
      } while ( reg3 != endPtr );
      break;
    case 1:
      reg1 = -(dx>>1);
      do {
        reg1 += dy;
        if ( reg1 > 0 ){
           reg3 -= ystep; 
           arc_ymin++;
           arc_ymax++;
           reg4 -= zystep; 
           reg1 -= dx; 
        }
        reg3++; reg4++; reg4++;
        arc_xmin--;
        arc_xmax--;
        reg2 += zrate; 
        curz = reg2;
        reg2 >>= 10;
        SETARCPIXEL(reg4,reg3,reg2);
        reg2 = curz;
      } while ( reg3 != endPtr );
      break;
    case 2:
      reg1 = -(dy>>1);
      do {
        reg1 += dx;
        if ( reg1 > 0 ) {
           reg3 ++;
           arc_xmin--;
           arc_xmax--;
           reg4 ++; reg4++;
           reg1 -= dy; 
        }
        reg3 += ystep; 
        arc_ymin--;
        arc_ymax--;
        reg4 += zystep; 
        reg2 += zrate; 
        curz = reg2;
        reg2 >>= 10;
        SETARCPIXEL(reg4,reg3,reg2);
        reg2 = curz;
      } while ( reg3 != endPtr );
      break;
    default:
      reg1 = -(dy>>1);
      do {
        reg1 += dx;
        if ( reg1 > 0 ) {
           reg3 ++;
           arc_xmin--;
           arc_xmax--;
           reg4 ++; reg4++;
           reg1 -= dy; 
        }
        reg3 -= ystep; 
        arc_ymin++;
        arc_ymax++;
        reg4 -= zystep; 
        reg2 += zrate; 
        curz = reg2;
        reg2 >>= 10;
        SETARCPIXEL(reg4,reg3,reg2);
        reg2 = curz;
      } while ( reg3 != endPtr );
  }
}

static void Nlm_Cylin3DHitT ( Nlm_Cylin3DPtr p )
{
  register long reg1;
  register long reg2;
  register long reg3;
  register long reg4;
  double        arc_clenXY;
  Uint1Ptr      radiusF;
  long          c_x1,c_x2;
  long          c_y1,c_y2;
  long          c_z1,c_z2;
  long          c_dx,c_dy,c_dz;
  long          rad;
  long          needChk;
  long          ystep;
  long          zystep;
  long          arc_x, arc_y;
  long          arc_porErr, arc_stepErr;
  long          arc_errX, arc_errY;
  long          arcN_x, arcN_y, arc_z;
  long          arcN_rMax, arcN_rMax2, arcN_rMin2;
  long          arcN_errX, arcN_errY, arcN_errBase;
  long          arc_stepSin, arc_stepCos;
  long          xx1,xx2;
  long          yy1,yy2;
  long          zz1,zz2;
  long          status2;
  long          orient;
  long          endPtr;
  long          offset1;
  long          offset2;
  long          zrate;
  long          dx, dy;
  long          curz;
  long          arc_scale1;
  long          arc_offset1;
  long          arc_xmin, arc_ymin, arc_xmax, arc_ymax;

  reg1 = (long)p;

  /* Rotate and check */
  reg2 = Nlm_stCon.a[0][0];
  reg3 = ((Nlm_Cylin3DPtr)reg1)->x1 / (long)reg2;
  reg4 = ((Nlm_Cylin3DPtr)reg1)->x2 / (long)reg2;
  reg2 = Nlm_stCon.a[0][1];
  reg3 += ((Nlm_Cylin3DPtr)reg1)->y1 / (long)reg2;
  reg4 += ((Nlm_Cylin3DPtr)reg1)->y2 / (long)reg2;
  reg2 = Nlm_stCon.a[0][2];
  reg3 += ((Nlm_Cylin3DPtr)reg1)->z1 / (long)reg2;
  reg4 += ((Nlm_Cylin3DPtr)reg1)->z2 / (long)reg2;
  reg2 = Nlm_stCon.c[0];
  reg3 += reg2;
  reg4 += reg2;
  c_x1 = reg3; c_x2 = reg4;

  reg2 = Nlm_stCon.a[1][0];
  reg3 = ((Nlm_Cylin3DPtr)reg1)->x1 / (long)reg2;
  reg4 = ((Nlm_Cylin3DPtr)reg1)->x2 / (long)reg2;
  reg2 = Nlm_stCon.a[1][1];
  reg3 += ((Nlm_Cylin3DPtr)reg1)->y1 / (long)reg2;
  reg4 += ((Nlm_Cylin3DPtr)reg1)->y2 / (long)reg2;
  reg2 = Nlm_stCon.a[1][2];
  reg3 += ((Nlm_Cylin3DPtr)reg1)->z1 / (long)reg2;
  reg4 += ((Nlm_Cylin3DPtr)reg1)->z2 / (long)reg2;
  reg2 = Nlm_stCon.c[1];
  reg3 += reg2;
  reg4 += reg2;
  c_y1 = reg3; c_y2 = reg4;

  reg2 = Nlm_stCon.a[2][0];
  reg3 = ((Nlm_Cylin3DPtr)reg1)->x1 / (long)reg2;
  reg4 = ((Nlm_Cylin3DPtr)reg1)->x2 / (long)reg2;
  reg2 = Nlm_stCon.a[2][1];
  reg3 += ((Nlm_Cylin3DPtr)reg1)->y1 / (long)reg2;
  reg4 += ((Nlm_Cylin3DPtr)reg1)->y2 / (long)reg2;
  reg2 = Nlm_stCon.a[2][2];
  reg3 += ((Nlm_Cylin3DPtr)reg1)->z1 / (long)reg2;
  reg4 += ((Nlm_Cylin3DPtr)reg1)->z2 / (long)reg2;
  reg2 = Nlm_stCon.c[2];
  reg3 += reg2;
  reg4 += reg2;
  c_z1 = reg3; c_z2 = reg4; c_dz = reg4-reg3;
  if ( c_dz > 0 ){
    reg2 = c_z1; c_z1 = c_z2; c_z2 = reg2;
    reg2 = c_y1; c_y1 = c_y2; c_y2 = reg2;
    reg2 = c_x1; c_x1 = c_x2; c_x2 = reg2;
    c_dz = -c_dz;
  }
  reg2 = ((Nlm_Cylin3DPtr)reg1)->radius / Nlm_stCon.scale;
  reg2 = (reg2*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-c_z1);
  if ( reg2 > 127 ) return;
  if ( reg2 == 0 ) reg2 = 1;
  radiusF = &(Nlm_Isqrt[reg2*(reg2+1)]);
  rad = reg2;
  needChk = 0;
  reg2 = Nlm_stCon.width>>1;
  reg3 = c_x1-reg2;
  c_x1 = reg2 + (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-c_z1);
  reg3 = c_x2-reg2;
  reg4 = reg2 + (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-c_z2);
  c_x2 = reg4;
  reg3 = c_x1;
  c_dx = reg4-reg3;
  if ( reg3 < reg4 ) {
    reg3 -= rad; reg4 += rad;
    if ( (reg4 < Nlm_stCon.xmin)||(reg3 > Nlm_stCon.xmax) ) return;
    reg3 -= 4; reg4 += 4;
    if ( (reg4 > Nlm_stCon.xmax)||(reg3 < Nlm_stCon.xmin) ) needChk = 1;
  } else {
    reg3 += rad; reg4 -= rad;
    if ( (reg3 < Nlm_stCon.xmin)||(reg4 > Nlm_stCon.xmax) ) return;
    reg3 += 4; reg4 -= 4;
    if ( (reg3 > Nlm_stCon.xmax)||(reg4 < Nlm_stCon.xmin) ) needChk = 1;
  }
  reg2 = Nlm_stCon.height>>1;
  reg3 = c_y1-reg2;
  c_y1 = reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-c_z1);
  reg3 = c_y2-reg2;
  reg4 = reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-c_z2);
  c_y2 = reg4;
  reg3 = c_y1;
  c_dy = reg4-reg3;
  if ( reg3 < reg4 ) {
    reg3 -= rad; reg4 += rad;
    if ( (reg4 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) return;
    reg3 -= 4; reg4 += 4;
    if ( (reg4 > Nlm_stCon.ymax)||(reg3 < Nlm_stCon.ymin) ) needChk = 1;
  } else {
    reg3 += rad; reg4 -= rad;
    if ( (reg3 < Nlm_stCon.ymin)||(reg4 > Nlm_stCon.ymax) ) return;
    reg3 += 4; reg4 -= 4;
    if ( (reg3 > Nlm_stCon.ymax)||(reg4 < Nlm_stCon.ymin) ) needChk = 1;
  }
  reg3 = c_z1;
  reg3 = (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-reg3);
  reg4 = c_z2;
  reg4 = (reg4*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-reg4);
  c_z1 = reg3; c_z2 = reg4; c_dz = reg4-reg3;

  /* c_x1,c_y1,c_z1 - first (top) point    
     c_x2,c_y2,c_z2 - second point
     c_dx,c_dy,c_dz - vector 
     rad            - radius
     needChk        - 1 if need */

  if ( (c_dx==0) && (c_dy==0) )
    return;
  ystep  = Nlm_stCon.widthCur * sizeof(Nlm_stCon.idBuffer[0]);
  zystep = Nlm_stCon.widthCur * sizeof(Nlm_stCon.zBuffer[0]);

  /* Elips - start */
  reg2 = c_dx*c_dx + c_dy*c_dy;
  arc_clenXY = sqrt ( (double)reg2 );
  reg3 = c_dz*c_dz;
  reg2 += reg3;
  reg4 = reg2/16000 + 1;
  reg2 /= reg4;
  reg3 /= reg4;
  if ( reg3 == 0 ) {
    reg2 = 16000; reg3 = 1;
  }
  arcN_rMax2 = reg2;
  arcN_rMin2 = reg3;
  reg2 = rad;
  arc_stepCos = arc_x = (long)((double)(-c_dy*reg2)/arc_clenXY); 
  arc_stepSin = arc_y = (long)((double)(c_dx*reg2)/arc_clenXY);
  arcN_rMax = arc_stepErr = reg2<<2;
  arc_porErr = reg2<<1;
  arcN_x= -arcN_rMax; arcN_y = 0;
  arcN_errBase = 0;
  arc_x--; 
  arc_errX = arc_stepErr; arc_errY = 0;
  arcBTotal = 0;
  arcBMinX = arcBMinY = -1000;
  arcBMaxX = arcBMaxY = 1000;
  arc_scale1 = (Nlm_stCon.zmaxPersp1-c_z1);
  arc_offset1 = arc_scale1>>1;
  do {
    for ( ;; ){
      reg1 = arc_porErr;
      reg3 = arc_errX; reg4 = arc_errY;
      if ( reg3 > reg1 ){
        arc_x ++; reg3 -= arc_stepErr;
      } else if ( reg3 < -reg1 ){
        arc_x --; reg3 += arc_stepErr;
      } else if ( reg4 > reg1 ){
        arc_y ++; reg4 -= arc_stepErr;
      } else if ( reg4 < -reg1 ){
        arc_y --; reg4 += arc_stepErr;
      } else break;
      arc_errX = reg3; arc_errY = reg4;
      /* we have arc_x, arc_y, rad and radiusF */
      reg1 = arc_y;
      if ( reg1 < 0 ) reg1 = -reg1;
      reg1 = (long)radiusF[reg1];
      if ( arc_x > reg1 ) {
        /*arc_x = reg1;*/
        arc_z = 0;
      } else if ( arc_x < -reg1 ) {
        /*arc_x = -reg1;*/
        arc_z = 0;
      } else {
        arc_z = Nlm_Isqrt[reg1*(reg1+1)+arc_x];
      }
      /* we have arc_x, arc_y, arc_z and rad */
      arcBufferX[arcBTotal] = arc_x*arc_scale1+arc_offset1;
      arcBufferY[arcBTotal] = arc_y*arc_scale1+arc_offset1;
      arcBufferZ[arcBTotal++] = arc_z*arc_scale1+arc_offset1;
      if ( needChk ) {
        if ( arcBMinX > arc_x ) arcBMinX = arc_x;
        if ( arcBMaxX < arc_x ) arcBMaxX = arc_x;
        if ( arcBMinY > arc_y ) arcBMinY = arc_y;
        if ( arcBMaxY < arc_y ) arcBMaxY = arc_y;
      }
    }
    if ( arcN_x < 0 ){
      arcN_errX = arcN_errBase + (arcN_x<<1) + 1;
      arcN_errY = arcN_errBase + (arcN_rMax2*((arcN_y<<1) + 1))/arcN_rMin2;
      if ( arcN_errX <= 0 ){
        if ( (arcN_errY <= 0) && (arcN_errY > arcN_errX) ){
          arcN_y++; arcN_errBase = arcN_errY; 
          arc_errX += arc_stepSin; arc_errY -= arc_stepCos;
        } else {
          arcN_x++; arcN_errBase = arcN_errX; 
          arc_errX -= arc_stepCos; arc_errY -= arc_stepSin;
        }
      } else {
        arcN_y++; arcN_errBase = arcN_errY; 
        arc_errX += arc_stepSin; arc_errY -= arc_stepCos;
      }
    } else {
      arcN_errX = arcN_errBase + (arcN_x<<1) + 1;
      arcN_errY = arcN_errBase + (arcN_rMax2*(-(arcN_y<<1) + 1))/arcN_rMin2;
      if ( (arcN_errY <= 0) && (arcN_y > 0) ){
        if ( (arcN_errX <= 0) && (arcN_errX > arcN_errY) ){
          arcN_x++; arcN_errBase = arcN_errX; 
          arc_errX -= arc_stepCos; arc_errY -= arc_stepSin;
        } else {
          arcN_y--; arcN_errBase = arcN_errY; 
          arc_errX -= arc_stepSin; arc_errY += arc_stepCos;
        }
      } else {
        arcN_x++; arcN_errBase = arcN_errX; 
        arc_errX -= arc_stepCos; arc_errY -= arc_stepSin;
      }
    }
  } while ( (arcN_x <= arcN_rMax)&&(arcN_y >= 0) );
  reg1 = rad;
  Nlm_stCon.xmin -= reg1;
  Nlm_stCon.xmax += reg1; 
  Nlm_stCon.ymin -= reg1;
  Nlm_stCon.ymax += reg1;
  if ( needChk ) {
    /* Calculate status1(reg1) and status2(reg2) */
    reg1 = 0;
    reg2 = 0;
    reg3 = c_x1;
    reg4 = Nlm_stCon.xmin;
    if ( reg3 < reg4 ) reg1 = 1;
    else if ( reg3 > Nlm_stCon.xmax ) reg1 = 2;
    reg3 = c_x2;
    if ( reg3 < reg4 ) reg2 = 1;
    else if ( reg3 > Nlm_stCon.xmax ) reg2 = 2;

    reg3 = c_y1;
    reg4 = Nlm_stCon.ymin;
    if ( reg3 < reg4 ) reg1 += 6;
    else if ( reg3 > Nlm_stCon.ymax ) reg1 += 3;
    reg3 = c_y2;
    if ( reg3 < reg4 ) reg2 += 6;
    else if ( reg3 > Nlm_stCon.ymax ) reg2 += 3;

    /* Save status2*/
    status2 = reg2;

    /* Calculate the first point (xx1,yy1,zz1)*/
    switch ( reg1 ) {
      case 0: 
        xx1 = c_x1; yy1=c_y1; zz1=c_z1;
        break;
      case 1:
        reg1 = c_x1; reg2 = Nlm_stCon.xmin;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy1 = reg3;
        xx1 = reg2;
        zz1 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 2:
        reg1 = c_x1; reg2 = Nlm_stCon.xmax;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy1 = reg3;
        xx1 = reg2;
        zz1 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 3:
        reg1 = c_y1; reg2 = Nlm_stCon.ymax;
        reg4 = c_y2-reg1;
        reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
        xx1 = reg3;
        yy1 = reg2;
        zz1 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 4:
        reg1 = c_x1; reg2 = Nlm_stCon.xmin;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy1 = reg3;
        xx1 = reg2;
        if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
          reg1 = c_y1; reg2 = Nlm_stCon.ymax;
          reg4 = c_y2-reg1;
          reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
          xx1 = reg3;
          yy1 = reg2;
        }
        zz1 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 5:
        reg1 = c_x1; reg2 = Nlm_stCon.xmax;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy1 = reg3;
        xx1 = reg2;
        if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
          reg1 = c_y1; reg2 = Nlm_stCon.ymax;
          reg4 = c_y2-reg1;
          reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
          xx1 = reg3;
          yy1 = reg2;
        }
        zz1 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 6:
        reg1 = c_y1; reg2 = Nlm_stCon.ymin;
        reg4 = c_y2-reg1;
        reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
        xx1 = reg3;
        yy1 = reg2;
        zz1 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 7:
        reg1 = c_x1; reg2 = Nlm_stCon.xmin;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy1 = reg3;
        xx1 = reg2;
        if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
          reg1 = c_y1; reg2 = Nlm_stCon.ymin;
          reg4 = c_y2-reg1;
          reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
          xx1 = reg3;
          yy1 = reg2;
        }
        zz1 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      default:
        reg1 = c_x1; reg2 = Nlm_stCon.xmax;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy1 = reg3;
        xx1 = reg2;
        if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
          reg1 = c_y1; reg2 = Nlm_stCon.ymin;
          reg4 = c_y2-reg1;
          reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
          xx1 = reg3;
          yy1 = reg2;
        }
        zz1 = c_z1 + c_dz*(reg2-reg1)/reg4;
    }

    /* Calculate the second point (xx2,yy2,zz2)*/
    switch ( status2 ) {
      case 0: 
        xx2 = c_x2; yy2=c_y2; zz2=c_z2;
        break;
      case 1:
        reg1 = c_x1; reg2 = Nlm_stCon.xmin;
        reg4 = c_x2-reg1;
        yy2 = c_y1 + c_dy*(reg2-reg1)/reg4;
        xx2 = reg2;
        zz2 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 2:
        reg1 = c_x1; reg2 = Nlm_stCon.xmax;
        reg4 = c_x2-reg1;
        yy2 = c_y1 + c_dy*(reg2-reg1)/reg4;
        xx2 = reg2;
        zz2 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 3:
        reg1 = c_y1; reg2 = Nlm_stCon.ymax;
        reg4 = c_y2-reg1;
        xx2 = c_x1 + c_dx*(reg2-reg1)/reg4;
        yy2 = reg2;
        zz2 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 4:
        reg1 = c_x1; reg2 = Nlm_stCon.xmin;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy2 = reg3;
        xx2 = reg2;
        if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
          reg1 = c_y1; reg2 = Nlm_stCon.ymax;
          reg4 = c_y2-reg1;
          reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
          xx2 = reg3;
          yy2 = reg2;
        }
        zz2 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 5:
        reg1 = c_x1; reg2 = Nlm_stCon.xmax;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy2 = reg3;
        xx2 = reg2;
        if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
          reg1 = c_y1; reg2 = Nlm_stCon.ymax;
          reg4 = c_y2-reg1;
          reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
          xx2 = reg3;
          yy2 = reg2;
        }
        zz2 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 6:
        reg1 = c_y1; reg2 = Nlm_stCon.ymin;
        reg4 = c_y2-reg1;
        xx2 = c_x1 + c_dx*(reg2-reg1)/reg4;
        yy2 = reg2;
        zz2 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      case 7:
        reg1 = c_x1; reg2 = Nlm_stCon.xmin;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy2 = reg3;
        xx2 = reg2;
        if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
          reg1 = c_y1; reg2 = Nlm_stCon.ymin;
          reg4 = c_y2-reg1;
          reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
          xx2 = reg3;
          yy2 = reg2;
        }
        zz2 = c_z1 + c_dz*(reg2-reg1)/reg4;
        break;
      default:
        reg1 = c_x1; reg2 = Nlm_stCon.xmax;
        reg4 = c_x2-reg1;
        reg3 = c_y1 + c_dy*(reg2-reg1)/reg4;
        yy2 = reg3;
        xx2 = reg2;
        if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
          reg1 = c_y1; reg2 = Nlm_stCon.ymin;
          reg4 = c_y2-reg1;
          reg3 = c_x1 + c_dx*(reg2-reg1)/reg4;
          xx2 = reg3;
          yy2 = reg2;
        }
        zz2 = c_z1 + c_dz*(reg2-reg1)/reg4;
    }
    if ( (xx1==xx2) && (yy1==yy2) ) {
      reg1 = rad;
      Nlm_stCon.xmin += reg1;
      Nlm_stCon.xmax -= reg1; 
      Nlm_stCon.ymin += reg1;
      Nlm_stCon.ymax -= reg1; 
      return;
    }
  } else {
    yy1 = c_y1; yy2 = c_y2;
    xx1 = c_x1; xx2= c_x2;
    zz1= c_z1; zz2 = c_z2;
  }
  reg3 = xx1;
  reg4 = xx2;
  if ( reg3 < reg4 ) {
    if ( (reg3 < Nlm_stCon.xmin)||(reg4 > Nlm_stCon.xmax) ) {
      reg1 = rad;
      Nlm_stCon.xmin += reg1;
      Nlm_stCon.xmax -= reg1; 
      Nlm_stCon.ymin += reg1;
      Nlm_stCon.ymax -= reg1; 
      return;
    }
    reg1 = 0;
  } else {
    if ( (reg4 < Nlm_stCon.xmin)||(reg3 > Nlm_stCon.xmax) ) {
      reg1 = rad;
      Nlm_stCon.xmin += reg1;
      Nlm_stCon.xmax -= reg1; 
      Nlm_stCon.ymin += reg1;
      Nlm_stCon.ymax -= reg1;
      return;
    }
    reg1 = 1;
  }
  reg3 = yy1;
  reg4 = yy2;
  if ( reg3 < reg4 ) {
    if ( (reg3 < Nlm_stCon.ymin)||(reg4 > Nlm_stCon.ymax) ) {
      reg1 = rad;
      Nlm_stCon.xmin += reg1;
      Nlm_stCon.xmax -= reg1; 
      Nlm_stCon.ymin += reg1;
      Nlm_stCon.ymax -= reg1; 
      return;
    }
  } else {
    if ( (reg4 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
      reg1 = rad;
      Nlm_stCon.xmin += reg1;
      Nlm_stCon.xmax -= reg1; 
      Nlm_stCon.ymin += reg1;
      Nlm_stCon.ymax -= reg1; 
      return;
    }
    reg1 |= 0x2;
  }
  orient = reg1;
  reg1 = rad;
  Nlm_stCon.xmin += reg1;
  Nlm_stCon.xmax -= reg1; 
  Nlm_stCon.ymin += reg1;
  Nlm_stCon.ymax -= reg1;

  /* Calculate offsets and pointers */
  reg1 = (yy1-Nlm_stCon.ymin) * Nlm_stCon.widthCur + (xx1-Nlm_stCon.xmin);
  offset1 = reg1;
  reg3 = (long)(Nlm_stCon.idBuffer + reg1);
  reg2 = (yy2-Nlm_stCon.ymin) * Nlm_stCon.widthCur + (xx2-Nlm_stCon.xmin);
  offset2 = reg2;
  reg4 = (long)(Nlm_stCon.idBuffer + reg2);
  /* reg3 - statrt reg4 - end */

  /* Will be:
     reg1 - last orientation
     reg2 - zz1
     reg3 - start
     reg4 - zPtr
     endPtr saved in endPtr */
  reg1 = orient;
  switch ( reg1 ){
    case 0:
      dy = yy2 - yy1;
      reg2 = xx2 - xx1;
      dx = reg2;
      if ( reg2>dy ) {
        zrate = ((zz2 - zz1)<<10)/reg2;
      } else {
        zrate = ((zz2 - zz1)<<10)/dy;
        reg1 = 2;
      }
      reg2 = zz1;
      endPtr = reg4;
      arc_xmin = Nlm_stCon.xmin - xx1;
      arc_xmax = Nlm_stCon.xmax - xx1;
      arc_ymin = Nlm_stCon.ymin - yy1;
      arc_ymax = Nlm_stCon.ymax - yy1;
      reg4 = (long)(Nlm_stCon.zBuffer + offset1);
      break;
    case 1:
      dy = yy2 - yy1;
      reg2 = xx1 - xx2;
      if ( reg2 > dy ) {
        zrate = ((zz1 - zz2)<<10)/reg2;
      } else {
        zrate = ((zz1 - zz2)<<10)/dy;
        reg1 = 3;
      }
      dx = reg2;
      reg2 = reg3; reg3 = reg4; reg4 = reg2;
      reg2 = zz2;
      endPtr = reg4;
      arc_xmin = Nlm_stCon.xmin - xx2;
      arc_xmax = Nlm_stCon.xmax - xx2;
      arc_ymin = Nlm_stCon.ymin - yy2;
      arc_ymax = Nlm_stCon.ymax - yy2;
      reg4 = (long)(Nlm_stCon.zBuffer + offset2);
      break;
    case 2:
      dy = yy1 - yy2;
      reg2 = xx2 - xx1;
      if ( reg2 > dy ){
        zrate = ((zz2 - zz1)<<10)/reg2;
        reg1 = 1;
      } else {
        zrate = ((zz2 - zz1)<<10)/dy;
        reg1 = 3;
      }
      dx = reg2;
      reg2 = zz1;
      endPtr = reg4;
      arc_xmin = Nlm_stCon.xmin - xx1;
      arc_xmax = Nlm_stCon.xmax - xx1;
      arc_ymin = Nlm_stCon.ymin - yy1;
      arc_ymax = Nlm_stCon.ymax - yy1;
      reg4 = (long)(Nlm_stCon.zBuffer + offset1);
      break;
    default:
      dy = yy1 - yy2;
      reg2 = xx1 - xx2;
      if ( reg2 > dy ){
        zrate = ((zz1 - zz2)<<10)/reg2;
        reg1 = 0;
      } else {
        zrate = ((zz1 - zz2)<<10)/dy;
        reg1 = 2;
      }
      dx = reg2;
      reg2 = reg3; reg3 = reg4; reg4 = reg2;
      reg2 = zz2;
      endPtr = reg4;
      arc_xmin = Nlm_stCon.xmin - xx2;
      arc_xmax = Nlm_stCon.xmax - xx2;
      arc_ymin = Nlm_stCon.ymin - yy2;
      arc_ymax = Nlm_stCon.ymax - yy2;
      reg4 = (long)(Nlm_stCon.zBuffer + offset2);
  }

  SETARCPIXELID(reg4,reg3,reg2,p);
  reg2 <<= 10;
  /* Will be: reg1 - err */
  switch ( reg1 ){
    case 0:
      reg1 = -(dx>>1);
      do {
        reg1 += dy;
        if ( reg1 > 0 ){
           reg3 += ystep; 
           arc_ymin--;
           arc_ymax--;
           reg4 += zystep; 
           reg1 -= dx; 
        }
        reg3 += sizeof(Nlm_stCon.idBuffer[0]); reg4++; reg4++;
        arc_xmin--;
        arc_xmax--;
        reg2 += zrate; 
        curz = reg2;
        reg2 >>= 10;
        SETARCPIXELID(reg4,reg3,reg2,p);
        reg2 = curz;
      } while ( reg3 != endPtr );
      break;
    case 1:
      reg1 = -(dx>>1);
      do {
        reg1 += dy;
        if ( reg1 > 0 ){
           reg3 -= ystep; 
           arc_ymin++;
           arc_ymax++;
           reg4 -= zystep; 
           reg1 -= dx; 
        }
        reg3 += sizeof(Nlm_stCon.idBuffer[0]); reg4++; reg4++;
        arc_xmin--;
        arc_xmax--;
        reg2 += zrate; 
        curz = reg2;
        reg2 >>= 10;
        SETARCPIXELID(reg4,reg3,reg2,p);
        reg2 = curz;
      } while ( reg3 != endPtr );
      break;
    case 2:
      reg1 = -(dy>>1);
      do {
        reg1 += dx;
        if ( reg1 > 0 ) {
           reg3 += sizeof(Nlm_stCon.idBuffer[0]);
           arc_xmin--;
           arc_xmax--;
           reg4 ++; reg4++;
           reg1 -= dy; 
        }
        reg3 += ystep; 
        arc_ymin--;
        arc_ymax--;
        reg4 += zystep; 
        reg2 += zrate; 
        curz = reg2;
        reg2 >>= 10;
        SETARCPIXELID(reg4,reg3,reg2,p);
        reg2 = curz;
      } while ( reg3 != endPtr );
      break;
    default:
      reg1 = -(dy>>1);
      do {
        reg1 += dx;
        if ( reg1 > 0 ) {
           reg3 += sizeof(Nlm_stCon.idBuffer[0]);
           arc_xmin--;
           arc_xmax--;
           reg4++; reg4++;
           reg1 -= dy; 
        }
        reg3 -= ystep; 
        arc_ymin++;
        arc_ymax++;
        reg4 -= zystep; 
        reg2 += zrate; 
        curz = reg2;
        reg2 >>= 10;
        SETARCPIXELID(reg4,reg3,reg2,p);
        reg2 = curz;
      } while ( reg3 != endPtr );
  }
}

static void Nlm_Cylin3DGetL ( Nlm_Cylin3DPtr p )
{
  register BigScalar ax;
  register BigScalar bx;
  long minXY, maxXY;

  ax = (BigScalar)p;
  bx = (BigScalar)(((Nlm_PSeg3DPtr)ax)->base.parent);
  if ( ((Nlm_Cylin3DPtr)ax)->x1 > ((Nlm_Cylin3DPtr)ax)->x2 ){
    maxXY = ((Nlm_Cylin3DPtr)ax)->x1; minXY = ((Nlm_Cylin3DPtr)ax)->x2;
  } else {
    maxXY = ((Nlm_Cylin3DPtr)ax)->x2; minXY = ((Nlm_Cylin3DPtr)ax)->x1;
  }
  maxXY += ((Nlm_Cylin3DPtr)ax)->radius;
  minXY -= ((Nlm_Cylin3DPtr)ax)->radius;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.minX > minXY )
       ((Nlm_PSeg3DPtr)bx)->segBox.minX = minXY;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.maxX < maxXY )
       ((Nlm_PSeg3DPtr)bx)->segBox.maxX = maxXY;
  if ( ((Nlm_Cylin3DPtr)ax)->y1 > ((Nlm_Cylin3DPtr)ax)->y2 ){
    maxXY = ((Nlm_Cylin3DPtr)ax)->y1; minXY = ((Nlm_Cylin3DPtr)ax)->y2;
  } else {
    maxXY = ((Nlm_Cylin3DPtr)ax)->y2; minXY = ((Nlm_Cylin3DPtr)ax)->y1;
  }
  maxXY += ((Nlm_Cylin3DPtr)ax)->radius;
  minXY -= ((Nlm_Cylin3DPtr)ax)->radius;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.minY > minXY )
       ((Nlm_PSeg3DPtr)bx)->segBox.minY = minXY;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.maxY < maxXY )
       ((Nlm_PSeg3DPtr)bx)->segBox.maxY = maxXY;
  if ( ((Nlm_Cylin3DPtr)ax)->z1 > ((Nlm_Cylin3DPtr)ax)->z2 ){
    maxXY = ((Nlm_Cylin3DPtr)ax)->z1; minXY = ((Nlm_Cylin3DPtr)ax)->z2;
  } else {
    maxXY = ((Nlm_Cylin3DPtr)ax)->z2; minXY = ((Nlm_Cylin3DPtr)ax)->z1;
  }
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.minZ > minXY )
       ((Nlm_PSeg3DPtr)bx)->segBox.minZ = minXY;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.maxZ < maxXY )
       ((Nlm_PSeg3DPtr)bx)->segBox.maxZ = maxXY;
}

static Nlm_PrimDef3D cylinDef = {
  (Nlm_PrimDraw3D)Nlm_Cylin3DDraw,
  (Nlm_PrimHitTest3D)Nlm_Cylin3DHitT,
  (Nlm_PrimGetLimits3D)Nlm_Cylin3DGetL,
  (Nlm_PrimCleanup3D)Nlm_Empty3D,
  (Nlm_Int2)LINE3D
};

Nlm_Prim3D Nlm_AddCylinder3D ( Nlm_Picture3D pic, Nlm_Segment3D segment, 
                               BigScalar userData, Uint1 layer, Uint1 color,
                               Int4 x1, Int4 y1, Int4 z1,
                               Int4 x2, Int4 y2, Int4 z2, Uint4 radius )
{
  Nlm_Cylin3D l;

  Nlm_stCon.pic = pic;
  Nlm_DiagReset ();
  l.base.userData = userData;
  l.base.layer = layer;
  l.base.color = color;
  l.base.fTable = &cylinDef;
  l.base.status = 0;
  l.x1 = (long)x1; l.x2 = (long)x2;
  l.y1 = (long)y1; l.y2 = (long)y2;
  l.z1 = (long)z1; l.z2 = (long)z2;
  l.radius = (long)radius;
  return (Nlm_Prim3D) Nlm_AddPrim3D ( (Nlm_PSeg3DPtr)segment, 
                                      (Nlm_Base3DPtr)&l,
                                      sizeof(Nlm_Cylin3D), 
                                      "AddCylin3D" );
}


