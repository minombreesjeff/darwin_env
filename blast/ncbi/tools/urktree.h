/*
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
* File Name: urktree.h
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.3 $
*
* File Description: trees header
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urktree.h,v $
* Revision 6.3  1998/09/16 17:46:47  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#ifndef _URKTREE__
#define _URKTREE__

#include <ncbi.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TREE_NULL     0
#define TREE_ROOTED   1
#define TREE_UNROOTED 2

typedef struct GeneTreeNode
{
  CharPtr  name;
  FloatHi  score;
  Boolean  flagPivot;
  Int4     level, nodenumber;
  Int4     xcoord, ycoord;
  struct GeneTreeNode PNTR ptrChildLeft, PNTR ptrChildRight, PNTR ptrUp;
  struct GeneTreeNode PNTR ptrLevelLeft, PNTR ptrLevelRight;
} TreeNode, PNTR TreeNodePtr;

extern TreeNodePtr TreeCenterNode (TreeNodePtr ptrNode);
extern TreeNodePtr ExploreUnrootedTreeNumber (TreeNodePtr ptrNode,
                                              Int4 nodenumber);
extern TreeNodePtr ExploreUnrootedTree (TreeNodePtr ptrNode,
                                        TreeNodePtr ptrSkip,
                                        Int4 nodenumber);

extern TreeNodePtr ExploreTreeNumber (TreeNodePtr ptrNode,
                                      Int4 nodenumber);
extern TreeNodePtr ExploreTreeLevel (TreeNodePtr ptrNode, TreeNodePtr ptrSkip,
                                     Int4 level, Boolean PNTR flagTakeNext);
extern TreeNodePtr ExploreTree (TreeNodePtr ptrNode, TreeNodePtr ptrSkip,
                                Boolean PNTR flagTakeNext);

extern Int4 GetDeepestLevelBranch (TreeNodePtr ptrNode, Int4 level);
extern Int4 GetDeepestLevel (TreeNodePtr ptrNode);
extern TreeNodePtr GetWidestLevelLeftNode (TreeNodePtr ptrNode,
                                           Int4 startlevel);
extern TreeNodePtr GetLevelLeftNode (TreeNodePtr ptrNode, Int4 level);
extern void SetLevelNode (TreeNodePtr ptrNode, Int4 level);
extern void SetAllLevelNodes (TreeNodePtr ptrNode);
extern Int4 SetNumberNode (TreeNodePtr ptrNode, Int4 nodenumber);
extern Int4 SetAllNumberNodes (TreeNodePtr ptrNode);
extern void ZeroNodes (TreeNodePtr ptrNode);
extern Int4 CountNodes (TreeNodePtr ptrNode, Int4 nodenumber);
extern Int4 CountNodesOnLevel (TreeNodePtr ptrNode, Int4 level);

extern TreeNodePtr HeadNode (TreeNodePtr ptrNode);
extern TreeNodePtr LastNode (TreeNodePtr ptrNode);

extern TreeNodePtr FindPivotNode (TreeNodePtr ptrNode,
                                  TreeNodePtr ptrSkip);
extern TreeNodePtr SetPivotNode (TreeNodePtr ptrNode, Int4 level);
extern TreeNodePtr SetAllPivotNodes (TreeNodePtr ptrNode);
extern void UnSetPivotNodes (TreeNodePtr ptrNode);

extern void SetLeftRightRootedTreeNeighbors (TreeNodePtr ptrNode, Int4 level);
extern void SetAllLeftRightRootedTreeNeighbors (TreeNodePtr ptrNode);

extern Int4 GetMaximumTreeXcoord (TreeNodePtr ptrNode);
extern Int4 GetMinimumTreeXcoord (TreeNodePtr ptrNode);
extern Int4 GetMaximumTreeYcoord (TreeNodePtr ptrNode);
extern Int4 GetMinimumTreeYcoord (TreeNodePtr ptrNode);
extern void SetAllCoordinates (TreeNodePtr ptrNode, Int4 Xcoord, Int4 Ycoord,
                               Int4 Xdelta, Int4 Ydelta);
extern void SetTheoreticalCoordinates (TreeNodePtr ptrNode,
                                       Int4 xcoord, Int4 ycoord,
                                       Int4 xdelta, Int4 ydelta,
                                       Int4 xroot, Int4 xparent,
                                       Int4 xoffset, Int4 xoffmax);
extern void SetAllTheoreticalCoordinates (TreeNodePtr ptrNode,
                                          Int4 xcoord, Int4 ycoord);
extern void AdjustTheoreticalCoordinates (TreeNodePtr ptrNode,
                                          Int4 Xcoord, Int4 Xdelta,
                                          Int4 level);
extern void AdjustAllTheoreticalCoordinates (TreeNodePtr ptrNode, Int4 Xcoord);
extern void ChangeTreeScale (TreeNodePtr ptrNode, Int4 Xcoord, Int4 Ycoord,
                             FloatHi sx, FloatHi sy);
extern void ShiftTree (TreeNodePtr ptrNode, Int4 sx, Int4 sy);

extern Int4 TestTree (TreeNodePtr ptrNode);
extern TreeNodePtr RootTree (TreeNodePtr ptrNode,
                             Int4 left, Int4 right);
extern TreeNodePtr UnrootTree (TreeNodePtr ptrNode);

extern TreeNodePtr TreeNodeNew (void);
extern TreeNodePtr TreeNodeFree (TreeNodePtr ptrNode);
extern TreeNodePtr TreeNodeFreeBranch (TreeNodePtr ptrNode);
extern TreeNodePtr TreeNodeFreeTree (TreeNodePtr ptrNode);

#ifdef __cplusplus
}
#endif

#endif
