/*   flint.c
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
* File Name:  flint.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   4/19/04
*
* $Revision: 1.4 $
*
* File Description:
*
*  Lint for FASTA - reformats to desired line length
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <objall.h>
#include <objsset.h>
#include <objsub.h>
#include <objfdef.h>
#include <seqport.h>
#include <sequtil.h>
#include <sqnutils.h>
#include <subutil.h>
#include <tofasta.h>
#include <gather.h>
#include <explore.h>

static void ProcessFasta (
  FILE *ifp,
  FILE *ofp,
  Int2 linelen,
  Int2 blocklen,
  Int2 grouplen
)

{
  Int2     blk;
  Char     buf [512];
  Char     ch;
  Int2     grp;
  Int2     idx;
  Boolean  in_defline;
  Int2     lin;
  Char     line [4004];
  CharPtr  ptr;
  CharPtr  str;
  size_t   total;

  in_defline = FALSE;
  idx = 0;
  blk = 0;
  lin = 0;
  grp = 0;

  total = FileRead (line, sizeof (Char), 4000, ifp);
  while (total > 0) {
    line [total] = '\0';

    str = line;
    ch = *str;
    while (ch != '\0') {

      /* look for defline start character */

      if (ch == '>') {

        /* flush any sequence remaining in buffer */

        if (lin > 0) {
          buf [idx] = '\0';
          fprintf (ofp, "%s\n", buf);
          idx = 0;
          blk = 0;
          lin = 0;
          grp = 0;
        }

        /* and set in_defline state flag */

        in_defline = TRUE;
      }

      if (in_defline) {

        /* look for carriage return, line feed, or end of input buffer */

        ptr = str;
        ch = *ptr;
        while (ch != '\0' && ch != '\n' && ch != '\r') {
          ptr++;
          ch = *ptr;
        }
        *ptr = '\0';

        if (ch == '\0') {

          /* defline continues on next block */

          fprintf (ofp, "%s", str);

          /* ch == '\0' will cause new buffer to be read */

        } else {

          /* found newline within input buffer */

          fprintf (ofp, "%s\n", str);

          /* clear state flag */

          in_defline = FALSE;

          str = ptr;
          str++;
          ch = *str;
        }

      } else {

        if (IS_ALPHA (ch)) {

          /* optionally separate blocks with space */

          if (blk >= blocklen && blocklen > 0) {
            buf [idx] = ' ';
            idx++;
            blk = 0;
          }

          /* save sequence character to buffer */

          buf [idx] = ch;
          idx++;
          blk++;
          lin++;
        }

        /* write sequence as soon as we have line of characters */

        if (lin >= linelen) {
          buf [idx] = '\0';

          /* optionally separate groups with blank line */

          if (grp >= grouplen && grouplen > 0) {
            fprintf (ofp, "\n");
            grp = 0;
          }

          /* print actual sequence line here */

          fprintf (ofp, "%s\n", buf);
          idx = 0;
          blk = 0;
          lin = 0;
          grp++;
        }

        str++;
        ch = *str;
      }
    }

    total = FileRead (line, sizeof (Char), 4000, ifp);
  }

  /* print any remaining sequence */

  if (lin > 0) {
    buf [idx] = '\0';
    if (grp >= grouplen && grouplen > 0) {
      fprintf (ofp, "\n");
    }
    fprintf (ofp, "%s\n", buf);
  }
}

#define i_argInputFile    0
#define o_argOutputFile   1
#define e_argLineLength   2
#define b_argBlockLength  3
#define g_argGroupLength  4

Args myargs [] = {
  {"Input File", "stdin", NULL, NULL,
    FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  {"Output File", "stdout", NULL, NULL,
    FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
  {"Line Length", "60", "10", "200",
    TRUE, 'e', ARG_INT, 0.0, 0, NULL},
  {"Block Length", "0", "0", "100",
    TRUE, 'b', ARG_INT, 0.0, 0, NULL},
  {"Group Length", "0", "0", "100",
    TRUE, 'g', ARG_INT, 0.0, 0, NULL},
};

Int2 Main (void)

{
  Int2     blocklen, grouplen, linelen;
  FILE     *ifp, *ofp;
  CharPtr  infile, outfile;

  /* standard setup */

  ErrSetFatalLevel (SEV_MAX);
  ErrClearOptFlags (EO_SHOW_USERSTR);
  ErrPathReset ();

  if (! GetArgs ("flint", sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  infile = (CharPtr) myargs [i_argInputFile].strvalue;
  outfile = (CharPtr) myargs [o_argOutputFile].strvalue;
  linelen = (Int2) myargs [e_argLineLength].intvalue;
  blocklen = (Int2) myargs [b_argBlockLength].intvalue;
  grouplen = (Int2) myargs [g_argGroupLength].intvalue;

  ifp = FileOpen (infile, "r");
  if (ifp == NULL) {
    Message (MSG_FATAL, "Unable to open input file");
    return 1;
  }

  ofp = FileOpen (outfile, "w");
  if (ofp == NULL) {
    Message (MSG_FATAL, "Unable to open output file");
    return 1;
  }

  ProcessFasta (ifp, ofp, linelen, blocklen, grouplen);

  FileClose (ofp);
  FileClose (ifp);

  return 0;
}

