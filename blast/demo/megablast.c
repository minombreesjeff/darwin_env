/* $Id: megablast.c,v 6.83 2001/12/28 20:42:16 dondosha Exp $
**************************************************************************
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
************************************************************************** 
 * $Revision 6.13$ *  
 * $Log: megablast.c,v $
 * Revision 6.83  2001/12/28 20:42:16  dondosha
 * Added options related to discontiguous words
 *
 * Revision 6.82  2001/11/15 12:37:26  dondosha
 * Error in previous commit - comma in wrong place
 *
 * Revision 6.81  2001/11/14 23:37:18  dondosha
 * Added parameters for ungapped and non-greedy gapped x-dropoffs
 *
 * Revision 6.80  2001/09/06 15:40:54  dondosha
 * Uncommented call to PrintTabularOutputHeader
 *
 * Revision 6.79  2001/08/08 22:38:16  dondosha
 * Added protection against endpoints beyond end of sequence for -D0 and -D1 outputs
 *
 * Revision 6.78  2001/07/27 21:47:36  dondosha
 * Fixed dummy variable declaration for call to StringToInt8
 *
 * Revision 6.77  2001/07/26 18:22:29  dondosha
 * Changed the effective length argument from float to string
 *
 * Revision 6.76  2001/07/20 18:47:21  dondosha
 * Scale cutoff_s2 if match reward not 1
 *
 * Revision 6.75  2001/07/03 20:50:33  madden
 * Commented out call to PrintTabularOutputHeader
 *
 * Revision 6.74  2001/06/28 21:23:33  dondosha
 * Print header for the -D3 output; added window size argument -A
 *
 * Revision 6.73  2001/06/21 21:49:55  dondosha
 * No need to declare extra variable vnp
 *
 * Revision 6.72  2001/06/21 21:43:57  dondosha
 * Destroy error returns
 *
 * Revision 6.71  2001/05/25 19:30:00  vakatov
 * Nested comment typo fixed
 *
 * Revision 6.70  2001/04/26 14:10:06  egorov
 * Call ReadDBBioseqFetchEnable function before fetching sequences.
 *
 * Revision 6.69  2001/03/19 22:39:25  dondosha
 * Allow location on the first query sequence for megablast
 *
 * Revision 6.68  2001/02/21 21:26:54  dondosha
 * Prefer gi to accession in -D[01] outputs
 *
 * Revision 6.67  2001/02/07 21:18:42  dondosha
 * Moved the MegaBlastPrintAlignInfo callback to blastool.c
 *
 * Revision 6.66  2001/02/05 16:48:34  dondosha
 * Parse all database general ids except BL_ORD_ID
 *
 * Revision 6.65  2001/01/12 17:48:05  dondosha
 * Check for NULL seqalign array on return from engine
 *
 * Revision 6.64  2001/01/05 17:12:49  dondosha
 * Correction in previous memory leak fix
 *
 * Revision 6.63  2001/01/03 21:45:31  dondosha
 * Fixed a memory leak - some edit blocks not freed in megablast
 *
 * Revision 6.62  2000/12/22 19:50:37  dondosha
 * Enabled percent identity cutoff for -D1 output format
 *
 * Revision 6.61  2000/12/21 22:33:05  dondosha
 * Added option to cut off results by identity percentage
 *
 * Revision 6.60  2000/11/29 16:28:06  dondosha
 * No need to extract ncbi4na-encoded subject from readdb any more
 *
 * Revision 6.59  2000/11/21 21:37:09  dondosha
 * Make -J option default FALSE for traditional output
 *
 * Revision 6.58  2000/11/20 22:13:37  shavirin
 * Changed name of the program to "megablast" for XML output.
 *
 * Revision 6.57  2000/11/20 17:02:06  dondosha
 * Minor corrections in command line option descriptions
 *
 * Revision 6.56  2000/11/08 18:30:20  kans
 * includes <sqnutils.h> for Mac compiler
 *
 * Revision 6.55  2000/11/03 20:18:12  dondosha
 * The adjustment of offsets and sorting of HSP array done before printing results in callbacks
 *
 * Revision 6.54  2000/10/31 15:08:05  dondosha
 * With -Q option print all query sequences
 *
 * Revision 6.53  2000/10/27 19:14:41  madden
 * Change description of -b option
 *
 * Revision 6.52  2000/10/26 18:54:12  dondosha
 * Added printing of reference to the traditional output
 *
 * Revision 6.51  2000/10/25 16:20:54  dondosha
 * Whenever possible, print accession in the -D3 output
 *
 * Revision 6.50  2000/10/23 19:59:03  dondosha
 * Added ability to print XML output
 *
 * Revision 6.49  2000/10/19 16:06:25  dondosha
 * Added a new output format, option -D 3, changed wordsize definition
 *
 * Revision 6.48  2000/10/18 17:05:42  dondosha
 * Added command line option -R to report log info at end of output
 *
 * Revision 6.47  2000/10/16 15:09:37  dondosha
 * Changed default for -U option to FALSE
 *
 * Revision 6.46  2000/10/13 20:15:58  dondosha
 * If local query id and no title in Bioseq, get it from the id
 *
 * Revision 6.45  2000/10/02 15:57:38  dondosha
 * Free query id buffer fo each HSP when it is printed
 *
 * Revision 6.44  2000/10/02 14:50:08  dondosha
 * Improvement to previous change
 *
 * Revision 6.43  2000/09/27 21:57:12  dondosha
 * Corrected one-line output for databases with ids of general type
 *
 * Revision 6.42  2000/09/20 19:12:10  dondosha
 * Print only IDs for subject sequences in -D[01] output with -f T option
 *
 * Revision 6.41  2000/09/19 17:09:17  dondosha
 * Removed restriction on query deflines length
 *
 * Revision 6.40  2000/09/06 21:17:38  dondosha
 * Added a -U option to allow ignoring of lower case
 *
 * Revision 6.39  2000/09/06 18:35:52  dondosha
 * Corrected percentage of identities computation for -D1 output
 *
 * Revision 6.38  2000/08/25 16:57:58  dondosha
 * Corrected comment for the -Q option
 *
 * Revision 6.37  2000/08/24 14:34:05  shavirin
 * Added return 1 if database do not exists on any path.
 *
 * Revision 6.36  2000/08/18 20:15:57  dondosha
 * Print only id, not full defline with -fT option
 *
 * Revision 6.35  2000/08/15 15:21:00  dondosha
 * If readdb returns no subject description, retrieve it from the id
 *
 * Revision 6.34  2000/08/10 17:12:45  dondosha
 * Improved the HTML output
 *
 * Revision 6.33  2000/08/03 17:50:49  dondosha
 * Check HSPs for going beyond ends of query in megablast
 *
 * Revision 6.32  2000/07/17 22:01:22  dondosha
 * Returned default for -J option to TRUE
 *
 * Revision 6.31  2000/07/17 21:53:37  dondosha
 * Set believe query to FALSE if want to print full defline
 *
 * Revision 6.30  2000/07/17 16:55:33  dondosha
 * Fixed uninitialized variable bug
 *
 * Revision 6.29  2000/07/14 18:04:09  dondosha
 * Set perform_culling option explicitly to FALSE; fix for the full defline id printing
 *
 * Revision 6.28  2000/07/11 16:47:33  dondosha
 * Save lower case masking information in options structure
 *
 * Revision 6.27  2000/07/06 17:28:11  dondosha
 * Added option to print full deflines in the output
 *
 * Revision 6.26  2000/06/28 18:30:53  dondosha
 * Another uninitialized variable in revision 6.24
 *
 * Revision 6.25  2000/06/28 15:57:16  dondosha
 * Fixed uninitialized variable bug from previous change
 *
 * Revision 6.24  2000/06/27 22:24:42  dondosha
 * Added option to output masked query sequences
 *
 * Revision 6.23  2000/06/22 14:30:21  dondosha
 * Added option to cut off hits by score
 *
 * Revision 6.22  2000/06/19 22:13:57  dondosha
 * Do not allow searching more than 1/2 INT2_MAX queries at a time (since last_context is an Int2)
 *
 * Revision 6.21  2000/06/15 17:35:34  dondosha
 * Removed program argument, changed master-slave to query-anchored
 *
 * Revision 6.20  2000/06/07 18:20:30  dondosha
 * Append result ASN.1 for all queries if seqalign file is specified
 *
 * Revision 6.19  2000/06/01 15:48:56  dondosha
 * Removed -K and -Y command line options
 *
 * Revision 6.18  2000/05/31 14:23:16  dondosha
 * Hold indexing of Bioseqs until after all of them are read
 *
 * Revision 6.17  2000/05/24 20:35:05  dondosha
 * Set cutoff_s parameter to wordsize - needed for reaping hitlists by evalue
 *
 * Revision 6.16  2000/05/17 21:28:12  dondosha
 * Fixed several memory leaks
 *
 * Revision 6.15  2000/05/17 17:40:54  dondosha
 * Removed unused variables; improved the way subject ids are printed in report; added maximal number of positions for a hash value parameter
 *
 * Revision 6.14  2000/05/12 19:52:53  dondosha
 * Use binary search to retrieve query id for printing results; increased maximal total query length default; do not free SeqEntries after last search
 *
 * Revision 6.13  2000/05/05 14:23:38  dondosha
 * Changed program name from mblastall to megablast
 *
 * Revision 6.12  2000/05/03 20:30:07  dondosha
 * Fixed memory leaks, changed score to number of differences for one-line output, added affine gapping
 *
 * Revision 6.11  2000/04/25 22:04:51  dondosha
 * Report number of differences instead of score in -D [01] mode
 *
 * Revision 6.10  2000/04/24 16:45:58  dondosha
 * Check evalues of hsps in the callbacks; default expect value 0 means it is ignored
 *
 * Revision 6.9  2000/04/21 19:43:29  dondosha
 * Added command line option: total length of queries for a single search
 *
 * Revision 6.8  2000/04/12 22:48:47  dondosha
 * Fixed synchronization problem in writing segments information
 *
 * Revision 6.7  2000/04/12 21:19:20  dondosha
 * Cleaned argument list
 *
 * Revision 6.6  2000/04/12 18:29:52  dondosha
 * Added callback MegaBlastPrintSegments, made BlastSearchHandleResults static
 *
 * Revision 6.5  2000/04/07 16:54:58  dondosha
 * Added option for segments only output; moved result handling callbacks from 
 * mblast.c
 *
 * Revision 6.4  2000/03/31 19:13:41  dondosha
 * Changed some names related to MegaBlast
 *
 * Revision 6.3  2000/02/17 17:42:58  dondosha
 * Added extra parameter to FastaToSeqEntryForDb call due to prototype change;
 * do not fill mask_loc to avoid sequence substitution when filtering
 *
 * Revision 6.2  2000/02/03 22:02:53  dondosha
 * Fixed some memory leaks, added option for one-line results printout
 *
 * Revision 6.1  2000/02/01 21:41:14  dondosha
 * Initial revision
 *
 *
*/

#include <objsset.h>
#include <blast.h>
#include <txalign.h>
#include <mblast.h>
#include <xmlblast.h>
#include <sqnutils.h>

#define DEFLINE_BUF 255


/* Used by the callback function. */
FILE *global_fp=NULL;
/*
	Callback to print out ticks, in UNIX only due to file systems
	portability issues.
*/
static int LIBCALLBACK
dummy_callback(Int4 sequence_number, Int4 number_of_positive_hits)
{
   return 0;
}

static int LIBCALLBACK
tick_callback(Int4 sequence_number, Int4 number_of_positive_hits)

{

#ifdef OS_UNIX
   
   fprintf(global_fp, "%s", ".");
   fflush(global_fp);
#endif
	return 0;
}

enum {
   MBLAST_ENDPOINTS = 0,
   MBLAST_SEGMENTS,
   MBLAST_ALIGNMENTS,
   MBLAST_ALIGN_INFO
};

static int LIBCALLBACK
MegaBlastPrintEndpoints(VoidPtr ptr)
{
   BlastSearchBlkPtr search = (BlastSearchBlkPtr) ptr;
   CharPtr subject_descr;
   SeqIdPtr sip, query_id;
   CharPtr query_buffer, title;
   CharPtr subject_buffer;
   Int4 query_length, q_start, q_end, q_shift=0, s_shift=0;
   Int4 subject_end;
   Int4 hsp_index;
   Boolean numeric_sip_type = FALSE;
   BLAST_HSPPtr hsp; 
   Int2 context, descr_len;
   Char context_sign;
   Uint4 header_index = 0;
   Int4 subject_gi, score;
   FILE *fp = (FILE *) search->output;

   if (search->current_hitlist == NULL || search->current_hitlist->hspcnt <= 0) {
      search->subject_info = BLASTSubjectInfoDestruct(search->subject_info);
      return 0;
   }

   if (search->rdfp)
      readdb_get_descriptor(search->rdfp, search->subject_id, &sip,
                            &subject_descr);
   else 
      sip = SeqIdSetDup(search->subject_info->sip);
   
   if (sip->choice != SEQID_GENERAL ||
       StringCmp(((DbtagPtr)sip->data.ptrvalue)->db, "BL_ORD_ID")) {
      if (search->pbp->mb_params->full_seqids) {
         subject_buffer = (CharPtr) Malloc(BUFFER_LENGTH + 1);
         SeqIdWrite(sip, subject_buffer, PRINTID_FASTA_LONG, BUFFER_LENGTH);
      } else
         numeric_sip_type = GetAccessionFromSeqId(SeqIdFindBest(sip, SEQID_GI), 
                                                  &subject_gi, &subject_buffer);
   } else {
      DbtagPtr db_tag = (DbtagPtr) sip->data.ptrvalue;
      if (db_tag->db && 
          (!StringCmp(db_tag->db, "THC") || 
           !StringICmp(db_tag->db, "TI")) && 
          db_tag->tag->id != 0) {
         subject_buffer = (CharPtr) Malloc(16);
         sprintf(subject_buffer, "%ld", db_tag->tag->id);
      } else {
         subject_buffer = StringTokMT(subject_descr, " \t", &subject_descr);
         subject_descr = subject_buffer;
      }
   }

   search->current_hitlist->hspcnt_max = search->current_hitlist->hspcnt;

   /* Only for the two sequences case, get offset shift if subject 
      is a subsequence */
   if (!search->rdfp && search->query_slp->next) {
       s_shift = SeqLocStart(search->query_slp->next);
       subject_end = SeqLocStop(search->query_slp->next);
   } else {
      s_shift = 0;
      subject_end = 
         readdb_get_sequence_length(search->rdfp, search->subject_id);
   }
   /* Get offset shift if query is a subsequence */
   q_shift = SeqLocStart(search->query_slp);

   for (hsp_index=0; hsp_index<search->current_hitlist->hspcnt; hsp_index++) {
      hsp = search->current_hitlist->hsp_array[hsp_index];
      if (hsp==NULL || (search->pbp->cutoff_e > 0 && 
	  hsp->evalue > search->pbp->cutoff_e)) 
	 continue;
      
      /* Correct query context is already found in BlastGetNonSumStatsEvalue */
      context = hsp->context; 
      query_id = search->qid_array[context/2];


      if (query_id == NULL) /* Bad hsp, something wrong */
	 continue; 
      hsp->context = context & 1;      
      query_length = search->query_context_offsets[context+1] -
         search->query_context_offsets[context] - 1;
      hsp->subject.end = hsp->subject.offset + hsp->subject.length;

      if (hsp->context) {
	 hsp->query.end = query_length - hsp->query.offset;
	 hsp->query.offset = 
	    hsp->query.end - hsp->query.length + 1;
	 context_sign = '-'; 
      } else {
	 hsp->query.end = (++hsp->query.offset) + hsp->query.length - 1;
         if (hsp->query.end > query_length) {
            hsp->subject.end -= (hsp->query.end - query_length);
            hsp->query.end = query_length;
         }
	 context_sign = '+';  
      }
      
      if (hsp->subject.end > subject_end) {
         hsp->query.end -= (hsp->subject.end - subject_end);
         hsp->subject.end = subject_end;
      }
      hsp->subject.offset++;
      
      query_buffer = NULL;
      if (query_id->choice == SEQID_LOCAL && 
          search->pbp->mb_params->full_seqids) {
         BioseqPtr query_bsp = BioseqLockById(query_id);
         title = StringSave(BioseqGetTitle(query_bsp));
         if (title)
            query_buffer = StringTokMT(title, " ", &title);
         else {
            Int4 query_gi;
            Boolean numeric_query_id =
               GetAccessionFromSeqId(query_bsp->id, &query_gi,
                                     &query_buffer);
         }  
         BioseqUnlock(query_bsp);
      } else {
         query_buffer = (CharPtr) Malloc(BUFFER_LENGTH + 1);
         if (!search->pbp->mb_params->full_seqids)
            SeqIdWrite(query_id, query_buffer, PRINTID_TEXTID_ACCESSION,
                       BUFFER_LENGTH);
         else 
            SeqIdWrite(query_id, query_buffer, PRINTID_FASTA_LONG,
                    BUFFER_LENGTH);
      }

      if (search->pbp->gap_open==0 && search->pbp->gap_extend==0)
	 score = ((hsp->subject.length + hsp->query.length)*
		   search->sbp->reward / 2 - hsp->score) / 
	    (search->sbp->reward - search->sbp->penalty);
      else 
	 score = hsp->score;

      if (context_sign == '+') {
	 q_start = hsp->query.offset;
	 q_end = hsp->query.end;
      } else {
	 q_start = hsp->query.end;
	 q_end = hsp->query.offset;
      }

      /* Adjust offsets if query is a subsequence, only for first query */
      if (context < 2) {
          q_start += q_shift;
          q_end += q_shift;
      }

      hsp->subject.offset += s_shift;
      hsp->subject.end += s_shift;

      if (numeric_sip_type)
	 fprintf(fp, "'%ld'=='%c%s' (%d %d %d %d) %d\n", subject_gi, 
		 context_sign, query_buffer, hsp->subject.offset, q_start, 
		 hsp->subject.end, q_end, score);
      else 
	 fprintf(fp, "'%s'=='%c%s' (%d %d %d %d) %d\n", 
		 subject_buffer, context_sign, query_buffer, 
		 hsp->subject.offset, q_start, 
		 hsp->subject.end, q_end, score);
      MemFree(query_buffer);
   }
   if (!numeric_sip_type && subject_buffer != subject_descr)
      MemFree(subject_buffer);
   MemFree(subject_descr);
   sip = SeqIdSetFree(sip);
   return 0;
}

static int LIBCALLBACK
MegaBlastPrintSegments(VoidPtr ptr)
{
   BlastSearchBlkPtr search = (BlastSearchBlkPtr) ptr;
   ReadDBFILEPtr rdfp = search->rdfp;
   BLAST_HSPPtr hsp; 
   Int4 i, subject_gi;
   Int2 context;
   CharPtr query_buffer, title;
   SeqIdPtr sip, query_id; 
   Int4 descr_len;
   Int4 index, hsp_index, score;
   Uint1Ptr query_seq, subject_seq = NULL;
   FloatHi perc_ident;
   Char strand;
   GapXEditScriptPtr esp;
   Int4 q_start, q_end, s_start, s_end, query_length, subj_length, numseg;
   Int4 q_off, num_ident, align_length, total_ident, q_shift=0, s_shift=0;
   Int4Ptr length, start;
   Uint1Ptr strands;
   CharPtr subject_descr, subject_buffer, buffer;
   Char tmp_buffer[BUFFER_LENGTH];
   Int4 buffer_size, max_buffer_size = LARGE_BUFFER_LENGTH;
   Boolean numeric_sip_type = FALSE;
   FILE *fp = (FILE *) search->output;

   if (search->current_hitlist == NULL || search->current_hitlist->hspcnt <= 0) {
      search->subject_info = BLASTSubjectInfoDestruct(search->subject_info);
      return 0;
   }

   subject_seq = search->subject->sequence_start + 1;


   if (search->rdfp)
      readdb_get_descriptor(search->rdfp, search->subject_id, &sip, &subject_descr);
   else 
      sip = SeqIdSetDup(search->subject_info->sip);

   if (sip->choice != SEQID_GENERAL ||
       StringCmp(((DbtagPtr)sip->data.ptrvalue)->db, "BL_ORD_ID")) {
      if (search->pbp->mb_params->full_seqids) { 
         subject_buffer = (CharPtr) Malloc(BUFFER_LENGTH + 1);
         SeqIdWrite(sip, subject_buffer, PRINTID_FASTA_LONG, BUFFER_LENGTH);
      } else
         numeric_sip_type = GetAccessionFromSeqId(SeqIdFindBest(sip, SEQID_GI), 
                                                  &subject_gi, &subject_buffer);
   } else {
      subject_buffer = StringTokMT(subject_descr, " \t", &subject_descr);
      subject_descr = subject_buffer;
   }

   buffer = (CharPtr) Malloc(LARGE_BUFFER_LENGTH);

   /* Only for the two sequences case, get offset shift if subject 
      is a subsequence */
   if (!search->rdfp && search->query_slp->next)
       s_shift = SeqLocStart(search->query_slp->next);
   /* Get offset shift if query is a subsequence */
   q_shift = SeqLocStart(search->query_slp);
   
   for (hsp_index=0; hsp_index<search->current_hitlist->hspcnt; hsp_index++) {
      hsp = search->current_hitlist->hsp_array[hsp_index];
      if (hsp==NULL || (search->pbp->cutoff_e > 0 && 
                        hsp->evalue > search->pbp->cutoff_e)) {
         GapXEditBlockDelete(hsp->gap_info); /* Don't need it anymore */
	 continue;
      }
      context = hsp->context;
      query_id = search->qid_array[context/2];

     
      if (query_id == NULL) /* Bad hsp, something wrong */
	 continue; 
      hsp->context = context & 1;

      if (search->pbp->gap_open==0 && search->pbp->gap_extend==0)
	 score = ((hsp->subject.length + hsp->query.length)*
		   search->sbp->reward / 2 - hsp->score) / 
	    (search->sbp->reward - search->sbp->penalty);
      else 
	 score = hsp->score;

      query_length = search->query_context_offsets[context+1] -
         search->query_context_offsets[context] - 1;

      q_off = hsp->query.offset;
      if (hsp->context) {
	 strand = '-'; 
	 hsp->query.end = query_length - hsp->query.offset;
	 hsp->query.offset = 
	    hsp->query.end - hsp->query.length;
      } else {
	 strand = '+';  
	 hsp->query.end = hsp->query.offset + hsp->query.length;
      }

      if (strand == '+') {
	 q_start = hsp->query.offset + 1;
	 q_end = hsp->query.end;
      } else {
	 q_start = hsp->query.end;
	 q_end = hsp->query.offset + 1;
      }
      s_start = hsp->subject.offset + 1;
      s_end = hsp->subject.offset + hsp->subject.length;

      /* Adjust offsets if query is a subsequence, only for first query */
      if (context < 2) {
          q_start += q_shift;
          q_end += q_shift;
      }

      s_start += s_shift;
      s_end += s_shift;

      if (query_id->choice == SEQID_LOCAL && 
          search->pbp->mb_params->full_seqids) {
         BioseqPtr query_bsp = BioseqLockById(query_id);
         title = StringSave(BioseqGetTitle(query_bsp));
         if (title)
            query_buffer = StringTokMT(title, " ", &title);
         else {
            Int4 query_gi;
            Boolean numeric_query_id =
               GetAccessionFromSeqId(query_bsp->id, &query_gi,
                                     &query_buffer);
         }  
         BioseqUnlock(query_bsp);
      } else {
         query_buffer = (CharPtr) Malloc(BUFFER_LENGTH + 1);
         if (!search->pbp->mb_params->full_seqids)
            SeqIdWrite(query_id, query_buffer, PRINTID_TEXTID_ACCESSION,
                       BUFFER_LENGTH);
         else 
            SeqIdWrite(query_id, query_buffer, PRINTID_FASTA_LONG,
                    BUFFER_LENGTH);
      }

      if (numeric_sip_type)
	 sprintf(buffer, "\n#'>%ld'=='%c%s' (%d %d %d %d) %d\na {\n  s %d\n  b %d %d\n  e %d %d\n", 
	      subject_gi, strand, query_buffer, 
	      s_start, q_start, s_end, q_end, score, score, 
	      s_start, q_start, s_end, q_end);
      else 
	 sprintf(buffer, "\n#'>%s'=='%c%s' (%d %d %d %d) %d\na {\n  s %d\n  b %d %d\n  e %d %d\n", 
	      subject_buffer, strand, query_buffer, 
	      s_start, q_start, s_end, q_end, score, score, 
	      s_start, q_start, s_end, q_end);
      buffer_size = StringLen(buffer);

      query_seq = search->context[context].query->sequence;

      esp = hsp->gap_info->esp;
        
      for (numseg=0; esp; esp = esp->next, numseg++);

      GXECollectDataForSeqalign(hsp->gap_info, hsp->gap_info->esp, numseg,
				&start, &length, &strands, 
				&q_off, &hsp->subject.offset);
      GapXEditBlockDelete(hsp->gap_info); /* Don't need it anymore */

      if (start[0] < 0) {
         length[0] += start[0];
         start[1] -= start[0];
         start[0] = 0;
      } 
      if (start[2*(numseg-1)] + length[numseg-1] > query_length) 
         length[numseg-1] = query_length - start[2*(numseg-1)];
      
      total_ident = 0;
      align_length = 0;
      for (i=0; i<numseg; i++) {
         align_length += length[i];
	 if (strand == '+') {
	    q_start = start[2*i] + 1;
	    q_end = q_start + length[i] - 1;
	 } else {
	    q_start = query_length - start[2*i];
	    q_end = q_start - length[i] + 1;
	 }
	 if (start[2*i] != -1 && start[2*i+1] != -1) {
	    num_ident = MegaBlastGetNumIdentical(query_seq, subject_seq, 
                                                 start[2*i], start[2*i+1], 
                                                 length[i], FALSE);
            perc_ident = (FloatHi) num_ident / length[i] * 100;
            total_ident += num_ident;
	    sprintf(tmp_buffer, "  l %d %d %d %d (%.0f)\n", start[2*i+1]+1, 
		    q_start, start[2*i+1]+length[i],
		    q_end, perc_ident);	 
	    if ((buffer_size += StringLen(tmp_buffer)) > max_buffer_size - 2) {
	       max_buffer_size *= 2;
	       buffer = (CharPtr) Realloc(buffer, max_buffer_size);
	    }
	    StringCat(buffer, tmp_buffer);
	 }
      }
      if (100*total_ident < align_length*search->pbp->mb_params->perc_identity) {
         GapXEditBlockDelete(hsp->gap_info); /* Don't need it anymore */
         continue;
      }
      StringCat(buffer, "}");
      fprintf(fp, "%s\n", buffer);
      MemFree(start);
      MemFree(length);
      MemFree(strands);
      MemFree(query_buffer);
   } /* End loop on hsp's */
   if (!numeric_sip_type && subject_buffer != subject_descr)
      MemFree(subject_buffer);
   MemFree(subject_descr);
   MemFree(buffer);
   sip = SeqIdSetFree(sip);
   fflush(fp);
   return 1;
}

#define NUMARG (sizeof(myargs)/sizeof(myargs[0]))

static Args myargs [] = {
  { "Database", 
    "nr", NULL, NULL, FALSE, 'd', ARG_STRING, 0.0, 0, NULL},       /* 0 */
  { "Query File", 
	"stdin", NULL, NULL, FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},/* 1 */
  { "Expectation value", 
	"1000000.0", NULL, NULL, FALSE, 'e', ARG_FLOAT, 0.0, 0, NULL},/* 2 */
  { "alignment view options:\n0 = pairwise,\n1 = query-anchored showing identities,\n2 = query-anchored no identities,\n3 = flat query-anchored, show identities,\n4 = flat query-anchored, no identities,\n5 = query-anchored no identities and blunt ends,\n6 = flat query-anchored, no identities and blunt ends,\n7 = XML Blast output", 
        "0", NULL, NULL, FALSE, 'm', ARG_INT, 0.0, 0, NULL},       /* 3 */
  { "BLAST report Output File", 
	"stdout", NULL, NULL, TRUE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},/* 4 */
  { "Filter query sequence",
        "T", NULL, NULL, FALSE, 'F', ARG_STRING, 0.0, 0, NULL},    /* 5 */
  { "X dropoff value for gapped alignment (in bits)",
	"20", NULL, NULL, FALSE, 'X', ARG_INT, 0.0, 0, NULL},      /* 6 */
  { "Show GI's in deflines",
        "F", NULL, NULL, FALSE, 'I', ARG_BOOLEAN, 0.0, 0, NULL},   /* 7 */
  { "Penalty for a nucleotide mismatch",
	"-3", NULL, NULL, FALSE, 'q', ARG_INT, 0.0, 0, NULL},      /* 8 */
  { "Reward for a nucleotide match",
	"1", NULL, NULL, FALSE, 'r', ARG_INT, 0.0, 0, NULL},       /* 9 */
  { "Number of database sequences to show one-line descriptions for (V)",
        "500", NULL, NULL, FALSE, 'v', ARG_INT, 0.0, 0, NULL},     /* 10 */
  { "Number of database sequence to show alignments for (B)",
        "250", NULL, NULL, FALSE, 'b', ARG_INT, 0.0, 0, NULL},     /* 11 */
  { "Type of output:\n0 - alignment endpoints and score,\n1 - all ungapped segments endpoints,\n2 - traditional BLAST output,\n3 - tab-delimited one line format",
        "0", NULL, NULL, FALSE, 'D', ARG_INT, 0.0, 0, NULL},       /* 12 */
  { "Number of processors to use",
        "1", NULL, NULL, FALSE, 'a', ARG_INT, 0.0, 0, NULL},       /* 13 */
  { "ASN.1 SeqAlign file", 
	NULL, NULL, NULL, TRUE, 'O', ARG_FILE_OUT, 0.0, 0, NULL},  /* 14 */
  { "Believe the query defline",
        NULL, NULL, NULL, TRUE, 'J', ARG_STRING, 0.0, 0, NULL},    /* 15 */
  { "Maximal total length of queries for a single search",
        "20000000", NULL, NULL, FALSE, 'M', ARG_INT, 0.0, 0, NULL},/* 16 */
  { "Word size (length of best perfect match)", 
        "28", NULL, NULL, FALSE, 'W', ARG_INT, 0.0, 0, NULL},      /* 17 */
  { "Effective length of the database (use zero for the real size)", 
        "0", NULL, NULL, FALSE, 'z', ARG_STRING, 0.0, 0, NULL},     /* 18 */
  { "Maximal number of positions for a hash value (set to 0 to ignore)",
        "0", NULL, NULL, FALSE, 'P', ARG_INT, 0.0, 0, NULL},       /* 19 */
  { "Query strands to search against database: 3 is both, 1 is top, 2 is bottom",
        "3", NULL, NULL, FALSE, 'S', ARG_INT, 0.0, 0, NULL},       /* 20 */
  { "Produce HTML output",
        "F", NULL, NULL, FALSE, 'T', ARG_BOOLEAN, 0.0, 0, NULL},   /* 21 */
  { "Restrict search of database to list of GI's",
	NULL, NULL, NULL, TRUE, 'l', ARG_STRING, 0.0, 0, NULL},    /* 22 */
  { "Cost to open a gap (zero invokes default behavior)",          /* 23 */
        "0", NULL, NULL, FALSE, 'G', ARG_INT, 0.0, 0, NULL},
  { "Cost to extend a gap (zero invokes default behavior)",        /* 24 */
        "0", NULL, NULL, FALSE, 'E', ARG_INT, 0.0, 0, NULL},
  { "Minimal hit score to report (0 for default behavior)",        /* 25 */
        "0", NULL, NULL, FALSE, 's', ARG_INT, 0.0, 0, NULL},
  { "Masked query output", 
	NULL, NULL, NULL, TRUE, 'Q', ARG_FILE_OUT, 0.0, 0, NULL},  /* 26 */
  { "Show full IDs in the output (default - only GIs or accessions)",
        "F", NULL, NULL, FALSE, 'f', ARG_BOOLEAN, 0.0, 0, NULL},   /* 27 */
  {"Use lower case filtering of FASTA sequence",                   
        "F", NULL, NULL, TRUE, 'U', ARG_BOOLEAN, 0.0, 0, NULL},    /* 28 */
  {"Report the log information at the end of output",
        "F", NULL, NULL, TRUE, 'R', ARG_BOOLEAN, 0.0, 0, NULL},    /* 29 */
  {"Identity percentage cut-off", 
        "0", NULL, NULL, FALSE, 'p', ARG_FLOAT, 0.0, 0, NULL},     /* 30 */
  { "Location on query sequence",                             
        NULL, NULL, NULL, TRUE, 'L', ARG_STRING, 0.0, 0, NULL},    /* 31 */
  { "Multiple Hits window size (zero for single hit algorithm)",   /* 32 */
        "0", NULL, NULL, FALSE, 'A', ARG_INT, 0.0, 0, NULL},
  { "X dropoff value for ungapped extension",
	"10", NULL, NULL, FALSE, 'y', ARG_INT, 0.0, 0, NULL},      /* 33 */
  { "X dropoff value for dynamic programming gapped extension",
	"50", NULL, NULL, FALSE, 'Z', ARG_INT, 0.0, 0, NULL}       /* 34 */
};

#define MAX_NUM_QUERIES 16383 /* == 1/2 INT2_MAX */

Int2 Main (void)
 
{
	AsnIoPtr aip, xml_aip;
	BioseqPtr query_bsp, PNTR query_bsp_array;
	BioSourcePtr source;
	BLAST_MatrixPtr matrix;
	BLAST_OptionsBlkPtr options;
	BLAST_KarlinBlkPtr ka_params=NULL, ka_params_gap=NULL;
	BlastPruneSapStructPtr prune;
	Boolean db_is_na, query_is_na, show_gi, believe_query=FALSE;
	Boolean html=FALSE;
	CharPtr params_buffer=NULL;
	Int4 number_of_descriptions, number_of_alignments;
	SeqAlignPtr  seqalign, PNTR seqalign_array;
        SeqAnnotPtr seqannot;
	SeqEntryPtr PNTR sepp;
	TxDfDbInfoPtr dbinfo=NULL, dbinfo_head;
	Uint1 align_type, align_view;
	Uint4 align_options, print_options;
	ValNodePtr  mask_loc, mask_loc_start, vnp, other_returns, error_returns;

	CharPtr blast_program, blast_database, blast_inputfile, blast_outputfile;
	FILE *infp, *outfp;
	Int4 index, num_bsps, total_length, total_processed = 0;
	Int2 ctr = 1;
	Char prefix[2];
	SeqLocPtr last_mask, mask_slp;
	Boolean done, first_seq = TRUE, hits_found;
	CharPtr masked_query_file;
        const char *dummystr;

        if (! GetArgs ("megablast", NUMARG, myargs))
	   return (1);

	UseLocalAsnloadDataAndErrMsg ();

	if (! SeqEntryLoad())
		return 1;

	ErrSetMessageLevel(SEV_WARNING);

	blast_program = "blastn";
        blast_database = myargs [0].strvalue;
        blast_inputfile = myargs [1].strvalue;
        blast_outputfile = myargs [4].strvalue;
	if (myargs[21].intvalue)
		html = TRUE;

	if ((infp = FileOpen(blast_inputfile, "r")) == NULL) {
	   ErrPostEx(SEV_FATAL, 0, 0, "blast: Unable to open input file %s\n", blast_inputfile);
	   return (1);
	}

	outfp = NULL;
	if (blast_outputfile != NULL) {
	   if ((outfp = FileOpen(blast_outputfile, "w")) == NULL) {
	      ErrPostEx(SEV_FATAL, 0, 0, "blast: Unable to open output file %s\n", blast_outputfile);
	      return (1);
	   }
	}

	align_view = (Int1) myargs[3].intvalue;

	align_type = BlastGetTypes(blast_program, &query_is_na, &db_is_na);
 
        if (myargs[15].strvalue) {
           if (myargs[15].strvalue[0] == 'f' || myargs[15].strvalue[0] == 'F' ||
               myargs[15].strvalue[0] == '0')
              believe_query = FALSE;
           else
              believe_query = TRUE;
        } else if (myargs[12].intvalue == MBLAST_ALIGNMENTS && 
                   !myargs[14].strvalue)
           believe_query = FALSE;
        else
           believe_query = TRUE;
        
        
	if (believe_query == FALSE && myargs[14].strvalue) 
	   ErrPostEx(SEV_FATAL, 0, 0, "-J option must be TRUE to produce a SeqAlign file");

	options = BLASTOptionNew(blast_program, TRUE);
	if (options == NULL)
		return 3;

	options->do_sum_stats = FALSE;
	options->is_neighboring = FALSE;
        options->expect_value  = (Nlm_FloatHi) myargs [2].floatvalue;
	number_of_descriptions = myargs[10].intvalue;	
	number_of_alignments = myargs[11].intvalue;	
	options->hitlist_size = MAX(number_of_descriptions, number_of_alignments);

	if (myargs[6].intvalue != 0)
           options->gap_x_dropoff = myargs[6].intvalue;
	if (myargs[33].intvalue != 0)
           options->dropoff_2nd_pass = myargs[33].intvalue;
        if (myargs[34].intvalue != 0)
           options->gap_x_dropoff_final = myargs[34].intvalue;

	if (StringICmp(myargs[5].strvalue, "T") == 0)
	   options->filter_string = StringSave("D");
	else
	   options->filter_string = StringSave(myargs[5].strvalue);
	
	show_gi = (Boolean) myargs[7].intvalue;
	options->penalty = myargs[8].intvalue;
	options->reward = myargs[9].intvalue;
	options->gap_open = myargs[23].intvalue;
	options->gap_extend = myargs[24].intvalue;

	if (options->gap_open == 0 && options->reward % 2 == 0 && 
	    options->gap_extend == options->reward / 2 - options->penalty)
	   /* This is the default value */
	   options->gap_extend = 0;

	options->genetic_code = 1;
	options->db_genetic_code = 1; /* Default; it's not needed here anyway */
	options->number_of_cpus = myargs[13].intvalue;
	if (myargs[17].intvalue != 0)
           options->wordsize = myargs[17].intvalue;
        if (myargs[25].intvalue == 0)
           options->cutoff_s2 = options->wordsize*options->reward;
        else 
           options->cutoff_s2 = myargs[25].intvalue;

	options->cutoff_s = (options->wordsize + 4)*options->reward;

        options->db_length = StringToInt8(myargs[18].strvalue, &dummystr);

	options->perform_culling = FALSE;
	/* Kludge */
        options->block_width  = myargs[19].intvalue;

	options->strand_option = myargs[20].intvalue;
        options->window_size = myargs[32].intvalue;

        print_options = 0;
        align_options = 0;
        align_options += TXALIGN_COMPRESS;
        align_options += TXALIGN_END_NUM;
        if (show_gi) {
	   align_options += TXALIGN_SHOW_GI;
	   print_options += TXALIGN_SHOW_GI;
        }
			
        if (align_view) {
	   align_options += TXALIGN_MASTER;
	   if (align_view == 1 || align_view == 3)
	      align_options += TXALIGN_MISMATCH;
	   if (align_view == 3 || align_view == 4 || align_view == 6)
	      align_options += TXALIGN_FLAT_INS;
	   if (align_view == 5 || align_view == 6)
	      align_options += TXALIGN_BLUNT_END;
        } else {
	   align_options += TXALIGN_MATRIX_VAL;
	   align_options += TXALIGN_SHOW_QS;
	}

	if (html) {
	   align_options += TXALIGN_HTML;
	   print_options += TXALIGN_HTML;
	}

	if (myargs[22].strvalue)
	   options->gifile = StringSave(myargs[22].strvalue);
   
	options->is_megablast_search = TRUE;
	if (myargs[12].intvalue == MBLAST_ENDPOINTS)
	   options->no_traceback = TRUE;
	else
	   options->no_traceback = FALSE;

	options->megablast_full_deflines = (Boolean) myargs[27].intvalue;
        options->perc_identity = (FloatLo) myargs[30].floatvalue;
        if (!believe_query)
           options->megablast_full_deflines = TRUE;
        /*if (options->megablast_full_deflines)
          believe_query = FALSE;*/

	query_bsp_array = (BioseqPtr PNTR) MemNew(MAX_NUM_QUERIES*sizeof(BioseqPtr));
	sepp = (SeqEntryPtr PNTR) MemNew(MAX_NUM_QUERIES*sizeof(SeqEntryPtr));

	StrCpy(prefix, "");

	global_fp = outfp;
        if (myargs[12].intvalue != MBLAST_ALIGNMENTS)
           options->output = outfp;

	if (myargs[12].intvalue==MBLAST_ALIGNMENTS) {
	   if (align_view != 7) {
              if (html) {
                 fprintf(outfp, "<HTML>\n<TITLE>MEGABLAST Search Results</TITLE>\n");
                 fprintf(outfp, "<BODY BGCOLOR=\"#FFFFFF\" LINK=\"#0000FF\" "
                         "VLINK=\"#660099\" ALINK=\"#660099\">\n");
                 fprintf(outfp, "<PRE>\n");
              }
              init_buff_ex(90);
              BlastPrintVersionInfo("megablast", html, outfp);
              fprintf(outfp, "\n");
              MegaBlastPrintReference(html, 90, outfp);
              fprintf(outfp, "\n");
              
              if(!PrintDbInformation(blast_database, !db_is_na, 70, outfp, html))
                 return 1;
              
              free_buff();
	
#ifdef OS_UNIX
              fprintf(global_fp, "%s", "Searching");
#endif
           }
	}
	
        aip = NULL;
        if (myargs[14].strvalue != NULL) {
           if ((aip = AsnIoOpen (myargs[14].strvalue,"w")) == NULL) {
              ErrPostEx(SEV_FATAL, 0, 0, "blast: Unable to open output file %s\n", "blastngp.sat");
              return 1;
           }
        }

        if (align_view == 7)
           xml_aip = AsnIoOpen(blast_outputfile, "wx");

        if (myargs[31].strvalue) {       
            CharPtr delimiters = " ,;";
            CharPtr location;
            location = myargs[31].strvalue;
            options->required_start = 
                atoi(StringTokMT(location, delimiters, &location)) - 1;
            options->required_end = atoi(location) - 1;
        }

	done = FALSE;
	while (!done) {
	   num_bsps = 0;
	   total_length = 0;
	   done = TRUE;
	   SeqMgrHoldIndexing(TRUE);
	   mask_slp = last_mask = NULL;
   
	   while ((sepp[num_bsps]=FastaToSeqEntryForDb(infp, query_is_na, NULL,
						       believe_query, prefix, &ctr, 
						       &mask_slp)) != NULL) {
              if (!myargs[28].intvalue) /* Lower case ignored */
                 mask_slp = SeqLocFree(mask_slp);
	      if (mask_slp) {
		 if (!last_mask)
		    options->query_lcase_mask = last_mask = mask_slp;
		 else {
		    last_mask->next = mask_slp;
		    last_mask = last_mask->next;
		 }
		 mask_slp = NULL;
	      }
	      query_bsp = NULL;
              SeqEntryExplore(sepp[num_bsps], &query_bsp, FindNuc);

	      if (query_bsp == NULL) {
		 ErrPostEx(SEV_FATAL, 0, 0, "Unable to obtain bioseq\n");
		 return 2;
	      }
	      
	      source = BioSourceNew();
	      source->org = OrgRefNew();
	      source->org->orgname = OrgNameNew();
	      source->org->orgname->gcode = options->genetic_code;
	      ValNodeAddPointer(&(query_bsp->descr), Seq_descr_source, source);
	      
	      query_bsp_array[num_bsps++] = query_bsp;
	      
	      total_length += query_bsp->length;
	      if (total_length > myargs[16].intvalue || 
		  num_bsps >= MAX_NUM_QUERIES) {
		 done = FALSE;
		 break;
	      }
	   }
	   SeqMgrHoldIndexing(FALSE);
	   other_returns = NULL;
	   error_returns = NULL;
	   
	   ReadDBBioseqFetchEnable ("megablast", blast_database, db_is_na, TRUE);
#if 0
	   fprintf(stderr, "Process %d queries with total length %ld\n", 
		   num_bsps, total_length);
#endif
	   if (myargs[12].intvalue==MBLAST_ENDPOINTS) 
	      seqalign_array = BioseqMegaBlastEngine(query_bsp_array, blast_program,
						     blast_database, options,
						     &other_returns, &error_returns,
						     dummy_callback, NULL, NULL, 0, 
						     MegaBlastPrintEndpoints);
	   else if (myargs[12].intvalue==MBLAST_SEGMENTS) 
	      seqalign_array = BioseqMegaBlastEngine(query_bsp_array, blast_program,
						     blast_database, options,
						     &other_returns, &error_returns,
						     dummy_callback, NULL, NULL, 0,
						     MegaBlastPrintSegments);
	   else if (myargs[12].intvalue==MBLAST_ALIGN_INFO) {
              PrintTabularOutputHeader(blast_database, 
                                       (num_bsps==1) ? query_bsp_array[0] : NULL,
                                       NULL, "megablast", 0, believe_query,
                                       global_fp);
	      seqalign_array = BioseqMegaBlastEngine(query_bsp_array, blast_program,
						     blast_database, options,
						     &other_returns, &error_returns,
						     dummy_callback, NULL, NULL, 0,
						     MegaBlastPrintAlignInfo);
	   } else /* if (myargs[12].intvalue==MBLAST_ALIGNMENTS) */
	      seqalign_array = BioseqMegaBlastEngine(query_bsp_array, blast_program,
				  blast_database, options, &other_returns, 
                                  &error_returns, align_view != 7 ? tick_callback : NULL,
                                  NULL, NULL, 0, NULL);
	   
#ifdef OS_UNIX
	   fflush(global_fp);
#endif

           if (error_returns) {
              BlastErrorPrint(error_returns);
              for (vnp = error_returns; vnp; vnp = vnp->next) {
                 BlastDestroyErrorMessage((BlastErrorMsgPtr)vnp->data.ptrvalue);
              }
              ValNodeFree(error_returns);
           }
              
              
	   if (myargs[12].intvalue==MBLAST_ALIGNMENTS) {
	      dbinfo = NULL;
	      ka_params = NULL;
	      ka_params_gap = NULL;
	      params_buffer = NULL;
	      mask_loc = NULL;
	      matrix = NULL;
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
		    matrix = vnp->data.ptrvalue;
		    break;
		 case SEQLOC_MASKING_NOTSET:
		 case SEQLOC_MASKING_PLUS1:
		 case SEQLOC_MASKING_PLUS2:
		 case SEQLOC_MASKING_PLUS3:
		 case SEQLOC_MASKING_MINUS1:
		 case SEQLOC_MASKING_MINUS2:
		 case SEQLOC_MASKING_MINUS3:
		    /*ValNodeAddPointer(&mask_loc, vnp->choice, vnp->data.ptrvalue);*/
		    break;
		 default:
		    break;
		 }
	      }	
	      
#ifdef OS_UNIX
              if(align_view != 7) {
                 fprintf(global_fp, "%s\n", " done");
              }
#endif
	      
              masked_query_file = myargs[26].strvalue;
              hits_found = FALSE;

              if (seqalign_array) {
                 for (index=0; index<num_bsps; index++) {
                    if (seqalign_array[index]==NULL) continue;
                    hits_found = TRUE;
                    if (align_view != 7) {
                       init_buff_ex(70);
                       AcknowledgeBlastQuery(query_bsp_array[index], 70, outfp, 
                                             believe_query, html);
                       free_buff();
                    }
                    seqalign = seqalign_array[index];
                    if (masked_query_file) {
                       SeqLocPtr query_slp = NULL;
                       ValNodeAddPointer(&query_slp, SEQLOC_WHOLE, SeqIdDup(SeqIdFindBest(query_bsp_array[index]->id, SEQID_GI)));
                       mask_slp = MaskSeqLocFromSeqAlign(seqalign);
                       PrintMaskedSequence(query_bsp_array[index], mask_slp,
                                           masked_query_file, first_seq);
                       first_seq = FALSE;
                       SeqLocSetFree(mask_slp);
                    }
                    if(align_view == 7) {
                       BXMLPrintOutput(xml_aip, seqalign, 
                                       options, "megablast", blast_database, 
                                       query_bsp_array[index], other_returns, 0, NULL);
                       AsnIoReset(xml_aip);
                       SeqAlignSetFree(seqalign);
                       continue;
                    }
                    seqannot = SeqAnnotNew();
                    seqannot->type = 2;
                    AddAlignInfoToSeqAnnot(seqannot, align_type);
                    seqannot->data = seqalign;
                    if (aip) {
                       SeqAnnotAsnWrite((SeqAnnotPtr) seqannot, aip, NULL);
                       AsnIoReset(aip);
                    }
                    if (outfp) { /* Uncacheing causes problems with ordinal nos. vs. gi's. */
                       prune = BlastPruneHitsFromSeqAlign(seqalign, number_of_descriptions, NULL);
                       ObjMgrSetHold();
                       init_buff_ex(85);
                       PrintDefLinesFromSeqAlign(prune->sap, 80,
                                                 outfp, print_options, FIRST_PASS, NULL);
                       free_buff();
                       
                       prune = BlastPruneHitsFromSeqAlign(seqalign, number_of_alignments, prune);
                       seqannot->data = prune->sap;
                       if (align_view != 0)
                          ShowTextAlignFromAnnot(seqannot, 60, outfp, NULL,
                                                 NULL, align_options, NULL, 
                                                 mask_loc, NULL);
                       else
                          ShowTextAlignFromAnnot(seqannot, 60, outfp, NULL, NULL, align_options, NULL, mask_loc, FormatScoreFunc);
                       seqannot->data = seqalign;
                       prune = BlastPruneSapStructDestruct(prune);
                       ObjMgrClearHold();
                       ObjMgrFreeCache(0);
                    }
                    seqannot = SeqAnnotFree(seqannot);
                 } /* End loop on seqaligns for different queries */
              } 

              if (!hits_found)
                 fprintf(outfp, "\n\n ***** No hits found ******\n\n");

              matrix = BLAST_MatrixDestruct(matrix);
	      
              if(html) 
                 fprintf(outfp, "<PRE>\n");
              init_buff_ex(85);
              dbinfo_head = dbinfo;
              if(align_view != 7) {
                 while (dbinfo) {
                    PrintDbReport(dbinfo, 70, outfp);
                    dbinfo = dbinfo->next;
                 }
              }
              dbinfo_head = TxDfDbInfoDestruct(dbinfo_head);
              
              if (ka_params) {
                 if(align_view != 7)
                    PrintKAParameters(ka_params->Lambda, ka_params->K, ka_params->H, 70, outfp, FALSE);
                 MemFree(ka_params);
              }
              if (ka_params_gap) {
                 if(align_view != 7)
                    PrintKAParameters(ka_params_gap->Lambda, ka_params_gap->K, ka_params_gap->H, 70, outfp, TRUE);
                 MemFree(ka_params_gap);
              }
              if(align_view != 7)
                 PrintTildeSepLines(params_buffer, 70, outfp);
              MemFree(params_buffer);
              free_buff();
              mask_loc_start = mask_loc;
              while (mask_loc) {
                 SeqLocSetFree(mask_loc->data.ptrvalue);
                 mask_loc = mask_loc->next;
              }
              ValNodeFree(mask_loc_start);
              
              ReadDBBioseqFetchDisable();
	   } else {
	      /* Just destruct all other_returns parts */
	      for (vnp=other_returns; vnp; vnp = vnp->next) {
		 switch (vnp->choice) {
		 case TXDBINFO:
		    TxDfDbInfoDestruct(vnp->data.ptrvalue);
		    break;
		 case TXKABLK_NOGAP:
		 case TXKABLK_GAP:
		 case TXPARAMETERS:
		    MemFree(vnp->data.ptrvalue);
		    break;
		 case TXMATRIX:
		    BLAST_MatrixDestruct(vnp->data.ptrvalue);
		    break;
		 default:
		    break;
		 }
	      }
	   }
	   other_returns = ValNodeFree(other_returns);
	   MemFree(seqalign_array);
           options->query_lcase_mask = 
              SeqLocSetFree(options->query_lcase_mask);

	   /* Freeing SeqEntries can be very expensive, do this only if 
	      this is not the last iteration of search */
	   if (!done) { 
	      for (index=0; index<num_bsps; index++) {
		 sepp[index] = SeqEntryFree(sepp[index]);
		 query_bsp_array[index] = NULL;
	      }	   
           }
           total_processed += num_bsps;
	} /* End of loop on complete searches */
        
        aip = AsnIoClose(aip);

        if (align_view == 7)
           xml_aip = AsnIoClose(xml_aip);

        if (align_view != 7 && html) 
           fprintf(outfp, "</PRE>\n</BODY>\n</HTML>\n");
        if (align_view != 7 && myargs[29].intvalue)
           fprintf(outfp, "Mega BLAST run finished, processed %d queries\n",
                   total_processed);
	MemFree(query_bsp_array);
	MemFree(sepp);
	options = BLASTOptionDelete(options);
	FileClose(infp);
	
	return 0;
}
