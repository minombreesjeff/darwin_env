/*   pictur3d.h
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
* File Name:  pictur3d.h
*
* Author:  Alex Smirnov
*
* Version Creation Date:   03/29/95
*
* $Revision: 6.0 $
*
* File Description: 
*  3D picture public header file.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*/

#ifndef _PICTUR3D_
#define _PICTUR3D_

#ifndef _VIBRANT_
#include <vibrant.h>
#endif

#ifndef _DIAGNOST_
#include <diagnost.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   DEFINES
*
*****************************************************************************/
/* Primitives type */
#define LINE3D     0x1
#define CYLINDER3D 0x2
#define SPHERE3D   0x4
#define TEXT3D     0x8
#define POLY3D     0x10

/* Text3D flags */
#define TEXT3D_LEFT    0x1
#define TEXT3D_CENTER  0x2
#define TEXT3D_RIGTH   0x3
#define TEXT3D_UPPER   0x10
#define TEXT3D_MIDDLE  0x20
#define TEXT3D_LOWER   0x30
#define TEXT3D_FRONT   0x100

/*****************************************************************************
*
*   TYPEDEFS
*
*****************************************************************************/
typedef struct Nlm_picture3d {
  VoidPtr  dummy;
}PNTR Nlm_Picture3D;

typedef struct Nlm_segment3d {
  VoidPtr  dummy;
}PNTR Nlm_Segment3D;

typedef struct Nlm_primitive3d {
  VoidPtr  dummy;
}PNTR Nlm_Prim3D;

typedef struct Nlm_point3d {
  Int4 x;
  Int4 y;
  Int4 z;
} Nlm_Point3D, PNTR Nlm_Point3DPtr;

typedef struct Nlm_box3d {
  Int4 minX;
  Int4 minY;
  Int4 minZ;
  Int4 maxX;
  Int4 maxY;
  Int4 maxZ;
} Nlm_Box3D, PNTR Nlm_Box3DPtr;

typedef struct Nlm_spher3d {
  Int4 x;
  Int4 y;
  Int4 z;
  Int4 radius;
} Nlm_Spher3D, PNTR Nlm_Spher3DPtr;

/*****************************************************************************
*
*   EXTERN FUNCTION
*
*****************************************************************************/
extern Nlm_Picture3D Nlm_CreatePicture3D  PROTO((void));
extern void          Nlm_DeletePicture3D  PROTO((Nlm_Picture3D pic));
extern void          Nlm_ResetPicture3D   PROTO((Nlm_Picture3D pic));
/* Colors */
extern Boolean       Nlm_AllocPalette3D    PROTO((Nlm_Picture3D pic, 
                                    Uint1 totalColors));
extern Boolean       Nlm_SetColor3D        PROTO((Nlm_Picture3D pic, 
                                    Uint1 color, 
                                    Uint1 red, Uint1 green, Uint1 blue));
extern Boolean       Nlm_GetColor3D        PROTO((Nlm_Picture3D pic, 
                                    Uint1 color,
                                    Uint1Ptr red, Uint1Ptr green, 
												Uint1Ptr blue));
/* Layers */
extern void          Nlm_SetLayer3D        PROTO((Nlm_Picture3D pic, 
                                    Uint1 layer, Boolean layerOn));
extern Boolean       Nlm_GetLayerStatus3D  PROTO((Nlm_Picture3D pic, 
                                    Uint1 layer));

/* Segments */
extern Nlm_Segment3D Nlm_AddSegment3D      PROTO((Nlm_Picture3D pic, 
                                    Nlm_Segment3D parent, BigScalar userData, 
                                    Uint1 layer));
extern void          Nlm_GetSegmentInfo3D  PROTO((Nlm_Picture3D pic, 
                                    Nlm_Segment3D segment, BigScalar PNTR userData,
                                    Uint1Ptr layer, Nlm_Segment3D PNTR parent));

/* Nlm_GetSegSphere3D() and Nlm_GetSegBox3D() return FALSE if the picture
 * (or segment) contains no subsegments in currently active layers
 */
extern Nlm_Boolean   Nlm_GetSegSphere3D    PROTO((Nlm_Picture3D pic,
                                                  Nlm_Segment3D segment,
                                                  Nlm_Spher3DPtr sph ));
extern Nlm_Boolean   Nlm_GetSegBox3D       PROTO((Nlm_Picture3D pic,
                                                  Nlm_Segment3D segment,
                                                  Nlm_Box3DPtr box));

extern void          Nlm_ChangeSegment3D   PROTO((Nlm_Picture3D pic,
                                    Nlm_Segment3D segment, 
                                    BigScalar userData, Uint1 layer));
extern void          Nlm_DeleteSegment3D   PROTO((Nlm_Picture3D pic, 
                                    Nlm_Segment3D segment));
/* Primitives */
extern Nlm_Prim3D    Nlm_AddLine3D         PROTO((Nlm_Picture3D pic,
                                    Nlm_Segment3D segment, BigScalar userData,
                                    Uint1 layer, Uint1 color,
                                    Int4 x1, Int4 y1, Int4 z1,
												Int4 x2, Int4 y2, Int4 z2));
extern Nlm_Prim3D    Nlm_AddSphere3D       PROTO((Nlm_Picture3D pic,
                                    Nlm_Segment3D segment, BigScalar userData,
                                    Uint1 layer, Uint1 color,
                                    Int4 x, Int4 y, Int4 z, Uint4 radius));
extern Nlm_Prim3D    Nlm_AddCylinder3D     PROTO((Nlm_Picture3D pic,
                                    Nlm_Segment3D segment, BigScalar userData,
                                    Uint1 layer, Uint1 color,
												Int4 x1, Int4 y1, Int4 z1,
                                    Int4 x2, Int4 y2, Int4 z2, Uint4 radius));
extern Nlm_Prim3D    Nlm_AddText3D         PROTO((Nlm_Picture3D pic,
                                    Nlm_Segment3D segment, BigScalar userData,
                                    Uint1 layer, Uint1 color,
                                    CharPtr string, Int4 x, Int4 y, Int4 z, 
                                    Uint4 height, Int4 offset, Int2 flags));
extern Nlm_Prim3D    Nlm_AddPoly3D         PROTO((Nlm_Picture3D pic, 
                                    Nlm_Segment3D segment, BigScalar userData, 
                                    Uint1 layer, Uint1 color,
                                    Int4 x1, Int4 y1, Int4 z1,
                                    Int4 x2, Int4 y2, Int4 z2 ));
extern Boolean       Nlm_AddVertPoly3D     PROTO((Nlm_Picture3D pic, 
                                    Nlm_Prim3D poly,
                                    Int4 x, Int4 y, Int4 z ));
extern void          Nlm_GetPrimInfo3D     PROTO((Nlm_Picture3D pic, 
                                    Nlm_Prim3D prim, BigScalar PNTR userData,
                                    Uint1Ptr layer, Uint1Ptr color,
                                    Nlm_Segment3D PNTR parent, 
												Uint2Ptr primType));
extern void          Nlm_ChangePrim3D      PROTO((Nlm_Picture3D pic, 
                                    Nlm_Prim3D prim, BigScalar userData,
                                    Uint1 layer, Uint1 color));
extern void          Nlm_DeletePrim3D      PROTO((Nlm_Picture3D pic, 
                                    Nlm_Prim3D prim));

/*****************************************************************************
*
*   NAME REDIFINITION
*
*****************************************************************************/
#define Segment3D         Nlm_Segment3D
#define Prim3D            Nlm_Prim3D
#define Picture3D         Nlm_Picture3D
#define Point3D           Nlm_Point3D
#define Point3DPtr        Nlm_Point3DPtr
#define Box3D             Nlm_Box3D
#define Box3DPtr          Nlm_Box3DPtr
#define Spher3D           Nlm_Spher3D 
#define Spher3DPtr        Nlm_Spher3DPtr
#define CreatePicture3D   Nlm_CreatePicture3D
#define ResetPicture3D    Nlm_ResetPicture3D
#define DeletePicture3D   Nlm_DeletePicture3D
#define AllocPalette3D    Nlm_AllocPalette3D
#define SetColor3D        Nlm_SetColor3D
#define GetColor3D        Nlm_GetColor3D
#define SetLayer3D        Nlm_SetLayer3D
#define GetLayerStatus3D  Nlm_GetLayerStatus3D
#define AddSegment3D      Nlm_AddSegment3D
#define GetSegmentInfo3D  Nlm_GetSegmentInfo3D
#define ChangeSegment3D   Nlm_ChangeSegment3D
#define DeleteSegment3D   Nlm_DeleteSegment3D
#define AddLine3D         Nlm_AddLine3D
#define AddSphere3D       Nlm_AddSphere3D
#define AddCylinder3D     Nlm_AddCylinder3D
#define AddText3D         Nlm_AddText3D
#define AddPoly3D         Nlm_AddPoly3D
#define AddVertPoly3D     Nlm_AddVertPoly3D
#define GetPrimInfo3D     Nlm_GetPrimInfo3D
#define ChangePrim3D      Nlm_ChangePrim3D
#define DeletePrim3D      Nlm_DeletePrim3D

#ifdef __cplusplus
}
#endif

#endif
