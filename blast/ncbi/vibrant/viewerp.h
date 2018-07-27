/*   viewerP.h
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
* File Name:  viewerP.h
*
* Author:  Jonathan Kans, Alex Smirnov, Jill Shermer
*
* Version Creation Date:   10/25/92
*
* $Revision: 6.2 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: viewerp.h,v $
* Revision 6.2  1997/11/19 17:38:13  kans
* added ChangeSentinelRectangle
*
* Revision 6.1  1997/11/18 00:41:09  kans
* PictureGrew is private extern
*
* Revision 6.0  1997/08/25 18:58:22  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 13:45:08  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 1.15  1995/05/17  15:15:14  kans
 * added Log line
 *
*
* ==========================================================================
*/

#ifndef _VIEWERP_
#define _VIEWERP_

#ifndef _VIBRANT_
#include <vibrant.h>
#endif

#ifndef _PICTURE_
#include <picture.h>
#endif

#ifndef _MAPPINGP_
#include <mappingp.h>
#endif

#ifndef _VIEWER_
#include <viewer.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   STRUCTURE TYPEDEFS
*
*****************************************************************************/

typedef struct Nlm_viewpdata {
  Nlm_VieweR       viewer;
  Nlm_SegmenT      picture;
  Nlm_VScaleInfo   scale;
  Nlm_VwrClckProc  click;
  Nlm_VwrClckProc  drag;
  Nlm_VwrClckProc  release;
  Nlm_VwrPanProc   pan;
  Nlm_VwrDrawProc  draw;
  Nlm_VoidPtr      data;
  Nlm_VwrFreeProc  cleanup;
} Nlm_ViewPData;

#define ViewPData Nlm_ViewPData

extern Nlm_Boolean Nlm_PictureGrew (Nlm_VieweR viewer);
extern void Nlm_ChangeSentinelRectangle (Nlm_PrimitivE prim, Nlm_Int4 left,
                                         Nlm_Int4 top, Nlm_Int4 right, Nlm_Int4 bottom);

#define PictureGrew Nlm_PictureGrew
#define ChangeSentinelRectangle Nlm_ChangeSentinelRectangle

#ifdef __cplusplus
}
#endif

#endif
