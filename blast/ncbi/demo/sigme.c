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
* File Name: sigme.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.13 $
*
* File Description: signal peptide and transmembrane region prediction
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: sigme.c,v $
* Revision 6.13  1998/12/18 16:24:56  kuzio
* big GIs
*
* Revision 6.12  1998/11/24 15:42:00  kuzio
* refine boundary condition for multiple potential leader pepides
*
* Revision 6.11  1998/11/16 14:34:13  kuzio
* flagBoundaryCondition
*
* Revision 6.10  1998/09/16 18:19:30  kuzio
* cvs logging
*
* ==========================================================================
*/

#include <ncbi.h>
#include <accentr.h>
#include <gather.h>
#include <tofasta.h>
#include <urkutil.h>
#include <urkfltr.h>
#include <urkptpf.h>
#include <urksigu.h>

#define TOP_ERROR 1
static char _this_module[] = "sigme";
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
  { "nucleotide GI", "0", "0", "9000000", TRUE,
    'g', ARG_INT, 0.0, 0, NULL},
  { "FastA file", NULL, NULL, NULL, TRUE,
    'f', ARG_STRING, 0.0, 0, NULL },
  { "gram negative signal peptide", "FALSE", "TRUE", "FALSE", TRUE,
    'n', ARG_BOOLEAN, 0.0, 0, NULL},
  { "gram positive signal peptide", "FALSE", "TRUE", "FALSE", TRUE,
    'p', ARG_BOOLEAN, 0.0, 0, NULL},
  { "Report boundary conditions", "FALSE", "TRUE", "FALSE", TRUE,
    'B', ARG_BOOLEAN, 0.0, 0, NULL}
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
  Int2        argcount;
  Boolean     flagHaveNet;

  Int4        gi;
  SeqEntryPtr sep;

  FILE        *fiop;
  Char        fastafile[256];
  CharPtr     title;

  ComProfPtr   leadprofile, cutprofile;
  FilterDatPtr fltp;
  ComPatPtr    cpp;

  SeqAlignPtr sap;
  SeqLocPtr   slph, slp, pslp, pslph;
  SeqPortPtr  spp;
  Int4        i, start, stop, prestart, poststop, pstart;
  Uint1Ptr    sequence;
  Boolean     flagCharge;

  FloatHi     leadcut = 3.3;
  FloatHi     cutcut = 2.1;
  Boolean     flagSPFuzz, flagBoundaryCondition;
  Int4        range = 40;
  Int4        ctermsig;

  FloatHi     cutoff = 50.0;
  Int4        window = 19;
  FloatHiPtr  fptr;

  Boolean     flagTitle;

  static GatherScope  gs;
  GatherScopePtr      gsp;
  static Gather_BS    gbs;
  Gather_BSPtr        gbsp;

  argcount = sizeof (myargs) / sizeof (Args);
  if (!GetArgs ("sigme", argcount, myargs))
    return 1;

  if (myargs[0].intvalue == 0 && myargs[1].strvalue == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 100,
               "No gi or FastA file given :: for help :   sigme -");
    ErrShow ();
    exit (1);
  }

  gsp = &gs;
  gbsp = &gbs;

  MemSet ((Pointer) gsp, 0, sizeof (GatherScope));
  MemSet ((Pointer) gsp->ignore, (int) (TRUE),
          (size_t) (OBJ_MAX * sizeof (Boolean)));

  gsp->ignore[OBJ_SEQDESC] = TRUE;
  gsp->ignore[OBJ_BIOSEQ] = FALSE;

  gi = myargs[0].intvalue;
  if (myargs[1].strvalue != NULL)
    StrCpy (fastafile, myargs[1].strvalue);
  else
    fastafile[0] = '\0';

  if (gi > 0)
  {
    if (!EntrezInit ("sigme", FALSE, &flagHaveNet))
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 102,
                 "Entrez init failed");
      ErrShow ();
      exit (1);
    }
  }

  fiop = NULL;
  if (gi > 0)
  {
    sep = EntrezSeqEntryGet (gi, SEQENTRY_READ_BIOSEQ);
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
    sep = FastaToSeqEntry (fiop, FALSE);
  }
  if (sep == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 104,
               "No seqentry found");
    ErrShow ();
    exit (1);
  }

  if (myargs[2].intvalue)
  {
    leadprofile = ReadProfile ("KSnsigl.mat");
    cutprofile = ReadProfile ("KSnsigc.mat");
  }
  else if (myargs[3].intvalue)
  {
    leadprofile = ReadProfile ("KSpsigl.mat");
    cutprofile = ReadProfile ("KSpsigc.mat");
  }
  else
  {
    leadprofile = ReadProfile ("KSesigl.mat");
    cutprofile = ReadProfile ("KSesigc.mat");
  }
  flagBoundaryCondition = (Boolean) myargs[4].intvalue;

  while (sep != NULL)
  {
    gbsp->bsp = NULL;
    gbsp->gi = gi;
    GatherSeqEntry (sep, (Pointer) gbsp, GetBioseq, (Pointer) gsp);

    if (gbsp->bsp != NULL)
    {
      if (ISA_aa (gbsp->bsp->mol))
      {
        title = FastaTitle (gbsp->bsp, ">", NULL);
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
        i = 0;
        while (sequence[i] != 0)
        {
          sequence[i] = (Uint1) TO_UPPER ((Char) sequence[i]);
          i++;
        }
        SeqPortFree (spp);

        flagTitle = FALSE;
        slph = slp = FilterSigSeq (gbsp->bsp, leadprofile, cutprofile,
                                   leadcut, cutcut, range,
                                   gbsp->bsp->id, FALSE, FALSE);
        ctermsig = EndOfSig (slp);
        if (slp != NULL)
        {
          flagTitle = TRUE;
          printf ("%s\n", title);
          printf (" Signal Sequence\n");
        }
        while (slp != NULL)
        {
          start = SeqLocStart (slp);
          stop = SeqLocStop (slp);
          printf ("%4ld %4ld   ", (long) (start+1), (long) (stop+1));
          for (i = start; i <= stop; i++)
            printf ("%c", (Char) sequence[i]);
          printf ("\n");
          slp = slp->next;
        }
        slp = slph;
        while (slp != NULL)
        {
          slph = slp->next;
          SeqLocFree (slp);
          slp = slph;
        }
/* determine fuzzy signal if requested */
        if (flagBoundaryCondition)
        {
          slp = FilterSigSeq (gbsp->bsp, leadprofile, cutprofile,
                              leadcut, cutcut, range,
                              gbsp->bsp->id, TRUE, TRUE);
          flagSPFuzz = FALSE;
          if (slp != NULL)
          {
            flagSPFuzz = TRUE;
            while (slp != NULL)
            {
              slph = slp->next;
              SeqLocFree (slp);
              slp = slph;
            }
          }
          if (flagSPFuzz)
          {
            if (flagTitle == FALSE)
            {
              flagTitle = TRUE;
              printf ("%s\n", title);
              printf (" Signal Sequence\n");
            }
            printf (" signal peptide prediction is fuzzy\n");
          }
        }

        fltp = FilterDatNew (AA_FILTER_COMP_KYTE, window);
        fltp->filterdatafile = StringSave ("KSkyte.flt");

        if (ReadFilterData (fltp) != 24)
        {
          printf ("Error: could not read filter data\n");
          exit (1);
        }
        fptr = FilterBioseq (gbsp->bsp, 0, gbsp->bsp->length-1, fltp);
        slp = FilterFilter (fptr, fltp->maxscr, window, cutoff,
                            gbsp->bsp->length, gbsp->bsp->id,
                            TRUE, FALSE);
        MemFree (fptr);
        slph = slp = CheckOverlap (slp, ctermsig);
        if (slp != NULL && !flagTitle)
        {
          printf ("%s\n", title);
        }
        MemFree (title);
        if (slp != NULL)
        {
          printf (" Transmembrane Sequence\n");
        }
        while (slp != NULL)
        {
          start = SeqLocStart (slp);
          stop = SeqLocStop (slp);
          printf ("%4ld %4ld   ", (long) (start+1), (long) (stop+1));
          prestart = start - 5;
          if (prestart < 0)
            prestart = 0;
          poststop = stop + 5;
          if (poststop > gbsp->bsp->length - 1)
            poststop = gbsp->bsp->length - 1;
          for (i = prestart; i < start; i++)
            printf ("%c",  TO_LOWER ((Char) sequence[i]));
          for (i = start; i <= stop; i++)
            printf ("%c", (Char) sequence[i]);
          for (i = stop + 1; i <= poststop; i++)
            printf ("%c",  TO_LOWER ((Char) sequence[i]));
          printf ("\n");
          cpp = CompilePattern ("[C,D,E,K,N,Q,H,R]", 1);
          if (cpp != NULL)
          {
            sap = PatternMatchBioseq (gbsp->bsp, cpp, 0);
            pslph = MatchSa2Sl (&sap);
            printf ("            ");
            for (i = prestart; i <= poststop; i++)
            {
              pslp = pslph;
              flagCharge = FALSE;
              while (pslp != NULL)
              {
                pstart = SeqLocStart (pslp);
                if (pstart == i)
                {
                  flagCharge = TRUE;
                  break;
                }
                pslp = pslp->next;
              }
              if (flagCharge)
                printf ("%c", (Char) sequence[i]);
              else
                printf ("-");
            }
            printf ("\n");
            ComPatFree (cpp);
            pslp = pslph;
            while (pslp != NULL)
            {
              pslph = pslp->next;
              SeqLocFree (pslp);
              pslp = pslph;
            }
          }
          slp = slp->next;
        }
        slp = slph;
        while (slp != NULL)
        {
          slph = slp->next;
          SeqLocFree (slp);
          slp = slph;
        }
        FilterDatFree (fltp);
        MemFree (sequence);
      }
      else
      {
        ErrPostEx (SEV_ERROR, TOP_ERROR, 106,
                   "Not a peptide bioseq");
        ErrShow ();
        exit (1);
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
    sep = NULL;
    if (fiop != NULL)
      sep = FastaToSeqEntry (fiop, FALSE);
  }

  ComProfFree (leadprofile);
  ComProfFree (cutprofile);
  FileClose (fiop);
  if (gi > 0)
    EntrezFini ();

  return 0;
}
