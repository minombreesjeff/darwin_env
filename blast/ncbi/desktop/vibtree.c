#include <vibtree.h>
#include <gather.h>
#include <seqport.h>

static SegmenT DrawTree (TreeNodePtr ptrNode, SegmenT seg,
                         Int4 originX, Int4 originY, Int4 count)
{
  Uint2 primID = 1;
  Int4  pX, pY;

  if (count > 32)
    count += 2;
  else
    count += 4;
  if (count > 30)
    count = 30;
  if (ptrNode->flagPivot)
    count = 0;

  if (ptrNode->ptrChildLeft != NULL)
  {
    pX = originX-64+count+count;
    pY = originY-64+count;
    AddLine (seg, originX, originY, pX, pY, FALSE, primID);
    seg = DrawTree (ptrNode->ptrChildLeft, seg, pX, pY, count);
  }
  if (ptrNode->ptrChildRight != NULL)
  {
    if (ptrNode->flagPivot)
      count = 0;
    pX = originX+64-count-count;
    pY = originY-64+count;
    AddLine (seg, originX, originY, pX, pY, FALSE, primID);
    seg = DrawTree (ptrNode->ptrChildRight, seg, pX, pY, count);
  }
  if (ptrNode->ptrChildLeft == NULL && ptrNode->ptrChildRight == NULL)
  {
    AddTextLabel (seg, originX, originY, ptrNode->name,
                  NULL, 0, LOWER_CENTER, 0);
  }
  return seg;
}

extern SegmenT GeneTreeDraw (TreeNodePtr ptrNode, SegmenT seg)
{
  if (ptrNode == NULL)
    return seg;

  if (seg == NULL)
    if ((seg = CreatePicture ()) == NULL)
      return seg;

  seg = DrawTree (ptrNode, seg, 0, 0, 0);

  return seg;
}
