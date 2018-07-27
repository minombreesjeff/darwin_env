 /*****************************************************************************
*
*   packfilt.c
*   	Packs input stream if Bioseq-set of SeqEntry
*
*****************************************************************************/
#include <sequtil.h>
#include <objall.h>

Args myargs[] = {
	{"Filename for asn.1 input","stdin",NULL,NULL,TRUE,'a',ARG_FILE_IN,0.0,0,NULL},
	{"Input is a Seq-entry","F", NULL ,NULL ,TRUE,'e',ARG_BOOLEAN,0.0,0,NULL},
	{"Input asnfile in binary mode","F",NULL,NULL,TRUE,'b',ARG_BOOLEAN,0.0,0,NULL},
	{"Output Filename","stdout", NULL,NULL,TRUE,'o',ARG_FILE_OUT,0.0,0,NULL},
	{"Output asnfile in binary mode","F",NULL,NULL,TRUE,'p',ARG_BOOLEAN,0.0,0,NULL},
	{"Output error logfile","stderr", NULL,NULL,TRUE,'l',ARG_FILE_OUT,0.0,0,NULL},
	};


Int2 Main(void)
{

	AsnIoPtr aip, aipout;
	AsnTypePtr atp = NULL, atp1, atp2;
	AsnModulePtr amp;
	Boolean error_msgs=TRUE;
	SeqEntryPtr the_set;
 Int4 num = 0;
	DataVal av;
	
	if ( ! GetArgs("packfilt", sizeof(myargs)/sizeof(Args), myargs))
		return 1;
	ErrSetMessageLevel(SEV_NONE);
	ErrSetOptFlags(EO_SHOW_CODES);

	if (! SeqEntryLoad())
		ErrShow();
	
		atp = AsnFind("Bioseq-set"); /* get the initial type pointers */
		if (atp == NULL)
			ErrShow();
	
		atp2 = AsnFind("Bioseq-set.seq-set.E");
		if (atp2 == NULL)
			ErrShow();

			/* open the i/o files in the right mode */

	if ((aip = 
		AsnIoOpen (myargs[0].strvalue, myargs[2].intvalue?"rb":"r")) == NULL){
		ErrPostEx(SEV_ERROR,0,0, "Can't read %s", myargs[0].strvalue);
		exit (1);
 }

	if ((aipout = 
		AsnIoOpen (myargs[3].strvalue, myargs[4].intvalue?"wb":"r")) == NULL){
		ErrPostEx(SEV_ERROR,0,0, "Can't write %s", myargs[3].strvalue);
		exit (1);
 }


    if (myargs[5].strvalue != NULL) { 			/* log errors instead of die */
        if (! ErrSetLog (myargs[5].strvalue))
            ErrShow();
        else
            ErrSetOpts (ERR_TEE, ERR_LOG_ON);
    }

	amp = AsnAllModPtr();
	if (amp == NULL)
		ErrShow();

		if (myargs[1].intvalue) {
			the_set = SeqEntryAsnRead(aip, NULL);
			SeqEntryPack(the_set);
       num ++;
       SeqEntryAsnWrite (the_set, aipout, NULL);
		   SeqEntryFree(the_set);
	} else {
		while ((atp = AsnReadId(aip, amp, atp)) != NULL) {
			if (atp == atp2) {  /* top level Seq-entry */
				the_set = SeqEntryAsnRead(aip, atp);
			   SeqEntryPack(the_set);
          num ++;
          SeqEntryAsnWrite (the_set, aipout, atp);
				SeqEntryFree(the_set);
			} else {
				AsnReadVal(aip, atp, &av);
				AsnWrite(aipout, atp, &av);
				AsnKillValue(atp, &av);
			}
		}
	}

	ErrPostEx(SEV_INFO, 1, 1, "[%ld] entries have been packed from %s.\n", num,myargs[0].strvalue);

	AsnIoClose(aip);
	AsnIoClose(aipout);
	return(0);
}
