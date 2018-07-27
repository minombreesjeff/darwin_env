/* ===========================================================================
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
*  any work or product based on this material.
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
* File Name:  spideymain.c
*
* Author:  Sarah Wheelan
*
* Version Creation Date:   5/01
*
* $Revision: 6.10 $
*
* File Description: main functions for running Spidey as a standalone 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: spideymain.c,v $
* Revision 6.10  2004/03/25 21:20:03  kskatz
* All SPI_is_acceptor_* functions have been corrected: 'N' no longer contributes to nor subtracts from the score, log odds are calculated and the scores added; they are however all antilogged because there are too many places in the code where the score is expected to be between 0 and 1.  Also, corrected sequence frequency determination in SPI_is_acceptor_user and SPI_is_donor_user, as well as correcting for 'N'. Finally, and this all began with, I added matrices for Dictyostelium - command line -r -m
*
* Revision 6.9  2003/04/04 19:42:56  kskatz
* Added a new command line option (-R) to allow external users to point spidey to a repeat database that it can pass on to blast for filtering repeats
*
* Revision 6.8  2002/10/02 16:47:11  kskatz
* clarifying the explanation of the -L option
*
* Revision 6.7  2002/10/02 16:03:16  kskatz
* added new command line option (-L) for user to use in conjunction with -X (big intron bool); -L allows user to supply the max size (bp) of the intron to use (default 220000) and sets the SPI_Option bigintron_size to this value
*
* Revision 6.6  2002/05/07 18:43:46  wheelan
* changes to support user-defined splice matrices
*
* Revision 6.5  2001/11/05 16:13:55  wheelan
* bug fix in multiple alignment code; changed call to SPI_PrintMult..
*
* Revision 6.4  2001/10/04 12:36:45  wheelan
* added bigintron option
*
* Revision 6.3  2001/08/24 13:45:48  wheelan
* changed printing options
*
* Revision 6.2  2001/07/10 16:45:53  wheelan
* changes to produce multiple alignments upon request
*
* Revision 6.1  2001/05/24 16:27:41  wheelan
* initial checkin
*
*
* ==========================================================================
*/


#include <ncbi.h>
#include <spidey.h>
#include <accid1.h>
#include <lsqfetch.h>

#define MYARGGENFILE   0
#define MYARGMRNAFILE  1
#define MYARGPRALIGN   2
#define MYARGOUTFILE   3
#define MYARGALNFILE   4
#define MYARGGILIST    5
#define MYARGNUMMOD    6
#define MYARGORG       7
#define MYARG1STEVAL   8
#define MYARG2NDEVAL   9
#define MYARG3RDEVAL   10
#define MYARGIDCUT     11
#define MYARGLENCUT    12
#define MYARGSPEC      13
#define MYARGASN       14
#define MYARGASNFILE   15
/*#define MYARGDRAFTFILE 16*/
#define MYARGMASKED    16
#define MYARGGETCDS    17
#define MYARGTABLE     18
/*#define MYARGACEDB     19*/
#define MYARGFROM      19 
#define MYARGTO        20
#define MYARGMULT      21
#define MYARGXL        22
#define MYARGXL_SIZE   23 /* added by KSK */
#define MYARGSTRAND    24
#define MYARGDSPLICE   25
#define MYARGASPLICE   26
#define MYARGREPDB     27

#define NUMARGS        28

Args myargs[NUMARGS] = {
   {"Input file -- genomic sequence(s)", NULL, NULL, NULL, FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
   {"Input file -- mRNA sequence(s)", NULL, NULL, NULL, FALSE, 'm', ARG_FILE_IN, 0.0, 0, NULL},
   {"Print alignment? 0=summary+alignments, 1=summary, 2=alignments, 3=summary&alignments in different files", "0", NULL, NULL, TRUE, 'p', ARG_INT, 0.0, 0, NULL},
   {"Output file 1 (summary or summary+aln)", "stdout", NULL, NULL, TRUE, 'o', ARG_STRING, 0.0, 0, NULL},
   {"Output file 2 (alignments)", "spidey.aln", NULL, NULL, TRUE, 'a', ARG_STRING, 0.0, 0, NULL},
   {"Input file is a GI list", "F", NULL, NULL, TRUE, 'G', ARG_BOOLEAN, 0.0, 0, NULL},
   {"Number of gene models", "1", NULL, NULL, TRUE, 'n', ARG_INT, 0.0, 0, NULL},
   {"Organism (genomic sequence) v=vertebrate,\nd = drosophila, p = plant, c = C. elegans, m = Dictyostelium discoideum", "v", NULL, NULL, TRUE, 'r', ARG_STRING, 0.0, 0, NULL},
   {"First-pass e-value", "0.0000000001", NULL, NULL, TRUE, 'e', ARG_FLOAT, 0.0, 0, NULL},
   {"Second-pass e-value", "0.001", NULL, NULL, TRUE, 'f', ARG_FLOAT, 0.0, 0, NULL},
   {"Third-pass e-value", "10", NULL, NULL, TRUE, 'g', ARG_FLOAT, 0.0, 0, NULL},
   {"% identity cutoff", "0", NULL, NULL, TRUE, 'c', ARG_INT, 0.0, 0, NULL},
   {"% length coverage cutoff", "0", NULL, NULL, TRUE, 'l', ARG_INT, 0.0, 0, NULL},
   {"interspecies alignment", "F", NULL, NULL, TRUE, 's', ARG_BOOLEAN, 0.0, 0, NULL},
   {"Print ASN.1 alignment?", "F", NULL, NULL, TRUE, 'j', ARG_BOOLEAN, 0.0, 0, NULL},
   {"File for asn.1", "spidey.asn", NULL, NULL, TRUE, 'k', ARG_STRING, 0.0, 0, NULL},
/*   {"File of draft boundary information", NULL, NULL, NULL, TRUE, 'b', ARG_FILE_IN, 0.0, 0, NULL},*/
   {"Is the input mRNA masked (lowercase)?", "F", NULL, NULL, TRUE, 'w', ARG_BOOLEAN, 0.0, 0, NULL},
   {"Fetch the CDS and compute its results also?", "F", NULL, NULL, TRUE, 'd', ARG_BOOLEAN, 0.0, 0, NULL},
   {"File with feature table", NULL, NULL, NULL, TRUE, 't', ARG_FILE_IN, 0.0, 0, NULL},
   /*{"ACEDB format", "F", NULL, NULL, TRUE, 'A', ARG_BOOLEAN, 0.0, 0, NULL},*/
   {"Start of genomic interval desired (from)", "0", NULL, NULL, TRUE, 'F', ARG_INT, 0.0, 0, NULL},
   {"Stop of genomic interval desired (to)", "0", NULL, NULL, TRUE, 'T', ARG_INT, 0.0, 0, NULL},
   {"Make a multiple alignment of all input mRNAs", "F", NULL, NULL, TRUE, 'u', ARG_BOOLEAN, 0.0, 0, NULL},
   {"Use extra-large intron sizes", "F", NULL, NULL, TRUE, 'X', ARG_BOOLEAN, 0.0, 0, NULL},
   {"The extra-large intron size to use", "220000", NULL, NULL, TRUE, 'L', ARG_INT, 0.0, 0, NULL},
   {"Restrict to plus (p) or minus (p) strand of genomic seq?", NULL, NULL, NULL, TRUE, 'S', ARG_STRING, 0.0, 0, NULL},
   {"File with donor splice matrix", NULL, NULL, NULL, TRUE, 'M', ARG_FILE_IN, 0.0, 0, NULL},
   {"File with acceptor splice matrix", NULL, NULL, NULL, TRUE, 'N', ARG_FILE_IN, 0.0, 0, NULL},
      /* KSK added */
   {"File (including path) to repeat blast database for filtering", NULL, NULL, NULL, TRUE, 'R', ARG_STRING, 0.0, 0, NULL},
};

static void SPI_FindAllNuc(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
static CharPtr ReadALine (CharPtr str, size_t size, FILE *fp);
static BioseqPtr SPI_GetBspFromGIOrAcc(CharPtr str);
static void SPI_GetSpliceInfo(SPI_OptionsPtr spot, FILE *sfp, Boolean donor);
static void SPI_ReadFeatureTable(FILE *ifp, SPI_bsinfoPtr spim_head);

Int2 Main()
{
   AsnIoPtr           aip;
   BioseqPtr          bsp;
   Pointer            dataptr;
   Uint2              datatype;
   Boolean            found;
   SPI_mRNAToHerdPtr  h_head;
   SPI_mRNAToHerdPtr  h_prev;
   SPI_mRNAToHerdPtr  hptr;
   FILE               *ifp;
   Boolean            isGIlist;
   Char               line[60];
   Boolean            lowercase;
   SeqLocPtr          lcaseloc;
   FILE               *ofp;
   FILE               *ofp2;
   SeqAlignPtr        sap;
   SeqAnnotPtr        sanp;
   SeqEntryPtr        sep;
   FILE               *sfp;
   SeqIdPtr           sip;
   SeqLocPtr          slp;
   SPI_bsinfoPtr      spig;
   SPI_bsinfoPtr      spig_head;
   SPI_bsinfoPtr      spig_prev;
   SPI_bsinfoPtr      spim;
   SPI_bsinfoPtr      spim_head;
   SPI_bsinfoPtr      spim_prev;
   SPI_OptionsPtr     spot;
   SPI_RegionInfoPtr  srip;
   SPI_RegionInfoPtr  srip_head;
   SPI_RegionInfoPtr  srip_prev;
   CharPtr            str;
   CharPtr            txt;

   ID1BioseqFetchEnable("spidey", FALSE);
   LocalSeqFetchInit(FALSE);
   /* standard setup */
   ErrSetFatalLevel (SEV_MAX);
   ErrClearOptFlags (EO_SHOW_USERSTR);
   UseLocalAsnloadDataAndErrMsg ();
   ErrPathReset ();
   if (! AllObjLoad ())
   {
      Message (MSG_FATAL, "AllObjLoad failed");
      return 1;
   }
   if (! SubmitAsnLoad ())
   {
      Message (MSG_FATAL, "SubmitAsnLoad failed");
      return 1;
   }
   if (! FeatDefSetLoad ())
   {
      Message (MSG_FATAL, "FeatDefSetLoad failed");
      return 1;
   }
   if (! SeqCodeSetLoad ())
   {
      Message (MSG_FATAL, "SeqCodeSetLoad failed");
      return 1;
   }
   if (! GeneticCodeTableLoad ())
   {
      Message (MSG_FATAL, "GeneticCodeTableLoad failed");
      return 1;
   }
   if (!GetArgs("SPIDEY", NUMARGS, myargs))
      return 0;
   /* set the error message level high to suppress warnings from BLAST */
   isGIlist = (Boolean)myargs[MYARGGILIST].intvalue;
   txt = myargs[MYARGGENFILE].strvalue;
   ifp = FileOpen(txt, "r");
   spig_head = NULL;
   if (ifp == NULL)
   {
      bsp = SPI_GetBspFromGIOrAcc(txt);
      if (bsp == NULL)
      {
         ErrPostEx(SEV_ERROR, 0, 0, "Can't open genomic input file\n");
         return -1;
      } else
      {
         spig_head = (SPI_bsinfoPtr)MemNew(sizeof(SPI_bsinfo));
         spig_head->bsp = bsp;
      }
   }
   if (spig_head == NULL)
   {
      spig_prev = NULL;
      /* read in the genomic sequence(s) first and put them into bsinfo structures */
      while ((dataptr = ReadAsnFastaOrFlatFile (ifp, &datatype, NULL, FALSE, FALSE, TRUE, FALSE)) != NULL)
      {
         if (datatype == OBJ_BIOSEQ)
         {
            spig = (SPI_bsinfoPtr)MemNew(sizeof(SPI_bsinfo));
            spig->bsp = (BioseqPtr)dataptr;
            if (spig_head == NULL)
               spig_head = spig_prev = spig;
            else
            {
               spig_prev->next = spig;
               spig_prev = spig;
            }
         } else if (datatype == OBJ_SEQENTRY)
         {
            sep = (SeqEntryPtr)dataptr;
            SeqEntryExplore(sep, &spig_head, SPI_FindAllNuc);
         }
      }
      FileClose(ifp);
   }
   if (spig_head == NULL)
   {
      ErrPostEx(SEV_ERROR, 0, 0, "No valid bioseqs in genomic file\n");
      return -1;
   } else if (ISA_aa(spig_head->bsp->mol))
   {
      ErrPostEx(SEV_ERROR, 0, 0, "At least one of the genomic sequences appears to be a protein.\n");
      return -1;
   }
   if (spig_head->next != NULL)
   {
      ErrPostEx(SEV_ERROR, 0, 0, "This version can only process one genomic sequence at a time.  Only the first sequence in this file will be used.\n");
      spig_head->next = NULL;
   }
   spim_head = spim_prev = NULL;
   txt = myargs[MYARGMRNAFILE].strvalue;
   ifp = FileOpen(txt, "r");
   if (ifp == NULL)
   {
      bsp = SPI_GetBspFromGIOrAcc(txt);
      if (bsp == NULL)
      {
         ErrPostEx(SEV_ERROR, 0, 0, "Can't open mRNA input file\n");
         return -1;
      } else
      {
         spim_head = (SPI_bsinfoPtr)MemNew(sizeof(SPI_bsinfo));
         spim_head->bsp = bsp;
      }
   }
   if (spim_head == NULL)
   {
      lowercase = (Boolean)myargs[MYARGMASKED].intvalue;
      lcaseloc = NULL;
      /* if the mRNA has lowercase masking, read it in carefully to record the masking */
      if (lowercase == TRUE)
      {
         while ((sep = FastaToSeqEntryForDb(ifp, TRUE, NULL, TRUE, NULL, NULL, &lcaseloc)) != NULL)
         {
            SeqEntryExplore(sep, &spim_head, SPI_FindAllNuc);
            if (lcaseloc != NULL)  /* put masking info into the bsinfo structure */
            {
               spim = spim_head;
               sip = SeqLocId(lcaseloc);
               found = FALSE;
               while (spim != NULL && !found)
               {
                  if (SeqIdComp(sip, spim->bsp->id) == SIC_YES)
                  {
                     found = TRUE;
                     spim->lcaseloc = lcaseloc;
                  }
                  spim = spim->next;
               }
               lcaseloc = NULL;
            }
         }
      } else if (isGIlist) /* mRNA file is a list of GIs, must fetch the bioseqs */
      {
         str = ReadALine(line, sizeof(line), ifp);
         while (str != NULL)
         {
            bsp = SPI_GetBspFromGIOrAcc(str);
            if (bsp != NULL)
            {
               spim = (SPI_bsinfoPtr)MemNew(sizeof(SPI_bsinfo));
               spim->bsp = bsp;
               if (spim_head == NULL)
                  spim_head = spim_prev = spim;
               else
               {
                  spim_prev->next = spim;
                  spim_prev = spim;
               }
            }
            str = ReadALine(line, sizeof(line), ifp);
         }
      } else /* mRNAs are FASTA or ASN.1, read them all in */
      {
         while ((dataptr = ReadAsnFastaOrFlatFile (ifp, &datatype, NULL, FALSE, FALSE, TRUE, FALSE)) != NULL)
         {
            if (datatype == OBJ_BIOSEQ)
            {
               spim = (SPI_bsinfoPtr)MemNew(sizeof(SPI_bsinfo));
               spim->bsp = (BioseqPtr)dataptr;
               if (spim_head == NULL)
                  spim_head = spim_prev = spim;
               else
               {
                  spim_prev->next = spim;
                  spim_prev = spim;
               }
            } else if (datatype == OBJ_SEQENTRY)
            {
               sep = (SeqEntryPtr)dataptr;
               SeqEntryExplore(sep, &spim_head, SPI_FindAllNuc);
            }
         }
      }
      FileClose(ifp);
   }
   if (spim_head == NULL)
   {
      ErrPostEx(SEV_ERROR, 0, 0, "No valid bioseqs in mRNA file\n");
      return -1;
   } else if (ISA_aa(spim_head->bsp->mol))
   {
      ErrPostEx(SEV_ERROR, 0, 0, "At least one of the mRNA sequences appears to be a protein\n");
      return -1;
   }
   txt = myargs[MYARGTABLE].strvalue;
   if (txt != NULL)
   {
      ifp = FileOpen(txt, "r");
      if (ifp == NULL)
      {
         ErrPostEx(SEV_ERROR, 0, 0, "Unable to open table file\n");
         return -1;
      }
      SPI_ReadFeatureTable(ifp, spim_head);
      spim = spim_head;
      while (spim != NULL)
      {
         if (spim->lcaseloc != NULL)
         {
            slp = (SeqLocPtr)ValNodeNew(NULL);
            slp->choice = SEQLOC_MIX;
            slp->data.ptrvalue = (Pointer)spim->lcaseloc;
            spim->lcaseloc = slp;
         }
         spim = spim->next;
      }
   }
   spim = spim_head;
   spot = (SPI_OptionsPtr)MemNew(sizeof(SPI_Options));
   spot->printaln = myargs[MYARGPRALIGN].intvalue;
   txt = myargs[MYARGOUTFILE].strvalue;
   ofp = FileOpen(txt, "w");
   if (ofp == NULL && spot->printaln != 3)
   {
      ErrPostEx(SEV_ERROR, 0, 0, "Unable to open output file\n");
      return -1;
   }
   if (spot->printaln >= 2)
   {
      txt = myargs[MYARGALNFILE].strvalue;
      ofp2 = FileOpen(txt, "a");
      if (ofp2 == NULL)
      {
         ErrPostEx(SEV_ERROR, 0, 0, "Unable to open output file 2\n");
         return -1;
      }
   } else
      ofp2 = NULL;
   ErrSetMessageLevel(SEV_MAX);
   spot->firstpasseval = myargs[MYARG1STEVAL].floatvalue;
   spot->secpasseval = myargs[MYARG2NDEVAL].floatvalue;
   spot->thirdpasseval = myargs[MYARG3RDEVAL].floatvalue;
   spot->numreturns = myargs[MYARGNUMMOD].intvalue;
   spot->idcutoff = myargs[MYARGIDCUT].intvalue;
   spot->lencutoff = myargs[MYARGLENCUT].intvalue;
   spot->interspecies = (Boolean)myargs[MYARGSPEC].intvalue;
   spot->printasn = (Boolean)myargs[MYARGASN].intvalue;
   spot->fetchcds = (Boolean)myargs[MYARGGETCDS].intvalue;
   /*spot->ace = (Boolean)myargs[MYARGACEDB].intvalue;*/
   spot->from = myargs[MYARGFROM].intvalue;
   spot->to = myargs[MYARGTO].intvalue;
   spot->makemult = (Boolean)myargs[MYARGMULT].intvalue;
   /*KSK*/
   spot->bigintron = (Boolean)myargs[MYARGXL].intvalue; 
   spot->bigintron_size = myargs[MYARGXL_SIZE].intvalue;
   spot->repeat_db_file = myargs[MYARGREPDB].strvalue;
   txt = myargs[MYARGORG].strvalue;
   if (!StringICmp(txt, "d") || !StringICmp(txt, "D")){
       spot->organism = SPI_FLY;
   }
   else if (!StringICmp(txt, "p") || !StringICmp(txt, "P")){
       spot->organism = SPI_PLANT;
   }
   else if (!StringICmp(txt, "c") || !StringICmp(txt, "C")){
       spot->organism = SPI_CELEGANS;
   }
   else if (!StringICmp(txt, "m") || !StringICmp(txt, "M")){
       spot->organism = SPI_DICTY;
   }
   else {
       spot->organism = SPI_VERTEBRATE;
   }
   sap = NULL;
   if (spot->printasn)
      spot->sap_head = &sap;
   txt = myargs[MYARGSTRAND].strvalue;
   if (txt != NULL)
   {
      if (StrChr(txt, 'p') || StrChr(txt, 'P'))
         spot->strand = Seq_strand_plus;
      else
         spot->strand = Seq_strand_minus;
   } else
      spot->strand = Seq_strand_both;
   /*txt = myargs[MYARGDRAFTFILE].strvalue;
   if (txt != NULL)
      spot->draftfile = StringSave(txt);*/
   txt = myargs[MYARGDSPLICE].strvalue;
   if (txt != NULL)
   {
      sfp = FileOpen(txt, "r");
      SPI_GetSpliceInfo(spot, sfp, TRUE);
      FileClose(sfp);
   }
   txt = myargs[MYARGASPLICE].strvalue;
   if (txt != NULL)
   {
      sfp = FileOpen(txt, "r");
      SPI_GetSpliceInfo(spot, sfp, FALSE);
      FileClose(sfp);
   }
   h_head = h_prev = NULL;
   srip_head = srip_prev = NULL;
   while (spim != NULL)
   {
      spot->lcaseloc = spim->lcaseloc;
      if (spot->draftfile == NULL)
         srip = SPI_AlnSinglemRNAToGen(spig_head, spim, ofp, ofp2, spot);
      else
      {
         hptr = SPI_AlnSinglemRNAToPieces(spig_head, spim, ofp, ofp2, spot);
         if (h_head != NULL)
         {
            h_prev->next = hptr;
            h_prev = hptr;
         } else
            h_head = h_prev = hptr;
      }
      if (srip != NULL)
      {
         if (srip_head != NULL)
         {
            srip_prev->next = srip;
            srip_prev = srip;
         } else
            srip_head = srip_prev = srip;
      }
      spim = spim->next;
   }
   if (spot->makemult)
   {
      SPI_MakeMultipleAlignment(srip_head);
      SPI_PrintMultipleAlignment(srip_head, FALSE, spig_head->bsp, ofp);
      SPI_RegionListFree(srip_head);
   } else
      SPI_RegionListFree(srip_head);
   /* create the ASN.1 output, if requested; need to use the continuous alignment */
   /* that was generated */
   if (spot->printasn && *(spot->sap_head) != NULL && spot->draftfile == NULL)
   {
      sanp = SeqAnnotForSeqAlign(*(spot->sap_head));
      txt = myargs[MYARGASNFILE].strvalue;
      aip = AsnIoOpen(txt, "w");
      SeqAnnotAsnWrite(sanp, aip, NULL);
      AsnIoClose(aip);
      SeqAlignSetFree(*(spot->sap_head));
   }
   FileClose(ofp);
   FileClose(ofp2);
   SPI_OptionsFree(spot);
   SPI_bsinfoFreeList(spim_head);
   SPI_bsinfoFreeList(spig_head);
   LocalSeqFetchDisable();
   ID1BioseqFetchDisable();
   return 0;
}

/***************************************************************************
*
*  SPI_FindAllNuc is a SeqEntryExplore callback function that puts all
*  the nucleotide bioseqs in a seqentry into a linked list of SPI_bsinfoPtrs.
*
***************************************************************************/
static void SPI_FindAllNuc(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
   BioseqPtr      bsp;
   SPI_bsinfoPtr  spi;
   SPI_bsinfoPtr  PNTR spim_head;

   spim_head = (SPI_bsinfoPtr PNTR)data;
   spi = *spim_head;
   if (IS_Bioseq(sep))
   {
      bsp = (BioseqPtr)sep->data.ptrvalue;
      if (ISA_na(bsp->mol))
      {
         if (spi != NULL)
         {
            while (spi->next != NULL)
            {
               spi = spi->next;
            }
            spi->next = (SPI_bsinfoPtr)MemNew(sizeof(SPI_bsinfo));
            spi = spi->next;
         } else
            spi = *spim_head = (SPI_bsinfoPtr)MemNew(sizeof(SPI_bsinfo));
         spi->bsp = bsp;
      }
   }
}

/***************************************************************************
*
*  ReadALine is copied from Jonathan's code -- it simply reads 
*  consecutive lines from a file.
*
***************************************************************************/
static CharPtr ReadALine (CharPtr str, size_t size, FILE *fp)
{
   Char     ch;
   CharPtr  ptr;
   CharPtr  rsult;

   if (str == NULL || size < 1 || fp == NULL) return NULL;
   *str = '\0';
   rsult = fgets (str, size, fp);
   if (rsult != NULL)
   {
      ptr = str;
      ch = *ptr;
      while (ch != '\0' && ch != '\n' && ch != '\r')
      {
         ptr++;
         ch = *ptr;
      }
      *ptr = '\0';
   }
   return rsult;
}

/***************************************************************************
*
*  SPI_GetBspFromGIOrAcc takes a string, decides whether it's probably
*  a GI or an accession, converts it to an appropriate seqid, and then
*  attempts to fetch and return the bioseq referred to by that id.
*
***************************************************************************/
static BioseqPtr SPI_GetBspFromGIOrAcc(CharPtr str)
{
   BioseqPtr  bsp;
   Int4       gi;
   Char       ptr;
   SeqIdPtr   sip;
   ValNode    vn;

   str = TrimSpacesAroundString(str);
   ptr = *str;
   if (IS_ALPHA(ptr))  /* accession */
   {
      sip = SeqIdFromAccessionDotVersion(str);
      bsp = BioseqLockById(sip);
   } else  /* it's a GI */
   {
      gi = atoi(str);
      vn.choice = SEQID_GI;
      vn.data.intvalue = gi;
      vn.next = NULL;
      bsp = BioseqLockById(&vn);
   }
   return bsp;
}

/***************************************************************************
*
*  SPI_ReadFeatureTable reads in a tab-delimited file and converts it
*  to feature information:
*
*  sequence id	name of feature		start	stop
*  NM_004377.1	repetitive_region	12	40
*
*  Masking and other feature information for all mRNAs should be in the
*  same file; ids not in the mRNA list or unknown feature names will be
*  ignored.
*
***************************************************************************/
static void SPI_ReadFeatureTable(FILE *ifp, SPI_bsinfoPtr spim_head)
{
   Char           c;
   CharPtr        fields[5];
   Boolean        found;
   Char           line[60];
   Int4           i;
   CharPtr        ptr;
   SeqIdPtr       sip;
   SeqLocPtr      slp;
   SPI_bsinfoPtr  spim;
   CharPtr        str;
   ValNode        vn;

   str = ReadALine(line, sizeof(line), ifp);
   while (str != NULL)
   {
      ptr = strtok(str, " \t");
      for (i=0; i<4; i++)
      {
         fields[i] = StringSave(ptr);
         ptr = strtok(NULL, " \t");
      }
      if (fields[1] != NULL && !StrCmp(fields[1], "repetitive_region"))
      {
         c = *fields[0];
         if (IS_ALPHA(c))
            sip = SeqIdFromAccessionDotVersion(fields[0]);
         else
         {
            vn.choice = SEQID_GI;
            vn.data.intvalue = atoi(fields[0]);
            vn.next = NULL;
            sip = &vn;
         }
         spim = spim_head;
         found = FALSE;
         while (!found && spim != NULL)
         {
            if (SeqIdIn(sip, spim->bsp->id) == TRUE)
               found = TRUE;
            else
               spim = spim->next;
         }
         if (found)
         {
            slp = SeqLocIntNew(atoi(fields[2]), atoi(fields[3]), Seq_strand_both, spim->bsp->id);
            slp->next = spim->lcaseloc;
            spim->lcaseloc = slp;
         }
      }
      str = ReadALine(line, sizeof(line), ifp);
   }
}

static void SPI_GetSpliceInfo(SPI_OptionsPtr spot, FILE *sfp, Boolean donor)
{
   Char               line[1000];
   CharPtr            ptr;
   SPI_SpliceInfoPtr  ssp;
   SPI_SpliceInfoPtr  ssp_head;
   SPI_SpliceInfoPtr  ssp_prev;
   CharPtr            str;

   if (sfp == NULL)
   {
      printf("Matrix file not found\n");
      return;
   }
   str = ReadALine(line, sizeof(line), sfp);
   /* should just be a single number */
   if (donor)
      spot->dsplicejunc = atoi(str);
   else
      spot->asplicejunc = atoi(str);
   /* A */
   str = ReadALine(line, sizeof(line), sfp);
   ptr = strtok(str, "\t");
   ssp_prev = NULL;
   while (ptr != NULL)
   {
      ssp = (SPI_SpliceInfoPtr)MemNew(sizeof(SPI_SpliceInfo));
      ssp->a = atof(ptr);
      if (ssp_prev != NULL)
      {
         ssp_prev->next = ssp;
         ssp_prev = ssp;
      } else
         ssp_head = ssp_prev = ssp;
      ptr = strtok(NULL, "\t");
   }
   if (donor)
      spot->dssp_head = ssp_head;
   else
      spot->assp_head = ssp_head;
   /* C */
   str = ReadALine(line, sizeof(line), sfp);
   ptr = strtok(str, "\t");
   ssp = ssp_head;
   while (ptr != NULL)
   {
      if (ssp == NULL)  /* error */
      {
         printf("error reading splice matrix -- not all lines are same length");
         return;
      }
      ssp->c = atof(ptr);
      ptr = strtok(NULL, "\t");
      ssp = ssp->next;
   }
   if (ssp != NULL)  /* error */
   {
      printf("error reading splice matrix -- not all lines are same length");
      return;
   }
   /* G */
   str = ReadALine(line, sizeof(line), sfp);
   ptr = strtok(str, "\t");
   ssp = ssp_head;
   while (ptr != NULL)
   {
      if (ssp == NULL)  /* error */
      {
         printf("error reading splice matrix -- not all lines are same length");
         return;
      }
      ssp->g = atof(ptr);
      ptr = strtok(NULL, "\t");
      ssp = ssp->next;
   }
   if (ssp != NULL)  /* error */
   {
      printf("error reading splice matrix -- not all lines are same length");
      return;
   }
   /* T */
   str = ReadALine(line, sizeof(line), sfp);
   ptr = strtok(str, "\t");
   ssp = ssp_head;
   while (ptr != NULL)
   {
      if (ssp == NULL)  /* error */
      {
         printf("error reading splice matrix -- not all lines are same length");
         return;
      }
      ssp->t = atof(ptr);
      ptr = strtok(NULL, "\t");
      ssp = ssp->next;
   }
   if (ssp != NULL)  /* error */
   {
      printf("error reading splice matrix -- not all lines are same length");
      return;
   }
}
