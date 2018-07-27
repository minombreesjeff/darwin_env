/*   salparam.h
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
* File Name:  salparam.h
*
* Author:  Colombe Chappey
*
* Version Creation Date:   1/27/96
*
* $Revision: 6.7 $
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

#ifndef _SALPARAM_
#define _SALPARAM_

#include <salsa.h>
#include <vibrant.h>

#ifdef __cplusplus
extern "C" {
#endif

/*********************************
***    SeqFontProc
***********************************/
extern void SeqFontProc (IteM i);

/*********************************
***    SelectColorProc
***********************************/
extern void setUpLetterColor ( Uint4 *colorRefs, Uint1 letter,
                               Uint1 red, Uint1 green, Uint1 blue );
/************************************
***   Translation
*************************************/
extern void rf1ItemProc (IteM i);
extern void rf2ItemProc (IteM i);
extern void rf3ItemProc (IteM i);
extern void rf4ItemProc (IteM i);
extern void rf5ItemProc (IteM i);
extern void rf6ItemProc (IteM i);
extern void rf7ItemProc (IteM i);
extern void rf8ItemProc (IteM i);
extern void rf9ItemProc (IteM i);
extern void rf10ItemProc (IteM i);

/************************************
***    Dialog Boxes
*************************************/

extern void FileInProc (ButtoN b);
extern void DefinePanelDialog (IteM i);
/*
typedef struct simpamdlg{
        Boolean ok;
}SimPamDlg, PNTR SimPamDlgPtr;

extern Boolean Sim2PamDialog (SimPamPtr spp);
*/
/**********************************************
***    
***   AlignDataSet_Restore
***     Inval Rect
***     RestorePort
***    
**********************************************/
extern void inval_panel (PaneL pnl, Int2 x, Int2 y);
extern void inval_window (WindoW w);
extern void inval_rect (Int2 left, Int2 top, Int2 right, Int2 bottom);
extern void inval_allines (EditAlignDataPtr adp, RecT *rp);
extern void inval_selstruct (EditAlignDataPtr adp, Uint2 ei, Uint2 ii, Uint2 it, Uint2 itemsubtype, RecT *rp, Int2 left, Int2 toright);
extern void inval_selstructpos (EditAlignDataPtr adp, Uint2 ei, Uint2 ii, Uint2 it, RecT *rp, Int4 pos);
extern void inval_selstructpos_tobottom (EditAlignDataPtr adp, Uint2 ei, Uint2 ii, Uint2 it, RecT *rp, Int4 pos);
extern void inval_selstructloc (EditAlignDataPtr adp, Uint2 ei, Uint2 ii, Uint2 it, Uint2 itemsubtype, RecT *rp, Int4 from, Int4 to);

extern void inval_selstructloc_forfeat (EditAlignDataPtr adp, Uint2 ei, Uint2 ii, Uint2 it, Uint2 itemsubtype, RecT *rp, Int4 from, Int4 to);

extern void inval_rectidselected (EditAlignDataPtr adp, RecT *rp);
extern void inval_pep (EditAlignDataPtr adp, RecT *rp);
extern void inval_all (EditAlignDataPtr adp, RecT *rp, Uint2 itemtype1, Uint2 itemtype2, Uint2 itemtype3, Int2 width);

extern void FormulaDialog (IteM i);

extern void FindPatternDialog (IteM i);

extern void DotPlotDialog (IteM i);
extern void DotPlotItem (IteM i);

extern void ExportTextFunc (ButtoN b);
extern void SalsaExportDialog (IteM i);

extern void SelectSeqDialog (IteM i);

extern void SelectRegionDialog (IteM i);

extern void ColorIdentityDialog (WindoW w);
extern void ColorIdentityDialogItem (IteM i);

NLM_EXTERN void PropagateFeatDialog (IteM i);

#ifdef __cplusplus
}
#endif

#endif
