/* $Id: twoseq_api.h,v 1.2 2004/03/24 19:14:21 papadopo Exp $
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
***************************************************************************/
/**************************************************************************

  File name: twoseq_api.h

  Author: Jason Papadopoulos

  Contents: Functions for C toolkit applications to compare two sequences
                using the rewritten blast engine 

***************************************************************************/

#ifndef _TWOSEQ_API_H_
#define _TWOSEQ_API_H_

#include <ncbi.h>
#include <objseq.h>
#include <tofasta.h>
#include <sqnutils.h>

#define MEGABLAST_CUTOFF 10000

/**
 * The type of blast search to perform. For nucleotide searches,
 * the blastn algorithm is used unless at least one input sequence
 * exceeds MEGABLAST_CUTOFF bases in size. In that case, megablast
 * is used instead. If the blast_hint is eSensitive, discontiguous
 * megablast with word size 12 is used.
 */
enum blast_type {
    eChoose = 0,        /**< blastn for nuc. sequences, blastp otherwise */
    eBlastn = 1,        /**< blastn or megablast (determined automatically) */
    eBlastp = 2         /**< blastp search on two protein sequences */
};

/**
 * Provide a hint on how the search is to be set up. At
 * present this only applies to nucleotide searches
 */
enum blast_hint {
    eSensitive = 0,     /**< trade off speed for sensitivity */
    eFast = 1           /**< trade off sensitivity for speed */
};

/**
  * The main user-visible setup structure for the API. This
  * only makes a (small) subset of the complete options available
  */
typedef struct {
    enum blast_hint hint;       /**< for nucleotide searches, how should
                                     the search be set up? 
                                     Default = eSensitive */
    enum blast_type program;    /**< the BLAST program to use.
                                     Default = eChoose */
    char strand;                /**< For nucleotide searches, the strand
                                     of the first sequence to check: 
                                     choices are Seq_strand_{plus|minus|both}
                                     Default is Seq_strand_both */
    double cutoff_evalue;       /**< Alignments whose E value is larger than
                                     this number are discarded. Default 10.0 */
    char* matrix;               /**< The scoring matrix to use (protein
                                     searches only). NULL means "BLOSUM62".
                                     Default is NULL */
    char* filter_string;        /**< Specifies filtering to apply to the
                                     first of the two input sequences. 
                                     NULL or "T" implies DUST/SEG, "F"
                                     turns off filtering. Default = NULL */
    Int4 word_size;             /**< The word size to use. 0 chooses the
                                     default for the specified program
                                     (i.e. 3 for blastp, 11 for blastn, 
                                     28 for blastn with large sequences).
                                     Default = 0 */
    Boolean gapped_calculation; /**< Perform gapped alignments. Default = TRUE*/
    Int4 nucleotide_match;      /**< For nucleotide searches, the reward
                                     for matching letters (default 1) */
    Int4 nucleotide_mismatch;   /**< For nucleotide searches, the penalty
                                     for mismatching letters (default -3) */
} BLAST_SummaryOptions;

/**
  * Allocate storage for an API setup structure and set the
  * default options for it.
  *
  * @param options pointer to be updated with newly allocated structure [out]
  * @return 0 for successful allocation, -1 otherwise
  */
Int2 BLAST_SummaryOptionsInit(BLAST_SummaryOptions **options);

/**
  * Free the storage previously allocated for an API setup structure
  *
  * @param options pointer tothe structure to be freed [in]
  * @return always NULL
  */
BLAST_SummaryOptions* BLAST_SummaryOptionsFree(BLAST_SummaryOptions *options);

/**
  * Perform a BLAST search on the two input sequences and return
  * the list of alignments the search generates
  * @param options structure describing how the search will be configured [in]
  * @param bsp1 the first sequence to be compared. Filtering and selection
  *             of nucleotide strand apply only to this sequence [in]
  * @param bsp2 the second sequence to be compared [in]
  * @param seqalign_out the list of alignments generated by the search.
  *             If search failed or no alignments were found, set to NULL [out]
  * @return 0 for a successful search, nonzero if search failed
  */
Int2 BLAST_TwoSequencesSearch(const BLAST_SummaryOptions *options,
                              Bioseq *bsp1, 
                              Bioseq *bsp2,
                              SeqAlign **seqalign_out);

#endif  /* !_TWOSEQ_API_H_ */
