/*   cleanasn.c
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
* File Name:  cleanasn.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   10/19/99
*
* $Revision: 6.1 $
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
#include <objall.h>
#include <objsset.h>
#include <objsub.h>
#include <sequtil.h>
#include <sqnutils.h>
#include <toasn3.h>

static void CleanupOneRecord (CharPtr directory, CharPtr filename)

{
  AsnIoPtr     aip;
  Pointer      dataptr;
  Uint2        datatype;
  Uint2        entityID;
  FILE*        fp;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep;

  Message (MSG_POST, "%s\n", filename);

  StringNCpy_0 (path, directory, sizeof (path));
  FileBuildPath (path, NULL, filename);

  fp = FileOpen (path, "r");
  if (fp == NULL) return;

  dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, &entityID, FALSE, FALSE, TRUE, FALSE);

  FileClose (fp);

  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep != NULL) {
    SeriousSeqEntryCleanup (sep, NULL, NULL);

    aip = AsnIoOpen (path, "w");
    if (aip != NULL) {
      if (datatype == OBJ_SEQSUB) {
        SeqSubmitAsnWrite ((SeqSubmitPtr) dataptr, aip, NULL);
      } else {
        SeqEntryAsnWrite (sep, aip, NULL);
      }
      AsnIoFlush (aip);
      AsnIoClose (aip);
    }
  }

  ObjMgrFreeByEntityID (entityID);
}

Args myargs [] = {
  {"Path to files", NULL, NULL, NULL,
    FALSE, 'p', ARG_STRING, 0.0, 0, NULL}
};

Int2 Main (void)

{
  ValNodePtr  dir, vnp;

  ErrSetFatalLevel (SEV_MAX);
  ErrClearOptFlags (EO_SHOW_USERSTR);
  UseLocalAsnloadDataAndErrMsg ();
  ErrPathReset ();

  if (! AllObjLoad ()) {
    Message (MSG_FATAL, "AllObjLoad failed");
    return 1;
  }
  if (! SubmitAsnLoad ()) {
    Message (MSG_FATAL, "SubmitAsnLoad failed");
    return 1;
  }
  if (! SeqCodeSetLoad ()) {
    Message (MSG_FATAL, "SeqCodeSetLoad failed");
    return 1;
  }
  if (! GeneticCodeTableLoad ()) {
    Message (MSG_FATAL, "GeneticCodeTableLoad failed");
    return 1;
  }

  if (! GetArgs ("cleanasn", sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }
  if (StringHasNoText (myargs [0].strvalue)) {
    return 0;
  }

  dir = DirCatalog ((CharPtr) myargs [0].strvalue);
  for (vnp = dir; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == 0) {
      CleanupOneRecord (myargs [0].strvalue, (CharPtr) vnp->data.ptrvalue);
    }
  }
  ValNodeFreeData (dir);

  return 0;
}

