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
* File Name: epi.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.22 $
*
* File Description: epi - protein and nucleic acid low complexity
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: epi.c,v $
* Revision 6.22  1999/02/25 15:09:17  kuzio
* optimization 3
*
* Revision 6.21  1998/12/18 16:24:55  kuzio
* big GIs
*
* Revision 6.20  1998/11/16 14:34:11  kuzio
* flagBoundaryCondition
*
* Revision 6.19  1998/09/16 18:19:29  kuzio
* cvs logging
*
* ==========================================================================
*/

#include <ncbi.h>
#include <accentr.h>
#include <gather.h>
#include <tofasta.h>
#include <urkutil.h>
#include <urkepi.h>

#define TOP_ERROR 1
static char _this_module[] = "epi";
#undef  THIS_MODULE
#define THIS_MODULE _this_module
static char _this_file[] = __FILE__;
#undef  THIS_FILE
#define THIS_FILE _this_file

typedef struct gather_Bioseq
{
  Int4      gi;
  BioseqPtr bsp;
} GatherBS, PNTR GatherBSPtr;

Args myargs[] =
{
  { "protein GI", "0", "0", "9000000", TRUE,
    'g', ARG_INT, 0.0, 0, NULL},
  { "FastA file", NULL, NULL, NULL, TRUE,
    'f', ARG_STRING, 0.0, 0, NULL},
  { "epi window", "16", "4", "128", TRUE,
    'w', ARG_INT, 0.0, 0, NULL},
  { "percent cutoff", "4.0", "0.0", "100.0", TRUE,
    'c', ARG_FLOAT, 0.0, 0, NULL},
  { "minimum epi window", "6", "4", "64", TRUE,
    'W', ARG_INT, 0.0, 0, NULL},
  { "minimum epi window percent cutoff", "0.0", "0.0", "100.0", TRUE,
    'C', ARG_FLOAT, 0.0, 0, NULL},
  { "linker", "5", "0", "32", TRUE,
    'l', ARG_INT, 0.0, 0, NULL},
  { "ntuple (nucleotides only)", "3", "1", "10", TRUE,
    'n', ARG_INT, 0.0, 0, NULL},
  { "high pass filter", "FALSE", "FALSE", "TRUE", TRUE,
    'p', ARG_BOOLEAN, 0.0, 0, NULL},
  { "X-out/N-out output for blast", "FALSE", "FALSE", "TRUE", TRUE,
    'x', ARG_BOOLEAN, 0.0, 0, NULL},
  { "numeric output", "FALSE", "FALSE", "TRUE", TRUE,
    'N', ARG_BOOLEAN, 0.0, 0, NULL},
  { "optimization 0=none 1=composition 2=homopolymer 3=probability function",
    "0", "0", "3", TRUE,
    'O', ARG_INT, 0.0, 0, NULL},
  { "extra blank line between output entries", "FALSE", "FALSE", "TRUE", TRUE,
    'R', ARG_BOOLEAN, 0.0, 0, NULL},
  { "output line length", "50", "40", "160", TRUE,
    'L', ARG_INT, 0.0, 0, NULL},
  { "force FastA to nucleotide (default amino acid)",
    "FALSE", "FALSE", "TRUE", TRUE,
    'F', ARG_BOOLEAN, 0.0, 0, NULL},
  { "print raw Epi scores", "FALSE", "FALSE", "TRUE", TRUE,
    'P', ARG_BOOLEAN, 0.0, 0, NULL},
  { "Filter boundary condition hits only", "FALSE", "FALSE", "TRUE", TRUE,
    'B', ARG_BOOLEAN, 0.0, 0, NULL}
};

static Boolean GetProteinBioseq (GatherContextPtr gcp)
{
  GatherBSPtr    gbsp;
  BioseqPtr      bsp;
  Int4           gi, entrezgi;

  if (gcp == NULL)
    return FALSE;
  if ((gbsp = (GatherBSPtr  ) gcp->userdata) == NULL)
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
  Int2        argcount;
  Boolean     flagHaveNet;

  Int4        gi;
  SeqEntryPtr sep;
  EpiDatPtr   epip;
  Int4        i;
  SeqLocPtr   slp, slpn;
  SeqPortPtr  spp;
  Int4        start, stop, linelen;
  Uint1Ptr    sequence;
  Boolean     flagIsAA;

  FILE        *fiop;
  Char        fastafile[256];
  CharPtr     title;

  static GatherScope  gs;
  GatherScopePtr      gsp;
  static GatherBS     gbs;
  GatherBSPtr         gbsp;

  argcount = sizeof (myargs) / sizeof (Args);
  if (!GetArgs ("Epi", argcount, myargs))
    return 1;

  gsp = &gs;
  gbsp = &gbs;

  MemSet ((Pointer) gsp, 0, sizeof (GatherScope));
  MemSet ((Pointer) gsp->ignore, (int) (TRUE),
          (size_t) (OBJ_MAX * sizeof (Boolean)));
  gsp->ignore[OBJ_BIOSEQ] = FALSE;

  gbsp->bsp = NULL;

  if (myargs[0].intvalue == 0 && myargs[1].strvalue == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 100,
               "No gi or FastA file given :: for help :   epi -");
    ErrShow ();
    exit (1);
  }

  gi = myargs[0].intvalue;
  if (myargs[1].strvalue != NULL)
    StrCpy (fastafile, myargs[1].strvalue);
  else
    fastafile[0] = '\0';

  linelen = myargs[13].intvalue;

  if (gi > 0)
  {
    if (!EntrezInit ("Epi", FALSE, &flagHaveNet))
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 102,
                 "Entrez init failed");
      ErrShow ();
      exit (1);
    }
  }

  if (gi > 0)
  {
    sep = EntrezSeqEntryGet (gi, SEQENTRY_READ_BIOSEQ);
  }
  else
  {
    if ((fiop = FileOpen (fastafile, "r")) == NULL)
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 103,
                 "Failed to open FastA file");
      ErrShow ();
      exit (1);
    }
    sep = FastaToSeqEntry (fiop, (Boolean) myargs[14].intvalue);
  }

  if (sep == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 104,
               "No seqentry found");
    ErrShow ();
    exit (1);
  }

  while (sep != NULL)
  {
    gbsp->gi = gi;
    GatherSeqEntry (sep, (Pointer) gbsp, GetProteinBioseq,
                         (Pointer) gsp);

    if (gbsp->bsp != NULL)
    {
      epip = EpiDatNew ();
      epip->window = myargs[2].intvalue;
      epip->percentcut = myargs[3].floatvalue;
      epip->mwindow = myargs[4].intvalue;
      epip->mpercentcut = myargs[5].floatvalue;
      epip->linker = myargs[6].intvalue;
      epip->nucleotuple = myargs[7].intvalue;
      epip->method = myargs[11].intvalue;
      epip->score = PredictEpiBioseq (gbsp->bsp, 0, gbsp->bsp->length-1,
                                      epip);
      if (epip->score != NULL)
      {
        if (myargs[15].intvalue)
        {
          for (i = 0; i < gbsp->bsp->length; i++)
            printf ("%8ld %lf\n", (long) (i+1), (double) epip->score[i]);
          exit (0);
        }
        slpn = FilterEpiBioseq (epip, gbsp->bsp,
                                (Boolean) myargs[8].intvalue,
                                (Boolean) myargs[16].intvalue);
        flagIsAA = (Boolean) ISA_aa (gbsp->bsp->mol);
        if (myargs[10].intvalue == TRUE)
        {
          slp = slpn;
          while (slp != NULL)
          {
            start = SeqLocStart (slp) + 1;
            stop = SeqLocStop (slp) + 1;
            printf ("%9ld %9ld\n", (long) start, (long) stop);
            slp = slp->next;
          }
        }
        else
        {
          if (epip->sequence == NULL)
          {
            sequence = (Uint1Ptr) MemNew ((size_t) (sizeof (Uint1) *
                                                    gbsp->bsp->length+1));
            spp = SeqPortNew (gbsp->bsp, 0, gbsp->bsp->length-1, 0,
                              Seq_code_iupacna);
            SeqPortSeek (spp, 0, SEEK_SET);
            i = 0;
            while ((sequence[i] = SeqPortGetResidue (spp)) != SEQPORT_EOF)
            {
              if (('a' <= (Char) sequence[i] && (Char) sequence[i] <= 'z') ||
                  ('A' <= (Char) sequence[i] && (Char) sequence[i] <= 'Z'))
                i++;
            }
            sequence[i] = 0;
            SeqPortFree (spp);
            i = 0;
            while (sequence[i] != 0)
            {
              sequence[i] = (Uint1) TO_UPPER ((Char) sequence[i]);
              i++;
            }
            epip->sequence = sequence;
          }
          else
          {
            sequence = epip->sequence;
          }
          slp = slpn;
          while (slp != NULL)
          {
            start = SeqLocStart (slp);
            stop = SeqLocStop (slp);
            for (i = start; i <= stop; i++)
            {
              if (myargs[9].intvalue == TRUE)
              {
                if (flagIsAA)
                  sequence[i] = (Uint1) 'x';
                else
                  sequence[i] = (Uint1) 'n';
              }
              else
              {
                sequence[i] = (Uint1) TO_LOWER ((Char) sequence[i]);
              }
            }
            slp = slp->next;
          }

          title = FastaTitle (gbsp->bsp, ">", NULL);
          printf ("%s\n", title);
          MemFree (title);
          i = 0;
          while (sequence[i] != 0)
          {
            printf ("%c", (Char) sequence[i]);
            i++;
            if (i % linelen == 0)
            {
              if (myargs[9].intvalue == TRUE)
                printf ("\n");
              else
                printf (" %8ld\n", (long) i);
            }
          }
          if (i % linelen != 0)
            printf ("\n");
          if (myargs[12].intvalue)
            printf ("\n");
        }
        slp = slpn;
        while (slp != NULL)
        {
          slpn = slp->next;
          SeqLocFree (slp);
          slp = slpn;
        }
      }
      else
      {
        ErrPostEx (SEV_ERROR, TOP_ERROR, 107,
                   "No epi predictions made");
        ErrShow ();
        exit (1);
      }
      EpiDatFree (epip);
    }
    else
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 105,
                 "No bioseq found");
      ErrShow ();
      exit (1);
    }
    sep = SeqEntryFree (sep);
    if (gi <= 0)
    {
      sep = FastaToSeqEntry (fiop, FALSE);
      gbsp->bsp = NULL;
    }
  }

  if (gi > 0)
    EntrezFini ();
  else
    FileClose (fiop);

  return 0;
}
