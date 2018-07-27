/* $Id: blast_prelim.c,v 1.7 2005/02/09 20:55:38 dondosha Exp $
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's offical duties as a United States Government employee and
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
*  Author: Ilya Dondoshansky
*
*****************************************************************************/

/** @file blast_prelim.c
 * Preliminary stage of a BLAST search performed by one of the threads
 * in a multi-threaded search.
 */

static char const rcsid[] = "$Id: blast_prelim.c,v 1.7 2005/02/09 20:55:38 dondosha Exp $";

#include <algo/blast/api/blast_prelim.h>
#include <algo/blast/core/blast_util.h>
#include <algo/blast/core/blast_gapalign.h>
#include <algo/blast/core/blast_setup.h>
#include <algo/blast/core/blast_engine.h>

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

BlastPrelimSearchThreadData* 
BlastPrelimSearchThreadDataInit(EBlastProgramType program,
   BLAST_SequenceBlk* query, BlastQueryInfo* query_info,
   const BlastSeqSrc* seq_src, LookupTableWrap* lut, 
   const BlastScoringOptions* score_options, 
   const BlastInitialWordOptions* word_options,
   const BlastExtensionOptions* ext_options, 
   const BlastHitSavingOptions* hit_options,
   const BlastEffectiveLengthsOptions* eff_len_options,
   const PSIBlastOptions* psi_options, 
   const BlastDatabaseOptions* db_options,
   BlastScoreBlk* sbp, BlastDiagnostics* diagnostics,
   BlastHSPStream* hsp_stream)
{
   BlastPrelimSearchThreadData* data = (BlastPrelimSearchThreadData*)
      calloc(1, sizeof(BlastPrelimSearchThreadData));
   
   data->program = program;
   data->query = query;
   data->query_info = BlastQueryInfoDup(query_info);
   data->seq_src = BlastSeqSrcCopy(seq_src);
   data->lut = lut;
   data->score_options = score_options;
   data->word_options = word_options;
   data->ext_options = ext_options;
   data->hit_options = hit_options;
   data->eff_len_options = eff_len_options;
   data->psi_options = psi_options;
   data->db_options = db_options;
   data->sbp = sbp;
   data->diagnostics = diagnostics;
   data->hsp_stream = hsp_stream;
   
   return data;
}


BlastPrelimSearchThreadData* 
BlastPrelimSearchThreadDataFree(BlastPrelimSearchThreadData* data)
{
   if (!data)
      return NULL;

   BlastSeqSrcFree(data->seq_src);
   BlastQueryInfoFree(data->query_info);
   sfree(data);
   return NULL;
}

void* Blast_PrelimSearchThreadRun(void* data)
{
   void* ret_status = NULL;
   Int2 status = 0;
   BlastPrelimSearchThreadData* search_data = 
      (BlastPrelimSearchThreadData*) data;
   
   status = Blast_RunPreliminarySearch(search_data->program, search_data->query, 
                search_data->query_info, search_data->seq_src, 
                search_data->score_options, search_data->sbp, search_data->lut, 
                search_data->word_options, search_data->ext_options, 
                search_data->hit_options, search_data->eff_len_options, 
                search_data->psi_options, search_data->db_options, 
                search_data->hsp_stream, search_data->diagnostics);

   BlastPrelimSearchThreadDataFree(search_data);

   return ret_status;
}
/* @} */

