/*   smdlg3.c
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
* File Name:  smdlg3.c
*
* Author:  Alex Smirnov
*
* Version Creation Date:   8/10/95
*
* $Revision: 6.1 $
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

/**************************************************************************/
/* INCLUDE */
/**************************************************************************/
#include <vibrant.h>
#include <picture.h> 
#include <fstyle.h> 
#include <fstylep.h> 

/**************************************************************************/
/* DEFINES */
/**************************************************************************/
#define SM_DLG_FEAT       0
#define SM_DLG_EXTRA      1
#define SM_DLG_SEQ        2

#define SM_ARROW_SIZE     10

#define APPLAY_DLG_UP     1

/**************************************************************************/
/* TYPEDEFS */
/**************************************************************************/
typedef struct {
  Int2 subClass;
  Int2 pType;
} Nlm_ClipField, PNTR Nlm_ClipFieldPtr;

typedef struct {
  Int2 checkNum[3];
  Int4 musk[3];
} Nlm_ParamToCheck;

static void  FeatureExtraDraw  ( PaneL p );
static void  SeqDraw           ( PaneL p );

/**************************************************************************/
/* EXTERN VARIABLE */
/**************************************************************************/
extern CharPtr mSM_allfeatureNames[];

/**************************************************************************/
/* STATIC VARIABLE */
/**************************************************************************/
static Int2    curDlg;
static Int2    curSubDlg[3];
static Nlm_ClipField inClipSchema[] = {
  /*0*/{MSM_SEGMENT,MSM_COLOR},    /*1*/{MSM_SEGMENT,MSM_SHADING},
  /*2*/{MSM_SEGMENT,MSM_STYLE},    /*3*/{MSM_SEGMENT,MSM_HEIGHT},
  /*4*/{MSM_SEG_BORD,MSM_COLOR},   /*5*/{MSM_SEG_BORD,MSM_TRUEFALSE},
  /*6*/{MSM_SEG_BORD,MSM_LTYPE},   /*7*/{MSM_SEG_BORD,MSM_PENWIDTH},
  /*8*/{MSM_FGAP,MSM_COLOR},       /*9*/{MSM_FGAP,MSM_STYLE},
  /*10*/{MSM_FGAP,MSM_LTYPE},      /*11*/{MSM_FGAP,MSM_PENWIDTH},
  /*12*/{MSM_FLABEL,MSM_COLOR},    /*13*/{MSM_FLABEL,MSM_STYLE},
  /*14*/{MSM_FLABEL,MSM_FONT},     /*15*/{MSM_FLABEL,MSM_HEIGHT},
  /*16*/{MSM_NOSUBCLASS,0}
};

#define MSM_SEG_SHOWORIENT   0x10    /* show orientation */
#define MSM_SEG_SHOWTRUNC    0x20    /* show trancation */
static Nlm_ParamToCheck paramToCheck[] = {
  /*0*/{{3,-1,-1},{0,0,0}},        /*1*/{{4,-1,-1},{0,0,0}},
  /*2*/{{0,2,5},{MSM_SEG_FORM,MSM_SEG_SHOWORIENT,MSM_SEG_SHOWTRUNC}},
                                   /*3*/{{1,-1,-1},{0,0,0}},
  /*4*/{{7,0,0},{0,0,0}},          /*5*/{{6,0,0},{0,0,0}},
  /*6*/{{8,0,0},{0,0,0}},          /*7*/{{8,0,0},{0,0,0}},
  /*8*/{{10,0,0},{0,0,0}},         /*9*/{{9,0,0},{0,0,0}},
  /*10*/{{11,0,0},{0,0,0}},        /*11*/{{11,0,0},{0,0,0}},
  /*12*/{{13,0,0},{0,0,0}},        /*13*/{{12,0,0},{0,0,0}},
  /*14*/{{14,0,0},{0,0,0}},        /*15*/{{14,0,0},{0,0,0}}
};

static BigScalar inClipBoard[17];

static CharPtr folderFeatNamesF[] = {
  "Segment","Border","Gap","Label", NULL
};
static CharPtr folderSeqNamesF[] = {
  "Segment","Border","Sequence label", NULL
};
static CharPtr PNTR folderNamesF[3] = {
  folderFeatNamesF, folderFeatNamesF, folderSeqNamesF
};
static GrouP   hGroupsFeat[4];
static GrouP   hGroupsExtra[4];
static GrouP   hGroupsSeq[3];
static GrouP   PNTR hGroups[3] = {
  hGroupsFeat, hGroupsExtra, hGroupsSeq
};
static Nlm_PnlActnProc  drawProc[3] = {
  FeatureExtraDraw, FeatureExtraDraw, SeqDraw
};

static PaneL exPanel[3];

static PopuP   featPopuP;
static LisT    featList;
static Int2    featAr[100];
static Int2    curFeatInd;
static Int2    lastFeatInd;
static ButtoN  applayButton;
static ButtoN  applayChecks[15];

static Int2   curExtraInd;

static ButtoN pasteButton[3];

static ButtoN borderCheckBox[3] = {NULL,NULL,NULL};
static GrouP  gapStlyleGroup[3] = {NULL,NULL,NULL};
static GrouP  labelStyleGroup[3] = {NULL,NULL,NULL};
static GrouP  segStyleGroup[3] = {NULL,NULL,NULL};
static PopuP  segHeightPopup[3] = {NULL,NULL,NULL};
static ButtoN trancCheckBox[3] = {NULL,NULL,NULL};
static ButtoN orientCheckBox[3] = {NULL,NULL,NULL};

static FontSpec  inFsp;

static CharPtr featLabelNames[] = {
  "Type", "Content", "Type:Content", "Summary"
};
static CharPtr seqLabelNames[] = {
  "FASTA short", "FASTA long", "TEXTID locus", "TEXTID accession",
  "TEXTID report"
};
static Int2 groupAr[10];

static Nlm_Uint1 rectFillSym [] = {
  0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x10,
  0x10, 0x10, 0x10, 0xFE
};
static Nlm_Uint1 diamondFillSym [] = {
  0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x10, 0x10,
  0x10, 0x10, 0x10, 0xFE
};
static Nlm_Uint1 ovalFillSym [] = {
  0x38, 0x7C, 0xFE, 0xFE, 0xFE, 0x7C, 0x38, 0x10,
  0x10, 0x10, 0x10, 0xFE
};
static Nlm_Uint1 downTriFillSym [] = {
  0xFE, 0xFE, 0x7C, 0x7C, 0x38, 0x38, 0x10, 0x10,
  0x10, 0x10, 0x10, 0xFE
};

static Int2 applayStatus = 0;

/**************************************************************************/
/* STATIC FUNCTION */
/**************************************************************************/
static void
/*FCN*/SetMyColor ( 
  BigScalar color
){
  Uint1 r,g,b;

  r = (Uint1)(color >> 16 ) & 0xFF;
  g = (Uint1)(color >> 8 ) & 0xFF;;
  b = (Uint1)color & 0xFF;
  SelectColor (r,g,b);
}

static void
/*FCN*/SetMyShadding ( 
  BigScalar shadding
){
  switch (shadding){
    case DARK_SHADING: 
      Dark();
      break;
    case MEDIUM_SHADING:
      Medium();
      break;
    case LIGHT_SHADING:
      Light();
      break;
    default:
      Solid();
  }
}

static void
/*FCN*/SetMyLType ( 
  BigScalar ltype
){
  switch (ltype){
    case DOTTED_LINE:
      Dotted(); break;
    case DASHED_LINE:
      Dashed(); break;
    default:
      Solid();
  }
}

static void
/*FCN*/UpdateTools ( 
  void
){
  BigScalar val, val1;
  Int2      curClass;

  switch ( curDlg ){
    case SM_DLG_FEAT:
      if ( curFeatInd == -1 ) {
        Nlm_ShowToolsSM ( 0 );
        return;
      }
      curClass = featAr[curFeatInd];
      break;
    case SM_DLG_EXTRA:
      if ( curExtraInd == -1 ) {
        Nlm_ShowToolsSM ( 0 );
        return;
      }
      curClass = MSM_EXTRA_GENBANK - curExtraInd;
      break;
    default:
      curClass = MSM_SEQUENCE;
  }
  /*Segment*/
  if ( curSubDlg[curDlg] == 0 ){
    val = Nlm_GetMuskCParamEd(curClass,MSM_SEGMENT,MSM_COLOR);
    Nlm_SetToolDefaultSM ( SM_TOOLS_COLOR, val );
    val = Nlm_GetMuskCParamEd(curClass,MSM_SEGMENT,MSM_SHADING);
    Nlm_SetToolDefaultSM ( SM_TOOLS_SHADDING, val );
    val = Nlm_GetMuskCParamEd(curClass,MSM_SEGMENT,MSM_LTYPE);
    val1 = Nlm_GetMuskCParamEd(curClass,MSM_SEGMENT,MSM_PENWIDTH);
    Nlm_SetToolDefaultSM ( SM_TOOLS_LTYPE, ((val1<<8)|val) );
    val = Nlm_GetMuskCParamEd(curClass,MSM_SEGMENT,MSM_STYLE);
    switch ( (Int2)val & MSM_SEG_FORM ){
      case MSM_SEG_BOX:
        Nlm_ShowToolsSM ( SM_SHTOOLS_COLOR|SM_SHTOOLS_SHADDING );
        break;
      case MSM_SEG_LINE:
        Nlm_ShowToolsSM ( SM_SHTOOLS_COLOR|SM_SHTOOLS_LTYPE );
        break;
      default:
        Nlm_ShowToolsSM ( SM_SHTOOLS_COLOR );
    }
  }
  /* Border */
  if ( curSubDlg[curDlg] == 1 ){
    val = Nlm_GetMuskCParamEd(curClass,MSM_SEG_BORD,MSM_COLOR);
    Nlm_SetToolDefaultSM ( SM_TOOLS_COLOR, val );
    val = Nlm_GetMuskCParamEd(curClass,MSM_SEG_BORD,MSM_LTYPE);
    val1 = Nlm_GetMuskCParamEd(curClass,MSM_SEG_BORD,MSM_PENWIDTH);
    Nlm_SetToolDefaultSM ( SM_TOOLS_LTYPE, ((val1<<8)|val) );
    Nlm_ShowToolsSM ( SM_SHTOOLS_COLOR|SM_SHTOOLS_LTYPE );
  }
  /* Gap */
  if ( (curSubDlg[curDlg] == 2)&&(curDlg != SM_DLG_SEQ) ){
    val = Nlm_GetMuskCParamEd(curClass,MSM_FGAP,MSM_COLOR);
    Nlm_SetToolDefaultSM ( SM_TOOLS_COLOR, val );
    val = Nlm_GetMuskCParamEd(curClass,MSM_FGAP,MSM_LTYPE);
    val1 = Nlm_GetMuskCParamEd(curClass,MSM_FGAP,MSM_PENWIDTH);
    Nlm_SetToolDefaultSM ( SM_TOOLS_LTYPE, ((val1<<8)|val) );
    Nlm_ShowToolsSM ( SM_SHTOOLS_COLOR|SM_SHTOOLS_LTYPE );
  }
  /*Label */
  if ( ((curSubDlg[curDlg] == 2)&&(curDlg == SM_DLG_SEQ)) ||
       ((curSubDlg[curDlg] == 3)&&(curDlg != SM_DLG_SEQ)) ){
    if ( curDlg == SM_DLG_SEQ ){
      val = Nlm_GetMuskCParamEd(curClass,MSM_SLABEL,MSM_COLOR);
    } else {
      val = Nlm_GetMuskCParamEd(curClass,MSM_FLABEL,MSM_COLOR);
    }
    Nlm_SetToolDefaultSM ( SM_TOOLS_COLOR, val );
    val = Nlm_GetMuskCParamEd(curClass,MSM_FLABEL,MSM_FONT);
    if ( val == 0 ){
      Nlm_GetFontSpec ( (Nlm_FonT)Nlm_programFont, &inFsp);
    } else {
      Nlm_GetFontSpec ( (Nlm_FonT)val, &inFsp);
    }
    Nlm_SetToolDefaultSM ( SM_TOOLS_FONT, (BigScalar)&inFsp );
    Nlm_ShowToolsSM ( SM_SHTOOLS_COLOR|SM_SHTOOLS_FONT );
  }
}

static void
/*FCN*/UpdateDlgControls ( 
  Boolean panel,
  Boolean other
){
  BigScalar val;
  WindoW    tmpPort;
  Int2      curClass;
  Int2      i;
  RecT      rp;

  switch ( curDlg ){
    case SM_DLG_FEAT:
      if ( curFeatInd == -1 ) return;
      curClass = featAr[curFeatInd];
      break;
    case SM_DLG_EXTRA:
      if ( curExtraInd == -1 ) return;
      curClass = MSM_EXTRA_GENBANK - curExtraInd;
      break;
    default:
      curClass = MSM_SEQUENCE;
  }
  if ( other ){
    val = Nlm_GetMuskCParamEd(curClass,MSM_SEGMENT,MSM_STYLE);
    if ( curDlg == SM_DLG_SEQ ){
      if ( val == MSM_SEG_BOX ) SetValue ( segStyleGroup[curDlg], 1 );
      else SetValue ( segStyleGroup[curDlg], 2 );
    } else {
      switch ( val & MSM_SEG_FORM ){
        case MSM_SEG_LINE:         i=4; break;
        case MSM_SEG_SYM_RECT:     i=2; break;
        case MSM_SEG_SYM_DIAMOND:  i=5; break;
        case MSM_SEG_SYM_OVAL:     i=3; break;
        case MSM_SEG_SYM_TRIANGLE: i=6; break;
        default:                   i=1 ;
      }
      SetValue ( segStyleGroup[curDlg], i );
      if ( val & MSM_SEG_SHOWORIENT ) SetStatus ( orientCheckBox[curDlg], TRUE );
      else SetStatus ( orientCheckBox[curDlg], FALSE );
      if ( val & MSM_SEG_SHOWTRUNC ) SetStatus ( trancCheckBox[curDlg], TRUE );
      else SetStatus ( trancCheckBox[curDlg], FALSE );
    }
    val = Nlm_GetMuskCParamEd(curClass,MSM_SEGMENT,MSM_HEIGHT);
    i = (val - 4)/2 + 1;
    if ( i <= 0 ) i = 1;
    if ( i > 9 ) i = 9;
    SetValue ( segHeightPopup[curDlg], i );
    val = Nlm_GetMuskCParamEd(curClass,MSM_SEG_BORD,MSM_TRUEFALSE);
    SetValue ( borderCheckBox[curDlg], (Boolean)val );
    val = Nlm_GetMuskCParamEd(curClass,MSM_FGAP,MSM_STYLE);
    if ( curDlg != SM_DLG_SEQ ){
      SetValue ( gapStlyleGroup[curDlg], (Int2)val + 1 );
    }
    val = Nlm_GetMuskCParamEd(curClass,MSM_FLABEL,MSM_STYLE);
    if ( curDlg != SM_DLG_SEQ ){
      switch ( val ){
        case MSM_LABEL_TYPE:    i=2; break;
        case MSM_LABEL_CONTENT: i=3; break;
        case MSM_LABEL_BOTH:    i=4; break;
        case MSM_LABEL_SUMMARY: i=5; break;
        default:                i=1;
      }
    } else {
      switch ( val ){
        case PRINTID_FASTA_LONG:       i=2; break;
        case PRINTID_TEXTID_LOCUS:     i=3; break;
        case PRINTID_TEXTID_ACCESSION: i=4; break;
        case PRINTID_REPORT:           i=5; break;
        default:                       i=1;
      }
    }
    SetValue(labelStyleGroup[curDlg],i);
  }
  if ( panel ){
    ObjectRect ( exPanel[curDlg], &rp );
    tmpPort = SavePort ( exPanel[curDlg] );
    Select ( exPanel[curDlg] );
    InvalRect ( &rp );
    RestorePort ( tmpPort );
  }
}

static void
/*FCN*/FeatureExtraDraw(
  PaneL p
){
  BigScalar val;
  RecT      rp;
  RecT      rdraw1;
  RecT      rdraw2;
  Int2      curClass;
  Int2      y0, y1, x0, x1, x2, x3;
  Nlm_PoinT p1, p2;
  Nlm_PoinT pPoly[3];
  Boolean   mayBeGap = FALSE;

  ObjectRect ( p, &rp );
  if ( curDlg == SM_DLG_FEAT )  {
    if ( curFeatInd == -1 ) return;
    curClass = featAr[curFeatInd];
  } else {
    if ( curExtraInd == -1 ) return;
    curClass = MSM_EXTRA_GENBANK - curExtraInd;
  }
  val = Nlm_GetMuskCParamEd(curClass,MSM_SEGMENT,MSM_STYLE);
  y0 = (rp.top + rp.bottom)/2;
  x0 = rp.left+10; x1 = x2 = (rp.left + rp.right)/2; x3 = rp.right-10;
  x1 -= 20; x2 += 20;
  SetMyColor ( Nlm_GetMuskCParamEd(curClass,MSM_SEGMENT,MSM_COLOR));
  switch ( val & MSM_SEG_FORM ){
    case MSM_SEG_BOX:
      y1 = y0 + (Int2)Nlm_GetMuskCParamEd(curClass,MSM_SEGMENT,MSM_HEIGHT);
      if ( val & MSM_SEG_SHOWORIENT ){
        x3 -= SM_ARROW_SIZE;
        pPoly[0].x = pPoly[2].x = x3;
        pPoly[1].x = x3+SM_ARROW_SIZE;
        pPoly[0].y = y0-2;
        pPoly[1].y = (y0+y1)/2-1;
        pPoly[2].y = y1+1;
      }
      LoadRect ( &rdraw1, x0, y0, x1, y1 );
      LoadRect ( &rdraw2, x2, y0, x3, y1 );
      SetMyShadding ( Nlm_GetMuskCParamEd(curClass,MSM_SEGMENT,MSM_SHADING));
      PaintRect ( &rdraw1 );
      PaintRect ( &rdraw2 );
      if ( val & MSM_SEG_SHOWORIENT ){
        PaintPoly ( 3, &(pPoly[0]) );
      }
      if ( (Boolean)Nlm_GetMuskCParamEd(curClass,MSM_SEG_BORD,MSM_TRUEFALSE) ){
        WidePen ( (Int1)Nlm_GetMuskCParamEd(curClass,MSM_SEG_BORD,MSM_PENWIDTH));
        SetMyLType ( Nlm_GetMuskCParamEd(curClass,MSM_SEG_BORD,MSM_LTYPE));
        SetMyColor ( Nlm_GetMuskCParamEd(curClass,MSM_SEG_BORD,MSM_COLOR));
        FrameRect( &rdraw1 );
        FrameRect( &rdraw2 );
        if ( val & MSM_SEG_SHOWORIENT ){
          FramePoly ( 3, &(pPoly[0]) );
        }
      }
      mayBeGap = TRUE;
      break;
    case MSM_SEG_LINE:
      y1 = y0+10;
      if ( val & MSM_SEG_SHOWORIENT ){
        x3 -= SM_ARROW_SIZE;
        pPoly[0].x = pPoly[2].x = x3;
        pPoly[1].x = x3+SM_ARROW_SIZE;
        pPoly[0].y = y0;
        pPoly[1].y = (y0+y1)/2;
        pPoly[2].y = y1;
      }
      WidePen ( (Int1)Nlm_GetMuskCParamEd(curClass,MSM_SEGMENT,MSM_PENWIDTH));
      SetMyLType ( Nlm_GetMuskCParamEd(curClass,MSM_SEGMENT,MSM_LTYPE));
      p1.y = p2.y = (y0+y1)/2;
      p1.x = x0; p2.x = x1;
      DrawLine(p1,p2);
      p1.x = x2; p2.x = x3;
      DrawLine(p1,p2);
      if ( val & MSM_SEG_SHOWORIENT ){
        Solid();
        PaintPoly ( 3, &(pPoly[0]) );
      }
      mayBeGap = TRUE;
      break;
    case MSM_SEG_SYM_RECT:
      Nlm_LoadRect ( &rdraw1, x0, y0, x0 + 8, y0 + 12 );
      Nlm_CopyBits (&rdraw1, &(rectFillSym[0]) );
      break;
    case MSM_SEG_SYM_DIAMOND:
      Nlm_LoadRect ( &rdraw1, x0, y0, x0 + 8, y0 + 12 );
      Nlm_CopyBits (&rdraw1, &(diamondFillSym[0]) );
      break;
    case MSM_SEG_SYM_OVAL:
      Nlm_LoadRect ( &rdraw1, x0, y0, x0 + 8, y0 + 12 );
      Nlm_CopyBits (&rdraw1, &(ovalFillSym[0]) );
      break;
    case MSM_SEG_SYM_TRIANGLE:
      Nlm_LoadRect ( &rdraw1, x0, y0, x0 + 8, y0 + 12 );
      Nlm_CopyBits (&rdraw1, &(downTriFillSym[0]) );
  }
  val = Nlm_GetMuskCParamEd(curClass,MSM_FGAP,MSM_STYLE);
  if ( (val != MSM_GAP_NONE) && mayBeGap ){
    SetMyColor ( Nlm_GetMuskCParamEd(curClass,MSM_FGAP,MSM_COLOR));
    WidePen ( (Int1)Nlm_GetMuskCParamEd(curClass,MSM_FGAP,MSM_PENWIDTH));
    SetMyLType ( Nlm_GetMuskCParamEd(curClass,MSM_FGAP,MSM_LTYPE));
    if ( val == MSM_GAP_ANGLE ){
      p1.x = x1; p2.x = (x1+x2)/2;
      p1.y = (y0+y1)/2; p2.y = y0;
      DrawLine(p1,p2);
      p1.x = x2;
      DrawLine(p1,p2);
    } else {
      p1.x = x1; p2.x = x2;
      p1.y = p2.y = (y0+y1)/2; 
      DrawLine(p1,p2);
    }
  }
  val = Nlm_GetMuskCParamEd(curClass,MSM_FLABEL,MSM_STYLE);
  if ( val != MSM_LABEL_NONE ){
    SetMyColor ( Nlm_GetMuskCParamEd(curClass,MSM_FLABEL,MSM_COLOR));
    Nlm_SelectFont ((FonT)Nlm_GetMuskCParamEd(curClass,MSM_FLABEL,MSM_FONT));
    y0 -= 5;
    y1 = y0;
    y0 -= (Int2)Nlm_GetMuskCParamEd(curClass,MSM_FLABEL,MSM_HEIGHT);
    LoadRect ( &rdraw1, x0, y0, x3, y1 );
    Nlm_DrawString ( &rdraw1, featLabelNames[(Int2)val], 'l', FALSE );
  }
}

static void
/*FCN*/SeqDraw(
  PaneL p
){
  BigScalar val;
  RecT      rp;
  RecT      rdraw;
  Int2      y0, y1, x0, x1;
  Nlm_PoinT p1, p2;

  ObjectRect ( p, &rp );
  val = Nlm_GetMuskCParamEd(MSM_SEQUENCE,MSM_SEGMENT,MSM_STYLE);
  y0 = (rp.top + rp.bottom)/2;
  x0 = rp.left+10; x1 = rp.right-10;
  if ( val == MSM_SEG_BOX ){
    y1 = y0 + (Int2)Nlm_GetMuskCParamEd(MSM_SEQUENCE,MSM_SEGMENT,MSM_HEIGHT);
    LoadRect ( &rdraw, x0, y0, x1, y1 );
    SetMyColor ( Nlm_GetMuskCParamEd(MSM_SEQUENCE,MSM_SEGMENT,MSM_COLOR));
    SetMyShadding ( Nlm_GetMuskCParamEd(MSM_SEQUENCE,MSM_SEGMENT,MSM_SHADING));
    PaintRect ( &rdraw );
    if ( (Boolean)Nlm_GetMuskCParamEd(MSM_SEQUENCE,MSM_SEG_BORD,MSM_TRUEFALSE)){
      WidePen ( (Int1)Nlm_GetMuskCParamEd(MSM_SEQUENCE,MSM_SEG_BORD,MSM_PENWIDTH));
      SetMyLType ( Nlm_GetMuskCParamEd(MSM_SEQUENCE,MSM_SEG_BORD,MSM_LTYPE));
      SetMyColor ( Nlm_GetMuskCParamEd(MSM_SEQUENCE,MSM_SEG_BORD,MSM_COLOR));
      FrameRect( &rdraw );
    }
  } else {
    p1.x = x0; p2.x = x1;
    p1.y = p2.y = (rp.top + rp.bottom)/2 + 1;
    SetMyColor ( Nlm_GetMuskCParamEd(MSM_SEQUENCE,MSM_SEGMENT,MSM_COLOR));
    WidePen ( (Int1)Nlm_GetMuskCParamEd(MSM_SEQUENCE,MSM_SEGMENT,MSM_PENWIDTH));
    SetMyLType ( Nlm_GetMuskCParamEd(MSM_SEQUENCE,MSM_SEGMENT,MSM_LTYPE));
    DrawLine(p1,p2);
  }
  val = Nlm_GetMuskCParamEd(MSM_SEQUENCE,MSM_FLABEL,MSM_STYLE)-1;
  SetMyColor ( Nlm_GetMuskCParamEd(MSM_SEQUENCE,MSM_FLABEL,MSM_COLOR));
  Nlm_SelectFont ((FonT)Nlm_GetMuskCParamEd(MSM_SEQUENCE,MSM_SLABEL,MSM_FONT));
  y0 -= 5;
  y1 = y0;
  y0 -= (Int2)Nlm_GetMuskCParamEd(MSM_SEQUENCE,MSM_SLABEL,MSM_HEIGHT);
  LoadRect ( &rdraw, x0, y0, x1, y1 );
  Nlm_DrawString ( &rdraw, seqLabelNames[(Int2)val], 'l', FALSE );
}

static void 
/*FCN*/OnClipCopy (
 ButtoN b
){
  Int2 curClass;
  Int2 i;

  switch ( curDlg ){
    case SM_DLG_FEAT:
      curClass = featAr[curFeatInd];
      break;
    case SM_DLG_EXTRA:
      curClass = MSM_EXTRA_GENBANK - curExtraInd;
      break;
    default:
      return;
  }
  for ( i=0; inClipSchema[i].subClass != MSM_NOSUBCLASS ;i++ ){
    inClipBoard[i] = Nlm_GetMuskCParamEd( curClass, inClipSchema[i].subClass,
                                          inClipSchema[i].pType );
  }
  Enable ( pasteButton[SM_DLG_FEAT] );
  Enable ( pasteButton[SM_DLG_EXTRA] );
}

static void 
/*FCN*/OnClipPaste (
 ButtoN b
){
  Int2 curClass;
  Int2 i;

  switch ( curDlg ){
    case SM_DLG_FEAT:
      curClass = featAr[curFeatInd];
      break;
    case SM_DLG_EXTRA:
      curClass = MSM_EXTRA_GENBANK - curExtraInd;
      break;
    default:
      return;
  }
  for ( i=0; inClipSchema[i].subClass != MSM_NOSUBCLASS ;i++ ){
    Nlm_SetMuskCParamEd( curClass, inClipSchema[i].subClass,
                         inClipSchema[i].pType, inClipBoard[i] );
  }
  UpdateDlgControls(TRUE,TRUE);
}

static void
/*FCN*/LoadFeatPopup (
  void
){
  Int2 tgroup;
  Int2 i,j;
  Int2 curInd = 0;

  tgroup = (Int2)Nlm_GetMuskCParamEd(MSM_GROUPS,MSM_NOSUBCLASS,MSM_NUM);
  for ( i=1; i<=tgroup; i++ ){
    for ( j=1; j<=MSM_TOTAL_POSITIVE; j++ ){
      if ( (Int2)Nlm_GetMuskCParamEd(j,MSM_FGROUP,MSM_NUM ) == i ) break;
    }
    if ( j > MSM_TOTAL_POSITIVE ) continue;
    groupAr[curInd++] = i;
    PopupItem ( featPopuP, 
               (CharPtr)Nlm_GetMuskCParamEd(MSM_GROUPS,i,MSM_STRING) );
  }
}

static void
/*FCN*/LoadFeatAr (
  void
){
  Int2 i,j,k;
  Int2 curGroup;
  Int2 nHideFeatures;

  curGroup = groupAr[GetValue ( featPopuP )-1];
  nHideFeatures = 0;
  for ( i=1; i<=MSM_TOTAL_POSITIVE; i++ ){
    j = (Int2)Nlm_GetMuskCParamEd(i,MSM_FGROUP,MSM_NUM );
    if ( j==curGroup ) {
      j = (Int2)Nlm_GetMuskCParamEd(i,MSM_FORDER,MSM_NUM );
      if ( j != 0 ){
        featAr[j-1] = i;
        nHideFeatures++;
      }
    }
  }
  k = nHideFeatures;
  for ( i=1; i<=MSM_TOTAL_POSITIVE; i++ ){
    j = (Int2)Nlm_GetMuskCParamEd(i,MSM_FGROUP,MSM_NUM );
    if ( j==curGroup ) {
      j = (Int2)Nlm_GetMuskCParamEd(i,MSM_FORDER,MSM_NUM );
      if ( j == 0 ) featAr[k++] = i;
    }
  }
  featAr[k] = 0;
  featAr[MSM_TOTAL_POSITIVE_R] = MSM_TOTAL_POSITIVE_R;
}

static void
/*FCN*/FillFetureList ( 
  Boolean needHide
){
  Int2 i;

  if ( needHide ){
    Hide( featList );
    Reset (featList);
  }
  i = 0;
  while ( featAr[i] != 0 ){
    ListItem ( featList, mSM_allfeatureNames[featAr[i]] );
    i++;
  }
  if ( i == 0 ) curFeatInd = -1;
  else {
    SetItemStatus ( featList, 1, TRUE );
    curFeatInd = 0;
    lastFeatInd = -1;
  }
  if ( needHide ) Show( featList );
}

static void
/*FCN*/OnFeaturePopUp(
  PopuP p
){
  LoadFeatAr ();
  FillFetureList(TRUE);
  UpdateDlgControls(TRUE,TRUE);
  UpdateTools();
}

static void 
/*FCN*/OnExtraList (
  LisT l
){
  curExtraInd = GetValue (l) - 1;
  UpdateDlgControls(TRUE,TRUE);
  UpdateTools();
}

static void 
/*FCN*/OnFeatList (
  LisT l
){
  Int2 i, ic, ilast;
  Int2 curClass;

  if ( curFeatInd == -1 ) return;
  ic = 0;
  for ( i=0; featAr[i] != 0; i++ ){
    if ( GetItemStatus(l, i+1) ) {
      ilast = i; ic++;
    }
  }
  if ( ic == 0 ) {
    if ( lastFeatInd == -1 ){
      SetItemStatus(l, curFeatInd+1, TRUE );
    } else {
      curFeatInd = lastFeatInd;
      lastFeatInd = -1;
      SetItemStatus(l, curFeatInd+1, TRUE );
    }
    Disable ( applayButton );
  } else if ( ic == 1 ) {
    if ( lastFeatInd != -1 ){
      lastFeatInd = -1;
      Disable ( applayButton );
    }
    curFeatInd = ilast;
  } else {
    if ( lastFeatInd == -1 ){
      lastFeatInd = curFeatInd;
      curClass = featAr[curFeatInd];
      for ( ic=0; inClipSchema[ic].subClass != MSM_NOSUBCLASS ;ic++ ){
        Nlm_SetMuskCParamEd( MSM_TOTAL_POSITIVE_R, inClipSchema[ic].subClass,
                             inClipSchema[ic].pType, 
                             Nlm_GetMuskCParamEd ( curClass, 
                             inClipSchema[ic].subClass,inClipSchema[ic].pType));
      }
      curFeatInd = MSM_TOTAL_POSITIVE_R;
      Enable ( applayButton );
    }
  }
  UpdateDlgControls(TRUE,TRUE);
  UpdateTools();
}

static void
/*FCN*/OnBorderCheck (
  ButtoN b
){
  Int2      curClass;
  BigScalar val;

  switch ( curDlg ){
    case SM_DLG_FEAT:
      curClass = featAr[curFeatInd];
      break;
    case SM_DLG_EXTRA:
      curClass = MSM_EXTRA_GENBANK - curExtraInd;
      break;
    default:
      curClass = MSM_SEQUENCE;
  }
  if ( GetStatus(b) ) val = (BigScalar)TRUE;
  else val = (BigScalar)FALSE;
  Nlm_SetMuskCParamEd(curClass,MSM_SEG_BORD,MSM_TRUEFALSE,val);
  UpdateDlgControls(TRUE,FALSE);
}

static void
/*FCN*/OnGapStyle (
  GrouP g
){
  Int2      curClass;
  BigScalar val;

  switch ( curDlg ){
    case SM_DLG_FEAT:
      curClass = featAr[curFeatInd];
      break;
    case SM_DLG_EXTRA:
      curClass = MSM_EXTRA_GENBANK - curExtraInd;
      break;
    default:
      return;
  }
  val = (BigScalar)(GetValue(g)-1);
  Nlm_SetMuskCParamEd(curClass,MSM_FGAP,MSM_STYLE,val);
  UpdateDlgControls(TRUE,FALSE);
}

static void
/*FCN*/OnLabelStyle (
  GrouP g
){
  Int2      curClass, curSubClass;
  BigScalar val;

  val = GetValue(g);
  switch ( curDlg ){
    case SM_DLG_FEAT:
      curClass = featAr[curFeatInd];
      curSubClass = MSM_FLABEL;
      break;
    case SM_DLG_EXTRA:
      curClass = MSM_EXTRA_GENBANK - curExtraInd;
      curSubClass = MSM_FLABEL;
      break;
    default:
      curClass = MSM_SEQUENCE;
      curSubClass = MSM_SLABEL;
  }
  if ( curDlg != SM_DLG_SEQ ){
    switch ( val ){
      case 2: val=MSM_LABEL_TYPE;    break;
      case 3: val=MSM_LABEL_CONTENT; break;
      case 4: val=MSM_LABEL_BOTH;    break;
      case 5: val=MSM_LABEL_SUMMARY; break;
      default: val=MSM_LABEL_NONE;
    }
  } else {
    switch ( val ){
      case 2: val=PRINTID_FASTA_LONG;       break;
      case 3: val=PRINTID_TEXTID_LOCUS;     break;
      case 4: val=PRINTID_TEXTID_ACCESSION; break;
      case 5: val=PRINTID_REPORT;           break;
      default: val=PRINTID_FASTA_SHORT;
    }
  }
  Nlm_SetMuskCParamEd(curClass,curSubClass,MSM_STYLE,val);
  UpdateDlgControls(TRUE,FALSE);
}

static void
/*FCN*/OnSegmentStyle (
  GrouP g
){

  Int2      curClass;
  BigScalar val;

  switch ( curDlg ){
    case SM_DLG_FEAT:
      curClass = featAr[curFeatInd];
      break;
    case SM_DLG_EXTRA:
      curClass = MSM_EXTRA_GENBANK - curExtraInd;
      break;
    default:
      curClass = MSM_SEQUENCE;
  }
  switch ( GetValue(segStyleGroup[curDlg]) ){
    case 4: val = MSM_SEG_LINE; break;
    case 2: val = MSM_SEG_SYM_RECT; break;
    case 5: val = MSM_SEG_SYM_DIAMOND; break;
    case 3: val = MSM_SEG_SYM_OVAL; break;
    case 6: val = MSM_SEG_SYM_TRIANGLE; break;
    default: val = MSM_SEG_BOX;
  }
  if ( curDlg == SM_DLG_SEQ ){
    if ( val != MSM_SEG_BOX ) val = MSM_SEG_LINE;
  } else {
    if ( GetStatus (trancCheckBox[curDlg]) ) val |= MSM_SEG_SHOWTRUNC;
    if ( GetStatus (orientCheckBox[curDlg]) ) val |= MSM_SEG_SHOWORIENT;
  }
  Nlm_SetMuskCParamEd(curClass,MSM_SEGMENT, MSM_STYLE, val);
  UpdateDlgControls(TRUE,FALSE);
  UpdateTools ();
}

static void
/*FCN*/OnHeight (
  PopuP p
){
  Int2      curClass;
  BigScalar val;

  switch ( curDlg ){
    case SM_DLG_FEAT:
      curClass = featAr[curFeatInd];
      break;
    case SM_DLG_EXTRA:
      curClass = MSM_EXTRA_GENBANK - curExtraInd;
      break;
    default:
      curClass = MSM_SEQUENCE;
  }
  val = (BigScalar)GetValue(p) * 2 + 2;
  Nlm_SetMuskCParamEd(curClass,MSM_SEGMENT, MSM_HEIGHT, val);
  UpdateDlgControls(TRUE,FALSE);
}

static void
/*FCN*/OnOrientation (
  ButtoN b
){
  OnSegmentStyle ( NULL );
}

static void
/*FCN*/OnTrancation (
  ButtoN b
){
  OnSegmentStyle ( NULL );
}

static void 
/*FCN*/OnChangePage ( 
  VoidPtr data, 
  Int2 newVal, 
  Int2 oldVal 
){
  SafeHide(hGroups[curDlg][oldVal]);
  curSubDlg[curDlg] = newVal;
  SafeShow(hGroups[curDlg][newVal]);
  UpdateTools();
} /* End of OnChangePage () */

static void
/*FCN*/OnWhole (
  ButtoN b,
  Int2   numF,
  Int2   numL
){
  Int2 i;

  if ( GetStatus (b) ){
    for ( i=numF; i<=numL; i++ ) SetStatus ( applayChecks[i], TRUE );
  } else {
    for ( i=numF; i<=numL; i++ ) SetStatus ( applayChecks[i], FALSE );
  }
}

static void
/*FCN*/OnWholeSeg (
  ButtoN b
){
  OnWhole ( b, 0, 5 );
}

static void
/*FCN*/OnWholeBorder (
  ButtoN b
){
  OnWhole ( b, 6, 8 );
}

static void
/*FCN*/OnWholeGap (
  ButtoN b
){
  OnWhole ( b, 9, 11 );
}

static void
/*FCN*/OnWholeLabel (
  ButtoN b
){
  OnWhole ( b, 12, 14 );
}

static void
/*FCN*/OnApplayApplay (
  ButtoN b
){
  Int4       curValue;
  Int4       curMusk;
  Int2       i, feat, j, chknum;
  Int2       curClass;

  applayStatus = 0;
  for ( feat = 0; featAr[feat] != 0; feat++ ){
    if ( GetItemStatus(featList, feat+1) ) {
      curClass = featAr[feat];
      for ( i=0; inClipSchema[i].subClass != MSM_NOSUBCLASS ;i++ ){
        for ( j=0; j<3; j++ ){
          chknum = paramToCheck[i].checkNum[j];
          if ( chknum == -1 ) break;
          curMusk = paramToCheck[i].musk[j];
          if ( curMusk == 0 ){
            if ( GetStatus ( applayChecks[chknum] ) ){
              Nlm_SetMuskCParamEd( curClass, inClipSchema[i].subClass,
                          inClipSchema[i].pType, 
                          Nlm_GetMuskCParamEd ( MSM_TOTAL_POSITIVE_R, 
                          inClipSchema[i].subClass,inClipSchema[i].pType));
            }
          } else {
            if ( GetStatus ( applayChecks[chknum] ) ){
              curValue = (Int4)Nlm_GetMuskCParamEd( curClass, 
                           inClipSchema[i].subClass, inClipSchema[i].pType );
              curValue &= ~curMusk; 
              curValue |= (Int4)Nlm_GetMuskCParamEd ( MSM_TOTAL_POSITIVE_R, 
                          inClipSchema[i].subClass,inClipSchema[i].pType );
              Nlm_SetMuskCParamEd( curClass, inClipSchema[i].subClass,
                          inClipSchema[i].pType, (BigScalar)curValue );
            }
          }
        }
      }
    }
  }
}

static void
/*FCN*/OnApplayCancel (
  ButtoN b
){
  applayStatus = 0;
}

static void
/*FCN*/OnApplay (
  ButtoN b
){
  WindoW w;
  GrouP  gTop, gBottom, gTmp;
 
  Nlm_DisableSM();
  w = MovableModalWindow (-40, -40,  -10, -10, "Parameters to set", NULL );
  gTop = HiddenGroup ( w, 2, 4, NULL );
  SetGroupSpacing ( gTop, 15, 10 );
  SetGroupMargins ( gTop, 15, 10 );
  CheckBox ( gTop, "", OnWholeSeg );
  gTmp = NormalGroup ( gTop, 3, 2, "Segment", systemFont, NULL );
  SetGroupMargins ( gTmp, 15, 10 );
  applayChecks[0] = CheckBox ( gTmp, "Style", NULL );
  applayChecks[1] = CheckBox ( gTmp, "Height", NULL );
  applayChecks[2] = CheckBox ( gTmp, "Orientation", NULL );
  applayChecks[3] = CheckBox ( gTmp, "Color", NULL );
  applayChecks[4] = CheckBox ( gTmp, "Shading", NULL );
  applayChecks[5] = CheckBox ( gTmp, "Trancation", NULL );
  CheckBox ( gTop, "", OnWholeBorder );
  gTmp = NormalGroup ( gTop, 3, 2, "Border", systemFont, NULL );
  SetGroupMargins ( gTmp, 15, 10 );
  applayChecks[6] = CheckBox ( gTmp, "Style", NULL );
  applayChecks[7] = CheckBox ( gTmp, "Color", NULL );
  applayChecks[8] = CheckBox ( gTmp, "Line type", NULL );
  CheckBox ( gTop, "", OnWholeGap );
  gTmp = NormalGroup ( gTop, 3, 2, "Gap", systemFont, NULL );
  SetGroupMargins ( gTmp, 15, 10 );
  applayChecks[9] = CheckBox ( gTmp, "Style", NULL );
  applayChecks[10] = CheckBox ( gTmp, "Color", NULL );
  applayChecks[11] = CheckBox ( gTmp, "Line type", NULL );
  CheckBox ( gTop, "", OnWholeLabel );
  gTmp = NormalGroup ( gTop, 3, 2, "Label", systemFont, NULL );
  SetGroupMargins ( gTmp, 15, 10 );
  applayChecks[12] = CheckBox ( gTmp, "Style", NULL );
  applayChecks[13] = CheckBox ( gTmp, "Color", NULL );
  applayChecks[14] = CheckBox ( gTmp, "Font", NULL );
  gBottom = HiddenGroup ( w, 2, 1, NULL );
  PushButton ( gBottom, " Set ", OnApplayApplay );
  PushButton ( gBottom, "Cancel", OnApplayCancel );
  AlignObjects ( ALIGN_CENTER, (HANDLE)gTop, (HANDLE)gBottom, (HANDLE)NULL );
  applayStatus = APPLAY_DLG_UP;
  Show (w);
  Select (w);
  Nlm_WaitForCondition ( applayStatus & APPLAY_DLG_UP);
  Nlm_ProcessAnEvent ();
  Hide (w);  
  Remove (w);
  Nlm_EnableSM();
}

static GrouP
/*FCN*/Nlm_CreateDlg (
  GrouP gTop
){
  GrouP gRet;
  GrouP gl, gr, gr1, gr2, gr3;
  GrouP gtmp, gtmp1;
  LisT  l;
 
  if ( curDlg != SM_DLG_SEQ ){
    gRet = HiddenGroup ( gTop, -2, 1, NULL );
    SetGroupSpacing ( gRet, 15, 1 );
  } else {
    gRet = HiddenGroup ( gTop, -1, 1, NULL );
  }
  if ( curDlg == SM_DLG_FEAT ){
    gl = gtmp = HiddenGroup ( gRet, 1, 5, NULL );
#ifdef WIN_MOTIF
    SetGroupSpacing(gl,5,10 );
#endif
    StaticPrompt ( gtmp, "Groups:", 0, 0, systemFont, 'l');
    featPopuP = PopupList ( gtmp, FALSE, OnFeaturePopUp );
    LoadFeatPopup();
    SetValue(featPopuP,1);
    StaticPrompt ( gtmp, "Features:", 0, 0, systemFont, 'l');
    featList = MultiList (gtmp, 10, 6, OnFeatList );
    LoadFeatAr();
    FillFetureList (FALSE);
    applayButton = PushButton ( gtmp, "Set for selection ...", OnApplay );
    Disable ( applayButton );
  } else if ( curDlg == SM_DLG_EXTRA ){
    gl = gtmp = HiddenGroup ( gRet, 1, 2, NULL );
#ifdef WIN_MOTIF
    SetGroupSpacing(gl,5,10 );
#endif
    StaticPrompt ( gtmp, "Extra:", 0, 0, systemFont, 'l');
    l = SingleList (gtmp, 10, 5, OnExtraList );
    ListItem ( l, "GENBANK" );
    ListItem ( l, "MEDLINE" );
    ListItem ( l, "BOTH" );
    SetValue ( l, 1 );
  }
  gr = HiddenGroup ( gRet, -1, 3, NULL );
  if ( curDlg != SM_DLG_SEQ ) {
    gr1 = HiddenGroup ( gr, 2,1, NULL );
    SetGroupSpacing( gr1, 10, 6 );
    exPanel[curDlg] = SimplePanel ( gr1, 250, 75, drawProc[curDlg] );
    gtmp = HiddenGroup ( gr1, 1,2, NULL );
    PushButton (gtmp,"Copy",OnClipCopy);
    pasteButton[curDlg] = PushButton (gtmp,"Paste",OnClipPaste);
    Disable ( pasteButton[curDlg] );
  } else {
    exPanel[curDlg] = SimplePanel ( gr, 400, 75, drawProc[curDlg] );
    gr1 = (GrouP)exPanel[curDlg];
  }
  gr2 = (GrouP)CreateFolderTabs ( gr, folderNamesF[curDlg], 0, 0, 0,
                                  SYSTEM_FOLDER_TAB, OnChangePage, NULL );
  gr3 = HiddenGroup ( gr, 0, 0, NULL );
  /* Border */
  hGroups[curDlg][1] = HiddenGroup ( gr3, -1, 1, NULL );
  borderCheckBox[curDlg] = CheckBox ( hGroups[curDlg][1], 
                           "Segment has the border", OnBorderCheck );
  Hide ( hGroups[curDlg][1] );
  if ( curDlg != SM_DLG_SEQ ) {
    /* Gap */
    hGroups[curDlg][2] = HiddenGroup ( gr3, -1, 1, NULL );
    gtmp = NormalGroup ( hGroups[curDlg][2], 3, 1, "Gap style", 
                         systemFont, OnGapStyle);
    gapStlyleGroup[curDlg] = gtmp;
    RadioButton ( gtmp, "None" );
    RadioButton ( gtmp, "Line" );
    RadioButton ( gtmp, "Angle" );
    Hide ( hGroups[curDlg][2] );
  }
  /* Label */
  if ( curDlg != SM_DLG_SEQ ) {    
    hGroups[curDlg][3] = HiddenGroup ( gr3, -1, 1, NULL );
    gtmp = NormalGroup ( hGroups[curDlg][3], 3, 2, "Label style", 
                         systemFont, OnLabelStyle);
    RadioButton ( gtmp, "None" );
    RadioButton ( gtmp, "Type" );
    RadioButton ( gtmp, "Content" );
    RadioButton ( gtmp, "Both" );
    RadioButton ( gtmp, "Summary" );
    Hide ( hGroups[curDlg][3] );
  } else {
    hGroups[curDlg][2] = HiddenGroup ( gr3, -1, 1, NULL );
    gtmp = NormalGroup ( hGroups[curDlg][2], 3, 2, "Label style", systemFont, 
                         OnLabelStyle);
    RadioButton ( gtmp, "FASTA_Short" );
    RadioButton ( gtmp, "FASTA_Long" );
    RadioButton ( gtmp, "TEXTID_Locus" );
    RadioButton ( gtmp, "TEXTID_Accession" );
    RadioButton ( gtmp, "TEXTID_Report" );
    Hide ( hGroups[curDlg][2] );
  }
  labelStyleGroup[curDlg] = gtmp;
  /*Segment */
  hGroups[curDlg][0] = HiddenGroup ( gr3, 2, 1, NULL );
  SetGroupSpacing ( hGroups[curDlg][0], 10, 5 );
  if ( curDlg != SM_DLG_SEQ ) {    
    gtmp = NormalGroup (hGroups[curDlg][0], 3,2, "Segment type",systemFont, 
                        OnSegmentStyle );
  } else {
    gtmp = NormalGroup (hGroups[curDlg][0], 1,2, "Segment type",systemFont, 
                        OnSegmentStyle );
  }
  segStyleGroup[curDlg] = gtmp;
  RadioButton ( gtmp, "Box" );
  if ( curDlg != SM_DLG_SEQ ) {    
    RadioButton ( gtmp, "Rect" );
    RadioButton ( gtmp, "Diamond" );
  }
  RadioButton ( gtmp, "Line" );
  if ( curDlg != SM_DLG_SEQ ) {    
    RadioButton ( gtmp, "Oval" );
    RadioButton ( gtmp, "Triangle" );
  }
  gtmp = HiddenGroup (hGroups[curDlg][0], 1,3, NULL);
  gtmp1 = HiddenGroup (gtmp, 2,1, NULL);
  StaticPrompt ( gtmp1, "Height:", 0, 0, systemFont, 'l');
  segHeightPopup[curDlg] = PopupList ( gtmp1, FALSE, OnHeight );
  PopupItem ( segHeightPopup[curDlg], "4" );
  PopupItem ( segHeightPopup[curDlg], "6" );
  PopupItem ( segHeightPopup[curDlg], "8" );
  PopupItem ( segHeightPopup[curDlg], "10" );
  PopupItem ( segHeightPopup[curDlg], "12" );
  PopupItem ( segHeightPopup[curDlg], "14" );
  PopupItem ( segHeightPopup[curDlg], "16" );
  PopupItem ( segHeightPopup[curDlg], "18" );
  PopupItem ( segHeightPopup[curDlg], "20" );
  if ( curDlg != SM_DLG_SEQ ) {
    orientCheckBox[curDlg] = CheckBox ( gtmp, "Orientation", 
                                        OnOrientation );
    trancCheckBox[curDlg] = CheckBox ( gtmp, "Truncation", 
                                        OnTrancation );
  }
  AlignObjects ( ALIGN_CENTER, (HANDLE)gr1, (HANDLE)gr2, (HANDLE)gr3, NULL );
  if ( curDlg != SM_DLG_SEQ ) {
#ifndef WIN_MOTIF
    AlignObjects ( ALIGN_MIDDLE, (HANDLE)gr, (HANDLE)gl, NULL );
#endif
    AlignObjects ( ALIGN_MIDDLE,  (HANDLE)hGroups[curDlg][0],
                   (HANDLE)hGroups[curDlg][1],(HANDLE)hGroups[curDlg][2],
                   (HANDLE)hGroups[curDlg][3], NULL );
    AlignObjects ( ALIGN_CENTER,  (HANDLE)hGroups[curDlg][0],
                   (HANDLE)hGroups[curDlg][1],(HANDLE)hGroups[curDlg][2],
                   (HANDLE)hGroups[curDlg][3], NULL );
  } else {
    AlignObjects ( ALIGN_MIDDLE,  (HANDLE)hGroups[curDlg][0],
                   (HANDLE)hGroups[curDlg][1],(HANDLE)hGroups[curDlg][2],
                   NULL );
    AlignObjects ( ALIGN_CENTER,  (HANDLE)hGroups[curDlg][0],
                   (HANDLE)hGroups[curDlg][1],(HANDLE)hGroups[curDlg][2],
                   NULL );
  }
  curSubDlg[curDlg] = 0;
  UpdateDlgControls (FALSE,TRUE);
  return gRet;
} /* End of CreateDlgFeatures () */


static void
/*FCN*/Nlm_ToolDlg ( 
  Int2 tools
){
  Int2      curClass, curSubClass;
  BigScalar val;
  FonT      fnt;
  Int2      fsize;

  switch ( curDlg ){
    case SM_DLG_FEAT:
      if ( curFeatInd == -1 ) return;
      curClass = featAr[curFeatInd];
      break;
    case SM_DLG_EXTRA:
      if ( curExtraInd == -1 ) return;
      curClass = MSM_EXTRA_GENBANK - curExtraInd;
      break;
    default:
      curClass = MSM_SEQUENCE;
  }
  if ( curSubDlg[curDlg] == 0 ){
    if ( tools == SM_TOOLS_COLOR ){
      val = Nlm_GetToolValueSM ( SM_TOOLS_COLOR );
      Nlm_SetMuskCParamEd(curClass,MSM_SEGMENT,MSM_COLOR,val);
    } else if ( tools == SM_TOOLS_SHADDING ){
      val = Nlm_GetToolValueSM ( SM_TOOLS_SHADDING );
      Nlm_SetMuskCParamEd(curClass,MSM_SEGMENT,MSM_SHADING,val);
    } else if ( tools == SM_TOOLS_LTYPE ){
      val = Nlm_GetToolValueSM (SM_TOOLS_LTYPE);
      Nlm_SetMuskCParamEd(curClass,MSM_SEGMENT,MSM_LTYPE, val&0xFF );
      Nlm_SetMuskCParamEd(curClass,MSM_SEGMENT,MSM_PENWIDTH, (val>>8)&0xFF );
    }
  } else if ( curSubDlg[curDlg] == 1 ){
    if ( tools == SM_TOOLS_COLOR ){
      val = Nlm_GetToolValueSM ( SM_TOOLS_COLOR );
      Nlm_SetMuskCParamEd(curClass,MSM_SEG_BORD,MSM_COLOR,val);
    } else if ( tools == SM_TOOLS_LTYPE ){
      val = Nlm_GetToolValueSM (SM_TOOLS_LTYPE);
      Nlm_SetMuskCParamEd(curClass,MSM_SEG_BORD,MSM_LTYPE, val&0xFF );
      Nlm_SetMuskCParamEd(curClass,MSM_SEG_BORD,MSM_PENWIDTH, (val>>8)&0xFF );
    }
  } else if ( (curSubDlg[curDlg] == 2)&&(curDlg != SM_DLG_SEQ) ){
    if ( tools == SM_TOOLS_COLOR ){
      val = Nlm_GetToolValueSM ( SM_TOOLS_COLOR );
      Nlm_SetMuskCParamEd(curClass,MSM_FGAP,MSM_COLOR,val);
    } else if ( tools == SM_TOOLS_LTYPE ){
      val = Nlm_GetToolValueSM (SM_TOOLS_LTYPE);
      Nlm_SetMuskCParamEd(curClass,MSM_FGAP,MSM_LTYPE, val&0xFF );
      Nlm_SetMuskCParamEd(curClass,MSM_FGAP,MSM_PENWIDTH, (val>>8)&0xFF );
    }
  } else {
    if ( curDlg == SM_DLG_SEQ ){
      curSubClass = MSM_SLABEL;
    } else {
      curSubClass = MSM_FLABEL;
    }
    if ( tools == SM_TOOLS_COLOR ){
      val = Nlm_GetToolValueSM ( SM_TOOLS_COLOR );
      Nlm_SetMuskCParamEd(curClass,curSubClass,MSM_COLOR,val);
    } else if ( tools == SM_TOOLS_FONT ){
      val = Nlm_GetToolValueSM ( SM_TOOLS_FONT );
      fnt = Nlm_LoadMuskFont ( (Nlm_FontSpecPtr)val, 0, &fsize );
      Nlm_SetMuskCParamEd(curClass,curSubClass,MSM_FONT,(BigScalar)fnt);
      Nlm_SetMuskCParamEd(curClass,curSubClass,MSM_HEIGHT,fsize);
    }
  }
  UpdateDlgControls ( TRUE, TRUE );
}

static void  Nlm_SetTDef (
  void
){
  if ( (curDlg == SM_DLG_FEAT) && Msk_OrderWasModified() ){
    Hide ( featPopuP );
    Reset (featPopuP );
    LoadFeatPopup();
    SetValue(featPopuP,1);
    Show ( featPopuP );
    LoadFeatAr();
    FillFetureList (TRUE);
    UpdateDlgControls (TRUE,TRUE);
  }
  UpdateTools();
}

/**************************************************************************/
/* GLOBAL FUNCTIONS */
/**************************************************************************/

GrouP
/*FCN*/Nlm_CreateDlgFeatures (
  GrouP gTop
){
  curDlg = SM_DLG_FEAT;
  return Nlm_CreateDlg ( gTop );
} /* End of CreateDlgFeatures () */

void
/*FCN*/Nlm_ToolDlgFeatures ( 
  Int2 tools
){
  curDlg = SM_DLG_FEAT;
  Nlm_ToolDlg ( tools );
}

void  Nlm_SetTDefFeatures (
  void
){
  curDlg = SM_DLG_FEAT;
  Nlm_SetTDef ();
}

GrouP
/*FCN*/Nlm_CreateDlgSeq (
  GrouP gTop
){
  curDlg = SM_DLG_SEQ;
  return Nlm_CreateDlg ( gTop );
} /* End of CreateDlgFeatures () */

void
/*FCN*/Nlm_ToolDlgSeq ( 
  Int2 tools
){
  curDlg = SM_DLG_SEQ;
  Nlm_ToolDlg ( tools );
}

void  Nlm_SetTDefSeq (
  void
){
  curDlg = SM_DLG_SEQ;
  Nlm_SetTDef ();
}

GrouP
/*FCN*/Nlm_CreateDlgExtra (
  GrouP gTop
){
  curDlg = SM_DLG_EXTRA;
  return Nlm_CreateDlg ( gTop );
} /* End of CreateDlgFeatures () */

void
/*FCN*/Nlm_ToolDlgExtra ( 
  Int2 tools
){
  curDlg = SM_DLG_EXTRA;
  Nlm_ToolDlg ( tools );
}

void  Nlm_SetTDefExtra (
  void
){
  curDlg = SM_DLG_EXTRA;
  Nlm_SetTDef ();
}

/* END */
