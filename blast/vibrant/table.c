/*   table.c
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
* File Name:  table.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   1/1/91
*
* $Revision: 6.1 $
*
* File Description:   Active tabular text
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: table.c,v $
* Revision 6.1  1998/07/02 18:24:32  vakatov
* Cleaned the code & made it pass through the C++ compilation
*
* Revision 6.0  1997/08/25 18:56:44  madden
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

#include "panels.h"

#define numCellsPerPage 256

typedef  struct  tableData {
  Handle     title;
  Int2       numPages;
  Handle     pageHandles;
  Int2       numColumns;
  Handle     colHandles;
  FonT       font;
  Int2       topRow;
  Int2       leftMargin;
  Int2       rightMargin;
  Int2       width;
  Int2       lineHeight;
  Int2       visLines;
  Int2       numLines;
  Int2       currentRow;
  Int2       tabRow;
  Int2       returnRow;
  Int2       currentColumn;
  TableProc  action;
} TableData;

typedef  Handle PageRec, PNTR PagePtr;

typedef  struct  cellRec {
  Uint4    start;
  size_t   count;
  Boolean  hilight;
  Boolean  gray;
} CellRec, PNTR CellPtr;

typedef  struct  colRec {
  TableProc  click;
  Int2       pos;
  Int2       width;
  Char       just;
  Boolean    wrap;
  Boolean    bar;
} ColRec, PNTR ColPtr;

static Boolean SetCellRec (TablE t, Int2 row, Int2 column, CellRec * cellRec)

{
  Int4       cell;
  CellPtr    cellPtr;
  Int2       index;
  Int2       numColumns;
  Int2       numPages;
  Int2       page;
  Handle     pageHandles;
  PagePtr    pagePtr;
  Boolean    rsult;
  TableData  tdata;

  rsult = FALSE;
  if (t != NULL && cellRec != NULL) {
    GetPanelExtra ((PaneL) t, &tdata);
    numPages = tdata.numPages;
    pageHandles = tdata.pageHandles;
    numColumns = tdata.numColumns;
    cell = (Int4) row * (Int4) numColumns + (Int4) column;
    page = (Int2) (cell / numCellsPerPage);
    index = (Int2) (cell % numCellsPerPage);
    if (pageHandles != NULL && page < numPages && column < numColumns) {
      pagePtr = (PagePtr) HandLock (pageHandles);
      if (pagePtr != NULL && pagePtr [page] != NULL) {
        cellPtr = (CellPtr) HandLock (pagePtr [page]);
        if (cellPtr != NULL) {
          cellPtr [index] = *cellRec;
          rsult = TRUE;
        }
        HandUnlock (pagePtr [page]);
      }
      HandUnlock (pageHandles);
    }
  }
  return rsult;
}

static Boolean GetCellRec (TablE t, Int2 row, Int2 column, CellRec * cellRec)

{
  Int4       cell;
  CellPtr    cellPtr;
  Int2       index;
  Int2       numColumns;
  Int2       numPages;
  Int2       page;
  Handle     pageHandles;
  PagePtr    pagePtr;
  Boolean    rsult;
  TableData  tdata;

  rsult = FALSE;
  if (t != NULL && cellRec != NULL) {
    GetPanelExtra ((PaneL) t, &tdata);
    numPages = tdata.numPages;
    pageHandles = tdata.pageHandles;
    numColumns = tdata.numColumns;
    cell = (Int4) row * (Int4) numColumns + (Int4) column;
    page = (Int2) (cell / numCellsPerPage);
    index = (Int2) (cell % numCellsPerPage);
    if (pageHandles != NULL && page < numPages && column < numColumns) {
      pagePtr = (PagePtr) HandLock (pageHandles);
      if (pagePtr != NULL && pagePtr [page] != NULL) {
        cellPtr = (CellPtr) HandLock (pagePtr [page]);
        if (cellPtr != NULL) {
          *cellRec = cellPtr [index];
          rsult = TRUE;
        }
        HandUnlock (pagePtr [page]);
      }
      HandUnlock (pageHandles);
    }
  }
  return rsult;
}

#ifdef XXX_UNUSED_STATIC_FUNC
static Boolean SetColRec (TablE t, Int2 column, ColRec * colRec)
{
  Handle     colHandles;
  ColPtr     colPtr;
  Int2       numColumns;
  Boolean    rsult;
  TableData  tdata;

  rsult = FALSE;
  if (t != NULL && colRec != NULL) {
    GetPanelExtra ((PaneL) t, &tdata);
    numColumns = tdata.numColumns;
    colHandles = tdata.colHandles;
    if (colHandles != NULL && column < numColumns) {
      colPtr = (ColPtr) HandLock (colHandles);
      if (colPtr != NULL) {
        colPtr [column] = *colRec;
        rsult = TRUE;
      }
      HandUnlock (colHandles);
    }
  }
  return rsult;
}
#endif /* XXX_UNUSED_STATIC_FUNC */


static Boolean GetColRec (TablE t, Int2 column, ColRec * colRec)

{
  Handle     colHandles;
  ColPtr     colPtr;
  Int2       numColumns;
  Boolean    rsult;
  TableData  tdata;

  rsult = FALSE;
  if (t != NULL && colRec != NULL) {
    GetPanelExtra ((PaneL) t, &tdata);
    numColumns = tdata.numColumns;
    colHandles = tdata.colHandles;
    if (colHandles != NULL && column < numColumns) {
      colPtr = (ColPtr) HandLock (colHandles);
      if (colPtr != NULL) {
        *colRec = colPtr [column];
        rsult = TRUE;
      }
      HandUnlock (colHandles);
    }
  }
  return rsult;
}

static void DrawTable (PaneL t)

{
  CellPtr    cellPtr;
  CharPtr    charPtr;
  ColPtr     colPtr;
  Int2       column;
  Handle     colHandles;
  RecT       dr;
  Handle     h;
  Int2       index;
  Int2       numColumns;
  Int2       numPages;
  Int2       off;
  Int2       page;
  Handle     pageHandles;
  PagePtr    pagePtr;
  RecT       q;
  RecT       r;
  Int2       row;
  SlatE      s;
  RecT       sr;
  TableData  tdata;

  s = (SlatE) Parent (t);
  GetPanelExtra (t, &tdata);
  h = tdata.title;
  numPages = tdata.numPages;
  pageHandles = tdata.pageHandles;
  numColumns = tdata.numColumns;
  colHandles = tdata.colHandles;
  SelectFont (tdata.font);
  ObjectRect (s, &sr);
  InsetRect (&sr, 4, 4);
  GetOffset (t, NULL, &off);
  if (h != NULL && pageHandles != NULL && colHandles != NULL && numColumns > 0) {
    charPtr = (CharPtr) HandLock (h);
    pagePtr = (PagePtr) HandLock (pageHandles);
    colPtr = (ColPtr) HandLock (colHandles);
    row = 0;
    column = 0;
    for (page = 0; page < numPages; page++) {
      if (pagePtr [page] != NULL) {
        cellPtr = (CellPtr) HandLock (pagePtr [page]);
        if (cellPtr != NULL) {
          for (index = 0; index < numCellsPerPage; index++) {
            if (colPtr [column].width > 0) {
              r.left = colPtr [column].pos;
              r.top = tdata.topRow + row * tdata.lineHeight;
              r.right = r.left + colPtr [column].width;
              r.bottom = r.top + tdata.lineHeight;
              OffsetRect (&r, 0, -off);
              if (RectInRect (&r, &sr) &&
                  (updateRgn == NULL || RectInRgn (&r, updateRgn))) {
                if (cellPtr [index].hilight) {
                  InvertColors ();
                }
                EraseRect (&r);
                q = r;
                InsetRect (&q, 5, 0);
                if (cellPtr [index].count > 0) {
                  DrawText (&q, charPtr + cellPtr [index].start,
                            cellPtr [index].count, colPtr [column].just,
                            cellPtr [index].gray);
                }
                if (cellPtr [index].hilight) {
                  InvertColors ();
                }
              }
            }
            column++;
            if (column >= numColumns) {
              row++;
              column = 0;
            }
          }
        }
        HandUnlock (pagePtr [page]);
      }
    }
    HandUnlock (colHandles);
    HandUnlock (pageHandles);
    HandUnlock (h);
  }
  if (colHandles != NULL) {
    InvertMode ();
    colPtr = (ColPtr) HandLock (colHandles);
    for (column = 0; column < numColumns; column++) {
      if (colPtr [column].bar) {
        ObjectRect (t, &r);
        r.left = colPtr [column].pos;
        SectRect (&r, &sr, &dr);
        dr.bottom = dr.top + tdata.visLines * tdata.lineHeight - 1;
        MoveTo (dr.left, dr.top);
        LineTo (dr.left, dr.bottom);
      }
    }
    HandUnlock (colHandles);
    CopyMode ();
  }
  SelectFont (systemFont);
}

extern void RecordColumn (TablE t, Int2 wid,
                          Char just, Boolean wrap,
                          Boolean bar, TableProc actn)

{
  Int2       chunk;
  ColPtr     colPtr;
  Handle     colHandles;
  Int2       numColumns;
  RecT       r;
  Int2       rightMargin;
  TableData  tdata;

  if (t != NULL) {
    ObjectRect (t, &r);
    GetPanelExtra ((PaneL) t, &tdata);
    numColumns = tdata.numColumns;
    colHandles = tdata.colHandles;
    chunk = 8;
    if (colHandles != NULL) {
      if (numColumns % chunk == 0) {
        colHandles = HandMore (colHandles, sizeof (ColRec) *
                                 (numColumns / chunk + 1) * chunk);
      }
    } else {
      colHandles = HandNew (sizeof (ColRec) * chunk);
    }
    numColumns++;
    colPtr = (ColPtr) HandLock (colHandles);
    colPtr [numColumns - 1].click = actn;
    if (numColumns == 1) {
      colPtr [numColumns - 1].pos = r.left;
    } else if (numColumns > 1) {
      colPtr [numColumns - 1].pos = colPtr [numColumns - 2].pos +
                                    colPtr [numColumns - 2].width;
    }
    colPtr [numColumns - 1].width = wid * MaxCharWidth ();
    colPtr [numColumns - 1].just = just;
    colPtr [numColumns - 1].wrap = wrap;
    colPtr [numColumns - 1].bar = bar;
    HandUnlock (colHandles);
    rightMargin = colPtr [numColumns - 1].pos + colPtr [numColumns - 1].width;
    if (tdata.rightMargin < rightMargin) {
      tdata.rightMargin = rightMargin;
    }
    tdata.numColumns = numColumns;
    tdata.colHandles = colHandles;
    SetPanelExtra ((PaneL) t, &tdata);
  }
}

static void RecordSegment (TablE t, Uint4 start,
                           Uint4 prev, Int2 len,
                           Int2 row, Int2 column)

{
  Int4       cell;
  CellPtr    cellPtr;
  Int2       index;
  Int2       numColumns;
  Int2       numPages;
  Int2       page;
  Handle     pageHandles;
  PagePtr    pagePtr;
  TableData  tdata;

  if (t != NULL) {
    GetPanelExtra ((PaneL) t, &tdata);
    numPages = tdata.numPages;
    pageHandles = tdata.pageHandles;
    numColumns = tdata.numColumns;
    cell = (Int4) row * (Int4) numColumns + (Int4) column;
    page = (Int2) (cell / numCellsPerPage);
    index = (Int2) (cell % numCellsPerPage);
    if (page >= numPages) {
      numPages = page + 1;
      if (pageHandles != NULL) {
        pageHandles = HandMore (pageHandles, sizeof (PageRec) * numPages);
      } else {
        pageHandles = HandNew (sizeof (PageRec));
      }
      tdata.numPages = numPages;
      tdata.pageHandles = pageHandles;
      SetPanelExtra ((PaneL) t, &tdata);
      pagePtr = (PagePtr) HandLock (pageHandles);
      if (pagePtr != NULL) {
        pagePtr [page] = HandNew (sizeof (CellRec) * numCellsPerPage);
      }
      HandUnlock (pageHandles);
    }
    if (pageHandles != NULL && column < numColumns) {
      pagePtr = (PagePtr) HandLock (pageHandles);
      if (pagePtr [page] != NULL) {
        cellPtr = (CellPtr) HandLock (pagePtr [page]);
        if (cellPtr != NULL) {
          cellPtr [index].start = start + prev;
          cellPtr [index].count = len;
          cellPtr [index].hilight = FALSE;
          cellPtr [index].gray = FALSE;
        }
        HandUnlock (pagePtr [page]);
      }
      HandUnlock (pageHandles);
    }
  }
}

static Int2 GetNextBlock (CharPtr title, Int2 maxwid)

{
  Int2  i;
  Int2  j;
  Int2  wid;

  i = 0;
  j = 0;
  wid = 0;
  if (title != NULL && maxwid > 0) {
    while (title [i] != '\0' && title [i] != '\n' &&
           title [i] != '\r' && title [i] != '\t' &&
           wid <= maxwid) {
      wid += CharWidth (title [i]);
      i++;
    }
    j = i;
    if (wid > maxwid) {
      j--;
      while (TextWidth (title, i) > maxwid) {
        i--;
      }
      while (i > 0 && (! IS_WHITESP(title [i])) && title [i - 1] != '-') {
        i--;
      }
      while (i > 0 && (IS_WHITESP(title [i - 1])) && title [i - 1] != '-') {
        i--;
      }
    } else if (title [i] != '\0') {
      while (i > 0 && (IS_WHITESP(title [i - 1]))) {
        i--;
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

static Uint4 CopyToHandle (TablE t, CharPtr title)

{
  CharPtr    charPtr;
  size_t     chunk;
  Handle     h;
  size_t     len;
  size_t     prev;
  size_t     size;
  TableData  tdata;

  prev = 0;
  if (t != NULL && title != NULL) {
    GetPanelExtra ((PaneL) t, &tdata);
    chunk = 4096;
    h = tdata.title;
    if (h != NULL) {
      charPtr = (CharPtr) HandLock (h);
      prev = StringLen (charPtr);
      HandUnlock (h);
      len = prev + StringLen (title);
      size = ((len + 1) / chunk + 1) * chunk;
      if ((len + 1) / chunk > (prev + 1) / chunk) {
        h = HandMore (h, size);
      }
      charPtr = (CharPtr) HandLock (h);
      StringNCat (charPtr, title, size);
      HandUnlock (h);
      tdata.title = h;
      SetPanelExtra ((PaneL) t, &tdata);
    } else {
      len = StringLen (title);
      size = ((len + 1) / chunk + 1) * chunk;
      h = HandNew (size);
      charPtr = (CharPtr) HandLock (h);
      StringNCpy (charPtr, title, size);
      HandUnlock (h);
      tdata.title = h;
      SetPanelExtra ((PaneL) t, &tdata);
    }
  }
  return prev;
}

extern void AppendTableText (TablE t, CharPtr title)

{
  Char       ch;
  ColPtr     colPtr;
  Int2       column;
  Handle     colHandles;
  RecT       dr;
  Int2       len;
  Int2       maxwid;
  Int2       newLines;
  Int2       numColumns;
  Int2       off;
  Uint4      prev;
  RecT       r;
  Int2       returnRow;
  Int2       row;
  SlatE      s;
  RecT       sr;
  Uint4      start;
  Int2       tabRow;
  TableData  tdata;
  WindoW     tempPort;

  if (t != NULL && title != NULL && title [0] != '\0') {
    tempPort = SavePort (t);
    prev = CopyToHandle (t, title);
    GetPanelExtra ((PaneL) t, &tdata);
    SelectFont (tdata.font);
    numColumns = tdata.numColumns;
    colHandles = tdata.colHandles;
    s = (SlatE) Parent (t);
    start = 0;
    len = 0;
    row = tdata.currentRow;
    tabRow = tdata.tabRow;
    returnRow = tdata.returnRow;
    column = tdata.currentColumn;
    if (colHandles != NULL) {
      colPtr = (ColPtr) HandLock (colHandles);
      while (title [start] != '\0') {
        while (title [start] == ' ') {
          start++;
        }
        if (title [start] != '\0') {
          ch = title [start];
          if (ch != '\0' && ch != '\n' && ch != '\r' && ch != '\t') {
            maxwid = INT2_MAX;
            if (numColumns > 0 && column < numColumns &&
                colPtr [column].wrap && colPtr [column].width >= 12) {
              maxwid = colPtr [column].width - 12;
            }
            len = GetNextBlock (title + start, maxwid);
            if (len > 0) {
              RecordSegment (t, start, prev, len, row, column);
              start += len;
            } else if (colPtr [column].wrap) {
              while (TextWidth (title + start, len) <= maxwid) {
                len++;
              }
              len--;
              if (len > 0) {
                RecordSegment (t, start, prev, len, row, column);
              }
              ch = title [start];
              while (ch != '\0' && ch != '\n' && ch != '\r' && ch != '\t') {
                start++;
                ch = title [start];
              }
            }
          }
          ch = title [start];
          if (ch == '\n') {
            start++;
            row = returnRow;
            tabRow = row;
            returnRow = MAX (returnRow, row + 1);
            column = 0;
          } else if (ch == '\r') {
            start++;
            row++;
            returnRow = MAX (returnRow, row + 1);
          } else if (ch == '\t') {
            start++;
            row = tabRow;
            column++;
          } else if (ch != '\0' && colPtr [column].wrap) {
            if (len == 0) {
              start++;
            }
            row++;
            returnRow = MAX (returnRow, row + 1);
          } else if (ch != '\0') {
            start++;
          }
        }
      }
      HandUnlock (colHandles);
    }
    newLines = row - tdata.numLines;
    r.left = tdata.leftMargin;
    r.top = tdata.topRow;
    r.right = tdata.rightMargin;
    r.bottom = tdata.topRow + row * tdata.lineHeight;
    RegisterRect ((PaneL) t, &r);
    r.top = tdata.topRow + tdata.numLines * tdata.lineHeight;
    RegisterRow (s, r.top, tdata.lineHeight, newLines);
    r.bottom = r.top + newLines * tdata.lineHeight;
    GetPanelExtra ((PaneL) t, &tdata);
    tdata.currentRow = row;
    tdata.tabRow = tabRow;
    tdata.returnRow = returnRow;
    tdata.currentColumn = column;
    tdata.numLines += newLines;
    SetPanelExtra ((PaneL) t, &tdata);
    SelectFont (systemFont);
    if (Enabled (t) && AllParentsEnabled (t) &&
        Visible (t) && AllParentsVisible (t)) {
      ObjectRect (s, &sr);
      InsetRect (&sr, 4, 4);
      GetOffset (t, NULL, &off);
      OffsetRect (&r, 0, -off);
      SectRect (&r, &sr, &dr);
      if (RectInRect (&dr, &sr)) {
        Select (t);
        InvalRect (&dr);
      }
    }
    RestorePort (tempPort);
  }
}

extern Boolean GetTableText (TablE t, Int2 row,
                             Int2 column, CharPtr text,
                             Uint4 maxsize)

{
  CellRec    cellRec;
  CharPtr    charPtr;
  Uint4      count;
  Handle     h;
  Boolean    rsult;
  Uint4      start;
  TableData  tdata;

  rsult = FALSE;
  if (text != NULL && maxsize > 0) {
    *text = '\0';
  }
  if (t != NULL && text != NULL && maxsize > 0) {
    h = NULL;
    count = 0;
    start = 0;
    if (row > 0 && column > 0) {
      row--;
      column--;
      GetPanelExtra ((PaneL) t, &tdata);
      h = tdata.title;
      GetCellRec (t, row, column, &cellRec);
      start = cellRec.start;
      count = cellRec.count;
    } else if (row > 0) {
      row--;
      GetPanelExtra ((PaneL) t, &tdata);
      h = tdata.title;
      GetCellRec (t, row, 0, &cellRec);
      start = cellRec.start;
      if (GetCellRec (t, row + 1, 0, &cellRec)) {
        if (cellRec.count > 0) {
          count = cellRec.start - start;
        } else {
          h = tdata.title;
          if (h != NULL) {
            charPtr = (CharPtr) HandLock (h);
            count = StringLen (charPtr) - start;
            HandUnlock (h);
          }
        }
      } else {
        h = tdata.title;
        if (h != NULL) {
          charPtr = (CharPtr) HandLock (h);
          count = StringLen (charPtr) - start;
          HandUnlock (h);
        }
      }
    } else {
      GetPanelExtra ((PaneL) t, &tdata);
      h = tdata.title;
      if (h != NULL) {
        charPtr = (CharPtr) HandLock (h);
        count = StringLen (charPtr);
        HandUnlock (h);
      }
    }
    if (count > maxsize - 1) {
      count = maxsize - 1;
    }
    if (h != NULL) {
      charPtr = (CharPtr) HandLock (h);
      charPtr += start;
      while (count > 0) {
        *text = *charPtr;
        text++;
        charPtr++;
        count--;
      }
      *text = '\0';
      HandUnlock (h);
      rsult = TRUE;
    }
  }
  return rsult;
}

extern Uint4 TableTextLength (TablE t, Int2 row, Int2 column)

{
  CellRec    cellRec;
  CharPtr    charPtr;
  Uint4      count;
  Handle     h;
  Uint4      start;
  TableData  tdata;

  count = 0;
  if (t != NULL) {
    count = 0;
    start = 0;
    if (row > 0 && column > 0) {
      row--;
      column--;
      GetPanelExtra ((PaneL) t, &tdata);
      GetCellRec (t, row, column, &cellRec);
      count = cellRec.count;
    } else if (row > 0) {
      row--;
      GetPanelExtra ((PaneL) t, &tdata);
      GetCellRec (t, row, 0, &cellRec);
      start = cellRec.start;
      if (GetCellRec (t, row + 1, 0, &cellRec)) {
        if (cellRec.count > 0) {
          count = cellRec.start - start;
        } else {
          h = tdata.title;
          if (h != NULL) {
            charPtr = (CharPtr) HandLock (h);
            count = StringLen (charPtr) - start;
            HandUnlock (h);
          }
        }
      } else {
        h = tdata.title;
        if (h != NULL) {
          charPtr = (CharPtr) HandLock (h);
          count = StringLen (charPtr) - start;
          HandUnlock (h);
        }
      }
    } else {
      GetPanelExtra ((PaneL) t, &tdata);
      h = tdata.title;
      if (h != NULL) {
        charPtr = (CharPtr) HandLock (h);
        count = StringLen (charPtr);
        HandUnlock (h);
      }
    }
  }
  return count;
}

extern void SetTableBlockHilight (TablE t, Int2 firstRow,
                                  Int2 lastRow, Int2 firstColumn,
                                  Int2 lastColumn, Boolean hilight)

{
  CellRec    cellRec;
  ColRec     colRec;
  Int2       column;
  Int2       off;
  RecT       r;
  Int2       row;
  SlatE      s;
  RecT       sr;
  TableData  tdata;
  WindoW     tempPort;

  if (t != NULL && firstRow > 0 && lastRow > 0 &&
      firstColumn > 0 && lastColumn > 0) {
    tempPort = SavePort (t);
    s = (SlatE) Parent (t);
    GetPanelExtra ((PaneL) t, &tdata);
    ObjectRect (s, &sr);
    InsetRect (&sr, 4, 4);
    GetOffset (t, NULL, &off);
    firstRow--;
    lastRow--;
    firstColumn--;
    lastColumn--;
    for (column = firstColumn; column <= lastColumn; column++) {
      GetColRec (t, column, &colRec);
      for (row = firstRow; row <= lastRow; row++) {
        GetCellRec (t, row, column, &cellRec);
        cellRec.hilight = hilight;
        SetCellRec (t, row, column, &cellRec);
        r.left = colRec.pos;
        r.top = tdata.topRow + row * tdata.lineHeight;
        r.right = r.left + colRec.width;
        r.bottom = r.top + tdata.lineHeight;
        OffsetRect (&r, 0, -off);
        if (Enabled (t) && AllParentsEnabled (t) &&
            Visible (t) && AllParentsVisible (t) &&
            RectInRect (&r, &sr)) {
          Select (t);
          InvalRect (&r);
        }
      }
    }
    RestorePort (tempPort);
  }
}

extern void SetTableHilight (TablE t, Int2 row,
                             Int2 column, Boolean hilight)

{
  SetTableBlockHilight (t, row, row, column, column, hilight);
}

extern Boolean GetTableHilight (TablE t, Int2 row, Int2 column)

{
  CellRec  cellRec;
  Boolean  rsult;

  rsult = FALSE;
  if (t != NULL && row > 0 && column > 0) {
    row--;
    column--;
    GetCellRec (t, row, column, &cellRec);
    rsult = cellRec.hilight;
  }
  return rsult;
}

extern void SetTableBlockGray (TablE t, Int2 firstRow,
                               Int2 lastRow, Int2 firstColumn,
                               Int2 lastColumn, Boolean gray)

{
  CellRec    cellRec;
  ColRec     colRec;
  Int2       column;
  Int2       off;
  RecT       r;
  Int2       row;
  SlatE      s;
  RecT       sr;
  TableData  tdata;
  WindoW     tempPort;

  if (t != NULL && firstRow > 0 && lastRow > 0 &&
      firstColumn > 0 && lastColumn > 0) {
    tempPort = SavePort (t);
    s = (SlatE) Parent (t);
    GetPanelExtra ((PaneL) t, &tdata);
    ObjectRect (s, &sr);
    InsetRect (&sr, 4, 4);
    GetOffset (t, NULL, &off);
    firstRow--;
    lastRow--;
    firstColumn--;
    lastColumn--;
    for (column = firstColumn; column <= lastColumn; column++) {
      GetColRec (t, column, &colRec);
      for (row = firstRow; row <= lastRow; row++) {
        GetCellRec (t, row, column, &cellRec);
        cellRec.gray = gray;
        SetCellRec (t, row, column, &cellRec);
        r.left = colRec.pos;
        r.top = tdata.topRow + row * tdata.lineHeight;
        r.right = r.left + colRec.width;
        r.bottom = r.top + tdata.lineHeight;
        OffsetRect (&r, 0, -off);
        if (Enabled (t) && AllParentsEnabled (t) &&
            Visible (t) && AllParentsVisible (t) &&
            RectInRect (&r, &sr)) {
          Select (t);
          InvalRect (&r);
        }
      }
    }
    RestorePort (tempPort);
  }
}

extern void SetTableGray (TablE t, Int2 row,
                          Int2 column, Boolean gray)

{
  SetTableBlockGray (t, row, row, column, column, gray);
}

extern Boolean GetTableGray (TablE t, Int2 row, Int2 column)

{
  CellRec  cellRec;
  Boolean  rsult;

  rsult = FALSE;
  if (t != NULL && row > 0 && column > 0) {
    row--;
    column--;
    GetCellRec (t, row, column, &cellRec);
    rsult = cellRec.gray;
  }
  return rsult;
}

extern Int2 TableNumLines (TablE t)

{
  Int2       rsult;
  TableData  tdata;

  rsult = 0;
  if (t != NULL) {
    GetPanelExtra ((PaneL) t, &tdata);
    rsult = tdata.numLines;
  }
  return rsult;
}

extern Int2 TableVisLines (TablE t)

{
  Int2       rsult;
  TableData  tdata;

  rsult = 0;
  if (t != NULL) {
    GetPanelExtra ((PaneL) t, &tdata);
    rsult = tdata.visLines;
  }
  return rsult;
}

static void TableClick (PaneL t, PoinT pt)

{
  TableProc  actn;
  ColRec     colRec;
  Int2       column;
  Boolean    goOn;
  Int2       off;
  RecT       r;
  Int2       row;
  SlatE      s;
  RecT       sr;
  TableData  tdata;

  if (t != NULL) {
    actn = NULL;
    s = (SlatE) Parent (t);
    GetPanelExtra (t, &tdata);
    ObjectRect (s, &sr);
    InsetRect (&sr, 4, 4);
    GetOffset (t, NULL, &off);
    if (pt.y + off >= tdata.topRow) {
      row = (pt.y + off - tdata.topRow) / tdata.lineHeight;
      goOn = TRUE;
      column = 0;
      r.left = 0;
      r.right = 0;
      r.top = tdata.topRow + row * tdata.lineHeight;
      r.bottom = r.top + tdata.lineHeight;
      OffsetRect (&r, 0, -off);
      while (goOn && column < tdata.numColumns) {
        GetColRec ((TablE) t, column, &colRec);
        actn = colRec.click;
        r.left = colRec.pos;
        r.right = r.left + colRec.width;
        if (RectInRect (&r, &sr) && PtInRect (pt, &r)) {
          goOn = FALSE;
          if (actn != NULL) {
            actn ((TablE) t, row + 1, column + 1);
          }
          actn = tdata.action;
          if (actn != NULL) {
            actn ((TablE) t, row + 1, column + 1);
          }
        } else {
          column++;
        }
      }
    }
  }
}

static void NewTable (TablE t, Int2 minwid, FonT font, TableProc actn)

{
  PoinT      npt;
  RecT       r;
  SlatE      s;
  TableData  tdata;

  SelectFont (systemFont);
  s = (SlatE) Parent (t);
  ObjectRect (s, &r);
  InsetRect (&r, 4, 4);
  GetNextPosition (t, &npt);
  tdata.title = NULL;
  tdata.numPages = 0;
  tdata.pageHandles = NULL;
  tdata.numColumns = 0;
  tdata.colHandles = NULL;
  tdata.font = font;
  tdata.topRow = npt.y;
  tdata.leftMargin = npt.x;
  tdata.rightMargin = npt.x + minwid;
  tdata.width = tdata.rightMargin - tdata.leftMargin;
  SelectFont (font);
  tdata.lineHeight = LineHeight ();
  SelectFont (systemFont);
  tdata.visLines = (r.bottom - r.top + 1) / tdata.lineHeight;
  tdata.numLines = 0;
  tdata.currentRow = 0;
  tdata.tabRow = 0;
  tdata.returnRow = 1;
  tdata.currentColumn = 0;
  tdata.action = actn;
  SetPanelExtra ((PaneL) t, &tdata);
  LoadRect (&r, npt.x, npt.y, tdata.rightMargin, npt.y);
  RegisterRect ((PaneL) t, &r);
  Break (t);
}

static void ResetTable (PaneL t)

{
  Int2       i;
  PagePtr    pagePtr;
  TableData  tdata;

  GetPanelExtra (t, &tdata);
  if (tdata.title != NULL) {
    HandFree (tdata.title);
  }
  if (tdata.pageHandles != NULL) {
    pagePtr = (PagePtr) HandLock (tdata.pageHandles);
    for (i = 0; i < tdata.numPages; i++) {
      HandFree (pagePtr [i]);
    }
    HandUnlock (tdata.pageHandles);
    HandFree (tdata.pageHandles);
  }
  if (tdata.colHandles != NULL) {
    HandFree (tdata.colHandles);
  }
  NewTable ((TablE) t, tdata.width, tdata.font, tdata.action);
}

extern TablE TablePanel (SlatE s, Int2 minwid, FonT font, TableProc actn)

{
  TablE   t;
  WindoW  tempPort;

  t = NULL;
  if (s != NULL) {
    tempPort = SavePort (s);
    t = (TablE) CustomPanel (s, DrawTable, sizeof (TableData), ResetTable);
    if (t != NULL) {
      SetPanelClick ((PaneL) t, TableClick, NULL, NULL, NULL);
      NewTable (t, minwid * stdCharWidth, font, actn);
    }
    RestorePort (tempPort);
  }
  return t;
}
