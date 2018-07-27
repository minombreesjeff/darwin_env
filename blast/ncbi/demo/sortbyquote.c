/*   sortbyquote.c
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
* File Name:  sortbyquote.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   10/2/04
*
* $Revision: 1.2 $
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

#include <ncbi.h>

static void CopyQuotedPart (
  CharPtr str,
  CharPtr buf
)

{
  Char     ch;
  CharPtr  ptr;

  if (buf == NULL) return;
  *buf = '\0';
  if (StringHasNoText (str)) return;

  ptr = StringChr (str, '"');
  if (ptr == NULL) return;
  StringCpy (buf, ptr + 1);

  ptr = StringChr (buf, '"');
  if (ptr == NULL) return;
  *ptr = '\0';

  ptr = buf;
  ch = *ptr;
  while (ch != '\0') {
    if (IS_UPPER (ch)) {
      *ptr = TO_LOWER (ch);
    }
    ptr++;
    ch = *ptr;
  }
}

static int QuotedStringCmp (
  CharPtr str1,
  CharPtr str2
)

{
  Char  buf1 [512];
  Char  buf2 [512];

  CopyQuotedPart (str1, buf1);
  CopyQuotedPart (str2, buf2);

  return StringCmp (buf1, buf2);
}

static int LIBCALLBACK SortStringByQuoted (
  VoidPtr ptr1,
  VoidPtr ptr2
)

{
  CharPtr     str1, str2;
  ValNodePtr  vnp1, vnp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;
  str1 = (CharPtr) vnp1->data.ptrvalue;
  str2 = (CharPtr) vnp2->data.ptrvalue;
  if (str1 == NULL || str2 == NULL) return 0;

  return QuotedStringCmp (str1, str2);
}

static void ProcessFileToSort (
  FILE *ifp,
  FILE *ofp
)


{
  FileCache   fc;
  ValNodePtr  head = NULL;
  Char        line [512];
  CharPtr     str;
  ValNodePtr  vnp;

  if (ifp == NULL || ofp == NULL) return;

  FileCacheSetup (&fc, ifp);

  str = FileCacheReadLine (&fc, line, sizeof (line), NULL);
  if (str == NULL) return;

  while (str != NULL) {
    if (StringDoesHaveText (str)) {
      ValNodeCopyStr (&head, 0, str);
    }
    str = FileCacheReadLine (&fc, line, sizeof (line), NULL);
  }
  if (head == NULL) return;

  head = ValNodeSort (head, SortStringByQuoted);

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    if (StringHasNoText (str)) continue;
    fprintf (ofp, "%s\n", str);
  }

  ValNodeFreeData (head);
}

/* Args structure contains command-line arguments */

#define i_argInputFile   0
#define o_argOutputFile  1

Args myargs [] = {
  {"Input File Name", "stdin", NULL, NULL,
    FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  {"Output File Name", "stdout", NULL, NULL,
    FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
};

Int2 Main (void)

{
  FILE     *ifp, *ofp;
  CharPtr  infile, outfile;

  ErrSetFatalLevel (SEV_MAX);
  ErrClearOptFlags (EO_SHOW_USERSTR);
  ErrPathReset ();

  /* process command line arguments */

  if (! GetArgs ("sortbyquote", sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  infile = (CharPtr) myargs [i_argInputFile].strvalue;
  outfile = (CharPtr) myargs [o_argOutputFile].strvalue;

  if (StringHasNoText (infile)) {
    Message (MSG_FATAL, "No input file");
    return 1;
  }

  if (StringHasNoText (outfile)) {
    Message (MSG_FATAL, "No output file");
    return 1;
  }

  ifp = FileOpen (infile, "r");
  if (ifp == NULL) {
    Message (MSG_FATAL, "Unable to open input file");
    return 1;
  }

  ofp = FileOpen (outfile, "w");
  if (ofp == NULL) {
    Message (MSG_FATAL, "Unable to open output file");
    FileClose (ifp);
    return 1;
  }

  ProcessFileToSort (ifp, ofp);

  FileClose (ofp);
  FileClose (ifp);

  return 0;
}

