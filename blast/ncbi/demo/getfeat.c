/*****************************************************************************
*
*   getfeat.c
*   	entrez version
*
*****************************************************************************/
#include <accentr.h>
#include <seqport.h>

#define NUMARGS 9
Args myargs[NUMARGS] = {
 /*0*/ { "Input string id", "ANANIFDR1", NULL, NULL, FALSE, 'i', ARG_STRING, 0.0,0,NULL},
 /*1*/ { "Output Seq-entry", "T", NULL, NULL, TRUE , 's', ARG_BOOLEAN, 0.0,0,NULL},
 /*2*/ { "Output File Name", "getfeat.out", NULL, NULL, FALSE, 'o', ARG_FILE_OUT, 0.0,0,NULL},
 /*3*/ { "GenInfo Import id", "0", NULL, NULL, TRUE , 'g', ARG_INT, 0.0,0,NULL},
 /*4*/ { "Id is GenBank accession", "F", NULL, NULL, TRUE , 'a', ARG_BOOLEAN, 0.0,0,NULL},
 /*5*/ { "Id is GenBank locus", "T", NULL, NULL, TRUE , 'l', ARG_BOOLEAN, 0.0,0,NULL},
 /*6*/ { "Id is PIR accession", "F", NULL, NULL, TRUE , 'b', ARG_BOOLEAN, 0.0,0,NULL},
 /*7*/ { "Id is PIR locus", "F", NULL, NULL, TRUE , 'p', ARG_BOOLEAN, 0.0,0,NULL},
 /*8*/ { "Return type 1=Bioseq,2=seg-set,3=nuc-prot,4=pub-set", "3", "1", "4", TRUE , 'r', ARG_INT, 0.0,0,NULL}};

/*
** Prototype for Coding Region exploring function
*/

static void LIBCALLBACK GetSeqFeat(AsnExpOptStructPtr aeosp);

/*
** We will need to pass both a file pointer and an ASN I/O stream pointer
** to the exploring function but we can only pass one pointer. We thus 
** combine them into a single structure and pass that pointer instead.
**
** Our exploring function, "GetSeqFeat", will now have access the both
** elements of this structure.
*/

typedef struct demostruct {
	FILE * fp;
	AsnIoPtr aip;
} DemoStruct;

/*
** Our Feature Presentation...
*/

Int2 Main(void)
{
	Boolean         seqentry; /* Arg 1 should always be TRUE */
	Int2            retcode;  /* Default is nuc-prot        */

	DemoStruct PNTR dsp;      /* Struct ptr used to pass data */

	TextSeqIdPtr    tsip;     /* Text object, name, locus, or release */
	SeqIdPtr        sip;      /* Same as ValNodePtr, generic data ptr */

	GiimPtr         giim;     /* GenInfo struct passed via "sip" */

	FILE* fp;
	AsnExpOptPtr aeop;
	AsnIoPtr aip;

	Int4 uid;
	SeqEntryPtr sep;
	Boolean is_network;

	/*
	** Get program arguments
	*/

	if ( !GetArgs("GetSeq 1.0", NUMARGS, myargs) ) return 1;

	/*
	** We'll be working with SeqEntry objects, so load the
	** the loaders here, this also gets the sequence alphabets.
	*/

	if ( !SeqEntryLoad() ) return 1;

	/*
	** Set parameters from the command line
	*/

	seqentry = (Boolean)myargs[1].intvalue;
	retcode = (Int2)myargs[8].intvalue;

	/*
	** Create a DemoStruct object, we will use this struct
	** to pass two other data elements to the exploring
	** function.
	*/

	dsp = MemNew(sizeof(DemoStruct));

	/*
	** Instantiate ASN stream pointers and data holders.
	** "sip" is used to pass the information in "tsip".
	*/

	tsip = TextSeqIdNew();
	sip = ValNodeNew(NULL);
	sip->data.ptrvalue = tsip;

	/*
	** From the command line, set either the name or accession field
	** and the choice field of the SeqIdPtr, this structure will be
	** used to return an ASN object from the CD-ROM or Network.
	*/

	if (myargs[3].intvalue)  {                /* giim */
		giim = GiimNew();
		giim->id = myargs[3].intvalue;
		sip->data.ptrvalue = giim;
		sip->choice = SEQID_GIIM;
	} else if (myargs[4].intvalue) {          /* GenBank accession */
		tsip->accession = myargs[0].strvalue;
		sip->choice = SEQID_GENBANK;
	} else if (myargs[5].intvalue) {          /* GenBank locus name */
		tsip->name = myargs[0].strvalue;
		sip->choice = SEQID_GENBANK;
	} else if (myargs[6].intvalue) {          /* PIR accession */
		tsip->accession = myargs[0].strvalue;
		sip->choice = SEQID_PIR;
	} else if (myargs[7].intvalue) {          /* PIR locus name */
		tsip->name = myargs[0].strvalue;
		sip->choice = SEQID_PIR;
	}

	/*	
	** Initialize, open and otherwise prepare for CD-ROM or Network access.
	*/

	if ( !EntrezInit("GetFeat", FALSE, &is_network) ) {
		ErrShow();
	}

	/*
	** Load the DemoStruct pointer
	*/

	fp = FileOpen(myargs[2].strvalue, "w");
	dsp->fp = fp;
	dsp->aip = AsnIoNew(ASNIO_TEXT_OUT, fp, NULL, NULL, NULL);

	/*
	** Note that no exploring is done at this point.  We are only 
	** setting up for the exploration.
	**
	** The exploration function, "GetSeqFeat", will be called each time
	** a "Seq-Feat" is encountered in the ASN data stream for this entry.
	*/

	aip = AsnIoNullOpen();
	if ((aeop = AsnExpOptNew(aip, "Seq-feat", (Pointer)dsp, GetSeqFeat)) == NULL)
		ErrPost(100, 1, "no aeop 1");

	if (seqentry) {
		uid = EntrezFindSeqId(sip);
		if ( !uid )
			ErrShow();
		else {
			sep = EntrezSeqEntryGet(uid, retcode);
			if (sep == NULL)
				ErrShow();
			else {
				SeqEntryAsnWrite(sep, aip, NULL);
				fflush(fp);
				AsnIoReset(aip);
				SeqEntryFree(sep);
			}
		}
	} else {
		Message(MSG_ERROR, "Oops");
	}

	AsnIoClose(aip);
	FileClose(fp);
	EntrezFini();
	return 0;
}


/***************************************************************************8
**
** The exploration function.  Used to find coding regions
*/

void LIBCALLBACK GetSeqFeat(AsnExpOptStructPtr aeosp)
{
	SeqFeatPtr sfp;
	SeqPortPtr spp;
	Uint1      residue;
	FILE*      fp;
	Int2       i;
	Char       buffer[101];
	DemoStruct PNTR dsp;

	/*
	** Make sure we are at the beginning of a sequence feature.
	** If so, then set "sfp" to this address.
	*/

	if (aeosp->dvp->intvalue != START_STRUCT) return;

	sfp = (SeqFeatPtr)aeosp->the_struct;

	/*
	** We are looking for coding regions, 3 is a cdregion. (Defined 
	** elsewhere). Anything else simply return.
	*/

	if (sfp->data.choice != 3) return;

	/*
	** The SeqLoc may be quite complex, whereas a SeqPort is a linear
	** region.  We will use single letter IUPAC nucleic acid symbols.
	**
	** This single call represents a very complex process. Thank you NCBI!
	**
	** Seq_code_iupacna is a symbol defined elsewhere.
	*/

	spp = SeqPortNewByLoc(sfp->location, Seq_code_iupacna);
	if (spp == NULL) return;

	/*
	** Get the output file pointer from the passed data
	** structure.
	*/

	dsp = (DemoStruct PNTR) aeosp->data;
	fp = dsp->fp;

	/*
	** Write "New CdRegion" to file, then write the annotation for
	** this region in ASN Print-Value format.
	**
	** Use the demo program, "getseq" to see the entire entry. For example,
	** the default for this program uses the genbank locus "ANANIFDR1".
	**
	**       getseq -i ananifdr1 -o anaifdr1.asn -l
	*/

	fprintf(fp, "\nNew CdRegion\n");
	SeqFeatAsnWrite(sfp, dsp->aip, NULL);
	AsnIoReset(dsp->aip);

	/*
	** Write out the coding region 60 characters per line
	*/

	i = 0;
 	while ((residue = SeqPortGetResidue(spp)) != SEQPORT_EOF) {
		if (! IS_residue(residue))
		{
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
		} else {
			buffer[i] = residue;
			i++;
			if (i == 60) {
				buffer[i] = '\0';
				fprintf(fp, "%s\n", buffer);
				i = 0;
			}
		}
	}

	/*
	** Write out any remaining buffer here.
	*/

	if (i) {
		buffer[i] = '\0';
		fprintf(fp, "%s\n", buffer);
	}


	/*
	** Write out EOF to mark the end of the coding region
	** Free up memory used by the ssp. Return.
	*/

	fprintf(fp, "EOF\n");
	SeqPortFree(spp);

	return;
}

