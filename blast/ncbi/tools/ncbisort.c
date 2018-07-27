static char const rcsid[] = "$Id: ncbisort.c,v 6.7 2003/07/15 20:17:39 coulouri Exp $";

/* $Id: ncbisort.c,v 6.7 2003/07/15 20:17:39 coulouri Exp $
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
* File Name:  $RCSfile: ncbisort.c,v $
*
* Author:  Sergei Shavirin
*
* Initial Version Creation Date: 03/24/1997
*
* $Revision: 6.7 $
*
* File Description:
*         Main file for SORTing library
*
* $Log: ncbisort.c,v $
* Revision 6.7  2003/07/15 20:17:39  coulouri
* remove signal() and setrlimit() calls
*
* Revision 6.6  2003/05/30 17:25:37  coulouri
* add rcsid
*
* Revision 6.5  2003/02/26 17:48:09  kimelman
* bugfix: format/argument mismatch in error message
*
* Revision 6.4  1998/06/01 14:52:37  madden
* Change to using TmpNam
*
* Revision 6.3  1998/04/09 20:40:45  kans
* use TmpNam for Mac sorting
*
* Revision 6.2  1998/04/09 18:54:23  kans
* on mac, \r must be converted to \n in buffer
*
* Revision 6.1  1997/12/02 18:57:38  shavirin
* Removed typecast warnings insprintf() and sscanf()
*
* Revision 6.0  1997/08/25 18:53:34  madden
* Revision changed to 6.0
*
* Revision 1.4  1997/05/05 14:36:57  shavirin
* Fixed tmp-prefix handling for non-UNIX platform
*
 * Revision 1.3  1997/05/01  20:16:21  shavirin
 * Added typecast to remove few warnings
 *
 * Revision 1.2  1997/05/01  17:22:31  shavirin
 * Added function SORTObjectFree()
 *
 * Revision 1.1  1997/03/13  21:52:15  shavirin
 * Initial revision
 *
*
* ==========================================================================
*/

#include <ncbisrti.h>

SORTObjectPtr SORTObjectNew(CharPtr prefix, Uchar tab, Int4 linelength,
                            Boolean reverse, Boolean unique)
{
  SORTDataPtr sdp;
  Int4 i;

  sdp = (SORTDataPtr) MemNew(sizeof(SORTData));
  sdp->temphead = (SORTempNodePtr) MemNew(sizeof(SORTempNode));
  global_temphead = sdp->temphead; /* Used for cleanup by signal */
  sdp->keyhead = (SORTKeyFieldPtr)MemNew(sizeof(SORTKeyField));
  
  if(!tables_initialized) {
    for (i = 0; i < UCHAR_LIM; ++i) {
      if (ISBLANK(i))
        blanks[i] = 1;
      if (IS_DIGIT(i))
        digits[i] = 1;
      if (IS_UPPER(i))
        fold_tolower[i] = tolower(i);
      else
        fold_tolower[i] = (Uint1) i;
    }
  }
    
  sdp->sortalloc = SORTALLOC;
  sdp->mergealloc = MERGEALLOC;

  if(linelength != 0)
    sdp->linelength = linelength;
  else
    sdp->linelength = 30;

  if(prefix != NULL) {
    sdp->prefix = StringSave(prefix);
  } else {
#ifndef OS_UNIX
    {{
      CharPtr tmp_dir = getenv ("TMP");
      
      if (!tmp_dir)
        sdp->prefix = StringSave(".");
      else 
        sdp->prefix = StringSave(tmp_dir);
    }}
#else /* not OS_UNIX */
    sdp->prefix = StringSave(PREFIX);
#endif
  }
  sdp->tab = tab;
  sdp->reverse  = reverse;
  sdp->unique = unique;

  return (SORTObjectPtr) sdp;
}

void SORTObjectFree(SORTObjectPtr object)
{
    SORTDataPtr sdp;
    SORTempNodePtr tnp_next, tnp_tmp;
    SORTKeyFieldPtr kfp_next, kfp_tmp;

    if((sdp = (SORTDataPtr) object) == NULL)
        return;

    for(tnp_tmp = sdp->temphead; tnp_tmp != NULL; tnp_tmp = tnp_next) {
        tnp_next = tnp_tmp->next;
        MemFree(tnp_tmp->name);
        MemFree(tnp_tmp);
    }

    for(kfp_tmp = sdp->keyhead; kfp_tmp != NULL; kfp_tmp = kfp_next) {
        kfp_next = kfp_tmp->next;
        MemFree(kfp_tmp->ignore);
        MemFree(kfp_tmp->translate);
        MemFree(kfp_tmp);
    }

    MemFree(sdp->prefix);
    MemFree(sdp);
}

SORTKeyFieldPtr SORTGetKeyHead(SORTObjectPtr sop)
{
  SORTDataPtr sdp;
  
  if((sdp = (SORTDataPtr) sop) == NULL)
    return NULL;

  return sdp->keyhead;
}
SORTErrorCode SORTSetReverse(Boolean reverse, SORTObjectPtr sop) 
{
  SORTDataPtr sdp;
  
  if((sdp = (SORTDataPtr) sop) == NULL)
    return SORTBadParameter;
  sdp->reverse  = reverse;
  return SORTNoError;
}
SORTErrorCode SORTSetUnique(Boolean unique, SORTObjectPtr sop) 
{
  SORTDataPtr sdp;
  
  if((sdp = (SORTDataPtr) sop) == NULL)
    return SORTBadParameter;

  sdp->unique = unique;
  return SORTNoError;
}
SORTErrorCode SORTSetLineLength(Int4 linelength, SORTObjectPtr sop) 
{
  SORTDataPtr sdp;
  
  if((sdp = (SORTDataPtr) sop) == NULL)
    return SORTBadParameter;

  if(linelength != 0)
    sdp->linelength = linelength;
  else
    sdp->linelength = 30;
  
  return SORTNoError;
}

SORTErrorCode SORTSetTab(Uchar tab, SORTObjectPtr sop) 
{
  SORTDataPtr sdp;
  
  if((sdp = (SORTDataPtr) sop) == NULL)
    return SORTBadParameter;

  sdp->tab = tab;
  
  return SORTNoError;
}
SORTErrorCode SORTSetPrefix(CharPtr prefix, SORTObjectPtr sop) 
{
  SORTDataPtr sdp;
  
  if((sdp = (SORTDataPtr) sop) == NULL)
    return SORTBadParameter;
  
  if(prefix != NULL) {
    if(sdp->prefix != NULL)
      MemFree(sdp->prefix);
    sdp->prefix = StringSave(prefix);
  } else {
    sdp->prefix = NULL;
  }

  return SORTNoError;
}

/* Sort any number of FILES onto the given OFP. */
SORTErrorCode SORTFiles(CharPtr PNTR files, Int4 nfiles, FILE *ofp, 
               SORTObjectPtr sop)
{
  SORTBuffer   buf;
  SORTLines   lines;
  SORTLinePtr tmp;
  Int4 i, ntmp;
  FILE *fp, *tfp;
  SORTempNodePtr node;
  SORTDataPtr sdp;
  Int4 ntemp = 0;
  CharPtr PNTR tempfiles;
  CharPtr temp;
  SORTErrorCode error_code = SORTNoError;
  
  if((sdp = (SORTDataPtr) sop) == NULL) {
    error_code = SORTBadParameter;
    goto return_from_function;
  }
  
  SORTInitBuf(&buf, sdp->sortalloc);
  SORTInitLines(&lines, sdp->sortalloc/sdp->linelength + 1);
  ntmp = lines.alloc;

  if((tmp = (SORTLinePtr) MemNew(ntmp * sizeof (SORTLine))) == NULL) {
    error_code = SORTNoMemory;
    goto return_from_function;
  }
  
  while (nfiles--) {
    if((fp = FileOpen(*files++, "r")) == NULL) {
      error_code = SORTBadFileName;
      goto return_from_function;
    }
    
    while (SORTFillBuf(&buf, fp)) {
      SORTFindLines(&buf, &lines, sdp);
      if (lines.used > ntmp) {
        while (lines.used > ntmp)
          ntmp *= 2;
        
        if((tmp = (SORTLinePtr) Realloc(tmp, ntmp*sizeof(SORTLine))) == NULL) {
          error_code = SORTNoMemory;
          goto return_from_function;
        }
      }
      SORTArrayLines(lines.lines, lines.used, tmp, sdp);

      if (feof(fp) && !nfiles && !ntemp) {
        tfp = ofp;
      } else {
        ++ntemp;

        if((temp =SORTAddTempName(sop)) == NULL) {
          error_code = SORTMiscError;
          goto return_from_function;
        }
        if((tfp = FileOpen(temp, "w")) == NULL) {
          error_code = SORTBadFileName;
          goto return_from_function;
        }
      }
      for (i = 0; i < lines.used; ++i)
        if (!sdp->unique || i == lines.used - 1
            || SORTCompare(&lines.lines[i], &lines.lines[i + 1], sdp)) {
          
          if((FileWrite(lines.lines[i].text, 1, lines.lines[i].length, tfp)) !=
             (Uint4) lines.lines[i].length) {
            error_code = SORTWriteError;
            goto return_from_function;
          }
          putc('\n', tfp);
        }
      if (tfp != ofp)
        FileClose(tfp);
    }
    FileClose(fp);
  }
  
  MemFree(buf.buf);
  MemFree((CharPtr) lines.lines);
  MemFree((CharPtr) tmp);
  
  if (ntemp) {
    if((tempfiles = (CharPtr PNTR) MemNew(ntemp * sizeof (CharPtr))) == NULL) {
      error_code = SORTNoMemory;
      goto return_from_function;
    }
    
    i = ntemp;
    for (node = sdp->temphead->next; node; node = node->next)
      tempfiles[--i] = node->name;
    SORTMergeFiles(tempfiles, ntemp, ofp, sop);
    MemFree((CharPtr) tempfiles);
  }
  
 return_from_function:
  SORTCleanup(sdp->temphead);
  return error_code;
}

/* Merge any number of FILES onto the given OFP. */
SORTErrorCode SORTMergeFiles(CharPtr files[], Int4 nfiles, FILE *ofp, 
                SORTObjectPtr sop)
{
  Int4 i, j, t;
  CharPtr temp;
  FILE *fps[NMERGE], *tfp;
  SORTDataPtr sdp;
  SORTErrorCode error_code = SORTNoError;

  if((sdp = (SORTDataPtr) sop) == NULL) {
    error_code = SORTBadParameter;
    goto return_from_function;
  }
  
  while (nfiles > NMERGE) {
    t = 0;
    for (i = 0; i < nfiles / NMERGE; ++i) {
      for (j = 0; j < NMERGE; ++j) {
        if((fps[j] = FileOpen(files[i * NMERGE + j], "r")) == NULL) {
          error_code = SORTBadFileName;
          goto return_from_function;
        }
      }
      if((temp = SORTAddTempName(sop)) == NULL) {
        error_code = SORTMiscError;
        goto return_from_function;
      }
      if((tfp = FileOpen(temp, "w")) == NULL) {
        error_code = SORTBadFileName;
        goto return_from_function;
      }

      SORTMergeFPS(fps, NMERGE, tfp, sdp);

      FileClose(tfp);

      for (j = 0; j < NMERGE; ++j)
        SORTDelTempName(files[i * NMERGE + j], sdp->temphead);

      files[t++] = temp;
    }
    for (j = 0; j < nfiles % NMERGE; ++j)
      if((fps[j] = FileOpen(files[i * NMERGE + j], "r")) == NULL) {
        error_code = SORTBadFileName;
        goto return_from_function;
      }
    if((temp = SORTAddTempName(sop)) == NULL) {
      error_code = SORTMiscError;
      goto return_from_function;
    }
    
    if((tfp = FileOpen(temp, "w")) == NULL) {
      error_code = SORTBadFileName;
      goto return_from_function;
    }

    SORTMergeFPS(fps, nfiles % NMERGE, tfp, sdp);
    FileClose(tfp);

    for (j = 0; j < nfiles % NMERGE; ++j)
      SORTDelTempName(files[i * NMERGE + j], sdp->temphead);
    files[t++] = temp;
    nfiles = t;
  }
  
  for (i = 0; i < nfiles; ++i) {
    if((fps[i] = FileOpen(files[i], "r")) == NULL) {
      error_code = SORTBadFileName;
      goto return_from_function;
    }
  }
  
  SORTMergeFPS(fps, i, ofp, sdp);

  for (i = 0; i < nfiles; ++i)
    SORTDelTempName(files[i], sdp->temphead);

 return_from_function:
  SORTCleanup(sdp->temphead);
  return error_code;
}

/* Clean up any remaining temporary files. */

static void SORTCleanup(SORTempNodePtr temphead)
{
  SORTempNodePtr node;
  
  for (node = temphead->next; node; node = node->next)
    FileRemove(node->name); 
  
  temphead->next = NULL;
}

/* Return a name for a temporary file. */
CharPtr SORTAddTempName(SORTObjectPtr sop)
{
  Char path [PATH_MAX];
  CharPtr name;
  SORTempNodePtr node;
  SORTDataPtr sdp;

  if((sdp = (SORTDataPtr) sop) == NULL)
    return NULL;


  TmpNam (path);
  name = StringSave (path);

  if((node = (SORTempNodePtr)MemNew(sizeof(SORTempNode))) == NULL) {
    SORTCleanup(sdp->temphead);
    return NULL;
  }

  node->name = name;
  node->next = sdp->temphead->next;
  sdp->temphead->next = node;
  
  return name;
}

/* Search through the list of temporary files for the given name;
   remove it if it is found on the list. */
static void SORTDelTempName(CharPtr name, SORTempNodePtr temphead)
{
  SORTempNodePtr node, temp;
  
  for (node = temphead; node->next; node = node->next) {
    if (!StringCmp(name, node->next->name))
      break;
  }
  
  if (node->next) {
    temp = node->next;
    FileRemove(temp->name);
    MemFree(temp->name);
    node->next = temp->next;
    MemFree((CharPtr) temp);
  }
}

/* Initialize BUF allocating ALLOC bytes initially. */

static SORTErrorCode SORTInitBuf (SORTBufferPtr buf, Int4 alloc)
{
  buf->alloc = alloc;
  if((buf->buf = (UcharPtr)MemNew(buf->alloc)) == NULL)
    return SORTNoMemory;
  buf->used = buf->left = 0;
  return SORTNoError;
}

/* Fill BUF reading from FP, moving buf->left bytes from the end
   of buf->buf to the beginning first.	If EOF is reached and the
   file wasn't terminated by a newline, supply one.  Return a count
   of bytes actually read. */

static Int4 SORTFillBuf(SORTBufferPtr buf, FILE *fp)
{
  Int4 cc, total = 0;

  MemMove(buf->buf, buf->buf + buf->used - buf->left, buf->left);
  buf->used = buf->left;

  while (!feof(fp) && !Nlm_MemChr(buf->buf, '\n', buf->used)) {
    if (buf->used == buf->alloc) {
      if((buf->buf = (UcharPtr) 
          Realloc(buf->buf, buf->alloc *= 2)) == NULL) {
        return (Int4) SORTNoMemory;
      }
    }

    if((cc = FileRead(buf->buf + buf->used, 
                      1, buf->alloc - buf->used, fp)) == 0 && ferror(fp)) {
      ErrLogPrintf("sort: read error (%s)\n",  strerror(errno));
      return (Int4) SORTReadError;
    }

#ifdef WIN_MAC
    {
      Int4      ct;
      UcharPtr  ptr;

      ptr = buf->buf + buf->used;
      ct = cc;
      while (ct > 0) {
        if (*ptr == '\r') {
          *ptr = '\n';
        }
        ptr++;
        ct--;
      }
    }
#endif

    buf->used += cc;
    total += cc;
  }
  
  if (feof(fp) && buf->used && buf->buf[buf->used - 1] != '\n') {
    if (buf->used == buf->alloc) {
      if((buf->buf = (UcharPtr) Realloc(buf->buf, buf->alloc *= 2)) == NULL) {
        return (Int4) SORTNoMemory;
      }
    }
    buf->buf[buf->used++] = '\n';
    ++total;
  }
  return total;
}

/* Initialize LINES, allocating space for ALLOC lines initially. */
static SORTErrorCode SORTInitLines(SORTLinesPtr lines, Int4 alloc)
{
  lines->alloc = alloc;
  if((lines->lines = 
      (SORTLinePtr)MemNew(lines->alloc * sizeof(SORTLine))) == NULL) 
    return SORTNoMemory;
  lines->used = 0;
  return SORTNoError;
}

/* Return a pointer to the first character of a field. */
static UcharPtr SORTBegField(SORTLinePtr line, SORTKeyFieldPtr key, 
                             SORTDataPtr sdp)
{
  register UcharPtr ptr = line->text, lim = ptr + line->length;
  register Int4 sword = key->sword, schar = key->schar;

  if (sdp->tab)
    while (ptr < lim && sword--) {
      while (ptr < lim && *ptr != sdp->tab)
        ++ptr;
      if (ptr < lim)
        ++ptr;
    } else
      while (ptr < lim && sword--) {
	while (ptr < lim && blanks[UCHAR(*ptr)])
	  ++ptr;
	while (ptr < lim && !blanks[UCHAR(*ptr)])
	  ++ptr;
      }
  
  if (key->skipsblanks)
    while (ptr < lim && blanks[UCHAR(*ptr)])
      ++ptr;
  
  while (ptr < lim && schar--)
    ++ptr;
  
  return ptr;
}

/* Find the limit of a field; i.e., a pointer to the first character
   after the field. */
static UcharPtr SORTLimField(SORTLinePtr line, SORTKeyFieldPtr key, 
                             SORTDataPtr sdp)
{
  register UcharPtr ptr = line->text, lim = ptr + line->length;
  register Int4 eword = key->eword, echar = key->echar;
  
  if (sdp->tab)
    while (ptr < lim && eword--) {
      while (ptr < lim && *ptr != sdp->tab)
        ++ptr;
      if (ptr < lim && (eword || key->skipeblanks))
        ++ptr;
    } else
      while (ptr < lim && eword--) {
	while (ptr < lim && blanks[UCHAR(*ptr)])
	  ++ptr;
	while (ptr < lim && !blanks[UCHAR(*ptr)])
	  ++ptr;
      }
  
  if (key->skipeblanks)
    while (ptr < lim && blanks[UCHAR(*ptr)])
      ++ptr;
  
  while (ptr < lim && echar--)
    ++ptr;
  
  return ptr;
}

/* Find the lines in BUF, storing pointers and lengths in LINES.
   Also replace newlines with NULs. */
static SORTErrorCode SORTFindLines(SORTBufferPtr buf, SORTLinesPtr lines, 
                          SORTDataPtr sdp)
{
  register UcharPtr beg, lim, ptr;
  SORTKeyFieldPtr key;

  beg = buf->buf;
  lim = buf->buf + buf->used;
  key = sdp->keyhead->next;
  
  lines->used = 0;
  
  while (beg < lim &&
         ((ptr = (UcharPtr)Nlm_MemChr(beg, '\n', lim - beg)) != NULL)) {
    /* There are various places in the code that rely on a NUL
       being at the end of in-core lines; NULs inside the lines
       will not cause trouble, though. */
    *ptr = '\0';
    
    if (lines->used == lines->alloc)
      if((lines->lines = (SORTLinePtr) Realloc((CharPtr)lines->lines,
                                               (lines->alloc *= 2) * 
                                               sizeof(SORTLine))) == NULL) 
        return SORTNoMemory;
    
    lines->lines[lines->used].text = beg;
    lines->lines[lines->used].length = ptr - beg;
    
    /* Precompute the position of the first key for efficiency. */
    if (key) {
      if (key->eword >= 0)
        lines->lines[lines->used].keylim =
          SORTLimField(&lines->lines[lines->used], key, sdp);
      else
        lines->lines[lines->used].keylim = ptr;
      
      if (key->sword >= 0)
        lines->lines[lines->used].keybeg =
          SORTBegField(&lines->lines[lines->used], key, sdp);
      else {
        if (key->skipsblanks)
          while (blanks[UCHAR(*beg)])
            ++beg;
        lines->lines[lines->used].keybeg = beg;
      }
    }
    
    ++lines->used;
    beg = ptr + 1;
  }
  
  buf->left = lim - beg;
  return SORTNoError;
}

/* Compare two strings containing decimal fractions < 1.  Each string
   should begin with a decimal point followed immediately by the digits
   of the fraction.  Strings not of this form are considered to be zero. */
static Int4 SORTFracCompare(register UcharPtr a, register UcharPtr b)
{
  register tmpa = UCHAR(*a), tmpb = UCHAR(*b);

  if (tmpa == '.' && tmpb == '.') {
    do
      tmpa = UCHAR(*++a), tmpb = UCHAR(*++b);
    while (tmpa == tmpb && digits[tmpa]);
    if (digits[tmpa] && digits[tmpb])
      return tmpa - tmpb;
    if (digits[tmpa]) {
      while (tmpa == '0')
        tmpa = UCHAR(*++a);
      if (digits[tmpa])
        return 1;
      return 0;
    }
    if (digits[tmpb]) {
      while (tmpb == '0')
        tmpb = UCHAR(*++b);
      if (digits[tmpb])
        return -1;
      return 0;
    }
    return 0;
  } else if (tmpa == '.') {
    do
      tmpa = UCHAR(*++a);
    while (tmpa == '0');
    if (digits[tmpa])
      return 1;
    return 0;
  } else if (tmpb == '.') {
    do
      tmpb = UCHAR(*++b);
    while (tmpb == '0');
    if (digits[tmpb])
      return -1;
    return 0;
  }
  return 0;
}

/* Compare two strings as numbers without explicitly converting them to
   machine numbers.  Comparatively slow for short strings, but asymptotically
   hideously fast. */
static Int4 SORTNumCompare(register UcharPtr a, register UcharPtr b)
{
  register Int4 tmpa, tmpb, loga, logb, tmp;
  
  tmpa = UCHAR(*a), tmpb = UCHAR(*b);
  
  if (tmpa == '-') {
    tmpa = UCHAR(*++a);
    if (tmpb != '-') {
      if (digits[tmpa] && digits[tmpb])
        return -1;
      return 0;
    }
    tmpb = UCHAR(*++b);
    
    while (tmpa == '0')
      tmpa = UCHAR(*++a);
    while (tmpb == '0')
      tmpb = UCHAR(*++b);
    
    while (tmpa == tmpb && digits[tmpa])
      tmpa = UCHAR(*++a), tmpb = UCHAR(*++b);
    
    if ((tmpa == '.' && !digits[tmpb]) || (tmpb == '.' && !digits[tmpa]))
      return -SORTFracCompare(a, b);
    
    if (digits[tmpa])
      for (loga = 1; digits[UCHAR(*++a)]; ++loga)
        continue;
    else
      loga = 0;

    if (digits[tmpb])
      for (logb = 1; digits[UCHAR(*++b)]; ++logb)
        continue;
    else
      logb = 0;
    
    if ((tmp = logb - loga) != 0)
      return tmp;
    
    if (! loga)
      return 0;
    
    return tmpb - tmpa;
  } else if (tmpb == '-') {
    if (digits[UCHAR(tmpa)] && digits[UCHAR(*++b)])
      return 1;
    return 0;
  } else {
    while (tmpa == '0')
      tmpa = UCHAR(*++a);
    while (tmpb == '0')
      tmpb = UCHAR(*++b);
    
    while (tmpa == tmpb && digits[tmpa])
      tmpa = UCHAR(*++a), tmpb = UCHAR(*++b);
    
    if ((tmpa == '.' && !digits[tmpb]) || (tmpb == '.' && !digits[tmpa]))
      return SORTFracCompare(a, b);
    
    if (digits[tmpa])
      for (loga = 1; digits[UCHAR(*++a)]; ++loga)
        continue;
    else
      loga = 0;
    
    if (digits[tmpb])
      for (logb = 1; digits[UCHAR(*++b)]; ++logb)
        continue;
    else
      logb = 0;
    
    if ((tmp = loga - logb) != 0)
      return tmp;
    
    if (! loga)
      return 0;
    
    return tmpa - tmpb;
  }
}

/* Return an integer <= 12 associated with a month name (0 if the name
   is not recognized. */
static Int4 SORTGetMonth(UcharPtr s, Int4 len)
{
  Char month[4];
  register Int4 i, lo = 0, hi = 12;

  if (len < 3)
    return 0;
  
  for (i = 0; i < 3; ++i)
    month[i] = fold_tolower[UCHAR(s[i])];
  month[3] = '\0';
  
  while (hi - lo > 1)
    if (StringCmp(month, monthtab[(lo + hi) / 2].name) < 0)
      hi = (lo + hi) / 2;
    else
      lo = (lo + hi) / 2;
  if (!StringCmp(month, monthtab[lo].name))
    return monthtab[lo].val;
  return 0;
}

/* Compare two lines trying every key in sequence until there
   are no more keys or a difference is found. */
static Int4 SORTKeyCompare(SORTLinePtr a, SORTLinePtr b, SORTDataPtr sdp)
{
  register UcharPtr texta, textb, lima, limb, translate;
  register Int4Ptr ignore;
  SORTKeyFieldPtr key;
  Int4 diff = 0, iter = 0, lena, lenb;
  
  for (key = sdp->keyhead->next; key; key = key->next, ++iter) {
    ignore = key->ignore;
    translate = key->translate;
    
    /* Find the beginning and limit of each field. */
    if (iter) {
      if (key->eword >= 0)
        lima = SORTLimField(a, key, sdp), limb = SORTLimField(b, key, sdp);
      else
        lima = a->text + a->length, limb = b->text + b->length;
      
      if (key->sword >= 0)
        texta = SORTBegField(a, key, sdp), textb = SORTBegField(b, key, sdp);
      else {
        texta = a->text, textb = b->text;
        if (key->skipsblanks) {
          while (texta < lima && blanks[UCHAR(*texta)])
            ++texta;
          while (textb < limb && blanks[UCHAR(*textb)])
            ++textb;
        }
      }
    } else {
      /* For the first iteration only, the key positions have
         been precomputed for us. */
      texta = a->keybeg, lima = a->keylim;
      textb = b->keybeg, limb = b->keylim;
    }
    
    /* Find the lengths. */
    lena = lima - texta, lenb = limb - textb;
    if (lena < 0)
      lena = 0;
    if (lenb < 0)
      lenb = 0;
    
    /* Actually compare the fields. */
    if (key->numeric) {
      if (*lima || *limb) {
        Uchar savea = *lima, saveb = *limb;

        *lima = *limb = '\0';
        diff = SORTNumCompare(texta, textb);
        *lima = savea, *limb = saveb;
      } else
        diff = SORTNumCompare(texta, textb);
      
      if (diff)
        return key->reverse ? -diff : diff;
      continue;
    } else if (key->month) {
      diff = SORTGetMonth(texta, lena) - SORTGetMonth(textb, lenb);
      if (diff)
        return key->reverse ? -diff : diff;
      continue;
    } else if (ignore && translate)
      while (texta < lima && textb < limb) {
        while (texta < lima && ignore[UCHAR(*texta)])
          ++texta;
        while (textb < limb && ignore[UCHAR(*textb)])
          ++textb;
        if (texta < lima && textb < limb &&
            translate[UCHAR(*texta++)] != translate[UCHAR(*textb++)]) {
          diff = translate[UCHAR(*--texta)] - translate[UCHAR(*--textb)];
          break;
        }
      } else if (ignore)
	while (texta < lima && textb < limb) {
          while (texta < lima && ignore[UCHAR(*texta)])
            ++texta;
          while (textb < limb && ignore[UCHAR(*textb)])
            ++textb;
          if (texta < lima && textb < limb && *texta++ != *textb++) {
            diff = *--texta - *--textb;
            break;
          }
        } else if (translate)
          while (texta < lima && textb < limb) {
	    if (translate[UCHAR(*texta++)] != translate[UCHAR(*textb++)]) {
              diff = translate[UCHAR(*--texta)] - translate[UCHAR(*--textb)];
              break;
            }
	  } else
            diff = MemCmp(texta, textb, MIN(lena, lenb));

    if (diff)
      return key->reverse ? -diff : diff;
    if ((diff = lena - lenb) != 0)
      return key->reverse ? -diff : diff;
  }
  return 0;
}

/* Compare two lines, returning negative, zero, or positive depending on
   whether A compares less than, equal to, or greater than B. */
static Int4 SORTCompare(register SORTLinePtr a, register SORTLinePtr b, 
                    SORTDataPtr sdp)
{
  Int4 diff, tmpa, tmpb, min;
  
  if (sdp->keyhead->next) {
    if ((diff = SORTKeyCompare(a, b, sdp)) != 0)
      return diff;
    if (!sdp->unique) {
      tmpa = a->length, tmpb = b->length;
      diff = MemCmp(a->text, b->text, MIN(tmpa, tmpb));
      if (! diff)
        diff = tmpa - tmpb;
    }
  } else {
    tmpa = a->length, tmpb = b->length;
    min = MIN(tmpa, tmpb);
    if (min == 0)
      diff = tmpa - tmpb;
    else {
      UcharPtr ap = a->text;
      UcharPtr bp = b->text;
      
      diff = *ap - *bp;
      if (diff == 0) {
        diff = MemCmp(ap, bp, min);
        if (diff == 0)
          diff = tmpa - tmpb;
      }
    }
  }
  return sdp->reverse ? -diff : diff;
}

/* Check that the lines read from the given FP come in order.  Return
   1 if they do and 0 if there is a disorder. */

static Int4 SORTCheckOrder(FILE *fp, SORTDataPtr sdp)
{
  SORTBuffer  buf;              /* Input buffer. */
  SORTLines lines;		/* Lines scanned from the buffer. */
  SORTLine  temp;		/* Copy of previous line. */
  Int4 cc;			/* Character count. */
  Int4 cmp;			/* Result of calling compare. */
  Int4 alloc, i, success = 1;

  SORTInitBuf(&buf, sdp->mergealloc);
  SORTInitLines(&lines, sdp->mergealloc/sdp->linelength + 1);
  alloc = sdp->linelength;
  if((temp.text = (UcharPtr)MemNew(alloc)) == NULL)
    return -1;

  cc = SORTFillBuf(&buf, fp);
  SORTFindLines(&buf, &lines, sdp);
  
  if (cc)
    do {
      /* Compare each line in the buffer with its successor. */
      for (i = 0; i < lines.used - 1; ++i) {
        cmp = SORTCompare(&lines.lines[i], &lines.lines[i + 1], sdp);
        if ((sdp->unique && cmp >= 0) || cmp > 0) {
          success = 0;
          goto finish;
        }
      }
      
      /* Save the last line of the buffer and refill the buffer. */
      if (lines.lines[lines.used - 1].length > alloc) {
        while (lines.lines[lines.used - 1].length + 1 > alloc)
          alloc *= 2;
        if((temp.text = (UcharPtr) Realloc(temp.text, alloc)) == NULL)
          return -1;
      }
      MemCpy((CharPtr)temp.text, (CharPtr)lines.lines[lines.used - 1].text,
             lines.lines[lines.used - 1].length + 1);
      temp.length = lines.lines[lines.used - 1].length;
      
      cc = SORTFillBuf(&buf, fp);
      if (cc) {
        SORTFindLines(&buf, &lines, sdp);
        /* Make sure the line saved from the old buffer contents is
           less than or equal to the first line of the new buffer. */
        cmp = SORTCompare(&temp, &lines.lines[0], sdp);
        if ((sdp->unique && cmp >= 0) || cmp > 0) {
          success = 0;
          break;
        }
      }
    }
    while (cc);

 finish:
  FileClose(fp);
  MemFree(buf.buf);
  MemFree((CharPtr) lines.lines);
  MemFree(temp.text);
  return success;
}

/* Merge lines from FPS onto OFP.  NFPS cannot be greater than NMERGE.
   Close FPS before returning. */

static Int4 SORTMergeFPS(FILE *fps[], register Int4 nfps, 
                         FILE *ofp, SORTDataPtr sdp)
{
  SORTBuffer buffer[NMERGE]; /* Input buffers for each file. */
  SORTLines  lines[NMERGE];	/* Line tables for each buffer. */
  SORTLine   saved;		/* Saved line for unique check. */
  Int4 savedflag = 0;		/* True if there is a saved line. */
  Int4 savealloc;		/* Size allocated for the saved line. */
  Int4 cur[NMERGE];		/* Current line in each line table. */
  Int4 ord[NMERGE];		/* Table representing a permutation of fps,
				   such that lines[ord[0]].lines[cur[ord[0]]]
				   is the smallest line and will be next
				   output. */
  register Int4 i, j, t;

  /* Allocate space for a saved line if necessary. */
  if (sdp->unique) {
    if((saved.text = (UcharPtr)MemNew(savealloc = sdp->linelength)) == NULL)
      return -1;
  }

  /* Read initial lines from each input file. */
  for (i = 0; i < nfps; ++i) {
    SORTInitBuf(&buffer[i], sdp->mergealloc);
    /* If a file is empty, eliminate it from future consideration. */
    while (i < nfps && !SORTFillBuf(&buffer[i], fps[i])) {
      FileClose(fps[i]);
      --nfps;
      for (j = i; j < nfps; ++j)
        fps[j] = fps[j + 1];
    }
    if (i == nfps)
      MemFree(buffer[i].buf);
    else {
      SORTInitLines(&lines[i], sdp->mergealloc/sdp->linelength + 1);
      SORTFindLines(&buffer[i], &lines[i], sdp);
      cur[i] = 0;
    }
  }
  
  /* Set up the ord table according to comparisons among input lines.
     Since this only reorders two items if one is strictly greater than
     the other, it is stable. */
  for (i = 0; i < nfps; ++i)
    ord[i] = i;
  for (i = 1; i < nfps; ++i)
    if (SORTCompare(&lines[ord[i - 1]].lines[cur[ord[i - 1]]],
                    &lines[ord[i]].lines[cur[ord[i]]], sdp) > 0)
      t = ord[i - 1], ord[i - 1] = ord[i], ord[i] = t, i = 0;
  
  /* Repeatedly output the smallest line until no input remains. */
  while (nfps) {
    /* If uniqified output is turned out, output only the last of
       an identical series of lines. */
    if (sdp->unique) {
      if (savedflag && 
          SORTCompare(&saved, &lines[ord[0]].lines[cur[ord[0]]], sdp)) {
        if(FileWrite(saved.text, 1, saved.length, ofp) != (Uint4) saved.length)
          return -1;
        putc('\n', ofp);
      }
      if (savealloc < lines[ord[0]].lines[cur[ord[0]]].length + 1) {
        while (savealloc < lines[ord[0]].lines[cur[ord[0]]].length + 1)
          savealloc *= 2;
        if((saved.text = (UcharPtr) Realloc(saved.text, savealloc)) == NULL)
          return -1;
      }
      saved.length = lines[ord[0]].lines[cur[ord[0]]].length;
      MemCpy((CharPtr)saved.text, 
             (CharPtr)lines[ord[0]].lines[cur[ord[0]]].text,
             saved.length + 1);
      savedflag = 1;
    } else {
      if(FileWrite(lines[ord[0]].lines[cur[ord[0]]].text, 1,
                   lines[ord[0]].lines[cur[ord[0]]].length, ofp) !=
         (Uint4) lines[ord[0]].lines[cur[ord[0]]].length)
        return -1;
      putc('\n', ofp);
    }
    
    /* Check if we need to read more lines into core. */
    if (++cur[ord[0]] == lines[ord[0]].used) {
      /* missing '{' can lead to mistakes in later edits... */
      if (SORTFillBuf(&buffer[ord[0]], fps[ord[0]])) {
        SORTFindLines(&buffer[ord[0]], &lines[ord[0]], sdp);
        cur[ord[0]] = 0;
      } else {
        /* We reached EOF on fps[ord[0]]. */
        for (i = 1; i < nfps; ++i)
          if (ord[i] > ord[0])
            --ord[i];
        --nfps;
        FileClose(fps[ord[0]]);
        MemFree(buffer[ord[0]].buf);
        MemFree((CharPtr) lines[ord[0]].lines);
        for (i = ord[0]; i < nfps; ++i) {
          fps[i] = fps[i + 1];
          buffer[i] = buffer[i + 1];
          lines[i] = lines[i + 1];
          cur[i] = cur[i + 1];
        }
        for (i = 0; i < nfps; ++i)
          ord[i] = ord[i + 1];
        continue;
      }
    }
    
      /* The new line just read in may be larger than other lines
	 already in core; push it back in the queue until we encounter
	 a line larger than it. */
    for (i = 1; i < nfps; ++i) {
      t = SORTCompare(&lines[ord[0]].lines[cur[ord[0]]],
                      &lines[ord[i]].lines[cur[ord[i]]], sdp);
      if (! t)
        t = ord[0] - ord[i];
      if (t < 0)
        break;
    }
    t = ord[0];
    for (j = 1; j < i; ++j)
      ord[j - 1] = ord[j];
    ord[i - 1] = t;
  }
  
  if (sdp->unique && savedflag) {
    if(FileWrite(saved.text, 1, saved.length, ofp) != (Uint4) saved.length)
      return -1;
    putc('\n', ofp);
    MemFree(saved.text);
  }
  return 0;
}

/* Sort the array LINES using TEMP for temporary space. */
static void SORTArrayLines(SORTLinePtr lines,  Int4 nlines, 
                           SORTLinePtr temp, SORTDataPtr sdp)
{
  register SORTLinePtr lo, hi, t;
  register Int4 nlo, nhi;
  
  if (nlines == 2) {
    if (SORTCompare(&lines[0], &lines[1], sdp) > 0)
      *temp = lines[0], lines[0] = lines[1], lines[1] = *temp;
    return;
  }
  
  nlo = nlines / 2;
  lo = lines;
  nhi = nlines - nlo;
  hi = lines + nlo;
  
  if (nlo > 1)
    SORTArrayLines(lo, nlo, temp, sdp);
  
  if (nhi > 1)
    SORTArrayLines(hi, nhi, temp, sdp);
  
  t = temp;
  
  while (nlo && nhi)
    if (SORTCompare(lo, hi, sdp) <= 0)
      *t++ = *lo++, --nlo;
    else
      *t++ = *hi++, --nhi;
  while (nlo--)
    *t++ = *lo++;
  
  for (lo = lines, nlo = nlines - nhi, t = temp; nlo; --nlo)
    *lo++ = *t++;
}

/* Check that each of the given FILES is ordered.
   Return a count of disordered files. */
Int4 SORTCheckOrderS(CharPtr files[], Int4 nfiles, SORTObjectPtr sop)
{
  Int4 i, disorders = 0;
  FILE *fp;
  SORTDataPtr sdp;

  if((sdp = (SORTDataPtr) sop) == NULL)
    return -1;
  
  for (i = 0; i < nfiles; ++i) {
    if((fp = FileOpen(files[i], "r")) == NULL)
      return -1;
    if (!SORTCheckOrder(fp, sdp)) {
      ErrLogPrintf("sort: disorder on %s\n", files[i]);
      ++disorders;
    }
    FileClose(fp);
  }
  return disorders;
}

/* Insert a key at the end of the list. */
SORTErrorCode SORTInsertKey(SORTKeyFieldPtr key, SORTKeyFieldPtr keyhead)
{
  SORTKeyFieldPtr k = keyhead;
  
  while (k->next) {
    k = k->next;
  }
  k->next = key;
  key->next = NULL;

  return SORTNoError;
}
