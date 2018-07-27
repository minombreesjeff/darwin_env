static char const rcsid[] = "$Id: bl2seq.c,v 6.60 2004/02/26 16:25:06 camacho Exp $";

/**************************************************************************
*                                                                         *
*                             COPYRIGHT NOTICE                            *
*                                                                         *
* This software/database is categorized as "United States Government      *
* Work" under the terms of the United States Copyright Act.  It was       *
* produced as part of the author's official duties as a Government        *
* employee and thus can not be copyrighted.  This software/database is    *
* freely available to the public for use without a copyright notice.      *
* Restrictions can not be placed on its present or future use.            *
*                                                                         *
* Although all reasonable efforts have been taken to ensure the accuracy  *
* and reliability of the software and data, the National Library of       *
* Medicine (NLM) and the U.S. Government do not and can not warrant the   *
* performance or results that may be obtained by using this software,     *
* data, or derivative works thereof.  The NLM and the U.S. Government     *
* disclaim any and all warranties, expressed or implied, as to the        *
* performance, merchantability or fitness for any particular purpose or   *
* use.                                                                    *
*                                                                         *
* In any work or product derived from this material, proper attribution   *
* of the author(s) as the source of the software or data would be         *
* appreciated.                                                            *
*                                                                         *
***************************************************************************
*
* $Log: bl2seq.c,v $
* Revision 6.60  2004/02/26 16:25:06  camacho
* Fix uninitialized variable errors that caused core dumps on empty input files
*
* Revision 6.59  2003/06/26 18:56:05  coulouri
* remove unnecessary variables
*
* Revision 6.58  2003/05/30 17:31:09  coulouri
* add rcsid
*
* Revision 6.57  2003/05/13 16:02:42  coulouri
* make ErrPostEx(SEV_FATAL, ...) exit with nonzero status
*
* Revision 6.56  2003/05/06 18:57:46  dondosha
* Do not set cutoff_s for megablast, it is not needed
*
* Revision 6.55  2003/04/22 19:38:00  dondosha
* Fix for ungapped search with tabular output
*
* Revision 6.54  2003/01/16 19:46:28  kans
* include accid1.h to fix Mac compiler error on missing symbols
*
* Revision 6.53  2003/01/10 20:55:36  dondosha
* Enable the Entrez client option without a special definition
*
* Revision 6.52  2003/01/07 15:47:11  dondosha
* Hide the -A argument if NCBI_ENTREZ_CLIENT is not defined
*
* Revision 6.51  2002/09/18 18:21:04  camacho
* Fixed memory leak when using the -U option
*
* Revision 6.50  2002/09/13 18:53:26  dondosha
* Corrected the way query and subject deflines are shown in all types of output
*
* Revision 6.49  2002/08/09 19:41:25  camacho
* 1) Added blast version number to command-line options
* 2) Added explanations for some default parameters
*
* Revision 6.48  2002/06/26 21:44:57  dondosha
* Set default gap penalties before checking command line options
*
* Revision 6.47  2002/06/24 18:24:05  dondosha
* Multiple memory freeing bug fixed
*
* Revision 6.46  2002/05/09 15:37:52  dondosha
* Call BLASTOptionNewEx instead of BLASTOptionNew, so megablast defaults are set in a central place
*
* Revision 6.45  2002/05/02 22:18:25  dondosha
* Copy bioseq if getting it from ID1, removing non-residue characters from sequence
*
* Revision 6.44  2002/05/01 16:43:53  dondosha
* Call BLASTOptionSetGapParams instead of setting options->matrix
*
* Revision 6.43  2002/04/29 19:55:26  madden
* Use ARG_FLOAT for db length
*
* Revision 6.42  2002/03/19 23:29:38  dondosha
* Do not increment options->wordsize by 4 for megablast any more
*
* Revision 6.41  2002/03/14 16:11:41  camacho
* Extended BlastTwoSequences to allow comparison between sequence and PSSM
*
* Revision 6.40  2001/07/19 22:05:47  dondosha
* Made db_length option a string, to convert to Int8 value
*
* Revision 6.39  2001/06/21 21:49:26  dondosha
* Destroy all error returns
*
* Revision 6.38  2001/05/11 22:03:21  dondosha
* Do not use fake Bioseqs in case of tabulated output
*
* Revision 6.37  2001/05/09 20:15:31  dondosha
* 1. Made program argument non-optional
* 2. Added -U argument for masking lower case
*
* Revision 6.36  2001/05/02 20:00:05  dondosha
* Create subject SeqLoc with plus strand (not both) when location is given
*
* Revision 6.35  2001/04/16 20:46:00  dondosha
* Improved error message when sequence not found in Entrez
*
* Revision 6.34  2001/04/04 20:30:05  dondosha
* Fixed a typo
*
* Revision 6.33  2001/04/03 21:59:50  dondosha
* Implemented tabulated output for non-megablast bl2seq
*
* Revision 6.32  2001/03/19 22:39:24  dondosha
* Allow location on the first query sequence for megablast
*
* Revision 6.31  2001/03/02 20:30:20  dondosha
* Typo fix
*
* Revision 6.30  2001/02/16 18:45:39  dondosha
* Fixed minor purify errors
*
* Revision 6.29  2001/02/07 21:18:42  dondosha
* Moved the MegaBlastPrintAlignInfo callback to blastool.c
*
* Revision 6.28  2001/01/24 20:51:50  dondosha
* Enabled splitting of the second sequence for 2 sequences with megablast
*
* Revision 6.27  2001/01/23 22:20:04  dondosha
* Do not free subject id in megablast callback
*
* Revision 6.26  2001/01/17 20:34:11  dondosha
* Use the subject id with best accession in the MegaBlastPrintAlignInfo callback
*
* Revision 6.25  2001/01/12 18:28:09  dondosha
* Pass the best accession ids in the SeqLocs in case of partial sequences
*
* Revision 6.24  2001/01/09 20:53:01  dondosha
* Locations start from 1; added handling of erroneous location input
*
* Revision 6.23  2001/01/09 20:16:27  dondosha
* Implemented from-to location options for both sequences in bl2seq
*
* Revision 6.22  2000/12/13 22:28:06  dondosha
* Unlock bioseqs in the end if they were obtained from entrez lookup
*
* Revision 6.21  2000/11/21 15:47:21  dondosha
* Corrected default wordsize for megablast option
*
* Revision 6.20  2000/11/15 22:21:47  dondosha
* Corrected default wordsize for Mega BLAST
*
* Revision 6.19  2000/11/09 15:01:00  dondosha
* Set longest intron length in options in nucleotide coordinates
*
* Revision 6.18  2000/11/08 22:24:07  dondosha
* Enabled new tblastn by adding longest intron option
*
* Revision 6.17  2000/11/08 18:22:46  kans
* includes <mblast.h> for Mac compiler
*
* Revision 6.16  2000/11/02 20:23:27  dondosha
* Allow megablast one-line style output; allow accession or gi input if NCBI_ENTREZ_CLIENT is defined
*
* Revision 6.15  2000/10/30 19:05:44  madden
* Added -Y search space option
*
* Revision 6.14  2000/10/20 20:10:01  dondosha
* Revert previous change - would fail on some platforms
*
* Revision 6.13  2000/10/20 19:45:16  dondosha
* Changed -d option type to string to allow 8-byte integer db sizes
*
* Revision 6.12  2000/10/13 20:32:32  madden
* Added call to AcknowledgeBlastQuery
*
* Revision 6.11  2000/10/06 21:37:08  dondosha
* Set Mega BlAST default parameters correctly before calling the engine
*
* Revision 6.10  2000/10/05 22:44:28  dondosha
* Set block_width to 0 for Mega BLAST - it has different meaning
*
* Revision 6.9  2000/10/03 15:16:12  madden
* Set program name in call to BlastTwoSequencesEx
*
* Revision 6.8  2000/09/12 21:48:41  dondosha
* Pass the correct scoring matrix to ShowTextAlignFromAnnot
*
* Revision 6.7  2000/06/15 15:29:57  dondosha
* Fixed several memory leaks; tblastn and tblastx enabled
*
* Revision 6.6  2000/05/24 20:36:08  dondosha
* If megablast is used, set cutoff_s and cutoff_s2 parameters
*
* Revision 6.5  2000/04/10 15:23:33  dondosha
* Added option to use MegaBlast for search
*
* Revision 6.2  1999/11/26 20:16:11  vakatov
* Added <sqnutils.h> to pick up proto of 'UseLocalAsnloadDataAndErrMsg()'
*
* Revision 6.1  1999/07/06 18:48:20  madden
* Compares two sequences
*/

#include <ncbi.h>
#include <objseq.h>
#include <objsset.h>
#include <sequtil.h>
#include <seqport.h>
#include <tofasta.h>
#include <blast.h>
#include <blastpri.h>
#include <txalign.h>
#include <sqnutils.h>
#include <mblast.h>
#include <accid1.h>

/* Used by the callback function. */
FILE *global_fp=NULL;

#define LOCAL_BUFLEN 255
static BioseqPtr
BioseqFromAccession(CharPtr accver, Int2 id_num, Boolean is_na, 
                    Boolean believe_defline)
{
   CharPtr accession, new_defline, version_str;
   Int4 version=0, gi, number, title_length, id_length;
   SeqIdPtr sip = NULL;
   TextSeqIdPtr tsip;
   PDBSeqIdPtr  psip;
   BioseqPtr bsp = NULL, bsp_tmp = NULL;
   ObjectIdPtr  oid;
   SeqPortPtr spp;
   Int2 retval, buf_length=512;
   Uint1 buf[512];
   Char tmp[LOCAL_BUFLEN];

   if (!ID1BioseqFetchEnable ("bl2seq", TRUE))
      ErrPostEx(SEV_FATAL, 1, 0, 
                "Entrez access interface currently unavailable\n");
   if (!IS_DIGIT(*accver)) {
      accession = StringTokMT(accver, ".", &version_str);
      if (version_str)
         version = atoi(version_str);
      
      if((sip = ValNodeNew (NULL)) == NULL)
         return NULL;
      if((tsip = TextSeqIdNew ()) == NULL)
         return NULL;
      
      tsip->accession = StringSave(accession);
      tsip->version = version;
      /* GenBank, EMBL, and DDBJ. */
      sip->choice = SEQID_GENBANK;
      sip->data.ptrvalue = (Pointer) tsip;
      gi = ID1FindSeqId (sip);
      
      if (gi == 0) {
         /* SwissProt. */
         sip->choice = SEQID_SWISSPROT;
         gi = ID1FindSeqId (sip);
      }
      if (gi == 0) {
        /* PIR */
         sip->choice = SEQID_PIR;
         gi = ID1FindSeqId (sip);
      }
      
      if (gi == 0) {
         /* PRF */
         sip->choice = SEQID_PRF;
         gi = ID1FindSeqId (sip);
      }
      
      if (gi == 0) {
         /* OTHER, probably 'ref' */
         sip->choice = SEQID_OTHER;
         gi = ID1FindSeqId (sip);
      }
      
      if(gi == 0) {
         /* OK. We failed to find gi using string as TextSeqId. Now trying
            last time - with PDBSeqIdPtr */
         
         if((psip = PDBSeqIdNew()) == NULL)
            return NULL;
         
         sip->choice = SEQID_PDB;
         tsip = TextSeqIdFree(tsip);
         sip->data.ptrvalue = psip;
         
         psip->mol = accession;
         psip->chain = version;
         
         gi = ID1FindSeqId (sip);
      }

      if (gi == 0) {
         ErrPostEx(SEV_WARNING, 0, 0, "Sequence %s not found\n", accver);
         return NULL;
      }
      sip = SeqIdFree(sip);
   } else
      gi = atoi(accver);

   if (gi > 0) {
      ValNodeAddInt(&sip, SEQID_GI, gi);
      if ((bsp_tmp = BioseqLockById(sip)) == NULL) {
         ErrPostEx(SEV_WARNING, 0, 0, "Gi %ld not found", gi);
         return NULL;
      }
   } 
   
   if (ISA_na(bsp_tmp->mol) != is_na) {
      BioseqUnlock(bsp_tmp);
      if (is_na)
         ErrPostEx(SEV_FATAL, 1, 0, 
                   "%s is a protein sequence, program requires nucleotide", 
                   accver);
      else
         ErrPostEx(SEV_FATAL, 1, 0, 
                   "%s is a nucleotide sequence, program requires protein", 
                   accver);
      return NULL;
   }
   
   bsp = BioseqNew();
   bsp->length = bsp_tmp->length;
   bsp->mol = bsp_tmp->mol;
   bsp->repr = Seq_repr_raw;
   bsp->seq_data = BSNew(bsp->length);
                   
   if (ISA_na(bsp->mol)) {
      spp = SeqPortNew(bsp_tmp, 0, -1, Seq_strand_plus, 
                       Seq_code_iupacna);
      bsp->seq_data_type = Seq_code_iupacna;
   } else {
      spp = SeqPortNew(bsp_tmp, 0, -1, Seq_strand_unknown, 
                       Seq_code_ncbieaa);
      bsp->seq_data_type = Seq_code_ncbieaa;
   }
   
   SeqPortSet_do_virtual(spp, TRUE);
   number = 0;
   
   bsp->repr = Seq_repr_raw;
   bsp->seq_data = BSNew(bsp->length);
   
   while (number < bsp->length) {
      retval = SeqPortRead(spp, buf, buf_length);
      if (retval < 0)
         continue;
      if (retval == 0)
         break;
      BSWrite(bsp->seq_data, buf, retval);
      number += retval;
   }
   
   SeqPortFree(spp);
   if (believe_defline) {
      new_defline = StringSave(BioseqGetTitle(bsp_tmp));
   } else {
      title_length = StringLen(BioseqGetTitle(bsp_tmp));
      SeqIdWrite(bsp_tmp->id, tmp, PRINTID_FASTA_LONG, LOCAL_BUFLEN);
      id_length = StringLen(tmp);
      title_length += id_length;
      title_length +=3;
      new_defline = (CharPtr) MemNew(title_length*sizeof(Char));
      StringCpy(new_defline, tmp);
      *(new_defline+id_length) = ' ';
      StringCpy(new_defline+id_length+1, BioseqGetTitle(bsp_tmp));
      *(new_defline+title_length-1) = NULLB;
   }

   bsp->descr = ValNodeAddStr(NULL, Seq_descr_title, new_defline);
   bsp->id = ValNodeNew(NULL);
   oid = ObjectIdNew();
   oid->str = (char*) Malloc(LOCAL_BUFLEN+1);
   if (believe_defline) {
      SeqIdWrite(bsp_tmp->id, oid->str, PRINTID_FASTA_LONG, LOCAL_BUFLEN);
   } else {
      sprintf(oid->str, "%d", id_num);
   }
   bsp->id->choice = SEQID_LOCAL;
   bsp->id->data.ptrvalue = (Pointer) oid;
   SeqMgrDeleteFromBioseqIndex(bsp_tmp);
   BioseqUnlock(bsp_tmp);
   BioseqPack(bsp);
   ID1BioseqFetchDisable();
   return bsp;
}
		
#define NUMARG (sizeof(myargs)/sizeof(myargs[0]))

static Args myargs [] = {
  { "First sequence",            
	NULL, NULL, NULL, FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL}, /* 0 */
  { "Second sequence",
	NULL, NULL, NULL, FALSE, 'j', ARG_FILE_IN, 0.0, 0, NULL}, /* 1 */
  { "Program name: blastp, blastn, blastx, tblastn, tblastx. For blastx 1st sequence should be nucleotide, tblastn 2nd sequence nucleotide",
        NULL, NULL, NULL, FALSE, 'p', ARG_STRING, 0.0, 0, NULL},/* 2 */
  { "Gapped",
        "T", NULL, NULL, FALSE, 'g', ARG_BOOLEAN, 0.0, 0, NULL},/* 3 */
  { "alignment output file",
	"stdout", NULL, NULL, FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},/* 4 */
  { "theor. db size (zero is real size)", 
	"0", NULL, NULL, FALSE, 'd', ARG_FLOAT, 0.0, 0, NULL},/* 5 */
  { "SeqAnnot output file",
	NULL, NULL, NULL, TRUE, 'a', ARG_FILE_OUT, 0.0, 0, NULL},/* 6 */
  { "Cost to open a gap (-1 invokes default behavior; non-default values can result in unreliable statistics)",
        "-1", NULL, NULL, FALSE, 'G', ARG_INT, 0.0, 0, NULL},/* 7 */
  { "Cost to extend a gap (-1 invokes default behavior; non-default values can result in unreliable statistics)",
        "-1", NULL, NULL, FALSE, 'E', ARG_INT, 0.0, 0, NULL},/* 8 */
  { "X dropoff value for gapped alignment (in bits) (zero invokes default "
    "behavior)\n      blastn 30, megablast 20, tblastx 0, all others 15",
        "0", NULL, NULL, FALSE, 'X', ARG_INT, 0.0, 0, NULL},/* 9 */
  { "Word size, default if zero (blastn 11, megablast 28, "
        "all others 3)",
        "0", NULL, NULL, FALSE, 'W', ARG_INT, 0.0, 0, NULL},/* 10 */
  { "Matrix",
        "BLOSUM62", NULL, NULL, FALSE, 'M', ARG_STRING, 0.0, 0, NULL},/* 11 */
  { "Penalty for a nucleotide mismatch (blastn only)",
        "-3", NULL, NULL, FALSE, 'q', ARG_INT, 0.0, 0, NULL},/* 12 */
  { "Reward for a nucleotide match (blastn only)",
        "1", NULL, NULL, FALSE, 'r', ARG_INT, 0.0, 0, NULL},/* 13 */
  { "Filter query sequence (DUST with blastn, SEG with others)",
        "T", NULL, NULL, FALSE, 'F', ARG_STRING, 0.0, 0, NULL},/* 14 */
  { "Expectation value (E)",
        "10.0", NULL, NULL, FALSE, 'e', ARG_FLOAT, 0.0, 0, NULL},/* 15 */
  { "Query strands to search against database (blastn only).  3 is both, 1 is top, 2 is bottom",
        "3", NULL, NULL, FALSE, 'S', ARG_INT, 0.0, 0, NULL},/* 16 */
  { "Produce HTML output",
        "F", NULL, NULL, FALSE, 'T', ARG_BOOLEAN, 0.0, 0, NULL},/* 17 */
  { "Use Mega Blast for search",
        "F", NULL, NULL, TRUE, 'm', ARG_BOOLEAN, 0.0, 0, NULL},/* 18 */
  { "Effective length of the search space (use zero for the real size)",
        "0", NULL, NULL, FALSE, 'Y', ARG_FLOAT, 0.0, 0, NULL}, /* 19 */
  { "Length of the largest intron allowed in tblastn for linking HSPs",
        "0", NULL, NULL, FALSE, 't', ARG_INT, 0.0, 0, NULL},/* 20 */
  { "Location on first sequence",
        NULL, NULL, NULL, TRUE, 'I', ARG_STRING, 0.0, 0, NULL},/* 21 */
  { "Location on second sequence",
        NULL, NULL, NULL, TRUE, 'J', ARG_STRING, 0.0, 0, NULL},/* 22 */
  { "Output format: 0 - traditional, 1 - tabular", 
        "0", NULL, NULL, FALSE, 'D', ARG_INT, 0.0, 0, NULL}, /* 23 */
  { "Use lower case filtering for the query sequence",
        "F", NULL, NULL, TRUE, 'U', ARG_BOOLEAN, 0.0, 0, NULL},/* 24 */
  { "Input sequences in the form of accession.version",
        "F", NULL, NULL, FALSE, 'A', ARG_BOOLEAN, 0.0, 0, NULL} /* 25 */
};

Int2 Main (void)
 
{
	
	AsnIoPtr aip;
	BioseqPtr fake_bsp = NULL, fake_subject_bsp = NULL, query_bsp = NULL, 
                  subject_bsp = NULL;
        BioseqPtr bsp1, bsp2;
	BLAST_KarlinBlkPtr ka_params=NULL, ka_params_gap=NULL;
	BLAST_OptionsBlkPtr options;
	Boolean seq1_is_na, seq2_is_na;
	CharPtr params_buffer=NULL;
        DbtagPtr        dbtagptr;
	Uint1 align_type;
	Uint4 align_options;
	SeqAlignPtr  seqalign;
        SeqAnnotPtr seqannot;
	SeqEntryPtr sep = NULL, sep1 = NULL;
	CharPtr blast_inputfile, blast_inputfile1, program_name, blast_outputfile;
	FILE *infp, *infp1, *outfp;
	ValNodePtr  mask_loc, mask_loc_start, vnp, other_returns=NULL, error_returns=NULL;
	BLAST_MatrixPtr matrix;
        Int4Ptr PNTR txmatrix;
        int (LIBCALLBACK *handle_results)PROTO((VoidPtr search)) = NULL;
        Boolean entrez_lookup = FALSE;
        CharPtr query_accver = NULL, subject_accver = NULL;
        Char buf[256] = { '\0' };
        Boolean html, seqannot_output, believe_query;
        Uint1 tabular_output;
        Boolean gapped_calculation;

    StringCpy(buf, "bl2seq ");
    StringNCat(buf, BlastGetVersionNumber(), sizeof(buf)-StringLen(buf)-1);
    if (! GetArgs (buf, NUMARG, myargs)) {
        return (1);
    }

	UseLocalAsnloadDataAndErrMsg ();

        if (! SeqEntryLoad())
                return 1;

	ErrSetMessageLevel(SEV_WARNING);
        entrez_lookup = (Boolean) myargs[25].intvalue;
        html = (Boolean) myargs[17].intvalue;
        seqannot_output = (myargs[6].strvalue != NULL);

        if (entrez_lookup) {
           query_accver = myargs [0].strvalue;
           subject_accver = myargs [1].strvalue;
        } else {
           blast_inputfile = myargs [0].strvalue;
           blast_inputfile1 = myargs [1].strvalue;
        }
        blast_outputfile = myargs [4].strvalue;

	program_name = StringSave(myargs[2].strvalue);
	if (StringCmp(program_name, "blastn") && 
	    StringCmp(program_name, "blastp") && 
	    StringCmp(program_name, "blastx") && 
	    StringCmp(program_name, "tblastn") && 
	    StringCmp(program_name, "tblastx")) {
		ErrPostEx(SEV_FATAL, 1, 0, "Program name must be blastn, blastp, blastx, tblastn or tblastx\n");
		return (1);
	}
	   
	align_type = BlastGetTypes(program_name, &seq1_is_na, &seq2_is_na);

	if (!entrez_lookup && (infp = FileOpen(blast_inputfile, "r")) == NULL)
	{
		ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open input file %s\n", blast_inputfile);
		return (1);
	}

	if (!entrez_lookup && (infp1 = FileOpen(blast_inputfile1, "r")) == NULL)
	{
		ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open input file %s\n", blast_inputfile1);
		return (1);
	}

	if ((outfp = FileOpen(blast_outputfile, "w")) == NULL)
	{
		ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open output file %s\n", blast_outputfile);
		return (1);
	}

        global_fp = outfp;
        gapped_calculation = (Boolean) myargs[3].intvalue;

	options = BLASTOptionNewEx(program_name, gapped_calculation,
                                   (Boolean) myargs[18].intvalue);
        tabular_output = (Uint1) myargs[23].intvalue; 
        believe_query = (seqannot_output || entrez_lookup); 
        if (entrez_lookup) {
           query_bsp = BioseqFromAccession(query_accver, 1, seq1_is_na, TRUE);
        } else {
           if (myargs[24].intvalue)
              sep = FastaToSeqEntryForDb(infp, seq1_is_na, NULL, 
                                         believe_query, NULL, NULL, 
                                         &options->query_lcase_mask);
           else
              sep = FastaToSeqEntryEx(infp, seq1_is_na, NULL, believe_query);

           if (sep != NULL) {
              query_bsp = NULL;
              if (seq1_is_na)
                 SeqEntryExplore(sep, &query_bsp, FindNuc);
              else
                 SeqEntryExplore(sep, &query_bsp, FindProt);

           }
        }
        if (query_bsp == NULL) {
           ErrPostEx(SEV_FATAL, 1, 0, "Unable to obtain bioseq\n");
           return 2;
        }

        if (!believe_query)
           fake_bsp = BlastMakeFakeBioseq(query_bsp, NULL);
        if (entrez_lookup) {
           subject_bsp = 
              BioseqFromAccession(subject_accver, 2, seq2_is_na, FALSE);
        } else {
           /*if (believe_query || html != 0)
             sep1 = FastaToSeqEntry(infp1, seq2_is_na);
             else*/
           sep1 = FastaToSeqEntryEx(infp1, seq2_is_na, NULL, FALSE);
           
           if (sep1 != NULL) {
              subject_bsp = NULL;
              if (seq2_is_na)
                 SeqEntryExplore(sep1, &subject_bsp, FindNuc);
              else
                 SeqEntryExplore(sep1, &subject_bsp, FindProt);
              
           }
        }
        
        if (subject_bsp == NULL) {
           ErrPostEx(SEV_FATAL, 1, 0, "Unable to obtain bioseq\n");
           return 2;
        }

        fake_subject_bsp = BioseqNew();
        fake_subject_bsp->descr = subject_bsp->descr;
        fake_subject_bsp->repr = subject_bsp->repr;
        fake_subject_bsp->mol = subject_bsp->mol;
        fake_subject_bsp->length = subject_bsp->length;
        fake_subject_bsp->seq_data = subject_bsp->seq_data;
        fake_subject_bsp->seq_data_type = subject_bsp->seq_data_type;
        dbtagptr = DbtagNew();
        dbtagptr->db = StringSave("BL_ORD_ID");
        dbtagptr->tag = ObjectIdNew();
        dbtagptr->tag->str = StringSave(BioseqGetTitle(subject_bsp));
        ValNodeAddPointer(&fake_subject_bsp->id, SEQID_GENERAL, dbtagptr);

    	if (myargs[19].floatvalue)
           options->searchsp_eff = (Nlm_FloatHi) myargs[19].floatvalue;


	options->filter_string = StringSave(myargs[14].strvalue);
	options->expect_value  = (Nlm_FloatHi) myargs [15].floatvalue;

        if (StringICmp("blastn", program_name) == 0)
        {
                options->penalty = myargs[12].intvalue;
                options->reward = myargs[13].intvalue;
        }

	options->db_length = (Int8) myargs[5].floatvalue;

	options->discontinuous = FALSE;

        if (myargs[9].intvalue != 0)
	{
               options->gap_x_dropoff = myargs[9].intvalue;
	}
        if (myargs[10].intvalue != 0)
               options->wordsize = (Int2) myargs[10].intvalue;

	if (options->is_megablast_search) {
	   options->cutoff_s2 = options->wordsize*options->reward;
        }
	options->matrix = MemFree(options->matrix);
        BLASTOptionSetGapParams(options, myargs[11].strvalue, 0, 0); 

        if (myargs[7].intvalue != -1)
              options->gap_open = myargs[7].intvalue;
        if (myargs[8].intvalue != -1)
               options->gap_extend = myargs[8].intvalue;

	options->strand_option = myargs[16].intvalue;

        /* Input longest intron length is in nucleotide scale; in the lower 
           level code it will be used in protein scale */
        if (myargs[20].intvalue > 0) 
           options->longest_intron = 
              MAX(myargs[20].intvalue, MAX_INTRON_LENGTH);

        if (believe_query) {
           bsp1 = query_bsp;
        } else {
           bsp1 = fake_bsp;
        }

        bsp2 = fake_subject_bsp;

        if (!myargs[21].strvalue && !myargs[22].strvalue) {
           seqalign = BlastTwoSequencesWithCallback(bsp1, bsp2, program_name, 
              options, &other_returns, &error_returns, handle_results);
        } else {
           /* Location(s) are provided */
           CharPtr delimiters = " ,;";
           CharPtr location;
           Int4 from, to;
           SeqLocPtr slp1 = NULL, slp2 = NULL;
           location = myargs[21].strvalue;
           if (location) {
              from = atoi(StringTokMT(location, delimiters, &location)) - 1;
              to = atoi(location) - 1;

              from = MAX(from, 0);
              if (to < 0) 
                 to = bsp1->length - 1;
              to = MIN(to, bsp1->length - 1);
              if (from >= bsp1->length) {
                 ErrPostEx(SEV_FATAL, 1, 0, 
                           "Location outside of the first sequence range\n");
                 return 3;
              }
              slp1 = SeqLocIntNew(from, to, options->strand_option,
                                  SeqIdFindBestAccession(bsp1->id));
           } else
              ValNodeAddPointer(&slp1, SEQLOC_WHOLE, SeqIdDup(SeqIdFindBestAccession(bsp1->id)));

           location = myargs[22].strvalue;
           if (location) {
              from = atoi(StringTokMT(location, delimiters, &location)) - 1;
              to = atoi(location) - 1;

              from = MAX(from, 0);
              if (to < 0) 
                 to = bsp2->length - 1;
              to = MIN(to, bsp2->length - 1);
              if (from >= bsp2->length) {
                 ErrPostEx(SEV_FATAL, 1, 0, 
                           "Location outside of the second sequence range\n");
                 return 3;
              }
              slp2 = SeqLocIntNew(from, to, Seq_strand_plus, SeqIdFindBestAccession(bsp2->id));
           } else
              ValNodeAddPointer(&slp2, SEQLOC_WHOLE, SeqIdDup(SeqIdFindBestAccession(bsp2->id)));
           
           seqalign = BlastTwoSequencesByLocWithCallback(slp1, slp2, program_name, options, &other_returns, &error_returns, handle_results, NULL);
           SeqLocFree(slp1);
           SeqLocFree(slp2);
        }

        if (error_returns) {
           BlastErrorPrint(error_returns);
           for (vnp = error_returns; vnp; vnp = vnp->next) {
              BlastDestroyErrorMessage((BlastErrorMsgPtr)vnp->data.ptrvalue);
           }
           ValNodeFree(error_returns);
        }
       
        ka_params = NULL;
        ka_params_gap = NULL;
        params_buffer = NULL;
        mask_loc = NULL;
        matrix = NULL;
        txmatrix = NULL;
        for (vnp=other_returns; vnp; vnp = vnp->next) {
           switch (vnp->choice) {
           case TXKABLK_NOGAP:
              ka_params = vnp->data.ptrvalue;
              break;
           case TXKABLK_GAP:
              ka_params_gap = vnp->data.ptrvalue;
              break;
           case TXPARAMETERS:
              params_buffer = vnp->data.ptrvalue;
              break;
           case TXMATRIX:
              matrix = vnp->data.ptrvalue;
              if (matrix && !tabular_output)
                 txmatrix = BlastMatrixToTxMatrix(matrix);
              break;
           case SEQLOC_MASKING_NOTSET:
           case SEQLOC_MASKING_PLUS1:
           case SEQLOC_MASKING_PLUS2:
           case SEQLOC_MASKING_PLUS3:
           case SEQLOC_MASKING_MINUS1:
           case SEQLOC_MASKING_MINUS2:
           case SEQLOC_MASKING_MINUS3:
              ValNodeAddPointer(&mask_loc, vnp->choice, vnp->data.ptrvalue);
              break;
           default:
              break;
           }
        }	
        if (!tabular_output || seqannot_output) {
           align_options = 0;
           align_options += TXALIGN_MATRIX_VAL;
           align_options += TXALIGN_SHOW_QS;
           align_options += TXALIGN_COMPRESS;
           align_options += TXALIGN_END_NUM;
           if (StringICmp("blastx", program_name) == 0) {
              align_options += TXALIGN_BLASTX_SPECIAL;
           }
           
           if (html)
              align_options += TXALIGN_HTML;

           seqannot = SeqAnnotNew();
           seqannot->type = 2;
           AddAlignInfoToSeqAnnot(seqannot, align_type);
           seqannot->data = seqalign;
           aip = NULL;
           if (seqannot_output)
              aip = AsnIoOpen (myargs[6].strvalue,"w");
           
           if (aip && seqannot) {
              SeqAnnotAsnWrite((SeqAnnotPtr) seqannot, aip, NULL);
              AsnIoReset(aip);
              aip = AsnIoClose(aip);
           }
        }
        if (!tabular_output) {    
           AcknowledgeBlastQuery(query_bsp, 70, outfp, believe_query, html);
           ShowTextAlignFromAnnot(seqannot, 60, outfp, NULL, NULL, align_options, txmatrix, mask_loc, FormatScoreFunc);
           
           seqannot = SeqAnnotFree(seqannot);
           if (txmatrix)
              txmatrix = TxMatrixDestruct(txmatrix);
           init_buff_ex(85);
        
           if (ka_params) {
              PrintKAParameters(ka_params->Lambda, ka_params->K, ka_params->H, 70, outfp, FALSE);
           }
        
           if (ka_params_gap) {
              PrintKAParameters(ka_params_gap->Lambda, ka_params_gap->K, ka_params_gap->H, 70, outfp, TRUE);
           }
        
           PrintTildeSepLines(params_buffer, 70, outfp);
           free_buff();
        } else {
           PrintTabularOutputHeader(NULL, query_bsp, NULL, 
              program_name, 0, believe_query, outfp);

           BlastPrintTabulatedResults(seqalign, query_bsp, NULL, 
              1, program_name, !gapped_calculation,
              believe_query, 0, 0, outfp, FALSE);
           SeqAlignSetFree(seqalign);
        }

        matrix = BLAST_MatrixDestruct(matrix);
        MemFree(ka_params);
        MemFree(ka_params_gap);
        MemFree(params_buffer);
    
        mask_loc_start = mask_loc;
        while (mask_loc) {
           SeqLocSetFree(mask_loc->data.ptrvalue);
           mask_loc = mask_loc->next;
        }
        ValNodeFree(mask_loc_start);
        
        fake_bsp = BlastDeleteFakeBioseq(fake_bsp);

        other_returns = ValNodeFree(other_returns);
    options->query_lcase_mask = SeqLocSetFree(options->query_lcase_mask);
	options = BLASTOptionDelete(options);
	MemFree(program_name);
	FileClose(outfp);

        if (entrez_lookup) {
           BioseqFree(query_bsp);
           BioseqFree(subject_bsp);
        } else {
           SeqEntryFree(sep);
           SeqEntryFree(sep1);
        }
	return 0;
}
	

