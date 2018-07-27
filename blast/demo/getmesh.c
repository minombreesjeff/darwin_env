
/*****************************************************************************
*
*   getmesh.c
*   	gets mesh terms from a Medline-entry
*
*****************************************************************************/
#include "allpub.h"

#define NUMARGS 3
Args myargs[NUMARGS] = {
	{ "Input data", NULL, "Medline-entry", NULL, FALSE, 'i', ARG_DATA_IN, 0.0,0,NULL},
	{ "Input data is binary", "F", NULL, NULL, TRUE , 'b', ARG_BOOLEAN, 0.0,0,NULL},
	{ "Output list", NULL, NULL, NULL, FALSE, 'o', ARG_FILE_OUT, 0.0,0,NULL}};

Int2 Main(void)
{
	AsnIoPtr aip;
	AsnTypePtr atp;
	DataVal value;
	static CharPtr intypes[2] = { "r", "rb" };
	Int2 intype;
	FILE *fp;

    if (! AsnLoad())
        Message(MSG_FATAL, "Unable to load allpub parse tree.");

    if (! GetArgs("GetMesh 1.0", NUMARGS, myargs))
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

	atp = MEDLINE_ENTRY;

	fprintf(fp, "MeSH terms =\n\n");
	while ((atp = AsnReadId(aip, amp, atp)) != NULL)
	{
		if (atp == MEDLINE_MESH_term)
		{
			AsnReadVal(aip, atp, &value);
			FilePuts(value.ptrvalue, fp);
			FilePuts("\n", fp);
			AsnKillValue(atp, &value);
		}
		else
			AsnReadVal(aip, atp, NULL);
	}

	aip = AsnIoClose(aip);

	FileClose(fp);

	return 0;
}

