/*   prim3d5.c
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
* File Name:  prim3d5.c
*
* Author:  Alex Smirnov
*
* Version Creation Date:   04/03/95
*
* $Revision: 6.2 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* $Log: prim3d5.c,v $
* Revision 6.2  1998/08/12 23:04:05  vakatov
* [64-bit platforms]  Made "idBuffer" be VoidPtr[] rather than Int4[];
* Fixed bugs mostly caused by casting between Int4 vars and 8-byte pointers
*
* Revision 6.1  1997/11/26 21:30:09  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.0  1997/08/25 18:56:42  madden
* Revision changed to 6.0
*
* Revision 5.3  1996/07/25 14:51:05  epstein
* BigScalar and Int4 changes for OSF1 portability
*
 * Revision 5.2  1996/06/24  16:25:37  hogue
 * Put sizeof(long) into Mem operations that assumed 4-byte long
 *
*
* log added by C. Hogue 24 June 96
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
#define SETPIXEL(zptr,imageptr,z,color10) \
if((Uint2)z>=*((Uint2Ptr)zptr)) \
{*((Uint2Ptr)zptr)=(Uint2)z;*((Uint1Ptr)imageptr)= \
(Uint1)(color10>>10);}

#define SETPIXELID(zptr,imageptr,z,curID) \
if((Uint2)z>=*((Uint2Ptr)zptr)) \
{*((Uint2Ptr)zptr)=(Uint2)z;*((VoidPtr*)imageptr)=curID;}

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
*   POLYGON
*
*****************************************************************************/
typedef struct Nlm_poly3d {
  Nlm_Base3D  base;
  long        x1,y1,z1;
  long        x2,y2,z2;
  long        vNum; 
  long        xmin, ymin, zmin;
  long        xmax, ymax, zmax;
  long        x,y,z,rad;
  Int4Ptr     vert;
} Nlm_Poly3D, PNTR Nlm_Poly3DPtr;

static void Nlm_Poly3DDraw ( Nlm_Poly3DPtr p )
{
  register long reg1;
  register long reg2;
  register long reg3;
  register long reg4;
  Int4Ptr       vPtr;
  long          x,y;
  long          vx[3];
  long          vy[3];
  long          vz[3];
  long          baseColor;
  long          sphColor, lineColor;
  long          dX1, dY1, dZ1;
  long          dX2, dY2, dZ2;
  long          curBaseColor;
  long          xsort[3];
  long          ysort[3];
  long          zsort[3];
  long          savex1, savex2,savecurz,x1,x2,curz;
  long          stepZX, stepColorX;
  long          stepXY1, stepXY2, stepXYm;
  long          stepZY, stepZYm, stepColorY, stepColorYm;
  long          color10, savecolor10;  
  Int2          needCheck, needCheckFr;
  Int2          vi;
  Int2          mLeft;
  long          Nlm_stCon_image_end = (long)Nlm_stCon.image +
                                       Nlm_stCon.width * Nlm_stCon.height - 1;

  reg1 = (long)p;
  vPtr = ((Nlm_Poly3DPtr)reg1)->vert;
  if ( vPtr == NULL ) return;

  /* Rotate and check */
  reg2 = ((Nlm_Poly3DPtr)reg1)->x;
  reg3 = ((Nlm_Poly3DPtr)reg1)->y;
  reg4 = ((Nlm_Poly3DPtr)reg1)->z;
  x = reg2 / Nlm_stCon.a[0][0] + reg3 / Nlm_stCon.a[0][1] +
      reg4 / Nlm_stCon.a[0][2] + Nlm_stCon.c[0];
  y = reg2 / Nlm_stCon.a[1][0] + reg3 / Nlm_stCon.a[1][1] +
      reg4 / Nlm_stCon.a[1][2] + Nlm_stCon.c[1];
  reg4 = reg2 / Nlm_stCon.a[2][0] + reg3 / Nlm_stCon.a[2][1] +
      reg4 / Nlm_stCon.a[2][2] + Nlm_stCon.c[2];
  reg4 = Nlm_stCon.zmaxPersp1-reg4;
  reg2 = Nlm_stCon.width>>1;
  reg3 = x-reg2;
  x = reg2 + (reg3*Nlm_stCon.zmaxPersp)/reg4;
  reg2 = Nlm_stCon.height>>1;
  reg3 = y-reg2;
  y =  reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/reg4;
  reg2 = ((Nlm_Poly3DPtr)reg1)->rad / Nlm_stCon.scale;
  reg2 = (reg2*Nlm_stCon.zmaxPersp)/reg4;
  needCheck = 0;
  x += reg2;
  if ( x < Nlm_stCon.xmin ) return;
  if ( x > Nlm_stCon.xmax ) needCheck=1;
  x -= reg2<<1;
  if ( x < Nlm_stCon.xmin ) needCheck=1;
  if ( x > Nlm_stCon.xmax ) return;
  y += reg2;
  if ( y < Nlm_stCon.ymin ) return;
  if ( y > Nlm_stCon.ymax ) needCheck=1;
  y -= reg2<<1;
  if ( y < Nlm_stCon.ymin ) needCheck=1;
  if ( y > Nlm_stCon.ymax ) return;

  reg2 = Nlm_stCon.a[0][0];
  reg3 = ((Nlm_Poly3DPtr)reg1)->x1 / (long)reg2;
  reg4 = ((Nlm_Poly3DPtr)reg1)->x2 / (long)reg2;
  reg2 = Nlm_stCon.a[0][1];
  reg3 += ((Nlm_Poly3DPtr)reg1)->y1 / (long)reg2;
  reg4 += ((Nlm_Poly3DPtr)reg1)->y2 / (long)reg2;
  reg2 = Nlm_stCon.a[0][2];
  reg3 += ((Nlm_Poly3DPtr)reg1)->z1 / (long)reg2;
  reg4 += ((Nlm_Poly3DPtr)reg1)->z2 / (long)reg2;
  reg2 = Nlm_stCon.c[0];
  reg3 += reg2;
  reg4 += reg2;
  vx[1] = reg3; vx[2] = reg4;

  reg2 = Nlm_stCon.a[1][0];
  reg3 = ((Nlm_Poly3DPtr)reg1)->x1 / (long)reg2;
  reg4 = ((Nlm_Poly3DPtr)reg1)->x2 / (long)reg2;
  reg2 = Nlm_stCon.a[1][1];
  reg3 += ((Nlm_Poly3DPtr)reg1)->y1 / (long)reg2;
  reg4 += ((Nlm_Poly3DPtr)reg1)->y2 / (long)reg2;
  reg2 = Nlm_stCon.a[1][2];
  reg3 += ((Nlm_Poly3DPtr)reg1)->z1 / (long)reg2;
  reg4 += ((Nlm_Poly3DPtr)reg1)->z2 / (long)reg2;
  reg2 = Nlm_stCon.c[1];
  reg3 += reg2;
  reg4 += reg2;
  vy[1] = reg3; vy[2] = reg4;

  reg2 = Nlm_stCon.a[2][0];
  reg3 = ((Nlm_Poly3DPtr)reg1)->x1 / (long)reg2;
  reg4 = ((Nlm_Poly3DPtr)reg1)->x2 / (long)reg2;
  reg2 = Nlm_stCon.a[2][1];
  reg3 += ((Nlm_Poly3DPtr)reg1)->y1 / (long)reg2;
  reg4 += ((Nlm_Poly3DPtr)reg1)->y2 / (long)reg2;
  reg2 = Nlm_stCon.a[2][2];
  reg3 += ((Nlm_Poly3DPtr)reg1)->z1 / (long)reg2;
  reg4 += ((Nlm_Poly3DPtr)reg1)->z2 / (long)reg2;
  reg2 = Nlm_stCon.c[2];
  reg3 += reg2;
  reg4 += reg2;
  vz[1] = reg3; vz[2] = reg4;

  reg2 = Nlm_stCon.width>>1;
  reg3 = vx[1]-reg2;
  vx[1] = reg2 + (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-vz[1]);
  reg3 = vx[2]-reg2;
  vx[2] = reg2 + (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-vz[2]);
  reg2 = Nlm_stCon.height>>1;
  reg3 = vy[1]-reg2;
  vy[1] = reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-vz[1]);
  reg3 = vy[2]-reg2;
  vy[2] = reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-vz[2]);
  reg3 = vz[1];
  vz[1] = (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-reg3);
  reg3 = vz[2];
  vz[2] = (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-reg3);

  baseColor = (Nlm_stCon.colorOffset +
    ((Nlm_Poly3DPtr)reg1)->base.color * Nlm_stCon.colorStep)<<10;
  sphColor = ((Nlm_stCon.colorStep*2)<<10)/3;
  lineColor = Nlm_stCon.colorStep - (sphColor>>10) - 1;
  baseColor += sphColor*1414/3146;

  vi = 2;
  while ( *vPtr != 0x7FFFFFFFL ){
    reg1 = vi;
    reg1++;
    if ( reg1 > 2 ) reg1 = 0;
    vi = (Int2)reg1;
    reg1 = (long)vPtr;
    reg2 = *(Int4Ptr)reg1; reg1 += sizeof(Int4);
    reg3 = *(Int4Ptr)reg1; reg1 += sizeof(Int4);
    reg4 = *(Int4Ptr)reg1; reg1 += sizeof(Int4);
    dX1 = *(Int4Ptr)reg1; reg1 += sizeof(Int4);
    dY1 = *(Int4Ptr)reg1; reg1 += sizeof(Int4);
    dZ1 = *(Int4Ptr)reg1; reg1 += sizeof(Int4);
    vPtr = (Int4Ptr)reg1;
    vx[vi] = reg2 / Nlm_stCon.a[0][0] + reg3 / Nlm_stCon.a[0][1] +
             reg4 / Nlm_stCon.a[0][2] + Nlm_stCon.c[0];
    vy[vi] = reg2 / Nlm_stCon.a[1][0] + reg3 / Nlm_stCon.a[1][1] +
             reg4 / Nlm_stCon.a[1][2] + Nlm_stCon.c[1];
    vz[vi] = reg2 / Nlm_stCon.a[2][0] + reg3 / Nlm_stCon.a[2][1] +
             reg4 / Nlm_stCon.a[2][2] + Nlm_stCon.c[2];
    reg4 = Nlm_stCon.zmaxPersp1-vz[vi];
    reg2 = Nlm_stCon.width>>1;
    reg3 = vx[vi]-reg2;
    vx[vi] = reg2 + (reg3*Nlm_stCon.zmaxPersp)/reg4;
    reg2 = Nlm_stCon.height>>1;
    reg3 = vy[vi]-reg2;
    vy[vi] =  reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/reg4;
    vz[vi] = (vz[vi]*Nlm_stCon.zmaxPersp)/reg4;

    reg2 = dX1; reg3 = dY1; reg4 = dZ1;
    dX1 = reg2 / Nlm_stCon.a[0][0] + reg3 / Nlm_stCon.a[0][1] +
          reg4 / Nlm_stCon.a[0][2];
    dY1 = reg2 / Nlm_stCon.a[1][0] + reg3 / Nlm_stCon.a[1][1] +
          reg4 / Nlm_stCon.a[1][2];
    dZ1 = reg2 / Nlm_stCon.a[2][0] + reg3 / Nlm_stCon.a[2][1] +
          reg4 / Nlm_stCon.a[2][2];
    reg1 = dZ1 - dX1 + dY1;
    if ( Nlm_stCon.scale < 128000 ){
      reg4 = 128000/Nlm_stCon.scale + 1;
      reg2 = dX1/reg4; reg3 = dY1/reg4; reg4 = dZ1/reg4;
    } else {
      reg2 = dX1; reg3 = dY1; reg4 = dZ1;
    }
    if ( (reg2*(vx[vi]-(Nlm_stCon.width>>1)) Y_PLUS
          reg3*(vy[vi]-(Nlm_stCon.height>>1))-
          reg4*Nlm_stCon.zmaxPersp) > 0 ){
      reg1 =- reg1;
    }
    reg1 = reg1*1000 /(1000000000L/Nlm_stCon.scale);
    if ( reg1 < -1414 ) reg1 = -1414;
    if ( reg1 > 1732 ) reg1 = 1732;
    curBaseColor = baseColor + (reg1*sphColor) / 3146;

    reg1 = vy[0];
    reg2 = vy[1];
    reg3 = vy[2];
    if ( reg1 < reg2 ){
      if ( reg3 < reg1 ){           /* 312 */
        xsort[0] = vx[2]; ysort[0] = reg3; zsort[0] = vz[2];
        xsort[1] = vx[0]; ysort[1] = reg1; zsort[1] = vz[0];
        xsort[2] = vx[1]; ysort[2] = reg2; zsort[2] = vz[1];
      } else if ( reg3 < reg2 ){    /* 132 */
        xsort[0] = vx[0]; ysort[0] = reg1; zsort[0] = vz[0];
        xsort[1] = vx[2]; ysort[1] = reg3; zsort[1] = vz[2];
        xsort[2] = vx[1]; ysort[2] = reg2; zsort[2] = vz[1];
      } else {                      /* 123 */
        xsort[0] = vx[0]; ysort[0] = reg1; zsort[0] = vz[0];
        xsort[1] = vx[1]; ysort[1] = reg2; zsort[1] = vz[1];
        xsort[2] = vx[2]; ysort[2] = reg3; zsort[2] = vz[2];
      }
    } else {
      if ( reg3 < reg2 ){           /* 321 */
        xsort[0] = vx[2]; ysort[0] = reg3; zsort[0] = vz[2];
        xsort[1] = vx[1]; ysort[1] = reg2; zsort[1] = vz[1];
        xsort[2] = vx[0]; ysort[2] = reg1; zsort[2] = vz[0];
      } else if ( reg3 < reg1 ){    /* 231 */
        xsort[0] = vx[1]; ysort[0] = reg2; zsort[0] = vz[1];
        xsort[1] = vx[2]; ysort[1] = reg3; zsort[1] = vz[2];
        xsort[2] = vx[0]; ysort[2] = reg1; zsort[2] = vz[0];
      } else {                      /* 213 */
        xsort[0] = vx[1]; ysort[0] = reg2; zsort[0] = vz[1];
        xsort[1] = vx[0]; ysort[1] = reg1; zsort[1] = vz[0];
        xsort[2] = vx[2]; ysort[2] = reg3; zsort[2] = vz[2];
      }
    }
    dX1 = xsort[1] - xsort[0];
    dY1 = ysort[1] - ysort[0];
    dZ1 = zsort[1] - zsort[0];
    dX2 = xsort[2] - xsort[0];
    dY2 = ysort[2] - ysort[0];
    dZ2 = zsort[2] - zsort[0];
    reg1 = dY1*dZ2 - dZ1*dY2;
    reg3 = dX1*dY2 - dY1*dX2;
    if ( reg3 < 0 ) {
      reg1=-reg1; reg3=-reg3; 
    }
    if ( reg3 != 0 ) {
      stepZX = reg1*(-1024)/reg3;
      stepColorX = stepZX*lineColor/Nlm_stCon.zmax;
    } else {
      stepZX = stepColorX = 0;
    }
    needCheckFr = 0;
    if ( needCheck ){
      reg1 = ysort[0];
      reg2 = ysort[2];
      if ( (reg1 > Nlm_stCon.ymax)||
           (reg2 < Nlm_stCon.ymin) ) continue;
      if ( (reg1 < Nlm_stCon.ymin)||
           (reg2 > Nlm_stCon.ymax) ) needCheckFr = 1;
      reg1 = vx[0]; reg2 = vx[1];
      if ( reg1 > reg2 ) { reg3 = reg1; reg1 = reg2; reg2 = reg3; }
      reg3 = vx[2];
      if ( reg3 < reg1 ) reg1 = reg3;
      else if ( reg3 > reg2 ) reg2 = reg3;
      if ( (reg1 > Nlm_stCon.xmax)||
           (reg2 < Nlm_stCon.xmin) ) continue;
      if ( (reg1 < Nlm_stCon.xmin)||
           (reg2 > Nlm_stCon.xmax) ) needCheckFr = 1;
    }
    y = ysort[0];
    if ( y == ysort[1] ){
      if ( xsort[0] < xsort[1] ){
        savex1 = xsort[0];
        savex2 = xsort[1];
        savecurz = zsort[0];
      } else {
        savex1 = xsort[1];
        savex2 = xsort[0];
        savecurz = zsort[1];
      }
      if ( ysort[2] != y ){
        reg1 = ysort[2]-y;
        stepXY1 = ((xsort[2]-savex1)<<10)/reg1;
        stepXY2 = ((xsort[2]-savex2)<<10)/reg1;
        stepZY = ((zsort[2] - savecurz)<<10)/reg1;
        ysort[1] = 0x80000000L;
      } else {
        stepZY = stepXY1 = stepXY2 = 0;
      }
    } else {
      savex1 = savex2 = xsort[0];
      savecurz = zsort[0];
      reg1 = xsort[0]; reg2 = ysort[0];
      if ( ((ysort[1]-reg2)*(xsort[2]-reg1) - 
            (xsort[1]-reg1)*(ysort[2]-reg2)) > 0 ){
        mLeft = 1;
        reg3 = ysort[1]-reg2;
        stepXY1 = ((xsort[1]-savex1)<<10)/reg3;
        stepZY = ((zsort[1]-zsort[0])<<10)/reg3;
        stepXY2 = ((xsort[2]-savex1)<<10)/(ysort[2]-reg2);
        reg3 = ysort[2]-ysort[1];
        if ( reg3 != 0 ){
          stepXYm = ((xsort[2]-xsort[1])<<10)/reg3;
          stepZYm = ((zsort[2]-zsort[1])<<10)/reg3;
          stepColorYm = (stepZYm*lineColor)/Nlm_stCon.zmax;
        } else { 
          stepZYm = stepXYm = stepColorYm = 0;
        }
      } else {
        mLeft = 0;
        reg3 = ysort[2]-reg2;
        stepXY1 = ((xsort[2]-savex1)<<10)/reg3;
        stepZY = ((zsort[2]-zsort[0])<<10)/reg3;
        stepXY2 = ((xsort[1]-savex1)<<10)/(ysort[1]-reg2);
        reg3 = ysort[2]-ysort[1];
        if ( reg3 != 0 ){
          stepXYm = ((xsort[2]-xsort[1])<<10)/reg3;
        } else stepXYm = 0;
      }
    }
    stepColorY = (stepZY*lineColor)/Nlm_stCon.zmax;
    savex1 <<= 10;
    savex2 <<= 10;
    savecurz <<= 10;
    savecolor10 = curBaseColor + savecurz*lineColor/Nlm_stCon.zmax;
    /* y, savex1(10), savex2(10), savecurz(10), stepZX(10), 
      savecolor10, stepColorX(10), stepColorY, stepZY,
      stepXY1, stepXY2, 
      mLeft, stepXYm, stepColorYm, stepZYm*/
    for (; y<=ysort[2];y++){
      color10 = savecolor10;
      curz = savecurz;
      x1 = (savex1>>10)+1;  x2 = savex2>>10;
      /* y, x1(10), x2(10), curz(10), stepZX(10), color10, stepColorX(10) */
      if ( x2 < x1 ) x1 = x2;
      if ( needCheckFr ){
        if ( y > Nlm_stCon.ymax ) break;
        if ( y < Nlm_stCon.ymin ) {
          if ( y == ysort[1] ){
            if ( mLeft ) {
              stepXY1 = stepXYm;
              stepColorY = stepColorYm;
              stepZY = stepZYm;
              savex1 = xsort[1]<<10;
            } else {
              stepXY2 = stepXYm;
              savex2 = xsort[1]<<10;
            }
          }
          savecolor10 += stepColorY;
          savecurz += stepZY;
          savex1 += stepXY1; savex2 += stepXY2;
          continue;
        }
        reg2 = Nlm_stCon.xmin;
        reg3 = Nlm_stCon.xmax;
        reg1 = x1;
        if ( reg1 > reg3 ) {
          if ( y == ysort[1] ){
            if ( mLeft ) {
              stepXY1 = stepXYm;
              stepColorY = stepColorYm;
              stepZY = stepZYm;
              savex1 = xsort[1]<<10;
            } else {
              stepXY2 = stepXYm;
              savex2 = xsort[1]<<10;
            }
          }
          savecolor10 += stepColorY;
          savecurz += stepZY;
          savex1 += stepXY1; savex2 += stepXY2;
          continue;
        }
        if ( reg1 < reg2 ) {
          x1 = reg2;
          reg4 = reg2-reg1;
          curz += stepZX*reg4;
          color10 += stepColorX*reg4;
        }
        reg1 = x2;
        if ( reg1 < reg2 ) {
          if ( y == ysort[1] ){
            if ( mLeft ) {
              stepXY1 = stepXYm;
              stepColorY = stepColorYm;
              stepZY = stepZYm;
              savex1 = xsort[1]<<10;
            } else {
              stepXY2 = stepXYm;
              savex2 = xsort[1]<<10;
            }
          }
          savecolor10 += stepColorY;
          savecurz += stepZY;
          savex1 += stepXY1; savex2 += stepXY2;
          continue;
        }
        if ( reg1 > reg3 ) {
          x2 = reg3;
        }
      }
      reg1 = y*Nlm_stCon.width;
      reg3 = (long)(Nlm_stCon.zBuffer + (reg1 + x1));
      reg1 = (long)(Nlm_stCon.image + reg1 );
      reg2 = reg1 + x2;
      reg1 += x1;
      if (reg1 < (long)Nlm_stCon.image  ||
          Nlm_stCon_image_end < reg2)
        break;
      while ( ((Uint4)reg2 - (Uint4)reg1) >= 4 ){
        reg4 = curz;
        curz = reg4 + stepZX;
        reg4 >>= 10;
        SETPIXEL(reg3,reg1,reg4,color10);
        color10 += stepColorX;
        reg3++; reg3++;
        reg1++;
        reg4 = curz;
        curz = reg4 + stepZX;
        reg4 >>= 10;
        SETPIXEL(reg3,reg1,reg4,color10);
        color10 += stepColorX;
        reg3++; reg3++;
        reg1++;
        reg4 = curz;
        curz = reg4 + stepZX;
        reg4 >>= 10;
        SETPIXEL(reg3,reg1,reg4,color10);
        color10 += stepColorX;
        reg3++; reg3++;
        reg1++;
        reg4 = curz;
        curz = reg4 + stepZX;
        reg4 >>= 10;
        SETPIXEL(reg3,reg1,reg4,color10);
        color10 += stepColorX;
        reg3++; reg3++;
        reg1++;
      }
      do {
        reg4 = curz;
        curz = reg4 + stepZX;
        reg4 >>= 10;
        SETPIXEL(reg3,reg1,reg4,color10);
        color10 += stepColorX;
        reg3++; reg3++;
      } while (reg1++ != reg2 );
      if ( y == ysort[1] ){
        if ( mLeft ) {
          stepXY1 = stepXYm;
          stepColorY = stepColorYm;
          stepZY = stepZYm;
          savex1 = xsort[1]<<10;
        } else {
          stepXY2 = stepXYm;
          savex2 = xsort[1]<<10;
        }
      }
      savecolor10 += stepColorY;
      savecurz += stepZY;
      savex1 += stepXY1; savex2 += stepXY2;
    }
  }
}

static void Nlm_Poly3DHitT ( Nlm_Poly3DPtr p )
{
  register long reg1;
  register long reg2;
  register long reg3;
  register long reg4;
  Int4Ptr       vPtr;
  long          x,y;
  long          vx[3];
  long          vy[3];
  long          vz[3];
  long          dX1, dY1, dZ1;
  long          dX2, dY2, dZ2;
  long          xsort[3];
  long          ysort[3];
  long          zsort[3];
  long          savex1, savex2,savecurz,x1,x2,curz;
  long          stepZX;
  long          stepXY1, stepXY2, stepXYm;
  long          stepZY, stepZYm;
  Int2          needCheck, needCheckFr;
  Int2          vi;
  Int2          mLeft;


  reg1 = (long)p;
  vPtr = ((Nlm_Poly3DPtr)reg1)->vert;
  if ( vPtr == NULL ) return;

  /* Rotate and check */
  reg2 = ((Nlm_Poly3DPtr)reg1)->x;
  reg3 = ((Nlm_Poly3DPtr)reg1)->y;
  reg4 = ((Nlm_Poly3DPtr)reg1)->z;
  x = reg2 / Nlm_stCon.a[0][0] + reg3 / Nlm_stCon.a[0][1] +
      reg4 / Nlm_stCon.a[0][2] + Nlm_stCon.c[0];
  y = reg2 / Nlm_stCon.a[1][0] + reg3 / Nlm_stCon.a[1][1] +
      reg4 / Nlm_stCon.a[1][2] + Nlm_stCon.c[1];
  reg4 = reg2 / Nlm_stCon.a[2][0] + reg3 / Nlm_stCon.a[2][1] +
      reg4 / Nlm_stCon.a[2][2] + Nlm_stCon.c[2];
  reg4 = Nlm_stCon.zmaxPersp1-reg4;
  reg2 = Nlm_stCon.width>>1;
  reg3 = x-reg2;
  x = reg2 + (reg3*Nlm_stCon.zmaxPersp)/reg4;
  reg2 = Nlm_stCon.height>>1;
  reg3 = y-reg2;
  y =  reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/reg4;
  reg2 = ((Nlm_Poly3DPtr)reg1)->rad / Nlm_stCon.scale;
  reg2 = (reg2*Nlm_stCon.zmaxPersp)/reg4;
  needCheck = 0;
  x += reg2;
  if ( x < Nlm_stCon.xmin ) return;
  if ( x > Nlm_stCon.xmax ) needCheck=1;
  x -= reg2<<1;
  if ( x < Nlm_stCon.xmin ) needCheck=1;
  if ( x > Nlm_stCon.xmax ) return;
  y += reg2;
  if ( y < Nlm_stCon.ymin ) return;
  if ( y > Nlm_stCon.ymax ) needCheck=1;
  y -= reg2<<1;
  if ( y < Nlm_stCon.ymin ) needCheck=1;
  if ( y > Nlm_stCon.ymax ) return;

  reg2 = Nlm_stCon.a[0][0];
  reg3 = ((Nlm_Poly3DPtr)reg1)->x1 / (long)reg2;
  reg4 = ((Nlm_Poly3DPtr)reg1)->x2 / (long)reg2;
  reg2 = Nlm_stCon.a[0][1];
  reg3 += ((Nlm_Poly3DPtr)reg1)->y1 / (long)reg2;
  reg4 += ((Nlm_Poly3DPtr)reg1)->y2 / (long)reg2;
  reg2 = Nlm_stCon.a[0][2];
  reg3 += ((Nlm_Poly3DPtr)reg1)->z1 / (long)reg2;
  reg4 += ((Nlm_Poly3DPtr)reg1)->z2 / (long)reg2;
  reg2 = Nlm_stCon.c[0];
  reg3 += reg2;
  reg4 += reg2;
  vx[1] = reg3; vx[2] = reg4;

  reg2 = Nlm_stCon.a[1][0];
  reg3 = ((Nlm_Poly3DPtr)reg1)->x1 / (long)reg2;
  reg4 = ((Nlm_Poly3DPtr)reg1)->x2 / (long)reg2;
  reg2 = Nlm_stCon.a[1][1];
  reg3 += ((Nlm_Poly3DPtr)reg1)->y1 / (long)reg2;
  reg4 += ((Nlm_Poly3DPtr)reg1)->y2 / (long)reg2;
  reg2 = Nlm_stCon.a[1][2];
  reg3 += ((Nlm_Poly3DPtr)reg1)->z1 / (long)reg2;
  reg4 += ((Nlm_Poly3DPtr)reg1)->z2 / (long)reg2;
  reg2 = Nlm_stCon.c[1];
  reg3 += reg2;
  reg4 += reg2;
  vy[1] = reg3; vy[2] = reg4;

  reg2 = Nlm_stCon.a[2][0];
  reg3 = ((Nlm_Poly3DPtr)reg1)->x1 / (long)reg2;
  reg4 = ((Nlm_Poly3DPtr)reg1)->x2 / (long)reg2;
  reg2 = Nlm_stCon.a[2][1];
  reg3 += ((Nlm_Poly3DPtr)reg1)->y1 / (long)reg2;
  reg4 += ((Nlm_Poly3DPtr)reg1)->y2 / (long)reg2;
  reg2 = Nlm_stCon.a[2][2];
  reg3 += ((Nlm_Poly3DPtr)reg1)->z1 / (long)reg2;
  reg4 += ((Nlm_Poly3DPtr)reg1)->z2 / (long)reg2;
  reg2 = Nlm_stCon.c[2];
  reg3 += reg2;
  reg4 += reg2;
  vz[1] = reg3; vz[2] = reg4;

  reg2 = Nlm_stCon.width>>1;
  reg3 = vx[1]-reg2;
  vx[1] = reg2 + (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-vz[1]);
  reg3 = vx[2]-reg2;
  vx[2] = reg2 + (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-vz[2]);
  reg2 = Nlm_stCon.height>>1;
  reg3 = vy[1]-reg2;
  vy[1] = reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-vz[1]);
  reg3 = vy[2]-reg2;
  vy[2] = reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-vz[2]);
  reg3 = vz[1];
  vz[1] = (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-reg3);
  reg3 = vz[2];
  vz[2] = (reg3*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-reg3);

  vi = 2;
  while ( *vPtr != 0x7FFFFFFFL ){
    reg1 = vi;
    reg1++;
    if ( reg1 > 2 ) reg1 = 0;
    vi = (Int2)reg1;
    reg1 = (long)vPtr;
    reg2 = *(Int4Ptr)reg1; reg1 += sizeof(Int4);
    reg3 = *(Int4Ptr)reg1; reg1 += sizeof(Int4);
    reg4 = *(Int4Ptr)reg1; reg1 += sizeof(Int4) + 3 * sizeof(Int4);
    vPtr = (Int4Ptr)reg1;
    vx[vi] = reg2 / Nlm_stCon.a[0][0] + reg3 / Nlm_stCon.a[0][1] +
             reg4 / Nlm_stCon.a[0][2] + Nlm_stCon.c[0];
    vy[vi] = reg2 / Nlm_stCon.a[1][0] + reg3 / Nlm_stCon.a[1][1] +
             reg4 / Nlm_stCon.a[1][2] + Nlm_stCon.c[1];
    reg4 = reg2 / Nlm_stCon.a[2][0] + reg3 / Nlm_stCon.a[2][1] +
             reg4 / Nlm_stCon.a[2][2] + Nlm_stCon.c[2];
    vz[vi] = reg4;
    reg4 = Nlm_stCon.zmaxPersp1-reg4;
    reg2 = Nlm_stCon.width>>1;
    reg3 = vx[vi]-reg2;
    vx[vi] = reg2 + (reg3*Nlm_stCon.zmaxPersp)/reg4;
    reg2 = Nlm_stCon.height>>1;
    reg3 = vy[vi]-reg2;
    vy[vi] =  reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/reg4;
    vz[vi] = (vz[vi]*Nlm_stCon.zmaxPersp)/reg4;

    reg1 = vy[0];
    reg2 = vy[1];
    reg3 = vy[2];
    if ( reg1 < reg2 ){
      if ( reg3 < reg1 ){           /* 312 */
        xsort[0] = vx[2]; ysort[0] = reg3; zsort[0] = vz[2];
        xsort[1] = vx[0]; ysort[1] = reg1; zsort[1] = vz[0];
        xsort[2] = vx[1]; ysort[2] = reg2; zsort[2] = vz[1];
      } else if ( reg3 < reg2 ){    /* 132 */
        xsort[0] = vx[0]; ysort[0] = reg1; zsort[0] = vz[0];
        xsort[1] = vx[2]; ysort[1] = reg3; zsort[1] = vz[2];
        xsort[2] = vx[1]; ysort[2] = reg2; zsort[2] = vz[1];
      } else {                      /* 123 */
        xsort[0] = vx[0]; ysort[0] = reg1; zsort[0] = vz[0];
        xsort[1] = vx[1]; ysort[1] = reg2; zsort[1] = vz[1];
        xsort[2] = vx[2]; ysort[2] = reg3; zsort[2] = vz[2];
      }
    } else {
      if ( reg3 < reg2 ){           /* 321 */
        xsort[0] = vx[2]; ysort[0] = reg3; zsort[0] = vz[2];
        xsort[1] = vx[1]; ysort[1] = reg2; zsort[1] = vz[1];
        xsort[2] = vx[0]; ysort[2] = reg1; zsort[2] = vz[0];
      } else if ( reg3 < reg1 ){    /* 231 */
        xsort[0] = vx[1]; ysort[0] = reg2; zsort[0] = vz[1];
        xsort[1] = vx[2]; ysort[1] = reg3; zsort[1] = vz[2];
        xsort[2] = vx[0]; ysort[2] = reg1; zsort[2] = vz[0];
      } else {                      /* 213 */
        xsort[0] = vx[1]; ysort[0] = reg2; zsort[0] = vz[1];
        xsort[1] = vx[0]; ysort[1] = reg1; zsort[1] = vz[0];
        xsort[2] = vx[2]; ysort[2] = reg3; zsort[2] = vz[2];
      }
    }
    dX1 = xsort[1] - xsort[0];
    dY1 = ysort[1] - ysort[0];
    dZ1 = zsort[1] - zsort[0];
    dX2 = xsort[2] - xsort[0];
    dY2 = ysort[2] - ysort[0];
    dZ2 = zsort[2] - zsort[0];
    reg1 = dY1*dZ2 - dZ1*dY2;
    reg3 = dX1*dY2 - dY1*dX2;
    if ( reg3 < 0 ) {
      reg1=-reg1; reg3=-reg3; 
    }
    if ( reg3 != 0 ) {
      stepZX = reg1*(-1024)/reg3;
    } else {
      stepZX = 0;
    }

    needCheckFr = 0;
    if ( needCheck ){
      reg1 = ysort[0];
      reg2 = ysort[2];
      if ( (reg1 > Nlm_stCon.ymax)||
           (reg2 < Nlm_stCon.ymin) ) continue;
      if ( (reg1 < Nlm_stCon.ymin)||
           (reg2 > Nlm_stCon.ymax) ) needCheckFr = 1;
      reg1 = vx[0]; reg2 = vx[1];
      if ( reg1 > reg2 ) { reg3 = reg1; reg1 = reg2; reg2 = reg3; }
      reg3 = vx[2];
      if ( reg3 < reg1 ) reg1 = reg3;
      else if ( reg3 > reg2 ) reg2 = reg3;
      if ( (reg1 > Nlm_stCon.xmax)||
           (reg2 < Nlm_stCon.xmin) ) continue;
      if ( (reg1 < Nlm_stCon.xmin)||
           (reg2 > Nlm_stCon.xmax) ) needCheckFr = 1;
    }
    y = ysort[0];
    if ( y == ysort[1] ){
      if ( xsort[0] < xsort[1] ){
        savex1 = xsort[0];
        savex2 = xsort[1];
        savecurz = zsort[0];
      } else {
        savex1 = xsort[1];
        savex2 = xsort[0];
        savecurz = zsort[1];
      }
      if ( ysort[2] != y ){
        reg1 = ysort[2]-y;
        stepXY1 = ((xsort[2]-savex1)<<10)/reg1;
        stepXY2 = ((xsort[2]-savex2)<<10)/reg1;
        stepZY = ((zsort[2] - savecurz)<<10)/reg1;
        ysort[1] = 0x80000000L;
      } else {
        stepXY1 = stepXY2 = 0;
      }
    } else {
      savex1 = savex2 = xsort[0];
      savecurz = zsort[0];
      reg1 = xsort[0]; reg2 = ysort[0];
      if ( ((ysort[1]-reg2)*(xsort[2]-reg1) - 
            (xsort[1]-reg1)*(ysort[2]-reg2)) > 0 ){
        mLeft = 1;
        reg3 = ysort[1]-reg2;
        stepXY1 = ((xsort[1]-savex1)<<10)/reg3;
        stepZY = ((zsort[1]-zsort[0])<<10)/reg3;
        stepXY2 = ((xsort[2]-savex1)<<10)/(ysort[2]-reg2);
        reg3 = ysort[2]-ysort[1];
        if ( reg3 != 0 ){
          stepXYm = ((xsort[2]-xsort[1])<<10)/reg3;
          stepZYm = ((zsort[2]-zsort[1])<<10)/reg3;
        } else { 
          stepXYm = 0;
        }
      } else {
        mLeft = 0;
        reg3 = ysort[2]-reg2;
        stepXY1 = ((xsort[2]-savex1)<<10)/reg3;
        stepZY = ((zsort[2]-zsort[0])<<10)/reg3;
        stepXY2 = ((xsort[1]-savex1)<<10)/(ysort[1]-reg2);
        reg3 = ysort[2]-ysort[1];
        if ( reg3 != 0 ){
          stepXYm = ((xsort[2]-xsort[1])<<10)/reg3;
        } else stepXYm = 0;
      }
    }
    savex1 <<= 10;
    savex2 <<= 10;
    savecurz <<= 10;
    /* y, savex1(10), savex2(10), savecurz(10), stepZX(10), 
      savecolor10, stepColorX(10), stepColorY, stepZY,
      stepXY1, stepXY2, 
      mLeft, stepXYm, stepColorYm, stepZYm*/
    for (; y<=ysort[2];y++){
      curz = savecurz;
      x1 = (savex1>>10)+1;  x2 = savex2>>10;
      /* y, x1(10), x2(10), curz(10), stepZX(10), color10, stepColorX(10) */
      if ( x2 < x1 ) x1 = x2;
      if ( needCheckFr ){
        if ( y > Nlm_stCon.ymax ) break;
        if ( y < Nlm_stCon.ymin ) {
          if ( y == ysort[1] ){
            if ( mLeft ) {
              stepXY1 = stepXYm;
              stepZY = stepZYm;
              savex1 = xsort[1]<<10;
            } else {
              stepXY2 = stepXYm;
              savex2 = xsort[1]<<10;
            }
          }
          savecurz += stepZY;
          savex1 += stepXY1; savex2 += stepXY2;
          continue;
        }
        reg2 = Nlm_stCon.xmin;
        reg3 = Nlm_stCon.xmax;
        reg1 = x1;
        if ( reg1 > reg3 ) {
          if ( y == ysort[1] ){
            if ( mLeft ) {
              stepXY1 = stepXYm;
              stepZY = stepZYm;
              savex1 = xsort[1]<<10;
            } else {
              stepXY2 = stepXYm;
              savex2 = xsort[1]<<10;
            }
          }
          savecurz += stepZY;
          savex1 += stepXY1; savex2 += stepXY2;
          continue;
        }
        if ( reg1 < reg2 ) {
          x1 = reg2;
          reg4 = reg2-reg1;
          curz += stepZX*reg4;
        }
        reg1 = x2;
        if ( reg1 < reg2 ) {
          if ( y == ysort[1] ){
            if ( mLeft ) {
              stepXY1 = stepXYm;
              stepZY = stepZYm;
              savex1 = xsort[1]<<10;
            } else {
              stepXY2 = stepXYm;
              savex2 = xsort[1]<<10;
            }
          }
          savecurz += stepZY;
          savex1 += stepXY1; savex2 += stepXY2;
          continue;
        }
        if ( reg1 > reg3 ) {
          x2 = reg3;
        }
      }
      reg1 = Nlm_stCon.xmin;
      x1 -= reg1; x2 -= reg1;
      reg1 = (y-Nlm_stCon.ymin)*Nlm_stCon.widthCur;
      reg3 = (long)(Nlm_stCon.zBuffer + (reg1 + x1));
      reg1 = (long)(Nlm_stCon.idBuffer + reg1);
      reg2 = reg1 + x2 * sizeof(Nlm_stCon.idBuffer[0]);
      reg1 += x1 * sizeof(Nlm_stCon.idBuffer[0]);
      reg2 -= 4 * sizeof(Nlm_stCon.idBuffer[0]);
      while (reg1 <= reg2) {
        reg4 = curz;
        curz = reg4 + stepZX;
        reg4 >>= 10;
        SETPIXELID(reg3,reg1,reg4,p);
        reg3++; reg3++;
        reg1 += sizeof(Nlm_stCon.idBuffer[0]);
        reg4 = curz;
        curz = reg4 + stepZX;
        reg4 >>= 10;
        SETPIXELID(reg3,reg1,reg4,p);
        reg3++; reg3++;
        reg1 += sizeof(Nlm_stCon.idBuffer[0]);
        reg4 = curz;
        curz = reg4 + stepZX;
        reg4 >>= 10;
        SETPIXELID(reg3,reg1,reg4,p);
        reg3++; reg3++;
        reg1 += sizeof(Nlm_stCon.idBuffer[0]);
        reg4 = curz;
        curz = reg4 + stepZX;
        reg4 >>= 10;
        SETPIXELID(reg3,reg1,reg4,p);
        reg3++; reg3++;
        reg1 += sizeof(Nlm_stCon.idBuffer[0]);
      }
      reg1 -= sizeof(Nlm_stCon.idBuffer[0]);
      reg2 += 4 * sizeof(Nlm_stCon.idBuffer[0]);
      do {
        reg1 += sizeof(Nlm_stCon.idBuffer[0]);
        reg4 = curz;
        curz = reg4 + stepZX;
        reg4 >>= 10;
        SETPIXELID(reg3,reg1,reg4,p);
        reg3++; reg3++;
      } while (reg1 != reg2 );
      if ( y == ysort[1] ){
        if ( mLeft ) {
          stepXY1 = stepXYm;
          stepZY = stepZYm;
          savex1 = xsort[1]<<10;
        } else {
          stepXY2 = stepXYm;
          savex2 = xsort[1]<<10;
        }
      }
      savecurz += stepZY;
      savex1 += stepXY1; savex2 += stepXY2;
    }
  }
}

static void Nlm_Poly3DGetL ( Nlm_Poly3DPtr p )
{
  register BigScalar ax;
  register BigScalar bx;
  register long  cx;
  long           minXYZ, maxXYZ;
  double         diam;

  ax = (BigScalar)p;
  bx = (BigScalar)(((Nlm_PSeg3DPtr)ax)->base.parent);
  maxXYZ = ((Nlm_Poly3DPtr)ax)->xmax; minXYZ = ((Nlm_Poly3DPtr)ax)->xmin;
  ((Nlm_Poly3DPtr)ax)->x = (maxXYZ+minXYZ)>>1;
  cx = maxXYZ - minXYZ;
  diam = (double)cx*(double)cx;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.minX > minXYZ )
       ((Nlm_PSeg3DPtr)bx)->segBox.minX = minXYZ;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.maxX < maxXYZ )
       ((Nlm_PSeg3DPtr)bx)->segBox.maxX = maxXYZ;
  maxXYZ = ((Nlm_Poly3DPtr)ax)->ymax; minXYZ = ((Nlm_Poly3DPtr)ax)->ymin;
  ((Nlm_Poly3DPtr)ax)->y = (maxXYZ+minXYZ)>>1;
  cx = maxXYZ - minXYZ;
  diam += (double)cx*(double)cx;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.minY > minXYZ )
       ((Nlm_PSeg3DPtr)bx)->segBox.minY = minXYZ;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.maxY < maxXYZ )
       ((Nlm_PSeg3DPtr)bx)->segBox.maxY = maxXYZ;
  maxXYZ = ((Nlm_Poly3DPtr)ax)->zmax; minXYZ = ((Nlm_Poly3DPtr)ax)->zmin;
  ((Nlm_Poly3DPtr)ax)->z = (maxXYZ+minXYZ)>>1;
  cx = maxXYZ - minXYZ;
  diam += (double)cx*(double)cx;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.minZ > minXYZ )
       ((Nlm_PSeg3DPtr)bx)->segBox.minZ = minXYZ;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.maxZ < maxXYZ )
       ((Nlm_PSeg3DPtr)bx)->segBox.maxZ = maxXYZ;
  diam = sqrt ( diam );
  ((Nlm_Poly3DPtr)ax)->rad = (long)(diam+0.5);
}

static void Nlm_Poly3DFree ( Nlm_Poly3DPtr p )
{
  if ( p->vert != NULL ) MemFree ( p->vert );
  p->vert = NULL;
  p->vNum = 0;
}

static Nlm_PrimDef3D lineDef = {
  (Nlm_PrimDraw3D)Nlm_Poly3DDraw,
  (Nlm_PrimHitTest3D)Nlm_Poly3DHitT,
  (Nlm_PrimGetLimits3D)Nlm_Poly3DGetL,
  (Nlm_PrimCleanup3D)Nlm_Poly3DFree,
  (Nlm_Int2)POLY3D
};

Nlm_Prim3D Nlm_AddPoly3D ( Nlm_Picture3D pic, Nlm_Segment3D segment, 
                           BigScalar userData, Uint1 layer, Uint1 color,
                           Int4 x1, Int4 y1, Int4 z1,
                           Int4 x2, Int4 y2, Int4 z2 )
{
  Nlm_Poly3D l;

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
  l.vNum = 0;
  if ( x1 < x2 ) {  l.xmin = x1; l.xmax = x2;
  } else {  l.xmin = x2; l.xmax = x1; }
  if ( y1 < y2 ) {  l.ymin = y1; l.ymax = y2;
  } else {  l.ymin = y2; l.ymax = y1; }
  if ( z1 < z2 ) {  l.zmin = z1; l.zmax = z2;
  } else {  l.zmin = z2; l.zmax = z1; }
  return (Nlm_Prim3D) Nlm_AddPrim3D ( (Nlm_PSeg3DPtr)segment, 
                                      (Nlm_Base3DPtr)&l,
                                      sizeof(Nlm_Poly3D), 
                                      "AddPoly3D" );
}

Boolean Nlm_AddVertPoly3D ( Nlm_Picture3D pic, Nlm_Prim3D poly,
                            Int4 x, Int4 y, Int4 z )
{
  register Nlm_Poly3DPtr p;
  register Int4Ptr       vertPtr;
  register long          vNum; 
  double                 sq;
  double                 dx,dy,dz;
  long                   x1,y1,z1,x2,y2,z2;

  p = (Nlm_Poly3DPtr)poly;
  if ( p == NULL ) return FALSE;
  vNum = p->vNum + 1;
  if ( vNum == 1 ){
    vertPtr = (Int4Ptr) MemNew( sizeof(Int4)*300 );
    p->vert = vertPtr;
    x1 = p->x1; y1 = p->y1; z1 = p->z1;
    x2 = p->x2; y2 = p->y2; z2 = p->z2;
  } else if ( vNum == 2 ) {
    x1 = p->x2; y1 = p->y2; z1 = p->z2;
    vertPtr = p->vert;
    x2 = *vertPtr++; y2 = *vertPtr++; z2 = *vertPtr++;
    vertPtr += 3;
  } else if ( ((vNum/50)*50) == vNum ){
    vertPtr = (Int4Ptr) MemNew( (vNum+50)*6*sizeof(Int4) );
    MemCopy ( vertPtr, p->vert, (vNum-1)*6*sizeof(Int4) );
    MemFree ( p->vert );
    p->vert = vertPtr;
    vertPtr += (vNum-3)*6;
    x1 = *vertPtr++; y1 = *vertPtr++; z1 = *vertPtr++;
    vertPtr += 3;
    x2 = *vertPtr++; y2 = *vertPtr++; z2 = *vertPtr++;
    vertPtr += 3;
  } else {
    vertPtr = p->vert;
    vertPtr += (vNum-3)*6;
    x1 = *vertPtr++; y1 = *vertPtr++; z1 = *vertPtr++;
    vertPtr += 3;
    x2 = *vertPtr++; y2 = *vertPtr++; z2 = *vertPtr++;
    vertPtr += 3;
  }
  if ( x < (Int4)p->xmin ) p->xmin = (long)x;
  else if ( x > (Int4)p->xmax ) p->xmax = (long)x;
  if ( y < (Int4)p->ymin ) p->ymin = (long)y;
  else if ( y > (Int4)p->ymax ) p->ymax = (long)y;
  if ( z < (Int4)p->zmin ) p->zmin = (long)z;
  else if ( z > (Int4)p->zmax ) p->zmax = (long)z;
  *vertPtr++ = (long)x; *vertPtr++ = (long)y; *vertPtr++ = (long)z;
  x -= (Int4)x1; y -= (Int4)y1; z -= (Int4)z1;
  x1 = x2-x1; y1 = y2-y1; z1 = z2-z1;
  x2 = (long)x; y2 = (long)y; z2 = (long)z;
  dx = (double)y1*(double)z2-(double)z1*(double)y2; 
  dy = (double)z1*(double)x2-(double)x1*(double)z2; 
  dz = (double)x1*(double)y2-(double)y1*(double)x2;
  sq = sqrt ( dx*dx + dy*dy + dz*dz );
  dx *= 1e9/sq; dy *= 1e9/sq; dz *= 1e9/sq;
  *vertPtr++ = (long)dx;
  *vertPtr++ = (long)dy;
  *vertPtr++ = (long)dz;
  *vertPtr = 0x7FFFFFFFL;
  p->vNum = vNum;
  return TRUE;
}

