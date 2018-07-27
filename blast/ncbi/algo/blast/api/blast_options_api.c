/* $Id: blast_options_api.c,v 1.8 2005/03/09 13:37:53 camacho Exp $
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
*  Author: Ilya Dondoshansky                                              *
**************************************************************************/

/** @file blast_options_api.c
 * Functions for C toolkit applications to perform a BLAST search, using the 
 * core engine, shared between C and C++ toolkits.
 */

#include <algo/blast/api/blast_options_api.h>
#include <algo/blast/core/blast_util.h>
#include <algo/blast/core/blast_filter.h>
#include <algo/blast/api/blast_seq.h>

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

Int2 SBlastOptionsNew(const char* program_name, SBlastOptions** options_out,
                      Blast_SummaryReturn* extra_returns)
{
   QuerySetUpOptions* query_options=NULL;	
   LookupTableOptions* lookup_options=NULL;
   BlastInitialWordOptions* word_options=NULL;
   BlastScoringOptions* score_options=NULL;
   BlastExtensionOptions* ext_options=NULL;
   BlastHitSavingOptions* hit_options=NULL;
   BlastEffectiveLengthsOptions* eff_len_options=NULL;
   PSIBlastOptions* psi_options = NULL;
   BlastDatabaseOptions* db_options = NULL;
   SBlastOptions* options;
   EBlastProgramType program = eBlastTypeUndefined;
   Int2 status = 0;

   if (!options_out || !extra_returns)
       return -1;

   BlastProgram2Number(program_name, &program);
   if (program == eBlastTypeUndefined) {
       char message[256];

       sprintf(message, 
               "Program name %s is not supported. The supported programs "
               "are blastn, blastp, blastx, tblastn, tblastx, rpsblast, "
               "rpstblastn\n", program_name);
       Blast_MessageWrite(&extra_returns->error, 2, 1, 0, message);
       return -1;
   }

   status = 
       BLAST_InitDefaultOptions(program, &lookup_options, &query_options, 
           &word_options, &ext_options, &hit_options, &score_options, 
           &eff_len_options, &psi_options, &db_options);
   
   if (status) {
       *options_out = NULL;
       Blast_MessageWrite(&extra_returns->error, 2, 1, 0, 
                          "Failed to initialize default options\n");
       return status;
   }

   if (program == eBlastTypeTblastn || program == eBlastTypeRpsTblastn ||
       program == eBlastTypeTblastx) {
       if ((status = BLAST_GeneticCodeFind(db_options->genetic_code, 
                                           &db_options->gen_code_string)))
           return status;
   }
   
   *options_out = options = (SBlastOptions*) calloc(1, sizeof(SBlastOptions));
   options->program = program;
   options->query_options = query_options;
   options->lookup_options = lookup_options;
   options->word_options = word_options;
   options->ext_options = ext_options;
   options->score_options = score_options;
   options->hit_options = hit_options;
   options->eff_len_options = eff_len_options;
   options->psi_options = psi_options;
   options->db_options = db_options;
   options->num_cpus = 1;

   /* Set default filter string to low complexity filtering. */
   SBlastOptionsSetFilterString(options, "T");

   return status;
}

SBlastOptions* SBlastOptionsFree(SBlastOptions* options)
{
    if (options) {
        LookupTableOptionsFree(options->lookup_options);
        BlastQuerySetUpOptionsFree(options->query_options);
        BlastExtensionOptionsFree(options->ext_options);
        BlastHitSavingOptionsFree(options->hit_options);
        BlastInitialWordOptionsFree(options->word_options);
        BlastScoringOptionsFree(options->score_options);
        BlastEffectiveLengthsOptionsFree(options->eff_len_options);
        PSIBlastOptionsFree(options->psi_options);
        BlastDatabaseOptionsFree(options->db_options);
        sfree(options);
    }
    return NULL;
}

Int2 SBlastOptionsSetEvalue(SBlastOptions* options, double evalue)
{
    if (options && options->hit_options) {
        options->hit_options->expect_value = evalue;
        return 0;
    } else
        return -1;
}

Int2 SBlastOptionsSetWordSize(SBlastOptions* options, Int4 word_size)
{
    if (options && options->lookup_options) {
        options->lookup_options->word_size = word_size;
        return 0;
    } else
        return -1;
}

Int2 SBlastOptionsSetDiscMbParams(SBlastOptions* options, Int4 template_length,
                                 Int4 template_type)
{
    if (!options || !options->lookup_options)
        return -1;

    options->lookup_options->lut_type = MB_LOOKUP_TABLE;
    options->lookup_options->mb_template_length = template_length;
    options->lookup_options->mb_template_type = template_type;
    
    return 0;
}

Int2 SBlastOptionsSetMatrixAndGapCosts(SBlastOptions* options, 
                                       const char* matrix_name, 
                                       Int4 gap_open, Int4 gap_extend)
{
    Int2 status = 0;

    if (!matrix_name || !options || !options->score_options)
        return -1;
    
    status = 
        BlastScoringOptionsSetMatrix(options->score_options, matrix_name);

    options->score_options->gap_open = gap_open;
    options->score_options->gap_extend = gap_extend;

    return status;
}

Int2 SBlastOptionsSetFilterString(SBlastOptions* options, const char* str)
{
    Int2 status = 0;
    if (!options || !options->query_options)
        return -1;

    /* Reset filtering options */
    sfree(options->query_options->filter_string);
    options->query_options->filter_string = strdup(str);
    options->query_options->filtering_options =
         SBlastFilterOptionsFree(options->query_options->filtering_options);
    status = BlastFilteringOptionsFromString(options->program, 
        options->query_options->filter_string, 
        &options->query_options->filtering_options, NULL);
    return status;
}

Int2 SBlastOptionsSetDbGeneticCode(SBlastOptions* options, Int4 gc)
{
    Int2 status = 0;

    if (gc == 0)
        return 0;

    if (!options || !options->db_options)
        return -1;

    options->db_options->genetic_code = gc;

    status = BLAST_GeneticCodeFind(gc, &options->db_options->gen_code_string);

    return status;
    
}

/* @} */

