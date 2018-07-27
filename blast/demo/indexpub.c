/*****************************************************************************
*
*   indexpub.c
*      indexes a Pub-set by Medline UID
*
*****************************************************************************/
#include "allpub.h"

#define NUMARGS 3
Args myargs[NUMARGS] = {
	{ "Input data", "medline.val", "Pub-set", NULL, FALSE, 'i', ARG_DATA_IN, 0.0,0,NULL},
	{ "Input data is binary", "T", NULL, NULL, TRUE , 'b', ARG_BOOLEAN, 0.0,0,NULL},
	{ "Output index table", "medline.idx", NULL, NULL, FALSE, 't', ARG_FILE_OUT, 0.0,0,NULL}};


Int2 Main(void)
{
	AsnIoPtr aip;
	AsnTypePtr atp;
	DataVal value;
	Int4 seekptr, tempseek, uid;
	static CharPtr intypes[2] = { "r", "rb" };
	Int2 intype;
	FILE *fp;

    if (! AsnLoad())
        Message(MSG_FATAL, "Unable to load allpub parse tree.");

	if (! GetArgs("IndexPub 1.0", NUMARGS, myargs))
		return 1;

	if (myargs[1].intvalue)        /* binary input is TRUE */
		intype = 1;
	else
		intype = 0;

	if ((aip = AsnIoOpen(myargs[0].strvalue, intypes[intype])) == NULL)
	{
		Message(MSG_ERROR, "Couldn't open %s", myargs[0].strvalue);
		return 1;
	}

	if ((fp = FileOpen(myargs[2].strvalue, "w")) == NULL)
	{
		Message(MSG_ERROR, "Couldn't open %s", myargs[2].strvalue);
		return 1;
	}

	atp = PUB_SET;
	tempseek = 0L;

	while ((atp = AsnReadId(aip, amp, atp)) != NULL)
	{
		if (atp == PUB_SET_medline_E)
			seekptr = tempseek;
		if (atp == MEDLINE_ENTRY_uid)
		{
			AsnReadVal(aip, atp, &value);
			uid = value.intvalue;
			fprintf(fp, "%ld %ld\n", uid, seekptr);
		}
		else
			AsnReadVal(aip, atp, NULL);
		tempseek = AsnIoTell(aip);
	}

	aip = AsnIoClose(aip);
	FileClose(fp);
	return 0;

}

