static char const rcsid[] = "$Id: urktree.c,v 6.6 2003/05/30 17:25:38 coulouri Exp $";

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
* File Name: urktree.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.6 $
*
* File Description: trees
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urktree.c,v $
* Revision 6.6  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.5  1998/09/16 18:03:37  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#include <urktree.h>

extern TreeNodePtr TreeCenterNode (TreeNodePtr ptrNode)
{
  if (ptrNode == NULL)
    return ptrNode;

  if (ptrNode->ptrUp == NULL)
    return ptrNode->ptrUp;

  if (ptrNode->ptrUp->ptrUp == ptrNode)
    return ptrNode;

  return TreeCenterNode (ptrNode->ptrUp);
}

extern TreeNodePtr ExploreUnrootedTreeNumber (TreeNodePtr ptrNode,
                                              Int4 nodenumber)
{
  ptrNode = TreeCenterNode (ptrNode);
  return ExploreUnrootedTree (ptrNode, ptrNode, nodenumber);
}

extern TreeNodePtr ExploreUnrootedTree (TreeNodePtr ptrNode,
                                        TreeNodePtr ptrSkip,
                                        Int4 nodenumber)
{
  TreeNodePtr ptrTemp;

  if (ptrNode == NULL)
    return ptrNode;

  if (ptrNode->nodenumber == nodenumber)
    return ptrNode;

  if ((ptrTemp = ExploreUnrootedTree (ptrNode->ptrChildLeft, ptrSkip,
                                      nodenumber)) != NULL)
    return ptrTemp;

  if ((ptrTemp = ExploreUnrootedTree (ptrNode->ptrChildRight, ptrSkip,
                                      nodenumber)) != NULL)
    return ptrTemp;

  if (ptrNode != ptrSkip)
    return ptrTemp;

  return ExploreUnrootedTree (ptrNode->ptrUp, ptrSkip, nodenumber);
}

extern TreeNodePtr ExploreTreeNumber (TreeNodePtr ptrNode,
                                      Int4 nodenumber)
{
  TreeNodePtr ptrTemp;

  if (ptrNode == NULL)
    return ptrNode;

  if (nodenumber == 0)
    return HeadNode (ptrNode);

  if (nodenumber == -1)
    return LastNode (HeadNode (ptrNode));

  if (ptrNode->nodenumber == nodenumber)
    return ptrNode;

  if ((ptrTemp = ExploreTreeNumber (ptrNode->ptrChildLeft,
                                    nodenumber)) != NULL)
    return ptrTemp;

  return ExploreTreeNumber (ptrNode->ptrChildRight, nodenumber);
}

extern TreeNodePtr ExploreTreeLevel (TreeNodePtr ptrNode, TreeNodePtr ptrSkip,
                                     Int4 level, Boolean PNTR flagTakeNext)
{
  TreeNodePtr ptrTemp;

  if (ptrNode == NULL)
    return ptrNode;

  if (ptrNode->level == level)
  {
    if (ptrSkip == NULL)
      return ptrNode;
    if (*flagTakeNext)
      return ptrNode;
    if (ptrNode == ptrSkip)
      *flagTakeNext = TRUE;
  }

  if ((ptrTemp = ExploreTreeLevel (ptrNode->ptrChildLeft, ptrSkip, level,
                                   flagTakeNext)) != NULL)
    return ptrTemp;

  return ExploreTreeLevel (ptrNode->ptrChildRight, ptrSkip, level,
                           flagTakeNext);
}

extern TreeNodePtr ExploreTree (TreeNodePtr ptrNode, TreeNodePtr ptrSkip,
                                Boolean PNTR flagTakeNext)
{
  TreeNodePtr ptrTemp;

  if (ptrNode == NULL)
    return ptrNode;

  if (ptrSkip == NULL)
    return ptrNode;
  if (*flagTakeNext)
    return ptrNode;
  if (ptrNode == ptrSkip)
    *flagTakeNext = TRUE;

  if ((ptrTemp = ExploreTree (ptrNode->ptrChildLeft, ptrSkip,
                              flagTakeNext)) != NULL)
    return ptrTemp;

  return ExploreTree (ptrNode->ptrChildRight, ptrSkip, flagTakeNext);
}

extern Int4 GetDeepestLevelBranch (TreeNodePtr ptrNode, Int4 level)
{
  if (ptrNode == NULL)
    return level;

  if (ptrNode->ptrChildLeft != NULL || ptrNode->ptrChildRight != NULL)
    if (ptrNode->level > level)
      level = ptrNode->level;

  level = GetDeepestLevelBranch (ptrNode->ptrChildLeft, level);
  level = GetDeepestLevelBranch (ptrNode->ptrChildRight, level);

  return level;
}

extern Int4 GetDeepestLevel (TreeNodePtr ptrNode)
{
  Int4 level;

  if (ptrNode == NULL)
    return -1;
  if (ptrNode->ptrChildLeft == NULL && ptrNode->ptrChildRight == NULL)
    return 0;
  level = GetDeepestLevelBranch (ptrNode, 0);
  level++;
  return level;
}

extern TreeNodePtr GetWidestLevelLeftNode (TreeNodePtr ptrNode,
                                           Int4 startlevel)
{
  Boolean     flagTakeNext;
  TreeNodePtr ptrNodeFind, ptrNodeLeft;
  Int4        nodecount, nodemax;

  if (ptrNode == NULL)
    return NULL;

  ptrNodeLeft = NULL;
  nodemax = 0;
  ptrNodeFind = NULL;
  flagTakeNext = FALSE;
  while ((ptrNodeFind = ExploreTree (ptrNode, ptrNodeFind,
         &flagTakeNext)) != NULL)
  {
    if (ptrNodeFind->ptrLevelLeft == NULL)
    {
      if (ptrNodeFind->level > startlevel)
      {
        nodecount = CountNodesOnLevel (ptrNodeFind, ptrNodeFind->level);
        if (nodecount > nodemax)
        {
          nodemax = nodecount;
          ptrNodeLeft = ptrNodeFind;;
        }
      }
    }
    flagTakeNext = FALSE;
  }
  return ptrNodeLeft;
}

extern TreeNodePtr GetLevelLeftNode (TreeNodePtr ptrNode, Int4 level)
{
  Boolean     flagTakeNext;
  TreeNodePtr ptrNodeFind;

  if (ptrNode == NULL)
    return NULL;

  ptrNodeFind = NULL;
  flagTakeNext = FALSE;
  while ((ptrNodeFind = ExploreTree (ptrNode, ptrNodeFind,
         &flagTakeNext)) != NULL)
  {
    if (ptrNodeFind->level == level)
      return ptrNodeFind;
    flagTakeNext = FALSE;
  }
  return NULL;
}

extern void SetLevelNode (TreeNodePtr ptrNode, Int4 level)
{
  if (ptrNode == NULL)
    return;

  ptrNode->level = level;
  level++;
  SetLevelNode (ptrNode->ptrChildLeft, level);
  SetLevelNode (ptrNode->ptrChildRight, level);

  return;
}

extern void SetAllLevelNodes (TreeNodePtr ptrNode)
{
  Int4 level;

  if ((ptrNode = HeadNode (ptrNode)) == NULL)
    return;

  level = 0;
  ptrNode->level = level;
  level++;
  SetLevelNode (ptrNode->ptrChildLeft, level);
  SetLevelNode (ptrNode->ptrChildRight, level);

  return;
}

extern Int4 SetNumberNode (TreeNodePtr ptrNode, Int4 nodenumber)
{
  if (ptrNode == NULL)
    return nodenumber;

  ptrNode->nodenumber = nodenumber++;
  nodenumber = SetNumberNode (ptrNode->ptrChildLeft, nodenumber);
  nodenumber = SetNumberNode (ptrNode->ptrChildRight, nodenumber);

  return nodenumber;
}

extern Int4 SetAllNumberNodes (TreeNodePtr ptrNode)
{
  Int4 nodenumber;

  if ((ptrNode = HeadNode (ptrNode)) == NULL)
    return -1;

  nodenumber = 0;
  ptrNode->nodenumber = nodenumber++;
  nodenumber = SetNumberNode (ptrNode->ptrChildLeft, nodenumber);
  nodenumber = SetNumberNode (ptrNode->ptrChildRight, nodenumber);

  return nodenumber;
}

extern void ZeroNodes (TreeNodePtr ptrNode)
{
  if (ptrNode == NULL)
    return;

  ptrNode->nodenumber = 0;
  ZeroNodes (ptrNode->ptrChildLeft);
  ZeroNodes (ptrNode->ptrChildRight);

  return;
}

extern Int4 CountNodes (TreeNodePtr ptrNode, Int4 nodenumber)
{
  if (ptrNode == NULL)
    return nodenumber;

  nodenumber++;
  nodenumber = CountNodes (ptrNode->ptrChildLeft, nodenumber);
  nodenumber = CountNodes (ptrNode->ptrChildRight, nodenumber);

  return nodenumber;
}

extern Int4 CountNodesOnLevel (TreeNodePtr ptrNode, Int4 level)
{
  Int4        nodenumber;
  TreeNodePtr ptrNodeFind;
  Boolean     flagTakeNext;

  if (ptrNode == NULL)
    return 0;

  nodenumber = 0;

  flagTakeNext = FALSE;
  if ((ptrNodeFind = ExploreTreeLevel (ptrNode, NULL, level,
       &flagTakeNext)) != NULL)
  {
    while (ptrNodeFind != NULL)
    {
      nodenumber++;
      ptrNodeFind = ptrNodeFind->ptrLevelRight;
    }
  }
  return nodenumber;
}

extern TreeNodePtr HeadNode (TreeNodePtr ptrNode)
{
  if (ptrNode == NULL)
    return ptrNode;

  while (ptrNode->ptrUp != NULL)
    ptrNode = ptrNode->ptrUp;

  return ptrNode;
}

extern TreeNodePtr LastNode (TreeNodePtr ptrNode)
{
  TreeNodePtr ptrNodeFind, ptrNodeCurrent;
  Boolean     flagTakeNext;

  if ((ptrNode = HeadNode (ptrNode)) == NULL)
    return ptrNode;

  flagTakeNext = FALSE;
  ptrNodeCurrent = ptrNode;
  while ((ptrNodeFind = ExploreTree (ptrNode, ptrNodeCurrent,
         &flagTakeNext)) != NULL)
  {
    ptrNodeCurrent = ptrNodeFind;
    flagTakeNext = FALSE;
  }
  return ptrNodeCurrent;
}

extern TreeNodePtr FindPivotNode (TreeNodePtr ptrNode,
                                  TreeNodePtr ptrSkip)
{
  if (ptrNode == NULL)
    return ptrNode;

  if (ptrNode != ptrSkip)
  {
    if (ptrNode->flagPivot)
      return ptrNode;
  }

  if ((ptrNode = FindPivotNode (ptrNode->ptrChildLeft, ptrSkip)) != NULL)
    return ptrNode;

  return FindPivotNode (ptrNode->ptrChildRight, ptrSkip);
}

extern TreeNodePtr SetPivotNode (TreeNodePtr ptrNode, Int4 level)
{
  TreeNodePtr ptrOne, ptrTwo;
  Boolean     flagTakeNext;

  if (ptrNode == NULL)
    return ptrNode;

  flagTakeNext = FALSE;
  ptrOne = ExploreTreeLevel (ptrNode, NULL, level, &flagTakeNext);
  ptrTwo = ExploreTreeLevel (ptrNode, ptrOne, level, &flagTakeNext);

  if (ptrOne != NULL && ptrTwo == NULL)
  {
    ptrOne->flagPivot = TRUE;
    return ptrOne;
  }
  else
  {
    return NULL;
  }
}

extern TreeNodePtr SetAllPivotNodes (TreeNodePtr ptrNode)
{
  Int4       i, level;
  TreeNodePtr ptrPivot;

  if ((ptrNode = HeadNode (ptrNode)) == NULL)
    return ptrNode;

  UnSetPivotNodes (ptrNode);
  level = GetDeepestLevelBranch (ptrNode, -1);

  for (i = 0; i < level; i++)
  {
    ptrPivot = SetPivotNode (ptrNode, i);
    if (ptrPivot != NULL)
      ptrNode = ptrPivot;
  }
  return HeadNode (ptrNode);
}

extern void UnSetPivotNodes (TreeNodePtr ptrNode)
{
  if (ptrNode == NULL)
    return;

  ptrNode->flagPivot = FALSE;;
  UnSetPivotNodes (ptrNode->ptrChildLeft);
  UnSetPivotNodes (ptrNode->ptrChildRight);

  return;
}

extern void SetLeftRightRootedTreeNeighbors (TreeNodePtr ptrNode, Int4 level)
{
  TreeNodePtr ptrNodeFind, ptrNodeCurrent;
  Boolean     flagTakeNext;

  if (ptrNode == NULL)
    return;

  ptrNodeFind = ptrNodeCurrent = NULL;
  flagTakeNext = FALSE;
  while ((ptrNodeFind = ExploreTreeLevel (ptrNode, ptrNodeFind, level,
          &flagTakeNext)) != NULL)
  {
    ptrNodeFind->ptrLevelLeft = NULL;
    ptrNodeFind->ptrLevelRight = NULL;
    if (ptrNodeCurrent != NULL)
    {
      ptrNodeCurrent->ptrLevelRight = ptrNodeFind;
      ptrNodeFind->ptrLevelLeft = ptrNodeCurrent;
    }
    ptrNodeCurrent = ptrNodeFind;
    flagTakeNext = FALSE;
  }

  return;
}

extern void SetAllLeftRightRootedTreeNeighbors (TreeNodePtr ptrNode)
{
  Int4  i, depth;

  if (TestTree (ptrNode) != TREE_ROOTED)
    return;
  ptrNode = HeadNode (ptrNode);
  SetLevelNode (ptrNode, 0);
  depth = GetDeepestLevel (ptrNode);
  for (i = 0; i <= depth; i++)
    SetLeftRightRootedTreeNeighbors (ptrNode, i);

  return;
}

extern Int4 GetMaximumTreeXcoord (TreeNodePtr ptrNode)
{
  Int4        max;
  TreeNodePtr ptrNodeCurrent;
  Boolean     flagTakeNext;

  if (ptrNode == NULL)
    return 0;

  max = ptrNode->xcoord;
  flagTakeNext = FALSE;
  ptrNodeCurrent = NULL;
  while ((ptrNodeCurrent = ExploreTree (ptrNode, ptrNodeCurrent,
         &flagTakeNext)) != NULL)
  {
    if (ptrNodeCurrent->xcoord > max)
      max = ptrNodeCurrent->xcoord;
    flagTakeNext = FALSE;
  }
  return max;
}

extern Int4 GetMinimumTreeXcoord (TreeNodePtr ptrNode)
{
  Int4        min;
  TreeNodePtr ptrNodeCurrent;
  Boolean     flagTakeNext;

  if (ptrNode == NULL)
    return 0;

  min = ptrNode->xcoord;
  flagTakeNext = FALSE;
  ptrNodeCurrent = NULL;
  while ((ptrNodeCurrent = ExploreTree (ptrNode, ptrNodeCurrent,
         &flagTakeNext)) != NULL)
  {
    if (ptrNodeCurrent->xcoord < min)
      min = ptrNodeCurrent->xcoord;
    flagTakeNext = FALSE;
  }
  return min;
}

extern Int4 GetMaximumTreeYcoord (TreeNodePtr ptrNode)
{
  Int4        max;
  TreeNodePtr ptrNodeCurrent;
  Boolean     flagTakeNext;

  if (ptrNode == NULL)
    return 0;

  max = ptrNode->ycoord;
  flagTakeNext = FALSE;
  ptrNodeCurrent = NULL;
  while ((ptrNodeCurrent = ExploreTree (ptrNode, ptrNodeCurrent,
         &flagTakeNext)) != NULL)
  {
    if (ptrNodeCurrent->ycoord > max)
      max = ptrNodeCurrent->ycoord;
    flagTakeNext = FALSE;
  }
  return max;
}

extern Int4 GetMinimumTreeYcoord (TreeNodePtr ptrNode)
{
  Int4        min;
  TreeNodePtr ptrNodeCurrent;
  Boolean     flagTakeNext;

  if (ptrNode == NULL)
    return 0;

  min = ptrNode->ycoord;
  flagTakeNext = FALSE;
  ptrNodeCurrent = NULL;
  while ((ptrNodeCurrent = ExploreTree (ptrNode, ptrNodeCurrent,
         &flagTakeNext)) != NULL)
  {
    if (ptrNodeCurrent->ycoord < min)
      min = ptrNodeCurrent->ycoord;
    flagTakeNext = FALSE;
  }
  return min;
}

extern void SetAllCoordinates (TreeNodePtr ptrNode, Int4 Xcoord, Int4 Ycoord,
                               Int4 Xdelta, Int4 Ydelta)
{
  Int4        level, levelold, tlevel, decomp;
  Int4        depth, width, widthold;
  Int4        midpoint, xleft, xright, xleftold, xrightold, xthis;
  Int4        xmin, xdiff;
  Int4        index, indexshift;
  TreeNodePtr ptrNodeCurr, ptrNodeThis, ptrNodeLeft, ptrNodeCheck, ptrNodeS;
  Boolean     flagTakeNext;

  if (TestTree (ptrNode) != TREE_ROOTED)
    return;
  ptrNode = HeadNode (ptrNode);
  ptrNode->xcoord = Xcoord;
  ptrNode->ycoord = Ycoord;

  SetAllLevelNodes (ptrNode);
  depth = GetDeepestLevel (ptrNode);

  levelold = level = 0;
  widthold = 1;
  xleftold = Xcoord;
  xrightold = Xcoord;
  while (level <= depth)
  {
    ptrNodeLeft = GetWidestLevelLeftNode (ptrNode, level);
    if (ptrNodeLeft == NULL)
      break;
    level = ptrNodeLeft->level;
    width = CountNodesOnLevel (ptrNode, level);
    if (widthold < width)
    {
      decomp = 1;
      midpoint = (xrightold + xleftold) / 2;
    }
    else
    {
      decomp = widthold / width;
      ptrNodeCheck = ptrNodeLeft;
      while (ptrNodeCheck->level != levelold)
        ptrNodeCheck = ptrNodeCheck->ptrUp;
      midpoint = ptrNodeCheck->xcoord;
    }
    if (level > levelold+1)
    {
      xthis = midpoint -
               (((width*Xdelta*decomp)+((width/2-1)*Xdelta*decomp))/2);
    }
    else
    {
      xthis = ptrNodeLeft->ptrUp->xcoord - (Xdelta*decomp);
    }
    ptrNodeThis = ptrNodeLeft;
    while (ptrNodeThis != NULL)
    {
      if (ptrNodeThis->ptrLevelLeft != NULL)
      {
        if (((ptrNodeThis->ptrChildLeft == NULL) &&
             (ptrNodeThis->ptrChildRight == NULL)) &&
            ((ptrNodeThis->ptrLevelLeft->ptrChildLeft == NULL) &&
             (ptrNodeThis->ptrLevelLeft->ptrChildRight == NULL)))
        xthis += (Xdelta*decomp);
      }
      ptrNodeThis->xcoord = xthis;
      ptrNodeThis->ycoord = Ycoord + (level*Ydelta);
      xthis += (Xdelta*2*decomp);
      ptrNodeThis->ptrLevelRight->xcoord = xthis;
      ptrNodeThis->ptrLevelRight->ycoord = Ycoord + (level*Ydelta);
      xthis += (Xdelta*decomp);
      ptrNodeThis = ptrNodeThis->ptrLevelRight->ptrLevelRight;
    }
    ptrNodeCurr = ptrNodeLeft;
    for (tlevel = level; tlevel != levelold; tlevel--)
    {
      ptrNodeThis = ptrNodeCurr;
      while (ptrNodeThis != NULL)
      {
        if (ptrNodeThis->xcoord == 0 && ptrNodeThis->ycoord == 0)
        {
          ptrNodeThis->xcoord = ptrNodeThis->ptrLevelLeft->xcoord + Xdelta;
          if (((ptrNodeThis->ptrChildLeft == NULL) &&
               (ptrNodeThis->ptrChildRight == NULL)) &&
              ((ptrNodeThis->ptrLevelLeft->ptrChildLeft == NULL) &&
               (ptrNodeThis->ptrLevelLeft->ptrChildRight == NULL)))
            ptrNodeThis->xcoord += Xdelta;
          ptrNodeThis->ycoord = Ycoord + (tlevel * Ydelta);
          if ((ptrNodeThis->xcoord >= ptrNodeThis->ptrLevelRight->xcoord) &&
              ptrNodeThis->ptrLevelRight->xcoord != 0 &&
              ptrNodeThis->ptrLevelRight->ycoord != 0)
          {
            xdiff = ptrNodeThis->xcoord -
                    ptrNodeThis->ptrLevelRight->xcoord + Xdelta;
            ptrNodeCheck = ptrNodeThis->ptrLevelRight;
            while (ptrNodeCheck != NULL)
            {
              if (ptrNodeCheck->xcoord != 0 &&
                  ptrNodeCheck->ycoord != 0)
              {
                ptrNodeCheck->xcoord += xdiff;
                flagTakeNext = FALSE;
                ptrNodeS = ptrNodeCheck;
                while ((ptrNodeS = ExploreTree (ptrNodeCheck, ptrNodeS,
                       &flagTakeNext)) != NULL)
                {
                  if (ptrNodeS->level <= level)
                    ptrNodeS->xcoord += xdiff;
                  flagTakeNext = FALSE;
                }
              }
              ptrNodeCheck = ptrNodeCheck->ptrLevelRight;
            }
          }
        }
        ptrNodeThis = ptrNodeThis->ptrLevelRight;
      }
      if (tlevel == levelold+1)
        break;
      ptrNodeThis = ptrNodeCurr;
      while (ptrNodeThis != NULL)
      {
        xthis = (ptrNodeThis->xcoord + ptrNodeThis->ptrLevelRight->xcoord) / 2;
        ptrNodeThis->ptrUp->xcoord = xthis;
        ptrNodeThis->ptrUp->ycoord = Ycoord + ((tlevel-1)*Ydelta);
        if (ptrNodeThis->ptrLevelRight->ptrLevelRight == NULL)
        {
          ptrNodeCheck = ptrNodeThis->ptrUp->ptrLevelRight;
          while (ptrNodeCheck != NULL)
          {
            xthis += (Xdelta*2*decomp);
            ptrNodeCheck->xcoord = xthis;
            ptrNodeCheck->ycoord = Ycoord + ((tlevel-1)*Ydelta);
            ptrNodeCheck = ptrNodeCheck->ptrLevelRight;
          }
        }
        ptrNodeThis = ptrNodeThis->ptrLevelRight->ptrLevelRight;
      }
      ptrNodeCurr = ptrNodeCheck = ptrNodeCurr->ptrUp;
      xthis = ptrNodeCheck->xcoord;
      ptrNodeCheck = ptrNodeCheck->ptrLevelLeft;
      while (ptrNodeCheck != NULL)
      {
        xthis -= (Xdelta*2*decomp);
        ptrNodeCheck->xcoord = xthis;
        ptrNodeCheck->ycoord = Ycoord + ((tlevel-1)*Ydelta);
        ptrNodeCheck = ptrNodeCheck->ptrLevelLeft;
      }
      while (ptrNodeCurr->ptrLevelLeft != NULL)
        ptrNodeCurr = ptrNodeCurr->ptrLevelLeft;
    }
    if (levelold == 0)
    {
      ptrNodeThis = ptrNodeCurr;
      if (ptrNodeThis != NULL)
      {
        xthis = (ptrNodeThis->xcoord + ptrNodeThis->ptrLevelRight->xcoord) / 2;
        ptrNodeThis->ptrUp->xcoord = xthis;
      }
    }
    levelold = level;
    widthold = width;

    ptrNodeLeft = GetLevelLeftNode (ptrNode, level);
    xleft = ptrNodeLeft->xcoord;
    while (ptrNodeLeft->ptrLevelRight != NULL)
      ptrNodeLeft = ptrNodeLeft->ptrLevelRight;
    xright = ptrNodeLeft->xcoord;

    xleftold = xleft;
    xrightold = xright;
  }
  for (level = 0; level <= depth; level++)
  {
    ptrNodeCheck = ptrNodeLeft = GetLevelLeftNode (ptrNode, level);
    if (ptrNodeLeft != NULL && ptrNodeLeft->ptrUp != NULL)
    {
      index = indexshift = 0;
      while (ptrNodeCheck->ptrLevelRight != NULL)
      {
        index++;
        if (ptrNodeCheck->ptrLevelRight->xcoord >=
            ptrNodeCheck->ptrLevelRight->ptrUp->xcoord)
        {
          indexshift = -1;
        }
        else
        {
          if (indexshift == -1)
            indexshift = index;
        }
        ptrNodeCheck = ptrNodeCheck->ptrLevelRight;
      }
      if (indexshift > -1)
      {
        ptrNodeCheck = ptrNodeLeft;
        index = 0;
        while (index < indexshift)
        {
          ptrNodeCheck = ptrNodeCheck->ptrLevelRight;
          index++;
        }
        xdiff = ptrNodeCheck->ptrUp->xcoord - ptrNodeCheck->xcoord - Xdelta;
        while (ptrNodeCheck != NULL)
        {
          ptrNodeCheck->xcoord += xdiff;
          ptrNodeCheck = ptrNodeCheck->ptrLevelRight;
        }
        level--;
      }
    }
  }
  xmin = GetMinimumTreeXcoord (ptrNode);
  if (xmin < 0)
  {
    xmin *= -1;
    xmin += 25;
    ShiftTree (ptrNode, xmin, 0);
  }
  return;
}

extern void SetTheoreticalCoordinates (TreeNodePtr ptrNode,
                                       Int4 xcoord, Int4 ycoord,
                                       Int4 xdelta, Int4 ydelta,
                                       Int4 xroot, Int4 xparent,
                                       Int4 xoffset, Int4 xoffmax)
{
  Int4 xLcoord, xRcoord, xLoffset, xRoffset;

  if (ptrNode == NULL)
    return;

  ptrNode->xcoord = xcoord;
  ptrNode->ycoord = ycoord;

  if (xcoord >= xroot)
  {
    xRoffset = (xoffset * xdelta * 3) + xparent;
    xLcoord = xRoffset - xdelta;
    if (xLcoord < xcoord)
      xLcoord = xcoord;
    xRcoord = xRoffset + xdelta;
    if (xoffset > xoffmax)
    {
      xoffset = (Int4) pow (2, 20);
    }
    SetTheoreticalCoordinates (ptrNode->ptrChildLeft,
                               xLcoord, ycoord+ydelta,
                               xdelta, ydelta, xroot,
                               xparent, 2*xoffset, xoffmax);
    SetTheoreticalCoordinates (ptrNode->ptrChildRight,
                               xRcoord, ycoord+ydelta,
                               xdelta, ydelta, xroot,
                               xparent, 2*xoffset+1, xoffmax);
  }
  else
  {
    xLoffset = xparent - (xoffset * xdelta * 3);
    xLcoord = xLoffset - xdelta;
    xRcoord = xLoffset + xdelta;
    if (xRcoord > xcoord)
      xRcoord = xcoord;
    SetTheoreticalCoordinates (ptrNode->ptrChildLeft,
                               xLcoord, ycoord+ydelta,
                               xdelta, ydelta, xroot,
                               xparent, 2*xoffset+1, xoffmax);
    SetTheoreticalCoordinates (ptrNode->ptrChildRight,
                               xRcoord, ycoord+ydelta,
                               xdelta, ydelta, xroot,
                               xparent, 2*xoffset, xoffmax);
  }
  return;
}

extern void SetAllTheoreticalCoordinates (TreeNodePtr ptrNode,
                                          Int4 xcoord, Int4 ycoord)
{
  Int4 xdelta, ydelta, xoffmax;

  if (TestTree (ptrNode) != TREE_ROOTED)
    return;
  ptrNode = HeadNode (ptrNode);

  ptrNode->xcoord = xcoord;
  ptrNode->ycoord = ycoord;
  xdelta = 1;
  ydelta = 1;

  xoffmax = (Int4) pow (2, 29);

  SetTheoreticalCoordinates (ptrNode->ptrChildLeft,
                             xcoord-xdelta, ycoord+ydelta, xdelta, ydelta,
                             xcoord, xcoord-xdelta, 0, xoffmax);
  SetTheoreticalCoordinates (ptrNode->ptrChildRight,
                             xcoord+xdelta, ycoord+ydelta, xdelta, ydelta,
                             xcoord, xcoord+xdelta, 0, xoffmax);
  return;
}

extern void AdjustTheoreticalCoordinates (TreeNodePtr ptrNode,
                                          Int4 Xcoord, Int4 Xdelta,
                                          Int4 level)
{
  Int4        xpleft, xpright, xpdelta, xleft, xright, xdelta;
  Boolean     flagTakeNext, flagOnEntry;
  TreeNodePtr ptrNodeLeftEnd, ptrNodeStart, ptrNodeCurrent, ptrNodeLoop;

  flagOnEntry = TRUE;
  flagTakeNext = FALSE;
  if ((ptrNodeLeftEnd = ExploreTreeLevel (ptrNode, NULL, level,
       &flagTakeNext)) == NULL)
    return;

  if (ptrNodeLeftEnd->ptrUp == NULL)
    return;

  ptrNodeCurrent = ptrNodeStart = ptrNodeLeftEnd;
  while (ptrNodeCurrent->ptrLevelRight != NULL)
  {
    if (flagOnEntry && ptrNodeCurrent->xcoord >= Xcoord)
    {
      flagOnEntry = FALSE;
      xleft = ptrNodeCurrent->xcoord;
      xpleft = ptrNodeCurrent->ptrUp->xcoord;
      xdelta = xleft - xpleft + Xdelta;
      for (ptrNodeLoop = ptrNodeCurrent;
           ptrNodeLoop != NULL;
           ptrNodeLoop = ptrNodeLoop->ptrLevelRight)
      {
        ptrNodeLoop->xcoord -= xdelta;
      }
      continue;
    }
    flagOnEntry = FALSE;
    if (ptrNodeCurrent->ptrLevelRight->ptrLevelRight == NULL &&
        ptrNodeCurrent == ptrNodeStart)
    {
      if (ptrNodeCurrent->ptrLevelLeft != NULL)
      {
        xdelta = ptrNodeCurrent->xcoord -
                 ptrNodeCurrent->ptrLevelRight->xcoord;
      }
      else
      {
        xdelta = ptrNodeCurrent->xcoord + Xdelta -
                 ptrNodeCurrent->ptrUp->xcoord;
      }
      if (xdelta < 0)
        xdelta *= -1;
      if (xdelta > 2*Xdelta)
      {
        if (ptrNodeCurrent->xcoord >= Xcoord)
        {
          while (ptrNodeCurrent != NULL)
          {
            ptrNodeCurrent->xcoord -= xdelta;
            ptrNodeCurrent = ptrNodeCurrent->ptrLevelRight;
          }
        }
        else
        {
          while (ptrNodeCurrent->ptrLevelRight != NULL)
          {
            ptrNodeCurrent = ptrNodeCurrent->ptrLevelRight;
          }
          while (ptrNodeCurrent != ptrNodeStart)
          {
            ptrNodeCurrent->xcoord += xdelta;
            ptrNodeCurrent = ptrNodeCurrent->ptrLevelLeft;
          }
          ptrNodeCurrent->xcoord += xdelta;
        }
      }
      ptrNodeCurrent = ptrNodeStart->ptrLevelRight;
      continue;
    }

    xleft = ptrNodeCurrent->xcoord;
    xright = ptrNodeCurrent->ptrLevelRight->xcoord;
    xdelta = xright - xleft;
    xpleft = ptrNodeCurrent->ptrUp->xcoord;
    xpright = ptrNodeCurrent->ptrLevelRight->ptrUp->xcoord;
    xpdelta = xpright - xpleft;

    if (xdelta > 2*Xdelta)
    {
      if (xleft >= Xcoord && xright >= Xcoord)
      {
        if (xright == xpright)
          xright -= Xdelta;
        ptrNodeCurrent->ptrLevelRight->xcoord = xright;
        xdelta = xright - xleft - Xdelta;
        if (((ptrNodeCurrent->ptrChildLeft == NULL) &&
             (ptrNodeCurrent->ptrChildRight == NULL)) &&
            ((ptrNodeCurrent->ptrLevelRight->ptrChildLeft == NULL) &&
             (ptrNodeCurrent->ptrLevelRight->ptrChildRight == NULL)))
          xdelta -= Xdelta;
        if (xdelta > xright - xpright + Xdelta)
          xdelta = xright - xpright + Xdelta;
        for (ptrNodeLoop = ptrNodeCurrent->ptrLevelRight;
             ptrNodeLoop != NULL;
             ptrNodeLoop = ptrNodeLoop->ptrLevelRight)
        {
          ptrNodeLoop->xcoord -= xdelta;
        }
      }
      else if (xleft < Xcoord && xright < Xcoord)
      {
        if (xleft == xpleft)
          xleft += Xdelta;
        ptrNodeCurrent->xcoord = xleft;
        xdelta = xpright - xleft - Xdelta - Xdelta;
        if (((ptrNodeCurrent->ptrChildLeft == NULL) &&
             (ptrNodeCurrent->ptrChildRight == NULL)) &&
            ((ptrNodeCurrent->ptrLevelLeft->ptrChildLeft == NULL) &&
             (ptrNodeCurrent->ptrLevelLeft->ptrChildRight == NULL)))
          xdelta -= Xdelta;
        if (xdelta > xpleft - xleft + Xdelta)
          xdelta = xpleft - xleft + Xdelta;
        for (ptrNodeLoop = ptrNodeCurrent;
             ptrNodeLoop != ptrNodeStart;
             ptrNodeLoop = ptrNodeLoop->ptrLevelLeft)
        {
          ptrNodeLoop->xcoord += xdelta;
        }
        ptrNodeLoop->xcoord += xdelta;
      }
      else
      {
        if (xpdelta > 2*Xdelta)
        {
          if (xright == xpright)
            xright -= Xdelta;
          if (xleft == xpleft)
            xleft += Xdelta;
        }
        ptrNodeCurrent->ptrLevelRight->xcoord = xright;
        xdelta = xright - xpright + Xdelta;
        for (ptrNodeLoop = ptrNodeCurrent->ptrLevelRight;
             ptrNodeLoop != NULL;
             ptrNodeLoop = ptrNodeLoop->ptrLevelRight)
        {
          ptrNodeLoop->xcoord -= xdelta;
        }
        ptrNodeCurrent->xcoord = xleft;
        xdelta = xpleft - xleft + Xdelta;
        for (ptrNodeLoop = ptrNodeCurrent;
             ptrNodeLoop != ptrNodeStart;
             ptrNodeLoop = ptrNodeLoop->ptrLevelLeft)
        {
          ptrNodeLoop->xcoord += xdelta;
        }
        ptrNodeLoop->xcoord += xdelta;
        if (xpdelta <= 2*Xdelta)
        {
          xright = xpright;
          xleft = xpleft;
          ptrNodeCurrent->ptrLevelRight->xcoord = xright;
          ptrNodeCurrent->xcoord = xleft;
        }
      }
      ptrNodeStart = ptrNodeCurrent->ptrLevelRight;
    }
    ptrNodeCurrent = ptrNodeCurrent->ptrLevelRight;
  }

  return;
}

extern void AdjustAllTheoreticalCoordinates (TreeNodePtr ptrNode, Int4 Xcoord)
{
  Int4  i, depth, xmin;
  Int4  Xdelta;

  if (TestTree (ptrNode) != TREE_ROOTED)
    return;
  ptrNode = HeadNode (ptrNode);
  depth = GetDeepestLevel (ptrNode);
  Xdelta = 1;

  for (i = 0; i <= depth; i++)
    AdjustTheoreticalCoordinates (ptrNode, Xcoord, Xdelta, i);

  xmin = GetMinimumTreeXcoord (ptrNode);
  if (xmin < 0)
  {
    xmin *= -1;
    xmin += 25;
    ShiftTree (ptrNode, xmin, 0);
  }
  return;
}

extern void ChangeTreeScale (TreeNodePtr ptrNode, Int4 Xcoord, Int4 Ycoord,
                             FloatHi sx, FloatHi sy)
{
  Boolean     flagTakeNext;
  TreeNodePtr ptrNodeCurrent;
  FloatHi     tx, ty;

  if (TestTree (ptrNode) != TREE_ROOTED)
    return;
  ptrNode = HeadNode (ptrNode);

  flagTakeNext = FALSE;
  ptrNodeCurrent = NULL;
  while ((ptrNodeCurrent = ExploreTree (ptrNode, ptrNodeCurrent,
         &flagTakeNext)) != NULL)
  {
    tx = (FloatHi) (ptrNodeCurrent->xcoord - Xcoord);
    ty = (FloatHi) (ptrNodeCurrent->ycoord - Ycoord);
    tx *= sx;
    ty *= sy;
    if (tx >= 0.0)
    {
      if (tx - (FloatHi) ((Int4) tx) >= 0.5)
        tx += 1.0;
    }
    else
    {
      if (tx + (FloatHi) ((Int4) tx) <= 0.5)
        tx -= 1.0;
    }
    if (ty >= 0.0)
    {
      if (ty - (FloatHi) ((Int4) ty) >= 0.5)
        ty += 1.0;
    }
    else
    {
      if (ty - (FloatHi) ((Int4) ty) <= 0.5)
        ty -= 1.0;
    }
    ptrNodeCurrent->xcoord = Xcoord + (Int4) tx;
    ptrNodeCurrent->ycoord = Ycoord + (Int4) ty;
    flagTakeNext = FALSE;
  }
  return;
}

extern void ShiftTree (TreeNodePtr ptrNode, Int4 sx, Int4 sy)
{
  Boolean     flagTakeNext;
  TreeNodePtr ptrNodeCurrent;

  if (TestTree (ptrNode) != TREE_ROOTED)
    return;
  ptrNode = HeadNode (ptrNode);

  flagTakeNext = FALSE;
  ptrNodeCurrent = NULL;
  while ((ptrNodeCurrent = ExploreTree (ptrNode, ptrNodeCurrent,
         &flagTakeNext)) != NULL)
  {
    ptrNodeCurrent->xcoord += sx;
    ptrNodeCurrent->ycoord += sy;
    flagTakeNext = FALSE;
  }
  return;
}

extern Int4 TestTree (TreeNodePtr ptrNode)
{
  if (ptrNode == NULL)
    return TREE_NULL;

  while (ptrNode != NULL && ptrNode->ptrUp != NULL)
  {
    if (ptrNode->ptrUp->ptrUp == ptrNode)
      return TREE_UNROOTED;
    ptrNode = ptrNode->ptrUp;
  }
  return TREE_ROOTED;
}

extern TreeNodePtr RootTree (TreeNodePtr ptrNode, Int4 left, Int4 right)
{
  TreeNodePtr ptrTemp;

  if (TestTree (ptrNode) == TREE_ROOTED)
    return NULL;

  ptrNode = ExploreUnrootedTreeNumber (ptrNode, left);
  if (ptrNode != NULL)
  {
    ptrTemp = TreeNodeNew ();

    if (ptrNode->ptrUp->ptrUp == ptrNode)
    {
      ptrTemp->ptrChildRight = ptrNode->ptrUp;
      ptrTemp->ptrChildLeft = ptrNode;
      ptrTemp->ptrChildRight->ptrUp = ptrTemp;
      ptrTemp->ptrChildLeft->ptrUp = ptrTemp;
      return ptrTemp;
    }

    while (ptrNode->ptrUp->ptrUp != ptrNode)
    {
/* if right then want to mirror and force left */
      if ((ptrNode->ptrChildRight != NULL) &&
          (ptrNode->ptrChildRight->nodenumber == right))
      {
        ptrTemp->ptrChildLeft = ptrNode->ptrChildRight;
        ptrNode->ptrChildRight = ptrNode->ptrChildLeft;
        ptrNode->ptrChildLeft = ptrTemp->ptrChildLeft;
        ptrTemp->ptrChildLeft = NULL;
      }

      if ((ptrNode->ptrChildLeft != NULL) &&
          (ptrNode->ptrChildLeft->nodenumber == right))
      {
        ptrTemp->ptrChildLeft = ptrNode->ptrChildLeft;
        ptrNode->ptrChildLeft->ptrUp = ptrTemp;
      }
      else /* will root the tree -- default is up */
      {
        if (ptrNode->ptrUp->ptrChildRight == ptrNode)
        {
/* if right then want to mirror and force left */
          ptrTemp->ptrChildRight = ptrNode->ptrUp->ptrChildRight;
          ptrNode->ptrUp->ptrChildRight = ptrNode->ptrUp->ptrChildLeft;
          ptrNode->ptrUp->ptrChildLeft = ptrTemp->ptrChildRight;
          ptrTemp->ptrChildRight = NULL;
        }
        if (ptrNode->ptrUp->ptrChildLeft == ptrNode)    /* must be */
        {
          ptrTemp->ptrChildLeft = ptrNode;
          ptrNode = ptrNode->ptrUp;
        }
      }
      ptrTemp->ptrChildRight = ptrNode;
      ptrNode->ptrChildLeft = ptrNode->ptrUp;
      ptrNode->ptrUp = ptrTemp;
      ptrNode = ptrTemp;
    }
    ptrNode->ptrChildLeft = ptrNode->ptrUp;
    while (ptrNode->ptrUp != NULL)
      ptrNode = ptrNode->ptrUp;
  }
  return ptrNode;
}

extern TreeNodePtr UnrootTree (TreeNodePtr ptrNode)
{
  TreeNodePtr ptrTemp;

  if (TestTree (ptrNode) == TREE_UNROOTED)
    return NULL;

  ptrNode = HeadNode (ptrNode);
  if (ptrNode != NULL)
  {
    ptrNode->ptrChildLeft->ptrUp = ptrNode->ptrChildRight;
    ptrNode->ptrChildRight->ptrUp = ptrNode->ptrChildLeft;
    ptrTemp = ptrNode->ptrChildLeft;
    ptrNode->ptrChildLeft = NULL;
    ptrNode->ptrChildRight = NULL;
    TreeNodeFree (ptrNode);
    ptrNode = ptrTemp;
  }
  return ptrNode;
}

extern TreeNodePtr TreeNodeNew (void)
{
  TreeNodePtr ptrNode;

  if ((ptrNode = (TreeNodePtr) MemNew (sizeof (TreeNode))) != NULL)
  {
    ptrNode->score = 0.0;
    ptrNode->flagPivot = FALSE;
    ptrNode->level = 0;
    ptrNode->nodenumber = 0;
    ptrNode->ptrUp = NULL;
    ptrNode->ptrChildLeft = NULL;
    ptrNode->ptrChildRight = NULL;
    ptrNode->ptrLevelLeft = NULL;
    ptrNode->ptrLevelRight = NULL;
  }
  return ptrNode;
}

extern TreeNodePtr TreeNodeFree (TreeNodePtr ptrNode)
{
  MemFree (ptrNode->name);
  return (TreeNodePtr) MemFree (ptrNode);
}

extern TreeNodePtr TreeNodeFreeBranch (TreeNodePtr ptrNode)
{
  if (ptrNode == NULL)
    return ptrNode;

  TreeNodeFreeBranch (ptrNode->ptrChildLeft);
  TreeNodeFreeBranch (ptrNode->ptrChildRight);

  return TreeNodeFree (ptrNode);
}

extern TreeNodePtr TreeNodeFreeTree (TreeNodePtr ptrNode)
{
  if (ptrNode == NULL)
    return ptrNode;

  return TreeNodeFreeBranch (HeadNode (ptrNode));
}
