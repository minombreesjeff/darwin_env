/*   ingenwin.c
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
* File Name:  ingenwin.c
*
* Author:  Fasika Aklilu
*
* Version Creation Date:   8/10/01
*
* $Revision: 6.12 $
*
* File Description:  handles windowing for ingenue
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include <ingenwin.h>
#include <urkcnsrt.h>



typedef struct ingsaveinfo{
  TexT       text;
} IngSaveInfo, PNTR IngSaveInfoPtr;


typedef struct selectdata {
  VieweR        vwr;
  Boolean       returnFALSE;
  Uint2         entityID;
  Uint2         itemID;
  Uint2         itemtype;
  Uint2         segID;
} IngSelectData, PNTR IngSelectPtr;


/*******************************************************************************

	Error messages and Global Variables

*******************************************************************************/


static char szErrMsg1[]="Attempt to open the Genome Viewer without a BioSeq object.";
static char szErrMsg2[]="Genome Viewer was unable to build its internal data structures.";
static char szErrMsg3[]="Genome Viewer was unable to start its GUI.";
static char szErrMsg4[]="Genome Viewer was unable to find BioSeq(eID,iID).";
static char szErrMsg5[]="Genome Viewer was unable to create a Raw representation of a single segmented BioSeq(eID,iID).";


static CharPtr IngfeatDefLabels[FEATDEF_MAX]={"Alignments", "Genes", "Organism", "Coding Regions", "Proteins", "preRNA", "mRNA", "tRNA", "rRNA", "snRNA", "scRNA", "otherRNA","Publications",  "Sequence Tracking",  "Imported", "Allele", "Attenuator", "C region", "CAAT signal", "Imported CDS", "Conflict", "D loop", "D segment", "Enhancer", "Exon", "GC signal", "iDNA", "Intron", "J segment", "LTR", "Mature Peptide", "Misc Binding Site", "Misc Difference", "Misc Feature", "Misc Recomb", "Misc RNA", "Misc Signal", "Misc Structure", "Modified Base", "Mutation", "N Region", "Old Sequence", "PolyA Signal", "PolyA Site","Precursor RNA", "Primary Transcript", "Primer Binding Site","Promotor", "Protein Binding Site", "RBS", "Repeat Region", "Repeat Unit", "Replication Origin", "S region", "Satellite", "Signal Peptide", "Source", "Stem Loop", "STS", "TATA Signal", "Terminator", "Transit Peptide", "Unsure", "V Region", "V Segment", "Variation", "Virion", "3' Clip", "3' UTR", "5' Clip", "5' UTR", "10 Signal", "35 Signal", "Site Reference" , "Region", "Comment", "Bond", "Site", "Retriction Site", "User Defined" , "Transcription Initiation", "Custom Numbering","Protein 2-D Structure", "Non Std Residue", "Heterogens", "Bio Source" , "Preprotein", "Mature Peptide aa","Signal Peptide aa", "Transit Peptide aa"};
 
/* extern */
extern Uint1  featDefTrack2[];
/* static */
static Uint2 fDefLookup[FEATDEF_MAX];/* maps featDefTrack to featDefFilter */
static Uint2 WinCounter=0; /* keep track of open windows */
static IngMsgInfo     msg; /* contains info for highlighting segments */
static Uint1Ptr       seq; /* buffer for GC content */
/*******************************************************************************

	Static Function Declarations

*******************************************************************************/
static Boolean Ing_SelectPrim (SegmenT seg, PrimitivE prim, Uint2 segID, Uint2 primID, Uint2 primCt, VoidPtr userdata);
static Boolean Ing_DeSelectPrim (SegmenT seg, PrimitivE prim, Uint2 segID, Uint2 primID, Uint2 primCt, VoidPtr userdata);
static Boolean Ing_DeSelectGenericPrim (SegmenT seg, PrimitivE prim, Uint2 segID,Uint2 primID, Uint2 primCt, VoidPtr userdata);
static void  Ing_ZoomerClickProc(VieweR v, SegmenT s, PoinT pt);
static void Ing_ResizeMainWindow(IngGenomeViewerPtr igvp,WindoW w, Boolean first);
static void  Ing_Ruler1ClickProc(VieweR v, SegmenT s, PoinT pt);
static void  Ing_Ruler1DragProc(VieweR v, SegmenT s, PoinT pt);
static void  Ing_Ruler1ReleaseProc(VieweR v, SegmenT s, PoinT pt);
static void Ing_UpdateZoomerVals(IngGenomeViewerPtr igvp, Int4 scaleX);
static void Ing_MoveCursor(IngGenomeViewerPtr igvp, Int4 from, Int4 to, Boolean bFirst);
static void Ing_FeatViewerClickProc(VieweR v, SegmenT s, PoinT pt);
static void Ing_FeatViewerPanProc(VieweR v, SegmenT s);
static void Ing_SequenceClickProc(VieweR v, SegmenT s, PoinT pt);
static Boolean Ing_PopulateZoomer(IngGenomeViewerPtr igvp);
static Int4  Ing_GetBPMaxScale(IngGenomeViewerPtr igvp);
static Int4  Ing_GetTPMaxScale(IngGenomeViewerPtr igvp, Int2 vwidth);
static SeqFeatPtr Ing_CreateNewFeature(Uint2 entityID, Uint2 itemID, BioseqPtr product, Uint1 seqfeattype, Uint1 featdeftype, CharPtr label, SeqLocPtr slp_feat, CharPtr color);
static void Ing_ResetTitle(IngGenomeViewerPtr igvp, Boolean clear);
static Int2 LIBCALLBACK Ing_OM_MsgFunc (OMMsgStructPtr ommsp);

/*******************************************************************************

  Function : Ing_PopulateDetailedPage()
 
  Purpose :  Bottom (Detailed) viewer drawing function

*******************************************************************************/

extern void Ing_PopulateDetailedPage(Pointer data)
{
  Int4               scaleX=0;
  Int4               top;
  Int4               bottom;
  IngGenomeViewerPtr igvp=NULL;
  BioseqPtr          bsp=NULL;
  Uint4              nRows;
  IngExploreSegs     gpn;
  Int2               nSegments;
  Char               scale_buf[35]={""};
  SelStructPtr       ssp=NULL;
  IngSelectData      sel;
  Boolean            bSelected = FALSE;
  IngReportPtr       Report = NULL;


  igvp=(IngGenomeViewerPtr)data;
  if (!igvp) return;
  WatchCursor();
  ssp=ObjMgrGetSelected();
  Reset(igvp->vBottom);
  igvp->pictBottom=DeletePicture(igvp->pictBottom);
  Reset(igvp->vRuler2);
  igvp->pictRuler2=DeletePicture(igvp->pictRuler2);
  Update();

  igvp->pictBottom=CreatePicture();
  igvp->pictRuler2=CreatePicture();
  scaleX=igvp->scaleX;
  if (igvp->bsp)
    bsp=igvp->bsp;
  else{
    bsp=GetBioseqGivenIDs (igvp->entityID, igvp->itemID, OBJ_BIOSEQ);
    igvp->bsp=bsp;
  }
  if (bsp==NULL){
    ErrPostEx(SEV_ERROR, 0, 0, szErrMsg4);
    ArrowCursor();
    return;
  }
  /*check the sequence length; could be undef.*/
  if (bsp->length==(Int4)-1){
    ErrPostEx(SEV_ERROR, 0, 0, szErrMsg5);
    ArrowCursor();
    return;
  }
  gpn.seg = igvp->pictRuler2;
  gpn.viewer = igvp->vRuler2;
  gpn.idx = 1;
  gpn.GrData = &igvp->GrData;
  gpn.left = igvp->from;
  gpn.right = igvp->to;
  gpn.scaleX = scaleX;
  gpn.seqbuf = igvp->seqbuf;
  gpn.bShowGC  =  igvp->bShowGC;
  gpn.bLabels = igvp->bLabels;
  gpn.bTop = FALSE;
  gpn.bRegister = TRUE;
  gpn.procID = igvp->procID;
  gpn.userKey = igvp->userKey;
  gpn.messagefunc = Ing_OM_MsgFunc;
  gpn.data = (Pointer)igvp;
  nSegments = SeqMgrExploreSegments(bsp, (Pointer)&gpn, Ing_ExploreSegments);

  /*this bioseq doesn't have any segments*/
  if (nSegments==0){
    igvp->idx = 2;
    top = igvp->GrData.SegBoxHeight;
    bottom = top-igvp->GrData.SegBoxHeight;
    Ing_AddGCRect(igvp->pictRuler2, igvp->bsp->id, igvp->entityID, igvp->itemID, OBJ_BIOSEQ, igvp->seqbuf, 0, top, igvp->SeqLength, bottom, scaleX, Seq_strand_plus, TRUE, TRUE, igvp->idx, igvp->bShowGC);
    igvp->bSegmented = FALSE;
  }
  else{
    igvp->bSegmented = TRUE;
  }
  
  Ing_AddRuler(igvp->pictRuler2, 2*igvp->GrData.SegBoxHeight, igvp->from, igvp->to, scaleX, 1, TRUE);
  AttachPicture(igvp->vRuler2, igvp->pictRuler2, (igvp->xposition<=0?INT4_MIN:igvp->xposition), INT4_MAX, UPPER_LEFT, MAX(1,scaleX) , 1, NULL); 
  SetViewerProcs(igvp->vRuler2, Ing_SequenceClickProc, NULL, NULL, NULL);

  nRows = Ing_PopDetailedPage(bsp, igvp->pictBottom, gpn.left, gpn.right, igvp->GrData.pClr, scaleX, 0, igvp->bLabels, igvp->isExons, &igvp->talign);
  nRows+=2;
  igvp->last_feat_position_bottom = nRows*(-20);

  /* add white characters to align picture with ruler */
  AddAttribute(igvp->pictBottom, COLOR_ATT, WHITE_COLOR, 0, 0, 0, 0);
  sprintf(scale_buf, "%d", igvp->from);
  AddLabel(igvp->pictBottom, 0, igvp->last_feat_position_bottom, scale_buf, SMALL_TEXT, 0, LOWER_CENTER, 0);
  AddLine(igvp->pictBottom, 0, igvp->last_feat_position_bottom, (igvp->to - igvp->from), igvp->last_feat_position_bottom, 0, 0);

  AttachPicture (igvp->vBottom,igvp->pictBottom, (igvp->xposition<=0?INT4_MIN:igvp->xposition), INT4_MAX, UPPER_LEFT, MAX(1,scaleX) , 1, NULL); 
  SetViewerProcs(igvp->vBottom,Ing_FeatViewerClickProc,NULL,NULL, Ing_FeatViewerPanProc);
  while (ssp){
    /* reselect objects in windows */
    sel.entityID  =  ssp->entityID;
    sel.itemID = ssp->itemID;
    sel.itemtype = ssp->itemtype;
    
    if (igvp->bOverviewSelected){
      sel.vwr = igvp->vTop;
      sel.returnFALSE = FALSE;
      ExploreSegment (igvp->pictTop, (Pointer) &sel, Ing_SelectPrim);
    }
    
    sel.vwr = igvp->vRuler2;      
    sel.returnFALSE = TRUE;
    ExploreSegment (igvp->pictRuler2, (Pointer) &sel, Ing_SelectPrim);
    sel.vwr = igvp->vBottom;      
    sel.returnFALSE = TRUE;
    ExploreSegment (igvp->pictBottom, (Pointer) &sel, Ing_SelectPrim);
    ssp = ssp->next;
  }

  if (igvp->hReport){
    Report = (IngReportPtr)GetObjectExtra(igvp->hReport);
    if(Report)
      Ing_PopulateReport(Report, igvp);
  }

  Ing_ResetTitle(igvp, FALSE);

  ArrowCursor();
}

/*******************************************************************************

  Function : Ing_PopulateOverviewPage()
 
  Purpose :  Top (Overview) viewer drawing functions

*******************************************************************************/

extern void Ing_PopulateOverviewPage(Pointer data, Boolean UpdateRuler)
{
  Int4               maxScaleX;
  IngGenomeViewerPtr igvp;
  BioseqPtr          bsp = NULL;
  Uint4              nLines;
  Char               scale_buf[35] = {""};


  igvp = (IngGenomeViewerPtr)data;
  if (!igvp) return;
  WatchCursor();
  Reset(igvp->vTop);
  igvp->pictTop = DeletePicture(igvp->pictTop);
  if(UpdateRuler){
    Reset(igvp->vRuler1);
    igvp->pictRuler1 = DeletePicture(igvp->pictRuler1);
  }
  Update();

  igvp->pictTop = CreatePicture();
  if (UpdateRuler)
    igvp->pictRuler1 = CreatePicture();
  maxScaleX = igvp->maxScaleX;
  if (igvp->bsp)
    bsp = igvp->bsp;
  else{
    bsp = GetBioseqGivenIDs (igvp->entityID, igvp->itemID, OBJ_BIOSEQ);
    igvp->bsp = bsp;
  }
  if (bsp==NULL){
    ErrPostEx(SEV_ERROR, 0, 0, szErrMsg4);
    ArrowCursor();
    return;
  }
  /*check the sequence length; could be undef.*/
  if (bsp->length==(Int4)-1){
    ErrPostEx(SEV_ERROR, 0, 0, szErrMsg5);
    ArrowCursor();
    return;
  }
  
  if (UpdateRuler)
    Ing_PopOverviewRuler(igvp->vRuler1, igvp->pictRuler1, bsp, igvp->GrData, igvp->from, igvp->to, igvp->seqbuf, igvp->maxScaleX);
  nLines=Ing_PopOverviewPage(bsp, igvp->pictTop, igvp->from, igvp->to, igvp->GrData.pClr, maxScaleX, &igvp->talign);
  igvp->last_feat_position_top=nLines*(-20);
  /* add scale value in white to vTop to align picture to ruler */
  AddAttribute(igvp->pictTop, COLOR_ATT, WHITE_COLOR, 0, 0, 0, 0);
  sprintf(scale_buf, "%d", igvp->from);
  AddLabel(igvp->pictTop, 0, igvp->last_feat_position_top, scale_buf, SMALL_TEXT, 0, LOWER_CENTER, 0);
  AddAttribute(igvp->pictTop, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);

  AttachPicture (igvp->vRuler1,igvp->pictRuler1, INT4_MIN, INT4_MAX, UPPER_LEFT, MAX(1, maxScaleX) , 1, NULL);
  SetViewerProcs(igvp->vRuler1,Ing_Ruler1ClickProc,Ing_Ruler1DragProc,Ing_Ruler1ReleaseProc, NULL);
  
  AttachPicture (igvp->vTop, igvp->pictTop, INT4_MIN, INT4_MAX, UPPER_LEFT,  MAX(1,maxScaleX) , 1, NULL);
  SetViewerProcs(igvp->vTop, NULL ,NULL,NULL, NULL);

  ArrowCursor();
}


/*******************************************************************************

  Function : Ing_GetNamesZoomedVal()
  
  Purpose : get from, to and scale data for bottom (Detailed) viewer

*******************************************************************************/
static void Ing_GetNamesZoomedVal(VieweR vMain,Int4Ptr from, Int4Ptr to,
	Int4Ptr scaleX,Int4 MaxLength)
{
Int4    scale_X;
BoxInfo port;

	ViewerBox(vMain,NULL,&port,NULL,&scale_X,NULL);
		
	if (from) *from=port.left;
	if (to) {
		*to=port.right;
		if (port.right>MaxLength){
			(*to)=MaxLength;
			if (port.left<0)(*to)+=ABS(port.left);
		}
	}
	if (scaleX) *scaleX=scale_X;
}





/*******************************************************************************

  Function : Ing_ExpandRegion(), Ing_DoRange(), Ing_GotoProc(), Ing_ShowALLProc()
 
  Purpose :  functions to show a limited range of the loaded sequence

*******************************************************************************/
static void Ing_ExpandRegion(IngGenomeViewerPtr igvp, Int4 start, Int4 end)
{
  RecT      rc;
  Int4      scaleX;
  Int4      from, to;
  Int4      index;
  PrimitivE prim;
  

  igvp->SeqLength=ABS(end-start);
  igvp->from=start;
  igvp->to=end; 
  if (igvp->slp)
    SeqLocFree(igvp->slp);
  igvp->slp=SeqLocIntNew(start, end, Seq_strand_plus, (SeqIdPtr)igvp->bsp->id);

  /* update Zoomer values, and select center bar */
  GetPosition(igvp->vTop, &rc);
  igvp->maxScaleX=Ing_GetTPMaxScale(igvp, rc.right-rc.left);
  scaleX=Ing_GetBPMaxScale(igvp);
  ExploreSegment (igvp->pictZoom,  (Pointer)igvp->vZoom, Ing_DeSelectGenericPrim);
  Ing_UpdateZoomerVals(igvp, scaleX);
  index=8;
  igvp->scaleX=igvp->scale_array[index-1];
  prim=GetPrimitive(igvp->pictZoom,(Uint2)index);
  HighlightPrimitive (igvp->vZoom, igvp->pictZoom, prim, FRAME_PRIMITIVE);
  /* re populate pictures */
  Ing_PopulateOverviewPage((Pointer)igvp, TRUE);
  Ing_PopulateDetailedPage((Pointer)igvp);
  Ing_GetNamesZoomedVal(igvp->vRuler2,&from,&to,NULL,igvp->SeqLength);
  igvp->xposition=from;
  Ing_MoveCursor(igvp ,from,to,TRUE); 

}

static void Ing_DoRange(ButtoN b)
{
  IngGenomeViewerPtr igvp;
  IngRangePtr        rnp;
  Int4               start=0, end=0;
  WindoW             w;
  BioseqPtr          bsp;
  PrimitivE          prim;


  w=ParentWindow(b);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  rnp=(IngRangePtr)GetObjectExtra(b);
  if ((!igvp) || (!rnp)) return;
  bsp=igvp->bsp;

  start=MAX(0, Ing_GetValue(rnp->left));
  end=MIN(Ing_GetValue(rnp->right), igvp->bsp->length-1);
  if (start>=end){
    if (start==end) return;
    Message (MSG_OK, "_from_ is greater than _to_");
    return;
  }
  Ing_ExpandRegion(igvp, start, end);
}


static void Ing_GotoProc(ButtoN b)
{
  Ing_DoRange(b);
}

static void Ing_ShowALLProc(ButtoN b)
{
  IngGenomeViewerPtr igvp=NULL;

  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(b));
  Ing_ExpandRegion(igvp, 0, igvp->bsp->length-1);
}

/***************************************************

  Function : Ing_UpdateGCGraph(), Ing_ShowGCContent(); Ing_ShowComplexity()
  
  Purpose : Show or update display of GC rich regions on sequence; Show low complexity regions

***************************************************/
static void Ing_UpdateGCGraph(IngGenomeViewerPtr igvp)
{
  IngExploreSegs      gpn;
  Int4                top;
  Int4                bottom;
  Int4                scaleX;


  Reset(igvp->vRuler2);
  igvp->pictRuler2=DeletePicture(igvp->pictRuler2);
  Update();

  igvp->pictRuler2=CreatePicture();
  scaleX=igvp->scaleX;

  if (igvp->bSegmented){
    gpn.idx=1;
    gpn.seg=igvp->pictRuler2;
    gpn.viewer=igvp->vRuler2;
    gpn.GrData=&igvp->GrData;
    gpn.seqbuf=igvp->seqbuf;
    gpn.bsp=igvp->bsp;
    gpn.scaleX=igvp->scaleX;
    gpn.bShowGC = igvp->bShowGC;
    gpn.bRegister = TRUE;
    gpn.procID = igvp->procID;
    gpn.userKey = igvp->userKey;
    if (igvp->slp){
      gpn.left=GetOffsetInBioseq(igvp->slp, igvp->bsp, SEQLOC_LEFT_END);
      gpn.right=GetOffsetInBioseq(igvp->slp, igvp->bsp, SEQLOC_RIGHT_END);
    }
    else{
      gpn.left=igvp->from;
      gpn.right=igvp->to;
    }
    SeqMgrExploreSegments(igvp->bsp, (Pointer)&gpn, Ing_ExploreSegments);
    
  }
  else{
    igvp->idx = 2;
    top=(2*igvp->GrData.SegBoxHeight);
    bottom=top-igvp->GrData.SegBoxHeight;
    Ing_AddGCRect(igvp->pictRuler2, igvp->bsp->id, igvp->entityID, igvp->itemID, OBJ_BIOSEQ, igvp->seqbuf, 0, top, igvp->SeqLength, bottom, scaleX, Seq_strand_plus, TRUE, TRUE, igvp->idx, igvp->bShowGC);
  }

  Ing_AddRuler(igvp->pictRuler2, 2*igvp->GrData.SegBoxHeight, igvp->from, igvp->to, scaleX, 1, TRUE);
  AttachPicture(igvp->vRuler2, igvp->pictRuler2, (igvp->xposition<=0?INT4_MIN:igvp->xposition), INT4_MAX, UPPER_LEFT, MAX(1,scaleX) , 1, NULL); 
  SetViewerProcs(igvp->vRuler2, Ing_SequenceClickProc, NULL, NULL, NULL);
 
}


extern Uint1Ptr Ing_FillSeqBuffer(BioseqPtr bsp, SeqLocPtr slp, Int4 from, Int4 to)
{
  Uint1Ptr        buffer;
  Int2             ctr;    
  SeqPortPtr       spp;
  Uint1Ptr         temp_seq;
  Int4             buf_len, i;
  Uint1Ptr         seq = NULL;


  buffer=(Uint1Ptr) MemNew (sizeof(Uint1)*101);
  MemSet((Pointer)buffer, '\0', sizeof(Char)*101); 
  temp_seq=NULL;

  seq = (Uint1Ptr) MemNew (sizeof(Uint1)*(to-from+1));
  if (slp)
    {
      spp = SeqPortNewByLoc(slp, Seq_code_ncbi4na);
    }
  else
    {
      spp = SeqPortNew (bsp, from, to, 0, Seq_code_ncbi4na); 
    }

  if (spp == NULL)
    {
      ErrPostEx (SEV_ERROR, 0, 0, "%s", "GV- Failed on SeqPortNew");
      return NULL;
    }
  temp_seq =  seq;
  do 
    {
      ctr = SeqPortRead(spp, buffer, 100);
      
      if (ctr > 0) 
        {  
          i = 0;
          buf_len = ctr;
          while (buf_len > 0)
            {
              *temp_seq = (Uint1)buffer[i];
              temp_seq++;
              i++;  
              buf_len--;
            }
        }
    } while (ctr != SEQPORT_EOF * -1);

  SeqPortFree(spp);
  if (buffer) MemFree(buffer);

  return seq;
}




static void Ing_GenericCancelButton(ButtoN b)
{
  WindoW  w;
  w=ParentWindow(b);
  Remove(w);
}


/*****************************************************

Function: Ing_PopulateCpG(); Ing_ResizeCpGWindow(); Ing_CreateCpGReport(); Ing_CloseCpGWindow()

Purpose:  Print CpG island report

****************************************************/

typedef struct ing_motifinfo {
  VieweR  v;
  SegmenT seg;
  CharPtr name;
  Pointer motif;
} IngMInfo, PNTR IngMInfoPtr;


static void Ing_PopulateCpG(IngMInfoPtr minfo)
{
  ACT_CGInfoPtr  temp = NULL;
  FloatHi confidence;
  Char    str[500] = {""};
  RecT    rc;
  Int4    row = 0;
  Int4    winsize;
  Int2    slen, swidth;
  Int2    cxChar, nchars;
  SegmenT seg;
  VieweR  v;
  

  v = minfo->v;

  Reset(v);
  DeletePicture(minfo->seg);
  Update();

  minfo->seg = CreatePicture();
  seg = minfo->seg;

  GetPosition(v, &rc);
  winsize = (rc.right-rc.left) - 3*stdCharWidth;
  SetSmallFont();
  
  sprintf(str, "  -- CpG islands --  ");  
  
  slen = StringLen(str);
  swidth = StringWidth(str);
  cxChar=(Int2)ceil((float)swidth/slen);
  nchars=(Int2)ceil((float)winsize/cxChar);

  row += Ing_PrintText(str, seg, row, winsize, RED_COLOR, 0);

  temp = (ACT_CGInfoPtr)minfo->motif;
  while (temp != NULL){
    confidence = (FloatHi)(temp->length/(FloatHi)(temp->c * temp->g));
    sprintf(str, "CpG: %d to %d  Conf: %f", temp->from + 1, temp->to + 1, confidence);
    row += Ing_PrintText(str, seg, row, winsize, BLACK_COLOR, 0);
    temp = temp->next;
  }
  AttachPicture (v, seg, INT4_MIN, INT4_MAX, UPPER_RIGHT, 1 , 1, NULL);

}

static void Ing_CloseCpGWindow(WindoW w)
{
  IngMInfoPtr minfo = NULL;
  ACT_CGInfoPtr cpg, tmp;

  minfo = GetObjectExtra(w);
  if (!minfo){
    Remove(w);
    return;
  }

  cpg = (ACT_CGInfoPtr)minfo->motif;
  while (cpg){
    tmp = cpg->next;
    MemFree(cpg);
    cpg = tmp;
  }

  Remove(w);
  return;
}

static void Ing_ResizeCpGWindow(WindoW w)
{
  RecT     rcW, rcR, rcVsb, rcHsb;
  Int2     height,width,gap,vsbWidth;
  Int2     hsbWidth,in;
  BaR      vsb;
  BaR      hsb;
  IngMInfoPtr minfo = NULL;
  VieweR   v;

  minfo = (IngMInfoPtr) GetObjectExtra(w);
  if (!minfo) return;

  v = minfo->v;
  ObjectRect(w, &rcW);
  width= rcW.right-rcW.left;
  height= rcW.bottom-rcW.top;
  SafeHide(v);
  Update();

  vsb = GetSlateVScrollBar ((SlatE) v);
  hsb = GetSlateHScrollBar ((SlatE) v);
  GetPosition(v, &rcR);
  GetPosition(vsb, &rcVsb);
  GetPosition(vsb, &rcHsb);
  vsbWidth=rcVsb.right-rcVsb.left;
  hsbWidth=rcHsb.right-rcHsb.left;

  gap=10;
  in=2;

  rcR.right=rcR.left+width-in-vsbWidth;
  rcR.bottom=rcR.top+height-in-gap-hsbWidth;
  SetPosition(v, &rcR);

  Ing_PopulateCpG(minfo);

  SafeShow(v);
  Update();
}


static WindoW Ing_CreateCpGReport(ACT_CGInfoPtr cpg)
{
  WindoW  w;
  VieweR  v;
  SegmenT pict;
  IngMInfoPtr minfo = NULL;
  Int2    left_margin;

  left_margin=5*stdCharWidth;  
  w = DocumentWindow(left_margin, -65 ,550, 250, "CpG Report", Ing_CloseCpGWindow, Ing_ResizeCpGWindow);
  v = CreateViewer(w, 500 ,200, TRUE, TRUE);
  pict = CreatePicture();
  
  minfo = (IngMInfoPtr)MemNew(sizeof(IngMInfo));
  minfo->motif = (Pointer) cpg;
  minfo->v = v;
  minfo->seg = pict;
  Ing_PopulateCpG(minfo);
  SetObjectExtra(w, (Pointer)minfo, StdCleanupExtraProc);

  Show(w);
  return w;
}

 /*****************************************************

Function: Ing_PopulateMotif(); Ing_ResizeMotifWindow(); Ing_CreateMotifReport(); Ing_CloseMotifWindow()

Purpose:  Print Motif report

****************************************************/
static void Ing_PopulateMotif(IngMInfoPtr minfo)
{
  MotifInfoPtr   temp = NULL;
  CharPtr        name = NULL;
  FloatHi        confidence;
  Char           str[500] = {""};
  RecT           rc;
  Int4           row = 0;
  Int4           winsize;
  Int4           slen, swidth;
  Int2           cxChar, nchars;
  VieweR         v;
  SegmenT        seg;


  v = minfo->v;

  Reset(v);
  DeletePicture(minfo->seg);
  Update();

  minfo->seg = CreatePicture();
  seg = minfo->seg;

  GetPosition(v, &rc);
  winsize = (rc.right-rc.left) - 3*stdCharWidth;
  SetSmallFont();

  name = minfo->name;
  sprintf(str, "  -- %s --  ", name);  
  
  slen = StringLen(str);
  swidth = StringWidth(str);
  cxChar = stdCharWidth;
  nchars= (Int2)ceil((float)winsize/cxChar);

  row += Ing_PrintText(str, seg, row, winsize, RED_COLOR, 0);
  
  temp = (MotifInfoPtr)minfo->motif;
  while (temp != NULL){
    sprintf(str, "%s: %d to %d  Strand: %s  Score: %f", name, temp->start + 1, temp->stop + 1, (temp->strand==Seq_strand_minus)?"minus":"plus", temp->score);
    row += Ing_PrintText(str, seg, row, winsize, BLACK_COLOR, 0);
    temp = temp->next;
  }
  AttachPicture (v, seg, INT4_MIN, INT4_MAX, UPPER_RIGHT, 1 , 1, NULL);

}

static void Ing_CloseMotifWindow(WindoW w)
{
  IngMInfoPtr minfo = NULL;
  MotifInfoPtr motif, tmp;

  minfo = GetObjectExtra(w);
  if (!minfo){
    Remove(w);
    return;
  }
  MemFree(minfo->name);
  motif = (MotifInfoPtr)minfo->motif;
  while (motif){
    tmp = motif->next;
    MemFree(motif);
    motif = tmp;
  }
  Remove(w);
  return;
}

static void Ing_ResizeMotifWindow(WindoW w)
{
  RecT     rcW, rcR, rcVsb, rcHsb;
  Int2     height,width,gap,vsbWidth;
  Int2     hsbWidth,in;
  BaR      vsb;
  BaR      hsb;
  VieweR   v;
  CharPtr  name = NULL;
  IngMInfoPtr minfo = NULL;

  minfo = (IngMInfoPtr) GetObjectExtra(w);
  if (!minfo) return;
  
  v = minfo->v;
  ObjectRect(w, &rcW);
  width= rcW.right-rcW.left;
  height= rcW.bottom-rcW.top;
  SafeHide(v);
  Update();

  vsb = GetSlateVScrollBar ((SlatE) v);
  hsb = GetSlateHScrollBar ((SlatE) v);
  GetPosition(v, &rcR);
  GetPosition(vsb, &rcVsb);
  GetPosition(vsb, &rcHsb);
  vsbWidth=rcVsb.right-rcVsb.left;
  hsbWidth=rcHsb.right-rcHsb.left;

  gap=10;
  in=2;

  rcR.right=rcR.left+width-in-vsbWidth;
  rcR.bottom=rcR.top+height-in-gap-hsbWidth;
  SetPosition(v, &rcR);

  Ing_PopulateMotif(minfo);

  SafeShow(v);
  Update();
}

static WindoW Ing_CreateMotifReport(MotifInfoPtr motif, CharPtr name)
{
  WindoW  w;
  VieweR  v;
  SegmenT pict;
  IngMInfoPtr minfo;
  Int2    left_margin;


  left_margin=5*stdCharWidth;  
  w = DocumentWindow(left_margin, -65 ,550, 250, "Motif Report", Ing_CloseMotifWindow, Ing_ResizeMotifWindow);
  v = CreateViewer(w, 500 ,200, TRUE, TRUE);
  pict = CreatePicture();
  
  minfo = (IngMInfoPtr)MemNew(sizeof(IngMInfo));
  minfo->name = name;
  minfo->motif = (Pointer)motif;
  minfo->v = v;
  minfo->seg = pict;
  Ing_PopulateMotif(minfo);
  SetObjectExtra(w, (Pointer)minfo, StdCleanupExtraProc);

  Show(w);
  return w;
}

/***************************************************

  Function : Ing_CpGProc(); Ing_PolyAProc(); Ing_RepeatProc(); Ing_SignalPeptideProc(); Ing_KozakProc(); Ing_UserMotifProc()
  
  Purpose : Find motif functions

***************************************************/

static void Ing_CpGProc(IteM i)
{
  IngGenomeViewerPtr igvp=NULL;
  WindoW             w=NULL;
  WindoW             ReportW;
  SeqLocPtr          slp = NULL;
  ACT_CGInfoPtr      cpg = NULL, temp = NULL;
  MsgAnswer          ans;
  

  w=ParentWindow(i);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  if (igvp==NULL) return;
  WatchCursor();
  Ing_ProgressNew(igvp, "Searching for CpG islands ....");
 
  cpg = ACT_FindCpG (igvp->bsp);
  
  if (cpg == NULL){
    Message (MSG_OK, "No CpG islands found");
    goto end;
  }

  ReportW = Ing_CreateCpGReport(cpg);

  ans = Message (MSG_YN, "Annotate CpG islands?");
  if (ans == ANS_YES) {
    IngfeatDefTrack[FEATDEF_misc_feature]=2;
    temp = cpg;
    while (temp != NULL){ 
      slp = SeqLocIntNew(temp->from, temp->to, Seq_strand_plus, igvp->bsp->id);
      Ing_CreateNewFeature(igvp->entityID, igvp->itemID, NULL, SEQFEAT_IMP, FEATDEF_misc_feature, "CpG island", slp, NULL);
      temp = temp->next;
    }
    ObjMgrSetDirtyFlag (igvp->entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, igvp->entityID, 0, 0);
  }

 end:
  ArrowCursor();
  Ing_ProgressEnd(igvp);

  if (ans == ANS_NO)
    Select(ReportW);

}

static void Ing_PolyAProc(IteM i)
{
  IngGenomeViewerPtr igvp=NULL;
  WindoW             w=NULL;
  WindoW             ReportW;
  SeqLocPtr          slp = NULL;
  MotifInfoPtr       polya = NULL;
  MotifInfoPtr       temp;
  MsgAnswer          ans;
  CharPtr            name;


  w=ParentWindow(i);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  if (igvp==NULL) return;
  WatchCursor();
  Ing_ProgressNew(igvp, "Searching for Poly A sites ....");
 
  polya = MOT_FindPolyA(igvp->bsp);
  
  if (polya == NULL){
    Message (MSG_OK, "No Poly A sites found");
    goto end;
  }

  name = StringSave("Poly A site");
  ReportW = Ing_CreateMotifReport(polya, name);

  ans = Message (MSG_YN, "Annotate Poly A sites?");
  if (ans == ANS_YES) {
    IngfeatDefTrack[FEATDEF_misc_feature]=2;
    
    temp = polya;
    while (temp != NULL){ 
      slp = SeqLocIntNew(temp->start, temp->stop, temp->strand, igvp->bsp->id);
      Ing_CreateNewFeature(igvp->entityID, igvp->itemID, NULL, SEQFEAT_IMP, FEATDEF_polyA_signal, "Poly A site", slp, NULL);
      temp = temp->next;
    }
    ObjMgrSetDirtyFlag (igvp->entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, igvp->entityID, 0, 0);
  }

 end:
  ArrowCursor();
  Ing_ProgressEnd(igvp);

  if (ans == ANS_NO)
    Select(ReportW);

}

static void Ing_RepeatsProc(IteM i)
{
  IngGenomeViewerPtr igvp=NULL;
  WindoW             w=NULL;
  WindoW             ReportW;
  SeqLocPtr          slp = NULL;
  MotifInfoPtr       repeats = NULL;
  MotifInfoPtr       temp;
  MsgAnswer          ans;
  CharPtr            name;


  w=ParentWindow(i);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  if (igvp==NULL) return;
  WatchCursor();
  Ing_ProgressNew(igvp, "Searching for Repeats ....");
 
  repeats = MOT_FindRepeats(igvp->bsp);
  
  if (repeats == NULL){
    Message (MSG_OK, "No Repeats found");
    goto end;
  }
 
  name = StringSave("Repeats");
  ReportW = Ing_CreateMotifReport(repeats, name);

  ans = Message (MSG_YN, "Annotate Repeats?");
  if (ans == ANS_YES) {
    IngfeatDefTrack[FEATDEF_misc_feature]=2;
    
    temp = repeats;
    while (temp != NULL){ 
      slp = SeqLocIntNew(temp->start, temp->stop, temp->strand, igvp->bsp->id);
      Ing_CreateNewFeature(igvp->entityID, igvp->itemID, NULL, SEQFEAT_IMP, FEATDEF_repeat_region, "Repeat", slp, NULL);
      temp = temp->next;
  }
    ObjMgrSetDirtyFlag (igvp->entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, igvp->entityID, 0, 0);
  }

 end:
  ArrowCursor();
  Ing_ProgressEnd(igvp);

  if (ans == ANS_NO)
    Select(ReportW);
}


static void Ing_KozakProc(IteM i)
{
  IngGenomeViewerPtr igvp=NULL;
  WindoW             w=NULL;
  WindoW             ReportW;
  SeqLocPtr          slp = NULL;
  MotifInfoPtr       kozaks = NULL;
  MotifInfoPtr       temp;
  MsgAnswer          ans;
  CharPtr            name;


  w=ParentWindow(i);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  if (igvp==NULL) return;
  WatchCursor();
  Ing_ProgressNew(igvp, "Searching for Kozak motifs ....");
 
  kozaks = MOT_FindKozak(igvp->bsp);
  
  if (kozaks == NULL){
    Message (MSG_OK, "No Kozak motifs found");
    goto end;
  }
  
  name = StringSave("Kozak");
  ReportW = Ing_CreateMotifReport(kozaks, name);

  ans = Message (MSG_YN, "Annotate Kozak?");
  if (ans == ANS_YES) {
    IngfeatDefTrack[FEATDEF_misc_feature]=2;
    
    temp = kozaks;
    while (temp != NULL){ 
      slp = SeqLocIntNew(temp->start, temp->stop, temp->strand, igvp->bsp->id);
      Ing_CreateNewFeature(igvp->entityID, igvp->itemID, NULL, SEQFEAT_IMP, FEATDEF_misc_feature, "Kozak", slp, NULL);
      temp = temp->next;
    }
    ObjMgrSetDirtyFlag (igvp->entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, igvp->entityID, 0, 0);
  }

 end:
  ArrowCursor();
  Ing_ProgressEnd(igvp);

  if (ans == ANS_NO)
    Select(ReportW);

}

static void Ing_SignalPeptideProc(IteM i)
{
  IngGenomeViewerPtr igvp=NULL;
  WindoW             w=NULL;
  WindoW             ReportW;
  SeqLocPtr          slp = NULL;
  MotifInfoPtr       signal = NULL;
  MotifInfoPtr       temp;
  MsgAnswer          ans;
  CharPtr            name;
  

  w=ParentWindow(i);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  if (igvp==NULL) return;
  WatchCursor();
  Ing_ProgressNew(igvp, "Searching for signal peptide ....");
 
  signal = MOT_FindSignalPeptide(igvp->bsp);
  
  if (signal == NULL){
    Message (MSG_OK, "No Signal Peptide motif found");
    goto end;
  }

  name = StringSave("Signal Peptide");
  ReportW = Ing_CreateMotifReport(signal, name);

  ans = Message (MSG_YN, "Annotate Signal Peptide?");
  if (ans == ANS_YES) {
    IngfeatDefTrack[FEATDEF_misc_feature]=2;
    
    temp = signal;
    while (temp != NULL){ 
      slp = SeqLocIntNew(temp->start, temp->stop, temp->strand, igvp->bsp->id);
      Ing_CreateNewFeature(igvp->entityID, igvp->itemID, NULL, SEQFEAT_IMP, FEATDEF_sig_peptide, "Signal Peptide", slp, NULL);
      temp = temp->next;
    }
    ObjMgrSetDirtyFlag (igvp->entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, igvp->entityID, 0, 0);
  }

 end:
  ArrowCursor();
  Ing_ProgressEnd(igvp);

  if (ans == ANS_NO)
    Select(ReportW);

}


static void Ing_FindMotif(ButtoN b)
{
  IngGenomeViewerPtr igvp=NULL;
  WindoW             w=NULL;
  WindoW             ReportW;
  SeqLocPtr          slp = NULL;
  BioseqPtr          bsp = NULL;
  MotifInfoPtr       user = NULL;
  MotifInfoPtr       temp;
  IngSaveInfoPtr     userinfo;
  Char               motif[100] = {""}, str[50]= {""};
  MsgAnswer          ans;
  CharPtr            name;
  

  userinfo = (IngSaveInfoPtr)GetObjectExtra(b);
  if (!userinfo) return;

  w=ParentWindow(b);
  Hide(w);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  if (igvp==NULL) return;

  WatchCursor();
  Ing_ProgressNew(igvp, "Searching for your motif ....");

  bsp = igvp->bsp;
  GetTitle(userinfo->text, motif, sizeof(motif));
  if (!StringHasNoText(motif))
    user = MOT_MotifSearch(motif, bsp, ISA_aa(bsp->mol));
  else {
    ErrPostEx(SEV_WARNING, 0, 0, "%s", "Enter a motif string");
    goto end;
  }

  if (user == NULL){
    Message (MSG_OK, "Your motif - %s - was not found", motif);
    goto end;
  }

  sprintf(str, "%s", motif);
  name = StringSave(str);
  ReportW = Ing_CreateMotifReport(user, name);

  ans = Message (MSG_YN, "Annotate Motifs?");
  if (ans == ANS_YES) {
    IngfeatDefTrack[FEATDEF_misc_feature]=2;
    
    temp = user;
    while (temp != NULL){ 
      slp = SeqLocIntNew(temp->start, temp->stop, temp->strand, bsp->id);
      Ing_CreateNewFeature(igvp->entityID, igvp->itemID, NULL, SEQFEAT_IMP, FEATDEF_misc_feature, "user motif", slp, NULL);
      temp = temp->next;
    }
    ObjMgrSetDirtyFlag (igvp->entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, igvp->entityID, 0, 0);
  }
  Remove(w);
  ArrowCursor();
  Ing_ProgressEnd(igvp);

  if (ans == ANS_NO)
    Select(ReportW);

  return;
 end:
  Show(w);
  ArrowCursor();
  Ing_ProgressEnd(igvp);
}

static void Ing_UserMotifProc(IteM i)
{
  IngGenomeViewerPtr  igvp = NULL;
  IngSaveInfoPtr      userinfo = NULL;
  GrouP               g, g1, g2;
  WindoW              w;
  ButtoN              b;

  igvp = (IngGenomeViewerPtr)GetObjectExtra(ParentWindow(i));
  if (!igvp) return;

  userinfo = (IngSaveInfoPtr)MemNew(sizeof(IngSaveInfo));

  w = FixedWindow(-50, -33, -10, -10,"Find User Defined Motif", StdCloseWindowProc);
  g = HiddenGroup(w, -2, -2, NULL);
  g1 = HiddenGroup(g, 2, 1, NULL);
  StaticPrompt(g1, "MOTIF string :  ", 0, 0, systemFont, 'l');
  userinfo->text = DialogText(g1, "", 20, NULL);
  g2= HiddenGroup(g, 2, 1, NULL);
  b = DefaultButton(g2, "Find", Ing_FindMotif);
  SetObjectExtra(b, (Pointer)userinfo, StdCleanupExtraProc);
  PushButton(g2, "Cancel", Ing_GenericCancelButton);

  SetObjectExtra(w, (Pointer)igvp, NULL);
  Select(w);
  Show(w);
  return;
}


static void Ing_ShowGCContent(IteM i)
{
  IngGenomeViewerPtr igvp;
  WindoW             w;
  Int4               scaleX;

  w=ParentWindow(i);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  if (igvp==NULL) return;
  WatchCursor();
  Ing_ProgressNew(igvp, "Computing GC content....");
  if (igvp->bShowGC){
    igvp->bShowGC = FALSE;
    SetTitle(igvp->d_Win->GC, "Show GC content");
  }
  else {
    igvp->bShowGC = TRUE;
    SetTitle(igvp->d_Win->GC, "Hide GC content");
  }

  if (igvp->seqbuf==NULL){
    igvp->seqbuf = Ing_FillSeqBuffer(igvp->bsp, igvp->slp, igvp->from, igvp->to);
    seq = igvp->seqbuf;
  }
  if (igvp->seqbuf){
    Ing_GetNamesZoomedVal(igvp->vBottom, NULL, NULL,&scaleX,igvp->SeqLength);
    Ing_UpdateGCGraph(igvp); 
  }

 ArrowCursor();
 Ing_ProgressEnd(igvp);
 return;
}

static void Ing_ShowComplexity(IteM i)
{
  IngGenomeViewerPtr igvp = NULL;
  WindoW             w = NULL;
  SeqLocPtr          slp_feat = NULL;
  SeqLocPtr          slp_dust = NULL;

  w=ParentWindow(i);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  if (igvp==NULL) return;
  WatchCursor();
  Ing_ProgressNew(igvp, "Running Dust....");
  if (igvp->bDustExists==TRUE){ /* already drawn */
    if (!IngfeatDefFilter[FEATDEF_REGION]){
      IngfeatDefTrack[FEATDEF_REGION]=2;
      IngfeatDefFilter[FEATDEF_REGION]=TRUE;
      ObjMgrSendMsg(OM_MSG_UPDATE, igvp->entityID, 0, 0);
    }
    goto end;
  }

  slp_dust = BioseqDust (igvp->bsp, 0, igvp->bsp->length-1, 20, 16, 5, 1);
  
  if (slp_dust == NULL){
    ErrPostEx (SEV_WARNING, 0, 0, "%s", "Ingenue found no low complexity regions in this sequence");
    goto end;
  }
  IngfeatDefTrack[FEATDEF_REGION]=2;
  igvp->bDustExists = TRUE;

  while ((Boolean)(slp_feat=SeqLocFindNext(slp_dust,slp_feat))){ 
    Ing_CreateNewFeature(igvp->entityID, igvp->itemID, NULL, SEQFEAT_IMP, FEATDEF_REGION, "low complexity", slp_feat, NULL);
  }
  ObjMgrSetDirtyFlag (igvp->entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, igvp->entityID, 0, 0);

 end:
  ArrowCursor();
  Ing_ProgressEnd(igvp);
  return;
}

/***************************************************

  Function : Ing_CreateNewFeature(); Ing_MakeSlpList(); Ing_MakeLocListFromSAP()
  
  Purpose :  Create and annotate a new feature; make SeqLocPtr list of start and stops to pass into Ing_CreateNewFeature()

***************************************************/

static SeqFeatPtr Ing_CreateNewFeature(Uint2 entityID, Uint2 itemID, BioseqPtr product, Uint1 seqfeattype, Uint1 featdeftype, CharPtr label, SeqLocPtr slp_feat, CharPtr color)
{
  SeqFeatPtr         sfp=NULL;
  OMProcControl      ompc;
  ObjMgrPtr          omp=NULL;
  ObjMgrProcPtr      ompp=NULL;
  CharPtr            loc=NULL;
  GeneRefPtr         grp=NULL;
  CdRegionPtr        cdr=NULL;
  ImpFeatPtr         ifp=NULL;
  RnaRefPtr          rrp=NULL;
  Uint1              rnatype=0;
  BioseqPtr          bsp = NULL;



  sfp=SeqFeatNew();
  if (!sfp) return NULL;
  sfp->idx.subtype = featdeftype;
  sfp->data.choice = seqfeattype;
  if (StringCmp(label, "low complexity")!= 0){
    sfp->location = (SeqLocPtr)SeqLocPackage(slp_feat);
  }
  else {
    sfp->location = slp_feat;
  }

  SetSeqFeatProduct(sfp, product);

  loc = SeqLocPrint (sfp->location);
  if (loc == NULL) {
    loc = StringSave ("?");
  }

  omp=ObjMgrReadLock();
  if (seqfeattype==SEQFEAT_GENE){
    grp = GeneRefNew ();
    sfp->data.value.ptrvalue = (Pointer) grp;
    ompp=ObjMgrProcFind(omp, 0, "Edit Gene", 0);
    bsp = (BioseqPtr)GetPointerForIDs(entityID, itemID, OBJ_BIOSEQ);
    sfp->location = SeqLocMerge(bsp, sfp->location, NULL, TRUE, FALSE, FALSE);
  }
  else if (seqfeattype==SEQFEAT_CDREGION){
    cdr=CdRegionNew();
    sfp->data.value.ptrvalue = (Pointer) cdr;
    ompp=ObjMgrProcFind(omp, 0, "Edit CdRgn", 0);
  }
  else if (seqfeattype==SEQFEAT_RNA){
/*     sep = SeqMgrGetSeqEntryForData(product); */
/*     if (sep != NULL){ */
/*       vnp = SeqEntryGetSeqDescr(sep, Seq_descr_name, NULL); */
    rrp = RnaRefNew();
    rrp->type = 2;
    sfp->data.value.ptrvalue = (Pointer) rrp;
    ompp=ObjMgrProcFind(omp, 0, "Edit RNA", 0);
  }
  else if (seqfeattype==SEQFEAT_REGION){
    ompp=ObjMgrProcFind(omp, 0, "Edit RgnFeat", 0);
  }
  else if (seqfeattype==SEQFEAT_IMP){
    ifp=ImpFeatNew(); 
    ifp->key = StringSave("misc_feature");
    sfp->data.value.ptrvalue=(Pointer)ifp; 
    sfp->qual = GBQualNew();
    sfp->qual->qual = StringSave("standard_name");
    sfp->qual->val = StringSave(label);
    ompp=ObjMgrProcFind(omp, 0, "Edit Unclassified ImpFeat", 0);
  }

  if (StringHasNoText(label))
    label = (CharPtr) FeatDefTypeLabel (sfp);
  if (label == NULL) {
    label = "?";
  }
/*   sfp->comment=StringSave(label); */
  
  /* tab-delimited files */
  if (color!=NULL)
    sfp->title=StringSave(color);
  else if (label)
    sfp->title=StringSave(label);
  
  MemSet ((Pointer) &ompc, 0, sizeof (OMProcControl));
  ompc.input_entityID = entityID;
  ompc.input_itemID = itemID;
  ompc.input_itemtype = OBJ_BIOSEQ;
  ompc.output_itemtype = OBJ_SEQFEAT;
  ompc.output_data = (Pointer)sfp;
  ompc.proc=ompp;
  ObjMgrUnlock();
  if (! AttachDataForProc (&ompc, FALSE)) {
    Message (MSG_ERROR, "Create Feature failed");
  }
  
  return sfp;
}

typedef struct ing_ivalinfo {
  Int4  start1;
  Int4  stop1;
  Int4  start2;
  Int4  stop2;
  struct ing_ivalinfo PNTR next;
} IngIvalInfo, PNTR IngIvalInfoPtr;


static IngIvalInfoPtr Ing_IvalInfoFree (
  IngIvalInfoPtr ival
)

{
  IngIvalInfoPtr  next;

  while (ival != NULL) {
    next = ival->next;
    MemFree (ival);
    ival = next;
  }
  return NULL;
}

static IngIvalInfoPtr Ing_GetAlignmentIntervals (SeqAlignPtr sap, Int4 from_row, Int4 to_row, Int4 from, Int4 to, Uint1 strand)
{
   AlnMsg2Ptr    amp1;
   AlnMsg2Ptr    amp2;
   IngIvalInfoPtr  ival;
   IngIvalInfoPtr  ival_head;
   IngIvalInfoPtr  ival_prev;
   SeqIdPtr        master = NULL;
   Int4         tmp;
   Int4         from_aln, to_aln;
   Int4         start, stop;


   if (sap == NULL || sap->saip == NULL)
      return NULL;
   strand = AlnMgr2GetNthStrand(sap, 2);

   if (!(from == 0 && to == -1)){ 
     from_aln = AlnMgr2MapBioseqToSeqAlign(sap, from, from_row); 
     to_aln = AlnMgr2MapBioseqToSeqAlign(sap, to, from_row);
     if (from_aln == -2 || to_aln == -2)
       return NULL;
      if (from_aln > to_aln)
       { 
         tmp = from_aln; 
         from_aln = to_aln;
         to_aln = tmp;
        } 
   } 
   else{
     from_aln= from;
     to_aln = to;
   }
   amp1 = AlnMsgNew2();
   amp1->from_aln = from_aln;
   amp1->to_aln = to_aln;
   amp1->row_num = from_row;
   amp2 = AlnMsgNew2();
   amp2->from_aln = from_aln;
   amp2->to_aln = to_aln;
   amp2->row_num = to_row;
   ival_head = ival_prev = NULL;
   while (AlnMgr2GetNextAlnBit(sap, amp1) && AlnMgr2GetNextAlnBit(sap, amp2))
   {
      if (amp1->type == AM_SEQ && amp2->type == AM_SEQ)
      {
         ival = (IngIvalInfoPtr)MemNew(sizeof(IngIvalInfo));
         ival->start1 = amp1->from_row;
         ival->stop1 = amp1->to_row;
         ival->start2 = amp2->from_row;
         ival->stop2 = amp2->to_row;
         if (Seq_strand_minus == strand){
           if (ival_head == NULL)
             ival_head = ival;
           else {
             ival->next = ival_head;
             ival_head = ival;
           }
         }
         else {
           if (ival_head == NULL)
             ival_head = ival_prev = ival;
           else
             {
               ival_prev->next = ival;
               ival_prev = ival;
             }
         }
      }
   }
   AlnMsgFree2(amp1);
   AlnMsgFree2(amp2);

   return ival_head;
}


static IngIvalInfoPtr Ing_OldGetAlignmentIntervals (SeqAlignPtr sap, Int4 from_row, Int4 to_row, Int4 from, Int4 to, Uint1 strand)
{
   AlnMsg2Ptr    amp1;
   AlnMsg2Ptr    amp2;
   IngIvalInfoPtr  ival;
   IngIvalInfoPtr  ival_head;
   IngIvalInfoPtr  ival_prev;
   SeqIdPtr        master = NULL;
   Int4         tmp;
   Int4         from_aln, to_aln;
   Int4         start, stop;


   if (sap == NULL || sap->saip == NULL)
      return NULL;
   if (!(from == 0 && to == -1)){ 
     from_aln = AlnMgr2MapBioseqToSeqAlign(sap, from, from_row); 
     to_aln = AlnMgr2MapBioseqToSeqAlign(sap, to, from_row);
     if (from_aln == -2 || to_aln == -2)
       return NULL;
      if (from_aln > to_aln)
       { 
         tmp = from_aln; 
         from_aln = to_aln;
         to_aln = tmp;
        } 
   } 
   else{
     from_aln= from;
     to_aln = to;
   }
   amp1 = AlnMsgNew2();
   amp1->from_aln = from_aln;
   amp1->to_aln = to_aln;
   amp1->row_num = from_row;
   amp2 = AlnMsgNew2();
   amp2->from_aln = from_aln;
   amp2->to_aln = to_aln;
   amp2->row_num = to_row;
   ival_head = ival_prev = NULL;
   while (AlnMgr2GetNextAlnBit(sap, amp1) && AlnMgr2GetNextAlnBit(sap, amp2))
   {
      if (amp1->type == AM_SEQ && amp2->type == AM_SEQ)
      {
         ival = (IngIvalInfoPtr)MemNew(sizeof(IngIvalInfo));
         ival->start1 = amp1->from_row;
         ival->stop1 = amp1->to_row;
         ival->start2 = amp2->from_row;
         ival->stop2 = amp2->to_row;
         if (ival_head == NULL)
            ival_head = ival_prev = ival;
         else
         {
            ival_prev->next = ival;
            ival_prev = ival;
         }
      }
   }
   AlnMsgFree2(amp1);
   AlnMsgFree2(amp2);
   return ival_head;
}


static IngIvalInfoPtr MergeAdjacentIntervals (
  IngIvalInfoPtr list
)

{
  IngIvalInfoPtr  curr, last, next;

  if (list != NULL) {
    curr = list->next;
    last = list;
    while (curr != NULL) {
      next = curr->next;
      if (curr->start2 == last->stop2 + 1) {
        last->stop2 = MAX (curr->stop2, last->stop2);
        MemFree (curr);
        last->next = next;
      } else {
        last = curr;
      }
      curr = next;
    }
  }
  return list;
}


static SeqLocPtr Ing_MakeLocListFromSAP(SeqAlignPtr salp, Boolean gap_split)
{
  SeqLocPtr    head = NULL;
  IngIvalInfoPtr  ival;
  IngIvalInfoPtr  ival_head;
  SeqIntPtr    sinp;
  SeqLocPtr    slp;
  SeqPntPtr    spp;
  Uint1        strand;
  SeqIdPtr     sip = NULL;
  Int4         from_row, to_row;

  sip = AlnMgr2GetNthSeqIdPtr(salp, 2);
  strand=AlnMgr2GetNthStrand(salp, 2);
  from_row = 2;
  to_row = 1;
  ival_head = (IngIvalInfoPtr)Ing_GetAlignmentIntervals (salp, from_row, to_row, 0, -1, strand);
  if (!ival_head) return NULL;
  ival_head = MergeAdjacentIntervals (ival_head);
  if (ival_head != NULL) {
    if (gap_split) {
      for (ival = ival_head; ival != NULL; ival = ival->next) {
        slp = SeqLocIntNew(ival->start1, ival->stop1, strand, sip);
        SeqLocAdd (&head, slp, TRUE, FALSE);
      }
    } else {
      for (ival = ival_head; ival->next != NULL; ival = ival->next) continue;
      slp = SeqLocIntNew(ival_head->start1, ival->stop1, strand, sip);
      SeqLocAdd (&head, slp, TRUE, FALSE);
    }
  }
  Ing_IvalInfoFree (ival_head);

  return head;
}

static SeqLocPtr Ing_OldMakeLocListFromSAP(SeqAlignPtr sap)
{
  SeqLocPtr slp_head = NULL;
  SeqLocPtr slp1 = NULL, slp2 = NULL;
  SeqLocPtr slp = NULL;
  AlnMsg2Ptr amp1 = NULL;
  AlnMsg2Ptr amp2 = NULL;
  SeqIdPtr  sip = NULL;
  Uint1     strand = 0;
  Boolean   more = FALSE;
  Boolean   bLastmasterwasgap = FALSE;
  Int4      i = 0, j = 0;
  Int4      numivals = 0;
  Int4Ptr   ivals = NULL;


  sip=AlnMgr2GetNthSeqIdPtr(sap, 1);
  strand=AlnMgr2GetNthStrand(sap, 2);
  numivals= AlnMgr2GetNumSegs(sap);

  ivals = (Int4Ptr)MemNew(sizeof(Int4)*2*numivals);

  amp1 = AlnMsgNew2();
  amp2 = AlnMsgNew2();
  amp1->from_aln = 0;
  amp1->to_aln = -1;
  amp1->row_num = 1;
  amp2->from_aln = 0;
  amp2->to_aln = -1; 
  amp2->row_num = 2;
  while ((Boolean)(more = AlnMgr2GetNextAlnBit(sap, amp2))){
    AlnMgr2GetNextAlnBit(sap, amp1);
    if (amp2->type == AM_SEQ){
      if (amp1->type == AM_GAP){
        bLastmasterwasgap = TRUE;
      }
      else {
        if (Seq_strand_minus == strand){
          if (!bLastmasterwasgap){
            i += 2;
            ivals[i-2] = amp1->from_row;
          }
          else 
            bLastmasterwasgap = FALSE;
          ivals[i-1] = amp1->to_row;
        }
        else {
          slp=SeqLocIntNew(amp1->from_row, amp1->to_row, strand, sip);
          if (slp_head==NULL){
            slp_head=slp;
          }
          else {
            SeqLocAdd(&slp_head, slp, TRUE, FALSE);
          }
        }
      }
    }
  }
  AlnMsgFree2(amp1);
  AlnMsgFree2(amp2);
  /* repack if on minus strand */
  if (Seq_strand_minus == strand){
    for(j = i - 1; j >= 0; j -= 2){
      slp=SeqLocIntNew(ivals[j-1], ivals[j], strand, sip);
      if (slp_head==NULL){
        slp_head=slp;
      }
      else {
        SeqLocAdd(&slp_head, slp, TRUE, FALSE);
      }
    }
  }
  MemFree(ivals);

  return slp_head;
}


static SeqLocPtr Ing_MakeSlpList(ValNodePtr vnp_list, Int4 numivals, Int4Ptr ivals, Uint1 strand, SeqIdPtr sip)
{
  SeqLocPtr  slp_head=NULL;
  SeqLocPtr  slp=NULL;
  Int4       i;
  ValNodePtr vnp=NULL;

  if (vnp_list){
    vnp=vnp_list;
    while (vnp !=NULL){
      slp=(SeqLocPtr)vnp->data.ptrvalue;
      if (slp_head==NULL){
        slp_head=slp;
      }
      else {
        SeqLocAdd(&slp_head, slp, TRUE, FALSE);
      }
      vnp=vnp->next;
    }
    ValNodeFree(vnp_list);
  }
  else {
    if (!(strand == Seq_strand_minus)){
      for(i=0; i<(2*numivals); i+=2){
        slp=SeqLocIntNew(ivals[i], ivals[i+1], strand, sip);
        if (slp_head==NULL){
          slp_head=slp;
        }
        else {
          SeqLocAdd(&slp_head, slp, TRUE, FALSE);
        }
      }
    }
    else {
      for(i=(2*numivals)-1; i>=0; i-=2){
        slp=SeqLocIntNew(ivals[i-1], ivals[i], strand, sip);
        if (slp_head==NULL){
          slp_head=slp;
        }
        else {
          SeqLocAdd(&slp_head, slp, TRUE, FALSE);
        }
      }
    }
  } 
  return slp_head;
}



/*******************************************************************************

  Function : Ing_RunORFinder(), Ing_DotMatrixProc()
  
  Purpose :  Calls to toolkit applications
*******************************************************************************/

static void Ing_RunORFinder(IteM i)
{
  WindoW             w;
  IngGenomeViewerPtr igvp=NULL;
  ValNodePtr         orflist=NULL;
  ValNodePtr         vnp=NULL;
  SeqLocPtr          slp=NULL;
  


  w=ParentWindow(i);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  if (igvp==NULL) return;
  WatchCursor();
  Ing_ProgressNew(igvp, "Running ORF finder....");
  if (igvp->bOrfExists==TRUE){
    if (!IngfeatDefFilter[FEATDEF_misc_feature]){
      IngfeatDefTrack[FEATDEF_misc_feature]=2;
      IngfeatDefFilter[FEATDEF_misc_feature]=TRUE;
      ObjMgrSendMsg(OM_MSG_UPDATE, igvp->entityID, 0, 0);
    }
    goto end;
  }
  orflist = (ValNodePtr)GetOrfList(igvp->bsp, 200);
  if (orflist==NULL){
    ErrPostEx (SEV_WARNING, 0, 0, "%s", "Ingenue found no open reading frames in this sequence");
    goto end;
  }
  IngfeatDefTrack[FEATDEF_misc_feature]=2;
  igvp->bOrfExists = TRUE;
  orflist =  (ValNodePtr)ClearNonMetOrfs (orflist);
  vnp=orflist;
  while (vnp){
    slp=(SeqLocPtr)vnp->data.ptrvalue;
    Ing_CreateNewFeature(igvp->entityID, igvp->itemID,NULL, SEQFEAT_IMP, FEATDEF_misc_feature, "orf", slp, NULL);
    vnp=vnp->next;
  }
  ValNodeFree(orflist);
  ObjMgrSetDirtyFlag (igvp->entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, igvp->entityID, 0, 0);

 end:
  ArrowCursor();
  Ing_ProgressEnd(igvp);
  return;
}

static void Ing_DotMatrixProc(IteM i)
{
   SeqAlignPtr     sap=NULL;
   SelStructPtr    ssp;
   IngGenomeViewerPtr igvp;


   igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(i));
   ssp = ObjMgrGetSelected();
   if (ssp == NULL){
     Ing_GetFileForDotMatrix(i);
     ArrowCursor();
     return;
   }
   if (ssp->itemtype != OBJ_SEQALIGN)
     {
       ErrPostEx(SEV_ERROR, 0, 0, "MUST select seqalign");
       ArrowCursor();
       return;
     }
   if (ssp)
     {
       sap = (SeqAlignPtr)GetPointerForIDs(ssp->entityID, ssp->itemID, OBJ_SEQALIGN);
       if (sap == NULL)
          {
            ErrPostEx(SEV_ERROR, 0, 0, "Error in copying seqalign");
            ArrowCursor();
            return;
          }
       Ing_CreateDotMatrixForm(igvp, sap, NULL, NULL);
     }
   
   ArrowCursor();
   return;

}





/*******************************************************************************

  Function(s) : Ing_SelectPrim(), Ing_DeSelectPrim(), Ing_DeSelectGenericPrim()
  
  Purpose : ExploreSegment() select/deselect callbacks

*******************************************************************************/
static Boolean Ing_SelectPrim (SegmenT seg, PrimitivE prim, Uint2 segID, Uint2 primID, Uint2 primCt, VoidPtr userdata)
{
  Uint2      entityID=0;
  Uint4      itemID=0;
  Uint2      itemtype=0;
  IngSelectPtr selp=NULL;

  if (!prim) return TRUE;

  selp = (IngSelectPtr) userdata;
  GetPrimitiveIDs(prim, &entityID, &itemID, &itemtype, 0);

  if (entityID == selp->entityID &&
      itemID == selp->itemID && itemtype == selp->itemtype){
    if (primID){
      HighlightPrimitive(selp->vwr, seg, prim, FRAME_PRIMITIVE);
      if (selp->returnFALSE)
        return FALSE;
    }
    else {
       HighlightSegment (selp->vwr, seg, FRAME_SEGMENT); 
       if (selp->returnFALSE)
         return FALSE; 
    } 
  }
  return TRUE;
}


static Boolean Ing_DeSelectPrim (SegmenT seg, PrimitivE prim, Uint2 segID, Uint2 primID, Uint2 primCt, VoidPtr userdata)

{
  Uint2      entityID=0;
  Uint4      itemID=0;
  Uint2      itemtype=0;
  IngSelectPtr  selp=NULL;
  
  if (!prim) return TRUE;

  selp = (IngSelectPtr) userdata;
  GetPrimitiveIDs(prim, &entityID, &itemID, &itemtype, NULL);

  if (entityID == selp->entityID &&
      itemID == selp->itemID &&
      itemtype == selp->itemtype){
    if (primID){
      HighlightPrimitive (selp->vwr, seg, prim, PLAIN_PRIMITIVE);
      if (selp->returnFALSE)
        return FALSE;
    }  
    else {
      HighlightSegment(selp->vwr, seg, PLAIN_SEGMENT);
      if (selp->returnFALSE) 
        return FALSE; 
    } 
  }
  return TRUE;
}


static Boolean Ing_DeSelectAll(SegmenT seg, PrimitivE prim, Uint2 segID,  Uint2 primID, Uint2 primCt, VoidPtr userdata)

{
  Int1               highlight=0;
  IngSelectPtr       selp=NULL;

  
  if (!prim) return TRUE;

  selp = (IngSelectPtr) userdata;  

  if (primID){
    GetPrimDrawAttribute (prim, NULL, NULL, NULL, NULL, NULL, &highlight);
    if (highlight == FRAME_PRIMITIVE)
      HighlightPrimitive (selp->vwr, seg, prim, PLAIN_PRIMITIVE);
  }
  else {
    highlight = SegmentStyle(seg);
    if (highlight == FRAME_SEGMENT)
      HighlightSegment (selp->vwr, seg, PLAIN_SEGMENT);
  }

  return TRUE;

}


static Boolean Ing_DeSelectGenericPrim (SegmenT seg, PrimitivE prim, Uint2 segID,Uint2 primID, Uint2 primCt, VoidPtr userdata)
{

  Int1  highlight;
  VieweR  v;

  v = (VieweR)userdata;
  GetPrimDrawAttribute (prim, NULL, NULL, NULL, NULL, NULL, &highlight);
  if (highlight != PLAIN_PRIMITIVE) 
    HighlightPrimitive (v, seg, prim, PLAIN_PRIMITIVE);

  return TRUE;
}


/***********************************************************************
Function: Ing_InitAlignArrays()

Purpose : Initialize alignment tracking arrays

*************************************************************/
extern void Ing_InitAlignArrays(IngTrackAlignsPtr tap, Boolean bFirst)
{
  if (!bFirst){
    if (tap->hidelist)
      ValNodeFree(tap->hidelist);
    if (tap->showlist)
      ValNodeFree(tap->showlist);
    if (tap->namelist)
      MemFree(tap->namelist);
    if (tap->hnamelist)
      MemFree(tap->hnamelist);
  }

  tap->hidelist = NULL;
  tap->showlist = NULL;
  tap->namelist = NULL;
  tap->hnamelist = NULL;
  tap->showindex =0;
  tap->hideindex = 0;
  tap->update = TRUE;
}

/*******************************************************************************

  Function(s) : Ing_InitMainDataPtr(), Ing_MainDataNew(), Ing_MainDataFree()
  
  Purpose : to create, initialize, or free the main data structure

*******************************************************************************/

static void Ing_InitMainDataPtr(IngGenomeViewerPtr igvp)
{
  igvp->hMain=NULL;
  igvp->hReport=NULL;
  igvp->slp=NULL;
  igvp->bsp=NULL;
  igvp->bspIsLocked=FALSE;
  igvp->filetype=0;
  igvp->seqbuf=NULL;
  igvp->vRuler1=NULL;
  igvp->vTop=NULL;
  igvp->vRuler2=NULL;
  igvp->vTop=NULL;
  igvp->bOverviewSelected=FALSE;
  igvp->bDustExists=FALSE;
  igvp->bOrfExists=FALSE;
  igvp->bShowGC = FALSE;
  igvp->TopHeight = 200;
  igvp->last_feat_position_top=0;
  igvp->last_feat_position_bottom = 0;
  igvp->xposition=0;
  igvp->ZoomedVal=1;
  igvp->scaleX=1;
  igvp->maxScaleX=1;
  igvp->bMaxScale=FALSE;
  igvp->defline=NULL; /* feature descriptors */
  igvp->defline2 = NULL;
  igvp->data = NULL;
  igvp->bLabels=TRUE; /* show-hide labels on features */
  igvp->isExons = FALSE; /* option to display spidey output */
  /* init progress window vars */
  igvp->progress_string=NULL;
  igvp->progress_timer=0;
  igvp->progress_running=FALSE;

  /* init align tracking arrays */
  Ing_InitAlignArrays(&igvp->talign, TRUE);

}

extern IngGenomeViewerPtr Ing_MainDataNew(void)
{
  IngGenomeViewerPtr igvp=NULL;

  igvp=(IngGenomeViewerPtr)MemNew(sizeof(IngGenomeViewer));
  if (!igvp) return NULL;
  Ing_InitMainDataPtr(igvp);
  return igvp;

}

static IngGenomeViewerPtr Ing_MainDataFree(IngGenomeViewerPtr igvp)
{
  if (igvp->vZoom) 
    igvp->vZoom=DeleteViewer(igvp->vZoom); 
  if (igvp->vTop)  
    igvp->vTop=DeleteViewer(igvp->vTop); 
  if (igvp->vBottom) 
    igvp->vBottom=DeleteViewer(igvp->vBottom);
  if (igvp->vRuler1)
    igvp->vRuler1=DeleteViewer(igvp->vRuler1);
  if (igvp->vRuler2) 
    igvp->vRuler2=DeleteViewer(igvp->vRuler2);
  if (igvp->scale_array)MemFree(igvp->scale_array);
  if (igvp->slp) SeqLocFree(igvp->slp);
  if (igvp->seqbuf)  MemFree(igvp->seqbuf);
  if (igvp->GrData.pClr)  Ing_FreeColorTable(igvp->GrData.pClr);
  if (igvp->defline) MemFree(igvp->defline);
  if (igvp->defline2) MemFree(igvp->defline2);
  if (igvp->entity_list) MemFree(igvp->entity_list);

  Ing_InitAlignArrays(&igvp->talign, FALSE);

  return igvp;
}

/*******************************************************************************

  Function(s) : Ing_AllButPartsCount()
  
  Purpose :  counter the number of bioseqs given a seqentry

*******************************************************************************/

static Int4 Ing_AllButPartsCount (SeqEntryPtr sep, Pointer mydata,
                             SeqEntryFunc mycallback,
                             Int4 index, Int2 indent)
{
  BioseqSetPtr  bssp;

  if (sep == NULL) return index;
  if (IS_Bioseq (sep)) {
    if (mycallback != NULL)
      (*mycallback) (sep, mydata, index, indent);
    return index + 1;
  }
  if (Bioseq_set_class (sep) != 4) {
    if (mycallback != NULL)
      (*mycallback) (sep, mydata, index, indent);
    index++;
  }
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  sep = bssp->seq_set;
  indent++;
  while (sep != NULL) {
    index = Ing_AllButPartsCount (sep, mydata, mycallback, index, indent);
    sep = sep->next;
  }
  return index;
}

/*******************************************************************************

  Function(s) : Ing_ResetTitle(); Ing_GetFastaTitle(); Ing_GetScoreAndEvalue()
  
  Purpose :  set title for highlighted object; get full length fasta description

*******************************************************************************/
static void Ing_ResetTitle(IngGenomeViewerPtr igvp, Boolean clear)
{
  RecT      rc;
  Int2      width, stringwd;
  Int2      nchars, nrows;
  Int2      nlines;
  Int2      len;
  CharPtr   c=NULL, end=NULL, str=NULL;
  Int4      i;
  
  
  SetTitle(igvp->featInfo[0], "");
  SetTitle(igvp->featInfo[1], "");
  SetTitle(igvp->featInfo[2], "");
  SetTitle(igvp->featInfo[3], "");
  if (!igvp->defline) return;
  if (clear && igvp->defline){
    MemFree(igvp->defline);
    igvp->defline=NULL;
    if (igvp->defline2){
      MemFree(igvp->defline2);
      igvp->defline2 = NULL;
    }
    return;
  }
  
  GetPosition(igvp->deflineg, &rc);
  nchars=MIN(130, (rc.right-rc.left)/igvp->GrData.cxChar);
  len=StringLen(igvp->defline);
  nlines = (Int2)ceil((float)len/nchars);
  for (i=0; i<3; i++){
    if ((i+1)>nlines){
      SetTitle(igvp->featInfo[i], "");
    }
    else {
      str=StringSave(igvp->defline);
      c=str+(i*nchars);
      len=StringLen(c);
      nchars=MIN(len, nchars);
      end=c+nchars;
      *end='\0';
      SetTitle(igvp->featInfo[i], c);
      MemFree(str);
    }
  }
  SetTitle(igvp->featInfo[i], igvp->defline2);
}


static CharPtr Ing_GetFastaTitle (BioseqPtr bsp, CharPtr pretext, CharPtr posttext)
{
  CharPtr   title = NULL;
  CharPtr   prep, postp, predum = "", postdum = "";
  Char      idbuf[L_ID], defbuf[L_DEF];
  Int4      prelen = 0, postlen = 0;
  SeqIdPtr  sip;

  if (bsp != NULL)
  {
    if ((sip = bsp->id) != NULL)
    {
      if (pretext != NULL)
      {
        prelen = StrLen (pretext);
        prep = pretext;
      }
      else
      {
        prep = predum;
      }
      if (posttext != NULL)
      {
        postlen = StrLen (posttext);
        postp = posttext;
      }
      else
      {
        postp = postdum;
      }
      if ((title = (CharPtr) MemNew (sizeof (Char) * (L_T+prelen+postlen)))
          != NULL)
      {
        switch (sip->choice)
        {
         case SEQID_LOCAL:
          idbuf[0] = '\0';
          if (sip->data.ptrvalue != NULL)
            StrNCpy (idbuf, ((ObjectIdPtr) sip->data.ptrvalue)->str, L_ID-1);
          idbuf[L_ID-1] = '\0';
          break;
         default:
          FastaId (bsp, idbuf, L_ID);
          break;
        }
        FastaDefLine (bsp, defbuf, L_DEF, NULL, NULL, 0);
        sprintf (title, "%s%s %s%s", prep, idbuf, defbuf, postp);
      }
    }
  }
  return title;
}


extern void Ing_GetScoreAndEvalue(SeqAlignPtr sap, CharPtr PNTR eval_buff, CharPtr PNTR bit_score_buff)
{
  Int4 score, number;
  FloatHi   bit_score, evalue;
  Char      buff1[10]={""}, buff2[10]={""};

  GetScoreAndEvalue(sap, &score, &bit_score, &evalue, &number);
  if (evalue < 1.0e-180)
    sprintf (buff1, "0.0");
  else if (evalue < 1.0e-99)
    sprintf (buff1, "%2.0le", evalue);
  else if (evalue < 0.0009)
    sprintf (buff1, "%3.1le", evalue);
  else if (evalue < 1.0)
    sprintf (buff1, "%4.3lf", evalue);
  else
    sprintf (buff1, "%5.1lf", evalue);
  *eval_buff = StringSave(buff1);

  if (bit_score > 9999)
    sprintf(buff2, "%4.3le", bit_score);
  else if (bit_score > 99.9)
    sprintf(buff2, "%4.1lf", bit_score);
  else
    sprintf(buff2, "%4.2lf", bit_score);
  *bit_score_buff = StringSave(buff2);
}


/*******************************************************************************

  Function : Ing_MsgFlush()
  
  Purpose : kill the viewer in response to a OM_MSG_FLUSH message
  
  Return value : OM_MSG_RET_OK if success

*******************************************************************************/
static Int2 Ing_MsgFlush(IngGenomeViewerPtr igvp,OMMsgStructPtr ommsp)
{

	if (igvp->entityID==ommsp->entityID &&
		((igvp->itemID==ommsp->itemID && ommsp->itemtype==OBJ_BIOSEQ)
        || (ommsp->itemID == 0 && ommsp->itemtype==0))){
     if (igvp->bspIsLocked)
       BioseqUnlock(igvp->bsp);
     ObjMgrFreeByEntityID(igvp->entityID);
     Ing_MainDataFree(igvp); 
     Remove(igvp->hMain);
#ifdef ING_STANDALONE
     WinCounter--;
     if (!WinCounter)
       Ing_StartupWindow();
#endif
	}
	return(OM_MSG_RET_OK);
}

/*******************************************************************************

  Function : Ing_OM_MsgFunc()
  
  Purpose : message callback for the viewer

*******************************************************************************/
static Int2 LIBCALLBACK Ing_OM_MsgFunc (OMMsgStructPtr ommsp)
{
  OMUserDataPtr      omudp;
  IngGenomeViewerPtr igvp=NULL;
  IngSelectData    sel;
  SeqMgrFeatContext  sfc;
  Int4               start=0, stop=0;
  Int4               s1=0, s2=0, n=1;
  Int4               score=0, number=0;
  AlnMsg2Ptr          amp;
  Boolean            refresh=FALSE;
  Char               buf[255]={""};
  CharPtr            str1=NULL;
  Char            str2[300]={""};
  CharPtr            loc=NULL;
  BioseqPtr          bsp=NULL, bsp_sap=NULL;
  SeqAlignPtr        salp=NULL;
  SeqIdPtr           sip=NULL;
  SeqEntryPtr        sep=NULL;
  Uint2              entityID=0;
  Uint2              itemID=0;
  Uint2              itemtype=0;
  Uint2              segID=0;
  Int2               highlight;
  IngReportPtr       Report=NULL;
  CharPtr            eval_buff, bit_score_buff;


  
  omudp = (OMUserDataPtr)(ommsp->omuserdata);
  igvp = (IngGenomeViewerPtr)omudp->userdata.ptrvalue;
/*   if (igvp->procID != omudp->procid) return OM_MSG_RET_OK; */
  bsp=GetBioseqGivenIDs (igvp->entityID, igvp->itemID, OBJ_BIOSEQ);
  if (!bsp) return (OM_MSG_RET_OK);
  if (!igvp) return(OM_MSG_RET_OK);
  
  if (igvp->hReport)
    Report=(IngReportPtr)GetObjectExtra(igvp->hReport);

  switch (ommsp->message)
    {
    case OM_MSG_DEL:
      break;
    case OM_MSG_CREATE:
      break;
    case OM_MSG_UPDATE:
      if (ObjMgrGetDirtyFlag (ommsp->entityID))
        refresh=TRUE;
      break;
    case OM_MSG_SELECT:
      entityID=ommsp->entityID;
      itemID=ommsp->itemID;
      itemtype=ommsp->itemtype;

      MemSet((Pointer)buf, '\0', sizeof(buf));

      sel.entityID=entityID;
      sel.itemID=itemID;
      sel.itemtype=itemtype;
      sel.segID=msg.segID;

      if (itemtype == OBJ_BIOSEQ ||
          itemtype == OBJ_SEQID){
        sel.vwr = igvp->vRuler2;
        sel.returnFALSE= TRUE;
        ExploreSegment(igvp->pictRuler2, (Pointer) &sel, Ing_SelectPrim);
      }
      else {
        /* highlight object in overview if picture is not compressed */
        if (igvp->maxScaleX<COMPRESS_SCALE){
          sel.vwr=igvp->vTop;
          sel.returnFALSE=FALSE;
          ExploreSegment (igvp->pictTop, (Pointer) &sel, Ing_SelectPrim);
          igvp->bOverviewSelected=TRUE;
        }
        
        /* highlight object in feature window */
        sel.vwr=igvp->vBottom;      
        sel.returnFALSE=TRUE;
        ExploreSegment (igvp->pictBottom, (Pointer) &sel, Ing_SelectPrim);
      }
      /* get defline label */
      
      if (msg.labelmsg==0) {
        Ing_ResetTitle(igvp, TRUE);
      }
      else if (msg.labelmsg == 2) {
        if (itemtype == OBJ_BIOSEQ || 
            itemtype == OBJ_SEQID){
          if (itemtype == OBJ_BIOSEQ){
            bsp = (BioseqPtr)GetPointerForIDs(ommsp->entityID, ommsp->itemID, OBJ_BIOSEQ);
          }
          else {
            sip =(SeqIdPtr)GetPointerForIDs(ommsp->entityID, ommsp->itemID, OBJ_SEQID);
            bsp = BioseqLockById(sip);
          }
          if (bsp) {
            str1=Ing_GetFastaTitle(bsp, NULL, NULL);
            if (igvp->defline)
              MemFree(igvp->defline);
            igvp->defline = StringSave(str1);

            if (igvp->defline2)
              MemFree(igvp->defline2);
            igvp->defline2 = NULL;
          }
          else {
            if (igvp->defline)
              MemFree(igvp->defline);
            igvp->defline=StringSave("Sequence: Unknown");
            if (igvp->defline2){
              MemFree(igvp->defline2);
              igvp->defline2 = NULL;
            }
          }
          Ing_ResetTitle(igvp, FALSE);
        }
        else if (itemtype==OBJ_SEQFEAT) {
          SeqMgrGetDesiredFeature(entityID, bsp, itemID,  0, NULL, &sfc);
          if (&sfc)
            {
              FeatDefLabel(sfc.sfp, buf, sizeof(buf)-1, OM_LABEL_BOTH);
              sprintf(str2, "%.150s", buf);
              if (igvp->defline)
                MemFree(igvp->defline);
              igvp->defline=StringSave(str2);
              
              loc=SeqLocPrint(sfc.sfp->location);
              if (loc)
                sprintf(str2, "%.150s", loc);
              else 
                sprintf(str2, "len: %d - %d", sfc.left, sfc.right);
              if (igvp->defline2)
                MemFree(igvp->defline2);
              igvp->defline2 = StringSave(str2);
            }
          else{
            if (igvp->defline)
              MemFree(igvp->defline);
            igvp->defline=StringSave("Feature: Unknown");
            if (igvp->defline2){
              MemFree(igvp->defline2);
              igvp->defline2 = NULL;
            }
          }
          Ing_ResetTitle(igvp, FALSE);
        }
        else if (itemtype==OBJ_SEQALIGN){  
          salp=(SeqAlignPtr)GetPointerForIDs(ommsp->entityID, ommsp->itemID, OBJ_SEQALIGN);
          sip=AlnMgr2GetNthSeqIdPtr(salp, 2);
          AlnMgr2GetNthSeqRangeInSA(salp, 1, &start, &stop);

          MemSet((Pointer)str2, '\0', sizeof(str2));
          bsp_sap=BioseqLockById(sip);
          if (bsp_sap){
            str1=Ing_GetFastaTitle(bsp_sap, "SeqAlign: ", NULL);
            loc=SeqLocPrint(salp->bounds);
            if (salp->score){
              MemSet((Pointer)buf, '\0', sizeof(buf));
              Ing_GetScoreAndEvalue(salp, &eval_buff, &bit_score_buff);
              if (loc)
                sprintf(buf, " %.150s  Score = %s  Expect = %s", loc, bit_score_buff, eval_buff);
              else
                sprintf(buf, "len: %d - %d  Score = %s  Expect = %s", start + 1, stop + 1, bit_score_buff, eval_buff);
              MemFree(eval_buff);
              MemFree(bit_score_buff);
            }
            else {
              if (loc)
                sprintf(buf, " %.150s",  loc);
              else 
                sprintf(buf, "len: %d - %d", start + 1, stop + 1);
            }

            BioseqUnlock(bsp_sap);

            if (igvp->defline)
              MemFree(igvp->defline);
            igvp->defline=StringSave(str1);
            if (igvp->defline2)
              MemFree(igvp->defline2);
            igvp->defline2 = StringSave(buf);
            Ing_ResetTitle(igvp, FALSE);
          }
          else {
            if (salp){
              sprintf(str2, "%s", "SeqAlign: ");    
              if (sip){
                MemSet((Pointer)buf, '\0', sizeof(buf));
                SeqIdWrite(sip, buf, PRINTID_FASTA_LONG, 41);
                StringCat(str2, buf);
                AlnMgr2IndexSeqAlign(salp);
                
                loc=SeqLocPrint(salp->bounds);
                if (salp->score){
                  MemSet((Pointer)buf, '\0', sizeof(buf));
                  Ing_GetScoreAndEvalue(salp, &eval_buff, &bit_score_buff);
                  if (loc)
                    sprintf(buf, " %.150s  Score = %s  Expect = %s", loc, bit_score_buff, eval_buff);
                  else
                    sprintf(buf, "len: %d - %d  Score = %s  Expect = %s", start + 1, stop + 1, bit_score_buff, eval_buff);
                  MemFree(eval_buff);
                  MemFree(bit_score_buff);
                }
                else {
                  if (loc)
                    sprintf(buf, " %.150s",  loc);
                  else 
                    sprintf(buf, "len: %d - %d", start + 1, stop + 1);
                }
              }
              else {
                StringCat(str2, "Unknown");
              }
            }
            if (igvp->defline)
              MemFree(igvp->defline);
            igvp->defline=StringSave(str2);
            if (igvp->defline2)
              MemFree(igvp->defline2);
            igvp->defline2 = StringSave(buf);
            Ing_ResetTitle(igvp, FALSE);
          }
        }
      }
      
      
      /* highlight spidey report */
      if (itemtype==OBJ_SEQALIGN && Report!=NULL && msg.segID){
       Ing_HighlightReportWindow(Report, FRAME_SEGMENT, msg.segID, (igvp->isExons?TRUE:FALSE));
       Report->isPrevSelected=TRUE;
       Report->last_segID=msg.segID;
     }
     break;
   case OM_MSG_DESELECT:

     sel.entityID=ommsp->entityID;
     sel.itemID=ommsp->itemID;
     sel.itemtype=ommsp->itemtype;
     if (sel.itemtype == OBJ_BIOSEQ || sel.itemtype == OBJ_SEQID){
       sel.returnFALSE=FALSE;
       sel.vwr=igvp->vRuler2;
       ExploreSegment (igvp->pictRuler2, (Pointer) &sel, Ing_DeSelectAll);
     }
     else {
       sel.segID = msg.segID;
       sel.returnFALSE=FALSE;
       sel.vwr=igvp->vBottom;
       ExploreSegment (igvp->pictBottom, (Pointer) &sel, Ing_DeSelectAll);
     }
     if (igvp->bOverviewSelected){
       sel.vwr=igvp->vTop;
       ExploreSegment (igvp->pictTop, (Pointer) &sel, Ing_DeSelectAll);
       igvp->bOverviewSelected = FALSE;
     }
   
     if (Report!=NULL){
       Ing_HighlightReportWindow(Report, PLAIN_SEGMENT, 0, (igvp->isExons?TRUE:FALSE));
       Report->isPrevSelected=FALSE;
       Report->last_segID=0;
     }

     break;
    case OM_MSG_CACHED:
      break;
    case OM_MSG_UNCACHED:
      break;
    case OM_MSG_TO_CLIPBOARD: 
      break;
    case OM_MSG_SETCOLOR:
      break;
    case OM_MSG_FLUSH:
      Ing_MsgFlush(igvp,ommsp);
      break;
    default:
      break;
    }
  
  /* data has been altered --> refresh all viewers */
  if (refresh){
    Ing_InitAlignArrays(&igvp->talign, FALSE);
    SeqMgrIndexFeatures (igvp->entityID, NULL);
    Ing_PopulateOverviewPage((Pointer)igvp, FALSE);
    Ing_PopulateDetailedPage((Pointer)igvp);
    Ing_ResetTitle(igvp, TRUE);
  }

  return OM_MSG_RET_OK;
    }




/*******************************************************************************

  Function : Ing_MoveCursor(), Ing_DoMoveCursor()
  
  Purpose : draw grey cursor on Ruler1

*******************************************************************************/
static void Ing_DoMoveCursor(VieweR v, SegmenT pict, Int2 SegBoxTop, Int2 SegBoxBottom, Int4 from, Int4 to,Int4 MaxLength, Boolean bFirst, Uint1Ptr grayClr)
{
  Int2 Half;
  WindoW  temport; 
  PrimitivE prim=NULL;
  enumPrimAddOrder oldOrder;
  Int4 scaleX,scaleY;
  Int4 primwidth; 
  PntInfo worldPt;
  PoinT viewerPt;
  RecT rc,rc2, rcV;
  BoxInfo boxCursor;
  BoxInfo boxSeg;
 

 temport=SavePort(ParentWindow(v)); 


	/*the cursor always has the index 1*/
	prim=GetPrimitive(pict, 1);
	if (prim && !bFirst){/*the first time this function is called (when the viewer is loaded, the cursor prim doesn't exist yet*/
		ViewerBox(v,NULL,NULL,&rc,&scaleX,&scaleY);
		PrimitiveBox(prim,scaleX,scaleY,&boxCursor);
		if (prim){
        DeletePrim(pict, prim);
			worldPt.x=boxCursor.left;
			worldPt.y=boxCursor.top;
			Nlm_MapWorldToViewer(v,worldPt,&viewerPt);
			rc2.left=viewerPt.x;
			rc2.top=viewerPt.y;
			worldPt.x=boxCursor.right;
			worldPt.y=boxCursor.bottom;
			Nlm_MapWorldToViewer(v,worldPt,&viewerPt);
			rc2.right=viewerPt.x;
			rc2.bottom=viewerPt.y;
			Select(v);
			InvalRect(&rc2);
         RestorePort(temport);
			Update();
		}
	}
	/*the cursor must be displayed before everything else*/

	oldOrder=ChangeAddPrimOrder(ADD_TO_HEAD);
   GetPosition(v, &rcV);

   /*the cursor always has an index=1*/
     AddAttribute(pict, COLOR_ATT|STYLE_ATT, (grayClr?grayClr:YELLOW_COLOR), SOLID_LINE, 0, 0, 0);
     AddRectangle(pict,MAX(1,from),SegBoxTop,
                  to,SegBoxBottom, NO_ARROW,TRUE,1);

	ChangeAddPrimOrder(oldOrder);
   ViewerBox(v,NULL,NULL,&rc,NULL,NULL);
	worldPt.x=from;
	worldPt.y=0;
	Nlm_MapWorldToViewer(v,worldPt,&viewerPt);
	rc.left=viewerPt.x;
	worldPt.x=to;
	worldPt.y=0;
	Nlm_MapWorldToViewer(v,worldPt,&viewerPt);
	rc.right=viewerPt.x+1;
   temport=SavePort(ParentWindow(v));
	Select(v);
	InvalRect(&rc);
   RestorePort(temport);
	Update();
}

static void Ing_MoveCursor(IngGenomeViewerPtr igvp, Int4 from, Int4 to, Boolean bFirst) 
{
  Int2  SegBoxTop;
  Int2  SegBoxBottom;
  Uint1 grayClr1[3];
  Uint1 grayClr2[3];
  RecT  rc;

	grayClr1[0]=230; 
	grayClr1[1]=230;
 	grayClr1[2]=230; 
	grayClr2[0]=248; 
	grayClr2[1]=248;
 	grayClr2[2]=248; 

   GetPosition(igvp->vRuler1, &rc);
   SegBoxTop=9*igvp->GrData.SegBoxHeight;
   SegBoxBottom = -200;
   Ing_DoMoveCursor(igvp->vRuler1,igvp->pictRuler1, SegBoxTop, SegBoxBottom,from,to,igvp->SeqLength, bFirst, grayClr1); 

   /*
   if (igvp->last_feat_position_top != 0){
     SegBoxTop = 40;
     SegBoxBottom = igvp->last_feat_position_top - 20;
     Ing_DoMoveCursor(igvp->vTop, igvp->pictTop, SegBoxTop, SegBoxBottom, from, to, igvp->SeqLength, bFirst, grayClr2);
   }
   */
}


static Boolean Ing_BuildStartupPage(IngGenomeViewerPtr igvp)
{
SegmenT        segRuler,segLines, pictControl;
BioseqPtr      bsp;
Int4           nSegments;
IngExploreSegs gpn;

	if (igvp==NULL) return(FALSE);
   Ing_PopulateOverviewPage((Pointer)igvp, TRUE);
   Ing_PopulateDetailedPage((Pointer)igvp);
	return(TRUE);
}

/*****************************************************************************

Function: Ing_UpdateZoomerVals(), Ing_PopulateZoomer(), Ing_ZoomProc(), Ing_ZoomerClickProc()

Purpose: zoom functions

*****************************************************************************/
static void Ing_UpdateZoomerVals(IngGenomeViewerPtr igvp, Int4 scaleX)
{
  Int4 index, z, sFactor;

  z=15;
  sFactor=MAX(1, scaleX/z);

  for (index=z; index>1; index--) 
    {
      igvp->scale_array[index-1] = MAX(1, scaleX);
      scaleX -=sFactor;
    }
  igvp->scale_array[index-1] = MAX(1, igvp->scale_array[index]/2);  

  igvp->scaleX=igvp->scale_array[igvp->scale_index];

}


static Boolean Ing_PopulateZoomer(IngGenomeViewerPtr igvp)
{
  Int4      index, x1, x2;
  Int4      y1, y2;
  Int4      width;
  Int4      z, sFactor;
  PrimitivE prim;
  Uint1     zoomClr[3];
  Int4      scaleX;

  zoomClr[0]=0; 
  zoomClr[1]=0;
  zoomClr[2]=0; 
  scaleX=Ing_GetBPMaxScale(igvp);
  z = 15;
  igvp->scale_array=(Int4Ptr)MemNew(sizeof(Int4)*z);

  sFactor = MAX(1, scaleX/z);
  width = z;
  x1 = 4;
  x2 = 2*(x1+width);
  y1 = 0;
  y2 = -1;

  AddAttribute(igvp->pictZoom, COLOR_ATT, BLUE_COLOR, 0, 0, 0, 0);
  AddLabel (igvp->pictZoom,  (x1+x2)/2 , y1, "OUT", SMALL_TEXT, 	0, UPPER_CENTER, 0);
  AddAttribute(igvp->pictZoom, COLOR_ATT, zoomClr, 0, 0, 0, 0);
  for (index=z; index>2; index--) 
    {
      prim=AddRectangle(igvp->pictZoom, x1, y1, x2, y2, 0, TRUE,(Uint2) index);
      x1+=1;
      x2-=1;
      y1-=4;
      y2-=4;
      igvp->scale_array[index-1] = MAX(1, scaleX);
      if (index==8){
        HighlightPrimitive (igvp->vZoom, igvp->pictZoom, prim, FRAME_PRIMITIVE);
        igvp->scaleX=MAX(1, scaleX);
        igvp->scale_index=index-1;
      }
      scaleX -=sFactor; 
      if (index==z)
        igvp->zoomFbar=prim;    
    }
  AddRectangle(igvp->pictZoom, x1, y1, x2, y2, 0, TRUE,(Uint2)index);
  igvp->scale_array[index] = MAX(1, scaleX);
  AddAttribute(igvp->pictZoom, COLOR_ATT, RED_COLOR, 0, 0, 0, 0);
  AddRectangle(igvp->pictZoom, x1+1, y1-4, x2-1, y2-4, 0, TRUE,(Uint2) index-1);
  igvp->scale_array[index-1] = MAX(1, igvp->scale_array[index]/2);  
/*   AddAttribute(igvp->pictZoom, COLOR_ATT, MAGENTA_COLOR, 0, 0, 0, 0); */
/*   AddLabel (igvp->pictZoom,  (x1+x2)/2 , y2-2*igvp->GrData.cyChar , "-", MEDIUM_TEXT,  0, UPPER_CENTER, 0); */
  AttachPicture (igvp->vZoom,igvp->pictZoom, INT4_MIN, INT4_MAX, UPPER_RIGHT, 1 , 1, NULL); 
  SetViewerProcs(igvp->vZoom,Ing_ZoomerClickProc,NULL,NULL,  NULL);
  return TRUE;
}


static void Ing_ZoomProc(IngGenomeViewerPtr  igvp)
{
  Int4        from, to;


  Ing_PopulateDetailedPage((Pointer)igvp);
  Ing_GetNamesZoomedVal(igvp->vRuler2,&from,&to,NULL,igvp->SeqLength);
  igvp->xposition=from;
  Ing_MoveCursor(igvp, from, to, FALSE);
}


static void Ing_ZoomerClickProc(VieweR v, SegmenT s, PoinT pt)
{
  PrimitivE  prim;
  Uint2      primCT, primID=0, segID;
  Int4       scaleX;
  IngGenomeViewerPtr igvp;


  igvp=(IngGenomeViewerPtr)GetObjectExtra((WindoW)ParentWindow(v));
  if (!igvp) return;

 if (FindSegPrim(v, pt, NULL, NULL, &prim))
   {
     FindSegment(v, pt, &segID, &primID, &primCT);
     if (primID==0)
       return;
     ExploreSegment (s,  (Pointer)v, Ing_DeSelectGenericPrim);
     HighlightPrimitive (v, s, prim, FRAME_PRIMITIVE);
     scaleX = MAX(1, igvp->scale_array[((int)primID)-1]);
     if (primID==15)
       igvp->bMaxScale=TRUE;
     else
       igvp->bMaxScale=FALSE;
     igvp->scale_index=primID-1;
     igvp->scaleX=scaleX;
     Ing_ZoomProc(igvp);
   }
}

/*****************************************************************************

Function: Ing_FeatViewerClickProc(), Ing_FeatViewerPanProc(), Ing_SequenceClickProc(), Ing_Ruler1ClickProc(), Ing_Ruler1DragProc(), Ing_Ruler1ReleaseProc()

Purpose: click and scroll main window functions

*****************************************************************************/
static void Ing_FeatViewerClickProc(VieweR v, SegmenT s, PoinT pt)
{
  PrimitivE  prim=NULL;
  Uint2      entityID=0,itemtype=0;
  Uint4      itemID=0;
  Uint2      primID=0;
  Uint2      segID=0;
  Int1       highlight=0; 
  IngGenomeViewerPtr igvp=NULL;
  SeqLocPtr  slp=NULL; 
  Int4       start, stop;
  Int4       i;
  Char       str1[100], str2[50]; 
  Uint2      strand;
  Int2       handled;
  SegmenT    subSeg=NULL;
  IngSelectData    sel;
  SelStructPtr       ssp=NULL;


  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(v));
  if (!igvp) return;
  
  /* if click was on a primitive */
  if (subSeg=FindSegPrim(v, pt, NULL, NULL, &prim)){ 
    GetPrimitiveIDs(prim, &entityID, &itemID, &itemtype, &primID);
    
    if  (entityID && itemID && itemtype){
      if (!primID){
        highlight=SegmentStyle(subSeg);
        segID=SegmentID(subSeg);
        msg.segID=segID;
      }
      else {
        GetPrimDrawAttribute (prim, NULL, NULL, NULL, NULL, NULL, &highlight);
        msg.segID=primID;
      }
      msg.labelmsg=2;
      if (highlight == PLAIN_PRIMITIVE){
        if (shftKey){
          ObjMgrAlsoSelect(entityID, itemID, itemtype, 0, NULL);
        }
        else
          ObjMgrSelect (entityID, itemID, itemtype, 0, NULL);
      }
      else if (!dblClick){
        ObjMgrDeSelect(entityID, itemID, itemtype, 0, NULL);
      }

   if (dblClick)
     {
       WatchCursor ();
       handled=GatherProcLaunch (OMPROC_EDIT, FALSE, entityID, itemID, itemtype, 0, 0, itemtype, 0);
       ArrowCursor();
       if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
       ErrPostEx(SEV_ERROR, 0, 0,"Unable to launch editor on feature." );
       }
     }
  }
  else  {
    Ing_ResetTitle(igvp, TRUE);
    ssp=ObjMgrGetSelected();
    
    if (ssp) 
      ObjMgrDeSelectAll();
   }
   
 }
 else 
   {
     Ing_ResetTitle(igvp, TRUE);
     ssp=ObjMgrGetSelected();
     if (ssp) 
       ObjMgrDeSelectAll();
   }
}


static void Ing_FeatViewerPanProc(VieweR v, SegmenT s)
{
IngGenomeViewerPtr igvp;
WindoW hParent;
Int4 from,to, scaleX;
PntInfo worldPt;
PoinT viewerPt;


	hParent=ParentWindow(v);
	if (!hParent) return;
	
	igvp=(IngGenomeViewerPtr)GetObjectExtra(hParent);
	if (!igvp) return;

	/*move the cursor*/
	Ing_GetNamesZoomedVal(igvp->vBottom,&from,&to,&scaleX,igvp->SeqLength);
   if (igvp->xposition != from){
     worldPt.x=from;
     worldPt.y=0;
     Nlm_MapWorldToViewer(igvp->vBottom,worldPt,&viewerPt);
     igvp->xposition=from;

     AttachPicture(igvp->vRuler2, igvp->pictRuler2, from, INT4_MAX, UPPER_LEFT, MAX(1,scaleX) , 1, NULL);
     SetViewerProcs(igvp->vRuler2, Ing_SequenceClickProc, NULL, NULL, NULL);
	Ing_GetNamesZoomedVal(igvp->vRuler2,&from,&to,&scaleX,igvp->SeqLength);
   Ing_MoveCursor(igvp, from, to, FALSE);
   igvp->xposition=from;
   }
}


static void Ing_SequenceClickProc(VieweR v, SegmenT s, PoinT pt)
{
  IngGenomeViewerPtr igvp=NULL;
  Uint2              entityID=0;
  Uint4              itemID=0;
  Uint2              itemtype=0;
  Uint2              primID=0;
  PrimitivE          prim;
  SelStructPtr       ssp=NULL;
  
  igvp=(IngGenomeViewerPtr)GetObjectExtra((WindoW)ParentWindow(v));
  ssp=ObjMgrGetSelected();

  if (FindSegPrim(v, pt, NULL, NULL, &prim))
   {
     GetPrimitiveIDs(prim, &entityID, &itemID, &itemtype, &primID);
     if (entityID && itemID && itemtype){
       msg.labelmsg=2;
       ObjMgrSelect (entityID, itemID, itemtype, 0, NULL);
       return;
     }
   }
  Ing_ResetTitle(igvp, TRUE);
  
   
}

PoinT  startPt;
PoinT  stopPt;

static void Ing_Ruler1ClickProc(VieweR v, SegmenT s, PoinT pt)
{
 RecT                rc;
 Int4                from, to;
 IngGenomeViewerPtr  igvp;

 igvp=(IngGenomeViewerPtr)GetObjectExtra((WindoW)ParentWindow(v));
 Ing_GetNamesZoomedVal(igvp->vRuler1,&from,&to, NULL,igvp->SeqLength);

 if (pt.x<from)
   pt.x=from;
 if (pt.x>to)
   pt.x=to;
 
  startPt=pt;
  stopPt=pt;
}

static void Ing_Ruler1DragProc(VieweR v, SegmenT s, PoinT pt)
{
  IngGenomeViewerPtr igvp;
  Int4    offset, LscaleX,scaleX, from, to, diff;
  PntInfo worldPt;
  PoinT   viewerPt;
  RecT    cursor, rc, rcDetail;
  WindoW  temport;
  
  igvp=(IngGenomeViewerPtr)GetObjectExtra((WindoW)ParentWindow(v));
  if (igvp->bMaxScale) 
    return; 
  
 Ing_GetNamesZoomedVal(igvp->vRuler1,&from,&to, &LscaleX,igvp->SeqLength);

 if (pt.x<from)
   pt.x=from;
 if (pt.x>to)
   pt.x=to;

  stopPt=pt;

  Ing_GetNamesZoomedVal(igvp->vRuler2,&from,&to, &scaleX,igvp->SeqLength);

  worldPt.x=(from+to)/2;
  worldPt.y=0;
  Nlm_MapWorldToViewer(v,worldPt,&viewerPt);


  offset = (stopPt.x -viewerPt.x)*LscaleX;
  diff=to-from;
  from+=offset;
  to=from+diff;

  igvp->xposition=from;
  Ing_MoveCursor(igvp,from,to, FALSE);
  AttachPicture(igvp->vRuler2, igvp->pictRuler2, from, INT4_MAX, UPPER_LEFT, MAX(1,scaleX) , 1, NULL);
  SetViewerProcs(igvp->vRuler2,Ing_SequenceClickProc,NULL,NULL, NULL);
  AttachPicture(igvp->vBottom, igvp->pictBottom, from, INT4_MAX, UPPER_LEFT, MAX(1,scaleX) , 1, NULL);
  SetViewerProcs(igvp->vBottom,Ing_FeatViewerClickProc,NULL,NULL, Ing_FeatViewerPanProc);
}


static void Ing_Ruler1ReleaseProc(VieweR v, SegmenT s, PoinT pt)
{
  IngGenomeViewerPtr igvp;
  Int4    offset, LscaleX,scaleX, from, to, diff;
  PntInfo worldPt;
  PoinT   viewerPt;
  RecT    cursor, rc, rcDetail;
  BaR     hsb;
  WindoW  temport;

  igvp=(IngGenomeViewerPtr)GetObjectExtra((WindoW)ParentWindow(v));
  if (igvp->bMaxScale) 
    return; 

  GetPosition(v, &rc);
  if (pt.x<rc.left)
    pt.x=rc.left;
  if (pt.x>rc.right)
    pt.x=rc.right;
  stopPt=pt;
  Ing_GetNamesZoomedVal(igvp->vRuler2,&from,&to, &scaleX,igvp->SeqLength);
  igvp->xposition=from;
  Ing_GetNamesZoomedVal(igvp->vRuler1,NULL, NULL, &LscaleX,igvp->SeqLength);

  worldPt.x=(from+to)/2;
  worldPt.y=0;
  Nlm_MapWorldToViewer(v,worldPt,&viewerPt);


  offset = (stopPt.x -viewerPt.x)*LscaleX;
  diff=to-from;
  from+=offset;
  to=from+diff;

  igvp->xposition=from;
  
  Ing_MoveCursor(igvp,from,to, FALSE);
  AttachPicture(igvp->vRuler2, igvp->pictRuler2, from, INT4_MAX, UPPER_LEFT,  MAX(1, scaleX), 1, NULL); 
  SetViewerProcs(igvp->vRuler2,Ing_SequenceClickProc,NULL,NULL, NULL);
  AttachPicture(igvp->vBottom, igvp->pictBottom, from, INT4_MAX, UPPER_LEFT, MAX(1,scaleX) , 1, NULL);
  SetViewerProcs(igvp->vBottom,Ing_FeatViewerClickProc,NULL,NULL, Ing_FeatViewerPanProc);
  
}


/*****************************************************************************

Function: Ing_ReadIn2ndBsp(), Ing_ReadSpideyInfo(), Ing_ReadDotMatrixInfo(), Ing_ReadInBlastAsn(), Ing_FGetLine(), Ing_ParseSeqIdFile(), Ing_ReadInFeatureTable()

Purpose:  read in sequence, seqannot or seqid-list files 

*****************************************************************************/

static void Ing_ReadIn2ndBsp (IngGenomeViewerPtr igvp, CharPtr path, CharPtr GI_list)
{
 
  BioseqPtr       bsp_1=NULL;

  bsp_1=(BioseqPtr)GetBioseqGivenIDs(igvp->entityID, igvp->itemID, OBJ_BIOSEQ);
  if (!bsp_1) return;

  Ing_CreateBlast2SeqsForm(igvp, bsp_1, path, GI_list);
}

static void Ing_ReadSpideyInfo(IngGenomeViewerPtr igvp, CharPtr path, CharPtr GI_list)
{
  BioseqPtr    bsp_1=NULL;
  bsp_1=(BioseqPtr)GetBioseqGivenIDs(igvp->entityID, igvp->itemID, OBJ_BIOSEQ);
  if (!bsp_1) return;

  Ing_CreateSpideyForm(igvp, bsp_1, path, GI_list);
}

static void Ing_ReadDotMatrixInfo(IngGenomeViewerPtr igvp, CharPtr path, CharPtr GI_list)
{
  BioseqPtr    bsp_1=NULL;
  bsp_1=(BioseqPtr)GetBioseqGivenIDs(igvp->entityID, igvp->itemID, OBJ_BIOSEQ);
  if (!bsp_1) return;

  Ing_CreateDotMatrixForm(igvp, NULL, path, GI_list);
}

static void Ing_ReadInBlastAsn (IngGenomeViewerPtr igvp, FILE * ifp)
{
  Pointer         dataptr;
  Uint2           datatype, entityID, itemID;
  BioseqPtr       bsp;
  SeqAlignPtr     n_sap=NULL;
  SeqAlignPtr     salp=NULL;
  SeqIdPtr        sip=NULL;
  SeqAnnotPtr     sanp=NULL; 
  Int4            start, i;
  Int4            stop;
  Char            str[10];
  IngReportPtr    Report=NULL;
  
  if (ifp == NULL)
    return;
 

   dataptr = ReadAsnFastaOrFlatFile (ifp, &datatype, NULL, FALSE, TRUE, TRUE, TRUE);
   if (!dataptr) goto end;
   if (datatype==OBJ_SEQANNOT){
     sanp = (SeqAnnotPtr)(dataptr);
     sanp->name=StringSave("Blast file");
     bsp=(BioseqPtr)GetBioseqGivenIDs(igvp->entityID, igvp->itemID, OBJ_BIOSEQ);
     Ing_AttachSeqAnnotToSeqEntry(igvp->entityID, sanp, bsp);
     ObjMgrSetDirtyFlag(igvp->entityID, TRUE);
     ObjMgrSendMsg(OM_MSG_UPDATE, igvp->entityID, 0, 0);
   }
   else{
     Message (MSG_OK, "File is not a seqannot");
   }
 end:
   ArrowCursor();
   return;
}

static CharPtr Ing_FGetLine (FILE *fp)
{
  ValNodePtr charp=NULL,
             vnp;
  CharPtr    buffer = NULL;
  Char       c;
  Int4       j, len=0;

  c = (Char)fgetc(fp);
  while (c!=255 && c!=NULLB && c!=EOF && c!='\0' && c!='\n')
  {
     if (c!='\t' && c!='\015') {
        ValNodeAddInt (&charp, 1, (Int4)c);
        len++;
     }
     c = (Char)fgetc(fp);
     if (c==EOF) {
       return NULL;
     }
  }
  if (charp) {
     buffer = (CharPtr)MemNew((size_t)((len+5)*sizeof(Char)));
     for (j=0, vnp=charp; vnp!=NULL; vnp=vnp->next, j++)
        buffer[j] = (Char) vnp->data.intvalue;
     buffer[j]='\0';
     ValNodeFree (charp);
  }
  else if (c=='\0' || c=='\n') {
     buffer = (CharPtr)MemNew((size_t)(sizeof(Char)));
     buffer[0]='\0';
  }
  return buffer;
}



extern CharPtr Ing_ParseSeqIdFile(FILE * fp)
{
   CharPtr         line=NULL;
   CharPtr         GI_list=NULL;
   CharPtr         ptr=NULL;
   BioseqPtr       bsp=NULL;
   Boolean         found = FALSE;

   line = Ing_FGetLine(fp);
   if (line==NULL) return NULL;
   while (line != NULL && !found) /* find first non-empty line */
   {
      if (!StringHasNoText(line))
      { 
         if (StringLen(line) > 0)
            found = TRUE;
      } else
      {
         MemFree(line);
         line = Ing_FGetLine(fp);
         if (line==NULL) return NULL;
      }
   }

   if (!StringHasNoText(line))
     {
         line = TrimSpacesAroundString(line);
         ptr = strtok(line, "\t");
         GI_list=StringSave(ptr);
         GI_list=Ing_StringCat(GI_list, " ");
         MemFree(line);
         line = Ing_FGetLine(fp);
     }
   
   while(line != NULL){
     if (!StringHasNoText(line))
       {
         line = TrimSpacesAroundString(line);
         ptr = strtok(line, "\t");
         GI_list=Ing_StringCat(GI_list, ptr);
         GI_list=Ing_StringCat(GI_list, " ");
         MemFree(line);
         line = Ing_FGetLine(fp);
       } else
         {
           MemFree(line);
           line = Ing_FGetLine(fp);
         }
   }
   return GI_list;
}

static void Ing_ReadInFeatureTable (IngGenomeViewerPtr igvp, FILE * ifp)
{
   Pointer         dataptr;
   Uint2           datatype, entityID;
   BioseqPtr       bsp;
   Uint2           itemID;
   SeqFeatPtr      sfp;
   SeqAnnotPtr     sanp;
   Int4            start, i;
   Int4            stop;
   IngReportPtr    Report=NULL;

   if (ifp == NULL)
      return;
   WatchCursor();
   dataptr = ReadAsnFastaOrFlatFile (ifp, &datatype, &entityID, FALSE, FALSE, TRUE, FALSE);
   if (!dataptr) return;
   if (datatype==OBJ_SEQANNOT){
     sanp = (SeqAnnotPtr)(dataptr);
     bsp=(BioseqPtr)GetBioseqGivenIDs(igvp->entityID, igvp->itemID, OBJ_BIOSEQ);
     entityID =Ing_AttachSeqAnnotToSeqEntry(igvp->entityID, (SeqAnnotPtr) dataptr, bsp);
     ObjMgrSetDirtyFlag(igvp->entityID, TRUE);
     ObjMgrSendMsg(OM_MSG_UPDATE, igvp->entityID, 0, 0);
   }
   else {
     Message (MSG_OK, "File is not a seqannot");
   }

   ArrowCursor ();
   return;

}

/*****************************************************************************

Function: Ing_StringCat()

Purpose:  concatinates checking that sufficient memory is allocated 

*****************************************************************************/
extern CharPtr Ing_StringCat(CharPtr a, CharPtr b)
{
  CharPtr temp=NULL;
  Int4    strlen=0;


  strlen=StringLen(a);
  strlen+=StringLen(b);
  temp=(CharPtr)MemNew(sizeof(Char)*(strlen+1));
  StringCat(temp, a);
  StringCat(temp, b);
  MemFree(a);

  return temp;
}


/*****************************************************************************

Function: Ing_AssembleGenscanInfo(), Ing_ReadInGenscan()

Purpose:  parse genscan output from file

*****************************************************************************/

static void Ing_AssembleGenscanInfo(IngGenomeViewerPtr igvp, IngInfoPtr lip)
{
   Int4         i = 0;
   IngInfoPtr   lip_tmp;
   Int4         n;
   Int4Ptr      ivals=NULL;
   Uint1        strand;
   CharPtr      tmp;
   SeqLocPtr    slp_list=NULL;
   SeqLocPtr    slp=NULL;

   if (lip == NULL)
      return;
   lip_tmp = lip;
   while (lip_tmp != NULL)
   {
      i++;
      lip_tmp = lip_tmp->next;
   }
   n = 0;
   ivals = (Int4Ptr)MemNew(2*i*sizeof(Int4));
   lip_tmp = lip;
   while (lip_tmp != NULL)
   {
      if ((tmp = StringStr(lip_tmp->fields[1], "PlyA")) != NULL)
      {
      } else if ((tmp = StringStr(lip_tmp->fields[1], "Prom")) != NULL)
      {
      } else 
      {
         if ((tmp = StringStr(lip_tmp->fields[2], "+"))!= NULL)
            strand = Seq_strand_plus;
         else
           strand = Seq_strand_minus;
         ivals[n] = atoi(lip_tmp->fields[3]);
         ivals[n+1] = atoi(lip_tmp->fields[4]);
         n+=2;
      }
      lip_tmp = lip_tmp->next;
   }

   slp_list=Ing_MakeSlpList(NULL, n/2, ivals, strand, igvp->bsp->id);
     Ing_CreateNewFeature(igvp->entityID, igvp->itemID, NULL, SEQFEAT_IMP, FEATDEF_misc_feature, "genscan predicted gene", slp_list, NULL);
   MemFree(ivals);
}


static void Ing_ReadInGenscan(FILE *ifp, IngGenomeViewerPtr igvp)
{
   Boolean         found=FALSE, first_pass=FALSE;
   Int4            i;
   Int4            len;
   CharPtr         letter=NULL, str=NULL;
   CharPtr         line=NULL;
   IngInfoPtr     lip=NULL;
   IngInfoPtr     lip_head=NULL;
   IngInfoPtr     lip_prev=NULL;
   CharPtr         ptr=NULL;
   Int4            prevlen;
   Int4            seq;
   CharPtr         tmp;

   WatchCursor();
   line = NULL;
   line = Ing_FGetLine(ifp);
   if (line==NULL) goto end;
   found = FALSE;
   while (line != NULL && !found) /* find first non-empty line */
   {
      if (!StringHasNoText(line))
      { 
         if (StringLen(line) > 0)
            found = TRUE;
      } else
      {
         MemFree(line);
         line = Ing_FGetLine(ifp);
         if (line==NULL) goto end;
      }
   }
   found = FALSE;
   while (!found/*  && line != NULL */)
     {
      tmp = StringStr(line, "----");
      if (tmp != NULL)
         found = TRUE;
      MemFree(line);
      line = Ing_FGetLine(ifp);
      if (line==NULL) goto end;

   }

   if (line==NULL) goto end;
   lip_prev = lip_head = NULL;
   while ((tmp = StringStr(line, "Predicted")) == NULL)
   {
      if (StringHasNoText(line))
      {
         if (lip_head != NULL){
           Ing_AssembleGenscanInfo(igvp, lip_head);
         }
         lip_prev = lip_head = NULL;
      } else
      {
         line = TrimSpacesAroundString(line);
         ptr = strtok(line, " ");
         lip = (IngInfoPtr)MemNew(sizeof(IngInfo));
         for (i=0; i<5; i++)
         {
            lip->fields[i] = StringSave(ptr);
            ptr = strtok(NULL, " ");
         }
         if (lip_head != NULL)
         {
            lip_prev->next = lip;
            lip_prev = lip;
         } else
            lip_head = lip_prev = lip;
         MemFree(line);
      }
      line = Ing_FGetLine(ifp);
      if (line==NULL) goto end;

   }
   

 end:   
   ObjMgrSetDirtyFlag(igvp->entityID, TRUE);
   ObjMgrSendMsg(OM_MSG_UPDATE, igvp->entityID, 0, 0);
   ArrowCursor();
   return;
}


/*****************************************************************************

Function: Ing_ReadALine(), Ing_GetSeqFeatType(), Ing_GetFeatDefType(), Ing_GetStrand()

Purpose:  parse genscan output from file

*****************************************************************************/

static CharPtr Ing_ReadALine (CharPtr str, size_t size, FILE *fp)

{
  Char     ch;
  CharPtr  ptr;
  CharPtr  rsult;

  if (str == NULL || size < 1 || fp == NULL) return NULL;
  *str = '\0';
  rsult = fgets (str, size, fp);
  if (rsult != NULL) {
    ptr = str;
    ch = *ptr;
    while (ch != '\0' && ch != '\n' && ch != '\r') {
      ptr++;
      ch = *ptr;
    }
    *ptr = '\0';
  }
  return rsult;
}

static Uint1 Ing_GetFeatDefType(CharPtr str)
{

  if (StringCmp(str, "Gene") == 0 || StringCmp(str, "gene")==0)
    return FEATDEF_GENE;
  else if (StringCmp(str, "CDS")==0)
    return FEATDEF_CDS;
  else if (StringCmp(str, "mRNA") == 0)
    return FEATDEF_mRNA;
  else if (StringCmp(str, "STS") == 0)
    return FEATDEF_STS;
  else if (StringCmp(str, "variation")==0)
    return FEATDEF_variation;
  else
    return FEATDEF_misc_feature;

}

static Uint1 Ing_GetSeqFeatType(CharPtr str)
{

  if (StringCmp(str, "Gene") == 0 || StringCmp(str, "gene")==0)
    return SEQFEAT_GENE;
  else if (StringCmp(str, "CDS")==0)
    return SEQFEAT_CDREGION;
  else if (StringCmp(str, "mRNA") == 0)
    return SEQFEAT_RNA;
  else if (StringCmp(str, "STS") == 0)
    return SEQFEAT_REGION;
  else if (StringCmp(str, "variation")==0)
    return SEQFEAT_REGION;
  else
    return SEQFEAT_IMP;

}

static Uint1 Ing_GetStrand(CharPtr str)
{
  if (str == NULL) return Seq_strand_unknown;

  if (StringCmp(str, "plus") == 0)
    return Seq_strand_plus;
  else if (StringCmp(str, "minus") == 0)
    return Seq_strand_minus;
  else if (StringCmp(str, "both")==0)
    return Seq_strand_both;
  else
    return Seq_strand_unknown;
}

/*****************************************************************************

Function: Ing_FreeLip(), Ing_AssembleTabDelimited(), Ing_ReadInTabDelimited(), is_in_range()

Purpose:  functions to parse tab delimited file

Tab Delimited format:
1) Top of the file must contain '>Tab Delimited'.

2) Columns..
1=start, 2=stop, 3=feature name, 4=uniqueID, 5=unit number(if it is part of a set), 6=color(choices are [red, black, cyan, green, magenta, purple, gray, yellow and blue]), 7=score, 8=strand.


*****************************************************************************/

static IngInfoPtr Ing_FreeLip(IngInfoPtr lip, Boolean delete_last)
{
  IngInfoPtr lip_tmp;
  Int4        i;

  lip_tmp=lip;
/*   lip=lip->next; */
  if (!delete_last){
    while (lip->next !=NULL){ /* don't delete the last node */
      for (i=0; i<4; i++)
        {
          if (lip_tmp->fields[i]) MemFree(lip_tmp->fields[i]);
        }
      
      MemFree(lip_tmp);
      lip_tmp=lip;
      lip=lip->next;
    }
  }
  else{

    while (lip !=NULL){ /* don't delete the last node */
      for (i=0; i<4; i++)
        {
          if (lip_tmp->fields[i]) MemFree(lip_tmp->fields[i]);
        }
      
      MemFree(lip_tmp);
      lip_tmp=lip;
      lip=lip->next;
    }
  }
  return NULL;

}

static void Ing_AssembleTabDelimited(IngGenomeViewerPtr igvp, IngInfoPtr lip, CharPtr id_ptr, Uint1 seqfeattype, Uint1 featdeftype, Int4 left, Int4 right)
{
   Int4         numivals =0;
   IngInfoPtr   lip_tmp;
   Int4         n, right_col, left_col;
   CharPtr      tmp=NULL;
   Uint1        strand;
   CharPtr      color = NULL;
   Int4         score;
   CharPtr      UID=NULL;
   Int4Ptr      ivals=NULL;
   CharPtr      label=NULL;
   SeqLocPtr    slp_list=NULL;

   if (igvp==NULL || lip == NULL)
      return;

    lip_tmp = lip;

   while (lip_tmp != NULL)
   {
      numivals++;
      lip_tmp = lip_tmp->next;
   }

   lip_tmp = lip;
   strand=Ing_GetStrand(lip_tmp->fields[7]);
   if (lip_tmp->fields[6]) 
     score=atoi(lip_tmp->fields[6]);
   else 
     score=1;
   if (lip_tmp->fields[5])
     color = lip_tmp->fields[5];
 
   if (lip->fields[3]){
     UID = (Char*) MemNew(sizeof(Char)*20);
     sprintf(UID, "%s",lip->fields[3]);
   }
   else
     UID=NULL;

   n = 0;
   if (numivals>1){
     ivals = (Int4Ptr)MemNew(2*numivals*sizeof(Int4));
     while (lip_tmp != NULL)
       {
         left_col=atoi(lip_tmp->fields[0]);
         right_col=atoi(lip_tmp->fields[1]);
          
         if ((!(left_col<left && right_col<left))
             || (!(left_col>right && right_col>right))){
/*            if (strand==Seq_strand_minus){ */
/*              ivals[n] = right_col; */
/*              ivals[n+1] = left_col; */
/*            } */
/*            else{ */
           ivals[n] = left_col;
           ivals[n+1] = right_col;
        /*    } */
         }
         n+=2;
         lip_tmp = lip_tmp->next;
       }

     slp_list=Ing_MakeSlpList(NULL, n/2, ivals, strand, igvp->bsp->id);
   }
   else /* just one piece */
     {
       left =MAX(left, atoi(lip->fields[0])) ;
       right = MIN(right, atoi(lip->fields[1]));
       slp_list = SeqLocIntNew(left, right, strand, igvp->bsp->id);
     }

   if (lip->fields[2]){ 
     label = (Char*) MemNew(sizeof(Char)*100); 
     sprintf(label, "(user) %s, %s",lip->fields[2], UID); 
    } 
   else { 
     label= (Char*) MemNew(sizeof(Char)*100);
     sprintf(label, "(user) Undefined"); 
   }
  
   Ing_CreateNewFeature(igvp->entityID, igvp->itemID, NULL, seqfeattype, featdeftype, label, slp_list, color);

   MemFree(ivals);
   MemFree(label);
   MemFree(UID);
}

static Boolean is_in_range(IngInfoPtr lip, Int4 left, Int4 right)
{
  IngInfoPtr     lip_tmp;
  Uint1           strand;
  Int4            right_end, left_end;


  if (left==0 && right==0) return TRUE;

  lip_tmp=lip;

  strand=Ing_GetStrand(lip_tmp->fields[7]);
  if (strand==Seq_strand_minus){
    right_end=(atoi(lip_tmp->fields[0]));
  }
  else{
    left_end=(atoi(lip_tmp->fields[0]));
  }
  
  while (lip_tmp->next != NULL)
    {
      lip_tmp = lip_tmp->next;
    }
  
  if (strand==Seq_strand_minus){
    left_end=(atoi(lip_tmp->fields[1]));
  }
  else{
    right_end=(atoi(lip_tmp->fields[1]));
  }

  if ((!(left_end<left && right_end<left))
      || (!(left_end>right && right_end>right))){
    return TRUE;
  }
  else
    return FALSE;


}
    

static void Ing_ReadInTabDelimited(IngGenomeViewerPtr igvp, BioseqPtr bsp, FILE *ifp)
{
  
   Boolean         found=FALSE;
   Boolean         SAVE=FALSE;
   Boolean         first_pass=TRUE;
   Boolean         was_saved=FALSE;
   Int4            i;
   Int4            len;
   CharPtr         letter=NULL;
   Char            line[1000];
   IngInfoPtr      lip=NULL;
   IngInfoPtr      lip_tmp=NULL;
   IngInfoPtr      lip_new=NULL;
   IngInfoPtr      lip_head=NULL;
   IngInfoPtr      lip_prev=NULL;
   CharPtr         ptr=NULL, id_ptr=NULL;
   Int4            prevlen, left=0, right=0;
   Int4            seq;
   Uint1           type;
   CharPtr         tmp=NULL, str=NULL, label_str=NULL;
  
   WatchCursor();
   if (bsp==NULL) goto end;

   MemSet((Pointer)line, '\0',sizeof(line));
   str=Ing_ReadALine(line, sizeof(line),ifp);
   found = FALSE;

   if (igvp->slp){
     left=GetOffsetInBioseq(igvp->slp, bsp, SEQLOC_LEFT_END);
     right=GetOffsetInBioseq(igvp->slp, bsp, SEQLOC_RIGHT_END);
   }
  else{
    left=0;
    right=igvp->SeqLength;
  }

   while (str!= NULL && !found) /* find first non-empty line */
   {
      if (!StringHasNoText(line))
      {
        if (StringNCmp(line, ">Tab Delimited", 14) == 0)
          {
            id_ptr = strtok(line+9, " ");
            found = TRUE;
            str = Ing_ReadALine(line, sizeof(line),ifp);
          }
        else {
          ErrPostEx(SEV_ERROR, 0, 0, "feature file must begin with \'>Tab Delimited\'");
          goto end;
        }
          
      } else
      {
         MemFree(line);
         str = Ing_ReadALine(line, sizeof(line), ifp);
      }
   }
   if (!found)
      goto end;

   lip_prev = lip_head = NULL;
   i=1;
   while (str != NULL)
   {
      if (SAVE)
      {
        if (is_in_range(lip, left, right)){
          if (lip_new != NULL){
            Ing_AssembleTabDelimited(igvp, lip_new, id_ptr, Ing_GetSeqFeatType(lip_new->fields[2]), Ing_GetFeatDefType(lip_new->fields[2]), left, right);
            lip_new=Ing_FreeLip(lip_new, TRUE);
            lip_prev=lip_head=lip;
          }
        }

         SAVE=FALSE;
         was_saved=TRUE;
      } 
      else{
        ptr = strtok(line, "\t");
        lip = (IngInfoPtr)MemNew(sizeof(IngInfo));
        for (i=0; i<numcols_tbdl; i++)
          {
            str=StringSave(ptr);
            lip->fields[i] = str;
            ptr = strtok(NULL, "\t");
          }
        
        if (lip->fields[4] == NULL || (StringCmp(lip->fields[4], "1")==0)){
        if (!first_pass){
          SAVE=TRUE;
          label_str=str;
          if (lip_head)
            lip_new=lip_head; 
          else 
            lip_new=lip;
        }

      }

        
        if (first_pass) first_pass=FALSE;
            
        
        if (!SAVE && lip_head!=NULL)
          {
            lip_prev->next = lip;
            lip_prev = lip;
          }
        else
          {
            if (!lip_head) 
              lip_head = lip_prev = lip;
          }
        MemSet((Pointer)line, '\0', sizeof(line));
        str = Ing_ReadALine(line, sizeof(line), ifp);
        
      }
   }

   /* save the last 2 lines */
   if (SAVE)
     {
       if (lip_new->fields[0] != NULL){
         Ing_AssembleTabDelimited(igvp, lip_new, id_ptr, Ing_GetSeqFeatType(lip_new->fields[2]), Ing_GetFeatDefType(lip_new->fields[2]), left, right);
         lip_new=Ing_FreeLip(lip_new, TRUE);
       }
       was_saved=TRUE;
     }

   if (lip_head->fields[0] && (!was_saved)){
     Ing_AssembleTabDelimited(igvp, lip_head, id_ptr, Ing_GetSeqFeatType(lip_head->fields[2]), Ing_GetFeatDefType(lip_head->fields[2]),left, right);
     lip_head=Ing_FreeLip(lip_head, TRUE);
   }
   else if (lip->fields[0])
     {
       Ing_AssembleTabDelimited(igvp, lip, id_ptr, Ing_GetSeqFeatType(lip->fields[2]), Ing_GetFeatDefType(lip->fields[2]), left, right);
       lip=Ing_FreeLip(lip, TRUE);

     }
   ObjMgrSetDirtyFlag(igvp->entityID, TRUE);
   ObjMgrSendMsg(OM_MSG_UPDATE, igvp->entityID, 0, 0);
 end:
   ArrowCursor();
   return;
}


/*****************************************************************************

Function: SqnGetBioseqGivenSeqLoc(), GetBioseqReferencedbyAnnot(), Ing_AttachSeqAnnotToSeqEntry()

Purpose:  attach a new feature (packaged in a seqannot) to a bioseq

*****************************************************************************/
static BioseqPtr SqnGetBioseqGivenSeqLoc (SeqLocPtr slp, Uint2 entityID)

{
  BioseqPtr    bsp=NULL;
  SeqEntryPtr  sep=NULL;
  SeqIdPtr     sip=NULL;
  SeqLocPtr    tmp=NULL;

  if (slp == NULL) return NULL;
  bsp = NULL;
  sip = SeqLocId (slp);
  if (sip != NULL) {
    bsp = BioseqFind (sip);
  } else {
    tmp = SeqLocFindNext (slp, NULL);
    if (tmp != NULL) {
      sip = SeqLocId (tmp);
      if (sip != NULL) {
        bsp = BioseqFind (sip);
        if (bsp != NULL) {
          sep = SeqMgrGetSeqEntryForData (bsp);
          entityID = ObjMgrGetEntityIDForChoice (sep);
          bsp = GetBioseqGivenSeqLoc (slp, entityID);
        }
      }
    }
  }
  return bsp;
}


static BioseqPtr GetBioseqReferencedByAnnot (SeqAnnotPtr sap, Uint2 entityID)

{
  SeqAlignPtr   align;
  BioseqPtr     bsp;
  DenseDiagPtr  ddp;
  DenseSegPtr   dsp;
  SeqFeatPtr    feat;
  SeqGraphPtr   graph;
  SeqIdPtr      sip;
  SeqLocPtr     slp;
  StdSegPtr     ssp;
  SeqLocPtr     tloc;

  if (sap == NULL) return NULL;
  switch (sap->type) {
    case 1 :
      feat = (SeqFeatPtr) sap->data;
      while (feat != NULL) {
        slp = feat->location;
        if (slp != NULL) {
          bsp = SqnGetBioseqGivenSeqLoc (slp, entityID);
          if (bsp != NULL) return bsp;
        }
        feat = feat->next;
      }
      break;
    case 2 :
      align = (SeqAlignPtr) sap->data;
      while (align != NULL) {
        if (align->segtype == 1) {
          ddp = (DenseDiagPtr) align->segs;
          if (ddp != NULL) {
            for (sip = ddp->id; sip != NULL; sip = sip->next) {
              bsp = BioseqFind (sip);
              if (bsp != NULL) return bsp;
            }
          }
        } else if (align->segtype == 2) {
          dsp = (DenseSegPtr) align->segs;
          if (dsp != NULL) {
            for (sip = dsp->ids; sip != NULL; sip = sip->next) {
              bsp = BioseqFind (sip);
              if (bsp != NULL) return bsp;
            }
          }
        } else if (align->segtype == 3) {
          ssp = (StdSegPtr) align->segs;
          if (ssp != NULL && ssp->loc != NULL) {
            for (tloc = ssp->loc; tloc != NULL; tloc = tloc->next) {
              bsp = BioseqFind (SeqLocId (tloc));
              if (bsp != NULL) return bsp;
            }
          }
        }
        align = align->next;
      }
      break;
    case 3 :
      graph = (SeqGraphPtr) sap->data;
      while (graph != NULL) {
        slp = graph->loc;
        if (slp != NULL) {
          bsp = SqnGetBioseqGivenSeqLoc (slp, entityID);
          if (bsp != NULL) return bsp;
        }
        graph = graph->next;
      }
      break;
    default :
      break;
  }
  return NULL;
}

extern Uint2 Ing_AttachSeqAnnotToSeqEntry (Uint2 entityID, SeqAnnotPtr sap, BioseqPtr bsp)
{
  Int2           genCode;
  SeqEntryPtr    oldscope;
  OMProcControl  ompc;
  SeqEntryPtr    sep;
  SeqFeatPtr     sfp = NULL;

  if (sap == NULL) return entityID;
  if (bsp==NULL)
    bsp =GetBioseqReferencedByAnnot (sap, entityID);

  if (bsp == NULL) return entityID;
  
  if (sap->type == 1) {
    sfp = (SeqFeatPtr) sap->data;
    sep = GetBestTopParentForData (entityID, bsp);
    genCode = SeqEntryToGeneticCode (sep, NULL, NULL, 0);
    SetEmptyGeneticCodes (sap, genCode);
  } 
  MemSet ((Pointer) &ompc, 0, sizeof (OMProcControl));
  ompc.input_entityID = entityID;
  ompc.input_itemID = GetItemIDGivenPointer (entityID, OBJ_BIOSEQ, (Pointer) bsp);
  ompc.input_itemtype = OBJ_BIOSEQ;
  ompc.output_itemtype = OBJ_SEQANNOT;
  ompc.output_data = (Pointer) sap;
  if (! AttachDataForProc (&ompc, FALSE)) {
    Message (MSG_ERROR, "Ing_AttachSeqAnnotToSeqEntry failed");
  } else if (sfp != NULL) {
    PromoteXrefs (sfp, bsp, entityID);
  }

return entityID;
}

/*******************************************************************************

  Function : Ing_RunIngenue()
  
  Purpose :  starts up a new window

*******************************************************************************/
static void Ing_RunIngenue(IngGenomeViewerPtr igvp, BioseqPtr bsp, Uint2 entityID)
{
  ObjMgrPtr         omp=NULL;
  OMProcControl     ompc;
  ObjMgrProcPtr     ompp=NULL;
  Int2              ret;

  
  WinCounter++;
  omp=ObjMgrReadLock();
  ompp=ObjMgrProcFind(omp, 0, "Ing_StandAlone", 0);
  ompc.input_itemtype = OBJ_BIOSEQ;
  ompc.input_data=(Pointer)bsp;
  ompc.input_choice=(Pointer)igvp;
  ompc.input_entityID=entityID;
  ompc.proc = ompp;
  ObjMgrUnlock();
  if (ompp != NULL)
    {
      ret = (*(ompp->func))((Pointer)&ompc);
      switch(ret)
        {
        case OM_MSG_RET_ERROR:
          ErrShow();
          break;
        default:
          break;
        }
    } 
}


/*****************************************************************************

Function: Ing_FileOpenSpecial(), Ing_FileOpenMain()

Purpose:  additional data (Special) file, or as the main (Main)sequence data to be viewed 

*****************************************************************************/

static Boolean Ing_FileOpenSpecial(IngGenomeViewerPtr igvp, CharPtr path, CharPtr GI_list)
{
  IngReportPtr Report=NULL;
  FILE*     fp=NULL;
  BioseqPtr bsp=NULL;
  RecT      rcLines, rcControl, rcFeats;
  Boolean   DoUpdate=TRUE;
  Int4      scaleX;
  WindoW    temport, w;
  Char      strErr[50]={""};
  
  if (!igvp) return FALSE;

  
  if (path != NULL || GI_list !=NULL){

    if (!StringHasNoText(path)){
      if (!(fp = FileOpen(path, "r"))) {
        sprintf(strErr, "Ingenue can't find file %s", path);
        Message (MSG_OK, strErr);
        return FALSE;
      }
      /* if file of seqid list, get GI_list */
      if (igvp->filetype > 119){ 
        GI_list=Ing_ParseSeqIdFile(fp);
        path = NULL;
        FileClose(fp);
        fp=NULL;
        if (!GI_list) return FALSE;
        if (igvp->filetype == Ing_SEQIDLISTFORBLAST)
          igvp->filetype=ALIGN_BLAST2SEQ;
        if (igvp->filetype == Ing_SEQIDLISTFORSPIDEY)
          igvp->filetype = Ing_SPIDEY;
      }
    }
    

    if (igvp->filetype==ALIGN_BLAST2SEQ)
      Ing_ReadIn2ndBsp(igvp, path, GI_list);
    else {

      if (igvp->filetype==Ing_GENSCAN){
        Ing_ReadInGenscan(fp, igvp);
      }
      else if (igvp->filetype==Ing_SPIDEY){
        Ing_ReadSpideyInfo(igvp, path, GI_list);
        DoUpdate=FALSE;
      }
      else if (igvp->filetype==ALIGN_FILE)
        {
          Ing_ReadInBlastAsn(igvp,fp);
          DoUpdate=FALSE;
        }
      else if (igvp->filetype==Ing_FEAT_TABLE)
        {
          Ing_ReadInFeatureTable(igvp, fp);
          igvp->talign.update = TRUE;
        }
      else if (igvp->filetype==Ing_TBDL){
        Ing_ReadInTabDelimited(igvp, igvp->bsp,  fp);
        igvp->talign.update = TRUE;
      } 
      else if (igvp->filetype == Ing_DOTMATRIX) {
        Ing_ReadDotMatrixInfo(igvp, path, GI_list);
        DoUpdate = FALSE;
      }
      else if (FALSE){/* seqid file*/
        GI_list=Ing_ParseSeqIdFile(fp);
        if (!GI_list) return FALSE;
        bsp=(BioseqPtr)GetBioseqGivenIDs(igvp->entityID, igvp->itemID, OBJ_BIOSEQ);
        if (!bsp) return FALSE;
        if (igvp->filetype==Ing_SEQIDLISTFORSPIDEY)
          Ing_CreateSpideyForm(igvp, bsp, NULL, GI_list);
        if (igvp->filetype==Ing_SEQIDLISTFORBLAST)
          Ing_CreateBlast2SeqsForm(igvp, bsp, NULL, GI_list);
        
        DoUpdate=FALSE;
      }
      
      if (fp)
        FileClose(fp); 
      if (DoUpdate){
        Ing_PopulateOverviewPage((Pointer)igvp, FALSE);
        Ing_PopulateDetailedPage((Pointer)igvp);
      }
    } 
    
    
  }
  else{
    return FALSE;
  }
  return TRUE;
}

static Boolean Ing_FileOpenMain(CharPtr path, IngGenomeViewerPtr igvp)
{
  FILE * fp;
  Uint2             datatype, entityID;
  SeqEntryPtr       sep_orig=NULL, sep=NULL;
  BioseqPtr         bsp=NULL;
  Pointer           dataptr=NULL;
  Char              strErr[50]={""};


  
  WatchCursor();
  Ing_ProgressNew(igvp, "Reading File.....");
    if (path != NULL)
      {
        if (!(fp = FileOpen(path, "r"))) {
          ArrowCursor();
          Ing_ProgressEnd(igvp);
          sprintf(strErr, "Ingenue can't find your file %s", path);
          Message (MSG_OK, strErr);
          return FALSE;
        }
        if ((dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, &entityID, TRUE, TRUE, FALSE, FALSE)) != NULL) 
          { 
            if (datatype==OBJ_SEQENTRY || datatype==OBJ_BIOSEQ){
              sep= GetTopSeqEntryForEntityID(entityID);

              igvp->entity_list=(IngEntityPtr)MemNew(sizeof(IngEntity));
              igvp->entity_list->entityID=entityID;
              igvp->entity_list->itemID=1;
              igvp->entity_list->bspcount=Ing_AllButPartsCount(sep, NULL, NULL, 0,0);
              igvp->entity_list->Sips=SAM_ExtractSips(sep);
              bsp=BioseqFindInSeqEntry((SeqId *)igvp->entity_list->Sips->data.ptrvalue, sep);/* get bsp of first SeqId */
            }
            else {
              Message (MSG_OK, "File is not a bioseq or seqentry");
              ArrowCursor();
              Ing_ProgressEnd(igvp);
              return FALSE;
            }
            FileClose(fp);
            if (!bsp) {
              ArrowCursor();
              Ing_ProgressEnd(igvp);
              return FALSE;
            }
            igvp->cur_sip=(SeqId *)igvp->entity_list->Sips->data.ptrvalue;
            igvp->cur_target=1;
            FileClose(fp);
            Ing_ProgressEnd(igvp);
            Ing_RunIngenue(igvp, bsp, entityID);
          }
        else {
          Ing_ProgressEnd(igvp);
          FileClose(fp);
        }
      }
    else{
      ArrowCursor();
      Ing_ProgressEnd(igvp);
      return FALSE;
    }
    ArrowCursor();
    return TRUE;
}



static Int4  Ing_GetTPMaxScale(IngGenomeViewerPtr igvp, Int2 vwidth)
{
  Int4   SeqLabel, scale;

  SeqLabel=(igvp->SeqLength)/6;
  scale = MAX(1, (igvp->SeqLength +SeqLabel )/(vwidth*igvp->ZoomedVal));
  
  return scale;
}

static Int4  Ing_GetBPMaxScale(IngGenomeViewerPtr igvp)
{
  RecT  rc;
  Int2  vwidth;

  GetPosition(igvp->vBottom, &rc);
  vwidth = rc.right-rc.left;

  if (!igvp->maxScaleX)
    igvp->maxScaleX=Ing_GetTPMaxScale(igvp, vwidth);
  return MIN(Ing_MaxScale, igvp->maxScaleX);
}


/*******************************************************************************

  Function : Ing_ResizeMainWindow(), Ing_ResizeWin()
  
  Purpose : resize window functions

*******************************************************************************/
static void Ing_ResizeMainWindow(IngGenomeViewerPtr igvp, WindoW w, Boolean bfirst)
{
  Int4     diff, Zoom, scaleX;
  RecT     rcDlg;
  RecT     rcVsb,rcHsb, rcZoom, rcInfo;
  RecT     rcTop; 
  RecT     rcRuler1;
  RecT     rcBottom;
  RecT     rcRuler2;
  RecT     rcGoto; 
  RecT     rcPrompt;
  Int2     height,width,gap,vsbWidth;
  Int2     in, hsbHeight;
  Int2     GotoHeight; 
  Int2     RulerHeight, InfoHeight;
  Int2     PromptHeight;
  Int2     TopHeight;
  Int2     ZoomWidth;
  Int4     i;
  Int4     line_height;
  BaR      vsb,hsb;
 
	/*main window size*/
	ObjectRect(w,&rcDlg);
	width= rcDlg.right-rcDlg.left;
	height= rcDlg.bottom-rcDlg.top;
   SafeHide(igvp->vRuler1);
   SafeHide(igvp->vTop);
   SafeHide(igvp->vRuler2);
   SafeHide(igvp->vBottom);
   SafeHide(igvp->vZoom);
   SafeHide(igvp->deflineg);
   SafeHide(igvp->progress_prompt);
   /* reset static prompt */

   Update();

	vsb = GetSlateVScrollBar ((SlatE) igvp->vBottom);
	hsb = GetSlateHScrollBar ((SlatE) igvp->vBottom);
   GetPosition(igvp->vTop, &rcTop);
   GetPosition(igvp->vRuler1, &rcRuler1);
   GetPosition(igvp->vBottom, &rcBottom);
   GetPosition(igvp->vRuler2, &rcRuler2);
   GetPosition(igvp->vZoom, &rcZoom);
   GetPosition(igvp->deflineg, &rcInfo);
   GetPosition(igvp->progress_prompt, &rcPrompt);
   GetPosition(hsb,&rcHsb);
   GetPosition(igvp->d_Win->Goto, &rcGoto);
   GetPosition(vsb,&rcVsb);

	gap=10;
	in=2;
   hsbHeight=rcHsb.bottom-rcHsb.top;
	vsbWidth=rcVsb.right-rcVsb.left;
   RulerHeight=(RULERDEFHEIGHT*igvp->GrData.cyChar);
   TopHeight=igvp->TopHeight;
   InfoHeight=rcInfo.bottom-rcInfo.top;
   GotoHeight=rcGoto.bottom-rcGoto.top;
   PromptHeight = rcPrompt.bottom-rcPrompt.top;
   ZoomWidth = rcZoom.right - rcZoom.left;
   line_height = LineHeight();

   rcRuler1.bottom=rcRuler1.top+RulerHeight + 20;
   rcRuler1.right=width-in-2*vsbWidth;
   rcTop.right= rcRuler1.right+vsbWidth;
   rcTop.top=rcRuler1.bottom;
   rcTop.bottom=rcTop.top+TopHeight;
   rcRuler2.top=rcTop.bottom+2*gap;
   rcRuler2.right=rcRuler1.right;
   rcRuler2.bottom=rcRuler2.top+RulerHeight + 20;
   rcBottom.top=rcRuler2.bottom;
   rcBottom.right=rcRuler1.right+vsbWidth;
   rcBottom.bottom=height-hsbHeight-2*gap-PromptHeight;
   rcPrompt.top = rcBottom.bottom + hsbHeight + gap;
   rcPrompt.bottom= rcPrompt.top + igvp->GrData.cyChar;
   rcZoom.right = MAX(rcRuler1.right, rcGoto.right+gap+ZoomWidth);
   rcZoom.left = rcZoom.right - ZoomWidth;
   rcGoto.bottom = rcGoto.top + GotoHeight;
   rcInfo.top = rcGoto.bottom + in;
   rcInfo.bottom = rcInfo.top + line_height;
   rcInfo.right = rcZoom.left - gap;

   SetPosition(igvp->vRuler1, &rcRuler1);
   SetPosition(igvp->vTop, &rcTop);
   SetPosition(igvp->vZoom, &rcZoom);
   SetPosition(igvp->deflineg, &rcInfo);
   SetPosition(igvp->vRuler2, &rcRuler2);
   SetPosition(igvp->vBottom, &rcBottom);
   SetPosition(igvp->progress_prompt, &rcPrompt);
   for (i = 0; i < 4; i++){
     SetPosition(igvp->featInfo[i], &rcInfo);
     rcInfo.top += line_height;
     rcInfo.bottom += line_height;
   }
   /*rescale the Picture/Viewer windows*/
   diff = rcTop.right-rcTop.left;
   scaleX=Ing_GetTPMaxScale(igvp, diff);
   igvp->maxScaleX=scaleX;
   if (!bfirst){
     scaleX=Ing_GetBPMaxScale(igvp);
     Ing_UpdateZoomerVals(igvp, scaleX);
   }
   if (bfirst){
     SafeShow(igvp->vRuler1);
     SafeShow(igvp->vTop);
     SafeShow(igvp->vZoom);
     SafeShow(igvp->vRuler2);
     SafeShow(igvp->vBottom);
     SafeShow(igvp->deflineg);
     SafeShow(igvp->progress_prompt);
     Update();
   }
}

static void Ing_ResizeWin(WindoW w)
{
IngGenomeViewerPtr igvp;
Int4 from,to;
	
	igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
	if (!igvp) return;
	
	Ing_ResizeMainWindow(igvp,w, FALSE);
   Ing_PopulateOverviewPage((Pointer) igvp, TRUE);
   Ing_PopulateDetailedPage((Pointer)igvp);
   /* attach zoomer picture */
   AttachPicture (igvp->vZoom,igvp->pictZoom, INT4_MIN, INT4_MAX, UPPER_RIGHT, 1 , 1, NULL); 
   SetViewerProcs(igvp->vZoom,Ing_ZoomerClickProc,NULL,NULL,  NULL);
   
   Ing_GetNamesZoomedVal(igvp->vRuler2,&from,&to,NULL,igvp->SeqLength);
   Ing_MoveCursor(igvp,from,to, TRUE); 
   igvp->xposition=from;
   SafeShow(igvp->vRuler1);
   SafeShow(igvp->vTop); 
   SafeShow(igvp->vZoom);
   SafeShow(igvp->vRuler2);
   SafeShow(igvp->vBottom); 
   SafeShow(igvp->deflineg); 
   SafeShow(igvp->progress_prompt);
 
   Update();
}


/***********************************************************
Function(s): ..

Purpose: Open procs

*************************************************************/
static void  Ing_OpenAcceptProc(ButtoN b)
{
  FILE * fp;
  IngGenomeViewerPtr igvp;
  Char              path [PATH_MAX]={""}, GI_list[PATH_MAX]={""};
  IngFileOpenDataPtr dfodp=NULL;
  WindoW            w;

  w=ParentWindow(b);

  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  if (!igvp) return;

  dfodp=(IngFileOpenDataPtr)GetObjectExtra(b);
  if (!dfodp) return;
  GetTitle(dfodp->PathName, path, sizeof(path));
  if (StringHasNoText (path)) {
    GetTitle(dfodp->GI, GI_list, sizeof(GI_list));
    if (StringHasNoText (GI_list)) {
      Message (MSG_OK, "Enter file name, GI or Accession number");
      Select (w);
      Show (w);
      Select (dfodp->PathName);
      return;
    }
  }

  Hide(ParentWindow(b));
  /* if -- new sequence */
  if (igvp->filetype <= 1){
    if (Ing_FileOpenMain(path, igvp)){
      Remove(ParentWindow(b));
      ProcessEvents();
    }
    else{
      SetTitle(dfodp->PathName, "");
      Disable(dfodp->OK);
      Show(ParentWindow(b));
    }
  }
  else{
    if (Ing_FileOpenSpecial(igvp, path, GI_list)){
         Remove(ParentWindow(b));
    }
    else{
      SetTitle(dfodp->PathName, "");
      Disable(dfodp->OK);
      Show(ParentWindow(b));
    }
  }

  if (dfodp->hStart){
    Remove(dfodp->hStart);
    dfodp->hStart=NULL;
  }
  /* else complain */
  
 }



static void Ing_OpenTextProc(TexT t)
{
Char               szFName[PATH_MAX]={""};
WindoW             hOpenDlg;
IngFileOpenDataPtr  igodp;

        
 igodp = (IngFileOpenDataPtr) GetObjectExtra (t);
 
 if (igodp==NULL) return;
 
 GetTitle(t, szFName, sizeof(szFName)-1);
 
 if (StringLen(szFName) == 0){
   Enable(igodp->g_GI);
   Disable(igodp->OK);
 }
 else{
   Enable(igodp->OK);
   Disable(igodp->g_GI);
 }
}


static void Ing_OpenCancelProc(ButtoN b)
{
  IngFileOpenDataPtr dfodp=NULL;


  dfodp=(IngFileOpenDataPtr)GetObjectExtra(b);
  if (dfodp && dfodp->hStart){
    Select(dfodp->hStart);
    Show(dfodp->hStart);
  }
  Remove(ParentWindow(b));

}

static void Ing_OpenBrowseProc(ButtoN b)
{
  IngFileOpenDataPtr igodp=NULL;
  Char   path[PATH_MAX]={""};

  igodp = (IngFileOpenDataPtr) GetObjectExtra (b);
  
  if (igodp==NULL) return;
  
  if (GetInputFileName (path, sizeof(path), NULL, NULL)){ 
    SetTitle(igodp->PathName, path);
    Ing_OpenTextProc(igodp->PathName);
  }
  return;  
}


static void Ing_NetOpen_access(TexT t)
{
Char                    szAccess[50]={""};
WindoW                  hOpenDlg;
IngFileOpenDataPtr       dfodp=NULL;

        hOpenDlg=(WindoW)ParentWindow(t);

        if (!hOpenDlg) return;
        
        dfodp = (IngFileOpenDataPtr) GetObjectExtra (t);

        if (dfodp==NULL) return;
        
        GetTitle(t, szAccess, sizeof(szAccess));

        if (StringLen(szAccess) == 0){
          Enable(dfodp->g_File);
          Disable(dfodp->OK);
        }
        else {
          Enable(dfodp->OK);
          Disable(dfodp->g_File);
        }
        return;
}


static void Ing_InputFileType(PopuP pops)
{
  IngGenomeViewerPtr igvp=NULL;
  Int2               value;

  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(pops));
  if (!igvp) return;
  value=GetValue(pops);

  if (igvp->filetype==ALIGN_BLAST){
    if (value == 2)
      igvp->filetype = Ing_SEQIDLISTFORBLAST;
  }
  else if (igvp->filetype==Ing_SPIDEY){
    if (value == 2)
      igvp->filetype = Ing_SEQIDLISTFORSPIDEY;
  }
}



extern void Ing_OpenFromFileORNetwork(IngGenomeViewerPtr igvp)
{
  WindoW              hOpenDlg=NULL;
  GrouP               g, c, g_top;
  IngFileOpenDataPtr  dfodp=NULL;
  ButtoN              b,b1,b2;
  TexT                t1;


  if (igvp==NULL) return;

  dfodp=(IngFileOpenDataPtr)MemNew(sizeof(IngFileOpenData));
  
  hOpenDlg = MovableModalWindow(-30, -20,  -10,  -10, "Ingenue - Get Sequence",  NULL);
  
  g_top = HiddenGroup (hOpenDlg, -1, 4, NULL);

  g = NormalGroup(g_top, -3, 1, "File name:",  systemFont, NULL);
  dfodp->g_File=g;

  SetGroupMargins(g, 10, 10);
  SetGroupSpacing(g, 10, 20); 
  dfodp->pops = PopupList (g, TRUE, Ing_InputFileType);
  PopupItem (dfodp->pops, "Asn or Fasta file");
  PopupItem (dfodp->pops, "list of SeqIds");
  SetValue(dfodp->pops, 1);

  dfodp->PathName = DialogText(g,"",15,Ing_OpenTextProc);
  SetObjectExtra(dfodp->PathName, (Pointer)dfodp, StdCleanupExtraProc);
  b = PushButton(g, " browse...", Ing_OpenBrowseProc);
  SetObjectExtra (b, (Pointer) dfodp, NULL);
/*   dfodp->OK  = DefaultButton(g, "Accept",  Ing_OpenAcceptProc); */
  
  StaticPrompt (g_top, "OR", 0, LineHeight(), systemFont, 'c');

  g = NormalGroup (g_top, -1, 1, "GI or Accession", systemFont,NULL);
  dfodp->g_GI=g;

  SetGroupMargins(g, 10, 10);
  SetGroupSpacing(g, 10, 20); 
  dfodp->GI=DialogText (g, "", 15, Ing_NetOpen_access);
  SetObjectExtra(dfodp->GI, (Pointer)dfodp, NULL);

  /*retrieve - cancel*/
  g = HiddenGroup (g_top, -2, -1, NULL);

  dfodp->OK=DefaultButton (g, "Accept", Ing_OpenAcceptProc);
  Disable(dfodp->OK);
  b2 = PushButton(g, "Cancel",  Ing_OpenCancelProc);
  
  
  SetObjectExtra (dfodp->OK, (Pointer) dfodp, NULL);
  SetObjectExtra (hOpenDlg, (Pointer)igvp, NULL);

  
  Select(hOpenDlg);
  Show(hOpenDlg);
  
}



extern void Ing_FileOpenProc(IngGenomeViewerPtr igvp, WindoW hStart)
{
IngFileOpenDataPtr               dfodp=NULL;
WindoW                          hOpenDlg;
GrouP                           g,g4;
ButtoN                          b,b1,b2;
TexT                            t1;    

    if (igvp==NULL) return;
  
    dfodp=(IngFileOpenDataPtr)MemNew(sizeof(IngFileOpenData));
           
    hOpenDlg = MovableModalWindow(-30, -20,  -10,  -10, "Ingenue - Open a local file",  NULL);
    g = NormalGroup(hOpenDlg, -2, 2, "File name:",  systemFont, NULL);
    SetGroupMargins(g, 10, 10);
    SetGroupSpacing(g, 10, 20);  
    t1 = DialogText(g,"",25,Ing_OpenTextProc);
    SetObjectExtra(t1, (Pointer)dfodp, StdCleanupExtraProc);
    b = PushButton(g, " browse...", Ing_OpenBrowseProc);
    SetObjectExtra (b, (Pointer) dfodp, NULL);
    dfodp->pops=NULL;
    b1 = DefaultButton(g, "OK",  Ing_OpenAcceptProc);
    Enable(b1);
    b2 = PushButton(g, "Cancel",  Ing_OpenCancelProc);
    SetObjectExtra (b1, (Pointer) dfodp, NULL);
    SetObjectExtra (b2, (Pointer) dfodp, NULL);
    Disable(b1);
    dfodp->OK = b1;
    dfodp->PathName=t1;
    dfodp->hStart=hStart;
    SetObjectExtra (hOpenDlg, (Pointer) igvp, NULL);
    Select(hOpenDlg);
    Show(hOpenDlg);
}

static void Ing_OpenGenscan(IteM i)
{
  IngGenomeViewerPtr igvp;
  WindoW            w;

  w=ParentWindow(i);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  igvp->filetype=Ing_GENSCAN;
  Ing_FileOpenProc(igvp, NULL); 
}

static void Ing_OpenBlast(IteM i)
{
  IngGenomeViewerPtr igvp;
  WindoW            w;

  w=ParentWindow(i);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  igvp->filetype=ALIGN_FILE;

  Ing_FileOpenProc(igvp, NULL);
}

static void Ing_OpenFeatTable(IteM i)
{
  IngGenomeViewerPtr igvp;
  WindoW            w;

  w=ParentWindow(i);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  igvp->filetype=Ing_FEAT_TABLE;
  Ing_FileOpenProc(igvp, NULL);

}
static void Ing_OpenTabDel(IteM i)
{
  IngGenomeViewerPtr igvp;
  WindoW            w;

  w=ParentWindow(i);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  igvp->filetype=Ing_TBDL;
  Ing_FileOpenProc(igvp, NULL);

}


/*******************************************************************************

  Function : Ing_EditFeature(), Ing_DeleteFeature(), Ing_MarkSeqAnnot()
  
  Purpose :  Edit or delete features

*******************************************************************************/

static void Ing_EditFeature(IteM i)
{
  SelStructPtr ssp=NULL;
  IngGenomeViewerPtr igvp=NULL;
  Int2       handled; 
  SeqEntryPtr sep;

  ssp=ObjMgrGetSelected();
  if (!ssp) {
    ErrPostEx(SEV_ERROR, 0, 0,"need to select a feature" );
    return;
  }
  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(i));

  WatchCursor ();
  handled=GatherProcLaunch (OMPROC_EDIT, FALSE, ssp->entityID, ssp->itemID, ssp->itemtype, 0, 0, ssp->itemtype, 0);
  ArrowCursor();
  if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
    ErrPostEx(SEV_ERROR, 0, 0,"unable to launch editor on feature" );
  }
}


typedef struct ing_markdata {
  SeqAlignPtr parent;
  Boolean     bSpidey;
  Int4        idx;
} IngMarkData, PNTR IngMarkPtr;


static void is_parent_spidey(SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)
{
  BioseqPtr     bsp=NULL;
  BioseqSetPtr  bssp=NULL;
  SeqAnnotPtr   sanp=NULL;
  SeqAlignPtr   salp=NULL;
  SeqAlignPtr   parent=NULL; 
  IngMarkPtr    markp = NULL; 
  Boolean       bSpidey = FALSE;

 markp=(IngMarkPtr)mydata;
 parent = markp->parent;

 if (IS_Bioseq(sep)) {
   bsp = (BioseqPtr)sep->data.ptrvalue;
   sanp = bsp->annot;
 } else if (IS_Bioseq_set(sep)) {
   bssp = (BioseqSetPtr)sep->data.ptrvalue;
   sanp = bssp->annot;
 } else return;
 while (sanp != NULL) {
   if (sanp->type == 2) {/*seqalign*/
     if (StringCmp(sanp->name, "Spidey") == 0){
       bSpidey = TRUE;
     }
     salp=(SeqAlignPtr)sanp->data;
     if (salp==parent){
       if (bSpidey){
         markp->bSpidey = TRUE;
       }
       return;
     }
   }
   sanp=sanp->next;
 }

}

static void Ing_MarkSeqAnnot(SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)
{
BioseqPtr     bsp=NULL;
BioseqSetPtr  bssp=NULL;
SeqAnnotPtr   sanp=NULL;
SeqAlignPtr   salp=NULL;
SeqAlignPtr   parent=NULL; 
IngMarkPtr    markp = NULL; 
Boolean       bSpidey = FALSE;
 Boolean       bNoParent = FALSE; 
 Int4          idx = 0;

 markp=(IngMarkPtr)mydata;
 parent = markp->parent;

 if (IS_Bioseq(sep)) {
   bsp = (BioseqPtr)sep->data.ptrvalue;
   sanp = bsp->annot;
 } else if (IS_Bioseq_set(sep)) {
   bssp = (BioseqSetPtr)sep->data.ptrvalue;
   sanp = bssp->annot;
 } else return;


 while (sanp != NULL) {
   if (sanp->type == 2) {/*seqalign*/
     if (StringCmp(sanp->name, "Spidey") == 0){
       bSpidey = TRUE;
       idx++;
     }
     salp=(SeqAlignPtr)sanp->data;
     if (salp==parent){
       sanp->idx.deleteme=1;
       if (bSpidey){
         markp->bSpidey = TRUE;
         markp->idx = idx;
       }
       return;
     }
   }
   sanp=sanp->next;
 }
}


static void Ing_DeleteFeature(IteM i)
{
  SelStructPtr ssp=NULL;
  SeqFeatPtr   sfp=NULL;
  SeqAlignPtr  child=NULL, parent=NULL;
  SeqAlignPtr  salp_head=NULL;
  SeqAlignPtr  salp_prev=NULL;
  SeqAlignPtr  salp=NULL;
  Boolean      Only_sap=FALSE;
  SeqEntryPtr  sep=NULL;
  Boolean      found=FALSE;
  IngGenomeViewerPtr igvp=NULL;
  Uint2        entityID = 0;
  IngMarkData        marked;


  ssp=ObjMgrGetSelected();
  if (!ssp) 
    return;
  else 
    entityID = ssp->entityID;

  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(i));
  if(!igvp) return;
  while (ssp){
    if (ssp->entityID!=0 && ssp->itemID!=0){
      if (ssp->itemtype==OBJ_SEQALIGN){
        child=(SeqAlignPtr)GetPointerForIDs(ssp->entityID, ssp->itemID, OBJ_SEQALIGN);
        if (!child) goto next;
        parent=AlnMgr2GetParent(child);
        salp=(SeqAlignPtr)parent->segs;
        while (salp && !found)
          {
            if (salp == child)
              found = TRUE;
            else
              {
                if (salp_head)
                  salp_prev = salp;
                else
                  salp_head = salp_prev = salp;
                salp = salp->next;
              }
          }
        if (!found)
          goto next;
        if (salp_head != NULL)
          {
            salp_prev->next = salp->next;
            salp->next = NULL;
            SeqAlignFree(salp);
          } else    
            {
              if (salp->next==NULL)
                Only_sap=TRUE;
              else {
                salp_head = salp->next;
                salp->next = NULL;
                SeqAlignFree(salp);
              }
            }
        if (!Only_sap){
          parent->segs = (Pointer)salp_head;
          if (parent->saip != NULL)
            AMAlignIndex2Free2(parent);
          parent->saip=NULL;
          AlnMgr2IndexLite(parent); 
          marked.parent = parent;
          marked.idx = 0;
          marked.bSpidey = FALSE;
          sep = GetTopSeqEntryForEntityID(igvp->entityID); 
          SeqEntryExplore(sep,(Pointer)&marked, is_parent_spidey);
          if (marked.bSpidey){
            Disable(igvp->d_Win->mrnaorexons);
          }
        }
        else{
          marked.parent = parent;
          marked.idx = 0;
          marked.bSpidey = FALSE;
          sep = GetTopSeqEntryForEntityID(igvp->entityID); 
          SeqEntryExplore(sep,(Pointer)&marked, Ing_MarkSeqAnnot);
          DeleteMarkedObjects(igvp->entityID, OBJ_SEQENTRY, (Pointer)sep);
          if (marked.bSpidey){
            Ing_DeleteSpidey(igvp, marked.idx);
          }
        }
      }
      else if (ssp->itemtype==OBJ_SEQFEAT){
        sfp=(SeqFeatPtr)GetPointerForIDs(ssp->entityID, ssp->itemID, OBJ_SEQFEAT);
        sfp->idx.deleteme=TRUE;
        DeleteMarkedObjects(ssp->entityID, OBJ_SEQFEAT, (Pointer)sfp);
      }
    }
  next:
    ssp = ssp->next;
  }
  if (entityID){
    ObjMgrSetDirtyFlag (entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
    ObjMgrDeSelectAll();
  }

  return;
}

/***********************************************************

  Function : Ing_ReplaceAllMismatched()
  
  Purpose :  Replace mismatched residues in genomic sequence

***********************************************************/
typedef struct ing_repmismatched{
  SeqAlignPtr sap;
  TexT        start;
  TexT        stop;
  ButtoN      all;
  ButtoN      gaps;
  ButtoN      inserts;
  GrouP       region;
} IngRepMismatched, PNTR IngRepMismatchedPtr;

static void Ing_ReplaceCancelProc(ButtoN b)
{
  Remove(ParentWindow(b));
}

static void Ing_ReplaceMismatchedProc(ButtoN b)
{
  IngGenomeViewerPtr igvp = NULL;
  IngRepMismatchedPtr rmp = NULL;
  WindoW              w;
  Int4                start, stop;
  Char                str[20]={""};


  w = ParentWindow(b);
  igvp = (IngGenomeViewerPtr)GetObjectExtra(w);
  rmp = (IngRepMismatchedPtr)GetObjectExtra(b);
  Hide(w);

  if (igvp == NULL || rmp == NULL){
    Show(w);
    return;
  }
  if (!GetStatus(rmp->all)){
    GetTitle(rmp->start, str, sizeof(str));
    start = atoi(str);
    GetTitle(rmp->stop, str, sizeof(str));
    stop = atoi(str);
  }

  Ing_ReplaceAllMismatchedInLocation(igvp, rmp->sap, start, stop, GetStatus(rmp->all), GetStatus(rmp->gaps), GetStatus(rmp->inserts));
  Remove(w);
}

static void Ing_ReplaceAllProc(ButtoN b)
{
  IngRepMismatchedPtr rmp;

  rmp = (IngRepMismatchedPtr)GetObjectExtra(b);

  if(GetStatus(rmp->all))
    Disable(rmp->region);
  else
    Enable(rmp->region);
}


static void Ing_CreateMismatchedDialog(IngGenomeViewerPtr igvp, SeqAlignPtr salp)
{
  IngRepMismatchedPtr rmp = NULL;
  WindoW           w;
  Int4             start, stop;
  GrouP            g, g1, g2, g3, main;
  ButtoN           b;
  Char             buf[20]={""};
  


  AlnMgr2GetNthSeqRangeInSA(salp, 1, &start, &stop);

  rmp = (IngRepMismatchedPtr)MemNew(sizeof(IngRepMismatched));
  w = MovableModalWindow(-50, -33 ,-10, -10, "Spidey", NULL);
  SetObjectExtra(w, (Pointer) igvp, NULL);

  rmp->sap = salp;
  main = HiddenGroup(w, 1, 3, NULL);
  rmp->all = CheckBox(main, "Replace All Mismatches in Alignment", Ing_ReplaceAllProc);
  SetObjectExtra(rmp->all, (Pointer)rmp, NULL);
  SetStatus(rmp->all, TRUE);

  g1 = NormalGroup(main, 1, 1, "- OR Select Range - ", NULL, NULL);
  rmp->region = g1;

  g = HiddenGroup(g1, 5, 1, NULL);
  StaticPrompt(g, "Genomic coordinates:   ", 0, 0, programFont, 'l');
  StaticPrompt(g, "from", 0, 0, programFont, 'l');
  sprintf(buf, "%d", start + 1);
  rmp->start = DialogText(g, buf, 5, NULL);
  StaticPrompt(g, "   to", 0, 0, programFont, 'l');
  sprintf(buf, "%d", stop + 1);
  rmp->stop = DialogText(g, buf, 5, NULL);
  Disable(rmp->region);

  rmp->gaps = CheckBox(main, "Fill in gaps on genomic", NULL);
  SetStatus(rmp->gaps, TRUE);
  rmp->inserts = CheckBox(main, "Delete inserts on genomic", NULL);
  SetStatus(rmp->inserts, TRUE);


  g2 = HiddenGroup(main, 2, 1, NULL);
  b = DefaultButton(g2, "Accept", Ing_ReplaceMismatchedProc);
  PushButton(g2, "Cancel", Ing_ReplaceCancelProc);
  SetObjectExtra(b, (Pointer)rmp,  NULL/* StdCleanupExtraProc */);
  Select(w);
  Show(w);

}

static void Ing_ReplaceAllMismatched(IteM i)
{
  IngGenomeViewerPtr igvp = NULL;
  SelStructPtr       ssp;
  SeqAlignPtr        salp = NULL;

  igvp = (IngGenomeViewerPtr)GetObjectExtra(ParentWindow(i));
  ssp = ObjMgrGetSelected();

  while (ssp){
    salp=(SeqAlignPtr)GetPointerForIDs(ssp->entityID, ssp->itemID, ssp->itemtype);
    if (salp){
      Ing_CreateMismatchedDialog(igvp, salp);
    }
    ssp = ssp->next;
  }
}


/*******************************************************************************

  Function : Ing_AnnotateAlignmentCancel(), Ing_AnnotateAlignmentAccept(), Ing_AnnotateAlignmentForm()
  
  Purpose :  change an alignment into a feature

*******************************************************************************/
#define _success_ 1
#define _already_exists_ 2
#define _failed_to_map_ 3


typedef struct ing_progfeat {
  BioseqPtr bsp_mrna;
  Int4      mRNA_left;
  Int4      genomic_left;
  Char      name[300];
  CharPtr   orig_loc;
  CharPtr   new_loc;
  CharPtr   str_prot;
  CharPtr   str_new;
  Uint1     type;
  Boolean   found;
  Boolean   prot_ok;
  Uint1     status;
  Uint1     strand;
  struct ing_progfeat PNTR next;
} IngProgFeat, PNTR IngProgFeatPtr;


typedef struct ing_annotalign {
  BioseqPtr bsp_mrna;
  SeqFeatPtr sfp_mrna;
  Boolean   holes;
  ButtoN    propc;
  WindoW    w;
  VieweR    viewer;
  SegmenT   seg;
  Int4      rowNum;
  SeqAlignPtr child;
  GrouP     feat_type;
  GrouP     gap_split;
  LisT      feat_list;
  Int4      nList;
  IngProgFeatPtr pfp;
  struct ing_annotalign PNTR next;
} IngAnnotAlign, PNTR IngAnnotAlignPtr;

Boolean mRNAFilter[FEATDEF_MAX];
Boolean mRNATrack[FEATDEF_MAX];
Uint2   mRNALookup[FEATDEF_MAX];


static Boolean LIBCALLBACK Ing_CheckGenFeats(SeqFeatPtr sfp, SeqMgrFeatContextPtr sfc)
{
  Char str[500]={""};
  IngProgFeatPtr pfp = NULL;

  pfp = (IngProgFeatPtr)sfc->userdata;
  FeatDefLabel(sfp, str, sizeof (str) - 1, OM_LABEL_BOTH);
  if (StringCmp(pfp->name, str) == 0 && 
      pfp->type == sfc->featdeftype){
    pfp->found = TRUE;
    return (FALSE);
  }
  return (TRUE);
}

static void Ing_PropagateClickProc(VieweR v, SegmenT s, PoinT pt)
{
  IngGenomeViewerPtr igvp=NULL;
  SelStructPtr       ssp=NULL;
  PrimitivE          prim;
  Uint2              entityID=0;
  Uint4              itemID=0;
  Uint2              itemtype=0;
  Uint2              primID=0;

  /*
  igvp=(IngGenomeViewerPtr)GetObjectExtra((WindoW)ParentWindow(v));
  if (!igvp) return;

  ssp=ObjMgrGetSelected();
  if (ssp){
    ObjMgrDeSelect(ssp->entityID, ssp->itemID, ssp->itemtype, 0, NULL);
  }

  if (FindSegPrim(v, pt, NULL, NULL, &prim))
   {
     GetPrimitiveIDs(prim, &entityID, &itemID, &itemtype, &primID);
     if (entityID && itemID && itemtype){
       msg.labelmsg=2;
       ObjMgrSelect (entityID, itemID, itemtype, 0, NULL);
       return;
     }
   }
  Ing_ResetTitle(igvp, TRUE);
  */
}

static void Ing_DrawScale(SegmenT seg, Int2 margin, Int4 res, Int4 pos, Int4 ybase, Int2 chw, Int2 Fh)
{
  Int4   x, y;
  Char   str[15] = {""};
  Uint1  blue[3];

  blue[0] = 0;
  blue[1] = 125;
  blue[2] = 245;

  x = margin + (pos * chw) + chw/2;
  y = ybase + Fh;

  sprintf(str,"%d", res);

  AddAttribute(seg, COLOR_ATT, blue, 0,0,0,0);
  AddLabel(seg, x, y, str, SMALL_TEXT, 0, UPPER_CENTER, 0);

  AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
  AddLabel(seg, x, y - 2, ".", SMALL_TEXT, 0, UPPER_CENTER, 0);

}

static void Ing_HighlightMismatch(SegmenT seg, Int2 margin, Int4 pos, Int4 ybase, Int2 chw, Int2 Fh)
{
  Int4 x1, x2, y1, y2;
  Uint1    yellow[3];


  yellow[0] = 224;
  yellow[1] = 224;
  yellow[2] = 0;

  x1 = margin + (pos * chw);
  x2 = x1 + chw;
  y1 = ybase + Fh;
  y2 = ybase - Fh;

  AddAttribute(seg, COLOR_ATT, yellow, 0,0,0,0);
  AddRectangle (seg, x1, y1, x2, y2, 0, TRUE, 0);
  AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
}

#define BLOCK_SIZE 10

static void Ing_PrintCompProteins (IngAnnotAlignPtr aap, IngProgFeatPtr pfp)
{
  Int4       len_orig, len_new, len_min, width_orig;
  Int4       n;
  Int4       i, j, k, row, add;
  Int4       start, exit;
  Int2       winsize, cxChar, nchars_row;
  Int2       last_pos = 0;
  Int2       margin, Fh, strw_new = 0, strw_orig = 0;
  Uint1      clr[3], cl2[3];
  CharPtr    str_orig, str_new;
  CharPtr    buf_orig, buf_new;
  CharPtr    orig, new;
  Char       str1[200] = {""}, str2[10] = {""};
  RecT       rc;
  SegmenT    seg;
  


  str_orig = pfp->str_prot;
  str_new = pfp->str_new;
  
  if (str_orig == NULL || str_new == NULL)
    return; /* should add a message here */

  margin = StringWidth("Genomic - ");
  clr[0] = 210;
  clr[1] = 154;
  clr[2] =  14;

  cl2[0] = 224;
  cl2[1] = 95;
  cl2[2] = 39;

  seg = aap->seg;
  GetPosition(aap->viewer, &rc);
  winsize = (rc.right - rc.left) - (margin + 3*stdCharWidth);
  row = aap->rowNum;
  
  n = 1;
  start = 1;

  len_orig = StringLen(str_orig);
  len_new = StringLen(str_new);
  len_min = MIN(len_orig, len_new);

  width_orig = StringWidth(str_orig);
  cxChar = (Int2)ceil((float)width_orig/len_orig);
  nchars_row=(Int2)((float)winsize/cxChar) - 10;
  nchars_row -= nchars_row % 10;
  if (nchars_row <= 0)
    return;

  Fh = FontHeight();

  buf_orig = (CharPtr)MemNew(sizeof(Char)*(nchars_row + 1));
  buf_new = (CharPtr)MemNew(sizeof(Char)*(nchars_row + 1));

  sprintf(str1, "Translated Protein Lengths - Genomic: %d, mRNA: %d", len_new, len_orig);
  row += Ing_PrintText(str1, seg, row, winsize + margin, cl2, 0);
  row++;

  orig = str_orig;
  new = str_new;
  for (i = 0; i < len_min; i += nchars_row){
    AddAttribute(seg, COLOR_ATT, clr, 0,0,0,0);
    AddLabel(seg, 0, row * (-15), "Genomic - " , SMALL_TEXT, 0, UPPER_RIGHT, 0);
    AddLabel(seg, 0, (row + 1) * (-15), "mRNA    - " , SMALL_TEXT, 0, UPPER_RIGHT, 0);
    for (j = 0; (j < nchars_row) && (n < len_min + start); j++){
      if (*orig != *new){
        Ing_HighlightMismatch(seg, margin, j, row * (-15), cxChar, Fh);
      }
      if (!(Boolean)((j)% BLOCK_SIZE)){ 
        Ing_DrawScale(seg, margin, n, j, row * (-15), cxChar, Fh);
      }
      buf_new[j] = *new;
      buf_orig[j] = *orig;
      new++;
      orig++;
      n++;
    }
    buf_orig[j] = '\0';
    buf_new[j] = '\0';
    AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
    AddLabel(seg, margin, row * (-15), buf_new , SMALL_TEXT, 0, UPPER_RIGHT, 0);
    AddLabel(seg, margin, (row + 1) * (-15), buf_orig , SMALL_TEXT, 0, UPPER_RIGHT, 0);
    strw_new = StringWidth(buf_new);
    strw_orig = StringWidth(buf_orig);
    row += 4;
  }

  last_pos = j;
  if (n < len_new + start){
    if (last_pos < nchars_row)
      row -= 4;
    exit = len_new - len_min + last_pos;
    for (i = last_pos; i <= exit; i += nchars_row){
      AddAttribute(seg, COLOR_ATT, clr, 0,0,0,0);
      AddLabel(seg, 0, row * (-15), "Genomic - " , SMALL_TEXT, 0, UPPER_RIGHT, 0);
      for (k = last_pos, j = 0; (k < nchars_row) && (n < len_new + start); j++, k++){
        if (!(Boolean)(k % BLOCK_SIZE)){
          Ing_DrawScale(seg, margin, n, k, row * (-15), cxChar, Fh);
        }
        buf_new[j] = *new;
        new++;
        n++;
      }
      buf_new[j] = '\0';
      AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
      AddLabel(seg, margin + strw_new, row * (-15), buf_new , SMALL_TEXT, 0, UPPER_RIGHT, 0);
      row +=4;
      strw_new = last_pos = 1;
    }
  }
  else if (n < len_orig + start){
    if (last_pos < nchars_row)
      row -= 4;
    exit = len_orig - len_min + last_pos;
    for (i = 0; i <= exit; i += nchars_row){
      AddAttribute(seg, COLOR_ATT, clr, 0,0,0,0);
      AddLabel(seg, 0, (row + 1) * (-15), "mRNA    - " , SMALL_TEXT, 0, UPPER_RIGHT, 0);

      for (k = last_pos, j = 0; (k < nchars_row) && (n < len_orig + start); k++, j++){
        if (!(Boolean)(k % BLOCK_SIZE)){
          Ing_DrawScale(seg, margin, n, k, row * (-15), cxChar, Fh);
        }
        buf_orig[j] = *orig;
        orig++;
        n++;
      }
      buf_orig[j] = '\0';
      AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
      AddLabel(seg, margin + strw_orig, (row + 1) * (-15), buf_orig , SMALL_TEXT, 0, UPPER_RIGHT, 0);
      row +=4;
      strw_orig = last_pos = 0;
    }
  }
  row -= 2;
  aap->rowNum = row;
}

static void Ing_PropagateReport(IngAnnotAlignPtr aap, IngProgFeatPtr pfp)
{
  PrimitivE  prim;
  Int4       left = 10;
  Int4       row;
  Int4       stringwidth = 0;
  Int2       winsize, top;
  Char       message[255]={""}, str[255] = {""};
  Uint1      ARROW=NO_ARROW;
  IngGenomeViewerPtr igvp = NULL;
  Uint1      gray[3];
  Uint1      red[3], green[3], dkblue[3];
  Uint1Ptr   title_clr = NULL;
  Boolean    print_translation = FALSE;
  RecT       rc;
  enumPrimAddOrder   oldOrder;


  row = aap->rowNum;

  gray[0] = 100;
  gray[1] = 100;
  gray[2] = 100;
  
  dkblue[0] = 0;
  dkblue[1] = 64;
  dkblue[2] = 194;

  green[0] = 0;
  green[1] = 200;
  green[2] = 12;

  red[0] = 224;
  red[1] = 0;
  red[2] = 64;

  GetPosition(aap->viewer, &rc);
  winsize = (rc.right - rc.left) - 3*stdCharWidth;
  
  if (pfp->strand == Seq_strand_minus)
    ARROW = LEFT_ARROW;
  else
    ARROW = RIGHT_ARROW;

  igvp=(IngGenomeViewerPtr)GetObjectExtra((WindoW)ParentWindow(aap->viewer));
  if (!igvp) return;

  if (pfp->status == _success_){
    if (pfp->prot_ok){
      title_clr = green;
      sprintf(message, "O.K.");
    }
    else {
      title_clr = red;
      sprintf(message, "WARNING -- unclassified translation discrepancy -- check CDS coordinates");
      print_translation = TRUE;
    }
  }
  else if (pfp->status == _already_exists_ ){
    title_clr = BLUE_COLOR;
    sprintf(message, "Already Exists");
  }
  else if (pfp->status == _failed_to_map_ ){
    title_clr = gray;
    sprintf(message, "Failed to map");
  }
  else {
    title_clr = BLACK_COLOR;
    sprintf(message, "??");
  }
  
  top = row*(-15) + 20;
  oldOrder = ChangeAddPrimOrder(ADD_TO_HEAD);
  row += Ing_PrintText(message, aap->seg, row, winsize, WHITE_COLOR, 0);
  AddAttribute(aap->seg, COLOR_ATT, title_clr , 0, 0, 0, 0);
  prim= AddRectangle(aap->seg, 0, top, MIN(StringWidth(message) + 2, winsize), (row - 1)*(-15) + 5, NO_ARROW, TRUE, 0);

  ChangeAddPrimOrder(oldOrder);

  sprintf(str, "%s", pfp->name);
  row += Ing_PrintText(str, aap->seg, row, winsize, BLACK_COLOR, 0);

  if (pfp->new_loc) {
    sprintf(str, "Genomic: %s", pfp->new_loc);
    row += Ing_PrintText(str, aap->seg, row, winsize, green, 0);
  }
  sprintf(str, "mRNA: %s", pfp->orig_loc);
  row += Ing_PrintText(str, aap->seg, row, winsize, dkblue, 0);

  if (print_translation && (!StringHasNoText(pfp->str_prot)) && (!StringHasNoText(pfp->str_new))){
    aap->rowNum = row;
    Ing_PrintCompProteins(aap, pfp);
    aap->rowNum += 3;
  }
  else
    aap->rowNum = row + 3;

}

static void Ing_PopulateProgWin(IngAnnotAlignPtr aap){
  IngProgFeatPtr pfp;

  pfp = aap->pfp;
  aap->rowNum = 0;
  Reset(aap->viewer);
  DeletePicture(aap->seg);
  Update();
  aap->seg=CreatePicture();

  while (pfp != NULL){
    Ing_PropagateReport(aap, pfp);
    pfp = pfp->next;
  }

  AttachPicture (aap->viewer, aap->seg, INT4_MIN, INT4_MAX, UPPER_RIGHT, 1 , 1, NULL);
  SetViewerProcs(aap->viewer, Ing_PropagateClickProc,NULL,NULL, NULL);
}

static void Ing_ResizePropagateWin(WindoW w)
{
  IngAnnotAlignPtr aap = NULL;
  RecT     rcW, rcR, rcVsb, rcHsb;
  Int2     height,width,gap,vsbWidth;
  Int2     hsbWidth,in;
  BaR      vsb, hsb;


  aap = (IngAnnotAlignPtr)GetObjectExtra(w);
  if (!aap) return;
  ObjectRect(aap->w, &rcW);
  width= rcW.right-rcW.left;
  height= rcW.bottom-rcW.top;
  SafeHide(aap->viewer);
  Update();

  vsb = GetSlateVScrollBar ((SlatE) aap->viewer);
  hsb = GetSlateHScrollBar ((SlatE) aap->viewer);
  GetPosition(aap->viewer, &rcR);
  GetPosition(vsb, &rcVsb);
  GetPosition(hsb, &rcHsb);
  vsbWidth=rcVsb.right-rcVsb.left;
  hsbWidth=rcHsb.right-rcHsb.left;

  gap=10;
  in=2;

  rcR.right=rcR.left+width-in-vsbWidth;
  rcR.bottom=rcR.top+height-in-gap-hsbWidth;
  SetPosition(aap->viewer, &rcR);

  Ing_PopulateProgWin(aap);

  SafeShow(aap->viewer);
  Update();
}

static SeqLocPtr Ing_MapLocForProp(SeqAlignPtr salp, Boolean gap_split, SeqLocPtr location, Uint1 strand)
{
  
  SeqLocPtr    head = NULL;
  IngIvalInfoPtr  ival;
  IngIvalInfoPtr  ival_head;
  ValNodePtr   vnp_cur = NULL;
  SeqIntPtr    sinp;
  SeqLocPtr    slp;
  SeqPntPtr    spp;
  SeqIdPtr     sip = NULL;
  Int4         from_row = 2, to_row = 1;
  Int4         start, stop;
  
  sip = AlnMgr2GetNthSeqIdPtr(salp, 1);
  slp = SeqLocFindNext (location, NULL);

  while (slp != NULL) {
    switch (slp->choice) {
    case SEQLOC_INT :
      sinp = (SeqIntPtr) slp->data.ptrvalue;
      if (sinp != NULL) {
/*         strand = sinp->strand; */
        ival_head = (IngIvalInfoPtr)Ing_GetAlignmentIntervals (salp, from_row, to_row, sinp->from, sinp->to, strand);
        if (!ival_head){
          if (head)
            SeqLocFree(head);
          return NULL;
        }
        ival_head = MergeAdjacentIntervals (ival_head);
          /* what if one or the other interval maps into a gap? */

        if (gap_split) {
          for (ival = ival_head; ival != NULL; ival = ival->next) {
            sinp = SeqIntNew ();
            sinp->from = ival->start2;
            sinp->to = ival->stop2;
            sinp->strand = strand;
            sinp->id = SeqIdDup (sip);
            if (head == NULL)
              vnp_cur = ValNodeAddPointer (&head, SEQLOC_INT, (Pointer) sinp);
            else
              vnp_cur = ValNodeAddPointer (&vnp_cur, SEQLOC_INT, (Pointer) sinp);
          }
        } else {
          sinp = SeqIntNew ();
          sinp->from = ival_head->start2;
          for (ival = ival_head; ival->next != NULL; ival = ival->next) continue;
          sinp->to = ival->stop2;
          sinp->strand = strand;
          sinp->id = SeqIdDup (sip);
          if (head == NULL)
            vnp_cur = ValNodeAddPointer (&head, SEQLOC_INT, (Pointer) sinp);
          else
            vnp_cur = ValNodeAddPointer (&vnp_cur, SEQLOC_INT, (Pointer) sinp);
        }
        Ing_IvalInfoFree (ival_head);
        
      }
      break;
    case SEQLOC_PNT :
      spp = (SeqPntPtr) slp->data.ptrvalue;
      if (spp != NULL) {
/*         strand = spp->strand; */
        ival_head = (IngIvalInfoPtr)Ing_GetAlignmentIntervals (salp, from_row, to_row, spp->point, spp->point, strand);
        if (!ival_head){
          if (head)
            SeqLocFree(head);
          return NULL;
        }
        
        spp = SeqPntNew ();
        spp->point = ival_head->start2;
        spp->strand = strand;
        spp->id = SeqIdDup (sip);
        if (head == NULL)
          vnp_cur = ValNodeAddPointer (&head, SEQLOC_PNT, (Pointer) spp);
        else 
          vnp_cur = ValNodeAddPointer (&vnp_cur, SEQLOC_PNT, (Pointer) spp);
        Ing_IvalInfoFree (ival_head);
        
      }
      break;
    case SEQLOC_PACKED_PNT :
      /* not yet implemented */
      break;
    default :
      break;
    }

    slp = SeqLocFindNext (location, slp);
  }

  if (head == NULL) return NULL;
  if (head->next == NULL) return head;

  slp = ValNodeNew (NULL);
  slp->choice = SEQLOC_MIX;
  slp->data.ptrvalue = (Pointer) head;
  slp->next = NULL;

  return slp;


}

static Boolean Ing_KeywordAlreadyInList (ValNodePtr head, CharPtr kwd)

{
  ValNodePtr  vnp;

  if (head == NULL || kwd == NULL) return FALSE;

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    if (StringICmp ((CharPtr) vnp->data.ptrvalue, kwd) == 0) return TRUE;
  }

  return FALSE;
}


static IngProgFeatPtr Ing_AddProgFeat(IngProgFeatPtr head, IngProgFeatPtr pfp)
{
  IngProgFeatPtr tmp;

  if (head == NULL)
    return pfp;

  tmp = head;
  while (tmp->next != NULL)
    tmp = tmp->next;
  tmp->next = pfp;

  return head;
}

static IngAnnotAlignPtr Ing_AnnotAlignFree(IngAnnotAlignPtr aap)
{
  IngProgFeatPtr pfp_tmp = NULL, pfp = NULL;
  IngAnnotAlignPtr aap_tmp = NULL;

  if (!aap) return NULL;
  while (aap){
    pfp = aap->pfp;
    while (pfp){
      if (pfp->str_new) MemFree(pfp->str_new);
      if (pfp->str_prot) MemFree(pfp->str_prot);
      pfp_tmp = pfp;
      pfp = pfp->next;
      MemFree(pfp_tmp);
    }
    aap_tmp = aap;
    aap = aap->next;
    MemFree(aap_tmp);
  }
  return NULL;
}

static Boolean LIBCALLBACK Ing_DoPropagate(SeqFeatPtr sfp, SeqMgrFeatContextPtr sfc)
{
  BioseqPtr bsp_genome = NULL;
  IngAnnotAlignPtr aap = NULL;
  IngProgFeatPtr   pfp = NULL;
  SeqFeatPtr sfp_new = NULL;
  SeqFeatPtr sfp_genome = NULL;
  SeqFeatPtr temp = NULL;
  SeqLocPtr  slp = NULL;
  Char       strErr[50]={""};
  ByteStorePtr    bs=NULL;
  CharPtr         str_new = NULL, str_prot = NULL;
  CharPtr         str = NULL;
  Int4            len = 0;
  Uint1           strand_rel_to_genome;
  Char            str_buf[300]={""};
  Boolean         prot_ok = TRUE;
  Boolean         gap_split;
  SeqIdPtr        sip_prot = NULL;
  BioseqPtr       bsp_prot = NULL;
  MsgAnswer       ans;
  SeqMgrFeatContext context;


  aap = (IngAnnotAlignPtr)sfc->userdata;
  if (!aap) return (FALSE);
  sfp_genome = aap->sfp_mrna;
  bsp_genome = BioseqFindFromSeqLoc(sfp_genome->location);

  /* check if feature is already on NG */
  pfp = (IngProgFeatPtr)MemNew(sizeof(IngProgFeat));
  FeatDefLabel (sfp, pfp->name, sizeof (pfp->name) - 1, OM_LABEL_BOTH);
  pfp->type = sfc->featdeftype;
  pfp->strand = sfc->strand;
  pfp->orig_loc = SeqLocPrint(sfp->location);
  pfp->found = FALSE;
  SeqMgrExploreFeatures(bsp_genome, (Pointer)pfp, Ing_CheckGenFeats, NULL, NULL, mRNAFilter);

  if (pfp->found) {
    sprintf (str_buf, "%s already exists. Do you still want to propagate it?", pfp->name);
    ans = Message (MSG_YN, str_buf);
    if (ans == ANS_NO) {
      pfp->status = _already_exists_;
      pfp->prot_ok = TRUE;
      Ing_PropagateReport(aap, pfp); 
      aap->pfp = Ing_AddProgFeat(aap->pfp, pfp);
      return (TRUE);
    }
  }
  /* map location */
  if (aap->holes == FALSE) {
    slp = productLoc_to_locationLoc(sfp_genome, sfp->location);
  }
  else {
    if (GetValue(aap->gap_split) == 1)
      gap_split = TRUE;
    else
      gap_split = FALSE;
    strand_rel_to_genome = SeqLocStrand(sfp_genome->location);
    slp = Ing_MapLocForProp(aap->child, gap_split, sfp->location, strand_rel_to_genome);
  }
  if (!slp) {
    pfp->status = _failed_to_map_;
    pfp->prot_ok = TRUE;
    Ing_PropagateReport(aap, pfp);
    aap->pfp = Ing_AddProgFeat(aap->pfp, pfp);
    return (TRUE);
  }

  if (sfp->idx.subtype == FEATDEF_GENE)
    slp = SeqLocMerge(bsp_genome, slp, NULL, TRUE, FALSE, FALSE);


  IngfeatDefTrack[sfc->featdeftype] = 2;

  temp = AsnIoMemCopy(sfp, (AsnReadFunc) SeqFeatAsnRead, (AsnWriteFunc) SeqFeatAsnWrite);

  /* copy fields from temp to new genomic feature */
  sfp_new = CreateNewFeatureOnBioseq(bsp_genome, sfc->seqfeattype, NULL);
  sfp_new->location = SeqLocFree(sfp_new->location);
  sfp_new->location = slp;
  sfp_new->product = AsnIoMemCopy(sfp->product, 
                                  (AsnReadFunc) SeqLocAsnRead,  (AsnWriteFunc) SeqLocAsnWrite);

  sfp_new->xref = temp->xref;
  sfp_new->data = temp->data;
  sfp_new->id = temp->id;
  sfp_new->idx.subtype = sfp->idx.subtype;
  sfp_new->partial = temp->partial;
  sfp_new->excpt = temp->excpt;
  sfp_new->except_text = temp->except_text;
  sfp_new->exp_ev = temp->exp_ev;
  sfp_new->comment = temp->comment;
  sfp_new->qual = temp->qual;
  sfp_new->title = temp->title;
  sfp_new->ext = temp->ext;
  sfp_new->cit = temp->cit;
  sfp_new->exp_ev = temp->exp_ev;
  sfp_new->dbxref = temp->dbxref;
  sfp_new->xref = temp->xref;
  sfp_new->pseudo = temp->pseudo;
  sfp_new->except_text = temp->except_text;

  pfp->new_loc = SeqLocPrint(sfp_new->location);

  /* check translated product */
  if (sfp->idx.subtype == FEATDEF_CDS){
    bs = ProteinFromCdRegionEx(sfp_new, FALSE, TRUE);
    if (!bs) return (TRUE);
    str_new = (CharPtr)BSMerge(bs, NULL);
    BSFree(bs);
    sip_prot = SeqLocId(sfp_new->product);
    bsp_prot = BioseqFind(sip_prot);
    str_prot = (CharPtr)GetSequenceByBsp(bsp_prot);

    prot_ok = FALSE;

    if (str_prot == NULL || str_new == NULL){
      if (str_prot == NULL)
        Message (MSG_OK, "Failed to find a protein sequence associated with the mRNA record, cannot check if the protein sequence matches the translated coding region."); 
      else
        Message (MSG_OK, "Failed to translate the new coding region."); 
      prot_ok = FALSE;
    }
    else if ((StringCmp(str_new, str_prot) == 0))
      prot_ok = TRUE;


    if (!prot_ok){
      if (sfp_new->excpt && !StringHasNoText(sfp_new->except_text)){
        len = StringWidth(sfp_new->except_text) + StringWidth("unclassified translation discrepancy");
        str = (CharPtr) MemNew(sizeof(Char)*len);
        sprintf(str, "%s, unclassified translation discrepancy", sfp_new->except_text);
        MemFree(sfp_new->except_text);
        sfp_new->except_text = str;
      }
      else {
        sfp_new->excpt = TRUE;
        sfp_new->except_text = StringSave("unclassified translation discrepancy");
      }
    }
    pfp->str_prot = str_prot;
    pfp->str_new = str_new;
  }

  MemFree(temp); /* do not SeqFeatFree */
  pfp->status = _success_;
  pfp->prot_ok = prot_ok;
  Ing_PropagateReport(aap, pfp);
  aap->pfp = Ing_AddProgFeat(aap->pfp, pfp);

  return (TRUE);
}

static void Ing_ClosePropagateWin(WindoW w)
{
  IngAnnotAlignPtr aap = NULL;

  aap = (IngAnnotAlignPtr)GetObjectExtra(w);
  aap = Ing_AnnotAlignFree(aap);

  Remove(w);
}

static void Ing_PropagateFeatures(BioseqPtr bsp_genome, IngAnnotAlignPtr aap)
{
  SegmenT       seg;

  aap->viewer = CreateViewer(aap->w, 500 ,150, TRUE, TRUE);
  aap->seg = CreatePicture();
  aap->rowNum = 0;
  aap->pfp = NULL;
  SeqMgrExploreFeatures(aap->bsp_mrna, (Pointer)aap, Ing_DoPropagate, NULL, NULL, mRNAFilter);
  AttachPicture (aap->viewer, aap->seg, INT4_MIN, INT4_MAX, UPPER_RIGHT, 1 , 1, NULL);
  SetViewerProcs(aap->viewer, Ing_PropagateClickProc,NULL,NULL, NULL);
  RealizeWindow(aap->w);

  Ing_ResizePropagateWin(aap->w);
  Show(aap->w);
  Update();
}

static void Ing_AnnotateAlignmentCancel(ButtoN b)
{
  Remove(ParentWindow(b));
}


static void Ing_mRNATrackTomRNAFilter(IngAnnotAlignPtr aap)
{
  Int4  i;
 
  for(i=0; i<aap->nList; i++){
    if (GetItemStatus(aap->feat_list, i+1)){
      mRNAFilter[mRNALookup[i]]=TRUE;
    }
    else {
      mRNAFilter[mRNALookup[i]]=FALSE;
    }
  }
}


static Boolean Ing_IsSpideyAlign(IngTrackAlignsPtr tap, SeqAlignPtr sap)
{
  Int4 i = 0;
  SeqAlignPtr sap_list = NULL;
  ValNodePtr  vnp = NULL;

  vnp = tap->showlist;
  while (vnp){
    sap_list = (SeqAlignPtr) vnp->data.ptrvalue;
    if (sap_list == sap){
      if (StringCmp(tap->namelist[i], "Spidey")==0)
        return TRUE;
      else 
        return FALSE;
    }
    vnp = vnp->next;
    i++;
  }
  return FALSE;
}


static void Ing_QuitPropagateWin(IteM i)
{
  
  Ing_ClosePropagateWin(ParentWindow(i));

}

static void Ing_AnnotateAlignmentAccept (ButtoN b)
{
  IngGenomeViewerPtr igvp = NULL;
  Int2               retval;
  SelStructPtr       ssp = NULL;
  SeqLocPtr          slp_list = NULL;
  SeqAlignPtr        child=NULL, parent=NULL;
  SeqAlignPtr        salp_prev=NULL, salp_head=NULL;
  SeqAlignPtr        salp=NULL;
  SeqFeatPtr         sfp_mrna = NULL;
  BioseqPtr          product = NULL;
  Boolean            found=FALSE, Only_sap=FALSE;
  SeqEntryPtr        sep = NULL;
  IngAnnotAlignPtr   aap = NULL;
  Int4               value, i;
  Uint2              itemID;
  WindoW             w;
  Uint1              seqfeattype=0;
  Uint1              featdeftype= 0;
  IngMarkData        marked;
  OMProcControl      ompc;
  Boolean            gap_split;
  MsgAnswer          ans;
  MenU               m;
  ChoicE             cg;


  WatchCursor();
  w=ParentWindow(b);
  igvp = (IngGenomeViewerPtr) GetObjectExtra (w);

  aap = (IngAnnotAlignPtr)GetObjectExtra(b);
  if (aap->nList > 0){
    aap->w = DocumentWindow(-50, -90 , 550, 250, "Feature Propagation Report", Ing_ClosePropagateWin, Ing_ResizePropagateWin);
    SetObjectExtra(aap->w, (Pointer)aap, NULL);
  }
  else 
    aap->w = NULL;
 
  m = PulldownMenu(aap->w, "Window");
  CommandItem(m, "Quit", Ing_QuitPropagateWin);


  product = aap->bsp_mrna;

  if (igvp==NULL || aap==NULL) goto end;
  Hide(w);
  value=GetValue(aap->feat_type);
  if (value == 1){
    seqfeattype = SEQFEAT_RNA;
    featdeftype = FEATDEF_mRNA;
  }
  else if (value==2){
    seqfeattype=SEQFEAT_CDREGION;
    featdeftype = FEATDEF_CDS;
  }
  else if (value==3){
    seqfeattype=SEQFEAT_GENE;
    featdeftype = FEATDEF_GENE;
  }

  ssp = ObjMgrGetSelected();

  while (ssp){
    if (ssp == NULL || (ssp->itemtype!=0 && ssp->itemtype != OBJ_SEQALIGN)){
      goto next;
    }
    child = (SeqAlignPtr)GetPointerForIDs(ssp->entityID, ssp->itemID, OBJ_SEQALIGN);
    if (!child) 
      goto next;

    parent=AlnMgr2GetParent(child);

    if (!dblClick){
      ObjMgrDeSelect(ssp->entityID, ssp->itemID, ssp->itemtype, 0, NULL);
    }

    /* make seqlocptrs and create new feature */
    if (GetValue(aap->gap_split) == 1)
      gap_split = TRUE;
    else
      gap_split = FALSE;

    if (!igvp->isExons  && 
        Ing_IsSpideyAlign(&igvp->talign, parent) && igvp->data){
      slp_list = Ing_MakeLocListFromSpidp(igvp->data, parent, gap_split);
      if (!slp_list)
        slp_list = Ing_MakeLocListFromSAP(child, gap_split);
    }
    else
      slp_list = Ing_MakeLocListFromSAP(child, gap_split);

    if (!slp_list) goto next;
    sfp_mrna = Ing_CreateNewFeature(igvp->entityID, igvp->itemID, product, seqfeattype, featdeftype, NULL, slp_list, NULL); 
    
    /* copy features from mRNA record */
    Ing_mRNATrackTomRNAFilter(aap);
    if (GetStatus(aap->propc) && aap->w){
      if (igvp->data != NULL)
        aap->holes = Ing_DoesAlignmentCoverAll(igvp->data);
      aap->child = child;
      aap->sfp_mrna = sfp_mrna;
      Ing_PropagateFeatures(igvp->bsp, aap); 
    }

    /* delete child alignment and reindex parent */
    ans = Message (MSG_YN, "Do you want to delete the alignment?");
    if (ans == ANS_YES) {
      if (parent){
        salp=(SeqAlignPtr)parent->segs;
        while (salp && !found)
          {
            if (salp == child)
              found = TRUE;
            else
              {
                if (salp_head)
                  salp_prev = salp;
                else
                salp_head = salp_prev = salp;
                salp = salp->next;
              }
          }
        if (!found)
          goto next;
        if (salp_head != NULL)
          {
            salp_prev->next = salp->next;
            salp->next = NULL;
            SeqAlignFree(salp);
          } else
            {
              if (salp->next==NULL)
                Only_sap=TRUE;
              else {
                salp_head = salp->next;
                salp->next = NULL;
                SeqAlignFree(salp);
              }
            }
        if (!Only_sap){
          parent->segs = (Pointer)salp_head;
          if (parent->saip != NULL)
            AMAlignIndex2Free2(parent);
          parent->saip=NULL;
          AlnMgr2IndexLite(parent); 
          marked.parent = parent;
          marked.idx = 0;
          marked.bSpidey = FALSE;
          sep = GetTopSeqEntryForEntityID(igvp->entityID); 
          SeqEntryExplore(sep,(Pointer)&marked, is_parent_spidey);
          if (marked.bSpidey)
            Disable(igvp->d_Win->mrnaorexons);
        }
        else{
          marked.parent = parent;
          marked.idx = 0;
          marked.bSpidey = FALSE;
          sep = GetTopSeqEntryForEntityID(igvp->entityID); 
          SeqEntryExplore(sep,(Pointer)&marked, Ing_MarkSeqAnnot);
          DeleteMarkedObjects(igvp->entityID, OBJ_SEQENTRY, (Pointer)sep);
          if (marked.bSpidey){
            Disable(igvp->d_Win->mrnaorexons);
            Ing_DeleteSpidey(igvp, marked.idx);
          }
        }
      }
      else {
        marked.parent = parent;
        marked.idx = 0;
        marked.bSpidey = FALSE;
        sep = GetTopSeqEntryForEntityID(igvp->entityID); 
        SeqEntryExplore(sep,(Pointer)&marked, Ing_MarkSeqAnnot);
        DeleteMarkedObjects(igvp->entityID, OBJ_SEQENTRY, (Pointer)sep);
        if (marked.bSpidey){
          Disable(igvp->d_Win->mrnaorexons);
          Ing_DeleteSpidey(igvp, marked.idx);
        }
      }
      Ing_InitAlignArrays(&igvp->talign, FALSE);
    }

    SeqMgrIndexFeatures(igvp->entityID, NULL);
    itemID=GetItemIDGivenPointer(igvp->entityID, OBJ_SEQFEAT, (Pointer)sfp_mrna);       
    GatherProcLaunch (OMPROC_EDIT, FALSE, igvp->entityID, itemID, OBJ_SEQFEAT, 0, 0, OBJ_SEQFEAT, 0);

    ObjMgrSetDirtyFlag (igvp->entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, igvp->entityID, 0, 0);
/*     BioseqUnlock(aap->bsp_mrna); */
  next:
    ssp = ssp->next;
/*     aap_old = aap; */
/*     aap = aap->next; */
/*     MemFree(aap_old); */
  }

 end:
  Remove(w);
  ArrowCursor();
  return;
}


static Boolean LIBCALLBACK Ing_SetToPropagateArray(SeqFeatPtr sfp, SeqMgrFeatContextPtr sfc)
{
  mRNATrack[sfc->featdeftype] = TRUE;
  return TRUE;
}

static void Ing_SetFeatList(LisT feat_list, Int4 index)
{
  Int4  i;
 
  for(i=0; i < index; i++){
    SetItemStatus(feat_list, i+1, TRUE);
  }
}


static void Ing_AnnotateAlignmentForm(IteM item)
{
  IngGenomeViewerPtr igvp;
  GrouP              g, g2, g3, g4, g5;
  ButtoN             b1, b2;
  IngAnnotAlignPtr   aap = NULL;
  IngAnnotAlignPtr   aap_head = NULL;
  IngAnnotAlignPtr   aap_new = NULL;
  BioseqPtr          bsp_mrna = NULL;
  BioseqPtr          bsp_genome = NULL;
  SeqAlignPtr        salp = NULL;
  WindoW             w;
  Int4               i;
  SelStructPtr       ssp = NULL, temp_ssp = NULL;
  SeqIdPtr           sip = NULL;
  SeqLocPtr          slp = NULL;
  Char               str_mrna[100]={""}, buf_mrna[100]={""};
  Char               str_gen[100]={""}, buf_gen[100]={""};
  Boolean            bFirst = TRUE;
  Boolean            found = FALSE;


  ssp = ObjMgrGetSelected();
  if (!ssp || ssp->itemtype != OBJ_SEQALIGN) return;

  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(item));
  if (!igvp) return;
  bsp_genome = igvp->bsp;


  w = FixedWindow(-50, -33, -10, -10, "Make Feature From Align", StdCloseWindowProc);
  if (!w) return;
  SetObjectExtra(w, (Pointer)igvp, NULL);
  g=HiddenGroup(w, 1, -3, NULL);
  SetGroupMargins(g, 10, 10);
  SetGroupSpacing(g, 10, 20);

  g2 = NormalGroup (g, 1, 2,  "New Feature Type", systemFont,NULL);
    
  aap=(IngAnnotAlignPtr)MemNew(sizeof(IngAnnotAlign));
  aap_head = aap;
  aap->feat_type = HiddenGroup(g2, 3, 1, NULL);
  RadioButton (aap->feat_type, "mRNA");
  RadioButton (aap->feat_type, "CDS");
  RadioButton (aap->feat_type, "Gene");
  SetValue (aap->feat_type, 1);

  aap->gap_split = HiddenGroup(g2, 2, 1, NULL);
  RadioButton(aap->gap_split, "Split at gaps");
  RadioButton(aap->gap_split, "Extend over gaps");
  SetValue(aap->gap_split, 1);

  SetObjectExtra((Handle)aap->feat_type, (Pointer)aap_head, NULL);

  temp_ssp = ssp;
  i = 0;
  while (temp_ssp){
    if (temp_ssp->itemtype != 0 && temp_ssp->itemtype == OBJ_SEQALIGN)
      i++;
    temp_ssp = temp_ssp->next;
  }

  g3 = NormalGroup (g, 1, i, "Features To Propagate", systemFont, NULL);
  MemSet((Pointer)mRNAFilter, 1, sizeof(mRNAFilter));
  i = 0;
  while (ssp){
    if (ssp->itemtype == 0 || ssp->itemtype != OBJ_SEQALIGN)
      goto next;
    salp = (SeqAlignPtr)GetPointerForIDs(ssp->entityID, ssp->itemID, OBJ_SEQALIGN);
    sip = AlnMgr2GetNthSeqIdPtr(salp, 2);
    bsp_mrna = BioseqLockById (sip);
    if (!sip || !bsp_mrna)
      goto next;
    g4 = HiddenGroup(g3, 1, 2, NULL);

    g5 = HiddenGroup(g4, 1, 5, NULL);
    aap->propc = CheckBox(g5, "Propagate Features", NULL);
    SetStatus(aap->propc, TRUE);

    SeqIdWrite (bsp_mrna->id, str_mrna, PRINTID_REPORT, sizeof (str_mrna) - 1);
    sprintf(buf_mrna, "from: %s", str_mrna);
    MultiLinePrompt(g5, buf_mrna, 350, programFont);

    SeqIdWrite (bsp_genome->id, str_gen, PRINTID_REPORT, sizeof(str_gen) - 1);
    sprintf(buf_gen, "to: %s", str_gen);
    MultiLinePrompt(g5, buf_gen, 350, programFont);

    MemSet((Pointer)mRNATrack, FALSE, sizeof(mRNATrack));
    MemSet((Pointer)mRNALookup, 0, sizeof(mRNALookup));
    if (!salp->bounds)
      SeqMgrExploreFeatures (bsp_mrna, NULL, Ing_SetToPropagateArray, NULL, NULL, mRNAFilter);
    else
      while ((slp = SeqLocFindNext(salp->bounds, slp)) != NULL)
        SeqMgrExploreFeatures (bsp_mrna, NULL, Ing_SetToPropagateArray, slp, NULL, mRNAFilter);

    if (!bFirst){
      aap_new = (IngAnnotAlignPtr)MemNew(sizeof(IngAnnotAlign));
      aap_new->feat_type = NULL;
      aap->next = aap_new;
      aap = aap_new;
    }
    else
      bFirst = FALSE;

    aap->bsp_mrna = bsp_mrna;
    aap->nList=0;
    aap->feat_list = MultiList(g4, 20, 6, NULL);
    for (i=0; i<FEATDEF_MAX; i++){
      if (mRNATrack[i]){
        ListItem(aap->feat_list, IngfeatDefLabels[i]);
        mRNALookup[aap->nList]=i;
        aap->nList++;
        found = TRUE;
      }
    }
    if (found)
      Ing_SetFeatList(aap->feat_list, aap->nList);
    else
      Disable(g3);
  next:
    ssp = ssp->next;
  }
  
  g = HiddenGroup(g, 2, 1, NULL);
  b1 =DefaultButton(g, "Accept", Ing_AnnotateAlignmentAccept);
  Enable(b1);
  b2 =PushButton(g, "Cancel", Ing_AnnotateAlignmentCancel);
  SetObjectExtra(b1, (Pointer)aap_head, NULL);
  SetObjectExtra(b2, (Pointer)aap_head, NULL);
  Select(w);
  Show(w);
}

/*******************************************************************************

  Function : Ing_ViewFlatFile() 
  
  Purpose :  view record as flat file

*******************************************************************************/

static void Ing_ViewFlatFile(IteM i)
{
  SeqEntryPtr sep = NULL;
  IngGenomeViewerPtr igvp = NULL;
  Char         path [PATH_MAX];
  FILE        *fp = NULL;
  Boolean      ok;
  ErrSev       level;

  igvp = (IngGenomeViewerPtr)GetObjectExtra(ParentWindow(i));
  sep = GetTopSeqEntryForEntityID(igvp->entityID);
  if (!sep) return;
  TmpNam(path);
  fp = FileOpen(path, "w");
  if (!fp) return;
  WatchCursor();
  Update();
  level = ErrSetMessageLevel (SEV_MAX);
  ok = SeqEntryToFlat(sep, fp, /* GENBANK_FMT */(Uint1) 0, /* SEQUIN_MODE */(Uint1) 8);
  ErrSetMessageLevel (level);
  FileClose(fp);

  if (ok){
    LaunchGeneralTextViewer (path, igvp->title);
    FileRemove (path);
  }
  else 
    FileRemove (path);

  ArrowCursor();
  Update();
}


/*******************************************************************************

  Function : Ing_NewFeatureMenuProc(), Ing_SetupNewFeaturesMenu(), Ing_FeatureFilter() 
  
  Purpose :  create a new feature

*******************************************************************************/

static void Ing_NewFeatureMenuProc (IteM i)

{
  MsgAnswer      ans;
  IngGenomeViewerPtr igvp;
  IngObjectPtr   nop;
  OMProcControl  ompc;
  ObjMgrProcPtr  ompp;
  Int2           retval;

  nop = (IngObjectPtr) GetObjectExtra (i);
  if (nop == NULL) return;

  igvp = nop->igvp;
  if (igvp == NULL) return;
  ompp = nop->ompp;
  if (ompp == NULL || ompp->func == NULL) return;

  MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
  ompc.input_entityID = igvp->entityID;
  ompc.input_itemID = igvp->itemID;
  ompc.input_itemtype = igvp->itemType;
  GatherDataForProc (&ompc, FALSE);
  ompc.proc = ompp;
  retval = (*(ompp->func)) (&ompc);
  if (retval == OM_MSG_RET_ERROR) {
    ErrShow ();
  }
}


static Boolean Ing_FeatureFilter(Uint1 featdeftype)
{
  if (featdeftype != FEATDEF_COMMENT && featdeftype!=FEATDEF_BIOSRC && featdeftype!=FEATDEF_PUB && featdeftype!=FEATDEF_ORG)
    return TRUE;
  return FALSE;

}

static void Ing_SetupNewFeaturesMenu(MenU m, IngGenomeViewerPtr igvp)
{
  FeatDispGroupPtr  fdgp;
  FeatDefPtr        fdp;
  IngObjectPtr      first;
  IteM              i;
  Uint1             key;
  CharPtr           label;
  IngObjectPtr      last;
  IngObjectPtr      nop;
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
          if (fdgp->groupkey != 0 && key!=4) {
            sub = SubMenu (m, fdgp->groupname);
            fdp = NULL;
            label = NULL;
            while ((fdp = FeatDefFindNext (fdp, &key, &label,fdgp->groupkey, FALSE)) != NULL) {
              if (Ing_FeatureFilter(key)/* key != FEATDEF_BAD */) {
                ompp = NULL;
                while ((ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, omtp->datatype, 0, ompp)) != NULL) {
                  if (ompp->subinputtype == fdp->featdef_key && Ing_FeatureFilter(ompp->subinputtype)/* ompp->subinputtype != FEATDEF_PUB */) {
                    i = CommandItem (sub, ompp->proclabel, Ing_NewFeatureMenuProc);
                    nop = (IngObjectPtr) MemNew (sizeof (IngObjectData));
                    if (nop != NULL) {
                      nop->kind = 1; /* feature creation item */
                      nop->ompp = ompp;
                      nop->igvp = igvp;
                      nop->item = i;
                      nop->molgroup = fdp->molgroup;
                    }
                    if (first == NULL) {
                      first = nop;
                    }
                    if (last != NULL) {
                      last->next = nop;
                    }
                    last = nop;
                    nop->igvp=igvp;
                    SetObjectExtra (i, (Pointer) nop, NULL);
                  }
                }
              }
            }
          }
        }

      }
    }
  }
}


/*******************************************************************************

  Function : Ing_CloseWin(), Ing_CloseProc(), Ing_QuitProc()
  
  Purpose : Close window (program is still running) or quit program 

*******************************************************************************/

static void Ing_CloseWin(WindoW w)
{
  IngGenomeViewerPtr igvp=NULL;

  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  ObjMgrSendMsg(OM_MSG_FLUSH, igvp->entityID, igvp->itemID, OBJ_BIOSEQ);

}

static void Ing_CloseProc(IteM i)
{
  Ing_CloseWin(ParentWindow(i));
}

static void Ing_QuitProc(IteM i)
{
  IngGenomeViewerPtr igvp;
  WindoW             w;

  w=ParentWindow(i);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  if (igvp->bspIsLocked)
    BioseqUnlock(igvp->bsp);
  Ing_MainDataFree(igvp);
  Remove(w);
  QuitProgram();
}


/*******************************************************************************

  Function : Ing_DoSave(), Ing_SaveProc() 
  
  Purpose : saving functions

*******************************************************************************/
static void Ing_DoSave(ButtoN b)
{
  BioseqPtr   bsp;
  SeqEntryPtr sep;
  AsnIoPtr    aip;
  Char        filename[21];
  WindoW      w;
  IngSaveInfoPtr igsi;


  w=ParentWindow(b);
  igsi=(IngSaveInfoPtr)GetObjectExtra(b); 
  bsp=(BioseqPtr)GetObjectExtra(w);
  GetTitle(igsi->text, filename, 20);
  sep=GetTopSeqEntryForEntityID(ObjMgrGetEntityIDForPointer(bsp));
  aip = AsnIoOpen(filename, "w");
  SeqEntryAsnWrite(sep, aip, NULL);
  aip = AsnIoClose(aip);

  Remove(w);

}

static void Ing_SaveProc(IteM i)
{
  IngGenomeViewerPtr igvp;
  WindoW             w;
  GrouP              g, g1, g3;
  ButtoN             b;
  IngSaveInfoPtr      igsi;

  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(i));
  igsi =(IngSaveInfoPtr)MemNew(sizeof(IngSaveInfo));

  w = FixedWindow(-50, -33, -10, -10,"SAVE - ASN.1 format", StdCloseWindowProc);
  g = HiddenGroup(w, 1, 3, NULL);
  g1 = HiddenGroup(g, 1, 2, NULL);
  StaticPrompt(g1, "File name:  ", 0, 0, systemFont, 'l');
  igsi->text = DialogText(g1, "UNTITLED", 20, NULL);
  g3= HiddenGroup(g, 2, 1, NULL);
  b = DefaultButton(g3, "Save", Ing_DoSave);
  SetObjectExtra(b, (Pointer)igsi, StdCleanupExtraProc);
  Enable(b);
  PushButton(g3, "Cancel", Ing_GenericCancelButton);
  SetObjectExtra(w, (Pointer)igvp->bsp, NULL);
  Select(w);
  Show(w);
  return;
}

/*******************************************************************************

  Function : Ing_PrintProc(), Ing_DoPrint()
  
  Purpose : print viewer

*******************************************************************************/
typedef struct ing_printinfo{
  ButtoN top;
  ButtoN bottom;
  ButtoN report;
  TexT   toptext;
  TexT   bottomtext;
  TexT   reporttext;
} IngPrintInfo, PNTR IngPrintInfoPtr;



static void Ing_DoPrint(ButtoN b)
{
  MsgAnswer  ans;
  WindoW     dlg;
  IngGenomeViewerPtr igvp;
  IngPrintInfoPtr pinfo = NULL;


  dlg = ParentWindow(b);
  Hide(dlg);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(dlg);
  pinfo = (IngPrintInfoPtr)GetObjectExtra(b);


  if (igvp == NULL || igvp->vTop == NULL ||
      igvp->vBottom == NULL || pinfo == NULL) return;

/* -- cannot create gif files from vibrant program --

#ifdef WIN_MOTIF 
{{

Boolean    print_top_failed = FALSE;
Boolean    print_bot_failed = FALSE;
Boolean    print_report_failed = FALSE;
FILE      *f = NULL;
Char       filename[21]={""};
IngReportPtr Report = NULL;
BoxInfo   pBox;
Int2       width = 700, height = 710;
SegmenT    pic = NULL;
gdImage*   im = NULL;
SetUpDrawingTools();


  Nlm_InitMuskStyles ();
  Nlm_SetMuskCurrentSt("style6");

  if (GetStatus(pinfo->top)){
    pic = igvp->pictTop;
    ASSERT(pic);
    SegmentBox (pic, &pBox);

    width  = (Int2)pBox.right - pBox.left;
    height = (Int2)pBox.top - pBox.bottom;
    GetTitle(pinfo->toptext, filename, 20);
    f = FileOpen(filename, "wb");
    ASSERT(f);

    im = gdImageCreate(width, height);
    if (!Nlm_Picture2gdImage (im, pic, -10, -10, LOWER_LEFT, 1, 1))
      print_top_failed = TRUE;
    gdImageGif(im, f);

    FileClose(f);
    f = NULL;
  }
  if (GetStatus(pinfo->bottom)){
    GetTitle(pinfo->bottomtext, filename, 20);
    pic = igvp->pictBottom;
    RecalculateSegment(pic, 1, 1);
    SegmentBox (pic, &pBox);

    f = FileOpen(filename, "wb");
    if (f){
      if (!Nlm_PictureToGIF(f, width, height, igvp->pictBottom, -10, -10, LOWER_LEFT, igvp->scaleX, 1, FALSE))
        print_bot_failed = TRUE;
    }
    else
      print_bot_failed = TRUE;
    FileClose(f);
    f = NULL;
  }
  if (GetStatus(pinfo->report)){
    Report = (IngReportPtr)GetObjectExtra(igvp->hReport);
    if (Report){
      pic = Report->Picture;
      RecalculateSegment(pic, 1, 1);
      SegmentBox (pic, &pBox);

      GetTitle(pinfo->reporttext, filename, 20);
      f = FileOpen(filename, "wb");
      if (f){
        if (!Nlm_PictureToGIF(f, width, height, pic, -10, -10, LOWER_LEFT, 1, 1, FALSE))
          print_report_failed = TRUE;
      }
      else 
        print_report_failed = TRUE;
      FileClose(f);
    }
  }
}}
  Remove(dlg);

  if (print_top_failed)
    Message (MSG_OK, "Printing overview (top viewer) failed."); 
  if (print_bot_failed)
    Message (MSG_OK, "Printing detailed view (bottom viewer) failed."); 
  if (print_report_failed)
    Message (MSG_OK, "Printing report window failed."); 

  Nlm_ExitMuskStyles();

*/

  Remove(dlg);
  ans = Message (MSG_YN, "Do you want to print just the visible area?");
  if (ans == ANS_YES) {
    PrintViewer (igvp->vTop);
    PrintViewer (igvp->vBottom);
  } else {
    PrintAllViewer(igvp->vTop);
    PrintAllViewer(igvp->vBottom);
  }

}

static void Ing_PrintProc(IteM i)
{
  IngGenomeViewerPtr igvp;
  WindoW             w;
  GrouP              g, g1, g2, g3;
  ButtoN             b;
  IngPrintInfoPtr    pinfo = NULL;
  IngReportPtr       Report = NULL;


  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(i));
  pinfo =(IngPrintInfoPtr)MemNew(sizeof(IngPrintInfo));
  if (igvp->hReport)
    Report = (IngReportPtr)GetObjectExtra(igvp->hReport);

  w = FixedWindow(-50, -33, -10, -10,"Print Window", StdCloseWindowProc);
  SetObjectExtra(w, (Pointer)igvp, NULL);
  g = HiddenGroup(w, 1, 3, NULL);
  SetGroupMargins(g, 10, 10);
  SetGroupSpacing(g, 10, 20); 


  g1 = HiddenGroup(g, 3, 2, NULL);

  pinfo->top = CheckBox(g1, "Overview (top)", NULL);
  SetStatus(pinfo->top, TRUE);
  StaticPrompt(g1, "Filename: ", 0, 0, systemFont, 'l');
  pinfo->toptext = DialogText(g1, "overview.gif", 20, NULL);
  pinfo->bottom = CheckBox(g1, "Detailed (bottom)", NULL);
  SetStatus(pinfo->bottom, TRUE);
  StaticPrompt(g1, "Filename: ", 0, 0, systemFont, 'l');
  pinfo->bottomtext = DialogText(g1, "detail.gif", 20, NULL);
  
  g2 = HiddenGroup(g, 1, 3, NULL);
  pinfo->report = CheckBox(g2, "Report", NULL);
  SetStatus(pinfo->report, TRUE);
  StaticPrompt(g2, "Filename: ", 0, 0, systemFont, 'l');
  pinfo->reporttext = DialogText(g2, "report.gif", 20, NULL);
  if (Report == NULL){
    SetStatus(pinfo->report, FALSE);
    Disable(g2);
  }

  g3= HiddenGroup(g, 2, 1, NULL);
  b = DefaultButton(g3, "Print", Ing_DoPrint);
  SetObjectExtra((Handle)b, pinfo, StdCleanupExtraProc);
  PushButton(g3, "Cancel", Ing_GenericCancelButton);
  Select(w);
  Show(w);
  return;
}

/*******************************************************************************

  Function : Ing_SetupMenus()
  
  Purpose : 

*******************************************************************************/

void Ing_SetupMenus ()

{
  MenU    m;  
#ifdef WIN_MAC
  m = AppleMenu (NULL);
  DeskAccGroup (m);
#endif

}


/*******************************************************************************

  Function : Ing_TargetAccept(), Ing_CreateTargetList(), Ing_ChangeTarget(), Ing_BspListProc(), Ing_LocalFileProc()
  
  Purpose : view a new sequence

*******************************************************************************/

static void Ing_TargetAccept(ButtoN b)
{
  Int4              i, value;
  IngGenomeViewerPtr igvp=NULL;
  BioseqPtr         bsp=NULL;
  SeqEntryPtr       sep=NULL;
  SeqIdPtr          sip=NULL;
  ValNode          *pvn=NULL;
  WindoW            hTarget;

  hTarget=ParentWindow(b);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(b);
  if (!igvp) return;
  Hide(hTarget);

  sep=GetTopSeqEntryForEntityID(igvp->entityID);
  value=GetValue(igvp->bsp_list);
    for(i = 1, pvn=igvp->entity_list->Sips; i <= value; i++, pvn=pvn->next)
      {
        if(i==value)
          {
            sip=(SeqId*)pvn->data.ptrvalue;
            bsp = BioseqFindInSeqEntry(sip, sep);
            igvp->cur_sip=sip;
            igvp->cur_target=value;
          }
      }

    if (bsp){
      Ing_RunIngenue(igvp, bsp, igvp->entityID);
      Remove(hTarget);
    }
    else {
      Select(hTarget);
      Show(hTarget);
    }
}


static void Ing_CreateTargetList(IngGenomeViewerPtr igvp)
{  
  ValNode             *pvn;
  Char                 szName[31]={""};
  ButtoN               b;
  WindoW               d;
  GrouP                h, h2;
  IngEntityPtr        e_list;

  if (!igvp) return;
  igvp->numseqs=0;
  d = MovableModalWindow(-50, -33 ,-10, -10, "Get Sequence from Memory", NULL);
  h = HiddenGroup(d, 1, 2,  NULL);
  igvp->bsp_list = SingleList(h,20,6,NULL);
  for (e_list=igvp->entity_list; e_list!=NULL; e_list=e_list->next){
    for(pvn = e_list->Sips; pvn != NULL; pvn = pvn->next) {
      SeqIdWrite((SeqId *)pvn->data.ptrvalue,szName,
                 PRINTID_FASTA_LONG,30);
      igvp->numseqs++;
      ListItem(igvp->bsp_list, szName);
    }
  }
  h2 = HiddenGroup(h, 2, 1, NULL);
  b= DefaultButton(h2, "Accept", Ing_TargetAccept);
  SetObjectExtra(b, (Pointer)igvp, NULL);
  Enable(b);
  PushButton(h2, "Cancel", Ing_GenericCancelButton);
  SetValue(igvp->bsp_list, igvp->cur_target);
  SetObjectExtra (d, (Pointer) igvp, NULL);
  RealizeWindow(d);
  Select(d);
  Show(d);
}


static void Ing_ChangeTarget(IteM i)
{
  IngGenomeViewerPtr    igvp;
  
  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(i));
  Ing_CreateTargetList(igvp);
}


static void Ing_BspListProc(IteM i)
{
  IngGenomeViewerPtr   igvp;

  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(i));
  Ing_CreateTargetList(igvp);
}

static void Ing_LocalFileProc(IteM i)
{
  IngGenomeViewerPtr igvp_new=NULL;

  if (!(igvp_new=Ing_MainDataNew()))
    return;
  Ing_FileOpenProc(igvp_new, ParentWindow(i)); 
}



/*******************************************************************************

  Function : Ing_CreateReOrderAlignForm(), Ing_AddAlignList(), Ing_RemoveAlignList(), Ing_AlignListAccept(), Ing_ResetAlignList(), Ing_UpdateNamelists(), Ing_RemoveNode(), Ing_AddNode()
  
  Purpose : change the order of alignments in display

*******************************************************************************/


typedef struct ing_seqform {
  LisT      feat_list;
  LisT      feat_list2;
  Int4      nList;
  Int4      nList2;
} IngseqForm, PNTR IngseqFormPtr;

static ValNodePtr vnp_left = NULL;
static ValNodePtr vnp_right = NULL;
static CharPtr PNTR  namelist_left = NULL;
static CharPtr PNTR  namelist_right = NULL;
static Int4  index1=0;
static Int4  index2=0;


static void Ing_ResetAlignList(IngseqFormPtr sformp)
{
  ValNodePtr vnp = NULL;
  Int4       i = 0;
  AMAlignIndex2Ptr   amaip = NULL;
  SeqAlignPtr       sap = NULL;
  SeqIdPtr          sip = NULL;
  Char              str[41] = {""}, buf[60]={""};
  
    Reset(sformp->feat_list);
    sformp->nList = 0;
    vnp = vnp_left;

    while (vnp){
      sap = (SeqAlignPtr)vnp->data.ptrvalue;
      amaip = (AMAlignIndex2Ptr) sap->saip;
      sip=AlnMgr2GetNthSeqIdPtr(amaip->saps[0], 2);
      SeqIdWrite(sip, str, PRINTID_FASTA_LONG, 41);
      sprintf(buf, "%s: ", namelist_left[i]);
      StringCat(buf, str);
      ListItem(sformp->feat_list, buf);
      sformp->nList++;
      vnp = vnp->next;
      i++;
    }
  
    Reset(sformp->feat_list2);
    sformp->nList2 = 0;
    vnp = vnp_right;
    i = 0;
    while(vnp){
      sap = (SeqAlignPtr)vnp->data.ptrvalue;
      amaip = (AMAlignIndex2Ptr) sap->saip;
      sip=AlnMgr2GetNthSeqIdPtr(amaip->saps[0], 2);
      SeqIdWrite(sip, str, PRINTID_FASTA_LONG, 41);
      sprintf(buf, "%s: ", namelist_right[i]);
      StringCat(buf, str);
      ListItem(sformp->feat_list2, buf);
      sformp->nList2++;
      vnp = vnp->next;
      i++;
    }
}


static void Ing_UpdateNamelists(Int2 list, Int4 index)
{
  Int4    i, j;
  CharPtr PNTR tmplist = NULL;
  

  if (list == 1){
    /* add name to left list */
    if (namelist_left == NULL){ /* index1 == 0 */
      namelist_left = (CharPtr PNTR)MemNew(sizeof(CharPtr));
      namelist_left[0] = namelist_right[index];
    }
    else {
      tmplist = (CharPtr PNTR)MemNew(sizeof(CharPtr)*index1);
      for (i = 0; i < index1-1; i++)
        tmplist[i] = namelist_left[i];
      tmplist[index1-1]=namelist_right[index];
      MemFree(namelist_left);
      namelist_left = tmplist;
    }

    /* remove name from right list */
    if (index2 == 0){
      MemFree(namelist_right);
      namelist_right = NULL;
    }
    else {
      tmplist = (CharPtr PNTR)MemNew(sizeof(CharPtr)*index2);
      for (i = 0, j = 0; i < index2; i++, j++){
        if (j == index2-1)
          j++;
        tmplist[i] = namelist_right[j];
      }
      MemFree(namelist_right);
      namelist_right = tmplist;
    }
  }
  else {
    /* add name to right list */
    if (namelist_right == NULL){ /* index2 == 0 */
      namelist_right = (CharPtr PNTR)MemNew(sizeof(CharPtr));
      namelist_right[0] = namelist_left[index];
    }
    else {
      tmplist = (CharPtr PNTR)MemNew(sizeof(CharPtr)*index2);
      for (i = 0; i < index2-1; i++)
        tmplist[i] = namelist_right[i];
      tmplist[index2-1]=namelist_left[index];
      MemFree(namelist_right);
      namelist_right = tmplist;
    }

    /* remove name from left list */
    if (index1 == 0){
      MemFree(namelist_left);
      namelist_left = NULL;
    }
    else {
      tmplist = (CharPtr PNTR)MemNew(sizeof(CharPtr)*index1);
      for (i = 0, j = 0; i < index1; i++, j++){
        if (j == index1-1)
          j++;
        tmplist[i] = namelist_left[j];
      }
      MemFree(namelist_left);
      namelist_left = tmplist;
    }
  }
}


static void Ing_RemoveNode (SeqAlignPtr salp, Int2 list)
{
  ValNodePtr vnp_cur = NULL;
  ValNodePtr vnp_prev = NULL;
  SeqAlignPtr salp2 = NULL;

  if (list == 1)
    vnp_cur = vnp_left;
  else 
    vnp_cur = vnp_right;

  while (vnp_cur){
    salp2 = (SeqAlignPtr)vnp_cur->data.ptrvalue;
    if (salp2 == salp){
      if (!vnp_prev){
        if (list == 1)
          vnp_left = vnp_cur->next;
        else 
          vnp_right = vnp_cur->next;
        vnp_cur->next = NULL;
        ValNodeFree(vnp_cur);
      }
      else {
        vnp_prev->next = vnp_cur->next;
        vnp_cur->next = NULL;
        ValNodeFree(vnp_cur);
      }
      return;
    }
    vnp_prev = vnp_cur;
    vnp_cur = vnp_cur->next;
  }
}

static void Ing_AddNode (SeqAlignPtr salp, Int2 list, Int4 i)
{

  Ing_UpdateNamelists(list, i);

  if (list == 1)
    ValNodeAddPointer(&vnp_left, OBJ_SEQALIGN, (Pointer)salp);
  else 
    ValNodeAddPointer(&vnp_right, OBJ_SEQALIGN, (Pointer)salp);
}

static void Ing_AddAlignList(ButtoN b)
{
  Int4 i = 0;
  Int4 j = 0;
  ValNodePtr    vnp = NULL;
  IngseqFormPtr sformp = NULL;
  SeqAlignPtr   sap = NULL;

  sformp = (IngseqFormPtr)GetObjectExtra(ParentWindow(b));
  if (!sformp) return;
  vnp = vnp_left;
  while (vnp){
    if (GetItemStatus(sformp->feat_list, i+1)){
      /* copy the contents of vnp1 to vnp2 */
      sap = (SeqAlignPtr)vnp->data.ptrvalue;
      index1--;
      index2++;
      vnp = vnp->next;
      Ing_AddNode(sap, 2, i-j);
      Ing_RemoveNode(sap, 1);
      i++;
      j++;
      continue;
    }
    i++;
    vnp = vnp->next;
  }

  Ing_ResetAlignList (sformp);
}


static void Ing_RemoveAlignList(ButtoN b)
{
  Int4 i = 0;
  Int4 j = 0;
  ValNodePtr vnp = NULL;
  IngseqFormPtr sformp = NULL;
  SeqAlignPtr   sap = NULL;

  sformp = (IngseqFormPtr)GetObjectExtra(ParentWindow(b));
  if (!sformp) return;
  vnp = vnp_right;
  while (vnp){
    if (GetItemStatus(sformp->feat_list2, i+1)){
      sap = (SeqAlignPtr)vnp->data.ptrvalue;
      index1++;
      index2--;
      vnp = vnp->next;
      Ing_AddNode(sap, 1, i-j);
      Ing_RemoveNode(sap , 2);
      i++;
      j++;
      continue;
    }
    i++;
    vnp = vnp->next;
  }

  Ing_ResetAlignList(sformp);

}

static void Ing_AlignListAccept(ButtoN b)
{
  IngGenomeViewerPtr igvp=NULL;
  IngseqFormPtr      sformp=NULL;
  WindoW             w;

  w=ParentWindow(b);
  sformp=(IngseqFormPtr)GetObjectExtra(w);
  igvp= (IngGenomeViewerPtr) GetObjectExtra (b);
  if (!sformp || !igvp) return;
  Hide (w);

  /* update global vars */
  Ing_InitAlignArrays(&igvp->talign, FALSE);

  igvp->talign.showlist = vnp_right;
  igvp->talign.namelist = namelist_right;
  igvp->talign.hidelist = vnp_left;
  igvp->talign.hnamelist = namelist_left;
  igvp->talign.hideindex = index1;
  igvp->talign.showindex = index2;

  Ing_PopulateOverviewPage((Pointer)igvp, FALSE);
  Ing_PopulateDetailedPage((Pointer)igvp);
  Remove(w);

}

static ValNodePtr Ing_ValNodeCopy (ValNodePtr vnp)
{
  ValNodePtr vnp_copy = NULL;
  ValNodePtr vnp_tmp = NULL;
  SeqAlignPtr sap = NULL;

  vnp_tmp = vnp;

  while(vnp_tmp){
    sap = (SeqAlignPtr)vnp_tmp->data.ptrvalue;
    ValNodeAddPointer(&vnp_copy, OBJ_SEQALIGN, sap);
    vnp_tmp = vnp_tmp->next;
  }

  return vnp_copy;
}

static CharPtr PNTR Ing_CharPtrCopy(CharPtr PNTR names, Int4 n)
{
  CharPtr PNTR names_copy = NULL;
  Int4         i;

  names_copy = (CharPtr PNTR) MemNew(sizeof(CharPtr)*n);
  for (i= 0; i<n; i++)
    names_copy[i] = names[i];

  return names_copy;
}


static void Ing_CreateReOrderAlignForm(IteM item)
{
  ButtoN               b;
  WindoW               d;
  GrouP                g;
  GrouP                h, h2;
  Int4                 i;
  IngseqFormPtr        sformp;
  IngGenomeViewerPtr   igvp=NULL;
  IngSeqAnnotData      sad;
  ValNodePtr           vnp = NULL;
  SeqIdPtr             sip = NULL;
  SeqAlignPtr          sap = NULL;
  AMAlignIndex2Ptr      amaip = NULL;
  Char                 buf[60]={""}, str[41]={""};
  

  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(item));
  if (!igvp) return;


  if (!igvp->talign.showlist && !igvp->talign.hidelist)  return;

  d = MovableModalWindow(-50, -33 ,-10, -10, "Change Alignment Order", NULL);
  g = HiddenGroup(d, 1, 2, NULL);
  h = HiddenGroup(g, 2, 2,  NULL);
  sformp=(IngseqFormPtr)MemNew(sizeof(IngseqForm));
  StaticPrompt (h, "Hide", 0, LineHeight() , systemFont, 'c');
  StaticPrompt (h, "Show", 0, LineHeight() , systemFont, 'c');

  sformp->feat_list = MultiList(h, 15, 10, NULL);
  sformp->nList=0;
  i = 0;
  vnp = igvp->talign.hidelist;
  while(vnp){
    sap = (SeqAlignPtr)vnp->data.ptrvalue;
    amaip = (AMAlignIndex2Ptr)sap->saip;
    sip=AlnMgr2GetNthSeqIdPtr(amaip->saps[0], 2);
    SeqIdWrite(sip, str, PRINTID_FASTA_LONG, 41);
    sprintf(buf, "%s", igvp->talign.hnamelist[i]);
    StringCat(buf, str);
    ListItem(sformp->feat_list, buf);
    sformp->nList++;
    vnp = vnp->next;
    i++;
  }
  index1 = i;
  vnp_left = Ing_ValNodeCopy(igvp->talign.hidelist);
  namelist_left = Ing_CharPtrCopy(igvp->talign.hnamelist, index1);

  /* get visible alignments if any */
  sformp->feat_list2 = MultiList(h, 15, 10, NULL);
  sformp->nList2 = 0;

  i = 0;
  vnp = igvp->talign.showlist;
  while (vnp){
    sap = (SeqAlignPtr)vnp->data.ptrvalue;
    amaip = (AMAlignIndex2Ptr)sap->saip;
    sip=AlnMgr2GetNthSeqIdPtr(amaip->saps[0], 2);
    SeqIdWrite(sip, str, PRINTID_FASTA_LONG, 41);
    sprintf(buf, "%s: ", igvp->talign.namelist[i]);
    StringCat(buf, str);
    ListItem(sformp->feat_list2, buf);
    sformp->nList2++;
    vnp = vnp->next;
    i++;
  }
  index2 = i;
  vnp_right = Ing_ValNodeCopy(igvp->talign.showlist);
  namelist_right = Ing_CharPtrCopy(igvp->talign.namelist, index2);

  h2 = HiddenGroup(g, 4, 1, NULL);
  b = PushButton(h2, "Add", Ing_AddAlignList);
  SetObjectExtra(b, (Pointer) igvp, NULL);

  b = PushButton(h2, "Remove", Ing_RemoveAlignList);
  SetObjectExtra(b, (Pointer) igvp, NULL);

  b= DefaultButton(h2, "Accept", Ing_AlignListAccept);
  SetObjectExtra(b, (Pointer)igvp, NULL);
  Enable(b);
  PushButton(h2, "Cancel", Ing_GenericCancelButton);
  SetObjectExtra (d, (Pointer) sformp, StdCleanupExtraProc);
  RealizeWindow(d);
  Select(d);
  Show(d);

}

/*******************************************************************************

  Function : Ing_SetShowList(), Ing_TrackToFilter(), Ing_ShowListAccept(), Ing_LabelsProc(), Ing_CreateShowListForm()
  
  Purpose : create a window to choose whether to show or hide specific features

*******************************************************************************/


static void Ing_SetShowList(LisT feat_list, Int4 nList)
{
  Int4  i;
 
  for(i=0; i<nList; i++){
    if (IngfeatDefTrack[fDefLookup[i]]==2)
      SetItemStatus(feat_list, i+1, TRUE);
    else
      SetItemStatus(feat_list, i+1, FALSE);
  }
}

static void Ing_TrackToFilter(IngseqFormPtr sformp)
{
  Int4  i;
 
  for(i=0; i<sformp->nList; i++){
    if (GetItemStatus(sformp->feat_list, i+1)){
      IngfeatDefFilter[fDefLookup[i]]=TRUE;
      IngfeatDefTrack[fDefLookup[i]]=2;
    }
    else {
      IngfeatDefFilter[fDefLookup[i]]=FALSE;
      IngfeatDefTrack[fDefLookup[i]]=1;
    }
  }

}

static void Ing_ShowListAccept(ButtoN b)
{
  IngGenomeViewerPtr igvp=NULL;
  IngseqFormPtr      sformp=NULL;
  WindoW             w;

  w=ParentWindow(b);
  sformp=(IngseqFormPtr)GetObjectExtra(w);
  igvp= (IngGenomeViewerPtr) GetObjectExtra (b);
  if (!sformp || !igvp) return;
  Hide (w);
  Ing_TrackToFilter(sformp);
  Ing_PopulateOverviewPage((Pointer)igvp, FALSE);
  Ing_PopulateDetailedPage((Pointer)igvp);
  Remove(w);
}


static void Ing_LabelsProc(IteM item)
{
  IngGenomeViewerPtr igvp;

  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(item));
  if (igvp->bLabels){
    igvp->bLabels=FALSE;
    SetTitle(igvp->d_Win->labels, "Show Labels");
  }
  else {
    igvp->bLabels=TRUE;
    SetTitle(igvp->d_Win->labels, "Hide Labels");
  }
  Ing_PopulateDetailedPage((Pointer)igvp);
}


typedef struct ing_simplepopup {
  PopuP   pop;
  TexT    text;
} IngSimplePopup, PNTR IngSimplePopupPtr;


static void Ing_AdjustTopHeight(ButtoN b)
{
  IngGenomeViewerPtr igvp = NULL;
  IngSimplePopupPtr  spp = NULL;
  Char               str[20] ={""};
  WindoW             Dlg;

  Dlg = ParentWindow(b);
  spp = (IngSimplePopupPtr)GetObjectExtra(b);
  igvp = (IngGenomeViewerPtr)GetObjectExtra(Dlg);
  if (!spp || !igvp) return;
  Hide(Dlg);
  GetTitle(spp->text, str, sizeof(str));
  igvp->TopHeight = atoi(str);
  Ing_ResizeWin(igvp->hMain);
  Remove(Dlg);
}

static void Ing_TopHeightProc(IteM item)
{
  IngGenomeViewerPtr igvp;
  WindoW             w;
  GrouP              g;
  Char               str[20] = {""};
  IngSimplePopupPtr  spp;
  ButtoN             b;

  igvp = GetObjectExtra (ParentWindow(item));
  if (!igvp) return;

  w = FixedWindow(-50, -33, -10, -10, "Overview Height", StdCloseWindowProc);
  SetObjectExtra(w, igvp, NULL);
  
  spp = (IngSimplePopupPtr)MemNew(sizeof(IngSimplePopup));
  g = HiddenGroup(w, -2, 0, NULL);
  SetGroupMargins(g, 10, 10);
  SetGroupSpacing(g, 10, 20);
  StaticPrompt(g, "New Height:  ", 0, 0, systemFont, 'l');
  sprintf(str, "%d", igvp->TopHeight);
  spp->text = DialogText(g, str, 10, NULL);
  b = DefaultButton(g, "OK", Ing_AdjustTopHeight);
  SetObjectExtra(b, (Pointer)spp, StdCleanupExtraProc);
  PushButton(g, "Cancel", Ing_GenericCancelButton);
  Select(w);
  Show(w);
}


static void Ing_CreateShowListForm(IteM item)
{  
  ButtoN               b;
  WindoW               d;
  GrouP                h, h2;
  Int4                 i;
  IngseqFormPtr        sformp;
  IngGenomeViewerPtr   igvp=NULL;

  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(item));
  if (!igvp) return;
  d = MovableModalWindow(-50, -33 ,-10, -10, "Show or Hide Features", NULL);
  h = HiddenGroup(d, 1, 2,  NULL);
  sformp=(IngseqFormPtr)MemNew(sizeof(IngseqForm));
  sformp->nList=0;
  sformp->feat_list = MultiList(h, 20, 6, NULL);
  for (i=0; i<FEATDEF_MAX; i++){
    if (IngfeatDefTrack[i]){
      ListItem(sformp->feat_list, IngfeatDefLabels[i]);
      fDefLookup[sformp->nList]=i;
      sformp->nList++;
    }
  }
  Ing_SetShowList(sformp->feat_list, sformp->nList);
  h2 = HiddenGroup(h, 2, 1, NULL);
  b= DefaultButton(h2, "Accept", Ing_ShowListAccept);
  SetObjectExtra(b, (Pointer)igvp, NULL);
  Enable(b);
  PushButton(h2, "Cancel", Ing_GenericCancelButton);
  SetObjectExtra (d, (Pointer) sformp, StdCleanupExtraProc);
  RealizeWindow(d);
  Select(d);
  Show(d);
}


/*******************************************************************************

  Function : Ing_SearchandSelect(), Ing_SearchOK(), Ing_SearchCancel(), Ing_CreateSearchForm()
  
  Purpose : create a window to choose whether to show or hide specific features

*******************************************************************************/
typedef struct search {
  VieweR  v;
  SegmenT pict;
  Boolean selected;
  CharPtr name;
} IngSearch, PNTR IngSearchPtr;


static Boolean LIBCALLBACK Ing_SearchandSelect(SeqFeatPtr sfp, SeqMgrFeatContextPtr sfc)
{
  IngSearchPtr  searchp=NULL;
  Char          str[100]={""};


  searchp=(IngSearchPtr)sfc->userdata;
  if (!searchp || StringHasNoText(searchp->name)) return FALSE;
  FeatDefLabel (sfp, str, sizeof (str) - 1, OM_LABEL_CONTENT);
  if (StrStr(str, searchp->name)){
    msg.segID=0;
    msg.labelmsg=0;
    if (searchp->selected){
      ObjMgrAlsoSelect(sfc->entityID, sfc->itemID, OBJ_SEQFEAT, 0, NULL);
    }
    else {
      ObjMgrSelect(sfc->entityID, sfc->itemID, OBJ_SEQFEAT, 0, NULL);
      searchp->selected=TRUE;
    }
  }

  return TRUE;

}

static void Ing_SearchOK(ButtoN b)
{
  IngSimplePopupPtr    ipp=NULL;
  IngGenomeViewerPtr   igvp=NULL;
  WindoW               w;
  IngSearch            search;
  Char                 str[255]={""};
  Char                 strErr[50]={""};

  
  ipp=(IngSimplePopupPtr)GetObjectExtra(b);
  w=ParentWindow(b);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(w);
  Hide(w);
  GetTitle(ipp->text, str, sizeof(str));
  search.v = igvp->vBottom;
  search.pict = igvp->pictBottom;
  search.name = StringSave(str);
  search.selected = FALSE;
  SeqMgrExploreFeatures (igvp->bsp, (Pointer) &search, Ing_SearchandSelect, igvp->slp, NULL, IngfeatDefFilter);
  if (!search.selected){
    sprintf(strErr, "%s was not found in record", str);
    Message (MSG_OK, strErr);
    Show(w);
    return;
  }
  MemFree(search.name);
  Remove(w);
}

static void Ing_SearchCancel(ButtoN b)
{
  Remove(ParentWindow(b));
}



static void Ing_CreateSearchForm(IteM item)
{
  IngGenomeViewerPtr   igvp=NULL;
  WindoW               d;
  GrouP                g;
  IngSimplePopupPtr    ipp=NULL;
  ButtoN               b;
  Int4                 i;

  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(item));
  
  d = MovableModalWindow(-50, -33 ,-10, -10, "Find Feature", NULL);
  ipp=(IngSimplePopupPtr)MemNew(sizeof(IngSimplePopup));

  g = HiddenGroup(d, -2, -2,  NULL);
  StaticPrompt (g, "Keyword: ", 0, LineHeight() , systemFont, 'l');
  ipp->text = DialogText(g, "", 15, NULL);
  b = DefaultButton(g, "OK", Ing_SearchOK);
  PushButton(g, "Cancel", Ing_SearchCancel);
  SetObjectExtra(b, (Pointer)ipp, StdCleanupExtraProc);
  SetObjectExtra(d, (Pointer)igvp, NULL);

  RealizeWindow(d);
  Select(d);
  Show(d);
}


/*******************************************************************************

  Function : Ing_GetBspFromGIOrAcc(), Ing_NetOpen_okProc(), Ing_NetOpen_cancelProc(), Ing_GetFromNetwork(), Ing_ItemToNetwork()
  
  Purpose : retrieve sequence from network

*******************************************************************************/

extern BioseqPtr Ing_GetBspFromGIOrAcc(CharPtr str)
{
   BioseqPtr  bsp;
   Int4       gi;
   Char       ptr;
   SeqIdPtr   sip;
   ValNode    vn;

   str = TrimSpacesAroundString(str);
   ptr = *str;
   if (IS_ALPHA(ptr))  /* accession */
   {
      sip = SeqIdFromAccessionDotVersion(str);
      bsp = BioseqLockById(sip);
   } else  /* it's a GI */
   {
      gi = atoi(str);
      vn.choice = SEQID_GI;
      vn.data.intvalue = gi;
      vn.next = NULL;
      bsp = BioseqLockById(&vn);
   }
   return bsp;
}

static void Ing_NetOpen_okProc(ButtoN b)
{
  BioseqPtr          bsp=NULL;
  WindoW             w;
  IngFileOpenDataPtr dfodp=NULL;
  IngGenomeViewerPtr igvp=NULL;
  Char               str[50]={""};
  Uint2              entityID;
  SeqEntryPtr        sep=NULL;


  w=(WindoW)ParentWindow(b);
  if (!w) return;
  
  dfodp=(IngFileOpenDataPtr)GetObjectExtra(b);
  igvp=(IngGenomeViewerPtr)GetObjectExtra(ParentWindow(b));

  if (!dfodp || !igvp) return;

  Hide(w);
  /*retrieve a Gi*/
  GetTitle(dfodp->GI, str, sizeof(str));
  if (StringHasNoText (str)) {
    Message (MSG_OK, "Enter GI or Accession number");
    Show (w);
    Select (w);
    Select (dfodp->GI);
    return;
  }

  WatchCursor();
  Ing_ProgressNew(igvp, "Fetching your record.....");

  bsp=Ing_GetBspFromGIOrAcc(str);

  if (!bsp){
    Ing_ProgressEnd(igvp);
    ArrowCursor();
    Message (MSG_OK, "Unable to find your record in the database.");
    Select (w);
    Show (w);
    Select (dfodp->GI);
    return;
  }
  igvp->bspIsLocked=TRUE;
  entityID=ObjMgrGetEntityIDForPointer((Pointer)bsp);
  sep=GetTopSeqEntryForEntityID(entityID);
  sep = SeqMgrGetSeqEntryForData (bsp);
  igvp->entity_list=(IngEntityPtr)MemNew(sizeof(IngEntity));
  igvp->entity_list->entityID=entityID;
  igvp->entity_list->itemID=1;
  igvp->entity_list->bspcount=Ing_AllButPartsCount(sep, NULL, NULL, 0, 0);
  igvp->entity_list->Sips=SAM_ExtractSips(sep);
  igvp->cur_sip=(SeqId *)igvp->entity_list->Sips->data.ptrvalue;
  igvp->cur_target=1;

  ArrowCursor();
  Ing_ProgressEnd(igvp);

  if (dfodp->hStart){
    Remove(dfodp->hStart);
    dfodp->hStart=NULL;
  }
  Remove(w);
  if (entityID!=0){
    Ing_RunIngenue(igvp, bsp, entityID);
  }
}


static void Ing_NetOpen_cancelProc(ButtoN b)
{
  IngFileOpenDataPtr dfodp=NULL;

  dfodp=(IngFileOpenDataPtr)GetObjectExtra(b);
  if (dfodp && dfodp->hStart){
    Select(dfodp->hStart);
    Show(dfodp->hStart);
  }
  Remove(ParentWindow(b));
}

extern void Ing_GetFromNetwork(IngGenomeViewerPtr igvp, WindoW hStart)
{
  WindoW  w;
  GrouP   g, c;
  ButtoN  b;
  Char    szAppName  [10]={"Ingenue"};
  IngFileOpenDataPtr dfodp=NULL;

  dfodp=(IngFileOpenDataPtr)MemNew(sizeof(IngFileOpenData));

  w = MovableModalWindow (-50, -33, -10, -10, "Download From NCBI", NULL);
  SetObjectExtra(w, (Pointer)igvp, NULL);

  if (w==NULL) return;
  SetGroupSpacing (w, 10, 10);
  /*accesion*/
  g = NormalGroup (w, 3, 0, "GI or Accession", systemFont,NULL);
  dfodp->GI=DialogText (g, "", 10, Ing_NetOpen_access);
  SetObjectExtra(dfodp->GI, (Pointer)dfodp, StdCleanupExtraProc);
  /*retrieve - cancel*/
  c = HiddenGroup (g, 4, 0, NULL);
  SetGroupSpacing (c, 10, 2);
  dfodp->OK=DefaultButton (c, "Retrieve", Ing_NetOpen_okProc);
  SetObjectExtra(dfodp->OK, (Pointer)dfodp, NULL);
  Disable(dfodp->OK);
  b=PushButton (c, "Cancel", Ing_NetOpen_cancelProc);
  SetObjectExtra(b, (Pointer)dfodp, NULL);
  dfodp->hStart=hStart;
/*   SetObjectExtra (w, (Pointer) unop, StdCleanupExtraProc); */
  
  /*display*/
  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
  RealizeWindow (w);
  Select (w);
  Show (w);
  Update ();
  
}

static void Ing_ItemtoNetwork(IteM i){
  IngGenomeViewerPtr igvp_new=NULL;

 
  if (!(igvp_new=Ing_MainDataNew()))
    return;
  Ing_GetFromNetwork(igvp_new, NULL);
}


/*******************************************************************************

  Function : Ing_FileOpenFromCommandline(), Ing_NetOpenFromCommandline()
  
  Purpose : retrieve a sequence from the commandline

*******************************************************************************/
extern void Ing_FileOpenFromCommandline(CharPtr filename, Int4 start, Int4 stop, Int4 spi_job, CharPtr spi_f, CharPtr spi_gilist, CharPtr spi_seqidfile, Int4 b2s_job, CharPtr b2s_f, CharPtr b2s_gilist, CharPtr b2s_seqidfile)
{
  IngGenomeViewerPtr igvp=NULL;

  if (StringHasNoText (filename)) {
    Message (MSG_OK, "Enter File name");
    return;
  }
  igvp=(IngGenomeViewerPtr)MemNew(sizeof(IngGenomeViewer));
  if (!igvp) return;
  Ing_InitMainDataPtr(igvp);
  igvp->from=start;
  igvp->to=stop;
  igvp->filetype=1;
  Ing_FileOpenMain(filename, igvp);
  if (spi_job != 0){
    igvp->filetype=spi_job;
    if (spi_f)
      Ing_FileOpenSpecial(igvp, spi_f, NULL);
    else if (spi_seqidfile)
      Ing_FileOpenSpecial(igvp, spi_seqidfile, NULL);
    else if (spi_gilist)
      Ing_FileOpenSpecial(igvp, NULL, spi_gilist);
  }
  if (b2s_job !=0){
    igvp->filetype=b2s_job;
    if (b2s_f)
      Ing_FileOpenSpecial(igvp, b2s_f, NULL);
    else if (b2s_seqidfile)
      Ing_FileOpenSpecial(igvp, b2s_seqidfile, NULL);
    else if (b2s_gilist)
      Ing_FileOpenSpecial(igvp, NULL, b2s_gilist);
  }
  ProcessEvents();
}

extern void Ing_NetOpenFromCommandline(CharPtr txt, Int4 start, Int4 stop, Int4 spi_job, CharPtr spi_f, CharPtr spi_gilist, CharPtr spi_seqidfile, Int4 b2s_job, CharPtr b2s_f, CharPtr b2s_gilist, CharPtr b2s_seqidfile)
{
  BioseqPtr   bsp=NULL;
  SeqEntryPtr sep;
  Uint2                entityID;
  IngGenomeViewerPtr   igvp=NULL;
  Char                 strErr[50]={""};

  if (StringHasNoText (txt)) {
    Message (MSG_OK, "Enter GI or Accession number");
    return;
  }

  WatchCursor();
 igvp=(IngGenomeViewerPtr)MemNew(sizeof(IngGenomeViewer));
  if (!igvp) return;
  Ing_InitMainDataPtr(igvp);
  igvp->filetype=1;
  igvp->from=start;
  igvp->to=stop;

  bsp=Ing_GetBspFromGIOrAcc(txt);

  if (!bsp){
    sprintf(strErr, "Ingenue can't find your record %s", txt);
    Message (MSG_OK, strErr);
    Ing_MainDataFree(igvp);
    MemFree(txt);
    ArrowCursor();
    return;
  }

  Ing_ProgressNew(igvp, "Fetching your record...");

  MemFree(txt);
  if (igvp->to == 0)
    igvp->to=bsp->length-1;

  entityID=ObjMgrGetEntityIDForPointer((Pointer)bsp);
  sep = GetTopSeqEntryForEntityID(entityID);
/*   sep = SeqMgrGetSeqEntryForData (bsp); */
  igvp->entity_list=(IngEntityPtr)MemNew(sizeof(IngEntity));
  igvp->entity_list->entityID=entityID;
  igvp->entity_list->itemID=1;
  igvp->entity_list->bspcount=Ing_AllButPartsCount(sep, NULL, NULL, 0, 0);
  igvp->entity_list->Sips=SAM_ExtractSips(sep);
  igvp->cur_sip=(SeqId *)igvp->entity_list->Sips->data.ptrvalue;
  igvp->cur_target=1;

  ArrowCursor();
  Ing_ProgressEnd(igvp);

  if (entityID!=0){
    Ing_RunIngenue(igvp, bsp, entityID);
  }

  if (spi_job != 0){
    igvp->filetype=spi_job;
    if (spi_f)
      Ing_FileOpenSpecial(igvp, spi_f, NULL);
    else if (spi_seqidfile)
      Ing_FileOpenSpecial(igvp, spi_seqidfile, NULL);
    else if (spi_gilist)
      Ing_FileOpenSpecial(igvp, NULL, spi_gilist);
  }
  if (b2s_job !=0){
    igvp->filetype=b2s_job;
    if (b2s_f)
      Ing_FileOpenSpecial(igvp, b2s_f, NULL);
    else if (b2s_seqidfile)
      Ing_FileOpenSpecial(igvp, b2s_seqidfile, NULL);
    else if (b2s_gilist)
      Ing_FileOpenSpecial(igvp, NULL, b2s_gilist);
  }
  ProcessEvents();
}



/*******************************************************************************

  Function : Ing_WinTimerProc(), Ing_ProgressNew(), Ing_ProgressEnd()
  
  Purpose : signal that the program is preforming a task (has not stalled)

*******************************************************************************/
extern void Ing_WinTimerProc(WindoW progressW)
{
  IngGenomeViewerPtr igvp;


  igvp=(IngGenomeViewerPtr)GetObjectExtra(progressW);
  if (igvp->progress_running){
      igvp->progress_string=Ing_StringCat(igvp->progress_string, "..");
      if (StringHasNoText(igvp->progress_counter))
        SetTitle(igvp->progress_prompt, igvp->progress_string);
      else 
        SetTitle(igvp->progress_prompt, igvp->progress_counter);
      Select(progressW);
      Update();
  }
  else {
    igvp->progress_timer=0;
    SetTitle(igvp->progress_prompt, "Ready !");
    MemFree(igvp->progress_string);
    igvp->progress_string=NULL;
    Select(progressW);
    Update();
  }
}


extern void Ing_ProgressNew(IngGenomeViewerPtr igvp, CharPtr name)
{
  WindoW  tmpMain = NULL;

  if (igvp->hMain == NULL){
    tmpMain = MovableModalWindow(-30, -20,  -10,  -10, "Ingenue - Get Sequence",  NULL);
    igvp->progress_prompt = StaticPrompt(tmpMain, "", 25*MaxCharWidth(), LineHeight(), systemFont, 'l');
    SetObjectExtra(tmpMain, (Pointer)igvp, NULL);
    igvp->hMain = tmpMain;
    Select(tmpMain);
    Show(tmpMain);
  }

  SetTitle(igvp->progress_prompt, name);
  igvp->progress_string=StringSave(name);
  igvp->progress_running=TRUE;
  Select(igvp->hMain);
  Update();
}

extern void Ing_ProgressEnd(IngGenomeViewerPtr igvp)
{
  igvp->progress_running=FALSE;
  MemSet((Pointer)igvp->progress_counter, '\0', sizeof(igvp->progress_counter));
  Ing_WinTimerProc(igvp->hMain);
}


/*******************************************************************************

  Function : Ing_SetupMainWindow(), Ing_BuildMainGui()
  
  Purpose : create the main window

*******************************************************************************/
static void Ing_SetupMainWindow(IngGenomeViewerPtr PNTR real_igvp)
{
WindoW         hMain;
GrouP          g, g1, g2, g3, subg2;
VieweR         v1,v2;
IteM           i; 
ButtoN         b1, b2; 
SegmenT        seg1, pict;
ChoicE         ch;
MenU           m1, m2, m3, m4, m5, m6, m7, m8;
MenU           submenu1, submenu2, submenu3; 
Char           title[100]={""}, str1[20]={""}, str2[20]={""}; 
Char           szAppName [10]={"Ingenue"}; 
FonT           hFnt;
Int2           cxChar,cyChar;
IngRangePtr    rnp;
IngGenomeViewerPtr igvp;
RecT           rcg;
Int4           Fh; 
 Int4           j; 

 
 igvp=*real_igvp;

#ifdef WIN_MAC
  Ing_SetupMenus ();
#endif
	hMain = DocumentWindow( 5*stdCharWidth, -5*stdCharWidth, 600, 900,
		igvp->title,Ing_CloseWin,Ing_ResizeWin);
	if (!hMain) return;

#ifndef WIN_MAC  
   Ing_SetupMenus ();
#endif
  SetObjectExtra (hMain, (Pointer) igvp, NULL);

  m1 = PulldownMenu (hMain, "File");
  submenu1=SubMenu(m1,"New Sequence"); 

#ifdef USE_ENTREZ 
  CommandItem(submenu1, "Network...", Ing_ItemtoNetwork); 
#endif 
  SetWindowTimer(hMain, IngenueCheckSocketsProc);

  igvp->d_Win=(IngWinDataPtr)MemNew(sizeof(IngWinData));
  CommandItem(submenu1, "File...", Ing_LocalFileProc); 
/*   igvp->imemory=CommandItem(submenu1, "Memory...", Ing_BspListProc); */
/*   Disable(igvp->imemory); */
  submenu2=SubMenu(m1, "Annotate");
  igvp->d_Win->Feat=submenu2;
  Disable(submenu2); 
  CommandItem(submenu2, "Genscan File..", Ing_OpenGenscan);
  CommandItem(submenu2, "Blast File..", Ing_OpenBlast);
  CommandItem(submenu2, "Feature Table..", Ing_OpenFeatTable);
  CommandItem(submenu2, "Tab Delimited..", Ing_OpenTabDel);
  SeparatorItem(m1);
  CommandItem(m1, "Show as Flat File", Ing_ViewFlatFile);
  i=CommandItem(m1, "Switch target", Ing_ChangeTarget);
  if (igvp->entity_list->bspcount<2)
    Disable(i);
  SeparatorItem(m1);
  CommandItem(m1, "Save", Ing_SaveProc);
  CommandItem(m1, "Print", Ing_PrintProc);
  CommandItem (m1, "Close Window", Ing_CloseProc); 
  SeparatorItem(m1);
  CommandItem (m1, "Exit", Ing_QuitProc); 

  m2 = PulldownMenu (hMain, "Edit");
  CommandItem(m2, "Edit Feature", Ing_EditFeature);
  CommandItem(m2, "Delete Feature", Ing_DeleteFeature);

  m3 = PulldownMenu (hMain, "Features");
  submenu2=SubMenu(m3, "Annotate New Feature");
  Ing_SetupNewFeaturesMenu(submenu2, igvp);
  igvp->d_Win->Annotate=m3;
  CommandItem(m3, "Edit Feature", Ing_EditFeature);
  CommandItem (m3, "Show or Hide Feature", Ing_CreateShowListForm); 
  CommandItem(m3, "Search ..", Ing_CreateSearchForm);


  m4 = PulldownMenu(hMain, "Alignments");
  CommandItem(m4, "Annotate Alignment", Ing_AnnotateAlignmentForm);
  CommandItem(m4, "Replace Mismatches..", Ing_ReplaceAllMismatched);

  CommandItem(m4, "Print Report", Ing_ReportWindow);
  igvp->d_Win->mrnaorexons = CommandItem(m4, "Spidey - as Exons", Ing_mRNAOrExons);
  Disable(igvp->d_Win->mrnaorexons);
  CommandItem (m4, "Switch Order", Ing_CreateReOrderAlignForm);


  m5 = PulldownMenu (hMain, "Options");
  igvp->d_Win->labels=CommandItem(m5, "Hide Labels", Ing_LabelsProc);
  CommandItem(m5, "Overview Height", Ing_TopHeightProc);

  m6 = PulldownMenu(hMain, "Tools");
  CommandItem(m6, "View Sequence", Ing_OpenUDV);
  CommandItem (m6, "View Aligns as Diags", Ing_DotDiagProc);
  
  /* do analysis */
  m7 = PulldownMenu (hMain, "Analysis");
  CommandItem(m7, "Spidey...", Ing_GetFileForSpidey);
  SeparatorItem(m7);
  CommandItem(m7, "Blast 2 Sequences...", Ing_Blast2SeqsProc);
  CommandItem(m7, "QBlast...", Ing_QBlastProc);
  SeparatorItem(m7);
  CommandItem(m7, "Dot-Matrix", Ing_DotMatrixProc);
  Disable(m7);
  igvp->d_Win->Analysis=m7;

  /* find motifs */
  m8 = PulldownMenu (hMain, "Motifs");
  CommandItem(m8, "CpG islands", Ing_CpGProc);
  i = CommandItem(m8, "Poly-A sites", Ing_PolyAProc);
  Disable(i); /* sarah needs to fix this */
  CommandItem(m8, "Repeats", Ing_RepeatsProc);
  CommandItem(m8, "Signal Peptide", Ing_SignalPeptideProc);
  i = CommandItem(m8, "Kozak", Ing_KozakProc);
  Disable(i); /* sarah needs to fix this */
  CommandItem(m8, "User Defined", Ing_UserMotifProc);
  SeparatorItem(m8);
  igvp->d_Win->GC = CommandItem(m8, "Show GC content", Ing_ShowGCContent);
  CommandItem(m8, "Find low complexity", Ing_ShowComplexity);
  CommandItem(m8, "Find ORF", Ing_RunORFinder);
  SeparatorItem(m8);
  Ing_tRNAscanMenu(m8);

  g=HiddenGroup(hMain,0,7,NULL);
  igvp->gMain=g;

  hFnt = igvp->GrData.hFnt;
  SelectFont(hFnt);
  cxChar=igvp->GrData.cxChar;
  cyChar=igvp->GrData.cyChar;
  rnp=(IngRangePtr)MemNew(sizeof(IngRange));
  g1 = HiddenGroup (g,  -2,  -1, NULL); 
  g2 = HiddenGroup (g1, -1,  -3, NULL); 
  g3=HiddenGroup(g2, -6, -2, NULL);
  SetGroupSpacing (g3, 10, 20);
  igvp->d_Win->Goto=g3;
  StaticPrompt (g3, "show ONLY from:", 0, 3*cyChar/2, hFnt, 'l');
  rnp->left = DialogText (g3, "0", (Int2)4, NULL);
  StaticPrompt (g3, "to:", 0, 3*cyChar/2, hFnt, 'l');
  rnp->right = DialogText (g3, "0", (Int2)4, NULL);
  b1=DefaultButton (g3, "OK", Ing_GotoProc);
  b2=PushButton (g3, "show ALL", Ing_ShowALLProc);
  SetObjectExtra(b1, (Pointer)rnp, StdCleanupExtraProc);
  SetObjectExtra(b2, (Pointer)rnp, NULL);

  subg2=HiddenGroup(g2, 1, 3, NULL);
  igvp->deflineg=subg2;
  for (j = 0; j < 4; j++)
    igvp->featInfo[j]= StaticPrompt(subg2, "", 30*cxChar, cyChar , programFont, 'l');

  igvp->vZoom=CreateViewer(g1, 75, 100,FALSE,FALSE);
  igvp->pictZoom=CreatePicture();
  if (igvp->hMain){
    Remove(igvp->hMain);
  }
  igvp->hMain=hMain;
  *real_igvp=igvp;

}


static WindoW Ing_BuildMainGui(IngGenomeViewerPtr igvp, Int2 numrows)
{
WindoW hMain;
GrouP  g;
IteM   i; 
SegmenT seg1,seg2;
Int2   Margins;
ChoicE  ch;
MenU     m1, m2, m3, m4, m5, m6, submenu1, submenu2; 
Char    title[100]; 

 
	if (!igvp) return(NULL);
   Ing_InitfeatDefFilter();
   Ing_SetupMainWindow(&igvp);

   Enable(igvp->d_Win->Goto);
   Enable(igvp->d_Win->Feat);
   Enable(igvp->imemory);
	hMain=igvp->hMain;
   g = igvp->gMain;
   igvp->vRuler1= CreateViewer(g,300,(RULERDEFHEIGHT*igvp->GrData.cyChar),FALSE,FALSE);
   igvp->pictRuler1 = CreatePicture();
   igvp->vTop = CreateViewer(g, 300 ,300, TRUE, TRUE);
   igvp->pictTop=CreatePicture();
   igvp->vRuler2= CreateViewer(g,300,(RULERDEFHEIGHT*igvp->GrData.cyChar)*2,FALSE,FALSE);
   igvp->pictRuler2 = CreatePicture();
   igvp->vBottom = CreateViewer(g, 300 ,300, TRUE, TRUE);
   igvp->pictBottom=CreatePicture();
   igvp->progress_prompt = StaticPrompt (g, "Ready !", 100, LineHeight(), programFont, 'l');

	AlignObjects(ALIGN_JUSTIFY, (HANDLE) igvp->deflineg, (HANDLE) g, NULL , NULL);
   
   
	return(hMain);	
}


/*******************************************************************************

  Function : Ing_AttachMessageProc()
  
  Purpose : registered message callback

*******************************************************************************/
extern Boolean Ing_AttachMessageProc(IngGenomeViewerPtr igvp, Uint2 procID, Uint2 eID)
{
  OMUserDataPtr omudp;
  Uint2         userkey;
  
  igvp->userKey=OMGetNextUserKey();
  igvp->procID=procID;
  
  omudp = ObjMgrAddUserData (eID, procID, OMPROC_VIEW, igvp->userKey);
  if (omudp == NULL) return(FALSE);
  
  omudp->messagefunc =  Ing_OM_MsgFunc;
  omudp->userdata.ptrvalue = (Pointer)igvp;
  
  return(TRUE);
}

/*******************************************************************************

  Function : Ing_GetSeqLocations()
  
  Purpose : Store sequence coordinates 

*******************************************************************************/

static void Ing_GetSeqLocations(IngGenomeViewerPtr igvp, BioseqPtr bsp)
{
  Int4 start, stop;

  start=igvp->from;
  stop=igvp->to;
  if (stop !=0 && stop<= bsp->length-1 && (start<stop)){
    igvp->slp=SeqLocIntNew(start, stop, (bsp->strand==Seq_strand_plus?1:2), bsp->id);
    igvp->SeqLength=stop-start+1;
  }
  else if (start==0 && stop==0){
    igvp->to=bsp->length-1;
    igvp->SeqLength = bsp->length;
  }
  igvp->bspLength=bsp->length;
}


/*******************************************************************************

  Function : Ing_LoadViewer()
  
  Purpose : retrieve data from object manager, start up a new window 

*******************************************************************************/
static IngGenomeViewerPtr Ing_LoadViewer(IngGenomeViewerPtr igvp, BioseqPtr bsp, Uint2 itemType, Int2 numrows)
{
WindoW             hMain;
Boolean            bRet;
Int4               from,to,scaleX, diff;
Uint2              entityID, itemID; 

	WatchCursor();

	/*BioSeq ?*/
	if (itemType!=OBJ_BIOSEQ) {
		ErrPostEx(SEV_ERROR, 0, 0, szErrMsg1);
		bRet=FALSE;
		goto fin;
	}
	/*get the BioSeq Pointer*/
	if (bsp==NULL){
		ErrPostEx(SEV_ERROR, 0, 0, szErrMsg4);
		bRet=FALSE;
		goto fin;
	}
	/*check the sequence length; could be undef.*/
	if (bsp->length==(Int4)-1){
		ErrPostEx(SEV_ERROR, 0, 0, szErrMsg5);
		bRet=FALSE;
		goto fin;
	}

	/*save the BioSeq ids*/
   entityID=ObjMgrGetEntityIDForPointer((Pointer)bsp);
   if (SeqMgrFeaturesAreIndexed (entityID) == 0) {
      SeqMgrIndexFeatures (entityID, NULL);
    }
   itemID=GatherItemIDByData(entityID, itemType, (Pointer)bsp);
   igvp->entityID = entityID;
	igvp->itemID = itemID;
   igvp->itemType=itemType;
   igvp->bsp = bsp;

   /* get seq loc coordinates if specified */
   Ing_GetSeqLocations(igvp, bsp);
   SeqIdWrite(bsp->id, igvp->title, PRINTID_FASTA_LONG, 41);
   SetTitle(igvp->hMain, igvp->title);
	/*build the GUI*/
	Ing_InitGrData(&igvp->GrData);
	
   hMain = Ing_BuildMainGui(igvp, numrows);
   igvp->hMain=hMain;
   if (hMain==NULL) {
     ErrPostEx(SEV_ERROR, 0, 0, szErrMsg3);
     bRet=FALSE;
     goto fin;
   }
     
   

   igvp->xposition=0;
	RealizeWindow(hMain);
	Ing_ResizeMainWindow(igvp,hMain, TRUE);

   if (Ing_PopulateZoomer(igvp)==FALSE) 
     {
       ErrPostEx(SEV_ERROR, 0, 0, szErrMsg2);
       bRet=FALSE;
       goto fin;
     }

   if ((Ing_BuildStartupPage(igvp))==FALSE)
     {
       ErrPostEx(SEV_ERROR, 0, 0, szErrMsg2);
       bRet=FALSE;
       goto fin;
     }


	Ing_GetNamesZoomedVal(igvp->vRuler2,&from,&to,&scaleX,igvp->SeqLength);
   Ing_MoveCursor(igvp,from,to, TRUE);
   igvp->xposition=from;
	Show(hMain);
	bRet=TRUE;

fin:
	if (bRet==FALSE){
		if (igvp){
        if (igvp->bspIsLocked)
          BioseqUnlock(igvp->bsp);
        igvp=Ing_MainDataFree(igvp);
        MemFree(igvp);
		}
	}
	ArrowCursor();
	return(igvp);
}


/*******************************************************************************

  Function : Ing_RegIngenueProc()
  
  Purpose : registered ingenue function

*******************************************************************************/
NLM_EXTERN Int2 LIBCALLBACK Ing_RegIngenueProc(Pointer data)
{
  IngGenomeViewerPtr  igvp;
  OMProcControlPtr    ompcp;
  BioseqPtr           bsp;
  SeqEntryPtr         sep;
  
  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) {
    ErrPostEx (SEV_ERROR, 0, 0, "Data NULL [1]");
    return OM_MSG_RET_ERROR;
  }
  
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
    ErrPostEx (SEV_ERROR, 0, 0, "Input Data NULL");
    return OM_MSG_RET_ERROR;
  }
  
  igvp=(IngGenomeViewerPtr)ompcp->input_choice;
  if (igvp == NULL) {    
    igvp = Ing_MainDataNew();
    sep = GetTopSeqEntryForEntityID(ompcp->input_entityID);
    igvp->entity_list=(IngEntityPtr)MemNew(sizeof(IngEntity));
    igvp->entity_list->entityID= ompcp->input_entityID;
    igvp->entity_list->itemID= ompcp->input_itemID;
    igvp->entity_list->bspcount=Ing_AllButPartsCount(sep, NULL, NULL, 0, 0);
    igvp->entity_list->Sips=SAM_ExtractSips(sep);
    igvp->cur_sip=(SeqId *)igvp->entity_list->Sips->data.ptrvalue;
    igvp->cur_target=1;
  }
  Ing_LoadViewer(igvp, bsp, OBJ_BIOSEQ, 0); 
  if (!igvp) return(OM_MSG_RET_ERROR);
  Ing_AttachMessageProc(igvp,ompcp->proc->procid,ompcp->input_entityID);
  return(OM_MSG_RET_DONE);
}

/*******************************************************************************

  Function : Ing_RegIngenueProc()
  
  Purpose : registered ingenue function, will create its own igvp soon

*******************************************************************************/
NLM_EXTERN Int2 LIBCALLBACK Ing_RegisterIngenueProc (Pointer data)
{
  IngGenomeViewerPtr  igvp;
  OMProcControlPtr    ompcp;
  BioseqPtr           bsp;
  SeqEntryPtr         sep;
  
  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) {
    ErrPostEx (SEV_ERROR, 0, 0, "Data NULL [1]");
    return OM_MSG_RET_ERROR;
  }
  
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
    ErrPostEx (SEV_ERROR, 0, 0, "Input Data NULL");
    return OM_MSG_RET_ERROR;
  }
  return OM_MSG_RET_OK;
  /*
  ErrPostEx (SEV_ERROR, 0, 0, "Not yet implemented");
  return OM_MSG_RET_ERROR;
  */
}

