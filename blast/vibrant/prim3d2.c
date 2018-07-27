/*   prim3d2.c
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
* File Name:  prim3d2.c
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
* $Log: prim3d2.c,v $
* Revision 6.1  1998/08/12 23:04:03  vakatov
* [64-bit platforms]  Made "idBuffer" be VoidPtr[] rather than Int4[];
* Fixed bugs mostly caused by casting between Int4 vars and 8-byte pointers
*
* Revision 6.0  1997/08/25 18:56:33  madden
* Revision changed to 6.0
*
* Revision 5.1  1996/07/25 14:50:13  epstein
* BigScalar change for OSF1 portability
*
 * Revision 5.0  1996/05/28  13:45:08  ostell
 * Set to revision 5.0
 *
 * Revision 1.4  1996/04/04  19:29:32  vakatov
 * The sphere XY-centering is attuned to match the cylinder centering.
 * Shere of small(1-3) radius still has a 1-point overhead over the
 * cylinder of the same radius.
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
*   SPHERE
*
*****************************************************************************/
typedef struct Nlm_sphere3d {
  Nlm_Base3D  base;
  long        x,y,z;
  long        radius;
} Nlm_Sphere3D, PNTR Nlm_Sphere3DPtr;

static void Nlm_Sphere3DDraw ( Nlm_Sphere3DPtr p )
{
  register long reg1;
  register long reg2;
  register long reg3;
  register long reg4;
  Uint1Ptr      cenPtr;
  Uint1Ptr      cenZPtr;
  Uint1Ptr      cenCurPtr;
  Uint1Ptr      cenCurZPtr;
  Uint1Ptr      curPtr;
  Uint1Ptr      radiusF;
  Uint1Ptr      zF;
  long          x,y,z;
  long          dy,dxEnd,dyEnd;
  long          xmin, ymin, xmax, ymax;
  long          baseColor, colorScale;

  reg1 = (long)p;

  /* Rotate and check */
  reg2 = ((Nlm_Sphere3DPtr)reg1)->x;
  reg3 = ((Nlm_Sphere3DPtr)reg1)->y;
  reg4 = ((Nlm_Sphere3DPtr)reg1)->z;
  x = reg2 / Nlm_stCon.a[0][0] + reg3 / Nlm_stCon.a[0][1] +
      reg4 / Nlm_stCon.a[0][2] + Nlm_stCon.c[0];
  y = reg2 / Nlm_stCon.a[1][0] + reg3 / Nlm_stCon.a[1][1] +
      reg4 / Nlm_stCon.a[1][2] + Nlm_stCon.c[1];
  reg4 = reg2 / Nlm_stCon.a[2][0] + reg3 / Nlm_stCon.a[2][1] +
      reg4 / Nlm_stCon.a[2][2] + Nlm_stCon.c[2];
  z = reg4;
  reg4 = Nlm_stCon.zmaxPersp1-reg4;
  reg2 = Nlm_stCon.width>>1;
  reg3 = x-reg2;
  x = reg2 + (reg3*Nlm_stCon.zmaxPersp)/reg4 + 1;
  reg2 = Nlm_stCon.height>>1;
  reg3 = y-reg2;
  y =  reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/reg4 + 1;
  reg2 = ((Nlm_Sphere3DPtr)reg1)->radius / Nlm_stCon.scale;
  reg2 = (reg2*Nlm_stCon.zmaxPersp)/reg4 - 1;
  z = (z*Nlm_stCon.zmaxPersp)/reg4;

  if ( reg2 > 127 ) return;
  if ( reg2 < 1 ) reg2 = 1;
  reg3 = x;
  xmin = Nlm_stCon.xmin - reg3;
  xmax = Nlm_stCon.xmax - reg3;
  if ( (reg2 < xmin) || (-reg2 > xmax) ) return;
  reg4 = y;
  ymin = Nlm_stCon.ymin - reg4;
  ymax = Nlm_stCon.ymax - reg4;
  if ( (reg2 < ymin) || (-reg2 > ymax) ) return;
  /* Calculate center pointers */
  reg4 = reg4 * Nlm_stCon.width + reg3;
  cenPtr = Nlm_stCon.image + reg4;
  reg4 <<= 1;
  cenZPtr =(Uint1Ptr)Nlm_stCon.zBuffer + reg4;

  /* calculate yStart and yEnd */
  reg3 = -reg2;
  if ( reg3 < ymin ) reg3 = ymin;
  dy = reg3;
  reg4 = reg3*Nlm_stCon.width;
  cenCurPtr = cenPtr + reg4;
  reg4 <<= 1;
  cenCurZPtr = cenZPtr + reg4;
  reg4 = reg2;
  if ( reg4 > ymax ) reg4 = ymax;
  dyEnd = reg4;

  radiusF = &(Nlm_Isqrt[reg2*(reg2+1)]);
  baseColor = Nlm_stCon.colorOffset + 1 +
              ((Nlm_Sphere3DPtr)reg1)->base.color * Nlm_stCon.colorStep +
              (Nlm_stCon.colorStep*141*(z+Nlm_stCon.zmax))/
              (628*Nlm_stCon.zmax);
  colorScale = ((reg2*314)<<8)/
               (Nlm_stCon.colorStep*(50+(50*z)/Nlm_stCon.zmax));

  while ( dy <= dyEnd ){
    reg1 = radiusF[dy];
    zF = &(Nlm_Isqrt[reg1*(reg1+1)]);
    dxEnd = reg1;
    if ( reg1 > xmax ) dxEnd = xmax;
    reg1 = -reg1;
    if ( reg1 < xmin ) reg1 = xmin;
    curPtr = cenCurPtr + reg1;
    reg4 = (long)(cenCurZPtr + (reg1 << 1));
    while ( dxEnd - reg1 >= 4 ){
      reg2 = zF[reg1];
      reg3 = z+reg2;
      if((Uint2)reg3>=*((Uint2Ptr)reg4)) {
        *((Uint2Ptr)reg4)=(Uint2)reg3;
        *((Uint1Ptr)curPtr)=(Uint1)(baseColor + 
        ((reg2-reg1 Y_PLUS dy)<<8)/colorScale);
      }
      reg4++; reg4++;
      curPtr++;
      reg1++;
      reg2 = zF[reg1];
      reg3 = z+reg2;
      if((Uint2)reg3>=*((Uint2Ptr)reg4)) {
        *((Uint2Ptr)reg4)=(Uint2)reg3;
        *((Uint1Ptr)curPtr)=(Uint1)(baseColor + 
        ((reg2-reg1 Y_PLUS dy)<<8)/colorScale);
      }
      reg4++; reg4++;
      curPtr++;
      reg1++;
      reg2 = zF[reg1];
      reg3 = z+reg2;
      if((Uint2)reg3>=*((Uint2Ptr)reg4)) {
        *((Uint2Ptr)reg4)=(Uint2)reg3;
        *((Uint1Ptr)curPtr)=(Uint1)(baseColor + 
        ((reg2-reg1 Y_PLUS dy)<<8)/colorScale);
      }
      reg4++; reg4++;
      curPtr++;
      reg1++;
      reg2 = zF[reg1];
      reg3 = z+reg2;
      if((Uint2)reg3>=*((Uint2Ptr)reg4)) {
        *((Uint2Ptr)reg4)=(Uint2)reg3;
        *((Uint1Ptr)curPtr)=(Uint1)(baseColor + 
        ((reg2-reg1 Y_PLUS dy)<<8)/colorScale);
      }
      reg4++; reg4++;
      curPtr++;
      reg1++;
    }
    while ( reg1 <= dxEnd ){
      reg2 = zF[reg1];
      reg3 = z+reg2;
      if((Uint2)reg3>=*((Uint2Ptr)reg4)) {
        *((Uint2Ptr)reg4)=(Uint2)reg3;
        *((Uint1Ptr)curPtr)=(Uint1)(baseColor + 
        ((reg2-reg1 Y_PLUS dy)<<8)/colorScale);
      }
      reg4++; reg4++;
      curPtr++;
      reg1++;
    }
    cenCurPtr += Nlm_stCon.width;
    cenCurZPtr += (Nlm_stCon.width << 1);
    dy++;
  }
}

static void Nlm_Sphere3DHitT ( Nlm_Sphere3DPtr p )
{
  register long reg1;
  register long reg2;
  register long reg3;
  register long reg4;
  Uint1Ptr      cenPtr;
  Uint1Ptr      cenZPtr;
  Uint1Ptr      cenCurPtr;
  Uint1Ptr      cenCurZPtr;
  Uint1Ptr      curPtr;
  Uint1Ptr      radiusF;
  Uint1Ptr      zF;
  long          x,y,z;
  long          dy,dxEnd,dyEnd;
  long          xmin, ymin, xmax, ymax;

  reg1 = (long)p;

  /* Rotate and check */
  reg2 = ((Nlm_Sphere3DPtr)reg1)->x;
  reg3 = ((Nlm_Sphere3DPtr)reg1)->y;
  reg4 = ((Nlm_Sphere3DPtr)reg1)->z;
  x = reg2 / Nlm_stCon.a[0][0] + reg3 / Nlm_stCon.a[0][1] +
      reg4 / Nlm_stCon.a[0][2] + Nlm_stCon.c[0];
  y = reg2 / Nlm_stCon.a[1][0] + reg3 / Nlm_stCon.a[1][1] +
      reg4 / Nlm_stCon.a[1][2] + Nlm_stCon.c[1];
  reg4 = reg2 / Nlm_stCon.a[2][0] + reg3 / Nlm_stCon.a[2][1] +
      reg4 / Nlm_stCon.a[2][2] + Nlm_stCon.c[2];
  z = reg4;
  reg4 = Nlm_stCon.zmaxPersp1-reg4;
  reg2 = Nlm_stCon.width>>1;
  reg3 = x-reg2;
  x = reg2 + (reg3*Nlm_stCon.zmaxPersp)/reg4 + 1;
  reg2 = Nlm_stCon.height>>1;
  reg3 = y-reg2;
  y =  reg2 Y_PLUS (reg3*Nlm_stCon.zmaxPersp)/reg4 + 1;
  reg2 = ((Nlm_Sphere3DPtr)reg1)->radius / Nlm_stCon.scale;
  reg2 = (reg2*Nlm_stCon.zmaxPersp)/reg4 - 1;
  z = (z*Nlm_stCon.zmaxPersp)/reg4;

  if ( reg2 > 127 ) return;
  if ( reg2 < 1 ) reg2 = 1;
  reg3 = x;
  xmin = Nlm_stCon.xmin - reg3;
  xmax = Nlm_stCon.xmax - reg3;
  if ( (reg2 < xmin) || (-reg2 > xmax) ) return;
  reg4 = y;
  ymin = Nlm_stCon.ymin - reg4;
  ymax = Nlm_stCon.ymax - reg4;
  if ( (reg2 < ymin) || (-reg2 > ymax) ) return;

  /* Calculate center pointers */
  reg4 = (reg4-Nlm_stCon.ymin) * Nlm_stCon.widthCur + (reg3-Nlm_stCon.xmin);
  cenZPtr =(Uint1Ptr)(Nlm_stCon.zBuffer  + reg4);
  cenPtr = (Uint1Ptr)(Nlm_stCon.idBuffer + reg4);

  /* calculate yStart and yEnd */
  reg3 = -reg2;
  if ( reg3 < ymin ) reg3 = ymin;
  dy = reg3;

  reg4 = reg3*Nlm_stCon.widthCur;
  cenCurZPtr = cenZPtr + reg4 * sizeof(Nlm_stCon.zBuffer [0]);
  cenCurPtr  = cenPtr  + reg4 * sizeof(Nlm_stCon.idBuffer[0]);

  reg4 = reg2;
  if ( reg4 > ymax ) reg4 = ymax;
  dyEnd = reg4;

  radiusF = &(Nlm_Isqrt[reg2*(reg2+1)]);
  while ( dy <= dyEnd ){
    reg1 = radiusF[dy];
    zF = &(Nlm_Isqrt[reg1*(reg1+1)]);
    dxEnd = reg1;
    if ( reg1 > xmax ) dxEnd = xmax;
    reg1 = -reg1;
    if ( reg1 < xmin ) reg1 = xmin;
    curPtr = cenCurPtr + reg1 * sizeof(Nlm_stCon.idBuffer[0]);
    reg4 = (long)(cenCurZPtr + (reg1 << 1));
    while ( dxEnd - reg1 >= 4 ){
      reg2 = zF[reg1];
      reg3 = z+reg2;
      if((Uint2)reg3>=*((Uint2Ptr)reg4)) {
        *((Uint2Ptr)reg4)=(Uint2)reg3;
        *((VoidPtr*)curPtr) = p;
      }
      reg4++; reg4++;
      curPtr += sizeof(Nlm_stCon.idBuffer[0]);
      reg1++;
      reg2 = zF[reg1];
      reg3 = z+reg2;
      if((Uint2)reg3>=*((Uint2Ptr)reg4)) {
        *((Uint2Ptr)reg4)=(Uint2)reg3;
        *((VoidPtr*)curPtr) = p;
      }
      reg4++; reg4++;
      curPtr += sizeof(Nlm_stCon.idBuffer[0]);
      reg1++;
      reg2 = zF[reg1];
      reg3 = z+reg2;
      if((Uint2)reg3>=*((Uint2Ptr)reg4)) {
        *((Uint2Ptr)reg4)=(Uint2)reg3;
        *((VoidPtr*)curPtr) = p;
      }
      reg4++; reg4++;
      curPtr += sizeof(Nlm_stCon.idBuffer[0]);
      reg1++;
      reg2 = zF[reg1];
      reg3 = z+reg2;
      if((Uint2)reg3>=*((Uint2Ptr)reg4)) {
        *((Uint2Ptr)reg4)=(Uint2)reg3;
        *((VoidPtr*)curPtr) = p;
      }
      reg4++; reg4++;
      curPtr += sizeof(Nlm_stCon.idBuffer[0]);
      reg1++;
    }
    while ( reg1 <= dxEnd ){
      reg2 = zF[reg1];
      reg3 = z+reg2;
      if((Uint2)reg3>=*((Uint2Ptr)reg4)) {
        *((Uint2Ptr)reg4)=(Uint2)reg3;
        *((VoidPtr*)curPtr) = p;
      }
      reg4++; reg4++;
      curPtr += sizeof(Nlm_stCon.idBuffer[0]);
      reg1++;
    }
    cenCurPtr  += (Nlm_stCon.widthCur * sizeof(Nlm_stCon.idBuffer[0]));
    cenCurZPtr += (Nlm_stCon.widthCur << 1);
    dy++;
  }
}

static void Nlm_Sphere3DGetL ( Nlm_Sphere3DPtr p )
{
  register BigScalar ax;
  register BigScalar bx;
  long minXY, maxXY;

  ax = (BigScalar)p;
  bx = (BigScalar)(((Nlm_PSeg3DPtr)ax)->base.parent);
  maxXY = ((Nlm_Sphere3DPtr)ax)->x + ((Nlm_Sphere3DPtr)ax)->radius;
  minXY = ((Nlm_Sphere3DPtr)ax)->x - ((Nlm_Sphere3DPtr)ax)->radius;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.minX > minXY )
       ((Nlm_PSeg3DPtr)bx)->segBox.minX = minXY;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.maxX < maxXY )
       ((Nlm_PSeg3DPtr)bx)->segBox.maxX = maxXY;
  maxXY = ((Nlm_Sphere3DPtr)ax)->y + ((Nlm_Sphere3DPtr)ax)->radius;
  minXY = ((Nlm_Sphere3DPtr)ax)->y - ((Nlm_Sphere3DPtr)ax)->radius;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.minY > minXY )
       ((Nlm_PSeg3DPtr)bx)->segBox.minY = minXY;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.maxY < maxXY )
       ((Nlm_PSeg3DPtr)bx)->segBox.maxY = maxXY;
  maxXY = ((Nlm_Sphere3DPtr)ax)->z + ((Nlm_Sphere3DPtr)ax)->radius;
  minXY = ((Nlm_Sphere3DPtr)ax)->z - ((Nlm_Sphere3DPtr)ax)->radius;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.minZ > minXY )
       ((Nlm_PSeg3DPtr)bx)->segBox.minZ = minXY;
  if ( ((Nlm_PSeg3DPtr)bx)->segBox.maxZ < maxXY )
       ((Nlm_PSeg3DPtr)bx)->segBox.maxZ = maxXY;
}

static Nlm_PrimDef3D sphereDef = {
  (Nlm_PrimDraw3D)Nlm_Sphere3DDraw,
  (Nlm_PrimHitTest3D)Nlm_Sphere3DHitT,
  (Nlm_PrimGetLimits3D)Nlm_Sphere3DGetL,
  (Nlm_PrimCleanup3D)Nlm_Empty3D,
  (Nlm_Int2)SPHERE3D
};

Nlm_Prim3D Nlm_AddSphere3D ( Nlm_Picture3D pic, Nlm_Segment3D segment,
                             BigScalar userData, Uint1 layer, Uint1 color,
                             Int4 x, Int4 y, Int4 z, Uint4 radius )
{
  Nlm_Sphere3D l;

  Nlm_stCon.pic = pic;
  Nlm_DiagReset ();
  l.base.userData = userData;
  l.base.layer = layer;
  l.base.color = color;
  l.base.fTable = &sphereDef;
  l.base.status = 0;
  l.x = (long)x; l.y = (long)y; l.z = (long)z;
  l.radius = radius;
  return (Nlm_Prim3D) Nlm_AddPrim3D ( (Nlm_PSeg3DPtr)segment, 
                                      (Nlm_Base3DPtr)&l,
                                      sizeof(Nlm_Sphere3D), 
                                      "AddSphere3D" );
}


