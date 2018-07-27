/*  document.c
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
* File Name:  document.c
*
* Author:  Jonathan Kans
*   
* Version Creation Date: 4/12/93
*
* $Revision: 6.17 $
*
* File Description:  Converts fielded text into final report in a document
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* 01-25-94 DGG + JK    Fixed MapDocPoint bug
*
* $Log: document.c,v $
* Revision 6.17  2004/08/06 18:48:28  kans
* subtle bug in DisplayFancy was missing one call to GetChar
*
* Revision 6.16  2004/07/29 01:56:47  kans
* DisplayFancy can handle DOS line endings when run on Mac
*
* Revision 6.15  2004/07/12 18:21:32  kans
* DrawTableItem had extra LineTo left over when drawing bar
*
* Revision 6.14  2004/07/11 16:31:03  kans
* added is_old_win to DrawTableItem
*
* Revision 6.13  2004/07/09 17:43:04  kans
* cache mechanism abandons binary tree in favor of separate field in itemPtr
*
* Revision 6.12  2004/06/17 16:06:46  kans
* removed artifact on drawing vertical bar on non-Mac platform - always go to barBottom - 1
*
* Revision 6.11  2003/03/27 18:23:54  kans
* increased 16000 character buffer to 24000 to handle long PubMed abstracts (e.g., pmid 12209194 technical report on toxicity studies)
*
* Revision 6.10  2003/02/28 19:14:58  kans
* DrawDocument bails if pixels == 0 to avoid getting stuck in almost infinite loop
*
* Revision 6.9  2000/05/22 16:38:24  kans
* added UpdateColFmt per Serge Bazhin request
*
* Revision 6.8  1999/11/05 19:59:35  beloslyu
* cast inserted
*
* Revision 6.7  1999/11/02 19:13:53  kans
* bulk append allows 32767 paragraphs
*
* Revision 6.6  1998/06/17 21:14:14  kans
* reset document clears first, last highlight item
*
* Revision 6.5  1998/06/16 21:14:29  kans
* fixed test for item between first and last highlight item
*
* Revision 6.4  1998/06/12 16:40:19  kans
* fixed warnings detected by unix compiler
*
* Revision 6.3  1998/05/02 19:42:31  kans
* added SetDocSimpleMode
*
* Revision 6.2  1997/11/26 21:29:50  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.1  1997/11/09 00:25:53  kans
* SaveTableItem does not strip preceeding tabs if tabCount is 0
*
* Revision 6.0  1997/08/25 18:55:30  madden
* Revision changed to 6.0
*
* Revision 5.23  1997/08/20 20:48:11  kans
* DrawTableItem selects curFont after invert, grey, color callbacks
*
* Revision 5.22  1997/08/12 18:42:56  kans
* added SaveDocumentItem
*
* Revision 5.21  1997/06/30 18:05:02  kans
* text per item can be more than 64K
*
* Revision 5.20  1997/06/11 14:27:04  kans
* add SetDocExtra to allow custom cell draw
*
* Revision 5.19  1997/06/04 00:06:24  kans
* support for Japanese by Tomo Koike of DDBJ
*
* Revision 5.18  1997/02/25 19:38:09  kans
* bulk append initializes cache (probably should do for regular append also)
*
 * Revision 5.17  1997/01/29  16:41:22  kans
 * using StringNCpy_0
 *
 * Revision 5.16  1996/10/28  21:19:17  kans
 * PrintDocument uses Nlm_XrmGetResource ("printCommand") as the command.
 *
 * Revision 5.15  1996/10/17  19:54:45  kans
 * fixed centering calculation on save to file
 *
 * Revision 5.14  1996/10/08  22:11:12  kans
 * get itemPtr of (item - 1) in InvalDocCols
 *
 * Revision 5.13  1996/10/08  14:25:51  kans
 * added InvalDocCols, suppress ddata.draw user callback if not full line
 *
 * Revision 5.12  1996/09/26  14:53:59  kans
 * ddata.cache is an allocated pointer, not a static head, since it needs
 * to be updated by the putCache function, but that function can only get
 * the out-of-synch ddata
 *
 * Revision 5.11  1996/09/26  13:12:15  kans
 * added standard cache callbacks, which use a simple binary tree
 *
 * Revision 5.10  1996/09/26  01:06:12  kans
 * barmax should be int4
 *
 * Revision 5.9  1996/09/25  23:43:17  kans
 * scroll proc uses old pgUp and pgDn, scrolls and updates first, then
 * calculates new pgUp, pgDn, and barmax, for faster performance
 *
 * Revision 5.8  1996/09/25  15:30:32  kans
 * added InvalDocument function
 *
 * Revision 5.7  1996/09/24  16:39:03  kans
 * GetDocText should not change notCached, since it doesn't keep text.
 *
 * Revision 5.6  1996/09/24  16:34:25  kans
 * GetDocText now records numRows, cached flags, back to real itemPtr blocks.
 *
 * Revision 5.5  1996/09/13  21:49:26  kans
 * bulk append item now records font properly
 *
 * Revision 5.4  1996/09/11  19:44:02  kans
 * *** empty log message ***
 *
 * Revision 5.3  1996/09/11  16:21:08  kans
 * add cacheifnever to adjustdocscroll, to pass correct numRows to visBefore
 *
 * Revision 5.2  1996/09/09  11:46:59  kans
 * UpdateLineStarts needed to return Int4.  Now virtual scrolling works.
 *
 * Revision 5.1  1996/09/09  00:15:01  kans
 * now uses AutonomousPanel4, virtual scroll bars, to have > 32K lines
 *
 * Revision 5.0  1996/05/28  13:45:08  ostell
 * Set to revision 5.0
 *
 * Revision 4.3  1996/03/11  19:18:14  kans
 * rearrange text len parameter now int4, to handle titin protein
 *
 * Revision 4.2  1996/03/09  23:37:15  kans
 * additional simplification functions to simulate single choice list
 *
 * Revision 4.1  1996/02/29  15:32:18  kans
 * can now invert cells that have no text (adjacent tabs)
 *
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 2.78  1995/05/17  15:15:14  kans
 * added Log line
 *
*
* ==========================================================================
*/

#include <vibrant.h>
#include <document.h>
#include <ncbilang.h>

/*****************************************************************************
*
*   SYMBOL DEFINES
*
*****************************************************************************/

#define JUST_LEFT   0
#define JUST_CENTER 1
#define JUST_RIGHT  2

#define MAXLISTS  256
#define LISTSIZE  256
#define MAXFONTS   32

/*****************************************************************************
*
*   TYPE DEFINES
*
*****************************************************************************/

typedef struct colxdata {
  FonT          font;
  Int2          position;
  Int2          pixWidth;
  Int2          pixInset;
  Int2          charWidth;
  Int2          charInset;
  unsigned int  just      : 2;
  unsigned int  zeroWidth : 1;
  unsigned int  wrap      : 1;
  unsigned int  bar       : 1;
  unsigned int  underline : 1;
  unsigned int  left      : 1;
  unsigned int  last      : 1;
} ColXData, PNTR ColXPtr;

typedef struct itemdata {
  DocPrntProc   prtProc;
  Pointer       dataPtr;
  CharPtr       text;
  CharPtr       cached;
  FonT          font;
  ColXPtr       colFmt;
  VoidPtr       extra;
  Int4          startsAt;
  Int2          numRows;
  Int2          numCols;
  Int2          lineHeight;
  Int2          leadHeight;
  Int2          minLines;
  Int2          minHeight;
  unsigned int  openSpace    : 1;
  unsigned int  keepWithNext : 1;
  unsigned int  keepTogether : 1;
  unsigned int  newPage      : 1;
  unsigned int  tabStops     : 1;
  unsigned int  docOwnsData  : 1;
  unsigned int  notCached    : 1;
  unsigned int  neverCached  : 1;
} ItemData, PNTR ItemPtr;

typedef struct listdata {
  ItemData  items [LISTSIZE];
} ListData, PNTR ListDPtr;

typedef struct masterdata {
  ListDPtr  list [MAXLISTS];
} MasterData, PNTR MasterPtr;

typedef struct docdata {
  DoC           doc;
  MasterPtr     master;
  DocDrawProc   draw;
  DocCellProc   drawcell;
  DocPanProc    pan;
  DocShadeProc  gray;
  DocShadeProc  invert;
  DocShadeProc  color;
  DocPutProc    put;
  DocGetProc    get;
  DocUpdProc    upd;
  VoidPtr       data;
  DocFreeProc   cleanup;
  ValNodePtr    colFmts;
  Int2          numItems;
  Int4          numLines;
  Int4          barmax;
  Int2          pgUp;
  Int2          pgDn;
  Int2          tabCount;
  Boolean       autoAdjust;
  ParPtr        defaultParFmt;
  ColPtr        defaultColFmt;
  FonT          defaultFont;
  Int2          firstHighlightItem;
  Int2          lastHighlightItem;
  Boolean       useRowsNotItems;
  DocNotfyProc  notify;
} DocData, PNTR DocDataPtr;

/*****************************************************************************
*
*   justToChar
*       Converts justification parameter to character code
*
*****************************************************************************/

static Char  justToChar [4] = {'l', 'c', 'r', 'l'};

/*****************************************************************************
*
*   fontHeights
*       Cached list of fonts and font pixel heights
*
*****************************************************************************/

static struct fontheights {
  FonT  font;
  Int2  height;
} fontHeights [MAXFONTS];

/*****************************************************************************
*
*   GetItemPtr (ddatptr, item)
*       Returns an item pointer given an item number
*
*****************************************************************************/

static ItemPtr GetItemPtr (DocDataPtr ddatptr, Int2 item)

{
  Int2       index;
  Int2       list;
  ItemPtr    itemPtr;
  ListDPtr   listPtr;
  MasterPtr  masterPtr;

  itemPtr = NULL;
  if (ddatptr != NULL && ddatptr->master != NULL && item < ddatptr->numItems) {
    list = (item / LISTSIZE);
    if (list < MAXLISTS) {
      masterPtr = ddatptr->master;
      listPtr = masterPtr->list [list];
      if (listPtr != NULL) {
        index = (item % LISTSIZE);
        itemPtr = &(listPtr->items [index]);
      }
    }
  }
  return itemPtr;
}

/*****************************************************************************
*
*   GetItemNum (ddatptr, desired)
*       Returns the number of the item containing the desired line
*
*****************************************************************************/

static Int2 GetItemNum (DocDataPtr ddatptr, Int4 desired)

{
  Boolean  goOn;
  ItemPtr  itemPtr;
  Int4     left;
  Int4     mid;
  Int4     right;

  mid = 1;
  if (ddatptr != NULL) {
    left = 1;
    right = ddatptr->numItems;
    goOn = TRUE;
    while (left <= right && goOn) {
      mid = (left + right) / 2;
      itemPtr = GetItemPtr (ddatptr, mid - 1);
      if (itemPtr != NULL) {
        if (desired < itemPtr->startsAt + itemPtr->numRows) {
          right = mid - 1;
        }
        if (desired >= itemPtr->startsAt) {
          left = mid + 1;
        }
      } else {
        goOn = FALSE;
        mid = 1;
      }
    }
  }
  return mid - 1;
}

/*****************************************************************************
*
*   GetNextBlock (title, maxwid, byPixels, wordWrap, tabStops, tabCount)
*       Returns the number of characters in the next block, including
*       trailing spaces (which are trimmed back later)
*
*****************************************************************************/

static Int2 GetNextBlock (CharPtr title, Int2 maxwid, Boolean byPixels,
                          Boolean wordWrap, Boolean tabStops, Int2 tabCount)

{
  Char     ch;
  Int2     i;
  Boolean  isjapanese;
  Int2     j;
  Int2     wid;

  isjapanese = IsJapanese ();
  if (isjapanese) {
  i = 0;
  j = 0;
  wid = 0;
  if (title != NULL && maxwid > 0) {
    ch = title [i];
    while (ch != '\0' && ch != '\n' && ch != '\r' &&
           (ch != '\t' || tabStops) && wid <= maxwid) {
      if (wordWrap) {
        if (ch == '\t' && tabStops) {
          if (byPixels) {
            wid += CharWidth (' ') * tabCount;
          } else {
            wid += tabCount;
          }
        } else {
          if (byPixels) {
            wid += TextWidth(title + i, Nlm_LetterByte(title + i));
          } else {
            wid += Nlm_LetterByte(title + i);
          }
        }
      }
      i += Nlm_LetterByte(title + i);
      ch = title [i];
    }
    j = i;
/* sorry, 1 or 2 byte letters only. but isn't it enough ? */
#define BackIsMBLetter(str, i)	((i > 1) && (IsMBLetter(title + i - 2)))
#define BackStep(str, i)	(BackIsMBLetter(str, i) ? 2 : 1)
    if (wid > maxwid && wordWrap) {
      j -= BackStep(title, j);
      if (byPixels) {
        while (TextWidth (title, i) > maxwid) {
          i -= BackStep(title, i);
        }
      } else {
        while (i > maxwid) {
          i -= BackStep(title, i);
        }
      }
	  while (i > 0 && title[i - 1] != ' ' && title [i - 1] != '-' && 
										!(BackIsMBLetter(title, i))) {
        /* at least in Japanese, almost all of multibyte letters are
           good as a word wrap point. */
        i -= BackStep(title, i);
	  }
      while (title [i] == ' ') {
        i++;
      }
    }
  }
  } else {
  i = 0;
  j = 0;
  wid = 0;
  if (title != NULL && maxwid > 0) {
    ch = title [i];
    while (ch != '\0' && ch != '\n' && ch != '\r' &&
           (ch != '\t' || tabStops) && wid <= maxwid) {
      if (wordWrap) {
        if (ch == '\t' && tabStops) {
          if (byPixels) {
            wid += CharWidth (' ') * tabCount;
          } else {
            wid += tabCount;
          }
        } else {
          if (byPixels) {
            wid += CharWidth (ch);
          } else {
            wid++;
          }
        }
      }
      i++;
      ch = title [i];
    }
    j = i;
    if (wid > maxwid && wordWrap) {
      j--;
      if (byPixels) {
        while (TextWidth (title, i) > maxwid) {
          i--;
        }
      } else {
        while (i > maxwid) {
          i--;
        }
      }
      while (i > 0 && title [i - 1] != ' ' && title [i - 1] != '-') {
        i--;
      }
      while (title [i] == ' ') {
        i++;
      }
    }
  }
  }
  if (i > 0 && i < j) {
    return i;
  } else if (j > 0) {
    return j;
  } else {
    return 0;
  }
}

/*****************************************************************************
*
*   CELL STRUCTURE DEFINITIONS
*
*****************************************************************************/

#define CELLCHUNK 128

typedef struct celldata {
  Uint4  start;
  Int2   count;
} CellData, PNTR CellPtr;

typedef struct parsedata {
  Int2     numCells;
  CellPtr  cellPtr;
} ParseData, PNTR ParsePtr;

/*****************************************************************************
*
*   RecordCell (itemPtr, parsePtr, start, len, row, col)
*       Records the character position and length for a given cell
*
*****************************************************************************/

static void RecordCell (ItemPtr itemPtr, ParsePtr parsePtr,
                        Uint4 start, Int2 len, Int2 row, Int2 col)

{
  Int4     cell;
  CellPtr  cellPtr;
  Int2     newCells;
  Int2     numCells;

  if (itemPtr != NULL && parsePtr != NULL && col < itemPtr->numCols) {
    numCells = parsePtr->numCells;
    cellPtr = parsePtr->cellPtr;
    cell = (Int4) ((Int4) row * (Int4) (itemPtr->numCols) + (Int4) col);
    if (cell >= (Int4) numCells) {
      newCells = (cell / CELLCHUNK + 1) * CELLCHUNK;
      if (cellPtr != NULL) {
        cellPtr = (CellPtr) MemMore (cellPtr, sizeof (CellData) * newCells);
        if (cellPtr != NULL) {
          while (numCells < newCells) {
            cellPtr [numCells].start = 0;
            cellPtr [numCells].count = 0;
            numCells++;
          }
        }
      } else {
        cellPtr = (CellPtr) MemNew (sizeof (CellData) * newCells);
      }
      parsePtr->cellPtr = cellPtr;
      parsePtr->numCells = newCells;
    }
    if (cellPtr != NULL) {
      cellPtr [cell].start = start;
      cellPtr [cell].count = len;
    }
  }
}

/*****************************************************************************
*
*   ParseText (itemPtr, parsePtr, text, byPixels, tabCount)
*       Parses text that contains \t, \r and \n characters into a list of
*       cells organized by row and column
*
*****************************************************************************/

static void ParseText (ItemPtr itemPtr, ParsePtr parsePtr,
                       CharPtr text, Boolean byPixels, Int2 tabCount)

{
  Int2     blklen;
  Char     ch;
  Int2     col;
  ColXPtr  colFmt;
  FonT     curFont;
  FonT     fnt;
  Int2     inset;
  Int2     insetLeft;
  Int2     insetRight;
  Char     just;
  Int2     len;
  Int2     maxwid;
  Int2     numCols;
  Int2     numRows;
  Int2     returnRow;
  Int2     row;
  Uint4    start;
  Int2     tabRow;
  Boolean  tabStops;
  Int2     width;
  Boolean  wrap;

  if (itemPtr != NULL && parsePtr != NULL && text != NULL && *text != '\0') {
    curFont = NULL;
    colFmt = itemPtr->colFmt;
    if (colFmt != NULL) {
      fnt = itemPtr->font;
      if (byPixels) {
        if (fnt != NULL) {
          SelectFont (fnt);
          curFont = fnt;
        } else {
          SelectFont (systemFont);
          curFont = systemFont;
        }
      }
      start = 0;
      row = 0;
      tabRow = 0;
      returnRow = 1;
      col = 0;
      tabStops = itemPtr->tabStops;
      numRows = 0;
      numCols = itemPtr->numCols;
      while (text [start] != '\0') {
        len = 0;
        wrap = FALSE;
        ch = text [start];
        if (ch != '\0' && ch != '\n' && ch != '\r' && (ch != '\t' || tabStops)) {
          maxwid = INT2_MAX;
          if (col < numCols) {
            if (byPixels) {
              width = colFmt [col].pixWidth;
              inset = colFmt [col].pixInset;
            } else {
              width = colFmt [col].charWidth;
              inset = colFmt [col].charInset;
            }
            just = justToChar [colFmt [col].just];
            wrap = colFmt [col].wrap;
            fnt = colFmt [col].font;
          } else {
            width = 0;
            inset = 0;
            just = 'l';
            wrap = FALSE;
            fnt = NULL;
          }
          if (just == 'c') {
            insetLeft = inset;
            insetRight = inset;
          } else if (just == 'l') {
            insetLeft = inset;
            insetRight = 0;
          } else if (just == 'r') {
            insetLeft = 0;
            insetRight = inset;
          } else {
            insetLeft = 0;
            insetRight = 0;
          }
          if (byPixels) {
            if (col < numCols && wrap && width > 0 &&
                width >= 2 + insetLeft + insetRight) {
              maxwid = width - 2 - insetLeft - insetRight;
            }
          } else {
            if (col < numCols && wrap && width > 0 &&
                width >= insetLeft + insetRight) {
              maxwid = width - insetLeft - insetRight;
            }
          }
          if (byPixels) {
            if (fnt != NULL) {
              if (fnt != curFont) {
                SelectFont (fnt);
                curFont = fnt;
              }
            } else {
              fnt = itemPtr->font;
              if (fnt != NULL) {
                if (fnt != curFont) {
                  SelectFont (fnt);
                  curFont = fnt;
                }
              }
            }
          }
          blklen = GetNextBlock (text + start, maxwid, byPixels,
                                 wrap, tabStops, tabCount);
          len = blklen;
          if (len > 0) {
            if (text [start + len] != '\0') {
              while (len > 0 && text [start + len - 1] == ' ') {
                len--;
              }
              if (len == 0) {
                len = blklen;
              }
            }
            RecordCell (itemPtr, parsePtr, start, len, row, col);
            if (len > 0 && col < numCols && colFmt [col].zeroWidth) {
              if (byPixels) {
                colFmt [col].pixWidth = TextWidth (text + start, len) +
                                        CharWidth (' ') + 2;
              } else {
                colFmt [col].charWidth = len + 1;
              }
            }
            start += blklen;
          } else {
            if (byPixels) {
              while (TextWidth (text + start, len) <= maxwid) {
                len++;
              }
            } else {
              while (len <= maxwid) {
                len++;
              }
            }
            len--;
            if (len > 0) {
              RecordCell (itemPtr, parsePtr, start, len, row, col);
            }
            ch = text [start];
            while (ch != '\0' && ch != '\n' && ch != '\r' && ch != '\t') {
              start++;
              ch = text [start];
            }
          }
        }
        ch = text [start];
        if (ch == '\n') {
          start++;
          row = returnRow;
          tabRow = row;
          returnRow++;
          col = 0;
        } else if (ch == '\r') {
          start++;
          row++;
          returnRow = MAX (returnRow, row + 1);
        } else if (ch == '\t') {
          start++;
          row = tabRow;
          col++;
          if (text [start] == '\0') {
            row = returnRow;
          }
        } else if (ch != '\0' && wrap) {
          if (len == 0) {
            start++;
          }
          row++;
          returnRow = MAX (returnRow, row + 1);
        } else if (ch != '\0') {
          start++;
        } else if (text [start - 1] != '\n' && text [start - 1] != '\r') {
          row = returnRow;
        }
        numRows = MAX (numRows, row);
      }
      itemPtr->numRows = MAX (numRows, itemPtr->minLines);
    }
    SelectFont (systemFont);
  } else if (itemPtr != NULL) {
    itemPtr->numRows = 0;
  }
}

/*****************************************************************************
*
*   RearrangeText (itemPtr, parsePtr, text)
*       Reconstructs text in row and column order so that the list of cells
*       does not need to persist
*
*****************************************************************************/

static void RearrangeText (ItemPtr itemPtr, ParsePtr parsePtr, CharPtr text)

{
  Int4     cell;
  CellPtr  cellPtr;
  Int2     col;
  ColXPtr  colFmt;
  CharPtr  dst;
  Int4     len;
  Int2     minLines;
  Int2     numCells;
  Int2     numCols;
  Int2     numRows;
  Int2     row;
  CharPtr  src;
  CharPtr  str;

  if (itemPtr != NULL && parsePtr != NULL && text != NULL && *text != '\0') {
    colFmt = itemPtr->colFmt;
    if (colFmt != NULL) {
      numRows = itemPtr->numRows;
      numCols = itemPtr->numCols;
      minLines = itemPtr->minLines;
      if (parsePtr->numCells > 0 && parsePtr->cellPtr != NULL) {
        cellPtr = parsePtr->cellPtr;
        numCells = parsePtr->numCells;
        len = (Int4) (numRows * numCols);
        for (row = 0; row < numRows; row++) {
          for (col = 0; col < numCols; col++) {
            cell = (Int4) ((Int4) row * (Int4) numCols + (Int4) col);
            if (cell < (Int4) numCells) {
              len += (Int4) (cellPtr [cell].count);
            }
          }
        }
        len += (Int4) minLines + (Int4) 4;
        str = NULL;
        if ((size_t)len < SIZE_MAX) {
          str = (CharPtr) MemNew ((size_t) (Uint4) len);
          if (str != NULL) {
            dst = str;
            for (row = 0; row < numRows; row++) {
              for (col = 0; col < numCols; col++) {
                cell = (Int2) ((Int4) row * (Int4) numCols + (Int4) col);
                if (cell < numCells) {
                  if (col > 0) {
                    *dst = '\t';
                    dst++;
                  }
                  src = text + cellPtr [cell].start;
                  len = (Int4) (cellPtr [cell].count);
                  while (len > 0) {
                    *dst = *src;
                    dst++;
                    src++;
                    len--;
                  }
                }
              }
              *dst = '\n';
              dst++;
            }
            while (row < minLines) {
              *dst = '\n';
              dst++;
              row++;
            }
            *dst = '\0';
          }
        }
        itemPtr->text = str;
      }
    }
  }
}

/*****************************************************************************
*
*   FormatText (ddatptr, item, itemPtr, byPixels, tabCount)
*       Calls the print function for the item, then parses and rearranges
*       the text so that it can be displayed, saved, or printed
*
*****************************************************************************/

static void FormatText (DocDataPtr ddatptr, Int2 item, ItemPtr itemPtr,
                        Boolean byPixels, Int2 tabCount)

{
  Boolean    needToParse;
  ParseData  parseData;
  CharPtr    text;

  if (itemPtr != NULL && itemPtr->text == NULL && itemPtr->prtProc != NULL) {
    needToParse = TRUE;
    text = NULL;
    if (ddatptr != NULL && ddatptr->get != NULL) {
      text = ddatptr->get (ddatptr->doc, item);
      if (text != NULL) {
        needToParse = FALSE;
      } else {
        text = itemPtr->prtProc (ddatptr->doc, item, itemPtr->dataPtr);
      }
    } else if (ddatptr != NULL) {
      text = itemPtr->prtProc (ddatptr->doc, item, itemPtr->dataPtr);
    }
    if (needToParse && text != NULL && *text != '\0') {
      parseData.numCells = 0;
      parseData.cellPtr = NULL;
      ParseText (itemPtr, &parseData, text, byPixels, tabCount);
      RearrangeText (itemPtr, &parseData, text);
      itemPtr->notCached = FALSE;
      itemPtr->neverCached = FALSE;
      MemFree (text);
      MemFree (parseData.cellPtr);
    } else {
      itemPtr->text = text;
    }
    text = itemPtr->text;
    if (text == NULL || *text == '\0') {
      MemFree (itemPtr->text);
      itemPtr->text = StringSave (" \n");
      itemPtr->numRows = 1;
      itemPtr->notCached = FALSE;
      itemPtr->neverCached = FALSE;
      needToParse = TRUE;
    }
    if (needToParse && ddatptr != NULL && ddatptr->put != NULL) {
      ddatptr->put (ddatptr->doc, item, itemPtr->text);
    }
  }
}

/*****************************************************************************
*
*   GetStartsAt (ddatptr, item)
*       Returns the first line of the item
*
*****************************************************************************/

static Int4 GetStartsAt (DocDataPtr ddatptr, Int2 item)

{
  Int4     startsAt;
  ItemPtr  itemPtr;

  startsAt = 0;
  if (ddatptr != NULL) {
    itemPtr = GetItemPtr (ddatptr, item);
    if (itemPtr != NULL) {
      startsAt = itemPtr->startsAt;
    }
  }
  return startsAt;
}

/*****************************************************************************
*
*   GetLineHeight (ddatptr, item)
*       Returns the pixel height of the item
*
*****************************************************************************/

static Int2 GetLineHeight (DocDataPtr ddatptr, Int2 item)

{
  ItemPtr  itemPtr;
  Int2     lineHeight;

  lineHeight = 0;
  if (ddatptr != NULL) {
    itemPtr = GetItemPtr (ddatptr, item);
    if (itemPtr != NULL) {
      lineHeight = itemPtr->lineHeight;
    }
  }
  return lineHeight;
}

/*****************************************************************************
*
*   SetTablePixFormat (colFmt, left, numCols)
*       Uses pixel widths to calculate the positions of columns for drawing
*
*****************************************************************************/

static void SetTablePixFormat (ColXPtr colFmt, Int2 left, Int2 numCols)

{
  Int2  i;

  if (colFmt != NULL) {
    colFmt [0].position = left;
    for (i = 1; i < numCols; i++) {
      if (colFmt [i].left) {
        colFmt [i].position = left;
      } else {
        colFmt [i].position = colFmt [i - 1].position + colFmt [i - 1].pixWidth;
      }
    }
  }
}

/*****************************************************************************
*
*   SetTableCharFormat (colFmt, numCols)
*       Uses character counts to calculate the positions of columns for saving
*
*****************************************************************************/

static void SetTableCharFormat (ColXPtr colFmt, Int2 numCols)

{
  Int2  i;

  if (colFmt != NULL) {
    colFmt [0].position = 0;
    for (i = 1; i < numCols; i++) {
      if (colFmt [i].left) {
        colFmt [i].position = 0;
      } else {
        colFmt [i].position = colFmt [i - 1].position + colFmt [i - 1].charWidth;
      }
    }
  }
}

/*****************************************************************************
*
*   UpdateLineStarts (ddatptr, frst)
*       Recalculates the first lines as a running sum of the number of rows
*
*****************************************************************************/

static Int4 UpdateLineStarts (DocDataPtr ddatptr, Int2 frst)

{
  Int2     i;
  ItemPtr  itemPtr;
  Int4     totalNumLines;

  totalNumLines = 0;
  if (ddatptr != NULL) {
    itemPtr = GetItemPtr (ddatptr, frst);
    if (itemPtr != NULL) {
      totalNumLines = itemPtr->startsAt;
    }
    for (i = frst; i < ddatptr->numItems; i++) {
      itemPtr = GetItemPtr (ddatptr, i);
      if (itemPtr != NULL) {
        itemPtr->startsAt = totalNumLines;
        totalNumLines += itemPtr->numRows;
      }
    }
  }
  return totalNumLines;
}

/*****************************************************************************
*
*   CollectRange (item, lowest, highest)
*       Records the range of items that were manipulated for the purpose of
*       updating line starts or uncaching formatted text on items that are
*       not currently visible
*
*****************************************************************************/

static void CollectRange (Int2 item, Int2Ptr lowest, Int2Ptr highest)

{
  if (lowest != NULL) {
    *lowest = MIN (*lowest, item);
  }
  if (highest != NULL) {
    *highest = MAX (*highest, item);
  }
}

/*****************************************************************************
*
*   CacheAndFormat (ddatptr, item, itemPtr, r, tabCount)
*       Formats text (for drawing) if the item text is not currently cached
*
*****************************************************************************/

static void CacheAndFormat (DocDataPtr ddatptr, Int2 item, ItemPtr itemPtr,
                            RectPtr r, Int2 tabCount)

{
  if (itemPtr != NULL && r != NULL && itemPtr->notCached) {
    SetTablePixFormat (itemPtr->colFmt, r->left, itemPtr->numCols);
    FormatText (ddatptr, item, itemPtr, TRUE, tabCount);
  }
}

/*****************************************************************************
*
*   CacheIfNever (ddatptr, item, itemPtr, r, tabCount)
*       Formats text (for obtaining the correct number of rows) if the item
*       text has never been cached
*
*****************************************************************************/

static void CacheIfNever (DocDataPtr ddatptr, Int2 item, ItemPtr itemPtr,
                          RectPtr r, Int2 tabCount)

{
  if (itemPtr != NULL && r != NULL && itemPtr->neverCached) {
    SetTablePixFormat (itemPtr->colFmt, r->left, itemPtr->numCols);
    FormatText (ddatptr, item, itemPtr, TRUE, tabCount);
  }
}

/*****************************************************************************
*
*   FreeCachedItem (ddatptr, item, lowest, highest)
*       Frees the formatted text string for an item that is not currently
*       visible
*
*****************************************************************************/

static void FreeCachedItem (DocDataPtr ddatptr, Int2 item,
                            Int2Ptr lowest, Int2Ptr highest)

{
  ItemPtr  itemPtr;

  if (ddatptr != NULL) {
    itemPtr = GetItemPtr (ddatptr, item);
    if (itemPtr != NULL) {
      itemPtr->text = (CharPtr) MemFree (itemPtr->text);
      itemPtr->notCached = TRUE;
    }
  }
}

/*****************************************************************************
*
*   VisLinesAbove (ddatptr, r, item, line, lowest, highest)
*       Returns the number of lines visible on the screen if the given line
*       in the given item is the last line visible
*
*****************************************************************************/

static Int2 VisLinesAbove (DocDataPtr ddatptr, RectPtr r, Int2 item,
                           Int2 line, Int2Ptr lowest, Int2Ptr highest)

{
  ItemPtr  itemPtr;
  Int2     pixels;
  Int2     vis;

  vis = 0;
  if (ddatptr != NULL && r != NULL && item < ddatptr->numItems) {
    pixels = r->bottom - r->top;
    while (pixels > 0 && item >= 0) {
      itemPtr = GetItemPtr (ddatptr, item);
      if (itemPtr != NULL) {
        CollectRange (item, lowest, highest);
        CacheIfNever (ddatptr, item + 1, itemPtr, r, ddatptr->tabCount);
        if (line < 0) {
          line = itemPtr->numRows - 1;
        }
        while (pixels > 0 && line >= 0 && line < itemPtr->numRows) {
          pixels -= itemPtr->lineHeight;
          if (pixels >= 0) {
            vis++;
          }
          line--;
        }
        pixels -= itemPtr->leadHeight;
      }
      item--;
    }
  }
  return vis;
}

/*****************************************************************************
*
*   VisLinesBelow (ddatptr, r, item, line, lowest, highest)
*       Returns the number of lines visible on the screen if the given line
*       in the given item is the first line visible
*
*****************************************************************************/

static Int2 VisLinesBelow (DocDataPtr ddatptr, RectPtr r, Int2 item,
                           Int2 line, Int2Ptr lowest, Int2Ptr highest)

{
  ItemPtr  itemPtr;
  Int2     pixels;
  Int2     vis;

  vis = 0;
  if (ddatptr != NULL && r != NULL && item < ddatptr->numItems && item >= 0) {
    pixels = r->bottom - r->top;
    while (pixels > 0 && item < ddatptr->numItems) {
      itemPtr = GetItemPtr (ddatptr, item);
      if (itemPtr != NULL) {
        CollectRange (item, lowest, highest);
        CacheIfNever (ddatptr, item + 1, itemPtr, r, ddatptr->tabCount);
        if (line  < 0) {
          line = 0;
          pixels -= itemPtr->leadHeight;
        }
        while (pixels > 0 && line >= 0 && line < itemPtr->numRows) {
          pixels -= itemPtr->lineHeight;
          if (pixels >= 0) {
            vis++;
          }
          line++;
        }
        line = -1;
      }
      item++;
    }
  }
  return vis;
}

/*****************************************************************************
*
*   PixelsBetween (ddatptr, r, firstLine, lastLine, lowest, highest)
*       Returns the number of pixels between two lines
*
*****************************************************************************/

static Int2 PixelsBetween (DocDataPtr ddatptr, RectPtr r, Int4 firstLine,
                           Int4 lastLine, Int2Ptr lowest, Int2Ptr highest)

{
  Int2     count;
  Int2     item;
  ItemPtr  itemPtr;
  Int4     line;
  Int4     numLines;
  Int2     onItem;
  Int2     pixels;

  pixels = 0;
  if (ddatptr != NULL && r != NULL) {
    count = ABS (lastLine - firstLine);
    item = GetItemNum (ddatptr, MIN (firstLine, lastLine));
    itemPtr = GetItemPtr (ddatptr, item);
    onItem = item;
    if (itemPtr != NULL && item >= 0) {
      CollectRange (item, lowest, highest);
      CacheIfNever (ddatptr, item + 1, itemPtr, r, ddatptr->tabCount);
      line = MIN (firstLine, lastLine) - itemPtr->startsAt;
      while (count > 0 && item < ddatptr->numItems) {
        if (item != onItem) {
          itemPtr = GetItemPtr (ddatptr, item);
          onItem = item;
        }
        numLines = 0;
        if (itemPtr != NULL) {
          CollectRange (item, lowest, highest);
          CacheIfNever (ddatptr, item + 1, itemPtr, r, ddatptr->tabCount);
          numLines = itemPtr->numRows;
          while (count > 0 && line >= 0 && line < numLines) {
            pixels += itemPtr->lineHeight;
            line++;
            count--;
          }
        }
        item++;
        if (line >= numLines) {
          itemPtr = GetItemPtr (ddatptr, item);
          onItem = item;
          if (itemPtr != NULL) {
            CollectRange (item, lowest, highest);
            CacheIfNever (ddatptr, item + 1, itemPtr, r, ddatptr->tabCount);
            pixels += itemPtr->leadHeight;
          }
        }
        line = 0;
      }
      if (lastLine < firstLine) {
        pixels = -pixels;
      }
    }
  }
  return pixels;
}

/*****************************************************************************
*
*   DrawTableItem (d, ddatptr, itemPtr, r, item, frst, grayProc,
                   invertProc, colorProc, force, tabCount)
*       Draws a given item on the screen, suppressing partial lines
*
*****************************************************************************/

static Boolean is_old_win = FALSE;
static Boolean old_win_set = FALSE;

static Int2 DrawTableItem (DoC d, DocDataPtr ddatptr, ItemPtr itemPtr, RectPtr r,
                          Int2 item, Int2 frst, DocShadeProc grayProc,
                          DocShadeProc invertProc, DocShadeProc colorProc,
                          Boolean force, Int2 tabCount)

{
  Int2     barBottom;
  Int2     barTop;
  Char     ch;
  Int2     col;
  Boolean  color;
  ColXPtr  colFmt;
  FonT     curFont;
  Uint4    currentColor;
  Boolean  drawBars;
  RecT     drw;
  FonT     fnt;
  Boolean  gray;
  Int2     insetLeft;
  Int2     insetRight;
  Boolean  invert;
  Char     just;
  Int2     lineHeight;
  Int2     num;
  CharPtr  ptr;
  RecT     rct;
  Int2     row;
  Int2     rsult;
  CharPtr  text;
  CharPtr  tmp;

#ifdef OS_MSWIN
  if (! old_win_set) {
    text = GetOpSysString ();
    if (StringICmp (text, "MS WINDOWS 95/98/Me") == 0 ||
        StringICmp (text, "MS WINDOWS 3.1") == 0) {
      is_old_win = TRUE;
    }
    text = MemFree (text);
    old_win_set = TRUE;
  }
#endif

  rsult = 0;
  if (d != NULL && ddatptr != NULL && itemPtr != NULL && r != NULL) {
    text = itemPtr->text;
    colFmt = itemPtr->colFmt;
    if (text != NULL && *text != '\0' && colFmt != NULL) {
      fnt = itemPtr->font;
      if (fnt != NULL) {
        curFont = fnt;
      } else {
        curFont = systemFont;
      }
      SelectFont (curFont);
      lineHeight = itemPtr->lineHeight;
      rct.top = r->top;
      rct.bottom = rct.top;
      row = 0;
      col = 0;
      ptr = text;
      ch = *ptr;
      currentColor = 0;
      if (colorProc != NULL && (! force)) {
        currentColor = GetColor ();
      }
      drawBars = FALSE;
      barTop = INT2_MAX;
      barBottom = INT2_MIN;
      while (ch != '\0') {
        if (ch == '\n') {
          rct.top = r->top + (row - frst) * lineHeight;
          rct.bottom = rct.top + lineHeight;
          InvertMode ();
          for (col = 0; col < itemPtr->numCols; col++) {
            rct.left = colFmt [col].position;
            rct.right = rct.left + colFmt [col].pixWidth;
            if (colFmt [col].bar) {
              drawBars = TRUE;
              barTop = MIN (barTop, rct.top);
              barBottom = MAX (barBottom, rct.bottom);
            }
            if (colFmt [col].underline) {
              MoveTo (rct.left, rct.bottom - 1);
              LineTo (rct.right, rct.bottom - 1);
            }
          }
          CopyMode ();
          col = 0;
          row++;
          ptr++;
        } else if (ch == '\t' && (! (itemPtr->tabStops))) {
          col++;
          ptr++;
          ch = *ptr;
          if (ch == '\t' && (! (itemPtr->tabStops)) &&
              row >= frst && col < itemPtr->numCols) {
            rct.top = r->top + (row - frst) * lineHeight;
            rct.bottom = rct.top + lineHeight;
            rct.left = colFmt [col].position;
            rct.right = rct.left + colFmt [col].pixWidth;
            LoadRect (&drw, MAX (rct.left, r->left), rct.top,
                      MIN (rct.right, r->right), rct.bottom);
            if (RectInRect (&drw, r) &&
                (force || updateRgn == NULL || RectInRgn (&rct, updateRgn))) {
              invert = FALSE;
              if (invertProc != NULL && (! force)) {
                invert = invertProc (d, item + 1, row + 1, col + 1);
              } else if (ddatptr->useRowsNotItems) {
                invert = (Boolean) (ddatptr->firstHighlightItem <= row + 1 &&
                                    ddatptr->lastHighlightItem >= row + 1);
              } else {
                invert = (Boolean) (ddatptr->firstHighlightItem <= item + 1 &&
                                    ddatptr->lastHighlightItem >= item + 1);
              }
              if (invert) {
                InvertColors ();
                EraseRect (&rct);
                InvertColors ();
              }
            }
          }
        } else  {
          num = 0;
          while (ch != '\0' && (ch != '\t' || itemPtr->tabStops) && ch != '\n') {
            num++;
            ch = ptr [num];
          }
          if (row >= frst && col < itemPtr->numCols) {
            rct.top = r->top + (row - frst) * lineHeight;
            rct.bottom = rct.top + lineHeight;
            rct.left = colFmt [col].position;
            rct.right = rct.left + colFmt [col].pixWidth;
            LoadRect (&drw, MAX (rct.left, r->left), rct.top,
                      MIN (rct.right, r->right), rct.bottom);
            if (RectInRect (&drw, r) &&
                (force || updateRgn == NULL || RectInRgn (&rct, updateRgn))) {
              just = justToChar [colFmt [col].just];
              if (just == 'c') {
                insetLeft = colFmt [col].pixInset;
                insetRight = colFmt [col].pixInset;
              } else if (just == 'l') {
                insetLeft = colFmt [col].pixInset;
                insetRight = 0;
              } else if (just == 'r') {
                insetLeft = 0;
                insetRight = colFmt [col].pixInset;
              } else {
                insetLeft = 0;
                insetRight = 0;
              }
              fnt = colFmt [col].font;
              if (fnt != NULL) {
                if (fnt != curFont) {
                  curFont = fnt;
                  SelectFont (curFont);
                }
              } else {
                fnt = itemPtr->font;
                if (fnt != NULL) {
                  if (fnt != curFont) {
                    curFont = fnt;
                    SelectFont (curFont);
                  }
                }
              }
              color = FALSE;
              if (colorProc != NULL && (! force)) {
                color = colorProc (d, item + 1, row + 1, col + 1);
                if (! color) {
                  SetColor (currentColor);
                }
              }
              invert = FALSE;
              if (invertProc != NULL && (! force)) {
                invert = invertProc (d, item + 1, row + 1, col + 1);
              } else if (ddatptr->useRowsNotItems) {
                invert = (Boolean) (ddatptr->firstHighlightItem <= row + 1 &&
                                    ddatptr->lastHighlightItem >= row + 1);
              } else {
                invert = (Boolean) (ddatptr->firstHighlightItem <= item + 1 &&
                                    ddatptr->lastHighlightItem >= item + 1);
              }
              gray = FALSE;
              if (grayProc != NULL && (! force)) {
                gray = grayProc (d, item + 1, row + 1, col + 1);
              }
              SelectFont (curFont);
              if (invert) {
                InvertColors ();
                EraseRect (&rct);
              }
              rct.left += insetLeft;
              rct.right -= insetRight;
              drw = rct;
              if (num > 0) {
                while (num > 0 && *ptr == '\t') {
                  num--;
                  ptr++;
                  drw.left += CharWidth (' ') * tabCount;
                }
                if (ddatptr->drawcell != NULL) {
                  tmp = (CharPtr) MemNew((size_t)(num + 2));
                  if (tmp != NULL) {
                    MemCopy (tmp, ptr, num);
                    ddatptr->drawcell (d, item + 1, row + 1, col + 1, &drw, tmp, just, gray);
                    MemFree (tmp);
                  }
                } else {
                  DrawText (&drw, ptr, num, just, gray);
                }
              }
              if (invert) {
                InvertColors ();
              }
              if (colorProc != NULL && (! force)) {
                SetColor (currentColor);
              }
              rct.left -= insetLeft;
              rct.right += insetRight;
            }
          }
          ptr += num;
        }
        ch = *ptr;
      }
      if (drawBars) {
        InvertMode ();
        for (col = 0; col < itemPtr->numCols; col++) {
          rct.left = colFmt [col].position;
          rct.right = rct.left + colFmt [col].pixWidth;
          if (colFmt [col].bar) {
            drawBars = TRUE;
            MoveTo (rct.left, barTop);
/*
#ifdef WIN_MAC
            LineTo (rct.left, barBottom -1);
#else
            LineTo (rct.left, barBottom);
#endif
*/
            if (is_old_win) {
              LineTo (rct.left, barBottom);
            } else {
              LineTo (rct.left, barBottom - 1);
            }
          }
        }
        CopyMode ();
      }
      rct.top = rct.bottom;
      rsult = rct.top - r->top;
    }
    SelectFont (systemFont);
  }
  return rsult;
}

/*****************************************************************************
*
*   DrawDocument (d)
*       Panel callback that traverses the item list and draws visible items
*       Clipping is to the intersection of the updateRgn (the portion of the
*       panel exposed) and the inset area of the document, leaving a four-
*       pixel margin
*
*****************************************************************************/

static void DrawDocument (PaneL d)

{
  DocData  ddata;
  RegioN   dst;
  Int4     firstLine;
  Int2     highest;
  Int2     item;
  ItemPtr  itemPtr;
  Int2     lowest;
  Int2     numRows;
  Int4     off;
  Int2     pixels;
  RecT     r;
  RecT     rct;
  BaR      sb;
  RegioN   src;

  GetPanelExtra (d, &ddata);
  if (ddata.numItems > 0) {
    ObjectRect (d, &r);
    InsetRect (&r, 4, 4);
    sb = GetSlateVScrollBar ((SlatE) d);
    if (sb != NULL) {
      src = CreateRgn ();
      dst = CreateRgn ();
      LoadRectRgn (src, r.left, r.top, r.right, r.bottom);
      SectRgn (src, updateRgn, dst);
      ClipRgn (dst);
      DestroyRgn (src);
      DestroyRgn (dst);
      off = GetBarValue (sb);
      pixels = 0;
      lowest = INT2_MAX;
      highest = INT2_MIN;
      item = GetItemNum (&ddata, off);
      itemPtr = GetItemPtr (&ddata, item);
      if (itemPtr != NULL) {
        firstLine = off - itemPtr->startsAt;
        do {
          if (itemPtr != NULL) {
            if (itemPtr->neverCached) {
              CollectRange (item, &lowest, &highest);
            }
            CacheAndFormat (&ddata, item + 1, itemPtr, &r, ddata.tabCount);
            SetTablePixFormat (itemPtr->colFmt, r.left, itemPtr->numCols);
            if (pixels != 0) {
              r.top += itemPtr->leadHeight;
            }
            rct = r;
            numRows = MIN (itemPtr->numRows - firstLine,
                           MAX ((r.bottom - rct.top) / itemPtr->lineHeight, 0));
            rct.bottom = rct.top + itemPtr->lineHeight * (itemPtr->numRows - firstLine);
            if (updateRgn == NULL || RectInRgn (&rct, updateRgn)) {
              pixels = DrawTableItem ((DoC) d, &ddata, itemPtr, &r, item, firstLine,
                                      ddata.gray, ddata.invert, ddata.color,
                                      FALSE, ddata.tabCount);
              if (ddata.draw != NULL) {
                rct.bottom = rct.top + itemPtr->lineHeight * numRows;
                if (RectInRect (&rct, &r) &&
                    (updateRgn == NULL || RectInRgn (&rct, updateRgn))) {
                  ddata.draw ((DoC) d, &rct, item + 1, firstLine);
                }
                ResetDrawingTools ();
              }
            } else {
              pixels = rct.bottom - rct.top;
            }
            r.top += pixels;
          }
          item++;
          firstLine = 0;
          itemPtr = GetItemPtr (&ddata, item);
        } while (r.top < r.bottom && item < ddata.numItems && pixels > 0);
        if (lowest < INT2_MAX) {
          ddata.numLines = UpdateLineStarts (&ddata, lowest);
          SetPanelExtra (d, &ddata);
        }
      }
      ResetClip ();
    }
  }
  SelectFont (systemFont);
}

/*****************************************************************************
*
*   DocumentScrlProc (sb, s, newval, oldval)
*       Scroll bar callback that takes suppressed partial lines into account
*
*****************************************************************************/

static void DocumentScrlProc (BaR sb, SlatE s, Int4 newval, Int4 oldval)

{
  Int4     barmax;
  Int4     barval;
  DocData  ddata;
  Int2     firstShown;
  Boolean  goToEnd;
  Int2     height;
  Int2     highest;
  Int2     highFree;
  Int2     item;
  ItemPtr  itemPtr;
  Int4     last;
  Int2     limit;
  Int2     line;
  Int2     lineInto;
  Int2     lowest;
  Int2     lowFree;
  Int4     min;
  Int2     pixels;
  Int2     pgDn;
  Int2     pgUp;
  RecT     r;
  Int2     vis;

  if (s != NULL && oldval != newval) {
    if (Visible (s) && AllParentsVisible (s)) {
      GetPanelExtra ((PaneL) s, &ddata);
      ObjectRect (s, &r);
      InsetRect (&r, 4, 4);
      height = r.bottom - r.top;

      Select (s);
      if (ddata.pgUp < 1 || ddata.pgDn < 1) {
        InsetRect (&r, -1, -1);
        InvalRect (&r);
      } else if ((newval > oldval && newval <= oldval + ddata.pgDn) ||
          (newval < oldval && newval >= oldval - ddata.pgUp)) {
        pixels = PixelsBetween (&ddata, &r, newval, oldval, NULL, NULL);
        if (ABS (pixels) < height) {
          min = MIN (oldval, newval);
          item = GetItemNum (&ddata, min);
          line = (Int2) (min - GetStartsAt (&ddata, item));
          vis = VisLinesBelow (&ddata, &r, item, line, NULL, NULL);
          last = min + vis - 1;
          limit = PixelsBetween (&ddata, &r, newval, last, NULL, NULL);
          item = GetItemNum (&ddata, last);
          line = (Int2) (last - GetStartsAt (&ddata, item));
          limit += GetLineHeight (&ddata, item);
          ScrollRect (&r, 0, pixels);
          r.top += limit;
          InsetRect (&r, -1, -1);
          InvalRect (&r);
        } else {
          InsetRect (&r, -1, -1);
          InvalRect (&r);
        }
      } else {
        InsetRect (&r, -1, -1);
        InvalRect (&r);
      }

      min = MIN (oldval, newval);
      item = GetItemNum (&ddata, min);
      ddata.numLines = UpdateLineStarts (&ddata, item);
      SetPanelExtra ((PaneL) s, &ddata);

      item = GetItemNum (&ddata, newval);
      line = (Int2) (newval - GetStartsAt (&ddata, item));
      firstShown = item;
      lineInto = line;

      barval = GetStartsAt (&ddata, firstShown) + lineInto;
      if (barval > ddata.barmax) {
        CorrectBarMax (sb, barval);
        CorrectBarValue (sb, barval);
      } else {
        CorrectBarValue (sb, barval);
      }
      Update ();

      lowest = INT2_MAX;
      highest = INT2_MIN;
      ObjectRect (s, &r);
      InsetRect (&r, 4, 4);
      pgUp = VisLinesAbove (&ddata, &r, firstShown, lineInto, &lowest, &highest) - 1;
      pgDn = VisLinesBelow (&ddata, &r, firstShown, lineInto, &lowest, &highest) - 1;
      if (pgDn < 1) {
        pgDn = 1;
      }
      if (pgUp < 1) {
        pgUp = 1;
      }
      /*
      item = GetItemNum (&ddata, newval + pgDn);
      line = (Int2) (newval + (Int4) pgDn - GetStartsAt (&ddata, item));
      VisLinesBelow (&ddata, &r, item, line, &lowest, &highest);
      */
      if (lowest < INT2_MAX) {
        ddata.numLines = UpdateLineStarts (&ddata, lowest);
        SetPanelExtra ((PaneL) s, &ddata);
      }

      lowFree = INT2_MAX;
      highFree = INT2_MIN;
      for (item = 0; item < lowest; item++) {
        FreeCachedItem (&ddata, item, &lowFree, &highFree);
      }
      for (item = highest + 1; item < ddata.numItems; item++) {
        FreeCachedItem (&ddata, item, &lowFree, &highFree);
      }
      if (lowFree < INT2_MAX) {
        ddata.numLines = UpdateLineStarts (&ddata, lowFree);
        SetPanelExtra ((PaneL) s, &ddata);
      }

      barmax = 0;
      goToEnd = (Boolean) (newval == ddata.barmax);
      if (ddata.numLines > 0 && ddata.numItems > 0) {
        itemPtr = GetItemPtr (&ddata, ddata.numItems - 1);
        if (itemPtr != NULL) {
          ObjectRect (s, &r);
          InsetRect (&r, 4, 4);
          lowest = INT2_MAX;
          highest = INT2_MIN;
          vis = VisLinesAbove (&ddata, &r, ddata.numItems - 1,
                               itemPtr->numRows - 1, &lowest, &highest);
          if (lowest < INT2_MAX) {
            ddata.numLines = UpdateLineStarts (&ddata, lowest);
            SetPanelExtra ((PaneL) s, &ddata);
          }
          barmax = ddata.numLines - vis;
        }
      }
      barval = GetStartsAt (&ddata, firstShown) + lineInto;
      if (goToEnd) {
        barval = barmax;
      }
      if (barval > ddata.barmax) {
        CorrectBarMax (sb, barmax);
        CorrectBarValue (sb, barval);
      } else {
        CorrectBarValue (sb, barval);
        CorrectBarMax (sb, barmax);
      }
      CorrectBarPage (sb, pgUp, pgDn);
      ddata.barmax = barmax;
      ddata.pgUp = pgUp;
      ddata.pgDn = pgDn;
      SetPanelExtra ((PaneL) s, &ddata);
      Update ();
      if (ddata.pan != NULL) {
        Select (s);
        ddata.pan ((DoC) s);
      }
    }
  }
}

/*****************************************************************************
*
*   CreateItemPtr (d, item)
*       Adds an item slot to the item list
*
*****************************************************************************/

static ItemPtr CreateItemPtr (DoC d, Int2 item)

{
  DocData    ddata;
  Int2       index;
  Int2       list;
  ItemPtr    itemPtr;
  ListDPtr   listPtr;
  MasterPtr  masterPtr;

  itemPtr = NULL;
  if (d != NULL && item < 32767) {
    GetPanelExtra ((PaneL) d, &ddata);
    if (ddata.master == NULL) {
      ddata.master = (MasterPtr) MemNew (sizeof (MasterData));
      SetPanelExtra ((PaneL) d, &ddata);
    }
    list = (item / LISTSIZE);
    if (list < MAXLISTS && ddata.master != NULL) {
      masterPtr = ddata.master;
      listPtr = masterPtr->list [list];
      if (listPtr == NULL) {
        masterPtr->list [list] = (ListDPtr) MemNew(sizeof(ListData));
        listPtr = masterPtr->list [list];
      }
      if (listPtr != NULL) {
        index = (item % LISTSIZE);
        itemPtr = &(listPtr->items [index]);
      }
    }
  }
  return itemPtr;
}

/*****************************************************************************
*
*   GetFontHeight (font)
*       Returns the pixel height of a font, caching for quicker access
*
*****************************************************************************/

static Int2 GetFontHeight (FonT font)

{
  Int2  height;
  Int2  i;

  height = 0;
  i = 0;
  while (i < MAXFONTS && fontHeights [i].font != font && fontHeights [i].font != NULL) {
    i++;
  }
  if (fontHeights [i].font == font) {
    height = fontHeights [i].height;
  } else {
    SelectFont (font);
    height = LineHeight ();
    if (i < MAXFONTS) {
      fontHeights [i].font = font;
      fontHeights [i].height = height;
    }
  }
  return height;
}

/*****************************************************************************
*
*   UpdateItemHeights (itemPtr)
*       Returns the maximum height of all fonts used in an item
*
*****************************************************************************/

static void UpdateItemHeights (ItemPtr itemPtr)

{
  ColXPtr  colFmt;
  FonT     fnt;
  Int2     i;
  Int2     linhgt;
  Int2     maxhgt;

  if (itemPtr != NULL) {
    colFmt = itemPtr->colFmt;
    maxhgt = 0;
    if (colFmt != NULL) {
      i = itemPtr->numCols;
      while (i > 0) {
        i--;
        fnt = colFmt [i].font;
        if (fnt != NULL) {
          linhgt = GetFontHeight (fnt);
          if (linhgt > maxhgt) {
            maxhgt = linhgt;
          }
        } else if (itemPtr->font == NULL) {
          linhgt = stdLineHeight;
          if (linhgt > maxhgt) {
            maxhgt = linhgt;
          }
        }
      }
    }
    fnt = itemPtr->font;
    if (fnt != NULL) {
      linhgt = GetFontHeight (fnt);
      if (linhgt > maxhgt) {
        maxhgt = linhgt;
      }
    }
    if (maxhgt == 0) {
      maxhgt = stdLineHeight;
    }
    itemPtr->lineHeight = MAX (maxhgt, itemPtr->minHeight);
    if (itemPtr->openSpace) {
      itemPtr->leadHeight = maxhgt;
    } else {
      itemPtr->leadHeight = 0;
    }
  }
}

/*****************************************************************************
*
*   SetDocAutoAdjust (d, autoAdjust)
*       Sets the document automatic scroll adjust parameter
*
*****************************************************************************/

extern void SetDocAutoAdjust (DoC d, Boolean autoAdjust)

{
  DocData  ddata;

  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    ddata.autoAdjust = autoAdjust;
    SetPanelExtra ((PaneL) d, &ddata);
  }
}

/* dgg here -- want to make use of tab settings other than from FancyFile call */

/*****************************************************************************
*
*   Nlm_SetDocTabstops (d, tabStops)
*       Sets the document tap stop interval
*
*****************************************************************************/

extern void Nlm_SetDocTabstops (DoC d, Int2 tabStops)
{
  DocData  ddata;

  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    ddata.tabCount = tabStops;
    SetPanelExtra ((PaneL) d, &ddata);
  }
}

/*****************************************************************************
*
*   AdjustDocScroll (d)
*       Calculates an estimate for the scroll bar maximum
*
*****************************************************************************/

extern void AdjustDocScroll (DoC d)

{
  Int4     barmax;
  Int4     barval;
  DocData  ddata;
  Int2     firstShown;
  Int2     highest;
  Int2     item;
  ItemPtr  itemPtr;
  Int2     line;
  Int2     lineInto;
  Int2     lowest;
  Int2     pgDn;
  Int2     pgUp;
  RecT     r;
  BaR      sb;
  Int2     vis;

  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    SelectFont (systemFont);
    sb = GetSlateVScrollBar ((SlatE) d);
    if (sb != NULL) {
      if (ddata.numLines > 0 && ddata.numItems > 0) {
        ObjectRect (d, &r);
        InsetRect (&r, 4, 4);
        barval = GetBarValue (sb);
        item = GetItemNum (&ddata, barval);
        line = (Int2) (barval - GetStartsAt (&ddata, item));
        firstShown = item;
        lineInto = line;
        lowest = INT2_MAX;
        highest = INT2_MIN;
        itemPtr = GetItemPtr (&ddata, ddata.numItems - 1);
        if (itemPtr != NULL) {
          CacheIfNever (&ddata, ddata.numItems, itemPtr, &r, ddata.tabCount);
          vis = VisLinesAbove (&ddata, &r, ddata.numItems - 1,
                               itemPtr->numRows - 1, &lowest, &highest);
        }
        pgUp = VisLinesAbove (&ddata, &r, firstShown, lineInto, &lowest, &highest) - 1;
        pgDn = VisLinesBelow (&ddata, &r, firstShown, lineInto, &lowest, &highest) - 1;
        if (pgDn < 1) {
          pgDn = 1;
        }
        if (pgUp < 1) {
          pgUp = 1;
        }
        /*
        item = GetItemNum (&ddata, barval + pgDn);
        line = (Int2) (barval + (Int4) pgDn - GetStartsAt (&ddata, item));
        VisLinesBelow (&ddata, &r, item, line, &lowest, &highest);
        */
        if (lowest < INT2_MAX) {
          ddata.numLines = UpdateLineStarts (&ddata, lowest);
          SetPanelExtra ((PaneL) d, &ddata);
        }
        itemPtr = GetItemPtr (&ddata, ddata.numItems - 1);
        if (itemPtr != NULL) {
          barmax = 0;
          ObjectRect (d, &r);
          InsetRect (&r, 4, 4);
          lowest = INT2_MAX;
          highest = INT2_MIN;
          vis = VisLinesAbove (&ddata, &r, ddata.numItems - 1,
                               itemPtr->numRows - 1, &lowest, &highest);
          if (lowest < INT2_MAX) {
            ddata.numLines = UpdateLineStarts (&ddata, lowest);
            SetPanelExtra ((PaneL) d, &ddata);
          }
          barmax = ddata.numLines - vis;
          barval = GetStartsAt (&ddata, firstShown) + lineInto;
          CorrectBarPage (sb, pgUp, pgDn);
          if (barval > ddata.barmax) {
            CorrectBarMax (sb, barmax);
            CorrectBarValue (sb, barval);
          } else {
            CorrectBarValue (sb, barval);
            CorrectBarMax (sb, barmax);
          }
          ddata.barmax = barmax;
          ddata.pgUp = pgUp;
          ddata.pgDn = pgDn;
          SetPanelExtra ((PaneL) d, &ddata);
        }
      } else {
        Reset (sb);
      }
    }
  }
}

/*****************************************************************************
*
*   ForceFormat (d, item)
*       Forces text format
*
*****************************************************************************/

extern void ForceFormat (DoC d, Int2 item)

{
  DocData  ddata;
  ItemPtr  itemPtr;
  RecT     r;

  if (d != NULL && item > 0) {
    GetPanelExtra ((PaneL) d, &ddata);
    itemPtr = GetItemPtr (&ddata, item - 1);
    if (itemPtr != NULL) {
      ObjectRect (d, &r);
      InsetRect (&r, 4, 4);
      itemPtr->text = (CharPtr) MemFree (itemPtr->text);
      itemPtr->notCached = TRUE;
      itemPtr->neverCached = TRUE;
      SetTablePixFormat (itemPtr->colFmt, r.left, itemPtr->numCols);
      FormatText (&ddata, item, itemPtr, TRUE, ddata.tabCount);
      UpdateItemHeights (itemPtr);
      ddata.numLines = UpdateLineStarts (&ddata, 0);
      SetPanelExtra ((PaneL) d, &ddata);
      if (ddata.autoAdjust) {
        AdjustDocScroll (d);
      }
    }
  }
}

/*****************************************************************************
*
*   defaultTable
*       Column format used if the colFmt parameter is NULL
*
*****************************************************************************/

static ColData defaultTable = {0, 0, 0, 0, NULL, 'l',
                               TRUE, FALSE, FALSE, FALSE, TRUE};

/*****************************************************************************
*
*   CacheColFmt (d, colFmt)
*       Caches the colFmt format structure
*
*****************************************************************************/

static ColXPtr CacheColFmt (DoC d, ColPtr colFmt)

{
  DocData     ddata;
  Boolean     found;
  Boolean     goOn;
  Int2        i;
  Int2        numCols;
  ColXPtr     rsult;
  ColXPtr     thisCol;
  ValNodePtr  vnp;

  rsult = NULL;
  if (d != NULL && colFmt != NULL) {
    i = 0;
    while (! colFmt [i].last) {
      i++;
    }
    i++;
    numCols = i;
    rsult = (ColXPtr) MemNew (numCols * sizeof (ColXData));
    if (rsult != NULL) {
      for (i = 0; i < numCols; i++) {
        rsult [i].position = 0;
        rsult [i].pixWidth = colFmt [i].pixWidth;
        rsult [i].pixInset = colFmt [i].pixInset;
        rsult [i].charWidth = colFmt [i].charWidth;
        rsult [i].charInset = colFmt [i].charInset;
        if (colFmt [i].just == 'l') {
          rsult [i].just = JUST_LEFT;
        } else if (colFmt [i].just == 'c') {
          rsult [i].just = JUST_CENTER;
        } else if (colFmt [i].just == 'r') {
          rsult [i].just = JUST_RIGHT;
        } else {
          rsult [i].just = JUST_LEFT;
        }
        rsult [i].font = colFmt [i].font;
        rsult [i].zeroWidth = (Boolean) (colFmt [i].pixWidth == 0);
        if (rsult [i].zeroWidth) {
          rsult [i].charWidth = 0;
        }
        rsult [i].wrap = colFmt [i].wrap;
        rsult [i].bar = colFmt [i].bar;
        rsult [i].underline = colFmt [i].underline;
        rsult [i].left = colFmt [i].left;
        rsult [i].last = colFmt [i].last;
      }
      GetPanelExtra ((PaneL) d, &ddata);
      vnp = ddata.colFmts;
      goOn = TRUE;
      while (vnp != NULL && goOn) {
        thisCol = (ColXPtr) vnp->data.ptrvalue;
        if (thisCol != NULL) {
          i = 0;
          while (! thisCol [i].last) {
            i++;
          }
          i++;
          if (i == numCols) {
            found = TRUE;
            for (i = 0; i < numCols; i++) {
              if (thisCol [i].zeroWidth) {
                thisCol [i].pixWidth = 0;
                thisCol [i].charWidth = 0;
              }
              if (thisCol [i].font != rsult [i].font ||
                  thisCol [i].pixWidth != rsult [i].pixWidth ||
                  thisCol [i].pixInset != rsult [i].pixInset ||
                  thisCol [i].charWidth != rsult [i].charWidth ||
                  thisCol [i].charInset != rsult [i].charInset ||
                  thisCol [i].just != rsult [i].just ||
                  thisCol [i].zeroWidth != rsult [i].zeroWidth ||
                  thisCol [i].wrap != rsult [i].wrap ||
                  thisCol [i].bar != rsult [i].bar ||
                  thisCol [i].underline != rsult [i].underline ||
                  thisCol [i].left != rsult [i].left ||
                  thisCol [i].last != rsult [i].last) {
                found = FALSE;
              }
            }
            if (found) {
              goOn = FALSE;
            }
          }
        }
        if (goOn) {
          vnp = vnp->next;
        }
      }
      if (goOn) {
        vnp = ValNodeNew (ddata.colFmts);
        if (vnp != NULL) {
          vnp->data.ptrvalue = (Pointer) rsult;
        }
        if (ddata.colFmts == NULL) {
          ddata.colFmts = vnp;
          SetPanelExtra ((PaneL) d, &ddata);
        }
      } else {
        MemFree (rsult);
        rsult = (ColXPtr) vnp->data.ptrvalue;
      }
    }
  }
  return rsult;
}

/*****************************************************************************
*
*   UpdateColFmt (d, *col)
*       Returns the maximum height of all fonts used in an item
*
*****************************************************************************/

extern void UpdateColFmt (DoC d, ColPtr col)

{
  DocData  dd;
  ItemPtr  itemPtr;
  Int2     item;

  GetPanelExtra ((PaneL) d, &dd);
  for (item = 0; item < dd.numItems; item++) {
    itemPtr = GetItemPtr (&dd, item);
    if (itemPtr != NULL) {
      itemPtr->colFmt = CacheColFmt (d, col);
    }
  }
  SetPanelExtra ((PaneL) d, &dd);
}

/*****************************************************************************
*
*   SetDocDefaults (d, defaultParFmt, defaultColFmt, defaultFont)
*       Sets default format preferences, so AppendItem, etc., can pass NULLs
*
*****************************************************************************/

extern void SetDocDefaults (DoC d, ParPtr defaultParFmt,
                            ColPtr defaultColFmt, FonT defaultFont)

{
  DocData  ddata;

  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    ddata.defaultParFmt = defaultParFmt;
    ddata.defaultColFmt = defaultColFmt;
    ddata.defaultFont = defaultFont;
    SetPanelExtra ((PaneL) d, &ddata);
  }
}

/*****************************************************************************
*
*   SetDocSimpleMode (d, useRowsNotItems)
*       Sets default highlighting range
*
*****************************************************************************/

extern void SetDocSimpleMode (DoC d, Boolean useRowsNotItems)

{
  DocData  ddata;

  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    ddata.useRowsNotItems = useRowsNotItems;
    SetPanelExtra ((PaneL) d, &ddata);
  }
}
/*****************************************************************************
*
*   SetDocHighlight (d, firstItem, lastItem)
*       Sets default highlighting range
*
*****************************************************************************/

extern void SetDocHighlight (DoC d, Int2 firstItem, Int2 lastItem)

{
  DocData  ddata;

  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    ddata.firstHighlightItem = MIN (firstItem, lastItem);
    ddata.lastHighlightItem = MAX (firstItem, lastItem);
    SetPanelExtra ((PaneL) d, &ddata);
  }
}

/*****************************************************************************
*
*   GetDocHighlight (d, firstItem, lastItem)
*       Returns default highlighting range
*
*****************************************************************************/

extern void GetDocHighlight (DoC d, Int2Ptr firstItem, Int2Ptr lastItem)

{
  DocData  ddata;

  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    if (firstItem != NULL) {
      *firstItem = ddata.firstHighlightItem;
    }
    if (lastItem != NULL) {
      *lastItem = ddata.lastHighlightItem;
    }
  }
}

/*****************************************************************************
*
*   SetDocNotify (d, notify)
*       Sets default click notification
*
*****************************************************************************/

static void DefaultNotifyClickProc (DoC d, PoinT pt)

{
  Int2     col;
  DocData  ddata;
  Int2     item;
  Int2     row;

  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    if (ddata.notify != NULL) {
      MapDocPoint (d, pt, &item, &row, &col, NULL);
      ddata.notify (d, item, row, col, dblClick);
    }
  }
}

extern void SetDocNotify (DoC d, DocNotfyProc notify)

{
  DocData  ddata;

  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    ddata.notify = notify;
    SetPanelExtra ((PaneL) d, &ddata);
    Nlm_SetPanelClick ((PaneL) d, (PnlClckProc) DefaultNotifyClickProc, NULL, NULL, NULL);
  }
}

/*****************************************************************************
*
*   SetupItem (d, ddatptr, itemPtr, proc, data, docOwnsData,
*              lines, parFmt, colFmt, font)
*       Processes user parameters into an item to append or insert into
*       the item list, or replace an item currently in the item list
*
*****************************************************************************/

static void SetupItem (DoC d, DocDataPtr ddatptr, ItemPtr itemPtr, DocPrntProc proc,
                       Pointer data, Boolean docOwnsData, Int2 lines,
                       ParPtr parFmt, ColPtr colFmt, FonT font)

{
  Int2  i;
  RecT  r;

  if (d != NULL && ddatptr != NULL && itemPtr != NULL && proc != NULL) {
    ObjectRect (d, &r);
    InsetRect (&r, 4, 4);
    itemPtr->prtProc = proc;
    itemPtr->dataPtr = data;
    itemPtr->text = NULL;
    if (font == NULL) {
      font = ddatptr->defaultFont;
    }
    itemPtr->font = font;
    if (colFmt == NULL) {
      colFmt = ddatptr->defaultColFmt;
    }
    if (colFmt == NULL) {
      defaultTable.pixWidth = r.right - r.left;
      defaultTable.charWidth = 80;
      colFmt = &defaultTable;
    }
    i = 0;
    while (! colFmt [i].last) {
      i++;
    }
    i++;
    itemPtr->numCols = i;
    itemPtr->numRows = MAX (lines, 1);
    itemPtr->colFmt = CacheColFmt (d, colFmt);
    itemPtr->extra = NULL;
    itemPtr->startsAt = 0;
    if (parFmt == NULL) {
      parFmt = ddatptr->defaultParFmt;
    }
    if (parFmt != NULL) {
      itemPtr->openSpace = parFmt->openSpace;
      itemPtr->keepWithNext = parFmt->keepWithNext;
      itemPtr->keepTogether = parFmt->keepTogether;
      itemPtr->newPage = parFmt->newPage;
      itemPtr->tabStops = parFmt->tabStops;
      itemPtr->minLines = MAX (parFmt->minLines, (Int2) 1);
      itemPtr->minHeight = parFmt->minHeight;
    } else {
      itemPtr->openSpace = TRUE;
      itemPtr->keepWithNext = FALSE;
      itemPtr->keepTogether = FALSE;
      itemPtr->newPage = FALSE;
      itemPtr->tabStops = FALSE;
      itemPtr->minLines = 0;
      itemPtr->minHeight = 0;
    }
    itemPtr->docOwnsData = docOwnsData;
    itemPtr->notCached = TRUE;
    itemPtr->neverCached = TRUE;
  }
}

/*****************************************************************************
*
*   AppendItem (d, proc, data, docOwnsData, lines,
*               parFmt, colFmt, font)
*       Processes user parameters and appends the item to the end of the
*       item list
*
*****************************************************************************/

extern void AppendItem (DoC d, DocPrntProc proc, Pointer data,
                        Boolean docOwnsData, Int2 lines,
                        ParPtr parFmt, ColPtr colFmt, FonT font)

{
  DocData   ddata;
  ItemData  itemData;
  ItemPtr   itemPtr;

  if (d != NULL && proc != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    MemFill (&itemData, 0, sizeof (ItemData));
    SetupItem (d, &ddata, &itemData, proc, data, docOwnsData,
               lines, parFmt, colFmt, font);
    if (ddata.upd != NULL) {
      ddata.upd (d, ddata.numItems, 0);
    }
    itemPtr = CreateItemPtr (d, ddata.numItems);
    GetPanelExtra ((PaneL) d, &ddata);
    if (itemPtr != NULL) {
      *itemPtr = *(&itemData);
      itemPtr->startsAt = ddata.numLines;
      UpdateItemHeights (itemPtr);
      ddata.numLines += itemPtr->numRows;
      (ddata.numItems)++;
      SetPanelExtra ((PaneL) d, &ddata);
      if (ddata.autoAdjust) {
        AdjustDocScroll (d);
      }
    }
  }
}

/*****************************************************************************
*
*   ReplaceItem (d, item, proc, data, docOwnsData,
*                lines, parFmt, colFmt, font)
*       Replaces an existing item in a document
*
*****************************************************************************/

extern void ReplaceItem (DoC d, Int2 item, DocPrntProc proc,
                         Pointer data, Boolean docOwnsData, Int2 lines,
                         ParPtr parFmt, ColPtr colFmt, FonT font)

{
  DocData  ddata;
  ItemPtr  itemPtr;
  Int4     startsAt;

  if (d != NULL && proc != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    if (item > 0 && item <= ddata.numItems) {
      item--;
      if (ddata.upd != NULL) {
        ddata.upd (d, item, item);
      }
      itemPtr = GetItemPtr (&ddata, item);
      if (itemPtr != NULL) {
        startsAt = itemPtr->startsAt;
        itemPtr->text = (CharPtr) MemFree (itemPtr->text);
        if (itemPtr->docOwnsData) {
          itemPtr->dataPtr = (Pointer) MemFree (itemPtr->dataPtr);
        } else {
          itemPtr->dataPtr = NULL;
        }
        itemPtr->extra = MemFree (itemPtr->extra);
        MemFill (itemPtr, 0, sizeof (ItemData));
        SetupItem (d, &ddata, itemPtr, proc, data, docOwnsData,
                   lines, parFmt, colFmt, font);
        itemPtr->startsAt = startsAt;
        UpdateItemHeights (itemPtr);
        ddata.numLines = UpdateLineStarts (&ddata, item);
        SetPanelExtra ((PaneL) d, &ddata);
        if (ddata.autoAdjust) {
          AdjustDocScroll (d);
        }
      }
    }
  }
}

/*****************************************************************************
*
*   InsertItem (d, item, proc, data, docOwnsData,
*               lines, parFmt, colFmt, font)
*       Inserts a new item into a document
*
*****************************************************************************/

extern void InsertItem (DoC d, Int2 item, DocPrntProc proc,
                        Pointer data, Boolean docOwnsData, Int2 lines,
                        ParPtr parFmt, ColPtr colFmt, FonT font)

{
  DocData  ddata;
  ItemPtr  fromItem;
  ItemPtr  itemPtr;
  Int2     j;
  Int4     startsAt;
  ItemPtr  toItem;

  if (d != NULL && proc != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    if (item > 0 && item <= ddata.numItems) {
      item--;
      if (ddata.upd != NULL) {
        ddata.upd (d, item, 0);
      }
      itemPtr = CreateItemPtr (d, ddata.numItems);
      if (itemPtr != NULL) {
        GetPanelExtra ((PaneL) d, &ddata);
        (ddata.numItems)++;
        SetPanelExtra ((PaneL) d, &ddata);
        for (j = ddata.numItems - 1; j > item; j--) {
          fromItem = GetItemPtr (&ddata, j - 1);
          toItem = GetItemPtr (&ddata, j);
          if (fromItem != NULL && toItem != NULL) {
            *toItem = *fromItem;
          }
        }
        itemPtr = GetItemPtr (&ddata, item);
        if (itemPtr != NULL) {
          startsAt = itemPtr->startsAt;
          MemFill (itemPtr, 0, sizeof (ItemData));
          SetupItem (d, &ddata, itemPtr, proc, data, docOwnsData,
                     lines, parFmt, colFmt, font);
          itemPtr->startsAt = startsAt;
          UpdateItemHeights (itemPtr);
          ddata.numLines = UpdateLineStarts (&ddata, item);
          SetPanelExtra ((PaneL) d, &ddata);
          if (ddata.autoAdjust) {
            AdjustDocScroll (d);
          }
        }
      }
    }
  }
}

/*****************************************************************************
*
*   CharPrtProc (ptr)
*       Standard print function used with AppendText
*
*****************************************************************************/

static CharPtr CharPrtProc (DoC d, Int2 item, Pointer ptr)

{
  if (ptr != NULL) {
    return StringSave( (CharPtr)ptr );
  } else {
    return NULL;
  }
}

/*****************************************************************************
*
*   SkipPastNewLine (text, cnt)
*       Finds the end of a line of text, up to a maximum number of characters
*
*****************************************************************************/

static Int2 SkipPastNewLine (CharPtr text, Int2 cnt)

{
  Char  ch;

  ch = *(text + cnt);
  while (ch != '\0' && ch != '\n' && cnt < 24380) {
    cnt++;
    ch = *(text + cnt);
  }
  while ((ch == '\n' || ch == '\r') && cnt < 24380) {
    cnt++;
    ch = *(text + cnt);
  }
  return cnt;
}

/*****************************************************************************
*
*   AppendText (d, text, parFmt, colFmt, font)
*       Special case of AppendItem that takes formatted text as its data
*
*****************************************************************************/

extern void AppendText (DoC d, CharPtr text, ParPtr parFmt,
                        ColPtr colFmt, FonT font)

{
  Int2     cnt;
  Int2     cntr;
  Int2     start;
  Pointer  txt;

  if (d != NULL) {
    if (text != NULL && *text != '\0') {
      start = 0;
      cntr = StringLen (text);
      cnt = MIN (cntr, 24000);
      cnt = SkipPastNewLine (text + start, cnt);
      while (cnt > 0) {
        txt = MemNew (cnt + 1);
        MemCopy (txt, text + start, cnt);
        ProgMon ("AppendText");
        AppendItem (d, CharPrtProc, txt, TRUE, (cnt / 50) + 1,
                    parFmt, colFmt, font);
        start += cnt;
        cntr -= cnt;
        cnt = MIN (cntr, 24000);
        cnt = SkipPastNewLine (text + start, cnt);
      }
    } else {
      ProgMon ("AppendText");
      AppendItem (d, CharPrtProc, "", FALSE, 1,
                  parFmt, colFmt, font);
    }
  }
}

/*****************************************************************************
*
*   ReplaceText (d, item, text, parFmt, colFmt, font)
*       Special case of ReplaceItem that takes formatted text as its data
*
*****************************************************************************/

extern void ReplaceText (DoC d, Int2 item, CharPtr text,
                         ParPtr parFmt, ColPtr colFmt, FonT font)

{
  Int2     cnt;
  Int2     cntr;
  Int2     start;
  Pointer  txt;

  if (d != NULL) {
    if (text != NULL && *text != '\0') {
      start = 0;
      cntr = StringLen (text);
      cnt = MIN (cntr, 24000);
      cnt = SkipPastNewLine (text + start, cnt);
      while (cnt > 0) {
        txt = MemNew (cnt + 1);
        MemCopy (txt, text + start, cnt);
        ReplaceItem (d, item, CharPrtProc, txt, TRUE, (cnt / 50) + 1,
                     parFmt, colFmt, font);
        start += cnt;
        cntr -= cnt;
        cnt = MIN (cntr, 24000);
        cnt = SkipPastNewLine (text + start, cnt);
        item++;
      }
    } else {
      ReplaceItem (d, item, CharPrtProc, "", FALSE, 1,
                   parFmt, colFmt, font);
    }
  }
}

/*****************************************************************************
*
*   InsertText (d, item, text, parFmt, colFmt, font)
*       Special case of InsertItem that takes formatted text as its data
*
*****************************************************************************/

extern void InsertText (DoC d, Int2 item, CharPtr text,
                        ParPtr parFmt, ColPtr colFmt, FonT font)

{
  Int2     cnt;
  Int2     cntr;
  Int2     start;
  Pointer  txt;

  if (d != NULL) {
    if (text != NULL && *text != '\0') {
      start = 0;
      cntr = StringLen (text);
      cnt = MIN (cntr, 24000);
      cnt = SkipPastNewLine (text + start, cnt);
      while (cnt > 0) {
        txt = MemNew (cnt + 1);
        MemCopy (txt, text + start, cnt);
        InsertItem (d, item, CharPrtProc, txt, TRUE, (cnt / 50) + 1,
                    parFmt, colFmt, font);
        start += cnt;
        cntr -= cnt;
        cnt = MIN (cntr, 24000);
        cnt = SkipPastNewLine (text + start, cnt);
        item++;
      }
    } else {
      InsertItem (d, item, CharPrtProc, "", FALSE, 1,
                  parFmt, colFmt, font);
    }
  }
}

/*****************************************************************************
*
*   DeleteItem (d, item)
*       Deletes an item from a document
*
*****************************************************************************/

extern void DeleteItem (DoC d, Int2 item)

{
  DocData  ddata;
  ItemPtr  fromItem;
  ItemPtr  itemPtr;
  Int2     j;
  Int4     startsAt;
  ItemPtr  toItem;

  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    if (item > 0 && item <= ddata.numItems) {
      item--;
      itemPtr = GetItemPtr (&ddata, item);
      if (itemPtr != NULL) {
        startsAt = itemPtr->startsAt;
        itemPtr->text = (CharPtr) MemFree (itemPtr->text);
        if (itemPtr->docOwnsData) {
          itemPtr->dataPtr = (Pointer) MemFree (itemPtr->dataPtr);
        } else {
          itemPtr->dataPtr = NULL;
        }
        itemPtr->extra = MemFree (itemPtr->extra);
        for (j = item; j < ddata.numItems - 1; j++) {
          fromItem = GetItemPtr (&ddata, j + 1);
          toItem = GetItemPtr (&ddata, j);
          if (fromItem != NULL && toItem != NULL) {
            *toItem = *fromItem;
          }
        }
        itemPtr->startsAt = startsAt;
        (ddata.numItems)--;
        ddata.numLines = UpdateLineStarts (&ddata, item);
        SetPanelExtra ((PaneL) d, &ddata);
        if (ddata.autoAdjust) {
          AdjustDocScroll (d);
        }
      }
    }
  }
}

/*****************************************************************************
*
*   BulkAppendItem (d, numItems, proc, estLines, parFmt, colFmt, font)
*       Populates a document in one step, assuming that the print function
*       doesn't need individual data
*
*****************************************************************************/

extern void BulkAppendItem (DoC d, Int2 numItems, DocPrntProc proc,
                            Int2 estLines, ParPtr parFmt,
                            ColPtr colFmt, FonT font)

{
  ColXPtr    colXPtr;
  DocData    ddata;
  Int2       i;
  Int2       index;
  Int2       item;
  ItemPtr    itemPtr;
  Int2       leadHeight = 0;
  Int2       lineHeight = stdLineHeight;
  Int2       list;
  ListDPtr   listPtr;
  MasterPtr  masterPtr;
  Int2       numCols;
  Int2       numLists;
  ParData    parData;
  RecT       r;

  if (d != NULL && numItems <= 32767) {
    GetPanelExtra ((PaneL) d, &ddata);
    if (ddata.master == NULL) {
      ddata.master = (MasterPtr) MemNew (sizeof (MasterData));
      SetPanelExtra ((PaneL) d, &ddata);
    }
    numLists = (numItems / LISTSIZE) + 1;
    ObjectRect (d, &r);
    InsetRect (&r, 4, 4);
    if (numLists <= MAXLISTS && ddata.master != NULL) {
      masterPtr = ddata.master;
      for (list = 0; list < numLists; list++) {
        listPtr = masterPtr->list [list];
        if (listPtr == NULL) {
          masterPtr->list [list] = (ListDPtr) MemNew(sizeof(ListData));
          listPtr = masterPtr->list [list];
        }
      }
      if (colFmt == NULL) {
        colFmt = ddata.defaultColFmt;
      }
      if (colFmt == NULL) {
        defaultTable.pixWidth = r.right - r.left;
        defaultTable.charWidth = 80;
        colFmt = &defaultTable;
      }
      i = 0;
      while (! colFmt [i].last) {
        i++;
      }
      i++;
      numCols = i;
      colXPtr = CacheColFmt (d, colFmt);
      if (parFmt == NULL) {
        parFmt = ddata.defaultParFmt;
      }
      if (parFmt != NULL) {
        parData.openSpace = parFmt->openSpace;
        parData.keepWithNext = parFmt->keepWithNext;
        parData.keepTogether = parFmt->keepTogether;
        parData.newPage = parFmt->newPage;
        parData.tabStops = parFmt->tabStops;
        parData.minLines = MAX (parFmt->minLines, (Int2) 1);
        parData.minHeight = parFmt->minHeight;
      } else {
        parData.openSpace = TRUE;
        parData.keepWithNext = FALSE;
        parData.keepTogether = FALSE;
        parData.newPage = FALSE;
        parData.tabStops = FALSE;
        parData.minLines = 0;
        parData.minHeight = 0;
      }
      if (font == NULL) {
        font = ddata.defaultFont;
      }
      for (item = 0; item < numItems; item++) {
        list = (item / LISTSIZE);
        listPtr = masterPtr->list [list];
        if (listPtr != NULL) {
          index = (item % LISTSIZE);
          itemPtr = &(listPtr->items [index]);
          if (itemPtr != NULL) {
            itemPtr->prtProc = proc;
            itemPtr->dataPtr = NULL;
            itemPtr->text = NULL;
            itemPtr->cached = NULL;
            itemPtr->font = font;
            itemPtr->numRows = MAX (estLines, 1);
            itemPtr->numCols = numCols;
            itemPtr->colFmt = colXPtr;
            itemPtr->extra = NULL;
            itemPtr->startsAt = 0;
            itemPtr->openSpace = parData.openSpace;
            itemPtr->keepWithNext = parData.keepWithNext;
            itemPtr->keepTogether = parData.keepTogether;
            itemPtr->newPage = parData.newPage;
            itemPtr->tabStops = parData.tabStops;
            itemPtr->minLines = parData.minLines;
            itemPtr->minHeight = parData.minHeight;
            itemPtr->docOwnsData = FALSE;
            itemPtr->notCached = TRUE;
            itemPtr->neverCached = TRUE;
            if (item == 0) {
              UpdateItemHeights (itemPtr);
              lineHeight = itemPtr->lineHeight;
              leadHeight = itemPtr->leadHeight;
            }
            itemPtr->lineHeight = lineHeight;
            itemPtr->leadHeight = leadHeight;
          }
        }
      }
      ddata.numItems = numItems;
      ddata.numLines = UpdateLineStarts (&ddata, 0);
      SetPanelExtra ((PaneL) d, &ddata);
      if (ddata.upd != NULL) {
        ddata.upd ((DoC) d, 0, 0);
      }
      if (ddata.autoAdjust) {
        AdjustDocScroll (d);
      }
    }
  }
}

/*****************************************************************************
*
*   StdCache functions
*       Simple binary tree cache
*
*****************************************************************************/

extern void StdPutDocCache (DoC d, Int2 item, CharPtr text)

{
  DocData  ddata;
  ItemPtr  itemPtr;

  if (d != NULL && item > 0) {
    GetPanelExtra ((PaneL) d, &ddata);
    itemPtr = GetItemPtr (&ddata, item - 1);
    if (itemPtr != NULL) {
      itemPtr->cached = MemFree (itemPtr->cached);
      itemPtr->cached = StringSave (text);
    }
  }
}

extern CharPtr StdGetDocCache (DoC d, Int2 item)

{
  DocData  ddata;
  ItemPtr  itemPtr;
  CharPtr  text = NULL;

  if (d != NULL && item > 0) {
    GetPanelExtra ((PaneL) d, &ddata);
    itemPtr = GetItemPtr (&ddata, item - 1);
    if (itemPtr != NULL) {
      text = StringSave (itemPtr->cached);
    }
  }
  return text;
}

extern void StdResetDocCache (DoC d, Int2 from, Int2 to)

{
  DocData  ddata;
  Int2     i;
  ItemPtr  itemPtr;

  GetPanelExtra ((PaneL) d, &ddata);
  for (i = 0; i < ddata.numItems; i++) {
    itemPtr = GetItemPtr (&ddata, i);
    if (itemPtr != NULL) {
      itemPtr->cached = MemFree (itemPtr->cached);
    }
  }
  SetPanelExtra ((PaneL) d, &ddata);
}

/*****************************************************************************
*
*   ResetDocument (d)
*       Performs garbage collection on document data
*
*****************************************************************************/

static void ResetDocument (PaneL d)

{
  DocData    ddata;
  Int2       i;
  ItemPtr    itemPtr;
  MasterPtr  masterPtr;
  RecT       r;

  GetPanelExtra (d, &ddata);
  if (ddata.data != NULL && ddata.cleanup != NULL) {
    ddata.cleanup ((DoC) d, ddata.data);
  }
  if (ddata.upd != NULL) {
    ddata.upd ((DoC) d, 0, 0);
  }
  ObjectRect (d, &r);
  InsetRect (&r, 4, 4);
  for (i = 0; i < ddata.numItems; i++) {
    itemPtr = GetItemPtr (&ddata, i);
    if (itemPtr != NULL) {
      itemPtr->text = (CharPtr) MemFree (itemPtr->text);
      if (itemPtr->docOwnsData) {
        itemPtr->dataPtr = (Pointer) MemFree (itemPtr->dataPtr);
      } else {
        itemPtr->dataPtr = NULL;
      }
      itemPtr->extra = MemFree (itemPtr->extra);
      itemPtr->cached = MemFree (itemPtr->cached);
    }
  }
  if (ddata.master != NULL) {
    masterPtr = ddata.master;
    for (i = 0; i < MAXLISTS; i++) {
      masterPtr->list [i] = (ListDPtr) MemFree(masterPtr->list [i]);
    }
  }
  ddata.colFmts = ValNodeFreeData (ddata.colFmts);
  ddata.numItems = 0;
  ddata.numLines = 0;
  ddata.barmax = 0;
  ddata.pgUp = 0;
  ddata.pgDn = 0;
  ddata.master = (MasterPtr) MemFree (ddata.master);
  for (i = 0; i < MAXFONTS; i++) {
    fontHeights [i].font = NULL;
    fontHeights [i].height = 0;
  }
  ddata.data = NULL;
  ddata.cleanup = NULL;
  ddata.firstHighlightItem = 0;
  ddata.lastHighlightItem = 0;
  SetPanelExtra ((PaneL) d, &ddata);
}

/*****************************************************************************
*
*   NewDocument (d)
*       Initialized document panel extra data
*
*****************************************************************************/

static void NewDocument (DoC d)

{
  DocData  ddata;
  Int2     i;

  ddata.numItems = 0;
  ddata.numLines = 0;
  ddata.barmax = 0;
  ddata.pgUp = 0;
  ddata.pgDn = 0;
  ddata.tabCount = 4;
  ddata.autoAdjust = TRUE;
  ddata.doc = d;
  ddata.master = NULL;
  ddata.draw = NULL;
  ddata.drawcell = NULL;
  ddata.pan = NULL;
  ddata.gray = NULL;
  ddata.invert = NULL;
  ddata.color = NULL;
  ddata.put = NULL;
  ddata.get = NULL;
  ddata.upd = NULL;
  ddata.data = NULL;
  ddata.cleanup = NULL;
  ddata.colFmts = NULL;
  for (i = 0; i < MAXFONTS; i++) {
    fontHeights [i].font = NULL;
    fontHeights [i].height = 0;
  }
  ddata.defaultParFmt = NULL;
  ddata.defaultColFmt = NULL;
  ddata.defaultFont = NULL;
  ddata.firstHighlightItem = 0;
  ddata.lastHighlightItem = 0;
  ddata.useRowsNotItems = FALSE;
  ddata.notify = NULL;
  SetPanelExtra ((PaneL) d, &ddata);
}

/*****************************************************************************
*
*   DocumentPanel (prnt, pixwidth, pixheight)
*       Creates an empty document object
*
*****************************************************************************/

extern DoC DocumentPanel (GrouP prnt, Int2 pixwidth, Int2 pixheight)

{
  DoC  d;
  WindoW    tempPort;

  d = NULL;
  if (prnt != NULL) {
    tempPort = SavePort (prnt);
    d = (DoC) AutonomousPanel4 (prnt, pixwidth, pixheight, DrawDocument,
                                DocumentScrlProc, NULL, sizeof (DocData),
                                ResetDocument, NULL);
    if (d != NULL) {
      NewDocument (d);
    }
    RestorePort (tempPort);
  }
  return d;
}

/*****************************************************************************
*
*   SetDocProcs (d, click, drag, release, pan)
*       Sets mouse callbacks for a document object
*
*****************************************************************************/

extern void SetDocProcs (DoC d, DocClckProc click, DocClckProc drag,
                         DocClckProc release, DocPanProc pan)

{
  DocData  ddata;

  if (d != NULL) {
    Nlm_SetPanelClick ((PaneL) d, (PnlClckProc) click, (PnlClckProc) drag,
                       NULL, (PnlClckProc) release);
    GetPanelExtra ((PaneL) d, &ddata);
    ddata.pan = pan;
    SetPanelExtra ((PaneL) d, &ddata);
  }
}

/*****************************************************************************
*
*   SetDocShade (d, draw, gray, invert, color)
*       Sets draw callbacks for a document object
*
*****************************************************************************/

extern void SetDocShade (DoC d, DocDrawProc draw, DocShadeProc gray,
                         DocShadeProc invert, DocShadeProc color)

{
  DocData  ddata;

  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    ddata.draw = draw;
    ddata.gray = gray;
    ddata.invert = invert;
    ddata.color = color;
    SetPanelExtra ((PaneL) d, &ddata);
  }
}

/*****************************************************************************
*
*   SetDocExtra (d, drawcell)
*       Sets drawcell callback for a document object
*
*****************************************************************************/

extern void SetDocExtra (DoC d, DocCellProc drawcell)

{
  DocData  ddata;

  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    ddata.drawcell = drawcell;
    SetPanelExtra ((PaneL) d, &ddata);
  }
}

/*****************************************************************************
*
*   SetDocCache (d, put, get, upd)
*       Sets cache callbacks for a document object
*
*****************************************************************************/

extern void SetDocCache (DoC d, DocPutProc put, DocGetProc get, DocUpdProc upd)

{
  DocData  ddata;

  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    ddata.put = put;
    ddata.get = get;
    ddata.upd = upd;
    SetPanelExtra ((PaneL) d, &ddata);
  }
}

/*****************************************************************************
*
*   SetDocData (d, data, cleanup)
*       Attaches instance data to a document object
*
*****************************************************************************/

void SetDocData (DoC d, VoidPtr data, DocFreeProc cleanup)

{
  DocData  ddata;

  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    ddata.data = data;
    ddata.cleanup = cleanup;
    SetPanelExtra ((PaneL) d, &ddata);
  }
}

/*****************************************************************************
*
*   GetDocData (d)
*       Returns the instance data attached to a document object
*
*****************************************************************************/

VoidPtr GetDocData (DoC d)

{
  DocData  ddata;

  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    return (ddata.data);
  } else {
    return NULL;
  }
}

/*****************************************************************************
*
*   GetDocParams (d, numItems, numLines)
*       Returns document-specific information
*
*****************************************************************************/

void GetDocParams4 (DoC d, Int2Ptr numItems, Int4Ptr numLines)

{
  DocData  ddata;
  Int2     items;
  Int4     lines;

  items = 0;
  lines = 0;
  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    items = ddata.numItems;
    lines = ddata.numLines;
  }
  if (numItems != NULL) {
    *numItems = items;
  }
  if (numLines != NULL) {
    *numLines = lines;
  }
}

void GetDocParams (DoC d, Int2Ptr numItems, Int2Ptr numLines)

{
  Int4  lines;

  GetDocParams4 (d, numItems, &lines);
  if (numLines != NULL) {
    *numLines = (Int2) lines;
  }
}

/*****************************************************************************
*
*   GetItemParams (d, item, startsAt, numRows, numCols, lineHeight, data)
*       Returns item-specific information
*
*****************************************************************************/

void GetItemParams4 (DoC d, Int2 item, Int4Ptr startsAt, Int2Ptr numRows,
                     Int2Ptr numCols, Int2Ptr lineHeight, Pointer PNTR data)

{
  Int2     cols;
  Int4     first;
  VoidPtr  dataPtr;
  DocData  ddata;
  Int2     height;
  ItemPtr  itemPtr;
  Int2     rows;

  cols = 0;
  first = 0;
  height = 0;
  rows = 0;
  dataPtr = NULL;
  if (d != NULL && item > 0) {
    GetPanelExtra ((PaneL) d, &ddata);
    itemPtr = GetItemPtr (&ddata, item - 1);
    if (itemPtr != NULL) {
      first = itemPtr->startsAt;
      rows = itemPtr->numRows;
      cols = itemPtr->numCols;
      height = itemPtr->lineHeight;
      dataPtr = itemPtr->dataPtr;
    }
  }
  if (startsAt != NULL) {
    *startsAt = first;
  }
  if (numRows != NULL) {
    *numRows = rows;
  }
  if (numCols != NULL) {
    *numCols = cols;
  }
  if (lineHeight != NULL) {
    *lineHeight = height;
  }
  if (data != NULL) {
    *data = dataPtr;
  }
}

void GetItemParams (DoC d, Int2 item, Int2Ptr startsAt, Int2Ptr numRows,
                    Int2Ptr numCols, Int2Ptr lineHeight, Pointer PNTR data)

{
  Int4  first;

  GetItemParams4 (d, item, &first, numRows, numCols, lineHeight, data);
  if (startsAt != NULL) {
    *startsAt = (Int2) first;
  }
}

/*****************************************************************************
*
*   GetColParams (d, item, col, pixPos, pixWidth, pixInset, just)
*       Returns column-specific information
*
*****************************************************************************/

void GetColParams (DoC d, Int2 item, Int2 col, Int2Ptr pixPos,
                   Int2Ptr pixWidth, Int2Ptr pixInset, CharPtr just)

{
  ColXPtr  colFmt;
  DocData  ddata;
  Int2     inset;
  ItemPtr  itemPtr;
  Char     jst;
  Int2     position;
  RecT     r;
  Int2     width;

  inset = 0;
  position = 0;
  width = 0;
  jst = 'l';
  if (d != NULL && item > 0 && col > 0) {
    GetPanelExtra ((PaneL) d, &ddata);
    ObjectRect (d, &r);
    InsetRect (&r, 4, 4);
    itemPtr = GetItemPtr (&ddata, item - 1);
    if (itemPtr != NULL && col <itemPtr->numCols + 1) {
      colFmt = itemPtr->colFmt;
      if (colFmt != NULL) {
        SetTablePixFormat (colFmt, r.left, itemPtr->numCols);
        position = colFmt [col - 1].position;
        width = colFmt [col - 1].pixWidth;
        inset = colFmt [col - 1].pixInset;
        jst = justToChar [colFmt [col - 1].just];
      }
    }
  }
  if (pixPos != NULL) {
    *pixPos = position;
  }
  if (pixWidth != NULL) {
    *pixWidth = width;
  }
  if (pixInset != NULL) {
    *pixInset = inset;
  }
  if (just != NULL) {
    *just = jst;
  }
}

/*****************************************************************************
*
*   MapDocPoint (d, pt, item, row, col, rct)
*       Converts a mouse point to a item, row, and column positions
*       within a document
*
*****************************************************************************/

extern void MapDocPoint (DoC d, PoinT pt, Int2Ptr item, Int2Ptr row, Int2Ptr col, RectPtr rct)

{
  Int2     cl;
  ColXPtr  colFmt;
  DocData  ddata;
  Int2     firstItem;
  Int2     firstLine;
  Boolean  goOn;
  Int2     i;
  Int2     itemNum;
  ItemPtr  itemPtr;
  Int2     itm;
  Int2     leadHeight;
  Int2     lineHeight;
  Int2     numCols;
  Int2     numRows;
  Int4     off;
  RecT     q;
  RecT     r;
  RecT     rc;
  Int2     rw;
  BaR      sb;

  itm = 0;
  rw = 0;
  cl = 0;
  LoadRect (&rc, 0, 0, 0, 0);
  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    ObjectRect (d, &r);
    InsetRect (&r, 4, 4);
    if (PtInRect (pt, &r)) {
      sb = GetSlateVScrollBar ((SlatE) d);
      if (sb != NULL) {
        goOn = TRUE;
        LoadRect (&q, r.left, r.top, r.right, r.top);
        off = GetBarValue (sb);
        itemNum = GetItemNum (&ddata, off);
        itemPtr = GetItemPtr (&ddata, itemNum);
        if (itemPtr != NULL) {
          firstLine = (Int2) (off - itemPtr->startsAt);
          firstItem = itemNum;
          while (goOn && itemNum < ddata.numItems) {
            itemPtr = GetItemPtr (&ddata, itemNum);
            if (itemPtr != NULL) {
              CacheIfNever (&ddata, itemNum + 1, itemPtr, &r, ddata.tabCount);
              SetTablePixFormat (itemPtr->colFmt, r.left, itemPtr->numCols);
              if (itemNum > firstItem) {
                leadHeight = itemPtr->leadHeight;
              } else {
                leadHeight = 0;
              }
              lineHeight = itemPtr->lineHeight;
              numRows = MIN (itemPtr->numRows - firstLine,
                             MAX ((r.bottom - q.top - leadHeight) / lineHeight, 0));
              numCols = itemPtr->numCols;
              colFmt = itemPtr->colFmt;
              q.bottom = q.top + leadHeight + lineHeight * numRows;
              if (PtInRect (pt, &q) && numRows > 0) {
                goOn = FALSE;
                itm = itemNum + 1;
                if (pt.y <= q.top + leadHeight) {
                  rw = 0;
                } else {
                  rw = firstLine + ((pt.y - q.top - leadHeight) / lineHeight) + 1;
                }
                if (numCols > 0 && colFmt != NULL) {
                  for (i = 0; i < numCols; i++) {
                    if (pt.x >= colFmt [i].position &&
                        pt.x < colFmt [i].position + colFmt [i].pixWidth) {
                      cl = i + 1;
                      if (rw > 0) {
                        rc.left = colFmt [i].position;
                        rc.top = q.top + leadHeight + lineHeight * (rw - firstLine - 1);
                        rc.right = rc.left + colFmt [i].pixWidth;
                        rc.bottom = rc.top + lineHeight;
                      }
                    }
                  }
                }
              } else {
                q.top = q.bottom;
              }
              if (numRows < itemPtr->numRows - firstLine) {
                goOn = FALSE;
              }
            }
            itemNum++;
            firstLine = 0;
          }
        }
      }
    }
  }
  if (item != NULL) {
    *item = itm;
  }
  if (row != NULL) {
    *row = rw;
  }
  if (col != NULL) {
    *col = cl;
  }
  if (rct != NULL) {
    *rct = rc;
  }
}

/*****************************************************************************
*
*   GetDocText (d, item, row, col)
*       Returns a string with text from a document.  If item, row, or col
*       parameters are 0, text is taken from any item, row, or column,
*       respectively.  The string must be freed by the application
*
*****************************************************************************/

extern CharPtr GetDocText (DoC d, Int2 item, Int2 row, Int2 col)

{
  ByteStorePtr  bsp;
  Char          ch;
  Int2          cl;
  DocData       ddata;
  Int2          i;
  ItemData      itemData;
  ItemPtr       itemPtr;
  Int2          k;
  Int2          num;
  CharPtr       ptr;
  RecT          r;
  Int2          rw;
  Int2          start;
  Int2          stop;
  CharPtr       text;

  text = NULL;
  if (d != NULL && item >= 0 && row >= 0 && col >= 0) {
    ObjectRect (d, &r);
    InsetRect (&r, 4, 4);
    GetPanelExtra ((PaneL) d, &ddata);
    if (ddata.numItems > 0) {
      bsp = BSNew (0);
      if (bsp != NULL) {
        if (item == 0) {
          start = 0;
          stop = ddata.numItems;
        } else if (item <= ddata.numItems) {
          start = item - 1;
          stop = item;
        } else {
          start = 0;
          stop = 0;
        }
        for (i = start; i < stop; i++) {
          itemPtr = GetItemPtr (&ddata, i);
          if (itemPtr != NULL) {
            itemData.text = NULL;
            itemData.prtProc = itemPtr->prtProc;
            itemData.dataPtr = itemPtr->dataPtr;
            itemData.font = itemPtr->font;
            itemData.extra = itemPtr->extra;
            itemData.openSpace = itemPtr->openSpace;
            itemData.tabStops = itemPtr->tabStops;
            itemData.numRows = itemPtr->numRows;
            itemData.numCols = itemPtr->numCols;
            itemData.minLines = itemPtr->minLines;
            itemData.minHeight = itemPtr->minHeight;
            itemData.colFmt = itemPtr->colFmt;
            SetTablePixFormat (itemData.colFmt, r.left, itemData.numCols);
            if (itemData.prtProc != NULL) {
              FormatText (&ddata, i + 1, &itemData, TRUE, ddata.tabCount);
              itemPtr->numRows = itemData.numRows;
              itemPtr->neverCached = itemData.neverCached;
              SetTablePixFormat (itemData.colFmt, r.left, itemData.numCols);
              text = itemData.text;
              if (text != NULL && *text != '\0' && itemData.colFmt != NULL) {
                if (i > start && itemData.openSpace > 0 && item == 0) {
                  BSPutByte (bsp, (Int2) '\n');
                }
                rw = 0;
                cl = 0;
                ptr = text;
                ch = *ptr;
                while (ch != '\0') {
                  if (ch == '\n') {
                    if ((rw + 1 == row && col == 0) || row == 0) {
                      BSPutByte (bsp, (Int2) '\n');
                    }
                    cl = 0;
                    rw++;
                    ptr++;
                  } else if (ch == '\t') {
                    if ((rw + 1 == row || row == 0) && col == 0) {
                      BSPutByte (bsp, (Int2) '\t');
                    }
                    cl++;
                    ptr++;
                  } else {
                    num = 0;
                    while (ch != '\0' && ch != '\t' && ch != '\n') {
                      num++;
                      ch = ptr [num];
                    }
                    if (num > 0 && (cl +1 == col || col == 0) &&
                        (rw + 1 == row || row == 0)) {
                      for (k = 0; k < num; k++) {
                        BSPutByte (bsp, (Int2) ptr [k]);
                      }
                    }
                    ptr += num;
                  }
                  ch = *ptr;
                }
              }
            }
            MemFree (itemData.text);
          }
        }
        text = (CharPtr) BSMerge(bsp, NULL);
        BSFree (bsp);
        ddata.numLines = UpdateLineStarts (&ddata, start);
        SetPanelExtra ((PaneL) d, &ddata);
      }
    }
  }
  return text;
}

/*****************************************************************************
*
*   ItemIsVisible (d, item, top, bottom, firstLine)
*       Finds the visible region of an item in a document
*
*****************************************************************************/

extern Boolean ItemIsVisible (DoC d, Int2 item, Int2Ptr top,
                              Int2Ptr bottom, Int2Ptr firstLine)

{
  DocData  ddata;
  Int2     firstItem;
  Int2     fstLine;
  Boolean  goOn;
  Int2     itemNum;
  ItemPtr  itemPtr;
  Int2     leadHeight;
  Int2     lineHeight;
  Int2     numRows;
  Int4     off;
  RecT     r;
  RecT     rct;
  Boolean  rsult;
  BaR      sb;

  rsult = FALSE;
  if (top != NULL) {
    *top = 0;
  }
  if (bottom != NULL) {
    *bottom = 0;
  }
  if (firstLine != NULL) {
   *firstLine = 0;
  }
  if (d != NULL) {
    ObjectRect (d, &r);
    InsetRect (&r, 4, 4);
    GetPanelExtra ((PaneL) d, &ddata);
    sb = GetSlateVScrollBar ((SlatE) d);
    if (item > 0 && item <= ddata.numItems && sb != NULL) {
      item--;
      goOn = TRUE;
      LoadRect (&rct, r.left, r.top, r.right, r.top);
      off = GetBarValue (sb);
      itemNum = GetItemNum (&ddata, off);
      itemPtr = GetItemPtr (&ddata, itemNum);
      if (itemPtr != NULL) {
        fstLine = (Int2) (off - itemPtr->startsAt);
        firstItem = itemNum;
        lineHeight = 0;
        leadHeight = 0;
        numRows = 0;
        while (goOn && itemNum < ddata.numItems && itemNum <= item) {
          itemPtr = GetItemPtr (&ddata, itemNum);
          if (itemPtr != NULL) {
            CacheIfNever (&ddata, itemNum + 1, itemPtr, &r, ddata.tabCount);
            SetTablePixFormat (itemPtr->colFmt, r.left, itemPtr->numCols);
            if (itemNum > firstItem) {
              leadHeight = itemPtr->leadHeight;
            } else {
              leadHeight = 0;
            }
            lineHeight = itemPtr->lineHeight;
            rct.top += leadHeight;
            numRows = MIN (itemPtr->numRows - fstLine,
                           MAX ((r.bottom - rct.top) / lineHeight, 0));
            rct.bottom = rct.top + lineHeight * numRows;
            if (itemNum == item) {
              goOn = FALSE;
              if (numRows > 0) {
                if (top != NULL) {
                  *top = rct.top;
                }
                if (bottom != NULL) {
                  *bottom = rct.bottom;
                }
                if (firstLine != NULL) {
                  *firstLine = fstLine;
                }
                rsult = TRUE;
              }
            } else {
              rct.top = rct.bottom;
            }
            if (numRows < itemPtr->numRows - fstLine) {
              goOn = FALSE;
            }
          }
          itemNum++;
          fstLine = 0;
        }
      }
    }
  }
  return rsult;
}

/*****************************************************************************
*
*   GetScrlParams (d, offset, firstShown, firstLine)
*       Returns scroll-specific information
*
*****************************************************************************/

extern Boolean GetScrlParams4 (DoC d, Int4Ptr offset,
                               Int2Ptr firstShown, Int2Ptr firstLine)

{
  DocData  ddata;
  Int2     item;
  Int4     line;
  Int4     off;
  Boolean  rsult;
  BaR      sb;

  rsult = FALSE;
  off = 0;
  item = 0;
  line = 0;
  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    sb = GetSlateVScrollBar ((SlatE) d);
    if (sb != NULL) {
      off = GetBarValue (sb);
      item = GetItemNum (&ddata, off);
      line = off - GetStartsAt (&ddata, item);
      rsult = TRUE;
    }
  }
  if (offset != NULL) {
    *offset = off;
  }
  if (firstShown != NULL) {
    *firstShown = item + 1;
  }
  if (firstLine != NULL) {
    *firstLine = (Int2) line;
  }
  return rsult;
}

extern Boolean GetScrlParams (DoC d, Int2Ptr offset,
                              Int2Ptr firstShown, Int2Ptr firstLine)

{
  Int4     off;
  Boolean  rsult;

  rsult = GetScrlParams4 (d, &off, firstShown, firstLine);
  if (offset != NULL) {
    *offset = (Int2) off;
  }
  return rsult;
}

/*****************************************************************************
*
*   RowIsVisible (d, item, row, top, bottom)
*       Finds the visible region of a row in an item of a document
*
*****************************************************************************/

extern Boolean RowIsVisible (DoC d, Int2 item, Int2 row,
                             Int2Ptr top, Int2Ptr bottom)

{
  Int2     bt;
  Int2     first;
  Int2     from;
  Int2     lineHeight;
  Int2     numRows;
  RecT     r;
  Boolean  rsult;
  Int2     tmp;
  Int2     to;
  Int2     tp;

  rsult = FALSE;
  if (top != NULL) {
    *top = 0;
  }
  if (bottom != NULL) {
    *bottom = 0;
  }
  if (d != NULL && item > 0) {
    if (Enabled (d) && AllParentsEnabled (d) &&
        Visible (d) && AllParentsVisible (d)) {
      if (ItemIsVisible (d, item, &tp, &bt, &first)) {
        ObjectRect (d, &r);
        InsetRect (&r, 4, 4);
        GetItemParams (d, item, NULL, &numRows, NULL, &lineHeight, NULL);
        from = MIN (row, numRows);
        to = MIN (row, numRows);
        first++;
        if (from > to) {
          tmp = from;
          from = to;
          to = tmp;
        }
        if (to >= first) {
          while (from > first) {
            tp += lineHeight;
            first++;
          }
          bt = tp;
          while (to >= from) {
            bt += lineHeight;
            from++;
          }
          if (tp >= r.top && tp <= r.bottom &&
              bt >= r.top && bt <= r.bottom) {
            if (top != NULL) {
              *top = tp;
            }
            if (bottom != NULL) {
              *bottom = bt;
            }
            rsult = TRUE;
          }
        }
      }
    }
  }
  return rsult;
}

/*****************************************************************************
*
*   InvalDocRows (d, item, from, to)
*       Invalidates rows in a document
*
*****************************************************************************/

extern void InvalDocRows (DoC d, Int2 item, Int2 from, Int2 to)

{
  Int2    bottom;
  Int2    first;
  Int2    lineHeight;
  Int2    numRows;
  RecT    r;
  WindoW  tempPort;
  Int2    tmp;
  Int2    top;

  if (d != NULL) {
    if (Enabled (d) && AllParentsEnabled (d) &&
        Visible (d) && AllParentsVisible (d)) {
      if (item > 0 && from >= 0 && to >= 0) {
        if (ItemIsVisible (d, item, &top, &bottom, &first)) {
          ObjectRect (d, &r);
          InsetRect (&r, 4, 4);
          GetItemParams (d, item, NULL, &numRows, NULL, &lineHeight, NULL);
          from = MIN (from, numRows);
          to = MIN (to, numRows);
          first++;
          if (from > to) {
            tmp = from;
            from = to;
            to = tmp;
          }
          if (to >= first) {
            while (from > first) {
              top += lineHeight;
              first++;
            }
            bottom = top;
            while (to >= from) {
              bottom += lineHeight;
              from++;
            }
            if (top >= r.top && top <= r.bottom &&
                bottom >= r.top && bottom <= r.bottom) {
              r.top = top;
              r.bottom = bottom;
              tempPort = SavePort (d);
              Select (d);
              InsetRect (&r, -1, -1);
              InvalRect (&r);
              RestorePort (tempPort);
            }
          }
        }
      } else if (item == 0) {
        tempPort = SavePort (d);
        Select (d);
        ObjectRect (d, &r);
        InsetRect (&r, 3, 3);
        InvalRect (&r);
        RestorePort (tempPort);
      }
    }
  }
}

/*****************************************************************************
*
*   InvalDocCols (d, item, from, to)
*       Invalidates columns in a document
*
*****************************************************************************/

extern void InvalDocCols (DoC d, Int2 item, Int2 from, Int2 to)

{
  Int2     bottom;
  ColXPtr  colFmt;
  DocData  ddata;
  ItemPtr  itemPtr;
  Int2     numCols;
  RecT     r;
  WindoW   tempPort;
  Int2     tmp;
  Int2     top;

  if (d != NULL) {
    if (Enabled (d) && AllParentsEnabled (d) &&
        Visible (d) && AllParentsVisible (d)) {
      if (item > 0 && from >= 0 && to >= 0) {
        if (ItemIsVisible (d, item, &top, &bottom, NULL)) {
          GetPanelExtra ((PaneL) d, &ddata);
          itemPtr = GetItemPtr (&ddata, item - 1);
          if (itemPtr == NULL) return;
          colFmt = itemPtr->colFmt;
          if (colFmt == NULL) return;
          ObjectRect (d, &r);
          InsetRect (&r, 4, 4);
          numCols = itemPtr->numCols;
          from = MIN (from, numCols);
          to = MIN (to, numCols);
          if (from > to) {
            tmp = from;
            from = to;
            to = tmp;
          }
          if (from > 0) {
            from--;
          }
          if (to > 0 && to <= numCols) {
            to--;
          }
          if (from >= 0 && from < numCols && to >= 0 && to < numCols) {
            r.left = colFmt [from].position;
            r.right = colFmt [to].position + colFmt [to].pixWidth;
            if (top >= r.top && top <= r.bottom &&
                bottom >= r.top && bottom <= r.bottom) {
              r.top = top;
              r.bottom = bottom;
              tempPort = SavePort (d);
              Select (d);
              InsetRect (&r, -1, -1);
              InvalRect (&r);
              RestorePort (tempPort);
            }
          }
        }
      } else if (item == 0) {
        tempPort = SavePort (d);
        Select (d);
        ObjectRect (d, &r);
        InsetRect (&r, 3, 3);
        InvalRect (&r);
        RestorePort (tempPort);
      }
    }
  }
}

/*****************************************************************************
*
*   InvalDocument (d)
*       Invalidates visible area of a document
*
*****************************************************************************/

extern void InvalDocument (DoC d)

{
  InvalDocRows (d, 0, 0, 0);
}

/*****************************************************************************
*
*   UpdateDocument (d, from, to)
*       Invalidates and updates items in a document
*
*****************************************************************************/

extern void UpdateDocument (DoC d, Int2 from, Int2 to)

{
  Int2     bottom;
  Boolean  botvis;
  DocData  ddata;
  Int2     item;
  ItemPtr  itemPtr;
  Int2     last;
  Int4     line;
  Int4     off;
  RecT     r;
  RecT     s;
  BaR      sb;
  Int2     start;
  Int2     stop;
  Int2     swap;
  WindoW   tempPort;
  Int2     top;
  Boolean  topvis;
  Int2     vis;

  if (d != NULL && from >= 0 && to >= 0) {
    GetPanelExtra ((PaneL) d, &ddata);
    if (from == 0 || from > ddata.numItems) {
      start = 0;
      from = 0;
    } else {
      start = from - 1;
    }
    if (to == 0 || to > ddata.numItems) {
      stop = ddata.numItems;
      to = ddata.numItems;
    } else {
      stop = to;
    }
    for (item = start; item < stop; item++) {
      itemPtr = GetItemPtr (&ddata,item);
      if (itemPtr != NULL) {
        itemPtr->text = (CharPtr) MemFree (itemPtr->text);
        itemPtr->notCached = TRUE;
        itemPtr->neverCached = TRUE;
        itemPtr->cached = (CharPtr) MemFree (itemPtr->cached);
      }
    }
    if (ddata.upd != NULL) {
      ddata.upd (d, MIN (from, to), MAX (from, to));
    }
    if (Enabled (d) && AllParentsEnabled (d) &&
        Visible (d) && AllParentsVisible (d)) {
      tempPort = SavePort (d);
      ObjectRect (d, &r);
      InsetRect (&r, 4, 4);
      Select (d);
      if (from > to && from != 0 && to != ddata.numItems) {
        swap = from;
        from = to;
        to = swap;
      }
      topvis = FALSE;
      botvis = FALSE;
      if (from > 0 && from <= ddata.numItems) {
        topvis = ItemIsVisible (d, from, &top, NULL, NULL);
        if (topvis) {
          r.top = top;
        }
      } else {
        topvis = TRUE;
      }
      if (to > 0 && to <= ddata.numItems) {
        botvis = ItemIsVisible (d, to, NULL, &bottom, NULL);
        if (botvis) {
          r.bottom = bottom;
        }
      } else {
        botvis = TRUE;
      }
      if (topvis || botvis) {
        InsetRect (&r, -1, -1);
        InvalRect (&r);
      } else {
        ObjectRect (d, &s);
        InsetRect (&s, 4, 4);
        sb = GetSlateVScrollBar ((SlatE) d);
        off = GetBarValue (sb);
        item = GetItemNum (&ddata, off);
        line = (Int2) (off - GetStartsAt (&ddata, item));
        vis = VisLinesBelow (&ddata, &s, item, line, NULL, NULL);
        last = GetItemNum (&ddata, off + vis - 1);
        if (item >= stop || last < start) {
        } else {
          InsetRect (&r, -1, -1);
          InvalRect (&r);
        }
      }
      RestorePort (tempPort);
    }
    AdjustDocScroll (d);
  }
}

/*****************************************************************************
*
*   SaveTableItem (itemPtr, f, tabStops, tabCount)
*       Reformats and saves an item to a file
*
*****************************************************************************/

static void SaveTableItem (ItemPtr itemPtr, FILE *f,
                           Boolean tabStops, Int2 tabCount)

{
  Char     ch;
  Int2     col;
  ColXPtr  colFmt;
  Int2     i;
  Int2     insetLeft;
  Int2     insetRight;
  Char     just;
  Int2     next;
  Int2     num;
  Int2     pos;
  CharPtr  ptr;
  Int2     row;
  CharPtr  text;

  if (itemPtr != NULL && f != NULL) {
    text = itemPtr->text;
    colFmt = itemPtr->colFmt;
    if (text != NULL && *text != '\0' && colFmt != NULL) {
      pos = 0;
      row = 0;
      col = 0;
      ptr = text;
      ch = *ptr;
      while (ch != '\0') {
        if (ch == '\n') {
          col = 0;
          row++;
          ptr++;
          pos = 0;
          fputc ('\n', f);
        } else if (ch == '\t' && (! tabStops)) {
          col++;
          ptr++;
        } else  {
          num = 0;
          while (ch != '\0' && (ch != '\t' || tabStops) && ch != '\n') {
            num++;
            ch = ptr [num];
          }
          if (num > 0 && col < itemPtr->numCols) {
            just = justToChar [colFmt [col].just];
            if (just == 'c') {
              insetLeft = colFmt [col].charInset;
              insetRight = colFmt [col].charInset;
            } else if (just == 'l') {
              insetLeft = colFmt [col].charInset;
              insetRight = 0;
            } else if (just == 'r') {
              insetLeft = 0;
              insetRight = colFmt [col].charInset;
            } else {
              insetLeft = 0;
              insetRight = 0;
            }
            next = colFmt [col].position + insetLeft;
            if (tabCount > 0) {
              while (num > 0 && *ptr == '\t') {
                num--;
                ptr++;
                next += tabCount;
              }
            }
            while (pos < next) {
              fputc (' ', f);
              pos++;
            }
            if (just == 'r') {
              next = colFmt [col].position + colFmt [col].charWidth -
                     insetRight - num;
              while (pos < next) {
                fputc (' ', f);
                pos++;
              }
            } else if (just == 'c') {
              next = colFmt [col].position + (colFmt [col].charWidth -
                     insetRight - num) / 2;
              while (pos < next) {
                fputc (' ', f);
                pos++;
              }
            }
            for (i = 0; i < num; i++) {
              fputc (ptr [i], f);
              pos++;
            }
          }
          ptr += num;
        }
        ch = *ptr;
      }
    }
  }
}

/*****************************************************************************
*
*   SaveDocument (d, f)
*       Saves all document items to a file
*
*****************************************************************************/

extern void SaveDocument (DoC d, FILE *f)

{
  DocData   ddata;
  Int2      i;
  ItemData  itemData;
  ItemPtr   itemPtr;

  if (d != NULL && f != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    ddata.put = NULL;
    ddata.get = NULL;
    ddata.upd = NULL;
    for (i = 0; i < ddata.numItems; i++) {
      itemPtr = GetItemPtr (&ddata, i);
      if (itemPtr != NULL) {
        itemData.text = NULL;
        itemData.cached = NULL;
        itemData.prtProc = itemPtr->prtProc;
        itemData.dataPtr = itemPtr->dataPtr;
        itemData.font = itemPtr->font;
        itemData.extra = itemPtr->extra;
        itemData.openSpace = itemPtr->openSpace;
        itemData.tabStops = itemPtr->tabStops;
        itemData.numRows = itemPtr->numRows;
        itemData.numCols = itemPtr->numCols;
        itemData.minLines = itemPtr->minLines;
        itemData.minHeight = itemPtr->minHeight;
        itemData.colFmt = itemPtr->colFmt;
        SetTableCharFormat (itemData.colFmt, itemData.numCols);
        if (itemData.prtProc != NULL) {
          FormatText (&ddata, i + 1, &itemData, FALSE, ddata.tabCount);
          SetTableCharFormat (itemData.colFmt, itemData.numCols);
          if (i > 0 && itemData.openSpace > 0) {
            fputc ('\n', f);
          }
          SaveTableItem (&itemData, f,
                         (Boolean) (unsigned int) itemData.tabStops,
                         ddata.tabCount);
          MemFree (itemData.text);
        }
      }
    }
  }
}

/*****************************************************************************
*
*   SaveDocumentItem (d, f, item)
*       Saves one document item to a file
*
*****************************************************************************/

extern void SaveDocumentItem (DoC d, FILE *f, Int2 item)

{
  DocData   ddata;
  ItemData  itemData;
  ItemPtr   itemPtr;

  if (d != NULL && f != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    ddata.put = NULL;
    ddata.get = NULL;
    ddata.upd = NULL;
    if (item > 0 && item <= ddata.numItems) {
      itemPtr = GetItemPtr (&ddata, item - 1);
      if (itemPtr != NULL) {
        itemData.text = NULL;
        itemData.cached = NULL;
        itemData.prtProc = itemPtr->prtProc;
        itemData.dataPtr = itemPtr->dataPtr;
        itemData.font = itemPtr->font;
        itemData.extra = itemPtr->extra;
        itemData.openSpace = itemPtr->openSpace;
        itemData.tabStops = itemPtr->tabStops;
        itemData.numRows = itemPtr->numRows;
        itemData.numCols = itemPtr->numCols;
        itemData.minLines = itemPtr->minLines;
        itemData.minHeight = itemPtr->minHeight;
        itemData.colFmt = itemPtr->colFmt;
        SetTableCharFormat (itemData.colFmt, itemData.numCols);
        if (itemData.prtProc != NULL) {
          FormatText (&ddata, item, &itemData, FALSE, ddata.tabCount);
          SetTableCharFormat (itemData.colFmt, itemData.numCols);
          /*
          if (item > 1 && itemData.openSpace > 0) {
            fputc ('\n', f);
          }
          */
          SaveTableItem (&itemData, f,
                         (Boolean) (unsigned int) itemData.tabStops,
                         ddata.tabCount);
          MemFree (itemData.text);
        }
      }
    }
  }
}

/*****************************************************************************
*
*   PrintDocument (d)
*       Reformats and sends a document to the printer
*
*****************************************************************************/

#ifdef WIN_MOTIF
extern CharPtr Nlm_XrmGetResource (const Char PNTR _resource);
#endif

extern void PrintDocument (DoC d)

{
#ifndef WIN_MOTIF
  DocData   ddata;
  Boolean   goOn;
  Int2      i;
  Int2      item;
  ItemPtr   itemPtr;
  ItemData  itemData;
  Int2      line;
  Boolean   newPage;
  Int2      pixels;
  RecT      r;
  RecT      rct;
  FloatHi   scale;
  Int2      visBelow;
  WindoW    w;

  if (d != NULL) {
    GetPanelExtra ((PaneL) d, &ddata);
    ddata.put = NULL;
    ddata.get = NULL;
    ddata.upd = NULL;
    if (ddata.numItems > 0) {
      ObjectRect (d, &rct);
      InsetRect (&rct, 4, 4);
      w = StartPrinting ();
      if (w != NULL) {
        for (i = 0; i < MAXFONTS; i++) {
          fontHeights [i].font = NULL;
          fontHeights [i].height = 0;
        }
        goOn = TRUE;
        pixels = 0;
        item = 0;
        newPage = TRUE;
        while (item < ddata.numItems && goOn) {
          if (newPage) {
            goOn = StartPage ();
            newPage = FALSE;
            PrintingRect (&r);
          }
          if (goOn) {
            itemPtr = GetItemPtr (&ddata, item);
            if (itemPtr != NULL) {
              itemData.text = NULL;
              itemData.cached = NULL;
              itemData.prtProc = itemPtr->prtProc;
              itemData.dataPtr = itemPtr->dataPtr;
              itemData.font = itemPtr->font;
              itemData.extra = itemPtr->extra;
              itemData.openSpace = itemPtr->openSpace;
              itemData.keepWithNext = itemPtr->keepWithNext;
              itemData.keepTogether = itemPtr->keepTogether;
              itemData.newPage = itemPtr->newPage;
              itemData.tabStops = itemPtr->tabStops;
              itemData.numRows = itemPtr->numRows;
              itemData.numCols = itemPtr->numCols;
              itemData.minLines = itemPtr->minLines;
              itemData.minHeight = itemPtr->minHeight;
              itemData.colFmt = (ColXPtr) MemNew (itemData.numCols * sizeof (ColXData));
              if (itemData.colFmt != NULL) {
                MemCopy (itemData.colFmt, itemPtr->colFmt,
                         itemData.numCols * sizeof (ColXData));
                for (i = 0; i < itemData.numCols; i++) {
                  if (itemData.colFmt [i].zeroWidth) {
                    itemData.colFmt [i].pixWidth = 0;
                  }
                }
              }
              UpdateItemHeights (&itemData);
              scale = (FloatHi) (r.right - r.left) / (FloatHi) (rct.right - rct.left);
              for (i = 0; i < itemData.numCols; i++) {
                itemData.colFmt [i].pixWidth = (Int2) (scale *
                                               (FloatHi) itemData.colFmt [i].pixWidth);
              }
              SetTablePixFormat (itemData.colFmt, r.left, itemData.numCols);
              if (itemData.prtProc != NULL) {
                FormatText (&ddata, item + 1, &itemData, TRUE, ddata.tabCount);
                SetTablePixFormat (itemData.colFmt, r.left, itemData.numCols);
                if (pixels != 0) {
                  r.top += itemData.leadHeight;
                }
                visBelow = (r.bottom - r.top) / itemData.lineHeight;
                if ((itemData.keepTogether && visBelow < itemData.numRows) ||
                    itemData.newPage) {
                  goOn = EndPage ();
                  if (goOn) {
                    goOn = StartPage ();
                  }
                  newPage = FALSE;
                  PrintingRect (&r);
                  visBelow = (r.bottom - r.top) / itemData.lineHeight;
                }
                line = 0;
                while (visBelow + line < itemData.numRows && goOn) {
                  pixels = DrawTableItem (d, &ddata, &itemData, &r, item, line, NULL,
                                          NULL, NULL, TRUE, ddata.tabCount);
                  r.top += pixels;
                  line += visBelow;
                  goOn = EndPage ();
                  if (goOn) {
                    StartPage ();
                  }
                  newPage = FALSE;
                  PrintingRect (&r);
                  visBelow = (r.bottom - r.top) / itemData.lineHeight;
                }
                if (visBelow > 0 && goOn) {
                  pixels = DrawTableItem (d, &ddata, &itemData, &r, item, line, NULL,
                                          NULL, NULL, TRUE, ddata.tabCount);
                  r.top += pixels;
                }
              }
              MemFree (itemData.text);
              MemFree (itemData.colFmt);
            }
          }
          item++;
          if (r.top >= r.bottom) {
            goOn = EndPage ();
            newPage = TRUE;
          }
        }
        if ((! newPage) && goOn) {
          goOn = EndPage ();
        }
        EndPrinting (w);
        for (i = 0; i < MAXFONTS; i++) {
          fontHeights [i].font = NULL;
          fontHeights [i].height = 0;
        }
      }
    }
  }
#endif
#ifdef WIN_MOTIF
  Char     cmmd [256];
  FILE     *f;
  Int2     len;
  CharPtr  printCmd;
  Char     str [PATH_MAX];

  if (d != NULL) {
    TmpNam (str);
    f = FileOpen (str, "w");
    if (f != NULL) {
      SaveDocument (d, f);
      FileClose (f);
      printCmd = Nlm_XrmGetResource ("printCommand");
      if (printCmd != NULL) {
        StringNCpy_0 (cmmd, printCmd, sizeof (cmmd) - 1);
      } else {
        StringCpy (cmmd, "lp -c");
      }
      MemFree (printCmd);
      len = (Int2) StringLen (cmmd);
      while (len > 0 && cmmd [len] == ' ') {
        cmmd [len] = '\0';
        len--;
      }
      StringCat (cmmd, " ");
      StringCat (cmmd, str);
      StringCat (cmmd, "; rm ");
      StringCat (cmmd, str);
      system (cmmd);
    } else {
      StringCpy (cmmd, "rm ");
      StringCat (cmmd, str);
      system (cmmd);
    }
  }
#endif
}

/*****************************************************************************
*
*   displayTable
*       Default column table for file display functions
*
*****************************************************************************/

static ColData displayTable = {0, 0, 80, 0, NULL, 'l',
                               TRUE, FALSE, FALSE, FALSE, TRUE};

/*****************************************************************************
*
*   GetChar (fp)
*       Gets a single character from a file, returning '\0' on end of file
*
*****************************************************************************/

static Char GetChar (FILE *fp)

{
#if (defined(OS_DOS) || defined (OS_NT))
  Int2  actual;
  Char  buf [32];
  Char  ch;

  ch = '\0';
  actual = (Int2) FileRead (buf, 1, 1, fp);
  if (actual > 0) {
    ch = buf [0];
  }
  return ch;
#else
  int  ch;

  ch = fgetc (fp);
  if (ch == EOF) {
    ch = '\0';
  }
  return (Char) ch;
#endif
}

/*****************************************************************************
*
*   DisplayFancy (d, file, parFmt, colFmt, font, tabStops)
*       Uses a document to display a file, allowing paragraph and column
*       formatting to be specified
*
*****************************************************************************/

extern void DisplayFancy (DoC d, CharPtr file, ParPtr parFmt,
                          ColPtr colFmt, FonT font, Int2 tabStops)

{
  Int2     actual;
  Char     ch;
  Int2     cnt;
  Int4     cntr;
  DocData  ddata;
  FILE     *fp;
  Int2     leftOver;
  ParData  para;
  RecT     r;
  WindoW   tempPort;
  CharPtr  text;
  CharPtr  txt;
#ifdef WIN_MAC
  CharPtr  p;
  CharPtr  q;
#endif
#if (defined(OS_DOS) || defined (OS_NT))
  CharPtr  p;
  CharPtr  q;
#endif

  if (d != NULL && file != NULL && file [0] != '\0') {
    GetPanelExtra ((PaneL) d, &ddata);
    ddata.tabCount = tabStops;
    SetPanelExtra ((PaneL) d, &ddata);
    if (parFmt == NULL) {
      parFmt = &para;
    }
    if (colFmt == NULL) {
      colFmt = &displayTable;
    }
    Reset (d);
    tempPort = SavePort (d);
    ObjectRect (d, &r);
    InsetRect (&r, 4, 4);
    displayTable.pixWidth = r.right - r.left;
    if (font == NULL) {
      font = systemFont;
    }
    text = (CharPtr) MemNew (24000);
    if (text != NULL) {
      fp = FileOpen (file, "r");
      if (fp != NULL) {
        leftOver = 0;
        cntr = FileLength (file);
        cnt = (Int2) MIN (cntr, 15000L);
        para.openSpace = FALSE;
        para.keepWithNext = FALSE;
        para.keepTogether = FALSE;
        para.newPage = FALSE;
        para.tabStops = TRUE;
        para.minLines = 0;
        para.minHeight = 0;
        while (cnt > 0 && cntr > 0) {
          txt = text + leftOver;
          actual = (Int2) FileRead (txt, 1, cnt, fp);
          if (actual > 0) {
            cnt = actual;
            txt [cnt] = '\0';
            ch = GetChar (fp);
            while (ch != '\0' && ch != '\n' && cnt < 15900) {
              txt [cnt] = ch;
              cnt++;
              ch = GetChar (fp);
            }
            while ((ch == '\n' || ch == '\r') && cnt < 15900) {
              txt [cnt] = ch;
              cnt++;
              ch = GetChar (fp);
            }
            txt [cnt] = '\0';
#if (defined(OS_DOS) || defined (OS_NT))
            p = text;
            q = text;
            while (*p) {
              if (*p == '\r') {
                p++;
              } else {
                *q = *p;
                p++;
                q++;
              }
            }
            *q = '\0';
#endif
#ifdef WIN_MAC
            p = text;
            q = text;
            while (*p) {
              if (*p == '\n') {
                *q = '\n';
                p++;
                q++;
                if (*p == '\r') {
                  p++;
                }
              } else if (*p == '\r') {
                *q = '\n';
                p++;
                q++;
                if (*p == '\n') {
                  p++;
                }
              } else {
                *q = *p;
                p++;
                q++;
              }
            }
            *q = '\0';
#endif
            AppendText (d, text, parFmt, colFmt, font);
            leftOver = 1;
            text [0] = ch;
            cntr -= cnt;
            cnt = (Int2) MIN (cntr, 15000L);
          } else {
            cnt = 0;
            cntr = 0;
          }
        }
        FileClose (fp);
      }
      text = (CharPtr) MemFree (text);
    }
    if (Enabled (d) && AllParentsEnabled (d) &&
        Visible (d) && AllParentsVisible (d)) {
      ObjectRect (d, &r);
      InsetRect (&r, 3, 3);
      Select (d);
      InvalRect (&r);
    }
    if (! ddata.autoAdjust) {
      AdjustDocScroll (d);
    }
    RestorePort (tempPort);
  }
}

/*****************************************************************************
*
*   DisplayFile (d, file, font)
*       Simple function to display a file in a document
*
*****************************************************************************/

extern void DisplayFile (DoC d, CharPtr file, FonT font)

{
  DisplayFancy (d, file, NULL, NULL, font, 4);
}

