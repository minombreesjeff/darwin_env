/*****************************************************************************
*
*   testval.c
*   	check translations
*       check for stop codons
*       Check for and fix non 3.0 asn spec things
*
*****************************************************************************/
#include <objsub.h>
#include <valid.h>

#define NUMARG 15
Args myargs[NUMARG] = {
	{"Filename for asn.1 input","stdin",NULL,NULL,TRUE,'i',ARG_FILE_IN,0.0,0,NULL},
	{"Input is a Seq-entry","F", NULL ,NULL ,TRUE,'e',ARG_BOOLEAN,0.0,0,NULL},
	{"Input is a Seq-submit","F", NULL ,NULL ,TRUE,'s',ARG_BOOLEAN,0.0,0,NULL},
	{"Input asnfile in binary mode","F",NULL,NULL,TRUE,'b',ARG_BOOLEAN,0.0,0,NULL},
	{"Filename for error messages","stderr",NULL,NULL,TRUE,'x',ARG_FILE_OUT,0.0,0,NULL} ,
	{"Filename for asn.1 output",NULL,NULL,NULL,TRUE,'o',ARG_FILE_OUT,0.0,0,NULL},
	{"Output asnfile in binary mode","F",NULL,NULL,TRUE,'t',ARG_BOOLEAN,0.0,0,NULL},
	{"Severity of error for count in return code","4","0","4",FALSE,'r',ARG_INT,0.0,0,NULL},
	{"Continue on ASN.1 error?","F",NULL,NULL,TRUE,'c',ARG_BOOLEAN,0.0,0,NULL},
	{"Patch invalid sequence residues?","F",NULL,NULL,TRUE,'p',ARG_BOOLEAN,0.0,0,NULL},
	{"Lowest severity of error to show","3","0","4",FALSE,'q',ARG_INT,0.0,0,NULL},
	{"ASN.1 spec level to filter","0","0","3",FALSE,'l',ARG_INT,0.0,0,NULL},
	{"Use SeqMgr indexing?","T",NULL,NULL,TRUE,'d',ARG_BOOLEAN,0.0,0,NULL},
	{"Validate alignments?","F",NULL,NULL,TRUE,'a',ARG_BOOLEAN,0.0,0,NULL},
	{"Require ISO-JTA?","F",NULL,NULL,TRUE,'j',ARG_BOOLEAN,0.0,0,NULL}};

CharPtr AsnIoGets PROTO((AsnIoPtr aip));  /* from asnio.h */
void LIBCALLBACK error_ret PROTO((Int2 value, CharPtr msg));


Int2 Main(void)
{
	AsnIoPtr aip, aipout=NULL;
	SeqEntryPtr sep;
	AsnTypePtr atp, atp2;
	AsnModulePtr amp;
	ValidStructPtr vsp;
	Int2 numerrors, found_one, fatal_error = 0, error_level, i, spec_version;
	CharPtr tmp;
	Boolean found;
	DataVal av;

					/* check command line arguments */
	if ( ! GetArgs("testval",NUMARG, myargs))
		return 1;

					/* load the sequence alphabets  */
					/* (and sequence parse trees)   */
	if (! SeqEntryLoad())
		ErrShow();
				/* Don't let the program die, no matter what */
	ErrSetFatalLevel(SEV_MAX);
	ErrSetMessageLevel(SEV_MAX);     /* don't report errors via Message()*/
	ErrSetOptFlags (EO_MSG_CODES);
	ErrSetLogfile(myargs[4].strvalue, 0);      /* report errors by printing to stderr */

	ErrSetOpts (ERR_IGNORE, ERR_LOG_ON);

	
				    /* get pointer to all loaded ASN.1 modules */
	amp = AsnAllModPtr();
	if (amp == NULL)
		ErrShow();

	if (myargs[2].intvalue)  /* Seq-submit */
	{
	    if (! SubmitAsnLoad())
    	    ErrPostEx(SEV_FATAL, 1,0, "Unable to SeqSubmitLoad.");

		atp = AsnFind("Seq-submit");    /* get the initial type pointers */
		if (atp == NULL)
			ErrShow();
		atp2 = AsnFind("Seq-submit.data.entrys.E");
		if (atp2 == NULL)
			ErrShow();
	}
	else if (! myargs[1].intvalue)   /* not Seq-entry */
	{
		atp = AsnFind("Bioseq-set");    /* get the initial type pointers */
		if (atp == NULL)
			ErrShow();
		atp2 = AsnFind("Bioseq-set.seq-set.E");
		if (atp2 == NULL)
			ErrShow();
	}

	error_level = (Int2)myargs[7].intvalue;
	spec_version = (Int2)myargs[11].intvalue;

					/* open the ASN.1 input file in the right mode */
	if ((aip = AsnIoOpen (myargs[0].strvalue, myargs[3].intvalue?"rb":"r"))
          == NULL)
	{
		ErrShow();
		return 1;
	}
	aip->spec_version = spec_version;


					/* open the ASN.1 output file in the right mode */
	if (myargs[5].strvalue != NULL)
	{
		if ((aipout = AsnIoOpen (myargs[5].strvalue, myargs[6].intvalue?"wb":"w"))
    	      == NULL)
		{
			ErrShow();
			return 1;
		}
	}

	if ((! myargs[3].intvalue) && (myargs[2].intvalue))
	{
									/* scan past any headers */
		found = FALSE;
		while (! found)
		{
			tmp = AsnIoGets(aip);
			if (tmp == NULL) {
			  printf("Unable to read file\n");
			  return 0;
			}
			*(aip->buf + aip->offset -1) = '\0';
			if (strstr(tmp, "Seq-submit") != NULL)
			{
				found = TRUE;
			}
			*(aip->buf + aip->offset - 1) = '\n';
		}

	}

	vsp = ValidStructNew();

	vsp->cutoff = (Int2)(myargs[10].intvalue);
	vsp->useSeqMgrIndexes = (Boolean)(myargs[12].intvalue); /* indexed validate */
	vsp->validateAlignments = (Boolean)(myargs[13].intvalue);
	vsp->farIDsInAlignments = (Boolean)(myargs[13].intvalue);
	vsp->alwaysRequireIsoJTA = (Boolean)(myargs[14].intvalue);

	if (myargs[8].intvalue)   /* continue on ASN.1 error */
		AsnIoSetErrorMsg(aip, error_ret);

	if (myargs[9].intvalue)   /* patch bad sequence residues */
		vsp->patch_seq = TRUE;
		
	numerrors = 0;
	found_one = FALSE;
	if ( myargs[1].intvalue)   /* read one Seq-entry */
	{
		sep = SeqEntryAsnRead(aip, NULL);

		if (sep == NULL)
		{
			fatal_error++;
			numerrors++;
			ErrPostEx(SEV_ERROR,0,0, "Couldn't load [%s]\n", myargs[0].strvalue);
		}
		else
		{
			found_one = TRUE;
			if (aip->io_failure)
			{
				vsp->non_ascii_chars = TRUE;
				aip->io_failure = FALSE;
			}
			ValidateSeqEntry(sep, vsp);
			for (i = 0; i <= 4 ; i++)
			{
				numerrors += vsp->errors[i];
				if (i >= error_level)
					fatal_error += vsp->errors[i];
			}
			if (aipout != NULL)
				SeqEntryAsnWrite(sep, aipout, NULL);
			SeqEntryFree(sep);
		}
	}
	else                      /* read Seq-entry's from a Bioseq-set */
	{
		while ((atp = AsnReadId(aip, amp, atp)) != NULL)
		{
			found_one = TRUE;
			if (atp == atp2)    /* top level Seq-entry */
			{
				sep = SeqEntryAsnRead(aip, atp);
				if (sep == NULL)
				{
					numerrors++;
					fatal_error++;
				}
				if (aip->io_failure)
				{
					vsp->non_ascii_chars = TRUE;
					aip->io_failure = FALSE;
				}
				ValidateSeqEntry(sep, vsp);
				if (aipout != NULL)
					SeqEntryAsnWrite(sep, aipout, atp);
				SeqEntryFree(sep);
				for (i = 0; i <=4; i++)
				{
					numerrors += vsp->errors[i];
					if (i >= error_level)
						fatal_error += vsp->errors[i];
				}
				ValidStructClear(vsp);
			}
			else
			{
				AsnReadVal(aip, atp, &av);
				if (aipout != NULL)
					AsnWrite(aipout, atp, &av);
				AsnKillValue(atp, &av);
			}
		}
	}

	AsnIoClose(aip);
	AsnIoClose(aipout);

	ValidStructFree(vsp);

	if (myargs[5].strvalue == NULL)   /* no output file */
	{
		if (! numerrors)
		{
			if (! found_one) {
				printf("Unable to read file\n");
			} else {
				printf("All entries are OK!\n");
			}
		}
		else
			printf("%d messges reported\n", (int)numerrors);
	}
	
    
	return( fatal_error );
}

void LIBCALLBACK error_ret (Int2 level, CharPtr msg)
{
	fprintf(stderr, "%s\n\n", msg);
	return;
}
