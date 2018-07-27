/*   asn2gb.c
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
* File Name:  asn2gb.c
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov, Jonathan Kans
*
* Version Creation Date:   10/21/98
*
* $Revision: 6.16 $
*
* File Description:  New GenBank flatfile generator application
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
#include <seqport.h>
#include <sequtil.h>
#include <sqnutils.h>
#include <subutil.h>
#include <tofasta.h>
#include <explore.h>
#include <ffprint.h>
#include <utilpub.h>
#include <jzmisc.h>
#include <gbfeat.h>
#include <gbftdef.h>
#include <edutil.h>
#include <asn2gnbp.h>

NLM_EXTERN Boolean SeqEntryToFlat (
  SeqEntryPtr sep,
  FILE *fp,
  Uint1 format,
  Uint1 mode
);

extern void SeriousSeqEntryCleanup (
  SeqEntryPtr sep,
  SeqEntryFunc taxfun,
  SeqEntryFunc taxmerge
);

extern void SeriousSeqEntryCleanupBulk (
  SeqEntryPtr sep
);

static void SaveSeqEntry (
  SeqEntryPtr sep,
  CharPtr filename
)

{
  AsnIoPtr  aop;

  if (sep == NULL) return;
  aop = AsnIoOpen (filename, "w");
  if (aop != NULL) {
    SeqEntryAsnWrite (sep, aop, NULL);
  }
  AsnIoClose (aop);
}

static void SaveAsn2gnbk (
  SeqEntryPtr sep,
  CharPtr filename,
  FmtType format,
  ModType mode,
  StlType style,
  FlgType flags
)

{
  FILE  *fp;

  if (sep == NULL) return;
  fp = FileOpen (filename, "w");
  if (fp != NULL) {
    SeqEntryToGnbk (sep, NULL, format, mode, style, flags, fp);
  }
  FileClose (fp);
}

typedef struct oldffdata {
  FILE   *fp;
  Uint1  format;
  Uint1  mode;
} OldFFData, PNTR OldFFPtr;

static void DoOneFlatFile (
  SeqEntryPtr sep,
  Pointer userdata
)

{
  OldFFPtr  ofp;

  ofp = (OldFFPtr) userdata;
  SeqEntryToFlat (sep, ofp->fp, ofp->format, ofp->mode);
}

static void SaveAsn2ff (
  SeqEntryPtr sep,
  CharPtr filename,
  FmtType format,
  ModType mode
)

{
  OldFFData  ofd;

  if (sep == NULL) return;
  MemSet ((Pointer) &ofd, 0, sizeof (OldFFData));
  ofd.fp = FileOpen (filename, "w");
  if (ofd.fp != NULL) {
    switch (format) {
      case GENBANK_FMT :
        ofd.format = 0;
        break;
      case EMBL_FMT :
        if (mode == RELEASE_MODE || mode == ENTREZ_MODE) {
          ofd.format = 1;
        } else {
          ofd.format = 3;
        }
        break;
      case GENPEPT_FMT :
        ofd.format = 2;
        break;
      default :
        ofd.format = 0;
        break;
    }
    switch (mode) {
      case RELEASE_MODE :
        ofd.mode = 6;
        break;
      case SEQUIN_MODE :
        ofd.mode = 8;
        break;
      default :
        ofd.mode = 7;
        break;
    }
    VisitElementsInSep (sep, (Pointer) &ofd, DoOneFlatFile);
  }
  FileClose (ofd.fp);
}

static void GetFirstGoodBioseq (
  BioseqPtr bsp,
  Pointer userdata
)

{
  BioseqPtr PNTR bspp;

  bspp = (BioseqPtr PNTR) userdata;
  if (*bspp != NULL) return;
  *bspp = bsp;
}

static Int2 HandleSingleRecord (
  CharPtr inputFile,
  CharPtr outputFile,
  FmtType format,
  ModType mode,
  StlType style,
  FlgType flags,
  Int4 from,
  Int4 to,
  Uint1 strand,
  Boolean both,
  Boolean nocleanup,
  Boolean justSSEC
)

{
  AsnIoPtr      aip;
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Pointer       dataptr;
  Uint2         datatype;
  Uint2         entityID;
  FILE          *fp;
  OldFFData     ofd;
  SeqEntryPtr   sep;
  SeqInt        sint;
  SeqLocPtr     slp = NULL;
  ValNode       vn;

  fp = FileOpen (inputFile, "r");
  if (fp == NULL) {
    Message (MSG_FATAL, "FileOpen failed for input file '%s'", inputFile);
    return 1;
  }

  dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, FALSE, FALSE, FALSE);

  FileClose (fp);

  entityID = ObjMgrRegister (datatype, dataptr);
  if (datatype == OBJ_SEQSUB || datatype == OBJ_SEQENTRY ||
        datatype == OBJ_BIOSEQ || datatype == OBJ_BIOSEQSET) {

/*
#ifdef WIN_MAC
#if __profile__
    ProfilerSetStatus (TRUE);
#endif
#endif
*/

    entityID = SeqMgrIndexFeatures (entityID, NULL);

/*
#ifdef WIN_MAC
#if __profile__
    ProfilerSetStatus (FALSE);
#endif
#endif
*/

    sep = GetTopSeqEntryForEntityID (entityID);

    if (sep == NULL) {
      sep = SeqEntryNew ();
      if (sep != NULL) {
        if (datatype == OBJ_BIOSEQ) {
          bsp = (BioseqPtr) dataptr;
          sep->choice = 1;
          sep->data.ptrvalue = bsp;
          SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, sep);
        } else if (datatype == OBJ_BIOSEQSET) {
          bssp = (BioseqSetPtr) dataptr;
          sep->choice = 2;
          sep->data.ptrvalue = bssp;
          SeqMgrSeqEntry (SM_BIOSEQSET, (Pointer) bssp, sep);
        } else {
          sep = SeqEntryFree (sep);
        }
      }
      sep = GetTopSeqEntryForEntityID (entityID);
    }

    if (sep != NULL) {
      FileRemove (outputFile);
#ifdef WIN_MAC
      FileCreate (outputFile, "TEXT", "ttxt");
#endif
      if (justSSEC) {
        aip = AsnIoOpen (outputFile, "w");
        if (aip != NULL) {
          SeriousSeqEntryCleanupBulk (sep);
          SeqEntryAsnWrite (sep, aip, NULL);
          AsnIoClose (aip);
        }
      } else {

        fp = FileOpen (outputFile, "w");
        if (both) {
          if (! nocleanup) {
            SeriousSeqEntryCleanup (sep, NULL, NULL);
          }
          fprintf (fp, "asn2ff:\n");
          MemSet ((Pointer) &ofd, 0, sizeof (OldFFData));
          ofd.fp = fp;
          switch (format) {
            case GENBANK_FMT :
              ofd.format = 0;
              break;
            case EMBL_FMT :
              if (mode == RELEASE_MODE || mode == ENTREZ_MODE) {
                ofd.format = 1;
              } else {
                ofd.format = 3;
              }
              break;
            case GENPEPT_FMT :
              ofd.format = 2;
              break;
            default :
              ofd.format = 0;
              break;
          }
          switch (mode) {
            case RELEASE_MODE :
              ofd.mode = 6;
              break;
            case SEQUIN_MODE :
              ofd.mode = 8;
              break;
            default :
              ofd.mode = 7;
              break;
          }
          VisitElementsInSep (sep, (Pointer) &ofd, DoOneFlatFile);
          fprintf (fp, "asn2gnbk:\n");
        }

        if (from > 0 && to > 0) {
          bsp = NULL;
          if (format == GENPEPT_FMT) {
            VisitSequencesInSep (sep, (Pointer) &bsp, VISIT_PROTS, GetFirstGoodBioseq);
          } else {
            VisitSequencesInSep (sep, (Pointer) &bsp, VISIT_NUCS, GetFirstGoodBioseq);
          }
          if (bsp != NULL) {
            MemSet ((Pointer) &vn, 0, sizeof (ValNode));
            MemSet ((Pointer) &sint, 0, sizeof (SeqInt));
            sint.from = from - 1;
            sint.to = to - 1;
            sint.strand = strand;
            sint.id = bsp->id;
            vn.choice = SEQLOC_INT;
            vn.data.ptrvalue = (Pointer) &sint;
            slp = &vn;
          }
        }

        SeqEntryToGnbk (sep, slp, format, mode, style, flags, fp);
        FileClose (fp);
      }
    }
  } else {
    Message (MSG_FATAL, "Datatype %d not recognized", (int) datatype);
  }
  ObjMgrFree (datatype, dataptr);

  return 0;
}

typedef struct hasgidata {
  Int4     gi;
  CharPtr  accn;
  Boolean  found;
} HasGiData, PNTR HasGiPtr;

static void LookForGi (
  SeqEntryPtr sep,
  Pointer mydata,
  Int4 index,
  Int2 indent
)

{
  BioseqPtr     bsp;
  HasGiPtr      hgp;
  SeqIdPtr      sip;
  TextSeqIdPtr  tsip;

  if (sep == NULL) return;
  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;
  hgp = (HasGiPtr) mydata;
  if (hgp == NULL) return;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    switch (sip->choice) {
      case SEQID_GI :
        if (sip->data.intvalue == hgp->gi) {
          hgp->found = TRUE;
          return;
        }
        break;
      case SEQID_GENBANK :
      case SEQID_EMBL :
      case SEQID_PIR :
      case SEQID_SWISSPROT :
      case SEQID_OTHER :
      case SEQID_DDBJ :
      case SEQID_PRF :
      case SEQID_TPG :
      case SEQID_TPE :
      case SEQID_TPD :
        tsip = (TextSeqIdPtr) sip->data.ptrvalue;
        if (tsip != NULL && hgp->accn!= NULL &&
            StringICmp (tsip->accession, hgp->accn) == 0) {
          hgp->found = TRUE;
          return;
        }
        break;
      default :
        break;
    }
  }
}

static Boolean SeqEntryHasGi (
  SeqEntryPtr sep,
  CharPtr accn
)

{
  HasGiData  hgd;
  long int   val;

  if (sep == NULL || StringHasNoText (accn)) return FALSE;
  MemSet ((Pointer) &hgd, 0, sizeof (HasGiData));
  if (sscanf (accn, "%ld", &val) == 1) {
    hgd.gi = (Int4) val;
  } else {
    hgd.accn = accn;
  }
  hgd.found = FALSE;
  SeqEntryExplore (sep, (Pointer) (&hgd), LookForGi);
  return hgd.found;
}

/* list of gi numbers that take astronomical time for asn2ff, exclude */

static Int4  badGiList [] = {
  6538453, 12659450, 12959181, 12959863, 12960545, 12961227, 13448054, 13735857, 0
};

static void LookForBadGis (
  BioseqPtr bsp,
  Pointer userdata
)

{
  BoolPtr   badFound;
  Int4      gi;
  Int2      idx;
  SeqIdPtr  sip;

  badFound = (BoolPtr) userdata;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_GI) {
      gi = sip->data.intvalue;
      if (gi != 0) {
        for (idx = 0; badGiList [idx] != 0; idx++) {
          if (gi == badGiList [idx]) {
            *badFound = TRUE;
          }
        }
      }
    }
  }
}

static Boolean SeqEntryNotInExcluded (
  SeqEntryPtr sep
)

{
  Boolean  badFound = FALSE;

  if (sep == NULL) return TRUE;
  VisitBioseqsInSep (sep, (Pointer) &badFound, LookForBadGis);
  if (badFound) return FALSE;
  return TRUE;
}

static void FreeUnpubAffil (
  PubdescPtr pdp,
  Pointer userdata
)

{
  AuthListPtr  alp;
  CitGenPtr    cgp;
  ValNodePtr   vnp;

  if (pdp == NULL) return;
  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice != PUB_Gen) continue;
    cgp = (CitGenPtr) vnp->data.ptrvalue;
    if (cgp == NULL) continue;
    if (cgp->cit != NULL) {
      if (StringNICmp (cgp->cit, "submitted", 8) == 0 ||
                       StringNICmp (cgp->cit, "unpublished", 11) == 0 ||
                       StringNICmp (cgp->cit, "in press", 8) == 0 ||
                       StringNICmp (cgp->cit, "to be published", 15) == 0) {
        cgp->cit = MemFree (cgp->cit);
        cgp->cit = StringSave ("Unpublished");
      }
    }
    alp = cgp->authors;
    if (alp == NULL) continue;
    alp->affil = AffilFree (alp->affil);
  }
}

static void LookForRefSeq (
  BioseqPtr bsp,
  Pointer userdata
)

{
  BoolPtr   hasRefseqP;
  SeqIdPtr  sip;

  hasRefseqP = (BoolPtr) userdata;
  if (*hasRefseqP) return;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_OTHER) {
      *hasRefseqP = TRUE;
      return;
    }
  }
}

static void CompareFlatFiles (
  CharPtr path1,
  CharPtr path2,
  CharPtr path3,
  SeqEntryPtr sep,
  FILE* fp,
  FmtType format,
  ModType mode,
  StlType style,
  FlgType flags,
  Int2 batch,
  Boolean nocleanup,
  CharPtr gbdjoin,
  Boolean useGbdjoin
)

{
#ifdef OS_UNIX
  Char     buf [256];
  Char     cmmd [256];
  size_t   ct;
  FILE     *fpo;

  if (sep == NULL) return;

  if (batch == 1) {

    if (! nocleanup) {
      SeriousSeqEntryCleanup (sep, NULL, NULL);
    }
    SeqEntryToGnbk (sep, NULL, format, mode, style, flags, fp);
    return; /* just make report, nothing to diff */

  } else if (batch == 2) {

    if (! nocleanup) {
      SeriousSeqEntryCleanup (sep, NULL, NULL);
    }
    SaveAsn2gnbk (sep, path2, format, mode, style, flags);
    SaveAsn2ff (sep, path1, format, mode);

  } else if (batch == 3) {

#ifdef ASN2GNBK_SUPPRESS_UNPUB_AFFIL
    VisitPubdescsInSep (sep, NULL, FreeUnpubAffil);
#endif

    if (! nocleanup) {
      SeriousSeqEntryCleanup (sep, NULL, NULL);
    }
    SaveAsn2gnbk (sep, path1, format, SEQUIN_MODE, style, flags);
    SaveAsn2gnbk (sep, path2, format, RELEASE_MODE, style, flags);

    /*
    SaveAsn2ff (sep, path1, format, SEQUIN_MODE);
    SaveAsn2ff (sep, path2, format, RELEASE_MODE);
    */

  } else if (batch == 4) {

    SaveAsn2ff (sep, path1, format, SEQUIN_MODE);
    SeriousSeqEntryCleanupBulk (sep);
    SeqMgrIndexFeatures (0, sep->data.ptrvalue);
    SaveAsn2ff (sep, path2, format, SEQUIN_MODE);

  }

/* this is one-time code to QA the toolkit with third party annotation SeqIDs
{
    AsnIoPtr  aip;
    aip = AsnIoOpen (path3, "w");
    if (aip == NULL) return;
    SeqEntryAsnWrite (sep, aip, NULL);
    AsnIoClose (aip);
    sprintf (cmmd, "oldasn2ff -a %s -o %s -m s -e -d", path3, path1);
    system (cmmd);
    sprintf (cmmd, "newasn2ff -a %s -o %s -m s -e -d", path3, path2);
    system (cmmd);
    sprintf (cmmd, "rm %s", path3);
    system (cmmd);
}
*/

  if (useGbdjoin) {
    sprintf (cmmd, "%s -o %s -n %s -d reports", gbdjoin, path1, path2);
    system (cmmd);

    sprintf (cmmd, "rm %s; rm %s", path1, path2);
    system (cmmd);
  } else {
    sprintf (cmmd, "sort %s | uniq -c > %s.suc; rm %s", path1, path1, path1);
    system (cmmd);

    sprintf (cmmd, "sort %s | uniq -c > %s.suc; rm %s", path2, path2, path2);
    system (cmmd);

    sprintf (cmmd, "diff %s.suc %s.suc > %s", path1, path2, path3);
    system (cmmd);

    sprintf (cmmd, "cat %s", path3);
    fpo = popen (cmmd, "r");
    if (fpo != NULL) {
      while ((ct = fread (buf, 1, sizeof (buf), fpo)) > 0) {
        fwrite (buf, 1, ct, fp);
        fflush (stdout);
      }
      pclose (fpo);
    }

    sprintf (cmmd, "rm %s.suc; rm %s.suc; rm %s", path1, path2, path3);
    system (cmmd);
  }

#else
  if (! nocleanup) {
    SeriousSeqEntryCleanup (sep, NULL, NULL);
  }

  SeqEntryToGnbk (sep, NULL, format, mode, style, flags, fp);
#endif
}

static void CheckOrder (
  SeqFeatPtr sfp,
  Pointer userdata
)

{
#ifdef ASN2GNBK_IGNORE_OUT_OF_ORDER
  BoolPtr    bp;
  BioseqPtr  bsp;
#endif
#ifdef ASN2GNBK_REPAIR_OUT_OF_ORDER
  BioseqPtr  bsp;
  SeqLocPtr  gslp;
  Boolean    hasNulls;
  Boolean    noLeft;
  Boolean    noRight;
#endif

  /* ignore order of bonds in heterogen features from PDB */

  if (sfp->data.choice == SEQFEAT_HET) return;

#ifdef ASN2GNBK_IGNORE_OUT_OF_ORDER
  bsp = BioseqFindFromSeqLoc (sfp->location);
  if (bsp != NULL && SeqLocBadSortOrder (bsp, sfp->location)) {
    bp = (BoolPtr) userdata;
    *bp = TRUE;
  }
#endif
#ifdef ASN2GNBK_REPAIR_OUT_OF_ORDER
  bsp = BioseqFindFromSeqLoc (sfp->location);
  if (bsp != NULL && SeqLocBadSortOrder (bsp, sfp->location)) {
    hasNulls = LocationHasNullsBetween (sfp->location);
    gslp = SeqLocMerge (bsp, sfp->location, NULL, FALSE, FALSE, hasNulls);
    if (gslp != NULL) {
      CheckSeqLocForPartial (sfp->location, &noLeft, &noRight);
      sfp->location = SeqLocFree (sfp->location);
      sfp->location = gslp;
      if (bsp->repr == Seq_repr_seg) {
        gslp = SegLocToParts (bsp, sfp->location);
        sfp->location = SeqLocFree (sfp->location);
        sfp->location = gslp;
      }
      FreeAllFuzz (sfp->location);
      SetSeqLocPartial (sfp->location, noLeft, noRight);
    }
  }
#endif
}

static Int2 HandleMultipleRecords (
  CharPtr inputFile,
  CharPtr outputFile,
  FmtType format,
  ModType mode,
  StlType style,
  FlgType flags,
  Int2 batch,
  Boolean binary,
  Boolean compressed,
  Boolean nocleanup,
  CharPtr gbdjoin,
  CharPtr accn,
  FILE *logfp
)

{
  AsnIoPtr      aip;
  AsnModulePtr  amp;
  AsnTypePtr    atp, atp_bss, atp_se;
  BioseqPtr     bsp;
  Char          buf [41];
  FILE          *fp;
  SeqEntryPtr   fsep;
  Boolean       hasgi;
  Boolean       hasRefSeq;
  Char          longest [41];
  Int4          numrecords = 0;
  FILE          *ofp = NULL;
  Boolean       outOfOrder;
  Char          path1 [PATH_MAX];
  Char          path2 [PATH_MAX];
  Char          path3 [PATH_MAX];
  SeqEntryPtr   sep;
  time_t        starttime, stoptime, worsttime;
  Boolean       useGbdjoin;
#ifdef OS_UNIX
  Char          cmmd [256];
  CharPtr       gzcatprog;
  int           ret;
  Boolean       usedPopen = FALSE;
#endif

  if (StringHasNoText (inputFile)) return 1;

#ifndef OS_UNIX
  if (compressed) {
    Message (MSG_ERROR, "Can only decompress on-the-fly on UNIX machines");
    return 1;
  }
#endif

  amp = AsnAllModPtr ();
  if (amp == NULL) {
    Message (MSG_FATAL, "Unable to load AsnAllModPtr");
    return 1;
  }

  atp_bss = AsnFind ("Bioseq-set");
  if (atp_bss == NULL) {
    Message (MSG_FATAL, "Unable to find ASN.1 type Bioseq-set");
    return 1;
  }

  atp_se = AsnFind ("Bioseq-set.seq-set.E");
  if (atp_se == NULL) {
    Message (MSG_FATAL, "Unable to find ASN.1 type Bioseq-set.seq-set.E");
    return 1;
  }

#ifdef OS_UNIX
  if (compressed) {
    gzcatprog = getenv ("NCBI_UNCOMPRESS-BINARY");
    if (gzcatprog != NULL) {
      sprintf (cmmd, "%s %s", gzcatprog, inputFile);
    } else {
      ret = system ("gzcat -h >/dev/null 2>&1");
      if (ret == 0) {
        sprintf (cmmd, "gzcat %s", inputFile);
      } else if (ret == -1) {
        Message (MSG_FATAL, "Unable to fork or exec gzcat in ScanBioseqSetRelease");
        return 1;
      } else {
        Message (MSG_FATAL, "Unable to find gzcat in ScanBioseqSetRelease - please edit your PATH environment variable");
        return 1;
      }
    }
    fp = popen (cmmd, binary? "rb" : "r");
    usedPopen = TRUE;
  } else {
    fp = FileOpen (inputFile, binary? "rb" : "r");
  }
#else
  fp = FileOpen (inputFile, binary? "rb" : "r");
#endif
  if (fp == NULL) {
    Message (MSG_ERROR, "FileOpen failed for input file '%s'", inputFile);
    return 1;
  }

  aip = AsnIoNew (binary? ASNIO_BIN_IN : ASNIO_TEXT_IN, fp, NULL, NULL, NULL);
  if (aip == NULL) {
    Message (MSG_ERROR, "AsnIoNew failed for input file '%s'", inputFile);
    return 1;
  }

  if (batch == 1 || format != GENBANK_FMT) {
    ofp = FileOpen (outputFile, "w");
    if (ofp == NULL) {
      AsnIoClose (aip);
      Message (MSG_FATAL, "FileOpen failed for output file '%s'", outputFile);
      return 1;
    }
  }

  TmpNam (path1);
  TmpNam (path2);
  TmpNam (path3);

  atp = atp_bss;

  longest [0] = '\0';
  worsttime = 0;

  while ((atp = AsnReadId (aip, amp, atp)) != NULL) {
    if (atp == atp_se) {
      sep = SeqEntryAsnRead (aip, atp);

      fsep = FindNthBioseq (sep, 1);
      if (fsep != NULL && fsep->choice == 1) {
        bsp = (BioseqPtr) fsep->data.ptrvalue;
        if (bsp != NULL) {
          SeqIdWrite (bsp->id, buf, PRINTID_FASTA_LONG, sizeof (buf));
#ifdef OS_UNIX
          if (batch != 1) {
            printf ("%s\n", buf);
            if (ofp != NULL) {
              fprintf (ofp, "%s\n", buf);
              fflush (ofp);
            }
          }
#endif
          if (logfp != NULL) {
            fprintf (logfp, "%s\n", buf);
            fflush (logfp);
          }
        }
      }

      hasgi = SeqEntryHasGi (sep, accn);
      if (hasgi) {
        sprintf (buf, "%s.before", accn);
        SaveSeqEntry (sep, buf);
        sprintf (buf, "%s.gbff.before", accn);
        SaveAsn2ff (sep, buf, format, SEQUIN_MODE);
        if (! nocleanup) {
          SeriousSeqEntryCleanup (sep, NULL, NULL);
        }
        sprintf (buf, "%s.after", accn);
        SaveSeqEntry (sep, buf);
        sprintf (buf, "%s.gbff.after", accn);
        SaveAsn2ff (sep, buf, format, SEQUIN_MODE);
        if (ofp != NULL) {
          FileClose (ofp);
        }
        AsnIoClose (aip);
        return 0;
      }
      outOfOrder = FALSE;
#ifdef ASN2GNBK_IGNORE_OUT_OF_ORDER
      VisitFeaturesInSep (sep, (Pointer) &outOfOrder, CheckOrder);
#endif
#ifdef ASN2GNBK_REPAIR_OUT_OF_ORDER
      VisitFeaturesInSep (sep, (Pointer) &outOfOrder, CheckOrder);
#endif
      if ((! outOfOrder) && StringHasNoText (accn)) {
        if ((format != GENPEPT_FMT && SeqEntryHasNucs (sep)) ||
           (format == GENPEPT_FMT && SeqEntryHasProts (sep))) {
          if (batch == 1 || SeqEntryNotInExcluded (sep)) {

            hasRefSeq = FALSE;
            VisitBioseqsInSep (sep, (Pointer) &hasRefSeq, LookForRefSeq);
            if (hasRefSeq) {
              if (batch != 1 && format == GENBANK_FMT && ofp == NULL) {
                ofp = FileOpen (outputFile, "w");
                if (ofp == NULL) {
                  ofp = stdout;
                }
              }
            }

            starttime = GetSecs ();
            useGbdjoin = (Boolean) (format == GENBANK_FMT && (! hasRefSeq));
            CompareFlatFiles (path1, path2, path3, sep, ofp,
                              format, mode, style, flags,
                              batch, nocleanup, gbdjoin, useGbdjoin);
            stoptime = GetSecs ();
            if (stoptime - starttime > worsttime) {
              worsttime = stoptime - starttime;
              StringCpy (longest, buf);
            }
            numrecords++;
          }
        }
      }
      SeqEntryFree (sep);
    } else {
      AsnReadVal (aip, atp, NULL);
    }
  }

  if (ofp != NULL) {
    FileClose (ofp);
  }

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

  if (logfp != NULL && (! StringHasNoText (longest))) {
    fprintf (logfp, "Longest processing time %ld seconds on %s\n",
             (long) worsttime, longest);
    fprintf (logfp, "Total number of records %ld\n", (long) numrecords);
    fflush (logfp);
  }

  return 0;
}

/* Args structure contains command-line arguments */

#define i_argInputFile   0
#define o_argOutputFile  1
#define f_argFormat      2
#define m_argMode        3
#define s_argStyle       4
#define g_argFlags       5
#define t_argBatch       6
#define b_argBinary      7
#define c_argCompressed  8
#define l_argLogFile     9
#define r_argRemote     10
#define a_argDoBoth     11
#define n_argNoCleanup  12
#ifdef OS_UNIX
#define q_argGbdJoin    13
#define j_argFrom       14
#define k_argTo         15
#define d_argStrand     16
#define x_argAccnToSave 17
#endif

Args myargs [] = {
  {"Input File Name", "stdin", NULL, NULL,
    FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  {"Output File Name", "stdout", NULL, NULL,
    FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
  {"Format (b GenBank, e EMBL, p GenPept, t Feature Table)", "b", NULL, NULL,
    FALSE, 'f', ARG_STRING, 0.0, 0, NULL},
  {"Mode (r Release, e Entrez, s Sequin, d Dump)", "s", NULL, NULL,
    FALSE, 'm', ARG_STRING, 0.0, 0, NULL},
  {"Style (n Normal, s Segmented, m Master, c Contig)", "n", NULL, NULL,
    FALSE, 's', ARG_STRING, 0.0, 0, NULL},
  {"Bit Flags (1 HTML, 2 ContigFeats, 4 ContigSources,\n\
                 8 FarTransl, 16 LockFarComp, 32 LockFarLoc,\n\
                 64 LockFarProd, 128 FreeSeqPort, 256 DDBJstyle,\n\
                 512 NewSource, 1024 OldLocus, 2048 GpsCdsUp,\n\
                 4096 GpsGeneDown)", "0", NULL, NULL,
    FALSE, 'g', ARG_INT, 0.0, 0, NULL},
  {"Batch (1 Report, 2 Asn2ff/Asn2gnbk, 3 Sequin/Release)", "0", NULL, NULL,
    TRUE, 't', ARG_INT, 0.0, 0, NULL},
  {"Bioseq-set is Binary", "F", NULL, NULL,
    TRUE, 'b', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Bioseq-set is Compressed", "F", NULL, NULL,
    TRUE, 'c', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Log file", NULL, NULL, NULL,
    TRUE, 'l', ARG_FILE_OUT, 0.0, 0, NULL},
  {"Remote Fetching", "F", NULL, NULL,
    TRUE, 'r', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Both asn2ff+asn2gnbk", "F", NULL, NULL,
    TRUE, 'a', ARG_BOOLEAN, 0.0, 0, NULL},
  {"No SeriousSeqEntryCleanup", "F", NULL, NULL,
    TRUE, 'n', ARG_BOOLEAN, 0.0, 0, NULL},
#ifdef OS_UNIX
#ifdef PROC_I80X86
  {"Gbdjoin Executable", "gbdjoin", NULL, NULL,
    TRUE, 'q', ARG_FILE_IN, 0.0, 0, NULL},
#else
  {"Gbdjoin Executable", "/netopt/genbank/subtool/bin/gbdjoin", NULL, NULL,
    TRUE, 'q', ARG_FILE_IN, 0.0, 0, NULL},
#endif
  {"SeqLoc From", "0", NULL, NULL,
    TRUE, 'j', ARG_INT, 0.0, 0, NULL},
  {"SeqLoc To", "0", NULL, NULL,
    TRUE, 'k', ARG_INT, 0.0, 0, NULL},
  {"SeqLoc Minus Strand", "F", NULL, NULL,
    TRUE, 'd', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Accession to extract", NULL, NULL, NULL,
    TRUE, 'x', ARG_STRING, 0.0, 0, NULL},
#endif
};


#include <accid1.h>
#include <lsqfetch.h>

Int2 Main (
  void
)

{
  CharPtr  accn = NULL;
  Int2     batch = 0;
  Boolean  binary = FALSE;
  Boolean  both = FALSE;
  Boolean  compressed = FALSE;
  FlgType  flags;
  FmtType  format;
  Int4     from = 0;
  CharPtr  gbdjoin = NULL;
  Boolean  justSSEC = FALSE;
  CharPtr  logfile = NULL;
  FILE     *logfp = NULL;
  ModType  mode;
  Boolean  nocleanup = FALSE;
  Char     path [PATH_MAX];
  CharPtr  progname;
  Int2     rsult = 0;
  time_t   starttime, stoptime;
  CharPtr  str;
  Uint1    strand = Seq_strand_plus;
  StlType  style;
  Int4     to = 0;

  /* standard setup */

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

  ProgramPath (path, sizeof (path));
  progname = StringRChr (path, DIRDELIMCHR);
  if (progname != NULL) {
    progname++;
  } else {
    progname = "asn2gnbk";
  }

  /* process command line arguments */

  if (! GetArgs (progname, sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  batch = (Int2) myargs [t_argBatch].intvalue;

  if (myargs [b_argBinary].intvalue) {
    binary = TRUE;
  } else {
    binary = FALSE;
  }

  if (myargs [c_argCompressed].intvalue) {
    compressed = TRUE;
  } else {
    compressed = FALSE;
  }

  if (myargs [a_argDoBoth].intvalue) {
    both = TRUE;
  } else {
    both = FALSE;
  }

  if (myargs [n_argNoCleanup].intvalue) {
    nocleanup = TRUE;
  } else {
    nocleanup = FALSE;
  }

  str = myargs [f_argFormat].strvalue;
  if (StringICmp (str, "b") == 0) {
    format = GENBANK_FMT;
  } else if (StringICmp (str, "e") == 0) {
    format = EMBL_FMT;
  } else if (StringICmp (str, "p") == 0) {
    format = GENPEPT_FMT;
  } else if (StringICmp (str, "t") == 0) {
    format = FTABLE_FMT;
  } else if (StringICmp (str, "c") == 0) {
    justSSEC = TRUE;
    format = GENBANK_FMT;
  } else {
    format = GENBANK_FMT;
  }

  str = myargs [m_argMode].strvalue;
  if (StringICmp (str, "r") == 0) {
    mode = RELEASE_MODE;
  } else if (StringICmp (str, "e") == 0) {
    mode = ENTREZ_MODE;
  } else if (StringICmp (str, "s") == 0) {
    mode = SEQUIN_MODE;
  } else if (StringICmp (str, "d") == 0) {
    mode = DUMP_MODE;
  } else {
    mode = RELEASE_MODE;
  }

  str = myargs [s_argStyle].strvalue;
  if (StringICmp (str, "n") == 0) {
    style = NORMAL_STYLE;
  } else if (StringICmp (str, "s") == 0) {
    style = SEGMENTED_STYLE;
  } else if (StringICmp (str, "m") == 0) {
    style = MASTER_STYLE;
  } else if (StringICmp (str, "c") == 0) {
    style = CONTIG_STYLE;
  } else {
    style = NORMAL_STYLE;
  }

  flags = (FlgType) myargs [g_argFlags].intvalue;

  if (myargs [r_argRemote].intvalue) {
    ID1BioseqFetchEnable ("asn2gnbk", FALSE);
    LocalSeqFetchInit (FALSE);
  }

  logfile = (CharPtr) myargs [l_argLogFile].strvalue;
  if (! StringHasNoText (logfile)) {
    logfp = FileOpen (logfile, "w");
  }

#ifdef OS_UNIX
  gbdjoin = myargs [q_argGbdJoin].strvalue;

  from = myargs [j_argFrom].intvalue;
  to = myargs [k_argTo].intvalue;
  if (myargs [d_argStrand].intvalue) {
    strand = Seq_strand_minus;
  } else {
    strand = Seq_strand_plus;
  }

  if (! StringHasNoText (myargs [x_argAccnToSave].strvalue)) {
    accn = myargs [x_argAccnToSave].strvalue;
  }
#endif

  starttime = GetSecs ();

  if (batch != 0 || accn != NULL) {
    rsult = HandleMultipleRecords (myargs [i_argInputFile].strvalue,
                                   myargs [o_argOutputFile].strvalue,
                                   format, mode, style, flags,
                                   batch, binary, compressed,
                                   nocleanup, gbdjoin, accn, logfp);
  } else {
    rsult = HandleSingleRecord (myargs [i_argInputFile].strvalue,
                                myargs [o_argOutputFile].strvalue,
                                format, mode, style, flags,
                                from, to, strand, both,
                                nocleanup, justSSEC);
  }

  stoptime = GetSecs ();
  if (logfp != NULL) {
    fprintf (logfp, "Finished in %ld seconds\n",
             (long) (stoptime - starttime));
    FileClose (logfp);
  }

  if (myargs [r_argRemote].intvalue) {
    LocalSeqFetchDisable ();
    ID1BioseqFetchDisable ();
  }

  return rsult;
}

