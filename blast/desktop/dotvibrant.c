/*   dotvibrant.c
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
* File Name:  dotvibrant.c
*
* Author:  Fasika Aklilu
*
* Version Creation Date:   8/8/01
*
* $Revision: 6.13 $
*
* File Description: mouse management, graphic engine of the sequence viewer
*                   part of this code is also used for the WWW Entrez viewer
*                   (WWW_UDV define)
* Modifications:
* --------------------------------------------------------------------------
* $Log: dotvibrant.c,v $
* Revision 6.13  2001/10/15 18:18:37  wheelan
* adapted to new alignment manager
*
* Revision 6.12  2001/08/08 22:41:31  aklilu
* added revision
*
* Revision 6.11  2000/10/31 22:39:14  vakatov
* Get rid of the erroneous casts to HANDLE in the Nlm_Enable/Disable calls
*
* Revision 6.10  2000/08/07 16:34:51  kans
* added public domain notice
*
Revision 6.9  2000/08/07 13:46:59  sicotte
added revision

Revision 6.8  2000/08/07 13:46:34  sicotte
added revision

Revision 6.7  2000/08/07 13:46:05  sicotte
added Version logging

Revision 6.6  2000/08/07 13:45:22  sicotte
fixed (long) cast in sprintf and fprintf
*
*
* ==========================================================================
*/
/* dotvibrant.c */

#include <dotviewer.h>
#include <alignmgr2.h>


 /****************************************************************************

     DEFINES                                                            
 ***************************************************************************/

#define MAXZOOMSCALEVAL 23
#define BLOCK_SIZE 10
#define VIS_LEN 100

#define dot_SEQVIEW 1
#define dot_FEATVIEW 2
#define dotaln_BEST 1
#define dotaln_OUTLYING_LARGE 2
#define dotaln_OUTLYING_SMALL 3
#define dotaln_GENERAL 4
#define dot_plot 1
#define align_plot 2


#define SHOW_MATCHES 1
#define SHOW_MISMATCHES 2

 /****************************************************************************

     GLOBAL VARIABLES                                                             
 ***************************************************************************/

static CharPtr  na_names[] = {"A", "C", "G", "T", "-"};
static CharPtr  aa_names [] =  {"-", "A", "B", "C", "D", "E", "F", "G", "H", "I", "K", "L", "M", "N", "P", "Q", "R", "S", "T", "V", "W","X", "Y", "Z", "U" , "*"};
static Int4  zoomScaleVal [MAXZOOMSCALEVAL] = {
  1L, 1L, 2L, 3L, 4L, 5L, 6L, 7L, 8L, 9L, 10L, 20L,
  30L, 40L, 50L, 60L, 70L, 80L, 90L, 100L, 200L, 500L, 1000L
};

Uint2  prev_primID = 0;

 /****************************************************************************

      FUNCTION DECLARATIONS                                                               
 ***************************************************************************/

static void DOT_DrawXAxis(SegmenT seg2, RecT  r, Int4 height, Int4 xstart,Int4 xstop, Int4 scale, SeqIdPtr sip);
static void DOT_DrawYAxis(SegmenT seg2, RecT  r, Int4 height, Int4 ystart, Int4 ystop, Int4 scale, Int4 Fh, SeqIdPtr sip);
static void DOT_VCrossHairs (RecT rcP, DOTVibDataPtr vdp, Int4 VFrom,Int4 HFrom);
static void DOT_HCrossHairs (RecT rcP, DOTVibDataPtr vdp, Int4 VFrom,Int4 HFrom);
static void DOT_QViewerClickProc(VieweR v, SegmenT seg, PoinT pt);
static void DOT_SViewerClickProc(VieweR v, SegmenT seg, PoinT pt);
static void DOT_ExitAlign(DOTAlignInfoPtr alp);
static DOTAlnPtr DOT_FindAlignment(DOTVibDataPtr vdp, Uint2 primID);
static Uint1Ptr DOT_GetNthSeqFromAlign (SeqAlignPtr sap, Int4 n);
static void DOT_SetColor(SegmenT  seg1, Int1 code, Boolean is_autopanel);

/* scoop declarations */
extern void SCP_OrganizeAlnsInSet(SeqAlignPtr sap, Int4 fuzz, SCP_ResultPtr scp, Int4 n);
static int LIBCALLBACK SCP_CompareSpins(VoidPtr ptr1, VoidPtr ptr2);
static void SCP_GetNthSeqRangeInSASet(SeqAlignPtr PNTR saparray, Int4 numsaps, Int4 n, Int4Ptr start, Int4Ptr stop);

/*_________________________________________(DOT_Compression)_____

  Purpose : Calculate sequence compression for window1

____________________________________________________________________*/

Int4 DOT_Compression (Int4  len, Int4 viewersize)
{
  Int4   cmpfactor;
  double f;

  

      f = (double)(len/viewersize);
      if (f <= 1)
        cmpfactor = 1;
      else
        cmpfactor = ceil(f);

      return cmpfactor;

}

/*_______________________________(DOT_SetupMenus)____________

  Purpose : Setup menus function for window1.

____________________________________________________________________*/
void DOT_SetupMenus ()

{
  MenU    m;  

#ifdef WIN_MAC
  m = AppleMenu (NULL);
  DeskAccGroup (m);
#endif

}



/*________________________________(DOT_DrawXGrids)____________________


  Purpose : Draw x-axis for DisplayHits, window1.

____________________________________________________________________*/
void DOT_DrawXGrids (RecT rupdate, RecT rcP, DOTVibDataPtr vdp, Int4 VFrom, Int4 HFrom, Int4 HTo, Int4 comp, Boolean GRID)
{
  Int4         x, y, y2, offset=0, start, end;
  Int4         scale_pos, pos, Hseq_pos;
  Char         scale_buf[15] = {""};	/*scale value*/
  

  offset=vdp->xstart;
 
  /* draw the axis no matter how short the length */
/*    if (vdp->curr_qlen <50) */
/*     return; */
 
  pos = 100;
 /* select the font type */
  SelectFont(vdp->Fnt);

  Black(); /* X axis */
  MoveTo (rcP.left,rcP.top-10);
  LineTo (rcP.left+ HTo,rcP.top-10);


  /* write sequence length */
  if ( vdp->curr_qlen - HFrom == HTo)
    {

      Red();
      sprintf(scale_buf, "%d", vdp->xstop);
      MoveTo (rcP.left+ HTo +10 ,rcP.top-5);
      PaintString (scale_buf);
    }

  HTo += HFrom;
  

      for (scale_pos = HFrom+1; scale_pos <= HTo+1; scale_pos++)
        {
          
          /*  draw 0 on axis */
          if (scale_pos == 1) 
            { 
              Black();
              x =  rcP.left;
              y =  rcP.top;
              MoveTo (x, y-10);
              LineTo (x,y-20);
              Blue();
              MoveTo(x,y -25);
              sprintf(scale_buf, "%d", offset);
              PaintString (scale_buf);
              
              
            } 
          else  
            {
              if (!(scale_pos % pos))
                {
                  Hseq_pos = (scale_pos*comp)+offset;
                  
                  x = rcP.left + scale_pos-HFrom;
                  y = MAX(rupdate.top, rcP.top);
                  y2 = MIN(rupdate.bottom, rcP.top+vdp->curr_slen-VFrom);
                  
                  if ((!(HTo<rcP.left))&& (y<y2) && GRID)
                    {
                      LtGray();
                      Dotted();
                      MoveTo (x, y);
                      LineTo (x,y2);  
                    }
                  
                  /* add scale values */
                  Black();
                  Solid();
                  y = rcP.top-10;
                  MoveTo (x, y);
                  LineTo (x,y-10);
                  sprintf(scale_buf, "%d", Hseq_pos);
                  x = rcP.left + scale_pos -HFrom - (StringWidth(scale_buf)/2);
                  y = rcP.top -25;
                  if (x>rcP.left)
                    {
                      Blue();
                      MoveTo(x,y);
                      PaintString (scale_buf);
                    }
                }
              else if (!(scale_pos % (pos/2)))
                {
                  x = rcP.left + scale_pos -HFrom;
                  y = rcP.top-10;
                  MoveTo (x, y);
                  LineTo (x,y-7);
                  
                }
              else if (!(scale_pos % (pos/10)))
                {
                  Black();              
                  x = rcP.left + scale_pos -HFrom;
                  y = rcP.top-10;
                  MoveTo (x, y);
                  LineTo (x,y-5);
                  
                }
            }
        }


 end:
  Black();
    
    
}

/*________________________________(DOT_DrawYGrids)____________________

  Purpose : Draw y-axis for DisplayHits, window1.

____________________________________________________________________*/
static void DOT_DrawYGrids (RecT rupdate, RecT rcP, DOTVibDataPtr vdp, Int4 VFrom, Int4 HFrom, Int4 VTo, Int4 comp, Boolean GRID)
{
  Int4         x, y, x2, offset=0;
  Int4         scale_pos, pos, Vseq_pos;
  Char         scale_buf[15] = {""};	/*scale value*/
  Int4         fh;
  Boolean      Decrement = FALSE;
  
  offset=vdp->ystart;
  
  /* draw the axis no matter how short the length */
/*   if (vdp->curr_slen <50) */
/*     return; */

  pos = 100;

  SelectFont(vdp->Fnt);
  fh = FontHeight();

  Black();
  MoveTo (rcP.left-10,rcP.top);
  LineTo (rcP.left-10,rcP.top+ VTo);


  if (vdp->curr_slen - VFrom == VTo)
    {
      /* write the sequence length */
      Red();
      sprintf(scale_buf, "%d", vdp->ystop);
      MoveTo (rcP.left-10 -(StringWidth(scale_buf)/2),rcP.top+ VTo +10 + (fh/2)); 
      PaintString (scale_buf);
    }

  VTo += VFrom;

  for (scale_pos = VFrom+1; scale_pos <= VTo+1; scale_pos++)
    {
      if (scale_pos == 1) 
        {
          Black();
          x =  rcP.left-10;
          y =  rcP.top;
          MoveTo (x, y);
          LineTo (x-10,y);
          Blue();
          sprintf(scale_buf, "%d", offset);
          MoveTo(rcP.left-25-StringWidth(scale_buf),y+fh-4);
          PaintString (scale_buf);
        } 
      else if (!(scale_pos % pos))
        {
          Vseq_pos = (!Decrement)?((scale_pos)*comp)+offset: offset - ((scale_pos)*comp);
          
          x = MAX(rupdate.left, rcP.left);
          x2 = MIN(rupdate.right, rcP.left+vdp->curr_qlen-HFrom);
          y = rcP.top -VFrom + scale_pos;
         
          /* draw vertical grid */
          if ((scale_pos != 0) && (x<x2) && GRID)
            {
              LtGray();
              Dotted(); 
              MoveTo (x, y);
              LineTo (x2,y);
            }
          /* add scale values */
          Black(); 
          Solid();
          x = rcP.left -10;
          MoveTo (x, y);
          LineTo (x-10,y);
          sprintf(scale_buf, "%d", Vseq_pos);
          x = rcP.left -25 - StringWidth(scale_buf);
          y = rcP.top-VFrom + scale_pos+ (fh/2);
           
          if (y-fh > rcP.top)
            {
                Blue();
                MoveTo(x,y);
                PaintString (scale_buf);
            }
          
        }
      else if (!(scale_pos % (pos/2)))
        {
          Black(); 
          x = rcP.left - 10;
          y = rcP.top - VFrom + scale_pos;
          MoveTo (x, y);
          LineTo (x-7,y);
        }
      else if (!(scale_pos % (pos/10)))
        {
          Black();
          x = rcP.left - 10;
          y = rcP.top - VFrom + scale_pos;
          MoveTo (x, y);
          LineTo (x-5,y);
        }
    }
  Black();
}


/*_______________________________________________(DOT_UpdateLRBT)________________

  Purpose : Computes Left, Right, Bottom and Top values for DisplayHits.

____________________________________________________________________*/
void DOT_UpdateLRBT (RecT r, RecT rcP, Int4Ptr Left, Int4Ptr Right, Int4Ptr Bottom, Int4Ptr Top)
{
  
  if (r.left > rcP.left)
    *Left = r.left;
  else
    *Left = rcP.left;
  
  if (r.right < rcP.right)
    *Right = r.right;
  else
    *Right = rcP.right;
  
  if (r.bottom < rcP.bottom)
    *Bottom = r.bottom;
  else
    *Bottom = rcP.bottom;
  
  if (r.top > rcP.top)
    *Top = r.top;
  else
    *Top = rcP.top;
  
  return;
}
/*________________________________________(DOT_AddRectMargins)_____________

  Purpose : Add horizontal and vertical margins to rect, window1.

____________________________________________________________________*/
static void DOT_AddRectMargins (RectPtr r, DOTVibDataPtr vdp)
{
  r->left += vdp->HORZ_MARGIN;
  r->top += vdp->VERT_MARGIN;

  return;
}

/*____________________________________________(DOT_ChangeMainViewerCutoff)______

  Purpose : Change cutoff function for threshold-ramp, window1.

____________________________________________________________________*/
static void DOT_ChangeMainViewerCutoff (BaR b, GraphiC g, Int2 new, Int2 old) 
{
  DOTVibDataPtr vdp;
  WindoW     w, temport;
  RecT       rcP;

  vdp = (DOTVibDataPtr) GetObjectExtra (b);

  Select(vdp->panel);
  ObjectRect(vdp->panel, &rcP);
  InsetRect(&rcP,4,4);

  w = (WindoW)ParentWindow(vdp->panel);
  temport = SavePort(w);

  vdp->sdp.TrampPos = new+20;
  
  DOT_AddRectMargins(&rcP, vdp);
  InvalRect (&rcP);
  RestorePort (temport);
  Update();

}

static PoinT curpnt;
static PoinT fstpnt;

/*________________________________________(DOT_RectOverlpRect)_____________

  Purpose : Find overlapping region between two rects.

____________________________________________________________________*/
static Boolean DOT_RectOverlpRect (RectPtr r1, RectPtr r2)
{
  if (r1->left <r2->left)
    r1->left = r2->left;
  if (r1->right >r2->right)
    r1->right = r2->right;
  if (r1->top < r2->top)
    r1->top = r2->top;
  if (r1->bottom > r2->bottom)
    r1->bottom = r2->bottom;

  return TRUE;
}

/*________________________________________(DOT_RectIntsRect)_____________

  Purpose : Checks if selected rect is in selectable rect, window1.

____________________________________________________________________*/
static Boolean DOT_RectIntsRect (RectPtr r1, RectPtr r2)
{
  
  if (r1->top > r2->bottom || r1->bottom < r2->top || r1->left > r2->right || r1->right < r2->left)
    return FALSE;
  else
    return TRUE;
}

/*________________________________________(DOT_LoopStop)_____________

  Purpose : calculate stop value in score_array for threshold-ramp.

____________________________________________________________________*/
static Int4 DOT_LoopStop (DOTVibDataPtr vdp)
{
  Int4   stop, pos;

  pos=ceil((double)(vdp->sdp.TrampPos*vdp->mip->unique)/100);

  if (pos>=vdp->mip->unique)
    return vdp->mip->index;
  else
    stop=vdp->mip->score_array[pos]+1;

  return stop;
}


/*_______________________________________________(DOT_DisplayHits)_________

Purpose : Draw function for window1.

____________________________________________________________________*/

static void DOT_DisplayHits (PaneL p)
{

  Int4           i, x, y, x2, y2, index;
  RecT           rcP, rupd, rcS, rcs, dr, rcR, rcP_off;
  Int4           Xscore, Yscore, comp, stop;
  Uint1Ptr PNTR  scores;
  DOTVibDataPtr    vdp=NULL;
  DOTMainDataPtr   mip=NULL;
  DOTSelDataPtr      data;
  WindoW         w;
  BaR            Vsb, Hsb;
  Int4           VFrom, HFrom, VTo, HTo, cutoff;
  Boolean        x_axis_incr;
  Boolean        query_on_minus = FALSE;
  Boolean        q_hitonplus, s_hitonplus;
  Int4           q_start, s_start, length;
  Int4           s_stop, q_stop, ycomp, xcomp;
  Int4           swdt, shgt;
  DOTDiagPtr     PNTR hitlist;
  DOTAlnPtr      PNTR Alnlist;
  Int4           Left, Right, Bottom, Top;
  Int2           dx, dy;
  DOTAlnPtr      PNTR alnL;
  DOTAlnPtr      aln;
  Int4           q_left, q_right;
  Int4           s_top, s_bottom;

  
  w = (WindoW)ParentWindow(p);
  rupd = updateRect;
  ObjectRect(p, &rcP);
  InsetRect(&rcP,4,4); 
  ClipRect(&rcP);
  
  if (!(vdp = (DOTVibDataPtr)GetObjectExtra(w))) return;
  
  VFrom = vdp->sdp.VFrom; 
  HFrom = vdp->sdp.HFrom;
  VTo = (MIN (vdp->sdp.PgLen+VFrom, vdp->curr_slen-VFrom));
  HTo = (MIN (vdp->sdp.PgWdth+HFrom, vdp->curr_qlen-HFrom));
  
  comp = vdp->comp;
  
  DOT_AddRectMargins (&rcP, vdp);
  DOT_UpdateLRBT (rupd, rcP, &Left, &Right, &Bottom, &Top); 
  mip = vdp->mip;
  data = (DOTSelDataPtr)vdp->data;
  if (vdp->selectMode == dot_FEATVIEW)
    {
      if(data->selected)
        {
          DOT_VCrossHairs (rcP, vdp, VFrom, HFrom);
          DOT_HCrossHairs (rcP, vdp, VFrom, HFrom);
        }
    }
  else
    {
  if (data->selected) 
    {
      rcs=data->rcS;
      
      dx=HFrom-data->H_pos;
      dy=VFrom-data->V_pos;
      
      xcomp=vdp->originalcomp-comp;
      ycomp=xcomp;
 
      rcS.left=rcs.left-dx;
      rcS.right=rcs.right-dx;
      rcS.top=rcs.top-dy;
      rcS.bottom=rcs.bottom-dy;
   
      rcR.left = Left;
      rcR.right = Right;      
      rcR.top = Top;
      rcR.bottom = Bottom;
      
      if (DOT_RectIntsRect(&rcS, &rcR))
        {
          rcP_off = rcP;
          DOT_RectOverlpRect(&rcS, &rcR);
          SectRect (&rcS, &rcP_off, &dr);
          Yellow();
          PaintRect (&dr);
          Black();
        }
    }
    }
  DOT_DrawXGrids(rupd, rcP, vdp, VFrom, HFrom, HTo, comp, vdp->showGrid);
  DOT_DrawYGrids(rupd, rcP, vdp, VFrom, HFrom, VTo, comp, vdp->showGrid);
  
  if (vdp->showDotPlot && mip)
    {
      hitlist = mip->hitlist;
      if (mip->unique<=1)
        stop = mip->index;
      else
        stop = DOT_LoopStop (vdp);

      if (vdp->strand1 == Seq_strand_minus)
        query_on_minus = TRUE;

      for (i = 0; i<stop ; i++)
        {       
          length = hitlist[i]->length;

          if (query_on_minus)
            q_start = mip->q_stop - hitlist[i]->q_start;
          else
            q_start = hitlist[i]->q_start - mip->q_start;
          s_start = hitlist[i]->s_start - mip->s_start;
          q_start = ((int)q_start/comp)-HFrom;
          s_start = ((int)s_start/comp)-VFrom;
          length = (int)length/comp; 

          if (query_on_minus){
            x = rcP.left + q_start;
            x2 = x - length;
            q_left = x2;
            q_right = x;
          }
          else {
            x = rcP.left + q_start;
            x2 = x + length;
            q_left = x;
            q_right = x2;
          }

          y = rcP.top +  s_start;
          y2 = y + length; 
          
          if (y > Bottom || y2 < Top  || q_left > Right || q_right < Left) 
            continue; /* outside of drawing Rgn */
          
          if (query_on_minus){ 
            if (q_left < rcP.left)
              { 
                y2 = y2 - (rcP.left - q_left);
                q_left = rcP.left; 
              } 
          }
          else { 
            if (q_left < rcP.left) { 
              y = y - (rcP.left - q_left); 
              q_left = rcP.left;
            }
          }

          if (y < rcP.top) {
            q_left = q_left - (rcP.top - y);
            y = rcP.top; 
          } 
 
          if (y < rcP.top) 
            {
              q_left = q_left + (rcP.top-y);
              y = rcP.top;
            } 
          
          if (q_left < rcP.left)
            { 
              y = y+(rcP.left-q_left);  
              q_left = rcP.left; 
            }
          
          if (query_on_minus)
            x2 = q_left; 
          else  
            x = q_left;
          
          MoveTo(x, y);
          LineTo(x2, y2);
          
        }
    }

  if (vdp->showALIGN && vdp->alp) /* overlay Blast hits */
    {
      Red();
      alnL = vdp->alp->Alnlist;
      stop = vdp->alp->index;
      for (i = 0; i<stop; i++)
        {
          aln = alnL[i];

          q_start=ABS(vdp->xstart - aln->q_start);
          s_start=ABS(vdp->ystart - aln->s_start);
          q_start=q_start/comp - HFrom;
          s_start=s_start/comp - VFrom;
          length = ABS(aln->q_stop - aln->q_start)+1;
          length = length/comp;

          if (vdp->strand1==Seq_strand_minus){
            q_hitonplus=FALSE;
            x = rcP.left + q_start;
            x2 = x - length;
            q_left = x2;
            q_right = x;
          }
          else{
            q_hitonplus=TRUE;
            x = rcP.left + q_start;
            x2 = x + length;
            q_left = x;
            q_right = x2;
          }

          if (vdp->strand2==Seq_strand_minus){
            s_hitonplus=FALSE;
            y = rcP.top + s_start;
            y2 = y - length;
            s_top = y2;
            s_bottom = y;
          }
          else {
            s_hitonplus=TRUE;
            y=rcP.top+s_start;
            y2=y+length;
            s_top = y;
            s_bottom = y2;
          }
         
          if (q_left > Right || q_right < Left ||
              s_top > Bottom || s_bottom < Top)
            continue;

          if (q_hitonplus==s_hitonplus){
            if (x<rcP.left)
              {
                y=y+(rcP.left-x);
                x=rcP.left;
              }
            if (y < rcP.top) 
              {
                x = x+(rcP.top-y);
                y=rcP.top;
              }
          }
          else{
            if (q_hitonplus){ /* s is minus */
              if (x< rcP.left)
                {
                  y = y-(rcP.left-x); 
                  x = rcP.left;
                }
            }
            else { /* q is on minus strand */
              if (x2< rcP.left)
                {
                  y2=y2-(rcP.left-x2);
                  x2 = rcP.left;
                }
            }
            if (s_hitonplus){ /* q is minus */
              if (y < rcP.top) 
                {
                  x = x-(rcP.top-y);
                  y=rcP.top;
                }
            }
            else {/* s is on minus strand */
              if (y2 < rcP.top) 
                {
                  x2=x2-(rcP.top-y2);
                  y2=rcP.top;
                }
            }
          }
          DOT_SetColor(NULL, aln->class, TRUE);
          MoveTo(x, y);
          LineTo(x2, y2);
        }
      Black();
    }
  
  ResetClip();
  return;
}


/*________________________________________(DOT_SetCurrSeqlen)_____________


  Purpose : Update displayed seq-length after reduce/enlarge functions.

____________________________________________________________________*/
  /*set the current size of the sequence display */
static void DOT_SetCurrSeqlen (DOTVibDataPtr vdp)
{
  Int4 comp;
  
  comp = vdp->comp;

    vdp->curr_slen = (vdp->ylen)/comp;
    vdp->curr_qlen = (vdp->xlen)/comp;

}


/*________________________________________(DOT_VScrlUpdate)_____________

  Purpose : Update function for vertical scroll proc, window1.

____________________________________________________________________*/

static void DOT_VScrlUpdate(DOTVibDataPtr vdp, BaR vsb, Int4 VCurPos)
{

  VCurPos = VCurPos*vdp->sdp.UnitY;

  /*set cursor position to new Units */
  VCurPos = VCurPos/vdp->sdp.UnitY;

  if (VCurPos<0) VCurPos=0;
  
  if (VCurPos >= vdp->sdp.YScrlMax)
    vdp->sdp.YScrlPos = vdp->sdp.YScrlMax;
  else 
    vdp->sdp.YScrlPos = VCurPos;
  
  vdp->sdp.VFrom=vdp->sdp.YScrlPos*vdp->sdp.UnitY;

  /*update scroll*/
  CorrectBarMax(vsb, vdp->sdp.YScrlMax);
  CorrectBarValue(vsb, vdp->sdp.YScrlPos);
  CorrectBarPage(vsb, vdp->sdp.YScrlPage, vdp->sdp.YScrlPage);
  
}


/*________________________________________(DOT_HScrlUpdate)_____________

  Purpose : Update function for horizontal scroll proc.

____________________________________________________________________*/

static void DOT_HScrlUpdate(DOTVibDataPtr vdp, BaR hsb, Int4 HCurPos)
{  
  HCurPos = HCurPos*vdp->sdp.UnitX;

  /*set cursor position to new Units */
  HCurPos = HCurPos/vdp->sdp.UnitX;

  if (HCurPos<0) HCurPos=0;
  
  if (HCurPos >= vdp->sdp.XScrlMax)
    vdp->sdp.XScrlPos = vdp->sdp.XScrlMax;
  else 
    vdp->sdp.XScrlPos = HCurPos;
  
  vdp->sdp.HFrom=vdp->sdp.XScrlPos*vdp->sdp.UnitX;

  /*update scroll*/
  CorrectBarMax(hsb, vdp->sdp.XScrlMax);
  CorrectBarValue(hsb, vdp->sdp.XScrlPos);
  CorrectBarPage(hsb, vdp->sdp.XScrlPage, vdp->sdp.XScrlPage);
  
}



/*________________________________________________(DOT_VscrlProc)_____________

  Purpose : Vertical Scroll proc, window1.

____________________________________________________________________*/
static void DOT_VscrlProc (BaR vsb, SlatE s, Int2 newval, Int2 oldval)
{
  WindoW 		temport, w;
  DOTVibDataPtr   vdp;
  RecT         rcP;
  Int2         dy, offset;
  Int2         visLines, vmargin, hmargin;
  PaneL        p;


  w = (WindoW)ParentWindow((PaneL)s);
  vdp = (DOTVibDataPtr)GetObjectExtra (w);
  p = vdp->panel;
  
  offset=vdp->sdp.HFrom;
  vmargin=vdp->VERT_MARGIN;
  hmargin=vdp->HORZ_MARGIN;

  temport = SavePort (w);

  Select(p);
  ObjectRect(p, &rcP);
  InsetRect(&rcP, 4, 4);
  ClipRect(&rcP);

  vdp->sdp.YScrlPos = newval;
  visLines = vdp->sdp.YScrlPage;
  vdp->sdp.VFrom=newval*vdp->sdp.UnitY;

  rcP.right = (MIN (rcP.left+hmargin+vdp->curr_qlen-offset, rcP.right));
  rcP.top += vmargin;

  dy = newval- oldval;
  if (ABS(dy) < vdp->sdp.YScrlPage)
    {
      ScrollRect(&rcP,  0, (Int2)((-dy)*vdp->sdp.UnitY));
    }
  else
    {
      InsetRect(&rcP, -1, -1);
      InvalRect(&rcP);
    }
  ResetClip();
  RestorePort(temport);
/*   Update(); */
  
}



/*________________________________________________(DOT_HscrlProc)_____________

  Purpose : Horizontal scroll proc, window1.

____________________________________________________________________*/
static void DOT_HscrlProc (BaR Hsb, SlatE s, Int2 newval, Int2 oldval)
{
  WindoW 		temport, w;
  DOTVibDataPtr   vdp;
  RecT         rcP;
  Int2         dx, visLines, offset, hmargin, vmargin;

  
  w = (WindoW)ParentWindow((PaneL)s);
  vdp = (DOTVibDataPtr)GetObjectExtra (w);

  temport = SavePort (w);
  Select(vdp->panel);
  ObjectRect(vdp->panel, &rcP);
  InsetRect(&rcP, 4, 4);
  ClipRect (&rcP);

  offset=vdp->sdp.VFrom;
  hmargin=vdp->HORZ_MARGIN;
  vmargin=vdp->VERT_MARGIN;

  rcP.bottom = (MIN (rcP.top+vmargin+vdp->curr_slen-offset, rcP.bottom));
  rcP.left += hmargin;

  vdp->sdp.XScrlPos = newval;
  visLines = vdp->sdp.XScrlPage;
  vdp->sdp.HFrom=newval*vdp->sdp.UnitX;

  dx = newval - oldval;

  if (ABS(dx) < vdp->sdp.XScrlPage)
    {
      ScrollRect(&rcP, (Int2)((-dx)*vdp->sdp.UnitX) , 0);
    }
  else
    {
      InsetRect(&rcP, -1, -1);
      InvalRect(&rcP);
    }
  
  ResetClip();
  RestorePort(temport);
/*   Update(); */
  
}


/*________________________________________(DOT_UpdateMainPanel)_____________

  Purpose : Update selection for window1.

____________________________________________________________________*/

static void DOT_UpdateMainPanel(DOTVibDataPtr vdp, Boolean update_all)
{
  WindoW     temport;
  RecT       rc;
  DOTSelDataPtr  data;
  Int4           dx, dy;

  data=(DOTSelDataPtr)vdp->data;

  temport = SavePort((WindoW)ParentWindow(vdp->panel));
  Select(vdp->panel);
  ObjectRect(vdp->panel, &rc);
  ClipRect(&rc);
  
  if (!update_all)
    {
      if (data->rm_lastselected)
        { 
          InsetRect(&data->old_rcS, -1, -1);
          InvalRect(&data->old_rcS);
          data->rm_lastselected=FALSE;
        }
      dx=data->H_pos-vdp->sdp.HFrom;
      dy=data->V_pos-vdp->sdp.VFrom;
      rc.left=data->rcS.left-dx;
      rc.right=data->rcS.right-dx;
      rc.top=data->rcS.top-dy;
      rc.bottom=data->rcS.bottom-dy;
      InsetRect(&rc, -1, -1);
      InvalRect (&rc);
    }
  else
    {
      InsetRect(&rc, -1, -1);
      InvalRect(&rc);
    }

  ResetClip();
  RestorePort(temport);
  Update();

}


/*______________________________________(DOT_VScroll)___________

  Purpose : Correct vertical scroll bar values, window1.

____________________________________________________________________*/
static void DOT_VScroll (DOTVibDataPtr vdp, BaR vsb)
{
 
  CorrectBarMax(vsb,vdp->sdp.YScrlMax);
  CorrectBarValue(vsb,vdp->sdp.YScrlPos);
  CorrectBarPage(vsb, vdp->sdp.YScrlPage, vdp->sdp.YScrlPage);

}
/*______________________________________(DOT_HScroll)___________

  Purpose : Correct horizontal scroll bar values, window1.

____________________________________________________________________*/
static void DOT_HScroll (DOTVibDataPtr vdp, BaR hsb)
{

  CorrectBarMax(hsb,vdp->sdp.XScrlMax);
  CorrectBarValue(hsb,vdp->sdp.XScrlPos);
  CorrectBarPage(hsb,vdp->sdp.XScrlPage, vdp->sdp.XScrlPage);

}
/*________________________________________(DOT_ComputePanelSize)_____________

  Purpose : Calculate panel size for scrolling functions, window1.

____________________________________________________________________*/
static void DOT_ComputePanelSize (RecT rcP, DOTVibDataPtr vdp, Int4Ptr PgWdth, Int4Ptr PgLen)
{

  InsetRect(&rcP, 4, 4);
  rcP.left += vdp->HORZ_MARGIN;
  rcP.top += vdp->VERT_MARGIN;

  *PgWdth =rcP.right-rcP.left;
  *PgLen  =rcP.bottom-rcP.top;

}

/*________________________________________(DOT_SetScrlVals)_____________

  Purpose : Set scroll values for window1.

____________________________________________________________________*/
static void DOT_SetScrlVals (DOTVibDataPtr vdp)
{
/*   Int4  scrollfctr = 40; */

/*   vdp->sdp.UnitY = vdp->sdp.PgLen/scrollfctr; */
  vdp->sdp.UnitY = 16; /*  constant value*/
  vdp->sdp.TotUnitsY = vdp->curr_slen/vdp->sdp.UnitY;
  vdp->sdp.YScrlPage = vdp->sdp.PgLen/vdp->sdp.UnitY;
  vdp->sdp.YScrlMax = vdp->sdp.TotUnitsY-(vdp->sdp.YScrlPage - vdp->VERT_MARGIN/vdp->sdp.UnitY);

/*   vdp->sdp.UnitX = vdp->sdp.PgWdth/scrollfctr; */
  vdp->sdp.UnitX = 16;/*  constant value*/
  vdp->sdp.TotUnitsX = vdp->curr_qlen/vdp->sdp.UnitY;
  vdp->sdp.XScrlPage = vdp->sdp.PgWdth/vdp->sdp.UnitX;
  vdp->sdp.XScrlMax = vdp->sdp.TotUnitsX-(vdp->sdp.XScrlPage - vdp->HORZ_MARGIN/vdp->sdp.UnitX);

  /* image is smaller than page size */
  if ((vdp->sdp.YScrlPage + vdp->HORZ_MARGIN/vdp->sdp.UnitX) > vdp->sdp.TotUnitsY)
    {
      vdp->sdp.YScrlMax = 0;
      vdp->sdp.YScrlPage = 0;
      vdp->sdp.YScrlPos = 0;
    }
  if (vdp->sdp.XScrlPage>vdp->sdp.TotUnitsX)
    {
      vdp->sdp.XScrlMax = 0;
      vdp->sdp.XScrlPage = 0;
      vdp->sdp.XScrlPos = 0;
    }
 
}

/*______________________________________(DOT_SetUpWin)___________

  Purpose : Scrolling info setup function for window1.

____________________________________________________________________*/
static void DOT_SetUpWin(WindoW w, PaneL p, DOTVibDataPtr vdp)
{

  Int4       Vcurpos, Hcurpos, height, width, viewersize, len;
  Int4       gap, lmargin, vsbWidth, hsbHeight;
  BaR        vsb, hsb;
  WindoW     temport;
  RecT       rcP, rcW, rcHsb, rcVsb;



  temport = SavePort (w);
  Select(p);
  ObjectRect(p, &rcP);

 /* Reset Panel Parameters */
  ObjectRect(w, &rcW);
  width = rcW.right-rcW.left;
  height = rcW.bottom-rcW.top;
  vsb = GetSlateVScrollBar ((SlatE) p);
  hsb = GetSlateHScrollBar ((SlatE) p);
  
  GetPosition(vsb,&rcVsb);
  GetPosition(hsb,&rcHsb);

  gap=2;
  lmargin=10;
  vsbWidth=rcVsb.right-rcVsb.left;
  hsbHeight=rcHsb.bottom-rcHsb.top;
    
  rcP.right = width - vsbWidth - gap;
  rcP.bottom = height - hsbHeight - gap;
  rcP.left=lmargin;

  SetPosition (vdp->panel, &rcP);
  AdjustPrnt (vdp->panel, &rcP, FALSE);

  viewersize=MIN(rcP.right-rcP.left,rcP.bottom-rcP.top)-vdp->HORZ_MARGIN;
  len=MAX(vdp->xlen, vdp->ylen);
  vdp->comp=DOT_Compression(len, viewersize);
  vdp->originalcomp=vdp->comp;
 

  vdp->sdp.YScrlPos = 0;
  vdp->sdp.XScrlPos = 0;

  DOT_SetCurrSeqlen (vdp);
  DOT_ComputePanelSize(rcP, vdp, &(vdp->sdp.PgWdth), &(vdp->sdp.PgLen));
  DOT_SetScrlVals(vdp);

  DOT_VScroll (vdp, vsb);
  DOT_HScroll (vdp, hsb);

  RestorePort(temport);

}




/*________________________________________(DOT_CloseSequenceWindow)_____________

  Purpose : Close function for Sequence Window.

____________________________________________________________________*/
static void  DOT_CloseSequenceWindow (ButtoN b)
{

  DOTVibDataPtr vdp2, vdp;
  DOTSelDataPtr  data;

  Uint1Ptr   qseq, sseq;

  vdp2=(DOTVibDataPtr)GetObjectExtra(ParentWindow(b));

  data=(DOTSelDataPtr)vdp2->data;
  data->selected=FALSE;
  vdp=data->vdp;
  SetTitle(vdp->Infopanel, vdp->iInfo);

  DOT_UpdateMainPanel(vdp, FALSE);
 
  vdp2->sv->pict1=DeletePicture(vdp2->sv->pict1);
  vdp2->sv->pict2=DeletePicture(vdp2->sv->pict2);
  
  if (vdp2->sv->salp)
    MemFree(vdp2->sv->salp);
  if (vdp2->sv) MemFree(vdp2->sv);
  if (vdp2->mip){
    DOT_FreeMainInfo(vdp2->mip);
    if (vdp2->mip) MemFree(vdp2->mip);
  }
  if (vdp2) MemFree(vdp2);
  Remove (vdp->ChildWin);
  vdp->ChildWin=NULL;

  prev_primID = 0;
}



/*_____________________________________________________________________

  Purpose : Remove feature linked list

____________________________________________________________________*/
static void DOT_FreeFeatPointers(DOTRowPtr drp)
{
  DOTFeatPtr dfp_temp=NULL, dfp=NULL;

  dfp=drp->dfp;
  if (dfp == NULL) return;
  dfp_temp=dfp;
  dfp=dfp->next;
  while(dfp != NULL)
    {
      dfp_temp=MemFree(dfp_temp);
      dfp_temp = dfp;
      dfp = dfp->next;
    }
  if (dfp_temp!=NULL) MemFree(dfp_temp);

}

/*________________________________________(DOT_CloseFeatWindow)_____________

  Purpose : Close function for Feature Window.

____________________________________________________________________*/
static void  DOT_CloseFeatWindow (IteM i)
{
  DOTFeatListPtr flp;
  DOTSelDataPtr  data;


      flp=(DOTFeatListPtr)GetObjectExtra(ParentWindow(i));
      if (!flp) return;

      data=(DOTSelDataPtr)flp->data;
      data->selected=FALSE;
      DOT_UpdateMainPanel(data->vdp, TRUE);

      flp->segQuery=DeletePicture(flp->segQuery);
      flp->segSubject=DeletePicture(flp->segSubject);
      DOT_FreeFeatPointers(flp->query_drp);
      DOT_FreeFeatPointers(flp->subject_drp);
      if (flp->featindex) flp->featindex=MemFree(flp->featindex);
      if (flp->query_drp) flp->query_drp=MemFree(flp->query_drp);
      if (flp->subject_drp)flp->subject_drp=MemFree(flp->subject_drp);
      if (flp->FeatWin) flp->FeatWin=Remove (flp->FeatWin);
      data->vdp->ChildWin=NULL;
      if (flp) flp=MemFree(flp);
     
}

/*_____________________________________________________________________

  Purpose : Remove 'sequence' or 'feature' window

____________________________________________________________________*/

static WindoW DOT_ClearLastWindow(WindoW w, Boolean is_sequence)
{
  DOTVibDataPtr vdp;
  DOTFeatListPtr flp;
  DOTSelDataPtr  data;

  if (!w) return w;

  if (is_sequence)
    {
      vdp=(DOTVibDataPtr)GetObjectExtra(w);
      if (!vdp) return w;

      data=(DOTSelDataPtr)vdp->data;
      data->selected=FALSE;

      if(vdp->sv->v1) DeleteViewer(vdp->sv->v1);
      if(vdp->sv->v2) DeleteViewer(vdp->sv->v2);
      if(vdp->sv->pict1) DeletePicture(vdp->sv->pict1);
      if(vdp->sv->pict2) DeletePicture(vdp->sv->pict2);
      vdp->sv=MemFree(vdp->sv);

/*       if (vdp->mip)  */
/*         { */
/*           DOT_FreeMainInfo(vdp->mip); */
/*           MemFree(vdp->mip); */
/*         } */

    }
  else
    {
      flp=(DOTFeatListPtr)GetObjectExtra(w);
      if (!flp) return w;

      data=(DOTSelDataPtr)flp->data;
      data->selected=FALSE;

      DeletePicture(flp->segQuery);
      DeletePicture(flp->segSubject);
     /*  DOT_FreeFeatPointers(flp->query_drp); */
/*       DOT_FreeFeatPointers(flp->subject_drp); */
/*       MemFree(flp->query_drp); */
/*       MemFree(flp->subject_drp); */
      MemFree(flp);
    }

  DOT_UpdateMainPanel(data->vdp, TRUE);

  w=Remove(w);

  return NULL;
 
}
/*_____________________________________________________________________

  Purpose : New set of function to show features on dotviewer

____________________________________________________________________*/

static void DOT_ModeProc(ChoicE i)
{
  WindoW      w, temport;
  RecT        rcP;
  DOTVibDataPtr  vdp;
  DOTSelDataPtr  data;
  

  w = (WindoW)ParentWindow(i);
  temport=SavePort(w);
  vdp = (DOTVibDataPtr)GetObjectExtra (w);
  if (!vdp) return;

  if (vdp->ChildWin !=NULL)
    {
      if (vdp->selectMode == 1)
        vdp->ChildWin=DOT_ClearLastWindow(vdp->ChildWin, TRUE);
      else
        vdp->ChildWin=DOT_ClearLastWindow(vdp->ChildWin, FALSE);
    }

  vdp->selectMode = GetValue(i);
  data=(DOTSelDataPtr)vdp->data;
  data->selected=FALSE;
  ObjectRect(vdp->panel, &rcP);
  Select(vdp->panel);
  InvalRect(&rcP);
  RestorePort(temport);
  Update();

}

static void DOT_VCrossHairs (RecT rcP, DOTVibDataPtr vdp, Int4 VFrom,Int4 HFrom)
{
  Int4   y, y2, x, x2, Vseq;
  Int4   cursor_size =15;
  Char   seq_pos[15];
/*   SelectPtr c_data; */

/*   c_data = cip->data; */

  /* vertical line */
  y = rcP.top-vdp->VERT_MARGIN+20;
  y2= rcP.bottom-1/* MIN(rcP.bottom - 1 , VTo) */;
  x2 = x = curpnt.x/* +HFrom */;
/*   x2 = x = rcP.left +c_h.x -cip->HORZ_MARGIN - cursor_size; */
  if (x>=rcP.left)
    {
      Magenta();
      MoveTo (x, y);
      LineTo (x2, y2);

    }
  Black();
  return;
  
}

static void DOT_HCrossHairs (RecT rcP, DOTVibDataPtr vdp, Int4 VFrom,Int4 HFrom)
{
  Int4  y, y2, x, x2;
  Int4  cursor_size = 15;
  Char  seq_pos[15];
/*   SelectPtr  c_data; */

/*   data = vdp->data; */

  /* horizontal line */
    y2 = y = curpnt.y /* + VFrom */;
/*   y2 = y = rcP.top + c_h.y  -cip->VERT_MARGIN - cursor_size; */
  x = rcP.left-vdp->HORZ_MARGIN+20;
  x2 = rcP.right-1/* MIN(rcP.right -1, HTo) */;
  if (y>=rcP.top)
    {
      Magenta ();
      MoveTo(x, y);
      LineTo (x2, y2);
    }
  Black();
  return;
  
}


static void DOT_MoveCrossHairs (RecT rcP, DOTVibDataPtr vdp)
{
  Int4   y, y2, x, x2;
  Int4   cursor_size = 15;
  Char   seq_pos[15];
  DOTSelDataPtr  data;

  SelectFont(vdp->Fnt);

  data = (DOTSelDataPtr) vdp->data;
  DOT_AddRectMargins(&rcP, vdp);

  /* vertical line */
  y = rcP.top-vdp->VERT_MARGIN+20;
  y2= rcP.bottom-1;
  x2 = x = curpnt.x;
  if (x>rcP.left)
    {
      MoveTo (x, y);
      LineTo (x2, y2);
    }

  /* horizontal line */
  y2 = y = curpnt.y;
  x = rcP.left-vdp->HORZ_MARGIN +20;
  x2 = rcP.right-1;
  if (y>rcP.top)
    {
      MoveTo(x, y);
      LineTo (x2, y2);

    }

  Black();


  return;
  
}


static void DOT_SelectLineProc (PaneL p)

{
  RecT        rcP;
  DOTSelDataPtr  data;
  DOTVibDataPtr  vdp;

  vdp = (DOTVibDataPtr) GetObjectExtra (ParentWindow(p));
  Dotted ();
  ObjectRect (p, &rcP);
  InsetRect (&rcP, 4, 4);
  DOT_MoveCrossHairs (rcP, vdp); 
}


/*________________________________________(DOT_SelectFrameProc)_____________

  Purpose : select frame for click and drag functions of window1.

____________________________________________________________________*/
static void DOT_SelectFrameProc (PaneL p)

{
  RecT  dr;
  RecT  or;
  RecT  r;

  Dotted ();
  ObjectRect (p, &or);
  InsetRect (&or, 2, 2);
  LoadRect (&r, fstpnt.x, fstpnt.y, curpnt.x, curpnt.y);
  SectRect (&r, &or, &dr);
  FrameRect (&dr);
  
}

/*________________________________________(DOT_DrawXAxis)_____________

  Purpose : Draw x-axis function for viewer1, window2.

____________________________________________________________________*/
void DOT_DrawXAxis(SegmenT seg2, RecT  r, Int4 height, Int4 xstart,Int4 xstop, Int4 scale, SeqIdPtr sip)
{
  Int4         pos, xlen, x, y, scale_pos, i, j, bigtick, midtick, smalltick;
  Char         scale_buf[15] = {""}, title[50]={""};	/*scale value*/
  Boolean      Decrement=FALSE;


  if (xstart>xstop)
    Decrement=TRUE;

  if (scale==0)
    scale=1;

  pos=100*scale;
  bigtick=10*scale;
  midtick=7*scale;
  smalltick=5*scale;
 

  xlen=ABS(xstop-xstart);

  /* axis label */ 
/*   SeqIdWrite(sip, title ,PRINTID_TEXTID_ACCESSION, 41); */
/*   AddLabel(seg2,r.left+xlen/2, height+25, title, SMALL_TEXT, 0, UPPER_CENTER, 0); */

  AddAttribute(seg2, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
  AddLine(seg2, r.left, height+10, r.left+xlen, height+10, FALSE,0);
/*   AddLine(seg2, r.left, height-ylen, r.left+xlen, height-ylen, FALSE, -1); */
  sprintf(scale_buf, "%d", xstop);
  AddAttribute(seg2, COLOR_ATT, RED_COLOR, 0,0,0,0);
  AddLabel(seg2, r.left+xlen+10*scale, height+5, scale_buf, SMALL_TEXT, 0, MIDDLE_RIGHT, 0);
  
  AddAttribute(seg2, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
  if (!Decrement)
    {
      for (scale_pos = xstart, i=0; scale_pos <= xstop; scale_pos++, i++)
        {
          
          if (!(scale_pos % pos))
            {
              x = r.left + i;
              y = height+10;
              AddAttribute(seg2, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
              AddLine(seg2, x, y, x, y+bigtick, FALSE,-1);
              sprintf(scale_buf, "%d", scale_pos);
              AddAttribute(seg2, COLOR_ATT, BLUE_COLOR, 0,0,0,0);
              AddLabel(seg2, x, y+15*scale, scale_buf, SMALL_TEXT, 0, UPPER_CENTER, 0);
              
            }
          else if (!(scale_pos % (pos/2)))
            {
              x = r.left + i;
              y = height+10;
              AddAttribute(seg2, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
              AddLine(seg2, x, y, x, y+midtick, FALSE,0);
              
            }
          else if (!(scale_pos % (pos/10)))
            {
              x = r.left + i;
              y = height+10;
              AddAttribute(seg2, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
              AddLine(seg2, x, y, x, y+smalltick, FALSE,0);
            }
        }
    }
  else
    {
      for (scale_pos = xstop, i=0, j=xstart; scale_pos <= xstart; scale_pos++, i++, j--)
        {
          
          if (!(scale_pos % pos))
                {
                  x = r.left + i;
                  y = height+10;
                  AddAttribute(seg2, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
                  AddLine(seg2, x, y, x, y+bigtick, FALSE,0);
                  sprintf(scale_buf, "%d", j);
                  AddAttribute(seg2, COLOR_ATT, BLUE_COLOR, 0,0,0,0);
                  AddLabel(seg2, x, y+15*scale, scale_buf, SMALL_TEXT, 0, UPPER_CENTER, 0);
                  
                }
              else if (!(scale_pos % (pos/2)))
                {
                  x = r.left + i;
                  y = height+10;
                  AddAttribute(seg2, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
                  AddLine(seg2, x, y, x, y+midtick, FALSE,0);
                  
                }
              else if (!(scale_pos % (pos/10)))
                {
                  x = r.left + i;
                  y = height+10;
                  AddAttribute(seg2, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
                  AddLine(seg2, x, y, x, y+smalltick, FALSE,0);
                }
        }
    }
  
}


/*________________________________________(DOT_DrawYAxis)_____________


  Purpose : Draw y-axis function for viewer1, window2.

____________________________________________________________________*/
void DOT_DrawYAxis(SegmenT seg2, RecT  r, Int4 height, Int4 ystart, Int4 ystop, Int4 scale, Int4 Fh, SeqIdPtr sip)
{
  Int4         smalltick, midtick, bigtick;
  Int4         pos, ylen, x, y, scale_pos, i, j, Fh_2,Fh_4; 
  Char         scale_buf[15] = {""}, title[50]={""};	/*scale value*/
  Boolean      Decrement = FALSE;


  if (ystart>ystop)
    Decrement=TRUE;

  if (scale==0)
    scale=1;

  pos=100*scale;
  smalltick=5*scale;
  midtick=7*scale;
  bigtick=10*scale;
 
  Fh_2=Fh/2;
  Fh_4=Fh/4;
  

  ylen=ABS(ystop-ystart);

/*   SeqIdWrite(sip, title ,PRINTID_TEXTID_ACCESSION, 41); */
/*   AddLabel(seg2, height - ylen/2, r.left - 20, title, SMALL_TEXT, 0, MIDDLE_CENTER, 0); */
 
  AddAttribute(seg2, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
  AddLine(seg2, r.left-10, height, r.left-10, height-ylen, FALSE, 0);

  sprintf(scale_buf, "%d", ystop);
  AddAttribute(seg2, COLOR_ATT, RED_COLOR, 0,0,0,0);
  AddLabel(seg2, r.left-10, height-ylen-10*scale, scale_buf, SMALL_TEXT, 0, MIDDLE_CENTER, 0);

  
  if (!Decrement)
    {
      for (scale_pos = ystart, i=0; scale_pos <= ystop; scale_pos++, i++)
        {
          if (!(scale_pos % pos))
            {
              x = r.left-10;
              y = height-i/* +VFrom */;
              AddAttribute(seg2, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
              AddLine(seg2, x, y, x-bigtick, y, FALSE, 0);
              
              sprintf(scale_buf, "%d", scale_pos);
              y = y-Fh_2;
              AddAttribute(seg2, COLOR_ATT, BLUE_COLOR, 0,0,0,0);
              AddLabel(seg2, x-15*scale, y, scale_buf, SMALL_TEXT, 0, MIDDLE_LEFT, 0);
              
            }
          else if (!(scale_pos % (pos/2)))
            {
              x = r.left-10;
              y = height-i/* +VFrom */;
              AddAttribute(seg2, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
              AddLine(seg2, x, y, x-midtick, y, FALSE,0);
              
            }
          else if (!(scale_pos % (pos/10)))
            {
              x = r.left-10;
              y = height-i/* +VFrom */;
              AddAttribute(seg2, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
              AddLine(seg2, x, y, x-smalltick, y, FALSE,0);
            }
          
        }
    }
  else
    {

      for (scale_pos = ystop, i=0, j=ystart; scale_pos <= ystart; scale_pos++, i++, j--)
        {
          if (!(scale_pos % pos))
            {
              x = r.left-10;
              y = height-i/* +VFrom */;
              AddAttribute(seg2, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
              AddLine(seg2, x, y, x-bigtick, y, FALSE,0);
              
              sprintf(scale_buf, "%d", j);
              y = y-Fh_2;
              AddAttribute(seg2, COLOR_ATT, BLUE_COLOR, 0,0,0,0);
              AddLabel(seg2, x-15*scale, y, scale_buf, SMALL_TEXT, 0, MIDDLE_LEFT, 0);
              
            }
          else if (!(scale_pos % (pos/2)))
            {
              x = r.left-10;
              y = height-i/* +VFrom */;
              AddAttribute(seg2, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
              AddLine(seg2, x, y, x-midtick, y, FALSE,0);
              
            }
          else if (!(scale_pos % (pos/10)))
            {
              x = r.left-10;
              y = height-i/* +VFrom */;
              AddAttribute(seg2, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
              AddLine(seg2, x, y, x-smalltick, y, FALSE,0);
            }     
        }
    }
}


/*________________________________________(DOT_DisplayDiags)_____________


  Purpose : Draw function for viewer1, window2.

____________________________________________________________________*/

static Boolean DOT_SVDisplayDiags(DOTVibDataPtr vdp2, DOTSelDataPtr data)
{
  RecT               rcP;
  DOTDiagPtr         PNTR hitlist;
  DOTVibDataPtr        vdp;
  VieweR             v;
  SegmenT            seg1, seg2, seg3, seg4;
  PrimitivE          prim;
  Char               buffer[50];
  Int4               stop, cutoff=0, q_start, s_start, length;
  Int4               x, y, x2, y2, tmp, Right_limit, Bottom_limit;
  Int4               i, j, s_stop, q_stop;
  Int4               p_VFrom, p_HFrom;
  Int4               width, height, primID;
  Int4               r_width, r_ht, x_start, y_start, x_stop, y_stop;
  Int4               right, left, top, bottom;
  Int4               Right, Left, Top, Bottom;
  Int4               lb_boundary, tr_boundary;
  Int4               right_end, bottom_end;
  Int4               diag, aln_diag;
  Int4               q_left, q_right;
  Int4               s_top, s_bottom;
  Boolean            q_hitonplus=FALSE, s_hitonplus=FALSE;
  Boolean            lt_fixed=FALSE, rb_fixed=FALSE;
  Boolean            lb_fixed=FALSE, rt_fixed=FALSE;
  Boolean            query_on_minus = FALSE;
  DOTAlnPtr PNTR     alnL, aln;

  seg1=CreateSegment(vdp2->sv->pict1, 1, 0); /* diags */
  seg2=CreateSegment(vdp2->sv->pict1, 2, 0); /* axis */
  seg3=CreateSegment(vdp2->sv->pict1, 3, 0); /* diag coordinates */
  v=vdp2->sv->v1;

  GetPosition(v, &rcP); 
  InsetRect(&rcP, 4, 4);
  vdp = data->vdp;
  width = rcP.right-rcP.left;
  height = rcP.bottom-rcP.top-2*(vdp2->VERT_MARGIN*vdp2->sv->scaleValue);
  p_VFrom=vdp->sdp.VFrom;
  p_HFrom=vdp->sdp.HFrom;
  /* Rect Parameters */
  rcP.left+=2*(vdp2->HORZ_MARGIN*vdp2->sv->scaleValue);
  DOT_DrawXAxis(seg2, rcP, height, data->q_start, data->q_stop, vdp2->sv->scaleValue, (vdp2->mip?vdp2->mip->qbsp->id:vdp2->alp->sip));
  DOT_DrawYAxis(seg2, rcP, height, data->s_start, data->s_stop, vdp2->sv->scaleValue, vdp2->Fh, (vdp2->mip?vdp2->mip->sbsp->id:vdp2->alp->sip->next));
  AddAttribute(seg1, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);


  Left=MIN(data->q_start, data->q_stop);
  Right=MAX(data->q_start, data->q_stop);
  Top=MIN(data->s_start, data->s_stop);
  Bottom=MAX(data->s_start, data->s_stop);
  right_end=rcP.left+(Right-Left);
  bottom_end=height-(Bottom-Top);

  if (vdp2->mip && vdp2->showDotPlot){
    hitlist = vdp2->mip->hitlist;
    if (vdp2->mip->unique<=1)
      stop=vdp2->mip->index;
    else
      stop=DOT_LoopStop (vdp2);
    
    if (vdp2->strand1 == Seq_strand_minus)
      query_on_minus = TRUE;
    
    for (i = 0; i<stop ; i++)
      {  
         length = hitlist[i]->length-1;
         if (query_on_minus) 
           x_start = data->q_stop - hitlist[i]->q_start;
         else
           x_start =  hitlist[i]->q_start - data->q_start;
         y_start =   ABS(data->s_start-  hitlist[i]->s_start);
         if (query_on_minus){ 
           x = rcP.left + x_start; 
           x2 = x - length; 
         } 
         else { 
           x = rcP.left + x_start;
           x2 = x + length;
         } 
         
         /* in dot matrix subject is always on plus */
         y = height- y_start;
         y2 = y - length;

         prim = AddLine(seg1, x, y, x2, y2, FALSE,(Uint2)i + 1);
         if (prev_primID == (i + 1))
           HighlightPrimitive(v, seg1, prim, FRAME_PRIMITIVE);
       }
  }

  if (vdp2->alp && vdp2->showALIGN){
    seg4=seg1;
    AddAttribute(seg4, COLOR_ATT, RED_COLOR, 0, 0, 0, 0);
    
    alnL=vdp2->alp->Alnlist;
    stop = vdp2->alp->index;
    for (j = 0; j<stop; j++)
      {
        aln = alnL[j];
        q_start=aln->q_start-Left;
        s_start=aln->s_start-Top;
        length=aln->q_stop-aln->q_start;

        if (vdp2->strand1 == Seq_strand_minus){
          q_hitonplus=FALSE;
          x = rcP.left+q_start;
          x2 = x - length;
          q_left = x2;
          q_right = x;
        }
        else{
          q_hitonplus=TRUE;
          x = rcP.left + q_start;
          x2 = x + length;
          q_left = x;
          q_right = x2;
        }

        if (vdp2->strand2 == Seq_strand_minus){
          s_hitonplus=FALSE;
          y = height - s_start;
          y2 = y + length;
          s_top = y2;
          s_bottom = y;
        }
        else{
          s_hitonplus=TRUE;
          y = height - s_start;
          y2 = y - length;
          s_top = y;
          s_bottom = y2;
        }
        
        lt_fixed = rb_fixed = lb_fixed = rt_fixed = FALSE;
        if (q_left > right_end || q_right < rcP.left ||
            s_top < bottom_end || s_bottom > height)
          continue;

        if (q_hitonplus==s_hitonplus){
          diag=Bottom-Right;
          aln_diag=s_start-q_start;
          if (x<rcP.left){
            if (y>height){
              if (rcP.left-x < y-height/* aln_diag < diag */){
                x=x+(y-height);
                y=height;
              }
              else{
                y=y-(rcP.left-x);
                x=rcP.left;
              }
            }
            else{
              y=y-(rcP.left-x);
              x=rcP.left;
            }
            lt_fixed=TRUE;
          }
          if (x2>right_end){
            if (y2<bottom_end){
              if (x2-right_end < bottom_end-y2/* aln_diag > diag */){
                x2=x2-(bottom_end-y2);
                y2=bottom_end;
              }
              else{
                y2=y2+(x2-right_end);
                x2=right_end;
              }
            }
            else {
              y2=y2+(x2-right_end);
              x2=right_end;
            }
            rb_fixed=TRUE;
          }
          
            if (y>height && !lt_fixed){
              x=x+(y-height);
              y=height;
            }
            if (y2<bottom_end && !rb_fixed){
              x2=x2-(bottom_end-y2);
              y2=bottom_end;
            }
          }
          else{
            if (q_hitonplus){/* s must be minus */
/*               if ( x > right_end || x2 < rcP.left || y2 <bottom_end || y>height) */
/*                 continue; */
              diag=Bottom-Left;
              aln_diag=s_start-q_start;
              if (x<rcP.left){
                if (y<bottom_end){
                  if (rcP.left-x < bottom_end-y/* aln_diag < diag */){
                    x=x+(bottom_end-y);
                    y=bottom_end;
                  }
                  else{
                    y=y+(rcP.left-x);
                    x=rcP.left;
                  }
                }
                else{
                  y=y+(rcP.left-x);
                  x=rcP.left;
                }
                lb_fixed=TRUE;
              }
              if (x2>right_end){
                if (y2>height){
                  if (x2-right_end < y2-height/* aln_diag > diag */){
                    x2=x2-(y2-height);
                    y2=height;
                  }
                  else{
                    y2=y2-(x2-right_end);
                    x2=right_end;
                  }
                }
                else{
                  y2=y2-(x2-right_end);
                  x2=right_end;
                }
                rt_fixed=TRUE;
              }
              if (y<bottom_end && !lb_fixed){
                x=x+(bottom_end-y);
                y=bottom_end;
              }
              if (y2>height && !rt_fixed){
                x2=x2-(y2-height);
                y2=height;
              }
            }
            else if (s_hitonplus){/* q must be minus*/
/*               if (y2 >height || y <bottom_end || x2<rcP.left || x>right_end) */
/*                 continue; */
              diag=Bottom-Left;
              aln_diag=s_start-q_start;
              if (x> right_end){
                if (y>height){
                  if (x-right_end < y-height/* aln_diag < diag */){
                    x=x-(y-height);
                    y=height;
                  }
                  else{
                    y=y-(x-right_end);
                    x=right_end;
                  }
                }
                else{
                  y=y-(x-right_end);
                  x=right_end;
                }
                lt_fixed=TRUE;
              }
              if(x2<rcP.left){
                if (y2<bottom_end){
                  if (rcP.left-x2 < bottom_end-y2/* aln_diag > diag */){
                    x2=x2+(bottom_end-y2);
                    y2=bottom_end;
                  }
                  else{
                    y2=y2+(rcP.left-x2);
                    x2=rcP.left;
                  }
                }
                else {
                  y2=y2+(rcP.left-x2);
                  x2=rcP.left;
                }
                rb_fixed=TRUE;
              }
              if (y>height && !lt_fixed){
                x=x-(y-height);
                y=height;
              }
              if (y2<bottom_end && !rb_fixed){
                x2=x2+(bottom_end-y2);
                y2=bottom_end;
              }
              
            }
          }
          DOT_SetColor(seg4, aln->class, FALSE);
          prim=AddLine(seg4, x, y, x2, y2, FALSE, 0);
          SetPrimitiveIDs(prim,aln->entityID,aln->itemID, OBJ_SEQALIGN, aln->primID);
          if (prev_primID == aln->primID)
            HighlightPrimitive(v, seg1, prim, FRAME_PRIMITIVE);
      }
  }
  
  return TRUE;
}

/*________________________________________(DOT_WorldtoScreen)_____________

  Purpose : calculate sequence coordinates from screen coords.

____________________________________________________________________*/
static Int4 DOT_WorldtoScreen(Int4 wPos, Int4 chw_2)
{
  Int4  sPos;

  sPos=wPos*chw_2;

  return sPos;
}

/*________________________________________(DOT_DrawScale)_____________

  Purpose : Draw scale of aligned seqs, viewer2, window2.

____________________________________________________________________*/
void DOT_DrawScale (SegmenT sbSeg, Uint1 strand1, Uint1 strand2, RecT  r, Int4 margin,Int4 res_cnt, Int4 bloc_cnt, Int4 Fh, Int4 q_pos, Int4 s_pos, Int4 chw_2, Int4 chw_4)
{
  Char      Buf[15]={""};
  Int4      x, x2,y1, y2, y3, y4, y5, y6, y7, y8;
  Int4      pos, col;


  col =DOT_WorldtoScreen(res_cnt+bloc_cnt, chw_2); 

  x = r.left+margin+col-chw_4;
  y1=7*Fh;
  y2=y1-Fh/2;
  y3=y2-Fh/2;
  AddAttribute(sbSeg, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
  AddLine(sbSeg, x, y2, x, y3, FALSE, 0);

  pos=(strand1==Seq_strand_plus)?res_cnt+q_pos:ABS(q_pos-res_cnt); 
  sprintf(Buf,"%d",pos);
  x2=x-StringWidth(Buf)/2;

  AddAttribute(sbSeg, COLOR_ATT, BLUE_COLOR, 0,0,0,0);
  AddLabel(sbSeg, x2,y1, Buf, SMALL_TEXT, 0, UPPER_RIGHT, 0);

  y4=2*Fh;
  y5=y4+Fh+Fh/2;
  y6=y5+Fh/2;
  AddAttribute(sbSeg, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
  AddLine(sbSeg, x, y6, x, y5, FALSE, 0);

  pos=(strand2==Seq_strand_plus)?res_cnt+s_pos:ABS(s_pos-res_cnt);
  sprintf(Buf,"%d",pos);
  x-=StringWidth(Buf)/2;

  AddAttribute(sbSeg, COLOR_ATT, BLUE_COLOR, 0,0,0,0);
  AddLabel(sbSeg, x2,y4, Buf, SMALL_TEXT, 0, UPPER_RIGHT, 0);

}

/*________________________________________(DOT_HlSeq)_____________

  Purpose : Draw line to show hit in alignment, viewer2, window2.

____________________________________________________________________*/
static void DOT_HlSeq(SegmenT sbSeg, RecT  rcP, Int4 margin, Int4 wPos, Int4 bloc_pos, Int4 hit_start, Int4 hit_stop, Int4 Fh, Int4 chw_2)
{
  Int4  xpos, len,  bloc_beg, begin, end;
  Int4  x1, x2, y1;


  bloc_beg = wPos-bloc_pos;
  begin = MAX(bloc_beg, hit_start);
  
  end = MIN(wPos,hit_stop);
  len = end - begin + 1;
  xpos = DOT_WorldtoScreen(begin, chw_2);
 
  x1= rcP.left + margin + xpos;
  x2= x1 + len * chw_2;
  y1 = 5 * Fh;

  AddAttribute(sbSeg, COLOR_ATT, RED_COLOR, 0,0,0,0);
  AddLine(sbSeg, x1, y1, x2, y1, FALSE,0); 

}

/*________________________________________(DOT_Highlight)_____________

  Purpose : Highlight matched residues, viewer2, window2.

____________________________________________________________________
*/
static void DOT_Highlight(SegmenT sbSeg, RecT  rcP, Int4 margin, Int4 wPos, Int4 Fh, Int4 chw_2, Boolean match)
{
  Int4  xpos, x1, x2, y1, y2;

  xpos=DOT_WorldtoScreen(wPos, chw_2);

  x1= rcP.left+margin+xpos;
  x2= x1+chw_2;
  y1 = 6*Fh;
  y2 = 4*Fh;

  if (match)
    AddAttribute(sbSeg, COLOR_ATT, YELLOW_COLOR,0,0,0,0);
  else
    AddAttribute(sbSeg, COLOR_ATT, CYAN_COLOR, 0,0,0,0);
  AddRectangle (sbSeg, x1, y1, x2, y2, 0, TRUE, 0);
}


/*________________________________________(DOT_UpdatePt)_____________

  Purpose : corrects clicked points for click/drag/release procs of window1.

____________________________________________________________________*/

static void DOT_UpdatePt (void)
{
  Int4    temp;

  if (curpnt.x < fstpnt.x)
    {
      temp = curpnt.x;
      curpnt.x = fstpnt.x;
      fstpnt.x = temp;
    }

  if (curpnt.y < fstpnt.y)
    {
      temp = curpnt.y;
      curpnt.y = fstpnt.y;
      fstpnt.y = temp;
    }

  return;
  
}

/*________________________________________(DOT_FillNewSeqBufs)_____________

  Purpose : Fill seq buffers for window2.

____________________________________________________________________*/
static void DOT_FillNewSeqBufs (DOTVibDataPtr vdp, DOTVibDataPtr vdp2, Boolean is_zoom)
{
  Int4      qlen, slen, i;
  Int4      q_start, s_start;
  Int4      q_left, q_right;
  Uint1Ptr  qseq=NULL, sseq=NULL, q=NULL, s=NULL, qBuf=NULL, sBuf=NULL;
  DOTSelDataPtr data=NULL;
  DOTMainDataPtr mip1=NULL, mip2=NULL;
  DOTAlignInfoPtr alp=NULL;


  data=(DOTSelDataPtr)vdp->data;
  mip1 =  vdp->mip;
  mip2 =  vdp2->mip;

  qlen=mip2->qlen;
  slen=mip2->slen;
  qseq = mip1->qseq;
  sseq = mip1->sseq;
  q_left=mip1->q_start;
  q_right = mip1->q_start + mip1->qlen - 1;
  s_start = mip1->s_start;
  if (is_zoom)
    {
      mip2->sseq=MemFree(mip2->sseq);
      mip2->qseq=MemFree(mip2->qseq);
    }
  
  
  /* get sequence position relative to the vdp sequence buffer */
  if (vdp2->strand1 == Seq_strand_minus)
    q_start = ABS(q_right - data->q_stop)/*  + q_left */;
  else
    q_start = ABS(q_left - data->q_start);

  q = qseq+ q_start;
  s = sseq+ ABS(s_start - data->s_start);


  if (!(qBuf = (Uint1Ptr) MemNew (sizeof(Uint1)*(qlen)))) return;
  if (!(sBuf = (Uint1Ptr) MemNew (sizeof(Uint1)*(slen)))) return;
  mip2->sseq=sBuf;
  mip2->qseq=qBuf;
  i=0;
  while(i< qlen)
    {        
      *qBuf=*q;
      i++;
      q++;
      qBuf++;
    }
  i=0;
  while(i< slen)
    {  
      *sBuf=*s;
      i++;
      s++;
      sBuf++;
    }
}

/*________________________________________(DOT_InitCInfo)_____________

  Purpose : Initialize vdp2 (DOTVibDataPtr for window2).

____________________________________________________________________*/

static void DOT_InitCInfo(DOTVibDataPtr vdp, DOTVibDataPtr vdp2, DOTSelDataPtr data)
{
  Char       colBuf[12]={""};
  Int4       i;
  Int4       q_left, q_right;
  DOTMainDataPtr mip1=NULL, mip2=NULL;
  BioseqPtr  qbsp, sbsp;
  

  /* set up second window parameters */  
  vdp2->sv=(DOTSeqViewrPtr)MemNew(sizeof(DOTSeqViewr));
  vdp2->Fnt = vdp->Fnt;
  vdp2->HORZ_MARGIN=vdp->HORZ_MARGIN;
  vdp2->VERT_MARGIN=vdp->VERT_MARGIN;
  vdp2->alp=vdp->alp;

  mip1 =  vdp->mip;
  if (mip1){
    qbsp = mip1->qbsp;
    sbsp = mip1->sbsp;
    mip2=(DOTMainDataPtr) MemNew (sizeof(DOTMainData));
    mip2=DOT_InitMainInfo (mip2, qbsp, sbsp, mip1->word_size, mip1->tree_limit, data->q_start, data->q_stop, data->s_start, data->s_stop);
    if (mip2->qslp)
      SeqLocFree(mip2->qslp);
    mip2->qslp = SeqLocIntNew(data->q_start, data->q_stop, vdp->strand1, qbsp->id);
    if (mip2->sslp)
      SeqLocFree(mip2->sslp);
    mip2->sslp = SeqLocIntNew(data->s_start, data->s_start, vdp->strand2, sbsp->id);
    mip2->qlen=data->qlen;
    mip2->slen=data->slen;
    vdp2->sdp.TrampPos=75;
    mip2->matrix = mip1->matrix;
    mip2->qstrand = mip1->qstrand;
    vdp2->mip= mip2;
  }
  else {
    vdp2->mip=NULL;
  }
  vdp2->xstart=data->q_start;
  vdp2->ystart=data->s_start;
  vdp2->xstop=data->q_stop;
  vdp2->ystop=data->s_stop;
  vdp2->xlen=data->qlen;
  vdp2->ylen=data->slen;
  vdp2->strand1=vdp->strand1;
  vdp2->strand2=vdp->strand2;
  vdp2->xname=vdp->xname;
  vdp2->yname=vdp->yname;
  /* sequence viewer initiatize */

  vdp2->sv->do_scale=TRUE;
  vdp2->sv->scaleValue=0;
  vdp2->sv->showLabels=FALSE;
  vdp2->sv->old_primID=-1;


  vdp2->curr_slen=data->slen;
  vdp2->curr_qlen=data->qlen;
  vdp2->data=data;
 
  /* hits info */

  vdp2->Fh=vdp->Fh;
  vdp2->charw=vdp->charw;
}


/*________________________________________(Init_bufs)_____________

  Purpose : Initialize sequence buffers to NULL.

____________________________________________________________________*/

static void Init_bufs(CharPtr qBuf, CharPtr sBuf, Int4 size)
{
  Int4   i;

  for (i = 0; i<size; i++)
    { 
      qBuf[i]= '\0'; 
      sBuf[i]= '\0'; 
    }
}


/*________________________________________(DOT_SVDisplaySequence)_____________

  Purpose : Draw function for viewer2 of second window.

____________________________________________________________________*/

static void  DOT_SVDisplaySequence(DOTVibDataPtr vdp2, DOTAlnPtr salp)
{
  RecT       rc;
  DOTVibDataPtr vdp;
  DOTSelDataPtr  data;
  DOTMainDataPtr mip1, mip2;
  SegmenT    pict2;
  SegmenT    nmSeg;
  SegmenT    hlSeg;
  SegmenT    clSeg;
  SegmenT    sbSeg;
  Boolean    match=FALSE;
  Uint1Ptr   q;
  Uint1Ptr   s;
  Uint1Ptr   end;
  Uint1Ptr   seq_aln1=NULL, seq_aln2=NULL;
  Int4       a,b,c, pos;
  Boolean    ambig=FALSE, is_na;
  Boolean    get_barposition=TRUE;
  Int4       i, k, spaces;
  Int4       prot_threshold=0, bloc_cntr;
  Int4       res_cnt, bloc_size;
  Int4       x, y, x2, y2, q_ahead;
  Int4       s_ahead, qavail, savail;
  Int4       vis_2, buf_len;
  Int4       bufsize, sglen, diff1, diff2;
  Int4       xstart, xstop, ystart, ystop;
  Int4       hit_start, hit_stop, q_pos, s_pos;
  Int4       V2Height, Fh, margin;
  Int4       xdiff_left,xdiff_right;
  Int4       ydiff_left,ydiff_right;
  Int4       width,height;
  Int4       chw_2, chw_4;
  Int4       qbufstart, qbufstop;
  Int4       sbufstart, sbufstop;
  BaR        vsb;
  CharPtr    qBuf;
  CharPtr    sBuf;
  CharPtr PNTR residue_names;
  Int4Ptr PNTR matrix;
  Uint1      strand1, strand2;
  Int4       num_cls, num_segs, seg_len;
  Int4       q_start, q_stop, s_start, s_stop;      
  Boolean    query_on_minus = FALSE;
  Uint1      highlight;


  data=(DOTSelDataPtr)vdp2->data;
  if (data==NULL) return;
  vdp=data->vdp;
  if (vdp==NULL)  return;
  mip1= vdp->mip;
  mip2= vdp2->mip;
  margin=MAX((StringWidth(vdp2->xname)), (StringWidth(vdp2->yname)))+10;
  GetPosition(vdp2->sv->v2, &rc);
  Fh=vdp2->Fh;
  vis_2=VIS_LEN/2;

  highlight = vdp2->sv->highlight;

  if (vdp2->strand1 == Seq_strand_minus)
    query_on_minus = TRUE;

  x = rc.left;
  y = 5*Fh; /* on top -cartesian */
  y2 = y -Fh;
  x2 = x + margin;
  chw_2=vdp2->charw/2;
  chw_4=vdp2->charw/4;
  pict2=vdp2->sv->pict2;
  width=ABS(data->q_stop-data->q_start);
  height=ABS(data->s_stop-data->s_start);

  /* in selected region coordinates */
  if (salp->show == dot_plot){
    matrix=mip1->matrix;
    is_na=mip1->is_na;
    if (mip1->is_na)
      residue_names=na_names;
    else
      residue_names=aa_names;

    strand1=mip1->qstrand;
    strand2=mip1->sstrand;

    if (query_on_minus) {
      q_start = data->q_stop - salp->q_start;
      q_stop = data->q_stop - salp->q_stop;
    }
    else {
      q_start = salp->q_start - data->q_start;
      q_stop = salp->q_stop - data->q_start;
    }
    s_start = ABS(data->s_start - salp->s_start);
    s_stop = ABS(data->s_start - salp->s_stop);

    xdiff_left = MIN(q_start, q_stop);
    xdiff_right = width - MAX(q_stop, q_start);
    ydiff_left = s_start;
    ydiff_right = height-s_stop;
    if (xdiff_left<ydiff_left)
      {
        xstart=0;
        ystart=ydiff_left-xdiff_left;
      }
    else
      {
        ystart=0;
        xstart=xdiff_left-ydiff_left;
      }
    
    if (xdiff_right<ydiff_right)
      {
        xstop= MAX(q_stop, q_start)+xdiff_right;
        ystop=s_stop+xdiff_right;
      }
    else
      {
        xstop=MAX(q_stop, q_start)+ydiff_right;
        ystop=s_stop+ydiff_right;
      }
    
    if ((xstop-xstart)!=(ystop-ystart))/* these should be equal*/
      return;
    
    data->xstart=xstart;
    data->xstop=xstop;
    data->ystart=ystart;
    data->ystop=ystop;
    
    buf_len=xstop-xstart+1;
    hit_start=q_start-xstart;
    hit_start+=hit_start/10;
    hit_stop=q_stop-xstart;
    if (query_on_minus) 
      q_pos = data->q_stop - xstart; 
    else
      q_pos = data->q_start + xstart;
    s_pos = data->s_start + ystart;
    s=vdp2->mip->qseq + xstart; 
    q=vdp2->mip->sseq + ystart; 
  }
  else if (salp->show == align_plot){
    matrix=vdp2->alp->matrix;
    is_na=vdp2->alp->is_na; 
    if (vdp2->alp->is_na)
      residue_names=na_names; 
    else 
      residue_names=aa_names; 
    strand1=AlnMgr2GetNthStrand(salp->sap, 1);
    strand2=AlnMgr2GetNthStrand(salp->sap, 2);
    q_pos=salp->q_start + 1;
    s_pos=salp->s_start + 1;
    hit_start=0;
    hit_stop=ABS(salp->q_stop-salp->q_start);
    seq_aln1=DOT_GetNthSeqFromAlign(salp->sap, 1);
    seq_aln2=DOT_GetNthSeqFromAlign(salp->sap, 2);
    if (seq_aln1==NULL || seq_aln2==NULL)
      return;
    q=seq_aln1;
    s=seq_aln2;
    buf_len=ABS(salp->q_stop-salp->q_start)+1;
  }
  end=s+buf_len-1;
  hit_stop+=hit_stop/10;
  seg_len=(rc.right-rc.left)/vdp2->charw;
  num_segs=buf_len/seg_len;
  if (seg_len%buf_len)
    num_segs++;
  if (num_segs==0)
      num_segs=1;
  num_cls=num_segs/5;
  if (5%num_segs)
    num_cls++;
  if (num_cls==0)
      num_cls=1;
  bufsize=seg_len+(seg_len/10)+2;
  qBuf=(CharPtr)MemNew(sizeof(Char)*bufsize);
  sBuf=(CharPtr)MemNew(sizeof(Char)*bufsize);
  i=0;
  a=0;b=0;c=0;
  spaces=0;res_cnt=0;bloc_cntr=0;bloc_size=1;
  k=1;
  for (a=0; a<num_cls; a++)
    {
      clSeg=CreateSegment(pict2,a+1,0);
      for(b=0; b<num_segs; b++)
        {
          sglen=seg_len;
          sbSeg=CreateSegment(clSeg,b+1,0);
          Init_bufs(qBuf,sBuf,seg_len);   
          for(c=0; c<sglen;c++)
            {
              if (!(i<buf_len))
                {
                  a=num_cls;
                  b=num_segs;
                  sglen=c;
                  if (hit_start<=i && i<=(hit_stop+bloc_size))
                    {
                      DOT_HlSeq(sbSeg, rc, margin, i+1, bloc_size, hit_start, hit_stop, Fh,chw_2);
                    }
                  goto end;
                }

              if (is_na)
                {
                  if (*q > 3 || *s > 3)
                    ambig=TRUE;
                }
              else
                {
                  if (*q >24 || *s >24 || *q<1 || *s<1 )
                    ambig=TRUE;
                }

              if (ambig)
                {
                  if (s<end) /* not end */
                    {
                      qBuf[c]=*residue_names[(int)*q];
                      sBuf[c]=*residue_names[(int)*s];
                      ambig=FALSE;
                      goto skip; /* continue */
                    }
                  else
                    {
                      a=num_cls;
                      b=num_segs;
                      sglen=c;
                      if (hit_start<=i && i<=(hit_stop+bloc_size))
                        {
                          DOT_HlSeq(sbSeg, rc, margin, i+1, bloc_size, hit_start, hit_stop, Fh,chw_2);
                        }
                      goto end;
                    }
                }
                        
              if (*s == *q)
                match=TRUE;
            
            
              qBuf[c]=*residue_names[(int)*q];
              sBuf[c]=*residue_names[(int)*s];
              
              if (match == TRUE)
                {
                  if (highlight == SHOW_MATCHES)
                    DOT_Highlight(sbSeg, rc, margin, i, Fh, chw_2, match);
                  match=FALSE;
                }
              else if (highlight == SHOW_MISMATCHES)
                DOT_Highlight(sbSeg, rc, margin, i, Fh, chw_2, match);
              
            skip:

              if (!(k % BLOCK_SIZE))
                {
                  c++;
                  sglen++;
                  i++;
                  buf_len++;
                  bloc_size=0;

                  qBuf[c] = ' ';
                  sBuf[c] = ' ';
                  if(match)
                    spaces++;
                  
                  bloc_cntr++; 
                  res_cnt=i-bloc_cntr;
                  DOT_DrawScale(sbSeg, strand1, strand2, rc, margin, res_cnt, bloc_cntr, Fh, q_pos, s_pos, chw_2, chw_4);
                  if (hit_start<=i && i<(hit_stop+BLOCK_SIZE))
                    {
                      DOT_HlSeq(sbSeg, rc, margin, i, BLOCK_SIZE, hit_start, hit_stop, Fh, chw_2);
                      if (get_barposition)
                        {
                          pos=DOT_WorldtoScreen(i,chw_2);
                          vdp2->sv->barp=pos+rc.left+margin;
                          get_barposition=FALSE;
                        }
                    }
                }
              
              i++;
              k++;
              q++;
              s++;
              bloc_size++;
              
            }
        end:
          
          /* attach buffer */

          pos=DOT_WorldtoScreen(i-sglen, chw_2);
          AddAttribute(sbSeg, COLOR_ATT, RED_COLOR, 0,0,0,0);
          AddLabel(sbSeg, x2+pos, y, qBuf , SMALL_TEXT, 0, UPPER_RIGHT, 0);
          AddAttribute(sbSeg, COLOR_ATT, BLACK_COLOR, 0,0,0,0);
          AddLabel(sbSeg, x2+pos, y2, sBuf , SMALL_TEXT, 0, UPPER_RIGHT, 0);
        }
    }

/*write seq names */

  nmSeg=CreateSegment(pict2, num_cls+1, 0);
  AddAttribute(nmSeg, COLOR_ATT, MAGENTA_COLOR, 0,0,0,0);
  AddLabel(nmSeg, x, y, vdp2->xname, SMALL_TEXT, 0, UPPER_RIGHT, 0);
  AddLabel(nmSeg, x, y2, vdp2->yname, SMALL_TEXT, 0, UPPER_RIGHT, 0);
    

  MemFree(qBuf);
  MemFree(sBuf);
  if (seq_aln1)
    MemFree(seq_aln1);
  if (seq_aln2)
    MemFree(seq_aln2);
  return;
    
}

/*________________________________________(DOT_SVPopulateSequenceViewer)______

  Purpose : Calls draw function for viewer2 of second window.

____________________________________________________________________*/
static Boolean DOT_SVPopulateSequenceViewer(DOTVibDataPtr vdp2)
{
  RecT    rc;
  Int4    x;
  BaR     hsb;
  Char    infoBuf[255];
  DOTSelDataPtr data;
  DOTAlnPtr salp;

  data=(DOTSelDataPtr)vdp2->data;
  if (data==NULL) return FALSE;

  ResetViewer(vdp2->sv->v2);
  vdp2->sv->pict2=DeletePicture(vdp2->sv->pict2);
  Update();

  vdp2->sv->pict2=CreatePicture();
  GetPosition(vdp2->sv->v2, &rc);

  salp=vdp2->sv->salp;
  if (salp==NULL)
    {
      AddAttribute(vdp2->sv->pict2, COLOR_ATT, RED_COLOR, 0,0,0,0);
      AddLabel(vdp2->sv->pict2, rc.left, rc.bottom-rc.top, "- CLICK on a diagonal to view the aligned sequence -", SMALL_TEXT, 0, UPPER_RIGHT, 0);
       
      /*reset infopanel2 title*/
      SetTitle(vdp2->Infopanel, vdp2->iInfo);
      Disable(vdp2->Ggoto);
    }
  else
    {
      DOT_SVDisplaySequence(vdp2, salp);

      /*reset infopanel2 title*/
      sprintf(infoBuf, "Selected.. %s(x-axis) [%d..%d] vs. %s(y-axis) [%d..%d]", vdp2->xname, salp->q_start + 1, salp->q_stop + 1, vdp2->yname, salp->s_start + 1, salp->s_stop + 1);

      SetTitle(vdp2->Infopanel,infoBuf);
      Enable(vdp2->Ggoto);
    }
  
  
  AttachPicture(vdp2->sv->v2, vdp2->sv->pict2, vdp2->sv->barp, INT4_MIN, UPPER_CENTER, 1, 1, NULL);
  ArrowCursor();
  return TRUE;
 
}


/*________________________________________(DOT_SVFindHit)_____________

  Purpose : gets sequence coordinates of clicked diag (viewer2, window2)..

____________________________________________________________________*/
DOTAlnPtr DOT_SVFindHit(DOTVibDataPtr vdp2, Int4 primID)
{
  DOTAlnPtr  salp;
  Int4     index, s_start, q_start, q_stop, len;
  Boolean  query_on_minus = FALSE;


  if (primID<1 || primID>vdp2->mip->index)
    return NULL;

  salp=(DOTAlnPtr)MemNew(sizeof(DOTAln));
  salp->show=dot_plot;
  index=primID-1;
  q_start=vdp2->mip->hitlist[index]->q_start;
  s_start=vdp2->mip->hitlist[index]->s_start;
  len=vdp2->mip->hitlist[index]->length-1;
  if (vdp2->strand1 == Seq_strand_minus)
    query_on_minus = TRUE;

  if (query_on_minus) { 
    salp->q_start = vdp2->xstart + ABS(vdp2->xstop - q_start);
    salp->q_stop = salp->q_start - len;
  }
  else {
    salp->q_start = q_start;
    salp->q_stop = q_start + len;
  }

  salp->s_start = s_start;
  salp->s_stop = s_start + len;
  salp->primID = (Uint2)primID;
  
  return salp;
}



/*________________________________________(DOT_SVGetDiag)_____________

  Purpose : Calls func to get sequence coordinates of clicked diag (viewer2, window2).

____________________________________________________________________*/

static Boolean DOT_SVGetDiag (Uint2 segID, Uint2 primID, VoidPtr userdata)

{
  DOTVibDataPtr  vdp2;
  DOTAlnPtr     salp;

  vdp2 = (DOTVibDataPtr) userdata;
  if (vdp2 == NULL || segID !=1) 
    return FALSE;

  salp=DOT_SVFindHit(vdp2, primID);
  userdata=(Pointer)salp;
  return TRUE;
}
/*________________________________________(DOT_DeSelectAll)_____________

   Purpose : Deselect all primitives.

____________________________________________________________________*/
static Boolean DOT_DeSelectAll(SegmenT seg, PrimitivE prim, Uint2 segID,  Uint2 primID, Uint2 primCt, VoidPtr userdata)
{
  Int1  highlight;
  VieweR  v;

  if (primID) {
    v = (VieweR) userdata;
    GetPrimDrawAttribute (prim, NULL, NULL, NULL, NULL, NULL, &highlight);
    if (highlight != PLAIN_PRIMITIVE) 
      HighlightPrimitive (v, seg, prim, PLAIN_PRIMITIVE);
  }

  return TRUE;
}


/*________________________________________(DOT_SVClickProc)_____________

   Purpose : Click proc for viewer1 of second window.

____________________________________________________________________*/
static void DOT_SVClickProc(VieweR v, SegmenT seg, PoinT pt)
{
  DOTVibDataPtr  vdp2;
  PrimitivE     prim=NULL;
  Uint2         primID = 0, entityID=0, itemID=0, itemtype=0;
  Int1          highlight=0;
  Int2          handled=0;
  DOTAlnPtr     salp=NULL;
  SegmenT       seg1 = NULL;

  vdp2=(DOTVibDataPtr)GetObjectExtra((WindoW)ParentWindow(v));
  if(!vdp2) return;
  seg1 = FindSegPrim(v, pt, NULL, NULL, &prim);
  if (seg1 != NULL)
    {
      GetPrimitiveIDs(prim, &entityID, &itemID, &itemtype, &primID);
    if (primID != 0)
      {
        GetPrimDrawAttribute (prim, NULL, NULL, NULL, NULL, NULL, &highlight);
        if (highlight == PLAIN_PRIMITIVE) {
          ExploreSegment (seg1, (Pointer)v, DOT_DeSelectAll);
          HighlightPrimitive (v, seg, prim, FRAME_PRIMITIVE);

          if (entityID && itemID && itemtype) { /* alignment */
            salp=DOT_FindAlignment(vdp2, primID);
          }
          else { /* dot matrix hit */
            salp = DOT_SVFindHit(vdp2, primID);
          }
          prev_primID = primID;
        }
        else {
          ExploreSegment (seg1, (Pointer)v, DOT_DeSelectAll);
          salp = NULL;
          prev_primID = 0;
        }
      }
  }
  else {
    ExploreSegment (seg, (Pointer)v, DOT_DeSelectAll);
    salp = NULL;
    prev_primID = 0;
  }
  if (vdp2->sv->salp) 
    MemFree(vdp2->sv->salp);
  vdp2->sv->salp=salp;
  DOT_SVPopulateSequenceViewer (vdp2);
}

  

/*________________________________________(DOT_SVPopulateDiagViewer)_____________

  Purpose : Calls draw function for diags.

____________________________________________________________________*/
static Boolean DOT_SVPopulateDiagViewer(DOTVibDataPtr vdp2)
{
  Int4       scale, index;
  Char       str[16], infoBuf[255];
  DOTSelDataPtr  data;
  BaR        vsb;
  
  
  data=(DOTSelDataPtr)vdp2->data;
  
  ResetViewer(vdp2->sv->v1);
  vdp2->sv->pict1=DeletePicture(vdp2->sv->pict1);
  Update();

  vdp2->sv->pict1=CreatePicture();
   
  if (DOT_SVDisplayDiags(vdp2, data)==FALSE)
    return FALSE;
  
  if (vdp2->sv->do_scale) 
    {
      for (index=1; index<MAXZOOMSCALEVAL; index++) 
        {
          sprintf (str, "%d", (long) (zoomScaleVal [index]));
          PopupItem (vdp2->sv->scale, str);
        }
      SetValue (vdp2->sv->scale, vdp2->sv->scaleIndex);
      vdp2->sv->do_scale = FALSE;

    }

  SafeShow(vdp2->sv->scale);

 
  AttachPicture(vdp2->sv->v1, vdp2->sv->pict1, INT4_MIN, INT4_MAX, LOWER_RIGHT,  vdp2->sv->scaleValue , vdp2->sv->scaleValue , NULL);
  if (vdp2->showDotPlot && vdp2->showALIGN){/* not clickable*/
    ArrowCursor();
    return TRUE;
  }
  SetViewerProcs (vdp2->sv->v1, DOT_SVClickProc, NULL, NULL, NULL);
  ArrowCursor();
  return TRUE;
  
}


/*____________________________________________(DOT_ChangeSequenceWindowCutoff)______

  Purpose : Change threshold for diag using threshold ramp.

____________________________________________________________________*/
static void DOT_ChangeSequenceViewerCutoff (BaR b, GraphiC g, Int2 new, Int2 old) 
{
  DOTVibDataPtr vdp2;
  WindoW     w;
  RecT       rcP;

  w=(WindoW)ParentWindow(b);
  vdp2 = (DOTVibDataPtr) GetObjectExtra (w);

  vdp2->sdp.TrampPos = new+20;
  DOT_SVPopulateDiagViewer(vdp2);

}

/*________________________________________(DOT_SVChangeScale)_____________

  Purpose : Change scale.

____________________________________________________________________*/
static void DOT_SVChangeScale (PopuP p)

{
  DOTVibDataPtr vdp2;
  Int4       index;

  vdp2 = (DOTVibDataPtr) GetObjectExtra (p);
  if (vdp2 != NULL) 
    {
      index = GetValue (vdp2->sv->scale);
      if (index <= MAXZOOMSCALEVAL && index > 0) 
        {
          vdp2->sv->scaleValue = zoomScaleVal [index];
        } 
      else 
        {
          vdp2->sv->scaleValue = 1;
        }

/*       AttachPicture(vdp2->sv->v1, vdp2->sv->pict1, INT4_MIN, INT4_MAX, LOWER_RIGHT,  vdp2->sv->scaleValue , vdp2->sv->scaleValue , NULL); */
/*       SetViewerProcs (vdp2->sv->v1, DOT_SVClickProc, NULL, NULL, NULL); */
      DOT_SVPopulateDiagViewer (vdp2);
    }
}
/*________________________________________(DOT_SVChangeLabels)_____________

  Purpose : Show or Hide coordinate labels for diags.

____________________________________________________________________*/
static void DOT_SVChangeLabels (GrouP g)

{
  DOTVibDataPtr  vdp2;

  vdp2 = (DOTVibDataPtr) GetObjectExtra (g);
  if (vdp2 != NULL) 
  {
      vdp2->sv->showLabels=(Boolean)(GetValue(vdp2->sv->Labels)==1);
      DOT_SVPopulateDiagViewer (vdp2);
  }
}

/*________________________________________(DOT_SVCalculateScaling)_____________

  Purpose : Estimates size of picture.

____________________________________________________________________*/
static void DOT_SVCalculateScaling (DOTVibDataPtr vdp2)

{
  RecT   r, world;
  Int4   index, r_hgt, r_wdt, w_hgt, w_wdt, scale;
  double f1, f2;

  w_hgt=vdp2->ylen+(vdp2->ylen*0.15);
  w_wdt=vdp2->xlen+(vdp2->xlen*0.15);

  GetPosition(vdp2->sv->v1, &r);
  r_hgt=r.bottom-r.top;
  r_wdt=r.right-r.left;

  f1=(float)w_hgt/r_hgt;
  f2=(float)w_wdt/r_wdt;

  scale=MAX(ceil(f1), ceil(f2));

  for (index=1; index<MAXZOOMSCALEVAL; index++) 
    {
      if (zoomScaleVal [index]>= scale)
        {
          vdp2->sv->scaleValue=zoomScaleVal[index];
          vdp2->sv->scaleIndex=index;
          return;
        }
    }

  vdp2->sv->scaleValue=zoomScaleVal[MAXZOOMSCALEVAL-1];
  vdp2->sv->scaleIndex=MAXZOOMSCALEVAL-1;

}

/*________________________________________(DOT_ResizeFeatWindow)_____________

  Purpose : Resize function for window2.

____________________________________________________________________*/

static void DOT_ResizeFeatWindow(WindoW w)
{
  RecT     rcDlg,rcQry,rcSub, rcVsb,rcHsb, rcQi, rcSi;
  Int2     height,width,vsbWidth,in,gap,hsbHeight,QueryHeight,SubjectHeight, halfw;
  BaR      vsb,hsb;
  DOTFeatListPtr flp;
  WindoW   temport;


  flp=(DOTFeatListPtr)GetObjectExtra(w);
  temport=SavePort(w);

  ObjectRect(w,&rcDlg);
  width= rcDlg.right-rcDlg.left;
  halfw=width/2;
  height= rcDlg.bottom-rcDlg.top;
  
  SafeHide(flp->Query);
  SafeHide(flp->Subject);
  SafeHide(flp->QInfo);
  SafeHide(flp->SInfo);
  Update();
  
  vsb = GetSlateVScrollBar ((SlatE) flp->Query);
  hsb = GetSlateHScrollBar ((SlatE) flp->Subject);
  
  GetPosition(flp->Query,&rcQry);
  GetPosition(flp->Subject,&rcSub);
  GetPosition(flp->QInfo, &rcQi);
  GetPosition(flp->SInfo, &rcSi);
  GetPosition(vsb,&rcVsb);
  GetPosition(hsb,&rcHsb);

  in=2;
  gap=10;
  vsbWidth=rcVsb.right-rcVsb.left;
  hsbHeight=rcHsb.bottom-rcHsb.top;
  QueryHeight=rcQry.bottom-rcQry.top;
  SubjectHeight=rcSub.bottom-rcSub.top;
  
  /*new sizes for the viewers*/	
  rcSub.right=width-in-vsbWidth;
  rcSub.bottom=height-in-hsbHeight;
  rcSub.left=halfw+in;
  rcSi.left=rcSub.left;
  rcQry.right=halfw-in-vsbWidth;
  rcQry.bottom=height-in-hsbHeight;

 
  
  /*set the new sizes*/
  SetPosition(flp->Query,&rcQry);
  AdjustPrnt (flp->Query, &rcQry, FALSE);
  SetPosition(flp->Subject,&rcSub);
  AdjustPrnt (flp->Subject, &rcSub, FALSE);
  SetPosition(flp->SInfo,&rcSi);
  AdjustPrnt (flp->SInfo, &rcSi, FALSE);

  AttachPicture (flp->Query,flp->segQuery, INT4_MIN, flp->vert_Qpos, UPPER_LEFT,1 , 1, NULL); 
  SetViewerProcs (flp->Query, DOT_QViewerClickProc, NULL, NULL, NULL);

  AttachPicture (flp->Subject,flp->segSubject, INT4_MIN, flp->vert_Spos, UPPER_LEFT,1 , 1, NULL);  
  SetViewerProcs (flp->Subject, DOT_SViewerClickProc, NULL, NULL, NULL);

  SafeShow(flp->QInfo);
  SafeShow(flp->SInfo);
  SafeShow(flp->Query);
  SafeShow(flp->Subject);
  RestorePort(temport);
  Update();
}

/*________________________________________(DOT_ResizeSequenceWindow)_____________

  Purpose : Resize function for window2.

____________________________________________________________________*/

static void DOT_ResizeSequenceWindow(WindoW w)
{
  Int4     lmargin;
  RecT     rcDlg,rcV1,rcV2, rcVsb,rcHsb, rcGoto;
  Int2     height,width,gap,vsbWidth,in,hsbHeight,V1Height,V2Height, goHeight;
  BaR      vsb,hsb;
  DOTVibDataPtr vdp2;
  Boolean  is_visible1, is_visible2;
  WindoW   temport;


  vdp2=(DOTVibDataPtr)GetObjectExtra(w);
  temport=SavePort(w);
  ObjectRect(w,&rcDlg);
  width= rcDlg.right-rcDlg.left;
  height= rcDlg.bottom-rcDlg.top;
  
  SafeHide(vdp2->sv->v1);
  SafeHide(vdp2->sv->v2);
  SafeHide(vdp2->Infopanel);
  SafeHide(vdp2->Ggoto);
  Update();
  
  vsb = GetSlateVScrollBar ((SlatE) vdp2->sv->v1);
  hsb = GetSlateHScrollBar ((SlatE) vdp2->sv->v1);
  
  GetPosition(vdp2->sv->v1,&rcV1);
  GetPosition(vdp2->sv->v2,&rcV2);
  GetPosition(vsb,&rcVsb);
  GetPosition(hsb,&rcHsb);
  GetPosition(vdp2->Ggoto, &rcGoto);


  gap=2;
  in=vdp2->Fh;
  lmargin=10;
  vsbWidth=rcVsb.right-rcVsb.left;
  hsbHeight=rcHsb.bottom-rcHsb.top;
  V1Height=rcV1.bottom-rcV1.top;
  V2Height=9*vdp2->Fh;
  goHeight= rcGoto.bottom-rcGoto.top;

  /*new sizes for the viewers*/	
  rcV1.left=lmargin;
  rcV1.right=width-gap-vsbWidth-rcV1.left;
  rcV2.bottom=height-gap-in-hsbHeight;
  rcV2.top=rcV2.bottom-V2Height;
  rcGoto.bottom=rcV2.top-gap;
  rcGoto.top=rcGoto.bottom-goHeight;
  rcV1.bottom=rcGoto.top-gap-hsbHeight;
  rcV2.left=lmargin;
  rcV2.right=width-gap-vsbWidth-rcV2.left;
  
  /*set the new sizes*/
  SetPosition(vdp2->sv->v1,&rcV1);
  AdjustPrnt (vdp2->sv->v1, &rcV1, FALSE);
  
  SetPosition(vdp2->sv->v2,&rcV2);
  AdjustPrnt (vdp2->sv->v2, &rcV2, FALSE);

  SetPosition(vdp2->Ggoto, &rcGoto);
  AdjustPrnt(vdp2->Ggoto, &rcGoto, FALSE);
  
  if ((rcV1.left<rcV1.right)&&(rcV1.top<rcV1.bottom))
    is_visible1=TRUE;
  if ((rcV2.left<rcV2.right)&&(rcV2.top<rcV2.bottom))
    is_visible2=TRUE;

  /*update viewers*/
  if (Visible (vdp2->sv->v1) && AllParentsVisible (vdp2->sv->v1))
    ViewerWasResized(vdp2->sv->v1);
  if (Visible (vdp2->sv->v2) && AllParentsVisible (vdp2->sv->v2))
    ViewerWasResized(vdp2->sv->v2);
  
  if (vdp2->sv->do_scale!=TRUE) /* window resized */
    {
      DOT_SVPopulateDiagViewer(vdp2);
      DOT_SVPopulateSequenceViewer(vdp2);
    }
                       
  SafeShow(vdp2->sv->v1);
  SafeShow(vdp2->sv->v2);
  SafeShow(vdp2->Infopanel);
  SafeShow(vdp2->Ggoto);
  ArrowCursor();
  RestorePort(temport);
  Update();
}

 static Boolean DOT_DeleteCursorPrim (SegmenT seg, PrimitivE prim, Uint2 segID,
                           Uint2 primID, Uint2 primCt, VoidPtr userdata)
{
  DeletePrim(seg,prim);
  return TRUE;
}


static Int4 DOT_PointCursorOnFeature(SegmenT seg, DOTSelFeatPtr feat_list, Int2 fontHt, Boolean found)
{
  Int4  yBase, cursorHt;
  Boolean is_first =TRUE;
  Int4  vertbar_pos;

  cursorHt=fontHt/4;

  if (!found)
    {
      AddAttribute(seg, COLOR_ATT, BLUE_COLOR, 0, 0, 0, 0);
      yBase = (-1)*(feat_list->feat_num*(fontHt)-cursorHt);
      vertbar_pos=yBase+3*fontHt;
      AddRectangle(seg, 0,yBase, 2,yBase-2*cursorHt,RIGHT_ARROW,TRUE,0);
    }
  else
    {
      AddAttribute(seg, COLOR_ATT, RED_COLOR, 0, 0, 0, 0);      
      while (feat_list)
        {
          yBase = (-1)*((feat_list->feat_num-1)*fontHt+cursorHt);
          if (is_first)
            {
              vertbar_pos=yBase+3*fontHt;
              is_first=FALSE;
            }
          AddRectangle(seg, 0,yBase, 2,yBase-2*cursorHt,RIGHT_ARROW,TRUE,0);
          feat_list=feat_list->next;
        }
    }
  return (vertbar_pos);
}

static DOTFeatPtr   DOT_FindNextDfp(DOTFeatIndexPtr fdindex, DOTFeatPtr dfp, Boolean use_first)
{
  if (!use_first)
    dfp=dfp->next;
  while (dfp != NULL)
    {
      if (fdindex[dfp->type].show)
        return dfp;
      dfp=dfp->next;
    }
  return NULL;
}


static DOTSelFeatPtr DOT_FindFeatureInViewer(DOTFeatIndexPtr fdindex, DOTRowPtr drp, Int4 cursor_pos)
{
  DOTFeatPtr dfp_head;
  DOTSelFeatPtr feat_list=NULL;
  Int4       i;

  /* fix this so that it also works for minus strand */
  i=1;
  dfp_head = DOT_FindNextDfp(fdindex, drp->dfp, TRUE);
  while (dfp_head != NULL)
    {
      if (dfp_head->left<= cursor_pos && dfp_head->right >= cursor_pos)
        {
          if (!feat_list)
            {
              feat_list=(DOTSelFeatPtr)MemNew(sizeof(DOTSelFeat));
            }
          else
            {
              feat_list->next=(DOTSelFeatPtr)MemNew(sizeof(DOTSelFeat));
              feat_list=feat_list->next;
            }
          feat_list->feat_num = i;
        }
      if (dfp_head->right > cursor_pos)
        {
          if (feat_list) 
            return feat_list;
          else 
            return NULL;
        }
      i++;
      dfp_head=DOT_FindNextDfp(fdindex, dfp_head, FALSE);
    }
  return NULL;
}



static DOTSelFeatPtr DOT_FindBetweenFeats(DOTFeatIndexPtr fdindex, DOTRowPtr drp, Int4 cursor_pos)
{
  DOTFeatPtr dfp1, dfp2;
  DOTSelFeatPtr feat_list=NULL;
  Int4       i;

  /* fix this so that it also works for minus strand */
  i=1;
  dfp1 = DOT_FindNextDfp(fdindex, drp->dfp, TRUE);
  if (!dfp1) goto end;
/*   dfp2 = DOT_FindNextDfp(fdindex, dfp1, FALSE); */
/*   if (!dfp2) goto end; */

  while (dfp1 != NULL /* && dfp2 != NULL */)
    {
      if (dfp1->left > cursor_pos)
        {
          if (!feat_list)
            {
              feat_list=(DOTSelFeatPtr)MemNew(sizeof(DOTSelFeat));
              feat_list->feat_num = i-1;
              return (feat_list);
            }
        }
    
      i++;
      dfp1=DOT_FindNextDfp(fdindex, dfp1, FALSE);
/*       dfp2=DOT_FindNextDfp(fdindex, dfp1, FALSE); */
    }

 end:
  feat_list=(DOTSelFeatPtr)MemNew(sizeof(DOTSelFeat));
  feat_list->next=NULL;
  feat_list->feat_num = MAX(i-1, 1);
  return (feat_list);

}
/*******************************************************************************

  Function : DOT_AddFeatureToSegment()
  
  Purpose : analyse one feature and add it to the Feature Viewer

*******************************************************************************/
static void DOT_AddFeatureToSegment(SeqMgrFeatContextPtr context,
			DOTPopFeatPtr pfp) 
{
PrimitivE  prim;
Int4	   yBase, xMargin;
Char     str[50]; 


 sprintf(str, "%s (%d - %d)", pfp->dfp_cur->label, pfp->dfp_cur->left, pfp->dfp_cur->right); 
 AddAttribute(pfp->TopParentSeg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
 yBase = (-1)*pfp->nfeats*pfp->fontHeight;
 xMargin=5;

 if (context->numivals==1){

 prim = AddLabel(pfp->TopParentSeg, xMargin, yBase, str, SMALL_TEXT, 0, UPPER_RIGHT, 0);
 SetPrimitiveIDs(prim,context->entityID,context->itemID,OBJ_SEQFEAT, 0);
	}
 /*
	else{
		DOT_AddSegmentedFeature(pfp->TopParentSeg,context->numivals,context->ivals, context->strand,context->entityID,context->itemID,yBase);
	}
 */
}

/*******************************************************************************

  Function : DOT_AddFeaturesToViewer()
  
  Purpose : callback called by SeqMgr (see also DOT_PopFeatureViewers(), below)

*******************************************************************************/
static Boolean LIBCALLBACK DOT_AddFeaturesToViewer(SeqFeatPtr sfp, 
			SeqMgrFeatContextPtr context)
{
BioseqPtr            parent;
SeqMgrSegmentContext contextPart;
SeqMgrFeatContextPtr context2;
Int4                 segID;
DOTFeatPtr           dfp_new=NULL;
DOTRowPtr            drp=NULL;
DOTFeatIndexPtr      fdindex=NULL; 
DOTPopFeatPtr         pfp=NULL;

 
	pfp = (DOTPopFeatPtr) context->userdata;

	if (pfp==NULL){
		return(FALSE);
	}


   drp=pfp->drp;
   
   if (pfp->dfp_cur == NULL) 
     {
       drp->dfp=(DOTFeatPtr)MemNew(sizeof(DOTFeat));
       pfp->dfp_cur=drp->dfp;
       dfp_new = drp->dfp;
     }
   else
     {
       if(!(pfp->dfp_cur->next=(DOTFeatPtr)MemNew(sizeof(DOTFeat)))) return(FALSE);
       pfp->dfp_cur=pfp->dfp_cur->next;
       dfp_new = pfp->dfp_cur;
     }

   if (!dfp_new) return(FALSE);

       
   if (context->strand>Seq_strand_minus ||
       context->strand==Seq_strand_unknown) 
     {
       pfp->dfp_cur->strand=Seq_strand_plus;
     }

       dfp_new->label=(CharPtr)FeatDefTypeLabel(sfp);
       dfp_new->left=context->left;
       dfp_new->right=context->right;
       dfp_new->type=context->featdeftype;
  
       /*add a feature to the viewer*/
/*      DOT_AddFeatureToSegment(context,pfp);  */
       

       pfp->nfeats++;
       fdindex = pfp->featindex;
       
       if (!fdindex[context->featdeftype].present)
         {
           fdindex[context->featdeftype].present=TRUE;
           fdindex[context->featdeftype].show=TRUE;
           fdindex[context->featdeftype].label=dfp_new->label;
  
         }
       
	return(TRUE);
}


static Boolean DOT_DeletePrims(SegmenT seg, PrimitivE prim, Uint2 segID,
                           Uint2 primID, Uint2 primCt, VoidPtr userdata)
{
  
  DeletePrim(seg, prim);
  return TRUE;
   
}

static void DOT_PlaceCursors(VieweR viewer, SegmenT seg, SegmenT segCursor, DOTFeatIndexPtr fdindex, DOTRowPtr drp, Int4 seqpos, Int2 fontHt, Int4Ptr ypos, Boolean from_click, Int4 fpos)
{
  Boolean found;
  DOTSelFeatPtr foundfeats;
  Int4    Y_pos;
  BaR     bar;
  WindoW  temport;
  RecT    rcP;

  if (!from_click)
    {
      foundfeats=DOT_FindFeatureInViewer(fdindex, drp, seqpos); 
      if (!foundfeats)
        {
          foundfeats=DOT_FindBetweenFeats(fdindex, drp, seqpos);
          found=FALSE;
        }
      else
        found=TRUE;
    }
  else
    {
      foundfeats=(DOTSelFeatPtr)MemNew(sizeof(DOTSelFeat));
      foundfeats->feat_num=fpos;
      foundfeats->next=NULL;
      found=TRUE;
    }
  ExploreSegment (segCursor, NULL, DOT_DeleteCursorPrim);
  Y_pos=DOT_PointCursorOnFeature(segCursor, foundfeats, fontHt, found);
  *ypos=Y_pos;

/*   if (from_main_viewer)  */
/*     { */
/*       bar=GetSlateHScrollBar((SlatE) viewer); */
/*       Y_pos=GetBarValue(bar); */
/*     } */
/*   else */
/*     SetBarValue(GetSlateHScrollBar((SlatE) viewer), Y_pos); */

  MemFree(foundfeats); /* fix this !!*/
  if (from_click)
    {
      temport = SavePort(ParentWindow(viewer));
      Select(viewer);
      GetPosition(viewer, &rcP);
      InsetRect(&rcP, -1, -1);
      InvalRect(&rcP);
      RestorePort(temport);
      Update();
    }
  else
    {
      AttachPicture (viewer, seg, INT4_MIN, Y_pos, UPPER_LEFT,1 , 1, NULL); 
    }
    
  
}


static void DOT_QViewerClickProc(VieweR v, SegmenT seg, PoinT pt)
{
  DOTFeatListPtr flp;
  DOTFeatPtr     dfp;
  DOTSelDataPtr  data;
  Int4           i, xmidPt, comp, j;
  Uint2          segID, primID, primCT;
  PrimitivE      prim;
  RecT           rc;
  Char           infoBuf[255];

  flp=(DOTFeatListPtr)GetObjectExtra((WindoW)ParentWindow(v));
  if (!flp) return;

  if (FindSegPrim(v, pt, NULL, NULL, &prim))
    {
      FindSegment(v, pt, &segID, &primID, &primCT);
      if (segID==1)
        dfp=flp->query_drp->dfp;
      else
        return;
      i=1;
      dfp=DOT_FindNextDfp(flp->featindex, dfp, TRUE);
      if (primID!=1)
        {
          for (; i<primID; i++)
            {
              dfp=DOT_FindNextDfp(flp->featindex, dfp, FALSE);
              if (dfp==NULL) return;
            }
        }


      if (dfp==NULL) return;

      data=(DOTSelDataPtr)flp->data;
      xmidPt=ABS(dfp->right+dfp->left)/2;
      data->q_start=xmidPt;
      GetPosition(data->vdp->panel, &rc);
      DOT_AddRectMargins(&rc, data->vdp);
      comp=data->vdp->comp;
      xmidPt=ABS(xmidPt-flp->mip->q_start);
      curpnt.x=MIN(rc.left+(xmidPt/comp), rc.right);
      
      DOT_PlaceCursors(v, seg, flp->segQCursor, flp->featindex, flp->query_drp, dfp->left, flp->fontHt, &(flp->vert_Qpos), TRUE, i);
      SetViewerProcs (v, DOT_QViewerClickProc, NULL, NULL, NULL);
      sprintf(infoBuf, "Hairs .. %s (x-axis)[%d]  vs.  %s (y-axis)[%d]", flp->mip->qname, data->q_start, flp->mip->sname, data->s_start);
      SetTitle(data->vdp->Infopanel,infoBuf);
      DOT_UpdateMainPanel(data->vdp, TRUE);
    }
}

static void DOT_SViewerClickProc(VieweR v, SegmenT seg, PoinT pt)
{
  DOTFeatListPtr flp;
  DOTFeatPtr     dfp;
  DOTSelDataPtr  data;
  Char           infoBuf[255];
  RecT           rc;
  Int4           i, ymidPt, comp;
  Uint2          segID, primID, primCT;
  PrimitivE      prim;


  flp=(DOTFeatListPtr)GetObjectExtra((WindoW)ParentWindow(v));
  if (!flp) return;

  if (FindSegPrim(v, pt, NULL, NULL, &prim))
    {
      FindSegment(v, pt, &segID, &primID, &primCT);
      if (segID==1)
        dfp=flp->subject_drp->dfp;
      else
        return;
      i=1;
      dfp=DOT_FindNextDfp(flp->featindex, dfp, TRUE);
      if (primID!=1)
        {
          for (; i<primID; i++)
            {
              dfp=DOT_FindNextDfp(flp->featindex, dfp, FALSE);
              if (dfp==NULL) return;
            }
        }


      if (dfp==NULL) return;

      data=(DOTSelDataPtr)flp->data;
      ymidPt = ABS(dfp->right+dfp->left)/2;
      data->s_start=ymidPt;
      GetPosition(data->vdp->panel, &rc);
      DOT_AddRectMargins(&rc, data->vdp);
      ymidPt=ABS(ymidPt -flp->mip->s_start);
      comp=data->vdp->comp;
      curpnt.y=MIN(rc.top+(ymidPt/comp), rc.bottom);

      DOT_PlaceCursors(v, seg, flp->segSCursor, flp->featindex, flp->subject_drp, dfp->left, flp->fontHt, &(flp->vert_Spos), TRUE, i);
      SetViewerProcs (v, DOT_SViewerClickProc, NULL, NULL, NULL);
      sprintf(infoBuf, "Hairs .. %s (x-axis)[%d]  vs.  %s (y-axis)[%d]", flp->mip->qname, data->q_start, flp->mip->sname, data->s_start);
      SetTitle(data->vdp->Infopanel,infoBuf);
      DOT_UpdateMainPanel(data->vdp, TRUE);
    }
}




static void DOT_PlaceFeat(DOTFeatListPtr flp, SegmenT seg, Int4 yBase, DOTFeatPtr dfp, Int4 primID)
{
  Char     str[50]; 
  Int4     xMargin;
  
  sprintf(str, "%s (%d,%d,%d)", dfp->label, dfp->left, dfp->right, ABS(dfp->right-dfp->left)); 
  AddAttribute(seg, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
  xMargin=5;
    
  AddLabel(seg, xMargin, yBase, str, SMALL_TEXT, 0, UPPER_RIGHT, primID);
 
}



static void DOT_UpdateFeatViewer (DOTFeatListPtr flp, VieweR viewer, SegmenT seg, SegmenT segName, SegmenT segCursor, VwrClckProc click, DOTRowPtr drp, Int4 cur_pos, Int4Ptr vert_pos)
{
  Int4 i;
  DOTFeatPtr dfeatp;
  DOTFeatIndexPtr fdindex;

  ExploreSegment(seg, NULL, DOT_DeletePrims);
  dfeatp=drp->dfp;
  fdindex=flp->featindex;

  i = 1;
  
  while (dfeatp != NULL)
    {
      if (fdindex[dfeatp->type].show)
        {
          DOT_PlaceFeat(flp, segName, (-1)*i*flp->fontHt, dfeatp, i);
          i++;
        }
      dfeatp=dfeatp->next;
    }
  if (i>1)
    {
      DOT_PlaceCursors(viewer, seg, segCursor, flp->featindex, drp, cur_pos, flp->fontHt, vert_pos, FALSE, 0);
      SetViewerProcs (viewer, click, NULL, NULL, NULL);

    }
  else
    {
      AddAttribute(seg, COLOR_ATT, RED_COLOR, 0,0,0,0);
      AddLabel(seg, 0, 0, "- no features -", SMALL_TEXT, 0, UPPER_RIGHT, 0);
      AttachPicture (viewer, seg, INT4_MIN, INT4_MAX, UPPER_LEFT,1 , 1, NULL); 
    }
}
/*_________________________________________________________________

  Function : DOT_PopFeatureViewers()
  
  Purpose : populate the Feature Viewer with the features.

___________________________________________________________________*/
static void DOT_PopFeatureViewers(DOTFeatListPtr flp)
{
  BioseqPtr query_bsp, subject_bsp;
  DOTPopFeat dpf;
  DOTRowPtr  drp1, drp2;
  Int2       fontH;
  DOTMainDataPtr mip;

  mip=flp->mip;
  if (!mip) return;
  drp1 = (DOTRowPtr)MemNew(sizeof(DOTRow));
  flp->query_drp=drp1;
  drp2=(DOTRowPtr)MemNew(sizeof(DOTRow));
  flp->subject_drp=drp2;

  fontH = FontHeight();
  flp->fontHt = fontH;
  /* seqmgrexplorefeatures goes through features in order left->right*/
  query_bsp=mip->qbsp;
  memset(&dpf, 0, sizeof(dpf));
  dpf.TopParentView=flp->Query;
  dpf.TopParentSeg= CreateSegment(flp->segQuery, 1, 0);
  dpf.nfeats = 1;
  dpf.fontHeight=fontH;
  dpf.drp=flp->query_drp; 
  dpf.dfp_cur=NULL;
  dpf.featindex=flp->featindex;
  flp->qFeatscount=SeqMgrExploreFeatures (query_bsp, (Pointer) &dpf, DOT_AddFeaturesToViewer, NULL, NULL, NULL);

  subject_bsp=mip->sbsp;
  memset(&dpf, 0, sizeof(dpf));
  dpf.TopParentView=flp->Subject;
  dpf.TopParentSeg= CreateSegment(flp->segSubject, 1, 0) ;
  dpf.nfeats = 1;
  dpf.fontHeight=fontH;
  dpf.drp=flp->subject_drp; 
  dpf.drp->dfp=NULL; 
  dpf.dfp_cur=NULL;
  dpf.featindex=flp->featindex;
  flp->sFeatscount=SeqMgrExploreFeatures (subject_bsp, (Pointer)&dpf, 
                          DOT_AddFeaturesToViewer, NULL, NULL, NULL);
}

static void DOT_HideAcceptProc(ButtoN b)
{

}

static void DOT_HideFeatList(ButtoN b)
{
    WindoW hHideDlg;
    DOTFeatListPtr flp;
    Int4 i, numrows;
    Boolean status;
    DOTFeatIndexPtr fdindex;

	hHideDlg = (WindoW)ParentWindow(b);
	if(hHideDlg == NULL) return;	
	flp = (DOTFeatListPtr) GetObjectExtra(hHideDlg);
   if(flp == NULL) return;
   numrows = flp->numrows;
   fdindex = flp->featindex;

    for(i = 1; i <= numrows; i++)
      {
        if(!GetItemStatus(flp->featList, i))
          {
            fdindex[fdindex[i].deref].show=FALSE;
          }
        else
          fdindex[fdindex[i].deref].show=TRUE;
      }
    DOT_UpdateFeatViewer(flp, flp->Query, flp->segQuery, flp->segQName, flp->segQCursor, DOT_QViewerClickProc, flp->query_drp, ((DOTSelDataPtr)flp->data)->q_start, &(flp->vert_Qpos));
    DOT_UpdateFeatViewer(flp, flp->Subject, flp->segSubject, flp->segSName, flp->segSCursor, DOT_SViewerClickProc, flp->subject_drp, ((DOTSelDataPtr)flp->data)->s_start, &(flp->vert_Spos));
    
    Remove(hHideDlg);
}

static void DOT_SetHideList(DOTFeatListPtr flp)
{
  Int4 i;
  DOTFeatIndexPtr fdindex;
  
  fdindex=flp->featindex;

  for(i = 1; i <= FEATDEF_MAX; i++) 
    {
      if (fdindex[fdindex[i].deref].present)
        {
          if (fdindex[fdindex[i].deref].show)
            SetItemStatus(flp->featList, i, TRUE);
          else
            SetItemStatus(flp->featList, i, FALSE);
        }
    }
}

static void DOT_HideFeatDlg(DOTFeatListPtr flp)
{
    Char szName[181], szName2[161];
    Int4 i, numrows;
    GrouP g, hg;
    ButtoN b;
    DOTFeatIndexPtr fdindex;

    if (flp==NULL) return;
    flp->hFeatDlg=NULL;

    flp->hFeatDlg = MovableModalWindow(-50, -20, -10, -10, "Hide/Show Features", NULL);
    if (flp->hFeatDlg==NULL) return;

    SetObjectExtra (flp->hFeatDlg, (void *)flp, NULL);


    hg = HiddenGroup(flp->hFeatDlg, 1, 2, NULL);
    g = NormalGroup(hg, 1, 2, "Choose Features to show:", systemFont, NULL);

    fdindex = flp->featindex;
    flp->featList = MultiList(g,20, 6, NULL);
    numrows=1;
    for(i = 1; i < FEATDEF_MAX; i++) 
      {
        if (fdindex[i].present)
          {
            ListItem(flp->featList, fdindex[i].label);
            fdindex[numrows].deref=i;
            numrows++;
          }   
      }
    DOT_SetHideList(flp);

    flp->numrows=numrows-1;
    g = HiddenGroup(hg, 2, 1, NULL);
    SetGroupSpacing(g, 15, 15);
    b = DefaultButton(g, "OK", (BtnActnProc) DOT_HideFeatList);

    Show(flp->hFeatDlg);
    return;
}

static void DOT_HideFeatDlgItem(IteM i)
{
  DOTFeatListPtr flp;
  WindoW         FeatWin;

  FeatWin=(WindoW)ParentWindow(i);
  if (FeatWin==NULL)return;

  flp=(DOTFeatListPtr)GetObjectExtra(FeatWin);
  DOT_HideFeatDlg(flp);
}

/*________________________________________(DOT_BuildFeatGUI)_____________

  Purpose : Creates viewers for Features.

____________________________________________________________________*/

static WindoW DOT_BuildFeatGUI (DOTFeatListPtr flp)
{
  WindoW  FeatWin;
  DOTMainDataPtr mip;
  MenU    m1;
  VieweR  v1, v2;
  SegmenT seg1, seg2;
  GrouP   subg1, subg2, g;
  PrompT  pr1, pr2;
  Int2   Margins;
  Char   str[255];

	if (!flp) return(NULL);

	Margins=10*stdCharWidth;
	FeatWin = DocumentWindow(Margins,Margins ,-10, -10,"Features", NULL, DOT_ResizeFeatWindow);
   if (!FeatWin) return(NULL);
   SetObjectExtra (FeatWin, (Pointer) flp, NULL);

   flp->FeatWin=FeatWin;

   m1 = PulldownMenu (FeatWin, "Options");
   CommandItem(m1, "Hide ..", DOT_HideFeatDlgItem);
   CommandItem(m1, "Close", DOT_CloseFeatWindow);

	mip= flp->mip;


	g=HiddenGroup(FeatWin,2,0,NULL);

   subg1=HiddenGroup(g, 0, 2, NULL);
   sprintf(str, "%s(x-axis)", mip->qname);
   flp->QInfo=StaticPrompt (subg1, str , 0, 0 , systemFont, 'l');
	v1=CreateViewer(subg1, 150 , 200, TRUE, TRUE);	
	seg1=CreatePicture();
   
/*    AlignObjects(ALIGN_JUSTIFY, (HANDLE) pr1, (HANDLE) v1, NULL, NULL); */

	/*viewer for close up of features*/
   subg2=HiddenGroup(g, 0, 2, NULL);
   sprintf(str, "%s(y-axis)", mip->sname);
   flp->SInfo= StaticPrompt (subg2, str , 0, 0, systemFont, 'l');
   v2 = CreateViewer(subg2, 150,200,TRUE,TRUE);
   seg2 = CreatePicture();
 
   flp->Query=v1;
   flp->segQuery=seg1;
	flp->Subject=v2; /* lines on the control panel*/
	flp->segSubject=seg2;
   flp->segQName =CreateSegment(flp->segQuery, 1, 0);
   flp->segSName=CreateSegment(flp->segSubject, 1, 0);
   flp->segQCursor=CreateSegment(flp->segQuery, 2, 0);
   flp->segSCursor=CreateSegment(flp->segSubject, 2, 0);

/*    AlignObjects(ALIGN_JUSTIFY, (HANDLE) pr2, (HANDLE) v2, NULL, NULL); */

   RealizeWindow(FeatWin);
   DOT_ResizeFeatWindow(FeatWin);

	return(FeatWin);	
}

typedef struct dot_goto{
  TexT   txt;
  ButtoN highlight;
} DOTGoto, PNTR DOTGotoPtr;

static void DOT_GotoProc(ButtoN b, TexT txt, Int4 value)
{
  DOTSelDataPtr data;
  DOTVibDataPtr vdp2=NULL;
  Int4       pos=0, margin, q_start, s_start;
  Int4       xstart, xstop, ystart, ystop;
  Int4       Qstart, Sstart;
  RecT       rc;
  DOTAlnPtr  salp;


  vdp2=(DOTVibDataPtr)GetObjectExtra(ParentWindow(b));
  if(!vdp2) return;
  data= vdp2->data;
  salp = vdp2->sv->salp;
  if (salp->show == dot_plot){
    xstart = data->q_start+data->xstart;
    xstop = data->q_start+data->xstop;
    ystart = data->s_start+data->ystart;
    ystop = data->s_start+data->ystop;
    Qstart = data->q_start;
    Sstart = data->s_start;
  }
  else {
    xstart = salp->q_start;
    xstop = salp->q_stop;
    ystart = salp->s_start;
    ystop = salp->s_stop;
    Qstart = salp->q_start;
    Sstart = salp->s_start;
  }

  pos=DOT_GetValue(txt);
  if (1 == value){
    if (pos<xstart)
      pos=xstart;
    if (pos>xstop)
      pos=xstop;
    q_start=pos-Qstart;
    q_start=q_start+q_start/10;
    pos=DOT_WorldtoScreen(q_start,vdp2->charw/2);
  }
  else if (2 == value){
    if (pos<ystart)
      pos=ystart;
    if (pos>ystop)
      pos=ystop;
    s_start=pos-Sstart;
    s_start=s_start+s_start/10;
    pos=DOT_WorldtoScreen(s_start,vdp2->charw/2);
  }

  GetPosition(vdp2->sv->v2, &rc);
  margin=MAX((StringWidth(vdp2->xname)), (StringWidth(vdp2->yname)))+10;
  vdp2->sv->barp=pos+rc.left+margin;
  AttachPicture(vdp2->sv->v2, vdp2->sv->pict2, vdp2->sv->barp, INT4_MIN, UPPER_CENTER, 1, 1, NULL);
}

static void DOT_TopProc(ButtoN b)
{
  DOTGotoPtr   gtp;

  gtp = (DOTGotoPtr)GetObjectExtra(b);
  DOT_GotoProc(b, gtp->txt, 1);
}

static void DOT_BottomProc(ButtoN b)
{
  DOTGotoPtr   gtp;

  gtp = (DOTGotoPtr)GetObjectExtra(b);
  DOT_GotoProc(b, gtp->txt, 2);
}

/*_______________________________________________(DOT_SVSwitchDisplay)___

  Purpose : Change Display between Dotplot and Blast hits

____________________________________________________________________*/


static void DOT_SVSwitchDisplay(PopuP p)

{
  DOTVibDataPtr       vdp2=NULL;
  WindoW              w;
  Int4                value;
  DOTGotoPtr          gtp=NULL;
  

  gtp = (DOTGotoPtr) GetObjectExtra(p);
  w=(WindoW)ParentWindow(p);
  vdp2=(DOTVibDataPtr)GetObjectExtra(w);
  if (!vdp2 || !vdp2->alp) return;

  SetTitle(gtp->txt, "");
  value=GetValue(p);
  
  if (value==1) /* dot plot */
    {
      vdp2->showDotPlot=TRUE;
      vdp2->showALIGN=FALSE;
    }
  else if (value==2) /* blast aligns */
    {
      vdp2->showDotPlot=FALSE;
      vdp2->showALIGN=TRUE;
    }
  else if (value==3) /* both */
    {
      vdp2->showDotPlot=TRUE;
      vdp2->showALIGN=TRUE;
    }

  DOT_SVPopulateDiagViewer(vdp2);
  if (vdp2->sv->salp){
    MemFree(vdp2->sv->salp);
    vdp2->sv->salp=NULL;
  }
  DOT_SVPopulateSequenceViewer(vdp2);

}

static void DOT_HighlightProc(ButtoN b)
{
  DOTVibDataPtr vdp2 = NULL;
  DOTGotoPtr    gtp = NULL;

  gtp = (DOTGotoPtr) GetObjectExtra(b);
  vdp2 = (DOTVibDataPtr)GetObjectExtra(ParentWindow(b));
  if (!vdp2 || !gtp) return;
  
  if (vdp2->sv->highlight == SHOW_MISMATCHES) {
    vdp2->sv->highlight = SHOW_MATCHES;
    SetTitle (gtp->highlight, "Show Mismatches");
  }
  else {
    vdp2->sv->highlight = SHOW_MISMATCHES;
    SetTitle (gtp->highlight, "Show Matches");
  }
  DOT_SVPopulateSequenceViewer(vdp2);

}


/*________________________________________(DOT_SVBuildDiagViewer)_____________

  Purpose : Creates viewers for second window.

____________________________________________________________________*/

static WindoW DOT_SVBuildDiagViewer(DOTVibDataPtr vdp2)
{
  WindoW    wSequence;
  GrouP     g, g2, s, s2, s3, s4;
  PrompT    pr1, pr2; 
  VieweR    v1,v2;
  ButtoN    b;
  RecT      rc;
  SegmenT   pict1,pict2;
  Int2      Margins, pixwidth; 
  MenU      menu; 
  BaR       hsb;
  Char      zoombuf[]={"Decrease scale to zoom in .."};
  Char      str1[41]={""}, str2[41]={""};
  DOTGotoPtr   gtp;
  DOTMainDataPtr mip;
  Char      title[60]={""};

	if (!vdp2) return(NULL);   

	Margins=10*stdCharWidth;
   sprintf(title, "%s", vdp2->xname);
   StringCat(title, "  vs. ");
   StringCat(title, vdp2->yname);
	wSequence = DocumentWindow(Margins,Margins ,-10, -10, title, NULL, DOT_ResizeSequenceWindow);
	if (!wSequence) return(NULL);
   GetPosition (wSequence,&rc);
   pixwidth=1200; /* some approximate value */
   /* first top group */
   s = HiddenGroup (wSequence,1, 4, NULL);
   /*threshold bar*/
   s3 = HiddenGroup (s,5, 0, NULL);
   pr2=StaticPrompt (s3, "Threshold-Ramp:", 0, 3*vdp2->Fh/2,vdp2->Fnt, 'l');
   pr1=StaticPrompt (s3, "  20%", 0, 3*vdp2->Fh/2, vdp2->Fnt, 'l');
   vdp2->sdp.ScrollBar = ScrollBar (s3, 15, 5, DOT_ChangeSequenceViewerCutoff);
   pr1=StaticPrompt (s3, "100%", 0, 3*vdp2->Fh/2, vdp2->Fnt, 'l');
   PushButton(s3, "Close", DOT_CloseSequenceWindow);
   SetObjectExtra(vdp2->sdp.ScrollBar, vdp2, NULL);
   CorrectBarMax (vdp2->sdp.ScrollBar, 80); /* 100% */
   CorrectBarValue (vdp2->sdp.ScrollBar, 60);/* 100% */
   /* second top group */
   s2 = HiddenGroup (s, 0, 2, NULL);
   if (vdp2->mip && vdp2->alp){
     s4=HiddenGroup (s2, 4, 0, NULL);

   }
   else{
     s4=HiddenGroup(s2, 2, 0, NULL);
   }
   SetGroupMargins(s4, 10, 10);
   SetGroupSpacing(s4, 10,10);

   pr1 = StaticPrompt (s4, zoombuf, StringWidth(zoombuf)+10 , popupMenuHeight, vdp2->Fnt, 'l');
#ifdef WIN_MAC
   vdp2->sv->scale = PopupList (s4, TRUE, DOT_SVChangeScale);
#endif

#ifndef WIN_MAC
   vdp2->sv->scale = PopupList (s4, FALSE, DOT_SVChangeScale);
#endif
   if (vdp2->mip)
     vdp2->showDotPlot=TRUE;
   if (vdp2->alp){
     vdp2->showALIGN=TRUE;
   }
   if (vdp2->mip && vdp2->alp){
/*      pr1 = StaticPrompt (s4, "", StringWidth(zoombuf)+10 , popupMenuHeight, vdp2->Fnt, 'l'); */
     vdp2->sv->showp=PopupList (s4, FALSE, DOT_SVSwitchDisplay);
     PopupItem (vdp2->sv->showp, "- dot plot -");
     PopupItem (vdp2->sv->showp, "- blast -");
     PopupItem (vdp2->sv->showp, "- both -");
     SetValue (vdp2->sv->showp, 1);
   }
   sprintf(vdp2->iInfo,"Diag not selected");
   vdp2->Infopanel= StaticPrompt (s2, vdp2->iInfo, pixwidth, 3*vdp2->Fh/2, vdp2->Fnt, 'l');
   SetObjectExtra (vdp2->sv->scale, vdp2, NULL);
	v1=CreateViewer(s,600,500,TRUE,TRUE);
	pict1=CreatePicture();
   /* bottom group */
	g=HiddenGroup(wSequence,0,2,NULL);
   g2=HiddenGroup(g, 8, 0, NULL);
   SetGroupMargins(g2, 10, 10);
   SetGroupSpacing(g2, 10, 10);
   gtp=(DOTGotoPtr)MemNew(sizeof(DOTGoto));
   SetObjectExtra (vdp2->sv->showp, (Pointer) gtp, NULL);
   vdp2->Ggoto=g2;
   mip=vdp2->mip;
   StaticPrompt (g2, "    -Goto-", 0, 3*vdp2->Fh/2, vdp2->Fnt, 'l');
   gtp->txt = DialogText (g2, "", (Int2)4, NULL);
   StaticPrompt (g2, "  on  ", 0, 3*vdp2->Fh/2, vdp2->Fnt, 'l');
   MemSet((Pointer)title, '\0', sizeof(title));
   sprintf(title, "  Top  ");
   b=PushButton (g2, title, DOT_TopProc);
   StaticPrompt (g2, " or ", 0, 3*vdp2->Fh/2, vdp2->Fnt, 'l');
   SetObjectExtra(b, (Pointer)gtp, StdCleanupExtraProc);

   MemSet((Pointer)title, '\0', sizeof(title));
   sprintf(title, "  Bottom  ");
   b=PushButton (g2, title, DOT_BottomProc);
   SetObjectExtra(b, (Pointer)gtp, NULL);
   StaticPrompt (g2, "  sequence       ", 0, 3*vdp2->Fh/2, vdp2->Fnt, 'l');
   gtp->highlight = PushButton (g2, "Show Mismatches", DOT_HighlightProc);
   vdp2->sv->highlight = SHOW_MATCHES;
   SetObjectExtra(gtp->highlight, (Pointer)gtp, NULL);
   Disable(g2);

	v2=CreateViewer(g,600,150,FALSE,TRUE);
	pict2=CreatePicture();

	vdp2->sv->w=wSequence;
	vdp2->sv->v1=v1;
	vdp2->sv->pict1=pict1;
	vdp2->sv->v2=v2;
	vdp2->sv->pict2=pict2;

	SetObjectExtra (vdp2->sv->w, (Pointer) vdp2, NULL);
	AlignObjects(ALIGN_JUSTIFY, (HANDLE) v2, (HANDLE)g, NULL, NULL);
	SetColorCell((GraphiC)vdp2->sv->v1, 0,0,255,192);

   RealizeWindow(wSequence);
   DOT_ResizeSequenceWindow(wSequence);

  /* calculate initial scale */
   DOT_SVCalculateScaling(vdp2);

	/*populate the viewer : hits*/
   if (DOT_SVPopulateDiagViewer(vdp2)==FALSE)
    goto end;

  if (DOT_SVPopulateSequenceViewer(vdp2)==FALSE)
    goto end;

  return(wSequence);	

 end:
   
   ErrPostEx (SEV_WARNING, 0, 0, "%s", "Display functions failed");
   return NULL;

}

/*________________________________________(DOT_UpdateDataRects)_____________

  Purpose : Updates main window rect params in data struct.

____________________________________________________________________
*/
static void DOT_UpdateDataRects (DOTSelDataPtr data, RecT rc, DOTVibDataPtr vdp, Boolean updateSelectedRect)
{
  Int4  width, height, VFrom=vdp->sdp.VFrom, HFrom=vdp->sdp.HFrom;
  Int4  comp, dx, dy, xstart, ystart;


  DOT_AddRectMargins(&rc, vdp);

  /* update the limits of selectable region of parent window */
  width=MIN (rc.right-rc.left, vdp->curr_qlen - HFrom );
  height=MIN (rc.bottom-rc.top, vdp->curr_slen - VFrom );
    
  data->rcP.left=rc.left;
  data->rcP.top=rc.top;
  data->rcP.right=rc.left+width+2;
  data->rcP.bottom=rc.top+height+2;

  /* update the size of selected rect */
  if (updateSelectedRect)
    {
      xstart=ABS(data->q_start-vdp->xstart);
      ystart=ABS(data->s_start-vdp->ystart);      
      comp=vdp->comp;
      dx=data->H_pos-HFrom;
      dy=data->V_pos-VFrom;
      width=ABS(data->q_stop-data->q_start)/comp;
      height=ABS(data->s_stop-data->s_start)/comp;

      data->rcS.left=rc.left+((xstart)/comp)/* -HFrom */;
      data->rcS.right=data->rcS.left+width;
      data->rcS.top=rc.top+((ystart)/comp)/* -VFrom */;
      data->rcS.bottom=data->rcS.top+height;
    }
}

static void DOT_InitFeatIndex(DOTFeatIndexPtr fdindex)
{
  Int4  i;

  for(i=1; i<FEATDEF_MAX; i++)
    {
      fdindex[i].show=TRUE;
    }
}

/*________________________________________(DOT_MsgFunc)_____________

  Purpose : Message Callback for the 2nd window.

____________________________________________________________________*/
static Int2 LIBCALLBACK DOT_MsgFunc (OMMsgStructPtr ommsp)
{
  DOTVibDataPtr vdp2 = NULL;
  OMUserDataPtr      omudp;



  omudp = (OMUserDataPtr)(ommsp->omuserdata);
  vdp2 = (DOTVibDataPtr)omudp->userdata.ptrvalue;

  switch (ommsp->message)
    {
    case OM_MSG_DEL:
      break;
    case OM_MSG_CREATE:
      break;
    case OM_MSG_UPDATE:
      break;
    case OM_MSG_SELECT:
      
     break;
   case OM_MSG_DESELECT:
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
      break;
    default:
      break;
    }

  return OM_MSG_RET_OK;
    
}


/*________________________________________(DOT_ClickProc)_____________

  Purpose : Click proc for main window - no action.

____________________________________________________________________*/
static void DOT_ClickProc (PaneL p, PoinT pt)
{
  DOTSelDataPtr   data;
  RecT        rc, prc;
  DOTVibDataPtr  vdp;


  ObjectRect(p, &prc);
/*   data = (DOTSelDataPtr) GetObjectExtra (p); */
  vdp = (DOTVibDataPtr)GetObjectExtra(ParentWindow(p));
  if(vdp==NULL) return;
  data=(DOTSelDataPtr)vdp->data;
  if (!data) return;

  /* specify clickable region */
  DOT_UpdateDataRects(data, prc, vdp, FALSE);

  
  rc = data->rcP;

  if (!PtInRect (pt, &rc)) 
    {
      if (pt.y < rc.top) pt.y = rc.top;
      if (pt.y > rc.bottom) pt.y = rc.bottom;
      if (pt.x < rc.left) pt.x = rc.left;
      if (pt.x > rc.right) pt.x = rc.right;
    }

  fstpnt = pt;
  curpnt = pt;
  
  InvertMode();
  if (vdp->selectMode==dot_SEQVIEW)
    DOT_SelectFrameProc (p);
  else
    DOT_SelectLineProc(p);

  if (data->selected)
    data->rm_lastselected=TRUE;
  else
    data->selected=TRUE;

  SetObjectExtra(p, data, NULL);

}

/*________________________________________(DOT_DragProc)_____________

  Purpose : Drag proc for main window - no action.

____________________________________________________________________*/
static void DOT_DragProc (PaneL p, PoinT pt)
{
  DOTSelDataPtr data;
  RecT      rc;
  DOTVibDataPtr vdp;

  vdp=(DOTVibDataPtr)GetObjectExtra(ParentWindow(p));
  if (vdp==NULL) return;
  data = (DOTSelDataPtr) GetObjectExtra (p);
  


  InvertMode();
  if (vdp->selectMode == dot_SEQVIEW)
    DOT_SelectFrameProc(p);
  else
    DOT_SelectLineProc(p);
  
  rc=data->rcP;


  if (!PtInRect (pt, &rc)) 
    {
      if (pt.y < rc.top) pt.y = rc.top;
      if (pt.y > rc.bottom) pt.y = rc.bottom;
      if (pt.x < rc.left) pt.x = rc.left;
      if (pt.x > rc.right) pt.x = rc.right;
    }

  curpnt = pt;
  
  if (vdp->selectMode==dot_SEQVIEW)
    DOT_SelectFrameProc(p);
  else
    DOT_SelectLineProc(p);
    
  
  if (data->selected)
    {
      data->rm_lastselected=TRUE;
    }
  else
    data->selected = TRUE;
  
  SetObjectExtra(p, data, NULL);

}


/*__________________________(DOT_ReleaseProc)_____________

  Purpose : Release Proc for main window - calls up second window.

________________________________________________________*/
static void DOT_ReleaseProc(PaneL p, PoinT pt)
{
  DOTSelDataPtr   data;
  MenU        menu, gmenu;
  Int4        VFrom, HFrom, sFeatscount, qFeatscount;
  DOTVibDataPtr  vdp2=NULL, vdp=NULL;
  DOTMainDataPtr mip1=NULL;
  DOTAlignInfoPtr alp=NULL;
  DOTFeatListPtr     flp;
  DOTDiagPtr  PNTR hitlist;
  Boolean     xaxis_incr=TRUE, yaxis_incr=TRUE;
  RecT        rc, rcP;
  Int4        width, height, index, xscale, yscale, scale, Y_pos;
  Int2        dx, dy;
  GrouP       g;
  Char        infoBuf[255];
  


  vdp = (DOTVibDataPtr)GetObjectExtra(ParentWindow(p));
  if (!vdp) return;

  data = (DOTSelDataPtr) vdp->data;

  if (!data->selected) return;

  rc = data->rcP;

  if (!PtInRect (pt, &rc)) 
    {
      if (pt.y < rc.top) pt.y = rc.top;
      if (pt.y > rc.bottom) pt.y = rc.bottom;
      if (pt.x < rc.left) pt.x = rc.left;
      if (pt.x > rc.right) pt.x = rc.right;
    }
  curpnt = pt;
  mip1 = vdp->mip;
  alp=vdp->alp;

  VFrom  = vdp->sdp.VFrom; 
  HFrom = vdp->sdp.HFrom;


  
  if (vdp->selectMode == dot_SEQVIEW)
    {
      DOT_UpdatePt();
      if (vdp->ChildWin==NULL)
        vdp2=(DOTVibDataPtr) MemNew (sizeof(DOTVibData));
      else
        {
          vdp2 = (DOTVibDataPtr)GetObjectExtra(vdp->ChildWin);
          vdp->ChildWin=DOT_ClearLastWindow(vdp->ChildWin, TRUE);
        }
/*       if (vdp2==NULL) return; */

      InvertMode();
      DOT_SelectFrameProc(p);

      dx=HFrom-data->H_pos;
      dy=VFrom-data->V_pos;
      
      /* previous rect coordinates */
      data->old_rcS.left=data->rcS.left-dx;
      data->old_rcS.right=data->rcS.right-dx;
      data->old_rcS.top=data->rcS.top-dy;
      data->old_rcS.bottom=data->rcS.bottom-dy;
      /* new rect coordinates on parent window */
      data->rcS.left = fstpnt.x;
      data->rcS.top = fstpnt.y;
      data->rcS.right = curpnt.x;
      data->rcS.bottom = curpnt.y;      
      data->H_pos=HFrom;
      data->V_pos=VFrom;      
  /* map selected region to sequence(world) coordinates 
     plus or minus one to account for errors when rounding off */

      data->q_start = MAX((fstpnt.x  - rc.left  + HFrom - 1)*vdp->comp, 0) + vdp->xstart;
      data->q_stop = MIN(((curpnt.x - rc.left +(HFrom+1))*vdp->comp)+vdp->xstart, vdp->xstop);
 
      data->s_start = MAX((fstpnt.y  - rc.top   + (VFrom-1))*vdp->comp, 0) + vdp->ystart;
      data->s_stop = MIN(((curpnt.y - rc.top +(VFrom+1))*vdp->comp)+vdp->ystart, vdp->ystop);
      
      data->qlen=ABS(data->q_stop-data->q_start)+1;
      data->slen=ABS(data->s_stop-data->s_start)+1;

      /* create new sequence buffers */
      DOT_InitCInfo(vdp, vdp2, data);

      if (vdp2->mip){
        DOT_FillNewSeqBufs(vdp, vdp2, FALSE);
/*         DOT_GetSeqs(vdp2->mip, TRUE); */
        if (DOT_BuildHitList(vdp2->mip, TRUE, TRUE)<0)
          {
            data->selected=FALSE;
            data->rm_lastselected =TRUE;
            SetTitle(vdp->Infopanel, vdp->iInfo);
            DOT_UpdateMainPanel(vdp, FALSE);
            Beep();
            return;/* no hits */
          }
      }
    /*reset infopanel*/
      
      sprintf(infoBuf, "Selected ..   %s (horizontal) [%d..%d]   vs.   %s (vertical) [%d..%d]", vdp->xname, data->q_start, data->q_stop, vdp->yname, data->s_start, data->s_stop);
      SetTitle(vdp->Infopanel,infoBuf);
      DOT_UpdateMainPanel(vdp, FALSE);
      
      /* create second window */

      vdp->ChildWin=DOT_SVBuildDiagViewer(vdp2);

    }
  else if (vdp->selectMode == dot_FEATVIEW && vdp->mip)
    {
      InvertMode();
      DOT_SelectLineProc(p);     
      dx=HFrom-data->H_pos;
      dy=VFrom-data->V_pos;
      /* previous hair coordinates */
      data->old_rcS.left=data->rcS.left-dx;
      data->old_rcS.right=data->rcS.right-dx;
      data->old_rcS.top=data->rcS.top-dy;
      data->old_rcS.bottom=data->rcS.bottom-dy;
      /* new hair coordinates on parent window */
      data->rcS.left = curpnt.x;
      data->rcS.top = curpnt.y;
      data->rcS.right = curpnt.x;
      data->rcS.bottom = curpnt.y;
      data->H_pos=HFrom;
      data->V_pos=VFrom;
      if (vdp->strand1 == Seq_strand_plus)
        {
          data->q_start = MIN(MAX((curpnt.x - rc.left +(HFrom))*vdp->comp, 0)+vdp->xstart, vdp->xstop);
        }
      else
        {
          data->q_start = MAX(vdp->xstart-((curpnt.x - rc.left +(HFrom))*vdp->comp), vdp->xstop);
        }
      
      if (vdp->strand2==Seq_strand_plus)
        {
          data->s_start = MIN(MAX((curpnt.y - rc.top +(VFrom))*vdp->comp, 0)+ vdp->ystart, vdp->ystop);
        }
      else
        {
          data->s_start = MAX(vdp->ystart-((curpnt.y - rc.top + (VFrom))*vdp->comp), vdp->ystop);
        }

      DOT_UpdateMainPanel(vdp, TRUE);
      /* look for features in selected region */

      if (vdp->ChildWin !=NULL)
        {
          flp=(DOTFeatListPtr)GetObjectExtra(vdp->ChildWin);
        }
      else /* first pass */
        {
          flp=(DOTFeatListPtr)MemNew(sizeof(DOTFeatList));
          flp->data=vdp->data;
          flp->mip=vdp->mip;
          flp->featindex=(DOTFeatIndexPtr) MemNew(sizeof(DOTFeatIndex)*FEATDEF_MAX);
          DOT_InitFeatIndex(flp->featindex);
          vdp->ChildWin=DOT_BuildFeatGUI(flp);
          if (!vdp->ChildWin) return;
          DOT_PopFeatureViewers(flp);
          if (flp->qFeatscount==0 && flp->sFeatscount==0)
            {
              data->selected=FALSE;
              DOT_UpdateMainPanel(vdp, TRUE);
              MemFree(flp);
              ErrPostEx(SEV_WARNING, 0, 0, "no features on bioseqs");
              return;
            }
        }
      
      DOT_UpdateFeatViewer(flp, flp->Query, flp->segQuery, flp->segQName, flp->segQCursor, DOT_QViewerClickProc, flp->query_drp, ((DOTSelDataPtr)flp->data)->q_start, &(flp->vert_Qpos));
      DOT_UpdateFeatViewer(flp, flp->Subject, flp->segSubject, flp->segSName, flp->segSCursor, DOT_SViewerClickProc, flp->subject_drp, ((DOTSelDataPtr)flp->data)->s_start, &(flp->vert_Spos));
      sprintf(infoBuf, "Hairs .. X-axis (%s) [%d]  vs.  Y-axis (%s) [%d]", vdp->xname, data->q_start, vdp->yname, data->s_start);
      SetTitle(vdp->Infopanel,infoBuf);
      
    }

  Show(vdp->ChildWin);
}

/*________________________________________(DOT_InitDataStruct)_____________

  Purpose : Initialize data structure.

____________________________________________________________________*/
static void DOT_InitDataStruct (DOTVibDataPtr vdp)
{
  DOTSelDataPtr data;
  RecT       rc;
  
  
  ObjectRect(vdp->panel, &rc);
  InsetRect(&rc, 4, 4);

  data = (DOTSelDataPtr) MemNew (sizeof (DOTSelData));
  data->selected = FALSE;
  data->q_start = 0;
  data->q_stop = 0;
  data->s_start = 0;
  data->s_stop = 0;
  data->V_pos=0;
  data->H_pos=0;
  data->vdp = vdp;
  DOT_UpdateDataRects(data, rc, vdp, FALSE);
  /* initialize document left and top parameters */


  vdp->data = (VoidPtr)data;
  if (vdp->panel != NULL) 
    SetObjectExtra (vdp->panel, data, NULL);
  else
    return;
}
/*________________________________________(DOT_ReducesizeProc)_____________
  
Purpose : Increase compression of main window display.

____________________________________________________________________*/

static void DOT_ReduceSizeProc (IteM i) 
{
  WindoW      w, temport;
  RecT        rcP;
  DOTVibDataPtr  vdp;
  DOTSelDataPtr   data;
  Int4        xscale, yscale, VCurPos, HCurPos;
  BaR         vsb, hsb;

  

  w = (WindoW)ParentWindow(i);
  temport = SavePort(w);

  vdp = (DOTVibDataPtr)GetObjectExtra (w);
  if (vdp==NULL) return;

  data=(DOTSelDataPtr)vdp->data;

  Select(vdp->panel);
  ObjectRect(vdp->panel, &rcP);

  vdp->comp *=2;
 
  DOT_SetCurrSeqlen (vdp);
  DOT_SetScrlVals (vdp);

  vdp->sdp.XScrlPos = (vdp->sdp.XScrlPos*vdp->sdp.UnitX/2)/vdp->sdp.UnitX;
  vdp->sdp.YScrlPos = (vdp->sdp.YScrlPos*vdp->sdp.UnitY/2)/vdp->sdp.UnitY;

  /*current scroll status*/
  vsb = GetSlateVScrollBar ((SlatE) vdp->panel);
  VCurPos=GetBarValue(vsb);
  hsb = GetSlateHScrollBar ((SlatE) vdp->panel);
  HCurPos=GetBarValue(hsb);

  /* update scroll values*/
  DOT_VScrlUpdate(vdp, vsb, VCurPos);
  DOT_HScrlUpdate(vdp, hsb, HCurPos);

  DOT_UpdateDataRects(data, rcP, vdp, TRUE);

  InsetRect(&rcP, -1, -1);
  InvalRect (&rcP);
  RestorePort (temport);
  Update();
  
}

/*________________________________________(DOT_EnlargeSizeProc)_____________
  Purpose : Reduce compression of main window display.

____________________________________________________________________*/
static void DOT_EnlargeSizeProc (IteM i) 
{
  WindoW    w, temport;
  RecT      rcP;
  DOTVibDataPtr vdp;
  DOTSelDataPtr  data;
  Int4     xscale, yscale, VCurPos, HCurPos;
  BaR      vsb, hsb;

  w = (WindoW)ParentWindow(i);
  temport = SavePort(w);

  vdp = (DOTVibDataPtr)GetObjectExtra (w);
  if (vdp==NULL) return;

  data=(DOTSelDataPtr)vdp->data;

  Select(vdp->panel);
  ObjectRect(vdp->panel, &rcP);
  if (vdp->comp >= 2)
    {
      vdp->comp/= 2;
    }

  DOT_SetCurrSeqlen(vdp);
  DOT_SetScrlVals (vdp);
  DOT_UpdateDataRects(data, rcP, vdp, TRUE);

  vdp->sdp.XScrlPos = (vdp->sdp.XScrlPos*vdp->sdp.UnitX*2)/vdp->sdp.UnitX;
  vdp->sdp.YScrlPos = (vdp->sdp.YScrlPos*vdp->sdp.UnitY*2)/vdp->sdp.UnitY;

  /*current scroll status*/
  vsb = GetSlateVScrollBar ((SlatE) vdp->panel);
  VCurPos=GetBarValue(vsb);
  hsb = GetSlateHScrollBar ((SlatE) vdp->panel);
  HCurPos=GetBarValue(hsb);

  /* update scroll values*/
  DOT_VScrlUpdate(vdp, vsb, VCurPos);
  DOT_HScrlUpdate(vdp, hsb, HCurPos);

  InsetRect(&rcP, -1, -1);
  InvalRect (&rcP);
  RestorePort (temport);
  Update();
  
}

static void DOT_ImageSizeHasChanged(DOTVibDataPtr vdp)
{
  BaR      vsb, hsb;
  Int4     HCurPos, VCurPos;

  DOT_SetCurrSeqlen(vdp);
  DOT_SetScrlVals (vdp);

  /*current scroll status*/
  vsb = GetSlateVScrollBar ((SlatE) vdp->panel);
  VCurPos=GetBarValue(vsb);
  hsb = GetSlateHScrollBar ((SlatE) vdp->panel);
  HCurPos=GetBarValue(hsb);

  /* update scroll values*/
  DOT_VScrlUpdate(vdp, vsb, VCurPos);
  DOT_HScrlUpdate(vdp, hsb, HCurPos);

}

/*________________________________________(DOT_OriginalSizeProc)_____________

  Purpose : Resize the main window display to the original size.

____________________________________________________________________*/
static void DOT_OriginalSizeProc (IteM i) 
{
  WindoW    w, temport;
  RecT      rcP;
  DOTVibDataPtr vdp;
  DOTSelDataPtr  data;
  Int4     xscale, yscale;

  w = (WindoW)ParentWindow(i);
  temport = SavePort(w);

  vdp = (DOTVibDataPtr)GetObjectExtra (w);
  if (vdp==NULL) return;

  data=(DOTSelDataPtr)vdp->data;

  Select(vdp->panel);
  ObjectRect(vdp->panel, &rcP);
  
  vdp->comp = vdp->originalcomp;
  
  DOT_ImageSizeHasChanged(vdp);
  DOT_UpdateDataRects(data, rcP, vdp, TRUE);


  InsetRect(&rcP, -1, -1);
  InvalRect (&rcP);
  RestorePort (temport);
  Update();
  
}

/*_______________________________________________(DOT_GetInfoProc)___

  Purpose : 'About' information on software features.

____________________________________________________________________*/

static void DOT_GetInfoProc (IteM i)

{
  WindoW   w;

  w = FixedWindow(-50, -1, -1, -1, "Info", StdCloseWindowProc);
  StaticPrompt (w, "Dot plotter..", 0,  popupMenuHeight, programFont, 'l');

  RealizeWindow(w);
  Show (w);
}

/*_______________________________________________(DOT_FeatAnalysisProc)____
  

  Purpose : Bring up Feature analysis selection window.

____________________________________________________________________*/
static void DOT_FeatAnalysisProc(ButtoN b)
{
  WindoW  w;
  GrouP   g, g2;


  w = FixedWindow(-50, -1, -1, -1, "ACUTE", StdCloseWindowProc);

  g = HiddenGroup(w, 0, 2, NULL);
  g2 = HiddenGroup(g, 0, 2, NULL);
  RadioButton(g2, "Coils");
  RadioButton(g2, "SEG");

  RealizeWindow(w);
  Show (w);
}


/*_______________________________________________(DOT_GetValue)________

 Purpose: Get int value of input TexT

_______________________________________________________________________*/
extern Int4 DOT_GetValue (TexT t)
{
  Char str[20];
  Int4 val;

  GetTitle (t,  str,  sizeof(str));
  if (StringHasNoText(str))
    {
      ErrPostEx (SEV_WARNING, 0, 0, "%s", "missing parameter(s)");
      return -1;
    }

  val=atoi(str);

  return val;
}


/*_______________________________________________(DOT_DoParams)___

  Purpose : Recalculate dot plot with new parameters.

____________________________________________________________________*/

void DOT_DoParams(ButtoN b)
{
  WindoW w;
  DOTparamsinfoPtr pip;
  DOTVibDataPtr       vdp;
  DOTMainDataPtr      mip;
  RecT             rc;
  Char             wordsize[5], treelimit[20];

  WatchCursor();
  w=(WindoW)ParentWindow(b);
  if (!(pip=(DOTparamsinfoPtr)GetObjectExtra(b))) return;
  if (!(vdp=(DOTVibDataPtr)GetObjectExtra(w))) return;
  mip=vdp->mip;

  DOT_FreeHitsArray(mip->hitlist, mip->index);

  mip->word_size = DOT_GetValue(pip->word_size);
  mip->tree_limit = DOT_GetValue(pip->tree_limit);
  mip->first_pass=TRUE;
  mip->cutoff_score=0;  

  if (pip) MemFree(pip);

  if (DOT_BuildHitList(mip, TRUE, TRUE)<0) 
    {
      Message(MSG_ERROR, "DOT- No hits");
      return; /* no hits */
    }

  DOT_UpdateMainPanel(vdp, TRUE);
  ArrowCursor();
  Remove(w);
}

void DOT_CancelParams(ButtoN b)
{

  WindoW  w;
  DOTparamsinfoPtr pip;

  w=ParentWindow(b);
  pip=(DOTparamsinfoPtr)GetObjectExtra(b);
  if (pip) MemFree(pip);
  Remove(w);
}


void DOT_QuitProg(ButtoN b)
{

  WindoW  w;
  DOTparamsinfoPtr pip;
  DOTVibDataPtr       vdp2;

  w=ParentWindow(b);
  pip=(DOTparamsinfoPtr)GetObjectExtra(b);
  vdp2=(DOTVibDataPtr)GetObjectExtra(w);
  if (vdp2->mip){
    DOT_FreeMainInfo(vdp2->mip);
    if (vdp2->mip) MemFree(vdp2->mip);
  }
  if (vdp2) MemFree(vdp2);
  if (pip) MemFree(pip);
  Remove(w);
  QuitProgram();
}

/*____________________________________________(DOT_StartDOTPLOT)_____________


  Purpose : Starts Dot Plot with user parameters.

____________________________________________________________________*/

void DOT_StartDOTPLOT(ButtoN b)
{
  WindoW w;
  DOTparamsinfoPtr pip;
  DOTVibDataPtr       vdp;
  DOTMainDataPtr      mip;
  RecT             rc;
  Char             wordsize[5], treelimit[20];
  BioseqPtr        qbsp, sbsp;


  WatchCursor();
  w=(WindoW)ParentWindow(b);
  pip=(DOTparamsinfoPtr)GetObjectExtra(b);
  vdp=(DOTVibDataPtr)GetObjectExtra(w);
  mip=(DOTMainDataPtr)MemNew(sizeof(DOTMainData));
/*   DOT_FreeHitsArray(mip->hitlist, mip->index); */

  mip->q_start= DOT_GetValue(pip->xstart);
  mip->q_stop=DOT_GetValue(pip->xstop);
  mip->s_start=DOT_GetValue(pip->ystart);
  mip->s_stop=DOT_GetValue(pip->ystop);
  mip->qlen = (mip->q_stop-mip->q_start) + 1;
  mip->slen = (mip->s_stop-mip->s_start) + 1;
  mip->word_size = DOT_GetValue(pip->word_size);
  mip->tree_limit = DOT_GetValue(pip->tree_limit);
  mip->first_pass=TRUE;
  mip->cutoff_score=0; 
  if (pip) MemFree(pip);

  qbsp=mip->qbsp;
  sbsp=mip->sbsp;
  Remove(w);

  DOT_CreateAndStore(mip, qbsp, sbsp, mip->q_start, mip->q_stop, mip->s_start, mip->s_stop, mip->word_size, mip->tree_limit, FALSE);

}



void DOT_SetupParamsWindow(DOTVibDataPtr vdp, Boolean is_startup, Boolean is_nuc)
{
  GrouP g, g1, g2,p1, p2;
  GrouP mainzoomg, zoomg1, zoomg2;
  ButtoN  b1, b2;
  DOTparamsinfoPtr pip;
  DOTMainDataPtr   mip;
  WindoW  w;
  Char    str1[20], str2[20], str3[20], str4[20];


  ArrowCursor();

  pip=(DOTparamsinfoPtr)MemNew(sizeof(DOTparamsinfo));
  mip=vdp->mip;

  w = FixedWindow(-50, -25, -1, -1, "Parameters", StdCloseWindowProc);
  SetObjectExtra(w, (Pointer)vdp, NULL);

  if (is_startup)
    {
      g=HiddenGroup(w, 1, 3, NULL);
      mainzoomg = NormalGroup(g, 1, 2, "Zoom Parameters", systemFont, NULL);
      sprintf(str1, "%d", mip->q_start);
      sprintf(str2, "%d", mip->s_start);
      sprintf(str3, "%d", mip->q_stop);
      sprintf(str4, "%d", mip->s_stop);
      zoomg1=HiddenGroup(mainzoomg, 3, 1, NULL);
      StaticPrompt(zoomg1, "x-axis:", 0, 0, systemFont, 'l');
      pip->xstart = DialogText(zoomg1, str1, 5, NULL);
      pip->xstop = DialogText(zoomg1, str3, 5, NULL);
      zoomg2=HiddenGroup(mainzoomg, 3, 1, NULL);
      StaticPrompt(zoomg2, "y-axis:", 0, 0, systemFont, 'l');
      pip->ystart = DialogText(zoomg2, str2, 5, NULL);
      pip->ystop = DialogText(zoomg2, str4, 5, NULL);
    }
  else
    g=HiddenGroup(w, 1, 2, NULL);

  g1 = NormalGroup(g, 1, 2, "Parameters", systemFont, NULL);
  sprintf(str1, "%d", mip->word_size);
  sprintf(str2, "%d", mip->tree_limit);
  if (is_nuc)
    sprintf(str3, "(4 - 11):", NULL);
  else
    sprintf(str3, "(1, 2 or 3):", NULL);

  p1=HiddenGroup(g1, 3, 1, NULL);
  StaticPrompt(p1, "Word size ", 0, 0, systemFont, 'l');
  StaticPrompt(p1, str3, 0, 0, systemFont, 'l');
  pip->word_size = DialogText(p1, str1, 5, NULL);

  p2=HiddenGroup(g1, 2, 1, NULL);
  StaticPrompt(p2, "Max number of hits:", 0, 0, systemFont, 'l');
  pip->tree_limit = DialogText(p2, str2, 5, NULL);

  if (!is_startup)
    {
      g2=HiddenGroup(g, 2, 1, NULL);
      b1 =PushButton(g2, "Accept", DOT_DoParams);
      b2 =PushButton(g2, "Cancel", DOT_CancelParams);
      SetObjectExtra(b1, (Pointer)pip, StdCleanupExtraProc);
      SetObjectExtra(b2, (Pointer)pip, StdCleanupExtraProc);
    }
  else
    {
      g2=HiddenGroup(g, 2, 1, NULL);
      b1 =PushButton(g2, "Accept", DOT_StartDOTPLOT);
      b2 =PushButton(g2, "Cancel", DOT_QuitProg);
      SetObjectExtra(b1, (Pointer)pip, StdCleanupExtraProc);
      SetObjectExtra(b2, (Pointer)pip, StdCleanupExtraProc);
    }
  
  Show(w);

}


static void DOT_ParametersProc (IteM i)
{
  DOTVibDataPtr          vdp;
  WindoW              temport, w;
  RecT                rc;
  
  w= (WindoW)ParentWindow(i);
  temport=SavePort(w);
  vdp=(DOTVibDataPtr)GetObjectExtra(w);

  if (vdp==NULL) return;

  DOT_SetupParamsWindow(vdp, FALSE, FALSE);

  Select(vdp->panel);
  ObjectRect(vdp->panel, &rc);
   
  InsetRect(&rc, -1, -1);
  InvalRect (&rc);
  RestorePort (temport);
  Update();

}


/*_________________________________________(DOT_GetNthSeqFromAlign)_____

  Purpose : Fill nucleotide sequence buffers.

____________________________________________________________________*/

static Uint1Ptr DOT_GetNthSeqFromAlign (SeqAlignPtr sap, Int4 n)
{
  BioseqPtr        bsp=NULL;
  SeqIdPtr         sip=NULL;
  SeqPortPtr       spp;
  Uint1Ptr         seq, temp_seq;
  Uint1            strand;
  Int4             offset, x;  
  Int4             start=0, stop=0;
  Int4             len, buf_len;
  Int2             ctr, i;    
  Uint1Ptr         buffer;
  Int4Ptr    PNTR  matrix;
  
  sip=AlnMgr2GetNthSeqIdPtr(sap, n);
  bsp=BioseqLockById(sip); 
  strand=AlnMgr2GetNthStrand(sap, n);
  AlnMgr2GetNthSeqRangeInSA(sap, n, &start, &stop);
  len=AlnMgr2GetAlnLength(sap, FALSE);
 /* initialize buffers */
  buffer=(Uint1Ptr) MemNew (sizeof(Uint1)*101); 
  MemSet((Pointer)buffer, '\0', sizeof(Char)*101);
  temp_seq = NULL;
  if (!(seq = (Uint1Ptr) MemNew (sizeof(Uint1)*(len)))) goto error;
  if (ISA_aa (bsp->mol)) 
    {
      spp = SeqPortNew (bsp, start, stop, strand, Seq_code_ncbi2na); 
    }
  else if (ISA_na(bsp->mol)) 
    {
      spp = SeqPortNew (bsp, start, stop, strand, Seq_code_ncbistdaa); 
    }
  if (spp == NULL)
    {
      ErrPostEx (SEV_ERROR, 0, 0, "%s", "DOT- Failed on SeqPortNew");
      goto error;
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
  BioseqUnlock(bsp);
  return seq;
 error:
  BioseqUnlock(bsp);
  return NULL;
}
/*_______________________________________________(DOT_DoZoom)___

  Purpose : Zoom into a specific region of sequence.

____________________________________________________________________*/

void DOT_DoZoom(ButtoN b)
{
  WindoW w;
  DOTparamsinfoPtr zip;
  DOTVibDataPtr     vdp;
  DOTSelDataPtr  data;
  DOTMainDataPtr mip;
  Int4           viewersize, newLen;
  RecT           rc;
  Char   xstart[20], xstop[20], ystart[20], ystop[20];


  w=(WindoW)ParentWindow(b);
  if (!(zip=(DOTparamsinfoPtr)GetObjectExtra(b))) return;
  if (!(vdp=(DOTVibDataPtr)GetObjectExtra(w))) return;
  mip=vdp->mip;
  data=vdp->data;
  Remove(w);

  DOT_FreeHitsArray(mip->hitlist, mip->index);


  mip->q_start= DOT_GetValue(zip->xstart);
  mip->q_stop=DOT_GetValue(zip->xstop);
  mip->s_start=DOT_GetValue(zip->ystart);
  mip->s_stop=DOT_GetValue(zip->ystop);
  mip->qlen=mip->q_stop-mip->q_start + 1;
  mip->slen=mip->s_stop-mip->s_start + 1;
  mip->first_pass = TRUE;
  mip->cutoff_score=0;  

  DOT_GetSeqs(mip, TRUE);
  if (DOT_BuildHitList(mip, TRUE, TRUE)<0) 
    {
      Message(MSG_ERROR, "DOT- No hits");
      return; /* no hits */
    } 
 
  ObjectRect(vdp->panel, &rc);
  viewersize=MIN(rc.right-rc.left,rc.bottom-rc.top)-vdp->HORZ_MARGIN;
  newLen=MAX(mip->qlen, mip->slen);
  vdp->comp=DOT_Compression(newLen, viewersize);

  DOT_ImageSizeHasChanged(vdp);

  DOT_UpdateMainPanel(vdp, TRUE);

  if (zip) MemFree(zip);
  
}



static void DOT_CancelZoom (ButtoN b)
{
  WindoW  w;
  DOTparamsinfoPtr zip;

  w=ParentWindow(b);
  zip=(DOTparamsinfoPtr)GetObjectExtra(b);
  if (zip) MemFree(zip);
  Remove(w);
}


void DOT_SetupZoomWindow(DOTVibDataPtr vdp)
{
  GrouP          g, zoomg, bottomg, maingroup;
  ButtoN         b1, b2;
  DOTparamsinfoPtr zip;
  DOTMainDataPtr   mip;
  WindoW         w;
  Char           str1[20], str2[20], str3[20], str4[20];

  if (!(zip=(DOTparamsinfoPtr)MemNew(sizeof(DOTparamsinfo)))) return;
  mip=vdp->mip;
  w = FixedWindow(-50, -25, -1, -1, "Zoom", StdCloseWindowProc);
  SetObjectExtra(w, (Pointer)vdp, NULL);
  maingroup=HiddenGroup(w, -1, 2, NULL);
  g = NormalGroup(maingroup, 1, 2, "Zoom Parameters", systemFont, NULL);
  sprintf(str1, "%d", mip->q_start);
  sprintf(str2, "%d", mip->s_start);
  sprintf(str3, "%d", mip->q_stop);
  sprintf(str4, "%d", mip->s_stop);
  zoomg=HiddenGroup(g, 3, 2, NULL);
  StaticPrompt(zoomg, "x-axis:", 0, 0, systemFont, 'l');
  zip->xstart = DialogText(zoomg, str1, 5, NULL);
  zip->xstop = DialogText(zoomg, str3, 5, NULL);
  StaticPrompt(zoomg, "y-axis:", 0, 0, systemFont, 'l');
  zip->ystart = DialogText(zoomg, str2, 5, NULL);
  zip->ystop = DialogText(zoomg, str4, 5, NULL);

  bottomg=HiddenGroup(maingroup, 2, 1, NULL);
  b1 =PushButton (bottomg, "Accept", DOT_DoZoom);
  b2 =PushButton(bottomg, "Cancel", DOT_CancelZoom);
  SetObjectExtra(b1, (Pointer)zip, StdCleanupExtraProc);
  SetObjectExtra(b2, (Pointer)zip, StdCleanupExtraProc);

  Show(w);
}

static void DOT_ZoomProc (IteM i)
{
  DOTVibDataPtr          vdp;
  WindoW              temport, w;
  RecT                rc;
  
  w= (WindoW)ParentWindow(i);
  temport=SavePort(w);
  vdp=(DOTVibDataPtr)GetObjectExtra(w);
  if (vdp==NULL) return;

  DOT_SetupZoomWindow(vdp); 
  
}


/*_______________________________________________(DOT_DoDisplayOpts)___

  Purpose : Update Display

____________________________________________________________________*/


static void DOT_DisplayOptsProc(ChoicE i)

{
  DOTVibDataPtr          vdp;
  WindoW              w;
  Int4                value;
  

  w=(WindoW)ParentWindow(i);
  vdp=(DOTVibDataPtr)GetObjectExtra(w);
  
  value=GetValue(i);
  
  if (value==1)
    {
      vdp->showDotPlot=TRUE;
      vdp->showALIGN=FALSE;
    }
  else if (value==2)
    {
      vdp->showDotPlot=TRUE;
      vdp->showALIGN=TRUE;
    }
  else if (value==3)
    {
      vdp->showDotPlot=FALSE;
      vdp->showALIGN=TRUE;
    }

  DOT_UpdateMainPanel(vdp, TRUE);
  
}


/*____________________________________________(DOT_AlignInfoNew)_____________


  Purpose : Initialize DOTAlignInfoPtr.

____________________________________________________________________*/
extern DOTAlignInfoPtr DOT_AlignInfoNew (void)
{
  DOTAlignInfoPtr alp;
  
  alp=(DOTAlignInfoPtr)MemNew(sizeof(DOTAlignInfo));
  alp->VERT_MARGIN=50;
  alp->HORZ_MARGIN=80;
  alp->do_scale=TRUE;
  alp->showLabels=FALSE;
  alp->Fh=FontHeight();
  return alp;
}
/*___________________________(AlnMgr2GetNextLine)_______


  Purpose : Get starts and stops for a seq_align.

____________________________________________________________________*/
static Boolean AlnMgr2GetNextLine(SeqAlignPtr sap, AlnMsg2Ptr amp1, AlnMsg2Ptr amp2, Int4Ptr x1, Int4Ptr y1, Int4Ptr x2, Int4Ptr y2, Int4Ptr n) 
{ 
  Boolean  retval; 
  
  if (sap == NULL || amp1 == NULL || amp2 == NULL || x1 == NULL || y1 == NULL || x2 == NULL || y2 == NULL ||n == NULL) 
    return FALSE;
  amp1->row_num = 1; 
  retval = AlnMgr2GetNextAlnBit(sap, amp1);
  if (retval == FALSE) 
    return FALSE; 
  if (amp1->type == AM_SEQ) 
    { 
      *x1 = amp1->from_row; 
      *x2 = amp1->to_row; 
    } else 
      { 
        if (*x2 == 0) 
          AlnMgr2GetNthSeqRangeInSA(sap, 1, x2, NULL); 
        *x1 = *x2;
      } 
  amp2->row_num = 2;
  retval = AlnMgr2GetNextAlnBit(sap, amp2);
  if (retval == FALSE) 
    return FALSE; 
  if (amp2->type == AM_SEQ) 
    { 
      *y1 = amp2->from_row;
      *y2 = amp2->to_row; 
    } else 
      { 
        if (*y2 == 0) 
          AlnMgr2GetNthSeqRangeInSA(sap, 2, y2, NULL); 
        *y1 = *y2;
      } 
  *n++;
  return TRUE; 
} 

/*____________________________________________(DOT_GetAlign)___________


  Purpose : Get seq_align coordinates, store as DOTAlnList array.

____________________________________________________________________*/
static Boolean DOT_GetAlign (DOTAlignInfoPtr alp)
{ 
  AlnMsg2Ptr    amp1, amp2;
  OMProcControlPtr ompcp;
  BioseqPtr    bsp1, bsp2;
  SeqAlignPtr  sap=NULL, salp=NULL, sap_tmp=NULL;
  SeqIdPtr     sip1, sip2;
  DOTAlnPtr    Aln;
  Boolean      saved=FALSE;
  Int4         x1, y1, x2, y2, numlines=0, n=0, i=0;
  Int4         xlen=0, ylen=0;
  Uint1         q_strand, s_strand;
  
   if (alp==NULL || alp->sap==NULL)
  {
    ArrowCursor();
    return(FALSE);
  }
  sap=alp->sap;

 if (sap->segtype==SAS_DISC)
    { 
      salp=(SeqAlignPtr)sap->segs;      
    } 
  else 
    {
      salp=sap; 
    } 
  alp->sip=AlnMgr2GetNthSeqIdPtr(salp, 1);
  alp->sip->next=AlnMgr2GetNthSeqIdPtr(salp, 2);
  alp->title = NULL;
  sap_tmp=salp;
  while (sap_tmp){
    numlines +=AlnMgr2GetNumSegs(sap_tmp);
    sap_tmp=sap_tmp->next;
  }
  amp1 = AlnMsgNew2(); 
  amp2 = AlnMsgNew2();
  amp1->from_aln = amp2->from_aln = 0;
  amp1->to_aln = amp2->to_aln = -1;  
  amp1->row_num = 1;
  amp2->row_num = 2;
  alp->Alnlist=(DOTAlnPtr PNTR) MemNew(sizeof(DOTAlnPtr)*numlines);
  n = x1 = x2 = y1 = y2 = i =xlen=ylen=0; 
  while (salp)
    {
      q_strand = AlnMgr2GetNthStrand(salp, 1);
      s_strand = AlnMgr2GetNthStrand(salp, 2);
      while (AlnMgr2GetNextAlnBit(salp, amp1) && AlnMgr2GetNextAlnBit(salp, amp2))
        { 
          if (amp1->type == AM_SEQ && amp2->type == AM_SEQ){
            saved=TRUE;
            Aln=(DOTAlnPtr)MemNew(sizeof(DOTAln));
            Aln->sap=salp;
            
            if (q_strand == Seq_strand_minus){
              Aln->q_start = x2;
              Aln->q_stop = x1;
            }
            else {
              Aln->q_start=x1;
              Aln->q_stop=x2;
            }
            if (s_strand == Seq_strand_minus){
              Aln->s_start = y2;
              Aln->s_stop = y1;
            }
            else {
              Aln->s_start=y1;
              Aln->s_stop=y2;
            }
            Aln->primID=i+1;
            Aln->entityID=salp->idx.entityID;
            Aln->itemID=salp->idx.itemID;
            Aln->class=dotaln_GENERAL;
            alp->Alnlist[i]=Aln;
            i++;
          }
        }
      AlnMsgReNew2(amp1);
      AlnMsgReNew2(amp2);
      amp1->to_aln = amp2->to_aln = -1;
      salp=salp->next;
    }
  
  if (!saved)
    {
      ErrPostEx(SEV_ERROR, 0, 0, "no alignments found");
      return (FALSE);
    }
  alp->index=i;
  AlnMsgFree2(amp1);
  AlnMsgFree2(amp2);

  bsp1=BioseqLockById(alp->sip); 
  bsp2=BioseqLockById(alp->sip->next); 
  alp->xstart=0;
  alp->xlen=bsp1->length;  
  alp->ystart=0;
  alp->ylen=bsp2->length; 
  BioseqUnlock(bsp1);
  BioseqUnlock(bsp2);
  return (TRUE);
} 

/*___________________________(DOT_FillEachAlign)___________


  Purpose : Fill Align structure from SeqAlignPtr PNTR

__________________________________________________________*/
static DOTAlnPtr DOT_FillEachAlign(DOTAlnPtr aln_list, SeqAlignPtr PNTR saps, Int4Ptr start, Int4 numsaps, Int4 class)
{
  Int4         i = 0;
  Int4         j = 0;
  AlnMsg2Ptr    amp1, amp2;
  DOTAlnPtr    Aln = NULL, Aln_head = NULL, aln_end = NULL, Aln_next = NULL;
  Uint1        q_strand, s_strand;


  amp1 = AlnMsgNew2(); 
  amp2 = AlnMsgNew2(); 
  amp1->from_aln = amp2->from_aln = 0;
  amp1->to_aln = amp2->to_aln = -1; 
  amp1->row_num = 1;
  amp2->row_num = 2;
  numsaps+=*start;
  if (aln_list){
    aln_end=aln_list;
    while (aln_end->next != NULL)
      aln_end=aln_end->next;
  }

  if (numsaps > 0)
    {
      for(i=*start; i<numsaps; i++){
        q_strand = AlnMgr2GetNthStrand(saps[i-*start], 1);
        s_strand = AlnMgr2GetNthStrand(saps[i-*start], 2);
        while(AlnMgr2GetNextAlnBit(saps[i-*start], amp1) && AlnMgr2GetNextAlnBit(saps[i-*start], amp2)) 
          { 
            if (amp1->type == AM_SEQ && amp2->type == AM_SEQ) {
              Aln=(DOTAlnPtr)MemNew(sizeof(DOTAln));
              Aln->sap=saps[i-*start];
              if (q_strand == Seq_strand_minus) {
                Aln->q_start = amp1->to_row;
                Aln->q_stop = amp1->from_row;
              }
              else {
                Aln->q_start = amp1->from_row;
                Aln->q_stop = amp1->to_row;
              }
              if (s_strand == Seq_strand_minus) {
                Aln->s_start = amp2->to_row;
                Aln->s_stop = amp2->from_row;
              }
              else {
                Aln->s_start = amp2->from_row;
                Aln->s_stop = amp2->to_row;
              }
              Aln->primID=j+1;
              Aln->entityID=saps[i-*start]->idx.entityID;
              Aln->itemID=saps[i-*start]->idx.itemID;
              Aln->class=class;
              if (Aln_head){
                Aln_next->next=Aln;
                Aln_next=Aln_next->next;
              }
              else {
                Aln_head=Aln_next=Aln;
              }
              j++;
            }
          }
      
        AlnMsgReNew2(amp1);
        AlnMsgReNew2(amp2);
        amp1->row_num = 1;
        amp2->row_num = 2;
      }
    }

  AlnMsgFree2(amp1);
  AlnMsgFree2(amp2);

  *start+=j;

  if (aln_list)
    aln_end->next=Aln_head;
  else 
    aln_list=Aln_head;

  return aln_list;
}
/*____________________________________________(DOT_IndexAlnlist)___________


  Purpose : index linked list of alignment coordinates into an array

____________________________________________________________________*/
static DOTAlnPtr PNTR DOT_IndexAlnlist(DOTAlnPtr aln_head, Int4Ptr index)
{
  DOTAlnPtr PNTR Alnlist=NULL;
  DOTAlnPtr aln_next;
  Int4      i=0;

  if (!aln_head || !index)
    return NULL;

  if(!(Alnlist=(DOTAlnPtr PNTR) MemNew(sizeof(DOTAlnPtr)* (*index)))){
    return NULL;
  }
  if (*index == 0 && aln_head) {
    i = 0;
    while (aln_head) {
      Alnlist[i] = aln_head;
      aln_head = aln_head->next;
      i++;
    }
    *index = i;
    return Alnlist;
  }

  for (i=0; i<*index; i++){
    Alnlist[i]=aln_head;
    aln_head=aln_head->next;
  }
  return Alnlist;
}
/*____________________________________________(DOT_FillAlignInfoPointer)___________


  Purpose : Get seq_align coordinates, store as DOTAlnList array.

____________________________________________________________________*/
extern Boolean DOT_FillAlignInfoPointer (DOTAlignInfoPtr alp)
{ 
  AlnMsg2Ptr    amp1, amp2;  
  DOTAlnPtr    PNTR Alnlist=NULL;
  DOTAlnPtr    aln_list=NULL;
  OMProcControlPtr ompcp;
  BioseqPtr    bsp1, bsp2;
  SeqAlignPtr  sap=NULL, salp=NULL, sap_tmp=NULL;
  Boolean      saved=FALSE;
  Int4         numsaps = 0, numlines = 0;
  Int4         i=0, n=0;
  Int4         xstart=0, xstop=0;
  Int4         ystart=0, ystop=0;
  Char         q_idbuf[42]={""}, s_idbuf[42]={""};
  AMAlignIndex2Ptr amaip=NULL;
  Int4         alncount=0;

  if (alp==NULL){
    ArrowCursor();
    return(FALSE);
  }
  sap=alp->sap;
  if (!sap) return FALSE;
  if (sap->saip != NULL && sap->saip->indextype == INDEX_PARENT){
    amaip=(AMAlignIndex2Ptr)sap->saip;
    if (!amaip) return FALSE;
    salp=(SeqAlignPtr)sap->segs;
  }
  else {
    amaip=NULL;
    salp=sap;
  } 

  n = AlnMgr2GetNumRows(salp);
  if (n <= 0){
    ArrowCursor();
       return(FALSE);
  }
  if (n > 2){
    ErrPostEx(SEV_ERROR, 0, 0, "alignment contains greater than two rows");
    SeqIdSetFree(alp->sip);
    ArrowCursor();
    return FALSE;
  }
  alp->sip=AlnMgr2GetNthSeqIdPtr(salp, 1);
  alp->sip->next=AlnMgr2GetNthSeqIdPtr(salp, 2);
  
  SeqIdWrite(alp->sip, q_idbuf,PRINTID_FASTA_SHORT, 41);
  SeqIdWrite(alp->sip->next, s_idbuf,PRINTID_FASTA_SHORT, 41);
  alp->name1=StringSave(q_idbuf);
  alp->name2=StringSave(s_idbuf);
  bsp1=BioseqLockById(alp->sip);
  bsp2=BioseqLockById(alp->sip->next);

  if (ISA_aa (bsp1->mol) && ISA_aa (bsp2->mol)) 
    {
      alp->is_na = FALSE;
    }
  else if (ISA_na(bsp1->mol) && ISA_na (bsp2->mol)) 
    {
      alp->is_na = TRUE;
      alp->matrix=(Int4Ptr PNTR)DOT_DNAScoringMatrix(-3, 1, 4);
    }
  else
    goto error;

   sap_tmp=salp;
   while (sap_tmp){
     sap_tmp=sap_tmp->next;
     numsaps++;
   }

  amp1 = AlnMsgNew2(); 
  amp2 = AlnMsgNew2(); 
  amp1->to_aln = amp2->to_aln = -1; 
  if (amaip){
    aln_list = DOT_FillEachAlign(aln_list, amaip->saps, &alncount, numsaps,  1);
    saved=TRUE;
  }
  else {
    aln_list = DOT_FillEachAlign(aln_list, &salp, &alncount, numsaps, 1);
    saved=TRUE;
  }

  if (!saved)
    {
       ErrPostEx(SEV_WARNING, 0, 0, "Invalid SeqAlign format");
       return (FALSE);
  }
  numlines = alncount;
  Alnlist = DOT_IndexAlnlist(aln_list, &numlines);
  if (!Alnlist) goto error;
  alp->Alnlist=Alnlist;
  AlnMgr2GetNthSeqRangeInSA(salp, 1, &xstart, &xstop);
  AlnMgr2GetNthSeqRangeInSA(salp, 2, &ystart, &ystop);
  alp->xstart=xstart;
  alp->xlen=xstop-xstart+1;
  alp->ystart=ystart;
  alp->ylen=ystop-ystart+1;
  alp->index=numlines;

  ArrowCursor();
  BioseqUnlock(bsp1);
  BioseqUnlock(bsp2);
  return TRUE;
 error:
  BioseqUnlock(bsp1);
  BioseqUnlock(bsp2);
  return FALSE;
}

/*____________________________________________(DOT_FillAlignInfoPointer)___________


  Purpose : Get seq_align coordinates, store as DOTAlnList array.

____________________________________________________________________*/
extern Boolean DOT_Prob_FillAlignInfoPointer (DOTAlignInfoPtr alp)
{ 
  AlnMsg2Ptr    amp1, amp2;  
  DOTAlnPtr    PNTR Alnlist=NULL;
  DOTAlnPtr    aln_list=NULL;
  OMProcControlPtr ompcp;
  BioseqPtr    bsp1, bsp2;
  SeqAlignPtr  sap=NULL, salp=NULL, sap_tmp=NULL;
  Boolean      saved=FALSE;
  Int4         numsaps = 0, numlines = 0;
  Int4         i=0, n=0;
  Int4         xstart=0, xstop=0;
  Int4         ystart=0, ystop=0;
  Char         q_idbuf[42]={""}, s_idbuf[42]={""};
  AMAlignIndex2Ptr amaip=NULL;
  Int4         alncount=0;

  if (alp==NULL){
    ArrowCursor();
    return(FALSE);
  }
  sap=alp->sap;
  if (!sap) return FALSE;
  
  if (sap->segtype==SAS_DISC){  
    salp=(SeqAlignPtr)sap->segs;  
  }  
  else { 
    salp=sap; 
  } 
  if (sap->saip == NULL)
    AlnMgr2IndexLite(sap);
  amaip = (AMAlignIndex2Ptr)sap->saip;
  
  n = AlnMgr2GetNumRows(salp);
  if (n <= 0){
    ArrowCursor();
       return(FALSE);
  }
  if (n > 2){
    ErrPostEx(SEV_ERROR, 0, 0, "alignment contains greater than two rows");
    SeqIdSetFree(alp->sip);
    ArrowCursor();
    return FALSE;
  }
  alp->sip=AlnMgr2GetNthSeqIdPtr(amaip->saps[0], 1);
  alp->sip->next=AlnMgr2GetNthSeqIdPtr(amaip->saps[0], 2);
  
  SeqIdWrite(alp->sip, q_idbuf,PRINTID_FASTA_SHORT, 41);
  SeqIdWrite(alp->sip->next, s_idbuf,PRINTID_FASTA_SHORT, 41);
  alp->name1=StringSave(q_idbuf);
  alp->name2=StringSave(s_idbuf);
  bsp1=BioseqLockById(alp->sip);
  bsp2=BioseqLockById(alp->sip->next);
  if (bsp2 == NULL){
    if (bsp1 != NULL)
      BioseqUnlock(bsp1);
  }
  if (ISA_aa (bsp1->mol) && ISA_aa (bsp2->mol)) 
    {
      alp->is_na = FALSE;
    }
  else if (ISA_na(bsp1->mol) && ISA_na (bsp2->mol)) 
    {
      alp->is_na = TRUE;
      alp->matrix=(Int4Ptr PNTR)DOT_DNAScoringMatrix(-3, 1, 4);
    }
  else
    goto error;

  amp1 = AlnMsgNew2(); 
  amp2 = AlnMsgNew2(); 
  amp1->to_aln = amp2->to_aln = -1; 
  if (amaip){
    aln_list = DOT_FillEachAlign(aln_list, amaip->saps, &alncount, amaip->numsaps,  1);
    saved=TRUE;
  }

  if (!saved)
    {
       ErrPostEx(SEV_WARNING, 0, 0, "no alignments saved");
       return (FALSE);
  }
  numlines = alncount;
  Alnlist = DOT_IndexAlnlist(aln_list, &numlines);
  if (!Alnlist) goto error;
  alp->Alnlist=Alnlist;
  alp->index=numlines;

  if (sap->segtype == SAS_DISC){
    alp->xstart = 1;
    alp->xlen = bsp1->length;
    alp->ystart = 1;
    alp->ylen = bsp2->length;
  }
  else {
    AlnMgr2GetNthSeqRangeInSA(salp, 1, &xstart, &xstop);
    AlnMgr2GetNthSeqRangeInSA(salp, 2, &ystart, &ystop);
    alp->xstart=xstart;
    alp->xlen=xstop-xstart+1;
    alp->ystart=ystart;
    alp->ylen=ystop-ystart+1;
  }

  ArrowCursor();
  BioseqUnlock(bsp1);
  BioseqUnlock(bsp2);
  return TRUE;
 error:
  BioseqUnlock(bsp1);
  BioseqUnlock(bsp2);
  return FALSE;
}

static SeqIdPtr DOT_GetNthSeqIdFromScp(SCP_ResultPtr scp, Int2 n)
{
  SeqIdPtr sip;
  SeqAlignPtr PNTR saps;

  
  if (scp->saps != NULL)
    saps=scp->saps;
  else if (scp->large_outliers !=NULL)
    saps=scp->large_outliers;
  else if (scp->small_outliers !=NULL)
    saps=scp->small_outliers;
  else
    return NULL;

  return(AlnMgr2GetNthSeqIdPtr(saps[0], n));  

}


/*____________________________________________(DOT_FillFromScp)___________


  Purpose : Get seq_align coordinates, store as DOTAlnList array.

____________________________________________________________________*/
static Boolean DOT_FillFromScp (DOTAlignInfoPtr alp, SCP_ResultPtr scp)
{ 
  BioseqPtr    bsp1=NULL, bsp2=NULL;
  DOTAlnPtr    PNTR Alnlist=NULL;
  DOTAlnPtr    aln_list =NULL;
  Char         text1[42];
  Char         text2[42];
  Int4         start=0;
  Boolean      saved=FALSE;
  Char         q_idbuf[42]={""}, s_idbuf[42]={""};

  if (alp==NULL)
  {
    ArrowCursor();
    return(FALSE);
  }
  if (scp->saps == NULL && scp->small_outliers ==NULL && scp->large_outliers==NULL) return (FALSE);

  alp->sip = DOT_GetNthSeqIdFromScp(scp, 1);
  alp->sip->next = DOT_GetNthSeqIdFromScp(scp, 2);
  SeqIdWrite(alp->sip, q_idbuf,PRINTID_FASTA_SHORT, 41);
  SeqIdWrite(alp->sip->next, s_idbuf,PRINTID_FASTA_SHORT, 41);
  alp->name1=StringSave(q_idbuf);
  alp->name2=StringSave(s_idbuf);
  bsp1=BioseqLockById(alp->sip);
  bsp2=BioseqLockById(alp->sip->next);

  if (bsp2 == NULL){
    if (bsp1 != NULL)
      BioseqUnlock(bsp1);
  }

  if (ISA_aa (bsp1->mol) && ISA_aa (bsp2->mol)) 
    {
      alp->is_na = FALSE;
    }
  else if (ISA_na(bsp1->mol) && ISA_na (bsp2->mol)) 
    {
      alp->is_na = TRUE;
      alp->matrix=(Int4Ptr PNTR)DOT_DNAScoringMatrix(-3, 1, 4);
    }
  else
    goto error;

  if (scp->saps){
    aln_list = DOT_FillEachAlign(aln_list, scp->saps, &start, scp->numsaps, 1);
    saved=TRUE;
  }
  if (scp->large_outliers){
/*     start= scp->numsaps; */
    aln_list = DOT_FillEachAlign(aln_list, scp->large_outliers, &start, scp->numlarge_outliers, 2);
    saved=TRUE;
  }
  if (scp->small_outliers){
/*     start= scp->numsaps+scp->numlarge_outliers; */
    aln_list = DOT_FillEachAlign(aln_list, scp->small_outliers, &start, scp->numsmall_outliers, 3);
    saved=TRUE;
  }
  if (!saved)
    {
      ErrPostEx(SEV_WARNING, 0, 0, "no alignments saved");
      goto error;
    }
  Alnlist = DOT_IndexAlnlist(aln_list, &start);
  if (!Alnlist) goto error;
  alp->Alnlist=Alnlist;
  alp->xstart=1;
  alp->xlen=scp->len1;
  alp->ystart=1;
  alp->ylen=scp->len2;
  alp->index=start;
   ArrowCursor();
   BioseqUnlock(bsp1);
   BioseqUnlock(bsp2);
   return TRUE;
 error:
   BioseqUnlock(bsp1);
   BioseqUnlock(bsp2);
   return (FALSE);
} 
/*_______________________________________________(DOT_DoBlast)___

  Purpose : Calls Blast2Seq.

____________________________________________________________________*/

static FloatHi DOT_get_eval(Int4 exp)
{
  FloatHi eval;
  Int4 i;

  eval = 1;
  for (i=1; i<=exp; i++)
  {
     eval = eval/10;
  }
  return eval;
}

static void DOT_DoBlast (ButtoN b)
{
  BLAST_OptionsBlkPtr options;
  SeqAlignPtr         sap, sap_final;
  DOTVibDataPtr       vdp;
  WindoW              temport, w;
  RecT                rc;
  DOTblastinfoPtr     bip;
  BioseqPtr           bsp1, bsp2;
  Int4                e;
  Int2                i;
  Int2                progval;
  Uint2               entityID=0;
  CharPtr             text=NULL;
  Boolean             is_local;
  Char                eval[11]={""};
  Char                str[20]={""};
  SeqEntryPtr         sep;
  SeqAnnotPtr         sanp;
  SCP_ResultPtr       scp=NULL;
  DOTMainDataPtr      mip=NULL;


  WatchCursor();
  w = (WindoW)ParentWindow(b);
  vdp = (DOTVibDataPtr)GetObjectExtra(w);
  if (vdp == NULL)  goto end;
  mip = vdp->mip;
  if (mip == NULL) goto end;

  Remove(w);

  bip = (DOTblastinfoPtr)GetObjectExtra(b);
  if (bip == NULL || bip->bsp1 == NULL || bip->bsp2 == NULL)
    goto end;
  bsp1 = bip->bsp1;
  bsp2 = bip->bsp2;

  progval = GetValue (bip->progname);
  if (progval == 1) 
    StringCpy(str, "blastn");
  else 
    StringCpy(str, "blastp");

  i = GetValue(bip->localorglobal);
  if (i == 1)
    is_local = TRUE;
  else
    is_local = FALSE;

   if (vdp->alp){ /* previous alignment*/
     vdp->alp->pict = NULL;
     DOT_ExitAlign(vdp->alp);
     vdp->showALIGN = FALSE;
   }
   
   scp = SCP_CompareOrderOrganizeBioseqs(bsp1, bsp2, mip->qslp, mip->sslp, str, mip->word_size, mip->tree_limit);
   Remove(w);
   vdp->showALIGN=TRUE;
   SetValue(vdp->displayOpts2, 2);
   vdp->alp=DOT_AlignInfoNew();
   vdp->alp->sap=NULL;
   vdp->alp->entityID=scp->saps[0]->idx.entityID;
   vdp->alp->itemID=scp->saps[0]->idx.itemID;
   if (!DOT_FillFromScp(vdp->alp, scp))
     {
       vdp->alp->pict=NULL;
       DOT_ExitAlign(vdp->alp);
       vdp->showALIGN=FALSE;
     }

   DOT_UpdateMainPanel(vdp, TRUE);
   end:
   if (text) MemFree(text);
   ArrowCursor();
   return;
}


static void DOT_CancelBlast (ButtoN b)
{
  WindoW  w;
  DOTblastinfoPtr bip;

  w=ParentWindow(b);
  bip=(DOTblastinfoPtr)GetObjectExtra(b);
  if (bip) MemFree(bip);
  Remove(w);
}


void DOT_SetupBlastWindow(DOTVibDataPtr vdp)
{
   DOTblastinfoPtr   bip;
   DOTMainDataPtr    mip;
   ButtoN            b;
   ButtoN            b1;
   GrouP             maingroup, topg, globalg, localg, gapsg, eANDwg;
   GrouP             submitg, maskg, blastg, bottomg, g1, g2, g3;
   Char              title[255]={""};
   WindoW            w;
   


   if (!(bip = (DOTblastinfoPtr)MemNew(sizeof(DOTblastinfo)))) return;
   mip=vdp->mip;

   bip->bsp1 = mip->qbsp;
   bip->bsp2 = mip->sbsp;
   StringCat(title, mip->qname);
   StringCat(title, " vs. ");
   StringCat(title, mip->sname);
   w = FixedWindow(-50, -25, -1, -1, title, StdCloseWindowProc);
   SetObjectExtra(w, vdp, NULL);

   maingroup = HiddenGroup(w, 1, 4, NULL);  
   StaticPrompt(maingroup, "Blast2Seqs Options ..", 0, popupMenuHeight, systemFont, 'l');
   topg = HiddenGroup (maingroup, -1, 2, NULL);
   blastg = NormalGroup(topg, 1,1, "Blast Program",  systemFont,NULL);
   bip->progname = HiddenGroup(blastg, 5, 0, NULL);
   RadioButton(bip->progname, "blastn");
   RadioButton(bip->progname, "blastp");
/*    RadioButton(bip->progname, "blastx"); */
/*    RadioButton(bip->progname, "tblastn"); */
/*    RadioButton(bip->progname, "tblastx"); */
   SetValue(bip->progname, 1);

   /*
   globalg = NormalGroup(topg,1, 1, "Alignment Type",  systemFont,NULL);
   bip->localorglobal = HiddenGroup(globalg, 2, 1, NULL);
   RadioButton(bip->localorglobal, "Local");
   RadioButton(bip->localorglobal, "Global");
   SetValue(bip->localorglobal, 1);

   bottomg=HiddenGroup(maingroup, 1, 2, NULL);
   localg = NormalGroup(bottomg, 1, 3, "Local Alignment Options", systemFont, NULL);
   g1 = NormalGroup(localg, 1,1, "",  systemFont,NULL);
   gapsg = HiddenGroup(g1, 3, 2, NULL);
   bip->gapped = HiddenGroup(gapsg, 0, 2, NULL);
   RadioButton(bip->gapped, "gapped");
   RadioButton(bip->gapped, "ungapped");
   SetValue(bip->gapped, 1);

   g2 = NormalGroup(localg, 1,1, "",  systemFont,NULL);
   maskg=HiddenGroup(g2, 0, 2, NULL);
   bip->maskrep = CheckBox(maskg, "Mask Repeats", NULL);
   SetStatus(bip->maskrep, FALSE);
   bip->masksimple = CheckBox(maskg, "Mask Simple Sequence", NULL);
   SetStatus(bip->masksimple, TRUE);
   */

   bottomg=HiddenGroup(maingroup, 1, 2, NULL);

   localg = NormalGroup(bottomg, 1, 3, "Local Alignment Options", systemFont, NULL);

   g3 = NormalGroup(localg, 1,1, "",  systemFont,NULL);
   eANDwg = HiddenGroup(g3, 2, 2, NULL);
   StaticPrompt(eANDwg, "hitlist:  e-", 0, 0, systemFont, 'l');
   bip->eval = DialogText(eANDwg, "10", 5, NULL);
   StaticPrompt(eANDwg, "wordsize:", 0, 0, systemFont, 'l');
   bip->wordsize = DialogText(eANDwg, "11", 5, NULL);


   submitg=HiddenGroup(bottomg, 2, 0, NULL);
   b = PushButton(submitg, "BLAST", DOT_DoBlast);
   b1 = PushButton(submitg, "Cancel", DOT_CancelBlast);
   SetObjectExtra(b1, (Pointer)bip, StdCleanupExtraProc);
   SetObjectExtra(b, (Pointer)bip, StdCleanupExtraProc);
   Show(w);

   return;


}


static void DOT_Blast2SeqProc (IteM i)

{
  DOTVibDataPtr          vdp;
  WindoW              w;
  
  w= (WindoW)ParentWindow(i);
  vdp=(DOTVibDataPtr)GetObjectExtra(w);
  if (vdp==NULL) return;
  Enable(vdp->displayOpts1);
  DOT_SetupBlastWindow(vdp); 
}

/*________________________________________(DOT_FreeAlnList)_____________


  Purpose : Free list of alignments in align list, then free ptr.

____________________________________________________________________*/
static void  DOT_FreeAlnList(DOTAlignInfoPtr alp)
{
  Int4 j, index;
  DOTAlnPtr PNTR alnL, aln;

  if (alp->Alnlist) 
    {
      alnL=alp->Alnlist;
      index=alp->index;
      
      for(j = 0; j < index; j++) 
        {
          aln=alnL[j];
          if (aln) MemFree(aln);
        }
      
      if (alnL) MemFree(alnL);
    }
}
/*____________________________________________(DOT_ExitAlign)___________


  Purpose : Clears alignment structure

____________________________________________________________________*/
static void DOT_ExitAlign(DOTAlignInfoPtr alp){


  if (alp->Alnlist)
    DOT_FreeAlnList(alp);
  
  if (alp->pict)
    DeletePicture(alp->pict);
  if (alp->is_na && alp->matrix)
    Free(alp->matrix);

  if (alp) MemFree(alp);

}
/*_______________________________________________(DOT_QuitMainWindow)___

  Purpose : Quit Main window.

____________________________________________________________________*/

static void DOT_CloseMainWindow (IteM i)

{
  WindoW  w;
  DOTVibDataPtr vdp;
  DOTSelDataPtr  data;

  w=(WindoW)ParentWindow(i);
  vdp=(DOTVibDataPtr)GetObjectExtra(w);  
  data=(DOTSelDataPtr)vdp->data;
  if (data) MemFree(data);

  if (vdp->xname) MemFree(vdp->xname);
  if (vdp->yname) MemFree(vdp->yname);
  if (vdp->mip)
    DOT_FreeMainInfoPtrEx(vdp->mip);

  if (vdp->alp)
    {
      if (vdp->alp->Alnlist)
        DOT_FreeAlnList(vdp->alp);
      if (vdp->alp) MemFree(vdp->alp);
    }
  if (vdp) MemFree(vdp);
  
  Remove(w);
}

static void DOT_ExitProgram(IteM i)
{
  DOT_CloseMainWindow(i);
  QuitProgram();
}
/*________________________________________(DOT_GridProc)_____________

  Purpose : Show or Hide the grid on Main window.

____________________________________________________________________*/

static void DOT_GridProc (ChoicE i) 
{
  WindoW      w, temport;
  RecT        rcP;
  DOTVibDataPtr  vdp;
  Int4        value;
  

  w = (WindoW)ParentWindow(i);
  vdp = (DOTVibDataPtr)GetObjectExtra (w);

  value = GetValue(i);
  if (value==1)
    vdp->showGrid = TRUE;
  else
    vdp->showGrid = FALSE;

  DOT_UpdateMainPanel(vdp, TRUE);
  
}

/*________________________________________(DOT_CreateWindowDetails)____

  Purpose : Create buttons on the main window.

*/
static void DOT_CreateWindowDetails (WindoW w, DOTVibDataPtr vdp)
{
  GrouP          g, g1, g2;
  Int4           index, maxzoom, wvals;
  Char           str[15], PNTR qname, PNTR sname;
  Int4           q_start, q_stop, s_start, s_stop;
  RecT           rc;
  ButtoN         blastButton;
  BaR 			  vsb;
  BaR 			  hsb;
  Int4           Xpixels, Ypixels, pixwidth, ychar;
  DOTSelDataPtr      data;
  DOTMainDataPtr     mip=NULL;
  DOTAlignInfoPtr    alp=NULL;
  PrompT             pr1, pr2;

  
  if (vdp->mip)
    mip=vdp->mip;
  if (vdp->alp)
    alp=vdp->alp;
  ObjectRect(w, &rc);
  pixwidth=rc.right-rc.left-10;
  g=HiddenGroup(w, 0, 3, NULL); 
  SetGroupSpacing(g, 10, 20);
  g1=HiddenGroup(g, 7, 0, NULL);
  g2=HiddenGroup (g,4, 0, NULL);  
  SetGroupSpacing(g2, 3, 0);
  pr2=StaticPrompt (g2, "Threshold-Ramp:", 0, 3*vdp->Fh/2, vdp->Fnt, 'l');
  pr1=StaticPrompt (g2, "  20%", 0, 3*vdp->Fh/2, vdp->Fnt, 'l');
  vdp->sdp.ScrollBar = ScrollBar (g2, 15, 5, DOT_ChangeMainViewerCutoff);
  pr1=StaticPrompt (g2, "100%", 0, 3*vdp->Fh/2, vdp->Fnt, 'l');
  if(mip){
    qname=mip->qname;
    sname=mip->sname;
    q_start=mip->q_start;
    s_start=mip->s_start;
    q_stop=mip->q_stop;
    s_stop=mip->s_start;
  }
  else if (alp){
    qname=alp->name1;
    sname=alp->name2;
    q_start=1;
    s_start=1;
    q_stop=alp->xlen;
    s_stop=alp->ylen;
  }
  sprintf(vdp->iInfo, "%s (x-axis)[%d..%d]  vs.   %s (y-axis)[%d..%d]", qname, q_start, q_stop, sname, s_start, s_stop); 
  vdp->Infopanel= StaticPrompt (g, vdp->iInfo, pixwidth, popupMenuHeight, vdp->Fnt, 'l');  
  SetObjectExtra(vdp->sdp.ScrollBar, vdp, NULL);
  CorrectBarMax (vdp->sdp.ScrollBar, 80); /* 100% */
  CorrectBarValue (vdp->sdp.ScrollBar, 60);/* 100% */
  /* Main Panel*/
  Xpixels = 600;
  Ypixels = 600;
  vdp->panel = AutonomousPanel (w, Xpixels, Ypixels, DOT_DisplayHits,  DOT_VscrlProc, DOT_HscrlProc , 0, NULL, NULL);
  DOT_SetUpWin (w, vdp->panel, vdp);
  DOT_InitDataStruct(vdp);
  SetPanelClick (vdp->panel, DOT_ClickProc, DOT_DragProc, NULL, DOT_ReleaseProc);

}

/*________________________________________(DOT_ResizeMainWindow)_____________

  Purpose : Resize function for main window.

____________________________________________________________________*/
static void DOT_ResizeMainWindow (WindoW w)
{
  RecT        rcP, rcW, rcHsb, rcVsb;
  DOTVibDataPtr  vdp;
  WindoW      temport;
  Int2        height, width, in , gap;
  Int4        VCurPos, HCurPos, vsbWidth, hsbHeight;
  PaneL       p;
  BaR         vsb, hsb;
  DOTSelDataPtr   data;


  vdp = (DOTVibDataPtr) GetObjectExtra (w);
  if (vdp == NULL) return;
  p = vdp->panel;
  
  if (p == NULL) return; 

  ObjectRect (w, &rcW);
  width = rcW.right - rcW.left;
  height = rcW.bottom - rcW.top;
  vsb = GetSlateVScrollBar ((SlatE) p);
  hsb = GetSlateHScrollBar ((SlatE) p);
  
  SafeHide(vdp->Infopanel);
  SafeHide(p);
  Update();

  GetPosition(vsb,&rcVsb);
  GetPosition(hsb,&rcHsb);
  GetPosition (p, &rcP);

  in=2;
  gap=10;
  vsbWidth=rcVsb.right-rcVsb.left;
  hsbHeight=rcHsb.bottom-rcHsb.top;

  
  rcP.right = width - vsbWidth - in;
  rcP.bottom = height - hsbHeight -in;

  SetPosition (p, &rcP);
  AdjustPrnt (p, &rcP, FALSE);
/*   Update(); */

  
  ObjectRect(p ,&rcP);
  temport=SavePort((WindoW)ParentWindow(p));
  Select(p);

  DOT_ComputePanelSize(rcP, vdp, &(vdp->sdp.PgWdth), &(vdp->sdp.PgLen));
  DOT_SetScrlVals (vdp);
  if (vdp->data)
    {
      data=(DOTSelDataPtr)vdp->data;
      DOT_UpdateDataRects(data, rcP, vdp, FALSE);
    }

  /*update scroll status*/
  VCurPos=GetBarValue(vsb);
  HCurPos=GetBarValue(hsb);
  DOT_VScrlUpdate(vdp, vsb, VCurPos);
  DOT_HScrlUpdate(vdp, hsb, HCurPos);
  
/*   InvalRect(&rcP); */
  RestorePort(temport);
  SafeShow(vdp->Infopanel);
  SafeShow(vdp->panel);
  Update();
  
}

/*_______________________________________________(DOT_NewAnalysis)___


  Purpose : Close old windows, begin new analysis.

____________________________________________________________________*/
static void DOT_NewAnalysis (ButtoN b)
{
  WindoW     w;
  DOTVibDataPtr vdp, vdp2;
  DOTSelDataPtr  data;
  
  w = (WindoW)ParentWindow (b);
  
  if (w!= NULL)
    {
      vdp = (DOTVibDataPtr)GetObjectExtra(w);
      data=(DOTSelDataPtr)vdp->data;
      if (data)
        MemFree(data);
      if (vdp)
        {
          if (vdp->mip)
            DOT_FreeMainInfoPtrEx(vdp->mip);
          MemFree(vdp);
        }
      Remove(w);
    }

  if (vdp->ChildWin!= NULL)
    {
      DOT_CloseSequenceWindow(b);
    }

}


/*____________________________________________(DOT_InitFont)_____________


  Purpose : Get font.

____________________________________________________________________*/

static void DOT_InitFont (DOTVibDataPtr vdp)
{
#ifdef WIN_MAC
  vdp->Fnt = ParseFont ("Monaco, 9");
#endif

#ifdef WIN_MSWIN
  vdp->Fnt = ParseFont ("Courier, 7");
#endif

#ifdef WIN_MOTIF
  vdp->Fnt = ParseFont ("fixed, 12");
#endif

	return;

}

/*____________________________________________(DOT_InitVibData)____________


  Purpose : Initialize DOTVibDataPtr for main window.

____________________________________________________________________*/

static void DOT_InitVibData(DOTVibDataPtr vdp)
{


  vdp->sdp.TrampPos=75;
  vdp->showGrid=TRUE;
  vdp->showDotPlot=TRUE;

  DOT_InitFont(vdp);
  SelectFont(vdp->Fnt);
/*   vdp->charw=MaxCharWidth(); */
/*   vdp->Fh=FontHeight(); */
  /* temporarily while being called from ingenue */
  vdp->charw = 15;
  vdp->Fh = 17;
  vdp->VERT_MARGIN = 50;
  vdp->HORZ_MARGIN = 80;
  vdp->selectMode = 1;

}
/*________________________________________(DOT_OpenSeqAnnotFile)_____________


  Purpose : Open a seqannot file with a seqalign and update the viewer.

____________________________________________________________________*/
static void DOT_OpenSeqAnnotFile(IteM i)
{
  DOTVibDataPtr vdp;
  FILE*         afile;
  Char      path [PATH_MAX];
  Pointer         dataptr=NULL;
  Uint2           datatype;
  SeqAnnotPtr     sanp=NULL;
  SeqAlignPtr     sap=NULL;
  DOTAlignInfoPtr alp;


  vdp=(DOTVibDataPtr)GetObjectExtra(ParentWindow(i));
  
  if (GetInputFileName(path, sizeof(path), "", "TEXT"))
    {
      if (path != NULL)
        {
          if (!(afile = FileOpen(path, "r"))){     
            ErrPostEx(SEV_FATAL, 0, 0, "file not found");
            return;
          } 
          dataptr = ReadAsnFastaOrFlatFile (afile, &datatype, NULL, FALSE, FALSE, TRUE, FALSE);
          if (!dataptr){     
            ErrPostEx(SEV_FATAL, 0, 0, "bad seqannot file");
              goto end;
          }
          sanp = (SeqAnnotPtr)(dataptr);
          sap = (SeqAlignPtr)(sanp->data);
          if (!sap){     
            ErrPostEx(SEV_FATAL, 0, 0, "bad seqannot file");
              goto end;
          } 
          if (sap->saip != NULL)
            AMAlignIndex2Free2(sap);
          sap->saip=NULL;
          AlnMgr2IndexLite(sap);
          AlnMgr2SortAlnSetByNthRowPos(sap, 1);

          Enable((HANDLE)vdp->displayOpts1);
          if (vdp->alp){ /* discard previous alignment*/
            vdp->alp->pict=NULL;
            DOT_ExitAlign(vdp->alp);
          }
          alp=DOT_AlignInfoNew();
          alp->sap = sap;
          if (!DOT_FillAlignInfoPointer(alp)){
            vdp->showALIGN=FALSE;
            goto end;
          }
          vdp->alp=alp;
          vdp->showALIGN=TRUE;
          SetValue(vdp->displayOpts2, 2);
        }

    end:
      DOT_UpdateMainPanel(vdp, TRUE);
      fclose(afile);
    }

  return;
}

/*________________________________________(DOT_MakeMainViewer)_____________


  Purpose : Create main window.

____________________________________________________________________*/

NLM_EXTERN Boolean DOT_MakeMainViewer (DOTMainDataPtr mip, DOTAlignInfoPtr alp)
{
  WindoW         w;
  Int2           margins;
  MenU		     m1, m2, m3, m4, s1, s2;
  IteM           i;
  Int4           n;
  ChoicE         ch;
  DOTVibDataPtr  vdp;
  SeqAlignPtr    sap=NULL;
  Char           title[60]={""};

  if (!mip && !alp) return FALSE;
  vdp=(DOTVibDataPtr)MemNew(sizeof(DOTVibData));
  if (!vdp) return FALSE;
  DOT_InitVibData(vdp);
  margins = 4*stdCharWidth;
  if (mip){
    vdp->mip=mip;
    vdp->xlen=mip->qlen;
    vdp->ylen=mip->slen;
    vdp->xstart=mip->q_start;
    vdp->ystart=mip->s_start;
    vdp->xstop=mip->q_stop;
    vdp->ystop=mip->s_stop;
    vdp->xname=mip->qname;
    vdp->yname=mip->sname;
    vdp->strand1=mip->qstrand;
    vdp->strand2=mip->sstrand;
    vdp->showDotPlot=TRUE;
  } else if (alp){
    sap=alp->sap;
    vdp->alp=alp;
    if(!mip){
      vdp->xlen=alp->xlen;
      vdp->ylen=alp->ylen;
      vdp->xstart=alp->xstart;
      vdp->ystart=alp->ystart;
      vdp->xstop=alp->xstart+alp->xlen-1;
      vdp->ystop=alp->ystart+alp->ylen-1;
      vdp->xname=alp->name1;
      vdp->yname=alp->name2;
      vdp->strand1=AlnMgr2GetNthStrand(alp->sap, 1);
      vdp->strand2=AlnMgr2GetNthStrand(alp->sap, 2);
    }
    vdp->showALIGN=TRUE;
  }
  sprintf(title, "%s", vdp->xname);
  StringCat(title, "  vs. ");
  StringCat(title, vdp->yname);
#ifdef WIN_MAC
  DOT_SetupMenus ();
#endif
  w = DocumentWindow (margins, margins, 800, 800, title, StdCloseWindowProc,   DOT_ResizeMainWindow);
#ifndef WIN_MAC
  DOT_SetupMenus (); 
#endif
  SetObjectExtra(w, (Pointer)vdp, NULL);
  m1 = PulldownMenu (w, "File"); 
/*   CommandItem (m1, "New analysis", DOT_NewAnalysis);  */
  i=CommandItem(m1, "Alignment (SeqAnnot) File...", DOT_OpenSeqAnnotFile);
  Disable(i);
  CommandItem (m1, "Close",DOT_CloseMainWindow); 
  SeparatorItem(m1);
  i = CommandItem (m1, "Exit",DOT_ExitProgram); 
#ifndef DOT_STANDALONE
  Disable(i);
#endif
  m2 = PulldownMenu (w, "Options"); 
  vdp->displayOpts1=SubMenu(m2,"Display");
  if (!sap) Disable((HANDLE)vdp->displayOpts1);
  vdp->displayOpts2=ChoiceGroup (vdp->displayOpts1, DOT_DisplayOptsProc);
  ChoiceItem (vdp->displayOpts2, "Dots ONLY");
  ChoiceItem (vdp->displayOpts2, "Dots & Aligns"); 
  ChoiceItem (vdp->displayOpts2, "Aligns ONLY"); 
  SetValue(vdp->displayOpts2,2); 
  SeparatorItem(m2);
  s1=SubMenu(m2,"Grid");
  ch=ChoiceGroup (s1, DOT_GridProc);
  ChoiceItem (ch, "show"); 
  ChoiceItem (ch, "hide");
  SeparatorItem(m2);
  s2=SubMenu(m2,"Select Mode");
  Disable(s2);
  ch=ChoiceGroup (s2, DOT_ModeProc);
  ChoiceItem (ch, "Sequence"); 
  ChoiceItem (ch, "Features");
  SetValue(ch, 1);
  SeparatorItem(m2);
  i = CommandItem (m2, "Parameters ..", DOT_ParametersProc); 
  SetObjectExtra(i, vdp, NULL);
  Disable(i);
  m3 = PulldownMenu(w, "Resize");
  i = CommandItem (m3, "Reduce", DOT_ReduceSizeProc); 
  i = CommandItem (m3, "Enlarge", DOT_EnlargeSizeProc); 
  i = CommandItem (m3, "Original", DOT_OriginalSizeProc);
  i = CommandItem (m3, "Zoom into region..", DOT_ZoomProc); 
  SetObjectExtra(i, vdp, NULL);
  m4 = PulldownMenu (w, "Analysis"); 
  i = CommandItem(m4, "Blast2Seq ..", DOT_Blast2SeqProc);
  if (!mip && alp)
    Disable(i);
  vdp->MainWin = w;
  DOT_CreateWindowDetails(w, vdp);
  RealizeWindow(w);
  Show (w);
  ProcessEvents();
  return TRUE;
}


static void DOT_SetColor(SegmenT  seg1, Int1 code, Boolean is_autopanel)
{

  if (!is_autopanel){
  if (code==dotaln_BEST)
    AddAttribute(seg1, COLOR_ATT, RED_COLOR, 0, 0, 0, 0);
  if (code==dotaln_OUTLYING_LARGE)
    AddAttribute(seg1, COLOR_ATT, BLUE_COLOR, 0, 0, 0, 0);
  if (code==dotaln_OUTLYING_SMALL)
    AddAttribute(seg1, COLOR_ATT, GREEN_COLOR, 0, 0, 0, 0);
  if (code==dotaln_GENERAL)
    AddAttribute(seg1, COLOR_ATT, BLACK_COLOR, 0, 0, 0, 0);
  }
  else{
  if (code==dotaln_BEST)
    Red();
  if (code==dotaln_OUTLYING_LARGE)
    Blue();
  if (code==dotaln_OUTLYING_SMALL)
    Green();
  if (code==dotaln_GENERAL)
    Black();
  }

}


static Boolean DOT_DeselectPrim (SegmenT seg, PrimitivE prim, Uint2 segID,Uint2 primID, Uint2 primCt, VoidPtr userdata)

{

  Int1  highlight;
  VieweR  v;

  v = (VieweR)userdata;
  GetPrimDrawAttribute (prim, NULL, NULL, NULL, NULL, NULL, &highlight);
  if (highlight != PLAIN_PRIMITIVE) 
    HighlightPrimitive (v, seg, prim, PLAIN_PRIMITIVE);

  return TRUE;
}

static DOTAlnPtr DOT_FindAlignment(DOTVibDataPtr vdp2, Uint2 primID)
{
  DOTAlnPtr aln, salp;
  DOTAlnPtr PNTR alnL;
  Int4      q_start, q_stop, s_start, s_stop;
  Int4      width, height;
  DOTSelDataPtr data;
  Int4      Left,Right,Top,Bottom,left,right,top,bottom;
  Boolean   lt_fixed=FALSE, rb_fixed=FALSE;


  if (!vdp2->alp && !vdp2->alp->Alnlist) return NULL;
  salp=(DOTAlnPtr)MemNew(sizeof(DOTAln));
  salp->show=align_plot;
  data=vdp2->data;
  alnL=vdp2->alp->Alnlist;
  aln=alnL[primID-1];
  salp->sap=aln->sap;
  salp->q_start=aln->q_start;
  salp->q_stop=aln->q_stop;
  salp->s_start=aln->s_start;
  salp->s_stop=aln->s_stop;
  salp->primID=(Uint2)primID;

  return salp;
}


/*____________________________________________(DOT_AlignPlotGivenSeqAlign)_____________


  Purpose : Run alignment display given seqalignpointer

____________________________________________________________________*/
NLM_EXTERN Boolean DOT_AlignPlotGivenSeqAlign(SeqAlignPtr sap)
{
  DOTAlignInfoPtr alp;

  alp=DOT_AlignInfoNew();
  alp->sap = sap;
  alp->entityID = sap->idx.entityID;
  if (!DOT_FillAlignInfoPointer(alp)){
    MemFree(alp);
    return FALSE;
  }

  if (!DOT_MakeMainViewer(NULL, alp))
    return FALSE;
  return TRUE;
}
/*____________________________________________(DOT_AlignPlotGivenScp)_____________


  Purpose : Run alignment display given seqalignpointer

____________________________________________________________________*/
NLM_EXTERN Boolean DOT_AlignPlotGivenScp(SCP_ResultPtr scp)
{
  DOTAlignInfoPtr alp;

  alp=DOT_AlignInfoNew();
  alp->sap=NULL;
  alp->entityID=scp->saps[0]->idx.entityID;
  alp->itemID=1;
  if (!DOT_FillFromScp(alp, scp)){
    MemFree(alp);
    return FALSE;
/*     QuitProgram(); */
  }
  if (!DOT_MakeMainViewer(NULL, alp))
    return FALSE;
  return TRUE;
}

/*_______________________________(DOT_RegDiagsDisplay)_____


  Purpose : Registered function for Dot Diag display.

_________________________________________________________*/

NLM_EXTERN Int2 LIBCALLBACK DOT_RegDiagsDisplay(Pointer data)
{
  OMProcControlPtr    ompcp = NULL;
  SeqAlignPtr         sap = NULL;
  
  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) {
    ErrPostEx (SEV_ERROR, 0, 0, "Data NULL [1]");
    return OM_MSG_RET_ERROR;
  }
  
  switch (ompcp->input_itemtype) {
  case OBJ_SEQALIGN :
    sap = (SeqAlignPtr) ompcp->input_data;
    break;
  case 0 :
    return OM_MSG_RET_ERROR;
  default :
    return OM_MSG_RET_ERROR;
  }
  
  if (sap == NULL) {
    ErrPostEx (SEV_ERROR, 0, 0, "Input Data is NULL or is not a seqalign");
    return OM_MSG_RET_ERROR;
  }

  if (!DOT_AlignPlotGivenSeqAlign(sap))
    return OM_MSG_RET_ERROR;
  else 
    return OM_MSG_RET_DONE;

}


/*_______________________________(Scoop Functions)_____


  Purpose : Run Blast 2 Seqs and sort results into high scoring, large outlying and small outlying alignments.

_________________________________________________________*/


NLM_EXTERN SCP_ResultPtr SCP_CompareOrderOrganizeBioseqs(BioseqPtr bsp1, BioseqPtr bsp2, SeqLocPtr slp1, SeqLocPtr slp2, CharPtr progname, Int4 wordsize, Int4 hitlist_size)
{
   Int4                 i;
   BLAST_OptionsBlkPtr  options;
   SeqAlignPtr          sap;
   SeqAlignPtr          sap_head;
   SeqAlignPtr          sap_prev;
   SCP_ResultPtr        scp;
   SCP_ResultPtr        scp_large;
   SCP_ResultPtr        scp_small;
   Int4                 start;
   Int4                 stop;
   SeqAnnotPtr          sanp;
   SeqEntryPtr          sep;
   Uint2                entityID=0, itemID=0;

   if (bsp1 == NULL || bsp2 == NULL)
      return NULL;
   options = BLASTOptionNew(progname, FALSE);
   options->filter_string = StringSave("m L;R");
   options->expect_value = 10;
   /* Fasika's changes */

   options->hitlist_size = hitlist_size;
   options->wordsize = wordsize;
   if (slp1 !=NULL && slp2 !=NULL){
     sap = BlastTwoSequencesByLoc(slp1, slp2, progname, options);
   }
   else {
     sap = BlastTwoSequences(bsp1, bsp2, progname, options);
   }

   sep = SeqMgrGetSeqEntryForData (bsp1);
   entityID = ObjMgrGetEntityIDForChoice(sep);
   sanp = SeqAnnotForSeqAlign (sap);
   if (sanp == NULL) return NULL;
   entityID =DOT_AttachSeqAnnotToSeqEntry(entityID, sanp, bsp1);
   SeqMgrIndexFeatures (entityID, (Pointer)bsp1);
   itemID =GatherItemIDByData(entityID, OBJ_BIOSEQ, (Pointer)bsp1);
   /* end of Fasika's changes */

   AlnMgr2IndexLite(sap);
   scp = (SCP_ResultPtr)MemNew(sizeof(SCP_Result));
   scp->len1=bsp1->length;
   scp->len2=bsp2->length;
   SCP_OrganizeAlnsInSet(sap, SCP_FUZZ, scp, 1);
   BLASTOptionDelete(options);
   fprintf(stdout, "Sequence 1 length: %d\n", bsp1->length);
   fprintf(stdout, "Sequence 2 length: %d\n", bsp2->length);
   SCP_GetNthSeqRangeInSASet(scp->saps, scp->numsaps, 1, &start, &stop);
   fprintf(stdout, "The best alignments cover from %d to %d of sequence 1\n", start, stop);
   scp_large = scp_small = NULL;
   if (scp->numlarge_outliers > 0)
   {
      sap_head = sap_prev = scp->large_outliers[0];
      for (i=1; i<scp->numlarge_outliers; i++)
      {
         sap_prev->next = scp->large_outliers[i];
         sap_prev = sap_prev->next;
      }
      sap = SeqAlignNew();
      sap->segtype = SAS_DISC;
      sap->segs = (Pointer)(sap_head);
      AlnMgr2IndexLite(sap);
      scp_large = (SCP_ResultPtr)MemNew(sizeof(SCP_Result));
      SCP_OrganizeAlnsInSet(sap, SCP_FUZZ, scp_large, 1);
      SCP_GetNthSeqRangeInSASet(scp_large->saps, scp_large->numsaps, 1, &start, &stop);
      fprintf(stdout, "Large repeats cover from %d to %d of sequence 1\n", start, stop);
   }
   if (scp->numsmall_outliers > 0)
   {
      sap_head = sap_prev = scp->small_outliers[0];
      for (i=1; i<scp->numsmall_outliers; i++)
      {
         sap_prev->next = scp->small_outliers[i];
         sap_prev = sap_prev->next;
      }
      sap = SeqAlignNew();
      sap->segtype = SAS_DISC;
      sap->segs = (Pointer)(sap_head);
      AlnMgr2IndexLite(sap);
      scp_small = (SCP_ResultPtr)MemNew(sizeof(SCP_Result));
      SCP_OrganizeAlnsInSet(sap, SCP_FUZZ, scp_small, 1);
      SCP_GetNthSeqRangeInSASet(scp_small->saps, scp_small->numsaps, 1, &start, &stop);
      fprintf(stdout, "Small repeats cover from %d to %d of sequence 1\n", start, stop);
   }
   return scp;
}

extern void SCP_OrganizeAlnsInSet(SeqAlignPtr sap, Int4 fuzz, SCP_ResultPtr scp, Int4 n)
{
   AMAlignIndex2Ptr  amaip;
   Boolean          conflict;
   Int4             curr;
   Int4             i;
   Int4             indextype;
   SeqAlignPtr      large=NULL;
   SeqAlignPtr      large_outliers=NULL;
   SeqAlignPtr      keep=NULL;
   SeqAlignPtr      keepers=NULL;
   SeqAlignPtr      salp=NULL;
   SeqAlignPtr      salp_head=NULL;
   SeqAlignPtr      salp_prev=NULL;
   SeqAlignPtr      small=NULL;
   SeqAlignPtr      small_outliers=NULL;
   SCP_nPtr         PNTR spin=NULL;
   Int4             start;
   Int4             stop;
   Int4             strand;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return;
   if (n > 2)
      return;
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   indextype = amaip->alnstyle;
   /* make sure that everything is on the plus strand of the nth sequence */
   for (i=0; i<amaip->numsaps; i++)
   {
      salp = amaip->saps[i];
      AssignIDsInEntity(1, OBJ_SEQALIGN, (Pointer)salp);
      strand = AlnMgr2GetNthStrand(salp, n);
      if (strand == Seq_strand_minus)
      {
         SAIndex2Free2(salp->saip);
         salp->saip = NULL;
         salp->next = NULL;
         SeqAlignListReverseStrand(salp);
         AlnMgr2IndexSingleChildSeqAlign(salp);
      }
   }
   /* spin structure: n1 = which alignment, n2 = start on first row, n3 =
      alignment length on 1st row, n4 = start on 2nd row, n5 = 2nd strand */
   spin = (SCP_nPtr PNTR)MemNew((amaip->numsaps)*sizeof(SCP_nPtr));
   for (i=0; i<amaip->numsaps; i++)
   {
      spin[i] = (SCP_nPtr)MemNew(sizeof(SCP_n));
      salp = amaip->saps[i];
      spin[i]->n1 = i;
      AlnMgr2GetNthSeqRangeInSA(salp, n, &start, &stop);
      spin[i]->n3 = stop - start;
      spin[i]->n2 = start;
      AlnMgr2GetNthSeqRangeInSA(salp, 3-n, &start, &stop);
      spin[i]->n4 = start;
      strand = AlnMgr2GetNthStrand(salp, 3-n);
      if (strand == Seq_strand_minus)
         spin[i]->n5 = -1;
      else
         spin[i]->n5 = 1;
   }
   HeapSort((Pointer)spin, (size_t)(amaip->numsaps), sizeof(SCP_nPtr), SCP_CompareSpins);
   strand = spin[0]->n5;
   for (i=1; i<amaip->numsaps; i++)
   {
      if (spin[i]->n5 != strand)
      {
         salp = amaip->saps[spin[i]->n1];
         salp->next = NULL;
         SeqAlignFree(salp);
         amaip->saps[spin[i]->n1] = NULL;
         spin[i]->n1 = -1;
      }
   }
   large_outliers = small_outliers = keepers = NULL;
   scp->numsaps++;
   salp = amaip->saps[spin[0]->n1];
   salp->next = NULL;
   keepers = keep = salp;
   for (curr=0; curr<amaip->numsaps; curr++)
   {
      if (spin[curr]->n1 != -1)
      {
         for (i=curr+1; i<amaip->numsaps; i++)
         {
            if (spin[i]->n1 != -1)
            {
               conflict = FALSE;
            /* check first for conflict on first row */
               if (spin[i]->n2 + spin[i]->n3 - 1 > spin[curr]->n2 + fuzz)
               {
                  if (spin[i]->n2 < spin[curr]->n2)
                     conflict = TRUE;
               }
               if (spin[i]->n2 < spin[curr]->n2 + spin[curr]->n3 - 1 - fuzz)
               {
                  if (spin[i]->n2 + spin[i]->n3 - 1 > spin[curr]->n2 + spin[curr]->n3 - 1)
                     conflict = TRUE;
               }
               if (spin[i]->n2 >= spin[curr]->n2)
               {
                  if (spin[i]->n2 + spin[i]->n3 - 1 <= spin[curr]->n2 + spin[curr]->n3 - 1)
                     conflict = TRUE;
               }
            /* then check for conflict and consistency on second row */
               if (spin[i]->n2 + spin[i]->n3 - 1 < spin[curr]->n2 + fuzz)
               {
                  if (strand == 1)
                  {
                     if (spin[i]->n4 + spin[i]->n3 - 1 > spin[curr]->n4 + fuzz)
                        conflict = TRUE;
                  } else if (strand == -1)
                  {
                     if (spin[curr]->n4 + spin[curr]->n3 - 1 - fuzz > spin[i]->n4)
                        conflict = TRUE;
                  }
               } else
               {
                  if (strand == 1)
                  {
                     if (spin[i]->n4 < spin[curr]->n4 + spin[curr]->n3 - fuzz)
                        conflict = TRUE;
                  } else if (strand == -1)
                  {
                     if (spin[i]->n4 + spin[i]->n3 - 1 - fuzz > spin[curr]->n4)
                        conflict = TRUE;
                  }
               }
               if (conflict)
               {
                  salp = amaip->saps[spin[i]->n1];
                  salp->next = NULL;
                  if (spin[i]->n3 >= SCP_LARGE)
                  {
                     scp->numlarge_outliers++;
                     if (large_outliers != NULL)
                     {
                        large->next = salp;
                        large = salp;
                     } else
                        large_outliers = large = salp;
                  } else
                  {
                     scp->numsmall_outliers++;
                     if (small_outliers != NULL)
                     {
                        small->next = salp;
                        small = salp;
                     } else
                        small_outliers = small = salp;
                  }
                  amaip->saps[spin[i]->n1] = NULL;
                  spin[i]->n1 = -1;
               }
            }
         }
      }
   }
   for (i=1; i<amaip->numsaps; i++)
   {
      if (spin[i]->n1 != -1)
      {
         scp->numsaps++;
         salp = amaip->saps[spin[i]->n1];
         salp->next = NULL;
         if (keepers == NULL)
            keepers = keep = salp;
         else
         {
            keep->next = salp;
            keep = salp;
         }
      }
   }
   scp->small_outliers = (SeqAlignPtr PNTR)MemNew((scp->numsmall_outliers)*sizeof(SeqAlignPtr));
   small = small_outliers;
   i = 0;
   while (small != NULL)
   {
      scp->small_outliers[i] = small;
      i++;
      small = small->next;
   }
   scp->large_outliers = (SeqAlignPtr PNTR)MemNew((scp->numlarge_outliers)*sizeof(SeqAlignPtr));
   large = large_outliers;
   i = 0;
   while (large != NULL)
   {
      scp->large_outliers[i] = large;
      i++;
      large = large->next;
   }
   scp->saps = (SeqAlignPtr PNTR)MemNew((scp->numsaps)*sizeof(SeqAlignPtr));
   keep = keepers;
   i = 0;
   while (keep != NULL)
   {
      scp->saps[i] = keep;
      i++;
      keep = keep->next;
   }

   salp_head = salp_prev = NULL;
   for (i=0; i<amaip->numsaps; i++)
   {
      MemFree(spin[i]);
      if (amaip->saps[i] != NULL)
      {
         amaip->saps[i]->next = NULL;
         if (salp_prev != NULL)
         {
            salp_prev->next = amaip->saps[i];
            salp_prev = salp_prev->next;
         } else
            salp_head = salp_prev = amaip->saps[i];
      }
   }
   sap->segs = (Pointer)(salp_head);
   AMAlignIndexFree(sap->saip);
   sap->saip = NULL;
   AlnMgrIndexLite(sap);
   MemFree(spin);
}

static int LIBCALLBACK SCP_CompareSpins(VoidPtr ptr1, VoidPtr ptr2)
{
   SCP_nPtr  spin1;
   SCP_nPtr  spin2;

   spin1 = *((SCP_nPtr PNTR) ptr1);
   spin2 = *((SCP_nPtr PNTR) ptr2);
   if (spin1 == NULL || spin2 == NULL)
      return 0;
   if (spin1->n3 > spin2->n3)
      return -1;
   if (spin1->n3 < spin2->n3)
      return 1;
   if (spin1->n2 < spin2->n2)
      return -1;
   if (spin1->n2 > spin2->n2)
      return 1;
   return 0;
}

static void SCP_GetNthSeqRangeInSASet(SeqAlignPtr PNTR saparray, Int4 numsaps, Int4 n, Int4Ptr start, Int4Ptr stop)
{
   Int4         i;
   SeqAlignPtr  salp;
   Int4         start_tmp;
   Int4         stop_tmp;
   Int4         tmp1;
   Int4         tmp2;
  
   start_tmp = stop_tmp = -1;
   for (i=0; i<numsaps; i++)
   {
      salp = saparray[i];
      if (n > salp->dim)
      {
         if (start)
            *start = -1;
         if (stop)
            *stop = -1;
         return;
      }
      AlnMgrGetNthSeqRangeInSA(salp, n, &tmp1, &tmp2);
      if (tmp1 < start_tmp || start_tmp == -1)
         start_tmp = tmp1;
      if (tmp2 > stop_tmp)
         stop_tmp = tmp2;
   }
   if (start)
      *start = start_tmp;
   if (stop)
      *stop = stop_tmp;
}
