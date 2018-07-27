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
* File Name: ccp.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.14 $
*
* File Description: coiled-coil prediction
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: ccp.c,v $
* Revision 6.14  1998/12/18 16:24:52  kuzio
* big GIs
*
* Revision 6.13  1998/11/16 14:34:09  kuzio
* flagBoundaryCondition
*
* Revision 6.12  1998/09/16 18:19:26  kuzio
* cvs logging
*
* ==========================================================================
*/

#include <ncbi.h>
#include <accentr.h>
#include <gather.h>
#include <tofasta.h>
#include <urkutil.h>
#include <urkpcc.h>

#define TOP_ERROR 1
static char _this_module[] = "ccp";
#undef  THIS_MODULE
#define THIS_MODULE _this_module
static char _this_file[] = __FILE__;
#undef  THIS_FILE
#define THIS_FILE _this_file

#define L_T     256
#define L_ID    32
#define L_DEF   220

typedef struct gather_Prot_Bioseq
{
  Int4      gi;
  BioseqPtr bsp;
} Gather_PBS, PNTR Gather_PBSPtr;

Args myargs[] =
{
  { "protein GI", "0", "0", "9000000", TRUE,
    'g', ARG_INT, 0.0, 0, NULL},
  { "FastA file", NULL, NULL, NULL, TRUE,
    'f', ARG_STRING, 0.0, 0, NULL},
  { "cc window 1", "22", "7", "42", TRUE,
    'w', ARG_INT, 0.0, 0, NULL},
  { "cc window 2", "-1", "7", "42", TRUE,
    'x', ARG_INT, 0.0, 0, NULL},
  { "cc window 3", "-1", "7", "42", TRUE,
    'y', ARG_INT, 0.0, 0, NULL},
  { "cc window 4", "-1", "7", "42", TRUE,
    'z', ARG_INT, 0.0, 0, NULL},
  { "sequence output", "FALSE", "FALSE", "TRUE", TRUE,
    'S', ARG_BOOLEAN, 0.0, 0, NULL},
  { "X-out sequence output for blast", "FALSE", "FALSE", "TRUE", TRUE,
    'X', ARG_BOOLEAN, 0.0, 0, NULL},
  { "stringent filter", "FALSE", "FALSE", "TRUE", TRUE,
    's', ARG_BOOLEAN, 0.0, 0, NULL},
  { "very stringent filter", "FALSE", "FALSE", "TRUE", TRUE,
    'v', ARG_BOOLEAN, 0.0, 0, NULL},
  { "output line length", "50", "40", "160", TRUE,
    'l', ARG_INT, 0.0, 0, NULL},
  { "data file 0=KSpcc 1=KSmtk 2=KSmtidk", "0", "0", "2", TRUE,
    'd', ARG_INT, 0.0, 0, NULL},
  { "Filter boundary condition hits only", "FALSE", "FALSE", "TRUE", TRUE,
    'B', ARG_BOOLEAN, 0.0, 0, NULL}
};

static Boolean GetProteinBioseq (GatherContextPtr gcp)
{
  Gather_PBSPtr  gpbsp;
  BioseqPtr      bsp;
  Int4           gi, entrezgi;

  if (gcp == NULL)
    return FALSE;
  if ((gpbsp = (Gather_PBSPtr) gcp->userdata) == NULL)
    return FALSE;

  if (gpbsp->bsp != NULL)
    return TRUE;
  if (gcp->thistype != OBJ_BIOSEQ)
    return TRUE;
  if ((bsp = (BioseqPtr) (gcp->thisitem)) == NULL)
    return TRUE;

  gi = gpbsp->gi;
  if (gi > 0)
  {
    entrezgi = GetGIForSeqId (bsp->id);
    if (gi == entrezgi)
      gpbsp->bsp = bsp;
    return TRUE;
  }
  else
  {
    gpbsp->bsp = bsp;
    return TRUE;
  }
}

Int2 Main (void)
{
  Int2        argcount;
  Boolean     flagHaveNet;

  Int4        gi;
  SeqEntryPtr sep;
  PccDatPtr   pccp;

  FILE        *fiop;
  Char        fastafile[256];
  CharPtr     title;
  CharPtr     datafile[3] = {"KSpcc.mat", "KSmtk.mat", "KSmtidk.mat"};

  Int4        i, iloop, iwindow, pccwindow, start, stop, linelen;
  FloatHiPtr  pccscore, pccscore1, pccscore2, pccscore3, pccscore4;
  SeqLocPtr   slp, slpn;
  Uint1Ptr    sequence;
  SeqPortPtr  spp;

  static GatherScope  gs;
  GatherScopePtr      gsp;
  static Gather_PBS   gpbs;
  Gather_PBSPtr       gpbsp;

  argcount = sizeof (myargs) / sizeof (Args);
  if (!GetArgs ("CCP", argcount, myargs))
    return 1;

  gsp = &gs;
  gpbsp = &gpbs;

  MemSet ((Pointer) gsp, 0, sizeof (GatherScope));
  MemSet ((Pointer) gsp->ignore, (int) (TRUE),
          (size_t) (OBJ_MAX * sizeof (Boolean)));
  gsp->ignore[OBJ_BIOSEQ] = FALSE;

  gpbsp->bsp = NULL;

  if (myargs[0].intvalue == 0 && myargs[1].strvalue == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 100,
               "No gi or FastA file given :: for help :   ccp -");
    ErrShow ();
    exit (1);
  }

  gi = myargs[0].intvalue;
  if (myargs[1].strvalue != NULL)
    StrCpy (fastafile, myargs[1].strvalue);
  else
    fastafile[0] = '\0';

  if (gi > 0)
  {
    if (!EntrezInit ("CCP", FALSE, &flagHaveNet))
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
    sep = FastaToSeqEntry (fiop, FALSE);
  }

  if (sep == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 104,
               "No seqentry found");
    ErrShow ();
    exit (1);
  }

  linelen = myargs[10].intvalue;
  while (sep != NULL)
  {
    gpbsp->gi = gi;
    GatherSeqEntry (sep, (Pointer) gpbsp, GetProteinBioseq,
                    (Pointer) gsp);

    if (gpbsp->bsp != NULL)
    {
      if (ISA_aa (gpbsp->bsp->mol))
      {
        title = FastaTitle (gpbsp->bsp, ">", NULL);

        iwindow = 0;
        pccscore1 = NULL;
        pccscore2 = NULL;
        pccscore3 = NULL;
        pccscore4 = NULL;
        for (i = 0; i < 4; i++)
        {
          pccwindow = 0;
          switch (i)
          {
           case 0:
            pccwindow = myargs[2].intvalue;
            break;
           case 1:
            pccwindow = myargs[3].intvalue;
            break;
           case 2:
            pccwindow = myargs[4].intvalue;
            break;
           case 3:
            pccwindow = myargs[5].intvalue;
            break;
           default:
            break;
          }
          if (pccwindow > 0)
          {
            pccp = PccDatNew ();
            pccp->window = pccwindow;
            MemFree (pccp->pccdatafile);
            pccp->pccdatafile = StringSave (datafile[myargs[11].intvalue]);
            if (ReadPccData (pccp) == 0)
            {
              ErrPostEx (SEV_ERROR, TOP_ERROR, 101,
                         "Could not open or read %s data file",
                         pccp->pccdatafile);
              ErrShow ();
              exit (1);
            }
            pccscore = PredictCCBioseq (gpbsp->bsp, 0, gpbsp->bsp->length-1,
                                        pccp);
            PccDatFree (pccp);
            if (pccscore != NULL)
            {
              iwindow++;
              switch (iwindow)
              {
               case 1:
                pccscore1 = pccscore;
                break;
               case 2:
                pccscore2 = pccscore;
                break;
               case 3:
                pccscore3 = pccscore;
                break;
               case 4:
                pccscore4 = pccscore;
                break;
               default:
                iwindow = 4;
                break;
              }
            }
          }
        }
        if (myargs[6].intvalue == FALSE)
        {
          printf ("%s\n", title);
          switch (iwindow)
          {
           case 1:
            for (iloop = 0; iloop < gpbsp->bsp->length; iloop++)
              printf ("%lf\n", (double) pccscore1[iloop]);
            break;
           case 2:
            for (iloop = 0; iloop < gpbsp->bsp->length; iloop++)
              printf ("%lf %lf\n", (double) pccscore1[iloop],
                                   (double) pccscore2[iloop]);
            break;
           case 3:
            for (iloop = 0; iloop < gpbsp->bsp->length; iloop++)
              printf ("%lf %lf %lf\n", (double) pccscore1[iloop],
                                       (double) pccscore2[iloop],
                                       (double) pccscore3[iloop]);
            break;
           case 4:
            for (iloop = 0; iloop < gpbsp->bsp->length; iloop++)
              printf ("%lf %lf %lf %lf\n", (double) pccscore1[iloop],
                                           (double) pccscore2[iloop],
                                           (double) pccscore3[iloop],
                                           (double) pccscore4[iloop]);
            break;
           default:
            ErrPostEx (SEV_ERROR, TOP_ERROR, 107,
                       "No coiled-coil predictions made");
            ErrShow ();
          }
        }
        else
        {
          sequence = (Uint1Ptr) MemNew ((size_t) (sizeof (Uint1) *
                                                  gpbsp->bsp->length+1));
          spp = SeqPortNew (gpbsp->bsp, 0, gpbsp->bsp->length-1, 0,
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

          for (iloop = 0; iloop < iwindow; iloop++)
          {
            switch (iloop)
            {
             default:
             case 0:
              pccscore = pccscore1;
              break;
             case 1:
              pccscore = pccscore2;
              break;
             case 2:
              pccscore = pccscore3;
              break;
             case 3:
              pccscore = pccscore4;
              break;
            }
            if (myargs[9].intvalue)
            {
              slpn = FilterCCVS (pccscore, 40, gpbsp->bsp->length, 32,
                                 gpbsp->bsp->id,
                                 (Boolean) myargs[12].intvalue);
            }
            else if (myargs[8].intvalue)
            {
              slpn = FilterCCVS (pccscore, 50, gpbsp->bsp->length, 24,
                                 gpbsp->bsp->id,
                                 (Boolean) myargs[12].intvalue);
            }
            else
            {
              slpn = FilterCC (pccscore, 50, gpbsp->bsp->length, 0,
                               gpbsp->bsp->id,
                               (Boolean) myargs[12].intvalue);
            }
            slp = slpn;
            while (slp != NULL)
            {
              start = SeqLocStart (slp);
              stop = SeqLocStop (slp);
              for (i = start; i <= stop; i++)
              {
                if (myargs[7].intvalue == TRUE)
                  sequence[i] = (Uint1) 'x';
                else
                  sequence[i] = (Uint1) TO_LOWER ((Char) sequence[i]);
              }
              slpn = slp->next;
              SeqLocFree (slp);
              slp = slpn;
            }
          }

          printf ("%s\n", title);
          i = 0;
          while (sequence[i] != 0)
          {
            printf ("%c", (Char) sequence[i]);
            i++;
            if (i % linelen == 0)
            {
              if (myargs[7].intvalue == TRUE)
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
        MemFree (title);
        MemFree (pccscore1);
        MemFree (pccscore2);
        MemFree (pccscore3);
        MemFree (pccscore4);
      }
      else
      {
        ErrPostEx (SEV_ERROR, TOP_ERROR, 106,
                   "Not a protein bioseq");
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
    sep = SeqEntryFree (sep);
    if (gi <= 0)
    {
      sep = FastaToSeqEntry (fiop, FALSE);
      gpbsp->bsp = NULL;
    }
  }

  if (gi > 0)
    EntrezFini ();
  else
    FileClose (fiop);

  return 0;
}
