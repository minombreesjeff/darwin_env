/* $Id: blast_format.c,v 1.85 2005/04/27 19:59:44 dondosha Exp $
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
* ===========================================================================
*
* Author: Ilya Dondoshansky
*
*/

/** @file blast_format.c
 * Formatting of BLAST results (SeqAlign)
 */

static char const rcsid[] = "$Id: blast_format.c,v 1.85 2005/04/27 19:59:44 dondosha Exp $";

#include <algo/blast/api/blast_format.h>
#include <algo/blast/api/blast_seq.h>
#include <algo/blast/core/blast_filter.h>
#include <algo/blast/core/blast_util.h>
#include <algo/blast/api/blast_returns.h>
#include <algo/blast/api/blast_seq.h>
#include <readdb.h>
#include <txalign.h>
#include <blfmtutl.h>
#include <xmlblast.h>

extern Uint1 LIBCALL 
BlastGetTypes PROTO((char* blast_program, Boolean* query_is_na, 
                     Boolean* db_is_na));
extern void PrintTabularOutputHeader PROTO((char* blast_database, 
               BioseqPtr query_bsp, SeqLocPtr query_slp, char* blast_program, 
               Int4 iteration, Boolean believe_query, FILE *outfp));
extern void BlastPrintTabulatedResults PROTO((SeqAlignPtr seqalign, 
               BioseqPtr query_bsp, SeqLocPtr query_slp, Int4 num_alignments, 
               char* blast_program, Boolean is_ungapped, 
               Boolean believe_query, Int4 q_shift, Int4 s_shift, FILE *fp, 
               Boolean print_query_info));
extern Int4 SeqLocTotalLen (char* prog_name, SeqLocPtr slp);

extern void MBXmlClose(MBXml* mbxp, ValNodePtr other_returns, 
                       Boolean ungapped);

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

Int2 BlastFormattingOptionsNew(EBlastProgramType program_number, 
        char* file_out, Int4 num_descriptions, Int4 num_alignments, 
        Int4 align_view, BlastFormattingOptions** format_options_ptr)
{
   BlastFormattingOptions* format_options; 

   /* For translated RPS blast, the query will be a
      nucleotide sequence and subject sequences will
      be protein; thus the formatting must behave as
      if a blastx search was performed */

   if (program_number == eBlastTypeRpsTblastn)
      program_number = eBlastTypeBlastx;

   if ((format_options = (BlastFormattingOptions*)
        calloc(1, sizeof(BlastFormattingOptions))) == NULL)
      return -1;
   format_options->believe_query = FALSE;
   if (align_view != 7 && align_view < 10) {
      FILE* outfp;
      if ((outfp = FileOpen(file_out, "w")) == NULL)
         return -1;
      format_options->outfp = (void *) outfp;
   } else {
      AsnIoPtr aip;
      char write_mode[3];

      switch (align_view) {
      case 7:
         strcpy(write_mode, "wx"); break;
      case 10:
         strcpy(write_mode, "w"); break;
      case 11:
         strcpy(write_mode, "wb"); break;
      default:
         return -1;
      }
      
      if ((aip = AsnIoOpen(file_out, write_mode)) == NULL)
         return -1;
      format_options->outfp = (void *) aip;
   }

   if ((program_number == eBlastTypeBlastx || program_number == eBlastTypeTblastx)
        && align_view > 0 && align_view < 7)
   {
          ErrPostEx(SEV_FATAL, 1, 0, "This option is not supported with blastx or tblastx");
          format_options = BlastFormattingOptionsFree(format_options);
          return -1;
   }


   format_options->number_of_descriptions = num_descriptions;
   format_options->number_of_alignments = num_alignments;
   
   format_options->print_options = 0;
   format_options->align_options = 0;
   format_options->align_options += TXALIGN_COMPRESS;
   format_options->align_options += TXALIGN_END_NUM;
   format_options->align_options += TXALIGN_SHOW_GI;
   format_options->print_options += TXALIGN_SHOW_GI;
   
   if (align_view == 0)
   {
      format_options->align_options += TXALIGN_MATRIX_VAL;
      format_options->align_options += TXALIGN_SHOW_QS;
   }
   else
   {
       format_options->align_options += TXALIGN_MASTER;
       if (align_view == 1 || align_view == 3)
            format_options->align_options += TXALIGN_MISMATCH;
       if (align_view == 3 || align_view == 4 || align_view == 6)
            format_options->align_options += TXALIGN_FLAT_INS;
        if (align_view == 5 || align_view == 6)
            format_options->align_options += TXALIGN_BLUNT_END;
   }

   if (program_number == eBlastTypeBlastx)
      format_options->align_options += TXALIGN_BLASTX_SPECIAL;
   
   format_options->align_view = align_view;

   *format_options_ptr = format_options;
   return 0;
}

BlastFormattingOptions* 
BlastFormattingOptionsFree(BlastFormattingOptions* format_options)
{
   /* In case of XML output, the i/o stream has already been closed. */
   if (format_options->align_view != 7) {
      if (format_options->align_view < 10)
         FileClose( (FILE *)format_options->outfp);
      else
         AsnIoClose( (AsnIoPtr)format_options->outfp);
   }

   sfree(format_options);
   return NULL;
}

Int2 
BlastPrintLogReport(const BlastFormattingOptions* format_options, Int4 count)
{
     if (format_options == NULL || format_options->outfp == NULL)
        return -1;

     fprintf((FILE*) format_options->outfp, "Mega BLAST run finished, processed %ld queries\n", (long) count);

     return 0;
}

#define BXML_INCLUDE_QUERY 0x1

static BlastOutputPtr 
CreateBlastOutputHead(CharPtr program, CharPtr database, SeqLocPtr query_loc, 
                     Int4 flags, Blast_SearchParams* search_param)
{
    BlastOutputPtr boutp;
    Char buffer[1024];
    SeqPortPtr spp;
    Boolean is_aa = FALSE;
    Int4 i;
    
    if((boutp = BlastOutputNew()) == NULL)
        return FALSE;
    
    if (strcmp(program, "rpsblast") == 0)
       program = "blastp";
    else if (strcmp(program, "rpstblastn") == 0)
       program = "blastx";

    /* For optimization BLOSUM62 may be loaded ones */
    if (query_loc) {
       SeqIdPtr sip = SeqLocId(query_loc);
       BioseqPtr bsp;
       SeqIdWrite(sip, buffer, PRINTID_FASTA_LONG, sizeof(buffer));
       boutp->query_ID = StringSave(buffer);

       bsp = BioseqLockById(sip);
       if(!bsp ||
          (boutp->query_def = StringSave(BioseqGetTitle(bsp))) == NULL) {
          boutp->query_def = StringSave("No definition line found");
       }
       BioseqUnlock(bsp);

       boutp->query_len = SeqLocLen(query_loc);

       if(flags & BXML_INCLUDE_QUERY) {
          boutp->query_seq = MemNew(boutp->query_len+1);
          is_aa = (strcmp(program, "blastp") || strcmp(program, "tblastn"));
          spp = SeqPortNewByLoc(query_loc, 
                   (Uint1)((is_aa) ? Seq_code_ncbieaa : Seq_code_iupacna));
          
          for (i = 0; i < boutp->query_len; i++) {
             boutp->query_seq[i] = SeqPortGetResidue(spp);
          }
          spp = SeqPortFree(spp);
       } else {
          boutp->query_seq = NULL;    /* Do we need sequence here??? */
       }
    }
    /* Program name */
    boutp->program = StringSave(program);

    /* Database name */
    boutp->db = StringSave(database);

    /* Version text */
    sprintf(buffer, "%s %s [%s]", program, BlastGetVersionNumber(), 
            BlastGetReleaseDate());
    boutp->version = StringSave(buffer);

    /* Reference */
    boutp->reference = BlastGetReference(FALSE);

    /* Filling parameters */
    boutp->param = ParametersNew();    
    boutp->param->expect = search_param->expect;
    boutp->param->gap_open = search_param->gap_open;
    boutp->param->gap_extend = search_param->gap_extension;
    boutp->param->matrix = StringSave(search_param->matrix);
    boutp->param->sc_match = search_param->match;
    boutp->param->sc_mismatch = search_param->mismatch;
    boutp->param->include = search_param->ethresh;
    boutp->param->filter = StringSave(search_param->filter_string);
    
    return boutp;
}

#define MACRO_atp_find(atp,name)\
        if((atp = AsnTypeFind(amp, #name))==NULL){\
                ErrPostEx(SEV_ERROR,0,0,\
                        "Could not find type <%s>", #name);\
                return NULL; \
        }

static MBXml* MBXmlInit(AsnIoPtr aip, CharPtr program, CharPtr database, 
                          SeqLocPtr query_loc, Int4 flags, Blast_SearchParams* search_params)
{
    MBXml* mbxp;
    AsnModulePtr amp;
    DataVal av;
    AsnTypePtr atp;
    Boolean retval = FALSE;

    AsnTypePtr       BLASTOUTPUT;
    AsnTypePtr       BLASTOUTPUT_program;
    AsnTypePtr       BLASTOUTPUT_version;
    AsnTypePtr       BLASTOUTPUT_reference;
    AsnTypePtr       BLASTOUTPUT_db;
    AsnTypePtr       BLASTOUTPUT_query_ID;
    AsnTypePtr       BLASTOUTPUT_query_def;
    AsnTypePtr       BLASTOUTPUT_query_len;
    AsnTypePtr       BLASTOUTPUT_query_seq;
    AsnTypePtr       BLASTOUTPUT_param;
    AsnTypePtr       BLASTOUTPUT_iterations;
    AsnTypePtr       BLASTOUTPUT_iterations_E;
    AsnTypePtr       BLASTOUTPUT_mbstat;

    if (strcmp(program, "rpsblast") == 0)
       program = "blastp";
    else if (strcmp(program, "rpstblastn") == 0)
       program = "blastx";

    mbxp = (MBXml*) MemNew(sizeof(MBXml));
    
    mbxp->aip = aip;
    
    if (! bxmlobjAsnLoad()) {
        return NULL;
    }
    
    amp = AsnAllModPtr();

    MACRO_atp_find(BLASTOUTPUT,BlastOutput);
    MACRO_atp_find(BLASTOUTPUT_program,BlastOutput.program);
    MACRO_atp_find(BLASTOUTPUT_version,BlastOutput.version);
    MACRO_atp_find(BLASTOUTPUT_reference,BlastOutput.reference);
    MACRO_atp_find(BLASTOUTPUT_db,BlastOutput.db);
    MACRO_atp_find(BLASTOUTPUT_query_ID,BlastOutput.query-ID);
    MACRO_atp_find(BLASTOUTPUT_query_def,BlastOutput.query-def);
    MACRO_atp_find(BLASTOUTPUT_query_len,BlastOutput.query-len);
    MACRO_atp_find(BLASTOUTPUT_query_seq,BlastOutput.query-seq);
    MACRO_atp_find(BLASTOUTPUT_param,BlastOutput.param);
    MACRO_atp_find(BLASTOUTPUT_iterations,BlastOutput.iterations);
    MACRO_atp_find(BLASTOUTPUT_iterations_E,BlastOutput.iterations.E);
    MACRO_atp_find(BLASTOUTPUT_mbstat,BlastOutput.mbstat);

    /* Start of iterations structure */
    mbxp->atp = BLASTOUTPUT_iterations_E;

    /* Head of all BlastOutput structure */
    mbxp->BlastOutput = BLASTOUTPUT;
    
    /* Head of iterations strucure */
    mbxp->BlastOutput_iterations = BLASTOUTPUT_iterations;

    /* Head of the final statistics for Mega BLAST */
    mbxp->BlastOutput_mbstat = BLASTOUTPUT_mbstat;
    
    mbxp->boutp = CreateBlastOutputHead(program, database, query_loc, flags, search_params);
    
    atp = AsnLinkType(NULL, BLASTOUTPUT);   /* link local tree */
    
    if (atp == NULL) {
        return NULL;
    }
    
    if (! AsnOpenStruct(mbxp->aip, atp, (Pointer) mbxp->boutp)) {
        return NULL;
    }

    if (mbxp->boutp->program != NULL) {
        av.ptrvalue = mbxp->boutp -> program;
        retval = AsnWrite(mbxp->aip, BLASTOUTPUT_program,  &av);
    }
    
    if (mbxp->boutp->version != NULL) {
        av.ptrvalue = mbxp->boutp->version;
        retval = AsnWrite(mbxp->aip, BLASTOUTPUT_version,  &av);
    }
    
    if (mbxp->boutp->reference != NULL) {
        av.ptrvalue = mbxp->boutp->reference;
        retval = AsnWrite(mbxp->aip, BLASTOUTPUT_reference,  &av);
    }

    if (mbxp->boutp -> db != NULL) {
        av.ptrvalue = mbxp->boutp->db;
        retval = AsnWrite(mbxp->aip, BLASTOUTPUT_db,  &av);
    }

    if (mbxp->boutp -> query_ID != NULL) {
        av.ptrvalue = mbxp->boutp->query_ID;
        retval = AsnWrite(mbxp->aip, BLASTOUTPUT_query_ID,  &av);
    }

    if (mbxp->boutp->query_def != NULL) {
        av.ptrvalue = mbxp->boutp->query_def;
        retval = AsnWrite(mbxp->aip, BLASTOUTPUT_query_def,  &av);
    }

    av.intvalue = mbxp->boutp->query_len;
    retval = AsnWrite(mbxp->aip, BLASTOUTPUT_query_len,  &av);

    if (mbxp->boutp->query_seq != NULL) {
        av.ptrvalue = mbxp->boutp->query_seq;
        retval = AsnWrite(mbxp->aip, BLASTOUTPUT_query_seq,  &av);
    }

    if (mbxp->boutp->param != NULL) {
        if (!ParametersAsnWrite(mbxp->boutp->param, 
                                mbxp->aip, BLASTOUTPUT_param)) {
            return NULL;
        }
    }

    if(!AsnOpenStruct(mbxp->aip, BLASTOUTPUT_iterations, NULL))
        return NULL;
    
    AsnIoFlush(mbxp->aip);
    
    return mbxp;
}


static StatisticsPtr
XMLBuildStatistics(Blast_SummaryReturn* sum_returns, Boolean ungapped)
{
   StatisticsPtr stat;
   Blast_DatabaseStats* db_stats = sum_returns->db_stats;

   stat = StatisticsNew();

   stat->eff_space = db_stats->eff_searchsp;
   stat->hsp_len = db_stats->hsp_length;
   stat->db_num= db_stats->dbnum;
   stat->db_len = db_stats->dblength;

   if(ungapped) {
      if(sum_returns->ka_params != NULL) {
         stat->lambda = sum_returns->ka_params->Lambda;
         stat->kappa = sum_returns->ka_params->K;
         stat->entropy = sum_returns->ka_params->H;
      }
   } else {
      if(sum_returns->ka_params_gap != NULL) {
         stat->lambda = sum_returns->ka_params_gap->Lambda;
         stat->kappa = sum_returns->ka_params_gap->K;
         stat->entropy = sum_returns->ka_params_gap->H;
      }
   }

   return stat;
}


static IterationPtr 
XMLBuildOneQueryIteration(SeqAlignPtr seqalign,
                                   Blast_SummaryReturn* sum_returns,
                                   Boolean is_ooframe, Boolean ungapped,
                                   Int4 iter_num, CharPtr message,
                                   BioseqPtr query, ValNodePtr mask_loc)
{
    IterationPtr iterp = IterationNew();
    iterp->iter_num = iter_num;

    if (query) {
       char buffer[1024];
       SeqIdWrite(query->id, buffer, PRINTID_FASTA_LONG, sizeof(buffer));
       iterp->query_ID = StringSave(buffer);

       if((iterp->query_def = StringSave(BioseqGetTitle(query))) == NULL) {
          iterp->query_def = StringSave("No definition line found");
       }

       iterp->query_len = query->length;
    }

    if(seqalign != NULL) {
       iterp->hits =
          BXMLSeqAlignToHits(seqalign, ungapped, is_ooframe, mask_loc);
    }

    iterp->stat = XMLBuildStatistics(sum_returns, ungapped);

    iterp->message = StringSave(message);

    return iterp;
}


Int2 BLAST_FormatResults(SeqAlignPtr head, char* blast_database,
        char* blast_program, Int4 num_queries, 
        SeqLocPtr query_slp, SeqLoc* mask_loc_head, 
        const BlastFormattingOptions* format_options, Boolean is_ooframe, 
        Int4** matrix, Blast_SummaryReturn* sum_returns)
{  
   SeqAlignPtr seqalign = head, sap, next_seqalign;
   SeqLocPtr mask_loc, next_mask_loc = NULL, tmp_loc = NULL;
   EBlastProgramType program_number;
   Uint1 align_type;
   Boolean query_is_na, db_is_na;
   Int4 query_index;
   SeqLocPtr slp, mask_slp;
   SeqIdPtr query_id;
   BioseqPtr bsp = NULL;
   AsnIoPtr aip = NULL;
   BlastPruneSapStruct* prune;
   SeqAnnotPtr seqannot;
   MBXml* mbxp = NULL;
   FILE *outfp = NULL;

   if (!format_options || !format_options->outfp || !query_slp)
      return -1;

   if (format_options->align_view != 7 && format_options->align_view < 10)
      outfp = (FILE *) format_options->outfp;
   else 
      aip = (AsnIoPtr) format_options->outfp; 

   if (strcmp(blast_program, "rpsblast") == 0)
      blast_program = "blastp";
   else if (strcmp(blast_program, "rpstblastn") == 0)
      blast_program = "blastx";

   BlastProgram2Number(blast_program, &program_number);

   /* Old toolkit might have different values for program numbers, so 
      use old toolkit function to determine alignment type. */

   align_type = BlastGetTypes(blast_program, &query_is_na, &db_is_na);

   if (blast_database)
      ReadDBBioseqFetchEnable ("blast", blast_database, db_is_na, TRUE);

   query_index = 0;
   slp = query_slp;
   mask_loc = mask_loc_head;

   if (!seqalign && format_options->align_view < 7) {
      /* Acknowledge all queries and report the "no hits found" message. */
      for ( ; slp; slp = slp->next) {
         bsp = BioseqLockById(SeqLocId(slp));
         init_buff_ex(70);
         AcknowledgeBlastQuery(bsp, 70, outfp, 
            format_options->believe_query, format_options->html);
         free_buff();
         BioseqUnlock(bsp);
         fprintf(outfp, " ***** No hits found ******\n\n\n");
      }
   }
      
   if (format_options->align_view == 7) {
      mbxp = MBXmlInit(aip, blast_program, blast_database, slp, 0, sum_returns->search_params);
   }

   while (seqalign) {
      /* Find which query the current SeqAlign is for */
      query_id = TxGetQueryIdFromSeqAlign(seqalign);
      for ( ; slp && query_index < num_queries; 
            ++query_index, slp = slp->next) {
         if (SeqIdComp(query_id, SeqLocId(slp)) == SIC_YES) {
            break;
         } else if (format_options->align_view < 7) {
            /* This query has no results; acknowledge the query and report a "no
               results" message. */
            bsp = BioseqLockById(SeqLocId(slp));
            init_buff_ex(70);
            AcknowledgeBlastQuery(bsp, 70, outfp, 
               format_options->believe_query, format_options->html);
            free_buff();
            BioseqUnlock(bsp);
            fprintf(outfp, " ***** No hits found ******\n\n\n");
         }
      }
      /* The following should never happen! We mustn't have a SeqAlign left
         without a corresponding query */
      if (!slp || query_index >= num_queries)
         break;

      slp = slp->next;

      /* Find the masking location for this query. Initialize next_mask_loc
	 to the current start of the chain, in case nothing for this query 
	 will be found. */
      next_mask_loc = mask_loc;
      for ( ; mask_loc; mask_loc = mask_loc->next) {
         mask_slp = (SeqLocPtr) mask_loc->data.ptrvalue;
         if (SeqIdComp(query_id, SeqLocId(mask_slp)) == SIC_YES) {
            break;
         }
      }
      /* Unlink the masking location for this query and save the next one */
      if (mask_loc) {
         for (next_mask_loc = mask_loc; next_mask_loc->next; 
              next_mask_loc = next_mask_loc->next) {
            mask_slp = (SeqLocPtr) next_mask_loc->next->data.ptrvalue;
            if (SeqIdComp(query_id, SeqLocId(mask_slp))
                != SIC_YES) {
               break;
            }
         }
         tmp_loc = next_mask_loc;
         next_mask_loc = next_mask_loc->next;
         tmp_loc->next = NULL;
      }

      /* On the next iteration we can start from the next query */
      ++query_index;

      /* Find where this query's SeqAlign chain ends */
      for (sap = seqalign; sap->next; sap = sap->next) {
         if (SeqIdComp(query_id, TxGetQueryIdFromSeqAlign(sap->next)) != 
             SIC_YES)
            break;
      }
      /* Unlink this query's SeqAlign chain and save the start of the 
         next chain*/
      next_seqalign = sap->next;
      sap->next = NULL;

      /* Retrieve this query's Bioseq */
      bsp = BioseqLockById(query_id);

      if (format_options->align_view < 7) {
         init_buff_ex(70);
         AcknowledgeBlastQuery(bsp, 70, outfp, 
            format_options->believe_query, format_options->html);
         free_buff();
      }
      if (format_options->align_view == 8 || format_options->align_view == 9) {
         if (format_options->align_view == 9)
            PrintTabularOutputHeader(blast_database, bsp, NULL, blast_program,
               0, format_options->believe_query, outfp);
         
         BlastPrintTabulatedResults(seqalign, bsp, NULL, 
            format_options->number_of_alignments, blast_program, 
            format_options->ungapped, format_options->believe_query, 0, 0, 
            outfp, (Boolean)(format_options->align_view == 9));

         ObjMgrFreeCache(0);
         
      } else if(format_options->align_view == 7) {
         IterationPtr iterp;
         
         iterp = XMLBuildOneQueryIteration(seqalign, sum_returns, FALSE, 
                    format_options->ungapped, query_index, NULL, bsp, 
                    mask_loc);
         IterationAsnWrite(iterp, mbxp->aip, mbxp->atp);
         AsnIoFlush(mbxp->aip);
         IterationFree(iterp);
      } else {
         seqannot = SeqAnnotNew();
         seqannot->type = 2;
         AddAlignInfoToSeqAnnot(seqannot, align_type);
         seqannot->data = seqalign;
         if (aip) {
            SeqAnnotAsnWrite((SeqAnnotPtr) seqannot, aip, NULL);
            AsnIoReset(aip);
         } else if (outfp) {
            ObjMgrSetHold();
            init_buff_ex(85);
            PrintDefLinesFromSeqAlignEx2(seqalign, 80, outfp, 
               format_options->print_options, FIRST_PASS, NULL,
               format_options->number_of_descriptions, NULL, NULL);
            free_buff();
            
            /** @todo FIXME: note that by calling BlastPruneHitsFromSeqAlign
             * we're making a COPY of the seqalign to print it out! Clearly
             * this could use a better design */
            prune = BlastPruneHitsFromSeqAlign(seqalign, 
                       format_options->number_of_alignments, NULL);
            seqannot->data = prune->sap;
            if(is_ooframe) {
               OOFShowBlastAlignment(prune->sap, mask_loc, outfp, 
                                     format_options->align_options, NULL);
            } else if (format_options->align_view != 0) {
               ShowTextAlignFromAnnot(seqannot, 60, outfp, NULL, NULL, 
                  format_options->align_options, matrix, mask_loc, NULL);
            } else {
               ShowTextAlignFromAnnot(seqannot, 60, outfp, NULL, NULL, 
                  format_options->align_options, matrix, mask_loc, 
                  FormatScoreFunc);
            }
            seqannot->data = seqalign;
            prune = BlastPruneSapStructDestruct(prune);
            ObjMgrClearHold();
            ObjMgrFreeCache(0);
         }
         /* Set data to NULL, because we do not free Seq-align here. */
         seqannot->data = NULL;
         seqannot = SeqAnnotFree(seqannot);
      }
      BioseqUnlock(bsp);
      bsp = NULL;
      seqalign = next_seqalign;
      /* Re-link the chain of Seq-aligns, to prevent memory leak. */
      sap->next = next_seqalign;
      /* Relink the mask locations so chain can be freed in the end.
       The 'tmp_loc' variable points to the location that was unlinked. */
      if (tmp_loc)
          tmp_loc->next = next_mask_loc;
      
      mask_loc = next_mask_loc;

   } /* End loop on seqaligns for different queries */

   if (format_options->align_view == 7 && mbxp) {
      MBXmlClose(mbxp, NULL, format_options->ungapped);
   }

   if (blast_database)
      ReadDBBioseqFetchDisable();

   return 0;
}

static Int2
s_PHIBlastCreateSeedSeqLoc(SPHIQueryInfo* pattern_info, SeqLoc* query_seqloc, 
                           SeqLoc** seed_seqloc_ptr)
{
    Int4 index;
    for (index = 0; index < pattern_info->num_patterns; ++index) {
        SPHIPatternInfo* this_occurrence = &pattern_info->occurrences[index];
        SeqInt* si = SeqIntNew();
        si->id = SeqIdDup(SeqLocId(query_seqloc));
        si->from = this_occurrence->offset;
        si->to = this_occurrence->offset + this_occurrence->length - 1;
        ValNodeAddPointer(seed_seqloc_ptr, SEQLOC_INT, si);
    }
    return 0;
}

static Int2
s_PHIBlastFormatPatternInfo(Blast_SummaryReturn* sum_returns, FILE* outfp)
{
    Int4 index;
    SPHIQueryInfo* pattern_info = sum_returns->pattern_info;
    double lenXprob = 
        sum_returns->db_stats->eff_dblength * pattern_info->probability;
    Int8 db_patterns;

    /* Get the pattern count in database from the diagnostics structure - it is
       equal to the number of "lookup" hits. */
    ASSERT(sum_returns->diagnostics && sum_returns->diagnostics->ungapped_stat);
    db_patterns = sum_returns->diagnostics->ungapped_stat->lookup_hits;

    fprintf(outfp, "\n%d occurrence(s) of pattern in query\n", 
            pattern_info->num_patterns);
    for (index = 0; index < pattern_info->num_patterns; ++index) {
        fprintf(outfp, "\n pattern %s\n at position %d of query sequence\n",
                sum_returns->search_params->pattern, 
                pattern_info->occurrences[index].offset + 1);
        fprintf(outfp, "effective database length=%.1e\n", 
                (double)sum_returns->db_stats->eff_dblength);
        fprintf(outfp, " pattern probability=%.1e\nlengthXprobability=%.1e\n", 
                pattern_info->probability, lenXprob);
        fprintf(outfp, 
                "\nNumber of occurrences of pattern in the database is %lld\n",
                db_patterns);
        fprintf(outfp, "WARNING: There may be more matching sequences with "
                "e-values below the threshold of %f\n", 
                sum_returns->search_params->expect);

    }
    return 0;
}

Int2 PHIBlastFormatResults(EBlastProgramType program, ValNode* phivnps,
                           char* blast_database, SeqLocPtr query_slp,
                           const BlastFormattingOptions* format_options, 
                           Blast_SummaryReturn* sum_returns)
{  
   Boolean query_is_na, db_is_na;
   BioseqPtr query_bsp = NULL;
   FILE *outfp = NULL;
   ValNodePtr pruneSeed = NULL;
   Uint1 featureOrder[FEATDEF_ANY];
   Uint1 groupOrder[FEATDEF_ANY];
   SeqLoc* seed_seqloc = NULL; /* SeqLoc containing pattern locations. */ 

   if (!format_options || !format_options->outfp || !query_slp)
      return -1;

   outfp = format_options->outfp;

   s_PHIBlastFormatPatternInfo(sum_returns, outfp);

   /* Old toolkit might have different values for program numbers, so 
      use old toolkit function to determine alignment type. */
   if (program == eBlastTypePhiBlastn)
       query_is_na = db_is_na = TRUE;
   else
       query_is_na = db_is_na = FALSE;

   if (blast_database)
      ReadDBBioseqFetchEnable ("blast", blast_database, db_is_na, TRUE);

   pruneSeed = 
       SeedPruneHitsFromSeedReturn(phivnps,
                                   format_options->number_of_descriptions);

   s_PHIBlastCreateSeedSeqLoc(sum_returns->pattern_info, query_slp, 
                              &seed_seqloc);

   PrintDefLinesExtra(phivnps, 80, outfp, format_options->print_options,
                      FIRST_PASS, NULL, seed_seqloc);

   if (format_options->number_of_alignments < 
       format_options->number_of_descriptions) {
       pruneSeed = 
           SeedPruneHitsFromSeedReturn(pruneSeed,
                                       format_options->number_of_alignments);
   }

   query_bsp = BioseqLockById(SeqLocId(query_slp));
   memset(featureOrder, 0, sizeof(featureOrder));
   memset(groupOrder, 0, sizeof(groupOrder));
   featureOrder[FEATDEF_REGION] = 1;
   groupOrder[FEATDEF_REGION] = 1;

   if (format_options->align_view != 0) {
       ShowTextAlignFromAnnotExtra(query_bsp, pruneSeed, seed_seqloc, 60, outfp,
                                   featureOrder, groupOrder,
                                   format_options->align_options, NULL, NULL, 
                                   NULL);
   } else {
       ShowTextAlignFromAnnotExtra(query_bsp, pruneSeed, seed_seqloc, 60, outfp,
                                   featureOrder, groupOrder,
                                   format_options->align_options, NULL, NULL, 
                                   FormatScoreFunc);
   }

   SeqLocSetFree(seed_seqloc);

   if (blast_database)
      ReadDBBioseqFetchDisable();

   return 0;
}

ValNode* PHIBlastResultsFree(ValNode* phivnps)
{
    ValNode* vnp;

    for (vnp = phivnps; vnp; vnp = vnp->next)
        SeqAlignSetFree((SeqAlign*) vnp->data.ptrvalue);
    ValNodeFree(phivnps);
    return NULL;
}

Int2 Blast_PrintOutputFooter(EBlastProgramType program_number, 
        const BlastFormattingOptions* format_options, 
        char* dbname, const Blast_SummaryReturn* sum_returns) 
{
   FILE *outfp;
   char* params_buffer=NULL;

   if (!format_options || !format_options->outfp)
      return -1;

   if (format_options->align_view >= 7)
      return 0;

   outfp = format_options->outfp;

   if(format_options->html) 
      fprintf(outfp, "<PRE>\n");
   init_buff_ex(85);

   if (dbname) {
       const Boolean kDbIsProt = 
           (program_number == eBlastTypeBlastp    ||
            program_number == eBlastTypeBlastx    ||
            program_number == eBlastTypePhiBlastp ||
            program_number == eBlastTypePsiBlast  ||
            program_number == eBlastTypeRpsBlast  ||
            program_number == eBlastTypeRpsTblastn);
       ReadDBFILE* rdfp = readdb_new(dbname, kDbIsProt);
       TxDfDbInfo* dbinfo_head,* dbinfo;
       dbinfo_head = Blast_GetDbInfo(rdfp);
       rdfp = readdb_destruct(rdfp);

       for (dbinfo = dbinfo_head; dbinfo; dbinfo = dbinfo->next) {
           PrintDbReport((TxDfDbInfoPtr) dbinfo, 70, outfp);
       }
       dbinfo_head = TxDfDbInfoDestruct(dbinfo_head);
   }

      
   if (sum_returns->ka_params)
   {
      BLAST_KAParameters* ka_params=sum_returns->ka_params;
      PrintKAParameters(ka_params->Lambda, ka_params->K, ka_params->H, 70, 
                           outfp, FALSE);
   }

 
   if (sum_returns->ka_params_gap && sum_returns->search_params->gapped_search)
   {
      BLAST_KAParameters* ka_params=sum_returns->ka_params_gap;
      if (program_number == eBlastTypePhiBlastp ||
          program_number == eBlastTypePhiBlastn) {
          PrintKAParametersExtra(ka_params->Lambda, ka_params->K, ka_params->H, 
                                 ka_params->C, 70, outfp, TRUE);
      } else {
          PrintKAParameters(ka_params->Lambda, ka_params->K, ka_params->H, 70, 
                            outfp, TRUE);
      }
   }
   params_buffer = 
      Blast_GetParametersBuffer(program_number, sum_returns);
   PrintTildeSepLines(params_buffer, 70, outfp);
   sfree(params_buffer);
   free_buff();

   return 0;
}

Int2 BLAST_PrintOutputHeader(const BlastFormattingOptions* format_options,
        Boolean is_megablast, const char* program_name, char* dbname, Boolean is_protein)
{
   Int2 status = 0;

   if (format_options->align_view < 7) {
      if (format_options->html) {
         fprintf(format_options->outfp, 
                 "<HTML>\n<TITLE>BLAST Search Results</TITLE>\n");
         fprintf(format_options->outfp, 
                 "<BODY BGCOLOR=\"#FFFFFF\" LINK=\"#0000FF\" "
                 "VLINK=\"#660099\" ALINK=\"#660099\">\n");
         fprintf(format_options->outfp, "<PRE>\n");
      }
      init_buff_ex(90);
      if (is_megablast) {
         BlastPrintVersionInfo("MEGABLAST", format_options->html, format_options->outfp);
         fprintf(format_options->outfp, "\n");
         MegaBlastPrintReference(format_options->html, 90, format_options->outfp);
      }
      else if (strcmp(program_name, "rpsblast") == 0 ||
               strcmp(program_name, "rpstblastn") == 0) {
         BlastPrintVersionInfo("RPS-BLAST", format_options->html, format_options->outfp);
      }
      else {
         BlastPrintVersionInfo(program_name, format_options->html, format_options->outfp);
         fprintf(format_options->outfp, "\n");
         BlastPrintReference(format_options->html, 90, format_options->outfp);
      }

      if(dbname) { 
         status = 
            PrintDbInformation(dbname, is_protein, 70, 
               format_options->outfp, format_options->html);
      }
      free_buff();
	
#ifdef OS_UNIX
      fprintf(format_options->outfp, "%s", "Searching\n\n");
#endif
   }

   return status;
}

#ifdef BUFFER_LENGTH
#undef BUFFER_LENGTH
#endif

/** Buffer length for printing sequence ids. */
#define BUFFER_LENGTH 256

void 
Blast_SeqIdGetDefLine(SeqIdPtr sip, char** buffer_ptr, Boolean ncbi_gi, Boolean accession_only)
{
   char* seqid_buffer = NULL;
   Int4 gi;
   Boolean numeric_id_type = FALSE;

   *buffer_ptr = NULL;

   if (sip == NULL)
	return;

   /* Check for ad hoc ID's generated by formatdb if the user does not provide any. */
   if (sip->choice != SEQID_GENERAL ||
       StringCmp(((DbtagPtr)sip->data.ptrvalue)->db, "BL_ORD_ID"))  
   {
      if ((!accession_only && !ncbi_gi) || sip->choice == SEQID_LOCAL) {
         seqid_buffer = (char*) malloc(BUFFER_LENGTH + 1);
         SeqIdWrite(sip, seqid_buffer, PRINTID_FASTA_LONG, BUFFER_LENGTH);
      } else if (accession_only) {
         seqid_buffer = (char*) malloc(BUFFER_LENGTH + 1);
         SeqIdWrite(SeqIdFindBestAccession(sip), seqid_buffer, 
                    PRINTID_TEXTID_ACC_VER, BUFFER_LENGTH);
      } else if (ncbi_gi) {
         numeric_id_type = 
            GetAccessionFromSeqId(SeqIdFindBest(sip, SEQID_GI), 
                                  &gi, &seqid_buffer);
      } else {
         numeric_id_type = 
	    GetAccessionFromSeqId(SeqIdFindBestAccession(sip), 
                                  &gi, &seqid_buffer);
      }
   }

   if (numeric_id_type && gi > 0) {
      seqid_buffer = (char*) malloc(16);
      sprintf(seqid_buffer, "%ld", (long) gi);
   }   
   if (!seqid_buffer) {
      /* If it's still NULL make a last ditch effort to get info. */
      char* title=NULL;
      BioseqPtr bsp = BioseqLockById(sip);
      if (bsp)
         title = strdup(BioseqGetTitle(bsp));
      BioseqUnlock(bsp);
      
      if (title) /* Use first token as id. */
         seqid_buffer = StringTokMT(title, " \t\n\r", &title);  
   }
   *buffer_ptr = seqid_buffer;

}

Int2 
Blast_SummaryReturnsPostError(Blast_SummaryReturn* sum_return)
{
    Blast_Message* error = NULL;

    if (!sum_return)
        return -1;

    /* If there was no error, there is nothing to post. */
    if (!sum_return->error)
        return 0;

    error = sum_return->error;

    ErrPostEx(error->severity, error->code, error->subcode, error->message);

    return 0;
}
/* @} */

