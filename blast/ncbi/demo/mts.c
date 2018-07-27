/*
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
* File Name: mts.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.11 $
*
* File Description: profile search
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: mts.c,v $
* Revision 6.11  1999/07/13 13:07:33  sicotte
* prefix functions SeqAlignSort* with URK to resolve toolkit conflicts
*
* Revision 6.10  1998/12/18 16:24:55  kuzio
* big GIs
*
* Revision 6.9  1998/11/16 14:34:12  kuzio
* flagBoundaryCondition
*
* Revision 6.8  1998/09/28 16:34:27  kuzio
* cmdln phrasing
*
* Revision 6.7  1998/09/16 18:19:29  kuzio
* cvs logging
*
* ==========================================================================
*/

#include <ncbi.h>
#include <accentr.h>
#include <gather.h>
#include <tofasta.h>
#include <urkutil.h>
#include <urkptpf.h>

#define TOP_ERROR 1
static char _this_module[] = "mts";
#undef  THIS_MODULE
#define THIS_MODULE _this_module
static char _this_file[] = __FILE__;
#undef  THIS_FILE
#define THIS_FILE _this_file

typedef struct gather_Bioseq
{
  Int4      gi;
  BioseqPtr bsp;
} Gather_BS, PNTR Gather_BSPtr;

Args myargs[] =
{
  { "profile matrix", NULL, NULL, NULL, FALSE,
    'm', ARG_STRING, 0.0, 0, NULL},
  { "integer matrix", "FALSE", "TRUE", "FALSE", TRUE,
    'M', ARG_BOOLEAN, 0.0, 0, NULL},
  { "GI", "0", "0", "9000000", TRUE,
    'g', ARG_INT, 0.0, 0, NULL},
  { "UID (GI) list", NULL, NULL, NULL, TRUE,
    'G', ARG_STRING, 0.0, 0, NULL},
  { "FastA file", NULL, NULL, NULL, TRUE,
    'f', ARG_STRING, 0.0, 0, NULL},
  { "FastA file is nucleotide", "FALSE", "TRUE", "FALSE", TRUE,
    'n', ARG_BOOLEAN, 0.0, 0, NULL},
  { "cutoff score", "0.0", "-1000000.0", "1000000.0", TRUE,
    'c', ARG_FLOAT, 0.0, 0, NULL},
  { "show hit sequences", "FALSE", "TRUE", "FALSE", TRUE,
    'a', ARG_BOOLEAN, 0.0, 0, NULL},
  { "output file", NULL, NULL, NULL, TRUE,
    'o', ARG_STRING, 0.0, 0, NULL},
  { "forced window size", "0", "0", "256", TRUE,
    'w', ARG_INT, 0.0, 0, NULL}
};

static Boolean GetBioseq (GatherContextPtr gcp)
{
  Gather_BSPtr   gbsp;
  BioseqPtr      bsp;
  Int4           gi, entrezgi;

  if (gcp == NULL)
    return FALSE;
  if ((gbsp = (Gather_BSPtr) gcp->userdata) == NULL)
    return FALSE;

  if (gbsp->bsp != NULL)
    return TRUE;
  if (gcp->thistype != OBJ_BIOSEQ)
    return TRUE;
  if ((bsp = (BioseqPtr) (gcp->thisitem)) == NULL)
    return TRUE;

  gi = gbsp->gi;
  if (gi > 0)
  {
    entrezgi = GetGIForSeqId (bsp->id);
    if (gi == entrezgi)
      gbsp->bsp = bsp;
    return TRUE;
  }
  else
  {
    gbsp->bsp = bsp;
    return TRUE;
  }
}

Int2 Main (void)
{
  Int2          argcount;
  Boolean       flagHaveNet;

  Int4          gi;
  SeqEntryPtr   sep;
  Boolean       flagEntrez;

  FILE          *fiop, *fout;
  Char          matrixfile[256], uidfile[256], fastafile[256];
  Char          bufin[256];
  CharPtr       title;
  BioseqPtr     bsp;

  static GatherScope  gs;
  GatherScopePtr      gsp;
  static Gather_BS    gbs;
  Gather_BSPtr        gbsp;

  Boolean       flagFirstPass, flagIsNA;
/*  Boolean       flagIsDNA; */
  ComProfPtr    profile, invprofile;
  FloatHi       cutoff;
  SeqAlignPtr   saph, sap;
  StdSegPtr     ssp;
  SeqLocPtr     slp;
  Int4          start, stop, proflen, windowsize;
  Uint1         strand;
  Char          cstr;
  SeqPortPtr    spp;
  Uint1Ptr      sequence;

  argcount = sizeof (myargs) / sizeof (Args);
  if (!GetArgs ("mts", argcount, myargs))
    return 1;

  if (myargs[2].intvalue == 0     &&
      myargs[3].strvalue == NULL  &&
      myargs[4].strvalue == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 100,
               "No gi, UID list, or FastA file given :: for help :   mts -");
    ErrShow ();
    exit (1);
  }

  StrCpy (matrixfile, myargs[0].strvalue);
  if ((profile = ReadProfile (matrixfile)) == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 101,
               "Failed to read profile: %s", matrixfile);
    ErrShow ();
    exit (1);
  }
  if (myargs[1].intvalue)
    IntegerProfile (profile);
  proflen = ProfLenMax (profile);
  windowsize = myargs[9].intvalue;
  if (windowsize > proflen)
    proflen = CatenateProfile (profile, proflen, windowsize-proflen);
  cutoff = myargs[6].floatvalue;
  if (myargs[7].intvalue == TRUE)
    sequence = (Uint1Ptr) MemNew ((size_t) (sizeof (Uint1) * (proflen+1)));
  else
    sequence = NULL;

  if (myargs[8].strvalue == NULL)
  {
    fout = stdout;
  }
  else
  {
    if ((fout = FileOpen (myargs[8].strvalue, "w")) == NULL)
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 101,
                 "Failed to open output file: %s", myargs[8].strvalue);
      ErrShow ();
      exit (1);
    }
  }

  gsp = &gs;
  gbsp = &gbs;

  MemSet ((Pointer) gsp, 0, sizeof (GatherScope));
  MemSet ((Pointer) gsp->ignore, (int) (TRUE),
          (size_t) (OBJ_MAX * sizeof (Boolean)));

  gsp->ignore[OBJ_BIOSEQ] = FALSE;

  uidfile[0] = '\0';
  fastafile[0] = '\0';

  gi = myargs[2].intvalue;
  if (myargs[3].strvalue != NULL)
    StrCpy (uidfile, myargs[3].strvalue);
  if (myargs[4].strvalue != NULL)
    StrCpy (fastafile, myargs[4].strvalue);

  flagEntrez = FALSE;
  if (gi > 0 || (StrLen (uidfile) > 0))
  {
    if (!EntrezInit ("mts", FALSE, &flagHaveNet))
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 102,
                 "Entrez init failed");
      ErrShow ();
      exit (1);
    }
    flagEntrez = TRUE;
  }

  fiop = NULL;

  if (gi > 0)
  {
    sep = EntrezSeqEntryGet (gi, SEQENTRY_READ_BIOSEQ);
  }
  else if (StrLen (uidfile) > 0)
  {
    if ((fiop = FileOpen (uidfile, "r")) == NULL)
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 103,
                 "Failed to open UID file: %s", uidfile);
      ErrShow ();
      exit (1);
    }
    while ((FileGets (bufin, sizeof (bufin), fiop)) != NULL)
    {
      if (bufin[0] == '>')
        continue;
      gi = atol (bufin);
      sep = EntrezSeqEntryGet (gi, SEQENTRY_READ_BIOSEQ);
      break;
    }
  }
  else
  {
    if ((fiop = FileOpen (fastafile, "r")) == NULL)
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 103,
                 "Failed to open FastA file: %s", fastafile);
      ErrShow ();
      exit (1);
    }
    sep = FastaToSeqEntry (fiop, (Boolean) myargs[5].intvalue);
  }

  if (sep == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 104,
               "No seqentry found");
    ErrShow ();
    exit (1);
  }

  flagFirstPass = TRUE;
/*  flagIsDNA = FALSE; */
  flagIsNA = FALSE;
  invprofile = NULL;
  while (sep != NULL)
  {
    gbsp->bsp = NULL;
    gbsp->gi = gi;
    GatherSeqEntry (sep, (Pointer) gbsp, GetBioseq, (Pointer) gsp);

    if (gbsp->bsp != NULL)
    {
      bsp = gbsp->bsp;
/*
 in multiple sequence sets the first entry determines
 if the set is nucleotides or peptides
*/
      if (flagFirstPass)
      {
/* just DNA? shouldn't have to (want to?) rev-cmp RNA */
/*        if (bsp->mol == Seq_mol_dna) */
        if (ISA_na (bsp->mol))
        {
          invprofile = InvertProfile (profile);
          if (myargs[1].intvalue)
            IntegerProfile (invprofile);
/*          flagIsDNA = TRUE; */
        }
        if (ISA_na (bsp->mol))
          flagIsNA = TRUE;
        flagFirstPass = FALSE;
      }

      title = FastaTitle (bsp, ">", NULL);
      if (myargs[1].intvalue)
        saph = sap = IntProfileMatchBioseq (gbsp->bsp, profile, invprofile,
                                            (Int4) cutoff, FALSE);
      else
        saph = sap = ProfileMatchBioseq (gbsp->bsp, profile, invprofile,
                                         cutoff, FALSE);
      if (sap != NULL)
        fprintf (fout, "%s\n", title);
      MemFree (title);
      URK_SeqAlignSortByStart (&sap);

      while (sap != NULL)
      {
        ssp = (StdSegPtr) sap->segs;
        slp = ssp->loc;
        start = SeqLocStart (slp);
        stop = SeqLocStop (slp);
        strand = SeqLocStrand (slp);
        if (strand != Seq_strand_minus)
          cstr = '+';
        else
          cstr = '-';
        if (myargs[7].intvalue)
        {
          if (flagIsNA)
            spp = SeqPortNew (gbsp->bsp, 0, gbsp->bsp->length-1, strand,
                              Seq_code_iupacna);
          else
            spp = SeqPortNew (gbsp->bsp, 0, gbsp->bsp->length-1, strand,
                              Seq_code_iupacaa);
          if (strand != Seq_strand_minus)
            SeqPortSeek (spp, start, SEEK_SET);
          else
            SeqPortSeek (spp, gbsp->bsp->length-stop-1, SEEK_SET);
          SeqPortRead (spp, sequence, (Int2) (stop-start+1));
          sequence[stop-start+1] = 0;
          fprintf (fout, "%8ld %8ld  %c %6.2f  %s\n",
                   (long) (start+1), (long) (stop+1),
                   cstr, (FloatHi) sap->score->value.realvalue,
                   (CharPtr) sequence);
          SeqPortFree (spp);
        }
        else
        {
          fprintf (fout, "%8ld %8ld  %c %6.2f\n",
                   (long) (start+1), (long) (stop+1),
                   cstr, (FloatHi) sap->score->value.realvalue);
        }
        fflush (fout);
        sap = sap->next;
      }

      sap = saph;
      while (sap != NULL)
      {
        saph = sap->next;
        SeqAlignFree (sap);
        sap = saph;
      }
    }
    else
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 105,
                 "No bioseq found");
      ErrShow ();
      exit (1);
    }

    SeqEntryFree (sep);

    if (StrLen (uidfile) > 0)
    {
      while ((FileGets (bufin, sizeof (bufin), fiop)) != NULL)
      {
        if (bufin[0] == '>')
          continue;
        gi = atol (bufin);
        sep = EntrezSeqEntryGet (gi, SEQENTRY_READ_BIOSEQ);
        break;
      }
    }
    else if (StrLen (fastafile) > 0)
    {
      sep = FastaToSeqEntry (fiop, (Boolean) myargs[5].intvalue);
    }
    else
    {
      sep = NULL;
    }
  }

  MemFree (sequence);
  ComProfFree (profile);
  ComProfFree (invprofile);
  if (fout != stdout)
    FileClose (fout);
  FileClose (fiop);
  if (flagEntrez)
    EntrezFini ();

  return 0;
}
