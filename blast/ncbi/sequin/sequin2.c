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
* $Revision: 6.377 $
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
#include <pmfapi.h>
#include <explore.h>
#include <aliparse.h>
#include <algo/blast/api/twoseq_api.h>
#ifdef WIN_MOTIF
#include <netscape.h>
#endif
#include <actutils.h>

extern EnumFieldAssoc  orgmod_subtype_alist [];
extern EnumFieldAssoc  subsource_subtype_alist [];
extern EnumFieldAssoc  biosource_genome_simple_alist [];

#define PRINTED_INT_MAX_LEN 15

#define CREATE_FASTA_REQUIRED 0
#define CREATE_FASTA_WARNING  1

const char *short_org = "[org=";
const char *long_org = "[Organism=";

/* these functions deal with commonly asked questions about package types - 
 * which ones are sets, which ones are single sequences, which ones have
 * which default molecule types.
 */
static Boolean PackageTypeIsSet (Int2 seqPackage)
{
  if (seqPackage == SEQ_PKG_POPULATION
      || seqPackage == SEQ_PKG_PHYLOGENETIC 
      || seqPackage == SEQ_PKG_MUTATION
      || seqPackage == SEQ_PKG_ENVIRONMENT
      || seqPackage == SEQ_PKG_GENBANK)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
  
}

static Boolean PackageTypeIsSingle (Int2 seqPackage)
{
  if (seqPackage == SEQ_PKG_SINGLE
      || seqPackage == SEQ_PKG_SEGMENTED
      || seqPackage == SEQ_PKG_GAPPED)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

static Boolean PackageTypeDefaultMolTypeIsDNA (Int2 seqPackage)
{
  if (PackageTypeIsSet (seqPackage) || seqPackage == SEQ_PKG_GENOMICCDNA)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/* These functions are used to find titles in SeqEntries */
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

/* These functions are used to change the values of modifiers in definition lines */

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

static CharPtr FindValuePairInDefLine (CharPtr mod_name, CharPtr def_line)
{
  CharPtr mod_name_loc, bracket_loc, eq_loc;
  
  if (mod_name == NULL || def_line == NULL)
  {
    return NULL;
  }
  
  mod_name_loc = StringISearch (def_line, mod_name);
  while (mod_name_loc != NULL)
  {
    if (mod_name_loc == def_line)
    {
      /* no bracket - skip to next occurence */
    }
    else
    {
      bracket_loc = mod_name_loc - 1;
      while (bracket_loc != def_line && isspace (*bracket_loc))
      {
        bracket_loc--;
      }
      if (*bracket_loc == '[')
      {
        eq_loc = mod_name_loc + StringLen (mod_name);
        while (*eq_loc != 0 && *eq_loc != '=')
        {
          eq_loc++;
        }
        if (*eq_loc == '=')
        {
          return bracket_loc;
        }
        else
        {
          /* no equals sign - skip to next occurrence */
        }
      }
      else
      {
        /* no bracket - skip to next occurrence */
      }
    }
    mod_name_loc = StringISearch (mod_name_loc + 1, mod_name);
  }
  return NULL;
}

static void RemoveValuePairFromDefline (CharPtr pair_start, CharPtr pair_end, CharPtr defline)
{
  CharPtr src, dst;

  if (pair_start == NULL || pair_end == NULL || defline == NULL
      || pair_end <= pair_start)
  {
    return;
  }
  
  dst = pair_start;
  src = pair_end;
  while (isspace (*src))
  {
    src++;
  }
  
  while (*src != 0)
  {
    *dst = *src;
    dst++;
    src++;
  }
  *dst = 0;
}

static CharPtr 
ReplaceValueInOneDefLine 
(CharPtr orig_defline,
 CharPtr value_name, 
 CharPtr new_value)
{
  CharPtr new_title;
  Boolean is_org = FALSE;
  Int4    new_title_len = 0;
  Boolean is_nontext;
  CharPtr value_loc = NULL, end_loc = NULL;
  CharPtr tmp_name;
  
  if (StringHasNoText (value_name))
  {
    return orig_defline;
  }
  
  if (StringICmp (value_name, "organism") == 0)
  {
    value_loc = FindValuePairInDefLine ("organism", orig_defline);
    if (value_loc == NULL)
    {
      value_loc = FindValuePairInDefLine ("org", orig_defline);
    }
    is_org = TRUE;
  }
  else
  {
    value_loc = FindValuePairInDefLine (value_name, orig_defline);
  }
  
  if (value_loc != NULL)
  {
    end_loc = StringChr (value_loc, ']');
    if (end_loc == NULL)
    {
      /* no end bracket */
      value_loc = NULL;
    }
    else
    {
      end_loc++;
    }
  }
  
  if (StringHasNoText (new_value))
  {
    if (value_loc == NULL)
    {
      /* old line had no value, no new value provided, no change */
    }
    else
    {
      RemoveValuePairFromDefline (value_loc, end_loc, orig_defline);
    }
  }
  else
  {
    /* keep part before pair, after pair, insert new value in position */
    new_title_len = StringLen (orig_defline)
                               + StringLen (value_name)
                               + StringLen (new_value)
                               + 5;
    new_title = MemNew (new_title_len * sizeof (Char));
    if (new_title != NULL)
    {
      if (value_loc == NULL)
      {
        if (!StringHasNoText (orig_defline))
        {
          StringCpy (new_title, orig_defline);
          StringCat (new_title, " ");
        }
      }
      else if (value_loc > orig_defline)
      {
        StringNCpy (new_title, orig_defline, value_loc - orig_defline);
      }
      StringCat (new_title, "[");
      tmp_name = StringSave (value_name);
      tmp_name [0] = TO_LOWER (tmp_name [0]);
      StringCat (new_title, tmp_name);
      is_nontext = IsNonTextModifier (tmp_name);
      tmp_name = MemFree (tmp_name);
      StringCat (new_title, "=");
      if (!is_nontext)
      {
        StringCat (new_title, new_value);
      }
      StringCat (new_title, "]");
      if (end_loc != NULL && *end_loc != 0)
      {
        StringCat (new_title, end_loc);
      }
      orig_defline = MemFree (orig_defline);
      orig_defline = new_title;
    }
  }  
  TrimSpacesAroundString (orig_defline);
  
  return orig_defline;
}

static void ApplyOneModToSeqEntry (SeqEntryPtr sep, CharPtr mod_name, CharPtr mod_value)
{
  BioseqPtr    bsp = NULL;
  SeqDescrPtr  sdp = NULL;
  
  if (sep == NULL || StringHasNoText (mod_name))
  {
    return;
  }
  
  if (IS_Bioseq (sep))
  {
    bsp = (BioseqPtr) sep->data.ptrvalue;
  }
  else if (IS_Bioseq_set (sep))
  {
    sep = FindNucSeqEntry (sep);
    if (sep != NULL && IS_Bioseq (sep))
    {
      bsp = (BioseqPtr) sep->data.ptrvalue;
    }
  }
  
  if (bsp == NULL)
  {
    return;
  }
  
  for (sdp = bsp->descr; sdp != NULL && sdp->choice != Seq_descr_title; sdp = sdp->next)
  {
  }

  if (sdp == NULL)
  {
    sdp = SeqDescrNew (NULL);
    sdp->choice = Seq_descr_title;
    if (bsp->descr == NULL)
    {
      bsp->descr = sdp;
    }
  }
  if (sdp != NULL)
  {
    sdp->data.ptrvalue = ReplaceValueInOneDefLine (sdp->data.ptrvalue,
                                                   mod_name, mod_value);
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
  Boolean      is_delta;
  Boolean      parseSeqId;
  Boolean      single;
  Int2Ptr      seqPackagePtr;
  ButtoN       import_btn;
  ButtoN       clear_btn;
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

static CharPtr GetModValueFromSeqEntry (SeqEntryPtr sep, CharPtr mod_name)
{
  CharPtr ttl = NULL;
  Char    lookfor [255];
  CharPtr value = NULL;
  Int4    value_len = 0;
  
  if (sep == NULL || StringHasNoText (mod_name))
  {
    return NULL;
  }

  SeqEntryExplore (sep, (Pointer) (&ttl), FindFirstTitle);
  if (StringHasNoText (ttl))
  {
    return NULL;
  }
  
  value_len = StringLen (ttl) + 1;
  value = (CharPtr) MemNew (value_len * sizeof (Char));
  
  if (value == NULL)
  {
    return NULL;
  }
  
  MakeSearchStringFromAlist (lookfor, mod_name);

  if (!LookForSearchString (ttl, lookfor, value, value_len - 1))
  {
    if (StringICmp (mod_name, "organism"))
    {
      MakeSearchStringFromAlist (lookfor, "org");
      if (!LookForSearchString (ttl, lookfor, value, value_len - 1))
      {
        value = MemFree (value);
      }
    }
    else if (StringICmp (mod_name, "org"))
    {
      MakeSearchStringFromAlist (lookfor, "organism");
      if (!LookForSearchString (ttl, lookfor, value, value_len - 1))
      {
        value = MemFree (value);
      }
    }
  }
  return value;  
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

extern SeqEntryPtr ImportOneGappedSequence (FILE *fp)
{
  BioseqPtr      bsp;
  Pointer        dataptr;
  Uint2          datatype;
  SeqEntryPtr    topsep;
  SeqSubmitPtr   ssp;
  ErrSev         oldsev;
  
  if (fp == NULL) return NULL;
  
  oldsev = ErrSetMessageLevel (SEV_MAX);
  bsp = ReadDeltaFasta (fp, NULL);
  ErrSetMessageLevel (oldsev);
  if (bsp == NULL)
  {
    topsep = NULL;
    dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, FALSE,
		  		    TRUE, FALSE);
    if (dataptr != NULL)
    {
      /* Get a pointer to the new SeqEntry */
      if (datatype == OBJ_SEQENTRY)
      {
        topsep = (SeqEntryPtr) dataptr;
      }
      else if (datatype == OBJ_BIOSEQ || datatype == OBJ_BIOSEQSET)
      {
        topsep = SeqMgrGetSeqEntryForData (dataptr);
      }
      else if (datatype == OBJ_SEQSUB) 
      {
        ssp = (SeqSubmitPtr) dataptr;
        if (ssp != NULL && ssp->datatype == 1)
        {
          topsep = (SeqEntryPtr) ssp->data;
        }
      }
    }
  }
  else
  {
    topsep = SeqMgrGetSeqEntryForData (bsp);
  }

  return topsep;
}

static SeqEntryPtr SegsetFromSeqEntryList (SeqEntryPtr list)
{
  SeqEntryPtr  first_sep, tmp_sep, next_sep;
  BioseqPtr    bsp;
  SeqDescrPtr  sdp = NULL, set_sdp;
  
  if (list == NULL)
  {
    return NULL;
  }
  
  first_sep = list;
  next_sep = first_sep->next;
  first_sep->next = NULL;
  
  /* grab title on first sequence to put on segmented bioseq */
  if (IS_Bioseq (first_sep) && first_sep->data.ptrvalue != NULL)
  {
    bsp = (BioseqPtr) first_sep->data.ptrvalue;
    sdp = bsp->descr;
    while (sdp != NULL && sdp->choice != Seq_descr_title)
    {
      sdp = sdp->next;
    }
  }

  while (next_sep != NULL)
  {
    tmp_sep = next_sep;
    next_sep = tmp_sep->next;
    tmp_sep->next = NULL;
    AddSeqEntryToSeqEntry (first_sep, tmp_sep, TRUE);
  }
  
  if (sdp != NULL && IS_Bioseq_set (first_sep))
  {
    tmp_sep = FindNucSeqEntry (first_sep);
    if (tmp_sep != NULL && IS_Bioseq (tmp_sep) && tmp_sep->data.ptrvalue != NULL)
    {
      bsp = tmp_sep->data.ptrvalue;
      set_sdp = bsp->descr;
      while (set_sdp != NULL && set_sdp->choice != Seq_descr_title)
      {
        set_sdp = set_sdp->next;
      }
      if (set_sdp == NULL)
      {
        set_sdp = CreateNewDescriptor (tmp_sep, Seq_descr_title);
      }
      if (set_sdp != NULL && StringHasNoText (set_sdp->data.ptrvalue))
      {
        set_sdp->data.ptrvalue = MemFree (set_sdp->data.ptrvalue);
        set_sdp->data.ptrvalue = sdp->data.ptrvalue;
        sdp->data.ptrvalue = StringSave ("");
      }
    }
  }  
  
  return first_sep;
}

static void ReplaceFakeIDWithIDFromTitle (BioseqPtr bsp);

static SeqEntryPtr ReadOneSegSet (FILE *fp, Boolean parse_id, ValNodePtr PNTR err_msg_list)
{
  SeqEntryPtr nextsep;
  CharPtr     errormsg = NULL;
  Char        lastchar;
  SeqEntryPtr seg_list = NULL, seg_list_last = NULL;
  BioseqPtr   bsp;
  
  if (fp == NULL)
  {
    return NULL;
  }
  
  nextsep = SequinFastaToSeqEntryEx (fp, TRUE, &errormsg, FALSE, &lastchar);
  while (nextsep != NULL ||
         (lastchar != (Char) EOF && lastchar != NULLB && lastchar != (Char) 255
          && lastchar != ']')) 
  {
    if (nextsep != NULL) 
    {
      /* replace fake ID with ID from title */
      if (IS_Bioseq (nextsep) && nextsep->data.ptrvalue != NULL)
      {
        bsp = (BioseqPtr) nextsep->data.ptrvalue;
        if (parse_id)
        {
          ReplaceFakeIDWithIDFromTitle ((BioseqPtr) nextsep->data.ptrvalue);
        }
        else
        {
          bsp->id = SeqIdFree (bsp->id);
        }
      }
      SeqEntryPack (nextsep); 
      if (seg_list_last == NULL)
      {
        seg_list = nextsep;
      }
      else
      {
        seg_list_last->next = nextsep;
      }
      seg_list_last = nextsep;
      
      ValNodeAddPointer (err_msg_list, 0, errormsg);
      errormsg = NULL;
    }
    nextsep = SequinFastaToSeqEntryEx (fp, FALSE, &errormsg, FALSE, &lastchar);
  }
  nextsep = SegsetFromSeqEntryList (seg_list);
  return nextsep;
}

static void AddDefaultMoleculeTypeAndLocation (SeqEntryPtr seq_list, Int2 seqPackage)
{
  SeqEntryPtr sep;
  CharPtr     old_value;
  
  for (sep = seq_list; sep != NULL; sep = sep->next)
  {
    old_value = GetModValueFromSeqEntry (sep, "moltype");
    if (StringHasNoText (old_value))
    {
      if (PackageTypeIsSet (seqPackage) || seqPackage == SEQ_PKG_GENOMICCDNA) 
      {
        ApplyOneModToSeqEntry (sep, "moltype", "dna");
      }
      else
      {
        ApplyOneModToSeqEntry (sep, "moltype", "rna");
      }
    }
    old_value = MemFree (old_value);
    old_value = GetModValueFromSeqEntry (sep, "location");
    if (StringHasNoText (old_value))
    {
      ApplyOneModToSeqEntry (sep, "location", "genomic");
    }
    old_value = MemFree (old_value);
  }  
}

static SeqEntryPtr 
ImportSequencesFromFile
(FILE           *fp, 
 SeqEntryPtr     sep_list,
 Boolean         is_na, 
 Boolean         parse_id,
 ValNodePtr PNTR err_msg_list)
{
  Int4          count;
  SeqEntryPtr   last;
  SeqEntryPtr   lastsep = NULL;
  Char          lastchar;
  Boolean       insegset = FALSE;
  SeqEntryPtr   nextsep;
  CharPtr       errormsg = NULL;
  BioseqPtr     bsp = NULL;
  SeqEntryPtr   new_sep_list = NULL;
  SeqEntryPtr   seg_list = NULL, seg_list_last = NULL;
  ErrSev        oldsev;
  Boolean       read_from_delta;
  
  count = 0;
  
  new_sep_list = NULL;
  last = NULL;
  
  bsp = NULL;
  if (is_na)
  {
    oldsev = ErrSetMessageLevel (SEV_MAX);
    bsp = ReadDeltaFasta (fp, NULL);
    ErrSetMessageLevel (oldsev);
  }
  if (bsp == NULL)
  {
    nextsep = SequinFastaToSeqEntryEx (fp, is_na, &errormsg, FALSE, &lastchar);
    read_from_delta = FALSE;
  }
  else
  {
    nextsep = SeqMgrGetSeqEntryForData (bsp);
    lastchar = 'a';
    read_from_delta = TRUE;
  }
  while (nextsep != NULL ||
         (lastchar != (Char) EOF && lastchar != NULLB && lastchar != (Char) 255)) 
  {
    if (nextsep != NULL) 
    {
      if (!read_from_delta
          && IS_Bioseq (nextsep) 
          && nextsep->data.ptrvalue != NULL)
      {
        bsp = (BioseqPtr) nextsep->data.ptrvalue;
        /* replace fake ID with ID from title for sequences that aren't deltas */
        if (parse_id)
        {
          ReplaceFakeIDWithIDFromTitle ((BioseqPtr) nextsep->data.ptrvalue);
        }
        else
        {
          bsp->id = SeqIdFree (bsp->id);
        }
        SeqEntryPack (nextsep);
      }
      ValNodeAddPointer (err_msg_list, 0, errormsg);
      errormsg = NULL;
    }
    else if (lastchar == '[')
    {
      nextsep = ReadOneSegSet (fp, parse_id, err_msg_list);
    }
    if (nextsep != NULL)
    {
      if (last == NULL) 
      {
        new_sep_list = nextsep;
        last = nextsep;
      }
      else 
      {
        last->next = nextsep;
        last = nextsep;
      }
    } 
    bsp = NULL;
    if (is_na)
    {
      oldsev = ErrSetMessageLevel (SEV_MAX);
      bsp = ReadDeltaFasta (fp, NULL);
      ErrSetMessageLevel (oldsev);
    }
    if (bsp == NULL)
    {
      nextsep = SequinFastaToSeqEntryEx (fp, is_na, &errormsg, FALSE, &lastchar);
      read_from_delta = FALSE;
    }
    else
    {
      nextsep = SeqMgrGetSeqEntryForData (bsp);
      lastchar = 'a';
      read_from_delta = TRUE;
    }
  }
  
  last = sep_list;
  while (last != NULL && last->next != NULL) 
  {
    last = last->next;
  }
  if (last == NULL)
  {
    sep_list = new_sep_list;
  }
  else
  {
    last->next = new_sep_list;
  }

  return sep_list;
}

static Boolean CollectIDsAndTitles (SeqEntryPtr new_list, SeqEntryPtr current_list);

static void ResetSegSetIDLists (SeqEntryPtr list)
{
  BioseqSetPtr bssp, parts;
  BioseqPtr    seg_bsp;
  SeqEntryPtr  sep;
  SeqLocPtr    loc, next_loc, last_loc;
  
  if (list == NULL)
  {
    return;
  }
  
  if (list->data.ptrvalue != NULL)
  {
    if (IS_Bioseq_set (list))
    {
      bssp = (BioseqSetPtr) list->data.ptrvalue;
      if (bssp->_class == BioseqseqSet_class_segset)
      {
        sep = bssp->seq_set;
        seg_bsp = NULL;
        parts = NULL;
        while (sep != NULL && (seg_bsp == NULL || parts == NULL))
        {
          if (IS_Bioseq (sep))
          {
            seg_bsp = sep->data.ptrvalue;
          }
          else if (IS_Bioseq_set (sep))
          {
            parts = sep->data.ptrvalue;
            if (parts != NULL && parts->_class != BioseqseqSet_class_parts)
            {
              parts = NULL;
            }
          }
          sep = sep->next;
        }
        if (seg_bsp != NULL)
        {
          /* remove old location */
          loc = (SeqLocPtr) seg_bsp->seq_ext;
          while (loc != NULL)
          {
            next_loc = loc->next;
            loc->next = NULL;
            loc = SeqLocFree (loc);
            loc = next_loc;
          }
          seg_bsp->seq_ext = NULL;
          /* put in new locations */
          sep = parts->seq_set;
          last_loc = NULL;
          while (sep != NULL)
          {
            if (IS_Bioseq (sep) && sep->data.ptrvalue != NULL)
            {
              loc = SeqLocWholeNew (sep->data.ptrvalue);
              if (loc != NULL)
              {
                if (last_loc == NULL)
                {
                  seg_bsp->seq_ext = loc;
                }
                else
                {
                  last_loc->next = loc;
                }
                last_loc = loc;
              }
            }
            sep = sep->next;
          }
        }
      }
      else
      {
        ResetSegSetIDLists (bssp->seq_set);
      }
    }
  }
  ResetSegSetIDLists (list->next);
}

static SeqEntryPtr RemoveZeroLengthSequences (SeqEntryPtr list, Int4Ptr pnum_seqs, Int4Ptr pnum_zero)
{
  SeqEntryPtr  prev_sep, next_sep, this_sep;
  Int4         num_seqs = 0, num_zero = 0;
  BioseqPtr    bsp;
  BioseqSetPtr bssp;
  
  if (list == NULL)
  {
    return NULL;
  }

  prev_sep = NULL;
  this_sep = list;
  while (this_sep != NULL)
  {
    num_seqs++;
    next_sep = this_sep->next;
    if (this_sep->data.ptrvalue == NULL)
    {
      num_zero++;
      if (prev_sep == NULL)
      {
        list = next_sep;
      }
      else
      {
        prev_sep->next = next_sep;
      }
      this_sep->next = NULL;
      SeqEntryFree (this_sep);
    }
    else if (IS_Bioseq (this_sep))
    {
      bsp = (BioseqPtr) this_sep->data.ptrvalue;
      if (bsp->length == 0)
      {
        num_zero++;
        
        if (prev_sep == NULL)
        {
          list = next_sep;
        }
        else
        {
          prev_sep->next = next_sep;
        }
        this_sep->next = NULL;
        SeqEntryFree (this_sep);
      }
      else
      {
        prev_sep = this_sep;
      }
    }
    else if (IS_Bioseq_set (this_sep))
    {
      bssp = (BioseqSetPtr) this_sep->data.ptrvalue;
      bssp->seq_set = RemoveZeroLengthSequences (bssp->seq_set, pnum_seqs, pnum_zero);
      if (bssp->seq_set == NULL)
      {
        num_zero++;
        if (prev_sep == NULL)
        {
          list = next_sep;
        }
        else
        {
          prev_sep->next = next_sep;
        }
        this_sep->next = NULL;
        SeqEntryFree (this_sep);
      }
      else
      {
        prev_sep = this_sep;
      }
    }
    else
    {
      prev_sep = this_sep;
    }
    this_sep = next_sep;
  }
  
  if (pnum_seqs != NULL)
  {
    *pnum_seqs += num_seqs;
  }
  if (pnum_zero != NULL)
  {
    *pnum_zero += num_zero;
  }
  return list;
}

static Boolean RejectZeroLengthSequences (SeqEntryPtr PNTR new_list)
{
  SeqEntryPtr next_sep;
  Int4        num_zero = 0, num_seq = 0;
  Boolean     rval = TRUE;
  Boolean     delete_all = FALSE;
  
  if (new_list == NULL)
  {
    return FALSE;
  }
  
  *new_list = RemoveZeroLengthSequences (*new_list, &num_seq, &num_zero);

  if (num_zero > 0)
  {
    ResetSegSetIDLists (*new_list);
    if (num_zero == num_seq)
    {
      Message (MSG_ERROR, "The sequences in your file are empty - you cannot import them.");
      delete_all = TRUE;
      rval = FALSE;
    }
    else if (ANS_CANCEL == Message (MSG_OKC, "%d sequences in your file are empty and cannot be imported.  "
                                    "Would you like to import the remaining sequences?", num_zero))
    {
      delete_all = TRUE;
      rval = FALSE;
    }
    if (delete_all)
    {
      
      while ((*new_list) != NULL)
      {
        next_sep = (*new_list)->next;
        (*new_list)->next = NULL;
        SeqEntryFree (*new_list);
        *new_list = next_sep;
      }
    }
  }
  return rval;
}

static Boolean RejectExtraSequences (SeqEntryPtr new_list, FastaPagePtr fpp)
{
  SeqEntryPtr sep, next_sep;
  
  if (new_list == NULL || fpp == NULL)
  {
    return FALSE;
  }
  else if (!fpp->single || new_list->next == NULL)
  {
    return TRUE;
  }

  if (fpp->is_na 
           && fpp->seqPackagePtr != NULL 
           && *(fpp->seqPackagePtr) != SEQ_PKG_GENOMICCDNA)
  {
    if (Message (MSG_YN, "You are importing multiple sequences - did you intend to create a batch submission?") == ANS_YES)
    {
      *(fpp->seqPackagePtr) = SEQ_PKG_GENBANK;
      fpp->single = FALSE;
      return TRUE;
    }
  }
  if (Message (MSG_YN, "You cannot import multiple sequences - import the first one and ignore the rest?") == ANS_YES)
  {
    sep = new_list->next;
    new_list->next = NULL;
    while (sep != NULL)
    {
      next_sep = sep->next;
      sep->next = NULL;
      sep = SeqEntryFree (sep);
      sep = next_sep;
    }
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

static Boolean ImportedSequenceTypeOk (SeqEntryPtr list, Int2 seqPackage)
{
  BioseqPtr bsp;
  MsgAnswer ans = ANS_YES;
  
  if (list == NULL || seqPackage != SEQ_PKG_SINGLE)
  {
    return TRUE;
  }
  if (list->choice == 1)
  {
    bsp = (BioseqPtr) list->data.ptrvalue;
    if (bsp != NULL && bsp->repr == Seq_repr_delta)
    {
      ans = Message (MSG_YN, "You have imported a gapped (delta) sequence.  Did you mean to do that?");
    }
  }
  else if (list->choice == 2)
  {
    ans = Message (MSG_YN, "You have imported a segmented sequence.  Did you mean to do that?");
  }
  if (ans == ANS_YES)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

static Boolean ImportFastaDialog (DialoG d, CharPtr filename)

{
  CharPtr       extension;
  FILE          *f;
  FastaPagePtr  fpp;
  ValNodePtr    head;
  Char          path [PATH_MAX];
  RecT          r;
  SeqEntryPtr   sep, new_sep_list, new_sep, test_sep;
  Boolean       rval = FALSE;

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  fpp = (FastaPagePtr) GetObjectExtra (d);
  if (fpp != NULL) {
    if (fpp->list != NULL && fpp->single)
    {
      if (!fpp->is_na
          || fpp->seqPackagePtr == NULL
          || *fpp->seqPackagePtr == SEQ_PKG_GENOMICCDNA)
      {
        Message (MSG_ERROR, "Can't import additional sequences!");
        return FALSE;
      }
      else
      {
        if (Message (MSG_YN, "You are importing multiple sequences - did you intend to create a batch submission?") == ANS_NO)
        {
          Message (MSG_ERROR, "Can't import additional sequences!");
          return FALSE;
        }
        else
        {
          *(fpp->seqPackagePtr) = SEQ_PKG_GENBANK;
          fpp->single = FALSE;
        }
      }
    }
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
      f = FileOpen (fpp->path, "r");
      if (f == NULL)
      {
        Message (MSG_ERROR, "Unable to open %s", fpp->path);
        fpp->path[0] = 0;
      }
      else
      {
        new_sep_list = ImportSequencesFromFile (f, NULL, fpp->is_na, fpp->parseSeqId, &head);
        if (fpp->seqPackagePtr != NULL 
            && *(fpp->seqPackagePtr) == SEQ_PKG_SEGMENTED
            && new_sep_list != NULL
            && IS_Bioseq (new_sep_list))
        {
          new_sep_list = SegsetFromSeqEntryList (new_sep_list);
        }
        FileClose (f);
      
        if (new_sep_list == NULL)
        {
          Message (MSG_ERROR, "Unable to read sequences from %s", fpp->path);
          fpp->path [0] = 0;
        }
        else if (! RejectZeroLengthSequences (&new_sep_list))
        {
          fpp->path [0] = 0;
        }
        else if (! RejectExtraSequences (new_sep_list, fpp))
        {
          /* if unsuccessful, delete new list */ 
          new_sep = new_sep_list;   
          while (new_sep != NULL)
          {
            test_sep = new_sep->next;
            SeqEntryFree (new_sep);
            new_sep = test_sep;
          }
          fpp->path [0] = 0;
        }
        else if (fpp->seqPackagePtr != NULL
                 && ! ImportedSequenceTypeOk (new_sep_list, *(fpp->seqPackagePtr)))
        {
          /* if unsuccessful, delete new list */ 
          new_sep = new_sep_list;   
          while (new_sep != NULL)
          {
            test_sep = new_sep->next;
            SeqEntryFree (new_sep);
            new_sep = test_sep;
          }
          fpp->path [0] = 0;
        }
        else if (CollectIDsAndTitles (new_sep_list, fpp->list))
        {
          /* add default molecule type and location */
          if (fpp->seqPackagePtr != NULL)
          {
            AddDefaultMoleculeTypeAndLocation (new_sep_list, *(fpp->seqPackagePtr));
          }
        
          /* if successful, link old and new lists */
          ValNodeLink (&(fpp->list), new_sep_list);
          rval = TRUE;
        }
        else
        {
          /* if unsuccessful, delete new list */ 
          new_sep = new_sep_list;   
          while (new_sep != NULL)
          {
            test_sep = new_sep->next;
            SeqEntryFree (new_sep);
            new_sep = test_sep;
          }
          fpp->path [0] = 0;
        }
      }
      
      if (fpp->list == NULL)
      {
        SafeHide (fpp->doc);
        Reset (fpp->doc);
        SafeShow (fpp->instructions);
        Update ();
        SetTitle (fpp->import_btn, "Import Nucleotide FASTA");
        Enable (fpp->import_btn);
        Disable (fpp->clear_btn);
      }
      else
      {        
        SafeHide (fpp->instructions);
        Update ();
        if (! fpp->is_na || fpp->single)
        {
          Disable (fpp->import_btn);
        }
        else
        {
          Enable (fpp->import_btn);
          SetTitle (fpp->import_btn, "Import Additional Nucleotide FASTA");
        }
        Enable (fpp->clear_btn);
        FormatFastaDoc (fpp);
        SafeShow (fpp->doc);
      }
      ArrowCursor ();
      Update ();
      return rval;
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
sequence in the spaces above.  Then click on either \
'Add/Modify Sequences' to create your sequences with the editor or \
'Import Nucleotide FASTA' to read a previously generated \
FASTA file that contains the sequence (which can be in segments).\
  The FASTA definition lines may be of the following form:\n\n\
>ID [organism=scientific name] [strain=name] [clone=name] title\n\n\
where the [ and ] brackets are actually in the text.";

static CharPtr  fastaGenMsg = "\
\nPlease enter information about the genomic \
sequence in the spaces above.  Then click on either \
'Add/Modify Sequences' to create your sequences with the editor or \
'Import Genomic FASTA' to read a previously generated FASTA file that \
contains the sequence (which can be in segments).  The \
FASTA definition lines may be of the following form:\n\n\
>ID [organism=scientific name] [strain=name] [clone=name] title\n\n\
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
>ID [gene=symbol] [protein=name] title\n\n\
where the [ and ] brackets are actually in the text.";

extern DialoG CreateFastaDialog (GrouP h, CharPtr title,
                                 Boolean is_na, Boolean is_mrna, CharPtr text,
                                 Boolean parseSeqId, Boolean single, Int2Ptr seqPackagePtr)

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

    fpp->seqPackagePtr = seqPackagePtr;
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
  /* new for alignment */
  TexT  missing;
  TexT  beginning_gap;
  TexT  middle_gap;
  TexT  end_gap;
  TexT  match;
  Int4  type;
} PhylipPage, PNTR PhylipPagePtr;


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
                           (IsPopPhyEtcSet (bssp->_class)))) {
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
                           (IsPopPhyEtcSet (bssp->_class)))) {
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

static CharPtr noOrgInTitleWarning =
"sequences have organism information in titles. " \
"It is critical to annotate the data file with organism and source information. " \
"Please quit Sequin and read the Sequin Quick Guide section on preparing the data files before proceeding.";

static CharPtr noSrcInTitleWarning =
"sequences have source information in titles. " \
"It is critical to annotate the data file with organism and source information. " \
"Please quit Sequin and read the Sequin Quick Guide section on preparing the data files before proceeding.";

static void CountTitlesWithoutOrganisms (SeqEntryPtr sep)
{
  BioseqSetPtr bssp;
  SeqEntryPtr  tmp;
  Int2         seqtitles = 0;
  Int2         seqtotals = 0;
  CharPtr      ttl;
  Char         str [256];

  /* count titles without organisms */
  if (sep != NULL && IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 || (IsPopPhyEtcSet (bssp->_class)))) {
      for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
        ttl = NULL;
        SeqEntryExplore (tmp, (Pointer) (&ttl), FindFirstTitle);
        if (ttl != NULL) {
          if (bssp->_class == BioseqseqSet_class_phy_set) {
            if (StringISearch (ttl, "[org=") != NULL || StringISearch (ttl, "[organism=") != NULL) {
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
}

static CharPtr  phylipNucMsg = "\
\nPlease enter information about the nucleotide \
sequence in the spaces above.  Then click on \
'Import Nucleotide Alignment' to read a file that \
contains the sequences.\n\n\
Beginning Gap: When some of the sequences in an \
alignment are shorter or longer than others, beginning \
gap characters are added to the beginning of the sequence \
to maintain the correct spacing.  These will not appear \
in your sequence file.\n\
Middle Gap: These characters are used to maintain the spacing \
inside an alignment.  These are not nucleotides and will \
not appear as part of your sequence file.\n\
End Gap: When some of the sequences in an alignment are shorter \
or longer than others, end gap characters are added to the end \
of the sequence to maintain the correct spacing.  These will \
not appear in your sequence file.\n\
Ambiguous/Unknown: These characters are used to represent \
indeterminate/ambiguous nucleotides.  These will appear in your \
sequence file as 'n'.\n\
Match: These characters are used to indicate positions where \
sequences are identical to the first sequence.  These will be \
replaced by the actual characters from the first sequence.";

static void SetPhylipDocInstructions (PhylipPagePtr ppp)
{
  if (ppp == NULL || ppp->doc == NULL) return;
  Reset (ppp->doc);
  AppendText (ppp->doc, phylipNucMsg, &faParFmt, &faColFmt, programFont);
  UpdateDocument (ppp->doc, 0, 0);
  Update ();
}

static Boolean ImportPhylipDialog (DialoG d, CharPtr filename)
{
  Char           path [PATH_MAX];
  PhylipPagePtr  ppp;
  SeqEntryPtr    sep;
  RecT           r;
  FILE           *fp;
  ObjMgrDataPtr  omdptop;
  ObjMgrData     omdata;
  Uint2          parenttype;
  Pointer        parentptr;
  Char           errStr [PATH_MAX + 64];
  Char           missing [15];
  Char           beginning_gap [15];
  Char           middle_gap [15];
  Char           end_gap [15];
  Char           match [15];
  CharPtr        no_org_err_msg = NULL;

  if (d == NULL || filename == NULL) return FALSE;

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  ppp = (PhylipPagePtr) GetObjectExtra (d);
  if (ppp == NULL) {
    return FALSE;
  }

  if (path [0] != '\0' || GetInputFileName (path, sizeof (path), ppp->extension, "TEXT")) {
    WatchCursor ();
    StringCpy (ppp->path, path);
    ObjectRect (ppp->doc, &r);
    InsetRect (&r, 4, 4);
    faColFmt.pixWidth = r.right - r.left;
    Reset (ppp->doc);
    Update ();
    ppp->sep = SeqEntryFree (ppp->sep);
    fp = FileOpen (path, "r");
    if (fp != NULL) {
      GetTitle (ppp->missing, missing, sizeof (missing) -1);
      GetTitle (ppp->beginning_gap, beginning_gap, sizeof (beginning_gap) - 1);
      GetTitle (ppp->middle_gap, middle_gap, sizeof (middle_gap) - 1);
      GetTitle (ppp->end_gap, end_gap, sizeof (end_gap) - 1);
      GetTitle (ppp->match, match, sizeof (match) - 1);
      ppp->sep = SeqEntryFromAlignmentFile (fp, missing, match, 
                                            beginning_gap, middle_gap, end_gap,
                                            "ABCDGHKMRSTUVWXYabcdghkmrstuvwxy",
                                            Seq_mol_na, no_org_err_msg);
      sep = ppp->sep;
      if (sep != NULL) {
        SaveSeqEntryObjMgrData (ppp->sep, &omdptop, &omdata);
        GetSeqEntryParent (ppp->sep, &parentptr, &parenttype);
        SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
        RestoreSeqEntryObjMgrData (sep, omdptop, &omdata);

        FormatPhylipDoc (ppp);
        SafeShow (ppp->doc);

        CountTitlesWithoutOrganisms (sep);
      } else {
        SendHelpScrollMessage (helpForm, "Organism and Sequences Form", "Nucleotide Page");
        SetPhylipDocInstructions (ppp);
      }
    } else {
      SetPhylipDocInstructions (ppp);
    }
  } else {
	sprintf (errStr, "ERROR: Unable to open file %s\n\n", path);
	AppendText (ppp->doc, errStr, &faParFmt, &faColFmt, programFont);
	AppendText (ppp->doc, strerror(errno), &faParFmt, &faColFmt, programFont);
	SafeShow (ppp->doc);
    Update ();
  }
  ArrowCursor ();
  Update ();
  return TRUE;
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


static DialoG CreatePhylipDialog (GrouP h, CharPtr title, CharPtr text,
                                  Uint1 format, CharPtr extension,
                                  Int4 type)

{
  PhylipPagePtr  ppp;
  GrouP          g;
  GrouP          m;
  GrouP          p;
  GrouP          s;
  GrouP          a;
  RecT          r;

  p = HiddenGroup (h, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  ppp = (PhylipPagePtr) MemNew (sizeof (PhylipPage));
  if (ppp != NULL) {

    SetObjectExtra (p, ppp, CleanupPhylipDialog);
    ppp->dialog = (DialoG) p;
    ppp->todialog = NULL;
    ppp->fromdialog = NULL;
    ppp->importdialog = ImportPhylipDialog;
    ppp->type = type;

    if (title != NULL && title [0] != '\0') {
      s = NormalGroup (p, 0, -2, title, systemFont, NULL);
    } else {
      s = HiddenGroup (p, 0, -2, NULL);
    }
    m = HiddenGroup (s, -1, 0, NULL);

    ppp->format = format;
    ppp->path [0] = '\0';
    StringNCpy_0 (ppp->extension, extension, sizeof (ppp->extension));

    /* new for alignment */
    a = NormalGroup (m, 4, 0, "Sequence Characters", systemFont, NULL);
    StaticPrompt (a, "Beginning Gap", 0, dialogTextHeight, systemFont, 'c');
    ppp->beginning_gap = DialogText (a, "-.Nn?", 5, NULL);
    StaticPrompt (a, "Ambiguous/Unknown", 0, dialogTextHeight, systemFont, 'c');
    ppp->missing = DialogText (a, "?Nn", 5, NULL);
    StaticPrompt (a, "Middle Gap", 0, dialogTextHeight, systemFont, 'c');
    ppp->middle_gap = DialogText (a, "-.", 5, NULL);
    StaticPrompt (a, "Match", 0, dialogTextHeight, systemFont, 'c');
    ppp->match = DialogText (a, ":", 5, NULL);
    StaticPrompt (a, "End Gap", 0, dialogTextHeight, systemFont, 'c');
    ppp->end_gap = DialogText (a, "-.Nn?", 5, NULL);
  
    g = HiddenGroup (m, 0, 0, NULL);
    ppp->doc = DocumentPanel (g, stdCharWidth * 27, stdLineHeight * 8);
    ObjectRect (ppp->doc, &r);
    InsetRect (&r, 4, 4);
    faColFmt.pixWidth = r.right - r.left;
    SetPhylipDocInstructions (ppp);
  }

  return (DialoG) p;
}

#define NUCLEOTIDE_PAGE   0
#define ORGANISM_PAGE     1
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
  {"Other-Genetic",            9},
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

/*---------------------------------------------------------------------*/
/*                                                                     */
/* HasZeroLengthSequence () -- Checks to see if any of a submission's  */
/*                             sequences are missing (ie -- zero       */
/*                             length).                                */
/*                                                                     */
/*---------------------------------------------------------------------*/

extern Boolean HasZeroLengthSequence (ForM newForm)
{
  SequencesFormPtr  sqfp;
  FastaPagePtr      fpp;
  SeqEntryPtr       sep;
  BioseqPtr         bsp;

  /* Get the list of Bioseqs to check */

  sqfp = (SequencesFormPtr) GetObjectExtra (newForm);
  if (NULL == sqfp)
    return TRUE;

  fpp = GetObjectExtra (sqfp->dnaseq);
  sep = fpp->list;

  /* Check the list */

  while (NULL != sep) {
    if (sep->choice == 1) { 
      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (bsp->length <= 0)
	return TRUE;
    }
    sep = sep->next;
  }

  /* If we made it to here, then */
  /* there were none found.      */

  return FALSE;
}

extern Boolean SequencesFormHasProteins (ForM f)

{
  FastaPagePtr      fpp;
  SequencesFormPtr  sqfp;

  sqfp = (SequencesFormPtr) GetObjectExtra (f);
  if (sqfp != NULL) {
    if (PackageTypeIsSet (sqfp->seqPackage)) return TRUE;
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
  if (sqfp != NULL && PackageTypeIsSingle (sqfp->seqPackage))
  {
    fpp = GetObjectExtra (sqfp->dnaseq);
    if (fpp != NULL) {
      if (fpp->list != NULL && fpp->list->next != NULL) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

extern DialoG CreateTagListDialogEx (GrouP h, Uint2 rows, Uint2 cols,
                                     Int2 spacing, Uint2Ptr types,
                                     Uint2Ptr textWidths, EnumFieldAssocPtr PNTR alists,
                                     Boolean useBar, Boolean noExtend,
                                     ToDialogFunc tofunc, FromDialogFunc fromfunc);

static CharPtr other_modifier_names [] = 
{
  "cDNA",
  "comment",
  "gene",
  "gene_syn",
  "molecule",
  "moltype",
  "mRNA",
  "note",
  "orf",
  "origin",
  "prot_desc",
  "protein_desc",
  "subsource"
};

static Int4 num_other_modifier_names = sizeof (other_modifier_names) / sizeof (CharPtr);

typedef enum {
    eModifierType_SourceQual = 0,
    eModifierType_Organism,
    eModifierType_Location,
    eModifierType_Lineage,
    eModifierType_GeneticCode,
    eModifierType_GeneticCodeComment,
    eModifierType_NucGeneticCode,
    eModifierType_MitoGeneticCode,
    eModifierType_MolType,
    eModifierType_Topology,
    eModifierType_Other
} EModifierType;

typedef struct modifierinfo 
{
  CharPtr       name;
  Uint1         subtype;
  CharPtr       value;
  EModifierType modtype;
} ModifierInfoData, PNTR ModifierInfoPtr;

static ModifierInfoPtr ModifierInfoNew (void)
{
  ModifierInfoPtr mip;
  mip = (ModifierInfoPtr) MemNew (sizeof (ModifierInfoData));
  if (mip == NULL) return NULL;
  mip->name = NULL;
  mip->value = NULL;
  mip->modtype = eModifierType_SourceQual;
  return mip;
}

static ModifierInfoPtr ModifierInfoFree (ModifierInfoPtr mip)
{
  if (mip == NULL) return NULL;
  mip->name = MemFree (mip->name);
  mip->value = MemFree (mip->value);
  mip = MemFree (mip);
  return mip;
}

static ValNodePtr ModifierInfoListFree (ValNodePtr list)
{
  if (list == NULL) return NULL;
  ModifierInfoListFree (list->next);
  list->next = NULL;
  list->data.ptrvalue = ModifierInfoFree (list->data.ptrvalue);
  ValNodeFree (list);
  return NULL;
}

static EModifierType GetModifierType (CharPtr mod_name)
{
  Int4 i;
  
  if (StringHasNoText (mod_name))
  {
    return eModifierType_SourceQual;
  }
  else if (StringICmp (mod_name, "organism") == 0
           || StringICmp (mod_name, "org") == 0)
  {
	  return eModifierType_Organism;
  }
  else if (StringICmp (mod_name, "location") == 0)
  {
    return eModifierType_Location;
  }
  else if (StringICmp (mod_name, "lineage") == 0)
  {
    return eModifierType_Lineage;
  }
  else if (StringICmp (mod_name, "gcode") == 0)
  {
    return eModifierType_NucGeneticCode;
  }
  else if (StringICmp (mod_name, "mgcode") == 0)
  {
    return eModifierType_MitoGeneticCode;
  }
  else if (StringICmp (mod_name, "genetic_code") == 0)
  {
    return eModifierType_GeneticCode;
  }
  else if (StringICmp (mod_name, "genetic_code_comment") == 0)
  {
    return eModifierType_GeneticCodeComment;
  }
  else if (StringICmp (mod_name, "moltype") == 0)
  {
    return eModifierType_MolType;
  }
  else if (StringICmp (mod_name, "topology") == 0)
  {
    return eModifierType_Topology;
  }
  else
  {
    for (i = 0; i < num_other_modifier_names; i++)
    {
      if (StringICmp (mod_name, other_modifier_names[i]) == 0)
      {
        return eModifierType_Other;
      }
    }
    return eModifierType_SourceQual;   
  }
}

static ModifierInfoPtr ParseOneBracketedModifier (CharPtr str)
{
  CharPtr         start, stop, eq_loc;
  ModifierInfoPtr mip;
  Int4            value_len, name_len;
  
  start = StringChr (str, '[');
  stop = StringChr (str, ']');
  if (start == NULL)
  {
    eq_loc = NULL;
  }
  else
  {
    eq_loc = StringChr (start + 1, '=');
  }
  
  if (start == NULL || stop == NULL || eq_loc == NULL) return NULL;
  
  mip = ModifierInfoNew();
  if (mip == NULL) return NULL;
  name_len = eq_loc - start + 1;
  value_len = stop - eq_loc + 1;
  mip->value = (CharPtr) MemNew (value_len * sizeof (Char));
  mip->name = (CharPtr) MemNew (name_len * sizeof (Char));
  if (mip->value == NULL || mip->name == NULL)
  {
  	ModifierInfoFree (mip);
  	return NULL;
  }
  StringNCpy (mip->value, eq_loc + 1, value_len - 2);
  mip->value [value_len - 1] = 0;
  StringNCpy (mip->name, start + 1, name_len - 2);
  mip->name [name_len - 1] = 0;
  
  if (StringICmp (mip->name, "note") == 0)
  {
    mip->name = MemFree (mip->name);
    mip->name = StringSave ("Note-SubSrc");
  }
  
  mip->modtype = GetModifierType (mip->name);
  if (mip->modtype == eModifierType_SourceQual)
  {
    mip->subtype = FindTypeForModNameText (mip->name);
  }
  else
  {
    mip->subtype = 0;
  }
  return mip;
}

static ValNodePtr ParseAllBracketedModifiers (CharPtr str)
{
  CharPtr         stop, cp;
  ValNodePtr      list = NULL;
  ValNodePtr      vnp;
  ModifierInfoPtr mip;
  
  cp = str;
  stop = StringChr (cp, ']');
  while (stop != NULL)
  {
  	mip = ParseOneBracketedModifier (cp);
  	if (mip == NULL)
  	{
  	  stop = NULL;
  	}
  	else
  	{
  	  vnp = ValNodeAdd (&list);
  	  if (vnp != NULL)
  	  {
  	  	vnp->data.ptrvalue = mip;
  	  }
  	  cp = stop + 1;
  	  stop = StringChr (cp, ']');
  	}
  }
  return list;
}

static CharPtr GetUnbracketedText (CharPtr str)
{
  CharPtr unbr_text, src, dst;
  
  if (StringHasNoText (str))
  {
  	return NULL;
  }
  unbr_text = StringSave (str);
  dst = StringChr (unbr_text, '[');
  src = StringChr (unbr_text, ']');
  while (dst != NULL && src != NULL && *src != 0)
  {
    src++;
    while (*src != '[' && *src != 0)
    {
      *dst = *src;
      dst++;
      src++;
    }
  	src = StringChr (src, ']');
  }
  if (dst != NULL)
  {
    *dst = 0;    
  }
  return unbr_text;
}

static ValNodePtr BuildModifierTypeList (ValNodePtr type_list, CharPtr new_title)
{
  ValNodePtr      modifier_info_list;
  ValNodePtr      info_vnp, type_vnp;
  ModifierInfoPtr mip;
  
  modifier_info_list = ParseAllBracketedModifiers (new_title);
  for (info_vnp = modifier_info_list; info_vnp != NULL; info_vnp = info_vnp->next)
  {
    mip = (ModifierInfoPtr)info_vnp->data.ptrvalue;
    if (mip == NULL) continue;
    if (mip->modtype == eModifierType_Organism) continue;
    if (mip->modtype == eModifierType_SourceQual)
    {
  	  for (type_vnp = type_list;
  	       type_vnp != NULL 
  	         && (type_vnp->choice != mip->subtype 
  	             || StringICmp (type_vnp->data.ptrvalue, mip->name) != 0); 
  	       type_vnp = type_vnp->next)
  	  {
  	  }
    }
    else
    {
  	  for (type_vnp = type_list;
  	       type_vnp != NULL && StringICmp (type_vnp->data.ptrvalue, mip->name) != 0;
  	       type_vnp = type_vnp->next)
  	  {
  	  }
    }
  	if (type_vnp == NULL)
  	{
  	  type_vnp = ValNodeNew (type_list);
  	  if (type_list == NULL) type_list = type_vnp;
  	  if (type_vnp != NULL)
  	  {
  	  	type_vnp->choice = mip->subtype;
  	  	type_vnp->data.ptrvalue = StringSave (mip->name);
  	  }
  	}
  }
  ModifierInfoListFree (modifier_info_list);
  return type_list;
}


static Uint2 modedit_types [] = {
  TAGLIST_PROMPT, TAGLIST_TEXT
};

static Uint2 modedit_widths [] = {
  0, 0,
};

static Uint2 nontextmodedittypes [] = 
{
  TAGLIST_PROMPT, TAGLIST_POPUP
};

ENUM_ALIST(nontextmodedit_alist)
  {"FALSE",             0},
  {"TRUE",              1},
END_ENUM_ALIST

EnumFieldAssocPtr nontextmodedit_alists [] = {
  NULL, nontextmodedit_alist
};

static Uint2 locationmodedittypes [] = 
{
  TAGLIST_PROMPT, TAGLIST_POPUP
};

EnumFieldAssocPtr locationmodedit_alists [] = {
  NULL, biosource_genome_simple_alist
};

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

      ptr = SaveStringFromText (fpfp->title);
      StringCat (title, ptr);
      MemFree (ptr);

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

      if (pbsp->descr != NULL) {
        vnp = ValNodeExtract (&(pbsp->descr), Seq_descr_title);
        vnp = ValNodeFreeData (vnp);
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
    acceptBtn = DefaultButton (c, "OK", AcceptThisFixup);
    SetObjectExtra (acceptBtn, fpfp, NULL);

    AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, (HANDLE) fpfp->orf, NULL);

    RealizeWindow (w);

    str [0] = '\0';
    sip = SeqIdFindWorst (pbsp->id);
    if (sip != NULL) {
      if (sip->choice == SEQID_LOCAL) {
        SeqIdWrite (sip, str, PRINTID_REPORT, sizeof (str));
      } else {
        SeqIdWrite (sip, str, PRINTID_FASTA_SHORT, sizeof (str));
      }
    }
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
    if (PackageTypeIsSingle (sqfp->seqPackage)
        || sqfp->seqPackage == SEQ_PKG_GENOMICCDNA)
    {
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
      } else if (sqfp->seqFormat == SEQ_FMT_ALIGNMENT) {
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
      sqfp->topSeqForConfirm = sqfp->currConfirmSeq;
      sqfp->currConfirmSeq = NULL;
      fpp = (FastaPagePtr) GetObjectExtra (sqfp->protseq);
      if (fpp != NULL) {
        sqfp->currConfirmSeq = fpp->list;
      }
      sqfp->currConfirmCount = 0;
      LetUserFixProteinInfo (sqfp);

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

static Int4 GeneticCodeFromStringAndList (CharPtr str, ValNodePtr list)
{
  while (list != NULL)
  {
    if (StringICmp (str, list->data.ptrvalue) == 0)
    {
      return list->choice;
    }
    list = list->next;
  }
  return 0;
}

static Int4 GeneticCodeFromString (CharPtr str)
{
  ValNodePtr gencodelist;
  Int4       gcode = 0;
  
  if (StringHasNoText (str))
  {
    gcode = 0;
  }
  else if (isdigit (str[0]))
  {
    gcode = atoi (str);
  }
  else
  {
    gencodelist = GetGeneticCodeValNodeList ();
    gcode = GeneticCodeFromStringAndList (str, gencodelist);
    gencodelist = ValNodeFreeData (gencodelist);
  }
  return gcode;
}

static CharPtr GeneticCodeStringFromIntAndList (Int4 num, ValNodePtr list)
{
  while (list != NULL)
  {
    if (list->choice == num)
    {
      return list->data.ptrvalue;
    }
    list = list->next;
  }
  return NULL;
}

static Int4 MolTypeFromString (CharPtr str)
{
  EnumFieldAssocPtr  eap;

  if (StringICmp (str, "dna") == 0)
  {
    return 253;
  }
  else if (StringICmp (str, "rna") == 0)
  {
    return 254;
  }
  for (eap = biomol_nucGen_alist; eap != NULL && eap->name != NULL; eap++)
  {
    if (StringICmp (eap->name, str) == 0)
    {
      return eap->value;
    }
  }
  for (eap = biomol_nucX_alist; eap != NULL && eap->name != NULL; eap++)
  {
    if (StringICmp (eap->name, str) == 0)
    {
      return eap->value;
    }
    else if (eap->name [0] == 'm'
             && StringICmp (eap->name, "mRNA [cDNA]") == 0
             && StringICmp (str, "mRNA") == 0)
    {
      return eap->value;
    }
  }
  return 0;
}

static Int4 TopologyFromString (CharPtr str)
{
  EnumFieldAssocPtr  eap;

  for (eap = topology_nuc_alist; eap != NULL && eap->name != NULL; eap++)
  {
    if (StringICmp (eap->name, str) == 0)
    {
      return eap->value;
    }
  }
  return 1; 
}

static void SetGeneticCodeForBioSource (BioSourcePtr biop, Int4 gcode, Boolean is_nuc)
{
  OrgRefPtr  orp;
  OrgNamePtr onp;

  if (biop == NULL || gcode < 0)
  {
    return;
  }
  
  orp = biop->org;
  if (biop->org == NULL)
  {
    biop->org = OrgRefNew ();
    orp = biop->org;
  }
  if (orp != NULL) {
    onp = orp->orgname;
    if (onp == NULL) {
      onp = OrgNameNew ();
      orp->orgname = onp;
    }
    if (onp != NULL) {
      if (is_nuc)
      {
        onp->gcode = gcode;
      }
      else
      {
        onp->mgcode = gcode;
      }
    }
  }
}

static void 
SetGeneticCodeFromTitle 
(BioSourcePtr biop,
 CharPtr title, 
 CharPtr look_for, 
 Boolean is_nuc)
{
  CharPtr    ptr;
  CharPtr    val;
  Int4       gcode;
  
  if (biop == NULL || StringHasNoText (title))
  {
    return;
  }
  
  ptr = StringISearch (title, look_for);
  if (ptr != NULL)
  {
    ptr += StringLen (look_for);
  }
  if (ptr != NULL) {
    val = StringSave (ptr);
    if (val != NULL) 
    {
      ptr = StringChr (val, ']');
      if (ptr != NULL) {
        *ptr = '\0';
      
        gcode = GeneticCodeFromString (val);
      
        SetGeneticCodeForBioSource (biop, gcode, is_nuc);     
      }
      MemFree (val);
    }
  }
}

#define USE_NUCLEAR_GENETIC_CODE       1
#define USE_MITOCHONDRIAL_GENETIC_CODE 2
#define USE_OTHER_GENETIC_CODE         3

static Int4 UseGeneticCodeForLocation (CharPtr location)
{
  if (StringHasNoText (location))
  {
    return USE_NUCLEAR_GENETIC_CODE;
  }
  else if (StringICmp (location, "Mitochondrion") == 0
           || StringICmp (location, "Kinetoplast") == 0)
  {
    return USE_MITOCHONDRIAL_GENETIC_CODE;
  }
  else if (StringICmp (location, "Chloroplast") == 0
           || StringICmp (location, "Chromoplast") == 0
           || StringICmp (location, "plastid") == 0
           || StringICmp (location, "cyanelle") == 0
           || StringICmp (location, "apicoplast") == 0
           || StringICmp (location, "leucoplast") == 0
           || StringICmp (location, "proplastid") == 0)
  {
    return USE_OTHER_GENETIC_CODE;
  }
  else
  {
    return USE_NUCLEAR_GENETIC_CODE;
  }
}

static void SetAllGeneticCodesFromTitle (BioSourcePtr biop, CharPtr title)
{
  CharPtr ptr1, ptr2;
  Int4    code_to_use;
  
  if (biop == NULL || StringHasNoText (title))
  {
    return;
  }
  
  ptr1 = StringISearch (title, "[location=");
  if (ptr1 != NULL) 
  {
    ptr1 += 10;
    ptr2 = StringChr (ptr1, ']');
    if (ptr2 != NULL)
    {
      *ptr2 = 0;
      
      code_to_use = UseGeneticCodeForLocation (ptr1);
      if (code_to_use == USE_OTHER_GENETIC_CODE)
      {
        SetGeneticCodeForBioSource (biop, 11, TRUE);     
      }
      else if (code_to_use == USE_NUCLEAR_GENETIC_CODE)
      {
        SetGeneticCodeFromTitle (biop, title, "[genetic_code=", TRUE);
      }
      else if (code_to_use == USE_MITOCHONDRIAL_GENETIC_CODE)
      {
        SetGeneticCodeFromTitle (biop, title, "[genetic_code=", FALSE);
      }
      *ptr2 = ']';
    }
  }
  
  SetGeneticCodeFromTitle (biop, title, "[gcode=", TRUE);
  SetGeneticCodeFromTitle (biop, title, "[mgcode=", FALSE);
  ExciseString (title, "[genetic_code=", "]");
  ExciseString (title, "[gcode=", "]");
  ExciseString (title, "[mgcode=", "]");
}

static void 
SetMoleculeAndMolTypeFromTitle 
(SeqEntryPtr sep,
 BioseqPtr   bsp, 
 CharPtr     title,
 Int2        seqPackage)
{
  ValNodePtr vnp;
  MolInfoPtr mip = NULL;
  Uint1      biomol;
  Int4       molecule;
  CharPtr    ptr;
  CharPtr    str = NULL;
  Int4       str_size;
  
  if (sep == NULL || bsp == NULL)
  {
    return;
  }

  /* prepare string buffer for values */
  str_size = StringLen (title);
  str = (CharPtr) MemNew ((str_size + 1) * sizeof (Char));
  if (str == NULL)
  {
    return;
  }      
  
  vnp = SeqEntryGetSeqDescr (sep, Seq_descr_molinfo, NULL);
  if (vnp == NULL)
  {
    if (seqPackage == SEQ_PKG_SINGLE)
    {
      biomol = 3;
      molecule = Seq_mol_rna;
    }
    else 
    {
      biomol = 1;
      molecule = Seq_mol_dna;
    }
  }
  else
  {
    mip = (MolInfoPtr) vnp->data.ptrvalue;
    biomol = mip->biomol;
    molecule = bsp->mol;
  }
  
  /* get moltype from defline */
  ptr = StringISearch (title, "[moltype=");
  if (ptr != NULL) {
    StringNCpy_0 (str, ptr + 9, str_size);
    ptr = StringChr (str, ']');
    if (ptr != NULL) {
      *ptr = '\0';
      biomol = MolTypeFromString (str);
      if (biomol == 1)
      {
        molecule = Seq_mol_na;
      }
      else if (biomol >= 2 && biomol <= 7)
      {
        molecule = Seq_mol_rna;
      }
      else if (biomol == 9)
      {
        molecule = Seq_mol_dna;
      }
      else if (biomol == 253)
      {
        molecule = Seq_mol_dna;
        biomol = 1;
      }
      else if (biomol == 254)
      {
        molecule = Seq_mol_rna;
        biomol = 1;
      }
      else if (biomol == 255)
      {
        molecule = Seq_mol_other;
      }
    }
  }

  /* get molecule from defline */      
  ptr = StringISearch (title, "[molecule=");
  if (ptr != NULL) {
    StringNCpy_0 (str, ptr + 10, str_size);
    ptr = StringChr (str, ']');
    if (ptr != NULL) {
      *ptr = '\0';
      if (StringICmp (str, "dna") == 0) {
        molecule = Seq_mol_dna;
      } else if (StringICmp (str, "rna") == 0) {
        molecule = Seq_mol_rna;
      }
    }
  }
  
  ptr = StringISearch (title, "[dna]");
  if (ptr != NULL)
  {
    molecule = Seq_mol_dna;
    ExciseString (title, "[dna", "]");
  }
  
  ptr = StringISearch (title, "[rna]");
  if (ptr != NULL)
  {
    molecule = Seq_mol_rna;
    ExciseString (title, "[rna", "]");
  }
  
  if (mip == NULL)
  {
    vnp = CreateNewDescriptor (sep, Seq_descr_molinfo);
    mip = MolInfoNew ();
    vnp->data.ptrvalue = mip;
  }
     
  mip->biomol = biomol;
  bsp->mol = molecule;  
  
  ExciseString (title, "[moltype", "]");
  ExciseString (title, "[molecule", "]");
  
  str = MemFree (str);
}

static void AddGeneticCodeComment (BioseqPtr bsp, CharPtr comment)
{
  SeqDescPtr         sdp;
  UserObjectPtr      uop = NULL;
  ObjectIdPtr        oip;
  UserFieldPtr       ufp, last_ufp = NULL;
  CharPtr            comment_fmt = "Submitter genetic code: %s";
  CharPtr            new_comment;
  Int4               new_comment_len;

  if (bsp == NULL || StringHasNoText (comment))
  {
    return;
  }
  
  sdp = bsp->descr;
  while (sdp != NULL && uop == NULL)
  {
    if (sdp->choice == Seq_descr_user && sdp->data.ptrvalue != NULL)
    {
      uop = (UserObjectPtr) sdp->data.ptrvalue;
      oip = uop->type;
      if (oip == NULL || StringCmp (oip->str, "Submission") != 0)
      {
        uop = NULL;
      }
    }
    sdp = sdp->next;
  }
  
  
  if (uop == NULL)
  {
    uop = UserObjectNew ();
    if (uop == NULL)
    {
      return;
    }
    uop->type = ObjectIdNew ();
    uop->type->str = StringSave ("Submission");
    ValNodeAddPointer (&bsp->descr, Seq_descr_user, uop);  
  }
  
  ufp = uop->data;
  while (ufp != NULL 
         && (ufp->label == NULL 
           || StringCmp (ufp->label->str, "AdditionalComment") != 0))
  {
    last_ufp = ufp;
    ufp = ufp->next;
  }
  
  if (ufp == NULL)
  {
    ufp = UserFieldNew ();
    ufp->label = ObjectIdNew ();
    ufp->label->str = StringSave ("AdditionalComment");
    if (last_ufp == NULL)
    {
      uop->data = ufp;
    }
    else
    {
      last_ufp->next = ufp;
    }
  }
  
  new_comment_len = StringLen (comment) + StringLen (comment_fmt);
  if (!StringHasNoText (ufp->data.ptrvalue))
  {
    new_comment_len += StringLen (ufp->data.ptrvalue);
  }
  new_comment = (CharPtr) MemNew (new_comment_len * sizeof (Char));
  sprintf (new_comment, comment_fmt, comment);
  
  if (!StringHasNoText (ufp->data.ptrvalue))
  {
    StringCat (new_comment, ufp->data.ptrvalue);
  }
  
  ufp->data.ptrvalue = MemFree (ufp->data.ptrvalue);
  ufp->data.ptrvalue = new_comment;
}

extern Boolean ProcessOneNucleotideTitle (Int2 seqPackage,
                                          SeqEntryPtr nsep, SeqEntryPtr top);
extern Boolean ProcessOneNucleotideTitle (Int2 seqPackage, 
                                          SeqEntryPtr nsep, SeqEntryPtr top)

{
  EnumFieldAssocPtr  ap;
  BioSourcePtr       biop;
  BioseqSetPtr       bssp;
  CharPtr            lin;
  OrgNamePtr         masteronp = NULL;
  OrgRefPtr          masterorp = NULL;
  BioseqPtr          nbsp;
  Boolean            needbiop;
  OrgNamePtr         onp;
  OrgRefPtr          orp;
  CharPtr            ptr;
  SeqEntryPtr        sep;
  CharPtr            str;
  CharPtr            title;
  ValNodePtr         vnp;
  Int4               topology;

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
      
      SetMoleculeAndMolTypeFromTitle (nsep, nbsp, title, seqPackage);

      if (nbsp->topology == 0)
      {
        topology = TOPOLOGY_LINEAR;
      }
      else
      {
        topology = nbsp->topology;
      }
  
      /* get topology from defline */
      ptr = StringISearch (title, "[topology=");
      if (ptr != NULL) {
        StringNCpy_0 (str, ptr + 10, PROC_NUC_STR_SIZE);
        ptr = StringChr (str, ']');
        if (ptr != NULL) {
          *ptr = '\0';
          topology = TopologyFromString (str);
        }
      }
      nbsp->topology = topology;
      
      /* add bankit comment for genetic code */
      ptr = StringISearch (title, "[genetic_code_comment=");
      if (ptr != NULL) {
        StringNCpy_0 (str, ptr + 22, PROC_NUC_STR_SIZE);
        ptr = StringChr (str, ']');
        if (ptr != NULL) {
          *ptr = '\0';
          AddGeneticCodeComment (nbsp, str);
        }
      }

      needbiop = FALSE;
      
      if (PackageTypeIsSet (seqPackage)
          || seqPackage == SEQ_PKG_GENBANK)
      {
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
          biop = BioSourceNew ();
          if (biop != NULL) {
            orp = OrgRefNew ();
            biop->org = orp;
            if (orp != NULL) {
              TrimSpacesAroundString (str);
              SetTaxNameAndRemoveTaxRef (orp, StringSave (str));
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
        
        SetAllGeneticCodesFromTitle (biop, title);
        
        if (seqPackage == SEQ_PKG_PHYLOGENETIC) {
          orp = biop->org;
          if (orp != NULL) {
            onp = orp->orgname;
            if (onp == NULL) {
              onp = OrgNameNew ();
              orp->orgname = onp;
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

        ptr = StringISearch (title, "[location=");
        if (ptr == NULL)
        {
          /* if location is not specified, make it genomic */
          biop->genome = 1;
        }
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
        
      }
      
      ExciseString (title, "[org=", "]");
      ExciseString (title, "[organism=", "]");
      ExciseString (title, "[lineage=", "]");
      ExciseString (title, "[molecule=", "]");
      ExciseString (title, "[moltype=", "]");
      ExciseString (title, "[location=", "]");
      ExciseString (title, "[topology=", "]");
      ExciseString (title, "[genetic_code_comment=", "]");
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
    if (PackageTypeIsSet (seqPackage)
        || seqPackage == SEQ_PKG_GENOMICCDNA)
    {
      needbiop = TRUE;
      if (GetAppParam ("SEQUIN", "PREFERENCES", "BIOSRCONALL", NULL, str, PROC_NUC_STR_SIZE)) {
        if (StringICmp (str, "FALSE") == 0) {
          needbiop = FALSE;
        }
      }
    }
  }
  MemFree (str);
  
  return TRUE;
}

static Boolean AutomaticNucleotideProcess (SequencesFormPtr sqfp, SeqEntryPtr nsep,
                                           SeqEntryPtr top)

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
        if (AutomaticNucleotideProcess (sqfp, tmp, top)) {
          rsult = TRUE;
        }
      }
    }
    return rsult;
  }
  return ProcessOneNucleotideTitle (sqfp->seqPackage, 
                                    nsep, top);
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


static void PutMolInfoOnSeqEntry (SequencesFormPtr sqfp, SeqEntryPtr sep)

{
  BioseqSetPtr bssp;
  MolInfoPtr   mip;
  Boolean      partial5;
  Boolean      partial3;
  ValNodePtr   vnp;

  return;
  if (sqfp != NULL && sep != NULL) {
    if (IS_Bioseq_set (sep))
    {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) 
      {
      	PutMolInfoOnSeqEntry (sqfp, sep);
      }
      return;
    }

    vnp = SeqEntryGetSeqDescr (sep, Seq_descr_molinfo, NULL);
    if (vnp == NULL)
    {
      vnp = CreateNewDescriptor (sep, Seq_descr_molinfo);
    }
    if (vnp != NULL)
    {
      mip = (MolInfoPtr) vnp->data.ptrvalue;
      if (mip == NULL)
      {
        mip = MolInfoNew ();
        vnp->data.ptrvalue = mip;
      }
      if (mip != NULL)
      {
        partial5 = GetStatus (sqfp->partial5);
        partial3 = GetStatus (sqfp->partial3);
        if (partial5 && partial3) {
          mip->completeness = 5;
        } else if (partial5) {
          mip->completeness = 3;
        } else if (partial3) {
          mip->completeness = 4;
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
                         (IsPopPhyEtcSet (bssp->_class)))) {
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
  if ((StringICmp (taxname, "Human immunodeficiency virus type 1") == 0) ||
      (StringICmp (taxname, "Human immunodeficiency virus 1") == 0)) {
    StringCpy (taxname, "HIV-1");
  } else if ((StringICmp (taxname,"Human immunodeficiency virus type 2")==0) ||
	     (StringICmp (taxname,"Human immunodeficiency virus 2")==0)) {
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
    if (sqfp->seqPackage == SEQ_PKG_GENOMICCDNA
        || PackageTypeIsSingle (sqfp->seqPackage))
    {
      return;
    }
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

static void AssignOneProtein 
(SeqEntryPtr      prot_sep, 
 SequencesFormPtr sqfp,
 SeqEntryPtr      assign_sep,
 BioseqPtr        nucbsp,
 Int2             code)
{
  MolInfoPtr        mip;
  SeqEntryPtr       msep = NULL;
  BioseqPtr         protbsp;
  SeqLocPtr         slp;
  Int4              mindiff;
  Boolean           partialN;
  Boolean           partialC;
  ValNodePtr        vnp;
  
  if (prot_sep == NULL
      || sqfp == NULL
      )
  {
    return;
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
    vnp = CreateNewDescriptor (prot_sep, Seq_descr_molinfo);
    if (vnp != NULL) {
      vnp->data.ptrvalue = (Pointer) mip;
    }
  }
  if (assign_sep != NULL) {
    if (sqfp->seqPackage == SEQ_PKG_GENOMICCDNA) {
      ClearBatchSuggestNucleotide ();
      msep = FindRnaByRefOnProtein (assign_sep, prot_sep);
      if (msep == NULL) {
        msep = FindRnaByRefOnRna (assign_sep, prot_sep);
      }
      if (msep == NULL && nucbsp != NULL && IS_Bioseq (prot_sep)) {
        protbsp = (BioseqPtr) prot_sep->data.ptrvalue;
        if (protbsp != NULL) {
          slp = PredictCodingRegion (nucbsp, protbsp, code);
          if (slp != NULL) {
            mindiff = INT4_MAX;
            FindRnaByLocationOverlap (assign_sep, slp, &mindiff, &msep);
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
      msep = assign_sep;
    }
    AddSeqEntryToSeqEntry (msep, prot_sep, TRUE);
    AutomaticProteinProcess (msep, prot_sep, code, sqfp->makeMRNA, NULL);
  } else {
    AutomaticProteinProcess (assign_sep, prot_sep, code, sqfp->makeMRNA, NULL);
  }  
}

static SeqEntryPtr FindSeqEntryWithTranscriptID (SeqEntryPtr sep, CharPtr transcript_id)
{
  SeqEntryPtr  found_sep = NULL;
  BioseqPtr    nbsp;
  SeqIdPtr     sip, sip_next;
  Char         tmp [128];
  BioseqSetPtr bssp;
  
  if (IS_Bioseq (sep))
  {
    nbsp = sep->data.ptrvalue;
    for (sip = nbsp->id; sip != NULL && found_sep == NULL; sip = sip_next)
    {
      sip_next = sip->next;
      sip->next = NULL;
      SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
      sip->next = sip_next;
      if (StringCmp (tmp, transcript_id) == 0)
      {
        found_sep = sep;
      }
    }
  }
  else
  {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    for (sep = bssp->seq_set; sep != NULL && found_sep == NULL; sep = sep->next)
    {
      found_sep = FindSeqEntryWithTranscriptID (sep, transcript_id);
    }
  }
  return found_sep; 
}

static SeqEntryPtr FindSeqEntryWithID (SeqEntryPtr sep_list, SeqIdPtr sip)
{
  BioseqPtr    bsp;
  BioseqSetPtr bssp;
  SeqEntryPtr  return_sep = NULL, sep;
  
  
  if (sep_list == NULL || sep_list->data.ptrvalue == NULL) 
  {
    return NULL;
  }
  else if (IS_Bioseq (sep_list))
  {
    bsp = sep_list->data.ptrvalue;
    if (SeqIdIn (sip, bsp->id))
    {
      return_sep = sep_list;
    }
  }
  else if (IS_Bioseq_set (sep_list))
  {
    bssp = sep_list->data.ptrvalue;
    for (sep = bssp->seq_set; sep != NULL && return_sep == NULL; sep = sep->next)
    {
      return_sep = FindSeqEntryWithID (sep, sip);
    }
  }
  return return_sep;
}

static void GetNucList (SeqEntryPtr sep, ValNodePtr PNTR sip_list)
{
  BioseqPtr    bsp;
  BioseqSetPtr bssp;
  
  if (sep == NULL || sep->data.ptrvalue == NULL)
  {
    return;
  }
  if (IS_Bioseq (sep))
  {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (!ISA_na (bsp->mol))
    {
      return;
    }
    else
    {
      ValNodeAddPointer (sip_list, 0, bsp->id);
    }
  }
  else if (IS_Bioseq_set (sep))
  {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    
    for (sep = bssp->seq_set; sep != NULL; sep = sep->next)
    {
      GetNucList (sep, sip_list);
    }
  }
}

typedef struct nucforprot
{
  Boolean done;
  PopuP   nuc_list;
} NucForProtData, PNTR NucForProtPtr;

static void AcceptNucForProt (ButtoN b)
{
  NucForProtPtr nfpp;
  
  nfpp = (NucForProtPtr) GetObjectExtra (b);
  if (nfpp == NULL) return;
  nfpp->done = TRUE;
}

static void ChooseMapType (GrouP g)
{
  NucForProtPtr nfpp;

  nfpp = (NucForProtPtr) GetObjectExtra (g);
  if (nfpp == NULL) return;
  
  if (GetValue (g) == 1)
  {
    Enable (nfpp->nuc_list);
  }
  else
  {
    Disable (nfpp->nuc_list);
  }
}

static SeqEntryPtr 
PromptForNucForProt 
(ValNodePtr sip_list,
 BioseqPtr pbsp,
 Int4 prot_pos, 
 Int4 num_prots,
 BoolPtr use_position,
 SeqEntryPtr top_sep)
{
  WindoW         w;
  NucForProtData nfpd;
  GrouP          h, map_choice = NULL;
  PrompT         p = NULL;
  ButtoN         b;
  ValNodePtr     vnp;
  SeqIdPtr       sip = NULL;
  Char           tmp [128];
  Char           prompt_text[200];
  Int4           sel, count;
  SeqEntryPtr    nsep;

  if (sip_list == NULL || pbsp == NULL || use_position == NULL) return NULL;

  if (*use_position)
  {
    sel = prot_pos + 1;
  }
  else
  {
    w = ModalWindow(-20, -13, -10, -10, NULL);
    h = HiddenGroup (w, -1, 0, NULL);
  
    sip = SeqIdFindWorst (pbsp->id);
    SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
  
    if (num_prots == ValNodeLen (sip_list))
    {
      map_choice = HiddenGroup (h, 0, 2, ChooseMapType);
      SetObjectExtra (map_choice, &nfpd, NULL);
      RadioButton (map_choice, "Match sequence 1 to protein 1, sequence 2 to protein 2, etc.");
      sprintf (prompt_text, "Please specify the nucleotide sequence for protein %s (#%d)",
             tmp, prot_pos + 1);
      RadioButton (map_choice, prompt_text);
      SetValue (map_choice, 2);
    }
    else
    {
      sprintf (prompt_text, "Please specify the nucleotide sequence for protein %s (#%d)",
               tmp, prot_pos + 1);

      p = StaticPrompt (h, prompt_text, 0, popupMenuHeight, programFont, 'l');
    }
    nfpd.nuc_list = PopupList (h, TRUE, NULL);
    for (vnp = sip_list; vnp != NULL; vnp = vnp->next)
    {
      sip = (SeqIdPtr) vnp->data.ptrvalue;
      SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
      PopupItem (nfpd.nuc_list, tmp);
    }
    SetValue (nfpd.nuc_list, 1);
  
    b = PushButton(h, "OK", AcceptNucForProt);
    SetObjectExtra (b, &nfpd, NULL);
    if (map_choice == NULL)
    {
      AlignObjects (ALIGN_CENTER, (HANDLE) p, (HANDLE) nfpd.nuc_list, (HANDLE) b, NULL);
    }
    else
    {
      AlignObjects (ALIGN_CENTER, (HANDLE) map_choice, (HANDLE) nfpd.nuc_list, (HANDLE) b, NULL);
    }
    Show(w); 
    Select (w);
    nfpd.done = FALSE;
    while (!nfpd.done)
    {
      ProcessExternalEvent ();
      Update ();
    }
    ProcessAnEvent ();
    if (map_choice == NULL || GetValue (map_choice) == 2)
    {
      sel = GetValue (nfpd.nuc_list);    
    }
    else
    {
      sel = prot_pos + 1;
      *use_position = TRUE;
    }    
    Remove (w);
  }
  for (vnp = sip_list, count = 1; vnp != NULL && count < sel; vnp = vnp->next, count++)
  {
  }
  if (vnp != NULL)
  {
    sip = (SeqIdPtr)vnp->data.ptrvalue;
  }
  
  nsep = FindSeqEntryWithID (top_sep, sip);
  return nsep;
}

static SeqEntryPtr FindNucForProtBySeqIDInTitle (SeqEntryPtr prot_sep, SeqEntryPtr nuc_list)
{
  CharPtr     title = NULL, cp, id_text = NULL;
  ObjectIdPtr oip = NULL;
  SeqIdPtr    sip;
  SeqEntryPtr nsep = NULL;

  SeqEntryExplore (prot_sep, (Pointer) (&title), FindFirstTitle);
  if (title != NULL) 
  {
    cp = StringChr (title, '[');
    if (cp == NULL)
    {
      id_text = StringSave (title);
    }
    else if (cp != title)
    {
      id_text = StringSave (title);
      id_text [cp - title] = 0;
    }
    if (id_text != NULL)
    {
      oip = ObjectIdNew ();
      if (oip != NULL)
      {
        oip->str = id_text;
        id_text = NULL;
        sip = ValNodeNew(NULL);
        if (sip != NULL)
        {
          sip->choice = SEQID_LOCAL;
          sip->data.ptrvalue = oip;
          oip = NULL;
          nsep = FindSeqEntryWithID (nuc_list, sip);
          sip = SeqIdFree (sip);
        }
        oip = ObjectIdFree (oip);
      }
      id_text = MemFree (id_text);
    }
  }
  return nsep;
}

static void AssignOneProteinForSequenceSet 
(SeqEntryPtr      prot_sep,
 SeqEntryPtr      nuc_list,
 Int2             code,
 Int4             prot_list_position,
 Int4             num_prots,
 BoolPtr          use_position,
 SequencesFormPtr sqfp)
{
  SeqEntryPtr nsep = NULL;
  BioseqPtr   nbsp = NULL, pbsp;
  ValNodePtr  sip_list = NULL;
  SeqIdPtr    sip;
  
  if (prot_sep == NULL || nuc_list == NULL || ! IS_Bioseq (prot_sep))
  {
    return;
  }
  
  pbsp = prot_sep->data.ptrvalue;

  GetNucList (nuc_list, &sip_list);
  if (sip_list == NULL)
  {
    return;
  }
  if (sip_list->next == NULL)
  {
    sip = (SeqIdPtr) sip_list->data.ptrvalue;
    nsep = FindSeqEntryWithID (nuc_list, sip);
  }
  else
  {
    if ((nsep = FindNucForProtBySeqIDInTitle (prot_sep, nuc_list)) != NULL)
    {
      /* found it, done */
    }
    else
    {
      nsep = PromptForNucForProt (sip_list, pbsp, prot_list_position, num_prots,
                                  use_position, nuc_list);
    } 
  }
    
  if (nsep == NULL || ! IS_Bioseq (nsep))
  {
    Message (MSG_ERROR, "Unable to find nucleotide for protein %d", prot_list_position + 1);
    return;
  }
  
  nbsp = (BioseqPtr) nsep->data.ptrvalue;
  if (nbsp != NULL) {
    SetBatchSuggestNucleotide (nbsp, code);
  }
  AssignOneProtein (prot_sep, sqfp, nsep, nbsp, code);
    
  if (nbsp != NULL) {
    ClearBatchSuggestNucleotide ();
  }
}

static void AssignProteinsForSequenceSet
(SeqEntryPtr      nuc_list,
 SeqEntryPtr      prot_list,
 SequencesFormPtr sqfp,
 Int2             code)
{
  Int2        count = 0;
  Char        str [128];
  Char        tmp [128];
  MonitorPtr  mon;
  BioseqPtr   bsp;
  SeqIdPtr    sip;
  SeqEntryPtr prot_sep, prot_next;
  Int4        list_position = 0;
  Int4        num_prots;
  Boolean     use_position = FALSE;
  
  num_prots = ValNodeLen (prot_list);
  mon = MonitorStrNewEx ("Predicting Coding Region", 20, FALSE);
  for (prot_sep = prot_list; prot_sep != NULL; prot_sep = prot_next, list_position++)
  {
    prot_next = prot_sep->next;
    prot_sep->next = NULL;    
    count++;
    if (mon != NULL) {
      str [0] = '\0';
      tmp [0] = '\0';
      bsp = (BioseqPtr) prot_list->data.ptrvalue;
      if (bsp != NULL) {
        sip = SeqIdFindWorst (bsp->id);
        SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
      }
      sprintf (str, "Processing sequence %d [%s]", (int) count, tmp);
      MonitorStrValue (mon, str);
      Update ();
    }
    AssignOneProteinForSequenceSet (prot_sep, nuc_list, code, list_position, 
                                    num_prots, &use_position, sqfp);  
  }
  mon = MonitorFree (mon);
  Update ();
}

static void AssignProteinsForSingleSequence 
(SeqEntryPtr      sep,
 SeqEntryPtr      prot_list,
 SequencesFormPtr sqfp,
 Int2             code)
{
  BioseqPtr   nucbsp = NULL;
  SeqEntryPtr nucsep;
  SeqEntryPtr prot_next;
  Int2        count = 0;
  Char        str [128];
  Char        tmp [128];
  MonitorPtr  mon;
  BioseqPtr   bsp;
  SeqIdPtr    sip;
  
  nucsep = FindNucSeqEntry (sep);
  if (nucsep != NULL && IS_Bioseq (nucsep)) {
    nucbsp = (BioseqPtr) nucsep->data.ptrvalue;
  }
  if (nucbsp != NULL) {
    SetBatchSuggestNucleotide (nucbsp, code);
  }
  mon = MonitorStrNewEx ("Predicting Coding Region", 20, FALSE);
  count = 0;
  while (prot_list != NULL) {
    prot_next = prot_list->next;
    prot_list->next = NULL;
    count++;
    if (mon != NULL) {
      str [0] = '\0';
      tmp [0] = '\0';
      bsp = (BioseqPtr) prot_list->data.ptrvalue;
      if (bsp != NULL) {
        sip = SeqIdFindWorst (bsp->id);
        SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
      }
      sprintf (str, "Processing sequence %d [%s]", (int) count, tmp);
      MonitorStrValue (mon, str);
      Update ();
    }
    AssignOneProtein (prot_list, sqfp, sep, nucbsp, code);

    prot_list = prot_next;
  }
  if (nucbsp != NULL) {
    ClearBatchSuggestNucleotide ();
  }
  mon = MonitorFree (mon);
  Update ();
}

static Pointer FastaSequencesFormToSeqEntryPtr (ForM f)

{
  Int2              ambig;
  Int2              annotType;
  BioSourcePtr      biop = NULL;
  BioseqPtr         bsp;
  BioseqSetPtr      bssp;
  Int2              code;
  SeqAnnotPtr       curr;
  DatePtr           dp;
  FastaPagePtr      fpp;
  IdListPtr         head;
  SeqEntryPtr       list;
  SeqEntryPtr       next;
  SeqEntryPtr       nsep;
  SeqEntryPtr       nucsep;
  ValNodePtr        nulvnp;
  ValNodePtr        nxtvnp;
  Boolean           partialmRNA5;
  Boolean           partialmRNA3;
  CharPtr           plural;
  SeqAnnotPtr       sap;
  SeqAnnotPtr PNTR  sapp;
  BioseqPtr         segseq;
  SeqEntryPtr       sep;
  SequencesFormPtr  sqfp;
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
    code = 1;
    list = NULL;
    fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
    if (fpp != NULL) {
      list = fpp->list;
      fpp->list = NULL;
    }
    if (sqfp->seqPackage == SEQ_PKG_GENOMICCDNA
        || PackageTypeIsSet (sqfp->seqPackage))
    {
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
    if (list != NULL) {
      if (list->next == NULL) {
        OnlyOneComponentWarning (sqfp);
      }
      while (list != NULL) {
        next = list->next;
        list->next = NULL;
        if (sep != NULL) {
          AddSeqEntryToSeqEntry (sep, list, TRUE);
          AutomaticNucleotideProcess (sqfp, list, list);
        } else {
          sep = list;
          AutomaticNucleotideProcess (sqfp, list, list);
        }
        if (sqfp->seqPackage == SEQ_PKG_GENOMICCDNA
            || PackageTypeIsSet (sqfp->seqPackage)) 
        {
          PutMolInfoOnSeqEntry (sqfp, list);
        }
        list = next;
      }
    }
    if (sep != NULL) {
      sqfp->dnamolfrommolinfo = 0;
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
    if (sqfp->seqPackage != SEQ_PKG_SEGMENTED &&
        sqfp->seqPackage != SEQ_PKG_GENOMICCDNA) {
      if (! TextHasNoText (sqfp->defline)) {
        ApplyAnnotationToAll (ADD_TITLE, sep, sqfp->partialLft, sqfp->partialRgt,
                              NULL, NULL, NULL, NULL, NULL, sqfp->defline);
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
      if (PackageTypeIsSingle (sqfp->seqPackage)
          || sqfp->seqPackage == SEQ_PKG_GENOMICCDNA)
      {
        AssignProteinsForSingleSequence (sep, list, sqfp, code);
      }
      else
      {
        AssignProteinsForSequenceSet (sep, list, sqfp, code);
      }
    }
    if (biop != NULL) {
      vnp = CreateNewDescriptor (sep, Seq_descr_source);
      if (vnp != NULL) {
        vnp->data.ptrvalue = (Pointer) biop;
      }
    }
    if (PackageTypeIsSet (sqfp->seqPackage)) {
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
	      SeqDescrAddPointer (&(sap->desc), Annot_descr_user, (Pointer) uop);

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
    }
    if (sqfp->seqPackage != SEQ_PKG_SEGMENTED &&
        sqfp->seqPackage != SEQ_PKG_GENOMICCDNA) {
      annotType = GetValue (sqfp->annotType);
      if (annotType > 0) {
        switch (annotType) {
          case 1 :
            ApplyAnnotationToAll (ADD_IMP, sep, sqfp->partialLft, sqfp->partialRgt,
                                  sqfp->geneName, NULL, NULL, NULL,
                                  sqfp->featcomment, NULL);
            break;
          case 2 :
            ApplyAnnotationToAll (ADD_RRNA, sep, sqfp->partialLft, sqfp->partialRgt,
                                  sqfp->geneName, NULL, NULL, sqfp->protOrRnaName,
                                  sqfp->featcomment, NULL);
            break;
          case 3 :
            ambig = ApplyAnnotationToAll (ADD_CDS, sep, sqfp->partialLft, sqfp->partialRgt,
                                          sqfp->geneName, sqfp->protOrRnaName, sqfp->protDesc, NULL,
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
  BioseqPtr         bsp;
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
              if (StringICmp (oip->str, "nuc_1") == 0) {
                GetTitle (sqfp->singleSeqID, str, sizeof (str));
                if (! StringHasNoText (str)) {
                  oip->str = MemFree (oip->str);
                  oip->str = StringSaveNoNull (str);
                  SeqMgrReplaceInBioseqIndex (bsp);
                }
                if (StringICmp (oip->str, "nuc_1") == 0) {
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
  ValNodePtr        vnp;

  sep = NULL;
  sqfp = (SequencesFormPtr) GetObjectExtra (f);
  if (sqfp != NULL) {
    code = 1;
    ppp = (PhylipPagePtr) GetObjectExtra (sqfp->dnaseq);
    if (ppp != NULL) {
      sep = ppp->sep;
      ppp->sep = NULL;
    }
    if (sep != NULL) {

      if (IS_Bioseq_set (sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        if (bssp != NULL && (bssp->_class == 7 ||
                             (IsPopPhyEtcSet (bssp->_class)))) {
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
            AutomaticNucleotideProcess (sqfp, tmp, tmp);
            PutMolInfoOnSeqEntry (sqfp, tmp);
          }
        }
      } else {
        OnlyOneComponentWarning (sqfp);
        PutMolInfoOnSeqEntry (sqfp, sep);
      }
      dp = DateCurr ();
      if (dp != NULL) {
        vnp = CreateNewDescriptor (sep, Seq_descr_create_date);
        if (vnp != NULL) {
          vnp->data.ptrvalue = (Pointer) dp;
        }
      }
    }
    if (PackageTypeIsSet (sqfp->seqPackage)) {
      if (! TextHasNoText (sqfp->defline)) {
        ApplyAnnotationToAll (ADD_TITLE, sep, sqfp->partialLft, sqfp->partialRgt,
                              NULL, NULL, NULL, NULL, NULL, sqfp->defline);
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
                                sqfp->geneName, NULL, NULL, NULL,
                                sqfp->featcomment, NULL);
          break;
        case 2 :
          ApplyAnnotationToAll (ADD_RRNA, sep, sqfp->partialLft, sqfp->partialRgt,
                                sqfp->geneName, NULL, NULL, sqfp->protOrRnaName,
                                sqfp->featcomment, NULL);
          break;
        case 3 :
          ambig = ApplyAnnotationToAll (ADD_CDS, sep, sqfp->partialLft, sqfp->partialRgt,
                                        sqfp->geneName, sqfp->protOrRnaName, sqfp->protDesc, NULL,
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

static void SeqEntryPtrToSourceTab (SequencesFormPtr sqfp);
static SeqEntryPtr GetSeqEntryFromSequencesForm (SequencesFormPtr sqfp);
static void FixDefinitionLines (SeqEntryPtr sep, BoolPtr cancelled, ValNodePtr PNTR fixes);
static ValNodePtr FreeStringPairList (ValNodePtr list);
static void ReplaceOrgWithOrganismInDefinitionLines (SeqEntryPtr sep);
static void ReplaceMolNamesWithMolBracketsInDefinitionLines (SeqEntryPtr sep);
static Boolean CheckSequencesForOrganisms (SequencesFormPtr sqfp);
static void SequencesFormDeleteProc (Pointer formDataPtr);

static void NucleotideImportFinish (SequencesFormPtr sqfp)
{
  FastaPagePtr  fpp;
  PhylipPagePtr ppp;
  SeqEntryPtr   sep = NULL;
  ValNodePtr    fixes = NULL;
  Boolean       cancelled = FALSE;
  
  if (sqfp == NULL) return;
  
  Enable (sqfp->import_mod_btn);
  Enable (sqfp->source_assist_btn);
  Enable (sqfp->specify_orgs_btn);
  Enable (sqfp->specify_locs_btn);
  Enable (sqfp->specify_gcode_btn);
  Enable (sqfp->specify_mgcode_btn);
  Enable (sqfp->clear_mods_btn);

  if (sqfp->seqFormat == SEQ_FMT_FASTA) {
    fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
    if (fpp != NULL) 
    {
      sep = fpp->list;
    }
  } else if (sqfp->seqFormat == SEQ_FMT_ALIGNMENT) {
    ppp = (PhylipPagePtr) GetObjectExtra (sqfp->dnaseq);
    if (ppp != NULL) {
      sep = ppp->sep;
    }
  }
  
  ReplaceOrgWithOrganismInDefinitionLines (sep);
  ReplaceMolNamesWithMolBracketsInDefinitionLines (sep);
  
  FixDefinitionLines (sep, &cancelled, &fixes);
  fixes = FreeStringPairList (fixes);
  if (cancelled)
  {
    SequencesFormDeleteProc (sqfp);    
  }

  SeqEntryPtrToSourceTab (sqfp);
}

static CharPtr GetFirstModValueFromSeqEntryTitles (SeqEntryPtr sep, CharPtr mod_name)
{
  CharPtr value = NULL;
  
  while (sep != NULL && value == NULL)
  {
    value = GetModValueFromSeqEntry (sep, mod_name);
    sep = sep->next;
  }
  return value;
}

static CharPtr GetOrganismNameFromSeqEntry (SeqEntryPtr sep)
{
  return GetFirstModValueFromSeqEntryTitles (sep, "organism");
}

static CharPtr GetFirstOrgNameFromSeqEntryTitles (SeqEntryPtr sep)
{
  CharPtr org_name = NULL;
  
  while (sep != NULL && org_name == NULL)
  {
    org_name = GetOrganismNameFromSeqEntry (sep);
    sep = sep->next;
  }
  return org_name;
}

static CharPtr GetTaxNameFromBiop (BioSourcePtr biop) 
{
  if (biop == NULL || biop->org == NULL || StringHasNoText (biop->org->taxname))
  {
  	return NULL;
  }
  else
  {
  	return StringSave (biop->org->taxname);
  }
}

static SeqIdPtr GetSeqIdFromSeqEntryPtr (SeqEntryPtr sep) 
{
  BioseqPtr    bsp;
  BioseqSetPtr bssp;
  
  if (sep == NULL || sep->data.ptrvalue == NULL) {
  	return NULL;
  }
  if (IS_Bioseq (sep)) {
  	bsp = (BioseqPtr) sep->data.ptrvalue;
    return bsp->id;
  } else if (IS_Bioseq_set(sep)) {
  	bssp = (BioseqSetPtr) sep->data.ptrvalue;
    return GetSeqIdFromSeqEntryPtr (bssp->seq_set);
  }
  return NULL;
}

static Boolean 
CheckOneSequenceForOrganisms 
(SeqEntryPtr sep,
 ValNodePtr PNTR no_org_list)
{
  CharPtr org_name_from_title;
  Boolean rval = FALSE;
  
  org_name_from_title = GetOrganismNameFromSeqEntry (sep);
 	if (org_name_from_title == NULL && no_org_list != NULL)
 	{
 	  rval = FALSE;
 	  ValNodeAddPointer (no_org_list, 0, GetSeqIdFromSeqEntryPtr (sep));
 	}
 	else
 	{
 	  rval = TRUE;
 	}
  org_name_from_title = MemFree (org_name_from_title);
  return rval;
}

static Boolean
CheckSegmentedSetForConflictOrganisms
(SeqEntryPtr     sep_list,
 ValNodePtr PNTR no_org_list,
 ValNodePtr PNTR conflict_org_list,
 CharPtr    PNTR conflict_org_name)
{
  Boolean      rval;
  CharPtr      first_org_name = NULL, other_org_name;
  SeqEntryPtr  sep, check_sep = NULL;
  BioseqPtr    bsp;
  BioseqSetPtr parts;
  
  if (sep_list == NULL || sep_list->data.ptrvalue == NULL
      || ! IS_Bioseq (sep_list))
  {
    return FALSE;
  }
  
  /* first segment should have organism name */
  rval = CheckOneSequenceForOrganisms (sep_list, no_org_list);
  first_org_name = GetFirstOrgNameFromSeqEntryTitles (sep_list);
  
  bsp = (BioseqPtr) sep_list->data.ptrvalue;
  if (bsp->repr == Seq_repr_seg)
  {
    if (sep_list->next != NULL
        && sep_list->next->data.ptrvalue != NULL
        && IS_Bioseq_set (sep_list->next))
    {
      parts = (BioseqSetPtr) sep_list->next->data.ptrvalue;
      check_sep = parts->seq_set;      
    }
  }
  else
  {
    check_sep = sep_list->next;
  }
  
  
  for (sep = sep_list->next; sep != NULL; sep = sep->next)
  {
    other_org_name = GetFirstOrgNameFromSeqEntryTitles (sep);
    if (other_org_name != NULL 
        && StringICmp (other_org_name, first_org_name) != 0
        && IS_Bioseq (sep)
        && sep->data.ptrvalue != NULL)
    {
      rval = FALSE;
      bsp = (BioseqPtr) sep->data.ptrvalue;
      ValNodeAddPointer (conflict_org_list, 0, bsp->id);
    }
    other_org_name = MemFree (other_org_name);
  }

  if (conflict_org_name != NULL)
  {
    *conflict_org_name = first_org_name;
  }
  else
  {
    first_org_name = MemFree (first_org_name);
  }
  return rval;
}

static void 
ReportMissingAndInconsistentOrganismNames 
(ValNodePtr no_org_list,
 ValNodePtr conflict_org_list,
 CharPtr    org_name_from_biop)
{
  Char         path [PATH_MAX];
  FILE         *fp;
  ValNodePtr   vnp;
  SeqIdPtr     sip;
  Char         tmp [128];

  if (no_org_list == NULL && conflict_org_list == NULL)
  {
    return;
  }
  
  TmpNam (path);
  fp = FileOpen (path, "wb");
  if (fp == NULL) return;

  if (no_org_list != NULL)
  {
    fprintf (fp, "The following sequences have no organism names.  You must supply one for each sequence listed.\n");
    for (vnp = no_org_list; vnp != NULL; vnp = vnp->next)
    {
      sip = vnp->data.ptrvalue;
      sip = SeqIdFindWorst (sip);
      SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
      fprintf (fp, "%s\n", tmp);
    }
  }
  if (conflict_org_list != NULL && !StringHasNoText (org_name_from_biop))
  {
    fprintf (fp, "The following sequences should all have the same organism name (%s).\n", org_name_from_biop);
    for (vnp = conflict_org_list; vnp != NULL; vnp = vnp->next)
    {
      sip = vnp->data.ptrvalue;
      sip = SeqIdFindWorst (sip);
      SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
      fprintf (fp, "%s\n", tmp);
    }    
  }

  FileClose (fp);
  LaunchGeneralTextViewer (path, "Organism Errors");
  FileRemove (path);
}


static Boolean CheckSequencesForOrganisms (SequencesFormPtr sqfp)
{
  SeqEntryPtr  sep_list, sep;
  BioSourcePtr biop = NULL;
  ValNodePtr   no_org_list = NULL;
  ValNodePtr   conflict_org_list = NULL;
  CharPtr      conflict_org_name = NULL;
  Boolean      rval = FALSE;
  
  if (sqfp == NULL) return FALSE;
  sep_list = GetSeqEntryFromSequencesForm (sqfp);
  if (sep_list == NULL) return FALSE;
  
  /* different organism checks for different types of sets */
  switch (sqfp->seqPackage)
  {
  	case SEQ_PKG_SINGLE:
    case SEQ_PKG_GAPPED:
    case SEQ_PKG_GENOMICCDNA:
      rval = CheckOneSequenceForOrganisms (sep_list, &no_org_list);
      break;
  	case SEQ_PKG_SEGMENTED:
      /* only complain if first segment does not have organism name, later segments don't need one */
      rval = CheckSegmentedSetForConflictOrganisms (sep_list, &no_org_list, 
                                                    &conflict_org_list, &conflict_org_name);
  	  break;
  	case SEQ_PKG_POPULATION:
      rval = TRUE;
      for (sep = sep_list; sep != NULL; sep = sep->next)
      {
        rval &= CheckOneSequenceForOrganisms (sep, &no_org_list);
      }
  	  break;
  	case SEQ_PKG_PHYLOGENETIC:
      rval = TRUE;
      for (sep = sep_list; sep != NULL; sep = sep->next)
      {
        rval &= CheckOneSequenceForOrganisms (sep, &no_org_list);
      }
  	  break;
  	case SEQ_PKG_MUTATION:
  	case SEQ_PKG_ENVIRONMENT:
    case SEQ_PKG_GENBANK:
      rval = TRUE;
      for (sep = sep_list; sep != NULL; sep = sep->next)
      {
        rval &= CheckOneSequenceForOrganisms (sep, &no_org_list);
      }
  	  break;
  }
  
  ReportMissingAndInconsistentOrganismNames (no_org_list,
                                             conflict_org_list, 
                                             conflict_org_name);
  no_org_list = ValNodeFree (no_org_list);
  conflict_org_list = ValNodeFree (conflict_org_list);
  conflict_org_name = MemFree (conflict_org_name);
  return rval;
}

static Boolean ImportSequencesForm (ForM f, CharPtr filename)

{
  SequencesFormPtr  sqfp;
  Boolean           rval = FALSE;

  sqfp = (SequencesFormPtr) GetObjectExtra (f);
  if (sqfp != NULL) {
    switch (sqfp->tagFromPage [sqfp->currentPage]) {
      case NUCLEOTIDE_PAGE :
        rval = ImportDialog (sqfp->dnaseq, "");
        if (rval)
        {
          NucleotideImportFinish (sqfp);
        }
        break;
      case MRNA_PAGE :
        rval = ImportDialog (sqfp->mrnaseq, "");
        break;
      case PROTEIN_PAGE :
        rval = ImportDialog (sqfp->protseq, "");
        break;
      case ORGANISM_PAGE:
        rval = ImportOrganismModifiersForSubmit (GetSeqEntryFromSequencesForm (sqfp));
        if (rval)
        {
          SeqEntryPtrToSourceTab (sqfp);
        }
        break;
      default :
        break;
    }
  }
  return rval;
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
        SafeSetTitle (importItm, "Import Organism Modifiers From File");
        SafeSetTitle (exportItm, "Export...");
        SafeEnable (importItm);
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
          case SEQ_FMT_ALIGNMENT :
            SafeSetTitle (importItm, "Import Nucleotide Alignment...");
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

static Boolean SequenceAssistantValidate (SeqEntryPtr seq_list, Int2 seqPackage);
static Boolean FinalSequenceValidation (SequencesFormPtr  sqfp)
{
  FastaPagePtr fpp;
  
  if (sqfp == NULL)
  {
    return FALSE;
  }
  if (sqfp->seqFormat == SEQ_FMT_ALIGNMENT || sqfp->seqPackage == SEQ_PKG_GAPPED)
  {
    /* we can't edit these, so there's no sense pestering the user...*/
    return TRUE;
  }
  fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
  if (fpp == NULL || fpp->list == NULL)
  {
    return FALSE;
  }
  
  return SequenceAssistantValidate (fpp->list, sqfp->seqPackage);
}

static void NextSequencesFormBtn (ButtoN b)

{
  SequencesFormPtr  sqfp;

  sqfp = (SequencesFormPtr) GetObjectExtra (b);
  if (sqfp != NULL) {
    if (sqfp->currentPage < sqfp->numPages - 1) {
      SetValue (sqfp->tbs, sqfp->currentPage + 1);
    } else if (sqfp->goToNext != NULL) {
      if (!CheckSequencesForOrganisms (sqfp) || !FinalSequenceValidation (sqfp)) return;
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

static void ShowSingleOrgSelector (SequencesFormPtr sqfp);
static void SetModifierList (DoC doc, ValNodePtr mod_list);
static void SeqEntryPtrToOrgDoc (SequencesFormPtr sqfp);

static void ClearOrganismModifiers (SequencesFormPtr sqfp)
{
  if (sqfp == NULL) return;
  Disable (sqfp->import_mod_btn);
  Disable (sqfp->source_assist_btn);
  Disable (sqfp->specify_orgs_btn);
  Disable (sqfp->specify_locs_btn);
  Disable (sqfp->specify_gcode_btn);
  Disable (sqfp->specify_mgcode_btn);
  Disable (sqfp->clear_mods_btn);

  SeqEntryPtrToOrgDoc (sqfp);
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
            SetTitle (fpp->import_btn, "Import Nucleotide FASTA");
            Enable (fpp->import_btn);
            Disable (fpp->clear_btn);
          }
        } else if (sqfp->seqFormat == SEQ_FMT_ALIGNMENT) {
          ppp = (PhylipPagePtr) GetObjectExtra (sqfp->dnaseq);
          if (ppp != NULL) {
            ResetPhylipPage (ppp);
            ppp->path [0] = '\0';
            SetPhylipDocInstructions (ppp);
          }
        }
        ClearOrganismModifiers (sqfp);
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
        if (PackageTypeIsSingle (sqfp->seqPackage)) {
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
  "Nucleotide", "Organism", "Proteins", NULL
};

static CharPtr  cdnaGenFormTabs [] = {
  "Genomic", "Organism", "Transcripts", "Proteins", NULL
};

static CharPtr  popPhyMutFormTabs [] = {
  "Nucleotide", "Organism", "Annotation", "Proteins", NULL
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
        WriteSequinAppParam ("PREFERENCES", "PARSENUCSEQID", "TRUE");
        SafeHide (sqfp->singleIdGrp);
      } else {
        WriteSequinAppParam ("PREFERENCES", "PARSENUCSEQID", "FALSE");
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
        WriteSequinAppParam ("PREFERENCES", "PARSEMRNASEQID", "TRUE");
      } else {
        WriteSequinAppParam ("PREFERENCES", "PARSEMRNASEQID", "FALSE");
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
        WriteSequinAppParam ("PREFERENCES", "PARSEPROTSEQID", "TRUE");
      } else {
        WriteSequinAppParam ("PREFERENCES", "PARSEPROTSEQID", "FALSE");
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
      WriteSequinAppParam ("PREFERENCES", "CREATEMRNA", "TRUE");
    } else {
      WriteSequinAppParam ("PREFERENCES", "CREATEMRNA", "FALSE");
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
      SafeHide (sqfp->protDescPpt);
      SafeHide (sqfp->protDesc);
      SafeShow (sqfp->annotGrp);
      Select (sqfp->geneName);
      break;
    case 2 :
      SafeSetTitle (sqfp->protOrRnaPpt, "rRNA Name");
      SafeShow (sqfp->protOrRnaPpt);
      SafeShow (sqfp->protOrRnaName);
      SafeHide (sqfp->protDescPpt);
      SafeHide (sqfp->protDesc);
      SafeShow (sqfp->annotGrp);
      Select (sqfp->protOrRnaName);
      break;
    case 3 :
      SafeSetTitle (sqfp->protOrRnaPpt, "Protein Name");
      SafeShow (sqfp->protOrRnaPpt);
      SafeShow (sqfp->protOrRnaName);
      SafeShow (sqfp->protDescPpt);
      SafeShow (sqfp->protDesc);
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
>[organism=scientific name]\n\n\
Additional information, e.g., [strain=name], \
can also be included. See the help documentation for \
full details";

static CharPtr  phyloOrgPhylipMsg = "\
\nFor phylogenetic studies, you should encode the \
organism names FASTA-like definition lines after \
the normal PHYLIP, NEXUS or MACAW file. These should \
be of the following form:\n\n\
>[organism=scientific name]\n\n\
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

extern CharPtr CreateListMessage (CharPtr msg_before, CharPtr msg_after, ValNodePtr id_list)
{
  Int4       num_pos;
  ValNodePtr vnp;
  CharPtr    msg_txt;
  Int4       txt_len = StringLen (msg_before) + StringLen (msg_after) + 3;
  Char       num_buf [14];
  
  if (id_list == NULL) return NULL;
  num_pos = ValNodeLen (id_list);
  for (vnp = id_list; vnp != NULL; vnp = vnp->next)
  {
    if (StringHasNoText (vnp->data.ptrvalue))
    {
      txt_len += PRINTED_INT_MAX_LEN;
    }
    else
    {
      txt_len += StringLen (vnp->data.ptrvalue) + 5;
    }
  }
  msg_txt = (CharPtr) MemNew (txt_len * sizeof (Char));
  if (msg_txt != NULL)
  {
    msg_txt [0] = 0;
    if (msg_before != NULL)
    {
      StringCat (msg_txt, msg_before);
      if (num_pos > 1)
      {
        StringCat (msg_txt, "s ");
      }
      else
      {
        StringCat (msg_txt, " ");
      }
    }
    
    for (vnp = id_list; vnp != NULL; vnp = vnp->next)
    {
      if (num_pos > 1 && vnp->next == NULL)
      {
        StringCat (msg_txt, "and ");
      }
      if (StringHasNoText (vnp->data.ptrvalue))
      {
        sprintf (num_buf, "%d", vnp->choice);
        StringCat (msg_txt, num_buf);
      }
      else
      {
        StringCat (msg_txt, vnp->data.ptrvalue);
      }
      if (vnp->next != NULL)
      {
  	    if (num_pos > 2)
  	    {
	        StringCat (msg_txt, ", ");
  	    }
  	    else
  	    {
  	  	  StringCat (msg_txt, " ");
  	    }
      }
    }
    StringCat (msg_txt, msg_after);
  }
  return msg_txt;
}


static Boolean ContinueWithErrorList (ValNodePtr err_list, Boolean ask_for_continue)
{
  ValNodePtr           vnp;
  GrouP                required_grp = NULL;
  GrouP                warning_grp = NULL;
  GrouP                h, g, c;
  PrompT               p;
  ButtoN               b;
  WindoW                w;
  ModalAcceptCancelData acd;
  Boolean               ok_to_continue = TRUE;

  
  if (err_list == NULL) return TRUE;
  
  acd.accepted = FALSE;
  acd.cancelled = FALSE;

  w = ModalWindow(-20, -13, -10, -10, NULL);
  h = HiddenGroup (w, -1, 0, NULL);
  g = HiddenGroup (h, 1, 0, NULL);
  /* create required list */
  for (vnp = err_list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->choice == CREATE_FASTA_REQUIRED)
    {
      ok_to_continue = FALSE;
      if (required_grp == NULL)
      {
        required_grp = NormalGroup (g, 1, 0, "Required", systemFont, NULL);
      }
      MultiLinePrompt (required_grp, vnp->data.ptrvalue, 600, systemFont);
    }
  }
  /* create warning list */
  for (vnp = err_list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->choice == CREATE_FASTA_WARNING)
    {
      if (warning_grp == NULL)
      {
        warning_grp = NormalGroup (g, 1, 0, "Warning", systemFont, NULL);
      }
      MultiLinePrompt (warning_grp, vnp->data.ptrvalue, 600, systemFont);
    }
  }
  
  if (! ask_for_continue)
  {
    p = NULL;
    c = HiddenGroup (h, 1, 0, NULL);
    b = PushButton (c, "OK", ModalCancelButton);
    SetObjectExtra (b, &acd, NULL);
  }
  else if (ok_to_continue)
  {
    p = StaticPrompt (h, "Continue anyway?",
                      0, dialogTextHeight, systemFont, 'c');
    c = HiddenGroup (h, 2, 0, NULL);
    b = PushButton (c, "Yes", ModalAcceptButton);
    SetObjectExtra (b, &acd, NULL);
    b = PushButton (c, "No", ModalCancelButton);
    SetObjectExtra (b, &acd, NULL);
  }
  else
  {
    p = StaticPrompt (h, "You must resolve the required errors before continuing.",
                      0, dialogTextHeight, systemFont, 'c');
    c = HiddenGroup (h, 1, 0, NULL);
    b = PushButton (c, "OK", ModalCancelButton);
    SetObjectExtra (b, &acd, NULL);
  }
  
  if (ask_for_continue)
  {
    AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) p, (HANDLE) c, NULL);
  }
  else
  {
    AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
  }
  
  Show(w); 
  Select (w);
  while (!acd.accepted && ! acd.cancelled)
  {
    ProcessExternalEvent ();
    Update ();
  }
  ProcessAnEvent ();
  Remove (w);
  if (acd.accepted)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
  
}

static CharPtr valid_iupac_characters = "atgcbdhkmnrsuvwy";

static Boolean SeqCharsOk
(CharPtr seq_chars,
 Int4 seq_num,
 CharPtr local_id,
 ValNodePtr PNTR err_list)
{
  CharPtr cp;
  Char    ch;
  Boolean at_least_one = FALSE;
  Int4    len = StringLen (seq_chars);
  CharPtr badchars;
  CharPtr err_msg;
  CharPtr empty_fmt_d = "There are no sequence characters for sequence %d.  Please enter some.";
  CharPtr empty_fmt_s = "There are no sequence characters for sequence %s.  Please enter some.";
  CharPtr bad_char_fmt_d = "There were %d illegal characters were found in sequence %d: %s."
  	         "  Repeated characters are listed only once. "
  	         "  You may only have IUPAC characters in your sequence ";
  CharPtr bad_char_fmt_s = "There were %d illegal characters were found in sequence %s: %s."
  	         "  Repeated characters are listed only once. "
  	         "  You may only have IUPAC characters in your sequence ";
  
  if (StringHasNoText (seq_chars))
  {
    err_msg = (CharPtr) MemNew (sizeof (Char) * 
                (StringLen (empty_fmt_d) + PRINTED_INT_MAX_LEN + StringLen (local_id)));
    if (err_msg != NULL)
    {
      if (StringHasNoText (local_id))
      {
        sprintf (err_msg, empty_fmt_d, seq_num);
      }
      else
      {
        sprintf (err_msg, empty_fmt_s, local_id);
      }
      ValNodeAddPointer (err_list, CREATE_FASTA_REQUIRED, err_msg);
    }
  	return FALSE;
  }
  
  badchars = (CharPtr) MemNew (sizeof (Char) * (len + 1));
  if (badchars == NULL) return FALSE;
  badchars[0] = 0;
  len = 0;
  for (cp = seq_chars; *cp != 0; cp++)
  {
    ch = TO_LOWER (*cp);
  	if (isspace ((Int4)ch))
  	{
  	  /* space allowed */
  	}
  	else if (StringChr (valid_iupac_characters, ch) == NULL)
  	{
  	  if (StringChr (badchars, *cp) == NULL)
  	  {
  	  	badchars [len] = ch;
  	  	len++; 
  	  	badchars [len] = 0;
  	  }
  	}
  	else 
  	{
  	  at_least_one = TRUE;
  	}
  }
  if (len > 0)
  {
    err_msg = (CharPtr) MemNew (sizeof (Char) * (StringLen (bad_char_fmt_d)
               + (2 * PRINTED_INT_MAX_LEN) + StringLen (local_id) + len
               + StringLen (valid_iupac_characters)));
    if (err_msg != NULL)
    {
      if (StringHasNoText (local_id))
      {
        sprintf (err_msg, bad_char_fmt_d, len, seq_num, badchars, valid_iupac_characters);
      }
      else
      {
        sprintf (err_msg, bad_char_fmt_s, len, local_id, badchars, valid_iupac_characters);
      }
      ValNodeAddPointer (err_list, CREATE_FASTA_REQUIRED, err_msg);
    }
  	return FALSE;
  }
  if (!at_least_one)
  {
    err_msg = (CharPtr) MemNew (sizeof (Char) * 
                (StringLen (empty_fmt_d) + PRINTED_INT_MAX_LEN + StringLen (local_id)));
    if (err_msg != NULL)
    {
      if (StringHasNoText (local_id))
      {
        sprintf (err_msg, empty_fmt_d, seq_num);
      }
      else
      {
        sprintf (err_msg, empty_fmt_s, local_id);
      }
      ValNodeAddPointer (err_list, CREATE_FASTA_REQUIRED, err_msg);
    }
  	return FALSE;
  }
  
  return TRUE;
}

static Boolean IsSequenceAllNs (CharPtr seq_str)
{
  CharPtr cp;
  
  if (StringHasNoText (seq_str)) return FALSE;
  
  for (cp = seq_str; *cp != 0; cp++)
  {
    if (isalpha ((Int4)(*cp)) && *cp != 'n' && *cp != 'N')
    {
      return FALSE;
    }
  }
  return TRUE;
}

static Boolean IsSequenceAllOneCharacter (CharPtr seq_str)
{
  CharPtr cp;
  Char    first_char = 0;
  
  if (StringHasNoText (seq_str)) return FALSE;
  
  for (cp = seq_str; *cp != 0; cp++)
  {
    if (isalpha ((Int4)(*cp)))
    {
      if (first_char == 0)
      {
        first_char = *cp;
      }
      else if (*cp != first_char)
      {
        return FALSE;
      }
    }
  }
  return TRUE;
  
}

static Int4 CountSeqChars (CharPtr seq_str)
{
  CharPtr cp;
  Int4    num_chars = 0;
  
  if (StringHasNoText (seq_str)) return 0;
  for (cp = seq_str; *cp != 0; cp++)
  {
    if (isalpha ((Int4)(*cp)))
    {
      num_chars++;
    }
  }
  return num_chars;
}

static CharPtr ReformatLocalId (CharPtr local_id)
{
  CharPtr cp, new_local_id;
  
  if (local_id == NULL) return NULL;
  
  cp = local_id;
  while (*cp == '>')
  {
    cp ++;
  }
  while (isspace ((Int4)(*cp)))
  {
  	cp++;
  }
  new_local_id = StringSave (cp);
  cp = new_local_id;
  while (*cp != 0)
  {
    if (isspace ((Int4)(*cp)))
    {
      *cp = '_';
    }
    cp++;
  }
  MemFree (local_id);
  return new_local_id;
}


const CharPtr bracket_mismatch_msg = "You have mismatched brackets in the definition line %s.";
const CharPtr missing_equals_msg = "Your bracketed pairs must be in the form [qualifier=value].  "
  	                    "You are missing an equals sign in the definition line %s.";
  	                    
const CharPtr mult_equals_msg = "You have more than one equals sign inside a bracketed pair in the definition line %s.  "
                        "Each pair must be in its own set of brackets.";  	                 

const CharPtr line_num_fmt = "at line %d";
const CharPtr id_str_fmt = "for sequence %s";

static CharPtr MakeBracketingErrorMessage (CharPtr msg_format, Int4 line_num, CharPtr id_str)
{
  CharPtr identifier, str;
  
  if (msg_format == NULL) 
  {
    return NULL;
  }
  if (id_str != NULL)
  {
    identifier = (CharPtr) MemNew ((StringLen (id_str_fmt) + StringLen (id_str)) * sizeof (Char));
    if (identifier != NULL)
    {
      sprintf (identifier, id_str_fmt, id_str);
    }
  }
  else
  {
    identifier = (CharPtr) MemNew ((StringLen (line_num_fmt) + PRINTED_INT_MAX_LEN) * sizeof (Char));
    if (identifier != NULL)
    {
      sprintf (identifier, line_num_fmt, line_num);
    }
  }
  if (identifier == NULL)
  {
    return NULL;
  }
  
  str = (CharPtr) MemNew (sizeof (Char) * (StringLen (msg_format) + StringLen (identifier)));
  if (str != NULL)
  {
    sprintf (str, msg_format, identifier);
  }
  return str;
}

static void ParseMultipleEqualsSigns 
(ValNodePtr PNTR pieces,
 CharPtr start,
 CharPtr stop,
 CharPtr eq_loc)
{
  CharPtr next_eq_loc, new_start;
  CharPtr cp;
  Int4    txt_len;
  CharPtr txt;
  
  if (pieces == NULL || start == NULL || stop == NULL || eq_loc == NULL)
  {
    return;
  }
  
  new_start = start;
  next_eq_loc = StringChr (eq_loc + 1, '=');
  while (next_eq_loc != NULL && next_eq_loc < stop)
  {
    /* backtrack to end of first non-alphabet character before next_eq_loc,
     * insert ][ to separate the pairs */
    cp = next_eq_loc - 1;
    /* backtrack past space */
    while (cp != eq_loc && isspace ((Int4)(*cp)))
    {
      cp--;
    }
    /* now backtrack over name */
    while (cp != eq_loc && !(isspace ((Int4)(*cp))))
    {
      cp--;
    }
    if (cp == eq_loc)
    {
      /* remove second equals sign */
      *next_eq_loc = ' ';
    }
    else
    {
      /* insert ] */
      txt_len = cp + 2 - new_start;
      if (new_start != start)
      {
        txt_len ++;
      }
      
      txt = (CharPtr) MemNew (txt_len * sizeof (Char));
      if (txt != NULL)
      {
        txt [0] = 0;
        if (new_start != start)
        {
          StringCat (txt, "[");
          txt_len --;
        }
        StringNCat (txt, new_start, txt_len - 2);
        StringCat (txt, "]");
        ValNodeAddPointer (pieces, 0, txt);
      }
      new_start = cp + 1;
    }
    eq_loc = next_eq_loc;
    next_eq_loc = StringChr (eq_loc + 1, '=');
  }
  
  /* the current eq_loc is the last one */
  txt_len = stop - new_start + 2;
  if (new_start != start)
  {
    txt_len ++;
  }
  txt = (CharPtr) MemNew (txt_len * sizeof (Char));
  if (txt != NULL)
  {
    txt [0] = 0;
    if (new_start != start)
    {
      StringCat (txt, "[");
      txt_len --;
    }
    StringNCat (txt, new_start, txt_len - 1);
    ValNodeAddPointer (pieces, 0, txt);
  }
}

static CharPtr AssembleDefLinePieces (ValNodePtr pieces)
{
  ValNodePtr vnp;
  Int4       str_len = 0;
  CharPtr    str = NULL;
  CharPtr    first_char_in_piece;
  CharPtr    last_char_in_piece;
  
  if (pieces == NULL) return NULL;
  
  for (vnp = pieces; vnp != NULL; vnp = vnp->next)
  {
    str_len += StringLen (vnp->data.ptrvalue) + 1;
  }
  str_len ++;
  str = (CharPtr) MemNew (sizeof (Char) * str_len);
  if (str != NULL)
  {
    str [0] = 0;
    for (vnp = pieces; vnp != NULL; vnp = vnp->next)
    {
      if (StringHasNoText (vnp->data.ptrvalue))
      {
        continue;
      }
      if (vnp != pieces)
      {
        StringCat (str, " ");
      }
      first_char_in_piece = (CharPtr) vnp->data.ptrvalue;
      while (*first_char_in_piece == ' ' 
           || *first_char_in_piece == '\t')
      {
        first_char_in_piece ++;
      }
      StringCat (str, first_char_in_piece);
      /* trim out trailing spaces in piece */
      last_char_in_piece = str + StringLen(str) - 1;
      while (*last_char_in_piece == ' '
          || *last_char_in_piece == '\t')
      {
        *last_char_in_piece = 0;
        last_char_in_piece --;        
      }
    }
  }
  return str;
}

/* later, add handling for equals signs without brackets? */
static CharPtr SuggestCorrectBracketing (CharPtr str)
{
  CharPtr    start, stop, next_start, next_stop, eq_loc;
  ValNodePtr pieces = NULL;
  CharPtr    cp = str;
  Boolean    done = FALSE;
  Int4       len;
  CharPtr    txt;

  if (str == NULL) return NULL;
  
  while (!done && *cp != 0)
  {  	
    start = StringChr (cp, '[');
    stop = StringChr (cp, ']');
    eq_loc = StringChr (cp, '=');
    if (start == NULL)
    {
      next_start = NULL;
    }
    else
    {
      next_start = StringChr (start + 1, '[');    	
    }

    if (start == NULL && stop == NULL) 
    {
      /* no brackets at all */
      len = StringLen (cp) + 1;
      if (len > 1)
      {
        txt = (CharPtr) MemNew (len * sizeof (Char));
        if (txt != NULL)
        {
      	  StringNCpy (txt, cp, len - 1);
      	  txt [len - 1] = 0;
          ValNodeAddPointer (&pieces, 0, txt);
        }
      }
      done = TRUE;
    }
    else if (start != NULL && stop != NULL && eq_loc != NULL
             && start < eq_loc && eq_loc < stop 
             && (next_start == NULL || next_start > stop))
    {
      if (start != cp)
      {
        /* copy text between bracketed pairs */
        len = start - cp;
        txt = (CharPtr) MemNew (len * sizeof (Char));
        if (txt != NULL)
        {
          StringNCpy (txt, cp, len - 1);
          txt [len - 1] = 0;
          ValNodeAddPointer (&pieces, 0, txt);
        }
      }
      ParseMultipleEqualsSigns (&pieces, start, stop, eq_loc);
      cp = stop + 1;
    }
    else if (start == NULL || (stop != NULL && start > stop))
    {
  	  eq_loc = StringChr (cp, '=');
  	  if (eq_loc == NULL || eq_loc == cp || eq_loc > stop)
  	  {
  	    /* if there is no equals sign, remove the offending bracket */
  		len = stop - cp + 1;
  		txt = (CharPtr) MemNew (len * sizeof (Char));
  		if (txt != NULL)
  		{
  		  StringNCpy (txt, cp, len - 1);
  		  txt [len - 1] = 0;
  		  ValNodeAddPointer (&pieces, 0, txt);
  		}
  		cp = stop + 1;
  	  }
  	  else 
  	  {
  	  	/* find the first non-alphabet character before the equals sign and put in a bracket */
  	  	start = eq_loc - 1;
  	  	/* skip over whitespace before equals sign */
  	  	while (cp != start && isspace ((Int4)(*start)))
  	  	{
  	  	  start --;
  	  	}
  	  	/* back up past qualifier name */
  	  	while (cp != start && isalpha ((Int4)(*start)))
  	  	{
  	  	  start --;
  	  	}
  	  	if (!isalpha ((Int4)(*start)))
  	  	{
  	  	  start++;
  	  	}
  	  	/* now insert left bracket */
  	  	len = (stop - cp) + 3;
  	  	txt = (CharPtr) MemNew (len * sizeof (Char));
  		  if (txt != NULL)
  		  {
  		    if (start > cp)
  		    {
  		      StringNCpy (txt, cp, start - cp);
  		    }
  		    StringCat (txt, "[");
  		    StringNCat (txt, start, stop - start + 1);
  		    txt [len - 1] = 0;
  		    ValNodeAddPointer (&pieces, 0, txt);
        }
  		  cp = stop + 1;
  	  }
    }
    else if (stop != NULL && eq_loc != NULL && eq_loc > stop)
    {
      next_stop = StringChr (stop + 1, ']');
      if (next_stop != NULL && next_stop < next_start && eq_loc < next_stop)
      {
      	/* remove the intermediate stop */
      	len = next_stop - cp;
      	txt = (CharPtr) MemNew (len * sizeof (Char));
  		if (txt != NULL)
  		{
  		  StringNCpy (txt, cp, stop - cp);
  		  StringNCat (txt, stop + 1, next_stop - stop);
  		  ValNodeAddPointer (&pieces, 0, txt);
  		}
  		cp = next_stop + 1;
      }
      else
      {
      	/* remove both the start and stop */
      	len = stop - cp - 1;
      	txt = (CharPtr) MemNew (len * sizeof (Char));
        if (txt != NULL)
        {
          if (start > cp)
          {
          	StringNCpy (txt, cp, start - cp);
          }
          StringNCat (txt, start + 1, stop - start - 1);
  		    ValNodeAddPointer (&pieces, 0, txt);
    		}
   		  cp = stop + 1;
      }
    }
    else if (eq_loc == NULL && start != NULL && stop != NULL)
    {
      /* we have brackets around text with no equals sign */
      /* remove the brackets */
      len = stop - start;
      txt = (CharPtr) MemNew (len * sizeof (Char));
      if (txt != NULL)
      {
        StringNCpy (txt, start + 1, len - 1);
        txt [len - 1] = 0;
        ValNodeAddPointer (&pieces, 0, txt);
      }
      cp = stop + 1;
    }
    else
    {
      /* we have a start without a stop */
      eq_loc = StringChr (start, '=');
      next_start = StringChr (start + 1, '[');
      if (eq_loc == NULL || (next_start != NULL && eq_loc > next_start))
      {
      	/* if we have no equals sign, remove the offending bracket */
      	if (next_start == NULL)
      	{
      	  /* if there are no more starts, copy the rest of the string and finish */
      	  len = StringLen (cp);
      	  done = TRUE;
      	}
      	else
      	{
      	  /* copy up to the next start */
      	  len = next_start - cp;      		
      	}
      	if (len > 1)
      	{
      	  txt = (CharPtr) MemNew (len * sizeof (Char));
          if (txt != NULL)
  		    {
  		      if (cp < start)
  		      {
 		          StringNCpy (txt, cp, start - cp);
 		          if (next_start - start > 1)
 		          {
 		      	    StringNCat (txt, start + 1, next_start - start - 1);
 		          }
  		      }
  		      else
  		      {
  		        StringNCpy (txt, start + 1, len);  		    	
  		      }
  		      ValNodeAddPointer (&pieces, 0, txt);
      	  }
      	}
      	cp += len;
      }
      else
      {
      	/* put everything before the next start inside the bracket */
      	if (next_start == NULL)
      	{
      	  len = StringLen (cp) + 2;
      	  done = TRUE;
      	}
      	else
      	{
      	  len = next_start - cp + 2;
      	}
      	txt = (CharPtr) MemNew (len * sizeof (Char));
        if (txt != NULL)
  		  {
  		    StringNCpy (txt, cp, len - 2);
  		    StringCat (txt, "]");
  		    ValNodeAddPointer (&pieces, 0, txt);
      	}
      	cp += len - 2;
      }
    }
  }
  
  txt = AssembleDefLinePieces (pieces);
  ValNodeFreeData (pieces);
  return txt;
}

static CharPtr DetectBadBracketing (CharPtr str)
{
  CharPtr start, stop, next_start, next_stop, eq_loc, next_eq_loc;
  
  
  if (str == NULL) return NULL;
  
  start = StringChr (str, '[');
  stop = StringChr (str, ']');
  if (start == NULL && stop == NULL) return NULL;
  if ((start != NULL && stop == NULL)
   || (start == NULL && stop != NULL)
   || (start > stop))
  {
  	return bracket_mismatch_msg;
  }
  eq_loc = StringChr (start + 1, '=');
  if (eq_loc == NULL || eq_loc > stop)
  {
    return missing_equals_msg;
  }
  next_eq_loc = StringChr (eq_loc + 1, '=');
  if (next_eq_loc != NULL && next_eq_loc < stop)
  {
    return mult_equals_msg;
  }
  while (start != NULL && stop != NULL)
  {
    next_start = StringChr (start + 1, '[');
    next_stop = StringChr (stop + 1, ']');
    if (next_start == NULL && next_stop == NULL) return FALSE;

    if ((next_start != NULL && next_stop == NULL)
     || (next_start == NULL && next_stop != NULL)
     || (next_start > next_stop)
     || (next_start < stop))
    {
  	  return bracket_mismatch_msg;
    }
    eq_loc = StringChr (next_start + 1, '=');
    if (eq_loc == NULL || eq_loc > next_stop)
    {
      return missing_equals_msg;
    }
    next_eq_loc = StringChr (eq_loc + 1, '=');
    if (next_eq_loc != NULL && next_eq_loc < next_stop)
    {
      return mult_equals_msg;
    }
    
    start = next_start;
    stop = next_stop;    
  }
  return NULL;
}

typedef struct fixbadlineform 
{
  WindoW  w;
  DoC     prompt_doc;
  TexT    new_line;	
  CharPtr new_text;
  Int4    line_num;
  Boolean done;
  Boolean cancelled;
  CharPtr orig_txt;
  CharPtr id_str;
} FixBadLineFormData, PNTR FixBadLineFormPtr;

static void SetBadLineFormPrompt (FixBadLineFormPtr fp, CharPtr msg)
{
  CharPtr str;
  if (fp == NULL || msg == NULL) return;
  
  str = MakeBracketingErrorMessage (msg, fp->line_num, fp->id_str);
  Reset (fp->prompt_doc);
  AppendText (fp->prompt_doc, str, NULL, NULL, programFont);
  MemFree (str);
}

static void FixBadLineOk (ButtoN b)
{
  FixBadLineFormPtr fp;
  CharPtr           msg;
  
  fp = (FixBadLineFormPtr) GetObjectExtra (b);
  if (fp == NULL) return;
  fp->new_text = MemFree (fp->new_text);
  fp->new_text = SaveStringFromText (fp->new_line);
  msg = DetectBadBracketing (fp->new_text);
  if (msg != NULL)
  {
    if (ANS_NO == Message (MSG_YN, "You have not corrected all of the "
              "bracketing errors - are you sure you want to continue?"))
    {
  	  SetBadLineFormPrompt (fp, msg);
  	  return;
    }
  }
  Remove (fp->w);
  fp->cancelled = FALSE;
  fp->done = TRUE;
}

static void FixBadLineCancel (ButtoN b)
{
  FixBadLineFormPtr fp;
  
  fp = (FixBadLineFormPtr) GetObjectExtra (b);
  if (fp == NULL) return;
  fp->new_text = MemFree (fp->new_text);
  fp->cancelled = TRUE;
  
  Remove (fp->w);
  fp->done = TRUE;
}

static void SuggestBracketFix (ButtoN b)
{
  FixBadLineFormPtr fp;
  
  fp = (FixBadLineFormPtr) GetObjectExtra (b);
  if (fp == NULL) return;
  fp->new_text = MemFree (fp->new_text);
  fp->new_text = SaveStringFromText (fp->new_line);
  SetTitle (fp->new_line, SuggestCorrectBracketing(fp->new_text));
  return;
}

static void ResetBracketFixText (ButtoN b)
{
  FixBadLineFormPtr fp;
  
  fp = (FixBadLineFormPtr) GetObjectExtra (b);
  if (fp == NULL) return;
  SetTitle (fp->new_line, fp->orig_txt);
  return;
}


static CharPtr FixBadBracketing (CharPtr bad_line, CharPtr msg, Int4 line_num, CharPtr id_str, BoolPtr cancelled)
{
  GrouP  g, c, d;
  ButtoN b;
  FixBadLineFormData fd;
  Int4               len;
  
  fd.w = MovableModalWindow(-20, -13, -10, -10, "Correct Bad Bracketing", NULL);
  g = HiddenGroup(fd.w, -1, 0, NULL);
  len = StringLen (bad_line) + 5;
  fd.orig_txt = bad_line;
  fd.line_num = line_num;
  fd.id_str = id_str;
  fd.new_text = NULL;
  fd.prompt_doc = DocumentPanel (g, stdCharWidth * 30, stdLineHeight * 3);
  SetBadLineFormPrompt (&fd, msg);
  fd.new_line = DialogText (g, bad_line, 30, NULL);
  d = HiddenGroup (g, 2, 0, NULL);
  b = PushButton (d, "Suggest Correction", SuggestBracketFix);
  SetObjectExtra (b, &fd, NULL);
  b = PushButton (d, "Reset to original text", ResetBracketFixText);
  SetObjectExtra (b, &fd, NULL);
  c = HiddenGroup (g, 4, 0, NULL);
  b = PushButton(c, "OK", FixBadLineOk);
  SetObjectExtra (b, &fd, NULL);
  b = PushButton(c, "Cancel", FixBadLineCancel);
  SetObjectExtra (b, &fd, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) fd.prompt_doc, (HANDLE) fd.new_line, 
                  (HANDLE) d, (HANDLE) c, NULL);
  
  Show(fd.w); 
  Select (fd.w);
  fd.done = FALSE;
  while (!fd.done)
  {
    ProcessExternalEvent ();
    Update ();
  }
  ProcessAnEvent ();
  if (fd.cancelled)
  {
  	if (cancelled != NULL)
  	{
      *cancelled = TRUE;
  	}
  	return NULL;
  }
  return fd.new_text;
}

static void SequenceIdHelp (IteM i)
{
  Message (MSG_OK,
        "  A sequence ID is a temporary ID which will be replaced with a unique"
        " GenBank accession number by the GenBank curators.  The sequence ID should"
        " be unique for each sequence in a record.  It could represent a clone,"
        " isolate, a laboratory designation for your specimen, or some other useful"
        " information, but this is not required."
        "  A sequence ID may not begin with a '>' character or contain spaces.");
}

static void OrganismNameHelp (IteM i)
{
  Message (MSG_OK, "You must enter a scientific name for your organism. "
           "The name does not need to be present in the list in the dialog.");
}

static void OrganismQualifiersHelp (IteM i)
{
  Message (MSG_OK, "If you have multiple organisms with the same scientific name, "
           "please use modifiers to distinguish the organisms from one another.  "
           "Strain, clone, isolate, and specimen voucher are modifiers frequently "
           "used for this purpose, but you may select any applicable modifiers.");
}

static void SequenceCharactersHelp (IteM i)
{
  Message (MSG_OK, "Please enter the nucleotides for your sequence into the  "
                   "sequence characters area.  You may only use the valid "
                   "IUPAC characters (%s).  You may not use *, -, ., or any other "
                   "alignment characters or punctuation in your sequence.  If you "
                   "are trying to import an alignment, you should use the Cancel "
                   "button to exit this dialog, then hit the Prev Page button and then "
                   "the Prev Form button to get to the Sequence Format dialog, and "
                   "select 'Alignment' instead of FASTA for the sequence data format.  "
                   "You will need to have a file prepared for import.", 
                    valid_iupac_characters);
}


static void ClearSequencesButton (ButtoN b)
{
  SequencesFormPtr   sqfp;
  
  sqfp = (SequencesFormPtr) GetObjectExtra (b);
  if (sqfp == NULL) return;
  SequencesFormDeleteProc (sqfp);
}

static void 
SetModifierList
(DoC doc,
 ValNodePtr mod_list)
{
  ValNodePtr vnp;
  Int4       num_modifiers = 0;
  Int4       text_len = 0;
  CharPtr    text;
  CharPtr    text_fmt = "Already have values for:\n";
    
  if (doc == NULL)
  {
    return;
  }
  Reset (doc);
  if (mod_list == NULL) 
  {
    text = StringSave ("No modifiers are present.");
  }
  else
  {
    text_len = StringLen (text_fmt) + 1;
    for (vnp = mod_list; vnp != NULL; vnp = vnp->next)
    {
      num_modifiers ++;
      text_len += StringLen (vnp->data.ptrvalue);
    }
    text_len += num_modifiers * 6;
    text = (CharPtr) MemNew (text_len * sizeof (Char));
    if (text != NULL)
    {
      StringCpy (text, text_fmt);
      for (vnp = mod_list; vnp != NULL; vnp = vnp->next)
      {
        if (vnp->next == NULL && num_modifiers > 1)
        {
          StringCat (text, "and ");
        }
      	StringCat (text, vnp->data.ptrvalue);
      	if (vnp->next != NULL)
      	{
      	  if (num_modifiers > 2)
      	  {
      	  	StringCat (text, ", ");
      	  }
      	  else
      	  {
      	  	StringCat (text, " ");
      	  }
      	}
      }
    }
  }
  AppendText (doc, text, NULL, NULL, programFont);
  InvalDocRows (doc, 0, 0, 0);
  MemFree (text);  
}


static SeqEntryPtr GetSeqEntryFromSequencesForm (SequencesFormPtr sqfp)
{
  SeqEntryPtr list = NULL;
  FastaPagePtr       fpp;
  PhylipPagePtr      ppp;
  SeqEntryPtr        sep;
  BioseqSetPtr       bssp;
  
  if (sqfp == NULL) return NULL;

  if (sqfp->seqPackage == SEQ_PKG_SEGMENTED) 
  {
    fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
    if (fpp != NULL) 
    {
      list = fpp->list;
    }
    if (list != NULL && IS_Bioseq_set (list))
    {
      list = FindNucSeqEntry (list);
    }
  }
  else if (sqfp->seqFormat == SEQ_FMT_FASTA) {
    fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
    if (fpp != NULL) 
    {
      list = fpp->list;
    }
  } else if (sqfp->seqFormat == SEQ_FMT_ALIGNMENT) {
    ppp = (PhylipPagePtr) GetObjectExtra (sqfp->dnaseq);
    if (ppp != NULL) {
      sep = ppp->sep;
      if (sep != NULL && IS_Bioseq_set (sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        if (bssp != NULL) {
          list = bssp->seq_set;
        }
      }
    }
  }
  return list;
}

static Boolean HasNoMoreThanOneOrganismName (SeqEntryPtr seq_list, CharPtr PNTR org_name)
{
  SeqEntryPtr  sep;
  CharPtr      ttl;
  CharPtr      title;
  Char         text [128];
  Char         last_org [128];
  Char         lookfor [128];
  Char         lookfor2 [128];
  Boolean      found_org;
  Boolean      found_diff_org = FALSE;

  MakeSearchStringFromAlist (lookfor, "Org");
  MakeSearchStringFromAlist (lookfor2, "Organism");
  last_org [0] = 0;

  for (sep = seq_list; sep != NULL && !found_diff_org; sep = sep->next)
  {
    ttl = NULL;
    found_org = TRUE;
    SeqEntryExplore (sep, (Pointer) (&ttl), FindFirstTitle);
    title = StringSaveNoNull (ttl);
    text [0] = '\0';
    if (! LookForSearchString (title, lookfor, text, sizeof (text) - 1)) 
    {
      if (! LookForSearchString (title, lookfor2, text, sizeof (text) - 1))
      {
        found_org = FALSE;
      }
    }
    if (found_org)
    {
      if (last_org [0] == 0)
      {
        StringCpy (last_org, text);
      }
      else if (StringCmp (last_org, text) != 0)
      {
        found_diff_org = TRUE;
      }
    }
  }
  if (found_diff_org)
  {
    return FALSE;
  }
  else
  {
    if (org_name != NULL)
    {
      if (last_org [0] == 0)
      {
        *org_name = 0;
      }
      else
      {
        *org_name = StringSave (last_org);
      }
    }
    return TRUE;
  }
}


extern Boolean IsNonTextModifier (CharPtr mod_name)
{
  if (StringICmp (mod_name, "transgenic") == 0
      || StringICmp (mod_name, "germline") == 0
      || StringICmp (mod_name, "environmental-sample") ==0
      || StringICmp (mod_name, "rearranged") == 0)
  {
    return TRUE;  
  }
  else
  {
    return FALSE;
  }
}

static Int4 GetValForEnumName (EnumFieldAssocPtr eap, CharPtr mod_value)
{
  if (StringHasNoText (mod_value) || eap == NULL)
  {
    return 0;
  }
  while (eap != NULL && eap->name != NULL) 
  {
    if (StringICmp (eap->name, mod_value) == 0)
    {
      return eap->value;
    }
    eap++;
  }
  return 0;
}

static void AddSeqIDAndValueToTagList 
(CharPtr id,
 CharPtr title,
 CharPtr lookfor, 
 CharPtr mod_name,
 Int2    seqPackage,
 ValNodePtr PNTR   head,
 ValNodePtr PNTR   found_modifiers)
{
  Char        text [128];
  CharPtr     str;
  Int4        len;
  Int4        mod_type;
  Boolean     is_nontext;

  if (head == NULL)
  {
    return;
  }

  if (found_modifiers != NULL)
  {
    *found_modifiers = BuildModifierTypeList (*found_modifiers, title);
  }
  
  is_nontext = IsNonTextModifier (mod_name);
  mod_type = GetModifierType (mod_name);

  text [0] = '\0';
  if (is_nontext)
  {
    if (LookForSearchString (title, lookfor, text, sizeof (text) - 1)) 
    {
      StringCpy (text, "1");
    }
    else
    {
      StringCpy (text, "0");
    }
  }
  else if (mod_type == eModifierType_Organism)
  {
    if (! LookForSearchString (title, (CharPtr) short_org, text, sizeof (text) - 1)) 
    {
      if (! LookForSearchString (title, (CharPtr) long_org, text, sizeof (text) - 1))
      {
        StringCpy (text, " ");
      }
    }
  }
  else if (mod_type == eModifierType_Location)
  {
    if (LookForSearchString (title, lookfor, text, sizeof (text) - 1)) 
    {
      sprintf (text, "%d", GetValForEnumName (biosource_genome_simple_alist, text));
    }
    else
    {
      StringCpy (text, "1");
    }
  }
  else if (mod_type == eModifierType_NucGeneticCode
           || mod_type == eModifierType_MitoGeneticCode)
  {
    if (LookForSearchString (title, lookfor, text, sizeof (text) - 1)) 
    {
      sprintf (text, "%d", GeneticCodeFromString (text));
    }
    else
    {
      StringCpy (text, "0");
    }
  }
  else if (mod_type == eModifierType_MolType)
  {
    if (LookForSearchString (title, lookfor, text, sizeof (text) - 1)) 
    {
      sprintf (text, "%d", MolTypeFromString (text));
    }
    else
    {
      if (PackageTypeIsSet (seqPackage) || seqPackage == SEQ_PKG_GENOMICCDNA) 
      {
        sprintf (text, "%d", 253);
      }
      else
      {
        sprintf (text, "%d", 254);
      }
    }
  }
  else if (mod_type == eModifierType_Topology)
  {
    if (LookForSearchString (title, lookfor, text, sizeof (text) - 1)) 
    {
      sprintf (text, "%d", TopologyFromString (text));
    }
    else
    {
      sprintf (text, "%d", TopologyFromString (""));
    }
  }
  else
  {
    if (! LookForSearchString (title, lookfor, text, sizeof (text) - 1)) {
      StringCpy (text, " ");
    }
  }
  len = StringLen (id) + StringLen (text);
  str = MemNew (len + 4);
  if (str != NULL) {
    StringCpy (str, id);
    StringCat (str, "\t");
    StringCat (str, text);
    StringCat (str, "\n");
  }
  
  ValNodeAddPointer (head, 0, str);
  
  
}

static void AddSeqIDAndValueFromSeqEntryToTagList 
(SeqEntryPtr     sep,
 CharPtr         lookfor, 
 CharPtr         mod_name,
 Int2            seqPackage,
 ValNodePtr PNTR head,
 ValNodePtr PNTR found_modifiers)
{
  BioseqPtr   bsp;
  SeqDescrPtr sdp;
  Char        id_txt [128];
  
  if (sep == NULL)
  {
    return;
  }
  if (IS_Bioseq (sep))
  {
    bsp = (BioseqPtr) sep->data.ptrvalue;
  }
  else if (IS_Bioseq_set (sep))
  {
    sep = FindNucSeqEntry (sep);
    if (sep != NULL && IS_Bioseq (sep))
    {
      bsp = (BioseqPtr) sep->data.ptrvalue;
    }
  }
  
  if (bsp == NULL)
  {
    return;
  }
  
  SeqIdWrite (SeqIdFindWorst (bsp->id), id_txt, PRINTID_REPORT, sizeof (id_txt));
  
  for (sdp = bsp->descr; sdp != NULL && sdp->choice != Seq_descr_title; sdp = sdp->next)
  {
  }

  if (sdp == NULL)
  {
    AddSeqIDAndValueToTagList (id_txt, NULL, lookfor, mod_name, seqPackage, 
                               head, found_modifiers);
  }
  else
  {
    AddSeqIDAndValueToTagList (id_txt, sdp->data.ptrvalue, lookfor, mod_name, 
                               seqPackage, head, found_modifiers);
  }  
}


static CharPtr GetValueFromTitle (CharPtr mod_name, CharPtr ttl)
{
  Char        text [128];
  Char        lookfor [128];
  Int4        mod_type;
  Boolean     is_nontext;
  
  if (StringHasNoText (mod_name) || StringHasNoText (ttl))
  {
    return NULL;
  }
  mod_type = GetModifierType (mod_name);
  is_nontext = IsNonTextModifier (mod_name);
  
  text [0] = '\0';
  if (mod_type == eModifierType_Organism)
  {
    if (! LookForSearchString (ttl, (CharPtr) short_org, text, sizeof (text) - 1)) 
    {
      if (! LookForSearchString (ttl, (CharPtr) long_org, text, sizeof (text) - 1))
      {
        StringCpy (text, " ");
      }
    }
  }
  else if (mod_type == eModifierType_Location)
  {
    if (! LookForSearchString (ttl, "[location=", text, sizeof (text) - 1)) 
    {
      StringCpy (text, "genomic");
    }
  }
  else if (IsNonTextModifier (mod_name))
  {
    MakeSearchStringFromAlist (lookfor, mod_name);
    if (LookForSearchString (ttl, lookfor, text, sizeof (text) - 1)) 
    {
      StringCpy (text, "TRUE");
    }
    else
    {
      StringCpy (text, "FALSE");
    }
  }
  else
  {
    MakeSearchStringFromAlist (lookfor, mod_name);
    if (! LookForSearchString (ttl, lookfor, text, sizeof (text) - 1)) {
      StringCpy (text, " ");
    }
  }      
  return StringSave (text);  
}

static Boolean IntValueInValNodeList (Int4 ival, ValNodePtr vnp)
{
  Boolean found_int = FALSE;
  
  while (vnp != NULL && !found_int)
  {
    if (vnp->data.intvalue == ival)
    {
      found_int = TRUE;
    }
    vnp = vnp->next;
  }
  
  return found_int;
}

static Boolean 
DoColumnlistsHaveIdenticalSourceInformation 
(ValNodePtr col1,
 ValNodePtr col2,
 ValNodePtr header)
{
  Boolean are_identical = TRUE;
  Int4    mod_type;
  
  while (col1 != NULL && col2 != NULL && header != NULL && are_identical)
  { 
    mod_type = GetModifierType (header->data.ptrvalue);
    if (mod_type != eModifierType_Other
        && mod_type != eModifierType_MolType
        && mod_type != eModifierType_Topology
        && StringCmp ((CharPtr) col1->data.ptrvalue, (CharPtr) col2->data.ptrvalue) != 0)
    {
      are_identical = FALSE;
    }
    col1 = col1->next;
    col2 = col2->next;
    header = header->next;
  }
  if ((col1 == NULL && col2 != NULL) || (col1 != NULL && col2 == NULL))
  {
    are_identical = FALSE;
  }
  return are_identical;
}

static Boolean HasAnySourceInformation (ValNodePtr header_list, ValNodePtr column_list)
{
  Boolean has_any = FALSE;
  Int4    mod_type;
  
  if (header_list == NULL || column_list == NULL)
  {
    return FALSE;
  }
  
  /* skip over SeqID column */
  header_list = header_list->next;
  column_list = column_list->next;
  while (header_list != NULL && column_list != NULL && ! has_any)
  {
    if (!StringHasNoText (column_list->data.ptrvalue))
    {
      mod_type = GetModifierType (header_list->data.ptrvalue);
      if (mod_type != eModifierType_Other
          && mod_type != eModifierType_MolType
          && mod_type != eModifierType_Topology)
      {
        has_any = TRUE;
      }
    }
    header_list = header_list->next;
    column_list = column_list->next;
  }
  return has_any;
}

static void ReportIdenticalOrganisms (ValNodePtr row_list, DoC doc)
{
  ValNodePtr checked_list = NULL;
  Int4       row_num, check_row_num;
  ValNodePtr row_vnp, check_row_vnp;
  ValNodePtr column_list, check_column_list;
  ValNodePtr header_vnp;
  Boolean    skip_this;
  ValNodePtr this_match_list;
  CharPtr    err_msg;
  Boolean    any_data_reported = FALSE;
  
  if (row_list == NULL || doc == NULL) return;
  
  /* we start with the header of the second column, because the first column
   * is the sequence ID */
  header_vnp = row_list->data.ptrvalue;
  if (header_vnp == NULL) return;
  header_vnp = header_vnp->next;
  if (header_vnp == NULL) return;
    
  for (row_vnp = row_list->next, row_num = 0;
       row_vnp != NULL; 
       row_vnp = row_vnp->next, row_num++)
  {
    /* don't need to check rows that have matched a previous row */
    skip_this = IntValueInValNodeList (row_num, checked_list);
    if (skip_this)
    {
      continue;
    }
    
    column_list = (ValNodePtr) row_vnp->data.ptrvalue;
    if (!HasAnySourceInformation (row_list->data.ptrvalue, column_list))
    {
      continue;
    }

    if (column_list == NULL || column_list->next == NULL)
    {
      continue;
    }
    this_match_list = NULL;
    for (check_row_vnp = row_vnp->next, check_row_num = row_num + 1;
         check_row_vnp != NULL;
         check_row_vnp = check_row_vnp->next, check_row_num++)
    {
      skip_this = IntValueInValNodeList (row_num, checked_list);
      if (skip_this)
      {
        continue;
      }
      check_column_list = (ValNodePtr) check_row_vnp->data.ptrvalue;
      if (check_column_list == NULL || check_column_list->next == NULL)
      {
        continue;
      }

      /* we compare the column lists, starting with the second column
       * because the first column contains the sequence ID
       */
      if (DoColumnlistsHaveIdenticalSourceInformation (column_list->next,
                                                       check_column_list->next,
                                                       header_vnp))
      {
        /* be sure to put the first row to match the other rows in the list */
        if (this_match_list == NULL)
        {
          ValNodeAddPointer (&this_match_list, row_num, column_list->data.ptrvalue);          
        }
        /* add the sequence ID for the check row to the list */
        ValNodeAddPointer (&this_match_list, check_row_num, check_column_list->data.ptrvalue);   
        ValNodeAddInt (&checked_list, 0, check_row_num);       
      }
    }
    
    /* if anything matched this row, put the list in the list of matches */
    if (this_match_list != NULL)
    {
      err_msg = CreateListMessage ("Sequence", 
                     " have identical source information.",
                     this_match_list);
      AppendText (doc, err_msg, &faParFmt, &faColFmt, programFont);
      err_msg = MemFree (err_msg);
      this_match_list = ValNodeFree (this_match_list);
      any_data_reported = TRUE;
    }
  }
  
  checked_list = ValNodeFree (checked_list);
  if (any_data_reported)
  {
    AppendText (doc, "\n", &faParFmt, &faColFmt, programFont);
  }
}

static void SummarizeModifiers (ValNodePtr row_list, DoC summary_doc)
{
  ValNodePtr header_vnp, row_vnp, column_vnp;
  Int4       column_offset, col_pos;
  Boolean    any_present;
  Boolean    all_present;
  Boolean    is_unique;
  CharPtr    first_value_seen;
  Boolean    all_unique;
  ValNodePtr values_seen;
  CharPtr    status;
  Int4       line_len;
  CharPtr    modifier_line = NULL;
  Int4       num_missing;
  
  if (row_list == NULL || row_list->next == NULL || summary_doc == NULL)
  {
    return;
  }
  
  header_vnp = row_list->data.ptrvalue;
  /* skip over sequence ID column */
  header_vnp = header_vnp->next;
  column_offset = 1;
  while (header_vnp != NULL)
  {
    any_present = FALSE;
    all_present = TRUE;
    is_unique = TRUE;
    all_unique = TRUE;
    first_value_seen = NULL;
    values_seen = NULL;
    num_missing = 0;
    
    /* skip over header line */
    row_vnp = row_list->next;
    while (row_vnp != NULL)
    {
      for (col_pos = 0, column_vnp = row_vnp->data.ptrvalue;
           col_pos < column_offset && column_vnp != NULL;
           col_pos++, column_vnp = column_vnp->next)
      {
      }
      if (column_vnp == NULL)
      {
        continue;
      }
      if (StringHasNoText (column_vnp->data.ptrvalue))
      {
        all_present = FALSE;
        num_missing++;
      }
      else
      {
        any_present = TRUE;
        if (first_value_seen == NULL)
        {
          first_value_seen = StringSave (column_vnp->data.ptrvalue);
          ValNodeAddPointer (&values_seen, 0, first_value_seen);
        }
        else 
        {
          if (StringCmp (first_value_seen, column_vnp->data.ptrvalue) != 0)
          {
            is_unique = FALSE;
          }
          
          if ( FindExactStringInStrings (values_seen, column_vnp->data.ptrvalue)
              == NULL)
          {
            ValNodeAddStr (&values_seen, 0, column_vnp->data.ptrvalue);
          }
          else
          {
            all_unique = FALSE;
          }
        }
      }
      row_vnp = row_vnp->next;
    }
    
    /* add summary line for this modifier */
    if (! any_present)
    {
      status = "%s: All missing (%d sequences). ";
    }
    else if (all_present && all_unique)
    {
      status = "%s: All present, all unique values. ";
    }
    else if (all_present && is_unique)
    {
      status = "%s: All present, one unique value. ";
    }
    else if (all_present && ! is_unique)
    {
      status = "%s: All present, mixed values. ";
    }
    else if (! all_present && all_unique)
    {
      status = "%s: %d missing, all unique values. ";
    }
    else if (! all_present && is_unique)
    {
      status = "%s: %d missing, one unique value present. ";
    }
    else if (! all_present && ! is_unique)
    {
      status = "%s: %d missing, mixed values. ";
    }
    
    line_len = StringLen (header_vnp->data.ptrvalue)
               + StringLen (status)
               + StringLen (first_value_seen)
               + 30;
               
    modifier_line = (CharPtr) MemNew (line_len * sizeof (Char));
    if (modifier_line != NULL)
    {
      if (all_present)
      {
        sprintf (modifier_line, status, header_vnp->data.ptrvalue);
      }
      else
      {
        sprintf (modifier_line, status, header_vnp->data.ptrvalue, num_missing);
      }
      
      if (!StringHasNoText (first_value_seen))
      {
        StringCat (modifier_line, "(");
        StringCat (modifier_line, first_value_seen);
        StringCat (modifier_line, ")");
      }
      AppendText (summary_doc, modifier_line, NULL, NULL, programFont);
      modifier_line = MemFree (modifier_line);
    }
        
    /* free up variables */
    first_value_seen = MemFree (first_value_seen);
    values_seen = ValNodeFree (values_seen);
    
    header_vnp = header_vnp->next;
    column_offset++;    
  }  
}

static void SeqEntryPtrToOrgDoc (SequencesFormPtr sqfp)
{
  SeqEntryPtr  seq_list, sep;
  ValNodePtr   found_modifiers = NULL, vnp;
  CharPtr      ttl;
  CharPtr      mod_name;
  CharPtr      org_name;
  BioseqPtr    bsp;
  SeqIdPtr     sip;
  Int4         seq_num;
  Char         tmp [128];
  ValNodePtr   column_list = NULL;
  ValNodePtr   row_list = NULL, row_vnp;
  ValNodePtr   header_vnp, header_list;
  Int4         column_width;
  CharPtr      mod_value;
  RecT         r;
  
  if (sqfp == NULL) return;
  Reset (sqfp->org_doc);
  Reset (sqfp->summary_doc);
  ObjectRect (sqfp->org_doc, &r);
  InsetRect (&r, 4, 4);
  faColFmt.pixWidth = r.right - r.left;

  seq_list = GetSeqEntryFromSequencesForm (sqfp);
  if (seq_list == NULL)
  {
    AppendText (sqfp->org_doc, 
                "You must create sequences before you can add source information.",
                &faParFmt, &faColFmt, programFont);
  }
  else
  {
    /* get list of modifiers */    
    for (sep = seq_list, seq_num = 0; sep != NULL; sep = sep->next, seq_num++)
    {
      ttl = NULL;
      SeqEntryExplore (sep, (Pointer) (&ttl), FindFirstTitle);
      found_modifiers = BuildModifierTypeList (found_modifiers, ttl);
    }
    
    /* create header line for table */
    /* store max column width in choice */
    column_list = NULL;
    ValNodeAddPointer (&column_list, 6, StringSave ("Seq ID"));
    ValNodeAddPointer (&column_list, 8, StringSave ("Organism"));
    for (vnp = found_modifiers; vnp != NULL; vnp = vnp->next)
    {
      ValNodeAddPointer (&column_list, StringLen (vnp->data.ptrvalue), StringSave ((CharPtr) vnp->data.ptrvalue));
    } 
    
    ValNodeAddPointer (&row_list, 0, column_list);
    header_list = column_list;
    
    /* create data lines for table */
    for (sep = seq_list, seq_num = 0; sep != NULL; sep = sep->next, seq_num++)
    {
      column_list = NULL;
      header_vnp = header_list;
      /* add Sequence ID */
      bsp = FindNucBioseq (sep);
      if (bsp != NULL)
      {
        sip = SeqIdFindWorst (bsp->id);
        SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
      }
      else
      {
        sprintf (tmp, "%d", seq_num);
      }
      column_width = MAX (StringLen (tmp), header_vnp->choice);
      header_vnp->choice = column_width;
      ValNodeAddPointer (&column_list, 0, StringSave (tmp));
      
      /* get title */
      ttl = NULL;
      SeqEntryExplore (sep, (Pointer) (&ttl), FindFirstTitle);
      
      /* add organism name */
      header_vnp = header_vnp->next;
      org_name = GetValueFromTitle ("organism", ttl);
      column_width = MAX (StringLen (org_name), header_vnp->choice);
      header_vnp->choice = column_width;
      ValNodeAddPointer (&column_list, 0, org_name);
      
      /* get remaining modifiers */
      for (vnp = found_modifiers; vnp != NULL; vnp = vnp->next)
      {
        header_vnp = header_vnp->next;
        mod_name = (CharPtr) vnp->data.ptrvalue;
        mod_value = GetValueFromTitle (mod_name, ttl);
        column_width = MAX (StringLen (mod_value), header_vnp->choice);
        header_vnp->choice = column_width;
        ValNodeAddPointer (&column_list, 0, mod_value);
      }
      ValNodeAddPointer (&row_list, 0, column_list);
    }
    ReportIdenticalOrganisms (row_list, sqfp->org_doc);

    SummarizeModifiers (row_list, sqfp->summary_doc);

    /* free table text */
    for (row_vnp = row_list; row_vnp != NULL; row_vnp = row_vnp->next)
    {
      column_list = (ValNodePtr) row_vnp->data.ptrvalue;
      row_vnp->data.ptrvalue = ValNodeFreeData (column_list);
    }
    row_list = ValNodeFree (row_list);

    ValNodeFreeData (found_modifiers);
  }
  /* update document */
  InvalDocRows (sqfp->org_doc, 0, 0, 0);
  InvalDocRows (sqfp->summary_doc, 0, 0, 0);
}

static void SeqEntryPtrToSourceTab (SequencesFormPtr sqfp)

{
  SeqEntryPtrToOrgDoc (sqfp);
}

typedef struct stringpair
{
  CharPtr       findstr;
  CharPtr       replstr;
  Int4          replint;
  EModifierType modtype;
} StringPairData, PNTR StringPairPtr;

static ValNodePtr FreeStringPairList (ValNodePtr list)
{
  StringPairPtr spp;
  
  if (list == NULL) return NULL;
  list->next = FreeStringPairList (list->next);
  spp = (StringPairPtr) list->data.ptrvalue;
  if (spp != NULL)
  {
  	MemFree (spp->findstr);
  	MemFree (spp->replstr);
  	MemFree (spp);
  	list->data.ptrvalue = NULL;
  }
  list = ValNodeFree (list);
  return list;
}

static ValNodePtr GetFastaModifierList (void)
{
  ValNodePtr mod_choices = NULL, last_mod_choice;
  Int4       i;

  ValNodeAddPointer (&mod_choices, eModifierType_Organism, StringSave ("Organism"));
  ValNodeAddPointer (&mod_choices, eModifierType_Location, StringSave ("Location"));
  last_mod_choice = ValNodeNew (mod_choices);
  last_mod_choice->choice = eModifierType_Lineage;
  last_mod_choice->data.ptrvalue = StringSave ("Lineage");

  last_mod_choice = ValNodeNew (mod_choices);
  last_mod_choice->choice = eModifierType_NucGeneticCode;
  last_mod_choice->data.ptrvalue = StringSave ("gcode");
  
  last_mod_choice = ValNodeNew (mod_choices);
  last_mod_choice->choice = eModifierType_MitoGeneticCode;
  last_mod_choice->data.ptrvalue = StringSave ("mgcode");
  
  last_mod_choice->next = GetSourceQualDescList (TRUE, TRUE);
  for (i = 0; i < num_other_modifier_names; i++)
  {
    ValNodeAddPointer (&mod_choices, eModifierType_Other, StringSave (other_modifier_names[i]));
  }

  return mod_choices;  
}

static StringPairPtr 
GetModifierFix (ModifierInfoPtr mip, BoolPtr cancelled, BoolPtr add_to_fixes)
{
  WindoW                w;
  GrouP  g, c;
  ButtoN b;
  ModalAcceptCancelData acd;
  CharPtr               str = NULL;
  CharPtr               prompt_fmt = "%s is not a valid qualifier type.  "
                                  "Please choose a valid qualifier type:";
  WindoW                ppt;
  DialoG                rename_choice;
  ValNodePtr            vnp_new;
  StringPairPtr         spp = NULL;
  CharPtr               old_name;
  ValNodePtr            mod_choices = NULL;
  Boolean               done;
  ButtoN                add_to_fixes_btn;
  
  mod_choices = GetFastaModifierList ();
  
  w = MovableModalWindow(-20, -13, -10, -10, "Unrecognized Qualifier", NULL);
  g = HiddenGroup(w, -1, 0, NULL);
  str = MemNew ((StringLen (mip->name) + StringLen (prompt_fmt) + PRINTED_INT_MAX_LEN) * sizeof (Char));
  sprintf (str, prompt_fmt, mip->name);
  ppt = MultiLinePrompt (g, str, 27 * stdCharWidth, programFont);
  str = MemFree (str);
  rename_choice = ValNodeSelectionDialog (g, mod_choices, 6,
                                          SourceQualValNodeName,
                                          ValNodeSimpleDataFree,
                                          SourceQualValNodeDataCopy,
                                          SourceQualValNodeMatch,
                                          "modifier",
                                          NULL, NULL, FALSE);
  add_to_fixes_btn = CheckBox (g, "Replace all instances", NULL);
  c = HiddenGroup (g, 4, 0, NULL);
  b = PushButton(c, "OK", ModalAcceptButton);
  SetObjectExtra (b, &acd, NULL);
  b = PushButton(c, "Cancel", ModalCancelButton);
  SetObjectExtra (b, &acd, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) ppt,
                              (HANDLE) rename_choice,
                              (HANDLE) add_to_fixes_btn,
                              (HANDLE) c,
                              NULL);
  
  Show(w); 
  Select (w);
  done = FALSE;
  while (!done)
  {
    acd.accepted = FALSE;
    acd.cancelled = FALSE;
    while (!acd.accepted && ! acd.cancelled)
    {
      ProcessExternalEvent ();
      Update ();
    }
    ProcessAnEvent ();
    if (acd.cancelled)
    {
      done = TRUE;
      *cancelled = TRUE;
    }
    else if (acd.accepted)
    {
      vnp_new = DialogToPointer (rename_choice);
      if (vnp_new != NULL)
      {
  	    old_name = mip->name;
  	    mip->name = SourceQualValNodeName (vnp_new);
  	    if (mip->name != NULL)
  	    {
  	      mip->name [0] = TO_LOWER (mip->name [0]);
  	    }
  	    mip->modtype = GetModifierType (mip->name);
  	    if (mip->modtype == eModifierType_SourceQual)
  	    {
  	      mip->subtype = vnp_new->choice;
  	    }
  	    else
  	    {
  	      mip->subtype = 0;
  	    }
        if (add_to_fixes != NULL)
        {
          *add_to_fixes = GetStatus (add_to_fixes_btn);
        }
        if (GetStatus (add_to_fixes_btn))
        {
          spp = (StringPairPtr) MemNew (sizeof(StringPairData));
  	      if (spp != NULL)
  	      {
  	        spp->findstr = old_name;
  	        spp->replstr = StringSave (vnp_new->data.ptrvalue);
  	        spp->modtype = mip->modtype;
  	        spp->replint = mip->subtype;
  	      }
  	      else 
  	      {
  	        MemFree (old_name);
  	      }
  	      ValNodeFreeData (vnp_new);
        }
        else
        {
        	MemFree (old_name);
        }
        done = TRUE;
      }
    }
  }
  Remove (w);
  
  return spp;
}

static ValNodePtr FixUnrecognizedModifier 
(ModifierInfoPtr mip, ValNodePtr fixes, BoolPtr cancelled)
{
  ValNodePtr    vnp;
  StringPairPtr spp;
  Boolean       found = FALSE;
  Boolean       add_to_fixes = FALSE;
  
  if (mip == NULL) return fixes;
  
  /* try to find mip->name in list of automatic fixes */
  for (vnp = fixes; vnp != NULL && !found; vnp = vnp->next)
  {
  	spp = (StringPairPtr) vnp->data.ptrvalue;
  	if (spp != NULL && StringCmp (spp->findstr, mip->name) == 0)
  	{
  	  found = TRUE;
  	  mip->name = MemFree (mip->name);
  	  mip->name = StringSave (spp->replstr);
  	  mip->modtype = spp->modtype;
  	  mip->subtype = spp->replint;
  	}
  }
  if (!found)
  {
  	/* get new fix and add to list*/
    spp = GetModifierFix (mip, cancelled, &add_to_fixes);
    if (add_to_fixes && spp != NULL)
    {
      vnp = ValNodeNew (fixes);
      if (fixes == NULL)
      {
      	fixes = vnp;
      }
      if (vnp != NULL)
      {
      	vnp->data.ptrvalue = spp;
      }
    }
  }
  return fixes;
}

static void 
FixUnrecognizedModifersInOneDefinitionLine
(SeqDescrPtr     sdp,
 BoolPtr         cancelled,
 ValNodePtr PNTR fixes)
{
  ValNodePtr      mod_list, vnp;
  CharPtr         title, new_title;
  ModifierInfoPtr mip;
  Int4            title_len = 0;
  Boolean         changed = FALSE;
  
  if (sdp == NULL || sdp->choice != Seq_descr_title
      || StringHasNoText (sdp->data.ptrvalue)
      || cancelled == NULL || fixes == NULL)
  {
    return;
  }
  
  mod_list = ParseAllBracketedModifiers (sdp->data.ptrvalue);
  title = GetUnbracketedText (sdp->data.ptrvalue);
  title_len = StringLen (title) + 1;
  
  for (vnp = mod_list; vnp != NULL && !*cancelled; vnp = vnp->next)
  {
    mip = (ModifierInfoPtr)vnp->data.ptrvalue;
    if (mip == NULL) continue;
    
    if (mip->modtype == eModifierType_SourceQual)
    {
      if (mip->subtype == 255)
      {
        /* fix qualifier name */
        *fixes =  FixUnrecognizedModifier (mip, *fixes, cancelled);
        changed = TRUE;
      }
    }
    title_len += 4 + StringLen (mip->name) + StringLen (mip->value);
  }
  
  if (!*cancelled && changed)
  {
    new_title = (CharPtr) MemNew (title_len * sizeof (Char));
    if (new_title != NULL)
    {
      for (vnp = mod_list; vnp != NULL; vnp = vnp->next)
      {
        mip = (ModifierInfoPtr)vnp->data.ptrvalue;
        if (mip == NULL) continue;
        StringCat (new_title, "[");
        StringCat (new_title, mip->name);
        StringCat (new_title, "=");
        StringCat (new_title, mip->value);
        StringCat (new_title, "] ");
      }
      StringCat (new_title, title);
      sdp->data.ptrvalue = MemFree (sdp->data.ptrvalue);
      sdp->data.ptrvalue = new_title;
    }
  }  

  mod_list = ModifierInfoListFree (mod_list);
}

static void 
FixBracketingInOneDefinitionLine 
(SeqDescrPtr sdp,
 BoolPtr     cancelled, 
 CharPtr     id_str)
{
  CharPtr title, new_title;
  CharPtr msg;
  
  if (sdp == NULL || sdp->choice != Seq_descr_title)
  {
    return;
  }
  
  title = (CharPtr) sdp->data.ptrvalue;
  
  msg = DetectBadBracketing (title);
  if (msg != NULL)
  {
  	new_title = FixBadBracketing (title, msg, 0, id_str, cancelled);
  	if (*cancelled)
  	{
  	  MemFree (new_title);
  	  return;
  	}
  	MemFree (sdp->data.ptrvalue);
  	sdp->data.ptrvalue = new_title;
  }
}

static void FixDefinitionLines (SeqEntryPtr sep, BoolPtr cancelled, ValNodePtr PNTR fixes)
{
  BioseqSetPtr bssp;
  BioseqPtr    bsp;
  SeqIdPtr     sip;
  Char         tmp [128];
  SeqDescrPtr  sdp;
  Boolean      local_cancelled = FALSE;

  if (sep == NULL) return;
  
  if (cancelled == NULL)
  {
    cancelled = &local_cancelled;
  }
  else if (*cancelled)
  {
    return;
  }
  
  if (IS_Bioseq_set (sep))
  {
    sdp = SeqEntryGetSeqDescr(sep, Seq_descr_title, NULL);
    FixBracketingInOneDefinitionLine (sdp, cancelled, (CharPtr) "Bioseq Set Title");
    FixUnrecognizedModifersInOneDefinitionLine (sdp, cancelled, fixes);

    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL)
    {
      FixDefinitionLines (bssp->seq_set, cancelled, fixes);
    }
  }
  else
  {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp != NULL)
    {
      sip = SeqIdFindWorst (bsp->id);
      SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
      sdp = SeqEntryGetSeqDescr(sep, Seq_descr_title, NULL);
      FixBracketingInOneDefinitionLine (sdp, cancelled, (CharPtr) tmp);
      FixUnrecognizedModifersInOneDefinitionLine (sdp, cancelled, fixes);
    }
  }
  
  FixDefinitionLines (sep->next, cancelled, fixes);
}

static CharPtr ReplaceOrgWithOrganismInOneDefinitionLine (CharPtr title)
{
  Char         lookfor [128];
  Char         text [128];
  CharPtr      new_title = NULL;
  
  if (StringHasNoText (title)) 
  {
    return title;
  }
    
  MakeSearchStringFromAlist (lookfor, "Org");

  if (LookForSearchString (title, lookfor, text, sizeof (text) - 1)) 
  {
    ExciseString (title, "[org=", "]");
    TrimSpacesAroundString (title);
    new_title = (CharPtr) MemNew (StringLen (title) + StringLen (long_org) + StringLen (text) + 3);
    if (new_title != NULL)
    {
      new_title [0] = 0;
      StringCat (new_title, title);
      StringCat (new_title, " ");
      StringCat (new_title, long_org);
      StringCat (new_title, text);
      StringCat (new_title, "]");
      title = MemFree (title);
      title = new_title;
    }
  }
  return title;
}


static void ReplaceOrgWithOrganismInDefinitionLines (SeqEntryPtr sep)
{
  BioseqSetPtr bssp;
  SeqDescrPtr  sdp;

  if (sep == NULL) return;
    
  if (IS_Bioseq_set (sep))
  {
    sdp = SeqEntryGetSeqDescr(sep, Seq_descr_title, NULL);
    if (sdp != NULL)
    {
      sdp->data.ptrvalue = ReplaceOrgWithOrganismInOneDefinitionLine (sdp->data.ptrvalue);
    }

    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL)
    {
      ReplaceOrgWithOrganismInDefinitionLines (bssp->seq_set);
    }
  }
  else
  {
    sdp = SeqEntryGetSeqDescr(sep, Seq_descr_title, NULL);
    if (sdp != NULL)
    {
      sdp->data.ptrvalue = ReplaceOrgWithOrganismInOneDefinitionLine (sdp->data.ptrvalue);
    }
  }
  
  ReplaceOrgWithOrganismInDefinitionLines (sep->next);  
}

static CharPtr ReplaceMolNameWithMolBracketsInOneDefinitionLine (CharPtr title)
{
  CharPtr      ptr;
  
  if (StringHasNoText (title)) 
  {
    return title;
  }
   
  ptr = StringISearch (title, "[dna]"); 
  if (ptr != NULL)
  {
    ExciseString (title, "[dna", "]");
    TrimSpacesAroundString (title);
    title = ReplaceValueInOneDefLine (title, "molecule", "dna");
  }
  
  ptr = StringISearch (title, "[rna]");
  if (ptr != NULL)
  {
    ExciseString (title, "[rna", "]");
    TrimSpacesAroundString (title);
    title = ReplaceValueInOneDefLine (title, "molecule", "rna");
  }
  
  return title;
}

static void ReplaceMolNamesWithMolBracketsInDefinitionLines (SeqEntryPtr sep)
{
  BioseqSetPtr bssp;
  SeqDescrPtr  sdp;

  if (sep == NULL) return;
    
  if (IS_Bioseq_set (sep))
  {
    sdp = SeqEntryGetSeqDescr(sep, Seq_descr_title, NULL);
    if (sdp != NULL)
    {
      sdp->data.ptrvalue = ReplaceMolNameWithMolBracketsInOneDefinitionLine (sdp->data.ptrvalue);
    }

    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL)
    {
      ReplaceMolNamesWithMolBracketsInDefinitionLines (bssp->seq_set);
    }
  }
  else
  {
    sdp = SeqEntryGetSeqDescr(sep, Seq_descr_title, NULL);
    if (sdp != NULL)
    {
      sdp->data.ptrvalue = ReplaceMolNameWithMolBracketsInOneDefinitionLine (sdp->data.ptrvalue);
    }
  }
  
  ReplaceMolNamesWithMolBracketsInDefinitionLines (sep->next);  
}

static void ImportModifiersButtonProc (ButtoN b)
{
  SequencesFormPtr sqfp;
  Boolean          rval;
  
  sqfp = (SequencesFormPtr) GetObjectExtra (b);
  if (sqfp == NULL) return;
  
  rval = ImportOrganismModifiersForSubmit (GetSeqEntryFromSequencesForm (sqfp));
  if (rval)
  {
    SeqEntryPtrToSourceTab (sqfp);
  }
}


typedef struct sourceassistant 
{
  CharPtr PNTR    defline_list;
  CharPtr PNTR    id_list;
  Int4            num_deflines;
  Int2            seqPackage;
  PopuP           modType;
  DoC             mod_doc;
  DialoG          orgmod_dlg;
  Boolean         done;
  Boolean         cancelled;
} SourceAssistantData, PNTR SourceAssistantPtr;


static ValNodePtr PrepareSourceAssistantTableData (SourceAssistantPtr sap)
{
  Int4               i;
  ValNodePtr         found_modifiers = NULL;
  ValNodePtr         vnp;
  ValNodePtr         column_list = NULL, row_list = NULL;
  ValNodePtr         header_list, header_vnp;
  Int4               column_width, num_columns = 0;
  CharPtr            org_name, mod_name, mod_value;
  Int4               max_column_width = 20;
  
  if (sap == NULL)
  {
    return NULL;
  }
  
  /* get list of modifiers */   
  /* location will be listed whether present or not */
  ValNodeAddPointer (&found_modifiers, 0, StringSave ("location")); 
  for (i = 0; i < sap->num_deflines; i++)
  {
    found_modifiers = BuildModifierTypeList (found_modifiers, sap->defline_list[i]);
  }
    
  /* create header line for table */
  /* store max column width in choice */
  ValNodeAddPointer (&column_list, 6, StringSave ("Seq ID"));
  ValNodeAddPointer (&column_list, 8, StringSave ("organism"));
  for (vnp = found_modifiers; vnp != NULL; vnp = vnp->next)
  {
    ValNodeAddPointer (&column_list, StringLen (vnp->data.ptrvalue), StringSave ((CharPtr) vnp->data.ptrvalue));
  } 
    
  ValNodeAddPointer (&row_list, 0, column_list);
  header_list = column_list;
  
  num_columns = ValNodeLen (column_list);

  /* create data lines for table */
  for (i = 0; i < sap->num_deflines; i++)
  {
    column_list = NULL;
    header_vnp = header_list;
    /* add Sequence ID */
    column_width = MAX (StringLen (sap->id_list[i]), header_vnp->choice);
    column_width = MIN (column_width, max_column_width);
    header_vnp->choice = column_width;
    ValNodeAddPointer (&column_list, 0, StringSave (sap->id_list[i]));
      
    /* add organism name */
    header_vnp = header_vnp->next;
    org_name = GetValueFromTitle ("organism", sap->defline_list[i]);
    column_width = MAX (StringLen (org_name), header_vnp->choice);
    column_width = MIN (column_width, max_column_width);
    header_vnp->choice = column_width;
    ValNodeAddPointer (&column_list, 0, org_name);
            
    /* get remaining modifiers */
    for (vnp = found_modifiers; vnp != NULL; vnp = vnp->next)
    {
      header_vnp = header_vnp->next;
      mod_name = (CharPtr) vnp->data.ptrvalue;
      mod_value = GetValueFromTitle (mod_name, sap->defline_list[i]);
      if (StringICmp (mod_name, "location") == 0 && StringHasNoText (mod_value))
      {
        /* display default value for location */
        mod_value = MemFree (mod_value);
        mod_value = StringSave ("genomic");
      }
      column_width = MAX (StringLen (mod_value), header_vnp->choice);
      column_width = MIN (column_width, max_column_width);
      header_vnp->choice = column_width;
      ValNodeAddPointer (&column_list, 0, mod_value);
    }
    ValNodeAddPointer (&row_list, 0, column_list);
  }
  ValNodeFreeData (found_modifiers);
  return row_list;
}


/* code for scientific name selection controls */
static ValNodePtr orglist = NULL;

typedef struct orginfo 
{
  CharPtr taxname;
  Int4    ngcode;
  Int4    mgcode;
} OrgInfoData, PNTR OrgInfoPtr;

static void LoadOrganismList (void)
{
  Char              str [PATH_MAX];
  ReadBufferData    rbd;
  CharPtr           line;
  CharPtr           ptr, ptr2, ptr3;
  FILE              *f;
  OrgInfoPtr        oip;

  if (orglist != NULL) return;
  ProgramPath (str, sizeof (str));
  ptr = StringRChr (str, DIRDELIMCHR);
  if (ptr != NULL) {
    *ptr = '\0';
    FileBuildPath (str, NULL, "taxlist.txt");
    f = FileOpen (str, "r");
    if (f == NULL) {
      if (GetAppParam ("NCBI", "NCBI", "DATA", "", str, sizeof (str))) {
        FileBuildPath (str, NULL, "taxlist.txt");
        f = FileOpen (str, "r");
      }
    }
    if (f != NULL) {
      rbd.fp = f;
      rbd.current_data = NULL;
      line = AbstractReadFunction (&rbd);
      line = AbstractReadFunction (&rbd);
      while (line != NULL)
      {
        oip = (OrgInfoPtr) MemNew (sizeof (OrgInfoData));
        if (oip != NULL)
        {
          oip->taxname = line;
          
          ptr = StringChr (line, '\t');
          /* skip over common name */
          if (ptr != NULL)
          {
            *ptr = 0;
            ptr = StringChr (ptr + 1, '\t');
          }
          if (ptr != NULL)
          {
            *ptr = 0;
            ptr++;
            ptr2 = StringChr (ptr, '\t');
            if (ptr2 != NULL)
            {
              *ptr2 = 0;
              ptr2++;
              ptr3 = StringChr (ptr2, '\t');
              if (ptr3 != NULL)
              {
                *ptr3 = 0;
              }
              oip->mgcode = atoi (ptr2);
            }
            oip->ngcode = atoi (ptr);
          }
          ValNodeAddPointer (&orglist, 0, oip);
        }
      	line = AbstractReadFunction (&rbd);
      }
      FileClose (f);
    }
  }
}


typedef struct organismselectiondialog
{
  DIALOG_MESSAGE_BLOCK
  TexT       tax_name_txt;
  DoC        org_list;
  Int4       org_row;
  CharPtr    tax_name_val;
} OrganismSelectionDialogData, PNTR OrganismSelectionDialogPtr;

static void CleanupOrganismSelectionDialog (GraphiC g, VoidPtr data)

{
  OrganismSelectionDialogPtr dlg;
  
  dlg = (OrganismSelectionDialogPtr) GetObjectExtra (g);
  if (dlg != NULL)
  {
    dlg->tax_name_val = MemFree (dlg->tax_name_val);
  }

  StdCleanupExtraProc (g, data);
}

static Boolean OrgNameHighlight (DoC doc, Int2 item, Int2 row, Int2 col)
{
  OrganismSelectionDialogPtr dlg;
  
  dlg = (OrganismSelectionDialogPtr) GetObjectExtra (doc);
  if (dlg == NULL) return FALSE;
  
  if (item == dlg->org_row) return TRUE;
  return FALSE;
}

static CharPtr GetTextForOrgPos (Int4 pos)
{
  ValNodePtr vnp;
  Int4       val;
  OrgInfoPtr oip;
  
  for (vnp = orglist, val = 1; vnp != NULL && val < pos; vnp = vnp->next, val++)
  {
  }
  if (vnp != NULL && vnp->data.ptrvalue != NULL)
  {
    oip = (OrgInfoPtr) vnp->data.ptrvalue;
  	return oip->taxname;;
  }
  else
  {
  	return NULL;
  }
}

static void GetOrgPosForText (CharPtr cp, Int4Ptr pos, Boolean PNTR match)
{
  ValNodePtr vnp;
  Int4       val = 1;
  CharPtr    dat;
  Int4       res;
  OrgInfoPtr oip;
  
  if (cp == NULL || pos == NULL || match == NULL) return;
  for (vnp = orglist; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->data.ptrvalue == NULL)
    {
      continue;
    }
    oip = (OrgInfoPtr) vnp->data.ptrvalue;
  	dat = oip->taxname;
  	res = StringCmp (cp, dat);
  	if (res < 0)
  	{
  	  *pos = val;
  	  *match = FALSE;
  	  return;
  	}
  	else if (res == 0)
  	{
  	  *pos = val;
  	  *match = TRUE;
  	  return;
  	}
  	val++;
  }
  *pos = val - 1;
  *match = FALSE;
}

static void OrgNameOnKey (SlatE s, Char ch)
{
  OrganismSelectionDialogPtr dlg;
  CharPtr                    str;
  
  dlg = (OrganismSelectionDialogPtr) GetObjectExtra (s);
  if (dlg == NULL) return;

  if ( (int) ch == 0 ) return;
  
  /* later, handle control key combos */
#ifdef WIN_MSWIN
  if (ch == 3)
  {
    str = SaveStringFromText (dlg->tax_name_txt);
    StringToClipboard (str);
    str = MemFree (str);
  }
#else
  if (ctrlKey && ch == 'c')
  {
    str = SaveStringFromText (dlg->tax_name_txt);
    StringToClipboard (str);
    str = MemFree (str);
  }
#endif
}

static void SetOrganismText (TexT t)
{
  OrganismSelectionDialogPtr dlg;
  Int4                       pos, prevpos;
  Boolean                    match;
  CharPtr                    old_val;
  Boolean                    changed_val = FALSE;
  
  dlg = (OrganismSelectionDialogPtr) GetObjectExtra (t);
  if (dlg == NULL) return;
  old_val = dlg->tax_name_val;
  dlg->tax_name_val = SaveStringFromText (dlg->tax_name_txt);
  if (dlg->tax_name_val != NULL)
  {
  	dlg->tax_name_val [0] = TO_UPPER (dlg->tax_name_val [0]);
  }
  if (!StringHasNoText (old_val) && StringCmp (old_val, dlg->tax_name_val) != 0)
  {
    changed_val = TRUE;
  }
  if (old_val != NULL)
  {
  	MemFree (old_val);
  }

  GetOrgPosForText (dlg->tax_name_val, &pos, &match);
  SetOffset (dlg->org_list, 0, pos - 1);
  if (pos != dlg->org_row)
  {
    prevpos = dlg->org_row;
    if (match)
    { 
      dlg->org_row = pos;
      SetTitle (dlg->tax_name_txt, dlg->tax_name_val);
    }
    else
    {
      dlg->org_row = -1;
    }
  	if (prevpos != -1)
    {
  	  InvalDocRows (dlg->org_list, prevpos, 1, 1);
    }
    if (match)
    {
      InvalDocRows (dlg->org_list, dlg->org_row, 1, 1);
    }
  }
  else if (!match)
  {
  	dlg->org_row = -1;
    InvalDocRows (dlg->org_list, pos, 1, 1);	
  }
}

static void SetOrganismDoc (DoC d, PoinT pt)
{
  Int2      item, row, prevrow;

  OrganismSelectionDialogPtr dlg;
  CharPtr           old_name;
  
  dlg = (OrganismSelectionDialogPtr) GetObjectExtra (d);
  if (dlg == NULL) return;
  
  MapDocPoint (d, pt, &item, &row, NULL, NULL);
  if (item > 0 && row > 0) {
    prevrow = dlg->org_row;
    dlg->org_row = item;
    if (item != prevrow)
    {
      if (prevrow != -1)
      {
        InvalDocRows (d, prevrow, 1, 1);
      }
      InvalDocRows (d, item, 1, 1);
      old_name = SaveStringFromText (dlg->tax_name_txt);
      SetTitle (dlg->tax_name_txt, GetTextForOrgPos (item));
      old_name = MemFree (old_name);
      dlg->tax_name_val = SaveStringFromText (dlg->tax_name_txt);
    }  	
  }
}


static void ResetOrganismSelectionDialog (OrganismSelectionDialogPtr dlg)
{  
  if (dlg == NULL)
  {
    return;
  }
  
  SetTitle (dlg->tax_name_txt, "");
  SetOrganismText (dlg->tax_name_txt);
}

static void DataToOrganismSelectionDialog (DialoG d, Pointer data)
{
  OrganismSelectionDialogPtr dlg;
  
  dlg = (OrganismSelectionDialogPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return;
  }
  
  if (StringHasNoText (data))
  {
    SetTitle (dlg->tax_name_txt, "");
  }
  else
  {
    SetTitle (dlg->tax_name_txt, data);
  }
  SetOrganismText (dlg->tax_name_txt);  
}

static Pointer OrganismSelectionDialogToData (DialoG d)
{
  OrganismSelectionDialogPtr dlg;
  
  dlg = (OrganismSelectionDialogPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return NULL;
  }
  else
  {
    return SaveStringFromText (dlg->tax_name_txt);
  }
}

static ParData orgListPar = {FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData orgListCol = {0, 0, 160, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, TRUE};

extern DialoG OrganismSelectionDialog (GrouP parent, CharPtr org_name)
{
  GrouP           grp;
  Int2            height;
  ValNodePtr      vnp;
  OrganismSelectionDialogPtr dlg;
  RecT                       r;
  OrgInfoPtr                 oip;

  dlg = (OrganismSelectionDialogPtr) MemNew (sizeof (OrganismSelectionDialogData));

  grp = HiddenGroup (parent, -1, 0, NULL);
  SetObjectExtra (grp, dlg, CleanupOrganismSelectionDialog);
  SetGroupSpacing (grp, 10, 10);

  dlg->dialog = (DialoG) grp;
  dlg->todialog = DataToOrganismSelectionDialog;
  dlg->fromdialog = OrganismSelectionDialogToData;
  dlg->dialogmessage = NULL;
  dlg->testdialog = NULL;

  LoadOrganismList ();	
  
  dlg->tax_name_txt = DialogText (grp, "", 20, SetOrganismText);
  SetObjectExtra (dlg->tax_name_txt, dlg, NULL);
  if (org_name != NULL)
  {
  	SetTitle (dlg->tax_name_txt, org_name);
  }
  SetOrganismText (dlg->tax_name_txt);

  SelectFont (programFont);
  height = LineHeight ();
  SelectFont (systemFont);
  dlg->org_list = DocumentPanel (grp, stdCharWidth * 25, height * 6);
  SetObjectExtra (dlg->org_list, dlg, NULL);
  
  ObjectRect (dlg->org_list, &r);
  InsetRect (&r, 4, 4);
  orgListCol.pixWidth = r.right - r.left;

  for (vnp = orglist; vnp != NULL; vnp = vnp->next)
  {
    oip = (OrgInfoPtr) vnp->data.ptrvalue;
    if (oip != NULL)
    {
  	  AppendText (dlg->org_list, oip->taxname, &orgListPar, &orgListCol, programFont);
    }
  }
  SetDocAutoAdjust (dlg->org_list, FALSE);
  SetDocProcs (dlg->org_list, SetOrganismDoc, NULL, NULL, NULL);
  SetDocShade (dlg->org_list, NULL, NULL, OrgNameHighlight, NULL);
  SetSlateChar ((SlatE) dlg->org_list, OrgNameOnKey);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) dlg->tax_name_txt, (HANDLE) dlg->org_list, NULL);
  
  return (DialoG) grp;
}

#define NUM_ORGS_DISPLAYED 5
typedef struct multiorganismselectiondialog
{
  DIALOG_MESSAGE_BLOCK
  DialoG       org_select_dlg;
  TexT         tax_name_txt [NUM_ORGS_DISPLAYED];
  ButtoN       copy_btn [NUM_ORGS_DISPLAYED];
  PrompT       id_txt [NUM_ORGS_DISPLAYED];
  DialoG       location_dlg [NUM_ORGS_DISPLAYED];
  GrouP        gcode_grp [NUM_ORGS_DISPLAYED];
  ButtoN       gcode_btn [NUM_ORGS_DISPLAYED];
  DialoG       gcode_dlg [NUM_ORGS_DISPLAYED];
  BaR          id_scroll;
  ValNodePtr   row_list;
  Int4         num_vals;
  ValNodePtr   geneticcodelist;
} MultiOrganismSelectionDialogData, PNTR MultiOrganismSelectionDialogPtr;

typedef struct multiorgcopybtn
{
  MultiOrganismSelectionDialogPtr dlg;
  Int4                            pos;
} MultiOrgCopyBtnData, PNTR MultiOrgCopyBtnPtr;

static void CleanupMultiOrganismSelectionDialog (GraphiC g, VoidPtr data)

{
  MultiOrganismSelectionDialogPtr dlg;
  
  dlg = (MultiOrganismSelectionDialogPtr) GetObjectExtra (g);
  if (dlg != NULL)
  {
    dlg->row_list = FreeTableDisplayRowList (dlg->row_list);
    dlg->geneticcodelist = ValNodeFree (dlg->geneticcodelist);
  }

  StdCleanupExtraProc (g, data);
}

static Int4 GetGeneticCodeForTaxNameAndLocation (CharPtr taxname, CharPtr location)
{
  ValNodePtr vnp;
  OrgInfoPtr oip;
  Boolean    found = FALSE;
  Int4       use_code;
  
  use_code = UseGeneticCodeForLocation (location);
  if (use_code == USE_OTHER_GENETIC_CODE)
  {
    return 11;
  }
  else if (StringHasNoText (taxname))
  {
    return -1;
  }
  
  for (vnp = orglist; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->data.ptrvalue == NULL)
    {
      continue;
    }
    oip = (OrgInfoPtr) vnp->data.ptrvalue;
    if (StringICmp (oip->taxname, taxname) == 0)
    {
      if (use_code == USE_NUCLEAR_GENETIC_CODE)
      {
        return oip->ngcode;
      }
      else
      {
        return oip->mgcode;
      }
    }
  }
  
  return -1;
}

static CharPtr GetTableDisplayCellValue (ValNodePtr row_list, Int4 row_num, Int4 col_num)
{
  ValNodePtr row_vnp, col_vnp;
  Int4       j;
  
  if (row_list == NULL)
  {
    return NULL;
  }
  
  for (row_vnp = row_list, j = 0;
       row_vnp != NULL && j < row_num; 
       row_vnp = row_vnp->next, j++)
  {
  }
  
  if (row_vnp == NULL)
  {
    return NULL;
  }
  
  for (col_vnp = row_vnp->data.ptrvalue, j = 0;
       col_vnp != NULL && j < col_num;
       col_vnp = col_vnp->next, j++)
  {
  }
  if (col_vnp == NULL)
  {
    return NULL;
  }
  else
  {
    return col_vnp->data.ptrvalue;
  }
}

static void 
UpdateTableDisplayCellValue 
(ValNodePtr row_list,
 Int4 row_num,
 Int4 col_num,
 CharPtr new_value)
{
  ValNodePtr row_vnp, col_vnp;
  Int4       j;
  
  if (row_list == NULL)
  {
    return;
  }
  
  for (row_vnp = row_list, j = 0;
       row_vnp != NULL && j < row_num; 
       row_vnp = row_vnp->next, j++)
  {
  }
  
  if (row_vnp == NULL)
  {
    return;
  }
  
  for (col_vnp = row_vnp->data.ptrvalue, j = 0;
       col_vnp != NULL && j < col_num;
       col_vnp = col_vnp->next, j++)
  {
  }
  if (col_vnp == NULL)
  {
    return;
  }
  else
  {
    col_vnp->data.ptrvalue = StringSave (new_value);
  }
}

static void 
UpdateGeneticCodePosition 
(MultiOrganismSelectionDialogPtr dlg,
 Int4    row_num,
 CharPtr taxname,
 CharPtr location)
{
  ValNode    vn;
  Int4       gcode = -1;
  Int4       offset;
  CharPtr    gcode_name;
  
  if (dlg == NULL || row_num < 0 || row_num >= NUM_ORGS_DISPLAYED)
  {
    return;
  }
    
  gcode = GetGeneticCodeForTaxNameAndLocation (taxname, location);
  
  if (gcode < 0)
  {
    offset = GetBarValue (dlg->id_scroll);
    gcode_name = GetTableDisplayCellValue (dlg->row_list, 
                                           offset + row_num, 3);
              
    vn.choice = GeneticCodeFromStringAndList (gcode_name, dlg->geneticcodelist);
    vn.next = NULL;
    vn.data.ptrvalue = gcode_name;
    PointerToDialog (dlg->gcode_dlg [row_num], &vn);
    Show (dlg->gcode_dlg [row_num]);
    Hide (dlg->gcode_btn [row_num]);
  }
  else
  {
    offset = GetBarValue (dlg->id_scroll);
    gcode_name = GeneticCodeStringFromIntAndList (gcode, dlg->geneticcodelist);
    UpdateTableDisplayCellValue (dlg->row_list, offset + row_num, 3, gcode_name);
    SetTitle (dlg->gcode_btn [row_num], gcode_name);
    Show (dlg->gcode_btn [row_num]);
    Hide (dlg->gcode_dlg [row_num]);
  }
}

static void DisplayPosition (MultiOrganismSelectionDialogPtr dlg, Int4 pos)
{
  Int4       row_num;
  ValNodePtr row_vnp, col_vnp;
  ValNode    vn;
  CharPtr    taxname = NULL;
  CharPtr    location = NULL;
  Int4       gcode;

  if (dlg == NULL)
  {
    return;
  }
  
  for (row_num = 0, row_vnp = dlg->row_list;
       row_num < pos && row_vnp != NULL;
       row_num++, row_vnp = row_vnp->next)
  {
  }
  
  for (row_num = 0;
       row_num < NUM_ORGS_DISPLAYED && row_vnp != NULL; 
       row_num++, row_vnp = row_vnp->next)
  {
    /* set ID */
    col_vnp = row_vnp->data.ptrvalue;
    SetTitle (dlg->id_txt [row_num], col_vnp->data.ptrvalue);
    /* set tax name */
    col_vnp = col_vnp->next;
    taxname = col_vnp->data.ptrvalue;
    SetTitle (dlg->tax_name_txt [row_num], taxname);
    /* set location */
    col_vnp = col_vnp->next;
    location = col_vnp->data.ptrvalue;
    if (StringHasNoText (location))
    {
      vn.choice = 1;
      vn.data.ptrvalue = "genomic";
    }
    else
    {
      vn.choice = GetValForEnumName (biosource_genome_simple_alist, location);
      vn.data.ptrvalue = location;
    }
    vn.next = NULL;
    PointerToDialog (dlg->location_dlg[row_num], &vn);
    
    Show (dlg->copy_btn [row_num]);
    Show (dlg->id_txt [row_num]);
    Show (dlg->tax_name_txt [row_num]);
    Show (dlg->location_dlg [row_num]);

    /* display genetic code */  
    col_vnp = col_vnp->next;   
    Show (dlg->gcode_grp [row_num]);
    
    gcode = GetGeneticCodeForTaxNameAndLocation (taxname, location);
    if (gcode < 0)
    {
      vn.choice = GeneticCodeFromStringAndList (col_vnp->data.ptrvalue, dlg->geneticcodelist);
      vn.next = NULL;
      vn.data.ptrvalue = col_vnp->data.ptrvalue;
      PointerToDialog (dlg->gcode_dlg [row_num], &vn);
      Show (dlg->gcode_dlg [row_num]);
      Hide (dlg->gcode_btn [row_num]);
    }
    else
    {
      SetTitle (dlg->gcode_btn [row_num], GeneticCodeStringFromIntAndList (gcode, dlg->geneticcodelist));
      Show (dlg->gcode_btn [row_num]);
      Hide (dlg->gcode_dlg [row_num]);
    }
  }
  
  while (row_num < NUM_ORGS_DISPLAYED)
  {
    Hide (dlg->copy_btn [row_num]);
    Hide (dlg->id_txt [row_num]);
    Hide (dlg->tax_name_txt [row_num]);
    Hide (dlg->location_dlg [row_num]);
    Hide (dlg->gcode_grp [row_num]);
    row_num++;
  }
}

static void CollectPositionValues (MultiOrganismSelectionDialogPtr dlg, Int4 pos)
{
  Int4       row_num;
  ValNodePtr row_vnp, col_vnp, val_vnp;
  Int4       gcode;
  CharPtr    taxname, location, gcode_name;

  if (dlg == NULL)
  {
    return;
  }
  for (row_num = 0, row_vnp = dlg->row_list;
       row_num < pos && row_vnp != NULL;
       row_num++, row_vnp = row_vnp->next)
  {
  }
  
  for (row_num = 0;
       row_num < NUM_ORGS_DISPLAYED && row_vnp != NULL; 
       row_num++, row_vnp = row_vnp->next)
  {
    col_vnp = row_vnp->data.ptrvalue;
    /* skip ID - it can't be edited */
    col_vnp = col_vnp->next;
    
    /* get tax name */
    col_vnp->data.ptrvalue = MemFree (col_vnp->data.ptrvalue);
    col_vnp->data.ptrvalue = SaveStringFromText (dlg->tax_name_txt [row_num]);
    taxname = col_vnp->data.ptrvalue;
    col_vnp = col_vnp->next;
    
    /* get location */
    val_vnp = DialogToPointer (dlg->location_dlg [row_num]);
    if (val_vnp == NULL)
    {
      location = NULL;
    }
    else
    {
      location = val_vnp->data.ptrvalue;
    }
    StringToLower (location);
    val_vnp = ValNodeFree (val_vnp);
    col_vnp->data.ptrvalue = MemFree (col_vnp->data.ptrvalue);
    col_vnp->data.ptrvalue = location;
    col_vnp = col_vnp->next;
    
    /* get genetic code */
    gcode = GetGeneticCodeForTaxNameAndLocation (taxname, location);
    if (gcode < 0)
    {
      val_vnp = DialogToPointer (dlg->gcode_dlg [row_num]);
      if (val_vnp == NULL)
      {
        gcode_name = NULL;
      }
      else
      {
        gcode_name = val_vnp->data.ptrvalue;
      }
      ValNodeFree (val_vnp);
    }
    else
    {
      gcode_name = StringSave (GeneticCodeStringFromIntAndList (gcode, dlg->geneticcodelist));
    }
    col_vnp->data.ptrvalue = MemFree (col_vnp->data.ptrvalue);
    col_vnp->data.ptrvalue = gcode_name;
    col_vnp = col_vnp->next;
    
  }  
}

static void MultiOrgScroll (BaR sb, GraphiC g, Int4 newval, Int4 oldval)
{
  MultiOrganismSelectionDialogPtr dlg;
  
  dlg = (MultiOrganismSelectionDialogPtr) GetObjectExtra (sb);
  if (dlg == NULL)
  {
    return;
  }
  
  /* first, collect old values */
  CollectPositionValues (dlg, oldval);
  
  /* set newly visible values */
  DisplayPosition (dlg, newval);  
}

static void MultiOrgCopy (ButtoN b)
{
  MultiOrgCopyBtnPtr mp;
  CharPtr            tax_name;
  ValNodePtr         val_vnp;
  CharPtr            location;

  mp = (MultiOrgCopyBtnPtr) GetObjectExtra (b);
  if (mp == NULL || mp->dlg == NULL || mp->pos < 0 || mp->pos >= NUM_ORGS_DISPLAYED)
  {
    return;
  }
  
  tax_name = (CharPtr) DialogToPointer (mp->dlg->org_select_dlg);
  SetTitle (mp->dlg->tax_name_txt [mp->pos], tax_name);
  
  /* get location for this row */
  val_vnp = DialogToPointer (mp->dlg->location_dlg [mp->pos]);
  if (val_vnp != NULL)
  {
    location = val_vnp->data.ptrvalue;
    UpdateGeneticCodePosition (mp->dlg, mp->pos, tax_name, location);
      
    val_vnp = ValNodeFreeData (val_vnp);
    /* location is freed when we free val_vnp */
    location = NULL;
  }
  
  tax_name = MemFree (tax_name);
}

static void DataToMultiOrganismSelectionDialog (DialoG d, Pointer userdata)
{
  MultiOrganismSelectionDialogPtr dlg;
  ValNodePtr                      row_list;

  dlg = (MultiOrganismSelectionDialogPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return;
  }
  
  row_list = (ValNodePtr) userdata;
  dlg->row_list = FreeTableDisplayRowList (dlg->row_list);
  dlg->row_list = CopyTableDisplayRowList (row_list);
  dlg->num_vals = ValNodeLen (dlg->row_list);
  
  CorrectBarMax (dlg->id_scroll, dlg->num_vals - NUM_ORGS_DISPLAYED);  
  CorrectBarValue (dlg->id_scroll, 0);
  DisplayPosition (dlg, 0);    
}

static Pointer MultiOrganismSelectionDialogToData (DialoG d)
{
  MultiOrganismSelectionDialogPtr dlg;
  Int4                            pos;
  ValNodePtr                      row_list;

  dlg = (MultiOrganismSelectionDialogPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return NULL;
  }
  
  pos = GetBarValue (dlg->id_scroll);
  CollectPositionValues (dlg, pos);
  row_list = CopyTableDisplayRowList (dlg->row_list);
  return row_list;
}

static void SetRowListColumn (ValNodePtr row_list, Int4 column, CharPtr new_value)
{
  ValNodePtr row_vnp, col_vnp;
  Int4       col_num;
  
  for (row_vnp = row_list; row_vnp != NULL; row_vnp = row_vnp->next)
  {
    for (col_vnp = row_vnp->data.ptrvalue, col_num = 0;
         col_vnp != NULL && col_num < column;
         col_vnp = col_vnp->next, col_num++)
    {
    }
    if (col_vnp != NULL)
    {
      col_vnp->data.ptrvalue = MemFree (col_vnp->data.ptrvalue);
      col_vnp->data.ptrvalue = StringSave (new_value);
    }
  }
}

static void 
ApplyOrgModColumnOrCell 
(CharPtr            mod_name,
 CharPtr            suggested_value,
 Int4               row, 
 SourceAssistantPtr sap,
 SeqEntryPtr        seq_list,
 ValNodePtr         row_list,
 Int4               row_list_column,
 Int2               seqPackage);
 
static Boolean 
ContinueWithAutopopulatedGeneticCodes 
(SeqEntryPtr        seq_list,
 SourceAssistantPtr sap,
 ValNodePtr         row_list,
 Int4               affected_row);

static void SetAllGeneticCodes (ButtoN b)
{
  MultiOrganismSelectionDialogPtr dlg;
  Int4                            scroll_pos;

  dlg = (MultiOrganismSelectionDialogPtr) GetObjectExtra (b);
  if (dlg == NULL)
  {
    return;
  }
  scroll_pos = GetBarValue (dlg->id_scroll);
  CollectPositionValues (dlg, scroll_pos);

  if (ContinueWithAutopopulatedGeneticCodes (NULL, NULL, dlg->row_list, -1))
  {
    ApplyOrgModColumnOrCell ("genetic_code", "Standard", -1, NULL, NULL, dlg->row_list, 3, 0);
    DisplayPosition (dlg, scroll_pos);
  }

}

static void AddGcodeCommentBtn (ButtoN b)
{
  MultiOrgCopyBtnPtr bp;
  Int4               scroll_pos;

  bp = (MultiOrgCopyBtnPtr) GetObjectExtra (b);
  if (bp == NULL || bp->dlg == NULL)
  {
    return;
  }

  scroll_pos = GetBarValue (bp->dlg->id_scroll);
  /* first, collect current values from dialog*/
  CollectPositionValues (bp->dlg, scroll_pos);

  ApplyOrgModColumnOrCell ("genetic_code_comment", NULL, bp->pos, NULL, NULL,
                           bp->dlg->row_list, 4, 0);
  /* now repopulate */
  DisplayPosition (bp->dlg, scroll_pos);
}

static void SetAllLocations (ButtoN b)
{
  MultiOrganismSelectionDialogPtr dlg;
  Int4                            scroll_pos;

  dlg = (MultiOrganismSelectionDialogPtr) GetObjectExtra (b);
  if (dlg == NULL)
  {
    return;
  }
  ApplyOrgModColumnOrCell ("location", "genomic", -1, NULL, NULL, dlg->row_list, 2, 0);
  scroll_pos = GetBarValue (dlg->id_scroll);
  DisplayPosition (dlg, scroll_pos);
}

static void SetAllOrganisms (ButtoN b)
{
  MultiOrganismSelectionDialogPtr dlg;
  Int4                            scroll_pos;

  dlg = (MultiOrganismSelectionDialogPtr) GetObjectExtra (b);
  if (dlg == NULL)
  {
    return;
  }
  ApplyOrgModColumnOrCell ("organism", NULL, -1, NULL, NULL, dlg->row_list, 1, 0);
  scroll_pos = GetBarValue (dlg->id_scroll);
  DisplayPosition (dlg, scroll_pos);
}

static void ChangeLocationOrTaxName (MultiOrgCopyBtnPtr bp)
{
  ValNodePtr           val_vnp;
  CharPtr              tax_name = NULL, location = NULL;

  if (bp == NULL)
  {
    return;
  }

  /* get taxname for this row */
  tax_name = SaveStringFromText (bp->dlg->tax_name_txt [bp->pos]);

  /* get location for this row */
  val_vnp = DialogToPointer (bp->dlg->location_dlg [bp->pos]);
  if (val_vnp->data.ptrvalue == NULL)
  {
    UpdateGeneticCodePosition (bp->dlg, bp->pos, tax_name, NULL);
  }
  else
  {
    location = val_vnp->data.ptrvalue;
    UpdateGeneticCodePosition (bp->dlg, bp->pos, tax_name, location);
    
    val_vnp = ValNodeFreeData (val_vnp);
    /* location is freed when we free val_vnp */
    location = NULL;
  }
  
  tax_name = MemFree (tax_name); 
}

static void ChangeLocationPopup (Pointer userdata)
{
  MultiOrgCopyBtnPtr   bp;
  
  bp = (MultiOrgCopyBtnPtr) userdata;
  if (bp == NULL)
  {
    return;
  }

  ChangeLocationOrTaxName (bp);
}

static void MultiOrgText (TexT t)
{
  MultiOrgCopyBtnPtr bp;
  CharPtr            cp;

  bp = (MultiOrgCopyBtnPtr) GetObjectExtra (t);
  if (bp == NULL)
  {
    return;
  }
  cp = SaveStringFromText (t);
  PointerToDialog (bp->dlg->org_select_dlg, cp);
  cp = MemFree (cp);
  ChangeLocationOrTaxName (bp);
}

static void ChangeGeneticCodePopup (Pointer userdata)
{
  MultiOrgCopyBtnPtr   bp;
  ValNodePtr           val_vnp;
  CharPtr              gcode_name;
  Int4                 offset;

  bp = (MultiOrgCopyBtnPtr) userdata;
  if (bp == NULL)
  {
    return;
  }
  
  val_vnp = DialogToPointer (bp->dlg->gcode_dlg [bp->pos]);
  if (val_vnp == NULL)
  {
    gcode_name = NULL;
  }
  else
  {
    gcode_name = val_vnp->data.ptrvalue;
  }
  
  offset = GetBarValue (bp->dlg->id_scroll);

  UpdateTableDisplayCellValue (bp->dlg->row_list, bp->pos + offset, 3, gcode_name);
  
  val_vnp = ValNodeFreeData (val_vnp);
  
}

static DialoG MultiOrganismSelectionDialog (GrouP parent)
{
  MultiOrganismSelectionDialogPtr dlg;
  GrouP                           grp, id_grp, scroll_grp;
  Int4                            k;
  MultiOrgCopyBtnPtr              bp;
  RecT                            r1, r2, r3;
  ValNodePtr                      gencodelist;
  ButtoN                          b;

  dlg = (MultiOrganismSelectionDialogPtr) MemNew (sizeof (MultiOrganismSelectionDialogData));

  grp = HiddenGroup (parent, -1, 0, NULL);
  SetObjectExtra (grp, dlg, CleanupMultiOrganismSelectionDialog);
  SetGroupSpacing (grp, 10, 10);

  dlg->dialog = (DialoG) grp;
  dlg->todialog = DataToMultiOrganismSelectionDialog;
  dlg->fromdialog = MultiOrganismSelectionDialogToData;
  dlg->dialogmessage = NULL;
  dlg->testdialog = NULL;
  
  dlg->num_vals = 0;
  dlg->row_list = NULL;
  
  dlg->geneticcodelist = GetGeneticCodeValNodeList ();

  dlg->org_select_dlg = OrganismSelectionDialog (grp, "");
  
  scroll_grp = NormalGroup (grp, 2, 0, "", NULL, NULL);
  
  id_grp = HiddenGroup (scroll_grp, 5, 0, NULL);
  SetGroupSpacing (id_grp, 10, 10);
  StaticPrompt (id_grp, "SeqID", 7 * stdCharWidth, 0, programFont, 'l');
  StaticPrompt (id_grp, "Copy", 0, 0, programFont, 'l');
  b = PushButton (id_grp, "Organism", SetAllOrganisms);
  SetObjectExtra (b, dlg, NULL);
  b = PushButton (id_grp, "Location", SetAllLocations);
  SetObjectExtra (b, dlg, NULL);
  b = PushButton (id_grp, "Genetic Code", SetAllGeneticCodes);
  SetObjectExtra (b, dlg, NULL);
  
  for (k = 0; k < NUM_ORGS_DISPLAYED; k++)
  {
    /* prompt with sequence ID */
     dlg->id_txt [k] = StaticPrompt (id_grp, "", 7 * stdCharWidth, 0, programFont, 'l');
    /* button for copying from organism selector */
    dlg->copy_btn [k] = PushButton (id_grp, "->", MultiOrgCopy);
    bp = (MultiOrgCopyBtnPtr) MemNew (sizeof (MultiOrgCopyBtnData));
    if (bp != NULL)
    {
      bp->dlg = dlg;
      bp->pos = k;
    }
    SetObjectExtra (dlg->copy_btn [k], bp, StdCleanupExtraProc);
    dlg->tax_name_txt [k] = DialogText (id_grp, "", 20, MultiOrgText);
    SetTextSelect (dlg->tax_name_txt [k], MultiOrgText, NULL);
    SetObjectExtra (dlg->tax_name_txt [k], bp, NULL);
    
    dlg->location_dlg [k] = EnumAssocSelectionDialog (id_grp, biosource_genome_simple_alist,
                                             "location", FALSE, ChangeLocationPopup, bp);
                                             
    dlg->gcode_grp [k] = HiddenGroup (id_grp, 0, 0, NULL);
    dlg->gcode_btn [k] = PushButton (dlg->gcode_grp [k], 
                                     "                                                ", 
                                     AddGcodeCommentBtn);
    SetObjectExtra (dlg->gcode_btn [k], bp, NULL);
    /* NOTE - need separate list because ValNodeSelectionDialog will free this one */
    gencodelist = GetGeneticCodeValNodeList ();
    dlg->gcode_dlg [k] = ValNodeSelectionDialog (dlg->gcode_grp [k], gencodelist, 6,
                                                 ValNodeStringName,
                                                 ValNodeSimpleDataFree,
                                                 ValNodeStringCopy,
                                                 ValNodeChoiceMatch,
                                                 "genetic code",
                                                  ChangeGeneticCodePopup, bp, FALSE);
    AlignObjects (ALIGN_LEFT, (HANDLE) dlg->gcode_btn [k], (HANDLE) dlg->gcode_dlg [k], NULL);
  }
 
  dlg->id_scroll = ScrollBar4 (scroll_grp, 0, 10, MultiOrgScroll);
  SetObjectExtra (dlg->id_scroll, dlg, NULL);

  AlignObjects (ALIGN_CENTER, (HANDLE) dlg->org_select_dlg, (HANDLE) scroll_grp, NULL);
  
  ObjectRect (dlg->copy_btn [0], &r1);
  ObjectRect (dlg->copy_btn [4], &r2);
  ObjectRect (dlg->id_scroll, &r3);
  r3.top = r1.top;
  r3.bottom = r2.bottom;
  SetPosition (dlg->id_scroll, &r3);
  
  return (DialoG) grp;
}


static void SourceAssistantExport (ButtoN b)
{
  SourceAssistantPtr sap;
  ValNodePtr         row_list = NULL;
  FILE               *fp;
  Char               path [PATH_MAX];
  
  sap = (SourceAssistantPtr) GetObjectExtra (b);
  if (sap == NULL) return;

  if (! GetOutputFileName (path, sizeof (path), NULL)) return;
  fp = FileOpen (path, "w");
  if (fp == NULL) 
  {
    Message (MSG_ERROR, "Unable to open %s", path);
    return;
  }
  
  row_list = PrepareSourceAssistantTableData (sap);

  PrintTableDisplayRowListToFile (row_list, fp);
  row_list = FreeTableDisplayRowList (row_list);
  FileClose (fp);
}

static void SourceAssistantOk (ButtoN b)
{
  SourceAssistantPtr sap;
  
  sap = (SourceAssistantPtr) GetObjectExtra (b);
  if (sap == NULL) return;
  sap->cancelled = FALSE;
  sap->done = TRUE;
}

static void SourceAssistantCancel (ButtoN b)
{
  SourceAssistantPtr sap;
  
  sap = (SourceAssistantPtr) GetObjectExtra (b);
  if (sap == NULL) return;
  if (Message (MSG_YN, "You will lose your changes if you cancel.  Are you sure?")
      == ANS_NO)
  {
    return;
  }
  
  sap->cancelled = TRUE;
  sap->done = TRUE;
}

static CharPtr GetFirstDeflineValue (SourceAssistantPtr sap, CharPtr mod_name)
{
  Char         text [128];
  Char         lookfor [128];
  Char         lookfor2 [128];
  Boolean      is_org = FALSE;
  Boolean      found_val = FALSE;
  Int4         i;

  if (sap == NULL)
  {
    return NULL;
  }
  
  if (StringICmp (mod_name, "org" ) == 0 || StringICmp (mod_name, "organism") == 0)
  {
    MakeSearchStringFromAlist (lookfor, "Org");
    MakeSearchStringFromAlist (lookfor2, "Organism");
    is_org = TRUE;
  }
  else
  {
    MakeSearchStringFromAlist (lookfor, mod_name);
  }

  for (i = 0; i < sap->num_deflines && !found_val; i++)
  {
    text [0] = '\0';
    if (LookForSearchString (sap->defline_list[i], lookfor, text, sizeof (text) - 1)) 
    {
      found_val = TRUE;
    }
    else if (is_org && LookForSearchString (sap->defline_list[i], lookfor2, text, sizeof (text) - 1))
    {
      found_val = TRUE;
    }
  }
  if (found_val)
  {
    return StringSave (text);
  }
  else
  {
    return NULL;
  }
}

static CharPtr 
GetTagListValueEx (TagListPtr tlp, Int4 seq_num, Int4 col_num)
{
  Int4         seq_pos;
  CharPtr      str = NULL;
  ValNodePtr   vnp;
  
  if (tlp == NULL) return NULL;
  
  for (vnp = tlp->vnp, seq_pos = 0;
       vnp != NULL && seq_pos != seq_num;
       vnp = vnp->next, seq_pos++)
  {
    
  }
  if (vnp != NULL)
  {
    str = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, col_num);
  }
  return str;
}

static CharPtr 
GetTagListValue (TagListPtr tlp, Int4 seq_num)
{
  return GetTagListValueEx (tlp, seq_num, 1);
}

static void 
ReplaceValueInDefLines 
(SourceAssistantPtr sap,
 CharPtr            value_name, 
 CharPtr            new_value, 
 Int4               seq_num)
{
  
  if (sap == NULL || seq_num < 0 || seq_num >= sap->num_deflines) return;

  sap->defline_list[seq_num] = ReplaceValueInOneDefLine (sap->defline_list[seq_num],
                                                         value_name, new_value);
}

static void UpdateOrgModDlg (SourceAssistantPtr sap)
{
  Int4               j;
  Char               old_value [128];
  Boolean            found_organism = FALSE;
  ValNodePtr         row_list, header_list = NULL;

  if (sap == NULL)
  {
    return;
  }
  row_list = PrepareSourceAssistantTableData (sap);
  PointerToDialog (sap->orgmod_dlg, row_list);
  
  if (row_list == NULL)
  {
    SetModifierList (sap->mod_doc, NULL);
    return;
  }
  
  for (j = 0; j < sap->num_deflines && !found_organism; j++)
  {
    if (! found_organism)
    {
      found_organism = LookForSearchString (sap->defline_list[j],
                                            "organism=",
                                            old_value, 
                                            sizeof (old_value) - 1);
      if (! found_organism)
      {
        found_organism = LookForSearchString (sap->defline_list[j],
                                              "org=",
                                              old_value, 
                                              sizeof (old_value) - 1);
      }
    }
  }
  header_list = row_list->data.ptrvalue;
  header_list = header_list->next;
  if (!found_organism)
  {
    header_list = header_list->next;
  }
  SetModifierList (sap->mod_doc, header_list);
  FreeTableDisplayRowList (row_list);
}

static EnumFieldAssocPtr BuildGeneticCodeEnum (void)
{
  ValNodePtr        gencodelist = NULL;
  Int4              num_gencodes = 0, index;
  EnumFieldAssocPtr gencode_alist = NULL;
  ValNodePtr        vnp;

  gencodelist = GetGeneticCodeValNodeList ();
  num_gencodes = ValNodeLen (gencodelist);
  gencode_alist = (EnumFieldAssocPtr) MemNew ((num_gencodes + 2) * sizeof (EnumFieldAssoc));

  gencode_alist [0].name = StringSave (" ");
  gencode_alist [0].value = 0;

  for (index = 1, vnp = gencodelist;
       index <= num_gencodes && vnp != NULL; 
       index++, vnp = vnp->next)
  {
    gencode_alist [index].name = StringSave (vnp->data.ptrvalue);
    gencode_alist [index].value = vnp->choice;
  }

  gencode_alist [index].name = NULL;
  ValNodeFreeData (gencodelist);
  return gencode_alist;
}

static EnumFieldAssocPtr FreeGeneticCodeEnum (EnumFieldAssocPtr gcode_alist)
{
  EnumFieldAssocPtr eap;
  
  for (eap = gcode_alist; eap != NULL && eap->name != NULL; eap++)
  {
    eap->name = MemFree (eap->name);
  }
  gcode_alist = MemFree (gcode_alist);
  return gcode_alist;
}

static GrouP MakeSourceInstructionGroup (GrouP parent)
{
  GrouP instr_grp;
  
  instr_grp = HiddenGroup (parent, 1, 0, NULL);
  StaticPrompt (instr_grp, "Scientific names should not be abbreviated.",
                0, 0, programFont, 'l');
  StaticPrompt (instr_grp, "(use 'Drosophila melanogaster' instead of 'D. melanogaster')",
                0, 0, programFont, 'l');
  return instr_grp;
}

static GrouP MakeGeneticCodeInstructionGroup (GrouP parent)
{
  GrouP instr_grp;
  
  instr_grp = HiddenGroup (parent, 1, 0, NULL);
  StaticPrompt (instr_grp, "Please choose the translation table for your sequence.",
                0, 0, programFont, 'l');
  StaticPrompt (instr_grp, "Examples: Standard, Bacterial and Plant Plastid, Vertebrate Mitochondrial",
                0, 0, programFont, 'l');
  
  return instr_grp;
}

static GrouP MakeGeneticCodeCommentInstructionGroup (GrouP parent)
{
  GrouP instr_grp;
  
  instr_grp = HiddenGroup (parent, 1, 0, NULL);
  StaticPrompt (instr_grp, "When a genetic code is determined automatically from the organism name and location,",
                0, 0, programFont, 'l');
  StaticPrompt (instr_grp, "you cannot edit the genetic code directly.  You may add information about the ",
                0, 0, programFont, 'l');
  StaticPrompt (instr_grp, "genetic code here if you wish.",
                0, 0, programFont, 'l');
  
  return instr_grp;
}

static GrouP MakeNontextInstructionGroup (GrouP parent)
{
  GrouP instr_grp;
  
  instr_grp = HiddenGroup (parent, 1, 0, NULL);
  StaticPrompt (instr_grp, "This modifier allows only TRUE/FALSE values.",
                0, 0, programFont, 'l');
  StaticPrompt (instr_grp, "The modifier will only appear in the file if it is set to TRUE,",
                0, 0, programFont, 'l');
  StaticPrompt (instr_grp, "otherwise it will be absent.",
                0, 0, programFont, 'l');
  
  return instr_grp;
}

static GrouP MakeLocationInstructionGroup (GrouP parent)
{
  GrouP instr_grp;
  
  instr_grp = HiddenGroup (parent, 1, 0, NULL);
  StaticPrompt (instr_grp, "Use this to specify the subcellular location or viral origin of the sequences.",
                0, 0, programFont, 'l');
  StaticPrompt (instr_grp, "Example: Use 'Genomic' for a sequence encoded by a nuclear gene.",
                0, 0, programFont, 'l');
  
  return instr_grp;
}

static CharPtr 
GetValueFromTagList 
(TagListPtr        tlp,
 Int4              row, 
 Boolean           is_nontext, 
 Int4              mod_type,
 Int2              seqPackage,
 EnumFieldAssocPtr gencode_alist)
{
  CharPtr new_value, tmp_value;
  
  new_value = GetTagListValue (tlp, row);
  if (is_nontext)
  {
    if (StringCmp (new_value, "1") != 0)
    {
      new_value = MemFree (new_value);
    }
  }
  else if (mod_type == eModifierType_Location)
  {
    tmp_value = GetEnumName (atoi(new_value), biosource_genome_simple_alist);
    StringToLower (tmp_value);
    new_value = MemFree (new_value);
    new_value = StringSave (tmp_value);
  }
  else if (mod_type == eModifierType_NucGeneticCode
           || mod_type == eModifierType_MitoGeneticCode)
  {
    tmp_value = GetEnumName (atoi(new_value), gencode_alist);
    new_value = MemFree (new_value);
    new_value = StringSave (tmp_value);
  }
  else if (mod_type == eModifierType_MolType)
  {
    if (seqPackage == SEQ_PKG_GENOMICCDNA)
    {
      tmp_value = GetEnumName (atoi (new_value), biomol_nucGen_alist);
    }
    else
    {
      tmp_value = GetEnumName (atoi (new_value), biomol_nucX_alist);
      if (StringICmp (tmp_value, "mRNA [cDNA]") == 0)
      {
        tmp_value = StringSave ("mRNA");
      }            
    }
    new_value = MemFree (new_value);
    new_value = StringSave (tmp_value);        
  }
  else if (mod_type == eModifierType_Topology)
  {
    tmp_value = GetEnumName (atoi (new_value), topology_nuc_alist);
    new_value = MemFree (new_value);
    new_value = StringSave (tmp_value);        
  }
  return new_value;
}

static void AddSeqIDAndValueToRowList 
(CharPtr         id,
 CharPtr         title,
 ValNodePtr PNTR row_list)
{
  Char        text [128];
  CharPtr     str = NULL;
  ValNodePtr  column_list = NULL;

  if (row_list == NULL)
  {
    return;
  }

  /* put ID in first location */
  ValNodeAddPointer (&column_list, 0, StringSave (id));

  /* get organism */
  str = NULL;
  if (LookForSearchString (title, (CharPtr) short_org, text, sizeof (text) - 1)) 
  {
    str = StringSave (text);
  } 
  else if (LookForSearchString (title, (CharPtr) long_org, text, sizeof (text) - 1))
  {
    str = StringSave (text);
  }
  
  ValNodeAddPointer (&column_list, 0, str);
  
  /* get location */
  str = NULL;
  if (LookForSearchString (title, "[location=", text, sizeof (text) - 1))
  {
    str = StringSave (text);
  }
  else
  {
    str = StringSave ("genomic");
  }
  ValNodeAddPointer (&column_list, 0, str);
  
  /* get genetic code */
  str = NULL;
  if (LookForSearchString (title, "[genetic_code=", text, sizeof (text) - 1))
  {
    str = StringSave (text);
  }
  else
  {
    str = StringSave ("Standard");
  }
  ValNodeAddPointer (&column_list, 0, str);
  
  /* get genetic code comment */
  str = NULL;
  if (LookForSearchString (title, "[genetic_code_comment=", text, sizeof (text) - 1))
  {
    str = StringSave (text);
  }
  else
  {
    str = NULL;
  }
  ValNodeAddPointer (&column_list, 0, str);
  
  ValNodeAddPointer (row_list, 0, column_list); 
}

static void AddSeqIDAndValueFromSeqEntryToRowList 
(SeqEntryPtr     sep,
 ValNodePtr PNTR row_list)
{
  BioseqPtr   bsp;
  SeqDescrPtr sdp;
  Char        id_txt [128];
  
  if (sep == NULL)
  {
    return;
  }
  if (IS_Bioseq (sep))
  {
    bsp = (BioseqPtr) sep->data.ptrvalue;
  }
  else if (IS_Bioseq_set (sep))
  {
    sep = FindNucSeqEntry (sep);
    if (sep != NULL && IS_Bioseq (sep))
    {
      bsp = (BioseqPtr) sep->data.ptrvalue;
    }
  }
  
  if (bsp == NULL)
  {
    return;
  }
  
  SeqIdWrite (SeqIdFindWorst (bsp->id), id_txt, PRINTID_REPORT, sizeof (id_txt));
  
  for (sdp = bsp->descr; sdp != NULL && sdp->choice != Seq_descr_title; sdp = sdp->next)
  {
  }

  if (sdp == NULL)
  {
    AddSeqIDAndValueToRowList (id_txt, NULL, row_list);
  }
  else
  {
    AddSeqIDAndValueToRowList (id_txt, sdp->data.ptrvalue, row_list);
  }  
}

static void EditOrganismColumn (SourceAssistantPtr sap, SeqEntryPtr seq_list)
{
  WindoW     w;
  GrouP      h, instr_grp, c;
  DialoG     dlg;
  ValNodePtr row_list, row_vnp, col_vnp;
  ModalAcceptCancelData  acd;
  ButtoN                 b;
  SeqEntryPtr            sep;
  Int4                   j;
  
  if (sap == NULL && seq_list == NULL)
  {
    return;
  }
  
  w = MovableModalWindow (-20, -13, -10, -10, "Organisms", NULL);
  h = HiddenGroup(w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
  
  dlg = MultiOrganismSelectionDialog (h);
    
  instr_grp = MakeSourceInstructionGroup (h);
  
  row_list = NULL;
  if (sap == NULL)
  {
    for (sep = seq_list; sep != NULL; sep = sep->next)
    {
      AddSeqIDAndValueFromSeqEntryToRowList (sep, &row_list);
    }
  }
  else
  {
    for (j = 0; j < sap->num_deflines; j++)
    {
      AddSeqIDAndValueToRowList (sap->id_list[j], sap->defline_list[j],
                                 &row_list);
    }
  }
  PointerToDialog (dlg, row_list);
  row_list = FreeTableDisplayRowList (row_list);
  
  c = HiddenGroup (h, 2, 0, NULL);
  b = PushButton (c, "Accept", ModalAcceptButton);
  SetObjectExtra (b, &acd, NULL);
  b = PushButton (c, "Cancel", ModalCancelButton);
  SetObjectExtra (b, &acd, NULL);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) dlg, (HANDLE) instr_grp,
                              (HANDLE) c, (HANDLE) NULL);

  Show (w);
  Select (w);
  acd.accepted = FALSE;
  acd.cancelled = FALSE;
  while (!acd.accepted && ! acd.cancelled)
  {
    ProcessExternalEvent ();
    Update ();
  }
  ProcessAnEvent ();
  if (acd.cancelled)
  {
    Remove (w);
    return;
  }
  else
  {
    row_list = DialogToPointer (dlg);
    if (sap == NULL)
    {
      for (sep = seq_list, j = 0, row_vnp = row_list;
           sep != NULL, row_vnp != NULL; 
           sep = sep->next, j++, row_vnp = row_vnp->next)
      {
        col_vnp = row_vnp->data.ptrvalue;
        /* skip sequence ID */
        if (col_vnp != NULL)
        {
          col_vnp = col_vnp->next;
        }
        /* add tax name */
        if (col_vnp != NULL)
        {
          ApplyOneModToSeqEntry (sep, "organism", col_vnp->data.ptrvalue);
          col_vnp = col_vnp->next;
        }
        
        /* add location */
        if (col_vnp != NULL)
        {
          ApplyOneModToSeqEntry (sep, "location", col_vnp->data.ptrvalue);
          col_vnp = col_vnp->next;
        }
        
        /* add genetic code */
        if (col_vnp != NULL)
        {
          ApplyOneModToSeqEntry (sep, "genetic_code", col_vnp->data.ptrvalue);
          col_vnp = col_vnp->next;
        }
        
        /* add genetic code comment */
        if (col_vnp != NULL)
        {
          ApplyOneModToSeqEntry (sep, "genetic_code_comment", col_vnp->data.ptrvalue);
          col_vnp = col_vnp->next;
        }
        
      }
    }
    else
    {
      for (j = 0, row_vnp = row_list;
           j < sap->num_deflines, row_vnp != NULL; 
           j++, row_vnp = row_vnp->next)
      {
        col_vnp = row_vnp->data.ptrvalue;
        /* skip sequence ID */
        if (col_vnp != NULL)
        {
          col_vnp = col_vnp->next;
        }
        /* add tax name */
        if (col_vnp != NULL)
        {
          ReplaceValueInDefLines (sap, "organism", col_vnp->data.ptrvalue, j);
          col_vnp = col_vnp->next;
        }
        
        /* add location */
        if (col_vnp != NULL)
        {
          ReplaceValueInDefLines (sap, "location", col_vnp->data.ptrvalue, j);
          col_vnp = col_vnp->next;
        }
        
        /* add genetic code */
        if (col_vnp != NULL)
        {
          ReplaceValueInDefLines (sap, "genetic_code", col_vnp->data.ptrvalue, j);
          col_vnp = col_vnp->next;
        }
        /* add genetic code comment */
        if (col_vnp != NULL)
        {
          ReplaceValueInDefLines (sap, "genetic_code_comment", col_vnp->data.ptrvalue, j);
          col_vnp = col_vnp->next;
        }
      }
    }
    row_list = FreeTableDisplayRowList (row_list);
    UpdateOrgModDlg (sap);  
    Remove (w);
  }

}

typedef struct clearcolumnvaluesdata
{
  SourceAssistantPtr sap;
  SeqEntryPtr        seq_list;
  TagListPtr         tlp;
  CharPtr            lookfor;
  CharPtr            mod_name;
  Int2               seqPackage;  
} ClearColumnValuesData, PNTR ClearColumnValuesPtr;

static void ClearColumnValues (ButtoN b)
{ 
  BioseqPtr            bsp;
  Char                 id_txt [128];
  ValNodePtr           head, vnp;
  SeqEntryPtr          sep, nsep;
  ClearColumnValuesPtr ccvp;
  Int4                 j;
  
  ccvp = (ClearColumnValuesPtr) GetObjectExtra (b);
  if (ccvp == NULL || (ccvp->sap == NULL && ccvp->seq_list == NULL) || ccvp->tlp == NULL)
  {
    return;
  }
  
  head = NULL;
  if (ccvp->sap == NULL)
  {
    for (sep = ccvp->seq_list; sep != NULL; sep = sep->next)
    {
      if (IS_Bioseq (sep))
      {
        bsp = (BioseqPtr) sep->data.ptrvalue;
      }
      else if (IS_Bioseq_set (sep))
      {
        nsep = FindNucSeqEntry (sep);
        if (nsep != NULL && IS_Bioseq (nsep))
        {
          bsp = (BioseqPtr) nsep->data.ptrvalue;
        }
      }
  
      if (bsp == NULL)
      {
        continue;
      }
  
      SeqIdWrite (SeqIdFindWorst (bsp->id), id_txt, PRINTID_REPORT, sizeof (id_txt));
  
      AddSeqIDAndValueToTagList (id_txt, NULL,
                                 ccvp->lookfor, ccvp->mod_name, ccvp->seqPackage, 
                                 &head, NULL);
    }
  }
  else
  {
    for (j = 0; j < ccvp->sap->num_deflines; j++)
    {
      AddSeqIDAndValueToTagList (ccvp->sap->id_list[j], NULL,
                                 ccvp->lookfor, ccvp->mod_name, ccvp->seqPackage, 
                                 &head, NULL);
    }
  }

  SendMessageToDialog (ccvp->tlp->dialog, VIB_MSG_RESET);
  ccvp->tlp->vnp = head;
  SendMessageToDialog (ccvp->tlp->dialog, VIB_MSG_REDRAW);
  for (j = 0, vnp = ccvp->tlp->vnp; vnp != NULL; j++, vnp = vnp->next) {
  }
  ccvp->tlp->max = MAX ((Int2) 0, (Int2) (j - ccvp->tlp->rows));
  CorrectBarMax (ccvp->tlp->bar, ccvp->tlp->max);
  CorrectBarPage (ccvp->tlp->bar, ccvp->tlp->rows - 1, ccvp->tlp->rows - 1);
  if (ccvp->tlp->max > 0) {
    SafeShow (ccvp->tlp->bar);
  } else {
    SafeHide (ccvp->tlp->bar);
  }
  SendMessageToDialog (ccvp->tlp->dialog, VIB_MSG_ENTER);
}

static void 
EditOrgModColumn 
(CharPtr            mod_name,
 SourceAssistantPtr sap, 
 SeqEntryPtr        seq_list,
 Int2               seqPackage)
{
  WindoW                 w;
  GrouP                  h, k, g, c;
  DialoG                 dlg;
  TagListPtr             tlp;
  Int4                   j;
  ValNodePtr             head = NULL;
  Char                   lookfor [128];
  ModalAcceptCancelData  acd;
  ButtoN                 b;
  ValNodePtr             found_modifiers = NULL, vnp;
  Boolean                is_nontext;
  CharPtr                new_value = NULL;
  Int4                   rows_shown;
  SeqEntryPtr            sep;
  Int4                   mod_type;
  EnumFieldAssocPtr      gencode_alist = NULL;
  EnumFieldAssocPtr      edit_alists [2];
  GrouP                  instr_grp = NULL;
  ClearColumnValuesData  ccvd;
  ButtoN                 clear_btn;
  CharPtr                mod_label;
  
  if (StringHasNoText (mod_name) || (sap == NULL && seq_list == NULL))
  {
    return;
  }
  
  if (StringICmp (mod_name, "moltype") == 0)
  {
    mod_label = StringSave ("molecule");
  }
  else
  {
    mod_label = StringSave (mod_name);
  }

  is_nontext = IsNonTextModifier (mod_name);
  mod_type = GetModifierType (mod_name);
  
  if (mod_type == eModifierType_Organism
      || mod_type == eModifierType_Location
      || mod_type == eModifierType_GeneticCode)
  {
    EditOrganismColumn (sap, seq_list);
    return;
  }
  
  w = MovableModalWindow (-20, -13, -10, -10, mod_label, NULL);
  h = HiddenGroup(w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
  
  k = HiddenGroup (h, 1, 0, NULL);
  g = HiddenGroup (k, 2, 0, NULL);
  StaticPrompt (g, "SeqID", 7 * stdCharWidth, 0, programFont, 'l');
  StaticPrompt (g, mod_label, 18 * stdCharWidth, 0, programFont, 'l');
  
  mod_label = MemFree (mod_label);
  
  if (sap == NULL)
  {
    rows_shown = 0;
    sep = seq_list;
    while (sep != NULL)
    {
      rows_shown++;
      sep = sep->next;
    }   
  }
  else
  {
    rows_shown = sap->num_deflines;
  }
  rows_shown = MIN (rows_shown, 5);
  
  if (is_nontext)
  {
    dlg = CreateTagListDialogEx (k, rows_shown, 2, 2,
                                 nontextmodedittypes, modedit_widths,
                                 nontextmodedit_alists, TRUE, TRUE, NULL, NULL);
    instr_grp = MakeNontextInstructionGroup (h);
  }
  else if (mod_type == eModifierType_Location)
  {
    dlg = CreateTagListDialogEx (k, rows_shown, 2, 2,
                                 locationmodedittypes, modedit_widths,
                                 locationmodedit_alists, TRUE, TRUE, NULL, NULL);
    instr_grp = MakeLocationInstructionGroup (h);
  }
  else if (mod_type == eModifierType_NucGeneticCode
           || mod_type == eModifierType_MitoGeneticCode)
  {
    gencode_alist = BuildGeneticCodeEnum ();
    edit_alists [0] = NULL;
    edit_alists [1] = gencode_alist;
    dlg = CreateTagListDialogEx (k, rows_shown, 2, 2,
                                 locationmodedittypes, modedit_widths,
                                 edit_alists, TRUE, TRUE, NULL, NULL);
    instr_grp = MakeGeneticCodeInstructionGroup (h);
  }
  else if (mod_type == eModifierType_MolType)
  {
    edit_alists [0] = NULL;
    if (seqPackage == SEQ_PKG_GENOMICCDNA)
    {
      edit_alists [1] = biomol_nucGen_alist;
    }
    else
    {
      edit_alists [1] = biomol_nucX_alist;
    }
    dlg = CreateTagListDialogEx (k, rows_shown, 2, 2,
                                 locationmodedittypes, modedit_widths,
                                 edit_alists, TRUE, TRUE, NULL, NULL);
  }
  else if (mod_type == eModifierType_Topology)
  {
    edit_alists [0] = NULL;
    edit_alists [1] = topology_nuc_alist;
    dlg = CreateTagListDialogEx (k, rows_shown, 2, 2,
                                 locationmodedittypes, modedit_widths,
                                 edit_alists, TRUE, TRUE, NULL, NULL);
  }
  else if (mod_type == eModifierType_Organism)
  {
    MultiOrganismSelectionDialog (k);
    
    
    dlg = CreateTagListDialogEx (k, rows_shown, 2, 2,
                                 modedit_types, modedit_widths,
                                 NULL, TRUE, TRUE, NULL, NULL);
    instr_grp = MakeSourceInstructionGroup (h);
  }
  else
  {
    dlg = CreateTagListDialogEx (k, rows_shown, 2, 2,
                                 modedit_types, modedit_widths,
                                 NULL, TRUE, TRUE, NULL, NULL);

  }
  
  tlp = (TagListPtr) GetObjectExtra (dlg);  
  if (tlp == NULL) return;

  MakeSearchStringFromAlist (lookfor, mod_name);

  head = NULL;
  if (sap == NULL)
  {
    for (sep = seq_list; sep != NULL; sep = sep->next)
    {
      AddSeqIDAndValueFromSeqEntryToTagList (sep, lookfor, mod_name, seqPackage,
                                             &head, &found_modifiers);
    }
  }
  else
  {
    for (j = 0; j < sap->num_deflines; j++)
    {
      AddSeqIDAndValueToTagList (sap->id_list[j], sap->defline_list[j],
                                 lookfor, mod_name, seqPackage, 
                                 &head, &found_modifiers);
    }
  }
  found_modifiers = ValNodeFreeData (found_modifiers);

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
  
  ccvd.sap = sap;
  ccvd.seq_list = seq_list;
  ccvd.tlp = tlp;
  ccvd.lookfor = lookfor;
  ccvd.mod_name = mod_name;
  ccvd.seqPackage = seqPackage; 
  
  clear_btn = PushButton (h, "Clear All Values", ClearColumnValues);
  SetObjectExtra (clear_btn, &ccvd, NULL);
  
  c = HiddenGroup (h, 2, 0, NULL);
  b = PushButton (c, "Accept", ModalAcceptButton);
  SetObjectExtra (b, &acd, NULL);
  b = PushButton (c, "Cancel", ModalCancelButton);
  SetObjectExtra (b, &acd, NULL);
  
  if (instr_grp == NULL)
  {
    AlignObjects (ALIGN_CENTER, (HANDLE) k, (HANDLE) clear_btn, (HANDLE) c, (HANDLE) NULL);
  }
  else
  {
    AlignObjects (ALIGN_CENTER, (HANDLE) k, (HANDLE) instr_grp, (HANDLE) clear_btn,
                                (HANDLE) c, (HANDLE) NULL);
  }

  Show (w);
  Select (w);
  acd.accepted = FALSE;
  acd.cancelled = FALSE;
  while (!acd.accepted && ! acd.cancelled)
  {
    ProcessExternalEvent ();
    Update ();
  }
  ProcessAnEvent ();
  if (acd.cancelled)
  {
    Remove (w);
    return;
  }
  else
  {
    tlp = GetObjectExtra (dlg);
    if (sap == NULL)
    {
      for (sep = seq_list, j = 0; sep != NULL; sep = sep->next, j++)
      {
        new_value = GetValueFromTagList (tlp, j, is_nontext, mod_type,
                                         seqPackage, gencode_alist);
        ApplyOneModToSeqEntry (sep, mod_name, new_value);
        new_value = MemFree (new_value);
      }
    }
    else
    {
      for (j = 0; j < sap->num_deflines; j++)
      {
        new_value = GetValueFromTagList (tlp, j, is_nontext, mod_type,
                                         seqPackage, gencode_alist);
        ReplaceValueInDefLines (sap, mod_name, new_value, j);
        new_value = MemFree (new_value);
     }
    }
    UpdateOrgModDlg (sap);  
    Remove (w);
  }
  gencode_alist = FreeGeneticCodeEnum (gencode_alist);
}

static void UpdateGeneticCodesForSourceAssistant (SourceAssistantPtr sap)
{
  Int4       j;
  Char       taxname [256];
  Char       location [256];
  CharPtr    gcode_name;
  Int4       gcode;
  ValNodePtr gencodelist;
  
  if (sap == NULL)
  {
    return;
  }

  gencodelist = GetGeneticCodeValNodeList ();
  
  for (j = 0; j < sap->num_deflines; j++)
  {
    if (!LookForSearchString (sap->defline_list[j],
                             "[organism=",
                            taxname, 
                            sizeof (taxname) - 1))
    {
      taxname [0] = 0;
    }
    if (!LookForSearchString (sap->defline_list[j],
                              "[location=",
                              location,
                              sizeof (location) - 1)
        || StringHasNoText (location))
    {
      sprintf (location, "genomic");
    }
    gcode = GetGeneticCodeForTaxNameAndLocation (taxname, location);
    if (gcode > 0)
    {
      gcode_name = GeneticCodeStringFromIntAndList (gcode, gencodelist);
      ReplaceValueInDefLines (sap, "genetic_code", gcode_name, j);
    }
  }
  gencodelist = ValNodeFreeData (gencodelist);
}

static void UpdateGeneticCodesForSeqEntryList (SeqEntryPtr seq_list)
{
  SeqEntryPtr sep;
  CharPtr     taxname, location, gcode_name;
  Int4        gcode;
  ValNodePtr  gencodelist;
  
  if (seq_list == NULL)
  {
    return;
  }
  
  gencodelist = GetGeneticCodeValNodeList ();

  for (sep = seq_list; sep != NULL; sep = sep->next)
  {
    taxname = GetModValueFromSeqEntry (sep, "organism");
    location = GetModValueFromSeqEntry (sep, "location");
    if (StringHasNoText (location))
    {
      location = MemFree (location);
      location = StringSave ("genomic");
    }
    gcode = GetGeneticCodeForTaxNameAndLocation (taxname, location);
    taxname = MemFree (taxname);
    location = MemFree (location);
    if (gcode > 0)
    {
      gcode_name = GeneticCodeStringFromIntAndList (gcode, gencodelist);
      ApplyOneModToSeqEntry (sep, "genetic_code", gcode_name);
    }
  }
  gencodelist = ValNodeFreeData (gencodelist);
}

static Boolean 
ContinueWithAutopopulatedGeneticCodes 
(SeqEntryPtr        seq_list,
 SourceAssistantPtr sap,
 ValNodePtr         row_list,
 Int4               affected_row)
{
  ValNodePtr  autopop_list = NULL, already_have = NULL;
  SeqEntryPtr sep, nuc_sep;
  BioseqPtr   bsp;
  Char        id_txt [128];
  Int4        j;
  CharPtr     list_msg = NULL;
  Int4        num_sequences = 0;
  Boolean     rval = TRUE;
  CharPtr     taxname, location, gcode_name;
  Int4        gcode;
  ValNodePtr  row_vnp, col_vnp;
  
  if (seq_list == NULL && sap == NULL && row_list == NULL)
  {
    return FALSE;
  }
  
  if (seq_list != NULL)
  {
    for (sep = seq_list, j = 0; sep != NULL; sep = sep->next, j++)
    {
      if (affected_row != -1 && affected_row != j)
      {
        continue;
      }
      bsp = NULL;
      if (IS_Bioseq (sep))
      {
        bsp = (BioseqPtr) sep->data.ptrvalue;
      }
      else if (IS_Bioseq_set (sep))
      {
        nuc_sep = FindNucSeqEntry (sep);
        if (nuc_sep != NULL && IS_Bioseq (nuc_sep))
        {
          bsp = (BioseqPtr) nuc_sep->data.ptrvalue;
        }
      }
      if (bsp == NULL)
      {
        continue;
      }
      
      taxname = GetModValueFromSeqEntry (sep, "organism");
      location = GetModValueFromSeqEntry (sep, "location");
      if (StringHasNoText (location))
      {
        location = MemFree (location);
        location = StringSave ("genomic");
      }
      gcode = GetGeneticCodeForTaxNameAndLocation (taxname, location);
      taxname = MemFree (taxname);
      location = MemFree (location);
      if (gcode > 0)
      {
        if (bsp != NULL)
        {
          SeqIdWrite (SeqIdFindWorst (bsp->id), id_txt, PRINTID_REPORT,
                      sizeof (id_txt));
          ValNodeAddPointer (&autopop_list, 0, StringSave (id_txt));
        }
      }
      else
      {
        gcode_name = GetModValueFromSeqEntry (sep, "genetic_code");
        if (!StringHasNoText (gcode_name))
        {
          SeqIdWrite (SeqIdFindWorst (bsp->id), id_txt, PRINTID_REPORT,
                      sizeof (id_txt));
          ValNodeAddPointer (&already_have, 0, StringSave (id_txt));
        }
        gcode_name = MemFree (gcode_name);
      }
      num_sequences++;
    }
  }
  else if (sap != NULL)
  {
    for (j = 0; j < sap->num_deflines; j++)
    {
      if (affected_row != -1 && affected_row != j)
      {
        continue;
      }
      taxname = GetValueFromTitle ("organism", sap->defline_list [j]);
      location = GetValueFromTitle ("location", sap->defline_list [j]);
      gcode = GetGeneticCodeForTaxNameAndLocation (taxname, location);
      taxname = MemFree (taxname);
      location = MemFree (location);
      if (gcode > 0)
      {
        ValNodeAddPointer (&autopop_list, 0, StringSave (sap->id_list[j]));
      }
      else
      {
        gcode_name = GetValueFromTitle ("genetic_code", sap->defline_list [j]);
        if (!StringHasNoText (gcode_name))
        {
          ValNodeAddPointer (&already_have, 0, StringSave (sap->id_list[j]));
        }
        gcode_name = MemFree (gcode_name);
      }
    }
    num_sequences = sap->num_deflines;
  }
  else if (row_list != NULL)
  {
    num_sequences = 0;
    for (row_vnp = row_list; row_vnp != NULL; row_vnp = row_vnp->next)
    {
      col_vnp = row_vnp->data.ptrvalue;
      if (col_vnp != NULL && col_vnp->next != NULL 
          && col_vnp->next->next != NULL
          && col_vnp->next->next->next != NULL)
      {
        taxname = col_vnp->next->data.ptrvalue;
        location = col_vnp->next->next->data.ptrvalue;
        gcode = GetGeneticCodeForTaxNameAndLocation (taxname, location);
        if (gcode > 0)
        {
          ValNodeAddPointer (&autopop_list, 0, StringSave (col_vnp->data.ptrvalue));
        }
      }
      num_sequences++;
    }
  }
  
  if (autopop_list != NULL)
  {
    list_msg = CreateListMessage ("Sequence", 
               (autopop_list->next == NULL ? 
               " has a genetic code determined by the location and scientific name.  The genetic code for this sequence cannot be edited."
               : " have genetic codes determined by the location and scientific name.  The genetic code for these sequences cannot be edited."),
               autopop_list);
    if (ValNodeLen (autopop_list) == num_sequences || affected_row != -1)
    {
      Message (MSG_ERROR, list_msg);
      rval = FALSE;
    }
    else
    {
      if (ANS_NO == Message (MSG_YN, 
                     "%s  Do you want to edit the genetic code for the remaining sequences?",
                     list_msg))
      {
        rval = FALSE;
      }
    }
    list_msg = MemFree (list_msg);
  }
  autopop_list = ValNodeFreeData (autopop_list);
  
  if (rval && already_have != NULL && affected_row == -1 && num_sequences > 1)
  {
    list_msg = CreateListMessage ("Sequence",
                     (already_have->next == NULL ?
                     " already has a genetic code.  Do you wish to overwrite this value?"
                     : "already have genetic codes.  Do you wish to overwrite these values?"),
                     already_have);                     
    if (ANS_NO == Message (MSG_YN, list_msg))
    {
      rval = FALSE;
    }
    list_msg = MemFree (list_msg);
  }
  return rval;
}

static void 
ApplyOrgModColumnOrCell 
(CharPtr            mod_name,
 CharPtr            suggested_value,
 Int4               row, 
 SourceAssistantPtr sap,
 SeqEntryPtr        seq_list,
 ValNodePtr         row_list,
 Int4               row_list_column,
 Int2               seqPackage)
{
  WindoW                w;
  GrouP                 h, g, c;
  TexT                  text_txt;
  PopuP                 nontext_popup;
  GrouP                 instr_grp = NULL;
  Boolean               found_value = FALSE;
  Int4                  j;
  ValNodePtr            head = NULL, value_vnp;
  ModalAcceptCancelData acd;
  ButtoN                b;
  Boolean               is_nontext;
  CharPtr               new_value = NULL;
  CharPtr               title;
  CharPtr               all_seq_fmt = "%s (all sequences)";
  CharPtr               one_seq_fmt = "%s (Seq_ID %s)";
  ValNode               vn;
  DialoG                org_dlg;
  DialoG                strvalue_dlg;
  Int4                  num_sequences = 0;
  SeqEntryPtr           sep, nuc_sep;
  Char                  id_txt[128];
  BioseqPtr             bsp = NULL;
  Int2                  mod_type;
  ValNodePtr            gencodelist = NULL;
  ValNodePtr            row_vnp = NULL, col_vnp;
  Int4                  row_num, col_num;
  CharPtr               mod_label;
  
  if (StringHasNoText (mod_name) || row < -1)
  {
    return;
  }
  if (sap == NULL && seq_list == NULL && row_list == NULL)
  {
    return;
  }
  
  if (seq_list != NULL)
  {
    for (sep = seq_list; sep != NULL; sep = sep->next)
    {
      if (num_sequences == row)
      {
        if (IS_Bioseq (sep))
        {
          bsp = (BioseqPtr) sep->data.ptrvalue;
        }
        else if (IS_Bioseq_set (sep))
        {
          nuc_sep = FindNucSeqEntry (sep);
          if (nuc_sep != NULL && IS_Bioseq (nuc_sep))
          {
            bsp = (BioseqPtr) nuc_sep->data.ptrvalue;
          }
        }
  
        if (bsp == NULL)
        {
          return;
        }
      }
      num_sequences++;
    }
  }
  else if (sap != NULL)
  {
    num_sequences = sap->num_deflines;
  }
  else if (row_list != NULL)
  {
    num_sequences = ValNodeLen (row_list);
  }
  
  if (row >= num_sequences)
  {
    return;
  }
 
  is_nontext = IsNonTextModifier (mod_name);
  mod_type = GetModifierType (mod_name);
 
  if (StringICmp (mod_name, "moltype") == 0)
  {
    mod_label = StringSave ("molecule");
  }
  else
  {
    mod_label = StringSave (mod_name);
  }

  if (row == -1)
  {
    title = (CharPtr) MemNew ((StringLen (mod_label) + StringLen (all_seq_fmt)) * sizeof (Char));
    sprintf (title, all_seq_fmt, mod_label);
  }
  else
  {
    if (seq_list != NULL)
    {
      if (bsp == NULL)
      {
        return;
      }
      SeqIdWrite (SeqIdFindWorst (bsp->id), id_txt, PRINTID_REPORT, sizeof (id_txt));
    }
    else if (sap != NULL)
    {
      StringNCpy (id_txt, sap->id_list[row], sizeof (id_txt) - 1);
      id_txt[sizeof(id_txt) - 1] = 0;
    }
    else if (row_list != NULL)
    {
      for (row_vnp = row_list, row_num = 0;
           row_vnp != NULL && row_num < row;
           row_vnp = row_vnp->next, row_num++)
      {
        
      }
      if (row_vnp != NULL)
      {
        col_vnp = row_vnp->data.ptrvalue;
        if (col_vnp != NULL)
        {
          StringNCpy (id_txt, col_vnp->data.ptrvalue, sizeof (id_txt) - 1);
          id_txt[sizeof(id_txt) - 1] = 0;
        }
        else
        {
          return;
        }
      }
      else
      {
        return;
      }
    }
    title = (CharPtr) MemNew ((StringLen (mod_label) 
                               + StringLen (one_seq_fmt)
                               + StringLen (id_txt)) * sizeof (Char));
    sprintf (title, one_seq_fmt, mod_label, id_txt);
  }
  
  w = MovableModalWindow (-20, -13, -10, -10, title, NULL);
  title = MemFree (title);
  
  h = HiddenGroup(w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
  
  g = HiddenGroup (h, 1, 0, NULL);

  if (is_nontext)
  {
    nontext_popup = PopupList (g, TRUE, NULL);
    PopupItem (nontext_popup, "FALSE");
    PopupItem (nontext_popup, "TRUE");
    if (StringICmp (suggested_value, "TRUE") == 0)
    {
      SetValue (nontext_popup, 2);
    }
    else
    {
      SetValue (nontext_popup, 1);
    }
    instr_grp = MakeNontextInstructionGroup (h);
  }
  else if (mod_type == eModifierType_Location)
  {
    if (StringHasNoText (suggested_value))
    {
      vn.choice = 1;
      vn.data.ptrvalue = "genomic";
    }
    else
    {
      vn.choice = GetValForEnumName (biosource_genome_simple_alist, suggested_value);
      vn.data.ptrvalue = suggested_value;
    }
    vn.next = NULL;
    strvalue_dlg = EnumAssocSelectionDialog (g, biosource_genome_simple_alist,
                                             "location", FALSE, NULL, NULL);
    PointerToDialog (strvalue_dlg, &vn);
    instr_grp = MakeLocationInstructionGroup (h);
  }
  else if (mod_type == eModifierType_Organism)
  {
    org_dlg = OrganismSelectionDialog (g, suggested_value);
    instr_grp = MakeSourceInstructionGroup (h);
  }
  else if (mod_type == eModifierType_NucGeneticCode
           || mod_type == eModifierType_MitoGeneticCode
           || mod_type == eModifierType_GeneticCode)
  {
    gencodelist = GetGeneticCodeValNodeList ();
    if (StringHasNoText (suggested_value))
    {
      vn.choice = 0;
    }
    else if (isdigit (suggested_value[0]))
    {
      vn.choice = atoi (suggested_value);
    }
    else
    {
      vn.choice = GeneticCodeFromStringAndList (suggested_value, gencodelist);
    }
    vn.next = NULL;
    vn.data.ptrvalue = suggested_value;
    strvalue_dlg = ValNodeSelectionDialog (g, gencodelist, 6,
                                        ValNodeStringName,
                                        ValNodeSimpleDataFree,
                                        ValNodeStringCopy,
                                        ValNodeChoiceMatch,
                                        "genetic code",
                                        NULL, NULL, FALSE);
    PointerToDialog (strvalue_dlg, &vn);
    instr_grp = MakeGeneticCodeInstructionGroup (h);
  }
  else if (mod_type == eModifierType_MolType)
  {
    if (StringHasNoText (suggested_value))
    {
      if (PackageTypeIsSet (seqPackage) || seqPackage == SEQ_PKG_GENOMICCDNA) 
      {
        vn.choice = 253;
      }
      else
      {
        vn.choice = 254;
      }
    }
    else if (isdigit (suggested_value[0]))
    {
      vn.choice = atoi (suggested_value);
    }
    else
    {
      vn.choice = MolTypeFromString (suggested_value);
    }
    vn.next = NULL;
    vn.data.ptrvalue = suggested_value;
    if (seqPackage == SEQ_PKG_GENOMICCDNA)
    {
      strvalue_dlg = EnumAssocSelectionDialog (g, biomol_nucGen_alist,
                                               "moltype", FALSE, NULL, NULL);
    }
    else
    {
      strvalue_dlg = EnumAssocSelectionDialog (g, biomol_nucX_alist,
                                               "moltype", FALSE, NULL, NULL);
    }
    PointerToDialog (strvalue_dlg, &vn);
  }
  else if (mod_type == eModifierType_Topology)
  {
    if (StringHasNoText (suggested_value))
    {
      vn.choice = 1;
    }
    else if (isdigit (suggested_value[0]))
    {
      vn.choice = atoi (suggested_value);
    }
    else
    {
      vn.choice = TopologyFromString (suggested_value);
    }
    vn.next = NULL;
    vn.data.ptrvalue = suggested_value;
    strvalue_dlg = EnumAssocSelectionDialog (g, topology_nuc_alist,
                                               "topology", FALSE, NULL, NULL);
    PointerToDialog (strvalue_dlg, &vn);
  }
  else
  {
    text_txt = DialogText (g, suggested_value, 20, NULL);
    if (mod_type == eModifierType_GeneticCodeComment)
    {
      instr_grp = MakeGeneticCodeCommentInstructionGroup (h);
    }
  }
    
  c = HiddenGroup (h, 2, 0, NULL);
  b = PushButton (c, "Accept", ModalAcceptButton);
  SetObjectExtra (b, &acd, NULL);
  b = PushButton (c, "Cancel", ModalCancelButton);
  SetObjectExtra (b, &acd, NULL);

  if (instr_grp == NULL)
  {
    AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, (HANDLE) NULL);
  }
  else
  {
    AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) instr_grp, (HANDLE) c, (HANDLE) NULL);
  }

  mod_label = MemFree (mod_label);

  Show (w);
  Select (w);
  acd.accepted = FALSE;
  acd.cancelled = FALSE;
  while (!acd.accepted && ! acd.cancelled)
  {
    ProcessExternalEvent ();
    Update ();
  }
  ProcessAnEvent ();
  if (acd.cancelled)
  {
    Remove (w);
    return;
  }
  else
  {
    if (is_nontext)
    {
      if (GetValue (nontext_popup) == 2)
      {
        new_value = StringSave ("2");
      }
    }
    else if (mod_type == eModifierType_Location
             || mod_type == eModifierType_NucGeneticCode
             || mod_type == eModifierType_MitoGeneticCode
             || mod_type == eModifierType_GeneticCode
             || mod_type == eModifierType_MolType
             || mod_type == eModifierType_Topology)
    {
      value_vnp = DialogToPointer (strvalue_dlg);
      new_value = value_vnp->data.ptrvalue;
      if (mod_type == eModifierType_Location)
      {
        StringToLower (new_value);
      }
      value_vnp = ValNodeFree (value_vnp);
      if (mod_type == eModifierType_MolType
          && StringICmp (new_value, "mRNA [cDNA]") == 0)
      {
        new_value = MemFree (new_value);
        new_value = StringSave ("mRNA");
      }
    }
    else if (mod_type == eModifierType_Organism)
    {
      new_value = DialogToPointer (org_dlg);
    }
    else
    {
      new_value = SaveStringFromText (text_txt);
    }
    
    if (seq_list != NULL)
    {
      for (sep = seq_list, j = 0;
           sep != NULL;
           sep = sep->next, j++)
      {
        if (j == row || row == -1)
        {
          ApplyOneModToSeqEntry (sep, mod_name, new_value);
        }
      }
      if (mod_type == eModifierType_Organism 
          || mod_type == eModifierType_Location
          || mod_type == eModifierType_GeneticCode)
      {
        UpdateGeneticCodesForSeqEntryList (seq_list);
      }
    }
    else if (sap != NULL)
    {
      for (j = 0; j < sap->num_deflines; j++)
      {
        if (j == row || row == -1)
        {
          ReplaceValueInDefLines (sap, mod_name, new_value, j);
        }
      }
      if (mod_type == eModifierType_Organism 
          || mod_type == eModifierType_Location
          || mod_type == eModifierType_GeneticCode)
      {
        UpdateGeneticCodesForSourceAssistant (sap);
      }
    }
    else if (row_list != NULL)
    {
      if (row < 0)
      {
        SetRowListColumn (row_list, row_list_column, new_value);
      }
      else if (row_vnp != NULL)
      {
        for (col_vnp = row_vnp->data.ptrvalue, col_num = 0;
             col_vnp != NULL && col_num < row_list_column;
             col_vnp = col_vnp->next, col_num++)
        {
        }
        if (col_vnp != NULL)
        {
          col_vnp->data.ptrvalue = MemFree (col_vnp->data.ptrvalue);
          col_vnp->data.ptrvalue = StringSave (new_value);
        }
      }
    }
    new_value = MemFree (new_value);
    UpdateOrgModDlg (sap);
    Remove (w);
  }
}

static void 
ApplyOrgModColumn 
(CharPtr mod_name,
 CharPtr suggested_value,
 SourceAssistantPtr sap)
{
  Int4    mod_type;
  Boolean is_nontext;
  
  mod_type = GetModifierType (mod_name);
  is_nontext = IsNonTextModifier (mod_name);
  
  if (mod_type == eModifierType_GeneticCode)
  {
    if (! ContinueWithAutopopulatedGeneticCodes (NULL, sap, NULL, -1))
    {
      return;
    }
  }
  else if (!StringHasNoText (suggested_value)  && ! IsNonTextModifier (mod_name)
      && sap->num_deflines > 1)
  {
    if (ANS_YES != Message (MSG_YN, "Warning!  Some sequences already contain "
                            "a value for %s.  Are you sure you want to "
                            "overwrite these values?", mod_name))
    {
      return;
    }
  }
  ApplyOrgModColumnOrCell (mod_name, suggested_value, -1, sap, NULL, NULL, 0, sap->seqPackage);
}

static void ApplyEditOrgModColumnBtn (ButtoN b, Boolean apply)
{
  SourceAssistantPtr sap;
  Int2               val;
  CharPtr            mod_name;
  CharPtr            suggested_value;

  sap = (SourceAssistantPtr) GetObjectExtra (b);
  if (sap == NULL) return;
  val = GetValue (sap->modType);
  if (val > 0) {
    if (val == 1)
    {
      mod_name = "organism";
    }
    else if (val == 2)
    {
      mod_name = "location";
    }
    else
    {
      mod_name = GetModifierPopupPositionName (val - 1);
    }
    if (apply)
    {
      if (IsNonTextModifier (mod_name))
      {
        suggested_value = StringSave ("TRUE");
      }
      else
      {
        suggested_value = GetFirstDeflineValue (sap, mod_name);
      }
      ApplyOrgModColumn (mod_name, suggested_value, sap);
      suggested_value = MemFree (suggested_value);
    }
    else
    {
      EditOrgModColumn (mod_name, sap, NULL, sap->seqPackage);
    }
  }
}

static void ApplyOrgModColBtn (ButtoN b)
{
  ApplyEditOrgModColumnBtn (b, TRUE);
}

static void EditOrgModColBtn (ButtoN b)
{
  ApplyEditOrgModColumnBtn (b, FALSE);
}

static void SourceAssistantImportModsBtn (ButtoN b)
{
  SourceAssistantPtr sap;

  sap = (SourceAssistantPtr) GetObjectExtra (b);
  if (sap == NULL) return;

  if (ImportOrganismModifiersForSourceAssistant (sap->num_deflines, 
                                                 sap->id_list, 
                                                 sap->defline_list))
  {
    UpdateOrgModDlg (sap);
  }
}

static void SourceAssistantClearAllModifiers (ButtoN b)
{
  SourceAssistantPtr sap;
  Int4               j;
  ValNodePtr         found_modifiers = NULL, vnp;

  sap = (SourceAssistantPtr) GetObjectExtra (b);
  if (sap == NULL) return;
  
  if (Message (MSG_YN, 
      "Are you sure you want to remove all of the source qualifiers from all of your sequences?")
      == ANS_NO)
  {
    return;
  }

  for (j = 0; j < sap->num_deflines; j++)
  {
    found_modifiers = BuildModifierTypeList (found_modifiers, sap->defline_list[j]);
  }
  
  for (j = 0; j < sap->num_deflines; j++)
  {
    for (vnp = found_modifiers; vnp != NULL; vnp = vnp->next)
    { 
      if (StringICmp (vnp->data.ptrvalue, "genetic_code") == 0
          || StringICmp (vnp->data.ptrvalue, "organism") == 0
          || StringICmp (vnp->data.ptrvalue, "location") == 0
          || StringICmp (vnp->data.ptrvalue, "genetic_code_comment") == 0
          || StringICmp (vnp->data.ptrvalue, "moltype") == 0
          || StringICmp (vnp->data.ptrvalue, "topology") == 0)
      {
        continue;
      }
      ReplaceValueInDefLines (sap, vnp->data.ptrvalue, NULL, j);
    }
  }
  
  found_modifiers = ValNodeFreeData (found_modifiers);
  UpdateOrgModDlg (sap);
}

static CharPtr  sourceModInstructionsMsg = "\
You may enter data for multiple modifiers. \
The data for each modifier is saved when you use the menu \
to change to a different modifier.";

static void OrgModDblClick (PoinT cell_coord, CharPtr header_text, CharPtr cell_text, Pointer userdata)
{
  SourceAssistantPtr sap;
  Int4               mod_type;
  
  sap = (SourceAssistantPtr) userdata;
  if (sap == NULL)
  {
    return;
  }
  
  mod_type = GetModifierType (header_text);
  
  if (cell_coord.x == 0)
  {
    return;
  }
  else if (cell_coord.y == 0)
  {
    EditOrgModColumn (cell_text, sap, NULL, sap->seqPackage);
  }
  else if (mod_type != eModifierType_GeneticCode 
           || ContinueWithAutopopulatedGeneticCodes (NULL, sap, NULL, cell_coord.y - 1))
  {
    ApplyOrgModColumnOrCell (header_text, cell_text, cell_coord.y - 1, sap, NULL, NULL, 0, sap->seqPackage);
  }
}

static void SourceAssistant (ButtoN b)
{
  SequencesFormPtr    sqfp;
  SeqEntryPtr         seq_list, cntr_sep;
  SourceAssistantData sad;
  CharPtr             ttl;
  BioseqPtr           bsp;
  SeqIdPtr            sip;
  Char                tmp [128];
  WindoW              w;
  GrouP               h, k, g, g2, mod_btn_grp, c;
  ValNodePtr          ttlvnp;
  Int4                i, max_i;
  FastaPagePtr        fpp;
  Int4                doc_width = stdCharWidth * 40;
  ButtoN              export_btn;
  
  sqfp = (SequencesFormPtr) GetObjectExtra (b);
  if (sqfp == NULL) return;
  
  seq_list = GetSeqEntryFromSequencesForm (sqfp);
  sad.num_deflines = 0;
  sad.seqPackage = sqfp->seqPackage;
    
  if (sad.seqPackage == SEQ_PKG_SEGMENTED) 
  {
    sad.num_deflines = 1;    
  }
  else
  {
    sad.num_deflines = ValNodeLen (seq_list);
  }

  if (sad.num_deflines == 0) return;
  sad.defline_list = (CharPtr PNTR) MemNew (sizeof (CharPtr) * sad.num_deflines);
  sad.id_list = (CharPtr PNTR) MemNew (sizeof (CharPtr) * sad.num_deflines);
  if (sad.defline_list == NULL || sad.id_list == NULL)
  {
    sad.defline_list = MemFree (sad.defline_list);
    sad.id_list = MemFree (sad.id_list);
    return;
  }
  
  if (sad.seqPackage == SEQ_PKG_SEGMENTED) 
  {
    i = 0;
    ttl = NULL;
    SeqEntryExplore (seq_list, (Pointer) (&ttl), FindFirstTitle);
    sad.defline_list[i] = StringSaveNoNull (ttl);
     
    bsp = FindNucBioseq (seq_list);
    if (bsp != NULL)
    {
      sip = SeqIdFindWorst (bsp->id);
      SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
    }
    else
    {
      sprintf (tmp, "%d", i);
    }
    sad.id_list [i] = StringSave (tmp);
  }
  else
  {
    for (cntr_sep = seq_list, i = 0;
         cntr_sep != NULL && i < sad.num_deflines;
         cntr_sep = cntr_sep->next, i++)
    {
      ttl = NULL;
      SeqEntryExplore (cntr_sep, (Pointer) (&ttl), FindFirstTitle);
      sad.defline_list[i] = StringSaveNoNull (ttl);
      
      bsp = FindNucBioseq (cntr_sep);
      if (bsp != NULL)
      {
        sip = SeqIdFindWorst (bsp->id);
        SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
      }
      else
      {
        sprintf (tmp, "%d", i);
      }
      sad.id_list [i] = StringSave (tmp);
    }  
  }
  
  sad.done = FALSE;
  sad.cancelled = FALSE;
  modedit_widths [0] = 7;
  modedit_widths [1] = 18;
    
  w = MovableModalWindow (-20, -13, -10, -10, "Specify Source Modifiers", NULL);
  h = HiddenGroup(w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
  
  k = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (k, "Import source modifiers table", 0, popupMenuHeight, programFont, 'l');
  b = PushButton (k, "Select File", SourceAssistantImportModsBtn);
  SetObjectExtra (b, &sad, NULL);
  
  g = NormalGroup (h, -1, 0, "", programFont, NULL);
  g2 = HiddenGroup (g, 2, 0, NULL);
  StaticPrompt (g2, "Select Modifier", 0, popupMenuHeight, programFont, 'l');
  sad.modType = PopupList (g2, TRUE, NULL);
  max_i = GetNumDeflineModifiers ();
      
  PopupItem (sad.modType, "Organism");
  PopupItem (sad.modType, "Location");
  for (i = 0; i < max_i; i++)
  {
    PopupItem (sad.modType, GetModifierPopupPositionName (i + 2));
  }
  SetObjectExtra (sad.modType, &sad, NULL);
  SetValue (sad.modType, 1);
  
  mod_btn_grp = HiddenGroup (g, 2, 0, NULL);
  b = PushButton (mod_btn_grp, "Apply One Value to All", ApplyOrgModColBtn);
  SetObjectExtra (b, &sad, NULL);
  b = PushButton (mod_btn_grp, "Edit Individual Values", EditOrgModColBtn);
  SetObjectExtra (b, &sad, NULL);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) g2, (HANDLE) mod_btn_grp, NULL);
    
  sad.mod_doc = DocumentPanel (h, doc_width, stdLineHeight * 3);
  SetDocAutoAdjust (sad.mod_doc, TRUE);

  sad.orgmod_dlg = TableDisplayDialog (h, doc_width, stdLineHeight * 16, 1, 1,
                                       OrgModDblClick, &sad);
  UpdateOrgModDlg (&sad);  

  export_btn = PushButton (h, "Export Source Modifier Table", SourceAssistantExport);
  SetObjectExtra (export_btn, &sad, NULL);
  
  c = HiddenGroup (h, 3, 0, NULL);
  b = PushButton(c, "OK", SourceAssistantOk);
  SetObjectExtra (b, &sad, NULL);
  b = PushButton (c, "Clear All Source Modifiers", SourceAssistantClearAllModifiers);
  SetObjectExtra (b, &sad, NULL);
  b = PushButton(c, "Cancel", SourceAssistantCancel);
  SetObjectExtra (b, &sad, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) k,
                              (HANDLE) g, 
                              (HANDLE) sad.mod_doc,
                              (HANDLE) sad.orgmod_dlg, 
                              (HANDLE) export_btn, 
                              (HANDLE) c, 
                              NULL);
  
  Show(w); 
  Select (w);
  while (!sad.done)
  {
    ProcessExternalEvent ();
    Update ();
  }
  ProcessAnEvent ();
  if (!sad.cancelled)
  {
    
    /* copy new deflines to sequence list */
    for (cntr_sep = seq_list, i = 0;
         cntr_sep != NULL && i < sad.num_deflines;
         cntr_sep = cntr_sep->next, i++)
    {
      ttlvnp = NULL;
      SeqEntryExplore (cntr_sep, (Pointer) (&ttlvnp), FindFirstSeqDescrTitle);
      if (ttlvnp == NULL) {
        ttlvnp = CreateNewDescriptor (cntr_sep, Seq_descr_title);
      }
      if (ttlvnp != NULL) {
        ttlvnp->data.ptrvalue = MemFree (ttlvnp->data.ptrvalue);
        ttlvnp->data.ptrvalue = sad.defline_list[i];
        sad.defline_list[i] = NULL;
      }
    }
    SeqEntryPtrToSourceTab (sqfp);
    fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
    if (fpp != NULL) 
    {
      Reset (fpp->doc);
      FormatFastaDoc (fpp);
    }
  }
  
  for (i = 0; i < sad.num_deflines; i++)
  {
    sad.defline_list[i] = MemFree (sad.defline_list[i]);
    sad.id_list[i] = MemFree (sad.id_list[i]);
  }
  sad.defline_list = MemFree (sad.defline_list);
  sad.id_list = MemFree (sad.id_list);
  
  Remove (w);
}

static void ApplyOneValueToAllSequencesDialog (ButtoN b, CharPtr mod_name)
{
  WindoW               w;
  SequencesFormPtr     sqfp;
  CharPtr              value;
  SeqEntryPtr          seq_list;
  FastaPagePtr         fpp;
  Int4                 mod_type;
  Boolean              is_nontext;
  
  sqfp = (SequencesFormPtr) GetObjectExtra (b);
  if (sqfp == NULL)
  {
    return;
  }
  
  seq_list = GetSeqEntryFromSequencesForm (sqfp);
  if (seq_list == NULL)
  {
    return;
  }
  
  is_nontext = IsNonTextModifier (mod_name);
  mod_type = GetModifierType (mod_name);
  value = GetFirstModValueFromSeqEntryTitles (seq_list, mod_name);
 
  if (mod_type == eModifierType_GeneticCode)
  {
    if (! ContinueWithAutopopulatedGeneticCodes (seq_list, NULL, NULL, -1))
    {
      value = MemFree (value);
      return;
    }
  }
  else if (!StringHasNoText (value)  && ! is_nontext
      && seq_list->next != NULL)
  {
    if (ANS_YES != Message (MSG_YN, "Warning!  Some sequences already contain "
                            "a value for %s.  Are you sure you want to "
                            "overwrite these values?", mod_name))
    {
      value = MemFree (value);
      return;
    }
  }


  w = ParentWindow ((Nlm_GraphiC) b);
  if (w != (WindoW)sqfp->form)
  {
    Remove (w);
  }

  ApplyOrgModColumnOrCell (mod_name, value, -1, NULL, seq_list, NULL, 0, sqfp->seqPackage);
  value = MemFree (value);
  
  SeqEntryPtrToSourceTab (sqfp);
  fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
  if (fpp != NULL) 
  {
    Reset (fpp->doc);
    FormatFastaDoc (fpp);
  } 
}

static void ApplyModValuesIndividually (ButtoN b, CharPtr mod_name)
{
  SequencesFormPtr     sqfp;
  SeqEntryPtr          seq_list;
  FastaPagePtr         fpp;
  
  sqfp = (SequencesFormPtr) GetObjectExtra (b);
  if (sqfp == NULL)
  {
    return;
  }
  
  seq_list = GetSeqEntryFromSequencesForm (sqfp);
  if (seq_list == NULL)
  {
    return;
  }

  EditOrgModColumn (mod_name, NULL, seq_list, sqfp->seqPackage);
  SeqEntryPtrToSourceTab (sqfp);  
  fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
  if (fpp != NULL) 
  {
    Reset (fpp->doc);
    FormatFastaDoc (fpp);
  }
}

static void ApplyModValuesAllBtn (ButtoN b)
{
  WindoW w;
  CharPtr mod_name;
  
  w = ParentWindow ((Nlm_GraphiC) b);
  mod_name = GetObjectExtra (w);
  if (StringHasNoText (mod_name))
  {
    return;
  }
  ApplyOneValueToAllSequencesDialog (b, mod_name);
  Remove (w);  
}

static void ApplyOrganismValuesAllBtn (ButtoN b)
{
  WindoW w;
  
  w = ParentWindow ((Nlm_GraphiC) b);
  ApplyOneValueToAllSequencesDialog (b, "organism");
  Remove (w);  
}

static void ApplyLocationValuesAllBtn (ButtoN b)
{
  WindoW w;
  
  w = ParentWindow ((Nlm_GraphiC) b);
  ApplyOneValueToAllSequencesDialog (b, "location");
  Remove (w);  
}

static void ApplyGeneticCodeValuesAllBtn (ButtoN b)
{
  WindoW w;
  
  w = ParentWindow ((Nlm_GraphiC) b);
  ApplyOneValueToAllSequencesDialog (b, "genetic_code");
  Remove (w);  
}

static void ApplyModValuesIndividuallyBtn (ButtoN b)
{
  WindoW w;
  CharPtr mod_name;
  
  w = ParentWindow ((Nlm_GraphiC) b);
  mod_name = GetObjectExtra (w);
  if (StringHasNoText (mod_name))
  {
    return;
  }
  ApplyModValuesIndividually (b, mod_name);
  Remove (w);  
}

static void ApplyOrgLocGcodeValuesIndividuallyBtn (ButtoN b)
{
  WindoW w;
  
  w = ParentWindow ((Nlm_GraphiC) b);
  ApplyModValuesIndividually (b, "organism");
  Remove (w);  
}

static void SpecifyModValueButton (ButtoN b, CharPtr mod_name)
{
  SequencesFormPtr sqfp;
  WindoW           w;
  GrouP            h;
  SeqEntryPtr      seq_list;
  Char             title[255];
  ButtoN           apply_one_btn;
  ButtoN           apply_all_btn;
  ButtoN           cancel_btn;
  CharPtr          mod_label = NULL;
  
  sqfp = (SequencesFormPtr) GetObjectExtra (b);
  if (sqfp == NULL || StringHasNoText (mod_name))
  {
    return;
  }
  
  if (StringICmp (mod_name, "moltype") == 0)
  {
    mod_label = StringSave ("molecule");
  }
  else
  {
    mod_label = StringSave (mod_name);
  }
  
  seq_list = GetSeqEntryFromSequencesForm (sqfp);
  if (seq_list == NULL)
  {
    Message (MSG_ERROR, "You must add sequences before you can add %s information!", mod_label);
    mod_label = MemFree (mod_label);
    return;
  }
  if (seq_list->next == NULL || sqfp->seqPackage == SEQ_PKG_SEGMENTED)
  {
    ApplyOneValueToAllSequencesDialog (b, mod_name);
    mod_label = MemFree (mod_label);
    return;
  }

  sprintf (title, "Edit %s Information", mod_label);
  title [5] = TO_UPPER (title [5]);
  
  w = MovableModalWindow (-20, -13, -10, -10, title, NULL);
  SetObjectExtra (w, StringSave (mod_name), StdCleanupExtraProc);
  h = HiddenGroup(w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
  
  sprintf (title, "Apply one %s to all sequences", mod_label);
  apply_one_btn = PushButton (h, title, ApplyModValuesAllBtn);
  SetObjectExtra (apply_one_btn, sqfp, NULL);
  sprintf (title, "Apply %s to sequences individually", mod_label);
  apply_all_btn = PushButton (h, title, ApplyModValuesIndividuallyBtn);
  SetObjectExtra (apply_all_btn, sqfp, NULL);
  cancel_btn = PushButton (h, "Cancel", StdCancelButtonProc);
  mod_label = MemFree (mod_label);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) apply_one_btn,
                             (HANDLE) apply_all_btn,
                             (HANDLE) cancel_btn,
                             NULL);
  Show(w); 
  Select (w);  
  
}

static void SpecifyOrganisms (ButtoN b)
{
  SpecifyModValueButton (b, "organism");
}

static void SpecifyLocations (ButtoN b)
{
  SpecifyModValueButton (b, "location");
}

static void SpecifyOrganismLocationGeneticCodeButton (ButtoN b)
{
  SequencesFormPtr sqfp;
  WindoW           w;
  GrouP            h;
  SeqEntryPtr      seq_list;
  ButtoN           apply_one_org_btn, apply_one_loc_btn, apply_one_gc_btn;
  ButtoN           apply_all_btn = NULL;
  ButtoN           cancel_btn;
  
  sqfp = (SequencesFormPtr) GetObjectExtra (b);
  if (sqfp == NULL)
  {
    return;
  }
  
  seq_list = GetSeqEntryFromSequencesForm (sqfp);
  if (seq_list == NULL)
  {
    Message (MSG_ERROR, "You must add sequences before you can add organisms, locations, or genetic codes!");
  }
  else if (seq_list->next == NULL || sqfp->seqPackage == SEQ_PKG_SEGMENTED)
  {
    ApplyModValuesIndividually (b, "organism");
  }
  else
  {
    w = MovableModalWindow (-20, -13, -10, -10, "Edit Organisms, Locations, and Genetic Codes", NULL);
    h = HiddenGroup(w, -1, 0, NULL);
    SetGroupSpacing (h, 10, 10);

    apply_one_org_btn = PushButton (h, "Apply one organism name to all sequences", ApplyOrganismValuesAllBtn);
    SetObjectExtra (apply_one_org_btn, sqfp, NULL);
  
    apply_one_loc_btn = PushButton (h, "Apply one location to all sequences", ApplyLocationValuesAllBtn);
    SetObjectExtra (apply_one_loc_btn, sqfp, NULL);
  
    apply_one_gc_btn = PushButton (h, "Apply one genetic to all sequences", ApplyGeneticCodeValuesAllBtn);
    SetObjectExtra (apply_one_gc_btn, sqfp, NULL);

    apply_all_btn = PushButton (h, "Apply organism name, location, and genetic code to sequences individually", ApplyOrgLocGcodeValuesIndividuallyBtn);
    SetObjectExtra (apply_all_btn, sqfp, NULL);
  
    cancel_btn = PushButton (h, "Cancel", StdCancelButtonProc);

    if (apply_all_btn == NULL)
    {
      AlignObjects (ALIGN_CENTER, (HANDLE) apply_one_org_btn,
                                  (HANDLE) apply_one_loc_btn,
                                  (HANDLE) apply_one_gc_btn,
                                  (HANDLE) cancel_btn,
                                  NULL);
    }
    else
    {
      AlignObjects (ALIGN_CENTER, (HANDLE) apply_one_org_btn,
                                  (HANDLE) apply_one_loc_btn,
                                  (HANDLE) apply_one_gc_btn,
                                  (HANDLE) apply_all_btn,
                                  (HANDLE) cancel_btn,
                                  NULL);
    }
    Show(w); 
    Select (w);  
  }
}

static void ApplyOneGeneticCodeToAllSequencesDialog (ButtoN b, Boolean is_nuc)
{
  WindoW w;
  
  w = ParentWindow ((Nlm_GraphiC) b);
  if (is_nuc)
  {
    ApplyOneValueToAllSequencesDialog (b, "gcode");
  }
  else
  {
    ApplyOneValueToAllSequencesDialog (b, "mgcode");
  }
  Remove (w);  
}

static void ApplyOneNuclearGeneticCodeToAllSequencesDialog (ButtoN b)
{
  ApplyOneGeneticCodeToAllSequencesDialog (b, TRUE);
}

static void ApplyOneMitochondrialGeneticCodeToAllSequencesDialog (ButtoN b)
{
  ApplyOneGeneticCodeToAllSequencesDialog (b, FALSE);
}

static void ApplyGeneticCodeIndividually (ButtoN b, Boolean is_nuc)
{
  if (is_nuc)
  {
    ApplyModValuesIndividually (b, "gcode");
  }
  else
  {
    ApplyModValuesIndividually (b, "mgcode");
  }
}

static void ApplyNuclearGeneticCodeIndividually (ButtoN b)
{
  ApplyGeneticCodeIndividually (b, TRUE);
}

static void ApplyMitochondrialGeneticCodeIndividually (ButtoN b)
{
  ApplyGeneticCodeIndividually (b, FALSE);
}

static void SpecifyGeneticCode (ButtoN b)
{
  SpecifyModValueButton (b, "genetic_code");
}

static void ClearAllSequenceModifiers (ButtoN b)
{
  SequencesFormPtr sqfp;
  SeqEntryPtr      seq_list, sep;
  CharPtr          ttl;
  ValNodePtr       found_modifiers = NULL, mod_vnp;
  FastaPagePtr     fpp;
  Int4             mod_type;
  
  sqfp = (SequencesFormPtr) GetObjectExtra (b);
  if (sqfp == NULL)
  {
    return;
  }
  
  seq_list = GetSeqEntryFromSequencesForm (sqfp);
  if (seq_list == NULL)
  {
    return;
  }  
  
  if (ANS_YES != Message (MSG_YN, "Are you sure you want to clear all of the modifiers on all of your sequences?"))
  {
    return;
  }
  
  for (sep = seq_list; sep != NULL; sep = sep->next)
  {
    ttl = NULL;
    SeqEntryExplore (sep, (Pointer) (&ttl), FindFirstTitle);
    found_modifiers = BuildModifierTypeList (found_modifiers, ttl);
    for (mod_vnp = found_modifiers; mod_vnp != NULL; mod_vnp = mod_vnp->next)
    {
      mod_type = GetModifierType (mod_vnp->data.ptrvalue);
      if (mod_type != eModifierType_Other
          && mod_type != eModifierType_Location
          && mod_type != eModifierType_MolType
          && mod_type != eModifierType_Topology
          && mod_type != eModifierType_GeneticCode
          && mod_type != eModifierType_GeneticCodeComment
          && mod_type != eModifierType_Organism)
      {
        ApplyOneModToSeqEntry (sep, mod_vnp->data.ptrvalue, NULL);
      }
    }
    found_modifiers = ValNodeFreeData (found_modifiers);
  }
  SeqEntryPtrToSourceTab (sqfp);
  fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
  if (fpp != NULL) 
  {
    Reset (fpp->doc);
    FormatFastaDoc (fpp);
  }
}

static void SummaryDocOnClick (DoC d, PoinT pt)
{
  SequencesFormPtr sqfp;
  SeqEntryPtr      seq_list, sep;
  Int2             pos_item, pos_row, pos_col;
  Int2             start_item = 0;
  Int2             end_item = -1;
  Int4             mod_num;
  CharPtr          ttl = NULL;
  ValNodePtr       found_modifiers = NULL, vnp;
  FastaPagePtr     fpp;
  Boolean          dbl_click;
  
  dbl_click = dblClick;
  if (!dbl_click)
  {
    return;
  }
  
  sqfp = (SequencesFormPtr) GetObjectExtra (d);
  if (sqfp == NULL) return;
  
  seq_list = GetSeqEntryFromSequencesForm (sqfp);
  if (seq_list == NULL)
  {
    return;
  }
  
  MapDocPoint (d, pt, &(pos_item), &(pos_row), &(pos_col), NULL);

  /* get list of modifiers */    
  ValNodeAddPointer (&found_modifiers, 0, "organism");
  for (sep = seq_list; sep != NULL; sep = sep->next)
  {
    ttl = NULL;
    SeqEntryExplore (sep, (Pointer) (&ttl), FindFirstTitle);
    found_modifiers = BuildModifierTypeList (found_modifiers, ttl);
  }
  
  for (vnp = found_modifiers, mod_num = 1;
       vnp != NULL && mod_num < pos_item;
       vnp = vnp->next, mod_num++)
  {
  }
  if (vnp != NULL)
  {
    EditOrgModColumn (vnp->data.ptrvalue, NULL, seq_list, sqfp->seqPackage);
    SeqEntryPtrToSourceTab (sqfp);  
    fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
    if (fpp != NULL) 
    {
      Reset (fpp->doc);
      FormatFastaDoc (fpp);
    }
  }
}

static GrouP CreateSourceTab (GrouP h, SequencesFormPtr sqfp)
{
  GrouP              mod_grp;
  GrouP              src_btns_grp;
  GrouP              k;

  if (h == NULL || sqfp == NULL)
  {
    return NULL;
  }
 
  modedit_widths [0] = 7;
  modedit_widths [1] = 18;

  mod_grp = HiddenGroup (h, -1, 0, NULL);
  SetGroupSpacing (mod_grp, 10, 20);

  sqfp->org_doc = DocumentPanel (mod_grp, stdCharWidth * 27, stdLineHeight * 2);
  SetDocAutoAdjust (sqfp->org_doc, TRUE);
  
  sqfp->summary_doc = DocumentPanel (mod_grp, stdCharWidth * 27, stdLineHeight * 12);
  SetObjectExtra (sqfp->summary_doc, sqfp, NULL);
  SetDocAutoAdjust (sqfp->summary_doc, TRUE);
  SetDocProcs (sqfp->summary_doc, SummaryDocOnClick, NULL, NULL, NULL);  

  sqfp->specify_orgs_btn = PushButton (mod_grp, 
                                       "Add Organisms, Locations, and Genetic Codes", 
                                       SpecifyOrganismLocationGeneticCodeButton);
  SetObjectExtra (sqfp->specify_orgs_btn, sqfp, NULL);
  Disable (sqfp->specify_orgs_btn);
  k = HiddenGroup (mod_grp, -1, 0, NULL);
  SetGroupSpacing (k, 10, 10);
  src_btns_grp = HiddenGroup (k, 2, 0, NULL);
  SetGroupSpacing (src_btns_grp, 10, 10);
  sqfp->import_mod_btn = PushButton (src_btns_grp, "Import Source Modifiers", ImportModifiersButtonProc);
  SetObjectExtra (sqfp->import_mod_btn, sqfp, NULL);
  Disable (sqfp->import_mod_btn);
  sqfp->source_assist_btn = PushButton (src_btns_grp, "Add Source Modifiers", SourceAssistant);
  SetObjectExtra (sqfp->source_assist_btn, sqfp, NULL);
  Disable (sqfp->source_assist_btn);
  sqfp->specify_locs_btn = NULL;
  sqfp->specify_gcode_btn = NULL;
  sqfp->specify_mgcode_btn = NULL;
    
  sqfp->clear_mods_btn = PushButton (k, "Clear All Source Modifiers", ClearAllSequenceModifiers);
  SetObjectExtra (sqfp->clear_mods_btn, sqfp, NULL);
  Disable (sqfp->clear_mods_btn);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) src_btns_grp, (HANDLE) sqfp->clear_mods_btn, NULL);

  SeqEntryPtrToSourceTab (sqfp);
  AlignObjects (ALIGN_CENTER, (HANDLE) sqfp->org_doc,
                              (HANDLE) sqfp->specify_orgs_btn,
                              (HANDLE) k,
                              NULL);
  
  return mod_grp;
}

typedef struct fastasummary 
{
  DIALOG_MESSAGE_BLOCK
  PrompT summary_ppt;
} FastaSummaryData, PNTR FastaSummaryPtr;

static void SequencesToFastaSummary (DialoG d, Pointer userdata)
{
  FastaSummaryPtr dlg;
  SeqEntryPtr     seq_list, sep, nsep;
  Int4            num_sequences = 0, tot_len = 0;
  BioseqPtr       bsp;
  CharPtr         str_format = "%d sequences, total length %d";
  Char            str[255];
  
  dlg = (FastaSummaryPtr) GetObjectExtra (d);
  if (dlg == NULL)
  {
    return;
  }
  
  seq_list = (SeqEntryPtr) userdata;
  for (sep = seq_list; sep != NULL; sep = sep->next)
  {
    num_sequences++;
    if (IS_Bioseq (sep)) 
    {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (bsp != NULL) 
      {
        tot_len += bsp->length;
      }
    } 
    else if (IS_Bioseq_set (sep)) 
    {
      nsep = FindNucSeqEntry (sep);
      if (nsep != NULL && IS_Bioseq (nsep)) 
      {
        bsp = (BioseqPtr) nsep->data.ptrvalue;
        if (bsp != NULL) 
        {
          tot_len += bsp->length;
        }
      }
    }
  }
  sprintf (str, str_format, num_sequences, tot_len);
  SetTitle (dlg->summary_ppt, str);
}

static DialoG FastaSummaryDialog (GrouP parent)
{
  FastaSummaryPtr dlg;
  GrouP           p;
  
  dlg = (FastaSummaryPtr) MemNew (sizeof (FastaSummaryData));
  p = HiddenGroup (parent, -1, 0, NULL);
  SetObjectExtra (p, dlg, StdCleanupExtraProc);
  SetGroupSpacing (p, 10, 10);

  dlg->dialog = (DialoG) p;
  dlg->todialog = SequencesToFastaSummary;
  dlg->fromdialog = NULL;
  dlg->dialogmessage = NULL;
  dlg->testdialog = NULL;

  dlg->summary_ppt = StaticPrompt (p, NULL, stdCharWidth * 20,
                                   popupMenuHeight, programFont, 'l');  
  return (DialoG) p;
}

static void SetDefaultMolTypesAndTopologies (SeqEntryPtr seq_list, Int2 seqPackage)
{
  SeqEntryPtr sep;
  CharPtr     ttl = NULL;
  Char        tmp[128];
  
  for (sep = seq_list; sep != NULL; sep = sep->next)
  {
    ttl = NULL;
    SeqEntryExplore (sep, (Pointer) (&ttl), FindFirstTitle);

    if (!LookForSearchString (ttl, "[moltype=", tmp, sizeof (tmp) - 1)) 
    {
      if (PackageTypeDefaultMolTypeIsDNA (seqPackage)) 
      {
        ApplyOneModToSeqEntry (sep, "moltype", "Genomic DNA");
      }
      else
      {
        ApplyOneModToSeqEntry (sep, "moltype", "Genomic RNA");
      }
    }
    ttl = NULL;
    SeqEntryExplore (sep, (Pointer) (&ttl), FindFirstTitle);
    if (!LookForSearchString (ttl, "[topology=", tmp, sizeof (tmp) - 1)) 
    {
      ApplyOneModToSeqEntry (sep, "topology", "Linear");
    }
  }
}

/* The Sequence Assistant will allow users to paste in FASTA or create sequences one at a time.
 * The list control for selecting a sequence to view and edit should be a DocPanel, so that we
 * can add and remove sequences without needing to destroy and recreate the dialog.
 * The sequences created should be stored as a SeqEntry list.
 */
typedef struct sequenceassistant 
{
  DoC            sequence_selector;
  DialoG         summary_dlg;
  DialoG         sequence_table;
  ButtoN         edit_btn;
  ButtoN         delete_btn;
  ButtoN         delete_all_btn;
  ButtoN         import_btn;
  
  Int2           sequence_row;  
  
  SeqEntryPtr    seq_list;
  
  Int2           seqPackage;

  Boolean        done;
  Boolean        cancelled;  
} SequenceAssistantData, PNTR SequenceAssistantPtr;

static ValNodePtr PrepareSequenceAssistantTableData (SequenceAssistantPtr sap)
{
  ValNodePtr         column_list = NULL, row_list = NULL;
  ValNodePtr         header_list, header_vnp;
  Int4               column_width, num_columns = 0;
  Int4               max_column_width = 20;
  SeqEntryPtr        sep, nsep;
  BioseqPtr          bsp;
  SeqIdPtr           sip;
  Char               tmp[128];
  CharPtr            ttl = NULL;
  
  if (sap == NULL)
  {
    return NULL;
  }
  SetDefaultMolTypesAndTopologies (sap->seq_list, sap->seqPackage);
      
  /* create header line for table */
  /* store max column width in choice */
  ValNodeAddPointer (&column_list, 6, StringSave ("Seq ID"));
  ValNodeAddPointer (&column_list, 6, StringSave ("Length"));
  ValNodeAddPointer (&column_list, 8, StringSave ("Molecule"));
  ValNodeAddPointer (&column_list, 8, StringSave ("Topology"));
  ValNodeAddPointer (&column_list, 11, StringSave ("Description"));
    
  ValNodeAddPointer (&row_list, 0, column_list);
  header_list = column_list;
  
  num_columns = 3;

  /* create data lines for table */
  for (sep = sap->seq_list; sep != NULL; sep = sep->next)
  {
    bsp = NULL;
    
    if (IS_Bioseq (sep))
    {
      bsp = (BioseqPtr) sep->data.ptrvalue;
    }
    else if (IS_Bioseq_set (sep))
    {
      nsep = FindNucSeqEntry (sep);
      if (nsep != NULL && IS_Bioseq (nsep))
      {
        bsp = (BioseqPtr) nsep->data.ptrvalue;
      }
    }
    
    column_list = NULL;
    
    /* add Sequence ID */
    header_vnp = header_list;
    sip = SeqIdFindWorst (bsp->id);
    SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp) - 1);    
    column_width = MAX (StringLen (tmp), header_vnp->choice);
    column_width = MIN (column_width, max_column_width);
    header_vnp->choice = column_width;
    ValNodeAddPointer (&column_list, 0, StringSave (tmp));
      
    /* add length */
    header_vnp = header_vnp->next;
    sprintf (tmp, "%d", bsp->length);
    column_width = MAX (StringLen (tmp), header_vnp->choice);
    column_width = MIN (column_width, max_column_width);
    header_vnp->choice = column_width;
    ValNodeAddPointer (&column_list, 0, StringSave (tmp));
    
    ttl = NULL;
    SeqEntryExplore (sep, (Pointer) (&ttl), FindFirstTitle);

    /* add molecule */
    header_vnp = header_vnp->next;
    if (!LookForSearchString (ttl, "[moltype=", tmp, sizeof (tmp) - 1)) 
    {
      if (PackageTypeDefaultMolTypeIsDNA (sap->seqPackage)) 
      {
        sprintf (tmp, "Genomic DNA");
      }
      else
      {
        sprintf (tmp, "Genomic RNA");
      }
    }
    column_width = MAX (StringLen (tmp), header_vnp->choice);
    column_width = MIN (column_width, max_column_width);
    header_vnp->choice = column_width;
    ValNodeAddPointer (&column_list, 0, StringSave (tmp));

    /* add topology */
    header_vnp = header_vnp->next;
    if (!LookForSearchString (ttl, "[topology=", tmp, sizeof (tmp) - 1)) 
    {
      sprintf (tmp, "Linear");
    }
    column_width = MAX (StringLen (tmp), header_vnp->choice);
    column_width = MIN (column_width, max_column_width);
    header_vnp->choice = column_width;
    ValNodeAddPointer (&column_list, 0, StringSave (tmp));

    /* add description */  
    header_vnp = header_vnp->next;
    column_width = MAX (StringLen (ttl), header_vnp->choice);
    column_width = MIN (column_width, max_column_width);
    header_vnp->choice = column_width;
    ValNodeAddPointer (&column_list, 0, StringSave (ttl));
  
    ValNodeAddPointer (&row_list, 0, column_list);
  }
  return row_list;
}

static void PopulateSequenceSelector (SequenceAssistantPtr sap, DoC selector)
{
  SeqEntryPtr sep, nsep;
  BioseqPtr   bsp;
  SeqIdPtr    sip;
  Char        tmp[128];
  Int2        seq_num;
  
  if (sap == NULL || selector == NULL)
  {
    return;
  }
  Reset (selector);
  for (sep = sap->seq_list, seq_num = 0; sep != NULL; sep = sep->next, seq_num++)
  {
    bsp = NULL;
    if (IS_Bioseq (sep))
    {
      bsp = sep->data.ptrvalue;
    }
    else if (IS_Bioseq_set (sep))
    {
      nsep = FindNucSeqEntry (sep);
      if (nsep != NULL && IS_Bioseq (nsep))
      {
        bsp = nsep->data.ptrvalue;
      }
    }
    
    if (bsp != NULL)
    {
      /* add to sequence_selector doc */
      sip = SeqIdFindWorst (bsp->id);
      SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp) - 1);
  	  AppendText (selector, tmp, &faParFmt, &faColFmt, programFont);  	  
    }
  }
  InvalDocRows (selector, 0, 0, 0);
  sap->sequence_row = -1;
  Disable (sap->edit_btn);
  Disable (sap->delete_btn);
  
}

static void UpdateSequenceAssistant (SequenceAssistantPtr sap)
{
  ValNodePtr row_list;
  
  if (sap == NULL)
  {
    return;
  }
  
  row_list = PrepareSequenceAssistantTableData (sap);
  PointerToDialog (sap->sequence_table, row_list);
  FreeTableDisplayRowList (row_list);

  PopulateSequenceSelector (sap, sap->sequence_selector);
  PointerToDialog (sap->summary_dlg, sap->seq_list);
  
  /* set title for import button */
  if (sap->seq_list != NULL && sap->seq_list->next != NULL)
  {
    SetTitle (sap->import_btn, "Import Additional Nucleotide FASTA");
  }
  else
  {
    SetTitle (sap->import_btn, "Import Nucleotide FASTA");
  }
  
  if (sap->seq_list == NULL)
  {
    Disable (sap->delete_all_btn);
  }
  else
  {
    Enable (sap->delete_all_btn);
  }
}

static SeqEntryPtr GetSequencesFromFile (CharPtr path, SeqEntryPtr current_list);

static void ImportSequenceAssistantEditData (SequenceAssistantPtr sap, CharPtr seq_str)
{
  Char         path [PATH_MAX];
  SeqEntryPtr  new_sep_list;

  if (sap == NULL || StringHasNoText (seq_str))
  {
    return;
  }
  
  TmpNam (path);
  new_sep_list = GetSequencesFromFile (path, sap->seq_list);
  if (new_sep_list != NULL)
  {
    ValNodeLink (&sap->seq_list, new_sep_list);
  }
  UpdateSequenceAssistant (sap);

  FileRemove (path);
}

static Boolean 
IsDuplicateID (SeqEntryPtr seq_list, BioseqPtr edit_bsp, SeqIdPtr sip);

static Uint2 idedit_types [] = {
  TAGLIST_PROMPT, TAGLIST_TEXT, TAGLIST_TEXT
};

static Uint2 idedit_widths [] = {
  4, 10, 40,
};

typedef struct seqedittag 
{
  ValNodePtr current_list;
  ValNodePtr new_list;  
} SeqEditTagData, PNTR SeqEditTagPtr;

static void 
AddSeqInfoToTagListData 
(SeqEntryPtr     list, 
 SeqEntryPtr     current_list,
 SeqEntryPtr     new_list,
 Int4Ptr         pnum_sequences,
 ValNodePtr PNTR taglist_data)
{
  SeqEntryPtr           sep;
  Char                  id_str [128];  
  CharPtr               title_txt;
  BioseqPtr             bsp;
  BioseqSetPtr          bssp;
  Int4                  num_sequences = 0;
  SeqDescrPtr           sdp;
  Boolean               is_dup;
  CharPtr               str;
  Int4                  len;
  
  if (taglist_data == NULL)
  {
    return;
  }
  
  /* collect current IDs and titles */
  for (sep = list; sep != NULL; sep = sep->next)
  {
    if (sep->data.ptrvalue == NULL)
    {
      continue;
    }
    else if (IS_Bioseq (sep))
    {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      /* collect ID */
      SeqIdWrite (bsp->id, id_str, PRINTID_REPORT, sizeof (id_str) - 1);    
      /* collect title */
      title_txt = NULL;
      sdp = bsp->descr;
      while (sdp != NULL && sdp->choice != Seq_descr_title)
      {
        sdp = sdp->next;
      }
      if (sdp != NULL)
      {
        title_txt = sdp->data.ptrvalue;
      }
      /* determine whether this is a duplicate */
      is_dup = IsDuplicateID (current_list, NULL, bsp->id)
                              || IsDuplicateID (new_list, bsp, bsp->id);
                                
      len = StringLen (id_str) + StringLen (title_txt);
      str = MemNew (len + 9);
      if (str != NULL) 
      {
        sprintf (str, "%s\t%s\t%s\n", 
                 is_dup ? "DUP" : "",
                 id_str,
                 title_txt == NULL ? "" : title_txt);
        ValNodeAddPointer (taglist_data, 0, str);
      }
      num_sequences ++;
    }
    else if (IS_Bioseq_set (sep))
    {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      AddSeqInfoToTagListData (bssp->seq_set, current_list, new_list,
                               &num_sequences, taglist_data);
    }
  }
  if (pnum_sequences != NULL)
  {
    *pnum_sequences += num_sequences;
  }
}

static void SeqListToTagListDialog (DialoG d, Pointer userdata)
{
  TagListPtr            tlp;
  SeqEditTagPtr         setp;
  Int4                  num_sequences = 0;
  ValNodePtr            taglist_data = NULL;

  tlp = (TagListPtr) GetObjectExtra (d);
  setp = (SeqEditTagPtr) userdata;
  
  if (tlp == NULL || setp == NULL || setp->new_list == NULL)
  {
    return;
  }
  
  /* collect current IDs and titles */
  AddSeqInfoToTagListData (setp->new_list, setp->current_list, setp->new_list, 
                           &num_sequences, &taglist_data);
  SendMessageToDialog (tlp->dialog, VIB_MSG_RESET); 
  tlp->vnp = taglist_data;
  SendMessageToDialog (tlp->dialog, VIB_MSG_REDRAW);
  tlp->max = MAX ((Int2) 0, (Int2) (num_sequences - tlp->rows));
  CorrectBarMax (tlp->bar, tlp->max);
  CorrectBarPage (tlp->bar, tlp->rows - 1, tlp->rows - 1);
  if (tlp->max > 0) {
    SafeShow (tlp->bar);
  } else {
    SafeHide (tlp->bar);
  }
}

static void CollectIdAndTitleFromDialog (TagListPtr tlp, SeqEntryPtr sep, Int4Ptr seq_num)
{
  BioseqPtr    bsp;
  BioseqSetPtr bssp;
  CharPtr      str;
  SeqDescrPtr  sdp;
  SeqIdPtr     new_sip;
  
  if (tlp == NULL || sep == NULL || seq_num == NULL)
  {
    return;
  }
  
  if (sep->data.ptrvalue != NULL)
  {
    if (IS_Bioseq (sep))
    {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      /* collect ID */
      str = GetTagListValueEx (tlp, *seq_num, 1);
      new_sip = MakeSeqID (str);
      str = MemFree (str);
      if (new_sip != NULL)
      {
        if (bsp->id != NULL)
        {
          new_sip->next = bsp->id->next;
          bsp->id->next = NULL;
          bsp->id = SeqIdFree (bsp->id);
        }
        bsp->id = new_sip;
      }
      /* collect title */
      str = GetTagListValueEx (tlp, *seq_num, 2);
      sdp = bsp->descr;
      while (sdp != NULL && sdp->choice != Seq_descr_title)
      {
        sdp = sdp->next;
      }
      if (sdp == NULL)
      {
        sdp = CreateNewDescriptor (sep, Seq_descr_title);
        sdp->data.ptrvalue = str;
      }
      else
      {
        sdp->data.ptrvalue = MemFree (sdp->data.ptrvalue);
        sdp->data.ptrvalue = str;
      }
      (*seq_num)++;
    }
    else if (IS_Bioseq_set (sep)) 
    {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      CollectIdAndTitleFromDialog (tlp, bssp->seq_set, seq_num);
    }
  }
  CollectIdAndTitleFromDialog (tlp, sep->next, seq_num); 
}


static Boolean FixDefinitionLinesInAddSequence (SeqEntryPtr sep_list)
{
  Boolean     defline_fix_cancel;
  SeqEntryPtr sep;
  BioseqPtr   bsp;
  SeqDescrPtr sdp;
  CharPtr     old_txt;
  Char        id_txt [128];
  
  if (sep_list == NULL)
  {
    return FALSE;
  }
  
  /* get user to fix definition lines here */
  defline_fix_cancel = FALSE;
  for (sep = sep_list; sep != NULL && !defline_fix_cancel; sep = sep->next)
  {
    if (IS_Bioseq (sep) && sep->data.ptrvalue != NULL)
    {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      sdp = bsp->descr;
      while (sdp != NULL && sdp->choice != Seq_descr_title)
      {
        sdp = sdp->next;
      }
      if (sdp != NULL && !StringHasNoText (sdp->data.ptrvalue))
      {
        SeqIdWrite (bsp->id, id_txt, PRINTID_REPORT, sizeof (id_txt) - 1);    
        old_txt = StringSave (sdp->data.ptrvalue);
        FixBracketingInOneDefinitionLine (sdp, &defline_fix_cancel, id_txt);
        FixUnrecognizedModifersInOneDefinitionLine (sdp, &defline_fix_cancel, NULL);
        if (defline_fix_cancel)
        {
          sdp->data.ptrvalue = MemFree (sdp->data.ptrvalue);
          sdp->data.ptrvalue = old_txt;
        }
        else
        {
          old_txt = MemFree (old_txt);
        }
      }
    }
  }
  if (defline_fix_cancel)
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

static Boolean IDsNeedFix (SeqEntryPtr list, SeqEntryPtr current_list)
{
  Boolean      need_fix = FALSE;
  BioseqPtr    bsp;
  BioseqSetPtr bssp;
  SeqEntryPtr  sep;
  
  if (list == NULL)
  {
    return FALSE;
  }
  /* check for unique IDs or missing IDs */
  for (sep = list; sep != NULL && ! need_fix; sep = sep->next)
  {
    if (sep->data.ptrvalue == NULL)
    {
      /* do nothing */
    }
    else if (IS_Bioseq (sep))
    {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      need_fix = bsp->id == NULL 
                   || IsDuplicateID (current_list, NULL, bsp->id)
                   || IsDuplicateID (list, bsp, bsp->id);
    }
    else if (IS_Bioseq_set (sep))
    {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      need_fix |= IDsNeedFix (bssp->seq_set, current_list);
    }
  }
  return need_fix;
}

static Int4 CountSequencesAndSegments (SeqEntryPtr list)
{
  Int4         num_seqs = 0;
  BioseqSetPtr bssp;
  
  while (list != NULL)
  {
    if (list->data.ptrvalue != NULL)
    {
      if (IS_Bioseq (list))
      {
        num_seqs ++;
      }
      else if (IS_Bioseq_set (list))
      {
        bssp = (BioseqSetPtr) list->data.ptrvalue;
        num_seqs += CountSequencesAndSegments (bssp->seq_set);
      }
    }
    list = list->next;
  }
  return num_seqs;
}

static Boolean CollectIDsAndTitles (SeqEntryPtr new_list, SeqEntryPtr current_list)
{
  WindoW                w;
  GrouP                 h, ppt, c;
  ButtoN                b;
  Int4                  num_sequences = 0, seq_num;
  BoolPtr               is_dup_list = NULL;
  Boolean               need_fix = FALSE;
  DialoG                dlg;
  TagListPtr            tlp;
  ModalAcceptCancelData acd;
  SeqEditTagData        setd;
 
  /* check for unique IDs - don't need to present dialog if they
   * are all present and unique */
  need_fix = IDsNeedFix (new_list, current_list);
  
  /* if no fixes are needed, do not present dialog */
  if (!need_fix)
  {
    return TRUE;
  }
  num_sequences = CountSequencesAndSegments (new_list);
  
  setd.current_list = current_list;
  setd.new_list = new_list;
  
  w = MovableModalWindow (-20, -13, -10, -10, "Provide Sequence IDs for your Sequences", NULL);
  h = HiddenGroup(w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  ppt = HiddenGroup (h, 3, 0, NULL);
  StaticPrompt (ppt, "", 4 * stdCharWidth, 0, programFont, 'l');
  StaticPrompt (ppt, "Sequence ID", 10 * stdCharWidth, 0, programFont, 'l');
  StaticPrompt (ppt, "Description", 40 * stdCharWidth, 0, programFont, 'l');

  dlg = CreateTagListDialogEx (h, MIN (num_sequences, 8), 3, 2,
                               idedit_types, idedit_widths,
                               NULL, TRUE, TRUE, SeqListToTagListDialog, NULL);
  PointerToDialog (dlg, &setd);

  tlp = (TagListPtr) GetObjectExtra (dlg);  
  if (tlp == NULL) return;

  c = HiddenGroup (h, 2, 0, NULL);
  b = PushButton (c, "Accept", ModalAcceptButton);
  SetObjectExtra (b, &acd, NULL);
  b = PushButton (c, "Cancel", ModalCancelButton);
  SetObjectExtra (b, &acd, NULL);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) ppt, (HANDLE) dlg, (HANDLE) c, (HANDLE) NULL);

  Show (w);
  Select (w);
  
  acd.cancelled = FALSE;
  while (need_fix && ! acd.cancelled)
  {
    acd.accepted = FALSE;
    while (!acd.accepted && ! acd.cancelled)
    {
      ProcessExternalEvent ();
      Update ();
    }
    ProcessAnEvent ();
    if (! acd.cancelled)
    {
      seq_num = 0;
      CollectIdAndTitleFromDialog (tlp, new_list, &seq_num);
      
      /* now check for unique IDs */
      need_fix = IDsNeedFix (new_list, current_list);
      if (need_fix)
      {
        Message (MSG_ERROR, "Blank or Duplicate IDs are not allowed!");
        PointerToDialog (dlg, &setd);
      }
      else if (! FixDefinitionLinesInAddSequence (new_list))
      {
        need_fix = TRUE;
      }
      
      if (need_fix)
      {
        PointerToDialog (dlg, &setd);
      }
    }
  }
  Remove (w);
  if (acd.cancelled)
  {
    return FALSE;
  }
  else
  {
    ResetSegSetIDLists (new_list);
    return TRUE;
  }
}

static void ReplaceFakeIDWithIDFromTitle (BioseqPtr bsp)
{
  SeqDescrPtr sdp;
  CharPtr     title_txt, new_id_str;
  Int4        id_len;
  
  if (bsp == NULL)
  {
    return;
  }
  
  bsp->id = SeqIdFree (bsp->id);
  
  sdp = bsp->descr;
  while (sdp != NULL && sdp->choice != Seq_descr_title)
  {
    sdp = sdp->next;
  }
  if (sdp != NULL && !StringHasNoText (sdp->data.ptrvalue))
  {
    title_txt = sdp->data.ptrvalue;
    /* skip any leading spaces */
    title_txt += StringSpn (title_txt, " \t");
    id_len = StringCSpn (title_txt, " \t");
    if (id_len > 0)
    {
      new_id_str = (CharPtr) MemNew ((id_len + 1) * sizeof (Char));
      if (new_id_str != NULL)
      {
        StringNCpy (new_id_str, title_txt, id_len);
        new_id_str [id_len] = 0;
        bsp->id = MakeSeqID (new_id_str);
        new_id_str = MemFree (new_id_str);
        /* remove id from title */
        title_txt += id_len;
        title_txt += StringSpn (title_txt, " \t");
        title_txt = StringSave (title_txt);
        sdp->data.ptrvalue = MemFree (sdp->data.ptrvalue);
        sdp->data.ptrvalue = title_txt;
      }
    }
  }  
}

static SeqEntryPtr GetSequencesFromFile (CharPtr path, SeqEntryPtr current_list) 
{
  FILE         *fp;
  SeqEntryPtr  new_sep_list, new_sep, test_sep;
  CharPtr      errormsg = NULL;
  Boolean      cancelled = FALSE;
  ValNodePtr   fixes = NULL;
  Boolean      parseSeqId = FALSE;
  Int4         num_sequences = 0;

  fp = FileOpen (path, "r");
  if (fp == NULL)
  {
    Message (MSG_ERROR, "Unable to open %s", path);
    return NULL;
  }
      
  new_sep_list = ImportSequencesFromFile (fp, NULL, TRUE, TRUE, NULL);

  if (new_sep_list == NULL)
  {
    Message (MSG_ERROR, "Unable to read sequences");
    return NULL;
  }
  else if (! RejectZeroLengthSequences (&new_sep_list))
  {
    return NULL;
  }
  else if (!CollectIDsAndTitles (new_sep_list, current_list))
  {    
    while (new_sep != NULL)
    {
      test_sep = new_sep->next;
      SeqEntryFree (new_sep);
      new_sep = test_sep;
    }
    FileClose (fp);
    return NULL;
  }
  
  /* fix bad bracketing and unrecognized modifiers */
  FixDefinitionLines (new_sep_list, &cancelled, &fixes);
  fixes = FreeStringPairList (fixes);
  
  if (cancelled)
  {
    while (new_sep != NULL)
    {
      test_sep = new_sep->next;
      SeqEntryFree (new_sep);
      new_sep = test_sep;
    }
    FileClose (fp);
    return NULL;
  }
     
  FileClose (fp);

  return new_sep_list;
}

static SeqEntryPtr GetSequencesFromText (TexT t, SeqEntryPtr current_list)
{
  CharPtr      seq_str;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep_list;
  FILE *fp;

  seq_str = SaveStringFromText (t);

  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp == NULL) return;
  fprintf (fp, "%s", seq_str);
  FileClose (fp);
  
  seq_str = MemFree (seq_str);
  
  sep_list = GetSequencesFromFile (path, current_list);
  FileRemove (path);
  return sep_list;
}

static ValNodePtr ReadLinesOfFile (CharPtr path)
{
  ReadBufferData    rbd;
  CharPtr           line;
  FILE              *f;
  ValNodePtr        line_list = NULL;
  
  f = FileOpen (path, "r");
  if (f == NULL)
  {
    Message (MSG_ERROR, "Unable to open %s", path);
    return NULL;
  }
  
  rbd.fp = f;
  rbd.current_data = NULL;
  line = AbstractReadFunction (&rbd);
  while (line != NULL)
  {
    ValNodeAddPointer (&line_list, 0, line);
    line = AbstractReadFunction (&rbd);
  }
  FileClose (f);
  return line_list;
}

static void AddLineListToText (ValNodePtr line_list, TexT t)
{
  CharPtr              old_seqstr, new_seqstr;
  Int4                 len;
  ValNodePtr           vnp;

  if (line_list == NULL || t == NULL)
  {
    return;
  }

  old_seqstr = SaveStringFromText (t);
  len = StringLen (old_seqstr) + 1;
  for (vnp = line_list; vnp != NULL; vnp = vnp->next)
  {
    len += StringLen (vnp->data.ptrvalue) + 3;
  }
  
  new_seqstr = (CharPtr) MemNew (len * sizeof (Char));
  if (new_seqstr != NULL)
  {
    StringCpy (new_seqstr, old_seqstr);
    StringCat (new_seqstr, "\n");
    for (vnp = line_list; vnp != NULL; vnp = vnp->next)
    {
      StringCat (new_seqstr, vnp->data.ptrvalue);
      StringCat (new_seqstr, "\n");
    }
    SetTitle (t, new_seqstr);
    new_seqstr = MemFree (new_seqstr);
  }
  old_seqstr = MemFree (old_seqstr);
}

static void AddSequenceImportFasta (ButtoN b)
{
  CharPtr              extension;
  Char                 path [PATH_MAX];
  TexT                 t;
  ValNodePtr           line_list;
  
  t = (TexT) GetObjectExtra (b);
  if (t == NULL)
  {
    return;
  }
  
  /* get filename from user */
  extension = GetAppProperty ("FastaNucExtension");
  if (! GetInputFileName (path, sizeof (path), extension, "TEXT")) return;
  
  line_list = ReadLinesOfFile (path);
  AddLineListToText (line_list, t);
  ValNodeFreeData (line_list);
}

static void CheckSequenceAssistantCharInput (TexT t)
{
  SequenceAssistantPtr sap;
  CharPtr              seq_str;
  CharPtr              found_bracket, found_next_bracket;
  CharPtr              found_ret;
  Int4                 num_seq = 0;
  MsgAnswer            ans;
  /* TODO - parseSeqId should be calculated based on the uniqueness of what falls after the bracket */
  Boolean              parseSeqId = TRUE;
  
  sap = (SequenceAssistantPtr) GetObjectExtra (t);
  if (sap == NULL) return;
  
  seq_str = SaveStringFromText (t);
  found_bracket = StringChr (seq_str, '>');
  if (found_bracket == NULL || *(found_bracket + 1) == 0
      || found_bracket != seq_str)
  {
    MemFree (seq_str);
    return;
  }
  found_ret = StringChr (found_bracket, '\n');
  if (found_ret == NULL)
  {
    MemFree (seq_str);
    return;
  }
  if (*(found_ret + 1) == 0)
  {
    MemFree (seq_str);
    return;
  }
  
  while (found_bracket != NULL)
  {
    found_ret = StringChr (found_bracket, '\n');
    if (found_ret == NULL)
    {
      /* last line is a defline */
      found_next_bracket = NULL;
    }
    else
    {
      found_next_bracket = StringChr (found_ret, '>');
    }
    num_seq++;
    found_bracket = found_next_bracket;
  }
  ans = Message (MSG_YN, "You are pasting in %d sequences, correct?", num_seq);
  if (ans == ANS_YES)
  {
    SetTitle (t, StringSave (""));

    ImportSequenceAssistantEditData (sap, seq_str);
    UpdateSequenceAssistant (sap);
  }
  MemFree (seq_str); 
}

static CharPtr GetSequenceString (SeqEntryPtr sep)
{
  CharPtr     seqbuf;
  BioseqPtr   bsp;
  SeqPortPtr  spp;
  Int2        ctr;
  
  if (sep == NULL || ! IS_Bioseq (sep))
  {
    return NULL;
  }
  
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL || bsp->length < 1) 
  {
    return NULL;
  }

  spp = SeqPortNew (bsp, 0, bsp->length - 1, Seq_strand_plus, Seq_code_iupacna);
  seqbuf = (CharPtr) MemNew ((bsp->length + 1) * sizeof (Char));
  if (seqbuf != NULL)
  {
    SeqPortSeek (spp, 0, SEEK_SET);
    ctr = SeqPortRead (spp, (UcharPtr)seqbuf, bsp->length);
    seqbuf[ctr] = 0;
  }
  
  return seqbuf;
}

static CharPtr ReformatSequenceText (CharPtr seq_text)
{
  CharPtr src, dst;
  CharPtr new_text;
  Int4    num_lines;
  Int4    len;
  Int4    line_len = 80;
  Int4    counter;

  if (StringHasNoText (seq_text))
  {
  	MemFree (seq_text);
  	return NULL;
  }
  len = StringLen (seq_text);
  num_lines = len / line_len;
  len += num_lines + 2;
  new_text = (CharPtr) MemNew (len * sizeof (Char));
  if (new_text == NULL)
  {
  	return seq_text;
  }
  dst = new_text;
  counter = 0;
  for (src = seq_text; *src != 0; src++)
  {
  	if (!isspace ((Int4)(*src)))
  	{
  	  *dst = *src;
  	  dst++;
  	  counter++;
  	  if (counter == line_len)
  	  {
  	  	*dst = '\n';
  	  	dst++;
  	  	counter = 0;
  	  }
  	}
  }
  *dst = 0;
  MemFree (seq_text);
  return new_text;
}

static void FixStringForByteStore (CharPtr seq_str)
{
  CharPtr cp_src, cp_dst;
  
  if (seq_str == NULL)
  {
    return;
  }
  
  cp_src = seq_str;
  cp_dst = seq_str;
  while (*cp_src != 0)
  {
    if (isalpha (*cp_src))
    {
      *cp_dst = TO_UPPER (*cp_src);
      cp_dst++;
    }
    cp_src++;
  }
  *cp_dst = 0;
}

static Boolean 
IsDuplicateID (SeqEntryPtr seq_list, BioseqPtr edit_bsp, SeqIdPtr sip)
{
  SeqEntryPtr  sep;
  BioseqPtr    bsp;
  BioseqSetPtr bssp;
  SeqIdPtr     tmp_sip;
  Boolean      is_dup = FALSE;
  
  if (seq_list == NULL || sip == NULL)
  {
    return FALSE;
  }
  
  for (sep = seq_list; sep != NULL && !is_dup; sep = sep->next)
  {
    if (sep->data.ptrvalue == NULL)
    {
      continue;
    }
    if (IS_Bioseq (sep)) 
    {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (bsp != edit_bsp)
      {
        for (tmp_sip = sip; tmp_sip != NULL; tmp_sip = tmp_sip->next)
        {
          if (SeqIdIn (tmp_sip, bsp->id))
          {
            is_dup = TRUE;
          }  
        }
      }
    } 
    else if (IS_Bioseq_set (sep)) 
    {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      is_dup |= IsDuplicateID (bssp->seq_set, edit_bsp, sip);
    }
  }
  return is_dup;
}

static Boolean CheckBracketingInDefLine (CharPtr PNTR defline, CharPtr id_txt)
{
  SeqDescr              tmp_sd;
  Boolean               cancelled = FALSE;

  if (defline == NULL || id_txt == NULL)
  {
    return FALSE;
  }
  
  tmp_sd.next = NULL;
  tmp_sd.choice = Seq_descr_title;
  tmp_sd.data.ptrvalue = StringSave (*defline);

  FixBracketingInOneDefinitionLine (&tmp_sd, &cancelled, id_txt);

  if (cancelled)
  {
    tmp_sd.data.ptrvalue = MemFree (tmp_sd.data.ptrvalue);
  }
  else
  {
    *defline = MemFree (*defline);
    *defline = tmp_sd.data.ptrvalue;
  }
  return !cancelled;
}

static void SequenceAssistantAddSequence (SequenceAssistantPtr sap)
{
  ModalAcceptCancelData acd;
  WindoW                w;
  GrouP                 h, sequence_grp, c;
  BioseqPtr             bsp = NULL;
  ButtoN                import_fasta_btn;
  TexT                  sequence_txt;
  CharPtr               ttl = NULL;
  Char                  str [200];
  ButtoN                b;
  SeqEntryPtr           last_sep = NULL, new_sep;
  CharPtr     errormsg = NULL;
  ValNodePtr  err_list = NULL;
  Boolean     done = FALSE;
  ByteStorePtr          new_bs = NULL;
  SeqIdPtr              new_sip = NULL;
  
  if (sap == NULL)
  {
    return;
  }
  
  w = MovableModalWindow (-20, -13, -10, -10, "Add New Sequence", NULL);
  h = HiddenGroup(w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  import_fasta_btn = PushButton (h, "Import Nucleotide FASTA", AddSequenceImportFasta);
  
  sequence_grp = NormalGroup (h, 1, 0, "Sequence Characters", programFont, NULL);
  SetGroupSpacing (sequence_grp, 10, 10);
  StaticPrompt (sequence_grp, "Paste or type the nucleotide sequence.", 0, 
                popupMenuHeight, programFont, 'l');
  sequence_txt = ScrollText (sequence_grp, 60, 10, programFont, FALSE, NULL);
  SetObjectExtra (sequence_txt, sap, NULL);
  
  SetObjectExtra (import_fasta_btn, sequence_txt, NULL);
  
  sprintf (str, "You may only use the valid IUPAC characters (%s).", 
                valid_iupac_characters);
  MultiLinePrompt (sequence_grp, str, 60 * stdCharWidth, programFont);
    
  c = HiddenGroup (h, 2, 0, NULL);
  b = PushButton(c, "Save", ModalAcceptButton);
  SetObjectExtra (b, &acd, NULL);
  b = PushButton(c, "Cancel", ModalCancelButton);
  SetObjectExtra (b, &acd, NULL);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) import_fasta_btn,
                              (HANDLE) sequence_grp,
                              (HANDLE) c, 
                              NULL);
                              
  Show(w); 
  Select (w);

  while (!done)
  {
    acd.accepted = FALSE;
    acd.cancelled = FALSE;  
    while (!acd.accepted && ! acd.cancelled)
    {
      ProcessExternalEvent ();
      Update ();
    }
    ProcessAnEvent ();
  
    if (acd.accepted)
    {  
      new_sep = GetSequencesFromText (sequence_txt, sap->seq_list);
      if (new_sep != NULL)
      {
        ValNodeLink (&(sap->seq_list), new_sep);
        UpdateSequenceAssistant (sap);
        done = TRUE;
      }
    }
    else if (acd.cancelled)
    {
      done = TRUE;
    }
    acd.accepted = FALSE;
    acd.cancelled = TRUE;
  }
  
  Remove (w);
}

static void SequenceAssistantEditSequence (SequenceAssistantPtr sap, Int4 seq_num)
{
  ModalAcceptCancelData acd;
  WindoW                w;
  GrouP                 h, g1 = NULL, g2, sequence_grp, c;
  SeqEntryPtr           sep;
  Int4                  i;
  Char                  window_title [150];
  BioseqPtr             bsp = NULL;
  SeqIdPtr              sip;
  Char                  tmp [128];
  TexT                  sequence_id_txt, title_txt, sequence_txt;
  CharPtr               ttl = NULL;
  Char                  str [200];
  ButtoN                b;
  CharPtr               seqbuf;
  SeqEntryPtr           last_sep = NULL, new_sep;
  CharPtr               id_str, title_str, seq_str;
  CharPtr               errormsg = NULL;
  ValNodePtr            err_list = NULL;
  Int2                  seq_num_for_error = seq_num;
  Boolean               done = FALSE;
  ByteStorePtr          new_bs = NULL;
  SeqIdPtr              new_sip = NULL;
  SeqDescrPtr           sdp;
  
  if (sap == NULL)
  {
    return;
  }
  
  for (i = 0, sep = sap->seq_list; i != seq_num && sep != NULL; i++, sep = sep->next)
  {
  }
  
  if (sep != NULL)
  {
    if (sep->data.ptrvalue == NULL)
    {
      return;
    }
    else if (IS_Bioseq_set (sep))
    {
      Message (MSG_ERROR, "Can't edit segmented set!");
      return;
    }
    else if (! IS_Bioseq (sep))
    {
      return;
    }
    bsp = sep->data.ptrvalue;
    if (bsp->repr == Seq_repr_delta)
    {
      Message (MSG_ERROR, "Can't edit delta sequence!");
      return;
    }
    sip = SeqIdFindWorst (bsp->id);
    SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp) - 1);    
    sprintf (window_title, "Edit %s", tmp);
  }
  else
  {
    sprintf (window_title, "Add new sequence");
  }
  
  w = MovableModalWindow (-20, -13, -10, -10, window_title, NULL);
  h = HiddenGroup(w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
  
  /* users can enter descriptions and IDs for individual sequences */
  g2 = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (g2, "Sequence ID", 0, popupMenuHeight, programFont, 'l');
  sequence_id_txt = DialogText (g2, "", 20, NULL);
  if (bsp != NULL)
  {
    SetTitle (sequence_id_txt, tmp);
  }
  StaticPrompt (g2, "Sequence Title", 0, popupMenuHeight, programFont, 'l');
  title_txt = DialogText (g2, "", 20, NULL);
  if (sep != NULL)
  {
    SeqEntryExplore (sep, (Pointer) (&ttl), FindFirstTitle);
    if (!StringHasNoText (ttl))
    {
      SetTitle (title_txt, ttl);
    }
  }
  
  sequence_grp = NormalGroup (h, 1, 0, "Sequence Characters", programFont, NULL);
  SetGroupSpacing (sequence_grp, 10, 10);
  StaticPrompt (sequence_grp, "Paste or type the nucleotide sequence.", 0, 
                popupMenuHeight, programFont, 'l');
  sequence_txt = ScrollText (sequence_grp, 60, 10, programFont, FALSE, CheckSequenceAssistantCharInput);
  SetObjectExtra (sequence_txt, sap, NULL);
  seqbuf = GetSequenceString (sep);
  seqbuf = ReformatSequenceText (seqbuf);

  SetTitle (sequence_txt, seqbuf);
  MemFree (seqbuf);
  
  sprintf (str, "You may only use the valid IUPAC characters (%s).", 
                valid_iupac_characters);
  MultiLinePrompt (sequence_grp, str, 60 * stdCharWidth, programFont);
    
  c = HiddenGroup (h, 2, 0, NULL);
  b = PushButton(c, "Save", ModalAcceptButton);
  SetObjectExtra (b, &acd, NULL);
  b = PushButton(c, "Cancel", ModalCancelButton);
  SetObjectExtra (b, &acd, NULL);
  
  if (g1 == NULL)
  {
    AlignObjects (ALIGN_CENTER, (HANDLE) g2, (HANDLE) sequence_grp,
                                (HANDLE) c, NULL);
  }
  else
  {
    AlignObjects (ALIGN_CENTER, (HANDLE) g1, (HANDLE) g2, (HANDLE) sequence_grp,
                                (HANDLE) c, NULL);
  }
                              
  Show(w); 
  Select (w);

  while (!done)
  {
    acd.accepted = FALSE;
    acd.cancelled = FALSE;  
    while (!acd.accepted && ! acd.cancelled)
    {
      ProcessExternalEvent ();
      Update ();
    }
    ProcessAnEvent ();
  
    if (acd.accepted)
    {  
      id_str = SaveStringFromText (sequence_id_txt);
      id_str = ReformatLocalId (id_str);
      title_str = SaveStringFromText (title_txt);
      seq_str = SaveStringFromText (sequence_txt);
      if (seq_num_for_error < 0)
      {
        seq_num_for_error = 0;
      }
  
      if (StringHasNoText (seq_str))
      {
        Message (MSG_ERROR, "You must supply sequence characters!");
      }
      else if (StringHasNoText (id_str))
      {
        Message (MSG_ERROR, "You must supply a sequence ID!");
      }
      else if (! CheckBracketingInDefLine (&title_str, id_str))
      {
        /* let user continue editing */
      }
      else
      {
        done = TRUE;
        if (!SeqCharsOk (seq_str, seq_num_for_error, id_str, &err_list))
        {
          if (!ContinueWithErrorList (err_list, TRUE))
          {
            done = FALSE;
          }
        }
        
        
        if (done)
        { 
          new_sip = MakeSeqID (id_str);
          if (IsDuplicateID (sap->seq_list, bsp, new_sip))
          {
            Message (MSG_ERROR,
                   "Sequence IDs must be unique within the record!  %s is a duplicate ID",
                   id_str);
            done = FALSE;
            new_sip = SeqIdFree (new_sip);
          }
        }
        
        if (done)
        {
          FixStringForByteStore (seq_str);
          new_bs = BSNew (1000);
          if (new_bs != NULL)
          {
            BSWrite (new_bs, (VoidPtr) seq_str, (Int4) StringLen (seq_str));
          }
          
          if (bsp == NULL)
          {
            /* create new Bioseq and add to list */
            bsp = BioseqNew ();
            
            new_sep = SeqEntryNew ();
            new_sep->choice = 1;
            new_sep->data.ptrvalue = bsp;
            for (sep = sap->seq_list;
                 sep != NULL;
                 sep = sep->next)
            {
              last_sep = sep;
            }
            if (last_sep == NULL)
            {
              sap->seq_list = new_sep;
            }
            else
            {
              last_sep->next = new_sep;
            }
          }

          /* replace ID */
          bsp->id = SeqIdFree (bsp->id);
          bsp->id = new_sip;
            
          /* replace title */
          for (sdp = bsp->descr;
               sdp != NULL && sdp->choice != Seq_descr_title; 
               sdp = sdp->next)
          {
          }
          if (sdp == NULL)
          {
            sdp = SeqDescrNew (bsp->descr);
            if (bsp->descr == NULL)
            {
              bsp->descr = sdp;
            }
            sdp->choice = Seq_descr_title;
          }
          sdp->data.ptrvalue = MemFree (sdp->data.ptrvalue);
          sdp->data.ptrvalue = StringSave (title_str);
          
          /* replace sequence data */
          bsp->seq_data = BSFree (bsp->seq_data);
          bsp->repr = Seq_repr_raw;
          bsp->mol = Seq_mol_na;
          bsp->seq_data_type = Seq_code_iupacna;            
          bsp->seq_data = new_bs;
          bsp->length = BSLen (new_bs);
          
          BioseqPack (bsp);

          UpdateSequenceAssistant (sap);          
        }
      }
      id_str = MemFree (id_str);
      title_str = MemFree (title_str);
      seq_str = MemFree (seq_str);
      err_list = ValNodeFreeData (err_list);
    }
    else if (acd.cancelled)
    {
      done = TRUE;
    }
    acd.accepted = FALSE;
    acd.cancelled = TRUE;
  }
  
  Remove (w);
}


static void SequenceAssistantOk (SequenceAssistantPtr sap)
{
  if (sap == NULL) return;
  sap->cancelled = FALSE;
  sap->done = TRUE;
}

static void SequenceAssistantOkButton (ButtoN b)
{
  SequenceAssistantPtr sap;
  
  sap = (SequenceAssistantPtr) GetObjectExtra (b);
  SequenceAssistantOk (sap);
}

static void SequenceAssistantOkItem (IteM i)
{
  SequenceAssistantPtr sap;
  
  sap = (SequenceAssistantPtr) GetObjectExtra (i);
  SequenceAssistantOk (sap);
}


static void SequenceAssistantCancel (SequenceAssistantPtr sap)
{  
  if (sap == NULL) return;
  
  if (Message (MSG_YN, 
      "Are you sure you want to cancel (and lose all your editing)?") != ANS_YES)
  {
    return;
  }
  sap->cancelled = TRUE;
  sap->done = TRUE;
}

static void SequenceAssistantCancelButton (ButtoN b)
{
  SequenceAssistantPtr sap;
  
  sap = (SequenceAssistantPtr) GetObjectExtra (b);
  SequenceAssistantCancel (sap);
}

static void SequenceAssistantCancelItem (IteM i)
{
  SequenceAssistantPtr sap;
  
  sap = (SequenceAssistantPtr) GetObjectExtra (i);
  SequenceAssistantCancel (sap);
}

static void DeleteAllSequences (SequenceAssistantPtr sap)
{
  SeqEntryPtr sep, next_sep;
  if (sap == NULL || sap->seq_list == NULL)
  {
    return;
  }
  if (Message (MSG_YN, "Are you sure you want to delete all of your sequences?") != ANS_YES)
  {
    return;
  }
  
  sep = sap->seq_list;
  while (sep != NULL) 
  {
    next_sep = sep->next;
    sep->next = NULL;
    SeqEntryFree (sep);
    sep = next_sep;
  }
  sap->seq_list = NULL;
  UpdateSequenceAssistant (sap);
}

static void DeleteAllSequencesButton (ButtoN b)
{
  SequenceAssistantPtr sap;
  
  sap = (SequenceAssistantPtr) GetObjectExtra (b);
  DeleteAllSequences (sap);
}

static void DeleteAllSequencesItem (IteM i)
{
  SequenceAssistantPtr sap;
  
  sap = (SequenceAssistantPtr) GetObjectExtra (i);
  DeleteAllSequences (sap);
}

static void SelectSequenceDoc (DoC d, PoinT pt)
{
  Int2      item, row, prevrow;
  SequenceAssistantPtr sap;
  
  sap = (SequenceAssistantPtr) GetObjectExtra (d);
  if (sap == NULL) return;
  
  MapDocPoint (d, pt, &item, &row, NULL, NULL);
  if (item > 0 && row > 0) {
    prevrow = sap->sequence_row;
    sap->sequence_row = item;
    if (item != prevrow)
    {
      if (prevrow != -1)
      {
        InvalDocRows (d, prevrow, 1, 1);
      }
      InvalDocRows (d, item, 1, 1);
    }  	
    Enable (sap->edit_btn);
    Enable (sap->delete_btn);
  }
}

static Boolean SequenceHighlight (DoC doc, Int2 item, Int2 row, Int2 col)
{
  SequenceAssistantPtr sap;
  
  sap = (SequenceAssistantPtr) GetObjectExtra (doc);
  if (sap == NULL) return FALSE;
  
  if (item == sap->sequence_row) return TRUE;
  return FALSE;
}

static void AddSequence (SequenceAssistantPtr sap)
{
  if (sap == NULL) return;
  SequenceAssistantAddSequence (sap);
}

static void AddSequenceButton (ButtoN b)
{
  SequenceAssistantPtr sap;
  
  sap = (SequenceAssistantPtr) GetObjectExtra (b);

  AddSequence (sap);
}

static void AddSequenceItem (IteM i)
{
  SequenceAssistantPtr sap;
  
  sap = (SequenceAssistantPtr) GetObjectExtra (i);
  AddSequence (sap);  
}

static Boolean ConfirmSequenceDelete (SeqEntryPtr sep)
{
  MsgAnswer            ans;
  BioseqPtr            bsp;
  SeqIdPtr             sip;
  Char                 tmp[128];
  SeqDescrPtr          sdp;
  
  if (sep == NULL)
  {
    return FALSE;
  }
  
  bsp = (BioseqPtr) sep->data.ptrvalue;
  sdp = bsp->descr;
  while (sdp != NULL && sdp->choice != Seq_descr_title)
  {
    sdp = sdp->next;
  }
  sip = SeqIdFindWorst (bsp->id);
  SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
  if (sdp != NULL && ! StringHasNoText (sdp->data.ptrvalue))
  {
    ans = Message (MSG_YN, "Are you sure you want to delete %s (%s)?", tmp, sdp->data.ptrvalue);
  }
  else
  {
    ans = Message (MSG_YN, "Are you sure you want to delete %s?", tmp);
  }
  if (ans == ANS_YES)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

static void DeleteSequence (SequenceAssistantPtr sap)
{
  Int2                 seq_num;
  SeqEntryPtr          sep, last_sep = NULL;
  
  if (sap == NULL || sap->seq_list == NULL) return;
  
  if (sap->seq_list != NULL)
  {
    for (sep = sap->seq_list, seq_num = 1;
         sep != NULL && seq_num < sap->sequence_row;
         sep = sep->next, seq_num++)
    {
      last_sep = sep;
    }
    if (!ConfirmSequenceDelete (sep))
    {
      return;
    }
    
    if (sep == NULL)
    {
      /* do nothing, deleted non-existent sequence */
    }
    else if (last_sep == NULL)
    {
      /* remove first in list */
      sap->seq_list = sap->seq_list->next;
      sep->next = NULL;
      sep = SeqEntryFree (sep);
    }
    else
    {
      last_sep->next = sep->next;
      sep->next = NULL;
      sep = SeqEntryFree (sep);
    }
    if (sap->sequence_row > 1)
    {
      sap->sequence_row --;
    }
  }
  UpdateSequenceAssistant (sap);
}

static void DeleteSequenceButton (ButtoN b)
{
  SequenceAssistantPtr sap;

  sap = (SequenceAssistantPtr) GetObjectExtra (b);
  DeleteSequence (sap);  
}

static void DeleteSequenceItem (IteM i)
{
  SequenceAssistantPtr sap;
  
  sap = (SequenceAssistantPtr) GetObjectExtra (i);
  DeleteSequence (sap);  
}

static void ImportFastaFileItem (IteM i)
{
  SequenceAssistantPtr sap;
  CharPtr              extension;
  Char                 path [PATH_MAX];
  SeqEntryPtr          new_sep_list;
  
  sap = (SequenceAssistantPtr) GetObjectExtra (i);
  if (sap == NULL)
  {
    return;
  }

  extension = GetAppProperty ("FastaNucExtension");
  if (! GetInputFileName (path, sizeof (path), extension, "TEXT")) return;
  
  new_sep_list = GetSequencesFromFile (path, sap->seq_list);
  if (new_sep_list != NULL
      && ImportedSequenceTypeOk (new_sep_list, sap->seqPackage))
  {
    ValNodeLink (&sap->seq_list, new_sep_list);
  }
  else
  {
    new_sep_list = SeqEntryFree (new_sep_list);
  }

  UpdateSequenceAssistant (sap);
}

static void ImportFastaFileButton (ButtoN b)
{
  SequenceAssistantPtr sap;
  CharPtr              extension;
  Char                 path [PATH_MAX];
  SeqEntryPtr          new_sep_list;
  
  sap = (SequenceAssistantPtr) GetObjectExtra (b);
  if (sap == NULL)
  {
    return;
  }

  extension = GetAppProperty ("FastaNucExtension");
  if (! GetInputFileName (path, sizeof (path), extension, "TEXT")) return;

  new_sep_list = GetSequencesFromFile (path, sap->seq_list);
  if (new_sep_list != NULL
      && ImportedSequenceTypeOk (new_sep_list, sap->seqPackage))
  {
    ValNodeLink (&sap->seq_list, new_sep_list);
  }
  else
  {
    new_sep_list = SeqEntryFree (new_sep_list);
  }

  UpdateSequenceAssistant (sap);
}

static SeqEntryPtr CopySeqEntryList (SeqEntryPtr seq_list)
{
  SeqEntryPtr new_seq_list, new_seq, last_seq;
  
  if (seq_list == NULL)
  {
    return NULL;
  }
  
  new_seq = AsnIoMemCopy ((Pointer) seq_list,
                          (AsnReadFunc) SeqEntryAsnRead,
                          (AsnWriteFunc) SeqEntryAsnWrite);
  new_seq_list = new_seq;
  last_seq = new_seq;
  
  seq_list = seq_list->next;
  
  while (last_seq != NULL && seq_list != NULL)
  {   
    new_seq = AsnIoMemCopy ((Pointer) seq_list,
                            (AsnReadFunc) SeqEntryAsnRead,
                            (AsnWriteFunc) SeqEntryAsnWrite);
    last_seq->next = new_seq;
    last_seq = last_seq->next;
    seq_list = seq_list->next;
  }
  
                          
  return new_seq_list;
}

static Boolean 
SequenceAssistantValidateSegments 
(SeqEntryPtr seq_list,
 ValNodePtr PNTR err_list)
{
  SeqEntryPtr sep;
  Boolean     all_one_char = TRUE;
  CharPtr     seqbuf;
  Char        first_seg_char = 0;
  Char        first_seg_char_this = 0;
  Boolean     rval = TRUE;
  Int4        total_len = 0;
  Boolean     non_N = FALSE;
  
  if (seq_list == NULL)
  {
    return TRUE;
  }
  
  for (sep = seq_list; sep != NULL; sep = sep->next)
  {
    seqbuf = GetSequenceString (sep);
    if (!StringHasNoText (seqbuf))
    {
      if (all_one_char)
      {
        if (IsSequenceAllOneCharacter(seqbuf))
        {
          if (first_seg_char == 0)
          {
            first_seg_char = seqbuf[StringSpn(seqbuf, " \t\n")];
          }
          else
          {
            first_seg_char_this = seqbuf[StringSpn(seqbuf, " \t\n")];
            if (first_seg_char_this != first_seg_char)
            {
              all_one_char = FALSE;
            }
          }
        }
        else
        {
          all_one_char = FALSE;
        }
      }
      
      total_len += CountSeqChars (seqbuf);
      non_N |= ! IsSequenceAllNs (seqbuf);
    }
    seqbuf = MemFree (seqbuf);
  }
  if (all_one_char && err_list != NULL)
  {
    ValNodeAddPointer (err_list, CREATE_FASTA_WARNING,
             StringSave ("Your segmented set sequences all consist entirely of the same character."));
  }
  if (!non_N)
  {
    if (err_list != NULL)
    {
      ValNodeAddPointer (err_list, CREATE_FASTA_REQUIRED, 
                         StringSave ("Your segmented set consists entirely of Ns. "
                         "This is not a valid sequence.  Please edit."));

    }
    rval = FALSE;
  }
  if (total_len < 50)
  {
    if (err_list != NULL)
    {
      /* Note - this is a required error because only small molecules can have
       * less than 50 base pairs, and the small molecules should never have
       * been sequenced in segments.
       */
      ValNodeAddPointer (err_list, CREATE_FASTA_REQUIRED, StringSave (
                     "You have fewer than 50 total base pairs in this "
                     "segmented set. GenBank will not accept segmented sets with "
                     "fewer than 50 base pairs. Please edit your sequence."));
    }
    rval = FALSE;
  }
  return rval;
}

static Boolean 
SequenceAssistantValidateSegmentedSetContentAndLength 
(SeqEntryPtr     seq_list,
 ValNodePtr PNTR err_list)
{
  BioseqPtr    bsp;
  BioseqSetPtr parts;
  
  if (seq_list == NULL)
  {
    return TRUE;
  }
  else if (IS_Bioseq (seq_list))
  {
    bsp = (BioseqPtr) seq_list;
    if (bsp->repr == Seq_repr_seg)
    {
      if (seq_list->next != NULL 
          && IS_Bioseq_set (seq_list->next)
          && seq_list->next->data.ptrvalue != NULL)
      {
        parts = (BioseqSetPtr) seq_list->next->data.ptrvalue;
        return SequenceAssistantValidateSegments (parts->seq_set, err_list);
      }
      else
      {
        return SequenceAssistantValidateSegments (seq_list, err_list);
      }
    }
    else
    {
      return SequenceAssistantValidateSegments (seq_list, err_list);
    }
  }
  else
  {
    return TRUE;
  }
}

static Boolean 
SequenceAssistantValidateNotSegmentedSetContentAndLength 
(SeqEntryPtr     seq_list,
 ValNodePtr PNTR err_list)
{
  CharPtr     seqbuf;
  Boolean     rval = TRUE;
  SeqEntryPtr sep, nsep;
  Int2        seq_num = 0;
  ValNodePtr  too_short_list = NULL;
  ValNodePtr  all_one_char_list = NULL;
  ValNodePtr  all_N_list = NULL;
  Char        id_str [128];
  CharPtr     err_msg;
  BioseqPtr   bsp;
  SeqIdPtr    sip;
  
  if (seq_list == NULL)
  {
    return TRUE;
  }
  
  for (sep = seq_list; sep != NULL; sep = sep->next, seq_num++)
  {
    nsep = FindNucSeqEntry (sep);
    if (nsep == NULL)
    {
      continue;
    }
    seqbuf = GetSequenceString (nsep);
    bsp = (BioseqPtr) nsep->data.ptrvalue;
    sip = SeqIdFindWorst (bsp->id);
    SeqIdWrite (sip, id_str, PRINTID_REPORT, sizeof (id_str));

    if (CountSeqChars (seqbuf) < 50)
    {
      ValNodeAddPointer (&too_short_list, seq_num, StringSave (id_str));
    }
    if (IsSequenceAllNs (seqbuf))
    {
      ValNodeAddPointer (&all_N_list, seq_num, StringSave (id_str));
      rval = FALSE;
    }
    else if (IsSequenceAllOneCharacter(seqbuf))
    {
      ValNodeAddPointer (&all_one_char_list, seq_num, StringSave (id_str));
    }
  }
  if (all_N_list != NULL)
  {
    if (err_list != NULL)
    {
      if (all_N_list->next == NULL)
      {
        err_msg = CreateListMessage ("In sequence", 
                     " there are only Ns. This not a valid sequence, please edit it.",
                     all_N_list);
      }
      else
      {
        err_msg = CreateListMessage ("In sequence", 
                   " there are only Ns. These are not valid sequences, please edit them.",
                     all_N_list);
      }
      ValNodeAddPointer (err_list, CREATE_FASTA_REQUIRED, err_msg);
    }
    rval = FALSE;
  }
  if (all_one_char_list != NULL)
  {
    if (err_list != NULL)
    {
      if (all_one_char_list->next == NULL)
      {
        err_msg = CreateListMessage ("In sequence", 
                   " one character is repeated for the entire sequence. This is not a valid sequence, please edit it.",
                   all_one_char_list);
      }
      else
      {
        err_msg = CreateListMessage ("In sequence", 
                     " one character is repeated for the entire sequence. These are not valid sequences, please edit them.",
                     all_one_char_list);
      }
      ValNodeAddPointer (err_list, CREATE_FASTA_WARNING, err_msg);
    }
    rval = FALSE;
  }
  if (too_short_list != NULL && err_list != NULL)
  {
    if (too_short_list->next == NULL)
    {
      err_msg = CreateListMessage ("Sequence", " is shorter than 50 base pairs. "
                     "GenBank will not accept sequences with "
                     "fewer than 50 base pairs. Please edit your sequence or "
                     "make sure that your comment explains why your sequence "
                     "is so short.",
                     too_short_list);
    }
    else
    {
      err_msg = CreateListMessage ("Sequence", " are shorter than 50 base pairs. "
                     "GenBank will not accept sequences with "
                     "fewer than 50 base pairs. Please edit your sequences or "
                     "make sure that your comments explain why your sequences "
                     "are so short.",
                     too_short_list);
    }
    ValNodeAddPointer (err_list, CREATE_FASTA_WARNING, err_msg);
  }
  all_N_list = ValNodeFreeData (all_N_list);
  too_short_list = ValNodeFreeData (too_short_list);
  all_one_char_list = ValNodeFreeData (all_one_char_list);
  
  return rval;
}

/* This function will add to a list of errors.  Any errors in the list with a choice of 0
 * cause the sequence to be unacceptable.  Any errors in the list with a choice of 1 are
 * a yes-no question - yes means that the user wants to go back and correct the problems.
 * no means the user would like to continue anyway.
 */
static Boolean 
SequenceAssistantValidateContentAndLength 
(SeqEntryPtr     seq_list,
 Int2            seqPackage,
 ValNodePtr PNTR err_list)
{
  Boolean           rval = TRUE;
  
  if (seq_list == NULL)
  {
    return TRUE;
  }

  if (seqPackage == SEQ_PKG_SEGMENTED)
  {
    rval = SequenceAssistantValidateSegmentedSetContentAndLength (seq_list, err_list);
  }
  else
  {
    rval = SequenceAssistantValidateNotSegmentedSetContentAndLength (seq_list, err_list);
  }
  return rval;
}

static Boolean SequenceAssistantValidate (SeqEntryPtr seq_list, Int2 seqPackage)
{
  ValNodePtr err_list = NULL;
  Boolean    rval;
  
  rval = SequenceAssistantValidateContentAndLength (seq_list, seqPackage, &err_list);

  if (err_list != NULL)
  {
    rval = ContinueWithErrorList (err_list, TRUE);
  }
  err_list = ValNodeFreeData (err_list);
  return rval;
}

static void EditOneSequenceTitle (SequenceAssistantPtr sap, Int4 seq_num)
{
  Int4                  seq_pos;
  SeqEntryPtr           sep, nsep;
  WindoW                w;
  CharPtr               title = NULL;
  BioseqPtr             bsp = NULL;
  CharPtr               title_fmt = "Title for %s";
  Char                  id_txt [128];
  GrouP                 h, g, c;
  ButtoN                b;
  ModalAcceptCancelData acd;
  TexT                  new_title_txt;
  SeqDescrPtr           sdp;
  CharPtr               old_txt;
  Boolean               done = FALSE, cancelled;
  
  if (sap == NULL)
  {
    return;
  }
  
  for (seq_pos = 0, sep = sap->seq_list;
       seq_pos != seq_num && sep != NULL;
       seq_pos ++, sep = sep->next)
  {
  }
  if (sep == NULL)
  {
    return;
  }
  
  if (IS_Bioseq (sep)) 
  {
    bsp = (BioseqPtr) sep->data.ptrvalue;
  } 
  else if (IS_Bioseq_set (sep)) 
  {
    nsep = FindNucSeqEntry (sep);
    if (nsep != NULL && IS_Bioseq (nsep)) 
    {
      bsp = (BioseqPtr) nsep->data.ptrvalue;
    }
  }
  if (bsp == NULL)
  {
    return;
  }
  
  sdp = bsp->descr;
  while (sdp != NULL && sdp->choice != Seq_descr_title)
  {
    sdp = sdp->next;
  }
  
  SeqIdWrite (SeqIdFindWorst (bsp->id), id_txt, PRINTID_REPORT, sizeof (id_txt));
  
  title = (CharPtr) MemNew ((StringLen (title_fmt) 
                               + StringLen (id_txt)) * sizeof (Char));
  sprintf (title, title_fmt, id_txt);
  w = MovableModalWindow (-20, -13, -10, -10, title, NULL);
  title = MemFree (title);
  
  h = HiddenGroup(w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
  
  g = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (g, "Title", 0, popupMenuHeight, programFont, 'l');
  new_title_txt = DialogText (g, "", 20, NULL);
  if (sdp != NULL && !StringHasNoText (sdp->data.ptrvalue))
  {
    SetTitle (new_title_txt, sdp->data.ptrvalue);
  }
  
  c = HiddenGroup (h, 2, 0, NULL);
  b = PushButton (c, "Accept", ModalAcceptButton);
  SetObjectExtra (b, &acd, NULL);
  b = PushButton (c, "Cancel", ModalCancelButton);
  SetObjectExtra (b, &acd, NULL);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);

  Show(w); 
  Select (w);
  
  while (!done)
  {
    acd.accepted = FALSE;
    acd.cancelled = FALSE;
  
    while (!acd.cancelled && ! acd.accepted)
    {
      ProcessExternalEvent ();
      Update ();
    }
    ProcessAnEvent ();
    if (acd.accepted)
    {
      if (sdp == NULL)
      {
        sdp = SeqDescrNew (bsp->descr);
        if (bsp->descr == NULL)
        {
          bsp->descr = sdp;
        }
        sdp->choice = Seq_descr_title;
      }
      old_txt = sdp->data.ptrvalue;
      sdp->data.ptrvalue = SaveStringFromText (new_title_txt);
      cancelled = FALSE;
      FixBracketingInOneDefinitionLine (sdp, &cancelled, id_txt);
      FixUnrecognizedModifersInOneDefinitionLine (sdp, &cancelled, NULL);
      if (cancelled)
      {
        sdp->data.ptrvalue = MemFree (sdp->data.ptrvalue);
        sdp->data.ptrvalue = old_txt;
      }
      else
      {
        old_txt = MemFree (old_txt);
        done = TRUE;
      }
    }
    else if (acd.cancelled)
    {
      done = TRUE;
    }
  }
  
  Remove (w);
}

#define SEQUENCE_ASSISTANT_MOLECULE_COLUMN 2
#define SEQUENCE_ASSISTANT_TOPOLOGY_COLUMN 3
#define SEQUENCE_ASSISTANT_TITLE_COLUMN    4

static Uint2 titleedit_types [] = {
  TAGLIST_PROMPT, TAGLIST_TEXT
};

static Uint2 titleedit_widths [] = {
  10, 40,
};

static ValNodePtr 
BuildTagDataForSeqEntry 
(SeqEntryPtr sep,
 ValNodePtr  list)
{
  BioseqPtr   bsp;
  SeqDescrPtr sdp;
  Char        id_txt [128];
  Int4        len;
  CharPtr     str, title_txt;

  if (sep == NULL)
  {
    return list;
  }
  if (IS_Bioseq (sep))
  {
    bsp = (BioseqPtr) sep->data.ptrvalue;
  }
  else if (IS_Bioseq_set (sep))
  {
    sep = FindNucSeqEntry (sep);
    if (sep != NULL && IS_Bioseq (sep))
    {
      bsp = (BioseqPtr) sep->data.ptrvalue;
    }
  }
  
  if (bsp == NULL)
  {
    return list;
  }
  
  SeqIdWrite (SeqIdFindWorst (bsp->id), id_txt, PRINTID_REPORT, sizeof (id_txt));
  
  for (sdp = bsp->descr; sdp != NULL && sdp->choice != Seq_descr_title; sdp = sdp->next)
  {
  }
  
  if (sdp == NULL || StringHasNoText (sdp->data.ptrvalue))
  {
    title_txt = "";
  }
  else
  {
    title_txt = sdp->data.ptrvalue;
  }
    
  len = StringLen (id_txt) + StringLen (title_txt) + 4;
  str = (CharPtr) MemNew (len * sizeof (Char));
  if (str != NULL) {
    sprintf (str, "%s\t%s\n", id_txt, title_txt);
  }
  
  ValNodeAddPointer (&list, 0, str);
  return list;
}

static Boolean 
PrevalidateOneNewSequenceTitle 
(SeqEntryPtr     sep,
 SeqDescrPtr     sdp,
 ValNodePtr PNTR fixes)
{
  BioseqPtr bsp;
  SeqIdPtr  sip;
  Char      tmp [128];
  Boolean   cancelled = FALSE;
  
  if (sep == NULL || sdp == NULL)
  {
    return FALSE;
  }
  
  if (IS_Bioseq (sep))
  {
    bsp = (BioseqPtr) sep->data.ptrvalue;
  }
  else if (IS_Bioseq_set (sep))
  {
    sep = FindNucSeqEntry (sep);
    if (sep != NULL && IS_Bioseq (sep))
    {
      bsp = (BioseqPtr) sep->data.ptrvalue;
    }
  }
  
  if (bsp == NULL)
  {
    return FALSE;
  }

  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp != NULL)
  {
    sip = SeqIdFindWorst (bsp->id);
    SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
    FixBracketingInOneDefinitionLine (sdp, &cancelled, (CharPtr) tmp);
    FixUnrecognizedModifersInOneDefinitionLine (sdp, &cancelled, fixes);
  }
  return cancelled;
}

static void ReplaceOneDefinitionLine (SeqEntryPtr sep, CharPtr new_title)
{
  BioseqPtr   bsp;
  SeqDescrPtr sdp;

  if (sep == NULL)
  {
    return;
  }
  
  if (IS_Bioseq (sep))
  {
    bsp = (BioseqPtr) sep->data.ptrvalue;
  }
  else if (IS_Bioseq_set (sep))
  {
    sep = FindNucSeqEntry (sep);
    if (sep != NULL && IS_Bioseq (sep))
    {
      bsp = (BioseqPtr) sep->data.ptrvalue;
    }
  }
  
  if (bsp == NULL)
  {
    return;
  }
  
  for (sdp = bsp->descr; sdp != NULL && sdp->choice != Seq_descr_title; sdp = sdp->next)
  {
  }
  if (sdp == NULL)
  {
    sdp = SeqDescrNew (bsp->descr);
    if (bsp->descr == NULL)
    {
      bsp->descr = sdp;
    }
  }
  sdp->data.ptrvalue = MemFree (sdp->data.ptrvalue);
  sdp->data.ptrvalue = new_title;
}

static void EditSequenceTitleColumns (SequenceAssistantPtr sap)
{
  WindoW                w;
  GrouP                 h, c;
  PrompT                ppt;
  ButtoN                b;
  Int4                  rows_shown = 0, j;
  SeqEntryPtr           sep;
  DialoG                dlg;
  TagListPtr            tlp;
  ValNodePtr            list = NULL, vnp;
  ModalAcceptCancelData acd;
  Boolean               done = FALSE;
  CharPtr               new_title;
  ValNodePtr            new_title_list = NULL;
  ValNodePtr            fixes = NULL;
  Boolean               cancelled_fixes = FALSE;
  
  if (sap == NULL || sap->seq_list == NULL)
  {
    return;
  }

  /* count available rows, in order to present correct number of rows in tag dialog */
  /* also collect data for taglist */
  sep = sap->seq_list;
  while (sep != NULL)
  {
    rows_shown++;
    list = BuildTagDataForSeqEntry (sep, list);
    sep = sep->next;
  }
  rows_shown = MIN (rows_shown, 5);
    
  w = MovableModalWindow (-20, -13, -10, -10, "Sequence Titles", NULL);
  
  h = HiddenGroup(w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
  
  ppt = StaticPrompt (h, "Title", 18 * stdCharWidth, 0, programFont, 'l');    
  
  dlg = CreateTagListDialogEx (h, rows_shown, 2, 2,
                               titleedit_types, titleedit_widths,
                               NULL, TRUE, TRUE, NULL, NULL);

  tlp = (TagListPtr) GetObjectExtra (dlg);  
  if (tlp == NULL) return;

  SendMessageToDialog (tlp->dialog, VIB_MSG_RESET);
  tlp->vnp = list;
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

  c = HiddenGroup (h, 2, 0, NULL);
  b = PushButton (c, "Accept", ModalAcceptButton);
  SetObjectExtra (b, &acd, NULL);
  b = PushButton (c, "Cancel", ModalCancelButton);
  SetObjectExtra (b, &acd, NULL);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) ppt, (HANDLE) dlg, (HANDLE) c, (HANDLE) NULL);

  Show (w);
  Select (w);
  while (!done)
  {
    acd.accepted = FALSE;
    acd.cancelled = FALSE;
    while (!acd.accepted && ! acd.cancelled)
    {
      ProcessExternalEvent ();
      Update ();
    }
    ProcessAnEvent ();
    if (acd.cancelled)
    {
      done = TRUE;
    }
    else
    {
      tlp = GetObjectExtra (dlg);
      cancelled_fixes = FALSE;
      for (sep = sap->seq_list, j = 0;
           sep != NULL && ! cancelled_fixes; 
           sep = sep->next, j++)
      {
        new_title = GetTagListValue (tlp, j);
        vnp = ValNodeNew (new_title_list);
        if (new_title_list == NULL)
        {
          new_title_list = vnp;
        }
        vnp->choice = Seq_descr_title;
        vnp->data.ptrvalue = new_title;
        cancelled_fixes = PrevalidateOneNewSequenceTitle (sep, vnp, &fixes);
      }
      fixes = FreeStringPairList (fixes);
      if (cancelled_fixes)
      {
        new_title_list = ValNodeFreeData (new_title_list);
      }
      else
      {
        for (sep = sap->seq_list, vnp = new_title_list;
             sep != NULL && vnp != NULL;
             sep = sep->next, vnp = vnp->next)
        {
          ReplaceOneDefinitionLine (sep, vnp->data.ptrvalue);
        }
        /* only use ValNodeFree because we use the data in the title descriptor */
        new_title_list = ValNodeFree (new_title_list);
        UpdateSequenceAssistant (sap);  
        done = TRUE;
      }
    }
  }
  Remove (w);
}

static void SequenceDblClick (PoinT cell_coord, CharPtr header_text, CharPtr cell_text, Pointer userdata)
{
  SequenceAssistantPtr sap;
  
  sap = (SequenceAssistantPtr) userdata;
  if (sap == NULL)
  {
    return;
  }
  if (cell_coord.x < 2)
  {
    return;
  }
  if (cell_coord.y == 0)
  {
    if (cell_coord.x == SEQUENCE_ASSISTANT_MOLECULE_COLUMN)
    {
      /* edit all molecule types */
      EditOrgModColumn ("moltype", NULL, sap->seq_list, sap->seqPackage);
    }
    else if (cell_coord.x == SEQUENCE_ASSISTANT_TOPOLOGY_COLUMN)
    {
      /* edit all topologies */
      EditOrgModColumn ("topology", NULL, sap->seq_list, sap->seqPackage);
    }
    else
    {
      /* edit all titles */
      EditSequenceTitleColumns (sap);
    }
    UpdateSequenceAssistant (sap);
  }
  else
  {
    if (cell_coord.x == SEQUENCE_ASSISTANT_MOLECULE_COLUMN)
    {
      /* edit one molecule type */
      ApplyOrgModColumnOrCell ("moltype", cell_text, cell_coord.y - 1, NULL, sap->seq_list,
                               NULL, 0, sap->seqPackage);
    }
    else if (cell_coord.x == SEQUENCE_ASSISTANT_TOPOLOGY_COLUMN)
    {
      /* edit one topology */
      ApplyOrgModColumnOrCell ("topology", cell_text, cell_coord.y - 1, NULL, sap->seq_list,
                               NULL, 0, sap->seqPackage);
    }
    else
    {
      /* edit one title */
      EditOneSequenceTitle (sap, cell_coord.y - 1);
    }
    UpdateSequenceAssistant (sap);
  }
}

static void SequenceAssistantEditButton (ButtoN b)
{
  SequenceAssistantPtr sap;
  
  sap = (SequenceAssistantPtr) GetObjectExtra (b);
  if (sap == NULL)
  {
    return;
  }
  
  SequenceAssistantEditSequence (sap, sap->sequence_row - 1);
}

static void SequenceAssistant (ButtoN b)
{
  SequencesFormPtr      sqfp;
  SequenceAssistantData sad;
  WindoW                w;
  MenU                  m;
  IteM                  i;
  GrouP                 h, k, edit_grp, selector_grp, c;
  FastaPagePtr          fpp;
  RecT                  r;
  SeqEntryPtr           sep, next_sep;
  ButtoN                add_btn;
  
  sqfp = (SequencesFormPtr) GetObjectExtra (b);
  if (sqfp == NULL) 
  {
    return;
  }
  
  fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
  if (fpp == NULL) 
  {
    return;
  }
  
  sad.done = FALSE;
  sad.cancelled = FALSE;
  
  sad.seq_list = CopySeqEntryList (fpp->list);
  sad.sequence_row = -1;
  
  sad.seqPackage = sqfp->seqPackage;
    
  w = MovableModalWindow (-20, -13, -10, -10, "Specify Sequences", NULL);
  h = HiddenGroup(w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  /* add menus */  
  m = PulldownMenu (w, "File");
  i = CommandItem (m, "Import FASTA file", ImportFastaFileItem);
  SetObjectExtra (i, &sad, NULL);
  i = CommandItem (m, "Done", SequenceAssistantOkItem);
  SetObjectExtra (i, &sad, NULL);
  i = CommandItem (m, "Cancel", SequenceAssistantCancelItem);
  SetObjectExtra (i, &sad, NULL);
  
  /* edit menu */
  m = PulldownMenu (w, "Edit");
  i = CommandItem (m, "Add Sequence", AddSequenceItem);
  SetObjectExtra (i, &sad, NULL);
  i = CommandItem (m, "Delete Sequence", DeleteSequenceItem);
  SetObjectExtra (i, &sad, NULL);
  i = CommandItem (m, "Delete All Sequences", DeleteAllSequencesItem);
  SetObjectExtra (i, &sad, NULL);
    
  k = HiddenGroup (h, 2, 0, NULL);  
  sad.import_btn = PushButton (k, "Import Additional Nucleotide FASTA", ImportFastaFileButton);
  SetObjectExtra (sad.import_btn, &sad, NULL);
  
  add_btn = PushButton (k, "Add New Sequence", AddSequenceButton);
  SetObjectExtra (add_btn, &sad, NULL);
  
  edit_grp = HiddenGroup (h, 2, 0, NULL);
  sad.sequence_selector = DocumentPanel (edit_grp, stdCharWidth * 10, stdLineHeight * 6);
  SetObjectExtra (sad.sequence_selector, &sad, NULL);
  SetDocProcs (sad.sequence_selector, SelectSequenceDoc, NULL, NULL, NULL);
  SetDocShade (sad.sequence_selector, NULL, NULL, SequenceHighlight, NULL);
  selector_grp = HiddenGroup (edit_grp, 0, 4, NULL);
  sad.edit_btn = PushButton (selector_grp, "Edit Sequence", SequenceAssistantEditButton);
  SetObjectExtra (sad.edit_btn, &sad, NULL);
  StaticPrompt (selector_grp, "", 0, popupMenuHeight, programFont, 'l');
  sad.delete_btn = PushButton (selector_grp, "Delete Sequence", DeleteSequenceButton);
  SetObjectExtra (sad.delete_btn, &sad, NULL);
  sad.delete_all_btn = PushButton (selector_grp, "Delete All Sequences", DeleteAllSequencesButton);
  SetObjectExtra (sad.delete_all_btn, &sad, NULL);

  sad.summary_dlg = FastaSummaryDialog (h);
  
  sad.sequence_table = TableDisplayDialog (h, stdCharWidth * 27, stdLineHeight * 8, 1, 2,
                                       SequenceDblClick, &sad);
  
  c = HiddenGroup (h, 2, 0, NULL);
  b = PushButton(c, "Done", SequenceAssistantOkButton);
  SetObjectExtra (b, &sad, NULL);
  b = PushButton(c, "Cancel", SequenceAssistantCancelButton);
  SetObjectExtra (b, &sad, NULL);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) k,
                              (HANDLE) edit_grp,
                              (HANDLE) sad.summary_dlg,
                              (HANDLE) sad.sequence_table,
                              (HANDLE) c, NULL);

  UpdateSequenceAssistant (&sad);
                                
  Show(w); 
  Select (w);
  
  while (!sad.done)
  {
    while (!sad.done)
    {
      ProcessExternalEvent ();
      Update ();
    }
    ProcessAnEvent ();
    if (!sad.cancelled)
    {

      if (SequenceAssistantValidate (sad.seq_list, sqfp->seqPackage))
      {
        /* check for number of sequences */
        if (sad.seq_list != NULL && sad.seq_list->next != NULL
            && PackageTypeIsSingle (sqfp->seqPackage))
        {
          if (Message (MSG_YN, "You are importing multiple sequences - did you intend to create a batch submission?") == ANS_YES)
          {
            sqfp->seqPackage = SEQ_PKG_GENBANK;
            fpp->single = FALSE;
          }
          else
          {
            sad.done = FALSE;
            sad.cancelled = FALSE;
          }
        }
      }
      else
      {
        sad.done = FALSE;
        sad.cancelled = FALSE;
      }
      
      if (sad.done)
      {
        ResetFastaPage (fpp);
        Reset (fpp->doc);
        if (sad.seq_list == NULL)
        {          
          SafeHide (fpp->doc);
          SafeShow (fpp->instructions);
          Update ();
          Enable (fpp->import_btn);
          Disable (fpp->clear_btn);
          ClearOrganismModifiers (sqfp);
        }
        else
        {    
          /* these statements make sure the column width is large enough */
          ObjectRect (fpp->doc, &r);
          InsetRect (&r, 4, 4);
          faColFmt.pixWidth = r.right - r.left;
 
          fpp->list = sad.seq_list;
          SafeHide (fpp->instructions);
          Update ();
          Disable (fpp->import_btn);
          Enable (fpp->clear_btn);
          FormatFastaDoc (fpp);
          SafeShow (fpp->doc);
          NucleotideImportFinish (sqfp);
        }      
      }
      else
      {
        sad.cancelled = FALSE;
        sad.done = FALSE;
      }  
    }
    else
    {
      /* clean up list of sequences from form, since they will not be used */
      sep = sad.seq_list;
      while (sep != NULL) 
      {
        next_sep = sep->next;
        sep->next = NULL;
        SeqEntryFree (sep);
        sep = next_sep;
      }
      sad.seq_list = NULL;
    }
  }
  Remove (w);
}

static void SpecifyMolecule (ButtoN b)
{
  SpecifyModValueButton (b, "moltype");
}

static void SpecifyTopology (ButtoN b)
{
  SpecifyModValueButton (b, "topology");
}

static GrouP CreateNucleotideTab (GrouP h, SequencesFormPtr sqfp)
{
  GrouP              q, g, x, y, k;
  ButtoN             prs = NULL, b = NULL;
  Char               str [32];
  Handle             h1, h2, h3, h4;
  Boolean            single, parseSeqId;
  GrouP              import_btn_grp;
  FastaPagePtr       fpp;
  
  q = HiddenGroup (h, -1, 0, NULL);
  SetGroupSpacing (q, 10, 10);
  g = HiddenGroup (q, -1, 0, NULL);
  SetGroupSpacing (g, 10, 10);
  x = HiddenGroup (g, -4, 0, NULL);
  
  
  sqfp->molecule_btn = PushButton (x, "Specify Molecule", SpecifyMolecule);
  SetObjectExtra (sqfp->molecule_btn, sqfp, NULL);
  sqfp->topology_btn = PushButton (x, "Specify Topology", SpecifyTopology);
  SetObjectExtra (sqfp->topology_btn, sqfp, NULL);
  y = HiddenGroup (g, -2, 0, NULL);
  SetGroupSpacing (y, 10, 2);
  sqfp->partial5 = CheckBox (y, "Incomplete at 5' end", NULL);
  sqfp->partial3 = CheckBox (y, "Incomplete at 3' end", NULL);

  if (sqfp->seqFormat == SEQ_FMT_FASTA) {
    prs = CheckBox (g, "Fasta definition line starts with sequence ID", ChangeDnaParse);
    sqfp->use_id_from_fasta_defline = prs;
    SetObjectExtra (prs, sqfp, NULL);
    parseSeqId = FALSE;
    if (GetAppParam ("SEQUIN", "PREFERENCES", "PARSENUCSEQID", NULL, str, sizeof (str))) {
      if (StringICmp (str, "TRUE") == 0) {
        parseSeqId = TRUE;
      }
    }
    else
    {
      parseSeqId = TRUE;
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
  if (PackageTypeIsSet (sqfp->seqPackage) &&
	    (sqfp->seqFormat == SEQ_FMT_FASTA)) 
	{
        sqfp->makeAlign = CheckBox (g, "Create Alignment", NULL);
      /*
      if (sqfp->seqPackage < SEQ_PKG_GENBANK) {
        SetStatus (sqfp->makeAlign, TRUE);
      }
      */
  }

  k = HiddenGroup (g, 0, 2, NULL);
  if (sqfp->seqFormat == SEQ_FMT_FASTA) {
    single = PackageTypeIsSingle (sqfp->seqPackage);
    if (sqfp->seqPackage == SEQ_PKG_GENOMICCDNA) {
      sqfp->dnaseq = CreateFastaDialog (k, "", TRUE, FALSE, fastaGenMsg, parseSeqId, single, NULL);
      fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
      import_btn_grp = HiddenGroup (g, 4, 0, NULL);
      fpp->import_btn = PushButton (import_btn_grp, "Import Genomic FASTA", ImportBtnProc);
    } else {
      sqfp->dnaseq = CreateFastaDialog (k, "", TRUE, FALSE, fastaNucMsg, parseSeqId, single, &(sqfp->seqPackage));
      fpp = (FastaPagePtr) GetObjectExtra (sqfp->dnaseq);
      import_btn_grp = HiddenGroup (g, 4, 0, NULL);
      fpp->import_btn = PushButton (import_btn_grp, "Import Additional Nucleotide FASTA", ImportBtnProc);
      SetTitle (fpp->import_btn, "Import Nucleotide FASTA");
      if (sqfp->seqPackage == SEQ_PKG_GAPPED) 
      {
        fpp->is_delta = TRUE;
      }
      else
      {
        fpp->is_delta = FALSE;
      }
    }
    SetObjectExtra (fpp->import_btn, sqfp, NULL);
    
    if (sqfp->seqPackage != SEQ_PKG_GAPPED 
        && sqfp->seqPackage != SEQ_PKG_GENOMICCDNA
        && sqfp->seqPackage != SEQ_PKG_SEGMENTED)
    {
      b = PushButton (import_btn_grp, "Add/Modify Sequences", SequenceAssistant);
      SetObjectExtra (b, sqfp, NULL);
    }
    fpp->clear_btn = PushButton (import_btn_grp, "Clear sequences", ClearSequencesButton);
    SetObjectExtra (fpp->clear_btn, sqfp, NULL);
    Disable (fpp->clear_btn);
  } else if (sqfp->seqFormat == SEQ_FMT_ALIGNMENT) {
    sqfp->dnaseq = CreatePhylipDialog (k, "", phylipNucMsg, sqfp->seqFormat, "",
                                       sqfp->seqPackage);
    import_btn_grp = HiddenGroup (g, 4, 0, NULL);
    import_btn_grp = HiddenGroup (g, 4, 0, NULL);
    b = PushButton (import_btn_grp, "Import Nucleotide Alignment", ImportBtnProc);
    SetObjectExtra (b, sqfp, NULL);
  }
  if (sqfp->makeAlign != NULL) {
    h1 = (Handle) sqfp->makeAlign;
    h2 = (Handle) import_btn_grp;
    h3 = (Handle) prs;
    h4 = (Handle) sqfp->singleIdGrp;
  } else {
    h1 = import_btn_grp;
    h2 = (Handle) prs;
    h3 = (Handle) sqfp->singleIdGrp;
    h4 = NULL;
  }
  AlignObjects (ALIGN_CENTER, (HANDLE) x, (HANDLE) y, (HANDLE) k,
                  (HANDLE) h1, (HANDLE) h2, (HANDLE) h3, (HANDLE) h4, NULL);
  return q;
}

static GrouP CreateTranscriptsTab (GrouP h, SequencesFormPtr sqfp)
{
  GrouP   q, g, y, k;
  ButtoN  prs, b;
  Boolean parseSeqId = FALSE;
  Char    str [32];
  
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

  if (GetAppParam ("SEQUIN", "PREFERENCES", "PARSEMRNASEQID", NULL, str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      parseSeqId = TRUE;
    }
  }
  SetStatus (prs, parseSeqId);

  k = HiddenGroup (g, 0, 2, NULL);
  sqfp->mrnaseq = CreateFastaDialog (k, "", TRUE, TRUE, fastaMrnaMsg, parseSeqId, FALSE, NULL);
  b = PushButton (g, "Import Transcript FASTA", ImportBtnProc);
  SetObjectExtra (b, sqfp, NULL);

  AlignObjects (ALIGN_CENTER, (HANDLE) y, (HANDLE) k, (HANDLE) prs, (HANDLE) b, NULL);
  return q;
}


static GrouP CreateProteinTab (GrouP h, SequencesFormPtr sqfp)
{
  GrouP   q, g, x, y, k;
  ButtoN  prs, mrna = NULL, b;
  Boolean parseSeqId;
  Char    str [32];
  
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
  sqfp->protseq = CreateFastaDialog (k, "", FALSE, FALSE, fastaProtMsg, parseSeqId, FALSE, NULL);
  b = PushButton (g, "Import Protein FASTA", ImportBtnProc);
  SetObjectExtra (b, sqfp, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) x, (HANDLE) y, (HANDLE) k,
                (HANDLE) prs, (HANDLE) b, (HANDLE) mrna, NULL);
  return q;
}


static GrouP CreateAnnotTab (GrouP h, SequencesFormPtr sqfp)
{
  GrouP  q, x, y, z;
  PrompT ppt1, ppt2;
  
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
  sqfp->protDescPpt = StaticPrompt (y, "Protein Description", 0, dialogTextHeight, programFont, 'l');
  sqfp->protDesc = DialogText (y, "", 20, NULL);
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
  Hide (sqfp->protDescPpt);
  Hide (sqfp->protDesc);
  /* Hide (sqfp->annotGrp); */
  return q;
}

extern ForM CreateInitOrgNucProtForm (Int2 left, Int2 top, CharPtr title,
                                      FormatBlockPtr format,
                                      BtnActnProc goToNext,
                                      BtnActnProc goBack,
                                      WndActnProc activateForm)

{
  GrouP              c;
  GrouP              h;
  GrouP              j;
  Int2               page;
  StdEditorProcsPtr  sepp;
  SequencesFormPtr   sqfp;
  WindoW             w;

  w = NULL;
  sqfp = MemNew (sizeof (SequencesForm));
  if (sqfp != NULL) {

    if (format != NULL) {
      sqfp->seqPackage = format->seqPackage;
      sqfp->seqFormat = format->seqFormat;
      sqfp->numSeqs = format->numSeqs;
      sqfp->submType = format->submType;
    } else {
      sqfp->seqPackage = SEQ_PKG_SINGLE;
      sqfp->seqFormat = SEQ_FMT_FASTA;
      sqfp->numSeqs = 0;
      sqfp->submType = SEQ_ORIG_SUBMISSION;
    }

    w = FixedWindow (left, top, -10, -10, title, NULL);
    SetObjectExtra (w, sqfp, StdCleanupFormProc);
    sqfp->form = (ForM) w;
    sqfp->toform = NULL;
    if (sqfp->seqFormat == SEQ_FMT_FASTA) {
      sqfp->fromform = FastaSequencesFormToSeqEntryPtr;
      sqfp->testform = FastaTestSequencesForm;
    } else if (sqfp->seqFormat == SEQ_FMT_ALIGNMENT) {
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

    if (sqfp->seqPackage == SEQ_PKG_GENOMICCDNA) {
      sqfp->tbs = CreateFolderTabs (j, cdnaGenFormTabs, NUCLEOTIDE_PAGE,
                                    0, 0, SYSTEM_FOLDER_TAB,
                                    ChangeSequencesPage, (Pointer) sqfp);
    } else if (sqfp->seqPackage == SEQ_PKG_SEGMENTED) {
      sqfp->tbs = CreateFolderTabs (j, seqSegFormTabs, NUCLEOTIDE_PAGE,
                                    0, 0, SYSTEM_FOLDER_TAB,
                                    ChangeSequencesPage, (Pointer) sqfp);
    } else {
      sqfp->tbs = CreateFolderTabs (j, popPhyMutFormTabs, NUCLEOTIDE_PAGE,
                                    0, 0, SYSTEM_FOLDER_TAB,
                                    ChangeSequencesPage, (Pointer) sqfp);
    }
    sqfp->currentPage = 0;
    page = 0;

    h = HiddenGroup (w, 0, 0, NULL);

    sqfp->pages [page] = CreateNucleotideTab (h, sqfp);
    Hide (sqfp->pages [page]);
    sqfp->tagFromPage [page] = NUCLEOTIDE_PAGE;
    page++;

    sqfp->pages [page] = CreateSourceTab (h, sqfp);
    Hide (sqfp->pages [page]);
    sqfp->tagFromPage [page] = ORGANISM_PAGE;
    page++;
    

    if (sqfp->seqPackage == SEQ_PKG_GENOMICCDNA) {
      sqfp->pages [page] = CreateTranscriptsTab (h, sqfp);
      Hide (sqfp->pages [page]);
      sqfp->tagFromPage [page] = MRNA_PAGE;
      page++;
    }
    
    if (sqfp->seqPackage != SEQ_PKG_GENOMICCDNA 
        && sqfp->seqPackage != SEQ_PKG_SEGMENTED)
    {
      sqfp->pages [page] = CreateAnnotTab (h, sqfp);
      Hide (sqfp->pages [page]);
      sqfp->tagFromPage [page] = ANNOTATE_PAGE;
      page++;
    }

    sqfp->pages [page] = CreateProteinTab (h, sqfp);
    Hide (sqfp->pages [page]);
    sqfp->tagFromPage [page] = PROTEIN_PAGE;
    page++;
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
    SendMessageToDialog (sqfp->dnaseq, VIB_MSG_INIT);
    SendMessageToDialog (sqfp->protseq, VIB_MSG_INIT);

    Show (sqfp->pages [sqfp->currentPage]);
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

extern SubmitBlockPtr ConvertSequinBlockToSubmitBlock (SequinBlockPtr sqp);

extern SubmitBlockPtr ConvertSequinBlockToSubmitBlock (SequinBlockPtr sqp)

{
  AffilPtr        affil;
  AuthorPtr       ap;
  AuthListPtr     authors;
  ContactInfoPtr  cip;
  CitSubPtr       csp;
  DatePtr         dp;
  CharPtr         os;
  SubmitBlockPtr  sbp;
  Char            str [64];

  sbp = NULL;
  if (sqp != NULL) {
    sbp = SubmitBlockNew ();
    if (sbp != NULL) {
      sbp->subtype = 1;
      os = GetOpSysString ();
      if (os != NULL) {
        sprintf (str, "Sequin %s - %s", SEQUIN_APPLICATION, os);
      } else {
        sprintf (str, "Sequin %s", SEQUIN_APPLICATION);
      }
      sbp->tool = StringSave (str);
      MemFree (os);
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
  BioseqSetPtr  bssp;
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
      if (sip != NULL && BioseqFind (sip) != bsp)
      {
        isLocalUnknownID = TRUE;
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
    if (IS_Bioseq_set (addhere)) {
      bssp = (BioseqSetPtr) addhere->data.ptrvalue;
      if (bssp != NULL && bssp->_class == BioseqseqSet_class_genbank) {
        addhere = bssp->seq_set;
      }
    }
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
extern Pointer ReadFromTPASmart (CharPtr accn, Uint2Ptr datatype, Uint2Ptr entityID);
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
  TrimSpacesAroundString (str);
  if (StringHasNoText (str)) {
    Message (MSG_OK, "Please enter an accession number or gi");
    Show (w);
    Select (w);
    Select (ffp->accession);
    return;
  }
  sep = NULL;
  uid = 0;
  /*
  if (! EntrezIsInited ()) {
    if (! SequinEntrezInit ("Sequin", FALSE, NULL)) {
      Remove (w);
      Show (startupForm);
      Select (startupForm);
      ArrowCursor ();
      return;
    }
  }
  */
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
    sep = PubSeqSynchronousQuery (uid, 0, -1);
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
      dataptr = ReadFromTPASmart (accn, &datatype, NULL);
      if (dataptr == NULL) {
        dataptr = ReadFromSmart (accn, &datatype, NULL);
        if (dataptr == NULL) {
          dataptr = ReadFromDirSub (accn, &datatype, NULL);
        }
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
        }
        if (datatype != OBJ_SEQSUB && uid > 0) {
          ArrowCursor ();
          Update ();
          if (!indexerVersion && Message (MSG_YN, repackageMsg) == ANS_YES) {
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
  /*
  if (! EntrezIsInited ()) {
    if (! SequinEntrezInit ("Sequin", FALSE, NULL)) {
      Remove (ParentWindow (b));
      ArrowCursor ();
      return;
    }
  }
  */
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
    sep = PubSeqSynchronousQuery (uid, 0, -1);
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

  SqnReadAlignView ((BaseFormPtr) ffp, updateTargetBspKludge, sep, TRUE);
}

extern void DownloadAndExtendProc (ButtoN b)

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
  /*
  if (! EntrezIsInited ()) {
    if (! SequinEntrezInit ("Sequin", FALSE, NULL)) {
      Remove (ParentWindow (b));
      ArrowCursor ();
      return;
    }
  }
  */
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
    sep = PubSeqSynchronousQuery (uid, 0, -1);
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

  SqnReadAlignView ((BaseFormPtr) ffp, updateTargetBspKludge, sep, FALSE);
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

static Boolean ReplaceTPAAccessionNumbers (
  CharPtr    seqid,
  ValNodePtr acc_list,
  SeqEntryPtr sep
)
{
  BioseqSetPtr      bssp;
  BioseqPtr         bsp;
  SeqDescrPtr       sdp;
  Char              str [128];
  SeqMgrDescContext context;
  UserObjectPtr     uop;
  ValNodePtr        vnp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return FALSE;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    /* this also delves into nuc-prot sets */
    if (bssp != NULL && (bssp->_class == 7 ||
                         (IsPopPhyEtcSet (bssp->_class)) ||
                         bssp->_class == 1)) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next)
      {
        if (ReplaceTPAAccessionNumbers (seqid, acc_list, sep))
        {
          return TRUE;
        }
      }
      return FALSE;
    }
  }
  if (!IS_Bioseq (sep)) return FALSE;

  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return FALSE;
  SeqIdWrite (bsp->id, str, PRINTID_REPORT, sizeof (str));
  if (StringCmp (str, seqid) != 0) return FALSE;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_user, &context);
  while (sdp != NULL
    && ((uop = (UserObjectPtr)sdp->data.ptrvalue) == NULL
      || StringICmp (uop->type->str, "TpaAssembly") != 0))
  {
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_user, &context);
  }
  if (sdp == NULL)
  {
    sdp = CreateNewDescriptor (sep, Seq_descr_user);
    if (sdp == NULL) return FALSE;
    uop = CreateTpaAssemblyUserObject ();
    if (uop == NULL) return FALSE;
    sdp->data.ptrvalue = uop;
  }

  for (vnp = acc_list; vnp != NULL; vnp = vnp->next)
  {
    AddAccessionToTpaAssemblyUserObject (uop, vnp->data.ptrvalue, 0, 0);
  }
  ValNodeFreeData (acc_list);
  
  return TRUE;
}

extern void LoadTPAAccessionNumbersFromFile (
  IteM i
)
{
  BaseFormPtr   bfp;
  SeqEntryPtr   sep;
  Char          path [PATH_MAX];
  FILE          *fp;
  Char          str [8192];
  size_t        len = 8192;
  Boolean       need_seqid;
  Char          seqid[100];
  Int4          seqid_len;
  CharPtr       cp;
  CharPtr       acc_end;
  Boolean       found_end;
  ValNodePtr    acc_list;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  path [0] = '\0';
  if (! GetInputFileName (path, sizeof (path), NULL, "TEXT")) return;
  
  fp = FileOpen (path, "r");
  if (fp == NULL) return;

  need_seqid = TRUE;
  acc_list = NULL;
  ReadLine (fp, str, len);
  while (Nlm_fileDone) 
  {
    cp = str;
    if (strlen (str) == 0)
    {
      ReadLine (fp, str, len);
      continue;
    }
    if (need_seqid)
    {
      seqid_len = StringCSpn (str, " \t");
      if (seqid_len > 0)
      {
        StringNCpy (seqid, str, seqid_len);
        seqid [seqid_len] = 0;
        need_seqid = FALSE;
      }
      cp = str + seqid_len + 1;
    }
    if (need_seqid)
    {
      ReadLine (fp, str, len);
      continue;
    }
    if (str [strlen (str) - 1] != ',')
    {
      need_seqid = TRUE;
    }
    
    found_end = FALSE;
    while (*cp != 0)
    {
      if (*cp == ' ' || *cp == ',' || *cp == '\t')
      {
        cp++;
      }
      else
      {
        acc_end = cp + 1;
        while (*acc_end != 0 && *acc_end != ',')
        {
          acc_end++;
        }
        if (*acc_end == 0)
        {
          found_end = TRUE;
        }
        else
        {
          *acc_end = 0;
        }
        ValNodeAddStr (&acc_list, 0, StringSave (cp));
        if (found_end)
        {
          cp = acc_end;
        }
        else
        {
          cp = acc_end + 1;
        }
      }
    }

    if (need_seqid == TRUE)
    {
      /* do something with accession list */
      if ( ! ReplaceTPAAccessionNumbers (seqid, acc_list, sep))
      {
        Message (MSG_ERROR,
                 "Unable to update accession numbers for %s (not found)",
                 seqid);
      }
      acc_list = NULL;
    }
      
    ReadLine (fp, str, len);
  }
  if (acc_list != NULL
    && ! ReplaceTPAAccessionNumbers (seqid, acc_list, sep))
  {
    Message (MSG_ERROR,
             "Unable to update accession numbers for %s (not found)",
             seqid);
  }

  FileClose (fp);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();
  return;
}

static void AddHistory (
  BioseqPtr  bsp,
  ValNodePtr acc_list
)
{
  SeqHistPtr      hist;
  ValNodePtr      vnp;
  SeqIdPtr        sip;
  TextSeqIdPtr    tsip;
  Uint4           whichdb;
  Char            prefix [20];

  if (bsp == NULL || acc_list == NULL) return;
  hist = bsp->hist;
  if (hist == NULL)
  {
    hist = SeqHistNew ();
    if (hist == NULL) return;
    bsp->hist = hist;
  }
  for (vnp = acc_list; vnp != NULL; vnp = vnp->next) {
    tsip = TextSeqIdNew ();
    if (tsip == NULL) return;
    tsip->accession = StringSave (vnp->data.ptrvalue);

    sip = ValNodeNew (hist->replace_ids);
    if (hist->replace_ids == NULL) {
      hist->replace_ids = sip;
    }
    if (sip == NULL) return;

    sip->data.ptrvalue = (Pointer) tsip;

    StringNCpy_0 (prefix, (CharPtr) vnp->data.ptrvalue, sizeof (prefix));
    whichdb = WHICH_db_accession (prefix);
    if (ACCN_IS_EMBL (whichdb)) {
      sip->choice = SEQID_EMBL;
    } else if (ACCN_IS_DDBJ (whichdb)) {
      sip->choice = SEQID_DDBJ;
    } else {
      sip->choice = SEQID_GENBANK;
    }
  }
  if (hist != NULL
    && hist->assembly == NULL
    && hist->replace_date == NULL
    && hist->replace_ids == NULL
    && hist->replaced_by_date == NULL
    && hist->replaced_by_ids == NULL
    && hist->deleted_date == NULL
    && ! hist->deleted)
  {
      bsp->hist = SeqHistFree (bsp->hist);
  }
}

static Boolean DoIDsMatch (CharPtr seqid, BioseqPtr bsp, Boolean AllowLocal)
{
  Char         str [128];
  Int4         seqid_len;
  SeqIdPtr     sip;

  if (bsp == NULL) return FALSE;

  SeqIdWrite (bsp->id, str, PRINTID_REPORT, sizeof (str));
  seqid_len = StringCSpn (str, ".");
  if (seqid_len > 0)
  {
    str [ seqid_len ] = 0;
  }
  if (StringCmp (str, seqid) == 0) return TRUE;

  for (sip = bsp->id; sip != NULL; sip = sip->next)
  {
    if (sip->choice == SEQID_LOCAL && AllowLocal)
    {
      SeqIdWrite (bsp->id, str, PRINTID_REPORT, sizeof (str));
      seqid_len = StringCSpn (str, ".");
      if (seqid_len > 0)
      {
        str [ seqid_len ] = 0;
      }
      if (StringCmp (str, seqid) == 0) return TRUE;
    }
  }
  return FALSE;
}

static Boolean AddAccessionToGenbankBlock (
  CharPtr     seqid,
  ValNodePtr  acc_list,
  SeqEntryPtr sep,
  Boolean     add_hist
)
{
  BioseqPtr    bsp;
  BioseqSetPtr bssp;
  GBBlockPtr   gbp;
  ValNodePtr   last_one;
  SeqDescrPtr       sdp;

  if (seqid == NULL || acc_list == NULL
    || sep == NULL || sep->data.ptrvalue == NULL) return FALSE;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    /* this also delves into nuc-prot sets */
    if (bssp != NULL && (bssp->_class == 7 ||
                         (IsPopPhyEtcSet (bssp->_class)) ||
                         bssp->_class == 1)) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next)
      {
        if (AddAccessionToGenbankBlock (seqid, acc_list, sep, add_hist))
        {
          return TRUE;
        }
      }
      return FALSE;
    }
  }
  if (!IS_Bioseq (sep)) return FALSE;

  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return FALSE;
  if (! DoIDsMatch (seqid, bsp, TRUE)) return FALSE;

  sdp = BioseqGetSeqDescr (bsp, Seq_descr_genbank, NULL);

  if (sdp == NULL)
  {
    sdp = CreateNewDescriptor (sep, Seq_descr_genbank);
    if (sdp == NULL) return FALSE;
  }
 
  if (sdp->data.ptrvalue == NULL)
  {
    sdp->data.ptrvalue = GBBlockNew ();
    if (sdp->data.ptrvalue == NULL) return FALSE;
  }
 
  gbp = (GBBlockPtr) sdp->data.ptrvalue;
  
  for (last_one = gbp->extra_accessions;
       last_one != NULL && last_one->next != NULL;
       last_one = last_one->next)
  {}
  if (last_one == NULL)
  {
    gbp->extra_accessions = acc_list;
  }
  else
  {
    last_one->next = acc_list;
  }
  if (add_hist)
  {
    AddHistory (bsp, acc_list);
  }
  return TRUE;
}
 
static void LoadSecondaryAccessionNumbersPlusHistFromFile (
  IteM    i,
  Boolean add_hist
)
{
  BaseFormPtr   bfp;
  SeqEntryPtr   sep;
  Char          path [PATH_MAX];
  FILE          *fp;
  Char          str [8192];
  size_t        len = 8192;
  Boolean       need_seqid;
  Char          seqid[100];
  Int4          seqid_len;
  CharPtr       cp;
  CharPtr       acc_end;
  Boolean       found_end;
  ValNodePtr    acc_list;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  path [0] = '\0';
  if (! GetInputFileName (path, sizeof (path), NULL, "TEXT")) return;
  
  fp = FileOpen (path, "r");
  if (fp == NULL) return;

  need_seqid = TRUE;
  acc_list = NULL;
  ReadLine (fp, str, len);
  while (Nlm_fileDone || str[0] != 0) 
  {
    cp = str;
    if (strlen (str) == 0)
    {
      ReadLine (fp, str, len);
      continue;
    }
    seqid_len = StringCSpn (str, " \t");
    if (seqid_len > 0)
    {
      StringNCpy (seqid, str, seqid_len);
      seqid [seqid_len] = 0;
      cp = str + seqid_len + 1;
    }
    else
    {
      ReadLine (fp, str, len);
      continue;
    }
    
    found_end = FALSE;
    while (*cp != 0)
    {
      if (*cp == ' ' || *cp == ' ')
      {
        cp++;
      }
      else
      {
        acc_end = cp + 1;
        while (*acc_end != 0 && *acc_end != ' ')
        {
          acc_end++;
        }
        if (*acc_end == 0)
        {
          found_end = TRUE;
        }
        else
        {
          *acc_end = 0;
        }
        ValNodeAddStr (&acc_list, 0, StringSave (cp));
        if (found_end)
        {
          cp = acc_end;
        }
        else
        {
          cp = acc_end + 1;
        }
      }
    }

    /* do something with accession list */
    if ( ! AddAccessionToGenbankBlock (seqid, acc_list, sep, add_hist))
    {
      Message (MSG_ERROR,
               "Unable to update accession numbers for %s (not found)",
               seqid);
    }
    acc_list = NULL;
      
    ReadLine (fp, str, len);
  }
  if (acc_list != NULL
    && ! AddAccessionToGenbankBlock (seqid, acc_list, sep, add_hist))
  {
    Message (MSG_ERROR,
             "Unable to update accession numbers for %s (not found)",
             seqid);
  }

  FileClose (fp);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();
  return;
}

extern void LoadSecondaryAccessionNumbersFromFile (
  IteM i
)
{
  LoadSecondaryAccessionNumbersPlusHistFromFile (i, FALSE);
}

extern void LoadHistoryAccessionNumbersFromFile (
  IteM i
)
{
  LoadSecondaryAccessionNumbersPlusHistFromFile (i, TRUE);
}

/* This section of code is used for managing lists of features.
 * Sometimes the features will be displayed alphabetically, sometimes
 * they will be displayed alphabetically with a list of the most used features
 * also appearing at the top of the list.
 */

/* This is used to compare feature names with the special alphabetical order */
static int CompareFeatureNames (CharPtr cp1, CharPtr cp2)
{
  /* NULL name goes at the end */
  if (cp1 == NULL && cp2 == NULL) return 0;
  if (cp1 == NULL) return 1;
  if (cp2 == NULL) return -1;

  /* starts with a space goes at the beginning */
  if (cp1 [0] == ' ' && cp2 [0] == ' ') return 0;
  if (cp1 [0] == ' ') return -1;
  if (cp2 [0] == ' ') return 1;

  /* Is "All" or [ALL FEATURES] goes at the beginning */
  if ((StringCmp (cp1, "All") == 0
    || StringCmp (cp1, "[ALL FEATURES]") == 0)
    && (StringCmp (cp2, "All") == 0
    || StringCmp (cp2, "[ALL FEATURES]") == 0))
  {
    return 0;
  }
  if (StringCmp (cp1, "All") == 0
    || StringCmp (cp1, "[ALL FEATURES]") == 0)
  {
    return -1;
  }
  if (StringCmp (cp2, "All") == 0
    || StringCmp (cp2, "[ALL FEATURES]") == 0)
  {
    return 1;
  }

  /* starts with a number -> goes at the end */
  if (cp1 [0] >= '0' && cp1 [0] <= '9'
   && cp2 [0] >= '0' && cp2 [0] <= '9')
  {
    return StringICmp (cp1, cp2);
  }
  if (cp1 [0] >= '0' && cp1 [0] <= '9')
  {
    return 1;
  }
  if (cp2 [0] >= '0' && cp2 [0] <= '9')
  {
    return -1;
  }

  /* starts with a tilde or dash - sort with other tildes, put before numbers after alphas */
  if (cp1 [0] == '~' && cp2 [0] == '~') 
  {
    return StringICmp (cp1 + 1, cp2 + 1);
  }
  if (cp1 [0] == '~') return 1;
  if (cp2 [0] == '~') return -1;

  if (cp1 [0] == '-' && cp2 [0] == '-') 
  {
    return StringICmp (cp1 + 1, cp2 + 1);
  }
  if (cp1 [0] == '-') return 1;
  if (cp2 [0] == '-') return -1;

  return StringICmp (cp1, cp2);
}

extern int LIBCALLBACK CompareFeatureValNodeStrings (VoidPtr ptr1, VoidPtr ptr2)
{
  ValNodePtr vnp1, vnp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;

  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);

  if (vnp1 == NULL || vnp2 == NULL) return 0;

  return CompareFeatureNames (vnp1->data.ptrvalue, vnp2->data.ptrvalue);
}

extern int LIBCALLBACK CompareImpFeatEnumFieldAssoc (VoidPtr ptr1, VoidPtr ptr2)
{
  ValNodePtr        vnp1, vnp2;
  EnumFieldAssocPtr ap1, ap2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;

  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;

  ap1 = (EnumFieldAssocPtr) vnp1->data.ptrvalue;
  ap2 = (EnumFieldAssocPtr) vnp2->data.ptrvalue;
  if (ap1 == NULL || ap2 == NULL) return 0;

  return CompareFeatureNames (ap1->name, ap2->name);
}

CharPtr MostUsedFeatureList[] = { 
  "CDS",
  "exon",
  "Gene",
  "intron",
  "mRNA",
  "rRNA",
  "RNA"
};

extern ValNodePtr InsertMostUsedFeatureValNodes (ValNodePtr old_list)
{
  ValNodePtr new_list, new_item, old_item;
  Int4       index;

  new_list = NULL;
  for (index = 0;
       index < sizeof (MostUsedFeatureList) / sizeof (CharPtr);
       index ++)
  {
    old_item = FindExactStringInStrings ( old_list, MostUsedFeatureList [index])
;
    if (old_item == NULL) continue;
    new_item = ValNodeNew ( new_list);
    if (new_item == NULL) return old_list;
    new_item->choice = old_item->choice;
    new_item->data.ptrvalue = StringSave (MostUsedFeatureList [index]);
    if (new_list == NULL) new_list = new_item;
  }
  if (new_item != NULL)
  {
    if (old_list != NULL &&
      ( StringCmp (old_list->data.ptrvalue, "All") == 0
       || StringCmp (old_list->data.ptrvalue, "[ALL FEATURES]") == 0))
    {
      new_item->next = old_list->next;
      old_list->next = new_list;
      new_list = old_list;
    }
    else
    {
      new_item->next = old_list;
    }
  }
  else
  {
    new_list = old_list;
  }
  return new_list;
}

static EnumFieldAssocPtr FindEnumFieldAssoc (
  EnumFieldAssocPtr alist,
  CharPtr findStr
)
{
  EnumFieldAssocPtr ap;
  
  for (ap = alist; ap != NULL && ap->name != NULL; ap++)
  {
    if (StringCmp (ap->name, findStr) == 0) return ap;
  }
  return NULL;
}

static void CopyEnumFieldAssoc (EnumFieldAssocPtr ap1, EnumFieldAssocPtr ap2)
{
  if (ap1 == NULL || ap2 == NULL) return;

  ap1->name = StringSave (ap2->name);
  ap1->value = ap2->value;
}

extern EnumFieldAssocPtr InsertMostUsedFeatureEnumFieldAssoc (
  EnumFieldAssocPtr alist
)
{
  Int4              num_total_fields, index, new_index;
  EnumFieldAssocPtr ap, new_alist, old_ap;

  num_total_fields = sizeof (MostUsedFeatureList) / sizeof (CharPtr);

  for (ap = alist; ap != NULL && ap->name != NULL; ap++)
  {
    num_total_fields ++;
  }
  /* need the last null field */
  num_total_fields ++;

  new_alist = MemNew (num_total_fields * sizeof (EnumFieldAssoc));
  if (new_alist == NULL) return alist;

  /* copy the first item if wildcard */
  if (StringCmp (alist->name, "[ALL FEATURES]") == 0)
  {
    CopyEnumFieldAssoc (new_alist, alist);
    new_index = 1;
  }
  else
  {
    new_index = 0;
  }

  for (index = 0;
       index < sizeof (MostUsedFeatureList) / sizeof (CharPtr);
       index ++)
  {
    old_ap = FindEnumFieldAssoc (alist, MostUsedFeatureList [index]);
    if (old_ap == NULL) continue;
    CopyEnumFieldAssoc (new_alist + new_index++, old_ap);
  }

  for (ap = alist; ap != NULL && ap->name != NULL; ap++)
  {
    CopyEnumFieldAssoc (new_alist + new_index ++, ap);
  }
  /* copy over the last null field */
  if (ap != NULL)
  {
    CopyEnumFieldAssoc (new_alist + new_index ++, ap);
  }
  return new_alist;
  
}

extern void SortEnumFieldAssocPtrArray (EnumFieldAssocPtr alist, CompareFunc compar)
{
  ValNodePtr        head, vnp;
  EnumFieldAssocPtr ap;
  Int4              index;

  /* first, create ValNode list so we can sort the data */
  head = NULL;
  for (ap = alist; ap != NULL && ap->name != NULL; ap++)
  {
    vnp = ValNodeNew (head);
    if (vnp == NULL) return;
    vnp->data.ptrvalue = MemNew (sizeof (EnumFieldAssoc));
    if (vnp->data.ptrvalue == NULL) return;
    MemCpy (vnp->data.ptrvalue, ap, sizeof (EnumFieldAssoc));
    if (head == NULL) head = vnp;
  }

  /* Now sort the ValNode list */
  head = SortValNode (head, compar);

  /* Now repopulate the EnumFieldAssoc list */
  index = 0;
  for (vnp = head; vnp != NULL; vnp = vnp->next)
  {
    MemCpy (alist + index++, vnp->data.ptrvalue, sizeof (EnumFieldAssoc));
  }

  /* And free the ValNode list */
  ValNodeFreeData (head);
}

static Uint2 UnusualFeatureTypes [] = {
  FEATDEF_ORG,
  FEATDEF_mutation,
  FEATDEF_site_ref,
  FEATDEF_gap,
  FEATDEF_NON_STD_RESIDUE,
  FEATDEF_NUM
};
 
extern ValNodePtr BuildFeatureValNodeList (
  Boolean prefer_most_used,
  CharPtr wild_card_name,
  Int4    wild_card_value,
  Boolean skip_unusual,
  Boolean skip_import
)
{
  FeatDefPtr  curr;
  ValNodePtr  head, vnp;
  Uint1       key;
  CharPtr     label = NULL;
  Uint2       subtype;
  Int4        index;
  Boolean     skip;
  Char        str [256];

  head = NULL;
  curr = FeatDefFindNext (NULL, &key, &label, FEATDEF_ANY, TRUE);
  while (curr != NULL) {
    skip = FALSE;
    if (skip_unusual)
    {
      for (index = 0;
           ! skip && index < sizeof ( UnusualFeatureTypes ) / sizeof (Uint2);
           index ++)
      {
        if (curr->featdef_key == UnusualFeatureTypes [ index ]) skip = TRUE;
      }
    }
    if (key != FEATDEF_BAD && ! skip) {
      
      subtype = curr->featdef_key;
	  if (subtype == FEATDEF_PUB)
	  {
        StringNCpy_0 (str, curr->typelabel, sizeof (str) - 15);
        StringCat (str, " (Publication)");
	  }
	  else if (subtype != FEATDEF_misc_RNA &&
          subtype != FEATDEF_precursor_RNA &&
          subtype != FEATDEF_mat_peptide &&
          subtype != FEATDEF_sig_peptide &&
          subtype != FEATDEF_transit_peptide &&
          subtype != FEATDEF_Imp_CDS)
      {
        StringNCpy_0 (str, curr->typelabel, sizeof (str) - 1);
      }
      else if (! skip_import)
      {
        StringNCpy_0 (str, curr->typelabel, sizeof (str) - 10);
        StringCat (str, "_imp");
      }
      else
      {
        skip = TRUE;
      }
      if (! skip)
      {
        vnp = ValNodeNew (head);
        if (head == NULL) {
          head = vnp;
        }
        if (vnp != NULL) {
          vnp->choice = subtype;
          vnp->data.ptrvalue = StringSave (str);
        }
      }
    }
    curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
  }
  if (head != NULL) {
    head = SortValNode (head, CompareFeatureValNodeStrings);
    head = InsertMostUsedFeatureValNodes (head);
    if (wild_card_name != NULL)
    {
      vnp = ValNodeNew (NULL);
      if (vnp != NULL) {
        vnp->choice = wild_card_value;
        vnp->data.ptrvalue = StringSave (wild_card_name);
        vnp->next = head;
        head = vnp;
      }
    }
  }
  return head;
}

extern void SetTaxNameAndRemoveTaxRef (OrgRefPtr orp, CharPtr taxname)
{
  ValNodePtr      vnp, next;
  ValNodePtr PNTR prev;
  DbtagPtr        dbt;
  Boolean         remove_taxrefs = FALSE;

  if (orp == NULL) return;

  if ( taxname == NULL || orp->taxname == NULL
    || StringCmp (taxname, orp->taxname) != 0)
  {
    remove_taxrefs = TRUE;
  }
  MemFree (orp->taxname);
  orp->taxname = taxname;

  if (! remove_taxrefs) return;

  orp->common = MemFree (orp->common);

  vnp = orp->db;
  if (vnp == NULL) return;
  prev = (ValNodePtr PNTR) &(orp->db);
  while (vnp != NULL) {
    next = vnp->next;
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    if (dbt != NULL && StringICmp ((CharPtr) dbt->db, "taxon") == 0) {
      *prev = vnp->next;
      vnp->next = NULL;
      DbtagFree (dbt);
      ValNodeFree (vnp);
    } else {
      prev = (ValNodePtr PNTR) &(vnp->next);
    }
    vnp = next;
  }
}

static Boolean
FindMatchingProprotein 
(SeqFeatPtr sfp,
 SeqMgrFeatContextPtr fcontext,
 BioseqPtr prot_bsp)
{
  SeqFeatPtr        prot_sfp;
  SeqMgrFeatContext pcontext;
  CharPtr           start;

  if (prot_bsp == NULL || fcontext == NULL) return FALSE;
  if (StringNICmp (fcontext->label, "encodes ", 8) == 0) {
    start = fcontext->label + 8;
  } else {
    start = fcontext->label;
  }
  prot_sfp = NULL;
  while ((prot_sfp = SeqMgrGetNextFeature (prot_bsp, prot_sfp, 
                                           0, 0, &pcontext)) != NULL) {
    if (StringCmp (pcontext.label, start) == 0) {
      return TRUE;
    } 
  }
  return FALSE;
}


static void 
RemoveRedundantProproteinMiscFeatsOnBioseq
(BioseqPtr bsp,
 Pointer userdata)
{
  SeqFeatPtr        sfp, cds;
  SeqMgrFeatContext fcontext, cds_context;
  BioseqPtr         bsp_prot;

  sfp = NULL;

  /* list misc feats */
  while ((sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext)) != NULL) {
    if (fcontext.featdeftype == FEATDEF_misc_feature
        &&  StringStr(fcontext.label, "proprotein") != NULL) {
      cds = NULL;
      while ((cds = SeqMgrGetNextFeature (bsp, cds, SEQFEAT_CDREGION, 0, &cds_context)) != NULL) {
        if (cds_context.left <= fcontext.left
            &&  cds_context.right >= fcontext.right) {
          /* Get Protein sequence, look for matching proprotein feat */
          bsp_prot = BioseqFind (SeqLocId(cds->product));
          if (FindMatchingProprotein (sfp, &fcontext, bsp_prot)) {
            sfp->idx.deleteme = TRUE;
          }
        }
      }
    }
  }

}


extern void RemoveRedundantProproteinMiscFeats (IteM i)
{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  /* Visit each bioseq to remove redundant proprotein misc feats */
  VisitBioseqsInSep (sep, NULL, RemoveRedundantProproteinMiscFeatsOnBioseq);

  DeleteMarkedObjects (bfp->input_entityID, 0, NULL);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();
}

typedef struct typestraindata
{
  FORM_MESSAGE_BLOCK
  
  TexT   find_this_txt;
  ButtoN when_string_not_found_btn;
  ButtoN case_insensitive_btn;
  GrouP  string_loc_grp;
  PopuP  field_choice_popup;
  ButtoN remove_found_text_btn;
  
  Boolean when_string_not_found;
  Boolean case_insensitive;
  Int4    string_loc;
  Int4    field_choice;
  CharPtr find_this;
  Boolean remove_found_text;
} TypeStrainData, PNTR TypeStrainPtr;

static Boolean MeetsTypeStrainConstraint (BioSourcePtr biop, TypeStrainPtr tsp)
{
  CharPtr      string_found = NULL;
  CharPtr      search_text = NULL;
  OrgModPtr    mod = NULL, prev_mod = NULL;
  SubSourcePtr ssp = NULL, prev_ssp = NULL;
  Boolean      rval;
  CharPtr      cp, destp;
  
  if (biop == NULL) return FALSE;
  if (tsp == NULL) return TRUE;
  if (StringHasNoText (tsp->find_this)) return TRUE;
  if (biop->org == NULL)
  {
  	if (tsp->when_string_not_found)
  	{
  	  return TRUE;
  	}
  	else
  	{
  	  return FALSE;
  	}
  }
  
  if (tsp->field_choice == 1)
  {
  	/* look for strain field */
  	if (biop->org->orgname == NULL)
  	{
  	  if (tsp->when_string_not_found)
  	  {
  	  	return TRUE;
  	  }
  	  else
  	  {
  	  	return FALSE;
  	  }
  	}
  	for (mod = biop->org->orgname->mod;
  	     mod != NULL && mod->subtype != ORGMOD_strain;
  	     mod = mod->next)
  	{
  	  prev_mod = mod;
  	}
  	if (mod != NULL)
  	{
  	  search_text = mod->subname;
  	}
  }
  else if (tsp->field_choice == 2)
  {
    /* look for biosource comment */
  	for (mod = biop->org->orgname->mod;
  	     mod != NULL && mod->subtype != 255;
  	     mod = mod->next)
  	{
  	  prev_mod = mod;
  	}
  	if (mod != NULL)
  	{
  	  search_text = mod->subname;
  	}
  	else
  	{
  	  for (ssp = biop->subtype; ssp != NULL && ssp->subtype != 255; ssp = ssp->next)
  	  {
  	  	prev_ssp = ssp;
  	  }
  	  if (ssp != NULL)
  	  {
  	  	search_text = ssp->name;
  	  }
  	}
  }
  else
  {
  	return FALSE;
  }
  if (search_text != NULL)
  {
  	if (tsp->case_insensitive)
  	{
  	  string_found = StringISearch (search_text, tsp->find_this);
  	}
  	else
  	{
  	  string_found = StringSearch (search_text, tsp->find_this);
  	}
  	if (string_found != NULL)
  	{
  	  if (tsp->string_loc == 2 && string_found != search_text)
  	  {
  	  	string_found = NULL;
  	  }
  	  else if (tsp->string_loc == 3)
  	  {
  	  	while (string_found != NULL && string_found[StringLen (tsp->find_this)] != 0)
  	  	{
  	      if (tsp->case_insensitive)
  	      {
  	        string_found = StringISearch (string_found + 1, tsp->find_this);
          }
          else
          {
            string_found = StringSearch (string_found + 1, tsp->find_this);
          }
  	  	}
  	  }
  	}
  }
  
  if (string_found == NULL) 
  {
    if (tsp->when_string_not_found)
  	{
  	  rval = TRUE;
  	}
  	else
  	{
  	  rval = FALSE;
  	}
  }
  else
  {
    if (tsp->when_string_not_found)
  	{
  	  rval = FALSE;
  	}
  	else
  	{
  	  rval = TRUE;
  	  if (tsp->remove_found_text)
  	  {
  	  	if (string_found == search_text)
  	  	{
  	  	  if (StringLen (string_found) == StringLen (tsp->find_this))
  	  	  {
  	  	  	/* remove entire mod or ssp */
  	  	  	if (mod != NULL)
  	  	  	{
  	  	  	  if (prev_mod == NULL)
  	  	  	  {
  	  	  	  	biop->org->orgname->mod = mod->next;
  	  	  	  }
  	  	  	  else
  	  	  	  {
  	  	  	  	prev_mod->next = mod->next;
  	  	  	  }
  	  	  	  mod->next = NULL;
  	  	  	  OrgModFree (mod);
  	  	  	}
  	  	  	else if (ssp != NULL)
  	  	  	{
  	  	  	  if (prev_ssp == NULL)
  	  	  	  {
  	  	  	  	biop->subtype = ssp->next;
  	  	  	  }
  	  	  	  else
  	  	  	  {
  	  	  	  	prev_ssp->next = ssp->next;
  	  	  	  	ssp->next = NULL;
  	  	  	  	SubSourceFree (ssp);
  	  	  	  }
  	  	  	  ssp->next = NULL;
  	  	  	  SubSourceFree (ssp);
  	  	  	}
  	  	  }
  	  	  else
  	  	  {
  	  	  	/* remove first part of string and shift remainder */
  	  	  	destp = search_text;
  	  	  	for (cp = search_text + StringLen (tsp->find_this); *cp != 0; cp++)
  	  	  	{
  	  	  	  *destp++ = *cp;
  	  	  	}
  	  	  	*destp = 0;
  	  	  }
  	  	}
  	  	else
  	  	{
  	  	  /* keep first part of string, skip match, keep remainder */
  	  	  destp = string_found;
  	  	  for (cp = string_found + StringLen (tsp->find_this); *cp != 0; cp++)
  	  	  {
  	  	  	*destp++ = *cp;
  	  	  }
  	  	  *destp = 0;
  	  	}
  	  }
  	}
  }
  return rval;
}

static void AddTypeStrainCommentsProc (BioSourcePtr biop, Pointer userdata)
{
  OrgModPtr          mod, last_mod;
  TypeStrainPtr      tsp;
  CharPtr            tmp;
  CharPtr            short_format = "type strain of %s";
  CharPtr            long_format = "%s; type strain of %s";

  if (biop == NULL || biop->org == NULL || biop->org->taxname == NULL) return;

  tsp = (TypeStrainPtr) userdata;
  
  if (! MeetsTypeStrainConstraint (biop, tsp)) return;
  
  if (biop->org->orgname == NULL)
  {
    biop->org->orgname = OrgNameNew ();
  }

  mod = biop->org->orgname->mod;
  last_mod = NULL;
  
  while (mod != NULL && mod->subtype != 255) {
    last_mod = mod;
    mod = mod->next;
  }
  if (mod != NULL) {
    if (StringStr (mod->subname, "type strain of") != NULL) return;
    tmp = (CharPtr) MemNew (StringLen (long_format) + StringLen (mod->subname) 
                            + StringLen (biop->org->taxname) + 1);
    if (tmp != NULL) {
      sprintf (tmp, long_format, mod->subname, biop->org->taxname);
      MemFree (mod->subname);
      mod->subname = tmp;
    }
  } else {
    mod = OrgModNew ();
    if (mod != NULL) {
      mod->subtype = 255;
      tmp = (CharPtr) MemNew (StringLen (short_format)
                            + StringLen (biop->org->taxname) + 1);
      if (tmp != NULL) {
        sprintf (tmp, short_format, biop->org->taxname);
        mod->subname = tmp;
      }
      if (last_mod == NULL) {
        biop->org->orgname->mod = mod;
      } else {
        last_mod->next = mod;
      }
    }
  }
}

static void CleanupTypeStrainForm (GraphiC g, VoidPtr data)

{
  TypeStrainPtr tsp;

  tsp = (TypeStrainPtr) data;
  if (tsp != NULL)
  {
  	tsp->find_this = MemFree (tsp->find_this);
  }
  MemFree (tsp);
  StdCleanupFormProc (g, data);
}

static void AddTypeStrainCommentsWithConstraintProc (ButtoN b)
{
  TypeStrainPtr tsp;
  SeqEntryPtr   sep;
  
  tsp = (TypeStrainPtr) GetObjectExtra (b);
  if (tsp == NULL) return;
  sep = GetTopSeqEntryForEntityID (tsp->input_entityID);
  if (sep == NULL) return;

  tsp->find_this = SaveStringFromText (tsp->find_this_txt);  
  tsp->when_string_not_found = GetStatus (tsp->when_string_not_found_btn);
  tsp->case_insensitive = GetStatus (tsp->case_insensitive_btn);
  tsp->string_loc = GetValue (tsp->string_loc_grp);
  tsp->field_choice = GetValue (tsp->field_choice_popup);
  tsp->remove_found_text = GetStatus (tsp->remove_found_text_btn);
  
  /* Visit each bioseq to remove redundant proprotein misc feats */
  VisitBioSourcesInSep (sep, tsp, AddTypeStrainCommentsProc);

  ObjMgrSetDirtyFlag (tsp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, tsp->input_entityID, 0, 0);
  Remove (tsp->form);
  ArrowCursor ();
  Update ();
}

extern void AddTypeStrainCommentsWithConstraint (IteM i)
{
  BaseFormPtr    bfp;
  TypeStrainPtr  tsp;
  WindoW         w;
  GrouP          h, k, l, m, c;
  ButtoN         b;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
	
  tsp = (TypeStrainPtr) MemNew (sizeof (TypeStrainData));
  if (tsp == NULL) return;
  tsp->input_entityID = bfp->input_entityID;

  w = FixedWindow (-50, -33, -10, -10, "Add Type Strain Comments", StdCloseWindowProc);
  if (w == NULL) {
	MemFree (tsp);
	return;
  }
  tsp->form = (ForM) w;
  SetObjectExtra (w, tsp, CleanupTypeStrainForm);
  
  h = HiddenGroup (w, 1, 0, NULL);
  k = HiddenGroup (h, 2, 0, NULL);

  StaticPrompt (k, "When this text is present", 0, dialogTextHeight, systemFont, 'c');
  tsp->find_this_txt = DialogText (k, "", 15, NULL);
  l = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (l, "In ", 0, dialogTextHeight, systemFont, 'c');
  tsp->field_choice_popup = PopupList (l, TRUE, NULL);
  PopupItem (tsp->field_choice_popup, "Strain");
  PopupItem (tsp->field_choice_popup, "Comment");
  SetValue (tsp->field_choice_popup, 1);
  tsp->string_loc_grp = HiddenGroup (h, 3, 0, NULL);
  RadioButton (tsp->string_loc_grp, "Anywhere in field");
  RadioButton (tsp->string_loc_grp, "At beginning of field");
  RadioButton (tsp->string_loc_grp, "At end of field");
  SetValue (tsp->string_loc_grp, 3);
  m = HiddenGroup (h, 2, 0, NULL);
  tsp->case_insensitive_btn = CheckBox (m, "Case Insensitive", NULL);
  tsp->when_string_not_found_btn = CheckBox (m, "When string is not found", NULL);  
  tsp->remove_found_text_btn = CheckBox (m, "Remove found text", NULL);

  c = HiddenGroup (h, 4, 0, NULL);
  b = DefaultButton (c, "Accept", AddTypeStrainCommentsWithConstraintProc);
  SetObjectExtra (b, tsp, NULL);
  b = PushButton (c, "Cancel", StdCancelButtonProc); 
  SetObjectExtra (b, tsp, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) k, (HANDLE) l, (HANDLE) tsp->string_loc_grp, 
                (HANDLE) m, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}

extern void AddTypeStrainCommentsToAll (IteM i)
{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  /* Visit each bioseq to remove redundant proprotein misc feats */
  VisitBioSourcesInSep (sep, NULL, AddTypeStrainCommentsProc);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  ArrowCursor ();
  Update ();
}

extern void SqnNewAlign (BioseqPtr bsp1, BioseqPtr bsp2, SeqAlignPtr PNTR salp)
{
  BLAST_SummaryOptions *options = NULL;
  Uint1 mol_was;

  if (bsp1 == NULL || bsp2 == NULL || salp == NULL) return;

  *salp = NULL;
  if (ISA_na (bsp1->mol) != ISA_na (bsp2->mol)) return;

  mol_was = bsp2->mol;
  bsp2->mol = bsp1->mol;
  BLAST_SummaryOptionsInit(&options);

  options->cutoff_evalue = 0.001;
  if (bsp1->length > 10000 || bsp2->length > 10000)
  {
    options->filter_string = StringSave ("m L");
    options->word_size = 20;
    options->cutoff_evalue = act_get_eval (60);
    if (ISA_na (bsp1->mol))
    {
      options->program = eBlastn;
    }
    else
    {
      options->program = eBlastp;
    }
    options->hint = eNone;
  }

  BLAST_TwoSequencesSearch(options, bsp1, bsp2, salp);
  bsp2->mol = mol_was;
  BLAST_SummaryOptionsFree(options);
  
}

/* This section of code is for the Remove Sequences From Alignments function. */

typedef struct alignmentsequencelist {
  SeqIdPtr sip;
  Char     descr[255];
} AlignmentSequenceListData, PNTR AlignmentSequenceListPtr;

static void 
ListSequencesInSeqEntry 
(SeqEntryPtr sep,
 ValNodePtr PNTR list, 
 Boolean show_nucs, 
 Boolean show_prots)
{
  BioseqPtr                bsp;
  BioseqSetPtr             bssp;
  ValNodePtr               vnp;
  AlignmentSequenceListPtr aslp;
  Int4                     offset;
  SeqIdPtr                 bsp_sip;
  
  if (sep == NULL) return;
  
  if (IS_Bioseq (sep))
  {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp == NULL) return;
    if (!show_nucs && ISA_na (bsp->mol))
    {
      return;
    }
    if (!show_prots && ISA_aa (bsp->mol))
    {
      return;
    }
    aslp = (AlignmentSequenceListPtr) MemNew (sizeof (AlignmentSequenceListData));
    if (aslp == NULL) return;
    aslp->sip = bsp->id;
    aslp->descr[0] = 0;
	  aslp->descr[253] = 0;
    offset = 0;
    for (bsp_sip = bsp->id; bsp_sip != NULL && offset < 250; bsp_sip = bsp_sip->next) {
	  if (aslp->descr[0] != 0) {
	    aslp->descr[offset] = ':';
	    offset ++;
	  }
      SeqIdWrite (bsp_sip, aslp->descr + offset, PRINTID_TEXTID_ACCESSION, 254 - offset);
      offset = StringLen (aslp->descr);
	}
    vnp = ValNodeNew (*list);
    if (vnp != NULL)
    {
      vnp->data.ptrvalue = aslp;
    }
    if (*list == NULL)
    {
      *list = vnp;
    }
  }
  else
  {
  	bssp = (BioseqSetPtr) sep->data.ptrvalue;
    for (sep = bssp->seq_set; sep != NULL; sep = sep->next) 
    {
      ListSequencesInSeqEntry (sep, list, show_nucs, show_prots);
    }
  }
}

typedef struct sequencelistctrl
{
  ValNodePtr      sequence_list;
  Nlm_LstActnProc actn;
  Pointer         userdata;
  
} SequenceListCtrlData, PNTR SequenceListCtrlPtr;

static void CleanupSequenceListCtrl (
  GraphiC g,
  VoidPtr data
)

{
  SequenceListCtrlPtr slcp;

  slcp = (SequenceListCtrlPtr) data;
  if (slcp != NULL) {
	  slcp->sequence_list = ValNodeFreeData (slcp->sequence_list);
  }
  MemFree (slcp);
}


static void SequenceListCtrlAction (LisT l)
{
  SequenceListCtrlPtr slcp;
  
  slcp = (SequenceListCtrlPtr) GetObjectExtra (l);
  if (slcp == NULL) return;
  
  if (slcp->actn != NULL)
  {
    SetObjectExtra (l, slcp->userdata, NULL);
    (slcp->actn) (l);
    SetObjectExtra (l, slcp, CleanupSequenceListCtrl);
  }
}

extern LisT 
MakeSequenceListControl 
(GrouP g,
 SeqEntryPtr sep,
 Nlm_LstActnProc actn,
 Pointer userdata,
 Boolean show_nucs,
 Boolean show_prots)
{
  LisT                     list_ctrl;
  SequenceListCtrlPtr      slcp;
  ValNodePtr               vnp;
  AlignmentSequenceListPtr aslp;
  
  slcp = (SequenceListCtrlPtr) MemNew (sizeof (SequenceListCtrlData));
  slcp->actn = actn;
  slcp->userdata = userdata;
  ListSequencesInSeqEntry (sep, &slcp->sequence_list, show_nucs, show_prots);
  
  list_ctrl = MultiList (g, 20, 8, SequenceListCtrlAction);
  SetObjectExtra (list_ctrl, slcp, CleanupSequenceListCtrl);
  
  for (vnp = slcp->sequence_list; vnp != NULL; vnp = vnp->next) {
    aslp = vnp->data.ptrvalue;
	  if (aslp != NULL) 
	  {
      ListItem (list_ctrl, aslp->descr);
	  }
  }

  return list_ctrl;
  
}


extern void SelectAllSequencesInListCtrl (LisT l)
{
  SequenceListCtrlPtr   slcp;
  ValNodePtr            vnp;
  Int2                  val;
  
  
  slcp = (SequenceListCtrlPtr) GetObjectExtra (l);
  if (slcp == NULL) return;
  
  for (val = 1, vnp = slcp->sequence_list; vnp != NULL; vnp = vnp->next, val++)
  {
    SetItemStatus (l, val, TRUE);
  }  
}


extern void UnSelectAllSequencesInListCtrl (LisT l)
{
  SequenceListCtrlPtr   slcp;
  ValNodePtr            vnp;
  Int2                  val;
  
  
  slcp = (SequenceListCtrlPtr) GetObjectExtra (l);
  if (slcp == NULL) return;
  
  for (val = 1, vnp = slcp->sequence_list; vnp != NULL; vnp = vnp->next, val++)
  {
    SetItemStatus (l, val, FALSE);
  }  
}


extern ValNodePtr GetSelectedSequenceList (LisT l)
{
  SequenceListCtrlPtr      slcp;
  ValNodePtr               sip_list = NULL, vnp;
  Int2                     val;
  AlignmentSequenceListPtr aslp;

  slcp = (SequenceListCtrlPtr) GetObjectExtra (l);
  if (slcp == NULL) return NULL;
  
  val = 1;
  for (vnp = slcp->sequence_list; vnp != NULL; vnp = vnp->next) 
  {
    aslp = vnp->data.ptrvalue;
	  if (aslp == NULL) continue;
	  if (GetItemStatus (l, val)) 
	  {
	    ValNodeAddPointer (&sip_list, 0, aslp->sip);
	  }
	  val++;
  }
  
  return sip_list;
}

/* This function is used so that a sequence ID will only appear once in the list,
 * even if it appears in more than one alignment or subalignment.
 */
static Boolean IsIDAlreadyInList (SeqIdPtr sip, ValNodePtr list)
{
  ValNodePtr vnp;
  AlignmentSequenceListPtr aslp;
  
  if (sip == NULL) return FALSE;
  
  for (vnp = list; vnp != NULL; vnp = vnp->next)
  {
    aslp = (AlignmentSequenceListPtr) vnp->data.ptrvalue;
    if (aslp != NULL && SeqIdComp (aslp->sip, sip) == SIC_YES)
    {
      return TRUE;
    }
  }
  return FALSE;
}

/* This function creates the list of sequence IDs and descriptions to use in 
 * the Remove Sequences From Alignments dialog.
 */
static void ListSequencesInAlignmentsCallback (SeqAnnotPtr sap, Pointer userdata)
{
  SeqAlignPtr salp;
  SeqIdPtr    sip_list, sip, bsp_sip;
  ValNodePtr PNTR list;
  ValNodePtr  vnp; 
  AlignmentSequenceListPtr aslp;
  BioseqPtr                bsp;
  Int4                     offset;

  if (sap == NULL || sap->type != 2 || userdata == NULL) return;
  salp = (SeqAlignPtr) sap->data;
  while (salp != NULL) 
  {
    list = (ValNodePtr PNTR)userdata;
    sip_list = SeqAlignIDList (salp);
    if (sip_list == NULL) return;
    for (sip = sip_list; sip != NULL; sip = sip->next) {
      if (IsIDAlreadyInList (sip, *list)) continue;
      aslp = (AlignmentSequenceListPtr) MemNew (sizeof (AlignmentSequenceListData));
	  if (aslp == NULL) return;
	  aslp->sip = sip;
	  bsp = BioseqFindCore (sip);
	  if (bsp != NULL) {
		  aslp->descr[0] = 0;
		  aslp->descr[253] = 0;
		  offset = 0;
		  for (bsp_sip = bsp->id; bsp_sip != NULL && offset < 250; bsp_sip = bsp_sip->next) {
			if (aslp->descr[0] != 0) {
			  aslp->descr[offset] = '\t';
			  offset ++;
			}
		    SeqIdWrite (bsp_sip, aslp->descr + offset, PRINTID_TEXTID_ACCESSION, 254 - offset);
			offset = StringLen (aslp->descr);
		  }
	  } else {
        SeqIdWrite (sip, aslp->descr, PRINTID_TEXTID_ACCESSION, 254);	    
	  }
	  vnp = ValNodeNew (*list);
	  vnp->data.ptrvalue = aslp;
	  if (*list == NULL) {
		  *list = vnp;
	  }	  
    }
    salp = salp->next;
  }
}

static ValNodePtr ListSequencesInAlignments (SeqEntryPtr sep)
{
	ValNodePtr list = NULL;
    VisitAnnotsInSep (sep, (Pointer) &list, ListSequencesInAlignmentsCallback);
    return list;
}

static LisT MakeAlignmentSequenceListControl (GrouP g, SeqEntryPtr sep, Nlm_LstActnProc actn, Pointer userdata)
{
  LisT                     list_ctrl;
  SequenceListCtrlPtr      slcp;
  ValNodePtr               vnp;
  AlignmentSequenceListPtr aslp;
  
  slcp = (SequenceListCtrlPtr) MemNew (sizeof (SequenceListCtrlData));
  slcp->actn = actn;
  slcp->userdata = userdata;
  slcp->sequence_list = ListSequencesInAlignments (sep);
  
  list_ctrl = MultiList (g, 16, 16, SequenceListCtrlAction);
  SetObjectExtra (list_ctrl, slcp, CleanupSequenceListCtrl);
  
  for (vnp = slcp->sequence_list; vnp != NULL; vnp = vnp->next) {
    aslp = vnp->data.ptrvalue;
	  if (aslp != NULL) 
	  {
      ListItem (list_ctrl, aslp->descr);
	  }
  }

  return list_ctrl;
  
}

typedef struct removeseqfromaligndata {
  FORM_MESSAGE_BLOCK
  LisT        sequence_list_ctrl;
  SeqEntryPtr sep;
  Boolean     remove_all_from_alignments;
  Boolean     no_remove_all_from_alignments;
  Boolean     remove_all_products;
  Boolean     no_remove_all_products;
} RemoveSeqFromAlignData, PNTR RemoveSeqFromAlignPtr;

static void DoRemoveSequencesFromAlignment (ButtoN b)
{
  RemoveSeqFromAlignPtr    rp;
  WindoW                   w;
  ValNodePtr               vnp, sip_list;
  
  if (b == NULL) return;
  rp = (RemoveSeqFromAlignPtr) GetObjectExtra (b);
  if (rp == NULL) return;
  
  w = (WindoW) rp->form;
  Hide (w);
  /* first, check for pairwise alignments */
  sip_list = GetSelectedSequenceList (rp->sequence_list_ctrl);
  for (vnp = sip_list; vnp != NULL; vnp = vnp->next)
  {
    if (IsSequenceFirstInPairwise (rp->sep, (SeqIdPtr) vnp->data.ptrvalue))
	  {
	  	Message (MSG_ERROR, "One of the selected sequences is the first in a pairwise alignment."
	  	"  You must convert the alignment to a multiple alignment before trying to remove this sequence.");
      Remove (rp->form);  
      ValNodeFree (sip_list);
      return;
	  }
  }

  for (vnp = sip_list; vnp != NULL; vnp = vnp->next)
  {
    RemoveSequenceFromAlignments (rp->sep, (SeqIdPtr) vnp->data.ptrvalue);
  }
 
  ValNodeFree (sip_list);
  DeleteMarkedObjects (rp->input_entityID, 0, NULL);
  ObjMgrSetDirtyFlag (rp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, rp->input_entityID, 0, 0);
  Remove (rp->form);  
}


extern void RemoveSequencesFromAlignment (IteM i)
{
  BaseFormPtr              bfp;
  WindoW                   w;
  RemoveSeqFromAlignPtr    rp;
  GrouP                    h, k, c;
  ButtoN                   b;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  if (bfp == NULL) return;

  rp = (RemoveSeqFromAlignPtr) MemNew (sizeof (RemoveSeqFromAlignData));
  if (rp == NULL) return;
  rp->input_entityID = bfp->input_entityID;
  rp->sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (rp->sep == NULL) 
  {
	  MemFree (rp);
	  return;
  }

  w = FixedWindow (-50, -33, -10, -10, "Remove Sequences From Alignment", StdCloseWindowProc);
  if (w == NULL) {
	MemFree (rp);
	return;
  }
  rp->form = (ForM) w;
  SetObjectExtra (w, rp, StdCleanupFormProc);
  
  h = HiddenGroup (w, -1, 0, NULL);
  k = HiddenGroup (h, 2, 0, NULL);

  rp->sequence_list_ctrl = MakeAlignmentSequenceListControl (k, rp->sep, NULL, NULL);

  c = HiddenGroup (h, 4, 0, NULL);
  b = DefaultButton (c, "Accept", DoRemoveSequencesFromAlignment);
  SetObjectExtra (b, rp, NULL);
  b = PushButton (c, "Cancel", StdCancelButtonProc); 
  SetObjectExtra (b, rp, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) k, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}

/* End of Remove Sequences From Alignments function code. */

/* This section of code is used for removing sequences from the record. */

typedef struct bioseqinalignmentdata {
	Boolean   found;
	BioseqPtr lookingfor;
} BioseqInAlignmentData, PNTR BioseqInAlignmentPtr;

static Boolean IsBioseqInThisAlignment (SeqAlignPtr salp, BioseqPtr bsp)
{
  SeqIdPtr sip;
  Boolean found = FALSE;

  for (sip = bsp->id; sip != NULL && ! found; sip = sip->next) 
  {
    found = SeqAlignFindSeqId (salp, sip);
  }
  return found;
}

static void FindAlignmentCallback (SeqAnnotPtr sap, Pointer userdata)
{
  BioseqInAlignmentPtr biap;
  SeqAlignPtr          salp;

  if (sap == NULL || sap->type != 2 || userdata == NULL) 
  {
    return;
  }
  biap = (BioseqInAlignmentPtr) userdata;
  if (biap->found) return;
  salp = (SeqAlignPtr) sap->data;
  if (salp == NULL) return;
  biap->found = IsBioseqInThisAlignment (salp, biap->lookingfor);

}

static Boolean IsBioseqInAnyAlignment (BioseqPtr bsp, Uint2 input_entityID)
{
  SeqEntryPtr           topsep;
  BioseqInAlignmentData biad;

  topsep = GetTopSeqEntryForEntityID (input_entityID);
  biad.found = FALSE;
  biad.lookingfor = bsp;

  VisitAnnotsInSep (topsep, &biad, FindAlignmentCallback);
  return biad.found;
}

static void DoesBioseqHaveFeaturesWithProductsCallback (SeqFeatPtr sfp, Pointer userdata)
{
  ValNodePtr PNTR list;
  ValNodePtr vnp;
  
  if (sfp == NULL || userdata == NULL) return;
  list = (ValNodePtr PNTR) userdata;
  
  if (sfp->product != NULL)
  {
  	vnp = ValNodeNew (*list);
  	if (vnp != NULL)
  	{
  	  vnp->data.ptrvalue = sfp;
  	}
  	if (*list == NULL)
  	{
  	  *list = vnp;
  	}
  }
}

static void RemoveBioseq (BioseqPtr bsp, RemoveSeqFromAlignPtr rp);

static void RemoveBioseqProducts (ValNodePtr product_feature_list, RemoveSeqFromAlignPtr rp)
{
  ValNodePtr vnp;
  SeqFeatPtr sfp;
  BioseqPtr  bsp;
  
  for (vnp = product_feature_list; vnp != NULL; vnp = vnp->next)
  {
    sfp = (SeqFeatPtr) vnp->data.ptrvalue;
    if (sfp != NULL)
    {
  	  bsp = BioseqFindFromSeqLoc (sfp->product);
  	  sfp->product = SeqLocFree (sfp->product);
  	  RemoveBioseq (bsp, rp);
    }
  }
}

static void RemoveEmptyNucProtSet (SeqEntryPtr sep)
{
  BioseqSetPtr bssp;
  BioseqPtr    bsp;

  if (sep == NULL || !IS_Bioseq_set (sep)) return;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp->_class != BioseqseqSet_class_nuc_prot) return;

  for (sep = bssp->seq_set; sep != NULL; sep = sep->next)
  {
  	if (!IS_Bioseq (sep)) return;
  	bsp = sep->data.ptrvalue;
  	if (bsp != NULL && !bsp->idx.deleteme) return;
  }
  bssp->idx.deleteme = TRUE;
}

typedef struct removealnorproductans 
{
  WindoW  w;
  Boolean ans;
  Boolean do_all;
  Boolean done;
} RemoveAlnOrProductAnsData, PNTR RemoveAlnOrProductAnsPtr;

static void RemoveAlnOrProductYes (ButtoN b)
{
  RemoveAlnOrProductAnsPtr rp;
  
  rp = (RemoveAlnOrProductAnsPtr) GetObjectExtra (b);
  if (rp == NULL) return;
  rp->ans = TRUE;
  rp->do_all = FALSE;
  Remove (rp->w);
  rp->done = TRUE;
}

static void RemoveAlnOrProductYesAll (ButtoN b)
{
  RemoveAlnOrProductAnsPtr rp;
  
  rp = (RemoveAlnOrProductAnsPtr) GetObjectExtra (b);
  if (rp == NULL) return;
  rp->ans = TRUE;
  rp->do_all = TRUE;
  Remove (rp->w);
  rp->done = TRUE;
}

static void RemoveAlnOrProductNo (ButtoN b)
{
  RemoveAlnOrProductAnsPtr rp;
  
  rp = (RemoveAlnOrProductAnsPtr) GetObjectExtra (b);
  if (rp == NULL) return;
  rp->ans = FALSE;
  rp->do_all = FALSE;
  Remove (rp->w);
  rp->done = TRUE;
}

static void RemoveAlnOrProductNoAll (ButtoN b)
{
  RemoveAlnOrProductAnsPtr rp;
  
  rp = (RemoveAlnOrProductAnsPtr) GetObjectExtra (b);
  if (rp == NULL) return;
  rp->ans = FALSE;
  rp->do_all = TRUE;
  Remove (rp->w);
  rp->done = TRUE;
}

static Boolean GetRemoveAlignments (RemoveSeqFromAlignPtr rp, CharPtr idstr)
{
  RemoveAlnOrProductAnsData rd;

  GrouP                    g, h, c;
  ButtoN                   b;
  CharPtr                  prompt_fmt = "%s is part of an alignment - would you like to remove it from the alignment before deleting it?";
  CharPtr                  prompt_str = NULL;
  
  if (rp == NULL || idstr == NULL) return FALSE;
  if (rp->remove_all_from_alignments) return TRUE;
  if (rp->no_remove_all_from_alignments) return FALSE;

  prompt_str = (CharPtr) MemNew (sizeof (Char) * (StringLen (prompt_fmt) + StringLen (idstr)));
  if (prompt_str == NULL) return FALSE;
  sprintf (prompt_str, prompt_fmt, idstr);
  rd.w = ModalWindow(-20, -13, -10, -10, NULL);
  h = HiddenGroup(rd.w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
  rd.done = FALSE;
  g = HiddenGroup (h, 1, 0, NULL);
  StaticPrompt (g, prompt_str, 0, popupMenuHeight, programFont, 'l');
  c = HiddenGroup (h, 4, 0, NULL);
  b = PushButton(c, "Yes", RemoveAlnOrProductYes);
  SetObjectExtra (b, &rd, NULL);
  b = PushButton(c, "Remove All", RemoveAlnOrProductYesAll);
  SetObjectExtra (b, &rd, NULL);
  b = DefaultButton(c, "No", RemoveAlnOrProductNo);
  SetObjectExtra (b, &rd, NULL);
  b = DefaultButton(c, "Remove None", RemoveAlnOrProductNoAll);
  SetObjectExtra (b, &rd, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
  prompt_str = MemFree (prompt_str);
  
  Show(rd.w); 
  Select (rd.w);
  rd.done = FALSE;
  while (!rd.done)
  {
    ProcessExternalEvent ();
    Update ();
  }
  ProcessAnEvent ();
  if (rd.do_all)
  {
    if (rd.ans)
    {
  	  rp->remove_all_from_alignments = TRUE;
  	  rp->no_remove_all_from_alignments = FALSE;
    }
    else
    {
  	  rp->remove_all_from_alignments = FALSE;
  	  rp->no_remove_all_from_alignments = TRUE;
    }
  }
  return rd.ans;
}


static Boolean GetRemoveProducts (RemoveSeqFromAlignPtr rp, CharPtr idstr)
{
  RemoveAlnOrProductAnsData rd;

  GrouP                    g, h, c;
  ButtoN                   b;
  CharPtr                  prompt_fmt = "%s contains features that have products (proteins, etc.).  Would you like to remove the product sequences?";
  CharPtr                  prompt_str = NULL;
  
  if (rp == NULL || idstr == NULL) return FALSE;
  if (rp->remove_all_products) return TRUE;
  if (rp->no_remove_all_products) return FALSE;

  prompt_str = (CharPtr) MemNew (sizeof (Char) * (StringLen (prompt_fmt) + StringLen (idstr)));
  if (prompt_str == NULL) return FALSE;
  sprintf (prompt_str, prompt_fmt, idstr);
  rd.w = ModalWindow(-20, -13, -10, -10, NULL);
  h = HiddenGroup(rd.w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
  rd.done = FALSE;
  g = HiddenGroup (h, 1, 0, NULL);
  StaticPrompt (g, prompt_str, 0, popupMenuHeight, programFont, 'l');
  c = HiddenGroup (h, 4, 0, NULL);
  b = PushButton(c, "Yes", RemoveAlnOrProductYes);
  SetObjectExtra (b, &rd, NULL);
  b = PushButton(c, "Remove All", RemoveAlnOrProductYesAll);
  SetObjectExtra (b, &rd, NULL);
  b = DefaultButton(c, "No", RemoveAlnOrProductNo);
  SetObjectExtra (b, &rd, NULL);
  b = DefaultButton(c, "Remove None", RemoveAlnOrProductNoAll);
  SetObjectExtra (b, &rd, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
  prompt_str = MemFree (prompt_str);
  
  Show(rd.w); 
  Select (rd.w);
  rd.done = FALSE;
  while (!rd.done)
  {
    ProcessExternalEvent ();
    Update ();
  }
  ProcessAnEvent ();
  if (rd.do_all)
  {
    if (rd.ans)
    {
  	  rp->remove_all_products = TRUE;
  	  rp->no_remove_all_products = FALSE;
    }
    else
    {
  	  rp->remove_all_products = FALSE;
  	  rp->no_remove_all_products = TRUE;
    }
  }
  return rd.ans;
}


static void RemoveBioseq (BioseqPtr bsp, RemoveSeqFromAlignPtr rp)
{
  ValNodePtr   product_feature_list = NULL;
  Char         str [128];
  SeqEntryPtr  sep;
  
  if (bsp == NULL || rp == NULL) return;	
  
  SeqIdWrite (bsp->id, str, PRINTID_REPORT, sizeof (str));

  if (IsBioseqInAnyAlignment (bsp, rp->input_entityID))
  {
    if (GetRemoveAlignments (rp, str))
    {
      RemoveSequenceFromAlignments (rp->sep, bsp->id);
    }
  }
  VisitFeaturesOnBsp (bsp, &product_feature_list, DoesBioseqHaveFeaturesWithProductsCallback);
  if (product_feature_list != NULL)
  {
    if (GetRemoveProducts (rp, str))
    {
      RemoveBioseqProducts (product_feature_list, rp);
    }
  }
        
  bsp->idx.deleteme = TRUE;
  /* remove nuc-prot set if we are deleting the nucleotide and its proteins */
  sep = GetBestTopParentForData (rp->input_entityID, bsp);
  RemoveEmptyNucProtSet (sep);

  ValNodeFree (product_feature_list);
  
}


static void DoRemoveSequencesFromRecord (ButtoN b)
{
  RemoveSeqFromAlignPtr    rp;
  WindoW                   w;
  ValNodePtr               sip_list, vnp;
  SeqIdPtr                 sip;
  BioseqPtr                bsp;
  
  if (b == NULL) return;
  rp = (RemoveSeqFromAlignPtr) GetObjectExtra (b);
  if (rp == NULL) return;
  
  w = (WindoW) rp->form;
  Hide (w);

  sip_list = GetSelectedSequenceList (rp->sequence_list_ctrl);
  for (vnp = sip_list; vnp != NULL; vnp = vnp->next)
  {
    sip = (SeqIdPtr) vnp->data.ptrvalue;
    bsp = BioseqFind (sip);
	  if (bsp != NULL)
	  {
	    RemoveBioseq (bsp, rp);
	  }
  }
  ValNodeFree (sip_list);
  
  DeleteMarkedObjects (rp->input_entityID, 0, NULL);
  ObjMgrSetDirtyFlag (rp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, rp->input_entityID, 0, 0);
  Remove (rp->form);  
}

extern void RemoveSequencesFromRecord (IteM i)
{
  BaseFormPtr              bfp;
  WindoW                   w;
  RemoveSeqFromAlignPtr    rp;
  GrouP                    h, k, c;
  ButtoN                   b;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  if (bfp == NULL) return;

  rp = (RemoveSeqFromAlignPtr) MemNew (sizeof (RemoveSeqFromAlignData));
  if (rp == NULL) return;
  rp->input_entityID = bfp->input_entityID;
  rp->sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (rp->sep == NULL) 
  {
	  MemFree (rp);
	  return;
  }
  
  rp->remove_all_from_alignments = FALSE;
  rp->remove_all_products = FALSE;
  rp->no_remove_all_from_alignments = FALSE;
  rp->no_remove_all_products = FALSE;
  
  w = FixedWindow (-50, -33, -10, -10, "Remove Sequences From Record", StdCloseWindowProc);
  if (w == NULL) 
  {
	  MemFree (rp);
	  return;
  }
  rp->form = (ForM) w;
  SetObjectExtra (w, rp, StdCleanupFormProc);
  
  h = HiddenGroup (w, -1, 0, NULL);
  k = HiddenGroup (h, 2, 0, NULL);

  rp->sequence_list_ctrl = MakeSequenceListControl (k, rp->sep, NULL, NULL, TRUE, TRUE);

  c = HiddenGroup (h, 4, 0, NULL);
  b = DefaultButton (c, "Accept", DoRemoveSequencesFromRecord);
  SetObjectExtra (b, rp, NULL);
  b = PushButton (c, "Cancel", StdCancelButtonProc); 
  SetObjectExtra (b, rp, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) k, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();  
}
