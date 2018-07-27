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

File name: blastconcatdef.h

Author: Karolina Maciag, Aleksandr Morgulis

Contents: type definitions and function prototypes for query 
          multiplexing code.

******************************************************************************/
/* $Revision: 1.3 $ 
*  $Log: blastconcatdef.h,v $
*  Revision 1.3  2004/04/20 14:55:47  morgulis
*  1. Fixed query offsets in results when -B option is used.
*  2. Fixes for lower case masking handling with -B option.
*
*  Revision 1.2  2003/12/29 15:42:46  coulouri
*  tblastn query concatenation fixes from morgulis
*
*  Revision 1.1  2003/03/24 20:47:28  madden
*  Utilities for concatenation of blastn/tblastn queries
*
* */

#ifndef _BLASTCONCATDEF_
#define _BLASTCONCATDEF_

#include <blastconcat.h>

/* AM: MQ_ResultInfo is a structure containing the information about the
       current number of results, best evalue and score for a particular
       query. Those are used to ensure correct printing of results in 
       the case of multiple queries. One such structure is maintained per
       input query. */
typedef struct mq_ResultInfo
{
  Uint4 NumResults;
  BLASTResultHitlistPtr PNTR results;
} MQ_ResultInfo, PNTR MQ_ResultInfoPtr;

/* AM: The structure used to convey subject id and evalue information to
       DivideSeqAligns() when using query multiplexing. */
typedef struct _MQ_ResultInfo
{
  Int4 subject_id;
  Nlm_FloatHi evalue;
} MQ_DivideResultsInfo, PNTR MQ_DivideResultsInfoPtr;

/* AM: This structure holds the pointer to the sarray of seqalign lists
       obtained by distributing seqaligns between queries when query
       multiplexing is used. */
typedef struct sapArrayData
{
  SeqAlignPtr PNTR sap_array;
} SapArrayData, PNTR SapArrayDataPtr;

/*--KM the search->MultQueries structure with information about
  the individual queries when the -B option is used for query 
  concatenation. Needed for the search block*/
typedef struct queries {
    Uint1 NumQueries;
    Int8 TotalLength;    /* AM: Total length of the concatenated query. */
    BspArray FakeBsps;   /* contain SeqIdPtr's */
    SeqLocPtr PNTR LCaseMasks; /* contain lower case masks in queries. */
    IntArray QueryStarts;   /* starts/ends: element for each query */
    IntArray QueryEnds;
    IntArray WhichQuery;    /* "Which" arrays: for each letter in concat seq */
    IntArray WhichPos;

    IntArray EffLengths;  /* AM: Effective lengths of queries */
    IntArray Adjustments; /* AM: Query length adjustments */

    /* AM: Array of effective search spaces that should be used in e-value 
           calculation instead of the search space size derived from the length
	   of the concatenated sequence. */
    FloatArray SearchSpEff;

    /* AM: Array of effective database lengths. */
    Int8Array DbLenEff;

    /* AM: The following are needed for cutoff correction. */
    Int4 MinLen;
    Int4 MinLenEff;
    Int8 MinDbLenEff;
    Nlm_FloatHi MinSearchSpEff;
    Nlm_FloatHi LambdaMin, LambdaMax, LogKMin, LogKMax;

    /* AM: The following are for storing hitlists relevant to a particular query. */
    Uint4 current_query; /* AM: Query currently being processed. */
    Boolean use_mq; /* AM: Tells some functions whether to use query multiplexing. */
    BLAST_HitListPtr PNTR HitListArray; /* Array of pointers to hitlists per query. */

    MQ_ResultInfoPtr result_info; /* Information about results found for each query. */
    Uint4 max_results_per_query;  /* Max number of results to keep for each query. */
    Boolean delete_current_hitlist; /* true, if search->current_hitlist would have
                                       been deleted in BlastSaveCurrentHitlist(). */

    BLAST_Score PNTR dropoff_2nd_pass_array; /* Individual values of dropoff_2nd_pass
                                                parameter per query. */
    Nlm_FloatHi PNTR lambda_array; /* Infividual values of kbp->Lambda per query. */

    SapArrayDataPtr sap_array_data; /* Pointer to the final array of seqalign lists. */

    /* The seqaligns are not here b/c they are not in the original 
       search structure either; they are created and used as they are
       needed in Main.  */ 
} Queries, PNTR QueriesPtr;


typedef struct _PrimaryNode
{
  SeqAlignPtr sap;
  Int4 subject_id;
  Nlm_FloatHi evalue;
  struct _PrimaryNode PNTR next;
} PrimaryNode, PNTR PrimaryNodePtr, PNTR PNTR PrimaryNodePtrArray;

/* ----Prototypes----- */

SeqAlignPtrArray LIBCALL DivideSeqAligns PROTO(( BLAST_OptionsBlkPtr options, SeqAlignPtr sap, 
                                                 QueriesPtr mult_queries, MQ_DivideResultsInfoPtr subjects ));
BioseqPtr LIBCALL BlastMakeFakeBspConcat PROTO((BspArray bsp_arr, Uint1 num_bsps, Boolean is_na, 
                                                Uint4 num_spacers)); /* AM: Added num_spacers parameter */
QueriesPtr LIBCALL BlastMakeMultQueries PROTO((BspArray fbsp_arr, Uint1 num_queries, Boolean is_na, Uint1 num_spacers,
                                               SeqLocPtr PNTR lcase_mask_arr ));
QueriesPtr LIBCALL BlastDuplicateMultQueries PROTO(( QueriesPtr source ));
Uint4 GetQueryNum( QueriesPtr mult_queries, Int4 offset, Int4 end, Int2 frame );
Uint4 LIBCALL GetNumSpacers PROTO(( BLAST_OptionsBlkPtr options,
                                    Boolean believe_query,
				    BspArray fake_bsp_arr ));
void LIBCALL InitHitLists PROTO(( BlastSearchBlkPtr search ));
Int4 LIBCALL ResultIndex1 PROTO(( BLASTResultHitlistPtr ptr,
                                  BLASTResultHitlistPtr PNTR results,
				  Int4 num_elements ));
Int4 LIBCALL ResultIndex PROTO(( Nlm_FloatHi target_e, Int4 target_score, Int4 subject_id,
                                 BLASTResultHitlistPtr PNTR results,
				 Int4 num_elements ));
void LIBCALL MQ_UpdateResultLists PROTO(( QueriesPtr mult_queries ));
SeqLocPtr LIBCALL ConcatSeqLoc PROTO(( QueriesPtr mult_queries, SeqLocPtr loc, SeqIdPtr id, Uint4 qnum ));

#endif

