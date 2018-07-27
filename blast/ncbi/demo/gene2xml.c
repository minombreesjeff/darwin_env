/*   gene2xml.c
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
* File Name:  gene2xml.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   1/27/05
*
* $Revision: 1.8 $
*
* File Description:
*
* Modifications:
* --------------------------------------------------------------------------
* ==========================================================================
*/

#include <ncbi.h>
#include <objall.h>
#include <objsset.h>
#include <objsub.h>
#include <objfdef.h>
#include <sqnutils.h>

#define NLM_GENERATED_CODE_PROTO
#include <objentgene.h>

#define GENE2XMLAPP_VER "1.1"

CharPtr GENE2XMLAPPLICATION = GENE2XMLAPP_VER;

NLM_EXTERN void AsnPrintNewLine PROTO((AsnIoPtr aip));

static Boolean CombineEG (
  AsnIoPtr aip,
  AsnIoPtr aop
)

{
  AsnTypePtr     atp_egs, atp_egse;
  EntrezgenePtr  egp;
  EntrezgeneSet  es;

  if (aip == NULL || aop == NULL) return FALSE;

  atp_egs = AsnFind ("Entrezgene-Set");
  if (atp_egs == NULL) {
    Message (MSG_POSTERR, "AsnFind failure on Entrezgene-Set");
    return FALSE;
  }

  atp_egse = AsnFind ("Entrezgene-Set.E");
  if (atp_egse == NULL) {
    Message (MSG_POSTERR, "AsnFind failure on Entrezgene-Set.E");
    return FALSE;
  }

  MemSet ((Pointer) &es, 0, sizeof (EntrezgeneSet));

  if (! AsnOpenStruct (aop, atp_egs, (Pointer) &es)) return FALSE;

  egp = EntrezgeneAsnRead (aip, NULL);
  while (egp != NULL) {
    EntrezgeneAsnWrite (egp, aop, atp_egse);
    EntrezgeneFree (egp);
    egp = EntrezgeneAsnRead (aip, NULL);
  }

  if (! AsnCloseStruct (aop, atp_egs, (Pointer) &es)) return FALSE;

  return TRUE;
}

static Boolean ExtractEG (
  AsnIoPtr aip,
  AsnIoPtr aop
)

{
  AsnModulePtr   amp;
  AsnTypePtr     atp, atp_egs, atp_egse;
  EntrezgenePtr  egp;

  if (aip == NULL || aop == NULL) return FALSE;

  amp = AsnAllModPtr ();
  if (amp == NULL) {
    Message (MSG_POSTERR, "Unable to load AsnAllModPtr");
    return 1;
  }

  atp_egs = AsnFind ("Entrezgene-Set");
  if (atp_egs == NULL) {
    Message (MSG_POSTERR, "AsnFind failure on Entrezgene-Set");
    return FALSE;
  }

  atp_egse = AsnFind ("Entrezgene-Set.E");
  if (atp_egse == NULL) {
    Message (MSG_POSTERR, "AsnFind failure on Entrezgene-Set.E");
    return FALSE;
  }

  atp = atp_egs;

  while ((atp = AsnReadId (aip, amp, atp)) != NULL) {
    if (atp == atp_egse) {
      egp = EntrezgeneAsnRead (aip, atp);
      EntrezgeneAsnWrite (egp, aop, NULL);
      EntrezgeneFree (egp);
      AsnPrintNewLine (aop);
      AsnIoFlush (aop);
      AsnIoReset (aop);
    } else {
      AsnReadVal (aip, atp, NULL);
    }
  }

  return TRUE;
}

static Boolean HasDesiredTaxId (
  EntrezgenePtr egp,
  Int4 taxid
)


{
  BioSourcePtr  biop;
  DbtagPtr      dbt;
  ObjectIdPtr   oip;
  OrgRefPtr     orp;
  ValNodePtr    vnp;

  if (egp == NULL) return FALSE;

  biop = (BioSourcePtr) egp->source;
  if (biop == NULL) return FALSE;
  orp = biop->org;
  if (orp == NULL) return FALSE;

  for (vnp = orp->db; vnp != NULL; vnp = vnp->next) {
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    if (dbt == NULL) continue;
    if (StringCmp (dbt->db, "taxon") != 0) continue;
    oip = dbt->tag;
    if (oip == NULL) continue;
    if (oip->id == taxid) return TRUE;
  }

  return FALSE;
}

static Boolean GenerateXML (
  AsnIoPtr aip,
  AsnIoPtr aop,
  Int4 taxid
)

{
  AsnModulePtr   amp;
  AsnTypePtr     atp, atp_egs, atp_egse;
  DataVal        dv;
  EntrezgenePtr  egp;

  if (aip == NULL || aop == NULL) return FALSE;

  amp = AsnAllModPtr ();
  if (amp == NULL) {
    Message (MSG_POSTERR, "Unable to load AsnAllModPtr");
    return 1;
  }

  atp_egs = AsnFind ("Entrezgene-Set");
  if (atp_egs == NULL) {
    Message (MSG_POSTERR, "AsnFind failure on Entrezgene-Set");
    return FALSE;
  }

  atp_egse = AsnFind ("Entrezgene-Set.E");
  if (atp_egse == NULL) {
    Message (MSG_POSTERR, "AsnFind failure on Entrezgene-Set.E");
    return FALSE;
  }

  atp = atp_egs;

  /*
  while ((atp = AsnReadId (aip, amp, atp)) != NULL) {
    AsnReadVal (aip, atp, &dv);
    AsnWrite (aop, atp, &dv);
    AsnKillValue (atp, &dv);
  }
  */

  while ((atp = AsnReadId (aip, amp, atp)) != NULL) {
    if (atp == atp_egse) {
      egp = EntrezgeneAsnRead (aip, atp);
      if (taxid == 0 || HasDesiredTaxId (egp, taxid)) {
        EntrezgeneAsnWrite (egp, aop, atp);
      }
      EntrezgeneFree (egp);
    } else {
      AsnReadVal (aip, atp, &dv);
      AsnWrite (aop, atp, &dv);
      AsnKillValue (atp, &dv);
    }
  }

  return TRUE;
}

static void ProcessOneRecord (
  CharPtr infile,
  CharPtr outfile,
  Int2 type,
  Boolean binary,
  Boolean compressed,
  Int4 taxid,
  Boolean zip
)

{
  AsnIoPtr  aip, aop;
  FILE      *fp;
  CharPtr   mode;
#ifdef OS_UNIX
  Char      cmmd [256];
  CharPtr   gzcatprog;
  CharPtr   gzipprog;
  int       ret;
  Boolean   usedPopen = FALSE;
#endif

#ifndef OS_UNIX
  if (compressed) {
    Message (MSG_POSTERR, "Can only decompress on-the-fly on UNIX machines");
    return;
  }
#endif

#ifdef OS_UNIX
  if (compressed) {
    gzcatprog = getenv ("NCBI_UNCOMPRESS_BINARY");
    if (gzcatprog != NULL) {
      sprintf (cmmd, "%s %s", gzcatprog, infile);
    } else {
      ret = system ("gzcat -h >/dev/null 2>&1");
      if (ret == 0) {
        sprintf (cmmd, "gzcat %s", infile);
      } else if (ret == -1) {
        Message (MSG_POSTERR, "Unable to fork or exec gzcat in ScanBioseqSetRelease");
        return;
      } else {
        ret = system ("zcat -h >/dev/null 2>&1");
        if (ret == 0) {
          sprintf (cmmd, "zcat %s", infile);
        } else if (ret == -1) {
          Message (MSG_POSTERR, "Unable to fork or exec zcat in ScanBioseqSetRelease");
          return;
        } else {
          Message (MSG_POSTERR, "Unable to find zcat or gzcat in ScanBioseqSetRelease - please edit your PATH environment variable");
          return;
        }
      }
    }
    fp = popen (cmmd, /* binary? "rb" : */ "r");
    usedPopen = TRUE;
  } else {
    fp = FileOpen (infile, binary? "rb" : "r");
  }
#else
  fp = FileOpen (infile, binary? "rb" : "r");
#endif
  if (fp == NULL) {
    Message (MSG_POSTERR, "FileOpen failed for input file '%s'", infile);
    return;
  }

  aip = AsnIoNew (binary? ASNIO_BIN_IN : ASNIO_TEXT_IN, fp, NULL, NULL, NULL);
  if (aip == NULL) {
    Message (MSG_ERROR, "AsnIoNew failed for input file '%s'", infile);
    return;
  }

  if (type == 1 && zip) {
    mode = "wb";
  } else if (type == 3) {
    mode = "wx";
  } else {
    mode = "w";
  }
  aop = AsnIoOpen (outfile, mode);
  if (aop == NULL) {
    AsnIoClose (aip);
    Message (MSG_POSTERR, "Unable to open output file");
    return;
  }

  /* process input file */

  switch (type) {
    case 1 :
      CombineEG (aip, aop);
      break;
    case 2 :
      ExtractEG (aip, aop);
      break;
    case 3 :
      GenerateXML (aip, aop, taxid);
      break;
    default :
      break;
  }

  AsnIoClose (aop);

  AsnIoFree (aip, FALSE);

#ifdef OS_UNIX
  if (usedPopen) {
    pclose (fp);
  } else {
    FileClose (fp);
  }
#else
  FileClose (fp);
#endif

#ifdef OS_UNIX
  if (zip) {
    gzipprog = getenv ("NCBI_COMPRESS_BINARY");
    if (gzipprog != NULL) {
      sprintf (cmmd, "%s -c %s > %s.gz", gzipprog, outfile, outfile);
    } else {
      ret = system ("gzip -h >/dev/null 2>&1");
      if (ret == 0) {
        sprintf (cmmd, "gzip -c %s > %s.gz", outfile, outfile);
        system (cmmd);
      } else if (ret == -1) {
        Message (MSG_POSTERR, "Unable to fork or exec gzip in gene2xml");
        return;
      }
    }
  }
#endif
}

static void FileRecurse (
  CharPtr directory,
  CharPtr results,
  CharPtr insuffix,
  CharPtr outsuffix,
  Int2 type,
  Boolean binary,
  Boolean compressed,
  Int4 taxid,
  Boolean zip,
  Boolean log
)

{
  Char        infile [FILENAME_MAX], inpath [PATH_MAX],
              outfile [FILENAME_MAX], outpath [PATH_MAX],
              path [PATH_MAX];
  CharPtr     ptr, str;
  ValNodePtr  head, vnp;

  if (insuffix == NULL) {
    insuffix = "";
  }
  if (outsuffix == NULL) {
    outsuffix = "";
  }

  /* get list of all files in source directory */

  head = DirCatalog (directory);

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == 0) {
      str = (CharPtr) vnp->data.ptrvalue;
      if (! StringHasNoText (str)) {

        /* does filename have desired substring? */

        ptr = StringStr (str, insuffix);
        if (ptr != NULL) {
          *ptr = '\0';

          /* process file that has desired input suffix (.ags or .agc) */

          StringNCpy_0 (inpath, directory, sizeof (inpath));
          sprintf (infile, "%s%s", str, insuffix);
          FileBuildPath (inpath, NULL, infile);

          if (StringDoesHaveText (results)) {
            StringNCpy_0 (outpath, results, sizeof (outpath));
          } else {
            StringNCpy_0 (outpath, directory, sizeof (outpath));
          }
          sprintf (outfile, "%s%s", str, outsuffix);
          FileBuildPath (outpath, NULL, outfile);

          if (log) {
            Message (MSG_POST, "Processing %s", infile);
          }

          ProcessOneRecord (inpath, outpath, type, binary, compressed, taxid, zip);
        }
      }
    } else if (vnp->choice == 1) {

      /* recurse into subdirectory */

      StringNCpy_0 (path, directory, sizeof (path));
      str = (CharPtr) vnp->data.ptrvalue;
      FileBuildPath (path, str, NULL);
      FileRecurse (path, results, insuffix, outsuffix, type, binary, compressed, taxid, zip, log);
    }
  }

  /* clean up file list */

  ValNodeFreeData (head);
}

/* Args structure contains command-line arguments */

#define p_argInputPath   0
#define r_argOutputPath  1
#define i_argInputFile   2
#define o_argOutputFile  3
#define b_argBinary      4
#define c_argCompressed  5
#define t_argTaxonID     6
#define x_argExtract     7
#define y_argCombine     8
#define z_argZipResult   9
#define l_argLog        10


Args myargs [] = {
  {"Path to Files", NULL, NULL, NULL,
    TRUE, 'p', ARG_STRING, 0.0, 0, NULL},
  {"Path for Results", NULL, NULL, NULL,
    TRUE, 'r', ARG_STRING, 0.0, 0, NULL},
  {"Single Input File", "stdin", NULL, NULL,
    TRUE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  {"Single Output File", "stdout", NULL, NULL,
    TRUE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
  {"File is Binary", "F", NULL, NULL,
    TRUE, 'b', ARG_BOOLEAN, 0.0, 0, NULL},
  {"File is Compressed", "F", NULL, NULL,
    TRUE, 'c', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Taxon ID to Filter", "0", NULL, NULL,
    TRUE, 't', ARG_INT, 0.0, 0, NULL},
  {"Extract .ags -> text .agc", "F", NULL, NULL,
    TRUE, 'x', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Combine .agc -> text .ags (for testing)", "F", NULL, NULL,
    TRUE, 'y', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Combine .agc -> binary .ags, then gzip", "F", NULL, NULL,
    TRUE, 'z', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Log Processing", "F", NULL, NULL,
    TRUE, 'l', ARG_BOOLEAN, 0.0, 0, NULL},
};

Int2 Main (void)

{
  Char     app [64], xmlbuf [128];
  Boolean  binary, compressed, combine, log, extract, zip;
  CharPtr  directory, infile, outfile, results, insuffix, outsuffix;
  Int4     taxid;
  Int2     type = 0;

  /* standard setup */

  ErrSetFatalLevel (SEV_MAX);
  ErrClearOptFlags (EO_SHOW_USERSTR);
  ErrSetLogfile ("stderr", ELOG_APPEND);
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
  if (! FeatDefSetLoad ()) {
    Message (MSG_FATAL, "FeatDefSetLoad failed");
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

  /* process command line arguments */

  sprintf (app, "gene2xml %s", GENE2XMLAPPLICATION);
  if (! GetArgs (app, sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  /* additional setup modifications */

  if (! objentgeneAsnLoad ()) {
    Message (MSG_POSTERR, "objentgeneAsnLoad failed");
    return 1;
  }

  if (GetAppParam ("NCBI", "SETTINGS", "XMLPREFIX", NULL, xmlbuf, sizeof (xmlbuf))) {
    AsnSetXMLmodulePrefix (StringSave (xmlbuf));
  }

  directory = (CharPtr) myargs [p_argInputPath].strvalue;
  results = (CharPtr) myargs [r_argOutputPath].strvalue;
  if (StringHasNoText (results)) {
    results = NULL;
  }
  infile = (CharPtr) myargs [i_argInputFile].strvalue;
  outfile = (CharPtr) myargs [o_argOutputFile].strvalue;

  binary = (Boolean) myargs [b_argBinary].intvalue;
  compressed = (Boolean) myargs [c_argCompressed].intvalue;
  taxid = (Int4) myargs [t_argTaxonID].intvalue;

  combine = (Boolean) myargs [y_argCombine].intvalue;
  extract = (Boolean) myargs [x_argExtract].intvalue;
  zip = (Boolean) myargs [z_argZipResult].intvalue;
  log = (Boolean) myargs [l_argLog].intvalue;

  if (zip) {
    if ((! combine) && (! extract)) {
      combine = TRUE;
    }
  }

  if (combine) {
    type = 1;
    insuffix = ".agc";
    outsuffix = ".ags";
  } else if (extract) {
    type = 2;
    insuffix = ".ags";
    outsuffix = ".agc";
  } else {
    type = 3;
    insuffix = ".ags";
    outsuffix = ".xgs";
  }

  /* process input file */

  if (StringDoesHaveText (directory)) {

    FileRecurse (directory, results, insuffix, outsuffix, type, binary, compressed, taxid, zip, log);

  } else if (StringDoesHaveText (infile) && StringDoesHaveText (outfile)) {

    ProcessOneRecord (infile, outfile, type, binary, compressed, taxid, zip);
  }

  return 0;
}

