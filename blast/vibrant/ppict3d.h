/*   ppict3d.h
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
* File Name:  ppict3d.h
*
* Author:  Alex Smirnov
*
* Version Creation Date:   03/29/95
*
* $Revision: 6.1 $
*
* File Description: 
*  3D picture private header file.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*/

#ifndef _PPICT3D_
#define _PPICT3D_

#ifndef _PICTUR3D_
#include <pictur3d.h>
#endif

#ifndef _PDIAGNOS_
#include <pdiagnos.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   DEFINES
*
*****************************************************************************/
#define HL_PRIMITIVE   0x1
#define HL_SEGMENT     0x2
#define HL_MASK        0x3

#ifdef WIN_MSWIN
#define NEGYSCREEN
#define Y_MINUS    -
#define Y_PLUS     +
#else
#define Y_MINUS    +
#define Y_PLUS     -
#endif

/*****************************************************************************
*
*   TYPEDEFS
*
*****************************************************************************/
typedef long PNTR longPtr;

typedef struct {
  Nlm_Picture3D pic;                      /* Current picture */
  Uint1Ptr      image;                    /* pointer to Image */
  Uint2Ptr      zBuffer;                  /* pointer to zBuffer */
  VoidPtr*      idBuffer;                 /* pointer to id Buffer */
  Int4          width;
  Int4          height;
  Int4          widthCur;
  Int4          heightCur;
  Int4          xmin, ymin;               /* Intrested area */
  Int4          xmax, ymax;
  Int4          zmax;
  Int4          a[3][3];                  /* xSCRN = x/a[0][0]+y/a[0][1]+ */
  Int4          c[3];                     /*        +z/a[0][2]+c[0]       */
  Int4          scale;
  Int4          colorOffset;              /* curColor = colorOffset +     */
  Int4          colorStep;                /*         3DColor*colorStep +  */
  Int4          colorScale;               /*         (curZ<<8)/colorScale */
  Int4          perspect;                 /* 1-10 */
  Int4          zmaxPersp;                /* zmax * perspect */
  Int4          zmaxPersp1;               /* zmax * (perspect + 1)*/
  Uint1         colorHL;
  Boolean       layerTable[256];
} Nlm_Context3D, PNTR Nlm_Context3DPtr;

typedef void        (*Nlm_PrimDraw3D)      PROTO((Nlm_VoidPtr objRef));
typedef void        (*Nlm_PrimHitTest3D)   PROTO((Nlm_VoidPtr objRef));
typedef void        (*Nlm_PrimGetLimits3D) PROTO((Nlm_VoidPtr objRef));
typedef void        (*Nlm_PrimCleanup3D)   PROTO((Nlm_VoidPtr objRef));

typedef struct Nlm_primDef3D {
  Nlm_PrimDraw3D        draw;
  Nlm_PrimHitTest3D     hittest;
  Nlm_PrimGetLimits3D   getlimits;
  Nlm_PrimCleanup3D     cleanup;
  Nlm_Int2              code;
}Nlm_PrimDef3D, PNTR Nlm_PrimDef3DPtr;

typedef struct Nlm_base3d {
  struct Nlm_base3d PNTR   parent;
  struct Nlm_base3d PNTR   next;
  Nlm_PrimDef3DPtr         fTable;
  BigScalar                   userData;
  Uint1                    layer;
  Uint1                    color;
  Uint2                    status;
}Nlm_Base3D, PNTR Nlm_Base3DPtr;

typedef struct Nlm_pseg3d {
  Nlm_Base3D     base;
  Nlm_Base3DPtr  last;
  Nlm_Base3DPtr  first;
  Nlm_Box3D      segBox;
  Nlm_Spher3D    segSph;
}Nlm_PSeg3D, PNTR Nlm_PSeg3DPtr;

typedef struct Nlm_ppict3d {
  Int4           version;
  Int4           versionPrev;
  Nlm_PSeg3D     seg;
  Boolean        layerTable[256];
  Uint1          colorR[256];
  Uint1          colorG[256];
  Uint1          colorB[256];
  Uint4          totalColors;
}Nlm_PPict3D, PNTR Nlm_PPict3DPtr;

/*****************************************************************************
*
*   EXTERN FUNCTION
*
*****************************************************************************/
extern Nlm_Base3DPtr Nlm_AddPrim3D  PROTO((Nlm_PSeg3DPtr seg, 
         Nlm_Base3DPtr prim, size_t pSize, CharPtr funName));
extern Nlm_Boolean   Nlm_UpdateGver PROTO((Nlm_PPict3DPtr pic));

#ifdef __cplusplus
}
#endif

#endif

