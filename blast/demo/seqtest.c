/*****************************************************************************
*
*   seqtest.c
*       test program for sequence display, SeqPort and ToFasta
*
*****************************************************************************/

#include <seqport.h>
#include <tofasta.h>

void BuildList (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);

#define NUMARG 2
Args myargs[NUMARG] = {
	{"Filename for asn.1 input","stdin",NULL,NULL,FALSE,'i',ARG_FILE_IN,0.0,0,NULL},
	{"Filename for output","stdout", NULL,NULL,TRUE,'o',ARG_FILE_OUT,0.0,0,NULL}};

Int2 Main(void)
{
	AsnIoPtr       aip;
	SeqEntryPtr    sep;
	BioseqPtr PNTR seqlist;
	Int4           seqnum, i, numseg, lens[10], j;
	Int2           ctr;
	SeqPortPtr     spp;
	Uint1          residue;
	FILE*          fp;
	CharPtr        title;
	Char           buffer[101];
	MonitorPtr     mon;

						/* check command line arguments */

	if ( ! GetArgs("SeqTest",NUMARG, myargs))
		return 1;

	mon = MonitorStrNew("SeqTest", 40);
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

	if ((aip = AsnIoOpen(myargs[0].strvalue, "r")) == NULL)
		return 1;

	/*
	** Write the output to "seqtest.out".
	*/

	fp = FileOpen(myargs[1].strvalue, "w");
	fprintf(fp, "Sequence summary:\n\n");

	/*
	** Read in the whole entry into the Sequence Entry Pointer, sep.
	** Close the ASN stream, which in turn closes the input file.
	*/

	sep = SeqEntryAsnRead(aip, NULL);
	aip = AsnIoClose(aip);

	mon = MonitorFree(mon);
	SetProgMon(NULL, NULL);

	/*
	** Determine how many Bioseqs are in this SeqEntry. Allocate
	** enough memory to hold a list of pointer to all of these
	** Bioseqs.  Invoke an Explore function to "visit"each Bioseq.
	** We are allowed to pass one pointer for use by the exploring
	** function, in this case, "BuildList".
	*/

	seqnum = BioseqCount(sep);
	seqlist = MemNew((size_t)(seqnum * sizeof(BioseqPtr)));
	BioseqExplore(sep, (Pointer) seqlist, BuildList);

	/*
	** For each Bioseq in the SeqEntry write out it's title
	** len, number of gaps, and number of segments. Write out
	** the length of each segment, up to 10.
	*/

	for(i = 0; i < seqnum; i++) {
		numseg = BioseqCountSegs(seqlist[i]);
		title = BioseqGetTitle(seqlist[i]);
		FilePuts((VoidPtr)title, fp);
		FilePuts("\n", fp);
		fprintf(fp, "len=%ld gaps=%ld segs=%ld\n", BioseqGetLen(seqlist[i]),
		BioseqGetGaps(seqlist[i]), numseg);
		if ((numseg > 1) && (numseg <= 10)) {
			BioseqGetSegLens (seqlist[i], lens);
			for (j = 0; j < numseg; j++)
				fprintf(fp, "  len = %ld\n", lens[j]);
		}
		FilePuts("\n", fp);
	}

	spp = SeqPortNew(seqlist[0], 0, -1, 0, Seq_code_iupacna);
	if (spp == NULL)
		Message(MSG_ERROR, "fail on SeqPortNew");

	fprintf(fp, "SeqPort: plus strand with SeqPortGetResidue\n\n");

	i = 0;
	while ((residue = SeqPortGetResidue(spp)) != SEQPORT_EOF) {
		if (! IS_residue(residue)) {
			buffer[i] = '\0';
			fprintf(fp, "%s\n", buffer);
			i = 0;
			switch (residue)
			{
				case SEQPORT_VIRT:
					fprintf(fp, "[Gap]\n");
					break;
				case SEQPORT_EOS:
					fprintf(fp, "[EOS]\n");
					break;
				default:
					fprintf(fp, "[Invalid Residue]\n");
					break;
			}

		}
		else {
			buffer[i] = residue;
			i++;
			if (i == 60) {
				buffer[i] = '\0';
				fprintf(fp, "%s\n", buffer);
				i = 0;
			}
		}
	}

	if (i) {
		buffer[i] = '\0';
		fprintf(fp, "%s\n", buffer);
	}

	fprintf(fp, "[EOF]\n");
	SeqPortFree(spp);

	fprintf(fp, "\nSeqPort on minus with SeqPortRead\n\n");
	spp = SeqPortNew(seqlist[0], 0, -1, Seq_strand_minus, Seq_code_iupacna);

	if (spp == NULL)
		Message(MSG_ERROR, "fail on SeqPortNew");

	do {
		ctr = SeqPortRead(spp, (Uint1Ptr)buffer, 60);
		if (ctr > 0) {
			buffer[ctr] = '\0';
			fprintf(fp, "%s\n", buffer);
		} else {
			ctr *= -1;
			switch (ctr)
			{
				case SEQPORT_VIRT:
					fprintf(fp, "[Gap]\n");
					break;
				case SEQPORT_EOS:
					fprintf(fp, "[EOS]\n");
					break;
				case SEQPORT_EOF:
					fprintf(fp, "[EOF]\n");
					break;
				default:
					fprintf(fp, "[Invalid Residue]\n");
					break;
			}
		}
	} while (ctr != SEQPORT_EOF);

	SeqPortFree(spp);

	/*
	** Write out the nucleic acid sequences in this SeqEntry
	*/

	fprintf(fp, "\nNucleic Acids in FASTA format:\n\n");
	SeqEntryToFasta(sep, fp, TRUE);

	/*
	** Write out the protein sequences in this SeqEntry.
	*/

	fprintf(fp, "\nProteins in FASTA format:\n\n");
	SeqEntryToFasta(sep, fp, FALSE);

	/*
	** Close the output file and free up allocated space.
	*/

	fclose(fp);
	MemFree(seqlist);
	SeqEntryFree(sep);

	return 0;
}

/*
** This SeqEntry exploration function copy the current pointer position inthe
** the Bioseq entry to a list of Bioseq pointers
*/

void BuildList(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	((BioseqPtr PNTR) data)[index] = (BioseqPtr)sep->data.ptrvalue;
	return;
}



