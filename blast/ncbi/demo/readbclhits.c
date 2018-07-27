/*  $Id: readbclhits.c,v 6.2 2002/10/08 15:31:55 dondosha Exp $
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
*   Program to read the binary hits file from blastclust and produce a text 
*   hit file in tabular format
*
* $Revision: 6.2 $
*/

#include <ncbi.h>

#ifdef NUMARG
#undef NUMARG
#endif
#define NUMARG (sizeof(myargs)/sizeof(myargs[0]))

typedef struct cluster_log_header
{
   Boolean numeric_id_type;
   Int4 size;
} ClusterLogHeader, PNTR ClusterLogHeaderPtr;

typedef struct cluster_log_info
{
   Int4 id1, id2;
   Int4 hsp_length1, hsp_length2;
   FloatHi bit_score, perc_identity;
} ClusterLogInfo, PNTR ClusterLogInfoPtr;

static Args myargs [] = {
   { "Binary input file with hits from blastclust run", /* 0 */
     "stdin", NULL, NULL, FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
   { "Text output file",                   /* 1 */
     "stdout", NULL, NULL, FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL} 
};

#define INFO_LIST_SIZE 1000

Int2 Main(void)
{
   FILE *hitsfp, *outfp;
   ClusterLogInfoPtr info;
   Int4 num_queries, num_hits, i, total_id_len;
   Int4Ptr gi_list = NULL, seq_len = NULL;
   CharPtr PNTR id_list = NULL;
   CharPtr ptr, id_string;
   ClusterLogHeader header;

   if (! GetArgs ("readbclhits", NUMARG, myargs))
      return (1);

   hitsfp = FileOpen(myargs[0].strvalue, "rb");
   outfp = FileOpen(myargs[1].strvalue, "w");

   FileRead(&header, sizeof(ClusterLogHeader), 1, hitsfp);

   if (header.numeric_id_type) {
      num_queries = header.size;
      gi_list = (Int4Ptr) MemNew(num_queries*sizeof(Int4));
      FileRead(gi_list, sizeof(Int4), num_queries, hitsfp);
   } else {
      total_id_len = header.size;
      num_queries = 0;
      id_string = (CharPtr) MemNew(total_id_len+1);
      FileRead(id_string, sizeof(Char), total_id_len, hitsfp);
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
   FileRead(seq_len, sizeof(Int4), num_queries, hitsfp);

   info = (ClusterLogInfoPtr) MemNew(INFO_LIST_SIZE*sizeof(ClusterLogInfo));

   fprintf(outfp, 
      "# Fields: id1, id2, length1, length2, bit score, percent identity\n");

   while ((num_hits = FileRead(info, sizeof(ClusterLogInfo), INFO_LIST_SIZE,
			       hitsfp)) > 0) {
      for (i=0; i<num_hits; ++i) {
         if (header.numeric_id_type) {
            fprintf(outfp, "%ld\t%ld\t%ld\t%ld\t%f\t%f\n", 
                    gi_list[info[i].id1], gi_list[info[i].id2], 
                    info[i].hsp_length1, info[i].hsp_length2, 
                    info[i].bit_score, info[i].perc_identity);
         } else {
            fprintf(outfp, "%s\t%s\t%ld\t%ld\t%f\t%f\n", id_list[info[i].id1],
                    id_list[info[i].id2], info[i].hsp_length1, 
                    info[i].hsp_length2, info[i].bit_score, 
                    info[i].perc_identity);
         }

      }
   } /* End loop on chunks of hits */
   
   FileClose(hitsfp);
   FileClose(outfp);
}
