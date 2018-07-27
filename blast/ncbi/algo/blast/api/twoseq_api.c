/* $Id: twoseq_api.c,v 1.45 2005/04/27 14:53:19 papadopo Exp $
***************************************************************************
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
* Author: Jason Papadopoulos                                              *
***************************************************************************/

/** @file twoseq_api.c
 * Functions for C toolkit applications to compare two sequences using the 
 * rewritten blast engine. 
 */

#include <algo/blast/core/blast_options.h>
#include <algo/blast/core/blast_setup.h>
#include <algo/blast/core/blast_message.h>
#include <algo/blast/core/blast_util.h>
#include <algo/blast/core/blast_engine.h>
#include <algo/blast/core/mb_lookup.h>
#include <algo/blast/core/blast_filter.h>
#include <algo/blast/core/hspstream_collector.h>
#include <algo/blast/api/seqsrc_multiseq.h>
#include <algo/blast/api/blast_seqalign.h>
#include <algo/blast/api/blast_seq.h>
#include <algo/blast/api/twoseq_api.h>
#include <algo/blast/api/blast_returns.h>

#include <algo/blast/api/blast_api.h>

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

Int2 BLAST_SummaryOptionsInit(BLAST_SummaryOptions **options)
{
    BLAST_SummaryOptions *new_options = (BLAST_SummaryOptions *)calloc(1,
                                                 sizeof(BLAST_SummaryOptions));
    if (new_options == NULL) {
        *options = NULL;
        return -1;
    }
    
    new_options->hint = eSensitive;
    new_options->program = eChoose;
    new_options->strand = Seq_strand_both;
    new_options->cutoff_evalue = 10.0;
    new_options->gapped_calculation = TRUE;
    new_options->use_megablast = FALSE;
    new_options->nucleotide_match = 1;
    new_options->nucleotide_mismatch = -3;
    new_options->longest_intron = 0;
    new_options->init_seed_method = eDefaultSeedType;

    *options = new_options;
    return 0;
} 

BLAST_SummaryOptions*
BLAST_SummaryOptionsFree(BLAST_SummaryOptions *options)
{
    sfree(options->matrix);
    sfree(options->filter_string);
    sfree(options);
    return NULL;
}

/** Fills the core options structures, given the summary options.
 * @param basic_options Basic options set by client [in]
 * @param options All internal options structures [in]
 * @param query_length Length of query sequence [in]
 */
static Int2 
s_TwoSeqBasicFillOptions(const BLAST_SummaryOptions* basic_options,
                         SBlastOptions* options,
                         Int4 query_length)
{
    Int2 status = 0;
    EBlastProgramType program_number = options->program;
    LookupTableOptions* lookup_options = options->lookup_options;
    QuerySetUpOptions* query_setup_options = options->query_options; 
    BlastInitialWordOptions* word_options = options->word_options;
    BlastExtensionOptions* ext_options = options->ext_options;
    BlastHitSavingOptions* hit_options = options->hit_options;
    BlastScoringOptions* score_options = options->score_options;
    BlastEffectiveLengthsOptions* eff_len_options = options->eff_len_options;
    BlastDatabaseOptions* db_options = options->db_options; 
    Boolean do_megablast = FALSE;
    Boolean do_ag_blast = FALSE;
    Boolean do_discontig = FALSE;
    Int4 greedy_align = 0;
    Int2 word_size = basic_options->word_size;
    char *matrix;

    if (program_number == eBlastTypeTblastn || 
        program_number == eBlastTypeTblastx) {
        if ((status = BLAST_GeneticCodeFind(db_options->genetic_code,
                                            &db_options->gen_code_string)))
            return status;
    }

    if (program_number == eBlastTypeBlastn) {
        if (basic_options->strand != Seq_strand_plus &&
            basic_options->strand != Seq_strand_minus &&
            basic_options->strand != Seq_strand_both) {
            return -2;
        }
        
        if (basic_options->use_megablast == TRUE)
           do_megablast = TRUE;

        /* If the query sequence is large enough, set up a megablast search */

        if (basic_options->hint != eNone && 
            query_length > MEGABLAST_CUTOFF) {
            do_megablast = TRUE;
            if (basic_options->gapped_calculation)
                greedy_align = 1;       /* one-pass, no ungapped */
        }

        /* For a megablast search or a blastn search with
           a non-default word size, turn on striding. Note that
           striding is beneficial even if the wordsize is
           smaller than the default */

        if (word_size != 0 || do_megablast)
            do_ag_blast = TRUE;

        /* If megablast was turned on but the input indicates a sensitive search
           is desired, or if word size is <=12, which is not used in contiguous
           megablast, switch to discontiguous megablast. 
           Because a sensitive search is the default, discontig. megablast will 
           be used by default when the first input sequence is large. */
        if (do_megablast && 
            (basic_options->hint == eSensitive || 
             (word_size != 0 && word_size <= 12))) {
            if (word_size == 0 || word_size > 12)
                word_size = 11;
            do_discontig = TRUE;
            do_ag_blast = FALSE;
        }
    }
    

    BLAST_FillLookupTableOptions(lookup_options, 
                                 program_number, 
                                 do_megablast,
                                 basic_options->word_threshold,
                                 word_size,
                                 0,              /* no variable wordsize */ 
                                 FALSE);         /* no PSSM */ 
 
    /* If discontiguous megablast is specified, choose
       the 11-of-21 optimal template).*/

    if (do_discontig) {
        lookup_options->mb_template_length = 21; 
        lookup_options->mb_template_type = MB_WORD_OPTIMAL;
    }
    else {
        lookup_options->mb_template_length = 0; 
        lookup_options->mb_template_type = 0;
    }
    
    BLAST_FillQuerySetUpOptions(query_setup_options, 
                                program_number, 
                                basic_options->filter_string,
                                basic_options->strand);
 
    BLAST_FillInitialWordOptions(word_options, 
                                 program_number, 
                                 (Boolean)greedy_align, 
                                 0,      /* default window size. */
                                 0);     /* default ungapped X dropoff */
 
    /* If we need to enforce a single-hit method, reset window size to 0. 
       To enforce two-hit method, set window size to a default non-zero 
       value */
    if (basic_options->init_seed_method == eOneHit)
       word_options->window_size = 0;
    else if (basic_options->init_seed_method == eTwoHits)
       word_options->window_size = BLAST_WINDOW_SIZE_PROT;

    BLAST_FillExtensionOptions(ext_options, 
                               program_number, 
                               greedy_align, 
                               basic_options->gap_x_dropoff,
                               0);       /* default final X dropoff */
 
    if (basic_options->matrix == NULL)
        matrix = BLAST_DEFAULT_MATRIX; /* BLOSUM62 */
    else
        matrix = basic_options->matrix;

    BLAST_FillScoringOptions(score_options, 
                             program_number, 
                             (Boolean)greedy_align, 
                             basic_options->nucleotide_mismatch,
                             basic_options->nucleotide_match,
                             matrix,
                             basic_options->gap_open,
                             basic_options->gap_extend);
 
    score_options->gapped_calculation = basic_options->gapped_calculation;
 
    BLAST_FillHitSavingOptions(hit_options, 
                               basic_options->cutoff_evalue,
                               0,     /* default number of alignments saved */
                               score_options->gapped_calculation,
                               0);    /* do not perform culling */

    hit_options->percent_identity = 0;   /* no percent identity cutoff */
    hit_options->longest_intron = basic_options->longest_intron;   /* For uneven gap statistics. */
  
    eff_len_options->db_length = (Int8)basic_options->db_length;

    return 0;
}

Int2 
BLAST_TwoSequencesSearch(BLAST_SummaryOptions *basic_options,
                         BioseqPtr bsp1, BioseqPtr bsp2, 
                         SeqAlign **seqalign_out)
{
    enum blast_type program_type = eChoose;
    SeqLocPtr query_slp = NULL;      /* sequence variables */
    SeqLocPtr subject_slp = NULL;
    Boolean seq1_is_aa, seq2_is_aa;
    Int2 status = 0;

    /* sanity checks */

    *seqalign_out = NULL;
    if (bsp1 == NULL || bsp2 == NULL)
        return 0;

    seq1_is_aa = ISA_aa(bsp1->mol);
    seq2_is_aa = ISA_aa(bsp2->mol);

    /* Find program type consistent with the sequences. */
    if (!seq1_is_aa && !seq2_is_aa) {
       if (basic_options->program == eTblastx)
          program_type = eTblastx;
       else
          program_type = eBlastn;
    } else if (seq1_is_aa && seq2_is_aa) {
       program_type = eBlastp;
    } else if (!seq1_is_aa && seq2_is_aa) {
       program_type = eBlastx;
    } else if (seq1_is_aa && !seq2_is_aa) {
       program_type = eTblastn;
    }

    /* Check if program type in options is consistent with the one determined
       from sequences. */
    if (basic_options->program == eChoose)
       basic_options->program = program_type;
    else if (basic_options->program != program_type)
       return -1;

    /* Convert the bioseqs into seqlocs. */

    ValNodeAddPointer(&query_slp, SEQLOC_WHOLE,
                      SeqIdDup(SeqIdFindBest(bsp1->id, SEQID_GI)));
    if (!query_slp)
       return -1;
    ValNodeAddPointer(&subject_slp, SEQLOC_WHOLE,
                      SeqIdDup(SeqIdFindBest(bsp2->id, SEQID_GI)));
    if (!subject_slp)
       return -1;

    status = BLAST_TwoSeqLocSets(basic_options, query_slp, subject_slp, 
                                 NULL, seqalign_out, NULL, NULL, NULL);
    SeqLocFree(query_slp);
    SeqLocFree(subject_slp);

    return status;
}

/** Calculates total length of a list of sequence locations. 
 * @param seqloc List of SeqLoc's [in]
 * @return Total length of all SeqLoc's in the list.
 */
static Int4 
s_SeqLocListLen(SeqLoc* seqloc)
{
   Int4 length = 0;

   for ( ; seqloc; seqloc = seqloc->next)
      length += SeqLocLen(seqloc);

   return length;
}

Int2 
BLAST_TwoSeqLocSets(const BLAST_SummaryOptions *basic_options,
                    SeqLoc* query_seqloc, SeqLoc* subject_seqloc, 
                    SeqLoc* masking_locs,
                    SeqAlign **seqalign_out,
                    SeqLoc** filter_out,
                    Boolean* mask_at_hash,
                    Blast_SummaryReturn* *extra_returns_ptr)
{
    SBlastOptions* options = NULL;
    const char *kProgram = NULL;
    Int2 status = 0;
    Blast_SummaryReturn* extra_returns;

    if (!basic_options || !query_seqloc || !subject_seqloc)
        return -1;

    switch(basic_options->program) {
    case eBlastn:
       kProgram = "blastn";
       break;
    case eBlastp:
       kProgram = "blastp";
       break;
    case eBlastx:
       kProgram = "blastx";
       break;
    case eTblastn:
       kProgram = "tblastn";
       break;
    case eTblastx:
       kProgram = "tblastx";
       break;
    default:
       return -1;
    }

    extra_returns = Blast_SummaryReturnNew();
    status = SBlastOptionsNew(kProgram, &options, extra_returns);

    if (status)
        return -1;
    
    status = s_TwoSeqBasicFillOptions(basic_options, options, 
                                      s_SeqLocListLen(query_seqloc));

    if (!status) {
        status = 
            Blast_TwoSeqLocSetsAdvanced(query_seqloc, subject_seqloc, 
                masking_locs, options, NULL, seqalign_out, filter_out, 
                mask_at_hash, extra_returns);
    }

    options = SBlastOptionsFree(options);

    if (extra_returns_ptr)
        *extra_returns_ptr = extra_returns;
    else 
        Blast_SummaryReturnFree(extra_returns);

    return status;
}

/* @} */

