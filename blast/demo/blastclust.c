/*  $RCSfile: blastclust.c,v $  $Revision: 6.28 $  $Date: 2001/11/16 20:34:17 $
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
* Author: Ilya Dondoshansky 
*
* File Description:
*   This is server which does neighboring searches
*
* ---------------------------------------------------------------------------
* $Log: blastclust.c,v $
* Revision 6.28  2001/11/16 20:34:17  dondosha
* Bug fix: BioseqUnlock was missing; also added ObjMgrFreeCache calls
*
* Revision 6.27  2001/08/15 17:49:08  dondosha
* Correction in previous change
*
* Revision 6.26  2001/08/15 17:27:17  dondosha
* Consider and save only the top HSP for each pair of sequences being compared
*
* Revision 6.25  2001/07/26 18:23:48  dondosha
* Added a -c option: configuration file with advanced BLAST options
*
* Revision 6.24  2001/02/13 16:26:03  dondosha
* Bug fix: memory freed in the wrong place
*
* Revision 6.23  2001/01/31 19:48:14  dondosha
* Change #define TMPDIR to environment variable
*
* Revision 6.22  2001/01/26 20:13:05  dondosha
* If at least one sequence id non-numeric, make them all non-numeric
*
* Revision 6.21  2000/12/19 18:52:38  dondosha
* Added option to disallow id parsing when formatting database
*
* Revision 6.20  2000/12/14 20:51:14  dondosha
* Do several Mega BLAST searches if more than 16383 sequences in input
*
* Revision 6.19  2000/12/07 22:57:34  dondosha
* Implemented nucleotide version using Mega BLAST
*
* Revision 6.18  2000/12/01 17:57:04  dondosha
* Handle local and general ids differently to avoid non-informative output
*
* Revision 6.17  2000/09/01 18:30:55  dondosha
* Create database memory map only once for all searches
*
* Revision 6.16  2000/08/08 17:58:55  dondosha
* Change back to create database with indexes
*
* Revision 6.15  2000/08/07 19:40:57  dondosha
* Removed sequence lengths from ClusterLogInfo structure - redundant information
*
* Revision 6.14  2000/08/07 15:13:36  dondosha
* Changed comment for -S option
*
* Revision 6.13  2000/08/04 23:24:40  dondosha
* Added functionality to choose neighbors based on percentage of identities
*
* Revision 6.12  2000/06/08 20:40:15  dondosha
* In case of equal lengths of sequences within a cluster or equal number of cluster elements, use alphabetic or numeric order when sorting ids and clusters
*
* Revision 6.11  2000/06/08 13:34:57  dondosha
* Increased buffer size for reading id file
*
* Revision 6.10  2000/06/07 21:57:04  dondosha
* Added option to provide a file with id list for reclustering
*
* Revision 6.9  2000/06/07 14:23:20  dondosha
* Changed ctime_r call to portable DayTimeStr
*
* Revision 6.8  2000/06/06 19:32:41  dondosha
* Changed progress interval back to 1000
*
* Revision 6.7  2000/06/06 19:23:42  dondosha
* Added option to finish incomplete clustering from the point where previous search ended
*
* Revision 6.5  2000/05/22 18:19:59  dondosha
* In case search set up fails, destruct all necessary stuff before going to next query
*
* Revision 6.4  2000/05/17 17:44:50  dondosha
* Cleaned from unused variables
*
* Revision 6.3  2000/05/05 18:16:56  dondosha
* Enhanced to process all types of SeqIds
*
* Revision 6.2  2000/05/03 16:50:40  dondosha
* Removed system calls, added sorting of clusters by size, sequences within clusters by length
*
* Revision 6.1  2000/04/27 14:47:18  dondosha
* Clustering of protein neighbours - initial revision
*
* Revision 1.1  2000/04/12 12:53:04   dondosha
* Clustering of protein neighbors
*
* ===========================================================================
*/
#include <ncbi.h>
#include <objseq.h>
#include <objsset.h>
#include <sequtil.h>
#include <seqport.h>
#include <tofasta.h>
#include <blast.h>
#include <blastpri.h>
#include <simutil.h>
#include <txalign.h>
#include <gapxdrop.h>
#include <sqnutils.h>
#include <mbalign.h>
#include <mblast.h>

#define DEBUG 0

/* Used by the callback function. */
FILE *global_fp=NULL;
static Int4Ptr root;
static TNlmMutex root_mutex;

static int LIBCALLBACK
tick_callback(Int4 sequence_number, Int4 number_of_positive_hits)
{
   return 0;
}

typedef struct blastclust_hsp_info
{
   Int4 id;
   Int4 score;
   Int4 index;
} BlastClustHspInfo, PNTR BlastClustHspInfoPtr;

typedef struct cluster_parameters
{
   Boolean bidirectional;
   FloatHi length_threshold;
   FloatHi score_threshold;
   FILE *logfp;
} ClusterParameters, PNTR ClusterParametersPtr;

typedef struct cluster_log_header
{
   Boolean numeric_id_type;
   Int4 size;
} ClusterLogHeader, PNTR ClusterLogHeaderPtr;

static ClusterParametersPtr global_parameters;

typedef struct cluster_log_info
{
   Int4 id1, id2;
   Int4 hsp_length1, hsp_length2;
   FloatHi bit_score, perc_identity;
} ClusterLogInfo, PNTR ClusterLogInfoPtr;

typedef struct blast_cluster_element
{
   Int4 gi;
   CharPtr id;
   Int4 len;
} BlastClusterElement, PNTR BlastClusterElementPtr;

typedef struct blast_cluster
{
   Int4 size;
   BlastClusterElementPtr PNTR elements;
} BlastCluster, PNTR BlastClusterPtr;

static int LIBCALLBACK
compare_cluster_elements(VoidPtr v1, VoidPtr v2)
{
   BlastClusterElementPtr e1, e2;
   
   e1 = *(BlastClusterElementPtr PNTR) v1;
   e2 = *(BlastClusterElementPtr PNTR) v2;

   if (e1->len > e2->len) 
      return -1;
   else if (e1->len < e2->len) 
      return 1; 
   else if (e1->gi > 0 && e2->gi>0) {
      if (e1->gi < e2->gi)
	 return -1;
      else if (e1->gi > e2->gi)
	 return 1;
   } else 
      return StrCmp(e1->id, e2->id);

   return 0;
}

static int LIBCALLBACK
compare_clusters(VoidPtr v1, VoidPtr v2)
{
   BlastClusterPtr c1, c2;

   c1 = *(BlastClusterPtr PNTR) v1;
   c2 = *(BlastClusterPtr PNTR) v2;

   if (c1->size > c2->size) 
      return -1;
   else if (c1->size < c2->size) 
      return 1;
   else 
      return compare_cluster_elements(c1->elements, c2->elements);
}

static int LIBCALLBACK
hsp_info_id_score_compare(VoidPtr v1, VoidPtr v2)
{
   BlastClustHspInfoPtr c1, c2;

   c1 = *(BlastClustHspInfoPtr PNTR) v1;
   c2 = *(BlastClustHspInfoPtr PNTR) v2;

   if (c1->id == c2->id && c1->score == c2->score)
      return 0;
   else if ((c1->id < c2->id) || 
            (c1->id == c2->id && c1->score > c2->score)) 
      return -1;
   else 
      return 1;
}

#define ORIGINAL_CLUSTER_SIZE 10
static int BlastClusterNeighbours(Int4 num_queries, Int4Ptr seq_len, 
				  CharPtr PNTR id_list, Int4Ptr gi_list,
				  Boolean PNTR used_id_index)
{
   BlastClusterPtr PNTR cluster;
   Int4 num_clusters, available_size, index, i;
   Boolean numeric_id_type;

   if (gi_list)
      numeric_id_type = TRUE;
   else if (id_list)
      numeric_id_type = FALSE;
   else 
      return 0;

    for (index=1; index<num_queries; index++) {
       i = index;
       while (root[i] != i)
	  i = root[i];
       root[index] = i;
    }

    cluster = (BlastClusterPtr PNTR) 
       MemNew(num_queries*sizeof(BlastClusterPtr));
    num_clusters = 0;

    for (index=0; index<num_queries; index++) {
       if (used_id_index != NULL && !used_id_index[index])
	  continue;
       if (root[index]==index) {
	  cluster[num_clusters] = 
	     (BlastClusterPtr) MemNew(sizeof(BlastCluster)); 
	  cluster[num_clusters]->size = 1;
	  cluster[num_clusters]->elements = 
	     (BlastClusterElementPtr PNTR) 
	     Malloc(ORIGINAL_CLUSTER_SIZE*sizeof(BlastClusterElementPtr));
	  cluster[num_clusters]->elements[0] = 
	     (BlastClusterElementPtr) MemNew(sizeof(BlastClusterElement));

	  if (numeric_id_type)
	     cluster[num_clusters]->elements[0]->gi = gi_list[index];
	  else
	     cluster[num_clusters]->elements[0]->id = id_list[index];
	  cluster[num_clusters]->elements[0]->len = seq_len[index];
	  available_size = ORIGINAL_CLUSTER_SIZE;

	  for (i=index+1; i<num_queries; i++) {
	     if (root[i]==index) {
		if (cluster[num_clusters]->size >= available_size) {
		      available_size *= 2;
		      cluster[num_clusters]->elements = 
			 (BlastClusterElementPtr PNTR) 
			 Realloc(cluster[num_clusters]->elements, 
				 available_size*sizeof(BlastClusterElementPtr));
		} 
		cluster[num_clusters]->elements[cluster[num_clusters]->size] = 
		   (BlastClusterElementPtr) MemNew(sizeof(BlastClusterElement));
		if (numeric_id_type)
		   cluster[num_clusters]->elements[cluster[num_clusters]->size]->gi = gi_list[i];
		else
		   cluster[num_clusters]->elements[cluster[num_clusters]->size]->id = id_list[i];

		cluster[num_clusters]->elements[cluster[num_clusters]->size]->len = seq_len[i];
		cluster[num_clusters]->size++;
	     }
	  }
	  num_clusters++;
       }
    }

    /* Sort each cluster in decreasing order of sequence lengths */
    for (index=0; index<num_clusters; index++) 
       HeapSort(cluster[index]->elements, cluster[index]->size, 
		sizeof(BlastClusterElementPtr), compare_cluster_elements);
    /* Sort clusters in decreasing order of sizes */
    HeapSort(cluster, num_clusters, sizeof(BlastClusterPtr), compare_clusters);
    /* Print out all clusters */
    for (index=0; index<num_clusters; index++) {
       for (i=0; i<cluster[index]->size; i++) {
	  if (numeric_id_type)
	     fprintf(global_fp, "%ld ", cluster[index]->elements[i]->gi); 
	  else 
	     fprintf(global_fp, "%s ", cluster[index]->elements[i]->id);
	  MemFree(cluster[index]->elements[i]);
       }
       fprintf(global_fp, "\n");
       MemFree(cluster[index]->elements);
       MemFree(cluster[index]);
    }
    MemFree(cluster);
    return 1;
}

/* The following function reclusters saved hits and returns the ordinal id of
   the last query found in the hits file, plus 1 */
   
#define INFO_LIST_SIZE 1000
#define FILE_BUFFER_SIZE 4096
static Int4 ReclusterFromFile(FILE *infofp, FILE *outfp, Int4Ptr PNTR gilp,
			      CharPtr PNTR PNTR idlp, Int4Ptr PNTR seqlp,
			      FILE *idfp)
{
   ClusterLogInfoPtr info;
   Int4 num_queries, num_hits, i, root1, root2, total_id_len;
   Int4Ptr gi_list = NULL, seq_len = NULL;
   CharPtr PNTR id_list = NULL;
   CharPtr ptr, id_string;
   FloatHi length_coverage, score_coverage; 
   ClusterLogHeader header;
   Int4 last_seq = -1;
   CharPtr id = NULL;
   Boolean PNTR used_id_index = NULL;

   FileRead(&header, sizeof(ClusterLogHeader), 1, infofp);

   if (header.numeric_id_type) {
      num_queries = header.size;
      gi_list = (Int4Ptr) MemNew(num_queries*sizeof(Int4));
      FileRead(gi_list, sizeof(Int4), num_queries, infofp);
   } else {
      total_id_len = header.size;
      num_queries = 0;
      id_string = (CharPtr) MemNew(total_id_len+1);
      FileRead(id_string, sizeof(Char), total_id_len, infofp);
      ptr = id_string;
      /* Count the ID's and change delimiter from space to null character */
      for (i=0; i<total_id_len; i++) {
	 if (*ptr==' ') {
	    num_queries++;
	    *ptr = '\0';
	 }
	 ptr++;
      }
      id_list = (CharPtr PNTR) MemNew(num_queries*sizeof(CharPtr));
      for (i=0; i<num_queries; i++) {
	 /* No need for allocation of new memory */
	 id_list[i] = id_string;
	 id_string += StringLen(id_string) + 1;
      }
      id_string = id_list[0];
   }

   seq_len = (Int4Ptr) Malloc(num_queries*sizeof(Int4));
   FileRead(seq_len, sizeof(Int4), num_queries, infofp);

   info = (ClusterLogInfoPtr) MemNew(INFO_LIST_SIZE*sizeof(ClusterLogInfo));

   root = (Int4Ptr) Malloc(num_queries*sizeof(Int4));
   for (i=0; i<num_queries; i++)
      root[i] = i;

   /* Test for list of ids to use for reclustering */
   if (idfp) {
      CharPtr id_str, used_id_string;
      CharPtr delimiter_list = " \t\n,;";
      Int4 length;

      used_id_string = (CharPtr) MemNew(FILE_BUFFER_SIZE+1);
      used_id_index = (Boolean PNTR) MemNew(num_queries*sizeof(Boolean));
      while((length = 
	     FileRead(used_id_string, sizeof(Char), FILE_BUFFER_SIZE, idfp))) {
	 ptr = used_id_string;
	 if (gi_list)
	    id_str = (CharPtr) Malloc(10);
	 while ((id = StringTokMT(ptr, delimiter_list, &ptr)) 
		!= NULL) {
	    if (ptr==NULL && length==FILE_BUFFER_SIZE) {
	       fseek(idfp, -StrLen(id), SEEK_CUR);
	       break;
	    }
	    for (i=0; i<num_queries; i++) {
	       if (gi_list) 
		  sprintf(id_str, "%ld", gi_list[i]);
	       else
		  id_str = id_list[i];
	       if (!StrCmp(id_str, id))
		  break;
	    }
	    if (i < num_queries)
	       used_id_index[i] = TRUE;
	 }
      }
      if (gi_list)
	 MemFree(id_str);
   }

   while ((num_hits = FileRead(info, sizeof(ClusterLogInfo), INFO_LIST_SIZE,
			       infofp)) > 0) {
      for (i=0; i<num_hits; i++) {
	 if (used_id_index && 
	     (!used_id_index[info[i].id1] || !used_id_index[info[i].id2]))
	    continue;
	 if (global_parameters->bidirectional)
	    length_coverage = MIN(((FloatHi)info[i].hsp_length1) / 
				  seq_len[info[i].id1], 
				  ((FloatHi)info[i].hsp_length2) / 
				  seq_len[info[i].id2]);
	 else
	    length_coverage = MAX(((FloatHi)info[i].hsp_length1) / 
				  seq_len[info[i].id1], 
				  ((FloatHi)info[i].hsp_length2) / 
				  seq_len[info[i].id2]);

         if (global_parameters->score_threshold < 3.0)
            score_coverage = info[i].bit_score / 
               (MAX(info[i].hsp_length1, info[i].hsp_length2));
         else 
            score_coverage = info[i].perc_identity;

	 if (length_coverage >= global_parameters->length_threshold && 
	     score_coverage >= global_parameters->score_threshold) {
	    root1 = info[i].id1;
	    while (root[root1] != root1)
	       root1 = root[root1];
	    root2 = info[i].id2;
	    while (root[root2] != root2)
	       root2 = root[root2];
	    
	    if (root1 < root2)
	       root[root2] = root1;
	    else if (root1 > root2)
	       root[root1] = root2;
	 }
      } /* End loop on hits from a chunk */
      last_seq = info[num_hits-1].id1;
   } /* End loop on chunks of hits */
 
   if (outfp != NULL) {
      global_fp = outfp;
      BlastClusterNeighbours(num_queries, seq_len, id_list, gi_list, used_id_index);
   }
   *gilp = gi_list;
   *idlp = id_list;
   *seqlp = seq_len;
   MemFree(id_string);
   MemFree(info);
   return last_seq + 1;
}

static FloatHi 
GapAlignPercentIdentity(GapAlignBlkPtr gabp)
{
   GapXEditScriptPtr esp = gabp->edit_block->esp;
   Int4 identical, total, q_index, s_index, i;
   FloatHi perc_identity;
   Uint1Ptr query, subject;

   q_index = gabp->query_start;
   s_index = gabp->subject_start;
   query = gabp->query + q_index;
   subject = gabp->subject + s_index;
   identical = total = 0;

   while (esp) {
      if (esp->op_type == GAPALIGN_SUB || esp->op_type == GAPALIGN_DECLINE) {
         for (i=0; i<esp->num; i++) {
            if (*query == *subject)
               identical++;
            query++;
            subject++;
         }
      } else if (esp->op_type == GAPALIGN_DEL) 
         subject += esp->num;
      else if (esp->op_type == GAPALIGN_INS)
         query += esp->num;
      total += esp->num;
      esp = esp->next;
   }
   perc_identity = ((FloatHi)identical) / total * 100;

   return perc_identity;
}
   

/* The following function prints only those hits which correspond to 
   an almost identical match of two sequences */
#define BUFFER_SIZE 80
static int LIBCALLBACK
PrintNeighbors(VoidPtr ptr)
{
    BLAST_HitListPtr current_hitlist;
    BLAST_HSPPtr hsp; 
    Int4 index;
    BlastSearchBlkPtr search;
    Int4 id1, id2, root1, root2, hspcnt;
    Int4 high_score=0;
    Nlm_FloatHi current_evalue=DBL_MAX;
    Int4 subject_length;
    Uint1Ptr subject, query;
    ClusterLogInfoPtr loginfo = NULL;
    BlastClustHspInfoPtr PNTR hsp_info_array;
    Int4 query_count;

    if (ptr == NULL)
        return 0;	
    
    search = (BlastSearchBlkPtr) ptr;
    
    if (search->current_hitlist == NULL || search->current_hitlist->hspcnt <= 0) {
        /* No hits to save. */
        search->subject_info = BLASTSubjectInfoDestruct(search->subject_info);
        return 0;
    }
    
    current_hitlist = search->current_hitlist;
    
    if (search->prog_number == blast_type_blastp)
        subject_length = readdb_get_sequence(search->rdfp, search->subject_id, &subject);
    else { /* Mega BLAST saves ncbi4na-encoded sequence */
        subject = search->subject->sequence_start;
        subject_length = search->subject->length;
    }
    hspcnt = current_hitlist->hspcnt;
    
    if (search->prog_number == blast_type_blastp)
       id1 = SeqId2OrdinalId(search->rdfp, search->query_id);
    else 
       id1 = -1;
    id2 = search->subject_id;
    
    if (id1 < id2) { /* Must be always true */
#define BUF_CHUNK_SIZE 1024
        Int4 query_length, q_length, s_length;
        FloatHi length_coverage, bit_score, score_coverage, perc_identity; 
	BLAST_KarlinBlkPtr kbp;
        GapAlignBlkPtr gap_align = search->gap_align;
        Int2 context;
        Int4 max_score = 0;

        if (search->prog_number == blast_type_blastp) {
           query = search->context[0].query->sequence;
           query_length = search->context[0].query->length;
        }

        query_count = 1;

        if (search->prog_number == blast_type_blastn) {
           hsp_info_array = (BlastClustHspInfoPtr PNTR) 
              Malloc(hspcnt*sizeof(BlastClustHspInfoPtr));
           for (index=0; index<hspcnt; index++) {
              hsp_info_array[index] = (BlastClustHspInfoPtr)
                 Malloc(sizeof(BlastClustHspInfo));
              hsp = current_hitlist->hsp_array[index];
              hsp_info_array[index]->id = 
                 SeqId2OrdinalId(search->rdfp,
                                 search->qid_array[hsp->context/2]);
              hsp_info_array[index]->score = hsp->score;
              hsp_info_array[index]->index = index;
           }
           HeapSort(hsp_info_array, hspcnt, sizeof(BlastClustHspInfoPtr), 
                    hsp_info_id_score_compare);
           for (index=1; index<hspcnt; index++) {
              if (hsp_info_array[index]->id < id2 && 
                  hsp_info_array[index]->id != 
                  hsp_info_array[query_count-1]->id) {
                 hsp_info_array[query_count] = hsp_info_array[index];
                 query_count++;
              } else 
                 hsp_info_array[index] = MemFree(hsp_info_array[index]);
           }
           
           hsp = current_hitlist->hsp_array[hsp_info_array[0]->index];
        } else {
           for (index=0; index<hspcnt; index++) {
              if (current_hitlist->hsp_array[index]->score > max_score) {
                 hsp = current_hitlist->hsp_array[index];
                 max_score = hsp->score;
              }
           }
        }

	if (global_parameters->logfp)
	   loginfo = (ClusterLogInfoPtr)
              MemNew(query_count*sizeof(ClusterLogInfo));
        
        index = 0;
        while (hsp) {
           /* Test if this hsp corresponds to an almost identical hit */
           if (search->prog_number == blast_type_blastn) {
              context = hsp->context;
              id1 = hsp_info_array[index]->id;
              query_length = 
                 search->query_context_offsets[context+1] -
                 search->query_context_offsets[context] - 1;
           }
           q_length = hsp->query.length;
           s_length = hsp->subject.length;
           
           if (global_parameters->bidirectional)
              length_coverage = MIN(((FloatHi)q_length) / query_length, 
                                    ((FloatHi)s_length) / subject_length);
           else
              length_coverage = MAX(((FloatHi)q_length) / query_length, 
                                    ((FloatHi)s_length) / subject_length);
           
           if (search->prog_number == blast_type_blastp)
              kbp = search->sbp->kbp_gap[search->first_context];
           else
              kbp = search->sbp->kbp[context];
           bit_score = ((hsp->score*kbp->Lambda) -
                        kbp->logK)/NCBIMATH_LN2;
           
           if (search->prog_number == blast_type_blastp) {
              gap_align->query_frame = ContextToFrame(search,
                                                      hsp->context);
              gap_align->subject_frame = hsp->subject.frame;
              gap_align->q_start = hsp->query.gapped_start;
              gap_align->s_start = hsp->subject.gapped_start;
              PerformGappedAlignmentWithTraceback(gap_align);
              perc_identity = GapAlignPercentIdentity(gap_align);
              gap_align->state_struct = 
                 GapXDropStateDestroy(gap_align->state_struct);
              gap_align->edit_block = 
                 GapXEditBlockDelete(gap_align->edit_block);
           } else {
              Int4Ptr length, start;
              Uint1Ptr strands;
              Int4 align_length = 0, numseg, i;
              GapXEditScriptPtr esp;
              query = search->context[context].query->sequence;
              esp = hsp->gap_info->esp;
              for (numseg=0; esp; esp = esp->next, numseg++);
              GXECollectDataForSeqalign(hsp->gap_info, 
                                        hsp->gap_info->esp, numseg,
                                        &start, &length, &strands, 
                                        &hsp->query.offset, 
                                        &hsp->subject.offset);
              GapXEditBlockDelete(hsp->gap_info);
              perc_identity = 0;
              for (i=0; i<numseg; i++) {
                 align_length += length[i];
                 if (start[2*i] != -1 && start[2*i+1] != -1)
                    perc_identity += MegaBlastGetNumIdentical(query, subject, start[2*i], start[2*i+1], length[i], FALSE);
              }
              perc_identity = perc_identity / align_length * 100;
           }
           if (global_parameters->score_threshold < 3.0)
              score_coverage = bit_score / (MAX(q_length, s_length));
           else
              score_coverage = perc_identity;
           
           if (global_parameters->logfp) {
              loginfo[index].id1 = id1;
              loginfo[index].id2 = id2;
              loginfo[index].hsp_length1 = q_length;
              loginfo[index].hsp_length2 = s_length;
              loginfo[index].bit_score = bit_score;
              loginfo[index].perc_identity = perc_identity;
           }
           
           if (length_coverage >= global_parameters->length_threshold && 
               score_coverage >= global_parameters->score_threshold) {
              root1 = id1;
              
              NlmMutexLockEx(&root_mutex);
              while (root[root1] != root1)
                 root1 = root[root1];
              root2 = id2;
              while (root[root2] != root2)
                 root2 = root[root2];
              
              if (root1 < root2)
                 root[root2] = root1;
              else if (root1 > root2)
                 root[root1] = root2;
              NlmMutexUnlock(root_mutex);
           }
           if (search->prog_number == blast_type_blastn && 
               ++index < query_count)
              hsp =
                 current_hitlist->hsp_array[hsp_info_array[index]->index];
           else
              hsp = NULL;
        }

        if (search->prog_number == blast_type_blastn) {
           for (index=0; index<query_count; index++)
              MemFree(hsp_info_array[index]);
           hsp_info_array = MemFree(hsp_info_array);
        }

	if (global_parameters->logfp) {
	   FileWrite(loginfo, sizeof(ClusterLogInfo), query_count, 
		     global_parameters->logfp);
           MemFree(loginfo);
	   /*fflush(global_parameters->logfp);*/
	}
    } else
       fprintf(stderr, "Error: this can't happen!\n");
    return 1;
}


#define VERSION_NO	5

#ifdef NUMARG
#undef NUMARG
#endif
#define NUMARG (sizeof(myargs)/sizeof(myargs[0]))

static Args myargs [] = {
   { "FASTA input file (program will format the database and remove files in the end)",                                                         /* 0 */
     "stdin", NULL, NULL, FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
   { "Number of CPU's to use",                                   /* 1 */
      "1", NULL, NULL, FALSE, 'a', ARG_INT, 0.0, 0, NULL},       
   { "Output file for list of clusters",                         /* 2 */
      "stdout", NULL, NULL, FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
   { "Length coverage threshold",                                /* 3 */
      "0.9", NULL, NULL, FALSE, 'L', ARG_FLOAT, 0.0, 0, NULL},   
   { "Score coverage threshold (bit score / length if < 3.0, percentage of identities otherwise)",                                                              /* 4 */
      "1.75", NULL, NULL, FALSE, 'S', ARG_FLOAT, 0.0, 0, NULL},  
   { "Require coverage on both neighbours?",                     /* 5 */
      "TRUE", NULL, NULL, FALSE, 'b', ARG_BOOLEAN, 0.0, 0, NULL},
   { "File to save all neighbours",                              /* 6 */
      NULL, NULL, NULL, TRUE, 's', ARG_FILE_OUT, 0.0, 0, NULL},
   { "File to restore neighbors for reclustering",               /* 7 */
      NULL, NULL, NULL, TRUE, 'r', ARG_FILE_IN, 0.0, 0, NULL}, 
   { "Input as a database",                                      /* 8 */
      NULL, NULL, NULL, TRUE, 'd', ARG_FILE_IN, 0.0, 0, NULL}, 
   { "Print progress messages (verbose mode)",                   /* 9 */
      "stdout", NULL, NULL, FALSE, 'v', ARG_FILE_OUT, 0.0, 0, NULL}, 
   { "Complete unfinished clustering",                           /* 10 */
      "FALSE", NULL, NULL, FALSE, 'C', ARG_BOOLEAN, 0.0, 0, NULL},   
   { "Restrict reclustering to id list",                         /* 11 */
      NULL, NULL, NULL, TRUE, 'l', ARG_FILE_IN, 0.0, 0, NULL},   
   { "Is input proteins?",                                       /* 12 */
      "TRUE", NULL, NULL, FALSE, 'p', ARG_BOOLEAN, 0.0, 0, NULL},
   { "Enable id parsing in database formatting?",                /* 13 */
      "TRUE", NULL, NULL, FALSE, 'e', ARG_BOOLEAN, 0.0, 0, NULL},
   { "Configuration file with advanced options",                 /* 14 */
      NULL, NULL, NULL, TRUE, 'c', ARG_FILE_IN, 0.0, 0, NULL} 
};

#define MAX_DB_SIZE 100000
#define MAX_GI_LENGTH 9
#define PROGRESS_INTERVAL 1000
#define MAX_NUM_QUERIES 16383 /* == 1/2 INT2_MAX */

Int2 Main (void)
{
    BLAST_OptionsBlkPtr options;
    BlastSearchBlkPtr search;
    Boolean db_is_na, query_is_na;
    Int4 qsize, dbsize, first_seq;
    ReadDBFILEPtr rdfp, rdfp_var;
    Uint1 align_type;
    SeqIdPtr sip;
    BioseqPtr query_bsp = NULL, PNTR query_bsp_array;
    CharPtr blast_program, blast_inputfile, blast_outputfile, blast_database,
       progress_file = NULL;
    CharPtr logfile, info_file, input_name;
    Int4 total_id_len = 0;
    FILE *outfp, *infofp, *progressfp, *idfp;
    Int4 index, i, num_queries, num_bsps, num_left;
    Int8 total_length;
    Int4Ptr gi_list = NULL, seq_len = NULL;
    CharPtr PNTR id_list = NULL, id_string = NULL;
    Boolean db_formatted = FALSE, numeric_id_type = TRUE;
    Char db_file[BUFFER_SIZE];
    ClusterLogHeader header;
    Boolean print_progress, finish_incomplete, is_prot, parse_mode;
    FDB_optionsPtr fdb_options;
    Char timestr[24];
    CharPtr tmpdir;

    if (! GetArgs ("blastclust", NUMARG, myargs))
       return (1);
    
    UseLocalAsnloadDataAndErrMsg ();
    
    if (! SeqEntryLoad())
       return 1;
    
    ErrSetMessageLevel(SEV_WARNING);

    global_parameters = (ClusterParametersPtr) MemNew(sizeof(ClusterParameters));

    global_parameters->length_threshold = myargs[3].floatvalue;
    global_parameters->score_threshold = myargs[4].floatvalue;
    global_parameters->bidirectional = (Boolean) myargs[5].intvalue;
    finish_incomplete = (Boolean) myargs[10].intvalue;
    
    print_progress = (Boolean) StrCmp(myargs[9].strvalue, "F");
    if (print_progress)
       progress_file = myargs[9].strvalue;

    if (progress_file != NULL &&
	(progressfp = FileOpen(progress_file, "w")) == NULL) {
       ErrPostEx(SEV_FATAL, 0, 0, "blastclust: Unable to open progress file %s\n",
		 progress_file);
       return (1);
    }
    
    blast_outputfile = myargs[2].strvalue;
    outfp = NULL;
    if (blast_outputfile != NULL) {
        if ((outfp = FileOpen(blast_outputfile, "w")) == NULL) {
            ErrPostEx(SEV_FATAL, 0, 0, "blastclust: Unable to open output file %s\n", blast_outputfile);
            return (1);
        }
    }

    info_file = myargs[7].strvalue;
    if (info_file) {
       /* Non-empty string means only retrieve neighbors for reclustering */
       if ((infofp = FileOpen(info_file, "rb")) == NULL) { 
	  ErrPostEx(SEV_FATAL, 0, 0, "blastclust: Unable to open neighbors file %s for reading\n", info_file);
	  return (1);
       }
       if (myargs[11].strvalue) {
	  if ((idfp = FileOpen(myargs[11].strvalue, "r")) == NULL) {
	     ErrPostEx(SEV_FATAL, 0, 0, "blastclust: Unable to open id file %s for reading\n", myargs[11].strvalue);
	     return (1);
	  }
       } else
	  idfp = NULL;
       /* No need for another search, simply get all the neighbours
	  and reculster them using new thresholds */
       ReclusterFromFile(infofp, outfp, &gi_list, &id_list, &seq_len, idfp);
       MemFree(gi_list);
       MemFree(id_list);
       MemFree(seq_len);
       FileClose(infofp);
       FileClose(outfp);
       return 0;
    } else
       infofp = NULL;

    is_prot = (Boolean) myargs[12].intvalue;
    parse_mode = (Boolean) myargs[13].intvalue;
    if (is_prot)
       blast_program = StringSave("blastp");
    else 
       blast_program = StringSave("blastn");

    if (myargs[8].strvalue)
       db_formatted = TRUE;

    if (db_formatted) {
       blast_database = myargs[8].strvalue;
    } else {
       /* Need to format the database */
       blast_inputfile = myargs[0].strvalue;
       input_name = FileNameFind(blast_inputfile);

       if (tmpdir = getenv("TMPDIR")) {
          blast_database = 
             Malloc(StringLen(input_name) + StringLen(tmpdir) + 2);
          sprintf(blast_database, "%s/%s", tmpdir, input_name);
       } else
          blast_database = blast_inputfile;

       fdb_options = MemNew(sizeof(FDB_options));
       fdb_options->db_file = StringSave(blast_inputfile);
       fdb_options->is_protein = (Int4) is_prot; 
       fdb_options->parse_mode = (Int4) parse_mode; 
       fdb_options->base_name = StringSave(blast_database);
       FastaToBlastDB(fdb_options, blast_database, 0);

       MemFree(fdb_options->db_file);
       MemFree(fdb_options->base_name);
       MemFree(fdb_options);
    }

    logfile = myargs[6].strvalue;
    if (logfile) { /* Empty string means do not write log information */
       if (finish_incomplete) {
	  if ((global_parameters->logfp = FileOpen(logfile, "ab+")) == NULL) { 
	     ErrPostEx(SEV_FATAL, 0, 0, "blast: Unable to open log file %s for appending\n", logfile);
	     return (1);
	  }
       } else {
	  if ((global_parameters->logfp = FileOpen(logfile, "wb+")) == NULL) { 
	     ErrPostEx(SEV_FATAL, 0, 0, "blast: Unable to open log file %s for writing\n", logfile);
	     return (1);
	  }
       }
    }
    global_fp = outfp;

    align_type = BlastGetTypes(blast_program, &query_is_na, &db_is_na);
    
    rdfp = readdb_new(blast_database, is_prot);
    
    options = BLASTOptionNew(blast_program, TRUE);
    
    if (options == NULL)
	return 3;
    
    options->use_real_db_size = TRUE;
    options->sort_gi_list = FALSE;
    
    options->expect_value  = 1e-6;	
    qsize = 300;
    dbsize = (20*1000*1000);
    options->searchsp_eff = (FloatHi) qsize * (FloatHi) dbsize;
    options->perform_culling = FALSE;
    options->do_not_reevaluate = TRUE;
    options->do_sum_stats = FALSE;
    options->number_of_cpus = myargs[1].intvalue;
    if (!is_prot) {
       options->is_megablast_search = TRUE;
       options->wordsize = 32;
       options->gap_open = options->gap_extend = 0;
       options->block_width = 0;
       options->window_size = 0;
    }    

    if (myargs[14].strvalue) {
       CharPtr string_options = NULL;
       Int2 string_options_len = 0;
       CharPtr error = NULL;
       FILE *config_file;
       Char opt_line[BUFFER_SIZE+1];
       
       if ((config_file = FileOpen(myargs[14].strvalue, "r")) == NULL) {
          ErrPostEx(SEV_FATAL, 0, 0, 
                    "Cannot open advanced options configuration file %s\n",
                    myargs[14].strvalue);
       }

       while (fgets(opt_line, BUFFER_SIZE, config_file) != 0) {
          if (*opt_line == '#')
             continue;
          if (!string_options) {
             string_options = (CharPtr) MemNew(BUFFER_SIZE+1);
             string_options_len = BUFFER_SIZE;
          } else {
             if (StrLen(string_options) + StrLen(opt_line) + 1 > 
                 string_options_len) {
                CharPtr tmp = (CharPtr) Realloc(string_options, 
                                                2*string_options_len);
                if (tmp) {
                   string_options = tmp;
                   string_options_len *= 2;
                }
             }
             StringCat(string_options, " ");
          }
          StringCat(string_options, opt_line);
       }
       if (string_options)
          parse_blast_options(options, string_options, &error, 
                              NULL, NULL, NULL);
       if (error) {
          ErrPostEx(SEV_WARNING, 0, 0, "blastclust: parse_blast_options: %s\n",
                    error);
       }
    }

    readdb_get_totals_ex(rdfp, &total_length, &num_queries, TRUE);

    root = (Int4Ptr) Malloc(num_queries*sizeof(Int4));

    ReadDBBioseqFetchEnable ("blastclust", blast_database, db_is_na, TRUE);
       
    if (is_prot && finish_incomplete) {
       first_seq = ReclusterFromFile(global_parameters->logfp, NULL, &gi_list,
				    &id_list, &seq_len, NULL);
    } else {
       first_seq = 0;
       for (index=0; index<num_queries; index++)
	  root[index] = index;
       
       gi_list = (Int4Ptr) MemNew(num_queries*sizeof(Int4));
       id_list = (CharPtr PNTR) MemNew(num_queries*sizeof(CharPtr));
       seq_len = (Int4Ptr) MemNew(num_queries*sizeof(Int4));
       
       for (index=0; index<num_queries; index++) {
	  readdb_get_descriptor(rdfp, index, &sip, NULL);
	  seq_len[index] = readdb_get_sequence_length(rdfp, index);
	  
          if (sip->choice == SEQID_LOCAL) {
             BioseqPtr bsp = BioseqLockById(sip);
             CharPtr title = StringSave(BioseqGetTitle(bsp));
             if (title) {
                numeric_id_type = FALSE;
                id_list[index] = StringTokMT(title, " ", &title);
             } else {
                numeric_id_type &=
                   GetAccessionFromSeqId(bsp->id, &gi_list[index], 
                                         &id_list[index]);
             }
             BioseqUnlock(bsp);
          } else if (sip->choice != SEQID_GENERAL)
             numeric_id_type &= 
                GetAccessionFromSeqId(sip, &gi_list[index], 
                                      &id_list[index]); 
          else {
             BioseqPtr bsp = BioseqLockById(sip);
             CharPtr title = StringSave(BioseqGetTitle(bsp));
             if (title) {
                numeric_id_type = FALSE;
                id_list[index] = StringTokMT(title, " ", &title);
             } else {
                numeric_id_type = FALSE;
                id_list[index] = (CharPtr) Malloc(BUFFER_SIZE+1);
                SeqIdWrite(sip, id_list[index],
                           PRINTID_FASTA_SHORT, BUFFER_SIZE);
             }
             BioseqUnlock(bsp);
          }
	  sip = SeqIdSetFree(sip);
       }
       header.numeric_id_type = numeric_id_type;
       
       if (numeric_id_type) {
	  id_list = MemFree(id_list);
	  header.size = num_queries;
       } else {
	  total_id_len = 0;
	  /* Check if some ids were gis and convert them to strings */
	  for (i=0; i<num_queries; i++) {
	     if (gi_list[i] > 0) {
		id_list[i] = (CharPtr) MemNew(10);
		sprintf(id_list[i], "%ld", gi_list[i]);
	     }
	     total_id_len += StringLen(id_list[i]) + 1;
	  }
	  gi_list = MemFree(gi_list);
	  id_string = (CharPtr) MemNew(total_id_len+1);
	  for (i=0; i<num_queries; i++) {
	     StringCat(id_string, id_list[i]);
	     StringCat(id_string, " ");
	  }
	  header.size = total_id_len;
       }
       
       FileWrite(&header, sizeof(ClusterLogHeader), 1, 
		 global_parameters->logfp);
       
       if (numeric_id_type) 
	  FileWrite(gi_list, sizeof(Int4), num_queries, 
		    global_parameters->logfp);
       else {
	  FileWrite(id_string, sizeof(Char), total_id_len, 
		    global_parameters->logfp);
	  MemFree(id_string);
       }
       
       FileWrite(seq_len, sizeof(Int4), num_queries, global_parameters->logfp);
       /*fflush(global_parameters->logfp);*/
    }

    if (print_progress) {
       DayTimeStr(timestr, TRUE, TRUE);
       if (finish_incomplete)
	  fprintf(progressfp, 
		  "%s Finish clustering of %ld queries, starting from query %ld\n", 
		  timestr, num_queries, first_seq);
       else
	  fprintf(progressfp, "%s Start clustering of %ld queries\n", 
		  timestr, num_queries);
    }

    if (!is_prot) {
       SeqAlignPtr PNTR head; /* Will not be used */

       query_bsp_array = (BioseqPtr PNTR) 
          Malloc((MIN(num_queries, MAX_NUM_QUERIES)+1)*sizeof(BioseqPtr));
       num_left = num_queries;
       while (num_left>0) {
          num_bsps = MIN(num_left, MAX_NUM_QUERIES);
          for (index=0; index<num_bsps; index++)
             query_bsp_array[index] = readdb_get_bioseq(rdfp, index);
          query_bsp_array[num_bsps] = NULL;
          
          head = BioseqMegaBlastEngine(query_bsp_array, blast_program, 
                                       blast_database, options, NULL, NULL,
                                       NULL, NULL, NULL, 0, PrintNeighbors);
          for (index=0; index<num_bsps; index++)
             query_bsp_array[index] = BioseqFree(query_bsp_array[index]);
          num_left -= num_bsps;
          ObjMgrFreeCache(0);
       }
       MemFree(query_bsp_array);
    } else { 
       for (index=first_seq; index<num_queries; index++) {
          query_bsp = readdb_get_bioseq(rdfp, index);
          /* Set up the search */
          options->first_db_seq = index + 1;
          
          search = BLASTSetUpSearchWithReadDbInternal(NULL, query_bsp, blast_program, seq_len[index], blast_database, options, NULL, NULL, NULL, 0, rdfp);
          if (search != NULL && !search->query_invalid) {
             search->handle_results = PrintNeighbors;
             
             /* Run BLAST. */
             search->queue_callback = NULL;
             search->thr_info->tick_callback = NULL;
             
             do_the_blast_run(search);
          }
          search = BlastSearchBlkDestruct(search);
          query_bsp = BioseqFree(query_bsp);
          ObjMgrFreeCache(0);
          
          if (print_progress && (index + 1)%PROGRESS_INTERVAL == 0) {
             DayTimeStr(timestr, TRUE, TRUE);
             fprintf(progressfp, "%s Finished processing of %ld queries\n", 
                     timestr, index+1);
          }
       } /* End of loop on queries */
    }
    rdfp = readdb_destruct(rdfp);
    BlastClusterNeighbours(num_queries, seq_len, id_list, gi_list, NULL);

    MemFree(seq_len);
    if (numeric_id_type)
       MemFree(gi_list);
    else {
       for (i=0; i<num_queries; i++)
	  MemFree(id_list[i]);
       MemFree(id_list);
    }
    MemFree(root);
    
    fflush(global_fp);
    options = BLASTOptionDelete(options);
    blast_program = (CharPtr) MemFree(blast_program);
    FileClose(outfp);
    if (global_parameters->logfp)
       FileClose(global_parameters->logfp);

    if (!db_formatted && StringLen(blast_database) > 0) {
       Char p_or_n = (is_prot) ? 'p' : 'n';
       sprintf(db_file, "%s.%chr", blast_database, p_or_n);
       FileRemove(db_file);
       sprintf(db_file, "%s.%cin", blast_database, p_or_n);
       FileRemove(db_file);
       sprintf(db_file, "%s.%csq", blast_database, p_or_n);
       FileRemove(db_file);
       if (parse_mode) {
          sprintf(db_file, "%s.%cnd", blast_database, p_or_n);
          FileRemove(db_file);
          sprintf(db_file, "%s.%cni", blast_database, p_or_n);
          FileRemove(db_file);
          sprintf(db_file, "%s.%csd", blast_database, p_or_n);
          FileRemove(db_file);
          sprintf(db_file, "%s.%csi", blast_database, p_or_n);
          FileRemove(db_file);
       }
    }
    MemFree(blast_database);
    return 0;
}

