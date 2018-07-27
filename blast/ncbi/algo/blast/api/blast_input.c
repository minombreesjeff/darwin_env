static char const rcsid[] = "$Id: blast_input.c,v 1.10 2003/12/03 17:30:25 dondosha Exp $";
/* ===========================================================================
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
* ===========================================================================*/

/*****************************************************************************

File name: blast_input.c

Author: Ilya Dondoshansky

Contents: Reading FASTA sequences for BLAST

$Revision: 1.10 $

******************************************************************************/
#include <objloc.h>
#include <tofasta.h>
#include <algo/blast/api/blast_input.h>
#include <algo/blast/api/blast_seq.h>

#define MAX_NUM_QUERIES 16383 /* == 1/2 INT2_MAX */
#define MAX_TOTAL_LENGTH 20000000

Boolean
BLAST_GetQuerySeqLoc(FILE *infp, Boolean query_is_na, Uint1 strand,
   Int4 start, Int4 end, BlastMaskLoc** lcase_mask, SeqLocPtr* query_slp, 
   Int4 ctr_start, Int4* num_queries)
{
   Int8 total_length;
   BioseqPtr query_bsp;
   SeqLocPtr mask_slp, last_slp;
   BlastMaskLoc* last_mask;
   char prefix[2];     /* for FastaToSeqEntryForDb */
   Int2 ctr = ctr_start + 1; /* Counter for FastaToSeqEntryForDb */
   Int4 query_index = 0;
   BlastMaskLoc* new_mask;
   SeqEntryPtr sep;
   Boolean done = TRUE;
   Int4 from, to;

   if (!query_slp)
      return -1;

   total_length = 0;
   *query_slp = NULL;
   last_slp = NULL;

   if (query_is_na && strand == Seq_strand_unknown)
      strand = Seq_strand_both;

   SeqMgrHoldIndexing(TRUE);
   mask_slp = NULL;
   last_mask = NULL;
   
   StrCpy(prefix, "");
   
   while ((sep=FastaToSeqEntryForDb(infp, query_is_na, NULL, FALSE, prefix, 
                                    &ctr, (lcase_mask ? &mask_slp : NULL))) != NULL)
   {
      if (mask_slp) {
         new_mask = BlastMaskLocFromSeqLoc(mask_slp, query_index);
         
         if (!last_mask)
            *lcase_mask = last_mask = new_mask;
         else {
            last_mask->next = new_mask;
            last_mask = last_mask->next;
         }
         /* Masking locations in a SeqLoc form are no longer needed */
         mask_slp = SeqLocSetFree(mask_slp);
      }
      ++query_index;
      
      query_bsp = NULL;
      if (query_is_na) {
         SeqEntryExplore(sep, &query_bsp, FindNuc);
      } else {
         SeqEntryExplore(sep, &query_bsp, FindProt);
      }
      
      if (query_bsp == NULL) {
         ErrPostEx(SEV_FATAL, 0, 0, "Unable to obtain bioseq\n");
         return 2;
      }
      
      /* Original from and to are 1-offsets, except when they are 0's,
         in which case they are start and end of sequence respectively */
      from = ((start > 0) ? start - 1 : 0);
      to = ((end > 0) ? end - 1 : query_bsp->length - 1);

      from = MIN(from, query_bsp->length - 1);
      to = MIN(to, query_bsp->length - 1);

      if ((strand == Seq_strand_plus) || (strand == Seq_strand_minus) ||
          (from > 0) || (to < query_bsp->length - 1))
      {
         SeqLocPtr new_slp = SeqLocIntNew(from, to, strand, 
                                SeqIdFindBest(query_bsp->id, SEQID_GI)); 
         if (last_slp) {
            last_slp->next = new_slp;
            last_slp = last_slp->next;
         } else {
            *query_slp = last_slp = new_slp;
         }
      } else {
         last_slp = ValNodeAddPointer(&last_slp, SEQLOC_WHOLE, 
                       SeqIdDup(SeqIdFindBest(query_bsp->id, SEQID_GI)));
         if (*query_slp == NULL)
            *query_slp = last_slp;
      }

      total_length += query_bsp->length;
      if (total_length > MAX_TOTAL_LENGTH || 
          query_index >= MAX_NUM_QUERIES) {
         done = FALSE;
         if (total_length > MAX_TOTAL_LENGTH) {
            ErrPostEx(SEV_INFO, 0, 0, 
               "Total length of queries has exceeded %ld\n",
               MAX_TOTAL_LENGTH);
         } else {
            ErrPostEx(SEV_INFO, 0, 0, 
               "Number of query sequences has exceeded the limit of %ld\n",
               MAX_NUM_QUERIES);
         }
         break;
      }
   }
   
   SeqMgrHoldIndexing(FALSE);

   if (num_queries)
      *num_queries = query_index;

   return done;
}

/** Probably can be removed */
Int4 MakeBlastSequenceBlkFromFasta(FILE *fasta_fp, BLAST_SequenceBlk* seq)
{
   BioseqPtr query_bsp;
   SeqEntryPtr query_sep;
   Boolean is_na = FALSE;
   Boolean believe_defline = TRUE;
   SeqLocPtr mask_slp = NULL;
   Int2 ctr=1;
   
   Uint1* sequence = NULL;
   
   query_sep=FastaToSeqEntryForDb(fasta_fp,
                               is_na, /* query is nucleotide? */
                               NULL, /* error message */
                               believe_defline, /* believe query defline? */
                               "", /* prefix for localid if not parseable */
                               &ctr, /* starting point for constructing a unique id */
                               &mask_slp);

   if (query_sep == NULL)
      return 1;
   
   SeqEntryExplore(query_sep, &query_bsp, FindProt);
   
   if (query_bsp == NULL)
      return 1;
   
   /* allocate contiguous space for the sequence */
   sequence = (Uint1 *) malloc(query_bsp->length);
   
   if (sequence == NULL)
      return 1;
   
   /* convert to ncbistdaa encoding */
   BioseqRawConvert(query_bsp, Seq_code_ncbistdaa);
   
   /* read the sequence */
   BSSeek(query_bsp->seq_data, 0, SEEK_SET);
   BSRead(query_bsp->seq_data, sequence, query_bsp->length);
   
   seq->length = query_bsp->length;
   seq->sequence = sequence;
   return 0;
}

