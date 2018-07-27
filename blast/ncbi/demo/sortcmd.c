/* $Id: sortcmd.c,v 6.1 1998/06/11 18:18:55 shavirin Exp $
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
* File Name:  $RCSfile: sortcmd.c,v $
*
* Author:  Sergei Shavirin
*
* Initial Version Creation Date: 03/24/1997
*
* $Revision: 6.1 $
*
* File Description:
*         Main file for sortcmd standalone executable.
*
* $Log: sortcmd.c,v $
* Revision 6.1  1998/06/11 18:18:55  shavirin
* Removed old style function declarations
*
* Revision 6.0  1997/08/25 18:20:42  madden
* Revision changed to 6.0
*
* Revision 1.4  1997/05/09 17:39:58  shavirin
* Added ability to place sorted file on initial file to be
* sorted
*
 * Revision 1.3  1997/03/18  17:27:55  shavirin
 * Fixed handling of stdin
 *
 * Revision 1.2  1997/03/18  16:54:30  shavirin
 * Changed sortcmd to ncbisort in usage string
 *
 * Revision 1.1  1997/03/13  21:53:21  shavirin
 * Initial revision
 *
*
* ==========================================================================
*/

#include <ncbisort.h>

#define UCHAR_LIM (UCHAR_MAX + 1)

static void usage (void);
static void badfieldspec(CharPtr s);
static UcharPtr fold_tolower(void);
static Int4Ptr nonprinting(void);
static Int4Ptr nondictionary(void);

Int2 Main(void)
{
  SORTKeyFieldPtr key = NULL;
  SORTKeyField gkey;
  CharPtr s;
  Int4 i, t, t2;
  Int4 checkonly = 0, mergeonly = 0, nfiles;
  CharPtr minus = "-", outfile = minus, *files, tmp;
  CharPtr sort_stdin = "stdin";
  FILE *ofp;
  SORTObjectPtr sop;
  SORTKeyFieldPtr keyhead;
  Int4 argc = GetArgc();
  CharPtr PNTR argv = GetArgv();

  if((sop = SORTObjectNew(NULL, '\0', 0,
                          FALSE, FALSE)) == NULL)
    return 1;

  if((keyhead = SORTGetKeyHead(sop)) == NULL)
    return -1;

  gkey.sword = gkey.eword = -1;
  gkey.ignore = NULL;
  gkey.translate = NULL;
  gkey.numeric = gkey.month = gkey.reverse = 0;
  gkey.skipsblanks = gkey.skipeblanks = 0;

  for (i = 1; i < argc; ++i) {
    if (argv[i][0] == '+') {
      if (key) {
        SORTInsertKey(key, keyhead);
      }

      if((key = (SORTKeyFieldPtr)MemNew(sizeof(SORTKeyField))) == NULL)
        return -1;

      key->eword = -1;
      key->ignore = NULL;
      key->translate = NULL;
      key->skipsblanks = key->skipeblanks = 0;
      key->numeric = key->month = key->reverse = 0;
      s = argv[i] + 1;
      if (!IS_DIGIT(*s))
        badfieldspec(argv[i]);
      for (t = 0; IS_DIGIT(*s); ++s)
        t = 10 * t + *s - '0';
      t2 = 0;
      if (*s == '.')
        for (++s; IS_DIGIT(*s); ++s)
          t2 = 10 * t2 + *s - '0';
      if (t2 || t) {
        key->sword = t;
        key->schar = t2;
      } else
        key->sword = -1;
      while (*s) {
        switch (*s) {
        case 'b':
          key->skipsblanks = 1;
          break;
        case 'd':
          key->ignore = nondictionary();
          break;
        case 'f':
          key->translate = fold_tolower();
          break;
        case 'i':
          key->ignore = nonprinting();
          break;
        case 'M':
          key->skipsblanks = key->skipeblanks = key->month = 1;
          break;
        case 'n':
          key->skipsblanks = key->skipeblanks = key->numeric = 1;
          break;
        case 'r':
          key->reverse = 1;
          break;
        default:
          badfieldspec(argv[i]);
          break;
        }
        ++s;
      }
    } else if (argv[i][0] == '-') {
      if (!StringCmp("-", argv[i]))
        break;
      s = argv[i] + 1;
      /* If thios is second part of +x.y -x.y key-structure ? */

      if (IS_DIGIT(*s)) {
        if (! key)
          usage();
        for (t = 0; IS_DIGIT(*s); ++s)
          t = t * 10 + *s - '0';
        t2 = 0;
        if (*s == '.')
          for (++s; IS_DIGIT(*s); ++s)
            t2 = t2 * 10 + *s - '0';
        key->eword = t;
        key->echar = t2;
        while (*s) {

          switch (*s) {
          case 'b':
            key->skipeblanks = 1;
            break;
          case 'd':
            key->ignore = nondictionary();
            break;
          case 'f':
            key->translate = fold_tolower();
            break;
          case 'i':
            key->ignore = nonprinting();
            break;
          case 'M':
            key->skipsblanks = key->skipeblanks = key->month = 1;
            break;
          case 'n':
            key->skipsblanks = key->skipeblanks = key->numeric = 1;
            break;
          case 'r':
            key->reverse = 1;
            break;
          default:
            badfieldspec(argv[i]);
            break;
          }
          ++s;
        }
        SORTInsertKey(key, keyhead);
        key = NULL;
      } else {
        while (*s) {
          switch (*s) {
          case 'b':
            gkey.skipsblanks = gkey.skipeblanks = 1;
            break;
          case 'c':
            checkonly = 1;
            break;
          case 'd':
            gkey.ignore = nondictionary();
            break;
          case 'f':
            gkey.translate = fold_tolower();
            break;
          case 'i':
            gkey.ignore = nonprinting();
            break;
          case 'M':
            gkey.skipsblanks = gkey.skipeblanks = gkey.month = 1;
            break;
          case 'm':
            mergeonly = 1;
            break;
          case 'n':
            gkey.skipsblanks = gkey.skipeblanks = gkey.numeric = 1;
            break;
          case 'o':
            if (s[1])
              outfile = s + 1;
            else {
              if (i == argc - 1) {
                fprintf(stderr, "ncbisort: missing argument to -o\n");
                exit(-1);
              } else
                outfile = argv[++i];
            }
            goto outer;
          case 'r':
            gkey.reverse = 1;
            SORTSetReverse(TRUE, sop);
            break;
          case 'T':
            if (s[1])
              SORTSetPrefix(s + 1, sop);
            else {
              if (i == argc - 1) {
                fprintf(stderr, "ncbisort: missing argument to -T\n");
                exit(-1);
              } else {
                SORTSetPrefix( argv[++i], sop);
              }
            }
            goto outer;
          case 't':
            if (s[1])
              SORTSetTab(*++s, sop);
            else if (i < argc - 1) {
              SORTSetTab(*argv[++i], sop);
              goto outer;
            } else {
              fprintf(stderr, "ncbisort: missing character for -tc\n");
              exit(-1);
            }
            break;
          case 'u':
            SORTSetUnique(TRUE, sop);
            break;
          case 'V':
            fprintf(stderr, "ncbisort, version 1.0a\n");
            break;
          default:
            usage();
            exit(-1);
          }
          ++s;
        }
      }
    } else
      break;
  outer:;
  }

  if (key) {
    SORTInsertKey(key, keyhead);
  }

  /* Inheritance of global options to individual keys. */

  for (key = keyhead->next; key; key = key->next)
    if (!key->ignore && !key->translate && !key->skipsblanks && !key->reverse
	&& !key->skipeblanks && !key->month && !key->numeric) {
      key->ignore = gkey.ignore;
      key->translate = gkey.translate;
      key->skipsblanks = gkey.skipsblanks;
      key->skipeblanks = gkey.skipeblanks;
      key->month = gkey.month;
      key->numeric = gkey.numeric;
      key->reverse = gkey.reverse;
    }

  if (!keyhead->next && (gkey.ignore || gkey.translate || gkey.skipsblanks
			 || gkey.reverse || gkey.skipeblanks
			 || gkey.month || gkey.numeric))
    SORTInsertKey(&gkey, keyhead);

  if (i < argc) {
    nfiles = argc - i;
    files = &argv[i];
  } else {
    nfiles = 1;
    files = &sort_stdin;
  }

  if (checkonly) {
    return (Int2)(SORTCheckOrderS(files, nfiles, sop));
  }

  if (StringCmp(outfile, "-")) {
    for (i = 0; i < nfiles; ++i)
      if (!StringCmp(outfile, files[i]))
        break;
    if (i == nfiles) {
      if((ofp = FileOpen(outfile, "w")) == NULL)
        return -1;
    } else {
      Uchar buf[8192];
      FILE *fp = FileOpen(outfile, "r");
      size_t cc;
      if(fp == NULL)
        return -1;

      tmp = SORTAddTempName(sop);

      if((ofp = FileOpen(tmp, "w")) == NULL)
        return -1;
      while ((cc = FileRead(buf, 1, sizeof(buf), fp)) > 0) {
          if(!FileWrite(buf, 1, cc, ofp))
              return -1;
      }
      
      if (ferror(fp))  {
          fprintf(stderr, "sort: error in fread (%s)\n",
                  strerror(errno));
          return(-1);
      }

      FileClose(ofp);
      FileClose(fp);
      files[i] = tmp;
      if((ofp = FileOpen(outfile, "w")) == NULL)
        return -1;
    }
  } else {
    ofp = stdout;
  }

  if (mergeonly)
    SORTMergeFiles(files, nfiles, ofp, sop);
  else
    SORTFiles(files, nfiles, ofp, sop);

  FileClose(ofp);

  return 0;
}

static void usage(void)
{
  fprintf(stderr,
	  "usage: ncbisort [ -cmu ] [ -tc ] [ -o file ] [ -T dir ]\n");
  fprintf(stderr,
	  "                [ -bdfiMnr ] [ +n [ -m ] . . . ] [ files . . . ]\n");
  exit(-1);
}
static void badfieldspec(CharPtr s)
{
  fprintf(stderr, "ncbisort: bad field specification %s\n", s);
  exit(-1);
}

static UcharPtr fold_tolower(void)
{
  UcharPtr buffer;
  Int4 j;

  buffer = (UcharPtr)MemNew(UCHAR_LIM);

  for (j = 0; j < UCHAR_LIM; ++j) {
    if ( IS_UPPER(j) )
      buffer[j] = (Uchar)TO_LOWER(j);
    else
      buffer[j] = (Uchar)j;
  }
  return buffer;
}

static Int4Ptr nonprinting(void)
{
  Int4Ptr buffer;
  Int4 j;

  buffer = (Int4Ptr) MemNew(UCHAR_LIM*sizeof(Int4));
  for (j = 0; j < UCHAR_LIM; ++j) {
    if (!IS_PRINT(j))
      buffer[j] = 1;
  }
  return buffer;
}

static Int4Ptr nondictionary(void)
{
  Int4Ptr buffer;
  Int4 j;

  buffer = (Int4Ptr)MemNew(UCHAR_LIM*sizeof(Int4));

  for (j = 0; j < UCHAR_LIM; ++j) {
    if (!IS_ALPHANUM(j) && !ISBLANK(j))
      buffer[j] = 1;
  }
  return buffer;
}

