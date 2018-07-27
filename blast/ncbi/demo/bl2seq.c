static char const rcsid[] = "$Id: bl2seq.c,v 6.75 2005/03/16 00:43:40 dondosha Exp $";

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
* Revision 6.75  2005/03/16 00:43:40  dondosha
* Correction to previous commit to make reported deflines the same as before
*
* Revision 6.74  2005/03/15 20:59:16  dondosha
* When retrieving Bioseq by gi, try BLAST databases first
*
* Revision 6.73  2005/03/03 15:05:47  dondosha
* Blast_FindRepeatFilterLoc renamed to Blast_FindRepeatFilterSeqLoc
*
* Revision 6.72  2005/02/08 20:43:03  dondosha
* Added repeats filtering for new engine
*
* Revision 6.71  2005/02/07 15:30:39  dondosha
* Removed restriction on the value of longest intron option
*
* Revision 6.70  2005/02/03 18:02:07  dondosha
* Pass summary returns to BLAST_FormatResults, needed for XML output
*
* Revision 6.69  2005/02/02 19:01:36  dondosha
* Use new high level API for performing the search
*
* Revision 6.68  2004/12/29 16:06:20  dondosha
* Small memory leak fix; correction in mask array allocation for non-translated search
*
* Revision 6.67  2004/12/29 15:20:55  dondosha
* Set gapped_calculation option for new engine; allocate appropriate size BlastSeqLoc array in BlastMaskLoc before formatting
*
* Revision 6.66  2004/10/04 14:05:06  madden
* Use Blast_PrintOutputFooter rather than BLAST_PrintSummaryReturns
*
* Revision 6.65  2004/09/13 15:15:54  madden
* Changes for BlastSeqLoc and BlastMaskLoc
*
* Revision 6.64  2004/08/16 14:26:47  madden
* Fix bad frees for blastx filtering locations
*
* Revision 6.63  2004/08/09 13:08:15  madden
* Bl2seq now can run queries with new blast engine
*
* Revision 6.62  2004/08/03 14:02:02  madden
* Introduce enum to make myargs more readable
*
* Revision 6.61  2004/06/30 16:03:48  kans
* include <blfmtutl.h>
*
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
#include <blfmtutl.h>

#include <algo/blast/api/twoseq_api.h>
#include <algo/blast/api/blast_format.h>
#include <algo/blast/api/blast_seq.h>
#include <algo/blast/api/repeats_filter.h>
#include <algo/blast/core/blast_util.h>


/* Used by the callback function. */
FILE *global_fp=NULL;

#define LOCAL_BUFLEN 255
static BioseqPtr
BioseqFromAccession(CharPtr accver, Boolean is_query, Boolean is_na)
{
   CharPtr accession, version_str;
   Int4 version=0, gi, number;
   SeqIdPtr sip = NULL;
   TextSeqIdPtr tsip;
   PDBSeqIdPtr  psip;
   BioseqPtr bsp = NULL, bsp_tmp = NULL;
   SeqPortPtr spp;
   Int2 retval, buf_length=512;
   Uint1 buf[512];
   char* defline;
   char* dummy_ptr = NULL;

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

   ID1BioseqFetchDisable();

   if (gi > 0) {
       /* First attempt to retrieve Bioseq from BLAST databases. */
       char* db_name = (is_na ? "nucl_dbs" : "prot_dbs");

       ValNodeAddInt(&sip, SEQID_GI, gi);
       ReadDBBioseqFetchEnable ("bl2seq", db_name, is_na, TRUE);
       bsp_tmp = BioseqLockById(sip);
       ReadDBBioseqFetchDisable();

       if (!bsp_tmp) {
           /* Try ID1 again as a last resort. */
           ID1BioseqFetchEnable("bl2seq", TRUE);
           bsp_tmp = BioseqLockById(sip);
           ID1BioseqFetchDisable();
       }
       sip = SeqIdFree(sip);
   }
   
   if (!bsp_tmp) {
       ErrPostEx(SEV_WARNING, 0, 0, "Gi %ld not found", gi);
       return NULL;
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
   defline = StringSave(BioseqGetTitle(bsp_tmp));
   /* Cut off any defline extensions due to combining of sequences in the BLAST
      database. */
   defline = StringTokMT(defline, ">", &dummy_ptr);
   /* If there did exist a second defline, then there is an extra space in front
      of the ">" sign, which should be removed. */
   if (dummy_ptr)
       defline[strlen(defline)-1] = NULLB;

   if (is_query) {
       ObjectIdPtr oid = ObjectIdNew();
       oid->str = (char*) Malloc(LOCAL_BUFLEN+1);
       SeqIdWrite(bsp_tmp->id, oid->str, PRINTID_FASTA_LONG, LOCAL_BUFLEN);
       ValNodeAddPointer(&bsp->id, SEQID_LOCAL, oid);
       bsp->descr = ValNodeAddStr(NULL, Seq_descr_title, defline);
   } else {
       Char seqid_buf[LOCAL_BUFLEN];
       char* title;
       DbtagPtr dbtagptr = DbtagNew();
       dbtagptr->db = StringSave("BL_ORD_ID");
       dbtagptr->tag = ObjectIdNew();
       ValNodeAddPointer(&bsp->id, SEQID_GENERAL, dbtagptr);
       
       SeqIdWrite(bsp_tmp->id, seqid_buf, PRINTID_FASTA_LONG, 
                  LOCAL_BUFLEN);
       
       title = (char*) Malloc(strlen(seqid_buf) + strlen(defline) + 2);
       sprintf(title, "%s %s", seqid_buf, defline);
       defline = MemFree(defline);
       bsp->descr = ValNodeAddStr(NULL, Seq_descr_title, title);
   }
   SeqMgrDeleteFromBioseqIndex(bsp_tmp);
   BioseqUnlock(bsp_tmp);
   BioseqPack(bsp);
   return bsp;
}
		
#define NUMARG (sizeof(myargs)/sizeof(myargs[0]))

typedef enum {
   ARG_QUERY = 0,
   ARG_SUBJECT,
   ARG_PROGRAM,
   ARG_GAPPED,
   ARG_OUT,
   ARG_DBSIZE,
   ARG_ASNOUT,
   ARG_GAPOPEN,
   ARG_GAPEXT,
   ARG_XDROP,
   ARG_WORDSIZE,
   ARG_MATRIX,
   ARG_MISMATCH,
   ARG_MATCH,
   ARG_FILTER,
   ARG_EVALUE,
   ARG_STRAND,
   ARG_HTML,
   ARG_USEMEGABLAST,
   ARG_SEARCHSP,
   ARG_INTRON,
   ARG_LOC1,
   ARG_LOC2,
   ARG_FORMAT,
   ARG_LCASE,
   ARG_ACCN,
   ARG_FORCE_OLD
} BlastArguments;

static Args myargs [] = {
  { "First sequence",            
	NULL, NULL, NULL, FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL}, /* ARG_QUERY */
  { "Second sequence",
	NULL, NULL, NULL, FALSE, 'j', ARG_FILE_IN, 0.0, 0, NULL}, /* ARG_SUBJECT */
  { "Program name: blastp, blastn, blastx, tblastn, tblastx. For blastx 1st sequence should be nucleotide, tblastn 2nd sequence nucleotide",
        NULL, NULL, NULL, FALSE, 'p', ARG_STRING, 0.0, 0, NULL}, /* ARG_PROGRAM */
  { "Gapped",
        "T", NULL, NULL, FALSE, 'g', ARG_BOOLEAN, 0.0, 0, NULL}, /* ARG_GAPPED */
  { "alignment output file",
	"stdout", NULL, NULL, FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL}, /* ARG_OUT */
  { "theor. db size (zero is real size)", 
	"0", NULL, NULL, FALSE, 'd', ARG_FLOAT, 0.0, 0, NULL}, /* ARG_DBSIZE */
  { "SeqAnnot output file",
	NULL, NULL, NULL, TRUE, 'a', ARG_FILE_OUT, 0.0, 0, NULL}, /* ARG_ASNOUT */
  { "Cost to open a gap (-1 invokes default behavior; non-default values can result in unreliable statistics)",
        "-1", NULL, NULL, FALSE, 'G', ARG_INT, 0.0, 0, NULL}, /* ARG_GAPOPEN */
  { "Cost to extend a gap (-1 invokes default behavior; non-default values can result in unreliable statistics)",
        "-1", NULL, NULL, FALSE, 'E', ARG_INT, 0.0, 0, NULL}, /* ARG_GAPEXT */
  { "X dropoff value for gapped alignment (in bits) (zero invokes default "
    "behavior)\n      blastn 30, megablast 20, tblastx 0, all others 15",
        "0", NULL, NULL, FALSE, 'X', ARG_INT, 0.0, 0, NULL}, /* ARG_XDROP */
  { "Word size, default if zero (blastn 11, megablast 28, "
        "all others 3)",
        "0", NULL, NULL, FALSE, 'W', ARG_INT, 0.0, 0, NULL}, /* ARG_WORDSIZE */
  { "Matrix",
        "BLOSUM62", NULL, NULL, FALSE, 'M', ARG_STRING, 0.0, 0, NULL}, /* ARG_MATRIX */
  { "Penalty for a nucleotide mismatch (blastn only)",
        "-3", NULL, NULL, FALSE, 'q', ARG_INT, 0.0, 0, NULL}, /* ARG_MISMATCH */
  { "Reward for a nucleotide match (blastn only)",
        "1", NULL, NULL, FALSE, 'r', ARG_INT, 0.0, 0, NULL}, /* ARG_MATCH */
  { "Filter query sequence (DUST with blastn, SEG with others)",
        "T", NULL, NULL, FALSE, 'F', ARG_STRING, 0.0, 0, NULL}, /* ARG_FILTER */
  { "Expectation value (E)",
        "10.0", NULL, NULL, FALSE, 'e', ARG_FLOAT, 0.0, 0, NULL}, /* ARG_EVALUE */
  { "Query strands to search against database (blastn only).  3 is both, 1 is top, 2 is bottom",
        "3", NULL, NULL, FALSE, 'S', ARG_INT, 0.0, 0, NULL}, /* ARG_STRAND */
  { "Produce HTML output",
        "F", NULL, NULL, FALSE, 'T', ARG_BOOLEAN, 0.0, 0, NULL}, /* ARG_HTML */
  { "Use Mega Blast for search",
        "F", NULL, NULL, TRUE, 'm', ARG_BOOLEAN, 0.0, 0, NULL}, /* ARG_USEMEGABLAST */
  { "Effective length of the search space (use zero for the real size)",
        "0", NULL, NULL, FALSE, 'Y', ARG_FLOAT, 0.0, 0, NULL}, /* ARG_SEARCHSP */
  { "Length of the largest intron allowed in tblastn for linking HSPs",
        "0", NULL, NULL, FALSE, 't', ARG_INT, 0.0, 0, NULL}, /* ARG_INTRON */
  { "Location on first sequence",
        NULL, NULL, NULL, TRUE, 'I', ARG_STRING, 0.0, 0, NULL}, /* ARG_LOC1 */
  { "Location on second sequence",
        NULL, NULL, NULL, TRUE, 'J', ARG_STRING, 0.0, 0, NULL}, /* ARG_LOC2 */
  { "Output format: 0 - traditional, 1 - tabular", 
        "0", NULL, NULL, FALSE, 'D', ARG_INT, 0.0, 0, NULL}, /* ARG_FORMAT */
  { "Use lower case filtering for the query sequence",
        "F", NULL, NULL, TRUE, 'U', ARG_BOOLEAN, 0.0, 0, NULL}, /* ARG_LCASE */
  { "Input sequences in the form of accession.version",
        "F", NULL, NULL, FALSE, 'A', ARG_BOOLEAN, 0.0, 0, NULL}, /* ARG_ACCN */
  {"Force use of old engine",
        "T", NULL, NULL, TRUE, 'V', ARG_BOOLEAN, 0.0, 0, NULL}  /* ARG_FORCE_OLD */
};

/**
 * Fetches sequences filling in either just the Bioseq's (if fetch from Entrez) or
 * both the BioseqPtr's and the SeqEntryPtr's (if read from FASTA).  The lcase_mask 
 * is also filled in with letters in query that were lower-case if myargs[ARG_LCASE].intvalue
 * is non-zero.
 *
 * @param seq1_is_na the query sequence is DNA if true [in]
 * @param seq2_is_na the subject sequence is DNA if true [in]
 * @param query_bsp pointer to query BioseqPtr, to be filled in [out]
 * @param subject_bsp pointer to subject BioseqPtr, to be filled in [out]
 * @param sep pointer to query SeqEntryPtr, to be filled in [out]
 * @param sep1 pointer to subject SeqEntryPtr, to be filled in [out]
 * @param lcase_mask pointer to lower-case masking data to be filled in [out]
 * @return TRUE on success, FALSE on failure.
*/

static Int4
BL2SEQ_GetSequences(Boolean seq1_is_na, Boolean seq2_is_na, BioseqPtr *query_bsp, BioseqPtr *subject_bsp, 
    SeqEntryPtr *sep, SeqEntryPtr *sep1, SeqLocPtr *lcase_mask, Boolean believe_query)
{
        Boolean entrez_lookup = (Boolean) myargs[ARG_ACCN].intvalue;
        char *query_accver = NULL, *subject_accver = NULL;  /* Used if entrez_lookup. */
	char *blast_inputfile = NULL, *blast_inputfile1 = NULL;  /* Used if FASTA read. */
        
        if (entrez_lookup) {
           query_accver = myargs [ARG_QUERY].strvalue;
           subject_accver = myargs [ARG_SUBJECT].strvalue;
        } else {
           blast_inputfile = myargs [ARG_QUERY].strvalue;
           blast_inputfile1 = myargs [ARG_SUBJECT].strvalue;
        }

        if (entrez_lookup) {
           *query_bsp = BioseqFromAccession(query_accver, TRUE, seq1_is_na);
        } else {
           FILE *infp;
	   if ((infp = FileOpen(blast_inputfile, "r")) == NULL)
	   {
		ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open input file %s\n", blast_inputfile);
		return FALSE;
	   }
           if (myargs[ARG_LCASE].intvalue)
              *sep = FastaToSeqEntryForDb(infp, seq1_is_na, NULL, 
                                         believe_query, NULL, NULL, 
                                         lcase_mask);
           else
              *sep = FastaToSeqEntryEx(infp, seq1_is_na, NULL, believe_query);

           FileClose(infp);

           if (*sep != NULL) {
              *query_bsp = NULL;
              if (seq1_is_na)
                 SeqEntryExplore(*sep, query_bsp, FindNuc);
              else
                 SeqEntryExplore(*sep, query_bsp, FindProt);

           }
        }
        if (*query_bsp == NULL) {
           ErrPostEx(SEV_FATAL, 1, 0, "Unable to obtain bioseq\n");
           return FALSE;
        }

        if (entrez_lookup) {
           *subject_bsp = 
              BioseqFromAccession(subject_accver, FALSE, seq2_is_na);
        } else {
           FILE *infp1;
	   if ((infp1 = FileOpen(blast_inputfile1, "r")) == NULL)
	   {
		ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open input file %s\n", blast_inputfile1);
		return FALSE;
	   }
           *sep1 = FastaToSeqEntryEx(infp1, seq2_is_na, NULL, FALSE);

           FileClose(infp1);

           if (*sep1 != NULL) {
              *subject_bsp = NULL;
              if (seq2_is_na)
                 SeqEntryExplore(*sep1, subject_bsp, FindNuc);
              else
                 SeqEntryExplore(*sep1, subject_bsp, FindProt);
              
           }
        }
        
        if (*subject_bsp == NULL) {
           ErrPostEx(SEV_FATAL, 1, 0, "Unable to obtain bioseq\n");
           return FALSE;
        }

        return TRUE;
}

/**
 * Creates SeqLoc's from the given BioseqPtr's.  if myargs for ARG_LOC1 or ARG_LOC2 are non-NULL, 
 * these are used in the creation of the SeqLoc's.
 *
 * @param bsp1 the query BioseqPtr [in]
 * @param bsp2 the subject BioseqPtr [in]
 * @param slp1 the query SeqLocPtr to be filled in [out]
 * @param slp2 the subject SeqLocPtr to be filled in [out]
 * @param strand_option specifies strand of slp1 [in]
 * @return TRUE on success, FALSE on failure.
*/

static Boolean
BL2SEQ_MakeSeqLoc(const BioseqPtr bsp1, const BioseqPtr bsp2, SeqLocPtr *slp1, SeqLocPtr *slp2, Uint1 strand_option)
{
       const char* k_delimiters = " ,;";
       CharPtr location;
       Int4 from, to;

       *slp1 = NULL;
       *slp2 = NULL;

       location = myargs[ARG_LOC1].strvalue;
       if (location) {
           from = atoi(StringTokMT(location, k_delimiters, &location)) - 1;
           to = atoi(location) - 1;

            from = MAX(from, 0);
            if (to < 0) 
                 to = bsp1->length - 1;
            to = MIN(to, bsp1->length - 1);
            if (from >= bsp1->length) {
                 ErrPostEx(SEV_FATAL, 1, 0, 
                           "Location outside of the first sequence range\n");
                 return FALSE;
            }
            *slp1 = SeqLocIntNew(from, to, strand_option,
                                 SeqIdFindBestAccession(bsp1->id));
        } else if (strand_option != Seq_strand_both) {
            *slp1 = SeqLocIntNew(0, bsp1->length-1, strand_option,
                                 SeqIdFindBestAccession(bsp1->id));
        } else
              ValNodeAddPointer(slp1, SEQLOC_WHOLE, SeqIdDup(SeqIdFindBestAccession(bsp1->id)));

        location = myargs[ARG_LOC2].strvalue;
        if (location) {
            from = atoi(StringTokMT(location, k_delimiters, &location)) - 1;
            to = atoi(location) - 1;

            from = MAX(from, 0);
            if (to < 0) 
                to = bsp2->length - 1;
            to = MIN(to, bsp2->length - 1);
            if (from >= bsp2->length) {
               ErrPostEx(SEV_FATAL, 1, 0, 
                           "Location outside of the second sequence range\n");
               return FALSE;
            }
            *slp2 = SeqLocIntNew(from, to, Seq_strand_plus, SeqIdFindBestAccession(bsp2->id));
         } else
            ValNodeAddPointer(slp2, SEQLOC_WHOLE, SeqIdDup(SeqIdFindBestAccession(bsp2->id)));

       return TRUE;
}

/**
 * Initializes and sets the summary options based upon the command-line args.
 *
 * @param ret_options object to be initialized and filled in [out]
 * @param program_number specifies blastn/blastp/blastx etc. [in]
 * @return TRUE on success, FALSE on failure.
 */
static Boolean
Bl2SEQ_SummaryOptionsSet(BLAST_SummaryOptions* *ret_options, EBlastProgramType program_number)
{
        BLAST_SummaryOptions* options;

        if (BLAST_SummaryOptionsInit(&options) != 0)
        {
                ErrPostEx(SEV_FATAL, 1, 0, "SummaryOptionsInit failed.");
                return FALSE;
        }

        options->hint = eNone;

        switch (program_number) {
            case eBlastTypeBlastn:
               options->program = eBlastn;
               break;
            case eBlastTypeBlastp:
               options->program = eBlastp;
               break;
            case eBlastTypeBlastx:
               options->program = eBlastx;
               break;
            case eBlastTypeTblastn:
               options->program = eTblastn;
               break;
            case eBlastTypeTblastx:
               options->program = eTblastx;
               break;
            default:  
               ErrPostEx(SEV_FATAL, 1, 0, "Program_number (%ld) not valid in Bl2SEQ_SummaryOptionsSet", (long) program_number);
               BLAST_SummaryOptionsFree(options);
               return FALSE;
        }

        options->cutoff_evalue = (Nlm_FloatHi) myargs [ARG_EVALUE].floatvalue;

        if (options->program == eBlastn)
        {
            options->nucleotide_mismatch = myargs[ARG_MISMATCH].intvalue;
            options->nucleotide_match = myargs[ARG_MATCH].intvalue;
            if (myargs[ARG_USEMEGABLAST].intvalue > 0)
                options->use_megablast = TRUE;
        }

        if (myargs[ARG_GAPOPEN].intvalue != -1)
              options->gap_open = myargs[ARG_GAPOPEN].intvalue;

        if (myargs[ARG_GAPEXT].intvalue != -1)
               options->gap_extend = myargs[ARG_GAPEXT].intvalue;

        options->strand = myargs[ARG_STRAND].intvalue;

        if (myargs[ARG_WORDSIZE].intvalue != 0)
               options->word_size = myargs[ARG_WORDSIZE].intvalue;

        if (myargs[ARG_MATRIX].strvalue)
               options->matrix = StringSave(myargs[ARG_MATRIX].strvalue);

        if (myargs[ARG_FILTER].strvalue)
               options->filter_string = StringSave(myargs[ARG_FILTER].strvalue);

        if (myargs[ARG_XDROP].intvalue != 0)
        {
               options->gap_x_dropoff = myargs[ARG_XDROP].intvalue;
        }

        if (program_number != eBlastTypeTblastx)
            options->gapped_calculation = (Boolean) myargs[ARG_GAPPED].intvalue;
        else
            options->gapped_calculation = FALSE;

        options->db_length = myargs[ARG_DBSIZE].floatvalue;

        *ret_options = options;

        return TRUE;
}

Int2 Main_new(void)

{
        BioseqPtr query_bsp=NULL, subject_bsp=NULL;
        BioseqPtr bsp1=NULL, bsp2=NULL;
        BioseqPtr fake_bsp=NULL, fake_subject_bsp=NULL;
        BlastFormattingOptions* format_options;
        BLAST_SummaryOptions* options=NULL;
        Blast_SummaryReturn* extra_returns=NULL;
        Boolean believe_query= FALSE;
        Boolean seq1_is_na, seq2_is_na;  /* seq1/2 is DNA if TRUE. */
        Boolean seqannot_output;   /* SeqAlign will be output. */
        Boolean entrez_lookup;     /* QUery/subject fetched from Entrez. */
        Boolean mask_at_hash=FALSE;  /* masking only on lookup table if TRUE. */
        DbtagPtr        dbtagptr;
        EBlastProgramType program_number;
        Int2 status; /* return value */
        Int4 align_view=0; /* Used for formatting */
        SeqAlignPtr seqalign=NULL;
        SeqEntryPtr sep=NULL, sep1=NULL;
        SeqLocPtr slp1, slp2;   /* Used for actual search. */
        SeqLocPtr filter_loc=NULL;  /* Location of regions filtered (returned by engine) */
        SeqLocPtr lcase_mask=NULL;    /* For lower-case masking info from query FASTA. */
        SeqLoc* repeat_mask = NULL; /* Repeat mask locations */
        Uint1 strand_option = 0; /* FIXME */
        
        strand_option = (Uint1) myargs[ARG_STRAND].intvalue;

        entrez_lookup = (Boolean) myargs[ARG_ACCN].intvalue;
        seqannot_output = (myargs[ARG_ASNOUT].strvalue != NULL);
        believe_query = (seqannot_output || entrez_lookup); 
        if (myargs[ARG_FORMAT].intvalue != 0)
           align_view = 9;  /* Means tabular output. */

        BlastProgram2Number(myargs[ARG_PROGRAM].strvalue, &program_number);

        seq1_is_na = (program_number == eBlastTypeBlastn ||
                  program_number == eBlastTypeBlastx ||
                  program_number == eBlastTypeRpsTblastn ||
                  program_number == eBlastTypeTblastx);

        seq2_is_na = (program_number == eBlastTypeBlastn ||
               program_number == eBlastTypeTblastn ||
               program_number == eBlastTypeTblastx);

        if (BL2SEQ_GetSequences(seq1_is_na, seq2_is_na, &query_bsp, &subject_bsp,
                                &sep, &sep1, &lcase_mask, believe_query) 
            == FALSE)
        {
                ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to get sequences");
                return (1);
        }

        if (!entrez_lookup) {
            if (!believe_query)
                fake_bsp = BlastMakeFakeBioseq(query_bsp, NULL);
            
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
            bsp1 = (believe_query ? query_bsp : fake_bsp);
            bsp2 = fake_subject_bsp;
        } else { /* Query and subject Bioseqs are already "fake". */
            bsp1 = query_bsp;
            bsp2 = subject_bsp;
        }

        if (BL2SEQ_MakeSeqLoc(bsp1, bsp2, &slp1, &slp2, strand_option) == FALSE)
                return 1;

        if (Bl2SEQ_SummaryOptionsSet(&options, program_number) == FALSE)
                return 1;

        /* Find repeat mask, if necessary */
        Blast_FindRepeatFilterSeqLoc(slp1, myargs[ARG_FILTER].strvalue, 
                                  &repeat_mask);
        /* Combine repeat mask with lower case mask */
        if (repeat_mask)
            lcase_mask = ValNodeLink(&lcase_mask, repeat_mask);
        
        status = BLAST_TwoSeqLocSets(options, slp1, slp2, lcase_mask, &seqalign, 
                                     &filter_loc, &mask_at_hash, 
                                     &extra_returns);

        /* Free the lower case mask in SeqLoc form. */
        lcase_mask = Blast_ValNodeMaskListFree(lcase_mask);

        /* Post warning or error messages, no matter what the search status 
           was. */
        Blast_SummaryReturnsPostError(extra_returns);

        if (status != 0)
        {
                ErrPostEx(SEV_FATAL, 1, 0, "BLAST_TwoSeqLocSets failed");
                return status;
        }

        if (myargs[ARG_ASNOUT].strvalue) {
            AsnIoPtr asnout =
               AsnIoOpen(myargs[ARG_ASNOUT].strvalue, (char*)"w");
            GenericSeqAlignSetAsnWrite(seqalign, asnout);
            asnout = AsnIoClose(asnout);
        }

        if ((status = BlastFormattingOptionsNew(program_number, 
                          myargs[ARG_OUT].strvalue, 0, 1, align_view, 
                          &format_options)) != 0)
           return status;

        if (myargs[ARG_HTML].intvalue) {
           format_options->html = TRUE;
           format_options->align_options += TXALIGN_HTML;
           format_options->print_options += TXALIGN_HTML;
        }
        format_options->believe_query = believe_query;

        if (mask_at_hash) {
            /* Masking locations in SeqLoc form are no longer needed */
            filter_loc = Blast_ValNodeMaskListFree(filter_loc);
        }

        /* Format the results */
        status = 
            BLAST_FormatResults(seqalign, NULL, myargs[ARG_PROGRAM].strvalue,
                                1, slp1, filter_loc, format_options, FALSE, NULL,
                                extra_returns);
        
        status = Blast_PrintOutputFooter(program_number, format_options, NULL, extra_returns);

        format_options = BlastFormattingOptionsFree(format_options);
        extra_returns = Blast_SummaryReturnFree(extra_returns);

        if (entrez_lookup) {
           BioseqFree(query_bsp);
           BioseqFree(subject_bsp);
        } else {
           SeqEntryFree(sep);
           SeqEntryFree(sep1);
        }

        options = BLAST_SummaryOptionsFree(options);
        seqalign = SeqAlignSetFree(seqalign);
        slp1 = SeqLocSetFree(slp1);
        slp2 = SeqLocSetFree(slp2);

        fake_bsp = BlastDeleteFakeBioseq(fake_bsp);

        return 0;

}

Int2 Main_old (void)
 
{
	
	AsnIoPtr aip;
	BioseqPtr fake_bsp = NULL, fake_subject_bsp = NULL, query_bsp = NULL, 
                  subject_bsp = NULL;
        BioseqPtr bsp1, bsp2;
	BLAST_KarlinBlkPtr ka_params=NULL, ka_params_gap=NULL;
	BLAST_OptionsBlkPtr options=NULL;
	Boolean seq1_is_na, seq2_is_na;
	CharPtr params_buffer=NULL;
        DbtagPtr        dbtagptr;
	Uint1 align_type;
	Uint4 align_options;
	SeqAlignPtr  seqalign;
        SeqAnnotPtr seqannot;
	SeqEntryPtr sep = NULL, sep1 = NULL;
	CharPtr program_name, blast_outputfile;
	FILE *outfp;
	ValNodePtr  mask_loc, mask_loc_start, vnp, other_returns=NULL, error_returns=NULL;
	BLAST_MatrixPtr matrix;
        Int4Ptr PNTR txmatrix;
        int (LIBCALLBACK *handle_results)PROTO((VoidPtr search)) = NULL;
        Boolean entrez_lookup = FALSE;
        Boolean html, seqannot_output, believe_query;
        Uint1 tabular_output;
        Boolean gapped_calculation;

        entrez_lookup = (Boolean) myargs[ARG_ACCN].intvalue;
        html = (Boolean) myargs[ARG_HTML].intvalue;
        seqannot_output = (myargs[ARG_ASNOUT].strvalue != NULL);

        blast_outputfile = myargs [ARG_OUT].strvalue;

	program_name = StringSave(myargs[ARG_PROGRAM].strvalue);
	if (StringCmp(program_name, "blastn") && 
	    StringCmp(program_name, "blastp") && 
	    StringCmp(program_name, "blastx") && 
	    StringCmp(program_name, "tblastn") && 
	    StringCmp(program_name, "tblastx")) {
		ErrPostEx(SEV_FATAL, 1, 0, "Program name must be blastn, blastp, blastx, tblastn or tblastx\n");
		return (1);
	}
	   
	align_type = BlastGetTypes(program_name, &seq1_is_na, &seq2_is_na);

	if ((outfp = FileOpen(blast_outputfile, "w")) == NULL)
	{
		ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open output file %s\n", blast_outputfile);
		return (1);
	}

        global_fp = outfp;
        gapped_calculation = (Boolean) myargs[ARG_GAPPED].intvalue;
        believe_query = (seqannot_output || entrez_lookup); 

        options = BLASTOptionNewEx(program_name, gapped_calculation,
                                   (Boolean) myargs[ARG_USEMEGABLAST].intvalue);

        if (BL2SEQ_GetSequences(seq1_is_na, seq2_is_na, &query_bsp, &subject_bsp,
                                &sep, &sep1, &(options->query_lcase_mask), 
                                believe_query) == FALSE)
        {
            ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to get sequences");
            return (1);
        }

        if (!entrez_lookup) {
            if (!believe_query)
                fake_bsp = BlastMakeFakeBioseq(query_bsp, NULL);
            
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
            bsp1 = (believe_query ? query_bsp : fake_bsp);
            bsp2 = fake_subject_bsp;
        } else {
            bsp1 = query_bsp;
            bsp2 = subject_bsp;
        }

        tabular_output = (Uint1) myargs[ARG_FORMAT].intvalue; 


    	if (myargs[ARG_SEARCHSP].floatvalue)
           options->searchsp_eff = (Nlm_FloatHi) myargs[ARG_SEARCHSP].floatvalue;


	options->filter_string = StringSave(myargs[ARG_FILTER].strvalue);
	options->expect_value  = (Nlm_FloatHi) myargs [ARG_EVALUE].floatvalue;

        if (StringICmp("blastn", program_name) == 0)
        {
                options->penalty = myargs[ARG_MISMATCH].intvalue;
                options->reward = myargs[ARG_MATCH].intvalue;
        }

	options->db_length = (Int8) myargs[ARG_DBSIZE].floatvalue;

	options->discontinuous = FALSE;

        if (myargs[ARG_XDROP].intvalue != 0)
	{
               options->gap_x_dropoff = myargs[ARG_XDROP].intvalue;
	}
        if (myargs[ARG_WORDSIZE].intvalue != 0)
               options->wordsize = (Int2) myargs[ARG_WORDSIZE].intvalue;

	if (options->is_megablast_search) {
	   options->cutoff_s2 = options->wordsize*options->reward;
        }
	options->matrix = MemFree(options->matrix);
        BLASTOptionSetGapParams(options, myargs[ARG_MATRIX].strvalue, 0, 0); 

        if (myargs[ARG_GAPOPEN].intvalue != -1)
              options->gap_open = myargs[ARG_GAPOPEN].intvalue;
        if (myargs[ARG_GAPEXT].intvalue != -1)
               options->gap_extend = myargs[ARG_GAPEXT].intvalue;

	options->strand_option = myargs[ARG_STRAND].intvalue;

        /* Input longest intron length is in nucleotide scale; in the lower 
           level code it will be used in protein scale */
        if (myargs[ARG_INTRON].intvalue > 0) 
           options->longest_intron = myargs[ARG_INTRON].intvalue;


        if (!myargs[ARG_LOC1].strvalue && !myargs[ARG_LOC2].strvalue) {
           seqalign = BlastTwoSequencesWithCallback(bsp1, bsp2, program_name, 
              options, &other_returns, &error_returns, handle_results);
        } else {
            SeqLocPtr slp1=NULL, slp2=NULL;
            if (BL2SEQ_MakeSeqLoc(bsp1, bsp2, &slp1, &slp2, options->strand_option) == FALSE)
                return 1;
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
              aip = AsnIoOpen (myargs[ARG_ASNOUT].strvalue,"w");
           
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


Int2 Main (void)

{
    Char buf[256] = { '\0' };  /* Used below for name and version. */
    Int2 status = 0;    /* return value of function. */

    StringCpy(buf, "bl2seq ");
    StringNCat(buf, BlastGetVersionNumber(), sizeof(buf)-StringLen(buf)-1);
    if (! GetArgs (buf, NUMARG, myargs)) {
        return (1);
    }

    UseLocalAsnloadDataAndErrMsg ();

    if (! SeqEntryLoad())
                return 1;

    ErrSetMessageLevel(SEV_WARNING);

    if (myargs[ARG_FORCE_OLD].intvalue != 0)
        status = Main_old();
    else
        status = Main_new();

    return status;
}
	

