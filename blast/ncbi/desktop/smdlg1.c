/*   smdlg1.c
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
* File Name:  smdlg1.c
*
* Author:  Alex Smirnov
*
* Version Creation Date:   8/10/95
*
* $Revision: 6.0 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: smdlg1.c,v $
* Revision 6.0  1997/08/25 18:24:48  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/04/03 20:00:43  vakatov
* Added "Max. number of alignments:" input textfield and linked that
* to the StyleMgr MSM_ALIGNMENT | MSM_NOSUBCLASS | MSM_SCALE parameter
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
#define SM_PANEL_H    20
#define SM_PANEL_W    300
#define LINE_PANEL_H  16
#define LINE_PANEL_W  250

/**************************************************************************/
/* TYPEDEFS */
/**************************************************************************/

/**************************************************************************/
/* STATIC VARIABLE */
/**************************************************************************/
static PaneL  linePanel;
static ButtoN bStackGene;
static ButtoN bHistory;
static ButtoN bSegments;

static TexT   spaceText;
static TexT   lableLenText;
static TexT   endScale;
static TexT   maxNAlign;

static PaneL  exPanel;
static Int2   activColor = 0;

/**************************************************************************/
/* STATIC FUNCTION */
/**************************************************************************/
static void
/*FCN*/TopStyleCheck (
  ButtoN b
){
  BigScalar val = 0;

  if ( GetStatus (bStackGene) ) val |= MSM_STACKGENE;
  if ( GetStatus (bHistory) ) val |= MSM_HISTORY;
  if ( GetStatus (bSegments) ) val |= MSM_SEGMENTS;
  Nlm_SetMuskCParamEd( MSM_TOPSTYLE,MSM_NOSUBCLASS,MSM_STYLE,val );
}

static void
/*FCN*/SetIntDialogTitle (
  TexT t 
){
  Nlm_Char cValue[64];
  long     iValue;
  Int2     pClass, pSubclass, pType;

  if (t == spaceText) {
    pClass    = MSM_TOPSTYLE;
    pSubclass = MSM_SPACE;
    pType     = MSM_HEIGHT;
  }
  else if (t == lableLenText) {
    pClass    = MSM_TOPSTYLE;
    pSubclass = MSM_LABEL;
    pType     = MSM_NUM;
  }
  else if (t == endScale) {
    pClass    = MSM_TOPSTYLE;
    pSubclass = MSM_ENDS;
    pType     = MSM_SCALE;
  }
  else if (t == maxNAlign) {
    pClass    = MSM_ALIGNMENT;
    pSubclass = MSM_NOSUBCLASS;
    pType     = MSM_SCALE;
  }
  else
    {
      ASSERT ( FALSE );
      return;
    }

  iValue = (long)Nlm_GetMuskCParamEd(pClass, pSubclass, pType);
  sprintf(cValue, "%ld", iValue);
  SetTitle(t, cValue);
}


static void 
/*FCN*/TextDialog ( 
  TexT t 
){
  Nlm_Char cValue[64];
  Nlm_Char PNTR cValuePtr;
  long     iValue = -1;
  Int4     cMax;
  Int2     pClass, pSubclass, pType;
  Int2     update = 0;

  if (t == spaceText) {
    cMax      = 50; 
    pClass    = MSM_TOPSTYLE;
    pSubclass = MSM_SPACE;
    pType     = MSM_HEIGHT;
  }
  else if (t == lableLenText) {
    cMax      = 100; 
    pClass    = MSM_TOPSTYLE;
    pSubclass = MSM_LABEL;
    pType     = MSM_NUM;
  }
  else if (t == endScale) {
    cMax      = 10000; 
    pClass    = MSM_TOPSTYLE;
    pSubclass = MSM_ENDS;
    pType     = MSM_SCALE;
  }
  else if (t == maxNAlign) {
    cMax      = INT4_MAX; 
    pClass    = MSM_ALIGNMENT;
    pSubclass = MSM_NOSUBCLASS;
    pType     = MSM_SCALE;
  }
  else
    {
      ASSERT ( FALSE );
      return;
    }

  Nlm_GetTitle( t, cValue, 64 );
  cValue[63] = '\0';
  for ( cValuePtr = cValue; IS_DIGIT(*cValuePtr); cValuePtr++ );
  if ( *cValuePtr != '\0' ) {
    *cValuePtr = 0;
    update = 1;
  }
  sscanf ( cValue, "%ld", &iValue );
  if ( (iValue < 0) || (cValue[0]==0) ) {
    iValue = 0;
    update = 2;
  }
  if ( (cValue[0] == '0') && (cValue[1] != 0) ){
    iValue = 0;
    update = 1;
  }
  if ( iValue > cMax ) {
    iValue = cMax ;
    update = 1;
  } 
  if ( update == 2 ){ 
    Nlm_SetTitle ( t, "" );
  } else if ( update ) {
    sprintf ( cValue, "%ld", iValue );
    SetTitle ( t, cValue );
  }

  Nlm_SetMuskCParamEd(pClass, pSubclass, pType, (BigScalar)iValue);
}


static void 
/*FCN*/AlignmentStyle (
 GrouP g
){
  BigScalar val;

  switch ( GetValue (g) ){
    case 1: val = MSM_SEQHIST; break;
    case 2: val = MSM_MPAIR; break;
    case 3: val = MSM_MDIM; break;
    default: val = MSM_FIXED;
  }
  Nlm_SetMuskCParamEd( MSM_ALIGNMENT,MSM_NOSUBCLASS,MSM_STYLE,val );
}

static void 
/*FCN*/LablesOn (
 GrouP g
){
  BigScalar val;

  switch ( GetValue (g) ){
    case 1: val = MSM_LABEL_TOP; break;
    case 2: val = MSM_LABEL_BOTTOM; break;
    case 3: val = MSM_LABEL_LEFT; break;
    default: val = MSM_LABEL_RIGHT;
  }
  Nlm_SetMuskCParamEd( MSM_TOPSTYLE,MSM_LABEL,MSM_STYLE,val);
}

static void 
/*FCN*/LinePanelDraw ( 
  PaneL p
){
  Int4      val;
  RecT      rp;
  PoinT     pt1, pt2;

  ObjectRect ( p, &rp );
  pt1.x = rp.left;
  pt2.x = rp.right;
  pt1.y = pt2.y = (rp.top+rp.bottom)/2;
  val = (Int4)Nlm_GetMuskCParamEd(MSM_ALIGNMENT,MSM_ALINE,MSM_PENWIDTH);
  WidePen ( (Int1)val );
  val = (Int4)Nlm_GetMuskCParamEd(MSM_ALIGNMENT,MSM_ALINE,MSM_LTYPE);
  switch ( val ){
    case DOTTED_LINE:
      Dotted(); break;
    case DASHED_LINE:
      Dashed(); break;
    default:
      Solid();
  }
  val = (Int4)Nlm_GetMuskCParamEd(MSM_ALIGNMENT,MSM_ALINE,MSM_COLOR);
  SelectColor ( (Uint1)((val>>16)&0xFF), (Uint1)((val>>8)&0xFF),
                (Uint1)(val&0xFF) );
  DrawLine ( pt1, pt2 );
}

static void 
/*FCN*/ExPanelDrawCursor ( 
  void
){
  RecT rp;
  RecT rDraw;
  Int4 numColor;
  Int2 w;

  numColor = (Int4)Nlm_GetMuskCParamEd(MSM_CCOLOR,MSM_NOSUBCLASS,MSM_NUM);
  ObjectRect ( exPanel, &rp );
  w = (rp.right - rp.left)/numColor;
  LoadRect ( &rDraw, rp.left + w*activColor, rp.top,
                       rp.left + w*(activColor+1), rp.bottom );
  Solid();
  Black();
  WidePen(1);
  InvertMode();
  FrameRect ( &rDraw );
  CopyMode();
}

static void 
/*FCN*/ExPanelDraw ( 
  PaneL p
){
  Int4 val;
  RecT rp;
  RecT rDraw;
  Int4 numColor;
  Int2 i, w;

  numColor = (Int4)Nlm_GetMuskCParamEd(MSM_CCOLOR,MSM_NOSUBCLASS,MSM_NUM);
  ObjectRect ( p, &rp );
  w = (rp.right - rp.left)/numColor;
  Solid();
  for ( i=0; i<numColor; i++ ){
    LoadRect ( &rDraw, rp.left + w*i + 2, rp.top + 4,
                       rp.left + w*(i+1) - 2, rp.bottom - 4 );
    val = (Int4)Nlm_GetMuskCParamEd(MSM_CCOLOR,i+1,MSM_COLOR);
    SelectColor ( (Uint1)((val>>16)&0xFF), (Uint1)((val>>8)&0xFF),
                  (Uint1)(val&0xFF) );
    PaintRect ( &rDraw );
  }
  ExPanelDrawCursor();
}

static void 
/*FCN*/ExPanelClick (
  PaneL p, 
  Nlm_PoinT pt
){
  Int4      numColor;
  Nlm_RecT  rp;
  Int2      i, w;

  numColor = (Int4)Nlm_GetMuskCParamEd(MSM_CCOLOR,MSM_NOSUBCLASS,MSM_NUM);
  ObjectRect (p, &rp);
  w = (rp.right - rp.left)/numColor;
  i = (pt.x - rp.left)/w;
  ExPanelDrawCursor();
  activColor = i;
  ExPanelDrawCursor();
  Nlm_SetToolDefaultSM  ( SM_TOOLS_COLOR, 
                          Nlm_GetMuskCParamEd(MSM_CCOLOR,i+1,MSM_COLOR));
  Nlm_ShowToolsSM (SM_SHTOOLS_COLOR);
}

static void 
/*FCN*/OnPeriod(
  GrouP g
){
  RecT rp;
  Int4 numColor;

  numColor = GetValue (g) + 1;
  Nlm_SetMuskCParamEd(MSM_CCOLOR,MSM_NOSUBCLASS,MSM_NUM,(BigScalar)numColor);
  activColor = 0;
  ObjectRect ( exPanel, &rp );
  Select (exPanel);  /* needed for Motif version - JK */
  ResetClip();
  InvalRect ( &rp );
  Nlm_SetToolDefaultSM  ( SM_TOOLS_COLOR, 
                          Nlm_GetMuskCParamEd(MSM_CCOLOR,1,MSM_COLOR));
  Nlm_ShowToolsSM (SM_SHTOOLS_COLOR);
}

/**************************************************************************/
/* GLOBAL FUNCTIONS */
/**************************************************************************/
GrouP
/*FCN*/Nlm_CreateDlgCommon ( 
  GrouP gTop
){
  Int4     val;
  GrouP    g, g1, g2, g3, g4 ;
  PrompT   prompt;

  g = HiddenGroup ( gTop, -1, 0, NULL);
  SetGroupSpacing ( g, 10, 15 );
  g1 = HiddenGroup ( g, 2, 1, NULL );
  SetGroupSpacing ( g1, 10, 1 );
  g2 = HiddenGroup ( g1, 1, 2, NULL );

  g3 = NormalGroup ( g2, 3, 1, "Show", systemFont, NULL );
  val = (Int4)Nlm_GetMuskCParamEd( MSM_TOPSTYLE,MSM_NOSUBCLASS,MSM_STYLE );
  bStackGene = CheckBox ( g3, "Stack Gene", TopStyleCheck );
  if ( val & MSM_STACKGENE ) SetStatus ( bStackGene, TRUE );
  bHistory = CheckBox ( g3, "History", TopStyleCheck );
  if ( val & MSM_HISTORY ) SetStatus ( bHistory, TRUE );
  bSegments = CheckBox ( g3, "Segments", TopStyleCheck );
  if ( val & MSM_SEGMENTS ) SetStatus ( bSegments, TRUE );

  g3 = HiddenGroup(g2, 2, 3, NULL);
  SetGroupMargins(g3, 1, 10);
  prompt = StaticPrompt(g3, "Space between lines : ", 0, 0, systemFont, 'l');
  spaceText = DialogText(g3, "20", 5, TextDialog);
  AlignObjects(ALIGN_MIDDLE, (HANDLE)prompt, (HANDLE)spaceText, NULL);
  SetIntDialogTitle( spaceText );

  prompt = StaticPrompt(g3, "Maximum label length : ", 0, 0, systemFont, 'l');
  lableLenText = DialogText(g3, "20", 5, TextDialog);
  AlignObjects(ALIGN_MIDDLE, (HANDLE)prompt, (HANDLE)lableLenText, NULL);
  SetIntDialogTitle( lableLenText );

  prompt = StaticPrompt(g3, "Show ends if scale > : ", 0, 0, systemFont, 'l');
  endScale = DialogText(g3, "100", 5, TextDialog);
  AlignObjects(ALIGN_MIDDLE, (HANDLE)prompt, (HANDLE)endScale, NULL);
  SetIntDialogTitle( endScale );

  prompt = StaticPrompt(g3, "Max. N-of-Alignments : ", 0, 0, systemFont, 'l');
  val = (Int4)Nlm_GetMuskCParamEd(MSM_ALIGNMENT, MSM_NOSUBCLASS, MSM_SCALE);
  maxNAlign = DialogText(g3, " ", 5, TextDialog);
  AlignObjects(ALIGN_MIDDLE, (HANDLE)prompt, (HANDLE)maxNAlign, NULL);
  SetIntDialogTitle( maxNAlign );

  g2 = HiddenGroup ( g1, 2, 1, NULL );
  SetGroupSpacing ( g2, 10, 1 );
  g3 = NormalGroup ( g2, 1, 4, "Alignment style", systemFont, AlignmentStyle );
  RadioButton ( g3, "Seq History" );
  RadioButton ( g3, "MPair" );
  RadioButton ( g3, "MDim" );
  RadioButton ( g3, "Fixed" );
  val = (Int4)Nlm_GetMuskCParamEd( MSM_ALIGNMENT, MSM_NOSUBCLASS,MSM_STYLE );
  switch ( val ) {
    case MSM_SEQHIST:
      SetValue ( g3, 1 ); break;
    case MSM_MPAIR:
      SetValue ( g3, 2 ); break;
    case MSM_MDIM:
      SetValue ( g3, 3 ); break;
    default:
      SetValue ( g3, 4 );
  }
  g4 = NormalGroup ( g2, 1, 4, "Labels on", systemFont, LablesOn );
  RadioButton ( g4, "Top" );
  RadioButton ( g4, "Bottom" );
  RadioButton ( g4, "Left" );
  RadioButton ( g4, "Right" );
  val = (Int4)Nlm_GetMuskCParamEd( MSM_TOPSTYLE,MSM_LABEL,MSM_STYLE );
  switch ( val ) {
    case MSM_LABEL_TOP:
      SetValue ( g4, 1 ); break;
    case MSM_LABEL_BOTTOM:
      SetValue ( g4, 2 ); break;
    case MSM_LABEL_LEFT:
      SetValue ( g4, 3 ); break;
    default:
      SetValue ( g4, 4 );
  }
  AlignObjects ( ALIGN_MIDDLE, (HANDLE)g1, (HANDLE)g3, (HANDLE)g4, NULL );
  g2 = HiddenGroup ( g, -2, 1, NULL );
  StaticPrompt ( g2, "Alignment lines:   ", 0, 0, systemFont, 'l');
  linePanel = SimplePanel ( g2, LINE_PANEL_W, LINE_PANEL_H, LinePanelDraw );
  AlignObjects ( ALIGN_CENTER, (HANDLE)g1, (HANDLE)g2, NULL );
  return g;
}

void
/*FCN*/Nlm_ToolDlgCommon ( 
  Int2 tools
){
  BigScalar val;
  RecT      rp;

  Select (linePanel);  /* needed for Motif version - JK */
  ResetClip();
  switch ( tools ){
    case SM_TOOLS_COLOR:
      val = Nlm_GetToolValueSM (SM_TOOLS_COLOR);
      Nlm_SetMuskCParamEd(MSM_ALIGNMENT,MSM_ALINE,MSM_COLOR,val);
      break;
    case SM_TOOLS_LTYPE:
      val = Nlm_GetToolValueSM (SM_TOOLS_LTYPE);
      Nlm_SetMuskCParamEd(MSM_ALIGNMENT,MSM_ALINE,MSM_LTYPE, val&0xFF );
      Nlm_SetMuskCParamEd(MSM_ALIGNMENT,MSM_ALINE,MSM_PENWIDTH, (val>>8)&0xFF );
  }
  ObjectRect ( linePanel, &rp );
  InvalRect ( &rp );
}

void  Nlm_SetTDefCommon (
  void
){
  Nlm_SetToolDefaultSM  ( SM_TOOLS_COLOR, 
    Nlm_GetMuskCParamEd(MSM_ALIGNMENT,MSM_ALINE,MSM_COLOR));
  Nlm_SetToolDefaultSM  ( SM_TOOLS_LTYPE, 
    (Nlm_GetMuskCParamEd(MSM_ALIGNMENT,MSM_ALINE,MSM_PENWIDTH) << 8) |
    Nlm_GetMuskCParamEd(MSM_ALIGNMENT,MSM_ALINE,MSM_LTYPE) );
  Nlm_ShowToolsSM ( SM_SHTOOLS_COLOR | SM_SHTOOLS_LTYPE );
}

GrouP 
/*FCN*/Nlm_CreateDlgCColor ( 
  GrouP gTop
){
  Int4     numColor;
  GrouP    g, g1, g2;

  g = HiddenGroup ( gTop, -2,1, NULL);
  SetGroupSpacing ( g, 10,1 );
  SetGroupMargins  ( g, 20,10 );
  numColor = (Int4)Nlm_GetMuskCParamEd(MSM_CCOLOR,MSM_NOSUBCLASS,MSM_NUM);
  if ( (numColor <= 1)||(numColor >6) ) {
    numColor = 2;
    Nlm_SetMuskCParamEd(MSM_CCOLOR,MSM_NOSUBCLASS,MSM_NUM,(BigScalar)numColor);
  }
  g2 = HiddenGroup ( g, 1,2, NULL);
  StaticPrompt ( g2, "Cycle colors: ", 0, 0, systemFont, 'l');
  exPanel = SimplePanel ( g2, SM_PANEL_W, SM_PANEL_H, ExPanelDraw );
  SetPanelClick ( exPanel, ExPanelClick, NULL, NULL, NULL );
  g1 = NormalGroup ( g, 1, 5, "Period", systemFont, OnPeriod );
  SetGroupMargins ( g1, 10, 1 );
  RadioButton ( g1,"2" );
  RadioButton ( g1,"3" );
  RadioButton ( g1,"4" );
  RadioButton ( g1,"5" );
  RadioButton ( g1,"6" );
  SetValue ( g1, numColor-1 );
  AlignObjects ( ALIGN_MIDDLE, (HANDLE)g1, (HANDLE)g2, NULL );
  return g;
}

void
/*FCN*/Nlm_ToolDlgCColor ( 
  Int2 tools
){
  Int4   val;
  Int4   numColor;
  WindoW tmpPort;
  RecT   rp;
  RecT   rDraw;
  Int2   w;

  ResetClip();
  tmpPort = SavePort (exPanel);
  Select ( exPanel);
  numColor = (Int4)Nlm_GetMuskCParamEd(MSM_CCOLOR,MSM_NOSUBCLASS,MSM_NUM);
  ObjectRect ( exPanel, &rp );
  w = (rp.right - rp.left)/numColor;
  Solid();
  LoadRect ( &rDraw, rp.left + w*activColor + 2, rp.top + 4,
             rp.left + w*(activColor+1) - 2, rp.bottom - 4 );
  val = (Int4)Nlm_GetToolValueSM (SM_TOOLS_COLOR);
  Nlm_SetMuskCParamEd(MSM_CCOLOR,activColor+1,MSM_COLOR,(BigScalar)val);
  SelectColor ( (Uint1)((val>>16)&0xFF), (Uint1)((val>>8)&0xFF),
                (Uint1)(val&0xFF) );
  PaintRect ( &rDraw );
  RestorePort ( tmpPort );
}

void  Nlm_SetTDefCColor (
  void
){
  Nlm_SetToolDefaultSM  ( SM_TOOLS_COLOR, 
    Nlm_GetMuskCParamEd(MSM_CCOLOR,activColor+1,MSM_COLOR));
  Nlm_ShowToolsSM ( SM_SHTOOLS_COLOR );
}

/* END */
