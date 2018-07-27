/*   scantest.c
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
* File Name:  scantest.c
*
* Author:  Kans, Schuler, Ostell
*
* Version Creation Date:   1/20/95
*
* $Revision: 6.0 $
*
* File Description: 
*       scans through sequence records on the Entrez discs
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#ifndef _NEW_CdEntrez_
#define _NEW_CdEntrez_
#endif

#include <ncbi.h>
#include <casn.h>
#include <accentr.h>
#include <cdromlib.h>
#include <seqport.h>
#include <objall.h>

#define CHARSPERLINE  50

typedef struct expstruct {
  FILE      *fp;
  AsnIoPtr  aip;
  Boolean   is_na;
  Uint1     feat;
} ExpStruct, PNTR ExpStructPtr;

static Char root [PATH_MAX];
static EntrezInfoPtr eip;
static EntrezDivInfo *div_info;

static Boolean  scanDNA = FALSE;
static Boolean  scanPRT = FALSE;
static Boolean  scanCDS = FALSE;

static void PrintSequence (BioseqPtr bsp, SeqFeatPtr sfp,
                           FILE *fp, Boolean is_na)

{
  Char        buffer [255];
  Uint1       code;
  Int2        count;
  Uint1       repr;
  Uint1       residue;
  SeqPortPtr  spp;
  CharPtr     title;
  CharPtr     tmp;

  if (bsp != NULL && fp != NULL) {
    if ((Boolean) ISA_na (bsp->mol) == is_na) {
      repr = Bioseq_repr (bsp);
      if (repr == Seq_repr_raw || repr == Seq_repr_const) {
        title = BioseqGetTitle (bsp);
        tmp = StringMove (buffer, ">");
        tmp = SeqIdPrint (bsp->id, tmp, PRINTID_FASTA_LONG);
        tmp = StringMove (tmp, " ");
        StringNCpy (tmp, title, 200);
        fprintf (fp, "%s\n", buffer);
        if (is_na) {
          code = Seq_code_iupacna;
        } else {
          code = Seq_code_iupacaa;
        }
        if (sfp != NULL) {
          spp = SeqPortNewByLoc (sfp->location, code);
        } else {
          spp = SeqPortNew (bsp, 0, -1, 0, code);
        }
        if (spp != NULL) {
          count = 0;
          while ((residue = SeqPortGetResidue (spp)) != SEQPORT_EOF) {
            if (! IS_residue (residue)) {
              buffer [count] = '\0';
              fprintf (fp, "%s\n", buffer);
              count = 0;
              switch (residue) {
                case SEQPORT_VIRT :
                  fprintf (fp, "[Gap]\n");
                  break;
                case SEQPORT_EOS :
                  fprintf (fp, "[EOS]\n");
                  break;
                default :
                  fprintf (fp, "[Invalid Residue]\n");
                  break;
              }
            } else {
              buffer [count] = residue;
              count++;
              if (count >= CHARSPERLINE) {
                buffer [count] = '\0';
                fprintf (fp, "%s\n", buffer);
                count = 0;
              }
            }
          }
          if (count != 0) {
            buffer [count] = '\0';
            fprintf (fp, "%s\n", buffer);
          }
          SeqPortFree (spp);
        }
      }
    }
  }
}

static void LIBCALLBACK GetSeqFeat (AsnExpOptStructPtr aeosp)

{
  BioseqPtr     bsp;
  ExpStructPtr  esp;
  SeqFeatPtr    sfp;

  if (aeosp->dvp->intvalue == START_STRUCT) {
    esp = (ExpStructPtr) aeosp->data;
    sfp = (SeqFeatPtr) aeosp->the_struct;
    if (esp != NULL && esp->fp != NULL && sfp != NULL &&
        sfp->data.choice == esp->feat) {
      bsp = BioseqFind (SeqLocId (sfp->location));
      if (bsp != NULL) {
        PrintSequence (bsp, sfp, esp->fp, esp->is_na);
      }
    }
  }
}

static void SeqEntryToFeat (SeqEntryPtr sep, FILE *fp,
                            Boolean is_na, Uint1 feat)

{
  AsnExpOptPtr  aeop;
  AsnIoPtr      aip;
  ExpStructPtr  esp;

  if (sep != NULL && fp != NULL) {
    esp = MemNew (sizeof (ExpStruct));
    if (esp != NULL) {
      aip = AsnIoNullOpen ();
      if (aip != NULL) {
        esp->fp = fp;
        esp->aip = AsnIoNew (ASNIO_TEXT_OUT, fp, NULL, NULL, NULL);
        esp->is_na = is_na;
        esp->feat = feat;
        aeop = AsnExpOptNew (aip, "Seq-feat", (Pointer) esp, GetSeqFeat);
        if (aeop != NULL) {
          SeqEntryAsnWrite (sep, aip, NULL);
          fflush (fp);
          AsnExpOptFree (aip, aeop);
        }
        AsnIoClose (aip);
      }
      MemFree (esp);
    }
  }
}

static void LIBCALLBACK GetFastaSeq (SeqEntryPtr sep, Pointer data,
                         Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  ExpStructPtr  esp;

  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    esp = (ExpStructPtr) data;
    if (bsp != NULL && esp != NULL && esp->fp != NULL) {
      PrintSequence (bsp, NULL, esp->fp, esp->is_na);
    }
  }
}

static void SeqEntryScan (SeqEntryPtr sep, FILE *fp, Boolean is_na)

{
  ExpStruct  es;

  if (sep != NULL && fp != NULL) {
    es.fp = fp;
    es.aip = NULL;
    es.is_na = is_na;
    es.feat = 0;
    SeqEntryExplore (sep, (Pointer) &es, GetFastaSeq);
  }
}

static void ProcessSeqEntry (SeqEntryPtr sep, FILE *fp)

{
  if (sep != NULL && fp != NULL) {
    if (scanDNA) {
      SeqEntryScan (sep, fp, TRUE);
    } else if (scanPRT) {
      SeqEntryScan (sep, fp, FALSE);
    } else if (scanCDS) {
      SeqEntryToFeat (sep, fp, TRUE, 3);
    }
  }
}

typedef struct filelist {
  Int2                  cdnum;
  CharPtr               fdir;
  CharPtr               fname;
  struct filelist PNTR  next;
} FileList, PNTR FileListPtr;

static void ProcessFile (FileListPtr flp, CharPtr root, CharPtr outputfile)

{
  CASN_Handle  casnh;
  FILE         *fp;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep;
  CASN_Type    type;

  if (flp != NULL) {
    fp = FileOpen (outputfile, "a");
    if (fp != NULL) {
      StringCpy (path, root);
      FileBuildPath (path, flp->fdir, NULL);
      FileBuildPath (path, NULL, flp->fname);
      if ((casnh = CASN_Open (path)) != NULL) {
        if (scanPRT) {
          type = CASN_Type_aa;
        } else {
          type = CASN_Type_nt;
        }
        if (CASN_DocType (casnh) == type) {
          while ((sep = CASN_NextSeqEntry (casnh)) != NULL) {
            ProcessSeqEntry (sep, fp);
            SeqEntryFree (sep);
          }
        }
        CASN_Close (casnh);
      }
      FileClose (fp);
    } else {
      Message (MSG_FATAL, "Unable to reopen output file");
    }
  }
}

static void ProcessFileList (FileListPtr flp, CharPtr outputfile)

{
  Int2         device;
  FileListPtr  next;
  Char         root [PATH_MAX];

  if (flp != NULL) {
    root [0] = '\0';
    device = flp->cdnum;
    flp = flp->next;
    while (flp != NULL) {
      next = flp->next;
      if (device != flp->cdnum) {
        if (! CdMountEntrezVolume (flp->cdnum, root, sizeof (root))) {
          Message (MSG_FATAL, "CdMountEntrezVolume failed");
          root [0] = '\0';
        }
      }
      ProcessFile (flp, root, outputfile);
      device = flp->cdnum;
      flp = next;
    }
  }
}

static FileListPtr FileListNew (FileListPtr flp, Int2 cdnum,
                                CharPtr fdir, CharPtr fname)

{
  FileListPtr  newnode;

  newnode = (FileListPtr) MemNew (sizeof (FileList));
  if (newnode != NULL) {
    if (flp != NULL) {
      while (flp->next != NULL && flp->next->cdnum <= cdnum) {
        flp = flp->next;
      }
      newnode->next = flp->next;
      flp->next = newnode;
    }
    newnode->cdnum = cdnum;
    if (fdir != NULL && *fdir != '\0') {
      newnode->fdir = StringSave (fdir);
    }
    if (fname != NULL && *fname != '\0') {
      newnode->fname = StringSave (fname);
    }
  }
  return newnode;
}

static Boolean LIBCALLBACK EnumerateFiles (int cdnum, const char *fdir,
                                           const char *fname, long fsize,
                                           void *opaque_data)

{
  FileListPtr      flp;
  FileListPtr PNTR head;

  head = (FileListPtr PNTR) opaque_data;
  flp = NULL;
  if (head != NULL) {
    flp = FileListNew (*head, (Int2) cdnum, (CharPtr) fdir, (CharPtr) fname);
    if (*head == NULL) {
      *head = flp;
    }
  } else {
    flp = FileListNew (NULL, (Int2) cdnum, (CharPtr) fdir, (CharPtr) fname);
  }
  return TRUE;
}

#define NUMARGS 4

Args myargs [NUMARGS] = {
  {"Scan DNA", "F", NULL, NULL, TRUE, 'd', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Scan Protein", "F", NULL, NULL, TRUE, 'p', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Scan Coding Regions", "F", NULL, NULL, TRUE, 'c', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Output File", "stdout", NULL, NULL, FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL}
};

extern Int2 Main (void)

{
  Char         div [8];
  FileListPtr  flp;
  FILE         *fp;
  Int2         i;
  Boolean      is_network;
  FileListPtr  next;
  Int2         sum;

  if (GetArgs ("Scantest", NUMARGS, myargs)) {
    scanDNA = (Boolean) myargs [0].intvalue;
    scanPRT = (Boolean) myargs [1].intvalue;
    scanCDS = (Boolean) myargs [2].intvalue;
    sum = 0;
    for (i = 0; i < 3; i++) {
      sum += myargs [i].intvalue;
    }
    if (sum == 1) {
      if (AllObjLoad () && SeqCodeSetLoad ()) {
        if (EntrezInit ("scantest", FALSE, &is_network)) {
          if (is_network) {
            Message (MSG_FATAL, "Network service does not allow scanning");
          } else {
            eip = EntrezGetInfo ();
            if (eip != NULL && eip->div_info != NULL) {
              flp = FileListNew (NULL, INT2_MIN, NULL, NULL);
              if (flp != NULL) {
                div_info = eip->div_info;
                for (i = 0; i < eip->div_count; i++) {
                  StringNCpy (div, div_info [i].tag, sizeof (div) - 1);
                  if (scanPRT) {
                    CdEnumFiles (CdDir_rec, TYP_AA, div, EnumerateFiles, &flp);
                  } else {
                    CdEnumFiles (CdDir_rec, TYP_NT, div, EnumerateFiles, &flp);
                  }
                }
                fp = FileOpen (myargs[3].strvalue, "w");
                if (fp != NULL) {
                  FileClose (fp);
                  ProcessFileList (flp, myargs[3].strvalue);
                } else {
                  Message (MSG_FATAL, "Unable to create output file");
                }
                while (flp != NULL) {
                  next = flp->next;
                  MemFree (flp->fdir);
                  MemFree (flp->fname);
                  MemFree (flp);
                  flp = next;
                }
                flp = NULL;
              } else {
                Message (MSG_FATAL, "Unable to allocate file list pointer");
              }
            } else {
              Message (MSG_FATAL, "Unable to obtain Entrez Info");
            }
          }
          EntrezFini ();
        } else {
          Message (MSG_FATAL, "Unable to connect to Entrez service");
        }
      } else {
        Message (MSG_FATAL, "Unable to load parse tables");
      }
    } else {
      Message (MSG_FATAL, "You must choose one of the three options");
    }
  }
  return 0;
}
