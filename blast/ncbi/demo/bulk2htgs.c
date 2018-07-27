/*   bulk2htgs.c
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
* File Name:  bulk2htgs.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   11/2/99
*
* $Revision: 6.6 $
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
#include <subutil.h>

static void AddBioSourceToBioseq (BioseqPtr bsp, CharPtr organism, BioSourcePtr bio)

{
  BioSourcePtr  biop;
  OrgRefPtr     orp;
  SeqDescrPtr   sdp;

  if (bsp == NULL) return;

  if (bio != NULL) {
    biop = AsnIoMemCopy ((Pointer) bio,
                         (AsnReadFunc) BioSourceAsnRead,
                         (AsnWriteFunc) BioSourceAsnWrite);
  } else {
    if (StringHasNoText (organism)) return;
    biop = BioSourceNew ();
    if (biop == NULL) return;
    orp = OrgRefNew ();
    if (orp == NULL) return;
    biop->org = orp;
    orp->taxname = StringSave (organism);
  }

  sdp = SeqDescrAdd (&(bsp->descr));
  if (sdp == NULL) return;
  sdp->choice = Seq_descr_source;
  sdp->data.ptrvalue = (Pointer) biop;
}

static void AddMolInfoToBioseq (BioseqPtr bsp, Boolean is_mrna, Int2 htgs_phase)

{
  MolInfoPtr   mip;
  SeqDescrPtr  sdp;

  if (bsp == NULL) return;

  mip = MolInfoNew ();
  if (mip == NULL) return;
  if (is_mrna) {
    mip->biomol = MOLECULE_TYPE_MRNA;
  } else {
    mip->biomol = MOLECULE_TYPE_GENOMIC;
  }
  switch (htgs_phase) {
    case 0 :
      mip->tech = MI_TECH_htgs_0;
      break;
    case 1 :
      mip->tech = MI_TECH_htgs_1;
      break;
    case 2 :
      mip->tech = MI_TECH_htgs_2;
      break;
    case 3 :
      mip->tech = MI_TECH_htgs_3;
      break;
    default :
      break;
  }

  sdp = SeqDescrAdd (&(bsp->descr));
  if (sdp == NULL) return;
  sdp->choice = Seq_descr_molinfo;
  sdp->data.ptrvalue = (Pointer) mip;
}

static void ConvertSeqID (BioseqPtr bsp, CharPtr general,
                          Boolean parse_colon, Boolean id_comment)

{
  Char         ch;
  CharPtr      db, id, ptr;
  DbtagPtr     dbt;
  Char         idcom [128], tmp [128];
  Boolean      justdigits;
  ObjectIdPtr  oip;
  SeqDescrPtr  sdp;
  SeqIdPtr     sip = NULL;
  long int     val;

  if (bsp == NULL) return;

  for (sip = bsp->id;
       sip != NULL && sip->choice != SEQID_LOCAL;
       sip = sip->next) continue;
  if (sip == NULL) return;

  oip = (ObjectIdPtr) sip->data.ptrvalue;
  if (oip == NULL) return;
  if (oip->str != NULL) {
    StringNCpy_0 (tmp, oip->str, sizeof (tmp));
  } else {
    sprintf (tmp, "%ld", (long) oip->id);
  }

  /* if colon in localid, parse db and id separately */

  ptr = StringChr (tmp, ':');
  if (parse_colon && ptr != NULL) {
    db = tmp;
    *ptr = '\0';
    ptr++;
    id = ptr;
  } else {
    db = NULL;
    id = tmp;
  }

  /* ignore db in localid if general tag passed in */

  if (! StringHasNoText (general)) {
    db = general;
  }

  if (StringHasNoText (db) || StringHasNoText (id)) return;
  dbt = DbtagNew ();
  if (dbt == NULL) return;

  /* insert dbtag between seqid and objectid, change choice and objectid */

  sip->choice = SEQID_GENERAL;
  sip->data.ptrvalue = (Pointer) dbt;
  dbt->db = StringSave (db);
  dbt->tag = oip;
  oip->str = MemFree (oip->str);

  for (justdigits = TRUE, ptr = id, ch = *ptr;
       ch != '\0';
       ptr++, ch = *ptr) {
    if (ch == ' ' || ch == '+' || ch == '-') {
    } else if (! IS_DIGIT (ch)) {
      justdigits = FALSE;
    }
  }

  if (justdigits && sscanf (id, "%ld", &val) == 1) {
    oip->id = (Int4) val;
  } else {
    oip->str = StringSave (id);
  }

  if (id_comment) {
    if (oip->str != NULL) {
      StringNCpy_0 (tmp, oip->str, sizeof (tmp));
    } else {
      sprintf (tmp, "%ld", oip->id);
    }
    sprintf (idcom, "This sequence was identified as %s by the submitter", tmp);
    sdp = SeqDescrAdd (&(bsp->descr));
    if (sdp != NULL) {
      sdp->choice = Seq_descr_comment;
      sdp->data.ptrvalue = (Pointer) StringSave (idcom);
    }
  }


  SeqMgrReplaceInBioseqIndex (bsp);
}

static void ProcessOneRecord (SeqSubmitPtr ssp, CharPtr organism,
                              BioSourcePtr biop, CharPtr general,
                              FILE* ofp, Boolean is_mrna,
                              Int2 htgs_phase, Boolean parse_colon,
                              Boolean id_comment, CharPtr comment,
                              Uint2 datatype, Pointer dataptr)

{
  AsnIoPtr     aip;
  BioseqPtr    bsp;
  Int4         pos;
  SeqDescrPtr  sdp;
  SeqEntryPtr  sep;

  if (ssp == NULL || ofp == NULL) return;
  if (organism == NULL && biop == NULL) return;
  if (datatype != OBJ_BIOSEQ) return;

  bsp = (BioseqPtr) dataptr;
  if (bsp == NULL) return;
  sep = SeqMgrGetSeqEntryForData (bsp);
  if (sep == NULL) return;

  AddBioSourceToBioseq (bsp, organism, biop);
  AddMolInfoToBioseq (bsp, is_mrna, htgs_phase);
  if (is_mrna) {
    bsp->mol = Seq_mol_rna;
  } else {
    bsp->mol = Seq_mol_dna;
  }
  ConvertSeqID (bsp, general, parse_colon, id_comment);
  if (! StringHasNoText (comment)) {
    sdp = SeqDescrAdd (&(bsp->descr));
    if (sdp != NULL) {
      sdp->choice = Seq_descr_comment;
      sdp->data.ptrvalue = (Pointer) StringSave (comment);
    }
  }
  sdp = SeqDescrAdd (&(bsp->descr));
  if (sdp != NULL) {
    sdp->choice = Seq_descr_create_date;
    sdp->data.ptrvalue = (Pointer) DateCurr ();
  }

  ssp->data = sep;
  ssp->datatype = 1;

  aip = AsnIoNew (ASNIO_TEXT_OUT, ofp, NULL, NULL, NULL);

  SeqSubmitAsnWrite (ssp, aip, NULL);

  pos = AsnIoTell (aip);
  AsnIoFree (aip, FALSE);
  fseek (ofp, pos, SEEK_SET);
  fprintf (ofp, "\n");

  ssp->data = NULL;
  SeqEntryFree (sep);
}

static BioSourcePtr ReadBioSource (CharPtr path)

{
  AsnIoPtr      aip;
  BioSourcePtr  biop = NULL;

  aip = AsnIoOpen (path, "r");
  if (aip == NULL) return NULL;

  biop = BioSourceAsnRead (aip, NULL);

  AsnIoClose (aip);

  return biop;
}

/* template file can contain either Seq-submit or Submit-block */

static SeqSubmitPtr ReadSubmitBlock (CharPtr path)

{
  CitSubPtr       csp;
  Pointer         dataptr;
  Uint2           datatype;
  FILE            *fp;
  SubmitBlockPtr  sbp = NULL;
  SeqSubmitPtr    ssp = NULL;

  fp = FileOpen (path, "r");
  if (fp == NULL) return NULL;

  dataptr = ReadAsnFastaOrFlatFile (fp, &datatype,  NULL, FALSE,
                                    FALSE, TRUE, FALSE);
  FileClose (fp);

  switch (datatype) {
    case OBJ_SUBMIT_BLOCK :
      sbp = (SubmitBlockPtr) dataptr;
      ssp = SeqSubmitNew ();
      if (ssp != NULL) {
        ssp->sub = sbp;
      }
      break;
    case OBJ_SEQSUB :
      ssp = (SeqSubmitPtr) dataptr;
      if (ssp != NULL) {
        sbp = ssp->sub;
      }
      break;
    default :
      break;
  }

  if (sbp != NULL) {
    csp = sbp->cit;
    if (csp != NULL) {
      csp->date = DateFree (csp->date);
      csp->date = DateCurr ();
    }
  }

  return ssp;
}

Args myargs [] = {
  {"Filename for FASTA input", "stdin", NULL, NULL,
    FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  {"Filename for Seq-submit template", NULL, NULL, NULL,
    FALSE, 't', ARG_FILE_IN, 0.0, 0, NULL},
  {"Filename for ASN.1 output", "stdout", NULL, NULL,
    FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
  {"Organism name", NULL, NULL, NULL,
    TRUE, 'n', ARG_STRING, 0.0, 0, NULL},
  {"General ID tag", NULL, NULL, NULL,
    TRUE, 'g', ARG_STRING, 0.0, 0, NULL},
  {"Sequences are mRNA?", "F", NULL, NULL,
    TRUE, 'm', ARG_BOOLEAN, 0.0, 0, NULL},
  {"HTGS phase?", "1", "0" ,"3",
    FALSE, 'p', ARG_INT, 0.0, 0, NULL},
  {"Parse colon in tag", "F", NULL, NULL,
    TRUE, 'd', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Comment", NULL, NULL, NULL,
    TRUE, 'c', ARG_STRING, 0.0, 0, NULL},
  {"Filename for BioSource", NULL, NULL, NULL,
    TRUE, 'b', ARG_FILE_IN, 0.0, 0, NULL},
  {"Make identifier comment", "F", NULL, NULL,
    TRUE, 'f', ARG_BOOLEAN, 0.0, 0, NULL},
};

Int2 Main (void)

{
  BioSourcePtr  biop = NULL;
  Pointer       dataptr;
  Uint2         datatype;
  CharPtr       fasta_fname, template_fname, output_fname,
                organism, generalid, comment, biosource_fname;
  Int2          htgs_phase;
  FILE          *ifp, *ofp;
  Boolean       id_comment, is_mrna, parse_colon;
  SeqSubmitPtr  ssp;

  ErrSetFatalLevel (SEV_FATAL);
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

  if (! GetArgs ("bulk2htgs", sizeof (myargs) / sizeof (Args), myargs)) {
    return 0;
  }

  fasta_fname = myargs [0].strvalue;
  template_fname = myargs [1].strvalue;
  output_fname = myargs [2].strvalue;
  organism = myargs [3].strvalue;
  generalid = myargs [4].strvalue;
  is_mrna = (Boolean) myargs [5].intvalue;
  htgs_phase = (Int2) myargs [6].intvalue;
  parse_colon = (Boolean) myargs [7].intvalue;
  comment = myargs [8].strvalue;
  biosource_fname = myargs [9].strvalue;
  id_comment = (Boolean) myargs [10].intvalue;

  if (StringHasNoText (output_fname)) {
    Message (MSG_FATAL, "Unable to open output file");
    return 1;
  }

  ssp = ReadSubmitBlock (template_fname);
  if (ssp == NULL) {
    Message (MSG_FATAL, "Unable to read template file");
    return 1;
  }
  ssp->datatype = 1;

  if (! StringHasNoText (biosource_fname)) {
    biop = ReadBioSource (biosource_fname);
    if (biop == NULL) {
      Message (MSG_FATAL, "Unable to read BioSource file");
      return 1;
    }
  }
  if (biop == NULL && StringHasNoText (organism)) {
    Message (MSG_FATAL, "Organism name or BioSource file is required for processing");
    return 1;
  }


  ifp = FileOpen (fasta_fname, "r");
  if (ifp == NULL) {
    Message (MSG_FATAL, "Unable to open input file");
    return 1;
  }

  ofp = FileOpen (output_fname, "w");
  if (ofp == NULL) {
    Message (MSG_FATAL, "Unable to create output file");
    return 1;
  }

  while ((dataptr = ReadAsnFastaOrFlatFile (ifp, &datatype, NULL, FALSE,
                                            FALSE, TRUE, FALSE)) != NULL) {
    ProcessOneRecord (ssp, organism, biop, generalid, ofp,
                      is_mrna, htgs_phase, parse_colon, id_comment,
                      comment, datatype, dataptr);
  }

  FileClose (ofp);
  FileClose (ifp);

  BioSourceFree (biop);
  SeqSubmitFree (ssp);

  return 0;
}

