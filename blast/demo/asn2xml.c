/*****************************************************************************
*
* $Id: asn2xml.c,v 1.2 2001/08/10 18:56:42 kans Exp $
*
*   asn2xml.c
*
*      This program is based on asn2asn.c but modified specifically to
*   efficiently convert asn.1 Seq-entry or Bioseq-set into XML. The
*   only real difference with asn2asn -x is that a special step puts
*   the Seq-inst through the object loaders to convert 2 bit and 4 bit
*   encodings to text formats (1 base per letter) that XML fans prefer.
*
*   The defaults are set so that you can pipe the binary asn.1 Bioseq-set
*   in the GenBank release or update.
*
*   cat update.ent | asn2xml | myxmlreader
*
* Author: Jim Ostell <ostell@ncbi.nlm.nih.gov>
*
*****************************************************************************/
#include <seqport.h>
#include <objsub.h>

#define NUMARG 6
Args myargs[NUMARG] = {
	{"Filename for asn.1 input","stdin",NULL,NULL,FALSE,'i',ARG_FILE_IN,0.0,0,NULL},
	{"Input is a Seq-entry","F", NULL ,NULL ,TRUE,'e',ARG_BOOLEAN,0.0,0,NULL},
	{"Input is a Seq-submit","F", NULL ,NULL ,TRUE,'s',ARG_BOOLEAN,0.0,0,NULL},
	{"Input asnfile in binary mode","T",NULL,NULL,TRUE,'b',ARG_BOOLEAN,0.0,0,NULL},
	{"Filename for XML output","stdout", NULL,NULL,TRUE,'o',ARG_FILE_OUT,0.0,0,NULL},
    {"Log errors to file named:",NULL,NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL}};

/*****************************************************************************
*
*   Main program loop to read, process, write SeqEntrys
*
*****************************************************************************/
Int2 Main(void)
{
	AsnIoPtr aipout=NULL, aipin;
	AsnTypePtr atp, atp_inst;
	AsnModulePtr amp;
	DataVal dv;
	CharPtr ftype;
	BioseqPtr bsp;

					/* check command line arguments */

	if ( ! GetArgs("asn2xml 1.0",NUMARG, myargs))
		return 1;

					/* load the sequence alphabets  */
					/* (and sequence parse trees)   */
	if (! SeqEntryLoad())
		ErrShow();
	if (! SubmitAsnLoad())
		ErrShow();
	if (! SeqCodeSetLoad())
		ErrShow();
				    /* get pointer to all loaded ASN.1 modules */
	amp = AsnAllModPtr();
	if (amp == NULL)
	{
		ErrShow();
		return 1;
	}

	if (myargs[1].intvalue)
		atp = AsnFind("Seq-entry");
	else if (myargs[2].intvalue)
		atp = AsnFind("Seq-submit");
	else
		atp = AsnFind("Bioseq-set");    /* get the initial type pointers */
	if (atp == NULL)
	{
		ErrShow();
		return 1;
	}

	atp_inst = AsnFind("Bioseq.inst");
	if (atp_inst == NULL)
	{
		ErrShow();
		return 1;
	}

					/* open the ASN.1 input file in the right mode */

	if ((aipin = AsnIoOpen (myargs[0].strvalue, myargs[3].intvalue?"rb":"r"))
          == NULL)
	{
		ErrPostEx(SEV_ERROR,0,0, "Can't open %s", myargs[0].strvalue);
		ErrShow();
		return 1;
	}

					/* open the ASN.1 output file in the right mode */

	if (myargs[4].strvalue != NULL)   /* output desired? */
	{
		ftype = "wx";

		if ((aipout = AsnIoOpen (myargs[4].strvalue, ftype)) == NULL)
		{
			ErrPostEx(SEV_ERROR,0,0, "Can't open %s", myargs[4].strvalue);
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

	while ((atp = AsnReadId(aipin, amp, atp)) != NULL)
	{
		if (atp == atp_inst)    /* need object loader convert */
		{
			bsp = BioseqNew();  /* need newly initialized bsp */
			BioseqInstAsnRead(bsp, aipin, atp);
			BioseqInstAsnWrite(bsp, aipout, atp);
			bsp = BioseqFree(bsp);
		}
		else
		{
	 		AsnReadVal(aipin, atp, &dv); /* read it */
			AsnWrite(aipout, atp, &dv);	 /* write it */
			AsnKillValue(atp, &dv);      /* free it */
		}
	}

	AsnIoClose(aipin);
	AsnIoClose(aipout);

	return(0);
}


