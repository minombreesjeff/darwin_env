/*****************************************************************************
*
*   testprt.c
*       test program for print functions
*
*****************************************************************************/

#include <prtutil.h>
#include <seqport.h>

void BuildList (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);

Int2 Main(void)
{
	AsnIoPtr       aip;
	SeqEntryPtr    sep;
	SeqEntryPtr PNTR seqlist;
	Int4           seqnum, i;
	FILE*          fp;
	BioseqPtr bsp;
	BioseqSetPtr bssp;
	SeqAnnotPtr sap;
	SeqFeatPtr sfp;

	StdPrintOptionsPtr spop;

	/*
	** Load SeqEntry object loader and sequence alphabets
	*/

	if (! SeqEntryLoad()) {
		Message(MSG_ERROR, "SeqEntryLoad failed");
		return 1;
	}

	if (! PrintTemplateSetLoad("objprt.prt")) {
		Message(MSG_ERROR, "PrintTemplateSetLoad failed");
		return 1;
	}

	spop = StdPrintOptionsNew(NULL);   /* open for generating strings */
	/****
	spop->newline = "\r";
    *****/

	/*
	** Use the file "example.prt" as the ASN I/O stream.  This file
	** can be found in the ncbi/demo.  It is in ASN.1 Print Value format.
	*/

	if ((aip = AsnIoOpen("example.prt", "r")) == NULL)
		return 1;

	/*
	** Write the output to "testprt.out".
	*/

	fp = FileOpen("stdout", "w");
	spop->fp = fp;
	spop->linelength = 80;
	fprintf(fp, "Sequence summary:\n\n");

	/*
	** Read in the whole entry into the Sequence Entry Pointer, sep.
	** Close the ASN stream, which in turn closes the input file.
	*/


	sep = SeqEntryAsnRead(aip, NULL);
	aip = AsnIoClose(aip);


	/*
	** Determine how many Bioseqs are in this SeqEntry. Allocate
	** enough memory to hold a list of pointer to all of these
	** Bioseqs.  Invoke an Explore function to "visit"each Bioseq.
	** We are allowed to pass one pointer for use by the exploring
	** function, in this case, "BuildList".
	*/

	seqnum = SeqEntryCount(sep);
	seqlist = MemNew((size_t)(seqnum * sizeof(SeqEntryPtr)));
	SeqEntryExplore(sep, (Pointer) seqlist, BuildList);

	/*
	** For each Bioseq in the SeqEntry write out it's title
	** len, number of gaps, and number of segments. Write out
	** the length of each segment, up to 10.
	*/


	for(i = 0; i < seqnum; i++) {
		fprintf(fp, "\n[%d]========================\n", (int)i);
		if (IS_Bioseq(seqlist[i]))
		{
			bsp = (BioseqPtr)(seqlist[i]->data.ptrvalue);
			sap = bsp->annot;
		}
		else
		{
			bssp = (BioseqSetPtr)(seqlist[i]->data.ptrvalue);
			sap = bssp->annot;
		}
		for (sap; sap != NULL; sap = sap->next)
		{
			if (sap->type == 1)   /* feature table */
			{
				for (sfp = (SeqFeatPtr)(sap->data); sfp != NULL; sfp = sfp->next)
				{
					if (! StdFormatPrint((Pointer)sfp, (AsnWriteFunc)SeqFeatAsnWrite, "StdSeqFeat", spop))
						Message(MSG_ERROR, "StdFormatPrint failed");

				  /********
					if (! StdFormatPrint((Pointer)sfp, (AsnWriteFunc)SeqFeatAsnWrite, "EntrySeqFeatData", spop))
						Message(MSG_ERROR, "StdFormatPrint failed");
					else if (spop->ptr != NULL)
					{
						fprintf(fp, "%s\n", spop->ptr);
						MemFree(spop->ptr);
					}


					if (! StdFormatPrint((Pointer)sfp, (AsnWriteFunc)SeqFeatAsnWrite, "StdSeqFeatCommon", spop))
						Message(MSG_ERROR, "StdFormatPrint failed");
					else if (spop->ptr != NULL)
					{
						fprintf(fp, "%s\n", spop->ptr);
						MemFree(spop->ptr);
					}

					if (! StdFormatPrint((Pointer)sfp, (AsnWriteFunc)SeqFeatAsnWrite, "StdSeqFeatProduct", spop))
						Message(MSG_ERROR, "StdFormatPrint failed");
					else if (spop->ptr != NULL)
					{
						fprintf(fp, "%s\n", spop->ptr);
						MemFree(spop->ptr);
					}

					if (! StdFormatPrint((Pointer)sfp, (AsnWriteFunc)SeqFeatAsnWrite, "StdSeqFeatLocation", spop))
						Message(MSG_ERROR, "StdFormatPrint failed");
					else if (spop->ptr != NULL)
					{
						fprintf(fp, "%s\n", spop->ptr);
						MemFree(spop->ptr);
					}
				********/
				}
			}
		}
		fprintf(fp, "++++++++++++++\n");
	}

	FileClose(fp);
	MemFree(seqlist);
	SeqEntryFree(sep);


	StdPrintOptionsFree(spop);
	PrintFormatListFreeAll(TRUE);   /* free all formats and templates */

	return 0;
}

/*
** This SeqEntry exploration function copy the current pointer position inthe
** the Bioseq entry to a list of Bioseq pointers
*/

void BuildList(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	((SeqEntryPtr PNTR) data)[index] = sep;
	return;
}


