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
* File Name: srchaa.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.15 $
*
* File Description: peptide pattern match
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: srchaa.c,v $
* Revision 6.15  1999/07/13 13:07:32  sicotte
* prefix functions SeqAlignSort* with URK to resolve toolkit conflicts
*
* Revision 6.14  1999/02/26 18:51:54  kuzio
* mismatch option
*
* Revision 6.13  1999/02/25 20:34:19  kuzio
* print title option
*
* Revision 6.12  1998/12/18 16:24:56  kuzio
* big GIs
*
* Revision 6.11  1998/09/21 14:48:54  kuzio
* cmdln pattern option
*
* Revision 6.10  1998/09/16 18:19:30  kuzio
* cvs logging
*
* ==========================================================================
*/

/*
CC   /TAXO-RANGE=ABEPV;
   -  `A' stands for archaebacteria      Archaea
   -  `B' stands for bacteriophages      Viruses
   -  `E' stands for eukaryotes          Eukaryotae
   -  `P' stands for prokaryotes         Eubacteria
   -  `V' stands for eukaryotic viruses  Viruses
*/

/*
Patterns: explanation and examples:

X       - any amino acid
[xyz]   - one of x y or z
(#1:#2) - in the range of

M (2:4)[ST] P
 a methionine
 a run of 2 to 4 serine or threonine residues
 a proline after the serine/threonine run

 YES: GRMSTSTPGLV
        ++++++

 NO:  GRMSTSTLVPG
        +++++!

(6:12)X
 defines a variable region in the range of 6 to 12 amino acid residues

see - ncbipros.dat for Prosite data
    - ncbipnam.dat for sample pattern names file
    - ncbiendo.dat for endopeptidase data
*/

#include <ncbi.h>
#include <accentr.h>
#include <gather.h>

#ifndef NO_TAX_NET
#include <taxutil.h>
#endif

#include <tofasta.h>
#include <urkutil.h>
#include <urkptpf.h>

#define TOP_ERROR 1
static char _this_module[] = "srchaa";
#undef  THIS_MODULE
#define THIS_MODULE _this_module
static char _this_file[] = __FILE__;
#undef  THIS_FILE
#define THIS_FILE _this_file

typedef struct gather_TaxId
{
  Int4   taxid;
} Gather_TaxId, PNTR Gather_TaxIdPtr;

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
    'f', ARG_STRING, 0.0, 0, NULL },
  { "use Prosite SkipFlag", "FALSE", "FALSE", "TRUE", TRUE,
    's', ARG_BOOLEAN, 0.0, 0, NULL },
  { "cut-off below  r  random score [25]", "1000000", "0", "1000000", TRUE,
    'r', ARG_INT, 0.0, 0, NULL},
#ifndef NO_TAX_NET
  { "use Prosite TaxonomyFlag", "FALSE", "FALSE", "TRUE", TRUE,
    't', ARG_BOOLEAN, 0.0, 0, NULL },
#endif
  { "protease digest information", "FALSE", "FALSE", "TRUE", TRUE,
    'p', ARG_BOOLEAN, 0.0, 0, NULL },
  { "sort digest by molecular weight", "FALSE", "FALSE", "TRUE", TRUE,
    'm', ARG_BOOLEAN, 0.0, 0, NULL },
  { "use ncbipnam.dat names file", "FALSE", "FALSE", "TRUE", TRUE,
    'n', ARG_BOOLEAN, 0.0, 0, NULL},
  { "user names file", NULL, NULL, NULL, TRUE,
    'N', ARG_STRING, 0.0, 0, NULL },
  { "user pattern", NULL, NULL, NULL, TRUE,
    'S', ARG_STRING, 0.0, 0, NULL },
  { "title on hit only", "FALSE", "FALSE", "TRUE", TRUE,
    'T', ARG_BOOLEAN, 0.0, 0, NULL},
  { "mismatch", "0", "0", "2", TRUE,
    'M', ARG_INT, 0.0, 0, NULL}
};

#ifndef NO_TAX_NET

static Boolean WhatOrg (Int4 taxid, CharPtr taxon)
{
  OrgRefPtr  orp;
  CharPtr    lineage;
  CharPtr    nodes[] = {"Archaea", "Viruses", "Eukaryotae", "Eubacteria"};
  Int4       i;
  Boolean    flagTaxHit;

/*
 orp pointers only; mem doesn't have to be free'd
*/
  if ((orp = tax1_getOrgRef (taxid, NULL, NULL, NULL)) != NULL)
  {
    if (orp->taxname != NULL)
    {
      flagTaxHit = FALSE;
      if ((lineage = get_lineage (orp->taxname)) != NULL)
      {
        for (i = 0; i < 4; i++)
        {
          if (StrStr (lineage, nodes[i]))
          {
            switch (i)
            {
             case 0:
              taxon[0] = 'A';
              flagTaxHit = TRUE;
              break;
             case 1:
              taxon[1] = 'B';
              taxon[4] = 'V';
              flagTaxHit = TRUE;
              break;
             case 2:
              taxon[2] = 'E';
              flagTaxHit = TRUE;
              break;
             case 3:
              taxon[3] = 'P';
              flagTaxHit = TRUE;
              break;
            }
          }
        }
        MemFree (lineage);
        return flagTaxHit;
      }
    }
  }
  return FALSE;
}

static Boolean GetTaxId (GatherContextPtr gcp)
{
  Gather_TaxIdPtr  gtip;
  ValNodePtr       desc;
  PdbBlockPtr      pdbp;
  BioSourcePtr     biop;
  OrgRefPtr        orp;
  Int4             taxid;
  CharPtr          source, aptr, bptr;

  if (gcp == NULL)
    return FALSE;
  if ((gtip = (Gather_TaxIdPtr) gcp->userdata) == NULL)
    return FALSE;

  if (gtip->taxid != 0)
    return TRUE;
  if (gcp->thistype != OBJ_SEQDESC)
    return TRUE;
  if ((desc = (ValNodePtr) (gcp->thisitem)) == NULL)
    return TRUE;

  while (desc != NULL)
  {
    biop = NULL;
    switch (desc->choice)
    {
     case Seq_descr_source:
      biop = (BioSourcePtr) desc->data.ptrvalue;
      break;
     case Seq_descr_pdb:
      pdbp = (PdbBlockPtr) desc->data.ptrvalue;
      source = (CharPtr) pdbp->source->data.ptrvalue;
      aptr = StrStr (source, "Organism_scientific: ");
      aptr += StrLen ("Organism_scientific: ");
      bptr = StrChr (aptr, ';');
      *bptr = '\0';
      if ((taxid = tax1_getTaxIdByName (aptr)) != 0)
      {
        gtip->taxid = taxid;
        return TRUE;
      }
      break;
     default:
      break;
    }
    if (biop != NULL)
    {
      orp = biop->org;
      if (orp != NULL)
      {
        if ((taxid = tax1_getTaxIdByOrgRef (orp)) != 0)
        {
          gtip->taxid = taxid;
          return TRUE;
        }
      }
    }
    desc = desc->next;
  }
  return FALSE;
}

#endif

static Boolean GetBioseq (GatherContextPtr gcp)
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
  ComPatPtr   cpp, cpph = NULL;
  SeqAlignPtr sap, sapn;
  StdSegPtr   ssp;
  SeqLocPtr   slp, slpn;
  Int4        start, stop;

  FILE        *fiop;
  Char        fastafile[256], namesfile[256];
  CharPtr     title;
  CharPtr     taxon;

  FloatHi     mw;
  ValNodePtr  namelist = NULL;

  static CharPtr pattern_file = "ncbipros.dat";
  static CharPtr protease_file = "ncbiendo.dat";
  static CharPtr names_file = "ncbipnam.dat";

  static GatherScope  gs;
  GatherScopePtr      gsp;
  static Gather_PBS   gpbs;
  Gather_PBSPtr       gpbsp;

#ifndef NO_TAX_NET
  Int4   i;
  static Char taxdata[8];
  static Gather_TaxId gti;
  Gather_TaxIdPtr     gtip;
#endif

#ifndef NO_TAX_NET
  Int2   ia=4, ib=5, ic=6, id=7, ie=8, ig=9, ih=10, ii=11;
#else
  Int2         ib=4, ic=5, id=6, ie=7, ig=8, ih=9,  ii=10;
#endif

  argcount = sizeof (myargs) / sizeof (Args);
  if (!GetArgs ("ProSiteSearch", argcount, myargs))
    return 1;

  if (myargs[0].intvalue == 0 && myargs[1].strvalue == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 100,
               "No gi or FastA file given :: for help :   srchaa -");
    ErrShow ();
    exit (1);
  }

  gsp = &gs;

#ifndef NO_TAX_NET
  gtip = &gti;
#endif
  gpbsp = &gpbs;

  MemSet ((Pointer) gsp, 0, sizeof (GatherScope));
  MemSet ((Pointer) gsp->ignore, (int) (TRUE),
          (size_t) (OBJ_MAX * sizeof (Boolean)));

  gsp->ignore[OBJ_SEQDESC] = TRUE;
  gsp->ignore[OBJ_BIOSEQ] = FALSE;

  gpbsp->bsp = NULL;

  gi = myargs[0].intvalue;
  if (myargs[1].strvalue != NULL)
    StrCpy (fastafile, myargs[1].strvalue);
  else
    fastafile[0] = '\0';

  if (gi > 0)
  {
    if (!EntrezInit ("srchaa", FALSE, &flagHaveNet))
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 102,
                 "Entrez init failed");
      ErrShow ();
      exit (1);
    }
  }

#ifndef NO_TAX_NET
  if (myargs[ia].intvalue)
  {
    if (!TaxArchInit ())
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 103,
                 "Taxonomy init failed");
      ErrShow ();
      exit (1);
    }
  }
#endif

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

  while (sep != NULL)
  {
    gsp->ignore[OBJ_SEQDESC] = TRUE;
    gsp->ignore[OBJ_BIOSEQ] = FALSE;
    gpbsp->bsp = NULL;
    gpbsp->gi = gi;
    GatherSeqEntry (sep, (Pointer) gpbsp, GetBioseq, (Pointer) gsp);

    taxon = NULL;
#ifndef NO_TAX_NET
    if (myargs[ia].intvalue)
    {
      for (i = 0; i < 8; i++)
        taxdata[i] = '-';
      taxon = taxdata;

      gsp->ignore[OBJ_SEQDESC] = FALSE;
      gsp->ignore[OBJ_BIOSEQ] = TRUE;

      gtip->taxid = 0;
      GatherSeqEntry (sep, (Pointer) gtip, GetTaxId, (Pointer) gsp);

      if (gtip->taxid != 0)
        WhatOrg (gtip->taxid, taxon);
      else
        taxon = NULL;
    }
#endif

    if (gpbsp->bsp != NULL)
    {
      if (ISA_aa (gpbsp->bsp->mol))
      {
        if (cpph == NULL)
        {
          namesfile[0] = '\0';
          if (myargs[id].intvalue)
            StrCpy (namesfile, names_file);
          if (myargs[ie].strvalue != NULL)
            StrCpy (namesfile, myargs[ie].strvalue);

          if (myargs[ig].strvalue != NULL)
          {
            if ((cpph = CompilePattern (myargs[ig].strvalue, 1)) != NULL)
              StrCpy (cpph->name, "User Pattern");
          }
          else
          {
            namelist = ReadPatternNames (namesfile);
            if (myargs[ib].intvalue)
              cpph = ReadPrositePattern (protease_file,
                                         (Boolean) myargs[2].intvalue,
                                         myargs[3].intvalue,
                                         taxon, NULL);
            else
              cpph = ReadPrositePattern (pattern_file,
                                         (Boolean) myargs[2].intvalue,
                                         myargs[3].intvalue,
                                         taxon, namelist);
          }
        }

        if (!(Boolean) myargs[ih].intvalue)
        {
          title = FastaTitle (gpbsp->bsp, ">", NULL);
          printf ("%s\n", title);
          MemFree (title);
        }
        cpp = cpph;
        while (cpp != NULL)
        {
          sap = PatternMatchBioseq (gpbsp->bsp, cpp,
                                    (Int4)myargs[ii].intvalue);
          if (myargs[ib].intvalue)
          {
            printf (">%s\n", cpp->name);
            if (sap != NULL)
              printf ("   Start     Stop       M.W.\n");
          }
          if (myargs[ib].intvalue)
          {
            EmbedMolecularWeightInfo (sap, gpbsp->bsp);
            if (myargs[ic].intvalue)
              URK_SeqAlignSortByMolWt (&sap);
            while (sap != NULL)
            {
              ssp = (StdSegPtr) sap->segs;
              slp = ssp->loc;
              start = SeqLocStart (slp);
              stop = SeqLocStop (slp);
              mw = ssp->scores->value.realvalue;
              printf ("%8ld %8ld    %9.2f\n",
                      (long) start+1, (long) stop+1, mw);
              sapn = sap->next;
              SeqAlignFree (sap);
              sap = sapn;
            }
          }
          else
          {
            slp = MatchSa2Sl (&sap);
            if (myargs[ih].intvalue && slp != NULL)
            {
              title = FastaTitle (gpbsp->bsp, ">", NULL);
              printf ("%s\n", title);
              MemFree (title);
            }
            while (slp != NULL)
            {
              start = SeqLocStart (slp);
              stop = SeqLocStop (slp);
              printf ("%8ld %8ld    %s\n",
                      (long) start+1, (long) stop+1, cpp->name);
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
    SeqEntryFree (sep);
    sep = NULL;
    if (fiop != NULL)
      sep = FastaToSeqEntry (fiop, FALSE);
  }

  ComPatFree (cpph);
  ValNodeFreeData (namelist);
  FileClose (fiop);
  if (gi > 0)
    EntrezFini ();
#ifndef NO_TAX_NET
  if (myargs[ia].intvalue)
    TaxArchFini ();
#endif
  return 0;
}
