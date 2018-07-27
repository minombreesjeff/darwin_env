/*
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
* File Name: blastcl3.c
*
* Author:  Tom Madden
*
* Version Creation Date:   05/16/95
*
* $Revision: 1.25 $
*
* File Description: 
*       Simulates "traditional" BLAST output
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*
*
* RCS Modification History:
* $Log: blastcl3.c,v $
* Revision 1.25  2001/04/13 23:01:22  juran
* Removed unused variable.
*
* Revision 1.24  2001/01/19 21:30:09  dondosha
* Call readdb...._ex functions to possibly save some time when finding db length
*
* Revision 1.23  2000/10/25 16:41:26  madden
* Add BioSource to query_bsp
*
* Revision 1.22  2000/09/28 16:36:59  dondosha
* Set parameters differently for megablast
*
* Revision 1.21  2000/08/28 15:17:45  dondosha
* Added functionality for megablast search
*
* Revision 1.20  2000/05/04 18:57:00  shavirin
* Added option to restrict search to results of Entrez2 lookup.
*
* Revision 1.19  2000/05/04 16:55:05  shavirin
* Removed message of the day for the RPS Blast.
*
* Revision 1.18  2000/05/03 17:33:40  shavirin
* Changed vesion info for RPS-BLAST.
*
* Revision 1.17  2000/05/02 18:07:38  shavirin
* Added option to use RPS Blast search - and corresponding changes to
* do this search over the network.
*
* Revision 1.16  1999/10/07 18:17:18  madden
* Remove FindProt, FindNuc and SeqAlignToFasta
*
* Revision 1.15  1999/08/20 16:37:15  shavirin
* Added protection against invalid program type.
*
* Revision 1.14  1999/04/20 14:51:59  madden
* Error message if TraditionalReport returns FALSE
*
* Revision 1.13  1999/04/13 14:59:31  madden
* Add more options (searchsp, culling, strand)
*
* Revision 1.12  1999/04/02 16:25:36  madden
* FileClose on error, check NULL pointer
*
* Revision 1.11  1999/03/26 15:59:51  madden
* Add option to use filter string, gifile, and get HTML output
*
* Revision 1.10  1999/03/05 15:43:41  madden
* added matrix option
*
* Revision 1.9  1999/01/03 22:33:20  kans
* now calls UseLocalAsnloadDataAndErrMsg
*
* Revision 1.8  1998/12/09 15:27:04  madden
* Add wordsize
*
* Revision 1.7  1998/11/05 17:55:25  madden
* Removed unused global_fp
*
* Revision 1.6  1998/05/02 20:39:38  kans
* global_fp is extern, removed unused callback function, removed unused variables, added newlines in long prompts
*
* Revision 1.5  1998/04/23 14:18:43  egorov
* Add number_of_hits parameter to TraditionalBlastReportLoc
*
* Revision 1.4  1998/04/22 19:58:06  egorov
* Fix minor bug after previous commit
*
* Revision 1.3  1998/04/22 18:10:06  egorov
* Add support for SeqLoc to blastcl3
*
* Revision 1.2  1998/04/16 19:35:30  madden
* Added Int4Ptr arg to TraditionalBlastReport specifying the numbers of hits
*
* Revision 1.1  1997/10/08 19:24:56  madden
* Main (command-line) client file
*
 * 
*/
#define BLASTCLI_BUF_SIZE 255
#include <sequtil.h>
#include <prtutil.h>
#include <tofasta.h>
#include <objblst3.h>
#include <netblap3.h>
#include <blastpri.h>
#include <dust.h>
#include <txalign.h>
#include <accentr.h>
#include <sqnutils.h>


/*
	Montior hook to print to stderr for UNIX clients.
*/

static int LIBCALLBACK UNIXMontiorHook(Nlm_MonitorPtr mon, MonCode code)

{
    switch (code)  {
#ifdef OS_UNIX
    case MonCode_Create :
	fprintf(stderr, "%s\n", (Nlm_CharPtr) mon->strTitle);
        break;
    case MonCode_StrValue :
	fprintf(stderr, "%s\n", (Nlm_CharPtr) mon->strValue);
        break;
#endif
    default :
        break;
    }
    return 0;
    
}


static void PrintMotd(CharPtr string, FILE *fp, Boolean html_format)
     
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
}


#define NUMARGS (sizeof(myargs)/sizeof(myargs[0]))

static Args myargs [] = {
    { "Program Name",           /* 0 */
      NULL, NULL, NULL, FALSE, 'p', ARG_STRING, 0.0, 0, NULL},
    { "Database",               /* 1 */
      "nr", NULL, NULL, FALSE, 'd', ARG_STRING, 0.0, 0, NULL},
    { "Query File",             /* 2 */
      "stdin", NULL, NULL, FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
    { "Expectation value (E)",  /* 3 */
      "10.0", NULL, NULL, FALSE, 'e', ARG_FLOAT, 0.0, 0, NULL},
    { "alignment view options:\n0 = pairwise,\n1 = master-slave showing identities,\n2 = master-slave no identities,\n3 = flat master-slave, show identities,\n4 = flat master-slave, no identities,\n5 = master-slave no identities and blunt ends,\n6 = flat master-slave, no identities and blunt ends", /* 4 */
      "0", NULL, NULL, FALSE, 'm', ARG_INT, 0.0, 0, NULL},
    { "BLAST report Output File", /* 5 */
      "stdout", NULL, NULL, TRUE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
    { "Filter query sequence (DUST with blastn, SEG with others)", /* 6 */
      "T", NULL, NULL, FALSE, 'F', ARG_STRING, 0.0, 0, NULL},
    { "Cost to open a gap (zero invokes default behavior)", /* 7 */
      "0", NULL, NULL, FALSE, 'G', ARG_INT, 0.0, 0, NULL},
    { "Cost to extend a gap (zero invokes default behavior)", /* 8 */
      "0", NULL, NULL, FALSE, 'E', ARG_INT, 0.0, 0, NULL},
    { "X dropoff value for gapped alignment (in bits)\n(zero invokes default behavior)", /* 9 */
      "0", NULL, NULL, FALSE, 'X', ARG_INT, 0.0, 0, NULL},
    { "Show GI's in deflines",  /* 10 */
      "F", NULL, NULL, FALSE, 'I', ARG_BOOLEAN, 0.0, 0, NULL},
    { "Penalty for a nucleotide mismatch (blastn only)", /* 11 */
      "-3", NULL, NULL, FALSE, 'q', ARG_INT, 0.0, 0, NULL},
    { "Reward for a nucleotide match (blastn only)", /* 12 */
	"1", NULL, NULL, FALSE, 'r', ARG_INT, 0.0, 0, NULL},
    { "Number of one-line descriptions (V)", /* 13 */
      "500", NULL, NULL, FALSE, 'v', ARG_INT, 0.0, 0, NULL},
    { "Number of alignments to show (B)", /* 14 */
      "250", NULL, NULL, FALSE, 'b', ARG_INT, 0.0, 0, NULL},
    { "Threshold for extending hits, default if zero", /* 15 */
      "0", NULL, NULL, FALSE, 'f', ARG_INT, 0.0, 0, NULL},
    { "Perfom gapped alignment (not available with tblastx)", /* 16 */
      "T", NULL, NULL, FALSE, 'g', ARG_BOOLEAN, 0.0, 0, NULL},
    { "Query Genetic code to use", /* 17 */
      "1", NULL, NULL, FALSE, 'Q', ARG_INT, 0.0, 0, NULL},
    { "DB Genetic code (for tblast[nx] only)", /* 18 */
      "1", NULL, NULL, FALSE, 'D', ARG_INT, 0.0, 0, NULL},
    { "Number of processors to use", /* 19 */
      "1", NULL, NULL, FALSE, 'a', ARG_INT, 0.0, 0, NULL},
    { "SeqAlign file",          /* 20 */
      NULL, NULL, NULL, TRUE, 'O', ARG_FILE_OUT, 0.0, 0, NULL},
    { "Believe the query defline", /* 21 */
      "F", NULL, NULL, FALSE, 'J', ARG_BOOLEAN, 0.0, 0, NULL},
    { "Word size, default if zero", /* 22 */
      "0", NULL, NULL, FALSE, 'W', ARG_INT, 0.0, 0, NULL},
    { "Start of the sequence",  /* 23 */
      "1", NULL, NULL, FALSE, 'A', ARG_INT, 0.0, 0, NULL},
    { "End of the  sequence (-1 is entire sequence)", /* 24 */
      "-1", NULL, NULL, FALSE, 'B', ARG_INT, 0.0, 0, NULL},
    { "Matrix",                 /* 25 */
      "BLOSUM62", NULL, NULL, FALSE, 'M', ARG_STRING, 0.0, 0, NULL},
    { "Produce HTML output",    /* 26 */
      "F", NULL, NULL, FALSE, 'T', ARG_BOOLEAN, 0.0, 0, NULL},
    { "Restrict search of database to list of GI's", /* 27 */
      NULL, NULL, NULL, TRUE, 'l', ARG_STRING, 0.0, 0, NULL},
    { "Number of best hits from a region to keep", /* 28 */
      "100", NULL, NULL, FALSE, 'K', ARG_INT, 0.0, 0, NULL},
    { "Length of region used to judge hits", /* 29 */
      "20", NULL, NULL, FALSE, 'L', ARG_INT, 0.0, 0, NULL},
    { "Effective length of the search space (use zero for the real size)", /* 30 */
      "0", NULL, NULL, FALSE, 'Y', ARG_FLOAT, 0.0, 0, NULL},
    { "Query strands to search against database (for blast[nx], and tblastx).  3 is both, 1 is top, 2 is bottom", /* 31 */
      "3", NULL, NULL, FALSE, 'S', ARG_INT, 0.0, 0, NULL},
    { "RPS Blast search",       /* 32 */
      "F", NULL, NULL, FALSE, 'R', ARG_BOOLEAN, 0.0, 0, NULL},
    { "Restrict search of database to results of Entrez2 lookup", /* 33 */
      NULL, NULL, NULL, TRUE, 'u', ARG_STRING, 0.0, 0, NULL},
    { "MegaBlast search",       /* 34 */
      "F", NULL, NULL, FALSE, 'N', ARG_BOOLEAN, 0.0, 0, NULL}
};

#define MAX_NUM_QUERIES 16383 /* == 1/2 INT2_MAX */
#define MAX_TOTAL_LENGTH 10000000
/*********************************************************************
*	"main" function to call blast for the client.  
*
*	This function checks the command-line arguments, opens the
*	connection to the server, processes all the entries in
*	the FASTA file (obtained using FastaToSeqEntry), and
*	closes the connection.
*********************************************************************/
Int2 Main (void)
 
{
    BLAST_OptionsBlkPtr	options;
    BLAST_KarlinBlkPtr	ka_params=NULL, ka_params_gap=NULL;
    BlastResponsePtr	response = NULL;
    BioseqPtr	query_bsp;
    BioSourcePtr source;
    BlastNet3Hptr	bl3hp;
    BlastVersionPtr	blast_version;
    Boolean		db_is_na, query_is_na, show_gi, believe_query=FALSE;
    Boolean		html=FALSE, status;
    CharPtr		ret_buffer=NULL, params_buffer=NULL;
    CharPtr		date, motd, version;
    Int2		num_of_queries, retval;
    Int4		number_of_descriptions, number_of_alignments;
    SeqEntryPtr	sep;
    SeqIdPtr	seqid_list=NULL;
    TxDfDbInfoPtr	dbinfo=NULL;
    Uint1		align_type, align_view;
    Uint4		align_options, print_options;
    Int4		startloc, endloc;
    SeqLocPtr	slp;
    
    CharPtr blast_database, blast_inputfile, blast_outputfile;
    Char    blast_program[32];
    
    FILE *infp, *outfp;
    
    if (! GetArgs ("blastcl3", NUMARGS, myargs)) {
        return (1);
    }
    
    UseLocalAsnloadDataAndErrMsg ();

    ErrSetFatalLevel (SEV_MAX); /* never die from ErrPostEx */
    
    if (! SeqEntryLoad())
        return 1;
    
    StringCpy(blast_program, myargs [0].strvalue);

    /* For RPS Blast - anything not "blastp" - is "tblastn" */
    
    if(myargs[32].intvalue) {
        if(StringICmp(blast_program, "blastp")) {
            StringCpy(blast_program, "blastx");
        }
    }

    blast_database = myargs [1].strvalue;
    blast_inputfile = myargs [2].strvalue;
    blast_outputfile = myargs [5].strvalue;
    if (myargs[26].intvalue)
        html = TRUE;
    
    if ((infp = FileOpen(blast_inputfile, "r")) == NULL) {
        ErrPostEx(SEV_FATAL, 0, 0, "blast: Unable to open input file %s\n", blast_inputfile);
        return (1);
    }
    
    outfp = NULL;
    if (blast_outputfile != NULL) {
        if ((outfp = FileOpen(blast_outputfile, "w")) == NULL) {
            ErrPostEx(SEV_FATAL, 0, 0, "blast: Unable to open output file %s\n", blast_outputfile);
            return (1);
        }
    }
    
    align_view = (Int1) myargs[4].intvalue;
    
    align_type = BlastGetTypes(blast_program, &query_is_na, &db_is_na);
    
    if(align_type == blast_type_undefined)
        return 1;
    
    if (StringICmp("blastx", blast_program) == 0) {
        if (align_view != 0) {
            ErrPostEx(SEV_FATAL, 0, 0, "This option is not available with blastx");
            return 1;
        }
    } else if (StringICmp("tblastx", blast_program) == 0) {
        if (align_view != 0) {
            ErrPostEx(SEV_FATAL, 0, 0, "This option is not available with tblastx");
            return 1;
        }
    }
    
    believe_query = FALSE;
    if (myargs[21].intvalue != 0)
        believe_query = TRUE;
    
    if (believe_query == FALSE && myargs[20].strvalue) {
        ErrPostEx(SEV_FATAL, 0, 0, "-J option must be TRUE to produce a SeqAlign file");
    }
    
    options = BLASTOptionNew(blast_program, (Boolean) myargs [16].intvalue);
    options->is_megablast_search = (Boolean) myargs[34].intvalue;
    if (options->is_megablast_search) 
       options->wordsize = 0;
    if (options == NULL)
        return 3;

    /* If option RPS Blast set - option "program" is ignored by
       the engine */
    
    if(myargs[32].intvalue) 
        options->is_rps_blast = TRUE;
    
    BLASTOptionSetGapParams(options, myargs[25].strvalue, 0, 0);
    options->expect_value  = (Nlm_FloatHi) myargs [3].floatvalue;
    number_of_descriptions = myargs[13].intvalue;	
    number_of_alignments = myargs[14].intvalue;	
    options->hitlist_size = MAX(number_of_descriptions, number_of_alignments);
    if (myargs[7].intvalue != 0)
        options->gap_open = myargs[7].intvalue;
    if (myargs[8].intvalue != 0)
        options->gap_extend = myargs[8].intvalue;
    if (myargs[9].intvalue != 0)
        options->gap_x_dropoff = myargs[9].intvalue;
    if (StringICmp(myargs[6].strvalue, "T") == 0) {
        if (StringICmp("blastn", blast_program) == 0)
            options->filter_string = StringSave("D");
        else
            options->filter_string = StringSave("S");
    } else {
        options->filter_string = StringSave(myargs[6].strvalue);
    }
    
    show_gi = (Boolean) myargs[10].intvalue;
    if (StringICmp("blastn", blast_program) == 0) {
        options->penalty = myargs[11].intvalue;
        options->reward = myargs[12].intvalue;
    } else {
        if (myargs[15].intvalue != 0) {
            options->threshold_first = myargs[15].intvalue;
            options->threshold_second = myargs[15].intvalue;
        }
    }
    
    options->genetic_code = myargs[17].intvalue;
    options->db_genetic_code = myargs[18].intvalue;
    options->number_of_cpus = myargs[19].intvalue;
    if (myargs[22].intvalue != 0) {
        options->wordsize = myargs[22].intvalue;
        if (options->is_megablast_search)
           options->wordsize += 4;
    }
    
    print_options = 0;
    align_options = 0;
    align_options += TXALIGN_COMPRESS;
    align_options += TXALIGN_END_NUM;
    if (StringICmp("blastx", blast_program) == 0) {
        align_options += TXALIGN_BLASTX_SPECIAL;
    }
    if (show_gi) {
        align_options += TXALIGN_SHOW_GI;
        print_options += TXALIGN_SHOW_GI;
    }
    if (myargs[16].intvalue == 0)
        print_options += TXALIGN_SHOW_NO_OF_SEGS;
    
    if (align_view) {
        align_options += TXALIGN_MASTER;
        if (align_view == 1 || align_view == 3)
            align_options += TXALIGN_MISMATCH;
        if (align_view == 3 || align_view == 4 || align_view == 6)
            align_options += TXALIGN_FLAT_INS;
        if (align_view == 5 || align_view == 6)
            align_options += TXALIGN_BLUNT_END;
    } else {
        align_options += TXALIGN_MATRIX_VAL;
        align_options += TXALIGN_SHOW_QS;
    }
    
    if (html) {
        align_options += TXALIGN_HTML;
        print_options += TXALIGN_HTML;
    }
    
    if (myargs[27].strvalue) {
        options->gifile = StringSave(myargs[27].strvalue);
    }
    
    options->hsp_range_max  = myargs[28].intvalue;
    if (options->hsp_range_max != 0 && !options->is_megablast_search)
        options->perform_culling = TRUE;
    if (!options->is_megablast_search)
       options->block_width  = myargs[29].intvalue;
    else /* In megablast this has different meaning, will be default only */
       options->block_width = 0;

    if (myargs[29].floatvalue)
        options->searchsp_eff = (Nlm_FloatHi) myargs[30].floatvalue;
    
    options->strand_option = myargs[31].intvalue;
    
    if(myargs[33].strvalue)
        options->entrez_query = StringSave(myargs[33].strvalue);
    
    if (options->is_megablast_search) {
       if (options->wordsize == 0)
          options->wordsize = 32;
       options->cutoff_s = options->wordsize;
       options->cutoff_s2 = options->wordsize - 4;
    }


    if (! BlastInit("blastcl3", &bl3hp, &response)) {
        ErrPostEx(SEV_FATAL, 0, 0, "Unable to initialize BLAST service");
        FileClose(infp);
        FileClose(outfp);
        return (1);
    }
    if (response && response->choice == BlastResponse_init) {
        blast_version = response->data.ptrvalue;
        version = StringSave(blast_version->version);
        date = StringSave(blast_version->date);
    } else {
        ErrPostEx(SEV_FATAL, 0, 0, "Unable to connect to service");
        FileClose(infp);
        FileClose(outfp);
        return 1;
    }

    BlastResponseFree(response);

    BlastNetBioseqFetchEnable(bl3hp, blast_database, db_is_na, TRUE);
    
    if(!myargs[32].intvalue) {
        motd = Blast3GetMotd(bl3hp);
        PrintMotd(motd, outfp, html);
        motd = MemFree(motd);
    }

    if (html)
        fprintf(outfp, "<PRE>\n");
    
    if(options->is_rps_blast == TRUE)
        BlastPrintVersionInfoEx("RPS-BLAST", html, version, date, outfp);
    else {
	init_buff_ex(90);
        BlastPrintVersionInfoEx(blast_program, html, version, date, outfp);
        fprintf(outfp, "\n");
        BlastPrintReference(html, 80, outfp);
	free_buff();
    }

    MemFree(version);
    MemFree(date);

    fprintf(outfp, "\n");
    num_of_queries=0;
    retval=0;

    if (options->is_megablast_search) {
       Int4 total_length, num_bsps;
       Boolean done;
       SeqLocPtr last_mask, mask_slp;
       Int2 ctr = 1;
       Char prefix[2];

       StrCpy(prefix, "");
       done = FALSE;
       slp = NULL;
       while (!done) {
	  num_bsps = 0;
	  total_length = 0;
	  done = TRUE;
	  SeqMgrHoldIndexing(TRUE);
	  mask_slp = last_mask = NULL;
	  while ((sep=FastaToSeqEntryForDb(infp, query_is_na, NULL,
					   believe_query, prefix, &ctr, 
					   &mask_slp)) != NULL) {
	     
	     if (mask_slp) {
		if (!last_mask)
		   options->query_lcase_mask = last_mask = mask_slp;
		else {
		   last_mask->next = mask_slp;
		   last_mask = last_mask->next;
		}
		mask_slp = NULL;
	     }
	     query_bsp = NULL;
	     if (query_is_na) 
		SeqEntryExplore(sep, &query_bsp, FindNuc);
	     else
		SeqEntryExplore(sep, &query_bsp, FindProt);
	     
	     if (query_bsp == NULL) {
		ErrPostEx(SEV_FATAL, 0, 0, "Unable to obtain bioseq\n");
		return 2;
	     }
	     
	     ValNodeAddPointer(&slp, SEQLOC_WHOLE,
			       SeqIdDup(SeqIdFindBest(query_bsp->id,
						      SEQID_GI)));
	     num_bsps++;
	     total_length += query_bsp->length;
	     if (total_length > MAX_TOTAL_LENGTH ||
		 num_bsps >= MAX_NUM_QUERIES) {
		done = FALSE;
		break;
	     }
	     sep = MemFree(sep); /* Do not free the underlying Bioseq */
	  }
	  SeqMgrHoldIndexing(FALSE);
	  if (num_bsps > 0)
	     status = TraditionalBlastReportLoc(slp, options, bl3hp, blast_program, blast_database, FALSE, outfp, TRUE, print_options, align_options, number_of_descriptions, number_of_alignments, NULL);
	  if (status == FALSE)
	     ErrPostEx(SEV_ERROR, 0, 0, 
		       "An error has occurred on the server\n");
	  slp = SeqLocSetFree(slp);
       }
    } else {
       while ((sep = FastaToSeqEntryEx(infp, query_is_na, NULL, believe_query)) != NULL) {
        query_bsp = NULL;
        SeqEntryExplore(sep, &query_bsp, query_is_na? FindNuc : FindProt);
        
        /* Read boundaries of location */
        startloc = myargs[23].intvalue - 1;
        if (myargs[24].intvalue == -1)
            endloc = query_bsp->length - 1;
        else
            endloc = myargs[24].intvalue - 1;
        
        if (query_bsp == NULL) {
            ErrPostEx(SEV_FATAL, 0, 0, "Unable to obtain bioseq\n");
            retval = 2;
            break;
        }

        source = BioSourceNew();
        source->org = OrgRefNew();
        source->org->orgname = OrgNameNew();
        source->org->orgname->gcode = options->genetic_code;
        ValNodeAddPointer(&(query_bsp->descr), Seq_descr_source, source);

	init_buff_ex(85);
        AcknowledgeBlastQuery(query_bsp, 70, outfp, FALSE, html);
	free_buff();
        
        if (startloc || endloc != query_bsp->length - 1) {
	   /* Create the SeqLoc */
	   slp = SeqLocIntNew(startloc, endloc, Seq_strand_both, query_bsp->id);
        
            status = TraditionalBlastReportLoc(slp, options, bl3hp, blast_program, blast_database, FALSE, outfp, TRUE, print_options, align_options, number_of_descriptions, number_of_alignments, NULL);
	    slp = SeqLocSetFree(slp);
        } else {
            status = TraditionalBlastReport(query_bsp, options, bl3hp,
					    blast_program, blast_database,
					    FALSE, outfp, TRUE, print_options,
					    align_options,
					    number_of_descriptions,
					    number_of_alignments, NULL);
	}
        if (status == FALSE)
            ErrPostEx(SEV_ERROR, 0, 0, "An error has occurred on the server\n");
        sep = SeqEntryFree(sep);
    }
    }
    options = BLASTOptionDelete(options);
    FileClose(infp);
    FileClose (outfp);
    BlastFini(bl3hp);
    return retval;
}
