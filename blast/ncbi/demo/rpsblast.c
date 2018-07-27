static char const rcsid[] = "$Id: rpsblast.c,v 6.67 2005/04/27 14:55:09 papadopo Exp $";

/* $Id: rpsblast.c,v 6.67 2005/04/27 14:55:09 papadopo Exp $
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
* File Name:  $RCSfile: rpsblast.c,v $
*
* Author:  Sergei Shavirin
*
* Initial Version Creation Date: 12/14/1999
*
* $Revision: 6.67 $
*
* File Description:
*         Main file for RPS BLAST program
*
* $Log: rpsblast.c,v $
* Revision 6.67  2005/04/27 14:55:09  papadopo
* change signature of BlastFillHitSavingOptions
*
* Revision 6.66  2005/02/03 18:02:07  dondosha
* Pass summary returns to BLAST_FormatResults, needed for XML output
*
* Revision 6.65  2005/02/02 19:01:36  dondosha
* Use new high level API for performing the search
*
* Revision 6.64  2005/01/10 18:36:47  dondosha
* BlastMaskLocDNAToProtein and BlastMaskLocProteinToDNA moved to core with changed signatures
*
* Revision 6.63  2005/01/10 13:48:20  madden
* Change to BLAST_FillInitialWordOptions prototype
*
* Revision 6.62  2004/12/22 15:38:34  kans
* removed include of blast_tback.h
*
* Revision 6.61  2004/12/21 18:03:04  dondosha
* BLAST_SearchEngine renamed to Blast_RunFullSearch
*
* Revision 6.60  2004/12/07 16:52:57  papadopo
* fix check for range of nucleotide query
*
* Revision 6.59  2004/12/02 20:46:38  dondosha
* Corrected off by 1 error in handling of query location argument
*
* Revision 6.58  2004/12/01 21:15:41  papadopo
* update handling of query range restriction when using the new engine
*
* Revision 6.57  2004/11/23 20:43:06  papadopo
* 1. Add support for the new engine (controlled by -V flag)
* 2. Use enum values instead of hardcoded offsets into myargs[]
* 3. Remove the option for ungapped RPS blast
*
* Revision 6.56  2004/06/30 12:33:30  madden
* Add include for blfmtutl.h
*
* Revision 6.55  2004/04/30 15:33:05  dondosha
* Added argument in call to BXMLPrintOutput
*
* Revision 6.54  2004/02/09 23:14:44  camacho
* Fix to prevent printing multiple Seq-annots in ASN.1 output
*
* Revision 6.53  2004/02/06 05:32:36  camacho
* Properly close ASN.1 stream
*
* Revision 6.52  2003/05/30 17:31:10  coulouri
* add rcsid
*
* Revision 6.51  2003/05/13 16:02:42  coulouri
* make ErrPostEx(SEV_FATAL, ...) exit with nonzero status
*
* Revision 6.50  2003/03/20 14:47:16  madden
* StringSave on asn1_mode
*
* Revision 6.49  2003/03/20 13:44:24  madden
* Fix -m 10/11 output to make them SeqAnnots
*
* Revision 6.48  2002/12/31 22:47:16  boemker
* Added support for printing output as ASN (text, with -m 10, or binary, with
* -m 11).
*
* Revision 6.47  2002/11/29 20:13:13  camacho
* Fix incorrect parameter to FastaToSeqEntryForDb
*
* Revision 6.46  2002/10/17 20:36:00  camacho
* Disallow -L option for tblastn
*
* Revision 6.45  2002/10/13 22:43:51  camacho
* Minor correction
*
* Revision 6.44  2002/10/10 14:49:43  camacho
*
* 1. Removed irrelevant options: -E, -G, -S, -H
* 2. Added -L option to provide range restriction on query sequence
*
* Revision 6.43  2002/09/18 20:34:30  camacho
* Restored -P option
*
* Revision 6.42  2002/08/20 15:17:42  camacho
* Fixed small memory leak
*
* Revision 6.41  2002/08/09 19:41:25  camacho
* 1) Added blast version number to command-line options
* 2) Added explanations for some default parameters
*
* Revision 6.40  2002/06/19 22:50:17  dondosha
* Added all queries information for tabular output with multiple queries
*
* Revision 6.39  2002/04/29 19:55:25  madden
* Use ARG_FLOAT for db length
*
* Revision 6.38  2001/08/28 17:45:01  madden
* Add -m 9 as tabular output with comments
*
* Revision 6.36  2001/06/27 16:20:00  dondosha
* Enabled tabular output for RPS Blast
*
* Revision 6.35  2001/04/13 14:19:08  madden
* Do not print verison banner for XML
*
* Revision 6.34  2001/03/26 14:28:53  madden
* Fix XML problems
*
* Revision 6.33  2000/11/07 18:32:38  shavirin
* Added program version to the output.
*
* Revision 6.32  2000/11/01 20:03:15  shavirin
* Removed not-used option -f for threshold.
*
* Revision 6.31  2000/10/27 19:14:41  madden
* Change description of -b option
*
* Revision 6.30  2000/10/23 19:58:21  dondosha
* Open and close AsnIo outside of call(s) to BXMLPrintOutput
*
* Revision 6.29  2000/10/18 20:55:16  shavirin
* Removed unused command-line parameters.
*
* Revision 6.28  2000/10/02 16:40:53  shavirin
* Fixed setting of TXALIGN_BLASTX_SPECIAL; option.
*
* Revision 6.27  2000/09/29 19:04:57  shavirin
* Fixed warnings and minor errors detected on Windows NT.
*
* Revision 6.26  2000/09/28 18:51:16  shavirin
* Adopted to new parameter BioseqPtr in print results callback.
*
* Revision 6.25  2000/09/27 19:09:04  shavirin
* Significantly redesigned external interface to RPS Blast.
*
* Revision 6.24  2000/08/29 16:54:55  shavirin
* Added option (m = 7) to print XML output.
*
* Revision 6.23  2000/08/21 21:19:16  shavirin
* Removed absolete variable MaxThreadsSem and related code.
*
* Revision 6.22  2000/08/21 19:24:28  madden
* Fix problem writing ASN.1 when multi-threading
*
* Revision 6.21  2000/08/18 19:38:31  madden
* Set believe_query and html flags in AcknowledgeBlastQuery
*
* Revision 6.20  2000/08/04 16:36:05  madden
* Concatenate rather than overwrite SeqAnnot
*
* Revision 6.19  2000/06/28 17:12:29  shavirin
* Fixed problem with -U T option: NULL-ed slp between different sequences.
*
* Revision 6.18  2000/06/27 15:25:19  madden
* Changed master-slave to query-anchored
*
* Revision 6.17  2000/06/20 15:49:35  shavirin
* Added BLAST database title to SeqAnnot output.
*
* Revision 6.16  2000/05/02 18:01:32  shavirin
* Adjusted to changes in RPSInti() function.
*
* Revision 6.15  2000/04/13 18:50:55  shavirin
* Fixed serious memory leaks.
*
* Revision 6.14  2000/04/12 14:15:41  shavirin
* Added back ObjMgrFreeCache together wirg seqmgr changes, those removed
* deadlock.
*
* Revision 6.13  2000/03/28 20:33:44  shavirin
* Changed logic of processing MT - multiple FASTA files.
*
* Revision 6.12  2000/03/10 20:00:15  shavirin
* Added multi-thread support for multi-FASTA files.X
*
* Revision 6.11  2000/03/02 21:06:09  shavirin
* Added -U option, that allows to consider low characters in FASTA files
* as filtered regions (for blastn, blastp and tblastn).
*
* Revision 6.10  2000/02/23 21:03:36  shavirin
* Fixed -z and -Y options in rpsblast.
*
* Revision 6.9  2000/02/17 21:28:55  shavirin
* Added option is_rps_blast = TRUE.
*
* Revision 6.8  2000/02/15 16:14:04  shavirin
* Minor changes.
*
* Revision 6.7  2000/02/11 22:05:01  shavirin
* Oprion do_sum_stats set to FALSE.
*
* Revision 6.6  2000/02/11 20:51:01  shavirin
* Added possibility to search PSSM database against DNA sequences.
*
* Revision 6.5  2000/02/08 17:39:08  shavirin
* Empty log message.
*
* Revision 6.4  2000/02/01 17:22:48  shavirin
* Updated function RPSViewSeqAlign().
*
* Revision 6.3  2000/01/07 22:34:05  shavirin
* Added printing of SeqAlignment if necessary.
*
* Revision 6.2  1999/12/30 18:37:53  shavirin
* Added NCBI header and Log information.
*
*
* ==========================================================================
*/

#include <ncbi.h>               /* Old engine includes */
#include <lookup.h>
#include <tofasta.h>
#include <txalign.h>
#include <ncbithr.h>
#include <rpsutil.h>
#include <xmlblast.h>
#include <blfmtutl.h>
#include <sqnutils.h>

#include <readdb.h>             /* Updated engine includes */
#include <algo/blast/api/blast_input.h>
#include <algo/blast/api/blast_format.h>
#include <algo/blast/api/blast_api.h>
#include <algo/blast/api/blast_seq.h>

#if PURIFY
#include "/am/purew/solaris2/new/../purify/purify-4.5-solaris2/purify.h"
#endif

enum {
    OPT_QUERY_FILE = 0,
    OPT_DB,
    OPT_PROT_QUERY,
    OPT_EVALUE,
    OPT_FORMAT,
    OPT_OUTPUT_FILE,
    OPT_XDROP_UNGAPPED,
    OPT_UNGAPPED_HITS,
    OPT_FILTER,
    OPT_XDROP_GAPPED,
    OPT_GAP_TRIGGER,
    OPT_NUMTHREADS,
    OPT_SHOW_GI,
    OPT_BELIEVE_QUERY,
    OPT_XDROP_GAPPED_FINAL,
    OPT_ASNOUT,
    OPT_NUM_DESC,
    OPT_NUM_RESULTS,
    OPT_DBLENGTH,
    OPT_SEARCHSP,
    OPT_HTML,
    OPT_LOGFILE,
    OPT_LCASE,
    OPT_RANGE,
    OPT_FORCE_OLD_ENGINE,
    NUM_ARGS    /* must come last */
};
    
static Args myargs[] = {
    /* OPT_QUERY_FILE */
    {"Input query sequence (this parameter must be set)",
     "stdin", NULL,NULL,FALSE,'i',ARG_FILE_IN, 0.0,0,NULL},
    /* OPT_DB */
    {"RPS BLAST Database",
     NULL, NULL,NULL,FALSE,'d',ARG_FILE_IN, 0.0,0,NULL},
    /* OPT_PROT_QUERY */
    {"Query sequence is protein ",
     "T", NULL,NULL,TRUE, 'p', ARG_BOOLEAN, 0.0,0,NULL},
    /* OPT_EVALUE */
    { "Expectation value (E)",
      "10.0", NULL, NULL, FALSE, 'e', ARG_FLOAT, 0.0, 0, NULL},
    /* OPT_FORMAT*/
    { "alignment view options:\n"
"0 = pairwise,\n"
"1 = query-anchored showing identities,\n"
"2 = query-anchored no identities,\n"
"3 = flat query-anchored, show identities,\n"
"4 = flat query-anchored, no identities,\n"
"5 = query-anchored no identities and blunt ends,\n"
"6 = flat query-anchored, no identities and blunt ends,\n"
"7 = XML Blast output,\n8 = tabular output, \n"
"9 = tabular output with comments",
      "0", NULL, NULL, FALSE, 'm', ARG_INT, 0.0, 0, NULL},
    /* OPT_OUTPUT_FILE */
    { "Output File for Alignment",
      "stdout", NULL, NULL, TRUE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
    /* OPT_XDROP_UNGAPPED */
    { "Dropoff (X) for blast extensions in bits (default if zero)",
      "7.0", NULL, NULL, FALSE, 'y', ARG_FLOAT, 0.0, 0, NULL},
    /* OPT_UNGAPPED_HITS */
    { "0 for multiple hit, 1 for single hit",
       "0",  NULL, NULL, FALSE, 'P', ARG_INT, 0.0, 0, NULL},
    /* OPT_FILTER */
    { "Filter query sequence with SEG",
      "F", NULL, NULL, FALSE, 'F', ARG_STRING, 0.0, 0, NULL},
    /* OPT_XDROP_GAPPED */
    { "X dropoff value for gapped alignment (in bits)",
      "15", NULL, NULL, FALSE, 'X', ARG_INT, 0.0, 0, NULL},
    /* OPT_GAP_TRIGGER */
    { "Number of bits to trigger gapping",
      "22.0", NULL, NULL, FALSE, 'N', ARG_FLOAT, 0.0, 0, NULL},
    /* OPT_NUMTHREADS */
    { "Number of processors to use",
      "1", NULL, NULL, FALSE, 'a', ARG_INT, 0.0, 0, NULL},
    /* OPT_SHOW_GI */
    { "Show GI's in deflines",
      "F", NULL, NULL, FALSE, 'I', ARG_BOOLEAN, 0.0, 0, NULL},
    /* OPT_BELIEVE_QUERY */
    { "Believe the query defline",
      "F", NULL, NULL, FALSE, 'J', ARG_BOOLEAN, 0.0, 0, NULL},
    /* OPT_XDROP_GAPPED_FINAL */
    { "X dropoff value for final gapped alignment (in bits)",
      "25", NULL, NULL, FALSE, 'Z', ARG_INT, 0.0, 0, NULL},
    /* OPT_ASNOUT */
    { "SeqAlign file ('Believe the query defline' must be TRUE)",
      NULL, NULL, NULL, TRUE, 'O', ARG_FILE_OUT, 0.0, 0, NULL},
    /* OPT_NUM_DESC */
    { "Number of database sequences to show one-line descriptions for (V)",
      "500", NULL, NULL, FALSE, 'v', ARG_INT, 0.0, 0, NULL},
    /* OPT_NUM_RESULTS */
    { "Number of database sequence to show alignments for (B)",
      "250", NULL, NULL, FALSE, 'b', ARG_INT, 0.0, 0, NULL},
    /* OPT_DBLENGTH */
    { "Effective length of the database (use zero for the real size)",
      "0", NULL, NULL, FALSE, 'z', ARG_FLOAT, 0.0, 0, NULL},
    /* OPT_SEARCHSP */
    { "Effective length of the search space (use zero for the real size)",
      "0", NULL, NULL, FALSE, 'Y', ARG_FLOAT, 0.0, 0, NULL},
    /* OPT_HTML */
    { "Produce HTML output",
      "F", NULL, NULL, FALSE, 'T', ARG_BOOLEAN, 0.0, 0, NULL},
    /* OPT_LOGIFLE */
    {"Logfile name ",
     "rpsblast.log", NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL},
    /* OPT_LCASE */
    {"Use lower case filtering of FASTA sequence",
     "F", NULL,NULL,TRUE,'U',ARG_BOOLEAN, 0.0,0,NULL},
    /* OPT_RANGE */
    { "Range restriction on query sequence (Format: start,stop) blastp only\n"
      "      0 in 'start' refers to the beginning of the sequence\n"
      "      0 in 'stop' refers to the end of the sequence",
      "0,0", NULL, NULL, TRUE, 'L', ARG_STRING, 0.0, 0, NULL},
    /* OPT_FORCE_OLD_ENGINE */
    { "Force use of the original BLAST engine",
      "T", NULL, NULL, TRUE, 'V', ARG_BOOLEAN, 0.0, 0, NULL},
};


/*------------------- RPS BLAST USING NEW ENGINE ------------------------*/

/* This is very similar to the routine of the same name
   in blast_driver.c; unneeded setup is removed */

static Int2 
BLAST_FillOptions(SBlastOptions* options)
{
   Int2 status;
   EBlastProgramType program_number = options->program;
   LookupTableOptions* lookup_options = options->lookup_options;
   QuerySetUpOptions* query_setup_options = options->query_options; 
   BlastInitialWordOptions* word_options = options->word_options;
   BlastExtensionOptions* ext_options = options->ext_options ;
   BlastHitSavingOptions* hit_options = options->hit_options;
   BlastScoringOptions* score_options = options->score_options;
   BlastEffectiveLengthsOptions* eff_len_options = options->eff_len_options;
   BlastDatabaseOptions* db_options = options->db_options;

   BLAST_FillLookupTableOptions(lookup_options, program_number, 
                                FALSE, /* megablast */
                                0,     /* default threshold */
                                0,     /* default wordsize */
                                FALSE, /* no variable wordsize */
                                FALSE);/* no PSSM (i.e. not psiblast) */

   BLAST_FillQuerySetUpOptions(query_setup_options, program_number, 
                              myargs[OPT_FILTER].strvalue, 0);

   BLAST_FillInitialWordOptions(word_options, program_number, FALSE,
                                0, myargs[OPT_XDROP_UNGAPPED].intvalue);

   /* set one-hit extensions if desired */
   if (myargs[OPT_UNGAPPED_HITS].intvalue == 1)
       word_options->window_size = 0;

   BLAST_FillExtensionOptions(ext_options, program_number, FALSE, 
                              myargs[OPT_XDROP_GAPPED].intvalue, 
                              myargs[OPT_XDROP_GAPPED_FINAL].intvalue);

   BLAST_FillScoringOptions(score_options, program_number, FALSE,
                0, 0,  /* nucleotide match/mismatch scores */
                NULL, 0, 0); /* Matrix, gap opening and gap extension penalties
                                will be set from the RPS database, so there is
                                no need to set them here. */

   score_options->gapped_calculation = TRUE;

   BLAST_FillHitSavingOptions(hit_options, 
                      myargs[OPT_EVALUE].floatvalue, 
                      MAX(myargs[OPT_NUM_DESC].intvalue, 
                          myargs[OPT_NUM_RESULTS].intvalue),
                      TRUE,
                      0);

   if (myargs[OPT_SEARCHSP].floatvalue != 0) {
      eff_len_options->searchsp_eff = (Int8) myargs[OPT_SEARCHSP].floatvalue; 
   }

   if (db_options && program_number == eBlastTypeRpsTblastn) {
      if ((status = BLAST_GeneticCodeFind(db_options->genetic_code, 
                       &db_options->gen_code_string))) {
         return status;
      }
   }

   return 0;
}

static Int2
s_ParseIntervalLocationArgument(char* arg, Int4* from_ptr, Int4* to_ptr)
{
   const char* delimiters = " ,;";
   Int4 from = 0, to = 0;

   if (!arg)
      return 0;
   
   from = atoi(StringTokMT(arg, delimiters, &arg));
   to = atoi(arg);
      
   from = MAX(from, 0);
   to = MAX(to, 0);

   *from_ptr = from;
   *to_ptr = to;

   if (from > to)
      return -1;
   else
      return 0;
}

/* Similar to the Main() of blast_driver.c; major differences are
        - only one thread supported
        - no two-sequences capability
        - on-the-fly tabular output not supported
*/
Int2 Main_New(void)
{
   Int2 status;
   Boolean query_is_na;
   LookupTableOptions* lookup_options;
   char* blast_program = NULL;
   EBlastProgramType program_number;
   BlastInitialWordOptions* word_options;
   BlastScoringOptions* score_options;
   BlastExtensionOptions* ext_options;
   BlastHitSavingOptions* hit_options;
   char* dbname = NULL;
   FILE *infp;
   Int2 ctr = 1;
   Int4 letters_read;
   Int4 query_from = 0, query_to = 0;
   SBlastOptions* options = NULL;
   QuerySetUpOptions* query_options=NULL;	
   BlastEffectiveLengthsOptions* eff_len_options=NULL;
   SeqLoc* query_slp = NULL;
   SeqAlign* seqalign = NULL;
   BlastFormattingOptions* format_options;
   PSIBlastOptions* psi_options = NULL;
   BlastDatabaseOptions* db_options = NULL;
   Blast_SummaryReturn* sum_returns=Blast_SummaryReturnNew();
   Boolean believe_defline = FALSE;
   Int4 num_queries = 0;
   SeqLoc* lcase_mask = NULL;
   SeqLoc* filter_loc = NULL;
   Boolean mask_at_hash;

   /* select protein or nucleotide query, and choose
      the appropriate program type */

   if (myargs[OPT_PROT_QUERY].intvalue == FALSE) {
       query_is_na = TRUE;
       blast_program = "rpstblastn";
   } else {
       query_is_na = FALSE;
       blast_program = "rpsblast";
   }

   status = SBlastOptionsNew(blast_program, &options, sum_returns);

   if (status) {
       if (sum_returns->error) {
           Blast_SummaryReturnsPostError(sum_returns);
           sum_returns = Blast_SummaryReturnFree(sum_returns);
       }
       return -1;
   }

   /* initialize the database and then the RPS_specific data files */

   dbname = myargs[OPT_DB].strvalue;

   BLAST_FillOptions(options);
   program_number = options->program;

   if ((status = BlastFormattingOptionsNew(program_number, 
                       myargs[OPT_OUTPUT_FILE].strvalue, 
                       myargs[OPT_NUM_DESC].intvalue, 
                       myargs[OPT_NUM_RESULTS].intvalue, 
                       myargs[OPT_FORMAT].intvalue, &format_options)) != 0) {
       ErrPostEx(SEV_FATAL, 1, 0,  "Formatting setup failed");
   }

   if (myargs[OPT_HTML].intvalue) {
       format_options->html = TRUE;
       format_options->align_options += TXALIGN_HTML;
       format_options->print_options += TXALIGN_HTML;
   }
   if (myargs[OPT_SHOW_GI].intvalue) {
       format_options->align_options += TXALIGN_SHOW_GI;
       format_options->print_options += TXALIGN_SHOW_GI;
   }

   BLAST_PrintOutputHeader(format_options, FALSE, blast_program, 
                           dbname, TRUE);

   believe_defline = myargs[OPT_BELIEVE_QUERY].intvalue;

   /* Get the query */

   if ((infp = FileOpen(myargs[OPT_QUERY_FILE].strvalue, "r")) == NULL) {
      ErrPostEx(SEV_FATAL, 1, 0, "Unable to open input file '%s'\n", 
                myargs[OPT_QUERY_FILE].strvalue);
      return (1);
   }

   if (s_ParseIntervalLocationArgument(myargs[OPT_RANGE].strvalue,
                                        &query_from, &query_to)) {
      ErrPostEx(SEV_FATAL, 1, 0, "Invalid subject sequence location\n");
      return -1;                   
   }
   if ((query_from != 0 || query_to != 0) && 
       (Boolean)myargs[OPT_PROT_QUERY].intvalue == FALSE) {
      ErrPostEx(SEV_FATAL, 1, 0, "No query range allowed for nucleotide query");
      return -1;                   
   }

   if ((Boolean)myargs[OPT_LCASE].intvalue) {
      letters_read = BLAST_GetQuerySeqLoc(infp, query_is_na, 0, 0, 
                                          query_from, query_to, &lcase_mask, 
                                          &query_slp, &ctr, &num_queries, 
                                          believe_defline);
   } else {
      letters_read = BLAST_GetQuerySeqLoc(infp, query_is_na, 0, 0,
                                          query_from, query_to, NULL, 
                                          &query_slp, &ctr, &num_queries, 
                                          believe_defline);
   }
   if (letters_read <= 0) {
        ErrPostEx(SEV_FATAL, 1, 0, "Failure reading query\n");
        return -1;
   }

   /* Call database search function. Pass NULL for tabular formatting structure 
    * pointer, because on-the-fly tabular formatting is not allowed for RPS 
    * BLAST.
    */
   status = 
       Blast_DatabaseSearch(query_slp, dbname, lcase_mask, options, NULL, 
                            &seqalign, &filter_loc, &mask_at_hash, sum_returns);

   /* Free the lower case mask in SeqLoc form. */
   lcase_mask = Blast_ValNodeMaskListFree(lcase_mask);
   
   /* If masking was done for lookup table only, free the masking locations,
      because they will not be used for formatting. */
   if (mask_at_hash)
       filter_loc = Blast_ValNodeMaskListFree(filter_loc);

   /* Post warning or error messages, no matter what the search status was. */
   Blast_SummaryReturnsPostError(sum_returns);

   if (status != 0) {
       ErrPostEx(SEV_FATAL, 1, 0, "BLAST search failed");
       return status;
   }

   /* do initial cleanup */

   /* format results */

   if (myargs[OPT_ASNOUT].strvalue) {
      AsnIoPtr asnout = AsnIoOpen(myargs[OPT_ASNOUT].strvalue, (char*)"w");
      GenericSeqAlignSetAsnWrite(seqalign, asnout);
      asnout = AsnIoClose(asnout);
   }
 
   status = BLAST_FormatResults(seqalign, dbname, blast_program, 
                                num_queries, query_slp,
                                filter_loc, format_options, 
                                FALSE, NULL, sum_returns);

   /* finish cleanup */
   filter_loc = Blast_ValNodeMaskListFree(filter_loc);
   seqalign = SeqAlignSetFree(seqalign);
   query_slp = SeqLocSetFree(query_slp);
   options = SBlastOptionsFree(options);

   /* Print the footer with summary information */
   Blast_PrintOutputFooter(program_number, format_options, dbname, sum_returns);

   sum_returns = Blast_SummaryReturnFree(sum_returns);

   if(format_options->html && myargs[OPT_FORMAT].intvalue < 7) {
      fprintf(format_options->outfp, "</PRE>\n</BODY>\n</HTML>\n");
   }
   BlastFormattingOptionsFree(format_options);
   return status;
}

/*------------------- RPS BLAST USING OLD ENGINE ------------------------*/

void PGPGetPrintOptions(Boolean gapped, Uint4Ptr align_options_out, 
                        Uint4Ptr print_options_out)
{
    Uint4 print_options, align_options;

    print_options = 0;
    if (gapped == FALSE)
        print_options += TXALIGN_SHOW_NO_OF_SEGS;
    
    align_options = 0;
    align_options += TXALIGN_COMPRESS;
    align_options += TXALIGN_END_NUM;

    if (myargs[OPT_SHOW_GI].intvalue) {
        align_options += TXALIGN_SHOW_GI;
        print_options += TXALIGN_SHOW_GI;
    } 
    
    if (myargs[OPT_HTML].intvalue) {
        align_options += TXALIGN_HTML;
        print_options += TXALIGN_HTML;
    }

    if(!myargs[OPT_PROT_QUERY].intvalue)
        align_options += TXALIGN_BLASTX_SPECIAL;
    
    if (myargs[OPT_FORMAT].intvalue != 0) {
        align_options += TXALIGN_MASTER;
        if (myargs[OPT_FORMAT].intvalue == 1 || 
            myargs[OPT_FORMAT].intvalue == 3)
            align_options += TXALIGN_MISMATCH;

        if (myargs[OPT_FORMAT].intvalue == 3 || 
            myargs[OPT_FORMAT].intvalue == 4 || 
            myargs[OPT_FORMAT].intvalue == 6)
            align_options += TXALIGN_FLAT_INS;

        if (myargs[OPT_FORMAT].intvalue == 5 || 
            myargs[OPT_FORMAT].intvalue == 6)
            align_options += TXALIGN_BLUNT_END;
    } else {
        align_options += TXALIGN_MATRIX_VAL;
        align_options += TXALIGN_SHOW_QS;
    }

    *align_options_out = align_options;
    *print_options_out = print_options;

    return;
}

void RPSBlastOptionsFree(RPSBlastOptionsPtr rpsbop)
{

    FileClose(rpsbop->outfp);
    AsnIoClose(rpsbop->aip);
    BLASTOptionDelete(rpsbop->options);
    readdb_destruct(rpsbop->rdfp);

    MemFree(rpsbop->rps_database);    
    MemFree(rpsbop->out_filename);
    MemFree(rpsbop);
    
    return;
}

static RPSBlastOptionsPtr RPSReadBlastOptions(void)
{
    RPSBlastOptionsPtr rpsbop;
    BLAST_OptionsBlkPtr options;
    static Int4 count=0;
    CharPtr location = NULL;
    
    rpsbop = MemNew(sizeof(RPSBlastOptions));
    

    if (myargs[OPT_OUTPUT_FILE].strvalue != NULL) 
	rpsbop->out_filename = StringSave(myargs[OPT_OUTPUT_FILE].strvalue);

    /* Note: these 2 parameters are necessary to intialize RPS Blast */
    rpsbop->rps_database = StringSave(myargs[OPT_DB].strvalue);    
    rpsbop->query_is_protein = myargs[OPT_PROT_QUERY].intvalue;
    rpsbop->num_threads = myargs[OPT_NUMTHREADS].intvalue;

    if((rpsbop->rdfp = readdb_new(myargs[OPT_DB].strvalue, TRUE)) == NULL)
        return NULL;
    
    /* rpsbop->rpsinfo = RPSInfoAttach(rpsinfo_main); */

    if (myargs[OPT_BELIEVE_QUERY].intvalue != 0)
        rpsbop->believe_query = TRUE;
    
    options = BLASTOptionNew(rpsbop->query_is_protein ? "blastp" : "tblastn", 
                             TRUE);
    rpsbop->options = options;
    
    /* rpsbop->options->query_lcase_mask = slp; External filtering */
    
    if (myargs[OPT_DBLENGTH].floatvalue)
        options->db_length = (Int8) myargs[OPT_DBLENGTH].floatvalue;
    
    if (myargs[OPT_SEARCHSP].floatvalue)
        options->searchsp_eff = (Nlm_FloatHi) myargs[OPT_SEARCHSP].floatvalue;
    
    /* Necessary options for RPS Blast */
    options->do_sum_stats = FALSE;
    options->is_rps_blast = TRUE; 
    
    rpsbop->number_of_descriptions = myargs[OPT_NUM_DESC].intvalue;
    rpsbop->number_of_alignments = myargs[OPT_NUM_RESULTS].intvalue;
    
    /* Set default gap params for matrix. */
    BLASTOptionSetGapParams(options, "BLOSUM62", 0, 0);
    
    if(myargs[OPT_FORMAT].intvalue == 7) {
       rpsbop->is_xml_output = TRUE;
    }
    else if (myargs[OPT_FORMAT].intvalue == 8)
       rpsbop->is_tabular = TRUE;
    else if (myargs[OPT_FORMAT].intvalue == 9)
    {
       rpsbop->is_tabular = TRUE;
       rpsbop->is_tabular_comments = TRUE;
    }
    else if (myargs[OPT_FORMAT].intvalue == 10 || 
             myargs[OPT_FORMAT].intvalue == 11)
    {
       rpsbop->is_asn1_output = TRUE;
	if (myargs[OPT_FORMAT].intvalue == 10)
		rpsbop->asn1_mode = StringSave("w");
	else
		rpsbop->asn1_mode = StringSave("wb");
    }
    else
       PGPGetPrintOptions(options->gapped_calculation, 
                          &rpsbop->align_options, &rpsbop->print_options);
    

    if (!rpsbop->is_xml_output && 
        !rpsbop->is_asn1_output && 
        (rpsbop->outfp = FileOpen(rpsbop->out_filename, "a")) == NULL) {
            ErrPostEx(SEV_FATAL, 1, 0, "rpsblast: Unable to open output "
                      "file %s\n", rpsbop->out_filename);
            return NULL;
    }

    if (myargs[OPT_ASNOUT].strvalue != NULL) {
        if (myargs[OPT_BELIEVE_QUERY].intvalue == 0) {
            ErrPostEx(SEV_FATAL, 1, 0, 
                      "-J option must be TRUE to use this option");
            return NULL;
        } else  {
            if ((rpsbop->aip = AsnIoOpen (myargs[OPT_ASNOUT].strvalue,"w")) == NULL) {
                ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open output "
                          "file %s\n", myargs[OPT_ASNOUT].strvalue);
                return NULL;
            }
    	}
    }
    else if (rpsbop->is_asn1_output)
    {
            if ((rpsbop->aip = AsnIoOpen (rpsbop->out_filename, rpsbop->asn1_mode)) == NULL) {
                ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open output "
                          "file %s\n", rpsbop->out_filename);
                return NULL;
            }
    }

    options->dropoff_2nd_pass  = (Int4) myargs[OPT_XDROP_UNGAPPED].floatvalue;
    options->expect_value  = (Nlm_FloatHi) myargs[OPT_EVALUE].floatvalue;
    options->hitlist_size = MAX(rpsbop->number_of_descriptions, 
                                rpsbop->number_of_alignments);
    
    if (myargs[OPT_UNGAPPED_HITS].intvalue == 1) {
        options->two_pass_method  = FALSE;
        options->multiple_hits_only  = FALSE;
    } else {
        /* all other inputs, including the default 0 use 2-hit method */
        options->two_pass_method  = FALSE;
        options->multiple_hits_only  = TRUE;
    }

    options->gap_x_dropoff = myargs[OPT_XDROP_GAPPED].intvalue;
    options->gap_x_dropoff_final = myargs[OPT_XDROP_GAPPED_FINAL].intvalue;
    options->gap_trigger = myargs[OPT_GAP_TRIGGER].floatvalue;
    
    if (StringICmp(myargs[OPT_FILTER].strvalue, "T") == 0) {
        options->filter_string = StringSave("S");
    } else {
        options->filter_string = StringSave(myargs[OPT_FILTER].strvalue);
    }

    options->number_of_cpus = (Int2) myargs[OPT_NUMTHREADS].intvalue;
    
    options->isPatternSearch = FALSE;

    options = BLASTOptionValidate(options, rpsbop->query_is_protein ? 
                                  "blastp" : "tblastn");
    
    /* Kludge: use required_{start,end} to store offsets that will restrict
     * the query sequence. These values are later converted into a SeqLoc */
    if ((location = myargs[OPT_RANGE].strvalue)) {
        options->required_start = 
           MAX(0, atol(StringTokMT(location, " ,;", &location)) - 1);
        options->required_end = MAX(0, atol(location) - 1);
        if (options->required_start < 0 || options->required_end < 0) {
            ErrPostEx(SEV_ERROR,0,0, "Invalid range restriction: %ld to %ld",
                    options->required_start, options->required_end);
            return NULL;
        }
        /* Range restriction is only allowed for protein queries */
        if (rpsbop->query_is_protein == FALSE && 
            (options->required_start != 0 && options->required_end != 0)) {
            ErrPostEx(SEV_ERROR, 0, 0, "Range restriction supported only for "
                    "protein queries");
            options->required_start = options->required_end = 0;
        }
    }

    if (options == NULL)
        return NULL;
    
    return rpsbop;
}

static void RPSViewSeqAlign(BioseqPtr query_bsp, 
                            SeqAlignPtr seqalign, RPSBlastOptionsPtr rpsbop, 
                            ValNodePtr other_returns)
{
    SeqAnnotPtr seqannot;
    BlastPruneSapStructPtr prune;
    Uint1 align_type;
    ValNodePtr vnp, mask;

    seqannot = SeqAnnotNew();
    seqannot->type = 2;
    align_type = BlastGetProgramNumber(rpsbop->query_is_protein ?
                                       "blastp" : "blastx");
    AddAlignInfoToSeqAnnot(seqannot, align_type); /* blastp or tblastn */
    
    if (rpsbop->aip != NULL) {     
        CharPtr title = readdb_get_title(rpsbop->rdfp); /* we don't own title */
        BLASTAddBlastDBTitleToSeqAnnot(seqannot, title);
    }

    if (rpsbop->outfp == NULL) {
        seqannot = SeqAnnotFree(seqannot);
        return;
    }

    free_buff();    
    init_buff_ex(128);
    
    /* BlastPrintReference(FALSE, 90, rpsbop->outfp); */
    fprintf(rpsbop->outfp, "\n");

    AcknowledgeBlastQuery(query_bsp, 70, rpsbop->outfp, 
                          rpsbop->believe_query, rpsbop->html);
    if(seqalign == NULL) {
        fprintf(rpsbop->outfp, "\nNo hits found for the sequence...\n\n");
        seqannot = SeqAnnotFree(seqannot);
        return;
    }
    
    prune = BlastPruneHitsFromSeqAlign(seqalign, 
                                       rpsbop->number_of_descriptions, NULL);
    PrintDefLinesFromSeqAlign(prune->sap, 80, rpsbop->outfp, 
                              rpsbop->print_options, FIRST_PASS, NULL);

    
    /* --------------------------------------- */
    
    prune = BlastPruneHitsFromSeqAlign(seqalign, rpsbop->number_of_alignments, 
                                       prune);
    seqannot->data = prune->sap;

    /* Write the ASN.1 Seq-annot */
    if (rpsbop->aip != NULL) {     
        SeqAnnotAsnWrite(seqannot, rpsbop->aip, NULL);
        AsnIoReset(rpsbop->aip);
    }

    /* ------ Mask needed ----- */

    mask = NULL;
    for (vnp=other_returns; vnp; vnp = vnp->next) {
        switch (vnp->choice) {
        case SEQLOC_MASKING_NOTSET:
        case SEQLOC_MASKING_PLUS1:
        case SEQLOC_MASKING_PLUS2:
        case SEQLOC_MASKING_PLUS3:
        case SEQLOC_MASKING_MINUS1:
        case SEQLOC_MASKING_MINUS2:
        case SEQLOC_MASKING_MINUS3:
            ValNodeAddPointer(&mask, vnp->choice, vnp->data.ptrvalue);
            break;
        default:
            break;
        }
    }

    if (myargs[OPT_FORMAT].intvalue != 0) {
        ShowTextAlignFromAnnot(seqannot, 60, rpsbop->outfp, 
                               NULL, NULL, rpsbop->align_options, NULL, 
                               mask, NULL);
    } else {
        ShowTextAlignFromAnnot(seqannot, 60, rpsbop->outfp, 
                               NULL, NULL, rpsbop->align_options, NULL, 
                               mask, FormatScoreFunc);
    }

    prune = BlastPruneSapStructDestruct(prune);

    seqannot->data = NULL;
    seqannot = SeqAnnotFree(seqannot);
    seqannot = SeqAnnotFree(seqannot);

    free_buff();
    return;
}

Boolean RPSFormatFooter(RPSBlastOptionsPtr rpsbop,  ValNodePtr other_returns)
{
    ValNodePtr  mask_loc, vnp;
    BLAST_KarlinBlkPtr ka_params=NULL, ka_params_gap=NULL;
    TxDfDbInfoPtr dbinfo=NULL, dbinfo_head;
    CharPtr params_buffer=NULL;
    BLAST_MatrixPtr blast_matrix;
    
    mask_loc = NULL;
    for (vnp=other_returns; vnp; vnp = vnp->next) {
        switch (vnp->choice) {
        case TXDBINFO:
            dbinfo = vnp->data.ptrvalue;
            break;
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
            blast_matrix = vnp->data.ptrvalue;
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
    
    free_buff();    
    init_buff_ex(85);
    dbinfo_head = dbinfo;
    while (dbinfo) {
        PrintDbReport(dbinfo, 70, rpsbop->outfp);
        dbinfo = dbinfo->next;
    }
    
    if (ka_params) {
        PrintKAParameters(ka_params->Lambda, ka_params->K, ka_params->H, 
                          70, rpsbop->outfp, FALSE);
    }
    
    if (ka_params_gap) {
        PrintKAParameters(ka_params_gap->Lambda, ka_params_gap->K,
                          ka_params_gap->H, 70, rpsbop->outfp, TRUE);
    }
    
    PrintTildeSepLines(params_buffer, 70, rpsbop->outfp);
    free_buff();

    fflush(rpsbop->outfp);

    return TRUE;
}

static SeqEntryPtr LIBCALLBACK RPSGetNextSeqEntry(SeqLocPtr PNTR slp, VoidPtr data)
{
    SeqEntryPtr sep;
    static TNlmMutex read_mutex;
    static FILE *infp;
    static Boolean end_of_data = FALSE;

    NlmMutexInit(&read_mutex);
    NlmMutexLock(read_mutex);
    
    if(end_of_data) {
        NlmMutexUnlock(read_mutex);
        return NULL;
    }

    /* Opening file with input sequences */
    if(infp == NULL) {
        if ((infp = FileOpen(myargs[OPT_QUERY_FILE].strvalue, "r")) == NULL) {
            ErrPostEx(SEV_FATAL, 1, 0, 
                      "rpsblast: Unable to open input file %s\n", 
                      myargs[OPT_QUERY_FILE].strvalue);
            end_of_data = TRUE;
            NlmMutexUnlock(read_mutex);
        }
    }
    
    if(myargs[OPT_LCASE].intvalue) {
        sep = FastaToSeqEntryForDb (infp, !myargs[OPT_PROT_QUERY].intvalue, 
                                    NULL, myargs[OPT_BELIEVE_QUERY].intvalue, 
                                    NULL, NULL, slp);
    } else {
        sep = FastaToSeqEntryEx (infp, !myargs[OPT_PROT_QUERY].intvalue, 
                                 NULL, myargs[OPT_BELIEVE_QUERY].intvalue);
    }
    
    if(sep == NULL) {            /* Probably last FASTA entry */
        end_of_data = TRUE;
        FileClose(infp);
        NlmMutexUnlock(read_mutex);
        return NULL;
    }
    
    NlmMutexUnlock(read_mutex);
    
    return sep;
}

static Boolean LIBCALLBACK RPSResultsCallback(BioseqPtr query_bsp,
                                  RPSBlastOptionsPtr rpsbop, 
                                  SeqAlignPtr seqalign, 
                                  ValNodePtr other_returns, 
                                  ValNodePtr error_returns, VoidPtr data)
{
    if(rpsbop->is_xml_output == TRUE) {
       AsnIoPtr aip = AsnIoOpen(rpsbop->out_filename, "wx");
       BXMLPrintOutput(aip, seqalign, rpsbop->options, 
                       rpsbop->query_is_protein ? 
                       "blastp" : "tblastn", rpsbop->rps_database, 
                       query_bsp, other_returns, 0, NULL, NULL);
       AsnIoClose(aip);
    } else if (rpsbop->is_tabular) {
	if (rpsbop->is_tabular_comments)
       		PrintTabularOutputHeader(rpsbop->rps_database, query_bsp, NULL, 
                                "rps-blast", 0, rpsbop->believe_query,
                                rpsbop->outfp);
       BlastPrintTabulatedResults(seqalign, query_bsp, NULL, 
                                  rpsbop->number_of_alignments,
                                  rpsbop->query_is_protein ? 
                                  "blastp" : "tblastn", FALSE,
                                  rpsbop->believe_query, 0, 0, rpsbop->outfp,
                                  FALSE);
    } else {
        RPSViewSeqAlign(query_bsp, seqalign, rpsbop, other_returns);
        RPSFormatFooter(rpsbop, other_returns);
    }
    
    return TRUE;
}

Int2 Main_Old(void)
{
    FILE *fd;
    RPSBlastOptionsPtr rpsbop;

    /* Truncate output file */
    if((fd = FileOpen(myargs[OPT_OUTPUT_FILE].strvalue, "w")) != NULL)
        FileClose(fd);
    
    if((rpsbop = RPSReadBlastOptions()) == NULL) {
        ErrPostEx(SEV_FATAL, 1, 0, "Unable to create RPS Blast options");
        return 1;
    }

    if (!rpsbop->is_xml_output && !rpsbop->is_tabular && !rpsbop->is_asn1_output)
	    BlastPrintVersionInfo("RPS-BLAST", rpsbop->html, rpsbop->outfp);
    
    RPSBlastSearchMT(rpsbop, RPSGetNextSeqEntry, NULL, 
                     RPSResultsCallback, NULL);
    RPSBlastOptionsFree(rpsbop);
}

/*-------------------------- TOP LEVEL DRIVER -------------------------*/

Int2 Main(void)
{
    Char buf[256] = { '\0' };

    StringCpy(buf, "rpsblast ");
    StringNCat(buf, BlastGetVersionNumber(), sizeof(buf)-StringLen(buf)-1);
    if (!GetArgs(buf, NUM_ARGS, myargs))
        return 1;
    
    if ( !ErrSetLog (myargs[OPT_LOGFILE].strvalue) ) { /* Logfile */
        ErrShow();
        return 1;
    } else {
        ErrSetOpts (ERR_CONTINUE, ERR_LOG_ON);
    }
    
    UseLocalAsnloadDataAndErrMsg ();
    
    if (!SeqEntryLoad())
        return 1;
        
    if (myargs[OPT_FORCE_OLD_ENGINE].intvalue == TRUE)
        return Main_Old();
    else
        return Main_New();
}
