/*  tofile.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE                          
*               National Center for Biotechnology Information
*                                                                          
*  This software/database is a "United States Government Work" under the   
*  terms of the United States Copyright Act.  It was written as part of    
*  the author's official duties as a United States Government employee and 
*  thus cannot be copyrighted.  This software/database is freely available 
*  to the public for use. The National Library of Medicine and the U.S.    
*  Government have not placed any restriction on its use or reproduction.  
*                                                                          
*  Although all reasonable efforts have been taken to ensure the accuracy  
*  and reliability of the software and data, the NLM and the U.S.          
*  Government do not and cannot warrant the performance or results that    
*  may be obtained by using this software or data. The NLM and the U.S.    
*  Government disclaim all warranties, express or implied, including       
*  warranties of performance, merchantability or fitness for any particular
*  purpose.                                                                
*                                                                          
*  Please cite the author in any work or product based on this material.   
*
* ===========================================================================
*
* File Name:  tofile.c
*
* Author:  Jonathan Kans
*   
* Version Creation Date: 9/24/91
*
* $Revision: 6.2 $
*
* File Description:  Converts fielded text into final report in a file
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: tofile.c,v $
* Revision 6.2  2003/03/27 18:23:42  kans
* increased 16000 character buffer to 24000 to handle long PubMed abstracts (e.g., pmid 12209194 technical report on toxicity studies)
*
* Revision 6.1  1998/06/12 20:05:48  kans
* fixed unix compiler warnings
*
* Revision 6.0  1997/08/25 18:07:51  madden
* Revision changed to 6.0
*
* Revision 5.2  1997/06/19 18:39:23  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.1  1996/10/17 19:54:10  kans
* fixed centering calculation
*
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 2.2  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <tofile.h>

typedef struct cellData {
  Uint2  start;
  Int2   count;
} CellData, PNTR CellPtr;

typedef struct tableData {
  CharPtr  text;
  CellPtr  cells;
  Int2     numCells;
  Int2     numRows;
  Int2     numCols;
  Boolean  openSpace;
  ColPtr   colFmt;
} TableData, PNTR TablePtr;

static ColData defaultTable = {0, 0, 0, 'l', TRUE, TRUE, TRUE};

/* ----- Function Prototypes ----- */

static Int2 GetNextBlock PROTO((CharPtr title, Int2 maxwid));
static void RecordCell PROTO((TablePtr tptr, Uint2 start, Int2 len, Int2 row, Int2 col));
static void SetTableFormat PROTO((ColPtr colFmt));
static void FormatTable PROTO((TablePtr tptr));
static Int2 SkipPastNewLine PROTO((CharPtr text, Int2 cnt));
static TablePtr TableSegment PROTO((CharPtr text, Int2 len, ParPtr parFmt, ColPtr colFmt));
static Boolean SaveTableToFile PROTO((Pointer dat, FILE *f));
static void FreeTable PROTO((TablePtr tptr));

/* ----- Function Bodies ----- */

static void SetTableFormat (ColPtr colFmt)

{
  Int2  i;

  colFmt [0].pos = 0;
  if (! colFmt [0].last) {
    i = 0;
    do {
      i++;
      colFmt [i].pos = colFmt [i - 1].pos + colFmt [i - 1].width;
    } while (! colFmt [i].last);
  }
}

static Int2 GetNextBlock (CharPtr title, Int2 maxwid)

{
  Char  ch;
  Int2  i;
  Int2  j;
  Int2  wid;

  i = 0;
  j = 0;
  wid = 0;
  if (title != NULL && maxwid > 0) {
    ch = title [i];
    while (ch != '\0' && ch != '\n' && ch != '\r' &&
           ch != '\t' && wid <= maxwid) {
      wid++;
      i++;
      ch = title [i];
    }
    j = i;
    if (wid > maxwid) {
      j--;
      while (i > maxwid) {
        i--;
      }
      while (i > 0 && title [i - 1] != ' ' && title [i - 1] != '-') {
        i--;
      }
      while (title [i] == ' ') {
        i++;
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

static void RecordCell (TablePtr tptr, Uint2 start, Int2 len, Int2 row, Int2 col)

{
  Int2     cell;
  CellPtr  cellPtr;
  Int2     chunk;
  Int2     newCells;
  Int2     numCells;
  Int2     numCols;

  if (tptr != NULL) {
    chunk = 128;
    numCells = tptr->numCells;
    numCols = tptr->numCols;
    if (col < numCols) {
      cell = (Int2) ((Int4) row * (Int4) numCols + (Int4) col);
      cellPtr = tptr->cells;
      if (cell >= numCells) {
        newCells = (cell / chunk + 1) * chunk;
        if (cellPtr != NULL) {
          cellPtr = MemMore (cellPtr, sizeof (CellData) * newCells);
          while (numCells < newCells) {
            cellPtr [numCells].start = 0;
            cellPtr [numCells].count = 0;
            numCells++;
          }
        } else {
          cellPtr = MemNew (sizeof (CellData) * newCells);
        }
        tptr->cells = cellPtr;
        tptr->numCells = newCells;
      }
      if (cellPtr != NULL) {
        cellPtr [cell].start = start;
        cellPtr [cell].count = len;
      }
    }
  }
}

static void FormatTable (TablePtr tptr)

{
  Int2       blklen;
  Char       ch;
  Int2       col;
  ColPtr     colFmt;
  Int2       insetLeft;
  Int2       insetRight;
  Char       just;
  Int2       len = 0;
  Int2       maxwid;
  Int2       numCols;
  Int2       returnRow;
  Int2       row;
  Uint2      start;
  Boolean    stripSpaces;
  Int2       tabRow;
  CharPtr    text;

  text = tptr->text;
  colFmt = tptr->colFmt;
  numCols = tptr->numCols;
  if (text != NULL) {
    start = 0;
    row = 0;
    tabRow = 0;
    returnRow = 1;
    col = 0;
    while (text [start] != '\0') {
      if (col < numCols) {
        stripSpaces = colFmt [col].strip;
      } else {
        stripSpaces = TRUE;
      }
      if (stripSpaces) {
        while (text [start] == ' ') {
          start++;
        }
      }
      ch = text [start];
      if (ch != '\0' && ch != '\n' && ch != '\r' && ch != '\t') {
        maxwid = INT2_MAX;
        just = colFmt [col].just;
        if (just == 'c') {
          insetLeft = colFmt [col].inset;
          insetRight = colFmt [col].inset;
        } else if (just == 'l') {
          insetLeft = colFmt [col].inset;
          insetRight = 0;
        } else if (just == 'r') {
          insetLeft = 0;
          insetRight = colFmt [col].inset;
        } else {
          insetLeft = 0;
          insetRight = 0;
        }
        if (col < numCols && colFmt [col].wrap && colFmt [col].width > 0 &&
            colFmt [col].width >= insetLeft + insetRight) {
          maxwid = colFmt [col].width - insetLeft - insetRight;
        }
        blklen = GetNextBlock (text + start, maxwid);
        len = blklen;
        if (len > 0) {
          if (text [start + len] != '\0') {
            while (len > 0 && text [start + len - 1] == ' ') {
              len--;
            }
          }
          RecordCell (tptr, start, len, row, col);
          if (len > 0) {
            if (colFmt [col].width == 0) {
              colFmt [col].width = len + 1;
            }
          }
          start += blklen;
        } else {
          while (len <= maxwid) {
            len++;
          }
          len--;
          if (len > 0) {
            RecordCell (tptr, start, len, row, col);
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
      } else if (ch != '\0' && colFmt [col].wrap) {
        if (len == 0) {
          start++;
        }
        row++;
        returnRow = MAX (returnRow, row + 1);
      } else if (ch != '\0') {
        start++;
      }
    }
    if (row > tptr->numRows) {
      tptr->numRows = row;
    }
  }
}

static TablePtr TableSegment (CharPtr text, Int2 len, ParPtr parFmt, ColPtr colFmt)

{
  Int2      i;
  TablePtr  tptr;
  CharPtr   txt;

  tptr = MemNew (sizeof (TableData));
  if (tptr != NULL) {
    txt = (CharPtr) MemNew (len + 1);
    MemCopy (txt, text, len);
    tptr->text = txt;
    tptr->cells = NULL;
    tptr->numCells = 0;
    tptr->numRows = 0;
    if (colFmt == NULL) {
      defaultTable.pos = 0;
      defaultTable.width = 80;
      colFmt = &defaultTable;
    }
    i = 0;
    while (! colFmt [i].last) {
      i++;
    }
    i++;
    tptr->numCols = i;
    tptr->colFmt = MemNew (i * sizeof (ColData));
    if (tptr->colFmt != NULL) {
      MemCopy (tptr->colFmt, colFmt, i * sizeof (ColData));
    }
    if (parFmt != NULL) {
      tptr->openSpace = parFmt->openSpace;
    } else {
      tptr->openSpace = TRUE;
    }
    SetTableFormat (colFmt);
    FormatTable (tptr);
  }
  return tptr;
}

static Boolean SaveTableToFile (Pointer dat, FILE *f)

{
  Int2      cell;
  CellPtr   cellPtr;
  Int2      col;
  ColPtr    colFmt;
  Int2      count;
  Int2      insetLeft;
  Int2      insetRight;
  Char      just;
  Int2      next;
  Int2      numCells;
  Int2      numCols;
  Int2      numRows;
  Int2      pos;
  Int2      row;
  Boolean   rsult;
  Uint2     start;
  CharPtr   text;
  TablePtr  tptr;

  rsult = TRUE;
  if (dat != NULL && f != NULL) {
    tptr = (TablePtr) dat;
    colFmt = tptr->colFmt;
    SetTableFormat (colFmt);
    text = tptr->text;
    cellPtr = tptr->cells;
    numCells = tptr->numCells;
    numCols = tptr->numCols;
    numRows = tptr->numRows;
    if (tptr->openSpace) {
      if (fputc ('\n', f) == EOF) {
        rsult = FALSE;
      }
    }
    for (row = 0; row < numRows; row++) {
      pos = 0;
      for (col = 0; col < numCols; col++) {
        cell = (Int2) ((Int4) row * (Int4) numCols + (Int4) col);
        if (cell < numCells) {
          if (text != NULL && cellPtr != NULL) {
            start = cellPtr [cell].start;
            count = cellPtr [cell].count;
            if (count > 0) {
              just = colFmt [col].just;
              if (just == 'c') {
                insetLeft = colFmt [col].inset;
                insetRight = colFmt [col].inset;
              } else if (just == 'l') {
                insetLeft = colFmt [col].inset;
                insetRight = 0;
              } else if (just == 'r') {
                insetLeft = 0;
                insetRight = colFmt [col].inset;
              } else {
                insetLeft = 0;
                insetRight = 0;
              }
              next = colFmt [col].pos + insetLeft;
              while (pos < next) {
                if (fputc (' ', f) == EOF) {
                  rsult = FALSE;
                }
                pos++;
              }
              if (just == 'r') {
                next = colFmt [col].pos + colFmt [col].width - insetRight - count;
                while (pos < next) {
                  if (fputc (' ', f) == EOF) {
                    rsult = FALSE;
                  }
                  pos++;
                }
              } else if (just == 'c') {
                next = colFmt [col].pos + (colFmt [col].width - insetRight - count) / 2;
                while (pos < next) {
                  if (fputc (' ', f) == EOF) {
                    rsult = FALSE;
                  }
                  pos++;
                }
              }
              while (count > 0) {
                if (fputc (text [start], f) == EOF) {
                  rsult = FALSE;
                }
                start++;
                count--;
                pos++;
              }
            }
          }
        }
      }
      if (fputc ('\n', f) == EOF) {
        rsult = FALSE;
      }
    }
  }
  return rsult;
}

static void FreeTable (TablePtr tptr)

{
  CellPtr  cellPtr;
  ColPtr   colFmt;
  CharPtr  text;

  if (tptr != NULL) {
    text = tptr->text;
    cellPtr = tptr->cells;
    colFmt = tptr->colFmt;
    tptr->text = MemFree (text);
    tptr->cells = MemFree (cellPtr);
    tptr->colFmt = MemFree (colFmt);
    MemFree (tptr);
  }
}

static Int2 SkipPastNewLine (CharPtr text, Int2 cnt)

{
  Char  ch;

  ch = *(text + cnt);
  while (ch != '\0' && ch != '\n' && cnt < 24300) {
    cnt++;
    ch = *(text + cnt);
  }
  while ((ch == '\n' || ch == '\r') && cnt < 24380) {
    cnt++;
    ch = *(text + cnt);
  }
  return cnt;
}

NLM_EXTERN Boolean SendTextToFile (FILE *f, CharPtr text, ParPtr parFmt, ColPtr colFmt)

{
  Int2      cnt;
  Int2      cntr;
  Boolean   rsult;
  Int2      start;
  TablePtr  tptr;

  rsult = TRUE;
  start = 0;
  cntr = StringLen (text);
  cnt = MIN (cntr, 24000);
  cnt = SkipPastNewLine (text + start, cnt);
  while (cnt > 0) {
    tptr = TableSegment (text + start, cnt, parFmt, colFmt);
    if (! SaveTableToFile (tptr, f)) {
      rsult = FALSE;
    }
    FreeTable (tptr);
    start += cnt;
    cntr -= cnt;
    cnt = MIN (cntr, 24000);
    cnt = SkipPastNewLine (text + start, cnt);
  }
  return rsult;
}
