/*   releasescan.c
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
* File Name:  releasescan.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   6/6/00
*
* $Revision: 6.2 $
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

/* scans binary ASN.1 Bioseq-set release files */

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
#include <toasn3.h>
#include <explore.h>

static Boolean LIBCALLBACK DoFeat (SeqFeatPtr sfp, SeqMgrFeatContextPtr context)

{
  DbtagPtr     dbt;
  GBQualPtr    gbq;
  GeneRefPtr   grp;
  CharPtr      label;
  ObjectIdPtr  oip;
  BioseqPtr    prod;
  SeqFeatPtr   prot;
  ProtRefPtr   prp;
  RnaRefPtr    rrp;
  SeqIdPtr     sip;
  SeqLocPtr    slp;
  Int4         start;
  Int4         stop;
  Char         str [256];
  tRNAPtr      trna;
  ValNodePtr   vnp;

  FILE  *fp;

  fp = (FILE *) context->userdata;

  label = (CharPtr) FeatDefTypeLabel (sfp);
  if (StringCmp (label, "Gene") == 0) {
    label = "gene";
  }
  if (StringHasNoText (label)) {
    label = "???";
  }

  slp = SeqLocFindNext (sfp->location, NULL);
  if (slp == NULL) return TRUE;

  start = GetOffsetInBioseq (slp, context->bsp, SEQLOC_START) + 1;
  stop = GetOffsetInBioseq (slp, context->bsp, SEQLOC_STOP) + 1;
  fprintf (fp, "%ld\t%ld\t%s\n", (long) start, (long) stop, label);

  while ((slp = SeqLocFindNext (sfp->location, slp)) != NULL) {
    start = GetOffsetInBioseq (slp, context->bsp, SEQLOC_START) + 1;
    stop = GetOffsetInBioseq (slp, context->bsp, SEQLOC_STOP) + 1;
    if (start != 0 && stop != 0) {
      fprintf (fp, "%ld\t%ld\n", (long) start, (long) stop);
    }
  }

  switch (context->seqfeattype) {
    case SEQFEAT_GENE :
      grp = (GeneRefPtr) sfp->data.value.ptrvalue;
      if (grp != NULL) {
        StringNCpy_0 (str, (CharPtr) grp->locus, sizeof (str));
        if (! StringHasNoText (str)) {
          fprintf (fp, "\t\t\tgene\t%s\n", str);
        }
        for (vnp = grp->syn; vnp != NULL; vnp = vnp->next) {
          StringNCpy_0 (str, (CharPtr) vnp->data.ptrvalue, sizeof (str));
          if (! StringHasNoText (str)) {
            fprintf (fp, "\t\t\tgene_syn\t%s\n", str);
          }
        }
      }
      break;
    case SEQFEAT_CDREGION :
      prod = BioseqFind (SeqLocId (sfp->product));
      prot = SeqMgrGetBestProteinFeature (prod, NULL);
      if (prot != NULL) {
        prp = (ProtRefPtr) prot->data.value.ptrvalue;
        if (prp != NULL) {
          if (prp->name != NULL) {
            for (vnp = prp->name; vnp != NULL; vnp = vnp->next) {
              StringNCpy_0 (str, (CharPtr) vnp->data.ptrvalue, sizeof (str));
              if (! StringHasNoText (str)) {
                fprintf (fp, "\t\t\tproduct\t%s\n", str);
              }
            }
          } else if (prp->desc != NULL) {
            StringNCpy_0 (str, prp->desc, sizeof (str));
            if (! StringHasNoText (str)) {
              fprintf (fp, "\t\t\tproduct\t%s\n", str);
            }
          }
          for (vnp = prp->activity; vnp != NULL; vnp = vnp->next) {
            StringNCpy_0 (str, (CharPtr) vnp->data.ptrvalue, sizeof (str));
            if (! StringHasNoText (str)) {
              fprintf (fp, "\t\t\tfunction\t%s\n", str);
            }
          }
          for (vnp = prp->ec; vnp != NULL; vnp = vnp->next) {
            StringNCpy_0 (str, (CharPtr) vnp->data.ptrvalue, sizeof (str));
            if (! StringHasNoText (str)) {
              fprintf (fp, "\t\t\tEC_number\t%s\n", str);
            }
          }
        }
      }
      if (prod != NULL) {
        for (sip = prod->id; sip != NULL; sip = sip->next) {
          if (sip->choice == SEQID_GENBANK ||
              sip->choice == SEQID_EMBL ||
              sip->choice == SEQID_DDBJ) {
            if (SeqIdWrite (sip, str, PRINTID_TEXTID_ACC_VER, sizeof (str)) != NULL) {
              fprintf (fp, "\t\t\tprotein_id\t%s\n", str);
            }
          }
        }
      }
      break;
    case SEQFEAT_RNA :
      rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
      if (rrp != NULL) {
        switch (rrp->ext.choice) {
          case 1 :
            StringNCpy_0 (str, (CharPtr) rrp->ext.value.ptrvalue, sizeof (str));
            if (! StringHasNoText (str)) {
              fprintf (fp, "\t\t\tproduct\t%s\n", str);
            }
            break;
          case 2 :
            trna = rrp->ext.value.ptrvalue;
            if (trna != NULL) {
              FeatDefLabel (sfp, str, sizeof (str) - 1, OM_LABEL_CONTENT);
              if (! StringHasNoText (str)) {
                fprintf (fp, "\t\t\tproduct\t%s\n", str);
              }
            }
            break;
          default :
            break;
        }
      }
      break;
    default :
      break;
  }
  if (! StringHasNoText (sfp->comment)) {
    fprintf (fp, "\t\t\tnote\t%s\n", sfp->comment);
  }
  for (gbq = sfp->qual; gbq != NULL; gbq = gbq->next) {
    if (! StringHasNoText (gbq->qual)) {
      if (! StringHasNoText (gbq->val)) {
        fprintf (fp, "\t\t\t%s\t%s\n", gbq->qual, gbq->val);
      }
    }
  }
  for (vnp = sfp->dbxref; vnp != NULL; vnp = vnp->next) {
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    if (dbt != NULL) {
      if (! StringHasNoText (dbt->db)) {
        oip = dbt->tag;
        if (oip->str != NULL && (! StringHasNoText (oip->str))) {
          fprintf (fp, "\t\t\tdb_xref\t%s:%s\n", dbt->db, oip->str);
        } else {
          fprintf (fp, "\t\t\tdb_xref\t%s:%ld\n", dbt->db, (long) oip->id);
        }
      }
    }
  }

  return TRUE;
}

static void DoBioseq (BioseqPtr bsp, Pointer userdata)

{
  FILE     *fp;
  Char     str [41];
  CharPtr  tmp;
  ValNode  vn;

  /* do not process protein bioseqs here */

  if (! ISA_na (bsp->mol)) return;

  fp = (FILE *) userdata;

  if (bsp->repr == Seq_repr_seg) {

    /* print FASTA ID chain and SeqLoc for segmented bioseq */

    MemSet ((Pointer) &vn, 0, sizeof (ValNode));
    vn.choice = SEQLOC_MIX;
    vn.data.ptrvalue = bsp->seq_ext;
    tmp = SeqLocPrint ((SeqLocPtr) &vn);
    if (tmp != NULL) {
      SeqIdWrite (bsp->id, str, PRINTID_FASTA_LONG, sizeof (str));
      fprintf (fp, ">%s %s\n", str, tmp);
    }
    MemFree (tmp);

  } else {

    /* normal FASTA output, including for raw parts of segmented bioseq */

    BioseqToFasta (bsp, fp, ISA_na (bsp->mol));
  }

  /* features on parts are indexed on segmented parent coordinates */

  if (SeqMgrGetParentOfPart (bsp, NULL) != NULL) return;

  /* visit features indexed on this bioseq */

  SeqMgrExploreFeatures (bsp, userdata, DoFeat, NULL, NULL, NULL);
}

static void DoRecord (SeqEntryPtr sep, Pointer userdata)

{
  /* index features on all bioseqs in current record */

  SeqMgrIndexFeatures (0, sep->data.ptrvalue);

  /* explore record, visit every bioseq */

  VisitBioseqsInSep (sep, userdata, DoBioseq);

  /* record cleaned up by ScanBioseqSetRelease */
}

/* command-line argument list */

#define p_argInputPath  0
#define o_argOutputFile 1
#define x_argFileSelect 2
#define b_argBinaryFile 3
#ifdef OS_UNIX
#define c_argCompressed 4
#endif

Args myargs [] = {
  {"Path to files", NULL, NULL, NULL,
    TRUE, 'p', ARG_STRING, 0.0, 0, NULL},
  {"Output File Name", "stdout", NULL, NULL,
    FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
  {"File selection substring", ".aso", NULL, NULL,
    TRUE, 'x', ARG_STRING, 0.0, 0, NULL},
  {"Binary file", "T", NULL, NULL,
    TRUE, 'b', ARG_BOOLEAN, 0.0, 0, NULL},
#ifdef OS_UNIX
  {"Compressed file", "F", NULL, NULL,
    TRUE, 'c', ARG_BOOLEAN, 0.0, 0, NULL},
#endif
};

/* toolkit ncbimain.c has C main function, wraps application Main */

Int2 Main (void)

{
  Boolean     binary, compressed = FALSE;
  CharPtr     dir, progname, str, subfile;
  FILE        *fp;
  ValNodePtr  head, vnp;
  Char        path [PATH_MAX];

  ErrSetFatalLevel (SEV_FATAL);
  ErrClearOptFlags (EO_SHOW_USERSTR);
  UseLocalAsnloadDataAndErrMsg ();
  ErrPathReset ();

  /* resolve internal pointers in object loader parse tables */

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

  ProgramPath (path, sizeof (path));
  progname = StringRChr (path, DIRDELIMCHR);
  if (progname != NULL) {
    progname++;
  } else {
    progname = "releasescan";
  }

  /* process command-line arguments */

  if (! GetArgs (progname, sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  dir = myargs [p_argInputPath].strvalue;
  binary = (Boolean) myargs [b_argBinaryFile].intvalue;
#ifdef OS_UNIX
  compressed = (Boolean) myargs [c_argCompressed].intvalue;
#endif

#ifndef OS_UNIX
  if (compressed) {
    Message (MSG_ERROR, "Can only decompress on-the-fly on UNIX machines");
    return 1;
  }
#endif

  fp = FileOpen (myargs [o_argOutputFile].strvalue, "a");
  if (fp == NULL) {
    Message (MSG_FATAL, "FileOpen failed");
    return 1;
  }

  head = DirCatalog (dir);

  /* process appropriate files within specified directory */

  for (vnp = head; vnp != NULL; vnp = vnp->next) {

    /* vnp->choice is 0 for file, 1 for subdirectory */

    if (vnp->choice == 0) {
      str = (CharPtr) vnp->data.ptrvalue;
      if (! StringHasNoText (str)) {
        subfile = myargs [x_argFileSelect].strvalue;

        /* does filename have desired substring? */

        if (StringHasNoText (subfile) || StringStr (str, subfile) != NULL) {
#ifdef OS_UNIX
          /* printf ("%s\n", str); */
#endif

          /* open a file, read one record at a time, present it to callback */

          StringNCpy_0 (path, dir, sizeof (path));
          FileBuildPath (path, NULL, str);

          ScanBioseqSetRelease (path, binary, compressed, (Pointer) fp, DoRecord);
        }
      }
    }
  }

  ValNodeFreeData (head);

  FileClose (fp);

  return 0;
}

