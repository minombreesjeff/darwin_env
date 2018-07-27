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
* File Name: twop.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.5 $
*
* File Description: best id blast of two proteins
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: twop.c,v $
* Revision 6.5  1998/09/16 18:19:32  kuzio
* cvs logging
*
* ==========================================================================
*/

/*
  BLAST pair score for longest gapped overlap
*/

#include <ncbi.h>
#include <accentr.h>
#include <tofasta.h>
#include <gather.h>
#include <blast.h>
#include <urkutil.h>
#include <urkptpf.h>

#define TOP_ERROR 1
static char _this_module[] = "twop";
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
  { "FastA protein target file (search)", NULL, NULL, NULL, TRUE,
    'F', ARG_STRING, 0.0, 0, NULL },
  { "FastA protein probe file (query)", NULL, NULL, NULL, TRUE,
    'f', ARG_STRING, 0.0, 0, NULL },
  { "List of protein gi's target file (search)", NULL, NULL, NULL, TRUE,
    'L', ARG_STRING, 0.0, 0, NULL },
  { "List of protein gi's probe file (query)", NULL, NULL, NULL, TRUE,
    'l', ARG_STRING, 0.0, 0, NULL },
  { "Output file", NULL, NULL, NULL, TRUE,
    'o', ARG_STRING, 0.0, 0, NULL },
  { "Show pattern match results", "FALSE", "FALSE", "TRUE", TRUE,
    'p', ARG_BOOLEAN, 0.0, 0, NULL }
};

static Boolean GetBioseq (GatherContextPtr gcp)
{
  Gather_BSPtr   gbsp;
  BioseqPtr      bsp;
  Int4           entrezgi;

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

  if (gbsp->gi > 0)
  {
    entrezgi = GetGIForSeqId (bsp->id);
    if (gbsp->gi == entrezgi)
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
  Int2           argcount;
  Boolean        flagHaveNet;

  FILE           *fioT, *fioP, *fout;
  Boolean        flagFastaT, flagFastaP, flagFastaL, flagFastal;
  Char           bufuid[256];
  Int4           giT, giP;

  SeqEntryPtr    sepT, sepP;
  BioseqPtr      bspT, bspP;
  SeqLocPtr      slpT, slpP;

  BLAST_OptionsBlkPtr blstopt;

  Int4           i, loop;
  Int4           startT, startP;
  SeqAlignPtr    sap, sapn;
  DenseSegPtr    dsp;
  SeqPortPtr     sppT, sppP;
  Uint1          seqT, seqP;
  Int4           effective_length, eff_len_max, seq_identity, seq_id_max;
  CharPtr        titleT, titleP;
  CharPtr        cptr;

  Boolean        flagPattern;
  CharPtr        pattern_file = "ncbipros.dat";
  ComPatPtr      cpp, cpph;
  SeqAlignPtr    sapT, sapP;

  static GatherScope  gs;
  GatherScopePtr      gsp;
  static Gather_BS    gbs;
  Gather_BSPtr        gbsp;

  argcount = sizeof (myargs) / sizeof (Args);
  if (!GetArgs ("twop", argcount, myargs))
    return 1;

  ErrSetLogLevel (SEV_ERROR);
  ErrSetMessageLevel (SEV_ERROR);

  gsp = &gs;
  MemSet ((Pointer) gsp, 0, sizeof (GatherScope));
  MemSet ((Pointer) gsp->ignore, (int) (TRUE),
          (size_t) (OBJ_MAX * sizeof (Boolean)));
  gsp->ignore[OBJ_BIOSEQ] = FALSE;
  gbsp = &gbs;

  flagPattern = (Boolean) myargs[5].intvalue;

  if (myargs[0].strvalue != NULL)
  {
    flagFastaT = TRUE;
    flagFastaL = FALSE;
  }
  else if (myargs[2].strvalue != NULL)
  {
    flagFastaT = FALSE;
    flagFastaL = TRUE;
  }
  else
  {
    flagFastaT = FALSE;
    flagFastaL = FALSE;
  }
  if (myargs[1].strvalue != NULL)
  {
    flagFastaP = TRUE;
    flagFastal = FALSE;
  }
  else if (myargs[3].strvalue != NULL)
  {
    flagFastaP = FALSE;
    flagFastal = TRUE;
  }
  else
  {
    flagFastaP = FALSE;
    flagFastal = FALSE;
  }

  if (!flagFastaT && !flagFastaL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 100,
               "No target file given :: for help :   twop -");
    ErrShow ();
    exit (1);
  }
  if (!flagFastaP && !flagFastal)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 100,
               "No probe file given :: for help :   twop -");
    ErrShow ();
    exit (1);
  }

  if (flagFastaL || flagFastal)
  {
    if (!EntrezInit ("twop", FALSE, &flagHaveNet))
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 102,
                 "Entrez init failed");
      ErrShow ();
      exit (1);
    }
  }

  giT = 0;
  giP = 0;

  if (flagFastaT)
  {
    if ((fioT = FileOpen (myargs[0].strvalue, "r")) == NULL)
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 102,
                 "Failed to open FastA file %s", myargs[0].strvalue);
      ErrShow ();
      exit (1);
    }
    sepT = FastaToSeqEntry (fioT, FALSE);
  }

  if (flagFastaP)
  {
    if ((fioP = FileOpen (myargs[1].strvalue, "r")) == NULL)
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 102,
                 "Failed to open FastA file %s", myargs[1].strvalue);
      ErrShow ();
      exit (1);
    }
    sepP = FastaToSeqEntry (fioP, FALSE);
  }

  if (flagFastaL)
  {
    if ((fioT = FileOpen (myargs[2].strvalue, "r")) == NULL)
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 102,
                 "Failed to open gi list file %s", myargs[2].strvalue);
      ErrShow ();
      exit (1);
    }
    while ((FileGets (bufuid, sizeof (bufuid), fioT)) != NULL)
    {
      if (bufuid[0] == '>')
        continue;
      break;
    }
    giT = atol (bufuid);
    sepT = EntrezSeqEntryGet (giT, SEQENTRY_READ_NUC_PROT);
  }

  if (flagFastal)
  {
    if ((fioP = FileOpen (myargs[3].strvalue, "r")) == NULL)
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 102,
                 "Failed to open gi list file %s", myargs[3].strvalue);
      ErrShow ();
      exit (1);
    }
    while ((FileGets (bufuid, sizeof (bufuid), fioP)) != NULL)
    {
      if (bufuid[0] == '>')
        continue;
      break;
    }
    giP = atol (bufuid);
    sepP = EntrezSeqEntryGet (giP, SEQENTRY_READ_NUC_PROT);
  }

  if (sepT == NULL || sepP == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 103,
               "Seqentry for target or probe not found");
    ErrShow ();
    exit (1);
  }

  if (myargs[4].strvalue != NULL)
  {
    if ((fout = FileOpen (myargs[4].strvalue, "w")) == NULL)
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 102,
                 "Failed to open output file %s", myargs[4].strvalue);
      ErrShow ();
      exit (1);
    }
  }
  else
  {
    fout = stdout;
  }

  if (flagPattern)
    cpph = ReadPrositePattern (pattern_file, FALSE, 25, NULL, NULL);
  else
    cpph = NULL;

  blstopt = BLASTOptionNew ("blastp", TRUE);
  blstopt->wordsize = 3;
  blstopt->expect_value = 10.0;
  blstopt->filter = 0;
  blstopt->gap_open = 9;
  blstopt->gap_extend = 2;
  blstopt->gap_x_dropoff = 50;
  blstopt->threshold_second = 9;
  blstopt->two_pass_method = FALSE;
  blstopt->multiple_hits_only = FALSE;

  while (sepT != NULL && sepP != NULL)
  {
    gbsp->gi = giT;
    gbsp->bsp = NULL;
    GatherSeqEntry (sepT, (Pointer) gbsp, GetBioseq, (Pointer) gsp);
    bspT = gbsp->bsp;
    gbsp->gi = giP;
    gbsp->bsp = NULL;
    GatherSeqEntry (sepP, (Pointer) gbsp, GetBioseq, (Pointer) gsp);
    bspP = gbsp->bsp;
    if (bspT == NULL || bspP == NULL)
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 104,
                 "Bioseq for target or probe not found");
      ErrShow ();
      exit (1);
    }
    slpT = SeqLocIntNew (0, bspT->length-1, Seq_strand_unknown, bspT->id);
    slpP = SeqLocIntNew (0, bspP->length-1, Seq_strand_unknown, bspP->id);

    sap = BlastTwoSequencesByLoc (slpT, slpP, "blastp", blstopt);
    seq_id_max = 0;
    eff_len_max = 0;
    while (sap != NULL)
    {
      effective_length = 0;
      seq_identity = 0;
      if (sap->segtype == 2)   /* DenseSeg gapped search */
      {
        sppT = SeqPortNew (bspT, 0, bspT->length-1, 0, Seq_code_iupacaa);
        sppP = SeqPortNew (bspP, 0, bspP->length-1, 0, Seq_code_iupacaa);
        dsp = sap->segs;
        for (loop = 0; loop < (Int4) dsp->numseg; loop++)
        {
          if (dsp->starts[loop*2] == -1 || dsp->starts[(loop*2)+1] == -1)
          {
            effective_length += dsp->lens[loop];
          }
          else
          {
            effective_length += dsp->lens[loop];
            startT = dsp->starts[loop*2];
            startP = dsp->starts[(loop*2)+1];
            SeqPortSeek (sppT, startT, SEEK_SET);
            SeqPortSeek (sppP, startP, SEEK_SET);
            for (i = 0; i < dsp->lens[loop]; i++)
            {
              seqT = SeqPortGetResidue (sppT);
              seqP = SeqPortGetResidue (sppP);
              if (seqT == seqP)
                seq_identity++;
            }
          }
        }
        SeqPortFree (sppT);
        SeqPortFree (sppP);
      }
      if (effective_length > eff_len_max)
      {
        seq_id_max = seq_identity;
        eff_len_max = effective_length;
      }
      sapn = sap->next;
      SeqAlignFree (sap);
      sap = sapn;
    }

    titleT = FastaTitle (bspT, ">", NULL);
    titleP = FastaTitle (bspP, ">", NULL);

    cptr = titleT;
    while (*cptr != '\0')
    {
      if (*cptr == ' ')
        break;
      cptr++;
    }
    *cptr = '\0';
    cptr = titleP;
    while (*cptr != '\0')
    {
      if (*cptr == ' ')
        break;
      cptr++;
    }
    *cptr = '\0';

    if (eff_len_max > 0)
    {
      fprintf (fout,
 "%16s %4ld %16s %4ld -- %6.2f identity %4ld residues\n",
 titleT, (long) bspT->length, titleP, (long) bspP->length,
 (FloatHi) seq_id_max / (FloatHi) eff_len_max * 100.0,
 (long) eff_len_max);
    }
    else
    {
      fprintf (fout,
 "%16s %4ld %16s %4ld -- %6.2f identity\n",
 titleT, (long) bspT->length, titleP, (long) bspP->length, 0.0);
    }
    cpp = cpph;
    while (cpp != NULL)
    {
      sapT = PatternMatchBioseq (bspT, cpp, 0);
      sapP = PatternMatchBioseq (bspP, cpp, 0);
      if (sapT != NULL && sapP != NULL)
      {
        fprintf (fout, "target and probe:  %s\n", cpp->name);
      }
      sap = sapT;
      while (sap != NULL)
      {
        sapn = sap->next;
        SeqAlignFree (sap);
        sap = sapn;
      }
      sap = sapP;
      while (sap != NULL)
      {
        sapn = sap->next;
        SeqAlignFree (sap);
        sap = sapn;
      }
      cpp = cpp->nextpattern;
    }
    SeqLocFree (slpT);
    SeqLocFree (slpP);
    SeqEntryFree (sepT);
    SeqEntryFree (sepP);
    MemFree (titleT);
    MemFree (titleP);
    if (flagFastaT)
    {
      sepT = FastaToSeqEntry (fioT, FALSE);
    }
    if (flagFastaP)
    {
      sepP = FastaToSeqEntry (fioP, FALSE);
    }
    if (flagFastaL)
    {
      bufuid[0] = '\0';
      FileGets (bufuid, sizeof (bufuid), fioT);
      giT = atol (bufuid);
      sepT = EntrezSeqEntryGet (giT, SEQENTRY_READ_NUC_PROT);
    }
    if (flagFastal)
    {
      bufuid[0] = '\0';
      FileGets (bufuid, sizeof (bufuid), fioP);
      giP = atol (bufuid);
      sepP = EntrezSeqEntryGet (giP, SEQENTRY_READ_NUC_PROT);
    }
  }

  if (sepT != NULL)
    SeqEntryFree (sepT);
  if (sepP != NULL)
    SeqEntryFree (sepP);
  FileClose (fioT);
  FileClose (fioP);
  if (flagFastaL || flagFastal)
    EntrezFini ();
  if (fout != stdout)
    FileClose (fout);
  BLASTOptionDelete (blstopt);
  if (flagPattern)
    ComPatFree (cpph);

  return 0;
}
