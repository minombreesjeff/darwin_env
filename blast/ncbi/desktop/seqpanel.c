/* $Id: seqpanel.c,v 6.34 2004/04/13 16:52:26 bollin Exp $
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
* Author:  Vlad Lebedev
*
* Comment: A single implementation for sequence and alignment viewers based
*          on feature indexing and Alignment Manager.
* ==========================================================================
*/

#include <bspview.h>
#include <seqpanel.h>

#include <objmgr.h>
#include <explore.h>

#include <alignmgr2.h>



enum ESeqNum   { eNumNone=1, eNumSide=2, eNumTop=3 };
enum ELineType { eTypeTopSeqNumbers, eTypeTopScaleMarks, eTypeSequence, 
                 eTypeAlignSequence, eTypeFeature };
enum EDrawGrid { eDrawGridOn=1, eDrawGridOff=2 };
enum EShowFeatures { eShowFeaturesOn=1, eShowFeaturesOff=2, eShowFeaturesAll=3 };

#define GRID_LINE_SPACE       6
#define NO_GRID_LINE_SPACE    2
#define SEQ_GROUP_SIZE       10  /* Sequence group size           */
#define SEQ_X_OFFSET          4
#define SEQ_Y_OFFSET          4
#define ALNMGR_GAP           -2
#define ROW_UNDEFINED        -1
#define PROT_PRODUCT_TYPE   255
#define GAP_CHAR            '.'

static Uint1 FillRectangleSym [] = { 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x00 };
static Uint1 FillLeftArrowSym [] = { 0x06, 0x1E, 0x7E, 0xFE, 0x7E, 0x1E, 0x06, 0x00 };
static Uint1 FillRightArrowSym[] = { 0xC0, 0xF0, 0xFC, 0xFE, 0xFC, 0xF0, 0xC0, 0x00 };

typedef struct seqParag { 
  ValNodePtr PNTR pFeatList;  /* pointer to an array of lists of feature indexes */ 
} SeqParaG, PNTR SeqParaGPtr;


static BioseqViewPtr GetBioseqViewPtr(PaneL p)
{
  return &((BioseqViewFormPtr) GetObjectExtra (p))->bvd;
}

static void FreeSeqPanelLines(SeqPanLinePtr PNTR splp, BioseqViewPtr bvp)
{
  Int4 i;
  for (i = 0; i < bvp->TotalLines; i++) MemFree(splp[i]);
  MemFree (splp);
}


static SeqPanLinePtr MakeSeqPanLine(Int2 type, Int4 line)
{
  SeqPanLinePtr plp = (SeqPanLinePtr)MemNew(sizeof(SeqPanLine));
  plp->lineType     = type;
  plp->bioSeqLine   = line;
  return plp;
}


static void ShowSeqView (BioseqViewPtr bvp, Boolean show)
{
  Int2 val = GetValue(bvp->newFeatControl);
  
  show ? SafeShow (bvp->seqView         ) : SafeHide (bvp->seqView         );
  show ? SafeShow (bvp->seqViewParentGrp) : SafeHide (bvp->seqViewParentGrp);
  show ? SafeShow (bvp->clickMe         ) : SafeHide (bvp->clickMe         );
  SafeHide (bvp->styleControlGrp);
  SafeHide (bvp->scaleControlGrp);
  SafeHide (bvp->findGeneGrp    );
  
  SafeHide (bvp->newFeatControl);
  Reset(bvp->newFeatControl);
  if (bvp->seqAlignMode) {
    PopupItem (bvp->newFeatControl, "Target");
    PopupItem (bvp->newFeatControl, "Off");
    PopupItem (bvp->newFeatControl, "Aligned");
  } else {
    PopupItem (bvp->newFeatControl, "Target");
    PopupItem (bvp->newFeatControl, "Off");
    if (val == eShowFeaturesAll) val = eShowFeaturesOn;
  }
  SetValue (bvp->newFeatControl, val);
  SafeShow (bvp->newFeatControl);
}


static void SelectSeqView (BioseqViewPtr bvp, Uint2 selentityID, Uint2 selitemID,
            Uint2 selitemtype, SeqLocPtr region, Boolean select, Boolean scrollto)
{
  RecT r_redraw;
  ValNodePtr vnp;
  Boolean found = FALSE;

  if (selitemtype != OBJ_SEQFEAT) return;

  for (vnp = bvp->Selection; vnp != NULL; vnp = vnp->next) {
    if (vnp->data.intvalue == selitemID) {
      vnp->choice = select;
      found = TRUE;
      break;
     }
  }
  
  if (select && !found) ValNodeAddInt(&bvp->Selection, 1, selitemID);
  
  UseWindow (ParentWindow(bvp->seqView));
  ObjectRect (bvp->seqView, &r_redraw);
  InvalRect (&r_redraw);
}


static Int4 x_Coord(BioseqViewPtr bvp, Int4 pos, Int4 row)  
{
  if (bvp->seqAlignMode) {
    Int4 tmp = AlnMgr2MapBioseqToSeqAlign(bvp->salp, pos, row);
    return tmp;
  }
  else return pos;
}


static Int4 x_FeatLeft(BioseqViewPtr bvp, Int4 fpos)  /* trim feature to fit alignment */
{
  if (bvp->seqAlignMode) return MAX(0, fpos);
  else return fpos;
}

static Int4 x_FeatRight(BioseqViewPtr bvp, Int4 fpos)  /* trim feature to fit alignment */
{
  if (bvp->seqAlignMode) {
    Int4 aln_len = AlnMgr2GetAlnLength(bvp->salp, FALSE);
    return MIN(aln_len-1, fpos);
  } else return fpos;
}

static void FillFeatureInfo(BioseqViewPtr bvp, BioseqPtr bsp, Int2 lineLength, Int4 pCount, Int4 row, Int4 idx, SeqParaGPtr PNTR ref)
{
  Int4 j;
  SeqMgrFeatContext fcontext;
  SeqFeatPtr        sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);

  while (sfp != NULL) {
    if (fcontext.seqfeattype != SEQFEAT_PUB  &&  
        fcontext.seqfeattype != SEQFEAT_BIOSRC  &&
       (fcontext.left        != fcontext.right)) {
      Boolean coding   = fcontext.seqfeattype == SEQFEAT_CDREGION;
      Int4    fLeft    = x_FeatLeft (bvp, fcontext.left );
      Int4    fRight   = x_FeatRight(bvp, fcontext.right);      
      Int4    paraFrom = floor(x_Coord(bvp, fLeft,  row) / lineLength);      /* feature starting paragraph */
      Int4    paraTo   = ceil (x_Coord(bvp, fRight, row) / lineLength) + 1;  /* feature ending paragraph   */
      /*BioseqPtr bsp_prot = BioseqFind (SeqLocId(sfp->product));*/

      for (j = paraFrom; j != paraTo  &&  j < pCount; j++) {
        ValNodeAddInt(&ref[j]->pFeatList[idx], fcontext.seqfeattype, fcontext.itemID);
        if (coding) ValNodeAddInt(&ref[j]->pFeatList[idx], PROT_PRODUCT_TYPE, fcontext.itemID); /* add space for prot product */
      }
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);      
  }  /* while */
}


static void MakeFeatureLine(Int4Ptr lCount, Int4 para, Int4 row, Int4 idx, SeqParaGPtr PNTR ref, SeqPanLinePtr PNTR splp)
{
  ValNodePtr vnp;
  for (vnp = ref[para]->pFeatList[idx]; vnp != NULL; vnp = vnp->next) {
    SeqPanLinePtr plp = MakeSeqPanLine(eTypeFeature, para);
    plp->idx          = vnp->data.intvalue;
    plp->protProduct  = vnp->choice==PROT_PRODUCT_TYPE ? TRUE : FALSE;
    plp->row          = row;
    splp[*lCount]     = plp;
    (*lCount)++;
  }
}


static SeqPanLinePtr PNTR CreateSeqPanelLines(Int2 lineLength, BioseqViewPtr bvp)
{
  BioseqPtr          bsp = bvp->bsp;

  SeqParaGPtr   PNTR ref;
  SeqPanLinePtr PNTR splp;
  Int2               fLines;
  Int4               lCount  = 0, i, j;
  Int4               pCount;                     /* Total number of paragraphs */
  Int4               alnRows = -1;
  Int4               alnValidRows = 0;
  Int4               featRows = 1;
  
  if (bvp->seqAlignMode) {
    fLines  = GetValue(bvp->newNumControl) == eNumTop ? 2 : 0;  
    alnRows = AlnMgr2GetNumRows(bvp->salp);  /* size of the alignment */
    if (bvp->viewWholeEntity  ||  GetValue(bvp->newFeatControl) == eShowFeaturesAll) featRows = alnRows; /* show features for all rows */
    pCount  = floor((AlnMgr2GetAlnLength(bvp->salp, FALSE)-1) / lineLength) + 1; /* alignment length */ 
    for (j = 1; j != alnRows + 1; j++) {  /* AlnMgr counts from 1, not 0    */
      SeqIdPtr  tmp_sip = AlnMgr2GetNthSeqIdPtr(bvp->salp, j);
      BioseqPtr tmp_bsp = BioseqLockById(tmp_sip);
      if (tmp_bsp != NULL) {
        alnValidRows++;  /* count avaliable alignments */
        BioseqUnlock (tmp_bsp);
      }
      SeqIdFree (tmp_sip);
    }
  } else {
    fLines  = GetValue(bvp->newNumControl) == eNumTop ? 3 : 1;  
    pCount  = floor(bsp->length / lineLength) + 1; /* sequence length */ 
  }
    
  ref = (SeqParaGPtr*) MemNew( (size_t)(sizeof(SeqParaGPtr)*pCount) );
  for (i = 0; i < pCount; i++)  {
    ref[i] = (SeqParaGPtr) MemNew(sizeof(SeqParaG));
    ref[i]->pFeatList = (ValNodePtr*) MemNew( (size_t)(sizeof(ValNodePtr)*featRows) );
  }
  
  if (GetValue(bvp->newFeatControl) != eShowFeaturesOff) {
    if (bvp->seqAlignMode) {  /* in alignment mode */
      for (i = 1; i != alnRows + 1; i++) {
        SeqIdPtr  sip_tmp = AlnMgr2GetNthSeqIdPtr(bvp->salp, i);
        BioseqPtr bsp_tmp = BioseqLockById(sip_tmp);

        if (bsp_tmp != NULL) {
          if (bvp->viewWholeEntity  ||  GetValue(bvp->newFeatControl) == eShowFeaturesAll) {
            FillFeatureInfo(bvp, bsp_tmp, lineLength, pCount, i, i - 1, ref);  /* show features for each seq in alignment */
          } else if (i == bvp->TargetRow) {
            FillFeatureInfo(bvp, bsp_tmp, lineLength, pCount, i, 0,     ref);  /* show features for target seq in alignment */
          }
          BioseqUnlock (bsp_tmp);
        }  /* bsp_tmp != NULL */
        SeqIdFree    (sip_tmp);
      }  /* for */
    } else {
      FillFeatureInfo(bvp, bsp, lineLength, pCount, 0, 0, ref);
    }  
  }  /* done with features */



  bvp->TotalLines = 0; /* go through all pararaphs and count total */
  for (i = 0; i < pCount; i++) {
    Int4 sub_total = 0;
    for (j = 0; j < featRows; j++) sub_total += ValNodeLen(ref[i]->pFeatList[j]); 
    bvp->TotalLines += fLines + sub_total + (alnRows == -1 ? 0 : alnValidRows); /* reserve space for alignment */
  }

  splp = (SeqPanLinePtr*) MemNew( (size_t)(sizeof(SeqPanLinePtr)*bvp->TotalLines) );
  for (i = 0; i < pCount; i++) {
    if (fLines > 1) { /* Numbers on top */
      splp[lCount++] = MakeSeqPanLine(eTypeTopSeqNumbers, i);
      splp[lCount++] = MakeSeqPanLine(eTypeTopScaleMarks, i);
    }
    
    if (bvp->seqAlignMode) {  
      for (j = 1; j != alnRows + 1; j++) {  /* AlnMgr counts from 1, not 0    */
        SeqIdPtr  tmp_sip = AlnMgr2GetNthSeqIdPtr(bvp->salp, j);
        BioseqPtr tmp_bsp = BioseqLockById(tmp_sip);
        
        if (tmp_bsp != NULL) {
          SeqPanLinePtr plp = MakeSeqPanLine(eTypeAlignSequence, i); /* Add align sequence line*/
          plp->row          = j;  /* Index position in the alignment (row) */
          splp[lCount++]    = plp;
          BioseqUnlock (tmp_bsp);
          
          if (bvp->viewWholeEntity  ||  GetValue(bvp->newFeatControl) == eShowFeaturesAll) {
            MakeFeatureLine(&lCount, i, j, j - 1, ref, splp); /* Add feature line       */
          } else if (j == bvp->TargetRow) {
            MakeFeatureLine(&lCount, i, j, 0,     ref, splp);
          }
        }
        SeqIdFree(tmp_sip);
      }
    } else {  /* Add usual sequence and features if not in alignment mode */
      splp[lCount++] = MakeSeqPanLine(eTypeSequence, i);  /* Add sequence line      */
      MakeFeatureLine(&lCount, i, 0, 0, ref, splp);      /* Add feature line       */
    }  /* bvp->seqAlignMode */
  }  /* for */

  for (i = 0; i < pCount; i++) {
    for (j = 0; j < featRows; j++) ValNodeFree (ref[i]->pFeatList[j]);
    MemFree (ref[i]->pFeatList);
    MemFree (ref[i]);
  }
  MemFree (ref);
  return splp;
}



static void onSeqViewClick (PaneL p, PoinT pt)
{
  BioseqViewPtr bvp;

  bvp  = GetBioseqViewPtr (p);
  bvp->wasDoubleClick = dblClick;
  bvp->wasShiftKey = shftKey;
  bvp->old_rect_shown = FALSE;
  bvp->pnt_start = pt;
  bvp->pnt_stop = pt;
}


static void onSeqViewRelease (PaneL p, PoinT pt)
{
  RecT          r;
  BaR           sb;
  BioseqPtr     bsp;
  BioseqViewPtr bvp;
  SeqPanLinePtr splp;
  Int4          line;
  Uint2         entityID = 0;
  Uint4         itemID = 0;
  Uint2         itemtype = 0;
  SeqEntryPtr   sep;

  bvp  = GetBioseqViewPtr (p);
  bsp  = bvp->bsp;
  sb   = GetSlateVScrollBar ((SlatE)bvp->seqView);

  ObjectRect (bvp->seqView, &r);
  InsetRect (&r, 4, 4);

  line = (pt.y-r.top-SEQ_Y_OFFSET) / bvp->LineHeight + GetBarValue(sb);
  if(line >= bvp->TotalLines) return;
  
  splp = bvp->SeqPanLines[line];
  switch (splp->lineType)  {
    case eTypeSequence:
      /*
      printf("Sequence from %d to %d\n", splp->bioSeqLine*bvp->CharsAtLine+1, splp->bioSeqLine*bvp->CharsAtLine+bvp->CharsAtLine);
      */
      break;
    case eTypeFeature:
      /*
      if (splp->protProduct) printf("Prot. product: %d\n", splp->idx);
      else printf("Feature. Index: %d\n", splp->idx);
      */
      if (splp->protProduct) {
        /* just get CDS feature from protein product */
        entityID = ObjMgrGetEntityIDForPointer (bsp);
        itemID = splp->idx;
        itemtype = OBJ_SEQFEAT;
      } else {
        entityID = ObjMgrGetEntityIDForPointer (bsp);
        itemID = splp->idx;
        itemtype = OBJ_SEQFEAT;
      }
      break;
  }
  if (itemID < 1) {
    if (! bvp->wasShiftKey) {
      ObjMgrDeSelect (0, 0, 0, 0, NULL);
    }
    return;
  }

  if (bvp->wasDoubleClick) {
      sep = GetTopSeqEntryForEntityID (entityID);
      if (bvp->launchSubviewers) {
        WatchCursor ();
        Update ();
        LaunchNewBioseqViewer (bvp->bsp, entityID, itemID, itemtype);
        ArrowCursor ();
        Update ();
        return;
      } else if (LaunchViewerNotEditor (bvp, sep, entityID, itemID, itemtype)) {
        WatchCursor ();
        Update ();
        LaunchNewBioseqViewer (bvp->bsp, entityID, itemID, itemtype);
        ArrowCursor ();
        Update ();
        return;
      } else if (bvp->launchEditors) {
        WatchCursor ();
        Update ();
        GatherProcLaunch (OMPROC_EDIT, FALSE, entityID, itemID,
                          itemtype, 0, 0, itemtype, 0);
        ArrowCursor ();
        Update ();
        return;
      } else {
        return;
      }
    if (! bvp->sendSelectMessages) return;
    if (bvp->wasShiftKey) {
      ObjMgrAlsoSelect (entityID, itemID, itemtype, 0, NULL);
    } else {
      ObjMgrSelect (entityID, itemID, itemtype, 0, NULL);
    }
  } else if (bvp->sendSelectMessages) {
      if (bvp->wasShiftKey) {
        ObjMgrAlsoSelect (entityID, itemID, itemtype, 0, NULL);
      } else {
        ObjMgrSelect (entityID, itemID, itemtype, 0, NULL);
      }
  }
}


static void ResizeSeqView (BioseqViewPtr bvp)
{
  RecT r;
  Int4 height, width;
  BaR  sb = GetSlateVScrollBar ((SlatE)bvp->seqView);
  Int4 bar_max;

  ObjectRect (bvp->seqView, &r);
  InsetRect (&r, 4, 4);
  width  = r.right  - r.left;
  height = r.bottom - r.top - 2 * SEQ_Y_OFFSET;

  bvp->BlocksAtLine = (width - SEQ_X_OFFSET - bvp->SeqStartPosX) / ((SEQ_GROUP_SIZE + 1) * bvp->CharWidth);
  if (bvp->BlocksAtLine == 0) bvp->BlocksAtLine = 1; /* Always at least 1 block */

  bvp->CharsAtLine = SEQ_GROUP_SIZE * bvp->BlocksAtLine;

  if (bvp->SeqPanLines) { FreeSeqPanelLines (bvp->SeqPanLines, bvp); bvp->SeqPanLines = NULL; }
  bvp->SeqPanLines = CreateSeqPanelLines (bvp->CharsAtLine, bvp);

  bar_max = bvp->TotalLines - height / bvp->LineHeight;
  /* If height is evenly divisible by bvp->LineHeight, bar_max is too small */
  if (height % bvp->LineHeight == 0) {
    bar_max ++;
  }
  SetBarMax (sb, bar_max);
  CorrectBarPage(sb, (height / bvp->LineHeight) - 1, (height / bvp->LineHeight) - 1);

  SetPanelClick(bvp->seqView, onSeqViewClick, NULL, NULL, onSeqViewRelease);
}


static void PopulateSeqAlnView (BioseqViewPtr bvp)
{
  RecT r;
  SelectFont ((FonT)(bvp->displayFont));

  
  bvp->DrawGrid   = GetValue (bvp->newGridControl) == eDrawGridOn;
  bvp->LineSpace  = bvp->DrawGrid ? GRID_LINE_SPACE-1 : NO_GRID_LINE_SPACE;
  bvp->CharHeight = FontHeight ();
  bvp->CharWidth  = CharWidth ('A');
  bvp->LineHeight = bvp->CharHeight + bvp->LineSpace;
  
  CorrectBarValue (GetSlateVScrollBar ((SlatE)bvp->seqView), 0); 
  ResizeSeqView (bvp);  

  Select (bvp->seqView);
  ObjectRect (bvp->seqView, &r);
  InsetRect (&r, 2, 2);
  InvalRect (&r);

  SetPanelClick(bvp->seqView, onSeqViewClick, NULL, NULL, onSeqViewRelease);
}


static void PopulateSeqView (BioseqViewPtr bvp)
{
  bvp->seqAlignMode = FALSE;
  bvp->SeqStartPosX = 100;
  PopulateSeqAlnView(bvp);
}


static void PopulateAlnView (BioseqViewPtr bvp)
{
  SeqIdPtr sip = bvp->bsp->id;
  bvp->seqAlignMode = TRUE;
  bvp->SeqStartPosX = 150;
  if (bvp->salp == NULL  &&  bvp->seqAlignMode) { /* Try to find an alignment */
    bvp->salp = SeqAlignListDup((SeqAlignPtr) FindSeqAlignInSeqEntry(bvp->bsp->seqentry, OBJ_SEQALIGN));
    if (bvp->salp == NULL) {  /* No alignment found or bug in AlignMgr (which is more likely). Switch to sequence mode */
      PopulateSeqView(bvp);
      return;
    }
    if (bvp->salp->segtype == SAS_DENSEG  &&  bvp->salp->next == NULL) {
      AlnMgr2IndexSingleChildSeqAlign(bvp->salp);
    } else {
      AlnMgr2IndexSeqAlign(bvp->salp);
    }
  }
  bvp->TargetRow = ROW_UNDEFINED;
  while (sip  &&  bvp->TargetRow == ROW_UNDEFINED) {
    bvp->TargetRow = AlnMgr2GetFirstNForSip(bvp->salp, sip);
    sip = sip->next;
  }

  PopulateSeqAlnView(bvp);
}





static void onCloseSeqPanel (PaneL p)
{
  BioseqViewPtr bvp;
  bvp  = GetBioseqViewPtr (p);

  if (bvp->SeqPanLines) FreeSeqPanelLines (bvp->SeqPanLines, bvp);
  bvp->SeqPanLines = NULL;

  ValNodeFree(bvp->Selection);
}


static void DrawTopScaleMarks(Int2 x, Int2 y, Int4 line, BioseqViewPtr bvp)
{
  Int2 block, ctr=0;
  
  Magenta ();
  for (block = 0;  block != bvp->BlocksAtLine  &&  ctr >= 0;  block++) {
    MoveTo(x+SEQ_X_OFFSET+bvp->SeqStartPosX+(block+1)*SEQ_GROUP_SIZE*bvp->CharWidth+block*bvp->CharWidth - bvp->CharWidth/2 - 1, y);
    LineTo(x+SEQ_X_OFFSET+bvp->SeqStartPosX+(block+1)*SEQ_GROUP_SIZE*bvp->CharWidth+block*bvp->CharWidth - bvp->CharWidth/2 - 1, y-bvp->CharHeight);

    MoveTo(x+SEQ_X_OFFSET+bvp->SeqStartPosX+(block+1)*SEQ_GROUP_SIZE*bvp->CharWidth+block*bvp->CharWidth - SEQ_GROUP_SIZE*bvp->CharWidth/2 - bvp->CharWidth/2 - 1, y);
    LineTo(x+SEQ_X_OFFSET+bvp->SeqStartPosX+(block+1)*SEQ_GROUP_SIZE*bvp->CharWidth+block*bvp->CharWidth - SEQ_GROUP_SIZE*bvp->CharWidth/2 - bvp->CharWidth/2 - 1, y-bvp->CharHeight/2);
  }
}

static void DrawTopSeqNums(Int2 x, Int2 y, Int4 line, BioseqViewPtr bvp)
{
  Int2 block, ctr=0;
  char buf[20];
  
  Magenta ();
  for (block = 0;  block != bvp->BlocksAtLine  &&  ctr >= 0;  block++) {
    sprintf(buf, "%d", line * bvp->CharsAtLine + (block+1)*SEQ_GROUP_SIZE);
    PaintStringEx (buf, x+SEQ_X_OFFSET+bvp->SeqStartPosX+(block+1)*SEQ_GROUP_SIZE*bvp->CharWidth+block*bvp->CharWidth - bvp->CharWidth*StrLen(buf), y);
  }
}


static void DrawSeqSideLineNumbers(Int2 x, Int2 y, Int4 line, BioseqViewPtr bvp)
{
  char buf[20];
  
  sprintf(buf, "%d", line * bvp->CharsAtLine + 1);    /* In sequence coordinates  */

  Magenta ();
  PaintStringEx (buf, x+ SEQ_X_OFFSET + (bvp->SeqStartPosX-30-bvp->CharWidth*StrLen(buf)), y);
}


static void DrawAlignSideLineNumbers(Int2 x, Int2 y, Int4 line, Int4 row, BioseqViewPtr bvp)
{
  char buf[20];
  Int4 aln_pos = line * bvp->CharsAtLine + 1;
  Int4 pos = AlnMgr2MapSeqAlignToBioseq(bvp->salp, aln_pos, row);
  
  while (pos == ALNMGR_GAP && aln_pos > 1) { /* count back if we in the gap */
    aln_pos--;
    pos = AlnMgr2MapSeqAlignToBioseq(bvp->salp, aln_pos, row);
  }
  if (pos == ALNMGR_GAP) pos = 1;  /* Gap at the begining of the alignment */
  
  sprintf(buf, "%d", pos); /* In alignment coordinates */

  Magenta ();
  PaintStringEx (buf, x+ SEQ_X_OFFSET + (bvp->SeqStartPosX-30-bvp->CharWidth*StrLen(buf)), y);
}


static SeqPortPtr SeqPortFromAlignmentInterval (Int4 seqstart, Int4 seqstop, Uint1 strand, BioseqPtr bsp)
{
  SeqIntPtr  sinp;
  SeqLocPtr  slp;
  SeqPortPtr spp;

  if (bsp == NULL) return NULL;
  sinp = SeqIntNew();
  if (sinp == NULL) return NULL;
  sinp->from = seqstart;
  sinp->to = seqstop;
  sinp->strand = strand;
  sinp->id = SeqIdDup (SeqIdFindBest (bsp->id, 0));
  slp = ValNodeNew (NULL);
  if (slp == NULL) {
    SeqIntFree (sinp);
    return NULL;
  }
  slp->choice = SEQLOC_INT;
  slp->data.ptrvalue = (Pointer) sinp;
  spp = SeqPortNewByLoc (slp, Seq_code_iupacna);
  SeqLocFree (slp);
  return spp;
}

static void SetSequenceIntervalBuf
(SeqAlignPtr salp,
 BioseqPtr   bsp,
 Int4        row,
 Int4        start,                             
 Int4        stop,
 Int4Ptr     seqstart,
 Int4Ptr     seqstop,
 Int4        aln_len,                             
 Uint1Ptr    target_buf)
{
  Int4       buf_len = stop - start + 1;
  Uint1      strand;
  Int4       i;
  SeqPortPtr spp;

  strand = SeqAlignStrand (salp, row - 1);
  MemSet (target_buf, 0, buf_len);
  /* if this is a minus strand sequence, start is stop and stop is start */
  if (strand == Seq_strand_minus) {
    *seqstop = AlnMgr2MapSeqAlignToBioseq(salp, start, row);
    *seqstart  = AlnMgr2MapSeqAlignToBioseq(salp, stop, row);
  } else {
    *seqstart = AlnMgr2MapSeqAlignToBioseq(salp, start, row);
    *seqstop  = AlnMgr2MapSeqAlignToBioseq(salp, stop, row);
  }

  if (strand == Seq_strand_minus) {
    i = stop;
    while (*seqstart == ALNMGR_GAP && i > 0) { /* count backward if we are in the gap */
      i--;
      *seqstart = AlnMgr2MapSeqAlignToBioseq(salp, i, row);
    }
  } else {
    i = start;
    while (*seqstart == ALNMGR_GAP && i < aln_len) { /* count forward if we in the gap */
      i++;
      *seqstart = AlnMgr2MapSeqAlignToBioseq(salp, i, row);
    }
  }
  if (*seqstop == -1 || *seqstop>=bsp->length) *seqstop = bsp->length - 1;  /* -1 means exeed sequence length */
  
  if (strand == Seq_strand_minus) {
    i = start;
    while (*seqstop == ALNMGR_GAP && i > 0) { /* count backward if we are in the gap */
      i--;
      *seqstop = AlnMgr2MapSeqAlignToBioseq(salp, i, row);
    }
  } else {
    i = stop;
    while (*seqstop == ALNMGR_GAP && i < aln_len) { /* count forward if we are in the gap */
      i++;
      *seqstop = AlnMgr2MapSeqAlignToBioseq(salp, i, row);
    }
  }
  
  if (*seqstart == ALNMGR_GAP  &&  *seqstop == ALNMGR_GAP) {
    return;
  }
  if (*seqstop  < 0) *seqstop  = bsp->length - 1;
  if (*seqstart < 0) *seqstart = *seqstop;
  if (strand == Seq_strand_minus) {
    if (*seqstop - *seqstart > buf_len) 
      *seqstart = *seqstop - buf_len;
  } else {
    if (*seqstop - *seqstart > buf_len) *seqstop = *seqstart + buf_len;  /* not to exeed the current line */
  }

  spp = SeqPortFromAlignmentInterval (*seqstart, *seqstop, strand, bsp);
  SeqPortRead  (spp, target_buf, *seqstop - *seqstart + 1);
  SeqPortFree  (spp);
}


extern void 
AlignmentIntervalToString 
(SeqAlignPtr salp,
 Int4        row,
 Int4        start,
 Int4        stop,
 Int4        target_row,
 Boolean     view_whole_entity,
 Uint1Ptr    seqbuf,
 Uint1Ptr    alnbuf,
 Int4 PNTR   alnbuffer_len)
{
  Int4       aln_len = AlnMgr2GetAlnLength(salp, FALSE);
  SeqIdPtr   sip     = AlnMgr2GetNthSeqIdPtr(salp, row);
  BioseqPtr  bsp     = BioseqLockById(sip);
  Int4       alnbuf_len = stop - start + 1;
  Uint1      strand;
  Int4       seqstart, seqstop;
  Int4       i, k;
  SeqPortPtr spp;
  Int4       seq_len;
  Uint1      target_strand;
  SeqIdPtr   sip_target;
  BioseqPtr  bsp_target;
  Int4       target_start;
  Int4       target_stop;
  Uint1Ptr   target_buf;
  Int4       aln_pos;

  MemSet(alnbuf, '-', alnbuf_len); /* assume all gaps and fill the sequence later */
  MemSet(seqbuf, 0, alnbuf_len);

  if (stop > aln_len) {
    MemSet (alnbuf + aln_len - start, 0, stop - aln_len);
    stop = aln_len - 1;
    alnbuf_len = stop - start + 1;
  }

  if (alnbuffer_len != NULL) {
    *alnbuffer_len = alnbuf_len;
  }

  strand = SeqAlignStrand (salp, row - 1);
  target_strand = SeqAlignStrand (salp, target_row - 1);
  /* if this is a minus strand sequence, start is stop and stop is start */
  if (strand == Seq_strand_minus) {
    seqstop = AlnMgr2MapSeqAlignToBioseq(salp, start, row);
    seqstart  = AlnMgr2MapSeqAlignToBioseq(salp, stop,  row);
  } else {
    seqstart = AlnMgr2MapSeqAlignToBioseq(salp, start, row);
    seqstop  = AlnMgr2MapSeqAlignToBioseq(salp, stop,  row);
  }

  if (strand == Seq_strand_minus) {
    i = stop;
    while (seqstart == ALNMGR_GAP && i > 0) { /* count backward if we are in the gap */
      i--;
      seqstart = AlnMgr2MapSeqAlignToBioseq(salp, i, row);
    }
  } else {
    i = start;
    while (seqstart == ALNMGR_GAP && i < aln_len) { /* count forward if we in the gap */
      i++;
      seqstart = AlnMgr2MapSeqAlignToBioseq(salp, i, row);
    }
  }
  if (seqstop == -1 || seqstop>=bsp->length) seqstop = bsp->length - 1;  /* -1 means exeed sequence length */
  
  if (strand == Seq_strand_minus) {
    i = start;
    while (seqstop == ALNMGR_GAP && i > 0) { /* count backward if we are in the gap */
      i--;
      seqstop = AlnMgr2MapSeqAlignToBioseq(salp, i, row);
    }
  } else {
    i = stop;
    while (seqstop == ALNMGR_GAP && i < aln_len) { /* count forward if we are in the gap */
      i++;
      seqstop = AlnMgr2MapSeqAlignToBioseq(salp, i, row);
    }
  }
  
  
  if (seqstart == ALNMGR_GAP  &&  seqstop == ALNMGR_GAP) seqstart = seqstop = 0;  /* whole line are gaps */
  if (seqstop  < 0) seqstop  = bsp->length - 1;
  if (seqstart < 0) seqstart = seqstop;
  if (strand == Seq_strand_minus) {
    if (seqstop - seqstart > alnbuf_len) 
      seqstart = seqstop - alnbuf_len;
  } else {
    if (seqstop - seqstart > alnbuf_len) seqstop = seqstart + alnbuf_len;  /* not to exeed the current line */
  }


  spp = SeqPortFromAlignmentInterval (seqstart, seqstop, strand, bsp);
  SeqPortRead  (spp, seqbuf, seqstop - seqstart + 1);
  if (seqbuf [stop - start] == 0) {
    seq_len = StringLen ((CharPtr) seqbuf);
  } else {
    seq_len = stop - start + 1;
  }
  SeqPortFree  (spp);
  BioseqUnlock (bsp);
  SeqIdFree    (sip);

  if (row != target_row  &&  ! view_whole_entity  &&  target_row != ROW_UNDEFINED)  {
    sip_target = AlnMgr2GetNthSeqIdPtr(salp, target_row);
    bsp_target = BioseqLockById(sip_target);

    target_buf = (Uint1Ptr) MemNew (stop - start + 1);
    MemSet (target_buf, 0, stop - start + 1);
    if (target_buf != NULL) {
      SetSequenceIntervalBuf (salp, bsp_target, target_row, start, stop, 
                              &target_start, &target_stop, aln_len, target_buf);
    }
  } else {
    sip_target = NULL;
    bsp_target = NULL;
    target_buf = NULL;
  }

  k = 0;
  i = 0;

  for (aln_pos = start; aln_pos <= stop; aln_pos ++) {
    Int4 seq_pos = AlnMgr2MapSeqAlignToBioseq(salp, aln_pos, row);
    Int4 target_pos = AlnMgr2MapSeqAlignToBioseq(salp, aln_pos, target_row);

    if (seq_pos >= 0) {
      alnbuf [aln_pos - start] = TO_LOWER (seqbuf[k]);
      /* Handle mismatches (insert dots when matched) */
      if (row != target_row  &&  ! view_whole_entity  &&  target_row != ROW_UNDEFINED)  {
        if(target_pos >= 0  && target_pos < bsp_target->length) { /* no gap in the target sequence */
          if (seqbuf[k] == target_buf[i]) {
            alnbuf[aln_pos - start] = GAP_CHAR;
          }
        }
      } /* mismatches */
      k++;
    }
    if (target_pos >= 0) {
      i++;
    }
  }    

  if (alnbuf[alnbuf_len] == 0) {
    *alnbuffer_len = StringLen ((CharPtr) alnbuf);
  }

  if (bsp_target != NULL) {
    BioseqUnlock (bsp_target);
  }
  if (sip_target != NULL) {
    SeqIdFree (sip_target);
  }
  if (target_buf != NULL) {
    MemFree (target_buf);
  }
}

static void DrawAlignment(Int2 x, Int2 y, Int4 line, Int4 row, Uint1Ptr buf, Uint1Ptr seqbuf, Uint1Ptr alnbuf, BioseqViewPtr bvp)
{
  Int2       block;
  Char       alnlabel[13];
  SeqIdPtr   sip     = AlnMgr2GetNthSeqIdPtr(bvp->salp, row);
  BioseqPtr  bsp     = BioseqLockById(sip);
  Int4       start   = line * bvp->CharsAtLine;
  Int4       stop    = start + bvp->BlocksAtLine * SEQ_GROUP_SIZE;
  Int4       alnbuf_len;
  SeqIdPtr   best_id;

  AlignmentIntervalToString (bvp->salp, row, start, stop, bvp->TargetRow,
                             bvp->viewWholeEntity, seqbuf, alnbuf, &alnbuf_len);

  
  /* finally draw everything */
  best_id = SeqIdFindBestAccession (bsp->id);
  SeqIdWrite (best_id, alnlabel, PRINTID_TEXTID_ACCESSION, 12);        /* Get label           */
  if (row == bvp->TargetRow  &&  !bvp->viewWholeEntity) Red();         /* Highlight current   */
  else Black();
  PaintStringEx (alnlabel, x+10, y);                                   /* Draw sequence label */

  Black ();
  block = 0;
  for (block = 0;  block != bvp->BlocksAtLine;  block++) {
    Int4 len  = SEQ_GROUP_SIZE;
    Int4 from = block * SEQ_GROUP_SIZE;
    
    if (from >= alnbuf_len) break;
    if (from + SEQ_GROUP_SIZE > alnbuf_len) len = alnbuf_len - from;
    
    MemCpy(buf, alnbuf + from, len);
    MemSet(buf+len, '\0', 1);
    PaintStringEx ( (CharPtr)buf, x+SEQ_X_OFFSET+bvp->SeqStartPosX+block*SEQ_GROUP_SIZE*bvp->CharWidth+block*bvp->CharWidth, y);
  }
  BioseqUnlock (bsp);
}  /* DrawAlignment*/



static void DrawSequence(Int2 x, Int2 y, Int4 line, SeqPortPtr spp, Uint1Ptr buf, BioseqViewPtr bvp)
{
  Int2 block, ctr = 0, i;

  Black ();
  for (block = 0;  block != bvp->BlocksAtLine  &&  ctr >= 0;  block++) {
    SeqPortSeek (spp, line*bvp->CharsAtLine+block*SEQ_GROUP_SIZE, SEEK_SET);
    ctr = SeqPortRead (spp, buf, SEQ_GROUP_SIZE);
    if (ctr > 0) {
      if (ctr < SEQ_GROUP_SIZE) MemSet(buf+ctr, '\0', 1);
      for (i = 0; i < ctr; i++) buf[i] = TO_LOWER (buf[i]);  /* convert to lowercase */
      PaintStringEx ( (CharPtr)buf, x+SEQ_X_OFFSET+bvp->SeqStartPosX+block*SEQ_GROUP_SIZE*bvp->CharWidth+block*bvp->CharWidth, y);
    }
  }
}


static void DrawLineEx(Int2 x1, Int2 y1, Int2 x2, Int2 y2, Int2 width)
{
  WidePen (width);
  MoveTo  (x1, y1); LineTo  (x2, y2);
  WidePen (1);
}


static void DrawLtGrid(Int2 x1, Int2 y1, Int2 x2, Int2 y2) { LtGray(); MoveTo (x1, y1); LineTo (x2, y2); }
static void DrawDkGrid(Int2 x1, Int2 y1, Int2 x2, Int2 y2) { DkGray(); MoveTo (x1, y1); LineTo (x2, y2); }


static Int2 SeqPos2XCoord(Int2 x, Int4 seqXPos, BioseqViewPtr bvp)
{
  Int4 blocks = seqXPos / SEQ_GROUP_SIZE;
  Int2 xPos = x + SEQ_X_OFFSET + bvp->SeqStartPosX + seqXPos * bvp->CharWidth + blocks*bvp->CharWidth;
  Int2 rPos = x + SEQ_X_OFFSET + bvp->SeqStartPosX + bvp->CharsAtLine * bvp->CharWidth + (blocks-1) * bvp->CharWidth;
  return xPos > rPos ? rPos : xPos;
}

static Int4 GetFeatureX(Int4 feat_pos, Int4 bsStart, Int4 bsFinish, Boolean is_start)
{
  Int4 lineFeatPos = is_start ? (bsStart  < feat_pos ? feat_pos : bsStart ): 
                                (bsFinish < feat_pos ? bsFinish : feat_pos);
  return lineFeatPos;
}


static Boolean IsInRange(Int4 pos, Int4 min_pos, Int4 max_pos)
{
  return min_pos <= pos  &&  pos <= max_pos;
}


static Boolean isSelected(Int2 itemID, BioseqViewPtr bvp)
{
    ValNodePtr vnp;

    for (vnp = bvp->Selection; vnp != NULL; vnp = vnp->next) {
      if (vnp->data.intvalue == itemID  &&  vnp->choice == 1) return TRUE;
    }
    return FALSE;
}



static void DrawFeature(Int2 x, Int2 y, Int4 line, Int4 row, Int2 itemID, 
                        Boolean protProduct, BioseqPtr bsp, BioseqViewPtr bvp)
{
  SeqMgrFeatContext fcontext;
  SeqFeatPtr        sfp;
  BioseqPtr         bsp_prot;
  CharPtr           str_prot;
  Int4              bsStart, bsFinish, ffStart, ffFinish, i;
  Int2              x1, x2;
  Char              featLabel[13];
  Int4              fLeft, fRight;
    
  if (bvp->seqAlignMode) {
    SeqIdPtr  sip_tmp = AlnMgr2GetNthSeqIdPtr(bvp->salp, row);
    BioseqPtr bsp_tmp = BioseqLockById(sip_tmp);
    sfp = SeqMgrGetDesiredFeature (ObjMgrGetEntityIDForPointer(bsp_tmp), bsp_tmp, itemID, 0, NULL, &fcontext);
    BioseqUnlock (bsp_tmp);
    SeqIdFree    (sip_tmp);
  }
  else {
    sfp = SeqMgrGetDesiredFeature (0, bsp, itemID, 0, NULL, &fcontext);
  }
  
  if (sfp == NULL) return;
  
  bsStart  = line    * bvp->CharsAtLine;
  bsFinish = bsStart + bvp->CharsAtLine;
  
  fLeft    = x_FeatLeft (bvp, fcontext.left );
  fRight   = x_FeatRight(bvp, fcontext.right);
  ffStart  = GetFeatureX(x_Coord(bvp, fLeft,  row), bsStart, bsFinish, TRUE ) - bsStart;
  ffFinish = GetFeatureX(x_Coord(bvp, fRight, row), bsStart, bsFinish, FALSE) - bsStart;
  x1       = SeqPos2XCoord(x, ffStart,  bvp);
  x2       = SeqPos2XCoord(x, ffFinish, bvp);

  fcontext.seqfeattype == SEQFEAT_CDREGION ? Blue() : Black();
  if (fcontext.label != NULL) {
    StrNCpy( featLabel, fcontext.label, 12); featLabel[12]='\0';
    PaintStringEx (featLabel, x+10, y);
  }
  
  if (protProduct) {
    bsp_prot = BioseqFind (SeqLocId(sfp->product));
    str_prot = GetSequenceByBsp (bsp_prot);
    if (str_prot == NULL) {
      PaintStringEx ("Protein sequence in not avaliable", SeqPos2XCoord(x, 0, bvp), y);
      return;
    }
    for (i = ffStart; i != ffFinish; i++) PaintStringEx ("~", SeqPos2XCoord(x, i, bvp), y);
  }
  else DrawLineEx(x1, y-bvp->LineHeight/2+2, x2, y-bvp->LineHeight/2+2, 1);
  
  
  for (i = 0; i != fcontext.numivals; i++) {
    RecT rect;
    Int4 ifLeft  = x_FeatLeft  (bvp, fcontext.strand==Seq_strand_minus ? fcontext.ivals[i*2+1] : fcontext.ivals[i*2]);
    Int4 ifRight = x_FeatRight (bvp, fcontext.strand==Seq_strand_minus ? fcontext.ivals[i*2] : fcontext.ivals[i*2+1]);
    Int4 regStart  = x_Coord(bvp, ifLeft,  row);
    Int4 regFinish = x_Coord(bvp, ifRight, row);
    Int4 fStart    = GetFeatureX(regStart,  bsStart, bsFinish, TRUE );
    Int4 fFinish   = GetFeatureX(regFinish, bsStart, bsFinish, FALSE);
    
    if( !IsInRange(regStart, bsStart,  bsFinish ) && !IsInRange(regFinish, bsStart,  bsFinish) &&
        !IsInRange(bsStart,  regStart, regFinish) && !IsInRange(bsFinish,  regStart, regFinish) ) continue;       

    x1 = SeqPos2XCoord(x, fStart  - bsStart, bvp);
    x2 = SeqPos2XCoord(x, fFinish - bsStart, bvp);
    
    if (protProduct) {  /* draw protein product */
      Int4 j, k, ivalLen = 0;
      Int4 subProdPos = 0;
      Char tmp[2];
      Int4 frame = 1;  /* center in group of 3 */
      
      if (sfp->data.choice == SEQFEAT_CDREGION) {
        CdRegionPtr crp = sfp->data.value.ptrvalue;
        if (crp != NULL ) {
          if (crp->frame == 2) frame++;
          if (crp->frame == 3) frame+=2;
        }
      }
      
      for (k = 0; k != i; k++) { 
          Int4 ifLeft  = x_FeatLeft  (bvp, fcontext.ivals[(k+1)*2]);
          Int4 ifRight = x_FeatRight (bvp, fcontext.ivals[k*2+1]  );
          subProdPos += (fcontext.strand==Seq_strand_minus ? ifRight - ifLeft : ifLeft - ifRight);
      }
      
      for (k = 1; k <= i; k++) { 
          Int4 ifLeft  = x_FeatLeft  (bvp, fcontext.ivals[(k-1)*2  ]);
          Int4 ifRight = x_FeatRight (bvp, fcontext.ivals[(k-1)*2+1]);
          if (k>0) ivalLen += (fcontext.strand==Seq_strand_minus ? (ifLeft - ifRight - frame) : (ifRight - ifLeft - frame));
      }

#if defined(WIN_MOTIF)
      Gray();
      DrawLineEx(x1, y-1, x2, y-1, bvp->CharHeight);
#elif defined(WIN_MSWIN)
      White();
      DrawLineEx(x1, y+2, x2, y+2, bvp->CharHeight);
#else
      White();
      DrawLineEx(x1, y-bvp->CharHeight, x2, y-bvp->CharHeight, bvp->CharHeight);
#endif 
      Blue();
      
      for (j = fLeft;  j < fRight;  j+=3) {
        Int4 xPos, pPos;
        Int4 alnJ = x_Coord(bvp, j, row); 
                
        if (alnJ + frame < fStart  ||  alnJ + frame > fFinish) continue;
     
        xPos = alnJ - bsStart + frame;        
        pPos = alnJ - fLeft - subProdPos;  /* subProdPos: take invervals in features into account */
        
        if (bvp->seqAlignMode) pPos = AlnMgr2MapSeqAlignToBioseq(bvp->salp, pPos, row);
        if (ivalLen % 3 > 0) xPos--;

        if (pPos % 3 == 0) pPos = pPos / 3;
        else pPos = pPos / 3 - 1;

        /* Check for gaps in this feature if in alignment mode */
        if (bvp->seqAlignMode  &&  AlnMgr2MapSeqAlignToBioseq(bvp->salp, alnJ, row) < 0) {
          PaintStringEx("-", SeqPos2XCoord(x, xPos, bvp), y);
          continue;
        }
        
        if (xPos >= bvp->CharsAtLine  ||  xPos > regFinish-bsStart) break;
        
        StrNCpy( tmp, &str_prot[pPos], 1); tmp[1] = '\0';
        PaintStringEx(tmp, SeqPos2XCoord(x, xPos, bvp), y);
      }  
    } /* protein product */
    else {  /* draw feature line */
      Int4 i;      
#if defined(WIN_MOTIF)
      DrawLineEx(x1+1, y-bvp->LineHeight/2+2, x2, y-bvp->LineHeight/2+2, 3);
      if (isSelected(itemID, bvp)) {
        Black();
        DrawLineEx(x1+1, y-bvp->LineHeight/2-1, x2, y-bvp->LineHeight/2-1, 1);
        DrawLineEx(x1+1, y-bvp->LineHeight/2+7, x2, y-bvp->LineHeight/2+7, 1);
      }
#elif defined(WIN_MSWIN)  /* should be verified */
      DrawLineEx(x1+1, y-bvp->LineHeight/2+2, x2, y-bvp->LineHeight/2+2, 3);
      if (isSelected(itemID, bvp)) {
        Black();
        DrawLineEx(x1+1, y-bvp->LineHeight/2-1, x2, y-bvp->LineHeight/2-1, 1);
        DrawLineEx(x1+1, y-bvp->LineHeight/2+7, x2, y-bvp->LineHeight/2+7, 1);
      }
#else
      DrawLineEx(x1, y-bvp->LineHeight/2+1, x2, y-bvp->LineHeight/2+1, 3);
      if (isSelected(itemID, bvp)) {
        Black();
        DrawLineEx(x1, y-bvp->LineHeight/2-2, x2, y-bvp->LineHeight/2-2, 1);
        DrawLineEx(x1, y-bvp->LineHeight/2+6, x2, y-bvp->LineHeight/2+6, 1);
      }
#endif      

      if (IsInRange(regStart, bsStart, bsFinish)) {
        LoadRect (&rect, x1, y-bvp->LineHeight/2-1, x1+7, y-bvp->LineHeight/2+6);
        CopyBits (&rect, fcontext.strand==Seq_strand_minus ? FillLeftArrowSym : FillRectangleSym );
      }
      if (IsInRange(regFinish, bsStart, bsFinish)) {
        LoadRect (&rect, x2, y-bvp->LineHeight/2-1, x2+7, y-bvp->LineHeight/2+6);
        CopyBits (&rect, fcontext.strand==Seq_strand_minus ? FillRectangleSym : FillRightArrowSym);
      }
      
      if (bvp->seqAlignMode) {  /* Check for gaps in this feature if in alignment mode */
#ifdef WIN_MOTIF
        Gray();
#else
        White();
#endif
        for (i = fStart; i < fFinish; i++) {
          if (AlnMgr2MapSeqAlignToBioseq(bvp->salp, i, row) < 0) {  /* Gap? */
            Int4 xPos = SeqPos2XCoord(x,  i - bsStart, bvp);
            PaintStringEx ( "-", xPos, bvp->DrawGrid ? y - 2 : y);  /* Highlight gaps in features */
          }
        }  /* for */
      }
    }  /* feature or product */
  }  /* for */
}



static void onDrawSeqPanel (PaneL p)
{
  BioseqViewPtr bvp;
  BioseqPtr     bsp;
  SeqPortPtr    spp;
  SeqPanLinePtr splp;
  Uint1Ptr      buf, seqbuf, alnbuf;
  char* sss = 0;
  BaR           sb;
  RecT          r;
  Int4          line;
  Int2          x, y;
 
  bvp = GetBioseqViewPtr (p);
  bsp = bvp->bsp;
  spp = SeqPortNew (bsp, 0, bsp->length-1, Seq_strand_plus, Seq_code_iupacna);
  buf = MemNew (SEQ_GROUP_SIZE+1);
  if (bvp->seqAlignMode) { /* allocate it here, to speed up drawing of alignments */
    seqbuf = MemNew (bvp->BlocksAtLine * SEQ_GROUP_SIZE + 3);
    alnbuf = MemNew (bvp->BlocksAtLine * SEQ_GROUP_SIZE + 3);
  }
  sb  = GetSlateVScrollBar ((SlatE)bvp->seqView);
  
  ObjectRect (p, &r);
  InsetRect (&r, 4, 4);
  x = r.left + 1;
  y = r.top  + bvp->CharHeight + SEQ_Y_OFFSET;
  
  SelectFont ((FonT)(bvp->displayFont));  
  for (line = GetBarValue(sb); line < bvp->TotalLines  &&  y <= r.bottom-2*SEQ_Y_OFFSET; line++) {
    if (IsInRange(y, updateRect.top,updateRect.bottom) || 
        IsInRange(y+bvp->LineHeight,updateRect.top,updateRect.bottom))
    {
      /* draw begin */
      splp = bvp->SeqPanLines[line];
      switch ( splp->lineType ) {
        case eTypeTopSeqNumbers:
  	      if (bvp->DrawGrid) DrawLtGrid(bvp->SeqStartPosX+1, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
          DrawTopSeqNums(x, y, splp->bioSeqLine, bvp);                               /* Draw top numbering   */
          break;
        case eTypeTopScaleMarks:
    	    if (bvp->DrawGrid) DrawLtGrid(bvp->SeqStartPosX+1, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
          DrawTopScaleMarks(x, y, splp->bioSeqLine, bvp);                            /* Draw top scale marks */
          break;
        case eTypeSequence:
          if (GetValue(bvp->newNumControl) != eNumNone) DrawSeqSideLineNumbers(x, y, splp->bioSeqLine, bvp); /* Draw line numbers    */
          DrawSequence(x, y, splp->bioSeqLine, spp, buf, bvp);                       /* Draw the sequence    */
          if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
          break;
        case eTypeAlignSequence:
          DrawAlignSideLineNumbers(x, y, splp->bioSeqLine, splp->row, bvp);
          DrawAlignment(x, y, splp->bioSeqLine, splp->row, buf, seqbuf, alnbuf, bvp);                /* Draw the alignment   */
          if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
          break;
        case eTypeFeature:
          if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
          DrawFeature(x, y, splp->bioSeqLine, splp->row, splp->idx, splp->protProduct, bsp, bvp);    /* Draw Features        */
          break;
      }
      if(bvp->DrawGrid && line<bvp->TotalLines-1 && splp->bioSeqLine!=bvp->SeqPanLines[line+1]->bioSeqLine)
          DrawDkGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);             /* Draw Horizontal Grid */
      /* draw end */
    }
    y += bvp->LineHeight;
  }

  if (bvp->DrawGrid) {
    DrawDkGrid(bvp->SeqStartPosX, r.top, bvp->SeqStartPosX, r.bottom);                               /* Draw Vertical Grid */
    if (bvp->seqAlignMode) DrawDkGrid(bvp->SeqStartPosX-50, r.top, bvp->SeqStartPosX-50, r.bottom);  /* Draw Second Grid   */
  }

  if (bvp->seqAlignMode) {
    MemFree (seqbuf);
    MemFree (alnbuf);
  }
  MemFree (buf);
  SeqPortFree (spp);
}

static void onVScrollBarSeqPanel (BaR sb, SlatE s, Int4 newval, Int4 oldval)
{
  BioseqViewPtr bvp;
  RecT r_scroll, r_redraw;
  Int2 pixels;
  Int2 lines;
  
  Select (s);
  bvp = GetBioseqViewPtr((PaneL)s);
  ObjectRect (bvp->seqView, &r_scroll);
  ObjectRect (bvp->seqView, &r_redraw);
  InsetRect (&r_scroll, 2, 2);
  InsetRect (&r_redraw, 2, 2);

  lines = (r_scroll.bottom - r_scroll.top - 2*SEQ_Y_OFFSET-3) / bvp->LineHeight;
  
  if (abs(oldval-newval) > lines) InvalRect(&r_redraw);
  else {
    pixels = (oldval - newval) * bvp->LineHeight;

    if (pixels < 0) {
      r_scroll.top   += SEQ_Y_OFFSET + 2;
      r_redraw.bottom = r_scroll.bottom = r_scroll.top + lines * bvp->LineHeight + 4;
      r_redraw.top    = r_scroll.bottom-abs(pixels)-bvp->LineHeight;
    }
    else {
      r_redraw.bottom = r_redraw.top + pixels + 2*SEQ_Y_OFFSET;
      r_scroll.bottom = r_scroll.top + lines * bvp->LineHeight + 6;
    }
    ScrollRect (&r_scroll, 0, pixels);
    InvalRect (&r_redraw);
  }
  
  Update ();
}

/* extern functions */

PaneL CreateSeqViewPanel (GrouP g, Int2 w, Int2 h)
{
  PaneL pnl = AutonomousPanel4 (g, w, h, onDrawSeqPanel, onVScrollBarSeqPanel, NULL, sizeof (BioseqViewPtr), onCloseSeqPanel, NULL);
  SetPanelClick(pnl, onSeqViewClick, NULL, NULL, onSeqViewRelease);
  return pnl;
}

BioseqPageData seqpnlPageData = {
  "Sequence", TRUE, TRUE, TRUE, FALSE, -1,
  PopulateSeqView, ShowSeqView, SelectSeqView,
  NULL, NULL,
  NULL, NULL, ResizeSeqView, NULL
};

BioseqPageData seqAlnPnlPageData = {
  "Alignment", TRUE, TRUE, TRUE, TRUE, -1,
  PopulateAlnView, ShowSeqView, SelectSeqView,
  NULL, NULL,
  NULL, NULL, ResizeSeqView, NULL
};


