/*****************************************************************************
*
*   makeset.c
*      build a Bioseq-set from a bunch of Seq-entry files
*      expects a file of file names in stdin
*      prints the Bioseq-set out stdout in binary
*
*****************************************************************************/
#include <all.h>

#define NUMARGS 6
Args myargs[NUMARGS] = {
	{ "Input File Names", "stdin", NULL, NULL, FALSE, 'i', ARG_FILE_IN, 0.0,0,NULL},
	{ "Input data is binary", "F", NULL, NULL, TRUE , 'd', ARG_BOOLEAN, 0.0,0,NULL},
	{ "Output data as binary", "T", NULL, NULL, TRUE , 'b', ARG_BOOLEAN, 0.0,0,NULL},
	{ "Output Bioseq-set", "stdout", "Bioseq-set", NULL, FALSE, 'o', ARG_DATA_OUT, 0.0,0,NULL},
	{ "Bioseq-set.class", "255", NULL, NULL, FALSE , 'c', ARG_INT, 0.0,0,NULL},
	{ "Bioseq-set.release", NULL, NULL, NULL, TRUE , 'r', ARG_STRING, 0.0,0,NULL}};



Int2 Main(void)
{
	AsnIoPtr aipin, aip;
	AsnTypePtr atp;
	DataVal value;
	Int4 seekptr, tempseek, uid;
	static CharPtr outtypes[2] = { "w", "wb" };
	static CharPtr intypes[2] = { "r", "rb" };
	Int2 outtype, intype;
	FILE *fp;
	Char fname[80];
	CharPtr tmp;

    if (! AsnLoad())
        Message(MSG_FATAL, "Unable to load all parse tree.");

	if (! GetArgs("MakeSet 1.0", NUMARGS, myargs))
		return 1;

	if (myargs[1].intvalue)        /* binary input is TRUE */
		intype = 1;
	else
		intype = 0;

	if (myargs[2].intvalue)        /* binary output is TRUE */
		outtype = 1;
	else
		outtype = 0;

	if ((aip = AsnIoOpen(myargs[3].strvalue, outtypes[outtype])) == NULL)
	{
		Message(MSG_ERROR, "Couldn't open %s for output", myargs[3].strvalue);
		return 1;
	}

	if ((fp = FileOpen(myargs[0].strvalue, "r")) == NULL)
	{
		Message(MSG_ERROR, "Couldn't open %s for input", myargs[0].strvalue);
		return 1;
	}

	AsnOpenStruct(aip, BIOSEQ_SET, NULL);
	value.intvalue = myargs[4].intvalue;
	AsnWrite(aip, BIOSEQ_SET_class, &value);
	if (myargs[5].strvalue != NULL)
	{
		value.ptrvalue = myargs[5].strvalue;
		AsnWrite(aip, BIOSEQ_SET_release, &value);
	}
	AsnOpenStruct(aip, BIOSEQ_SET_seq_set, NULL);

	while (FileGets(fname, (size_t)78, fp) != NULL)
	{
		tmp = fname;
		if (*tmp <= ' ') break;
		while (*tmp >= ' ') tmp++;
		*tmp = '\0';
		aipin = AsnIoOpen(fname, intypes[intype]);
		atp = SEQ_ENTRY;
		atp = AsnReadId(aipin, amp, atp);
		AsnReadVal(aipin, atp, &value);
		AsnWrite(aip, BIOSEQ_SET_seq_set_E, &value);
		while (AsnGetLevel(aipin) > 0)
		{
			atp = AsnReadId(aipin, amp, atp);
			AsnReadVal(aipin, atp, &value);
			AsnWrite(aip, atp, &value);
			AsnKillValue(atp, &value);
		}
		aipin = AsnIoClose(aipin);
	}
	AsnCloseStruct(aip, BIOSEQ_SET_seq_set, NULL);
	AsnCloseStruct(aip, BIOSEQ_SET, NULL);
	aip = AsnIoClose(aip);
	FileClose(fp);
	return 0;
}


