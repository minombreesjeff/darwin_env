/*****************************************************************************
*
*   asn2asn.c
*   	This is a framework for reading an ASN.1 Seq-entry or Bioseq-set,
*       doing some processing on it, and outputting it again. seqport.h
*       is included since it covers most necessary utilities that you might
*       need. You may need to add others for specialized reports and so on.
*
*       The check for aipout == NULL is to show how to change the code if
*         no output is desired. Change the default in myargs from "stdout" to
*         NULL to make output command line optional.
*
*       This program can be used "as is" to convert between binary and text
*        ASN.1 through the object loaders.
*
*****************************************************************************/
#include <seqport.h>

#define NUMARG 7
Args myargs[NUMARG] = {
	{"Filename for asn.1 input","stdin",NULL,NULL,FALSE,'i',ARG_FILE_IN,0.0,0,NULL},
	{"Input is a Seq-entry","F", NULL ,NULL ,TRUE,'e',ARG_BOOLEAN,0.0,0,NULL},
	{"Input asnfile in binary mode","F",NULL,NULL,TRUE,'b',ARG_BOOLEAN,0.0,0,NULL},
	{"Filename for asn.1 output","stdout", NULL,NULL,TRUE,'o',ARG_FILE_OUT,0.0,0,NULL},
	{"Output asnfile in binary mode?","F", NULL ,NULL ,TRUE,'s',ARG_BOOLEAN,0.0,0,NULL},
    {"Log errors to file named:",NULL,NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL},
	{"Output asnfile in XML?","F", NULL ,NULL ,TRUE,'x',ARG_BOOLEAN,0.0,0,NULL}};


/*****************************************************************************
*
*   Put any internal prototypes here
*   NB: the function must match the prototype exactly, including "static" or
*       not.
*
*****************************************************************************/
static void SeqEntryProcess PROTO((SeqEntryPtr sep));  /* dummy function */


/*****************************************************************************
*
*   Main program loop to read, process, write SeqEntrys
*
*****************************************************************************/
Int2 Main(void)
{
	AsnIoPtr aipout=NULL, aipin;
	SeqEntryPtr sep;
	AsnTypePtr atp, atp2;
	AsnModulePtr amp;
	DataVal dv;
	CharPtr ftype;

					/* check command line arguments */

	if ( ! GetArgs("asn2asn",NUMARG, myargs))
		return 1;

					/* load the sequence alphabets  */
					/* (and sequence parse trees)   */
	if (! SeqEntryLoad())
		ErrShow();
				    /* get pointer to all loaded ASN.1 modules */
	amp = AsnAllModPtr();
	if (amp == NULL)
	{
		ErrShow();
		return 1;
	}

	atp = AsnFind("Bioseq-set");    /* get the initial type pointers */
	if (atp == NULL)
	{
		ErrShow();
		return 1;
	}
	atp2 = AsnFind("Bioseq-set.seq-set.E");
	if (atp2 == NULL)
	{
		ErrShow();
		return 1;
	}

					/* open the ASN.1 input file in the right mode */

	if ((aipin = AsnIoOpen (myargs[0].strvalue, myargs[2].intvalue?"rb":"r"))
          == NULL)
	{
		ErrPostEx(SEV_ERROR,0,0, "Can't open %s", myargs[0].strvalue);
		ErrShow();
		return 1;
	}

					/* open the ASN.1 output file in the right mode */

	if (myargs[3].strvalue != NULL)   /* output desired? */
	{
		ftype = "w";
		if (myargs[4].intvalue)
			ftype = "wb";
		if (myargs[6].intvalue)
			ftype = "wx";

		if ((aipout = AsnIoOpen (myargs[3].strvalue, ftype)) == NULL)
		{
			ErrPostEx(SEV_ERROR,0,0, "Can't open %s", myargs[3].strvalue);
			ErrShow();
			return 1;
		}
	}

                                /* log errors instead of die */
    if (myargs[5].strvalue != NULL)
    {
        if (! ErrSetLog (myargs[5].strvalue))
		{
            ErrShow();
			return 1;
		}
        else
            ErrSetOpts (ERR_CONTINUE, ERR_LOG_ON);
    }

	if ( myargs[1].intvalue)   /* read one Seq-entry */
	{
		sep = SeqEntryAsnRead(aipin, NULL);
		SeqEntryProcess(sep);     /* do any processing */
		if (aipout != NULL)
			SeqEntryAsnWrite(sep, aipout, NULL);  
		SeqEntryFree(sep);
	}
	else                      /* read Seq-entry's from a Bioseq-set */
	{
		while ((atp = AsnReadId(aipin, amp, atp)) != NULL)
		{
			if (atp == atp2)    /* top level Seq-entry */
			{
				sep = SeqEntryAsnRead(aipin, atp);
				SeqEntryProcess(sep);     /* do any processing */
				if (aipout != NULL)
					SeqEntryAsnWrite(sep, aipout, atp);  
				SeqEntryFree(sep);
			}
			else
			{
				if (aipout == NULL)    /* don't need to read the data */
					AsnReadVal(aipin, atp, NULL);
				else
				{
			 		AsnReadVal(aipin, atp, &dv); /* read it */
					AsnWrite(aipout, atp, &dv);	 /* write it */
					AsnKillValue(atp, &dv);      /* free it */
				}
			}
		}
	}

	AsnIoClose(aipin);
	AsnIoClose(aipout);

	return(0);
}


/*****************************************************************************
*
*   void SeqEntryProcess (sep)
*      just a dummy routine that does nothing
*
*****************************************************************************/
static void SeqEntryProcess (SeqEntryPtr sep)
{
	if (sep == NULL)
		return;
	return;
}

