/* $Id: rpsblast.c,v 6.38 2001/08/28 17:45:01 madden Exp $
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
* File Name:  $RCSfile: rpsblast.c,v $
*
* Author:  Sergei Shavirin
*
* Initial Version Creation Date: 12/14/1999
*
* $Revision: 6.38 $
*
* File Description:
*         Main file for RPS BLAST program
*
* $Log: rpsblast.c,v $
* Revision 6.38  2001/08/28 17:45:01  madden
* Add -m 9 as tabular output with comments
*
* Revision 6.36  2001/06/27 16:20:00  dondosha
* Enabled tabular output for RPS Blast
*
* Revision 6.35  2001/04/13 14:19:08  madden
* Do not print verison banner for XML
*
* Revision 6.34  2001/03/26 14:28:53  madden
* Fix XML problems
*
* Revision 6.33  2000/11/07 18:32:38  shavirin
* Added program version to the output.
*
* Revision 6.32  2000/11/01 20:03:15  shavirin
* Removed not-used option -f for threshold.
*
* Revision 6.31  2000/10/27 19:14:41  madden
* Change description of -b option
*
* Revision 6.30  2000/10/23 19:58:21  dondosha
* Open and close AsnIo outside of call(s) to BXMLPrintOutput
*
* Revision 6.29  2000/10/18 20:55:16  shavirin
* Removed unused command-line parameters.
*
* Revision 6.28  2000/10/02 16:40:53  shavirin
* Fixed setting of TXALIGN_BLASTX_SPECIAL; option.
*
* Revision 6.27  2000/09/29 19:04:57  shavirin
* Fixed warnings and minor errors detected on Windows NT.
*
* Revision 6.26  2000/09/28 18:51:16  shavirin
* Adopted to new parameter BioseqPtr in print results callback.
*
* Revision 6.25  2000/09/27 19:09:04  shavirin
* Significantly redesigned external interface to RPS Blast.
*
* Revision 6.24  2000/08/29 16:54:55  shavirin
* Added option (m = 7) to print XML output.
*
* Revision 6.23  2000/08/21 21:19:16  shavirin
* Removed absolete variable MaxThreadsSem and related code.
*
* Revision 6.22  2000/08/21 19:24:28  madden
* Fix problem writing ASN.1 when multi-threading
*
* Revision 6.21  2000/08/18 19:38:31  madden
* Set believe_query and html flags in AcknowledgeBlastQuery
*
* Revision 6.20  2000/08/04 16:36:05  madden
* Concatenate rather than overwrite SeqAnnot
*
* Revision 6.19  2000/06/28 17:12:29  shavirin
* Fixed problem with -U T option: NULL-ed slp between different sequences.
*
* Revision 6.18  2000/06/27 15:25:19  madden
* Changed master-slave to query-anchored
*
* Revision 6.17  2000/06/20 15:49:35  shavirin
* Added BLAST database title to SeqAnnot output.
*
* Revision 6.16  2000/05/02 18:01:32  shavirin
* Adjusted to changes in RPSInti() function.
*
* Revision 6.15  2000/04/13 18:50:55  shavirin
* Fixed serious memory leaks.
*
* Revision 6.14  2000/04/12 14:15:41  shavirin
* Added back ObjMgrFreeCache together wirg seqmgr changes, those removed
* deadlock.
*
* Revision 6.13  2000/03/28 20:33:44  shavirin
* Changed logic of processing MT - multiple FASTA files.
*
* Revision 6.12  2000/03/10 20:00:15  shavirin
* Added multi-thread support for multi-FASTA files.X
*
* Revision 6.11  2000/03/02 21:06:09  shavirin
* Added -U option, that allows to consider low characters in FASTA files
* as filtered regions (for blastn, blastp and tblastn).
*
* Revision 6.10  2000/02/23 21:03:36  shavirin
* Fixed -z and -Y options in rpsblast.
*
* Revision 6.9  2000/02/17 21:28:55  shavirin
* Added option is_rps_blast = TRUE.
*
* Revision 6.8  2000/02/15 16:14:04  shavirin
* Minor changes.
*
* Revision 6.7  2000/02/11 22:05:01  shavirin
* Oprion do_sum_stats set to FALSE.
*
* Revision 6.6  2000/02/11 20:51:01  shavirin
* Added possibility to search PSSM database against DNA sequences.
*
* Revision 6.5  2000/02/08 17:39:08  shavirin
* Empty log message.
*
* Revision 6.4  2000/02/01 17:22:48  shavirin
* Updated function RPSViewSeqAlign().
*
* Revision 6.3  2000/01/07 22:34:05  shavirin
* Added printing of SeqAlignment if necessary.
*
* Revision 6.2  1999/12/30 18:37:53  shavirin
* Added NCBI header and Log information.
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <lookup.h>
#include <tofasta.h>
#include <txalign.h>
#include <ncbithr.h>
#include <rpsutil.h>
#include <xmlblast.h>
#include <sqnutils.h>

#if PURIFY
#include "/am/purew/solaris2/new/../purify/purify-4.5-solaris2/purify.h"
#endif

AsnIoPtr aip_glb=NULL;

#define NUMARG (sizeof(myargs)/sizeof(myargs[0]))

static Args myargs [] = {
    {"Input query sequence (this parameter must be set)",  /* 0 */
     "stdin", NULL,NULL,FALSE,'i',ARG_FILE_IN, 0.0,0,NULL},
    {"RPS BLAST Database",            /* 1 */
     NULL, NULL,NULL,FALSE,'d',ARG_FILE_IN, 0.0,0,NULL},
    {"Query sequence is protein ",     /* 2 */
     "T", NULL,NULL,TRUE, 'p', ARG_BOOLEAN, 0.0,0,NULL},
    { "Expectation value (E)",        /* 3 */
      "10.0", NULL, NULL, FALSE, 'e', ARG_FLOAT, 0.0, 0, NULL},
    { "alignment view options:\n0 = pairwise,\n1 = query-anchored showing identities,\n2 = query-anchored no identities,\n3 = flat query-anchored, show identities,\n4 = flat query-anchored, no identities,\n5 = query-anchored no identities and blunt ends,\n6 = flat query-anchored, no identities and blunt ends,\n7 = XML Blast output,\n8 = tabular output, \n9 = tabular output with comments", /* 4 */
      "0", NULL, NULL, FALSE, 'm', ARG_INT, 0.0, 0, NULL},
    { "Output File for Alignment", /* 5 */
      "stdout", NULL, NULL, TRUE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
    { "Dropoff (X) for blast extensions in bits (default if zero)", /* 6 */
      "7.0", NULL, NULL, FALSE, 'y', ARG_FLOAT, 0.0, 0, NULL},
    { "0 for multiple hits 1-pass, 1 for single hit 1-pass, 2 for 2-pass", /* 7 */
      "0", NULL, NULL, FALSE, 'P', ARG_INT, 0.0, 0, NULL},
    { "Filter query sequence with SEG", /* 8 */
      "F", NULL, NULL, FALSE, 'F', ARG_STRING, 0.0, 0, NULL},
    { "Cost to open a gap",     /* 9 */
      "11", NULL, NULL, FALSE, 'G', ARG_INT, 0.0, 0, NULL},
    { "Cost to extend a gap",   /* 10 */
      "1", NULL, NULL, FALSE, 'E', ARG_INT, 0.0, 0, NULL},
    { "X dropoff value for gapped alignment (in bits)", /* 11 */
      "15", NULL, NULL, FALSE, 'X', ARG_INT, 0.0, 0, NULL},
    { "Number of bits to trigger gapping", /* 12 */
      "22.0", NULL, NULL, FALSE, 'N', ARG_FLOAT, 0.0, 0, NULL},
    { "Gapped",                 /* 13 */
      "T", NULL, NULL, FALSE, 'g', ARG_BOOLEAN, 0.0, 0, NULL},
    { "Start of required region in query", /* 14 */
      "1", NULL, NULL, FALSE, 'S', ARG_INT, 0.0, 0, NULL},
    { "End of required region in query (-1 indicates end of query)", /* 15 */
      "-1", NULL, NULL, FALSE, 'H', ARG_INT, 0.0, 0, NULL},
    { "Number of processors to use", /* 16 */
      "1", NULL, NULL, FALSE, 'a', ARG_INT, 0.0, 0, NULL},
    { "Show GI's in deflines",  /* 17 */
      "F", NULL, NULL, FALSE, 'I', ARG_BOOLEAN, 0.0, 0, NULL},
    { "Believe the query defline", /* 18 */
      "F", NULL, NULL, FALSE, 'J', ARG_BOOLEAN, 0.0, 0, NULL},
    { "X dropoff value for final gapped alignment (in bits)", /* 19 */
      "25", NULL, NULL, FALSE, 'Z', ARG_INT, 0.0, 0, NULL},
    { "SeqAlign file ('Believe the query defline' must be TRUE)", /* 20 */
      NULL, NULL, NULL, TRUE, 'O', ARG_FILE_OUT, 0.0, 0, NULL},
    { "Number of database sequences to show one-line descriptions for (V)", /* 21 */
      "500", NULL, NULL, FALSE, 'v', ARG_INT, 0.0, 0, NULL},
    { "Number of database sequence to show alignments for (B)", /* 22 */
      "250", NULL, NULL, FALSE, 'b', ARG_INT, 0.0, 0, NULL},
    { "Effective length of the database (use zero for the real size)", /* 23 */
      "0", NULL, NULL, FALSE, 'z', ARG_INT, 0.0, 0, NULL},
    { "Effective length of the search space (use zero for the real size)", /* 24 */
      "0", NULL, NULL, FALSE, 'Y', ARG_FLOAT, 0.0, 0, NULL},
    { "Produce HTML output",  /* 25 */
      "F", NULL, NULL, FALSE, 'T', ARG_BOOLEAN, 0.0, 0, NULL},
    {"Logfile name ",  /* 26 */
     "rpsblast.log", NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL},
    {"Use lower case filtering of FASTA sequence",    /* 27 */
     "F", NULL,NULL,TRUE,'U',ARG_BOOLEAN, 0.0,0,NULL},
};

void PGPGetPrintOptions(Boolean gapped, Uint4Ptr align_options_out, 
                        Uint4Ptr print_options_out)
{
    Uint4 print_options, align_options;

    print_options = 0;
    if (gapped == FALSE)
        print_options += TXALIGN_SHOW_NO_OF_SEGS;
    
    align_options = 0;
    align_options += TXALIGN_COMPRESS;
    align_options += TXALIGN_END_NUM;

    if (myargs[17].intvalue) {
        align_options += TXALIGN_SHOW_GI;
        print_options += TXALIGN_SHOW_GI;
    } 
    
    if (myargs[25].intvalue) {
        align_options += TXALIGN_HTML;
        print_options += TXALIGN_HTML;
    }

    if(!myargs[2].intvalue)
        align_options += TXALIGN_BLASTX_SPECIAL;
    
    if (myargs[4].intvalue != 0) {
        align_options += TXALIGN_MASTER;
        if (myargs[4].intvalue == 1 || myargs[4].intvalue == 3)
            align_options += TXALIGN_MISMATCH;
        if (myargs[4].intvalue == 3 || myargs[4].intvalue == 4 || myargs[4].intvalue == 6)
            align_options += TXALIGN_FLAT_INS;
        if (myargs[4].intvalue == 5 || myargs[4].intvalue == 6)
            align_options += TXALIGN_BLUNT_END;
    } else {
        align_options += TXALIGN_MATRIX_VAL;
        align_options += TXALIGN_SHOW_QS;
    }

    *align_options_out = align_options;
    *print_options_out = print_options;

    return;
}

void RPSBlastOptionsFree(RPSBlastOptionsPtr rpsbop)
{

    FileClose(rpsbop->outfp);
    BLASTOptionDelete(rpsbop->options);
    readdb_destruct(rpsbop->rdfp);

    MemFree(rpsbop->rps_database);    
    MemFree(rpsbop);
    
    return;
}

static RPSBlastOptionsPtr RPSReadBlastOptions(void)
{
    RPSBlastOptionsPtr rpsbop;
    BLAST_OptionsBlkPtr options;
    static Int4 count=0;
    
    rpsbop = MemNew(sizeof(RPSBlastOptions));

    if (myargs [5].strvalue != NULL) {
	rpsbop->out_filename = StringSave(myargs [5].strvalue);
        if ((rpsbop->outfp = FileOpen(myargs [5].strvalue, "a")) == NULL) {
            ErrPostEx(SEV_FATAL, 0, 0, "rpsblast: Unable to open output "
                      "file %s\n", myargs [5].strvalue);
            return NULL;
        }
    }

    /* Note: these 2 parameters are necessary to intialize RPS Blast */
    rpsbop->rps_database = StringSave(myargs [1].strvalue);    
    rpsbop->query_is_protein = myargs[2].intvalue;
    rpsbop->num_threads = myargs[16].intvalue;

    if((rpsbop->rdfp = readdb_new(myargs [1].strvalue, TRUE)) == NULL)
        return NULL;
    
    /* rpsbop->rpsinfo = RPSInfoAttach(rpsinfo_main); */

    if (myargs[18].intvalue != 0)
        rpsbop->believe_query = TRUE;
    
    options = BLASTOptionNew(rpsbop->query_is_protein ? "blastp" : "tblastn", 
                             (Boolean)myargs[13].intvalue);
    rpsbop->options = options;
    
    /* rpsbop->options->query_lcase_mask = slp; External filtering */
    
    if (myargs[23].intvalue)
        options->db_length = (Int8) myargs[23].intvalue;
    
    if (myargs[24].floatvalue)
        options->searchsp_eff = (Nlm_FloatHi) myargs[24].floatvalue;
    
    /* Necessary options for RPS Blast */
    options->do_sum_stats = FALSE;
    options->is_rps_blast = TRUE; 
    
    rpsbop->number_of_descriptions = myargs[21].intvalue;
    rpsbop->number_of_alignments = myargs[22].intvalue;
    
    /* Set default gap params for matrix. */
    BLASTOptionSetGapParams(options, "BLOSUM62", 0, 0);
    
    if(myargs[4].intvalue == 7)
       rpsbop->is_xml_output = TRUE;
    else if (myargs[4].intvalue == 8)
       rpsbop->is_tabular = TRUE;
    else if (myargs[4].intvalue == 8 || myargs[4].intvalue == 9)
    {
       rpsbop->is_tabular = TRUE;
       rpsbop->is_tabular_comments = TRUE;
    }
    else
       PGPGetPrintOptions(options->gapped_calculation, 
                          &rpsbop->align_options, &rpsbop->print_options);
    
    /* decrement by one to agree with program values. */
    options->required_start = myargs[14].intvalue - 1;
    options->required_end = myargs[15].intvalue;
    
    if (options->required_end != -1) {
        options->required_end--;
    }

    options->dropoff_2nd_pass  = (Int4) myargs [6].floatvalue;
    options->expect_value  = (Nlm_FloatHi) myargs [3].floatvalue;
    options->hitlist_size = MAX(rpsbop->number_of_descriptions, 
                                rpsbop->number_of_alignments);
    
    if (myargs[13].intvalue != 0) {
        if (myargs[7].intvalue == 0) {
            options->two_pass_method  = FALSE;
            options->multiple_hits_only  = TRUE;
        } else if (myargs[7].intvalue == 1) {
            options->two_pass_method  = FALSE;
            options->multiple_hits_only  = FALSE;
        } else {
            options->two_pass_method  = TRUE;
            options->multiple_hits_only  = FALSE;
        }
        options->gap_open = myargs[9].intvalue;
        options->gap_extend = myargs[10].intvalue;

        /*  options->decline_align = myargs[??].intvalue; */

        options->gap_x_dropoff = myargs[11].intvalue;
        options->gap_x_dropoff_final = myargs[19].intvalue;
        options->gap_trigger = myargs[12].floatvalue;
    }
    
    if (StringICmp(myargs[8].strvalue, "T") == 0) {
        options->filter_string = StringSave("S");
    } else {
        options->filter_string = StringSave(myargs[8].strvalue);
    }

    /* Only one CPU may be used at this time inside the core engine*/    
    options->number_of_cpus = (Int2) myargs[16].intvalue;
    
    options->isPatternSearch = FALSE;

    options = BLASTOptionValidate(options, rpsbop->query_is_protein ? 
                                  "blastp" : "tblastn");
    
    if (options == NULL)
        return NULL;
    
    if (options == NULL)
        return NULL;
    
    return rpsbop;
}

static void RPSViewSeqAlign(BioseqPtr query_bsp, 
                            SeqAlignPtr seqalign, RPSBlastOptionsPtr rpsbop, 
                            ValNodePtr other_returns)
{
    SeqAnnotPtr seqannot;
    BlastPruneSapStructPtr prune;
    Uint1 align_type;
    CharPtr title;
    ValNodePtr vnp, mask;

    free_buff();    
    init_buff_ex(128);
    
    /* BlastPrintReference(FALSE, 90, rpsbop->outfp); */
    fprintf(rpsbop->outfp, "\n");

    AcknowledgeBlastQuery(query_bsp, 70, rpsbop->outfp, 
                          rpsbop->believe_query, rpsbop->html);
    if(seqalign == NULL) {
        fprintf(rpsbop->outfp, "\nNo hits found for the sequence...\n\n");
        return;
    }
    
    prune = BlastPruneHitsFromSeqAlign(seqalign, 
                                       rpsbop->number_of_descriptions, NULL);
    PrintDefLinesFromSeqAlign(prune->sap, 80, rpsbop->outfp, 
                              rpsbop->print_options, FIRST_PASS, NULL);

    seqannot = SeqAnnotNew();
    seqannot->type = 2;
    align_type = BlastGetProgramNumber(rpsbop->query_is_protein ?
                                       "blastp" : "blastx");
    
    AddAlignInfoToSeqAnnot(seqannot, align_type); /* blastp or tblastn */
    
    /* Now adding database title from SeqAnnot */

    title = readdb_get_title(rpsbop->rdfp);
    
    BLASTAddBlastDBTitleToSeqAnnot(seqannot, title);
    
    /* --------------------------------------- */
    
    prune = BlastPruneHitsFromSeqAlign(seqalign, rpsbop->number_of_alignments, 
                                       prune);
    seqannot->data = prune->sap;

    if(aip_glb != NULL) {     
        SeqAnnotAsnWrite(seqannot, aip_glb, NULL);
	AsnIoReset(aip_glb);
    }

    /* ------ Mask needed ----- */

    mask = NULL;
    for (vnp=other_returns; vnp; vnp = vnp->next) {
        switch (vnp->choice) {
        case SEQLOC_MASKING_NOTSET:
        case SEQLOC_MASKING_PLUS1:
        case SEQLOC_MASKING_PLUS2:
        case SEQLOC_MASKING_PLUS3:
        case SEQLOC_MASKING_MINUS1:
        case SEQLOC_MASKING_MINUS2:
        case SEQLOC_MASKING_MINUS3:
            ValNodeAddPointer(&mask, vnp->choice, vnp->data.ptrvalue);
            break;
        default:
            break;
        }
    }

    if (myargs[4].intvalue != 0) {
        ShowTextAlignFromAnnot(seqannot, 60, rpsbop->outfp, 
                               NULL, NULL, rpsbop->align_options, NULL, 
                               mask, NULL);
    } else {
        ShowTextAlignFromAnnot(seqannot, 60, rpsbop->outfp, 
                               NULL, NULL, rpsbop->align_options, NULL, 
                               mask, FormatScoreFunc);
    }

    prune = BlastPruneSapStructDestruct(prune);

    seqannot->data = NULL;
    seqannot = SeqAnnotFree(seqannot);

    free_buff();
    return;
}

Boolean RPSFormatFooter(RPSBlastOptionsPtr rpsbop,  ValNodePtr other_returns)
{
    ValNodePtr  mask_loc, vnp;
    BLAST_KarlinBlkPtr ka_params=NULL, ka_params_gap=NULL;
    TxDfDbInfoPtr dbinfo=NULL, dbinfo_head;
    CharPtr params_buffer=NULL;
    BLAST_MatrixPtr blast_matrix;
    
    mask_loc = NULL;
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
            blast_matrix = vnp->data.ptrvalue;
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
    
    free_buff();    
    init_buff_ex(85);
    dbinfo_head = dbinfo;
    while (dbinfo) {
        PrintDbReport(dbinfo, 70, rpsbop->outfp);
        dbinfo = dbinfo->next;
    }
    
    if (ka_params) {
        PrintKAParameters(ka_params->Lambda, ka_params->K, ka_params->H, 
                          70, rpsbop->outfp, FALSE);
    }
    
    if (ka_params_gap) {
        PrintKAParameters(ka_params_gap->Lambda, ka_params_gap->K,
                          ka_params_gap->H, 70, rpsbop->outfp, TRUE);
    }
    
    PrintTildeSepLines(params_buffer, 70, rpsbop->outfp);
    free_buff();

    fflush(rpsbop->outfp);

    return TRUE;
}

static SeqEntryPtr LIBCALLBACK RPSGetNextSeqEntry(SeqLocPtr PNTR slp, VoidPtr data)
{
    SeqEntryPtr sep;
    static TNlmMutex read_mutex;
    static FILE *infp;
    static Boolean end_of_data = FALSE;

    NlmMutexInit(&read_mutex);
    NlmMutexLock(read_mutex);
    
    if(end_of_data) {
        NlmMutexUnlock(read_mutex);
        return NULL;
    }

    /* Opening file with input sequences */
    if(infp == NULL) {
        if ((infp = FileOpen(myargs [0].strvalue, "r")) == NULL) {
            ErrPostEx(SEV_FATAL, 0, 0, 
                      "rpsblast: Unable to open input file %s\n", 
                      myargs [0].strvalue);
            end_of_data = TRUE;
            NlmMutexUnlock(read_mutex);
        }
    }
    
    if(myargs[27].intvalue) {
        sep = FastaToSeqEntryForDb (infp, !myargs[2].intvalue, NULL, myargs[18].intvalue, NULL, NULL, slp);
    } else {
        sep = FastaToSeqEntryEx (infp, !myargs[2].intvalue, NULL, myargs[18].intvalue);
    }
    
    if(sep == NULL) {            /* Probably last FASTA entry */
        end_of_data = TRUE;
        FileClose(infp);
        NlmMutexUnlock(read_mutex);
        return NULL;
    }
    
    NlmMutexUnlock(read_mutex);
    
    return sep;
}

static Boolean LIBCALLBACK RPSResultsCallback(BioseqPtr query_bsp,
                                  RPSBlastOptionsPtr rpsbop, 
                                  SeqAlignPtr seqalign, 
                                  ValNodePtr other_returns, 
                                  ValNodePtr error_returns, VoidPtr data)
{
    if(rpsbop->is_xml_output == TRUE) {
       AsnIoPtr aip = AsnIoOpen(rpsbop->out_filename, "wx");
       BXMLPrintOutput(aip, seqalign, rpsbop->options, 
                       rpsbop->query_is_protein ? 
                       "blastp" : "tblastn", rpsbop->rps_database, 
                       query_bsp, other_returns, 0, NULL);
       AsnIoClose(aip);
    } else if (rpsbop->is_tabular) {
	if (rpsbop->is_tabular_comments)
       		PrintTabularOutputHeader(rpsbop->rps_database, query_bsp, NULL, 
                                "rps-blast", 0, rpsbop->believe_query,
                                rpsbop->outfp);
       BlastPrintTabulatedResults(seqalign, query_bsp, NULL, 
                                  rpsbop->number_of_alignments,
                                  rpsbop->query_is_protein ? 
                                  "blastp" : "tblastn", FALSE,
                                  rpsbop->believe_query, 0, 0, rpsbop->outfp);
    } else {
        RPSViewSeqAlign(query_bsp, seqalign, rpsbop, other_returns);
        RPSFormatFooter(rpsbop, other_returns);
    }
    
    return TRUE;
}

Int2 Main(void)
{
    FILE *fd;
    Int4 i;
    RPSBlastOptionsPtr rpsbop;

    if (!GetArgs("rpsblast", NUMARG, myargs))
	return 1;
    
    if ( !ErrSetLog (myargs[26].strvalue) ) { /* Logfile */
        ErrShow();
        return 1;
    } else {
        ErrSetOpts (ERR_CONTINUE, ERR_LOG_ON);
    }
    
    UseLocalAsnloadDataAndErrMsg ();
    
    if (!SeqEntryLoad())
        return 1;
        
    /* Truncate output file */
    if((fd = FileOpen(myargs [5].strvalue, "w")) != NULL)
        FileClose(fd);
    
    if (myargs[20].strvalue != NULL) {
        if (myargs[18].intvalue == 0) {
            ErrPostEx(SEV_FATAL, 0, 0, 
                      "-J option must be TRUE to use this option");
            return 1;
        } else  {
            if ((aip_glb = AsnIoOpen (myargs[20].strvalue,"w")) == NULL) {
                ErrPostEx(SEV_FATAL, 0, 0, "blast: Unable to open output "
                          "file %s\n", myargs[20].strvalue);
                return 1;
            }
    	}
    }
        
    if((rpsbop = RPSReadBlastOptions()) == NULL) {
        ErrPostEx(SEV_FATAL, 0, 0, "Unable to create RPS Blast options");
        return 1;
    }

    if (!rpsbop->is_xml_output && !rpsbop->is_tabular)
	    BlastPrintVersionInfo("RPS-BLAST", rpsbop->html, rpsbop->outfp);
    
    /* VoidPtr bsp_data = NULL, print_data = NULL; */
    RPSBlastSearchMT(rpsbop, RPSGetNextSeqEntry, NULL, 
                     RPSResultsCallback, NULL);
    RPSBlastOptionsFree(rpsbop);
    
    return 0;
}
