/*
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
* File Name:  testcgi.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   4/24/98
*
* $Revision: 6.27 $
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

/*
*  To compile on Solaris:
*
*   cc -xildoff -o testcgi.cgi testcgi.c -lgen -lm
*
*  To compile on SGI:
*
*   cc -mips1 -o testcgi.cgi testcgi.c -lm -lPW -lsun
*/

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* for development on Mac, need to define popen and pclose */

#if defined(__POWERPC__) || defined(powerc) || defined(__powerc) || defined(__POWERPC)
#define popen fopen
#define pclose fclose
#endif

/* convenient defines and typedefs from NCBI toolkit */

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef Pointer
typedef void * Pointer;
#endif

#ifndef Char
typedef char Char, * CharPtr;
#endif

#ifndef Bool
typedef unsigned char Bool, * BoolPtr;
#endif

#ifndef TRUE
#define TRUE ((Bool)1)
#endif

#ifndef FALSE
#define FALSE ((Bool)0)
#endif

#ifndef Int2
typedef short Int2, * Int2Ptr;
#endif

#ifndef Int4
typedef long Int4, * Int4Ptr;
#endif

#ifndef MIN
#define MIN(a,b)	((a)>(b)?(b):(a))
#endif

/* useful portable character macros from NCBI toolkit (assumes ASCII) */

#define IS_DIGIT(c)	('0'<=(c) && (c)<='9')
#define IS_UPPER(c)	('A'<=(c) && (c)<='Z')
#define IS_LOWER(c)	('a'<=(c) && (c)<='z')
#define IS_ALPHA(c)	(IS_UPPER(c) || IS_LOWER(c))
#define TO_LOWER(c)	((Char)(IS_UPPER(c) ? (c)+' ' : (c)))
#define TO_UPPER(c)	((Char)(IS_LOWER(c) ? (c)-' ' : (c)))
#define IS_WHITESP(c) (((c) == ' ') || ((c) == '\n') || ((c) == '\r') || ((c) == '\t'))
#define IS_ALPHANUM(c) (IS_ALPHA(c) || IS_DIGIT(c))
#define IS_PRINT(c)	(' '<=(c) && (c)<='~')

/* url decode/encode functions modified from NCBI toolkit */

/* Return integer (0..15) corresponding to the "ch" as a hex digit
 * Return -1 on error
 */
static int s_HexChar(char ch)
{
  if ('0' <= ch  &&  ch <= '9')
    return ch - '0';
  if ('a' <= ch  &&  ch <= 'f')
    return 10 + (ch - 'a');
  if ('A' <= ch  &&  ch <= 'F')
    return 10 + (ch - 'A');
  return -1;
}

/* The URL-encoding table
 */
static const char s_Encode[256][4] = {
  "%00", "%01", "%02", "%03", "%04", "%05", "%06", "%07",
  "%08", "%09", "%0A", "%0B", "%0C", "%0D", "%0E", "%0F",
  "%10", "%11", "%12", "%13", "%14", "%15", "%16", "%17",
  "%18", "%19", "%1A", "%1B", "%1C", "%1D", "%1E", "%1F",
  "+",   "!",   "%22", "%23", "$",   "%25", "%26", "'",
  "(",   ")",   "*",   "%2B", ",",   "-",   ".",   "%2F",
  "0",   "1",   "2",   "3",   "4",   "5",   "6",   "7",
  "8",   "9",   "%3A", "%3B", "%3C", "%3D", "%3E", "%3F",
  "%40", "A",   "B",   "C",   "D",   "E",   "F",   "G",
  "H",   "I",   "J",   "K",   "L",   "M",   "N",   "O",
  "P",   "Q",   "R",   "S",   "T",   "U",   "V",   "W",
  "X",   "Y",   "Z",   "%5B", "%5C", "%5D", "%5E", "_",
  "%60", "a",   "b",   "c",   "d",   "e",   "f",   "g",
  "h",   "i",   "j",   "k",   "l",   "m",   "n",   "o",
  "p",   "q",   "r",   "s",   "t",   "u",   "v",   "w",
  "x",   "y",   "z",   "%7B", "%7C", "%7D", "%7E", "%7F",
  "%80", "%81", "%82", "%83", "%84", "%85", "%86", "%87",
  "%88", "%89", "%8A", "%8B", "%8C", "%8D", "%8E", "%8F",
  "%90", "%91", "%92", "%93", "%94", "%95", "%96", "%97",
  "%98", "%99", "%9A", "%9B", "%9C", "%9D", "%9E", "%9F",
  "%A0", "%A1", "%A2", "%A3", "%A4", "%A5", "%A6", "%A7",
  "%A8", "%A9", "%AA", "%AB", "%AC", "%AD", "%AE", "%AF",
  "%B0", "%B1", "%B2", "%B3", "%B4", "%B5", "%B6", "%B7",
  "%B8", "%B9", "%BA", "%BB", "%BC", "%BD", "%BE", "%BF",
  "%C0", "%C1", "%C2", "%C3", "%C4", "%C5", "%C6", "%C7",
  "%C8", "%C9", "%CA", "%CB", "%CC", "%CD", "%CE", "%CF",
  "%D0", "%D1", "%D2", "%D3", "%D4", "%D5", "%D6", "%D7",
  "%D8", "%D9", "%DA", "%DB", "%DC", "%DD", "%DE", "%DF",
  "%E0", "%E1", "%E2", "%E3", "%E4", "%E5", "%E6", "%E7",
  "%E8", "%E9", "%EA", "%EB", "%EC", "%ED", "%EE", "%EF",
  "%F0", "%F1", "%F2", "%F3", "%F4", "%F5", "%F6", "%F7",
  "%F8", "%F9", "%FA", "%FB", "%FC", "%FD", "%FE", "%FF"
};
#define VALID_URL_SYMBOL(ch)  (s_Encode[(unsigned char)ch][0] != '%')

static Bool Url_Decode
(const void* src_buf,
 size_t      src_size,
 size_t*     src_read,
 void*       dst_buf,
 size_t      dst_size,
 size_t*     dst_written)
{
  unsigned char *src = (unsigned char*)src_buf;
  unsigned char *dst = (unsigned char*)dst_buf;

  *src_read    = 0;
  *dst_written = 0;
  if (!src_size  ||  !dst_size)
    return TRUE;

  for ( ;  *src_read != src_size  &&  *dst_written != dst_size;
        (*src_read)++, (*dst_written)++, src++, dst++) {
    switch ( *src ) {
    case '%': {
      int i1, i2;
      if (*src_read + 2 > src_size)
        return TRUE;
      if ((i1 = s_HexChar(*(++src))) == -1)
        return (Bool)(*dst_written ? TRUE : FALSE);
      if (*src_read + 3 > src_size)
        return TRUE;
      if ((i2 = s_HexChar(*(++src))) == -1)
        return (Bool)(*dst_written ? TRUE : FALSE);

      *dst = (unsigned char)((i1 << 4) + i2);
      *src_read += 2;
      break;
    }

    case '+': {
      *dst = ' ';
      break;
    }

    default:
      if ( VALID_URL_SYMBOL(*src) )
        *dst = *src;
      else
        return (Bool)(*dst_written ? TRUE : FALSE);
    }
  }

  /*
  ASSERT( src == (unsigned char*)src_buf + *src_read    );
  ASSERT( dst == (unsigned char*)dst_buf + *dst_written );
  */
  return TRUE;
}

static void Url_Encode
(const void* src_buf,
 size_t      src_size,
 size_t*     src_read,
 void*       dst_buf,
 size_t      dst_size,
 size_t*     dst_written)
{
  unsigned char *src = (unsigned char*)src_buf;
  unsigned char *dst = (unsigned char*)dst_buf;

  *src_read    = 0;
  *dst_written = 0;
  if (!src_size  ||  !dst_size)
    return;

  for ( ;  *src_read != src_size  &&  *dst_written != dst_size;
        (*src_read)++, (*dst_written)++, src++, dst++) {
    const char* subst = s_Encode[*src];
    if (*subst != '%') {
      *dst = *subst;
    } else if (*dst_written < dst_size - 2) {
      *dst = '%';
      *(++dst) = *(++subst);
      *(++dst) = *(++subst);
      *dst_written += 2;
    }
    else {
      return;
    }
  }
  /*
  ASSERT( src == (unsigned char*)src_buf + *src_read    );
  ASSERT( dst == (unsigned char*)dst_buf + *dst_written );
  */
}

/* combined read and decode, encode and write functions */

#define URLBUF 256

static size_t ReadAndDecode (CharPtr buf, size_t size, FILE *file)

{
  size_t  cnt;
  size_t  ct;
  size_t  len;
  size_t  more;
  size_t  srcrd;
  Char    tmp [URLBUF + 2];

  if (buf == NULL || size < 3 || file == NULL) return 0;
  len = MIN ((size_t) URLBUF, (size_t) (size - 2));
  cnt = fread (tmp, 1, len, file);
  if (cnt == 0) return 0;
  more = 0;
  if (tmp [cnt - 1] == '%') {
    more = 2;
  } else if (cnt > 1 && tmp [cnt - 2] == '%') {
    more = 1;
  }
  if (more > 0) {
    ct = fread (tmp + cnt, 1, more, file);
    if (ct == more) {
      cnt += more;
    }
  }
  if (Url_Decode (tmp, cnt, &srcrd, buf, size, &ct)) {
    return ct;
  }
  return 0;
}

static size_t EncodeAndWrite (CharPtr buf, size_t size, FILE *file)

{
  size_t  cnt;
  size_t  ct = 0;
  size_t  len;
  size_t  srcrd;
  Char    tmp [URLBUF * 3];

  if (buf == NULL || size < 1 || file == NULL) return 0;
  while (size > 0) {
    len = MIN ((size_t) URLBUF, (size_t) size);
    Url_Encode (buf, len, &srcrd, tmp, URLBUF, &cnt);
    buf += srcrd;
    size -= srcrd;
    if (cnt > 0) {
      ct += fwrite (tmp, 1, cnt, file);
    }
    if (srcrd == 0) return ct;
  }
  return ct;
}

/* copy of query string is parsed with strtok into tag and val paired arrays */

#define MAX_ENTRIES  32

static CharPtr  query = NULL;
static Int2     num_tags = 0;
static CharPtr  tag [MAX_ENTRIES];
static CharPtr  val [MAX_ENTRIES];


static Bool ParseQuery (CharPtr qstr, Bool queryRequired)

{
  size_t   len;
  CharPtr  ptr;

/*
*  given a query string enzyme=EcoRI&pattern=GAATTC
*/

/* The >Message prefix causes Sequin to display the message to the user */

  if (qstr == NULL) {
    printf ("Content-type: text/html\r\n\r\n");
    printf (">Message\nFAILURE - Query not detected\n");
    fflush (stdout);
    return FALSE;
  }

/* allocates a copy of query string that can be modified during parsing */

  len = strlen (qstr);
  query = malloc (len + 3);

  if (query == NULL) {
    printf ("Content-type: text/html\r\n\r\n");
    printf (">Message\nFAILURE - query allocation failed\n");
    fflush (stdout);
    return FALSE;
  }

  memset (query, 0, len + 2);
  strcpy (query, qstr);

/* parse tag=value&tag=value query into arrays for easier interpretation */

  memset (tag, 0, sizeof (tag));
  memset (val, 0, sizeof (val));

  ptr = strtok (query, "=");
  for (num_tags = 0; num_tags < MAX_ENTRIES && ptr != NULL; num_tags++) {
    tag [num_tags] = ptr;
    ptr = strtok (NULL, "&");
    if (ptr != NULL) {
      val [num_tags] = ptr;
      ptr = strtok (NULL, "=");
    }
  }

/*
*  given the above query example, the tag and val arrays are now:
*
*   tag [0] = "enzyme"    val [0] = "EcoRI"
*   tag [1] = "pattern"   val [1] = "GAATTC"
*   tag [2] = NULL        val [2] = NULL
*
*  and num_tags is 2
*/

/* verify that any required query string is present in the URL */

  if (queryRequired && num_tags == 0) {
    printf ("Content-type: text/html\r\n\r\n");
    printf (">Message\nFAILURE - Unable to parse tokens from query:\n'%s'\n", query);
    fflush (stdout);
    return FALSE;
  }

  return TRUE;
}


static CharPtr FindByName (CharPtr find)

{
  Int2  i;

/* search the tag array for the desired name, returning the associated value */

  if (find == NULL) return NULL;
  for (i = 0; i < num_tags; i++) {
    if (tag [i] != NULL && strcmp (tag [i], find) == 0) {
      return val [i];
    }
  }
  return NULL;
}


static Int2 ListHasString (CharPtr list [], CharPtr str)

{
  Int2  i;

/* search a null-terminated array of strings, returning an integer index */

  if (str == NULL || list == NULL) return -1;
  for (i = 0; list [i] != NULL; i++) {
    if (strcmp (str, list [i]) == 0) return i;
  }
  return -1;
}


#define SEND_FILENAME_ARGS_BEFORE  1
#define SEND_FILENAME_ARGS_AFTER   2
#define SEND_STDIN                 3

static void RunCustom (CharPtr tempfile)

{
  CharPtr   arguments;
  Char      buf [256];
  Char      cmmd [256];
  size_t    ct;
  FILE*     fp;
  Int2      meth = SEND_STDIN;
  CharPtr   method;
  CharPtr   port;
  CharPtr   program;
  long int  val;

/* protect against custom requests on public server */

  port = getenv ("SERVER_PORT");
  if (port != NULL) {
    if (sscanf (port, "%ld", &val) == 1 && val == 80) {
      return;
    }
  }

/* program argument would be actual path on cgi server machine */

  program = FindByName ("program");
  if (program == NULL) {
    printf (">Message\nFAILURE - no program path specified\n");
    fflush (stdout);
    return;
  }

/* method defaults to sending data to program via stdin */

  method = FindByName ("method");
  if (method == NULL || strstr (method, "stdin") != NULL) {
    meth = SEND_STDIN;
  } else if (strstr (method, "filename") != NULL) {
    meth = SEND_FILENAME_ARGS_AFTER;
  } else if (strstr (method, "arguments") != NULL) {
    meth = SEND_FILENAME_ARGS_BEFORE;
  } else {
    meth = SEND_STDIN;
  }

/* note that for arguments, %20 in a query string is converted to a space */

  arguments = FindByName ("arguments");
  if (arguments == NULL) {
    arguments = "";
  }

/* launch program sending arguments and filename or data in appropriate order */

  switch (meth) {
    case SEND_FILENAME_ARGS_BEFORE :
      sprintf (cmmd, "%s %s %s", program, arguments, tempfile);
      break;
    case SEND_FILENAME_ARGS_AFTER :
      sprintf (cmmd, "%s %s %s", program, tempfile, arguments);
      break;
    case SEND_STDIN :
    default :
      sprintf (cmmd, "%s %s < %s", program, arguments, tempfile);
      break;
  }
  fp = popen (cmmd, "r");
  if (fp == NULL) return;

/* assumes program sends output to stdout */

  while ((ct = fread (buf, 1, sizeof (buf), fp)) > 0) {
    EncodeAndWrite (buf, ct, stdout);
    fflush (stdout);
  }
  pclose (fp);
}


static void RunEcho (CharPtr tempfile)

{
  size_t  ct;
  Char    buf [256];
  FILE*   fp;

/* reconstruct and print the query string */

/*
  for (i = 0; i < num_tags; i++) {
    if (i > 0) {
      sprintf (buf, "%");
      EncodeAndWrite (buf, strlen (buf), stdout);
      fflush (stdout);
    }
    sprintf (buf, "%s=%s", tag [i], val [i]);
    EncodeAndWrite (buf, strlen (buf), stdout);
    fflush (stdout);
  }
  sprintf (buf, "\n");
  EncodeAndWrite (buf, strlen (buf), stdout);
  fflush (stdout);
*/

/* now echo the data file */

  fp = fopen (tempfile, "r");
  if (fp == NULL) return;

  while ((ct = fread (buf, 1, sizeof (buf), fp)) > 0) {
    EncodeAndWrite (buf, ct, stdout);
    fflush (stdout);
  }
  fclose (fp);
}


static void RunSeg (CharPtr tempfile)

{
  Char     buf [256];
  Char     cmmd [256];
  size_t   ct;
  FILE*    fp;
  float    hi;
  CharPtr  hicut;
  CharPtr  lowcut;
  Char     tmp [16];
  CharPtr  window;

/* launch seg with -x parameter, arguments, and name of data file */

  window = FindByName ("window");
  if (window == NULL) {
    window = "12";
  }
  lowcut = FindByName ("lowcut");
  if (lowcut == NULL) {
    lowcut = "2.2";
  }
  hicut = FindByName ("hicut");
  if (hicut == NULL || hicut [0] == '\0') {
    if (sscanf (lowcut, "%f", &hi) == 1) {
      hi += 0.3;
      sprintf (tmp, "%4.2f", hi);
      hicut = tmp;
    } else {
      hicut = "2.5";
    }
  }

  sprintf (cmmd, "/usr/ncbi/bin/seg %s %s %s %s -x", tempfile, window, lowcut, hicut);
  fp = popen (cmmd, "r");
  if (fp == NULL) return;

/* send processed FASTA data from seg directly to stdout and calling program */

  while ((ct = fread (buf, 1, sizeof (buf), fp)) > 0) {
    EncodeAndWrite (buf, ct, stdout);
    fflush (stdout);
  }
  pclose (fp);
}


#define MAX_FIELDS  9

static void RunTrnaScan (CharPtr tempfile)

{
  CharPtr   aa;
  CharPtr   beg;
  Char      buf [256];
  Char      cmmd [256];
  CharPtr   end;
  CharPtr   field [MAX_FIELDS];
  FILE*     fp;
  CharPtr   id;
  Int2      idNotSent = TRUE;
  Int2      inBody = FALSE;
  CharPtr   intronBeg;
  CharPtr   intronEnd;
  long int  intronStart;
  long int  intronStop;
  Int2      numFields = 0;
  CharPtr   ptr;
  CharPtr   speed;
  long int  start;
  long int  stop;
  Char      str [80];

/* launch tRNAscan-SE with -q parameter and name of data file */

  speed = FindByName ("speed");
  if (speed != NULL && strcmp (speed, "slow") == 0) {
    sprintf (cmmd, "/am/MolBio/trnascan-SE/bin/tRNAscan-SE -q -C %s", tempfile);
  } else {
    sprintf (cmmd, "/am/MolBio/trnascan-SE/bin/tRNAscan-SE -q %s", tempfile);
  }

  fp = popen (cmmd, "r");
  if (fp == NULL) return;

/* line by line processing of tRNAscan-SE output table */

  while (fgets (buf, sizeof (buf), fp) != NULL) {

    if (inBody) {
      memset (field, 0, sizeof (field));

/*
*  parse tab-delimited output line into array of fields, avoiding use of
*  strtok so that empty columns (adjacent tabs) are properly assigned to
*  field array
*/

      ptr = buf;
      for (numFields = 0; numFields < MAX_FIELDS && ptr != NULL; numFields++) {
        field [numFields] = ptr;
        ptr = strchr (ptr, '\t');
        if (ptr != NULL) {
          *ptr = '\0';
          ptr++;
        }
      }

/* interested in ID, start, stop, amino acid, and intron start and stop */

      id = field [0];
      beg = field [2];
      end = field [3];
      aa = field [4];
      intronBeg = field [6];
      intronEnd = field [7];

      if (numFields > 7 &&
          sscanf (beg, "%ld", &start) == 1 &&
          sscanf (end, "%ld", &stop) == 1 &&
          sscanf (intronBeg, "%ld", &intronStart) == 1 &&
          sscanf (intronEnd, "%ld", &intronStop) == 1) {

/* first line of output gives SeqId from FASTA definition line */

        if (idNotSent) {
          sprintf (str, ">Features %s tRNAscan-SE\n", id);
          EncodeAndWrite (str, strlen (str), stdout);
          fflush (stdout);
          idNotSent = FALSE;
        }

/* first line of feature has start (tab) stop (tab) feature key */
/* multiple intervals would have lines of start (tab) stop */

        if (intronStart == 0 && intronStop == 0) {
          sprintf (str, "%ld\t%ld\ttRNA\n", (long) start, (long) stop);
          EncodeAndWrite (str, strlen (str), stdout);
          fflush (stdout);
        } else {
          sprintf (str, "%ld\t%ld\ttRNA\n", (long) start, (long) (intronStart - 1));
          EncodeAndWrite (str, strlen (str), stdout);
          fflush (stdout);
          sprintf (str, "%ld\t%ld\t\n", (long) (intronStop + 1), (long) stop);
          EncodeAndWrite (str, strlen (str), stdout);
          fflush (stdout);
        }

/* qualifier lines are (tab) (tab) (tab) qualifier key (tab) value */

        if (strstr (aa, "Pseudo") != NULL) {
          sprintf (str, "\t\t\tnote\ttRNA-Pseudo\n");
          EncodeAndWrite (str, strlen (str), stdout);
          fflush (stdout);
        } else {
          sprintf (str, "\t\t\tproduct\t%s\n", aa);
          EncodeAndWrite (str, strlen (str), stdout);
          fflush (stdout);
        }

/* dash (formerly empty) gene qualifier to suppress /gene (e.g., if tRNA is in an intron) */

        sprintf (str, "\t\t\tgene\t-\n");
        EncodeAndWrite (str, strlen (str), stdout);
        fflush (stdout);
      }
    }

/* detect last line of table header, ignoring everything before data section */

    if (strstr (buf, "-----") != NULL) {
      inBody = TRUE;
    }
  }
  pclose (fp);

  if (idNotSent) {
    sprintf (str, ">Message\ntRNAscan-SE found no tRNA genes in this sequence\n");
    EncodeAndWrite (str, strlen (str), stdout);
    fflush (stdout);
  }
}


/* this program can provide several services, specified in URL query string */

static CharPtr services [] = {
  "", "custom", "echo", "seg", "trnascan", NULL
};

/* main function of cgi program, called by HTTPD server */

main (int argc, char *argv[])

{
  CharPtr  bf;
  Char     buf [256];
  size_t   ct;
  FILE*    fp;
  CharPtr  method;
  CharPtr  ptr;
  CharPtr  request;
  Int2     service;
  Char     tempfile [L_tmpnam];

/* at startup, first verify environment */

  method = getenv ("REQUEST_METHOD");
  if (method == NULL) {
    printf ("Content-type: text/html\r\n\r\n");
    printf (">Message\nFAILURE - Program launched from command line\n");
    fflush (stdout);
    return 1;
  }

/* ensure that the POST method is being sent from the HTTPD server */

  if (strcmp (method, "POST") != 0) {
    printf ("Content-type: text/html\r\n\r\n");
    printf (">Message\nFAILURE - Method (%s) must be POST\n", method);
    fflush (stdout);
    return 1;
  }

/* backward compatibility for query in URL after ? character */

  ptr = getenv ("QUERY_STRING");
  if (ptr != NULL && strlen (ptr) > 0) {
    if (! ParseQuery (ptr, TRUE)) {
      printf ("Content-type: text/html\r\n\r\n");
      printf (">Message\nFAILURE - QUERY_STRING parsing failed\n");
      fflush (stdout);
      if (query != NULL) {
        free (query);
      }
      return 1;
    }
  }

/*
*  copy all of stdin to temporary file before sending anything to stdout,
*  to get around an apparent limitation in the HTTPD implementation that
*  can cause socket deadlock
*/

  tmpnam (tempfile);
  fp = fopen (tempfile, "w");
  if (fp == NULL) {
    printf ("Content-type: text/html\r\n\r\n");
    printf (">Message\nFAILURE - file open failed\n");
    fflush (stdout);
    return 1;
  }

  while ((ct = ReadAndDecode (buf, sizeof (buf), stdin)) > 0) {

    bf = buf;

/* write data part of buffer to temporary file */

    fwrite (bf, 1, ct, fp);
  }
  fflush (fp);
  fclose (fp);

/* now send required first header information to stdout */

  printf ("Content-type: text/html\r\n\r\n");
  fflush (stdout);

/* expect request=custom, request=echo, request=seg, or request=trnascan */

  request = FindByName ("request");
  if (request == NULL) {
    printf (">Message\nFAILURE - No service request\n");
    fflush (stdout);
    return 1;
  }

/* compare request value against list of available services */

  service = ListHasString (services, request);
  if (service < 1) {
    printf (">Message\nFAILURE - Unable to match request '%s'\n", request);
    fflush (stdout);
    return 1;
  }

/* call appropriate external analysis program */

  switch (service) {

#ifdef ALLOW_CUSTOM_PROGRAM

/* for security, custom programs not allowed without symbol at compile time */

    case 1 :
      RunCustom (tempfile);
      break;

#endif /* ALLOW_CUSTOM_PROGRAM */

    case 2 :
      RunEcho (tempfile);
      break;
    case 3 :
      RunSeg (tempfile);
      break;
    case 4 :
      RunTrnaScan (tempfile);
      break;
    default :
      break;
  }

/* flush buffer, cleanup temporary files and allocated memory, and exit */

  fflush (stdout);
  remove (tempfile);
  free (query);
  return 0;
}

