
/*****************************************************************************
*
*   checksub.c
*     reads a sequenct submission with the object loaders
*
*****************************************************************************/
#include <objsub.h>

#define NUMARGS 3
Args myargs[NUMARGS] = {
	{ "Input data", NULL, "Seq-submit", NULL, FALSE, 'i', ARG_DATA_IN, 0.0,0,NULL},
	{ "Input data is binary", "F", NULL, NULL, TRUE , 'b', ARG_BOOLEAN, 0.0,0,NULL},
	{ "Output data", "stdout", "Seq-submit", NULL, FALSE, 'o', ARG_DATA_OUT, 0.0,0,NULL}};

Int2 Main(void)
{
	AsnIoPtr aip, aipout;
	AsnTypePtr atp;
	static CharPtr intypes[2] = { "r", "rb" };
	Int2 intype;
	SeqSubmitPtr ssp;

    if (! SubmitAsnLoad())
        Message(MSG_FATAL, "Unable to load parse trees.");

	atp = AsnFind("Seq-submit");
	if (atp == NULL)
		Message(MSG_FATAL, "Unable to find Seq-submit");

    if (! GetArgs("CheckSub 1.0", NUMARGS, myargs))
		return 1;

	if (myargs[1].intvalue)        /* binary input is TRUE */
		intype = 1;
	else
		intype = 0;

	if ((aip = AsnIoOpen(myargs[0].strvalue, intypes[intype])) == NULL)
	{
		Message(MSG_FATAL, "Couldn't open %s", myargs[0].strvalue);
		return 1;
	}

	if ((aipout = AsnIoOpen(myargs[2].strvalue, "w")) == NULL)
	{
		Message(MSG_ERROR, "Couldn't open %s", myargs[2].strvalue);
		return 1;
	}

	ssp = SeqSubmitAsnRead(aip, NULL);
	if (ssp == NULL)
		Message(MSG_FATAL, "Couldn't read input file");
	AsnIoClose(aip);

	if (! SeqSubmitAsnWrite(ssp, aipout, NULL))
		Message(MSG_FATAL, "Couldn't write output file");

	AsnIoClose(aipout);
	SeqSubmitFree(ssp);

	return 0;
}

