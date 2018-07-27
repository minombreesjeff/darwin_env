/*  document.h
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
* File Name:  document.h
*
* Author:  Jonathan Kans
*   
* Version Creation Date: 4/12/93
*
* $Revision: 6.2 $
*
* File Description:  Converts fielded text into final report in a document
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: document.h,v $
* Revision 6.2  2000/05/22 16:38:24  kans
* added UpdateColFmt per Serge Bazhin request
*
* Revision 6.1  1998/05/02 19:42:36  kans
* added SetDocSimpleMode
*
* Revision 6.0  1997/08/25 18:55:36  madden
* Revision changed to 6.0
*
* Revision 5.7  1997/08/12 18:43:02  kans
* added SaveDocumentItem
*
* Revision 5.6  1997/06/11 14:27:09  kans
* add SetDocExtra to allow custom cell draw
*
* Revision 5.5  1996/10/08 14:25:51  kans
* added InvalDocCols, suppress ddata.draw user callback if not full line
*
 * Revision 5.4  1996/09/26  13:12:15  kans
 * added standard cache callbacks, which use a simple binary tree
 *
 * Revision 5.3  1996/09/25  15:30:32  kans
 * added InvalDocument function
 *
 * Revision 5.2  1996/09/11  19:44:02  kans
 * *** empty log message ***
 *
 * Revision 5.1  1996/09/09  00:15:01  kans
 * now uses AutonomousPanel4, virtual scroll bars, to have > 32K lines
 *
 * Revision 5.0  1996/05/28  13:45:08  ostell
 * Set to revision 5.0
 *
 * Revision 4.1  1996/03/09  23:37:15  kans
 * additional simplification functions to simulate single choice list
 *
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 2.32  1995/05/17  15:15:14  kans
 * added Log line
 *
*
* ==========================================================================
*/

#ifndef _DOCUMENT_
#define _DOCUMENT_

#ifndef _VIBRANT_
#include <vibrant.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* DOC OBJECT HANDLE TYPE */

typedef struct Nlm_doctype {
  Nlm_VoidPtr  dummy;
} HNDL Nlm_DoC;

/* DOC PROCEDURE TYPES */

typedef Nlm_CharPtr (*Nlm_DocPrntProc)  PROTO((Nlm_DoC, Nlm_Int2, Pointer));
typedef void        (*Nlm_DocClckProc)  PROTO((Nlm_DoC, Nlm_PoinT));
typedef void        (*Nlm_DocNotfyProc) PROTO((Nlm_DoC, Nlm_Int2, Nlm_Int2, Nlm_Int2, Nlm_Boolean));
typedef void        (*Nlm_DocDrawProc)  PROTO((Nlm_DoC, Nlm_RectPtr, Nlm_Int2, Nlm_Int2));
typedef void        (*Nlm_DocCellProc)  PROTO((Nlm_DoC, Nlm_Int2, Nlm_Int2, Nlm_Int2, Nlm_RectPtr, Nlm_CharPtr, Nlm_Char, Nlm_Boolean));
typedef Nlm_Boolean (*Nlm_DocShadeProc) PROTO((Nlm_DoC, Nlm_Int2, Nlm_Int2, Nlm_Int2));
typedef void        (*Nlm_DocDataProc)  PROTO((Nlm_DoC, Pointer));
typedef void        (*Nlm_DocPanProc)   PROTO((Nlm_DoC));
typedef void        (*Nlm_DocFreeProc)  PROTO((Nlm_DoC, Nlm_VoidPtr));

typedef void        (*Nlm_DocPutProc)   PROTO((Nlm_DoC, Nlm_Int2, Nlm_CharPtr));
typedef Nlm_CharPtr (*Nlm_DocGetProc)   PROTO((Nlm_DoC, Nlm_Int2));
typedef void        (*Nlm_DocUpdProc)   PROTO((Nlm_DoC, Nlm_Int2, Nlm_Int2));

/* COLUMN AND PARAGRAPH STRUCTURES */

typedef struct Nlm_coldata {
  Nlm_Int2      pixWidth;
  Nlm_Int2      pixInset;
  Nlm_Int2      charWidth;
  Nlm_Int2      charInset;
  Nlm_FonT      font;
  Nlm_Char      just;
  unsigned int  wrap      : 1;
  unsigned int  bar       : 1;
  unsigned int  underline : 1;
  unsigned int  left      : 1;
  unsigned int  last      : 1;
} Nlm_ColData, PNTR Nlm_ColPtr;

typedef struct Nlm_pardata {
  unsigned int  openSpace    : 1;
  unsigned int  keepWithNext : 1;
  unsigned int  keepTogether : 1;
  unsigned int  newPage      : 1;
  unsigned int  tabStops     : 1;
  Nlm_Int2      minLines;
  Nlm_Int2      minHeight;
} Nlm_ParData, PNTR Nlm_ParPtr;

/* DOC PROCEDURES */

extern Nlm_DoC Nlm_DocumentPanel  PROTO((Nlm_GrouP prnt, Nlm_Int2 pixwidth, Nlm_Int2 pixheight));

extern void    Nlm_SetDocProcs    PROTO((Nlm_DoC d, Nlm_DocClckProc click, Nlm_DocClckProc drag,
                                  Nlm_DocClckProc release, Nlm_DocPanProc pan));
extern void    Nlm_SetDocShade    PROTO((Nlm_DoC d, Nlm_DocDrawProc draw, Nlm_DocShadeProc gray,
                                  Nlm_DocShadeProc invert, Nlm_DocShadeProc color));
extern void    Nlm_SetDocExtra    PROTO((Nlm_DoC d, Nlm_DocCellProc drawcell));
extern void    Nlm_SetDocCache    PROTO((Nlm_DoC d, Nlm_DocPutProc put, Nlm_DocGetProc get,
                                  Nlm_DocUpdProc upd));

extern void    Nlm_SetDocData     PROTO((Nlm_DoC d, VoidPtr data, Nlm_DocFreeProc cleanup));
extern VoidPtr Nlm_GetDocData     PROTO((Nlm_DoC d));
extern Nlm_CharPtr Nlm_GetDocText PROTO((Nlm_DoC d, Nlm_Int2 item, Nlm_Int2 row, Nlm_Int2 col));
extern void    Nlm_MapDocPoint    PROTO((Nlm_DoC d, Nlm_PoinT pt, Nlm_Int2Ptr item, Nlm_Int2Ptr row,
                                  Nlm_Int2Ptr col, Nlm_RectPtr rct));
extern void    Nlm_PrintDocument  PROTO((Nlm_DoC d));
extern void    Nlm_SaveDocument   PROTO((Nlm_DoC d, FILE *f));
extern void    Nlm_SaveDocumentItem PROTO((Nlm_DoC d, FILE *f, Nlm_Int2 item));

/* PARAMETER PROCEDURES */

extern void Nlm_GetDocParams4  PROTO((Nlm_DoC d, Nlm_Int2Ptr numItems, Nlm_Int4Ptr numLines));
extern void Nlm_GetDocParams   PROTO((Nlm_DoC d, Nlm_Int2Ptr numItems, Nlm_Int2Ptr numLines));
extern void Nlm_GetItemParams4 PROTO((Nlm_DoC d, Nlm_Int2 item, Nlm_Int4Ptr startsAt,
			Nlm_Int2Ptr numRows, Nlm_Int2Ptr numCols, Nlm_Int2Ptr lineHeight, Pointer PNTR data));
extern void Nlm_GetItemParams  PROTO((Nlm_DoC d, Nlm_Int2 item, Nlm_Int2Ptr startsAt,
			Nlm_Int2Ptr numRows, Nlm_Int2Ptr numCols, Nlm_Int2Ptr lineHeight, Pointer PNTR data));
extern void Nlm_GetColParams   PROTO((Nlm_DoC d, Nlm_Int2 item, Nlm_Int2 col, Nlm_Int2Ptr pixPos,
			Nlm_Int2Ptr pixWidth, Nlm_Int2Ptr pixInset, Nlm_CharPtr just));

/* TABLE PROCEDURES */

extern void Nlm_AppendItem  PROTO((Nlm_DoC d, Nlm_DocPrntProc proc, Pointer data,
			Nlm_Boolean docOwnsData, Nlm_Int2 lines, Nlm_ParPtr parFmt,
			Nlm_ColPtr colFmt, Nlm_FonT font));
extern void Nlm_AppendText  PROTO((Nlm_DoC d, Nlm_CharPtr text, Nlm_ParPtr parFmt,
			Nlm_ColPtr colFmt, Nlm_FonT font));

extern void Nlm_ReplaceItem PROTO((Nlm_DoC d, Nlm_Int2 item, Nlm_DocPrntProc proc,
			Pointer data, Nlm_Boolean docOwnsData, Nlm_Int2 lines,
			Nlm_ParPtr parFmt, Nlm_ColPtr colFmt, Nlm_FonT font));
extern void Nlm_ReplaceText PROTO((Nlm_DoC d, Nlm_Int2 item, Nlm_CharPtr text,
			Nlm_ParPtr parFmt, Nlm_ColPtr colFmt, Nlm_FonT font));

extern void Nlm_InsertItem  PROTO((Nlm_DoC d, Nlm_Int2 item, Nlm_DocPrntProc proc,
			Pointer data, Nlm_Boolean docOwnsData, Nlm_Int2 lines,
			Nlm_ParPtr parFmt, Nlm_ColPtr colFmt, Nlm_FonT font));
extern void Nlm_InsertText  PROTO((Nlm_DoC d, Nlm_Int2 item, Nlm_CharPtr text,
			Nlm_ParPtr parFmt, Nlm_ColPtr colFmt, Nlm_FonT font));

extern void Nlm_DeleteItem  PROTO((Nlm_DoC d, Nlm_Int2 item));

/* BULK APPEND PROCEDURE */

extern void Nlm_BulkAppendItem PROTO((Nlm_DoC d, Nlm_Int2 numItems, Nlm_DocPrntProc proc,
            Nlm_Int2 estLines, Nlm_ParPtr parFmt, Nlm_ColPtr colFmt, Nlm_FonT font));

/* STANDARD CACHE CALLBACK PROCEDURES */

extern void Nlm_StdPutDocCache    PROTO((Nlm_DoC d, Nlm_Int2 item, Nlm_CharPtr text));
extern Nlm_CharPtr Nlm_StdGetDocCache PROTO((Nlm_DoC d, Nlm_Int2 item));
extern void Nlm_StdResetDocCache  PROTO((Nlm_DoC d, Nlm_Int2 from, Nlm_Int2 to));

/* SIMPLIFICATION PROCEDURES */

extern void Nlm_SetDocDefaults  PROTO((Nlm_DoC d, Nlm_ParPtr defaultParFmt,
            Nlm_ColPtr defaultColFmt, Nlm_FonT defaultFont));
extern void Nlm_SetDocSimpleMode PROTO((Nlm_DoC d, Nlm_Boolean useRowsNotItems));
extern void Nlm_SetDocHighlight  PROTO((Nlm_DoC d, Nlm_Int2 firstItem, Nlm_Int2 lastItem));
extern void Nlm_GetDocHighlight  PROTO((Nlm_DoC d, Nlm_Int2Ptr firstItem, Nlm_Int2Ptr lastItem));
extern void Nlm_SetDocNotify  PROTO((Nlm_DoC d, Nlm_DocNotfyProc notify));

/* INVALIDATION AND UPDATE PROCEDURE */

extern Nlm_Boolean Nlm_ItemIsVisible PROTO((Nlm_DoC d, Nlm_Int2 item, Nlm_Int2Ptr top,
            Nlm_Int2Ptr bottom, Nlm_Int2Ptr firstLine));
extern Nlm_Boolean Nlm_RowIsVisible PROTO((Nlm_DoC d, Nlm_Int2 item, Nlm_Int2 row,
            Nlm_Int2Ptr top, Nlm_Int2Ptr bottom));
extern Nlm_Boolean Nlm_GetScrlParams4 PROTO((Nlm_DoC d, Nlm_Int4Ptr offset,
            Nlm_Int2Ptr firstShown, Nlm_Int2Ptr firstLine));
extern Nlm_Boolean Nlm_GetScrlParams PROTO((Nlm_DoC d, Nlm_Int2Ptr offset,
            Nlm_Int2Ptr firstShown, Nlm_Int2Ptr firstLine));
extern void Nlm_UpdateDocument PROTO((Nlm_DoC d, Nlm_Int2 from, Nlm_Int2 to));
extern void Nlm_InvalDocRows PROTO((Nlm_DoC d, Nlm_Int2 item, Nlm_Int2 from, Nlm_Int2 to));
extern void Nlm_InvalDocCols PROTO((Nlm_DoC d, Nlm_Int2 item, Nlm_Int2 from, Nlm_Int2 to));
extern void Nlm_InvalDocument PROTO((Nlm_DoC d));
extern void Nlm_ForceFormat PROTO((Nlm_DoC d, Nlm_Int2 item));
extern void Nlm_UpdateColFmt (Nlm_DoC d, Nlm_ColPtr col);

extern void Nlm_AdjustDocScroll PROTO((Nlm_DoC d));
extern void Nlm_SetDocAutoAdjust PROTO((Nlm_DoC d, Nlm_Boolean autoAdjust));

extern void Nlm_SetDocTabstops PROTO((Nlm_DoC d, Int2 tabStops));  /* dgg added this */

/* STANDARD FILE DISPLAY PROCEDURES */

extern void Nlm_DisplayFile   PROTO((Nlm_DoC d, Nlm_CharPtr file, Nlm_FonT font));
extern void Nlm_DisplayFancy  PROTO((Nlm_DoC d, Nlm_CharPtr file, Nlm_ParPtr parFmt,
            Nlm_ColPtr colFmt, Nlm_FonT font, Nlm_Int2 tabStops));

#define DoC Nlm_DoC
#define DocPrntProc Nlm_DocPrntProc
#define DocClckProc Nlm_DocClckProc
#define DocNotfyProc Nlm_DocNotfyProc
#define DocDrawProc Nlm_DocDrawProc
#define DocCellProc Nlm_DocCellProc
#define DocShadeProc Nlm_DocShadeProc
#define DocDataProc Nlm_DocDataProc
#define DocPanProc Nlm_DocPanProc
#define DocFreeProc Nlm_DocFreeProc
#define DocPutProc Nlm_DocPutProc
#define DocGetProc Nlm_DocGetProc
#define DocUpdProc Nlm_DocUpdProc
#define ColPtr Nlm_ColPtr
#define ColData Nlm_ColData
#define ParData Nlm_ParData
#define ParPtr Nlm_ParPtr
#define DocumentPanel Nlm_DocumentPanel
#define SetDocProcs Nlm_SetDocProcs
#define SetDocShade Nlm_SetDocShade
#define SetDocExtra Nlm_SetDocExtra
#define SetDocCache Nlm_SetDocCache
#define SetDocData Nlm_SetDocData
#define GetDocData Nlm_GetDocData
#define GetDocText Nlm_GetDocText
#define MapDocPoint Nlm_MapDocPoint
#define PrintDocument Nlm_PrintDocument
#define SaveDocument Nlm_SaveDocument
#define SaveDocumentItem Nlm_SaveDocumentItem
#define GetDocParams4 Nlm_GetDocParams4
#define GetDocParams Nlm_GetDocParams
#define GetItemParams4 Nlm_GetItemParams4
#define GetItemParams Nlm_GetItemParams
#define GetColParams Nlm_GetColParams
#define AppendItem Nlm_AppendItem
#define AppendText Nlm_AppendText
#define ReplaceItem Nlm_ReplaceItem
#define ReplaceText Nlm_ReplaceText
#define InsertItem Nlm_InsertItem
#define InsertText Nlm_InsertText
#define DeleteItem Nlm_DeleteItem
#define BulkAppendItem Nlm_BulkAppendItem
#define StdPutDocCache Nlm_StdPutDocCache
#define StdGetDocCache Nlm_StdGetDocCache
#define StdResetDocCache Nlm_StdResetDocCache
#define SetDocDefaults Nlm_SetDocDefaults
#define SetDocSimpleMode Nlm_SetDocSimpleMode
#define SetDocHighlight Nlm_SetDocHighlight
#define GetDocHighlight Nlm_GetDocHighlight
#define SetDocNotify Nlm_SetDocNotify
#define ItemIsVisible Nlm_ItemIsVisible
#define RowIsVisible Nlm_RowIsVisible
#define GetScrlParams4 Nlm_GetScrlParams4
#define GetScrlParams Nlm_GetScrlParams
#define UpdateDocument Nlm_UpdateDocument
#define InvalDocRows Nlm_InvalDocRows
#define InvalDocCols Nlm_InvalDocCols
#define InvalDocument Nlm_InvalDocument
#define ForceFormat Nlm_ForceFormat
#define UpdateColFmt Nlm_UpdateColFmt
#define AdjustDocScroll Nlm_AdjustDocScroll
#define SetDocAutoAdjust Nlm_SetDocAutoAdjust
#define SetDocTabstops Nlm_SetDocTabstops
#define DisplayFile Nlm_DisplayFile
#define DisplayFancy Nlm_DisplayFancy
 
#ifdef __cplusplus
}
#endif

#endif
