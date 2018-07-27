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
* File Name: srchnt.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.16 $
*
* File Description: nucleotide pattern match
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: srchnt.c,v $
* Revision 6.16  1999/07/13 13:07:32  sicotte
* prefix functions SeqAlignSort* with URK to resolve toolkit conflicts
*
* Revision 6.15  1999/02/26 18:52:11  kuzio
* correct cast
*
* Revision 6.14  1998/12/18 16:24:57  kuzio
* big GIs
*
* Revision 6.13  1998/09/21 14:48:54  kuzio
* cmdln pattern option
*
* Revision 6.12  1998/09/16 18:19:31  kuzio
* cvs logging
*
* ==========================================================================
*/

/*
Patterns: explanation and examples:

N       - any nucleotide
[xyz]   - one of x y or z
(#1:#2) - in the range of

a class of enhancer sequence:

G[CT][ST][GT]T[AT]C[AG][AC]G[AT][AG]S[AG][ACT]
[CT][CT]S[CT]AC[CT]CG[AT](1:4)AGC

a type of promoter region:

[AGT]TAAG   (50:70)N     ANNATG[AG]

=========   ========     =========
a kind of   50 to 70     Kozak-like
promoter    nucleotides  initiator

a bogus example (but syntactically correct):

A[(3:5)GT]C

A
 followed by
(3 to 5 G's)  or  (a single T)
 followed by
C

using above would find:     AGGGC AGGGGGC ATCG
                            +++++ +++++++ +++

using above would not find: AGGGT ATTTC   AGGC
                            ++++! ++!     +++!

But, variable differential ranges are not supported.
Define two separate patterns.

see - ncbiren.dat for REN data
    - ncbirnam.dat for sample REN names data file
*/

#include <ncbi.h>
#include <accentr.h>
#include <gather.h>
#include <tofasta.h>
#include <urkutil.h>
#include <urkptpf.h>

#define TOP_ERROR 1
static char _this_module[] = "srchnt";
#undef  THIS_MODULE
#define THIS_MODULE _this_module
static char _this_file[] = __FILE__;
#undef  THIS_FILE
#define THIS_FILE _this_file

typedef struct gather_Nuc_Bioseq
{
  Int4      gi;
  BioseqPtr bsp;
} Gather_NBS, PNTR Gather_NBSPtr;

Args myargs[] =
{
  { "nucleotide GI", "0", "0", "9000000", TRUE,
    'g', ARG_INT, 0.0, 0, NULL},
  { "FastA file", NULL, NULL, NULL, TRUE,
    'f', ARG_STRING, 0.0, 0, NULL },
  { "local REN file", NULL, NULL, NULL, TRUE,
    'r', ARG_STRING, 0.0, 0, NULL },
  { "sort by fragment length", "FALSE", "FALSE", "TRUE", TRUE,
    's', ARG_BOOLEAN, 0.0, 0, NULL},
  { "mismatch", "0", "0", "2", TRUE,
    'm', ARG_INT, 0.0, 0, NULL},
  { "enzyme with known cutsite only", "TRUE", "FALSE", "TRUE", TRUE,
    'c', ARG_BOOLEAN, 0.0, 0, NULL},
  { "use ncbirnam.dat names file", "FALSE", "FALSE", "TRUE", TRUE,
    'n', ARG_BOOLEAN, 0.0, 0, NULL},
  { "user names file", NULL, NULL, NULL, TRUE,
    'N', ARG_STRING, 0.0, 0, NULL },
  { "user pattern", NULL, NULL, NULL, TRUE,
    'S', ARG_STRING, 0.0, 0, NULL }
};

static Boolean GetBioseq (GatherContextPtr gcp)
{
  Gather_NBSPtr  gnbsp;
  BioseqPtr      bsp;
  Int4           gi, entrezgi;

  if (gcp == NULL)
    return FALSE;
  if ((gnbsp = (Gather_NBSPtr) gcp->userdata) == NULL)
    return FALSE;

  if (gnbsp->bsp != NULL)
    return TRUE;
  if (gcp->thistype != OBJ_BIOSEQ)
    return TRUE;
  if ((bsp = (BioseqPtr) (gcp->thisitem)) == NULL)
    return TRUE;

  gi = gnbsp->gi;
  if (gi > 0)
  {
    entrezgi = GetGIForSeqId (bsp->id);
    if (gi == entrezgi)
      gnbsp->bsp = bsp;
    return TRUE;
  }
  else
  {
    gnbsp->bsp = bsp;
    return TRUE;
  }
}

Int2 Main (void)
{
  Int2        argcount;
  Boolean     flagHaveNet;

  Int4        gi;
  SeqEntryPtr sep;
  ComPatPtr   cpp, cpph = NULL;
  SeqAlignPtr sap, sapn;
  SeqLocPtr   slp, slpn, slpt;
  StdSegPtr   ssp;
  Int4        start;

  FILE        *fiop;
  Char        fastafile[256], namesfile[256];
  CharPtr     title;

  ValNodePtr  namelist;

  static CharPtr pattern_file = "ncbiren.dat";
  static CharPtr names_file = "ncbirnam.dat";

  static GatherScope  gs;
  GatherScopePtr      gsp;
  static Gather_NBS   gpbs;
  Gather_NBSPtr       gnbsp;

  argcount = sizeof (myargs) / sizeof (Args);
  if (!GetArgs ("REN_Search", argcount, myargs))
    return 1;

  if (myargs[0].intvalue == 0 && myargs[1].strvalue == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 100,
               "No gi or FastA file given :: for help :   srchnt -");
    ErrShow ();
    exit (1);
  }

  gsp = &gs;
  gnbsp = &gpbs;

  MemSet ((Pointer) gsp, 0, sizeof (GatherScope));
  MemSet ((Pointer) gsp->ignore, (int) (TRUE),
          (size_t) (OBJ_MAX * sizeof (Boolean)));

  gsp->ignore[OBJ_SEQDESC] = TRUE;
  gsp->ignore[OBJ_BIOSEQ] = FALSE;

  gnbsp->bsp = NULL;

  gi = myargs[0].intvalue;
  if (myargs[1].strvalue != NULL)
    StrCpy (fastafile, myargs[1].strvalue);
  else
    fastafile[0] = '\0';

  if (gi > 0)
  {
    if (!EntrezInit ("srchnt", FALSE, &flagHaveNet))
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
    sep = FastaToSeqEntry (fiop, TRUE);
  }

  if (sep == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 104,
               "No seqentry found");
    ErrShow ();
    exit (1);
  }

  namesfile[0] = '\0';
  if (myargs[6].intvalue)
    StrCpy (namesfile, names_file);
  if (myargs[7].strvalue != NULL)
    StrCpy (namesfile, myargs[7].strvalue);

  namelist = ReadPatternNames (namesfile);

  while (sep != NULL)
  {
    gnbsp->gi = gi;
    GatherSeqEntry (sep, (Pointer) gnbsp, GetBioseq, (Pointer) gsp);

    if (gnbsp->bsp != NULL)
    {
      if (ISA_na (gnbsp->bsp->mol))
      {
        if (cpph == NULL)
        {
          if (myargs[8].strvalue != NULL)
          {
            if ((cpph = CompilePattern (myargs[8].strvalue, 0)) != NULL)
              StrCpy (cpph->name, "User Pattern");
          }
          else
          {
            if (myargs[2].strvalue != NULL)
              cpph = ReadRENPattern (myargs[2].strvalue,
                                     (Boolean) myargs[5].intvalue, namelist);
            else
              cpph = ReadRENPattern (pattern_file,
                                     (Boolean) myargs[5].intvalue, namelist);
          }
          PalindromeCheck (cpph);
        }
        title = FastaTitle (gnbsp->bsp, ">", NULL);
        printf ("%s\n", title);
        MemFree (title);
        cpp = cpph;
        while (cpp != NULL)
        {
          printf (">%s\n", cpp->name);
          sap = PatternMatchBioseq (gnbsp->bsp, cpp,
                                    (Int4) myargs[4].intvalue);
          if (sap != NULL)
            printf ("   Start\n");
          URK_SeqAlignSortByStart (&sap);
          if (myargs[3].intvalue)
          {
            EmbedFragLengthInfo (sap, gnbsp->bsp->length);
            URK_SeqAlignSortByLength (&sap);
            while (sap != NULL)
            {
              ssp = (StdSegPtr) sap->segs;
              slp = ssp->loc;
              if (slp->choice != SEQLOC_MIX)
              {
                start = SeqLocStart (slp);
              }
              else
              {
                slpt = (SeqLocPtr) slp->data.ptrvalue;
                start = SeqLocStart (slpt);
              }
              printf ("%8ld  %8ld\n", (long) start+1,
                                      (long) ssp->scores->value.intvalue);
              sapn = sap->next;
              SeqAlignFree (sap);
              sap = sapn;
            }
          }
          else
          {
            slp = MatchSa2Sl (&sap);
            while (slp != NULL)
            {
              if (slp->choice != SEQLOC_MIX)
              {
                start = SeqLocStart (slp);
              }
              else
              {
                slpt = (SeqLocPtr) slp->data.ptrvalue;
                start = SeqLocStart (slpt);
              }
              printf ("%8ld\n", (long) start+1);
              slpn = slp->next;
              SeqLocFree (slp);
              slp = slpn;
            }
          }
          cpp = cpp->nextpattern;
        }
      }
      else
      {
        ErrPostEx (SEV_ERROR, TOP_ERROR, 106,
                   "Not a nucleic bioseq");
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
      sep = FastaToSeqEntry (fiop, TRUE);
  }

  ComPatFree (cpph);
  ValNodeFreeData (namelist);
  FileClose (fiop);
  if (gi > 0)
    EntrezFini ();

  return 0;
}
