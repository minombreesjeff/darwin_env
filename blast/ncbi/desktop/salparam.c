/*   salparam.c
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
* File Name:  salparam.c
*
* Author:  Colombe Chappey
*
* Version Creation Date:   1/27/96
*
* $Revision: 6.57 $
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
#include <salparam.h>
#include <saledit.h>
#include <salutil.h>
#include <salstruc.h>
#include <salsap.h>
#include <salpanel.h>
#include <salfiles.h>
#include <saled.h>
#include <salprop.h>
#include <dotmatrx.h>
#include <urkptpf.h>
#include <dlogutil.h>
#include <biosrc.h>
#include <satutil.h>
#include <alignmgr2.h>

#define OBJ_VIRT 254

static void DotPlotBtn (ButtoN b);

static void get_client_rectxy (PaneL p, RectPtr prc, Int2 x, Int2 y)
{
  ObjectRect (p, prc);
  InsetRect (prc, x, y);
}

/*********************************
***    Change Font Procedures
***********************************/
extern void SeqFontProc (IteM i)
{
  WindoW             temport;        
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr   adp;
  FonT               f;
  Nlm_FontSpec       font;

  wdp = (SeqEditViewFormPtr) GetObjectExtra ((WindoW)ParentWindow (i));
  if ( ( adp = GetAlignDataPanel (wdp->pnl) ) == NULL ) return;
  GetFontSpec ((FonT)(adp->font), &font);
  if ( ChooseFont(&font, CFF_MONOSPACE, NULL)) 
  {
     f = CreateFont(&font);
     adp->font = (Handle)f; 
     SelectFont (f);
     adp->lineheight = LineHeight () + adp->interline;
     adp->leading = Leading ();
     adp->charw   = CharWidth ('0');
     adp->ascent  = Ascent ();
     adp->scaleheight  = 2 * adp->lineheight;
     adp->margin.left  = adp->marginleft * adp->charw;
     temport = SavePort((WindoW)ParentWindow (i));
     Select (wdp->pnl);
     do_resize_window (wdp->pnl, adp, FALSE);
     inval_panel (wdp->pnl, -1 ,-1);
     RestorePort (temport);
  } 
  Update ();
  return;
}

static void ChangeFontButton (ButtoN b)
{
  WindoW           wdialog;        
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  FonT             f;
  Nlm_FontSpec     font;

  wdialog = ParentWindow (b);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  GetFontSpec ((FonT)(adp->font), &font);
  if ( ChooseFont(&font, CFF_READ_FSP, NULL)) 
  {
         f = CreateFont(&font);
         adp->newstyle->font = (Handle)f;
  } 
  return;
}

static void ChangeInterlineButton (TexT text)
{
  WindoW           wdialog;        
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  Char             str[16];

  wdialog = ParentWindow (text);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  GetTitle (text, str, 16);
  adp->newstyle->interline = (Int2) atoi (str);
  return;
}

static void ChangeMarginIndexButton (ButtoN bn)
{
  WindoW             wdialog;        
  DialogBoxDataPtr   dbdp;
  EditAlignDataPtr   adp;

  wdialog = ParentWindow (bn);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  adp->newstyle->marginwithindex = GetStatus (bn);
  return;
}

static void ChangeMarginPosButton (ButtoN bn)
{
  WindoW           wdialog;        
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;

  wdialog = ParentWindow (bn);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  adp->newstyle->marginwithpos = GetStatus (bn);
  return;
}

static void DrawScaleButton (ButtoN bn)
{
  WindoW           wdialog;        
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;

  wdialog = ParentWindow (bn);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  adp->newstyle->draw_scale = GetStatus (bn);
  return;
}

static void DrawBarScaleButton (ButtoN bn)
{
  WindoW           wdialog;        
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;

  wdialog = ParentWindow (bn);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  adp->newstyle->draw_bars = GetStatus (bn);
  return;
}

static void ColumnCellButton (TexT text)
{
  WindoW           wdialog;        
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  Int2             val;
  Char             str[16];

  wdialog = ParentWindow (text);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  GetTitle (text, str, 16);
  val = (Int2) atoi (str);
  if (val > 50) {
         val = 50;
         SetTitle (text, "40");
  }
  adp->newstyle->columnpcell = (Uint1)val;
  return;
}
static void ChangeMarginButton (TexT text)
{
  WindoW           wdialog;        
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  Int2             margin;
  Char             str[16];

  wdialog = ParentWindow (text);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  GetTitle (text, str, 16);
  margin = (Int2) atoi (str);
  if (margin > 40) {
         margin = 40;
         SetTitle (text, "40");
  }
  adp->newstyle->marginleft = margin;
  return;
}

static void ChangeMarginPopup (PopuP p)
{
  WindoW           wdialog;        
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  Char             str[8];
  Int2             val;

  wdialog = ParentWindow (p);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  switch ( (val = GetValue (p)) ) {
      case 2:
         adp->marginleft = SALSA_PHYLIP;
         break;
      case 3:
         adp->marginleft = SALSA_MACAW;
         break;
      default:
         break;
  }
  sprintf (str, "%d", (int) adp->marginleft);
  SetTitle (dbdp->txt5, str);
  adp->margin.left = adp->marginleft * adp->charw;
  return;
}

/*********************************
***    SelectColorProc
***********************************/

extern void setUpLetterColor ( Uint4 *colorRefs, Uint1 letter,
                               Uint1 red, Uint1 green, Uint1 blue )
{
  colorRefs[letter] = GetColorRGB(red,green,blue);
}

static void SelectColorScaleButton (ButtoN bn)
{
  WindoW           wdialog;        
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  Uint1            r, g, b;

  wdialog = ParentWindow (bn);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  if ( adp->seqnumber == 0 ) return;
  if ( ChooseColorDialog (&r, &g, &b, 0) ) {
     setUpLetterColor ( adp->newstyle->colorRefs, (Uint1)COLOR_SCALE, r, g, b);
  }
  return;
}

static void SelectColorIdButton (ButtoN bn)
{
  WindoW           wdialog;        
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  Uint1            r, g, b;

  wdialog = ParentWindow (bn);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  if ( adp->seqnumber == 0 ) return;
  if ( ChooseColorDialog (&r, &g, &b, 0) ) {
      setUpLetterColor ( adp->newstyle->colorRefs, (Uint1)COLOR_ID, r, g, b );
  }
  return;
}

static void SelectColorIdMasterButton (ButtoN bn)
{
  WindoW           wdialog;        
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  Uint1            r, g, b;

  wdialog = ParentWindow (bn);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  if ( adp->seqnumber == 0 ) return;
  if ( ChooseColorDialog (&r, &g, &b, 0) ) {
    setUpLetterColor(adp->newstyle->colorRefs, (Uint1)COLOR_ID_MASTER, r, g, b);
  }
  return;
}

static void SelectColorSelectionButton (ButtoN bn)
{
  WindoW           wdialog;        
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  Uint1            r, g, b;

  wdialog = ParentWindow (bn);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  if ( adp->seqnumber == 0 ) return;
  if ( ChooseColorDialog (&r, &g, &b, 0) ) {
     setUpLetterColor (adp->newstyle->colorRefs, (Uint1)COLOR_SELECT, r, g, b);
  }
  return;
}

static void SelectAaColorPopup (PopuP p)
{
  WindoW           wdialog;        
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr     adp;
  Int2             val;
  Uint1            r, g, b;

  wdialog = ParentWindow (p);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  if ( adp->seqnumber == 0 ) return;
  if ( ChooseColorDialog (&r, &g, &b, 0) ) {
    switch ( ( val = GetValue(p) ) ) {
     case 1:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('A' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('C' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('D' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('E' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('F' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('G' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('H' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('I' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('K' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('L' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('M' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('N' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('P' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('Q' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('R' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('S' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('T' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('V' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('W' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('Y' - '*'), r, g, b);
        break;
     case 2:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('M' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('F' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('L' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('V' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('I' - '*'), r, g, b);
        break;
     case 3:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('R' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('H' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('E' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('K' - '*'), r, g, b);
        break;
     case 4:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('T' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('N' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('P' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('S' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('D' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('Q' - '*'), r, g, b);
        break;
     case 5:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('G' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('C' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('A' - '*'), r, g, b);
        break;
     case 6:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('*' - '*'), r, g, b);
        break;
     default:
        break;
    }
  }
  return;
}

static void SelectAnColorPopup (PopuP p)
{
  WindoW           wdialog;        
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr     adp;
  Int2             val;
  Uint1            r, g, b;

  wdialog = ParentWindow (p);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  if ( adp->seqnumber == 0 ) return;
  if ( ChooseColorDialog (&r, &g, &b, 0) ) {
    switch ( ( val = GetValue(p) ) ) {
     case 1:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('a' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('c' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('t' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('g' - '*'), r, g, b);
        break;
     case 2:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('a' - '*'), r, g, b);
        break;
     case 3:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('c' - '*'), r, g, b);
        break;
     case 4:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('g' - '*'), r, g, b);
        break;
     case 5:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('t' - '*'), r, g, b);
        break;
     case 6:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('g' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('a' - '*'), r, g, b);
        break;
     case 7:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('t' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('c' - '*'), r, g, b);
        break;
     case 8:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('a' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('c' - '*'), r, g, b);
        break;
     case 9:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('a' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('t' - '*'), r, g, b);
        break;
     case 10:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('g' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('c' - '*'), r, g, b);
        break;
     case 11:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('g' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('t' - '*'), r, g, b);
        break;
     case 12:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('g' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('c' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('a' - '*'), r, g, b);
        break;
     case 13:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('a' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('c' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('t' - '*'), r, g, b);
        break;
     case 14:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('g' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('a' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('t' - '*'), r, g, b);
        break;
     case 15:
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('g' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('t' - '*'), r, g, b);
        setUpLetterColor(adp->newstyle->colorRefs,(Uint1)('c' - '*'), r, g, b);
        break;
     default:
        break;
    }
  }
  return;
}

static void ResetBnStyleProc (ButtoN b)
{
  WindoW             w, wdialog;        
  DialogBoxDataPtr   dbdp;
  EditAlignDataPtr   adp;
  Uint2              j;

  wdialog = ParentWindow (b);
  Hide (wdialog);
  Update();
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  w = dbdp->w;
  if ( ( adp = GetAlignEditData (w) ) == NULL ) return;

  adp->newstyle->font = adp->font;
  adp->newstyle->interline = adp->interline;
  adp->newstyle->marginwithindex = adp->marginwithindex;
  adp->newstyle->marginwithpos = adp->marginwithpos;
  adp->newstyle->marginleft = adp->marginleft;
  adp->newstyle->draw_scale = adp->draw_scale;
  adp->newstyle->draw_bars = adp->draw_bars;
  adp->newstyle->rowpcell = adp->rowpcell;
  adp->newstyle->columnpcell = adp->columnpcell;
  for (j=0; j<256; j++)
         adp->newstyle->colorRefs[j] = adp->colorRefs[j];
  MemFree (adp->newstyle);
  adp->newstyle = NULL;
  Remove (wdialog);
  return;
} 

static void ApplyBnStyleProc (ButtoN b)
{
  WindoW             w, wdialog;        
  WindoW             temport;
  DialogBoxDataPtr   dbdp;
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr   adp;
  Uint2              j;

  wdialog = ParentWindow (b);
  Hide (wdialog);
  Update();
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  w = dbdp->w;
  if ( ( adp = GetAlignEditData (w) ) == NULL ) return;
  wdp = (SeqEditViewFormPtr) GetObjectExtra (w);

  WatchCursor();
  if (adp->font != adp->newstyle->font)
  {
         adp->font = adp->newstyle->font;
         SelectFont ((FonT)(adp->font));
         adp->lineheight = LineHeight () + adp->interline;
         adp->leading = Leading ();
         adp->charw   = CharWidth ('0');
         adp->ascent  = Ascent ();
         adp->scaleheight  = 2 * adp->lineheight;
         adp->margin.left  = adp->marginleft * adp->charw;
  }
  if (adp->interline != adp->newstyle->interline) {
         adp->interline = adp->newstyle->interline;
         SelectFont ((FonT)(adp->font));
         adp->lineheight = LineHeight () + adp->interline;
  }
  if (adp->marginwithindex != adp->newstyle->marginwithindex)
         adp->marginwithindex = adp->newstyle->marginwithindex;
  if (adp->marginwithpos != adp->newstyle->marginwithpos)
         adp->marginwithpos = adp->newstyle->marginwithpos;

  if (adp->marginleft != adp->newstyle->marginleft) {
         adp->marginleft = adp->newstyle->marginleft;
         adp->margin.left = adp->marginleft * adp->charw;
  }
  if (adp->draw_scale != adp->newstyle->draw_scale)
         adp->draw_scale = adp->newstyle->draw_scale;
  if (adp->draw_bars != adp->newstyle->draw_bars)
         adp->draw_bars = adp->newstyle->draw_bars;
  adp->nscaleline = 0;
  if ( adp->draw_scale ) adp->nscaleline ++;
  if ( adp->draw_bars )  adp->nscaleline ++;
  if (adp->rowpcell != adp->newstyle->rowpcell) {
     adp->rowpcell = adp->newstyle->rowpcell;
  }
  if (adp->columnpcell != adp->newstyle->columnpcell) 
  {
     Int2 x, y;
     adp->columnpcell = adp->newstyle->columnpcell;

     y = 0; x = 0;
     if (adp->columnpcell > 0) {
        y = (Int2) (adp->pnlWidth -adp->marginleft) / (Int2) adp->columnpcell;
        x = (Int2)(adp->pnlWidth -adp->marginleft -y) % (Int2)(adp->columnpcell);
     }
     adp->visibleWidth = (Int2) (adp->pnlWidth -adp->marginleft -y -x);
     if (adp->visibleWidth < 10) {
        adp->firstssp = NULL;
     }
  }
  for (j=0; j<256; j++)
         adp->colorRefs[j] = adp->newstyle->colorRefs[j];
  MemFree (adp->newstyle);
  adp->newstyle = NULL;
  do_resize_window (wdp->pnl, adp, TRUE);
  temport = SavePort(w);
  Select (wdp->pnl);
  inval_panel (wdp->pnl, -1 ,-1); 
  RestorePort (temport);
  Update ();
  Remove (wdialog);
  Update ();
  ArrowCursor();
  return;
}
/***************************************************
*** Translation
****************************************************/
static void SetRfFunc (PaneL pnl, Uint2 rf)
{
  WindoW           temport;
  EditAlignDataPtr adp;
  SeqEditViewFormPtr wdp;
  SelStructPtr     ssp;
  ValNodePtr       vnp;
  SeqParamPtr      prm;
  Int2             j;
  float hratio;
  
  wdp = (SeqEditViewFormPtr) GetObjectExtra ((WindoW)ParentWindow (pnl));
  if ( ( adp = GetAlignDataPanel (pnl) ) == NULL ) return;
  if (adp->seqnumber == 0 || ISA_aa(adp->mol_type))
    return;
  hratio = (float)adp->hoffset / (float)adp->length;
  temport = SavePort(pnl);
  Select (pnl);
  inval_panel (pnl, -1, -1);
  if ( checkOMss_for_itemtype (OBJ_BIOSEQ) == 0 ) {
         ssp = &(adp->master);
  }
  else ssp = ObjMgrGetSelected();  
  for (; ssp != NULL; ssp = ssp->next) {
     if (checkssp_for_editor (ssp) && ssp->itemtype == OBJ_BIOSEQ) {
        for (vnp = adp->params; vnp != NULL; vnp = vnp->next) {
           prm = (SeqParamPtr) vnp->data.ptrvalue;
           if ( prm->entityID == ssp->entityID ) {
             if (rf < (Uint2) 6) {
                prm->rf[rf] = TRUE; 
             }
             switch ((int)rf) {
               case 0: 
                       prm->rf[1] = prm->rf[2] = FALSE; 
                       SetStatus (wdp->rfitem [1], FALSE);
                       SetStatus (wdp->rfitem [2], FALSE);
                       SetStatus (wdp->rfitem [6], FALSE);
                       SetStatus (wdp->rfitem [8], FALSE);
                       break;
               case 1: 
                       prm->rf[0] = prm->rf[2] = FALSE; 
                       SetStatus (wdp->rfitem [0], FALSE);
                       SetStatus (wdp->rfitem [2], FALSE);
                       SetStatus (wdp->rfitem [6], FALSE);
                       SetStatus (wdp->rfitem [8], FALSE);
                       break;
               case 2: 
                       prm->rf[0] = prm->rf[1] = FALSE; 
                       SetStatus (wdp->rfitem [0], FALSE);
                       SetStatus (wdp->rfitem [1], FALSE);
                       SetStatus (wdp->rfitem [6], FALSE);
                       SetStatus (wdp->rfitem [8], FALSE);
                       break;
               case 3: 
                       prm->rf[4] = prm->rf[5] = FALSE; 
                       SetStatus (wdp->rfitem [4], FALSE);
                       SetStatus (wdp->rfitem [5], FALSE);
                       SetStatus (wdp->rfitem [7], FALSE);
                       SetStatus (wdp->rfitem [8], FALSE);
                       break;
               case 4: 
                       prm->rf[3] = prm->rf[5] = FALSE; 
                       SetStatus (wdp->rfitem [3], FALSE);
                       SetStatus (wdp->rfitem [5], FALSE);
                       SetStatus (wdp->rfitem [7], FALSE);
                       SetStatus (wdp->rfitem [8], FALSE);
                       break;
               case 5: 
                       prm->rf[3] = prm->rf[4] = FALSE; 
                       SetStatus (wdp->rfitem [3], FALSE);
                       SetStatus (wdp->rfitem [4], FALSE);
                       SetStatus (wdp->rfitem [7], FALSE);
                       SetStatus (wdp->rfitem [8], FALSE);
                       break;
               case 6: if (GetStatus (wdp->rfitem [6]) == FALSE) {
                        prm->rf[0] =FALSE; prm->rf[1] =FALSE; prm->rf[2]=FALSE; 
                       } else {
                        prm->rf[0] =TRUE; prm->rf[1] =TRUE; prm->rf[2] =TRUE; 
                       }
                       SetStatus (wdp->rfitem [0], FALSE);
                       SetStatus (wdp->rfitem [1], FALSE);
                       SetStatus (wdp->rfitem [2], FALSE);
                       SetStatus (wdp->rfitem [8], FALSE);
                       break;
               case 7: if (GetStatus (wdp->rfitem [7]) == FALSE) {
                        prm->rf[3] =FALSE; prm->rf[4] =FALSE; prm->rf[5]=FALSE; 
                       } else {
                        prm->rf[3] = TRUE; prm->rf[4] = TRUE; prm->rf[5] =TRUE; 
                       }
                       SetStatus (wdp->rfitem [3], FALSE);
                       SetStatus (wdp->rfitem [4], FALSE);
                       SetStatus (wdp->rfitem [5], FALSE);
                       SetStatus (wdp->rfitem [8], FALSE);
                       break;
               case 8: 
                       if (GetStatus (wdp->rfitem [8]) == FALSE) {
                        prm->rf[0] =FALSE; prm->rf[1] =FALSE; prm->rf[2]=FALSE; 
                        prm->rf[3] =FALSE; prm->rf[4] =FALSE; prm->rf[5]=FALSE; 
                       } else {
                        prm->rf[0] =TRUE; prm->rf[1] =TRUE; prm->rf[2] =TRUE; 
                        prm->rf[3] =TRUE; prm->rf[4] =TRUE; prm->rf[5] =TRUE; 
                       }
                       for (j=0; j<8; j++) 
                          SetStatus (wdp->rfitem [0], FALSE);
                       break;
               case 9: 
                       prm->rf[0] =FALSE; prm->rf[1] =FALSE; prm->rf[2]=FALSE; 
                       prm->rf[3] =FALSE; prm->rf[4] =FALSE; prm->rf[5]=FALSE; 
                       for (j=0; j<9; j++) 
                          SetStatus (wdp->rfitem [j], FALSE);
                       break;
               default : break;
             } 
           } 
        } 
     }
  }
  data_collect_arrange (adp, TRUE);
  SeqEdSetCorrectBarMax (pnl, adp, hratio);
  RestorePort (temport);
}

extern void rf1ItemProc (IteM i) {
   SetRfFunc ((PaneL)GetPanelFromWindow((WindoW)ParentWindow (i)), (Uint2)0);
}
extern void rf2ItemProc (IteM i) {
   SetRfFunc ((PaneL)GetPanelFromWindow((WindoW)ParentWindow (i)), (Uint2)1);
}
extern void rf3ItemProc (IteM i) {
   SetRfFunc ((PaneL)GetPanelFromWindow((WindoW)ParentWindow (i)), (Uint2)2);
}
extern void rf4ItemProc (IteM i) {
   SetRfFunc ((PaneL)GetPanelFromWindow((WindoW)ParentWindow (i)), (Uint2)3);
}
extern void rf5ItemProc (IteM i) {
   SetRfFunc ((PaneL)GetPanelFromWindow((WindoW)ParentWindow (i)), (Uint2)4);
}
extern void rf6ItemProc (IteM i) {
   SetRfFunc ((PaneL)GetPanelFromWindow((WindoW)ParentWindow (i)), (Uint2)5);
}
extern void rf7ItemProc (IteM i) {
   SetRfFunc ((PaneL)GetPanelFromWindow((WindoW)ParentWindow (i)), (Uint2)6);
}
extern void rf8ItemProc (IteM i) {
   SetRfFunc ((PaneL)GetPanelFromWindow((WindoW)ParentWindow (i)), (Uint2)7);
}
extern void rf9ItemProc (IteM i) {
   SetRfFunc ((PaneL)GetPanelFromWindow((WindoW)ParentWindow (i)), (Uint2)8);
}
extern void rf10ItemProc (IteM i) {
   SetRfFunc ((PaneL)GetPanelFromWindow((WindoW)ParentWindow (i)), (Uint2)9);
}

extern void FileInProc (ButtoN b)
{
  WindoW           wdialog;
  DialogBoxDataPtr dbdp;
  FILE             *fp;
  int              n;
  long             lens;
  Char             namep [PATH_MAX];
  Char             str [64];
 
  wdialog = ParentWindow (b);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if (!GetInputFileName (namep, PATH_MAX, "", "TEXT"))
    return;
  SetTitle (dbdp->txt1, namep);  
  if ( dbdp->align_format == SALSA_PHYLIP ) {
         if ( (fp = FileOpen (namep, "r")) == NULL) {
                return ;
         }
         fscanf(fp, "%d %ld", &n, &lens);
         FileClose(fp);
         if (n < 2) {
                return ;
         }
         sprintf (str, "%d", (int) n);
         SetTitle (dbdp->txt2, str);
         sprintf (str, "%ld", (long) lens);
         SetTitle (dbdp->txt3, str);
  }
  return;
}

extern void DefinePanelDialog (IteM i)
{
  WindoW           w, wdialog;
  EditAlignDataPtr adp;
  DialogBoxDataPtr dbdp;
  GrouP            g;
  ButtoN           esBtn1, esBtn3, esBtn4;
  PopuP            popf;
  Char             str[16];
  Uint2            j;

  w = ParentWindow (i);
  if ( ( adp = GetAlignEditData (w) ) == NULL ) return;

  adp->newstyle = (AlignStylePtr) MemNew (sizeof (AlignStyle));
  adp->newstyle->font = adp->font;
  adp->newstyle->interline = adp->interline;
  adp->newstyle->marginwithindex = adp->marginwithindex;
  adp->newstyle->marginwithpos = adp->marginwithpos;
  adp->newstyle->marginleft = adp->marginleft;
  adp->newstyle->draw_scale = adp->draw_scale;
  adp->newstyle->draw_bars = adp->draw_bars;
  adp->newstyle->rowpcell = adp->rowpcell;
  adp->newstyle->columnpcell = adp->columnpcell;
  for (j=0; j< 256; j++) 
         adp->newstyle->colorRefs[j] = adp->colorRefs[j];

  wdialog = FixedWindow (-50, -33, -10, -10, "Sequence", NULL);
  dbdp = (DialogBoxDataPtr) MemNew (sizeof (DialogBoxData));
  SetObjectExtra (wdialog, (Pointer) dbdp, StdCleanupExtraProc);
  dbdp->w = w;
  g = HiddenGroup (wdialog, 1, 0, NULL);
  PushButton (g, "Font", ChangeFontButton );

  g = HiddenGroup (wdialog, 2, 0, NULL);
  esBtn4 = CheckBox (g, "draw scale", DrawScaleButton);  
  SetStatus (esBtn4, adp->draw_scale);
  esBtn4 = CheckBox (g, "draw bars", DrawBarScaleButton);  
  SetStatus (esBtn4, adp->draw_bars);

  g = HiddenGroup (wdialog, 4, 0, NULL);
  StaticPrompt (g, "column per cell",0,popupMenuHeight,systemFont, 'l');
  sprintf (str, "%d", (int) adp->columnpcell);
  dbdp->txt4 = DialogText (g, str, 5, ColumnCellButton);
  StaticPrompt (g, "Interline", 0, popupMenuHeight, systemFont, 'l');
  sprintf (str, "%d", (int) adp->interline);
  dbdp->txt3 = DialogText (g, str, 5, ChangeInterlineButton);

  g = HiddenGroup (wdialog, 4, 0, NULL);
  StaticPrompt (g, "Margin : ", 0, popupMenuHeight, systemFont, 'l');
  sprintf (str, "%d", (int) adp->marginleft);
  dbdp->txt5 = DialogText (g, str, 5, ChangeMarginButton);
  StaticPrompt (g, "columns", 0, popupMenuHeight,systemFont, 'l');
  popf = PopupList (g, TRUE, ChangeMarginPopup);
  PopupItem (popf, " ");
  PopupItem (popf, "PHYLIP");
  PopupItem (popf, "Macaw");
  SetValue  (popf, 1);

  g = HiddenGroup (wdialog, 2, 0, NULL);
  esBtn3 = CheckBox (g, "with positions", ChangeMarginPosButton);  
  SetStatus (esBtn3, adp->marginwithpos);
  esBtn1 = CheckBox (g, "with index  ", ChangeMarginIndexButton);  
  SetStatus (esBtn1, adp->marginwithindex);

if (ISA_na(adp->mol_type))
{
  g = HiddenGroup (wdialog, 7, 0, NULL);
  StaticPrompt (g, "Color:", 0, popupMenuHeight, systemFont, 'l');
  PushButton (g, "SeqId", SelectColorIdButton );
  PushButton (g, "master", SelectColorIdMasterButton );
  PushButton (g, "selection", SelectColorSelectionButton );
  PushButton (g, "scale", SelectColorScaleButton );
  popf = PopupList (g, TRUE, SelectAnColorPopup);
  PopupItem (popf, "ATGC");
  PopupItem (popf, "A");
  PopupItem (popf, "C");
  PopupItem (popf, "G");
  PopupItem (popf, "T");
  PopupItem (popf, "AG");
  PopupItem (popf, "TC");
  PopupItem (popf, "AC");
  PopupItem (popf, "AT");
  PopupItem (popf, "GC");
  PopupItem (popf, "GT");
  PopupItem (popf, "GCA");
  PopupItem (popf, "ACT");
  PopupItem (popf, "GAT");
  PopupItem (popf, "GTC");
  PopupItem (popf, "-");
  SetValue  (popf, 1);
  popf = PopupList (g, TRUE, SelectAaColorPopup);
  PopupItem (popf, "All");
  PopupItem (popf, "MFLVI");
  PopupItem (popf, "RHEK");
  PopupItem (popf, "TNPSDQ");
  PopupItem (popf, "GCA");
  PopupItem (popf, "*");
  SetValue  (popf, 1);
}
  g = HiddenGroup (wdialog, 2, 0, NULL);
  PushButton (g, "Cancel", ResetBnStyleProc);
  PushButton (g, "Accept", ApplyBnStyleProc);
  Show (wdialog); 
  return;
}

extern void inval_panel (PaneL pnl, Int2 x, Int2 y)
{
  RecT     r;

  get_client_rectxy (pnl, &r, x, y);
  InvalRect (&r);
  return;
}

extern void inval_window (WindoW w)
{
  PaneL pnl;
  pnl = GetPanelFromWindow (w);
  if (pnl != NULL) { 
     inval_panel (pnl, -1 ,-1);
  }
  return ;
}

extern void inval_rect (Int2 left, Int2 top, Int2 right, Int2 bottom)
{
  RecT         rm;
  LoadRect (&rm, left, top, right, bottom);
  InsetRect (&rm, -1, -1);
  InvalRect (&rm); 
}


extern void inval_allines (EditAlignDataPtr adp, RecT *rp)
{
  Int2         ptx, pty, Seqlens_pix, j;

  ptx = rp->left + adp->margin.left;
  pty = rp->top;
  j = adp->visibleWidth;
  if (adp->columnpcell > 0) 
     j += (Int2) adp->visibleWidth / (Int2) adp->columnpcell;
  Seqlens_pix = j * adp->charw;
  for (j = 0; j < adp->pnlLine; j++) 
  {
     if ( adp->seqEntity_id[j] > 0 ) {
        inval_rect (ptx, pty, (Int2)(ptx + Seqlens_pix), (Int2)(pty + adp->lineheight));
     }
     pty += adp->lineheight;
  }
}

extern void inval_selstruct (EditAlignDataPtr adp, Uint2 ei, Uint2 ii, Uint2 it, Uint2 itemsubtype, RecT *rp, Int2 left, Int2 toright)
{
  Int2         ptx = rp->left + left, 
               pty = rp->top, 
               j;
  for (j = 0; j < adp->pnlLine; j++) 
  {
    if (is_sameId(adp->seqEntity_id[j], adp->item_id[j], adp->itemtype[j], 255, ei, ii, it, 255) )
    {
       if (itemsubtype == 255 || adp->itemsubtype[j] == itemsubtype) {
          inval_rect (ptx, pty, (Int2)(ptx + toright), (Int2)(pty + adp->lineheight));
       }
    }
    pty += adp->lineheight;
  }
}

extern void inval_selstructpos (EditAlignDataPtr adp, Uint2 ei, Uint2 ii, Uint2 it, RecT *rp, Int4 pos)
{
  Int2         ptx = rp->left, 
               pty;
  Int4         column1, line1;

  SeqPosToLineColumn (ii, ei, it, pos, &line1, &column1, adp->hoffset, adp);
  pty = rp->top + line1 * adp->lineheight;
  inval_rect ((Int2)(ptx + adp->margin.left + (column1-2) * adp->charw),  
              (Int2)(pty), (Int2)(ptx+adp->margin.left+(column1+2) *adp->charw),
              (Int2)(pty + adp->lineheight));
  return;
}

extern void inval_selstructpos_tobottom (EditAlignDataPtr adp, Uint2 ei, Uint2 ii, Uint2 it, RecT *rp, Int4 pos)
{
  Int2         ptx = rp->left, 
               pty;
  Int4         column1, line1;
  Int4         width;

  SeqPosToLineColumn (ii, ei, it, pos, &line1, &column1, adp->hoffset, adp);
  pty = rp->top + line1 * adp->lineheight;
  width = adp->visibleWidth;
  if (adp->columnpcell > 0)
       width += (Int2) adp->visibleWidth / (Int2) adp->columnpcell;

  inval_rect ((Int2)(ptx + adp->margin.left + (column1-2) * adp->charw),  
              (Int2)(pty), (Int2)(ptx+adp->margin.left+ width *adp->charw),
              (Int2)(pty + adp->lineheight));
  pty += adp->lineheight;
  inval_rect ((Int2)(ptx),
              (Int2)(pty), (Int2)(ptx+adp->margin.left+ width *adp->charw),
              (Int2) rp->bottom);
  return;
}

extern void inval_selstructloc (EditAlignDataPtr adp, Uint2 ei, Uint2 ii, Uint2 it, Uint2 itemsubtype, RecT *rp, Int4 from, Int4 to)
{
  RecT         rid;
  Int2         ptx = rp->left, 
               pty;
  Int4         column1, column2, line1;
  Int4         width, j;
  Int4         aln,
               alnmax;
  Boolean      bool1, bool2;

if (from>to) {
   from +=1; to-=1;
} else {
   from-=1; to+=1;
}
  aln = -1;
  alnmax=-1;

  for (j=0; j<MAXLineWindow; j++) {
        if (ii == adp->item_id[j] && ei == adp->seqEntity_id[j] && it == adp->itemtype[j] ) {
           if (aln<0)
              aln++;  
        }
  }
  alnmax = (adp->pnlLine / adp->seqnumber);  
  if (aln<0)
     return;
  if (from < adp->colonne[adp->hoffset - adp->bufferstart]) {     
     from = adp->colonne[adp->hoffset - adp->bufferstart + aln*adp->visibleWidth];
  }
  from += (aln * adp->visibleWidth);
  SeqPosToLineColumn (ii, ei, it, from, &line1, &column1, adp->hoffset, adp);
  if (line1<0)
     from = adp->colonne[adp->hoffset - adp->bufferstart + aln*adp->visibleWidth];

  while (line1 < 0 && aln < alnmax)
  {
     aln++;
     from += (Int4)(adp->visibleWidth);
     SeqPosToLineColumn (ii, ei, it, from, &line1, &column1, adp->hoffset, adp);
  }
  if (line1 < 0)
     return;
  width = adp->visibleWidth;
  if (adp->columnpcell > 0) 
       width += (Int2) adp->visibleWidth / (Int2) adp->columnpcell;
  pty = rp->top + line1 * adp->lineheight;

  for (j = line1; j < adp->pnlLine; j++) 
  {
    if (is_sameId(adp->seqEntity_id[j], adp->item_id[j], adp->itemtype[j], 255, ei, ii, it, 255)) 
    {
     if (itemsubtype == 255 || adp->itemsubtype[j] == itemsubtype) 
     {
       bool1 = SeqPosInLineColumn (from, adp->alignline[j], &column1, adp->hoffset, adp);
       bool2 = SeqPosInLineColumn (to, adp->alignline[j], &column2, adp->hoffset, adp);
       if (bool1 && bool2) {
          LoadRect (&rid, (Int2)(ptx +adp->margin.left+(column1-1) *adp->charw),
                  pty, (Int2)(ptx + adp->margin.left + (column2+1) *adp->charw),
                       (Int2)(pty + adp->lineheight));
          InvalRect (&rid);
       }
       else if ( bool1 ) {
          LoadRect (&rid, (Int2)(ptx+adp->margin.left+(column1-1) *adp->charw), 
                  pty, (Int2)(ptx + adp->margin.left + width * adp->charw), 
                       (Int2)(pty + adp->lineheight));
          InvalRect (&rid);
       }
       else if ( bool2 ) {
          LoadRect (&rid, (Int2)(ptx + adp->margin.left), pty, 
                     (Int2)(ptx + adp->margin.left + (column2+1) * adp->charw), 
                     (Int2)(pty + adp->lineheight));
          InvalRect (&rid);
       }
       else if ( !bool1 && !bool2 && column1 == -1 && column2 == -2 ) { 
          LoadRect (&rid, (Int2)(ptx + adp->margin.left), pty, 
                          (Int2)(ptx + adp->margin.left + width * adp->charw), 
                          (Int2)(pty + adp->lineheight));
          InvalRect (&rid);
       }
     }
    }
    pty += adp->lineheight;
  }
}


extern void inval_selstructloc_forfeat (EditAlignDataPtr adp, Uint2 ei, Uint2 ii, Uint2 it, Uint2 itemsubtype, RecT *rp, Int4 from, Int4 to)
{
  RecT         rid;
  Int2         ptx = rp->left, 
               pty;
  Int4         column1, column2, line1;
  Int4         width, j;
  Uint4        aln;
  Boolean      bool1, bool2;

  if (from < adp->colonne[adp->hoffset - adp->bufferstart]) {     
     aln = 0;
  for (j=0; j<MAXLineWindow; j++) {
        if (ii == adp->item_id[j] && ei == adp->seqEntity_id[j] && it == adp->itemtype[j] ) {
           aln = adp->alignline [j]; 
           break;
        }
     }
     from = adp->colonne[adp->hoffset - adp->bufferstart + aln*adp->visibleWidth];
  }
  SeqPosToLineColumn (ii, ei, it, from, &line1, &column1, adp->hoffset, adp);
  if (line1 < 0) {
     return;
  }
  width = adp->visibleWidth;
  if (adp->columnpcell > 0) 
       width += (Int2) adp->visibleWidth / (Int2) adp->columnpcell;
  pty = rp->top + line1 * adp->lineheight;
  
  for (j = line1; j < adp->pnlLine; j++) 
  {

    if (is_sameId(adp->seqEntity_id[j], adp->item_id[j], adp->itemtype[j], 255, ei, ii, it, 255)) 
    {
     if (itemsubtype == 255 || adp->itemsubtype[j] == itemsubtype) 
     {
       bool1 = SeqPosInLineColumn (from, adp->alignline[j], &column1, adp->hoffset, adp);
       bool2 = SeqPosInLineColumn (to, adp->alignline[j], &column2, adp->hoffset, adp);
       if (bool1 && bool2) {
          LoadRect (&rid, (Int2)(ptx +adp->margin.left+(column1-1) *adp->charw),
                  pty, (Int2)(ptx + adp->margin.left + (column2+1) *adp->charw),
                       (Int2)(pty + adp->lineheight));
          InvalRect (&rid);
       }
       else if ( bool1 ) {
          LoadRect (&rid, (Int2)(ptx+adp->margin.left+(column1-1) *adp->charw), 
                  pty, (Int2)(ptx + adp->margin.left + width * adp->charw), 
                       (Int2)(pty + adp->lineheight));
          InvalRect (&rid);
       }
       else if ( bool2 ) {
          LoadRect (&rid, (Int2)(ptx + adp->margin.left), pty, 
                     (Int2)(ptx + adp->margin.left + (column2+1) * adp->charw), 
                     (Int2)(pty + adp->lineheight));
          InvalRect (&rid);
       }
       else if ( !bool1 && !bool2 && column1 == -1 && column2 == -2 ) { 
          LoadRect (&rid, (Int2)(ptx + adp->margin.left), pty, 
                          (Int2)(ptx + adp->margin.left + width * adp->charw), 
                          (Int2)(pty + adp->lineheight));
          InvalRect (&rid);
       }
     }
    }
    pty += adp->lineheight;
  }
}
extern void inval_rectidselected (EditAlignDataPtr adp, RecT *rp)
{
  SelStructPtr ssp = NULL;
  SelStruct    sp;
  Int2         ptx, pty;
  Int2         width, j;

  ssp = ObjMgrGetSelected ();
  if (ssp == NULL) return;
  ptx = rp->left;
  pty = rp->top;
  for (j = 0; j < adp->pnlLine; j++) 
  {
        sp.entityID = adp->seqEntity_id[j];
        sp.itemID = adp->item_id[j];
        sp.itemtype = adp->itemtype[j];
        sp.regiontype = 0;
        if( (ssp = is_selectedbyID (sp.entityID, sp.itemID, sp.itemtype))!=NULL)
        {
           width = adp->visibleWidth;
           if (adp->columnpcell > 0) 
                 width += (Int2) adp->visibleWidth / (Int2) adp->columnpcell;
           inval_rect (ptx, pty, (Int2)(ptx +adp->margin.left +width *adp->charw), 
                            (Int2)(pty + adp->lineheight));
        }
        pty += adp->lineheight;
  }
}

extern void inval_pep (EditAlignDataPtr adp, RecT *rp)
{
  Int2         ptx, pty;
  Int2         width, j;

  ptx = rp->left;
  pty = rp->top;
  for (j = 0; j < adp->pnlLine; j++) {
        if((adp->itemsubtype[j] >=EDITDEF_RF1 
         && adp->itemsubtype[j] <=EDITDEF_RF6)
        || adp->itemsubtype[j] == FEATDEF_TRSL ) {
           width = adp->visibleWidth;
           if (adp->columnpcell > 0) 
                 width +=(Int2) adp->visibleWidth / (Int2) adp->columnpcell;
           inval_rect ((Int2)(ptx +adp->margin.left), pty,
                       (Int2)(ptx +adp->margin.left + width *adp->charw), 
                       (Int2)(pty + adp->lineheight));
        }
        pty += adp->lineheight;
  }
}

/******************************************************************/
extern void inval_all (EditAlignDataPtr adp, RecT *rp, Uint2 itemtype1, Uint2 itemtype2, Uint2 itemtype3, Int2 width)
{
  SelStructPtr ssp = NULL;
  ssp = ObjMgrGetSelected ();
  if (ssp == NULL) return;
  while (ssp != NULL) 
  {
         if ( checkssp_for_editor(ssp) && (ssp->itemtype == itemtype1 
         || ssp->itemtype == itemtype2 || ssp->itemtype == itemtype3)) {
            if (ssp->itemtype == OBJ_SEQFEAT || ssp->itemtype == OBJ_VIRT) 
               inval_selstruct (adp, ssp->entityID, ssp->itemID, ssp->itemtype, 255, rp, adp->margin.left, (Int2)(width * adp->charw));
            else
               inval_selstructloc (adp, ssp->entityID, ssp->itemID, ssp->itemtype, 255, rp, SeqLocStart ((SeqLocPtr) ssp->region), SeqLocStop((SeqLocPtr) ssp->region)); 
         }
         ssp = ssp->next;
  }
}

/******************************************************************/
/******************************************************************/
static void refreshlst (LisT lst, EditAlignDataPtr adp)
{
  AlignNodePtr     anp;
  SeqParamPtr      prm;
  ValNodePtr       vnp, vnp2;
  Char             str [68];
  Char             str2 [24];
  CharPtr          tmp;

  Reset (lst);
  vnp2 = adp->params;
  for (vnp = adp->anp_list; vnp!=NULL && vnp2!=NULL; vnp = vnp->next)
  {
     str [0] = '\0';
     tmp = str;
     prm = (SeqParamPtr) vnp2->data.ptrvalue;
     sprintf (str2, "%d ", prm->group);
     tmp = StringMove (tmp, str2);
     anp = (AlignNodePtr) vnp->data.ptrvalue;
     SeqIdWrite (anp->sip, str2, PRINTID_REPORT, sizeof (str2));
     tmp = StringMove (tmp, str2);
     ListItem (lst, str);
     vnp2 = vnp2->next;
  }
}

static void grouplst (ButtoN b)
{
  DialogBoxDataPtr dbdp;
  LisT             lst;
  EditAlignDataPtr adp;
  ValNodePtr       vnp;
  SeqParamPtr      prm;
  Int2             n;
  Int2             j, jmax;
  Boolean          val,
                   select = FALSE;

  dbdp = (DialogBoxDataPtr) GetObjectExtra (ParentWindow (b));
  if ( ( adp = GetAlignEditData (dbdp->w) ) != NULL ) {
     lst = dbdp->lst1;
     n = adp->ngroup + 1;
     vnp = adp->params;
     jmax = CountItems (lst);
     for (j=1; j <= jmax && vnp != NULL; j++, vnp = vnp->next)
     {   
        val = GetItemStatus (lst, j);
        if (val) {
           prm = (SeqParamPtr) vnp->data.ptrvalue;
           prm->group = n;
           select = TRUE;
        }
     }
     if (select) {
        adp->ngroup ++;
        refreshlst (lst, adp);
     }
  }
}

static void ungrouplst (ButtoN b)
{
  DialogBoxDataPtr dbdp;
  LisT             lst;
  EditAlignDataPtr adp;
  ValNodePtr       vnp;
  SeqParamPtr      prm;
  Int2             j, jmax;
  Boolean          val;

  dbdp = (DialogBoxDataPtr) GetObjectExtra (ParentWindow (b));
  if ( ( adp = GetAlignEditData (dbdp->w) ) != NULL ) {
     lst = dbdp->lst1;
     vnp = adp->params;
     jmax = CountItems (lst);
     for (j=1; j <= jmax && vnp != NULL; j++, vnp = vnp->next)
     {   
        val = GetItemStatus (lst, j);
        if (val) {
           prm = (SeqParamPtr) vnp->data.ptrvalue;
           prm->group = 0;
        }
     }
     adp->ngroup = 0;
     refreshlst (lst, adp);
  }
}

static void mergelst (ButtoN b)
{
  DialogBoxDataPtr dbdp;
  LisT             lst;
  EditAlignDataPtr adp;
  ValNodePtr       vnp=NULL;
  Int2             j, jmax;
  Boolean          val;

  dbdp = (DialogBoxDataPtr) GetObjectExtra (ParentWindow (b));
  if ( ( adp = GetAlignEditData (dbdp->w) ) != NULL ) {
     lst = dbdp->lst1;
     jmax = CountItems (lst);
     for (j=1; j <= jmax && vnp != NULL; j++, vnp = vnp->next)
     {   
        val = GetItemStatus (lst, j);
     }
  }
}

static void unmergelst (ButtoN b)
{
  DialogBoxDataPtr dbdp;

  dbdp = (DialogBoxDataPtr) GetObjectExtra (ParentWindow (b));
}
static void DismissGroupButton (ButtoN b)
{
  WindoW             wdialog;        
  DialogBoxDataPtr   dbdp;
  EditAlignDataPtr   adp;
  ValNodePtr         vnp;
  SeqParamPtr        prm;

  wdialog = ParentWindow (b);
  Hide (wdialog);
  Update();
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  {
     for (vnp = adp->params; vnp != NULL; vnp = vnp->next)
     {   
        prm = (SeqParamPtr) vnp->data.ptrvalue;
        prm->group = 0;
     }
     adp->ngroup = 0;
  }  
  Remove (wdialog);
  return;
}

static void AcceptGroupButton (ButtoN b)
{
  WindoW             w, wdialog;        
  WindoW             temport;
  DialogBoxDataPtr   dbdp;
  SeqEditViewFormPtr wdp;

  wdialog = ParentWindow (b);
  Hide (wdialog);
  Update();
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  w = dbdp->w;
  wdp = (SeqEditViewFormPtr) GetObjectExtra (w);
  temport = SavePort(w);
  Select (wdp->pnl);
  inval_panel (wdp->pnl, -1 ,-1);
  RestorePort (temport);
  Update ();
  Remove (wdialog);
  Update ();
  return;
}
/******************************************************************/
extern void FormulaDialog (IteM i)
{
  WindoW           w, wdialog;
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  GrouP            g1, g2, g3;
  ButtoN           b1, b2, b3, b4, b5;
  PrompT           p;
  Int2             numberlines = 15;

  w = ParentWindow (i);
  wdialog = FixedWindow (-50, -33, -10, -10, " ", StdCloseWindowProc);
  dbdp = (DialogBoxDataPtr) MemNew (sizeof (DialogBoxData));
  SetObjectExtra (wdialog, (Pointer) dbdp, StdCleanupExtraProc);
  dbdp->w = w;
  if ( ( adp = GetAlignEditData (w) ) == NULL ) return;
 
  g1 = HiddenGroup (wdialog, 2, 0, NULL);

  g2 = HiddenGroup (g1, 0, 2, NULL);
  p = StaticPrompt (g2, "Select sequences", 0, dialogTextHeight, systemFont, 'c');         
  dbdp->lst1 = MultiList (g2, numberlines, 10, NULL);
  refreshlst (dbdp->lst1, adp);
  AlignObjects (ALIGN_CENTER, (HANDLE) dbdp->lst1, (HANDLE) p, NULL);

  g3 = HiddenGroup (g1, 0, 6, NULL);
  b1 = PushButton (g3, "Group", grouplst);
  b2 = PushButton (g3, "Ungroup", ungrouplst);
  b3 = PushButton (g3, "Merge", mergelst);
  b3 = PushButton (g3, "All visible", unmergelst);
  b4 = PushButton (g3, "Accept", AcceptGroupButton);
  b5 = PushButton (g3, "Dismiss", DismissGroupButton);
  AlignObjects (ALIGN_CENTER, (HANDLE) b1, (HANDLE) b2, (HANDLE) b3, (HANDLE) b4, NULL);

  RealizeWindow (wdialog);
  Show (wdialog);
  return;
}
/******************************************************************/
/******************************************************************/
static void BoolButton (GrouP g)
{
  WindoW           wdialog;        
  DialogBoxDataPtr dbdp;

  wdialog = ParentWindow (g);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  dbdp->db_bool = (Boolean)((GetValue (g)) == 1);
  return;
}

/*******************************************************
***
***   FIND functions: John Kuzio function
***
*******************************************************/
#define FIND_MAXRANGE 10000

extern ValNodePtr JK_NTPattern (CharPtr pattern, ValNodePtr sqloc_list, Boolean flagInvert, Uint1 mol_type)
{
  ValNodePtr      patlist = NULL;
  ValNodePtr      vnp;
  SeqLocPtr       slp, slptmp,
                  next;
  SeqIdPtr        sip;
  SeqIntPtr       sit;
  Uint1Ptr        sequence;
  ComPatPtr       cpp;
  Int4            from, to;
  Int4            new_lens;
  Int2            jk_moltype;
  SeqAlignPtr     salp;

  if (pattern == NULL)
     return NULL;
  CleanPattern (pattern);
  if (StringLen (pattern) == 0)
     return NULL;
  if (ISA_na(mol_type)) jk_moltype = 0;
  else jk_moltype = 1;
  cpp = CompilePattern (pattern, jk_moltype);
  if (cpp != NULL && flagInvert)
     cpp = InvertPattern (cpp);
  if (cpp == NULL)
     return NULL;
     
  for (vnp = sqloc_list; vnp != NULL; vnp = vnp->next)
  {
     slp = (SeqLocPtr)vnp->data.ptrvalue; 
     sip = SeqLocId(slp);
     from = SeqLocStart(slp);
     to = SeqLocStop(slp);
     sequence = (Uint1Ptr)load_seq_data (sip, from, to, (Boolean)jk_moltype, &new_lens);
     if (sequence != NULL) {
        salp = PatternMatch (sequence, 0, Seq_strand_plus, sip, cpp, 0, 0, TRUE);
        if (salp != NULL) {
           slp = MatchSa2Sl (&salp);
           slptmp=slp;
           while (slptmp!=NULL) {
              next = slptmp->next;
              slptmp->next = NULL;
              if (from > 0)
              {
                 sit = (SeqIntPtr)slptmp->data.ptrvalue;
                 sit->from += from;
                 sit->to += from;
              }
              ValNodeAddPointer (&patlist, 0, (Pointer)slptmp);
              slptmp = next;
           }
        }
        MemFree (sequence);
     }
   }
  cpp = ComPatFree (cpp);
  return patlist;
}

typedef struct ccid {
  SeqIdPtr    sip;
  SeqEntryPtr sep;
} CcId, PNTR CcIdPtr;
 
static void FindSeqEntryForSeqIdCallback (SeqEntryPtr sep, Pointer mydata,
                                          Int4 index, Int2 indent)
{
  BioseqPtr          bsp;
  SeqIdPtr           sip;
  CcIdPtr            cip;
 
  if (sep != NULL && sep->data.ptrvalue && mydata != NULL) {
     cip = (CcIdPtr)mydata;
     if (IS_Bioseq(sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp!=NULL && ISA_na (bsp->mol)) {
           sip = SeqIdFindBest(bsp->id, 0);
           if (SeqIdForSameBioseq(cip->sip, sip))
              cip->sep = sep;
        }
     }   
  }
}
 
static Int2 CC_SeqEntryToGeneticCode (Uint2 entityID, SeqIdPtr sip)
{
  SeqEntryPtr sep_head,
              sep;
  CcId        ci;
  Int2        genCode = 0;

  sep_head  = GetTopSeqEntryForEntityID (entityID);
  ci.sip = SeqIdDup (sip);
  ci.sep = NULL;
  SeqEntryExplore(sep_head,(Pointer)&ci, FindSeqEntryForSeqIdCallback);
  sep = ci.sep;
  SeqIdFree (ci.sip);
  if (sep!=NULL)
     genCode = SeqEntryToGeneticCode (sep, NULL, NULL, 0);
  return genCode;
}

static CharPtr GetReverseString (CharPtr str)
{
  CharPtr new_str;
  Int4    len, idx;
  
  if (StringHasNoText (str)) return NULL;
  len = StringLen (str) + 1;
  new_str = (CharPtr) MemNew (len * sizeof (Char));
  if (new_str != NULL)
  {
    len -= 2;
    idx = 0;
    while (idx <= len)
    {
      new_str [idx] = str [len - idx];
      idx++;
    }
    new_str [idx] = 0;
  }
  return new_str;
}

extern ValNodePtr JK_NTPattern2Ex 
(CharPtr pattern, ValNodePtr sqloc_list, 
 Boolean flagInvert, Uint2 entityID, Uint1 pick_frame)
{
  ByteStorePtr    bsp;
  ValNodePtr      patlist = NULL;
  ValNodePtr      vnp;
  SeqAlignPtr     salp;
  SeqLocPtr       slp, slptmp,
                  next,
                  slp3;
  SeqLocPtr       slp_plus, slp_minus;
  SeqIdPtr        sip;
  Uint1Ptr        sequence;
  ComPatPtr       cpp;
  Int4            from, 
                  to;
  Int2            jk_moltype;
  Int2            genCode = Seq_code_ncbieaa;
  Uint1           frame, start_frame, stop_frame;
  CharPtr         rev_str;
  CharPtr         debug_str;
  Int4            minus_to, minus_from, prot_len, minus_offset;

  if (pattern == NULL)
    return NULL;
  CleanPattern (pattern);
  if (StringLen (pattern) == 0)
    return NULL;
  jk_moltype = 1;
  cpp = CompilePattern (pattern, jk_moltype);
  if (cpp != NULL && flagInvert)
    cpp = InvertPattern (cpp);
  if (cpp == NULL)
    return NULL;
     
  for (vnp = sqloc_list; vnp != NULL; vnp = vnp->next)
  {
    slp = (SeqLocPtr)vnp->data.ptrvalue; 
    sip = SeqLocId(slp);
    genCode = CC_SeqEntryToGeneticCode (entityID, sip);
    if (genCode == 0)
      genCode = Seq_code_ncbieaa;
    from = SeqLocStart(slp);
    to = SeqLocStop(slp);
    slp_plus = fuzz_loc (from, to, Seq_strand_plus, sip, TRUE, TRUE);
    slp_minus = fuzz_loc (from, to, Seq_strand_minus, sip, TRUE, TRUE);
    if (slp_plus != NULL && slp_minus != NULL) 
    {
      if (pick_frame < 1 || pick_frame > 6)
      {
        start_frame = 1;
        stop_frame = 6;
      }
      else
      {
        start_frame = pick_frame;
        stop_frame = pick_frame;
      }
      for (frame=start_frame; frame <= stop_frame; frame++) 
      {
        if (frame > 3)
        {
          bsp = cds_to_pept (slp_minus, frame - 3, genCode, TRUE);        
        }
        else
        {
          bsp = cds_to_pept (slp_plus, frame, genCode, TRUE);
        }
        if (bsp!=NULL) 
        {
          sequence = (Uint1Ptr) BSMerge (bsp, NULL);
          if (frame > 3)
          {
            rev_str = GetReverseString ((CharPtr)sequence);
            sequence = MemFree (sequence);
            sequence = (Uint1Ptr) rev_str;
          }
          debug_str = (CharPtr) sequence;
          BSFree (bsp);
          if (sequence != NULL) 
          {
            salp = PatternMatch (sequence, 0, Seq_strand_plus, sip, cpp, 0, 0, TRUE);
            if (salp!=NULL) 
            {
              slp = MatchSa2Sl (&salp);
              slptmp=slp;
              while (slptmp!=NULL) 
              {
                 next = slptmp->next;
                 slptmp->next = NULL;
                 if (frame > 3)
                 {
                   prot_len = SeqLocLen (slp_minus);
                   minus_offset = prot_len % 3;
                   if (frame == 4)
                   {
                     /* no shift for frame */
                   }
                   else if (frame == 5)
                   {
                     minus_offset --;
                   }
                   else if (frame == 6)
                   {
                     minus_offset -=2;
                   }
                   minus_from = 3*SeqLocStart(slptmp) + from + minus_offset;
                   minus_to = 3*SeqLocStop(slptmp) + from + minus_offset;
                   slp3 = SeqLocIntNew (minus_from, minus_to, SeqLocStrand(slptmp), SeqLocId(slptmp));
                 }
                 else
                 {
                   slp3 = SeqLocIntNew ((Int4)(from + 3*SeqLocStart(slptmp)+frame-1), (Int4)(from + 3*SeqLocStop(slptmp)+frame+1), SeqLocStrand(slptmp), SeqLocId(slptmp));
                 }
                 ValNodeFree (slptmp);
                 ValNodeAddPointer (&patlist, 0, (Pointer)slp3);
                 slptmp = next;
              }
            }
            MemFree (sequence);
          }
        }
      }
      ValNodeFree (slp_plus);
      ValNodeFree (slp_minus);
    }
  }
  cpp = ComPatFree (cpp);
  return patlist;
}

extern ValNodePtr JK_NTPattern2 (CharPtr pattern, ValNodePtr sqloc_list, Boolean flagInvert, Uint2 entityID)
{
  return JK_NTPattern2Ex (pattern, sqloc_list, flagInvert, entityID, 0);
}

/******************************************************************/
static CharPtr get_string_fromtext (TexT txt)
{ 
  Char               str [128];
  CharPtr            strp;
  Int4               lens;
  Int4               j;

  GetTitle (txt, str, sizeof(str)-1);
  if (str == NULL)
     return NULL;
  lens = StringLen(str);
  if (lens < 1)
     return NULL;
  for (j=0, strp = str; j<lens; j++, strp++) {
     *strp = TO_UPPER(*strp);
  }
  strp = (CharPtr) MemNew ((StringLen(str)+4) * sizeof (Char));
  StringCpy (strp, str);
  return strp;
}

static void FindPatternButton (ButtoN b)
{
  WindoW             wdialog;
  WindoW             temport;
  DialogBoxDataPtr   dbdp;
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr   adp;
  ValNodePtr         vnp;
  SelStructPtr       ssp;
  Char               str[128];
  CharPtr            strp;
  SeqParamPtr        prm;
  Uint1              frame;
  Int2               count = 0;
  Boolean            invert= FALSE,
                     translate= FALSE;
 
  wdialog = ParentWindow (b);
/****************
  Hide (wdialog);
*****************/
  Update();
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  strp = get_string_fromtext (dbdp->txt1);
  if (strp == NULL) {
     SetTitle (dbdp->prompt, "No item found");
     return;
  }
  wdp = (SeqEditViewFormPtr) GetObjectExtra (dbdp->w);
  if ((adp = GetAlignEditData (dbdp->w)) == NULL )
     return;
  if (adp->current_pattern != NULL) {
     if ((StringStr(adp->current_pattern, strp)) && (StringLen(adp->current_pattern) == StringLen (strp)))
     {
        MemFree (strp);
        adp->cur_pat = ShowNextPattern (adp->match_pat, adp->cur_pat, adp->edit_pos);
        return;
     }
     else {
        MemFree (adp->current_pattern);
     }
  }
  adp->current_pattern = strp;
  if (adp->match_pat != NULL)
     adp->match_pat = SelStructDelList (adp->match_pat);
  if (dbdp->bt!=NULL)
     invert = (Boolean) GetStatus (dbdp->bt);
  if (dbdp->bt2!=NULL)
     translate = (Boolean) GetStatus (dbdp->bt2);
  if (translate && !invert) {
     vnp = JK_NTPattern2 (strp, adp->sqloc_list, invert, adp->input_entityID);
  }
  else 
     vnp = JK_NTPattern (strp, adp->sqloc_list, invert, adp->mol_type);
  if (vnp != NULL) {
     adp->match_pat = SetupMatchPatList (vnp, adp->anp_list);
     for (ssp=adp->match_pat; ssp!=NULL; ssp=ssp->next)
        count++;
     if (count ==0)
        SetTitle (dbdp->prompt, "No item found");
     else if (count ==1) {
        SetTitle (dbdp->prompt, "1 item found");
     } else {
        sprintf (str, "%d items found   -   next=CTRL->, previous=CTRL<- ", (int) count);
        SetTitle (dbdp->prompt, str);
     }
     if (translate) {
        for (frame=0; frame<10; frame++) {
           SetStatus (wdp->rfitem [frame], FALSE);
           for (vnp = adp->params; vnp != NULL; vnp = vnp->next) {
              prm = (SeqParamPtr) vnp->data.ptrvalue;
              prm->rf[frame] = FALSE;
           }  
        }
        for (ssp=adp->match_pat; ssp!=NULL; ssp=ssp->next) {
           frame = SeqLocStart((SeqLocPtr)ssp->region)%3;
           if (frame <8) {
              SetStatus (wdp->rfitem [frame], TRUE);
              for (vnp = adp->params; vnp != NULL; vnp = vnp->next) {
                 prm = (SeqParamPtr) vnp->data.ptrvalue;
                 prm->rf[frame] = TRUE;
              }
           }
        }   
        do_resize_window (wdp->pnl, adp, TRUE);
        temport = SavePort(dbdp->w);
        Select (wdp->pnl);
        inval_panel (wdp->pnl, -1 ,-1);
        RestorePort (temport);
        Update ();
     }
     adp->cur_pat = ShowNextPattern (adp->match_pat, NULL, adp->edit_pos);
  }
  else {
     SetTitle (dbdp->prompt, "No item found");
  }
/*****************
  Remove (wdialog);
******************/
  Update ();
  return;
}

extern void FindPatternDialog (IteM i)
{
  WindoW             w, wdialog;
  DialogBoxDataPtr   dbdp;
  EditAlignDataPtr   adp;
  GrouP              g1, g2, g3;

  w = ParentWindow (i);
  if ( ( adp = GetAlignEditData (w) ) == NULL ) return;
  wdialog=FixedWindow (-50, -33, -10, -10, "Find", StdCloseWindowProc);
  dbdp = (DialogBoxDataPtr) MemNew (sizeof (DialogBoxData));
  SetObjectExtra (wdialog, (Pointer) dbdp, StdCleanupExtraProc);
  dbdp->w = w;
  dbdp->bt = NULL;
  dbdp->bt2 = NULL;

  g1 = HiddenGroup (wdialog, 1, 0, NULL);
  StaticPrompt (g1, "Find pattern:", 0, popupMenuHeight, programFont, 'l');

  g1 = HiddenGroup (wdialog, 1, 0, NULL);
  if (adp->current_pattern == NULL) {
     dbdp->txt1 = ScrollText (g1, 25, 8, programFont, TRUE, NULL);
  } else {
     dbdp->txt1 = ScrollText (g1, 25, 8, programFont, TRUE, NULL);
  } 
  g3 = HiddenGroup (wdialog, 1, 0, NULL);
  dbdp->prompt = StaticPrompt (g3, "", (Int2)(25*stdCharWidth), stdLineHeight, programFont, 'l'); 
  if (ISA_na(adp->mol_type))
  {
     g2 = HiddenGroup (wdialog, 4, 0, NULL);
     dbdp->bt = CheckBox (g2, "reverse complement", NULL);  
     SetStatus (dbdp->bt, FALSE);
     dbdp->bt2 = CheckBox (g2, "translate sequence", NULL);
     SetStatus (dbdp->bt2, FALSE); 
     PushButton (g2, "Find Next", FindPatternButton);
     PushButton (g2, "Dismiss", StdCancelButtonProc);
  }
  else {
     g2 = HiddenGroup (wdialog, 2, 0, NULL);
     PushButton (g2, "Find Next", FindPatternButton);
     PushButton (g2, "Close", StdCancelButtonProc);
  }
  RealizeWindow (wdialog);
  Show (wdialog);
  return;
}

/******************************************************************/
static void GetInt4Value (TexT text)
{
  WindoW           wdialog;
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  Char             str[16];
 
  wdialog = ParentWindow (text);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ((adp = GetAlignEditData (dbdp->w)) == NULL ) return;
  GetTitle (text, str, 16);
  adp->int4value = (Int2) atoi (str);
  return;
}

static void GetInt4Value2 (TexT text)
{
  WindoW           wdialog;
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  Char             str[16];
 
  wdialog = ParentWindow (text);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ((adp = GetAlignEditData (dbdp->w)) == NULL ) return;
  GetTitle (text, str, 16);
  adp->int4value2 = (Int2) atoi (str);
  return;
}
/******************************************************************
***
***   DOT MATRIX
***
******************************************************************/
static int LIBCALLBACK callback(SeqAlignPtr seqalign)
{
  if (seqalign != NULL)
     LaunchAlignEditor (seqalign);
  return 1;
}
 
static void DotPlotBtn (ButtoN b)
{
  WindoW             wdialog;
  DialogBoxDataPtr   dbdp;
  EditAlignDataPtr   adp;
  SeqEntryPtr        sep;
  BioseqPtr          bsp;
  ValNodePtr         vnp;
  SeqLocPtr          slp = NULL;
  SeqIdPtr           sipnew = NULL;
  Int2               status;

  wdialog = ParentWindow (b);
  Hide (wdialog);
  Update();
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  if (ISA_na(adp->mol_type) && adp->seqnumber == 1) {
     if (adp->align_format == SALSA_FASTA)
        sep = FastaRead (NULL, Seq_mol_na);
     else if (adp->align_format == SALSA_ASN1) {
        sep = AsnReadForSalsa (NULL);
        sep = getfirst_sep (sep, adp->mol_type);
     }   
     if (sep != NULL && IS_Bioseq (sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        sipnew = SeqIdFindBest(bsp->id, 0);   
        vnp = adp->sqloc_list;
        if (vnp!=NULL) {
           slp = (SeqLocPtr) vnp->data.ptrvalue;
           WatchCursor ();
           status = DotMatrixSearch(SeqLocId(slp), sipnew, callback);
           ArrowCursor ();
        }
     }
  } 
  else
     ErrPostEx (SEV_ERROR, 0, 0, "There should be 1 sequence in the editor");
  Remove (wdialog);
  Update ();
}

extern void DotPlotDialog (IteM i)
{
  WindoW           w, wdialog;
  DialogBoxDataPtr dbdp;
  GrouP            g1, g2;
  ButtoN           bt1, bt2;

  w = ParentWindow (i);
  wdialog = FixedWindow (-50, -33, -10, -10, "Dot Matrix", StdCloseWindowProc);
  dbdp = (DialogBoxDataPtr) MemNew (sizeof (DialogBoxData));
  SetObjectExtra (wdialog, (Pointer) dbdp, StdCleanupExtraProc);
  dbdp->w = w;
  g1 = HiddenGroup (wdialog, 3, 0, NULL);
  StaticPrompt (g1, "Input format:", 0, popupMenuHeight, systemFont, 'l');
  bt1 = RadioButton (g1, "Fasta");
  bt2 = RadioButton (g1, "ASN.1");
  Disable (bt2);
  SetValue (g1, 2);

  g2 = HiddenGroup (wdialog, 4, 0, NULL);
  PushButton (g2, "Process", DotPlotBtn);
  PushButton (g2, "Dismiss", StdCancelButtonProc);
  RealizeWindow (wdialog);
  Show (wdialog);
  return;
}

/******************************************************************/
extern void DotPlotItem (IteM i)
{
  EditAlignDataPtr   adp;
  SelStructPtr       ssp;
  AlignNodePtr       anp;
  SeqLocPtr          slp;
  SeqIdPtr           sip1 = NULL, 
                     sip2 = NULL;
  Int2               status;
  Int2               j;

  adp = GetAlignEditData ((WindoW) ParentWindow (i));
  if (adp->seqnumber == 0 ) return;
  if (GetNumberObjMgrSelect() != 2) {
     Message (MSG_OK, "Dot Matrix: Please select 2 sequences");
     return;
  }
  for (j=0, ssp = ObjMgrGetSelected(); j<2 && ssp!= NULL; ssp = ssp->next)
  {
     if ( checkssp_for_editor (ssp) ) {
        anp = (AlignNodePtr) AlignNodeFind (adp->anp_list, ssp->entityID, ssp->itemID, ssp->itemtype);
        if ( anp != NULL )
        {
           slp = CollectSeqLocFromAlignNode (anp);
           if (j==0) sip1 = SeqLocId (slp);
           else sip2 = SeqLocId (slp);
        }
     }
     j++;
  }
  if (sip1!=NULL && sip2!=NULL) {
     WatchCursor ();
     status = DotMatrixSearch(sip1, sip2, callback);
     ArrowCursor ();
  }
}

/******************************************
***  EXPORT FUNCTION
*******************************************/
static Boolean FindBspFromItem (GatherContextPtr gcp)

{
  BioseqPtr  PNTR bspp;

  bspp = (BioseqPtr PNTR) gcp->userdata;
  if (bspp != NULL && gcp->thistype == OBJ_BIOSEQ) {
    *bspp = (BioseqPtr) gcp->thisitem;
  }
  return TRUE;
}

/* weird little kludge so that FASTA+GAP exports can contain */
/* leading/trailing gaps to include the entirety of all */
/* sequences */
static SeqAlignPtr ExtendToAllSeq(SeqAlignPtr origsap)
{
   MsgAnswer    ans;
   BioseqPtr    bsp;
   Int4         i;
   Int4         n;
   Int4         pad;
   SeqAlignPtr  sap;
   SeqAlignPtr  sap_new;
   SeqIdPtr     sip;
   Int4         start;
   Int4         stop;

   if (origsap == NULL)
      return NULL;
   sap = SeqAlignBoolSegToDenseSeg(origsap);
   AlnMgr2IndexSingleChildSeqAlign(sap);
   n = AlnMgr2GetNumRows(sap);
   pad = 0;
   for (i=0; i<n; i++)
   {
      sip = AlnMgr2GetNthSeqIdPtr(sap, i+1);
      bsp = BioseqLockById(sip);
      if (bsp != NULL) {
        AlnMgr2GetNthSeqRangeInSA(sap, i+1, &start, &stop);
        pad += start;
        pad += bsp->length - 1 - stop;
        BioseqUnlock(bsp);
      }
      SeqIdFree(sip);
   }
   if (pad <= 0)
   {
      SeqAlignFree(sap);
      return origsap;
   }
   ans = Message(MSG_OKC, "This alignment does not contain the entirety of all of your sequences.\nDo you want to pad the alignment by %d nucleotides and gaps\nso that all sequences start with 1 and end with their last residue?", pad);
   if (ans == ANS_CANCEL)
   {
      SeqAlignFree(sap);
      return origsap;
   } else
   {
      sap_new = AlnMgr2PadConservatively(sap);
      SeqAlignFree(sap);
      sap = SeqAlignDenseSegToBoolSeg(sap_new);
      SeqAlignFree(sap_new);
      return sap;
   }
}

extern void ExportTextFunc (ButtoN b)
{
  WindoW           wdialog;
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  Char             namep [PATH_MAX];
  SelStructPtr     ssp,
                   ssphead = NULL;
  BioseqPtr        bsp;
  SeqAnnotPtr      sap;
  SeqAlignPtr      salp;
  SeqLocPtr        slp;
  AlignNodePtr     anp;
  ValNodePtr       vnp;
  Int4             start1, stop1;
  Int4             start=-1, 
                   stop=-1;
  FILE             *fout;

  wdialog = ParentWindow (b);
  Hide (wdialog);
  Update();
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  if (!GetOutputFileName (namep, PATH_MAX, NULL))
     return;
  WatchCursor ();
  ssphead = NULL;
  if ( checkOMss_for_itemtype (OBJ_BIOSEQ) != 0 ) {
     ssphead = (SelStructPtr)ObjMgrGetSelected ();
  }
  start1 = adp->int4value - 1;
  stop1 = adp->int4value2 - 1;   
  if (adp->align_format == SALSA_ASN1) {
     sap = SeqAnnotBoolSegToDenseSeg (adp->sap_align);
     if (sap != NULL) {
        seqannot_write (sap, namep);
        SeqAnnotFree (sap);
     }
     return;
  }
  fout = FileOpen (namep, "w");
  if (fout != NULL) {
        if (adp->align_format == SALSA_SHWTXT) {
           ShowAlignmentText (fout, adp, ssphead, adp->marginleft, start1, stop1, FALSE);
        }
        else if (adp->align_format == SALSA_FASTA) {
           salp = (SeqAlignPtr)adp->sap_align->data;
           if (ssphead==NULL) {
              for (vnp = adp->anp_list; vnp != NULL; vnp = vnp->next) {
                 if ( (anp = (AlignNodePtr) vnp->data.ptrvalue) != NULL)
                 {
                    GatherItem (anp->seq_entityID, anp->bsp_itemID, OBJ_BIOSEQ, (Pointer) (&bsp), FindBspFromItem);
                    if (bsp!=NULL) {
                       start = AlignCoordToSeqCoord (start1, bsp->id, salp, adp->sqloc_list, 0);
                       stop = AlignCoordToSeqCoord (stop1, bsp->id, salp, adp->sqloc_list, 0);
                       EditBioseqToFasta (bsp, fout, start, stop);
                    }
                 }
              }
           } else for (ssp=ssphead;ssp!=NULL; ssp=ssp->next) {
              slp = (SeqLocPtr)ssp->region;
              if (slp!=NULL) {
                 if (GetAlignCoordFromSeqLoc(slp, salp, &start, &stop)) {
                    GatherItem (ssp->entityID, ssp->itemID, ssp->itemtype, (Pointer) (&bsp), FindBspFromItem);
                    if (bsp != NULL) {
                       start = AlignCoordToSeqCoord (start1, bsp->id, salp, adp->sqloc_list, 0);
                       stop = AlignCoordToSeqCoord (stop1, bsp->id, salp, adp->sqloc_list, 0);
                       EditBioseqToFasta (bsp, fout, start, stop);
                    }
                 }
              }
           }
        }
        else if (adp->align_format == SALSA_FASTGAP) {
           salp = (SeqAlignPtr)adp->sap_align->data;
           salp = ExtendToAllSeq(salp);
           showfastagap_fromalign (salp, 60, fout);
        }
        else if (adp->align_format == SALSA_PHYLIP) {
           ShowAlignmentText (fout, adp, ssphead, 11, start1, stop1, FALSE);
        }
        FileClose (fout);
  }
  ArrowCursor ();
  Remove (wdialog);
  Update ();
}
 
extern void SalsaExportDialog (IteM i)
{
  WindoW           w, wdialog;
  DialogBoxDataPtr dbdp;
  GrouP            g;
  EditAlignDataPtr adp;
  Char             str [255], str2[10];
  CharPtr          tmp;
  SelStructPtr     ssp = NULL,
                   ssphead = NULL;
  ValNodePtr       vnp;
  AlignNodePtr     anp;
  Int4             j;
  Int2             selected_seq = 0;
  Boolean          first = TRUE;
 
  w = ParentWindow (i);
  if ( ( adp = GetAlignEditData (w) ) == NULL ) return;
  if ( checkOMss_for_itemtype (OBJ_BIOSEQ) == 0 ) {
     for (vnp = adp->anp_list; vnp != NULL; vnp = vnp->next) {
         if ( (anp = (AlignNodePtr) vnp->data.ptrvalue) != NULL)
         {
            ssp=SelStructNew(anp->seq_entityID, anp->bsp_itemID, OBJ_BIOSEQ, 0, 0, anp->sip, 0, FALSE);
            ssphead = (SelStructPtr)SelStructAdd(ssphead, ssp);
         }
     }
  } else {
     ssphead = (SelStructPtr)ObjMgrGetSelected ();
     selected_seq = checkOMss_for_itemtype (OBJ_BIOSEQ);
  }
  if (ssphead ==NULL) {
     return;
  }
  wdialog = FixedWindow (-50, -33, -10, -10, "Export", StdCloseWindowProc);
  dbdp = (DialogBoxDataPtr) MemNew (sizeof (DialogBoxData));
  SetObjectExtra (wdialog, (Pointer) dbdp, StdCleanupExtraProc);
  dbdp->w = w;
  g = HiddenGroup (wdialog, 2, 0, NULL);
  StaticPrompt (g, "Export:  ", 0, popupMenuHeight, systemFont, 'l');
  ssp=ssphead;
  while (first && ssp!=NULL) {
     tmp = str;
     for (j=0; j<10 && ssp!=NULL; j++, ssp=ssp->next) {
        SeqIdWrite (SeqLocId((SeqLocPtr)ssp->region), str2, PRINTID_REPORT, sizeof (str2));
        tmp = StringMove (tmp, str2);
        tmp = StringMove (tmp, " ");
     }
     if (ssp==NULL && !first)
        tmp = StringMove (tmp, " ...");
     if (!first)
        g = HiddenGroup (wdialog, 1, 0, NULL);
     else first = FALSE;
     StaticPrompt (g, str, 0, popupMenuHeight, programFont, 'l');
  }
  if (selected_seq == 1) {
     adp->int4value = SeqLocStart((SeqLocPtr)ssphead->region) +1;
     adp->int4value2 = SeqLocStop((SeqLocPtr)ssphead->region) +1;
  }
  else {
     adp->int4value = 1;
     adp->int4value2= adp->length;
  }
  if (selected_seq == 0)
     ssphead = SelStructDelList (ssphead);
  g = HiddenGroup (wdialog, 5, 0, NULL);
  StaticPrompt (g, "Positions:  ", 0, popupMenuHeight, systemFont, 'l');
  StaticPrompt (g, "from", 0, popupMenuHeight, programFont, 'l');
  sprintf (str2, "%ld", (long) adp->int4value);
  DialogText (g, str2, (Int2)6, GetInt4Value);
  StaticPrompt (g, "to", 0, popupMenuHeight, programFont, 'l');
  sprintf (str2, "%ld", (long) adp->int4value2);
  DialogText (g, str2, (Int2)6, GetInt4Value2);
  g = HiddenGroup (wdialog, 2, 0, NULL);
  PushButton (g, "Proceed", ExportTextFunc); 
  PushButton (g, "Cancel", StdCancelButtonProc);
  RealizeWindow (wdialog);
  Show (wdialog);
  return;
}
/******* ???????????
static void SelectVisibleSeq (CharPtr str, Int4 start, Int4 stop, Boolean show, EditAlignDataPtr adp)
{
  SeqParamPtr      prm;
  ValNodePtr       vnp,
                   curr;
  AlignNodePtr     anp;
  CharPtr          tmp,
                   tmp2;
  Int4             j=0;

  curr = adp->anp_list;
  for (vnp = adp->params; vnp != NULL; vnp = vnp = vnp->next) {
     anp = (AlignNodePtr) curr->data.ptrvalue;
     tmp = load_seq_data (anp->sip, start-1, stop+1, (Boolean)ISA_aa(adp->mol_type), &j);
     tmp2 = StringStr(tmp, str);
     if (show && tmp2==NULL) {
        prm = (SeqParamPtr) vnp->data.ptrvalue;
     }
     else if (!show && tmp2!=NULL) {
        prm = (SeqParamPtr) vnp->data.ptrvalue;
     }
     curr = curr->next;
  }

  return; 
}
******/

static void SelectSeqFunc (ButtoN b)
{
  WindoW           wdialog;
  WindoW           temport;
  DialogBoxDataPtr dbdp;
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr adp;
  SelStructPtr     ssp;
  SeqLocPtr        slp;
  CharPtr          str;
  Int4             j;

  wdialog = ParentWindow (b);
  Hide (wdialog);
  Update();
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  wdp = (SeqEditViewFormPtr) GetObjectExtra (dbdp->w);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  ssp = (SelStructPtr) adp->extra_data;
  if (ssp==NULL)
     return;
  slp = (SeqLocPtr) ssp->region;
  if (slp==NULL) {
     adp->extra_data = NULL;
     return;
  }
  str = load_seq_data (SeqLocId(slp), SeqLocStart(slp), SeqLocStop(slp), (Boolean)ISA_aa(adp->mol_type), &j);
  adp->extra_data = NULL;
  /*SelectVisibleSeq(str,  SeqLocStart(slp), SeqLocStop(slp), dbdp->bool, adp);*/
  data_collect_arrange (adp, TRUE);
  temport = SavePort (wdp->pnl);
  Select (wdp->pnl);
  inval_panel (wdp->pnl, -1, -1);
  RestorePort (temport);
  Update ();
  Remove (wdialog);
  Update ();
  ArrowCursor();
  return;
}

static void HighlightSelectionFunc (ButtoN b)
{
  WindoW           wdialog;
  DialogBoxDataPtr dbdp;
  SeqEditViewFormPtr wdp;
  EditAlignDataPtr adp;
  SelStructPtr     ssp;
  Int4             j;
  Uint1Ptr         rgb;

  ValNodePtr       vnp;
  SeqLocPtr        slp;
  AlignNodePtr     anp;
  CharPtr          str,
                   tmp,
                   tmp2;
  SeqAlignPtr      salp;
  Int4             start, stop;
  Int4             start2, stop2;
  Int4             starta, stopa;
  Boolean          show;

  rgb=(Uint1Ptr)MemNew((size_t)(3*sizeof(Uint1)));
  rgb[0]=0;
  rgb[1]=255;
  rgb[2]=0;
  wdialog = ParentWindow (b);
  Hide (wdialog);
  Update();
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  wdp = (SeqEditViewFormPtr) GetObjectExtra (dbdp->w);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  ssp = (SelStructPtr) adp->extra_data;
  if (ssp==NULL)
     return;
  slp = (SeqLocPtr) ssp->region;
  if (slp==NULL) {
     adp->extra_data = NULL;
     return;
  }
  start = SeqLocStart(slp);
  stop = SeqLocStop(slp);
  str = load_seq_data (SeqLocId(slp), start, stop, (Boolean)ISA_aa(adp->mol_type), &j);
  adp->extra_data = NULL;
  show = dbdp->db_bool;
/*
  ObjMgrDeferUpdate ();
*/
  salp=(SeqAlignPtr) adp->sap_align->data;
  starta= SeqCoordToAlignCoord (start, SeqLocId(slp), salp, 0, 0);
  stopa = SeqCoordToAlignCoord (stop, SeqLocId(slp), salp, 0, 0);
  for (vnp = adp->anp_list; vnp != NULL; vnp = vnp->next) {
     anp = (AlignNodePtr) vnp->data.ptrvalue;
     start2 = AlignCoordToSeqCoord (starta, anp->sip, salp, adp->sqloc_list, 0);
     stop2 = AlignCoordToSeqCoord (stopa, anp->sip, salp, adp->sqloc_list, 0); 
     tmp = load_seq_data (anp->sip, start2-1, stop2+1, (Boolean)ISA_aa(adp->mol_type), &j);
     tmp2 = StringStr(tmp, str);
     if (show && tmp2!=NULL) {
        slp = SeqLocIntNew (start2, stop2, Seq_strand_plus, anp->sip);
        ObjMgrSetColor (anp->seq_entityID, anp->bsp_itemID, OBJ_BIOSEQ, OM_REGION_SEQLOC, (Pointer)slp, rgb);  
        SeqLocFree (slp);
     }
     else if (!show && tmp2==NULL) {
        slp = SeqLocIntNew (start2, stop2, Seq_strand_plus, anp->sip);
        ObjMgrSetColor (anp->seq_entityID, anp->bsp_itemID, OBJ_BIOSEQ, OM_REGION_SEQLOC, (Pointer)slp, rgb);  
        SeqLocFree (slp);
     }
  }
/*
  EnableUpdates ();
*/
  return;
}

extern void SelectSeqDialog (IteM i)
{
  WindoW           w, wdialog;
  DialogBoxDataPtr dbdp;
  GrouP            g,
                   g2, g3, g4, g5;
  EditAlignDataPtr adp;
  Char             str [64]; 
  CharPtr          tmp;
  SelStructPtr     ssphead = NULL;
  SeqLocPtr        slp;
  Int4             j;
 
  ssphead = (SelStructPtr)ObjMgrGetSelected ();
  if (ssphead == NULL)
     return;
  if (ssphead->region == NULL)
     return;
  w = ParentWindow (i);
  if ( ( adp = GetAlignEditData (w) ) == NULL ) return;
  str[0] = '\0';
  tmp = str;
  adp->int4value = -1;
  adp->int4value2 = -1;
  slp = (SeqLocPtr)ssphead->region;
  tmp = load_seq_data (SeqLocId(slp), SeqLocStart(slp), SeqLocStop(slp), (Boolean)ISA_aa(adp->mol_type), &j);
  adp->int4value = SeqLocStart(slp) +1;
  adp->int4value2 = SeqLocStop(slp) +1;
  adp->extra_data = (Pointer) ssphead;

  wdialog = FixedWindow (-50, -33, -10, -10, "Export", StdCloseWindowProc);
  dbdp = (DialogBoxDataPtr) MemNew (sizeof (DialogBoxData));
  SetObjectExtra (wdialog, (Pointer) dbdp, StdCleanupExtraProc);
  dbdp->w = w;
  dbdp->db_bool = TRUE;

  g = HiddenGroup (wdialog, 3, 0, NULL);
  StaticPrompt(g, "Select the sequences ", 0, popupMenuHeight, systemFont,'l');

  g2 = HiddenGroup (g, 2, 0, BoolButton);
  RadioButton (g2, "showing");
  RadioButton (g2, "not showing");
  SetValue (g2, 1);

  g3 = HiddenGroup (wdialog, 2, 0, NULL);
  sprintf (str, "the pattern  %s", tmp);
  StaticPrompt (g3, str, 0, popupMenuHeight, systemFont, 'l');

  g4 = HiddenGroup (wdialog, 5, 0, NULL);
  sprintf(str, "from position  %d  to  %d", adp->int4value, adp->int4value2);
  StaticPrompt (g4, str, 0, popupMenuHeight, systemFont, 'l');

  g5 = HiddenGroup (wdialog, 3, 0, NULL);
  PushButton (g5, "Show", HighlightSelectionFunc); 
  PushButton (g5, "Select", SelectSeqFunc); 
  PushButton (g5, "Cancel", StdCancelButtonProc);
  RealizeWindow (wdialog);
  Show (wdialog);
  return;
}

static void SelectRegionFunc (ButtoN b)
{
  WindoW           w;      /* window from where the region is selected */
  WindoW           wdialog;/* window of the dialog box */
  DialogBoxDataPtr dbdp;   /*        extra data */
  EditAlignDataPtr adp;
  SeqAlignPtr      salp1,
                   salp2;
  Char             str[16];
  Int4             left, right;

  wdialog = (WindoW) ParentWindow (b);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  w = dbdp->w;
  if ( ( adp = GetAlignEditData (w) ) == NULL ) return;
  GetTitle (dbdp->txt2, str, 16);
  left = (Int4) (atol (str) - 1);
  GetTitle (dbdp->txt3, str, 16);
  right = (Int4) (atol (str)-1);
  if ((left > 0 || right < adp->length) && left < right-2)
  {
     Hide (wdialog);
     Update();
     if ( adp->seqnumber == 0 ) return;
     salp1 = SeqAlignBoolSegToDenseSeg ((SeqAlignPtr) adp->sap_align->data); 
     if (salp1) 
     {
        salp2 = SeqAlignTrunc (salp1, left, right);
        if (salp2)
           LaunchAlignEditor (salp2);
        else 
           SeqAlignSetFree (salp1);
     }
     Remove (wdialog);
  }
  return;    
}


extern void SelectRegionDialog (IteM i)
{
  WindoW           w, wdialog;
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  GrouP            g;
  Char             str[16];
 
  w = (WindoW) ParentWindow (i);
  wdialog = FixedWindow (-50, -33, -10, -10, "Sequence", NULL);
  dbdp = (DialogBoxDataPtr) MemNew (sizeof (DialogBoxData));
  SetObjectExtra (wdialog, (Pointer) dbdp, StdCleanupExtraProc);
  dbdp->w = w;
  if ( ( adp = GetAlignEditData (w) ) == NULL ) return;

  g = HiddenGroup (wdialog, 1, 0, NULL);
  StaticPrompt (g, "Select positions in the alignment:", 0, dialogTextHeight, systemFont, 'l');

  g = HiddenGroup (wdialog, 4, 0, NULL);
  StaticPrompt (g, "from ", 0, dialogTextHeight, systemFont, 'l');
  sprintf (str, "%ld", (long) ( adp->gr.left + 1 ));
  dbdp->txt2 = DialogText (g, str, 8, NULL);
  StaticPrompt (g, "to ", 0, dialogTextHeight, systemFont, 'l');
  sprintf (str, "%ld", (long) adp->gr.right);
  dbdp->txt3 = DialogText (g, str, 8, NULL);

  g = HiddenGroup (wdialog, 2, 0, NULL);
  PushButton (g, "OK", SelectRegionFunc);
  PushButton (g, "Cancel", StdCancelButtonProc);
  Show (wdialog);
  return;
}

extern void ColorIdentityDialog (WindoW w)
{
  WindoW           wdialog;
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  GrouP            g,
                   g2;
  Char             str[16];
  Int2             j, k, x;
 
  wdialog = FixedWindow (-50, -33, -10, -10, "Color Identity", NULL);
  dbdp = (DialogBoxDataPtr) MemNew (sizeof (DialogBoxData));
  SetObjectExtra (wdialog, (Pointer) dbdp, StdCleanupExtraProc);
  dbdp->w = w;
  if ( ( adp = GetAlignEditData (w) ) == NULL ) return;
  g = HiddenGroup (wdialog, 1,0,NULL);
  StaticPrompt (g, "Select a color for range of conservation", 0, dialogTextHeight, programFont, 'l');
  x = 0;
  g = HiddenGroup (wdialog, 6, 5, NULL);
  for (j=0; j<100; j += 20) {
     for (k=0; k<2; k++) {
        if (k==0)
           sprintf (str, "%d - %d%c", j, j+10, '%');
        else 
           sprintf (str, "  %d - %d%c", j+10, j+20, '%');
        StaticPrompt (g, str, 0, dialogTextHeight, programFont, 'l');

        StaticPrompt (g, "[color]", 0, dialogTextHeight, programFont, 'l'); 
        dbdp->color[x] = PopupList (g, TRUE, NULL);
        PopupItem (dbdp->color[x], "none");
        PopupItem (dbdp->color[x], "new");
        PopupItem (dbdp->color[x], "marine");
        PopupItem (dbdp->color[x], "blue");
        PopupItem (dbdp->color[x], "red");
        PopupItem (dbdp->color[x], "yellow");
        PopupItem (dbdp->color[x], "green");
        SetValue  (dbdp->color[x], adp->popcolor[x]);
        x++;
     } 
  }
  g2 = HiddenGroup (wdialog, 2, 0, NULL);
  PushButton (g2, "Accept", StdCancelButtonProc);
  PushButton (g2, "Dismiss", StdCancelButtonProc);
  Show (wdialog);
  return;
}

extern void ColorIdentityDialogItem (IteM i)
{
  ColorIdentityDialog ((WindoW) ParentWindow (i));
}


/**********************************************************
***
*** Propagate from list of features
*** to one or more Bioseqs
***
***********************************************************/

static void PropagateFeatureProc (ButtoN b)
{
  WindoW           wdialog;
  PaneL            pnl;
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  SeqAnnotPtr      sap;
  SeqAlignPtr      salp;
  SelEdStructPtr   feat,
                   sesp;
  SeqLocPtr        slp;
  SeqIdPtr         sip_source;
  ValNodePtr       vnp,
                   vnpf,
                   vnpfeat = NULL;
  AlignNodePtr     anp;
  Uint2            eID, iID,
                   subtype;
  Int2             j, jmax,
                   k, kmax;
  Boolean          val;

  wdialog = ParentWindow (b);
  Hide (wdialog);
  Update ();
  dbdp = (DialogBoxDataPtr) GetObjectExtra (ParentWindow (b));
  if ( ( pnl = GetPanelFromWindow (dbdp->w) ) != NULL ) 
  {
   if ( ( adp = GetAlignEditData (dbdp->w) ) != NULL ) { 
    adp->stoptransl |= PROPAG_GETSTOP;
    adp->stoptransl |= PROPAG_SETSTOP;
    if (adp->seqnumber > 0)  
    {
     sap = SeqAnnotBoolSegToDenseSeg (adp->sap_align);
     salp = (SeqAlignPtr) sap->data;
     vnpfeat = NULL;
     vnpf = adp->allseqfeat;
     kmax = CountItems (dbdp->lst3);
     jmax = CountItems (dbdp->lst2);
     for (k=1; k<=kmax; k++)
     {
        val = GetItemStatus (dbdp->lst3, k);
        if (val) {
           vnp = adp->anp_list; 
           for (j=1; j<=jmax; j++) 
           {
              val = GetItemStatus (dbdp->lst2, j);
              if (val) {
                    feat  = (SelEdStructPtr) vnpf->data.ptrvalue;
                    subtype = vnpf->choice;
                    anp = (AlignNodePtr) vnp->data.ptrvalue;
                    eID = anp->seq_entityID;
                    iID = anp->bsp_itemID;
                    anp = (AlignNodePtr) vnp->data.ptrvalue;
                    slp = AsnIoMemCopy ((Pointer) feat->region, (AsnReadFunc) SeqLocAsnRead, (AsnWriteFunc) SeqLocAsnWrite);
                    sip_source = SeqIdDup (anp->sip);

                    sesp = new_seledstruct_fromseqloc (eID, feat->itemID, (Uint2)OBJ_SEQFEAT, subtype, feat->bsp_itemID, slp, feat->label, sip_source, 0, 0);
                    if (sesp != NULL) {
                       ValNodeAddPointer(&vnpfeat, 0, (Pointer) sesp);
                    }
              }
              vnp = vnp->next;
           }
        }
        vnpf = vnpf->next;
     }
     val = FeatListToProp (adp, vnpfeat);
     if (val) {    
        sap = SeqAnnotFree (sap);
        ObjMgrSendMsg (OM_MSG_UPDATE, adp->master.entityID, adp->master.itemID, OBJ_BIOSEQ); 
     }
    }
   }
  }
  Remove (wdialog);
  return; 
}


static void getchoicegaps (GrouP c)
{
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  Int2             j;

  dbdp = (DialogBoxDataPtr) GetObjectExtra (ParentWindow (c));
  if ( ( adp = GetAlignEditData (dbdp->w) ) != NULL ) {
     j = GetValue (c);
     if (j == 2) 
        adp->gap_choice = IGNORE_GAP_CHOICE;
     else
        adp->gap_choice = DEFAULT_GAP_CHOICE;
  }
}

static void select_lst_sseq (LisT lst)
{
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  ValNodePtr       vnp,
                   vnpf;
  SelEdStructPtr   feat;
  SeqIdPtr         sip;
  SeqLocPtr        slp,
                   slpseq;
  Int2             j, k, jmax, kmax;
  Boolean          val;

  dbdp = (DialogBoxDataPtr) GetObjectExtra (ParentWindow (lst));
  if ( ( adp = GetAlignEditData (dbdp->w) ) != NULL ) 
  { 
     vnp = adp->sqloc_list; 
     jmax = CountItems (lst);
     for (j=1; j<=jmax; j++) 
     {
        val = GetItemStatus (lst, j);
        vnpf = adp->allseqfeat;
        kmax = CountItems (dbdp->lst3);
        for (k=1; k<=kmax; k++)
        {
           slpseq = (SeqLocPtr) vnp->data.ptrvalue;
           feat  = (SelEdStructPtr) vnpf->data.ptrvalue;
           slp = (SeqLocPtr) feat->region;
           sip = SeqLocId (slp);
           if (SeqIdForSameBioseq (SeqLocId(slpseq), sip)) {
                 SetItemStatus (dbdp->lst3, k, val);
           }
           else { 
              if (val) {
                 SetItemStatus (dbdp->lst3, k, FALSE);
              }
           }
           vnpf = vnpf->next;
        }
        vnp = vnp->next;
     }
  }
}

static void select_lst_feat (LisT lst)
{
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  ValNodePtr       vnp,
                   vnpf;
  SelEdStructPtr   feat;
  SeqLocPtr        slp,
                   slpseq;
  SeqIdPtr         sip;
  Int2             j, k, jmax, kmax;
  Boolean          val;

  dbdp = (DialogBoxDataPtr) GetObjectExtra (ParentWindow (lst));
  if ( ( adp = GetAlignEditData (dbdp->w) ) != NULL ) { 
     vnp = adp->sqloc_list; 
     jmax = CountItems (dbdp->lst1);
     for (j=1; j<=jmax; j++) 
     {
        vnpf = adp->allseqfeat;
        kmax = CountItems (lst);
        val = FALSE;
        for (k=1; k<=kmax; k++)
        {
           slpseq = (SeqLocPtr) vnp->data.ptrvalue;
           feat  = (SelEdStructPtr) vnpf->data.ptrvalue;
           slp = (SeqLocPtr) feat->region;
           sip = SeqLocId (slp);
           if (SeqIdForSameBioseq (SeqLocId(slpseq), sip)) {
              val = GetItemStatus (lst, k);
           }
           if (val) break;
           vnpf = vnpf->next;
        }
        SetItemStatus (dbdp->lst1, j, val);
        vnp = vnp->next;
     }
  }
}

static void StopTranslateBtn (ButtoN bn)
{
  WindoW           wdialog;
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;

  wdialog = ParentWindow (bn);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  if (GetStatus (bn))
     adp->stoptransl |= PROPAG_SETSTOP; 
  return;
}

static void TranslUntilStopBtn (ButtoN bn)
{
  WindoW           wdialog;
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;

  wdialog = ParentWindow (bn);
  dbdp = (DialogBoxDataPtr) GetObjectExtra (wdialog);
  if ( ( adp = GetAlignEditData (dbdp->w) ) == NULL ) return;
  if (GetStatus (bn))
     adp->stoptransl |= PROPAG_GETSTOP; 
  return;
}

static void selectall (LisT lst)
{
  Int2 j, max;
  max = CountItems (lst);
  for (j=1; j<=max; j++) {
     SetItemStatus (lst, j, TRUE);
  }
}

static void selectall1 (ButtoN b)
{
  DialogBoxDataPtr dbdp;
  dbdp = (DialogBoxDataPtr) GetObjectExtra (ParentWindow (b));
  selectall (dbdp->lst1);
}

static void selectall2 (ButtoN b)
{
  DialogBoxDataPtr dbdp;
  dbdp = (DialogBoxDataPtr) GetObjectExtra (ParentWindow (b));
  selectall (dbdp->lst2);
}

static void selectall3 (ButtoN b)
{
  DialogBoxDataPtr dbdp;
  dbdp = (DialogBoxDataPtr) GetObjectExtra (ParentWindow (b));
  selectall (dbdp->lst3);
}

static ValNodePtr AddAllFeatureFunc (EditAlignDataPtr adp, ValNodePtr allseqfeat)
{
  AlignNodePtr     anp;
  ValNodePtr       vnp;
  SeqLocPtr        slp;

  allseqfeat = SeqfeatlistFree (allseqfeat);
  for (vnp = adp->anp_list; vnp != NULL; vnp = vnp->next) {
     anp = (AlignNodePtr) vnp->data.ptrvalue;
     if ( anp != NULL ) {
           slp = CollectSeqLocFromAlignNode (anp);
           CollectFeatureForAlign (slp, anp, adp->featOrder, adp->groupOrder);
           allseqfeat = CollectFeatureForEditor (slp, allseqfeat, anp->seq_entityID, anp->bsp_itemID, adp->featOrder, TRUE);
     }
  }
  return allseqfeat;
}

NLM_EXTERN void PropagateFeatDialog (IteM i)
{
  WindoW           w, wdialog;
  DialogBoxDataPtr dbdp;
  EditAlignDataPtr adp;
  GrouP            g1, g2, g2b, g3, g4, g5,
                   d, e, c, h;
  LisT             lst_seq,
                   lst_sseq,
                   lst_feat; 
  Char             str [128];
  Char             str2 [24];
  CharPtr          tmp;
  CharPtr          strp;
  ValNodePtr       vnp;
  SelEdStructPtr   feat;
  SeqLocPtr        slp,
                   slpseq;
  Int4             start, stop;
  Uint1            strand;
  ButtoN           b;
  PrompT           p;
  GrouP            warn_grp;

  ValNodePtr  allseqfeat = NULL;

  w = ParentWindow (i);
  if ( ( adp = GetAlignEditData (w) ) == NULL ) 
     return;
  adp->allseqfeat = AddAllFeatureFunc (adp, adp->allseqfeat);
  allseqfeat = adp->allseqfeat;
  wdialog = FixedWindow (-50, -33, -10, -10, "Feature Propagation", StdCloseWindowProc);
  dbdp = (DialogBoxDataPtr) MemNew (sizeof (DialogBoxData));
  SetObjectExtra (wdialog, (Pointer) dbdp, StdCleanupExtraProc);
  dbdp->w = w;

  g1 = HiddenGroup (wdialog, 0, -7, NULL);

  warn_grp = HiddenGroup (g1, 0, 1, NULL);
  MultiLinePrompt (warn_grp, "WARNING: THIS FEATURE IS OBSOLETE.  Please use the Edit->Feature Propagate... menu item"
                " in the main Sequin menu.", 30 * stdCharWidth, systemFont);  

  g2 = HiddenGroup (g1, 2, 0, NULL);

  g4 = HiddenGroup (g2, 0, -3, NULL);
  p = StaticPrompt (g4, "Select source sequences", 0, dialogTextHeight,systemFont, 'c');
  lst_sseq = ExtendedList (g4, 12, 5, select_lst_sseq);
  for (vnp = adp->sqloc_list; vnp!=NULL; vnp = vnp->next)
  {
     str [0] = '\0';
     slpseq = (SeqLocPtr) vnp->data.ptrvalue;
     SeqIdWrite (SeqLocId(slpseq), str, PRINTID_REPORT, sizeof (str));
     ListItem (lst_sseq, str);
  }
  dbdp->lst1 = lst_sseq;
  b = PushButton (g4, "Select all", selectall1);
  AlignObjects (ALIGN_CENTER, (HANDLE) lst_sseq, (HANDLE) b, (HANDLE) p, NULL);

  g5 = HiddenGroup (g2, 0, -3, NULL);
  p = StaticPrompt (g5, "Select target sequences", 0, dialogTextHeight,systemFont, 'c');
  lst_seq = ExtendedList (g5, 12, 5, NULL);
  for (vnp = adp->sqloc_list; vnp!=NULL; vnp = vnp->next)
  {
     str [0] = '\0';
     slpseq = (SeqLocPtr) vnp->data.ptrvalue;
     SeqIdWrite (SeqLocId(slpseq), str, PRINTID_REPORT, sizeof (str));
     ListItem (lst_seq, str);
  }
  dbdp->lst2 = lst_seq;
  b = PushButton (g5, "Select all", selectall2);
  AlignObjects (ALIGN_CENTER, (HANDLE) lst_seq, (HANDLE) b, (HANDLE) p, NULL);

  g2b = HiddenGroup (g1, 1, 0, NULL);
  g3 = HiddenGroup (g2b, -1, 0, NULL);
  p = StaticPrompt (g3, "Select source Features", 0, dialogTextHeight,systemFont, 'c');
  lst_feat = ExtendedList (g3, 31, 5, select_lst_feat);
  for (vnp = allseqfeat; vnp!=NULL; vnp=vnp->next)
  {
     str [0] = '\0';
     tmp = str;
     feat  = (SelEdStructPtr) vnp->data.ptrvalue;
     slp = (SeqLocPtr) feat->region;
     start = SeqLocStart (slp) +1;
     stop = SeqLocStop (slp) +1;
     strand = SeqLocStrand (slp);
     if (vnp->choice == FEATDEF_GENE)
        tmp = StringMove (tmp, "GENE: ");
     else  if (vnp->choice == FEATDEF_mRNA)
        tmp = StringMove (tmp, "mRNA: ");
     else  if (vnp->choice == FEATDEF_CDS)
        tmp = StringMove (tmp, "CDS: ");
     else if (vnp->choice>=first_GBFeat && vnp->choice<number_GBFeat+first_GBFeat) {
        strp = GBFeat[(vnp->choice-first_GBFeat)];
        tmp = StringMove (tmp, strp); 
        tmp = StringMove (tmp, ": ");
     }
     if (feat->label != NULL)
        tmp = StringMove (tmp, feat->label);
     if (strand == Seq_strand_minus) {
        sprintf (str2, " (%ld..%ld) minus strand", (long)start, (long)stop);
     } else
        sprintf (str2, " (%ld..%ld)", (long)start, (long)stop);
     tmp = StringMove (tmp, str2);
     ListItem (lst_feat, str);
  }
  dbdp->lst3 = lst_feat;
  b = PushButton (g3, "Select all", selectall3);
  AlignObjects (ALIGN_CENTER, (HANDLE) lst_feat, (HANDLE) b, (HANDLE) p, NULL);

  c = HiddenGroup (g1, 2, 0, getchoicegaps);
  RadioButton (c, "split at gaps");
  RadioButton (c, "extend over gaps");
  SetValue (c, (Int2)(adp->gap_choice + 1));

  d = HiddenGroup (g1, 1, 0, NULL);
  CheckBox (d, "stop CDS translation at internal stop codon", StopTranslateBtn);
  SetValue (d, (Int2)(1));

  e = HiddenGroup (g1, 1, 0, NULL);
  CheckBox (e, "translate CDS after partial 3' boundary", TranslUntilStopBtn);
  SetValue (e, (Int2)(0));

  h = HiddenGroup (g1, 2, 0, NULL);
  PushButton (h, "Propagate", PropagateFeatureProc);
  PushButton (h, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) warn_grp, (HANDLE) g2, (HANDLE) g3, (HANDLE) c, (HANDLE) d, (HANDLE) e, (HANDLE) h, NULL);

  RealizeWindow (wdialog);
  Show (wdialog);
  return;
}


