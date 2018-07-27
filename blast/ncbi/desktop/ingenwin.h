/* ingenwin.h
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
* File Name:  ingenwin.h
*
* Author:  Fasika Aklilu
*
* Version Creation Date:   8/9/01
*
* $Revision: 6.4 $
*
* File Description: handles windowing for ingenue
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#ifndef _INGENWIN_
#define _INGENWIN_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN_GIF
#include <gifgen.h>
#include <ncbigif.h>
#include <ncbifile.h>
#endif

#include <asn2ff.h>
#include <tfuns.h>
#include <actutils.h>
#include <accentr.h>
#include <netblap3.h>
#include <objmgr.h> 
#include <import.h>
#include <cdrgn.h>
#include <saledit.h> 
#include <seqsub.h> 
#include <drawingp.h> 
#include <seg.h> 
#include <dust.h>
#include <udviewer.h> 
#include <ingengraph.h>
#include <motif.h>

/******************************************************
 *
 *  defines for the Genome Viewer of UDV
 *
******************************************************/
#define RULERDEFHEIGHT 7 /*default height of Ruler * cyChar */
#define Ing_MaxScale  5000 /* max zoomable scale */

#define numcols_genscan 5
#define numcols_tbdl 8
#define L_T      256
#define L_ID     32
#define L_DEF    220

#define MYARGASNFILE      0
#define MYARGFASTAFILE    1
#define MYARGGENSCANFILE  2
#define MYARGBLASTFILE    3
#define MYARGSPIDFILE     4
#define MYARGTBDLFILE   5
#define MYARGBEGIN      6
#define MYARGEND        7
#define REG_INGENUE    ObjMgrProcLoad(OMPROC_EDIT, "Ing_StandAlone", "Ing_StandAlone",OBJ_BIOSEQ,0,OBJ_BIOSEQ,0, NULL, Ing_RegIngenueProc, PROC_PRIORITY_DEFAULT)
#define REGISTER_INGENUE    ObjMgrProcLoad(OMPROC_EDIT, "Ingenue_StandAlone", "Ingenue Editor",OBJ_BIOSEQ,0,OBJ_BIOSEQ,0, NULL, Ing_RegisterIngenueProc, PROC_PRIORITY_DEFAULT)


/******************************************************
 *
 *  data structures for the Genome Viewer 
 *
******************************************************/


  typedef struct gv_colorcell{
    Uint1 rgb[3];
  } Ing_ColorCell;

typedef struct gvclickdata{/*used to select analysis in Line Viewer*/
   VieweR       viewer;
	Uint2        segID;
   Boolean      found;
} IngClickData;


typedef struct ingmsginfo{
  Uint2       segID;
  Uint1       labelmsg;
  Boolean     bSegmented;
} IngMsgInfo, PNTR IngMsgInfoPtr;

  typedef struct line_range{
    TexT     left;
    TexT     right;
    ButtoN   showall;
  }IngRange, PNTR IngRangePtr;


  typedef struct line_comp{
    ButtoN   GC;
    ButtoN   cmpl;
    TexT     window;
    TexT     low;
    TexT     high;
  }IngComp, PNTR IngCompPtr;

  typedef struct ing_snt{
    SeqIdPtr   sip;
    BioseqPtr  parent;
    SeqLocPtr  slp;
    Uint1Ptr   PNTR  pClr;
    Int4       left;
    Int4       right;
    SegmenT    seg;
    BoxInfo    box;
    Int4       scaleX;
  } IngSnt, PNTR IngSntPtr;

  typedef struct line_save{
    TexT     text;
  }IngSave, PNTR IngSavePtr;

typedef struct line_info {
   CharPtr  fields[8];
   struct line_info PNTR next;
} IngInfo, PNTR IngInfoPtr;

  typedef struct ig_fileopendata{
    GrouP   g_File;
    ButtoN  OK;
    TexT    PathName;
    PopuP   pops;
    GrouP   g_GI;
    ButtoN  E_OK;
    TexT    GI;
    WindoW  hStart;
  } IngFileOpenData, PNTR IngFileOpenDataPtr;


typedef struct ing_report{
  WindoW   hMain;
  VieweR   Viewer;
  SegmenT  Picture;
  IngGenomeViewerPtr  igvp;
  Boolean  isPrevSelected;
  Boolean  bSummary;
  Int4Ptr  vert_positions;
  Uint2    last_segID;
} IngReport, PNTR IngReportPtr;


  /* modified from NewObjectData - sequin.h*/
typedef struct gv_objectdata {
  Int2             kind;   /* 1 = feature creation, 2 = analysis */
  ObjMgrProcPtr    ompp;
  IngGenomeViewerPtr igvp;
  IteM             item;
  Uint1            molgroup;
  struct gv_objectdata PNTR next;
} IngObjectData, PNTR IngObjectPtr;

  typedef struct ig_netopen {
    WindoW  hMain;
    TexT    Entry;
    ButtoN  ok;
    IngGenomeViewerPtr  igvp;
  } IngNetOpen, PNTR IngNetOpenPtr;


/******************************************************
 *
 *  function declarations
 *
******************************************************/
NLM_EXTERN Int2 LIBCALLBACK Ing_RegIngenueProc (Pointer data);
NLM_EXTERN Int2 LIBCALLBACK Ing_RegisterIngenueProc (Pointer data);
extern void Ing_FileOpenProc(IngGenomeViewerPtr igvp, WindoW w);
extern void Ing_ReadmRNAs(FILE *fp, IngGenomeViewerPtr igvp, CharPtr GI_list);
extern void Ing_CreateDotMatrixForm(IngGenomeViewerPtr igvp, SeqAlignPtr sap, CharPtr path, CharPtr GI_list);
extern void Ing_CreateSpideyForm(IngGenomeViewerPtr igvp, BioseqPtr bsp1, CharPtr path, CharPtr GI_list);
extern void Ing_ReportWindow(IteM i);
extern void Ing_DisplayReport(IngReportPtr Report, IngGenomeViewerPtr igvp);
extern void Ing_UpdateType1(IngGenomeViewerPtr igvp, Boolean update_lines, Boolean update_control, Boolean update_range);

extern void Ing_RegBlastProc(IteM i);
extern void Ing_Blast2SeqsProc(IteM i);
extern void Ing_QBlastProc(IteM i);
  extern void IngenueCheckSocketsProc (WindoW w);
  extern BioseqPtr Ing_GetBspFromGIOrAcc(CharPtr str);
extern void Ing_GetFileForSpidey(IteM i);
extern void Ing_DotDiagProc(IteM i);
extern void Ing_OpenUDV(IteM i);
extern Int4 Ing_AccessionToGi (CharPtr string);
extern Int4 Ing_GetValue (TexT t);
extern void Ing_CreateBlast2SeqsForm(IngGenomeViewerPtr igvp, BioseqPtr bsp1, CharPtr path, CharPtr GI_list);
extern Boolean Ing_AttachMessageProc(IngGenomeViewerPtr igvp, Uint2 procID, Uint2 eID);
extern void Ing_OpenFromFileORNetwork(IngGenomeViewerPtr igvp);
extern Uint2 Ing_AttachSeqAnnotToSeqEntry (Uint2 entityID, SeqAnnotPtr sap, BioseqPtr bsp);
extern void Ing_GetFromNetwork(IngGenomeViewerPtr igvp, WindoW hStart);
extern Boolean Ing_DeletePrims(SegmenT seg, PrimitivE prim, Uint2 segID, Uint2 primID, Uint2 primCt, VoidPtr userdata);
extern void Ing_NetOpenFromCommandline(CharPtr txt, Int4 start, Int4 stop, Int4 spi_job, CharPtr spi_filename, CharPtr spi_gilist, CharPtr spi_seqidfile, Int4 b2s_job, CharPtr b2s_filename, CharPtr b2s_gilist, CharPtr b2s_seqidfile);
extern void Ing_FileOpenFromCommandline(CharPtr txt, Int4 start, Int4 stop, Int4 spi_job, CharPtr spi_filename, CharPtr spi_gilist,  CharPtr spi_seqidfile, Int4 b2s_job, CharPtr b2s_filename, CharPtr b2s_gilist, CharPtr b2s_seqidfile);
  extern void Ing_SearchAli(SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent);
extern void Ing_HighlightReportWindow(IngReportPtr Report, Int1 highlight, Uint2 segID, Boolean rePopulate);
extern void Ing_PopulateReport(IngReportPtr Report, IngGenomeViewerPtr igvp);
  extern void Ing_ProgressNew(IngGenomeViewerPtr igvp, CharPtr name);
  extern void Ing_ProgressEnd(IngGenomeViewerPtr igvp);
  extern void Ing_WinTimerProc(WindoW processW);
  extern void Ing_StartupWindow(void);
  extern CharPtr Ing_ParseSeqIdFile(FILE * fp);
  extern CharPtr Ing_StringCat(CharPtr a, CharPtr b);
  extern void Ing_mRNAOrExons(IteM item);
  extern IngGenomeViewerPtr Ing_MainDataNew(void);
  extern void Ing_PopulateOverviewPage(Pointer data, Boolean UpdateRuler);
  extern void Ing_PopulateDetailedPage(Pointer data);
  extern void Ing_DeleteSpidey(IngGenomeViewerPtr igvp, Int4 index);
  extern Uint1Ptr Ing_FillSeqBuffer(BioseqPtr bsp, SeqLocPtr slp, Int4 start, Int4 stop);
  extern void Ing_GetScoreAndEvalue(SeqAlignPtr sap, CharPtr PNTR eval_buff, CharPtr PNTR bit_score_buff);
  extern void Ing_InitAlignArrays(IngTrackAlignsPtr tap, Boolean bFirst);
  extern SeqLocPtr Ing_MakeLocListFromSpidp(Pointer userdata, SeqAlignPtr sap, Boolean gap_split);
  extern void Ing_tRNAscanMenu (MenU m);
extern Int4 Ing_PrintText(CharPtr str, SegmenT seg, Int4 row, Int4 winsize, Uint1Ptr clr, Uint2 primID);
  extern void Ing_ReplaceAllMismatchedInLocation(IngGenomeViewerPtr igvp, SeqAlignPtr sap, Int4 start, Int4 stop, Boolean All, Boolean fillGaps, Boolean delInserts);
  extern void Ing_GetFileForDotMatrix(IteM i);
  extern Boolean Ing_DoesAlignmentCoverAll(Pointer data);

#ifdef __cplusplus
}
#endif

#endif /* ndef _INGENWIN_ */

