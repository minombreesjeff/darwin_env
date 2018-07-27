/*  panels.h
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
* File Name:  panels.h
*
* Author:  Jonathan Kans
*   
* Version Creation Date: 11/18/91
*
* $Revision: 6.0 $
*
* File Description:  Active tabular text and graphic objects
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: panels.h,v $
* Revision 6.0  1997/08/25 18:56:13  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 13:45:08  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 2.4  1995/05/17  15:15:14  kans
 * added Log line
 *
*
* ==========================================================================
*/

/*
*  WARNING:  The table and palette objects are obsolete, and slated for removal in
*            mid-1995.  They should not be used.  The document object replaces them.
*/

#ifndef _PANELS_
#define _PANELS_

#ifndef _VIBRANT_
#include <vibrant.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef  struct  PaletteType {
  Nlm_VoidPtr  dummy;
} HNDL PalettE;

typedef  struct  TableType {
  Nlm_VoidPtr  dummy;
} HNDL TablE;

typedef  void  (*PaintProc) PROTO((PalettE, Int2, RectPtr));
typedef  void  (*PaletteProc) PROTO((PalettE, Int2));

typedef  void  (*TableProc) PROTO((TablE, Int2, Int2));

PalettE  PalettePanel PROTO((SlatE s, Int2 pixwidth, PaletteProc actn));
void     AppendPalette PROTO((PalettE p, RectPtr r, PaintProc draw, PaletteProc click, Int2 value));
void     SetPaletteValue PROTO((PalettE p, Int2 num, Int2 value));
Int2     GetPaletteValue PROTO((PalettE p, Int2 num));
Int2     PaletteNumItems PROTO((PalettE p));

TablE   TablePanel PROTO((SlatE s, Int2 minwid, FonT font, TableProc actn));
void    RecordColumn PROTO((TablE t, Int2 wid, Char just, Boolean wrap, Boolean bar, TableProc actn));
void    AppendTableText PROTO((TablE t, CharPtr title));
Boolean GetTableText PROTO((TablE t, Int2 row, Int2 column, CharPtr text, Uint4 maxsize));
Uint4   TableTextLength PROTO((TablE t, Int2 row, Int2 column));
void    SetTableBlockHilight PROTO((TablE t, Int2 firstRow, Int2 lastRow, Int2 firstColumn, Int2 lastColumn, Boolean hilight));
void    SetTableHilight PROTO((TablE t, Int2 row, Int2 column, Boolean hilight));
Boolean GetTableHilight PROTO((TablE t, Int2 row, Int2 column));
void SetTableBlockGray PROTO((TablE t, Int2 firstRow, Int2 lastRow, Int2 firstColumn, Int2 lastColumn, Boolean gray));
void    SetTableGray PROTO((TablE t, Int2 row, Int2 column, Boolean gray));
Boolean GetTableGray PROTO((TablE t, Int2 row, Int2 column));
Int2    TableNumLines PROTO((TablE t));
Int2    TableVisLines PROTO((TablE t));

#ifdef __cplusplus
}
#endif

#endif
