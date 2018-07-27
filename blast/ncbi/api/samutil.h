/*   $Id: samutil.h,v 1.59 2001/01/24 23:04:01 hurwitz Exp $
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
* File Name:  $Id: samutil.h,v 1.59 2001/01/24 23:04:01 hurwitz Exp $
*
* Author:  Lewis Geer
*
* Version Creation Date:   8/12/99
*
* $Revision: 1.59 $
*
* File Description: Utility functions for AlignIds and SeqAlignLocs
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: samutil.h,v $
* Revision 1.59  2001/01/24 23:04:01  hurwitz
* added a couple utility functions
*
* Revision 1.58  2000/11/01 20:50:22  hurwitz
* made functions for get block starts and stops from populated paraGs
*
* Revision 1.57  2000/10/05 21:27:42  hurwitz
* bug fix for making ruler, added functions to get bioseq start and len of each aligned block
*
* Revision 1.56  2000/09/08 21:47:57  hurwitz
* added DDE_GetNumResidues function
*
* Revision 1.55  2000/08/29 18:01:38  lewisg
* fix dde launch logic
*
* Revision 1.54  2000/08/25 22:10:33  hurwitz
* added utility function
*
* Revision 1.53  2000/08/07 23:02:39  hurwitz
* when merging panels fails, scroll to spot where alignments differ
*
* Revision 1.52  2000/08/03 17:12:38  hurwitz
* added functions to check if alignments are mergeable
*
* Revision 1.51  2000/07/25 20:30:22  hurwitz
* bug fixes: panel update when file is closed, double-click launches UDV from DDE, seqName agrees in status line and left col
*
* Revision 1.50  2000/07/17 17:49:12  hurwitz
* fixed bug.  when copying ParaG's need to copy all rows so edits can be saved properly
*
* Revision 1.49  2000/07/05 18:42:17  hurwitz
* added split block function to DDV
*
* Revision 1.48  2000/06/30 22:31:52  hurwitz
* added merge block function for DDV
*
* Revision 1.47  2000/06/15 20:26:06  hurwitz
* added left/right/center justify for DDE
*
* Revision 1.46  2000/06/07 19:09:36  hurwitz
* made DDE_ReMakeRuler work with linked list of ParaGs
*
* Revision 1.45  2000/05/25 21:40:42  hurwitz
* rows hidden in DDV are hidden in DDE, can save edits when rows are hidden in DDE
*
* Revision 1.44  2000/05/19 13:48:32  hurwitz
* made a version of DDE that doesn't allow aligned gaps, changed wording for adding new rows
*
* Revision 1.43  2000/05/16 19:43:02  hurwitz
* grey out create block, delete block, undo, and redo as needed
*
* Revision 1.42  2000/05/15 23:39:33  lewisg
* shred cblast, add menu items for gapped/ungapped, fix pdbheaders
*
* Revision 1.41  2000/05/12 21:18:14  hurwitz
* added window asking if user wants to save unsaved edits for dde
*
* Revision 1.40  2000/05/05 20:24:14  hurwitz
* some bug fixes, also redraw proper block in DDE after a save operation that causes a merge of 2 blocks
*
* Revision 1.39  2000/05/04 22:43:39  hurwitz
* don't launch DDE on top of DDV, change some wording, redraw DDE after save to AlnMgr
*
* Revision 1.38  2000/05/02 19:50:37  hurwitz
* fixed some bugs with launching DDE from DDV, added new alnMgr fn for positioning DDE on proper column
*
* Revision 1.37  2000/04/26 21:53:22  hurwitz
* added save function to tell AlnMgr about edits made in DDE
*
* Revision 1.36  2000/04/21 23:00:51  hurwitz
* can launch DDE from DDV
*
* Revision 1.35  2000/04/13 18:57:03  hurwitz
* for DDE: many bug fixes, also get rid of columns that are all unaligned gaps
*
* Revision 1.34  2000/04/10 21:41:25  lewisg
* move alignment menus into ddv, udv from cn3d
*
* Revision 1.33  2000/04/07 16:21:09  hurwitz
* made delete block faster, added delete block to edit menu
*
* Revision 1.32  2000/04/05 20:52:35  hurwitz
* added GUI control for shifting left and right alignment boundaries
*
* Revision 1.31  2000/04/03 22:26:31  hurwitz
* can now shift a row with click and drag
*
* Revision 1.30  2000/03/29 20:02:48  hurwitz
* keep track of master during move row operations
*
* Revision 1.29  2000/03/28 21:03:14  hurwitz
* added gui control to re-order rows
*
* Revision 1.28  2000/03/25 00:22:10  hurwitz
* put DDE_StackPtr in DDV_Main, add to stack inside DDE api's, added insert char, delete char, home and end keyboard control
*
* Revision 1.27  2000/03/23 00:00:00  hurwitz
* DDE api's are called with stack now
*
* Revision 1.26  2000/03/20 22:22:48  hurwitz
* added more checks to verify subroutine, 1 bug fix
*
* Revision 1.25  2000/03/16 15:51:32  hurwitz
* added function to create an aligned block
*
* Revision 1.24  2000/03/14 22:08:21  hurwitz
* undo and redo working properly, restore-original function added
*
* Revision 1.23  2000/03/10 23:01:43  hurwitz
* added undo and redo functions, first pass
*
* Revision 1.22  2000/03/09 22:28:40  hurwitz
* added shift block and delete block, a bug fix too
*
* Revision 1.21  2000/03/08 22:02:07  hurwitz
* added verify function, debugging, handle align_start != 0
*
* Revision 1.20  2000/03/06 22:45:58  hurwitz
* can shift right boundary of an aligned block left and right, DDVRuler updates added
*
* Revision 1.19  2000/03/01 22:49:40  lewisg
* import bioseq, neatlyindex, get rid of dead code
*
* Revision 1.18  2000/02/29 21:13:06  hurwitz
* added low level functions for shifting left and right the left alignment boundary
*
* Revision 1.17  2000/02/28 16:28:39  hurwitz
* added functions for deleting an aligned gap
*
* Revision 1.16  2000/02/24 23:37:00  hurwitz
* added ability to insert gaps
*
* Revision 1.15  2000/02/18 16:06:22  hurwitz
* for editing multiple sequence alignments: shift row right now working
*
* Revision 1.14  2000/02/15 22:40:57  lewisg
* add ability to launch udv so that it colors by row, fixes to colormgr, track rows from viewmgr, fix visual c projects
*
* Revision 1.13  2000/02/15 17:44:12  hurwitz
* reverted to 1.11
*
* Revision 1.11  2000/02/14 23:09:08  hurwitz
* got rid of calls to DDV_RulerDescrNew() because of library conflicts
*
* Revision 1.10  2000/02/14 20:58:57  hurwitz
* added functions for editing multiple sequence alignments: hide/show row, move row, shift row left
*
* Revision 1.9  2000/01/24 20:54:35  vakatov
* SAM_ViewString::  made #define to fix for the DLL build on PC
*
* Revision 1.8  1999/12/11 01:30:34  lewisg
* fix bugs with sharing colors between ddv and cn3d
*
* Revision 1.7  1999/12/03 23:17:23  lewisg
* Patrick's new global update msg, argument passing when launching ddv, experimental editing
*
* Revision 1.6  1999/11/24 21:24:30  vakatov
* Fixed for the C++ and/or MSVC DLL compilation
*
* Revision 1.5  1999/11/15 18:30:07  lewisg
* get rid of extra redraws when selecting
*
* Revision 1.4  1999/10/05 23:18:16  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 1.3  1999/09/27 17:49:12  lewisg
* fix denseseg constructor, bug in valnode loops, add SAM_ValNodeByPosition
*
* Revision 1.2  1999/09/21 19:38:59  lewisg
* update broken declarations
*
* Revision 1.1  1999/09/21 18:09:14  lewisg
* binary search added to color manager, various bug fixes, etc.
*
* Revision 1.3  1999/09/03 14:01:40  lewisg
* use faster seqid compare SAM_CompareID
*
* Revision 1.2  1999/09/01 23:02:59  lewisg
* binary search in color functions
*
* Revision 1.1  1999/08/13 22:08:16  lewisg
* color manager updated to use alignment coords
*
*
* ==========================================================================
*/


#ifndef SAMUTIL_H
#define SAMUTIL_H

#include <ncbistd.h>
#include <objall.h>
#include <sequtil.h>
#include <objgen.h>
#include <objalignloc.h>
#include <pgppop.h>    /*-- dih*/

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define SAM_SIPBUF 80

#define SAM_NOLAP 0x1
#define SAM_NOLAPFRONT 0x3
#define SAM_NOLAPBACK 0x5
#define SAM_TOTALLAP 0x8
#define SAM_FRONTLAP 0x10
#define SAM_BACKLAP 0x20

#define ThrowError if(!Nlm_ErrSetContext(THIS_MODULE,THIS_FILE,__LINE__,DBFLAG,0,0,0)) goto error
#define ErrorReturn(error, string, retval) {Nlm_ErrSetContext(THIS_MODULE,THIS_FILE,__LINE__,DBFLAG,0,0,0); \
    Nlm_ErrPostEx((error), 0, 0, (string)); Nlm_ErrShow(); return(retval);}
#define ErrorReturnVoid(error, string) {Nlm_ErrSetContext(THIS_MODULE,THIS_FILE,__LINE__,DBFLAG,0,0,0); \
    Nlm_ErrPostEx((error), 0, 0, (string)); Nlm_ErrShow(); return;}
    
typedef Boolean (*StartNetworkProc) (Boolean UseNetwork);
typedef SeqEntryPtr (*SAM_SeqFetchProc) (Int4 uid, Int2 retcode);

/* same as Vibrant RecT */
typedef  struct  _SAM_RecT {
  Nlm_Int2  left;
  Nlm_Int2  top;
  Nlm_Int2  right;
  Nlm_Int2  bottom;
} SAM_RecT, PNTR SAM_RecTPtr;

/* defines so that views can be identified */
#define SAMVIEWSELF 0
#define SAMVIEWCN3D 1
#define SAMVIEWDDV 2
#define SAMVIEWUDV 3
#define SAMVIEWSEQUIN 4
#define SAMVIEWNENTREZ 5
#define SAMVIEWDDE 6
#define SAMVIEWCN3DDDV 7

#define SAM_ViewString "Viewer Global"

typedef  struct  _SAM_ViewerGlobal {
  SAM_RecT Rect;  /* where the master viewer wants the called viewer to go */
  SAM_SeqFetchProc  FetchProc;  /* the fetch function the master has set */
  Int4 MasterViewer;  /* the name of the master viewer */
  DDV_ColorGlobal *pCGlobal;
  Int4 Row;
  StartNetworkProc NetStartProc;
  void *BlastFile;
  void *BlastNet;
  void *BlastFileGap;
  void *BlastNetGap;
  void *BlastMany;
} SAM_ViewGlobal, PNTR SAM_ViewGlobalPtr;


/*----------------------------------------------------------------------------
*  This is where I'm going to put my code for now...   Dave Hurwitz
*                                                      1/21/00
*
*  These are the structutes and functions needed for the multiple
*  alignment editor.  Perhaps it will be called DDE (as opposed to
*  DDV).  (2-d editor, rather than 2-d viewer).
*
*  These functions will make use of a MsaParaGPopList, which in
*  turn points to a list of ParaG's.  Each ParaG describes a single
*  row, or sequence.  Each MsaParaGPopList describes all the sequences
*  that are being edited.
*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
*  this structure has info for multiple sequence alignment and editing.
*---------------------------------------------------------------------------*/
typedef struct DDE_info {
  MsaParaGPopListPtr  pPopList;      /* the paragraphs as they're edited */
  Boolean*            pVisible;      /* whether each original row is visible */
  Int4*               pRowOrder;     /* order original (incl invisible) rows are displayed */
  Int4*               pRowOrderLUT;  /* look-up-table: on-screen -> original rows */
  Int4                TotalNumRows;  /* row count -- all rows, visible and invisible */
} DDE_Info, PNTR DDE_InfoPtr;

/*----------------------------------------------------------------------------
*  this structure has info for keeping a history of edit changes.
*---------------------------------------------------------------------------*/
#define DDE_STACK_SIZE  300          /* stack size -- arbitrary */
typedef struct DDE_stack {
  DDE_InfoPtr*  pArray;              /* array of DDE_InfoPtr's */
  DDE_InfoPtr   pOrig;               /* pointer to original structure */
  DDE_InfoPtr   pEdit;               /* pointer to structure being edited */
  Int4          NumInStack;          /* number of items in the stack */
  Int4          NumFromStart;        /* number of items from start to StackIndex */
  Int4          StackIndex;          /* next available slot on stack */
  Int4          StackMin;            /* can back up until here */
  Int4          StackMax;            /* can advance until here */
  Int4          FromRow;             /* row that's moving */
  Int4          SaveRow;             /* last drawn underline */
  Int4          FromCol;             /* for calculating num chars shifting left/right */
  Int4          SaveCol;             /* for last drawn dotted box */
  Int4          BlockIndex;          /* record of block that's being adjusted */
  Int4          LeftBoundary;        /* record of whether Left or Right boundary shift */
  Int4          LaunchBlock;         /* record of block editor was launched on */
  Boolean       IsUnAligned;         /* record of whether block editor was launched on is aligned */
  Int4          NumBlocks;           /* num blocks at last save */
  Boolean       SomethingToSave;     /* whether there are edits to save */
} DDE_Stack, PNTR DDE_StackPtr;

/*=====================================================================
*  these are the public member functions of DDE_Info.
*======================================================================*/
NLM_EXTERN DDE_StackPtr DDE_NewStack(MsaParaGPopListPtr pPopList);
NLM_EXTERN DDE_StackPtr DDE_FreeStack(DDE_StackPtr pStack);
NLM_EXTERN Boolean DDE_RestoreRowOrder(DDE_StackPtr pStack, Boolean Save);
NLM_EXTERN Boolean DDE_HideRow(DDE_StackPtr pStack, Int4 Row, Boolean Save);
NLM_EXTERN Boolean DDE_HideAllRows(DDE_StackPtr pStack, Boolean Save);
NLM_EXTERN Boolean DDE_HideNewRow(DDE_StackPtr pStack, Int4 Row, Boolean Save);
NLM_EXTERN Boolean DDE_ShowRow(DDE_StackPtr pStack, Int4 Row, Boolean Save);
NLM_EXTERN Boolean DDE_ShowAllRows(DDE_StackPtr pStack, Boolean Save);
NLM_EXTERN Boolean DDE_ShowNewRow(DDE_StackPtr pStack, Int4 Row, Boolean Save);
NLM_EXTERN Boolean DDE_MoveRow(DDE_StackPtr pStack, Int4 From, Int4 To, Boolean Save);
NLM_EXTERN Boolean DDE_ShiftRow(DDE_StackPtr pStack, Int4 Row, Int4 NumChars, Boolean Save,
                                Boolean ShiftBack);
NLM_EXTERN Boolean DDE_InsertGap(DDE_StackPtr pStack, Int4 Row, Int4 Pos, Boolean Save);
NLM_EXTERN Boolean DDE_RemoveGap(DDE_StackPtr pStack, Int4 Row, Int4 Pos, Boolean Save);
NLM_EXTERN Boolean DDE_ShiftLeftBoundary(DDE_StackPtr pStack, Int4 BlockIndex, Int4 NumChars, Boolean Save);
NLM_EXTERN Boolean DDE_ShiftRightBoundary(DDE_StackPtr pStack, Int4 BlockIndex, Int4 NumChars, Boolean Save);
NLM_EXTERN void DDE_ShiftBlock(DDE_StackPtr pStack, Int4 BlockIndex, Int4 NumChars, Boolean Save);
NLM_EXTERN void DDE_CreateBlock(DDE_StackPtr pStack, Int4 Left, Int4 Right, Boolean Save);
NLM_EXTERN Int4    DDE_DeleteBlock(DDE_StackPtr pStack, Int4 BlockIndex, Boolean Save);
NLM_EXTERN Boolean DDE_Select(DDE_StackPtr pStack, Int4 Row, Int4 Left, Int4 Right, Boolean Save);
NLM_EXTERN Boolean DDE_DeSelect(DDE_StackPtr pStack, Int4 Row, Int4 Left, Int4 Right, Boolean Save);
NLM_EXTERN Boolean DDE_SelectCol(DDE_StackPtr pStack, Int4 ColStart, Int4 ColStop, Boolean Save);
NLM_EXTERN Boolean DDE_DeSelectCol(DDE_StackPtr pStack, Int4 ColStart, Int4 ColStop, Boolean Save);
NLM_EXTERN Boolean DDE_Add(DDE_StackPtr pStack);
NLM_EXTERN Boolean DDE_Prev(DDE_StackPtr pStack);
NLM_EXTERN Boolean DDE_Next(DDE_StackPtr pStack);
NLM_EXTERN Boolean DDE_AtStartOfStack(DDE_StackPtr pStack);
NLM_EXTERN Boolean DDE_AtEndOfStack(DDE_StackPtr pStack);
NLM_EXTERN void DDE_GetOriginal(DDE_StackPtr pStack, Boolean Save);
NLM_EXTERN Int4 DDE_GetIndexOfMaster(DDE_StackPtr pStack);
NLM_EXTERN Int4 DDE_GetAlignStart(DDE_InfoPtr pEditInfo, Int4 BlockIndex);
NLM_EXTERN Int4 DDE_GetAlignStart2(MsaParaGPopListPtr pPopList, Int4 BlockIndex);
NLM_EXTERN Int4 DDE_GetAlignStop(DDE_InfoPtr pEditInfo, Int4 BlockIndex);
NLM_EXTERN Int4 DDE_GetAlignStop2(MsaParaGPopListPtr pPopList, Int4 BlockIndex);
NLM_EXTERN Int4 DDE_GetBlockWidth(DDE_InfoPtr pEditInfo, Int4 BlockIndex);
NLM_EXTERN Int4 DDE_GetNumBlocks(DDE_InfoPtr pEditInfo);
NLM_EXTERN Int4 DDE_GetNumBlocks2(MsaParaGPopListPtr pPopList);
NLM_EXTERN Boolean DDE_IsColValid(MsaParaGPopListPtr pPopList, Int4 Col,
                                  Int4 PNTR pBlockIndex, Boolean PNTR pUnAligned,
                                  Int4 PNTR pOffset, Int4 PNTR pWidth);
NLM_EXTERN Int4    DDE_GetNumSegmentsInBlock(DDE_InfoPtr pEditInfo, Int4 BlockIndex);
NLM_EXTERN Boolean DDE_CreateArraysForDenseSeg(DDE_InfoPtr pEditInfo, Int4 BlockIndex,
                                               Int4 PNTR pStarts, Int4 PNTR pLens);
NLM_EXTERN Int4    DDE_GetFirstDisplayCoord(MsaParaGPopListPtr pPopList);
NLM_EXTERN Boolean DDE_IsLeftAlignedGapInRows(DDE_InfoPtr pEditInfo);
NLM_EXTERN Boolean DDE_IsRightAlignedGapInRows(DDE_InfoPtr pEditInfo);
NLM_EXTERN void    DDE_RemoveAlignedGapsFromEnds(DDE_InfoPtr pEditInfo);
NLM_EXTERN void    DDE_RemoveAlignedGapsFromEndOfRow(DDE_StackPtr pStack, Int4 Row, Boolean RightSide);
NLM_EXTERN Boolean DDE_LeftJustify(DDE_StackPtr pStack, Boolean Save);
NLM_EXTERN Boolean DDE_RightJustify(DDE_StackPtr pStack, Boolean Save);
NLM_EXTERN Boolean DDE_CenterJustify(DDE_StackPtr pStack, Boolean Save);
NLM_EXTERN Boolean DDE_FirstColumnIsAligned(DDE_InfoPtr pEditInfo);
NLM_EXTERN Boolean DDE_LastColumnIsAligned(DDE_InfoPtr pEditInfo);
NLM_EXTERN ValNodePtr  DDE_GetTxtListPtr(DDE_InfoPtr pEditInfo, Int4 Row);
NLM_EXTERN ValNodePtr  DDE_GetMsaTxtNode(ValNodePtr head, Int4 DispCoord, Int4 PNTR pOffset);
NLM_EXTERN ValNodePtr  DDE_GetMsaTxtNode2(ValNodePtr pg_head, Int4 DispCoord, Int4 PNTR pOffset);
NLM_EXTERN ValNodePtr  DDE_GetMsaTxtNodeGivenBioseqCoord(MsaParaGPopListPtr pPopList,
                                                         Int4 BioseqCoord, Int4 Row);
NLM_EXTERN Int4 DDE_GetStart(MsaParaGPopListPtr pPopList, Int4 BlockIndex, Int4 Row);
NLM_EXTERN Int4 DDE_GetLen(MsaParaGPopListPtr pPopList, Int4 BlockIndex);

/*=====================================================================
*  here are some helper functions
*======================================================================*/
NLM_EXTERN DDE_InfoPtr  DDE_New(MsaParaGPopListPtr pPopList, Int4 TotalNumRows);
NLM_EXTERN DDE_InfoPtr  DDE_Free(DDE_InfoPtr pEditInfo, Int4 TotalNumRows);
NLM_EXTERN DDE_InfoPtr  DDE_Copy(DDE_InfoPtr pEditInfo, Int4 TotalNumRows);
NLM_EXTERN MsaParaGPopListPtr DDE_PopListNew(MsaParaGPopListPtr pPopList, Int4 TotalNumRows);
NLM_EXTERN MsaParaGPopListPtr DDE_PopListFree(MsaParaGPopListPtr pPopList, Int4 TotalNumRows);
NLM_EXTERN ParaGPtr    DDE_ParaGNew(ParaGPtr pParaG);
NLM_EXTERN ParaGPtr    DDE_ParaGFree(ParaGPtr pParaG);
NLM_EXTERN Int4        DDE_GetNumVisibleRows(DDE_InfoPtr pEditInfo);
NLM_EXTERN Int4        DDE_GetDisplayRow(DDE_InfoPtr pEditInfo, Int4 Row);
NLM_EXTERN Int4        DDE_GetInsertRow(DDE_InfoPtr pEditInfo, Int4 Row);
NLM_EXTERN Boolean     DDE_IsBefore(DDE_InfoPtr pEditInfo, Int4 Row1, Int4 Row2);
NLM_EXTERN ParaGPtr    DDE_GetParaGPtr(DDE_InfoPtr pEditInfo, Int4 Row);
NLM_EXTERN ParaGPtr    DDE_GetParaGPtr2(MsaParaGPopListPtr pPopList, Int4 Row);
NLM_EXTERN ValNodePtr  DDE_GetTxtListPtr2(MsaParaGPopListPtr pPopList, Int4 Row);
NLM_EXTERN void        DDE_SetTxtListPtr(DDE_InfoPtr pEditInfo, Int4 Row, ValNodePtr vnp);
NLM_EXTERN ValNodePtr  DDE_AddMsaTxtNode(ValNodePtr ptxtList, Int4 from, Int4 to,
                            Boolean IsGap, Boolean IsUnAligned, Boolean Before);
NLM_EXTERN ValNodePtr  DDE_AddRulerDescrNode(ValNodePtr pRulerDescr, Int4 DispStart,
                            Int4 DispStop, Int4 AlignStart, Boolean IsUnAligned, Boolean Before);
NLM_EXTERN void        DDE_IncDisplayCoords(ValNodePtr ptxtList);
NLM_EXTERN void        DDE_DecDisplayCoords(ValNodePtr ptxtList);
NLM_EXTERN ValNodePtr  DDE_AddAGap(DDE_InfoPtr pEditInfo, Int4 Row,
                            ValNodePtr ptxtList, Boolean Before, Boolean ForceUnAligned);
NLM_EXTERN ValNodePtr  DDE_AddGapAndSplitNode(ValNodePtr pInsertNode, Int4 Offset, Int4 Pos);
NLM_EXTERN ValNodePtr  DDE_RemoveAGap(DDE_InfoPtr pEditInfo, Int4 Row, ValNodePtr pRemoveNode,
                                      Boolean* pNodeDeleted);
NLM_EXTERN Int4        DDE_GetGapIndex(ValNodePtr ptxtList, ValNodePtr pMidttxtList,
                            Boolean Before);
NLM_EXTERN ValNodePtr  DDE_GetPrevVNP(ValNodePtr head, ValNodePtr curr);
NLM_EXTERN Boolean     DDE_ShiftRowLeft1(DDE_InfoPtr pEditInfo, Int4 Row, ValNodePtr ptxtList,
                                         Boolean OkToAddGaps);
NLM_EXTERN Boolean     DDE_ShiftRowRight1(DDE_InfoPtr pEditInfo, Int4 Row, ValNodePtr ptxtList,
                                          ValNodePtr prev_vnp, Boolean InsertLeftGap);
NLM_EXTERN void        DDE_AddGapToStartOfAllRows(DDE_InfoPtr pEditInfo, Boolean ReMakeRuler);
NLM_EXTERN void        DDE_AddGapToEndOfAllRows(DDE_InfoPtr pEditInfo, Boolean ReMakeRuler);
NLM_EXTERN Boolean     DDE_RemoveGapFromStartOfAllRows(DDE_InfoPtr pEditInfo, Boolean ReMakeRuler);
NLM_EXTERN Boolean     DDE_RemoveGapFromEndOfAllRows(DDE_InfoPtr pEditInfo, Boolean ReMakeRuler);
NLM_EXTERN Boolean     DDE_CleanEnds(DDE_InfoPtr);
NLM_EXTERN Boolean     DDE_IsLeftAlignedGap(ValNodePtr ptxtList);
NLM_EXTERN Boolean     DDE_IsLeftAlignedGapList(ValNodePtr ptxtList, Boolean DeleteIt);
NLM_EXTERN Boolean     DDE_IsRightAlignedGap(ValNodePtr ptxtList);
NLM_EXTERN Boolean     DDE_IsTerminalRightAlignedGap(ValNodePtr ptxtList);
NLM_EXTERN Boolean     DDE_IsRightAlignedGapList(ValNodePtr PNTR pptxtList, Boolean DeleteIt);
NLM_EXTERN Boolean     DDE_LeftMerge(ValNodePtr ptxtList);
NLM_EXTERN Boolean     DDE_LeftAddNode(ValNodePtr ptxtList, Boolean PNTR pNodeDeletedToo);
NLM_EXTERN void        DDE_LeftMergeAndAddNodeList(ValNodePtr ptxtList, Boolean AfterGap);
NLM_EXTERN Boolean     DDE_RightMerge(ValNodePtr PNTR pptxtList, Boolean PNTR pHeadChanged);
NLM_EXTERN Boolean     DDE_RightAddNode(ValNodePtr PNTR pptxtList, Boolean PNTR pHeadChanged);
NLM_EXTERN Boolean     DDE_RightMergeAndAddNodeList(ValNodePtr PNTR pptxtList);
NLM_EXTERN void        DDE_ShiftLeftList(DDE_InfoPtr pEditInfo, Int4 Row, ValNodePtr ptxtList,
                                         Boolean AfterGap, Boolean LastCharIsUnAligned);
NLM_EXTERN Boolean     DDE_ShiftRightList(ValNodePtr PNTR pptxtList, ValNodePtr StopNode,
                                          Boolean FirstCharIsUnAligned, Boolean InsertLeftGap,
                                          Boolean OkToShrinkLastNode);
NLM_EXTERN ValNodePtr  DDE_GetLastVNP(ValNodePtr ptxtList);
NLM_EXTERN Int4        DDE_GetNumResidues(MsaParaGPopListPtr pPopList, Int4 Row);
NLM_EXTERN Boolean     DDE_IsStartOfAlignment(ValNodePtr ptxtList);
NLM_EXTERN Boolean     DDE_IsEndOfAlignment(ValNodePtr ptxtList);
NLM_EXTERN Boolean     DDE_ShiftLeftBoundaryLeft1(DDE_InfoPtr pEditInfo, Int4 BlockIndex);
NLM_EXTERN Boolean     DDE_ShiftLeftBoundaryRight1(DDE_InfoPtr pEditInfo, Int4 BlockIndex);
NLM_EXTERN Boolean     DDE_ShiftRightBoundaryLeft1(DDE_InfoPtr pEditInfo, Int4 BlockIndex);
NLM_EXTERN Boolean     DDE_ShiftRightBoundaryRight1(DDE_InfoPtr pEditInfo, Int4 BlockIndex);
NLM_EXTERN ValNodePtr  DDE_ReMakeRuler(MsaParaGPopListPtr pPopList, Boolean Replace, Int4 AlignStart);
NLM_EXTERN ValNodePtr  DDE_ReMakeRulerForRow(MsaParaGPopListPtr pPopList, Int4 Row, Int4 AlignStart);
NLM_EXTERN Boolean     DDE_AreIdenticalRulers(ValNodePtr pRuler1, ValNodePtr pRuler2);
NLM_EXTERN Boolean     DDE_AreIdenticalRulerDescrs(DDVRulerDescrPtr p1, DDVRulerDescrPtr p2);
NLM_EXTERN Boolean     DDE_AreSimilarRulers(ValNodePtr pRuler1, ValNodePtr pRuler2,
                                            Int4* pDispCoord1,  Int4* pDispCoord2);
NLM_EXTERN Boolean     DDE_AreSimilarRulerDescrs(DDVRulerDescrPtr p1, DDVRulerDescrPtr p2);
NLM_EXTERN Boolean     DDE_MergeNodes(ValNodePtr ptxtList);
NLM_EXTERN void        DDE_MergeNodesLists(DDE_InfoPtr pEditInfo);
NLM_EXTERN Int4        DDE_GetFirstAlignIndex(MsaParaGPopListPtr pPopList);
NLM_EXTERN void        DDE_Verify(DDE_InfoPtr pEditInfo);
NLM_EXTERN ValNodePtr  DDE_SplitNode(ValNodePtr ptxtList, Int4 Offset);
NLM_EXTERN void        DDE_SetTextStyle(MsaTxtDispPtr msap);
NLM_EXTERN Boolean     DDE_GetColStatusForRow(DDE_InfoPtr pEditInfo, Int4 Row, Int4 Col,
                                              Boolean PNTR pUnAligned, Boolean PNTR pGap);
NLM_EXTERN Boolean     DDE_GetGapStatusOfRows(DDE_InfoPtr pEditInfo, Boolean PNTR pGapArray,
                                              Int4 Col);
NLM_EXTERN Boolean     DDE_AreArraysSame(Boolean PNTR pArray1, Boolean PNTR pArray2, Int4 Size);
NLM_EXTERN Boolean     DDE_GetAlignIndices(DDE_InfoPtr pEditInfo, Int4 PNTR pIndices, Int4 Col);
NLM_EXTERN Boolean     DDE_AddIndicesToArray(Int4 PNTR pArray, Int4 PNTR pIndices,
                                             Int4 Size, Int4 ArrayIndex);
NLM_EXTERN Int4        DDE_GetNumLeadingUnAlignedGaps(DDE_InfoPtr pEditInfo, Int4 Row);
NLM_EXTERN Int4        DDE_GetNumTrailingUnAlignedGaps(DDE_InfoPtr pEditInfo, Int4 Row);

/*----------------------------------------------------------------------------
*  This is the end of my code -- Dave Hurwitz
*----------------------------------------------------------------------------*/

/*****************************************************************************

Function: SAM_ReplaceGI()

Purpose: Tries to replace all gi's in a seqalign with the best accession ID's
  
Parameters: salp, the seqalign to be worked over.

Returns: This function only works on a non-indexed list of seqaligns

*****************************************************************************/

NLM_EXTERN void SAM_ReplaceGI(SeqAlign *salp);

/*****************************************************************************

Function: SAM_ExtractSips()

Purpose: Return a ValNode list containing a SeqId for each Bioseq
         contained in a SeqEntry.
  
Parameters: sep, pointer to the SeqEntry to explore

Returns: ValNode list of pointers to SeqId's.  Do NOT deallocate, these are
         not duplicates!

*****************************************************************************/

NLM_EXTERN ValNode * SAM_ExtractSips(SeqEntry *sep);


/*****************************************************************************

Function: SAM_MakeViewerFree()

Purpose: Set an object to use OM_OPT_FREE_IF_NO_VIEW flag.
  
Parameters: data, pointer to the object to be flagged.

Returns: 1 if OK, 0 otherwise.

*****************************************************************************/

NLM_EXTERN Int4 SAM_MakeViewerFree (void *data);


/*****************************************************************************

Function: SAM_MakeTemp()

Purpose: Make an object temporary loaded.
  
Parameters: data, pointer to the object to be flagged.

Returns: 1 if OK, 0 otherwise.

*****************************************************************************/

NLM_EXTERN Int4 SAM_MakeTemp (void *data);


/*****************************************************************************

Function: SAM_ValNodePut()

Purpose: Put a ValNode in a list of ValNodes by cardinal order.
  
Parameters: ppvnHead, pointer to head of ValNode list
            Num, the number of the ValNode to insert before.  begins with 0.
            pvnInsert, the valnode to insert

Returns: 1 if OK, 0 otherwise.

Notes: Hangs on loops that don't include the head.

*****************************************************************************/

NLM_EXTERN Int4 SAM_ValNodePut
(ValNode **ppvnHead, ValNode *pvnInsert, Int4 Num);


/*****************************************************************************

Function: SAM_ValNodeExtract()

Purpose: Takes a ValNode out of a list of ValNodes by cardinal order.
  
Parameters: ppvnHead, pointer to head of ValNode list
            Num, the number of the ValNode to extract (the 2nd, 3rd, etc.)

Returns: The extracted ValNode. NULL otherwise.

Notes: Hangs on loops that don't include the head.

*****************************************************************************/

NLM_EXTERN ValNode * SAM_ValNodeExtract(ValNode **ppvnHead, Int4 Num);

/*****************************************************************************

Function: SAM_ValNodeByPosition()

Purpose: Return pointer to ValNode in a list of ValNodes by cardinal order.
  
Parameters: ppvnHead, pointer to head of ValNode list
            Num, the number of the ValNode to get (the 2nd, 3rd, etc.).
                 starts at 0.

Returns: The ValNode at position NUM. NULL otherwise.

Notes: Hangs on loops that don't include the head.

*****************************************************************************/

NLM_EXTERN ValNode * SAM_ValNodeByPosition(ValNode **ppvnHead, Int4 Num);


/*****************************************************************************

Function: SAM_SeqAlignExtract()

Purpose: Takes a SeqAlign out of a list of SeqAligns.  The extracted SeqAlign
         is the first one pointer matches the passed SeqAlign pointer.
  
Parameters: psalpHead, pointer to head of SeqAlign list
            salpCheck, points to the SeqAlign to be extracted

Returns: The extracted SeqAlign. NULL otherwise

Notes: Hangs on loops that don't include the head.

*****************************************************************************/

NLM_EXTERN SeqAlign * SAM_SeqAlignExtract
(SeqAlign **psalpHead, SeqAlign *salpCheck);

/*****************************************************************************
*
*   Adds a SeqAlign newnode to the end of a SeqAlign chain started by head.
*   
*   If the head is NULL, makes the newnode the head.
*   Returns the head of the SeqAlign chain, otherwise returns NULL on error.
*
*****************************************************************************/

NLM_EXTERN SeqAlign * SAM_Add2SeqAlign(SeqAlign ** head, SeqAlign *newnode);

/*****************************************************************************
*
*   Frees a list of SeqId's.  Returns the remaining SeqId * if fails,
*   Otherwise NULL. 
*
*****************************************************************************/

NLM_EXTERN SeqId * SAM_FreeSeqIdSet(SeqId *sip);

/*****************************************************************************
*
*   Retrieves SeqIds from a set of seqlocs, duplicates the first SeqId, then
*   appends it to a list of SeqId's, which is returned.  
*
*   Returns NULL if the seqlocs are of any type other that SEQLOC_INT or the
*   SeqLoc doesn't contain a SeqId.
*
*****************************************************************************/

NLM_EXTERN SeqId * SAM_SeqIdFromSeqLoc(SeqLoc *slp, Int4 *NumSeqs);


/*****************************************************************************

Function: SAM_NewDenseSeg()

Purpose: Constructs a new DenseSeg, all arrays initialized to sizes based on
          NumSeqs and NumSegs

Parameters: NumSeqs, the number of sequences
            NumSegs, the number of Segments
            Strands, if TRUE, initialize the strands array.

Returns: The new DenseSeg. NULL otherwise.

*****************************************************************************/

NLM_EXTERN DenseSeg *SAM_NewDenseSeg
(Int4 NumSeqs, Int4 NumSegs, Boolean Strands);


/*****************************************************************************
*
*   Constructs a new SeqAlign of type type and segment type segtype.  Add segs
*   to the seg pointer and sets the dimension of the SeqAlign to dim.
*
*   Returns NULL on error
*
*****************************************************************************/

NLM_EXTERN SeqAlign *SAM_NewSeqAlign
(Uint1 type, Uint1 segtype, Pointer segs, Int2 dim);


/*****************************************************************************
*
*   Checks to see how a postion is inside or is in front of or in back of a
*   range.
*   If it is inside, return SAM_TOTALLAP
*   If it doesn't, return SAM_NOLAP & SAM_NOLAPFRONT if in front
*   If it doesn't and is in back return SAM_NOLAP & SAM_NOLAPBACK
*
*****************************************************************************/

NLM_EXTERN Int4 SAM_InRange(Int4 Position, Int4 From, Int4 To);

/*****************************************************************************
*
*   Checks to see if Range1 overlaps Range2.
*   If it does completely, return SAM_TOTALLAP
*   If it doesn't, return SAM_NOLAP & SAM_NOLAPFRONT if in front
*   If it doesn't and is in back return SAM_NOLAP & SAM_NOLAPBACK
*   If pRange1 overlaps the front of pRange2, return SAM_FRONTLAP
*   If pRange1 overlaps the rear of pRange2, return SAM_BACKLAP
*
*****************************************************************************/

NLM_EXTERN Int4 SAM_RangeOverlap(Int4 From1, Int4 To1, Int4 From2, Int4 To2);

/*****************************************************************************
*
*   Lexically compare two SeqId's.  Checks ALL sips on both chains.
*   Returns -1 if sip1 <  sip2
*            0 if sip1 == sip2
*            1 if sip1 >  sip2
*
*****************************************************************************/

NLM_EXTERN Int4 SAM_LexicalComp(SeqId *sip1, SeqId *sip2);

/*****************************************************************************
*
*   Orders two SeqId's for binary searches, etc.  DOES check the full
*   ValNode lists.  The ordering is arbitrary but consistent.
*   Returns -1 if sip1 <  sip2
*            0 if sip1 == sip2
*            1 if sip1 >  sip2
*
*****************************************************************************/

NLM_EXTERN Int4 SAM_OrderSeqIDChain (SeqId *sip1, SeqId *sip2);

/*****************************************************************************
*
*   Orders two SeqId's for binary searches, etc. Does NOT check the full
*   ValNode lists.  The ordering is arbitrary but consistent.
*   Returns -1 if sip1 <  sip2
*            0 if sip1 == sip2
*            1 if sip1 >  sip2
*
*****************************************************************************/

NLM_EXTERN Int4 SAM_OrderSeqID(SeqId *sip1, SeqId *sip2);

/*****************************************************************************
*
*   Orders two SeqId's for binary searches, etc.  If any id's in chains are
*   equivalent, returns 0.
*   Returns -1 if sip1 <  sip2
*            0 if sip1 == sip2
*            1 if sip1 >  sip2
*
*****************************************************************************/

NLM_EXTERN Boolean SAM_SeqIdCompareAll(SeqId *sip1, SeqId *sip2);

/*****************************************************************************
*
*   Compare two AlignId to make sure all valnodes compare exactly
*
*****************************************************************************/

NLM_EXTERN Boolean SAM_AlignIdCompare(AlignId *saip1, AlignId *saip2);


/*****************************************************************************
*
*     Looks for single AlignId, "a" in chain of AlignIds, "b"
*
*****************************************************************************/

NLM_EXTERN Boolean SAM_AlignIdIn (AlignId *a, AlignId *b);


/*****************************************************************************
*
*   	Compares a to b and returns
*
*   SIC_DIFF   = different types, could not be compared
*   SIC_NO     = types could be compared, and ids are different
*   SIC_YES    = types could be compared, and ids are the same
*
*****************************************************************************/

NLM_EXTERN Uint1 SAM_AlignIdComp (AlignId *a, AlignId *b);


/*******************************************************
*
*   duplicate a list of AlignId *
*
*******************************************************/

NLM_EXTERN AlignId * SAM_AlignIdDupList (AlignId *id_list);


/*******************************************************
*
*   Duplicates one AlignId
*
*******************************************************/

NLM_EXTERN AlignId * SAM_AlignIdDup (AlignId *oldid);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* SAMUTIL_H */
