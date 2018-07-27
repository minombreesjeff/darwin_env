/*   pictur3d.c
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
* File Name:  pictur3d.c
*
* Author:  Alex Smirnov
*
* Version Creation Date:   04/03/95
*
* $Revision: 6.0 $
*
* File Description: 
*  3D picture file.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#ifndef _PPICT3D_
#include <ppict3d.h>
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
Nlm_Context3D Nlm_stCon;

/*****************************************************************************
*
*   STATIC VARIABLE
*
*****************************************************************************/
static CharPtr Pict3Dclass = "Picture3D";

/*****************************************************************************
*
*   FUNCTIONS
*
*****************************************************************************/
static void Nlm_AddToSeg ( Nlm_PSeg3DPtr s, Nlm_Base3DPtr p )
{
  register long ax;
  register long bx;

  ax = (long)s;
  bx = (long)(((Nlm_PSeg3DPtr)ax)->last);
  if ( bx ) {
    ((Nlm_PSeg3DPtr)ax)->last = p;
    ax = (long)p;
    ((Nlm_Base3DPtr)bx)->next = (Nlm_Base3DPtr)ax;
	 ((Nlm_Base3DPtr)ax)->next = NULL;
    ((Nlm_Base3DPtr)ax)->parent = (Nlm_Base3DPtr)s;
  } else {
    bx = (long)p;
	 ((Nlm_PSeg3DPtr)ax)->last = ((Nlm_PSeg3DPtr)ax)->first = 
      (Nlm_Base3DPtr)bx;
	 ((Nlm_Base3DPtr)bx)->next = NULL;
	 ((Nlm_Base3DPtr)bx)->parent = (Nlm_Base3DPtr)ax;
  }
}

static void Nlm_DeleteFromSeg ( Nlm_PSeg3DPtr seg, Nlm_Base3DPtr base )
{
  register Nlm_Base3DPtr b;
  register Nlm_Base3DPtr bprev;
  
  bprev = NULL;
  for ( b = seg->first; (b != base) && (b != NULL); b = b->next ){
    bprev = b;
  }
  if ( b != NULL ) {
    if ( bprev != NULL ) bprev->next = b->next;
    else seg->first = b->next;
	 if ( seg->last == b ) seg->last = bprev;
  }
}

static void Nlm_Seg3DDraw ( Nlm_PSeg3DPtr s )
{
  register long ax;
  register long  bx;
  long           spx, spy, spz, rad;
  Uint1          color;

  ax = (long)s;
  rad = ((Nlm_PSeg3DPtr)ax)->segSph.radius / Nlm_stCon.scale;
  spz = ((Nlm_PSeg3DPtr)ax)->segSph.x / Nlm_stCon.a[2][0] + 
			((Nlm_PSeg3DPtr)ax)->segSph.y / Nlm_stCon.a[2][1] + 
			((Nlm_PSeg3DPtr)ax)->segSph.z / Nlm_stCon.a[2][2] + 
			Nlm_stCon.c[2];
  spx = ((Nlm_PSeg3DPtr)ax)->segSph.x / Nlm_stCon.a[0][0] + 
			((Nlm_PSeg3DPtr)ax)->segSph.y / Nlm_stCon.a[0][1] + 
			((Nlm_PSeg3DPtr)ax)->segSph.z / Nlm_stCon.a[0][2] + 
			Nlm_stCon.c[0];
  bx = Nlm_stCon.width>>1;
  spx -= bx;
  bx = bx + (spx*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-spz);
  if ( (Nlm_stCon.xmin > (bx + rad)) || (Nlm_stCon.xmax < (bx - rad)) )
	 return;
  spy = ((Nlm_PSeg3DPtr)ax)->segSph.x / Nlm_stCon.a[1][0] + 
			((Nlm_PSeg3DPtr)ax)->segSph.y / Nlm_stCon.a[1][1] + 
			((Nlm_PSeg3DPtr)ax)->segSph.z / Nlm_stCon.a[1][2] + 
			Nlm_stCon.c[1];
  bx = Nlm_stCon.height>>1;
  spy -= bx;
  bx = bx Y_PLUS (spy*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-spz);
  if ( (Nlm_stCon.ymin > (bx + rad)) || (Nlm_stCon.ymax < (bx - rad)) )
	 return;
  ax = (long)(((Nlm_PSeg3DPtr)ax)->first);
  while ( ax ){
	 if ( Nlm_stCon.layerTable[((Nlm_Base3DPtr)ax)->layer] ){
      if ( ((Nlm_Base3DPtr)ax)->status & HL_MASK ){
        color = ((Nlm_Base3DPtr)ax)->color;
        ((Nlm_Base3DPtr)ax)->color = Nlm_stCon.colorHL;
		  ((Nlm_Base3DPtr)ax)->fTable->draw ( (Nlm_VoidPtr)ax );
        ((Nlm_Base3DPtr)ax)->color = color;
      } else {
		  ((Nlm_Base3DPtr)ax)->fTable->draw ( (Nlm_VoidPtr)ax );
      }
	 }
	 ax = (long)(((Nlm_Base3DPtr)ax)->next);
  }
}

static void Nlm_Seg3DHitT ( Nlm_PSeg3DPtr s )
{
  register long ax;
  register long  bx;
  long           spx, spy, spz, rad;

  ax = (long)s;
  rad = ((Nlm_PSeg3DPtr)ax)->segSph.radius / Nlm_stCon.scale;
  spz = ((Nlm_PSeg3DPtr)ax)->segSph.x / Nlm_stCon.a[2][0] + 
			((Nlm_PSeg3DPtr)ax)->segSph.y / Nlm_stCon.a[2][1] + 
			((Nlm_PSeg3DPtr)ax)->segSph.z / Nlm_stCon.a[2][2] + 
			Nlm_stCon.c[2];
  spx = ((Nlm_PSeg3DPtr)ax)->segSph.x / Nlm_stCon.a[0][0] + 
			((Nlm_PSeg3DPtr)ax)->segSph.y / Nlm_stCon.a[0][1] + 
			((Nlm_PSeg3DPtr)ax)->segSph.z / Nlm_stCon.a[0][2] + 
			Nlm_stCon.c[0];
  bx = Nlm_stCon.width>>1;
  spx -= bx;
  bx = bx + (spx*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-spz);
  if ( (Nlm_stCon.xmin > (bx + rad)) || (Nlm_stCon.xmax < (bx - rad)) )
	 return;
  spy = ((Nlm_PSeg3DPtr)ax)->segSph.x / Nlm_stCon.a[1][0] + 
			((Nlm_PSeg3DPtr)ax)->segSph.y / Nlm_stCon.a[1][1] + 
			((Nlm_PSeg3DPtr)ax)->segSph.z / Nlm_stCon.a[1][2] + 
			Nlm_stCon.c[1];
  bx = Nlm_stCon.height>>1;
  spy -= bx;
  bx = bx Y_PLUS (spy*Nlm_stCon.zmaxPersp)/(Nlm_stCon.zmaxPersp1-spz);
  if ( (Nlm_stCon.ymin > (bx + rad)) || (Nlm_stCon.ymax < (bx - rad)) )
	 return;
  ax = (long)(((Nlm_PSeg3DPtr)ax)->first);
  while ( ax ){
	 if ( Nlm_stCon.layerTable[((Nlm_Base3DPtr)ax)->layer] ){
		((Nlm_Base3DPtr)ax)->fTable->hittest ( (Nlm_VoidPtr)ax );
	 }
	 ax = (long)(((Nlm_Base3DPtr)ax)->next);
  }
}

static void Nlm_Seg3DGetL ( Nlm_PSeg3DPtr s )
{
  register long ax;
  register long bx;
  double        diam;

  ax = (long)s;
  ((Nlm_PSeg3DPtr)ax)->segBox.minX = ((Nlm_PSeg3DPtr)ax)->segBox.minY =
  ((Nlm_PSeg3DPtr)ax)->segBox.minZ = INT4_MAX;
  ((Nlm_PSeg3DPtr)ax)->segBox.maxX = ((Nlm_PSeg3DPtr)ax)->segBox.maxY =
  ((Nlm_PSeg3DPtr)ax)->segBox.maxZ = INT4_MIN;
  ax = (long)(((Nlm_PSeg3DPtr)ax)->first);
  while ( ax ){
	 if ( Nlm_stCon.layerTable[((Nlm_Base3DPtr)ax)->layer] ){
		((Nlm_Base3DPtr)ax)->fTable->getlimits ( (Nlm_VoidPtr)ax );
	 }
	 ax = (long)(((Nlm_Base3DPtr)ax)->next);
  }
  ax = (long)s;
  bx = (long)(((Nlm_PSeg3DPtr)ax)->segBox.maxX - 
					((Nlm_PSeg3DPtr)ax)->segBox.minX);
  diam = (double)bx * (double)bx;
  bx = (long)(((Nlm_PSeg3DPtr)ax)->segBox.maxY - 
					((Nlm_PSeg3DPtr)ax)->segBox.minY);
  diam += (double)bx * (double)bx;
  bx = (long)(((Nlm_PSeg3DPtr)ax)->segBox.maxZ - 
					((Nlm_PSeg3DPtr)ax)->segBox.minZ);
  diam += (double)bx * (double)bx;
  diam = sqrt ( diam );
  ((Nlm_PSeg3DPtr)ax)->segSph.radius = (long)(diam/2.0) + 1;
  ((Nlm_PSeg3DPtr)ax)->segSph.x = (((Nlm_PSeg3DPtr)ax)->segBox.maxX + 
                            ((Nlm_PSeg3DPtr)ax)->segBox.minX)>>1 ;
  ((Nlm_PSeg3DPtr)ax)->segSph.y = (((Nlm_PSeg3DPtr)ax)->segBox.maxY + 
                            ((Nlm_PSeg3DPtr)ax)->segBox.minY)>>1 ;
  ((Nlm_PSeg3DPtr)ax)->segSph.z = (((Nlm_PSeg3DPtr)ax)->segBox.maxZ + 
                            ((Nlm_PSeg3DPtr)ax)->segBox.minZ)>>1 ;
  ax = (long)s;
  bx = (long)(((Nlm_PSeg3DPtr)ax)->base.parent);
  if ( bx ){
	 if (((Nlm_PSeg3DPtr)bx)->segBox.minX > ((Nlm_PSeg3DPtr)ax)->segBox.minX)
		((Nlm_PSeg3DPtr)bx)->segBox.minX = ((Nlm_PSeg3DPtr)ax)->segBox.minX;
	 if (((Nlm_PSeg3DPtr)bx)->segBox.minY > ((Nlm_PSeg3DPtr)ax)->segBox.minY)
		((Nlm_PSeg3DPtr)bx)->segBox.minY = ((Nlm_PSeg3DPtr)ax)->segBox.minY;
	 if (((Nlm_PSeg3DPtr)bx)->segBox.minZ > ((Nlm_PSeg3DPtr)ax)->segBox.minZ)
		((Nlm_PSeg3DPtr)bx)->segBox.minZ = ((Nlm_PSeg3DPtr)ax)->segBox.minZ;
	 if (((Nlm_PSeg3DPtr)bx)->segBox.maxX < ((Nlm_PSeg3DPtr)ax)->segBox.maxX)
		((Nlm_PSeg3DPtr)bx)->segBox.maxX = ((Nlm_PSeg3DPtr)ax)->segBox.maxX;
	 if (((Nlm_PSeg3DPtr)bx)->segBox.maxY < ((Nlm_PSeg3DPtr)ax)->segBox.maxY)
		((Nlm_PSeg3DPtr)bx)->segBox.maxY = ((Nlm_PSeg3DPtr)ax)->segBox.maxY;
	 if (((Nlm_PSeg3DPtr)bx)->segBox.maxZ < ((Nlm_PSeg3DPtr)ax)->segBox.maxZ)
		((Nlm_PSeg3DPtr)bx)->segBox.maxZ = ((Nlm_PSeg3DPtr)ax)->segBox.maxZ;
  }
}

static void Nlm_Seg3DClean ( Nlm_PSeg3DPtr s )
{
  register long ax;
  register long bx;

  ax = (long)(s->first);
  while ( ax ){
	 ((Nlm_Base3DPtr)ax)->fTable->cleanup ( (Nlm_VoidPtr)ax );
	 bx = (long)(((Nlm_Base3DPtr)ax)->next);
	 MemFree ( (Nlm_VoidPtr)ax );
	 ax = bx;
  }
}

static Nlm_PrimDef3D segmentDef = {
  (Nlm_PrimDraw3D)Nlm_Seg3DDraw,
  (Nlm_PrimHitTest3D)Nlm_Seg3DHitT,
  (Nlm_PrimGetLimits3D)Nlm_Seg3DGetL,
  (Nlm_PrimCleanup3D)Nlm_Seg3DClean,
  (Nlm_Int2)0
};

Nlm_Picture3D Nlm_CreatePicture3D ( void )
{
  register Nlm_PPict3DPtr p;
  Int2 i;

  Nlm_DiagReset ();
  p = (Nlm_PPict3DPtr)MemNew ( sizeof(Nlm_PPict3D) );
  if ( p == NULL ) {
	 Nlm_DiagPutRecord ( DA_ERROR, Pict3Dclass, "CreatePicture3D",
								"Memory allocation error" );
	 return NULL;
  }
  for (i=0;i<256;i++) p->layerTable[i] = TRUE;
  p->seg.base.fTable = &segmentDef;
  return (Nlm_Picture3D)p;
}

void Nlm_DeletePicture3D ( Nlm_Picture3D pic )
{
  Nlm_ResetPicture3D ( pic );
  MemFree ( pic );
}

void Nlm_ResetPicture3D ( Nlm_Picture3D pic )
{
  register Nlm_PPict3DPtr p;
  Int2 i;

  Nlm_DiagReset ();
  Nlm_stCon.pic = pic;
  p = (Nlm_PPict3DPtr)pic;
  p->seg.base.fTable->cleanup ( (Nlm_VoidPtr)&(p->seg) );
  for (i=0;i<256;i++) {
    p->layerTable[i] = TRUE;
    p->colorR[i] = p->colorG[i] = p->colorB[i] = 0;
  }
  p->totalColors = 0;
  p->version++;
}

Boolean Nlm_AllocPalette3D ( Nlm_Picture3D pic, Uint1 totalColors )
{
  register Nlm_PPict3DPtr p;

  Nlm_DiagReset ();
  p = (Nlm_PPict3DPtr)pic;
  if ( p->totalColors != 0 ) {
	 Nlm_DiagPutRecord ( DA_ERROR, Pict3Dclass, "AllocPalette3D",
	                     "Palette is allocated already" );
	 return FALSE;
  }
  p->totalColors = totalColors;
  p->version++;
  return TRUE;
}

Boolean Nlm_SetColor3D ( Nlm_Picture3D pic, Uint1 color, 
                         Uint1 red, Uint1 green, Uint1 blue )
{
  register Nlm_PPict3DPtr p;

  Nlm_DiagReset ();
  p = (Nlm_PPict3DPtr)pic;
  if ( p->totalColors <= color ) {
    Nlm_DiagPutRecord ( DA_ERROR, Pict3Dclass, "SetColor3D",
                        "Color index is out of range" );
    return FALSE;
  }
  p->colorR[color] = red;
  p->colorG[color] = green;
  p->colorB[color] = blue;
  p->version++;
  return TRUE;
}

Boolean Nlm_GetColor3D ( Nlm_Picture3D pic, Uint1 color,
                         Uint1Ptr red, Uint1Ptr green, Uint1Ptr blue )
{
  register Nlm_PPict3DPtr p;

  Nlm_DiagReset ();
  p = (Nlm_PPict3DPtr)pic;
  if ( p->totalColors <= color ) {
    Nlm_DiagPutRecord ( DA_ERROR, Pict3Dclass, "SetColor3D",
                        "Color index is out of range" );
    return FALSE;
  }
  *red = p->colorR[color];
  *green = p->colorG[color];
  *blue = p->colorB[color];
  return TRUE;
}

void Nlm_SetLayer3D ( Nlm_Picture3D pic, Uint1 layer, Boolean layerOn )
{
  register Nlm_PPict3DPtr p;

  Nlm_DiagReset ();
  p = (Nlm_PPict3DPtr)pic;
/*  if ( layer == (Uint1)0 ){
    if ( !layerOn ) Nlm_DiagPutRecord ( DA_ERROR, Pict3Dclass,
                    "SetLayer3D",
                    "Can not switch OFF layer 0" );
  } else {
*/
    p->layerTable[layer] = layerOn;
    p->version++;
/*  } */
}

Boolean Nlm_GetLayerStatus3D ( Nlm_Picture3D pic, Uint1 layer )
{
  register Nlm_PPict3DPtr p;

  Nlm_DiagReset ();
  p = (Nlm_PPict3DPtr)pic;
  return p->layerTable[layer];
}

Nlm_Segment3D Nlm_AddSegment3D ( Nlm_Picture3D pic, Nlm_Segment3D parent, 
                                 BigScalar userData, Uint1 layer )
{
  register Nlm_PSeg3DPtr  s;
  register Nlm_PPict3DPtr p;

  Nlm_DiagReset ();
  Nlm_stCon.pic = pic;
  p = (Nlm_PPict3DPtr)pic;
  s = (Nlm_PSeg3DPtr)MemNew ( sizeof(Nlm_PSeg3D) );
  if ( s == NULL ){
    Nlm_DiagPutRecord ( DA_ERROR, Pict3Dclass, "AddSegment3D",
                       "Memory allocation error" );
    return NULL;
  }
  s->base.fTable = &segmentDef;
  s->base.userData = userData;
  s->base.layer = layer;
  if ( parent == NULL ){
    Nlm_AddToSeg ( &(p->seg), (Nlm_Base3DPtr)s );
  } else {
    Nlm_AddToSeg ( (Nlm_PSeg3DPtr)parent, (Nlm_Base3DPtr)s );
  }
  p->version++;
  return (Nlm_Segment3D)s;
}

void Nlm_GetSegmentInfo3D ( Nlm_Picture3D pic, Nlm_Segment3D segment, 
                            BigScalar PNTR userData,  Uint1Ptr layer, 
                            Nlm_Segment3D PNTR parent )
{
  register Nlm_PSeg3DPtr  s;

  Nlm_DiagReset ();
  s = (Nlm_PSeg3DPtr)segment;
  if ( userData != NULL ) *userData = s->base.userData;
  if ( layer != NULL ) *layer = s->base.layer;
  if ( parent != NULL ) {
    if ( s->base.parent == (Nlm_Base3DPtr)&(((Nlm_PPict3DPtr)pic)->seg) ){
      *parent = NULL;
    } else {
      *parent = (Nlm_Segment3D)s->base.parent;
    }
  }
}


Nlm_Boolean Nlm_GetSegSphere3D (Nlm_Picture3D pic, Nlm_Segment3D segment,
                                Nlm_Spher3DPtr sph)
{
  register Nlm_PSeg3DPtr seg = (Nlm_PSeg3DPtr)segment;

  Nlm_DiagReset ();
  if (seg == NULL)  {
    if ( !Nlm_UpdateGver( (Nlm_PPict3DPtr)pic ) )
      return FALSE;
    seg = &((Nlm_PPict3DPtr)pic)->seg;
  }

  if (sph != NULL)
    *sph = seg->segSph;
  return TRUE;
}


Nlm_Boolean Nlm_GetSegBox3D (Nlm_Picture3D pic, Nlm_Segment3D segment,
                             Nlm_Box3DPtr box)
{
  register Nlm_PSeg3DPtr seg = (Nlm_PSeg3DPtr)segment;

  Nlm_DiagReset ();
  if (seg == NULL)  {
    if ( !Nlm_UpdateGver( (Nlm_PPict3DPtr)pic ) )
      return FALSE;
    seg = &((Nlm_PPict3DPtr)pic)->seg;
  }

  if (box != NULL)
    *box = seg->segBox;
  return TRUE;
}


void Nlm_ChangeSegment3D ( Nlm_Picture3D pic, Nlm_Segment3D segment, 
                           BigScalar userData, Uint1 layer )
{
  register Nlm_PSeg3DPtr  s;

  Nlm_DiagReset ();
  s = (Nlm_PSeg3DPtr)segment;
  s->base.userData = userData;
  s->base.layer = layer;
  ((Nlm_PPict3DPtr)pic)->version++;
}

void Nlm_DeleteSegment3D ( Nlm_Picture3D pic, Nlm_Segment3D segment )
{
  register Nlm_PSeg3DPtr  s;

  Nlm_DiagReset ();
  Nlm_stCon.pic = pic;
  s = (Nlm_PSeg3DPtr)segment;
  s->base.fTable->cleanup ( (Nlm_VoidPtr)s );
  Nlm_DeleteFromSeg ( (Nlm_PSeg3DPtr)s->base.parent, (Nlm_Base3DPtr)s );
  MemFree ( s );
  ((Nlm_PPict3DPtr)pic)->version++;
}

void Nlm_GetPrimInfo3D ( Nlm_Picture3D pic, Nlm_Prim3D prim, 
                         BigScalar PNTR userData, Uint1Ptr layer, Uint1Ptr color,
                         Nlm_Segment3D PNTR parent, Uint2Ptr primType )
{
  register Nlm_Base3DPtr p;

  Nlm_DiagReset ();
  p = (Nlm_Base3DPtr)prim;
  if ( userData != NULL ) *userData = p->userData;
  if ( layer != NULL ) *layer = p->layer;
  if ( color != NULL ) *color = p->color;
  if ( parent != NULL ) {
    if ( p->parent == (Nlm_Base3DPtr)&(((Nlm_PPict3DPtr)pic)->seg) ){
      *parent = NULL;
    } else {
      *parent = (Nlm_Segment3D)p->parent;
    }
  }
  if ( primType != NULL ) *primType = p->fTable->code;
}

void Nlm_ChangePrim3D ( Nlm_Picture3D pic, Nlm_Prim3D prim, BigScalar userData,
                        Uint1 layer, Uint1 color )
{
  register Nlm_Base3DPtr p;

  Nlm_DiagReset ();
  p = (Nlm_Base3DPtr)prim;
  p->userData = userData;
  p->layer = layer;
  p->color = color;
  ((Nlm_PPict3DPtr)pic)->version++;
}

void Nlm_DeletePrim3D ( Nlm_Picture3D pic, Nlm_Prim3D prim )
{
  register Nlm_Base3DPtr p;

  Nlm_DiagReset ();
  Nlm_stCon.pic = pic;
  p = (Nlm_Base3DPtr)prim;
  p->fTable->cleanup ( (Nlm_VoidPtr)p );
  Nlm_DeleteFromSeg ( (Nlm_PSeg3DPtr)p->parent, (Nlm_Base3DPtr)p );
  MemFree ( p );
  ((Nlm_PPict3DPtr)pic)->version++;
}

Nlm_Base3DPtr Nlm_AddPrim3D ( Nlm_PSeg3DPtr seg, Nlm_Base3DPtr prim,
                              size_t pSize, CharPtr funName )
{
  register Nlm_Base3DPtr p;
  Nlm_PPict3DPtr         pic;

  pic = (Nlm_PPict3DPtr)Nlm_stCon.pic;
  if ( (long)prim->color > (long)pic->totalColors ){
    Nlm_DiagPutRecord ( DA_ERROR, Pict3Dclass, funName,
                        "Color is out of range" );
    return NULL;
  }
  p = (Nlm_Base3DPtr)MemNew ( pSize );
  if ( p == NULL ){
    Nlm_DiagPutRecord ( DA_ERROR, Pict3Dclass, funName,
                        "Memory allocation error" );
    return NULL;
  }
  MemCopy ( p, prim, pSize );
  if ( seg == NULL ){
    Nlm_AddToSeg ( &(pic->seg), p );
  } else {
    Nlm_AddToSeg ( seg, p );
  }
  pic->version++;
  return p;
}

Nlm_Boolean Nlm_UpdateGver ( Nlm_PPict3DPtr pic )
{
  register Nlm_PPict3DPtr p = pic;
  if (p == NULL)
    return FALSE;

  Nlm_stCon.pic = (Nlm_Picture3D)p;
  if ( p->version != p->versionPrev ){
    p->seg.base.fTable->getlimits ( (Nlm_VoidPtr)&(p->seg) );
    p->versionPrev = p->version;
  }

  {{
    Nlm_Boolean empty = (Nlm_Boolean)
      (p->seg.segBox.minX == INT4_MAX  &&  p->seg.segBox.maxX == INT4_MIN  &&
       p->seg.segBox.minY == INT4_MAX  &&  p->seg.segBox.maxY == INT4_MIN  &&
       p->seg.segBox.minZ == INT4_MAX  &&  p->seg.segBox.maxZ == INT4_MIN);

    return (Nlm_Boolean)( !empty );
  }}
}

