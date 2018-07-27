/*   $Id: viewmgr.c,v 1.36 2002/05/09 15:34:51 kans Exp $
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
* File Name:  $Id: viewmgr.c,v 1.36 2002/05/09 15:34:51 kans Exp $
*
* Author:  Lewis Geer
*
* Version Creation Date:   2/1/00
*
* $Revision: 1.36 $
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
* $Log: viewmgr.c,v $
* Revision 1.36  2002/05/09 15:34:51  kans
* commented out unneeded headers in tools library
*
* Revision 1.35  2001/01/18 22:28:06  hurwitz
* added fast option for ViewMgr_Update
*
* Revision 1.34  2001/01/07 08:11:13  vakatov
* Fixed C++ style comments to the C-style ones
*
* Revision 1.33  2001/01/06 01:09:00  lewisg
* get indexed begin in make multiple, copy seqaligns correctly
*
* Revision 1.32  2001/01/02 23:38:20  lewisg
* null target and begin pointers
*
* Revision 1.31  2000/12/29 21:10:43  lewisg
* bug fixes
*
* Revision 1.30  2000/08/30 13:43:11  lewisg
* change seqalign state when made into multiple
*
* Revision 1.29  2000/08/01 12:54:29  lewisg
* *** empty log message ***
*
* Revision 1.28  2000/06/20 19:35:11  hurwitz
* use indexed seqAlign when necessary, make multiple when redrawing
*
* Revision 1.27  2000/06/16 13:44:10  lewisg
* fix Int4->int fcn declaration
*
* Revision 1.26  2000/06/13 18:23:54  hurwitz
* made ViewMgr_MakeMultiple routine, call this on each launch of DDE rather than launch of DDV
*
* Revision 1.25  2000/06/08 20:52:42  hurwitz
* fixed a bug with show/hide rows
*
* Revision 1.24  2000/06/07 23:08:48  lewisg
* fix show/hide for large number of rows
*
* Revision 1.23  2000/05/12 16:15:26  hurwitz
* reverted to not doing IntersectOnMaster for DDE, now determined by call to ViewMgr_Attach
*
* Revision 1.22  2000/05/10 19:02:36  hurwitz
* for dde, always do IntersectOnMaster
*
* Revision 1.21  2000/04/17 21:46:55  lewisg
* do not do double index on viewmgr update, rename menus
*
* Revision 1.20  2000/04/10 20:33:40  lewisg
* fix show/hide for blast multiple, make blast multiple API generic
*
* Revision 1.19  2000/04/07 18:57:17  thiessen
* fix VRow2TRow
*
* Revision 1.18  2000/04/04 22:18:42  lewisg
* add defline to ddv, fix seq import bugs, set boundbox
*
* Revision 1.17  2000/04/04 17:51:54  lewisg
* fix various seq import bugs
*
* Revision 1.16  2000/03/29 23:38:06  lewisg
* hide/show, fixes to saving and opening
*
* Revision 1.15  2000/03/27 22:15:04  lewisg
* add show/hide row dialog
*
* Revision 1.14  2000/03/15 19:32:20  lewisg
* launch only single udv window
*
* Revision 1.13  2000/03/14 14:20:15  lewisg
* add extern row conversion functions
*
* Revision 1.12  2000/03/10 18:47:00  lewisg
* add show/hide
*
* Revision 1.11  2000/03/08 21:46:13  lewisg
* cn3d saves viewport, misc bugs
*
* Revision 1.10  2000/03/02 21:11:05  lewisg
* use bandalign for import sequence, make standalone ddv use viewmgr, make dialogs modal, send color update
*
* Revision 1.9  2000/03/01 22:53:40  lewisg
* import bioseq, neatlyindex, get rid of dead code
*
* Revision 1.8  2000/02/22 17:22:33  lewisg
* fix mac error
*
* Revision 1.7  2000/02/19 01:23:58  lewisg
* use ibm, add row tracking code, various bug fixes
*
* Revision 1.6  2000/02/16 16:17:34  lewisg
* add extra argument to AlnMgrMakeMultByIntersectOnMaster
*
* Revision 1.5  2000/02/15 23:02:46  kans
* return 0 instead of NULL for Int4 return value - too bad only the Mac compiler can catch this
*
* Revision 1.4  2000/02/15 22:40:57  lewisg
* add ability to launch udv so that it colors by row, fixes to colormgr, track rows from viewmgr, fix visual c projects
*
* Revision 1.3  2000/02/10 15:51:58  lewisg
* cn3d responds and send correct update messages.  many coloring bug fixes
*
* Revision 1.2  2000/02/05 19:37:45  kans
* fixed almost certain unintended assignment in if statement - caught by Mac compiler
*
* Revision 1.1  2000/02/05 01:32:23  lewisg
* add viewmgr, move place freeing is done in ddv, modify visual c++ projects
*
*
*
* ==========================================================================
*/

#include <viewmgr.h>
#include <objmgr.h>
#include <alignmgr.h>
#include <sqnutils.h>
/*
#include <ddvcolor.h>
#include <actutils.h>
*/


/*****************************************************************************

Function: ViewMgr_Find

Purpose: Finds a seqalign by pointer inside of the ViewMgr_Global
  
Parameters: salp, the seqalign
            pGlobal, the ViewMgr_Global

Returns: a pointer to info about the alignment

*****************************************************************************/

static ViewMgr_AlignInfo * ViewMgr_Find(ViewMgr_Global *pGlobal,
                                        SeqAlign *salp)
{
    ValNode *pvn;
    ViewMgr_AlignInfo *pInfo;


    if(pGlobal == NULL || salp == NULL) return NULL;

    for(pvn = pGlobal->pAlignList; pvn != NULL; pvn = pvn->next) {
        pInfo = (ViewMgr_AlignInfo *)pvn->data.ptrvalue;
        if(pInfo == NULL) continue;
        if(pInfo->pView == salp) return pInfo;
    }
    return NULL;
}

static ViewMgr_AlignInfo * ViewMgr_GetInfo(SeqAlign *salp)
{
    ViewMgr_Global *pGlobal;

    pGlobal = GetAppProperty("ViewMgr");
    if(pGlobal == NULL || salp == NULL) return NULL;
    return ViewMgr_Find(pGlobal, salp);
}

static Int4 ViewMgr_xVRow2TRow(ViewMgr_Global *pGlobal, SeqAlign *salp,
                               Int4 VRow)
{
    ViewMgr_AlignInfo * pInfo;
    ValNode *pvn;
    Int4 TRow, nVRow;
    Boolean isHidden;

    if(pGlobal == NULL || salp == NULL || VRow <= 0) return -1;
    pInfo = ViewMgr_Find(pGlobal, salp);
    if(pInfo == NULL) ErrorReturn(SEV_ERROR, "ViewMgr_xVRow2TRow", -1);

    for (TRow = 1, nVRow = 0; ; TRow++) {
        isHidden = FALSE;
        for (pvn = pInfo->pHiddenRows; pvn != NULL; pvn = pvn->next) {
            if (pvn->data.intvalue == TRow) {
                isHidden = TRUE;
                break;
            }
        }
        if (!isHidden) nVRow++;
        if (nVRow == VRow)
            break;
    }
    return TRow;
}

/*****************************************************************************

Function: ViewMgr_VRow2TRow

Purpose: Converts a viewed row to a target row
  
Parameters: salp, the seqalign
            VRow, the row number in the viewed SeqAlign

Returns: the row in the target SeqAlign, -1 on failure

*****************************************************************************/
NLM_EXTERN Int4 ViewMgr_VRow2TRow(SeqAlign *salp, Int4 VRow)
{
    ViewMgr_Global *pGlobal;

    pGlobal = GetAppProperty("ViewMgr");
    if (pGlobal == NULL) 
        ErrorReturn(SEV_ERROR, "ViewMgr_TRow2VRow", -1);
    return ViewMgr_xVRow2TRow(pGlobal, salp, VRow);
}


static Int4 ViewMgr_xTRow2VRow (ViewMgr_Global *pGlobal, SeqAlign *salp,
                               Int4 TRow)
{
    ViewMgr_AlignInfo * pInfo;
    ValNode *pvn;
    Int4 VRow = TRow;

    if(pGlobal == NULL || salp == NULL) 
        ErrorReturn(SEV_ERROR, "ViewMgr_xTRow2VRow", -1);
    pInfo = ViewMgr_Find(pGlobal, salp);
    if(pInfo == NULL) return -1;

    for(pvn = pInfo->pHiddenRows; pvn != NULL; pvn = pvn->next) {
        if(pvn->data.intvalue < TRow) VRow--;
        if(pvn->data.intvalue == TRow) return -1;
    }
    return VRow;
}

/*****************************************************************************

Function: ViewMgr_TRow2VRow

Purpose: Finds a seqalign by pointer inside of the ViewMgr_Global
  
Parameters: salp, the seqalign
            TRow, the row number in the target SeqAlign

Returns: the row in the viewed SeqAlign, -1 on failure

*****************************************************************************/
NLM_EXTERN Int4 ViewMgr_TRow2VRow (SeqAlign *salp, Int4 TRow)
{
    ViewMgr_Global *pGlobal;

    pGlobal = GetAppProperty("ViewMgr");
    if (pGlobal == NULL) 
        ErrorReturn(SEV_ERROR, "ViewMgr_TRow2VRow", -1);
    return ViewMgr_xTRow2VRow(pGlobal, salp, TRow);
}


/* frees a from/to valnode chain */
static void ViewMgr_ClearFromTo(ValNode *Transform)
{
    ValNode *pvn;

    if(Transform == NULL) return;
    for(pvn = (ValNode *)Transform->data.ptrvalue; pvn != NULL;
        pvn = pvn->next) MemFree(pvn->data.ptrvalue);
    ValNodeFree((ValNode *)Transform->data.ptrvalue);
}

Int4 ViewMgr_ClearTrans(SeqAlign *salp)
{
    ViewMgr_Global *pGlobal;
    ValNode *pvn;
    ViewMgr_AlignInfo *pInfo;

    pGlobal = GetAppProperty("ViewMgr");
    if (pGlobal == NULL) 
        ErrorReturn(SEV_ERROR, "ViewMgr_ClearTrans", -1);

    pInfo = ViewMgr_Find(pGlobal, salp);
    if (pInfo == NULL) ErrorReturn(SEV_ERROR, "ViewMgr_ClearTrans", -1);

    for(pvn = pInfo->pTransforms; pvn != NULL; pvn = pvn->next)
        ViewMgr_ClearFromTo(pvn);

    ValNodeFree(pInfo->pTransforms);
    return 1;
}

static void ViewMgr_AddFromTo(ValNode *Transform, Int4 From, Int4 To)
{
    ValNode *pvn;
    ViewMgr_FromTo *pFromTo;
    
    if(Transform == NULL) ErrorReturnVoid(SEV_ERROR, "ViewMgr_AddFromTo");
    for(pvn = (ValNode *)Transform->data.ptrvalue; pvn != NULL;
    pvn = pvn->next) {
        pFromTo = pvn->data.ptrvalue;
        if(pFromTo == NULL) continue;
        if(pFromTo->From == From || pFromTo->To == To) return;
    }
    
    pFromTo = MemNew(sizeof(ViewMgr_FromTo));
    pFromTo->From = From;
    pFromTo->To = To;
    
    pvn = (ValNode *)Transform->data.ptrvalue;
    ValNodeAddPointer(&pvn, 0, pFromTo);
}


Int4 ViewMgr_AddTransform(SeqAlign * salp, Int4 Transform, Int4 From, Int4 To)
{
    ValNode *pvn;
    ViewMgr_AlignInfo *pInfo;

    pInfo = ViewMgr_GetInfo(salp);
    if (pInfo == NULL) ErrorReturn(SEV_ERROR, "ViewMgr_AddTransform", -1);

    for(pvn = pInfo->pTransforms; pvn != NULL; pvn = pvn->next)
        if(Transform == pvn->choice) break;

    if(pvn == NULL) pvn = ValNodeAddPointer(&pInfo->pTransforms, (Nlm_Int2)Transform,
        NULL);
    ViewMgr_AddFromTo(pvn, From, To);
    return 1;
}


/*****************************************************************************

Function: ViewMgr_GetRow

Purpose: Returns the row that a bioseq viewer is looking at, given the
         viewer's unique UserKey
  
Parameters: pGlobal, the ViewMgr_Global
            UserKey, the userkey of the viewer

Returns: the row, -1 on failure

*****************************************************************************/

NLM_EXTERN Int4 ViewMgr_GetRow(DDV_ColorGlobal *pCGlobal, Uint2 UserKey)
{
    ViewMgr_Global *pVGlobal;
    ViewMgr_AlignInfo *pInfo;
    ValNode *pvn;

    pVGlobal = GetAppProperty("ViewMgr");
    if (pVGlobal == NULL) ErrorReturn(SEV_ERROR, "ViewMgr_GetRow", -1);

    pInfo = ViewMgr_Find(pVGlobal, (SeqAlign *)pCGlobal->pObject);
    if(pInfo == NULL) ErrorReturn(SEV_ERROR, "ViewMgr_GetRow", -1);

    for(pvn = pInfo->pUserKey2Row; pvn != NULL; pvn = pvn->next)
        if(UserKey == pvn->choice) return ViewMgr_xTRow2VRow(pVGlobal,
            (SeqAlign *)pCGlobal->pObject, pvn->data.intvalue);
    ErrorReturn(SEV_ERROR, "ViewMgr_GetRow", -1);
}

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
                               Int4 Row)
{
    ViewMgr_Global *pVGlobal;
    ViewMgr_AlignInfo *pInfo;

    pVGlobal = GetAppProperty("ViewMgr");
    if (pVGlobal == NULL) ErrorReturn(SEV_ERROR, "ViewMgr_SetRow", -1);

    pInfo = ViewMgr_Find(pVGlobal, (SeqAlign *)pCGlobal->pObject);
    if(pInfo == NULL) ErrorReturn(SEV_ERROR, "ViewMgr_SetRow", -1);

    ValNodeAddInt(&pInfo->pUserKey2Row, UserKey, ViewMgr_xVRow2TRow(pVGlobal,
            (SeqAlign *)pCGlobal->pObject, Row));

    return 1;
}

/*****************************************************************************

Function: ViewMgr_FreeInfo

Purpose: Free a ViewMgr_AlignInfo
  
Parameters: pInfo, to be freed

Returns: 1 on success

*****************************************************************************/

NLM_EXTERN Int4 ViewMgr_FreeInfo(ViewMgr_AlignInfo *pInfo)
{
    if(pInfo == NULL) ErrorReturn(SEV_ERROR, "ViewMgr_FreeInfo", -1);
    SeqAlignListFree(pInfo->pTarget);
    SeqAlignListFree(pInfo->pBegin);
    SeqAlignListFree(pInfo->pBeginIndexed);
    ValNodeFree(pInfo->pHiddenRows);
    ValNodeFree(pInfo->pDeleteRows);
    /*ViewMgr_ClearTrans(pInfo->pTransforms);*/
    MemFree(pInfo);
    return 1;
}


/*****************************************************************************

Function: ViewMgr_RemoveSA

Purpose: Takes the information about a seqalign out of the ViewMgr
  
Parameters: salp, the seqalign to remove

Returns: 1 on success

*****************************************************************************/

NLM_EXTERN Int4 ViewMgr_RemoveSA(SeqAlign *salp)
{
    ViewMgr_AlignInfo *pInfo;
    ValNode *pvn, *pvnPrevious = NULL;
    ViewMgr_Global *pGlobal;

    pGlobal = GetAppProperty("ViewMgr");
    if(pGlobal == NULL) return -1;
    for(pvn = pGlobal->pAlignList; pvn != NULL; pvn = pvn->next) {
        pInfo = (ViewMgr_AlignInfo *)pvn->data.ptrvalue;
        if(pInfo == NULL) continue;
        if(pInfo->pView == salp) break;
        pvnPrevious = pvn;
    }
    
    if(pvn == NULL) return -1;
    if(pvn == pGlobal->pAlignList) {
        ViewMgr_FreeInfo(pInfo);
        if(pvn->next != NULL) pGlobal->pAlignList = pvn->next;
        else pGlobal->pAlignList = NULL;
        MemFree(pvn);
    }
    else {
        pvnPrevious->next = pvn->next;
        ViewMgr_FreeInfo(pInfo);
        MemFree(pvn);
    }
    return 1;
}


/*****************************************************************************

Function: ViewMgr_New

Purpose: Creates a new View Manager global for tracking SeqAligns
  
Returns: the new global

*****************************************************************************/

NLM_EXTERN ViewMgr_Global * ViewMgr_New(void)
{
    ViewMgr_Global *pGlobal;

    pGlobal = GetAppProperty("ViewMgr");
    if (pGlobal == NULL) {
        pGlobal = MemNew(sizeof(ViewMgr_Global));
        if (pGlobal == NULL) return NULL;
        SetAppProperty("ViewMgr", pGlobal);

        /* set up the registration function and get a userkey */
        ObjMgrProcLoad(OMPROC_VIEW, "ViewMgr Register", "Media", OBJ_SEQALIGN, 0,
            OBJ_SEQALIGN, 0, NULL, ViewMgr_RegisterFunc, 0);
        
        pGlobal->userkey = OMGetNextUserKey();
    }
    return pGlobal;
}

/*****************************************************************************

Function: ViewMgr_GetTarget

Purpose: return the original *indexed* SeqAlign given the view seqalign
  
*****************************************************************************/
NLM_EXTERN SeqAlign * ViewMgr_GetTarget(SeqAlign *salp)
{
    ViewMgr_AlignInfo *pInfo;

    pInfo = ViewMgr_GetInfo(salp);
    if(pInfo == NULL) return NULL;

    return pInfo->pTarget;
}

/*****************************************************************************

Function: ViewMgr_GetBegin

Purpose: return the original SeqAlign given the view seqalign
  
*****************************************************************************/
NLM_EXTERN SeqAlign * ViewMgr_GetBegin(SeqAlign *salp)
{
    ViewMgr_AlignInfo *pInfo;

    pInfo = ViewMgr_GetInfo(salp);
    if(pInfo == NULL) return NULL;

    return pInfo->pBegin;
}

/*****************************************************************************

Function: ViewMgr_SetBegin

Purpose: make pNewBegin the original SeqAlign
  
*****************************************************************************/
NLM_EXTERN SeqAlign * ViewMgr_SetBegin(SeqAlign *salp, SeqAlign *pNewBegin,
                                       Boolean Neat, Boolean Intersect)
{
    ViewMgr_AlignInfo *pInfo;

    pInfo = ViewMgr_GetInfo(salp);
    if(pInfo == NULL) return NULL;
    SeqAlignListFree(pInfo->pBegin);
    pInfo->pBegin = pNewBegin;
    pInfo->Neat = Neat;
    pInfo->Intersect = Intersect;
    return pInfo->pBegin;
}

/*****************************************************************************

Function: ViewMgr_GetBeginIndexed

Purpose: return the original SeqAlign given the view seqalign.  This is
an indexed copy of the original SeqAlign.
  
*****************************************************************************/
NLM_EXTERN SeqAlign * ViewMgr_GetBeginIndexed(SeqAlign *salp)
{
    ViewMgr_AlignInfo *pInfo;

    pInfo = ViewMgr_GetInfo(salp);
    if(pInfo == NULL) return NULL;

    if(!pInfo->BeginIndexed) {
        if(pInfo->Intersect) {
            AlnMgrMakeMultByIntersectOnMaster(pInfo->pBeginIndexed, TRUE);
        }
        else {
            AlnMgrIndexSeqAlign(pInfo->pBeginIndexed);
        }
        pInfo->BeginIndexed = TRUE;
    }
    return pInfo->pBeginIndexed;
}


/*****************************************************************************

Function: ViewMgr_IsNeat

Purpose: is the seqalign neatly indexed?
  
*****************************************************************************/
NLM_EXTERN Boolean ViewMgr_IsNeat(SeqAlign *salp)
{
    ViewMgr_AlignInfo *pInfo;
    pInfo = ViewMgr_GetInfo(salp);
    if(!pInfo) return FALSE;
    
    if(pInfo->Neat) return TRUE;
    return FALSE;
}


/*****************************************************************************

Function: ViewMgr_IsIBM

Purpose: is the seqalign IBMed?
  
*****************************************************************************/
NLM_EXTERN Boolean ViewMgr_IsIBM(SeqAlign *salp)
{
    ViewMgr_AlignInfo *pInfo;
    pInfo = ViewMgr_GetInfo(salp);
    if(!pInfo) return FALSE;
    
    if(pInfo->Intersect) return TRUE;
    return FALSE;
}


/*****************************************************************************

Function: ViewMgr_Delete

Purpose: Creates a new View Manager global for tracking SeqAligns
  
Returns: the new global

*****************************************************************************/

NLM_EXTERN ViewMgr_Global * ViewMgr_Delete(ViewMgr_Global *pGlobal)
{
    ValNode * pvn;
    if (pGlobal != NULL) {
        for(pvn = pGlobal->pAlignList; pvn != NULL; pvn = pvn->next) {
            ViewMgr_FreeInfo((ViewMgr_AlignInfo *)pvn->data.ptrvalue);
        }

        ValNodeFree(pGlobal->pAlignList);
        pGlobal = MemFree(pGlobal);
    }
    return(pGlobal);
}

static void ViewMgr_Swap(SeqAlign *salp1, SeqAlign *salp2)
{
    void *pSwap;
    Int4 lSwap;
    GatherIndex gSwap;
    
    lSwap = (Int4)salp1->type;
    salp1->type = salp2->type;
    salp2->type = (Uint1)lSwap;
    
    lSwap = (Int4)salp1->segtype;
    salp1->segtype = salp2->segtype;
    salp2->segtype = (Uint1)lSwap;
    
    lSwap = (Int4)salp1->dim;
    salp1->dim = salp2->dim;
    salp2->dim = (Int2)lSwap;
    
    pSwap = (void *)salp1->score;
    salp1->score = salp2->score;
    salp2->score = (ScorePtr)pSwap;
    
    pSwap = (void *)salp1->segs;
    salp1->segs = salp2->segs;
    salp2->segs = pSwap;
    
    pSwap = (void *)salp1->next;
    salp1->next = salp2->next;
    salp2->next = (SeqAlign *)pSwap;
    
    pSwap = (void *)salp1->bounds;
    salp1->bounds = salp2->bounds;
    salp2->bounds = (SeqLocPtr)pSwap;
    
    pSwap = (void *)salp1->master;
    salp1->master = salp2->master;
    salp2->master = (SeqIdPtr)pSwap;
    
    pSwap = (void *)salp1->saip;
    salp1->saip = salp2->saip;
    salp2->saip = (SeqAlignIndexPtr)pSwap;
    
    MemCpy((void *)&gSwap, (void *)&salp1->idx, sizeof(GatherIndex));
    MemCpy((void *)&salp1->idx, (void *)&salp2->idx, sizeof(GatherIndex));
    MemCpy((void *)&salp2->idx, (void *)&gSwap, sizeof(GatherIndex));
    
    lSwap = (Int4)salp1->alignID;
    salp1->alignID = salp2->alignID;
    salp2->alignID = (Uint2)lSwap;
}

static int LIBCALLBACK ViewMgr_CmpInt(void *int1, void *int2)
{
    if(int1 && int2) return *(Int4 *)int2 - *(Int4 *)int1;
    else return 0;
}

static Int4 ViewMgr_Massage(ViewMgr_AlignInfo *pInfo)
{
    Int4 *throwaway, len, i;
    ValNode *pvn;
    
    SeqAlignListFree(pInfo->pTarget);
    pInfo->pTarget = NULL;
    SeqAlignListFree(pInfo->pBeginIndexed);
    pInfo->pBeginIndexed = NULL;
    pInfo->BeginIndexed = FALSE;

    if(pInfo->Neat) {
        if(!AlnMgrNeatlyIndex(pInfo->pBegin)) 
            ErrorReturn(SEV_ERROR, "ViewMgr_Massage", -1);
        /* delete neat rows */
        len = ValNodeLen(pInfo->pHiddenRows);
        pInfo->pBeginIndexed = SeqAlignListDup(pInfo->pBegin);
        AlnMgrNeatlyIndex(pInfo->pBeginIndexed);
        if(len > 0) {
            throwaway = MemNew(sizeof(Int4)*len);
            for(pvn = pInfo->pHiddenRows, i = 0; pvn != NULL; pvn = pvn->next, i++) 
                throwaway[i] = pvn->data.intvalue;
            pInfo->pTarget = AlnMgrTossNeatRows(pInfo->pBegin, throwaway, len);
            MemFree(throwaway);
        }
        else { 
            pInfo->pTarget = SeqAlignListDup(pInfo->pBegin);
            AlnMgrNeatlyIndex(pInfo->pTarget);
        }
    }
    else {
        pInfo->pTarget = SeqAlignListDup(pInfo->pBegin);
        pInfo->pBeginIndexed = SeqAlignListDup(pInfo->pBegin);
    }
    
    if(pInfo->Intersect) {
        AlnMgrMakeMultByIntersectOnMaster(pInfo->pTarget, TRUE);
    }
    else {
        AlnMgrIndexSeqAlign(pInfo->pTarget);

        /* delete unneat rows */
        if(!pInfo->Neat) {
            len = ValNodeLen(pInfo->pHiddenRows);
            if(len > 0) {
                throwaway = MemNew(sizeof(Int4)*len);
                for(pvn = pInfo->pHiddenRows, i = 0; pvn != NULL; pvn = pvn->next, i++) 
                    throwaway[i] = pvn->data.intvalue;
                HeapSort(throwaway, len, sizeof(Int4), ViewMgr_CmpInt);
                for(i = 0; i < len; i++) {
                    AlnMgrDeleteNthRow(pInfo->pTarget, throwaway[i]);
                    AlnMgrReIndexSeqAlign(pInfo->pTarget);
                }
                MemFree(throwaway);
            }
        }
    }
    return 1;
}

/*****************************************************************************

Function: ViewMgr_MakeMultiple

Purpose: Turns a pairwise seqalign into a multiple
  
Parameters: salp, the seqalign to be turned into a multiple

Returns: 1 on success, 0 otherwise

*****************************************************************************/

NLM_EXTERN Int4 ViewMgr_MakeMultiple(SeqAlign *salp)
{
    ViewMgr_AlignInfo *pInfo;
    SeqAlign *pSeqAlign;

    if(salp == NULL) 
        ErrorReturn(SEV_ERROR, "ViewMgr_MakeMultiple", -1);
    pInfo = ViewMgr_GetInfo(salp);
    if(pInfo == NULL) ErrorReturn(SEV_ERROR, "ViewMgr_MakeMultiple", -1);

    pSeqAlign = AlnMgrGetSubAlign(ViewMgr_GetBeginIndexed(salp), NULL, 0, -1);

    if(pSeqAlign == NULL) return 0;
    pInfo->pBegin = pSeqAlign;
    /*    ViewMgr_Massage(pInfo); */

    /*    AlnMgrCopyIndexedParentIntoSap(pInfo->pTarget, salp); */
    
    return ViewMgr_Update(salp);
}


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
                               Uint2 itemID)
{
    ViewMgr_AlignInfo *pInfo;
    ViewMgr_Global *pGlobal;

    pGlobal = ViewMgr_New();
    if(pGlobal == NULL || salp == NULL) 
        ErrorReturn(SEV_ERROR, "ViewMgr_Attach", -1);

    /* don't bother attaching it again */
    if(ViewMgr_Find(pGlobal, salp) != NULL) return 1;

    if (entityID == 0) {
        entityID = ObjMgrGetEntityIDForPointer(salp);
        itemID =
            GetItemIDGivenPointer(entityID, OBJ_SEQALIGN, (void *) salp);
    }

    /* check to see if indexed, if it is, return 
    if(AlnMgrCheckAlignForParent(salp) > 0) 
        ErrorReturn(SEV_ERROR, "ViewMgr_Attach", -1);*/
    
    /* add a message func.  use AppProperty hack to pass values */
    GatherSpecificProcLaunch(0, "ViewMgr Register", OMPROC_VIEW,
        FALSE, entityID, itemID, OBJ_SEQALIGN);

    pInfo = MemNew(sizeof(ViewMgr_AlignInfo));
    if(pInfo == NULL) ErrorReturn(SEV_ERROR, "ViewMgr_Attach", -1);

    pInfo->Neat = Neat;
    pInfo->Intersect = Intersect;
    pInfo->pBegin = SeqAlignListDup(salp);
    ViewMgr_Massage(pInfo);

    AlnMgrCopyIndexedParentIntoSap(pInfo->pTarget, salp);
    
    pInfo->pView = salp;
    pInfo->entityID = entityID;
    
    ValNodeAddPointer(&pGlobal->pAlignList, 0, pInfo);

    return 1;
}


/*****************************************************************************

Function: ViewMgr_Add

Purpose: Add a seqalign to a seqalign already in the viewmgr
  
Parameters: salp, the seqalign to be added to.  Must be attached to viewmgr
            salpAdd, the seqalign to be added.  should not be indexed.

Returns: 1 on success

*****************************************************************************/

NLM_EXTERN Int4 ViewMgr_Add(SeqAlign *salp, SeqAlign *salpAdd)
{
    ViewMgr_AlignInfo *pInfo;
    SeqAlign *pSeqAlign;

    if(salp == NULL || salpAdd == NULL) 
        ErrorReturn(SEV_ERROR, "ViewMgr_Add", -1);
    pInfo = ViewMgr_GetInfo(salp);
    if(pInfo == NULL) ErrorReturn(SEV_ERROR, "ViewMgr_Add", -1);

    /* check indexed state */
/*    if (AlnMgrCheckAlignForParent(salp) < 0 ||
        AlnMgrCheckAlignForParent(salpAdd) >= 0)
        ErrorReturn(SEV_ERROR, "ViewMgr_Add", -1);*/

    for(pSeqAlign = pInfo->pBegin; pSeqAlign->next != NULL;
        pSeqAlign = pSeqAlign->next);

    pSeqAlign->next = salpAdd;
    
    return ViewMgr_Update(salp);
}


/*****************************************************************************

Function: ViewMgr_SetHidden

Purpose: Set a row to contribute / not contribute to the seqalign
  
Parameters: salp, the seqalign to be shown/hid
            Row, the row to show or hide
            Hidden, 

Returns: 1 on success, 0 on setup failure, -1 if the requested action can't
         be done.

*****************************************************************************/

NLM_EXTERN Int4 ViewMgr_SetHidden(SeqAlign *salp, Boolean Hidden, Int4 Row)
{
    ViewMgr_AlignInfo *pInfo;
    ValNode *pvn, *pvnPrev;

    pInfo = ViewMgr_GetInfo(salp);
    if(pInfo == NULL) return 0;
    /* check to see if indexed */
    if (AlnMgrCheckAlignForParent(salp) < 0) return 0;
    if(pInfo->Intersect && !pInfo->Neat) return 0;  /* can't handle this case */
    if (Hidden) {
        /*	if(AlnMgrGetNumRows(pInfo->pBegin) - ValNodeLen(pInfo->pHiddenRows) < 3) return -1;  not indexed */
        for(pvn = pInfo->pHiddenRows; pvn != NULL; pvn = pvn->next)
            if(pvn->data.intvalue == Row) break;
        if(pvn == NULL) {
            ValNodeAddInt(&pInfo->pHiddenRows, (Nlm_Int2)Row, Row);
            return 1;
            }
        else return -1;
    }
    else {
        for(pvnPrev = pvn = pInfo->pHiddenRows; pvn != NULL; pvn = pvn->next) {
            if(pvn->data.intvalue == Row) {
                if(pvnPrev == pvn) pInfo->pHiddenRows = pvn->next;
                else pvnPrev->next = pvn->next;
                MemFree(pvn);
                return 1;
            }
            else pvnPrev = pvn;
        }
        return -1;
    }
    
    return 0;
}


NLM_EXTERN Int4 ViewMgr_Update(SeqAlign *salp) {
  ViewMgr_Update2(salp, FALSE);
}


/*****************************************************************************

Function: ViewMgr_Update

Purpose: Copies the target into the viewed seqalign, performs all the 
        transformations, then sends an update message
  
Parameters: salp, the seqalign to work on

Returns: 1 on success

*****************************************************************************/
NLM_EXTERN Int4 ViewMgr_Update2(SeqAlign *salp, Boolean Fast)
{
    ViewMgr_AlignInfo *pInfo;
    Uint2 itemID;
    DDVUpdateMSG dum;

    pInfo = ViewMgr_GetInfo(salp);
    if(pInfo == NULL) return 0;

    ViewMgr_Massage(pInfo);

    AlnMgrCopyIndexedParentIntoSap(pInfo->pTarget, salp);

    if (!Fast) {
      itemID =
          GetItemIDGivenPointer(pInfo->entityID, OBJ_SEQALIGN, (void *) salp);

      /* first tell everyone to recolor the entity */
      MemSet(&dum, 0, sizeof(DDVUpdateMSG));
      dum.data = NULL;
      dum.type = UPDATE_TYPE_RECOLOR;

      ObjMgrSendProcMsg(OM_MSG_UPDATE, pInfo->entityID, itemID, OBJ_SEQALIGN,
				  0, 0, (Pointer)&dum);

      /* then redraw the entity */
      MemSet(&dum, 0, sizeof(DDVUpdateMSG));
      dum.data = NULL;
      dum.type = UPDATE_TYPE_VIEWMGR;

      ObjMgrSendProcMsg(OM_MSG_UPDATE, pInfo->entityID, itemID, OBJ_SEQALIGN,
				  0, 0, (Pointer)&dum);
    }

    return 1;
}


/*****************************************************************************

Function: ViewMgr_MsgFunc

Purpose: this listens to the entity the seqalign sits in.  If there is a
         request that the object be deleted and the viewmgr has the last
         messagefunc, the object is deleted.
  
*****************************************************************************/

static Int2 LIBCALLBACK ViewMgr_MsgFunc(OMMsgStructPtr ommsp)
{
    OMUserDataPtr omudp = NULL;
    Int4 Count = 0;
    OMMessageFunc messagefunc = NULL;
    ViewMgr_Global *pGlobal;
    ValNode *pvn;
    ViewMgr_AlignInfo *pInfo;

    omudp = (OMUserDataPtr) (ommsp->omuserdata);

    switch (ommsp->message) {
    case OM_MSG_UPDATE:
        break;

    case OM_MSG_DEL:
        /* kill the object if there are no more message funcs */
        for(;omudp != NULL; omudp = omudp->next) {
            if(omudp->messagefunc != NULL) {
                Count++;
                messagefunc = omudp->messagefunc;
            }
        }
        if(Count == 1 && messagefunc == ViewMgr_MsgFunc) {
            ObjMgrFreeUserData(ommsp->entityID, 0, 0, 0);  /* deadlock? */
            pGlobal = GetAppProperty("ViewMgr");
            if(pGlobal != NULL) {
                for(pvn = pGlobal->pAlignList; pvn != NULL; pvn = pvn->next) {
                    pInfo = (ViewMgr_AlignInfo *)pvn->data.ptrvalue;
                    if(pInfo == NULL) continue;
                    if(pInfo->entityID == ommsp->entityID) {
                        ViewMgr_RemoveSA(pInfo->pView);
                        break;
                    }
                }
                if(pGlobal->pAlignList == NULL) {
                    ViewMgr_Delete(pGlobal);
                    RemoveAppProperty("ViewMgr");
                }
            }
        }
        return OM_MSG_RET_OK;
        break;

    default:
        break;      

    }
return OM_MSG_RET_OK;
}


/*****************************************************************************

Function: ViewMgr_RegisterFunc

Purpose: Adds the ViewMgr message function to the seqalign
  
*****************************************************************************/

NLM_EXTERN Int2 LIBCALLBACK ViewMgr_RegisterFunc(Pointer data)
{
    OMProcControlPtr ompcp = NULL;
    OMUserDataPtr omudp = NULL;
    ViewMgr_Global *pGlobal;

    ompcp = (OMProcControlPtr) data;
    if (ompcp == NULL) {
        Message(MSG_ERROR, "ViewMgr: Data Null 1");
        return OM_MSG_RET_ERROR;
    }
    
    if(ompcp->proc == NULL) {
        Message(MSG_ERROR, "ViewMgr: Data Null 2");
        return OM_MSG_RET_ERROR;
    }

    switch (ompcp->input_itemtype) {
    case OBJ_SEQALIGN:
        break;
    default:
        return OM_MSG_RET_ERROR;
    }

    if (ompcp->input_data == NULL) {
        Message(MSG_ERROR, "ViewMgr: Null Data 3");
        return OM_MSG_RET_ERROR;
    }
    pGlobal = GetAppProperty("ViewMgr");
    if(pGlobal == NULL) return OM_MSG_RET_ERROR;

    pGlobal->procid = ompcp->proc->procid;
    omudp = ObjMgrAddUserData(ompcp->input_entityID, ompcp->proc->procid,
                              OMPROC_EDIT, pGlobal->userkey);
    if (omudp != NULL) {
        omudp->messagefunc = ViewMgr_MsgFunc;
    }

    return OM_MSG_RET_OK;
}

