/*   sequin2.c
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
* File Name:  sequin2.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   1/22/95
*
* $Revision: 6.109 $
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

#include "sequin.h"
#include <document.h>
#include <sequtil.h>
#include <biosrc.h>
#include <cdrgn.h>
#include <seqsub.h>
#include <tofasta.h>
#include <gather.h>
#include <subutil.h>
#include <suggslp.h>
#include <toasn3.h>
#include <toporg.h>
#include <salfiles.h>
#include <salsap.h>
#include <salign.h>
#include <edutil.h>
#include <vsm.h>
#include <accentr.h>
#include <accutils.h>
#include <explore.h>
#include <aliparse.h>
#ifdef WIN_MOTIF
#include <netscape.h>
#endif

extern EnumFieldAssoc  orgmod_subtype_alist [];
extern EnumFieldAssoc  subsource_subtype_alist [];
extern EnumFieldAssoc  biosource_genome_simple_alist [];

static void FindFirstTitle (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  CharPtr PNTR  ttlptr;

  if (mydata == NULL) return;
  ttlptr = (CharPtr PNTR) mydata;
  if (*ttlptr != NULL) return;
  *ttlptr = SeqEntryGetTitle (sep);
}

static void FindFirstSeqDescrTitle (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  ValNodePtr PNTR  vnpptr;

  if (mydata == NULL) return;
  vnpptr = (ValNodePtr PNTR) mydata;
  if (*vnpptr != NULL) return;
  *vnpptr = SeqEntryGetSeqDescr (sep, Seq_descr_title, NULL);
}

static void FindFirstSeqEntryTitle (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  SeqEntryPtr PNTR  sepptr;

  if (mydata == NULL) return;
  sepptr = (SeqEntryPtr PNTR) mydata;
  if (*sepptr != NULL) return;
  if (SeqEntryGetSeqDescr (sep, Seq_descr_title, NULL) != NULL) {
   *sepptr = sep;
  }
}

typedef struct fastapage {
  DIALOG_MESSAGE_BLOCK
  Char         path [PATH_MAX];
  SeqEntryPtr  list;
  ValNodePtr   errmsgs;
  DoC          doc;
  GrouP        instructions;
  Boolean      is_na;
  Boolean      is_mrna;
  Boolean      parseSeqId;
  Boolean      single;
} FastaPage, PNTR FastaPagePtr;

static ParData faParFmt = {FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData faColFmt = {0, 0, 80, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, TRUE};

static void ResetFastaPage (FastaPagePtr fpp)

{
  SeqEntryPtr  next;
  SeqEntryPtr  sep;

  if (fpp != NULL) {
    sep = fpp->list;
    while (sep != NULL) {
      next = sep->next;
      sep->next = NULL;
      SeqEntryFree (sep);
      sep = next;
    }
    fpp->list = NULL;
    fpp->errmsgs = ValNodeFreeData (fpp->errmsgs);
  }
}

extern void MakeSearchStringFromAlist (CharPtr str, CharPtr name)

{
  Char     ch;
  CharPtr  ptr;

  StringCpy (str, "[");
  StringCat (str, name);
  StringCat (str, "=");
  ptr = str;
  ch = *ptr;
  while (*ptr != '\0') {
    *ptr = TO_LOWER (ch);
    ptr++;
    ch = *ptr;
  }
}

static Boolean LookForSearchString (CharPtr title, CharPtr str, CharPtr tmp, size_t maxsize)

{
  CharPtr  ptr;

  ptr = StringISearch (title, str);
  if (ptr != NULL) {
    StringNCpy_0 (tmp, ptr + StringLen (str), maxsize);
     ptr = StringChr (tmp, ']');
     if (ptr != NULL) {
       *ptr = '\0';
     }
    return TRUE;
  }
  return FALSE;
}

static void AddReportLine (CharPtr str, CharPtr name, CharPtr tmp)

{
  StringCat (str, name);
  StringCat (str, ": ");
  StringCat (str, tmp);
  StringCat (str, "\n");
}

static CharPtr singlewarn = "\
ERROR - You may not enter multiple segments for a single sequence submission.\
You should either clear the nucleotide and import a single FASTA record, or\
return to the Sequence Format form and choose the proper submission type.\n\n";

#define FastaFormatBufLen 1000

static void FormatFastaDoc (FastaPagePtr fpp)

{
  EnumFieldAssocPtr  ap;
  BioseqPtr          bsp;
  Boolean            hasErrors;
  CharPtr            label;
  Int4               len;
  Char               lookfor [256];
  CharPtr            measure;
  SeqEntryPtr        nsep;
  Int2               num;
  CharPtr            plural;
  CharPtr            ptr;
  SeqIdPtr           sip;
  SeqEntryPtr        sep;
  CharPtr            str;
  CharPtr            title;
  CharPtr            ttl;
  CharPtr            tmp;
  ValNodePtr         vnp;

  if (fpp != NULL) {
    str = MemNew (sizeof (char) * FastaFormatBufLen);
    tmp = MemNew (sizeof (char) * FastaFormatBufLen);
    if (str == NULL || tmp == NULL) return;
    num = 0;
    len = 0;
    hasErrors = FALSE;
    for (sep = fpp->list; sep != NULL; sep = sep->next) {
      num++;
      if (IS_Bioseq (sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp != NULL) {
          len += bsp->length;
        }
      } else if (IS_Bioseq_set (sep)) {
        nsep = FindNucSeqEntry (sep);
        if (nsep != NULL && IS_Bioseq (nsep)) {
          bsp = (BioseqPtr) nsep->data.ptrvalue;
          if (bsp != NULL) {
            len += bsp->length;
          }
        }
      }
    }
    if (num > 1) {
      plural = "s";
    } else {
      plural = "";
    }
    if (fpp->single && num > 1) {
      AppendText (fpp->doc, singlewarn, &faParFmt, &faColFmt, programFont);
      hasErrors = TRUE;
    }
    if (fpp->is_mrna) {
      label = "Message";
      measure = "nucleotides";
    } else if (fpp->is_na) {
      label = "Segment";
      measure = "nucleotides";
    } else {
      label = "Sequence";
      measure = "amino acids";
    }
    if (fpp->is_mrna) {
      sprintf (str, "%d transcript sequence%s, total length %ld %s\n",
               (int) num, plural, (long) len, measure);
    } else if (fpp->is_na) {
      sprintf (str, "%d nucleotide segment%s, total length %ld %s\n",
               (int) num, plural, (long) len, measure);
    } else {
      sprintf (str, "%d protein sequence%s, total length %ld %s\n",
               (int) num, plural, (long) len, measure);
    }
    AppendText (fpp->doc, str, &faParFmt, &faColFmt, programFont);
    AppendText (fpp->doc, "\nChoose Clear from the Edit menu to clear these sequences",
                &faParFmt, &faColFmt, programFont);
    vnp = fpp->errmsgs;
    num = 0;
    for (sep = fpp->list; sep != NULL; sep = sep->next) {
      num++;
      len = 0;
      sip = NULL;
      tmp [0] = '\0';
      if (IS_Bioseq (sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp != NULL) {
          len = bsp->length;
          sip = SeqIdFindWorst (bsp->id);
          SeqIdWrite (sip, tmp, PRINTID_REPORT, FastaFormatBufLen);
        }
      } else if (IS_Bioseq_set (sep)) {
        nsep = FindNucSeqEntry (sep);
        if (nsep != NULL && IS_Bioseq (nsep)) {
          bsp = (BioseqPtr) nsep->data.ptrvalue;
          if (bsp != NULL) {
            len = bsp->length;
            sip = SeqIdFindWorst (bsp->id);
            SeqIdWrite (sip, tmp, PRINTID_REPORT, FastaFormatBufLen);
          }
        }
      }
      sprintf (str, "\n%s %d\nLength: %ld %s\nSequence ID: %s\n", label,
               (int) num, (long) len, measure, tmp);
      ttl = NULL;
      SeqEntryExplore (sep, (Pointer) (&ttl), FindFirstTitle);
      title = StringSaveNoNull (ttl);
      if (title != NULL && (! fpp->is_na)) {
        if (LookForSearchString (title, "[gene=", tmp, FastaFormatBufLen - 1)) {
          AddReportLine (str, "Gene", tmp);
        } else {
          StringCat (str, "No gene name detected\n");
        }
        if (LookForSearchString (title, "[prot=", tmp, FastaFormatBufLen - 1)) {
          AddReportLine (str, "Protein", tmp);
        } else if (LookForSearchString (title, "[protein=", tmp, FastaFormatBufLen - 1)) {
          AddReportLine (str, "Protein", tmp);
        } else {
          StringCat (str, "No protein name detected\n");
        }
        if (LookForSearchString (title, "[gene_syn=", tmp, FastaFormatBufLen - 1)) {
          AddReportLine (str, "Gene Syn", tmp);
        } 
        if ((LookForSearchString (title, "[protein_desc=", tmp, FastaFormatBufLen - 1)) ||
	    (LookForSearchString (title, "[prot_desc=", tmp, FastaFormatBufLen - 1))) {
          AddReportLine (str, "Protein Desc", tmp);
        } 
        ptr = StringISearch (title, "[orf]");
        if (ptr != NULL) {
        StringCat (str, "ORF indicated\n");
        }
        if (LookForSearchString (title, "[comment=", tmp, FastaFormatBufLen - 1)) {
          AddReportLine (str, "Comment", tmp);
        }
      }
      if (title != NULL && fpp->is_na && (! fpp->is_mrna)) {
        if (LookForSearchString (title, "[org=", tmp, FastaFormatBufLen - 1)) {
          AddReportLine (str, "Organism", tmp);
        }
        if (LookForSearchString (title, "[organism=", tmp, FastaFormatBufLen - 1)) {
          AddReportLine (str, "Organism", tmp);
        }
        if (LookForSearchString (title, "[lineage=", tmp, FastaFormatBufLen - 1)) {
          AddReportLine (str, "Lineage", tmp);
        }
        for (ap = orgmod_subtype_alist; ap->name != NULL; ap++) {
          MakeSearchStringFromAlist (lookfor, ap->name);
          if (LookForSearchString (title, lookfor, tmp, FastaFormatBufLen - 1)) {
            AddReportLine (str, ap->name, tmp);
          }
        }
        for (ap = subsource_subtype_alist; ap->name != NULL; ap++) {
          MakeSearchStringFromAlist (lookfor, ap->name);
          if (LookForSearchString (title, lookfor, tmp, FastaFormatBufLen - 1)) {
            AddReportLine (str, ap->name, tmp);
          }
        }
        if (LookForSearchString (title, "[note=", tmp, FastaFormatBufLen - 1)) {
          AddReportLine (str, "Note", tmp);
        }
        if (LookForSearchString (title, "[comment=", tmp, FastaFormatBufLen - 1)) {
          AddReportLine (str, "Comment", tmp);
        }
        if (LookForSearchString (title, "[subsource=", tmp, FastaFormatBufLen - 1)) {
          AddReportLine (str, "Note", tmp);
        }
        if (LookForSearchString (title, "[molecule=", tmp, FastaFormatBufLen - 1)) {
          AddReportLine (str, "Molecule", tmp);
        }
        if (LookForSearchString (title, "[moltype=", tmp, FastaFormatBufLen - 1)) {
          AddReportLine (str, "MolType", tmp);
        }
        if (LookForSearchString (title, "[location=", tmp, FastaFormatBufLen - 1)) {
          AddReportLine (str, "Location", tmp);
        }
        /*
        for (ap = biosource_genome_simple_alist; ap->name != NULL; ap++) {
          MakeSearchStringFromAlist (lookfor, ap->name);
          ptr = StringISearch (lookfor, "=");
          if (ptr != NULL) {
            *ptr = '\0';
          }
          if (LookForSearchString (title, lookfor, tmp, FastaFormatBufLen - 1)) {
            AddReportLine (str, ap->name, tmp);
          }
        }
        ptr = StringISearch (title, "[dna]");
        if (ptr != NULL) {
          AddReportLine (str, "DNA", "");
        }
        ptr = StringISearch (title, "[rna]");
        if (ptr != NULL) {
          AddReportLine (str, "RNA", "");
        }
        */
      }
      if (title != NULL && fpp->is_na && fpp->is_mrna) {
        if (LookForSearchString (title, "[gene=", tmp, FastaFormatBufLen - 1)) {
          AddReportLine (str, "Gene", tmp);
        } else {
          StringCat (str, "No gene name detected\n");
        }
        if (LookForSearchString (title, "[mrna=", tmp, FastaFormatBufLen - 1)) {
          AddReportLine (str, "mRNA", tmp);
        } else if (LookForSearchString (title, "[cdna=", tmp, FastaFormatBufLen - 1)) {
          AddReportLine (str, "cDNA", tmp);
        } else {
          StringCat (str, "No mRNA name detected\n");
        }
        if (LookForSearchString (title, "[comment=", tmp, FastaFormatBufLen - 1)) {
          AddReportLine (str, "Comment", tmp);
        }
      }
      MemFree (title);
      ttl = NULL;
      SeqEntryExplore (sep, (Pointer) (&ttl), FindFirstTitle);
      title = StringSaveNoNull (ttl);
      if (title != NULL) {
        if (! fpp->is_na) {
          ExciseString (title, "[gene=", "]");
          ExciseString (title, "[gene_syn=", "]");
          ExciseString (title, "[prot=", "]");
          ExciseString (title, "[protein=", "]");
          ExciseString (title, "[prot_desc=", "]");
          ExciseString (title, "[protein_desc=", "]");
          ExciseString (title, "[orf", "]");
          ExciseString (title, "[comment", "]");
        } else if (fpp->is_mrna) {
          ExciseString (title, "[gene=", "]");
          ExciseString (title, "[mrna=", "]");
          ExciseString (title, "[cdna=", "]");
          ExciseString (title, "[comment=", "]");
        } else {
          ExciseString (title, "[org=", "]");
          ExciseString (title, "[organism=", "]");
          ExciseString (title, "[lineage=", "]");
          for (ap = orgmod_subtype_alist; ap->name != NULL; ap++) {
            MakeSearchStringFromAlist (lookfor, ap->name);
            ExciseString (title, lookfor, "]");
          }
          for (ap = subsource_subtype_alist; ap->name != NULL; ap++) {
            MakeSearchStringFromAlist (lookfor, ap->name);
            ExciseString (title, lookfor, "]");
          }
          ExciseString (title, "[note=", "]");
          ExciseString (title, "[comment=", "]");
          ExciseString (title, "[subsource=", "]");
          /*
          ExciseString (title, "[clone=", "]");
          ExciseString (title, "[strain=", "]");
          ExciseString (title, "[isolate=", "]");
          */
          ExciseString (title, "[molecule=", "]");
          ExciseString (title, "[moltype=", "]");
          ExciseString (title, "[location=", "]");
          /*
          for (ap = biosource_genome_simple_alist; ap->name != NULL; ap++) {
            MakeSearchStringFromAlist (lookfor, ap->name);
            ptr = StringISearch (lookfor, "=");
            if (ptr != NULL) {
              *ptr = '\0';
            }
            ExciseString (title, lookfor, "]");
          }
          ExciseString (title, "[dna", "]");
          ExciseString (title, "[rna", "]");
          */
        }
        TrimSpacesAroundString (title);
        if (! StringHasNoText (title)) {
          StringCat (str, "Title: ");
          StringNCat (str, title, 128);
          StringCat (str, "\n");
        } else {
          StringCat (str, "No title detected\n");
        }
      }
      MemFree (title);
      if (vnp != NULL && vnp->data.ptrvalue != NULL) {
        hasErrors = TRUE;
        StringCat (str, (CharPtr) vnp->data.ptrvalue);
        StringCat (str, "\n");
      }
      AppendText (fpp->doc, str, &faParFmt, &faColFmt, programFont);
      if (vnp != NULL) {
        vnp = vnp->next;
      }
    }
    MemFree (str);
    MemFree (tmp);
    UpdateDocument (fpp->doc, 0, 0);
    if (hasErrors) {
      Beep ();
      Beep ();
      Beep ();
    }
  }
}

static Boolean ImportFastaDialog (DialoG d, CharPtr filename)

{
  BioseqPtr     bsp;
  Int4          count;
  CharPtr       errormsg;
  CharPtr       extension;
  FILE          *f;
  FastaPagePtr  fpp;
  ValNodePtr    head;
  Boolean       insegset;
  Boolean       isLocalUnknownID;
  SeqEntryPtr   last;
  Char          lastchar;
  SeqEntryPtr   lastsep;
  SeqEntryPtr   nextsep;
  ObjectIdPtr   oid;
  Char          path [PATH_MAX];
  RecT          r;
  SeqEntryPtr   sep;
  SeqIdPtr      sip;
  Char          str [32];
  ValNodePtr    vnp;

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  fpp = (FastaPagePtr) GetObjectExtra (d);
  if (fpp != NULL) {
    extension = NULL;
    if (fpp->is_mrna) {
      extension = GetAppProperty ("FastaNucExtension");
    } else if (fpp->is_na) {
      extension = GetAppProperty ("FastaNucExtension");
    } else {
      extension = GetAppProperty ("FastaProtExtension");
    }
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), extension, "TEXT")) {
      WatchCursor ();
      StringCpy (fpp->path, path);
      ObjectRect (fpp->doc, &r);
      InsetRect (&r, 4, 4);
      faColFmt.pixWidth = r.right - r.left;
      /*
      ResetFastaPage (fpp);
      */
      Reset (fpp->doc);
      Update ();
      sep = fpp->list;
      head = fpp->errmsgs;
      errormsg = NULL;
      f = FileOpen (fpp->path, "r");
      if (f != NULL) {
        count = 0;
        last = sep;
        while (last != NULL) {
          count++;
          last = last->next;
        }
        last = sep;
        while (last != NULL && last->next != NULL) {
          last = last->next;
        }
        lastsep = NULL;
        insegset = FALSE;
        nextsep = SequinFastaToSeqEntryEx (f, fpp->is_na, &errormsg, fpp->parseSeqId, &lastchar);
        while (nextsep != NULL ||
               (lastchar != EOF && lastchar != NULLB && lastchar != 255)) {
          if (nextsep != NULL) {
            count++;
            if (IS_Bioseq (nextsep) && nextsep->data.ptrvalue != NULL) {
              bsp = (BioseqPtr) nextsep->data.ptrvalue;
              isLocalUnknownID = FALSE;
              sip = bsp->id;
              if (sip != NULL && sip->choice == SEQID_LOCAL) {
                oid = (ObjectIdPtr) sip->data.ptrvalue;
                if (oid != NULL && oid->str != NULL) {
                  isLocalUnknownID = (Boolean) (StringICmp (oid->str, "unknown") == 0);
                }
              }
              if ((! fpp->parseSeqId) || isLocalUnknownID) {
                oid = ObjectIdNew ();
                if (oid != NULL) {
                  if (fpp->is_na) {
                    sprintf (str, "nuc %ld", (long) count);
                  } else {
                    sprintf (str, "prot %ld", (long) count);
                  }
                  oid->str = StringSave (str);
                  sip = ValNodeNew (NULL);
                  if (sip != NULL) {
                    sip->choice = SEQID_LOCAL;
                    sip->data.ptrvalue = (Pointer) oid;
                    bsp->id = SeqIdFree (bsp->id);
                    bsp->id = sip;
                    SeqMgrReplaceInBioseqIndex (bsp);
                  } else {
                    ObjectIdFree (oid);
                  }
                }
              }
            }
            SeqEntryPack (nextsep);
            if (sep != NULL) {
              if (insegset) {
                if (lastsep != NULL) {
                  AddSeqEntryToSeqEntry (lastsep, nextsep, TRUE);
                } else {
                  lastsep = nextsep;
                  last->next = nextsep;
                  last = nextsep;
                }
              } else {
                last->next = nextsep;
                last = nextsep;
              }
            } else {
              if (insegset && lastsep == NULL) {
                lastsep = nextsep;
                sep = nextsep;
                last = sep;
              } else {
                sep = nextsep;
                last = sep;
              }
            }
            vnp = ValNodeNew (head);
            if (head == NULL) {
              head = vnp;
            }
            if (vnp != NULL) {
              vnp->data.ptrvalue = errormsg;
              errormsg = NULL;
            }
          } else if (lastchar == '[') {
            insegset = TRUE;
            lastsep = NULL;
          } else if (lastchar == ']') {
            insegset = FALSE;
          }
          nextsep = SequinFastaToSeqEntryEx (f, fpp->is_na, &errormsg, fpp->parseSeqId, &lastchar);
        }
        FileClose (f);
        MemFree (errormsg);
      }
      fpp->list = sep;
      fpp->errmsgs = head;
      SafeHide (fpp->instructions);
      Update ();
      FormatFastaDoc (fpp);
      SafeShow (fpp->doc);
      ArrowCursor ();
      Update ();
      return TRUE;
    }
  }
  return FALSE;
}

static void CleanupFastaDialog (GraphiC g, VoidPtr data)

{
  FastaPagePtr  fpp;

  fpp = (FastaPagePtr) data;
  if (fpp != NULL) {
    ResetFastaPage (fpp);
  }
  MemFree (data);
}

static CharPtr  fastaNucMsg = "\
\nPlease enter information about the nucleotide \
sequence in the spaces above.  Then click on \
'Import Nucleotide FASTA' to read a FASTA file that \
contains the sequence (which can be in segments).  The \
FASTA definition lines may be of the following form:\n\n\
>ID [org=scientific name] [strain=name] [clone=name] title\n\n\
where the [ and ] brackets are actually in the text.";

static CharPtr  fastaGenMsg = "\
\nPlease enter information about the genomic \
sequence in the spaces above.  Then click on \
'Import Genomic FASTA' to read a FASTA file that \
contains the sequence (which can be in segments).  The \
FASTA definition lines may be of the following form:\n\n\
>ID [org=scientific name] [strain=name] [clone=name] title\n\n\
where the [ and ] brackets are actually in the text.";

static CharPtr  fastaMrnaMsg  = "\
\nPlease enter information about the transcript \
sequences in the spaces above.  Then click on \
'Import Transcript FASTA' to read a FASTA file that \
contains the sequence (which can be in segments).  The \
FASTA definition lines may be of the following form:\n\n\
>ID [gene=symbol] [mrna=name] title\n\n\
where the [ and ] brackets are actually in the text.";

static CharPtr  fastaProtMsg = "\
\nPlease enter information about the protein \
sequences in the spaces above.  Then click on \
'Import Protein FASTA' to read a FASTA file that \
contains the sequences.  The FASTA definition lines should \
be of the following form:\n\n\
>ID [gene=symbol] [prot=name] title\n\n\
where the [ and ] brackets are actually in the text.";

extern DialoG CreateFastaDialog (GrouP h, CharPtr title,
                                 Boolean is_na, Boolean is_mrna, CharPtr text,
                                 Boolean parseSeqId, Boolean single)

{
  FastaPagePtr  fpp;
  GrouP         g;
  GrouP         m;
  GrouP         p;
  GrouP         s;

  p = HiddenGroup (h, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  fpp = (FastaPagePtr) MemNew (sizeof (FastaPage));
  if (fpp != NULL) {

    SetObjectExtra (p, fpp, CleanupFastaDialog);
    fpp->dialog = (DialoG) p;
    fpp->todialog = NULL;
    fpp->fromdialog = NULL;
    fpp->importdialog = ImportFastaDialog;

    if (title != NULL && title [0] != '\0') {
      s = NormalGroup (p, 0, -2, title, systemFont, NULL);
    } else {
      s = HiddenGroup (p, 0, -2, NULL);
    }
    m = HiddenGroup (s, -1, 0, NULL);

    fpp->path [0] = '\0';
    fpp->is_na = is_na;
    fpp->is_mrna = is_mrna;
    fpp->parseSeqId = parseSeqId;
    fpp->single = single;

    g = HiddenGroup (m, 0, 0, NULL);
    fpp->instructions = MultiLinePrompt (g, text, 27 * stdCharWidth, programFont);
    fpp->doc = DocumentPanel (g, stdCharWidth * 27, stdLineHeight * 8);
    SetDocAutoAdjust (fpp->doc, FALSE);
    Hide (fpp->doc);
    AlignObjects (ALIGN_CENTER, (HANDLE) fpp->instructions,
                  (HANDLE) fpp->doc, NULL);
  }

  return (DialoG) p;
}

typedef struct phylippage {
  DIALOG_MESSAGE_BLOCK
  Uint1        format;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep;
  ValNodePtr   errmsgs;
  DoC          doc;
  GrouP        instructions;
  Char         extension [10];
} PhylipPage, PNTR PhylipPagePtr;

static ParData phParFmt = {FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData phColFmt = {0, 0, 80, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, TRUE};

#define PhylipFormatBufLen 1000

static void FormatPhylipDoc (PhylipPagePtr ppp)

{
  EnumFieldAssocPtr  ap;
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  CharPtr            label;
  Int4               len;
  Char               lookfor [128];
  CharPtr            measure;
  SeqEntryPtr        nsep;
  Int2               num;
  CharPtr            plural;
  SeqIdPtr           sip;
  SeqEntryPtr        sep;
  CharPtr            str;
  CharPtr            title;
  CharPtr            ttl;
  CharPtr            tmp;
  ValNodePtr         vnp;

  if (ppp != NULL) {
    str = MemNew (sizeof (char) * PhylipFormatBufLen);
    tmp = MemNew (sizeof (char) * PhylipFormatBufLen);
    if (str == NULL || tmp == NULL) return;
    num = 0;
    len = 0;
    sep = ppp->sep;
    if (sep != NULL && IS_Bioseq_set (sep)) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      if (bssp != NULL && (bssp->_class == 7 ||
                           (bssp->_class >= 13 && bssp->_class <= 16))) {
        for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
          num++;
          if (IS_Bioseq (sep)) {
            bsp = (BioseqPtr) sep->data.ptrvalue;
            if (bsp != NULL) {
              len += bsp->length;
            }
          } else if (IS_Bioseq_set (sep)) {
            nsep = FindNucSeqEntry (sep);
            if (nsep != NULL && IS_Bioseq (nsep)) {
              bsp = (BioseqPtr) nsep->data.ptrvalue;
              if (bsp != NULL) {
                len += bsp->length;
              }
            }
          }
        }
      }
    }
    if (num > 1) {
      plural = "s";
    } else {
      plural = "";
    }
    label = "Sequence";
    measure = "nucleotides";
    sprintf (str, "%d nucleotide sequence%s, total length %ld %s\n",
             (int) num, plural, (long) len, measure);
    AppendText (ppp->doc, str, &faParFmt, &faColFmt, programFont);
    AppendText (ppp->doc, "\nChoose Clear from the Edit menu to clear these sequences",
                &faParFmt, &faColFmt, programFont);
    vnp = ppp->errmsgs;
    num = 0;
    sep = ppp->sep;
    if (sep != NULL && IS_Bioseq_set (sep)) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      if (bssp != NULL && (bssp->_class == 7 ||
                           (bssp->_class >= 13 && bssp->_class <= 16))) {
        for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
          num++;
          len = 0;
          sip = NULL;
          tmp [0] = '\0';
          if (IS_Bioseq (sep)) {
            bsp = (BioseqPtr) sep->data.ptrvalue;
            if (bsp != NULL) {
              len = bsp->length;
              sip = SeqIdFindWorst (bsp->id);
              SeqIdWrite (sip, tmp, PRINTID_REPORT, FastaFormatBufLen);
            }
          } else if (IS_Bioseq_set (sep)) {
            nsep = FindNucSeqEntry (sep);
            if (nsep != NULL && IS_Bioseq (nsep)) {
              bsp = (BioseqPtr) nsep->data.ptrvalue;
              if (bsp != NULL) {
                len = bsp->length;
                sip = SeqIdFindWorst (bsp->id);
                SeqIdWrite (sip, tmp, PRINTID_REPORT, FastaFormatBufLen);
              }
            }
          }
          sprintf (str, "\n%s %d\nLength: %ld %s\nSequence ID: %s\n", label,
                   (int) num, (long) len, measure, tmp);
          ttl = NULL;
          SeqEntryExplore (sep, (Pointer) (&ttl), FindFirstTitle);
          title = StringSaveNoNull (ttl);
          if (title != NULL) {
            if (LookForSearchString (title, "[org=", tmp, PhylipFormatBufLen - 1)) {
              AddReportLine (str, "Organism", tmp);
            }
            if (LookForSearchString (title, "[organism=", tmp, PhylipFormatBufLen - 1)) {
              AddReportLine (str, "Organism", tmp);
            }
            if (LookForSearchString (title, "[lineage=", tmp, PhylipFormatBufLen - 1)) {
              AddReportLine (str, "Lineage", tmp);
            }
            for (ap = orgmod_subtype_alist; ap->name != NULL; ap++) {
              MakeSearchStringFromAlist (lookfor, ap->name);
              if (LookForSearchString (title, lookfor, tmp, PhylipFormatBufLen - 1)) {
                AddReportLine (str, ap->name, tmp);
              }
            }
            for (ap = subsource_subtype_alist; ap->name != NULL; ap++) {
              MakeSearchStringFromAlist (lookfor, ap->name);
              if (LookForSearchString (title, lookfor, tmp, PhylipFormatBufLen - 1)) {
                AddReportLine (str, ap->name, tmp);
              }
            }
            if (LookForSearchString (title, "[note=", tmp, PhylipFormatBufLen - 1)) {
              AddReportLine (str, "Note", tmp);
            }
            if (LookForSearchString (title, "[subsource=", tmp, PhylipFormatBufLen - 1)) {
              AddReportLine (str, "Note", tmp);
            }
            if (LookForSearchString (title, "[molecule=", tmp, FastaFormatBufLen - 1)) {
              AddReportLine (str, "Molecule", tmp);
            }
            if (LookForSearchString (title, "[moltype=", tmp, FastaFormatBufLen - 1)) {
              AddReportLine (str, "MolType", tmp);
            }
            if (LookForSearchString (title, "[location=", tmp, FastaFormatBufLen - 1)) {
              AddReportLine (str, "Location", tmp);
            }
            /*
            for (ap = biosource_genome_simple_alist; ap->name != NULL; ap++) {
              MakeSearchStringFromAlist (lookfor, ap->name);
              ptr = StringISearch (lookfor, "=");
              if (ptr != NULL) {
                *ptr = '\0';
              }
              if (LookForSearchString (title, lookfor, tmp, PhylipFormatBufLen - 1)) {
                AddReportLine (str, ap->name, tmp);
              }
            }
            ptr = StringISearch (title, "[dna]");
            if (ptr != NULL) {
              AddReportLine (str, "DNA", "");
            }
            ptr = StringISearch (title, "[rna]");
            if (ptr != NULL) {
              AddReportLine (str, "RNA", "");
            }
            */
          }
          MemFree (title);
          ttl = NULL;
          SeqEntryExplore (sep, (Pointer) (&ttl), FindFirstTitle);
          title = StringSaveNoNull (ttl);
          if (title != NULL) {
            ExciseString (title, "[org=", "]");
            ExciseString (title, "[organism=", "]");
            ExciseString (title, "[lineage=", "]");
            for (ap = orgmod_subtype_alist; ap->name != NULL; ap++) {
              MakeSearchStringFromAlist (lookfor, ap->name);
              ExciseString (title, lookfor, "]");
            }
            for (ap = subsource_subtype_alist; ap->name != NULL; ap++) {
              MakeSearchStringFromAlist (lookfor, ap->name);
              ExciseString (title, lookfor, "]");
            }
            ExciseString (title, "[note=", "]");
            ExciseString (title, "[subsource=", "]");
            /*
            ExciseString (title, "[clone=", "]");
            ExciseString (title, "[strain=", "]");
            ExciseString (title, "[isolate=", "]");
            */
            ExciseString (title, "[molecule=", "]");
            ExciseString (title, "[moltype=", "]");
            ExciseString (title, "[location=", "]");
            /*
            for (ap = biosource_genome_simple_alist; ap->name != NULL; ap++) {
              MakeSearchStringFromAlist (lookfor, ap->name);
              ptr = StringISearch (lookfor, "=");
              if (ptr != NULL) {
                *ptr = '\0';
              }
              ExciseString (title, lookfor, "]");
            }
            ExciseString (title, "[dna", "]");
            ExciseString (title, "[rna", "]");
            */
            TrimSpacesAroundString (title);
            if (! StringHasNoText (title)) {
              StringCat (str, "Title: ");
              StringNCat (str, title, 128);
              StringCat (str, "\n");
            } else {
              StringCat (str, "No title detected\n");
            }
          }
          MemFree (title);
          if (vnp != NULL && vnp->data.ptrvalue != NULL) {
            StringCat (str, (CharPtr) vnp->data.ptrvalue);
            StringCat (str, "\n");
          }
          AppendText (ppp->doc, str, &faParFmt, &faColFmt, programFont);
          if (vnp != NULL) {
            vnp = vnp->next;
          }
        }
      }
    }
    MemFree (str);
    MemFree (tmp);
    UpdateDocument (ppp->doc, 0, 0);
  }
}

static void ResetPhylipPage (PhylipPagePtr ppp)

{
  if (ppp != NULL) {
    ppp->sep = SeqEntryFree (ppp->sep);
    ppp->errmsgs = ValNodeFreeData (ppp->errmsgs);
  }
}

static void LookForATitle (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BoolPtr  rsult;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  rsult = (BoolPtr) mydata;
  if (SeqEntryGetSeqDescr (sep, Seq_descr_title, NULL) != NULL) {
    *rsult = FALSE;
  }
}

static Boolean SeqEntryHasNoTitles (SeqEntryPtr sep)

{
  Boolean hasNoTitles = TRUE;

  SeqEntryExplore (sep, (Pointer) &hasNoTitles, LookForATitle);
  return hasNoTitles;
}

static CharPtr noOrgInTitleWarning =
"sequences have organism information in titles. " \
"It is critical to annotate the data file with organism and source information. " \
"Please quit Sequin and read the Sequin Quick Guide section on preparing the data files before proceeding.";

static CharPtr noSrcInTitleWarning =
"sequences have source information in titles. " \
"It is critical to annotate the data file with organism and source information. " \
"Please quit Sequin and read the Sequin Quick Guide section on preparing the data files before proceeding.";

static Boolean ImportPhylipDialog (DialoG d, CharPtr filename)

{
  AlignFileDataPtr  afdp;
  BioseqSetPtr   bssp;
  Char           ch;
  ErrInfoPtr     eip;
  Uint1          format;
  FILE           *fp;
  ValNodePtr     head;
  CharPtr        msg;
  ObjMgrDataPtr  omdptop;
  ObjMgrData     omdata;
  Uint2          parenttype;
  Pointer        parentptr;
  PhylipPagePtr  ppp;
  Char           path [PATH_MAX];
  ParsedInfoPtr  pip;
  CharPtr        ptr;
  RecT           r;
  ValNodePtr     sdp;
  SeqEntryPtr    sep = NULL;
  Int2           seqtitles;
  Int2           seqtotals;
  Char           str [256];
  SeqEntryPtr    tmp;
  CharPtr        ttl;
  ValNodePtr     vnp;

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  ppp = (PhylipPagePtr) GetObjectExtra (d);
  if (ppp != NULL) {
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), ppp->extension, "TEXT")) {
      WatchCursor ();
      StringCpy (ppp->path, path);
      ObjectRect (ppp->doc, &r);
      InsetRect (&r, 4, 4);
      faColFmt.pixWidth = r.right - r.left;
      /*
      ResetPhylipPage (ppp);
      */
      Reset (ppp->doc);
      Update ();
      ppp->sep = SeqEntryFree (ppp->sep);
      head = NULL;
      fp = FileOpen (path, "r");
      if (fp != NULL) {
        if (ppp->format >= SEQ_FMT_CONTIGUOUS && ppp->format <= SEQ_FMT_INTERLEAVE) {
          while (fgets (str, sizeof (str), fp) != NULL) {
            if (str [0] == '>') {
              ptr = str;
              ch = *ptr;
              while (ch != '\0' && ch != '\n' && ch != '\r') {
                ptr++;
                ch = *ptr;
              }
              *ptr = '\0';
              ValNodeCopyStr (&head, 1, str + 1);
            }
          }
        }
        FileClose (fp);
        format = 0;
        switch (ppp->format) {
          case SEQ_FMT_CONTIGUOUS :
            format = SALSA_CONTIGUOUS;
            break;
          case SEQ_FMT_INTERLEAVE :
            format = SALSA_INTERLEAVE;
            break;
          default :
            format = SALSA_ND;
            break;
        }
        if (format > 0) {
          sep = NULL;
          if (newAlignReader) {
            fp = FileOpen (path, "r");
            if (fp != NULL) {
              afdp = Ali_Read (fp);
              FileClose (fp);
              if (afdp != NULL) {
                pip = afdp->info;
                if (pip != NULL) {
                  if (pip->contigOrInter == ALI_CONTIGUOUS && ppp->format == SEQ_FMT_INTERLEAVE) {
                    AppendText (ppp->doc, "ERROR: Interleaved specified, Contiguous read\n\n", &faParFmt, &faColFmt, programFont);
                  } else if (pip->contigOrInter == ALI_INTERLEAVED && ppp->format == SEQ_FMT_CONTIGUOUS) {
                    AppendText (ppp->doc, "ERROR: Contiguous specified, Interleaved read\n\n", &faParFmt, &faColFmt, programFont);
                  }
                }
                for (eip = afdp->errors; eip != NULL; eip = eip->next) {
                  size_t   len;
                  if (eip->info == NULL) {
                    AppendText (ppp->doc, "eip->info is NULL\n\n", &faParFmt, &faColFmt, systemFont);
                    continue;
                  }
                  len = StringLen (eip->info) + 60;
                  msg = MemNew (len);
                  if (msg == NULL) continue;
                  if (eip->level == LEVEL_MULTI) {
                    StringCpy (msg, "MULTIPLE ERRORS:");
                  } else if (eip->level == LEVEL_ERROR) {
                    StringCpy (msg, "ERROR:");
                  } else if (eip->level == LEVEL_WARNING) {
                    StringCpy (msg, "WARNING:");
                  } else if (eip->level == LEVEL_INFO) {
                    StringCpy (msg, "INFO:");
                  }
                  if (eip->rowNum != 0) {
                    sprintf (str, " [Line %ld]", (long) eip->rowNum);
                    StringCat (msg, str);
                  }
                  if (eip->info != NULL) {
                    StringCat (msg, " ");
                    StringCat (msg, eip->info);
                  }
                  StringCat (msg, "\n\n");
                  AppendText (ppp->doc, msg, &faParFmt, &faColFmt, programFont);
                  MemFree (msg);
                }
                sep = ALI_ConvertToNCBIData (afdp);
                Ali_Free (afdp);
                afdp = NULL;
              }
            }
          } else {
            sep = ReadLocalAlignment (format, path);
          }
          if (sep !=NULL) 
          {
            ppp->sep = sep;
            SafeHide (ppp->instructions);
            Update ();
            SaveSeqEntryObjMgrData (sep, &omdptop, &omdata);
            GetSeqEntryParent (sep, &parentptr, &parenttype);
            SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
            RestoreSeqEntryObjMgrData (sep, omdptop, &omdata);

            if (sep != NULL && IS_Bioseq_set (sep) && SeqEntryHasNoTitles (sep)) {
              bssp = (BioseqSetPtr) sep->data.ptrvalue;
              if (bssp != NULL && (bssp->_class == 7 ||
                                   (bssp->_class >= 13 && bssp->_class <= 16))) {
                for (tmp = bssp->seq_set, vnp = head;
                     tmp != NULL && vnp != NULL;
                     tmp = tmp->next, vnp = vnp->next) {
                  sdp = CreateNewDescriptor (tmp, Seq_descr_title);
                  if (sdp != NULL) {
                    sdp->data.ptrvalue = vnp->data.ptrvalue;
                    vnp->data.ptrvalue = NULL;
                  }
                }
              }
            }

            FormatPhylipDoc (ppp);
            SafeShow (ppp->doc);

            if (sep != NULL && IS_Bioseq_set (sep)) {
              bssp = (BioseqSetPtr) sep->data.ptrvalue;
              if (bssp != NULL && (bssp->_class == 7 ||
                                   (bssp->_class >= 13 && bssp->_class <= 16))) {
                seqtitles = 0;
                seqtotals = 0;
                for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
                  /*
                  ttl = SeqEntryGetTitle (tmp);
                  */
                  ttl = NULL;
                  SeqEntryExplore (tmp, (Pointer) (&ttl), FindFirstTitle);
                  if (ttl != NULL) {
                    if (bssp->_class == BioseqseqSet_class_phy_set) {
                      if (StringISearch (ttl, "[org=") != NULL ||
                          StringISearch (ttl, "[organism=") != NULL) {
                        seqtitles++;
                      }
                    } else if (StringISearch (ttl, "[") != NULL) {
                      seqtitles++;
                    }
                  }
                  seqtotals++;
                }
                if (seqtotals != seqtitles) {
                  sprintf (str, "None");
                  if (seqtitles > 0) {
                    sprintf (str, "Only %d", (int) seqtitles);
                  }
                  ArrowCursor ();
                  Update ();
                  Beep ();
                  if (bssp->_class == BioseqseqSet_class_phy_set) {
                    Message (MSG_OK, "%s of %d %s", str, (int) seqtotals, noOrgInTitleWarning);
                  } else {
                    Message (MSG_OK, "%s of %d %s", str, (int) seqtotals, noSrcInTitleWarning);
                  }
                }
              }
            }
          } else {
            SafeHide (ppp->doc);
            Update ();
            SafeShow (ppp->instructions);
          }
        } else {
          SafeHide (ppp->doc);
          Update ();
          SafeShow (ppp->instructions);
        }
        ValNodeFreeData (head);
      } else {
        SafeHide (ppp->doc);
        Update ();
        SafeShow (ppp->instructions);
      }
      ArrowCursor ();
      Update ();
      return TRUE;
    }
  }
  return FALSE;
}

static void CleanupPhylipDialog (GraphiC g, VoidPtr data)

{
  PhylipPagePtr  ppp;

  ppp = (PhylipPagePtr) data;
  if (ppp != NULL) {
    ResetPhylipPage (ppp);
  }
  MemFree (data);
}

static CharPtr  phylipNucMsg = "\
\nPlease enter information about the nucleotide \
sequence in the spaces above.  Then click on \
'Import Nucleotide Contiguous' to read a file that \
contains the sequences.";

static CharPtr  nexusNucMsg = "\
\nPlease enter information about the nucleotide \
sequence in the spaces above.  Then click on \
'Import Nucleotide Interleaved' to read a file that \
contains the sequences.";

static DialoG CreatePhylipDialog (GrouP h, CharPtr title, CharPtr text,
                                  Uint1 format, CharPtr extension)

{
  PhylipPagePtr  ppp;
  GrouP          g;
  GrouP          m;
  GrouP          p;
  GrouP          s;

  p = HiddenGroup (h, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  ppp = (PhylipPagePtr) MemNew (sizeof (PhylipPage));
  if (ppp != NULL) {

    SetObjectExtra (p, ppp, CleanupPhylipDialog);
    ppp->dialog = (DialoG) p;
    ppp->todialog = NULL;
    ppp->fromdialog = NULL;
    ppp->importdialog = ImportPhylipDialog;

    if (title != NULL && title [0] != '\0') {
      s = NormalGroup (p, 0, -2, title, systemFont, NULL);
    } else {
      s = HiddenGroup (p, 0, -2, NULL);
    }
    m = HiddenGroup (s, -1, 0, NULL);

    ppp->format = format;
    ppp->path [0] = '\0';
    StringNCpy_0 (ppp->extension, extension, sizeof (ppp->extension));

    g = HiddenGroup (m, 0, 0, NULL);
    ppp->instructions = MultiLinePrompt (g, text, 27 * stdCharWidth, programFont);
    ppp->doc = DocumentPanel (g, stdCharWidth * 27, stdLineHeight * 8);
    SetDocAutoAdjust (ppp->doc, FALSE);
    Hide (ppp->doc);
    AlignObjects (ALIGN_CENTER, (HANDLE) ppp->instructions,
                  (HANDLE) ppp->doc, NULL);
  }

  return (DialoG) p;
}

#define NUM_PAGES  8

#define ORGANISM_PAGE     0
#define NUCLEOTIDE_PAGE   1
#define MRNA_PAGE         2
#define PROTEIN_PAGE      3
#define ANNOTATE_PAGE     4

static ENUM_ALIST(biomol_nucX_alist)
  {"Genomic DNA",            253},
  {"Genomic RNA",            254},
  {"Precursor RNA",            2},
  {"mRNA [cDNA]",              3},
  {"Ribosomal RNA",            4},
  {"Transfer RNA",             5},
  {"Small nuclear RNA",        6},
  {"Small cytoplasmic RNA",    7},
  {"Other-Genetic [plasmid]",  9},
  {"cRNA",                    11},
  {"Small nucleolar RNA",     12},
END_ENUM_ALIST

static ENUM_ALIST(biomol_nucGen_alist)
  {"Genomic DNA",            253},
  {"Genomic RNA",            254},
END_ENUM_ALIST

static ENUM_ALIST(topology_nuc_alist)
{"Linear",          TOPOLOGY_LINEAR},
{"Circular",        TOPOLOGY_CIRCULAR},
END_ENUM_ALIST

typedef struct sequencesform {
  FORM_MESSAGE_BLOCK
  GrouP           pages [NUM_PAGES];
  Int2            currentPage;
  Int2            tagFromPage [NUM_PAGES];
  Int2            numPages;
  DialoG          tbs;

  DialoG          genbio;
  Uint1           dnamolfrommolinfo;
  PopuP           moltypePopup;
  EnumFieldAssoc  PNTR moltypeAlist;
  PopuP           topologyPopup;
  ButtoN          partial5;
  ButtoN          partial3;
  GrouP           singleIdGrp;
  TexT            singleSeqID;
  ButtoN          makeAlign;
  DialoG          dnaseq;

  Int2            seqPackage;
  Int2            seqFormat;
  Int2            numSeqs;

  ButtoN          protTechBoth;
  ButtoN          partialN;
  ButtoN          partialC;
  Boolean         makeMRNA;
  DialoG          protseq;

  DialoG          mrnaseq;
  ButtoN          partialmRNA5;
  ButtoN          partialmRNA3;

  GrouP           annotType;
  GrouP           annotGrp;
  ButtoN          partialLft;
  ButtoN          partialRgt;
  TexT            geneName;
  PrompT          protOrRnaPpt;
  TexT            protOrRnaName;
  TexT            featcomment;
  TexT            defline;
  ButtoN          orgPrefix;
  PopuP           genome;
  PopuP           gencode;

  ButtoN          nextBtn;
  ButtoN          prevBtn;
  BtnActnProc     goToNext;
  BtnActnProc     goToPrev;

  SeqEntryPtr     topSeqForConfirm;
  SeqEntryPtr     currConfirmSeq;
  FormActnFunc    putItAllTogether;
  Int2            currConfirmCount;
} SequencesForm, PNTR SequencesFormPtr;

extern Boolean SequencesFormHasProteins (ForM f)

{
  FastaPagePtr      fpp;
  SequencesFormPtr  sqfp;

  sqfp = (SequencesFormPtr) GetObjectExtra (f);
  if (sqfp != NULL) {
    if (sqfp->seqPackage >= SEQ_PKG_POPULATION) return TRUE;
    fpp = GetObjectExtra (sqfp->protseq);
    if (fpp != NULL) {
      if (fpp->path [0] != '\0') {
        return TRUE;
      }
    }
  }
  return FALSE;
}

extern Boolean SequencesFormHasTooManyNucleotides (ForM f)

{
  FastaPagePtr      fpp;
  SequencesFormPtr  sqfp;

  sqfp = (SequencesFormPtr) GetObjectExtra (f);
  if (sqfp != NULL) {
    if (sqfp->seqPackage > SEQ_PKG_SINGLE) return FALSE;
    fpp = GetObjectExtra (sqfp->dnaseq);
    if (fpp != NULL) {
      if (fpp->list != NULL && fpp->list->next != NULL) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

static Boolean UpdateBspMolGatherFunc (GatherContextPtr gcp)

{
  BioseqPtr         bsp;
  SequencesFormPtr  sqfp;
  UIEnum            val;
  
  if (gcp == NULL) return TRUE;
  sqfp = (SequencesFormPtr) gcp->userdata;
  if (sqfp == NULL) return TRUE;
  if (gcp->thistype == OBJ_BIOSEQ && gcp->thisitem != NULL) {
    bsp = (BioseqPtr) gcp->thisitem;
    if (bsp->mol != Seq_mol_dna && bsp->mol != Seq_mol_rna) {
      bsp->mol = (Uint1) sqfp->dnamolfrommolinfo;
    }
    if (GetEnumPopup (sqfp->topologyPopup, topology_nuc_alist, &val)) {
      bsp->topology = (Uint1) val;
    }
  }
  return TRUE;
}

extern DialoG CreateTagListDialogEx (GrouP h, Uint2 rows, Uint2 cols,
                                     Int2 spacing, Uint2Ptr types,
                                     Uint2Ptr textWidths, EnumFieldAssocPtr PNTR alists,
                                     Boolean useBar, Boolean noExtend,
                                     ToDialogFunc tofunc, FromDialogFunc fromfunc);

typedef struct fixnucorm {
  FORM_MESSAGE_BLOCK
  PopuP              modType;
  GrouP              modGrp;
  GrouP              pptGrp;
  DialoG             modifiers;
  Int2               oldModVal;

  SequencesFormPtr   sqfp;
} FixNucForm, PNTR FixNucFormPtr;

static ENUM_ALIST(combined_subtype_alist)
  {" ",                    0},
  {"Authority",           31},
  {"Anamorph",            36},
  {"Breed",               38},
  {"Cell-line",            2},
  {"Cell-type",            3},
  {"Chromosome",           4},
  {"Clone",                5},
  {"Clone-lib",            6},
  {"Country",             29},
  {"Cultivar",             7},
  {"Dev-stage",            8},
  {"Ecotype",             34},
  {"Forma",               32},
  {"Forma-specialis",     33},
  {"Haplotype",            9},
  {"Isolate",             10},
  {"Lab-host",            11},
  {"Lineage",             26},
  {"Location",            25},
  {"Map",                 12},
  {"Molecule",            24},
  {"Natural-host",        13},
  {"Old Name",            28},
  {"Organism",             1},
  {"Plasmid-name",        14},
  {"Plastid-name",        15},
  {"Segment",             30},
  {"Sex",                 16},
  {"Specimen-voucher",    27},
  {"Strain",              17},
  {"Sub-species",         18},
  {"Synonym",             35},
  {"Teleomorph",          37},
  {"Tissue-lib",          19},
  {"Tissue-type",         20},
  {"Transposon-name",     21},
  {"Variety",             22},
  {"Note",                23},
END_ENUM_ALIST

static void ModifierDialogToSeqEntryPtr (DialoG d, PopuP t, Pointer data, FixNucFormPtr fnfp)

{
  EnumFieldAssocPtr  ap;
  BioseqPtr          bsp;
  size_t             len;
  SeqEntryPtr        list;
  Char               lookfor [128];
  SeqEntryPtr        nsep;
  CharPtr            str;
  CharPtr            title;
  TagListPtr         tlp;
  CharPtr            tmp;
  ValNodePtr         ttlvnp;
  UIEnum             val;
  ValNodePtr         vnp;

  tlp = (TagListPtr) GetObjectExtra (d);
  list = (SeqEntryPtr) data;
  if (tlp != NULL && tlp->vnp != NULL) {
    val = (UIEnum) fnfp->oldModVal;
    for (ap = combined_subtype_alist; ap->name != NULL && ap->value != val; ap++) continue;
    if (ap->name == NULL) return;
    if (StringICmp (ap->name, "Organism") == 0) {
      MakeSearchStringFromAlist (lookfor, "Org");
    } else {
      MakeSearchStringFromAlist (lookfor, ap->name);
    }
    for (vnp = tlp->vnp; vnp != NULL && list != NULL; vnp = vnp->next, list = list->next) {
      bsp = NULL;
      if (IS_Bioseq (list)) {
        bsp = (BioseqPtr) list->data.ptrvalue;
      } else if (IS_Bioseq_set (list)) {
        nsep = FindNucSeqEntry (list);
        if (nsep != NULL && IS_Bioseq (nsep)) {
          bsp = (BioseqPtr) nsep->data.ptrvalue;
        }
      }
      if (bsp != NULL) {
        ttlvnp = NULL;
        SeqEntryExplore (list, (Pointer) (&ttlvnp), FindFirstSeqDescrTitle);
        if (ttlvnp == NULL) {
          ttlvnp = CreateNewDescriptor (list, Seq_descr_title);
        }
        if (ttlvnp != NULL) {
          title = (CharPtr) ttlvnp->data.ptrvalue;
          ExciseString (title, lookfor, "]");
          str = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 1);
          TrimSpacesAroundString (str);
          if (! StringHasNoText (str)) {
            len = StringLen (title) + StringLen (str) + StringLen (lookfor) + 10;
            tmp = MemNew (len);
            if (tmp != NULL) {
              StringCpy (tmp, title);
              StringCat (tmp, " ");
              StringCat (tmp, lookfor);
              StringCat (tmp, str);
              StringCat (tmp, "]");
              ttlvnp->data.ptrvalue = MemFree (ttlvnp->data.ptrvalue);
              ttlvnp->data.ptrvalue = StringSave (tmp);
            }
            MemFree (tmp);
          }
        }
      }
    }
  }
}

static void SeqEntryPtrToModifierDialog (DialoG d, PopuP p, Pointer data, FixNucFormPtr fnfp)

{
  EnumFieldAssocPtr  ap;
  BioseqPtr          bsp;
  ValNodePtr         head;
  Int2               j;
  size_t             len;
  SeqEntryPtr        list;
  Char               lookfor [128];
  SeqEntryPtr        nsep;
  CharPtr            ptr;
  SeqIdPtr           sip;
  CharPtr            str;
  Char               text [128];
  CharPtr            title;
  TagListPtr         tlp;
  CharPtr            ttl;
  Char               tmp [128];
  UIEnum             val;
  ValNodePtr         vnp;

  tlp = (TagListPtr) GetObjectExtra (d);
  list = (SeqEntryPtr) data;
  if (tlp != NULL) {
    if (! GetEnumPopup (p, combined_subtype_alist, &val)) return;
    fnfp->oldModVal = (Int2) val;
    for (ap = combined_subtype_alist; ap->name != NULL && ap->value != val; ap++) continue;
    if (ap->name == NULL) return;
    if (StringICmp (ap->name, "Organism") == 0) {
      MakeSearchStringFromAlist (lookfor, "Org");
    } else {
      MakeSearchStringFromAlist (lookfor, ap->name);
    }
    head = NULL;
    while (list != NULL) {
      bsp = NULL;
      if (IS_Bioseq (list)) {
        bsp = (BioseqPtr) list->data.ptrvalue;
      } else if (IS_Bioseq_set (list)) {
        nsep = FindNucSeqEntry (list);
        if (nsep != NULL && IS_Bioseq (nsep)) {
          bsp = (BioseqPtr) nsep->data.ptrvalue;
        }
      }
      if (bsp != NULL) {
        vnp = ValNodeNew (head);
        if (head == NULL) {
          head = vnp;
        }
        if (vnp != NULL) {
          ttl = NULL;
          SeqEntryExplore (list, (Pointer) (&ttl), FindFirstTitle);
          title = StringSaveNoNull (ttl);
          sip = SeqIdFindWorst (bsp->id);
          SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
          ptr = StringChr (tmp, '|');
          if (ptr == NULL) {
            ptr = tmp;
          } else {
            ptr++;
          }
          text [0] = '\0';
          if (! LookForSearchString (title, lookfor, text, sizeof (text) - 1)) {
            StringCpy (text, " ");
          }
          len = StringLen (ptr) + StringLen (text);
          str = MemNew (len + 4);
          if (str != NULL) {
            StringCpy (str, ptr);
            StringCat (str, "\t");
            StringCat (str, text);
            StringCat (str, "\n");
          }
          vnp->data.ptrvalue = str;
        }
      }
      list = list->next;
    }
    SendMessageToDialog (tlp->dialog, VIB_MSG_RESET);
    tlp->vnp = head;
    SendMessageToDialog (tlp->dialog, VIB_MSG_REDRAW);
    for (j = 0, vnp = tlp->vnp; vnp != NULL; j++, vnp = vnp->next) {
    }
    tlp->max = MAX ((Int2) 0, (Int2) (j - tlp->rows));
    CorrectBarMax (tlp->bar, tlp->max);
    CorrectBarPage (tlp->bar, tlp->rows - 1, tlp->rows - 1);
    if (tlp->max > 0) {
      SafeShow (tlp->bar);
    } else {
      SafeHide (tlp->bar);
    }
    SendMessageToDialog (tlp->dialog, VIB_MSG_ENTER);
  }
}

static CharPtr noOrgInTitleReject =
"sequences have organism information in titles. " \
"It is critical to annotate the data file with organism and source information. " \
"Sequin will not continue until you supply this information.";

static Boolean NotEnoughOrgTitles (SequencesFormPtr sqfp, SeqEntryPtr sep)

{
  MsgAnswer  ans;
  Int2       seqtitles = 0;
  Int2       seqtotals = 0;
  Char       str [256];
  CharPtr    title;

  if (sqfp == NULL || sep == NULL) return FALSE;

  while (sep != NULL) {
    title = NULL;
    SeqEntryExplore (sep, (Pointer) (&title), FindFirstTitle);
    if (title != NULL) {
      if (sqfp->seqPackage == SEQ_PKG_PHYLOGENETIC) {
        if (StringISearch (title, "[org=") != NULL ||
            StringISearch (title, "[organism=") != NULL) {
          seqtitles++;
        }
      } else if (StringISearch (title, "[") != NULL) {
        seqtitles++;
      }
    }
    sep = sep->next;
    seqtotals++;
  }

  if (seqtotals != seqtitles) {
    sprintf (str, "None");
    if (seqtitles > 0) {
      sprintf (str, "Only %d", (int) seqtitles);
    }
    ArrowCursor ();
    Update ();
    Beep ();
    if (! indexerVersion) {
      if (sqfp->seqPackage == SEQ_PKG_PHYLOGENETIC) {
        Message (MSG_OK, "%s of %d %s", str, (int) seqtotals, noOrgInTitleReject);
        return TRUE;
      }
    } else {
      if (sqfp->seqPackage == SEQ_PKG_PHYLOGENETIC) {
        ans = Message (MSG_YN, "%s of %d %s (Regular version will block here - continue?)", str, (int) seqtotals, noOrgInTitleReject);
        if (ans == ANS_NO) return TRUE;
      }
    }
  }

  return FALSE;
}

static void AcceptNucleotideFixup (ButtoN b)

{
  FixNucFormPtr     fnfp;
  SeqEntryPtr       sep;
  SequencesFormPtr  sqfp;

  fnfp = (FixNucFormPtr) GetObjectExtra (b);
  if (fnfp == NULL) return;
  if (fnfp->sqfp != NULL) {
    Hide (fnfp->form);
    Update ();
    sqfp = fnfp->sqfp;
    if (fnfp->oldModVal != 0) {
      sep = sqfp->currConfirmSeq;
      ModifierDialogToSeqEntryPtr (fnfp->modifiers, fnfp->modType, sep, fnfp);
    }
    sep = sqfp->currConfirmSeq;
    if (NotEnoughOrgTitles (sqfp, sep)) {
      Show (fnfp->form);
      Update ();
      return;
    }
    Remove (fnfp->form);
    Update ();
    if (sqfp->putItAllTogether != NULL) {
      sqfp->putItAllTogether (sqfp->form);
    }
  }
}

static void ChangeModifier (PopuP p)

{
  FixNucFormPtr     fnfp;
  SeqEntryPtr       sep;
  SequencesFormPtr  sqfp;
  UIEnum            val;

  fnfp = (FixNucFormPtr) GetObjectExtra (p);
  if (fnfp == NULL) return;
  sqfp = fnfp->sqfp;
  if (sqfp == NULL) return;
  sep = sqfp->currConfirmSeq;
  if (! GetEnumPopup (p, combined_subtype_alist, &val)) {
    val = 0;
  }
  if (val > 0) {
    SafeHide (fnfp->pptGrp);
    SafeShow (fnfp->modGrp);
  } else {
    SafeHide (fnfp->modGrp);
    SafeShow (fnfp->pptGrp);
  }
  if (fnfp->oldModVal != 0) {
    ModifierDialogToSeqEntryPtr (fnfp->modifiers, fnfp->modType, sep, fnfp);
  }
  SeqEntryPtrToModifierDialog (fnfp->modifiers, fnfp->modType, sep, fnfp);
}

static void FixNucMessage (ForM f, Int2 mssg)

{
  FixNucFormPtr  fnfp;

  fnfp = (FixNucFormPtr) GetObjectExtra (f);
  if (fnfp != NULL) {
    switch (mssg) {
      case VIB_MSG_QUIT :
        break;
      case VIB_MSG_CLOSE :
        break;
      case VIB_MSG_CUT :
        StdCutTextProc (NULL);
        break;
      case VIB_MSG_COPY :
        StdCopyTextProc (NULL);
        break;
      case VIB_MSG_PASTE :
        StdPasteTextProc (NULL);
        break;
      case VIB_MSG_DELETE :
        StdDeleteTextProc (NULL);
        break;
      default :
        if (fnfp->appmessage != NULL) {
          fnfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void FixUpNucActivate (WindoW w)

{
  IteM           closeItm;
  IteM           exportItm;
  FixNucFormPtr  fnfp;
  IteM           importItm;

  fnfp = (FixNucFormPtr) GetObjectExtra (w);
  if (fnfp != NULL) {
    if (fnfp->activate != NULL) {
      fnfp->activate (w);
    }
    importItm = FindFormMenuItem ((BaseFormPtr) fnfp, VIB_MSG_IMPORT);
    exportItm = FindFormMenuItem ((BaseFormPtr) fnfp, VIB_MSG_EXPORT);
    closeItm = FindFormMenuItem ((BaseFormPtr) fnfp, VIB_MSG_CLOSE);
    SafeDisable (importItm);
    SafeDisable (exportItm);
    SafeDisable (closeItm);
  }
}

static Uint2 modedit_types [] = {
  TAGLIST_PROMPT, TAGLIST_TEXT
};

static Uint2 modedit_widths [] = {
  0, 0,
};

static CharPtr  sourceModMsg = "\
\nThe Modifier popup lets you select between organism name, \
strain, isolate, and other source modifiers.\n\n\
The resulting spreadsheet lets you enter or edit source \
information for all sequence components.\n\n\
Scientific names should not be abbreviated (use 'Drosophila \
melanogaster' instead of 'D. melanogaster').";

static void LetUserFixNucleotideInfo (SequencesFormPtr sqfp)

{
  ButtoN             b;
  GrouP              c;
  FixNucFormPtr      fnfp;
  GrouP              g;
  GrouP              h;
  GrouP              k;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  GrouP              t;
  WindoW             w;

  if (sqfp == NULL) return;

  sep = sqfp->currConfirmSeq;

  w = NULL;
  fnfp = (FixNucFormPtr) MemNew (sizeof (FixNucForm));
  if (fnfp != NULL) {
    fnfp->sqfp = sqfp;
    w = FixedWindow (-50, -33, -10, -10, "Source Modifiers", NULL);
    SetObjectExtra (w, fnfp, StdCleanupFormProc);
    fnfp->form = (ForM) w;
    fnfp->formmessage = FixNucMessage;

#ifndef WIN_MAC
    CreateStdEditorFormMenus (w);
#endif

    fnfp->activate = NULL;
    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      fnfp->activate = sepp->activateForm;
      fnfp->appmessage = sepp->handleMessages;
    }
    SetActivate (w, FixUpNucActivate);

    h = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (h, 10, 10);

    g = HiddenGroup (h, 2, 0, NULL);
    StaticPrompt (g, "Modifier", 0, popupMenuHeight, programFont, 'l');
    fnfp->modType = PopupList (g, TRUE, ChangeModifier);
    InitEnumPopup (fnfp->modType, combined_subtype_alist, NULL);
    SetEnumPopup (fnfp->modType, combined_subtype_alist, 0);
    SetObjectExtra (fnfp->modType, fnfp, NULL);

    k = HiddenGroup (h, 0, 0, NULL);

    fnfp->modGrp = HiddenGroup (k, 1, 0, NULL);
    t = HiddenGroup (fnfp->modGrp, 2, 0, NULL);
    StaticPrompt (t, "SeqID", 7 * stdCharWidth, 0, programFont, 'l');
    StaticPrompt (t, "Value", 18 * stdCharWidth, 0, programFont, 'l');

    modedit_widths [0] = 7;
    modedit_widths [1] = 18;
    fnfp->modifiers = CreateTagListDialogEx (fnfp->modGrp, 5, 2, 2,
                                             modedit_types, modedit_widths,
                                             NULL, TRUE, TRUE, NULL, NULL);
    fnfp->oldModVal = 0;
    Hide (fnfp->modGrp);

    fnfp->pptGrp = HiddenGroup (k, 1, 0, NULL);
    MultiLinePrompt (fnfp->pptGrp, sourceModMsg, 25 * stdCharWidth, programFont);

    c = HiddenGroup (h, 2, 0, NULL);
    b = PushButton (c, "OK", AcceptNucleotideFixup);
    SetObjectExtra (b, fnfp, NULL);

    AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) fnfp->modGrp,
                  (HANDLE) fnfp->pptGrp, (HANDLE) c, NULL);

    RealizeWindow (w);

    SeqEntryPtrToModifierDialog (fnfp->modifiers, fnfp->modType, sep, fnfp);
    Show (w);
    Select (w);
    Update ();
    SendHelpScrollMessage (helpForm, "Source Modifiers Form", NULL);
  }
}

typedef struct fixprotform {
  FORM_MESSAGE_BLOCK
  TexT               geneDesc;
  TexT               geneSymbol;
  ButtoN             orf;
  TexT               protDesc;
  TexT               protName;
  TexT               seqID;
  TexT               comment;
  TexT               title;

  SequencesFormPtr   sqfp;
  SeqEntryPtr        esep;
  SeqEntryPtr        nsep;
  SeqEntryPtr        psep;
  BioseqPtr          pbsp;
} FixProtForm, PNTR FixProtFormPtr;

static void LetUserFixProteinInfo (SequencesFormPtr sqfp);

static void AcceptThisFixup (ButtoN b)

{
  FixProtFormPtr    fpfp;
  Boolean           needsOpener;
  SeqEntryPtr       old;
  BioseqPtr         pbsp;
  SeqEntryPtr       psep;
  CharPtr           ptr;
  SeqLocPtr         slp;
  SequencesFormPtr  sqfp;
  Char              str [256];
  CharPtr           title;
  Char              tmp [256];
  ValNodePtr        vnp;

  fpfp = (FixProtFormPtr) GetObjectExtra (b);
  if (fpfp == NULL) return;
  if (fpfp->sqfp != NULL) {
    Hide (fpfp->form);
    Update ();
    sqfp = fpfp->sqfp;
    psep = fpfp->psep;
    pbsp = fpfp->pbsp;
    GetTitle (fpfp->seqID, str, sizeof (str));
    if (! StringHasNoText (str)) {
      pbsp->id = SeqIdFree (pbsp->id);
      pbsp->id = MakeSeqID (str);
      SeqMgrReplaceInBioseqIndex (pbsp);
    } else {
      pbsp->id = SeqIdFree (pbsp->id);
      slp = CreateWholeInterval (fpfp->nsep);
      old = SeqEntrySetScope (fpfp->esep);
      pbsp->id = MakeNewProteinSeqId (slp, NULL);
      SeqMgrReplaceInBioseqIndex (pbsp);
      SeqEntrySetScope (old);
      SeqLocFree (slp);
    }
    title = MemNew (1000 * sizeof (Char));
    if (title != NULL) {
      tmp [0] = '\0';
      needsOpener = TRUE;

      GetTitle (fpfp->geneSymbol, str, sizeof (str));
      TrimSpacesAroundString (str);
      if (! StringHasNoText (str)) {
        if (needsOpener) {
          StringCat (title, "[gene=");
          needsOpener = FALSE;
        }
        StringCat (title, str);
      }
      GetTitle (fpfp->geneDesc, str, sizeof (str));
      TrimSpacesAroundString (str);
      if (! StringHasNoText (str)) {
        if (needsOpener) {
          StringCat (title, "[gene=");
          needsOpener = FALSE;
        } else {
          StringCat (title, ";");
        }
        StringCat (title, str);
      }
      if (! needsOpener) {
        StringCat (title, "] ");
      }

      needsOpener = TRUE;
      GetTitle (fpfp->protName, str, sizeof (str));
      TrimSpacesAroundString (str);
      if (! StringHasNoText (str)) {
        if (needsOpener) {
          StringCat (title, "[prot=");
          needsOpener = FALSE;
        }
        StringCat (title, str);
      }
      GetTitle (fpfp->protDesc, str, sizeof (str));
      TrimSpacesAroundString (str);
      if (! StringHasNoText (str)) {
        if (needsOpener) {
          StringCat (title, "[prot=");
          needsOpener = FALSE;
        } else {
          StringCat (title, ";");
        }
        StringCat (title, str);
      }
      if (! needsOpener) {
        StringCat (title, "] ");
      }

      needsOpener = TRUE;
      GetTitle (fpfp->comment, str, sizeof (str));
      TrimSpacesAroundString (str);
      if (! StringHasNoText (str)) {
        if (needsOpener) {
          StringCat (title, "[comment=");
          needsOpener = FALSE;
        }
        StringCat (title, str);
      }
      if (! needsOpener) {
        StringCat (title, "] ");
      }

      if (GetStatus (fpfp->orf)) {
        StringCat (title, "[orf] ");
      }

      ptr = SaveStringFromText (fpfp->title);
      StringCat (title, ptr);
      MemFree (ptr);

      if (pbsp->descr != NULL) {
        vnp = ValNodeFindNext (pbsp->descr, NULL, Seq_descr_title);
        if (vnp != NULL && vnp->data.ptrvalue != NULL) {
          vnp->data.ptrvalue = MemFree (vnp->data.ptrvalue);
          ValNodeExtract (&(pbsp->descr), Seq_descr_title);
        }
      }
      vnp = CreateNewDescriptor (psep, Seq_descr_title);
      if (vnp != NULL) {
        vnp->data.ptrvalue = StringSave (title);
      }
    }
    MemFree (title);

    Remove (fpfp->form);
    Update ();
    if (sqfp->currConfirmSeq != NULL) {
      sqfp->currConfirmSeq = sqfp->currConfirmSeq->next;
    }
    LetUserFixProteinInfo (sqfp);
  }
}

static void FixProtMessage (ForM f, Int2 mssg)

{
  FixProtFormPtr  fpfp;

  fpfp = (FixProtFormPtr) GetObjectExtra (f);
  if (fpfp != NULL) {
    switch (mssg) {
      case VIB_MSG_QUIT :
        break;
      case VIB_MSG_CLOSE :
        break;
      case VIB_MSG_CUT :
        StdCutTextProc (NULL);
        break;
      case VIB_MSG_COPY :
        StdCopyTextProc (NULL);
        break;
      case VIB_MSG_PASTE :
        StdPasteTextProc (NULL);
        break;
      case VIB_MSG_DELETE :
        StdDeleteTextProc (NULL);
        break;
      default :
        if (fpfp->appmessage != NULL) {
          fpfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void FixUpProtActivate (WindoW w)

{
  IteM            closeItm;
  IteM            exportItm;
  FixProtFormPtr  fpfp;
  IteM            importItm;

  fpfp = (FixProtFormPtr) GetObjectExtra (w);
  if (fpfp != NULL) {
    if (fpfp->activate != NULL) {
      fpfp->activate (w);
    }
    importItm = FindFormMenuItem ((BaseFormPtr) fpfp, VIB_MSG_IMPORT);
    exportItm = FindFormMenuItem ((BaseFormPtr) fpfp, VIB_MSG_EXPORT);
    closeItm = FindFormMenuItem ((BaseFormPtr) fpfp, VIB_MSG_CLOSE);
    SafeDisable (importItm);
    SafeDisable (exportItm);
    SafeDisable (closeItm);
  }
}

static Boolean HasMinimalInformation (BioseqPtr pbsp)

{
  CharPtr     ptr;
  SeqIdPtr    sip;
  Char        str [128];
  CharPtr     title;
  ValNodePtr  vnp;

  if (pbsp == NULL) return FALSE;
  sip = SeqIdFindWorst (pbsp->id);
  SeqIdWrite (sip, str, PRINTID_REPORT, sizeof (str));
  if (StringHasNoText (str)) return FALSE;
  if (pbsp->descr == NULL) return FALSE;
  vnp = ValNodeFindNext (pbsp->descr, NULL, Seq_descr_title);
  if (vnp == NULL || vnp->data.ptrvalue == NULL) return FALSE;
  title = (CharPtr) vnp->data.ptrvalue;
  ptr = StringISearch (title, "[gene=");
  if (ptr == NULL) {
    ptr = StringISearch (title, "[gene_syn=");
  }
  if (ptr == NULL) return FALSE;
  StringNCpy_0 (str, ptr + 6, sizeof (str));
  ptr = StringChr (str, ']');
  if (ptr == NULL) return FALSE;
  *ptr = '\0';
  ptr = StringChr (str, ';');
  if (ptr != NULL) {
    *ptr = '\0';
    ptr++;
  }
  if (StringHasNoText (str)) return FALSE;
  ptr = StringISearch (title, "[prot=");
  if (ptr != NULL) {
    StringNCpy_0 (str, ptr + 6, sizeof (str));
    ptr = StringChr (str, ']');
  } else {
    ptr = StringISearch (title, "[protein=");
    if (ptr != NULL) {
      StringNCpy_0 (str, ptr + 9, sizeof (str));
      ptr = StringChr (str, ']');
    }
  }
  if (ptr == NULL) return FALSE;
  *ptr = '\0';
  ptr = StringChr (str, ';');
  if (ptr != NULL) {
    *ptr = '\0';
    ptr++;
  }
  if (StringHasNoText (str)) return FALSE;
  return TRUE;
}

static void LetUserFixProteinInfo (SequencesFormPtr sqfp)

{
  ButtoN             acceptBtn;
  GrouP              c;
  SeqEntryPtr        esep;
  FixProtFormPtr     fpfp;
  GrouP              g;
  GrouP              k;
  Int2               len;
  SeqEntryPtr        nsep;
  BioseqPtr          pbsp;
  SeqEntryPtr        psep;
  CharPtr            ptr;
  Boolean            quickmode;
  StdEditorProcsPtr  sepp;
  SeqIdPtr           sip;
  Char               str [256];
  CharPtr            title;
  ValNodePtr         vnp;
  WindoW             w;

  if (sqfp == NULL) return;

  quickmode = TRUE;
  if (GetAppParam ("SEQUIN", "PREFERENCES", "QUICKMODE", NULL, str, sizeof (str))) {
    if (StringICmp (str, "FALSE") == 0) {
      quickmode = FALSE;
    }
  }

  while (quickmode) {
    psep = sqfp->currConfirmSeq;
    if (psep == NULL) {
      if (sqfp->putItAllTogether != NULL) {
        sqfp->putItAllTogether (sqfp->form);
        return;
      }
    }
    pbsp = (BioseqPtr) psep->data.ptrvalue;
    if (pbsp != NULL) {
      if (HasMinimalInformation (pbsp)) {
        (sqfp->currConfirmCount)++;
        sqfp->currConfirmSeq = psep->next;
      } else {
        quickmode = FALSE;
      }
    } else {
      quickmode = FALSE;
    }
  }

  psep = sqfp->currConfirmSeq;
  if (psep == NULL) {
    if (sqfp->putItAllTogether != NULL) {
      sqfp->putItAllTogether (sqfp->form);
      return;
    }
  }

  esep = sqfp->topSeqForConfirm;
  if (esep == NULL || psep == NULL) return;
  nsep = FindNucSeqEntry (esep);
  if (nsep == NULL) return;
  pbsp = (BioseqPtr) psep->data.ptrvalue;
  if (pbsp == NULL) return;

  (sqfp->currConfirmCount)++;

  w = NULL;
  fpfp = (FixProtFormPtr) MemNew (sizeof (FixProtForm));
  if (fpfp != NULL) {

    fpfp->sqfp = sqfp;
    fpfp->esep = esep;
    fpfp->nsep = nsep;
    fpfp->psep = psep;
    fpfp->pbsp = pbsp;

    sprintf (str, "Product %d, length %ld amino acids",
             (int) sqfp->currConfirmCount, (long) pbsp->length);
    w = FixedWindow (-50, -33, -10, -10, str, NULL);
    SetObjectExtra (w, fpfp, StdCleanupFormProc);
    fpfp->form = (ForM) w;
    fpfp->formmessage = FixProtMessage;

#ifndef WIN_MAC
    CreateStdEditorFormMenus (w);
#endif

    fpfp->activate = NULL;
    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      fpfp->activate = sepp->activateForm;
      fpfp->appmessage = sepp->handleMessages;
    }
    SetActivate (w, FixUpProtActivate);

    SelectFont (programFont);
    len = StringWidth ("Protein Name") + 2;
    SelectFont (systemFont);

    g = HiddenGroup (w, 1, 0, NULL);

    k = HiddenGroup (g, 2, 0, NULL);
    StaticPrompt (k, "Sequence ID", len, dialogTextHeight, programFont, 'l');
    fpfp->seqID = DialogText (k, "", 20, NULL);
    StaticPrompt (k, "Title", len, dialogTextHeight, programFont, 'l');
    fpfp->title = DialogText (k, "", 20, NULL);

    k = HiddenGroup (g, 2, 0, NULL);
    StaticPrompt (k, "Gene Symbol", len, dialogTextHeight, programFont, 'l');
    fpfp->geneSymbol = DialogText (k, "", 20, NULL);
    StaticPrompt (k, "Description", len, dialogTextHeight, programFont, 'l');
    fpfp->geneDesc = DialogText (k, "", 20, NULL);

    k = HiddenGroup (g, 2, 0, NULL);
    StaticPrompt (k, "Protein Name", len, dialogTextHeight, programFont, 'l');
    fpfp->protName = DialogText (k, "", 20, NULL);
    StaticPrompt (k, "Description", len, dialogTextHeight, programFont, 'l');
    fpfp->protDesc = DialogText (k, "", 20, NULL);

    k = HiddenGroup (g, 2, 0, NULL);
    StaticPrompt (k, "Comment", len, 3 * Nlm_stdLineHeight, programFont, 'l');
    fpfp->comment = ScrollText (k, 20, 3, programFont, TRUE, NULL);

    AlignObjects (ALIGN_RIGHT, (HANDLE) fpfp->seqID, (HANDLE) fpfp->title,
                  (HANDLE) fpfp->geneSymbol, (HANDLE) fpfp->geneDesc,
                  (HANDLE) fpfp->protName, (HANDLE) fpfp->protDesc,
                  (HANDLE) fpfp->comment, NULL);

    fpfp->orf = NULL;
    if (pbsp->descr != NULL) {
      vnp = ValNodeFindNext (pbsp->descr, NULL, Seq_descr_title);
      if (vnp != NULL && vnp->data.ptrvalue != NULL) {
        title = (CharPtr) vnp->data.ptrvalue;
        if (StringISearch (title, "[orf]") != NULL) {
          fpfp->orf = CheckBox (w, "Open Reading Frame", NULL);
        }
      }
    }

    c = HiddenGroup (w, 2, 0, NULL);
    acceptBtn = PushButton (c, "OK", AcceptThisFixup);
    SetObjectExtra (acceptBtn, fpfp, NULL);

    AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, (HANDLE) fpfp->orf, NULL);

    RealizeWindow (w);

    sip = SeqIdFindWorst (pbsp->id);
    SeqIdWrite (sip, str, PRINTID_REPORT, sizeof (str));
    SetTitle (fpfp->seqID, str);
    if (pbsp->descr != NULL) {
      vnp = ValNodeFindNext (pbsp->descr, NULL, Seq_descr_title);
      if (vnp != NULL && vnp->data.ptrvalue != NULL) {
        title = (CharPtr) vnp->data.ptrvalue;
        ptr = StringISearch (title, "[gene=");
        if (ptr != NULL) {
          StringNCpy_0 (str, ptr + 6, sizeof (str));
          ptr = StringChr (str, ']');
          if (ptr != NULL) {
            *ptr = '\0';
            ptr = StringChr (str, ';');
            if (ptr != NULL) {
              *ptr = '\0';
              ptr++;
            }
            SetTitle (fpfp->geneSymbol, str);
            SetTitle (fpfp->geneDesc, ptr);
          }
        }
        ptr = StringISearch (title, "[prot=");
        if (ptr != NULL) {
          StringNCpy_0 (str, ptr + 6, sizeof (str));
          ptr = StringChr (str, ']');
        } else {
          ptr = StringISearch (title, "[protein=");
          if (ptr != NULL) {
            StringNCpy_0 (str, ptr + 9, sizeof (str));
            ptr = StringChr (str, ']');
          }
        }
        if (ptr != NULL) {
          *ptr = '\0';
          ptr = StringChr (str, ';');
          if (ptr != NULL) {
            *ptr = '\0';
            ptr++;
          }
          SetTitle (fpfp->protName, str);
          SetTitle (fpfp->protDesc, ptr);
        }
        ptr = StringISearch (title, "[comment=");
        if (ptr != NULL) {
          StringNCpy_0 (str, ptr + 9, sizeof (str));
          ptr = StringChr (str, ']');
          if (ptr != NULL) {
            *ptr = '\0';
            SetTitle (fpfp->comment, str);
          }
        }
        ptr = StringISearch (title, "[orf]");
        if (ptr != NULL) {
          SetStatus (fpfp->orf, TRUE);
        } else {
          Hide (fpfp->orf);
        }
        ExciseString (title, "[gene=", "]");
        ExciseString (title, "[prot=", "]");
        ExciseString (title, "[protein=", "]");
        ExciseString (title, "[orf", "]");
        ExciseString (title, "[comment", "]");
        TrimSpacesAroundString (title);
        SetTitle (fpfp->title, title);
      }
    }

    Select (fpfp->seqID);
    Show (w);
    Select (w);
    Update ();
  }
}

extern void ConfirmSequencesFormParsing (ForM f, FormActnFunc putItAllTogether)

{
  BioseqSetPtr      bssp;
  FastaPagePtr      fpp;
  PhylipPagePtr     ppp;
  SeqEntryPtr       sep;
  SequencesFormPtr  sqfp;

  sqfp = (SequencesFormPtr) GetObjectExtra (f);
  if (sqfp != NULL) {
    sqfp->putItAllTogether = putItAllTogether;
    sqfp->topSeqForConfirm = NULL;
    sqfp->currConfirmSeq = NULL;
    if (sqfp->seqPackage < SEQ_PKG_POPULATION) {
      fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
      if (fpp != NULL) {
        sqfp->topSeqForConfirm = fpp->list;
      }
      sqfp->currConfirmSeq = NULL;
      fpp = (FastaPagePtr) GetObjectExtra (sqfp->protseq);
      if (fpp != NULL) {
        sqfp->currConfirmSeq = fpp->list;
      }
      sqfp->currConfirmCount = 0;
      LetUserFixProteinInfo (sqfp);
    } else {
      if (sqfp->seqFormat == SEQ_FMT_FASTA) {
        fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
        if (fpp != NULL) {
          sqfp->currConfirmSeq = fpp->list;
        }
      } else if (sqfp->seqFormat >= SEQ_FMT_CONTIGUOUS && sqfp->seqFormat <= SEQ_FMT_INTERLEAVE) {
        ppp = (PhylipPagePtr) GetObjectExtra (sqfp->dnaseq);
        if (ppp != NULL) {
          sep = ppp->sep;
          if (sep != NULL && IS_Bioseq_set (sep)) {
            bssp = (BioseqSetPtr) sep->data.ptrvalue;
            if (bssp != NULL) {
              sqfp->currConfirmSeq = bssp->seq_set;
            }
          }
        }
      }
      LetUserFixNucleotideInfo (sqfp);
    }
  }
}

extern void AddToSubSource (BioSourcePtr biop, CharPtr title, CharPtr label, Uint1 subtype)

{
  CharPtr       ptr;
  SubSourcePtr  ssp;
  CharPtr       str;
  SubSourcePtr  tmpssp;

  if (biop == NULL || title == NULL || label == NULL) return;
  str = MemNew (StringLen (title));
  if (str == NULL) return;
  ptr = StringISearch (title, label);
  if (ptr != NULL) {
    StringCpy (str, ptr + StringLen (label));
    ptr = StringChr (str, ']');
    if (ptr != NULL) {
      *ptr = '\0';
      TrimSpacesAroundString (str);
      ssp = SubSourceNew ();
      if (biop->subtype == NULL) {
        biop->subtype = ssp;
      } else {
        tmpssp = biop->subtype;
        while (tmpssp->next != NULL) {
          tmpssp = tmpssp->next;
        }
        tmpssp->next = ssp;
      }
      if (ssp != NULL) {
        ssp->subtype = subtype;
        ssp->name = StringSave (str);
      }
    }
  }
  MemFree (str);
}

extern void AddToOrgMod (BioSourcePtr biop, CharPtr title, CharPtr label, Uint1 subtype)

{
  OrgModPtr   mod;
  OrgNamePtr  onp;
  OrgRefPtr   orp;
  CharPtr     ptr;
  CharPtr     str;
  OrgModPtr   tmpmod;

  if (biop == NULL || title == NULL || label == NULL) return;
  str = MemNew (StringLen (title));
  if (str == NULL) return;
  ptr = StringISearch (title, label);
  if (ptr != NULL) {
    StringCpy (str, ptr + StringLen (label));
    ptr = StringChr (str, ']');
    if (ptr != NULL) {
      *ptr = '\0';
      TrimSpacesAroundString (str);
      orp = biop->org;
      if (orp == NULL) {
        orp = OrgRefNew ();
        biop->org = orp;
      }
      if (orp != NULL) {
        onp = orp->orgname;
        if (onp == NULL) {
          onp = OrgNameNew ();
          orp->orgname = onp;
        }
        if (onp != NULL) {
          mod = OrgModNew ();
          if (onp->mod == NULL) {
            onp->mod = mod;
          } else {
            tmpmod = onp->mod;
            while (tmpmod->next != NULL) {
              tmpmod = tmpmod->next;
            }
            tmpmod->next = mod;
          }
          if (mod != NULL) {
            mod->subtype = subtype;
            mod->subname = StringSave (str);
          }
        }
      }
    }
  }
  MemFree (str);
}

#define PROC_NUC_STR_SIZE 4096

extern Boolean ProcessOneNucleotideTitle (Int2 seqPackage, DialoG genbio, PopuP genome,
                                          PopuP gencode, SeqEntryPtr nsep, SeqEntryPtr top,
                                          BioSourcePtr masterbiop);
extern Boolean ProcessOneNucleotideTitle (Int2 seqPackage, DialoG genbio, PopuP genome,
                                          PopuP gencode, SeqEntryPtr nsep, SeqEntryPtr top,
                                          BioSourcePtr masterbiop)

{
  EnumFieldAssocPtr  ap;
  Uint1              biomol;
  BioSourcePtr       biop;
  BioseqSetPtr       bssp;
  Int2               code;
  CharPtr            lin;
  OrgNamePtr         masteronp = NULL;
  OrgRefPtr          masterorp = NULL;
  MolInfoPtr         mip;
  BioseqPtr          nbsp;
  Boolean            needbiop;
  OrgNamePtr         onp;
  OrgRefPtr          orp;
  CharPtr            ptr;
  SeqEntryPtr        sep;
  CharPtr            str;
  CharPtr            title;
  UIEnum             val;
  ValNodePtr         vnp;

  if (nsep == NULL || top == NULL) return FALSE;
  nbsp = (BioseqPtr) nsep->data.ptrvalue;
  if (nbsp == NULL) return FALSE;
  if (! ISA_na (nbsp->mol)) return FALSE;
  str = MemNew (PROC_NUC_STR_SIZE * sizeof (Char));
  if (str == NULL) return FALSE;
  sep = NULL;
  SeqEntryExplore (top, (Pointer) &sep, FindFirstSeqEntryTitle);
  if (sep != NULL) {
    vnp = SeqEntryGetSeqDescr (sep, Seq_descr_title, NULL);
    if (vnp != NULL && vnp->data.ptrvalue != NULL) {
      title = (CharPtr) vnp->data.ptrvalue;
      needbiop = FALSE;
      if (seqPackage >= SEQ_PKG_POPULATION && seqPackage <= SEQ_PKG_GENBANK) {
        needbiop = TRUE;
        if (GetAppParam ("SEQUIN", "PREFERENCES", "BIOSRCONALL", NULL, str, PROC_NUC_STR_SIZE)) {
          if (StringICmp (str, "FALSE") == 0) {
            needbiop = FALSE;
          }
        }
      }
      if ((! needbiop) && StringISearch (title, "[") != NULL) {
        for (ap = orgmod_subtype_alist; ap->name != NULL; ap++) {
          MakeSearchStringFromAlist (str, ap->name);
          if (StringISearch (title, str) != NULL) {
            needbiop = TRUE;
          }
        }
        for (ap = subsource_subtype_alist; ap->name != NULL; ap++) {
          MakeSearchStringFromAlist (str, ap->name);
          if (StringISearch (title, str) != NULL) {
            needbiop = TRUE;
          }
        }
        if (StringISearch (title, "[note=") != NULL) {
          needbiop = TRUE;
        }
        if (StringISearch (title, "[comment=") != NULL) {
          needbiop = TRUE;
        }
        if (StringISearch (title, "[subsource=") != NULL) {
          needbiop = TRUE;
        }
        /*
        if (StringISearch (title, "[strain=") != NULL ||
            StringISearch (title, "[isolate=") != NULL ||
            StringISearch (title, "[clone=") != NULL) {
          needbiop = TRUE;
        }
        */
      }
      biop = NULL;
      ptr = StringISearch (title, "[org=");
      if (ptr != NULL) {
        StringNCpy_0 (str, ptr + 5, PROC_NUC_STR_SIZE);
      } else {
        ptr = StringISearch (title, "[organism=");
        if (ptr != NULL) {
          StringNCpy_0 (str, ptr + 10, PROC_NUC_STR_SIZE);
        }
      }
      if (ptr != NULL) {
        ptr = StringChr (str, ']');
        if (ptr != NULL) {
          *ptr = '\0';
          if (SetBioSourceDialogTaxName (genbio, str)) {
            biop = (BioSourcePtr) DialogToPointer (genbio);
          } else {
            biop = BioSourceNew ();
            if (biop != NULL) {
              orp = OrgRefNew ();
              biop->org = orp;
              if (orp != NULL) {
                TrimSpacesAroundString (str);
                orp->taxname = StringSave (str);
                if (masterbiop != NULL) {
                  masterorp = masterbiop->org;
                  if (masterorp != NULL) {
                    masteronp = masterorp->orgname;
                    if (masteronp != NULL) {
                      onp = OrgNameNew ();
                      orp->orgname = onp;
                      if (onp != NULL) {
                        onp->gcode = masteronp->gcode;
                        onp->mgcode = masteronp->mgcode;
                      }
                    }
                  }
                }
              }
            }
          }
        }
      } else if (needbiop && masterbiop != NULL) {
        masterorp = masterbiop->org;
        if (masterorp != NULL) {
          if (SetBioSourceDialogTaxName (genbio, masterorp->taxname)) {
            biop = (BioSourcePtr) DialogToPointer (genbio);
          } else {
            biop = BioSourceNew ();
            if (biop != NULL) {
              orp = OrgRefNew ();
              biop->org = orp;
              if (orp != NULL) {
                orp->taxname = StringSave (masterorp->taxname);
                orp->common = StringSave (masterorp->common);
                masteronp = masterorp->orgname;
                if (masteronp != NULL) {
                  onp = OrgNameNew ();
                  orp->orgname = onp;
                  if (onp != NULL) {
                    onp->gcode = masteronp->gcode;
                    onp->mgcode = masteronp->mgcode;
                  }
                }
              }
            }
          }
        }
      }
      if (biop != NULL) {
        ptr = StringISearch (title, "[lineage=");
        if (ptr != NULL) {
          lin = StringSave (ptr + 9);
          if (lin != NULL) {
            ptr = StringChr (lin, ']');
            if (ptr != NULL) {
              *ptr = '\0';
              orp = biop->org;
              if (orp != NULL) {
                onp = orp->orgname;
                if (onp == NULL) {
                  onp = OrgNameNew ();
                  orp->orgname = onp;
                }
                if (onp != NULL) {
                  onp->lineage = MemFree (onp->lineage);
                  onp->lineage = StringSave (lin);
                }
              }
            }
            MemFree (lin);
          }
        }
        if (seqPackage == SEQ_PKG_PHYLOGENETIC) {
          orp = biop->org;
          if (orp != NULL) {
            onp = orp->orgname;
            if (onp == NULL) {
              onp = OrgNameNew ();
              orp->orgname = onp;
            }
            if (onp != NULL) {
              if (onp->gcode == 0 && onp->mgcode == 0) {
                code = gcIndexToId [GetValue (gencode)];
                if (GetEnumPopup (genome, biosource_genome_simple_alist, &val)) {
                  if (val == 4 || val == 5) {
                    onp->mgcode = gcIndexToId [GetValue (gencode)];
                  } else {
                    onp->gcode = gcIndexToId [GetValue (gencode)];
                  }
                  biop->genome = (Uint1) val;
                }
              } else if (GetEnumPopup (genome, biosource_genome_simple_alist, &val)) {
                biop->genome = (Uint1) val;
              }
            }
          }
        }
        vnp = CreateNewDescriptor (top, Seq_descr_source);
        if (vnp != NULL) {
          vnp->data.ptrvalue = (Pointer) biop;
        }
        for (ap = orgmod_subtype_alist; ap->name != NULL; ap++) {
          MakeSearchStringFromAlist (str, ap->name);
          AddToOrgMod (biop, title, str, (Int2) ap->value);
          ExciseString (title, str, "]");
        }
        for (ap = subsource_subtype_alist; ap->name != NULL; ap++) {
          MakeSearchStringFromAlist (str, ap->name);
          AddToSubSource (biop, title, str, (Int2) ap->value);
          ExciseString (title, str, "]");
        }
        AddToOrgMod (biop, title, "[note=", 255);
        ExciseString (title, "[note=", "]");
        AddToOrgMod (biop, title, "[comment=", 255);
        ExciseString (title, "[comment=", "]");
        AddToSubSource (biop, title, "[subsource=", 255);
        ExciseString (title, "[subsource=", "]");
        /*
        AddToOrgMod (biop, title, "[strain=", 2);
        AddToOrgMod (biop, title, "[isolate=", 17);
        AddToSubSource (biop, title, "[clone=", 3);
        ExciseString (title, "[strain=", "]");
        ExciseString (title, "[isolate=", "]");
        ExciseString (title, "[clone=", "]");
        */
        ptr = StringISearch (title, "[molecule=");
        if (ptr != NULL) {
          StringNCpy_0 (str, ptr + 10, PROC_NUC_STR_SIZE);
          ptr = StringChr (str, ']');
          if (ptr != NULL) {
            *ptr = '\0';
            if (StringICmp (str, "dna") == 0) {
              nbsp->mol = Seq_mol_dna;
            } else if (StringICmp (str, "rna") == 0) {
              nbsp->mol = Seq_mol_rna;
            }
          }
        }
        ptr = StringISearch (title, "[location=");
        if (ptr != NULL) {
          StringNCpy_0 (str, ptr + 10, PROC_NUC_STR_SIZE);
          ptr = StringChr (str, ']');
          if (ptr != NULL) {
            *ptr = '\0';
            if (StringICmp (str, "Mitochondrial") == 0) { /* alternative spelling */
              biop->genome = 5;
            }
            for (ap = biosource_genome_simple_alist; ap->name != NULL; ap++) {
              if (StringICmp (str, ap->name) == 0) {
                biop->genome = (Uint1) ap->value;
              }
            }
          }
        }
        ptr = StringISearch (title, "[moltype=");
        if (ptr != NULL) {
          biomol = 0;
          StringNCpy_0 (str, ptr + 9, PROC_NUC_STR_SIZE);
          ptr = StringChr (str, ']');
          if (ptr != NULL) {
            *ptr = '\0';
            if (StringICmp (str, "genomic") == 0) {
              biomol = MOLECULE_TYPE_GENOMIC;
            } else if (StringICmp (str, "mRNA") == 0) {
              biomol = MOLECULE_TYPE_MRNA;
            }
            if (biomol != 0) {
              mip = MolInfoNew ();
              if (mip != NULL) {
                mip->biomol = biomol;
                vnp = CreateNewDescriptor (nsep, Seq_descr_molinfo);
                if (vnp != NULL) {
                  vnp->data.ptrvalue = (Pointer) mip;
                }
              }
            }
          }
        }
        /*
        for (ap = biosource_genome_simple_alist; ap->name != NULL; ap++) {
          MakeSearchStringFromAlist (str, ap->name);
          ptr = StringISearch (str, "=");
          if (ptr != NULL) {
            *ptr = '\0';
          }
          ptr = StringISearch (title, str);
          if (ptr != NULL) {
            biop->genome = (Uint1) ap->value;
          }
          ExciseString (title, str, "]");
        }
        ptr = StringISearch (title, "[dna]");
        if (ptr != NULL) {
          nbsp->mol = Seq_mol_dna;
          ExciseString (title, "[dna", "]");
        }
        ptr = StringISearch (title, "[rna]");
        if (ptr != NULL) {
          nbsp->mol = Seq_mol_rna;
          ExciseString (title, "[rna", "]");
        }
        */
      }
      ExciseString (title, "[org=", "]");
      ExciseString (title, "[organism=", "]");
      ExciseString (title, "[lineage=", "]");
      ExciseString (title, "[molecule=", "]");
      ExciseString (title, "[moltype=", "]");
      ExciseString (title, "[location=", "]");
      TrimSpacesAroundString (title);
      if (StringHasNoText (title) || sep != top) {
        vnp = NULL;
        if (IS_Bioseq (sep)) {
          nbsp = (BioseqPtr) sep->data.ptrvalue;
          vnp = ValNodeExtract (&(nbsp->descr), Seq_descr_title);
        } else if (IS_Bioseq_set (sep)) {
          bssp = (BioseqSetPtr) sep->data.ptrvalue;
          vnp = ValNodeExtract (&(bssp->descr), Seq_descr_title);
        }
        if (vnp != NULL && StringHasNoText ((CharPtr) vnp->data.ptrvalue)) {
          vnp = ValNodeFreeData (vnp);
        }
        if (sep != top && vnp != NULL) {
          if (IS_Bioseq (top)) {
            nbsp = (BioseqPtr) top->data.ptrvalue;
            ValNodeLink (&(nbsp->descr), vnp);
          } else if (IS_Bioseq_set (top)) {
            bssp = (BioseqSetPtr) top->data.ptrvalue;
            ValNodeLink (&(bssp->descr), vnp);
          }
        }
      }
    }
  } else {
    needbiop = FALSE;
    if (seqPackage >= SEQ_PKG_POPULATION && seqPackage <= SEQ_PKG_GENBANK) {
      needbiop = TRUE;
      if (GetAppParam ("SEQUIN", "PREFERENCES", "BIOSRCONALL", NULL, str, PROC_NUC_STR_SIZE)) {
        if (StringICmp (str, "FALSE") == 0) {
          needbiop = FALSE;
        }
      }
    }
    if (needbiop && masterbiop != NULL) {
      masterorp = masterbiop->org;
      if (masterorp != NULL) {
        biop = NULL;
        if (SetBioSourceDialogTaxName (genbio, masterorp->taxname)) {
          biop = (BioSourcePtr) DialogToPointer (genbio);
        } else {
          biop = BioSourceNew ();
          if (biop != NULL) {
            orp = OrgRefNew ();
            biop->org = orp;
            if (orp != NULL) {
              orp->taxname = StringSave (masterorp->taxname);
              orp->common = StringSave (masterorp->common);
              onp = orp->orgname;
              if (onp == NULL) {
                orp->orgname = OrgNameNew ();
                onp = orp->orgname;
              }
              if (onp != NULL) {
                masteronp = masterorp->orgname;
                if (masteronp != NULL) {
                  if (onp->gcode == 0 && onp->mgcode == 0) {
                    onp->gcode = masteronp->gcode;
                    onp->mgcode = masteronp->mgcode;
                  }
                }
                biop->genome = masterbiop->genome;
                if (onp->gcode == 0 && onp->mgcode == 0) {
                  code = gcIndexToId [GetValue (gencode)];
                  if (GetEnumPopup (genome, biosource_genome_simple_alist, &val)) {
                    if (val == 4 || val == 5) {
                      onp->mgcode = gcIndexToId [GetValue (gencode)];
                    } else {
                      onp->gcode = gcIndexToId [GetValue (gencode)];
                    }
                    biop->genome = (Uint1) val;
                  }
                }
              }
            }
          }
        }
        if (biop != NULL) {
          vnp = CreateNewDescriptor (nsep, Seq_descr_source);
          if (vnp != NULL) {
            vnp->data.ptrvalue = (Pointer) biop;
          }
        }
      }
    }
  }
  MemFree (str);
  return TRUE;
}

static Boolean AutomaticNucleotideProcess (SequencesFormPtr sqfp, SeqEntryPtr nsep,
                                           SeqEntryPtr top, BioSourcePtr masterbiop)

{
  BioseqSetPtr  bssp;
  Boolean       rsult;
  SeqEntryPtr   tmp;

  if (sqfp == NULL || nsep == NULL || top == NULL) return FALSE;
  if (IS_Bioseq_set (nsep)) {
    bssp = (BioseqSetPtr) nsep->data.ptrvalue;
    rsult = FALSE;
    if (bssp != NULL) {
      for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
        if (AutomaticNucleotideProcess (sqfp, tmp, top, masterbiop)) {
          rsult = TRUE;
        }
      }
    }
    return rsult;
  }
  return ProcessOneNucleotideTitle (sqfp->seqPackage, sqfp->genbio, sqfp->genome,
                                    sqfp->gencode, nsep, top, masterbiop);
}

typedef struct idlist {
  BioseqPtr  bsp;
  CharPtr    key;
  struct idlist PNTR left;
  struct idlist PNTR right;
} IdList, PNTR IdListPtr;

static void BuildTree (IdListPtr PNTR head, BioseqPtr bsp, CharPtr x)

{
  Int2       comp;
  IdListPtr  idlist;
  SeqIdPtr   sip;
  Char       str [64];

  if (*head != NULL) {
    idlist = *head;
    comp = StringICmp (idlist->key, x);
    if (comp < 0) {
      BuildTree (&(idlist->right), bsp, x);
    } else if (comp > 0) {
      BuildTree (&(idlist->left), bsp, x);
    } else {
      sip = MakeNewProteinSeqId (NULL, NULL);
      if (sip != NULL) {
        bsp->id = SeqIdFree (bsp->id);
        bsp->id = sip;
        SeqMgrReplaceInBioseqIndex (bsp);
        sip = SeqIdFindWorst (bsp->id);
        SeqIdWrite (sip, str, PRINTID_REPORT, sizeof (str));
        BuildTree (head, bsp, str);
      }
    }
  } else {
    idlist = MemNew (sizeof (IdList));
    if (idlist != NULL) {
      *head = idlist;
      idlist->bsp = bsp;
      sip = SeqIdFindWorst (bsp->id);
      SeqIdWrite (sip, str, PRINTID_REPORT, sizeof (str));
      idlist->key = StringSave (str);
      idlist->left = NULL;
      idlist->right = NULL;
    }
  }
}

static void FreeTree (IdListPtr PNTR head)

{
  IdListPtr  idlist;

  if (head != NULL && *head != NULL) {
    idlist = *head;
    FreeTree (&(idlist->left));
    FreeTree (&(idlist->right));
    MemFree (idlist->key);
    MemFree (idlist);
  }
}

static void ResolveCollidingIDs (IdListPtr PNTR head, SeqEntryPtr list)

{
  BioseqPtr  bsp;
  SeqIdPtr   sip;
  Char       str [64];

  if (head == NULL) return;
  while (list != NULL) {
    if (IS_Bioseq (list)) {
      bsp = (BioseqPtr) list->data.ptrvalue;
      if (bsp != NULL) {
        sip = SeqIdFindWorst (bsp->id);
        SeqIdWrite (sip, str, PRINTID_REPORT, sizeof (str));
        BuildTree (head, bsp, str);
      }
    }
    list = list->next;
  }
}

static void PutMolInfoOnSeqEntry (SequencesFormPtr sqfp, SeqEntryPtr sep, Uint1 biomol)

{
  BioseqPtr   bsp;
  MolInfoPtr  mip;
  Boolean     partial5;
  Boolean     partial3;
  UIEnum      val;
  ValNodePtr  vnp;

  if (sqfp != NULL && sep != NULL) {
    mip = MolInfoNew ();
    if (mip != NULL) {
      mip->biomol = biomol;
      partial5 = GetStatus (sqfp->partial5);
      partial3 = GetStatus (sqfp->partial3);
      if (partial5 && partial3) {
        mip->completeness = 5;
      } else if (partial5) {
        mip->completeness = 3;
      } else if (partial3) {
        mip->completeness = 4;
      }
      vnp = CreateNewDescriptor (sep, Seq_descr_molinfo);
      if (vnp != NULL) {
        vnp->data.ptrvalue = (Pointer) mip;
        if (mip->biomol == 1) {
          sqfp->dnamolfrommolinfo = Seq_mol_na;
        } else if (mip->biomol >= 2  && mip->biomol <= 7) {
          sqfp->dnamolfrommolinfo = Seq_mol_rna;
        } else if (mip->biomol == 9) {
          sqfp->dnamolfrommolinfo = Seq_mol_dna;
        } else if (mip->biomol == 253) {
          sqfp->dnamolfrommolinfo = Seq_mol_dna;
          mip->biomol = 1;
        } else if (mip->biomol == 254) {
          sqfp->dnamolfrommolinfo = Seq_mol_rna;
          mip->biomol = 1;
        } else if (mip->biomol == 255) {
          sqfp->dnamolfrommolinfo = Seq_mol_other;
        }
      }
      if (sqfp->seqPackage > SEQ_PKG_GENOMICCDNA && IS_Bioseq (sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp != NULL) {
          if (bsp->mol != Seq_mol_dna && bsp->mol != Seq_mol_rna) {
            bsp->mol = (Uint1) sqfp->dnamolfrommolinfo;
          }
          if (GetEnumPopup (sqfp->topologyPopup, topology_nuc_alist, &val)) {
            bsp->topology = (Uint1) val;
          }
        }
      }
    }
  }
}

static void PrefixOrgToDefline (SeqEntryPtr sep)

{
  BioSourcePtr  biop;
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  CharPtr       def;
  OrgRefPtr     orp;
  CharPtr       ptr;
  CharPtr       str;
  Char          taxname [64];
  ValNodePtr    ttl;
  ValNodePtr    vnp;

  if (sep == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (bssp->_class >= 13 && bssp->_class <= 16))) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        PrefixOrgToDefline (sep);
      }
      return;
    }
  }

  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;

  taxname [0] = '\0';
  orp = NULL;
  biop = NULL;
  ttl = NULL;
  vnp = bsp->descr;
  for (vnp = bsp->descr; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == Seq_descr_source) {
      biop = (BioSourcePtr) vnp->data.ptrvalue;
    } else if (vnp->choice == Seq_descr_org) {
      orp = (OrgRefPtr) vnp->data.ptrvalue;
    } else if (vnp->choice == Seq_descr_title) {
      ttl = vnp;
    }
  }
  if (orp == NULL && biop != NULL) {
    orp = biop->org;
  }
  if (orp == NULL) return;
  if (ttl == NULL) return;
  StringNCpy_0 (taxname, orp->taxname, sizeof (taxname));
  ptr = StringSearch (taxname, "(");
  if (ptr != NULL) {
    *ptr = '\0';
  }
  TrimSpacesAroundString (taxname);
  if (StringICmp (taxname, "Human immunodeficiency virus type 1") == 0) {
    StringCpy (taxname, "HIV-1");
  } else if (StringICmp (taxname, "Human immunodeficiency virus type 2") == 0) {
    StringCpy (taxname, "HIV-2");
  }

  def = (CharPtr) ttl->data.ptrvalue;
  if (StringHasNoText (def)) return;

  ptr = StringISearch (def, taxname);
  if (ptr != NULL && ptr == def) return;
  str = MemNew ((StringLen (taxname) + StringLen (def) + 4) * sizeof (Char));
  if (str == NULL) return;
  StringCpy (str, taxname);
  StringCat (str, " ");
  StringCat (str, def);
  ttl->data.ptrvalue = MemFree (ttl->data.ptrvalue);
  ttl->data.ptrvalue = str;
}

static CharPtr onecomponent = "\
Multiple sequence components are expected in this submission.\n\
They should all be read in at the same time from the same file.";

static void OnlyOneComponentWarning (SequencesFormPtr sqfp)

{
  CharPtr  type;

  if (sqfp != NULL) {
    if (sqfp->seqPackage == SEQ_PKG_GENOMICCDNA) return;
    switch (sqfp->seqPackage) {
      case SEQ_PKG_SEGMENTED :
        type = "segmented sequence";
        break;
      case SEQ_PKG_POPULATION :
        type = "population set";
        break;
      case SEQ_PKG_PHYLOGENETIC :
        type = "phylogenetic set";
        break;
      case SEQ_PKG_MUTATION :
        type = "mutation set";
        break;
      case SEQ_PKG_ENVIRONMENT :
        type = "environmental samples";
        break;
      case SEQ_PKG_GENBANK :
        type = "batch submission";
        break;
      default :
        type = "unknown set";
        break;
    }
    Message (MSG_OK, "WARNING - There is only one component in this %s.\n%s",
             type, onecomponent);
  }
}

static void AutomaticMrnaProcess (SeqEntryPtr nucsep, SeqEntryPtr mrnasep, Boolean partial5, Boolean partial3)

{
  CharPtr     allele = NULL;
  BioseqPtr   bsp;
  GeneRefPtr  grp;
  SeqLocPtr   gslp;
  Boolean     hasNulls;
  MolInfoPtr  mip;
  BioseqPtr   mrnabsp;
  BioseqPtr   nucbsp;
  SeqLocPtr   oldslp;
  CharPtr     ptr;
  RnaRefPtr   rrp;
  SeqFeatPtr  sfp;
  SeqIdPtr    sip;
  SeqLocPtr   slp;
  Char        str [128];
  CharPtr     ttl;
  ValNodePtr  vnp;

  if (nucsep == NULL || mrnasep == NULL) return;
  if (IS_Bioseq (nucsep) && IS_Bioseq (mrnasep)) {
    nucbsp = (BioseqPtr) nucsep->data.ptrvalue;
    mrnabsp = (BioseqPtr) mrnasep->data.ptrvalue;
    if (nucbsp == NULL || mrnabsp == NULL) return;
    slp = AlignmRNA2genomic (nucbsp, mrnabsp);
    if (slp == NULL) return;
    sip = SeqLocId (slp);
    if (sip != NULL) {
      bsp = BioseqFind (sip);
      if (bsp != NULL) {
        if (bsp->repr == Seq_repr_seg) {
          oldslp = slp;
          slp = SegLocToParts (bsp, oldslp);
          FreeAllFuzz (slp);
          SeqLocFree (oldslp);
        }
      }
    }
    StripLocusFromSeqLoc (slp);
    str [0] = '\0';
    ttl = NULL;
    vnp = ValNodeFindNext (mrnabsp->descr, NULL, Seq_descr_title);
    if (vnp != NULL) {
      ttl = (CharPtr) vnp->data.ptrvalue;
    }
    if (ttl != NULL) {
      ptr = StringISearch (ttl, "[gene=");
      if (ptr != NULL) {
        StringNCpy_0 (str, ptr + 6, sizeof (str));
        ptr = StringChr (str, ']');
        if (ptr != NULL) {
          *ptr = '\0';
          ptr = StringChr (str, ';');
          if (ptr != NULL) {
            *ptr = '\0';
            ptr++;
            allele = StringChr (ptr, ';');
            if (allele != NULL) {
              *allele = '\0';
              allele++;
            }
          }
          grp = CreateNewGeneRef (str, allele, ptr, FALSE);
          if (grp != NULL) {
            if (ExtendGene (grp, nucsep, slp)) {
              grp = GeneRefFree (grp);
            } else {
              sfp = CreateNewFeature (nucsep, NULL, SEQFEAT_GENE, NULL);
              if (sfp != NULL) {
                sfp->data.value.ptrvalue = (Pointer) grp;
                sfp->location = SeqLocFree (sfp->location);
                sfp->location = AsnIoMemCopy ((Pointer) slp,
                                              (AsnReadFunc) SeqLocAsnRead,
                                              (AsnWriteFunc) SeqLocAsnWrite);
                sip = SeqLocId (sfp->location);
                if (sip != NULL) {
                  bsp = BioseqFind (sip);
                } else {
                  bsp = nucbsp;
                }
                if (bsp != NULL) {
                  gslp = SeqLocMerge (bsp, sfp->location, NULL, TRUE, FALSE, FALSE);
                  if (gslp != NULL) {
                    sfp->location = SeqLocFree (sfp->location);
                    sfp->location = gslp;
                    if (bsp->repr == Seq_repr_seg) {
                      gslp = SegLocToPartsEx (bsp, sfp->location, TRUE);
                      sfp->location = SeqLocFree (sfp->location);
                      sfp->location = gslp;
                      hasNulls = LocationHasNullsBetween (sfp->location);
                      sfp->partial = (sfp->partial || hasNulls);
                    }
                    FreeAllFuzz (gslp);
                  }
                }
              }
            }
          }
        }
      }
      str [0] = '\0';
      ptr = StringISearch (ttl, "[mrna=");
      if (ptr != NULL) {
        StringNCpy_0 (str, ptr + 6, sizeof (str));
        ptr = StringChr (str, ']');
        if (ptr != NULL) {
          *ptr = '\0';
        }
      } else {
        ptr = StringISearch (ttl, "[cdna=");
        if (ptr != NULL) {
          StringNCpy_0 (str, ptr + 6, sizeof (str));
          ptr = StringChr (str, ']');
          if (ptr != NULL) {
            *ptr = '\0';
          }
        }
      }
    }
    rrp = RnaRefNew ();
    if (rrp != NULL) {
      rrp->type = 2;
      if (! StringHasNoText (str)) {
        rrp->ext.choice = 1;
        rrp->ext.value.ptrvalue = StringSave (str);
      }
      sfp = CreateNewFeature (nucsep, NULL, SEQFEAT_RNA, NULL);
      if (sfp != NULL) {
        sfp->data.value.ptrvalue = (Pointer) rrp;
        sfp->location = SeqLocFree (sfp->location);
        sfp->location = AsnIoMemCopy ((Pointer) slp,
                                      (AsnReadFunc) SeqLocAsnRead,
                                      (AsnWriteFunc) SeqLocAsnWrite);
        SetSeqFeatProduct (sfp, mrnabsp);
        SetSeqLocPartial (sfp->location, partial5, partial3);
        sfp->partial = (sfp->partial || partial5 || partial3);
        if (ttl != NULL) {
          ptr = StringISearch (ttl, "[comment=");
          if (ptr != NULL) {
            StringNCpy_0 (str, ptr + 9, sizeof (str));
            ptr = StringChr (str, ']');
            if (ptr != NULL) {
              *ptr = '\0';
            }
            if (! StringHasNoText (str)) {
              sfp->comment = StringSave (str);
            }
          }
        }
      }
    }
    SeqLocFree (slp);
    ExciseString (ttl, "[mrna", "]");
    ExciseString (ttl, "[cdna", "]");
    ExciseString (ttl, "[gene", "]");
    ExciseString (ttl, "[comment", "]");
    TrimSpacesAroundString (ttl);
    if (StringHasNoText (ttl)) {
      ValNodeExtract (&(mrnabsp->descr), Seq_descr_title);
    }
    mip = MolInfoNew ();
    if (mip != NULL) {
      mip->biomol = 3;
      if (partial5 && partial3) {
        mip->completeness = 5;
      } else if (partial5) {
        mip->completeness = 3;
      } else if (partial3) {
        mip->completeness = 4;
      }
      vnp = CreateNewDescriptor (mrnasep, Seq_descr_molinfo);
      if (vnp != NULL) {
        vnp->data.ptrvalue = (Pointer) mip;
      }
    }
    mrnabsp->mol = Seq_mol_rna;
  }
}

static void ExciseStringFromBioseq (SeqEntryPtr sep, CharPtr from, CharPtr to)

{
  BioseqPtr   bsp;
  CharPtr     title;
  ValNodePtr  vnp;

  if (sep == NULL) return;
  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL || bsp->descr == NULL) return;
  vnp = SeqEntryGetSeqDescr (sep, Seq_descr_title, NULL);
  if (vnp == NULL) return;
  title = (CharPtr) vnp->data.ptrvalue;
  if (title == NULL) return;
  ExciseString (title, from, to);
  TrimSpacesAroundString (title);
  if (StringHasNoText (title)) {
    ValNodeExtract (&(bsp->descr), Seq_descr_title);
  }
}

static Boolean LookForStringInBioseq (SeqEntryPtr sep, Uint1 mol, CharPtr str, CharPtr tmp, size_t maxsize)

{
  BioseqPtr   bsp;
  CharPtr     title;
  ValNodePtr  vnp;

  if (sep == NULL || tmp == NULL) return FALSE;
  if (! IS_Bioseq (sep)) return FALSE;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL || bsp->mol != mol || bsp->descr == NULL) return FALSE;
  vnp = ValNodeFindNext (bsp->descr, NULL, Seq_descr_title);
  if (vnp == NULL || vnp->data.ptrvalue == NULL) return FALSE;
  title = (CharPtr) vnp->data.ptrvalue;
  return LookForSearchString (title, str, tmp, maxsize);
}

static void FindBioseqWithString (SeqEntryPtr sep, Uint1 mol, CharPtr tag, CharPtr str, SeqEntryPtr PNTR rsult)

{
  BioseqPtr     bsp = NULL;
  BioseqSetPtr  bssp = NULL;
  Char          tmp [256];

  if (sep == NULL || sep->data.ptrvalue == NULL || rsult == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (LookForStringInBioseq (sep, mol, tag, tmp, sizeof (tmp) - 1)) {
      if (StringICmp (str, tmp) == 0) {
        *rsult = sep;
        return;
      }
    }
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
      FindBioseqWithString (sep, mol, tag, str, rsult);
    }
  }
}

static SeqEntryPtr FindRnaByRefOnRna (SeqEntryPtr sep, SeqEntryPtr psep)

{
  SeqEntryPtr  msep;
  Char         tmp [256];

  msep = NULL;
  if (sep == NULL || psep == NULL) return NULL;
  if (LookForStringInBioseq (psep, Seq_mol_aa, "[prot=", tmp, sizeof (tmp) - 1)) {
    FindBioseqWithString (sep, Seq_mol_rna, "[prot=", tmp, &msep);
    if (msep != NULL) {
      ExciseStringFromBioseq (msep, "[prot=", "]");
      return msep;
    }
  } else if (LookForStringInBioseq (psep, Seq_mol_aa, "[protein=", tmp, sizeof (tmp) - 1)) {
    FindBioseqWithString (sep, Seq_mol_rna, "[protein=", tmp, &msep);
    if (msep != NULL) {
      ExciseStringFromBioseq (msep, "[protein=", "]");
      return msep;
    }
  }
  return msep;
}

static void FindRnaByName (SeqEntryPtr sep, CharPtr str, SeqEntryPtr PNTR msep)

{
  BioseqPtr     bsp = NULL;
  BioseqSetPtr  bssp = NULL;
  RnaRefPtr     rrp;
  SeqAnnotPtr   sap;
  SeqFeatPtr    sfp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  if (str == NULL || msep == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        if (sfp->data.choice == SEQFEAT_RNA) {
          rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
          if (rrp != NULL && rrp->type == 2 && rrp->ext.choice == 1 && sfp->product != NULL) {
            if (StringICmp (rrp->ext.value.ptrvalue, str) == 0) {
              bsp = BioseqFind (SeqLocId (sfp->product));
              if (bsp != NULL) {
                *msep = SeqMgrGetSeqEntryForData (bsp);
              }
            }
          }
        }
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
  if (bssp != NULL) {
    for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
      FindRnaByName (sep, str, msep);
    }
  }
}

static SeqEntryPtr FindRnaByRefOnProtein (SeqEntryPtr sep, SeqEntryPtr psep)

{
  SeqEntryPtr  msep;
  Char         tmp [256];

  msep = NULL;
  if (sep == NULL || psep == NULL) return NULL;
  if (LookForStringInBioseq (psep, Seq_mol_aa, "[mrna=", tmp, sizeof (tmp) - 1)) {
    FindRnaByName (sep, tmp, &msep);
    if (msep != NULL) {
      ExciseStringFromBioseq (psep, "[mrna=", "]");
      return msep;
    }
  }
  return msep;
}

static void FindRnaByLocationOverlap (SeqEntryPtr sep, SeqLocPtr slp,
                                      Int4Ptr mindiff, SeqEntryPtr PNTR msep)

{
  BioseqPtr     bsp = NULL;
  BioseqSetPtr  bssp = NULL;
  Int4          diff;
  RnaRefPtr     rrp;
  SeqAnnotPtr   sap;
  SeqFeatPtr    sfp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  if (slp == NULL || mindiff == NULL || msep == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        if (sfp->data.choice == SEQFEAT_RNA) {
          rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
          if (rrp != NULL && rrp->type == 2 && sfp->product != NULL) {
            diff = SeqLocAinB (slp, sfp->location);
            if (diff >= 0) {
              if (diff < *mindiff) {
                bsp = BioseqFind (SeqLocId (sfp->product));
                if (bsp != NULL) {
                  *mindiff = diff;
                  *msep = SeqMgrGetSeqEntryForData (bsp);
                }
              }
            }
          }
        }
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
  if (bssp != NULL) {
    for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
      FindRnaByLocationOverlap (sep, slp, mindiff, msep);
    }
  }
}

static void FuseNucProtBiosources (SeqEntryPtr sep)

{
  BioSourcePtr  biop1, biop2;
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  ValNodePtr    PNTR prev;
  ValNodePtr    sdp1, sdp2;
  SeqEntryPtr   tmp;

  if (sep == NULL) return;
  if (! IS_Bioseq_set (sep)) return;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp == NULL || bssp->_class != BioseqseqSet_class_nuc_prot) return;
  tmp = FindNucSeqEntry (sep);
  if (tmp == NULL) return;
  if (! IS_Bioseq (tmp)) return;
  bsp = (BioseqPtr) tmp->data.ptrvalue;
  if (bsp == NULL) return;
  prev = &(bssp->descr);
  sdp1 = bssp->descr;
  while (sdp1 != NULL && sdp1->choice != Seq_descr_source) {
    prev = &(sdp1->next);
    sdp1 = sdp1->next;
  }
  if (sdp1 == NULL) return;
  sdp2 = SeqEntryGetSeqDescr (tmp, Seq_descr_source, NULL);
  if (sdp2 == NULL) return;
  biop1 = (BioSourcePtr) sdp1->data.ptrvalue;
  biop2 = (BioSourcePtr) sdp2->data.ptrvalue;
  if (CmpOrgById (biop1, biop2)) {
    *prev = sdp1->next;
    sdp1->next = NULL;
    SeqDescrFree (sdp1);
  }
}

static Pointer FastaSequencesFormToSeqEntryPtr (ForM f)

{
  Int2              ambig;
  Int2              annotType;
  Uint1             biomol;
  BioSourcePtr      biop;
  BioseqPtr         bsp;
  BioseqSetPtr      bssp;
  Int2              code;
  Int2              count;
  SeqAnnotPtr       curr;
  DatePtr           dp;
  FastaPagePtr      fpp;
  GatherScope       gs;
  IdListPtr         head;
  SeqEntryPtr       list;
  Int4              mindiff;
  MolInfoPtr        mip;
  MonitorPtr        mon;
  SeqEntryPtr       msep;
  SeqEntryPtr       next;
  SeqEntryPtr       nsep;
  BioseqPtr         nucbsp;
  SeqEntryPtr       nucsep;
  ValNodePtr        nulvnp;
  ValNodePtr        nxtvnp;
  Boolean           partialN;
  Boolean           partialC;
  Boolean           partialmRNA5;
  Boolean           partialmRNA3;
  BioseqPtr         protbsp;
  CharPtr           plural;
  SeqAnnotPtr       sap;
  SeqAnnotPtr PNTR  sapp;
  BioseqPtr         segseq;
  SeqEntryPtr       sep;
  SeqIdPtr          sip;
  SeqLocPtr         slp;
  SequencesFormPtr  sqfp;
  Char              str [128];
  Char              tmp [128];
  UIEnum            val;
  ValNodePtr        vnp;
  ObjectIdPtr       oip;
  UserFieldPtr      ufp;
  UserObjectPtr     uop;

  sep = NULL;
  sqfp = (SequencesFormPtr) GetObjectExtra (f);
  if (sqfp != NULL) {
    WatchCursor ();
    Update ();
    head = NULL;
    fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
    if (fpp != NULL) {
      ResolveCollidingIDs (&head, fpp->list);
    }
    fpp = (FastaPagePtr) GetObjectExtra (sqfp->protseq);
    if (fpp != NULL) {
      ResolveCollidingIDs (&head, fpp->list);
    }
    fpp = (FastaPagePtr) GetObjectExtra (sqfp->mrnaseq);
    if (fpp != NULL) {
      ResolveCollidingIDs (&head, fpp->list);
    }
    FreeTree (&head);
    biop = (BioSourcePtr) DialogToPointer (sqfp->genbio);
    if (biop != NULL) {
      code = BioSourceToGeneticCode (biop);
      if (code == 0) {
        code = gcIndexToId [GetValue (sqfp->gencode)];
      }
    } else if (sqfp->seqPackage == SEQ_PKG_PHYLOGENETIC) {
      code = gcIndexToId [GetValue (sqfp->gencode)];
    } else {
      code = 1;
    }
    list = NULL;
    fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
    if (fpp != NULL) {
      list = fpp->list;
      fpp->list = NULL;
    }
    if (sqfp->seqPackage >= SEQ_PKG_GENOMICCDNA) {
      bssp = BioseqSetNew ();
      if (bssp != NULL) {
        switch (sqfp->seqPackage) {
          case SEQ_PKG_GENOMICCDNA :
            bssp->_class = BioseqseqSet_class_gen_prod_set;
            break;
          case SEQ_PKG_POPULATION :
            bssp->_class = 14;
            break;
          case SEQ_PKG_PHYLOGENETIC :
            bssp->_class = 15;
            break;
          case SEQ_PKG_MUTATION :
            bssp->_class = 13;
            break;
          case SEQ_PKG_ENVIRONMENT :
            bssp->_class = 16;
            break;
          case SEQ_PKG_GENBANK :
            bssp->_class = 7;
            break;
          default :
            bssp->_class = 7;
            break;
        }
        sep = SeqEntryNew ();
        if (sep != NULL) {
          sep->choice = 2;
          sep->data.ptrvalue = (Pointer) bssp;
        }
      }
    }
    biomol = 0;
    if (GetEnumPopup (sqfp->moltypePopup, sqfp->moltypeAlist, &val)) {
      biomol = (Uint1) val;
    }
    if (list != NULL) {
      if (sqfp->seqPackage >= SEQ_PKG_SEGMENTED) {
        if (list->next == NULL) {
          OnlyOneComponentWarning (sqfp);
        }
      }
      while (list != NULL) {
        next = list->next;
        list->next = NULL;
        if (sep != NULL) {
          AddSeqEntryToSeqEntry (sep, list, TRUE);
          AutomaticNucleotideProcess (sqfp, list, list, biop);
        } else {
          sep = list;
          AutomaticNucleotideProcess (sqfp, list, list, biop);
        }
        if (sqfp->seqPackage > SEQ_PKG_SEGMENTED) {
          PutMolInfoOnSeqEntry (sqfp, list, biomol);
        }
        list = next;
      }
    }
    if (sep != NULL) {
      sqfp->dnamolfrommolinfo = 0;
      if (sqfp->seqPackage < SEQ_PKG_GENOMICCDNA) {
        PutMolInfoOnSeqEntry (sqfp, sep, biomol);
        if (sqfp->dnamolfrommolinfo > 0 ||
            (GetEnumPopup (sqfp->topologyPopup, topology_nuc_alist, &val) && val > 0)) {
          MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
          gs.seglevels = 1;
          gs.scope = sep;
          MemSet ((Pointer) (gs.ignore), (int) (TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
          gs.ignore[OBJ_BIOSEQ] = FALSE;
          gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
          gs.ignore[OBJ_SEQANNOT] = FALSE;
          gs.ignore[OBJ_SEQDESC] = FALSE;
          GatherSeqEntry (sep, (Pointer) sqfp, UpdateBspMolGatherFunc, &gs);
        }
      } else {
      }
      dp = DateCurr ();
      if (dp != NULL) {
        vnp = CreateNewDescriptor (sep, Seq_descr_create_date);
        if (vnp != NULL) {
          vnp->data.ptrvalue = (Pointer) dp;
        }
      }
    }
    if (sep != NULL && sqfp->seqPackage == SEQ_PKG_SEGMENTED) {
      nsep = FindNucSeqEntry (sep);
      if (nsep != NULL && nsep->choice == 1) {
        segseq = (BioseqPtr) nsep->data.ptrvalue;
        if (segseq != NULL && segseq->repr == Seq_repr_seg && segseq->seq_ext_type == 1) {
          vnp = (ValNodePtr) segseq->seq_ext;
          while (vnp != NULL) {
            nxtvnp = vnp->next;
            if (nxtvnp != NULL && vnp->choice != SEQLOC_NULL) {
              nulvnp = ValNodeNew (NULL);
              if (nulvnp != NULL) {
                nulvnp->choice = SEQLOC_NULL;
                nulvnp->next = nxtvnp;
                vnp->next = nulvnp;
              }
            }
            vnp = nxtvnp;
          }
        }
      }
    }
    if (sqfp->seqPackage >= SEQ_PKG_POPULATION &&
        sqfp->seqPackage <= SEQ_PKG_GENBANK) {
      if (! TextHasNoText (sqfp->defline)) {
        ApplyAnnotationToAll (ADD_TITLE, sep, sqfp->partialLft, sqfp->partialRgt,
                              NULL, NULL, NULL, NULL, sqfp->defline);
      }
      if (GetStatus (sqfp->orgPrefix)) {
        PrefixOrgToDefline (sep);
      }
    }

    if (sep != NULL && sqfp->seqPackage == SEQ_PKG_GENOMICCDNA) {
      list = NULL;
      fpp = (FastaPagePtr) GetObjectExtra (sqfp->mrnaseq);
      if (fpp != NULL) {
        list = fpp->list;
        /* now we will keep instantiated mrna bioseqs */
        fpp->list = NULL;
      }
      /*
      if (list != NULL) {
        nucsep = FindNucSeqEntry (sep);
        if (nucsep != NULL) {
          while (list != NULL) {
            next = list->next;
            AutomaticMrnaProcess (nucsep, list, FALSE, FALSE);
            list = next;
          }
        }
      }
      */
      if (list != NULL) {
        nucsep = FindNucSeqEntry (sep);
        if (nucsep != NULL) {
          partialmRNA5 = GetStatus (sqfp->partialmRNA5);
          partialmRNA3 = GetStatus (sqfp->partialmRNA3);
          while (list != NULL) {
            next = list->next;
            list->next = NULL;
            AddSeqEntryToSeqEntry (sep, list, TRUE);
            AutomaticMrnaProcess (nucsep, list, partialmRNA5, partialmRNA3);
            list = next;
          }
        }
      }
    }

    list = NULL;
    fpp = (FastaPagePtr) GetObjectExtra (sqfp->protseq);
    if (fpp != NULL) {
      list = fpp->list;
      fpp->list = NULL;
    }
    if (list != NULL) {
      nucbsp = NULL;
      nucsep = FindNucSeqEntry (sep);
      if (nucsep != NULL && IS_Bioseq (nucsep)) {
        nucbsp = (BioseqPtr) nucsep->data.ptrvalue;
      }
      if (nucbsp != NULL) {
        SetBatchSuggestNucleotide (nucbsp, code);
      }
      mon = MonitorStrNewEx ("Predicting Coding Region", 20, FALSE);
      count = 0;
      while (list != NULL) {
        next = list->next;
        list->next = NULL;
        count++;
        if (mon != NULL) {
          str [0] = '\0';
          tmp [0] = '\0';
          bsp = (BioseqPtr) list->data.ptrvalue;
          if (bsp != NULL) {
            sip = SeqIdFindWorst (bsp->id);
            SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
          }
          sprintf (str, "Processing sequence %d [%s]", (int) count, tmp);
          MonitorStrValue (mon, str);
          Update ();
        }
        mip = MolInfoNew ();
        if (mip != NULL) {
          mip->biomol = 8;
          if (GetStatus (sqfp->protTechBoth)) {
            mip->tech = 10;
          } else {
            mip->tech = 13;
          }
          partialN = GetStatus (sqfp->partialN);
          partialC = GetStatus (sqfp->partialC);
          if (partialN && partialC) {
            mip->completeness = 5;
          } else if (partialN) {
            mip->completeness = 3;
          } else if (partialC) {
            mip->completeness = 4;
          }
          vnp = CreateNewDescriptor (list, Seq_descr_molinfo);
          if (vnp != NULL) {
            vnp->data.ptrvalue = (Pointer) mip;
          }
        }
        if (sep != NULL) {
          msep = NULL;
          if (sqfp->seqPackage == SEQ_PKG_GENOMICCDNA) {
            ClearBatchSuggestNucleotide ();
            msep = FindRnaByRefOnProtein (sep, list);
            if (msep == NULL) {
              msep = FindRnaByRefOnRna (sep, list);
            }
            if (msep == NULL && nucbsp != NULL && IS_Bioseq (list)) {
              protbsp = (BioseqPtr) list->data.ptrvalue;
              if (protbsp != NULL) {
                slp = PredictCodingRegion (nucbsp, protbsp, code);
                if (slp != NULL) {
                  mindiff = INT4_MAX;
                  FindRnaByLocationOverlap (sep, slp, &mindiff, &msep);
                }
                SeqLocFree (slp);
              }
            }
          }
          if (msep != NULL) {
            msep = GetBestTopParentForDataEx (ObjMgrGetEntityIDForChoice (msep),
                                              (BioseqPtr) msep->data.ptrvalue, TRUE);
          }
          if (msep == NULL) {
            msep = sep;
          }
          AddSeqEntryToSeqEntry (msep, list, TRUE);
          AutomaticProteinProcess (msep, list, code, sqfp->makeMRNA, NULL);
        } else {
          sep = list;
          AutomaticProteinProcess (sep, list, code, sqfp->makeMRNA, NULL);
        }
        list = next;
      }
      if (nucbsp != NULL) {
        ClearBatchSuggestNucleotide ();
      }
      mon = MonitorFree (mon);
      Update ();
    }
    if (biop != NULL) {
      vnp = CreateNewDescriptor (sep, Seq_descr_source);
      if (vnp != NULL) {
        vnp->data.ptrvalue = (Pointer) biop;
      }
    }
    if (sqfp->seqPackage >= SEQ_PKG_POPULATION &&
        sqfp->seqPackage <= SEQ_PKG_GENBANK) {
        if (GetStatus (sqfp->makeAlign)) {
        sap = SeqEntryToSeqAlign (sep, Seq_mol_na);
        if (sap != NULL && sap->type == 2) {

          oip = ObjectIdNew ();
          oip->str = StringSave ("Hist Seqalign");
          uop = UserObjectNew ();
          uop->type = oip;

          oip = ObjectIdNew();
          oip->str = StringSave ("Hist Seqalign");
          ufp = UserFieldNew ();
          ufp->choice = 4;
          ufp->data.boolvalue = TRUE;
          ufp->label = oip;

          uop->data = ufp;
	      ValNodeAddPointer (&(sap->desc), Annot_descr_user, (Pointer) uop);

          sapp = NULL;
          if (IS_Bioseq (sep)) {
            bsp = (BioseqPtr) sep->data.ptrvalue;
            sapp = &(bsp->annot);
          } else if (IS_Bioseq_set (sep)) {
            bssp = (BioseqSetPtr) sep->data.ptrvalue;
            sapp = &(bssp->annot);
          }
          if (sapp != NULL) {
            if (*sapp != NULL) {
              curr = *sapp;
              while (curr->next != NULL) {
                curr = curr->next;
              }
              curr->next = sap;
            } else {
              *sapp = sap;
            }
          }
        }
      }
      annotType = GetValue (sqfp->annotType);
      if (annotType > 0) {
        switch (annotType) {
          case 1 :
            ApplyAnnotationToAll (ADD_IMP, sep, sqfp->partialLft, sqfp->partialRgt,
                                  sqfp->geneName, NULL, NULL,
                                  sqfp->featcomment, NULL);
            break;
          case 2 :
            ApplyAnnotationToAll (ADD_RRNA, sep, sqfp->partialLft, sqfp->partialRgt,
                                  sqfp->geneName, NULL, sqfp->protOrRnaName,
                                  sqfp->featcomment, NULL);
            break;
          case 3 :
            ambig = ApplyAnnotationToAll (ADD_CDS, sep, sqfp->partialLft, sqfp->partialRgt,
                                          sqfp->geneName, sqfp->protOrRnaName, NULL,
                                          sqfp->featcomment, NULL);
            if (ambig > 0) {
              if (ambig > 1) {
                 plural = "records";
               } else {
                 plural = "record";
               }
               Message (MSG_OK, "Possible ambiguous frames detected in %d %s",
                        (int) ambig, plural);
            }
            break;
          default :
            break;
        }
      }
    }
    ArrowCursor ();
    Update ();
  }
  FuseNucProtBiosources (sep);
  return (Pointer) sep;
}

static ValNodePtr FastaTestSequencesForm (ForM f)

{
  BioSourcePtr      biop;
  BioseqPtr         bsp;
  Int2              code;
  FastaPagePtr      fpp;
  ValNodePtr        head;
  ObjectIdPtr       oip;
  CharPtr           ptr;
  SeqEntryPtr       sep;
  SeqIdPtr          sip;
  SequencesFormPtr  sqfp;
  Char              str [128];
  CharPtr           title;

  head = NULL;

  sqfp = (SequencesFormPtr) GetObjectExtra (f);
  if (sqfp != NULL) {

    if (sqfp->seqPackage != SEQ_PKG_PHYLOGENETIC) {
      head = TestDialog (sqfp->genbio);
      if (head != NULL) {
        head->choice = 1;
      } else {
        if (sqfp->seqPackage <= SEQ_PKG_GENOMICCDNA) {
          biop = (BioSourcePtr) DialogToPointer (sqfp->genbio);
          code = BioSourceToGeneticCode (biop);
          BioSourceFree (biop);
          if (code == 0) {
            head = AddStringToValNodeChain (head, "genetic code", 1);
          }
        }
      }
    } else {
      /*
      head = AddStringToValNodeChain (head, "organism name (in FASTA definition line)", 1);
      */
    }

    fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
    if (fpp != NULL) {
      if (fpp->list == NULL) {
        head = AddStringToValNodeChain (head, "nucleotide sequence", 1);
      } else if (head != NULL) {
        sep = fpp->list;
        if (sep != NULL) {
          title = NULL;
          SeqEntryExplore (sep, (Pointer) (&title), FindFirstTitle);
          if (title != NULL) {
            ptr = StringISearch (title, "[org=");
            if (ptr != NULL) {
              StringNCpy_0 (str, ptr + 5, sizeof (str));
              ptr = StringChr (str, ']');
              if (ptr != NULL) {
                *ptr = '\0';
                head = ValNodeFreeData (head);
              }
            } else {
              ptr = StringISearch (title, "[organism=");
              if (ptr != NULL) {
                StringNCpy_0 (str, ptr + 10, sizeof (str));
                ptr = StringChr (str, ']');
                if (ptr != NULL) {
                  *ptr = '\0';
                  head = ValNodeFreeData (head);
                }
              }
            }
          }
        }
      }
      sep = fpp->list;
      if (sep != NULL && sqfp->seqPackage == SEQ_PKG_SINGLE && IS_Bioseq (sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp != NULL) {
          sip = bsp->id;
          if (sip != NULL && sip->choice == SEQID_LOCAL) {
            oip = (ObjectIdPtr) sip->data.ptrvalue;
            if (oip != NULL && oip->str != NULL) {
              if (StringICmp (oip->str, "nuc 1") == 0) {
                GetTitle (sqfp->singleSeqID, str, sizeof (str));
                if (! StringHasNoText (str)) {
                  oip->str = MemFree (oip->str);
                  oip->str = StringSaveNoNull (str);
                  SeqMgrReplaceInBioseqIndex (bsp);
                }
                if (StringICmp (oip->str, "nuc 1") == 0) {
                  head = AddStringToValNodeChain (head, "unique identifier for your nucleotide sequence", 1);
                  SafeShow (sqfp->singleIdGrp);
                }
              }
            }
          }
        }
      }
    }

  }
  return head;
}

static void LaunchSequinQuickGuide (void)

{
  Char       str [256];
#ifdef WIN_MOTIF
  NS_Window  window = NULL;
#endif

  sprintf (str,
           "http://www.ncbi.nlm.nih.gov/Sequin/QuickGuide/sequin.htm#before");
#ifdef WIN_MAC
  Nlm_SendURLAppleEvent (str, "MOSS", NULL);
#endif
#ifdef WIN_MSWIN
  Nlm_MSWin_OpenDocument (str);
#endif
#ifdef WIN_MOTIF
  NS_OpenURL (&window, str, NULL, TRUE);
  NS_WindowFree (window);
#endif
}

extern Boolean allowUnableToProcessMessage;

static CharPtr noOrgInTitleAbort =
"sequences have organism information in titles. " \
"It is critical to annotate the data file with organism and source information. " \
"Sequin will not continue processing this submission. " \
"Please read the Sequin Quick Guide section on preparing the data files before proceeding. " \
"Do you wish to launch your browser on the Sequin Quick Guide automatically?";

static CharPtr pleaseReadLocalGuide =
"Please read your local copy of the Sequin Quick Guide before annotating your data file.";

static CharPtr noSrcInTitleAbort =
"sequences have source information in titles. " \
"It is critical to annotate the data file with organism and source information. " \
"Sequin will continue processing this submission. " \
"However, please consider reading the Sequin Quick Guide section on preparing the data files before proceeding.";

static Pointer PhylipSequencesFormToSeqEntryPtr (ForM f)

{
  Int2              ambig;
  Int2              annotType;
  MsgAnswer         ans;
  Uint2             biomol;
  BioSourcePtr      biop;
  BioseqSetPtr      bssp;
  Int2              code;
  DatePtr           dp;
  CharPtr           plural;
  PhylipPagePtr     ppp;
  SeqEntryPtr       sep;
  Int2              seqtitles;
  Int2              seqtotals;
  Char              str [256];
  SequencesFormPtr  sqfp;
  SeqEntryPtr       tmp;
  CharPtr           ttl;
  UIEnum            val;
  ValNodePtr        vnp;

  sep = NULL;
  sqfp = (SequencesFormPtr) GetObjectExtra (f);
  if (sqfp != NULL) {
    biop = (BioSourcePtr) DialogToPointer (sqfp->genbio);
    if (biop != NULL) {
      code = BioSourceToGeneticCode (biop);
      if (code == 0) {
        code = gcIndexToId [GetValue (sqfp->gencode)];
      }
    } else if (sqfp->seqPackage == SEQ_PKG_PHYLOGENETIC) {
      code = gcIndexToId [GetValue (sqfp->gencode)];
    } else {
      code = 1;
    }
    ppp = (PhylipPagePtr) GetObjectExtra (sqfp->dnaseq);
    if (ppp != NULL) {
      sep = ppp->sep;
      ppp->sep = NULL;
    }
    if (sep != NULL) {
      if (biop != NULL) {
        vnp = CreateNewDescriptor (sep, Seq_descr_source);
        if (vnp != NULL) {
          vnp->data.ptrvalue = (Pointer) biop;
        }
      }
      biomol = 0;
      if (GetEnumPopup (sqfp->moltypePopup, sqfp->moltypeAlist, &val)) {
        biomol = (Uint1) val;
      }
      if (IS_Bioseq_set (sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        if (bssp != NULL && (bssp->_class == 7 ||
                             (bssp->_class >= 13 && bssp->_class <= 16))) {
          seqtitles = 0;
          seqtotals = 0;
          for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
            /*
            ttl = SeqEntryGetTitle (tmp);
            */
            ttl = NULL;
            SeqEntryExplore (sep, (Pointer) (&ttl), FindFirstTitle);
            if (ttl != NULL) {
              if (sqfp->seqPackage == SEQ_PKG_PHYLOGENETIC) {
                if (StringISearch (ttl, "[org=") != NULL ||
                    StringISearch (ttl, "[organism=") != NULL) {
                  seqtitles++;
                }
              } else if (StringISearch (ttl, "[") != NULL) {
                seqtitles++;
              }
            }
            seqtotals++;
          }
          if (seqtotals != seqtitles) {
            sprintf (str, "None");
            if (seqtitles > 0) {
              sprintf (str, "Only %d", (int) seqtitles);
            }
            ArrowCursor ();
            Update ();
            Beep ();
            if (! indexerVersion) {
              if (sqfp->seqPackage == SEQ_PKG_PHYLOGENETIC) {
                ans = Message (MSG_YN, "%s of %d %s", str, (int) seqtotals, noOrgInTitleAbort);
                if (ans == ANS_YES) {
                  LaunchSequinQuickGuide ();
                } else {
                  Message (MSG_OK, "%s", pleaseReadLocalGuide);
                }
                allowUnableToProcessMessage = FALSE;
                QuitProgram ();
                return NULL; /* aborting */
              } else {
                Message (MSG_OK, "%s of %d %s", str, (int) seqtotals, noSrcInTitleAbort);
              }
            } else {
              if (sqfp->seqPackage == SEQ_PKG_PHYLOGENETIC) {
                Message (MSG_OK, "%s of %d %s (Regular version will abort here.)", str, (int) seqtotals, noOrgInTitleAbort);
              } else {
                Message (MSG_OK, "%s of %d %s (Regular version will continue here.)", str, (int) seqtotals, noSrcInTitleAbort);
              }
            }
          }
        }
        if (bssp != NULL) {
          switch (sqfp->seqPackage) {
            case SEQ_PKG_POPULATION :
              bssp->_class = 14;
              break;
            case SEQ_PKG_PHYLOGENETIC :
              bssp->_class = 15;
              break;
            case SEQ_PKG_MUTATION :
              bssp->_class = 13;
              break;
            case SEQ_PKG_ENVIRONMENT :
              bssp->_class = 16;
              break;
            case SEQ_PKG_GENBANK :
              bssp->_class = 7;
              break;
            default :
              bssp->_class = 7;
              break;
          }
          tmp = bssp->seq_set;
          if (tmp == NULL || tmp->next == NULL) {
            OnlyOneComponentWarning (sqfp);
          }
          for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
            AutomaticNucleotideProcess (sqfp, tmp, tmp, biop);
            PutMolInfoOnSeqEntry (sqfp, tmp, biomol);
          }
        }
      } else {
        OnlyOneComponentWarning (sqfp);
        PutMolInfoOnSeqEntry (sqfp, sep, biomol);
      }
      dp = DateCurr ();
      if (dp != NULL) {
        vnp = CreateNewDescriptor (sep, Seq_descr_create_date);
        if (vnp != NULL) {
          vnp->data.ptrvalue = (Pointer) dp;
        }
      }
    }
    if (sqfp->seqPackage >= SEQ_PKG_POPULATION &&
        sqfp->seqPackage <= SEQ_PKG_GENBANK) {
      if (! TextHasNoText (sqfp->defline)) {
        ApplyAnnotationToAll (ADD_TITLE, sep, sqfp->partialLft, sqfp->partialRgt,
                              NULL, NULL, NULL, NULL, sqfp->defline);
      }
      if (GetStatus (sqfp->orgPrefix)) {
        PrefixOrgToDefline (sep);
      }
    }
    annotType = GetValue (sqfp->annotType);
    if (annotType > 0) {
      switch (annotType) {
        case 1 :
          ApplyAnnotationToAll (ADD_IMP, sep, sqfp->partialLft, sqfp->partialRgt,
                                sqfp->geneName, NULL, NULL,
                                sqfp->featcomment, NULL);
          break;
        case 2 :
          ApplyAnnotationToAll (ADD_RRNA, sep, sqfp->partialLft, sqfp->partialRgt,
                                sqfp->geneName, NULL, sqfp->protOrRnaName,
                                sqfp->featcomment, NULL);
          break;
        case 3 :
          ambig = ApplyAnnotationToAll (ADD_CDS, sep, sqfp->partialLft, sqfp->partialRgt,
                                        sqfp->geneName, sqfp->protOrRnaName, NULL,
                                        sqfp->featcomment, NULL);
          if (ambig > 0) {
            if (ambig > 1) {
               plural = "records";
             } else {
               plural = "record";
             }
             Message (MSG_OK, "Possible ambiguous frames detected in %d %s",
                      (int) ambig, plural);
          }
          break;
        default :
          break;
      }
    }
  }
  FuseNucProtBiosources (sep);
  return (Pointer) sep;
}

static ValNodePtr PhylipTestSequencesForm (ForM f)

{
  ValNodePtr        head;
  PhylipPagePtr     ppp;
  CharPtr           ptr;
  SeqEntryPtr       sep;
  SequencesFormPtr  sqfp;
  Char              str [128];
  CharPtr           title;

  head = NULL;

  sqfp = (SequencesFormPtr) GetObjectExtra (f);
  if (sqfp != NULL) {

    if (sqfp->seqPackage != SEQ_PKG_PHYLOGENETIC) {
      head = TestDialog (sqfp->genbio);
      if (head != NULL) {
        head->choice = 1;
      }
    } else {
      /*
      head = AddStringToValNodeChain (head, "organism name (in FASTA definition line)", 1);
      */
    }

    ppp = (PhylipPagePtr) GetObjectExtra (sqfp->dnaseq);
    if (ppp != NULL) {
      if (ppp->sep == NULL) {
        head = AddStringToValNodeChain (head, "nucleotide sequence", 1);
      } else if (head != NULL) {
        sep = ppp->sep;
        if (sep != NULL) {
          title = NULL;
          SeqEntryExplore (sep, (Pointer) (&title), FindFirstTitle);
          if (title != NULL) {
            ptr = StringISearch (title, "[org=");
            if (ptr != NULL) {
              StringNCpy_0 (str, ptr + 5, sizeof (str));
              ptr = StringChr (str, ']');
              if (ptr != NULL) {
                *ptr = '\0';
                head = ValNodeFreeData (head);
              }
            } else {
              ptr = StringISearch (title, "[organism=");
              if (ptr != NULL) {
                StringNCpy_0 (str, ptr + 10, sizeof (str));
                ptr = StringChr (str, ']');
                if (ptr != NULL) {
                  *ptr = '\0';
                  head = ValNodeFreeData (head);
                }
              }
            }
          }
        }
      }
    }

  }
  return head;
}

static Boolean ImportSequencesForm (ForM f, CharPtr filename)

{
  SequencesFormPtr  sqfp;

  sqfp = (SequencesFormPtr) GetObjectExtra (f);
  if (sqfp != NULL) {
    switch (sqfp->tagFromPage [sqfp->currentPage]) {
      case NUCLEOTIDE_PAGE :
        return ImportDialog (sqfp->dnaseq, "");
      case MRNA_PAGE :
        return ImportDialog (sqfp->mrnaseq, "");
      case PROTEIN_PAGE :
        return ImportDialog (sqfp->protseq, "");
      default :
        break;
    }
  }
  return FALSE;
}

static void ImportBtnProc (ButtoN b)

{
  SequencesFormPtr  sqfp;

  sqfp = (SequencesFormPtr) GetObjectExtra (b);
  if (sqfp) {
    ImportSequencesForm (sqfp->form, "");
  }
}

static void SetOrgNucProtImportExportItems (SequencesFormPtr sqfp)

{
  IteM  exportItm;
  IteM  importItm;

  if (sqfp != NULL) {
    importItm = FindFormMenuItem ((BaseFormPtr) sqfp, VIB_MSG_IMPORT);
    exportItm = FindFormMenuItem ((BaseFormPtr) sqfp, VIB_MSG_EXPORT);
    switch (sqfp->tagFromPage [sqfp->currentPage]) {
      case ORGANISM_PAGE :
        SafeSetTitle (importItm, "Import...");
        SafeSetTitle (exportItm, "Export...");
        SafeDisable (importItm);
        SafeDisable (exportItm);
        break;
      case NUCLEOTIDE_PAGE :
        switch (sqfp->seqFormat) {
          case SEQ_FMT_FASTA :
            if (sqfp->seqPackage == SEQ_PKG_GENOMICCDNA) {
              SafeSetTitle (importItm, "Import Genomic FASTA...");
            } else {
              SafeSetTitle (importItm, "Import Nucleotide FASTA...");
            }
            break;
          case SEQ_FMT_CONTIGUOUS :
            SafeSetTitle (importItm, "Import Nucleotide Contiguous...");
            break;
          case SEQ_FMT_INTERLEAVE :
            SafeSetTitle (importItm, "Import Nucleotide Interleaved...");
            break;
          default :
            SafeSetTitle (importItm, "Import Nucleotide FASTA...");
            break;
        }
        SafeSetTitle (exportItm, "Export...");
        SafeEnable (importItm);
        SafeDisable (exportItm);
        break;
      case MRNA_PAGE :
        SafeSetTitle (importItm, "Import Transcript FASTA...");
        SafeSetTitle (exportItm, "Export...");
        SafeEnable (importItm);
        SafeDisable (exportItm);
        break;
      case PROTEIN_PAGE :
        SafeSetTitle (importItm, "Import Protein FASTA...");
        SafeSetTitle (exportItm, "Export...");
        SafeEnable (importItm);
        SafeDisable (exportItm);
        break;
      case ANNOTATE_PAGE :
        SafeSetTitle (importItm, "Import...");
        SafeSetTitle (exportItm, "Export...");
        SafeDisable (importItm);
        SafeDisable (exportItm);
        break;
      default :
        break;
    }
  }
}

static void ChangeSequencesPage (VoidPtr data, Int2 newval, Int2 oldval)

{
  SequencesFormPtr  sqfp;

  sqfp = (SequencesFormPtr) data;
  if (sqfp != NULL) {
    sqfp->currentPage = newval;
    SafeHide (sqfp->pages [oldval]);
    Update ();
    switch (sqfp->tagFromPage [newval]) {
      case ORGANISM_PAGE :
        SendMessageToDialog (sqfp->genbio, VIB_MSG_ENTER);
        break;
      case NUCLEOTIDE_PAGE :
        SendMessageToDialog (sqfp->dnaseq, VIB_MSG_ENTER);
        break;
      case MRNA_PAGE :
        SendMessageToDialog (sqfp->mrnaseq, VIB_MSG_ENTER);
        break;
      case PROTEIN_PAGE :
        SendMessageToDialog (sqfp->protseq, VIB_MSG_ENTER);
        break;
      default :
        break;
    }
    if (newval == 0) {
      SafeSetTitle (sqfp->prevBtn, "<< Prev Form");
    } else {
      SafeSetTitle (sqfp->prevBtn, "<< Prev Page");
    }
    if (newval == sqfp->numPages - 1) {
      SafeSetTitle (sqfp->nextBtn, "Next Form >>");
    } else {
      SafeSetTitle (sqfp->nextBtn, "Next Page >>");
    }
    SetOrgNucProtImportExportItems (sqfp);
    SafeShow (sqfp->pages [newval]);
    Update ();
    switch (sqfp->tagFromPage [newval]) {
      case ORGANISM_PAGE :
        SendHelpScrollMessage (helpForm, "Organism and Sequences Form", "Organism Page");
        break;
      case NUCLEOTIDE_PAGE :
        if (sqfp->seqPackage == SEQ_PKG_GENOMICCDNA) {
          SendHelpScrollMessage (helpForm, "Organism and Sequences Form", "Genomic Page");
        } else {
          SendHelpScrollMessage (helpForm, "Organism and Sequences Form", "Nucleotide Page");
        }
        break;
      case MRNA_PAGE :
        SendHelpScrollMessage (helpForm, "Organism and Sequences Form", "Transcripts Page");
        break;
      case PROTEIN_PAGE :
        SendHelpScrollMessage (helpForm, "Organism and Sequences Form", "Proteins Page");
        break;
      case ANNOTATE_PAGE :
        SendHelpScrollMessage (helpForm, "Organism and Sequences Form", "Annotation Page");
        break;
      default :
        break;
    }
  }
}

static void NextSequencesFormBtn (ButtoN b)

{
  SequencesFormPtr  sqfp;

  sqfp = (SequencesFormPtr) GetObjectExtra (b);
  if (sqfp != NULL) {
    if (sqfp->currentPage < sqfp->numPages - 1) {
      SetValue (sqfp->tbs, sqfp->currentPage + 1);
    } else if (sqfp->goToNext != NULL) {
      (sqfp->goToNext) (b);
    }
  }
}

static void PrevSequencesFormBtn (ButtoN b)

{
  SequencesFormPtr  sqfp;

  sqfp = (SequencesFormPtr) GetObjectExtra (b);
  if (sqfp != NULL) {
    if (sqfp->currentPage > 0) {
      SetValue (sqfp->tbs, sqfp->currentPage - 1);
    } else if (sqfp->goToPrev != NULL) {
      (sqfp->goToPrev) (b);
    }
  }
}

static void SequencesFormDeleteProc (Pointer formDataPtr)

{
  FastaPagePtr      fpp;
  PhylipPagePtr     ppp;
  SequencesFormPtr  sqfp;

  sqfp = (SequencesFormPtr) formDataPtr;
  if (sqfp != NULL) {
    switch (sqfp->tagFromPage [sqfp->currentPage]) {
      case ORGANISM_PAGE :
        ClearText (CurrentVisibleText ());
        break;
      case NUCLEOTIDE_PAGE :
        if (sqfp->seqFormat == SEQ_FMT_FASTA) {
          fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
          if (fpp != NULL) {
            ResetFastaPage (fpp);
            fpp->path [0] = '\0';
            SafeHide (fpp->doc);
            Reset (fpp->doc);
            SafeShow (fpp->instructions);
            Update ();
          }
        } else if (sqfp->seqFormat >= SEQ_FMT_CONTIGUOUS && sqfp->seqFormat <= SEQ_FMT_INTERLEAVE) {
          ppp = (PhylipPagePtr) GetObjectExtra (sqfp->dnaseq);
          if (ppp != NULL) {
            ResetPhylipPage (ppp);
            ppp->path [0] = '\0';
            SafeHide (ppp->doc);
            Reset (ppp->doc);
            SafeShow (ppp->instructions);
            Update ();
          }
        }
        break;
      case MRNA_PAGE :
        if (sqfp->seqFormat == SEQ_FMT_FASTA) {
          fpp = (FastaPagePtr) GetObjectExtra (sqfp->mrnaseq);
          if (fpp != NULL) {
            ResetFastaPage (fpp);
            fpp->path [0] = '\0';
            SafeHide (fpp->doc);
            Reset (fpp->doc);
            SafeShow (fpp->instructions);
            Update ();
          }
        }
        break;
      case PROTEIN_PAGE :
        if (sqfp->seqPackage <= SEQ_PKG_GENOMICCDNA) {
          if (sqfp->seqFormat == SEQ_FMT_FASTA) {
            fpp = (FastaPagePtr) GetObjectExtra (sqfp->protseq);
            if (fpp != NULL) {
              ResetFastaPage (fpp);
              fpp->path [0] = '\0';
              SafeHide (fpp->doc);
              Reset (fpp->doc);
              SafeShow (fpp->instructions);
              Update ();
            }
          }
        } else {
          ClearText (CurrentVisibleText ());
        }
        break;
      default :
        break;
    }
  }
}

static CharPtr  seqSegFormTabs [] = {
  "Organism", "Nucleotide", "Proteins", NULL
};

static CharPtr  cdnaGenFormTabs [] = {
  "Organism", "Genomic", "Transcripts", "Proteins", NULL
};

static CharPtr  popPhyMutFormTabs [] = {
  "Organism", "Nucleotide", "Annotation", NULL
};

static void PasteIntoDialog (DialoG seq)

{
  Char     ch;
  FILE     *fp;
  Char     path [PATH_MAX];
  CharPtr  ptr;
  CharPtr  str;

  if (Nlm_ClipboardHasString ()) {
    TmpNam (path);
    fp = FileOpen (path, "w");
    if (fp == NULL) return;
    str = ClipboardToString ();
    if (str != NULL) {
      ptr = str;
      ch = *ptr;
      while (ch != '\0') {
        if (ch == '\r') {
          *ptr = '\n';
        }
        ptr++;
        ch = *ptr;
      }
      FilePuts (str, fp);
      MemFree (str);
    }
    FileClose (fp);
    ImportDialog (seq, path);
    FileRemove (path);
  }
}

static void SequencesFormMessage (ForM f, Int2 mssg)

{
  SequencesFormPtr  sqfp;

  sqfp = (SequencesFormPtr) GetObjectExtra (f);
  if (sqfp != NULL) {
    switch (mssg) {
      case VIB_MSG_IMPORT :
        ImportSequencesForm (f, NULL);
        break;
      case VIB_MSG_CUT :
        StdCutTextProc (NULL);
        break;
      case VIB_MSG_COPY :
        StdCopyTextProc (NULL);
        break;
      case VIB_MSG_PASTE :
        switch (sqfp->tagFromPage [sqfp->currentPage]) {
          case ORGANISM_PAGE :
            StdPasteTextProc (NULL);
            break;
          case NUCLEOTIDE_PAGE :
            PasteIntoDialog (sqfp->dnaseq);
            break;
          case MRNA_PAGE :
            PasteIntoDialog (sqfp->mrnaseq);
            break;
          case PROTEIN_PAGE :
            PasteIntoDialog (sqfp->protseq);
            break;
          default :
            StdPasteTextProc (NULL);
            break;
        }
        break;
      case VIB_MSG_DELETE :
        SequencesFormDeleteProc (sqfp);
        break;
      default :
        if (sqfp->appmessage != NULL) {
          sqfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void InitOrgNucProtFormActivate (WindoW w)

{
  SequencesFormPtr  sqfp;

  sqfp = (SequencesFormPtr) GetObjectExtra (w);
  if (sqfp != NULL) {
    if (sqfp->activate != NULL) {
      sqfp->activate (w);
    }
    SetOrgNucProtImportExportItems (sqfp);
  }
}

static void ChangeDnaParse (ButtoN b)

{
  FastaPagePtr      fpp;
  SequencesFormPtr  sqfp;

  sqfp = (SequencesFormPtr) GetObjectExtra (b);
  if (sqfp != NULL) {
    fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
    if (fpp != NULL) {
      fpp->parseSeqId = GetStatus (b);
      if (fpp->parseSeqId) {
        SetAppParam ("SEQUIN", "PREFERENCES", "PARSENUCSEQID", "TRUE");
        SafeHide (sqfp->singleIdGrp);
      } else {
        SetAppParam ("SEQUIN", "PREFERENCES", "PARSENUCSEQID", "FALSE");
        SafeShow (sqfp->singleIdGrp);
      }
    }
  }
}

static void ChangeMrnaParse (ButtoN b)

{
  FastaPagePtr      fpp;
  SequencesFormPtr  sqfp;

  sqfp = (SequencesFormPtr) GetObjectExtra (b);
  if (sqfp != NULL) {
    fpp = (FastaPagePtr) GetObjectExtra (sqfp->mrnaseq);
    if (fpp != NULL) {
      fpp->parseSeqId = GetStatus (b);
      if (fpp->parseSeqId) {
        SetAppParam ("SEQUIN", "PREFERENCES", "PARSEMRNASEQID", "TRUE");
      } else {
        SetAppParam ("SEQUIN", "PREFERENCES", "PARSEMRNASEQID", "FALSE");
      }
    }
  }
}

static void ChangeProtParse (ButtoN b)

{
  FastaPagePtr      fpp;
  SequencesFormPtr  sqfp;

  sqfp = (SequencesFormPtr) GetObjectExtra (b);
  if (sqfp != NULL) {
    fpp = (FastaPagePtr) GetObjectExtra (sqfp->protseq);
    if (fpp != NULL) {
      fpp->parseSeqId = GetStatus (b);
      if (fpp->parseSeqId) {
        SetAppParam ("SEQUIN", "PREFERENCES", "PARSEPROTSEQID", "TRUE");
      } else {
        SetAppParam ("SEQUIN", "PREFERENCES", "PARSEPROTSEQID", "FALSE");
      }
    }
  }
}

static void ChangeMrnaFlag (ButtoN b)

{
  SequencesFormPtr  sqfp;

  sqfp = (SequencesFormPtr) GetObjectExtra (b);
  if (sqfp != NULL) {
    sqfp->makeMRNA = GetStatus (b);
    if (sqfp->makeMRNA) {
      SetAppParam ("SEQUIN", "PREFERENCES", "CREATEMRNA", "TRUE");
    } else {
      SetAppParam ("SEQUIN", "PREFERENCES", "CREATEMRNA", "FALSE");
    }
  }
}

static void ChangeAnnotType (GrouP g)

{
  SequencesFormPtr  sqfp;
  Int2              val;

  sqfp = (SequencesFormPtr) GetObjectExtra (g);
  if (sqfp == NULL) return;
  val = GetValue (g);
  switch (val) {
    case 1 :
      SafeHide (sqfp->protOrRnaPpt);
      SafeHide (sqfp->protOrRnaName);
      SafeShow (sqfp->annotGrp);
      Select (sqfp->geneName);
      break;
    case 2 :
      SafeSetTitle (sqfp->protOrRnaPpt, "rRNA Name");
      SafeShow (sqfp->protOrRnaPpt);
      SafeShow (sqfp->protOrRnaName);
      SafeShow (sqfp->annotGrp);
      Select (sqfp->protOrRnaName);
      break;
    case 3 :
      SafeSetTitle (sqfp->protOrRnaPpt, "Protein Name");
      SafeShow (sqfp->protOrRnaPpt);
      SafeShow (sqfp->protOrRnaName);
      SafeShow (sqfp->annotGrp);
      Select (sqfp->protOrRnaName);
      break;
    default :
      SafeHide (sqfp->annotGrp);
      break;
  }
  Update ();
}

static CharPtr  phyloOrgFastaMsg = "\
\nFor phylogenetic studies, you should encode the \
organism names in the individual nucleotide sequence \
FASTA definition lines. These should be of the \
following form:\n\n\
>[org=scientific name]\n\n\
Additional information, e.g., [strain=name], \
can also be included. See the help documentation for \
full details";

static CharPtr  phyloOrgPhylipMsg = "\
\nFor phylogenetic studies, you should encode the \
organism names FASTA-like definition lines after \
the normal PHYLIP, NEXUS or MACAW file. These should \
be of the following form:\n\n\
>[org=scientific name]\n\n\
Additional information, e.g., [strain=name], \
can also be included. See the help documentation for \
full details";

static void PopulateGeneticCodePopup (PopuP gc)

{
  Int2  i;

   if (gc != NULL) {
    PopupItem (gc, " ");
    for (i = 1; i <= numGeneticCodes; i++) {
      PopupItem (gc, gcNames [i]);
    }
  }
}

extern ForM CreateInitOrgNucProtForm (Int2 left, Int2 top, CharPtr title,
                                      FormatBlockPtr format,
                                      BtnActnProc goToNext,
                                      BtnActnProc goBack,
                                      WndActnProc activateForm)

{
  ButtoN             b;
  GrouP              c;
  GrouP              f1, f2, f3;
  GrouP              g;
  GrouP              h;
  Handle             h1, h2, h3, h4;
  GrouP              j;
  GrouP              k;
  ButtoN             mrna;
  GrouP              mult;
  GrouP              p;
  Int2               page;
  Boolean            parseSeqId;
  PrompT             ppt1, ppt2;
  ButtoN             prs;
  GrouP              q;
  StdEditorProcsPtr  sepp;
  Boolean            single;
  SequencesFormPtr   sqfp;
  Char               str [32];
  WindoW             w;
  GrouP              x;
  GrouP              y;
  GrouP              z;

  w = NULL;
  sqfp = MemNew (sizeof (SequencesForm));
  if (sqfp != NULL) {

    if (format != NULL) {
      sqfp->seqPackage = format->seqPackage;
      sqfp->seqFormat = format->seqFormat;
      sqfp->numSeqs = format->numSeqs;
    } else {
      sqfp->seqPackage = SEQ_PKG_SINGLE;
      sqfp->seqFormat = SEQ_FMT_FASTA;
      sqfp->numSeqs = 0;
    }

    w = FixedWindow (left, top, -10, -10, title, NULL);
    SetObjectExtra (w, sqfp, StdCleanupFormProc);
    sqfp->form = (ForM) w;
    sqfp->toform = NULL;
    if (sqfp->seqFormat == SEQ_FMT_FASTA) {
      sqfp->fromform = FastaSequencesFormToSeqEntryPtr;
      sqfp->testform = FastaTestSequencesForm;
    } else if (sqfp->seqFormat >= SEQ_FMT_CONTIGUOUS && sqfp->seqFormat <= SEQ_FMT_INTERLEAVE) {
      sqfp->fromform = PhylipSequencesFormToSeqEntryPtr;
      sqfp->testform = PhylipTestSequencesForm;
    }
    sqfp->importform = ImportSequencesForm;
    sqfp->formmessage = SequencesFormMessage;

#ifndef WIN_MAC
    CreateSqnInitialFormMenus (w);
#endif

    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      sqfp->appmessage = sepp->handleMessages;
    }

    SetGroupSpacing (w, 10, 10);

    j = HiddenGroup (w, 10, 0, NULL);

    if (sqfp->seqPackage <= SEQ_PKG_SEGMENTED) {
      sqfp->tbs = CreateFolderTabs (j, seqSegFormTabs, ORGANISM_PAGE,
                                    0, 0, SYSTEM_FOLDER_TAB,
                                    ChangeSequencesPage, (Pointer) sqfp);
    } else if (sqfp->seqPackage == SEQ_PKG_GENOMICCDNA) {
      sqfp->tbs = CreateFolderTabs (j, cdnaGenFormTabs, ORGANISM_PAGE,
                                    0, 0, SYSTEM_FOLDER_TAB,
                                    ChangeSequencesPage, (Pointer) sqfp);
    } else {
      sqfp->tbs = CreateFolderTabs (j, popPhyMutFormTabs, ORGANISM_PAGE,
                                    0, 0, SYSTEM_FOLDER_TAB,
                                    ChangeSequencesPage, (Pointer) sqfp);
    }
    sqfp->currentPage = 0;
    page = 0;

    h = HiddenGroup (w, 0, 0, NULL);

    q = HiddenGroup (h, 0, 0, NULL);
    SetGroupSpacing (q, 10, 20);
    sqfp->genbio = CreateSimpleBioSourceDialog (q, "");
    if (sqfp->seqPackage == SEQ_PKG_PHYLOGENETIC) {
      p = HiddenGroup (q, -1, 0, NULL);
      SetGroupSpacing (p, 10, 20);
      if (sqfp->seqFormat == SEQ_FMT_FASTA) {
        mult = MultiLinePrompt (p, phyloOrgFastaMsg, 27 * stdCharWidth, programFont);
      } else if (sqfp->seqFormat >= SEQ_FMT_CONTIGUOUS && sqfp->seqFormat <= SEQ_FMT_INTERLEAVE) {
        mult = MultiLinePrompt (p, phyloOrgPhylipMsg, 27 * stdCharWidth, programFont);
      } else {
        mult = MultiLinePrompt (p, phyloOrgFastaMsg, 27 * stdCharWidth, programFont);
      }
      f3 = HiddenGroup (p, -1, 0, NULL);
      SetGroupSpacing (f3, 10, 10);

      f1 = HiddenGroup (f3, 3, 0, NULL);
      StaticPrompt (f1, "Location of Sequence",
                    0, popupMenuHeight, programFont, 'l');
      sqfp->genome = PopupList (f1, TRUE, SetGenome);
      SetObjectExtra (sqfp->genome, GetObjectExtra (sqfp->genbio), NULL);
      InitEnumPopup (sqfp->genome, biosource_genome_simple_alist, NULL);
      SetValue (sqfp->genome, 2);
      ReplaceBioSourceGenomePopup (sqfp->genbio, sqfp->genome);

      f2 = HiddenGroup (f3, 3, 0, NULL);
      StaticPrompt (f2, "Genetic Code for Translation", 0, popupMenuHeight, programFont, 'l');
      sqfp->gencode = PopupList (f2, TRUE, NULL);
      PopulateGeneticCodePopup (sqfp->gencode);
      SetValue (sqfp->gencode, 1);

      AlignObjects (ALIGN_CENTER, (HANDLE) mult, (HANDLE) f3, NULL);
      Hide (sqfp->genbio);
    }
    sqfp->pages [page] = q;
    Hide (sqfp->pages [page]);
    sqfp->tagFromPage [page] = ORGANISM_PAGE;
    page++;

    q = HiddenGroup (h, -1, 0, NULL);
    SetGroupSpacing (q, 10, 20);
    g = HiddenGroup (q, -1, 0, NULL);
    SetGroupSpacing (g, 10, 10);
    x = HiddenGroup (g, -4, 0, NULL);
    StaticPrompt (x, "Molecule", 0, popupMenuHeight, programFont, 'l');
    sqfp->moltypePopup = PopupList (x, TRUE, NULL);
    if (sqfp->seqPackage == SEQ_PKG_GENOMICCDNA) {
      sqfp->moltypeAlist = biomol_nucGen_alist;
    } else {
      sqfp->moltypeAlist = biomol_nucX_alist;
    }
    InitEnumPopup (sqfp->moltypePopup, sqfp->moltypeAlist, NULL);
    if (sqfp->seqPackage >= SEQ_PKG_SEGMENTED) {
      SetEnumPopup (sqfp->moltypePopup, sqfp->moltypeAlist, 253);
    } else {
      SetEnumPopup (sqfp->moltypePopup, sqfp->moltypeAlist, 3);
    }
    StaticPrompt (x, "Topology", 0, popupMenuHeight, programFont, 'c');
    sqfp->topologyPopup = PopupList (x, TRUE, NULL);
    InitEnumPopup (sqfp->topologyPopup, topology_nuc_alist, NULL);
    SetEnumPopup (sqfp->topologyPopup, topology_nuc_alist, TOPOLOGY_LINEAR);
    y = HiddenGroup (g, -2, 0, NULL);
    SetGroupSpacing (y, 10, 2);
    sqfp->partial5 = CheckBox (y, "Incomplete at 5' end", NULL);
    sqfp->partial3 = CheckBox (y, "Incomplete at 3' end", NULL);
    prs = NULL;
    if (sqfp->seqFormat == SEQ_FMT_FASTA) {
      prs = CheckBox (g, "Fasta definition line starts with sequence ID", ChangeDnaParse);
      SetObjectExtra (prs, sqfp, NULL);
      parseSeqId = FALSE;
      if (GetAppParam ("SEQUIN", "PREFERENCES", "PARSENUCSEQID", NULL, str, sizeof (str))) {
        if (StringICmp (str, "TRUE") == 0) {
          parseSeqId = TRUE;
        }
      }
      SetStatus (prs, parseSeqId);
      if (sqfp->seqPackage == SEQ_PKG_SINGLE) {
        sqfp->singleIdGrp = HiddenGroup (g, 2, 0, NULL);
        StaticPrompt (sqfp->singleIdGrp, "Enter unique identifier for this sequence", 0, dialogTextHeight, programFont, 'l');
        sqfp->singleSeqID = DialogText (sqfp->singleIdGrp, "", 6, NULL);
        if (parseSeqId) {
          Hide (sqfp->singleIdGrp);
        }
      }
    }
    if (sqfp->seqPackage >= SEQ_PKG_POPULATION && sqfp->seqFormat == SEQ_FMT_FASTA) {
      sqfp->makeAlign = CheckBox (g, "Create Alignment", NULL);
      /*
      if (sqfp->seqPackage < SEQ_PKG_GENBANK) {
        SetStatus (sqfp->makeAlign, TRUE);
      }
      */
    }

    k = HiddenGroup (g, 0, 2, NULL);
    b = NULL;
    if (sqfp->seqFormat == SEQ_FMT_FASTA) {
      single = (Boolean) (sqfp->seqPackage == SEQ_PKG_SINGLE);
      if (sqfp->seqPackage == SEQ_PKG_GENOMICCDNA) {
        sqfp->dnaseq = CreateFastaDialog (k, "", TRUE, FALSE, fastaGenMsg, parseSeqId, single);
        b = PushButton (g, "Import Genomic FASTA", ImportBtnProc);
      } else {
        sqfp->dnaseq = CreateFastaDialog (k, "", TRUE, FALSE, fastaNucMsg, parseSeqId, single);
        b = PushButton (g, "Import Nucleotide FASTA", ImportBtnProc);
      }
      SetObjectExtra (b, sqfp, NULL);
    } else if (sqfp->seqFormat == SEQ_FMT_CONTIGUOUS) {
      sqfp->dnaseq = CreatePhylipDialog (k, "", phylipNucMsg, sqfp->seqFormat, "");
      b = PushButton (g, "Import Nucleotide Contiguous", ImportBtnProc);
      SetObjectExtra (b, sqfp, NULL);
    } else if (sqfp->seqFormat == SEQ_FMT_INTERLEAVE) {
      sqfp->dnaseq = CreatePhylipDialog (k, "", nexusNucMsg, sqfp->seqFormat, "");
      b = PushButton (g, "Import Nucleotide Interleaved", ImportBtnProc);
      SetObjectExtra (b, sqfp, NULL);
    }
    if (sqfp->makeAlign != NULL) {
      h1 = (Handle) sqfp->makeAlign;
      h2 = (Handle) b;
      h3 = (Handle) prs;
      h4 = (Handle) sqfp->singleIdGrp;
    } else {
      h1 = (Handle) b;
      h2 = (Handle) prs;
      h3 = (Handle) sqfp->singleIdGrp;
      h4 = NULL;
    }
    AlignObjects (ALIGN_CENTER, (HANDLE) x, (HANDLE) y, (HANDLE) k,
                  (HANDLE) h1, (HANDLE) h2, (HANDLE) h3, (HANDLE) h4, NULL);
    sqfp->pages [page] = q;
    Hide (sqfp->pages [page]);
    sqfp->tagFromPage [page] = NUCLEOTIDE_PAGE;
    page++;

    if (sqfp->seqPackage == SEQ_PKG_GENOMICCDNA) {
      q = HiddenGroup (h, -1, 0, NULL);
      SetGroupSpacing (q, 10, 20);
      g = HiddenGroup (q, -1, 0, NULL);
      SetGroupSpacing (g, 10, 10);
      y = HiddenGroup (g, -2, 0, NULL);
      SetGroupSpacing (y, 10, 2);
      sqfp->partialmRNA5 = CheckBox (y, "Incomplete at 5' end", NULL);
      sqfp->partialmRNA3 = CheckBox (y, "Incomplete at 3' end", NULL);
      prs = CheckBox (g, "Fasta definition line starts with sequence ID", ChangeMrnaParse);
      SetObjectExtra (prs, sqfp, NULL);
      parseSeqId = FALSE;
      if (GetAppParam ("SEQUIN", "PREFERENCES", "PARSEMRNASEQID", NULL, str, sizeof (str))) {
        if (StringICmp (str, "TRUE") == 0) {
          parseSeqId = TRUE;
        }
      }
      SetStatus (prs, parseSeqId);

      k = HiddenGroup (g, 0, 2, NULL);
      sqfp->mrnaseq = CreateFastaDialog (k, "", TRUE, TRUE, fastaMrnaMsg, parseSeqId, FALSE);
      b = PushButton (g, "Import Transcript FASTA", ImportBtnProc);
      SetObjectExtra (b, sqfp, NULL);

      AlignObjects (ALIGN_CENTER, (HANDLE) y, (HANDLE) k, (HANDLE) prs, (HANDLE) b, NULL);
      sqfp->pages [page] = q;
      Hide (sqfp->pages [page]);
      sqfp->tagFromPage [page] = MRNA_PAGE;
      page++;
    }

    if (sqfp->seqPackage <= SEQ_PKG_GENOMICCDNA) {
      q = HiddenGroup (h, -1, 0, NULL);
      SetGroupSpacing (q, 10, 20);
      g = HiddenGroup (q, -1, 0, NULL);
      SetGroupSpacing (g, 10, 10);
      x = HiddenGroup (g, -1, 0, NULL);
      sqfp->protTechBoth = CheckBox (x,
             "Conceptual translation confirmed by peptide sequencing", NULL);
      y = HiddenGroup (g, -2, 0, NULL);
      SetGroupSpacing (y, 10, 2);
      sqfp->partialN = CheckBox (y, "Incomplete at NH2 end", NULL);
      sqfp->partialC = CheckBox (y, "Incomplete at CO2H end", NULL);
      prs = CheckBox (g, "Fasta definition line starts with sequence ID", ChangeProtParse);
      SetObjectExtra (prs, sqfp, NULL);
      parseSeqId = FALSE;
      if (GetAppParam ("SEQUIN", "PREFERENCES", "PARSEPROTSEQID", NULL, str, sizeof (str))) {
        if (StringICmp (str, "TRUE") == 0) {
          parseSeqId = TRUE;
        }
      }
      SetStatus (prs, parseSeqId);
      sqfp->makeMRNA = FALSE;
      mrna = NULL;
      if (sqfp->seqPackage != SEQ_PKG_GENOMICCDNA) {
        mrna = CheckBox (g, "Create initial mRNA with CDS intervals", ChangeMrnaFlag);
        SetObjectExtra (mrna, sqfp, NULL);
        if (GetAppParam ("SEQUIN", "PREFERENCES", "CREATEMRNA", NULL, str, sizeof (str))) {
          if (StringICmp (str, "TRUE") == 0) {
            sqfp->makeMRNA = TRUE;
          }
        }
      }
      SetStatus (mrna, sqfp->makeMRNA);
      k = HiddenGroup (g, 0, 2, NULL);
      sqfp->protseq = CreateFastaDialog (k, "", FALSE, FALSE, fastaProtMsg, parseSeqId, FALSE);
      b = PushButton (g, "Import Protein FASTA", ImportBtnProc);
      SetObjectExtra (b, sqfp, NULL);
      AlignObjects (ALIGN_CENTER, (HANDLE) x, (HANDLE) y, (HANDLE) k,
                    (HANDLE) prs, (HANDLE) b, (HANDLE) mrna, NULL);
      sqfp->pages [page] = q;
      Hide (sqfp->pages [page]);
      sqfp->tagFromPage [page] = PROTEIN_PAGE;
      page++;
    } else if (sqfp->seqPackage >= SEQ_PKG_POPULATION) {
      q = HiddenGroup (h, -1, 0, NULL);
      SetGroupSpacing (q, 10, 10);
      ppt1 = StaticPrompt (q, "Add feature across full length of all sequences",
                          0, 0, programFont, 'l');
      sqfp->annotType = HiddenGroup (q, 5, 0, ChangeAnnotType);
      SetObjectExtra (sqfp->annotType, sqfp, NULL);
      RadioButton (sqfp->annotType, "Gene");
      RadioButton (sqfp->annotType, "rRNA");
      RadioButton (sqfp->annotType, "CDS");
      RadioButton (sqfp->annotType, "None");
      SetValue (sqfp->annotType, 1);
      sqfp->annotGrp = HiddenGroup (q, -1, 0, NULL);
      SetGroupSpacing (sqfp->annotGrp, 10, 10);
      x = HiddenGroup (sqfp->annotGrp, 2, 0, NULL);
      sqfp->partialLft = CheckBox (x, "Incomplete at 5' end", NULL);
      sqfp->partialRgt = CheckBox (x, "Incomplete at 3' end", NULL);
      y = HiddenGroup (sqfp->annotGrp, 2, 0, NULL);
      sqfp->protOrRnaPpt = StaticPrompt (y, "Protein Name", 0, dialogTextHeight, programFont, 'l');
      sqfp->protOrRnaName = DialogText (y, "", 20, NULL);
      StaticPrompt (y, "Gene Symbol", 0, dialogTextHeight, programFont, 'l');
      sqfp->geneName = DialogText (y, "", 20, NULL);
      StaticPrompt (y, "Comment", 0, 3 * Nlm_stdLineHeight, programFont, 'l');
      sqfp->featcomment = ScrollText (y, 20, 3, programFont, TRUE, NULL);
      ppt2 = StaticPrompt (q, "Add title to all sequences if not in definition line",
                           0, 0, programFont, 'c');
      z = HiddenGroup (q, 2, 0, NULL);
      StaticPrompt (z, "Title       ", 0, 3 * Nlm_stdLineHeight, programFont, 'c');
      sqfp->defline = ScrollText (z, 20, 3, programFont, TRUE, NULL);
      sqfp->orgPrefix = CheckBox (q, "Prefix title with organism name", NULL);
      AlignObjects (ALIGN_CENTER, (HANDLE) ppt1, (HANDLE) sqfp->annotType,
                    (HANDLE) x, (HANDLE) y, (HANDLE) ppt2, (HANDLE) z,
                    (HANDLE) sqfp->orgPrefix, NULL);
      Hide (sqfp->protOrRnaPpt);
      Hide (sqfp->protOrRnaName);
      /* Hide (sqfp->annotGrp); */
      sqfp->pages [page] = q;
      Hide (sqfp->pages [page]);
      sqfp->tagFromPage [page] = ANNOTATE_PAGE;
      page++;
    }
    sqfp->numPages = page;

    c = HiddenGroup (w, 3, 0, NULL);
    SetGroupSpacing (c, 10, 2);
    sqfp->goToPrev = goBack;
    sqfp->prevBtn = PushButton (c, " << Prev Form ", PrevSequencesFormBtn);
    SetObjectExtra (sqfp->prevBtn, sqfp, NULL);
    sqfp->goToNext = goToNext;
    sqfp->nextBtn = PushButton (c, " Next Page >> ", NextSequencesFormBtn);
    SetObjectExtra (sqfp->nextBtn, sqfp, NULL);

    AlignObjects (ALIGN_CENTER, (HANDLE) j, (HANDLE) c,
                  (HANDLE) sqfp->pages [0], (HANDLE) sqfp->pages [1],
                  (HANDLE) sqfp->pages [2], (HANDLE) sqfp->pages [3], NULL);

    RealizeWindow (w);

    SafeSetTitle (sqfp->prevBtn, "<< Prev Form");
    SafeSetTitle (sqfp->nextBtn, "Next Page >>");

    sqfp->activate = activateForm;
    SetActivate (w, InitOrgNucProtFormActivate);

    SendMessageToDialog (sqfp->tbs, VIB_MSG_INIT);
    SendMessageToDialog (sqfp->genbio, VIB_MSG_INIT);
    SendMessageToDialog (sqfp->dnaseq, VIB_MSG_INIT);
    SendMessageToDialog (sqfp->protseq, VIB_MSG_INIT);

    Show (sqfp->pages [sqfp->currentPage]);
    SendMessageToDialog (sqfp->genbio, VIB_MSG_ENTER);
  }
  return (ForM) w;
}

static void MakePubAndDefLine (SequinBlockPtr sbp, SeqEntryPtr sep)

{
  AffilPtr     affil;
  AuthListPtr  alp;
  CitGenPtr    cgp;
  PubdescPtr   pdp;
  ValNodePtr   pep;
  ValNodePtr   vnp;
  /*
  BioseqSetPtr  bssp;
  Char          str [256];
  CharPtr       ttl;
  */

  if (sep == NULL) return;
  /*
  if (SeqEntryGetTitle (sep) != NULL) return;
  ttl = NULL;
  SeqEntryExplore (sep, (Pointer) (&ttl), FindFirstTitle);
  if (ttl != NULL) {
    vnp = CreateNewDescriptor (sep, Seq_descr_title);
    if (vnp != NULL) {
      StringNCpy_0 (str, ttl, sizeof (str) - 32);
      if (IS_Bioseq_set (sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        if (bssp != NULL && bssp->_class == 1) {
          StringCat (str, ", and translated products");
        }
      }
      vnp->data.ptrvalue = StringSave (str);
    }
  }
  */
  if (sbp == NULL || sbp->citsubauthors == NULL) return;
  pdp = PubdescNew ();
  if (pdp != NULL) {
    vnp = CreateNewDescriptor (sep, Seq_descr_pub);
    if (vnp != NULL) {
      vnp->data.ptrvalue = (Pointer) pdp;
      pdp->reftype = 0;
      pep = ValNodeNew (NULL);
      pdp->pub = pep;
      if (pep != NULL) {
        cgp = CitGenNew ();
        if (cgp != NULL) {
          pep->choice = PUB_Gen;
          pep->data.ptrvalue = cgp;
          cgp->cit = StringSave ("unpublished");
          alp = AsnIoMemCopy ((Pointer) sbp->citsubauthors,
                              (AsnReadFunc) AuthListAsnRead,
                              (AsnWriteFunc) AuthListAsnWrite);
          cgp->authors = alp;
          if (alp != NULL) {
            affil = AsnIoMemCopy ((Pointer) sbp->citsubaffil,
                                  (AsnReadFunc) AffilAsnRead,
                                  (AsnWriteFunc) AffilAsnWrite);
            alp->affil = affil;
            if (affil != NULL) {
              affil->phone = MemFree (affil->phone);
              affil->fax = MemFree (affil->fax);
              affil->email = MemFree (affil->email);
            }
          }
          cgp->title = sbp->citsubtitle;
          sbp->citsubtitle = NULL;
        }
      }
    }
  }
}

static SubmitBlockPtr ConvertSequinBlockToSubmitBlock (SequinBlockPtr sqp)

{
  AffilPtr        affil;
  AuthorPtr       ap;
  AuthListPtr     authors;
  ContactInfoPtr  cip;
  CitSubPtr       csp;
  DatePtr         dp;
  SubmitBlockPtr  sbp;
  Char            str [64];

  sbp = NULL;
  if (sqp != NULL) {
    sbp = SubmitBlockNew ();
    if (sbp != NULL) {
      sbp->subtype = 1;
      sprintf (str, "Sequin %s", SEQUIN_APPLICATION);
      sbp->tool = StringSave (str);
      sbp->reldate = sqp->releasedate;
      dp = sbp->reldate;
      if (dp != NULL && dp->data [0] == 1 && dp->data [1] > 0) {
        if (dp->data [2] == 0) {
          dp->data [2] = 1;
        }
        if (dp->data [3] == 0) {
          switch (dp->data [2]) {
            case 4 :
            case 6 :
            case 9 :
            case 11 :
              dp->data [3] = 30;
              break;
            case 2 :
              dp->data [3] = 28;
              break;
            default :
              dp->data [3] = 31;
              break;
          }
        }
      }
      cip = ContactInfoNew ();
      if (cip != NULL) {
        ap = sqp->contactperson;
        cip->contact = ap;
        if (ap != NULL) {
          affil = sqp->citsubaffil;
          if (affil != NULL) {
            if (ap->affil != NULL) {
              affil->phone = MemFree (affil->phone);
              affil->fax = MemFree (affil->fax);
              affil->email = MemFree (affil->email);
              affil->phone = StringSave (ap->affil->phone);
              affil->fax = StringSave (ap->affil->fax);
              affil->email = StringSave (ap->affil->email);
              ap->affil = AffilFree (ap->affil);
            }
            ap->affil = affil;
          }
        }
      }
      sbp->contact = cip;
      csp = CitSubFromContactInfo (cip);
      sbp->cit = csp;
      if (csp != NULL) {
        authors = csp->authors;
        if (authors != NULL) {
          affil = authors->affil;
          authors->affil = NULL;
          csp->authors = AuthListFree (csp->authors);
          csp->authors = sqp->citsubauthors;
          authors = csp->authors;
          if (authors != NULL) {
            authors->affil = affil;
            if (affil != NULL) {
              affil->phone = MemFree (affil->phone);
              affil->fax = MemFree (affil->fax);
              affil->email = MemFree (affil->email);
            }
          }
        }
      }
      sbp->hup = sqp->holduntilpublished;
    }
    MemFree (sqp);
  }
  return sbp;
}

extern Uint2 PackageFormResults (SequinBlockPtr sbp, SeqEntryPtr sep, Boolean makePubAndDefLine)

{
  Uint2         entityID;
  SeqSubmitPtr  ssp;

  entityID = 0;
  if (sep != NULL) {
    if (sbp != NULL) {
      ssp = SeqSubmitNew ();
      if (ssp != NULL) {
        ssp->datatype = 1;
        ssp->data = (Pointer) sep;
        if (makePubAndDefLine) {
          MakePubAndDefLine (sbp, sep);
        }
        sbp->citsubtitle = MemFree (sbp->citsubtitle);
        ssp->sub = ConvertSequinBlockToSubmitBlock (sbp);
        ObjMgrConnect (OBJ_SEQENTRY, sep->data.ptrvalue, OBJ_SEQSUB, (Pointer) ssp);
        if (! ObjMgrRegister (OBJ_SEQSUB, (Pointer) ssp)) {
          ErrPostEx (SEV_ERROR, 0, 0, "ObjMgrRegister failed.");
        }
      } else {
        if (! ObjMgrRegister (OBJ_SEQENTRY, (Pointer) sep)) {
          ErrPostEx (SEV_ERROR, 0, 0, "ObjMgrRegister failed.");
        }
      }
    } else {
      if (! ObjMgrRegister (OBJ_SEQENTRY, (Pointer) sep)) {
        ErrPostEx (SEV_ERROR, 0, 0, "ObjMgrRegister failed.");
      }
    }
    if (EntrezASN1Detected (sep)) {
      ErrPostEx (SEV_WARNING, 0, 0, "This record was retrieved from Entrez.");
    }
    entityID = ObjMgrGetEntityIDForChoice (sep);
  }
  return entityID;
}

static void GetRawBsps (BioseqPtr bsp, Pointer userdata)

{
  ValNodePtr PNTR  head;

  if (bsp->repr != Seq_repr_raw) return;
  head = (ValNodePtr PNTR) userdata;
  ValNodeAddPointer (head, 0, (Pointer) bsp);
}

static void ParseInMoreProteinsCommon (IteM i, Boolean doSuggest)

{
  SeqEntryPtr  addhere;
  MsgAnswer    ans;
  BaseFormPtr  bfp;
  BioseqPtr    bsp;
  Int2         code;
  Int4         count;
  ValNodePtr   descr;
  CharPtr      errormsg;
  CharPtr      extension;
  FILE         *fp;
  ValNodePtr   head;
  Boolean      isLocalUnknownID;
  SeqEntryPtr  last;
  SeqEntryPtr  list;
  Boolean      makeMRNA;
  MolInfoPtr   mip;
  MonitorPtr   mon;
  SeqEntryPtr  nextsep;
  BioseqPtr    nucbsp;
  SeqEntryPtr  nucsep;
  ObjectIdPtr  oid;
  Boolean      parseSeqId;
  Char         path [PATH_MAX];
  ValNodePtr   rawBsps = NULL;
  ValNodePtr   rawvnp;
  SeqEntryPtr  sep;
  SeqIdPtr     sip;
  SeqLocPtr    slp;
  Char         str [64];
  BioseqPtr    target = NULL;
  Char         tmp [128];
  ValNodePtr   vnp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  extension = GetAppProperty ("FastaProtExtension");
  if (! GetInputFileName (path, sizeof (path), extension, "TEXT")) return;
  fp = FileOpen (path, "r");
  if (fp == NULL) return;

  ans = Message (MSG_YN, "Do FASTA definition lines start with seqID?");
  parseSeqId = (Boolean) (ans == ANS_YES);

  WatchCursor ();
  Update ();

  count = 0;
  list = NULL;
  last = NULL;
  head = NULL;
  errormsg = NULL;
  code = SeqEntryToGeneticCode (sep, NULL, NULL, 0);

  nucsep = FindNucSeqEntry (sep);
  slp = CreateWholeInterval (sep);

  nextsep = SequinFastaToSeqEntryEx (fp, FALSE, &errormsg, parseSeqId, NULL);
  while (nextsep != NULL) {
    count++;
    if (IS_Bioseq (nextsep) && nextsep->data.ptrvalue != NULL) {
      bsp = (BioseqPtr) nextsep->data.ptrvalue;
      isLocalUnknownID = FALSE;
      sip = bsp->id;
      if (sip != NULL && sip->choice == SEQID_LOCAL) {
        oid = (ObjectIdPtr) sip->data.ptrvalue;
        if (oid != NULL && oid->str != NULL) {
          isLocalUnknownID = (Boolean) (StringICmp (oid->str, "unknown") == 0);
        }
      }
      if ((! parseSeqId) || isLocalUnknownID) {
        sip = MakeNewProteinSeqId (slp, NULL);
        if (sip != NULL) {
          bsp->id = SeqIdFree (bsp->id);
          bsp->id = sip;
          SeqMgrReplaceInBioseqIndex (bsp);
        }
      }
    }
    SeqEntryPack (nextsep);
    if (last != NULL) {
      last->next = nextsep;
      last = nextsep;
    } else {
      list = nextsep;
      last = list;
    }
    if (! StringHasNoText (errormsg)) {
      vnp = ValNodeNew (head);
      if (head == NULL) {
        head = vnp;
      }
      if (vnp != NULL) {
        vnp->data.ptrvalue = errormsg;
        errormsg = NULL;
      }
    }
    nextsep = SequinFastaToSeqEntryEx (fp, FALSE, &errormsg, parseSeqId, NULL);
  }

  SeqLocFree (slp);
  FileClose (fp);

  ArrowCursor ();
  Update ();

  if (head != NULL) {
    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      Message (MSG_POSTERR, "%s\n", (CharPtr) vnp->data.ptrvalue);
    }
    ValNodeFreeData (head);
    ans = Message (MSG_YN, "Errors detected - do you wish to proceed?");
    if (ans == ANS_NO) {
      sep = list;
      while (sep != NULL) {
        nextsep = sep->next;
        sep->next = NULL;
        SeqEntryFree (sep);
        sep = nextsep;
      }
      return;
    }
  }

  if (list == NULL) return;
  
  ans = Message (MSG_YN, "Do you wish to make default mRNAs?");
  makeMRNA = (Boolean) (ans == ANS_YES);

  WatchCursor ();
  Update ();

  nucbsp = NULL;
  nucsep = FindNucSeqEntry (sep);
  if (nucsep != NULL && IS_Bioseq (nucsep)) {
    nucbsp = (BioseqPtr) nucsep->data.ptrvalue;
  }
  if (nucbsp != NULL) {
    SetBatchSuggestNucleotide (nucbsp, code);
  }
  descr = ExtractBioSourceAndPubs (sep);
  mon = MonitorStrNewEx ("Predicting Coding Region", 20, FALSE);

  rawBsps = NULL;
  if (! doSuggest) {
    VisitBioseqsInSep (sep, (Pointer) &rawBsps, GetRawBsps);
  }
  rawvnp = rawBsps;

  count = 0;
  while (list != NULL) {
    nextsep = list->next;
    list->next = NULL;
    count++;
    if (mon != NULL) {
      str [0] = '\0';
      tmp [0] = '\0';
      bsp = (BioseqPtr) list->data.ptrvalue;
      if (bsp != NULL) {
        sip = SeqIdFindWorst (bsp->id);
        SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
      }
      sprintf (str, "Processing sequence %d [%s]", (int) count, tmp);
      MonitorStrValue (mon, str);
      Update ();
    }
    mip = MolInfoNew ();
    if (mip != NULL) {
      mip->biomol = 8;
      mip->tech = 13;
      vnp = CreateNewDescriptor (list, Seq_descr_molinfo);
      if (vnp != NULL) {
        vnp->data.ptrvalue = (Pointer) mip;
      }
    }
    addhere = sep;
    target = NULL;
    if (! doSuggest) {
      if (rawvnp != NULL) {
        target = (BioseqPtr) rawvnp->data.ptrvalue;
        if (SeqMgrGetParentOfPart (target, NULL) == NULL) {
          addhere = SeqMgrGetSeqEntryForData (target);
        }
        rawvnp = rawvnp->next;
      }
    }
    AddSeqEntryToSeqEntry (addhere, list, TRUE);
    AutomaticProteinProcess (addhere, list, code, makeMRNA, target);
    list = nextsep;
  }

  ValNodeFree (rawBsps);

  mon = MonitorFree (mon);
  if (nucbsp != NULL) {
    ClearBatchSuggestNucleotide ();
  }
  ReplaceBioSourceAndPubs (sep, descr);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();
}

extern void ParseInMoreProteins (IteM i)

{
  ParseInMoreProteinsCommon (i, TRUE);
}

extern void ParseInProteinsInOrder (IteM i)

{
  ParseInMoreProteinsCommon (i, FALSE);
}

extern void ParseInMoreMRNAs (IteM i)

{
  MsgAnswer    ans;
  BaseFormPtr  bfp;
  BioseqPtr    bsp;
  Int4         count;
  CharPtr      errormsg;
  CharPtr      extension;
  FILE         *fp;
  ValNodePtr   head;
  Boolean      isLocalUnknownID;
  SeqEntryPtr  last;
  SeqEntryPtr  list;
  MonitorPtr   mon;
  SeqEntryPtr  nextsep;
  SeqEntryPtr  nucsep;
  ObjectIdPtr  oid;
  Boolean      parseSeqId;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep;
  SeqIdPtr     sip;
  SeqLocPtr    slp;
  Char         str [32];
  Char         tmp [128];
  ValNodePtr   vnp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  extension = GetAppProperty ("FastaNucExtension");
  if (! GetInputFileName (path, sizeof (path), extension, "TEXT")) return;
  fp = FileOpen (path, "r");
  if (fp == NULL) return;

  ans = Message (MSG_YN, "Do FASTA definition lines start with seqID?");
  parseSeqId = (Boolean) (ans == ANS_YES);

  WatchCursor ();
  Update ();

  count = 0;
  list = NULL;
  last = NULL;
  head = NULL;
  errormsg = NULL;

  nucsep = FindNucSeqEntry (sep);
  slp = CreateWholeInterval (sep);

  nextsep = SequinFastaToSeqEntryEx (fp, TRUE, &errormsg, parseSeqId, NULL);
  while (nextsep != NULL) {
    count++;
    if (IS_Bioseq (nextsep) && nextsep->data.ptrvalue != NULL) {
      bsp = (BioseqPtr) nextsep->data.ptrvalue;
      isLocalUnknownID = FALSE;
      sip = bsp->id;
      if (sip != NULL && sip->choice == SEQID_LOCAL) {
        oid = (ObjectIdPtr) sip->data.ptrvalue;
        if (oid != NULL && oid->str != NULL) {
          isLocalUnknownID = (Boolean) (StringICmp (oid->str, "unknown") == 0);
        }
      }
      if ((! parseSeqId) || isLocalUnknownID) {
        sip = MakeNewProteinSeqId (slp, NULL);
        if (sip != NULL) {
          bsp->id = SeqIdFree (bsp->id);
          bsp->id = sip;
          SeqMgrReplaceInBioseqIndex (bsp);
        }
      }
    }
    SeqEntryPack (nextsep);
    if (last != NULL) {
      last->next = nextsep;
      last = nextsep;
    } else {
      list = nextsep;
      last = list;
    }
    if (! StringHasNoText (errormsg)) {
      vnp = ValNodeNew (head);
      if (head == NULL) {
        head = vnp;
      }
      if (vnp != NULL) {
        vnp->data.ptrvalue = errormsg;
        errormsg = NULL;
      }
    }
    nextsep = SequinFastaToSeqEntryEx (fp, TRUE, &errormsg, parseSeqId, NULL);
  }

  SeqLocFree (slp);
  FileClose (fp);

  ArrowCursor ();
  Update ();

  if (head != NULL) {
    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      Message (MSG_POSTERR, "%s\n", (CharPtr) vnp->data.ptrvalue);
    }
    ValNodeFreeData (head);
    ans = Message (MSG_YN, "Errors detected - do you wish to proceed?");
    if (ans == ANS_NO) {
      sep = list;
      while (sep != NULL) {
        nextsep = sep->next;
        sep->next = NULL;
        SeqEntryFree (sep);
        sep = nextsep;
      }
      return;
    }
  }

  if (list == NULL) return;
  
  WatchCursor ();
  Update ();

  nucsep = FindNucSeqEntry (sep);
  if (nucsep == NULL) return;

  mon = MonitorStrNewEx ("Reading mRNA sequences", 20, FALSE);
  count = 0;
  while (list != NULL) {
    nextsep = list->next;
    list->next = NULL;
    count++;
    if (mon != NULL) {
      str [0] = '\0';
      tmp [0] = '\0';
      bsp = (BioseqPtr) list->data.ptrvalue;
      if (bsp != NULL) {
        sip = SeqIdFindWorst (bsp->id);
        SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
      }
      sprintf (str, "Processing sequence %d [%s]", (int) count, tmp);
      MonitorStrValue (mon, str);
      Update ();
    }
    AutomaticMrnaProcess (nucsep, list, FALSE, FALSE);
    SeqEntryFree (list);
    list = nextsep;
  }
  mon = MonitorFree (mon);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();
}

/*#ifdef ALLOW_DOWNLOAD*/
typedef struct fetchform {
  FORM_MESSAGE_BLOCK
  GrouP           accntype;
  TexT            accession;
  ButtoN          accept;
} FetchForm, PNTR FetchFormPtr;

static void FetchFormMessage (ForM f, Int2 mssg)

{
  FetchFormPtr  ffp;

  ffp = (FetchFormPtr) GetObjectExtra (f);
  if (ffp != NULL) {
    switch (mssg) {
      case VIB_MSG_CUT :
        StdCutTextProc (NULL);
        break;
      case VIB_MSG_COPY :
        StdCopyTextProc (NULL);
        break;
      case VIB_MSG_PASTE :
        StdPasteTextProc (NULL);
        break;
      case VIB_MSG_DELETE :
        StdDeleteTextProc (NULL);
        break;
      default :
        if (ffp->appmessage != NULL) {
          ffp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void ExamineIdProc (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr  bsp;
  Int2       i;
  BoolPtr    idTypes;
  SeqIdPtr   sip;

  if (sep == NULL || sep->data.ptrvalue == NULL || mydata == NULL) return;
  idTypes = (BoolPtr) mydata;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sip = bsp->id;
    while (sip != NULL) {
      i = (Int2) sip->choice;
      if (i >= 0 && i < NUM_SEQID) {
        (idTypes [i])++;
      }
      sip = sip->next;
    }
  }
}

static Boolean OwnedByOtherDatabase (SeqEntryPtr sep, BoolPtr idTypes)

{
  Int2  i;

  if (sep == NULL || idTypes == NULL) return FALSE;
  for (i = 0; i < NUM_SEQID; i++) {
    idTypes [i] = FALSE;
  }
  BioseqExplore (sep, (Pointer) idTypes, ExamineIdProc);
  if (! (idTypes [SEQID_GENBANK])) return TRUE;
  if (idTypes [SEQID_EMBL] || idTypes [SEQID_DDBJ]) return TRUE;
  if (! FindNucSeqEntry (sep)) return TRUE;
  return FALSE;
}

static CharPtr cantUpdateMsg = "Sequin updating is\n\
currently only being tested for GenBank records.\n\
Please send e-mail to info@ncbi.nlm.nih.gov if you\n\
have any questions.";

static Int4 AccessionToGi (CharPtr string)
{
   /*
   CharPtr str;
   LinkSetPtr lsp;
   Int4 gi;

   str = MemNew (StringLen (string) + 10);
   sprintf (str, "\"%s\" [ACCN]", string);
   lsp = EntrezTLEvalString (str, TYP_NT, -1, NULL, NULL);
   MemFree (str);
   if (lsp == NULL) return 0;
   if (lsp->num <= 0) {
       LinkSetFree (lsp);
       return 0;
   }
   gi = lsp->uids [0];
   LinkSetFree (lsp);
   return gi;
   */
   Int4      gi;
   SeqIdPtr  sip;

   sip = SeqIdFromAccessionDotVersion (string);
   if (sip == NULL) return 0;
   gi = GetGIForSeqId (sip);
   SeqIdFree (sip);
   return gi;
}

static void LookForReplacedByCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr   bsp;
  SeqHistPtr  hist;
  BoolPtr     rsult;

  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;
  hist = bsp->hist;
  if (hist == NULL) return;
  if (hist->replaced_by_ids != NULL) {
    rsult = (BoolPtr) mydata;
    if (rsult == NULL) return;
    *rsult = TRUE;
  }
}

#ifdef USE_SMARTNET
extern Pointer ReadFromDirSub (CharPtr accn, Uint2Ptr datatype, Uint2Ptr entityID);
extern Pointer ReadFromSmart (CharPtr accn, Uint2Ptr datatype, Uint2Ptr entityID);
#endif

static void DownloadProc (ButtoN b)

{
  CharPtr       accn = NULL;
  MsgAnswer     ans;
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Pointer       dataptr = NULL;
  Uint2         datatype;
  CharPtr       dbname;
  Uint2         entityID;
  FetchFormPtr  ffp;
  Int2          handled;
  Boolean       idTypes [NUM_SEQID];
  Boolean       isReplaced = FALSE;
  SeqEntryPtr   sep;
  Char          str [32];
  Int4          uid;
  ForM          w;

  ffp = (FetchFormPtr) GetObjectExtra (b);
  if (ffp == NULL) return;
  w = ffp->form;
  Hide (w);
  WatchCursor ();
  Update ();
  GetTitle (ffp->accession, str, sizeof (str));
  if (StringHasNoText (str)) {
    Message (MSG_OK, "Please enter an accession number or gi");
    Show (w);
    Select (w);
    Select (ffp->accession);
    return;
  }
  sep = NULL;
  uid = 0;
  if (! EntrezIsInited ()) {
    /* EntrezBioseqFetchEnable ("Sequin", TRUE); */
    if (! SequinEntrezInit ("Sequin", FALSE, NULL)) {
      Remove (w);
      Show (startupForm);
      Select (startupForm);
      ArrowCursor ();
      return;
    }
  }
  if (GetValue (ffp->accntype) == 1) {
    /*
    sip = ValNodeNew (NULL);
    if (sip != NULL) {
      tsip = TextSeqIdNew ();
      if (tsip != NULL) {
        tsip->accession = StringSave (str);
        sip->choice = SEQID_GENBANK;
        sip->data.ptrvalue = (Pointer) tsip;
        uid = EntrezFindSeqId (sip);
        if (uid == 0) {
          sip->choice = SEQID_EMBL;
          uid = EntrezFindSeqId (sip);
        }
        if (uid == 0) {
          sip->choice = SEQID_DDBJ;
          uid = EntrezFindSeqId (sip);
        }
      }
    }
    SeqIdFree (sip);
    */
    uid = AccessionToGi (str);
    accn = str;
  } else {
    if (! StrToLong (str, &uid)) {
     uid = 0;
    }
  }
  if (uid > 0) {
    sep = EntrezSeqEntryGet (uid, 0);
    /* EntrezFini (); */
    if (sep == NULL) {
      ArrowCursor ();
      Message (MSG_OK, "Unable to find this record in the database.");
      Show (w);
      Select (w);
      Select (ffp->accession);
      return;
    }
    if (IS_Bioseq (sep)) {
      datatype = OBJ_BIOSEQ;
    } else if (IS_Bioseq_set (sep)) {
      datatype = OBJ_BIOSEQSET;
    } else {
      ArrowCursor ();
      Message (MSG_OK, "Unable to find this record in the database.");
      Show (w);
      Select (w);
      Select (ffp->accession);
      return;
    }
    Remove (w);
    SeqEntryExplore (sep, (Pointer) (&isReplaced), LookForReplacedByCallback);
    if (isReplaced) {
      ans = Message (MSG_YN, "This record has been replaced.  Are you sure you want to edit it?");
      if (ans == ANS_NO) {
        SeqEntryFree (sep);
        Show (startupForm);
        Select (startupForm);
        ArrowCursor ();
        return;
      }
    }
    dataptr = (Pointer) sep->data.ptrvalue;
  } else if (! StringHasNoText (accn)) {
#ifdef USE_SMARTNET
    if (accn != NULL) {
      dataptr = ReadFromSmart (accn, &datatype, NULL);
      if (dataptr == NULL) {
        dataptr = ReadFromDirSub (accn, &datatype, NULL);
      }
    }
#endif
  }
  if (dataptr != NULL) {
    entityID = ObjMgrRegister (datatype, dataptr);
    if (dataptr != NULL && entityID > 0) {
      if (datatype == OBJ_SEQSUB || datatype == OBJ_SEQENTRY ||
          datatype == OBJ_BIOSEQ || datatype == OBJ_BIOSEQSET) {
        WatchCursor ();
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
        if (sep != NULL && OwnedByOtherDatabase (sep, idTypes)) {
          dbname = NULL;
          if (idTypes [SEQID_EMBL]) {
            dbname = "EMBL";
          } else if (idTypes [SEQID_DDBJ]) {
            dbname = "DDBJ";
          }
          /*
          if (dbname != NULL) {
            Message (MSG_OK, "This record is owned by %s.  %s", dbname, cantUpdateMsg);
          } else {
            Message (MSG_OK, "This record is not owned by GenBank.  %s", cantUpdateMsg);
          }
          */
        }
        if (datatype != OBJ_SEQSUB && uid > 0) {
          ArrowCursor ();
          Update ();
          if (Message (MSG_YN, repackageMsg) == ANS_YES) {
            globalEntityID = entityID;
            globalsep = sep;
            StringNCpy_0 (globalPath, str, sizeof (globalPath));
            WatchCursor ();
            Update ();
            w = CreateSubmitBlockForm (-50, -33, "Submitting Authors",
                                       FALSE, TRUE, NULL, JustRegisterSeqEntryBtn,
                                       AddSubmitBlockToSeqEntry);
            ArrowCursor ();
            if (w != NULL) {
              Show (w);
              Select (w);
              SendHelpScrollMessage (helpForm, "Submitting Authors Form", NULL);
              return;
            } else {
              Message (MSG_FATAL, "Unable to create window.");
              SeqEntryFree (sep);
              Show (startupForm);
              Select (startupForm);
              return;
            }
          }
        }
        seqviewprocs.filepath = str;
        seqviewprocs.forceSeparateViewer = TRUE;
        handled = GatherProcLaunch (OMPROC_VIEW, FALSE, entityID, 1,
                                    OBJ_BIOSEQ, 0, 0, OBJ_BIOSEQ, 0);
        seqviewprocs.filepath = NULL;
        ArrowCursor ();
        if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
          Message (MSG_FATAL, "Unable to launch viewer.");
          SeqEntryFree (sep);
          Show (startupForm);
          Select (startupForm);
          return;
        } else {
          SendHelpScrollMessage (helpForm, "Editing the Record", NULL);
        }
        ObjMgrSetOptions (OM_OPT_FREE_IF_NO_VIEW, entityID);
        ObjMgrSetDirtyFlag (entityID, TRUE);
      } else {
        Message (MSG_ERROR, "Unable to process object type %d.", (int) datatype);
        ObjMgrDelete (datatype, dataptr);
        Show (startupForm);
        Select (startupForm);
        ArrowCursor ();
      }
    } else {
      Show (startupForm);
      Select (startupForm);
      ArrowCursor ();
    }
  } else {
    /* EntrezFini (); */
    ArrowCursor ();
    Message (MSG_OK, "Unable to find this record in the database.");
    Show (w);
    Select (w);
    Select (ffp->accession);
  }
}

extern void DownloadAndUpdateProc (ButtoN b)

{
  FetchFormPtr  ffp;
  SeqEntryPtr   sep;
  Char          str [32];
  Int4          uid;

  ffp = (FetchFormPtr) GetObjectExtra (b);
  if (ffp == NULL) return;
  Hide (ParentWindow (b));
  WatchCursor ();
  Update ();
  GetTitle (ffp->accession, str, sizeof (str));
  if (StringHasNoText (str)) {
    Remove (ParentWindow (b));
    ArrowCursor ();
    return;
  }
  sep = NULL;
  uid = 0;
  if (! EntrezIsInited ()) {
    /* EntrezBioseqFetchEnable ("Sequin", TRUE); */
    if (! SequinEntrezInit ("Sequin", FALSE, NULL)) {
      Remove (ParentWindow (b));
      ArrowCursor ();
      return;
    }
  }
  if (GetValue (ffp->accntype) == 1) {
    uid = AccessionToGi (str);
  } else {
    if (! StrToLong (str, &uid)) {
     uid = 0;
    }
  }
  Remove (ParentWindow (b));
  ArrowCursor ();
  Update ();
  if (uid > 0) {
    sep = EntrezSeqEntryGet (uid, 0);
    /* EntrezFini (); */
    if (sep == NULL) {
      Message (MSG_OK, "Unable to find this record in the database.");
      return;
    }
    if (IS_Bioseq (sep)) {
    } else if (IS_Bioseq_set (sep)) {
    } else {
      Message (MSG_OK, "Unable to find this record in the database.");
      return;
    }
  }

  SqnReadAlignView (updateTargetBspKludge, sep);
}

static void CancelFetchProc (ButtoN b)

{
  StdCancelButtonProc (b);
  Show (startupForm);
  Select (startupForm);
}

static void FetchTextProc (TexT t)

{
  Boolean       alldigits;
  Char          ch;
  FetchFormPtr  ffp;
  CharPtr       ptr;
  Char          str [32];

  ffp = (FetchFormPtr) GetObjectExtra (t);
  if (ffp == NULL) return;
  GetTitle (t, str, sizeof (str));
  if (StringHasNoText (str)) {
    SafeDisable (ffp->accept);
  } else {
    SafeEnable (ffp->accept);
    TrimSpacesAroundString (str);
    alldigits = TRUE;
    ptr = str;
    ch = *ptr;
    while (ch != '\0') {
      if (! IS_DIGIT (ch)) {
        alldigits = FALSE;
      }
      ptr++;
      ch = *ptr;
    }
    if (alldigits) {
      SafeSetValue (ffp->accntype, 2);
    } else {
      SafeSetValue (ffp->accntype, 1);
    }
  }
}

extern void CommonFetchFromNet (BtnActnProc actn, BtnActnProc cancel)

{
  GrouP              c;
  FetchFormPtr       ffp;
  GrouP              g;
  StdEditorProcsPtr  sepp;
  WindoW             w;

  Hide (startupForm);
  Update ();
  w = NULL;
  ffp = MemNew (sizeof (FetchForm));
  if (ffp != NULL) {
    w = FixedWindow (-50, -33, -10, -10, "Download From Entrez", NULL);
    SetObjectExtra (w, ffp, StdCleanupFormProc);
    ffp->form = (ForM) w;
    ffp->formmessage = FetchFormMessage;

#ifndef WIN_MAC
    CreateSqnInitialFormMenus (w);
#endif

    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      ffp->appmessage = sepp->handleMessages;
    }
    SetGroupSpacing (w, 10, 10);

    g = HiddenGroup (w, -3, 0, NULL);
    StaticPrompt (g, "Type", 0, stdLineHeight, programFont, 'l');
    ffp->accntype = HiddenGroup (g, 4, 0, NULL);
    RadioButton (ffp->accntype, "Accession");
    RadioButton (ffp->accntype, "GI");
    SetValue (ffp->accntype, 1);
    ffp->accession = DialogText (g, "", 6, FetchTextProc);
    SetObjectExtra (ffp->accession, ffp, NULL);

    c = HiddenGroup (w, 4, 0, NULL);
    SetGroupSpacing (c, 10, 2);
    ffp->accept = DefaultButton (c, "Retrieve", actn);
    SetObjectExtra (ffp->accept, ffp, NULL);
    Disable (ffp->accept);
    PushButton (c, "Cancel", cancel);

    AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
    RealizeWindow (w);

    if (sepp != NULL) {
      SetActivate (w, sepp->activateForm);
    }
    Select (ffp->accession);
    Show (w);
    Select (w);
    Update ();
  } else {
    Show (startupForm);
    Select (startupForm);
  }
}

extern void FetchFromNet (ButtoN b)

{
  CommonFetchFromNet (DownloadProc, CancelFetchProc);
}

/*#else
#define FetchFromNet NULL
#endif*/

/*
static Boolean FindPerfectSubMatch (CharPtr prot, CharPtr trans, Int4 start,
                                    Int4 len, Uint1 frame, Int2 strand,
                                    Int4Ptr fromPtr, Int4Ptr toPtr)

{
  int      ch;
  Int2     d [256];
  Int4     from;
  int      i;
  int      j;
  int      k;
  size_t   protLen;
  Boolean  rsult;
  Int4     to;
  size_t   transLen;

  rsult = FALSE;
  from = 0;
  to = 0;
  if (prot != NULL && trans != NULL) {
    protLen = StringLen (prot);
    transLen = StringLen (trans);
    if (protLen <= transLen) {
      for (ch = 0; ch < 256; ch++) {
        d [ch] = protLen;
      }
      for (j = 0; j < protLen - 1; j++) {
        d [(int) prot [j]] = protLen - j - 1;
      }
      i = protLen;
      do {
        j = protLen;
        k = i;
        do {
          k--;
          j--;
        } while (j >= 0 && prot [j] == trans [k]);
        if (j >= 0) {
          i += d [(int) trans [i - 1]];
        }
      } while (j >= 0 && i <= transLen);
      if (j < 0) {
        i -= protLen;
        from = (long) (i * 3 + (frame - 1));
        to = from + 3 * protLen;
        if (trans [i + protLen] == '*') {
          to += 3;
        }
        if (strand == Seq_strand_plus) {
          from += 1;
        } else if (strand == Seq_strand_minus) {
          from = len - from;
          to = len - to + 1;
        }
        rsult = TRUE;
      }
    }
  }
  if (fromPtr != NULL) {
    *fromPtr = from + start;
  }
  if (toPtr != NULL) {
    *toPtr = to + start;
  }
  return rsult;
}

static Boolean CheckOneFrame (BioseqPtr bsp, Int4 start, Int4 len,
                              CharPtr prot, Int2 gencode,
                              Uint1 frame, Int2 strand,
                              Int4Ptr fromPtr, Int4Ptr toPtr)

{
  ByteStorePtr  bs;
  Char          ch;
  ValNodePtr    code;
  CdRegionPtr   crp;
  CharPtr       ptr;
  Boolean       rsult;
  SeqFeatPtr    sfp;
  CharPtr       trans;
  ValNodePtr    vnp;

  rsult = FALSE;
  if (bsp != NULL && gencode > 0) {
    sfp = SeqFeatNew ();
    if (sfp != NULL) {
      sfp->data.choice = SEQFEAT_CDREGION;
      crp = CdRegionNew ();
      sfp->data.value.ptrvalue = (Pointer) crp;
      if (crp != NULL) {
        crp->orf = FALSE;
        crp->conflict = FALSE;
        crp->frame = frame;
        crp->gaps = 0;
        crp->mismatch = 0;
        crp->stops = 0;
        code = ValNodeNew (NULL);
        if (code != NULL) {
          code->choice = 254;
          vnp = ValNodeNew (NULL);
          code->data.ptrvalue = vnp;
          if (vnp != NULL) {
            vnp->choice = 2;
            vnp->data.intvalue = (Int4) gencode;
          }
        }
        crp->genetic_code = code;
        crp->code_break = NULL;
        AddIntToSeqFeat (sfp, start, start + len - 1, bsp, -1, -1, strand);
        trans = NULL;
        bs = ProteinFromCdRegion (sfp, TRUE);
        if (bs != NULL) {
          trans = BSMerge (bs, NULL);
          BSFree (bs);
        }
        if (trans != NULL) {
          ptr = trans;
          ch = *ptr;
          while (ch != '\0') {
            *ptr = TO_UPPER (ch);
            ptr++;
            ch = *ptr;
          }
          if (trans [0] == '-') {
            trans [0] = prot [0];
          }
          rsult = FindPerfectSubMatch (prot, trans, start, len,
                                       frame, strand, fromPtr, toPtr);
          MemFree (trans);
        }
      }
      SeqFeatFree (sfp);
    }
  }
  return rsult;
}

#define PREDICT_BLOCK_SIZE 30000L

static SeqLocPtr FindSingleCodingInterval (BioseqPtr nuc, BioseqPtr prot, Int2 genCode)

{
  Int4        cdsFrom;
  Int4        cdsTo;
  Char        ch;
  Int4        cntr;
  Uint1       frame;
  Int4        from;
  Int4        incr;
  Int4        len;
  Boolean     matched;
  size_t      protLen;
  CharPtr     protstr;
  CharPtr     ptr;
  SeqFeatPtr  sfp;
  SeqLocPtr   slp;
  Int4        start;
  Int2        strand;
  Int4        tmp;
  Int4        to;

  slp = NULL;
  if (nuc != NULL && prot != NULL) {
    cdsFrom = 0;
    cdsTo = 0;
    strand = Seq_strand_unknown;
    protstr = NULL;
    if (prot->length > 0) {
      protstr = BSMerge (prot->seq_data, NULL);
      if (protstr != NULL) {
        ptr = protstr;
        ch = *ptr;
        while (ch != '\0') {
          *ptr = TO_UPPER (ch);
          ptr++;
          ch = *ptr;
        }
        protLen = StringLen (protstr);
        matched = FALSE;
        for (frame = 1; frame <= 3 && (! matched); frame++) {
          strand = Seq_strand_plus;
          start = 0;
          cntr = nuc->length;
          len = MIN (cntr, (Int4) (PREDICT_BLOCK_SIZE + (Int4) protLen * 3L));
          while (len > 0 && (! matched)) {
            incr = MIN (cntr, PREDICT_BLOCK_SIZE);
            matched = CheckOneFrame (nuc, start, len, protstr, genCode, frame,
                                     strand, &cdsFrom, &cdsTo);
            start += incr;
            cntr -= incr;
            len = MIN (cntr, (Int4) (PREDICT_BLOCK_SIZE + (Int4) protLen * 3L));
          }
        }
        for (frame = 1; frame <= 3 && (! matched); frame++) {
          strand = Seq_strand_minus;
          start = 0;
          cntr = nuc->length;
          len = MIN (cntr, (Int4) (PREDICT_BLOCK_SIZE + (Int4) protLen * 3L));
          while (len > 0 && (! matched)) {
            incr = MIN (cntr, PREDICT_BLOCK_SIZE);
            matched = CheckOneFrame (nuc, start, len, protstr, genCode, frame,
                                     strand, &cdsFrom, &cdsTo);
            start += incr;
            cntr -= incr;
            len = MIN (cntr, (Int4) (PREDICT_BLOCK_SIZE + (Int4) protLen * 3L));
          }
        }
        if (matched) {
          sfp = SeqFeatNew ();
          if (sfp != NULL) {
            from = cdsFrom - 1;
            to = cdsTo - 1;
            if (from > to) {
              tmp = from;
              from = to;
              to = tmp;
            }
            AddIntToSeqFeat (sfp, from, to, nuc, -1, -1, strand);
            slp = sfp->location;
            sfp->location = NULL;
          }
          SeqFeatFree (sfp);
        }
      }
      MemFree (protstr);
    }
  }
  return slp;
}
*/

