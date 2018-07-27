static char const rcsid[] = "$Id: blast_seq.c,v 1.39 2004/04/16 14:44:07 papadopo Exp $";
/*
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
* ===========================================================================*/

/*****************************************************************************

File name: blast_seq.c

Author: Ilya Dondoshansky

Contents: Functions converting between SeqLocs and structures used in BLAST.

******************************************************************************
 * $Revision: 1.39 $
 * */

#include <seqport.h>
#include <sequtil.h>
#include <objloc.h>
#include <algo/blast/api/blast_seq.h>
#include <algo/blast/core/blast_filter.h>
#include <algo/blast/core/blast_util.h>
#include <algo/blast/core/blast_encoding.h>

BlastMaskLoc* BlastMaskLocFromSeqLoc(SeqLocPtr mask_slp, Int4 index)
{
   BlastMaskLoc* new_mask = (BlastMaskLoc*) calloc(1, sizeof(BlastMaskLoc));
   BlastSeqLoc* loc,* last_loc = NULL,* head_loc = NULL;
   SeqIntPtr si;

   new_mask->index = index;

   if (mask_slp->choice == SEQLOC_PACKED_INT)
      mask_slp = (SeqLocPtr) mask_slp->data.ptrvalue;

   for ( ; mask_slp; mask_slp = mask_slp->next) {
      si = (SeqIntPtr) mask_slp->data.ptrvalue;
      loc = BlastSeqLocNew(si->from, si->to);
      if (!last_loc) {
         last_loc = head_loc = loc;
      } else {
         last_loc->next = loc;
         last_loc = last_loc->next;
      }
   }
   new_mask->loc_list = head_loc;
   return new_mask;
}

SeqLocPtr BlastMaskLocToSeqLoc(Uint1 program_number, BlastMaskLoc* mask_loc, 
                            SeqLocPtr slp)
{
   BlastSeqLoc* loc;
   SeqIntPtr si;
   SeqLocPtr mask_head = NULL, last_mask = NULL;
   SeqLocPtr mask_slp_last, mask_slp_head, new_mask_slp;
   SeqIdPtr seqid;
   SSeqRange* di;
   Int4 index;
   Boolean translated_query;
   Uint1 num_frames;
   
   translated_query = (program_number == blast_type_blastx || 
                       program_number == blast_type_tblastx);

   num_frames = (translated_query ? NUM_FRAMES : 1);

   for (index = 0; slp; ++index, slp = slp->next) {
      /* Find the mask locations for this query */
      for ( ; mask_loc && (mask_loc->index/num_frames < index); 
            mask_loc = mask_loc->next);
      if (!mask_loc)
         /* No more masks */
         break;

      if (mask_loc->index != index)
         /* There is no mask for this sequence */
         continue;

      seqid = SeqLocId(slp);
      while (mask_loc && (mask_loc->index/num_frames == index)) {
      
         mask_slp_head = mask_slp_last = NULL;
         for (loc = mask_loc->loc_list; loc; loc = loc->next) {
            di = (SSeqRange*) loc->ptr;
            si = SeqIntNew();
            si->from = di->left;
            si->to = di->right;
            si->id = SeqIdDup(seqid);
            if (!mask_slp_last)
               mask_slp_last = 
                  ValNodeAddPointer(&mask_slp_head, SEQLOC_INT, si);
            else 
               mask_slp_last = 
                  ValNodeAddPointer(&mask_slp_last, SEQLOC_INT, si);
         }

         if (mask_slp_head) {
            Uint1 frame_index;
            new_mask_slp = ValNodeAddPointer(NULL, SEQLOC_PACKED_INT, 
                                             mask_slp_head);

            frame_index = 
               (translated_query ? (mask_loc->index % num_frames) + 1 : 0);

            /* The 'choice' of the SeqLoc in masks should show the frame,
               with values 1..6 when queries are translated; otherwise
               it does not matter. */
            if (!last_mask) {
               last_mask = ValNodeAddPointer(&mask_head, 
                              frame_index, new_mask_slp);
            } else {
               last_mask = ValNodeAddPointer(&last_mask, 
                              frame_index, new_mask_slp);
            }
         }
         mask_loc = mask_loc->next;
      }
   }
   return mask_head;
}

Int2 BlastMaskLocDNAToProtein(BlastMaskLoc** mask_loc_ptr, SeqLocPtr slp)
{
   Int2 status = 0;
   BlastMaskLoc* last_mask = NULL,* head_mask = NULL,* mask_loc; 
   Int4 dna_length;
   BlastSeqLoc* dna_loc,* prot_loc_head,* prot_loc_last;
   SSeqRange* dip;
   Int4 index, context;
   Int2 frame;
   Int4 from, to;

   if (!mask_loc_ptr)
      return -1;

   mask_loc = *mask_loc_ptr;

   for (index = 0; slp && mask_loc; ++index, slp = slp->next) {
      if (index < mask_loc->index)
         /* Advance to the next nucleotide sequence */
         continue;

      dna_length = SeqLocLen(slp);
      /* Reproduce this mask for all 6 frames, with translated 
         coordinates */
      for (context = 0; context < NUM_FRAMES; ++context) {
         if (!last_mask) {
            head_mask = last_mask = (BlastMaskLoc *) calloc(1, sizeof(BlastMaskLoc));
         } else {
            last_mask->next = (BlastMaskLoc *) calloc(1, sizeof(BlastMaskLoc));
            last_mask = last_mask->next;
         }
         
         last_mask->index = NUM_FRAMES * index + context;
         prot_loc_last = prot_loc_head = NULL;
         
         frame = BLAST_ContextToFrame(blast_type_blastx, context);

         for (dna_loc = mask_loc->loc_list; dna_loc; 
              dna_loc = dna_loc->next) {
            dip = (SSeqRange*) dna_loc->ptr;
            if (frame < 0) {
               from = (dna_length + frame - dip->right)/CODON_LENGTH;
               to = (dna_length + frame - dip->left)/CODON_LENGTH;
            } else {
               from = (dip->left - frame + 1)/CODON_LENGTH;
               to = (dip->right - frame + 1)/CODON_LENGTH;
            }
            if (!prot_loc_last) {
               prot_loc_head = prot_loc_last = BlastSeqLocNew(from, to);
            } else { 
               prot_loc_last->next = BlastSeqLocNew(from, to);
               prot_loc_last = prot_loc_last->next; 
            }
         }
         last_mask->loc_list = prot_loc_head;
      }
      /* Go to the next nucleotide mask */
      mask_loc = mask_loc->next;
   }

   /* Free the mask with nucleotide coordinates */
   BlastMaskLocFree(*mask_loc_ptr);
   /* Return the new mask with protein coordinates */
   *mask_loc_ptr = head_mask;

   return status;
}


Int2 BlastMaskLocProteinToDNA(BlastMaskLoc** mask_loc_ptr, SeqLocPtr slp)
{
   Int2 status = 0;
   Int4 index;
   BlastMaskLoc* mask_loc;
   BlastSeqLoc* loc;
   SSeqRange* dip;
   Int4 dna_length;
   Int2 frame;
   Int4 from, to;

   if (!mask_loc_ptr) 
      return -1;

   mask_loc = *mask_loc_ptr;

   index = NUM_FRAMES;

   while (slp && mask_loc) {
      if (index <= mask_loc->index) {
         /* Advance to the next nucleotide sequence */
         index += NUM_FRAMES;
         slp = slp->next;
      } else {
         dna_length = SeqLocLen(slp);
         frame = BLAST_ContextToFrame(blast_type_blastx, 
                                mask_loc->index % NUM_FRAMES);

         for (loc = mask_loc->loc_list; loc; loc = loc->next) {
            dip = (SSeqRange*) loc->ptr;
            if (frame < 0)	{
               to = dna_length - CODON_LENGTH*dip->left + frame;
               from = dna_length - CODON_LENGTH*dip->right + frame + 1;
            } else {
               from = CODON_LENGTH*dip->left + frame - 1;
               to = CODON_LENGTH*dip->right + frame - 1;
            }
            dip->left = from;
            dip->right = to;
         }
         /* Advance to the next mask */
         mask_loc = mask_loc->next;
      }
   }
   return status;
}

static Int4 BLAST_SetUpQueryInfo(SeqLocPtr slp, Uint1 program, 
               BlastQueryInfo** query_info_ptr)
{
   Uint4 length, protein_length;
   Boolean translate = 
      (program == blast_type_blastx || program == blast_type_tblastx);
   Boolean is_na = (program == blast_type_blastn);
   Int2 num_frames, frame;
   Uint1 strand;
   BlastQueryInfo* query_info;
   Int4* context_offsets;
   Int4 index;
   Int4 total_contexts;
   Uint4 max_length = 0;

   if (translate)
      num_frames = 6;
   else if (is_na)
      num_frames = 2;
   else
      num_frames = 1;

   if ((query_info = (BlastQueryInfo*) malloc(sizeof(BlastQueryInfo)))
       == NULL)
      return -1;

   query_info->first_context = 0;
   query_info->num_queries = ValNodeLen(slp);
   query_info->last_context = query_info->num_queries*num_frames - 1;
   total_contexts = query_info->last_context + 1;

   if ((strand = SeqLocStrand(slp)) == Seq_strand_minus) {
      if (translate)
         query_info->first_context = 3;
      else
         query_info->first_context = 1;
   } else if (strand == Seq_strand_plus) {
      if (translate)
         query_info->last_context -= 3;
      else 
         query_info->last_context -= 1;
   }

   if ((context_offsets = (Int4*) 
      malloc((total_contexts+1)*sizeof(Int4))) == NULL)
      return -1;

   if ((query_info->eff_searchsp_array = 
      (Int8*) malloc(total_contexts*sizeof(Int8))) == NULL)
      return -1;
   if ((query_info->length_adjustments =
        (Int4*) malloc(total_contexts*sizeof(Int4))) == NULL)
       return -1;

   context_offsets[0] = 0;

   query_info->context_offsets = context_offsets;
   
   /* Fill the context offsets */
   for (index = 0; slp; slp = slp->next, index += num_frames) {
      length = SeqLocLen(slp);  /* FIXME: could return -1 */
      strand = SeqLocStrand(slp);
      if (translate) {
         Int2 first_frame, last_frame;
         if (strand == Seq_strand_plus) {
            first_frame = 0;
            last_frame = 2;
         } else if (strand == Seq_strand_minus) {
            first_frame = 3;
            last_frame = 5;
         } else {
            first_frame = 0;
            last_frame = 5;
         }
         for (frame = 0; frame < first_frame; ++frame)
            context_offsets[index+frame+1] = context_offsets[index+frame];
         for (frame = first_frame; frame <= last_frame; ++frame) {
            protein_length = (length - frame%CODON_LENGTH)/CODON_LENGTH;
            max_length = MAX(max_length, protein_length);
            context_offsets[index+frame+1] = 
               context_offsets[index+frame] + protein_length + 1;
         }
         for ( ; frame < num_frames; ++frame)
            context_offsets[index+frame+1] = context_offsets[index+frame];
      } else {
         max_length = MAX(max_length, length);
         
         if (is_na) {
            if (strand == Seq_strand_plus) {
               context_offsets[index+1] = context_offsets[index] + length + 1;
               context_offsets[index+2] = context_offsets[index+1];
            } else if (strand == Seq_strand_minus) {
               context_offsets[index+1] = context_offsets[index];
               context_offsets[index+2] = 
                  context_offsets[index+1] + length + 1;
            } else {
               context_offsets[index+1] = context_offsets[index] + length + 1;
               context_offsets[index+2] = 
                  context_offsets[index+1] + length + 1;
            }
         } else {
            context_offsets[index+1] = context_offsets[index] + length + 1;
         }
      }
   }
   query_info->max_length = max_length;

   *query_info_ptr = query_info;
   return 0;
}

/** Given a SeqPort, fills a preallocated sequence buffer 
 * in the correct encoding.
 */
static Int2 SeqPortToSequenceBuffer(SeqPortPtr spp, Uint1 encoding, 
                                    Uint1** buffer)
{
   Uint1* buffer_var = *buffer;
   Uint1 residue;

   if (!buffer_var || !spp)
      return -1;

   SeqPortSet_do_virtual(spp, TRUE);

   switch (encoding) {
   case BLASTP_ENCODING: case NCBI4NA_ENCODING:
      while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF) {
         if (IS_residue(residue)) {
           *(buffer_var++) = residue;
         }
      }
      break;
   case BLASTNA_ENCODING:
      while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF) {
         if (IS_residue(residue)) {
            *(buffer_var++) = NCBI4NA_TO_BLASTNA[residue];
         }
      }
      break;
   default:
      break;
   }

   *buffer = buffer_var;
   return 0;
}

/** Fills sequence buffer for a single SeqLoc; 
 * fills both strands if necessary.
 */
static Int2 SeqLocFillSequenceBuffer(SeqLocPtr slp, Uint1 encoding, 
        Boolean add_sentinel_bytes, Boolean both_strands, Uint1* buffer)
{
   Uint1* buffer_var;
   SeqPortPtr spp = NULL;
   Uint1 sentinel = 
      (encoding == BLASTNA_ENCODING ? NCBI4NA_TO_BLASTNA[NULLB] : NULLB);
   Uint1 seq_code, strand;

   buffer_var = buffer;

   if (add_sentinel_bytes) {
      *buffer_var = sentinel;
      ++buffer_var;
   }

   if (encoding == BLASTP_ENCODING) {
      seq_code = Seq_code_ncbistdaa;
      strand = Seq_strand_unknown;
   } else {
      seq_code = Seq_code_ncbi4na;
      strand = SeqLocStrand(slp);
   }

   spp = SeqPortNewByLoc(slp, seq_code);
   SeqPortSet_do_virtual(spp, TRUE);

   SeqPortToSequenceBuffer(spp, encoding, &buffer_var);
   spp = SeqPortFree(spp);

   if (add_sentinel_bytes)
      *buffer_var = sentinel;

   if (both_strands && strand == Seq_strand_both) {
      SeqLocPtr tmp_slp=NULL;

      ++buffer_var;

      tmp_slp = SeqLocIntNew(SeqLocStart(slp), SeqLocStop(slp),
                             Seq_strand_minus, SeqLocId(slp));
            
      spp = SeqPortNewByLoc(tmp_slp, Seq_code_ncbi4na);
      SeqPortToSequenceBuffer(spp, encoding, &buffer_var);
      if (add_sentinel_bytes)
         *buffer_var = sentinel;

      spp = SeqPortFree(spp);
      SeqLocFree(tmp_slp);
   }

   return 0;
}

/** Find a genetic code string in ncbistdaa encoding, given an integer 
 * genetic code value.
 */
Int2 BLAST_GeneticCodeFind(Int4 gc, Uint1** genetic_code)
{
   ValNodePtr vnp;
   GeneticCodePtr gcp;
   char* gen_code_eaa = NULL;
   Uint1* gen_code_stdaa = NULL;
   Int4 gen_code_length = 0, index;
   SeqMapTablePtr smtp;

   gcp = GeneticCodeFind(gc, NULL);
   for (vnp = (ValNodePtr)gcp->data.ptrvalue; vnp != NULL; 
        vnp = vnp->next) {
      if (vnp->choice == 3) {  /* ncbieaa */
         gen_code_eaa = (char*)vnp->data.ptrvalue;
         break;
      }
   }

   if (!gen_code_eaa)
      return -1;
   smtp = SeqMapTableFind(Seq_code_ncbistdaa, Seq_code_ncbieaa);
   gen_code_length = StrLen(gen_code_eaa);
   *genetic_code = gen_code_stdaa = (Uint1*) calloc(gen_code_length+1, 1);

   if (!gen_code_stdaa)
      return -2;

   for (index = 0; index < gen_code_length; ++index) {
      gen_code_stdaa[index] = 
         SeqMapTableConvert(smtp, gen_code_eaa[index]);
   }
   
   return 0;
}

/** BLAST_GetSequence
 * Purpose:     Get the sequence for the BLAST engine, put in a Uint1 buffer
 * @param slp SeqLoc to extract sequence for [in]
 * @param query_info The query information structure, pre-initialized,
 *                   but filled here [in]
 * @param query_options Query setup options, containing the genetic code for
 *                      translation [in]
 * @param num_frames How many frames to get for this sequence? [in]
 * @param encoding In what encoding to retrieve the sequence? [in]
 * @param buffer_out Buffer to hold plus strand or protein [out]
 * @param buffer_length Length of buffer allocated [out]
 */
static Int2 
BLAST_GetSequence(SeqLocPtr slp, BlastQueryInfo* query_info, 
   const QuerySetUpOptions* query_options, Uint1 num_frames, Uint1 encoding, 
   Uint1* *buffer_out, Int4 *buffer_length)
{
   Int2		status=0; /* return value. */
   Int4 total_length; /* Total length of all queries/frames/strands */
   Int4		index; /* Loop counter */
   SeqLocPtr	slp_var; /* loop variable */
   Uint1*	buffer,* buffer_var; /* buffer offset to be worked on. */
   Boolean add_sentinel_bytes = TRUE;
   Uint1* genetic_code=NULL;
   Boolean translate = FALSE;
   Int4 offset = 0;

   if (query_info) {
      *buffer_length = total_length = 
         query_info->context_offsets[query_info->last_context+1] + 1;
   } else {
      /* Subject sequence in 2 sequences comparison */
      *buffer_length = SeqLocLen(slp);
      if (encoding == NCBI4NA_ENCODING) {
         /* Searches with translated subjects (tblastn, tblastx) */
         add_sentinel_bytes = FALSE;
         total_length = *buffer_length;
      } else {
         total_length = (*buffer_length) + 2;
      }
   }

   if (num_frames == 6) {
      /* Sequence must be translated in 6 frames. This can only happen
         for query - subject sequences are translated later. */
      Int4 gc;
      
      translate = TRUE;
      gc = (query_options ? query_options->genetic_code : 1);

      if ((status = BLAST_GeneticCodeFind(gc, &genetic_code)) != 0)
         return status;
   }

   *buffer_out = buffer = (Uint1 *) malloc((total_length)*sizeof(Uint1));
   
   for (index = 0, slp_var = slp; slp_var; 
        slp_var = slp_var->next, index += num_frames)
   {
      if (translate) {
         Uint1* na_buffer;
         Int2 frame, frame_start, frame_end;
         Int4 na_length;
         Uint1 strand;

         na_length = SeqLocLen(slp_var);
         strand = SeqLocStrand(slp_var);
         /* Retrieve nucleotide sequence in an auxiliary buffer; 
            then translate into the appropriate place in the 
            preallocated buffer */
         if (strand == Seq_strand_plus) {
            na_buffer = (Uint1 *) malloc(na_length + 2);
            frame_start = 0;
            frame_end = 2;
         } else if (strand == Seq_strand_minus) {
            na_buffer = (Uint1 *) malloc(na_length + 2);
            frame_start = 3;
            frame_end = 5;
         } else {
            na_buffer = (Uint1*) malloc(2*na_length + 3);
            frame_start = 0;
            frame_end = 5;
         }
         SeqLocFillSequenceBuffer(slp_var, encoding, TRUE, TRUE, na_buffer);
         buffer_var = na_buffer + 1;
      
         for (frame = frame_start; frame <= frame_end; frame++) {
            offset = query_info->context_offsets[index+frame];
            if (strand == Seq_strand_both && frame == 3) {
               /* Advance the nucleotide sequence pointer to the 
                  beginning of the reverse strand */
               buffer_var += na_length + 1;
            }
            BLAST_GetTranslation(buffer_var, NULL, na_length, 
               frame%3+1, &buffer[offset], genetic_code);
         }
         sfree(na_buffer);
      } else {
         /* This can happen both for query and subject, so query_info 
            might not be initialized here. */
         if (query_info)
            offset = query_info->context_offsets[index];
         SeqLocFillSequenceBuffer(slp_var, encoding, add_sentinel_bytes, 
                                  (num_frames == 2), &buffer[offset]);
      }
      /* For subjects, do only one SeqLoc at a time */
      if (!query_info)
         break;
   }

   return status;
}

Int2 BLAST_SetUpQuery(Uint1 program_number, SeqLocPtr query_slp, 
        const QuerySetUpOptions* query_options, 
        BlastQueryInfo** query_info, BLAST_SequenceBlk* *query_blk)
{
   Uint1* buffer;	/* holds sequence for plus strand or protein. */
   Int4 buffer_length;
   Int2 status;
   Uint1 num_frames;
   Uint1 encoding;

   if ((status = BLAST_SetUpQueryInfo(query_slp, program_number, query_info)))
      return status;

   if (program_number == blast_type_blastn) {
      encoding = BLASTNA_ENCODING;
      num_frames = 2;
   } else if (program_number == blast_type_blastp ||
              program_number == blast_type_rpsblast ||
              program_number == blast_type_tblastn) {
      encoding = BLASTP_ENCODING;
      num_frames = 1;
   } else if (program_number == blast_type_rpstblastn) {
      encoding = NCBI4NA_ENCODING;
      num_frames = 1;
   } else { 
      encoding = NCBI4NA_ENCODING;
      num_frames = 6;
   }

   if ((status=BLAST_GetSequence(query_slp, *query_info, query_options,
                  num_frames, encoding, &buffer, &buffer_length)))
      return status; 
        
   /* Do not count the first and last sentinel bytes in the 
      query length */
   if ((status=BlastSetUp_SeqBlkNew(buffer, buffer_length-2, 
                                    0, query_blk, TRUE)))
      return status;

   return 0;
}

Int2 BLAST_SetUpSubject(Uint1 program_number, 
        SeqLocPtr subject_slp, BLAST_SequenceBlk** subject)
{
   Int2 status = 0;
   Uint1* subject_buffer = NULL; /* Buffer for the compressed subject 
                                      sequence in two sequences case */
   Int4 buffer_length=0; /* Length of subject sequence for two sequences 
                            case */
   Uint1 encoding;

   if (program_number == blast_type_blastn)
      encoding = BLASTNA_ENCODING;
   else if (program_number == blast_type_tblastn ||
            program_number == blast_type_tblastx) {
      encoding = NCBI4NA_ENCODING;
   } else {
      encoding = BLASTP_ENCODING;
   }

   if ((status = BLAST_GetSequence(subject_slp, NULL, NULL, 1, encoding,
                                   &subject_buffer, &buffer_length)))
      return status;
   
   /* Initialize the sequence block, saving the sequence buffer in 
      'sequence_start'. */
   if ((status=BlastSetUp_SeqBlkNew(subject_buffer, buffer_length,
                                    0, subject, TRUE)))
      return status;

   /* If subject sequence is nucleotide, create compressed sequence buffer
      and save it in 'sequence'. For blastn, the sentinel bytes should not 
      be included in the packed sequence. */
   if (program_number == blast_type_blastn)
      ++subject_buffer;

   if (program_number == blast_type_blastn || 
       program_number == blast_type_tblastn || 
       program_number == blast_type_tblastx) {
      BLAST_PackDNA(subject_buffer, buffer_length, encoding, 
                    &((*subject)->sequence));
      (*subject)->sequence_allocated = TRUE;
   }

   return 0;
}
