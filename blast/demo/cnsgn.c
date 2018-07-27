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
* File Name: cnsgn.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.9 $
*
* File Description: consign - codon biased orf selection
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: cnsgn.c,v $
* Revision 6.9  1998/12/18 16:24:53  kuzio
* big GIs
*
* Revision 6.8  1998/11/16 14:34:10  kuzio
* flagBoundaryCondition
*
* Revision 6.7  1998/10/13 17:15:13  kuzio
* preliminary Markov
*
* Revision 6.6  1998/09/28 16:42:18  kuzio
* no met orf check
*
* Revision 6.5  1998/09/16 18:19:27  kuzio
* cvs logging
*
* ==========================================================================
*/

#include <ncbi.h>
#include <accentr.h>
#include <gather.h>
#include <tofasta.h>

#include <toasn3.h>     /* needed by tfuns.h */
#include <tfuns.h>

#ifndef NO_BLS_NET
#include <blast.h>
#include <netblap3.h>
#endif

#include <urkptpf.h>
#include <urkepi.h>
#include <urkpcc.h>
#include <urkcnsrt.h>
#include <urkbias.h>

#define TOP_ERROR 1
static char _this_module[] = "cnsgn";
#undef  THIS_MODULE
#define THIS_MODULE _this_module
static char _this_file[] = __FILE__;
#undef  THIS_FILE
#define THIS_FILE _this_file

Uint1 AAForCodon (Uint1Ptr cdn, CharPtr gcode);

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
  { "percent stdev shift", "50.0", "0.0", "100.0", TRUE,
    'd', ARG_FLOAT, 0.0, 0, NULL},
  { "minimum cluster for global usage", "2", "1", "4096", TRUE,
    'c', ARG_INT, 0.0, 0, NULL},
  { "minimum similar biased hits", "4", "1", "4096", TRUE,
    'h', ARG_INT, 0.0, 0, NULL},
#ifndef NO_BLS_NET
  { "cut off top  n  blast hits", "0", "0", "32", TRUE,
    'n', ARG_INT, 0.0, 0, NULL},
  { "seed with Blast not ProSite search", "FALSE", "TRUE", "FALSE", TRUE,
    'b', ARG_BOOLEAN, 0.0, 0, NULL},
#endif
  { "post-bias ProSite pattern match", "FALSE", "TRUE", "FALSE", TRUE,
    'p', ARG_BOOLEAN, 0.0, 0, NULL},
  { "keep internal ORFs", "FALSE", "TRUE", "FALSE", TRUE,
    'i', ARG_BOOLEAN, 0.0, 0, NULL},
  { "minimum nt_orf override", "1800", "1", "3000", TRUE,
    'o', ARG_INT, 0.0, 0, NULL},
  { "codon usage scan window", "360", "150", "3000", TRUE,
    'w', ARG_INT, 0.0, 0, NULL},
  { "codon usage scan score cutoff", "1.8", "0.0", "10.0", TRUE,
    's', ARG_FLOAT, 0.0, 0, NULL},
  { "show Markov statistics for seed ORFs (not used for selection yet)",
    "FALSE", "TRUE", "FALSE", TRUE,
    'm', ARG_BOOLEAN, 0.0, 0, NULL}
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

static void AddBioSourceToSeqEntry (SeqEntryPtr sep)
{
  BioseqPtr    bsp;
  ValNodePtr   descorp;
  BioSourcePtr biop;
  OrgNamePtr   onp;
  OrgRefPtr    orp;

  if (sep == NULL)
    return;
  if ((bsp = sep->data.ptrvalue) == NULL)
    return;

  descorp = ValNodeNew (bsp->descr);
  descorp->choice = Seq_descr_source;
  biop = BioSourceNew ();
  descorp->data.ptrvalue = biop;
  orp = OrgRefNew ();
  biop->org = orp;
  onp = OrgNameNew ();
  orp->orgname = onp;
  biop->genome = 1;
  onp->gcode = 1;
  onp->mgcode = 1;

  if (bsp->descr == NULL)
    bsp->descr = descorp;

  return;
}

static SeqLocPtr SeqLocSave (SeqLocPtr slp, SeqAlignPtr sap, Int4 skiptop)
{
  Int4         loopcount = 0;
  SeqLocPtr    slpt = NULL;

  while (sap != NULL)
  {
    loopcount++;
    if (loopcount > skiptop)
      break;
    sap = sap->next;
  }
  if (sap != NULL)
    slpt = SeqLocDup (slp);
  return slpt;
}

static void NameTagOrfs (SeqLocPtr slp)
{
  SeqIdPtr     id, sip;
  TextSeqIdPtr tsip;
  Char         name[256];
  Int4         i;

  i = 0;
  while (slp != NULL)
  {
    i++;
    sprintf (name, "ORF__%ld", (long) i);
    tsip = TextSeqIdNew ();
    tsip->name = StringSave (name);
    sip = ValNodeNew (NULL);
    sip->choice = SEQID_OTHER;
    sip->data.ptrvalue = (Pointer) tsip;
    id = SeqLocId (slp);
    ValNodeLink (&id, sip);
    slp = slp->next;
  }
  return;
}

static void TransferNameTags (SeqLocPtr slptarget, SeqLocPtr slpsource)
{
  SeqLocPtr    slp;
  Int4         tstart, tstop, sstart, sstop;
  SeqIdPtr     sip, id;
  TextSeqIdPtr tsip;
  Char         name[256];

  while (slptarget != NULL)
  {
    tstart = SeqLocStart (slptarget);
    tstop = SeqLocStop (slptarget);
    slp = slpsource;
    while (slp != NULL)
    {
      sstart = SeqLocStart (slp);
      sstop = SeqLocStop (slp);
      if (tstart == sstart && tstop == sstop)
        break;
      slp = slp->next;
    }
    if (slp == NULL)
    {
      StrCpy (name, "ORF__?");
    }
    else
    {
      sip = SeqLocId (slp);
      if (sip != NULL)
      {
        while (sip->next != NULL)
          sip = sip->next;
        if (sip->choice == SEQID_OTHER)
        {
          tsip = (TextSeqIdPtr) sip->data.ptrvalue;
          if (tsip->name != NULL)
            StrCpy (name, tsip->name);
          else
            StrCpy (name, "ORF__?");
        }
        else
        {
          StrCpy (name, "ORF__?");
        }
      }
      else
      {
        StrCpy (name, "ORF__?");
      }
    }
    tsip = TextSeqIdNew ();
    tsip->name = StringSave (name);
    sip = ValNodeNew (NULL);
    sip->choice = SEQID_OTHER;
    sip->data.ptrvalue = (Pointer) tsip;
    id = SeqLocId (slptarget);
    ValNodeLink (&id, sip);
    slptarget = slptarget->next;
  }
  return;
}

static void ShowAllFoundOrfs (SeqLocPtr slp)
{
  Int4         start, stop;
  SeqLocPtr    slpt;
  SeqIdPtr     sip;
  TextSeqIdPtr tsip;
  Char         name[256];

  while (slp != NULL)
  {
    if (slp->choice != SEQLOC_MIX)
    {
      start = SeqLocStart (slp);
      stop = SeqLocStop (slp);
    }
    else
    {
      slpt = (SeqLocPtr) slp->data.ptrvalue;
      start = SeqLocStart (slpt);
      stop = SeqLocStop (slpt);
    }
    sip = SeqLocId (slp);
    if (sip != NULL)
    {
      while (sip->next != NULL)
        sip = sip->next;
      if (sip->choice == SEQID_OTHER)
      {
        tsip = (TextSeqIdPtr) sip->data.ptrvalue;
        if (tsip->name != NULL)
          StrCpy (name, tsip->name);
        else
          StrCpy (name, "ORF__?");
      }
      else
      {
        StrCpy (name, "ORF__?");
      }
    }
    else
    {
      StrCpy (name, "ORF__?");
    }
    printf ("%-10s %8ld %8ld\n",
            name, (long) start+1, (long) stop+1);
    slp = slp->next;
  }
  return;
}

static Boolean ShowKnownFoundOrfs (GatherContextPtr gcp)
{
  Gather_CDSPtr gcdsp;
  SeqAnnotPtr   sap;
  SeqFeatPtr    sfp;
  SeqLocPtr     slp, slpt;
  BioseqPtr     bsp;

  GBQualPtr     gbqp;
  ValNodePtr    vnp;
  DbtagPtr      dtp;
  ObjectIdPtr   oip;
  Char          buf[64];
  Int4          start, stop, stt, stp;

  SeqIdPtr      sip;
  TextSeqIdPtr  tsip;
  Char          name[256];

  if (gcp == NULL)
    return FALSE;
  if ((gcdsp = (Gather_CDSPtr) gcp->userdata) == NULL)
    return FALSE;
  if (gcp->thistype != OBJ_SEQANNOT)
    return TRUE;
  if ((sap = (SeqAnnotPtr) (gcp->thisitem)) == NULL)
    return TRUE;

  while (sap != NULL)
  {
    sfp = (SeqFeatPtr) sap->data;
    while (sfp != NULL)
    {
      if (sfp->data.choice == SEQFEAT_CDREGION)
      {
        if (sfp->location->choice == SEQLOC_INT)
        {
          if ((bsp = BioseqLockById (SeqLocId (sfp->location))) != NULL)
          {
            if (SeqLocMatch (gcdsp->slpFound, sfp->location))
            {
              slp = sfp->location;
              if (slp->choice != SEQLOC_MIX)
              {
                start = SeqLocStart (slp);
                stop = SeqLocStop (slp);
              }
              else
              {
                slpt = (SeqLocPtr) slp->data.ptrvalue;
                start = SeqLocStart (slpt);
                stop = SeqLocStop (slpt);
              }
              buf[0] = '\0';
              if (sfp->qual != NULL)
              {
                gbqp = sfp->qual;
                if (gbqp->val != NULL)
                {
                  StrNCpy (buf, gbqp->val, 48);
                  buf[48] = '\0';
                }
              }
              else if (sfp->comment != NULL)
              {
                StrNCpy (buf, sfp->comment, 48);
                buf[48] = '\0';
              }
              else if (sfp->dbxref != NULL)
              {
                vnp = sfp->dbxref;
                if (vnp != NULL)
                {
                  dtp = vnp->data.ptrvalue;
                  if (dtp != NULL)
                  {
                    oip = dtp->tag;
                    if (oip != NULL)
                    {
                      StrNCpy (buf, oip->str, 48);
                      buf[48] = '\0';
                    }
                  }
                }
              }
              else
              {
                sprintf (buf, "ORF");
              }
              slp = gcdsp->slpFound;
              while (slp != NULL)
              {
                stt = SeqLocStart (slp);
                stp = SeqLocStop (slp);
                if (stt == start && stp == stop)
                {
                  sip = SeqLocId (slp);
                  if (sip != NULL)
                  {
                    while (sip->next != NULL)
                      sip = sip->next;
                    if (sip->choice == SEQID_OTHER)
                    {
                      tsip = (TextSeqIdPtr) sip->data.ptrvalue;
                      if (tsip->name != NULL)
                        StrCpy (name, tsip->name);
                      else
                        StrCpy (name, "ORF__?");
                    }
                    else
                    {
                      StrCpy (name, "ORF__?");
                    }
                  }
                  else
                  {
                    StrCpy (name, "ORF__?");
                  }
                  break;
                }
                slp = slp->next;
              }
              if (slp == NULL)
                StrCpy (name, "ORF__?");
              printf ("%-10s %-48s %8ld %8ld\n",
                      name, buf, (long) start+1, (long) stop+1);
            }
            BioseqUnlock (bsp);
          }
        }
      }
      sfp = sfp->next;
    }
    sap = sap->next;
  }
  return TRUE;
}

Int2 Main (void)
{
  Int2        argcount;
  Boolean     flagHaveNet;

  Int4           gi;
  FILE           *fiop;
  Char           fastafile[256];
  SeqEntryPtr    sep;
  SeqLocPtr      slp, slpn, slpg, slpp, slpa;
  SeqAlignPtr    sap, sapn;

  static GatherScope  gs;
  GatherScopePtr      gsp;
  static Gather_BS    gbs;
  Gather_BSPtr        gbsp;
  Gather_CDSPtr       gcdsp;

#ifndef NO_BLS_NET
  static CharPtr blast_program = "blastp";
  static CharPtr blast_database = "nr";
  BlastNet3Hptr        bl3hp;
  BlastResponsePtr     bl3rp;
  BLAST_OptionsBlkPtr  bl3op;
  ValNodePtr           vnp1, vnp2;
  Boolean              flagGap;
  SeqLocPtr            slpepi;
#endif

  static CharPtr pattern_file = "ncbipros.dat";
  ComPatPtr      cpp, cpph, cp, cph;

  BioseqPtr      bspnt;
  ValNodePtr     orfhead, orflist;
#ifndef NO_BLS_NET
  Int4           nt_blast;
  Int4           skiptop;
#endif
  Int4           nt_pattern, nt_postpattern, nt_orf;
  Int4           start, stop;
  Uint1          strand;

  GeneticCodePtr gcp;
  ValNodePtr     vnp;
  Int2           gcode;
  Uint1          cdn[3];
  CharPtr        gcd, gcdi;
  SeqPortPtr     spp;
  Uint1Ptr       aaseq;
  Int4           ntpos, aapos;

  Boolean     flagBlastSeed;
  Boolean     flagPostBiasProsite, flagKeepInternalOrfs;
  FloatHi     probcut;
  Int4        clustmin, findmin;
  Int4        tripletwindow, frame, ipos, endpos, iscore;
  FloatHi     scorecutoff;
  FloatHiPtr  score;

#ifndef NO_BLS_NET
  EpiDatPtr      epip;
  BioseqPtr      bspaa;
#endif
  CharPtr        aa, cptr;
  SeqEntryPtr    sepaa;
  ValNodePtr     seqhead, seqlist;
  SeqIdPtr       sip;
  TextSeqIdPtr   tsip;
  Char           name[256];

  Boolean        flagShowMarkov;
  FreqPtr        frqp;
  CharPtr        bases = "ACGT";
  CharPtr        pattern;
  Int4           codons, number, resn, res, i, n;
  Int4Ptr        adjcdnf0, adjcdnf1, adjcdnf2;
  FloatHiPtr     adjcdns0, adjcdns1, adjcdns2;

  Int4           argnum, argoff;

#ifndef NO_BLS_NET
  argoff = 2;
#else
  argoff = 0;
#endif

  argcount = sizeof (myargs) / sizeof (Args);
  if (!GetArgs ("cnsgn", argcount, myargs))
    return 1;

  gi = myargs[1].intvalue;
  if (myargs[0].strvalue != NULL)
    StrCpy (fastafile, myargs[0].strvalue);
  else
    fastafile[0] = '\0';

  if (myargs[1].intvalue == 0 && myargs[0].strvalue == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 100,
               "No gi or FastA file given : for help : cnsgn -");
    ErrShow ();
    exit (1);
  }

  gsp = &gs;
  MemSet ((Pointer) gsp, 0, sizeof (GatherScope));
  MemSet ((Pointer) gsp->ignore, (int) (TRUE),
          (size_t) (OBJ_MAX * sizeof (Boolean)));
  gsp->ignore[OBJ_BIOSEQ] = FALSE;

  gbsp = &gbs;
  gbsp->bsp = NULL;

  gcdsp = GatherCDSNew ();

  if (gi > 0)
  {
    if (!EntrezInit ("cnsgn", FALSE, &flagHaveNet))
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 101,
                 "Entrez init failed");
      ErrShow ();
      exit (1);
    }
    if (!EntrezBioseqFetchEnable ("cnsgn", TRUE))
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 101,
               "Entrez fetch enable failed");
      ErrShow ();
      exit (1);
    }
  }

#ifndef NO_BLS_NET
  flagBlastSeed = (Boolean) myargs[6].intvalue;
#else
  flagBlastSeed = FALSE;
#endif
  argnum = 5;
  flagPostBiasProsite = (Boolean) myargs[argnum+argoff].intvalue;

#ifndef NO_BLS_NET
  if (flagBlastSeed)
  {
    if (!BlastInit (blast_program, &bl3hp, &bl3rp))
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 101,
                 "Blast init failed");
      ErrShow ();
      exit (1);
    }
  }
#endif

  if (!flagBlastSeed)
  {
    cpph = cpp = ReadPrositePattern (pattern_file, TRUE, -1, NULL, NULL);
    if (cpph == NULL)
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 101,
                 "read failed %s", pattern_file);
      ErrShow ();
      exit (1);
    }
  }
  if (flagPostBiasProsite)
  {
    cph = cp = ReadPrositePattern (pattern_file, TRUE, 25, NULL, NULL);
    if (cph == NULL)
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 101,
                 "read failed %s", pattern_file);
      ErrShow ();
      exit (1);
    }
  }

  if (gi > 0)
  {
    sep = EntrezSeqEntryGet (gi, SEQENTRY_READ_NUC_PROT);
  }
  else
  {
    if ((fiop = FileOpen (fastafile, "r")) == NULL)
    {
      ErrPostEx (SEV_ERROR, TOP_ERROR, 102,
                 "Failed to open FastA file");
      ErrShow ();
      exit (1);
    }
    sep = FastaToSeqEntry (fiop, TRUE); /* is_na */
    AddBioSourceToSeqEntry (sep);
    FileClose (fiop);
  }

  if (sep == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 103,
               "No SeqEntry found");
    ErrShow ();
    exit (1);
  }

  gbsp->gi = gi;
  GatherSeqEntry (sep, (Pointer) gbsp, GetBioseq, (Pointer) gsp);

  if (gbsp->bsp == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 104,
               "No Bioseq found");
    ErrShow ();
    exit (1);
  }
  if (!ISA_na (gbsp->bsp->mol))
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 105,
               "Not a nucleic acid Bioseq");
    ErrShow ();
    exit (1);
  }
  bspnt = gbsp->bsp;
  bspnt->topology = Seq_strand_both;

  slpg = NULL;
  slpp = NULL;
  argnum = 7;
#ifndef NO_BLS_NET
  nt_blast = myargs[argnum+argoff].intvalue;
#endif
  nt_pattern = myargs[argnum+argoff].intvalue;
  nt_postpattern = 150;
  nt_orf = 150;
  argnum = 6;
  flagKeepInternalOrfs = (Boolean) myargs[argnum+argoff].intvalue;

#ifndef NO_BLS_NET
  skiptop = myargs[5].intvalue;
  if (gi > 0 && skiptop == 0)
    skiptop++;
#endif

/* orflist is returned sorted by size of ORF */
  orflist =  GetOrfList (bspnt, (Int2) nt_orf);
  orfhead = orflist =  ClearNonMetOrfs (orflist);

  vnp = bspnt->descr;
  gcode = 0;
  while (vnp != NULL)
  {
    if (vnp->choice == Seq_descr_source)
    {
      gcode = BioSourceToGeneticCode ((BioSourcePtr) vnp->data.ptrvalue);
      break;
    }
    vnp = vnp->next;
  }
  if (gcode == 0 && sep->choice == 2)
  {
    vnp = ((BioseqSetPtr) (sep->data.ptrvalue))->descr;
    while (vnp != NULL)
    {
      if (vnp->choice == Seq_descr_source)
      {
        gcode = BioSourceToGeneticCode ((BioSourcePtr) vnp->data.ptrvalue);
        break;
      }
      vnp = vnp->next;
    }
  }
  if (gcode == 0)
    gcode = 1; /* standard */
  gcp = GeneticCodeFind (gcode, NULL);
  if (gcp == NULL)
    exit (1);
  gcd = gcdi = NULL;
  vnp = (ValNodePtr) gcp->data.ptrvalue;
  while (vnp != NULL)
  {
    if (vnp->choice == 6)       /* sncbieaa */
      gcdi = (CharPtr) vnp->data.ptrvalue;
    else if (vnp->choice == 3)  /* ncbieaa */
      gcd = (CharPtr) vnp->data.ptrvalue;
    vnp = vnp->next;
  }
  if (gcdi == NULL)
    gcdi = gcd;
  if (gcdi == NULL)
  {
    ErrPostEx (SEV_ERROR, TOP_ERROR, 105,
               "Could not get genetic code for translation");
    ErrShow ();
    exit (1);
  }
  seqhead = NULL;
  while (orflist != NULL)
  {
    slp = (SeqLocPtr) orflist->data.ptrvalue;
    if (slp->choice == SEQLOC_MIX)
      slp = (SeqLocPtr) slp->data.ptrvalue;
    start = SeqLocStart (slp);
    stop = SeqLocStop (slp);
    strand = SeqLocStrand (slp);
    aa = (CharPtr) MemNew ((size_t) (sizeof(Char)*(stop-start)));
    cptr = aa;
    spp = SeqPortNew (bspnt, start, stop, strand, Seq_code_ncbi4na);
    ntpos = start;
    aapos = 0;
    while (ntpos < start+3)
    {
      cdn[0] = SeqPortGetResidue (spp);
      ntpos++;
      cdn[1] = SeqPortGetResidue (spp);
      ntpos++;
      cdn[2] = SeqPortGetResidue (spp);
      ntpos++;
      aa[aapos] = AAForCodon (cdn, gcdi);
      aapos++;
    }
    while (ntpos <= stop)
    {
      cdn[0] = SeqPortGetResidue (spp);
      ntpos++;
      cdn[1] = SeqPortGetResidue (spp);
      ntpos++;
      cdn[2] = SeqPortGetResidue (spp);
      ntpos++;
      aa[aapos] = AAForCodon (cdn, gcd);
      aapos++;
    }
    aa[aapos] = '\0';
    sepaa = FastaToSeqBuff (aa, &cptr, FALSE);
    seqlist = ValNodeNew (NULL);
    seqlist->data.ptrvalue = (Pointer) sepaa;
    ValNodeLink (&seqhead, seqlist);
    SeqPortFree (spp);
    MemFree (aa);
    orflist = orflist->next;
  }

  seqlist = seqhead;
  orflist = orfhead;
  while (orflist != NULL)
  {
    slp = (SeqLocPtr) orflist->data.ptrvalue;
    start = SeqLocStart (slp);
    stop = SeqLocStop (slp);
    strand = SeqLocStrand (slp);
    if (flagBlastSeed)
    {
#ifndef NO_BLS_NET
      if (stop - start + 1 >= nt_blast)
      {
        sepaa = (SeqEntryPtr) seqlist->data.ptrvalue;
        gbsp->gi = 0;
        gbsp->bsp = NULL;
        GatherSeqEntry (sepaa, (Pointer) gbsp, GetBioseq, (Pointer) gsp);
        bspaa = gbsp->bsp;
        epip = EpiDatNew ();
        epip->score = PredictEpiBioseq (bspaa, 0, bspaa->length-1, epip);
        slpepi = FilterEpiBioseq (epip, bspaa, FALSE, FALSE);
        EpiDatFree (epip);
        flagGap = TRUE;
        bl3op = BLASTOptionNew (blast_program, flagGap);
        if (slpepi == NULL)
        {
          bl3op->expect_value = 0.00000001;
          bl3op->wordsize = 2;
        }
        else
        {
          bl3op->expect_value = 0.000001;
          bl3op->wordsize = 3;
        }
        sap = BlastBioseqNet (bl3hp, bspaa, blast_program, blast_database,
                              bl3op, &vnp1, &vnp2, NULL);
        BLASTOptionDelete (bl3op);
        while (slpepi != NULL)
        {
          slpn = slpepi->next;
          SeqLocFree (slpepi);
          slpepi = slpn;
        }
        SeqLocLink (&slpg, SeqLocSave (slp, sap, skiptop));
        while (sap != NULL)
        {
          sapn = sap->next;
          SeqAlignFree (sap);
          sap = sapn;
        }
      }
#else
/* should never get here */
      {
        ErrPostEx (SEV_ERROR, TOP_ERROR, 110,
                   "No BLAST services with this compiled format");
        ErrShow ();
        exit (1);
      }
#endif
    }
    else if ((!flagBlastSeed && stop - start + 1 >= nt_pattern) ||
              (flagPostBiasProsite && stop - start + 1 >= nt_postpattern))
    {
      vnp = bspnt->descr;
      gcode = 0;
      while (vnp != NULL)
      {
        if (vnp->choice == Seq_descr_source)
        {
          gcode = BioSourceToGeneticCode ((BioSourcePtr) vnp->data.ptrvalue);
          break;
        }
        vnp = vnp->next;
      }
      if (gcode == 0 && sep->choice == 2)
      {
        vnp = ((BioseqSetPtr) (sep->data.ptrvalue))->descr;
        while (vnp != NULL)
        {
          if (vnp->choice == Seq_descr_source)
          {
            gcode = BioSourceToGeneticCode ((BioSourcePtr) vnp->data.ptrvalue);
            break;
          }
          vnp = vnp->next;
        }
      }
      if (gcode == 0)
        gcode = 1; /* standard */
      gcp = GeneticCodeFind (gcode, NULL);
      if (gcp == NULL)
        exit (1);
      gcd = gcdi = NULL;
      vnp = (ValNodePtr) gcp->data.ptrvalue;
      while (vnp != NULL)
      {
        if (vnp->choice == 6)       /* sncbieaa */
          gcdi = (CharPtr) vnp->data.ptrvalue;
        else if (vnp->choice == 3)  /* ncbieaa */
          gcd = (CharPtr) vnp->data.ptrvalue;
        vnp = vnp->next;
      }
      if (gcdi == NULL)
        gcdi = gcd;     /* gcd != NULL ? */

      spp = SeqPortNew (bspnt, start, stop, strand, Seq_code_ncbi4na);
      aaseq = (Uint1Ptr) MemNew ((size_t)
                                 (sizeof (Uint1) * (((stop-start)/3)+2)));
      ntpos = start;
      aapos = 0;
      while (ntpos < start+3)
      {
        cdn[0] = SeqPortGetResidue (spp);
        ntpos++;
        cdn[1] = SeqPortGetResidue (spp);
        ntpos++;
        cdn[2] = SeqPortGetResidue (spp);
        ntpos++;
        aaseq[aapos] = AAForCodon (cdn, gcdi);
        aapos++;
      }
      while (ntpos <= stop)
      {
        cdn[0] = SeqPortGetResidue (spp);
        ntpos++;
        cdn[1] = SeqPortGetResidue (spp);
        ntpos++;
        cdn[2] = SeqPortGetResidue (spp);
        ntpos++;
        aaseq[aapos] = AAForCodon (cdn, gcd);
        aapos++;
      }
      aaseq[aapos] = '\0';

      if (!flagBlastSeed && stop - start + 1 >= nt_pattern)
      {
        cpp = cpph;
        while (cpp != NULL)
        {
          sap = PatternMatch (aaseq, 0, Seq_strand_plus, SeqLocId (slp),
                              cpp, 0, Seq_strand_unknown, FALSE);
          if (sap != NULL)
            break;
          cpp = cpp->nextpattern;
        }
        if (sap != NULL)
        {
          SeqLocLink (&slpg, SeqLocDup (slp));
        }
        while (sap != NULL)
        {
          sapn = sap->next;
          SeqAlignFree (sap);
          sap = sapn;
        }
      }
      if (flagPostBiasProsite && stop - start + 1 >= nt_postpattern)
      {
        cp = cph;
        while (cp != NULL)
        {
          sap = PatternMatch (aaseq, 0, Seq_strand_plus, SeqLocId (slp),
                              cp, 0, Seq_strand_unknown, FALSE);
          if (sap != NULL)
            break;
          cp = cp->nextpattern;
        }
        if (sap != NULL)
        {
          SeqLocLink (&slpp, SeqLocDup (slp));
        }
        while (sap != NULL)
        {
          sapn = sap->next;
          SeqAlignFree (sap);
          sap = sapn;
        }
      }

      SeqPortFree (spp);
      MemFree (aaseq);
    }
    orflist = orflist->next;
    seqlist = seqlist->next;
  }

  slpa = NULL;
  orflist = orfhead;
  while (orflist != NULL)
  {
    slp = (SeqLocPtr) orflist->data.ptrvalue;
    SeqLocLink (&slpa, SeqLocDup (slp));
    while (slp != NULL)
    {
      slpn = slp->next;
      SeqLocFree (slp);
      slp = slpn;
    }
    orflist->data.ptrvalue = NULL;
    orflist = orflist->next;
  }
  ValNodeFree (orfhead);

  gsp->ignore[OBJ_BIOSEQ] = TRUE;
  gsp->ignore[OBJ_SEQANNOT] = FALSE;

  argnum = 10;
  flagShowMarkov = (Boolean) myargs[argnum+argoff].intvalue;

  if (flagShowMarkov)
  {
    printf ("\n!!!!!!!!      Markov      !!!!!!!!\n\n");

    frqp = ConKovTrainCDS (bspnt, slpg);

    printf ("Encoding codon triplets (only if non-zero in any frame)\n");
    codons = 9;
    pattern = (CharPtr) MemNew ((size_t) (sizeof (Char) * (codons+1)));
    number = (Int4) pow (4, codons);
    for (i = 0; i < number; i++)
    {
      resn = i;
      for (n = 0; n < codons; n++)
      {
        res = resn % 4;
        pattern[codons-n-1] = bases[res];
        resn /= 4;
      }
      if (frqp->frame0trip[i] != 0 ||
          frqp->frame1trip[i] != 0 ||
          frqp->frame2trip[i] != 0)
      {
        printf ("%8d %s %6d %6d %6d\n", i, pattern, frqp->frame0trip[i],
                                                    frqp->frame1trip[i],
                                                    frqp->frame2trip[i]);
      }
    }
    MemFree (pattern);
    printf ("Codon usage in 3 frames of CDS\n");
    codons = 3;
    pattern = (CharPtr) MemNew ((size_t) (sizeof (Char) * (codons+1)));
    number = (Int4) pow (4, codons);
    for (i = 0; i < number; i++)
    {
      resn = i;
      for (n = 0; n < codons; n++)
      {
        res = resn % 4;
        pattern[codons-n-1] = bases[res];
        resn /= 4;
      }
      printf ("%8d %s       %6d %6d %6d\n", i, pattern, frqp->frame0cdnobs[i],
                                                        frqp->frame1cdnobs[i],
                                                        frqp->frame2cdnobs[i]);
    }
    MemFree (pattern);
    printf ("std encoding non-zero triplets in frame 0 (in-frame)\n");
    codons = 9;
    pattern = (CharPtr) MemNew ((size_t) (sizeof (Char) * (codons+1)));
    number = (Int4) pow (4, codons);
    for (i = 0; i < number; i++)
    {
      resn = i;
      for (n = 0; n < codons; n++)
      {
        res = resn % 4;
        pattern[codons-n-1] = bases[res];
        resn /= 4;
      }
      if (frqp->frame0trip[i] != 0)
      {
        printf ("%8d %s %8.4f %8.4f %8.4f\n", i, pattern, frqp->frame0std[i],
                                                          frqp->frame1std[i],
                                                          frqp->frame2std[i]);
      }
    }
    MemFree (pattern);
    printf ("std encoding non-zero triplets in frame 1 (out of frame)\n");
    codons = 9;
    pattern = (CharPtr) MemNew ((size_t) (sizeof (Char) * (codons+1)));
    number = (Int4) pow (4, codons);
    for (i = 0; i < number; i++)
    {
      resn = i;
      for (n = 0; n < codons; n++)
      {
        res = resn % 4;
        pattern[codons-n-1] = bases[res];
        resn /= 4;
      }
      if (frqp->frame1trip[i] != 0)
      {
        printf ("%8d %s %8.4f %8.4f %8.4f\n", i, pattern, frqp->frame0std[i],
                                                          frqp->frame1std[i],
                                                          frqp->frame2std[i]);
      }
    }
    MemFree (pattern);
    printf ("std encoding non-zero triplets in frame 2 (out of frame)\n");
    codons = 9;
    pattern = (CharPtr) MemNew ((size_t) (sizeof (Char) * (codons+1)));
    number = (Int4) pow (4, codons);
    for (i = 0; i < number; i++)
    {
      resn = i;
      for (n = 0; n < codons; n++)
      {
        res = resn % 4;
        pattern[codons-n-1] = bases[res];
        resn /= 4;
      }
      if (frqp->frame2trip[i] != 0)
      {
        printf ("%8d %s %8.4f %8.4f %8.4f\n", i, pattern, frqp->frame0std[i],
                                                          frqp->frame1std[i],
                                                          frqp->frame2std[i]);
      }
    }
    MemFree (pattern);
    printf ("Adjusted 3-frame codon usage\n");
    adjcdnf0 = (Int4Ptr) MemNew ((size_t) (sizeof (Int4) * 64));
    adjcdnf1 = (Int4Ptr) MemNew ((size_t) (sizeof (Int4) * 64));
    adjcdnf2 = (Int4Ptr) MemNew ((size_t) (sizeof (Int4) * 64));
    adjcdns0 = (FloatHiPtr) MemNew ((size_t) (sizeof (FloatHi) * 64));
    adjcdns1 = (FloatHiPtr) MemNew ((size_t) (sizeof (FloatHi) * 64));
    adjcdns2 = (FloatHiPtr) MemNew ((size_t) (sizeof (FloatHi) * 64));
    codons = 9;
    number = (Int4) pow (4, codons);
    res = (Int4) pow (4, 3);
    for (i = 0; i < number; i++)
    {
      if (frqp->frame0std[i] >= 2.5)
      {
        n = i;
        resn = n % res;
        n /= res;
        resn = n % res;
        adjcdnf0[resn] += frqp->frame0trip[i];
      }
      if (frqp->frame1std[i] >= 2.5)
      {
        n = i;
        resn = n % res;
        n /= res;
        resn = n % res;
        adjcdnf1[resn] += frqp->frame1trip[i];
      }
      if (frqp->frame2std[i] >= 2.5)
      {
        n = i;
        resn = n % res;
        n /= res;
        resn = n % res;
        adjcdnf2[resn] += frqp->frame2trip[i];
      }
    }
    codons = 3;
    pattern = (CharPtr) MemNew ((size_t) (sizeof (Char) * (codons+1)));
    number = (Int4) pow (4, codons);
    for (i = 0; i < number; i++)
    {
      resn = i;
      for (n = 0; n < codons; n++)
      {
        res = resn % 4;
        pattern[codons-n-1] = bases[res];
        resn /= 4;
      }
      printf ("%8d %s       %6d %6d %6d\n", i, pattern, adjcdnf0[i],
                                                        adjcdnf1[i],
                                                        adjcdnf2[i]);
    }
    MemFree (pattern);
    codons = 3;
    pattern = (CharPtr) MemNew ((size_t) (sizeof (Char) * (codons+1)));
    number = (Int4) pow (4, codons);
    for (i = 0; i < number; i++)
    {
      resn = i;
      for (n = 0; n < codons; n++)
      {
        res = resn % 4;
        pattern[codons-n-1] = bases[res];
        resn /= 4;
      }
      if (adjcdnf0[i] == 0)
        adjcdnf0[i] = 1;
      if (adjcdnf1[i] == 0)
        adjcdnf1[i] = 1;
      if (adjcdnf2[i] == 0)
        adjcdnf2[i] = 1;
      adjcdns0[i] = (FloatHi) (frqp->frame0cdnobs[i] - adjcdnf0[i]) /
                    (FloatHi) sqrt ((FloatHi) adjcdnf0[i]);
      adjcdns1[i] = (FloatHi) (frqp->frame1cdnobs[i] - adjcdnf1[i]) /
                    (FloatHi) sqrt ((FloatHi) adjcdnf1[i]);
      adjcdns2[i] = (FloatHi) (frqp->frame2cdnobs[i] - adjcdnf2[i]) /
                    (FloatHi) sqrt ((FloatHi) adjcdnf2[i]);
      printf ("%8d %s %8.4f %8.4f %8.4f\n", i, pattern, adjcdns0[i],
                                                        adjcdns1[i],
                                                        adjcdns2[i]);
    }
    MemFree (pattern);
    MemFree (adjcdnf0);
    MemFree (adjcdnf1);
    MemFree (adjcdnf2);
    MemFree (adjcdns0);
    MemFree (adjcdns1);
    MemFree (adjcdns2);
    FreqFree (frqp);
  }

  printf ("\n!!!!!!!!     All ORFs     !!!!!!!!\n\n");
  SortByLocOrfs (&slpa);
  NameTagOrfs (slpa);
  gcdsp->slpAll = slpa;
  ShowAllFoundOrfs (slpa);

  if (gi > 0)
  {
    printf ("\n!!!!!!!!  All Known ORFs  !!!!!!!!\n\n");
    gcdsp->slpFound = slpa;
    GatherSeqEntry (sep, (Pointer) gcdsp, ShowKnownFoundOrfs, (Pointer) gsp);
    gcdsp->slpFound = NULL;
  }

  if (flagBlastSeed)
  {
    printf ("\n!!!!!!   Seed Blast ORFs    !!!!!!\n\n");
  }
  else
  {
    printf ("\n!!!!!!  Seed Pattern ORFs   !!!!!!\n\n");
  }
  SortByLocOrfs (&slpg);
  TransferNameTags (slpg, slpa);
  ShowAllFoundOrfs (slpg);

  probcut = myargs[2].floatvalue / 100.0;
  clustmin = myargs[3].intvalue;
  findmin = myargs[4].intvalue;

  slp = FindSimilarBiasOrfs (sep, probcut, clustmin, findmin, slpg, slpa);

/* add blast or pattern hits to found list */
  SeqLocLink (&slp, slpg);
  slpg = NULL;
/* add any postbias pattern hits to found list */
  SeqLocLink (&slp, slpp);
  slpp = NULL;

  UniqueOrfs (&slp);
  SortByLocOrfs (&slp);

  if (!flagKeepInternalOrfs)
    RemoveInternalOrfs (&slp);

  printf ("\n!! Seed ORFs plus ORFs With CU Bias To Seed ORFs !\n\n");
  TransferNameTags (slp, slpa);
  gcdsp->slpHit = slp;
  ShowAllFoundOrfs (slp);

  if (gi > 0)
  {
    gcdsp->slpFound = slp;
    printf ("\n!!!!!!!!! Found ORFs That Are Known ORFs !!!!!!!!!\n\n");
    GatherSeqEntry (sep, (Pointer) gcdsp, ShowKnownFoundOrfs, (Pointer) gsp);
    gcdsp->slpFound = NULL;
  }

  printf ("\n!!!!   Six Frame Codon Usage  !!!!\n");
  argnum = 8;
  tripletwindow = myargs[argnum+argoff].intvalue;
  argnum = 9;
  scorecutoff = myargs[argnum+argoff].floatvalue;

  gcdsp->tableGlobal = CodonTableFromSeqLoc (bspnt, slp);
  endpos = bspnt->length - 3 - tripletwindow;
  for (frame = 0; frame < 3; frame++)
  {
    score = BiasScoreBioseq (bspnt, gcdsp->tableGlobal, tripletwindow,
                             frame, Seq_strand_plus);
    printf ("\nplus strand -- frame %ld\n\n", (long) (frame+1));
    iscore = 0;
    for (ipos = frame; ipos < endpos; ipos += 3)
    {
      if (score[iscore] <= scorecutoff)
      {
        slp = gcdsp->slpAll;
        while (slp != NULL)
        {
          if (SeqLocStrand (slp) == Seq_strand_plus)
          {
            start = SeqLocStart (slp);
            stop = SeqLocStop (slp);
            if (ipos >= start && ipos <= stop)
              break;
          }
          slp = slp->next;
        }
        if (slp == NULL)
        {
          StrCpy (name, "-");
        }
        else
        {
          sip = SeqLocId (slp);
          if (sip != NULL)
          {
            while (sip->next != NULL)
              sip = sip->next;
            if (sip->choice == SEQID_OTHER)
            {
              tsip = (TextSeqIdPtr) sip->data.ptrvalue;
              if (tsip->name != NULL)
                StrCpy (name, tsip->name);
              else
                StrCpy (name, "-");
            }
            else
            {
              StrCpy (name, "-");
            }
          }
          else
          {
            StrCpy (name, "-");
          }
        }
        printf ("%8ld %8.5f %-10s\n", (long) (ipos+1), score[iscore], name);
      }
      iscore++;
    }
    MemFree (score);
  }
  for (frame = 0; frame < 3; frame++)
  {
    score = BiasScoreBioseq (bspnt, gcdsp->tableGlobal, tripletwindow,
                             frame, Seq_strand_minus);
    printf ("\nminus strand -- frame %ld\n\n", (long) (frame+1));
    iscore = 0;
    for (ipos = frame; ipos < endpos; ipos += 3)
    {
      if (score[iscore] <= scorecutoff)
      {
        slp = gcdsp->slpAll;
        while (slp != NULL)
        {
          if (SeqLocStrand (slp) == Seq_strand_minus)
          {
            start = SeqLocStart (slp);
            stop = SeqLocStop (slp);
            if (ipos >= start && ipos <= stop)
              break;
          }
          slp = slp->next;
        }
        if (slp == NULL)
        {
          StrCpy (name, "-");
        }
        else
        {
          sip = SeqLocId (slp);
          if (sip != NULL)
          {
            while (sip->next != NULL)
              sip = sip->next;
            if (sip->choice == SEQID_OTHER)
            {
              tsip = (TextSeqIdPtr) sip->data.ptrvalue;
              if (tsip->name != NULL)
                StrCpy (name, tsip->name);
              else
                StrCpy (name, "-");
            }
            else
            {
              StrCpy (name, "-");
            }
          }
          else
          {
            StrCpy (name, "-");
          }
        }
        printf ("%8ld %8.5f %-10s\n", (long) (ipos+1), score[iscore], name);
      }
      iscore++;
    }
    MemFree (score);
  }

  seqlist = seqhead;
  while (seqlist != NULL)
  {
    seqlist->data.ptrvalue =
     (Pointer) SeqEntryFree ((SeqEntryPtr) seqlist->data.ptrvalue);
    seqlist = seqlist->next;
  }
  ValNodeFree (seqhead);
  GatherCDSFree (gcdsp);
  SeqEntryFree (sep);

  if (gi > 0)
  {
    EntrezBioseqFetchDisable ();
    EntrezFini ();
  }
#ifndef NO_BLS_NET
  if (flagBlastSeed)
    BlastFini (bl3hp);
#endif
  if (!flagBlastSeed)
    ComPatFree (cpph);
  if (flagPostBiasProsite)
    ComPatFree (cph);

  return 0;
}
