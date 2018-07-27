/*   prim3d1.c
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
* File Name:  prim3d1.c
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
#define SETPIXEL(zptr,imageptr,z,color,colorScale) \
if((Uint2)z>=*((Uint2Ptr)zptr)) \
{*((Uint2Ptr)zptr)=(Uint2)z;*((Uint1Ptr)imageptr)= \
(Uint1)(color+(z<<8)/colorScale);}

#define SETPIXELID(zptr,imageptr,z,curID) \
if((Uint2)z>=*((Uint2Ptr)zptr)) \
{*((Uint2Ptr)zptr)=(Uint2)z;*((VoidPtr*)imageptr)=(VoidPtr)curID;}

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

/*****************************************************************************
*
*   STATIC VARIABLE
*
*****************************************************************************/

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
*   LINE
*
*****************************************************************************/
typedef struct Nlm_line3d {
  Nlm_Base3D  base;
  long        x1,y1,z1;
  long        x2,y2,z2;
} Nlm_Line3D, PNTR Nlm_Line3DPtr;

static void Nlm_Line3DDraw ( Nlm_Line3DPtr p )
{
  register long reg1;
  register long reg2;
  register long reg3;
  register long reg4;
  long          x1,x2;
  long          y1,y2;
  long          z1,z2;
  long          dx, dy, dz;
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
  long          ystep;
  long          zystep;
  long          curz;

  reg1 = (long)p;

  /* Rotate and check */
  reg2 = Nlm_stCon.a[0][0];
  reg3 = ((Nlm_Line3DPtr)reg1)->x1 / (long)reg2;
  reg4 = ((Nlm_Line3DPtr)reg1)->x2 / (long)reg2;
  reg2 = Nlm_stCon.a[0][1];
  reg3 += ((Nlm_Line3DPtr)reg1)->y1 / (long)reg2;
  reg4 += ((Nlm_Line3DPtr)reg1)->y2 / (long)reg2;
  reg2 = Nlm_stCon.a[0][2];
  reg3 += ((Nlm_Line3DPtr)reg1)->z1 / (long)reg2;
  reg4 += ((Nlm_Line3DPtr)reg1)->z2 / (long)reg2;
  reg2 = Nlm_stCon.c[0];
  reg3 += reg2;
  reg4 += reg2;
  x1 = reg3; x2 = reg4;

  reg2 = Nlm_stCon.a[1][0];
  reg3 = ((Nlm_Line3DPtr)reg1)->x1 / (long)reg2;
  reg4 = ((Nlm_Line3DPtr)reg1)->x2 / (long)reg2;
  reg2 = Nlm_stCon.a[1][1];
  reg3 += ((Nlm_Line3DPtr)reg1)->y1 / (long)reg2;
  reg4 += ((Nlm_Line3DPtr)reg1)->y2 / (long)reg2;
  reg2 = Nlm_stCon.a[1][2];
  reg3 += ((Nlm_Line3DPtr)reg1)->z1 / (long)reg2;
  reg4 += ((Nlm_Line3DPtr)reg1)->z2 / (long)reg2;
  reg2 = Nlm_stCon.c[1];
  reg3 += reg2;
  reg4 += reg2;
  y1 = reg3; y2 = reg4;

  reg2 = Nlm_stCon.a[2][0];
  reg3 = ((Nlm_Line3DPtr)reg1)->x1 / (long)reg2;
  reg4 = ((Nlm_Line3DPtr)reg1)->x2 / (long)reg2;
  reg2 = Nlm_stCon.a[2][1];
  reg3 += ((Nlm_Line3DPtr)reg1)->y1 / (long)reg2;
  reg4 += ((Nlm_Line3DPtr)reg1)->y2 / (long)reg2;
  reg2 = Nlm_stCon.a[2][2];
  reg3 += ((Nlm_Line3DPtr)reg1)->z1 / (long)reg2;
  reg4 += ((Nlm_Line3DPtr)reg1)->z2 / (long)reg2;
  reg2 = Nlm_stCon.c[2];
  reg3 += reg2;
  reg4 += reg2;
  z1 = reg3; z2 = reg4;

  reg2 = Nlm_stCon.width>>1;
  reg3 = x1-reg2;
  x1 = reg2 + (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-z1);
  reg3 = x2-reg2;
  reg4 = reg2 + (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-z2);
  x2 = reg4;
  reg3 = x1;
  if ( reg3 < reg4 ) {
   if ( (reg4 < Nlm_stCon.xmin)||(reg3 > Nlm_stCon.xmax) ) return;
  } else {
   if ( (reg3 < Nlm_stCon.xmin)||(reg4 > Nlm_stCon.xmax) ) return;
  }
  dx = reg4-reg3;
  reg2 = Nlm_stCon.height>>1;
  reg3 = y1-reg2;
  y1 = reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-z1);
  reg3 = y2-reg2;
  reg4 = reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-z2);
  y2 = reg4;
  reg3 = y1;
  if ( reg3 < reg4 ) {
   if ( (reg4 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) return;
  } else {
   if ( (reg3 < Nlm_stCon.ymin)||(reg4 > Nlm_stCon.ymax) ) return;
  }
  dy = reg4-reg3;
  reg3 = z1;
  reg3 = (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-reg3);
  reg4 = z2;
  reg4 = (reg4*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-reg4);
  z1 = reg3; z2 = reg4; dz = reg4-reg3;

  /* x1,y1,z1 - first point    
    x2,y2,z2 - second point
    dx,dy,dz - vector */

  /* Set color and colorscale */
  color = Nlm_stCon.colorOffset + 
    ((Nlm_Line3DPtr)reg1)->base.color * Nlm_stCon.colorStep + 
    (Nlm_stCon.colorStep >> 1);
  colorScale = Nlm_stCon.colorScale << 1;
  ystep = Nlm_stCon.width;
  zystep = ystep<<1;

  /* Calculate status1(reg1) and status2(reg2) */
  reg1 = 0;
  reg2 = 0;
  reg3 = x1;
  reg4 = Nlm_stCon.xmin;
  if ( reg3 < reg4 ) reg1 = 1;
  else if ( reg3 > Nlm_stCon.xmax ) reg1 = 2;
  reg3 = x2;
  if ( reg3 < reg4 ) reg2 = 1;
  else if ( reg3 > Nlm_stCon.xmax ) reg2 = 2;

  reg3 = y1;
  reg4 = Nlm_stCon.ymin;
  if ( reg3 < reg4 ) reg1 += 6;
  else if ( reg3 > Nlm_stCon.ymax ) reg1 += 3;
  reg3 = y2;
  if ( reg3 < reg4 ) reg2 += 6;
  else if ( reg3 > Nlm_stCon.ymax ) reg2 += 3;

  /* Save status2*/
  status2 = reg2;

  /* Calculate the first point (xx1,yy1,zz1)*/
  switch ( reg1 ) {
    case 0: 
      xx1 = x1; yy1=y1; zz1=z1;
      break;
    case 1:
      reg1 = x1; reg2 = Nlm_stCon.xmin;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy1 = reg3;
      xx1 = reg2;
      zz1 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 2:
      reg1 = x1; reg2 = Nlm_stCon.xmax;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy1 = reg3;
      xx1 = reg2;
      zz1 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 3:
      reg1 = y1; reg2 = Nlm_stCon.ymax;
      reg4 = y2-reg1;
      reg3 = x1 + dx*(reg2-reg1)/reg4;
      xx1 = reg3;
      yy1 = reg2;
      zz1 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 4:
      reg1 = x1; reg2 = Nlm_stCon.xmin;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy1 = reg3;
      xx1 = reg2;
      if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
        reg1 = y1; reg2 = Nlm_stCon.ymax;
        reg4 = y2-reg1;
        reg3 = x1 + dx*(reg2-reg1)/reg4;
        xx1 = reg3;
        yy1 = reg2;
      }
      zz1 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 5:
      reg1 = x1; reg2 = Nlm_stCon.xmax;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy1 = reg3;
      xx1 = reg2;
      if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
        reg1 = y1; reg2 = Nlm_stCon.ymax;
        reg4 = y2-reg1;
        reg3 = x1 + dx*(reg2-reg1)/reg4;
        xx1 = reg3;
        yy1 = reg2;
      }
      zz1 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 6:
      reg1 = y1; reg2 = Nlm_stCon.ymin;
      reg4 = y2-reg1;
      reg3 = x1 + dx*(reg2-reg1)/reg4;
      xx1 = reg3;
      yy1 = reg2;
      zz1 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 7:
      reg1 = x1; reg2 = Nlm_stCon.xmin;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy1 = reg3;
      xx1 = reg2;
      if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
        reg1 = y1; reg2 = Nlm_stCon.ymin;
        reg4 = y2-reg1;
        reg3 = x1 + dx*(reg2-reg1)/reg4;
        xx1 = reg3;
        yy1 = reg2;
      }
      zz1 = z1 + dz*(reg2-reg1)/reg4;
      break;
    default:
      reg1 = x1; reg2 = Nlm_stCon.xmax;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy1 = reg3;
      xx1 = reg2;
      if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
        reg1 = y1; reg2 = Nlm_stCon.ymin;
        reg4 = y2-reg1;
        reg3 = x1 + dx*(reg2-reg1)/reg4;
        xx1 = reg3;
        yy1 = reg2;
      }
      zz1 = z1 + dz*(reg2-reg1)/reg4;
  }

  /* Calculate the second point (xx2,yy2,zz2)*/
  switch ( status2 ) {
    case 0: 
      xx2 = x2; yy2=y2; zz2=z2;
      break;
    case 1:
      reg1 = x1; reg2 = Nlm_stCon.xmin;
      reg4 = x2-reg1;
      yy2 = y1 + dy*(reg2-reg1)/reg4;
      xx2 = reg2;
      zz2 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 2:
      reg1 = x1; reg2 = Nlm_stCon.xmax;
      reg4 = x2-reg1;
      yy2 = y1 + dy*(reg2-reg1)/reg4;
      xx2 = reg2;
      zz2 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 3:
      reg1 = y1; reg2 = Nlm_stCon.ymax;
      reg4 = y2-reg1;
      xx2 = x1 + dx*(reg2-reg1)/reg4;
      yy2 = reg2;
      zz2 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 4:
      reg1 = x1; reg2 = Nlm_stCon.xmin;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy2 = reg3;
      xx2 = reg2;
      if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
        reg1 = y1; reg2 = Nlm_stCon.ymax;
        reg4 = y2-reg1;
        reg3 = x1 + dx*(reg2-reg1)/reg4;
        xx2 = reg3;
        yy2 = reg2;
      }
      zz2 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 5:
      reg1 = x1; reg2 = Nlm_stCon.xmax;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy2 = reg3;
      xx2 = reg2;
      if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
        reg1 = y1; reg2 = Nlm_stCon.ymax;
        reg4 = y2-reg1;
        reg3 = x1 + dx*(reg2-reg1)/reg4;
        xx2 = reg3;
        yy2 = reg2;
      }
      zz2 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 6:
      reg1 = y1; reg2 = Nlm_stCon.ymin;
      reg4 = y2-reg1;
      xx2 = x1 + dx*(reg2-reg1)/reg4;
      yy2 = reg2;
      zz2 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 7:
      reg1 = x1; reg2 = Nlm_stCon.xmin;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy2 = reg3;
      xx2 = reg2;
      if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
        reg1 = y1; reg2 = Nlm_stCon.ymin;
        reg4 = y2-reg1;
        reg3 = x1 + dx*(reg2-reg1)/reg4;
        xx2 = reg3;
        yy2 = reg2;
      }
      zz2 = z1 + dz*(reg2-reg1)/reg4;
      break;
    default:
      reg1 = x1; reg2 = Nlm_stCon.xmax;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy2 = reg3;
      xx2 = reg2;
      if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
        reg1 = y1; reg2 = Nlm_stCon.ymin;
        reg4 = y2-reg1;
        reg3 = x1 + dx*(reg2-reg1)/reg4;
        xx2 = reg3;
        yy2 = reg2;
      }
      zz2 = z1 + dz*(reg2-reg1)/reg4;
  }

  reg3 = xx1;
  reg4 = xx2;
  if ( reg3 < reg4 ) {
    if ( (reg3 < Nlm_stCon.xmin)||(reg4 > Nlm_stCon.xmax) ) return;
    reg1 = 0;
  } else {
    if ( (reg4 < Nlm_stCon.xmin)||(reg3 > Nlm_stCon.xmax) ) return;
    reg1 = 1;
  }
  reg3 = yy1;
  reg4 = yy2;
  if ( reg3 < reg4 ) {
    if ( (reg3 < Nlm_stCon.ymin)||(reg4 > Nlm_stCon.ymax) ) return;
  } else {
    if ( (reg4 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) return;
    reg1 |= 0x2;
  }
  orient = reg1;

  /* Calculate offsets and pointers */
  reg1 = yy1 * Nlm_stCon.width + xx1;
  offset1 = reg1;
  reg3 = (long)(Nlm_stCon.image + reg1);
  reg2 = yy2 * Nlm_stCon.width + xx2;
  offset2 = reg2;
  reg4 = (long)(Nlm_stCon.image + reg2);
  if ( reg3 == reg4 ){
    reg2 = (long)(Nlm_stCon.zBuffer + reg1);
    reg1 = zz1;
    if ( reg1 < zz2 ) reg1 = zz2;
    SETPIXEL ( reg2, reg3, reg1, color, colorScale );
    return;
  }
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
      reg4 = (long)(Nlm_stCon.zBuffer + offset2);
  }

  SETPIXEL(reg4,reg3,reg2,color,colorScale);
  reg2 <<= 10;
  /* Will be: reg1 - err */
  switch ( reg1 ){
    case 0:
      reg1 = -(dx>>1);
      do {
        reg1 += dy;
        if ( reg1 > 0 ){
           reg3 += ystep; 
           reg4 += zystep; 
           reg1 -= dx; 
        }
        reg3++; reg4++; reg4++;
        reg2 += zrate; 
        curz = reg2;
        reg2 >>= 10;
        SETPIXEL(reg4,reg3,reg2,color,colorScale);
        reg2 = curz;
      } while ( reg3 != endPtr );
      break;
    case 1:
      reg1 = -(dx>>1);
      do {
        reg1 += dy;
        if ( reg1 > 0 ){
           reg3 -= ystep; 
           reg4 -= zystep; 
           reg1 -= dx; 
        }
      reg3++; reg4++; reg4++;
        reg2 += zrate; 
        curz = reg2;
        reg2 >>= 10;
        SETPIXEL(reg4,reg3,reg2,color,colorScale);
        reg2 = curz;
      } while ( reg3 != endPtr );
      break;
    case 2:
      reg1 = -(dy>>1);
      do {
        reg1 += dx;
        if ( reg1 > 0 ) {
           reg3 ++;
           reg4 ++; reg4++;
           reg1 -= dy; 
        }
        reg3 += ystep; 
        reg4 += zystep; 
        reg2 += zrate; 
        curz = reg2;
        reg2 >>= 10;
        SETPIXEL(reg4,reg3,reg2,color,colorScale);
        reg2 = curz;
      } while ( reg3 != endPtr );
      break;
    default:
      reg1 = -(dy>>1);
      do {
        reg1 += dx;
        if ( reg1 > 0 ) {
           reg3 ++;
           reg4 ++; reg4++;
           reg1 -= dy; 
        }
        reg3 -= ystep; 
        reg4 -= zystep; 
        reg2 += zrate; 
        curz = reg2;
        reg2 >>= 10;
        SETPIXEL(reg4,reg3,reg2,color,colorScale);
        reg2 = curz;
      } while ( reg3 != endPtr );
  }
}

static void Nlm_Line3DHitT ( Nlm_Line3DPtr p )
{
  register long reg1;
  register long reg2;
  register long reg3;
  register long reg4;
  long          x1,x2;
  long          y1,y2;
  long          z1,z2;
  long          dx, dy, dz;
  long          xx1,xx2;
  long          yy1,yy2;
  long          zz1,zz2;
  long          status2;
  long          orient;
  long          endPtr;
  long          offset1;
  long          offset2;
  long          zrate;
  long          ystep;
  long          zystep;
  long          curz;

  reg1 = (long)p;

  /* Rotate and check */
  reg2 = Nlm_stCon.a[0][0];
  reg3 = ((Nlm_Line3DPtr)reg1)->x1 / (long)reg2;
  reg4 = ((Nlm_Line3DPtr)reg1)->x2 / (long)reg2;
  reg2 = Nlm_stCon.a[0][1];
  reg3 += ((Nlm_Line3DPtr)reg1)->y1 / (long)reg2;
  reg4 += ((Nlm_Line3DPtr)reg1)->y2 / (long)reg2;
  reg2 = Nlm_stCon.a[0][2];
  reg3 += ((Nlm_Line3DPtr)reg1)->z1 / (long)reg2;
  reg4 += ((Nlm_Line3DPtr)reg1)->z2 / (long)reg2;
  reg2 = Nlm_stCon.c[0];
  reg3 += reg2;
  reg4 += reg2;
  x1 = reg3; x2 = reg4;

  reg2 = Nlm_stCon.a[1][0];
  reg3 = ((Nlm_Line3DPtr)reg1)->x1 / (long)reg2;
  reg4 = ((Nlm_Line3DPtr)reg1)->x2 / (long)reg2;
  reg2 = Nlm_stCon.a[1][1];
  reg3 += ((Nlm_Line3DPtr)reg1)->y1 / (long)reg2;
  reg4 += ((Nlm_Line3DPtr)reg1)->y2 / (long)reg2;
  reg2 = Nlm_stCon.a[1][2];
  reg3 += ((Nlm_Line3DPtr)reg1)->z1 / (long)reg2;
  reg4 += ((Nlm_Line3DPtr)reg1)->z2 / (long)reg2;
  reg2 = Nlm_stCon.c[1];
  reg3 += reg2;
  reg4 += reg2;
  y1 = reg3; y2 = reg4;

  reg2 = Nlm_stCon.a[2][0];
  reg3 = ((Nlm_Line3DPtr)reg1)->x1 / (long)reg2;
  reg4 = ((Nlm_Line3DPtr)reg1)->x2 / (long)reg2;
  reg2 = Nlm_stCon.a[2][1];
  reg3 += ((Nlm_Line3DPtr)reg1)->y1 / (long)reg2;
  reg4 += ((Nlm_Line3DPtr)reg1)->y2 / (long)reg2;
  reg2 = Nlm_stCon.a[2][2];
  reg3 += ((Nlm_Line3DPtr)reg1)->z1 / (long)reg2;
  reg4 += ((Nlm_Line3DPtr)reg1)->z2 / (long)reg2;
  reg2 = Nlm_stCon.c[2];
  reg3 += reg2;
  reg4 += reg2;
  z1 = reg3; z2 = reg4;

  reg2 = Nlm_stCon.width>>1;
  reg3 = x1-reg2;
  x1 = reg2 + (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-z1);
  reg3 = x2-reg2;
  reg4 = reg2 + (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-z2);
  x2 = reg4;
  reg3 = x1;
  if ( reg3 < reg4 ) {
   if ( (reg4 < Nlm_stCon.xmin)||(reg3 > Nlm_stCon.xmax) ) return;
  } else {
   if ( (reg3 < Nlm_stCon.xmin)||(reg4 > Nlm_stCon.xmax) ) return;
  }
  dx = reg4-reg3;
  reg2 = Nlm_stCon.height>>1;
  reg3 = y1-reg2;
  y1 = reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-z1);
  reg3 = y2-reg2;
  reg4 = reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-z2);
  y2 = reg4;
  reg3 = y1;
  if ( reg3 < reg4 ) {
   if ( (reg4 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) return;
  } else {
   if ( (reg3 < Nlm_stCon.ymin)||(reg4 > Nlm_stCon.ymax) ) return;
  }
  dy = reg4-reg3;
  reg3 = z1;
  reg3 = (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-reg3);
  reg4 = z2;
  reg4 = (reg4*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-reg4);
  z1 = reg3; z2 = reg4; dz = reg4-reg3;

  /* x1,y1,z1 - first point    
    x2,y2,z2 - second point
    dx,dy,dz - vector */

  ystep  = Nlm_stCon.widthCur * sizeof(Nlm_stCon.idBuffer[0]);
  zystep = Nlm_stCon.widthCur * sizeof(Nlm_stCon.zBuffer[0]);

  /* Calculate status1(reg1) and status2(reg2) */
  reg1 = 0;
  reg2 = 0;
  reg3 = x1;
  reg4 = Nlm_stCon.xmin;
  if ( reg3 < reg4 ) reg1 = 1;
  else if ( reg3 > Nlm_stCon.xmax ) reg1 = 2;
  reg3 = x2;
  if ( reg3 < reg4 ) reg2 = 1;
  else if ( reg3 > Nlm_stCon.xmax ) reg2 = 2;

  reg3 = y1;
  reg4 = Nlm_stCon.ymin;
  if ( reg3 < reg4 ) reg1 += 6;
  else if ( reg3 > Nlm_stCon.ymax ) reg1 += 3;
  reg3 = y2;
  if ( reg3 < reg4 ) reg2 += 6;
  else if ( reg3 > Nlm_stCon.ymax ) reg2 += 3;

  /* Save status2*/
  status2 = reg2;

  /* Calculate the first point (xx1,yy1,zz1)*/
  switch ( reg1 ) {
    case 0: 
      xx1 = x1; yy1=y1; zz1=z1;
      break;
    case 1:
      reg1 = x1; reg2 = Nlm_stCon.xmin;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy1 = reg3;
      xx1 = reg2;
      zz1 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 2:
      reg1 = x1; reg2 = Nlm_stCon.xmax;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy1 = reg3;
      xx1 = reg2;
      zz1 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 3:
      reg1 = y1; reg2 = Nlm_stCon.ymax;
      reg4 = y2-reg1;
      reg3 = x1 + dx*(reg2-reg1)/reg4;
      xx1 = reg3;
      yy1 = reg2;
      zz1 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 4:
      reg1 = x1; reg2 = Nlm_stCon.xmin;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy1 = reg3;
      xx1 = reg2;
      if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
        reg1 = y1; reg2 = Nlm_stCon.ymax;
        reg4 = y2-reg1;
        reg3 = x1 + dx*(reg2-reg1)/reg4;
        xx1 = reg3;
        yy1 = reg2;
      }
      zz1 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 5:
      reg1 = x1; reg2 = Nlm_stCon.xmax;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy1 = reg3;
      xx1 = reg2;
      if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
        reg1 = y1; reg2 = Nlm_stCon.ymax;
        reg4 = y2-reg1;
        reg3 = x1 + dx*(reg2-reg1)/reg4;
        xx1 = reg3;
        yy1 = reg2;
      }
      zz1 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 6:
      reg1 = y1; reg2 = Nlm_stCon.ymin;
      reg4 = y2-reg1;
      reg3 = x1 + dx*(reg2-reg1)/reg4;
      xx1 = reg3;
      yy1 = reg2;
      zz1 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 7:
      reg1 = x1; reg2 = Nlm_stCon.xmin;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy1 = reg3;
      xx1 = reg2;
      if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
        reg1 = y1; reg2 = Nlm_stCon.ymin;
        reg4 = y2-reg1;
        reg3 = x1 + dx*(reg2-reg1)/reg4;
        xx1 = reg3;
        yy1 = reg2;
      }
      zz1 = z1 + dz*(reg2-reg1)/reg4;
      break;
    default:
      reg1 = x1; reg2 = Nlm_stCon.xmax;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy1 = reg3;
      xx1 = reg2;
      if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
        reg1 = y1; reg2 = Nlm_stCon.ymin;
        reg4 = y2-reg1;
        reg3 = x1 + dx*(reg2-reg1)/reg4;
        xx1 = reg3;
        yy1 = reg2;
      }
      zz1 = z1 + dz*(reg2-reg1)/reg4;
  }

  /* Calculate the second point (xx2,yy2,zz2)*/
  switch ( status2 ) {
    case 0: 
      xx2 = x2; yy2=y2; zz2=z2;
      break;
    case 1:
      reg1 = x1; reg2 = Nlm_stCon.xmin;
      reg4 = x2-reg1;
      yy2 = y1 + dy*(reg2-reg1)/reg4;
      xx2 = reg2;
      zz2 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 2:
      reg1 = x1; reg2 = Nlm_stCon.xmax;
      reg4 = x2-reg1;
      yy2 = y1 + dy*(reg2-reg1)/reg4;
      xx2 = reg2;
      zz2 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 3:
      reg1 = y1; reg2 = Nlm_stCon.ymax;
      reg4 = y2-reg1;
      xx2 = x1 + dx*(reg2-reg1)/reg4;
      yy2 = reg2;
      zz2 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 4:
      reg1 = x1; reg2 = Nlm_stCon.xmin;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy2 = reg3;
      xx2 = reg2;
      if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
        reg1 = y1; reg2 = Nlm_stCon.ymax;
        reg4 = y2-reg1;
        reg3 = x1 + dx*(reg2-reg1)/reg4;
        xx2 = reg3;
        yy2 = reg2;
      }
      zz2 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 5:
      reg1 = x1; reg2 = Nlm_stCon.xmax;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy2 = reg3;
      xx2 = reg2;
      if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
        reg1 = y1; reg2 = Nlm_stCon.ymax;
        reg4 = y2-reg1;
        reg3 = x1 + dx*(reg2-reg1)/reg4;
        xx2 = reg3;
        yy2 = reg2;
      }
      zz2 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 6:
      reg1 = y1; reg2 = Nlm_stCon.ymin;
      reg4 = y2-reg1;
      xx2 = x1 + dx*(reg2-reg1)/reg4;
      yy2 = reg2;
      zz2 = z1 + dz*(reg2-reg1)/reg4;
      break;
    case 7:
      reg1 = x1; reg2 = Nlm_stCon.xmin;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy2 = reg3;
      xx2 = reg2;
      if ( (reg3 < Nlm_stCon.ymin) || (reg3 > Nlm_stCon.ymax) ) {
        reg1 = y1; reg2 = Nlm_stCon.ymin;
        reg4 = y2-reg1;
        reg3 = x1 + dx*(reg2-reg1)/reg4;
        xx2 = reg3;
        yy2 = reg2;
      }
      zz2 = z1 + dz*(reg2-reg1)/reg4;
      break;
    default:
      reg1 = x1; reg2 = Nlm_stCon.xmax;
      reg4 = x2-reg1;
      reg3 = y1 + dy*(reg2-reg1)/reg4;
      yy2 = reg3;
      xx2 = reg2;
      if ( (reg3 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) {
        reg1 = y1; reg2 = Nlm_stCon.ymin;
        reg4 = y2-reg1;
        reg3 = x1 + dx*(reg2-reg1)/reg4;
        xx2 = reg3;
        yy2 = reg2;
      }
      zz2 = z1 + dz*(reg2-reg1)/reg4;
  }

  reg3 = xx1;
  reg4 = xx2;
  if ( reg3 < reg4 ) {
    if ( (reg3 < Nlm_stCon.xmin)||(reg4 > Nlm_stCon.xmax) ) return;
    reg1 = 0;
  } else {
    if ( (reg4 < Nlm_stCon.xmin)||(reg3 > Nlm_stCon.xmax) ) return;
    reg1 = 1;
  }
  reg3 = yy1;
  reg4 = yy2;
  if ( reg3 < reg4 ) {
    if ( (reg3 < Nlm_stCon.ymin)||(reg4 > Nlm_stCon.ymax) ) return;
  } else {
    if ( (reg4 < Nlm_stCon.ymin)||(reg3 > Nlm_stCon.ymax) ) return;
    reg1 |= 0x2;
  }
  orient = reg1;

  /* Calculate offsets */
  reg3 = Nlm_stCon.xmin;
  xx1 -= reg3;  xx2 -= reg3;
  reg3 = Nlm_stCon.ymin;
  yy1 -= reg3; yy2 -=reg3;
  /* Calculate offsets and pointers */
  reg1 = yy1 * Nlm_stCon.widthCur + xx1;
  offset1 = reg1;
  reg3 = (long)(Nlm_stCon.idBuffer + reg1);
  reg2 = yy2 * Nlm_stCon.widthCur + xx2;
  offset2 = reg2;
  reg4 = (long)(Nlm_stCon.idBuffer + reg2);
  if ( reg3 == reg4 ){
    reg2 = (long)(Nlm_stCon.zBuffer + reg1);
    reg1 = zz1;
    if ( reg1 < zz2 ) reg1 = zz2;
    SETPIXELID ( reg2, reg3, reg1, p );
    return;
  }
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
      reg4 = (long)(Nlm_stCon.zBuffer + offset2);
  }

  SETPIXELID(reg4,reg3,reg2,p);
  reg2 <<= 10;
  /* Will be: reg1 - err */
  switch ( reg1 ){
    case 0:
      reg1 = -(dx>>1);
      do {
        reg1 += dy;
        if ( reg1 > 0 ){
           reg3 += ystep; 
           reg4 += zystep; 
           reg1 -= dx; 
        }
        reg3 += sizeof(Nlm_stCon.idBuffer[0]); reg4++; reg4++;
        reg2 += zrate; 
        curz = reg2;
        reg2 >>= 10;
        SETPIXELID(reg4,reg3,reg2,p);
        reg2 = curz;
      } while ( reg3 != endPtr );
      break;
    case 1:
      reg1 = -(dx>>1);
      do {
        reg1 += dy;
        if ( reg1 > 0 ){
           reg3 -= ystep; 
           reg4 -= zystep; 
           reg1 -= dx; 
        }
        reg3 += sizeof(Nlm_stCon.idBuffer[0]); reg4++; reg4++;
        reg2 += zrate; 
        curz = reg2;
        reg2 >>= 10;
        SETPIXELID(reg4,reg3,reg2,p);
        reg2 = curz;
      } while ( reg3 != endPtr );
      break;
    case 2:
      reg1 = -(dy>>1);
      do {
        reg1 += dx;
        if ( reg1 > 0 ) {
           reg3 += sizeof(Nlm_stCon.idBuffer[0]);
           reg4 ++; reg4++;
           reg1 -= dy; 
        }
        reg3 += ystep; 
        reg4 += zystep; 
        reg2 += zrate; 
        curz = reg2;
        reg2 >>= 10;
        SETPIXELID(reg4,reg3,reg2,p);
        reg2 = curz;
      } while ( reg3 != endPtr );
      break;
    default:
      reg1 = -(dy>>1);
      do {
        reg1 += dx;
        if ( reg1 > 0 ) {
           reg3 += sizeof(Nlm_stCon.idBuffer[0]);
           reg4 ++; reg4++;
           reg1 -= dy; 
        }
        reg3 -= ystep; 
        reg4 -= zystep; 
        reg2 += zrate; 
        curz = reg2;
        reg2 >>= 10;
        SETPIXELID(reg4,reg3,reg2,p);
        reg2 = curz;
      } while ( reg3 != endPtr );
  }
}

static void Nlm_Line3DGetL ( Nlm_Line3DPtr p )
{
  register BigScalar ax;
  register BigScalar bx;
  long minXY, maxXY;

  ax = (BigScalar)p;
  bx = (BigScalar)(((Nlm_PSeg3DPtr)ax)->base.parent);
  if ( ((Nlm_Line3DPtr)ax)->x1 > ((Nlm_Line3DPtr)ax)->x2 ){
    maxXY = ((Nlm_Line3DPtr)ax)->x1; minXY = ((Nlm_Line3DPtr)ax)->x2;
  } else {
    maxXY = ((Nlm_Line3DPtr)ax)->x2; minXY = ((Nlm_Line3DPtr)ax)->x1;
  }
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.minX > minXY )
       ((Nlm_PSeg3DPtr)bx)->segBox.minX = minXY;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.maxX < maxXY )
       ((Nlm_PSeg3DPtr)bx)->segBox.maxX = maxXY;
  if ( ((Nlm_Line3DPtr)ax)->y1 > ((Nlm_Line3DPtr)ax)->y2 ){
    maxXY = ((Nlm_Line3DPtr)ax)->y1; minXY = ((Nlm_Line3DPtr)ax)->y2;
  } else {
    maxXY = ((Nlm_Line3DPtr)ax)->y2; minXY = ((Nlm_Line3DPtr)ax)->y1;
  }
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.minY > minXY )
       ((Nlm_PSeg3DPtr)bx)->segBox.minY = minXY;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.maxY < maxXY )
       ((Nlm_PSeg3DPtr)bx)->segBox.maxY = maxXY;
  if ( ((Nlm_Line3DPtr)ax)->z1 > ((Nlm_Line3DPtr)ax)->z2 ){
    maxXY = ((Nlm_Line3DPtr)ax)->z1; minXY = ((Nlm_Line3DPtr)ax)->z2;
  } else {
    maxXY = ((Nlm_Line3DPtr)ax)->z2; minXY = ((Nlm_Line3DPtr)ax)->z1;
  }
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.minZ > minXY )
       ((Nlm_PSeg3DPtr)bx)->segBox.minZ = minXY;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.maxZ < maxXY )
       ((Nlm_PSeg3DPtr)bx)->segBox.maxZ = maxXY;
}

static Nlm_PrimDef3D lineDef = {
  (Nlm_PrimDraw3D)Nlm_Line3DDraw,
  (Nlm_PrimHitTest3D)Nlm_Line3DHitT,
  (Nlm_PrimGetLimits3D)Nlm_Line3DGetL,
  (Nlm_PrimCleanup3D)Nlm_Empty3D,
  (Nlm_Int2)LINE3D
};

Nlm_Prim3D Nlm_AddLine3D ( Nlm_Picture3D pic, Nlm_Segment3D segment, 
                           BigScalar userData, Uint1 layer, Uint1 color,
                           Int4 x1, Int4 y1, Int4 z1,
                           Int4 x2, Int4 y2, Int4 z2 )
{
  Nlm_Line3D l;

  Nlm_stCon.pic = pic;
  Nlm_DiagReset ();
  l.base.userData = userData;
  l.base.layer = layer;
  l.base.color = color;
  l.base.fTable = &lineDef;
  l.base.status = 0;
  l.x1 = (long)x1; l.x2 = (long)x2;
  l.y1 = (long)y1; l.y2 = (long)y2;
  l.z1 = (long)z1; l.z2 = (long)z2;
  return (Nlm_Prim3D) Nlm_AddPrim3D ( (Nlm_PSeg3DPtr)segment, 
                                      (Nlm_Base3DPtr)&l,
                                      sizeof(Nlm_Line3D), 
                                      "AddLine3D" );
}


