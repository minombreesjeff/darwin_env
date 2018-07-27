/*   smtools.c
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
* File Name:  smtools.c
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
#define INPANEL_COLOR    0
#define INPANEL_SHADDING 1
#define INPANEL_LTYPE    2
#define INIT_FONTSET     6

/**************************************************************************/
/* TYPEDEFS */
/**************************************************************************/
typedef struct {
  BigScalar val;
  GrouP     g;
  Boolean   vis;
} Nlm_ToolNode;

typedef void (*Nlm_InPanelDraw)   (Int2 x, Int2 y, Int2 w, Int2 h, 
                                   Int2 i, Int2 j);
typedef void (*Nlm_InPanelChange) (void);

typedef struct {
  PaneL   p;
  Int2    width;
  Int2    height;
  Int2    x, y;
  Int2    curx, cury;
  Nlm_InPanelDraw   draw;
  Nlm_InPanelChange change;
} Nlm_InterPanel, PNTR Nlm_InterPanelPtr;

typedef struct {
  Nlm_Uint1  red;
  Nlm_Uint1  green;
  Nlm_Uint1  blue;
} NLMINTERNALCOLOR;

typedef struct s_AddFontList {
  struct s_AddFontList PNTR next;
  Nlm_FontSpec              fsp;
} Nlm_AddFontList, PNTR Nlm_AddFontListPtr;

static void colorDraw    (Int2 x,Int2 y,Int2 w,Int2 h,Int2 i,Int2 j);
static void shaddingDraw (Int2 x,Int2 y,Int2 w,Int2 h,Int2 i,Int2 j);
static void lTypeDraw    (Int2 x,Int2 y,Int2 w,Int2 h,Int2 i,Int2 j);
static void colorChg     (void);
static void shaddingChg  (void);
static void lTypeChg     (void);

/**************************************************************************/
/* STATIC VARIABLE */
/**************************************************************************/
static Nlm_ToolNode      tools[SM_TOOLS_TOTAL];
static Nlm_SmToolsProc   curProc = NULL;
static LisT              fontList = NULL;

#ifdef WIN_MAC
static Nlm_FontSpec       fontNames[INIT_FONTSET] = {
{"Geneva", 9, STYLE_REGULAR, CHARSET_ANSI, PITCH_VARIABLE, FAMILY_SWISS},
{"Times", 9, STYLE_REGULAR, CHARSET_ANSI, PITCH_VARIABLE, FAMILY_ROMAN},
{"Monaco", 9, STYLE_REGULAR, CHARSET_ANSI, PITCH_FIXED, FAMILY_MODERN},
{"Geneva", 12, STYLE_REGULAR, CHARSET_ANSI, PITCH_VARIABLE, FAMILY_SWISS},
{"Times", 12, STYLE_REGULAR, CHARSET_ANSI, PITCH_VARIABLE, FAMILY_ROMAN},
{"Monaco", 12, STYLE_REGULAR, CHARSET_ANSI, PITCH_FIXED, FAMILY_MODERN} };
#endif
#ifdef WIN_MSWIN
static Nlm_FontSpec       fontNames[INIT_FONTSET] = {
{"Arial", 9, STYLE_REGULAR, CHARSET_ANSI, PITCH_VARIABLE, FAMILY_SWISS},
{"Times", 9, STYLE_REGULAR, CHARSET_ANSI, PITCH_VARIABLE, FAMILY_ROMAN},
{"Courier New", 9, STYLE_REGULAR, CHARSET_ANSI, PITCH_FIXED, FAMILY_MODERN},
{"Arial", 12, STYLE_REGULAR, CHARSET_ANSI, PITCH_VARIABLE, FAMILY_SWISS},
{"Times", 12, STYLE_REGULAR, CHARSET_ANSI, PITCH_VARIABLE, FAMILY_ROMAN},
{"Courier New", 12, STYLE_REGULAR, CHARSET_ANSI, PITCH_FIXED, FAMILY_MODERN}};
#endif
#ifdef WIN_MOTIF
static Nlm_FontSpec       fontNames[INIT_FONTSET] = {
{"helvetica", 12, STYLE_REGULAR, CHARSET_ANSI, PITCH_VARIABLE, FAMILY_SWISS},
{"times", 12, STYLE_REGULAR, CHARSET_ANSI, PITCH_VARIABLE, FAMILY_ROMAN},
{"courier", 12, STYLE_REGULAR, CHARSET_ANSI, PITCH_FIXED, FAMILY_MODERN},
{"helvetica", 14, STYLE_REGULAR, CHARSET_ANSI, PITCH_VARIABLE, FAMILY_SWISS},
{"times", 14, STYLE_REGULAR, CHARSET_ANSI, PITCH_VARIABLE, FAMILY_ROMAN},
{"courier", 14, STYLE_REGULAR, CHARSET_ANSI, PITCH_FIXED, FAMILY_MODERN}};
#endif

static Nlm_AddFontListPtr addFontList = NULL;

/* Basic colors */
static NLMINTERNALCOLOR theBasicColors[16] = {
{128,128,128},{255,255,255},{255,0  ,0  },{0  ,255,0  },
{0  ,0  ,255},{255,255,0  },{255,0  ,255},{0  ,255,255},
{0  ,0  ,0  },{192,192,192},{128,0  ,0  },{0  ,128,0  },
{0  ,0  ,128},{128,128,0  },{128,0  ,128},{0  ,128,128} };

static Nlm_InterPanel interPanels [] = {
 {(PaneL)0, 16, 12, 8, 2, 0, 0, colorDraw, colorChg },
 {(PaneL)0, 20, 24, 2, 2, 0, 0, shaddingDraw, shaddingChg },
 {(PaneL)0, 40, 15, 2, 3, 0, 0, lTypeDraw, lTypeChg }
};

/**************************************************************************/
/* STATIC FUNCTION */
/**************************************************************************/
static void
/*FCN*/colorDraw (
  Int2 x, 
  Int2 y, 
  Int2 w, 
  Int2 h, 
  Int2 i, 
  Int2 j
){
  RecT  rdraw;

  LoadRect ( &rdraw, x, y, x+w, y+h );
  SelectColor ( theBasicColors[j*8+i].red,
                theBasicColors[j*8+i].green,
                theBasicColors[j*8+i].blue );
  PaintRect ( &rdraw );
  Black();
  FrameRect ( &rdraw );
}

static void 
/*FCN*/colorChg (
  void
){
  Int2 i;

  i = interPanels[INPANEL_COLOR].cury * 8 + 
      interPanels[INPANEL_COLOR].curx;
  tools[SM_TOOLS_COLOR].val = 
      ((BigScalar)theBasicColors[i].red << 16) |
      ((BigScalar)theBasicColors[i].green << 8) |
      ((BigScalar)theBasicColors[i].blue);
  if ( curProc != NULL ) curProc (SM_TOOLS_COLOR); 
}

static void
/*FCN*/shaddingDraw (
  Int2 x, 
  Int2 y, 
  Int2 w, 
  Int2 h, 
  Int2 i, 
  Int2 j
){
  RecT  rdraw;

  LoadRect ( &rdraw, x, y, x+w, y+h );
  Black();
  switch (j*2 + i){
    case 0:
      Solid(); break;
    case 1:
      Dark(); break;
    case 2:
      Medium(); break;
    default:
      Light();
  }
  PaintRect ( &rdraw );
  FrameRect ( &rdraw );
}

static void 
/*FCN*/shaddingChg (
  void
){
  Int2 i;

  i = interPanels[INPANEL_SHADDING].cury * 2 + 
      interPanels[INPANEL_SHADDING].curx;
  switch (i){
    case 0:
      i = SOLID_SHADING; break;
    case 1:
      i = DARK_SHADING; break;
    case 2:
      i = MEDIUM_SHADING; break;
    default:
      i = LIGHT_SHADING;
  }
  tools[SM_TOOLS_SHADDING].val =  (BigScalar)i;
  if ( curProc != NULL ) curProc (SM_TOOLS_SHADDING); 
}

static void
/*FCN*/lTypeDraw (
  Int2 x, 
  Int2 y, 
  Int2 w, 
  Int2 h, 
  Int2 i, 
  Int2 j
){
  PoinT pt1, pt2;

  pt1.x = x+1 ; pt2.x = x+w-2;
  pt1.y = pt2.y = y + h/2;
  Black();
  switch ( i*3 + j ){
    case 0:
      WidePen ( 1 );
      Solid(); break;
    case 1:
      WidePen ( 1 );
      Dotted(); break;
    case 2:
      WidePen ( 1 );
      Dashed(); break;
    case 3:
      Solid(); 
      WidePen ( 2 ); break;
    case 4:
      Solid(); 
      WidePen ( 3 ); break;
    default:
      Solid(); 
      WidePen ( 4 ); break;
  }
  DrawLine ( pt1, pt2 );
}

static void 
/*FCN*/lTypeChg (
  void
){
  Int2 i;

  i = interPanels[INPANEL_LTYPE].curx * 3 + 
      interPanels[INPANEL_LTYPE].cury;
  switch(i){
    case 0:
      i = 0x100 | SOLID_LINE; break;
    case 1:
      i = 0x100 | DOTTED_LINE; break;
    case 2:
      i = 0x100 | DASHED_LINE; break;
    case 3:
      i = 0x200 | SOLID_LINE; break;
    case 4:
      i = 0x300 | SOLID_LINE; break;
    default:
      i = 0x400 | SOLID_LINE; break;
  }
  tools[SM_TOOLS_LTYPE].val =  (BigScalar)i;
  if ( curProc != NULL ) curProc (SM_TOOLS_LTYPE); 
}

static void 
/*FCN*/InPanelCursor (
  Nlm_InterPanelPtr pInfo,
  RecT         PNTR rpPtr
){
  RecT rdraw;
  Int2 x,y;

  if ( pInfo->curx >= 0 ){
    WidePen (1);
    Black();
    Solid();
    InvertMode();
    x = rpPtr->left + pInfo->curx*(pInfo->width+4);
    y = rpPtr->top +  pInfo->cury*(pInfo->height+4),
    LoadRect ( &rdraw, x, y, 
               x+pInfo->width+4, y+pInfo->height+4 );
    FrameRect ( &rdraw );
    CopyMode();
  }
}

static void 
/*FCN*/InPanelClick ( 
  PaneL p, 
  Nlm_PoinT pt
){
  Nlm_RecT          rp;
  Nlm_InterPanelPtr pInfo;
  Int2              x, y;

  ObjectRect (p, &rp);
  pInfo = &(interPanels[0]);
  while ( pInfo->p != p ) pInfo++;
  Nlm_ObjectRect (p, &rp);
  x = pt.x - rp.left;
  y = pt.y - rp.top;
  x /= pInfo->width + 4;
  y /= pInfo->height + 4;
  if ( (x < 0) || (y < 0) ||
       (x >= pInfo->x) || (y >= pInfo->y) ||
       ((pInfo->curx == x) && (pInfo->cury == y)) ) return;
  InPanelCursor ( pInfo, &rp );
  pInfo->curx = x;
  pInfo->cury = y;
  InPanelCursor ( pInfo, &rp );
  pInfo->change();
} /* End of InPanelClick() */

static void
/*FCN*/InPanelDraw (
  PaneL p
){
  Nlm_InterPanelPtr pInfo;
  RecT              rp;
  Int2              i, j;

  ObjectRect (p, &rp);
  pInfo = &(interPanels[0]);
  while ( pInfo->p != p ) pInfo++;
  for ( i=0; i<pInfo->x; i++ ){
    for ( j=0; j<pInfo->y; j++ ){
      pInfo->draw ( rp.left + 2 + i*(pInfo->width+4),
                    rp.top + 2 + j*(pInfo->height+4),
                    pInfo->width, pInfo->height,
                    i, j );
    }
  }
  InPanelCursor( pInfo, &rp );
}

static void
/*FCN*/InPanelSet (
  Nlm_InterPanelPtr pInfo,
  Int2 curx, 
  Int2 cury
){
  Nlm_WindoW tmpPort;
  RecT       rp;

  if ( (curx != pInfo->curx) || (cury != pInfo->cury) ){
    ResetClip();
    tmpPort = SavePort (pInfo->p);
    Select ( pInfo->p );
    ObjectRect ( pInfo->p, &rp);
    InPanelCursor ( pInfo, &rp );
    pInfo->curx = curx;
    pInfo->cury = cury;
    InPanelCursor ( pInfo, &rp );
    RestorePort (tmpPort);
  }
}

static void
/*FCN*/InPanel ( 
  GrouP             g, 
  Nlm_InterPanelPtr pInfo
){
  SetGroupMargins (g, 10, 10 );
  pInfo->p = SimplePanel ( g, pInfo->x * (pInfo->width + 4), 
             pInfo->y * (pInfo->height + 4), 
             InPanelDraw );
  SetPanelClick ( pInfo->p, InPanelClick, NULL, NULL, NULL );
} /* End of InPanel () */

static void 
/*FCN*/Nlm_FontList ( 
  LisT lst
){
  Nlm_AddFontListPtr addFontPtr;
  Nlm_FontSpec       fsp;
  Nlm_Int2           val;
  Nlm_Int2           i;
  Nlm_Boolean        upd = FALSE;
  Nlm_Char           fullFontName[64];

  val = Nlm_GetValue (lst) - 1;
  if ( (val >= 0) && (tools[SM_TOOLS_FONT].val != (BigScalar)NULL) ){
    if ( val == 0 ){
      fsp = *((Nlm_FontSpecPtr)tools[SM_TOOLS_FONT].val);
      Nlm_DisableSM();
      if ( Nlm_ChooseFont ( &fsp, CFF_READ_FSP, NULL) ) {
        Nlm_EnableSM();
        fsp.name[FONT_NAME_SIZE-1] = 0;
        *((Nlm_FontSpecPtr)tools[SM_TOOLS_FONT].val) = fsp;
        for ( i=0; i<INIT_FONTSET; i++ ){
          if ( Nlm_EqualFontSpec ( &fsp, &(fontNames[i]) ) ) break;
        }
        if ( i == INIT_FONTSET ){
          addFontPtr = addFontList;
          while ( addFontPtr != NULL ) {
            if ( Nlm_EqualFontSpec ( &fsp, &(addFontPtr->fsp) ) ) break;
            addFontPtr = addFontPtr->next;
          }
          if ( addFontPtr == NULL ){
            sprintf ( fullFontName, "%s %d ", fsp.name, fsp.size );
            if ( fsp.style & STYLE_BOLD ) StringCat ( fullFontName, "b" );
            if ( fsp.style & STYLE_ITALIC ) StringCat ( fullFontName, "i" );
            if ( fsp.style & STYLE_UNDERLINE ) StringCat ( fullFontName, "u" );
            ListItem ( lst, fullFontName );
            addFontPtr = addFontList;
            if ( addFontPtr == NULL ) {
              addFontList = addFontPtr = MemNew ( sizeof(Nlm_AddFontList) );
            } else {
              while ( addFontPtr->next != NULL ) addFontPtr = addFontPtr->next;
              addFontPtr->next = MemNew ( sizeof(Nlm_AddFontList) );
              addFontPtr = addFontPtr->next;
            }
            addFontPtr->next = NULL;
            addFontPtr->fsp = fsp;
          }
        }
        upd = TRUE;
      } else {
        Nlm_EnableSM();
      }
    } else if ( val <= INIT_FONTSET ) {
      *((Nlm_FontSpecPtr)tools[SM_TOOLS_FONT].val) = fontNames[val-1];
      upd = TRUE;
    } else {
      val -= 7;
      addFontPtr = addFontList;
      while ( val ){
        addFontPtr = addFontPtr->next;
        val--;
      }
      *((Nlm_FontSpecPtr)tools[SM_TOOLS_FONT].val) = addFontPtr->fsp;
      upd = TRUE;
    }
    if ( upd && (curProc != NULL) ) curProc ( SM_TOOLS_FONT );
  }
}

static void
/*FCN*/OnCustomColor (
  ButtoN bn
){
  Uint4  color;
  Uint1  r,g,b;

  color = (Uint4)tools[SM_TOOLS_COLOR].val;
  r = (Uint1)(color >> 16 ) & 0xFF;
  g = (Uint1)(color >> 8 ) & 0xFF;;
  b = (Uint1)color & 0xFF;
  Nlm_DisableSM();
  if ( Nlm_ChooseColorDialog ( &r,&g,&b, TRUE ) ){
    Nlm_EnableSM();
    tools[SM_TOOLS_COLOR].val = (BigScalar)
      (((Uint4)r << 16) | ((Uint4)g << 8) | (Uint4)b);
    InPanelSet ( &(interPanels[INPANEL_COLOR]), -1, 0 );
    if ( curProc != NULL ) curProc (SM_TOOLS_COLOR); 
  } else {
    Nlm_EnableSM();
  }
}

/**************************************************************************/
/* GLOBAL FUNCTIONS */
/**************************************************************************/
GrouP 
/*FCN*/Nlm_CtreateToolsSM ( 
  WindoW w
){
  GrouP     g1;
  Int2      i;
  Nlm_Char  fullFontName[32];

  g1 = HiddenGroup ( w, 4, 1, NULL );
  SetGroupSpacing (g1, 10, 0);
  tools[SM_TOOLS_COLOR].g = NormalGroup ( g1 ,-1, 2,"Color", systemFont, NULL);
  InPanel ( tools[SM_TOOLS_COLOR].g, &(interPanels[INPANEL_COLOR]) );
  AlignObjects ( ALIGN_CENTER, 
    (HANDLE)PushButton ( tools[SM_TOOLS_COLOR].g, "Other...", OnCustomColor ),
    (HANDLE)interPanels[INPANEL_COLOR].p, NULL);

  tools[SM_TOOLS_SHADDING].g = 
      NormalGroup ( g1 ,1, 1,"Shading", systemFont, NULL);
  InPanel ( tools[SM_TOOLS_SHADDING].g, &(interPanels[INPANEL_SHADDING]));

  tools[SM_TOOLS_LTYPE].g = 
      NormalGroup ( g1 ,1, 1,"Line Type", systemFont, NULL);
  InPanel ( tools[SM_TOOLS_LTYPE].g, &(interPanels[INPANEL_LTYPE]) );

  tools[SM_TOOLS_FONT].g = 
      NormalGroup ( g1 ,1, 1,"Font", systemFont, NULL);
#ifdef MOTIF
  fontList = SingleList ( tools[SM_TOOLS_FONT].g, 12, 4, Nlm_FontList );
#else
  fontList = SingleList ( tools[SM_TOOLS_FONT].g, 8, 4, Nlm_FontList );
#endif
  ListItem ( fontList, "Other..." );
  for ( i=0; i<INIT_FONTSET; i++ ) {
    sprintf ( fullFontName, "%s %d", fontNames[i].name, fontNames[i].size );
    ListItem ( fontList, fullFontName );
  }
  Nlm_ShowToolsSM ( 0 );
  return g1;
}

void 
/*FCN*/Nlm_SetToolsCallbackSM ( 
  Nlm_SmToolsProc proc
){
  curProc = proc;
}

void 
/*FCN*/Nlm_SetToolDefaultSM  ( 
  Int2      tool, 
  BigScalar val
){
  tools[tool].val = val;
}

BigScalar
/*FCN*/Nlm_GetToolValueSM  ( 
  Int2 tool
){
  return tools[tool].val;
}

void
/*FCN*/Nlm_ShowToolsSM ( 
Int2 show_tools
){
  Nlm_FontSpecPtr    valfsp;
  Nlm_AddFontListPtr addFontPtr;
  Int4               val;
  Int2               curx, cury, i;
  NLMINTERNALCOLOR   color;

  if ( show_tools & SM_SHTOOLS_COLOR ){
    Enable (tools[SM_TOOLS_COLOR].g);
    val = (Int4)tools[SM_TOOLS_COLOR].val;
    color.red = (Uint1)((val>>16)&0xFF);
    color.green = (Uint1)((val>>8)&0xFF);
    color.blue = (Uint1)(val&0xFF);
    for ( cury=0; cury<2; cury++ ){
      for ( curx=0; curx<8; curx++ ){
        i = curx + cury*8;
        if ( (color.red == theBasicColors[i].red) &&
             (color.green == theBasicColors[i].green) &&
             (color.blue == theBasicColors[i].blue)) break;
      }
      if ( curx != 8 ) break;
    }
    if ( cury == 2 ) curx = -1;
    InPanelSet ( &(interPanels[INPANEL_COLOR]), curx, cury );
  } else Disable (tools[SM_TOOLS_COLOR].g);
  if ( show_tools & SM_SHTOOLS_SHADDING ){
    Enable (tools[SM_TOOLS_SHADDING].g);
    val = (Int4)tools[SM_TOOLS_SHADDING].val;
    switch ( val ){
      case DARK_SHADING:
        curx = 1; cury = 0; break;
      case MEDIUM_SHADING:
        curx = 0; cury = 1; break;
      case LIGHT_SHADING:
        curx = 1; cury = 1; break;
      default:
        curx = cury = 0;
    }
    InPanelSet ( &(interPanels[INPANEL_SHADDING]), curx, cury );
  } else Disable (tools[SM_TOOLS_SHADDING].g);
  if ( show_tools & SM_SHTOOLS_LTYPE ){
    Enable (tools[SM_TOOLS_LTYPE].g);
    val = (Int4)tools[SM_TOOLS_LTYPE].val;
    switch (val) {
      case (0x100 | DOTTED_LINE):
        curx = 0; cury = 1; break;
      case (0x100 | DASHED_LINE):
        curx = 0; cury = 2; break;
      case (0x200 | SOLID_LINE):
        curx = 1; cury = 0; break;
      case (0x300 | SOLID_LINE):
        curx = 1; cury = 1; break;
      case (0x400 | SOLID_LINE):
        curx = 1; cury = 2; break;
      default:
        curx = cury = 0;
    }
    InPanelSet ( &(interPanels[INPANEL_LTYPE]), curx, cury );
  } else Disable (tools[SM_TOOLS_LTYPE].g);
  if ( show_tools & SM_SHTOOLS_FONT ){
    Hide(tools[SM_TOOLS_FONT].g);
    Enable (tools[SM_TOOLS_FONT].g);
    valfsp = (Nlm_FontSpecPtr)tools[SM_TOOLS_FONT].val;
    i = -1;
    if ( valfsp != NULL ) {
      for ( i=0; i<INIT_FONTSET; i++ ){
        if ( EqualFontSpec ( valfsp, &(fontNames[i])) ){
          break;
        }
      }
      if ( i == INIT_FONTSET ){
        addFontPtr = addFontList;
        while ( addFontPtr != NULL ) {
          if ( Nlm_EqualFontSpec ( valfsp, &(addFontPtr->fsp) ) ) break;
          addFontPtr = addFontPtr->next;
          i++;
        }
        if ( addFontPtr == NULL ) i = -1;
      }
    }
    i += 2;
    SetValue(fontList, i);
    Show(tools[SM_TOOLS_FONT].g);
  } else Disable (tools[SM_TOOLS_FONT].g);
}

/* END */
