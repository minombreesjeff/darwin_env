/*   pictureP.h
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
* File Name:  pictureP.h
*
* Author:  Jonathan Kans, Alex Smirnov, Jill Shermer
*
* Version Creation Date:   10/23/92
*
* $Revision: 6.3 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 022495   Alex
*
* $Log: picturep.h,v $
* Revision 6.3  2002/08/07 18:13:42  kans
* G/SetPrimitiveIDs, itemID is Uint4
*
* Revision 6.2  1999/10/13 17:45:47  kans
* added entityID, itemID, and itemtype to primitive internal structure, added Get and Set functions
*
* Revision 6.1  1999/10/04 17:16:33  kans
* include ncbidraw.h instead of vibrant.h, a couple Nlm_ prefixes
*
* Revision 6.0  1997/08/25 18:56:27  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 13:45:08  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 1.14  1995/05/17  15:15:14  kans
 * added Log line
 *
*
* ==========================================================================
*/

#ifndef _PICTUREP_
#define _PICTUREP_

#ifndef _NCBIDRAW_
#include <ncbidraw.h>
#endif

#ifndef _PICTURE_
#include <picture.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   INTERNAL TYPE DEFINES
*
*****************************************************************************/

/*  Primitive types */

#define UNKNOWN     0
#define PICTURE     1
#define SEGMENT     2
#define GENERIC     3

/*****************************************************************************
*
*   STRUCTURE TYPEDEFS
*
*****************************************************************************/

/*  BasePRec is the base record of each primitive,
    storing the next pointer and an identity code */

typedef struct Nlm_baseprec {
  struct Nlm_baseprec  PNTR next;
  struct Nlm_baseprec  PNTR prev;
  Nlm_Int1                  code;
} Nlm_BasePData, Nlm_BasePRec, PNTR Nlm_BasePPtr;

typedef struct Nlm_attpdata {
  Nlm_Uint1  color [3];
  Nlm_Int1   linestyle;
  Nlm_Int1   shading;
  Nlm_Int1   penwidth;
  Nlm_Int1   mode;
} Nlm_AttPData, PNTR Nlm_AttPPtr;

typedef struct Nlm_segpdata {
  Nlm_BoxInfo   box;
  Nlm_BasePPtr  head;
  Nlm_BasePPtr  tail;
  Nlm_BasePPtr  parent;
  Nlm_Int4      maxscale;
  Nlm_Uint2     segID;
  Nlm_Int1      penwidth;
  Nlm_Int1      highlight;
  Nlm_Boolean   visible;
} Nlm_SegPData;

typedef struct Nlm_segprec {
  Nlm_BasePData  base;
  Nlm_SegPData   seg;
  Nlm_AttPData   attLast;
} Nlm_SegPRec, Nlm_PicPRec, PNTR Nlm_SegPPtr, PNTR Nlm_PicPPtr;

/* GenPRec is the base record of each visible primitive;
   pdp is the pointer to a method table */

typedef struct Nlm_genprec {
  Nlm_BasePData  base;
  Nlm_Int1       highlight;
  Nlm_AttPData   att;
  Nlm_Uint2      primID;
  Nlm_Uint2      entityID;
  Nlm_Uint4      itemID;
  Nlm_Uint2      itemtype;
  Nlm_PrimDefPtr pdp;
  double         data;
} Nlm_GenPRec, PNTR Nlm_GenPPtr;

#define BasePData Nlm_BasePData
#define BasePRec Nlm_BasePRec
#define BasePPtr Nlm_BasePPtr
#define AttPData Nlm_AttPData
#define AttPPtr Nlm_AttPPtr
#define SegPData Nlm_SegPData
#define SegPRec Nlm_SegPRec
#define PicPRec Nlm_PicPRec
#define SegPPtr Nlm_SegPPtr
#define PicPPtr Nlm_PicPPtr
#define GenPRec Nlm_GenPRec
#define GenPPtr Nlm_GenPPtr

#ifdef __cplusplus
}
#endif

#endif
