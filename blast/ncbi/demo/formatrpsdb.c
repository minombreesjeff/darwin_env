static char const rcsid[] = "$Id: formatrpsdb.c,v 1.15 2005/02/22 14:17:31 camacho Exp $";

/*****************************************************************************

  
                          PUBLIC DOMAIN NOTICE
              National Center for Biotechnology Information

    This software/database is a "United States Government Work" under the
    terms of the United States Copyright Act.  It was written as part of    
    the author's official duties as a United States Government employee
    and thus cannot be copyrighted.  This software/database is freely
    available to the public for use. The National Library of Medicine and
    the U.S. Government have not placed any restriction on its use or
    reproduction.

    Although all reasonable efforts have been taken to ensure the accuracy
    and reliability of the software and data, the NLM and the U.S.
    Government do not and cannot warrant the performance or results that
    may be obtained by using this software or data. The NLM and the U.S.
    Government disclaim all warranties, express or implied, including
    warranties of performance, merchantability or fitness for any
    particular purpose.

    Please cite the author in any work or product based on this material.

***************************************************************************

   File Name:  formatrpsdb.c

   Author:  Jason Papadopoulos
   
   File Description: convert a collection of files, each containing
        a sequence and position-specific scoring matrix encoded in
        an ASN.1 Score-matrix-parameters format, into a protein database.
        Auxiliary files are also generated that make the generated
        database suitable for use with RPS blast.

***************************************************************************
    $Log: formatrpsdb.c,v $
    Revision 1.15  2005/02/22 14:17:31  camacho
    Fix bioseq data type

    Revision 1.14  2005/02/08 17:03:12  papadopo
    performance optimization: do not build the PSSM linked list incrementally

    Revision 1.13  2005/01/10 13:48:20  madden
    Change to BLAST_FillInitialWordOptions prototype

    Revision 1.12  2004/11/23 13:56:19  camacho
    Add quotations to matrix names in error message

    Revision 1.11  2004/11/23 13:52:31  camacho
    Add matrix names to error message when there is a mismatch

    Revision 1.10  2004/11/04 20:52:31  papadopo
    prepend 'Blast' to RPSInfo and related structures

    Revision 1.9  2004/10/12 15:12:35  papadopo
    1. Modify scoremat IO to comply with new scoremat spec
    2. Remove check that residue frequencies read from scoremat are <= 1.0
    3. Add input argument to specify the underlying score matrix, or to
    	use the score matrix specified in the scoremat if present

    Revision 1.8  2004/09/15 18:06:13  papadopo
    1. Verify that the scale factor is the same for all input scoremats
    2. Do not use the scoremat 'identifier' field to determine the underlying
    	score matrix; hardwire to BLOSUM62 temporarily
    3. Use BlastSeqLoc's instead of ListNodes

    Revision 1.7  2004/08/25 14:47:50  camacho
    Removed duplicate code

    Revision 1.6  2004/07/29 14:15:00  papadopo
    remove (obsolete) call to MakeAllWordSequence

    Revision 1.5  2004/07/19 17:17:47  papadopo
    if an input scoremat only contains residue frequencies, perform impala-specific scaling and create a PSSM

    Revision 1.4  2004/07/12 16:30:44  papadopo
    LookupTable->BlastLookupTable

    Revision 1.3  2004/06/30 19:52:00  camacho
    Added #include <blfmtutl.h>

    Revision 1.2  2004/06/29 16:00:44  papadopo
    add explicit prototype for BlastGetVersionNumber(), use enum value instead of explicit 0

    Revision 1.1  2004/06/28 14:02:52  papadopo
    Initial revision


**************************************************************************/

#include <ncbi.h>
#include <sequtil.h>
#include <readdb.h>
#include <sqnutils.h>
#include <tofasta.h>
#include <objseq.h>
#include <objscoremat.h>
#include <blfmtutl.h>
#include <posit.h>
#include <profiles.h>

#include <algo/blast/core/blast_options.h>
#include <algo/blast/core/blast_lookup.h>
#include <algo/blast/core/blast_filter.h>
#include <algo/blast/core/blast_stat.h>
#include <algo/blast/core/blast_rps.h>
#include <algo/blast/core/blast_util.h>

/* compile-time check */

#if (HITS_ON_BACKBONE != RPS_HITS_PER_CELL)
#error "Blastp lookup table incompatible with RPS lookup table"
#endif

/* binary compatibility with RPS blast *requires*
   the following */

#define RPS_NUM_LOOKUP_CELLS 32768

/* RPS-related data */

typedef struct RPS_DbInfo {
    Int4 num_seqs;
    char lookup_file[PATH_MAX];
    char pssm_file[PATH_MAX];
    char aux_file[PATH_MAX];
    char matrix_file[PATH_MAX];
    FILE * lookup_fd;
    FILE * pssm_fd;
    FILE * aux_fd;
    Int4 **posMatrix;
    Int4 alphabet_size;
    Int4 gap_open;
    Int4 gap_extend;
    Int4 scale_factor;
    Int4 curr_seq_offset;
    QuerySetUpOptions *query_options;
    LookupTableOptions *lookup_options;
    BlastLookupTable *lookup;
} RPS_DbInfo;

/* program's arguments */

Args dump_args[] = {
    { "Title for database file",
      NULL, NULL, NULL, TRUE, 't', ARG_STRING, 0.0, 0, NULL},
    {"Input file containing list of ASN.1 Scoremat filenames",
     NULL, NULL,NULL,FALSE,'i',ARG_FILE_IN, 0.0,0,NULL},
    {"Logfile name",
     "formatrpsdb.log", NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL},
    {"Create index files for database",
      "F", NULL, NULL, TRUE, 'o', ARG_BOOLEAN, 0.0, 0, NULL},
    {"Database volume size in millions of letters",
      "0", "0", NULL, TRUE, 'v', ARG_INT, 0.0, 0, NULL},
    {"Scoremat files are binary",
      "F", NULL, NULL, TRUE, 'b', ARG_BOOLEAN, 0.0, 0, NULL},
    {"Threshold for extending hits for RPS database",
      "11.0", NULL, NULL, TRUE, 'f', ARG_FLOAT, 0.0, 0, NULL},
    {"Base name of output database (same as input file if not specified)",
      NULL, NULL, NULL, TRUE, 'n', ARG_STRING, 0.0, 0, NULL},
    {"For scoremats that contain only residue frequencies, the scaling "
     "factor to apply when creating PSSMs",
      "100.0", NULL, NULL, TRUE, 'S', ARG_FLOAT, 0.0, 0, NULL},
    {"The gap opening penalty (if not present in the scoremat)",
      "11", NULL, NULL, TRUE, 'G', ARG_INT, 0.0, 0, NULL},
    {"The gap extension penalty (if not present in the scoremat)",
      "1", NULL, NULL, TRUE, 'E', ARG_INT, 0.0, 0, NULL},
    {"Underlying score matrix (if not present in the scoremat)",
      "BLOSUM62", NULL, NULL, TRUE, 'U', ARG_STRING, 0.0, 0, NULL},
};

enum {
    title_arg = 0,
    input_arg,
    logfile_arg,
    index_arg,
    dbsize_arg,
    binary_arg,
    threshold_arg,
    output_arg,
    scale_arg,
    gap_open_arg,
    gap_extend_arg,
    underlying_matrix_arg,
    NUMARG               /* must be last */
};

/* Initialize the structures that will become
   the RPS auxiliary files */

void RPS_DbInfoInit(RPS_DbInfo *info, Int4 num_files, char *db_name) 
{
    Int4 i;
    memset(info, 0, sizeof(*info));

    sprintf(info->pssm_file, "%s.rps", db_name);
    info->pssm_fd = FileOpen(info->pssm_file, "wb");
    if (info->pssm_fd == NULL)
        ErrPostEx(SEV_FATAL, 1, 0, "Could not open %s\n", info->pssm_file);

    sprintf(info->lookup_file, "%s.loo", db_name);
    info->lookup_fd = FileOpen(info->lookup_file, "wb");
    if (info->lookup_fd == NULL)
        ErrPostEx(SEV_FATAL, 1, 0, "Could not open %s\n", info->lookup_fd);

    sprintf(info->aux_file, "%s.aux", db_name);
    info->aux_fd = FileOpen(info->aux_file, "w");
    if (info->aux_fd == NULL)
        ErrPostEx(SEV_FATAL, 1, 0, "Could not open %s\n", info->aux_file);

    /* Write the magic numbers to the PSSM file */

    i = RPS_MAGIC_NUM;
    FileWrite(&i, sizeof(Int4), 1, info->pssm_fd);

    /* Fill in space for the sequence offsets. The PSSM
       data gets written after this list of integers. Also
       write the number of sequences to the PSSM file */

    FileWrite(&num_files, sizeof(Int4), 1, info->pssm_fd);
    info->num_seqs = num_files;
    for (i = 0; i < num_files + 1; i++)
        FileWrite(&i, sizeof(Int4), 1, info->pssm_fd);
}

/* Update the input scoremat with a new PSSM and modified
   statistics. Scoremat must contain only residue frequencies
        seq is the sequence and set of score frequencies read in 
                from the next data file
        seq_size is the number of letters in this sequence
        alphabet_size refers to the number of PSSM rows
        ScalingFactor is the multiplier for all PSSM scores
*/
Int2 RPSUpdateStatistics(PssmWithParameters *seq,
                         Int4 seq_size, Int4 alphabet_size,
                         Nlm_FloatHi ScalingFactor)
{
    compactSearchItems *compactSearch;
    posSearchItems *posSearch;
    BLAST_ResFreqPtr stdrfp;

    Int4 i, j;
    Uint1 *query;
    Bioseq *bsp;
    BLAST_ScoreBlk *sbp;
    PssmPtr pssm = seq->pssm;
    PssmParametersPtr params = seq->params;
    Int4 *open, *extend;
    Int4 array_size;
    Nlm_FloatHi *lambda, *K, *H;
    ValNodePtr freq_list;
    ValNodePtr score_list;
    CharPtr matrix_name;

    /* pick out the underlying score matrix */

    matrix_name = params->rpsdbparams->matrixName;

    /* allocate query array and PSSM row array */

    posSearch = (posSearchItems *) MemNew (sizeof(posSearchItems));
    compactSearch = (compactSearchItems *)MemNew(sizeof(compactSearchItems)); 
    query = (Uint1 *)MemNew(seq_size * sizeof(Uint1));

    /* Read in the sequence residues from the scoremat structure. */

    bsp = pssm->query->data.ptrvalue;
    if (!bsp->seq_data || !ISA_aa(bsp->mol) || 
        bsp->seq_data_type != Seq_code_ncbistdaa) {
        return -1;
    }

    BSSeek(bsp->seq_data, 0, SEEK_SET);
    BSRead(bsp->seq_data, query, seq_size);
    compactSearch->query = query;
    compactSearch->qlength = seq_size;
        
    /* set up a score block */

    sbp = BLAST_ScoreBlkNew(Seq_code_ncbistdaa, 1);
    if (sbp == NULL)
        return -2;
    sbp->read_in_matrix = TRUE;
    sbp->protein_alphabet = TRUE;
    sbp->posMatrix = NULL;
    sbp->number_of_contexts = 1;
    BlastScoreBlkMatFill(sbp, matrix_name);
    compactSearch->matrix = sbp->matrix;
    compactSearch->alphabetSize = alphabet_size;
    compactSearch->gapped_calculation = TRUE;
    compactSearch->pseudoCountConst = 10;
    compactSearch->ethresh = 0.001;
    BlastScoreBlkFill(sbp,  (CharPtr) query, seq_size, 0);
        
    /* manually set up all the different Karlin blocks */

    sbp->kbp_gap_std[0] = BlastKarlinBlkCreate();
    BlastKarlinBlkGappedCalc(sbp->kbp_gap_std[0], 
                             params->rpsdbparams->gapOpen,
                             params->rpsdbparams->gapExtend, 
                             sbp->name, NULL);
    sbp->kbp_gap_psi[0] = BlastKarlinBlkCreate();
    BlastKarlinBlkGappedCalc(sbp->kbp_gap_psi[0], 
                             params->rpsdbparams->gapOpen,
                             params->rpsdbparams->gapExtend, 
                             sbp->name, NULL);
    if (sbp->kbp_gap_std[0] == NULL || sbp->kbp_gap_psi[0] == NULL)
        return -3;
        
    array_size = BlastKarlinGetMatrixValues(sbp->name, &open, &extend, 
                                            &lambda, &K, &H, NULL);
    if (array_size > 0) {
        for (i = 0; i < array_size; i++) {
            if (open[i] == INT2_MAX && extend[i] == INT2_MAX) {
                sbp->kbp_ideal = BlastKarlinBlkCreate();
                sbp->kbp_ideal->Lambda = lambda[i];
                sbp->kbp_ideal->K = K[i];
                sbp->kbp_ideal->H = H[i];
                break;
            }
        }
        MemFree(open);
        MemFree(extend);
        MemFree(lambda);
        MemFree(K);
        MemFree(H);
    }
    if (sbp->kbp_ideal == NULL)
       sbp->kbp_ideal = BlastKarlinBlkStandardCalcEx(sbp);

    /* point to all the Karlin blocks just generated */

    compactSearch->lambda = sbp->kbp_gap_std[0]->Lambda;
    compactSearch->kbp_std = sbp->kbp_std;
    compactSearch->kbp_psi = sbp->kbp_psi;
    compactSearch->kbp_gap_psi = sbp->kbp_gap_psi;
    compactSearch->kbp_gap_std = sbp->kbp_gap_std;
    compactSearch->lambda_ideal = sbp->kbp_ideal->Lambda;
    compactSearch->K_ideal = sbp->kbp_ideal->K;
    
    /* compute the ideal residue frequencies */

    stdrfp = BlastResFreqNew(sbp);
    BlastResFreqStdComp(sbp,stdrfp); 
    compactSearch->standardProb = MemNew(alphabet_size * sizeof(double));
    for(i = 0; i < alphabet_size; i++)
        compactSearch->standardProb[i] = stdrfp->prob[i];
    stdrfp = BlastResFreqDestruct(stdrfp);

    /* allocate space for a PSSM and for the position frequencies */

    posSearch->posMatrix = (Int4 **)MemNew((seq_size + 1) * sizeof(Int4 *));
    posSearch->posPrivateMatrix = (Int4 **)MemNew((seq_size + 1) * 
                                                  sizeof(Int4 *));
    posSearch->posFreqs = (Nlm_FloatHi **)MemNew((seq_size + 1) * 
                                                 sizeof(Nlm_FloatHi *));
    for (i = 0; i < seq_size + 1; i++) {
        posSearch->posMatrix[i] = (Int4 *)MemNew(alphabet_size * sizeof(Int4));
        posSearch->posPrivateMatrix[i] = (Int4 *)MemNew(alphabet_size * 
                                                        sizeof(Int4));
        posSearch->posFreqs[i] = (Nlm_FloatHi *)MemNew(alphabet_size * 
                                                       sizeof(Nlm_FloatHi));
    }

    /* read in the list of position frequencies */

    freq_list = pssm->intermediateData->freqRatios;

    if (pssm->byRow == FALSE) {
        for (i = 0; i < seq_size; i++) {
            for (j = 0; j < alphabet_size; j++) {
                if (freq_list == NULL)
                    break;
                posSearch->posFreqs[i][j] = freq_list->data.realvalue;
                
                if (posSearch->posFreqs[i][j] < 0.0)
                    return -4;

                freq_list = freq_list->next;
            }
            if (j < alphabet_size)
                break;
        }
    }
    else {
        for (j = 0; j < alphabet_size; j++) {
            for (i = 0; i < seq_size; i++) {
                if (freq_list == NULL)
                    break;
                posSearch->posFreqs[i][j] = freq_list->data.realvalue;
                
                if (posSearch->posFreqs[i][j] < 0.0)
                    return -5;

                freq_list = freq_list->next;
            }
            if (i < seq_size)
                break;
        }
    }

    if (freq_list != NULL || i < seq_size || j < alphabet_size)
        return -6;

    /* finally, build the PSSM and scale it */

    posSearch->stdFreqRatios = PSIMatrixFrequencyRatiosNew(matrix_name);
    posFreqsToMatrix(posSearch, compactSearch);
    impalaScaling(posSearch, compactSearch, ScalingFactor, TRUE);
    posSearch->stdFreqRatios = PSIMatrixFrequencyRatiosFree(
                                          posSearch->stdFreqRatios);

    /* turn the computed PSSM into a linked list.
       Do *not* rely on ValNodeAddInt() to append new
       entries; build the whole list first */

    pssm->finalData->scalingFactor = Nlm_Nint(ScalingFactor);

    score_list = NULL;
    for (i = 0; i < seq_size * alphabet_size; i++) {
        ValNodePtr score_list_tmp = ValNodeNew(NULL);
        score_list_tmp->next = score_list;
        score_list = score_list_tmp;
    }
    pssm->finalData->scores = score_list;

    if (pssm->byRow == FALSE) {
        for (i = 0; i < seq_size; i++) {
            for (j = 0; j < alphabet_size; j++) {
                score_list->data.intvalue = posSearch->posPrivateMatrix[i][j];
                score_list = score_list->next;
            }
        }
    }
    else {
        for (j = 0; j < alphabet_size; j++) {
            for (i = 0; i < seq_size; i++) {
                score_list->data.intvalue = posSearch->posPrivateMatrix[i][j];
                score_list = score_list->next;
            }
        }
    }

    /* update the scoremat Karlin values with the ones just
       computed. RPS blast only needs the kappa field below */

    pssm->finalData->kappa = sbp->kbp_gap_psi[0]->K;

    /* clean up */

    MemFree(query);
    sbp = BLAST_ScoreBlkDestruct(sbp);
    compactSearch->standardProb = MemFree(compactSearch->standardProb);
    compactSearchDestruct(compactSearch);
    posCheckpointFreeMemory(posSearch, seq_size);
    MemFree(posSearch);
    return 0;
}

/* Incrementally update the RPS PSSM file with the
   PSSM for the next input sequence
        info contains all the information on data files
                and parameters from previously added sequences
        seq is the sequence and PSSM read in from the next data file
        seq_index refers to the (0-based) position of this sequence
                in the complete list of seqences
        seq_size is the number of letters in this sequence
        alphabet_size refers to the number of PSSM rows
*/
Int2 RPSUpdatePSSM(RPS_DbInfo *info, 
                    PssmWithParameters *seq, 
                    Int4 seq_index,
                    Int4 seq_size,
                    Int4 alphabet_size)
{
    Int4 i, j;
    ValNodePtr score_list;
    PssmPtr pssm = seq->pssm;

    /* point to the end of the PSSM file and dump
       the PSSM for this sequence, column by column.
       Since the scoremat stores the PSSM scores in
       a linked list, also verify that the list is
       exactly as long as expected. Note that RPS
       blast requires an extra column at the end
       of the PSSM */

    fseek(info->pssm_fd, 0, SEEK_END);
    score_list = pssm->finalData->scores;
    info->posMatrix = (Int4 **)MemNew((seq_size + 1) * sizeof(Int4 *));
    for (i = 0; i < seq_size + 1; i++)
        info->posMatrix[i] = (Int4 *)MemNew(alphabet_size * sizeof(Int4));

    if (pssm->byRow == FALSE) {
        for (i = 0; i < seq_size; i++) {
            for (j = 0; j < alphabet_size; j++) {
                if (score_list == NULL)
                    break;
                info->posMatrix[i][j] = score_list->data.intvalue;
                score_list = score_list->next;
            }
            if (j < alphabet_size)
                break;
        }
    }
    else {
        for (j = 0; j < alphabet_size; j++) {
            for (i = 0; i < seq_size; i++) {
                if (score_list == NULL)
                    break;
                info->posMatrix[i][j] = score_list->data.intvalue;
                score_list = score_list->next;
            }
            if (i < seq_size)
                break;
        }
    }

    if (i < seq_size || j < alphabet_size) {
        ErrPostEx(SEV_ERROR, 0, 0, "PSSM was truncated early");
        return 1;
    }
    if (score_list != NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "PSSM is too large for this sequence");
        return 1;
    }

    /* manually fill in the extra (last) column of the PSSM.
       Note that the value to use should more appropriately
       be BLAST_SCORE_MIN, but we instead follow the convention
       used in copymat */

    for (i = 0; i < alphabet_size; i++)
       info->posMatrix[seq_size][i] = -BLAST_SCORE_MAX;

    /* Dump the score matrix, column by column */

    for (i = 0; i < seq_size + 1; i++) {
       FileWrite(info->posMatrix[i], sizeof(Int4), 
                 alphabet_size, info->pssm_fd);
    }

    /* Write the next context offset. Note that the
       RPSProfileHeader structure is one int too large for
       our purposes, so that the index of this sequence
       must be decremented to get the right byte offset
       into the file */

    fseek(info->pssm_fd, sizeof(BlastRPSProfileHeader) + 
                        (seq_index - 1) * sizeof(Int4), SEEK_SET);
    FileWrite(&info->curr_seq_offset, sizeof(Int4), 1, info->pssm_fd);

    return 0;
}

/* Incrementally update the BLAST lookup table with
   words derived from the present sequence 
        info contains all the information on data files
                and parameters from previously added sequences
        seq is the sequence and PSSM read in from the next data file
        seq_size is the number of letters in this sequence
*/
Int2 RPSUpdateLookup(RPS_DbInfo *info, 
                    PssmWithParameters *seq, 
                    Int4 seq_size)
{
    BlastSeqLoc *lookup_segment = NULL;

    /* Tell the blast engine to index the entire input
       sequence. Since only the PSSM matters for lookup 
       table creation, the process does not require 
       actually extracting the sequence data from 'seq'*/

    BlastSeqLocNew(&lookup_segment, 0, seq_size - 1);

    /* add this sequence to the lookup table. NULL
       is passed in place of the query */

    if (_BlastAaLookupIndexQuery(info->lookup, info->posMatrix,
                                  NULL, lookup_segment,
                                  info->curr_seq_offset) != 0) {
        ErrPostEx(SEV_ERROR, 0, 0, "Cannot index BLAST query");
        return 1;
    }

    BlastSeqLocFree(lookup_segment);
    return 0;
}

/* The first sequence in the list determines several
   parameters that all other sequences in the list must
   have. In this case, extra initialization is required

        info contains all the information on data files
                and parameters from previously added sequences
        seq is the sequence and PSSM read in from the next data file
        seq_index refers to the (0-based) position of this sequence
                in the complete list of seqences
        seq_size is the number of letters in this sequence
        alphabet_size refers to the number of PSSM rows
*/
Int2 RPSAddFirstSequence(RPS_DbInfo *info, 
                    PssmWithParameters *seq, 
                    Int4 seq_size,
                    Int4 alphabet_size)
{
    PssmPtr pssm = seq->pssm;
    PssmParametersPtr params = seq->params;
    Int4 threshold;

    info->alphabet_size = alphabet_size;
    info->gap_open = params->rpsdbparams->gapOpen;
    info->gap_extend = params->rpsdbparams->gapExtend;
    info->scale_factor = pssm->finalData->scalingFactor;

    /* scale up the threshold value and convert to integer */

    threshold = (Int4)((double)info->scale_factor *
                       dump_args[threshold_arg].floatvalue);

    /* create BLAST lookup table */

    info->lookup_options = NULL;
    info->lookup = NULL;
    if (LookupTableOptionsNew(eBlastTypeBlastp, 
                              &info->lookup_options) != 0) {
        ErrPostEx(SEV_ERROR, 0, 0, "Cannot create lookup options");
        return 1;
    }

    if (BLAST_FillLookupTableOptions(info->lookup_options,
                                     eBlastTypeBlastp,
                                     FALSE, /* no megablast */
                                     threshold, /* neighboring threshold */
                                     BLAST_WORDSIZE_PROT,
                                     FALSE, /* no variable words */
                                     TRUE  /* use a PSSM */
                                    ) != 0) {
        ErrPostEx(SEV_ERROR, 0, 0, "Cannot set lookup table options");
        return 1;
    }

    if (BlastAaLookupNew(info->lookup_options, &info->lookup) != 0) {
        ErrPostEx(SEV_ERROR, 0, 0, "Cannot allocate lookup table");
        return 1;
    }

    /* Perform generic query setup */

    if (BlastQuerySetUpOptionsNew(&info->query_options) != 0) {
        ErrPostEx(SEV_ERROR, 0, 0, "Generic query setup failed");
        return 1;
    }

    if (BLAST_FillQuerySetUpOptions(info->query_options,
                                    eBlastTypeBlastp,
                                    NULL,        /* no filtering */
                                    0            /* strand not applicable */
                                   ) != 0) {
        ErrPostEx(SEV_ERROR, 0, 0, "Cannot fill query options");
        return 1;
    }

    /* Write the header of the RPS .aux file */

    fprintf(info->aux_fd, "BLOSUM62\n");
    fprintf(info->aux_fd, "%d\n", info->gap_open);
    fprintf(info->aux_fd, "%d\n", info->gap_extend);
    fprintf(info->aux_fd, "%le\n", 0.0); /* ungapped K not used by RPS blast */
    fprintf(info->aux_fd, "%le\n", 0.0); /* ungapped H not used by RPS blast */
    fprintf(info->aux_fd, "%d\n", 0);   /* max seq size not used by RPS blast */
    fprintf(info->aux_fd, "%d\n", 0);   /* DB size not used by RPS blast */
    fprintf(info->aux_fd, "%lf\n", (double)info->scale_factor);
    return 0;
}

/* Perform basic checking on the next input sequence,
   and update the RPS data files 
        info contains all the information on data files
                and parameters from previously added sequences
        seq is the sequence and PSSM read in from the next data file
        seq_index refers to the (0-based) position of this sequence
                in the complete list of seqences
*/
Int2 RPSAddSequence(RPS_DbInfo *info, 
                    PssmWithParameters *seq, 
                    Int4 seq_index)
{
    Int4 alphabet_size, seq_size;
    Int4 i;
    Int2 status;
    BioseqPtr bsp;
    PssmPtr pssm;
    PssmParametersPtr params;
    FormatRpsDbParametersPtr rps_params;

    pssm = seq->pssm;
    if (pssm == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "No PSSM data present");
        return 1;
    }

    /* Get the dimensions of the PSSM. It's expected that
       these numbers are *not* switched within the scoremat
       if the 'byrow' field is TRUE */

    seq_size = pssm->numColumns;
    alphabet_size = pssm->numRows;

    bsp = (BioseqPtr)pssm->query->data.ptrvalue;
    if (seq_size != bsp->length) {
        ErrPostEx(SEV_ERROR, 0, 0, "Number of PSSM rows does not match "
                             "size of sequence (%d vs %d)", 
                             seq_size, bsp->length);
        return 1;
    }

    if (alphabet_size <= 0) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid alphabet size");
        return 1;
    }

    /* Add RPS parameters if not present */

    params = seq->params;
    if (params == NULL) {
        params = seq->params = PssmParametersNew();
        if (params == NULL) {
            ErrPostEx(SEV_ERROR, 0, 0, 
                        "Cannot manually create PSSM parameters");
            return 1;
        }
    }

    rps_params = params->rpsdbparams;
    if (rps_params == NULL) {
        rps_params = params->rpsdbparams = FormatRpsDbParametersNew();
        if (rps_params == NULL) {
            ErrPostEx(SEV_ERROR, 0, 0, 
                        "Cannot manually create RPS DB parameters");
            return 1;
        }
    }

    /* Fill in PSSM creation parameters if they are not present already */

    if (rps_params->gapOpen == 0)
        rps_params->gapOpen = dump_args[gap_open_arg].intvalue;
    if (rps_params->gapExtend == 0)
        rps_params->gapExtend = dump_args[gap_extend_arg].intvalue;
    if (rps_params->matrixName == NULL)
        rps_params->matrixName = 
                        strdup(dump_args[underlying_matrix_arg].strvalue);

    if (pssm->finalData == NULL) {
        if (pssm->intermediateData == NULL ||
            pssm->intermediateData->freqRatios == NULL) {
            ErrPostEx(SEV_ERROR, 0, 0, "No residue frequency data available");
            return 1;
        }

        /* Residue frequencies are available but a PSSM
           is not; construct the PSSM and its associated Karlin
           values. In the process, scale the PSSM scores.

           The scoremat is updated with the new PSSM and Karlin values */

        pssm->finalData = PssmFinalDataNew();
        if (pssm->finalData == NULL) {
            ErrPostEx(SEV_ERROR, 0, 0, "PSSM score structure not allocated");
            return 1;
        }
        status = RPSUpdateStatistics(seq, seq_size, alphabet_size,
                                dump_args[scale_arg].floatvalue);
        if (status != 0) {
            ErrPostEx(SEV_ERROR, 0, 0, 
                      "PSSM creation failed with error %d", status);
            return 1;
        }
    }

    /* Beyond this point, pssm->finalData and
       pssm->rpsdbparams must both be valid.

       If this is the first sequence processed, fill
       in the info structure with scoremat data. All
       of these parameters must be exactly the same 
       for all sequences in the DB, or else RPS blast
       will not work */

    if (info->alphabet_size == 0) {
        if (RPSAddFirstSequence(info, seq, seq_size, alphabet_size) != 0)
            return 1;
    }
    
    /* Do sanity checking on the input sequence */

    if (alphabet_size != info->alphabet_size) {
        ErrPostEx(SEV_ERROR, 0, 0, "Alphabets do not match");
        return 1;
    }
    if (rps_params->gapOpen != info->gap_open) {
        ErrPostEx(SEV_ERROR, 0, 0, "Gap open penalties do not match");
        return 1;
    }
    if (rps_params->gapExtend != info->gap_extend) {
        ErrPostEx(SEV_ERROR, 0, 0, "Gap extend penalties do not match");
        return 1;
    }
    if (pssm->finalData->scalingFactor != info->scale_factor) {
        ErrPostEx(SEV_ERROR, 0, 0, "Scaling factor does not match");
        return 1;
    }
    if (StringICmp(rps_params->matrixName,
                   dump_args[underlying_matrix_arg].strvalue)) {
        ErrPostEx(SEV_ERROR, 0, 0, "Score matrix does not match: "
                  "input=\"%s\" vs command-line=\"%s\"", 
                  rps_params->matrixName,
                  dump_args[underlying_matrix_arg].strvalue);
        return 1;
    }

    /* Add this sequence' PSSM to the PSSM file. Also keep 
       the PSSM in memory for use with indexing */

    if (RPSUpdatePSSM(info, seq, seq_index, seq_size, alphabet_size) != 0)
        return 1;

    /* index this sequence, add it to the lookup table */

    if (RPSUpdateLookup(info, seq, seq_size) != 0)
        return 1;

    /* Write the sequence size and the Karlin k value to the .aux file */

    fprintf(info->aux_fd, "%d\n", seq_size);
    fprintf(info->aux_fd, "%le\n", pssm->finalData->kappa);

    /* Update the current total length, and free the PSSM
       that was previously allocated. The sequence length
       must include the trailing sentinel */

    info->curr_seq_offset += (seq_size + 1); 
    if (info->posMatrix != NULL) {
        for (i = 0; i < seq_size + 1; i++)
            MemFree(info->posMatrix[i]);
        info->posMatrix = MemFree(info->posMatrix);
    }

    return 0;
}

/* Once all sequences have been processed, perform
   final setup on the BLAST lookup table and finish
   up the RPS files */

void RPS_DbClose(RPS_DbInfo *info)
{
    /* Write the last context offset to the PSSM file. 
       This is the total number of letters for all RPS
       DB sequences combined */

    fseek(info->pssm_fd, sizeof(BlastRPSProfileHeader) + 
                        (info->num_seqs - 1) * sizeof(Int4), SEEK_SET);
    FileWrite(&info->curr_seq_offset, sizeof(Int4), 1, info->pssm_fd);

    /* Pack the lookup table into its compressed form */

    if (_BlastAaLookupFinalize(info->lookup) != 0) {
        ErrPostEx(SEV_WARNING, 0, 0, "Failed to compress lookup table");
    }
    else {
        /* Change the lookup table format to match that
           of the legacy BLAST lookup table */

        BlastRPSLookupFileHeader header;
        BlastLookupTable *lut = info->lookup;
        Int4 i, index; 
        Int4 cursor, old_cursor;
        LookupBackboneCell *cell;
        RPSBackboneCell empty_cell;

        memset(&header, 0, sizeof(header));
        header.magic_number = RPS_MAGIC_NUM;

        /* for each lookup table cell */

        for (index = cursor = 0; index < lut->backbone_size; index++) {
            cell = &lut->thick_backbone[index];

            if (cell->num_used == 0)
                continue;

            /* The cell contains hits */

            if (cell->num_used <= RPS_HITS_PER_CELL) {
                /* if 3 hits or less, just update each hit offset
                   to point to the end of the word rather than
                   the beginning */

                for (i = 0; i < cell->num_used; i++)
                    cell->payload.entries[i] += BLAST_WORDSIZE_PROT - 1;
            }
            else {
                /* if more than 3 hits, pack the first hit into the
                   lookup table cell, pack the overflow array byte 
                   offset into the cell, and compress the resulting
                   'hole' in the overflow array. Update the hit
                   offsets as well */

                old_cursor = cell->payload.overflow_cursor;
                cell->payload.entries[0] = lut->overflow[old_cursor] +
                                        BLAST_WORDSIZE_PROT - 1;
                cell->payload.entries[1] = cursor * sizeof(Int4);
                for (i = 1; i < cell->num_used; i++, cursor++) {
                    lut->overflow[cursor] = lut->overflow[old_cursor + i] +
                                        BLAST_WORDSIZE_PROT - 1;
                }
            }
        }

        header.start_of_backbone = sizeof(header);
        header.end_of_overflow = header.start_of_backbone + 
                   (RPS_NUM_LOOKUP_CELLS + 1) * sizeof(RPSBackboneCell) +
                   cursor * sizeof(Int4);

        /* write the lookup file header */

        FileWrite(&header, sizeof(header), 1, info->lookup_fd);

        /* write the thick backbone */
        
        FileWrite(lut->thick_backbone, sizeof(RPSBackboneCell),
                  lut->backbone_size, info->lookup_fd);

        /* write extra backbone cells */

        memset(&empty_cell, 0, sizeof(empty_cell));
        for (i = lut->backbone_size; i < RPS_NUM_LOOKUP_CELLS + 1; i++) {
            FileWrite(&empty_cell, sizeof(empty_cell), 1, info->lookup_fd);
        }

        /* write the new overflow array */

        FileWrite(lut->overflow, sizeof(Int4), cursor, info->lookup_fd);
    }

    /* Free data, close files */

    info->lookup = LookupTableDestruct(info->lookup);
    BlastQuerySetUpOptionsFree(info->query_options);
    FileClose(info->lookup_fd);
    FileClose(info->pssm_fd);
    FileClose(info->aux_fd);
}

Int2 Main(void) 
{
    FormatDBPtr    fdbp;
    FDB_optionsPtr options;
    FILE *fd;
    char filename[PATH_MAX];
    char buf[256];
    char *open_args;
    char *output_dbname;
    AsnIoPtr ioptr = NULL;
    PssmWithParametersPtr sequence = NULL;
    PssmPtr pssm = NULL;
    SeqEntryPtr sep;
    BioseqPtr bsp;
    BlastDefLinePtr bdp;
    ValNodePtr file_list;
    ValNodePtr tmp_file;
    Int4 num_files;
    Int4 index;
    RPS_DbInfo rps_info;
    
    /* get arguments */
    StringCpy(buf, "formatrpsdb ");
    StringNCat(buf, BlastGetVersionNumber(), sizeof(buf)-StringLen(buf));
    if (!GetArgs(buf, NUMARG, dump_args))
        return 1;

    if (!SeqEntryLoad())
        return 0;
    
    if (!ErrSetLog(dump_args[logfile_arg].strvalue))
        ErrShow();
    else
        ErrSetOpts(ERR_CONTINUE, ERR_LOG_ON);
    UseLocalAsnloadDataAndErrMsg();
    ErrSetMessageLevel(SEV_WARNING);
    
    if (dump_args[output_arg].strvalue)
        output_dbname = dump_args[output_arg].strvalue;
    else
        output_dbname = dump_args[input_arg].strvalue;

    options = FDBOptionsNew(output_dbname,
                            TRUE,
                            dump_args[title_arg].strvalue,
                            FALSE, FALSE, FALSE, FALSE, FALSE, 
                            dump_args[index_arg].intvalue,
                            NULL, NULL,
                            ((Int8)dump_args[dbsize_arg].intvalue)*1000000, 
                            0, FORMATDB_VER, FALSE, eCleanNever);
    if (options == NULL)
        return 1;

    /* Initialize formatdb structure */
    if ((fdbp = FormatDBInit(options)) == NULL)
        return 2;        
    
    if (dump_args[binary_arg].intvalue == TRUE)
        open_args = "rb";
    else
        open_args = "r";

    if((fd = FileOpen(dump_args[input_arg].strvalue, "r")) == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Could not open %s\n", 
                                dump_args[input_arg].strvalue);
        return 3;
    }
          
    /* read in all the file names, and count the number
       of sequences the DB will have */

    file_list = NULL;
    num_files = 0;
    while (fgets(filename, PATH_MAX, fd) != 0) {
        char *tmp = strchr(filename, '\n');
        if (tmp)
            *tmp = 0;
        ValNodeAddPointer(&file_list, 0, StringSave(filename));
        num_files++;
    }
    FileClose(fd);

    /* Initialize the RPS structure */

    RPS_DbInfoInit(&rps_info, num_files, output_dbname);

    /* Walk through each file in the list, recover the
       scoremat from it, add the enclosed Bioseq to the 
       database (formatdb style), then send the rest of
       the scoremat to update the RPS data files. Be
       sure to only work with one scoremat at a time, since
       these structures can be quite large */

    index = 0;
    for (tmp_file = file_list; tmp_file; tmp_file = tmp_file->next) {
        char *tmp = (char *)tmp_file->data.ptrvalue;

        ioptr = AsnIoOpen(tmp, open_args);
        if (ioptr == NULL) {
            ErrPostEx(SEV_ERROR, 0, 0, "file '%s' not found", tmp);
            break;
        }

        sequence = PssmWithParametersAsnRead(ioptr, NULL);
        if (sequence == NULL) {
            ErrPostEx(SEV_ERROR, 0, 0, "no data found in file '%s'", tmp);
            break;
        }

        pssm = sequence->pssm;
        if (pssm == NULL) {
            ErrPostEx(SEV_ERROR, 0, 0, "no score matrix in file '%s'", tmp);
            break;
        }

        sep = pssm->query;
        if (sep == NULL || !IS_Bioseq(sep)) {
            ErrPostEx(SEV_ERROR, 0, 0, "no Bioseq data in file '%s'", tmp);
            break;
        }

        bsp = (BioseqPtr)(sep->data.ptrvalue);
        bdp = FDBGetDefAsnFromBioseq(bsp);
        if (FDBAddBioseq(fdbp, bsp, bdp)) {
            ErrPostEx(SEV_ERROR, 0, 0, "DB update failed for file '%s'", tmp);
            break;
        }

        if (RPSAddSequence(&rps_info, sequence, index)) {
            ErrPostEx(SEV_ERROR, 0, 0, "RPS update failed for file '%s'", tmp);
            break;
        }

        sequence = PssmWithParametersFree(sequence);
        ioptr = AsnIoClose(ioptr);
        index++;
    }
          
    /* clean up scoremat data, if applicable */

    if (sequence)
        sequence = PssmWithParametersFree(sequence);
    if (ioptr)
        ioptr = AsnIoClose(ioptr);

    /* Dump indexes, deallocate structure, arrays, etc. */

    ValNodeFreeData(file_list);

    RPS_DbClose(&rps_info);

    if(FormatDBClose(fdbp))
        return 4;

    FDBOptionsFree(options);

    return 0;
}
