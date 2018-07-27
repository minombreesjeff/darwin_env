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
* File Name: dst.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.7 $
*
* File Description: a dust utility
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: dst.c,v $
* Revision 6.7  1998/12/18 16:24:54  kuzio
* big GIs
*
* Revision 6.6  1998/09/16 18:19:28  kuzio
* cvs logging
*
*
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <accentr.h>
#include <gather.h>
#include <tofasta.h>
#include <urkutil.h>
#include <urkdust.h>

#define TOP_ERROR 1
static char _this_module[] = "dust";
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
  { "FastA file", NULL, NULL, NULL, TRUE,
    'f', ARG_STRING, 0.0, 0, NULL},
  { "nucleotide GI", "0", "0", "9000000", TRUE,
    'g', ARG_INT, 0.0, 0, NULL},
  { "dust window", "64", "16", "256", TRUE,
    'w', ARG_INT, 0.0, 0, NULL},
  { "dust cutoff", "20", "10", "100", TRUE,
    'c', ARG_INT, 0.0, 0, NULL},
  { "brute force", "FALSE", "FALSE", "TRUE", TRUE,
    'b', ARG_BOOLEAN, 0.0, 0, NULL},
  { "output line length", "50", "40", "160", TRUE,
    'l', ARG_INT, 0.0, 0, NULL},
  { "n-out for Blast", "FALSE", "FALSE", "TRUE", TRUE,
    'n', ARG_BOOLEAN, 0.0, 0, NULL},
  { "numeric output", "FALSE", "FALSE", "TRUE", TRUE,
    'N', ARG_BOOLEAN, 0.0, 0, NULL}
};

static Boolean GetNucleotideBioseq (GatherContextPtr gcp)
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

  static GatherScope  gs;
  GatherScopePtr      gsp;
  static Gather_BS    gbs;
  Gather_BSPtr        gbsp;

  Int4           i, start, stop, linelen;
  SeqLocPtr      slp, slpn;
  Uint1Ptr       sequence;
  SeqPortPtr     spp;

  DustDataPtr    ddp;
  DustRegionPtr  drp;

  argcount = sizeof (myargs) / sizeof (Args);
  if (!GetArgs ("dust", argcount, myargs))
    return 1;

  gsp = &gs;
  gbsp = &gbs;

  MemSet ((Pointer) gsp, 0, sizeof (GatherScope));
  MemSet ((Pointer) gsp->ignore, (int) (TRUE),
          (size_t) (OBJ_MAX * sizeof (Boolean)));
  gsp->ignore[OBJ_BIOSEQ] = FALSE;

  gbsp->bsp = NULL;

  if (myargs[1].intvalue == 0 && myargs[0].strvalue == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 100,
               "No gi or FastA file given :: for help :   dust -");
    ErrShow ();
    exit (1);
  }

  gi = myargs[1].intvalue;
  if (myargs[0].strvalue != NULL)
    StrCpy (fastafile, myargs[0].strvalue);
  else
    fastafile[0] = '\0';

  linelen = myargs[5].intvalue;

  if (gi > 0)
  {
    if (!EntrezInit ("dust", FALSE, &flagHaveNet))
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
    sep = FastaToSeqEntry (fiop, TRUE);
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
    GatherSeqEntry (sep, (Pointer) gbsp, GetNucleotideBioseq,
                    (Pointer) gsp);

    if (gbsp->bsp != NULL)
    {
      if (ISA_na (gbsp->bsp->mol))
      {
        ddp = DustDataNew ();
        ddp->windowsize = myargs[2].intvalue;
        ddp->level = myargs[3].intvalue;
        ddp->percent = myargs[3].floatvalue;
        ddp->flagBrute = (Boolean) myargs[4].intvalue;
        drp = DustBioseq (gbsp->bsp, 0, gbsp->bsp->length-1, ddp);
        slpn = slp = DustFilter (drp, ddp->percent, gbsp->bsp->id);
        DustDataFree (ddp);
        DustRegionFree (drp);

        title = FastaTitle (gbsp->bsp, ">", NULL);
        printf ("%s\n", title);
        MemFree (title);
        if (myargs[7].intvalue == TRUE)
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

          slp = slpn;
          while (slp != NULL)
          {
            start = SeqLocStart (slp);
            stop = SeqLocStop (slp);
            for (i = start; i <= stop; i++)
            {
              if (myargs[6].intvalue == TRUE)
                sequence[i] = (Uint1) 'n';
              else
                sequence[i] = (Uint1) TO_LOWER ((Char) sequence[i]);
            }
            slp = slp->next;
          }

          i = 0;
          while (sequence[i] != 0)
          {
            printf ("%c", (Char) sequence[i]);
            i++;
            if (i % linelen == 0)
            {
              if (myargs[6].intvalue == TRUE)
                printf ("\n");
              else
                printf (" %8ld\n", (long) i);
            }
          }
          if (i % linelen != 0)
            printf ("\n");

          SeqPortFree (spp);
          MemFree (sequence);
        }
        slp = slpn;
        while (slp != NULL)
        {
          slpn = slp->next;
          SeqLocFree (slp);
          slp = slpn;
        }
      }
    }
    sep = SeqEntryFree (sep);
    if (gi <= 0)
    {
      sep = FastaToSeqEntry (fiop, TRUE);
      gbsp->bsp = NULL;
    }
  }

  if (gi > 0)
    EntrezFini ();
  else
    FileClose (fiop);
  return 0;
}
