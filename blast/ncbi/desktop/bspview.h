/*   seqview.h
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
* File Name:  seqview.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   4/30/95
*
* $Revision: 6.50 $
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

#ifndef _SEQVIEW_
#define _SEQVIEW_

#include <dlogutil.h>
#include <document.h>
#include <viewer.h>
#include <glbpic.h>


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct seqPanelLines {
  Int4    bioSeqLine;   /* this line refers to bioseq line           */
  Int2    lineType;     /* what to draw on this line (see ELineType) */
  Int4    idx;          /* feature index                             */
  Int4    row;          /* index in alignment                        */
  Boolean protProduct;
} SeqPanLine, PNTR SeqPanLinePtr;


/* bioseqviewdata pointer is passed to callbacks to display views */

typedef struct bioseqviewdata {
  BioseqPtr       bsp;

  VieweR          vwr;
  SegmenT         pict;
  DoC             doc;
  TexT            text;
  PaneL           pnl;

  PaneL           udv;
  PaneL           ddv;

  GrouP           styleControlGrp;
  GrouP           scaleControlGrp;
  GrouP           findGeneGrp;
  GrouP           docTxtControlGrp;
  GrouP           baseCtgControlGrp;
  GrouP           modeControlGrp;
  GrouP           newGphControlGrp;
  GrouP           pnlParentGrp;

  PaneL           seqView;
  GrouP           seqViewParentGrp;
  PopuP           newFeatControl;
  PopuP           newNumControl;
  PopuP           newGridControl;

  /* for new sequence and alignment viewers */
  Int4            LineHeight, LineSpace, CharHeight, CharWidth, BlocksAtLine, CharsAtLine, TotalLines, TargetRow;
  Int2            SeqStartPosX;  /* Draw Sequence from this x pos */
  SeqAlignPtr     salp;
  Boolean         DrawGrid, seqAlignMode;
  SeqPanLinePtr   PNTR SeqPanLines;
  ValNodePtr      Selection;


  GrouP           udvParentGrp;
  /*
  GrouP           ddvParentGrp;
  */
  PrompT          clickMe;

  Boolean         useScrollText;
  Boolean         launchEditors;
  Boolean         launchSubviewers;
  Boolean         sendSelectMessages;
  Boolean         highlightSelections;
  Boolean         hasTargetControl;

  Boolean         viewWholeEntity;
  Boolean         scaleNotCalculated;
  Boolean         moveToOldPos;

  PopuP           style;
  PopuP           scale;
  IteM            legendItem;
  Boolean         legendOK;
  Int4            maxScale;
  Int4            minIndex;

  PopuP           seqControl;
  PopuP           featControl;
  PopuP           numControl;

  PopuP           ffModeCtrl;
  PopuP           ffStyleCtrl;
  ButtoN          ffCustomBtn;

  PopuP           newGphStyle;
  PopuP           newGphFilter;
  PopuP           newGphScale;
  PopuP           newGphAlnScore;
  PopuP           newGphAlnCutoff;
  ChoicE          newGphLayout;

  ValNodePtr      slp_list;
  ValNodePtr      g_list;
  ValNodePtr      anp_node;
  ValNodePtr      ftype_list;
  Uint2           seq_entityID;
  GlobalDrawPtr   gdraw_p;
  Boolean         isGenome;

  Int2            expansion;

  Int2            itemClicked;
  Boolean         wasDoubleClick;
  Boolean         wasShiftKey;
  PoinT           pnt_start;
  PoinT           pnt_stop;
  Boolean         old_rect_shown;

  ValNodePtr      bsplist;

  FonT            displayFont;
  ValNodePtr      sentinelList;
  ValNodePtr      entityList;       /* for parts of genome record */
  ValNodePtr      tempResultList;   /* to view results before attaching */
  ForM            form;
} BioseqViewData, PNTR BioseqViewPtr;

/* callback prototypes for implementing operations on a given page in a viewer */

typedef void (*BioseqViewProc) (BioseqViewPtr bvp);
typedef void (*BioseqShowHideProc) (BioseqViewPtr bvp, Boolean show);
typedef void (*BioseqExportProc) (BioseqViewPtr bvp, CharPtr filename, CharPtr dfault);
typedef void (*BioseqSelectProc) (BioseqViewPtr bvp, Uint2 entityID, Uint2 itemID, Uint2 itemtype, SeqLocPtr region, Boolean select, Boolean scrollto);

/* bioseqpagedata pointer array allows flexible control of pages in viewer */

typedef struct bioseqpagedata {
  CharPtr                label;
  Boolean                nucOK;
  Boolean                protOK;
  Boolean                genomeOK;
  Boolean                needAlignment;
  Int4                   maxLength;
  BioseqViewProc         populate;
  BioseqShowHideProc     show;
  BioseqSelectProc       highlight;
  BioseqViewProc         toClipboard;
  BioseqViewProc         print;
  BioseqExportProc       exprt;
  BioseqExportProc       togif;
  BioseqViewProc         resize;
  struct bioseqpagedata  PNTR next;
} BioseqPageData, PNTR BioseqPagePtr;

/* bioseqpagedata records are available for all pre-defined report types */

extern BioseqPageData mapPageData;
extern BioseqPageData sumPageData;
extern BioseqPageData gphPageData;
extern BioseqPageData alnPageData;
extern BioseqPageData seqPageData;
/*
extern BioseqPageData udvPageData;
extern BioseqPageData ddvPageData;
*/

extern BioseqPageData gbgnPageData;
extern BioseqPageData gnbkPageData;
extern BioseqPageData emblPageData;
extern BioseqPageData ddbjPageData;
extern BioseqPageData gnptPageData;
extern BioseqPageData ftblPageData;

extern BioseqPageData fstaPageData;
extern BioseqPageData qualPageData;
extern BioseqPageData asnPageData;
extern BioseqPageData xmlPageData;
extern BioseqPageData gbseqPageData;
extern BioseqPageData dskPageData;

extern BioseqPageData asn2gphGphPageData;
extern BioseqPageData seqpnlPageData;
extern BioseqPageData seqAlnPnlPageData;

/*
*  The SeqViewProcsPtr may be registered with a call to SetAppProperty
*  e.g., SetAppProperty ("SeqDisplayForm", &viewprocs), where viewprocs
*  is a persistent structure filled with callback function pointers
*  specific for a given application.
*/

/* seqviewprocs is registered to allow communication to library functions */

typedef GrouP (*SeqViewControlsProc) (GrouP prnt, BaseFormPtr bfp, Int2 doctype, Int4 uid);
typedef GrouP (*SeqViewFetchAlignsProc) (GrouP prnt, BaseFormPtr bfp);
typedef Boolean (*SeqViewUpdateFetchCounts) (GrouP g, SeqEntryPtr sep);

typedef struct seqviewprocs {
  Boolean          hasTargetControl;
  Boolean          hasDoneButton;
  Boolean          hasDuplicateButton;
  Boolean          allowScrollText;
  Boolean          startInScrollText;
  Boolean          launchEditors;
  Boolean          launchSubviewers;
  Boolean          sendSelectMessages;
  Boolean          highlightSelections;
  Boolean          forceSeparateViewer;
  Boolean          keepSmartViewerVisible;

  Boolean          cleanupObjectPtr;
  WndActnProc      activateForm;
  WndActnProc      closeForm;
  WndActnProc      createMenus;
  GrpActnProc      createToolBar;

  FormMessageFunc  handleMessages;

  Int2             minPixelWidth;
  Int2             minPixelHeight;
  Int2             initNucPage;
  Int2             initProtPage;
  CharPtr          initNucLabel;
  CharPtr          initProtLabel;
  CharPtr          initGenomeLabel;
  Int2             useFolderTabs;

  FonT             displayFont;
  CharPtr          filepath;

  BioseqPagePtr    pageSpecs;

  SeqViewControlsProc  makeControls;
  GrpActnProc updateControls;
  SeqViewFetchAlignsProc  makeAlignBtn;
  SeqViewUpdateFetchCounts  updateCounts;

  IteM             alignWithChecked;  /* application sets to EntrezGlobalsPtr->alignWithChecked */
  Boolean          alignDefault;  /* application sets to EntrezGlobalsPtr->alignDefault */

  Boolean          lockFarComponents;
} SeqViewProcs, PNTR SeqViewProcsPtr;

typedef struct bioseqviewform {
  FORM_MESSAGE_BLOCK

  BioseqPagePtr   bioseqNucPageList;
  BioseqPagePtr   bioseqProtPageList;
  BioseqPagePtr   currentBioseqPage;
  ButtoN          pubseq;

  Int2            currentNucPage;
  Int2            currentProtPage;

  Handle          nucViewControl;
  Handle          protViewControl;
  Handle          targetControl;
  EnumFieldAssoc  PNTR targetAlist;
  Boolean         usePopupForTarget;
  Int4            numTargets;
  Int4            targetScratchSpace;
  GrouP           controls;
  GrpActnProc     updateControls;
  GrouP           retrieveAlignments;
  SeqViewUpdateFetchCounts  updateCounts;
  Boolean         hasaligns;

  EnumFieldAssoc  PNTR workingAlist;
  Int4            workingCount;
  Int4            workingTargets;

  BioseqViewData  bvd;

  Boolean         cleanupObjectPtr;
  WndActnProc     activateForm;

  ForM            toolForm;
} BioseqViewForm, PNTR BioseqViewFormPtr;

#define REGISTER_NEW_SEQENTRY_VIEW ObjMgrProcLoad(OMPROC_VIEW,"View Bioseq Report","Bioseq Report",OBJ_BIOSEQ,0,OBJ_BIOSEQ,0,NULL,NewSeqEntryViewGenFunc,PROC_PRIORITY_DEFAULT)
#define REGISTER_SMART_SEQENTRY_VIEW ObjMgrProcLoad(OMPROC_VIEW,"View Smart Bioseq Report","Bioseq Report",OBJ_BIOSEQ,0,OBJ_BIOSEQ,0,NULL,SmartSeqEntryViewGenFunc,PROC_PRIORITY_DEFAULT)

extern ForM LIBCALL CreateNewSeqEntryViewForm (Int2 left, Int2 top, CharPtr title,
                                            BioseqPtr bsp, SeqViewProcsPtr svpp);

/* RemoveSeqEntryViewer will hide and reuse the last window in Smart mode */
extern ForM RemoveSeqEntryViewer (ForM f);

extern Int2 LIBCALLBACK NewSeqEntryViewGenFunc (Pointer data);
extern Int2 LIBCALLBACK SmartSeqEntryViewGenFunc (Pointer data);

extern void LIBCALL NewSaveBioseqViewFormGifItemTable (Pointer formDataPtr, CharPtr filename);

extern void LIBCALL AddBioseqPageToList (BioseqPagePtr PNTR head, BioseqPagePtr bpp);
extern BioseqPagePtr LIBCALL BioseqPageListFree (BioseqPagePtr bpp);

extern IteM CreateLegendItem (MenU m, BaseFormPtr bfp);

extern ForM MakeToolFormForBioseqView (BaseFormPtr bfp, GrpActnProc createToolBar);
extern void SetBioseqViewTarget (BaseFormPtr fp, CharPtr seqId);

extern BioseqViewPtr GetBioseqViewPtrFromBaseFormPtr (BaseFormPtr fp);
NLM_EXTERN void Nlm_LaunchWebPage (Char *url);

extern Boolean BioseqViewCanSaveFasta (ForM f, Boolean nucs, Boolean prots, Boolean onlyTarget);
extern Boolean ExportBioseqViewFasta (ForM f, CharPtr filename, Boolean nucs, Boolean prots, Boolean onlyTarget);

extern Boolean SeqnSeqEntrysToFasta (SeqEntryPtr sep, FILE *fp, Boolean is_na, Uint1 group_segs);

/* The following functions are normally for internal use */

extern Int2 LIBCALLBACK BioseqViewMsgFunc (OMMsgStructPtr ommsp);

extern Boolean InBioseqViewEntityList (Uint2 entityID, BioseqViewPtr bvp);
extern void LIBCALL LaunchNewBioseqViewer (BioseqPtr bsp, Uint2 entityID, Uint2 itemID, Uint2 itemtype);
extern Boolean LIBCALL IsAGenomeRecord (SeqEntryPtr sep);
extern Boolean LIBCALL IsANamedAlignment (Uint2 entityID, Uint2 itemID, Uint2 itemtype);
extern Boolean IsSegmentedBioseqWithoutParts (SeqEntryPtr sep);
extern Boolean IsADeltaBioseq (SeqEntryPtr sep);
extern Boolean LIBCALL LaunchViewerNotEditor (BioseqViewPtr bvp, SeqEntryPtr sep,
                                              Uint2 entityID, Uint2 itemID, Uint2 itemtype);

extern ValNodePtr LIBCALL GetUidsForSeqEntryAligns (SeqEntryPtr sep);
extern ValNodePtr LIBCALL GetIdStringsForSeqEntryAligns (SeqEntryPtr sep);
extern void LIBCALL GetUidsForOneSeqAnnot (SeqAnnotPtr sap, ValNodePtr PNTR vnpp, Uint1 align_type);
extern int LIBCALLBACK SortByVnpDataIntvalue (VoidPtr ptr1, VoidPtr ptr2);

extern void ShowGeneList (ButtoN b);
extern void EnableDisableLegendItem (BioseqViewPtr bvp, Boolean enable);

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* ndef _SEQVIEW_ */

