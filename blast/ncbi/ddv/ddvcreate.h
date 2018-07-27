/*  $Id: ddvcreate.h,v 1.31 2001/01/29 20:35:49 hurwitz Exp $
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
* File Name:  ddvcreate.h
*
* Author:  Sarah Wheelan
*
* Version Creation Date:   08/99
*
* $Revision: 1.31 $
*
* File Description: 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ddvcreate.h,v $
* Revision 1.31  2001/01/29 20:35:49  hurwitz
* make gap between aligned blocks optional
*
* Revision 1.30  2001/01/25 21:49:14  hurwitz
* added option for extra gap char (tilde) between aligned blocks
*
* Revision 1.29  2000/09/08 21:50:39  hurwitz
* made DDV_ReadSeqBin public
*
* Revision 1.28  2000/05/04 22:43:38  hurwitz
* don't launch DDE on top of DDV, change some wording, redraw DDE after save to AlnMgr
*
* Revision 1.27  2000/04/21 23:00:50  hurwitz
* can launch DDE from DDV
*
* Revision 1.26  2000/04/19 15:45:50  hurwitz
* can create display for a block
*
* Revision 1.25  2000/03/31 21:33:07  durand
* added new default color schemas for BLAST
*
* Revision 1.24  2000/03/28 13:33:27  durand
* moved DDV_Disp_Opt data structure to pgppop.h
*
* Revision 1.23  2000/03/21 19:26:45  durand
* adapt ddvcreate to be used by pgppop
*
* Revision 1.22  2000/03/20 13:46:23  durand
* fixed a bug in BLAST functions; added DDV_BLAST_COLOR0 to produce a BLAST output without any layout
*
* Revision 1.21  2000/02/28 16:52:47  durand
* wrote a new manager for unaligned regions
*
* Revision 1.20  2000/02/23 21:34:29  durand
* allow custom display of gap characters
*
* Revision 1.19  2000/02/23 19:49:33  durand
* use row number instead of SeqId for coloring
*
* Revision 1.18  2000/02/14 16:49:34  durand
* add new options for BLAST output
*
* Revision 1.17  2000/02/02 14:44:31  durand
* added function to create data structure for block editor, fixed some bugs
*
* Revision 1.16  2000/01/26 13:38:55  durand
* update the GUI for the editor. Add functions to create the data to be used by the editor
*
* Revision 1.15  1999/12/20 20:20:41  lewisg
* allow cn3d to do color and ddv to do case when both are running
*
* Revision 1.14  1999/12/20 14:45:37  durand
* add new functions for the new BLAST outputs
*
* Revision 1.13  1999/12/08 22:42:17  durand
* add the code to produce colored BLAST outputs
*
* Revision 1.12  1999/12/03 23:17:22  lewisg
* Patrick's new global update msg, argument passing when launching ddv, experimental editing
*
* Revision 1.11  1999/12/03 15:11:49  durand
* add DDV_AllGapInLowerCase declaration
*
* Revision 1.10  1999/11/24 21:26:30  vakatov
* Fixed for the C++ and/or MSVC DLL compilation
*
* Revision 1.9  1999/11/17 22:43:57  durand
* speed up the selection manager for large SeqAlign
*
* Revision 1.8  1999/11/09 17:08:59  durand
* transfer some functions from ddvgraph to ddvcreate, so that ddvcreate remains Vibrant free and can be compiled with BLAST
*
* Revision 1.7  1999/11/03 21:29:48  durand
* add CTRL and SHFT keys for mouse selection. redesign the loader functions of DDV to properly register the message callbacks
*
* Revision 1.6  1999/10/29 14:15:40  durand
* add simple mouse selection functions
*
* Revision 1.5  1999/10/22 20:12:47  durand
* add Export command (text, HTML and Phylip formats)
*
* Revision 1.4  1999/10/20 13:17:19  durand
* add display for disc. SeqAlign tails
*
* Revision 1.3  1999/10/15 21:57:36  durand
* add a UI for display options
*
* Revision 1.2  1999/10/12 15:01:29  lewisg
* resolve confict with internal/ddv
*
* Revision 1.1  1999/09/30 14:10:26  durand
* add ddv to toolkit
*
* Revision 1.5  1999/09/30 13:38:09  durand
* DDV_CreateDisplayFromIndex takes ParaG_Size as an argument
*
*
*
* ==========================================================================
*/
#ifndef _DDVCREATE_
#define _DDVCREATE_

#include <ncbi.h>
#include <alignmgr.h>
#include <udvseq.h>
#include <pgppop.h>
#include <udvdef.h>
#include <ddvcreate.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************

  structure

*******************************************************************************/
typedef struct uabitdescr{
	Int4    from;
	Int4    to;
	Boolean IsGap;
} UABitDescr, PNTR UABitDescrPtr;

typedef struct uamsg{ /*UnAligned Messaging structure*/
	Int4    from_a; /*asked coord.*/
	Int4    to_a;
	Int4    from_r; /*returned coord.*/
	Int4    to_r;
	Int4    currentPos;
	Uint1   DispType;
	Uint1   strand;
	Boolean IsGap;
	UABitDescr ubd[3];
} UAMsg, PNTR UAMsgPtr;

typedef struct descridisp {
        Int4            from;
        Int4            to;
        Int4            UAnum;
        Int4            UAMaxlength;
        Boolean         IsGap;
        Uint1           TextStyle;
        Uint1           strand;
} DescriDisp, PNTR DescriDispPtr;

/*******************************************************************************

  defines

*******************************************************************************/
#define RulerStyle_One_Start      ((Uint1)1)
#define RulerStyle_Continue_Start ((Uint1)2)

/*size limit of a seqalign for edition (number of columns)*/
#define LIMIT_EDITOR_SIZE 10000

#define DDV_BLAST_COLOR0 0
#define DDV_BLAST_COLOR1 1
#define DDV_BLAST_COLOR2 2
#define DDV_BLAST_COLOR3 3
#define DDV_BLAST_COLOR4 4
#define DDV_BLAST_COLOR5 5
#define DDV_BLAST_COLOR6 6

/*******************************************************************************

  functions

*******************************************************************************/
NLM_EXTERN Boolean DDV_CreateDisplayFromIndex(SeqAlignPtr sap, MsaParaGPopListPtr mpplp, 
		Int2 LineSize, DDV_Disp_OptPtr ddop);
NLM_EXTERN Boolean DDV_CreateDisplayFromIndex2(SeqAlignPtr sap, MsaParaGPopListPtr mpplp, 
		Int2 LineSize, DDV_Disp_OptPtr ddop, Boolean WantExtraGapChar);

NLM_EXTERN Boolean DDV_CreateDisplayFromIndex_EX(SeqAlignPtr sap, MsaParaGPopListPtr mpplp, 
		Int2 LineSize, DDV_Disp_OptPtr ddop,Int4 start, Int4 stop);
NLM_EXTERN Boolean DDV_CreateDisplayFromIndex_EX2(SeqAlignPtr sap, MsaParaGPopListPtr mpplp, 
		Int2 LineSize, DDV_Disp_OptPtr ddop,Int4 start, Int4 stop, Boolean WantExtraGapChar);

NLM_EXTERN Boolean DDV_CreateDisplay_DiscAlign(SeqAlignPtr sap, 
		MsaParaGPopListPtr mpplp, Int2 LineSize,DDV_Disp_OptPtr ddop);
NLM_EXTERN Boolean DDV_CreateDisplay_DiscAlign2(SeqAlignPtr sap, 
		MsaParaGPopListPtr mpplp, Int2 LineSize,DDV_Disp_OptPtr ddop, Boolean WantExtraGapChar);

NLM_EXTERN Boolean DDV_BuildDisp_BlockEditor(SeqAlignPtr sap, 
		MsaParaGPopListPtr mpplp, Int4 block_num);
NLM_EXTERN UAMsgPtr UAMgrIntUAMsg( Int4 MaxLength, Int4 BspLength,
		Int4 BspStart,Int4 BspStop, Int4 ddpFrom, Int4 ddpTo,Uint1 DispType, Uint1 strand);
NLM_EXTERN UAMsgPtr UAMgrDelUAMsg(UAMsgPtr PNTR uamp);
NLM_EXTERN void UAMgrUAMsgGetInfo(UAMsgPtr uamp,Int4Ptr from_bsp, Int4Ptr to_bsp, 
		BoolPtr IsGap);
NLM_EXTERN Boolean UAMgrGetNextUAbit(UAMsgPtr uamp,Int4 BspStart,Int4 BspStop);

NLM_EXTERN ValNodePtr UABuildDescriptor(SeqAlignPtr sap, Int4 nBsp, Int2 LineSize,
	DDV_Disp_OptPtr ddop, Int4Ptr TotLength,Boolean AddLeftUAPart,
	Boolean AddRightUAPart);
NLM_EXTERN ValNodePtr UABuildDescriptor2(SeqAlignPtr sap, Int4 nBsp, Int2 LineSize,
	DDV_Disp_OptPtr ddop, Int4Ptr TotLength,Boolean AddLeftUAPart,
	Boolean AddRightUAPart, Boolean WantExtraGapChar);

NLM_EXTERN ValNodePtr UABuildDescriptorForBlockEditor(SeqAlignPtr sap, Int4 nBsp, 
	Int4 block_num,Int4 LineSize,Int4Ptr TotLength);
NLM_EXTERN ValNodePtr DDV_BuildTailDescriptor(SeqAlignPtr sap, Int4 nBsp, Int2 LineSize,
	Uint1 which_tail,Int4Ptr TotLength,Int4Ptr cumulpop);
NLM_EXTERN void DDV_InitDefSAPdispStyles(DDV_Disp_OptPtr ddop);
NLM_EXTERN void DDV_InitCn3DSAPdispStyles(DDV_Disp_OptPtr ddop);
NLM_EXTERN void DDV_InitDDESAPdispStyles(DDV_Disp_OptPtr ddop);
NLM_EXTERN Uint4Ptr DDV_BuildBspEntitiesTbl(ValNodePtr PNTR TableHead,Int4 nBsp);
NLM_EXTERN Uint1Ptr DDV_ReadSeqBin (SeqIdPtr sip, Int4 from, Int4 to, 
                                    Boolean IsProt,Int2 len,Uint1 strand);

/*export functions*/
NLM_EXTERN void DDV_DumpSAPInAFile(MsaParaGPopListPtr mpplp,DDVOptionsBlockPtr dobp, 
		FILE *hFile,Uint4 option, DDV_ColorGlobal *gclr);
NLM_EXTERN void DDV_DumpSAPInFastaFile(MsaParaGPopListPtr mpplp,DDVOptionsBlockPtr dobp, 
		FILE *hFile,Boolean bPrintGap);

/*BLAST stuffs; function to build new outputs*/
NLM_EXTERN Boolean DDV_DisplayBlastPairList(SeqAlignPtr sap, ValNodePtr mask,
	FILE *fp, Boolean is_na, Uint4 tx_option,Uint1 ColorScheme);

/*layout manager stuff*/
NLM_EXTERN Boolean DDV_InitColour_When_Start(SeqAlignPtr sap,MsaParaGPopListPtr mpplp,
	DDV_ColorGlobal * * gclr, Boolean NoColor);
NLM_EXTERN Boolean DDV_LayoutUAregion(SeqAlignPtr sap,MsaParaGPopListPtr mpplp,
	DDV_ColorGlobal * * gclr,Uint1 * pRGB,Uint1 style,Uint1 * pUARGB,
	Uint1 UAstyle);
NLM_EXTERN void DDV_LayoutGap(DDV_ColorGlobal *pcg,ValNodePtr vnp_seq1,
		ValNodePtr vnp_seq2,Int4 row2,Uint1 style,Uint1 *rgb);
NLM_EXTERN void DDV_LayoutMaskRegions(DDV_ColorGlobal *pcg,
	ValNodePtr vnp_query,ValNodePtr mask,Int4 row,Uint1 style,Uint1 *rgb);
NLM_EXTERN void DDV_LayoutISOColors(DDV_ColorGlobal *pcg,ValNodePtr * row_list,Int4 nRow,
	Int4 Master,Boolean bSetMaster,Int4Ptr * matrix,Uint1 IdentClr,Uint1 SimilClr,
	Uint1 OtherClr,Uint1 IdentSty,Uint1 SimilSty,Uint1 OtherSty);

/* create display for a single aligned block or an unaligned region */
NLM_EXTERN MsaParaGPopListPtr DDE_CreateDisplayForBlock(SeqAlignPtr sap, Int4 BlockIndex);
NLM_EXTERN MsaParaGPopListPtr DDE_CreateDisplayForUnAligned(SeqAlignPtr sap, Int4 UAIndex);
NLM_EXTERN MsaParaGPopListPtr DDE_CreateDisplay(SeqAlignPtr sap, Int4 BlockIndex,
                                                Boolean IsUnAligned, Int4* pNumBlocks);

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* ndef _DDVCREATE_ */

