/*   salpanel.h
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
* File Name:  salpanel.h
*
* Author:  Colombe Chappey
*
* Version Creation Date:   1/27/96
*
* $Revision: 6.4 $
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

#ifndef _SALPANEL_
#define _SALPANEL_

#include <salsa.h>
#include <seqport.h>
#include <vibrant.h>
#include <txalign.h>

extern WindoW getwindow_frompanel (PaneL pnl);
extern PaneL GetPanelFromWindow (WindoW w);
extern EditAlignDataPtr GetAlignEditData (WindoW w);
extern EditAlignDataPtr GetAlignDataPanel (PaneL pnl);

/********************************************************
***    
***  SetupScrollBar   
***    CorrectBarMax_v: nr of sequences NOT shown
***    CorrectBarMax_h: length of alignment NOT shown
***    CorrectBarPage_v: v_Page - 1 
***    CorrectBarPage_h: visibleWidth - 1 
***    
*********************************************************/
extern BaR  SeqEdGetSlateScrollBar (PaneL pnl);
extern Int4 SeqEdGetValueScrollBar (PaneL pnl);
extern void SeqEdSetValueScrollBar (PaneL pnl, Int4 value);
extern void SeqEdCorrectBarPage (PaneL pnl, Int4 page1, Int4 page2);
extern void SeqEdCorrectBarValue (PaneL pnl, Int4 value);
extern void SeqEdCorrectBarMax (PaneL pnl, Int4 value);
extern void SeqEdSetCorrectBarMax (PaneL pnl, EditAlignDataPtr adp, float hratio);
extern void VscrlProc (BaR sb, SlatE s, Int4 newval, Int4 oldval);
extern void HscrlProc (BaR sb, SlatE s, Int4 newval, Int4 oldval);

extern Int4 hoffset2voffset (EditAlignDataPtr adp, ValNodePtr anp_list, Int4 line_len, Int4 left, Int4 right, Int4 hoffset);

/************************************************************
***  ResizeAlignDataWindow: Call Back for DocumentWindow
*************************************************************/
extern void do_resize_panel (PaneL pnl, EditAlignDataPtr adp, Int4 width, Int4 height, Boolean rearrange);
extern void do_resize_window (PaneL pnl, EditAlignDataPtr adp, Boolean rearrange);
/*****************************************************************
***
***     Draw Alignment in Panel p 
***
******************************************************************/
extern void on_draw (PaneL p);
#endif
