static char const rcsid[] = "$Id: repeats_filter.c,v 1.7 2005/04/21 15:00:36 dondosha Exp $";

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
 * File Name:  $RCSfile: repeats_filter.c,v $
 *
 * Author: Ilya Dondoshansky
 *
 */

/** @file repeats_filter.c
 * Repeats filtering API for the new BLAST code
 */

/* Prototypes of functions defined below */
#include <algo/blast/api/repeats_filter.h>
#include <algo/blast/api/blast_api.h>
#include <algo/blast/core/blast_filter.h>
#include <algo/blast/api/blast_seq.h>
#include <algo/blast/api/seqsrc_readdb.h>

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

/** Sets options for a repeats search.
 * @param options Options wrapper structure to modify [in] [out]
 */
static Int2 
s_SetRepeatsSearchOptions(SBlastOptions* options)
{
    Int2 status = 0;

    if (!options || !options->score_options || !options->word_options ||
        !options->ext_options)
        return -1;

    if ((status = SBlastOptionsSetEvalue(options, REPEATS_SEARCH_EVALUE)))
        return status;
    if ((status = SBlastOptionsSetWordSize(options, REPEATS_SEARCH_WORD_SIZE)))
        return status;
    if ((status = 
         SBlastOptionsSetFilterString(options, REPEATS_SEARCH_FILTER_STRING)))
        return status;

    options->lookup_options->lut_type = NA_LOOKUP_TABLE;
    options->score_options->penalty = REPEATS_SEARCH_PENALTY;
    options->score_options->gap_open = REPEATS_SEARCH_GAP_OPEN;
    options->score_options->gap_extend = REPEATS_SEARCH_GAP_EXTEND;
    options->word_options->x_dropoff = REPEATS_SEARCH_XDROP_UNGAPPED;
    options->ext_options->gap_x_dropoff_final = REPEATS_SEARCH_XDROP_FINAL;

    return status;
}

/** Create a SeqLoc with repeat masking locations, given the results of a 
 * BLAST search against a database of repeats.
 * @param query_seqloc Query sequence locations [in]
 * @param results Internal results structure, returned from a BLAST search
 *                against a repeats database [in]
 * @param mask_seqloc List of ValNode's, one per query, containing mask 
 *                    locations. [out] 
 */
static Int2
s_FillMaskLocFromBlastHSPResults(SeqLoc* query_seqloc, BlastHSPResults* results,
                                 SeqLoc* *mask_seqloc)
{
    Int4 num_seqs = 0;
    Int4 query_index;
    SeqLoc* slp;
    BlastMaskLoc* mask;

    if (!query_seqloc || !mask_seqloc)
        return -1;

    *mask_seqloc = NULL;

    if (!results) {
        return 0;
    }

    num_seqs = ValNodeLen(query_seqloc);
    mask = BlastMaskLocNew(num_seqs);

    for (query_index = 0, slp = query_seqloc; slp;
         ++query_index, slp = slp->next) {
        Int4 query_length, query_start;
        Int4 hit_index;
        BlastSeqLoc* loc_list = NULL, *ordered_loc_list = NULL;
        BlastSeqLoc* last_loc = NULL; 
        BlastHitList* hit_list = results->hitlist_array[query_index];
       
        if (!hit_list) {
            continue;
        }
        query_length = SeqLocLen(slp);
        query_start = SeqLocStart(slp);

        /* Find all HSP intervals in query */
        for (hit_index = 0; hit_index < hit_list->hsplist_count; ++hit_index) {
            Int4 hsp_index;
            BlastHSPList* hsp_list = hit_list->hsplist_array[hit_index];
            /* HSP lists cannot be NULL! */
            ASSERT(hsp_list);
            for (hsp_index = 0; hsp_index < hsp_list->hspcnt; ++hsp_index) {
                Int4 left, right;
                BlastHSP* hsp = hsp_list->hsp_array[hsp_index];
                /* HSP cannot be NULL! */
                ASSERT(hsp);
                if (hsp->query.frame == hsp->subject.frame) {
                    left = hsp->query.offset;
                    right = hsp->query.end - 1;
                } else {
                    left = query_length - hsp->query.end;
                    right = query_length - hsp->query.offset - 1;
                }
                /* Shift the coordinates so they correspond to the full 
                   sequence. */
                left += query_start;
                right += query_start;
                /* If this is the first mask for this query, create a new 
                   BlastSeqLoc, otherwise append to the end of the list. */
                if (!last_loc)
                    loc_list = last_loc = BlastSeqLocNew(NULL, left, right);
                else
                    last_loc = BlastSeqLocNew(&last_loc, left, right);
            }
        }
        /* Make the intervals unique */
        CombineMaskLocations(loc_list, &ordered_loc_list, 
                             REPEAT_MASK_LINK_VALUE);

        /* Free the list of locations that's no longer needed. */
        loc_list = BlastSeqLocFree(loc_list);

        mask->seqloc_array[query_index] = ordered_loc_list;
    }

    *mask_seqloc = BlastMaskLocToSeqLoc(eBlastTypeBlastn, mask, query_seqloc);

    mask = BlastMaskLocFree(mask);

    return 0;
}

Int2
Blast_FindRepeatFilterSeqLoc(SeqLoc* query_seqloc,
                             const char* filter_string, 
                             SeqLoc* *mask_loc)
{
    char* repeat_database = NULL;
    SBlastOptions* options = NULL;
    Blast_SummaryReturn* sum_returns = NULL;
    Int2 status = 0;
    BlastSeqSrc* seq_src = NULL;
    SeqLoc* filter_loc = NULL; /* Dummy variable, since search will be performed 
                                  without filtering. */
    Boolean mask_at_hash = FALSE; /* Dummy variable. */
    BlastHSPResults* results = NULL;
    SBlastFilterOptions* filtering_options = NULL;

    if (filter_string == NULL)
       return 0;
    
    status = BlastFilteringOptionsFromString(eBlastTypeBlastn, filter_string,
        &filtering_options, NULL);

    if (status)
      return status;

    /* If repeat filtering not requested, return success. */
    if (filtering_options == NULL || filtering_options->repeatFilterOptions == NULL)
    {
        filtering_options = SBlastFilterOptionsFree(filtering_options);
        return 0;
    }
    
    repeat_database = filtering_options->repeatFilterOptions->database;

    sum_returns = Blast_SummaryReturnNew();
    status = SBlastOptionsNew("blastn", &options, sum_returns);
    
    seq_src = ReaddbBlastSeqSrcInit(repeat_database, FALSE, 0, 0);
    
    if (!seq_src) {
        Blast_MessageWrite(&sum_returns->error, 2, 1, 0,  
                           "Initialization of subject sequences source failed");
    } else {
        char* error_str = BlastSeqSrcGetInitError(seq_src);
        if (error_str)
            Blast_MessageWrite(&sum_returns->error, 2, 1, 0, error_str); 
    }

    /* If there was an error initializing the sequence source, return without 
       doing the search. */
    if (sum_returns->error) {
        *mask_loc = NULL;
        filtering_options = SBlastFilterOptionsFree(filtering_options);
        return -1;
    }

    s_SetRepeatsSearchOptions(options);

    status = 
        Blast_RunSearch(query_seqloc, seq_src, NULL, options, NULL,
                        &results, &filter_loc, &mask_at_hash, sum_returns);

    /* The ReadDBFILE structure will not be destroyed here, because the 
       initialising function used readdb_attach */
    BlastSeqSrcFree(seq_src);

    Blast_SummaryReturnFree(sum_returns);

    /* filter_loc must be NULL on return from Blast_RunSearch, but call the 
       destruction function anyway - in case changes are made in the future. */
    filter_loc = Blast_ValNodeMaskListFree(filter_loc);

    options = SBlastOptionsFree(options);
    filtering_options = SBlastFilterOptionsFree(filtering_options);

    if (!status) {
        /* Extract the repeat locations from the search results */
        s_FillMaskLocFromBlastHSPResults(query_seqloc, results, mask_loc);
    }

    results = Blast_HSPResultsFree(results);
    return status;
}

/* @} */

