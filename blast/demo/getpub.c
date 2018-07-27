/*****************************************************************************
*
*   getpub.c
*      does an indexed lookup for medline entries by medline uid
*
*****************************************************************************/
#include "allpub.h"

#define NUMARGS 5
Args myargs[NUMARGS] = {
	{ "Input binary data", "medline.val", "Pub-set", NULL, FALSE, 'i', ARG_DATA_IN, 0.0,0,NULL},
	{ "Medline UID to find", "88055872", NULL,NULL,FALSE,'u', ARG_INT, 0.0, 0, NULL },
	{ "Input index table", "medline.idx", NULL,NULL,FALSE,'t', ARG_FILE_IN, 0.0,0,NULL },
	{ "Output data", "stdout", "Medline-entry",NULL,FALSE,'o',ARG_DATA_OUT, 0.0,0,NULL},
	{ "Output data is binary", "F", NULL, NULL, FALSE , 'b', ARG_BOOLEAN, 0.0,0,NULL}};


Int2 Main(void)
{
	AsnIoPtr aip, aipout;
	AsnTypePtr atp;
	DataVal value;
	Int4 seekptr, uid, uid_to_find;
	static CharPtr outtypes[2] = { "w", "wb" };
	Int2 outtype;
	FILE *fp;
	Boolean done, first;
	int retval;

    if (! AsnLoad())
        Message(MSG_FATAL, "Unable to load allpub parse tree.");

	if (! GetArgs("GetPub 1.0", NUMARGS, myargs))
		return 1;

	if (myargs[4].intvalue)        /* binary output is TRUE */
		outtype = 1;
	else
		outtype = 0;

	if ((aip = AsnIoOpen(myargs[0].strvalue, "rb")) == NULL)
	{
		Message(MSG_ERROR, "Couldn't open %s", myargs[0].strvalue);
		return 1;
	}

	if ((aipout = AsnIoOpen(myargs[3].strvalue, outtypes[outtype])) == NULL)
	{
		Message(MSG_ERROR, "Couldn't open %s", myargs[3].strvalue);
		return 1;
	}

	if ((fp = FileOpen(myargs[2].strvalue, "r")) == NULL)
	{
		Message(MSG_ERROR, "Couldn't open %s", myargs[2].strvalue);
		return 1;
	}

	uid_to_find = myargs[1].intvalue;
	done = FALSE;
	first = TRUE;
	while (! done)
	{
		retval = fscanf(fp, "%ld %ld", &uid, &seekptr);
		if (retval == EOF)
		{
			Message(MSG_ERROR, "UID %ld not found", uid_to_find);
			return 1;
		}
		if (uid == uid_to_find)
			done = TRUE;
	}
	FileClose(fp);

	atp = MEDLINE_ENTRY;
	AsnIoSeek(aip, seekptr);
	done = FALSE;
	while (! done)
	{
		atp = AsnReadId(aip, amp, atp);
		AsnReadVal(aip, atp, &value);
		AsnWrite(aipout, atp, &value);
		AsnKillValue(atp, &value);

		if (! first)
		{
			if (atp == MEDLINE_ENTRY)
				done = TRUE;
		}
		else
			first = FALSE;
	}

	AsnIoClose(aip);
	AsnIoClose(aipout);

	return 0;
}

