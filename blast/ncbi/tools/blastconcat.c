static char const rcsid[] = "$Id: blastconcat.c,v 1.10 2005/01/10 18:52:29 coulouri Exp $";

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

File name: blastconcat.c

Author: Karolina Maciag, Aleksandr Morgulis

Contents: implementation of functions needed for query multiplexing 
          functionality.

******************************************************************************/
/* $Revision: 1.10 $ 
*  $Log: blastconcat.c,v $
*  Revision 1.10  2005/01/10 18:52:29  coulouri
*  fixes from morgulis to allow concatenation of >255 queries in [t]blastn
*
*  Revision 1.9  2004/04/21 19:25:04  coulouri
*  do not cast lvalues
*
*  Revision 1.8  2004/04/21 13:54:23  camacho
*  Remove unused variable
*
*  Revision 1.7  2004/04/20 14:55:47  morgulis
*  1. Fixed query offsets in results when -B option is used.
*  2. Fixes for lower case masking handling with -B option.
*
*  Revision 1.6  2004/02/26 15:52:29  papadopo
*  Mike Gertz' modifications to unify handling of gapped Karlin blocks between protein and nucleotide searches
*
*  Revision 1.5  2003/12/29 15:42:46  coulouri
*  tblastn query concatenation fixes from morgulis
*
*  Revision 1.4  2003/05/30 17:25:36  coulouri
*  add rcsid
*
*  Revision 1.3  2003/03/25 15:02:11  madden
*  Code cleanup from A. Morgulis
*
*  Revision 1.2  2003/03/24 21:16:03  madden
*  Cast before calling StrCat
*
*  Revision 1.1  2003/03/24 20:47:28  madden
*  Utilities for concatenation of blastn/tblastn queries
*
* */

#include <ncbi.h>
#include <objalign.h>
#include <objseq.h>

#ifndef _BLASTCONCAT_
#include <blastconcat.h>
#endif

#include <blastpri.h>
#include <blastdef.h>
#include <blastconcatdef.h>

/* AM: The following function returns the minimal number of spacers necessary to
       create the concatenated query.

       Parameters:

         options	The option block created based on blast command line arguments.
	 believe_query	Flag indicating whether to believe the original bsp;
         fake_bsp_arr	The array of queries to be concatenated
*/
Uint4 LIBCALL GetNumSpacers PROTO(( BLAST_OptionsBlkPtr options, Boolean believe_query,
                      BspArray fake_bsp_arr ))
{
  Int4               dropoff_1st_pass_bits,	/* Dropoff ("X") used for the first pass (bits) */
                     dropoff_2nd_pass_bits,     /* Dropoff ("X") used for the second pass (bits ) */
                     gap_x_dropoff_bits,	/* Dropoff ("X") used for gapped alignments (bits) */
                     gap_x_dropoff_final_bits,  /* Dropoff ("X") used for final gapped alignment (bits) */
		     query_length,		/* The length of the first query in fake_bsp_arr. */
		     query_loc_start,           /* Index of the start of the query sequence */
		     index,
		     i;
  BLAST_Score        dropoff_1st_pass, 		/* Dropoff ("X") used for the first pass */
                     dropoff_2nd_pass,          /* Dropoff ("X") used for the second pass */
                     gap_x_dropoff,  		/* Dropoff ("X") used for gapped alignments */
	             gap_x_dropoff_final,      	/* Dropoff ("X") used for final gapped alignment */
	             X_score = 0,		/* The score of the ambiguous match */
	             max_dropoff;       	/* max of dropoff_1st_pass, dropoff_2nd_pass, gap_x_dropoff, gap_x_dropoff_final */
  Nlm_FloatHi        Lambda;			/* Lambda parameter (used here for converting scores from bit scores) */
  BLAST_KarlinBlkPtr kbp,			/* Pointer to blast parameter block (contains Lambda)*/
	             kbp_gap;
  SeqLocPtr          query_slp,			/* Sequence location corresponding to the whole fake_bsp_arr[0] */
                     private_slp;		/* Sequence location constructed based on the query strand */
  BioseqPtr	     fake_bsp, 			/* Constructed if the original query can not be trusted */
                     query_bsp;			/* == fake_bsp_arr[0] */
  Int2		     first_context,		/* First context of the sequence */
                     last_context,		/* Last context of the sequence */
		     status;
  Uint1		     alphabet,			/* Sequence alphabet identifier */
                     strand,			/* Sequence strands (taken from options) */
		     residue;			/* Temporary used as an iterator in the loop over the letters in the sequence */
  BLAST_ScoreBlkPtr  sbp;			/* Score block constructed from the sequence */
  SeqPortPtr	     spp = NULL;		/* Sequence port for reading sequence content */
  Uint1Ptr	     query_seq = NULL,		/* Points to the sequence data */
                     query_seq_start = NULL;    /* Points to the actual start of the sequence data */
  BLAST_ScorePtr     PNTR matrix;		/* Blast parameter matrix (contained in sbp) */
  ValNodePtr         error_return = NULL;

  query_bsp = fake_bsp_arr[0];

  if( believe_query ) fake_bsp = query_bsp;
  else fake_bsp = BlastMakeFakeBioseq( query_bsp, NULL );

  query_slp = NULL;
  ValNodeAddPointer( &query_slp, SEQLOC_WHOLE, 
                     SeqIdDup( SeqIdFindBest( fake_bsp->id, SEQID_GI )));

  BlastGetFirstAndLastContext( options->program_name, query_slp,
                               &first_context, &last_context,
			       options->strand_option );

  query_length = SeqLocLen( query_slp );

  if( StringCmp( options->program_name, "blastn" ) == 0 )
    alphabet = BLASTNA_SEQ_CODE;
  else alphabet = Seq_code_ncbistdaa;

  sbp = BLAST_ScoreBlkNew( alphabet, last_context + 1 );

  if( StringCmp( options->program_name, "blastn" ) != 0 )
  {
    sbp->read_in_matrix = TRUE;
    BlastScoreSetAmbigRes( sbp, 'X' );
  }
  else
  {
    if( options->matrix != NULL && *(options->matrix) != NULLB )
      sbp->read_in_matrix = TRUE;
    else sbp->read_in_matrix = FALSE;

    BlastScoreSetAmbigRes( sbp, 'N' );
  }

  sbp->penalty = options->penalty;
  sbp->reward = options->reward;
  sbp->query_length = query_length;

  if( options->matrix != NULL )
    status = BlastScoreBlkMatFill( sbp, options->matrix );
  else status = BlastScoreBlkMatFill( sbp, "BLOSUM62" );

  if( status != 0 )
  {
    ErrPostEx( SEV_WARNING, 0, 0, 
               "BlastScoreBlkMatFill returned non-zero status" );
  }

  query_loc_start = SeqLocStart( query_slp );
  strand = SeqLocStrand( query_slp );

  if( strand == Seq_strand_unknown 
      || strand == Seq_strand_plus 
      || strand == Seq_strand_both )
    private_slp = SeqLocIntNew( query_loc_start, SeqLocStop( query_slp ),
                                Seq_strand_plus, SeqLocId( query_slp ) );
  else
    private_slp = SeqLocIntNew( query_loc_start, SeqLocStop( query_slp ),
                                Seq_strand_minus, SeqLocId( query_slp ) );
  
  if( StringCmp( options->program_name, "tblastn" ) == 0 )
  {
    spp = SeqPortNewByLoc( private_slp, Seq_code_ncbistdaa );
    SeqPortSet_do_virtual( spp, TRUE );
  }
  else 
  {
      spp = SeqPortNewByLoc( private_slp, Seq_code_ncbi4na );
      SeqPortSet_do_virtual( spp, TRUE );
  }

  if( spp )
  {
    query_seq_start = (Uint1Ptr)MemNew( 2*(query_length + 2)*sizeof( Char ) );
    query_seq_start[0] = NULLB;
    query_seq = query_seq_start + 1;
    index = 0;

    while( (residue = SeqPortGetResidue( spp )) != SEQPORT_EOF )
      if( IS_residue( residue ) )
      {
        if( residue == 24 ) residue = 21;

        query_seq[index] = residue;
	++index;
      }

    query_seq[index] = NULLB;
    spp = SeqPortFree( spp );

    if( StringCmp( options->program_name, "blastn" ) == 0 )
      for( index = 0; index <= query_length + 1; ++index )
        query_seq_start[index] = ncbi4na_to_blastna[query_seq_start[index]];
  }


  if( StringCmp( options->program_name, "blastn" ) == 0 )
  {
    if( first_context == 0 )
      BlastScoreBlkFill( sbp, (CharPtr)query_seq_start, query_length, 0 );

    if( last_context == 1 )
      BlastScoreBlkFill( sbp, (CharPtr)query_seq_start, query_length, 1 );
    /* EMG: kbp and kbp_gap both appear to be unused in this routine */
    sbp->kbp_gap = sbp->kbp_gap_std;
    sbp->kbp = sbp->kbp_std;
    kbp     = sbp->kbp[first_context];
    kbp_gap = sbp->kbp[first_context];
    Lambda = kbp->Lambda;
  }
  else /* tblastn */
  {
    BlastScoreBlkFill( sbp, (CharPtr)query_seq_start, query_length, 0 );
    sbp->kbp_gap_std[0] = BlastKarlinBlkCreate();
    BlastKarlinBlkGappedCalcEx( sbp->kbp_gap_std[0], options->gap_open,
                                options->gap_extend, options->decline_align,
				sbp->name, &error_return );
    sbp->kbp_gap = sbp->kbp_gap_std;
    sbp->kbp = sbp->kbp_std;
    kbp_gap = sbp->kbp_gap[first_context];
    Lambda = kbp_gap->Lambda;
  }

  matrix = sbp->matrix;

  if( StringCmp( options->program_name, "blastn" ) == 0 )
    X_score = abs(-matrix[0][sbp->ambiguous_res[0]]);
  else /* tblastn */
  {
    X_score = INT4_MAX;

    for( i = 1; i < sbp->mat_dim2; ++i )
      if( matrix[i][sbp->ambiguous_res[0]] 
          && abs( matrix[i][sbp->ambiguous_res[0]] ) < X_score )
        X_score = abs( matrix[i][sbp->ambiguous_res[0]] );
  }

  dropoff_1st_pass_bits = options->dropoff_1st_pass;
  dropoff_2nd_pass_bits = options->dropoff_2nd_pass;
  gap_x_dropoff_bits = options->gap_x_dropoff;
  gap_x_dropoff_final_bits = options->gap_x_dropoff_final;

  if( !dropoff_1st_pass_bits )
    dropoff_1st_pass_bits = (Int4)DROPOFF_NUMBER_OF_BITS;

  if( !dropoff_2nd_pass_bits )
    dropoff_2nd_pass_bits = (Int4)DROPOFF_NUMBER_OF_BITS;

  gap_x_dropoff = (BLAST_Score)(gap_x_dropoff_bits*NCBIMATH_LN2/Lambda);
  gap_x_dropoff_final 
    = (BLAST_Score)(gap_x_dropoff_final_bits*NCBIMATH_LN2/Lambda);
  gap_x_dropoff_final = MAX( gap_x_dropoff_final, gap_x_dropoff );
  dropoff_1st_pass 
    = (BLAST_Score)ceil( (Nlm_FloatHi)dropoff_1st_pass_bits*NCBIMATH_LN2
                                     /Lambda );
  dropoff_2nd_pass 
    = (BLAST_Score)ceil( (Nlm_FloatHi)dropoff_2nd_pass_bits*NCBIMATH_LN2
                                     /Lambda );

  max_dropoff = MAX( gap_x_dropoff_final, gap_x_dropoff );
  
  if( max_dropoff < dropoff_1st_pass ) max_dropoff = dropoff_1st_pass;

  if( max_dropoff < dropoff_2nd_pass ) max_dropoff = dropoff_2nd_pass;

  SeqLocFree( private_slp );
  BLAST_ScoreBlkDestruct( sbp );
  SeqLocFree( query_slp );
  SeqMgrDeleteFromBioseqIndex( fake_bsp );
  return (Uint4)ceil( ((Nlm_FloatHi)max_dropoff*1.5)/X_score );
}

/* AM: This functions returns the number of the original query given an
       offset and frame into the concatenated query. */
Uint4 GetQueryNum( QueriesPtr mult_queries, Int4 offset, Int4 end, Int2 frame )
{
  Int4 tmp_offset = 0;
  Int4 tmp_end = 0;

  if( frame == 1 )
    if( offset > mult_queries->TotalLength ) 
    {
      tmp_offset = 2*mult_queries->TotalLength + 1 - offset;
      tmp_end = 2*mult_queries->TotalLength + 1 - end;
    }
    else 
    {
      tmp_offset = offset;
      tmp_end = end;
    }
  else if( frame == -1 ) 
  {
    tmp_offset = mult_queries->TotalLength - offset;
    tmp_end = mult_queries->TotalLength - end;
  }
  else if( frame == 0 )
  {
    tmp_offset = offset;
    tmp_end = end;
  }

  while( tmp_offset != tmp_end
         && (mult_queries->WhichQuery[tmp_offset] <= 0 
	     || mult_queries->WhichQuery[tmp_offset] 
	        > mult_queries->NumQueries) ) 
    if( tmp_offset > tmp_end )
      --tmp_offset;
    else ++tmp_offset;

  return mult_queries->WhichQuery[tmp_offset] - 1;
}

/* AM: Comparison routine for SeqAlignPtr used by DivideSeqAligns. */
int LIBCALL CompareSeqAlignPtr PROTO(( Nlm_VoidPtr afirst, Nlm_VoidPtr asecond ))
{
  Nlm_FloatHi bit_score1, evalue1, bit_score2, evalue2;
  Int4 number1, score1, number2, score2;
  PrimaryNodePtr PNTR first_node = (PrimaryNodePtr PNTR) afirst;
  PrimaryNodePtr PNTR second_node = (PrimaryNodePtr PNTR) asecond;
  SeqAlignPtr first = (*first_node)->sap;
  SeqAlignPtr second = (*second_node)->sap;
  GetScoreAndEvalue( first, &score1, &bit_score1, &evalue1, &number1 );
  GetScoreAndEvalue( second, &score2, &bit_score2, &evalue2, &number2 );

  if( (*first_node)->evalue > (*second_node)->evalue ) return -1;
  else if( (*first_node)->evalue < (*second_node)->evalue ) return 1;
  else if( score1 > score2 ) return 1;
  else if( score1 < score2 ) return -1;
  else if( (*first_node)->subject_id > (*second_node)->subject_id ) return 1;
  else if( (*first_node)->subject_id < (*second_node)->subject_id ) return -1;
  else return 0;
}

/* AM: This function divides a list of alignments for the concatenated query into the 
       array of lists of alignments per each original query.

       params: 

       options		    --- program options (used to determine which program is being run).
       sap                  --- points to the head of the list of alignments for concatenated query.
       mult_queries         --- points to the structure containing information about individual 
	                        queries.
       subjects		    --- array of structures containing information of subject ids and 
                                evalues of seqaligns. This is necessary to ensure correct order
				of results.

       return value         --- array of pointers to the heads of the lists of alignments: one per
                                every original query.
*/
SeqAlignPtrArray LIBCALL DivideSeqAligns PROTO(( BLAST_OptionsBlkPtr options,
                                                 SeqAlignPtr sap, 
                                                 QueriesPtr mult_queries,
						 MQ_DivideResultsInfoPtr subjects ))
{
  typedef struct _SecondaryNode
  {
    SeqAlignPtr sap;			/* SeqAlign asociated with this node */
    SeqAlignPtr primary;		/* The primary node asociated with this node */
    struct _SecondaryNode PNTR next;	/* Next element in the list */
  } SecondaryNode, PNTR SecondaryNodePtr, PNTR PNTR SecondaryNodePtrArray;

  SeqAlignPtrArray 	result = NULL;				/* The resulting array of seqalign lists. Returned by the function */
  SeqAlignPtr 		head = NULL, 
   			next_sap = NULL;
  SecondaryNodePtrArray secondary = NULL;			/* The array of secondary seqalign lists */
  SecondaryNodePtr 	secondary_node_pool = NULL;		/* The set of free secondary nodes */
  SecondaryNodePtr 	next_free_secondary_node = NULL, 
  			new_secondary_node;
  PrimaryNodePtrArray   primary = NULL;				/* The array of primary seqalign lists */
  PrimaryNodePtrArray   primaries_per_query;			/* Array for heap sort. */
  PrimaryNodePtr 	primary_node_pool = NULL;		/* The set of free primary nodes */
  PrimaryNodePtr 	next_free_primary_node = NULL, 
  			new_primary_node;
  IntArray 		lengths;				/* Array of sized of secondary node lists */
  Uint1Array 		new_primary;
  Uint4 		num_queries = 0, 			/* Total number of original queries */
 			num_seqaligns = 0, 			/* Number of seqaligns in the input list */
			start = 0, 
			query = 0;
  Uint4 		count = 0, 
  			count1 = 0, 
			primary_array_size = 0;			/* The maximum size of a primary node list */
  SeqIdPtr 		subject_id = NULL, 			/* The id of the "current" subject sequence */
  			last_id = NULL;				/* Saved id of the previous subject sequence. */
  SeqLocPtr		loc = NULL;				/* Sequence location if segs is of type StdSeg */

  num_queries = mult_queries->NumQueries;

  /* Count the seqaligns in the input list */
  for( head = sap; head; head = head->next ) ++num_seqaligns;
  
  /* Memory allocation */
  result = (SeqAlignPtrArray)MemNew( sizeof( SeqAlignPtr )*num_queries );
  primary = (PrimaryNodePtrArray)MemNew( sizeof( PrimaryNodePtr )*num_queries );
  primary_node_pool = (PrimaryNodePtr)MemNew( sizeof( PrimaryNode )*num_seqaligns );
  next_free_primary_node = primary_node_pool;
  secondary = (SecondaryNodePtrArray)MemNew( sizeof( SecondaryNodePtr )*num_queries );
  secondary_node_pool = (SecondaryNodePtr)MemNew( sizeof( SecondaryNode )*num_seqaligns );
  next_free_secondary_node = secondary_node_pool;
  lengths = (IntArray)MemNew( sizeof( Int4 )*num_queries );
  new_primary = (Uint1Array)MemNew( sizeof( Uint1 )*num_queries );

  /* Primary and secondary lists creation. */
  for( count = 0; sap; sap = next_sap )
  {
    if( StringCmp( options->program_name, "blastn" ) == 0 )
      start = *((DenseSegPtr)(sap->segs))->starts;
    else
    {
      loc = ((StdSegPtr)(sap->segs))->loc;
      start = SeqLocStart( loc );
    }

    while( mult_queries->WhichQuery[start] <= 0
           || mult_queries->WhichQuery[start] > num_queries )
      ++start;

    query = mult_queries->WhichQuery[start] - 1;
    next_sap = sap->next;
    subject_id = TxGetSubjectIdFromSeqAlign( sap );

    if( !last_id || SeqIdComp( subject_id, last_id ) != SIC_YES )
      MemFill( (void *)new_primary, 0, sizeof( Uint1 )*num_queries );

    last_id = subject_id;

    if( !new_primary[query] )
    {
      ++lengths[query];
      new_primary[query] = 1;
      new_primary_node = next_free_primary_node++;
      new_primary_node->sap = sap;
      new_primary_node->evalue = subjects[count].evalue;
      new_primary_node->subject_id = subjects[count++].subject_id;
      new_primary_node->next = primary[query];
      primary[query] = new_primary_node;
    }
    else
    {
      new_secondary_node = next_free_secondary_node++;
      new_secondary_node->sap = sap;
      new_secondary_node->primary = primary[query]->sap;
      new_secondary_node->next = secondary[query];
      secondary[query] = new_secondary_node;
    }
  }

  /* Allocate primary array for sorting. */
  for( count = 0; count < num_queries; ++count )
    if( lengths[count] > primary_array_size )
      primary_array_size = lengths[count];

  /* primary = (SortPairNodePtr)MemNew( sizeof( SortPairNode )*primary_array_size ); */
  primaries_per_query = (PrimaryNodePtrArray)MemNew( 
    sizeof( PrimaryNodePtr )*primary_array_size );

  /* Sort the primary arrays, then merge primary and secondary nodes 
     for each query.*/
  for( count = 0; count < num_queries; ++count )
  {
    for( count1 = 0, new_primary_node = primary[count]; new_primary_node; 
         new_primary_node = new_primary_node->next, ++count1 )
      primaries_per_query[count1] = new_primary_node;

    result[count] = NULL;
    Nlm_HeapSort( (Nlm_VoidPtr)primaries_per_query, count1, 
                  sizeof( PrimaryNodePtr ), CompareSeqAlignPtr );

    for( count1 = 0; count1 < lengths[count]; ++count1 )
    {
      primaries_per_query[count1]->sap->next = result[count];
      result[count] = primaries_per_query[count1]->sap;
    }

    for( new_secondary_node = secondary[count]; new_secondary_node;
         new_secondary_node = new_secondary_node->next )
    {
      new_secondary_node->sap->next = new_secondary_node->primary->next;
      new_secondary_node->primary->next = new_secondary_node->sap;
    }

    MemFill( (void *)primaries_per_query, 0, sizeof( SeqAlignPtr )*primary_array_size );
  }

  /* Cleanup */
  MemFree( (void *)primary );
  MemFree( (void *)primary_node_pool );
  MemFree( (void *)primaries_per_query );
  MemFree( (void *)new_primary );
  MemFree( (void *)lengths );
  MemFree( (void *)secondary_node_pool );
  MemFree( (void *)secondary );

  /* Now correct offsets in result. */
  for( count = 0; count < num_queries; ++count )
    if( result[count] )
    {
      sap = result[count];

      while( sap )
      {
	SeqIdPtr tmpid = NULL;

        if( StringCmp( options->program_name, "blastn" ) == 0 )
        {
	  Uint4 i = 0;
	  DenseSegPtr dsp = (DenseSegPtr)(sap->segs);

	  for( ; i < (dsp->dim)*(dsp->numseg); i += dsp->dim )
	  {
            start = dsp->starts[i];
	    query = mult_queries->WhichQuery[start + 1] - 1;
	    
	    if( start != -1 )
	      start = start - mult_queries->QueryStarts[query];

            dsp->starts[i] = start;
	  }

	  tmpid = SeqIdDup( mult_queries->FakeBsps[query]->id );
	  tmpid->next = dsp->ids->next;
	  dsp->ids = tmpid;
        }
        else
        {
	  Uint4 end = 0, len;
	  SeqLocPtr loc, newloc = NULL;
	  StdSegPtr ssp = (StdSegPtr)(sap->segs);
	  StdSegPtr tmpssp = ssp;
	  SeqIdPtr newid = NULL;

	  while( tmpssp )
	  {
	    if( ssp->loc->choice != SEQLOC_EMPTY )
	    {
	      loc = ssp->loc;
	      start = SeqLocStart( loc );
	      query = mult_queries->WhichQuery[start + 1] - 1;
	      tmpid = SeqIdDup( mult_queries->FakeBsps[query]->id );
	      break;
	    }

	    tmpssp = tmpssp->next;
	  }

	  while( ssp )
	  {
	    loc = ssp->loc;
	    start = SeqLocStart( loc );
	    
	    if( start != -1 )
	    {
	      end = SeqLocStop( loc );
	      len = end - start;
	      start = start - mult_queries->QueryStarts[query];
	      end = start + len;
	      newloc = SeqLocIntNew( start, end, SeqLocStrand( loc ), tmpid );
	    }
	    else
	    {
	      newloc = ValNodeNew( NULL );
	      newloc->choice = SEQLOC_EMPTY;
	      newloc->data.ptrvalue = SeqIdDup( tmpid );
	    }

	    newid = SeqIdDup( tmpid );
	    newid->next = ssp->ids->next;
	    ssp->ids = newid; 
	    newloc->next = loc->next;
	    ssp->loc = newloc;
	    SeqLocFree( loc );
	    ssp = ssp->next;
	  }
        }

	sap = sap->next;
      }
    }

  return result;
}

/* BlastMakeFakeBspConcat
   Given an array of fake_bsps, this function creates a new fake_bsp. The data
   field of the new bsp is constructed using the concatenated sequences of the 
   bsps in the array, in the same order as they appear in the array.
   Protein sequences are concatenated directly.
   Nucleotide sequences are encoded with two per byte, so they are concatenated
   byte-wise.
   Parameters: bsp_arr, the array of fake_bsps from which sequences are to be pulled
               num_bsps, the maximum number of bsps in the array to use
               is_na: true if query is nucleotide, false if amino acid seq
               num_spacers: the number of spacer characters between the original
                            sequences in the concatenated query. (AM:)
   note: assumes that the array bsp_arr has at least num_bsp elements! 

   AM: Changes to align on the byte boundaries.
*/
BioseqPtr LIBCALL 
BlastMakeFakeBspConcat PROTO((BspArray bsp_arr, Uint4 num_bsps, Boolean is_na, Uint4 num_spacers)){
	BioseqPtr tot;
	Int4 bsp_iter, letter_iter;
		/* "compact" len refers to #bytes to store the seq, not to #of letters */
	Int4 tot_len, tot_compact_len, spacer_compact_len;
	Int4 indiv_len;		/*keep an Int4 to satisfy BlastGetSequenceFromBioseq */
	Uint4 total_written;
	BioseqPtr curr_bsp;
	Uint1Ptr indiv_seq = NULL, concat_seq;
	ByteStorePtr byte_store;
        Uint1 PNTR spacer_seq_nuc, PNTR spacer_seq_prot;
	SeqLocPtr slp = NULL;
	SeqPortPtr spp = NULL;
	Uint1 residue, shift = 0;
	Int4 index = 0;
	Uint1 newcode = Seq_code_ncbi4na, oldcode;
        ByteStorePtr from = NULL, 
	             to = NULL;

        /* AM: Determine the size and allocate spacer arrays. */
        if( is_na )
	{
	  num_spacers = (num_spacers + 1)/2;

          /* Recode all BSPs to NCBI4na */
	  for( bsp_iter = 0; bsp_iter < num_bsps; ++bsp_iter ) 
	  {
	    oldcode = bsp_arr[bsp_iter]->seq_data_type;
	    from = bsp_arr[bsp_iter]->seq_data;

	    if( !(to = BSConvertSeq( from, newcode, oldcode, 
	                             bsp_arr[bsp_iter]->length )) )
              return NULL;

            bsp_arr[bsp_iter]->seq_data_type = newcode;
            bsp_arr[bsp_iter]->seq_data = to;
	  }
        }

        spacer_seq_nuc = (Uint1 PNTR)MemNew( sizeof( Uint1 )*(num_spacers + 1) );
	spacer_seq_prot = spacer_seq_nuc;

	/* create the spacer sequences to insert between sequences */
	spacer_compact_len = num_spacers;
        
	for( letter_iter = 0; letter_iter < spacer_compact_len; ++letter_iter )
	  if( is_na ) spacer_seq_nuc[letter_iter] = TWO_N;
	  else spacer_seq_prot[letter_iter] = 'X';

        spacer_seq_nuc[letter_iter] = 0;
	tot = BioseqNew();

	if (tot == NULL)
		return NULL;		

	/* initialize the fake_bsp with fields of the first bsp in the array */
	tot = (BioseqPtr) BlastMakeFakeBioseq( (BioseqPtr) *bsp_arr, NULL);

	/* find total length of letters */
	tot_len = 0;

	for( bsp_iter=0; bsp_iter<num_bsps; bsp_iter++ ) 
	{
  	  curr_bsp = *(bsp_arr+bsp_iter);
	  tot_len += curr_bsp->length;

	  if( is_na && bsp_iter != num_bsps - 1 && curr_bsp->length%2 ) ++tot_len;
	}

	/* compact length indicates actual number of bytes in representation */
	tot_compact_len = (is_na) ? (tot_len + 1)/2 : tot_len;
	tot_compact_len += spacer_compact_len * (num_bsps-1);

	/* total len is in terms of number of letters */
	tot_len += ( (is_na) ? num_spacers*2 : num_spacers) * (num_bsps-1); 
	
	concat_seq = MemNew((1 + tot_compact_len) * sizeof(Uint1));
	concat_seq[0] = 0;

	if (concat_seq == NULL)
		return NULL;

        total_written = 0;
	
        for( bsp_iter = 0; bsp_iter < num_bsps; ++bsp_iter )
	{
	  curr_bsp = bsp_arr[bsp_iter];
	  indiv_len = curr_bsp->length;

	  if( is_na )
	  {
	    /* indiv_seq = (Uint1Ptr)(bsp_arr[bsp_iter]->seq_data->chain->str); */
	    indiv_len = (indiv_len + 1)/2;
	    indiv_seq = MemNew( (1 + indiv_len)*sizeof( Uint1 ) );
	    
	    if( !indiv_seq ) return NULL;

            slp = NULL;
	    index = 0;
	    shift = 0;
	    spp = SeqPortNew( bsp_arr[bsp_iter], 0, -1, Seq_strand_plus, Seq_code_ncbi4na );

	    while( (residue = SeqPortGetResidue( spp )) != SEQPORT_EOF )
	    {
	      if( shift )
	      {
	        indiv_seq[index/2] = ((indiv_seq[index/2])&0xF0);
	        residue = (residue&0xF);
		indiv_seq[index/2] |= residue;
	      }
	      else
	      {
	        indiv_seq[index/2] = 0;
	        residue = (residue << 4);
		indiv_seq[index/2] = residue;
	      }

	      shift = 1 - shift;
	      ++index;
	    }

            indiv_seq[indiv_len] = 0;
	    spp = SeqPortFree( spp );

	    if( bsp_iter )
	    {
	      StrCat( (CharPtr) concat_seq, (CharPtr) spacer_seq_nuc );
	      total_written += num_spacers;
            }

	    StrCat( (CharPtr) concat_seq, (CharPtr) indiv_seq );
	    total_written += indiv_len;

	    if( curr_bsp->length%2 )
	    {
	      concat_seq[total_written - 1] = concat_seq[total_written - 1]&0xF0;
	      concat_seq[total_written - 1] = concat_seq[total_written - 1]|LAST_N;
	    }

	    indiv_seq = MemFree( indiv_seq );
	  }
	  else
	  {
 	    indiv_seq = (Uint1Ptr)BlastGetSequenceFromBioseq( bsp_arr[bsp_iter], &indiv_len );
	    
	    if( bsp_iter ) StrCat( (CharPtr) concat_seq, (CharPtr) spacer_seq_prot );

	    StrCat( (CharPtr) concat_seq, (CharPtr) indiv_seq );
	  }
	}  

	byte_store = BSNew(0);
	BSWrite(byte_store, (VoidPtr) concat_seq, tot_compact_len);

	tot->seq_data = byte_store;
	tot->length = tot_len;	
	
        MemFree( (void *)spacer_seq_nuc );
	return (tot);
}

/* Duplicates Queries structure for use with multiple threads. */
QueriesPtr LIBCALL BlastDuplicateMultQueries PROTO(( QueriesPtr source ))
{
  QueriesPtr result = NULL;
  result = (QueriesPtr) MemNew (sizeof (Queries) );
  
  if( !result ) return NULL;

  result->NumQueries = source->NumQueries;
  result->TotalLength = source->TotalLength;
  result->FakeBsps = source->FakeBsps;
  result->LCaseMasks = source->LCaseMasks;
  result->QueryStarts = source->QueryStarts;
  result->QueryEnds = source->QueryEnds;
  result->WhichQuery = source->WhichQuery;
  result->WhichPos = source->WhichPos;
  result->EffLengths = source->EffLengths;
  result->Adjustments = source->Adjustments;
  result->SearchSpEff = source->SearchSpEff;
  result->DbLenEff = source->DbLenEff;
  result->MinLen = source->MinLen;
  result->MinLenEff = source->MinLenEff;
  result->MinDbLenEff = source->MinDbLenEff;
  result->MinSearchSpEff = source->MinSearchSpEff;
  result->LambdaMin = source->LambdaMin;
  result->LambdaMax = source->LambdaMax;
  result->LogKMin = source->LogKMin;
  result->LogKMax = source->LogKMax;
  result->HitListArray = NULL;
  result->result_info = source->result_info;
  result->max_results_per_query = source->max_results_per_query;
  result->dropoff_2nd_pass_array = source->dropoff_2nd_pass_array;
  result->lambda_array = source->lambda_array;
  result->sap_array_data = source->sap_array_data;
  return result;
}

/* BlastMakeMultQueries
   Given an array of fake_bsps corresponding to individual BLAST queries, returns
   a pointer to a Queries structure containing information about the individual 
   queries. 
   Parameters: bsp_arr, the array of fake_bsps
               num_queries, the maximum number of fake_bsps from the array to process 
   note: assumes that bsp_arr has minimum num_queries elements
         Arrays created giving information about query number and position within
         individual query for each location on the concatenated sequence are
         based on numbering beginning with 1, not 0 for queries and positions
*/
QueriesPtr LIBCALL 
BlastMakeMultQueries PROTO((BspArray bsp_arr, Uint4 num_queries, Boolean is_na, Uint4 num_spacers, SeqLocPtr PNTR lcase_mask_arr))
{
	QueriesPtr queries;
	Int4 bsp_iter, pos_iter;
	BioseqPtr curr_bsp;
	Int4 curr_index;
	Int4 curr_length;
        IntArray starts, ends;
	Int4 query, pos, query_holder;
	Int4 *which_query;
	Int4 *which_pos;

	queries = (QueriesPtr) MemNew (sizeof (Queries) );

	queries->NumQueries = num_queries;
	queries->FakeBsps = bsp_arr;	
	queries->LCaseMasks = lcase_mask_arr;

	/* AM: These has to be dynamically allocated (bug fix) */
	starts = (IntArray)MemNew( sizeof( Int4 )*num_queries );
	ends   = (IntArray)MemNew( sizeof( Int4 )*num_queries );

        /* AM: Inserted allocation of effective lengths, adjustment, search 
	   space size, and dblen arrays. */
	queries->SearchSpEff = (FloatArray)MemNew( sizeof( Nlm_FloatHi )*num_queries );
	queries->DbLenEff = (Int8Array)MemNew( sizeof( Int8 )*num_queries );
	queries->Adjustments = (IntArray)MemNew( sizeof( Int4 )*num_queries );
	queries->EffLengths = (IntArray)MemNew( sizeof( Int4 )*num_queries );
	queries->result_info = (MQ_ResultInfoPtr)MemNew( sizeof( MQ_ResultInfo )*num_queries );
	queries->lambda_array = (Nlm_FloatHi PNTR)MemNew( sizeof( Nlm_FloatHi )*num_queries );
	queries->dropoff_2nd_pass_array 
	  = (BLAST_Score PNTR)MemNew( sizeof( BLAST_Score )*num_queries );
        queries->sap_array_data = (SapArrayDataPtr)MemNew( sizeof( SapArrayData ) );

	queries->MinLen      = INT4_MAX;
	queries->MinLenEff   = INT4_MAX;
	queries->MinDbLenEff = INT8_MAX;
		
        queries->HitListArray = NULL;
	queries->use_mq = FALSE;

	/* determine and record query starts and ends in the concatenated query */
	curr_index = 0;
	for (bsp_iter=0; bsp_iter<num_queries; bsp_iter++) {
		curr_bsp = *(bsp_arr + bsp_iter);
		curr_length = curr_bsp->length;
		starts[bsp_iter] = curr_index;
		curr_index += curr_length;
		ends[bsp_iter] = curr_index - 1;
		curr_index += num_spacers;

		if( is_na && bsp_iter != num_queries - 1 && curr_length%2 ) ++curr_index;
	}
	curr_index -= num_spacers;
	/* now curr_index = total number of letters in concat. query */
	queries->QueryStarts = starts;
	queries->QueryEnds = ends;

	/* create arrays showing querynum/pos in indiv. query for each letter in the 
           concatenated query sequence 
	   assumes well-formed starts/ends arrays */
	which_query = MemNew (sizeof(Int4) * curr_index);
	which_pos = MemNew (sizeof(Int4) * curr_index);
	query = query_holder = 1;
	pos = 1;
	for (pos_iter=0; pos_iter<curr_index; pos_iter++) {
		/* zero-th letter in seq will be query 1, pos 1 */
		*(which_query + pos_iter) = query; 
		*(which_pos + pos_iter) = pos; 

	  	if (ends[query_holder-1] == pos_iter) {
			query_holder++;
			query = SPACER_POS;
			pos = 1;
		} else if (starts[query_holder-1] == pos_iter) {
			query = query_holder;
			pos = 1;
		} else {
			pos++;
		}	
	}
	queries->WhichQuery = which_query;
	queries->WhichPos = which_pos;
	return queries;	
}

/* ConcatSeqLoc():

   Takes a SeqLoc of type SEQLOCK_PACKED_INT that has offsets local to 
   individual queries and returns a new SeqLoc with offsets relative to
   the concatenated query.

*/
SeqLocPtr LIBCALL ConcatSeqLoc PROTO(( QueriesPtr mult_queries, SeqLocPtr loc, SeqIdPtr id, Uint4 qnum ))
{
  SeqLocPtr result = NULL;
  SeqLocPtr current = NULL, newloc = NULL, last = NULL;
  Uint4 start, end;

  if( mult_queries && loc->choice == SEQLOC_PACKED_INT ) 
  {
    result = ValNodeNew( NULL );
    result->choice = loc->choice;
    current = (SeqLocPtr)loc->data.ptrvalue;

    while( current )
    {
      start = SeqLocStart( current );
      end = SeqLocStop( current );
      start = mult_queries->QueryStarts[qnum] + start;
      end = mult_queries->QueryStarts[qnum] + end;
      newloc = SeqLocIntNew( start, end, SeqLocStrand( loc ), id );

      if( last )
        last->next = newloc;
      else
        result->data.ptrvalue = (void *)newloc;

      last = newloc;
      current = current->next;
    }
  }

  return result;
}

/* InitHitLists():

   This function breaks search->current_hitlist into multiple lists for
   each query and fills in the structures in search->mult_queries.

*/
void LIBCALL InitHitLists PROTO(( BlastSearchBlkPtr search ))
{
  QueriesPtr queries;
  BLAST_HitListPtr * hitlist_array;
  BLAST_HitListPtr current_hitlist;
  BLAST_HSPPtr hsp;
  Uint4 i, query_num;

  if( !search ) return;

  queries = search->mult_queries;
  current_hitlist = search->current_hitlist;

  if( !queries || !current_hitlist ) return;

  hitlist_array = queries->HitListArray;

  if( !hitlist_array )
  {
    hitlist_array = (BLAST_HitListPtr *)MemNew( 
      sizeof( BLAST_HitListPtr )*queries->NumQueries );
    queries->HitListArray = hitlist_array;

    for( i = 0; i < queries->NumQueries; ++i )
      hitlist_array[i] = BlastHitListNew( search );
  }

  for( i = 0; i < queries->NumQueries; ++i )
  {
    BlastHitListPurge( hitlist_array[i] );
    hitlist_array[i]->next = NULL;
    hitlist_array[i]->hspmax = current_hitlist->hspmax; 
    hitlist_array[i]->hspcnt = 0;
    hitlist_array[i]->hspcnt_max = current_hitlist->hspcnt_max;
    hitlist_array[i]->further_process = current_hitlist->further_process;
    hitlist_array[i]->do_not_reallocate = current_hitlist->do_not_reallocate;
    MemFree( hitlist_array[i]->lh_helper );
    hitlist_array[i]->lh_helper = NULL;
    hitlist_array[i]->lh_helper_size = current_hitlist->lh_helper_size;
    MemFree( hitlist_array[i]->hsp_array );
    hitlist_array[i]->hsp_array = (BLAST_HSPPtr PNTR)MemNew(
      sizeof( BLAST_HSPPtr )*hitlist_array[i]->hspmax );
  }

  for( i = 0; i < current_hitlist->hspcnt; ++i )
  {
    hsp = current_hitlist->hsp_array[i];

    if( hsp )
    {
      query_num = GetQueryNum( queries, hsp->query.offset, 
                               hsp->query.end, hsp->query.frame );
      hitlist_array[query_num]->hsp_array[hitlist_array[query_num]->hspcnt] = hsp;
      current_hitlist->hsp_array[i] = NULL;
      ++hitlist_array[query_num]->hspcnt;
    }
  }
}
/* TEMPORARY FUNCTION - FOR DEBUGGING ONLY */
void LIBCALL MQ_CheckLists PROTO(( BLASTResultHitlistPtr PNTR local, Int4 lsize,
                                   BLASTResultHitlistPtr PNTR res, Int4 rsize ))
{
  Int4 i, j;

  for( i = 0; i < lsize; ++i )
  {
    int found = 0;

    for( j = 0; j < rsize; ++j )
      if( local[i] == res[j] )
      {
        found = 1;
        break;
      }

    if( !found )
    {
      fprintf( stderr, "ERROR: %d is not in res\n", i );
      return;
    }
  }
}

/* ResultIndex1():

   This function first calls the binary search via ResultIndex(). Then tries
   to locate the exact object in the array by pointer.

*/
Int4 LIBCALL ResultIndex1 PROTO(( BLASTResultHitlistPtr ptr,
                                 BLASTResultHitlistPtr PNTR results,
				 Int4 num_elements ))
{
  Int4 del_index = ResultIndex( ptr->best_evalue, ptr->high_score,
                                ptr->subject_id, results, num_elements );
  ASSERT( del_index < num_elements );
  
  if( results[del_index] == ptr ) return del_index;

  while( del_index >= 0 && results[del_index]->subject_id == ptr->subject_id )
    --del_index;

  while( ++del_index < num_elements && results[del_index] != ptr );

  ASSERT( del_index < num_elements );

  return del_index;
}

/* ResultIndex():

   This function performs a binary search in results for an element with
   goven evalue, score, and subject id. The number of elements in results
   is given by num_elements.

*/
Int4 LIBCALL ResultIndex PROTO(( Nlm_FloatHi target_e, Int4 target_score, Int4 subject_id,
                  BLASTResultHitlistPtr PNTR results,
		  Int4 num_elements ))
{
  Int4 high_index, low_index, new_index, old_index;

  if( !num_elements ) return 0;

  high_index = 0;
  low_index = num_elements - 1;
  new_index = (high_index + low_index)/2;
  old_index = new_index;

  while( 1 )
  {
    if( results[new_index]->best_evalue > target_e )
      low_index = new_index;
    else if( results[new_index]->best_evalue < target_e )
      high_index = new_index;
    else if( results[new_index]->high_score < target_score )
      low_index = new_index;
    else if( results[new_index]->high_score > target_score )
      high_index = new_index;
    else if( results[new_index]->subject_id < subject_id )
      low_index = new_index;
    else high_index = new_index;

    new_index = (high_index + low_index)/2;

    if( old_index == new_index )
    {
      while( new_index < num_elements )
        if( results[new_index]->best_evalue < target_e ) ++new_index;
        else if( results[new_index]->best_evalue == target_e
                 && results[new_index]->high_score > target_score )
          ++new_index;
        else if( results[new_index]->best_evalue == target_e
	         && results[new_index]->high_score == target_score
		 && results[new_index]->subject_id > subject_id )
          ++new_index;
        else break;

      break;
    }

    old_index = new_index;
  }

  return new_index;
}

/* MQ_UpdateResultLists():

   Updates the individual hitlists in mult_queries removing 
   unreferenced elements.

*/
void LIBCALL MQ_UpdateResultLists PROTO(( QueriesPtr mult_queries ))
{
  Uint4 qnum, index, new_index = 0;
  MQ_ResultInfoPtr result_info;

  for( qnum = 0; qnum < mult_queries->NumQueries; ++qnum )
  {
    result_info = mult_queries->result_info + qnum;

    for( index = 0; index < result_info->NumResults; ++index )
      if( result_info->results[index]->num_ref > 0 )
        result_info->results[new_index++] = result_info->results[index];

    result_info->NumResults = new_index;
  }
}

