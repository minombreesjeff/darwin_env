/* $Id: psiblast.c,v 1.5 2004/11/05 15:39:54 coulouri Exp $
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
* File Name:  $RCSfile: psiblast.c,v $
*
* Author:  Sergei Shavirin
*
* Initial Version Creation Date: 04/21/2000
*
* $Revision: 1.5 $
*
* File Description:
*         WWW PSI BLAST Main file
*
* $Log: psiblast.c,v $
* Revision 1.5  2004/11/05 15:39:54  coulouri
* bring in system includes after toolkit includes so that LONG_BIT is defined correctly on amd64
*
* Revision 1.4  2003/11/21 17:51:32  dondosha
* Minor bug fix
*
* Revision 1.3  2003/07/15 19:57:11  coulouri
* use sigaction()
*
* Revision 1.2  2003/05/09 21:09:04  dondosha
* Removed unused variables
*
* Revision 1.1  2002/08/06 19:18:53  dondosha
* Source for WWW BLAST server, initial revision.
*
* Revision 6.6  2000/11/03 20:38:41  shavirin
* Minor formating change.
*
* Revision 6.5  2000/10/31 20:19:39  shavirin
* Added printing progeress messages to avoid browser timeout.
*
* Revision 6.4  2000/08/10 18:18:26  shavirin
* Fixed problem with printing PHI Blast output.
*
* Revision 6.3  2000/08/10 14:40:50  shavirin
* Fixed typo.
*
* Revision 6.2  2000/08/09 20:32:18  shavirin
* Added hidden printing of posFreqs encoded array.
*
* Revision 6.1  2000/05/17 15:49:40  shavirin
* Initial revision.
*
*
* ==========================================================================
*/

#include <wwwblast.h>
#include <signal.h>

static CharPtr non_printed_tag[] = { "PSI_MATRIX", "PHI_PATTERN", 
                                     "checked_GI", "PSI_KARLIN_K",
                                     "PHI_BLAST", "defline", "NEXT_I",
                                     "E_THRESH", "STEP_NUMBER", NULL
};

#define DEFAULT_ETHRESH "0.001"

static void SigAlrmHandler(int i)
{
    i = 0;
}

/*
  Formats the evalue and bit_score in a standard way.
*/

static Int4 Sip2GI(SeqIdPtr sip)
{
    SeqIdPtr	sip_tmp;
    
    for(sip_tmp = sip; sip_tmp != NULL; sip_tmp = sip_tmp->next) {
	if(sip_tmp->choice == SEQID_GI) {
	    return sip_tmp->data.intvalue;
	}
    }
    return 0;
}

static void PSIPrintHiddenData(WWWInfoPtr info, Boolean is_phi_blast)
{
    Int4 i, j, number;
    CharPtr chptr;
    Boolean found;

    number = WWWGetNumEntries(info);

    for(i = 0; i < number; i++) {
        
        chptr = WWWGetNameByIndex(info, i);
        
        for(j = 0, found = FALSE; non_printed_tag[j] != NULL; j++) {
            if(!StringICmp(non_printed_tag[j], chptr)) {
		if(!StringICmp("PHI_PATTERN", chptr) && is_phi_blast)
                    found = FALSE;
                else {
                    found = TRUE;
                    break;
                }
            }
        }
        if(!found) {
	    fprintf(stdout, "<INPUT TYPE=\"hidden\" "
                    "NAME= \"%s\" VALUE=\"%s\">\n",
		    WWWGetNameByIndex(info, i), 
		    WWWGetValueByIndex(info, i));
	}
    }
    return;
}

static Boolean  TestConvergence(GIListPtr good, GIListPtr checked, 
	SeqAlignPtr goodseqaligns, SeqAlignPtr badseqaligns, Int2Ptr marks)
{
    Int4		gi;
    SeqAlignPtr 	gsl = goodseqaligns;
    SeqIdPtr		sip;
    Boolean		retval = TRUE, found;
    GIListPtr		gil = NULL;
    Int4		count;
    
    if(goodseqaligns == NULL) {
	return FALSE; 
    }
    /* Fill in marks list */

    for (gsl = goodseqaligns, count = 0; gsl; gsl = gsl->next, count++) {
    
	marks[count] = (Int2) 0;

	/* get SeqIdPtr using SeqAlignPtr */
	sip = TxGetSubjectIdFromSeqAlign(gsl);

	/* get GI from SeqIdPtr */
	gi = Sip2GI(sip);


	/* check if this gi presents in the list of previous cheched GI's */
	found = FALSE;
	for (gil = checked; !found && gil; gil = gil->next) {
	    if (gil->gi == gi) {
		found = TRUE;
		marks[count] += SEQ_ALIGN_MARK_PREVCHECKED;
	    }
	}

	/* check if this gi presents in the list of previous Good GI's */
	found = FALSE;
	for (gil = good; !found && gil; gil = gil->next) {
	    if (gil->gi == gi) {
		found = TRUE;
		marks[count] += SEQ_ALIGN_MARK_PREVGOOD;
	    }
	}
    }

    for (gsl = badseqaligns; gsl; gsl = gsl->next, count++) {
    
	marks[count] = (Int2) SEQ_ALIGN_MARK_PREVGOOD;

	/* get SeqIdPtr using SeqAlignPtr */
	sip = TxGetSubjectIdFromSeqAlign(gsl);

	/* get GI from SeqIdPtr */
	gi = Sip2GI(sip);


	/* check if this gi presents in the list of previous cheched GI's */
	found = FALSE;
	for (gil = checked; !found && gil; gil = gil->next) {
	    if (gil->gi == gi) {
		found = TRUE;
		marks[count] += SEQ_ALIGN_MARK_PREVCHECKED;
	    }
	}
    }
    /* marks is built, now scan needed lists and figure out if process is converged */

    /* Important:  Convergence is now defined as follows:
       Each seq which is good now, should have been checked on previous iteration
       */

    retval = TRUE;
    for (gsl = goodseqaligns, count = 0;
	    retval && gsl; gsl = gsl->next, count++) {
    
	/* get SeqIdPtr using SeqAlignPtr */
	sip = TxGetSubjectIdFromSeqAlign(gsl);

	/* get GI from SeqIdPtr */
	gi = Sip2GI(sip);

	if (!(marks[count] & SEQ_ALIGN_MARK_PREVCHECKED))
	    retval = FALSE;
    }
    return retval;
}

Int2 Main (void)
 
{
    WWWBlastInfoPtr theInfo;
    BLASTPrintDataPtr	print_data;
    PSIDataPtr		psidata; 
    Nlm_FloatHi		ethresh_old;
    Boolean		isConverged;
    SeqAlignPtr		BadSeqAlignments, GoodSeqAlignments, lastGood = NULL;
    Int2		countGood, countBad;
    Int2Ptr		marks;
    struct sigaction sa;
    sigset_t sigset;

    sigfillset(&sigset);
    sa.sa_mask = sigset;

    sa.sa_flags = SA_RESTART;
    sa.sa_handler = SigAlrmHandler;
    sigaction(SIGALRM, &sa, NULL);
    sigaction(SIGPIPE, &sa, NULL);
    
    UseLocalAsnloadDataAndErrMsg ();

    if (! SeqEntryLoad()) {
        fprintf(stdout, "Cannot read NCBI DATA directory\n");
        return 1;
    }
    
    /* This function will read posting data, set-up config file and
       write small message into logfile (if it exists) */
    
    if((theInfo = WWWBlastReadArgs(NULL)) == NULL)
        return 1;

    /* Read options into structure */
    if(!WWWCreateSearchOptions(theInfo)) {
        return 1;
    }

    /* validate them */
    if(!WWWValidateOptions(theInfo)) {
        return 1;
    }
    if (theInfo->options->ethresh == 0.0)
        theInfo->options->ethresh = atof(DEFAULT_ETHRESH);
    
    if (theInfo->is_phi_blast == FALSE) {
        if((print_data = PSIBlastSearch(theInfo)) == NULL ||
           print_data->seqalign == NULL) {
            fprintf(stdout, "<PRE>\n");
            fprintf(stdout, "\n\nNo hit found...\n\n");

            if(print_data != NULL) {
                
                /* We will print bottom of the page eventually */
                init_buff();

                if (print_data->ka_params_gap) {
                    PrintKAParameters(print_data->ka_params_gap->Lambda, 
                                      print_data->ka_params_gap->K, 
                                      print_data->ka_params_gap->H, 
                                      70, stdout, TRUE);
                }

                PrintTildeSepLines(print_data->buffer, 70, stdout);
                free_buff();
            }
            fprintf(stdout, "</PRE></BODY></HTML>\n");

            FileClose(stdout);
            return(0);
        }
    } else {
        /* PHI Blast can work only with 1 CPU ;-))) */
	theInfo->options->number_of_cpus = 1;
    	if((print_data = PHIBlastSearch(theInfo)) == NULL ||
           print_data->vnp == NULL || print_data->seqloc == NULL) {

            fprintf(stdout, "<PRE>\n");
            fprintf(stdout, "\n<BR>No hit found...<BR>\n");
            
            if(print_data != NULL) {
                
                /* We will print bottom of the page eventually */
                init_buff();                
                if (print_data->ka_params_gap) {                
                    PrintKAParameters(print_data->ka_params_gap->Lambda, 
                                      print_data->ka_params_gap->K, 
                                      print_data->ka_params_gap->H, 
                                      70, stdout, TRUE);
                }
                
                PrintTildeSepLines(print_data->buffer, 70, stdout);
                free_buff();
            }

            fprintf(stdout, "</PRE></BODY></HTML>\n");

            FileClose(stdout);
            return(0);
        }
    }

    /* At this point we have non-NULL seqalign and therefore can 
       print some results out */
    
    if((psidata = print_data->psidata) == NULL) {
        WWWBlastErrMessage(BLASTMiscError, NULL);
        BLASTPrintDataFree(print_data); 
        WWWBlastInfoFree(theInfo);
        return 1;
    }
    
    ethresh_old = theInfo->options->ethresh;
    
    if (theInfo->options->ethresh == 0.0)
    	ethresh_old = atof(DEFAULT_ETHRESH);
    
    isConverged = FALSE;
    if (theInfo->is_phi_blast == FALSE) {
        SplitSeqAlign(print_data->seqalign, &GoodSeqAlignments, 
                      &BadSeqAlignments, &lastGood, &marks, 
                      &countBad, &countGood, ethresh_old);
        
        isConverged = TestConvergence(psidata->PrevGoodGIs, 
                                      psidata->PrevCheckedGIs, 
                                      GoodSeqAlignments, 
                                      BadSeqAlignments, marks);
    }
    
    fprintf(stdout, "<FORM NAME=\"PSI_BLAST\" METHOD=\"POST\" "
            "ACTION=\"%s?step%d\" ENCTYPE= \"multipart/form-data\">\n", 
            getenv("SCRIPT_NAME") == NULL ? "" : getenv("SCRIPT_NAME"),
            /* WWWGetPort(theInfo->info), */ psidata->StepNumber);
    
    if(isConverged) {
        fprintf(stdout, "<FONT color=\"blue\">"
                "<HR><CENTER><b><h1>Converged !!!</h1></b></CENTER>"
                "</FONT><p>");
    }
    
    fprintf(stdout, "<HR>");
    
    fprintf(stdout, "<PRE><a href=\"#Evalue\">E-value</a> threshold for inclusion in PSI-Blast iteration %d: %lg <BR>", psidata->StepNumber+1, ethresh_old);
    fprintf(stdout, "E-value threshold for inclusion in PSI-Blast iteration %d: ", psidata->StepNumber+2);
    fprintf(stdout, "<INPUT TYPE=\"text\" NAME=\"E_THRESH\" VALUE=\"%lg\" MAXLENGTH=\"10\">\r", ethresh_old);
    
    fprintf(stdout, "<HR>\r");             
    fprintf(stdout, "<BR><BR>");   

    fprintf(stdout, "<INPUT TYPE=\"hidden\" NAME =\"STEP_NUMBER\" VALUE =\"%d\">\n", 
	    psidata->StepNumber+1);

    if (psidata->StepNumber > 0)
	fprintf(stdout, "<HR><CENTER><b> Results of PSI-Blast iteration %d</b></CENTER>\n", psidata->StepNumber);
    
    fflush(stdout);
    
    if (theInfo->is_phi_blast == FALSE) {
	PSIPrintOutput(theInfo, print_data, BadSeqAlignments, 
                       GoodSeqAlignments, lastGood, marks, countBad, 
                       countGood, ethresh_old);
    } else {
	PHIPrintOutput(theInfo, print_data, print_data->vnp, ethresh_old);
    }

    MemFree(marks);

    fprintf(stdout, "</PRE>\n");
    
    if (psidata->StepNumber > 0) {
        fprintf(stdout, "<INPUT TYPE=\"hidden\" "
                "NAME= \"PSI_MATRIX\" VALUE = \"%s\">\n", 
                psidata->matrix62);
        fprintf(stdout, "<INPUT TYPE=\"hidden\" "
                "NAME= \"POS_FREQS\" VALUE = \"%s\">\n", 
                psidata->CHARPosFreqs); 
        fprintf(stdout, "<INPUT TYPE=\"hidden\" "
                "NAME= \"PSI_KARLIN_K\" VALUE = \"%lf\">", psidata->karlinK);
    }
    
    PSIPrintHiddenData(theInfo->info, theInfo->is_phi_blast);
    
    fprintf(stdout, "</FORM>");

    if (theInfo->is_phi_blast == FALSE) {
        print_data->seqalign = NULL;     
        SeqAlignSetFree(GoodSeqAlignments);
        SeqAlignSetFree(BadSeqAlignments);
    }

    BLASTPrintDataFree(print_data); 
    WWWBlastInfoFree(theInfo);

    fflush(stdout);
    return 0;
}

