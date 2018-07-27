/*****************************************************************************
*
*   fdef.c
*       test program for sequence display, SeqPort and ToFasta
*
*****************************************************************************/

#include <objfdef.h>
#include <sequtil.h>

void BuildList (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);

Int2 Main(void)
{
	AsnIoPtr       aip;
	SeqEntryPtr    sep;
	FILE*          fp;
	MonitorPtr     mon;
	CharPtr groupname, featlabel, feattypelabel, featmenu;
	Uint1 groupkey, ftype;
	FeatDefPtr fdp;
	FeatDispGroupPtr fdgp;

	mon = MonitorStrNew("fdef", 40);
	SetProgMon(StdProgMon, (Pointer)mon);

	/*
	** Load SeqEntry object loader and sequence alphabets
	*/

	if (! SeqEntryLoad()) {
		Message(MSG_ERROR, "SeqEntryLoad failed");
		return 1;
	}

	/*
	** Use the file "example.prt" as the ASN I/O stream.  This file
	** can be found in the ncbi/demo.  It is in ASN.1 Print Value format.
	*/

	if ((aip = AsnIoOpen("example.prt", "r")) == NULL)
		return 1;

	/*
	** Write the output to "stdout".
	*/

	fp = FileOpen("stdout", "w");

	/*
	** Read in the whole entry into the Sequence Entry Pointer, sep.
	** Close the ASN stream, which in turn closes the input file.
	*/


	sep = SeqEntryAsnRead(aip, NULL);
	aip = AsnIoClose(aip);

	mon = MonitorFree(mon);
	SetProgMon(NULL, NULL);

	if (FeatDefSetLoad() == NULL)
	{
		Message(MSG_ERROR, "Couldn't FeatDefSetLoad");
		return 0;
	}

	fdgp = NULL;
	while ((fdgp = DispGroupFindNext(fdgp, &groupkey, &groupname)))
	{
		fprintf(fp, "[%d] %s\n", (int)groupkey, groupname);
		fdp = NULL;
		while ((fdp = FeatDefFindNext(fdp, &ftype, &featmenu, groupkey, TRUE))
			!= NULL)
		{
			fprintf(fp, "    [%d] %s\n", (int)ftype, featmenu);
		}
	}
	fprintf(fp, "\n");

	SeqEntryExplore(sep, fp, BuildList);

	SeqEntryFree(sep);

	FileClose(fp);

	return 0;
}


void BuildList(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	BioseqPtr bsp;
	BioseqSetPtr bssp;
	SeqAnnotPtr sap;
	SeqFeatPtr sfp;
	FILE *fp;
	CharPtr typelabel, label;
	Uint1 ftype;

	fp = (FILE *)data;

	if (IS_Bioseq(sep))
	{
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		sap = bsp->annot;
	}
	else
	{
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		sap = bssp->annot;
	}

	while (sap != NULL)
	{
		if (sap->type == 1)
		{
			for (sfp = (SeqFeatPtr)(sap->data); sfp != NULL; sfp = sfp->next)
			{
				ftype = FindFeatDefType(sfp);
				typelabel = FeatDefTypeLabel(sfp);
				label = FeatDefLabel(sfp);
				if (typelabel == NULL) typelabel = "(null)";
				if (label == NULL) label = "(null)";
				fprintf(fp, "[%d] [%s] [%s]\n", (int)ftype, typelabel, label);
			}
		}
		sap = sap->next;
	}


	return;
}



