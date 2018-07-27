/*   $Id: viewmgr.h,v 1.18 2001/01/18 22:28:06 hurwitz Exp $
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
* File Name:  $Id: viewmgr.h,v 1.18 2001/01/18 22:28:06 hurwitz Exp $
*
* Author:  Lewis Geer
*
* Version Creation Date:   2/1/00
*
* $Revision: 1.18 $
*
* File Description: The ViewMgr is the part of the alignment management
*                   system that creates a viewable seqalign from an original
*                   seqalign (called a target) by applying a set of
*                   transforms to the target, such as show/hide rows.  The
*                   ViewMgr also deals with allocation/deallocation of
*                   the target and view seqaligns.
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: viewmgr.h,v $
* Revision 1.18  2001/01/18 22:28:06  hurwitz
* added fast option for ViewMgr_Update
*
* Revision 1.17  2000/08/30 13:43:11  lewisg
* change seqalign state when made into multiple
*
* Revision 1.16  2000/08/01 12:54:29  lewisg
* *** empty log message ***
*
* Revision 1.15  2000/07/08 20:43:55  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 1.14  2000/06/20 19:35:12  hurwitz
* use indexed seqAlign when necessary, make multiple when redrawing
*
* Revision 1.13  2000/06/13 18:23:54  hurwitz
* made ViewMgr_MakeMultiple routine, call this on each launch of DDE rather than launch of DDV
*
* Revision 1.12  2000/05/24 21:42:59  hurwitz
* getting hide/show rows to work with DDV and DDE together
*
* Revision 1.11  2000/04/17 21:47:52  lewisg
* do not do double index on viewmgr update, rename menus
*
* Revision 1.10  2000/03/29 23:38:06  lewisg
* hide/show, fixes to saving and opening
*
* Revision 1.9  2000/03/27 22:15:04  lewisg
* add show/hide row dialog
*
* Revision 1.8  2000/03/14 14:20:15  lewisg
* add extern row conversion functions
*
* Revision 1.7  2000/03/10 18:47:01  lewisg
* add show/hide
*
* Revision 1.6  2000/03/08 21:46:14  lewisg
* cn3d saves viewport, misc bugs
*
* Revision 1.5  2000/03/02 21:11:05  lewisg
* use bandalign for import sequence, make standalone ddv use viewmgr, make dialogs modal, send color update
*
* Revision 1.4  2000/03/01 22:53:41  lewisg
* import bioseq, neatlyindex, get rid of dead code
*
* Revision 1.3  2000/02/19 01:23:58  lewisg
* use ibm, add row tracking code, various bug fixes
*
* Revision 1.2  2000/02/15 22:40:57  lewisg
* add ability to launch udv so that it colors by row, fixes to colormgr, track rows from viewmgr, fix visual c projects
*
* Revision 1.1  2000/02/05 01:32:23  lewisg
* add viewmgr, move place freeing is done in ddv, modify visual c++ projects
*
*
*
* ==========================================================================
*/

#ifndef _VIEWMGR_
#define _VIEWMGR_

#include <ncbi.h>
#include <objalign.h>
#include <ddvcolor.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ViewMgr_ChildInfo {
    SeqAlign *pChild; /* pointer to the child */
    Boolean Hidden;   /* is this child hidden */
    Boolean Keep;     /* should this alignment be kept */
} ViewMgr_ChildInfo;

typedef struct _ViewMgr_AlignInfo {
    SeqAlign *pTarget, *pView, *pBegin, *pBeginIndexed;  
    /* the target, view, beginning, and indexed beginning seqaligns */
    ValNode *pHiddenRows; /* rows to hide */
    ValNode *pDeleteRows; /* rows to delete */
    Boolean Intersect;    /* do we run intersect on master? */
    Boolean Neat;         /* do we neatly index the seqalign? */
    ValNode *pChildInfo;  /* list of ViewMgr_ChildInfo, one per child. NYI */
    Uint2 entityID;
    ValNode *pUserKey2Row;  /* converts a userkey to a row #*/
    ValNode *pTransforms;    /* these point to valnode chains of ViewMgr_FromTo */
    Boolean BeginIndexed;
} ViewMgr_AlignInfo;

typedef struct _ViewMgr_Global {
    Int4 refcount;
    ValNode *pAlignList;  /* list of ViewMgr_AlignInfo */
    Uint2 userkey;
    Uint2 procid;
} ViewMgr_Global;

typedef struct _ViewMgr_FromTo {
    Int4 From;
    Int4 To;
} ViewMgr_FromTo;

#define VMGR_ROWWISE (0)
#define VMGR_DELETE (1)
#define VMGR_HIDE (2)
#define VMGR_IBM (3)
#define VMGR_REORDER (4)

/*****************************************************************************

Function: ViewMgr_VRow2TRow

Purpose: Converts a viewed row to a target row
  
Parameters: salp, the seqalign
            pGlobal, the ViewMgr_Global
            VRow, the row number in the viewed SeqAlign

Returns: the row in the target SeqAlign, -1 on failure

*****************************************************************************/
NLM_EXTERN Int4 ViewMgr_VRow2TRow(SeqAlign *salp, Int4 VRow);

/*****************************************************************************

Function: ViewMgr_TRow2VRow

Purpose: Finds a seqalign by pointer inside of the ViewMgr_Global
  
Parameters: salp, the seqalign
            TRow, the row number in the target SeqAlign

Returns: the row in the viewed SeqAlign, -1 on failure

*****************************************************************************/
NLM_EXTERN Int4 ViewMgr_TRow2VRow (SeqAlign *salp, Int4 TRow);

/*****************************************************************************

Function: ViewMgr_MakeMultiple

Purpose: Turns a pairwise seqalign into a multiple
  
Parameters: salp, the seqalign to be turned into a multiple

Returns: 1 on success, 0 otherwise

*****************************************************************************/

NLM_EXTERN Int4 ViewMgr_MakeMultiple(SeqAlign *salp);

/*****************************************************************************

Function: ViewMgr_GetTarget

Purpose: return the original *indexed* SeqAlign given the view seqalign
  
*****************************************************************************/
NLM_EXTERN SeqAlign * ViewMgr_GetTarget(SeqAlign *salp);

/*****************************************************************************

Function: ViewMgr_GetBegin

Purpose: return the original SeqAlign given the view seqalign
  
*****************************************************************************/
NLM_EXTERN SeqAlign * ViewMgr_GetBegin(SeqAlign *salp);

/*****************************************************************************

Function: ViewMgr_SetBegin

Purpose: make pNewBegin the original SeqAlign
  
*****************************************************************************/
NLM_EXTERN SeqAlign * ViewMgr_SetBegin(SeqAlign *salp, SeqAlign *pNewBegin,
                                       Boolean Neat, Boolean Intersect);

/*****************************************************************************

Function: ViewMgr_GetBeginIndexed

Purpose: return the original SeqAlign given the view seqalign.  This is
an indexed copy of the original SeqAlign.
  
*****************************************************************************/
NLM_EXTERN SeqAlign * ViewMgr_GetBeginIndexed(SeqAlign *salp);

/*****************************************************************************

Function: ViewMgr_IsNeat

Purpose: is the seqalign neatly indexed?
  
*****************************************************************************/
NLM_EXTERN Boolean ViewMgr_IsNeat(SeqAlign *salp);

/*****************************************************************************

Function: ViewMgr_IsIBM

Purpose: is the seqalign IBMed?
  
*****************************************************************************/
NLM_EXTERN Boolean ViewMgr_IsIBM(SeqAlign *salp);

/*****************************************************************************

Function: ViewMgr_SetRow

Purpose: Sets the row that a bioseq viewer is looking at, given the
         viewer's unique UserKey
  
Parameters: pGlobal, the DDV_ColorGlobal
            UserKey, the userkey of the viewer
            Row, the row the viewer is looking at

Returns: 1 on success, 0 on failure

*****************************************************************************/
NLM_EXTERN Int4 ViewMgr_SetRow(DDV_ColorGlobal *pCGlobal, Uint2 UserKey,
                               Int4 Row);

/*****************************************************************************

Function: ViewMgr_GetRow

Purpose: Returns the row that a bioseq viewer is looking at, given the
         viewer's unique UserKey
  
Parameters: pGlobal, the ViewMgr_Global
            UserKey, the userkey of the viewer

Returns: the row, -1 on failure

*****************************************************************************/
NLM_EXTERN Int4 ViewMgr_GetRow(DDV_ColorGlobal *pCGlobal, Uint2 UserKey);

/*****************************************************************************

Function: ViewMgr_FreeInfo

Purpose: Free a ViewMgr_AlignInfo
  
Parameters: pInfo, to be freed

Returns: 1 on success

*****************************************************************************/
NLM_EXTERN Int4 ViewMgr_FreeInfo(ViewMgr_AlignInfo *pInfo);

/*****************************************************************************

Function: ViewMgr_RemoveSA

Purpose: Takes the information about a seqalign out of the ViewMgr
  
Parameters: salp, the seqalign to remove

Returns: 1 on success

*****************************************************************************/
NLM_EXTERN Int4 ViewMgr_RemoveSA(SeqAlign *salp);

/*****************************************************************************

Function: ViewMgr_New

Purpose: Creates a new View Manager global for tracking SeqAligns
  
Returns: the new global

*****************************************************************************/
NLM_EXTERN ViewMgr_Global * ViewMgr_New(void);

/*****************************************************************************

Function: ViewMgr_Attach

Purpose:  Adds the seqalign to the global and starts tracking it
  
Parameters: salp, a seqalign that *isn't* indexed.
            Intersect, true if intersect-on-master should be applied
            Neat, true if AlnMgrNeatlyIndex should be applied
            entityID, itemID of object passed in.  If entityID == 0,
                then salp is used to look up the values.

Returns: 0 on error, 1 otherwise

Notes: will index the seqalign

*****************************************************************************/
NLM_EXTERN Int4 ViewMgr_Attach(SeqAlign *salp, Boolean Neat,
                               Boolean Intersect, Uint2 entityID,
                               Uint2 itemID);

/*****************************************************************************

Function: ViewMgr_Add

Purpose: Add a seqalign to a seqalign already in the viewmgr
  
Parameters: salp, the seqalign to be added to.  Must be attached to viewmgr
            salpAdd, the seqalign to be added.  Shouldn't be indexed.

Returns: 1 on success

*****************************************************************************/
NLM_EXTERN Int4 ViewMgr_Add(SeqAlign *salp, SeqAlign *salpAdd);

/*****************************************************************************

Function: ViewMgr_SetHidden

Purpose: Set a row to contribute / not contribute to the seqalign
  
Parameters: salp, the seqalign to be shown/hid
            Row, the row to show or hide
            Hidden, 

Returns: 1 on success, 0 on setup failure, -1 if the requested action can't
         be done.

*****************************************************************************/
NLM_EXTERN Int4 ViewMgr_SetHidden(SeqAlign *salp, Boolean Hidden, Int4 Row);

/*****************************************************************************

Function: ViewMgr_Update

Purpose: Copies the target into the viewed seqalign, performs all the 
        transformations, then sends an update message
  
Parameters: salp, the seqalign to work on

Returns: 1 on success

*****************************************************************************/
NLM_EXTERN Int4 ViewMgr_Update(SeqAlign *salp);
NLM_EXTERN Int4 ViewMgr_Update2(SeqAlign *salp, Boolean Fast);

/*****************************************************************************

Function: ViewMgr_RegisterFunc

Purpose: Adds the ViewMgr message function to the seqalign
  
*****************************************************************************/
NLM_EXTERN Int2 LIBCALLBACK ViewMgr_RegisterFunc(Pointer data);

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
