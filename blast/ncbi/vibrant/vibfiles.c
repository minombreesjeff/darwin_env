/*   vibfiles.c
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
* File Name:  vibfiles.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   7/1/91
*
* $Revision: 6.3 $
*
* File Description: 
*       Vibrant file functions
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: vibfiles.c,v $
* Revision 6.3  2003/09/02 21:26:56  kans
* ReadChar and CloseString do not rely on feof, which does not work on Mac OS X
*
* Revision 6.2  2002/06/13 16:15:13  kans
* fix includes for OS_UNIX_DARWIN with WIN_MAC (EN) - still bug in vibutils.c file dialog
*
* Revision 6.1  1999/03/11 20:26:27  vakatov
* Get in-sync the printf's format and args in some functions
*
* Revision 5.1  1997/01/29 16:41:22  kans
* using StringNCpy_0
* ==========================================================================
*/

#include <vibtypes.h>
#include <vibprocs.h>
#include <vibincld.h>

#ifdef VAR_ARGS
#include <varargs.h>
#else
#include <stdarg.h>
#endif

Nlm_Boolean  Nlm_fileDone = TRUE;
Nlm_Int2     Nlm_fileError = 0;
Nlm_Char     Nlm_termCH = '\0';

static Nlm_Int2  charCount;

static void Nlm_DrawACharacter (Nlm_Char ch)

{
  Nlm_WindoW  w;

  w = Nlm_FrontWindow ();
  Nlm_DoSendChar ((Nlm_GraphiC) w, ch, TRUE);
}

static Nlm_Char Nlm_ReadCharFromTerm (void)

{
  Nlm_Char     ch;
/*#ifdef OS_MAC*/
#ifdef WIN_MAC 
  EventRecord  event;
#endif
#ifdef WIN_MSWIN
  MSG          msg;
  HWND         hwnd;
#endif

  ch = '\0';
/*#ifdef OS_MAC*/
#ifdef WIN_MAC
  while (ch == '\0') {
    if (GetNextEvent (keyDownMask + autoKeyMask, &event)) {
      ch = (Nlm_Char) (event.message % 256);
    }
  }
  Nlm_DrawACharacter (ch);
  if (ch == '\3') {
    ch = '\0';
  }
#endif
#ifdef WIN_MSWIN
  hwnd = GetFocus ();
  if (hwnd != NULL) {
    hwnd = GetParent (hwnd);
    if (hwnd != NULL) {
      SetFocus (hwnd);
    }
  }
  while (ch == '\0') {
    while (PeekMessage (&msg, NULL, WM_KEYDOWN, WM_KEYUP, PM_NOREMOVE)) {
      if (GetMessage (&msg, NULL, WM_KEYDOWN, WM_KEYUP)) {
        TranslateMessage (&msg);
        DispatchMessage (&msg);
      }
    }
    while (PeekMessage (&msg, NULL, WM_CHAR, WM_CHAR, PM_NOREMOVE)) {
      if (GetMessage (&msg, NULL, WM_CHAR, WM_CHAR)) {
        ch = (Nlm_Char) msg.wParam;
      }
    }
    if (ch == '\r') {
      ch = '\n';
    }
  }
  Nlm_DrawACharacter (ch);
#endif
#ifdef WIN_MOTIF
#endif
  return ch;
}

extern Nlm_Char Nlm_ReadChar (FILE *f)

{
  Nlm_Char  ch;
  int       getcrsult;

  ch = '\0';
  Nlm_fileDone = FALSE;
  if (f != NULL) {
    getcrsult = fgetc (f);
    ch = (Nlm_Char) getcrsult;
    if (getcrsult == EOF /* && feof (f) */) {
      ch = '\0';
    }
  } else {
    ch = Nlm_ReadCharFromTerm ();
  }
  Nlm_fileDone = (Nlm_Boolean) (ch != '\0');
  return ch;
}

extern void Nlm_WriteChar (FILE *f, Nlm_Char ch)

{
  if (f != NULL) {
    Nlm_fileError = fputc (ch, f);
    Nlm_fileDone = (Nlm_Boolean) (Nlm_fileError == 0 || ferror (f) == 0);
  } else {
    Nlm_DrawACharacter (ch);
    Nlm_fileError = FALSE;
    Nlm_fileDone = TRUE;
  }
}

extern void Nlm_WriteLn (FILE *f)

{
  Nlm_WriteChar (f, '\n');
}

static void Nlm_OpenString (FILE *f, Nlm_CharPtr str,
                            Nlm_Boolean ignore, size_t maxsize)

{
  Nlm_fileDone = FALSE;
  if (str != NULL && maxsize > 0) {
    str [0] = '\0';
  }
  charCount = 0;
  do {
    Nlm_termCH = Nlm_ReadChar (f);
  } while (Nlm_termCH == ' ' && ignore);
}

static void Nlm_InsertNextChar (FILE *f, Nlm_CharPtr str,
                                Nlm_Boolean ignore, size_t maxsize)

{
  do {
    if (charCount == 0 && ignore && Nlm_termCH == ' ') {
    } else if (Nlm_termCH == '\b') {
      if (charCount > 0) {
        charCount--;
      }
    } else if (charCount < (Nlm_Int2) maxsize && Nlm_termCH != '\0') {
      str [charCount] = Nlm_termCH;
      charCount++;
    }
    Nlm_termCH = Nlm_ReadChar (f);
  } while (Nlm_termCH == '\b' || (charCount == 0 && ignore && Nlm_termCH == ' '));
}

static void Nlm_CloseString (FILE *f, Nlm_CharPtr str, size_t maxsize)

{
  if (charCount <= (Nlm_Int2) maxsize) {
    str [charCount] = '\0';
  }
  Nlm_fileDone = (Nlm_Boolean) /* (! feof (f)) */ (Nlm_termCH != '\0');
}

extern void Nlm_ReadString (FILE *f, Nlm_CharPtr str, size_t maxsize)

{
  Nlm_fileDone = FALSE;
  if (str != NULL) {
    Nlm_OpenString (f, str, TRUE, maxsize);
    while (Nlm_termCH > ' ') {
      Nlm_InsertNextChar (f, str, TRUE, maxsize);
    }
    Nlm_CloseString (f, str, maxsize);
  }
}

extern void Nlm_ReadField (FILE *f, Nlm_CharPtr str, size_t maxsize)

{
  Nlm_fileDone = FALSE;
  if (str != NULL) {
    Nlm_OpenString (f, str, TRUE, maxsize);
    while (Nlm_termCH >= ' ') {
      Nlm_InsertNextChar (f, str, TRUE, maxsize);
    }
    Nlm_CloseString (f, str, maxsize);
  }
}

extern void Nlm_ReadLine (FILE *f, Nlm_CharPtr str, size_t maxsize)

{
  Nlm_fileDone = FALSE;
  if (str != NULL) {
    Nlm_OpenString (f, str, FALSE, maxsize);
    while (Nlm_termCH != '\n' && Nlm_termCH != '\r' && Nlm_termCH != '\0') {
      Nlm_InsertNextChar (f, str, FALSE, maxsize);
    }
    Nlm_CloseString (f, str, maxsize);
  }
}

extern void Nlm_WriteString (FILE *f, Nlm_CharPtr str)

{
  Nlm_Int2  i;

  Nlm_fileDone = FALSE;
  if (str != NULL) {
    i = 0;
    while (str [i] != '\0') {
      Nlm_WriteChar (f, str [i]);
      i++;
    }
  }
}

extern void Nlm_ReadText (FILE *f, Nlm_CharPtr str, size_t maxsize)

{
  Nlm_ReadLine (f, str, maxsize);
}

#ifdef VAR_ARGS
void CDECL Nlm_WriteText (f, format, va_alist)
  FILE *f;
  char *format;
  va_dcl
#else
void CDECL Nlm_WriteText (FILE *f, char *format, ...)
#endif

{
  va_list  args;
  char     buf[120];

#ifdef VAR_ARGS
  va_start(args);
#else
  va_start(args, format);
#endif
  vsprintf(buf, format, args);
  va_end(args);

  Nlm_WriteString (f, buf);
}

#ifdef VAR_ARGS
void CDECL Nlm_WriteLog (format, va_alist)
  char *format;
  va_dcl
#else
void CDECL Nlm_WriteLog (char *format, ...)
#endif

{
  va_list  args;
  char     buf[120];
  FILE     *f;

#ifdef VAR_ARGS
  va_start(args);
#else
  va_start(args, format);
#endif
  vsprintf(buf, format, args);
  va_end(args);

  f = Nlm_FileOpen ("LogFile", "a");
  if (f != NULL) {
    Nlm_WriteString (f, buf);
    Nlm_FileClose (f);
  }
}

extern Nlm_Uint2 Nlm_ReadCard (FILE *f)

{
  Nlm_Uint2  cardval;
  Nlm_Char   ioStr [256];

  Nlm_fileDone = FALSE;
  cardval = 0;
  Nlm_ReadString (f, ioStr, sizeof (ioStr));
  if (Nlm_fileDone) {
    Nlm_fileDone = Nlm_StrToCard (ioStr, &cardval);
  }
  return cardval;
}

extern void Nlm_WriteCard (FILE *f, Nlm_Uint2 cardval, Nlm_Int2 length)

{
  Nlm_Char  ioStr [256];

  Nlm_fileDone = FALSE;
  Nlm_CardToStr (cardval, ioStr, length, sizeof (ioStr));
  Nlm_WriteString (f, ioStr);
}

extern Nlm_Int2 Nlm_ReadInt (FILE *f)

{
  Nlm_Int2  intval;
  Nlm_Char  ioStr [256];

  Nlm_fileDone = FALSE;
  intval = 0;
  Nlm_ReadString (f, ioStr, sizeof (ioStr));
  if (Nlm_fileDone) {
    Nlm_fileDone = Nlm_StrToInt (ioStr, &intval);
  }
  return intval;
}

extern void Nlm_WriteInt (FILE *f, Nlm_Int2 intval, Nlm_Int2 length)

{
  Nlm_Char  ioStr [256];

  Nlm_fileDone = FALSE;
  Nlm_IntToStr (intval, ioStr, length, sizeof (ioStr));
  Nlm_WriteString (f, ioStr);
}

extern Nlm_Int4 Nlm_ReadLong (FILE *f)

{
  Nlm_Int4  longval;
  Nlm_Char  ioStr [256];

  Nlm_fileDone = FALSE;
  longval = 0;
  Nlm_ReadString (f, ioStr, sizeof (ioStr));
  if (Nlm_fileDone) {
    Nlm_fileDone = Nlm_StrToLong (ioStr, &longval);
  }
  return longval;
}

extern void Nlm_WriteLong (FILE *f, Nlm_Int4 longval, Nlm_Int2 length)

{
  Nlm_Char  ioStr [256];

  Nlm_fileDone = FALSE;
  Nlm_LongToStr (longval, ioStr, length, sizeof (ioStr));
  Nlm_WriteString (f, ioStr);
}

extern Nlm_FloatLo Nlm_ReadReal (FILE *f)

{
  Nlm_FloatHi  doubleval;
  Nlm_Char     ioStr [256];
  Nlm_FloatLo  realval;

  Nlm_fileDone = FALSE;
  doubleval = 0.0;
  Nlm_ReadString (f, ioStr, sizeof (ioStr));
  if (Nlm_fileDone) {
    Nlm_fileDone = Nlm_StrToDouble (ioStr, &doubleval);
  }
  realval = (Nlm_FloatLo) doubleval;
  return realval;
}

extern void Nlm_WriteReal (FILE *f, Nlm_FloatLo realval,
                           Nlm_Int2 length, Nlm_Int2 dec)

{
  Nlm_Char  ioStr [256];

  Nlm_fileDone = FALSE;
  Nlm_RealToStr (realval, ioStr, length, dec, sizeof (ioStr));
  Nlm_WriteString (f, ioStr);
}

extern Nlm_FloatHi Nlm_ReadDouble (FILE *f)

{
  Nlm_FloatHi  doubleval;
  Nlm_Char     ioStr [256];

  Nlm_fileDone = FALSE;
  doubleval = 0.0;
  Nlm_ReadString (f, ioStr, sizeof (ioStr));
  if (Nlm_fileDone) {
    Nlm_fileDone = Nlm_StrToDouble (ioStr, &doubleval);
  }
  return doubleval;
}

extern void Nlm_WriteDouble (FILE *f, Nlm_FloatHi doubleval,
                             Nlm_Int2 length, Nlm_Int2 dec)

{
  Nlm_Char  ioStr [256];

  Nlm_fileDone = FALSE;
  Nlm_DoubleToStr (doubleval, ioStr, length, dec, sizeof (ioStr));
  Nlm_WriteString (f, ioStr);
}

extern Nlm_Boolean Nlm_StrToCard (Nlm_CharPtr str, Nlm_Uint2Ptr cardval)

{
  Nlm_Char      ch;
  Nlm_Int2      i;
  Nlm_Int2      len;
  Nlm_Char      local [64];
  Nlm_Boolean   nodigits;
  Nlm_Boolean   rsult;
  unsigned int  val;

  rsult = FALSE;
  if (cardval != NULL) {
    *cardval = (Nlm_Uint2) 0;
  }
  len = (Nlm_Int2) Nlm_StringLen (str);
  if (len != 0) {
    rsult = TRUE;
    nodigits = TRUE;
    for (i = 0; i < len; i++) {
      ch = str [i];
      if (ch == ' ') {
      } else if (ch < '0' || ch > '9') {
        rsult = FALSE;
      } else {
        nodigits = FALSE;
      }
    }
    if (nodigits) {
      rsult = FALSE;
    }
    if (rsult && cardval != NULL) {
      Nlm_StringNCpy_0 (local, str, sizeof (local));
      if (sscanf (local, "%u", &val) == 1) {
        *cardval = (Nlm_Uint2) val;
      }
    }
  }
  return rsult;
}

extern Nlm_Boolean Nlm_StrToInt (Nlm_CharPtr str, Nlm_Int2Ptr intval)

{
  Nlm_Char     ch;
  Nlm_Int2     i;
  Nlm_Int2     len;
  Nlm_Char     local [64];
  Nlm_Boolean  nodigits;
  Nlm_Boolean  rsult;
  int          val;

  rsult = FALSE;
  if (intval != NULL) {
    *intval = (Nlm_Int2) 0;
  }
  len = (Nlm_Int2) Nlm_StringLen (str);
  if (len != 0) {
    rsult = TRUE;
    nodigits = TRUE;
    for (i = 0; i < len; i++) {
      ch = str [i];
      if (ch == ' ' || ch == '+' || ch == '-') {
      } else if (ch < '0' || ch > '9') {
        rsult = FALSE;
      } else {
        nodigits = FALSE;
      }
    }
    if (nodigits) {
      rsult = FALSE;
    }
    if (rsult && intval != NULL) {
      Nlm_StringNCpy_0 (local, str, sizeof (local));
      if (sscanf (local, "%d", &val) == 1) {
        *intval = (Nlm_Int2) val;
      }
    }
  }
  return rsult;
}

extern Nlm_Boolean Nlm_StrToLong (Nlm_CharPtr str, Nlm_Int4Ptr longval)

{
  Nlm_Char     ch;
  Nlm_Int2     i;
  Nlm_Int2     len;
  Nlm_Char     local [64];
  Nlm_Boolean  nodigits;
  Nlm_Boolean  rsult;
  long int     val;

  rsult = FALSE;
  if (longval != NULL) {
    *longval = (Nlm_Int4) 0;
  }
  len = (Nlm_Int2) Nlm_StringLen (str);
  if (len != 0) {
    rsult = TRUE;
    nodigits = TRUE;
    for (i = 0; i < len; i++) {
      ch = str [i];
      if (ch == ' ' || ch == '+' || ch == '-') {
      } else if (ch < '0' || ch > '9') {
        rsult = FALSE;
      } else {
        nodigits = FALSE;
      }
    }
    if (nodigits) {
      rsult = FALSE;
    }
    if (rsult && longval != NULL) {
      Nlm_StringNCpy_0 (local, str, sizeof (local));
      if (sscanf (local, "%ld", &val) == 1) {
        *longval = val;
      }
    }
  }
  return rsult;
}

extern Nlm_Boolean Nlm_StrToPtr (Nlm_CharPtr str, Nlm_VoidPtr PNTR ptrval)

{
  Nlm_Char     ch;
  Nlm_Int2     i;
  Nlm_Int2     len;
  Nlm_Char     local [64];
  Nlm_Boolean  nodigits;
  Nlm_Boolean  rsult;
  long         val;

  rsult = FALSE;
  if (ptrval != NULL) {
    *ptrval = NULL;
  }
  len = (Nlm_Int2) Nlm_StringLen (str);
  if (len != 0) {
    rsult = TRUE;
    nodigits = TRUE;
    for (i = 0; i < len; i++) {
      ch = str [i];
      if (ch == ' ') {
      } else if (ch < '0' || ch > '9') {
        rsult = FALSE;
      } else {
        nodigits = FALSE;
      }
    }
    if (nodigits) {
      rsult = FALSE;
    }
    if (rsult && ptrval != NULL) {
      Nlm_StringNCpy_0 (local, str, sizeof (local));
      if (sscanf (local, "%ld", &val) == 1) {
        *ptrval = (Nlm_VoidPtr) val;
      }
    }
  }
  return rsult;
}

extern Nlm_Boolean Nlm_StrToReal (Nlm_CharPtr str, Nlm_FloatLoPtr realval)

{
  Nlm_Char     ch;
  Nlm_Int2     i;
  Nlm_Int2     len;
  Nlm_Char     local [64];
  Nlm_Boolean  nodigits;
  Nlm_Boolean  rsult;
  float        val;

  rsult = FALSE;
  if (realval != NULL) {
    *realval = (Nlm_FloatLo) 0;
  }
  len = (Nlm_Int2) Nlm_StringLen (str);
  if (len != 0) {
    rsult = TRUE;
    nodigits = TRUE;
    for (i = 0; i < len; i++) {
      ch = str [i];
      if (ch == '+' || ch == '-' || ch == '.' || ch == 'E' || ch == 'e') {
      } else if (ch < '0' || ch > '9') {
        rsult = FALSE;
      } else {
        nodigits = FALSE;
      }
    }
    if (nodigits) {
      rsult = FALSE;
    }
    if (rsult && realval != NULL) {
      Nlm_StringNCpy_0 (local, str, sizeof (local));
      for (i = 0; i < len; i++) {
        local [i] = TO_UPPER (local [i]);
      }
      if (sscanf (local, "%f", &val) == 1) {
        *realval = val;
      }
    }
  }
  return rsult;
}

extern Nlm_Boolean Nlm_StrToDouble (Nlm_CharPtr str, Nlm_FloatHiPtr doubleval)

{
  Nlm_Char     ch;
  Nlm_Int2     i;
  Nlm_Int2     len;
  Nlm_Char     local [64];
  Nlm_Boolean  nodigits;
  Nlm_Boolean  rsult;
  double       val;

  rsult = FALSE;
  if (doubleval != NULL) {
    *doubleval = (Nlm_FloatHi) 0;
  }
  len = (Nlm_Int2) Nlm_StringLen (str);
  if (len != 0) {
    rsult = TRUE;
    nodigits = TRUE;
    for (i = 0; i < len; i++) {
      ch = str [i];
      if (ch == '+' || ch == '-' || ch == '.' || ch == 'E' || ch == 'e') {
      } else if (ch < '0' || ch > '9') {
        rsult = FALSE;
      } else {
        nodigits = FALSE;
      }
    }
    if (nodigits) {
      rsult = FALSE;
    }
    if (rsult && doubleval != NULL) {
      Nlm_StringNCpy_0 (local, str, sizeof (local));
      for (i = 0; i < len; i++) {
        local [i] = TO_UPPER (local [i]);
      }
      if (sscanf (local, "%lf", &val) == 1) {
        *doubleval = val;
      }
    }
  }
  return rsult;
}

extern void Nlm_CardToStr (Nlm_Uint2 cardval, Nlm_CharPtr str,
                           Nlm_Int2 length, size_t maxsize)
{
  Nlm_Char  temp [80];

  sprintf (temp, "%*u", length, (int)cardval);
  Nlm_StringNCpy_0 (str, temp, maxsize);
}

extern void Nlm_IntToStr (Nlm_Int2 intval, Nlm_CharPtr str,
                          Nlm_Int2 length, size_t maxsize)

{
  Nlm_Char  temp [80];

  sprintf (temp, "%*d", length, intval);
  Nlm_StringNCpy_0 (str, temp, maxsize);
}

extern void Nlm_LongToStr (Nlm_Int4 longval, Nlm_CharPtr str,
                           Nlm_Int2 length, size_t maxsize)

{
  Nlm_Char  temp [80];

  sprintf (temp, "%*ld", length, (long)longval);
  Nlm_StringNCpy_0 (str, temp, maxsize);
}

extern void Nlm_PtrToStr (Nlm_VoidPtr ptrval, Nlm_CharPtr str,
                          Nlm_Int2 length, size_t maxsize)

{
  Nlm_Char  temp [80];

  sprintf (temp, "%*ld", length, (long) ptrval);
  Nlm_StringNCpy_0 (str, temp, maxsize);
}

extern void Nlm_RealToStr (Nlm_FloatLo realval, Nlm_CharPtr str,
                           Nlm_Int2 length, Nlm_Int2 dec,
                           size_t maxsize)

{
  Nlm_Char  temp [80];

  sprintf (temp, "%*.*f", length, dec, (float)realval);
  Nlm_StringNCpy_0 (str, temp, maxsize);
}

extern void Nlm_DoubleToStr (Nlm_FloatHi doubleval, Nlm_CharPtr str,
                             Nlm_Int2 length, Nlm_Int2 dec,
                             size_t maxsize)

{
  Nlm_Char  temp [80];

  sprintf (temp, "%*.*lf", length, dec, (double)doubleval);
  Nlm_StringNCpy_0 (str, temp, maxsize);
}
