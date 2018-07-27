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
* File Name: netblap3.c
*
* Author: Tom Madden
*
* Version Creation Date:   05/8/97
*
* File Description: 
*       Application Programming Interface (API) for BLAST network server
*
* RCS Modification History:
* $Log: netblap3.c,v $
* Revision 1.90  2001/09/06 20:27:10  dondosha
* threshold_first removed from options
*
* Revision 1.89  2001/08/31 14:31:30  dondosha
* Correction to previous change needed for Mac
*
* Revision 1.88  2001/08/30 17:53:05  dondosha
* Fixed memory leak in Blast3GetDbinfo
*
* Revision 1.87  2001/05/02 19:42:48  egorov
* Make the NetBlastGetMatrix() external
*
* Revision 1.86  2001/04/26 21:55:43  juran
* Squelch compiler warnings.
*
* Revision 1.85  2001/04/02 18:49:13  dondosha
* Removed readdb_new_ex call
*
* Revision 1.84  2001/03/01 17:42:51  madden
* Removed unneeded line
*
* Revision 1.83  2001/03/01 15:16:46  dondosha
* Do pass matrix as well as pseudo frequences, if the former is provided
*
* Revision 1.82  2001/02/16 16:13:08  dondosha
* If posFreqs are present, do not copy matrix to and from net matrix
*
* Revision 1.81  2001/01/19 21:30:09  dondosha
* Call readdb...._ex functions to possibly save some time when finding db length
*
* Revision 1.80  2001/01/19 20:56:46  dondosha
* In NetDbinfo2TxDbinfo get database total length information from readdb
*
* Revision 1.79  2001/01/04 15:54:17  dondosha
* Added percent identity to the search parameters
*
* Revision 1.78  2000/12/19 18:41:37  madden
* Calls to BlastSetUserErrorString and BlastDeleteUserErrorString moved to blastool.c
*
* Revision 1.77  2000/11/16 22:26:31  dondosha
* Add endpoint results from Mega BLAST to search response
*
* Revision 1.76  2000/09/28 16:46:40  dondosha
* Changed MegaBlast related code to get a single SeqAlignPtr from server
*
* Revision 1.75  2000/09/20 22:10:25  madden
* Fix memory leaks
*
* Revision 1.74  2000/09/13 13:47:48  dondosha
* Typo fix
*
* Revision 1.73  2000/09/12 21:55:10  dondosha
* Pass the correct scoring matrix to ShowTextAlignFromAnnot
*
* Revision 1.72  2000/08/28 15:16:51  dondosha
* Added functionality to process megablast searches
*
* Revision 1.71  2000/08/03 20:54:51  shavirin
* Fixed some C++ specific errors.
*
* Revision 1.70  2000/08/03 18:32:27  shavirin
* Fixed bug in the function BlastNetMatrixToBlastMatrix()
*
* Revision 1.69  2000/06/22 18:01:49  shavirin
* Added check for 0s passed for required end and required start from
* the network.
*
* Revision 1.68  2000/06/19 19:02:52  dondosha
* Pass required_start and required_end between parameters and options - these are from and to positions on query
*
* Revision 1.67  2000/06/15 20:02:48  egorov
* Allow dispatcher == NULL in NetFini.  We need
* this when call SubmitRequest->ReestablishNetBlast() from an external
* program, eg. blastque
*
* Revision 1.66  2000/06/15 16:22:44  egorov
* Make SubmitRequest() external
*
* Revision 1.65  2000/06/07 17:16:17  shavirin
* Added handligng of posFreqs matrix - S&W blastpgp specific.
*
* Revision 1.64  2000/06/06 14:08:54  madden
* Fill in matrix name if NULL, required for ASN.1
*
* Revision 1.63  2000/05/30 15:08:57  shavirin
* Rolled back revision without swaping UIDs from Entrez query.
*
* Revision 1.60  2000/05/05 18:44:05  shavirin
* Added protection against uids== NULL in BLASTGetUidsFromQuery().
*
* Revision 1.59  2000/05/04 18:09:26  shavirin
* Added new function  BLASTGetUidsFromQuery().
*
* Revision 1.58  2000/04/28 18:05:46  shavirin
* Added parameter is_rps_blast to Network BLAST ASN.1
*
* Revision 1.57  2000/04/18 16:30:37  madden
* Fix memory leaks
*
* Revision 1.56  2000/01/20 18:37:13  madden
* Add vecscrn.h to includes
*
* Revision 1.55  2000/01/20 18:33:36  madden
* Use VSMakeSeqLoc in place of VSMakeDisplaySeqLoc
*
* Revision 1.54  2000/01/20 14:35:06  madden
* Updated VecScreen calls
*
* Revision 1.53  1999/12/03 16:43:22  egorov
* Add additional mutex in formating, which protects network fetch callbacks
* from MT-unsafe operations.
*
* Revision 1.52  1999/12/02 22:14:27  shavirin
* Fixed bug with printing H parameter in the bottom of Blast output.
*
* Revision 1.51  1999/11/24 21:42:30  vakatov
* Fixed for the C++ and/or MSVC DLL compilation
*
* Revision 1.50  1999/11/16 15:52:55  egorov
* Put additional mutex to protect GenericGetService function
*
* Revision 1.49  1999/11/12 16:35:00  shavirin
* Added adjustement of use_best_align parameters in OptionsToParameters
* and parameteresTooptions().
*
* Revision 1.48  1999/11/05 16:59:30  madden
* Fix more leaks
*
* Revision 1.47  1999/11/05 15:30:08  madden
* Fix some memory leaks
*
* Revision 1.46  1999/10/27 15:57:52  madden
* Changes for use_real_db_size
*
* Revision 1.45  1999/10/27 13:00:02  madden
* Changes to return-parts
*
* Revision 1.44  1999/09/22 14:06:26  madden
* NULL out gilist to prevent double freeing
*
* Revision 1.43  1999/08/19 19:37:48  shavirin
* Added new functions corresponding to the PHI/PSI Blast.
*
* Revision 1.41  1999/06/21 20:33:58  madden
* Plug some leaks
*
* Revision 1.40  1999/05/04 19:41:45  madden
* done now goes through callback
*
* Revision 1.39  1999/04/21 20:54:46  madden
* Make BlastBioseq non-private
*
* Revision 1.38  1999/04/20 14:51:35  madden
* BlastBioseqNetCore and BlastSeqLocNetCore have return status
*
* Revision 1.37  1999/04/16 19:05:40  madden
* Change done
*
* Revision 1.36  1999/04/16 15:53:25  madden
* Add TraditionalBlastReportExtra function
*
* Revision 1.35  1999/04/13 14:59:30  madden
* Add more options (searchsp, culling, strand)
*
* Revision 1.34  1999/04/02 16:25:53  madden
* Reestablish logic
*
* Revision 1.33  1999/03/19 21:47:38  madden
* Add Blast3GetDbinfo function
*
* Revision 1.32  1999/03/15 21:56:10  madden
* fix for error message that was freed
*
* Revision 1.31  1999/03/05 15:42:35  madden
* Fixed memory leaks
*
* Revision 1.30  1999/03/05 15:02:43  egorov
* Bug fixed.  Return NULL if no kablk found in the list of responses in the BlastGetKaParams function.
*
* Revision 1.29  1999/03/04 17:59:57  egorov
* Allow error_returns == NULL on input
*
* Revision 1.28  1999/03/01 20:55:59  egorov
* Fix infinite loop reestablishing connection to blast server (thanx to Sergei Shavirin)
*
* Revision 1.27  1999/01/12 21:05:58  victorov
* server will now report an error if the ni-queue if full
*
* Revision 1.26  1998/12/09 15:27:05  madden
* Add wordsize
*
* Revision 1.25  1998/11/03 21:46:19  egorov
* Add support for entrez-query and org-name
*
* Revision 1.24  1998/10/26 19:42:57  madden
* Check for NULL matrix or filter_string
*
* Revision 1.23  1998/10/06 18:28:07  egorov
* Fix MT problem with dispatcher_lock
*
* Revision 1.22  1998/09/22 16:12:53  egorov
* Use BlastErrorPrintExtra instead of BlastErrorPrint to redirect error messages both to log and to program output file
*
* Revision 1.21  1998/09/01 20:17:03  madden
*  Fixed uninitialzed problem in BlastNetBioseqFetchDisable, changed prototype
*
* Revision 1.20  1998/08/14 17:43:26  madden
* non-NULL f_order and g_order in formatting
*
* Revision 1.19  1998/08/14 16:04:48  egorov
* Create TLS for BlastNetFetchStruct to make it multi-thread safe.  Using some of global variables is commented out
*
* Revision 1.18  1998/07/28 16:57:51  egorov
* Make StringSave() for some CharPtr in assignments to save memory and avoid memory crashes
*
* Revision 1.16  1998/06/08 21:58:36  madden
* Check for NULL Bioseq in FetchFunc, print out Error message if NULL
*
* Revision 1.15  1998/05/08 21:40:15  vakatov
* fixed a tiny type cast(one more *)
*
* Revision 1.14  1998/05/08 20:56:22  madden
* Fix PC compile warnings, rename callback
*
* Revision 1.13  1998/05/08 01:08:09  madden
* Removed unused variables
*
* Revision 1.12  1998/04/24 18:35:50  madden
* Do not send Seq-descr to server
*
* Revision 1.11  1998/04/24 16:01:41  egorov
* Remove BlastErrorPrint from BlastSeqLocNetCore
*
* Revision 1.10  1998/04/23 14:19:31  egorov
* Allow other_returns in BlastSeqLocNet to be NULL
*
* Revision 1.9  1998/04/22 18:10:06  egorov
* Add support for SeqLoc to blastcl3
*
* Revision 1.8  1998/04/17 19:24:00  madden
* Transmit expect value, hitlist_size, and genetic codes to server
*
* Revision 1.7  1998/04/16 19:35:31  madden
* Added Int4Ptr arg to TraditionalBlastReport specifying the numbers of hits
*
* Revision 1.6  1998/04/09 16:16:33  madden
* Added BlastNetFetchCompare function
*
* Revision 1.5  1997/12/16 19:10:51  madden
* Fixed Codecenter errors
*
* Revision 1.4  1997/12/01 22:05:51  madden
* Changed call to BlastValidateOptionsEx
*
* Revision 1.3  1997/11/28 18:17:39  madden
* Changes for multiple db searches
*
* Revision 1.1  1997/10/08 19:27:20  madden
* Network support for gapped blast
*
*/

#include <ncbinet.h>
#include <ncbithr.h>
#include <txalign.h>
#include <jzmisc.h>
#include <blastpat.h>
#include <netblap3.h>
#include <vecscrn.h>
#include <ent2api.h>

#define BLAST_SERVER_RETRIES  2

#define BLASTNET_SHORT_BUFLEN 25
#define BLASTNET_BUF_SIZE 255
#define BLASTNET_INIT 0
#define BLASTNET_READY 1
#define BLASTNET_DISABLE 2

typedef struct _blastnetbioseqfetch {
	Uint1 BlastNetFetchState;
	CharPtr dbname;	/* Name of the database. */
	BlastNet3Hptr bl3hp;	/* Network connection. */
	Uint2 ctr;
	Boolean is_prot; /* Is it a protein or not. */
} BlastNetFetchStruct, PNTR BlastNetFetchStructPtr;

static TNlmTls blastnetfetch_tls = NULL;

static Int2 num_attached = 0;
static NI_DispatcherPtr dispatcher = NULL;
/* Mutex for dispatcher. */
TNlmMutex dispatcher_lock = NULL;

static TNlmMutex formating_mutex; /* Mutex to regulate formating in TraditionalBlastReport */

static Boolean BlastInitEx PROTO((CharPtr program_name, BlastNet3Hptr bl3hp, BlastResponsePtr PNTR respp, Boolean reesatblish));
 
static Boolean RealSubmitRequest PROTO((BlastNet3Hptr bl3hptr, BlastRequestPtr blreqp, BlastResponsePtr PNTR response, NetProgressCallback callback));

/* error_occurred and old_error_hook should not be accessed directly.
The functions BlastSetErrorStatus, BlastGetErrorStatus, and BlastSetErrorHook
will change these variables. */
static Boolean error_occurred;
static ErrHookProc old_error_hook;

/*
	The next five functions set an error hook and detect
	whether an error occurred (i.e., contact with the
	server was lost).

	BlastSetErrorHook should be called first, the function
	BlastErrHookProc is set as the "handler"; BlastSetErrorStatus
	should then be called to set the error status to FALSE; 
	BlastGetErrorStatus should be called to determine if an error 
	occurred; and then BlastResetOldHook should be called to restore 
	the original hook, in case BLAST is called from within another
	application that uses this (original) hook.

	BlastSetErrorStatus is also called, to set the error status
	sometimes if a BlastAsnRead fails.  This is taken as evidence
	of an error, even if none is reported.
*/

static int LIBCALLBACK
BlastErrHookProc(const ErrDesc *err)

{
	error_occurred = TRUE;
	return 1;
}

static void
BlastSetErrorHook(void)

{
	old_error_hook = Nlm_ErrSetHandler(BlastErrHookProc);
	return;
}

static void
BlastSetErrorStatus(Boolean status)

{
	error_occurred = status;
}

static Boolean
BlastGetErrorStatus(void)

{
	return error_occurred;
}

static void
BlastResetOldHook(void)

{
	Nlm_ErrSetHandler(old_error_hook);
	return;
}

/*****************************************************************************
*
*   NetInit ()
*
*****************************************************************************/

static Boolean NetInit(void)
{
	Boolean retval = FALSE;

	NlmMutexLockEx(&dispatcher_lock);

	if(num_attached++ > 0)
	{
	    NlmMutexUnlock(dispatcher_lock);
	    return TRUE;
	}
        
    	dispatcher = NI_GenericInit(NULL, NULL, TRUE, NULL, 0);

	if (dispatcher)
		retval = TRUE;

	NlmMutexUnlock(dispatcher_lock);

	return retval;
}


/*****************************************************************************
*
*   ForceNetInit ()
*
*****************************************************************************/

static Boolean ForceNetInit(void)
{
	Boolean retval;

	NlmMutexLockEx(&dispatcher_lock);

	num_attached = 0; /* force re-attempt to contact dispatcher */
	retval = NetInit();

	NlmMutexUnlock(dispatcher_lock);

	return retval;
}

/*****************************************************************************
*
*   NetFini ()

	BlastNet3Hptr bl3hp: was returned by BlastInit
	Boolean deallocate: should BlastNet3Hptr be deallocated (or will it be reused for
		a reconnection).
*
*****************************************************************************/

static Boolean NetFini(BlastNet3Hptr bl3hp, Boolean deallocate)
{
    NlmMutexLockEx(&dispatcher_lock);

	if (num_attached > 0)
		num_attached--;

	if (bl3hp != NULL)
	{
		NI_ServiceDisconnect(bl3hp->svcp);
		if (deallocate)
                    bl3hp = (BlastNet3Hptr) MemFree(bl3hp);

		if (num_attached == 0)
		{	/* Disconnect if last service to dispatcher. */
			if (dispatcher)
			    NI_EndServices (dispatcher);
			dispatcher = NULL;
		}
	}

	NlmMutexUnlock(dispatcher_lock);

    return TRUE;
}


/*****************************************************************************
*
*   GenericReestablishNet ()
*
*****************************************************************************/

static Boolean GenericReestablishNet(CharPtr svcName, Boolean showErrs, BlastNet3Hptr bl3hp)
{
    Handle mon = NULL;
    Boolean retval;
    CharPtr buf;

    buf = (CharPtr) MemNew(2 * StrLen(svcName) + 60);

    if (showErrs) {
        sprintf (buf, "Re-establishing %s Service", svcName);
        mon = MonitorStrNew(buf, 40);
        sprintf (buf, "Requesting %s service", svcName);
        MonitorStrValue((MonitorPtr) mon, buf);
    }

    NetFini(bl3hp, FALSE);

    retval = BlastInitEx(NULL, bl3hp, NULL, FALSE);

    if (!retval)
    {
        sprintf (buf, "%s get failed; re-contacting dispatcher", svcName);
        MonitorStrValue((MonitorPtr) mon, buf);
        retval = FALSE;
        if (ForceNetInit())
        { /* successfully established contact w/dispatcher */
            sprintf (buf, "%s get failed; re-requesting %s service",
                     svcName, svcName);
            MonitorStrValue((MonitorPtr) mon, buf);
	    retval = BlastInitEx(NULL, bl3hp, NULL, FALSE);
        }
        else {
            ErrPost(CTX_UNKNOWN, 1, "Unable to re-contact dispatcher");
            if (showErrs) {
                ErrShow();
            }
        }
    }

    MonitorFree((MonitorPtr) mon);

    if (! retval )
    {
        sprintf (buf, "Unable to re-establish %s service", svcName);
        ErrPost(CTX_UNKNOWN, 1, buf);
        if (showErrs) {
            ErrShow();
        }
    }

    MemFree(buf);
    return retval;
}

/*****************************************************************************
*
*   ReestablishNetBlast ()
*
*****************************************************************************/

static Boolean ReestablishNetBlast(BlastNet3Hptr bl3hp)
{
    return GenericReestablishNet("NETBLAST", TRUE, bl3hp);
}

/*****************************************************************************
*
*   BlastInit ()
*
*****************************************************************************/

static Boolean BlastInitEx (CharPtr program_name, BlastNet3Hptr bl3hp, BlastResponsePtr PNTR respp, Boolean reestablish)

{
	BlastRequestPtr request;
	BlastResponsePtr response;
	NI_HandPtr svcp = NULL;

	if (bl3hp == NULL)
	{
		return FALSE;
	}


	if (!NetInit())
    	{
       	 	ErrPostEx(SEV_ERROR, 0, 0, "NI_ServiceGet [%s] (%s)", ni_errlist[ni_errno], ni_errtext);
		return FALSE;
	}

	NlmMutexLockEx(&dispatcher_lock);
    	svcp = NI_GenericGetService(dispatcher, NULL, "NETBLAST", "blast3", FALSE);
	NlmMutexUnlock(dispatcher_lock);

    	if (svcp == NULL)
    	{
       	 	ErrPostEx(SEV_ERROR, 0, 0, "NI_ServiceGet [%s] (%s)", ni_errlist[ni_errno], ni_errtext);
       	 	BlastFini(NULL);
       	 	return FALSE;
    	}

	bl3hp->svcp = svcp;


	request = ValNodeNew(NULL);
	request->choice = BlastRequest_init;

	SubmitRequest(bl3hp, request, &response, NULL, reestablish);
    	BlastRequestFree (request);

	if (respp)
		*respp = response;
	else
    		BlastResponseFree (response);

	return TRUE;

}

/*****************************************************************************
*
*   BlastInit ()
*
*****************************************************************************/

NLM_EXTERN Boolean LIBCALL
BlastInit (CharPtr program_name, BlastNet3Hptr PNTR bl3hpp, BlastResponsePtr PNTR respp)

{
	BlastNet3Hptr bl3hp_pri;

	if (bl3hpp == NULL)
	{
     ErrPostEx(SEV_ERROR, 0, 0, "BlastInitMT, BlastNet3Hptr PNTR is NULL");
     return FALSE;
	}

	bl3hp_pri = (BlastNet3Hptr) MemNew(sizeof(BlastNet3Hptr));
	if (bl3hp_pri == NULL)
		return FALSE;

	*bl3hpp = bl3hp_pri;

	return BlastInitEx(program_name, bl3hp_pri, respp, TRUE);
}

/*****************************************************************************
*
*   BlastFini ()
*
*****************************************************************************/

static Boolean s_BlastFini (BlastNet3Hptr bl3hptr)

{
	Boolean retval = TRUE;
	BlastRequestPtr request;
	BlastResponsePtr response;


	if (bl3hptr == NULL || bl3hptr->svcp == NULL)
		return FALSE;

        request = ValNodeNew(NULL);
        request->choice = BlastRequest_fini;
	SubmitRequest(bl3hptr, request, &response, NULL, FALSE);
        BlastRequestFree (request);
        BlastResponseFree (response);

	NetFini(bl3hptr, TRUE);
	return retval;
}

/* the only thing done here is to suppress errors */

NLM_EXTERN Boolean LIBCALL 
BlastFini (BlastNet3Hptr bl3hptr)

{
    short erract;
    ErrDesc err;
    Boolean retval;

    ErrGetOpts(&erract, NULL);
    ErrSetOpts(ERR_IGNORE, 0);
    ErrFetch(&err);

    retval = s_BlastFini(bl3hptr);

    ErrSetOpts(erract, 0);
    ErrFetch(&err);

    return retval;
}

NLM_EXTERN BlastParametersPtr LIBCALL
BlastOptionsToParameters (BLAST_OptionsBlkPtr options)

{
	BlastParametersPtr parameters;


	if (options == NULL)
	{
		return NULL;
	}

	parameters = BlastParametersNew();
	parameters->gapped_alignment = options->gapped_calculation;
	if (options->cutoff_s == 0)
	{
		parameters->Cutoff_cutoff = ValNodeAddFloat(NULL, Cutoff_cutoff_evalue, options->expect_value);
	}
	else
	{
		parameters->Cutoff_cutoff = ValNodeAddInt(NULL, Cutoff_cutoff_score, options->cutoff_s);
	}
	if (options->cutoff_s2 == 0)
	{
		parameters->Cutoff2_cutoff2 = ValNodeAddFloat(NULL, Cutoff2_cutoff2_evalue, options->e2);
	}
	else
	{
		parameters->Cutoff2_cutoff2 = ValNodeAddInt(NULL, Cutoff2_cutoff2_score, options->cutoff_s2);
	}
	parameters->hitlist_size = options->hitlist_size;
	parameters->first_threshold = options->threshold_second;
	parameters->second_threshold = options->threshold_second;
	parameters->nucl_penalty = options->penalty;
	parameters->nucl_reward = options->reward;
	parameters->gap_open = options->gap_open;
	parameters->gap_extend = options->gap_extend;
	parameters->genetic_code = options->genetic_code;
	parameters->db_genetic_code = options->db_genetic_code;
	parameters->low_complexity_filtering = options->filter;
	parameters->ethresh = options->ethresh;
        parameters->max_num_passes = options->maxNumPasses;
        parameters->pseudo_count_const = options->pseudoCountConst;

        parameters->gifile = StringSave(options->gifile);
        parameters->gilist = options->gilist;
        parameters->matrix = StringSave(options->matrix);
        parameters->filter_string = StringSave(options->filter_string);
	parameters->entrez_query = StringSave(options->entrez_query);
        parameters->word_size = options->wordsize;
        parameters->db_length = options->db_length;
        parameters->searchsp_eff = options->searchsp_eff;
        parameters->hsp_range_max = options->hsp_range_max;
        parameters->block_width = options->block_width;
        parameters->perform_culling = options->perform_culling;
        parameters->strand_option = options->strand_option;

        parameters->phi_pattern = StringSave(options->phi_pattern);
        parameters->use_real_db_size = options->use_real_db_size;
#if 0
        parameters->db_dir_prefix = StringSave(options->db_dir_prefix);
#endif
        parameters->use_best_align = options->use_best_align;

        if(options->required_start != 0)
            parameters->required_start = options->required_start;
        
        if(options->required_end != 0)
            parameters->required_end = options->required_end;

        parameters->is_rps_blast = options->is_rps_blast;
        parameters->tweak_parameters = options->tweak_parameters;
        parameters->smith_waterman = options->smith_waterman;
        parameters->is_megablast = options->is_megablast_search;
        parameters->query_lcase_mask = (ValNodePtr) options->query_lcase_mask;
        parameters->endpoint_results = options->no_traceback;
        parameters->percent_identity = (FloatHi) options->perc_identity;
	return parameters;
}

/*
	Translates the BlastDbinfoPtr into TxDfDbInfoPtr.
*/

NLM_EXTERN TxDfDbInfoPtr LIBCALL
NetDbinfo2TxDbinfo(BlastDbinfoPtr net_dbinfo)
{
	TxDfDbInfoPtr dbinfo=NULL, dbinfo_head;

	dbinfo_head = NULL;
	while (net_dbinfo)
	{
		dbinfo = TxDfDbInfoNew(dbinfo);
		dbinfo->is_protein = net_dbinfo->is_protein;
		dbinfo->name = StringSave(net_dbinfo->name);
		dbinfo->definition = StringSave(net_dbinfo->definition);
		dbinfo->date = StringSave(net_dbinfo->date);
                dbinfo->total_length = net_dbinfo->total_length;
                /* total length in net_dbinfo is Int4, might be incorrect */
                if (dbinfo->total_length < 0)
                   dbinfo->total_length += sizeof(Int4);
                dbinfo->number_seqs = net_dbinfo->number_seqs;
		if (dbinfo_head == NULL)
			dbinfo_head = dbinfo;
		net_dbinfo = net_dbinfo->next;
	}

	return dbinfo_head;
}

NLM_EXTERN BlastNet3BlockPtr LIBCALL
BlastNet3BlockDestruct(BlastNet3BlockPtr blnet)

{
	BlastResponsePtr next, response;

	if (blnet == NULL)
		return NULL;

	/* The entire gilist is not copied, so this prevents an error. */
	if (blnet->parameters)
		blnet->parameters->gilist = NULL;
	BlastParametersFree(blnet->parameters);
	/* individual elements freed elsewhere. */
	response = blnet->response;
	while(response)
	{
		next = response->next;
		response->data.ptrvalue = NULL;
		BlastResponseFree(response);
		response = next;
	}
	MemFree(blnet->dbname);
	MemFree(blnet);

	return NULL;
}

/*
	Creates a new BlastNet3BlockNew, used for searching.
*/
NLM_EXTERN BlastNet3BlockPtr LIBCALL
BlastNet3BlockNew(CharPtr program, CharPtr dbname)

{
	BlastNet3BlockPtr retval=NULL;

	retval = (BlastNet3BlockPtr) MemNew(sizeof(BlastNet3Block));

	if (retval)
	{
		if (StringICmp("blastn", program) == 0)
			retval->prog_type = Blast_search_program_blastn;
		else if (StringICmp("blastp", program) == 0)
			retval->prog_type = Blast_search_program_blastp;
		else if (StringICmp("blastx", program) == 0)
			retval->prog_type = Blast_search_program_blastx;
		else if (StringICmp("tblastn", program) == 0)
			retval->prog_type = Blast_search_program_tblastn;
		else if (StringICmp("tblastx", program) == 0)
			retval->prog_type = Blast_search_program_tblastx;
		if (dbname)
			retval->dbname = StringSave(dbname);
	}

	return retval;
}

static Boolean
QueryIsProteinFromType(Uint2 type)
{
	switch (type)
	{
		case Blast_search_program_blastn:
			return FALSE;
		case Blast_search_program_blastp:
			return TRUE;
		case Blast_search_program_blastx:
			return FALSE;
		case Blast_search_program_tblastn:
			return TRUE;
		case Blast_search_program_tblastx:
			return FALSE;
		default: 
			return FALSE;
	}
}

static BlastResponsePtr GetResponsePtr(BlastResponsePtr response, Nlm_Uint1 choice)

{
	while (response)
	{
		if (response->choice == choice)
		{
			break;
		}
		response = response->next;
	}

	return response;
}


NLM_EXTERN BlastDbinfoPtr LIBCALL
BlastRequestDbInfo (BlastNet3Hptr bl3hp, CharPtr database, Boolean is_prot)

{
	BlastRequestPtr request=NULL;
	BlastResponsePtr response=NULL;
	BlastDbinfoPtr dbinfo=NULL;
	BlastDbinfoGetPtr dbinfo_get;


	dbinfo_get = BlastDbinfoGetNew();
	dbinfo_get->name = database;

	if (is_prot)
		dbinfo_get->type = Blast_dbinfo_get_type_protein;
	else
		dbinfo_get->type = Blast_dbinfo_get_type_nucleotide;

	ValNodeAddPointer(&request, BlastRequest_db_info_specific, dbinfo_get);
	SubmitRequest(bl3hp, request, &response, NULL, TRUE);
	response = GetResponsePtr(response, BlastResponse_db_info_specific);

	if (response)
	{
		dbinfo = (BlastDbinfoPtr) response->data.ptrvalue;
		response->data.ptrvalue = NULL;
		BlastResponseFree(response);
	}

	dbinfo_get->name = NULL;	/* Not owned by this function. */
        BlastRequestFree(request);

	return dbinfo;
}

NLM_EXTERN BlastDbinfoPtr LIBCALL
BlastGetDbInfo (BlastNet3BlockPtr blnet3blkptr)

{
	BlastResponsePtr response;
	BlastDbinfoPtr dbinfo=NULL, dbinfo_head, last;

	last = NULL;
	dbinfo = NULL;
	dbinfo_head = NULL;
	response = blnet3blkptr->response;
	while (response)
	{
		response = GetResponsePtr(response, BlastResponse_db_info_specific);
		if (response)
		{
			last = dbinfo;
			dbinfo = (BlastDbinfoPtr) response->data.ptrvalue;
			if (dbinfo_head == NULL)
				dbinfo_head = dbinfo;
			if (last)
				last->next = dbinfo;
			response = response->next;	
		}
	}

	return dbinfo_head;
}

BlastMatrixPtr LIBCALL
NetBlastGetMatrix(BlastNet3BlockPtr blnet3blkptr)

{
	BlastResponsePtr response;
	BlastMatrixPtr matrix=NULL;
	

	response = GetResponsePtr(blnet3blkptr->response, BlastResponse_matrix);

	if (response)
		matrix = (BlastMatrixPtr) response->data.ptrvalue;

	return matrix;
}

NLM_EXTERN CharPtr LIBCALL
BlastGetParameterBuffer (BlastNet3BlockPtr blnet3blkptr)

{
	BlastResponsePtr response;
	CharPtr buffer=NULL;

	response = GetResponsePtr(blnet3blkptr->response, BlastResponse_parameters);

	if (response)
		buffer = (CharPtr) response->data.ptrvalue;

	return buffer;
}

/*
	Find the BlastKABlkPtr of the type (gapped or ungapped) specified.
*/
NLM_EXTERN BlastKABlkPtr LIBCALL
BlastGetKaParams (BlastNet3BlockPtr blnet3blkptr, Boolean gapped)

{
	BlastResponsePtr response;
	BlastKABlkPtr kablk=NULL;


	response = blnet3blkptr->response;
	while (response)
	{
		response = GetResponsePtr(response, BlastResponse_kablk);
		if (response)
		{
                    kablk = (BlastKABlkPtr) response->data.ptrvalue;
			if (kablk->gapped == gapped)
				break;
			response = response->next;	
			kablk = NULL;
		}
	}

	return kablk;
}

/*
	Converts 'standard' BLAST matrix to network matrix. 
*/
NLM_EXTERN BlastMatrixPtr LIBCALL
BlastMatrixToBlastNetMatrix(BLAST_MatrixPtr matrix)

{
    BlastMatrixPtr net_matrix;
    Int4 index1, index2;
    ValNodePtr vnp = NULL, newvnp = NULL;
    
    if (matrix == NULL)
        return NULL;
    
    net_matrix = (BlastMatrixPtr) MemNew(sizeof(BlastMatrix));
    
    net_matrix->is_protein = matrix->is_prot;

    if (matrix->name)
        net_matrix->name = StringSave(matrix->name);
    else
        net_matrix->name = StringSave("unknown");

    net_matrix->row_length = matrix->rows;
    net_matrix->column_length = matrix->columns;
    net_matrix->karlinK = matrix->karlinK;
    
    if(matrix->posFreqs != NULL) {
        for (vnp=NULL,index1 = matrix->rows-1; index1 >= 0; index1--) {
            for (index2 = matrix->columns-1; index2 >= 0; index2--) {
                newvnp = (ValNodePtr) Nlm_MemNew(sizeof(ValNode));
                newvnp->data.realvalue =  matrix->posFreqs[index1][index2];
                newvnp->next = vnp;
                vnp = newvnp;
            }
        }
        net_matrix->posFreqs = vnp;
    }
    if (matrix->matrix != NULL) {
       for (vnp=NULL,index1 = matrix->rows-1; index1 >= 0; index1--) {
          for (index2 = matrix->columns-1; index2 >= 0; index2--) {
             newvnp = (ValNodePtr) Nlm_MemNew(sizeof(ValNode));
             newvnp->data.intvalue =  matrix->matrix[index1][index2];
             newvnp->next = vnp;
             vnp = newvnp;
          }
       }
       net_matrix->scores = vnp;
    }
    return net_matrix;
    
}

/*
	Coverts the 'network' matrix to a 'tools' matrix.
*/
NLM_EXTERN BLAST_MatrixPtr LIBCALL
BlastNetMatrixToBlastMatrix (BlastMatrixPtr net_matrix)

{
    BLAST_MatrixPtr blast_matrix;
    Int4 index1, index2;
    Int4Ptr PNTR matrix;
    Nlm_FloatHi **posFreqs;
    ValNodePtr vnp;
    
    if (net_matrix == NULL)
        return NULL;
    
    blast_matrix = (BLAST_MatrixPtr) MemNew(sizeof(BLAST_Matrix));
    
    blast_matrix->is_prot = net_matrix->is_protein;
    blast_matrix->name = StringSave(net_matrix->name);
    blast_matrix->rows = net_matrix->row_length;
    blast_matrix->columns = net_matrix->column_length;
    blast_matrix->karlinK = net_matrix->karlinK;
    
    if(net_matrix->posFreqs != NULL) {
        vnp = net_matrix->posFreqs;
        posFreqs = (Nlm_FloatHi **) 
            MemNew(blast_matrix->rows*sizeof(Nlm_FloatHi *));

        for (index1=0; index1 < blast_matrix->rows; index1++) {
            posFreqs[index1] = (Nlm_FloatHi *) MemNew(blast_matrix->columns*sizeof(Nlm_FloatHi));
            for (index2=0; index2<blast_matrix->columns; index2++) {
                posFreqs[index1][index2] = (Nlm_FloatHi) vnp->data.realvalue;
                vnp = vnp->next;
            }
        }
        blast_matrix->posFreqs = posFreqs;
    } 
    if (net_matrix->scores != NULL) {
       vnp = net_matrix->scores;
       matrix = (Int4Ptr PNTR) MemNew(blast_matrix->rows*sizeof(Int4Ptr));
       for (index1=0; index1<blast_matrix->rows; index1++) {
          matrix[index1] = (Int4Ptr) MemNew(blast_matrix->columns*sizeof(Int4));
          for (index2=0; index2<blast_matrix->columns; index2++) {
             matrix[index1][index2] = (Int4) vnp->data.intvalue;
             vnp = vnp->next;
          }
       }
       blast_matrix->matrix = matrix;
    }


    return blast_matrix;
}

NLM_EXTERN ValNodePtr LIBCALL
BlastGetMaskedLoc (BlastNet3BlockPtr blnet3blkptr)

{
	BlastResponsePtr response;
	BlastMaskPtr blast_mask;
	ValNodePtr mask_loc=NULL;

	response = blnet3blkptr->response;

	while (response)
	{
		response = GetResponsePtr(response, BlastResponse_mask);
		if (response)
		{
			blast_mask = (BlastMaskPtr) response->data.ptrvalue;
			/* location is turned over to calling routine to be deallocated. */
			ValNodeAddPointer(&mask_loc, blast_mask->frame, blast_mask->location);
			blast_mask->location = NULL;
			BlastMaskFree(blast_mask);
			response = response->next;	
		}
	}

	return mask_loc;
}

static BioseqPtr 
PrivateBlastGetBioseq(BlastNet3Hptr bl3hptr, CharPtr database, SeqIdPtr sip, Boolean is_prot)

{
	BioseqPtr bsp=NULL;
	BlastSeqIdPtr blast_sip;
	BlastRequestPtr request = NULL;
	BlastResponsePtr response = NULL;

	blast_sip = BlastSeqIdNew();
	blast_sip->is_protein = is_prot;
	blast_sip->database = database;
	blast_sip->id = sip;
	ValNodeAddPointer(&request, BlastRequest_db_seq_get, blast_sip);
	SubmitRequest(bl3hptr, request, &response, NULL, TRUE);
	
	response = GetResponsePtr(response, BlastResponse_db_seq_get);
	if (response)
	{
		bsp = (BioseqPtr) response->data.ptrvalue;
		response->data.ptrvalue = NULL;
		BlastResponseFree(response);
	}

	/* These two were not allocated here. */
	blast_sip->id = NULL;
	blast_sip->database = NULL;
	BlastRequestFree(request);

	return bsp;
}

NLM_EXTERN BioseqPtr LIBCALL
BlastGetBioseq(BlastNet3BlockPtr blnet3blkptr, SeqIdPtr sip)

{
	BioseqPtr bsp;
	
	bsp = PrivateBlastGetBioseq(blnet3blkptr->bl3hptr, blnet3blkptr->dbname, sip, QueryIsProteinFromType(blnet3blkptr->prog_type));

	return bsp;
}

/*
	Basic functions to submit BLAST runs.

*/

NLM_EXTERN SeqAlignPtr LIBCALL
BlastBioseq (BlastNet3BlockPtr blnet3blkptr, ValNodePtr *error_returns, Boolean
             PNTR status, ValNodePtr *other_returns)

{
	BlastRequestPtr request = NULL;
	BlastSearchPtr search = NULL;
	BlastResponsePtr response = NULL;
	ValNodePtr node;
	SeqAlignPtr seqalign=NULL;
	/* Uint1 err_id; */

#if 0        
	err_id = BlastSetUserErrorString("netblast:", blnet3blkptr->bsp->id, TRUE);
#endif        

	search = BlastSearchNew();
	search->program = blnet3blkptr->prog_type;
/*
	search->query = (struct struct_Bioseq *) blnet3blkptr->bsp;
*/
	search->query = blnet3blkptr->bsp;
	search->database = blnet3blkptr->dbname;
	search->parameters = blnet3blkptr->parameters;
	search->mask = blnet3blkptr->mask;
	search->matrix =
	   BlastMatrixToBlastNetMatrix(blnet3blkptr->blast_matrix);
	search->query_set = blnet3blkptr->bsp_set;
	ValNodeAddPointer(&request, BlastRequest_search, search);
	*status = SubmitRequest(blnet3blkptr->bl3hptr, request, &response, blnet3blkptr->callback, TRUE);
	
	blnet3blkptr->response = response;
	node = GetResponsePtr(response, BlastResponse_alignment);
	if (node)
            seqalign = (SeqAlignPtr) node->data.ptrvalue;

        if (other_returns) { 
           /* MegaBLAST endpoint returns */
           node = GetResponsePtr(response, BlastResponse_mbalign);
           if (node)
              ValNodeAddPointer(other_returns, BlastResponse_mbalign, node->data.ptrvalue);
        }
	if (error_returns)
	{
	    node = GetResponsePtr(response, BlastResponse_error);
	    if (node)
	    	ValNodeAddPointer(error_returns, BlastResponse_error, node->data.ptrvalue);
	}

	/* These five are not allocated here. */
	search->query = NULL;
	search->database = NULL;
	search->parameters = NULL;
	search->mask = NULL;
	search->query_set = NULL;
        BlastRequestFree(request);
#if 0        
	BlastDeleteUserErrorString(err_id);
#endif        

	return seqalign;
}

/*
	Functions to submit BLAST runs and get back parts.

*/

static BlastPartsPtr 
BlastBioseqByParts (BlastNet3BlockPtr blnet3blkptr, ValNodePtr *error_returns, Boolean PNTR status)

{
	BlastRequestPtr request = NULL;
	BlastSearchPtr search = NULL;
	BlastResponsePtr response = NULL;
	ValNodePtr node;
	BlastPartsPtr blast_parts=NULL;
	/* Uint1 err_id; */

#if 0        
	err_id = BlastSetUserErrorString("netblast:", blnet3blkptr->bsp->id, TRUE);
#endif        

	search = BlastSearchNew();
	search->program = blnet3blkptr->prog_type;
/*
	search->query = (struct struct_Bioseq *) blnet3blkptr->bsp;
*/
	search->query = blnet3blkptr->bsp;
	search->database = blnet3blkptr->dbname;
	search->parameters = blnet3blkptr->parameters;
	search->mask = blnet3blkptr->mask;
	search->matrix = BlastMatrixToBlastNetMatrix(blnet3blkptr->blast_matrix);
	search->return_parts = TRUE;
	ValNodeAddPointer(&request, BlastRequest_search, search);
	*status = SubmitRequest(blnet3blkptr->bl3hptr, request, &response, blnet3blkptr->callback, TRUE);
	
	blnet3blkptr->response = response;
	node = GetResponsePtr(response, BlastResponse_parts);
	if (node)
            blast_parts = (BlastPartsPtr) node->data.ptrvalue;
	
	if (error_returns)
	{
	    node = GetResponsePtr(response, BlastResponse_error);
	    if (node)
	    	ValNodeAddPointer(error_returns, BlastResponse_error, node->data.ptrvalue);
	}

	/* These four are not allocated here. */
	search->query = NULL;
	search->database = NULL;
	search->parameters = NULL;
	search->mask = NULL;
        BlastRequestFree(request);
#if 0        
	BlastDeleteUserErrorString(err_id);
#endif        

	return blast_parts;
}

/* ------------ Set os functions to support PHI-Blast ----------- */

static BlastPhialignPtr LIBCALL
SeedBioseq (BlastNet3BlockPtr blnet3blkptr, ValNodePtr *error_returns, 
            Boolean PNTR status, ValNodePtr PNTR vnp_out)
     
{
    BlastRequestPtr request = NULL;
    BlastSearchPtr search = NULL;
    BlastResponsePtr response = NULL;
    ValNodePtr node, vnp = NULL;
    SeqAlignPtr seqalign=NULL;
    /* Uint1 err_id; */
    BlastPhialignPtr bphp = NULL;
    
    search = BlastSearchNew();
    search->program = blnet3blkptr->prog_type;
    search->query = blnet3blkptr->bsp;
    search->database = blnet3blkptr->dbname;
    search->parameters = blnet3blkptr->parameters;
    search->mask = blnet3blkptr->mask;
    search->matrix = BlastMatrixToBlastNetMatrix(blnet3blkptr->blast_matrix);
    ValNodeAddPointer(&request, BlastRequest_search, search);
    *status = SubmitRequest(blnet3blkptr->bl3hptr, request, &response, 
                            blnet3blkptr->callback, TRUE);
    
    blnet3blkptr->response = response;
    node = GetResponsePtr(response, BlastResponse_phialign);
    if (node)
        bphp = (BlastPhialignPtr) node->data.ptrvalue;
    
    if (error_returns) {
        node = GetResponsePtr(response, BlastResponse_error);
        if (node)
            ValNodeAddPointer(error_returns, BlastResponse_error, 
                              node->data.ptrvalue);
    }

    /* Now re-creating ValNode from SeqAligns */

    while (response) {
        if (response->choice == BlastResponse_alignment) {
            ValNodeAddPointer(&vnp, BlastResponse_alignment, 
                              response->data.ptrvalue);
        }
        response = response->next;
    }
    *vnp_out = vnp;
    response = blnet3blkptr->response;
    
    /* These four are not allocated here. */
    search->query = NULL;
    search->database = NULL;
    search->parameters = NULL;
    search->mask = NULL;
    BlastRequestFree(request);
#if 0        
    BlastDeleteUserErrorString(err_id);
#endif        
    
    return bphp;
}

NLM_EXTERN BlastPhialignPtr LIBCALL
SeedBioseqNetCore(BlastNet3Hptr bl3hp, BioseqPtr bsp, CharPtr program, 
                  CharPtr database, BLAST_OptionsBlkPtr options, 
                  ValNodePtr *other_returns, ValNodePtr *error_returns, 
                  NetProgressCallback callback, BLAST_MatrixPtr blast_matrix,
                  Boolean PNTR ret_status, ValNodePtr PNTR vnp_out)   
{
    BlastKABlkPtr ka_blk;
    BlastDbinfoPtr dbinfo;
    BlastMatrixPtr net_matrix;
    BLAST_MatrixPtr matrix;
    BlastNet3BlockPtr blnet;
    Boolean options_allocated = FALSE;
    CharPtr params_buffer;
    Int2 status;
    SeqAlignPtr seqalign = NULL;
    TxDfDbInfoPtr txdbinfo;
    ValNodePtr descr, mask;

    BlastPhialignPtr bphp = NULL;
    
    
    if (bl3hp == NULL || bsp == NULL || program == NULL || database == NULL)
        return NULL;
    
    if (error_returns)
        *error_returns = NULL;

    if (other_returns)
        *other_returns = NULL;

    /* If no options, use default. */
    if (options == NULL) {
        options = BLASTOptionNew(program, FALSE);
        options_allocated = TRUE;
    }
    
    status = BLASTOptionValidateEx(options, program, error_returns);
    if (status != 0) {	/* error messages in other_returns? */
        return NULL;
    }

    blnet = BlastNet3BlockNew(program, database);
    /* 
       Remove the Seq-descr as this is not needed for 
       BLASTing and the title often contains none-ASCII
       characters.  Keep the pointer and replace. 
       */
    descr = bsp->descr;
    bsp->descr = NULL;
    
    blnet->bsp = bsp;
    blnet->parameters = BlastOptionsToParameters(options);
    if (options_allocated) {
        options = BLASTOptionDelete(options);
    }

    blnet->callback = callback;
    blnet->bl3hptr = bl3hp;
    blnet->blast_matrix = blast_matrix;
    
    bphp = SeedBioseq(blnet, error_returns, ret_status, vnp_out);
    
    if (other_returns) {
        *other_returns = NULL;
        mask = BlastGetMaskedLoc(blnet);
        if (mask)
            ValNodeLink(other_returns, mask);
        dbinfo = BlastGetDbInfo(blnet);
        txdbinfo = NetDbinfo2TxDbinfo(dbinfo);
        ValNodeAddPointer (other_returns, TXDBINFO, txdbinfo);
        dbinfo = BlastDbinfoFree(dbinfo);
        params_buffer = BlastGetParameterBuffer(blnet);
        ValNodeAddPointer(other_returns, TXPARAMETERS, params_buffer);
        ka_blk = BlastGetKaParams(blnet, FALSE);
        if (ka_blk)
            ValNodeAddPointer (other_returns, TXKABLK_NOGAP, ka_blk);
        ka_blk = BlastGetKaParams(blnet, TRUE);
        if (ka_blk)
            ValNodeAddPointer (other_returns, TXKABLK_GAP, ka_blk);
        net_matrix = NetBlastGetMatrix(blnet);
        matrix = BlastNetMatrixToBlastMatrix(net_matrix);
        net_matrix = BlastMatrixFree(net_matrix);
        if (matrix)
            ValNodeAddPointer (other_returns, TXMATRIX, matrix);
    }
    
    blnet = BlastNet3BlockDestruct(blnet);
    
    bsp->descr = descr;
    
    return bphp;
}

/* -------------------------------------------------------------- */

NLM_EXTERN CharPtr LIBCALL 
Blast3GetMotd(BlastNet3Hptr bl3hptr)

{
	BlastResponsePtr response=NULL;
	BlastRequestPtr request=NULL;
	CharPtr string;
	
        request = ValNodeNew(NULL);
        request->choice = BlastRequest_motd;
        SubmitRequest(bl3hptr, request, &response, NULL, TRUE);
        BlastRequestFree (request);
	if (response == NULL || response->choice != BlastResponse_motd)
		return NULL;

	string = StringSave((CharPtr) response->data.ptrvalue);

	BlastResponseFree(response);

	return string;
}

NLM_EXTERN BlastDbinfoPtr LIBCALL 
Blast3GetDbinfo(BlastNet3Hptr bl3hptr)

{
	BlastResponsePtr response=NULL;
	BlastRequestPtr request=NULL;
	BlastDbinfoPtr dbinfo;
	
        request = ValNodeNew(NULL);
        request->choice = BlastRequest_db_info;
        SubmitRequest(bl3hptr, request, &response, NULL, TRUE);
        BlastRequestFree (request);
	if (response == NULL || response->choice != BlastResponse_db_info)
           return (BlastDbinfoPtr) BlastResponseFree(response);

	dbinfo = (BlastDbinfoPtr) response->data.ptrvalue;
        MemFree(response);

	return dbinfo;
}

NLM_EXTERN Boolean
SubmitRequest(BlastNet3Hptr bl3hptr, BlastRequestPtr blreqp, BlastResponsePtr PNTR response, NetProgressCallback callback, Boolean reestablish)

{
    Boolean status;
    Int2 index;

    for(index=0; index<BLAST_SERVER_RETRIES; index++)
    {

	status = RealSubmitRequest(bl3hptr, blreqp, response, callback);

        if (status == TRUE)
            break;

	if (reestablish)
		ReestablishNetBlast(bl3hptr);
    }

    return status;

}

/*
	Status returned indicates a (potential) error.  if not done, this is an error.
*/
#define PRINT_DEBUG_ASN 0
#if PRINT_DEBUG_ASN	
static	tmpi = 0;
#endif 

static Boolean
RealSubmitRequest(BlastNet3Hptr bl3hptr, BlastRequestPtr blreqp, BlastResponsePtr PNTR response, NetProgressCallback callback)

{
	AsnIoPtr asnin, asnout;
	Boolean cancel, done;
	BlastResponsePtr bllist, blresp, head;
#if PRINT_DEBUG_ASN	
        AsnIoPtr asnout_test;
	Char buf[1024];
#endif

	if (bl3hptr == NULL)
		return FALSE;

	if (response == NULL)
		return FALSE;

	asnout = bl3hptr->svcp->waip;
	asnin = bl3hptr->svcp->raip;

#if PRINT_DEBUG_ASN	
        sprintf(buf, "request.%d.out", ++tmpi);
        asnout_test = AsnIoOpen(buf, "w");
	BlastRequestAsnWrite(blreqp, asnout_test, NULL);
#endif	

	done = BlastRequestAsnWrite(blreqp, asnout, NULL);
	if (done == FALSE)
		return FALSE;

#if PRINT_DEBUG_ASN	
        AsnIoReset(asnout_test);
        AsnIoClose(asnout_test);   
#endif	
	AsnIoReset(asnout);

	head = NULL;
	done = FALSE;
	while (!done && (blresp = BlastResponseAsnRead(asnin, NULL)) != NULL)
	{
		switch (blresp->choice)
		{
		     case BlastResponse_done:
			done = TRUE;
		     case BlastResponse_queued:
		     case BlastResponse_start:
		     case BlastResponse_progress:
			if (callback)
			{
				if (callback(blresp, &cancel) != TRUE)
				{
					done = TRUE;
				}
			}
			blresp = BlastResponseFree(blresp);
			break;

		     case BlastResponse_init:
		     case BlastResponse_motd:
		     case BlastResponse_fini:
			done = TRUE;

		     default:
			if (head == NULL)
			{
				head = blresp;
				bllist = blresp;
			}
			else
			{
				bllist->next = blresp;
				bllist = bllist->next;
			}
			break;
		}
	}
	*response = head;
	return done;
}


static void BlastNetFetchCleanup (TNlmTls tls, VoidPtr ptr)
{
    BlastNetFetchStructPtr bnfsp = (BlastNetFetchStructPtr) ptr;
    
    if (bnfsp) {
        bnfsp->dbname = (CharPtr) MemFree(bnfsp->dbname);
        MemFree(bnfsp);
    }
    return;
}

/*
	Checks the chain of BlastNetFetchStructPtr's for one
	which belongs to the calling thread. If none is found,
	NULL isreturned; otherwise the BlastNetFetchStructPtr is
	returned.
*/
static BlastNetFetchStructPtr
BlastNetFindFetchStruct(BlastNet3Hptr bl3hp, CharPtr dbname, Boolean is_na)

{
	BlastNetFetchStructPtr	bnfsp = NULL;

	if (NlmTlsGetValue(blastnetfetch_tls, (VoidPtr *)(&bnfsp)))
	{
		if (bnfsp == NULL)
		{
			bnfsp = (BlastNetFetchStructPtr) MemNew(sizeof(BlastNetFetchStruct));
			bnfsp->dbname = StringSave(dbname);
			bnfsp->is_prot = (is_na == TRUE) ? FALSE : TRUE;
			bnfsp->bl3hp = bl3hp;
			bnfsp->BlastNetFetchState = BLASTNET_INIT;
			NlmTlsSetValue(&blastnetfetch_tls, bnfsp, 
				BlastNetFetchCleanup);
		}
	}

	return bnfsp;
}

static Boolean BlastNetInit(BlastNetFetchStructPtr bnfsp)

{
	return TRUE;

}


/**********************************************************************

	Fetches the Bioseq, based on the ordinal number of the
	sequence in the database.

************************************************************************/

static Int2 LIBCALLBACK BlastNetBioseqFetchFunc(Pointer data)
{
	Boolean status;
	Char buffer[64];
	OMProcControlPtr ompcp;
        ObjMgrProcPtr ompp;
	OMUserDataPtr omdp;
	SeqIdPtr sip, best_id;
	BlastNetFetchStructPtr blfsp;
	SeqEntryPtr sep;
	BioseqPtr bsp, core_bsp;

	ompcp = (OMProcControlPtr)data;
        ompp = ompcp->proc;

	blfsp = BlastNetFindFetchStruct(NULL, NULL, FALSE);

	if (blfsp->BlastNetFetchState == BLASTNET_INIT)
	{
		status = BlastNetInit(blfsp);
		if (status == FALSE)
			return OM_MSG_RET_OK;
		blfsp->BlastNetFetchState = BLASTNET_READY;
	}

	sip = (SeqIdPtr) (ompcp->input_data);

	best_id = SeqIdFindBest(sip, SEQID_GI);

	if (best_id == NULL)
	{
		core_bsp = BioseqFindCore(sip);
		if (core_bsp)
			best_id = SeqIdFindBest(core_bsp->id, SEQID_GI);
	}

	if (best_id == NULL)
		return OM_MSG_RET_OK;

        
	/* A BioseqPtr is returned by this function. */
	bsp = PrivateBlastGetBioseq(blfsp->bl3hp, blfsp->dbname, best_id, blfsp->is_prot);
        if (bsp == NULL)
        {
                SeqIdWrite(best_id, buffer, PRINTID_FASTA_LONG, sizeof(buffer));
                ErrPost(CTX_UNKNOWN, 1, "Unable to retrieve %s", buffer);
                return OM_MSG_RET_ERROR;
        }
	sep = SeqEntryNew();
	sep->choice = 1;
	sep->data.ptrvalue = bsp;
	SeqMgrSeqEntry(SM_BIOSEQ, (Pointer)bsp, sep);
	ompcp->output_data = (Pointer)bsp;
	ompcp->output_entityID = ObjMgrGetEntityIDForChoice(sep);
	omdp = ObjMgrAddUserData(ompcp->output_entityID, ompp->procid, OMPROC_FETCH, 0);

	return OM_MSG_RET_DONE;
}

/*
	Compare BlastNetFetchStructPtr structures with some traits.  TRUE if identical, otherwise
	FALSE.
*/
static Boolean
BlastNetFetchCompare (BlastNetFetchStructPtr blfsp1, BlastNet3Hptr bl3hp, CharPtr dbname, Boolean is_na)

{
	if (blfsp1 == NULL)
		return FALSE;

	if (StringCmp(blfsp1->dbname, dbname) != 0)
		return FALSE; 
	if (blfsp1->is_prot == is_na)
		return FALSE; 
	if (blfsp1->bl3hp != bl3hp)
		return FALSE; 

	return TRUE;
}

/*********************************************************************

	Enables the fetching.  Initializes needed structures and calls
	BlastNetInit.

**********************************************************************/

NLM_EXTERN Boolean LIBCALL 
BlastNetBioseqFetchEnable(BlastNet3Hptr bl3hp, CharPtr dbname, Boolean is_na, Boolean now)

{
        Boolean result;
        BlastNetFetchStructPtr blfsp = NULL;
        ObjMgrPtr omp;
        ObjMgrProcPtr ompp;

              /* check if already enabled ***/

        omp = ObjMgrGet();
        ompp = ObjMgrProcFind(omp, 0, "BlastNetBioseqFetch", OMPROC_FETCH);
        if (ompp != NULL)   /* already initialized */
	{
		blfsp = BlastNetFindFetchStruct(bl3hp, dbname, is_na);
			if (BlastNetFetchCompare(blfsp, bl3hp, dbname, is_na) == FALSE)
			{
				blfsp->dbname = (CharPtr) MemFree(blfsp->dbname);
				blfsp->dbname = StringSave(dbname);
				blfsp->is_prot = (is_na == TRUE) ? FALSE : TRUE;
				blfsp->bl3hp = bl3hp;
				
			}
	}
	else
	{
		blfsp = BlastNetFindFetchStruct(bl3hp, dbname, is_na);

		ObjMgrProcLoad(OMPROC_FETCH, "BlastNetBioseqFetch", 
			"BlastNetBioseqFetch", OBJ_SEQID, 0,OBJ_BIOSEQ,0,
                        (Pointer)blfsp, BlastNetBioseqFetchFunc, PROC_PRIORITY_DEFAULT);

		blfsp->BlastNetFetchState = BLASTNET_INIT;
	}

	blfsp->ctr++;    /* count number of enables */

	if (blfsp->BlastNetFetchState == BLASTNET_READY)
	{
			  return TRUE;
	}

        if (now)
        {
		result = BlastNetInit(blfsp);
                if (! result)
                {
                        return result;
                }
		blfsp->BlastNetFetchState = BLASTNET_READY;
        }
        else
	{
		blfsp->BlastNetFetchState = BLASTNET_INIT;
	}

        return TRUE;
}

/*****************************************************************************
*
*		BlastNetBioseqFetchDisable()
*
*	Calls readdb_destruct if necessary to deallocate resources.
*
*****************************************************************************/
NLM_EXTERN void LIBCALL BlastNetBioseqFetchDisable(BlastNet3Hptr bl3hp, CharPtr dbname, Boolean is_na)
{
        ObjMgrPtr omp;
        ObjMgrProcPtr ompp;
        BlastNetFetchStructPtr blfsp;

        omp = ObjMgrGet();
        ompp = ObjMgrProcFind(omp, 0, "BlastNetBioseqFetch", OMPROC_FETCH);
        if (ompp == NULL)   /* not initialized */
                return;

	blfsp = BlastNetFindFetchStruct(bl3hp, dbname, is_na);
	if (! blfsp->ctr)   /* no enables active */
		return;

	blfsp->ctr--;
	if (blfsp->ctr)   /* connection still pending */
			  return;

        if (blfsp->BlastNetFetchState == BLASTNET_READY)
	{
		blfsp->BlastNetFetchState = BLASTNET_DISABLE;  /* not active */
	}

        return;
}

/*
	Runs a BLAST request and returns a SeqAlignPtr for formatting.
	Note that the network connection must be established beforehand
	(i.e., BlastNet3BlockPtr blnet should be initialized).
*/
NLM_EXTERN SeqAlignPtr LIBCALL
BlastBioseqNet(BlastNet3Hptr bl3hp, BioseqPtr bsp, CharPtr program, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, NetProgressCallback callback)

{
	Boolean status;

	return BlastBioseqNetCore(bl3hp, bsp, program, database, options, other_returns, error_returns, callback, NULL, &status);
}

NLM_EXTERN SeqAlignPtr LIBCALL
BlastSeqLocNet(BlastNet3Hptr bl3hp, SeqLocPtr slp, CharPtr program, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, NetProgressCallback callback)

{
	Boolean status;

	return BlastSeqLocNetCore(bl3hp, slp, program, database, options, other_returns, error_returns, callback, NULL, &status);
}

NLM_EXTERN SeqAlignPtr LIBCALL
MegaBlastSeqLocNetCore(BlastNet3Hptr bl3hp, SeqLocPtr slp, CharPtr program, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, NetProgressCallback callback, Boolean PNTR ret_status)
{
	BlastKABlkPtr ka_blk;
	BlastDbinfoPtr dbinfo;
	BlastNet3BlockPtr blnet;
	Boolean options_allocated = FALSE;
	CharPtr params_buffer;
	BlastMatrixPtr net_matrix;
	BLAST_MatrixPtr matrix;
	Int2 status;
	SeqAlignPtr seqalign = NULL;
	TxDfDbInfoPtr txdbinfo;
	ValNodePtr mask;
	SeqEntryPtr sep;
	BioseqPtr bsp;
	Uint2 entityID = 0;

	if (bl3hp == NULL || slp == NULL || program == NULL || database == NULL)
		return NULL;

	if (error_returns)
		*error_returns = NULL;

	if (other_returns)
		*other_returns = NULL;

	/* If no options, use default. */
	if (options == NULL)
	{
		options = BLASTOptionNew(program, FALSE);
		options_allocated = TRUE;
	}

	status = BLASTOptionValidateEx(options, program, error_returns);
	if (status != 0)
	{	/* error messages in other_returns? */
		return NULL;
	}

	blnet = BlastNet3BlockNew(program, database);

	blnet->bsp_set = BioseqSetNew();
	sep = NULL;
	for (; slp; slp = slp->next) {
	   bsp = BioseqLockById(SeqLocId(slp));
	   ValNodeAddPointer(&sep, 1, bsp);
	   SeqMgrSeqEntry(OBJ_BIOSEQ, (Pointer)bsp, sep);
	   BioseqUnlock(bsp);
	}

	((BioseqSetPtr)blnet->bsp_set)->seq_set = sep;
	entityID = ObjMgrRegister(OBJ_SEQENTRY, (Pointer)sep);
	blnet->bsp = (BioseqPtr)sep->data.ptrvalue;
	blnet->parameters = BlastOptionsToParameters(options);
        if (options_allocated)
        {
                options = BLASTOptionDelete(options);
        }

	blnet->callback = callback;
	blnet->bl3hptr = bl3hp;

	seqalign = BlastBioseq(blnet, error_returns, ret_status, other_returns);

        /* BlastBioseq saves the MegaBLAST endpoint alignments in error_returns
           since there is nowhere else to save them */


	if (other_returns)
	{
		mask = BlastGetMaskedLoc(blnet);
		if (mask)
			ValNodeLink(other_returns, mask);
		dbinfo = BlastGetDbInfo(blnet);
		txdbinfo = NetDbinfo2TxDbinfo(dbinfo);
		ValNodeAddPointer (other_returns, TXDBINFO, txdbinfo);
		dbinfo = BlastDbinfoFree(dbinfo);
		params_buffer = BlastGetParameterBuffer(blnet);
		ValNodeAddPointer(other_returns, TXPARAMETERS, params_buffer);
		ka_blk = BlastGetKaParams(blnet, FALSE);
		if (ka_blk)
			ValNodeAddPointer (other_returns, TXKABLK_NOGAP, ka_blk);
		ka_blk = BlastGetKaParams(blnet, TRUE);
		if (ka_blk)
			ValNodeAddPointer (other_returns, TXKABLK_GAP, ka_blk);
		net_matrix = NetBlastGetMatrix(blnet);
		matrix = BlastNetMatrixToBlastMatrix(net_matrix);
		net_matrix = BlastMatrixFree(net_matrix);
		if (matrix)
		   ValNodeAddPointer (other_returns, TXMATRIX, matrix);
	}

	blnet = BlastNet3BlockDestruct(blnet);

	return seqalign;
}

NLM_EXTERN SeqAlignPtr LIBCALL
BlastBioseqNetCore(BlastNet3Hptr bl3hp, BioseqPtr bsp, CharPtr program, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, NetProgressCallback callback, BLAST_MatrixPtr blast_matrix, Boolean PNTR ret_status)

{
	BlastKABlkPtr ka_blk;
	BlastDbinfoPtr dbinfo;
	BlastMatrixPtr net_matrix;
	BLAST_MatrixPtr matrix;
	BlastNet3BlockPtr blnet;
	Boolean options_allocated = FALSE;
	CharPtr params_buffer=NULL;
	Int2 status;
	SeqAlignPtr seqalign = NULL;
	TxDfDbInfoPtr txdbinfo;
	ValNodePtr descr, mask;

	if (bl3hp == NULL || bsp == NULL || program == NULL || database == NULL)
		return NULL;

	if (error_returns)
		*error_returns = NULL;

	if (other_returns)
		*other_returns = NULL;

	/* If no options, use default. */
	if (options == NULL)
	{
		options = BLASTOptionNew(program, FALSE);
		options_allocated = TRUE;
	}

	status = BLASTOptionValidateEx(options, program, error_returns);
	if (status != 0)
	{	/* error messages in other_returns? */
		return NULL;
	}

	blnet = BlastNet3BlockNew(program, database);
	/* 
	Remove the Seq-descr as this is not needed for 
	BLASTing and the title often contains none-ASCII
	characters.  Keep the pointer and replace. 
	*/
	descr = bsp->descr;
	bsp->descr = NULL;

	blnet->bsp = bsp;
	blnet->parameters = BlastOptionsToParameters(options);
        if (options_allocated)
        {
                options = BLASTOptionDelete(options);
        }

	blnet->callback = callback;
	blnet->bl3hptr = bl3hp;
	blnet->blast_matrix = blast_matrix;

	seqalign = BlastBioseq(blnet, error_returns, ret_status, NULL);
	dbinfo = BlastGetDbInfo(blnet);
	net_matrix = NetBlastGetMatrix(blnet);
	params_buffer = BlastGetParameterBuffer(blnet);
	if (other_returns)
	{
		*other_returns = NULL;
		mask = BlastGetMaskedLoc(blnet);
		if (mask)
			ValNodeLink(other_returns, mask);
		txdbinfo = NetDbinfo2TxDbinfo(dbinfo);
		ValNodeAddPointer (other_returns, TXDBINFO, txdbinfo);
		ValNodeAddPointer(other_returns, TXPARAMETERS, params_buffer);
		params_buffer = NULL; /* Prevents freeing below. */
		ka_blk = BlastGetKaParams(blnet, FALSE);
		if (ka_blk)
			ValNodeAddPointer (other_returns, TXKABLK_NOGAP, ka_blk);
		ka_blk = BlastGetKaParams(blnet, TRUE);
		if (ka_blk)
			ValNodeAddPointer (other_returns, TXKABLK_GAP, ka_blk);
		matrix = BlastNetMatrixToBlastMatrix(net_matrix);
		if (matrix)
			ValNodeAddPointer (other_returns, TXMATRIX, matrix);
	}
	/* The next three lines prevent memory leaks when other_returns is NULL. */
	params_buffer = MemFree(params_buffer);
	dbinfo = BlastDbinfoFree(dbinfo);
	net_matrix = BlastMatrixFree(net_matrix);

	blnet = BlastNet3BlockDestruct(blnet);

	bsp->descr = descr;

	return seqalign;
}

NLM_EXTERN BlastPartsPtr LIBCALL
BlastBioseqNetReturnParts(BlastNet3Hptr bl3hp, BioseqPtr bsp, CharPtr program, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, NetProgressCallback callback, BLAST_MatrixPtr blast_matrix, Boolean PNTR ret_status)

{
	BlastKABlkPtr ka_blk;
	BlastDbinfoPtr dbinfo;
	BlastMatrixPtr net_matrix;
	BLAST_MatrixPtr matrix;
	BlastNet3BlockPtr blnet;
	BlastPartsPtr blast_parts;
	Boolean options_allocated = FALSE;
	CharPtr params_buffer;
	Int2 status;
	TxDfDbInfoPtr txdbinfo;
	ValNodePtr descr, mask;

	if (bl3hp == NULL || bsp == NULL || program == NULL || database == NULL)
		return NULL;

	if (error_returns)
		*error_returns = NULL;

	if (other_returns)
		*other_returns = NULL;

	/* If no options, use default. */
	if (options == NULL)
	{
		options = BLASTOptionNew(program, FALSE);
		options_allocated = TRUE;
	}

	status = BLASTOptionValidateEx(options, program, error_returns);
	if (status != 0)
	{	/* error messages in other_returns? */
		return NULL;
	}

	blnet = BlastNet3BlockNew(program, database);
	/* 
	Remove the Seq-descr as this is not needed for 
	BLASTing and the title often contains none-ASCII
	characters.  Keep the pointer and replace. 
	*/
	descr = bsp->descr;
	bsp->descr = NULL;

	blnet->bsp = bsp;
	blnet->parameters = BlastOptionsToParameters(options);
        if (options_allocated)
        {
                options = BLASTOptionDelete(options);
        }

	blnet->callback = callback;
	blnet->bl3hptr = bl3hp;
	blnet->blast_matrix = blast_matrix;

	blast_parts = BlastBioseqByParts(blnet, error_returns, ret_status);
	if (other_returns)
	{
		*other_returns = NULL;
		mask = BlastGetMaskedLoc(blnet);
		if (mask)
			ValNodeLink(other_returns, mask);
		dbinfo = BlastGetDbInfo(blnet);
		txdbinfo = NetDbinfo2TxDbinfo(dbinfo);
		ValNodeAddPointer (other_returns, TXDBINFO, txdbinfo);
		dbinfo = BlastDbinfoFree(dbinfo);
		params_buffer = BlastGetParameterBuffer(blnet);
		ValNodeAddPointer(other_returns, TXPARAMETERS, params_buffer);
		ka_blk = BlastGetKaParams(blnet, FALSE);
		if (ka_blk)
			ValNodeAddPointer (other_returns, TXKABLK_NOGAP, ka_blk);
		ka_blk = BlastGetKaParams(blnet, TRUE);
		if (ka_blk)
			ValNodeAddPointer (other_returns, TXKABLK_GAP, ka_blk);
		net_matrix = NetBlastGetMatrix(blnet);
		matrix = BlastNetMatrixToBlastMatrix(net_matrix);
		net_matrix = BlastMatrixFree(net_matrix);
		if (matrix)
			ValNodeAddPointer (other_returns, TXMATRIX, matrix);
	}

	blnet = BlastNet3BlockDestruct(blnet);

	bsp->descr = descr;

	return blast_parts;
}

NLM_EXTERN SeqAlignPtr LIBCALL
BlastSeqLocNetCore(BlastNet3Hptr bl3hp, SeqLocPtr slp, CharPtr program, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, NetProgressCallback callback, BLAST_MatrixPtr blast_matrix, Boolean PNTR status)

{
	SeqAlignPtr	seqalign = NULL;
	ValNodePtr	vnp;

	SeqPortPtr	spp;
	Bioseq		bs;
	BioseqPtr	bsp = &bs;
	Int4		the_len, res_index;
	Int2		residue;
	ByteStorePtr	bp;
	SeqLocPtr	whole_slp;
	ValNodePtr	mask_loc;

	/* get Bioseq by SeqLoc */


	the_len = SeqLocLen(slp);
	bp = BSNew((Uint4)the_len);

	if (ISA_na(SeqLocMol(slp)))
	    spp = SeqPortNewByLoc (slp, Seq_code_iupacna);
	else
	    spp = SeqPortNewByLoc (slp, Seq_code_iupacaa);

	MemSet((Pointer)bsp, 0, sizeof(Bioseq));
	bsp->length = (Int4)the_len;
	bsp->repr = Seq_repr_raw;
	bsp->mol = (Uint1) SeqLocMol(slp);
	bsp->id = SeqLocId(slp);

	if (ISA_na(bsp->mol))
	    bsp->seq_data_type = Seq_code_iupacna;
	else
	    bsp->seq_data_type = Seq_code_iupacaa;

	SeqPortSeek(spp, 0, SEEK_SET);
	BSSeek(bp, 0, SEEK_SET);
	for (res_index = 0; res_index < the_len; res_index++)
	{
	    residue = SeqPortGetResidue(spp);
	    BSPutByte(bp, residue);
	}
	bsp->seq_data = bp;

	SeqPortFree(spp);

	/* perform search */

	seqalign = BlastBioseqNetCore(bl3hp, bsp, program, database, options, 
		other_returns, error_returns, callback, NULL, status);
		
	/* offset the alignment */

	AdjustOffSetsInSeqAlign(seqalign, slp, NULL);
		
	if (other_returns) {
	    mask_loc = NULL;
	    for (vnp=*other_returns; vnp; vnp = vnp->next)
	    {
		switch (vnp->choice) {
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

	    /* adjust offset in mask_loc */

	    if (mask_loc && slp) {
		SeqLocPtr	maskslp;
		SeqIntPtr	masksip;
		Int4	offset;
		ValNodePtr	vnp;

		whole_slp = NULL;

		ValNodeAddPointer(&whole_slp, SEQLOC_WHOLE, SeqIdDup(SeqIdFindBest(bsp->id, SEQID_GI)));

		offset = GetOffsetInLoc(slp, whole_slp, SEQLOC_START);

		for (vnp = mask_loc; vnp; vnp = vnp->next) {

		    for (maskslp = (SeqLocPtr) vnp->data.ptrvalue; maskslp; maskslp = maskslp->next) {
			masksip = (SeqIntPtr) maskslp->data.ptrvalue;

			masksip->from += offset;
			masksip->to += offset;
		    }
		}
	    }
	}

	return seqalign;
}

#if 0
FILE *global_fp;
#endif

static  Boolean LIBCALLBACK
callback (BlastResponsePtr brp, Boolean PNTR cancel)

{

#if 0
        fprintf(global_fp, ".");
        fflush(global_fp);
#endif
	return TRUE;

}

static Boolean
TraditionalBlastReportEngine(SeqLocPtr slp, BioseqPtr bsp, BLAST_OptionsBlkPtr options, BlastNet3Hptr bl3hp, CharPtr program, CharPtr database, Boolean html, FILE *outfp, Boolean verbose, Uint4 print_options, Uint4 align_options, Int4 number_of_descriptions, Int4 number_of_alignments, Int4Ptr number_of_hits, Uint4 overview)

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
    ValNodePtr mask_loc, mask_loc_start, other_returns, error_returns, vnp, vnp1=NULL;
    Uint1 align_type;
    Uint1 f_order[FEATDEF_ANY], g_order[FEATDEF_ANY];

    SeqLocPtr tmp_slp;
    DenseSegPtr dsp, next_dsp;
    MegaBlastResultsPtr mb_results = NULL;
    
    MemSet((Pointer)(g_order), 0, (size_t)(FEATDEF_ANY* sizeof(Uint1)));
    MemSet((Pointer)(f_order), 0, (size_t)(FEATDEF_ANY* sizeof(Uint1)));
    
    if (bsp == NULL && slp == NULL)
        return FALSE;
    
    if (bl3hp == NULL || program == NULL || database == NULL || outfp == NULL)
        return FALSE;
    
    align_type = BlastGetTypes(program, &query_is_na, &db_is_na);
#if 0
    global_fp = outfp;
#endif
    
    init_buff_ex(85);
    dbinfo = BlastRequestDbInfo(bl3hp, database, !db_is_na);
    if (dbinfo)
        PrintDbInformationBasic(database, !db_is_na, 70, dbinfo->definition, dbinfo->number_seqs, dbinfo->total_length, outfp, html);
    dbinfo = BlastDbinfoFree(dbinfo);
    free_buff();
    
    fprintf(outfp, "Searching");
    
    if (bsp) {
        seqalign = BlastBioseqNetCore(bl3hp, bsp, program, database, options,
				      &other_returns, &error_returns, callback,
				      NULL, &status);
    } else if (options->is_megablast_search)
        seqalign = MegaBlastSeqLocNetCore(bl3hp, slp, program, database, options, &other_returns, &error_returns, callback, &status);
    else
        seqalign = BlastSeqLocNetCore(bl3hp, slp, program, database, options, &other_returns, &error_returns, callback, NULL, &status);
    
    fprintf(outfp, " done\n");
    
    if (overview) {
        fprintf(outfp, "\n\n");
        if (bsp)
            length= bsp->length;
        else
            length= SeqLocLen(slp);
        
        VSMakeSeqLoc(&seqalign, &vnp1, length, NULL);
        VSPrintOverviewFromSeqLocs(vnp1, length, outfp);
    }
    
    BlastErrorPrintExtra(error_returns, TRUE, outfp);
    
    mask_loc = NULL;
    matrix = NULL;
    txmatrix = NULL;
    for (vnp=other_returns; vnp; vnp = vnp->next) {
        switch (vnp->choice) {
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
            /* This function should not use matrix ??? */
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
	   if (!options->is_megablast_search)
	      ValNodeAddPointer(&mask_loc, vnp->choice, vnp->data.ptrvalue);
            break;
        case BlastResponse_mbalign:
           mb_results = (MegaBlastResultsPtr) vnp->data.ptrvalue;
           break;
        default:
            break;
        }
    }	
    
    NlmMutexLockEx(&formating_mutex);
    
    ReadDBBioseqFetchEnable ("blastcl3", database, db_is_na, TRUE);

    if (mb_results) {
       /* Results come as alignment endpoints only from Mega BLAST */
       MegaBlastHitPtr mb_hit = mb_results->mbhits /*, next_hit */ ;


       while (mb_hit) {
          fprintf(outfp, "%s\t%s\t%d\t%d\t%d\t%d\t%d\n", mb_hit->id1,
                  mb_hit->id2, mb_hit->query_offset, mb_hit->subject_offset,
                  mb_hit->query_end, mb_hit->subject_end, mb_hit->score);
          mb_hit = mb_hit->next;
       }
       MegaBlastResultsFree(mb_results);
    }


    tmp_slp = slp;

    while (seqalign) {
       if (!options->is_megablast_search)
          next_seqalign = NULL;
       else {
          sap = seqalign;
          while (sap != NULL) { 
             if (sap->next != NULL) {
                dsp = (DenseSegPtr) (sap->segs);
                next_dsp = (DenseSegPtr) (sap->next->segs);
                
                if (SeqIdComp(dsp->ids, next_dsp->ids) != SIC_YES) {
                   next_seqalign = sap->next;
                   sap->next = NULL;
                }
             } else
                next_seqalign = NULL;
             sap = sap->next;
          }

          dsp = (DenseSegPtr) (seqalign->segs);
          while (tmp_slp && SeqIdComp(dsp->ids, SeqLocId(tmp_slp)) != SIC_YES)
             tmp_slp = tmp_slp->next;
          if (tmp_slp == NULL) /* Should never happen */
             break;
          bsp = BioseqLockById(SeqLocId(tmp_slp));
          init_buff_ex(85);
          AcknowledgeBlastQuery(bsp, 70, outfp, FALSE, html);
          free_buff();
          BioseqUnlock(bsp);
       }

       seqannot = SeqAnnotNew();
       seqannot->type = 2;
       AddAlignInfoToSeqAnnot(seqannot, align_type);
       seqannot->data = seqalign;
       prune = BlastPruneHitsFromSeqAlign(seqalign, number_of_descriptions, NULL);
       ObjMgrSetHold();
       init_buff_ex(85);
       PrintDefLinesFromSeqAlign(prune->sap, 80, outfp, print_options, FIRST_PASS, NULL);
       free_buff();
       
       prune = BlastPruneHitsFromSeqAlign(seqalign, number_of_alignments, prune);
       seqannot->data = prune->sap;
       if (align_options & TXALIGN_MASTER)
          ShowTextAlignFromAnnot(seqannot, 60, outfp, f_order, g_order, align_options, txmatrix, mask_loc, NULL);
       else
          ShowTextAlignFromAnnot(seqannot, 60, outfp, f_order, g_order, align_options, txmatrix, mask_loc, FormatScoreFunc);
       seqannot->data = seqalign;
       number_of_hits_private = prune->original_number; 
       prune = BlastPruneSapStructDestruct(prune);
       ObjMgrClearHold();
       ObjMgrFreeCache(0);

       if (options->is_megablast_search)
          tmp_slp = tmp_slp->next;

       if (seqannot)
          seqannot = SeqAnnotFree(seqannot);

       seqalign = next_seqalign;
    }
    mask_loc_start = mask_loc;
    while (mask_loc) {
        SeqLocSetFree((SeqLocPtr) mask_loc->data.ptrvalue);
        mask_loc = mask_loc->next;
    }
    ValNodeFree(mask_loc_start);
    
    BLAST_MatrixDestruct(matrix);
    if (txmatrix)
       txmatrix = TxMatrixDestruct(txmatrix);

    if (verbose) {
       init_buff_ex(85);
       tx_dbinfo_head = tx_dbinfo;
       while (tx_dbinfo) {
	  PrintDbReport(tx_dbinfo, 70, outfp);
	  tx_dbinfo = tx_dbinfo->next;
       }
       tx_dbinfo_head = TxDfDbInfoDestruct(tx_dbinfo_head);
       
       if (ka_params) {
	  PrintKAParameters(ka_params->lambda, ka_params->k, ka_params->h, 70, outfp, FALSE);
	  MemFree(ka_params);
       }
       
       if (ka_params_gap) {
	  PrintKAParameters(ka_params_gap->lambda, ka_params_gap->k, ka_params_gap->h, 70, outfp, TRUE);
	  MemFree(ka_params_gap);
       }
       PrintTildeSepLines(params_buffer, 70, outfp);
       MemFree(params_buffer);
       free_buff();
    }

    ReadDBBioseqFetchDisable();
    other_returns = ValNodeFree(other_returns);
    
    NlmMutexUnlock(formating_mutex);
    
    if (number_of_hits)
       *number_of_hits = number_of_hits_private;
    return status;
}
/*
  Formats a 'traditional' BLAST report.
*/

NLM_EXTERN Boolean LIBCALL
TraditionalBlastReport(BioseqPtr bsp, BLAST_OptionsBlkPtr options, BlastNet3Hptr bl3hp, CharPtr program, CharPtr database, Boolean html, FILE *outfp, Boolean verbose, Uint4 print_options, Uint4 align_options, Int4 number_of_descriptions, Int4 number_of_alignments, Int4Ptr number_of_hits)

{
	return TraditionalBlastReportEngine(NULL, bsp, options, bl3hp, program, database, html, outfp, verbose, print_options, align_options, number_of_descriptions, number_of_alignments, number_of_hits, BLAST_OVERVIEW_NONE);
}

NLM_EXTERN Boolean LIBCALL
TraditionalBlastReportExtra(BioseqPtr bsp, BLAST_OptionsBlkPtr options, BlastNet3Hptr bl3hp, CharPtr program, CharPtr database, Boolean html, FILE *outfp, Boolean verbose, Uint4 print_options, Uint4 align_options, Int4 number_of_descriptions, Int4 number_of_alignments, Int4Ptr number_of_hits, Uint4 overview)

{
	return TraditionalBlastReportEngine(NULL, bsp, options, bl3hp, program, database, html, outfp, verbose, print_options, align_options, number_of_descriptions, number_of_alignments, number_of_hits, overview);
}


NLM_EXTERN Boolean LIBCALL
TraditionalBlastReportLoc(SeqLocPtr slp, BLAST_OptionsBlkPtr options, BlastNet3Hptr bl3hp, CharPtr program, CharPtr database, Boolean html, FILE *outfp, Boolean verbose, Uint4 print_options, Uint4 align_options, Int4 number_of_descriptions, Int4 number_of_alignments, Int4Ptr number_of_hits)

{
	return TraditionalBlastReportEngine(slp, NULL, options, bl3hp, program, database, html, outfp, verbose, print_options, align_options, number_of_descriptions, number_of_alignments, number_of_hits, BLAST_OVERVIEW_NONE);
}

NLM_EXTERN Boolean LIBCALL
TraditionalBlastReportLocExtra(SeqLocPtr slp, BLAST_OptionsBlkPtr options, BlastNet3Hptr bl3hp, CharPtr program, CharPtr database, Boolean html, FILE *outfp, Boolean verbose, Uint4 print_options, Uint4 align_options, Int4 number_of_descriptions, Int4 number_of_alignments, Int4Ptr number_of_hits, Uint4 overview)

{
	return TraditionalBlastReportEngine(slp, NULL, options, bl3hp, program, database, html, outfp, verbose, print_options, align_options, number_of_descriptions, number_of_alignments, number_of_hits, overview);
}


/*
        Converst the BlastParametersPtr (used by network service) to
        BLAST_OptionsBlkPtr (used by blast).
*/

NLM_EXTERN BLAST_OptionsBlkPtr
parametersToOptions (BlastParametersPtr parameters, CharPtr program, ValNodePtr PNTR error_return)

{
        BLAST_OptionsBlkPtr options;
        Int2 status;

        if (program == NULL)
                return NULL;

        if (parameters == NULL) {
            options = BLASTOptionNew(program, TRUE);
        } else {
                options = BLASTOptionNew(program, (Boolean) parameters->gapped_alignment);
                options->threshold_second = parameters->second_threshold;
                if (parameters->Cutoff_cutoff)
                {
                        if (parameters->Cutoff_cutoff->choice == Cutoff_cutoff_evalue)
                                options->expect_value = parameters->Cutoff_cutoff->data.realvalue;
                        else if (parameters->Cutoff_cutoff->choice == Cutoff_cutoff_score)
                                options->cutoff_s = parameters->Cutoff_cutoff->data.intvalue;
                }
                if (parameters->Cutoff2_cutoff2)
                {
                        if (parameters->Cutoff2_cutoff2->choice == Cutoff2_cutoff2_evalue)
                                options->e2 = parameters->Cutoff2_cutoff2->data.realvalue;
                        else if (parameters->Cutoff2_cutoff2->choice == Cutoff2_cutoff2_score)
                                options->cutoff_s2 = parameters->Cutoff2_cutoff2->data.intvalue;
                }
                /* compensates for client not providing this.  Remove this at some point? */
                if (parameters->hitlist_size != 0)
                        options->hitlist_size = parameters->hitlist_size;
                options->penalty = parameters->nucl_penalty;
                options->reward = parameters->nucl_reward;
                options->gap_open = parameters->gap_open;
                options->gap_extend = parameters->gap_extend;
                /* compensates for client not providing this.  Remove this at some point? */
                if (parameters->genetic_code != 0)
                        options->genetic_code = parameters->genetic_code;
                /* compensates for client not providing this.  Remove this at some point? */
                if (parameters->db_genetic_code != 0)
                        options->db_genetic_code = parameters->db_genetic_code;

                options->filter = parameters->low_complexity_filtering;
                options->ethresh = parameters->ethresh;
                options->maxNumPasses = parameters->max_num_passes;
                options->pseudoCountConst = parameters->pseudo_count_const;
                
		if (parameters->gifile)
		{
                	options->gifile = StringSave(parameters->gifile);
		}
                options->gilist = parameters->gilist;
		if (parameters->matrix)
		{
			options->matrix = (CharPtr) MemFree(options->matrix);
                	options->matrix = StringSave(parameters->matrix);
		}
		if (parameters->filter_string)
		{
               		options->filter_string = (CharPtr) MemFree(options->filter_string);
               		options->filter_string = StringSave(parameters->filter_string);
		}
                if (parameters->entrez_query)
                    options->entrez_query = StringSave(parameters->entrez_query);
                /* compensates for client not providing this.  Remove this at some point? */
		if (parameters->word_size)
                    options->wordsize = parameters->word_size;
		if (parameters->db_length)
                    options->db_length = parameters->db_length;
		if (parameters->searchsp_eff)
                    options->searchsp_eff = parameters->searchsp_eff;
		if (parameters->hsp_range_max)
                    options->hsp_range_max = parameters->hsp_range_max;
		if (parameters->block_width)
                    options->block_width = parameters->block_width;
		if (parameters->perform_culling)
                    options->perform_culling = parameters->perform_culling;
		if (parameters->strand_option)
                    options->strand_option = parameters->strand_option;

                if(parameters->phi_pattern) {
                    options->phi_pattern = StringSave(parameters->phi_pattern);
                    options->isPatternSearch = TRUE;
                }
        	options->use_real_db_size = parameters->use_real_db_size;
#if 0
                if(parameters->db_dir_prefix) {
                    options->db_dir_prefix = StringSave(parameters->db_dir_prefix);
		}
#endif
        	options->use_best_align = parameters->use_best_align;

                if(parameters->required_start != 0)
                    options->required_start = parameters->required_start;
                if(parameters->required_end != 0)
                    options->required_end = parameters->required_end;
                
        	options->is_rps_blast = parameters->is_rps_blast;
                options->tweak_parameters = parameters->tweak_parameters;
                options->smith_waterman = parameters->smith_waterman;
		options->is_megablast_search = parameters->is_megablast;
                options->query_lcase_mask = 
                   (SeqLocPtr) parameters->query_lcase_mask;
                options->no_traceback = parameters->endpoint_results;
                options->perc_identity = (FloatLo) parameters->percent_identity;
        }

	if ((status = BLASTOptionValidateEx(options, program, error_return))) {
            return NULL;
	}

        return options;
}
/* This function is interface to the Entrez2 engine. It may be used
   to get list of gis corresponding to the Entrez Boolean string or
   just number of such hits in the Entrez database */

NLM_EXTERN Int4 BLASTGetUidsFromQuery(CharPtr query, Int4Ptr PNTR uids, 
                                      Boolean is_na, Boolean count_only)
{
    Entrez2ReplyPtr e2ry;
    Entrez2RequestPtr  e2rq;
    E2ReplyPtr e2rp;
    Int4 count = 0;
    Entrez2BooleanReplyPtr e2br;
    Entrez2IdListPtr e2idlist;
    
    if(uids != NULL)
        *uids = NULL;
    
    EntrezSetProgramName ("BLAST API");
    EntrezSetServer ("www.ncbi.nlm.nih.gov", 80, 
                     "/entrez/utils/entrez2server.fcgi");
    
    e2rq = EntrezCreateBooleanRequest (!count_only, FALSE, 
                                       is_na? "Nucleotide" : "Protein", 
                                       query, 0, 0, NULL, 0, 0);
    
    e2ry = EntrezSynchronousQuery (e2rq);
    
    if (e2ry == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, 
                  "NULL returned from EntrezSynchronousQuery()");
        return -1;
    }

    if((e2rp = e2ry->reply) == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid ASN.1: E2ReplyPtr==NULL");
        return -1;
    }
    
    switch(e2rp->choice) {
        
    case E2Reply_error:
        ErrPostEx(SEV_ERROR, 0, 0, (CharPtr) e2rp->data.ptrvalue);
        count = -1;
        break;
    case E2Reply_eval_boolean:
        e2br = (Entrez2BooleanReplyPtr) e2rp->data.ptrvalue;
        count = e2br->count;
        if((e2idlist = e2br->uids) != NULL) {
            count = e2idlist->num;
            if(uids != NULL) {
                *uids = (Int4Ptr) MemNew(sizeof(Int4)*count);
                BSSeek((ByteStorePtr) e2idlist->uids, 0, SEEK_SET);
                BSRead((ByteStorePtr) e2idlist->uids, *uids, sizeof(Int4)*count);
            }
        }
        break;
    default:
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid reply type from the server: %d", e2rp->choice);
        count = -1;
        break;
        
    }
    
    Entrez2ReplyFree(e2ry);
    Entrez2RequestFree(e2rq);

    return count;
}
