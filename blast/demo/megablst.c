/* $Id: megablst.c,v 6.6 1999/11/26 19:42:49 shavirin Exp $
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
* File Name:  $RCSfile: megablst.c,v $
*
* Author:  Webb Miller and Co.
* Adopted for NCBI standard libraries by Sergey Shavirin
*
* Initial Creation Date: 10/27/1999
*
* $Revision: 6.6 $
*
* File Description:
*         Main file for Mega Blast program
*
* $Log: megablst.c,v $
* Revision 6.6  1999/11/26 19:42:49  shavirin
* Fixed warnings of windows compiler.
*
* Revision 6.5  1999/11/24 20:39:30  shavirin
* Added possibility to produce Traditional Blast Output.
*
* Revision 6.4  1999/11/03 21:28:23  shavirin
* Added option to print output in real time.
*
* Revision 6.3  1999/11/03 21:06:16  shavirin
* Fixed assignment of extention type parameter.
*
* Revision 6.2  1999/11/03 19:56:26  shavirin
* Distinguished search functions and printing functions.
*
* Revision 6.1  1999/10/29 15:40:49  shavirin
* Initial revision.
*
*
* Initial revision.
*
* ==========================================================================
*/

#include <ncbi.h>
#include <readdb.h>
#include <txalign.h>
#include <mbutils.h>
#include <blastpri.h>
#include <sqnutils.h>

#define NUMARG (sizeof(myargs)/sizeof(myargs[0]))
static Args myargs[] =
{
    {"Database",                                            /*0*/
     NULL, NULL, NULL, FALSE, 'd', ARG_STRING, 0.0, 0, NULL},
    {"Query File",                                          /*1*/
     "stdin", NULL, NULL, FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
    {"Dropoff (X) for extensions",                          /*2*/
     "180", NULL, NULL, FALSE, 'X', ARG_INT, 0.0, 0, NULL},
    {"Length of perfect match (in bytes) before extension", /*3*/
     "7", NULL, NULL, FALSE, 'w', ARG_INT, 0.0, 0, NULL},
    {"Maximal number of entries allowed in a 12mer",        /*4*/
     "100", NULL, NULL, FALSE, 'p', ARG_INT, 0.0, 0, NULL},
    {"Number of charaters output for sequence id, maximum 50", /*5*/
     "40", NULL, NULL, FALSE, 'n', ARG_INT, 0.0, 0, NULL},
    {"Perform est search instead of blast search (T/F)",    /*6*/
     "F", NULL, NULL, FALSE, 'e', ARG_BOOLEAN, 0.0, 0, NULL},
    {"Mismatch penalty (greedy extension) (match is +10)",  /*7*/
     "-20", NULL, NULL, FALSE, 'r', ARG_INT, 0.0, 0, NULL},
    {"Extension option 0:Greedy 1:Greedy affine",           /*8*/
     "0", NULL, NULL, FALSE, 'P', ARG_INT, 0.0, 0, NULL},
    {"Match score (0 for defaults)",                        /*9*/
     "10", NULL, NULL, FALSE, 'M', ARG_INT, 0.0, 0, NULL},
    {"GapOpen Penalty (0 for defaults)",                    /*10*/
     "0", NULL, NULL, FALSE, 'O', ARG_INT, 0.0, 0, NULL},
    {"Gap Extension Penalty (0 for defaults)",              /*11*/
     "0", NULL, NULL, FALSE, 'E', ARG_INT, 0.0, 0, NULL},
    {"Cutoff for gap-alignment",                            /*12*/
     "200", NULL, NULL, FALSE, 't', ARG_INT, 0.0, 0, NULL},
    {"X dropoff for gapfree extension",                     /*13*/
     "50", NULL, NULL, FALSE, 'x', ARG_INT, 0.0, 0, NULL},
    {"Number of words output for sequence id",              /*14*/
     "0", NULL, NULL, FALSE, 'N', ARG_INT, 0.0, 0, NULL},
    {"Normalize the coordinates of reverse compliment output", /*15*/
     "F", NULL, NULL, FALSE, 'Z', ARG_BOOLEAN, 0.0, 0, NULL},
    {"Output alignment mode: (0) no alignment (1) summary (2) alignment",/*16*/
     "0", NULL, NULL, FALSE, 'A', ARG_INT, 0.0, 0, NULL},
    {"Query reverse compliment or not",                     /*17*/
     "T", NULL, NULL, FALSE, 'c', ARG_BOOLEAN, 0.0, 0, NULL},
    {"Log file name",                                       /*18*/
     "mblast.log", NULL, NULL, FALSE, 'l', ARG_FILE_OUT, 0.0, 0, NULL},
    {"Print output in real time",                              /*19*/
     "T", NULL, NULL, FALSE, 'R', ARG_BOOLEAN, 0.0, 0, NULL},
    {"Output Filename",                                          /*20*/
     "stdout", NULL, NULL, FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
};

void MBFreeAlignParameters(align_params_t *mbap)
{
    MemFree(mbap);
    return;
}
align_params_t *MBCreateAlignParameters(void)
{
    align_params_t *mbap;
        
    mbap = MemNew(sizeof(align_params_t));

    mbap->xdrop_ext = myargs[2].intvalue;
    mbap->len_perf_match = myargs[3].intvalue-1;
    mbap->max_12mer_ents = myargs[4].intvalue;
    
    mbap->idlen = myargs[5].intvalue;

    if (myargs[14].intvalue > 0)
	mbap->idlen = -myargs[14].intvalue;

    mbap->mismatch_penalty_mb = - myargs[7].intvalue;

    if (mbap->mismatch_penalty_mb <= 0) {
        ErrPostEx(SEV_FATAL, 0, 0, "mismatch must be negative");
        MBFreeAlignParameters(mbap);
        return NULL;
    }
    
    mbap->match_score = myargs[9].intvalue;
    mbap->gap_open_penalty = myargs[10].intvalue;
    mbap->gap_extension_penalty = myargs[11].intvalue;
    mbap->gap_align_cutoff = myargs[12].intvalue;
    mbap->xdrop_gapfree = myargs[13].intvalue;
    mbap->normalize_revcomp_flag = myargs[15].intvalue;
    
    /* prog_flag = 0: est_search 1: mb with greedy */

    switch(myargs[6].intvalue) {
    case 1:
        mbap->prog_flag = MB_DO_EST;
        break;
    case 0:
    default:
        mbap->prog_flag = MB_DO_GREEDY;
        break;
    }
    
    mbap->output_al =  myargs[16].intvalue;
    mbap->revcomp_flag = myargs[17].intvalue;
    mbap->real_time_print = myargs[19].intvalue;
    
    if(!mbap->real_time_print) {
        mbap->output_al = 2; /* produce full alignment in non-realtime mode */
    }
    
    if (myargs[8].intvalue == 0) { /* greedy case */
        mbap->match_score = MATCH_SCORE_SCALE;
        mbap->gap_open_penalty = 0;
        mbap->gap_extension_penalty = mbap->mismatch_penalty_mb + MATCH_SCORE_SCALE/2;
    } else if (myargs[8].intvalue == 1 || myargs[8].intvalue == 2) { 
        /* affine case  2 is for compatibility */
        if (mbap->gap_extension_penalty <= 0) {
            ErrPostEx(SEV_ERROR, 0, 0, "In the affine case, the gap "
                      "extension penalty must be positive.");
            return NULL;
        }
    } else {
        ErrPostEx(SEV_ERROR, 0, 0, "This extention option number is invalid");
        return NULL;
    }

    if (mbap->idlen > HDRSAVE) mbap->idlen = HDRSAVE;

    if (myargs[20].strvalue != NULL) {
        if ((mbap->outfp = FileOpen(myargs [20].strvalue, "w")) == NULL) {
            ErrPostEx(SEV_FATAL, 0, 0, "megablast: Unable to open output "
                      "file %s\n", myargs [20].strvalue);
            return NULL;
        }
    }

    return mbap;
}
Int2 Main(void)
{
    FILE *infp;
    mbt_t *mbt;
    qdb_t *qdb;
    align_params_t *mbap;
    gal_t *gp;
    MBHitlistPtr mbhlp;
    Uint4 align_options, print_options;
    SeqAlignPtr sap;
    ValNodePtr vnp, vnp_sap, sep_vnp = NULL;
    SeqAnnotPtr seqannot;
    Int4 num_seqs;

    if (!GetArgs("mb", NUMARG, myargs))
	return (1);
    
    if ( !ErrSetLog (myargs[18].strvalue) ) { /* Logfile */
        ErrShow();
        return 1;
    } else {
        ErrSetOpts (ERR_CONTINUE, ERR_LOG_ON);
    }

    UseLocalAsnloadDataAndErrMsg ();

    if (! SeqEntryLoad())
        return 1;

    if((mbap = MBCreateAlignParameters()) == NULL)
        return 1;
    
    if ((infp = FileOpen(myargs[1].strvalue, "r")) == NULL) {
	ErrPostEx(SEV_FATAL, 0, 0,
                  "blast: Unable to open input file %s\n", myargs[1].strvalue);
	return (1);
    }
    
    mbt = mb_init(myargs[0].strvalue); /* Blast database */
    qdb = (qdb_t*) MemNew(sizeof(*qdb)); /* Allocating 15 MBytes */
    
    do {
        if(qdb_fill_block(infp, qdb, mbap->revcomp_flag) <=0) {
            ErrPostEx(SEV_ERROR, 0,0,"Failure to read input sequence");
            return 1;
        }
        
        /* XXX */        
        mb_start(mbt, qdb->sequence, qdb_total_length(qdb), 
                 mbap->len_perf_match, mbap->max_12mer_ents); 
        
        if (mbap->prog_flag == MB_DO_EST) {
            est_search(mbt);
            break;  /* Only one pass ?? */
	} else { /* GREEDY */

            if((mbhlp = MBEngineCore(mbt, qdb, mbap)) == NULL)
                continue;
            
            if(mbhlp != NULL) {
#ifdef ORIGINAL_OUTPUT
                MBPrintHitList(mbt, mbhlp, qdb, mbap);
#else
                vnp_sap = MBSeqAlignFromHitList(mbt, qdb, mbhlp, &sep_vnp, 
                                                &num_seqs, mbap);
                
                ReadDBBioseqFetchEnable("megablast", myargs[0].strvalue,
                                        TRUE, TRUE);
                print_options = 0;
                print_options += TXALIGN_SHOW_GI;

                align_options = 0;
                align_options += TXALIGN_COMPRESS;
                align_options += TXALIGN_END_NUM;
                align_options += TXALIGN_MATRIX_VAL;
                align_options += TXALIGN_SHOW_QS;

                seqannot = SeqAnnotNew();
                seqannot->type = 2;
                AddAlignInfoToSeqAnnot(seqannot, 2);
                
                for(vnp = vnp_sap; vnp != NULL; vnp=vnp->next) {
                    SeqIdPtr sip;
                    BioseqPtr query_bsp;
                    sap = vnp->data.ptrvalue;
                    sip = TxGetQueryIdFromSeqAlign(sap);
                    query_bsp = BioseqLockById(sip);
                    fprintf(mbap->outfp, 
"--------------------------------------------------------------\n");
                    AcknowledgeBlastQuery(query_bsp, 70, mbap->outfp, 
                                          TRUE, FALSE);
                    free_buff();
                    fprintf(mbap->outfp, 
"--------------------------------------------------------------\n");
                    PrintDefLinesFromSeqAlign(sap, 80, mbap->outfp, 
                                              print_options, FIRST_PASS, NULL);
                    seqannot->data = sap;
                    ShowTextAlignFromAnnot(seqannot, 60, mbap->outfp, 
                                           NULL, NULL, align_options, NULL, 
                                           NULL, FormatScoreFunc);
                    seqannot->data = NULL;
                }
                
#endif
                MBHitlistFree(mbhlp);
            }            
        }
    } while (!feof(infp));
    

    for(vnp = sep_vnp; vnp != NULL; vnp = vnp->next) {
        SeqEntryFree(vnp->data.ptrvalue);
        vnp->data.ptrvalue = NULL;
    }
    ValNodeFree(sep_vnp);

    for(vnp = vnp_sap; vnp != NULL; vnp = vnp->next) {
        SeqAlignSetFree(vnp->data.ptrvalue);
        vnp->data.ptrvalue = NULL;
    }
    ValNodeFree(vnp_sap);

    mb_end(mbt);
    FileClose(infp);
    FileClose(mbap->outfp);
    MemFree(qdb);
    MBFreeAlignParameters(mbap);
    free_buff();

    return 0;
}
