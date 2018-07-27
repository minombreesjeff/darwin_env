/* $Id: wwwblast.c,v 1.15 2004/05/03 15:11:46 dondosha Exp $
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
* File Name:  $RCSfile: wwwblast.c,v $
*
* Author:  Sergei Shavirin
*
* Initial Creation Date: 03/15/2000
*
* $Revision: 1.15 $
*
* File Description:
*        Standalone WWW Blast CGI program.
*
* $Log: wwwblast.c,v $
* Revision 1.15  2004/05/03 15:11:46  dondosha
* Added support for masking query in XML output
*
* Revision 1.14  2004/03/02 16:44:11  dondosha
* Do not print closing </form> for XML output
*
* Revision 1.13  2004/01/16 17:35:20  dondosha
* Fixed mouseover problems
*
* Revision 1.12  2003/11/20 22:19:35  dondosha
* Pass www_root_path to the PrintDefLines... function
*
* Revision 1.11  2003/11/20 20:10:52  dondosha
* Decide whether to print header only after the output type is determined
*
* Revision 1.10  2003/11/20 19:10:33  dondosha
* Do not print progress messages if XML output requested
*
* Revision 1.9  2003/11/05 22:40:28  dondosha
* Do not shift coordinates for tabular output if query is a subsequence - they are already shifted in the seqalign
*
* Revision 1.8  2003/06/04 16:12:51  dondosha
* Set db genetic code for formatting
*
* Revision 1.7  2003/05/13 22:00:37  dondosha
* More changes to make alternative links work
*
* Revision 1.6  2003/05/13 21:17:01  dondosha
* Pass www_blast_type to txalign in all cases
*
* Revision 1.5  2003/05/09 21:09:05  dondosha
* Removed unused variables
*
* Revision 1.4  2003/05/02 23:04:03  dondosha
* Fixed paths for non-standard setups, removed blast_form.map
*
* Revision 1.3  2003/04/29 15:55:56  dondosha
* Always use root path in links
*
* Revision 1.2  2003/02/10 21:48:53  dondosha
* Added support for multi-query XML output
*
* Revision 1.1  2002/12/02 18:05:53  dondosha
* Moved from different locations to a common one
*
* Revision 6.32  2002/06/19 22:50:17  dondosha
* Added all queries information for tabular output with multiple queries
*
* Revision 6.31  2002/06/18 21:14:28  dondosha
* Added return statement in the end of AppendMegaBlastHit
*
* Revision 6.30  2002/01/08 22:36:25  dondosha
* Added tabular output functionality
*
* Revision 6.29  2000/11/29 16:11:31  dondosha
* If multiple queries, send proper lower case mask to the search engine
*
* Revision 6.28  2000/11/17 14:40:04  dondosha
* Changed MegaBlastResultsNew call to MemNew
*
* Revision 6.27  2000/11/16 22:37:07  dondosha
* Added endpoint Mega BLAST results handling
*
* Revision 6.26  2000/10/31 20:18:52  shavirin
* Added printing of correct XML output when no hits found
* Added printing progeress messages to avoid browser timeout.
*
* Revision 6.25  2000/10/23 20:19:57  dondosha
* Open and close AsnIo outside calls to BXMLPrintOutput function
*
* Revision 6.24  2000/10/18 20:19:31  shavirin
* Added title for OOF Blastx.
*
* Revision 6.23  2000/10/16 22:18:17  shavirin
* Added possibility to perform OOF blastx
*
* Revision 6.22  2000/10/16 20:27:03  shavirin
* Added possibility to run RPS Blast.
*
* Revision 6.21  2000/09/28 16:48:20  dondosha
* Changed MegaBlast related code to get a single SeqAlignPtr from server
*
* Revision 6.20  2000/09/28 15:16:55  shavirin
* Added message if request was limited to results of Entrez query.
*
* Revision 6.19  2000/09/27 22:17:04  shavirin
* Added possibility to limit search to results of entrez query.
*
* Revision 6.18  2000/09/13 22:28:10  dondosha
* Removed extra </PRE> that is now printed in PrintDefLinesFromSeqAlign
*
* Revision 6.17  2000/09/13 20:47:51  dondosha
* Small cleanup with closures of html blocks
*
* Revision 6.16  2000/09/12 21:57:28  dondosha
* Pass the correct scoring matrix to ShowTextAlignFromAnnot
*
* Revision 6.15  2000/09/11 17:51:07  shavirin
* Removed redundant <PRE> tag.
*
* Revision 6.14  2000/09/08 20:18:12  dondosha
* Print the title, background and GIF image before creating options
*
* Revision 6.13  2000/09/08 14:49:28  dondosha
* Allow graphical overview with multiple queries
*
* Revision 6.12  2000/09/07 18:01:38  dondosha
* Pass a callback to the server from TraditionalBlastReportEngineWithImage; allow multiple queries for all BLAST searches
*
* Revision 6.11  2000/09/05 18:00:25  dondosha
* Added query acknowledgement for each query in WWWBlastDoSearch for megablast
*
* Revision 6.10  2000/09/01 17:55:14  dondosha
* Call SeqEntryLoad at the beginning; corrections for megablast page
*
* Revision 6.9  2000/08/30 22:22:35  dondosha
* Enhance function WWWBlastDoSearch to handle megablast search
*
* Revision 6.8  2000/08/28 20:20:59  dondosha
* Added functionality for megablast web page
*
* Revision 6.7  2000/08/09 20:30:30  shavirin
* Added possibility to print XML output.
*
* Revision 6.6  2000/07/31 20:44:12  shavirin
* Minor change (initialized variable) from Haruna Cofer (haruna@detroit.sgi.com)
*
* Revision 6.5  2000/05/17 15:50:51  shavirin
* Moved many functions to the wwwbutl.c file.
*
* Revision 6.4  2000/04/21 18:10:59  shavirin
* Added possibility to print Patrick's alignment.
*
* Revision 6.3  2000/03/28 14:44:20  shavirin
* Changed function ctime_r to ctime() for compatibility.
*
* Revision 6.2  2000/03/24 16:05:37  shavirin
* Added option to be used as NCBI client/server.
*
* Revision 6.1  2000/03/20 19:01:00  shavirin
* Initial revision.
*
*
* ==========================================================================
*/

#include <wwwblast.h>
#include <xmlblast.h>

static int LIBCALLBACK WWWTickCallback(Int4 sequence_number, 
                                       Int4 number_of_positive_hits)
{
    if(!TestSTDOut()) {
	return -1;
    }
    
    /*    fprintf(stdout, "."); */

    printf("<!-- Progress msg from the server %d %d-->\n", 
           sequence_number, number_of_positive_hits);

    fflush(stdout);
    
    return 1;
}

/* Callback in case of XML output should not print anything to the stdout */
static int LIBCALLBACK WWWXMLTickCallback(Int4 sequence_number, 
                                          Int4 number_of_positive_hits)
{
   if(!TestSTDOut()) {
      return -1;
   }
   fflush(stdout);
   
   return 1;
}

static Int4 get_number_alignment(SeqAlignPtr align)
{
    Int4 num = 0;

    while(align)
    {
	++num;
	align = align->next;
    }

    return num;
}
static void
PrintMotd(CharPtr string, FILE *fp, Boolean html_format)

{
    Char buffer[100];
    CharPtr ptr;
    
    if (string == NULL)
        return;
    
    buffer[0] = NULLB;
    ptr = buffer;
    
    if (html_format) {
        fprintf(fp, "<PRE>\n");
    }
    
    while (*string != NULLB) {
        if (*string == '~') {
            *ptr = NULLB;
            fprintf(fp, "%s\n", buffer);
            buffer[0] = NULLB;
            ptr = buffer;
            string++;
            if (*string == NULLB)
                break;
        } else {
            *ptr=*string;
            ptr++;  string++;
        }
    }
    *ptr = NULLB;
    fprintf(fp, "%s\n", buffer);
    
    if (html_format) {
        fprintf(fp, "</PRE>\n");
    }
    
    fflush(fp);

    return;
}
#ifdef NCBI_CLIENT_SERVER

static  Boolean LIBCALLBACK
callback (BlastResponsePtr brp, Boolean PNTR cancel)

{
        fprintf(stdout, "</PRE>\n<PRE>");
	return TRUE;
}

static Boolean
TraditionalBlastReportEngineWithImage(SeqLocPtr slp, BioseqPtr bsp, BlastNet3Hptr bl3hp, WWWBlastInfoPtr theInfo)

{
    BlastDbinfoPtr dbinfo;
    BlastKABlkPtr ka_params=NULL, ka_params_gap=NULL;
    BlastPruneSapStructPtr prune;
    BLAST_MatrixPtr matrix;
    Int4Ptr PNTR txmatrix;
    Boolean query_is_na, db_is_na;
    Boolean status;
    CharPtr params_buffer=NULL;
    Int4 number_of_hits_private=0, length;
    SeqAlignPtr seqalign = NULL, sap, next_seqalign;
    SeqAnnotPtr seqannot=NULL;
    TxDfDbInfoPtr tx_dbinfo=NULL, tx_dbinfo_head;
    ValNodePtr mask_loc = NULL, mask_loc_start, next_mask_loc = NULL;
    SeqLocPtr mask_slp = NULL;
    ValNodePtr other_returns, error_returns, vnp, vnp1=NULL;
    Uint1 align_type;
    Uint1 f_order[FEATDEF_ANY], g_order[FEATDEF_ANY];
    /* Variables for multiple query output */
    SeqLocPtr tmp_slp;
    Boolean done = TRUE;
    BLAST_OptionsBlkPtr options = theInfo->options;
    CharPtr program = theInfo->program, database = theInfo->database;
    Uint4 align_options = theInfo->align_options;
    DenseSegPtr dsp, next_dsp;
    AsnIoPtr xml_aip;
    MegaBlastResultsPtr mb_results = NULL;
    Boolean tabular_output = (theInfo->align_view == HitTable || 
                              theInfo->align_view == HitTableWithHeader);
    
    MemSet((Pointer)(g_order), 0, (size_t)(FEATDEF_ANY* sizeof(Uint1)));
    MemSet((Pointer)(f_order), 0, (size_t)(FEATDEF_ANY* sizeof(Uint1)));
    
    if (bsp == NULL && slp == NULL)
        return FALSE;
    
    if (bl3hp == NULL || program == NULL || database == NULL)
        return FALSE;
    
    align_type = BlastGetTypes(program, &query_is_na, &db_is_na);
    
    init_buff_ex(85);
    dbinfo = BlastRequestDbInfo(bl3hp, database, !db_is_na);
    
    if (dbinfo && !tabular_output && 
        !(options->is_megablast_search && options->no_traceback))
        PrintDbInformationBasic(database, !db_is_na, 70, dbinfo->definition, dbinfo->number_seqs, dbinfo->total_length, stdout, TRUE);
    dbinfo = BlastDbinfoFree(dbinfo);
    free_buff();
    
    if (bsp) 
        seqalign = BlastBioseqNetCore(bl3hp, bsp, program, database, options, &other_returns, &error_returns, callback, NULL, &status);
    else if (options->is_megablast_search) {
        seqalign = MegaBlastSeqLocNetCore(bl3hp, slp, program, database, options, &other_returns, &error_returns, callback, &status);
    } else
        seqalign = BlastSeqLocNetCore(bl3hp, slp, program, database, options, &other_returns, &error_returns, callback, NULL, &status);
    
    
    BlastErrorPrintExtra(error_returns, TRUE, stdout);
    
    mask_loc = next_mask_loc = NULL;
    matrix = NULL;
    txmatrix = NULL;
    for (vnp=other_returns; vnp; vnp = vnp->next) {
        switch (vnp->choice) {
           case BlastResponse_mbalign:
           mb_results = (MegaBlastResultsPtr) vnp->data.ptrvalue;
           break;
        case TXDBINFO:
	    tx_dbinfo = (TxDfDbInfoPtr) vnp->data.ptrvalue;
            break;
        case TXKABLK_NOGAP:
            ka_params = (BlastKABlkPtr) vnp->data.ptrvalue;
            break;
        case TXKABLK_GAP:
	    ka_params_gap = (BlastKABlkPtr) vnp->data.ptrvalue;
            break;
        case TXPARAMETERS:
	    params_buffer = (CharPtr) vnp->data.ptrvalue;
            break;
        case TXMATRIX:
            matrix = (BLAST_MatrixPtr) vnp->data.ptrvalue;
            if (matrix)
                txmatrix = BlastMatrixToTxMatrix(matrix);
            /*BLAST_MatrixDestruct(matrix);*/
            break;
        case SEQLOC_MASKING_NOTSET:
        case SEQLOC_MASKING_PLUS1:
        case SEQLOC_MASKING_PLUS2:
        case SEQLOC_MASKING_PLUS3:
        case SEQLOC_MASKING_MINUS1:
        case SEQLOC_MASKING_MINUS2:
        case SEQLOC_MASKING_MINUS3:
            ValNodeAddPointer(&mask_loc, vnp->choice, vnp->data.ptrvalue);
	    break;
        default:
            break;
        }
    }	
    
    mask_loc_start = mask_loc;

    /* If results come as alignment endpoints only from Mega BLAST */
    if (mb_results) {
       MegaBlastHitPtr mb_hit = mb_results->mbhits, next_hit;

       while (mb_hit) {
          fprintf(stdout, "%s\t%s\t%d\t%d\t%d\t%d\t%d\n", mb_hit->id1,
                  mb_hit->id2, mb_hit->query_offset, mb_hit->subject_offset,
                  mb_hit->query_end, mb_hit->subject_end, mb_hit->score);
          mb_hit = mb_hit->next;
       }
       MegaBlastResultsFree(mb_results);
    } else if (!seqalign) {
       done = FALSE;
    }
    
    ReadDBBioseqFetchEnable ("blastall", database, db_is_na, TRUE);
    ReadDBBioseqSetDbGeneticCode(theInfo->options->db_genetic_code);
   
    tmp_slp = slp;
    
    if(theInfo->xml_output)
        xml_aip = AsnIoOpen("stdout", "wx");

    if (tabular_output) {
       if (theInfo->align_view == HitTableWithHeader)
          PrintTabularOutputHeader(database, bsp, slp,
              (options->is_megablast_search ? "megablast" : program),
              0, FALSE, stdout);

       BlastPrintTabulatedResults(seqalign, bsp, slp, 
                                  theInfo->number_of_alignments,
                                  program, 
                                  !theInfo->options->gapped_calculation,
                                  FALSE, 0, 0, stdout,
                                  (theInfo->align_view == HitTableWithHeader));
       SeqAlignSetFree(seqalign);
    } else {
    Int4 query_number = 1;
    while (seqalign) {
        if (!options->is_megablast_search)
            next_seqalign = NULL;
        else {
            SeqIdPtr seqid = TxGetQueryIdFromSeqAlign(seqalign); 
            sap = seqalign;
            while (sap != NULL) { 
                if (sap->next != NULL) {
                    if (SeqIdComp(seqid, TxGetQueryIdFromSeqAlign(sap->next) 
                                  != SIC_YES)) {
                        next_seqalign = sap->next;
                        sap->next = NULL;
                    }
                } else {
                    next_seqalign = NULL;
                }
                sap = sap->next;
            }
            /* Find this query's SeqLoc and Bioseq */
            while (tmp_slp && SeqIdComp(seqid, SeqLocId(tmp_slp)) != SIC_YES)
                tmp_slp = tmp_slp->next;
            if (tmp_slp == NULL) /* Should never happen */
                break;
            bsp = BioseqLockById(SeqLocId(tmp_slp));
            init_buff_ex(85);
            fprintf(stdout, "<HR><BR>");
            AcknowledgeBlastQuery(bsp, 70, stdout, FALSE, TRUE);
            free_buff();
            BioseqUnlock(bsp);

            /* Separate the mask locations list for this query. */
            if (mask_loc) {
                mask_slp = (SeqLocPtr) mask_loc->data.ptrvalue;
                while (SeqIdComp(SeqLocId(mask_slp), seqid) != SIC_YES) {
                    mask_loc = mask_loc->next;
                    if (!mask_loc)
                        break;
                    mask_slp = (SeqLocPtr) mask_loc->data.ptrvalue;
                }
                if (mask_loc) {
                    next_mask_loc = mask_loc->next;
                    mask_loc->next = NULL;
                }
            }
        }
        
        if(theInfo->xml_output) {
            printf("<PRE>");
            BXMLPrintOutput(xml_aip, seqalign, options, 
                            program, database, 
                            bsp, other_returns, 0, NULL, mask_loc);
            AsnIoReset(xml_aip);
            printf("</PRE>");
            
        } else {
            
            seqannot = SeqAnnotNew();
            seqannot->type = 2;
            AddAlignInfoToSeqAnnot(seqannot, align_type);
            seqannot->data = seqalign;
            
            if(theInfo->show_overview) {
                Char f_name[64], title[1024], href[64], form_name[16];
                Int4 align_num;       
                
                sprintf(f_name, "%ld%ld.gif", (long)random(), (long)getpid());
                sprintf(href, "nph-viewgif.cgi?");
                
                align_num = get_number_alignment(seqalign); 
                sprintf(title, 
                        "<H3><a href=\"%s/blast/docs/newoptions.html#graphical-overview\"> "
                        "Distribution of %ld Blast Hits on the Query Sequence</a> "
                        "</H3>\n", theInfo->www_root_path, (long)align_num);
                
                /* Open HTML form */
                sprintf(form_name, "BLASTFORM%ld", query_number);
                fprintf(stdout, "<FORM NAME=\"%s\">\n", form_name);
                fflush(stdout);
                
                PrintOneAlignmentOverview(seqannot, stdout, form_name, href, f_name,
                                          title, query_number);
                ++query_number;
            }
            
            prune = BlastPruneHitsFromSeqAlign(seqalign, theInfo->number_of_descriptions, NULL);
            ObjMgrSetHold();
            init_buff_ex(85);
            
            PrintDefLinesFromSeqAlignWithPath(prune->sap, 80, stdout, 
               theInfo->print_options, FIRST_PASS, NULL, 
               theInfo->number_of_descriptions, database, 
               theInfo->www_blast_type, theInfo->www_root_path);
            free_buff();
            
            prune = BlastPruneHitsFromSeqAlign(seqalign, theInfo->number_of_alignments, prune);
            seqannot->data = prune->sap;
            
            if(theInfo->color_schema != 0 && 
               (!StringICmp(program, "blastn") || 
                !StringICmp(program, "blastp"))) {
                
                if(!DDV_DisplayBlastPairList(prune->sap, mask_loc, stdout, 
                                             query_is_na, align_options, 
                                             theInfo->color_schema)) { 
                    fprintf(stdout, 
                            "\n\n!!!\n   "
                            "    --------  Failure to print alignment...  --------"
                            "\n!!!\n\n");
                    fflush(stdout);
                }
            } else {
                
                if(options->is_ooframe) {
                    printf("<PRE>");
                    OOFShowBlastAlignment(seqalign, /*mask*/ NULL,
                                          stdout, align_options, txmatrix);
                } else {
                    if (align_options & TXALIGN_MASTER) {
                        ShowTextAlignFromAnnot2(seqannot, 60, stdout, f_order,
                           g_order, align_options, txmatrix, mask_loc, NULL, 
                           NULL, theInfo->www_blast_type);
                    } else {
                        ShowTextAlignFromAnnot2(seqannot, 60, stdout, f_order, 
                           g_order, align_options, txmatrix, mask_loc, 
                           FormatScoreFunc, NULL, theInfo->www_blast_type);
                    }
                }
            }
            
            seqannot->data = seqalign;
            number_of_hits_private = prune->original_number; 
            prune = BlastPruneSapStructDestruct(prune);
            ObjMgrClearHold();
            ObjMgrFreeCache(0);
        } /* If else xml_output */
        
        if (options->is_megablast_search) {
            tmp_slp = tmp_slp->next;

            /* Reconnect the mask locations */
            if (next_mask_loc) {
                mask_loc->next = next_mask_loc;
                mask_loc = next_mask_loc;
            } 
        }

        if (seqannot)
            seqannot = SeqAnnotFree(seqannot);
        seqalign = next_seqalign;
        fprintf(stdout, "<PRE>\n</form>\n");
    }
    } /* End if not hit table */ 
    if (!done) { /* seqalign == NULL */
    if(theInfo->xml_output && !options->is_ooframe) {
       BlastErrorMsgPtr error_msg;
       CharPtr message;
       
       if (error_returns == NULL) {
                message = "No hits found";
       } else {
          error_msg = error_returns->data.ptrvalue;
          message = error_msg->msg;
       }
       
       BXMLPrintOutput(xml_aip, NULL, 
                       options, program, database, 
                       bsp, other_returns, 0, message, NULL);
       
       if (error_returns != NULL) {
          MemFree(error_msg->msg);
          MemFree(error_msg);
          MemFree(error_returns);
       }
       
       AsnIoReset(xml_aip);
    } else if (!tabular_output) {
       fprintf(stdout, "\n\n ***** No hits found ******\n\n");
    }
    }
    if(theInfo->xml_output)
        xml_aip = AsnIoClose(xml_aip);
    
    BLAST_MatrixDestruct(matrix);
    if (txmatrix)
        txmatrix = TxMatrixDestruct(txmatrix);
    
    for (mask_loc = mask_loc_start; mask_loc; mask_loc = mask_loc->next) {
        SeqLocSetFree(mask_loc->data.ptrvalue);
    }
    ValNodeFree(mask_loc_start);
               
    init_buff_ex(85);
    tx_dbinfo_head = tx_dbinfo;
    if (!tabular_output && 
        !(options->is_megablast_search && options->no_traceback)) {
        while (tx_dbinfo) {
            PrintDbReport(tx_dbinfo, 70, stdout);
            tx_dbinfo = tx_dbinfo->next;
        }
    }
    tx_dbinfo_head = TxDfDbInfoDestruct(tx_dbinfo_head);
    
    if (ka_params) {
        if (!tabular_output && 
            !(options->is_megablast_search && options->no_traceback))
            PrintKAParameters(ka_params->lambda, ka_params->k, ka_params->h, 
                              70, stdout, FALSE);
            MemFree(ka_params);
    }
    if (ka_params_gap) {
        if (!tabular_output && 
            !(options->is_megablast_search && options->no_traceback))
            PrintKAParameters(ka_params_gap->lambda, ka_params_gap->k, ka_params_gap->h, 70, stdout, TRUE);
        MemFree(ka_params_gap);
    }
    
    if (!tabular_output && 
        !(options->is_megablast_search && options->no_traceback))
        PrintTildeSepLines(params_buffer, 70, stdout);
    
    printf("</PRE></BODY></HTML>\n");
    MemFree(params_buffer);
    free_buff();
    
    other_returns = ValNodeFree(other_returns);
    
    return status;
}

static Boolean
TraditionalBlastReportWithImage(BioseqPtr bsp, BlastNet3Hptr bl3hp, 
                                WWWBlastInfoPtr theInfo)
{
   return TraditionalBlastReportEngineWithImage(NULL, bsp, bl3hp, theInfo);
}
static Boolean
TraditionalBlastReportLocWithImage(SeqLocPtr slp, BlastNet3Hptr bl3hp,
                                   WWWBlastInfoPtr theInfo)
{
   return TraditionalBlastReportEngineWithImage(slp, NULL, bl3hp, theInfo);
}

Boolean WWWBlastDoClientSearch(WWWBlastInfoPtr theInfo)
{
    BlastNet3Hptr	bl3hp;
    BlastResponsePtr	response;
    BlastVersionPtr	blast_version;
    CharPtr		date, motd, version;
    Boolean status;
    
    if(theInfo == NULL)
	return FALSE;
        
    if (!BlastInit("blastcl3", &bl3hp, &response)) {
        WWWBlastErrMessage(BLASTErrClient, NULL);                
        return FALSE;
    }
    
    if (response && response->choice == BlastResponse_init) {
        blast_version = response->data.ptrvalue;
        version = blast_version->version;
        date = blast_version->date;
    } else {
        WWWBlastErrMessage(BLASTErrClient, NULL);                
        return FALSE;
    }

    BlastNetBioseqFetchEnable(bl3hp, theInfo->database, 
                              theInfo->db_is_na, TRUE);
    
#ifdef BLAST_PRINT_MOTD
    motd = Blast3GetMotd(bl3hp);
    PrintMotd(motd, stdout, TRUE);
    motd = MemFree(motd);
#endif
    
    if (!theInfo->options->is_megablast_search)
       status = TraditionalBlastReportWithImage(theInfo->fake_bsp, bl3hp, theInfo);
    else 
       status = TraditionalBlastReportLocWithImage(theInfo->query_slp, bl3hp, theInfo);
    if (status == FALSE) {
        WWWBlastErrMessage(BLASTErrServer, NULL);
        return FALSE;
    }
    
    return TRUE;
}
#endif

#define BUFFER_LENGTH 255

static int LIBCALLBACK
AppendMegaBlastHit(VoidPtr ptr)
{
   BlastSearchBlkPtr search = (BlastSearchBlkPtr) ptr;
   SeqIdPtr subject_id, query_id;
   CharPtr subject_descr, subject_buffer;
   Int4 index, query_length;
   Int2 context;
   BLAST_HSPPtr hsp;
   MegaBlastResultsPtr mb_results;
   MegaBlastHitPtr new_hit, last_hit = NULL;

   if (search->current_hitlist == NULL || search->current_hitlist->hspcnt <= 0) {
      search->subject_info = BLASTSubjectInfoDestruct(search->subject_info);
      return 0;
   }

   mb_results = (MegaBlastResultsPtr)
      search->mb_endpoint_results->data.ptrvalue;
   if (mb_results) {
       for (last_hit = mb_results->mbhits; last_hit->next; 
            last_hit = last_hit->next);
   }
   readdb_get_descriptor(search->rdfp, search->subject_id, &subject_id,
			 &subject_descr);
   if (subject_id->choice != SEQID_GENERAL) {
      subject_buffer = (CharPtr) Malloc(BUFFER_LENGTH + 1);
      SeqIdWrite(SeqIdFindBestAccession(subject_id), subject_buffer,
                 PRINTID_TEXTID_ACC_VER, BUFFER_LENGTH);
   } else {
      subject_buffer = StringTokMT(subject_descr, " ", &subject_descr);
      subject_descr = subject_buffer;
   }

   for (index=0; index<search->current_hitlist->hspcnt; index++) {
      hsp = search->current_hitlist->hsp_array[index];
      if (hsp==NULL || (search->pbp->cutoff_e > 0 && 
	  hsp->evalue > search->pbp->cutoff_e)) 
	 continue;
      new_hit = Malloc(sizeof(MegaBlastHit));
      new_hit->next = NULL;
      context = hsp->context; 
      query_id = search->qid_array[context/2];

      new_hit->id2 = StringSave(subject_buffer);
      if (query_id->choice == SEQID_LOCAL) {
         BioseqPtr query_bsp = BioseqLockById(query_id);
         CharPtr title = StringSave(BioseqGetTitle(query_bsp));
         if (title) {
            new_hit->id1 = StringTokMT(title, " ", &title);
         } else {
            Int4 query_gi;
            Boolean numeric_query_id =
                GetAccessionFromSeqId(query_bsp->id, &query_gi,
                                      &new_hit->id1);
            if (numeric_query_id) {
                new_hit->id1 = Malloc(10);
                sprintf(new_hit->id1, "%d", query_gi);
            }
         }  
         BioseqUnlock(query_bsp);
      } else {
          new_hit->id1 = (CharPtr) Malloc(BUFFER_LENGTH + 1);
          SeqIdWrite(SeqIdFindBestAccession(query_id), new_hit->id1,
                     PRINTID_TEXTID_ACC_VER, BUFFER_LENGTH);
      }
      query_length = search->query_context_offsets[context+1] -
         search->query_context_offsets[context] - 1;
      if (context & 1) {
	 new_hit->query_end = query_length - hsp->query.offset;
	 new_hit->query_offset = 
	    new_hit->query_end - hsp->query.length + 1;
      } else {
         new_hit->query_offset = hsp->query.offset + 1;
	 new_hit->query_end = new_hit->query_offset + hsp->query.length - 1;
      }

      new_hit->subject_offset = hsp->subject.offset + 1;
      new_hit->subject_end = hsp->subject.end;
      
      if (search->pbp->gap_open==0 && search->pbp->gap_extend==0)
	 new_hit->score = ((hsp->subject.length + hsp->query.length)*
		   search->sbp->reward / 2 - hsp->score) / 
	    (search->sbp->reward - search->sbp->penalty);
      else 
	 new_hit->score = hsp->score;
      if (last_hit == NULL) {
         last_hit = new_hit;
         mb_results = MemNew(sizeof(MegaBlastResults));
         mb_results->mbhits = last_hit;
         search->mb_endpoint_results->data.ptrvalue = mb_results;
      } else {
         last_hit->next = new_hit;
         last_hit = last_hit->next;
      }
   }
   return 0;
}

Boolean WWWBlastDoSearch(WWWBlastInfoPtr theInfo)
{
    SeqAlignPtr  seqalign = NULL;
    ValNodePtr mask_loc = NULL, mask_loc_start, next_mask_loc = NULL;
    SeqLocPtr mask_slp = NULL;
    ValNodePtr vnp, other_returns, error_returns;
    TxDfDbInfoPtr dbinfo=NULL, dbinfo_head;
    BLAST_KarlinBlkPtr ka_params=NULL, ka_params_gap=NULL;
    BLAST_MatrixPtr matrix;
    Int4Ptr PNTR txmatrix;
    CharPtr params_buffer=NULL;
    SeqAnnotPtr seqannot = NULL;
    BlastPruneSapStructPtr prune;
    SeqAlignPtr PNTR seqalignp = NULL;
    Boolean is_megablast, done = FALSE, all_done = FALSE;
    SeqLocPtr query_slp, query_lcase_mask, lcase_mask;
    Int4 index;
    AsnIoPtr xml_aip;
    MegaBlastResultsPtr mb_results = NULL;
    BLAST_OptionsBlkPtr options = theInfo->options;
    Boolean tabular_output = (theInfo->align_view == HitTable || 
                              theInfo->align_view == HitTableWithHeader);
    MBXmlPtr mbxp = NULL;
    int LIBCALLBACK (*callback)(Int4, Int4);
    Int4 query_number;

    if(theInfo == NULL)
	return FALSE;
    
    is_megablast = options->is_megablast_search;

    query_slp = theInfo->query_slp;
    lcase_mask = query_lcase_mask = options->query_lcase_mask;

    if(!theInfo->xml_output && !tabular_output && 
       !(is_megablast && options->no_traceback)) {
        PrintDbInformation(theInfo->database, !theInfo->db_is_na, 
                           70, stdout, TRUE);
    }

    if(!tabular_output && options->entrez_query != NULL && 
       theInfo->gi_list_total > 0) {

        printf("Your search was limited by an Entrez query: '%s'\n"
               "<!-- %d sequences-->\n<P>\n",
               options->entrez_query, 
               theInfo->gi_list_total);
    }
    
    ReadDBBioseqFetchEnable ("blastall", theInfo->database, 
                             theInfo->db_is_na, TRUE);
    
    if(theInfo->xml_output)
       xml_aip = AsnIoOpen("stdout", "wx");
    
    while (!all_done) { /* Loop on complete BLAST searches */
        if (!query_slp || is_megablast)
            all_done = TRUE;
        
        other_returns = NULL;
        error_returns = NULL;
        
        if(!theInfo->xml_output) {
            printf("</PRE>\n");
            callback = WWWTickCallback;
        } else {
            callback = WWWXMLTickCallback;
        }

        if (!is_megablast) {
            if (SeqIdComp(SeqLocId(lcase_mask), SeqLocId(query_slp)) == 
                SIC_YES) {
                options->query_lcase_mask = (SeqLocPtr) ValNodeNew(NULL);
                MemCpy(options->query_lcase_mask, lcase_mask, sizeof(SeqLoc));
                options->query_lcase_mask->next = NULL;
                lcase_mask = lcase_mask->next;
            }
            if (query_slp) {
                seqalign = BioseqBlastEngineByLocEx(query_slp, theInfo->program, theInfo->database, options, &other_returns, &error_returns, callback, NULL, theInfo->gi_list, theInfo->gi_list_total);
            } else {
                seqalign = BioseqBlastEngineEx(theInfo->fake_bsp, theInfo->program, theInfo->database, options, &other_returns, &error_returns, callback, NULL, theInfo->gi_list, theInfo->gi_list_total);
            }
        } else {
           if (options->no_traceback)
            seqalignp = BioseqMegaBlastEngineByLoc(query_slp, theInfo->program, theInfo->database, options, &other_returns, &error_returns, callback, NULL, theInfo->gi_list, theInfo->gi_list_total, AppendMegaBlastHit);
           else
            seqalignp = BioseqMegaBlastEngineByLoc(query_slp, theInfo->program, theInfo->database, options, &other_returns, &error_returns, callback, NULL, theInfo->gi_list, theInfo->gi_list_total, NULL);
        }

        if(!theInfo->xml_output)
            printf("<PRE>");
        
        BlastErrorPrint(error_returns);
        
        dbinfo = NULL;
        ka_params = NULL;
        ka_params_gap = NULL;
        params_buffer = NULL;
        mask_loc = NULL;
        matrix = NULL;
        txmatrix = NULL;
        for (vnp=other_returns; vnp; vnp = vnp->next) {
            switch (vnp->choice) {
            case TXDBINFO:
                dbinfo = vnp->data.ptrvalue;
                break;
            case TXKABLK_NOGAP:
                ka_params = vnp->data.ptrvalue;
                break;
            case TXKABLK_GAP:
                ka_params_gap = vnp->data.ptrvalue;
                break;
            case TXPARAMETERS:
                params_buffer = vnp->data.ptrvalue;
                break;
            case TXMATRIX:
                matrix = vnp->data.ptrvalue;
                if (matrix)
                    txmatrix = (Int4Ptr PNTR) BlastMatrixToTxMatrix(matrix);
                break;
            case SEQLOC_MASKING_NOTSET:
            case SEQLOC_MASKING_PLUS1:
            case SEQLOC_MASKING_PLUS2:
            case SEQLOC_MASKING_PLUS3:
            case SEQLOC_MASKING_MINUS1:
            case SEQLOC_MASKING_MINUS2:
            case SEQLOC_MASKING_MINUS3:
                ValNodeAddPointer(&mask_loc, vnp->choice, vnp->data.ptrvalue);
                break;
            case BlastResponse_mbalign:
               mb_results = (MegaBlastResultsPtr) vnp->data.ptrvalue;
               break;
            default:
                break;
            }
        }	
        
        fflush(stdout);
        
        if (mb_results) {
           /* Results come as alignment endpoints only from Mega BLAST */
           MegaBlastHitPtr mb_hit = mb_results->mbhits, next_hit;
           
           while (mb_hit) {
              fprintf(stdout, "%s\t%s\t%d\t%d\t%d\t%d\t%d\n", mb_hit->id1,
                      mb_hit->id2, mb_hit->query_offset, mb_hit->subject_offset,
                      mb_hit->query_end, mb_hit->subject_end, mb_hit->score);
              next_hit = mb_hit->next;
              MemFree(mb_hit->id1);
              MemFree(mb_hit->id2);
              MemFree(mb_hit);
              mb_hit = next_hit;
           }
           MemFree(mb_results);
           done = TRUE;
        } else
           done = FALSE;
        
        if (theInfo->xml_output && is_megablast) {
           mbxp = PSIXmlInit(xml_aip, "megablast", theInfo->database, 
                             options, theInfo->fake_bsp, 0);
        }

        ReadDBBioseqSetDbGeneticCode(theInfo->options->db_genetic_code);
        query_number = 1;

        mask_loc_start = mask_loc;

        for (index=0; !done; index++) {
            if (is_megablast)
                seqalign = seqalignp[index];
            else 
                done = TRUE;
            if (seqalign) {
                /* Separate the mask locations list for this query, if there
                   are multiple queries. */
                if (mask_loc) {
                    SeqIdPtr seqid = TxGetQueryIdFromSeqAlign(seqalign);
                    mask_slp = (SeqLocPtr) mask_loc->data.ptrvalue;
                    while (SeqIdComp(SeqLocId(mask_slp), seqid) != SIC_YES) {
                        mask_loc = mask_loc->next;
                        if (!mask_loc)
                            break;
                        mask_slp = (SeqLocPtr) mask_loc->data.ptrvalue;
                    }
                    if (mask_loc) {
                        next_mask_loc = mask_loc->next;
                        mask_loc->next = NULL;
                    }
                }
                if(theInfo->xml_output) {
                   if (!is_megablast) {
                      BXMLPrintOutput(xml_aip, seqalign, options, 
                         theInfo->program, theInfo->database, 
                         theInfo->fake_bsp, other_returns, 0, NULL, mask_loc);
                      AsnIoReset(xml_aip);
                   } else {
                       IterationPtr iterp;
                       BioseqPtr bsp = BioseqLockById(SeqLocId(query_slp));

                       iterp = BXMLBuildOneQueryIteration(seqalign, 
                                  NULL, FALSE, 
                                  !options->gapped_calculation, index, 
                                  NULL, bsp, mask_loc);
                       IterationAsnWrite(iterp, mbxp->aip, mbxp->atp);
                       AsnIoFlush(mbxp->aip);
                       IterationFree(iterp);

                       BioseqUnlock(bsp);
                   }
                } else if (tabular_output) {
                   if (theInfo->align_view == HitTableWithHeader)
                      PrintTabularOutputHeader(theInfo->database, 
                          theInfo->fake_bsp, query_slp, 
                          (options->is_megablast_search ? 
                           "megablast" : theInfo->program),
                          0, FALSE, stdout);

                   BlastPrintTabulatedResults(seqalign, theInfo->fake_bsp, 
                        query_slp, theInfo->number_of_alignments, 
                        theInfo->program, 
                        !theInfo->options->gapped_calculation, FALSE, 
                        0, 0, stdout, 
                        (theInfo->align_view == HitTableWithHeader));
                } else {
                    seqannot = SeqAnnotNew();
                    seqannot->type = 2;
                    AddAlignInfoToSeqAnnot(seqannot, theInfo->align_type);
                    seqannot->data = seqalign;
                    
#if !defined(NCBI_CLIENT_SERVER) && defined (NCBI_ENTREZ_CLIENT)
                    if(theInfo->show_tax_blast) {
                        fprintf(stdout, "<a href=#taxblast>Taxonomy reports</a><BR>");
                    }
#endif
                    
                    if (is_megablast || query_slp) {
                        BioseqPtr bsp = BioseqLockById(SeqLocId(query_slp));
                        init_buff_ex(85);
                        fprintf(stdout, "<HR><BR>");
                        AcknowledgeBlastQuery(bsp, 70, stdout, FALSE, TRUE);
                        free_buff();
                        BioseqUnlock(bsp);
                    }
                    
                    /* Now printing nice gif with alignment overview */
                    
                    if(theInfo->show_overview) {
                        Char f_name[64], title[1024], href[64], form_name[16];
                        Int4 align_num;
                        
                        sprintf(f_name, "%ld%ld.gif", (long)random(), (long)getpid());
                        sprintf(href, "nph-viewgif.cgi?");
                        
                        align_num = get_number_alignment(seqalign); 
                        sprintf(title, 
                                "<H3><a href=\"%s/blast/docs/newoptions.html#graphical-overview\"> "
                                "Distribution of %ld Blast Hits on the Query Sequence</a> "
                                "</H3>\n", 
                                theInfo->www_root_path, (long)align_num);  
                        
                        
                        /* Open HTML form */
                        sprintf(form_name, "BLASTFORM%ld", query_number);
                        fprintf(stdout, "<FORM NAME=\"%s\">\n", form_name);
                        fflush(stdout);
                        
                        PrintOneAlignmentOverview(seqannot, stdout, form_name, href, 
                                                  f_name, title, query_number); 
                        ++query_number;
                    }
                    
                    prune = BlastPruneHitsFromSeqAlign(seqalign, theInfo->number_of_descriptions, NULL);
                    ObjMgrSetHold();
                    init_buff_ex(85);
                    PrintDefLinesFromSeqAlignWithPath(prune->sap, 80, stdout, 
                       theInfo->print_options, FIRST_PASS, NULL, -1,
                       NULL, theInfo->www_blast_type, theInfo->www_root_path);
                    free_buff();
                    
                    prune = BlastPruneHitsFromSeqAlign(seqalign, theInfo->number_of_alignments, prune);
                    seqannot->data = prune->sap;
                    
                    if(theInfo->color_schema != 0 && 
                       (!StringICmp(theInfo->program, "blastn") || 
                        !StringICmp(theInfo->program, "blastp"))) {
                        
                        if(!DDV_DisplayBlastPairList(prune->sap, mask_loc, stdout, 
                                                     theInfo->query_is_na, 
                                                     theInfo->align_options, 
                                                     theInfo->color_schema)) { 
                            fprintf(stdout, 
                                    "\n\n!!!\n   "
                                    "    --------  Failure to print alignment...  --------"
                                    "\n!!!\n\n");
                            fflush(stdout);
                        }
                    } else {

                        if(options->is_ooframe) {
                            printf("<PRE>");
                            OOFShowBlastAlignment(seqalign, /*mask*/ NULL,
                                                  stdout, theInfo->align_options, txmatrix);
                        } else {                        
                            if (theInfo->align_view != 0) {
                                ShowTextAlignFromAnnot2(seqannot, 60, stdout, 
                                   NULL, NULL, theInfo->align_options, 
                                   txmatrix, mask_loc, NULL, NULL, 
                                   theInfo->www_blast_type);
                            } else {
                                ShowTextAlignFromAnnot2(seqannot, 60, stdout, 
                                   NULL, NULL, theInfo->align_options, 
                                   txmatrix, mask_loc, FormatScoreFunc,
                                   NULL, theInfo->www_blast_type);
                            }
                        }
                    }
                    seqannot->data = NULL; /* Don't want to delete seqalign yet */
                    prune = BlastPruneSapStructDestruct(prune);
                    ObjMgrClearHold();
                    ObjMgrFreeCache(0);
                }
                /* Reconnect the mask locations, in case of multiple queries */
                if (next_mask_loc) {
                    mask_loc->next = next_mask_loc;
                    mask_loc = next_mask_loc;
                } 
            } else if (!is_megablast) { /* while(seqalign) */
                /* seqalign == NULL */
        
                if(theInfo->xml_output && !options->is_ooframe) {
                    BlastErrorMsgPtr error_msg;
                    CharPtr message;
                    
                    if (error_returns == NULL) {
                        message = "No hits found";
                    } else {
                        error_msg = error_returns->data.ptrvalue;
                        message = error_msg->msg;
                    }
                    
                    BXMLPrintOutput(xml_aip, NULL, 
                                    options, theInfo->program, 
                                    theInfo->database, 
                                    theInfo->fake_bsp, other_returns, 
                                    0, message, NULL);
                    
                    if (error_returns != NULL) {
                        MemFree(error_msg->msg);
                        MemFree(error_msg);
                        MemFree(error_returns);
                    }
                    
                    AsnIoReset(xml_aip);
                } else {
                    fprintf(stdout, "\n\n ***** No hits found ******\n\n");
                }
            }
            
            if (is_megablast) { 
                query_slp = query_slp->next;
                if (!query_slp)
                    done = TRUE;
            }
            if(!theInfo->xml_output) {
                seqannot = SeqAnnotFree(seqannot);
                fprintf(stdout, "</form>\n");
            }
        }
        
        matrix = BLAST_MatrixDestruct(matrix);
        if (txmatrix)
            txmatrix = (Int4Ptr PNTR) TxMatrixDestruct(txmatrix);
        
        dbinfo_head = dbinfo;
        if(!theInfo->xml_output && !tabular_output && 
           !(is_megablast && options->no_traceback)) {        
            fprintf(stdout, "<PRE>");
            init_buff_ex(85);
            while (dbinfo) {
                PrintDbReport(dbinfo, 70, stdout);
                dbinfo = dbinfo->next;
            }
        }
        
        dbinfo_head = TxDfDbInfoDestruct(dbinfo_head);
        
        if (ka_params) {
            if(!theInfo->xml_output && !tabular_output && 
               !(is_megablast && options->no_traceback)) {        
                PrintKAParameters(ka_params->Lambda, ka_params->K, 
                                  ka_params->H, 70, stdout, FALSE);
            }
            MemFree(ka_params);
        }
        
        if (ka_params_gap) {
            if(!theInfo->xml_output && !tabular_output &&
               !(is_megablast && options->no_traceback)) {        
                PrintKAParameters(ka_params_gap->Lambda, ka_params_gap->K,
                                  ka_params_gap->H, 70, stdout, TRUE);
            }
            MemFree(ka_params_gap);
        }
        
        if(!theInfo->xml_output && !tabular_output && 
           !(is_megablast && options->no_traceback)) {        
            
            PrintTildeSepLines(params_buffer, 70, stdout);
            
#if !defined(NCBI_CLIENT_SERVER) && defined (NCBI_ENTREZ_CLIENT)
            
            fprintf(stdout, "<HR>\n");
            
            if( !tabular_output && theInfo->show_tax_blast) {
                TXBHtmlReport(seqalign, stdout, theInfo->query_is_na,
                              theInfo->db_is_na, theInfo->database, 
                              NULL, NULL, theInfo->show_gi);
            }
            
#endif
        }
        
        if (is_megablast) {
            Int4 i;
            for (i=0; i<index; i++)
                SeqAlignSetFree(seqalignp[i]);
            MemFree(seqalignp);
        } else if(seqalign != NULL)
            seqalign = SeqAlignSetFree(seqalign);
        
        MemFree(params_buffer);
        free_buff();
        
        for (mask_loc = mask_loc_start; mask_loc; mask_loc = mask_loc->next) {
            SeqLocSetFree(mask_loc->data.ptrvalue);
        }
        ValNodeFree(mask_loc_start);
        
        other_returns = ValNodeFree(other_returns);
        
        if (!is_megablast && query_slp) {
            query_slp = query_slp->next;
            if (!query_slp)
                all_done = TRUE;
        }
    } /* End of loop over all searches */

    SeqLocSetFree(query_lcase_mask);

    if (theInfo->xml_output) {
       if (is_megablast && mbxp != NULL)
          MBXmlClose(mbxp, other_returns, !options->gapped_calculation);
    }
    ReadDBBioseqFetchDisable();

    if (!theInfo->xml_output)
       fprintf(stdout, "</PRE>\n</BODY>\n</HTML>\n");

    return TRUE;
}

static void WWWBlastPrintTopHeader(WWWBlastInfoPtr theInfo)
{
    fprintf(stdout, "<HTML>\n");
    fprintf(stdout, "<TITLE>BLAST Search Results</TITLE>\n"); 
    fflush(stdout);
    
    fprintf(stdout, "<BODY BGCOLOR=\"#FFFFFF\" LINK=\"#0000FF\" "
            "VLINK=\"#660099\" ALINK=\"#660099\">\n");
    fprintf(stdout, "<map name=img_map0>\n");
    fprintf(stdout, "<area shape=rect coords=2,1,48,21 "
            "href=\"http://www.ncbi.nlm.nih.gov\">\n");
    fprintf(stdout, "<area shape=rect coords=385,1,435,21 "
            "href=\"%s/blast/index.html\">\n", theInfo->www_root_path);
    fprintf(stdout, "<area shape=rect coords=436,1,486,21 "
            "href=\"http://www.ncbi.nlm.nih.gov/Entrez/\">\n");
    fprintf(stdout, "<area shape=rect coords=487,1,508,21 "
            "href=\"%s/blast/docs/blast_help.html\">\n", theInfo->www_root_path);
    fprintf(stdout, "</map>\n"); 
    fprintf(stdout, "<IMG USEMAP=#img_map0 WIDTH=509 HEIGHT=22 "
            "SRC=\"%s/blast/images/blast_results.gif\" ISMAP> \n", 
            theInfo->www_root_path);
}

void WWWBlastPrintHeader(WWWBlastInfoPtr theInfo)
{

    fprintf(stdout, "<PRE>\n");
    init_buff_ex(90);
    if (theInfo->options->is_megablast_search) {
        BlastPrintVersionInfo("megablast", TRUE, stdout);
    } else if (theInfo->options->is_rps_blast){
        BlastPrintVersionInfo("rps-blast", TRUE, stdout);
    } else if (theInfo->options->is_ooframe){
        BlastPrintVersionInfo("OOF BLASTX", TRUE, stdout);
    } else {
        BlastPrintVersionInfo(theInfo->program, TRUE, stdout);
        fprintf(stdout, "\n");
        BlastPrintReference(TRUE, 90, stdout);
    }
    fprintf(stdout, "\n");
    if (!theInfo->options->is_megablast_search && !theInfo->query_slp)
       AcknowledgeBlastQuery(theInfo->query_bsp, 70, stdout, 
                             theInfo->believe_query, TRUE);
    free_buff();

    return;
}

Int2 Main(void)
{
    WWWBlastInfoPtr theInfo;

    UseLocalAsnloadDataAndErrMsg ();
            
    if (! SeqEntryLoad())
        return 1;
    
    ErrSetMessageLevel(SEV_WARNING);

    /* This function will read posting data, set-up config file and
       write small message into logfile (if it exists) */
    
    if((theInfo = WWWBlastReadArgs(NULL)) == NULL)
        return 1;
    
    /* Read options into structure */
    if(!WWWCreateSearchOptions(theInfo)) {
        return 1;
    }
    
    if (!theInfo->xml_output) 
       WWWBlastPrintTopHeader(theInfo);

    /* validate them */
    if(!WWWValidateOptions(theInfo)) {
        return 1;
    }

    /* Print BLAST Header */

    if(!theInfo->xml_output && 
       theInfo->align_view != HitTableWithHeader) {        
        WWWBlastPrintHeader(theInfo);
    }

    /* Do the search and Format output */

#ifdef NCBI_CLIENT_SERVER
    WWWBlastDoClientSearch(theInfo);
#else
    WWWBlastDoSearch(theInfo);
#endif

    WWWBlastInfoFree(theInfo);

    return 0;
}
