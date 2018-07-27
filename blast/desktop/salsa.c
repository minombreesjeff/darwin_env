/* ===========================================================================
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
* File Name:  salsa.c
*
* Author:  Colombe Chappey
*
* Version Creation Date:   1/27/96
*
* $Revision: 6.154 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/
#include <salsa.h>
#include <saledit.h>
#include <salutil.h>
#include <salsap.h>
#include <salstruc.h>
#include <salpanel.h>
#include <saled.h>
#include <salparam.h>
#include <salfiles.h>
#include <salprop.h>
#include <salmedia.h>
#include <dlogutil.h>
#include <sqnutils.h>
#include <seqmgr.h>
#include <fstyle.h>
#include <satutil.h>
#include <picture.h>
#include <viewer.h>
#include <drawseq.h>
#include <vsm.h>
#include <bspview.h>
#include <alignval.h>
#include <salpacc.h>
#include <salpedit.h>
#include <salptool.h>
#include <alignmgr.h>

/******/
#include <pgppop.h>
/****/

#define OBJ_VIRT 254
#define MARGINLEFT15 16
#define MARGINLEFT25 22

static SelEdStructPtr seq_info=NULL;

static EditAlignDataPtr SeqAlignToEditAlignData (EditAlignDataPtr adp, SeqAlignPtr salp, Int4 start, SeqIdPtr mastersip, Uint1 showfeature);
static void CleanupAlignDataPanel (GraphiC g, VoidPtr data);
static void CloseWindowProc (PaneL pnl);


static Boolean AmIConfiguredForTheNetwork (void)

{
  SeqEditViewProcsPtr  svpp;

  svpp = (SeqEditViewProcsPtr) GetAppProperty ("SeqEditDisplayForm");
  if (svpp != NULL)
     if(svpp->download != NULL) return TRUE;
  return FALSE;
}

static Boolean seqentry_write (SeqEntryPtr sep, CharPtr path)
{
  Char         name[PATH_MAX];
  AsnIoPtr     aip;
  AsnTypePtr   atp;
  AsnModulePtr amp;

  if ( sep == NULL ) {
    return 0;
  }
  if (path == NULL )
  {
     if (! GetInputFileName (name, PATH_MAX,"","TEXT"))  {
        return 0;
     }
     path = name;
  }
  amp = AsnAllModPtr ();
  atp = AsnTypeFind (amp,"SeqEntry");
  if ((aip = AsnIoOpen (path,"w")) == NULL) {
    return 0;
  }
  SeqEntryAsnWrite ( sep, aip, atp ) ; 
  aip = AsnIoClose (aip);
  return 1;
}

static void get_client_rect (PaneL p, RectPtr prc)
{
  ObjectRect (p, prc);
  InsetRect (prc, HRZ_BORDER_WIDTH, VER_BORDER_WIDTH);
}

static Int2 getsize_seqids (ValNodePtr sloc, Uint2 printid)
{
  ValNodePtr  vnp;
  SeqLocPtr   slp;
  SeqIdPtr    sip;
  Char        str [52];
  Int4        lens, 
              max = 15;

  if (printid < 1)
     return 15;

  for (vnp=sloc; vnp!=NULL; vnp=vnp->next) 
  {
     slp = (SeqLocPtr) vnp->data.ptrvalue;
     if (slp!=NULL) {
        sip = SeqLocId (slp);
        if (sip!=NULL) {
           SeqIdWrite (sip, str, printid, 50);
           lens = (Int4)StringLen (str);
           if (lens > max)
              max = lens;
        }
     }
  }
  return max;
}

/**************************************************
*** update_sequenceeditormenu
***************************************************/
static void seqeditor2aligneditor (SeqEditViewFormPtr wdp)
{
   Enable (wdp->delitem);
   Enable (wdp->conscolor);
   Disable (wdp->importalg);
   Enable (wdp->expfsagitem);
   Enable (wdp->expalgitem);
   Enable (wdp->expasnitem);
 
   Enable (wdp->menu_align);
   Enable (wdp->selmaster);
   Enable (wdp->selall);
   Enable (wdp->selsubs);
   Enable (wdp->showdifitem);
   Enable (wdp->showallitem);
   Enable (wdp->dotmat);
   Enable (wdp->alreport);
   Disable (wdp->tmpcdspitem);
   Disable (wdp->tmpcdsnitem);
}

static void aligneditor2seqeditor (SeqEditViewFormPtr wdp)
{
   Disable (wdp->delitem);
   Disable (wdp->conscolor);
   Enable (wdp->importalg);
   Disable (wdp->expfsagitem);
   Disable (wdp->expalgitem);
   Disable (wdp->expasnitem);

   Disable (wdp->menu_align);
   Disable (wdp->selmaster);
   Disable (wdp->selall);
   Disable (wdp->selsubs);
   Disable (wdp->showdifitem);
   Disable (wdp->showallitem);
   Disable (wdp->dotmat);
   Disable (wdp->alreport);
   Enable (wdp->tmpcdspitem);
   Enable (wdp->tmpcdsnitem);
}

static void editor2viewer (SeqEditViewFormPtr wdp)
{
  Disable (wdp->undoitem);
  Disable (wdp->pasteitem);
  Disable (wdp->cutitem);
  Disable (wdp->copyitem);
  Disable (wdp->viewmodeitem);
  Enable  (wdp->editmodeitem);
}

static void viewer2editor (SeqEditViewFormPtr wdp)
{
  Int2 k;

  Enable (wdp->undoitem);
  Enable (wdp->pasteitem);
  Enable (wdp->viewmodeitem);
  Disable(wdp->editmodeitem);

  k = checkOMss_for_itemtype (OBJ_BIOSEQ);
  if (k > 0) {
     Enable (wdp->cutitem);
     Enable (wdp->copyitem);
  }
  else {
     Disable (wdp->cutitem);
     Disable (wdp->copyitem);
  }

}

static void update_sequenceeditormenu (SeqEditViewFormPtr wdp, EditAlignDataPtr adp)
{
  ResetClip ();
  if (adp->seqnumber == 1)
     aligneditor2seqeditor (wdp);
  else  
  {
     seqeditor2aligneditor (wdp);
     if (adp->charmode)
        Disable (wdp->showdifitem);
     else 
        Disable (wdp->showallitem);
  }

  if (adp->edit_mode == SEQ_EDIT)
     viewer2editor (wdp);
  else 
     editor2viewer (wdp);

  if (ISA_na (adp->mol_type)) 
  {
     Enable (wdp->showfeatbt);
     Enable (wdp->translatebt);
     Disable (wdp->savefeatbt);
     Enable (wdp->closebt);
     Enable (wdp->svclosebt);
  }
/***** TEMPO **/
  Disable (wdp->conscolor);
  if (ISA_aa (adp->mol_type)) 
  {
     Disable (wdp->alreport);
     if (adp->edit_mode==SEQ_VIEW) {
        Disable (wdp->viewmodeitem);
        Disable (wdp->editmodeitem);
     }
  }
 /*****************/
}

/**************************************************
***  AlignDataInit
***************************************************/
static EditAlignDataPtr AdpOptionsInit (EditAlignDataPtr adp, Int2 marginleft, Uint1 columnpcell, FonT font, Uint1 display_panel)
{
  Uint2  j;
  Uint4  carColor;

  adp->marginleft = marginleft;
  adp->intersalpwidth = 15;
  adp->interline = 0;

  if (font == NULL) {
#ifdef WIN_MAC
  font = ParseFont ("Monaco, 9");
#endif
#ifdef WIN_MSWIN
  font = ParseFont ("Courier, 9");
#endif
#ifdef WIN_MOTIF
  font = ParseFont ("fixed, 12");
#endif
  }
  adp->font = (Handle)(font);
  SelectFont (font);
  adp->charw   = CharWidth ('0');
  adp->ascent  = Ascent ();
  adp->leading = Leading ();
  adp->lineheight   = LineHeight () + adp->interline + 1;
  adp->scaleheight  = 2 * adp->lineheight;
  adp->margin.left  = adp->marginleft * adp->charw;
  adp->margin.right = EDIT_MARGIN_RIGHT;
  adp->margin.bottom= EDIT_MARGIN_BOT;
  SelectFont (systemFont);

  carColor = GetColorRGB(255, 255, 255);
  for (j=0; j<256; j++) adp->colorRefs[j] = carColor;
  adp->colorRefs[COLOR_SCALE] = GetColorRGB(255,0,255);
  adp->colorRefs[COLOR_ID] = carColor;
  adp->colorRefs[COLOR_ID_MASTER] = GetColorRGB(255, 128, 0);
  adp->colorRefs[COLOR_CDS] = GetColorRGB(0, 255, 0);
  adp->colorRefs[COLOR_SELECT] = GetColorRGB(0, 0, 0);
  adp->colorRefs[COLOR_GAP] = GetColorRGB(0, 0, 0);
  adp->colorRefs[COLOR_STAR] = GetColorRGB (255, 0, 0);
/*
  adp->colorRefs[(Uint1) ('M' - '*')] = GetColorRGB (255, 128, 0);
*/
  adp->popcolor[0] = adp->popcolor[1] = adp->popcolor[2] = adp->popcolor[3] = 1;  
  adp->popcolor[4] = adp->popcolor[5] = 6;
  adp->popcolor[6] = adp->popcolor[7] = 4;
  adp->popcolor[8] = adp->popcolor[9] = 3;

  adp->marginwithindex = FALSE;
  adp->marginwithpos = TRUE;
  adp->marginwithIds = FALSE;
  adp->marginwithfeatid = TRUE;
  adp->marginwithgroup = FALSE;

  adp->styleNum = GetMuskCurrentSt();
  adp->newstyle = NULL;
  adp->showfeat = FALSE;
  adp->drawrfp = adp->drawrfm = adp->drawcomplt = FALSE;

  adp->draw_scale = FALSE; 
  adp->draw_bars = FALSE; 
  adp->nscaleline = 0;
  if ( adp->draw_scale ) adp->nscaleline ++;
  if ( adp->draw_bars )  adp->nscaleline ++;
  adp->columnpcell = columnpcell; 
  adp->rowpcell = 0;
  adp->displaytype = TRUE;
  adp->charmode = FALSE;
  adp->edit_pos = -1;
  adp->feat_pos = -1;
  adp->click_feat = 0;
  adp->clickwhat = 0;
  adp->mouse_mode = 0;

  adp->align_format = SALSA_FASTA;

  adp->printid = 0;
  adp->prot_mode = ALLPROTAA;
  adp->stoptransl = 0;
  adp->spliteditmode = FALSE;
  adp->gap_choice = IGNORE_GAP_CHOICE;

  adp->edit_mode = SEQ_EDIT;
  adp->display_panel = display_panel;
  adp->all_sequences = FALSE;
  adp->draw_emptyline = TRUE;
  adp->tmpfile [0] = '\0';

  return adp;
}

static EditAlignDataPtr AdpFieldsInit (EditAlignDataPtr adp, Int2 width, Int2 height)
{
  Int2         x, y;

  adp->seqnumber = 0;
  adp->length = 0;
  adp->seg_bioseq = FALSE;
  adp->master.entityID = 0; 
  adp->master.itemID = 0;  
  adp->master.itemtype = 0;
  adp->master.region = NULL;
  adp->master.regiontype = 0;

  adp->seq_info = NULL;
  adp->sqloc_list = NULL;
  adp->bsqloc_list = NULL;
  adp->size_labels = 15;

  adp->sap_align = NULL;
  adp->sap_original = NULL;
  adp->sap1_original= NULL;
  adp->blocks = NULL;

  adp->dirty = FALSE;

  adp->anp_list = NULL;
  adp->anp_graph = NULL;
  MemSet((Pointer)(adp->featOrder), 0, (size_t)(FEATDEF_ANY*sizeof(Uint1)));
  MemSet((Pointer)(adp->groupOrder),0, (size_t)(FEATDEF_ANY*sizeof(Uint1)));

  adp->voffset = 0;
  adp->hoffset = 0;

  adp->pnlLine = height - adp->margin.bottom/adp->lineheight;
  adp->pnlWidth= width - adp->margin.right/adp->charw;

  y = 0; x = 0;
  if (adp->columnpcell > 0) {
     y = (Int2) (adp->pnlWidth -adp->marginleft) / (Int2) adp->columnpcell;
     x = (Int2) (adp->pnlWidth -adp->marginleft -y) % (Int2)(adp->columnpcell);
     if (x == 9)  
        x = -1; 
  }
  adp->visibleWidth  = (Int2) (adp->pnlWidth -adp->marginleft -y -x);

  adp->vscrollbar_mode = TRUE;
  adp->vPage = adp->pnlLine - 1;  
  adp->hPage = adp->visibleWidth - 1; 
  adp->curalignline = 0;
  adp->nlines = 0;
  adp->numberalignline = 0;
  adp->item_id = NULL;
  adp->seqEntity_id = NULL;
  adp->alignline = NULL;
  adp->itemtype =NULL;
  adp->itemsubtype =NULL;
  
  adp->gr.left = 0;
  adp->gr.right = 0;

  adp->edit_item.entityID = 0;
  adp->edit_item.itemID = 0;
  adp->edit_item.itemtype = 0;
  adp->firstssp = NULL;
  adp->buffer = NULL;
  adp->linebuff = NULL;
  adp->buffertail = NULL;
  adp->bufferlength= 0;
  adp->minbufferlength= 0;
  adp->bufferstart = 0; 
  adp->editbuffer  = TMP_EDITBUFFER;
  adp->colonne = NULL;
  adp->select_block = NULL;

  adp->caret.entityID= adp->master.entityID;
  adp->caret.itemID = adp->master.itemID;
  adp->caret.itemtype= adp->master.itemtype;
  adp->caret.regiontype = 0;
  adp->caret.region = NULL;
  adp->caret_line= 0;
  adp->feat_line= 0;
  adp->caret_orig= 0;

  adp->params = NULL;

  adp->feat= NULL;
  adp->seqfeat= NULL;
  adp->nfeat= 0;

  adp->mol_type = Seq_mol_na;

  adp->current_pattern = NULL;
  adp->match_pat = NULL;
  adp->cur_pat = NULL;
  adp->ngroup = 0;
  adp->extra_data = NULL;
  return adp;
}


static EditAlignDataPtr EditAlignDataInit (SelStructPtr master, Int2 width, Int2 height, Int2 marginleft, Int2 columnpcell, Handle font, Boolean size_pixel, Uint1 display_panel)
{
  EditAlignDataPtr adp;

  adp = (EditAlignDataPtr) MemNew (sizeof(EditAlignData));
  AdpOptionsInit (adp, marginleft, columnpcell, (FonT)font, display_panel);
  if (size_pixel)
     adp =AdpFieldsInit(adp, width/adp->charw, height/adp->lineheight);
  else 
     adp =AdpFieldsInit(adp, width, height);
  return adp;
}

/******************************
***  Proc FreeEditAlignData
*******************************/
static EditAlignDataPtr AdpFieldsFree (EditAlignDataPtr adp)
{
  ValNodePtr       vnp;
  SeqParamPtr      prm;

  FreeAlignNode (adp->anp_list);
  adp->anp_list = NULL; 
  if (adp->anp_graph!=NULL)
     FreeAlignNode (adp->anp_graph);
  adp->anp_graph = NULL; 

  adp->seq_info = SelEdStructListDel (adp->seq_info);
  adp->sqloc_list = ValNodeFreeType (&(adp->sqloc_list), TypeSeqLoc);
  adp->bsqloc_list = ValNodeFreeType (&(adp->bsqloc_list), TypeSeqLoc);

  adp->size_labels = 15;

  if (adp->master.region != NULL) 
        adp->master.region = SeqLocFree ((SeqLocPtr) adp->master.region);
  
  adp->sap_align = CompSeqAnnotFree (adp->sap_align);
  adp->sap_original = NULL;
  adp->sap1_original= NULL;
  adp->blocks = NULL;
 
  adp->firstssp = NULL;
  adp->lastses  = NULL;
  
  MemFree (adp->item_id);
  MemFree (adp->seqEntity_id);
  MemFree (adp->itemtype);
  MemFree (adp->itemsubtype); 
  MemFree (adp->alignline);
  adp->item_id=NULL;
  adp->seqEntity_id=NULL;
  adp->itemtype=NULL;
  adp->itemsubtype=NULL;
  adp->alignline=NULL;

  adp->buffer = BufferFree (adp->buffer);
  adp->buffertail = NULL;

  adp->linebuff = FreeTextAlignList(adp->linebuff);
  MemFree (adp->colonne);
  adp->colonne = NULL;
  adp->newstyle = NULL;
  
  adp->select_block = NULL;
  if (adp->caret.region != NULL)
        adp->caret.region = SeqLocFree ((SeqLocPtr) adp->caret.region);
  
  adp->feat = NULL;
  adp->seqfeat = NULL;

  for (vnp = adp->params; vnp != NULL; vnp = vnp->next) {
        prm = (SeqParamPtr) vnp->data.ptrvalue;
        if (prm != NULL) MemFree (prm);
        vnp->data.ptrvalue = NULL;
  }
  adp->params = ValNodeFree (adp->params);
 
  if (adp->current_pattern != NULL)
     MemFree (adp->current_pattern);
  adp->current_pattern = NULL;
  if (adp->match_pat != NULL) 
     SelStructDelList (adp->match_pat);
  adp->match_pat = NULL;
  adp->cur_pat = NULL;

  adp->extra_data = NULL;
  return adp;
}

static EditAlignDataPtr EditAlignDataFree (EditAlignDataPtr adp)
{
  if (adp!=NULL) {
     adp = AdpFieldsFree (adp);
     MemFree (adp);
     adp=NULL;
  }
  return NULL;
}

/*********************************************************
***
***  ResizeAlignDataWindow: 
***            CallBack for DocumentWindow
**********************************************************/
static void ResizeAlignDataWindow (WindoW w)
{
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr   adp;
  WindoW             temport;

  wdp = (SeqEditViewFormPtr) GetObjectExtra (w);
  if (wdp == NULL) return;
  temport = SavePort(w);
  Select (wdp->pnl);
  GetPanelExtra (wdp->pnl, &adp);
  if ( adp == NULL ) return;
  do_resize_window (wdp->pnl, adp, FALSE);
  inval_panel (wdp->pnl, -1, -1);
  RestorePort (temport);
  Update ();
  return;
}

/*********************************************************
***
***   RESET FUNCTIONS
***    update sequence length
***
**********************************************************/
typedef struct updatesegstruc {
  BioseqSetPtr      parts;
  BioseqPtr         segseq;
  BioseqSetPtr      segset;
} UpdateSegStruc, PNTR UpdateSegStrucPtr;

static void AddBspToSegSet (BioseqPtr segseq, BioseqPtr bsp)

{
  SeqIdPtr   sip;
  SeqLocPtr  slp;

  if (segseq == NULL) return;
  slp = ValNodeNew ((ValNodePtr) segseq->seq_ext);
  if (slp == NULL) return;
  if (segseq->seq_ext == NULL) {
    segseq->seq_ext = (Pointer) slp;
  }
  if (bsp != NULL && bsp->length > 0) {
    segseq->length += bsp->length;
    slp->choice = SEQLOC_WHOLE;
    sip = SeqIdFindBest (bsp->id, 0);
    slp->data.ptrvalue = (Pointer) SeqIdDup (sip);
  } else {
    slp->choice = SEQLOC_NULL;
  }
}


static void DoUpdateSegSet (BioseqPtr segseq, BioseqSetPtr parts)
 
{
  BioseqPtr    bsp;
  Boolean      notFirst;
  Boolean      nullsBetween;
  SeqFeatPtr   sfp;
  SeqFeatPtr   sfpnext;
  SeqLocPtr    slp;
  SeqEntryPtr  tmp;

  if (segseq == NULL || parts == NULL || parts->seq_set == NULL) return;
  tmp = parts->seq_set;
  notFirst = FALSE;
  nullsBetween = FALSE;
  segseq->length = 0;
  switch (segseq->seq_ext_type) {
    case 1:    /* seg-ext */
      slp = (ValNodePtr) segseq->seq_ext;
      while (slp != NULL) {
        if (slp->choice == SEQLOC_NULL) {
          nullsBetween = TRUE;
        }
        slp = slp->next;
      }  
      SeqLocSetFree ((ValNodePtr) segseq->seq_ext);
      break;
    case 2:   /* reference */
      SeqLocFree ((ValNodePtr) segseq->seq_ext);
      break;
    case 3:   /* map */
      sfp = (SeqFeatPtr) segseq->seq_ext;
      while (sfp != NULL) {
        sfpnext = sfp->next;
        SeqFeatFree (sfp);
        sfp = sfpnext;
      }
      break;
    default:
      break;
  }
  segseq->seq_ext = NULL;
  while (tmp != NULL) {
    if (nullsBetween && notFirst) {
      AddBspToSegSet (segseq, NULL);
    }
    bsp = (BioseqPtr) tmp->data.ptrvalue;
    if (bsp != NULL) {
      AddBspToSegSet (segseq, bsp);
    }      
    notFirst = TRUE;
    tmp = tmp->next;
  }
}
 
 
static void FindSegSetComponentsCallback (SeqEntryPtr sep, Pointer mydata,
                                          Int4 index, Int2 indent)
 
{
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  UpdateSegStrucPtr  ussp;
 
  if (sep != NULL && sep->data.ptrvalue && mydata != NULL) {
    ussp = (UpdateSegStrucPtr) mydata;
    if (IS_Bioseq(sep)) {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (ISA_na (bsp->mol)) {
        ussp->segseq = bsp;
      }  
    } else if (IS_Bioseq_set(sep)) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      if (bssp->_class == 2) {
        ussp->segset = bssp;
      } else if (bssp->_class == 4) {
        ussp->parts = bssp;
      }  
    }
  }
}
 
static Int4 UpdateSegList (SeqEntryPtr sep, Pointer mydata,
                           SeqEntryFunc mycallback,
                           Int4 index, Int2 indent)

{
  BioseqSetPtr  bssp;

  if (sep == NULL) return index;
  if (mycallback != NULL)
    (*mycallback) (sep, mydata, index, indent);
  index++;
  if (IS_Bioseq (sep)) return index;
  if (Bioseq_set_class (sep) == 4) return index;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  sep = bssp->seq_set;
  indent++;
  while (sep != NULL) {
    index = UpdateSegList (sep, mydata, mycallback, index, indent);
    sep = sep->next;
  }
  return index;
}

#define UpdateSegExplore(a,b,c) UpdateSegList(a, b, c, 0L, 0);

static EditAlignDataPtr EditAlignDataChangeLength (EditAlignDataPtr adp, Int4 cutlength, SelStructPtr ssp)
{
  SeqLocPtr    slp_master;
  SeqIntPtr    sit_master;
  DenseSegPtr  dsp;
  SeqAnnotPtr  sap;
  SeqAlignPtr  salp;
  SeqEntryPtr  sep;
  UpdateSegStruc  uss;
  Int4         start;

  adp->length -= cutlength;

  sap = SeqAnnotBoolSegToDenseSeg (adp->sap_align);
  salp = (SeqAlignPtr) sap->data;
  dsp = (DenseSegPtr) salp->segs;
  *(dsp->lens) -= cutlength;
  CompSeqAnnotFree (adp->sap_align);
  adp->sap_align = SeqAnnotDenseSegToBoolSeg (sap);

  adp->sqloc_list = ValNodeFreeType (&(adp->sqloc_list), TypeSeqLoc);
  adp->bsqloc_list= ValNodeFreeType (&(adp->bsqloc_list), TypeSeqLoc); 
  adp->sqloc_list = SeqLocListFromSeqAlign (salp); 
  adp->bsqloc_list = SeqLocListOfBioseqsFromSeqAlign (salp);

  slp_master = (SeqLocPtr) adp->master.region;
  sit_master = (SeqIntPtr) slp_master->data.ptrvalue;
  sit_master->to -= cutlength;

  FreeAlignNode(adp->anp_list);
  if (adp->display_panel==0 || adp->display_panel==1 || adp->display_panel==2) 
  {
     adp->anp_list = (ValNodePtr) CollAlignFromSeqAnnot (sap, (SeqLocPtr) adp->master.region, adp->featOrder, adp->groupOrder, COLLECT_MD, FALSE, FALSE, FALSE);
  } 
  else if (adp->display_panel == 3) {
     adp->anp_list = (ValNodePtr) CollAlignFromSeqAnnot (sap, (SeqLocPtr) adp->master.region, adp->featOrder, adp->groupOrder, COLLECT_MP, TRUE, FALSE, FALSE);
  }
  if (adp->display_panel==2) {
     adp->anp_graph = (ValNodePtr) CollAlignFromSeqAnnot (sap, (SeqLocPtr)adp->master.region, adp->featOrder, adp->groupOrder, COLLECT_MP, TRUE, FALSE, FALSE);
  }
  sap = SeqAnnotFree (sap);
  clean_annot_for_anp(&(adp->anp_list));
  OrderFeatProc (adp->anp_list);
 
  start = SeqLocStart((SeqLocPtr)ssp->region); 
  setposition_tossp (&(adp->caret), start, start);
  adp->caret_orig = start; 
  adp->feat = update_featpept (adp, adp->feat, NULL, ssp, (-cutlength), 255);
  adp->seqfeat=update_featpept (adp, adp->seqfeat,NULL,ssp,(-cutlength), 255);
 
  adp->bufferlength = MIN((Int4)(adp->length),(Int4) adp->minbufferlength);
  if (abs((adp->bufferstart +adp->bufferlength)-adp->length) < 100)
               adp->bufferlength = adp->length - adp->bufferstart;

  if ( adp->seg_bioseq ) {
     sep = GetBestTopParentForItemID (adp->master.entityID, adp->master.itemID, adp->master.itemtype);
     uss.segseq = NULL;
     uss.parts = NULL;
     uss.segset = NULL;
     UpdateSegExplore (sep, (Pointer) &uss, FindSegSetComponentsCallback);
     if (uss.segseq != NULL && uss.parts != NULL && uss.segset != NULL) {
        DoUpdateSegSet (uss.segseq, uss.parts);
     }
  }
  if (adp->current_pattern != NULL)
     MemFree (adp->current_pattern);
  adp->current_pattern = NULL;
  return adp;
}

static void reset_features (PaneL pnl, EditAlignDataPtr adp)
{
 if (adp->showfeat) {
    Select (pnl);
    HideFeatureFunc (adp);
    adp->showfeat = FALSE;
    ShowFeatureProc (pnl, TRUE);
 }
}

static void reset_features_afterlengthchange(PaneL pnl, EditAlignDataPtr adp) { 
  reset_features (pnl, adp);
}

static void update_select_region (SelStructPtr ommsp, PaneL pnl, EditAlignDataPtr adp, Boolean update_caret)
{
  SeqAlignPtr salp;
  SeqIdPtr    sip;
  SeqLocPtr   slp;
  RecT        rp;
  Int4        from, to;
  Int4        froms, tos;
  Int4        minp, maxp;
  Int2        chklocp, chklocp2;
  float hratio;
   
  get_client_rect (pnl, &rp);
  salp = (SeqAlignPtr) adp->sap_align->data;
  slp = (SeqLocPtr) ommsp->region;
  sip = SeqLocId (slp);
  chklocp = chkloc (sip, SeqLocStart (slp), adp->sqloc_list, &froms);
  from = SeqCoordToAlignCoord (froms, sip, salp, 0, chklocp);
  chklocp2 = chkloc (sip, SeqLocStop (slp), adp->sqloc_list, &tos);
  to = SeqCoordToAlignCoord (tos, sip, salp, 0, chklocp2);

  if(chklocp>-1 && chklocp2>-1) {
     if (to < adp->hoffset || from > adp->hoffset+ adp->visibleLength)  
     {
        adp->voffset = hoffset2voffset(adp, adp->anp_list, adp->visibleWidth, 0, adp->length-1, from);
        ResetClip ();
        data_collect_arrange (adp, TRUE);
        hratio = (float)MAX((Int4)(from-50), (Int4)0) / (float)adp->length;
        SeqEdSetCorrectBarMax (pnl, adp, hratio);
     }
  }
  if (update_caret && (from != SeqLocStart((SeqLocPtr)adp->caret.region))) {
     locate_region (&(adp->caret), from, to, sip, Seq_strand_plus, FALSE);
  }
  if (!adp->display_panel) {
     if (update_caret) {
        if (chklocp2==APPEND_RESIDUE)
           slp = SeqLocIntNew (froms, tos-1, 0, sip);
        else
           slp = SeqLocIntNew (froms, tos, 0, sip);
/************* BEFORE = REPLACE to do*****/
        ommsp->region = slp;
        to_update_prompt (pnl, ommsp, NULL, NULL, TRUE, adp->printid);
     }
     else {
        to_update_prompt (pnl, &(adp->caret), NULL, NULL, TRUE, adp->printid); 
     }
     update_edititem (pnl);
  }
  minp = MIN (from, to)-2;
  maxp = MAX (from, to)+2;
  inval_selstructloc (adp, ommsp->entityID, ommsp->itemID, ommsp->itemtype, 255, &rp, minp, maxp);
  return;
}


static EditAlignDataPtr update_length (PaneL pnl, EditAlignDataPtr adp, Int4 cutlength)
{
  SelStructPtr     ssp;
  RecT             rp;
  SeqLocPtr        slp;
  ValNodePtr       vnp;
  AlignNodePtr     anp;
  SeqAlignPtr      salp;
  Int4             from, 
                   to,
                   select_length;
  Boolean          localssp=FALSE;
  float hratio;
  
  if (adp!=NULL && cutlength != 0) 
  {
     get_client_rect (pnl, &rp);
     slp = (SeqLocPtr)(adp->caret.region);
     ssp = getOMselect_for_itemtype (OBJ_BIOSEQ);
     if (ssp == NULL) {
        localssp = TRUE;
        from = adp->caret_orig - cutlength;
        to = adp->caret_orig;
        for (vnp = adp->anp_list; vnp != NULL; vnp = vnp->next) {
           anp = (AlignNodePtr) vnp->data.ptrvalue;
           if (SeqIdForSameBioseq(anp->sip, SeqLocId(slp))) 
               break;
        }
        ssp = SelStructNew (anp->seq_entityID, anp->bsp_itemID, OBJ_BIOSEQ, from, to, anp->sip, Seq_strand_plus, FALSE);
     }
     else {
        select_length = SeqLocStop((SeqLocPtr)ssp->region) - SeqLocStart((SeqLocPtr)ssp->region) +1;
        if (select_length != cutlength) {
           ErrPostEx (SEV_ERROR, 0, 0, "Selected region differs from deleted sequence's length");
           return adp;
        }
     }
     adp = EditAlignDataChangeLength (adp, cutlength, ssp);
     if (localssp)
        SelStructDel (ssp);
     salp = (SeqAlignPtr) adp->sap_align->data;
     if (!adp->display_panel)
        to_update_prompt (pnl, &(adp->caret), salp, adp->sqloc_list, FALSE, adp->printid);
     data_collect_arrange (adp, TRUE);
     hratio = (float)adp->hoffset / (float)adp->length;
     SeqEdSetCorrectBarMax (pnl, adp, hratio);
  }
  return adp;
}


static SeqAlignPtr create_list_alignedsegs (SeqAlignPtr salp)
{
  SeqAlignPtr   salp2 = NULL;
  DenseDiagPtr  ddp ,
                ddphead=NULL;
  SeqIdPtr      sip = NULL,
                siphead = NULL;
  BioseqPtr     bsp;
  Int2Ptr       tab;
  Int4Ptr       startp;
  Int4          start,
                from = -1,
                lens,
                lensmax;
  Int4          k;
  Int2          dim = 0;

  if (salp!=NULL)
  {
     if (salp->segtype == 1) 
     {
        ddp = (DenseDiagPtr) salp->segs;
        if (ddp!=NULL)
           sip = ddp->id;
     }
  }
  if (sip != NULL) {
     bsp = BioseqLockById (sip);
     if (bsp!=NULL) {
        lensmax = bsp->length;
        BioseqUnlock (bsp);
        tab =  (Int2Ptr)MemNew ((size_t)((lensmax +4)*sizeof(Int2)));
        for (k=0; k<lensmax; k++)
           tab[k]=1;
        siphead = AddSeqId (&siphead, sip);
        dim = 1;
        for (salp2=salp; salp2!=NULL; salp2=salp2->next)
        {
         if (salp2->type > 0) 
         {
           ddp = (DenseDiagPtr) salp2->segs;
           for (; ddp!=NULL; ddp=ddp->next)
           {
              startp = ddp->starts;
              lens=(Int4)ddp->len;
              for (k=*startp; k<*startp+lens && k<lensmax; k++)
              {
                 tab[k]++;
              }
              if (from<0)
                 from = *startp;
              siphead = AddSeqId (&siphead, ddp->id->next);
           }
           dim++;
         }
        }
        k=0;
        while (k<lensmax)
        {
           if (tab[k] == dim)
           {
              start = k;
              k++;
              while (k<lensmax && tab[k] == dim)
                 k++;
              lens = k-start; 
              ddp = DenseDiagCreate (dim, siphead, &start, lens, NULL, NULL);
              DenseDiagLink (&ddphead, ddp);
           } 
           k++;
        }
        MemFree (tab);
     }
  }
  salp2 = NULL;
  if (ddphead != NULL)
  {
     salp2 = SeqAlignNew ();
     salp2->type = 3;
     salp2->segtype = 1; /*COMPSEG;*/
     salp2->dim = 2; 
     salp2->segs = (Pointer) ddphead;
  }
  return salp2;
}
/*********************************************************
***
***   RESET FUNCTIONS
***     Reset data structure 
***
**********************************************************/
static EditAlignDataPtr EditAlignDataReset (EditAlignDataPtr adp, Int2 width, Int2 height, SeqAlignPtr salp)
{
  SeqAlignPtr  newsalp;
  SeqIdPtr     mastersip;
  SeqAlignPtr  blocks = NULL;
  SeqAnnotPtr  sap = NULL;

  if (salp != NULL) {
     newsalp = salp;
  } else {
     newsalp = SeqAlignBoolSegToDenseSeg ((SeqAlignPtr)adp->sap_align->data);
  }
  if (newsalp == NULL)
     return adp;

  mastersip =SeqIdDup (SeqIdFindBest(SeqLocId((SeqLocPtr)adp->master.region), 0));
/*if (adp->blocks!=NULL) {  */  /* yanli comment out, 7/19/1999 */
  if (adp->blocks!=NULL || adp->blocks == NULL) {     
         /* no matter blocks exists or not previously, yanli, 7/19/1999 */
/**
     SeqAlignSetFree (adp->blocks);
**/
     adp->blocks=NULL;
     if (adp->sap1_original)
        blocks=create_list_alignedsegs ((SeqAlignPtr)adp->sap1_original->data);   
 
  }
  if (adp->sap1_original)
  {
     sap = adp->sap1_original;
     adp->sap1_original=NULL;
  }
  adp = AdpFieldsFree (adp);
  adp = AdpFieldsInit (adp, width, height); 
  adp = SeqAlignToEditAlignData (adp, newsalp, adp->hoffset, mastersip, (Uint1)(adp->showfeat));
  if (adp && blocks) {
/*   adp->sap1_original=sap;  */
                    /* yanli comment out, 7/19/1999 */
     adp->blocks = blocks;
  }
  if(adp && sap) adp->sap1_original=sap;  /* yanli added, 7/19/1999 */
 
  return adp;
}

/******************************************
***
***  MESSAGE FUNCTIONS
***
***  SalsaFormMessage 
***       sends update message when the windows is closed
***
***  BioseqEditMsgFunc
***       picks up update messages 
***       if itemtype is OBJ_SEQFEAT: recollect data structure
***
*******************************************/
static void SalsaFormMessage (ForM f, Int2 mssg)
{
  SeqEditViewFormPtr  wdp;

  wdp = (SeqEditViewFormPtr) GetObjectExtra (f);
  if (wdp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        Beep ();
        break;
      default :
        if (wdp->appmessage != NULL) {
          wdp->appmessage (f, mssg);
        }
        break;
    }
  }
}

/*************************************************************
**************************************************************/
static Int2 LIBCALLBACK BioseqEditMsgFunc (OMMsgStructPtr ommsp)
{
  WindoW             currentport,
                     temport;
  OMUserDataPtr      omudp;
  SeqEditViewFormPtr wdp = NULL;
  EditAlignDataPtr   adp;
  SeqAlignPtr        salp,
                     newsalp = NULL;
  SeqAnnotPtr        sap = NULL;
  SelStructPtr       ssptmp;
  ValNodePtr         vnp;
  SeqLocPtr          slp;
  BioseqPtr          bsp;
  SeqIdPtr           sip;
  Int4               cutlength,
                     from=0, 
                     to = 0;
  Int2               width;
  Int2               chklocp;
  RecT               rp;
  Boolean            ok;
  float hratio;
  
  SeqEditViewProcsPtr svpp;

  omudp = (OMUserDataPtr)(ommsp->omuserdata);
  if (omudp == NULL) return OM_MSG_RET_ERROR;
  wdp = (SeqEditViewFormPtr) omudp->userdata.ptrvalue;
  if (wdp == NULL) return OM_MSG_RET_ERROR;
  if ( ( adp = GetAlignDataPanel (wdp->pnl) ) == NULL ) 
         return OM_MSG_RET_ERROR;
  if ( adp->seqnumber == 0 ) 
         return OM_MSG_RET_ERROR;

  currentport = ParentWindow (wdp->pnl);
  temport = SavePort (currentport);
  UseWindow (currentport);
  Select (wdp->pnl);
  get_client_rect (wdp->pnl, &rp);
  width = adp->visibleWidth;
  if (adp->columnpcell > 0)
     width += (Int2) adp->visibleWidth / (Int2) adp->columnpcell;
  salp = (SeqAlignPtr) adp->sap_align->data;
  switch (ommsp->message) 
  {
      case OM_MSG_UPDATE:
          if (! Visible (ParentWindow (wdp->pnl))) return OM_MSG_RET_OK;
          cutlength = 0;
          for (vnp=adp->bsqloc_list; vnp!=NULL; vnp=vnp->next) 
          {
             slp = (SeqLocPtr) vnp->data.ptrvalue;
             bsp = BioseqLockById (SeqLocId(slp));
             if (bsp != NULL) {
                cutlength = (Int4) SeqLocLen (slp) - bsp->length;
                BioseqUnlock (bsp);
                if (cutlength !=  0) break;
             }
          }
          if (cutlength != 0 && adp->input_format == OBJ_BIOSEQ) 
          {
             update_length (wdp->pnl, adp, cutlength);
          }
          if (cutlength != 0 && adp->input_format == OBJ_SEQALIGN) 
          {
             adp = EditAlignDataReset (adp, (rp.right-rp.left), (rp.bottom-rp.top), NULL);
             if (adp == NULL) {
                return  OM_MSG_RET_ERROR;
             }
             do_resize_window (wdp->pnl, adp, TRUE);
          }
          if (cutlength!=0 && (adp->input_format==OBJ_BIOSEQ || adp->input_format==OBJ_SEQALIGN)) 
          {
             Select (wdp->pnl);
             reset_features_afterlengthchange (wdp->pnl, adp);
             if (cutlength < 0)
                inval_selstructpos_tobottom(adp, adp->caret.entityID, adp->caret.itemID, adp->caret.itemtype, &rp, adp->edit_pos);
             else if (cutlength > 0 && adp->caret.region!=NULL) {
                inval_selstructpos_tobottom(adp, adp->caret.entityID, adp->caret.itemID, adp->caret.itemtype, &rp, SeqLocStart((SeqLocPtr)adp->caret.region));
             }
             else 
                inval_panel (wdp->pnl, -1, -1);
             adp->edit_pos = SeqLocStart((SeqLocPtr)adp->caret.region);
          }
          if (cutlength == 0 && ommsp->itemtype == OBJ_BIOSEQ) 
          {
             if (adp->input_format == OBJ_BIOSEQ) 
             {
                if (ommsp->regiontype == OM_REGION_SEQLOC) 
                {
                   sip = SeqLocId ((SeqLocPtr) ommsp->region);
                   from = SeqLocStart ((SeqLocPtr) ommsp->region);
                   from = SeqCoordToAlignCoord(from, sip, salp, 0, 0);
                   to = SeqLocStop ((SeqLocPtr) ommsp->region);
                   chklocp =chkloc(sip, to, adp->sqloc_list, &to);
                   to = SeqCoordToAlignCoord(to, sip, salp, 0, chklocp);
                   inval_selstructloc (adp, ommsp->entityID, ommsp->itemID, ommsp->itemtype, 255, &rp, from, to);
                } 
                else {
                    if (adp->curfeat != NULL) {
                       adp->feat = del_ssp_fromid (adp->feat, (Uint2) SEQFEAT_CDREGION, adp->curfeat);
                       adp->curfeat = NULL;
                    }
                    reset_features (wdp->pnl, adp);
                }
             }
             else if (adp->input_format == OBJ_SEQALIGN) {
                repopulate_panel (currentport, adp, (SeqAlignPtr)adp->sap_original->data);
             }
          }
          else if (cutlength == 0 && ommsp->itemtype == OBJ_VIRT)
          {
             data_collect_arrange (adp, TRUE);
             hratio = (float)adp->hoffset / (float)adp->length;
             SeqEdSetCorrectBarMax (wdp->pnl, adp, hratio);
             reset_features (wdp->pnl, adp);
             inval_panel (wdp->pnl, -1, -1);
          }
          else if (cutlength == 0)
          {
/********
Sequin send ommsp->itemtype==0 when update features 
*********/
             reset_features (wdp->pnl, adp);
             inval_panel (wdp->pnl, -1, -1);
          }
          break;
      case OM_MSG_SETCOLOR:
          if (ommsp->itemtype == OBJ_BIOSEQ)
          {
             svpp = (SeqEditViewProcsPtr) GetAppProperty ("SeqEditDisplayForm");
             if (svpp)
                adp->colorRefs[COLOR_SELECT] = GetColorRGB(svpp->colorR_HL,svpp->colorG_HL,svpp->colorB_HL);
             
             inval_panel (wdp->pnl, -1, -1);
          }
          break;
      case OM_MSG_DESELECT:
          if (ommsp->itemtype == OBJ_BIOSEQ)
          {
             if (ommsp->regiontype == OM_REGION_SEQLOC) {
                if (is_seqvisible(ommsp->entityID, ommsp->itemID, adp->seq_info)) 
                {
                   slp = (SeqLocPtr)ommsp->region;
                   ssptmp = SelStructNew (ommsp->entityID, ommsp->itemID, ommsp->itemtype, SeqLocStart (slp)+1, SeqLocStop (slp), SeqLocId (slp), SeqLocStrand(slp), FALSE);
                   update_select_region (ssptmp, wdp->pnl, adp, FALSE); 
                   SelStructDel (ssptmp);
/************
                   sip = SeqLocId ((SeqLocPtr) ommsp->region);
                   from = SeqLocStart ((SeqLocPtr) ommsp->region);
                   from = SeqCoordToAlignCoord(from, sip, salp, 0, 0);
                   to = SeqLocStop ((SeqLocPtr) ommsp->region);
                   chklocp =chkloc(sip, to, adp->sqloc_list, &to);
                   to = SeqCoordToAlignCoord(to, sip, salp, 0, chklocp);
                   inval_selstructloc (adp, ommsp->entityID, ommsp->itemID, ommsp->itemtype, 255, &rp, from, to);
                   if ( adp->caret.regiontype == OM_REGION_SEQLOC ) {
                      from = SeqLocStart ((SeqLocPtr)adp->caret.region); 
                      if (from != SeqLocStop((SeqLocPtr)adp->caret.region)) {
                         setposition_tossp (&(adp->caret), from, from);
                      }
                   }
***************/
                   if (!adp->display_panel)
                      to_update_prompt (wdp->pnl, &(adp->caret), (SeqAlignPtr) adp->sap_align->data, adp->sqloc_list, FALSE, adp->printid); 
                   if (adp->caret.entityID!=ommsp->entityID && adp->caret.itemID!=ommsp->itemID) {
                      inval_selstructpos(adp,adp->caret.entityID, adp->caret.itemID, adp->caret.itemtype, &rp, from);
                   }
                }
             }
          }
          else if (ommsp->itemtype == OBJ_SEQFEAT) 
          {
             if (ommsp->regiontype == OM_REGION_SEQLOC) {
                sip = SeqLocId ((SeqLocPtr) ommsp->region);
                from = SeqLocStart ((SeqLocPtr) ommsp->region);
                from = SeqCoordToAlignCoord(from, sip, salp, 0, 0);
                to = SeqLocStop ((SeqLocPtr) ommsp->region);
                chklocp =chkloc(sip, to, adp->sqloc_list, &to);
                to = SeqCoordToAlignCoord(to, sip, salp, 0, chklocp);
                inval_selstructloc_forfeat (adp, ommsp->entityID, ommsp->itemID, ommsp->itemtype, 255, &rp, from, to);
             }
          }
          break;

      case OM_MSG_SELECT: 
          if (ommsp->itemtype == OBJ_BIOSEQ) 
          {
             if (ommsp->regiontype == OM_REGION_SEQLOC) {
                if(is_seqvisible(ommsp->entityID, ommsp->itemID, adp->seq_info)) 
                {
                   slp = (SeqLocPtr)ommsp->region;
                   ssptmp = SelStructNew (ommsp->entityID, ommsp->itemID, ommsp->itemtype, SeqLocStart (slp), SeqLocStop (slp), SeqLocId (slp), SeqLocStrand(slp), FALSE);
                   update_select_region (ssptmp, wdp->pnl, adp, FALSE); 
                   SelStructDel (ssptmp);
                }
             } 
             else  {
                ssptmp=is_selectedbyID (ommsp->entityID, ommsp->itemID, ommsp->itemtype);
                if ( ssptmp != NULL ) 
                {
                   if (is_seqvisible(ommsp->entityID, ommsp->itemID, adp->seq_info)) 
                   {
                      slp = (SeqLocPtr)ssptmp->region;
                      update_select_region (ssptmp, wdp->pnl, adp, FALSE); 
                   } 
                   else 
                      inval_selstruct (adp, ommsp->entityID, ommsp->itemID, ommsp->itemtype, 255, &rp, adp->margin.left,(Int2)(width *adp->charw));
                }
             }
          }
          if (ommsp->itemtype == OBJ_SEQFEAT)
          {
           if ( adp->showfeat && adp->seqfeat!=NULL )
           {   
             if (ommsp->region == NULL) {
                ommsp->region = checkseqlocfeature_for_editor (ommsp->entityID, ommsp->itemID, adp->seqfeat);
                if (ommsp->region != NULL) {
                   ommsp->regiontype = OM_REGION_SEQLOC;
                   checkselectsequinfeature_for_editor (adp->seqfeat);
                }
             }
             if (ommsp->regiontype == OM_REGION_SEQLOC) 
             {
                sip = SeqLocId ((SeqLocPtr) ommsp->region);
                from = SeqLocStart ((SeqLocPtr) ommsp->region);
                from = SeqCoordToAlignCoord(from, sip, salp, 0, 0);
                to = SeqLocStop ((SeqLocPtr) ommsp->region);
                chklocp =chkloc(sip, to, adp->sqloc_list, &to);
                to = SeqCoordToAlignCoord(to, sip, salp, 0, chklocp);
/*
                if(to < adp->hoffset || from > adp->hoffset+ adp->visibleLength)
                {
                   adp->voffset = (Int2)(from/(adp->visibleWidth));
                   ResetClip ();
                   data_collect_arrange (adp, TRUE);
                   hratio = (float)adp->hoffset / (float)adp->length;
                   SeqEdSetCorrectBarMax (wdp->pnl, adp, hratio);
                }
*/
                inval_selstructloc_forfeat (adp, ommsp->entityID, ommsp->itemID, ommsp->itemtype, 255, &rp, from, to);
             } else {
                inval_selstruct (adp, ommsp->entityID, ommsp->itemID, ommsp->itemtype, 255, &rp, adp->margin.left,(Int2)(width *adp->charw));
             }
           }
           else {   
             inval_panel (wdp->pnl, -1, -1);
           }
          }
          break;
      case OM_MSG_DEL:
          break;
      case OM_MSG_HIDE:
          bsp = GetBioseqGivenIDs (ommsp->entityID, ommsp->itemID, ommsp->itemtype);
          if (bsp)
          {
             sap = adp->sap_original;
             if (sap)
             {
                newsalp = (SeqAlignPtr)sap->data;
                ok = SeqAlignIDCache (newsalp, SeqIdFindBest (bsp->id, 0));
                if (ok) 
                {
                   if (adp->sap1_original)
                      SeqAlignIDCache ((SeqAlignPtr)adp->sap1_original->data, SeqIdFindBest (bsp->id, 0));
                   set_seqnot_visible (ommsp->entityID, ommsp->itemID, adp->seq_info);
                   repopulate_panel (currentport, adp, newsalp);
                }
             }
          }
          break;
      case OM_MSG_SHOW:
          bsp = GetBioseqGivenIDs (ommsp->entityID, ommsp->itemID, ommsp->itemtype);
          if (bsp)
          {
             sap = adp->sap_original;
             if (sap)
             {
                newsalp = (SeqAlignPtr)sap->data;
                newsalp=SeqAlignIDUncache (newsalp, SeqIdFindBest (bsp->id, 0));
                if (newsalp) 
                {
                   if (adp->sap1_original)
                      SeqAlignIDUncache ((SeqAlignPtr)adp->sap1_original->data, SeqIdFindBest (bsp->id, 0));
                   set_seqvisible (ommsp->entityID, ommsp->itemID, adp->seq_info);
                   repopulate_panel (currentport, adp, newsalp);
                }
             }
          }
          break;
      case OM_MSG_FLUSH:
          CloseWindowProc (wdp->pnl);
          break;
      default:
          break;
  }
  RestorePort (temport);
  UseWindow (temport);
  return OM_MSG_RET_OK;
}

static void checkEntityIDForMsg (SeqEditViewFormPtr wdp)
{
  EditAlignDataPtr adp;
  AlignNodePtr    anp;
  ValNodePtr      vnp;
  OMUserDataPtr   omudp;
  Uint2           eID;
  SeqEntryPtr     sep;

  if ( ( adp = GetAlignDataPanel (wdp->pnl) ) == NULL ) 
         return;
  if ( adp->seqnumber == 0 ) 
         return;
  for (vnp=adp->anp_list; vnp!=NULL; vnp=vnp->next) {
   anp = (AlignNodePtr) vnp->data.ptrvalue;
   if (anp!=NULL) {
      sep = GetTopSeqEntryForEntityID (anp->seq_entityID);
      if (sep!=NULL) {
         eID = SeqMgrGetEntityIDForSeqEntry(sep);
         omudp = ObjMgrGetUserData (eID, wdp->procid, OMPROC_EDIT, wdp->userkey);
         if (omudp == NULL) {
            omudp = ObjMgrAddUserData (eID, wdp->procid, OMPROC_EDIT, wdp->userkey);
            if (omudp != NULL) {
               omudp->userdata.ptrvalue = (Pointer) wdp;
               omudp->messagefunc = BioseqEditMsgFunc;
            }
         }
      }
   }
  }
}



/******************************************
***
***  Repopulate functions 
***
*******************************************/

extern void repopulate_panel (WindoW w, EditAlignDataPtr adp, SeqAlignPtr salp)
{
  WindoW temport;
  SeqEditViewFormPtr wdp;

  if (salp==NULL || adp==NULL)
     return;
  wdp = (SeqEditViewFormPtr) GetObjectExtra (w);
/*** TEMPO ***/
  seq_info = adp->seq_info;
  adp->seq_info = NULL;
/*** TEMPO ***/
  EditAlignDataRepopulateFromSeqAlign (wdp->pnl, adp, salp);
  temport = SavePort(w);
  Select (wdp->pnl);
  inval_panel (wdp->pnl, -1 ,-1);
  RestorePort (temport);
  wdp = (SeqEditViewFormPtr) GetObjectExtra (w);
  if (wdp) {
     update_sequenceeditormenu (wdp, adp);
     checkEntityIDForMsg (wdp);
  } 
  seq_info = NULL;
}

static SelEdStructPtr is_sip_inseqinfo (SeqIdPtr sip, SelEdStructPtr seq_info)
{
  SelEdStructPtr tmp;
  SeqLocPtr      slp;
 
  for (tmp=seq_info; tmp!=NULL; tmp=tmp->next)
  {
     slp=(SeqLocPtr)tmp->region;
     if (SeqIdForSameBioseq (sip, SeqLocId(slp)))
     {
        return tmp;
     }
  }
  return NULL;
}


static void ImportFromFileFunc (WindoW w, Int2 method)
{
  EditAlignDataPtr adp;
  SeqAlignPtr      salp;

  if (w==NULL)
     return;
  if ( ( adp = GetAlignEditData (w) ) == NULL ) 
     return;
  adp->align_format = (Uint1) method;
  salp = ImportFromFile (adp);
  if (salp!=NULL) 
     repopulate_panel (w, adp, salp);
}

static void ImportFromFile1Proc (IteM i)
{
  ImportFromFileFunc ((WindoW)ParentWindow(i), (Int2)PRGALIGNDEFAULT);
}

static void ImportFromFile2Proc (IteM i)
{
  ImportFromFileFunc ((WindoW)ParentWindow(i), (Int2)PRGALIGNBANDBL);
}
/**
static void ImportFromFile3Proc (IteM i)
{
  ImportFromFileFunc ((WindoW)ParentWindow(i), (Int2)PRG_FASTA_IMPORT);
}
**/
static void ImportFromFile4Proc (IteM i)
{
  ImportFromFileFunc ((WindoW)ParentWindow(i), (Int2)PRG_BLAST);
}

static void ImportAlignmentProc (IteM i)
{
  WindoW           w;
  EditAlignDataPtr adp;
  SeqEntryPtr      sep;
  SeqIdPtr         sip,
                   import_sip;
  Char             str[52],
                   import_str[52];
  SeqAlignPtr      salp;
  BioseqPtr        bsp;

  w = (WindoW)ParentWindow(i);
  if ( ( adp = GetAlignEditData (w) ) == NULL ) 
     return;
  
  sep = ReadAnyAlignment (ISA_aa(adp->mol_type), NULL);
  salp = (SeqAlignPtr) FindSeqAlignInSeqEntry (sep, (Uint1)OBJ_SEQALIGN);
  import_sip = SeqAlignId (salp, 0); 
  if (import_sip!=NULL)
  {
     SeqIdWrite (import_sip, import_str, PRINTID_TEXTID_LOCUS, 50);
     sip = SeqLocId((SeqLocPtr)adp->master.region);
     bsp = BioseqLockById (sip);
     if (bsp) {
        SeqIdWrite (bsp->id, str, PRINTID_FASTA_LONG, 50);
        BioseqUnlock (bsp);
     }
     else
        SeqIdWrite (sip, str, PRINTID_FASTA_LONG, 50);
     if ((StringStr(str, import_str))!=NULL)
     {
        sip = SeqLocId((SeqLocPtr)adp->master.region);
        SeqAlignIdReplace (salp, 0, sip);
        repopulate_panel (w, adp, salp);
        return;
     }
  }
  SeqEntryFree (sep);
}

static void CCFetchFromNetFunc (WindoW w, Int2 method)
{
  WindoW           temport;
  PaneL            pnl;
  EditAlignDataPtr adp;
  Boolean          net_configuation;

  if (w==NULL)
     return;
  net_configuation = AmIConfiguredForTheNetwork();
  if (!net_configuation) {
     Message (MSG_OK, "No network configuration");
     return;
  }
  if ( ( pnl= GetPanelFromWindow (w) ) == NULL) 
     return;
  temport = SavePort (pnl);
  Select (pnl);
  if ( ( adp = GetAlignEditData (w) ) == NULL ) 
     return;
  adp->align_format = (Uint1) method;
  CCFetchFromNet (adp, w);
  RestorePort (temport);
}

static void CCFetchFromNet1Proc (IteM i)
{
  CCFetchFromNetFunc ((WindoW)ParentWindow(i), (Int2)PRGALIGNDEFAULT);
}

static void CCFetchFromNet2Proc (IteM i)
{
  CCFetchFromNetFunc ((WindoW)ParentWindow(i), (Int2)PRGALIGNBANDBL);
}
/**
static void CCFetchFromNet3Proc (IteM i)
{
  CCFetchFromNetFunc ((WindoW)ParentWindow(i), (Int2)PRG_FASTA_IMPORT);
}
**/
static void CCFetchFromNet4Proc (IteM i)
{
  CCFetchFromNetFunc ((WindoW)ParentWindow(i), (Int2)PRG_BLAST);
}

/***********************************************/
static void ExportTextProc (IteM i)
{
  EditAlignDataPtr adp;

  if ( ( adp = GetAlignEditData ((WindoW)ParentWindow (i)) ) == NULL ) return;
  adp->align_format = SALSA_SHWTXT;
  SalsaExportDialog (i);
}

static void ExportFastaProc (IteM i)
{
  EditAlignDataPtr adp;
  if ( ( adp = GetAlignEditData ((WindoW)ParentWindow (i)) ) == NULL ) return;
  adp->align_format = SALSA_FASTA;
  SalsaExportDialog (i);
}
static void ExportFastaGapProc (IteM i)
{
  EditAlignDataPtr adp;
  if ( ( adp = GetAlignEditData ((WindoW)ParentWindow (i)) ) == NULL ) return;
  adp->align_format = SALSA_FASTGAP;
  SalsaExportDialog (i);
}
static void ExportPhyProc (IteM i)
{
  EditAlignDataPtr adp;
  if ( ( adp = GetAlignEditData ((WindoW)ParentWindow (i)) ) == NULL ) return;
  adp->align_format = SALSA_PHYLIP;
  SalsaExportDialog (i);
}
static void ExportSeqAnnotProc (IteM i)
{
  EditAlignDataPtr adp;
  if ( ( adp = GetAlignEditData ((WindoW)ParentWindow (i)) ) == NULL ) return;
  adp->align_format = SALSA_ASN1;
  SalsaExportDialog (i);
}

/***********************************************************
***
*** CloseWindow 
***     reads the temporary file
***     sends a message to ObjMgr (ObjMgrSendMessage(MSG_UPDATE))
***     frees the data structure
***     deletes the temporary file 
***
************************************************************/
static void CloseWindowProc (PaneL pnl)
{
  WindoW             w;
  EditAlignDataPtr   adp;
  SeqEntryPtr        prevsep;
  SeqEntryPtr        currentsep;

  w = (WindoW)ParentWindow (pnl);
  if ( ( adp = GetAlignEditData (w) ) == NULL ) return;
  if (adp->dirty) {
     if (adp->input_format == OBJ_BIOSEQ) 
     {
        Hide (w);
        Update ();
        prevsep = seqentry_read (adp->tmpfile);
        currentsep = GetTopSeqEntryForEntityID (adp->master.entityID);
        ReplaceSeqEntryWithSeqEntry (currentsep, prevsep, TRUE);
        ObjMgrSendMsg (OM_MSG_UPDATE, adp->master.entityID, adp->master.itemID, adp->master.itemtype);
        Remove (w);
     }
     else if (adp->input_format == OBJ_SEQALIGN && adp->dirty) 
     {
/******
        SaveAlignDlg (w);
******/
        Hide (w);
        Update ();
        Remove (w);
     }
  }
  else {
     Hide (w);
     Update ();
     Remove (w);
  }
}

static void CloseWindowItemProc (IteM i)
{
  PaneL            pnl;
  if ( ( pnl= GetPanelFromWindow ((WindoW)ParentWindow (i)) ) == NULL) return;
  CloseWindowProc (pnl);
}

static void CloseWindowButton (ButtoN b)
{
  PaneL            pnl;
  if ( ( pnl= GetPanelFromWindow ((WindoW)ParentWindow (b)) ) == NULL) return;
  CloseWindowProc (pnl);
}

static void AcceptCloseFunc (WindoW w, EditAlignDataPtr adp)
{
  PaneL              pnl;

  if ( ( pnl= GetPanelFromWindow (w) ) == NULL) return;
  Hide (w);
  Update ();
  if (adp->dirty) {
     if (adp->input_format == OBJ_BIOSEQ) {
        SaveAllFeatProc (pnl);
        ObjMgrSendMsg (OM_MSG_UPDATE, adp->master.entityID, adp->master.itemID, adp->master.itemtype);
     }
  }
  Remove (w);
}

static void AcceptCloseWithoutSaveCDSProc (ButtoN b)
{
  WindoW           wdialog;
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;

  wdialog = ParentWindow (b);
  Hide (wdialog);
  Update ();
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) != NULL ) {
     adp->feat = SeqfeatlistFree (adp->feat);
     AcceptCloseFunc (dbdp->w, adp);
  }
  Remove (wdialog);
}

static void SaveCDSConfirmDlg (WindoW w)
{
  WindoW           wdialog;
  DialogBoxDataPtr dbdp;
  GrouP            g, c;

  wdialog = FixedWindow (-50, -33, -10, -10, "Save CDS", StdCloseWindowProc);
  dbdp = (DialogBoxDataPtr) MemNew (sizeof (DialogBoxData));
  SetObjectExtra (wdialog, (Pointer) dbdp, StdCleanupExtraProc);
  dbdp->w = w;
  g = HiddenGroup (wdialog, 1, 0, NULL);
  StaticPrompt (g, "Closing the window will lose unsaved features.", 0, dialogTextHeight,systemFont, 'l');
  c = HiddenGroup (wdialog, 2, 0, NULL);
  PushButton (c, "OK", AcceptCloseWithoutSaveCDSProc);
  PushButton (c, "Cancel", StdCancelButtonProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
  RealizeWindow (wdialog);
  Show (wdialog);
  return;
}

static void AcceptCloseWindowButton (ButtoN b)
{
  WindoW             w;
  EditAlignDataPtr   adp;

  w = (WindoW)ParentWindow (b);
  if ((adp = GetAlignEditData (w)) == NULL ) return;
  ObjMgrSetDirtyFlag (adp->input_entityID, TRUE);
  if (adp->feat != NULL) {
     SaveCDSConfirmDlg (w);
  }
  else AcceptCloseFunc (w, adp);
}

static void AcceptCloseWindowItemProc (IteM i)
{
  WindoW             w;
  EditAlignDataPtr   adp;

  w = (WindoW)ParentWindow (i);
  if ((adp = GetAlignEditData (w)) == NULL ) return;
  ObjMgrSetDirtyFlag (adp->input_entityID, TRUE);
  if (adp->feat != NULL) {
     SaveCDSConfirmDlg (w);
  }
  else AcceptCloseFunc (w, adp);
}

/*********************************************************
***
***
*********************************************************/
static void UndoProc (IteM i)
{
  PaneL              pnl;
  EditAlignDataPtr   adp;
  SeqEntryPtr        prevsep, currentsep;
  if ( ( pnl= GetPanelFromWindow ((WindoW)ParentWindow (i)) ) == NULL) return;
  if ( ( adp = GetAlignDataPanel (pnl) ) == NULL) return;
  if (adp->tmpfile != NULL && adp->tmpfile[0] != '\0') 
  {
     if (adp->master.entityID > 0) {
        Update ();
        prevsep = seqentry_read (adp->tmpfile);
        currentsep = GetTopSeqEntryForEntityID (adp->master.entityID);
        ReplaceSeqEntryWithSeqEntry (currentsep, prevsep, TRUE);
        ObjMgrSendMsg (OM_MSG_UPDATE, adp->master.entityID, adp->master.itemID, adp->master.itemtype);
     }
  }
  return;
}

/*********************************************************
***  CutProc
***    get the selected segment, call do_cut 
***    Deselect the segment
**********************************************************/
static void CutProc (IteM i)
{
  PaneL              pnl;
  EditAlignDataPtr   adp;
  SelStructPtr       ssp = NULL;

  if ( ( pnl= GetPanelFromWindow ((WindoW)ParentWindow (i)) ) == NULL) return;
  if ( ( adp = GetAlignDataPanel (pnl) ) == NULL) return;
  if (adp->input_format == OBJ_BIOSEQ)
  {
     if ( checkOMss_for_itemtype (OBJ_BIOSEQ) == 0 ) return;
     ssp = ObjMgrGetSelected ();
     for (; ssp != NULL; ssp = ssp->next)
        if ( ssp->itemtype == OBJ_BIOSEQ && checkssp_for_editor (ssp) ) 
           break;
     if (ssp != NULL) {
        do_cut (pnl, adp, ssp, TRUE);
     }
  }
  else if (adp->input_format == OBJ_SEQALIGN) {
     Beep ();
  }
  return;
}

/******************************
***  PasteProc
***     
*******************************/
static void PasteProc (IteM i)
{
  PaneL              pnl;
  EditAlignDataPtr   adp;
  SelStructPtr       ssp = NULL;
  BioseqPtr          bsp = NULL;
  ObjMgrDataPtr      omdp = NULL;

  if ( ( pnl= GetPanelFromWindow ((WindoW)ParentWindow (i)) ) == NULL) 
     return;
  if ( ( adp = GetAlignDataPanel (pnl) ) == NULL ) 
     return;
  if (adp->input_format != OBJ_BIOSEQ)
     return;
  if (ClipboardHasString ()) 
  {
     CharPtr     str = ClipboardToString ();
     CharPtr     strtmp;
     size_t      len = StringLen (str);

     strtmp = char_to_insert (str, len, adp->mol_type);
     if (insertchar_atcaret (strtmp, adp)) {
        adp->dirty = TRUE;
        ObjMgrSetDirtyFlag (adp->caret.entityID, TRUE);
        ObjMgrSendMsg (OM_MSG_UPDATE, adp->caret.entityID, adp->caret.itemID, adp->caret.itemtype);
     }
     MemFree (strtmp);
     return;
  }
  if ( adp->caret.regiontype == 0 ) {
        ErrPostEx (SEV_ERROR, 0, 0, "Click before paste");
        return;
  }
  omdp = ObjMgrGetClipBoard ();
  if (omdp == NULL) {
        ErrPostEx (SEV_ERROR, 0, 0, "ClipBoard empty");
        return;
  }
  if (omdp->datatype != OBJ_BIOSEQ || omdp->dataptr == NULL) {
        ErrPostEx (SEV_ERROR, 0, 0, "Error: ClipBoard is not BIOSEQ");
        return;
  }
  bsp = (BioseqPtr) omdp->dataptr;
  if (bsp!=NULL && bsp->length > 0) {
     if (ISA_na (bsp->mol) != ISA_na (adp->mol_type)) {
        ErrPostEx (SEV_ERROR, 0, 0, "Error: Pasting uncorrect molecule type [%d / %d]", bsp->mol, adp->mol_type);
     }
     else {
        ssp = ObjMgrGetSelected ();
        if (ssp!=NULL) {
           if (checkssp_for_editor(ssp) && ssp->itemtype==OBJ_BIOSEQ) {
              if (SeqIdForSameBioseq (SeqLocId((SeqLocPtr)(adp->caret.region)), SeqLocId((SeqLocPtr)(ssp->region))) )
                 if (do_cut (pnl, adp, ssp, FALSE)) 
                    adp->dirty = TRUE;
           }
        }
        else ssp = NULL;
        if (do_paste (pnl, adp, bsp->id) ) {
           adp->dirty = TRUE;
           ObjMgrSetDirtyFlag (adp->caret.entityID, TRUE);
           ObjMgrSendMsg (OM_MSG_UPDATE, adp->caret.entityID, adp->caret.itemID, adp->caret.itemtype);
        }
     }
  }
  return;
}

/******************************
*******************************/
static void DeleteProc (IteM i)
{
  WindoW             w;
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr   adp;
  SelStructPtr       ssp = NULL;
  SeqAnnotPtr        sap;
  SeqAlignPtr        newsalp;
  SeqLocPtr          slp;
  Int2               j;
  Boolean            ok,
                     deletion = FALSE;

  w = (WindoW)ParentWindow (i);
  if (w==NULL)
     return;
  wdp = (SeqEditViewFormPtr) GetObjectExtra ((WindoW)ParentWindow (i));
  if (wdp==NULL)
     return;
  if ((adp = GetAlignDataPanel(wdp->pnl))!= NULL) {
     if ((j = GetNumberObjMgrSelect()) < 1) {
        ErrPostEx (SEV_ERROR, 0, 0, "No selection");
        return;
     }
     if (j >= adp->seqnumber) {
        ErrPostEx (SEV_ERROR, 0, 0, "You will delete all the sequences except the last one");
        return;
     }
     if (adp->sap_original)
        sap = adp->sap_original;
     newsalp = (SeqAlignPtr)sap->data;
     ssp = ObjMgrGetSelected();
     for (; ssp != NULL; ssp = ssp->next) 
     {
        if (checkssp_for_editor (ssp) && ssp->itemtype == OBJ_BIOSEQ)
        {
           ok = FALSE;
           slp=(SeqLocPtr)ssp->region;
           if (slp)
           {
              if ((adp->sap1_original && (FindSeqIdinSeqAlign ((SeqAlignPtr)adp->sap1_original->data, SeqLocId(slp))) > 0) || (FindSeqIdinSeqAlign ((SeqAlignPtr)sap->data, SeqLocId(slp))) > 0)
              {
/**
                 ObjMgrSendMsg(OM_MSG_HIDE,ssp->entityID, ssp->itemID, ssp->itemtype);
**/
                 ok = SeqAlignIDCache (newsalp, SeqLocId((SeqLocPtr)ssp->region));
                 if (ok)
                    deletion=TRUE;
              }
              else
                 Message (MSG_OK, "Can not delete a sequence from the original input");
           }
        }
        else if ( ssp->itemtype == OBJ_VIRT ) {
            adp->feat = del_ssp_fromid (adp->feat, 255, ss_to_ses(ssp));
            adp->dirty = TRUE;
            ObjMgrSendMsg (OM_MSG_UPDATE, ssp->entityID, ssp->itemID, ssp->itemtype);
            ObjMgrDeSelect (ssp->entityID, ssp->itemID, ssp->itemtype, ssp->regiontype, ssp->region);
            return;
        }
        else if (ssp->itemtype == OBJ_SEQFEAT ) {
            ErrPostEx (SEV_ERROR, 0, 0, "Can't delete a feature");
            return;
        }
     }
     if (deletion)
     {
        repopulate_panel (w, adp, newsalp);
        ObjMgrDeSelectAll ();
     }
  }
  return;
}

/**********************************************************
***
***  ChangeCharModeProc:   
***     adp->charmode shows all char 
***     otherwise dots replacing chars identical to master's
***
**********************************************************/
static void ChangeCharModeProc (IteM i)
{
  PaneL            pnl;
  WindoW           w,
                   temport;
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr adp;
 
  w = (WindoW)ParentWindow (i); 
  if (w==NULL)
     return;
  wdp = (SeqEditViewFormPtr) GetObjectExtra (w);
  if (wdp==NULL)
     return;
  if ( ( pnl= GetPanelFromWindow (w) ) == NULL) 
     return;
  if ( ( adp = GetAlignDataPanel (pnl) ) == NULL ) 
     return;
  adp->charmode = ! adp->charmode;
  ResetClip ();
  if ( !adp->charmode) {
     Enable(wdp->showdifitem);
     Disable (wdp->showallitem);
  } else {
     Disable(wdp->showdifitem);
     Enable (wdp->showallitem);
  }
  temport = SavePort(w);
  Select (pnl);
  inval_panel (pnl, -1, -1);
  RestorePort (temport);
  return;
}

/*********************************************************
***    
***  SelectAllProc: 
***    SelectAllSeqEntry + highlight all the sequence Ids 
***      
**********************************************************/
static void SelectAllProc (IteM i)
{
  WindoW           temport;
  PaneL            pnl;
  EditAlignDataPtr adp;
  ValNodePtr       vnp;
  SeqLocPtr        slp;
  AlignNodePtr     anp;
  Uint2            ei, ii;
  Boolean          first = TRUE;

  if ( ( pnl= GetPanelFromWindow ((WindoW)ParentWindow (i)) ) == NULL) return;
  temport = SavePort((WindoW)ParentWindow (i));
  Select (pnl);
  if ( ( adp = GetAlignDataPanel (pnl) ) == NULL ) return;
  if ( adp->seqnumber == 0 || adp->anp_list == NULL) return;
  for (vnp =adp->anp_list; vnp !=NULL; vnp =vnp->next)
  {
     anp = (AlignNodePtr) vnp->data.ptrvalue;
     if (anp != NULL)
     {   
           ei = anp->seq_entityID;
           ii = anp->bsp_itemID;
           slp = CollectSeqLocFromAlignNode (anp);
           if (slp != NULL) { 
              if (first) {
                 ObjMgrSelect (ei, ii, OBJ_BIOSEQ, OM_REGION_SEQLOC, slp);
                 first = FALSE;
              }
              else {
                 ObjMgrAlsoSelect (ei, ii, OBJ_BIOSEQ, OM_REGION_SEQLOC, slp);
              }
           }
     }   
  }  
  RestorePort (temport);
  return;
}

/*********************************************************
***
***  SelectMasterProc
***
**********************************************************/
static void SelectMasterProc (IteM i)
{
  WindoW           temport;
  PaneL            pnl;
  EditAlignDataPtr adp;
  SelStructPtr     ssp;
  SeqLocPtr        slp, slptmp;
  RecT             rp;

  if ( ( pnl= GetPanelFromWindow ((WindoW)ParentWindow (i)) ) == NULL) return;
  if ( ( adp = GetAlignDataPanel (pnl) ) == NULL ) return;
  if ( adp->seqnumber == 0 ) return;
  temport = SavePort((WindoW)ParentWindow (i));
  Select (pnl);
  ssp = ObjMgrGetSelected();  
  if ( !checkssp_for_editor (ssp) ) return;
  if ( ssp->itemtype == OBJ_BIOSEQ && (adp->master.itemID != ssp->itemID
     || adp->master.entityID != ssp->entityID )) 
  {
         adp->master.entityID = ssp->entityID;
         adp->master.itemID = ssp->itemID;
         SeqLocFree ((SeqLocPtr) adp->master.region);
         slp = (SeqLocPtr) ssp->region;
         slptmp = SeqLocIntNew (SeqLocStart(slp), SeqLocStop(slp), SeqLocStrand(slp), SeqLocId(slp));
         adp->master.region = (Pointer) slptmp;
         if (adp->charmode) {
            inval_panel (pnl, -1, -1);
         }
         else {
            get_client_rect (pnl, &rp);
            inval_rect (rp.left, rp.top, (Int2)(rp.left+adp->margin.left-1), rp.bottom);
         }
  }
  RestorePort (temport);
}

static void SelectSubsProc (IteM i)
{
  WindoW           temport;
  PaneL            pnl;
  EditAlignDataPtr adp;
  SeqAlignPtr      salp;
  ValNodePtr       vnp = NULL;
  SeqIdPtr         sip;
  CharPtr PNTR     bufstr;
  SeqLocPtr        slp;
  Int4             from, 
                   to,
                   masterlength,
                   k, k2;
  Int2             seqnumber,
                   nalgline,
                   j;
  Uint2            eID, iID;
  Boolean          goOn= FALSE,
                   sel = FALSE,
                   first=TRUE;
  BoolPtr          select;
  SeqEditViewProcsPtr svpp=NULL;
  
  svpp = (SeqEditViewProcsPtr) GetAppProperty ("SeqEditDisplayForm");
  if ( ( pnl= GetPanelFromWindow ((WindoW)ParentWindow (i)) ) == NULL)
     return;
  if ( ( adp = GetAlignDataPanel (pnl) ) == NULL ) return;
  if ( adp->seqnumber == 0 ) return;
  temport = SavePort((WindoW)ParentWindow (i));
  Select (pnl);

  masterlength = SeqLocLen((SeqLocPtr)adp->master.region);
  select=(BoolPtr)MemNew((size_t) ((masterlength+5) * sizeof(Boolean)));
  MemSet ((Pointer)select, 0, (size_t)((masterlength+5)*sizeof(Boolean)));
  salp = (SeqAlignPtr)adp->sap_align->data;
  from = 0;
  to = adp->length-1;
  seqnumber = adp->seqnumber;
  goOn = read_buffer_fromalignnode (adp, &vnp, from, to, &nalgline);

  if (goOn)
  {
     sip = SeqAlignId (salp, 0);
     bufstr=buf2array (vnp, 0, seqnumber-1);
     for (j=1; j<seqnumber; j++) 
     {
		for (k=0; k<to+1; k++)
        {
		   if (bufstr[j][k] !='-' && bufstr[0][k]!='-' && bufstr[j][k] != bufstr[0][k])
           {
              k2 = AlignCoordToSeqCoord(k, sip, salp, adp->sqloc_list,0);
              select[k2]=TRUE; 
           }
        }
     }
     eID=adp->master.entityID;
     iID=adp->master.itemID;
     k2=-1;
     k=0;
     sel=select[k];
     if (sel)
        k2=k;
     k++;
     for (; k<masterlength; k++)
     {
        if(select[k] && !sel) {
           k2=k;
           sel=TRUE;
        }
        else if (!select[k] && sel) {
           if (k2>-1) {
              slp=SeqLocIntNew (k2, k-1, Seq_strand_plus, sip);
              if (first) {
                 ObjMgrSelect(eID, iID, OBJ_BIOSEQ, OM_REGION_SEQLOC,(Pointer)slp);
                 first=FALSE;
              } else
                 ObjMgrAlsoSelect(eID, iID, OBJ_BIOSEQ, OM_REGION_SEQLOC,(Pointer)slp);
           }
           sel=FALSE;
           k2=-1;
        }
     }
     if (k2>-1) {
        slp=SeqLocIntNew (k2, k-1, Seq_strand_plus, sip);
        if (first) {
           ObjMgrSelect (eID, iID, OBJ_BIOSEQ, OM_REGION_SEQLOC,(Pointer)slp);
           first=FALSE;
        } else
           ObjMgrAlsoSelect(eID, iID, OBJ_BIOSEQ, OM_REGION_SEQLOC,(Pointer)slp);
     }
  }
  RestorePort (temport);
  MemFree(select);/*don't forget, dear Colombe, to free your memory !*/
}

/**********************************************************
***
*** RefreshAlignData: refreches the window
***
***********************************************************/
static void RefreshAlignDataProc (PaneL pnl)
{
  WindoW           temport;
  EditAlignDataPtr adp;

  if ( ( adp = GetAlignDataPanel (pnl) ) == NULL ) return;
  if ( adp->seqnumber == 0 ) return;
  data_collect_arrange (adp, TRUE);
  temport = SavePort (pnl);
  Select (pnl);
  inval_panel (pnl, -1, -1);
  RestorePort (temport);
  return;
}

static void RefreshAlignDataItem (IteM i)
{
  PaneL            pnl;
  if ( ( pnl= GetPanelFromWindow ((WindoW)ParentWindow (i)) ) == NULL) return;
  RefreshAlignDataProc (pnl);
  CaptureSlateFocus ((SlatE) pnl);
}

static void RefreshAlignDataButton (ButtoN b)
{
  PaneL            pnl;
  if ( ( pnl= GetPanelFromWindow ((WindoW)ParentWindow (b)) ) == NULL) return;
  RefreshAlignDataProc (pnl);
  CaptureSlateFocus ((SlatE) pnl);
}

/**********************************************************
***
***  Complement
***
***  SetRf, SetRfFunc:
***
***********************************************************/
static void ComplementProc (EditAlignDataPtr adp)
{
  SelStructPtr     ssp;
  ValNodePtr       vnp;
  SeqParamPtr      prm;

  if ( checkOMss_for_itemtype (OBJ_BIOSEQ) == 0 ) {
     ssp = &(adp->master);
  }
  else ssp = ObjMgrGetSelected();  
  for (; ssp != NULL; ssp = ssp->next) {
     if (checkssp_for_editor (ssp) && ssp->itemtype == OBJ_BIOSEQ) 
     {
        for (vnp = adp->params; vnp != NULL; vnp = vnp->next) {
           prm = (SeqParamPtr) vnp->data.ptrvalue;
           if ( prm->entityID == ssp->entityID )
              prm->complement = !(prm->complement);
        } 
     }
  }
  return; 
}

static void ComplementItemProc (IteM i)
{
  WindoW           temport;
  PaneL            pnl;
  EditAlignDataPtr adp;
  float hratio;
  
  if ( ( pnl= GetPanelFromWindow ((WindoW)ParentWindow (i)) ) == NULL) return;
  if ( ( adp = GetAlignDataPanel (pnl) ) == NULL ) return;
  if (adp->seqnumber == 0 || ISA_aa(adp->mol_type))
    return;
  adp->drawcomplt = GetStatus (i);
  temport = SavePort((WindoW)ParentWindow (i));
  Select (pnl);
  inval_panel (pnl, -1, -1);
  ComplementProc (adp);
  data_collect_arrange (adp, TRUE);
  hratio = (float)adp->hoffset / (float)adp->length;
  SeqEdSetCorrectBarMax (pnl, adp, hratio);
  RestorePort (temport);
}


static void changeseqid (PaneL pnl, Uint2 choice)
{
  WindoW           temport;
  EditAlignDataPtr adp;

  if ( ( adp = GetAlignDataPanel (pnl) ) == NULL ) 
     return;
  if (adp->seqnumber == 0)
    return;
  if (choice < 6) {
     adp->printid = choice;
     adp->size_labels = getsize_seqids (adp->sqloc_list, choice);
     adp->marginwithIds = TRUE;
  }
  else if (choice == PRINTID_GIcc) {
     adp->printid = choice;
     adp->size_labels = 10; 
     adp->marginwithIds = TRUE;
  }
  else if (choice == 6) {
     adp->printid = 0;
     adp->size_labels = 15;
     adp->marginwithIds = FALSE;
  }
  temport = SavePort(pnl);
  Select (pnl);
  inval_panel (pnl, -1, -1);
  RestorePort (temport);
}
static void changeseqid1 (IteM i) {
  changeseqid ((PaneL)GetPanelFromWindow((WindoW)ParentWindow (i)), (Uint2)PRINTID_GIcc);
}
static void changeseqid2 (IteM i) {
  changeseqid ((PaneL)GetPanelFromWindow((WindoW)ParentWindow (i)), (Uint2)PRINTID_FASTA_LONG);
}
static void changeseqid3 (IteM i) {
  changeseqid ((PaneL)GetPanelFromWindow((WindoW)ParentWindow (i)), (Uint2)PRINTID_TEXTID_LOCUS);
}
static void changeseqid4 (IteM i) {
  changeseqid ((PaneL)GetPanelFromWindow((WindoW)ParentWindow (i)), (Uint2)PRINTID_TEXTID_ACCESSION);
}
static void changeseqid6 (IteM i) {
  changeseqid ((PaneL)GetPanelFromWindow((WindoW)ParentWindow (i)), (Uint2)6);
}

/******************************************************************
***
***  MakemRNArProc
***  MakeCdRgnProc , MakeCdRgnrProc
***  SaveFeatureButton
***
*******************************************************************/

static void MakeCdRgnProc (IteM i) {
  MakeFeatProc((PaneL)GetPanelFromWindow((WindoW)ParentWindow (i)), SEQFEAT_CDREGION, Seq_strand_plus);
}

static void MakeCdRgnrProc (IteM i) {
  MakeFeatProc((PaneL)GetPanelFromWindow((WindoW)ParentWindow (i)), SEQFEAT_CDREGION, Seq_strand_minus);
}

static void SaveFeatureButton (ButtoN b)
{
  PaneL              pnl;
  if ( ( pnl= GetPanelFromWindow ((WindoW)ParentWindow (b)) ) == NULL) return;
  SaveFeatProc (pnl);
  CaptureSlateFocus ((SlatE) pnl);
}

/******************************************************************
***
***  TranslateProc , TranslateButton
***
*******************************************************************/

static void TranslateButton (ButtoN b)
{
  PaneL            pnl;
  EditAlignDataPtr adp;

  if ( ( pnl= GetPanelFromWindow ((WindoW)ParentWindow (b)) ) == NULL) return;
  if ( ( adp = GetAlignDataPanel (pnl) ) == NULL ) return;
  if ( adp->seqnumber == 0 ) return;
  if (GetNumberObjMgrSelect() == 0)
     TranslateAllBioseq (pnl, adp);
  if ( checkOMss_for_itemtype (OBJ_VIRT) != 0 
     || checkOMss_for_itemtype (OBJ_SEQFEAT) != 0 ) {
     CdRgnToProtProc (pnl, adp);
     if (!adp->display_panel) {
        update_translateitem (pnl, adp->seqfeat, adp->feat);
        update_codonstartbt (pnl, adp->seqfeat, adp->feat);
     }
  }
  CaptureSlateFocus ((SlatE) pnl);
}

/***********************************************************
***  FeatEditModeProc
************************************************************/
static void SelectFeatEditMode (PopuP p)
{
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr   adp;
  Int2               val;

  wdp = (SeqEditViewFormPtr) GetObjectExtra ((WindoW)ParentWindow (p));
  if ( ( adp = GetAlignDataPanel (wdp->pnl) ) == NULL ) return;
  val = GetValue(p); 
  if (val==1) 
     adp->spliteditmode = FALSE;
  else if (val==2) 
     adp->spliteditmode = TRUE;
  CaptureSlateFocus ((SlatE) wdp->pnl);
}

static void EditModeProc (IteM i)
{
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr   adp;
 
  wdp = (SeqEditViewFormPtr) GetObjectExtra ((WindoW)ParentWindow (i));
  if ( ( adp = GetAlignDataPanel (wdp->pnl) ) != NULL )
  {
     if (adp->edit_mode == SEQ_EDIT || adp->edit_mode == ALIGN_EDIT)
     {
        adp->edit_mode = SEQ_VIEW;
        editor2viewer (wdp);
     }
     else {
        if (adp->seqnumber > 1)
           adp->edit_mode = ALIGN_EDIT;
        else
           adp->edit_mode = SEQ_EDIT;
        viewer2editor (wdp);
     }
  }
}

/***********************************************************
***  ShowFeatureItemProc
***  ShowFeatureButtonProc
************************************************************/
static void ShowFeatureItemProc (IteM i)
{
  PaneL              pnl;
  if ( ( pnl= GetPanelFromWindow ((WindoW)ParentWindow (i)) ) == NULL) return;
  ShowFeatureProc (pnl, TRUE);
}
 
static void ShowFeatureButtonProc (ButtoN b)
{
  PaneL              pnl;
  if ( ( pnl= GetPanelFromWindow ((WindoW)ParentWindow (b)) ) == NULL) return;
  ShowFeatureProc (pnl, TRUE);
  CaptureSlateFocus ((SlatE) pnl);
}

/***********************************************************
***  ChangeProtModeProc
************************************************************/
static void ChangeProtModeProc (IteM i)
{
  WindoW           temport;
  PaneL            pnl;
  EditAlignDataPtr adp;

  if ( ( pnl= GetPanelFromWindow ((WindoW)ParentWindow (i)) ) == NULL) return;
  if ( ( adp = GetAlignDataPanel (pnl) ) == NULL ) return;
  if ( adp->seqnumber == 0 ) return;
  adp->prot_mode = ALLPROTAA;
  adp->firstssp = NULL;
  data_collect_arrange (adp, FALSE);
  temport = SavePort((WindoW)ParentWindow (i));
  Select (pnl);
  inval_panel (pnl, -1, -1);
  RestorePort (temport);
}

static void ChangeProtModeProc2 (IteM i)
{
  WindoW           temport;
  PaneL            pnl;
  EditAlignDataPtr adp;

  if ( ( pnl= GetPanelFromWindow ((WindoW)ParentWindow (i)) ) == NULL) return;
  if ( ( adp = GetAlignDataPanel (pnl) ) == NULL ) return;
  if ( adp->seqnumber == 0 ) return;
  adp->prot_mode = MPROTAA;
  temport = SavePort((WindoW)ParentWindow (i));
  Select (pnl);
  inval_panel (pnl, -1, -1);
  RestorePort (temport);
}

static void ChangeProtModeProc3 (IteM i)
{
  WindoW           temport;
  PaneL            pnl;
  EditAlignDataPtr adp;

  if ( ( pnl= GetPanelFromWindow ((WindoW)ParentWindow (i)) ) == NULL) return;
  if ( ( adp = GetAlignDataPanel (pnl) ) == NULL ) return;
  if ( adp->seqnumber == 0 ) return;
  adp->prot_mode = PUTPROT;
  adp->firstssp = NULL;
  data_collect_arrange (adp, FALSE);
  temport = SavePort((WindoW)ParentWindow (i));
  Select (pnl);
  inval_panel (pnl, -1, -1);
  RestorePort (temport);
}

/*****************************************************************
***    SetupMenus
***
  CommandItem (m, "Open Sequences", OpenSequenceProc);
  CommandItem (m, "Open SeqAnnot",  OpenSeqAnnotProc); 
  CommandItem (m, "Open Alignment", OpenAlignmentProc);
  CommandItem (m, "Add Alignment", OpenAlignmentProc);

*******************************************************************/
static void EditorEditMenu (MenU m, SeqEditViewFormPtr wdp)
{
  wdp->undoitem = CommandItem (m, "Undo", UndoProc);
  SeparatorItem (m);
  wdp->cutitem   = CommandItem (m, "Cut", CutProc);
  Disable (wdp->cutitem);
  wdp->pasteitem = CommandItem (m, "Paste", PasteProc);
  wdp->copyitem  = CommandItem (m, "Copy", do_copy);
  Disable (wdp->copyitem);
  CommandItem (m, "Refresh", RefreshAlignDataItem);
  SeparatorItem (m);
  wdp->delitem   = CommandItem (m, "Delete Seq", DeleteProc);
  Disable (wdp->delitem);
/****
  SeparatorItem (m);
  CommandItem (m, "Select Region", SelectRegionDialog);
****/
}
  
static void CommonViewMenu (MenU m, SeqEditViewFormPtr wdp, Boolean isa_aa)
{
  IteM  localItem;
  MenU  sub;

  wdp->viewmodeitem = CommandItem (m, "View mode", EditModeProc);
  wdp->editmodeitem = CommandItem (m, "Edit mode", EditModeProc);
  SeparatorItem (m); 
  sub = SubMenu (m, "Label");
    CommandItem (sub, "Accession", changeseqid4);
    CommandItem (sub, "Gi", changeseqid1);
    CommandItem (sub, "Locus", changeseqid3);
    CommandItem (sub, "Complete", changeseqid2);
    CommandItem (sub, "None", changeseqid6);
  CommandItem (m, "Font", SeqFontProc);
  wdp->prefitem=CommandItem (m, "Preferences", DefinePanelDialog);
  
  if (!isa_aa) {
     SeparatorItem (m);
     localItem = StatusItem (m, "Complement", ComplementItemProc);
     SetStatus ( localItem, FALSE);
     sub = SubMenu (m, "Reading frames");
    wdp->rfitem[0] = StatusItem (sub, "1", rf1ItemProc);
    SetStatus ( wdp->rfitem [0], FALSE);
    wdp->rfitem[1] = StatusItem (sub, "2", rf2ItemProc);
    SetStatus ( wdp->rfitem [1], FALSE);
    wdp->rfitem[2] = StatusItem (sub, "3", rf3ItemProc);
    SetStatus ( wdp->rfitem [2], FALSE);
    wdp->rfitem[6] = StatusItem (sub, "all+", rf7ItemProc);
    SetStatus ( wdp->rfitem [6], FALSE);
    SeparatorItem (sub);
    wdp->rfitem[3] = StatusItem (sub, "4", rf4ItemProc);
    SetStatus ( wdp->rfitem [3], FALSE);
    wdp->rfitem[4] = StatusItem (sub, "5", rf5ItemProc);
    SetStatus ( wdp->rfitem [4], FALSE);
    wdp->rfitem[5] = StatusItem (sub, "6", rf6ItemProc);
    SetStatus ( wdp->rfitem [5], FALSE);
    wdp->rfitem[7] = StatusItem (sub, "all-", rf8ItemProc);
    SetStatus ( wdp->rfitem [7], FALSE);
    SeparatorItem (sub);
    wdp->rfitem[8] = StatusItem (sub, "all", rf9ItemProc);
    SetStatus ( wdp->rfitem [8], FALSE);
    wdp->rfitem[9] = StatusItem (sub, "none", rf10ItemProc);
    SetStatus ( wdp->rfitem [0], FALSE);
     sub = SubMenu (m, "Translation style");
     CommandItem (sub, "all", ChangeProtModeProc);
     CommandItem (sub, "M*", ChangeProtModeProc2);
     CommandItem (sub, "orf", ChangeProtModeProc3);
  }
#ifdef SALSA_DEBUG
  SeparatorItem (m);
  VSMAddToMenu (m, VSM_DESKTOP);
#endif
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

static void SalsaNewFeatureMenuProc (IteM i)

{
  EditAlignDataPtr  adp;
  MsgAnswer         ans;
  NewFeaturePtr     nfp;
  OMProcControl     ompc;
  ObjMgrProcPtr     ompp;
  PaneL             pnl;
  Int2              retval;

  pnl = (PaneL) GetPanelFromWindow ((WindoW) ParentWindow (i));
  if (pnl == NULL) return;
  adp = GetAlignDataPanel (pnl);
  if (adp == NULL) return;
  nfp = (NewFeaturePtr) GetObjectExtra (i);
  if (nfp == NULL) return;
  ompp = nfp->ompp;
  if (ompp == NULL || ompp->func == NULL) return;
  if (ompp->subinputtype == FEATDEF_BIOSRC) {
    ans = Message (MSG_YN, editOldSrcDescMsg);
    if (ans == ANS_NO) return;
  }
  MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
  ompc.input_entityID = adp->master.entityID;
  ompc.input_itemID = adp->master.itemID;
  ompc.input_itemtype = adp->master.itemtype;
  GatherDataForProc (&ompc, FALSE);
  ompc.proc = ompp;
  retval = (*(ompp->func)) (&ompc);
  if (retval == OM_MSG_RET_ERROR) {
    ErrShow ();
  }
}

static void SalsaNewFeaturesMenu (MenU m, Boolean is_na)

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
                      i = CommandItem (sub, ompp->proclabel, SalsaNewFeatureMenuProc);
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
                    subtype != FEATDEF_site_ref) {
                  i = CommandItem (sub, ompp->proclabel, SalsaNewFeatureMenuProc);
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
      
/**************************************************************
*** Menu item not used :
***   wdp->translateitem
***   wdp->codonstitem
***   wdp->savefeatitem
*** items not tested 
     CommandItem (m1, "Select", SelectSeqDialog);
***************************************************************/
static void SetupMenus (WindoW w, Boolean is_alignment, 
            Boolean isa_aa, Boolean editor_mode,
            Boolean ext_dist_menu, Boolean ext_align_menu, Boolean ext_tree_menu)
{
  SeqEditViewFormPtr wdp;
  MenU               mf, m1, m2, m3, m4;
  MenU               sub;

  SeqEditViewProcsPtr svpp=NULL;

  wdp = (SeqEditViewFormPtr) GetObjectExtra (w);
  if (wdp == NULL) 
     return;

  mf = PulldownMenu (w, "File");
  sub = SubMenu (mf, "Read Sequence File");
  wdp->importseq=CommandItem (sub, "BLAST", ImportFromFile4Proc);
  wdp->importseq=CommandItem (sub, "BLAST with extensions", ImportFromFile1Proc);
  wdp->importalg=CommandItem (sub, "Global Alignment", ImportFromFile2Proc);
/********TEMPO
  wdp->importnet=CommandItem (sub,"Manual Alignment", ImportFromFile3Proc);
****/
  sub = SubMenu (mf,"Download From Entrez");
  CommandItem (sub, "BLAST", CCFetchFromNet4Proc);
  CommandItem (sub, "BLAST with extensions", CCFetchFromNet1Proc);
  CommandItem (sub, "Global Alignment", CCFetchFromNet2Proc);
/*****TEMPO
  CommandItem (sub,"Manual Alignment", CCFetchFromNet3Proc);
***/
  wdp->importalg=CommandItem(mf, "Import Alignment", ImportAlignmentProc);

  sub = SubMenu (mf, "Export");
  CommandItem (sub, "Text", ExportTextProc);
  CommandItem (sub, "Fasta", ExportFastaProc);
  wdp->expfsagitem=CommandItem (sub, "Fasta+gaps", ExportFastaGapProc);
  wdp->expalgitem=CommandItem (sub, "Phylip Format", ExportPhyProc);
  wdp->expasnitem=CommandItem (sub, "Asn1 SeqAlign", ExportSeqAnnotProc);

  SeparatorItem (mf);
  if (is_alignment) {
     CommandItem (mf, "Dismiss", CloseWindowItemProc);  
  }
  else {
     if (editor_mode)
        CommandItem (mf, "Accept Changes", AcceptCloseWindowItemProc);
     CommandItem (mf, "Cancel", CloseWindowItemProc);
  }
  m1 = PulldownMenu (w, "Edit");
  EditorEditMenu (m1, wdp);

  m2 = PulldownMenu (w, "View");
  CommonViewMenu (m2, wdp, isa_aa);
  SeparatorItem (m2);
  CommandItem (m2, "Find/F", FindPatternDialog);

  wdp->showfeatitem = NULL;
  wdp->hidefeatitem = NULL;
  wdp->propaitem = NULL;
  wdp->tmpcdspitem=NULL;
  
  if (editor_mode)
  {
     m3= PulldownMenu (w, "Features");    
     if (!is_alignment) {
        SalsaNewFeaturesMenu (m3, (! isa_aa));
     }
     else
     {
        wdp->propaitem = CommandItem (m3, "Propagate", PropagateFeatDialog);
     }
/****
     if (!isa_aa && !is_alignment)  
     { 
        SeparatorItem (m3);
        wdp->tmpcdspitem=CommandItem(m3, "Temporary CDS +", MakeCdRgnProc);
        wdp->tmpcdsnitem=CommandItem(m3, "Temporary CDS -", MakeCdRgnrProc);
     }
*****/
  } 
  else {
     svpp = (SeqEditViewProcsPtr) GetAppProperty ("SeqEditDisplayForm");
     if (svpp) {
        if (svpp->Cn3D_On) {
           if(!is_alignment) {
              m3= PulldownMenu (w, "Features");    
              wdp->showfeatitem = CommandItem (m3, "Show Features", ShowFeatureItemProc);
              wdp->hidefeatitem = CommandItem (m3, "Hide Features", ShowFeatureItemProc);
              Disable (wdp->hidefeatitem);
           }
        }
        else {
           m3= PulldownMenu (w, "Features");    
           wdp->showfeatitem = CommandItem (m3, "Show Features", ShowFeatureItemProc);
           wdp->hidefeatitem = CommandItem (m3, "Hide Features", ShowFeatureItemProc);
           Disable (wdp->hidefeatitem);
        }
     }
     else {
        m3= PulldownMenu (w, "Features");    
        wdp->showfeatitem = CommandItem (m3, "Show Features", ShowFeatureItemProc);
        wdp->hidefeatitem = CommandItem (m3, "Hide Features", ShowFeatureItemProc);
        Disable (wdp->hidefeatitem);
     }
  }

  m4 = PulldownMenu (w, "Alignment");
  wdp->selmaster = CommandItem (m4, "Select Reference", SelectMasterProc);
  wdp->selall = CommandItem (m4, "Select All", SelectAllProc);
  SeparatorItem (m4);
  wdp->showdifitem=CommandItem (m4, "Show Substitutions", ChangeCharModeProc);
  wdp->showallitem=CommandItem (m4, "Show All", ChangeCharModeProc);
  SeparatorItem (m4);
  wdp->selsubs= CommandItem (m4, "Select Variations", SelectSubsProc);
/*if (!isa_aa)
     wdp->conscolor=CommandItem (m4, "Conservation", ColorIdentityDialogItem); */
/* yanli started */
  wdp->conscolor=CommandItem (m4, "Conservation", ColorIdentityDialogItem);
  svpp = (SeqEditViewProcsPtr) GetAppProperty ("SeqEditDisplayForm");
  if (svpp)
     svpp->conscolor = wdp->conscolor;
/* yanli ended */

  
  svpp = (SeqEditViewProcsPtr) GetAppProperty ("SeqEditDisplayForm");
  if (svpp) {
     if (!svpp->Cn3D_On) {
        SeparatorItem (m4);
        wdp->dotmat = CommandItem (m4, "Dot Matrix (BLAST)", DotPlotItem);
/**
        wdp->alreport = CommandItem (m4, "Align Report", AlignReportItem);
**/
     }
  }
  wdp->menu_align = m4; 
/*****
  if (is_alignment && ext_dist_menu)
  {
     SeparatorItem (m4);
     CommandItem (m4, "debug", NULL);
     CommandItem (m4, "Formula", FormulaDialog);
     CommandItem (m4, "Pw distance", PwDistanceItem);
     CommandItem (m4, "Pw dist per pos", DistPosItem);
     CommandItem (m4, "Gp distance", GpDistanceItem);
     CommandItem (m4, "Sort simil", SortbySimItem);
     CommandItem (m4, "Sort lenght", SortbyLenItem);
     CommandItem (m4, "Quorum", QuorumItem);
     CommandItem (m4, "Ks Ka gp", NonsyToSynItem1);
     CommandItem (m4, "Ks Ka gp wd", NonsyToSynItem2);
     CommandItem (m4, "Ks Ka ref", NonsyToSynItem3);
     CommandItem (m4, "Ks Ka ref wd", NonsyToSynItem4);
     CommandItem (m4, "Ks Ka btw gps", NonsyToSynItem5);
     CommandItem (m4, "Ks Ka btw gps wd", NonsyToSynItem6);
  }
#ifdef SALSA_DEBUG
  if (is_alignment && ext_tree_menu)
  {
     SeparatorItem (m4);
     CommandItem (m4, "debug", NULL);
     CommandItem (m4, "Show tree", TreeViewItem);
  }
#endif
*****/
}

/***************************************************************
***    CleanupSequenceEditorForm 
***
***      sends a message calling ObjMgrFreeUserData
***      StdCleanupFormProc
******************************************************************/
static void CleanupSequenceEditorForm (GraphiC g, VoidPtr data)

{
  StdCleanupFormProc (g, data);
}

/**********************************************************
***
***  CleanupAlignDataPanel: Callback to free any instance
***     specific memory that may be pointed to in the extra. 
***
***********************************************************/
static void CleanupAlignDataPanel (GraphiC g, VoidPtr data)

{
  EditAlignDataPtr   adp;
  SeqEditViewFormPtr wdp;
  Int4               strlens;
  SeqEntryPtr sep;
  Uint2       eID;
  OMUserDataPtr omudp;
  SelEdStructPtr sesp;

  adp = (EditAlignDataPtr) data;
  if (adp != NULL) 
  {
     wdp = (SeqEditViewFormPtr) adp->wdp;
     if (wdp != NULL) {
      if (wdp->input_entityID > 0) 
      {
        ObjMgrFreeUserData (wdp->input_entityID, wdp->procid, wdp->proctype, wdp->userkey);
        for (sesp=adp->seq_info; sesp!=NULL; sesp=sesp->next)
        {
              sep = GetTopSeqEntryForEntityID (sesp->entityID);
              if (sep!=NULL) {
                 eID = SeqMgrGetEntityIDForSeqEntry(sep);
                 omudp = ObjMgrGetUserData (eID, wdp->procid, OMPROC_EDIT, wdp->userkey);
                 if (omudp != NULL) 
                    ObjMgrFreeUserData (eID, wdp->procid, wdp->proctype, wdp->userkey);
              }
        }   
      }
     } 
     strlens = StringLen(adp->tmpfile);
     if (strlens > 0) 
        FileRemove (adp->tmpfile);
     adp = EditAlignDataFree (adp);
  }
}

/*****************************************************************
***
*** SalsaPanelHasResized
*** FindIdsInSalsaPanel
*** SaveSalsaPanel
*** ResetSalsaTextPanel
*** SalsaTextPanel
*** PopulateSalsaPanel
***
***/
#define SIDLAND          1
#define SEQLAND          2
#define BADLAND          3
#define HOLDDNLAND       4
#define HOLDUPLAND       5
/*****************************************************************/

static void ViewForGraph (VieweR view, Uint2 entityID, EditAlignDataPtr adp, Int2 width, ValNodePtr anp_graph)
{
  SegmenT   pic;
  SeqLocPtr slpmaster;
  ValNode   vn;
  Int4      y_pos = 0;
  Int4      scale;
  Int4      max_width;
  Uint1     style = MSM_MPAIR;

  pic = CreatePicture();
  slpmaster = (SeqLocPtr) adp->master.region;
  vn.data.ptrvalue = slpmaster;
  vn.next = NULL;
  max_width = CountMaxSeqLabel(&vn);
  scale = FigureMaxScale (&vn, (Int2)width, max_width);
  if (scale<=0)
     scale = 1;
  max_width += 2;
  max_width *= scale;
  DrawMPAlignment (anp_graph, 0, adp->length-1, slpmaster, entityID, scale, &y_pos, (Uint1)style, FALSE, pic);
  AttachPicture (view, pic, -max_width, INT4_MAX, UPPER_LEFT, scale, 1, NULL);
}

static Boolean anp_has_feature (AlignNodePtr anp)
{
  AlignSegPtr asp;

  if(anp != NULL)
  {
     for(asp = anp->segs; asp != NULL; asp = asp->next) {
        if(asp->cnp != NULL)
           return TRUE;
     }
  }
  return FALSE;
}

static Uint1 getmoltype (SeqIdPtr sip)
{
  BioseqPtr bsp;
  Uint1     moltype;

  if (sip != NULL) {
     bsp = BioseqLockById (sip);
     if (bsp != NULL) {
        moltype = bsp->mol;
        BioseqUnlock (bsp);
        return moltype;
     }
  }
  return Seq_mol_na;
}

static Boolean is_segbioseq (BioseqPtr bsp)
{
  BioseqPtr   bigbsp; 
  SeqEntryPtr sep;

  sep = SeqEntryFind (bsp->id);
  if (sep != NULL) {
     bigbsp = find_big_bioseq (sep);
     if (bigbsp != NULL) 
     {
        if (bigbsp->repr == Seq_repr_seg && bigbsp->length > bsp->length) 
           return TRUE;
     }
  }
  return FALSE;
}

static void getcurrentfeatstyle (EditAlignDataPtr adp)
{
  Int2             oldstyle;
  Uint1            groupNum;
  Int2             j;

  oldstyle = GetMuskCurrentSt ();
  SetMuskCurrentSt (GetMuskStyleName (adp->styleNum));
  for(j =0; j<FEATDEF_ANY; j++) 
  {
     adp->featOrder[j] = (Uint1)GetMuskCParam(j, MSM_FORDER, MSM_NUM);
     groupNum = (Uint1)GetMuskCParam(j, MSM_FGROUP, MSM_NUM);
     adp->groupOrder[j] = (Uint1)GetMuskCParam(MSM_GROUPS, (Int2)groupNum, MSM_NUM);
  }
  SetMuskCurrentSt (GetMuskStyleName (oldstyle));
}

static ValNodePtr SetupOptions (Int2 seqnumber)
{
  ValNodePtr  params = NULL; 
  SeqParamPtr prm;
  Int2        j, k;
 
  if (seqnumber > 0) {
   for (j = 0; j < seqnumber; j++) {
     prm = (SeqParamPtr) MemNew (sizeof(SeqParam));
     prm->entityID = 0;
     prm->itemID = 0;
     prm->complement = FALSE;
     for (k=0; k<6; k++) prm->rf[k] = FALSE;
     prm->group = 0;
     prm->sip_cons = NULL;
     ValNodeAddPointer (&(params), 0, (Pointer) prm);
   }
  }
  return params; 
}

static void CheckSeqAlignCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)
{
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  SeqAnnotPtr        sap,
                     pre;

  if (sep != NULL && sep->data.ptrvalue) {
     if (IS_Bioseq(sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp!=NULL) {
           pre=NULL;
           sap=bsp->annot;
           while (sap!= NULL)
           {
              if (sap->type == 2) {
                 if (is_dim1seqalign ((SeqAlignPtr) sap->data)) {
                    if (pre==NULL) {
                       bsp->annot=sap->next;
                       sap->next=NULL;
                       sap = SeqAnnotFree (sap);
                       sap=bsp->annot; 
                    }
                    else {
                       pre->next=sap->next;
                       pre=sap->next;
                       sap->next=NULL; 
                       sap = SeqAnnotFree (sap);
                       sap=pre;
                    }
                 }
                 else {
                    pre=sap;
                    sap=sap->next;
                 }
              }
              else {
                 pre=sap;
                 sap=sap->next;
              }
           }
        }
     }
     else if(IS_Bioseq_set(sep)) {
        bssp = (BioseqSetPtr)sep->data.ptrvalue;
        if (bssp!=NULL) {
           pre=NULL;
           sap=bssp->annot;
           while (sap!= NULL)
           {
              if (sap->type == 2) {
                 if (is_dim1seqalign ((SeqAlignPtr) sap->data)) {
                    if (pre==NULL) {
                       bssp->annot=sap->next;
                       sap->next=NULL;
                       sap = SeqAnnotFree (sap);
                       sap=bssp->annot; 
                    }
                    else {
                       pre=sap->next;
                       sap->next=NULL; 
                       sap = SeqAnnotFree (sap);
                       sap=sap->next;
                    }
                 }
                 else {
                    pre=sap;
                    sap=sap->next;
                 }
              }
              else {
                 pre=sap;
                 sap=sap->next;
              }
           }
        }
     }
  }
}

static SelEdStructPtr SetSeqInfo (SeqAlignPtr salp, SelEdStructPtr seq_info)
{
  SeqAlignPtr   salptmp;
  ValNodePtr    vnp = NULL;
  SeqLocPtr     slp;
  SeqIdPtr      sip;
  Int4          start,
                stop;
  Uint1         strand;
  Int2          offset;
  SelEdStructPtr sesp,
                tmp;

  if (salp==NULL)
     return NULL;
  for (salptmp=salp; salptmp!=NULL; salptmp=salptmp->next)
  {
     sip = SeqAlignId(salptmp, 0);
     if (sip != NULL)
     {
        offset = 0;
        for (; sip != NULL; sip = sip->next)
        {
         if ((tmp=is_sip_inseqinfo(sip, seq_info)) == NULL)
         {
           start = SeqAlignStart (salptmp, offset);
           stop = SeqAlignStop (salptmp, offset);
           strand = SeqAlignStrand (salptmp, offset);
           slp = SeqLocIntNew (start, stop, strand, sip);
           if (slp!=NULL) {
              sesp=new_seledstruct_fromseqloc(0,0,0,0,0,slp,NULL,NULL,0,0);
              seq_info=SelEdStructAdd(seq_info, sesp);
           }
         }
/***** ???????????????
         else if (offset>0) {
           tmp->offset++;
         }
***************/
         offset++;
        }
     }
  }
  return seq_info;
}


static void CheckSeqAlignInSeqEntry (BioseqPtr bsp)
{
  SeqEntryPtr      sep,
                   sep_head;
  Uint2            entityID;

  sep = SeqMgrGetSeqEntryForData (bsp);
  entityID = ObjMgrGetEntityIDForChoice (sep);
  sep_head = GetTopSeqEntryForEntityID (entityID);
  SeqEntryExplore (sep_head, NULL, CheckSeqAlignCallback);
  return;
}


static EditAlignDataPtr BioseqToEditAlignData (EditAlignDataPtr adp, BioseqPtr bsp, Uint1 showfeature)
{
  SeqEntryPtr      sep;
  ValNodePtr       vnp;
  SeqLocPtr        slp;
  SeqAlignPtr      salp;
  SeqAnnotPtr      sap;
  AlignNodePtr     anp;

  if (adp == NULL)
      return NULL;
  if (showfeature)  {
     adp->showfeat = TRUE;
     getcurrentfeatstyle (adp);
  }  
  adp->input_format = OBJ_BIOSEQ;
  sep = SeqEntryNew ();
  if ( sep == NULL )  {
     MemFree (adp);
     return NULL;
  }
  sep->choice = 1;           
  sep->data.ptrvalue = (Pointer) bsp;
  vnp = SeqEntryToSeqLoc (sep, &(adp->seqnumber), bsp->mol);
  if (vnp == NULL) { 
     MemFree (adp);
     return NULL;
  }
  adp->sqloc_list = vnp;
  adp->bsqloc_list = SeqEntryToSeqLoc (sep, &(adp->seqnumber), bsp->mol);
  adp->size_labels = getsize_seqids (adp->sqloc_list, adp->printid);
  adp->params = SetupOptions  (adp->seqnumber);
  adp->length = MaxLengthSeqLoc (adp->sqloc_list);
  salp = SeqLocToFastaSeqAlign (adp->sqloc_list);
  sap = SeqAnnotForSeqAlign (salp);
  slp = (SeqLocPtr) vnp->data.ptrvalue;
  adp->master.region = (Pointer) SeqLocFromSeqAlign (salp, NULL);
  if ( adp->master.region == NULL ) {
     MemFree (adp);
     return NULL;
  }
  adp->master.regiontype = 1;
  adp->caret.regiontype = 1;
  adp->caret.region = SeqLocIntNew (0, 0, Seq_strand_plus, SeqLocId ((SeqLocPtr)adp->master.region));

  adp->mol_type = bsp->mol;
  if (ISA_aa(adp->mol_type)) {
     adp->colorRefs[COLOR_SELECT] = GetColorRGB(255, 255, 0);
  }
  else {
     adp->colorRefs[COLOR_SELECT] = GetColorRGB(0, 0, 0);
  }
  if (adp->display_panel==0 || adp->display_panel==1 || adp->display_panel==2) 
  {
     adp->anp_list = (ValNodePtr) CollAlignFromSeqAnnot (sap, (SeqLocPtr) adp->master.region, adp->featOrder, adp->groupOrder, COLLECT_MD, FALSE, FALSE, FALSE);
  } 
  else if (adp->display_panel == 3) {
     adp->anp_list = (ValNodePtr) CollAlignFromSeqAnnot (sap, (SeqLocPtr) adp->master.region, adp->featOrder, adp->groupOrder, COLLECT_MP, TRUE, FALSE, FALSE);
  }
  if (adp->display_panel==2) {
     adp->anp_graph = (ValNodePtr) CollAlignFromSeqAnnot (sap, (SeqLocPtr)adp->master.region, adp->featOrder, adp->groupOrder, COLLECT_MP, TRUE, FALSE, FALSE);
  }
  clean_annot_for_anp (&(adp->anp_list));
  if ( adp->anp_list ==NULL ) {
     MemFree (adp);
     return NULL;
  }
  if (showfeature) {
     adp->seqfeat = SeqfeatlistFree (adp->seqfeat);
     for (vnp = adp->anp_list; vnp != NULL; vnp = vnp->next) {
        anp = (AlignNodePtr)vnp->data.ptrvalue;
        if (anp!=NULL && anp_has_feature(anp)) {
           slp = CollectSeqLocFromAlignNode (anp);
           adp->seqfeat=CollectFeatureForEditor (slp, adp->seqfeat, anp->seq_entityID, anp->bsp_itemID, adp->featOrder, FALSE);
        }
     }
  }
  adp->gr.left = 0;
  adp->gr.right = adp->length;
  adp->sap_align = SeqAnnotDenseSegToBoolSeg (sap);
  adp->sap_original = sap;
  sep->data.ptrvalue = NULL;
  SeqEntryFree (sep);  
  adp = SetupDataBuffer (adp);
  adp = SetupDataPanel  (adp);
  if (adp!=NULL) 
  {
     adp->seg_bioseq = is_segbioseq (bsp);
     CheckSeqAlignInSeqEntry (bsp);
     adp->seq_info = SetSeqInfo ((SeqAlignPtr)adp->sap_align->data, seq_info);
  }
  return adp;  
}

/***************************************************************
*** SeqAlignToEditAlignData
*** SetupAlignDataSap
******************************************************************/

static SeqAlignPtr SeqAlignList2PosStrand (SeqAlignPtr salp)
{
  SeqAlignPtr salptmp;
  Int4Ptr     lenp;
  Int4Ptr     startp;
  Uint1Ptr    strandp;
  Int4        numseg;
  Int2        dim;
  Int4        j, k, n, tmp;


for (salptmp=salp; salptmp!=NULL; salptmp=salptmp->next)
{
  if ((SeqAlignStrand(salptmp,0) == Seq_strand_minus)
   && (SeqAlignStrand(salptmp,1) != Seq_strand_minus))
  {
     if (salptmp->segtype == 2)
     {
         DenseSegPtr dsp;
        dsp = (DenseSegPtr) salptmp->segs;
        strandp = dsp->strands;
        numseg = dsp->numseg;
        dim = dsp->dim;
        lenp = dsp->lens;
        startp = dsp->starts;
        for (j=0; j < numseg*dim && strandp!=NULL; j++, strandp++)
            {
                if (*strandp == Seq_strand_minus)
                    *strandp = Seq_strand_plus;
                else if (*strandp != Seq_strand_minus)
                    *strandp = Seq_strand_minus;
            }
        for (j=0, k=numseg-1; j<numseg/2; j++, k--) {
            tmp=lenp[j];
            lenp[j]=lenp[k];
            lenp[k]=tmp;
        }
        for (j=0, k=(dim*numseg-dim); j<(dim*numseg-1)/2; j+=dim, k-=dim) {
            for (n=0; n<dim; n++) {
                tmp=startp[j+n];
                startp[j+n]=startp[k+n];
                startp[k+n]=tmp;
            }
        }
     }
  }
}
return salp;
}

static Boolean SetupAlignDataSap (EditAlignDataPtr adp, SeqAlignPtr salp_original, SeqIdPtr mastersip)
{
  SeqAnnotPtr  sap, 
               saptmp;
  SeqAlignPtr  newsalp, 
               salp,
               salp1;

  Uint2        entityID_sap;

  if (adp == NULL || salp_original == NULL)
     return FALSE;
  /*************************************/
  /** check if all ->type are NOT 0
  *** if all 0 -> all cached from CN3D viewer
  **************/
  for (newsalp = salp_original; newsalp!=NULL; newsalp=newsalp->next)
     if (newsalp->type > 0)
        break;
  if (newsalp== NULL)
     return FALSE;
  /**************************************/
  if ( salp_original->segtype == 1 ) 
  {
     adp->blocks = create_list_alignedsegs (salp_original);
     salp = DenseDiagToDenseSeg (salp_original, TRUE);
     adp->sap1_original = SeqAnnotForSeqAlign (salp_original);
  }
  else
     salp = salp_original;

  if ( salp->segtype == 2 ) 
  {
/*************/
         adp->sap_original = SeqAnnotForSeqAlign (salp);
/************/
         saptmp = SeqAnnotForSeqAlign (salp);
         sap = (SeqAnnotPtr) AsnIoMemCopy (saptmp, (AsnReadFunc) SeqAnnotAsnRead, (AsnWriteFunc) SeqAnnotAsnWrite); 
         saptmp->data=NULL;
         saptmp = SeqAnnotFree(saptmp);
         if (sap == NULL) 
         {
            return FALSE;
         }
         salp = (SeqAlignPtr)sap->data;


/*       if (is_dim2seqalign(salp)) || (salp->dim==2 && adp->display_panel==1)) 
*/
{{
salp = SeqAlignList2PosStrand(salp);
}}
         if (is_dim2seqalign (salp))
         {
            salp1 = (SeqAlignPtr) sap->data;
            sap->data=NULL;
            sap = SeqAnnotFree(sap);
/*
            salp1 = SortSeqAlign (&salp1);
*/
            sap = multseqalign_from_pairseqalign (salp1);
            SeqAlignSetFree (salp1);
            if (sap == NULL) 
            {
               return FALSE;
            }
         }
         newsalp = (SeqAlignPtr) sap->data;
         adp->seqnumber = newsalp->dim;
         adp->sqloc_list = SeqLocListFromSeqAlign (newsalp);
         adp->bsqloc_list = SeqLocListOfBioseqsFromSeqAlign (newsalp);
         adp->printid = PRINTID_TEXTID_ACCESSION;
         adp->size_labels = getsize_seqids (adp->sqloc_list, adp->printid);
         adp->params = SetupOptions  (adp->seqnumber);

         entityID_sap=ObjMgrRegister (OBJ_SEQANNOT, (Pointer)sap);

         adp->master.region=(Pointer)SeqLocFromSeqAlign(newsalp, mastersip);
         if (adp->master.region == NULL ) {
            adp->master.region=(Pointer)SeqLocFromSeqAlign(newsalp,NULL);
         }
         adp->master.regiontype = 1;
         if (adp->display_panel==0 || adp->display_panel==1 || adp->display_panel==2) 
         {
            adp->anp_list = (ValNodePtr) CollAlignFromSeqAnnot (sap, (SeqLocPtr) adp->master.region, adp->featOrder, adp->groupOrder, COLLECT_MD, FALSE, FALSE, FALSE);  } 
         else if (adp->display_panel == 3) 
         {
            adp->anp_list = (ValNodePtr) CollAlignFromSeqAnnot (sap, (SeqLocPtr) adp->master.region, adp->featOrder, adp->groupOrder, COLLECT_MP, TRUE, FALSE, FALSE);
         } 
         if (adp->display_panel==2) {
            adp->anp_graph = (ValNodePtr) CollAlignFromSeqAnnot (sap, (SeqLocPtr)adp->master.region, adp->featOrder, adp->groupOrder, COLLECT_MP, TRUE, FALSE, FALSE);
         } 
         clean_annot_for_anp(&(adp->anp_list));
         if ( adp->anp_list == NULL ) {
            adp->seqnumber = 0;
            return FALSE;
         }
         adp->length = GetAlignLengthFromAlignNode ((AlignNodePtr) adp->anp_list->data.ptrvalue);
         adp->gr.left = 0;
         adp->gr.right = adp->length; 
         salp1 = SeqAlignDenseSegToBoolSeg (newsalp);

         adp->sap_align = SeqAnnotForSeqAlign (salp1);
         adp->mol_type = getmoltype (SeqLocId((SeqLocPtr)adp->master.region));
         if (ISA_aa(adp->mol_type)) 
         {
            adp->colorRefs[COLOR_SELECT] = GetColorRGB(255, 255, 0);
         }
         else {
            adp->colorRefs[COLOR_SELECT] = GetColorRGB(0, 0, 0);
         }
         sap = SeqAnnotFree (sap);
         ObjMgrDelete (OBJ_SEQANNOT, (Pointer)sap);
  }
  return TRUE;
}

static void SeqAlignBioseqRegister (SeqAlignPtr salp, Uint2 *entityID, Uint2 *itemID)
{
  SeqAlignPtr tmp;
  SeqIdPtr    sip;
  Int2        j;
  Uint2       eID=0, iID=0;
  Boolean     first=TRUE;

  for (tmp=salp; tmp!=NULL; tmp=tmp->next)
  {
     for (j=0; j<tmp->dim; j++)
     {
        sip=SeqAlignId (tmp, j);
        eID = BioseqFindEntity (sip, &iID);
        if (first) {
           *entityID = eID;
           *itemID = iID;
           first = FALSE;
        }
     }
  }
}

static EditAlignDataPtr SeqAlignToEditAlignData (EditAlignDataPtr adp, SeqAlignPtr salp, Int4 start, SeqIdPtr mastersip, Uint1 showfeature)
{
  AlignNodePtr     anp;
  SeqLocPtr        slp;
  SeqIdPtr         sip;
  ValNodePtr       vnp;
  BioseqPtr        bsp;
  Boolean          ok=FALSE;
 
  if (adp == NULL || salp == NULL) 
     return NULL;
  if (SeqAlignMolType (salp) == 0)
     return NULL;
  if (salp!=NULL) 
  {
     SeqAlignBioseqRegister (salp, &(adp->master.entityID), &(adp->master.itemID));
     adp->master.itemtype = OBJ_BIOSEQ;
     if (showfeature) 
     {
        adp->showfeat = TRUE;
        getcurrentfeatstyle (adp);
     }  
     adp->input_format = OBJ_SEQALIGN;
     ok = SetupAlignDataSap (adp, salp, mastersip);
  }
  if (!ok) {
     adp->input_format = OBJ_BIOSEQ;
     bsp = BioseqLockById (mastersip);
     if (bsp) {
        adp = BioseqToEditAlignData (adp, bsp, showfeature);
        ok = (Boolean) (adp!=NULL);
        if (adp!=NULL) 
           adp->sap_original = SeqAnnotForSeqAlign (salp);
        BioseqUnlock (bsp);
     }
  } 
  if (ok) 
  {
        if (showfeature == SEQ_FEAT1 && mastersip != NULL) {
           adp->seqfeat = SeqfeatlistFree (adp->seqfeat);
           for (vnp = adp->anp_list; vnp != NULL; vnp = vnp->next) {
              anp = (AlignNodePtr)vnp->data.ptrvalue; 
              if (anp!=NULL) {
                 for (sip=mastersip; sip!=NULL; sip=sip->next) {
                    if (SeqIdForSameBioseq(anp->sip, sip)) {
                       if(anp_has_feature(anp)) {
                          slp = CollectSeqLocFromAlignNode (anp);
                          adp->seqfeat=CollectFeatureForEditor (slp, adp->seqfeat, anp->seq_entityID, anp->bsp_itemID, adp->featOrder, FALSE);
                          break;
                       }
                    }
                 }
              } 
           }  
        }
        else if (showfeature) { 
           adp->seqfeat = SeqfeatlistFree (adp->seqfeat);
           for (vnp = adp->anp_list; vnp != NULL; vnp = vnp->next) {
              anp = (AlignNodePtr)vnp->data.ptrvalue; 
              if (anp!=NULL) {
                 if (anp_has_feature(anp)) {
                    slp = CollectSeqLocFromAlignNode (anp);
                    adp->seqfeat=CollectFeatureForEditor (slp, adp->seqfeat, anp->seq_entityID, anp->bsp_itemID, adp->featOrder, FALSE);
                 }
              } 
           }  
        }
        adp->caret.regiontype = 1;
        adp->caret.region = SeqLocIntNew (start, start, Seq_strand_plus, SeqLocId((SeqLocPtr)adp->master.region));
        adp = SetupDataBuffer (adp);
        adp = SetupDataPanel  (adp);
        if (adp!=NULL && adp->seqnumber > 1) {
           adp->draw_scale = TRUE;
           adp->draw_bars = TRUE;
           adp->marginwithindex = FALSE;
           adp->marginwithIds = TRUE;
           adp->marginwithgroup = FALSE;
        }
        if (adp!=NULL) {
           adp->seq_info = SetSeqInfo (salp, seq_info);
           return adp;
        }
  }
  adp = EditAlignDataFree (adp);
  return NULL;
}

extern void SalsaPanelHasResized (PaneL pnl)
{
  EditAlignDataPtr adp; 
  RecT             rp;
 
  GetPanelExtra (pnl, &adp); 
  if (adp != NULL) { 
     get_client_rect (pnl, &rp);
     do_resize_panel (pnl, adp, (Int2)(rp.right - rp.left), (Int2)(rp.bottom - rp.top), TRUE);
  } 
  SetPanelExtra (pnl, &adp); 
}

extern Boolean FindIdsInSalsaPanel (PaneL pnl, PoinT pt, Uint2 *entityID, Uint2 *itemID, Uint2 *itemtype)
{
  EditAlignDataPtr adp;
  RecT             rp;
  Int4             pos;
  Int2             line;
  Uint2            iID, eID, itype, isubtype;
  Uint1            what;
  SeqAlignPtr      salp;

  *entityID = 0; 
  *itemID = 0; 
  *itemtype = 0; 
  GetPanelExtra (pnl, &adp);
  if (adp != NULL) {
     get_client_rect (pnl, &rp);
     what = locate_point (pt, rp, &iID, &eID, &itype, &isubtype, &pos, &line, adp);
     if (what == SIDLAND || (what == SEQLAND && adp->seqnumber == 1)) {
        if (eID) {
           *entityID = eID;
           *itemID = iID;
           *itemtype = itype;
           return TRUE;
        }
     }
     if (what == SEQLAND) {
        salp = SeqAlignBoolSegToDenseSeg((SeqAlignPtr)adp->sap_align->data);
        *entityID = ObjMgrRegister (OBJ_SEQALIGN, (Pointer) salp);
        *itemID = 1;
        *itemtype = OBJ_SEQALIGN;
        return TRUE;
     }
  }
  return FALSE;  
}

extern void SaveSalsaPanel (PaneL pnl)
{
  Char             namep [PATH_MAX];
  EditAlignDataPtr adp;
  Int4             start, stop;
  FILE             *fout;

  if (!GetOutputFileName (namep, PATH_MAX, NULL))
     return;
  GetPanelExtra (pnl, &adp);
  if (adp != NULL) {
     fout = FileOpen (namep, "w");
     if (fout != NULL) {
        WatchCursor ();
        start = 0;
        stop = adp->length -1;
        ShowAlignmentText (fout, adp, NULL, adp->marginleft, start, stop, FALSE);
        ArrowCursor ();
        FileClose (fout);
     }
  }
}

static void ResetSalsaTextPanel (PaneL pnl) 
{ 
  EditAlignDataPtr adp; 
 
  GetPanelExtra (pnl, &adp); 
  if (adp != NULL) { 
    adp = EditAlignDataFree (adp); 
  } 
  SetPanelExtra (pnl, &adp); 
} 
 
extern PaneL SalsaTextPanel (GrouP g, Int2 wid, Int2 hgt)
{
  PaneL pnl;
  pnl = AutonomousPanel4 (g, wid, hgt, on_draw, VscrlProc, NULL,
                         sizeof (EditAlignDataPtr), ResetSalsaTextPanel, NULL);
  return pnl;
}

extern void PopulateSalsaPanel (PaneL pnl, SeqEntryPtr sep, Boolean all_seq, Uint1 sequence_shown, Uint1 show_feat, Uint1 numbering, FonT font)
{
  EditAlignDataPtr adp = NULL;
  BioseqPtr        bsp = NULL;
  SeqAlignPtr      salp = NULL;
  RecT             rp;
  Int2             height, width;
  Uint2            entityID, itemID;
  SeqEditViewProcsPtr  svpp;

  if (sep == NULL)
     return;
  get_client_rect (pnl, &rp);
  width = (rp.right -rp.left);
  height = (rp.bottom -rp.top);

  if (all_seq || sequence_shown != SEQ_SHOW1)
     salp = (SeqAlignPtr) FindSeqAlignInSeqEntry (sep, OBJ_SEQALIGN);
 
  if (salp==NULL && IS_Bioseq(sep)) {
     bsp = (BioseqPtr) sep->data.ptrvalue;
     if (bsp != NULL) {
        adp = EditAlignDataInit(NULL,width,height,MARGINLEFT15,10,font, TRUE, 1);
        adp->Cn3D_On = FALSE;
        svpp = (SeqEditViewProcsPtr) GetAppProperty ("SeqEditDisplayForm");
        if (svpp) 
           adp->Cn3D_On =svpp->Cn3D_On;

        adp = BioseqToEditAlignData (adp, bsp, show_feat);
     }
  }
  if (salp!=NULL) {
     get_client_rect (pnl, &rp);
     adp =EditAlignDataInit(NULL,width,height, MARGINLEFT15, 10, font, TRUE, 1);
     adp->Cn3D_On = FALSE;
     svpp = (SeqEditViewProcsPtr) GetAppProperty ("SeqEditDisplayForm");
     if (svpp) 
        adp->Cn3D_On =svpp->Cn3D_On;

     if (IS_Bioseq(sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        adp = SeqAlignToEditAlignData (adp, salp, 0, bsp->id, show_feat);
     }
     else {
        adp = SeqAlignToEditAlignData (adp, salp, 0, NULL, show_feat);
     }
  }
  if (adp != NULL)
  {
     adp->font = (Handle)(font);
     adp->draw_emptyline = FALSE;
     SetPanelExtra (pnl, &adp);
     adp->marginwithindex = FALSE;
     adp->marginwithfeatid = TRUE;
     adp->all_sequences = all_seq;
     if (!all_seq) {
        adp->charmode = TRUE;
        if (sequence_shown>SEQ_NUM1 && IS_Bioseq(sep)) {
           entityID = ObjMgrGetEntityIDForChoice (sep);
           bsp = (BioseqPtr) sep->data.ptrvalue;
           BioseqFindEntity (bsp->id, &itemID);
           adp->master.entityID = entityID;
           adp->master.itemID = itemID;
        }
     }
     if (numbering == SEQ_NUM1) {
        adp->draw_scale = FALSE;
        adp->draw_bars = FALSE;
        adp->marginwithpos = TRUE;
        adp->marginwithgroup = FALSE;
     }
     else if (numbering == SEQ_NUM2) {
        adp->draw_scale = TRUE;
        adp->draw_bars = TRUE;
        adp->marginwithpos = TRUE;
        adp->marginwithgroup = FALSE;
     }
     else {
        adp->draw_scale = FALSE;
        adp->draw_bars = FALSE;
        adp->marginwithpos = FALSE;
        adp->marginwithgroup = FALSE;
     }
     do_resize_panel(pnl,adp, width, height, TRUE);
     checkselectsequinfeature_for_editor (adp->seqfeat);
  }
}


extern EditAlignDataPtr EditAlignDataRepopulateFromSeqAlign (PaneL pnl, EditAlignDataPtr adp, SeqAlignPtr salp)
{
  EditAlignDataPtr adpnew;
  RecT rp;
  float hratio;

  get_client_rect (pnl, &rp);
  adpnew = EditAlignDataReset (adp, (rp.right-rp.left), (rp.bottom-rp.top), salp);
  if (adpnew == NULL) {
     return adp;
  } 
  adp = adpnew;
  do_resize_window  (pnl, adp, TRUE);
  reset_features (pnl, adp);
  hratio = (float)adp->hoffset / (float)adp->length;
  SeqEdSetCorrectBarMax (pnl, adp, hratio);
  adp->dirty = TRUE;
  return adp;
}

static SeqEditViewFormPtr SeqEditViewFormNew (void)
{
  SeqEditViewFormPtr wdp;

  wdp = (SeqEditViewFormPtr) MemNew (sizeof (SeqEditViewForm));
  wdp->pnl = NULL;
  wdp->data = NULL;
  wdp->graph = NULL;

  wdp->pos = NULL;
  wdp->gototxt = NULL;
  wdp->lookattxt = NULL;
  wdp->gotobt = NULL;
  wdp->lookatbt = NULL;

  wdp->rfitem[0] = NULL;  wdp->rfitem[1] = NULL; wdp->rfitem[2] = NULL;
  wdp->rfitem[3] = NULL;  wdp->rfitem[4] = NULL; wdp->rfitem[5] = NULL;
  wdp->rfitem[6] = NULL;  wdp->rfitem[7] = NULL; wdp->rfitem[8] = NULL;
  wdp->rfitem[9] = NULL; 
  wdp->btngp = NULL;
  wdp->editmodeitem = NULL;
  wdp->viewmodeitem=NULL;
  wdp->showfeatbt = NULL;
  wdp->showfeatitem = NULL;
  wdp->hidefeatitem = NULL;
  wdp->translatebt = NULL;
  wdp->translateitem = NULL;
  wdp->codonstitem = NULL;
  wdp->savefeatbt = NULL;
  wdp->savefeatitem = NULL;
  wdp->refreshbt = NULL;
  wdp->closebt = NULL;
  wdp->svclosebt = NULL;
  wdp->cutitem = NULL;
  wdp->delitem = NULL; 
  wdp->copyitem = NULL; 
  wdp->pasteitem = NULL;
  wdp->selmaster = NULL;
  wdp->showdifitem = NULL;
  wdp->showallitem = NULL;

  wdp->extended_align_menu = FALSE;
  wdp->extended_dist_menu = FALSE;
  wdp->extended_tree_menu = FALSE;

  return wdp;
}   
/********************************************
***    Create new Window 
***
***
***********************************************/
static ForM CC_CreateBioseqViewForm (Int2 left, Int2 top, CharPtr windowname, BioseqPtr bsp, Pointer dummy)
{
  SeqEditViewProcsPtr  svpp;
  WindoW             w;
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr   adp;
  PaneL              pnl;
  GrouP              g;
  RecT               rw,    /* window rect */
                     rp,    /* panel rect  */
                     rb;    /* button rect */
  PopuP              pop;
  Char               str [16];
  SeqEntryPtr        sep = NULL;
  SelStructPtr       ssp = NULL;
  Int2               window_width = 650;
  Int2               wid, 
                     window_hgt = 300,
                     hgt,
                     charwidth, 
                     lineheight;
  Uint1              display_panel = 0;
  Uint1              showfeat = 0;
  FonT               font;
  Boolean            is_prot = FALSE,
                     bool;
  
  if (bsp==NULL)
     return NULL;
  is_prot = (Boolean)(ISA_aa(bsp->mol));
  wdp = SeqEditViewFormNew ();
  if (wdp==NULL)
     return NULL;
  
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
  lineheight = LineHeight ();

  w = DocumentWindow (left, top, (Int2)(-10), (Int2)(-10),  windowname, NULL, ResizeAlignDataWindow);
  SetObjectExtra (w, (Pointer) wdp, CleanupSequenceEditorForm);
  wdp->form = (ForM) w;
  wdp->formmessage = SalsaFormMessage;
  svpp = (SeqEditViewProcsPtr) GetAppProperty ("SeqEditDisplayForm");
  if (svpp != NULL) {
      SetActivate (w, svpp->activateForm);
      wdp->appmessage = svpp->handleMessages;
      wdp->extended_align_menu = svpp->extended_align_menu;
      wdp->extended_dist_menu = svpp->extended_dist_menu;
      wdp->extended_tree_menu = svpp->extended_tree_menu;
      showfeat = svpp->showfeat;
      if (svpp->minPixelWidth > 0) {
         window_width = svpp->minPixelWidth;
         if (window_width > 800) window_width = 800;
      }
      if (svpp->minPixelHeight > 0) {
         window_hgt = svpp->minPixelHeight;
         if (window_hgt > 300) window_hgt = 300;
      }
  }
  wid = (Int2)(window_width/charwidth);
  hgt = (Int2)(window_hgt / lineheight);

  if (dummy != NULL) {
     ssp = (SelStructPtr) dummy;    
     if (ssp->regiontype>0)
        display_panel = ssp->regiontype;
  }
  adp = EditAlignDataInit (NULL, wid, hgt, MARGINLEFT25, 10, font, FALSE, display_panel);
  adp->Cn3D_On = FALSE;
  if (svpp) 
     adp->Cn3D_On =svpp->Cn3D_On;
  adp = BioseqToEditAlignData (adp, bsp, showfeat);
  if (adp==NULL)
     return NULL;
  if (dummy!=NULL) {
     ssp = (SelStructPtr) dummy;    
     adp->input_entityID = ssp->entityID;;
     adp->input_itemID = ssp->itemID;
     adp->input_itemtype = ssp->itemtype;
  }
  else {
     adp->input_entityID = 0;
     adp->input_itemID = 0;
     adp->input_itemtype = 0;
  }
  adp->wdp = (Pointer) wdp;
  if (svpp == NULL) {
     SetupMenus (w, FALSE, is_prot, TRUE, FALSE, FALSE, FALSE);
  }
  else 
  {
     adp->edit_mode = (Uint1)(!svpp->viewer_mode);
     bool = (Boolean)(!svpp->viewer_mode);
     SetupMenus (w, FALSE, is_prot, bool, svpp->extended_dist_menu, svpp->extended_align_menu, svpp->extended_tree_menu);
  }
  if (!is_prot) 
  {
     g = HiddenGroup (w, 5, 0, NULL);                  
     sprintf (str, "%ld", (long) adp->caret_orig);
     wdp->gotobt = PushButton (g, "Go to:", GoToButton);
     wdp->gototxt = DialogText (g, str, (Int2)6, NULL);
     wdp->lookatbt = PushButton (g, "Look at:", LookAtButton);
     wdp->lookattxt = DialogText (g, str, (Int2)6, NULL);
     pop = PopupList (g, TRUE, SelectFeatEditMode);
     PopupItem (pop, "Merge feature mode"); 
     PopupItem (pop, "Split feature mode"); 
     SetValue (pop, 1);
  }
  else {
     g = HiddenGroup (w, 4, 0, NULL);                  
     sprintf (str, "%ld", (long) adp->caret_orig);
     wdp->gotobt = PushButton (g, "Go to:", GoToButton);
     wdp->gototxt = DialogText (g, str, (Int2)6, NULL);
     wdp->lookatbt = PushButton (g, "Look at:", LookAtButton);
     wdp->lookattxt = DialogText (g, str, (Int2)6, NULL);
  }  
  g = HiddenGroup (w, 1, 0, NULL);                  
  wdp->pos = StaticPrompt (g, "", window_width, dialogTextHeight, programFont, 'l');
  
  g = HiddenGroup (w, 1, 0, NULL);
  pnl = AutonomousPanel4 (g, window_width, window_hgt, on_draw, VscrlProc, 
                            NULL, sizeof (EditAlignDataPtr), NULL, NULL);
  SetPanelExtra (pnl, &adp);
  SetObjectExtra (pnl, (Pointer) adp, CleanupAlignDataPanel);
  wdp->pnl = pnl;

  if (!is_prot) 
  {
     wdp->btngp = HiddenGroup (w, 6, 0, NULL);
     wdp->showfeatbt= PushButton (wdp->btngp, "Show Feat.", ShowFeatureButtonProc);
     wdp->translatebt = PushButton (wdp->btngp, "Translate", TranslateButton);
     wdp->savefeatbt = PushButton (wdp->btngp, "Save CDS", SaveFeatureButton);
     wdp->refreshbt = PushButton (wdp->btngp, "Refresh", RefreshAlignDataButton);
     wdp->svclosebt = PushButton (wdp->btngp, "Accept", AcceptCloseWindowButton);
     wdp->closebt = PushButton (wdp->btngp, "Cancel", CloseWindowButton);
  }
  update_sequenceeditormenu (wdp, adp);

  RealizeWindow (w);

  ObjectRect (w, &rw);
  GetPosition (pnl, &rp);
  adp->xoff = rp.left;
  adp->yoff = rp.top;
  adp->x = (rw.right -rw.left+1) - (rp.right -rp.left+1) - rp.left;
  adp->y = (rw.bottom-rw.top +1) - (rp.bottom-rp.top +1) - rp.top;

  if (!is_prot) {
     GetPosition (wdp->closebt, &rb);
     adp->ybutt = (rw.bottom-rw.top +1) - (rb.bottom-rb.top +1) - rb.top;
  }
  
  SetPanelClick(pnl, on_click, on_drag, on_hold, on_release);
  SetSlateChar ((SlatE) pnl, on_key);
  SetWindowTimer (w, on_time);
  do_resize_window (pnl, adp, TRUE);

  if (adp->showfeat) {
     adp->showfeat = FALSE;
     if (wdp->showfeatbt!=NULL)
        ShowFeatureButtonProc (wdp->showfeatbt);
     if (wdp->showfeatitem!=NULL)
        ShowFeatureItemProc (wdp->showfeatitem);
  }
  if (!adp->display_panel)
     to_update_prompt (pnl, &(adp->caret), (SeqAlignPtr) adp->sap_align->data, adp->sqloc_list, FALSE, adp->printid);
  
  if (adp->master.entityID > 0) {
     sep = GetTopSeqEntryForEntityID (adp->master.entityID);
     TmpNam (adp->tmpfile);
     seqentry_write (sep, adp->tmpfile);
  }
  return (ForM) w;
}

/********************************************
***
***    Create new Window from SeqAlign 
***
***********************************************/
static ForM CreateSeqAlignEditForm (Int2 left, Int2 top, CharPtr windowname, SeqAlignPtr salp, Pointer dummy)
{
  SeqEditViewProcsPtr  svpp;
  WindoW             w;
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr   adp;
  PaneL              pnl;
  GrouP              g;
  RecT               rw,    /* window rect */
                     rp,    /* panel rect  */
                     rb;    /* button rect */
  PopuP              pop;
  SeqEntryPtr        sep = NULL;
  SelStructPtr       ssp = NULL; 
  Char               str [16];
  Int4               start = 0;
  Int2               window_width = 650,
                     wid,
                     window_hgt = 300,
                     hgt,
                     charwidth, lineheight;
  Uint1              display_panel = 0;
  Uint1              showfeat = 0;
  Uint1              moltype = 0;
  FonT               font;
  Boolean            is_prot = FALSE,
                     bool;
  
  if (salp==NULL)
     return NULL;
  moltype = SeqAlignMolType(salp);
  if (moltype == 0)
     return NULL;
  is_prot = (Boolean)(ISA_aa(moltype));
  wdp = SeqEditViewFormNew ();
  if (wdp==NULL)
     return NULL;
 
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
  lineheight = LineHeight ();

  w = DocumentWindow (left, top, (Int2)(-10), (Int2)(-10),  windowname, NULL, ResizeAlignDataWindow);
  SetObjectExtra (w, (Pointer) wdp, CleanupSequenceEditorForm);
  wdp->form = (ForM) w;
  wdp->formmessage = SalsaFormMessage;
  svpp = (SeqEditViewProcsPtr) GetAppProperty ("SeqEditDisplayForm");
  if (svpp != NULL) {
      SetActivate (w, svpp->activateForm);
      wdp->appmessage = svpp->handleMessages;
      wdp->extended_align_menu = svpp->extended_align_menu;
      wdp->extended_dist_menu = svpp->extended_dist_menu;
      wdp->extended_tree_menu = svpp->extended_tree_menu;
      showfeat = svpp->showfeat;

/*** BECAUSE it crashes if tries to load the features  **/ 
/*** before getting the entityIDs of the sequences     **/
      showfeat = FALSE;
      if (svpp->minPixelWidth > 0) {
         window_width = svpp->minPixelWidth;
         if (window_width > 800) 
            window_width = 800;
      }
      if (svpp->minPixelHeight > 0) {
         window_hgt = svpp->minPixelHeight;
         if (window_hgt > 300) 
            window_hgt = 300;
      }
  }
  wid = (Int2)(window_width/charwidth);
  hgt = (Int2)(window_hgt / lineheight);

  if (dummy != NULL) {
     ssp = (SelStructPtr) dummy;    
     if (ssp->regiontype>0)
        display_panel = ssp->regiontype;
  }
  adp = EditAlignDataInit (NULL, wid, hgt, MARGINLEFT25, 10, font, FALSE, display_panel);
  adp->Cn3D_On = FALSE;
  if (svpp) 
     adp->Cn3D_On =svpp->Cn3D_On;
  if (dummy != NULL) {
     ssp = (SelStructPtr) dummy;    
     adp->input_entityID = ssp->entityID;
     adp->input_itemID = ssp->itemID;
     adp->input_itemtype = ssp->itemtype;
     if (ssp->region!=NULL)
        start = (Int4)SeqLocStart((SeqLocPtr)ssp->region);
  }
  else {
     adp->input_entityID = 0;
     adp->input_itemID = 0;
     adp->input_itemtype = 0;
  }
  adp = SeqAlignToEditAlignData (adp, salp, start, NULL, showfeat); 
  if (adp == NULL)
     return NULL;
  adp->wdp = (Pointer) wdp;
  if (svpp == NULL)
  {
     adp->edit_mode = ALIGN_EDIT;
     SetupMenus (w, TRUE, is_prot, FALSE, FALSE, FALSE, FALSE);
  }
  else 
  {
     if (svpp->viewer_mode)
        adp->edit_mode = SEQ_VIEW;
     else 
        adp->edit_mode = ALIGN_EDIT;
     bool = (Boolean)(!svpp->viewer_mode);
     SetupMenus (w, TRUE, is_prot, bool, svpp->extended_dist_menu, svpp->extended_align_menu, svpp->extended_tree_menu);
  }
  if (!is_prot) 
  {
     g = HiddenGroup (w, 5, 0, NULL);                  
     sprintf (str, "%ld", (long) adp->caret_orig);
     wdp->gotobt = PushButton (g, "Go to:", GoToButton);
     wdp->gototxt = DialogText (g, str, (Int2)6, NULL);
     wdp->lookatbt = PushButton (g, "Look at:", LookAtButton);
     wdp->lookattxt = DialogText (g, str, (Int2)6, NULL);
     pop = PopupList (g, TRUE, SelectFeatEditMode);
     PopupItem (pop, "Merge feature mode"); 
     PopupItem (pop, "Split feature mode"); 
     SetValue (pop, 1);
  }
  else {
     g = HiddenGroup (w, 4, 0, NULL);                  
     sprintf (str, "%ld", (long) adp->caret_orig);
     wdp->gotobt = PushButton (g, "Go to:", GoToButton);
     wdp->gototxt = DialogText (g, str, (Int2)6, NULL);
     wdp->lookatbt = PushButton (g, "Look at:", LookAtButton);
     wdp->lookattxt = DialogText (g, str, (Int2)6, NULL);
  }  

  g = HiddenGroup (w, 1, 0, NULL);
  wdp->pos = StaticPrompt (g, "", window_width, dialogTextHeight, programFont, 'l');

  g = HiddenGroup (w, 1, 0, NULL);
/************************* HORIZONTAL SCROLLBAR>>*****************
  if (is_prot) 
  {
     pnl = AutonomousPanel4 (g, window_width, window_hgt, on_draw, NULL, 
                            HscrlProc, sizeof (EditAlignDataPtr), NULL, NULL);
     adp->vscrollbar_mode = FALSE;
  }
  else
****************************************************************/
     pnl = AutonomousPanel4 (g, window_width, window_hgt, on_draw, VscrlProc, 
                            NULL, sizeof (EditAlignDataPtr), NULL, NULL);
  SetPanelExtra (pnl, &adp);
  SetObjectExtra (pnl, (Pointer) adp, CleanupAlignDataPanel);
  wdp->pnl = pnl;

  if (!is_prot) {
     wdp->btngp = HiddenGroup (w, 5, 0, NULL);
     wdp->showfeatbt= PushButton (wdp->btngp, "Show feat.", ShowFeatureButtonProc);
     wdp->translatebt = PushButton (wdp->btngp, "Translate", TranslateButton);
     wdp->savefeatbt = PushButton (wdp->btngp, "Save CDS", SaveFeatureButton);
     wdp->refreshbt = PushButton (wdp->btngp, "Refresh", RefreshAlignDataButton);
     wdp->closebt = PushButton (wdp->btngp, "Dismiss", CloseWindowButton);
  }
  update_sequenceeditormenu (wdp, adp);
  
  RealizeWindow (w);
  
  ObjectRect (w, &rw);
  GetPosition (pnl, &rp);
  adp->xoff = rp.left;
  adp->yoff = rp.top;
  adp->x = (rw.right -rw.left+1) - (rp.right -rp.left+1) - rp.left;
  adp->y = (rw.bottom-rw.top +1) - (rp.bottom-rp.top +1) - rp.top;

  if (!is_prot) {
     GetPosition (wdp->closebt, &rb);
     adp->ybutt = (rw.bottom-rw.top +1) - (rb.bottom-rb.top +1) - rb.top;
  }

  if (dummy != NULL) {
     adp->voffset = hoffset2voffset(adp, adp->anp_list, adp->visibleWidth, 0, adp->length-1, start);
  }
  SetPanelClick(pnl, on_click, on_drag, on_hold, on_release);
  SetSlateChar ((SlatE) pnl, on_key);
  SetWindowTimer (w, on_time);
  do_resize_window (pnl, adp, TRUE);
  if (dummy != NULL) {
     SeqEdCorrectBarValue (pnl, (Int2) adp->voffset);
  }

  if (adp->showfeat) {
     adp->showfeat = FALSE;
     if (wdp->showfeatbt!=NULL)
        ShowFeatureButtonProc (wdp->showfeatbt);
     if (wdp->showfeatitem!=NULL)
        ShowFeatureItemProc (wdp->showfeatitem);
  }
  if (!adp->display_panel)
     to_update_prompt (pnl, &(adp->caret), (SeqAlignPtr) adp->sap_align->data, adp->sqloc_list, FALSE, adp->printid);

  if (adp->master.entityID > 0) {
     sep = GetTopSeqEntryForEntityID (adp->master.entityID);
     TmpNam (adp->tmpfile);
     seqentry_write (sep, adp->tmpfile);
  }
  return (ForM) w;
}


/********************************************
***
***    Create new Window from SeqAlign 
***
***********************************************/

static void DoReplaceBtn (ButtoN b)
{
  WindoW             w;
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr   adp;
  SeqAnnotPtr        sap_copy;
  SeqAlignPtr        salp,
                     salp2;
  SeqIdPtr           target_id,
                     source_id;
  CompSegPtr         csp;
  PropaStructPtr     psp;
  BioseqPtr          target_bsp,
                     source_bsp;
  ValNodePtr         targetfeats = NULL;
  Uint2              master_entityID,
                     master_itemID,
                     master_itemtype;

  w = (WindoW)ParentWindow (b);
  wdp = (SeqEditViewFormPtr) GetObjectExtra (w);
  adp = (EditAlignDataPtr) wdp->data; 
  if (adp != NULL) {
     adp->stoptransl = 0;
     salp = (SeqAlignPtr) adp->sap_align->data;
     if (salp->segtype == COMPSEG) {
        csp = (CompSegPtr) salp->segs;
        if (csp->ids != NULL && csp->ids->next!=NULL) {
           target_id = SeqIdDup(csp->ids);
           source_id = SeqIdDup(csp->ids->next);
           master_entityID = adp->master.entityID;
           master_itemID = adp->master.itemID;
           master_itemtype = adp->master.itemtype;
           sap_copy = (SeqAnnotPtr) AsnIoMemCopy ((Pointer) adp->sap_original, (AsnReadFunc) SeqAnnotAsnRead, (AsnWriteFunc) SeqAnnotAsnWrite); 
           salp2 = SeqAlignBoolSegToDenseSeg (salp);
           psp = CreatePropaStruc (target_id, source_id, salp2);

           if (psp) {
              psp->gap_choice = IGNORE_GAP_CHOICE;
              psp->stoptransl = adp->stoptransl;
              psp->keep_protID = FALSE;
 
              Hide (w); 
              Update ();
              Remove (w);
              Nlm_RemoveDyingWindows ();

              target_bsp = BioseqLockById (target_id);
              if (target_bsp) {
SeqLocPtr target_slp;
                 target_slp = SeqLocIntNew (0, target_bsp->length-1, Seq_strand_plus, target_id);
                 targetfeats = CollectFeatureForEditor (target_slp, targetfeats, psp->target_entityID, psp->target_bsp_itemID, NULL, TRUE);
                 PropagateFeatureBySeqLock (psp->sap, psp->target_bsp_itemID, psp->source_entityID, psp->source_sep, targetfeats, psp->gap_choice);
                 BioseqUnlock (target_bsp);
/**** TODO:
free targetfeats **********************/
              }
              PropagateFeatureByApply (psp);
              psp->sap->data = NULL;
              psp->sap = SeqAnnotFree (psp->sap);
              psp->target_sep = NULL;
              psp->source_seqfeat = NULL;
              psp->target_seqfeat = NULL;
              MemFree(psp);

              target_bsp = BioseqLockById (target_id);
              source_bsp = BioseqLockById (source_id);
              if (target_bsp && source_bsp) {
                 target_bsp->seq_data = BSFree (target_bsp->seq_data);
                 target_bsp->seq_data = BSDup(source_bsp->seq_data);
                 target_bsp->seq_data_type = source_bsp->seq_data_type;
                 target_bsp->length = source_bsp->length;
                 BioseqRawPack (target_bsp);
                 BioseqUnlock (target_bsp);
                 ObjMgrSetDirtyFlag (master_entityID, TRUE);
                 ObjMgrSendMsg (OM_MSG_UPDATE, master_entityID, master_itemID, master_itemtype);
              }
           }
           SeqAlignSetFree (salp2);
           SeqIdFree (target_id);
           SeqIdFree (source_id);
           /**************************** Next Salp ****/
           salp=NULL;
           if (sap_copy) {
	      salp = (SeqAlignPtr)(sap_copy->data);
	      salp = salp->next;
           }
           if (salp)
              LaunchAlignViewer(salp);
        }
     }
  }
  return;
}

static Int2 what_seqalign (SeqAlignPtr salp)
{
  DenseSegPtr   dsp;
  CompSegPtr    csp;
  Int4          numseg;
  Int2          dim;
  Int4Ptr       startsi;
  BoolPtr       starts;
  Boolean       start1, start2, start3, start4;
  
  if (salp->segtype == 2)
  {
     dsp = (DenseSegPtr)salp->segs;
     startsi = dsp->starts;
     numseg = dsp->numseg;
     dim = dsp->dim;
     start1=(Boolean)(startsi[0]>-1);
     start2=(Boolean)(startsi[1]>-1);
     start3=(Boolean)(startsi[(numseg*dim)-2]>-1);
     start4=(Boolean)(startsi[(numseg*dim)-1]>-1);
  }
  else if (salp->segtype == COMPSEG) {
     csp = (CompSegPtr)salp->segs;
     starts = csp->starts;
     numseg = csp->numseg;
     dim = csp->dim;
     start1=starts[0];
     start2=starts[1];
     start3=starts[(numseg*dim)-2];
     start4=starts[(numseg*dim)-1];
  }
  else
     return 0;
  if (!start3) {
     if (start1)
        return 2;
     else if(!start2)
         return 4; /* extension to the right */
     return 5; /* HS XXX modify behavior of code.. now uses whole sequence for merge */
     return 3; /* Could merge either before or after .. by default it's after */
  } else if (!start1) { 
      /* start3 == TRUE */
     return 1;
  } else  if(start1 && start4) { /* start3==TRUE already */
      return 5; /* nothing new, just take sequence from new one */
  } else if (start1 && start3)
      return 6;
  return 0;
}

static Int4 SeqAlignFirstLength (SeqAlignPtr salp)
{
  Int4          len = 0;
  DenseSegPtr   dsp;
  CompSegPtr    csp;

  if (salp->segtype == 2)
  {
     dsp = (DenseSegPtr)salp->segs;
     len = (Int4)*(dsp->lens);
  }
  else if (salp->segtype == COMPSEG) {
     csp = (CompSegPtr)salp->segs;
     len = (Int4)*(csp->lens);
  }
  return len;
}

static Int4 SeqAlignLastLength (SeqAlignPtr salp)
{
  Int4          len = 0;
  DenseSegPtr   dsp;
  CompSegPtr    csp;

  if (salp->segtype == 2)
  {
     dsp = (DenseSegPtr)salp->segs;
     len = (Int4)dsp->lens[dsp->numseg-1];
  }
  else if (salp->segtype == COMPSEG) {
     csp = (CompSegPtr)salp->segs;
     len = (Int4)csp->lens[csp->numseg-1];
  }
  return len;
}

static Boolean get_bestpos_formerge (SeqAlignPtr salp, Int4Ptr from, Int4Ptr to)
{
  Int2          j;
  Boolean       insert_before=FALSE,
                insert_after=FALSE;
  Int4          from1, to1, from2, to2;
  BioseqPtr     bsp;
  SeqIdPtr      sip;

  *from = -1;
  *to = -1;
  if (salp == NULL)
     return FALSE;
  AlnMgrIndexSingleChildSeqAlign(salp);
  AlnMgrGetNthSeqRangeInSA(salp, 1, &from1, &to1);
  AlnMgrGetNthSeqRangeInSA(salp, 2, &from2, &to2);
  if (from1 == 0)
  {
     if (from2 != 0)
     {
        *from = 0;
        *to = from2 - 1;
     } else
     {
        *from = from2;
        *to = to2;
     }
  } else if (from2 == 0)
  {
     if (from1 != 0)
     {
        *from = to2 + 1;
        sip = AlnMgrGetNthSeqIdPtr(salp, 2);
        bsp = BioseqLockById(sip);
        if (bsp != NULL)
           *to = bsp->length -1;
        BioseqUnlock(bsp);
        SeqIdFree(sip);
     } else
     {
        *from = from2;
        *to = to2;
     }
  }
  *to+=1; /* create a bug to fix a bug -- SW */
  return; /* above fix put in 1/29/01 since coordinates were not correct -- SW */
  j=what_seqalign(salp);
  if (j==1 || j==3) 
  { /* when j==3, there is also a bit afterwards that could be merged.*/
     insert_before = TRUE;
     *from = SeqAlignStart(salp, 1);
     *to = *from + SeqAlignFirstLength (salp)-1; /* HS bug fix */
     return TRUE;
  }
  if (j==2 || j==4)
  {
     insert_after = TRUE;
     *from = SeqAlignStop (salp, 1) - SeqAlignLastLength (salp) + 1;
     *to = SeqAlignStop (salp, 1);
     return TRUE;
  }
  if (j==5 || j==6) { /* copy over the whole seqalign. .. nothing new */
      *from = SeqAlignStart(salp,1);
      *to = SeqAlignStop(salp,1);
  }
  return FALSE; 
}

static void DoMergeBtn (ButtoN b)
{
  WindoW             w;
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr   adp;
  SeqAnnotPtr        sap_copy;
  SeqAlignPtr        salp;
  CompSegPtr         csp;
  SeqIdPtr           target_id,
                     source_id,
                     merge_id;
  BioseqPtr          merge_bsp;
  ValNodePtr         sqloc_list=NULL;
  Char               str [32];
  Int4               from, to;
  Uint2              master_entityID,
                     master_itemID,
                     master_itemtype;
  Boolean            keep_protID=TRUE,
                     spliteditmode,
                     ok=FALSE;


  w = (WindoW)ParentWindow (b);
  wdp = (SeqEditViewFormPtr) GetObjectExtra (w);
  adp = (EditAlignDataPtr) wdp->data; 
  if (adp != NULL) {
     if (wdp->keep_protid1)
        keep_protID = GetStatus(wdp->keep_protid1);
     salp = (SeqAlignPtr) adp->sap_align->data;
     if (salp->segtype == COMPSEG) {
        csp = (CompSegPtr) salp->segs;
        if (csp->ids != NULL && csp->ids->next!=NULL) {
           target_id = SeqIdDup(csp->ids);
           source_id = SeqIdDup(csp->ids->next);
           master_entityID = adp->master.entityID;
           master_itemID = adp->master.itemID;
           master_itemtype = adp->master.itemtype;
           sap_copy = (SeqAnnotPtr) AsnIoMemCopy ((Pointer) adp->sap_original, (AsnReadFunc) SeqAnnotAsnRead, (AsnWriteFunc) SeqAnnotAsnWrite);
           GetTitle (wdp->fromtxt, str, sizeof (str));
           StrToLong (str, &from);
           from--;
           GetTitle (wdp->totxt, str, sizeof (str));
           StrToLong (str, &to);
           to--;
           merge_bsp = BioseqCopy (NULL, source_id, from, to, Seq_strand_plus, TRUE);

           adp->sap_align->data = NULL;
           sqloc_list = adp->sqloc_list;
           adp->sqloc_list = NULL;
           spliteditmode = adp->spliteditmode;

           Hide (w);
           Update ();

           if (merge_bsp) 
           {
              merge_id = merge_bsp->id;
              ok= MergeFunc (target_id, source_id, merge_id, salp, from, to, sqloc_list, spliteditmode, keep_protID);
              if (ok) {
                 ObjMgrSetDirtyFlag (master_entityID, TRUE);
                 ObjMgrSendMsg (OM_MSG_UPDATE, master_entityID, master_itemID, master_itemtype);
              }
           }
           SeqIdFree (target_id);
           SeqIdFree (source_id);
           Remove (w);
           /**************************** Next Salp ****/
           salp=NULL;
           if (sap_copy) {
              salp = (SeqAlignPtr)(sap_copy->data);
              salp = salp->next;
           }
           if (salp)
              LaunchAlignViewer(salp);
        }
     }
  }
  return;
}

static void DoCopyFeatBtn (ButtoN b)
{
  WindoW             w;
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr   adp;
  SeqAnnotPtr        sap_copy;
  SeqAlignPtr        salp,
                     salp2;
  CompSegPtr         csp;
  PropaStructPtr     psp;
  SeqIdPtr           target_id,
                     source_id;
  Uint2              entityID, itemID;
  Boolean            val = FALSE;
  
  w = (WindoW)ParentWindow (b);
  wdp = (SeqEditViewFormPtr) GetObjectExtra (w);
  adp = (EditAlignDataPtr) wdp->data; 
  if (adp != NULL) {
     if (wdp->gap_choicebn) {
        val = GetStatus(wdp->gap_choicebn);
        if (val)
           adp->gap_choice = TAKE_GAP_CHOICE;
        else
           adp->gap_choice = IGNORE_GAP_CHOICE;
     } 
     val = FALSE;
     if (wdp->keep_protid2)
        val = GetStatus(wdp->keep_protid2);
     adp->stoptransl = 0;
     sap_copy = (SeqAnnotPtr) AsnIoMemCopy ((Pointer) adp->sap_original, (AsnReadFunc) SeqAnnotAsnRead, (AsnWriteFunc) SeqAnnotAsnWrite); 
     salp = (SeqAlignPtr) adp->sap_align->data;
     if (salp->segtype == COMPSEG) {
        csp = (CompSegPtr) salp->segs;
        if (csp->ids != NULL && csp->ids->next!=NULL) {
           target_id = SeqIdDup (csp->ids);
           source_id = SeqIdDup (csp->ids->next);

           salp2 = SeqAlignBoolSegToDenseSeg (salp);
           psp = CreatePropaStruc (target_id, source_id, salp2);

           if (psp) {
              psp->gap_choice = adp->gap_choice;
              psp->stoptransl = adp->stoptransl;
              psp->keep_protID = val;

              Hide (w); 
              Update ();
              Remove (w);
              Nlm_RemoveDyingWindows ();

              PropagateFeatureByApply (psp);
              psp->sap->data = NULL;
              psp->sap = SeqAnnotFree (psp->sap);
              psp->target_sep = NULL;
              psp->source_seqfeat = NULL;
              psp->target_seqfeat = NULL;
              MemFree(psp);
           }

           entityID = BioseqFindEntity (target_id, &itemID);
           SeqAlignSetFree (salp2);
           ObjMgrSetDirtyFlag (entityID, TRUE);
           ObjMgrSendMsg (OM_MSG_UPDATE, entityID, itemID, OBJ_BIOSEQ);
           SeqIdFree (target_id);
           SeqIdFree (source_id);
           
           /**************************** Next Salp ****/
           salp=NULL;
           if (sap_copy) {
              salp = (SeqAlignPtr)(sap_copy->data);
              salp = salp->next;
           }
           /**************************** Next Salp ****/
           if (salp)
              LaunchAlignViewer (salp);
        }
     }
  }
  return;
}


static void LaunchAlignEdit (ButtoN b)
{
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr   adp;
  SeqAlignPtr        salp;

  wdp = (SeqEditViewFormPtr) GetObjectExtra ((WindoW)ParentWindow (b));
  adp = (EditAlignDataPtr) wdp->data;
  if (adp != NULL)
  {
     salp = (SeqAlignPtr)adp->sap_align->data;
     salp = SeqAlignBoolSegToDenseSeg (salp);
     if (salp != NULL)
        LaunchAlignEditor (salp);
  }
  return;
}

static void CloseThisWindowProc (ButtoN b)
{
  WindoW             w;  
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr   adp;

  w = (WindoW)ParentWindow (b);
  wdp = (SeqEditViewFormPtr) GetObjectExtra (w);
  adp = (EditAlignDataPtr) wdp->data;
  if (adp != NULL) {
     EditAlignDataFree (adp);
     wdp->data = NULL;
  }
  Hide (w);
  Update ();
  Remove (w);
}

static void SkipThisWindowProc (ButtoN b)
{
  WindoW             w;  
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr   adp;
  SeqAlignPtr        salp;

  w = (WindoW)ParentWindow (b);
  wdp = (SeqEditViewFormPtr) GetObjectExtra (w);
  adp = (EditAlignDataPtr) wdp->data;
  if (adp->sap_original) {
     salp = (SeqAlignPtr)(adp->sap_original->data);
     salp = salp->next;
  }
  if (adp != NULL) {
     EditAlignDataFree (adp);
     wdp->data = NULL;
  }
  Hide (w);
  Update ();
  Remove (w);
  if (salp)
     LaunchAlignViewer(salp);
}


/***************************************************************
***  switch_featOrder
***
*****************************************************************/
static void switch_featOrder (EditAlignDataPtr adp, Uint1 choice)
{
  Int2  oldstyle;
  Int2  j;
  Int4  groupNum;

  if (choice) {
     oldstyle = GetMuskCurrentSt ();
     SetMuskCurrentSt (GetMuskStyleName (adp->styleNum));
     for(j =0; j<FEATDEF_ANY; j++)
     {   
        adp->featOrder[j] = (Uint1)GetMuskCParam(j, MSM_FORDER, MSM_NUM);
        groupNum = (Uint1)GetMuskCParam(j, MSM_FGROUP, MSM_NUM);
        adp->groupOrder[j] = (Uint1)GetMuskCParam(MSM_GROUPS, (Int2)groupNum, MSM_NUM);
     } 
     SetMuskCurrentSt (GetMuskStyleName (oldstyle));
  }
  else
     for(j=0; j<FEATDEF_ANY; ++j) adp->featOrder[j] = choice;
}

static void ChangeRMCProc (GrouP g)

{
  Int2                val;
  SeqEditViewFormPtr  wdp;

  wdp = (SeqEditViewFormPtr) GetObjectExtra (g);
  if (wdp == NULL) return;
  val = GetValue (wdp->replaceMergeCopyGroup);
  switch (val) {
    case 1 :
      SafeHide (wdp->rmcExtra [1]);
      SafeHide (wdp->rmcExtra [2]);
      SafeShow (wdp->rmcExtra [0]);
      SafeEnable (wdp->acceptBtn);
      break;
    case 2 :
      SafeHide (wdp->rmcExtra [0]);
      SafeHide (wdp->rmcExtra [2]);
      SafeShow (wdp->rmcExtra [1]);
      SafeEnable (wdp->acceptBtn);
      break;
    case 3 :
      SafeHide (wdp->rmcExtra [0]);
      SafeHide (wdp->rmcExtra [1]);
      SafeShow (wdp->rmcExtra [2]);
      SafeEnable (wdp->acceptBtn);
      break;
    default :
      SafeHide (wdp->rmcExtra [1]);
      SafeHide (wdp->rmcExtra [0]);
      SafeHide (wdp->rmcExtra [2]);
      SafeDisable (wdp->acceptBtn);
      break;
  }
}

static void AcceptSeqAlignViewForm (ButtoN b)

{
  Int2                val;
  SeqEditViewFormPtr  wdp;

  wdp = (SeqEditViewFormPtr) GetObjectExtra (b);
  if (wdp == NULL) return;
  val = GetValue (wdp->replaceMergeCopyGroup);
  switch (val) {
    case 1 :
      DoReplaceBtn (b);
      break;
    case 2 :
      DoMergeBtn (b);
      break;
    case 3 :
      DoCopyFeatBtn (b);
      break;
    default :
      break;
  }
}

static ForM CreateSeqAlignViewForm (Int2 left, Int2 top, CharPtr windowname, SeqAlignPtr salp, Pointer dummy)
{
  VieweR             view = NULL;
  SeqEditViewProcsPtr  svpp;
  WindoW             w;
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr   adp;
  GrouP              g, g2, h;
  ButtoN             mergebtn;
  SelStructPtr       ssp; 
  Int4               start = 0,
                     stop;
  Int2               wid, hgt;
  ValNodePtr         vnp;
  AlignNodePtr       anp;

  Char strid1[16], strid2[16];
  Char str [64];
  Int2 j;

  wdp = (SeqEditViewFormPtr) MemNew (sizeof (SeqEditViewForm));
  w = FixedWindow (left, top, (Int2)(-10), (Int2)(-10),  windowname, NULL /*, ResizeAlignDataWindow */);
  SetObjectExtra (w, (Pointer) wdp, CleanupSequenceEditorForm);
  wdp->form = (ForM) w;
  wdp->formmessage = SalsaFormMessage;
  svpp = (SeqEditViewProcsPtr) GetAppProperty ("SeqEditDisplayForm");
  if (svpp != NULL) {
      SetActivate (w, svpp->activateForm);
      wdp->appmessage = svpp->handleMessages;
  }
  wdp->pnl = NULL;
  wdp->data = NULL;

  adp = EditAlignDataInit (NULL, 100, 12, MARGINLEFT25, 10, NULL, FALSE, 3);
  adp->Cn3D_On = FALSE;
  if (svpp) 
     adp->Cn3D_On =svpp->Cn3D_On;
  if (dummy != NULL) {
     ssp = (SelStructPtr) dummy;    
     adp->input_entityID = ssp->entityID;
     adp->input_itemID = ssp->itemID;
     adp->input_itemtype = ssp->itemtype;
     if (ssp->region!=NULL)
        start = (Int4)SeqLocStart((SeqLocPtr)ssp->region);
  }
  else {
     adp->input_entityID = 0;
     adp->input_itemID = 0;
     adp->input_itemtype = 0;
  }
  if (salp != NULL) 
  {
     if (svpp != NULL) 
        adp->showfeat = svpp->showfeat;
     switch_featOrder (adp, 1);
     adp = SeqAlignToEditAlignData (adp, salp, start, NULL, (Uint1)(FALSE)); 
  }
  if (adp == NULL)
     return NULL;
  adp->wdp = (Pointer) wdp;

  if (adp->anp_list && adp->master.region) 
  {
     j=0;
     for (vnp=adp->anp_list; vnp!=NULL; vnp=vnp->next) {
        if (vnp->choice != OBJ_SEQANNOT) {
           anp = (AlignNodePtr) vnp->data.ptrvalue;
           j++;
           if (j==1) 
              SeqIdWrite (anp->sip, strid1, PRINTID_REPORT, 16);
           else if (j==2)
              SeqIdWrite (anp->sip, strid2, PRINTID_REPORT, 16);
        }
     }
  }

             /* Panel for aligned sequences *
             ********************************/
  wid = (Int2)(adp->pnlWidth*adp->charw+adp->margin.right);
  hgt = (Int2)(adp->pnlLine * adp->lineheight + adp->margin.bottom);
  view = CreateViewer(w, wid, hgt, TRUE, TRUE);
  wdp->graph = view;
  wdp->data = (ValNodePtr) adp;

  g = HiddenGroup (w, -1, 0, NULL);                  
  SetGroupSpacing (g, 5, 5);

  wdp->replaceMergeCopyGroup = HiddenGroup (g, 3, 0, ChangeRMCProc);
  SetObjectExtra (wdp->replaceMergeCopyGroup, (Pointer) wdp, NULL);
  SetGroupSpacing (wdp->replaceMergeCopyGroup, 10, 5);
  RadioButton (wdp->replaceMergeCopyGroup, "Replace");
  mergebtn = RadioButton (wdp->replaceMergeCopyGroup, "Merge");
  RadioButton (wdp->replaceMergeCopyGroup, "Copy Features");

  h = HiddenGroup (g, 0, 0, NULL);

  wdp->rmcExtra [0] = HiddenGroup (h, 5, 0, NULL);
  sprintf (str, "'%s' by '%s'", strid1, strid2);
  StaticPrompt (wdp->rmcExtra [0], str, 0, 0, programFont, 'l');
  Hide (wdp->rmcExtra [0]);

  wdp->rmcExtra [1] = HiddenGroup (h, 6, 0, NULL);
  get_bestpos_formerge (salp, &start, &stop);
  StaticPrompt (wdp->rmcExtra [1], "From ", 0, popupMenuHeight, programFont, 'l');
  sprintf (str, "%d", (int) (start+1));
  wdp->fromtxt = DialogText (wdp->rmcExtra [1], str, 5, NULL);
  StaticPrompt (wdp->rmcExtra [1], " To ", 0, popupMenuHeight, programFont, 'l');
  sprintf (str, "%d", (int) (stop+1));
  wdp->totxt = DialogText (wdp->rmcExtra [1], str, 5, NULL);
  sprintf (str, " of '%s' with '%s'", strid2, strid1);
  StaticPrompt (wdp->rmcExtra [1], str, 0, popupMenuHeight, programFont, 'l');
/*****
  if (start < 0 && stop < 0) {
     Disable (mergebtn);
     Disable (wdp->fromtxt);
     Disable (wdp->totxt);
  }
******/
  wdp->keep_protid1 = CheckBox (wdp->rmcExtra [1], "Keep Protein ID", NULL);
  SetStatus (wdp->keep_protid1, TRUE); 
  Hide (wdp->rmcExtra [1]);

  wdp->rmcExtra [2] = HiddenGroup (h, 6, 0, NULL);
  sprintf (str, "From '%s' To '%s'", strid2, strid1);
  StaticPrompt (wdp->rmcExtra [2], str, 0, popupMenuHeight, programFont, 'l'); 
  sprintf (str, "'%s' contains exons and introns", strid2);
  wdp->gap_choicebn = CheckBox (wdp->rmcExtra [2], str, NULL);
  wdp->keep_protid2 = CheckBox (wdp->rmcExtra [2], "Keep Protein ID", NULL);
  Hide (wdp->rmcExtra [2]);

  AlignObjects (ALIGN_CENTER, (HANDLE) wdp->replaceMergeCopyGroup,
                (HANDLE) wdp->rmcExtra [0], (HANDLE) wdp->rmcExtra [1],
                (HANDLE) wdp->rmcExtra [2], NULL);

  g2 = HiddenGroup (w, 4, 0, NULL);
  wdp->acceptBtn = PushButton (g2, "Accept", AcceptSeqAlignViewForm);
  SetObjectExtra (wdp->acceptBtn, (Pointer) wdp, NULL);
  Disable (wdp->acceptBtn);
  PushButton (g2, "Preview Alignment", LaunchAlignEdit);
  PushButton (g2, "Cancel", CloseThisWindowProc);
  if (salp->next)
     PushButton (g2, "Skip", SkipThisWindowProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) view, (HANDLE) g, (HANDLE) g2, NULL);
  RealizeWindow (w);

             /* register Panel & adp in SeqEditViewFormPtr *
             ***************************************/
  ViewForGraph (view, adp->input_entityID, adp, wid, adp->anp_list);
  return (ForM) w;
}

/************************************************
***
***   SeqEditFunc : to launch BIOSEQ editor
***
************************************************/
extern Int2 LIBCALLBACK SeqEditFunc (Pointer data)
{
  WindoW              w = NULL;
  BioseqPtr           bsp = NULL;
  SeqEditViewFormPtr  wdp = NULL;
  OMProcControlPtr    ompcp;
  OMUserDataPtr       omudp;
  SeqIdPtr            sip;
  Char                str [64];
  SelStruct ss;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) 
     return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      bsp = (BioseqPtr) ompcp->input_data;
      break;
   case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (bsp == NULL) {
     return OM_MSG_RET_ERROR;
  }  
  ss.entityID = ompcp->input_entityID;
  ss.itemID = ompcp->input_itemID;
  ss.itemtype = ompcp->input_itemtype;
  ss.regiontype =0;
  ss.region = NULL;

  sip = SeqIdFindWorst (bsp->id);
  SeqIdWrite (sip, str, PRINTID_REPORT, 64);
  w = (WindoW) CC_CreateBioseqViewForm (-40, -90, str, bsp, &ss);
  if (w != NULL) {
     wdp = (SeqEditViewFormPtr) GetObjectExtra (w);
     if (wdp != NULL) {
        wdp->input_entityID = ompcp->input_entityID;
        wdp->input_itemID = ompcp->input_itemID;
        wdp->input_itemtype = ompcp->input_itemtype;
        wdp->this_itemtype = OBJ_BIOSEQ;
        wdp->this_subtype = bsp->repr;
        wdp->procid = ompcp->proc->procid;
        wdp->proctype = ompcp->proc->proctype;
        wdp->userkey = OMGetNextUserKey ();
        omudp = ObjMgrAddUserData (ompcp->input_entityID, ompcp->proc->procid, OMPROC_EDIT, wdp->userkey);
        if (omudp != NULL) {
          omudp->userdata.ptrvalue = (Pointer) wdp;
          omudp->messagefunc = BioseqEditMsgFunc;
        }
        checkEntityIDForMsg (wdp);
     }
     Show (w);
     Update ();
     CaptureSlateFocus ((SlatE) wdp->pnl);
     Select (w);
     return OM_MSG_RET_DONE;
  }
  return OM_MSG_RET_ERROR;
}

/************************************************
***
***   AlgEditFunc : to launch SEQALIGN editor
***
************************************************/
extern Int2 LIBCALLBACK AlgEditFunc (Pointer data)
{
  WindoW              w = NULL;
  SeqAlignPtr         salp = NULL;
  SeqEditViewFormPtr  wdp = NULL;
  OMProcControlPtr    ompcp;
  OMUserDataPtr       omudp;
  Char                str [64];
  SelStruct           ss;
  SeqAnnotPtr         sap, sap2;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) {
     ErrPostEx (SEV_ERROR, 0, 0, "Data NULL [1]");
     return OM_MSG_RET_ERROR;
  }
  switch (ompcp->input_itemtype) {
    case OBJ_SEQALIGN :
      salp = (SeqAlignPtr) ompcp->input_data;
      break;
   case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (salp == NULL) {
     ErrPostEx (SEV_ERROR, 0, 0, "Data NULL [2]");
     return OM_MSG_RET_ERROR;
  }
/********** TRICK in case there is a list of SeqAlign *********/
  if (salp->next!=NULL) {
     sap = SeqAnnotForSeqAlign (salp);
     sap2 = (SeqAnnotPtr) AsnIoMemCopy ((Pointer) sap, (AsnReadFunc) SeqAnnotAsnRead, (AsnWriteFunc) SeqAnnotAsnWrite);
     salp = (SeqAlignPtr) sap2->data;
     salp->next = NULL;
     sap->data=NULL;
     sap = SeqAnnotFree (sap);
     sap2->data=NULL;
     sap2 = SeqAnnotFree (sap2);
  }
/********** TRICK *********/

  ss.entityID = ompcp->input_entityID;
  ss.itemID = ompcp->input_itemID;
  ss.itemtype = ompcp->input_itemtype;
  ss.regiontype =0;
  ss.region = NULL;

  SeqIdWrite (SeqAlignId(salp, 0), str, PRINTID_REPORT, 64);
  w = (WindoW) CreateSeqAlignEditForm (-40, -90, str, salp, &ss);
  if (w != NULL) {
     wdp = (SeqEditViewFormPtr) GetObjectExtra (w);
     if (wdp != NULL) {
        wdp->input_entityID = ompcp->input_entityID;
        wdp->input_itemID = ompcp->input_itemID;
        wdp->input_itemtype = ompcp->input_itemtype;
        wdp->this_itemtype = OBJ_SEQALIGN;
        wdp->this_subtype = 0;
        wdp->procid = ompcp->proc->procid;
        wdp->proctype = ompcp->proc->proctype;
        wdp->userkey = OMGetNextUserKey ();
        omudp = ObjMgrAddUserData (ompcp->input_entityID, ompcp->proc->procid, OMPROC_EDIT, wdp->userkey);
        if (omudp != NULL) {
           omudp->userdata.ptrvalue = (Pointer) wdp;
           omudp->messagefunc = BioseqEditMsgFunc;
        }
        checkEntityIDForMsg (wdp);
     }
     Show (w);
     Update ();
     CaptureSlateFocus ((SlatE) wdp->pnl);
     Select (w);
     return OM_MSG_RET_DONE;
  }
  return OM_MSG_RET_ERROR;
}

/************************************************
***
***   AnnotAlgEditFunc : to launch SEQANNOT editor
***
************************************************/
extern Int2 LIBCALLBACK AnnotAlgEditFunc (Pointer data)
{
  WindoW              w;
  SeqAnnotPtr         sap;
  SeqAlignPtr         salp = NULL;
  SeqEditViewFormPtr  wdp = NULL;
  OMProcControlPtr    ompcp;
  OMUserDataPtr       omudp;
  Char                str [64];
  SelStruct           ss;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) {
     ErrPostEx (SEV_ERROR, 0, 0, "Data NULL [1]");
     return OM_MSG_RET_ERROR;
  }
  switch (ompcp->input_itemtype) {
    case OBJ_SEQANNOT :
      sap = (SeqAnnotPtr) ompcp->input_data;
      break;
   case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (sap == NULL) {
     ErrPostEx (SEV_ERROR, 0, 0, "Data NULL [2]");
     return OM_MSG_RET_ERROR;
  }
  if (sap->data==NULL || sap->type!=2) {
     return OM_MSG_RET_ERROR;
  }
  salp = (SeqAlignPtr)sap->data;

/****************************************/
{
  SeqEditViewProcsPtr svpp;

  svpp = (SeqEditViewProcsPtr) GetAppProperty ("SeqEditDisplayForm");
  if (svpp != NULL) {
     if (svpp->Cn3D_On)
        salp = DenseSegToDenseDiag (salp);
  }
}
/*******************************************/

  if (salp == NULL)
     return OM_MSG_RET_ERROR;
  ss.entityID = ompcp->input_entityID;
  ss.itemID = ompcp->input_itemID;
  ss.itemtype = ompcp->input_itemtype;
  ss.regiontype =0;
  ss.region = NULL;

  SeqIdWrite (SeqAlignId(salp, 0), str, PRINTID_REPORT, 64);
  w = (WindoW) CreateSeqAlignEditForm (-40, -90, str, salp, &ss);
  if (w != NULL) 
  {
     wdp = (SeqEditViewFormPtr) GetObjectExtra (w);
     if (wdp != NULL) 
     {
        wdp->input_entityID = ompcp->input_entityID;
        wdp->input_itemID = ompcp->input_itemID;
        wdp->input_itemtype = ompcp->input_itemtype;
        wdp->this_itemtype = OBJ_SEQALIGN;
        wdp->this_subtype = 0;
        wdp->procid = ompcp->proc->procid;
        wdp->proctype = ompcp->proc->proctype;
        wdp->userkey = OMGetNextUserKey ();
        omudp = ObjMgrAddUserData (ompcp->input_entityID, ompcp->proc->procid, OMPROC_EDIT, wdp->userkey);
        if (omudp != NULL) 
        {
           omudp->userdata.ptrvalue = (Pointer) wdp;
           omudp->messagefunc = BioseqEditMsgFunc;
        }
        checkEntityIDForMsg (wdp);
     }
     Show (w);
     Update ();
     CaptureSlateFocus ((SlatE) wdp->pnl);
     Select (w);

if (salp->segtype == 1) {
SeqAlignPtr tmp, newsalp;
Boolean ok;
EditAlignDataPtr   adp;

  adp = GetAlignDataPanel(wdp->pnl);
  for ( tmp=salp; tmp!=NULL; tmp=tmp->next)
  {
     if (tmp->type<1)
     {
        sap=adp->sap_original;
        if (sap)
        {
           newsalp = (SeqAlignPtr)sap->data;
           ok = SeqAlignIDCache (newsalp, SeqAlignId (tmp, 1));
           if (ok)
           {
/*
              if (adp->sap1_original)
                 SeqAlignIDCache ((SeqAlignPtr)adp->sap1_original->data, SeqIdFindBest (bsp->id, 0));
*/
              repopulate_panel (w, adp, newsalp);
           }
        }
     }
  }
}
     return OM_MSG_RET_DONE;
  }
  return OM_MSG_RET_ERROR;
}

extern Int2 LIBCALLBACK AlgViewFunc (Pointer data)
{
  WindoW              w;
  SeqAlignPtr         salp = NULL;
  SeqEditViewFormPtr  wdp = NULL;
  OMProcControlPtr    ompcp;
  OMUserDataPtr       omudp;
  Char                str [64];
  SelStruct           ss;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) {
     ErrPostEx (SEV_ERROR, 0, 0, "Data NULL [1]");
     return OM_MSG_RET_ERROR;
  }
  switch (ompcp->input_itemtype) {
    case OBJ_SEQALIGN :
      salp = (SeqAlignPtr) ompcp->input_data;
      break;
   case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (salp == NULL) {
     ErrPostEx (SEV_ERROR, 0, 0, "Data NULL [2]");
     return OM_MSG_RET_ERROR;
  }
  ss.entityID = ompcp->input_entityID;
  ss.itemID = ompcp->input_itemID;
  ss.itemtype = ompcp->input_itemtype;
  ss.regiontype =0;
  ss.region = NULL;
  SeqIdWrite (SeqAlignId(salp, 0), str, PRINTID_REPORT, 64);
  w = (WindoW) CreateSeqAlignViewForm (-50, -33, str, salp, &ss);
  if (w == NULL) {
     return OM_MSG_RET_ERROR;
  }
  wdp = (SeqEditViewFormPtr) GetObjectExtra (w);
  if (wdp != NULL) {
     wdp->input_entityID = ompcp->input_entityID;
     wdp->input_itemID = ompcp->input_itemID;
     wdp->input_itemtype = ompcp->input_itemtype;
     wdp->this_itemtype = OBJ_SEQALIGN;
     wdp->this_subtype = 0;
     wdp->procid = ompcp->proc->procid;
     wdp->proctype = ompcp->proc->proctype;
     wdp->userkey = OMGetNextUserKey ();
     omudp = ObjMgrAddUserData (ompcp->input_entityID, ompcp->proc->procid, OMPROC_EDIT, wdp->userkey);
     if (omudp != NULL) {
        omudp->userdata.ptrvalue = (Pointer) wdp;
        omudp->messagefunc = BioseqEditMsgFunc;
     }
     checkEntityIDForMsg (wdp);
  }
  Show (w);
  Update ();
  Select (w);
  return OM_MSG_RET_DONE;
}

/***********************************************************
***
*** Launch Editors
***     
***
************************************************************/
extern void LaunchAnnotAlignEditor (SeqAnnotPtr sap)
{
  Uint2            entityID,
                   itemID;
  Int2             handled;
  Uint2            options;

  if (sap != NULL) {
     entityID = ObjMgrRegister (OBJ_SEQANNOT, (Pointer) sap);
     options = ObjMgrGetOptions(entityID);
     options |= OM_OPT_FREE_IF_NO_VIEW;
     ObjMgrSetOptions(options, entityID);
     itemID = GetItemIDGivenPointer (entityID, OBJ_SEQANNOT, (Pointer) sap);
     handled = GatherProcLaunch (OMPROC_EDIT, FALSE, entityID, itemID, OBJ_SEQANNOT, 0, 0, OBJ_SEQANNOT, 0);
  }
}

extern void LaunchAlignEditor (SeqAlignPtr salp)
{
  Uint2            entityID,
                   itemID;
  Int2             handled;
  Uint2            options;

  if (salp != NULL) {
     entityID = ObjMgrRegister (OBJ_SEQALIGN, (Pointer) salp);
     options = ObjMgrGetOptions(entityID);
     options |= OM_OPT_FREE_IF_NO_VIEW;
     ObjMgrSetOptions(options, entityID);
     itemID = GetItemIDGivenPointer (entityID, OBJ_SEQALIGN, (Pointer) salp);
     handled = GatherProcLaunch (OMPROC_EDIT, FALSE, entityID, itemID, OBJ_SEQALIGN, 0, 0, OBJ_SEQALIGN, 0);
  }
}

extern void LaunchAlignViewer (SeqAlignPtr salp)
{
  Uint2            entityID,
                   itemID;
  Int2             handled;
  Uint2            options;

  if (salp != NULL) {
     entityID = ObjMgrRegister (OBJ_SEQALIGN, (Pointer) salp);
     options = ObjMgrGetOptions(entityID);
     options |= OM_OPT_FREE_IF_NO_VIEW;
     ObjMgrSetOptions(options, entityID);
     itemID = GetItemIDGivenPointer (entityID, OBJ_SEQALIGN, (Pointer) salp);
     handled = GatherProcLaunch (OMPROC_VIEW, FALSE, entityID, itemID, OBJ_SEQALIGN, 0, 0, OBJ_SEQALIGN, 0);
  }
}

extern Int2 LIBCALLBACK LaunchAlignEditorFromDesktop (Pointer data)
{
  OMProcControlPtr ompcp;
  SeqEntryPtr      sep; 
  SeqAlignPtr      salp;

  ompcp = (OMProcControlPtr) data;
  sep=ReadLocalAlignment(SALSAA_CONTIGUOUS, NULL);
  if (!sep) 
     return 0;
  salp = (SeqAlignPtr) FindSeqAlignInSeqEntry (sep, OBJ_SEQALIGN);
  if (salp != NULL) { 
     LaunchAlignEditor (salp);
  }
  return OM_MSG_RET_OK;
}

extern Int2 LIBCALLBACK LaunchAlignEditorFromDesktop2 (Pointer data)
{
  OMProcControlPtr ompcp;
  SeqEntryPtr      sep; 
  SeqAlignPtr      salp;

  ompcp = (OMProcControlPtr) data;
  sep=ReadLocalAlignment(SALSAA_INTERLEAVE, NULL);
  if (!sep) 
     return 0;
  salp = (SeqAlignPtr) FindSeqAlignInSeqEntry (sep, OBJ_SEQALIGN);
  if (salp != NULL) { 
     LaunchAlignEditor (salp);
  }
  return OM_MSG_RET_OK;
}

extern Int2 LIBCALLBACK LaunchAlignEditorFromDesktop3 (Pointer data)
{
  OMProcControlPtr ompcp;
  SeqAlignPtr      salp;
  Uint2            entityID;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) {
     ErrPostEx (SEV_ERROR, 0, 0, "Data NULL [1]");
     return OM_MSG_RET_ERROR;
  }
  switch (ompcp->input_itemtype) {
    case OBJ_SEQALIGN :
      salp = (SeqAlignPtr) ompcp->input_data;
      break;
   case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (salp == NULL) {
     ErrPostEx (SEV_ERROR, 0, 0, "Data NULL [2]");
     return OM_MSG_RET_ERROR;
  }
  salp = aaSeqAlign_to_dnaSeqAlign (SeqAlignDup(salp), NULL, NULL);
  entityID = ObjMgrRegister (OBJ_SEQALIGN, (Pointer) salp);
  return OM_MSG_RET_OK;
}

extern Int2 LIBCALLBACK LaunchAlignEditorFromDesktop4 (Pointer data)
{
  OMProcControlPtr ompcp;

  ompcp = (OMProcControlPtr) data;
  return OM_MSG_RET_OK;
}


/**********************************************************/

static void SeqAlignDeleteInSeqEntryCallBack (SeqEntryPtr sep, Pointer mydata,
                                          Int4 index, Int2 indent)
{
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  SeqAnnotPtr        sap,
                     pre;
  BoolPtr            dirtyp;
  
  if (sep != NULL && sep->data.ptrvalue && mydata != NULL) {
     dirtyp = (BoolPtr)mydata;
     if (IS_Bioseq(sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp!=NULL) {
           sap=bsp->annot;
           pre=NULL;
           while (sap) {
              if (sap->type == 2) {
                 if (pre==NULL) {
                    bsp->annot = sap->next;
                    sap->next=NULL;
                    sap = SeqAnnotFree (sap);
                    if (bsp->annot)
                       sap=bsp->annot->next;
                 }
                 else {
                    pre=sap->next;
                    sap->next=NULL;
                    sap = SeqAnnotFree (sap);
                    if (pre)
                       sap=pre->next;
                 }
                 *dirtyp=TRUE;
              }
              else {
                 pre=sap;
                 sap=sap->next;
              }
           }
        }
     }
     else if(IS_Bioseq_set(sep)) {
        bssp = (BioseqSetPtr)sep->data.ptrvalue;
        if (bssp!=NULL) {
           sap=bssp->annot;
           pre=NULL;
           while (sap) {
              if (sap->type == 2) {
                 if (pre==NULL) {
                    bssp->annot = sap->next;
                    sap->next=NULL;
                    sap = SeqAnnotFree (sap);
                    if (bssp->annot)
                       sap=bssp->annot->next;
                 }
                 else {
                    pre=sap->next;
                    sap->next=NULL;
                    sap = SeqAnnotFree (sap);
                    if (pre)
                       sap=pre->next;
                 }
                 *dirtyp=TRUE;
              }
              else {
                 pre=sap;
                 sap=sap->next;
              }
           }
        }
     }
  }
}

extern Int2 LIBCALLBACK UpdateSeqAlign (Pointer data)
{

  OMProcControlPtr  ompcp;
  SeqAlignPtr       salp=NULL,
                    salpnew;
  SeqAnnotPtr       sap=NULL,
                    sapcopy;
  SeqEntryPtr       sep=NULL,
                    sepnew=NULL;
  Uint2             entityID,
                    itemID;
  MsgAnswer         ans;
  SeqSubmitPtr      ssp;
  Boolean           ok = TRUE, 
                    dirty = FALSE;
   
  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;

  if (ompcp->input_data == NULL) return OM_MSG_RET_ERROR;

  switch(ompcp->input_itemtype)
    {
    case OBJ_BIOSEQ :
      sep = SeqMgrGetSeqEntryForData (ompcp->input_data);
      break;
    case OBJ_BIOSEQSET :
      sep = SeqMgrGetSeqEntryForData (ompcp->input_data);
      break;
    case OBJ_SEQENTRY :
      sep = ompcp->input_data;
      break;
    case OBJ_SEQSUB :
      ssp = ompcp->input_data;
      if(ssp->datatype==1)
         sep = (SeqEntryPtr)ssp->data;
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (sep==NULL)
     return OM_MSG_RET_ERROR;
  entityID = ObjMgrGetEntityIDForChoice (sep);
  if (entityID < 1)
     return OM_MSG_RET_ERROR;

  sepnew = ReadAnyAlignment (FALSE, NULL);
  if (sepnew) {
     salpnew = (SeqAlignPtr) FindSeqAlignInSeqEntry (sepnew, OBJ_SEQALIGN);
     if (salpnew) {
        ok = ValidateSeqAlignandACCInSeqEntry (sepnew, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE);
        if (ok) {
           salp = (SeqAlignPtr) FindSeqAlignInSeqEntry (sep, OBJ_SEQALIGN);
           if (salp) {
              ans = Message (MSG_OKC, "Do you wish to replace (OK) or add (Cancel) the alignment in your SeqEntry?");
              if (ans == ANS_OK)
              {
                 SeqEntryExplore (sep, &dirty, SeqAlignDeleteInSeqEntryCallBack);
              }
           }
           sap=SeqAnnotForSeqAlign(salpnew);
           sapcopy = (SeqAnnotPtr) AsnIoMemCopy (sap, (AsnReadFunc) SeqAnnotAsnRead, (AsnWriteFunc) SeqAnnotAsnWrite);
           SeqAlignAddInSeqEntry (sep, sapcopy);
           sap->data=NULL;
           MemFree(sap);
           ObjMgrSetDirtyFlag (entityID, TRUE);
           itemID = GetItemIDGivenPointer (entityID, OBJ_SEQENTRY, (Pointer) sep);
           ObjMgrSendMsg (OM_MSG_UPDATE, entityID, itemID, OBJ_SEQENTRY);
        }
     }
     ObjMgrFree (OBJ_SEQENTRY, (Pointer)sepnew);
     sepnew=NULL;
  }
  return OM_MSG_RET_OK;
}











