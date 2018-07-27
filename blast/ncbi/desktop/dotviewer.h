/* dotviewer.h*/

#ifndef _DOTVIEWER_
#define _DOTVIEWER_

#ifdef __cplusplus
extern "C" {
#endif


  /****************************************************************************

      INCLUDE SECTION                                                                       
   ***************************************************************************/
#include <ncbi.h>
#include <accentr.h>
#include <vibrant.h>
#include <picture.h>
#include <picturep.h>
#include <viewer.h>
#include <tofasta.h>
#include <ncbidraw.h>
#include <seqport.h>
#include <sequtil.h>
#include <blastkar.h>
#include <blastpri.h>
#include <blastdef.h>
#include <objseq.h>
#include <salpstat.h>
#include <seqmgr.h>
#include <seqgraph.h>
#include <actutils.h>
#include <alignmgr.h>
#include <dotseq.h>

 
/****************************************************************************

      DEFINES SECTION                                                                 
 ***************************************************************************/
  /* scoop defines */
#define SCP_IDENTITY     0
#define SCP_CONTAINS     1
#define SCP_ISCONTAINED  2
#define SCP_DOVETAIL     3
#define SCP_HALFTAIL1    4
#define SCP_HALFTAIL2    5
#define SCP_MIDDLE       6
#define SCP_REPEATS      7
#define SCP_LARGE  1000
#define SCP_FUZZ  50
  /* scoop defines - end*/

#define REGISTER_DiagsDisplay  ObjMgrProcLoad (OMPROC_EDIT, "DOT_DiagsDisplay", "DOT_DiagsDisplay", OBJ_MAX, 0, OBJ_VIBRANT_PICTURE, 0, NULL, DOT_RegDiagsDisplay, PROC_PRIORITY_HIGHEST);


 /****************************************************************************

      DATA STRUCTURE SECTION                                                                
 ***************************************************************************/

/*******************************************************
* temporary form structures                            *
********************************************************/
typedef struct dot_blastinfo
{
   BioseqPtr  bsp1;
   BioseqPtr  bsp2;
   GrouP      localorglobal;
   GrouP      progname;
   GrouP      gapped;
   TexT       eval;
   TexT       wordsize;
   ButtoN     newblast;
   ButtoN     maskrep;
   ButtoN     masksimple;
} DOTblastinfo, PNTR DOTblastinfoPtr;

typedef struct dot_displayinfo
{
  GrouP      BLASTorDotPlot;
  GrouP      grid;
} DOTdisplayinfo, PNTR DOTdisplayinfoPtr;


typedef struct dot_paramsinfo
{
  TexT       xstart;
  TexT       xstop;
  TexT       ystart;
  TexT       ystop;
  TexT       word_size;
  TexT       tree_limit;
} DOTparamsinfo, PNTR DOTparamsinfoPtr;

/* scoop typedefs */
typedef struct scp_repeat {
   Int4   from1;
   Int4   to1;
   Int4   from2;
   Int4   to2;
   Uint1  strand2;
   struct scp_repeat PNTR next;
} SCP_Rpt, PNTR SCP_RptPtr;

typedef struct scp_result {
   Int4         relationship;
   Int4         len1;
   Int4         len2;
   Int4         numsaps;
   SeqAlignPtr  PNTR saps;
   Int4         numlarge_outliers;
   SeqAlignPtr  PNTR large_outliers;
   Int4         numsmall_outliers;
   SeqAlignPtr  PNTR small_outliers;
   SCP_RptPtr   repeats;
} SCP_Result, PNTR SCP_ResultPtr;

typedef struct scp_n {
   Int4  n1;
   Int4  n2;
   Int4  n3;
   Int4  n4;
   Int4  n5;
} SCP_n, PNTR SCP_nPtr;
/* scoop typedefs - end */

/*******************************************************
* storage for  alignment display                       *
********************************************************/

typedef struct saln {
  Uint1 show;
  Int4 q_start;
  Int4 q_stop;
  Int4 s_start;
  Int4 s_stop;
  Int1 class;
  SeqAlignPtr sap;
  Uint2 entityID;
  Uint2 itemID;
  Uint2 primID;
  struct saln PNTR next;
} DOTAln, PNTR DOTAlnPtr;

typedef struct algn{
  Uint2    entityID;
  Uint2    itemID;
  Int4     xlen;
  Int4     ylen;
  Int4     xstart;
  Int4     ystart;
  Int4     Fh;
  WindoW   w;
  VieweR   v;
  SeqAlignPtr sap;
  SegmenT     pict;
  SegmenT     seg1;
  PopuP    scale;
  Int4     scaleValue;
  Int4     scaleIndex;
  Boolean  showLabels;
  Boolean  do_scale;
  PrompT   Infopanel;
  CharPtr  title;
  CharPtr  name1, name2;
  SeqIdPtr  sip;
  Int4Ptr  PNTR matrix; /* dna matrix */
  Boolean     is_na; 
  Int4     HORZ_MARGIN;
  Int4     VERT_MARGIN;
  Int4     index; /* num. of alignments */
  DOTAlnPtr    PNTR  Alnlist;
} DOTAlignInfo, PNTR DOTAlignInfoPtr;


/*******************************************************
* scroll and threshold data                            *
********************************************************/

  typedef struct scrolldata {
    /* threshold bar info */
    BaR      ScrollBar; 
    Int4     TrampPos;
    /* main window scroll information */
    Int4     YScrlMax;
    Int4     XScrlMax;
    Int4     YScrlPos;
    Int4     XScrlPos;
    Int4     UnitY;
    Int4     UnitX;
    Int4     TotUnitsY;
    Int4     TotUnitsX;
    Int4     PgWdth;
    Int4     PgLen;
    Int4     YScrlPage;
    Int4     XScrlPage;
    /* scroll positions in pixel coordinates */
    Int4     HFrom; 
    Int4     VFrom;
  } DOTScrollData, PNTR DOTScrollDataPtr;


/*******************************************************
* store sequence data on selected region for new display*
*                                                       *
********************************************************/
typedef struct seqviewr{
  Boolean   do_scale;
  PopuP     scale, showp;
  Int4      scaleIndex;
  Int4      scaleValue;
  GrouP     Labels;
  Boolean   showLabels;
  PoinT     old_pt;
  Int2      old_primID;
  Uint2     highlight;
  SegmenT   pict1;
  SegmenT   pict2;
  DOTAlnPtr salp;
  VieweR    v1;
  VieweR    v2;
  WindoW    w;
  Int4      barp;
} DOTSeqViewr, PNTR DOTSeqViewrPtr;


/*******************************************************
* main vibrant data structure                          *
********************************************************/
typedef struct dotvibdata{
  WindoW MainWin;
  WindoW ChildWin;
  Int4   curr_slen;
  Int4   curr_qlen;
  Int4   xlen, ylen;
  PaneL  panel;
  Boolean showGrid;
  Boolean     showDotPlot;
  Boolean     showALIGN;
  PrompT      Infopanel;
  Char        iInfo[255];
  Int4        comp; 
  Int4        originalcomp;
  /* Alignment options */
  Boolean     Blast2Seq_show;
  MenU        displayOpts1;
  ChoicE      displayOpts2;
  GrouP       Ggoto;
  Int4        ystart, xstart, ystop, xstop;
  CharPtr     xname;
  CharPtr     yname;
   Uint1Ptr    seq1, seq2; 
  Uint1       strand1, strand2;
  DOTAlignInfoPtr alp;
  /* second window elements */
  DOTSeqViewrPtr sv;
  DOTScrollData  sdp;
  Uint2       selectMode;
  VoidPtr     data;
  FonT        Fnt;
  Int4        Fh;
  Int4        charw; 
  Int4        VERT_MARGIN;
  Int4        HORZ_MARGIN;
  DOTMainDataPtr     mip;
} DOTVibData, PNTR DOTVibDataPtr;


/*******************************************************
* structures to store feature data for the feature viewer *
********************************************************/

typedef struct dot_feat_info {
   CharPtr  label;
   Int4     left;
   Int4     right;
   Uint1    strand;
   Int2     type;
   struct dot_feat_info PNTR next;
} DOTFeat, PNTR DOTFeatPtr;


typedef struct dot_row {
   DOTFeatPtr dfp;
} DOTRow, PNTR DOTRowPtr;

typedef struct dot_featindex {
  CharPtr   label;
  Boolean   present;
  Boolean   show;
  Int2      deref; 
} DOTFeatIndex, PNTR DOTFeatIndexPtr;


typedef struct dot_popfeat{
  VieweR TopParentView;
  SegmenT TopParentSeg;
  DOTRowPtr drp;
  DOTFeatIndexPtr featindex;
  DOTFeatPtr dfp_cur;
  Int4       nfeats;
  Int2       fontHeight;
} DOTPopFeat, PNTR DOTPopFeatPtr;


typedef struct dot_feats_list{
  WindoW FeatWin;
  WindoW hFeatDlg;
  VieweR Query;
  SegmenT segQuery;
  SegmenT segQName;
  SegmenT segQCursor;
  VieweR  Subject;
  SegmenT segSubject;
  SegmenT segSName;
  SegmenT segSCursor;
  Int4    qFeatscount;
  Int4    sFeatscount;
  Int2    fontHt;
  Int4    vert_Qpos;
  Int4    vert_Spos;
  Uint2     procID;
  Uint2     userKey;
  Uint2     entityID;
  Uint2     itemID;
  LisT      featList;
  PrompT     QInfo;
  PrompT     SInfo;
  DOTRowPtr  query_drp;
  DOTRowPtr  subject_drp;
  DOTFeatIndexPtr featindex; 
  Int4       numrows; /* list of feature types */
  VoidPtr     data;
  DOTMainDataPtr   mip;
} DOTFeatList, PNTR DOTFeatListPtr;

typedef struct dot_sel_feat{
  Int4      feat_num;
  struct dot_sel_feat PNTR next;
} DOTSelFeat, PNTR DOTSelFeatPtr;


/*******************************************************
* store sequence data on selected region for new display*
*                                                       *
********************************************************/


typedef struct selectdata{
  Int4      q_start;
  Int4      s_start;
  Int4      q_stop;
  Int4      s_stop;
  Int4      xstart, xstop, ystart, ystop;
  Int4      qlen;
  Int4      slen;
  RecT      rcS;  
  RecT      old_rcS; 
  RecT      rcP; 
  Boolean   selected;
  Boolean   rm_lastselected;
  Int2      H_pos;
  Int2      V_pos;
  DOTVibDataPtr vdp;
} DOTSelData, PNTR DOTSelDataPtr;



 /****************************************************************************

      FUNCTION DECLARATIONS                                                               
 ***************************************************************************/
NLM_EXTERN Int2 LIBCALLBACK DOT_RegDiagsDisplay(Pointer data);
NLM_EXTERN Boolean DOT_MakeMainViewer(DOTMainDataPtr vdp, DOTAlignInfoPtr alp);
NLM_EXTERN Boolean DOT_AlignPlotGivenSeqAlign(SeqAlignPtr sap);
NLM_EXTERN Boolean DOT_AlignPlotGivenScp(SCP_ResultPtr scp);

extern DOTAlignInfoPtr DOT_AlignInfoNew();
extern Boolean DOT_FillAlignInfoPointer (DOTAlignInfoPtr alp);
extern Int4    DOT_GetValue(TexT t);

/* scoop declarations */
NLM_EXTERN SCP_ResultPtr SCP_CompareOrderOrganizeBioseqs(BioseqPtr bsp1, BioseqPtr bsp2, SeqLocPtr slp1, SeqLocPtr slp2, CharPtr progname, Int4 wordsize, Int4 hitlist_size);

extern void SCP_OrganizeAlnsInSet(SeqAlignPtr sap, Int4 fuzz, SCP_ResultPtr scp, Int4 n);
/* scoop declarations - end */

#ifdef __cplusplus
}
#endif

#endif /* _DOTVIEWER_ */

