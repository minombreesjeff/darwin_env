/**************************************************************************
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
*                                                                         *
**************************************************************************/
/* $Revision 1.0 $  
* $Log: vecscreen.c,v $
* Revision 6.6  2003/05/13 16:02:42  coulouri
* make ErrPostEx(SEV_FATAL, ...) exit with nonzero status
*
* Revision 6.5  2001/01/09 17:30:51  madden
* Fix umr
*
* Revision 6.4  2000/05/10 14:33:45  kitts
* Added extra error checks and messages
* Added "No hits" output moved from VSPrintListFromSeqLocs
* Fixed some memory leaks
*
* Revision 6.3  2000/05/01 17:10:17  kitts
* Added option for alternative output formats
* Added some missing HTML tags
* Removed redundant definitions
*
* Revision 6.2  2000/01/24 19:09:07  vakatov
* + #include <vecscrn.h>
*
* Revision 6.1  2000/01/20 18:58:30  madden
* Main file for vector screening
*
*/
#include <ncbi.h>
#include <objseq.h>
#include <objsset.h>
#include <sequtil.h>
#include <seqport.h>
#include <tofasta.h>
#include <blast.h>
#include <blastpri.h>
#include <simutil.h>
#include <txalign.h>
#include <gapxdrop.h>
#include <sqnutils.h>
#include <salsap.h>
#include <vecscrn.h>


#define NUMARG 4
#define VECSCREEN_INFO "VecScreen"

static Args myargs [NUMARG] = {
  { "Query File", 
	"stdin", NULL, NULL, FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  { "VecScreen report Output File", 
	"stdout", NULL, NULL, FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},
  { "Database", 
	"UniVec", NULL, NULL, FALSE, 'd', ARG_STRING, 0.0, 0, NULL},
  { "Output format:\n      0 = HTML format, with alignments\n      1 = HTML format, no alignments\n      2 = Text list, with alignments\n      3 = Text list, no alignments\n     ",
	"0", NULL, NULL, FALSE, 'f', ARG_INT, 0.0, 0, NULL}
};

Int2 Main (void)
 
{
	BioseqPtr query_bsp;
	BLAST_MatrixPtr matrix=NULL;
	BLAST_KarlinBlkPtr ka_params=NULL, ka_params_gap=NULL;
	Boolean db_is_na=TRUE, query_is_na=TRUE, believe_query=FALSE;
	Boolean html=TRUE, output_alignments=TRUE, output_blast_params=TRUE;
	Boolean screen_ok;
	CharPtr params_buffer=NULL;
	CharPtr database=NULL;
	Int2 hits;
	SeqAlignPtr seqalign;
	SeqAnnotPtr seqannot;
	SeqEntryPtr sep;
	TxDfDbInfoPtr dbinfo=NULL, dbinfo_head;
	Uint1 align_type;
	Uint4 align_options;
	ValNodePtr  mask_loc, vnp, vnp1=NULL, other_returns, error_returns;

	CharPtr blast_inputfile, blast_outputfile;
	FILE *infp, *outfp;

	if (! GetArgs ("vecscreen", NUMARG, myargs))
	{
		return (1);
	}

	UseLocalAsnloadDataAndErrMsg ();

	if (! SeqEntryLoad())
		return 1;

	ErrSetMessageLevel(SEV_WARNING);

	blast_inputfile = myargs [0].strvalue;
	blast_outputfile = myargs [1].strvalue;
	database = myargs[2].strvalue;
	if (myargs [3].intvalue > 1)
		html = FALSE;
	if (myargs [3].intvalue == 1 || myargs [3].intvalue == 3)
	{
		output_alignments = FALSE;
		output_blast_params = FALSE;
	}

	if ((infp = FileOpen(blast_inputfile, "r")) == NULL)
	{
		ErrPostEx(SEV_FATAL, 1, 0, "vecscreen: Unable to open input file %s\n", blast_inputfile);
		return (1);
	}

	outfp = NULL;
	if (blast_outputfile != NULL)
		outfp = FileOpen(blast_outputfile, "w");
	if (outfp == NULL)
	{
		ErrPostEx(SEV_FATAL, 1, 0, "vecscreen: Unable to open output file %s\n", blast_outputfile);
		return (1);
	}

	align_type = BlastGetTypes("blastn", &query_is_na, &db_is_na);

	align_options = 0;
	align_options += TXALIGN_COMPRESS;
	align_options += TXALIGN_END_NUM;
	align_options += TXALIGN_MATRIX_VAL;
	align_options += TXALIGN_SHOW_QS;

	if (html)
	{
		align_options += TXALIGN_HTML;
		fprintf(outfp, "<HTML>\n<TITLE>VecScreen Search Results</TITLE>\n");
		fprintf(outfp, "<BODY BGCOLOR=\"#FFFFFF\" LINK=\"#0000FF\" "
			"VLINK=\"#660099\" ALINK=\"#660099\">\n");
	}

	while ((sep=FastaToSeqEntryEx(infp, query_is_na, NULL, believe_query)) != NULL) 
	{
		query_bsp = NULL;
		SeqEntryExplore(sep, &query_bsp, FindNuc);
		if (query_bsp == NULL)
		{
			ErrPostEx(SEV_FATAL, 1, 0, "Unable to obtain bioseq\n");
			sep = SeqEntryFree(sep);
			return 2;
		}

		if (html)
		{
			fprintf(outfp, "<PRE>");
			init_buff_ex(90);
			BlastPrintVersionInfo("blastn", TRUE, outfp);
			fprintf(outfp, "\n");
			BlastPrintReference(TRUE, 90, outfp);
			fprintf(outfp, "\n");
			AcknowledgeBlastQuery(query_bsp, 70, outfp, believe_query, TRUE);
			PrintDbInformation(database, !db_is_na, 70, outfp, TRUE);
			free_buff();
		}
		else
		{
			VSPrintListIdLine (query_bsp, VECSCREEN_INFO, database, outfp);
		}

		error_returns = NULL;
		screen_ok = TRUE;
		hits = VSScreenSequence(query_bsp, NULL, database, &seqalign, &vnp1, &other_returns, &error_returns);

		if (hits == 0)
		{
		        if (query_bsp->length > 0)
		            fprintf (outfp, html ? "\n<B> ****** No hits found ******</B>\n\n\n" : "No hits found\n");
			else
			    fprintf (outfp, html ? "\n<B> *** No valid query sequence ***</B>\n\n\n" : "WARNING: No valid query sequence\n");
		}
		else if (hits > 0)
		{
		        if (html)
			    screen_ok = VSPrintOverviewFromSeqLocs(vnp1, query_bsp->length, outfp);
		        else
			    screen_ok = VSPrintListFromSeqLocs(vnp1, outfp);
		}
		else  /* hits < 0 */
		{
			ErrPostEx(SEV_ERROR, 0, 0, "VSScreenSequence: screen failed\n");
			fprintf (outfp, html ? "\n<B> *** Screen failed ***</B>\n\n\n" : "ERROR: Screen failed\n");
			screen_ok = FALSE;
		}


		BlastErrorPrint(error_returns);
		error_returns = BlastErrorChainDestroy (error_returns);


		fprintf (outfp, html ? "</PRE>\n" : "\n");


		for (vnp=vnp1; vnp; vnp = vnp->next)
		{
			SeqLocFree(vnp->data.ptrvalue);
		}
		vnp1 = ValNodeFree(vnp1);


		dbinfo = NULL;
		ka_params = NULL;
		ka_params_gap = NULL;
		params_buffer = NULL;
		mask_loc = NULL;
		for (vnp=other_returns; vnp; vnp = vnp->next)
		{
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


		ReadDBBioseqFetchEnable ("vecscreen", database, db_is_na, TRUE);

		if (seqalign)
		{
			seqannot = SeqAnnotNew();
			seqannot->type = 2;
			AddAlignInfoToSeqAnnot(seqannot, align_type);
			seqannot->data = seqalign;
			if (output_alignments && outfp)
			{	/* Uncacheing causes problems with ordinal nos. vs. gi's. */
				if (!html)
					fprintf (outfp, "Alignments\n");
				ShowTextAlignFromAnnot(seqannot, 60, outfp, NULL, NULL, align_options, NULL, mask_loc, FormatScoreFunc);
			}
			seqannot = SeqAnnotFree(seqannot);
		}


		matrix = BLAST_MatrixDestruct(matrix);

		if (html && output_blast_params)
			fprintf(outfp, "<PRE>\n");

		init_buff_ex(85);
		dbinfo_head = dbinfo;
		while (dbinfo)
		{
			if (output_blast_params)
				PrintDbReport(dbinfo, 70, outfp);
			dbinfo = dbinfo->next;
		}
		dbinfo_head = TxDfDbInfoDestruct(dbinfo_head);

		if (ka_params)
		{
			if (output_blast_params)
				PrintKAParameters(ka_params->Lambda, ka_params->K, ka_params->H, 70, outfp, FALSE);
			MemFree(ka_params);
		}

		if (ka_params_gap)
		{
			if (output_blast_params)
				PrintKAParameters(ka_params_gap->Lambda, ka_params_gap->K, ka_params_gap->H, 70, outfp, TRUE);
			MemFree(ka_params_gap);
		}

		if (output_blast_params)
			PrintTildeSepLines(params_buffer, 70, outfp);

		MemFree(params_buffer);
		free_buff();
		vnp = mask_loc;
		while (mask_loc) {
		        SeqLocSetFree(mask_loc->data.ptrvalue);
		        mask_loc = mask_loc->next;
		}
		ValNodeFree(vnp);
	
		ReadDBBioseqFetchDisable();
		other_returns = ValNodeFree(other_returns);
		sep = SeqEntryFree(sep);

		if (output_blast_params)
			fprintf(outfp, html ? "</PRE>\n\n" : "\n");

		if (!screen_ok)
		        return 3;

	}

	if (html)
		fprintf(outfp, "</BODY>\n</HTML>\n");

	FileClose(infp);

	return 0;
}


