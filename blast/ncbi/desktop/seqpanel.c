/* $Id: seqpanel.c,v 6.144 2005/04/27 20:09:54 bollin Exp $
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
#include <salpanel.h>
#include <salparam.h>
#include <salfiles.h>
#include <edutil.h>
#include <subutil.h>
#include <actutils.h>
#include <algo/blast/api/twoseq_api.h>
#include <alignval.h>

enum ESeqNum   { eNumNone=1, eNumSide=2, eNumTop=3 };
enum EDrawGrid { eDrawGridOn=1, eDrawGridOff=2 };
enum EShowFeatures { eShowFeaturesOn=1, eShowFeaturesOff=2, eShowFeaturesAll=3 };
enum ELineType { eTypeTopSeqNumbers, eTypeTopScaleMarks, eTypeSequence, 
                 eTypeAlignSequence, eTypeFeature, eTypeAlignDivider,
                 eTypeFrame1, eTypeFrame2, eTypeFrame3,  
                 eTypeFrame4, eTypeFrame5, eTypeFrame6,
                 eTypeSequenceComplement };
                 
#define GRID_LINE_SPACE       6
#define NO_GRID_LINE_SPACE    2
#define SEQ_GROUP_SIZE       10  /* Sequence group size           */
#define SEQ_X_OFFSET          4
#define SEQ_Y_OFFSET          4
#define ALNMGR_GAP           -2
#define ROW_UNDEFINED        -1
#define PROT_PRODUCT_TYPE   255
#define PROT_ON_THE_FLY     254
#define GAP_CHAR            '.'


static Uint1 FillRectangleSym [] = { 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x00 };
static Uint1 FillLeftArrowSym [] = { 0x06, 0x1E, 0x7E, 0xFE, 0x7E, 0x1E, 0x06, 0x00 };
static Uint1 FillRightArrowSym[] = { 0xC0, 0xF0, 0xFC, 0xFE, 0xFC, 0xF0, 0xC0, 0x00 };

typedef struct seqParag { 
  ValNodePtr PNTR pFeatList;  /* pointer to an array of lists of feature indexes */ 
} SeqParaG, PNTR SeqParaGPtr;

/* struct that holds data for SeqEditor window */
typedef struct seqedformdata 
{

  FORM_MESSAGE_BLOCK
  
  BioseqViewFormPtr bfp;
  GrouP             upper_button_group;
  ButtoN            goto_btn;
  TexT              goto_txt;
  ButtoN            lookat_btn;
  TexT              lookat_txt;	
  PrompT            position_label;
  GrouP             lower_button_group;
  ButtoN            accept;
  ButtoN            cancel;
  IteM              frames[10];
  IteM              undo_menu;
  IteM              redo_menu;
  
  Boolean           spliteditmode;          /* This flag indicates whether new nucleotides
                                             * entered into the sequence will become part of
                                             * a feature or will introduce an exon into the
                                             * location of the features containing this span.
                                             */
  Int4              edit_pos_start;         
  Int4              edit_pos_end;
  Int4              edit_pos_orig;
  Uint2             moltype;
  SeqEdJournalPtr   undo_list;              /* This is a list of journal entries used for
                                             * undo and redo.
                                             */
  SeqEdJournalPtr   last_journal_entry;     /* This entry points to the last action taken -
                                             * undo should unplay starting with this entry,
                                             * redo should start playing beginning with
                                             * last_journal_entry->next.
                                             * This should never, ever be NULL, as undo_list
                                             * contains start and end journal entries.
                                             */
  BioseqPtr         orig_bsp;
  
  CharPtr           current_pattern;        /* This holds the most recently selected pattern
                                             * from the Find dialog.
                                             */
  Boolean           current_pattern_revcomp;/* This indicates whether the most recent search
                                             * in the Find dialog was a reverse-complement
                                             * search.
                                             */
  Boolean           current_pattern_translate; /*This indicates whether the most recent search
                                             * in the Find dialog was a translation search.
                                             */
  Int4              current_pattern_translate_frame_choice; /* This indicates the frame
                                             * for the translated search for the most recent
                                             * search in the Find dialog.
                                             */
  ValNodePtr        match_list;             /* This holds a list of SeqLocs that contain 
                                             * current_pattern.
                                             */
  SeqFeatPtr        feature_to_drag;        /* This points to the last feature to receive a
                                             * single click, in preparation for dragging the
                                             * end.
                                             */
  SeqLocPtr         feature_orig_loc;       /* This indicates the original position of the
                                             * feature whose end is being dragged. */
  EMoveType         feature_drag_type;      /* This indicates whether we are dragging the
                                             * left end, dragging the right end, or sliding
                                             * the entire interval. */
  Int4              feature_drag_origin; 
  Int4              feature_drag_interval;  /* integer offset of interval to edit */
  Int4              last_drag_change;       /* amount of change for last drag command -
                                             * we only want to resize the window if
                                             * we have pulled a feature interval onto or off
                                             * of a new line.
                                             */
                                             
  ValNodePtr        gapvnp;                 /* used for indexing gap features */                                             
  
} SeqEdFormData, PNTR SeqEdFormPtr;

typedef struct seqpanpara 
{
  Int4               sequenceLineOffset;    /* This indicates the position of the line of 
                                             * sequence data described by the paragraph. 
                                             */
  Boolean            indexed;               /* This flag indicates whether the paragraph has
                                             * been indexed or not.
                                             */  
  SeqPanLinePtr PNTR lines;                 /* If the paragraph has been indexed, lines will
                                             * point to an array of pointers to SeqPanLinePtr
                                             * with the lines for this paragraph.  Otherwise
                                             * lines should be NULL.
                                             */
  Int4               num_lines;             /* If the paragraph has been indexed, num_lines
                                             * will indicate the size of the array in lines.
                                             * Otherwise num_lines should be zero.
                                             */
} SeqPanParaData, PNTR SeqPanParaPtr;

/* This is the number of paragraphs to be allocated per chapter. */
#define SEQ_PAN_CHAPTER_SIZE 50


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
  plp->on_the_fly   = FALSE;
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
  Int4 j, start, stop;
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
      start = SeqLocStart (sfp->location);
      stop = SeqLocStop (sfp->location);

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
  Int4Ptr            alnValidRows = NULL;
  Int4               featRows = 1;
  SeqAlignPtr        tmp_salp;
  Int4Ptr            lines_per_alignment = NULL;
  Int4               num_alignments;
  Int4               aln_idx;
  SeqParaGPtr   PNTR ref_offset;
  
  if (bvp->seqAlignMode) {
    alnRows = AlnMgr2GetNumRows(bvp->salp);  /* size of the alignment */
    fLines  = GetValue(bvp->newNumControl) == eNumTop ? 2 : 0;  
    pCount = 0;
    num_alignments = 0;
    for (tmp_salp = bvp->salp; tmp_salp != NULL; tmp_salp = tmp_salp->next)
    {
      num_alignments ++;
    }
    lines_per_alignment = (Int4Ptr) MemNew (sizeof (Int4) * num_alignments);
    if (lines_per_alignment == NULL) return NULL;
    alnValidRows = (Int4Ptr) MemNew (sizeof (Int4) * num_alignments);
    if (alnValidRows == NULL)
    {
      MemFree (lines_per_alignment);
      return NULL;
    }
    for (i=0; i < num_alignments; i++)
    {
      lines_per_alignment [i] = 0;
      alnValidRows [i] = 0;
    }
    for (tmp_salp = bvp->salp, aln_idx = 0; tmp_salp != NULL; tmp_salp = tmp_salp->next, aln_idx++)
    {
      lines_per_alignment [aln_idx] = floor((AlnMgr2GetAlnLength(tmp_salp, FALSE)-1) / lineLength) + 1; /* alignment length */ 
      pCount  += lines_per_alignment [aln_idx];  
      if (bvp->viewWholeEntity  ||  GetValue(bvp->newFeatControl) == eShowFeaturesAll) 
      {
      	if (featRows == 1)
      	  featRows = alnRows;
      	else
      	  featRows += alnRows;
      } /* show features for all rows */

      for (j = 1; j != alnRows + 1; j++) {  /* AlnMgr counts from 1, not 0    */
        SeqIdPtr  tmp_sip = AlnMgr2GetNthSeqIdPtr(tmp_salp, j);
        BioseqPtr tmp_bsp = BioseqLockById(tmp_sip);
        if (tmp_bsp != NULL) {
          alnValidRows [aln_idx]++;  /* count avaliable alignments */
          BioseqUnlock (tmp_bsp);
        }
        SeqIdFree (tmp_sip);
      }
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
  ref_offset = ref;
  
  if (GetValue(bvp->newFeatControl) != eShowFeaturesOff) {
    if (bvp->seqAlignMode) {  /* in alignment mode */
      for (tmp_salp = bvp->salp, aln_idx = 0; tmp_salp != NULL; tmp_salp = tmp_salp->next, aln_idx ++)
      {
        for (i = 1; i != alnRows + 1; i++) {
          SeqIdPtr  sip_tmp = AlnMgr2GetNthSeqIdPtr(tmp_salp, i);
          BioseqPtr bsp_tmp = BioseqLockById(sip_tmp);

          if (bsp_tmp != NULL) {
            /* TODO: Need to calculate better values for passing to FillFeatureInfo */
            if (bvp->viewWholeEntity  ||  GetValue(bvp->newFeatControl) == eShowFeaturesAll) {
              FillFeatureInfo(bvp, bsp_tmp, lineLength, lines_per_alignment [aln_idx], i, i - 1, ref_offset);  /* show features for each seq in alignment */
            } else if (i == bvp->TargetRow) {
              FillFeatureInfo(bvp, bsp_tmp, lineLength, lines_per_alignment [aln_idx], i, 0, ref_offset);  /* show features for target seq in alignment */
            }
            BioseqUnlock (bsp_tmp);
          }  /* bsp_tmp != NULL */
          SeqIdFree    (sip_tmp);
        }  /* for */
        ref_offset += lines_per_alignment [aln_idx];
      }
    } else {
      FillFeatureInfo(bvp, bsp, lineLength, pCount, 0, 0, ref);
    }  
  }  /* done with features */


  bvp->TotalLines = 0; /* go through all pararaphs and count total */
  if (bvp->seqAlignMode)
  {
    for (tmp_salp = bvp->salp, aln_idx = 0; tmp_salp != NULL; tmp_salp = tmp_salp->next, aln_idx ++)
    {
      /* add lines for features */
      for (i = 0; i < lines_per_alignment [aln_idx]; i++) {
        Int4 sub_total = 0;
        for (j = 0; j < featRows; j++) sub_total += ValNodeLen(ref[i]->pFeatList[j]); 
        bvp->TotalLines += fLines + sub_total + (alnRows == -1 ? 0 : alnValidRows[aln_idx]); /* reserve space for alignment */
      }
      /* add one more for divider */
      if (tmp_salp->next != NULL) bvp->TotalLines ++;
    }
  	
  }
  else
  {
    /* add lines for features */
    for (i = 0; i < pCount; i++) {
      Int4 sub_total = 0;
      for (j = 0; j < featRows; j++) sub_total += ValNodeLen(ref[i]->pFeatList[j]); 
      bvp->TotalLines += fLines + sub_total;
    }
    /* add lines for frames */
    for (i = 0; i < 6; i++)
    {
      if (bvp->frames[i])
      {
        bvp->TotalLines += pCount;
      }
    }
    /* add lines for complements */
    if (bvp->ShowComplement)
    {
      bvp->TotalLines += pCount;
    }
  }


  splp = (SeqPanLinePtr*) MemNew( (size_t)(sizeof(SeqPanLinePtr)*bvp->TotalLines) );
  if (bvp->seqAlignMode)
  {
    ref_offset = ref;
    for (tmp_salp = bvp->salp, aln_idx = 0; tmp_salp != NULL; tmp_salp = tmp_salp->next, aln_idx ++)
    {
      for (i = 0; i < lines_per_alignment [aln_idx]; i++)
      {
        if (fLines > 1) { /* Numbers on top */
          splp[lCount++] = MakeSeqPanLine(eTypeTopSeqNumbers, i);
          splp[lCount++] = MakeSeqPanLine(eTypeTopScaleMarks, i);
        }
    
        for (j = 1; j != alnRows + 1; j++) {  /* AlnMgr counts from 1, not 0    */
          SeqIdPtr  tmp_sip = AlnMgr2GetNthSeqIdPtr(tmp_salp, j);
          BioseqPtr tmp_bsp = BioseqLockById(tmp_sip);
        
          if (tmp_bsp != NULL) {
            SeqPanLinePtr plp = MakeSeqPanLine(eTypeAlignSequence, i); /* Add align sequence line*/
            plp->row          = j;  /* Index position in the alignment (row) */
            splp[lCount++]    = plp;
            BioseqUnlock (tmp_bsp);
          
            if (bvp->viewWholeEntity  ||  GetValue(bvp->newFeatControl) == eShowFeaturesAll) {
              MakeFeatureLine(&lCount, i, j, j - 1, ref_offset, splp); /* Add feature line       */
            } else if (j == bvp->TargetRow) {
              MakeFeatureLine(&lCount, i, j, 0,     ref_offset, splp);
            }
          }
          SeqIdFree(tmp_sip);
        }      	
      }
      /* add divider line between alignments */
      if (tmp_salp->next != NULL)
      {
      	splp[lCount++] = MakeSeqPanLine (eTypeAlignDivider, i);
      }
      ref_offset += lines_per_alignment [aln_idx];    	
    }
  	
  }
  else
  {
    for (i = 0; i < pCount; i++) {
      if (fLines > 1) { /* Numbers on top */
        splp[lCount++] = MakeSeqPanLine(eTypeTopSeqNumbers, i);
        splp[lCount++] = MakeSeqPanLine(eTypeTopScaleMarks, i);
      }
      splp[lCount++] = MakeSeqPanLine(eTypeSequence, i);  /* Add sequence line      */
      if (bvp->ShowComplement)
      {
      	splp[lCount++] = MakeSeqPanLine (eTypeSequenceComplement, i);
      }
      MakeFeatureLine(&lCount, i, 0, 0, ref, splp);      /* Add feature line       */  
      if (bvp->frames [0])
        splp[lCount++] = MakeSeqPanLine (eTypeFrame1, i);
      if (bvp->frames [1])
        splp[lCount++] = MakeSeqPanLine (eTypeFrame2, i);
      if (bvp->frames [2])
        splp[lCount++] = MakeSeqPanLine (eTypeFrame3, i);
      if (bvp->frames [3])
        splp[lCount++] = MakeSeqPanLine (eTypeFrame4, i);
      if (bvp->frames [4])
        splp[lCount++] = MakeSeqPanLine (eTypeFrame5, i);
      if (bvp->frames [5])
        splp[lCount++] = MakeSeqPanLine (eTypeFrame6, i);
    }
  }/* bvp->seqAlignMode */

  for (i = 0; i < pCount; i++) {
    for (j = 0; j < featRows; j++) ValNodeFree (ref[i]->pFeatList[j]);
    MemFree (ref[i]->pFeatList);
    MemFree (ref[i]);
  }
  MemFree (ref);
  MemFree (lines_per_alignment);
  MemFree (alnValidRows);
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
  height = r.bottom - r.top - 3 * SEQ_Y_OFFSET;

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
  SeqIdPtr tmpsip;
  SeqLocPtr slp;
  
  bvp->seqAlignMode = TRUE;
  bvp->SeqStartPosX = 150;
  
  /* if we're switching between segments we might need to load a
   * different alignment */
  if (bvp->salp != NULL)
  {
    if ((bvp->bsp->repr != Seq_repr_seg && bvp->salp->next != NULL)
  	  || (bvp->TargetRow = AlnMgr2GetFirstNForSip (bvp->salp, sip)) == -1)
  	{
  	  bvp->salp = SeqAlignFree (bvp->salp);
  	}
  	else
  	{
  	  PopulateSeqAlnView(bvp);
  	  return;	
  	}
  }
  
  if (bvp->salp == NULL  &&  bvp->seqAlignMode) { /* Try to find an alignment */
    bvp->salp = FindAlignmentsForBioseq (bvp->bsp);
      
    if (bvp->salp == NULL) {  /* No alignment found or bug in AlignMgr (which is more likely). Switch to sequence mode */
      PopulateSeqView(bvp);
      return;
    }    
  }
  bvp->TargetRow = ROW_UNDEFINED;
  if (bvp->bsp->repr == Seq_repr_seg)
  {
    slp = (SeqLocPtr) bvp->bsp->seq_ext;
    while (slp != NULL && bvp->TargetRow == ROW_UNDEFINED)
    {
      tmpsip = SeqLocId (slp);
      while (tmpsip  &&  bvp->TargetRow == ROW_UNDEFINED)
      {
        bvp->TargetRow = AlnMgr2GetFirstNForSip(bvp->salp, tmpsip);
        tmpsip = tmpsip->next;
      }
      slp = slp->next;
    }
  }
  else
  {
    while (sip  &&  bvp->TargetRow == ROW_UNDEFINED) {
      bvp->TargetRow = AlnMgr2GetFirstNForSip(bvp->salp, sip);
      sip = sip->next;
    }  	
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
    MoveTo(x+SEQ_X_OFFSET+bvp->SeqStartPosX+(block+1)*SEQ_GROUP_SIZE*bvp->CharWidth+block*bvp->CharWidth - bvp->CharWidth/2 - 1, y - 1);
    LineTo(x+SEQ_X_OFFSET+bvp->SeqStartPosX+(block+1)*SEQ_GROUP_SIZE*bvp->CharWidth+block*bvp->CharWidth - bvp->CharWidth/2 - 1, y-bvp->CharHeight);

    MoveTo(x+SEQ_X_OFFSET+bvp->SeqStartPosX+(block+1)*SEQ_GROUP_SIZE*bvp->CharWidth+block*bvp->CharWidth - SEQ_GROUP_SIZE*bvp->CharWidth/2 - bvp->CharWidth/2 - 1, y - 1);
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

static void DrawAlignmentDivider (Int2 x, Int2 y, BioseqViewPtr bvp)
{
  Int2 block, ctr=0;
  char buf[20];
  
  Magenta ();
  for (block = 0;  block != bvp->BlocksAtLine  &&  ctr >= 0;  block++) {
    sprintf(buf, "~~~~~~~~~~");
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
  SelectFont ((FonT)(bvp->displayFont));    
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

  if (bsp == NULL)
  {
    *seqstart = ALNMGR_GAP;
    *seqstop = ALNMGR_GAP;
    return;
  }
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


static void 
AlignmentIntervalToString 
(SeqAlignPtr salp,
 Int4        row,
 Int4        start,
 Int4        stop,
 Int4        target_row,
 Boolean     view_whole_entity,
 Uint1Ptr    seqbuf,
 Uint1Ptr    alnbuf,
 Int4 PNTR   alnbuffer_len,
 Boolean     show_substitutions)
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
  if (target_row < 0) return;

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
  
  if (seqstop == -1 || seqstop>=bsp->length)
  {
    seqstop = bsp->length - 1;  /* -1 means exeed sequence length */
  }
  
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
    {
      seqstart = seqstop - alnbuf_len;
    }
  } else {
    if (seqstop - seqstart > alnbuf_len) 
    {
      seqstop = seqstart + alnbuf_len;  /* not to exeed the current line */
    }
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
      if (show_substitutions)
      {
        /* Handle mismatches (insert dots when matched) */
        if (row != target_row  &&  ! view_whole_entity  &&  target_row != ROW_UNDEFINED)  {
          if(target_pos >= 0  && target_pos < bsp_target->length) { /* no gap in the target sequence */
            if (seqbuf[k] == target_buf[i]) {
              alnbuf[aln_pos - start] = GAP_CHAR;
            }
          }
        }   /* mismatches */
      }
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


static Boolean isSelected(Int2 itemID, BioseqViewPtr bvp)
{
    ValNodePtr vnp;

    for (vnp = bvp->Selection; vnp != NULL; vnp = vnp->next) {
      if (vnp->data.intvalue == itemID  &&  vnp->choice == 1) return TRUE;
    }
    return FALSE;
}


static void DrawAlignment
(Int2 x, Int2 y, Int4 line, Int4 row, Uint1Ptr buf, Uint1Ptr seqbuf,
 Uint1Ptr alnbuf, BioseqViewPtr bvp, Int4 aln_idx)
{
  Int2        block;
  Char        alnlabel[13];
  SeqIdPtr    sip;
  BioseqPtr   bsp;
  Int4        start   = line * bvp->CharsAtLine;
  Int4        stop    = start + bvp->BlocksAtLine * SEQ_GROUP_SIZE;
  Int4        alnbuf_len;
  SeqIdPtr    best_id;
  SeqAlignPtr tmp_salp;
  Int4        i;
  RecT        rct;
  Boolean     invert = FALSE;
  
  for (i=0, tmp_salp = bvp->salp; i < aln_idx && tmp_salp != NULL; i++, tmp_salp = tmp_salp->next)
  {  	
  }
  if (tmp_salp == NULL) return;
  sip = AlnMgr2GetNthSeqIdPtr(tmp_salp, row);
  bsp = BioseqLockById(sip);

  AlignmentIntervalToString (tmp_salp, row, start, stop, bvp->TargetRow,
                             bvp->viewWholeEntity, seqbuf, alnbuf, &alnbuf_len,
                             bvp->showAlnSubstitutions);
  
  /* finally draw everything */
  best_id = SeqIdFindBestAccession (bsp->id);
  SeqIdWrite (best_id, alnlabel, PRINTID_TEXTID_ACCESSION, 12);        /* Get label           */
  if (row == bvp->TargetRow  &&  !bvp->viewWholeEntity) Red();         /* Highlight current   */
  else Black();
  
  /* We need to select the font here because BioseqLockById will update
   * the Desktop if it is open, which will change the font setting.
   */
  SelectFont ((FonT)(bvp->displayFont));  
  PaintStringEx (alnlabel, x+10, y);                                   /* Draw sequence label */

  Black ();
  
  if (isSelected (bsp->idx.itemID, bvp))
  {
    InvertColors ();
    LoadRect (&rct, x + SEQ_X_OFFSET + bvp->SeqStartPosX, y,
              x + SEQ_X_OFFSET + bvp->SeqStartPosX 
                + bvp->BlocksAtLine * SEQ_GROUP_SIZE * bvp->CharWidth
                + (bvp->BlocksAtLine - 1) * bvp->CharWidth,
              y - bvp->LineHeight);
    EraseRect (&rct);
    invert = TRUE;
  }

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
  if (invert)
  {
    InvertColors ();
  }
}  /* DrawAlignment*/

static Int2 SeqPos2XCoord(Int2 x, Int4 seqXPos, BioseqViewPtr bvp);


static void DrawSequenceCaret (Int2 x, Int2 y, Int4 line, BioseqViewPtr bvp)
{
  PoinT pt1, pt2;
  Int4  lenv = 3;
  Int4  lenh = 2;
  Int4  colstart, colstop, row;
  WindoW w;
  SeqEdFormPtr sefp;
  
  if (bvp == NULL) return;
  w = ParentWindow (bvp->seqView);
  if (w == NULL) return;
  sefp = (SeqEdFormPtr) GetObjectExtra (w);
  if (sefp == NULL) return;
  if (line < sefp->edit_pos_start / bvp->CharsAtLine
      || line > sefp->edit_pos_end / bvp->CharsAtLine)
  {
    return;
  }
  
  Red ();
  WidePen (2);
  
  row = y + 1;
  
  if (sefp->edit_pos_start / bvp->CharsAtLine == line)
  {
    /* draw start of caret */  
    colstart = SeqPos2XCoord(x, sefp->edit_pos_start - line * bvp->CharsAtLine, bvp);
    pt1.x = colstart;
    pt1.y = row - lenv;
    pt2.x = colstart;
    pt2.y = row;
    DrawLine (pt1, pt2); 	
  }
  else
  {
  	colstart = SeqPos2XCoord(x, 0, bvp);
  }
  
  if (sefp->edit_pos_end == sefp->edit_pos_start)
  {
  	colstop = colstart;
  }
  else if (sefp->edit_pos_end / bvp->CharsAtLine == line)
  {
  	colstop = SeqPos2XCoord(x, sefp->edit_pos_end - line * bvp->CharsAtLine - 1, bvp) + bvp->CharWidth;
    pt1.x = colstop;
    pt1.y = row - lenv;
    pt2.x = colstop;
    pt2.y = row;
    DrawLine (pt1, pt2); 	
  }
  else
  {
  	colstop = SeqPos2XCoord (x, bvp->CharsAtLine, bvp);
  }

  if (sefp->edit_pos_start == sefp->edit_pos_end)
  {
  	colstart -= lenh;
  	colstop += lenh;
  }

  pt1.x = colstart;
  pt1.y = row;
  pt2.x = colstop;
  pt2.y = row;
  DrawLine (pt1, pt2);  
  WidePen (1);
  Black ();
}

static void DrawSequenceCodonBoundaries (Int2 x, Int2 y, Int4 frame, Int4 line, BioseqViewPtr bvp)
{
  Int4  offset;
  PoinT pt1, pt2;
  Int2  block;
  Int4  low, high;
  
  if (frame == 0) return;
  Gray ();
  if (frame < 4)
  {
    offset = 3 - ((line * bvp->CharsAtLine) % 3) + frame - 1;	
    if (offset > 2) offset -= 3;
  }
  else
  {
    offset = 7 - frame + ((bvp->bsp->length - (line * bvp->CharsAtLine)) % 3);
    if (offset > 2) offset -= 3;
  }
  
  low = y + 2;
  high = y - bvp->CharHeight;
  
  for (block = 0;  block != bvp->BlocksAtLine;  block++) 
  {
    if (offset > 0)
    {
  	  pt1.x = x+SEQ_X_OFFSET+bvp->SeqStartPosX+block*SEQ_GROUP_SIZE*bvp->CharWidth+block*bvp->CharWidth;
  	  pt2.x = pt1.x + offset * bvp->CharWidth;
  	  pt1.y = low;
  	  pt2.y = low;
  	  DrawLine (pt1, pt2);
    }
    while (offset + 2 < SEQ_GROUP_SIZE)
    {
  	  pt1.x = x + SEQ_X_OFFSET + bvp->SeqStartPosX 
  	         + block * SEQ_GROUP_SIZE * bvp->CharWidth 
  	         + block * bvp->CharWidth
  	         + offset * bvp->CharWidth;
  	  pt2.x = pt1.x;
  	  pt1.y = low;
  	  pt2.y = high;
  	  DrawLine (pt1, pt2);
      pt2.x = pt1.x + 3 * bvp->CharWidth;
      pt2.y = low;
      DrawLine (pt1, pt2);
      offset += 3;
    }
	pt1.x = x + SEQ_X_OFFSET + bvp->SeqStartPosX 
  	       + block * SEQ_GROUP_SIZE * bvp->CharWidth 
  	       + block * bvp->CharWidth
  	       + offset * bvp->CharWidth;
  	pt2.x = pt1.x;
  	pt1.y = low;
  	pt2.y = high;
  	DrawLine (pt1, pt2);    	
    
    if (offset < SEQ_GROUP_SIZE)
    {
  	  pt1.x = x + SEQ_X_OFFSET + bvp->SeqStartPosX 
  	         + block * SEQ_GROUP_SIZE * bvp->CharWidth 
  	         + block * bvp->CharWidth
  	         + offset * bvp->CharWidth;
      pt2.x = pt1.x + (SEQ_GROUP_SIZE - offset) * bvp->CharWidth;
      pt1.y = low;
      pt2.y = low;
      DrawLine (pt1, pt2);      
    }
    offset -= SEQ_GROUP_SIZE;
    if (offset < 0)
    {
      offset += 3;
    }
  }
  Black ();
}

static void DrawSequence(Int2 x, Int2 y, Int4 line, SeqPortPtr spp, Uint1Ptr buf, BioseqViewPtr bvp)
{
  Int2 block, ctr = 0, i;

  y -= Descent();
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
  DrawSequenceCaret (x, y, line, bvp);
}

static void DrawSequenceComplement(Int2 x, Int2 y, Int4 line, SeqPortPtr spp, Uint1Ptr buf, BioseqViewPtr bvp)
{
  Int2 block, ctr = 0, i;

  y -= Descent();
  Black ();
  PaintStringEx ("complement", x+10, y);
  for (block = 0;  block != bvp->BlocksAtLine  &&  ctr >= 0;  block++) {
    SeqPortSeek (spp, line*bvp->CharsAtLine+block*SEQ_GROUP_SIZE, SEEK_SET);
    ctr = SeqPortRead (spp, buf, SEQ_GROUP_SIZE);
    if (ctr > 0) {
      if (ctr < SEQ_GROUP_SIZE) MemSet(buf+ctr, '\0', 1);
      for (i = 0; i < ctr; i++) buf[i] = TO_LOWER (buf[i]);  /* convert to lowercase */
      complement_string ((CharPtr)buf);
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


static void DrawMismatchBox (Int4 x, Int4 y, Int4 lineheight, Int4 charwidth)
{
  PoinT pt1, pt2;
  
  Red ();
  pt1.x = x;
  pt1.y = y;
  pt2.x = x;
  pt2.y = y - lineheight;
  DrawLine (pt1, pt2);
  pt1.y = pt2.y;
  pt1.x = x + charwidth;
  DrawLine (pt1, pt2);
  pt2.x = pt1.x;
  pt2.y = y;
  DrawLine (pt1, pt2);
  pt1.x = x;
  pt1.y = y;
  DrawLine (pt1, pt2);
  Blue ();
}

/* This function puts the letters for a minus strand product in the correct positions
 * in the feature line.
 */
static void PaintMinusProtein 
(Int4              x,
 Int4              y,
 Int4              row,
 SeqMgrFeatContext fcontext,
 Int4              interval_offset,
 Int4              bsStart,
 Int4              bsFinish,
 Int4              frame,
 CharPtr           str_prot,
 CharPtr           str_trans,
 BioseqViewPtr     bvp)
{
  Int4 product_start, k;
  Int4 prot_pos, x_pos;
  Char tmp[2];
  Int4 prot_len, trans_len;
  Int4 aln_pos_right, aln_pos_left;
  Int4 product_pos;
  
  product_start = 0;
  tmp [1] = 0;
  if (str_prot == NULL || interval_offset >= fcontext.numivals) return;
  prot_len = StringLen (str_prot);
  /* calculate the length of the translation string - it may be longer or shorter
   * than our protein, or it might not have been provided.
   */
  if (str_trans == NULL)
  {
  	trans_len = 0;
  }
  else
  {
  	trans_len = StringLen (str_trans);
  }
  for (k = 0; k < interval_offset; k++)
  {
  	product_start += fcontext.ivals [2 * k] - fcontext.ivals [2 * k + 1] + 1;
  }
  /* in the alignment coordinate system, find the mapping for the right end of the
   * interval.
   */
  aln_pos_right = x_Coord(bvp, fcontext.ivals [2 * interval_offset], row);
  
  /* does this interval extend into the next line? */
  if (aln_pos_right > bsFinish - 1)
  {
    if (bvp->seqAlignMode) 
    {
  	  /* need to find out how much of the product appears in lines after this one.
  	   * Can't simply subtract the distance, because we can't count the gap characters
  	   * as part of the product.
  	   */
  	   for (k = fcontext.ivals [2 * interval_offset]; k >= fcontext.ivals [2 * interval_offset + 1]; k--)
  	   {
  	     if (x_Coord (bvp, k, row) <= bsFinish)
  	     {
  	   	   break;
  	     }
  	   }
  	   product_start += fcontext.ivals [2 * interval_offset] - k + 1;
    }
    else
    {
      /* how much of the product appears in lines after this one? */
  	  product_start += fcontext.ivals [ 2 * interval_offset] - bsFinish + 1;
    }
  }

  product_pos = product_start;
  product_pos -= frame - 1;
  aln_pos_left = x_Coord(bvp, fcontext.ivals [2 * interval_offset + 1], row);
  
  if (aln_pos_right > bsFinish - 1)
  {
  	aln_pos_right = bsFinish - 1;
  }
  if (aln_pos_left < bsStart)
  {
  	aln_pos_left = bsStart;
  }

  
  /* we need to step through the alignment coordinates */
  for (k = aln_pos_right; k >= aln_pos_left; k--)
  {
    /* Check for gaps in this feature if in alignment mode */
    if (bvp->seqAlignMode && AlnMgr2MapSeqAlignToBioseq(bvp->salp, k, row) < 0) {
      x_pos = SeqPos2XCoord (x, k - bsStart, bvp);
      PaintStringEx("-", x_pos, y);
      continue;
    }
    else
    {
      prot_pos = product_pos / 3;
      if (prot_pos >= prot_len)
      {
        continue;
      }
      if (product_pos % 3 == 1)
      {
  	    x_pos = SeqPos2XCoord (x, k - bsStart, bvp);
      	tmp[0] = str_prot [prot_pos];
      	if (prot_pos < trans_len && str_trans[prot_pos] != str_prot[prot_pos])
      	{
      	  DrawMismatchBox (x_pos, y, bvp->LineHeight, bvp->CharWidth);
      	}
      	if (str_prot [prot_pos] == '*')
      	{
          Red();
      	}
        PaintStringEx(tmp, x_pos, y);
      	if (prot_pos < trans_len && str_trans[prot_pos] != str_prot[prot_pos])
      	{
      	  Blue();
      	}
      }    	
      product_pos ++;
    }
  }
}

static void PaintPlusProtein 
(Int4              x,
 Int4              y,
 Int4              row,
 SeqMgrFeatContext fcontext,
 Int4              interval_offset,
 Int4              bsStart,
 Int4              bsFinish,
 Int4              frame,
 CharPtr           str_prot,
 CharPtr           str_trans,
 BioseqViewPtr     bvp)
{
  Int4 product_start, k;
  Int4 prot_pos, x_pos;
  Char tmp[2];
  Int4 prot_len, trans_len;
  Int4 aln_pos_right, aln_pos_left;
  Int4 product_pos;

  product_start = 0;
  tmp [1] = 0;
  if (str_prot == NULL || interval_offset >= fcontext.numivals) return;
  prot_len = StringLen (str_prot);
  /* calculate the length of the translation string - it may be longer or shorter
   * than our protein, or it might not have been provided.
   */
  if (str_trans == NULL)
  {
  	trans_len = 0;
  }
  else
  {
  	trans_len = StringLen (str_trans);
  }
  for (k = 0; k < interval_offset; k++)
  {
  	product_start += fcontext.ivals [2 * k + 1] - fcontext.ivals [2 * k] + 1;
  }
  /* in the alignment coordinate system, find the mapping for the right end of the
   * interval.
   */
  aln_pos_right = x_Coord(bvp, fcontext.ivals [2 * interval_offset + 1], row);
  aln_pos_left = x_Coord (bvp, fcontext.ivals [2 * interval_offset], row);
  
  /* does this interval extend into the previous line? */
  if (aln_pos_left < bsStart)
  {
    if (bvp->seqAlignMode) 
    {
  	  /* need to find out how much of the product appears in lines before this one.
  	   * Can't simply subtract the distance, because we can't count the gap characters
  	   * as part of the product.
  	   */
  	   for (k = fcontext.ivals [2 * interval_offset]; k <= fcontext.ivals [2 * interval_offset + 1]; k++)
  	   {
  	     if (x_Coord (bvp, k, row) >= bsStart)
  	     {
  	   	   break;
  	     }
  	   }
  	   product_start += k - fcontext.ivals [2 * interval_offset];
    }
    else
    {
      /* how much of the product appears in lines before this one? */
  	  product_start += bsStart - fcontext.ivals [ 2 * interval_offset];
    }
  }

  product_pos = product_start;
  product_pos -= frame - 1;
  if (product_start == 0 && product_pos > 2)
  {  	
  	product_pos -= 3;
  }
  if (aln_pos_right > bsFinish - 1)
  {
    aln_pos_right = bsFinish - 1;
  }
  if (aln_pos_left < bsStart)
  {
    aln_pos_left = bsStart;
  }
  /* we need to step through the alignment coordinates */
  for (k = aln_pos_left; k <= aln_pos_right; k++)
  {
    /* Check for gaps in this feature if in alignment mode */
    if (bvp->seqAlignMode && AlnMgr2MapSeqAlignToBioseq(bvp->salp, k, row) < 0) {
      x_pos = SeqPos2XCoord (x, k - bsStart, bvp);
      PaintStringEx("-", x_pos, y);
      continue;
    }
    else
    {
      prot_pos = product_pos / 3;
      if (prot_pos >= prot_len)
      {
        continue;
      }
  	  x_pos = SeqPos2XCoord (x, k - bsStart, bvp);
      if (product_pos % 3 == 1)
      {
      	if (prot_pos < trans_len && str_trans[prot_pos] != str_prot[prot_pos])
      	{
      	  DrawMismatchBox (x_pos, y, bvp->LineHeight, bvp->CharWidth);
      	}
      	if (str_prot [prot_pos] == '*')
      	{
          Red();
      	}
      	tmp[0] = str_prot [prot_pos];	
        PaintStringEx(tmp, x_pos, y);
      	if (str_prot [prot_pos] == '*')
      	{
      	  Blue();
      	}        
      }
      product_pos ++;   	
    }
  }
}

static void DrawFeatureCodonLines 
(Int2 x, Int2 y, Int4 line, Int4 row, Int2 seqY, Int2 itemID, 
                        Boolean protProduct, BioseqPtr bsp, BioseqViewPtr bvp)
{
  Int4              codon_pos;  
  Int4              k;
  Int4              x_pos;
  Char              tmp[2];
  Int4              aln_pos_right, aln_pos_left;
  Int4              product_pos;
  Int4              int_num;
  PoinT             pt1, pt2, pt3, pt4;
  Int4              bsStart, bsFinish;
  SeqFeatPtr        sfp;
  SeqMgrFeatContext fcontext;
  CdRegionPtr       crp;
  Int2              frame = 1;

  if (!isSelected(itemID, bvp) || ! protProduct) 
  {
    return;
  }
  
  /* need to find feature */
  if (bvp->seqAlignMode) 
  {
    SeqIdPtr  sip_tmp = AlnMgr2GetNthSeqIdPtr(bvp->salp, row);
    BioseqPtr bsp_tmp = BioseqLockById(sip_tmp);
    sfp = SeqMgrGetDesiredFeature (ObjMgrGetEntityIDForPointer(bsp_tmp), bsp_tmp, itemID, 0, NULL, &fcontext);
    BioseqUnlock (bsp_tmp);
    SeqIdFree    (sip_tmp);
  }
  else
  {
    sfp = SeqMgrGetDesiredFeature (0, bsp, itemID, 0, NULL, &fcontext);
  }
  
  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION) return;
  crp = sfp->data.value.ptrvalue;
  if (crp == NULL ) return;
  if (crp->frame == 2) frame--;
  if (crp->frame == 3) frame-=2;
  
  bsStart  = line    * bvp->CharsAtLine;
  bsFinish = bsStart + bvp->CharsAtLine;
  

  /* then cycle through intervals, using only the ones applicable to line */
  product_pos = frame - 1;
  tmp [1] = 0;

  for (int_num = 0; int_num < fcontext.numivals; int_num++)
  {
  	if (fcontext.strand == Seq_strand_minus)
  	{
  	  aln_pos_left = x_Coord(bvp, fcontext.ivals [ 2 * int_num + 1], row);
  	  aln_pos_right = x_Coord(bvp, fcontext.ivals [ 2 * int_num], row);
  	}
  	else
  	{
  	  aln_pos_left = x_Coord(bvp, fcontext.ivals [ 2 * int_num], row);
  	  aln_pos_right = x_Coord(bvp, fcontext.ivals [ 2 * int_num + 1], row);
  	}
  	if (aln_pos_right < bsStart || aln_pos_left > bsFinish)
  	{
  	  product_pos += aln_pos_right - aln_pos_left + 1;
  	}
  	else if (fcontext.strand == Seq_strand_minus && aln_pos_right > bsFinish)
  	{
  	  /* find the portion of this interval that extends into the next line */
      if (bvp->seqAlignMode) 
      {
  	    /* need to find out how much of the product appears in lines after this one.
  	     * Can't simply subtract the distance, because we can't count the gap characters
  	     * as part of the product.
  	     */
  	     for (k = aln_pos_right; k >= aln_pos_left; k--)
  	     {
  	       if (x_Coord (bvp, k, row) <= bsFinish)
  	       {
  	   	     break;
  	       }
  	     }
  	     product_pos += aln_pos_right - k + 1;
      }
      else
      {
        /* how much of the product appears in lines before this one? */
  	    product_pos += aln_pos_right - bsFinish + 1;
      }
  	}
  	else if (fcontext.strand != Seq_strand_minus && aln_pos_left < bsStart)
  	{
  	  /* find the portion of this interval that extends into the previous line */	
      if (bvp->seqAlignMode) 
      {
  	    /* need to find out how much of the product appears in lines before this one.
  	     * Can't simply subtract the distance, because we can't count the gap characters
  	     * as part of the product.
  	     */
  	     for (k = aln_pos_left; k <= aln_pos_right; k++)
  	     {
  	       if (x_Coord (bvp, k, row) >= bsStart)
  	       {
  	   	     break;
  	       }
  	     }
  	     product_pos += k - aln_pos_left;
      }
      else
      {
        /* how much of the product appears in lines before this one? */
  	    product_pos += bsStart - aln_pos_left;
      }
  	}
  	
  	/* we will only look at alignment coordinates inside our line */
  	if (fcontext.strand == Seq_strand_minus)
  	{
  	  if (aln_pos_right > bsFinish - 1)
  	  {
  	  	aln_pos_right = bsFinish - 1;
  	  }

  	  if (aln_pos_left < bsStart)
  	  {
  	  	aln_pos_left = bsStart;
  	  }
  	}
  	else
  	{
      if (aln_pos_right > bsFinish)
      {
        aln_pos_right = bsFinish;
      }
      if (aln_pos_left < bsStart)
      {
        aln_pos_left = bsStart;
      }
  	}
  	
  	if (fcontext.strand == Seq_strand_minus)
  	{
  	  /* minus strand - step through in minus direction */
      for (k = aln_pos_right; k >= aln_pos_left; k--)
      {
        /* Check for gaps in this feature if in alignment mode */
        if (bvp->seqAlignMode && AlnMgr2MapSeqAlignToBioseq(bvp->salp, k, row) < 0) 
        {
          continue; /* we don't draw codon lines or increase the product position in a gap */
        }
        else
        {
          x_pos = SeqPos2XCoord (x, k - bsStart, bvp);
          codon_pos = product_pos % 3;
       	  if (codon_pos != 1)
       	  {
    	    pt1.y = y - 1;
    	    pt2.y = y - bvp->LineHeight;
    	    pt3.y = seqY;
    	    pt4.y = seqY + 4;
      	
      	    if (codon_pos == 0)
      	    {
      	      pt1.x = x_pos + bvp->CharWidth;
      	    }
      	    else if (codon_pos == 2)
      	    {
      	      pt1.x = x_pos;
      	    }
      	    pt2.x = pt1.x;
      	    pt3.x = pt1.x;
      	    pt4.x = pt1.x;
      	    Gray ();
      	    DrawLine (pt1, pt2);
      	    DrawLine (pt3, pt4);
      	    Blue ();	
       	  }
       	  product_pos ++;  
  	    }
      }
  	}
  	else
  	{
  	  /* plus strand - step through in plus direction */
  	  for (k = aln_pos_left; k <= aln_pos_right; k++)
  	  {
        /* Check for gaps in this feature if in alignment mode */
        if (bvp->seqAlignMode && AlnMgr2MapSeqAlignToBioseq(bvp->salp, k, row) < 0) 
        {
          continue; /* we don't draw codon lines or increase the product position in a gap */
        }
        else
        {
          x_pos = SeqPos2XCoord (x, k - bsStart, bvp);
   	      codon_pos = product_pos % 3;
       	  if (codon_pos != 1)
       	  {
    	    pt1.y = y;
    	    pt2.y = y - bvp->LineHeight;
    	    pt3.y = seqY;
    	    pt4.y = seqY + 4;
      	
      	    if (codon_pos == 0)
      	    {
      	      pt1.x = x_pos;
      	    }
      	    else if (codon_pos == 2)
      	    {
      	      pt1.x = x_pos + bvp->CharWidth;
      	    }
      	    pt2.x = pt1.x;
      	    pt3.x = pt1.x;
      	    pt4.x = pt1.x;
      	    Gray ();
      	    DrawLine (pt1, pt2);
      	    DrawLine (pt3, pt4);
      	    Blue ();	
       	  }
       	  product_pos ++;
        }
      }
  	}
  }  
}


static void DrawFeature(Int2 x, Int2 y, Int4 line, Int4 row, Int2 itemID, 
                        Boolean protProduct, Boolean on_the_fly,
                        BioseqPtr bsp, BioseqViewPtr bvp)
{
  SeqMgrFeatContext fcontext;
  SeqFeatPtr        sfp;
  BioseqPtr         bsp_prot;
  CharPtr           str_prot = NULL;
  Int4              bsStart, bsFinish, ffStart, ffFinish, i;
  Int2              x1, x2;
  Char              featLabel[13];
  Int4              fLeft, fRight;
  Int4              prot_len;
  ByteStorePtr      bs;
  CharPtr           str_trans = NULL;

  y -= Descent();
    
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
  if (on_the_fly)
  {
  	StringCpy (featLabel, "on-the-fly");
    PaintStringEx (featLabel, x+10, y);
  }
  else if (fcontext.label != NULL) 
  {
    StrNCpy( featLabel, fcontext.label, 12); featLabel[12]='\0';
    PaintStringEx (featLabel, x+10, y);
  }
  
  if (bvp->show_translation_errors)
  { 
    bs = ProteinFromCdRegionEx (sfp, TRUE, FALSE);
    if (bs != NULL) 
    {
      str_trans = BSMerge (bs, NULL);
      bs = BSFree (bs);
    }
  }
  else
  {
  	str_trans = NULL;
  }
  
  if (protProduct) {
    bsp_prot = BioseqFind (SeqLocId(sfp->product));
    if (on_the_fly && fcontext.seqfeattype == SEQFEAT_CDREGION
        && sfp->data.value.ptrvalue != NULL)
    {
      if (str_trans == NULL)
      {
        bs = ProteinFromCdRegionEx (sfp, TRUE, FALSE);
        if (bs != NULL) 
        {
          str_prot = BSMerge (bs, NULL);
          bs = BSFree (bs);
        }	
      }
      else
      {
        str_prot = str_trans;	
      }
    }
    else
    {
      str_prot = GetSequenceByBsp (bsp_prot);
    }
    if (str_prot == NULL) {
      PaintStringEx ("Protein sequence is not available", SeqPos2XCoord(x, 0, bvp), y);
      return;
    }
    prot_len = StringLen (str_prot) - 1;
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
      Int4 frame = 1;  /* center in group of 3 */
      
      if (sfp->data.choice == SEQFEAT_CDREGION) {
        CdRegionPtr crp = sfp->data.value.ptrvalue;
        if (crp != NULL ) {
          if (crp->frame == 2) frame++;
          if (crp->frame == 3) frame+=2;
        }
      }
      
#if defined(WIN_MOTIF)
      White();
      DrawLineEx(x1, y-1, x2, y-1, bvp->CharHeight);
#elif defined(WIN_MSWIN)
      White();
      DrawLineEx(x1, y+2, x2, y+2, bvp->CharHeight);
#else
      White();
      DrawLineEx(x1, y-bvp->CharHeight, x2, y-bvp->CharHeight, bvp->CharHeight);
#endif 
      Blue();

      if (fcontext.strand == Seq_strand_minus)
      {
        PaintMinusProtein  (x, y, row, fcontext, i, bsStart, bsFinish, frame, str_prot, str_trans, bvp);
      }
      else
      {
        PaintPlusProtein (x, y, row, fcontext, i, bsStart, bsFinish, frame, str_prot, str_trans, bvp);
      }
    } /* protein product */
    else {  /* draw feature line */
      Int4 i; 
           
      fcontext.seqfeattype == SEQFEAT_CDREGION ? Blue() : Black();
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


static void DrawFrame(Int2 x, Int2 y, Int4 line, Int4 frame, BioseqViewPtr bvp)
{
  SeqFeatPtr    fake_cds;
  ByteStorePtr  bs;
  CharPtr       frame_str;
  Uint1         strand; 
  Int4          seqStart; 
  Int4          bsStart, bsFinish, j;
  Int4          xPos, pPos, frame_len;
  Char          tmp[2];
  Char          label[50];
  Int4          offset;
  
  y -= Descent();
  
  seqStart  = line    * bvp->CharsAtLine;
  
  /* determine start of line */
  if (frame < 4)
  {
    offset = 3 - ((line * bvp->CharsAtLine) % 3) + frame - 1;	
  	strand = Seq_strand_plus;
  }
  else
  {
    offset = 7 - frame + ((bvp->bsp->length - (line * bvp->CharsAtLine)) % 3);
  	strand = Seq_strand_minus;
  }
  
  if (offset > 2)
  {
  	offset = offset % 3;
  }
  
  if (offset == 2 && line > 0)
  {
  	offset -= 3;
  }

  bsStart = seqStart + offset;
  bsFinish = (line + 1) * bvp->CharsAtLine + offset;
  frame_len = bsFinish - bsStart + 1;
  bsFinish -= frame_len % 3;
  
  if (frame_len % 3 == 2 && bsFinish + 3 < bvp->bsp->length)
  {
  	bsFinish += 3;
  }
  /* must not extend past end of sequence */
  while (bsFinish > bvp->bsp->length - 1)
  {
  	bsFinish -= 3;
  }
  /* should not extend past end of line */
  while (bsFinish > seqStart + bvp->CharsAtLine)
  {
  	bsFinish -= 3;
  }
  frame_len = bsFinish - bsStart + 1;  
  
  fake_cds = make_fake_cds(bvp->bsp, bsStart, bsFinish, strand);
  
  bs = ProteinFromCdRegionEx(fake_cds, TRUE, FALSE);
  SeqFeatFree(fake_cds);
  if(bs == NULL) return;
  frame_str = BSMerge (bs, NULL);
  bs = BSFree (bs);
  if (frame_str == NULL) return;
  frame_len = StringLen (frame_str);
  
  Black ();
  if (frame < 4)
  {
    sprintf(label, "frame +%d", frame);
  }
  else
  {
  	sprintf (label, "frame %d", 3 - frame);
  }
  PaintStringEx (label, x+10, y);

  tmp[1] = '\0';
  if (frame < 4)
  {
    for (j = offset + 1, pPos = 0;  j < bsFinish && pPos < frame_len;  j+=3, pPos ++) {

      xPos = SeqPos2XCoord(x, j, bvp);
      tmp[0] = frame_str[pPos];   
      PaintStringEx(tmp, xPos, y);
    }
  }
  else
  {
    for (j = offset + 1, pPos = frame_len - 1;  j < bsFinish && pPos > -1;  j+=3, pPos --) {

      xPos = SeqPos2XCoord(x, j, bvp);
      tmp[0] = frame_str[pPos];   
      PaintStringEx(tmp, xPos, y);
    }
  }
  MemFree (frame_str);
}


static void DrawSeqPanel (BioseqViewPtr bvp)
{
  BioseqPtr        bsp;
  SeqPortPtr       spp;
  SeqPanLinePtr    splp;
  Uint1Ptr         buf, seqbuf, alnbuf;
  BaR              sb;
  RecT             r;
  Int4             line;
  Int2             x, y;
  Int4             aln_idx;
  Int4             start;
  PaneL            p;
  
  if (bvp == NULL) return;
  bsp = bvp->bsp;
  p = bvp->seqView;
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
  aln_idx = 0;
  start = GetBarValue (sb);
  for (line = 0; line < start && line < bvp->TotalLines; line++)
  {
    splp = bvp->SeqPanLines[line];
    if (splp->lineType == eTypeAlignDivider)
    {
      aln_idx++;
    }
  }
  
  for (line = start; line < bvp->TotalLines  &&  y <= r.bottom-2*SEQ_Y_OFFSET; line++) {
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
          DrawSequence(x, y, splp->bioSeqLine, spp, buf, bvp);                /* Draw the sequence    */
          if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
          break;
        case eTypeSequenceComplement:
          DrawSequenceComplement(x, y, splp->bioSeqLine, spp, buf, bvp);                       /* Draw the sequence    */
          if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
          break;
        case eTypeAlignSequence:
          DrawAlignSideLineNumbers(x, y, splp->bioSeqLine, splp->row, bvp);
          DrawAlignment(x, y, splp->bioSeqLine, splp->row, buf, seqbuf, alnbuf, bvp, aln_idx);                /* Draw the alignment   */
          if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
          if (bvp->last_aln_row_clicked == splp->row)
          {
            DrawSequenceCaret (x, y, splp->bioSeqLine, bvp);
          }
          break;
      	case eTypeAlignDivider:
      	  DrawAlignmentDivider (x, y, bvp);
      	  aln_idx++;
      	  break;
        case eTypeFeature:
          if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
          DrawFeature(x, y, splp->bioSeqLine, splp->row, splp->idx, 
                      splp->protProduct, FALSE, bsp, bvp);    /* Draw Features        */
          break;
      	case eTypeFrame1:
      	  DrawFrame(x, y, splp->bioSeqLine, 1, bvp);
          if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
     	  break;
      	case eTypeFrame2:
       	  DrawFrame(x, y, splp->bioSeqLine, 2, bvp);
          if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
      	  break;
     	case eTypeFrame3:
      	  DrawFrame(x, y, splp->bioSeqLine, 3, bvp);
          if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
      	  break;
      	case eTypeFrame4:
      	  DrawFrame(x, y, splp->bioSeqLine, 4, bvp);
          if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
      	  break;
      	case eTypeFrame5:
       	  DrawFrame(x, y, splp->bioSeqLine, 5, bvp);
          if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
      	  break;
     	case eTypeFrame6:
      	  DrawFrame(x, y, splp->bioSeqLine, 6, bvp);
          if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
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

static void onDrawSeqPanel (PaneL p)
{
  BioseqViewPtr bvp;
  
  bvp = GetBioseqViewPtr (p);
  if (bvp == NULL) return;
  DrawSeqPanel (bvp);	
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

/* New Sequence Editor Functions */

/* This section of the code is for the new sequence editor.
 * New data types for the new sequence editor include:
 *
 * SeqPanPara: a structure to hold information about a "paragraph" of data
 *     for display/editing.  Each paragraph represents one line of sequence
 *     data, where the length of the line is determined by the width of the
 *     display window.  Depending on the options chosen by the user, a paragraph
 *     could include lines for numbering, lines for features, lines for frames,
 *     and/or a line for the complement, in addition to the main line which
 *     displays the sequence data.
 *
 * chapters: a "chapter" is an array of SEQ_PAN_CHAPTER_SIZE SeqPanPara
 *     structures.  When the Sequence Editor is first opened, a ValNode list
 *     of enough chapters to hold the entire sequence is created but left
 *     unindexed (i.e., information about features is not collected).  If
 *     during the course of editing the sequence more chapters are needed,
 *     these will be appended to the end of the chapter list.  Leftover
 *     SeqPanPara entries will have the sequenceLineOffset member set to -1.
 *     A chapter will only be indexed if it is about to be viewed, but it
 *     will retain its information until the editor window is resized.
 *     The scroll position is based on the estimated number of lines for
 *     unindexed paragraphs plus the actual number of lines for indexed
 *     paragraphs.  As previously unindexed paragraphs are indexed, the
 *     maximum scroll position is adjusted.
 *
 * SeqEdForm: a structure to hold information about the sequence editor
 *     form, including a BioseqView structure (used by functions shared
 *     by the sequence and alignment view panels and the new sequence editor).
 *
 * SeqEdJournal: a structure that holds information about changes the user has made
 *     in the past so that these actions may be undone or redone.
 *
 */




/* This section of code manages the journal entries used for undo, redo, and cancel.
 */
 
/* The SeqEdFormPtr contains two SeqEdJournalPtr members - undo_list and last_journal_entry.
 * undo_list starts with an eSeqEdJournalStart journal entry and ends with an eSeqEdJournalEnd
 * journal entry.  These entries should not be removed until the SeqEdFormPtr is to be freed.
 * They are used to indicate that the undo/redo state is at either end.
 * The last_journal_entry points to the first action to be undone if cancelling or undoing.
 * If last_journal_entry points to an eSeqEdJournalEnd entry, the previous item is the first action
 * to be undone.
 * To redo, start playing entries after the last_journal_entry.
 */
 
static void SetUndoRedoStatus (SeqEdFormPtr sefp);

/* This function initializes the SeqEdJournalPtr members for SeqEdFormPtr.  An initial journal
 * entry list with a start and end is created, and last_journal_entry points to the end.
 */ 
static void InitJournal (SeqEdFormPtr sefp)
{
  sefp->undo_list = (SeqEdJournalPtr) MemNew (sizeof (SeqEdJournalData));
  sefp->undo_list->action = eSeqEdJournalStart;
  sefp->last_journal_entry = (SeqEdJournalPtr) MemNew (sizeof (SeqEdJournalData));
  sefp->last_journal_entry->action = eSeqEdJournalEnd;
  sefp->undo_list->next = sefp->last_journal_entry;
  sefp->undo_list->prev = NULL;
  sefp->last_journal_entry->next = NULL;
  sefp->last_journal_entry->prev = sefp->undo_list;
}

static Boolean AddJournalEntryEx
(ESeqEdJournalAction action,
 Int4                offset,
 Int4                num_chars,
 CharPtr             char_data,
 SeqEdFormPtr        sefp,
 Boolean             is_unknown_gap)
{
  SeqEdJournalPtr sejp, prev, end, next;
  Boolean         rval = FALSE;
  Boolean         feats_deleted = FALSE;
  MsgAnswer       ans = ANS_YES;  
  
  if (sefp == NULL || sefp->last_journal_entry == NULL) return rval;
  
  for (end = sefp->last_journal_entry; end != NULL && end->action != eSeqEdJournalEnd; end = end->next)
  {	
  }
  if (sefp->last_journal_entry != end)
  {
    next = (SeqEdJournalPtr) sefp->last_journal_entry->next;
    if (next != end)
    {
      /* set prev for next in list to NULL */  
      next->prev = NULL;
      /* set next for next to last in list to NULL */
      ((SeqEdJournalPtr)(end->prev))->next = NULL;
      /* free middle of list */
      SeqEdJournalFree (next);
      
      /* reattach end */
      sefp->last_journal_entry->next = end;
      end->prev = sefp->last_journal_entry;
    }
    sefp->last_journal_entry = end;
  }
  
  sejp = SeqEdJournalNewSeqEdit (action, offset, num_chars, char_data, 
                                 sefp->spliteditmode, sefp->bfp->bvd.bsp, 
                                 sefp->bfp->bvd.bsp->mol, sefp->input_entityID);
  sejp->unknown_gap = is_unknown_gap;
  prev = end->prev;
  prev->next = sejp;
  sejp->prev = prev;
  sejp->next = end;
  end->prev = sejp;

  rval = PlayJournal (sejp, &sefp->last_journal_entry, 1, &feats_deleted);
  if (feats_deleted)
  {
    ans = Message (MSG_YN, "You are about to delete one or more features - do you want to continue?");
    if (ans == ANS_NO)
    {
      rval = UnplayJournal (&sefp->last_journal_entry, 1);
    }
  }
  
  SetUndoRedoStatus (sefp);
  return rval;
}

static Boolean AddJournalEntry 
(ESeqEdJournalAction action,
 Int4                offset,
 Int4                num_chars,
 CharPtr             char_data,
 SeqEdFormPtr        sefp)
{
  return AddJournalEntryEx (action, offset, num_chars, char_data, sefp, FALSE);
}

/* This function initializes a previously allocated chapter.  The sequenceLineOffset
 * position values are set with new values, and the paragraphs are set to an
 * unindexed state.
 */
static void 
InitSeqPanChapter 
(SeqPanParaPtr sppp,
 Int4 sequenceLineOffset,
 Int4 total_paragraphs)
{
  Int4 i, j;
  if (sppp == NULL) return;
  
  for (i = 0; i < SEQ_PAN_CHAPTER_SIZE; i++)
  {
    if (i + sequenceLineOffset < total_paragraphs)
    {
      sppp[i].sequenceLineOffset = i + sequenceLineOffset;
    }
  	else
  	{
  	  sppp[i].sequenceLineOffset = -1;
  	}
  	sppp[i].indexed = FALSE;
  	if (sppp[i].lines != NULL)
  	{
  	  for (j = 0; j < sppp[i].num_lines; j++)
  	  {
        MemFree(sppp[i].lines[j]);
  	  }
      sppp[i].lines = MemFree (sppp[i].lines);  		
  	}
  	sppp[i].num_lines = 0;
  }  
}

/* This function frees a list of chapters.  This should only be done when the
 * sequence editor window is closed, as freeing the chapter list and generating
 * a new chapter list is expensive.
 */
static ValNodePtr FreeChapterList (ValNodePtr chapter_list)
{
  SeqPanParaPtr sppp;
  if (chapter_list == NULL) return NULL;
  FreeChapterList (chapter_list->next);
  chapter_list->next = NULL;
  sppp = chapter_list->data.ptrvalue;
  /* InitSeqPanChapter frees the SeqPanelLines in the chapter */
  InitSeqPanChapter (sppp, 0, 0);
  MemFree (sppp);
  chapter_list->data.ptrvalue = NULL;
  ValNodeFree (chapter_list);
  return NULL;
}

/* This function frees the ValNode lists in the array of ValNode lists
 * used for calculating the features for each chapter.
 * This data should only be freed when the sequence editor window is
 * closed, as the code assumes that this memory has been allocated
 * and is available for use.
 */
static ValNodePtr PNTR FreeFeatureLists (ValNodePtr PNTR feature_lists)
{
  Int4 j;
  if (feature_lists == NULL)
  {
  	return NULL;
  }
  for (j = 0; j < SEQ_PAN_CHAPTER_SIZE; j++)
  {
  	feature_lists [j] = ValNodeFree (feature_lists [j]);
  }
  MemFree (feature_lists);
  return NULL;
}

/* This function creates a new chapter (an array of SEQ_PAN_CHAPTER_SIZE SeqPanPara
 * structures) and initializes them.
 */
static SeqPanParaPtr CreateNewSeqPanChapter 
(Int4 sequenceLineOffset,
 Int4 total_paragraphs)
{
  SeqPanParaPtr sppp;
  
  sppp = (SeqPanParaPtr) MemNew (SEQ_PAN_CHAPTER_SIZE * sizeof (SeqPanParaData));
  if (sppp != NULL)
  {
    InitSeqPanChapter (sppp, sequenceLineOffset, total_paragraphs);
  }
  return sppp;
}

/* This function examines the options from the GUI to determine the number of lines that
 * will appear in each paragraph other than the feature lines.
 * This function is used to estimate the size of unindexed paragraphs.
 */
static Int4 CountCommonLines (BioseqViewPtr bvp)
{
  Int4 num_lines = 1; /* start with one, for sequence */
  Int4 i;
  
  if (GetValue (bvp->newNumControl) == eNumTop)
  {
  	num_lines += 2;
  }
  
  /* add lines for frames */
  for (i = 0; i < 6; i++)
  {
    if (bvp->frames[i])
    {
      num_lines ++;
    }
  }
  /* add lines for complements */
  if (bvp->ShowComplement)
  {
    num_lines ++;
  }  
  return num_lines;
}

/* This function will add chapters to the existing chapter list to accommodate the
 * number of required chapters.  It does not remove unneeded chapters.
 */
static void CreateSeqPanChapterList (Int2 lineLength, BioseqViewPtr bvp)
{
  ValNodePtr    chapter_vnp;
  Int4          num_paragraphs;
  Int4          paragraph_count = 0;
  SeqPanParaPtr chapter;
  
  if (bvp == NULL || lineLength == 0) return;
  
  num_paragraphs  = floor(bvp->bsp->length / lineLength) + 1; /* sequence length */ 

  chapter_vnp = bvp->chapter_list;
  
  while (paragraph_count < num_paragraphs)
  {
    if (chapter_vnp == NULL)
    {
   	  chapter_vnp = ValNodeNew (bvp->chapter_list);
   	  if (bvp->chapter_list == NULL) bvp->chapter_list = chapter_vnp;
   	  chapter = CreateNewSeqPanChapter (paragraph_count, num_paragraphs);
   	  chapter_vnp->data.ptrvalue = chapter;
    }
    else
    {
      chapter = chapter_vnp->data.ptrvalue;
      InitSeqPanChapter (chapter, paragraph_count, num_paragraphs);
    }
    if (chapter == NULL || chapter_vnp == NULL)
    {
      bvp->chapter_list = FreeChapterList (bvp->chapter_list);
      return;
    }
    paragraph_count += SEQ_PAN_CHAPTER_SIZE;
    chapter_vnp = chapter_vnp->next;
  }
  /* we reuse an array of SEQ_PAN_CHAPTER_SIZE ValNodePtr to calculate the features for
   * each chapter.  This is allocated once and reused until the chapter list is freed.
   */
  if (bvp->feature_lists == NULL)
  {
  	bvp->feature_lists = (ValNodePtr PNTR) MemNew (SEQ_PAN_CHAPTER_SIZE * sizeof (ValNodePtr));
  }
  bvp->TotalLines = num_paragraphs * CountCommonLines (bvp);
}

static void AdjustSeqEdScrollBar (BioseqViewPtr bvp)
{
  BaR  sb;
  Int4 curr_pos, max_pos;
  RecT r;
  Int4 height, num_visible_lines;
  
  if (bvp == NULL) return;
  sb  = GetSlateVScrollBar ((SlatE)bvp->seqView);
  curr_pos = GetBarValue (sb);

  ObjectRect (bvp->seqView, &r);
  InsetRect (&r, SEQ_Y_OFFSET, SEQ_Y_OFFSET);
  height = r.bottom - r.top;
  num_visible_lines = height / bvp->LineHeight;

  max_pos = bvp->TotalLines - num_visible_lines;
  CorrectBarMax (sb, max_pos);
  if (curr_pos > max_pos)
  {
  	SetBarValue (sb, max_pos);
  }
  else
  {
  	SetBarValue (sb, curr_pos);
  }
  CorrectBarPage(sb, (height / bvp->LineHeight) - 1, (height / bvp->LineHeight) - 1);  
}

/* This function indexes a paragraph.  This function should only be called by IndexSeqPanChapter,
 * as the sequence editor code frequently makes the assumption that if the first paragraph in a
 * chapter is indexed, all of the paragraphs in the chapter are indexed.  Also, it is more
 * efficient to calculate the feature_lists for an entire chapter, rather than for individual
 * paragraphs.
 */
static void IndexSeqPanPara 
(BioseqViewPtr bvp,
 SeqPanParaPtr sppp,
 ValNodePtr feature_list)
{
  Int4       num_lines, prev_lines;
  Int4       i;
  Boolean    show_top_scale = FALSE;
  Boolean    show_features = FALSE;
  ValNodePtr vnp;
  
  if (bvp == NULL || sppp == NULL) return;
  
  prev_lines = CountCommonLines (bvp);
  num_lines = prev_lines;

  if (GetValue (bvp->newNumControl) == eNumTop)
  {
  	show_top_scale = TRUE;
  }
  
  /* add lines for features */
  if (GetValue(bvp->newFeatControl) != eShowFeaturesOff) 
  {
    show_features = TRUE;
    num_lines += ValNodeLen (feature_list);
  }
  
  /* allocate lines and create */
  sppp->lines = (SeqPanLinePtr PNTR) MemNew (num_lines * sizeof (SeqPanLinePtr));
  if (sppp->lines == NULL) return;
  sppp->num_lines = num_lines;
  
  i = 0;
  if (show_top_scale) { /* Numbers on top */
    sppp->lines[i++] = MakeSeqPanLine(eTypeTopSeqNumbers, sppp->sequenceLineOffset);
    sppp->lines[i++] = MakeSeqPanLine(eTypeTopScaleMarks, sppp->sequenceLineOffset);
  }
  sppp->lines[i++] = MakeSeqPanLine(eTypeSequence, sppp->sequenceLineOffset);  /* Add sequence line      */
  if (bvp->ShowComplement)
  {
    sppp->lines[i++] = MakeSeqPanLine (eTypeSequenceComplement, sppp->sequenceLineOffset);
  }
  if (show_features)
  {
  	for (vnp = feature_list; vnp != NULL; vnp = vnp->next)
  	{
  	  sppp->lines[i] = MakeSeqPanLine (eTypeFeature, sppp->sequenceLineOffset);
  	  sppp->lines[i]->idx = vnp->data.intvalue;
  	  if (vnp->choice == PROT_PRODUCT_TYPE)
  	  {
  	  	sppp->lines[i]->protProduct = TRUE;
  	  	sppp->lines[i]->on_the_fly = FALSE;
  	  }
  	  else if (vnp->choice == PROT_ON_THE_FLY)
  	  {
  	  	sppp->lines[i]->protProduct = TRUE;
  	  	sppp->lines[i]->on_the_fly = TRUE;
  	  }
  	  else
  	  {
  	  	sppp->lines[i]->protProduct = FALSE;
  	  	sppp->lines[i]->on_the_fly = FALSE;
  	  }
  	  sppp->lines[i]->row = 0;
  	  i++;
  	}
  }
  if (bvp->frames [0])
    sppp->lines[i++] = MakeSeqPanLine (eTypeFrame1, sppp->sequenceLineOffset);
  if (bvp->frames [1])
    sppp->lines[i++] = MakeSeqPanLine (eTypeFrame2, sppp->sequenceLineOffset);
  if (bvp->frames [2])
    sppp->lines[i++] = MakeSeqPanLine (eTypeFrame3, sppp->sequenceLineOffset);
  if (bvp->frames [3])
    sppp->lines[i++] = MakeSeqPanLine (eTypeFrame4, sppp->sequenceLineOffset);
  if (bvp->frames [4])
    sppp->lines[i++] = MakeSeqPanLine (eTypeFrame5, sppp->sequenceLineOffset);
  if (bvp->frames [5])
    sppp->lines[i++] = MakeSeqPanLine (eTypeFrame6, sppp->sequenceLineOffset);  
  
  sppp->indexed = TRUE;
  bvp->TotalLines += num_lines - prev_lines;
  AdjustSeqEdScrollBar (bvp);
}

/* the array of ValNodePtr lists for each chapter will be reused. */
/* it must have been pre-allocated to SEQ_PAN_CHAPTER_SIZE lists. */
static void
GetSeqChapterFeatureInfo 
(BioseqViewPtr bvp,
 BioseqPtr bsp,
 Int2 lineLength,
 Int4 sequenceLineOffset,
 Uint2 entityID,
 ValNodePtr PNTR feature_lists)
{
  Int4 j, start, stop;
  SeqMgrFeatContext fcontext;
  SeqFeatPtr        sfp = SeqEdGetNextFeature (bsp, NULL, 0, 0, &fcontext, FALSE, FALSE, entityID);
  Int4 chapter_start, chapter_stop;
  
  chapter_start = sequenceLineOffset * lineLength;
  chapter_stop = chapter_start + SEQ_PAN_CHAPTER_SIZE * lineLength;
  for (j = 0; j < SEQ_PAN_CHAPTER_SIZE; j++)
  {
  	feature_lists [j] = ValNodeFree (feature_lists [j]);
  }
    
  while (sfp != NULL) {
  	
    if (fcontext.seqfeattype != SEQFEAT_PUB  &&  
        fcontext.seqfeattype != SEQFEAT_BIOSRC  &&
        ((fcontext.left >= chapter_start && fcontext.left < chapter_stop)
         || (fcontext.right >= chapter_start && fcontext.right < chapter_stop)
         || (fcontext.left <= chapter_start && fcontext.right > chapter_stop)))
    {
      Boolean coding   = fcontext.seqfeattype == SEQFEAT_CDREGION;
      Int4    paraFrom = floor(fcontext.left / lineLength);      /* feature starting paragraph */
      Int4    paraTo   = ceil (fcontext.right / lineLength) + 1;  /* feature ending paragraph   */
      /*BioseqPtr bsp_prot = BioseqFind (SeqLocId(sfp->product));*/
      start = SeqLocStart (sfp->location);
      stop = SeqLocStop (sfp->location);

      if (paraFrom < sequenceLineOffset) 
      {
      	paraFrom = sequenceLineOffset;
      }
      if (paraTo > sequenceLineOffset + SEQ_PAN_CHAPTER_SIZE)
      {
      	paraTo = sequenceLineOffset + SEQ_PAN_CHAPTER_SIZE;
      }
      for (j = paraFrom; j < paraTo; j++) {
        ValNodeAddInt(&(feature_lists[j - sequenceLineOffset]), fcontext.seqfeattype, fcontext.itemID);
        if (coding) 
        {
          ValNodeAddInt(&(feature_lists[j - sequenceLineOffset]), PROT_PRODUCT_TYPE, fcontext.itemID); /* add space for prot product */	
          if (bvp->on_the_fly)
          {
            ValNodeAddInt(&(feature_lists[j - sequenceLineOffset]), PROT_ON_THE_FLY, fcontext.itemID); /* add space for prot product */	
          }
        }
      }
    }
    sfp = SeqEdGetNextFeature (bsp, sfp, 0, 0, &fcontext, FALSE, FALSE, entityID); 
  }  /* while */
}


/* This function indexes a chapter.  It should be called whenever a chapter is about to become
   visible. */
static void 
IndexSeqPanChapter 
(BioseqViewPtr   bvp,
 Int4            lineLength,
 SeqPanParaPtr   sppp,
 Int4            chapter_number,
 ValNodePtr PNTR feature_lists)
{
  Int4          i;
  Int4          start;
  
  if (sppp == NULL) return;
  if (sppp->indexed) return;
    
  start = chapter_number * SEQ_PAN_CHAPTER_SIZE;
  
  /* get features for this block */  
  GetSeqChapterFeatureInfo (bvp, bvp->bsp, lineLength, start, bvp->seq_entityID, feature_lists);
    
  for (i = 0; i < SEQ_PAN_CHAPTER_SIZE && sppp[i].sequenceLineOffset > -1; i++)
  {
    IndexSeqPanPara (bvp, sppp + i, feature_lists[i]);
  }
  AdjustSeqEdScrollBar (bvp);
}


/* This function counts the lines assigned to a chapter - either the estimated lines
 * if the chapter is unindexed, or the actual lines if the chapter is indexed.
 */
static Int4 CountChapterLines (ValNodePtr chapter_vnp, BioseqViewPtr bvp)
{
  SeqPanParaPtr sppp;
  Int4          i;
  Int4          num_lines = 0;
  
  if (chapter_vnp == NULL || bvp == NULL) return 0;
  sppp = (SeqPanParaPtr) chapter_vnp->data.ptrvalue;
  if (sppp == NULL) return 0;
  if (sppp->indexed)
  {
  	for (i = 0; i < SEQ_PAN_CHAPTER_SIZE; i++)
  	{
  	  num_lines += sppp[i].num_lines;
  	}
  }
  else
  {
  	num_lines = SEQ_PAN_CHAPTER_SIZE * CountCommonLines (bvp);
  }
  return num_lines;
}

static void UnindexSeqPanChapter (BioseqViewPtr bvp, ValNodePtr chapter_vnp, Int4 chapter_number)
{
  Int4 new_lines;
  Int4 old_lines;
  Int4 num_paragraphs;
  
  if (chapter_vnp == NULL || bvp == NULL) return;
  
  old_lines = CountChapterLines (chapter_vnp, bvp);
  num_paragraphs = floor(bvp->bsp->length / bvp->CharsAtLine) + 1; /* sequence length */ 

  InitSeqPanChapter ((SeqPanParaPtr) chapter_vnp->data.ptrvalue, 
                     chapter_number * SEQ_PAN_CHAPTER_SIZE, num_paragraphs);
  new_lines = CountChapterLines (chapter_vnp, bvp);
  bvp->TotalLines -= old_lines - new_lines;
  AdjustSeqEdScrollBar (bvp);
}

/* Given a scroll position, find the chapter that that scroll position will fall into.
 * This function should be used to find the first chapter that will need to be indexed
 * when drawing.
 */
static ValNodePtr FindChapterForOffset 
(BioseqViewPtr bvp,
 Int4 offset,
 Int4Ptr chapter_offset,
 Int4Ptr chapter_number)
{
  Int4 estimated_paragraph_size;
  Int4 line;
  ValNodePtr chapter_vnp;
  SeqPanParaPtr sppp;
  Int4          j;
  Int4          pos;
  
  estimated_paragraph_size = CountCommonLines (bvp);
  line = 0;
  pos = 0;
  for (chapter_vnp = bvp->chapter_list; chapter_vnp != NULL; chapter_vnp = chapter_vnp->next)
  {
    *chapter_offset = line;
  	sppp = (SeqPanParaPtr) chapter_vnp->data.ptrvalue;
  	for (j = 0; j < SEQ_PAN_CHAPTER_SIZE && sppp[j].sequenceLineOffset > -1; j++)
  	{
  	  if (sppp[j].indexed)
  	  {
  	  	line += sppp[j].num_lines;
  	  }
  	  else 
  	  {
  	  	line += estimated_paragraph_size;
  	  }
  	}
  	if (line > offset)
  	{
  	  *chapter_number = pos;
  	  return chapter_vnp;
  	}
  	pos++;
  }
  return NULL;
}


/* This function gets the SeqPanLinePtr for the line at scroll position offset. */
static SeqPanLinePtr SeqEdGetSeqPanelLineForOffset (Int4 offset, BioseqViewPtr bvp)
{
  Int4          estimated_paragraph_size;
  Int4          line;
  ValNodePtr    chapter_vnp;
  SeqPanParaPtr sppp;
  Int4          j;
  Int4          chapter_number;
  
  estimated_paragraph_size = CountCommonLines (bvp);
  line = 0;
  for (chapter_vnp = bvp->chapter_list, chapter_number = 0;
       chapter_vnp != NULL;
       chapter_vnp = chapter_vnp->next, chapter_number++)
  {
  	sppp = (SeqPanParaPtr) chapter_vnp->data.ptrvalue;
  	if (! sppp->indexed && line + SEQ_PAN_CHAPTER_SIZE * estimated_paragraph_size > offset)
  	{
  	  IndexSeqPanChapter (bvp, bvp->CharsAtLine, sppp, chapter_number, bvp->feature_lists);
  	}
  	if (sppp->indexed)
  	{
  	  for (j = 0; j < SEQ_PAN_CHAPTER_SIZE && sppp[j].sequenceLineOffset > -1; j++)
  	  {
  	    if (offset < line + sppp[j].num_lines)
  	    {
  	      return sppp[j].lines [offset - line];
  	    }
  	  	line += sppp[j].num_lines;  		
  	  }
  	}
  	else 
  	{
  	  line += estimated_paragraph_size * SEQ_PAN_CHAPTER_SIZE;
  	}
  }
  return NULL;
}


/* This function gets the scroll position for a given sequence location */
static Int4 SeqEdGetScrollPosForSequencePos (Uint4 sequence_pos, BioseqViewPtr bvp)
{
  Int4          desired_line;
  Int4          estimated_paragraph_size;
  Int4          line;
  ValNodePtr    chapter_vnp;
  SeqPanParaPtr sppp;
  Int4          j;
  Int4          chapter_number;
  Int4          chapter_offset;
  
  desired_line = sequence_pos / bvp->CharsAtLine;
  estimated_paragraph_size = CountCommonLines (bvp);
  line = 0;
  for (chapter_vnp = bvp->chapter_list, chapter_number = 0;
       chapter_vnp != NULL;
       chapter_vnp = chapter_vnp->next, chapter_number++)
  {
  	sppp = (SeqPanParaPtr) chapter_vnp->data.ptrvalue;
  	if (desired_line < (chapter_number + 1) * SEQ_PAN_CHAPTER_SIZE)
  	{
  	  chapter_offset = desired_line - chapter_number * SEQ_PAN_CHAPTER_SIZE;
  	  if (! sppp->indexed)
  	  {
  	    IndexSeqPanChapter (bvp, bvp->CharsAtLine, sppp, chapter_number, bvp->feature_lists);
  	  }
  	  for (j = 0; j < chapter_offset; j++)
  	  {
  	  	line += sppp[j].num_lines;
  	  }
  	  return line;
  	}
  	else if (sppp->indexed)
  	{
  	  for (j = 0; j < SEQ_PAN_CHAPTER_SIZE && sppp[j].sequenceLineOffset > -1; j++)
  	  {
  	  	line += sppp[j].num_lines;  		
  	  }
  	}
  	else 
  	{
  	  line += estimated_paragraph_size * SEQ_PAN_CHAPTER_SIZE;
  	}
  }
  return line;
}

static void onSeqEdVScrollBarSeqPanel (BaR sb, SlatE s, Int4 newval, Int4 oldval)
{
  BioseqViewPtr bvp;
  RecT          r_scroll;
  Int2          pixels;
  Int2          lines;
  ValNodePtr    first_chapter_vnp;
  SeqPanParaPtr sppp;
  Int4          first_chapter_offset, chapter_number;
  Int4          bmax, bmax_before;
  Int4          totallines;
  Int4          pixels_same;
  
  Select (s);
  
  bvp = GetBioseqViewPtr((PaneL)s);
  ObjectRect (bvp->seqView, &r_scroll);
  /* the SEQ_Y_OFFSET border is blank */
  InsetRect (&r_scroll, 4, 4);

  /* if we are scrolling into an unindexed chapter, we need to index the
   * new chapter and reset our scroll position
   */
  if (newval != oldval)
  {
    first_chapter_vnp = FindChapterForOffset (bvp, newval, &first_chapter_offset, &chapter_number);
    if (first_chapter_vnp == NULL) return;

    sppp = (SeqPanParaPtr)first_chapter_vnp->data.ptrvalue;
    if (sppp == NULL) return;
    if (! sppp->indexed)
    {
      bmax_before = GetBarMax (sb);
      totallines = bvp->TotalLines;
      IndexSeqPanChapter (bvp, bvp->CharsAtLine, sppp, chapter_number, bvp->feature_lists);
      bmax = GetBarMax (sb);
      newval = bmax * ((double)newval / (double)bmax_before);
      if (newval > bmax)
      {
      	newval = bmax;
      }
      CorrectBarValue (sb, newval);
    }
  }

  lines = (r_scroll.bottom - r_scroll.top) / bvp->LineHeight;
  
  if (abs(oldval-newval) > lines)
  {
    /* none of the old lines are visible - redraw the entire page. */
    InvalRect(&r_scroll);
  }
  else
  {
    pixels = (oldval - newval) * bvp->LineHeight;
    pixels_same = (lines - ABS (oldval - newval)) * bvp->LineHeight;

    ScrollRect (&r_scroll, 0, pixels);

    if (pixels < 0)
    {
      /* redraw the bottom */
      r_scroll.top    += pixels_same;
    }
    else
    {
      /* redraw the top */
      r_scroll.bottom += pixels_same;
    }
    InsetRect (&r_scroll, -1, -1);
    InvalRect (&r_scroll);
  }
  
  Update ();
}


/* This function draws the currently visible paragraph lines. */
static void DrawSeqChapters (BioseqViewPtr bvp)
{
  BaR              sb;
  Int4             start;
  Int4             first_chapter_offset;
  Int4             line;
  ValNodePtr       first_chapter_vnp, chapter_vnp;
  Int4             chapter_number = 0;
  Int4             num_visible_lines;
  SeqPanParaPtr    sppp;
  Int4             c_n;
  Int4             j, k;
  SeqPanLinePtr    splp;
  PaneL            p;
  RecT             r;
  Int4             x, y;
  SeqPortPtr       spp;
  Uint1Ptr         buf;
  Int4             para_offset;
  Int4             seqY = 0;

  if (bvp == NULL) return;
  
  sb  = GetSlateVScrollBar ((SlatE)bvp->seqView);
  start = GetBarValue (sb);
  p = bvp->seqView;
  spp = SeqPortNew (bvp->bsp, 0, bvp->bsp->length-1, Seq_strand_plus, Seq_code_iupacna);
  buf = MemNew (SEQ_GROUP_SIZE+1);

  ObjectRect (p, &r);
  InsetRect (&r, SEQ_Y_OFFSET, SEQ_Y_OFFSET);
  x = r.left + 1;
  y = r.top + bvp->LineHeight;
  SelectFont ((FonT)(bvp->displayFont));  

  num_visible_lines = (r.bottom - r.top) / bvp->LineHeight;

  /* index all the chapters that are visible */
  first_chapter_vnp = FindChapterForOffset (bvp, start, &first_chapter_offset, &chapter_number);
  if (first_chapter_vnp == NULL) return;
    
  for (chapter_vnp = first_chapter_vnp, line = first_chapter_offset, c_n = chapter_number;
       chapter_vnp != NULL && line < start + num_visible_lines;
       chapter_vnp = chapter_vnp->next, c_n++)
  {
  	sppp = (SeqPanParaPtr) chapter_vnp->data.ptrvalue;
    IndexSeqPanChapter (bvp, bvp->CharsAtLine, sppp, c_n, bvp->feature_lists);
  	for (j = 0;
  	     j < SEQ_PAN_CHAPTER_SIZE && sppp[j].sequenceLineOffset > -1 && line < start + num_visible_lines;
  	     j++)
  	{
  	  /* if this paragraph is not visible, skip it */
  	  if (line + sppp[j].num_lines < start)
  	  {
  	  	line += sppp[j].num_lines;
  	  	continue;
  	  }
  	  /* skip over lines that are not visible in this paragraph */
  	  if (line < start)
  	  {
  	  	para_offset = start - line;
  	  }
  	  else
  	  {
  	  	para_offset = 0;
  	  }
  	  for (k = line + para_offset;
  	       k < start + num_visible_lines && k - line < sppp[j].num_lines;
  	       k++)
  	  {
/*  	    DrawMarker (x, y, bvp->LineHeight); */
  	  	splp = sppp[j].lines [k - line];
        switch ( splp->lineType ) 
        {
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
            DrawSequence(x, y, splp->bioSeqLine, spp, buf, bvp);                /* Draw the sequence    */
            if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
            DrawSequenceCodonBoundaries (x, y, bvp->frame_for_codon_draw, splp->bioSeqLine, bvp);
            seqY = y;
            break;
          case eTypeSequenceComplement:
            DrawSequenceComplement(x, y, splp->bioSeqLine, spp, buf, bvp);                       /* Draw the sequence    */
            if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
            break;
          case eTypeFeature:
            if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
            DrawFeature(x, y, splp->bioSeqLine, splp->row, splp->idx,
                        splp->protProduct, splp->on_the_fly, bvp->bsp, bvp);    /* Draw Features        */
            DrawFeatureCodonLines (x, y, splp->bioSeqLine, splp->row, seqY, splp->idx, splp->protProduct, bvp->bsp, bvp);
            break;
      	  case eTypeFrame1:
      	    DrawFrame(x, y, splp->bioSeqLine, 1, bvp);
            if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
     	    break;
      	  case eTypeFrame2:
       	    DrawFrame(x, y, splp->bioSeqLine, 2, bvp);
            if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
      	    break;
     	  case eTypeFrame3:
      	    DrawFrame(x, y, splp->bioSeqLine, 3, bvp);
            if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
      	    break;
      	  case eTypeFrame4:
      	    DrawFrame(x, y, splp->bioSeqLine, 4, bvp);
            if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
      	    break;
      	  case eTypeFrame5:
       	    DrawFrame(x, y, splp->bioSeqLine, 5, bvp);
            if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
      	    break;
       	  case eTypeFrame6:
      	    DrawFrame(x, y, splp->bioSeqLine, 6, bvp);
            if (bvp->DrawGrid) DrawLtGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);
     	    break;
        }
        y += bvp->LineHeight;
  	  }
  	  if(bvp->DrawGrid && k == sppp[j].num_lines)
  	  {
        DrawDkGrid(x, y+bvp->LineSpace/2, r.right, y+bvp->LineSpace/2);             /* Draw Horizontal Grid */
  	  }

  	  line += sppp[j].num_lines;
  	}
  }    
}

#define SEQ_ED_PRINT_SEQ_OFFSET 15

static void PrintMinusProtein 
(CharPtr           dest_str,
 Int4              row,
 SeqMgrFeatContext fcontext,
 Int4              interval_offset,
 Int4              bsStart,
 Int4              bsFinish,
 Int4              frame,
 CharPtr           str_prot,
 BioseqViewPtr     bvp)
{
  Int4 product_start, k;
  Int4 prot_pos;
  Int4 prot_len;
  Int4 aln_pos_right, aln_pos_left;
  Int4 product_pos;
  
  if (dest_str == NULL) return;
  
  product_start = 0;
  if (str_prot == NULL || interval_offset >= fcontext.numivals) return;
  prot_len = StringLen (str_prot);
  for (k = 0; k < interval_offset; k++)
  {
  	product_start += fcontext.ivals [2 * k] - fcontext.ivals [2 * k + 1] + 1;
  }
  /* in the alignment coordinate system, find the mapping for the right end of the
   * interval.
   */
  aln_pos_right = x_Coord(bvp, fcontext.ivals [2 * interval_offset], row);
  
  /* does this interval extend into the next line? */
  if (aln_pos_right > bsFinish - 1)
  {
    if (bvp->seqAlignMode) 
    {
  	  /* need to find out how much of the product appears in lines after this one.
  	   * Can't simply subtract the distance, because we can't count the gap characters
  	   * as part of the product.
  	   */
  	   for (k = fcontext.ivals [2 * interval_offset]; k >= fcontext.ivals [2 * interval_offset + 1]; k--)
  	   {
  	     if (x_Coord (bvp, k, row) <= bsFinish)
  	     {
  	   	   break;
  	     }
  	   }
  	   product_start += fcontext.ivals [2 * interval_offset] - k + 1;
    }
    else
    {
      /* how much of the product appears in lines after this one? */
  	  product_start += fcontext.ivals [ 2 * interval_offset] - bsFinish + 1;
    }
  }

  product_pos = product_start;
  product_pos -= frame - 1;
  aln_pos_left = x_Coord(bvp, fcontext.ivals [2 * interval_offset + 1], row);
  
  if (aln_pos_right > bsFinish - 1)
  {
  	aln_pos_right = bsFinish - 1;
  }
  if (aln_pos_left < bsStart)
  {
  	aln_pos_left = bsStart;
  }

  
  /* we need to step through the alignment coordinates */
  for (k = aln_pos_right; k >= aln_pos_left; k--)
  {
    /* Check for gaps in this feature if in alignment mode */
    if (bvp->seqAlignMode && AlnMgr2MapSeqAlignToBioseq(bvp->salp, k, row) < 0) {
      dest_str [k - bsStart] = '-';
      continue;
    }
    else
    {
      prot_pos = product_pos / 3;
      if (prot_pos >= prot_len)
      {
        continue;
      }
      if (product_pos % 3 == 1)
      {
        dest_str [k - bsStart] = str_prot [prot_pos];
      }    	
      product_pos ++;
    }
  }
}


static void PrintPlusProtein 
(CharPtr           dest_str,
 Int4              row,
 SeqMgrFeatContext fcontext,
 Int4              interval_offset,
 Int4              bsStart,
 Int4              bsFinish,
 Int4              frame,
 CharPtr           str_prot,
 BioseqViewPtr     bvp)
{
  Int4 product_start, k;
  Int4 prot_pos;
  Int4 prot_len;
  Int4 aln_pos_right, aln_pos_left;
  Int4 product_pos;

  product_start = 0;
  if (str_prot == NULL || interval_offset >= fcontext.numivals) return;
  prot_len = StringLen (str_prot);
  for (k = 0; k < interval_offset; k++)
  {
  	product_start += fcontext.ivals [2 * k + 1] - fcontext.ivals [2 * k] + 1;
  }
  /* in the alignment coordinate system, find the mapping for the right end of the
   * interval.
   */
  aln_pos_right = x_Coord(bvp, fcontext.ivals [2 * interval_offset + 1], row);
  aln_pos_left = x_Coord (bvp, fcontext.ivals [2 * interval_offset], row);
  
  /* does this interval extend into the previous line? */
  if (aln_pos_left < bsStart)
  {
    if (bvp->seqAlignMode) 
    {
  	  /* need to find out how much of the product appears in lines before this one.
  	   * Can't simply subtract the distance, because we can't count the gap characters
  	   * as part of the product.
  	   */
  	   for (k = fcontext.ivals [2 * interval_offset]; k <= fcontext.ivals [2 * interval_offset + 1]; k++)
  	   {
  	     if (x_Coord (bvp, k, row) >= bsStart)
  	     {
  	   	   break;
  	     }
  	   }
  	   product_start += k - fcontext.ivals [2 * interval_offset];
    }
    else
    {
      /* how much of the product appears in lines before this one? */
  	  product_start += bsStart - fcontext.ivals [ 2 * interval_offset];
    }
  }

  product_pos = product_start;
  product_pos += frame - 1;
  if (aln_pos_right > bsFinish - 1)
  {
    aln_pos_right = bsFinish - 1;
  }
  if (aln_pos_left < bsStart)
  {
    aln_pos_left = bsStart;
  }
  /* we need to step through the alignment coordinates */
  for (k = aln_pos_left; k <= aln_pos_right; k++)
  {
    /* Check for gaps in this feature if in alignment mode */
    if (bvp->seqAlignMode && AlnMgr2MapSeqAlignToBioseq(bvp->salp, k, row) < 0) {
      dest_str [k - bsStart] = '-';
      continue;
    }
    else
    {
      prot_pos = product_pos / 3;
      if (prot_pos >= prot_len)
      {
        continue;
      }
      if (product_pos % 3 == 1)
      {
        dest_str [k - bsStart] = str_prot [prot_pos];
      }
      product_pos ++;   	
    }
  }
}


static void PrintFeature(CharPtr feature_line, Int4 line, Int4 row, Int2 itemID, 
                        Boolean protProduct, BioseqPtr bsp, BioseqViewPtr bvp)
{
  SeqMgrFeatContext fcontext;
  SeqFeatPtr        sfp;
  BioseqPtr         bsp_prot;
  CharPtr           str_prot;
  Int4              bsStart, bsFinish, ffStart, ffFinish, i;
  Int4              fLeft, fRight;
  CharPtr           no_prot_msg = "Protein sequence is not available";
    
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
  
  if (feature_line == NULL) return;
  MemSet (feature_line, ' ', bvp->CharsAtLine + SEQ_ED_PRINT_SEQ_OFFSET);
  feature_line [bvp->CharsAtLine + SEQ_ED_PRINT_SEQ_OFFSET] = 0;
  
  bsStart  = line    * bvp->CharsAtLine;
  bsFinish = bsStart + bvp->CharsAtLine;
  
  fLeft    = x_FeatLeft (bvp, fcontext.left );
  fRight   = x_FeatRight(bvp, fcontext.right);
  ffStart  = GetFeatureX(x_Coord(bvp, fLeft,  row), bsStart, bsFinish, TRUE ) - bsStart;
  ffFinish = GetFeatureX(x_Coord(bvp, fRight, row), bsStart, bsFinish, FALSE) - bsStart;

  if (fcontext.label != NULL) 
  {
    StrNCpy( feature_line, fcontext.label, MIN (12, StringLen (fcontext.label)));
  }
  
  if (protProduct) {
    bsp_prot = BioseqFind (SeqLocId(sfp->product));
    str_prot = GetSequenceByBsp (bsp_prot);
    if (str_prot == NULL) 
    {
      StringNCpy (feature_line + SEQ_ED_PRINT_SEQ_OFFSET, no_prot_msg, StringLen (no_prot_msg));
      return;
    }
  }
  
  for (i = 0; i != fcontext.numivals; i++) {
    Int4 interval_left = fcontext.strand==Seq_strand_minus ? fcontext.ivals[i*2+1] : fcontext.ivals[i*2];
    Int4 interval_right = fcontext.strand==Seq_strand_minus ? fcontext.ivals[i*2] : fcontext.ivals[i*2+1];
    Int4 draw_left = MAX (interval_left, bsStart);
    Int4 draw_right = MIN (interval_right, bsFinish - 1); 
 
    if (interval_left < bsStart && interval_right < bsStart) continue;
    if (interval_left > bsFinish - 1 && interval_right > bsFinish -1) continue;
        
    if (protProduct) {  /* draw protein product */
      Int4 frame = 1;  /* center in group of 3 */
      
      if (sfp->data.choice == SEQFEAT_CDREGION) {
        CdRegionPtr crp = sfp->data.value.ptrvalue;
        if (crp != NULL ) {
          if (crp->frame == 2) frame++;
          if (crp->frame == 3) frame+=2;
        }
      }

      
      if (fcontext.strand == Seq_strand_minus)
      {
        PrintMinusProtein (feature_line + SEQ_ED_PRINT_SEQ_OFFSET, row, fcontext, i,
                           bsStart, bsFinish, frame, str_prot, bvp);
      }
      else
      {
        PrintPlusProtein (feature_line + SEQ_ED_PRINT_SEQ_OFFSET, row, fcontext, i,
                           bsStart, bsFinish, frame, str_prot, bvp);
      }
    } /* protein product */
    else 
    {  /* draw ASCII feature line */

      if (fcontext.strand == Seq_strand_minus)
      {
      	if (interval_left >= draw_left)
      	{
      	  MemSet (feature_line + SEQ_ED_PRINT_SEQ_OFFSET + draw_left - bsStart,
      	          '<', 1);
      	  draw_left ++;
      	}
      }
      else
      {
      	if (interval_right <= draw_right)
      	{
      	  MemSet (feature_line + SEQ_ED_PRINT_SEQ_OFFSET + draw_right - bsStart,
      	          '>', 1);
      	  draw_right --;
      	}
      }
      if (draw_right > draw_left)
      {
      	MemSet (feature_line + SEQ_ED_PRINT_SEQ_OFFSET + draw_left - bsStart,
      	        '-', draw_right - draw_left + 1);
      }      
    }  /* feature or product */
  }  /* for */
}


static void PrintTranslatedFrame(CharPtr file_line, Int4 line, Int4 frame, BioseqViewPtr bvp)
{
  SeqFeatPtr    fake_cds;
  ByteStorePtr  bs;
  CharPtr       frame_str;
  Uint1         strand; 
  Int4          seqStart; 
  Int4          bsStart, bsFinish, j;
  Int4          pPos, frame_len;
  Char          tmp[2];
  Char          label[50];
  Int4          offset;
  Int4          line_pos;

  if (file_line == NULL || bvp == NULL) return;
  
  MemSet (file_line, ' ', bvp->CharsAtLine + SEQ_ED_PRINT_SEQ_OFFSET);
  file_line [bvp->CharsAtLine + SEQ_ED_PRINT_SEQ_OFFSET] = 0;
  
    
  seqStart  = line    * bvp->CharsAtLine;
  
  /* determine start of line */
  if (frame < 4)
  {
    offset = 3 - ((line * bvp->CharsAtLine) % 3) + frame - 1;	
  	strand = Seq_strand_plus;
  }
  else
  {
    offset = 7 - frame + ((bvp->bsp->length - (line * bvp->CharsAtLine)) % 3);
  	strand = Seq_strand_minus;
  }
  
  if (offset > 2)
  {
  	offset = offset % 3;
  }
  
  if (offset == 2 && line > 0)
  {
  	offset -= 3;
  }

  bsStart = seqStart + offset;
  bsFinish = (line + 1) * bvp->CharsAtLine + offset;
  frame_len = bsFinish - bsStart + 1;
  bsFinish -= frame_len % 3;
  
  if (frame_len % 3 == 2 && bsFinish + 3 < bvp->bsp->length)
  {
  	bsFinish += 3;
  }
  /* must not extend past end of sequence */
  while (bsFinish > bvp->bsp->length - 1)
  {
  	bsFinish -= 3;
  }
  /* should not extend past end of line */
  while (bsFinish > seqStart + bvp->CharsAtLine)
  {
  	bsFinish -= 3;
  }
  frame_len = bsFinish - bsStart + 1;  
  
  fake_cds = make_fake_cds(bvp->bsp, bsStart, bsFinish, strand);
  
  bs = ProteinFromCdRegionEx(fake_cds, TRUE, FALSE);
  SeqFeatFree(fake_cds);
  if(bs == NULL) return;
  frame_str = BSMerge (bs, NULL);
  bs = BSFree (bs);
  if (frame_str == NULL) return;
  frame_len = StringLen (frame_str);
  
  if (frame < 4)
  {
    sprintf(label, "frame +%d", frame);
  }
  else
  {
  	sprintf (label, "frame %d", 3 - frame);
  }
  StringNCpy ((CharPtr)file_line, label, StringLen (label));


  tmp[1] = '\0';
  if (frame < 4)
  {
    for (j = offset + 1, pPos = 0;  j < bsFinish && pPos < frame_len;  j+=3, pPos ++) 
    {
      line_pos = SEQ_ED_PRINT_SEQ_OFFSET + j;
      if (frame_str [pPos] != 0 && line_pos < SEQ_ED_PRINT_SEQ_OFFSET + bvp->CharsAtLine)
        file_line [line_pos] = frame_str[pPos];
    }
  }
  else
  {
    for (j = offset + 1, pPos = frame_len - 1;  j < bsFinish && pPos > -1;  j+=3, pPos --) 
    {
      line_pos = SEQ_ED_PRINT_SEQ_OFFSET + j;
      if (frame_str [pPos] != 0 && line_pos < SEQ_ED_PRINT_SEQ_OFFSET + bvp->CharsAtLine)
        file_line [line_pos] = frame_str[pPos];
    }
  }
  MemFree (frame_str);
}


static void PrintFrame (CharPtr file_line, Int4 frame, Int4 seq_pos, Int4 seq_end, Int4 CharsAtLine)
{
  Int4 offset, i;
  
  if (file_line == NULL) return;
  MemSet (file_line, ' ', CharsAtLine + SEQ_ED_PRINT_SEQ_OFFSET);
  if (frame < 1 || frame > 6) return;
  
  if (frame < 4)
  {
  	offset = frame + 3 - seq_pos % 3;
  	offset = offset % 3;
  	for (i = offset; i < CharsAtLine && i + seq_pos < seq_end; i += 3)
  	{
  	  file_line [i + SEQ_ED_PRINT_SEQ_OFFSET] = '|';
  	}
  }
  else
  {
    frame = frame - 3;
  	if (seq_end > seq_pos + CharsAtLine)
  	{
  	  offset = frame + 3 - (seq_end - (seq_pos + CharsAtLine)) % 3;
  	  offset = offset % 3;
  	  seq_end = seq_pos + CharsAtLine;
  	}
  	else
  	{
  	  offset = frame;
  	}
  	for (i = seq_end - offset - seq_pos - 1; i >= 0; i-=3)
  	{
  	  file_line [i + SEQ_ED_PRINT_SEQ_OFFSET] = '|';
  	}
  }
}


static void ShiftToRemoveDataBeforeToAfterFrom 
(CharPtr file_line, Int4 from, Int4 to, Int4 seq_pos)
{
  Int4    line_len, space_len, i;
  
  if (file_line == NULL) return;

  if (to > seq_pos)
  {
    line_len = StringLen (file_line) - SEQ_ED_PRINT_SEQ_OFFSET;
    if (to < seq_pos + line_len)
    {
      file_line [SEQ_ED_PRINT_SEQ_OFFSET + to - seq_pos + 1] = 0;
    }
  }

  space_len = from - seq_pos;
  if (space_len > 0)
  {
    for (i = 0; i < space_len; i++)
    {
      file_line [SEQ_ED_PRINT_SEQ_OFFSET + i] = ' ';
    }
  }
}


static void SeqEdPrintTextLineToFile 
(CharPtr file_line, Int4 from, Int4 to, Int4 seq_pos, FILE *fp)
{
  if (file_line == NULL) return;
  
  ShiftToRemoveDataBeforeToAfterFrom (file_line, from, to, seq_pos);
  if (StringHasNoText (file_line + SEQ_ED_PRINT_SEQ_OFFSET))
  {
    return;
  }
  fprintf (fp, "%s\n", file_line);
}


/* We need to adjust the output so that it starts with from and ends with to,
 * which means some of the lines may need to be shifted.
 * Problems:  lines from different paragraphs may have different numbers of
 * feature lines, and if we are constructing a merged paragraph, we will need 
 * to make sure that feature lines for the same features line up.
 * We could either print on a delay, or have some form of look-ahead to get the next
 * paragraph...
 */
static void ExportSequenceText (SeqEdFormPtr sefp, Int4 from, Int4 to, FILE *fp)
{
  Int4          c_n, first_c_n, last_c_n;
  ValNodePtr    chapter_vnp;
  SeqPanParaPtr sppp;
  Int4          first_paragraph_number, last_paragraph_number, p_n, l_n;
  Int4          ctr, i, k;
  Uint1Ptr      buf;
  SeqPortPtr    spp;
  BioseqViewPtr bvp;
  SeqPanLinePtr splp;
  Int4          seq_pos;
  Uint1Ptr      file_line;
  Char          label[SEQ_ED_PRINT_SEQ_OFFSET];
  ValNodePtr    curr_para_strings = NULL;
  Int4          line_shift;
  
  if (sefp == NULL) return;
  
  bvp = &(sefp->bfp->bvd);
  line_shift = from % bvp->CharsAtLine;
  
  /* find the first chapter and paragraph we'd want to use */
  first_c_n = from / (SEQ_PAN_CHAPTER_SIZE * bvp->CharsAtLine);
  last_c_n = to / (SEQ_PAN_CHAPTER_SIZE * bvp->CharsAtLine);
  first_paragraph_number = (from % (SEQ_PAN_CHAPTER_SIZE * bvp->CharsAtLine)) / bvp->CharsAtLine;
  last_paragraph_number = (to % (SEQ_PAN_CHAPTER_SIZE * bvp->CharsAtLine)) / bvp->CharsAtLine;
  
  spp = SeqPortNew (bvp->bsp, 0, bvp->bsp->length-1, Seq_strand_plus, Seq_code_iupacna);
  buf = MemNew (bvp->CharsAtLine + 1);
  
  file_line = MemNew (SEQ_ED_PRINT_SEQ_OFFSET + bvp->CharsAtLine + 1);
  if (file_line == NULL) return;
  file_line [bvp->CharsAtLine + 13] = 0;
   
  for (c_n = 0, chapter_vnp = bvp->chapter_list;
       chapter_vnp != NULL && c_n <= last_c_n;
       chapter_vnp = chapter_vnp->next, c_n ++)
  {
    if (c_n < first_c_n)
    {
      continue;
    }
    /* index our chapter */
    sppp = (SeqPanParaPtr)chapter_vnp->data.ptrvalue;
    IndexSeqPanChapter (bvp, bvp->CharsAtLine, sppp, c_n, bvp->feature_lists);
                        
    if (c_n == last_c_n)
    {
      l_n = last_paragraph_number + 1;
    }
    else
    {
      l_n = SEQ_PAN_CHAPTER_SIZE;
    }
    if (c_n > first_c_n)
    {
      first_paragraph_number = 0;
    }
    
    for (p_n = first_paragraph_number; p_n < l_n; p_n++)
    {
      /* print sequence lines, feature lines, and translation lines - other lines can
       * be skipped */
      curr_para_strings = NULL;
      
      for (k = 0; k < sppp [p_n].num_lines; k++)
      {
  	    splp = sppp[p_n].lines [k];
        switch ( splp->lineType )
        {
          case eTypeSequence:
            MemSet (file_line, ' ', bvp->CharsAtLine + SEQ_ED_PRINT_SEQ_OFFSET);
            seq_pos = (c_n * SEQ_PAN_CHAPTER_SIZE + p_n) * bvp->CharsAtLine;
            if (seq_pos < from)
            {
              sprintf (label, "%d", seq_pos + 1 + line_shift);
            }
            else
            {
              sprintf (label, "%d", seq_pos + 1);
            }
            StringNCpy ((CharPtr)file_line, label, StringLen (label));
            ctr = 0;
            SeqPortSeek (spp, seq_pos, SEEK_SET);
            ctr = SeqPortRead (spp, file_line + SEQ_ED_PRINT_SEQ_OFFSET, bvp->CharsAtLine);
            if (ctr > 0) 
            {
              /* set end to NULL if at end of sequence */
              if (ctr < bvp->CharsAtLine) MemSet(buf+ctr, '\0', 1);
              /* convert to lowercase */
              for (i = SEQ_ED_PRINT_SEQ_OFFSET; i < ctr + SEQ_ED_PRINT_SEQ_OFFSET; i++)
              {
              	buf[i] = TO_LOWER (buf[i]);
              } 
              
              SeqEdPrintTextLineToFile ((CharPtr)file_line, from, to, seq_pos, fp);

              if (bvp->frame_for_codon_draw != 0)
              {
                PrintFrame ((CharPtr)file_line, bvp->frame_for_codon_draw, (c_n * SEQ_PAN_CHAPTER_SIZE + p_n) * bvp->CharsAtLine,
                            bvp->bsp->length, bvp->CharsAtLine);
                SeqEdPrintTextLineToFile ((CharPtr)file_line, from, to, seq_pos, fp);
              }
            }  
            break;
          case eTypeFeature:
            PrintFeature((CharPtr)file_line, splp->bioSeqLine, splp->row, splp->idx,
                         splp->protProduct, bvp->bsp, bvp);
            SeqEdPrintTextLineToFile ((CharPtr)file_line, from, to, seq_pos, fp);
            break;
          case eTypeFrame1:
            PrintTranslatedFrame((CharPtr)file_line, splp->bioSeqLine, 1, bvp);
            SeqEdPrintTextLineToFile ((CharPtr)file_line, from, to, seq_pos, fp);
            break;
          case eTypeFrame2:
            PrintTranslatedFrame((CharPtr)file_line, splp->bioSeqLine, 2, bvp);
            SeqEdPrintTextLineToFile ((CharPtr)file_line, from, to, seq_pos, fp);
            break;
          case eTypeFrame3:
            PrintTranslatedFrame((CharPtr)file_line, splp->bioSeqLine, 3, bvp);
            SeqEdPrintTextLineToFile ((CharPtr)file_line, from, to, seq_pos, fp);
            break;
          case eTypeFrame4:
            PrintTranslatedFrame((CharPtr)file_line, splp->bioSeqLine, 4, bvp);
            SeqEdPrintTextLineToFile ((CharPtr)file_line, from, to, seq_pos, fp);
            break;
          case eTypeFrame5:
            PrintTranslatedFrame((CharPtr)file_line, splp->bioSeqLine, 5, bvp);
            SeqEdPrintTextLineToFile ((CharPtr)file_line, from, to, seq_pos, fp);
            break;
          case eTypeFrame6:
            PrintTranslatedFrame((CharPtr)file_line, splp->bioSeqLine, 6, bvp);
            SeqEdPrintTextLineToFile ((CharPtr)file_line, from, to, seq_pos, fp);
            break;
        }
      }
      fprintf (fp, "\n");      
    }
    
    if (c_n == first_c_n)
    {
      first_paragraph_number = 0;
    }
  }
}


/* This function initializes the SeqEdForm structure */
static SeqEdFormPtr SeqEdFormNew (void)
{
  SeqEdFormPtr sefp;
  
  sefp = (SeqEdFormPtr) MemNew (sizeof (SeqEdFormData));
  if (sefp != NULL)
  {
  	sefp->bfp = NULL;
  	sefp->spliteditmode = FALSE;
  	sefp->edit_pos_start = 0;
  	sefp->edit_pos_end = 0;
  	sefp->current_pattern = NULL;
  	sefp->gapvnp = NULL;
  }
  InitJournal (sefp);
  return sefp;
}

static void SeqEdScrollToSeqPosition (SeqEdFormPtr sefp, Int4 pos)
{
  BaR          sb;
  Int4         scroll_pos;
  WindoW       currentport, temport;

  if (sefp == NULL) return;
  if (pos < 0 || pos > sefp->bfp->bvd.bsp->length) return;
  sb = GetSlateVScrollBar ((SlatE) sefp->bfp->bvd.seqView);
  if (sb == NULL) return;
  
  scroll_pos = SeqEdGetScrollPosForSequencePos (pos, &(sefp->bfp->bvd));
  
  SetBarValue (sb, scroll_pos);
  currentport = ParentWindow (sefp->bfp->bvd.seqView);
  temport = SavePort (currentport);
  Select (sefp->bfp->bvd.seqView);
  inval_panel (sefp->bfp->bvd.seqView, -1, -1);	
  RestorePort (temport);
}

static void SeqEdUpdateStatus (SeqEdFormPtr sefp)
{
  Char      str[255];
  Int4      seq_pos, seq_end_pos;
  SeqIdPtr  sip;
  BioseqPtr bsp;
  Char      tmp_buf[42];
  Int4      aln_pos, aln_end_pos, aln_len;
  
  if (sefp == NULL || sefp->position_label == NULL) return;
  
  if (sefp->bfp != NULL && sefp->bfp->bvd.salp != NULL && sefp->bfp->bvd.seqAlignMode)
  {
    if (sefp->bfp->bvd.last_aln_row_clicked > 0)
    {
      sip = AlnMgr2GetNthSeqIdPtr(sefp->bfp->bvd.salp, sefp->bfp->bvd.last_aln_row_clicked);
      bsp = BioseqFind (sip);
      if (bsp != NULL) {
        sip = SeqIdFindBestAccession (bsp->id);
      }
      SeqIdWrite (sip, tmp_buf, PRINTID_TEXTID_ACCESSION, sizeof(tmp_buf) - 1);
      
      aln_pos = sefp->edit_pos_start;
      aln_end_pos = sefp->edit_pos_end;
      seq_pos = AlnMgr2MapSeqAlignToBioseq (sefp->bfp->bvd.salp, 
                                            aln_pos,
                                            sefp->bfp->bvd.last_aln_row_clicked);
      seq_end_pos = AlnMgr2MapSeqAlignToBioseq (sefp->bfp->bvd.salp, 
                                            aln_end_pos,
                                            sefp->bfp->bvd.last_aln_row_clicked);
      while (aln_pos > -1 && seq_pos < 0)
      {
        aln_pos--;
        seq_pos = AlnMgr2MapSeqAlignToBioseq (sefp->bfp->bvd.salp, 
                                              aln_pos,
                                              sefp->bfp->bvd.last_aln_row_clicked);
      }

      aln_len = AlnMgr2GetAlnLength(sefp->bfp->bvd.salp, FALSE); 
      while (aln_end_pos < aln_len && seq_end_pos < 0)
      {        
        seq_end_pos = AlnMgr2MapSeqAlignToBioseq (sefp->bfp->bvd.salp, 
                                                  aln_end_pos,
                                                  sefp->bfp->bvd.last_aln_row_clicked);
        aln_end_pos ++;                                                  
      }
      
      if (seq_pos < 0 && seq_end_pos >= 0)
      {
        seq_pos = 0;
      }
      
      if (seq_pos < 0)
      {
        sprintf (str, "%s", tmp_buf);
      }
      else if (seq_pos == seq_end_pos)
      {
        sprintf (str, "%s %d", tmp_buf, seq_pos);
      }
      else
      {
        sprintf (str, "%s %d-%d", tmp_buf, seq_pos + 1, seq_end_pos);
      }
    }
    else 
    {
      str[0] = 0;
    }
  }
  else
  {
    if (sefp->edit_pos_start == sefp->edit_pos_end)
    {
  	  sprintf (str, "Position %d", sefp->edit_pos_start);
    }
    else
    {
  	  sprintf (str, "Selected %d - %d", sefp->edit_pos_start + 1, sefp->edit_pos_end);
    }
  }
  SetTitle (sefp->position_label, str);	
}

static void SeqEdGoToButton (ButtoN b)
{
  SeqEdFormPtr sefp;
  Char         str [16];
  Int4         val;

  sefp = (SeqEdFormPtr) GetObjectExtra ((WindoW)ParentWindow (b));
  if (sefp == NULL || sefp->bfp == NULL) return;
  
  GetTitle (sefp->goto_txt, str, 15);
  if (! CCStrToLong (str, &val))
  {
  	return;
  }
  
  if (val < 0)
  {
  	val = 0;
  }
  else if (val > sefp->bfp->bvd.bsp->length)
  {
  	val = sefp->bfp->bvd.bsp->length;
  }

  sefp->edit_pos_start = val;  
  sefp->edit_pos_end = val;
  SeqEdScrollToSeqPosition (sefp, val);
  SeqEdUpdateStatus (sefp);
}

static void SeqEdLookAtButton (ButtoN b)
{
  SeqEdFormPtr sefp;
  Char         str [16];
  Int4         val;

  sefp = (SeqEdFormPtr) GetObjectExtra ((WindoW)ParentWindow (b));
  if (sefp == NULL || sefp->bfp == NULL) return;
  
  GetTitle (sefp->lookat_txt, str, 15);
  if (! CCStrToLong (str, &val))
  {
  	return;
  }
  
  if (val < 0)
  {
  	val = 0;
  }
  else if (val > sefp->bfp->bvd.bsp->length)
  {
  	val = sefp->bfp->bvd.bsp->length;
  }
  SeqEdScrollToSeqPosition (sefp, val);
}

static void SeqEdSelectFeatEditMode (PopuP p)
{
  Int2         val;
  SeqEdFormPtr sefp;

  sefp = (SeqEdFormPtr) GetObjectExtra ((WindoW)ParentWindow (p));
  val = GetValue(p); 
  if (val==1) 
     sefp->spliteditmode = FALSE;
  else if (val==2) 
     sefp->spliteditmode = TRUE;
  CaptureSlateFocus ((SlatE) sefp->bfp->bvd.seqView);
}

static void RepositionSeqEdPanel (WindoW w)
{
  SeqEdFormPtr       sefp;
  RecT               rw;    /* window rect        */
  RecT               rp;    /* panel rect         */
  RecT               rct;   /* new rect for panel */
  RecT               rb;    /* button group rect  */
  Int4               buttonheight, buttonwidth;

  sefp = (SeqEdFormPtr) GetObjectExtra (w);
  if (sefp == NULL) return;
  
  ObjectRect (w, &rw);
  GetPosition (sefp->bfp->bvd.seqView, &rp);
  GetPosition ((GrouP) sefp->lower_button_group, &rb);
  buttonwidth  = rb.right  - rb.left;
  buttonheight = rb.bottom - rb.top;
  LoadRect ( &rct, rp.left, rp.top, (Int4)(rw.right - rw.left - rp.left), 
            (Int4)(rw.bottom - rw.top - buttonheight - 3 * SEQ_Y_OFFSET));
  SetPosition (sefp->bfp->bvd.seqView, &rct );
  AdjustPrnt (sefp->bfp->bvd.seqView, &rct, FALSE);
  LoadRect (&rb, rb.left, rct.bottom + SEQ_Y_OFFSET, rb.left + buttonwidth,
            rct.bottom + buttonheight + SEQ_Y_OFFSET);
  SetPosition(sefp->lower_button_group, &rb);
  AdjustPrnt (sefp->lower_button_group, &rb, FALSE);
}

static Int4 XCoordToSeqPos (Int2 x, Int2 xcoord, BioseqViewPtr bvp)
{
  Int4 num_chars;
  Int4 seqXPos;
  
  if (bvp == NULL) return 0;
  xcoord -= x;
  xcoord -= SEQ_X_OFFSET;
  xcoord -= bvp->SeqStartPosX;
  num_chars = xcoord / bvp->CharWidth;
  seqXPos = 0;
  while (num_chars > SEQ_GROUP_SIZE)
  {
  	seqXPos += SEQ_GROUP_SIZE;
  	num_chars -= SEQ_GROUP_SIZE + 1;
  }
  seqXPos += num_chars;
  return seqXPos;
}


static Int4 SeqEdGetLineForPosition (SeqEdFormPtr sefp, PoinT pt, Int4Ptr x, Int4Ptr y)
{
  BaR  sb;
  RecT rp;
  Int4 start, screen_offset;
  Int4 rval;
  Int4 max;
  Int4 direction = 0;
  Int4 screen_lines;
  
  if (sefp == NULL || x == NULL || y == NULL) return 0;

  sb  = GetSlateVScrollBar ((SlatE)sefp->bfp->bvd.seqView);
  max = GetBarMax (sb);
  
  ObjectRect (sefp->bfp->bvd.seqView, &rp);
  InsetRect (&rp, SEQ_Y_OFFSET, SEQ_Y_OFFSET);

  start = GetBarValue (sb);
  screen_offset = (pt.y - rp.top) / sefp->bfp->bvd.LineHeight;
  if (screen_offset < 0)
  {
    if (start > 0)
    {
      direction = -1;
      rval = start - 1;
    }
    else
    {
      rval = 0;
    }
  }
  else if (pt.y >= rp.bottom - sefp->bfp->bvd.LineHeight)
  {
    screen_lines = (rp.bottom - rp.top) / sefp->bfp->bvd.LineHeight;
  	if (start < max)
  	{
  	  direction = 1;
  	  rval = start + screen_lines + 1;
  	}
  	else
  	{
  	  rval = sefp->bfp->bvd.TotalLines - 1;
  	}
  }
  else
  {
  	rval = start + screen_offset;
  }
  *x = XCoordToSeqPos (rp.left, pt.x, &(sefp->bfp->bvd));
  if (rval > sefp->bfp->bvd.TotalLines - 1)
  {
  	rval = sefp->bfp->bvd.TotalLines - 1;
  }
  *y = rval;

  return direction;
}

static Int4 
FindClickedFeatureInterval 
(Int4 seq_pos, SeqMgrFeatContext fcontext, BoolPtr is_left_end, BoolPtr is_right_end)
{
  Int4 j = 0;
  
  if (fcontext.ivals == NULL || is_left_end == NULL || is_right_end == NULL) return -1;
  for (j = 0; j < fcontext.numivals; j ++)
  {
    if (seq_pos >= fcontext.ivals [2 * j] && seq_pos <= fcontext.ivals [2 * j + 1])
    {
      /* inside plus strand interval */
      if (seq_pos == fcontext.ivals [2 * j])
      {
      	*is_left_end = TRUE;
      }
      else
      {
      	*is_left_end = FALSE;
      }
      if (seq_pos == fcontext.ivals [2 * j + 1])
      {
      	*is_right_end = TRUE;
      }
      else
      {
      	*is_right_end = FALSE;
      }
      return j;
    }
    else if (seq_pos >= fcontext.ivals [2 * j + 1] && seq_pos <= fcontext.ivals [2 * j])
    {
      /* inside minus strand interval */
      if (seq_pos == fcontext.ivals [2 * j + 1])
      {
      	*is_left_end = TRUE;
      }
      else
      {
      	*is_left_end = FALSE;
      }
      if (seq_pos == fcontext.ivals [2 * j])
      {
      	*is_right_end = TRUE;
      }
      else
      {
      	*is_right_end = FALSE;
      }
      return j;
    }
  }
  return -1;
}


static void SeqEdTranslateAndJournalOneCDS (SeqFeatPtr sfp, SeqEdFormPtr sefp)
{
  SeqEdJournalPtr end, next, prev, sejp;
  
  if (sefp == NULL) return;
  
  /* find correct position in journal list */
  for (end = sefp->last_journal_entry; end != NULL && end->action != eSeqEdJournalEnd; end = end->next)
  {	
  }
  if (sefp->last_journal_entry != end)
  {
    next = (SeqEdJournalPtr) sefp->last_journal_entry->next;
    if (next != end)
    {
      /* set prev for next in list to NULL */  
      next->prev = NULL;
      /* set next for next to last in list to NULL */
      ((SeqEdJournalPtr)(end->prev))->next = NULL;
      /* free middle of list */
      SeqEdJournalFree (next);
      
      /* reattach end */
      sefp->last_journal_entry->next = end;
      end->prev = sefp->last_journal_entry;
    }
    sefp->last_journal_entry = end;
  }
  sejp = SeqEdJournalNewTranslate (sfp, sefp->bfp->bvd.bsp, sefp->input_entityID);
  prev = end->prev;
  prev->next = sejp;
  sejp->prev = prev;
  sejp->next = end;
  end->prev = sejp;
  sefp->last_journal_entry = sejp;
  SeqEdTranslateOneCDS (sfp, sefp->bfp->bvd.bsp, sefp->input_entityID);	

  SetUndoRedoStatus (sefp);
}


static void SeqEdTranslateCDSCmd (SeqEdFormPtr sefp)
{
  ValNodePtr        vnp;
  SeqFeatPtr        sfp;
  SeqMgrFeatContext fcontext;
  
  if (sefp == NULL || sefp->bfp == NULL) return;
  
  for (vnp = sefp->bfp->bvd.Selection; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->choice != 1) continue;
    
  	sfp = SeqMgrGetDesiredFeature (0, sefp->bfp->bvd.bsp, vnp->data.intvalue, 0, NULL, &fcontext);
    SeqEdTranslateAndJournalOneCDS (sfp, sefp);
  }
  Select (sefp->bfp->bvd.seqView);
  inval_panel (sefp->bfp->bvd.seqView, -1, -1);
  CaptureSlateFocus ((SlatE) sefp->bfp->bvd.seqView); 
}

static void SeqEdTranslateCDSBtn (ButtoN b)
{
  SeqEdFormPtr sefp;
  
  sefp = (SeqEdFormPtr) GetObjectExtra (b);
  SeqEdTranslateCDSCmd (sefp);
}


static void SeqEdTranslateCDSItem (IteM i)
{
  SeqEdFormPtr sefp;
  
  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  SeqEdTranslateCDSCmd (sefp);
}

static void SeqEdSelectFeature (SeqEdFormPtr sefp, Int4 idx)
{
  if (sefp == NULL) return;
  
  sefp->bfp->bvd.Selection = ValNodeNew (NULL);
  if (sefp->bfp->bvd.Selection != NULL)
  {
    sefp->bfp->bvd.Selection->data.intvalue = idx;
    sefp->bfp->bvd.Selection->choice = 1;
  }    	
}

static Int4 FindBestNonGapPosition (Int4 seq_offset, BioseqPtr bsp, Boolean push_left)
{
  DeltaSeqPtr dsp;
  Int4        cum_offset = 0, seg_len;
  SeqLocPtr   slp;
  SeqLitPtr   slip;
  
  if (seq_offset < 0 || bsp == NULL)
  {
    return 0;
  }
  else if (seq_offset > bsp->length)
  {
    return bsp->length;
  }
  else if (bsp->repr != Seq_repr_delta
      || bsp->seq_ext_type != 4)
  {
    return seq_offset;
  }
  
  dsp = (DeltaSeqPtr) bsp->seq_ext;
  while (dsp != NULL)
  {
    seg_len = 0;
		if (dsp->choice == 1) 
		{  /* SeqLoc */
		  slp = (SeqLocPtr)(dsp->data.ptrvalue);
		  seg_len = SeqLocLen (slp);
		  if (seq_offset >= cum_offset && seq_offset < cum_offset + seg_len)
		  {
		    return seq_offset;
		  }
		  cum_offset += seg_len;
		}
		else if (dsp->choice == 2)
		{
		  slip = (SeqLitPtr) (dsp->data.ptrvalue);
		  seg_len = slip->length;
		  if (seq_offset >= cum_offset && seq_offset < cum_offset + seg_len)
		  {
		    if (slip->seq_data == NULL && slip->fuzz != NULL && slip->fuzz->choice == 4)
		    {
		      if (push_left)
		      {
		        return cum_offset;
		      }
		      else
		      {
		        return cum_offset + seg_len;
		      }
		    }
		    else
		    {
		      return seq_offset;
		    }
		  }
		  cum_offset += seg_len;
		}
		dsp = dsp->next;
  }
  return cum_offset;
}

static void RemapSeqEdIntervalForGap (SeqEdFormPtr sefp)
{
  Int4        start_remap, end_remap;
  
  if (sefp == NULL || sefp->bfp == NULL 
      || sefp->bfp->bvd.bsp == NULL
      || sefp->bfp->bvd.bsp->repr != Seq_repr_delta
      || sefp->bfp->bvd.bsp->seq_ext_type != 4)
  {
    return;
  }
  
  /* cannot select nucleotides inside a gap of unknown length.
   * selection must include all of a gap of unknown length or none
   * of a gap of unknown length.
   */
  
  start_remap = FindBestNonGapPosition (sefp->edit_pos_start, 
                                        sefp->bfp->bvd.bsp,
                                        TRUE);
  if (sefp->edit_pos_start == sefp->edit_pos_end)
  {
    sefp->edit_pos_start = start_remap;
    sefp->edit_pos_end = start_remap;
    sefp->edit_pos_orig = start_remap;
  }
  else
  {
    end_remap = FindBestNonGapPosition (sefp->edit_pos_end,
                                        sefp->bfp->bvd.bsp,
                                        FALSE);
    if (sefp->edit_pos_start == sefp->edit_pos_orig)
    {
      sefp->edit_pos_orig = start_remap;
    }
    else
    {
      sefp->edit_pos_orig = end_remap;
    }
    sefp->edit_pos_start = start_remap;
    sefp->edit_pos_end = end_remap;
  }
   
}

static void SeqEdOnClick (PaneL pnl, PoinT pt)
{
  WindoW            w;
  SeqEdFormPtr      sefp;
  Int4              direction;
  Int4              x, y;
  SeqPanLinePtr     splp;
  SeqMgrFeatContext fcontext;
  SeqFeatPtr        sfp;
  Boolean           is_double_click;
  Boolean           is_left_end, is_right_end;
  Boolean           partial5, partial3;
	
  w = ParentWindow(pnl);
  if (w == NULL) return;
  sefp = (SeqEdFormPtr) GetObjectExtra (w);
  if (sefp == NULL) return;
  
  /* need to get double_click state before functions that might call
   * CountCommonLines, which checks the state of popups, which clears
   * the dblClick flag
   */
  is_double_click = dblClick;
  
  direction = SeqEdGetLineForPosition (sefp, pt, &x, &y);
  splp = SeqEdGetSeqPanelLineForOffset (y, &(sefp->bfp->bvd));
  if (splp->lineType == eTypeSequence)
  {
    sefp->feature_to_drag = NULL;
    sefp->feature_orig_loc = SeqLocFree (sefp->feature_orig_loc);
    if (x < 0)
    {
      sefp->edit_pos_start = 0;
      sefp->edit_pos_end = sefp->bfp->bvd.bsp->length;
      sefp->edit_pos_orig = sefp->edit_pos_start;
    }
    else
    {
      sefp->edit_pos_start = x + splp->bioSeqLine * sefp->bfp->bvd.CharsAtLine;
      if (sefp->edit_pos_start > sefp->bfp->bvd.bsp->length)
      {
        sefp->edit_pos_start = sefp->bfp->bvd.bsp->length;
      }
      sefp->edit_pos_end = sefp->edit_pos_start;
      sefp->edit_pos_orig = sefp->edit_pos_start;
    }
    RemapSeqEdIntervalForGap (sefp);
    Select (sefp->bfp->bvd.seqView);
    inval_panel (sefp->bfp->bvd.seqView, -1, -1);
    SeqEdUpdateStatus (sefp);
    CaptureSlateFocus ((SlatE) sefp->bfp->bvd.seqView); 
  } 
  else if (splp->lineType == eTypeFeature) 
  {
    /* make sure this is a feature that is "real" (i.e., not a gap, which we can't edit) */
    sfp = SeqMgrGetDesiredFeature (0, sefp->bfp->bvd.bsp, splp->idx, 0, NULL, &fcontext);
    if (sfp == NULL || sfp->idx.subtype == FEATDEF_gap)
    {
      return;
    }
    if (is_double_click) 
    {
      sefp->feature_to_drag = NULL;
      sefp->feature_orig_loc = SeqLocFree (sefp->feature_orig_loc);

      WatchCursor ();
      Update ();
      GatherProcLaunch (OMPROC_EDIT, FALSE, sfp->idx.entityID, splp->idx, OBJ_SEQFEAT, 0, 0, OBJ_SEQFEAT, 0);
      ArrowCursor ();
      Update ();
    } 
    else
    {
      if (!isSelected (splp->idx, &(sefp->bfp->bvd)))
      {
        SeqEdSelectFeature (sefp, splp->idx);
      }
      else
      {
      	sefp->bfp->bvd.Selection = ValNodeFree (sefp->bfp->bvd.Selection);
      }
      sefp->feature_to_drag = SeqMgrGetDesiredFeature (0, sefp->bfp->bvd.bsp, splp->idx, 0, NULL, &fcontext);
      sefp->feature_orig_loc = SeqLocFree (sefp->feature_orig_loc);
      sefp->feature_drag_interval = FindClickedFeatureInterval (x + splp->bioSeqLine * sefp->bfp->bvd.CharsAtLine, fcontext, &is_left_end, &is_right_end);
      if (is_left_end)
      {
      	sefp->feature_drag_type = eLeftEnd;
      }
      else if (is_right_end)
      {
      	sefp->feature_drag_type = eRightEnd;
      }
      else
      {
      	sefp->feature_drag_type = eSlide;
      }
      sefp->feature_orig_loc = SeqLocMerge (sefp->bfp->bvd.bsp, sefp->feature_to_drag->location, NULL,
                                            FALSE, FALSE, FALSE);
      CheckSeqLocForPartial (sefp->feature_to_drag->location, &partial5, &partial3);
      SetSeqLocPartial (sefp->feature_orig_loc, partial5, partial3);
      sefp->feature_drag_origin = x + splp->bioSeqLine * sefp->bfp->bvd.CharsAtLine;
      sefp->last_drag_change = 0;
      Select (sefp->bfp->bvd.seqView);
      inval_panel (sefp->bfp->bvd.seqView, -1, -1);
      CaptureSlateFocus ((SlatE) sefp->bfp->bvd.seqView);    
    }
  }
  else if (splp->lineType == eTypeFrame1)
  {
    if (sefp->bfp->bvd.frame_for_codon_draw == 1)
    {
      sefp->bfp->bvd.frame_for_codon_draw = 0;
    }
    else
    {
      sefp->bfp->bvd.frame_for_codon_draw = 1;    	
    }
    Select (sefp->bfp->bvd.seqView);
    inval_panel (sefp->bfp->bvd.seqView, -1, -1);
    sefp->feature_to_drag = NULL;
    sefp->feature_orig_loc = SeqLocFree (sefp->feature_orig_loc);
    CaptureSlateFocus ((SlatE) sefp->bfp->bvd.seqView);     
  }
  else if (splp->lineType == eTypeFrame2)
  {
    if (sefp->bfp->bvd.frame_for_codon_draw == 2)
    {
      sefp->bfp->bvd.frame_for_codon_draw = 0;
    }
    else
    {
      sefp->bfp->bvd.frame_for_codon_draw = 2;    	
    }
    Select (sefp->bfp->bvd.seqView);
    inval_panel (sefp->bfp->bvd.seqView, -1, -1);
    sefp->feature_to_drag = NULL;
    sefp->feature_orig_loc = SeqLocFree (sefp->feature_orig_loc);
    CaptureSlateFocus ((SlatE) sefp->bfp->bvd.seqView);     
  }
  else if (splp->lineType == eTypeFrame3)
  {
    if (sefp->bfp->bvd.frame_for_codon_draw == 3)
    {
      sefp->bfp->bvd.frame_for_codon_draw = 0;
    }
    else
    {
      sefp->bfp->bvd.frame_for_codon_draw = 3;    	
    }
    Select (sefp->bfp->bvd.seqView);
    inval_panel (sefp->bfp->bvd.seqView, -1, -1);
    sefp->feature_to_drag = NULL;
    sefp->feature_orig_loc = SeqLocFree (sefp->feature_orig_loc);
    CaptureSlateFocus ((SlatE) sefp->bfp->bvd.seqView);     
  }
  else if (splp->lineType == eTypeFrame4)
  {
    if (sefp->bfp->bvd.frame_for_codon_draw == 4)
    {
      sefp->bfp->bvd.frame_for_codon_draw = 0;
    }
    else
    {
      sefp->bfp->bvd.frame_for_codon_draw = 4;    	
    }
    Select (sefp->bfp->bvd.seqView);
    inval_panel (sefp->bfp->bvd.seqView, -1, -1);
    sefp->feature_to_drag = NULL;
    sefp->feature_orig_loc = SeqLocFree (sefp->feature_orig_loc);
    CaptureSlateFocus ((SlatE) sefp->bfp->bvd.seqView);     
  }
  else if (splp->lineType == eTypeFrame5)
  {
    if (sefp->bfp->bvd.frame_for_codon_draw == 5)
    {
      sefp->bfp->bvd.frame_for_codon_draw = 0;
    }
    else
    {
      sefp->bfp->bvd.frame_for_codon_draw = 5;    	
    }
    Select (sefp->bfp->bvd.seqView);
    inval_panel (sefp->bfp->bvd.seqView, -1, -1);
    sefp->feature_to_drag = NULL;
    sefp->feature_orig_loc = SeqLocFree (sefp->feature_orig_loc);
    CaptureSlateFocus ((SlatE) sefp->bfp->bvd.seqView);     
  }
  else if (splp->lineType == eTypeFrame6)
  {
    if (sefp->bfp->bvd.frame_for_codon_draw == 6)
    {
      sefp->bfp->bvd.frame_for_codon_draw = 0;
    }
    else
    {
      sefp->bfp->bvd.frame_for_codon_draw = 6;    	
    }
    Select (sefp->bfp->bvd.seqView);
    inval_panel (sefp->bfp->bvd.seqView, -1, -1);
    sefp->feature_to_drag = NULL;
    sefp->feature_orig_loc = SeqLocFree (sefp->feature_orig_loc);
    CaptureSlateFocus ((SlatE) sefp->bfp->bvd.seqView);     
  }
  else
  {
    sefp->feature_to_drag = NULL;
    sefp->feature_orig_loc = SeqLocFree (sefp->feature_orig_loc);
    CaptureSlateFocus ((SlatE) sefp->bfp->bvd.seqView);     
  }
}

/* we only need to recalculate the feature lines for the paragraph we have just dragged
 * the feature ends into or out of.
 */
static Boolean NeedToReindex (SeqEdFormPtr sefp, Int4 last_change, Int4 this_change)
{
  Int4          left, right;
  Int4          old_top_line, old_bottom_line;
  Int4          new_top_line, new_bottom_line;
  Int4          change_line_top, change_line_bottom;
  Int4          top_chapter_number, bottom_chapter_number, c_n;
  BioseqViewPtr bvp;
  ValNodePtr    chapter_vnp;
  
  if (sefp == NULL || sefp->feature_to_drag == NULL || last_change == this_change)
  {
    return FALSE;  	
  }
  if (! SeqEdGetNthIntervalEndPoints (sefp->feature_orig_loc,
                                      sefp->feature_drag_interval,
                                       &left, &right))
  {
    return FALSE;  	
  }
  bvp = &(sefp->bfp->bvd);

  old_top_line = (left + last_change) / bvp->CharsAtLine;
  new_top_line = (left + this_change) / bvp->CharsAtLine;
  old_bottom_line = (right + last_change) / bvp->CharsAtLine;
  new_bottom_line = (right + this_change) / bvp->CharsAtLine;
  
  if (old_top_line == new_top_line && old_bottom_line == new_bottom_line)
  {
    return FALSE;  	
  }
  if (old_top_line != new_top_line)
  {
    change_line_top = MIN (old_top_line, new_top_line);
  }
  else
  {
  	change_line_top = MIN (old_bottom_line, new_bottom_line);
  }
  if (old_bottom_line != new_bottom_line)
  {
  	change_line_bottom = MAX (old_bottom_line, new_bottom_line);
  }
  else
  {
  	change_line_bottom = MAX (old_top_line, new_top_line);
  }
  top_chapter_number = change_line_top / SEQ_PAN_CHAPTER_SIZE;
  bottom_chapter_number = change_line_bottom / SEQ_PAN_CHAPTER_SIZE;
  for (chapter_vnp = sefp->bfp->bvd.chapter_list, c_n = 0;
       chapter_vnp != NULL && c_n <= bottom_chapter_number;
       chapter_vnp = chapter_vnp->next, c_n++)
  {
  	if (c_n >= top_chapter_number)
  	{
      UnindexSeqPanChapter (bvp, chapter_vnp, c_n);
      IndexSeqPanChapter (bvp, bvp->CharsAtLine, (SeqPanParaPtr)chapter_vnp->data.ptrvalue,
                          c_n, bvp->feature_lists);  	
  	}
  }
  return TRUE;
}

static void ScrollToDragPos (SeqEdFormPtr sefp, Int4 change, Int4 direction)
{
  BioseqViewPtr bvp;
  Int4          desired_scroll_pos, current_scroll_pos;
  BaR           sb;
  Int4          bar_max;
  RecT          r;
  Int4          height, num_visible_lines, paragraph_number = 0, feat_offset;
  Int4          chapter_number, chapter_offset;
  ValNodePtr    chapter_vnp;
  SeqPanParaPtr sppp;
    
  if (sefp == NULL) return;
  
  bvp = &(sefp->bfp->bvd);
  sb  = GetSlateVScrollBar ((SlatE)bvp->seqView);
  if (sb == NULL) return;
  bar_max = GetBarMax (sb);
  desired_scroll_pos = SeqEdGetScrollPosForSequencePos (sefp->feature_drag_origin + change, bvp);
  if (direction > 0 && desired_scroll_pos < bar_max)
  {
  	desired_scroll_pos ++;
  }
  else if (direction < 0 && desired_scroll_pos > 0)
  {
  	desired_scroll_pos --;
  }
  current_scroll_pos = GetBarValue (sb);
  if (desired_scroll_pos < current_scroll_pos)
  {
  	SetBarValue (sb, current_scroll_pos - 1);
  }
  else 
  {
    ObjectRect (bvp->seqView, &r);
    InsetRect (&r, SEQ_Y_OFFSET, SEQ_Y_OFFSET);
    height = r.bottom - r.top;
    num_visible_lines = height / bvp->LineHeight;
    
    chapter_vnp = FindChapterForOffset (bvp, desired_scroll_pos,
                                        &chapter_offset, &chapter_number);
    if (chapter_vnp == NULL)
    {
      return;
    }
    paragraph_number = (sefp->feature_drag_origin + change / bvp->CharsAtLine) % SEQ_PAN_CHAPTER_SIZE;
    sppp = (SeqPanParaPtr) chapter_vnp->data.ptrvalue;
  	for (feat_offset = 0;
  	     feat_offset < sppp[paragraph_number].num_lines 
  	       && sppp[paragraph_number].lines[feat_offset]->idx != 
  	          sefp->feature_to_drag->idx.itemID;
  	     feat_offset ++)
  	{
  	}
    if (desired_scroll_pos + feat_offset > current_scroll_pos + num_visible_lines - 2)
    {
  	  SetBarValue (sb, current_scroll_pos + 1);	  
    }
  }
  
}


/* we don't want to drag the right end up past the left end
 * and we don't want to drag the left end down past the right end.
 */
static Int4 SeqEdCorrectFeatureChange 
(SeqLocPtr slp, Int4 interval_offset, Int4 change, EMoveType move_type, BioseqPtr bsp)
{
  Int4 left, right;
  Int4 corrected_change;
  
  if (slp == NULL || bsp == NULL) return 0;
  
  if (! SeqEdGetNthIntervalEndPoints (slp, interval_offset, &left, &right)) return 0;
  
  corrected_change = change;
  switch (move_type)
  {
  	case eLeftEnd:
  	  if (left + change < 0)
  	  {
  	  	corrected_change = 0 - left;
  	  }
  	  else if (left + change > bsp->length - 1)
  	  {
  	  	corrected_change = bsp->length - 1 - left;
  	  }
  	  break;
  	case eRightEnd:
  	  if (right + change < 0)
  	  {
  	  	corrected_change = 0 - right;
  	  }
  	  else if (right + change > bsp->length - 1)
  	  {
  	  	corrected_change = bsp->length - 1 - right;
  	  }
  	  break;
  	case eSlide:
  	  if (change < 0)
  	  {
  	    if (left + change < 0)
  	    {
  	  	  corrected_change = 0 - left;
  	    }	  	
  	  }
  	  else
  	  {
  	  	if (right + change > bsp->length - 1)
  	  	{
  	  	  corrected_change = bsp->length - 1 - right;
  	  	}
  	  }
  	  break;
  }
  return corrected_change;
}


static void SeqEdOnDrag (PaneL pnl, PoinT pt)
{
  WindoW           w;
  SeqEdFormPtr     sefp;
  Int4             direction;
  Int4             x, y, drag_pos, old_scroll_pos;
  SeqPanLinePtr    splp;
  BaR              sb;
  Int4             corrected_pos, drag_line;
  Int4             change, corrected_change;
	
  w = ParentWindow(pnl);
  if (w == NULL) return;
  sefp = (SeqEdFormPtr) GetObjectExtra (w);
  if (sefp == NULL) return;

  direction = SeqEdGetLineForPosition (sefp, pt, &x, &y);
  splp = SeqEdGetSeqPanelLineForOffset (y, &(sefp->bfp->bvd));
  if (splp == NULL) return;
  
  if (sefp->feature_to_drag != NULL)
  {
    if (!isSelected (sefp->feature_to_drag->idx.itemID, &(sefp->bfp->bvd)))
    {
      SeqEdSelectFeature (sefp, splp->idx);
    }

    change = x + splp->bioSeqLine * sefp->bfp->bvd.CharsAtLine - sefp->feature_drag_origin;
    corrected_change = SeqEdCorrectFeatureChange (sefp->feature_orig_loc,
                                                  sefp->feature_drag_interval,
                                                  change,
                                                  sefp->feature_drag_type,
                                                  sefp->bfp->bvd.bsp);
    corrected_pos = sefp->feature_drag_origin + corrected_change;
              
    if (corrected_pos < 0)
    {
      return;
    }
    drag_line = corrected_pos / sefp->bfp->bvd.CharsAtLine;

    SeqEdFeatureAdjust (sefp->feature_to_drag,
                        sefp->feature_orig_loc,
                        corrected_pos - sefp->feature_drag_origin,
                        sefp->feature_drag_type,
                        sefp->feature_drag_interval,
                        sefp->bfp->bvd.bsp);
                            
    if (direction < 0 && drag_line > 0)
    {
      drag_line --;
    }
    if (direction > 0)
    {
      drag_line ++;
    }
    
    if (NeedToReindex (sefp, sefp->last_drag_change, corrected_change) || direction != 0)
    {
      ScrollToDragPos (sefp, corrected_change, direction);
    }    
    sefp->last_drag_change = corrected_change;
    Select (sefp->bfp->bvd.seqView);
    inval_panel (sefp->bfp->bvd.seqView, -1, -1);
    CaptureSlateFocus ((SlatE) sefp->bfp->bvd.seqView);     
  }
  else if (splp->lineType == eTypeSequence)
  {
  	drag_pos = x + splp->bioSeqLine * sefp->bfp->bvd.CharsAtLine;
  	if (drag_pos >= sefp->edit_pos_orig) 
  	{
  	  drag_pos++;
  	}
  	if (drag_pos < 0)
  	{
  	  drag_pos = 0;
  	}
  	if (drag_pos > sefp->bfp->bvd.bsp->length)
  	{
  	  drag_pos = sefp->bfp->bvd.bsp->length;
  	}
  	if (drag_pos < sefp->edit_pos_orig)
  	{
      sefp->edit_pos_start = drag_pos;
  	  sefp->edit_pos_end = sefp->edit_pos_orig;
  	}
  	else
  	{
  	  sefp->edit_pos_start = sefp->edit_pos_orig;
  	  sefp->edit_pos_end = drag_pos;	
  	}
  	RemapSeqEdIntervalForGap (sefp);
  	if (direction != 0)
    {
      sb  = GetSlateVScrollBar ((SlatE)sefp->bfp->bvd.seqView);
      old_scroll_pos = GetBarValue (sb);
      SetBarValue (sb, old_scroll_pos + direction);
    }
    Select (sefp->bfp->bvd.seqView);
    inval_panel (sefp->bfp->bvd.seqView, -1, -1);
    SeqEdUpdateStatus (sefp);
  }
  else
  {
    sefp->feature_to_drag = NULL;
    if (direction != 0)
    {
      sb  = GetSlateVScrollBar ((SlatE)sefp->bfp->bvd.seqView);
      old_scroll_pos = GetBarValue (sb);
      SetBarValue (sb, old_scroll_pos + direction);
    }
  }
  
}

static void SeqEdOnRelease (PaneL pnl, PoinT pt)
{
  WindoW           w;
  SeqEdFormPtr     sefp;
  SeqEdJournalPtr  sejp, prev, end, next;
	
  w = ParentWindow(pnl);
  if (w == NULL) return;
  sefp = (SeqEdFormPtr) GetObjectExtra (w);
  if (sefp == NULL || sefp->last_journal_entry == NULL) return;
  
  if (sefp->feature_to_drag == NULL)
  {
  	return;
  }
  /* don't bother adding a journal entry if the location didn't change */
  if (SeqLocCompare (sefp->feature_to_drag->location, sefp->feature_orig_loc) == SLC_A_EQ_B)
  {
  	return;
  }
  
  for (end = sefp->last_journal_entry; end != NULL && end->action != eSeqEdJournalEnd; end = end->next)
  {	
  }
  if (sefp->last_journal_entry != end)
  {
    next = (SeqEdJournalPtr) sefp->last_journal_entry->next;
    if (next != end)
    {
      /* set prev for next in list to NULL */  
      next->prev = NULL;
      /* set next for next to last in list to NULL */
      ((SeqEdJournalPtr)(end->prev))->next = NULL;
      /* free middle of list */
      SeqEdJournalFree (next);
      
      /* reattach end */
      sefp->last_journal_entry->next = end;
      end->prev = sefp->last_journal_entry;
    }
    sefp->last_journal_entry = end;
  }
  SeqEdRepairIntervalOrder (sefp->feature_to_drag, sefp->bfp->bvd.bsp);
  sejp = SeqEdJournalNewFeatEdit (eSeqEdFeatMove, sefp->feature_to_drag, 
                                  sefp->feature_orig_loc,
                                  sefp->bfp->bvd.bsp, 
                                  sefp->bfp->bvd.bsp->mol, sefp->input_entityID);
  sefp->feature_orig_loc = NULL;
  sefp->feature_to_drag = NULL;
  prev = end->prev;
  prev->next = sejp;
  sejp->prev = prev;
  sejp->next = end;
  end->prev = sejp;
  sefp->last_journal_entry = sejp;

  SetUndoRedoStatus (sefp);
}

static void SetSeqEdPanelClick (SeqEdFormPtr sefp)
{
  if (sefp == NULL) return;
  SetPanelClick(sefp->bfp->bvd.seqView, SeqEdOnClick, SeqEdOnDrag, NULL, SeqEdOnRelease);
}


static void SeqAlnToggleItemSelection (BioseqViewPtr bvp, Int2 itemID)
{
  ValNodePtr vnp;
  
  for (vnp = bvp->Selection; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->data.intvalue == itemID)
    {
      if (vnp->choice == 1)
      {
        vnp->choice = 0;
      }
      else
      {
        vnp->choice = 1;
      }
      return;    
    }
  }
  ValNodeAddInt (&bvp->Selection, 1, itemID);
}


static void SeqAlnOnClick (PaneL pnl, PoinT pt)
{
  WindoW            w;
  SeqEdFormPtr      sefp;
  Int4              direction;
  Int4              x, y;
  SeqPanLinePtr     splp;
  SeqIdPtr          sip;
  BioseqPtr         bsp;
  Boolean           is_double_click;
  Boolean           is_ctrl;
  Int4              aln_pos;
	
  w = ParentWindow(pnl);
  if (w == NULL) return;
  sefp = (SeqEdFormPtr) GetObjectExtra (w);
  if (sefp == NULL) return;
  
  is_double_click = dblClick;
  is_ctrl = ctrlKey;
  
  direction = SeqEdGetLineForPosition (sefp, pt, &x, &y);
  if (y < 0 || y > sefp->bfp->bvd.TotalLines - 1 || direction != 0)
  {
    return;
  }
  splp = sefp->bfp->bvd.SeqPanLines [y];
  if (splp->lineType == eTypeAlignSequence)
  {
    sip = AlnMgr2GetNthSeqIdPtr(sefp->bfp->bvd.salp, splp->row);
    bsp = BioseqFind (sip);
    if (bsp != NULL)
    {
      if (!is_ctrl)
      {
        sefp->bfp->bvd.Selection = ValNodeFree (sefp->bfp->bvd.Selection);
      }
      SeqAlnToggleItemSelection (&(sefp->bfp->bvd), bsp->idx.itemID);
      inval_panel (sefp->bfp->bvd.seqView, -1, -1);
      
      aln_pos = x + splp->bioSeqLine * sefp->bfp->bvd.CharsAtLine;
      sefp->edit_pos_start = aln_pos;
      sefp->edit_pos_end = sefp->edit_pos_start;
      sefp->edit_pos_orig = sefp->edit_pos_start;
      sefp->bfp->bvd.last_aln_row_clicked = splp->row;
      SeqEdUpdateStatus (sefp);
    }
  } 
  CaptureSlateFocus ((SlatE) sefp->bfp->bvd.seqView);     

}

static void SeqAlnOnDrag (PaneL pnl, PoinT pt)
{
  WindoW        w;
  SeqEdFormPtr  sefp;
  Boolean       is_double_click, is_ctrl;
  Int4          direction;
  SeqPanLinePtr splp;
  Int4          x, y, drag_pos, old_scroll_pos;
  BaR           sb;
  
  w = ParentWindow(pnl);
  if (w == NULL) return;
  sefp = (SeqEdFormPtr) GetObjectExtra (w);
  if (sefp == NULL) return;
  
  is_double_click = dblClick;
  is_ctrl = ctrlKey;
  
  direction = SeqEdGetLineForPosition (sefp, pt, &x, &y);
  if (y < 0 || y > sefp->bfp->bvd.TotalLines - 1)
  {
    return;
  }
  splp = sefp->bfp->bvd.SeqPanLines [y];
  if (splp->lineType == eTypeAlignSequence 
      && splp->row == sefp->bfp->bvd.last_aln_row_clicked)
  {
  	drag_pos = x + splp->bioSeqLine * sefp->bfp->bvd.CharsAtLine;
  	if (drag_pos >= sefp->edit_pos_orig) 
  	{
  	  drag_pos++;
  	}
  	if (drag_pos < 0)
  	{
  	  drag_pos = 0;
  	}
  	if (drag_pos > sefp->bfp->bvd.bsp->length)
  	{
  	  drag_pos = sefp->bfp->bvd.bsp->length;
  	}
  	if (drag_pos < sefp->edit_pos_orig)
  	{
      sefp->edit_pos_start = drag_pos;
  	  sefp->edit_pos_end = sefp->edit_pos_orig;
  	}
  	else
  	{
  	  sefp->edit_pos_start = sefp->edit_pos_orig;
  	  sefp->edit_pos_end = drag_pos;	
  	}
  }
  if (direction != 0)
  {
    sb  = GetSlateVScrollBar ((SlatE)sefp->bfp->bvd.seqView);
    old_scroll_pos = GetBarValue (sb);
    SetBarValue (sb, old_scroll_pos + direction);
  }
  Select (sefp->bfp->bvd.seqView);
  inval_panel (sefp->bfp->bvd.seqView, -1, -1);
  SeqEdUpdateStatus (sefp);
}


static void SetSeqAlnPanelClick (SeqEdFormPtr sefp)
{
  if (sefp == NULL) return;
/*  SetPanelClick(sefp->bfp->bvd.seqView, SeqAlnOnClick, SeqEdOnDrag, NULL, SeqEdOnRelease);*/
  SetPanelClick(sefp->bfp->bvd.seqView, SeqAlnOnClick, SeqAlnOnDrag, NULL, NULL);
}


static void ResizeSeqEdView (SeqEdFormPtr sefp)
{
  RecT r;
  Int4 height, width;
  BioseqViewPtr bvp;
  BaR  sb;
  Int4 scroll_pos;
  SeqPanLinePtr splp;
  Int4 sequence_pos = 0;
  
  if (sefp == NULL) return;
  
  bvp = &(sefp->bfp->bvd);
  if (bvp->seqAlignMode)
  {
    ResizeSeqView (bvp);
    SetSeqAlnPanelClick (sefp);
    return;
  }
  sb = GetSlateVScrollBar ((SlatE)bvp->seqView);
  scroll_pos = GetBarValue(sb);
  splp = SeqEdGetSeqPanelLineForOffset (scroll_pos, bvp);
  if (splp != NULL)
  {
    sequence_pos = splp->bioSeqLine * bvp->CharsAtLine;  	
  }

  ObjectRect (bvp->seqView, &r);
  InsetRect (&r, SEQ_Y_OFFSET, SEQ_Y_OFFSET);
  width  = r.right  - r.left;
  height = r.bottom - r.top;

  bvp->BlocksAtLine = (width - SEQ_X_OFFSET - bvp->SeqStartPosX) / ((SEQ_GROUP_SIZE + 1) * bvp->CharWidth);
  if (bvp->BlocksAtLine == 0) bvp->BlocksAtLine = 1; /* Always at least 1 block */

  bvp->CharsAtLine = SEQ_GROUP_SIZE * bvp->BlocksAtLine;
  /* This function will add to the chapter list if it is not already big enough. */
  CreateSeqPanChapterList (bvp->CharsAtLine, bvp);
  AdjustSeqEdScrollBar (bvp);
  
  scroll_pos = SeqEdGetScrollPosForSequencePos (sefp->edit_pos_start, bvp);
  SetBarValue (sb, scroll_pos);

  SetSeqEdPanelClick(sefp);
}


static void ResizeSeqEditorWindow (WindoW w)
{
  SeqEdFormPtr       sefp;
  WindoW             temport;
  
  sefp = (SeqEdFormPtr) GetObjectExtra (w);
  if (sefp == NULL) return;
  temport = SavePort(w);
  Select (sefp->bfp->bvd.seqView);
  RepositionSeqEdPanel (w);
  if (sefp->bfp->bvd.seqAlignMode)
  {
    ResizeSeqView (&(sefp->bfp->bvd));
    SetSeqAlnPanelClick (sefp);
  }
  else
  {
    ResizeSeqEdView (sefp);
  }
  Select (sefp->bfp->bvd.seqView);
  inval_panel (sefp->bfp->bvd.seqView, -1, -1);
  RestorePort (temport);
  Update ();
  return;
}

static void ChangeSeqEditControls (PopuP p)

{
  SeqEdFormPtr  sefp;

  sefp = (SeqEdFormPtr) GetObjectExtra (p);
  if (sefp == NULL) return;
  
  sefp->bfp->bvd.DrawGrid   = GetValue (sefp->bfp->bvd.newGridControl) == 1;

  ResizeSeqEdView (sefp);
  Select (sefp->bfp->bvd.seqView);
  inval_panel (sefp->bfp->bvd.seqView, -1, -1);
}

static void SeqEdAccept (SeqEdFormPtr sefp)

{
  Uint2         entityID;
  
  if (sefp == NULL) return;
  Hide (sefp->form);
   
  entityID = ObjMgrGetEntityIDForPointer (sefp->bfp->bvd.bsp);
  ObjMgrSetDirtyFlag (entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
  Remove (sefp->form); 
  Update ();	
}
static void SeqEdAcceptButton (ButtoN b)

{
  SeqEdFormPtr  sefp;
  
  sefp = (SeqEdFormPtr) GetObjectExtra (b);
  SeqEdAccept (sefp);
}

static void SeqEdAcceptMenuItem (IteM i)

{
  SeqEdFormPtr  sefp;
  
  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  SeqEdAccept (sefp);
}

static void SetUndoRedoStatus (SeqEdFormPtr sefp);

static void SeqEdCancel (SeqEdFormPtr sefp)

{
  Uint2         entityID;

  if (sefp == NULL) return;
  Hide (sefp->form);
  if (UnplayJournal ( &sefp->last_journal_entry, -1))
  {
    entityID = ObjMgrGetEntityIDForPointer (sefp->bfp->bvd.bsp);
    ObjMgrSetDirtyFlag (entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
    Remove (sefp->form); 
    Update (); 
  } 
}

static void SeqEdCancelButton (ButtoN b)
{
  SeqEdFormPtr  sefp;

  sefp = (SeqEdFormPtr) GetObjectExtra (b);
  SeqEdCancel (sefp);	
}

static void SeqEdCancelMenuItem (IteM i)

{
  SeqEdFormPtr  sefp;

  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  SeqEdCancel (sefp);	
}

static void SeqEdComplement (IteM i)
{
  SeqEdFormPtr  sefp;

  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL) return;
  
  sefp->bfp->bvd.ShowComplement = GetStatus (i);
  ResizeSeqEdView (sefp);
  Select (sefp->bfp->bvd.seqView);
  inval_panel (sefp->bfp->bvd.seqView, -1, -1);  
}

static void SetFrameMenuMulti (SeqEdFormPtr sefp)
{
  if (sefp == NULL) return;

  /* set all plus flag */
  if (sefp->bfp->bvd.frames [0] && sefp->bfp->bvd.frames [1]
      && sefp->bfp->bvd.frames [2])	
  {
  	SetStatus (sefp->frames[3], TRUE);
  }
  else
  {
  	SetStatus (sefp->frames[3], FALSE);
  }
  
  /* set all minus flag */
  if (sefp->bfp->bvd.frames [3] && sefp->bfp->bvd.frames [4]
      && sefp->bfp->bvd.frames [5])	
  {
  	SetStatus (sefp->frames[7], TRUE);
  }
  else
  {
  	SetStatus (sefp->frames[7], FALSE);
  }
  
  /* set all flag */
  if (sefp->bfp->bvd.frames [0] && sefp->bfp->bvd.frames [1]
      && sefp->bfp->bvd.frames [2] && sefp->bfp->bvd.frames [3]
      && sefp->bfp->bvd.frames [4] && sefp->bfp->bvd.frames [5])	
  {
    SetStatus (sefp->frames[8], TRUE);
  }
  else
  {
  	SetStatus (sefp->frames[8], FALSE);
  }
  
  /* set none flag */
  if (sefp->bfp->bvd.frames [0] || sefp->bfp->bvd.frames [1]
      || sefp->bfp->bvd.frames [2] || sefp->bfp->bvd.frames [3]
      || sefp->bfp->bvd.frames [4] || sefp->bfp->bvd.frames [5])	
  {
    SetStatus (sefp->frames[9], FALSE);
  }
  else
  {
  	SetStatus (sefp->frames[9], TRUE);
  }

  /* set individual frame flags */
  SetStatus (sefp->frames[0], sefp->bfp->bvd.frames [0]);
  SetStatus (sefp->frames[1], sefp->bfp->bvd.frames [1]);
  SetStatus (sefp->frames[2], sefp->bfp->bvd.frames [2]);
  SetStatus (sefp->frames[4], sefp->bfp->bvd.frames [3]);
  SetStatus (sefp->frames[5], sefp->bfp->bvd.frames [4]);
  SetStatus (sefp->frames[6], sefp->bfp->bvd.frames [5]); 

  ResizeSeqEdView (sefp);
  Select (sefp->bfp->bvd.seqView);
  inval_panel (sefp->bfp->bvd.seqView, -1, -1); 
}

static void SeqEdFrame1 (IteM i)
{
  SeqEdFormPtr  sefp;

  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL) return;
  
  sefp->bfp->bvd.frames [0] = GetStatus (i);
  SetFrameMenuMulti (sefp);	
}

static void SeqEdFrame2 (IteM i)
{
  SeqEdFormPtr  sefp;

  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL) return;
  
  sefp->bfp->bvd.frames [1] = GetStatus (i);
  SetFrameMenuMulti (sefp);	  	
}

static void SeqEdFrame3 (IteM i)
{
  SeqEdFormPtr  sefp;

  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL) return;
  
  sefp->bfp->bvd.frames [2] = GetStatus (i);
  SetFrameMenuMulti (sefp);	  	
}

static void SeqEdFrameAllPlus (IteM i)
{
  SeqEdFormPtr  sefp;
  Boolean       b;

  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL) return;

  b = GetStatus (i);
  sefp->bfp->bvd.frames [0] = b;
  sefp->bfp->bvd.frames [1] = b;
  sefp->bfp->bvd.frames [2] = b;
  SetFrameMenuMulti (sefp);	  	
}

static void SeqEdFrame4 (IteM i)
{
  SeqEdFormPtr  sefp;

  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL) return;
  
  sefp->bfp->bvd.frames [3] = GetStatus (i);
  SetFrameMenuMulti (sefp);	  	
}

static void SeqEdFrame5 (IteM i)
{
  SeqEdFormPtr  sefp;

  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL) return;
  
  sefp->bfp->bvd.frames [4] = GetStatus (i);
  SetFrameMenuMulti (sefp);	  	
}

static void SeqEdFrame6 (IteM i)
{
  SeqEdFormPtr  sefp;

  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL) return;
  
  sefp->bfp->bvd.frames [5] = GetStatus (i);
  SetFrameMenuMulti (sefp);	  	
}

static void SeqEdFrameAllMinus (IteM i)
{
  SeqEdFormPtr  sefp;
  Boolean       b;

  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL) return;

  b = GetStatus (i);
  sefp->bfp->bvd.frames [3] = b;
  sefp->bfp->bvd.frames [4] = b;
  sefp->bfp->bvd.frames [5] = b;
  SetFrameMenuMulti (sefp);	  	
}

static void SeqEdFrameAll (IteM i)
{
  SeqEdFormPtr  sefp;
  Boolean       b;

  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL) return;

  b = GetStatus (i);  
  sefp->bfp->bvd.frames [0] = b;
  sefp->bfp->bvd.frames [1] = b;
  sefp->bfp->bvd.frames [2] = b;
  sefp->bfp->bvd.frames [3] = b;
  sefp->bfp->bvd.frames [4] = b;
  sefp->bfp->bvd.frames [5] = b;
  SetFrameMenuMulti (sefp);	  		
}

static void SeqEdFrameNone (IteM i)
{
  SeqEdFormPtr  sefp;

  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL) return;
  
  sefp->bfp->bvd.frames [0] = FALSE;
  sefp->bfp->bvd.frames [1] = FALSE;
  sefp->bfp->bvd.frames [2] = FALSE;
  sefp->bfp->bvd.frames [3] = FALSE;
  sefp->bfp->bvd.frames [4] = FALSE;
  sefp->bfp->bvd.frames [5] = FALSE;
  SetFrameMenuMulti (sefp);	  		  
}

static void SeqEdToggleProteinMismatch (IteM i)
{
  SeqEdFormPtr  sefp;

  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL) return;
  sefp->bfp->bvd.show_translation_errors = ! sefp->bfp->bvd.show_translation_errors;
  Select (sefp->bfp->bvd.seqView);
  inval_panel (sefp->bfp->bvd.seqView, -1, -1);  
}

static void SeqEdToggleProteinOnTheFly (IteM i)
{
  SeqEdFormPtr  sefp;

  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL) return;
  sefp->bfp->bvd.on_the_fly = GetStatus (i);
  ResizeSeqEdView (sefp);
  Select (sefp->bfp->bvd.seqView);
  inval_panel (sefp->bfp->bvd.seqView, -1, -1);  
}

static void SeqEdDeleteDeltaSeq (SeqEdFormPtr sefp, Boolean save_clip)
{
  Int4        curr_pos = 0, slip_start, delete_start, delete_len;
  ESeqEdJournalAction action;
  SeqLocPtr   slp;
  SeqLitPtr   slip = NULL;
  DeltaSeqPtr dsp, dsp_next;
  Boolean     ok_to_continue = TRUE;
  Boolean     is_unknown_gap;
  CharPtr     clip_string = NULL, tmp_string;
  Int4        clip_len = 0;
  
  if (sefp == NULL
      || sefp->bfp->bvd.bsp == NULL || sefp->bfp->bvd.bsp->repr != Seq_repr_delta
      || sefp->bfp->bvd.bsp->seq_ext_type != 4 || sefp->bfp->bvd.bsp->seq_ext == NULL
      || sefp->edit_pos_start < 0)
  {
    return;
  }
  
  dsp = (DeltaSeqPtr) sefp->bfp->bvd.bsp->seq_ext;
  while (dsp != NULL && curr_pos < sefp->edit_pos_end && ok_to_continue)
  {
    dsp_next = dsp->next;
    if (dsp->data.ptrvalue == NULL) continue;
    slip_start = curr_pos;
    slip = NULL;
		if (dsp->choice == 1) 
		{  /* SeqLoc */
		  slp = (SeqLocPtr)(dsp->data.ptrvalue);
      curr_pos += SeqLocLen (slp);
		}
		else if (dsp->choice == 2)
		{
		  slip = (SeqLitPtr) (dsp->data.ptrvalue);
		  curr_pos += slip->length;
		}
		if (slip != NULL && curr_pos > sefp->edit_pos_start)
		{
		  is_unknown_gap = FALSE;
		  if (slip->seq_data == NULL)
		  {
		    action = eSeqEdDeleteGap;
		    if (slip->fuzz != NULL && slip->fuzz->choice == 4)
		    {
		      is_unknown_gap = TRUE;
		    }
		  }
		  else
		  {
		    action = eSeqEdDelete;
		  }
		  if (slip_start < sefp->edit_pos_start)
		  {
		    delete_start = sefp->edit_pos_start;
		  }
		  else
		  {
		    delete_start = slip_start;
		  }
		  if (sefp->edit_pos_end > slip_start + slip->length)
		  {
		    delete_len = slip->length - (delete_start - slip_start);
		  }
		  else
		  {
		    delete_len = sefp->edit_pos_end - delete_start;
		  }
		  if (is_unknown_gap && (delete_start > slip_start || delete_len < slip->length))
		  {
		    /* don't delete part of an unknown gap.  delete if all is selected,
		     * otherwise leave it alone */
		     if (delete_start > slip_start)
		     {
		       /* we won't be deleting in the unknown gap, so move the cursor to the end */
		       sefp->edit_pos_start = slip_start + slip->length;
		     }
		  }
		  else
		  {
        ok_to_continue = AddJournalEntryEx (action, delete_start - clip_len,
                                            delete_len, NULL,
                                            sefp, is_unknown_gap);
        clip_len += delete_len;
        tmp_string = (CharPtr) MemNew ((clip_len + 1) * sizeof (Char));
        if (tmp_string != NULL)
        {
          if (clip_string != NULL)
          {
            StringCpy (tmp_string, clip_string);
          }
          if (action == eSeqEdDeleteGap)
          {
            MemSet (tmp_string + clip_len - delete_len, 'N', delete_len);
          }
          else
          {
            StringNCpy (tmp_string, sefp->last_journal_entry->char_data, delete_len);
          }
          tmp_string [clip_len] = 0;
          
          clip_string = MemFree (clip_string);
          clip_string = tmp_string;
        }
		  }
		}
		dsp = dsp_next;
  }
  if (clip_string != NULL)
  {
    if (save_clip)
    {
      StringToClipboard (clip_string);
    }
    clip_string = MemFree (clip_string);
  }
  sefp->edit_pos_end = sefp->edit_pos_start;
}

static void SeqEdCutDeltaSeq (SeqEdFormPtr sefp)
{
  SeqEdDeleteDeltaSeq (sefp, TRUE);
}

static void SeqEdDelete (SeqEdFormPtr sefp, Boolean save_clip)
{  
  if (sefp == NULL || sefp->edit_pos_start == sefp->edit_pos_end) return;
  
  if (sefp->bfp->bvd.bsp->repr == Seq_repr_delta)
  {
    /* construct separate journal entries for gaps and non-gaps */
    SeqEdDeleteDeltaSeq (sefp, save_clip);
  }
  else
  {
    if (AddJournalEntry (eSeqEdDelete, sefp->edit_pos_start,
                         sefp->edit_pos_end - sefp->edit_pos_start,
                         NULL, sefp))
    {
      sefp->edit_pos_end = sefp->edit_pos_start;
      if (save_clip)
      {
        StringToClipboard (sefp->last_journal_entry->char_data);
      }
    }
  }
}

static void SeqEdCut (SeqEdFormPtr sefp)
{
  SeqEdDelete (sefp, TRUE);
  ResizeSeqEdView (sefp);
  Select (sefp->bfp->bvd.seqView);
  inval_panel (sefp->bfp->bvd.seqView, -1, -1);
  SeqEdUpdateStatus (sefp);
}

static void SeqEdCutMenuItem (IteM i)
{
  SeqEdFormPtr sefp;

  if (i == NULL) return;
  sefp = (SeqEdFormPtr) GetObjectExtra (i);

  SeqEdCut (sefp);	
}

static void SeqEdPaste (SeqEdFormPtr sefp)
{
  CharPtr      str;
  CharPtr      insert_str;
  
  if (sefp == NULL) return;
  
  str = ClipboardToString ();

  insert_str = char_to_insert (str, StringLen (str), sefp->bfp->bvd.bsp->mol);
  if (insert_str == NULL) return;
  if (sefp->edit_pos_start != sefp->edit_pos_end)
  {
    SeqEdDelete (sefp, FALSE);
  }
  if (AddJournalEntry (eSeqEdInsert, sefp->edit_pos_start,
   	                   StringLen (insert_str), insert_str, sefp))
  {
    sefp->edit_pos_start += StringLen (str);
    sefp->edit_pos_end += StringLen (str);
    ResizeSeqEdView (sefp);
    Select (sefp->bfp->bvd.seqView);
    inval_panel (sefp->bfp->bvd.seqView, -1, -1);
    SeqEdUpdateStatus (sefp);
  } 
}

static void SeqEdPasteMenuItem (IteM i)
{
  SeqEdFormPtr sefp;

  if (i == NULL) return;
  sefp = (SeqEdFormPtr) GetObjectExtra (i);

  SeqEdPaste (sefp);	
}

static void ChooseGapType (GrouP g)
{
  TexT t;
  
  t = GetObjectExtra (g);
  if (t == NULL)
  {
    return;
  }
  if (GetValue (g) == 2)
  {
    Enable (t);
  }
  else
  {
    Disable (t);
  }
}

static void MakeGapFeats (
  BioseqPtr bsp,
  Pointer userdata
)

{
  Char             buf [32];
  Int4             currpos = 0;
  BioseqPtr        fakebsp = NULL;
  IntFuzzPtr       fuzz;
  ValNodePtr PNTR  gapvnp;
  ImpFeatPtr       ifp;
  SeqLitPtr        litp;
  SeqAnnotPtr      sap = NULL;
  SeqFeatPtr       sfp;
  SeqIdPtr         sip;
  SeqLocPtr        slp;
  ValNodePtr       vnp;

  if (bsp == NULL || bsp->repr != Seq_repr_delta) return;
  gapvnp = (ValNodePtr PNTR) userdata;
  sip = SeqIdFindBest (bsp->id, 0);
  if (sip == NULL) return;

  for (vnp = (ValNodePtr)(bsp->seq_ext); vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == 1) {
      slp = (SeqLocPtr) vnp->data.ptrvalue;
      if (slp == NULL) continue;
      currpos += SeqLocLen (slp);
    }
    if (vnp->choice == 2) {
      litp = (SeqLitPtr) vnp->data.ptrvalue;
      if (litp == NULL) continue;
      if (litp->seq_data == NULL && litp->length > 0) {
        if (fakebsp == NULL) {
          /* to be freed with MemFree, not BioseqFree */
          fakebsp = MemNew (sizeof (Bioseq));
          if (fakebsp == NULL) return;
          sap = SeqAnnotNew ();
          if (sap == NULL) return;
          sap->type = 1;
          fakebsp->annot = sap;
          ValNodeAddPointer (gapvnp, 0, (Pointer) fakebsp);
        }
        ifp = ImpFeatNew ();
        if (ifp == NULL) continue;
        ifp->key = StringSave ("gap");
        sfp = SeqFeatNew ();
        if (sfp == NULL) continue;
        sfp->data.choice = SEQFEAT_IMP;
        sfp->data.value.ptrvalue = (Pointer) ifp;
        sfp->next = (SeqFeatPtr) sap->data;
        sap->data = (Pointer) sfp;
        fuzz = litp->fuzz;
        if (fuzz != NULL && fuzz->choice == 4 && fuzz->a == 0) {
          AddQualifierToFeature (sfp, "estimated_length", "unknown");
          sfp->location = AddIntervalToLocation (NULL, sip, currpos, currpos + litp->length - 1, FALSE, FALSE);
        } else {
          sprintf (buf, "%ld", (long) litp->length);
          AddQualifierToFeature (sfp, "estimated_length", buf);
          sfp->location = AddIntervalToLocation (NULL, sip, currpos, currpos + litp->length - 1, FALSE, FALSE);
        }
      }
      currpos += litp->length;
    }
  }
}

static void FreeSeqEdFormGapFeatList (SeqEdFormPtr sefp)
{
  BioseqPtr   bsp;
  SeqAnnotPtr sap, sapnext;

  if (sefp == NULL || sefp->gapvnp == NULL)
  {
    return;
  }
  
  bsp = (BioseqPtr) sefp->gapvnp->data.ptrvalue;
  if (bsp != NULL) {
    sap = bsp->annot;
    while (sap != NULL) {
      sapnext = sap->next;
      SeqAnnotFree (sap);
      sap = sapnext;
    }
  }
  /* frees fake Bioseq that was created by MemNew, not BioseqNew */
  sefp->gapvnp = ValNodeFreeData (sefp->gapvnp);
}

static void SeqEdReindexGaps (SeqEdFormPtr sefp, BioseqPtr edit_bsp)
{
  FreeSeqEdFormGapFeatList (sefp);
  
  MakeGapFeats (edit_bsp, &(sefp->gapvnp));
  SeqMgrClearFeatureIndexes (sefp->input_entityID, NULL);
  SeqMgrIndexFeaturesExEx (sefp->input_entityID, NULL, FALSE, FALSE, sefp->gapvnp);
}

static void SeqEdInsertGapMenuItem (IteM i)
{
  SeqEdFormPtr          sefp;
  ModalAcceptCancelData acd;
  WindoW                w;
  GrouP                 h, g, k, gap_type_grp, c;
  TexT                  gap_len_txt;
  ButtoN                b;
  Boolean               done;
  CharPtr               gap_len_str;
  Int4                  gap_type, gap_len;
  Boolean               is_unknown_gap;
  Boolean               changed = FALSE;

  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL || sefp->edit_pos_start < 0)
  {
    return;
  }
  
  w = MovableModalWindow(-20, -13, -10, -10, "Insert Gap", NULL);
  h = HiddenGroup (w, -1, 0, NULL);
  
  g = HiddenGroup (h, 2, 0, NULL);
  gap_type_grp = HiddenGroup (g, 0, 2, ChooseGapType);
  RadioButton (gap_type_grp, "Unknown length");
  RadioButton (gap_type_grp, "Known length:");
  k = HiddenGroup (g, 0, 2, NULL);
  StaticPrompt (k, "", 0, popupMenuHeight, programFont, 'l');
  gap_len_txt = DialogText (k, "100", 5, NULL);
  
  SetObjectExtra (gap_type_grp, gap_len_txt, NULL);
  SetValue (gap_type_grp, 1);
  ChooseGapType (gap_type_grp);
  
  c = HiddenGroup (h, 2, 0, NULL);
  b = PushButton (c, "Accept", ModalAcceptButton);
  SetObjectExtra (b, &acd, NULL);
  b = PushButton (c, "Cancel", ModalCancelButton);
  SetObjectExtra (b, &acd, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
  Show (w);
  Select (w);
  
  done = FALSE;
  while (!done)
  {
    acd.accepted = FALSE;
    acd.cancelled = FALSE;
    while (!acd.accepted && ! acd.cancelled)
    {
      ProcessExternalEvent ();
      Update ();
    }
    ProcessAnEvent ();
    if (acd.cancelled)
    {
      done = TRUE;
    }
    else
    {
      gap_type = GetValue (gap_type_grp);
      if (gap_type == 2)
      {
        is_unknown_gap = FALSE;
      }
      else
      {
        is_unknown_gap = TRUE;
      }
      gap_len = 100;
      if (! is_unknown_gap)
      {
        gap_len = 0;
        gap_len_str = SaveStringFromText (gap_len_txt);
        if (!StringHasNoText (gap_len_str))
        {
          gap_len = atoi (gap_len_str);
        }
        gap_len_str = MemFree (gap_len_str);
      }
      if (gap_len < 1)
      {
        Message (MSG_ERROR, "Gap length must be greater than zero!");
      }
      else
      {
        RemapSeqEdIntervalForGap (sefp);
        SeqEdDelete (sefp, FALSE);
        AddJournalEntryEx (eSeqEdInsertGap, sefp->edit_pos_start,
                                            gap_len, NULL,
                                            sefp, is_unknown_gap);
        SeqEdReindexGaps (sefp, sefp->bfp->bvd.bsp);
        changed = TRUE;

        done = TRUE;
      }
    }
  }
  Remove (w);
  if (changed)
  {
    ResizeSeqEdView (sefp);
    Select (sefp->bfp->bvd.seqView);
    inval_panel (sefp->bfp->bvd.seqView, -1, -1);
  }
}

static void SeqEdCopy (SeqEdFormPtr sefp)
{
  SeqLocPtr    slp;
  SeqPortPtr   spp;
  Int4         len;
  CharPtr      str;
  
  if (sefp == NULL || sefp->edit_pos_start == sefp->edit_pos_end) return;
  
  len = sefp->edit_pos_end - sefp->edit_pos_start + 1;
  str = (CharPtr) MemNew ((len + 1) * sizeof (Char));
  if (str == NULL)
    return;
  
  slp = SeqLocIntNew (sefp->edit_pos_start, sefp->edit_pos_end - 1,
  	                      Seq_strand_plus, sefp->bfp->bvd.bsp->id);
  spp = SeqPortNewByLoc (slp, Seq_code_iupacna);
  SeqPortRead  (spp, (Uint1Ptr)str, len);
  SeqPortFree  (spp);
  SeqLocFree (slp);
  StringToClipboard (str);
}

static void SeqEdCopyMenuItem (IteM i)
{
  SeqEdFormPtr sefp;

  if (i == NULL) return;
  sefp = (SeqEdFormPtr) GetObjectExtra (i);

  SeqEdCopy (sefp);	
}

static void SeqEdOnKey (SlatE s, Char ch)
{
  PaneL            pnl;
  WindoW           w;
  SeqEdFormPtr     sefp;
  Int4             new_pos;
  BaR              sb;
  Int4             scroll_pos;
  CharPtr          str;
  Int4             del_start, del_stop;

  if ( (int) ch == 0 ) return;

  pnl = (PaneL) s;
  Select (pnl);
  w = ParentWindow(pnl);
  if (w == NULL) return;
  sefp = (SeqEdFormPtr) GetObjectExtra (w);
  if (sefp == NULL) return;
  CaptureSlateFocus ((SlatE) sefp->bfp->bvd.seqView);
  
  /* later, handle control key combos */
#ifdef WIN_MSWIN
  if (ch == 3)
  {
    SeqEdCopy (sefp);
  }
  else if (ch == 24)
  {
    SeqEdCut (sefp);
  }
  else if (ch == 22)
  {
    SeqEdPaste (sefp);
  }
#else
  if (ctrlKey)
  {
    if (ch == 'c')
    {
      SeqEdCopy (sefp);	
    }
    else if (ch == 'x')
    {
      SeqEdCut (sefp);
    }
    else if (ch == 'v')
    {
      SeqEdPaste (sefp);
    }
  	return;
  }
#endif
  else
  {
    /* look at key pressed - if it's a good sequence character, insert it */
    if ( (str = char_to_insert (&ch, 1, sefp->bfp->bvd.bsp->mol)) != NULL) 
    {
      if (sefp->edit_pos_start != sefp->edit_pos_end)
      {
        SeqEdDelete (sefp, FALSE);
      }
      if (AddJournalEntry (eSeqEdInsert, sefp->edit_pos_start,
   	                   StringLen (str), str, sefp))
      {
   	    sefp->edit_pos_start += StringLen (str);
   	    sefp->edit_pos_end += StringLen (str);
        ResizeSeqEdView (sefp);
        Select (sefp->bfp->bvd.seqView);
  	    inval_panel (pnl, -1, -1);
      }
    }
    else
    {
      /* handle movements and deletions */
      new_pos = sefp->edit_pos_start;
      switch (ch)
      {
        case NLM_LEFT: 
          new_pos--;
          break;
        case NLM_RIGHT:
    	    new_pos++;
    	    break;
    	  case NLM_UP:
    	    new_pos -= sefp->bfp->bvd.CharsAtLine;
    	    break;
    	  case NLM_DOWN:
    	    new_pos += sefp->bfp->bvd.CharsAtLine;
    	    if (new_pos > sefp->bfp->bvd.bsp->length
    	        && new_pos - sefp->bfp->bvd.bsp->length < sefp->bfp->bvd.CharsAtLine)
    	    {
            new_pos = sefp->bfp->bvd.bsp->length;
          }
    	    break;
        case NLM_DEL:
          /* handle deletion */
          if (sefp->edit_pos_end == sefp->edit_pos_start)
          {
            sefp->edit_pos_end = sefp->edit_pos_start + 1;
            if (sefp->edit_pos_end > sefp->bfp->bvd.bsp->length)
            {
              sefp->edit_pos_end = sefp->edit_pos_start;
              return;
            }
          }
          RemapSeqEdIntervalForGap (sefp);
          SeqEdDelete (sefp, FALSE);
          ResizeSeqEdView (sefp);
          Select (sefp->bfp->bvd.seqView);
	        inval_panel (pnl, -1, -1);
          break;
        case '\b':
    	    /* handle backspace */
    	    if (sefp->edit_pos_start == sefp->edit_pos_end)
    	    {
    	      if (sefp->edit_pos_start == 0)
    	      {
    	        del_start = -1;
            }
    	      else
    	      {
              del_start = sefp->edit_pos_start - 1;
              del_stop = sefp->edit_pos_start;
  	        }
          }
    	    else
    	    {
            del_start = sefp->edit_pos_start;
            del_stop = sefp->edit_pos_end;
  	      }
  	  	  
          if (del_start >= 0)
          {
            sefp->edit_pos_start = del_start;
            sefp->edit_pos_end = del_stop;
            RemapSeqEdIntervalForGap (sefp);
            new_pos = sefp->edit_pos_start;
            SeqEdDelete (sefp, FALSE);
            ResizeSeqEdView (sefp);
            Select (sefp->bfp->bvd.seqView);
            inval_panel (pnl, -1, -1);
  	      }
  	      break;
      }
      if (new_pos < 0 || new_pos > sefp->bfp->bvd.bsp->length)
      {
        Beep ();
      }
      else
      {
        sefp->edit_pos_start = new_pos;
  	    sefp->edit_pos_end = new_pos;
        sb = GetSlateVScrollBar (s);
        if (sb != NULL)
  	    {
  	      scroll_pos = SeqEdGetScrollPosForSequencePos (sefp->edit_pos_start, &(sefp->bfp->bvd));
          SetBarValue (sb, scroll_pos);
  	    }
        Select (sefp->bfp->bvd.seqView);
  	    inval_panel (pnl, -1, -1);
      }
    }
    SeqEdUpdateStatus (sefp);
  }
}

static void SeqEdFormMessage (ForM f, Int2 mssg)
{
  SeqEdFormPtr  sefp;

  sefp = (SeqEdFormPtr) GetObjectExtra (f);
  if (sefp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        Beep ();
        break;
      default :
        if (sefp->appmessage != NULL) {
          sefp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void CleanupSeqEdForm (GraphiC g, VoidPtr data)

{
  SeqEdFormPtr  sefp;

  sefp = (SeqEdFormPtr) data;
  if (sefp != NULL)
  {
    ObjMgrFreeUserData (sefp->input_entityID, sefp->procid, sefp->proctype, sefp->userkey);
    FreeSeqEdFormGapFeatList (sefp);
  }

  StdCleanupFormProc (g, data);
}

static void onCloseSeqEdPanel (PaneL p)
{
  BioseqViewPtr bvp;
  bvp  = GetBioseqViewPtr (p);

  bvp->chapter_list = FreeChapterList (bvp->chapter_list);
  bvp->feature_lists = FreeFeatureLists (bvp->feature_lists);

  ValNodeFree(bvp->Selection);
}

static void SetUndoRedoStatus (SeqEdFormPtr sefp)
{
  SeqEdJournalPtr sejp;
  
  if (sefp == NULL || sefp->last_journal_entry == NULL) return;
  
  sejp = sefp->undo_list->next;
  if (sejp->action == eSeqEdJournalEnd)
  {
  	Disable (sefp->undo_menu);
  	Disable (sefp->redo_menu);
  	return;
  }
  
  /* Set redo */
  if (sefp->last_journal_entry->action == eSeqEdJournalEnd)
  {
  	Disable (sefp->redo_menu);
  }
  else
  {
    sejp = sefp->last_journal_entry->next;
    if (sefp->last_journal_entry->action == eSeqEdJournalEnd)
    {
      Disable (sefp->redo_menu);
    }
    else
    {
  	  Enable (sefp->redo_menu);    	
    }
  }
  
  /* Set undo */
  if (sefp->last_journal_entry->action == eSeqEdJournalStart)
  {
  	Disable (sefp->undo_menu);
  }
  else if (sefp->last_journal_entry->action == eSeqEdJournalEnd)
  {
  	sejp = sefp->last_journal_entry->prev;
  	if (sejp->action == eSeqEdJournalStart)
  	{
  	  Disable (sefp->undo_menu);
  	}
  	else
    {
      Enable (sefp->undo_menu);
    }
  }
  else
  {
  	Enable (sefp->undo_menu);
  }
}

static void SeqEdUndoMenuItem (IteM i)
{
  SeqEdFormPtr sefp;
  
  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL) return;

  if (UnplayJournal (&sefp->last_journal_entry, 1))
  {
    RemapSeqEdIntervalForGap (sefp);
    SetUndoRedoStatus (sefp);
    ResizeSeqEdView (sefp);
    Select (sefp->bfp->bvd.seqView);
    inval_panel (sefp->bfp->bvd.seqView, -1, -1);
  }
}

static void SeqEdRedoMenuItem (IteM i)
{
  SeqEdFormPtr sefp;
  
  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL) return;

  if (PlayJournal (sefp->last_journal_entry->next, &sefp->last_journal_entry, 1, NULL))
  {
    RemapSeqEdIntervalForGap (sefp);
    SetUndoRedoStatus (sefp);
    ResizeSeqEdView (sefp);
    Select (sefp->bfp->bvd.seqView);
    inval_panel (sefp->bfp->bvd.seqView, -1, -1);
  }
}

typedef struct seqedexportform
{
  ForM         form;
  SeqEdFormPtr sefp;
  Boolean      isFasta;
  TexT         start;
  TexT         stop;
} SeqEdExportForm, PNTR SeqEdExportFormPtr;

static void SeqEdExportProc (ButtoN b)
{
  SeqEdExportFormPtr sxfp;
  Char               namep [PATH_MAX];
  Char               val [100];
  Int4               start, stop;
  FILE * fout;
  
  if (b == NULL) return;
  sxfp = (SeqEdExportFormPtr) GetObjectExtra (b);
  if (sxfp == NULL || sxfp->sefp == NULL) return;
  GetTitle (sxfp->start, val, sizeof (val) - 1);
  start = atoi (val);
  GetTitle (sxfp->stop, val, sizeof (val) - 1);
  stop = atoi (val);
  
  if (start >= stop) return;
  if (stop > sxfp->sefp->bfp->bvd.bsp->length)
    stop = sxfp->sefp->bfp->bvd.bsp->length;
  if (!GetOutputFileName (namep, PATH_MAX, NULL))
     return;
  fout = FileOpen (namep, "w");
  if (fout != NULL) {
    if (sxfp->isFasta)
    {
      EditBioseqToFasta (sxfp->sefp->bfp->bvd.bsp, fout, start - 1, stop - 1);	
    }
    else
    {
      ExportSequenceText (sxfp->sefp, start - 1, stop - 1, fout);	
    }
    FileClose (fout);
    Hide (sxfp->form);
    Remove (sxfp->form);
  }
}

static void SeqEdExportMenu (IteM i, Boolean isFasta)
{
  WindoW             w;
  SeqEdFormPtr       sefp;
  SeqEdExportFormPtr sxfp;
  GrouP              h, c;
  Char               str[255];
  ButtoN             b;
  
  if (i == NULL) return;
  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL) return;
  sxfp = (SeqEdExportFormPtr) MemNew (sizeof (SeqEdExportForm));
  if (sxfp == NULL) return;
  if (isFasta)
  {
    w = FixedWindow (-50, -33, -10, -10, "Export FASTA", StdCloseWindowProc);
  }
  else
  {
    w = FixedWindow (-50, -33, -10, -10, "Export Text", StdCloseWindowProc);
  }  

  SetObjectExtra (w, sxfp, NULL);
  sxfp->form = (ForM) w;
  sxfp->sefp = sefp;
  sxfp->isFasta = isFasta;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 2, 0);
  StaticPrompt (h, "From", 0, dialogTextHeight,  systemFont,  '1');
  if (sefp->edit_pos_start != sefp->edit_pos_end)
  {
    sprintf (str, "%d", sefp->edit_pos_start + 1);
  }
  else
  {
  	sprintf (str, "%d", 1);
  }
  sxfp->start = DialogText (h, str, 5, NULL);
  StaticPrompt (h, "To", 0, dialogTextHeight,  systemFont,  '1');
  if (sefp->edit_pos_end != sefp->edit_pos_start)
  {
    sprintf (str, "%d", sefp->edit_pos_end);  	
  }
  else
  {
  	sprintf (str, "%d", sefp->bfp->bvd.bsp->length);
  }
  sxfp->stop = DialogText (h, str, 5, NULL);
  
  c = HiddenGroup (h, 4, 0, NULL);
  b = PushButton (c, "Accept", SeqEdExportProc);
  SetObjectExtra (b, sxfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  RealizeWindow (w);
  Show (w);
}

static void SeqEdExportFastaMenuItem (IteM i)
{
  SeqEdExportMenu (i, TRUE);
}

static void SeqEdExportTextMenuItem (IteM i)
{
  SeqEdExportMenu (i, FALSE);
}

static void ShowNextSeqEdPattern (SeqEdFormPtr sefp)
{
  ValNodePtr vnp;
  SeqLocPtr  slp = NULL;
  Int4       pattern_start;
  Int4       pattern_len;
  
  if (sefp == NULL) return;
  
  /* Look at current edit position, find next position in list, scroll to it */	
  vnp = sefp->match_list;
  pattern_len = StringLen (sefp->current_pattern);
  if (sefp->current_pattern_translate)
  {
    pattern_len *= 3;
  }
  while (vnp != NULL)
  {
    slp = (SeqLocPtr) vnp->data.ptrvalue;
    pattern_start = GetOffsetInBioseq (slp, sefp->bfp->bvd.bsp, SEQLOC_LEFT_END);
    if ((pattern_start == sefp->edit_pos_start 
        && sefp->edit_pos_end - sefp->edit_pos_start < pattern_len - 1)
        || pattern_start > sefp->edit_pos_start)
    {
      sefp->edit_pos_start = pattern_start;
      sefp->edit_pos_end = pattern_start + pattern_len;
      SeqEdScrollToSeqPosition (sefp, sefp->edit_pos_start);
      SeqEdUpdateStatus (sefp);
      return;
    }
    vnp = vnp->next;
  }
  if (sefp->match_list != NULL)
  {
  	slp = (SeqLocPtr) sefp->match_list->data.ptrvalue;
    sefp->edit_pos_start = GetOffsetInBioseq (slp, sefp->bfp->bvd.bsp, SEQLOC_LEFT_END);
    sefp->edit_pos_end = sefp->edit_pos_start + pattern_len;
    SeqEdScrollToSeqPosition (sefp, sefp->edit_pos_start);
    SeqEdUpdateStatus (sefp);
  }
}

static ValNodePtr MatchListFree (ValNodePtr list)
{
  SeqLocPtr slp;
  
  if (list == NULL) return NULL;
  MatchListFree (list->next);
  list->next = NULL;
  slp = (SeqLocPtr) list->data.ptrvalue;
  SeqLocFree (slp);
  ValNodeFree (list);
  return NULL;
}

static CharPtr GetPatternFromDialogText (TexT txt)
{
  Char  str [128];
  Int4  len;
  CharPtr strp, sp, dp;
  
  GetTitle (txt, str, sizeof (str) - 1);
  len = StringLen (str) + 1;
  if (len < 2) return NULL;	
  strp = (CharPtr) MemNew (sizeof (Char) * len);
  sp = str;
  dp = strp;
  while (*sp != 0)
  {
    if (*sp == '*')
    {
      *dp = *sp;
      dp++;
    }
    else if (isalpha ((Int4)(*sp)))
    {
      *dp = TO_UPPER (*sp);
      dp++;
    }
    sp++;
  }
  *dp = 0;
  if (*strp == 0)
  {
  	MemFree (strp);
  	strp = NULL;
  }
  return strp;
}

typedef struct finddlgdata
{
  FORM_MESSAGE_BLOCK
  WindoW      w; 
  TexT        pattern_txt;
  GrouP       search_choice_grp;
  ButtoN      reverse_complement_btn;
  PopuP       frame_choice_popup;
  PrompT      prompt;

} FindDlgData, PNTR FindDlgPtr;

static void LIBCALLBACK MatchProc (Int4 position, CharPtr name, CharPtr pattern,
                       Int2 cutSite, Uint1 strand, Pointer userdata)

{
  ValNodePtr PNTR loc_list;
  
  loc_list = (ValNodePtr PNTR) userdata;
  if (loc_list == NULL) return;
  
  ValNodeAddInt (loc_list, 0, position);
}

static void ConvertIntListToLocList (ValNodePtr loc_list, BioseqPtr bsp, Int4 pattern_len)
{
  ValNodePtr vnp;
  Int4       start;
  Int4       stop;
  SeqLocPtr  slp;
  
  if (loc_list == NULL || bsp == NULL || pattern_len == 0)
  {
    return;
  }
  
  for (vnp = loc_list; vnp != NULL; vnp = vnp->next)
  {
    start = vnp->data.intvalue;
    stop = start + pattern_len - 1;
    slp = SeqLocIntNew(start, stop, Seq_strand_plus, SeqIdDup(bsp->id));
    vnp->data.ptrvalue = slp;
  }
}


static ValNodePtr FindSeqMatch (BioseqPtr bsp, CharPtr pattern)

{
  SeqSearchPtr  tbl;
  ValNodePtr    pattern_loc_list = NULL;
  SeqPortPtr    spp;
  Uchar         buf[2];
  Int2          ctr;

  if (bsp == NULL || StringHasNoText (pattern))
  {
    return NULL;
  }
  
  tbl = SeqSearchNew (MatchProc, &pattern_loc_list);
  if (tbl == NULL) return NULL;

  SeqSearchAddNucleotidePattern (tbl, "Find", pattern, 1, SEQ_SEARCH_JUST_TOP_STRAND); 

  spp = SeqPortNew (bsp, 0, bsp->length-1, Seq_strand_plus, Seq_code_iupacna);
  SeqPortSeek (spp, 0, SEEK_SET);
  ctr = SeqPortRead (spp, buf, 1);
  while (ctr > 0)
  {
    SeqSearchProcessCharacter (tbl, buf[0]);
    ctr = SeqPortRead (spp, buf, 1);
  }

  SeqSearchFree (tbl);
  ConvertIntListToLocList (pattern_loc_list, bsp, StringLen (pattern));
  return pattern_loc_list;
}

static ValNodePtr MergeIntLists (ValNodePtr list1, ValNodePtr list2)
{
  ValNodePtr vnp1, vnp2, new_list = NULL;
  
  vnp1 = list1;
  vnp2 = list2;
  
  while (vnp1 != NULL || vnp2 != NULL)
  {
    if (vnp1 == NULL)
    {
      ValNodeAddInt (&new_list, 0, vnp2->data.intvalue);
      vnp2 = vnp2->next;
    }
    else if (vnp2 == NULL)
    {
      ValNodeAddInt (&new_list, 0, vnp1->data.intvalue);
      vnp1 = vnp1->next;
    }
    else if (vnp1->data.intvalue < vnp2->data.intvalue)
    {
      ValNodeAddInt (&new_list, 0, vnp1->data.intvalue);
      vnp1 = vnp1->next;
    }
    else if (vnp2->data.intvalue < vnp1->data.intvalue)
    {
      ValNodeAddInt (&new_list, 0, vnp2->data.intvalue);
      vnp2 = vnp2->next;
    }
    else
    {
      /* values are equal, take only one */
      ValNodeAddInt (&new_list, 0, vnp1->data.intvalue);
      vnp1 = vnp1->next;
      vnp2 = vnp2->next;
    }
  }
  return new_list;
}

static ValNodePtr FindTranslationMatchOneFrame 
(BioseqPtr       bsp,
 CharPtr         pattern,
 Int4            frame)
{
  SeqFeatPtr    fake_cds;
  Uint1         strand;
  Int4          start;
  Int4          stop;
  ByteStorePtr  bs;
  CharPtr       frame_str;
  CharPtr       cp;
  ValNodePtr    pattern_loc_list = NULL, vnp;
  Int4          prot_pos;
  TextFsaPtr    tfp;
  Int2          state;
  ValNodePtr    matches;
  Uchar         ch;
  Int4          pattern_len;
  CharPtr       scratch_pattern;
  
  if (bsp == NULL || StringHasNoText (pattern))
  {
    return NULL;
  }
  
  switch (frame)
  {
    case 1:
      start = 0;
      stop = bsp->length - 1;
      strand = Seq_strand_plus;
      break;
    case 2:
      start = 1;
      stop = bsp->length - 1;
      strand = Seq_strand_plus;
      break;
    case 3:
      start = 2;
      stop = bsp->length - 1;
      strand = Seq_strand_plus;
      break;
    case 4:
      start = 0;
      stop = bsp->length - 1;
      strand = Seq_strand_minus;
      break;
    case 5:
      start = 0;
      stop = bsp->length - 2;
      strand = Seq_strand_minus;
      break;
    case 6:
      start = 0;
      stop = bsp->length - 3;
      strand = Seq_strand_minus;
      break;
    default:
      return NULL;
      break;
  } 
  
  fake_cds = make_fake_cds(bsp, start, stop, strand);
  
  bs = ProteinFromCdRegionEx(fake_cds, TRUE, FALSE);
  SeqFeatFree(fake_cds);
  if(bs == NULL) return NULL;
  frame_str = BSMerge (bs, NULL);
  bs = BSFree (bs);
  if (frame_str == NULL) return NULL;
  scratch_pattern = StringSave (pattern);
  if (strand == Seq_strand_minus)
  {
    reverse_string (scratch_pattern);
  }
  
  tfp = TextFsaNew ();
  TextFsaAdd (tfp, scratch_pattern);
  
  state = 0;
  cp = frame_str;
  ch = *cp;
  if (strand == Seq_strand_minus)
  {
    prot_pos = stop + 1;
  }
  else
  {
    prot_pos = start;
  }
  pattern_len = StringLen (scratch_pattern);
  while (ch != '\0') {
    matches = NULL;
    state = TextFsaNext (tfp, state, ch, &matches);
    if (matches != NULL) 
    {
      if (strand == Seq_strand_minus)
      {
        /* need to add locations in reverse order */
        vnp = ValNodeNew(NULL);
        if (vnp != NULL)
        {
          vnp->next = pattern_loc_list;
          vnp->data.intvalue = prot_pos - 3;
          pattern_loc_list = vnp;
        }
      }
      else
      {
        ValNodeAddInt (&pattern_loc_list, 0, prot_pos + 3 - (3 * pattern_len));    
      }
    }
    cp++;
    ch = *cp;
    if (strand == Seq_strand_minus)
    {
      prot_pos -= 3;
    }
    else
    {
      prot_pos += 3;
    }
  }

  MemFree (frame_str);
  MemFree (scratch_pattern);
  
  return pattern_loc_list;
}

static ValNodePtr FindTranslation (BioseqPtr bsp, CharPtr pattern, Int4 pick_frame)
{
  Int4       i;
  ValNodePtr loc_list = NULL;
  ValNodePtr new_list, comb_list;
  if (bsp == NULL || StringHasNoText (pattern))
  {
    return NULL;
  }
  
  if (pick_frame < 1 || pick_frame > 6)
  {
    for (i = 1; i <= 6; i++)
    {
      new_list = FindTranslationMatchOneFrame (bsp, pattern, i);
      comb_list = MergeIntLists (loc_list, new_list);
      ValNodeFree (new_list);
      ValNodeFree (loc_list);
      loc_list = comb_list;
    }
  }
  else
  {
    loc_list = FindTranslationMatchOneFrame (bsp, pattern, pick_frame);
  }
  ConvertIntListToLocList (loc_list, bsp, StringLen (pattern));
  return loc_list;  
}

static void CleanPattern (CharPtr pattern, Int4 search_choice)
{
  CharPtr cp;
  
  if (pattern == NULL) return;
  for (cp = pattern; *cp != 0; cp++)
  {
    if (search_choice == 3)
    {
      *cp = toupper (*cp);
    }
    else
    {
      *cp = tolower (*cp);
    }
  }
}

static void FindPatternButton (ButtoN b)
{
  SeqEdFormPtr       sefp;
  FindDlgPtr         fdp;
  WindoW             wdialog;
  CharPtr            strp;
  Int4               count;
  Char               str[255];
  Uint1              pick_frame;
  Int4               search_choice = 0;
  
  if (b == NULL) return;
  sefp = (SeqEdFormPtr) GetObjectExtra (b);
  if (sefp == NULL) return;
  wdialog = ParentWindow (b);
  fdp = (FindDlgPtr) GetObjectExtra (wdialog);
  strp = GetPatternFromDialogText (fdp->pattern_txt);

  if (fdp->search_choice_grp != NULL)
  {
    search_choice = GetValue (fdp->search_choice_grp);
  }
  
  CleanPattern (strp, search_choice);
  if (GetStatus (fdp->reverse_complement_btn) && search_choice != 3)
  {
    complement_string (strp);
    reverse_string (strp);
  }
  
  if (strp == NULL) {
     SetTitle (fdp->prompt, "No item found");
     return;
  }
  
  if (sefp->current_pattern != NULL) {
    if ((StringStr(sefp->current_pattern, strp)) 
         && (StringLen(sefp->current_pattern) == StringLen (strp))
         && (fdp->search_choice_grp == NULL
             || (search_choice == 1 && !sefp->current_pattern_translate
              && sefp->current_pattern_revcomp == GetStatus (fdp->reverse_complement_btn))
             || (search_choice == 3 && sefp->current_pattern_translate
              && sefp->current_pattern_translate_frame_choice == GetValue (fdp->frame_choice_popup))))
    {
      MemFree (strp);
      ShowNextSeqEdPattern (sefp);
      return;
    }
    else 
    {
      MemFree (sefp->current_pattern);
    }
  }
  sefp->current_pattern = strp;
  if (sefp->match_list != NULL)
     sefp->match_list = MatchListFree (sefp->match_list);
  if (search_choice == 1)
  {
    sefp->current_pattern_revcomp = (Boolean) GetStatus (fdp->reverse_complement_btn);
    sefp->current_pattern_translate = FALSE;
  }
  else if (search_choice == 3)
  {
    sefp->current_pattern_translate = TRUE;
    sefp->current_pattern_translate_frame_choice = GetValue (fdp->frame_choice_popup);
  }
  
  if (sefp->current_pattern_translate) 
  {
    pick_frame = sefp->current_pattern_translate_frame_choice;
    if (pick_frame > 0)
    {
      pick_frame --;
    }
    sefp->match_list = FindTranslation (sefp->bfp->bvd.bsp, strp, pick_frame);
  }
  else 
  {
    sefp->match_list = FindSeqMatch (sefp->bfp->bvd.bsp, strp);
  }
  if (sefp->match_list != NULL) {
    count = ValNodeLen (sefp->match_list);
    if (count ==0)
      SetTitle (fdp->prompt, "No item found");
    else if (count ==1) {
      SetTitle (fdp->prompt, "1 item found");
    } else {
      sprintf (str, "%d items found   -   next=CTRL->, previous=CTRL<- ", (int) count);
      SetTitle (fdp->prompt, str);
    }
    ShowNextSeqEdPattern (sefp);
  }
  else {
     SetTitle (fdp->prompt, "No item found");
  }
  Update ();
  return;  
  	
}

static void ChangeSearchChoice (GrouP g)
{
  WindoW     wdialog;
  FindDlgPtr fdp;
  Int4       val;
  
  wdialog = ParentWindow (g);
  fdp = (FindDlgPtr) GetObjectExtra (wdialog);
  if (fdp == NULL) return;
  val = GetValue (g);
  if (val == 1)
  {
    Enable (fdp->reverse_complement_btn);
    Disable (fdp->frame_choice_popup);
  }
  else
  {
    Disable (fdp->reverse_complement_btn);
    Enable (fdp->frame_choice_popup);
  }
}

static void SeqEdFindPatternDialog (IteM i)
{
  WindoW             w, wdialog;
  FindDlgPtr         fdp;
  SeqEdFormPtr       sefp;
  GrouP              h, g1, g2;
  ButtoN             b;
  PrompT             p1;

  if (i == NULL) return;
  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL) return;
  
  w = ParentWindow (i);
  
  wdialog=FixedWindow (-50, -33, -10, -10, "Find", StdCloseWindowProc);
  fdp = (FindDlgPtr) MemNew (sizeof (FindDlgData));
  SetObjectExtra (wdialog, (Pointer) fdp, StdCleanupExtraProc);
  fdp->w = w;
  fdp->search_choice_grp = NULL;
  
  h = HiddenGroup (wdialog, -1, 0, NULL);

  p1 = StaticPrompt (h, "Find pattern:", 0, popupMenuHeight, programFont, 'l');

  fdp->pattern_txt = ScrollText (h, 25, 8, programFont, TRUE, NULL);
  fdp->prompt = StaticPrompt (h, "", (Int2)(25*stdCharWidth), stdLineHeight, programFont, 'l'); 
  if (ISA_na(sefp->bfp->bvd.bsp->mol))
  {
    fdp->search_choice_grp = NormalGroup (h, 2, 0, "Search in", systemFont, ChangeSearchChoice);
    RadioButton (fdp->search_choice_grp, "Nucleotide sequence");
    fdp->reverse_complement_btn = CheckBox (fdp->search_choice_grp, "reverse complement", NULL);
    SetStatus (fdp->reverse_complement_btn, FALSE);
    RadioButton (fdp->search_choice_grp, "Translated frame");
    fdp->frame_choice_popup = PopupList (fdp->search_choice_grp, TRUE, NULL);
    PopupItem (fdp->frame_choice_popup, "Any");
    PopupItem (fdp->frame_choice_popup, "+1");
    PopupItem (fdp->frame_choice_popup, "+2");
    PopupItem (fdp->frame_choice_popup, "+3");
    PopupItem (fdp->frame_choice_popup, "-1");
    PopupItem (fdp->frame_choice_popup, "-2");
    PopupItem (fdp->frame_choice_popup, "-3");
    SetValue (fdp->frame_choice_popup, 1);
    Disable (fdp->frame_choice_popup);
    SetValue (fdp->search_choice_grp, 1);
    
    g1 = fdp->search_choice_grp;
    g2 = HiddenGroup (h, 2, 0, NULL);
    b = PushButton (g2, "Find Next", FindPatternButton);
    SetObjectExtra (b, sefp, NULL); 
    PushButton (g2, "Dismiss", StdCancelButtonProc);
  }
  else {
    g1 = HiddenGroup (h, 2, 0, NULL);
    b = PushButton (g1, "Find Next", FindPatternButton); 
    SetObjectExtra (b, sefp, NULL); 
    PushButton (g1, "Close", StdCancelButtonProc);
  }
  
  AlignObjects (ALIGN_CENTER, (HANDLE) p1, (HANDLE) fdp->pattern_txt, (HANDLE)fdp->prompt,
                (HANDLE) fdp->search_choice_grp, (HANDLE) g2, NULL);
  RealizeWindow (wdialog);
  Show (wdialog);
  return;
}

static void ObjMgrSelectSeqEdSelection (SeqEdFormPtr sefp)
{
  SeqLocPtr slp;
  Uint2     itemID;
  Uint2     entityID;
  
  if (sefp == NULL || sefp->edit_pos_start == sefp->edit_pos_end) return;
  slp = SeqLocIntNew (sefp->edit_pos_start, sefp->edit_pos_end - 1,
  	                      Seq_strand_plus, sefp->bfp->bvd.bsp->id);
  	                      
  entityID = ObjMgrGetEntityIDForPointer (sefp->bfp->bvd.bsp);
  BioseqFindEntity (sefp->bfp->bvd.bsp->id, &itemID);
  	      
  ObjMgrSelect (entityID, itemID, OBJ_BIOSEQ, OM_REGION_SEQLOC, slp);
  
}

typedef struct newfeaturedata {
  ObjMgrProcPtr  ompp;
  IteM           item;
  Uint1          molgroup;
  struct newfeaturedata PNTR next;
} NewFeatureData, PNTR NewFeaturePtr;

static CharPtr  editOldSrcDescMsg = "\
You may really want to edit an existing BioSource descriptor instead.\n\
Proceed anyway?";

static void SeqEditNewFeatureMenuProc (IteM i)

{
  MsgAnswer         ans;
  NewFeaturePtr     nfp;
  OMProcControl     ompc;
  ObjMgrProcPtr     ompp;
  Int2              retval;
  SeqEdFormPtr      sefp;
  Uint2             itemID;

  sefp = (SeqEdFormPtr) GetObjectExtra ((WindoW) ParentWindow (i));
  if (sefp == NULL) return;

  nfp = (NewFeaturePtr) GetObjectExtra (i);
  if (nfp == NULL) return;
  ompp = nfp->ompp;
  if (ompp == NULL || ompp->func == NULL) return;
  if (ompp->subinputtype == FEATDEF_BIOSRC) {
    ans = Message (MSG_YN, editOldSrcDescMsg);
    if (ans == ANS_NO) return;
  }
  MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
  ompc.input_entityID =  ObjMgrGetEntityIDForPointer (sefp->bfp->bvd.bsp);
  BioseqFindEntity (sefp->bfp->bvd.bsp->id, &itemID);
  ompc.input_itemID = itemID;
  ompc.input_itemtype = OBJ_BIOSEQ;
  ObjMgrSelectSeqEdSelection (sefp);
  GatherDataForProc (&ompc, FALSE);
  ompc.proc = ompp;
  retval = (*(ompp->func)) (&ompc);
  if (retval == OM_MSG_RET_ERROR) {
    ErrShow ();
  }
}

static void SeqEdNewFeaturesMenu (MenU m, Boolean is_na)

{
  FeatDispGroupPtr  fdgp;
  FeatDefPtr        fdp;
  NewFeaturePtr     first;
  IteM              i;
  Uint1             key;
  CharPtr           label;
  NewFeaturePtr     last;
  NewFeaturePtr     nfp;
  ObjMgrPtr         omp;
  ObjMgrProcPtr     ompp;
  ObjMgrTypePtr     omtp;
  MenU              sub;
  Uint2             subtype;

  if (m == NULL) return;
  omp = ObjMgrGet ();
  if (omp == NULL) return;
  ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, 0, 0, NULL);
  if (ompp == NULL) return;
  omtp = NULL;
  first = NULL;
  last = NULL;
  while ((omtp = ObjMgrTypeFindNext (omp, omtp)) != NULL) {
    ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, omtp->datatype, 0, NULL);
    if (ompp != NULL) {
      switch (omtp->datatype) {
        case OBJ_SEQFEAT :
          fdgp = NULL;
          while ((fdgp = DispGroupFindNext (fdgp, &key, &label)) != NULL) {
            if (fdgp->groupkey != 0) {
              sub = SubMenu (m, fdgp->groupname);
              fdp = NULL;
              label = NULL;
              while ((fdp = FeatDefFindNext (fdp, &key, &label,
                     fdgp->groupkey, FALSE)) != NULL) {
                if (key != FEATDEF_BAD) {
                  ompp = NULL;
                  while ((ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT,
				          omtp->datatype, 0, ompp)) != NULL) {
                    if (ompp->subinputtype == fdp->featdef_key &&
                        ompp->subinputtype != FEATDEF_PUB) {
                      i = CommandItem (sub, ompp->proclabel, SeqEditNewFeatureMenuProc);
                      nfp = (NewFeaturePtr) MemNew (sizeof (NewFeatureData));
                      if (nfp != NULL) {
                        nfp->ompp = ompp;
                        nfp->item = i;
                        nfp->molgroup = fdp->molgroup;
                      }
                      if (first == NULL) {
                        first = nfp;
                      }
                      if (last != NULL) {
                        last->next = nfp;
                      }
                      last = nfp;
                      SetObjectExtra (i, (Pointer) nfp, StdCleanupExtraProc);
                      if ((is_na && (fdp->molgroup == 2 || fdp->molgroup == 3)) ||
                          ((! is_na) && (fdp->molgroup == 1 || fdp->molgroup == 3))) {
                      } else {
                        Disable (i);
                      }
                    }
                  }
                }
              }
            }
          }
          sub = SubMenu (m, "Remaining Features");
          fdp = NULL;
          label = NULL;
          while ((fdp = FeatDefFindNext (fdp, &key, &label, 0, FALSE)) != NULL) {
            if (key != FEATDEF_BAD) {
              ompp = NULL;
              while ((ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT,
                      omtp->datatype, 0, ompp)) != NULL) {
                subtype = ompp->subinputtype;
                if (subtype == fdp->featdef_key &&
                    subtype != FEATDEF_PUB &&
                    subtype != FEATDEF_Imp_CDS &&
                    subtype != FEATDEF_misc_RNA &&
                    subtype != FEATDEF_precursor_RNA &&
                    subtype != FEATDEF_mat_peptide &&
                    subtype != FEATDEF_sig_peptide &&
                    subtype != FEATDEF_transit_peptide &&
                    subtype != FEATDEF_source &&
                    subtype != FEATDEF_virion &&
                    subtype != FEATDEF_mutation &&
                    subtype != FEATDEF_allele &&
                    subtype != FEATDEF_site_ref &&
                    subtype != FEATDEF_gap) {
                  i = CommandItem (sub, ompp->proclabel, SeqEditNewFeatureMenuProc);
                  nfp = (NewFeaturePtr) MemNew (sizeof (NewFeatureData));
                  if (nfp != NULL) {
                    nfp->ompp = ompp;
                    nfp->item = i;
                    nfp->molgroup = fdp->molgroup;
                  }
                  if (first == NULL) {
                    first = nfp;
                  }
                  if (last != NULL) {
                    last->next = nfp;
                  }
                  last = nfp;
                  SetObjectExtra (i, (Pointer) nfp, StdCleanupExtraProc);
                  if ((is_na && (fdp->molgroup == 2 || fdp->molgroup == 3)) ||
                      ((! is_na) && (fdp->molgroup == 1 || fdp->molgroup == 3))) {
                  } else {
                    Disable (i);
                  }
                }
              }
            }
          }
          break;
        default :
          break;
      }
    }
  }
}


static void CreateSeqEdMenus (WindoW w)

{
  MenU         edit_menu;
  IteM         localItem;
  MenU         sub;
  SeqEdFormPtr sefp;
  Int4         i;
  Boolean      is_na;
  
  if (w == NULL) return;
  sefp = (SeqEdFormPtr) GetObjectExtra (w);
  if (sefp == NULL) return;
  
  /* File Menu */
  edit_menu = PulldownMenu (w, "File");
  sub = SubMenu (edit_menu, "Export");
  localItem = CommandItem (sub, "FASTA", SeqEdExportFastaMenuItem);
  SetObjectExtra (localItem, sefp, NULL);
  localItem = CommandItem (sub, "Text", SeqEdExportTextMenuItem);
  SetObjectExtra (localItem, sefp, NULL);
  
  localItem = CommandItem (edit_menu, "Accept", SeqEdAcceptMenuItem);
  SetObjectExtra (localItem, sefp, NULL);
  localItem = CommandItem (edit_menu, "Cancel", SeqEdCancelMenuItem);
  SetObjectExtra (localItem, sefp, NULL);
 
  
  /* Edit Menu */
  edit_menu = PulldownMenu (w, "Edit");
  sefp->undo_menu = CommandItem (edit_menu, "Undo", SeqEdUndoMenuItem);
  SetObjectExtra (sefp->undo_menu, sefp, NULL);
  Disable (sefp->undo_menu);
  sefp->redo_menu = CommandItem (edit_menu, "Redo", SeqEdRedoMenuItem);
  SetObjectExtra (sefp->redo_menu, sefp, NULL);
  Disable (sefp->redo_menu);
  localItem = CommandItem (edit_menu, "Cut", SeqEdCutMenuItem);
  SetObjectExtra (localItem, sefp, NULL);
  localItem = CommandItem (edit_menu, "Paste", SeqEdPasteMenuItem);
  SetObjectExtra (localItem, sefp, NULL);
  localItem = CommandItem (edit_menu, "Copy", SeqEdCopyMenuItem);
  SetObjectExtra (localItem, sefp, NULL);
  localItem = CommandItem (edit_menu, "Find", SeqEdFindPatternDialog);
  SetObjectExtra (localItem, sefp, NULL);
  localItem = CommandItem (edit_menu, "Translate CDS", SeqEdTranslateCDSItem);
  SetObjectExtra (localItem, sefp, NULL);
  if (sefp->bfp != NULL 
      && sefp->bfp->bvd.bsp != NULL
      && sefp->bfp->bvd.bsp->repr == Seq_repr_delta
      && sefp->bfp->bvd.bsp->seq_ext_type == 4)
  {
    localItem = CommandItem (edit_menu, "Insert Gap", SeqEdInsertGapMenuItem);
    SetObjectExtra (localItem, sefp, NULL);
  }
  
 
  /* View menu */
  edit_menu = PulldownMenu (w, "View");
  localItem = StatusItem (edit_menu, "Complement", SeqEdComplement);
  SetStatus ( localItem, FALSE); 
  SetObjectExtra (localItem, sefp, NULL);
 
  sub = SubMenu (edit_menu, "Reading frames");
  sefp->frames[0] = StatusItem (sub, " +1", SeqEdFrame1);
  sefp->frames[1] = StatusItem (sub, " +2", SeqEdFrame2);
  sefp->frames[2] = StatusItem (sub, " +3", SeqEdFrame3);
  sefp->frames[3] = StatusItem (sub, "All +", SeqEdFrameAllPlus);
  sefp->frames[4] = StatusItem (sub, " -1", SeqEdFrame4);
  sefp->frames[5] = StatusItem (sub, " -2", SeqEdFrame5);
  sefp->frames[6] = StatusItem (sub, " -3", SeqEdFrame6);
  sefp->frames[7] = StatusItem (sub, "All -", SeqEdFrameAllMinus);
  sefp->frames[8] = StatusItem (sub, "All", SeqEdFrameAll);
  sefp->frames[9] = StatusItem (sub, "None", SeqEdFrameNone);
  
  for (i = 0; i < 10; i++)
  {
    SetObjectExtra (sefp->frames[i], sefp, NULL);
  	SetStatus (sefp->frames[i], FALSE);
  }
  SetStatus (sefp->frames[9], TRUE);
  localItem = StatusItem (edit_menu, "Protein Mismatches", SeqEdToggleProteinMismatch);
  SetObjectExtra (localItem, sefp, NULL);
  SetStatus (localItem, FALSE);
  localItem = StatusItem (edit_menu, "On-the-fly Protein Translations", SeqEdToggleProteinOnTheFly);
  SetObjectExtra (localItem, sefp, NULL);
  SetStatus (localItem, sefp->bfp->bvd.on_the_fly);
   
  edit_menu = PulldownMenu (w, "Features");    
  is_na = ISA_na (sefp->bfp->bvd.bsp->mol);
  SeqEdNewFeaturesMenu (edit_menu, is_na);
}

static Uint1 GetMoleculeType (BioseqPtr bsp)
{
  SeqDescPtr         sdp;
  MolInfoPtr         mip;
  SeqMgrDescContext  dcontext;

  if (bsp == NULL) return MOLECULE_TYPE_GENOMIC;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
  if (sdp == NULL) return MOLECULE_TYPE_GENOMIC;
  mip = (MolInfoPtr) sdp->data.ptrvalue;
  if (mip == NULL) return MOLECULE_TYPE_GENOMIC;
  return mip->biomol;
}

/*************************************************************
**************************************************************/
static Int2 LIBCALLBACK SeqEditMsgFunc (OMMsgStructPtr ommsp)
{
  WindoW             currentport,
                     temport;
  OMUserDataPtr      omudp;
  SeqEdFormPtr       sefp = NULL;
  
  omudp = (OMUserDataPtr)(ommsp->omuserdata);
  if (omudp == NULL) return OM_MSG_RET_ERROR;
  sefp = (SeqEdFormPtr) omudp->userdata.ptrvalue;
  if (sefp == NULL) return OM_MSG_RET_ERROR;

  currentport = ParentWindow (sefp->bfp->bvd.seqView);
  temport = SavePort (currentport);
  UseWindow (currentport);
  Select (sefp->bfp->bvd.seqView);
  switch (ommsp->message) 
  {
      case OM_MSG_UPDATE:
          ResizeSeqEdView (sefp);
          Select (sefp->bfp->bvd.seqView);
          inval_panel (sefp->bfp->bvd.seqView, -1, -1);	
          break;
      case OM_MSG_DESELECT:
          break;

      case OM_MSG_SELECT: 
          break;
      case OM_MSG_DEL:
          Hide (sefp->form);
          Remove (sefp->form);
          break;
      case OM_MSG_HIDE:
          break;
      case OM_MSG_SHOW:
          break;
      case OM_MSG_FLUSH:
          SeqEdCancel (sefp);	
          break;
      default:
          break;
  }
  RestorePort (temport);
  UseWindow (temport);
  return OM_MSG_RET_OK;
}


static void OnDrawSeqEdPanel (PaneL p)
{
  BioseqViewPtr bvp;
  
  bvp = GetBioseqViewPtr (p);
  if (bvp == NULL) return;
  DrawSeqChapters (bvp);
}


extern ForM CreateSeqEditorWindow (Int2 left, Int2 top, CharPtr windowname, BioseqPtr bsp)
{
  SeqEdFormPtr       sefp;
  WindoW             w;
  GrouP              g;
  PopuP              pop;
  Char               str [16];
  Int2               window_width = 650;
  Int2               window_hgt = 300,
                     charwidth,
                     lineheight;
  FonT               font;
  Boolean            is_prot = FALSE;
  Int4               btns_across;
  OMUserDataPtr      omudp;
  Uint1              seqtype;
  ButtoN             b;
  
  if (bsp==NULL || (bsp->repr != Seq_repr_raw && bsp->repr != Seq_repr_delta))
  {
    return NULL;
  }
  is_prot = (Boolean)(ISA_aa(bsp->mol));
  sefp = SeqEdFormNew ();
  if (sefp == NULL) 
  {
    return NULL;
  }

  /* prepare BSP for editing */
  if (ISA_na(bsp->mol))
  {
    seqtype = Seq_code_iupacna;
  }
  else
  {
    seqtype = Seq_code_ncbieaa;
  }
  if (bsp->seq_data_type != seqtype && bsp->repr != Seq_repr_delta)
  {
    BioseqRawConvert(bsp, seqtype);
  }
  
#ifdef WIN_MAC
  font = ParseFont ("Monaco, 9");
#endif
#ifdef WIN_MSWIN
  font = ParseFont ("Courier, 9");
#endif
#ifdef WIN_MOTIF
  font = ParseFont ("fixed, 12");
#endif
  SelectFont (font);
  charwidth  = CharWidth ('0');
  lineheight = FontHeight () + SEQ_Y_OFFSET;

  w = DocumentWindow (left, top, (Int2)(-10), (Int2)(-10),  windowname, NULL, ResizeSeqEditorWindow);
  SetObjectExtra (w, (Pointer) sefp, CleanupSeqEdForm);
  sefp->form = (ForM) w;
  sefp->formmessage = SeqEdFormMessage;

  sefp->input_entityID = ObjMgrGetEntityIDForPointer (bsp);

  /* initialize view pointer   */
  sefp->bfp = (BioseqViewFormPtr) MemNew (sizeof (BioseqViewForm));
  if (sefp->bfp == NULL) return NULL; 
  
  sefp->bfp->bvd.bsp = bsp;
  sefp->moltype = GetMoleculeType (bsp);
  sefp->bfp->bvd.seqAlignMode = FALSE;
  sefp->bfp->bvd.DrawGrid = FALSE;
  sefp->bfp->bvd.SeqStartPosX = 100; 
  sefp->bfp->bvd.CharHeight = FontHeight ();
  sefp->bfp->bvd.displayFont = font;
  sefp->bfp->bvd.LineHeight = lineheight;
  sefp->bfp->bvd.CharWidth = charwidth;
  sefp->bfp->bvd.seq_entityID = sefp->input_entityID;
  sefp->bfp->bvd.on_the_fly = FALSE;
 
  CreateSeqEdMenus (w);
   
  if (is_prot)
  {
  	btns_across = 4;
  }
  else
  {
  	btns_across = 5;
  }
  sefp->upper_button_group = HiddenGroup (w, -10, -10, NULL);
  SetGroupSpacing (sefp->upper_button_group, 10, 3);
  
  g = HiddenGroup (sefp->upper_button_group, btns_across, 0, NULL);                  
  SetGroupSpacing (g, 10, 3);
  sprintf (str, "%ld", (long) sefp->edit_pos_start);
  sefp->goto_btn = PushButton (g, "Go to:", SeqEdGoToButton);
  sefp->goto_txt = DialogText (g, str, (Int2)6, NULL);
  sefp->lookat_btn = PushButton (g, "Look at:", SeqEdLookAtButton);
  sefp->lookat_txt = DialogText (g, str, (Int2)6, NULL);
  if (!is_prot) 
  {
     pop = PopupList (g, TRUE, SeqEdSelectFeatEditMode);
     PopupItem (pop, "Merge feature mode"); 
     PopupItem (pop, "Split feature mode"); 
     SetValue (pop, 1);
  }
  
  /* numbering location and grid controls */  
  StaticPrompt (g, " Numbering: ", 0, popupMenuHeight, programFont, 'l');
  sefp->bfp->bvd.newNumControl = PopupList (g, TRUE, ChangeSeqEditControls);
  SetObjectExtra (sefp->bfp->bvd.newNumControl, sefp, NULL);
  PopupItem (sefp->bfp->bvd.newNumControl, "None");
  PopupItem (sefp->bfp->bvd.newNumControl, "Side");
  PopupItem (sefp->bfp->bvd.newNumControl, "Top");
  SetValue (sefp->bfp->bvd.newNumControl, 3);    
  StaticPrompt (g, " Grid: ", 0, popupMenuHeight, programFont, 'l');
  sefp->bfp->bvd.newGridControl = PopupList (g, TRUE, ChangeSeqEditControls);
  SetObjectExtra (sefp->bfp->bvd.newGridControl, sefp, NULL);
  PopupItem (sefp->bfp->bvd.newGridControl, "On");
  PopupItem (sefp->bfp->bvd.newGridControl, "Off");
  SetValue (sefp->bfp->bvd.newGridControl, 2);
  
  g = HiddenGroup (w, 1, 0, NULL);
                      
  sefp->position_label = StaticPrompt (g, "", window_width, dialogTextHeight, programFont, 'l');
  SeqEdUpdateStatus (sefp);

  
  g = HiddenGroup (w, 1, 0, NULL);
  sefp->bfp->bvd.seqView = AutonomousPanel4 (g, window_width, window_hgt, OnDrawSeqEdPanel, onSeqEdVScrollBarSeqPanel, NULL, sizeof (BioseqViewPtr), onCloseSeqEdPanel, NULL);

  SetObjectExtra (sefp->bfp->bvd.seqView, sefp->bfp, NULL);
  
  sefp->lower_button_group = HiddenGroup (w, 6, 0, NULL);
  SetGroupSpacing (sefp->lower_button_group, 10, 3);
  StaticPrompt (sefp->lower_button_group, "Features:", 0, dialogTextHeight, programFont, 'l');
  sefp->bfp->bvd.newFeatControl = PopupList (sefp->lower_button_group, TRUE, ChangeSeqEditControls);
  PopupItem (sefp->bfp->bvd.newFeatControl, "Shown");
  PopupItem (sefp->bfp->bvd.newFeatControl, "Hidden");
  SetValue (sefp->bfp->bvd.newFeatControl, 1);
  SetObjectExtra (sefp->bfp->bvd.newFeatControl, sefp, NULL);
  sefp->accept = PushButton (sefp->lower_button_group, "Accept", SeqEdAcceptButton);
  SetObjectExtra (sefp->accept, sefp, NULL);
  sefp->cancel = PushButton (sefp->lower_button_group, "Cancel", SeqEdCancelButton);
  SetObjectExtra (sefp->cancel, sefp, NULL);
  if (!is_prot) 
  {
    b = PushButton (sefp->lower_button_group, "Translate CDS", SeqEdTranslateCDSBtn);
    SetObjectExtra (b, sefp, NULL);
  }
  ResizeSeqEdView (sefp);
  RealizeWindow (w);
  SetSlateChar ((SlatE) sefp->bfp->bvd.seqView, SeqEdOnKey);
  SetSeqEdPanelClick (sefp);
  
  /* register to receive update messages */
  sefp->userkey = OMGetNextUserKey ();
  sefp->procid = 0;
  sefp->proctype = OMPROC_EDIT;
  omudp = ObjMgrAddUserData (sefp->input_entityID, sefp->procid, sefp->proctype, sefp->userkey);
  if (omudp != NULL) {
    omudp->userdata.ptrvalue = (Pointer) sefp;
    omudp->messagefunc = SeqEditMsgFunc;
  }

  return (ForM) w;
}

static Int4 GetScrollPosForAlnPos (Int4 aln_pos, BioseqViewPtr bvp)
{
  Int4 aln_line = 0, display_line, mult;
  
  if (bvp == NULL || aln_pos < 0)
  {
    return 0;
  }
  
  aln_line = aln_pos / bvp->CharsAtLine;
  if (aln_line >= bvp->TotalLines)
  {
    return bvp->TotalLines - 1;
  }
  
  if (bvp->SeqPanLines[aln_line]->bioSeqLine > 0)
  {
    mult = aln_line / bvp->SeqPanLines[aln_line]->bioSeqLine;
    if (mult > 1)
    {
      display_line = aln_line * mult;
    }
    else
    {
      display_line = aln_line;
    }
  }
  else
  {
    display_line = aln_line;
  }
  
  if (display_line >= bvp->TotalLines)
  {
    display_line = bvp->TotalLines - 1;
  }
  
  while (bvp->SeqPanLines[display_line]->bioSeqLine > aln_line
         && display_line > -1)
  {
    display_line --;
  }
  
  while (bvp->SeqPanLines[display_line]->bioSeqLine < aln_line 
         && display_line < bvp->TotalLines - 1)
  {
    display_line++;
  }

  if (bvp->SeqPanLines[display_line]->bioSeqLine == aln_line)
  {
    while (display_line > -1 
           && bvp->SeqPanLines[display_line]->bioSeqLine == aln_line)
    {
      display_line --;
    }
    display_line++;
  }
  return display_line;
}

static void SeqAlnSelectItem (BioseqViewPtr bvp, Int2 itemID)
{
  ValNodePtr vnp;
  
  for (vnp = bvp->Selection; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->data.intvalue == itemID)
    {
      vnp->choice = 1;
      return;    
    }
  }
  ValNodeAddInt (&bvp->Selection, 1, itemID);
}

static void SeqAlnUnselectItem (BioseqViewPtr bvp, Int2 itemID)
{
  ValNodePtr vnp;
  
  for (vnp = bvp->Selection; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->data.intvalue == itemID)
    {
      vnp->choice = 0;
      return;    
    }
  }
}

static void SeqAlnScrollToAlnPos (SeqEdFormPtr sefp, Int4 pos)
{
  BaR          sb;
  Int4         scroll_pos, bmax;
  WindoW       currentport, temport;
  Int4         aln_len;

  if (sefp == NULL 
      || sefp->bfp == NULL
      || sefp->bfp->bvd.salp == NULL)
  {
    return;
  }
  aln_len = AlnMgr2GetAlnLength(sefp->bfp->bvd.salp, FALSE); 

  if (pos < 0)
  {
    pos = 0;
  }
  else if (pos > aln_len)
  {
    pos = aln_len;
  }

  sb = GetSlateVScrollBar ((SlatE) sefp->bfp->bvd.seqView);
  if (sb == NULL) return;
  bmax = GetBarMax (sb);

  scroll_pos = GetScrollPosForAlnPos (pos, &(sefp->bfp->bvd));
  if (scroll_pos > bmax)
  {
    scroll_pos = bmax;
  }
  
  SetBarValue (sb, scroll_pos);
  currentport = ParentWindow (sefp->bfp->bvd.seqView);
  temport = SavePort (currentport);
  Select (sefp->bfp->bvd.seqView);
  inval_panel (sefp->bfp->bvd.seqView, -1, -1);	
  RestorePort (temport);
}

static void SeqAlnGoToAlnPosBtn (ButtoN b)
{
  SeqEdFormPtr sefp;
  Int4         aln_pos;
  Char         str [16];

  sefp = (SeqEdFormPtr) GetObjectExtra ((WindoW)ParentWindow (b));
  if (sefp == NULL || sefp->bfp == NULL) return;
  
  GetTitle (sefp->goto_txt, str, 15);
  if (! CCStrToLong (str, &aln_pos))
  {
  	return;
  }
  
  SeqAlnScrollToAlnPos (sefp, aln_pos);
}

static void SeqAlnGoToSeqPosBtn (ButtoN b)
{
  SeqEdFormPtr sefp;
  Int4         seq_pos, aln_pos;
  Char         str [16];
  SeqIdPtr     sip;
  BioseqPtr    bsp;

  sefp = (SeqEdFormPtr) GetObjectExtra ((WindoW)ParentWindow (b));
  if (sefp == NULL 
      || sefp->bfp == NULL
      || sefp->bfp->bvd.salp == NULL)
  {
    return;
  }

  sip = AlnMgr2GetNthSeqIdPtr(sefp->bfp->bvd.salp, sefp->bfp->bvd.TargetRow);
  bsp = BioseqFind (sip);
  if (bsp == NULL)
  {
    return;
  }
  
  GetTitle (sefp->lookat_txt, str, 15);
  if (! CCStrToLong (str, &seq_pos))
  {
  	return;
  }
  
  if (seq_pos < 0)
  {
    seq_pos = 0;
  }
  else if (seq_pos >= bsp->length)
  {
    seq_pos = bsp->length - 1;
  }
  
  aln_pos = AlnMgr2MapBioseqToSeqAlign (sefp->bfp->bvd.salp,
                                        seq_pos, 
                                        sefp->bfp->bvd.TargetRow);
  
  SeqAlnScrollToAlnPos (sefp, aln_pos);
}

static void SeqAlnClose (SeqEdFormPtr sefp)

{  
  if (sefp == NULL) return;
  Hide (sefp->form);
  Remove (sefp->form); 
  Update ();	
}

static void SeqAlnCloseButton (ButtoN b)

{
  SeqEdFormPtr  sefp;
  
  sefp = (SeqEdFormPtr) GetObjectExtra (b);
  SeqAlnClose (sefp);
}

static void SeqAlnCloseMenuItem (IteM i)

{
  SeqEdFormPtr  sefp;
  
  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  SeqAlnClose (sefp);
}

static Int4 FindMaxLabelLen (SeqAlignPtr salp)
{
  SeqIdPtr  sip;
  BioseqPtr bsp;
  Char      tmpbuf[42];
  Int4      i;
  Int4      max_len = 0;
  Int4      len;

  if (salp == NULL) return 0;
  for (i = 0; i < salp->dim; i++) {
    sip = AlnMgr2GetNthSeqIdPtr(salp, i + 1);
    bsp = BioseqFind (sip);
    if (bsp != NULL) {
      sip = SeqIdFindBestAccession (bsp->id);
    }
    SeqIdWrite (sip, tmpbuf, PRINTID_TEXTID_ACCESSION, 41);
    len = StringLen (tmpbuf) + 1;
    if (len > max_len) {
      max_len = len;
    }
  }
  return max_len;
}

static CharPtr GetSeqAlignLabels (SeqAlignPtr salp, Int4Ptr label_len)
{
  CharPtr   labels;
  BioseqPtr bsp;
  SeqIdPtr  sip;
  Int4      i;

  if (salp == NULL || label_len == NULL) {
    return NULL;
  }

  *label_len = FindMaxLabelLen (salp);
  if (*label_len < 1) return NULL;

  labels = (CharPtr) MemNew ((*label_len + 1) * salp->dim * sizeof (Char));
  if (labels == NULL) return NULL;
  MemSet (labels, 0, *label_len * salp->dim * sizeof (Char));

  for (i = 0; i < salp->dim; i++) {
    sip = AlnMgr2GetNthSeqIdPtr(salp, i + 1);
    bsp = BioseqFind (sip);
    if (bsp != NULL) {
      sip = SeqIdFindBestAccession (bsp->id);
    }
    SeqIdWrite (sip, labels + i * (*label_len + 1) * sizeof (Char),
                PRINTID_TEXTID_ACCESSION, *label_len);
  }
  return labels;
}

extern void 
WriteAlignmentInterleaveToFile 
(SeqAlignPtr salp,
 FILE        *fp,
 Int4        seq_chars_per_row,
 Boolean     show_substitutions)
{
  Int4     row, start, stop;
  Uint1Ptr alnbuf;
  Uint1Ptr seqbuf;
  Int4     alnbuf_len;
  Int4     aln_len = AlnMgr2GetAlnLength(salp, FALSE);
  CharPtr  alnlabels;
  CharPtr  printed_line;
  Int4     printed_line_len;
  CharPtr  label_pos;
  Int4     label_len = 0;

  if (salp == NULL || fp == NULL) return;

  alnlabels = GetSeqAlignLabels (salp, &label_len);
  if (alnlabels != NULL) {
    alnbuf = (Uint1Ptr) MemNew ((seq_chars_per_row + 1)* sizeof (Uint1));
    if (alnbuf != NULL) {
      seqbuf = (Uint1Ptr) MemNew ((seq_chars_per_row + 1) * sizeof (Uint1));
      if (seqbuf != NULL) {
        printed_line_len = label_len + 1 + seq_chars_per_row + 3;
        printed_line = (CharPtr) MemNew (printed_line_len * sizeof (Char));
        if (printed_line != NULL) {
          printed_line [ printed_line_len - 1] = 0;
          printed_line [ printed_line_len - 2] = '\n';
          start = 0;
          stop = seq_chars_per_row - 1;
          while (start < aln_len) {
            for (row = 1; row <= salp->dim; row++) {
              MemSet (printed_line, ' ', printed_line_len - 2);
              label_pos = alnlabels + (row - 1) * (label_len + 1) * sizeof (Char);
              MemCpy (printed_line, label_pos, StringLen (label_pos));
              AlignmentIntervalToString (salp, row, start, stop, 1, TRUE, 
                                         seqbuf, alnbuf, &alnbuf_len,
                                         show_substitutions);
              MemCpy (printed_line + label_len + 1, alnbuf, alnbuf_len);
              fprintf (fp, printed_line);
            }
            fprintf (fp, "\n");
            start = stop + 1;
            stop += seq_chars_per_row;
          }
          MemFree (printed_line);
        }
        MemFree (seqbuf);
      }
      MemFree (alnbuf);
    }
    MemFree (alnlabels);
  }
}

extern void WriteAlignmentContiguousToFile
(SeqAlignPtr salp,
 FILE        *fp,
 Int4        seq_chars_per_row,
 Boolean     show_substitutions)
{
  Int4         num_segments;
  SeqAlignPtr  tmp_salp;
  Int4         idx;
  CharPtr PNTR alnlabels = NULL;
  Int4Ptr      label_len = NULL;
  Int4Ptr      aln_len = NULL;
  Uint1Ptr     alnbuf = NULL;
  Uint1Ptr     seqbuf = NULL;
  CharPtr      printed_line = NULL;
  Int4         alnbuf_len;
  Int4         printed_line_len;
  CharPtr      label_pos;
  Int4         row, start, stop;
  
  if (salp == NULL || fp == NULL) return;
  
  num_segments = 0;
  for (tmp_salp = salp; tmp_salp != NULL; tmp_salp = tmp_salp->next)
  {
  	num_segments++;
  }
  
  
  /* get labels and lengths for all segments */
  alnlabels = (CharPtr PNTR) MemNew (sizeof (CharPtr) * num_segments);
  label_len = (Int4Ptr) MemNew (sizeof (Int4) * num_segments);
  aln_len = (Int4Ptr) MemNew (sizeof (Int4) * num_segments);
  if (alnlabels != NULL && label_len != NULL && aln_len != NULL)
  {  	
    for (tmp_salp = salp, idx = 0; tmp_salp != NULL && idx < num_segments; tmp_salp = tmp_salp->next, idx++)
    {
  	  alnlabels [idx] = GetSeqAlignLabels (tmp_salp, &label_len[idx]);
      aln_len [idx]= AlnMgr2GetAlnLength(tmp_salp, FALSE);

    }

    /* get buffers */
    alnbuf = (Uint1Ptr) MemNew (seq_chars_per_row * sizeof (Uint1));
    seqbuf = (Uint1Ptr) MemNew (seq_chars_per_row * sizeof (Uint1));
    printed_line_len = seq_chars_per_row + 3;
    printed_line = (CharPtr) MemNew (printed_line_len * sizeof (Char));
    if (alnbuf != NULL && seqbuf != NULL && printed_line != NULL) {
      printed_line [ printed_line_len - 1] = 0;
      printed_line [ printed_line_len - 2] = '\n';

      for (row = 1; row <= salp->dim; row++) {
        if (salp->next != NULL)
        {
          fprintf (fp, "[\n");
        }
        for (tmp_salp = salp, idx = 0;
             tmp_salp != NULL && idx < num_segments;
             tmp_salp = tmp_salp->next, idx++)
        {
          label_pos = alnlabels [idx] + (row - 1) * (label_len[idx] + 1) * sizeof (Char);
          fprintf (fp, ">%s\n", label_pos);
          start = 0;
          stop = seq_chars_per_row - 1;
          while (start < aln_len [idx]) {
            MemSet (printed_line, ' ', printed_line_len - 2);
            AlignmentIntervalToString (tmp_salp, row, start, stop, 1, TRUE, 
                                       seqbuf, alnbuf, &alnbuf_len,
                                       show_substitutions);
            MemCpy (printed_line, alnbuf, alnbuf_len);
            fprintf (fp, printed_line);
            start = stop + 1;
            stop += seq_chars_per_row;
          }
          fprintf (fp, "\n");
        }
        if (salp->next != NULL)
        {
          fprintf (fp, "]\n");
        }
      }
    }
    MemFree (alnbuf);
    MemFree (seqbuf);
    MemFree (printed_line);
  }
  MemFree (label_len);  
  MemFree (alnlabels);
  MemFree (aln_len);
}

static void SeqAlnExportAln (SeqEdFormPtr sefp, Boolean interleave)
{
  FILE *fp;
  Char  path[PATH_MAX];
  
  if (sefp == NULL || sefp->bfp == NULL || sefp->bfp->bvd.salp == NULL)
  {
    return;
  }

  if (! GetOutputFileName (path, sizeof (path), "")) return;
  fp = FileOpen (path, "w");
  if (fp == NULL) 
  {
    Message (MSG_ERROR, "Unable to open %s", path);
    return;
  }
  if (interleave)
  {
    WriteAlignmentInterleaveToFile (sefp->bfp->bvd.salp, fp,
                                    sefp->bfp->bvd.CharsAtLine,
                                    sefp->bfp->bvd.showAlnSubstitutions);
  } else {
    WriteAlignmentContiguousToFile (sefp->bfp->bvd.salp, fp, 
                                    sefp->bfp->bvd.CharsAtLine,
                                    sefp->bfp->bvd.showAlnSubstitutions);
  }
  
  FileClose (fp);
}

static void SeqAlnExportAlnMenuItem (IteM i, Boolean interleave)
{
  SeqEdFormPtr  sefp;
  
  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  SeqAlnExportAln (sefp, interleave);
}

static void SeqAlnExportInterleave (IteM i)
{
  SeqAlnExportAlnMenuItem (i, TRUE);
}

static void SeqAlnExportContiguous (IteM i)
{
  SeqAlnExportAlnMenuItem (i, FALSE);
}

static void SeqAlnShowSubstitutionsMenuItem (IteM i)
{
  SeqEdFormPtr  sefp;
  WindoW        temport;
  
  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp != NULL && sefp->bfp != NULL)
  {
    sefp->bfp->bvd.showAlnSubstitutions = GetStatus (i);
    temport = SavePort(sefp->form);
    Select (sefp->bfp->bvd.seqView);
    inval_panel (sefp->bfp->bvd.seqView, -1, -1);
    RestorePort (temport);
    Update ();
  }
}

static void RemoveSequencesFromAlignment (IteM i)
{
  SeqEdFormPtr  sefp;
  Int4          n;
  SeqIdPtr      sip;
  BioseqPtr     bsp;
  SeqEntryPtr   sep;
  ValNodePtr    sip_list = NULL, vnp;
  
  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  
  if (sefp != NULL && sefp->bfp != NULL && sefp->bfp->bvd.salp != NULL)
  {
    sep = GetTopSeqEntryForEntityID (sefp->input_entityID);
    /* first, check for pairwise alignments */
    for (n = 1; n < sefp->bfp->bvd.salp->dim; n++)
    {
      sip = AlnMgr2GetNthSeqIdPtr(sefp->bfp->bvd.salp, n);
      bsp = BioseqFind (sip);
      if (bsp != NULL && isSelected (bsp->idx.itemID, &(sefp->bfp->bvd)))
      {
        if (IsSequenceFirstInPairwise (sep, sip))
	      {
	  	    Message (MSG_ERROR, "One of the selected sequences is the first in a pairwise alignment."
	  	    "  You must convert the alignment to a multiple alignment before trying to remove this sequence.");
	  	    sip_list = ValNodeFree (sip_list);
          return;
	      }
	      else
	      {
	        ValNodeAddPointer (&sip_list, bsp->idx.itemID, sip);
	      }
      }
    }
    
    for (vnp = sip_list; vnp != NULL; vnp = vnp->next)
    {
      SeqAlignBioseqDeleteById (sefp->bfp->bvd.salp, vnp->data.ptrvalue);
      SeqAlnUnselectItem (&(sefp->bfp->bvd), vnp->choice);
    }
    sip_list = ValNodeFree (sip_list);
    DeleteMarkedObjects (sefp->input_entityID, 0, NULL);
    ObjMgrSetDirtyFlag (sefp->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, sefp->input_entityID, 0, 0);  
    /* reindex alignment */
    SAIndex2Free2(sefp->bfp->bvd.salp->saip);
    sefp->bfp->bvd.salp->saip = NULL;
    AlnMgr2IndexSeqAlign(sefp->bfp->bvd.salp);
    /* update alignment editor window */
    ResizeSeqEditorWindow ((WindoW)sefp->form);
  }
}

typedef struct setalntarget 
{
  SeqEdFormPtr sefp;
  Int4         target;  
} SetAlnTargetData, PNTR SetAlnTargetPtr;

static void SeqAlnSetTarget (IteM i)
{
  SetAlnTargetPtr satp;
  
  satp = (SetAlnTargetPtr) GetObjectExtra (i);
  if (satp != NULL && satp->sefp != NULL
      && satp->sefp->bfp != NULL
      && satp->sefp->bfp->bvd.salp != NULL
      && satp->target > 0
      && satp->target <= satp->sefp->bfp->bvd.salp->dim)
  {
    satp->sefp->bfp->bvd.TargetRow = satp->target;
    ResizeSeqEditorWindow ((WindoW) satp->sefp->form);
  }
}

static void SeqAlnValidateAlignment (IteM i)
{
  SeqEdFormPtr    sefp;

  sefp = (SeqEdFormPtr) GetObjectExtra (i);
  if (sefp == NULL) return;
  
  ValidateSeqAlign (sefp->bfp->bvd.salp, 0, TRUE, TRUE, TRUE, FALSE, FALSE, NULL);
}

static void CreateAlnMenus (WindoW w)

{
  MenU            edit_menu;
  IteM            localItem;
  MenU            sub;
  SeqEdFormPtr    sefp;
  Int4            n;
  SetAlnTargetPtr satp;
  Char            label [128];
  SeqIdPtr        sip;
  BioseqPtr       bsp;
  
  if (w == NULL) return;
  sefp = (SeqEdFormPtr) GetObjectExtra (w);
  if (sefp == NULL) return;
  
  /* File Menu */
  edit_menu = PulldownMenu (w, "File");
  sub = SubMenu (edit_menu, "Export");
  localItem = CommandItem (sub, "Interleave", SeqAlnExportInterleave);
  SetObjectExtra (localItem, sefp, NULL);
  localItem = CommandItem (sub, "Contiguous", SeqAlnExportContiguous);
  SetObjectExtra (localItem, sefp, NULL);
  
  localItem = CommandItem (edit_menu, "Close", SeqAlnCloseMenuItem);
  SetObjectExtra (localItem, sefp, NULL);
  
  /* Edit Menu */
  edit_menu = PulldownMenu (w, "Edit");
  localItem = CommandItem (edit_menu, "Remove Sequences From Alignment", 
                           RemoveSequencesFromAlignment);
  SetObjectExtra (localItem, sefp, NULL);
  localItem = CommandItem (edit_menu, "Validate Alignemtn", SeqAlnValidateAlignment);
  SetObjectExtra (localItem, sefp, NULL);

  /* View menu */
  edit_menu = PulldownMenu (w, "View");
  sub = SubMenu (edit_menu, "Target");
  for (n = 1; n <= sefp->bfp->bvd.salp->dim; n++)
  {
    sip = AlnMgr2GetNthSeqIdPtr(sefp->bfp->bvd.salp, n);
    bsp = BioseqFind (sip);
    if (bsp != NULL)
    {
      sip = SeqIdFindBestAccession (bsp->id);
    }
    SeqIdWrite (sip, label, PRINTID_REPORT, sizeof (label) - 1);
    localItem = CommandItem (sub, label, SeqAlnSetTarget);
    satp = (SetAlnTargetPtr) MemNew (sizeof (SetAlnTargetData));
    if (satp != NULL)
    {
      satp->sefp = sefp;
      satp->target = n;
    }
    SetObjectExtra (localItem, satp, StdCleanupExtraProc);
  }
  localItem = StatusItem (edit_menu, "Show Substitutions", SeqAlnShowSubstitutionsMenuItem);
  SetObjectExtra (localItem, sefp, NULL);
}

extern ForM CreateAlnEditorWindow (Int2 left, Int2 top, CharPtr windowname, SeqAlignPtr salp, Uint2 entityID)
{
  SeqEdFormPtr       sefp;
  WindoW             w;
  GrouP              g;
  Char               str [16];
  Int2               window_width = 650;
  Int2               window_hgt = 300,
                     charwidth,
                     lineheight;
  FonT               font;
  Int4               btns_across;
  OMUserDataPtr      omudp;
  SeqIdPtr           sip;
  
  if (salp == NULL)
  {
    return NULL;
  }
  sefp = SeqEdFormNew ();
  if (sefp == NULL) 
  {
    return NULL;
  }
  
#ifdef WIN_MAC
  font = ParseFont ("Monaco, 9");
#endif
#ifdef WIN_MSWIN
  font = ParseFont ("Courier, 9");
#endif
#ifdef WIN_MOTIF
  font = ParseFont ("fixed, 12");
#endif
  SelectFont (font);
  charwidth  = CharWidth ('0');
  lineheight = FontHeight () + SEQ_Y_OFFSET;

  w = DocumentWindow (left, top, (Int2)(-10), (Int2)(-10),  windowname, NULL, ResizeSeqEditorWindow);
  SetObjectExtra (w, (Pointer) sefp, CleanupSeqEdForm);
  sefp->form = (ForM) w;
  sefp->formmessage = SeqEdFormMessage;

  sefp->input_entityID = entityID;

  /* initialize view pointer   */
  sefp->bfp = (BioseqViewFormPtr) MemNew (sizeof (BioseqViewForm));
  if (sefp->bfp == NULL) return NULL; 
  
  sefp->bfp->bvd.seqAlignMode = TRUE;
  sefp->bfp->bvd.salp = salp;
  AlnMgr2IndexSeqAlign(sefp->bfp->bvd.salp);
  sip = AlnMgr2GetNthSeqIdPtr(sefp->bfp->bvd.salp, 1);
  sefp->bfp->bvd.bsp = BioseqFind (sip);
  sefp->bfp->bvd.TargetRow = 1;
  sefp->bfp->bvd.DrawGrid = FALSE;
  sefp->bfp->bvd.SeqStartPosX = (FindMaxLabelLen (sefp->bfp->bvd.salp) + 10) * charwidth; 
  sefp->bfp->bvd.CharHeight = FontHeight ();
  sefp->bfp->bvd.displayFont = font;
  sefp->bfp->bvd.LineHeight = lineheight;
  sefp->bfp->bvd.CharWidth = charwidth;
  sefp->bfp->bvd.seq_entityID = sefp->input_entityID;
  sefp->bfp->bvd.on_the_fly = FALSE;
  sefp->bfp->bvd.showAlnSubstitutions = FALSE;
 
  CreateAlnMenus (w);
   
	btns_across = 4;
  sefp->upper_button_group = HiddenGroup (w, -10, -10, NULL);
  SetGroupSpacing (sefp->upper_button_group, 10, 3);
  
  g = HiddenGroup (sefp->upper_button_group, btns_across, 0, NULL);                  
  SetGroupSpacing (g, 10, 3);
  sprintf (str, "%ld", (long) sefp->edit_pos_start);
  sefp->goto_btn = PushButton (g, "Go to alignment position:", SeqAlnGoToAlnPosBtn);
  sefp->goto_txt = DialogText (g, str, (Int2)6, NULL);
  sefp->lookat_btn = PushButton (g, "Go to sequence position:", SeqAlnGoToSeqPosBtn);
  sefp->lookat_txt = DialogText (g, str, (Int2)6, NULL);
  
  /* numbering location and grid controls */  
  StaticPrompt (g, " Numbering: ", 0, popupMenuHeight, programFont, 'l');
  sefp->bfp->bvd.newNumControl = PopupList (g, TRUE, ChangeSeqEditControls);
  SetObjectExtra (sefp->bfp->bvd.newNumControl, sefp, NULL);
  PopupItem (sefp->bfp->bvd.newNumControl, "None");
  PopupItem (sefp->bfp->bvd.newNumControl, "Side");
  PopupItem (sefp->bfp->bvd.newNumControl, "Top");
  SetValue (sefp->bfp->bvd.newNumControl, 3);    
  StaticPrompt (g, " Grid: ", 0, popupMenuHeight, programFont, 'l');
  sefp->bfp->bvd.newGridControl = PopupList (g, TRUE, ChangeSeqEditControls);
  SetObjectExtra (sefp->bfp->bvd.newGridControl, sefp, NULL);
  PopupItem (sefp->bfp->bvd.newGridControl, "On");
  PopupItem (sefp->bfp->bvd.newGridControl, "Off");
  SetValue (sefp->bfp->bvd.newGridControl, 2);
  
  g = HiddenGroup (w, 1, 0, NULL);
                      
  sefp->position_label = StaticPrompt (g, "", window_width, dialogTextHeight, programFont, 'l');
  SeqEdUpdateStatus (sefp);
  
  g = HiddenGroup (w, 1, 0, NULL);

  sefp->bfp->bvd.seqView = AutonomousPanel4 (g, window_width, window_hgt, onDrawSeqPanel, onVScrollBarSeqPanel, NULL, sizeof (BioseqViewPtr), onCloseSeqPanel, NULL);

  SetObjectExtra (sefp->bfp->bvd.seqView, sefp->bfp, NULL);
  
  sefp->lower_button_group = HiddenGroup (w, 6, 0, NULL);
  SetGroupSpacing (sefp->lower_button_group, 10, 3);
  StaticPrompt (sefp->lower_button_group, "Features:", 0, dialogTextHeight, programFont, 'l');
  sefp->bfp->bvd.newFeatControl = PopupList (sefp->lower_button_group, TRUE, ChangeSeqEditControls);
  PopupItem (sefp->bfp->bvd.newFeatControl, "Target Sequence");
  PopupItem (sefp->bfp->bvd.newFeatControl, "Hidden");
  PopupItem (sefp->bfp->bvd.newFeatControl, "All Sequences");
  SetValue (sefp->bfp->bvd.newFeatControl, 2);
  SetObjectExtra (sefp->bfp->bvd.newFeatControl, sefp, NULL);
  sefp->accept = PushButton (sefp->lower_button_group, "Close", SeqAlnCloseButton);
  SetObjectExtra (sefp->accept, sefp, NULL);
  ResizeSeqView (&(sefp->bfp->bvd));
  RealizeWindow (w);
/*  SetSlateChar ((SlatE) sefp->bfp->bvd.seqView, SeqEdOnKey); */
  SetSeqAlnPanelClick (sefp);
  
  /* register to receive update messages */
  sefp->userkey = OMGetNextUserKey ();
  sefp->procid = 0;
  sefp->proctype = OMPROC_EDIT;
  omudp = ObjMgrAddUserData (sefp->input_entityID, sefp->procid, sefp->proctype, sefp->userkey);
  if (omudp != NULL) {
    omudp->userdata.ptrvalue = (Pointer) sefp;
    omudp->messagefunc = SeqEditMsgFunc;
  }

  return (ForM) w;
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

/* The following section of code is used for retranslating a CDS and updating
 * the protein features based on an alignment between the old and new protein
 * sequences.
 */
static Int4 SeqEdRemapCoord (SeqAlignPtr salp, Int4 coord, Boolean move_up, Int4 len)

{
  Int4 aln_pos;
  
  if (salp == NULL) return -1;
  aln_pos = AlnMgr2MapBioseqToSeqAlign (salp, coord, 1);
  while (aln_pos == -1)
  {
  	if (move_up)
  	{
  	  if (coord >= len - 1)
  	  {
  	  	return len - 1;
  	  }
  	  else
  	  {
  	  	coord ++;
  	  }
  	}
  	else
  	{
  	  if (coord <= 0)
  	  {
  	  	return 0;
  	  }
  	  else
  	  {
  	  	coord --;
  	  }
  	}
  	aln_pos = AlnMgr2MapBioseqToSeqAlign (salp, coord, 1);
  }
  return AlnMgr2MapSeqAlignToBioseq (salp, aln_pos, 2);
}


static void SeqEdRemapSeqIntLoc (SeqAlignPtr salp, SeqIntPtr sintp, Int4 seq_len)

{  
  if (salp == NULL || sintp == NULL) return;
  sintp->from = SeqEdRemapCoord (salp, sintp->from, TRUE, seq_len);
  sintp->to = SeqEdRemapCoord (salp, sintp->to, FALSE, seq_len); 
}

static void SeqEdRemapSeqPntLoc (SeqAlignPtr salp, SeqPntPtr spp, Int4 seq_len)

{  
  if (salp == NULL || spp == NULL) return;
  
  spp->point = SeqEdRemapCoord (salp, spp->point, FALSE, seq_len);
}


static void SeqEdRemapPackSeqPnt (SeqAlignPtr salp, PackSeqPntPtr pspp, Int4 seq_len)

{
  Uint1          used;

  if (salp == NULL || pspp == NULL) return;
  for (used = 0; used < pspp->used; used++) 
  {
    pspp->pnts [used] = SeqEdRemapCoord (salp, pspp->pnts [used], FALSE, seq_len);
  }
}


NLM_EXTERN void SeqEdRemapLocation (SeqAlignPtr salp, SeqLocPtr slp, Int4 seq_len)

{

  if (slp == NULL) return;
  switch (slp->choice) {
    case SEQLOC_INT :
      SeqEdRemapSeqIntLoc (salp, slp->data.ptrvalue, seq_len);
      break;
    case SEQLOC_PNT :
      SeqEdRemapSeqPntLoc (salp, slp->data.ptrvalue, seq_len);
      break;
    case SEQLOC_PACKED_PNT :
      SeqEdRemapPackSeqPnt (salp, slp->data.ptrvalue, seq_len);
      break;
    default :
      break;
  }
}


static Boolean LocationMatchesEntireSequence (SeqLocPtr slp, BioseqPtr bsp)
{
  SeqIntPtr sip;
  
  if (slp == NULL || bsp == NULL) return FALSE;
  
  if (slp->choice == SEQLOC_WHOLE && SeqIdIn (slp->data.ptrvalue, bsp->id))
  {
  	return TRUE;
  }
  else if (slp->choice == SEQLOC_INT)
  {
    sip = (SeqIntPtr) slp->data.ptrvalue;
    if (sip != NULL && sip->from == 0 && sip->to == bsp->length - 1)
    {
      return TRUE;
    }
  }
  return FALSE;
}

static void MakeLocationMatchEntireSequence (SeqLocPtr slp, BioseqPtr bsp)
{
  SeqIntPtr sip;
  
  if (slp == NULL || bsp == NULL) return;
  
  if (slp->choice == SEQLOC_WHOLE)
  {
  	SeqIdFree (slp->data.ptrvalue);
  	slp->data.ptrvalue = SeqIdDup (bsp->id);
  }
  else if (slp->choice == SEQLOC_INT)
  {
    sip = (SeqIntPtr) slp->data.ptrvalue;
    if (sip == NULL)
    {
      sip = SeqIntNew ();
      slp->data.ptrvalue = sip;
    }
    if (sip != NULL)
    {
      sip->from = 0;
      sip->to = bsp->length - 1;
    }
  }
}

NLM_EXTERN Boolean SeqEdFixProteinFeatures (BioseqPtr oldbsp, BioseqPtr newbsp, Boolean force_fix)
{
  SeqAlignPtr       salp = NULL;
  Boolean           revcomp = FALSE;
  SeqFeatPtr        sfp;
  SeqMgrFeatContext fcontext;
  Boolean           tried_to_get_alignment = FALSE;
  Boolean           unmappable_feats = FALSE;
  SeqLocPtr         slp_tmp = NULL;
  
  if (oldbsp == NULL || newbsp == NULL) return FALSE;

  /* get alignment between old and new proteins */

  if (ISA_na (oldbsp->mol) != ISA_na (newbsp->mol)) return FALSE;

  /* iterate through the features on the old protein.  Full length features
   * should be set to the new length.  Other features should be mapped through
   * the alignment (if possible), otherwise warn the user that they could not
   * be remapped. */      

  if (!force_fix)
  {
    for (sfp = SeqMgrGetNextFeature (oldbsp, NULL, 0, 0, &fcontext);
         sfp != NULL && !unmappable_feats;
         sfp = SeqMgrGetNextFeature (oldbsp, sfp, 0, 0, &fcontext))
    {
      if (sfp->idx.subtype != FEATDEF_PROT)
      {
        if (salp == NULL)
        {
          salp = Sequin_GlobalAlign2Seq (oldbsp, newbsp, &revcomp);
        }
        if (salp == NULL)
        {
          unmappable_feats = TRUE;
        } 
        else
        {
          slp_tmp = (SeqLocPtr) AsnIoMemCopy (sfp->location,
                                              (AsnReadFunc) SeqLocAsnRead,
                                              (AsnWriteFunc) SeqLocAsnWrite);
          SeqEdRemapLocation (salp, slp_tmp, newbsp->length);	  	  	
          if (slp_tmp == NULL)
          {
            unmappable_feats = TRUE;
          }
          else
          {
            slp_tmp = SeqLocFree (slp_tmp);
          }
        }
      }
    }
    if (unmappable_feats)
    {
      return FALSE;
    }
  }
  
  for (sfp = SeqMgrGetNextFeature (oldbsp, NULL, 0, 0, &fcontext);
       sfp != NULL;
       sfp = SeqMgrGetNextFeature (oldbsp, sfp, 0, 0, &fcontext))
  {
    if (sfp->idx.subtype == FEATDEF_PROT)
    {
      /* make new location match new sequence length */
      MakeLocationMatchEntireSequence (sfp->location, newbsp);
    }
    else
    {
      if (salp == NULL && !tried_to_get_alignment)
      {
        salp = Sequin_GlobalAlign2Seq (oldbsp, newbsp, &revcomp);
        tried_to_get_alignment = TRUE;
      }
      if (salp != NULL)
      {
        SeqEdRemapLocation (salp, sfp->location, newbsp->length);	  	  	
      }
      else
      {
        unmappable_feats = TRUE;
      }
    }
  } 
        
  if (salp != NULL)
  {
    SeqAlignFree (salp);
  }
  if (unmappable_feats)
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}


NLM_EXTERN void SeqEdTranslateOneCDS (SeqFeatPtr sfp, BioseqPtr featbsp, Uint2 entityID)
{
  ByteStorePtr  bs;
  Char          ch;
  CharPtr       prot;
  CharPtr       ptr;
  Int4          star_at_end = 0;
  BioseqPtr     old_prot;
  SeqIdPtr      new_prot_id;
  SeqEntryPtr   parent, new_prot_sep;
  SeqLocPtr     slp;
  Uint1         seq_data_type;
  Int4          old_length;
  BioseqPtr     newbsp;
  ProtRefPtr    prp;
  SeqFeatPtr    prot_sfp;
  
  if (featbsp == NULL || sfp == NULL || sfp->location == NULL 
      || sfp->data.choice != SEQFEAT_CDREGION) 
  {
  	return;
  }
  
  old_prot = BioseqFindFromSeqLoc (sfp->product);
  new_prot_id = SeqIdDup (SeqLocId (sfp->product));
  if (new_prot_id == NULL)
  {
  	new_prot_id = MakeNewProteinSeqId (sfp->location, featbsp->id);
  }
  	
  bs = ProteinFromCdRegionEx (sfp, TRUE, FALSE);
  if (bs != NULL) {
    prot = BSMerge (bs, NULL);
    bs = BSFree (bs);
    if (prot != NULL) {
      ptr = prot;
      ch = *ptr;
      while (ch != '\0') {
        *ptr = TO_UPPER (ch);
        if (ch == '*') {
          star_at_end = 1;
        } else {
          star_at_end = 0;
        }
        ptr++;
        ch = *ptr;
      }
      if (star_at_end)
      {
      	*(ptr - 1) = 0;
      }
      bs = BSNew (1000);
      if (bs != NULL) {
        ptr = prot;
        BSWrite (bs, (VoidPtr) ptr, (Int4) StringLen (ptr));
      }
      MemFree (prot);
    }
    newbsp = BioseqNew ();
    if (newbsp != NULL) {
      newbsp->id = SeqIdParse ("lcl|CdRgnTransl");
      newbsp->repr = Seq_repr_raw;
      newbsp->mol = Seq_mol_aa;
      newbsp->seq_data_type = Seq_code_ncbieaa;
      newbsp->seq_data = bs;
      newbsp->length = BSLen (bs);
      
      if (old_prot == NULL)
      {
  	    /* need to create a new protein sequence */
  	    SeqIdFree (newbsp->id);
  	    newbsp->id = new_prot_id;
  	    new_prot_sep = SeqEntryNew ();
  	    new_prot_sep->choice = 1;
  	    new_prot_sep->data.ptrvalue = newbsp;
  	    parent = GetBestTopParentForData (entityID, featbsp);
  	    if (parent != NULL)
  	    {
  	      AddSeqEntryToSeqEntry (parent, new_prot_sep, TRUE);
  	    }
  	    slp = ValNodeNew (NULL);
  	    if (slp != NULL)
  	    {
  	      slp->choice = SEQLOC_WHOLE;
  	      slp->data.ptrvalue = SeqIdDup (new_prot_id);
  	    }
  	    sfp->product = slp;
  	    
  	    /* create full length protein feature */
        prp = ProtRefNew ();
        prot_sfp = CreateNewFeature (new_prot_sep, NULL, SEQFEAT_PROT, NULL);
        if (prot_sfp != NULL) {
          prot_sfp->data.value.ptrvalue = (Pointer) prp;
        }
      }
      else
      {
        /* propagate features to new protein */
        if (!SeqEdFixProteinFeatures (old_prot, newbsp, TRUE))
        {
          Message (MSG_ERROR, "Unable to construct alignment between old and new "
                  "proteins - you will need to adjust the protein features "
                  "manually.");
        }
             	
      	/* then replace old protein with new */
      	seq_data_type = old_prot->seq_data_type;
      	bs = old_prot->seq_data;
      	old_length = old_prot->length;
      	old_prot->seq_data_type = newbsp->seq_data_type;
      	old_prot->seq_data = newbsp->seq_data;
      	old_prot->length = newbsp->length;
      	newbsp->seq_data_type = seq_data_type;
      	newbsp->seq_data = bs;
      	newbsp->length = old_length;
      	BioseqFree (newbsp);
      }
    }
  }
}


#define SEQ_ED_TRANSLATE_BUFFER_SIZE 32000
NLM_EXTERN SeqEdJournalPtr SeqEdJournalNewTranslate
(SeqFeatPtr sfp,
 BioseqPtr  bsp,
 Uint2      entityID)
{
  SeqEdJournalPtr sejp;
  BioseqPtr       product_bsp;
  SeqEntryPtr     sep;
  AsnIoMemPtr     aimp;
  
  if (sfp == NULL) return NULL;
  sejp = (SeqEdJournalPtr) MemNew (sizeof (SeqEdJournalData));
  if (sejp == NULL) return NULL;
  sejp->action = eSeqEdTranslate;
  sejp->offset = 0;
  sejp->spliteditmode = FALSE;
  sejp->sfp = sfp;
  sejp->slp = NULL;
  sejp->bsp = bsp;
  sejp->moltype = 0;
  sejp->entityID = entityID;
  sejp->prev = NULL;
  sejp->next = NULL;
  sejp->char_data = MemNew (SEQ_ED_TRANSLATE_BUFFER_SIZE); 
  if (sejp->char_data == NULL)
  {
    sejp = MemFree (sejp);
    return sejp;
  }
  sejp->num_chars = 0;
  
  product_bsp = BioseqFindFromSeqLoc (sfp->product);
  if (product_bsp != NULL)
  {
    sep = product_bsp->seqentry;
    aimp = AsnIoMemOpen("wb", (BytePtr)sejp->char_data, SEQ_ED_TRANSLATE_BUFFER_SIZE);	/* open to write asn1 to it */
    SeqEntryAsnWrite(sep, aimp->aip, NULL);	/* write it */
    AsnIoFlush(aimp->aip);					/* flush it */
    sejp->num_chars = aimp->count;					/* record how many bytes in it */
    AsnIoMemClose(aimp);					/* close it */
    if (sejp->num_chars == SEQ_ED_TRANSLATE_BUFFER_SIZE)
    {
      /* unable to write record */
      sejp->char_data = MemFree (sejp->char_data);
      sejp = MemFree (sejp);
    }
  }
    
  return sejp;	  
}


static Boolean UnplayOneTranslationJournalEntry (SeqEdJournalPtr sejp)
{
  BioseqPtr   product_bsp;
  SeqEntryPtr current_sep = NULL, prev_sep = NULL;
  SeqEntryPtr top_sep;
  AsnIoMemPtr aimp;
  SeqLocPtr   product_slp = NULL;
  
  if (sejp == NULL || sejp->action != eSeqEdTranslate 
      || sejp->sfp == NULL || sejp->sfp->idx.subtype != FEATDEF_CDS)
  {
    return FALSE;
  }

  product_bsp = BioseqFindFromSeqLoc (sejp->sfp->product);
  if (product_bsp != NULL)
  {
    current_sep = product_bsp->seqentry;    
  }
  
  if (sejp->num_chars > 0)
  {
	  aimp = AsnIoMemOpen("rb", (BytePtr)sejp->char_data, sejp->num_chars);	/* open to read from buffer */
	  prev_sep = SeqEntryAsnRead(aimp->aip, NULL);	/* read it */
	  AsnIoMemClose(aimp);					/* close it */    
	  product_bsp = (BioseqPtr) prev_sep->data.ptrvalue;
	  product_slp = ValNodeNew (NULL);
    if (product_slp != NULL)
    {
      product_slp->choice = SEQLOC_WHOLE;
      product_slp->data.ptrvalue = SeqIdDup (SeqIdFindBest (product_bsp->id, 0));
    }
  }
  if (current_sep == NULL)
  {
    sejp->num_chars = 0;
  }
  else
  {
    aimp = AsnIoMemOpen("wb", (BytePtr)sejp->char_data, SEQ_ED_TRANSLATE_BUFFER_SIZE);	/* open to write asn1 to it */
    SeqEntryAsnWrite(current_sep, aimp->aip, NULL);	/* write it */
    AsnIoFlush(aimp->aip);					/* flush it */
    sejp->num_chars = aimp->count;	/* record how many bytes in it */
    AsnIoMemClose(aimp);					  /* close it */
  }
	
	if (current_sep == NULL && prev_sep == NULL)
	{
	  /* do nothing */
	}
	else if (current_sep == NULL)
	{
	  /* add product to feature where there wasn't one before */
	  AddSeqEntryToSeqEntry (sejp->bsp->seqentry, prev_sep, TRUE);
	}
	else if (prev_sep == NULL)
	{
	  /* remove product from feature */
	  top_sep = GetTopSeqEntryForEntityID (sejp->entityID);
	  RemoveSeqEntryFromSeqEntry (top_sep, current_sep, TRUE);
	}
	else
	{
	  /* replace product sequence */
    ReplaceSeqEntryWithSeqEntry (current_sep, prev_sep, TRUE);
	}
	sejp->sfp->product = product_slp;
	return TRUE;
}

static Boolean PlayOneTranslationJournalEntry (SeqEdJournalPtr sejp)
{
  BioseqPtr   product_bsp;
	SeqEntryPtr current_sep;
	AsnIoMemPtr aimp;

  if (sejp == NULL || sejp->action != eSeqEdTranslate || sejp->sfp == NULL
      || sejp->sfp->idx.subtype != FEATDEF_CDS)
  {
    return FALSE;
  }

  product_bsp = BioseqFindFromSeqLoc (sejp->sfp->product);
  if (product_bsp == NULL)
  {
    sejp->num_chars = 0;
  }
  else
  {
    current_sep = product_bsp->seqentry;
    aimp = AsnIoMemOpen("wb", (BytePtr)sejp->char_data, SEQ_ED_TRANSLATE_BUFFER_SIZE);	/* open to write asn1 to it */
    SeqEntryAsnWrite(current_sep, aimp->aip, NULL);	/* write it */
    AsnIoFlush(aimp->aip);					/* flush it */
    sejp->num_chars = aimp->count;	/* record how many bytes in it */
    AsnIoMemClose(aimp);					  /* close it */    
  }
  	
  SeqEdTranslateOneCDS (sejp->sfp, sejp->bsp, sejp->entityID);	
  return TRUE;
}

static Boolean PlayOneJournalEntry (SeqEdJournalPtr sejp, BoolPtr pfeats_deleted)
{
  Boolean    rval = FALSE;
  SeqLocPtr  slp;
  SeqPortPtr spp;
  
  if (sejp == NULL)
    return rval;	
  switch (sejp->action)
  {
  	case eSeqEdInsert:
    case eSeqEdInsertGap:
      rval = SeqEdInsert (sejp);
  	  break;
  	case eSeqEdDelete:
    case eSeqEdDeleteGap:
  	  slp = SeqLocIntNew (sejp->offset, sejp->offset + sejp->num_chars - 1,
  	                      Seq_strand_plus, sejp->bsp->id);
      spp = SeqPortNewByLoc (slp, Seq_code_iupacna);
      SeqPortRead  (spp, (Uint1Ptr)sejp->char_data, sejp->num_chars + 1);
      SeqPortFree  (spp);
	    SeqEdDeleteFromBsp (sejp, pfeats_deleted);
      SeqLocFree (slp);
      rval = TRUE;
  	  break;
  	case eSeqEdFeatMove:
  	  slp = sejp->sfp->location;
  	  sejp->sfp->location = sejp->slp;
  	  sejp->slp = slp;
  	  SeqEdReindexFeature (sejp->sfp, sejp->bsp);
  	  rval = TRUE;
  	  break;
    case eSeqEdTranslate:
      rval = PlayOneTranslationJournalEntry (sejp);
      break;
  	case eSeqEdJournalEnd:
  	case eSeqEdJournalStart:
  	  rval = TRUE;
  	  break;
  }
  return rval;
}

NLM_EXTERN Boolean PlayJournal 
(SeqEdJournalPtr      list, 
 SeqEdJournalPtr PNTR last, 
 Int4                 num_steps,
 BoolPtr              pfeats_deleted)
{
  Boolean rval = TRUE;
  Int4 steps_removed = 0;
  
  while (list != NULL && list->action != eSeqEdJournalEnd && rval
         && (num_steps == -1 || steps_removed < num_steps))
  {
    if (list->action != eSeqEdJournalStart)
    {
      rval = PlayOneJournalEntry (list, pfeats_deleted);
      steps_removed ++;    	
    }
    *last = list;
    list = list->next;
  }
  return rval;
}

static Boolean UnplayOneJournalEntry (SeqEdJournalPtr sejp)
{
  Boolean    rval = FALSE;
  SeqLocPtr  slp;
  SeqPortPtr spp;
  
  if (sejp == NULL)
    return FALSE;	
  switch (sejp->action)
  {
  	case eSeqEdDelete:
    case eSeqEdDeleteGap:
      rval = SeqEdInsert (sejp);
  	  break;
  	case eSeqEdInsert:
    case eSeqEdInsertGap:
  	  slp = SeqLocIntNew (sejp->offset, sejp->offset + sejp->num_chars - 1,
  	                      Seq_strand_plus, sejp->bsp->id);
      spp = SeqPortNewByLoc (slp, Seq_code_iupacna);
      SeqPortRead  (spp, (Uint1Ptr)sejp->char_data, sejp->num_chars);
      SeqPortFree  (spp);
	    SeqEdDeleteFromBsp (sejp, NULL);
      SeqLocFree (slp);
      rval = TRUE;
  	  break;
  	case eSeqEdFeatMove:
  	  slp = sejp->sfp->location;
  	  sejp->sfp->location = sejp->slp;
  	  sejp->slp = slp;
  	  SeqEdReindexFeature (sejp->sfp, sejp->bsp);
  	  rval = TRUE;
  	  break;
    case eSeqEdTranslate:
      rval = UnplayOneTranslationJournalEntry (sejp);
      break;
  	case eSeqEdJournalEnd:
  	case eSeqEdJournalStart:
  	  rval = TRUE;
  	  break;
  } 
  return rval; 
}

NLM_EXTERN Boolean UnplayJournal (SeqEdJournalPtr PNTR last, Int4 num_steps)
{
  Int4            steps_removed = 0;
  Boolean         rval = TRUE;
  
  if (last == NULL || *last == NULL) return FALSE;
  while (*last != NULL && (*last)->action != eSeqEdJournalStart 
         && rval && (num_steps == -1 || steps_removed < num_steps))
  {
  	rval = UnplayOneJournalEntry (*last);
  	if (rval)
  	{
  	  if ((*last)->action != eSeqEdJournalEnd)
  	  {
  	    steps_removed ++;  	  	
  	  }
  	  *last = (*last)->prev;
  	}
  }
  return rval;
}


static SeqAlignPtr LIBCALLBACK GetSeqAlign (BioseqPtr bsp1, BioseqPtr bsp2)
{
   BLAST_SummaryOptions *options = NULL;
   SeqAlignPtr           salp = NULL;
   
   if (bsp1 == NULL || bsp2 == NULL) return NULL;

   BLAST_SummaryOptionsInit(&options);
   if (bsp1->length > 10000 || bsp2->length > 10000)
   {
      options->filter_string = StringSave ("m L");
      options->word_size = 20;
      options->cutoff_evalue = act_get_eval (60);
      options->hint = eNone;
   }
   else
   {
    options->filter_string = StringSave ("m F");
   }
   if (ISA_na (bsp1->mol))
   {
     options->program = eBlastn;
   }
   else
   {
     options->program = eBlastp;
   }

   BLAST_TwoSequencesSearch(options, bsp1, bsp2, &salp);
   BLAST_SummaryOptionsFree(options);
   return salp;
}

NLM_EXTERN SeqAlignPtr Sequin_GlobalAlign2Seq (BioseqPtr bsp1, BioseqPtr bsp2, BoolPtr revcomp)
{
   return Sqn_GlobalAlign2SeqEx (bsp1, bsp2, revcomp, GetSeqAlign);
}
