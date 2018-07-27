/*****************************************************************************
*
*   getseq.c
*     entrez version
*
*****************************************************************************/
#include <accentr.h>
#include <tofasta.h>

#define NUMARGS 9
Args myargs[NUMARGS] = {
 /*0*/ { "Input string id",  "M65051", NULL, NULL, FALSE, 'i', ARG_STRING, 0.0,0,NULL},
 /*1*/ { "Output Seq-entry", "T", NULL, NULL, TRUE , 's', ARG_BOOLEAN, 0.0,0,NULL},
 /*2*/ { "Output File Name", "stdout", NULL, NULL, FALSE, 'o', ARG_FILE_OUT, 0.0,0,NULL},
 /*3*/ { "GenInfo Import id", "0", NULL, NULL, TRUE , 'g', ARG_INT, 0.0,0,NULL},
 /*4*/ { "Id is GenBank accession", "T", NULL, NULL, TRUE , 'a', ARG_BOOLEAN, 0.0,0,NULL},
 /*5*/ { "Id is GenBank locus", "F", NULL, NULL, TRUE , 'l', ARG_BOOLEAN, 0.0,0,NULL},
 /*6*/ { "Id is PIR accession", "F", NULL, NULL, TRUE , 'b', ARG_BOOLEAN, 0.0,0,NULL},
 /*7*/ { "Id is PIR locus", "F", NULL, NULL, TRUE , 'p', ARG_BOOLEAN, 0.0,0,NULL},
 /*8*/ { "Return type 1=Bioseq,2=seg-set,3=nuc-prot,4=pub-set", "1", "1", "4", TRUE , 'r', ARG_INT, 0.0,0,NULL}};

Int2 Main(void)
{
	Boolean      seqentry; /* Arg 1 should always be TRUE */
	Int2         retcode;  /* Default is nuc-prot         */

	TextSeqIdPtr tsip;     /* Text object, name, locus, or release */
	SeqIdPtr     sip;      /* Same as a ValNodePtr, generic data ptr implemented */
	                       /*   as a choice and a union.  Used to pass "tsip" data */
	GiimPtr      giim;     /* GenInfo structure passed via "sip" in the special case */
	                       /*   of accessing a by GenInfo Id */

	Int4         uid;
	SeqEntryPtr  sep;
	FILE * output;

	/*
	** Get program arguments
	*/

	if ( !GetArgs("GetSeq 1.1", NUMARGS, myargs) ) return 1;

	/*
	** Set parameters from the command line
	*/

	seqentry = (Boolean)myargs[1].intvalue;
	retcode = (Int2)myargs[8].intvalue;

	/*
	** Instantiate ASN stream pointers and data.
	** "sip" is used to pass the information in "tsip".
	*/

	tsip = TextSeqIdNew();
	sip = ValNodeNew(NULL);
	sip->data.ptrvalue = tsip;

	/*
	** From the command line, set either the name or accession field
	** and the choice field of SeqIdPtr, this structure will be used
	** to return an ASN stream from the CD-ROM
	*/

	if (myargs[3].intvalue) {                      /* GenInfo import ID */
		giim = GiimNew();
		giim->id = myargs[3].intvalue;
		sip->data.ptrvalue = giim;
		sip->choice = SEQID_GIIM;
	} else if (myargs[4].intvalue)  {              /* GenBank accession */
		tsip->accession = myargs[0].strvalue;
		sip->choice = SEQID_GENBANK;
	} else if (myargs[5].intvalue)  {              /* GenBank locus     */
		tsip->name = myargs[0].strvalue;
		sip->choice = SEQID_GENBANK;
	} else if (myargs[6].intvalue) {               /* PIR accession     */
		tsip->accession = myargs[0].strvalue;
		sip->choice = SEQID_PIR;
	} else if (myargs[7].intvalue) {               /* PIR locus name    */
		tsip->name = myargs[0].strvalue;
		sip->choice = SEQID_PIR;
	}

	/*
	** Initialize, open and otherwise prepare for CD-ROM or Network access.
	*/

	if ( !EntrezInit("getfasta", FALSE, NULL) ) {
		Message(MSG_ERROR, "Init failed");
		return 1;
	}

	/*
	** Using the SeqEntryPtr structure query the access routines
	** and get the internal access number for this SeqEntry
	**
	** If the SeqEntryPtr comes up without a hit, report the error and
	** and exit.
	**
	** Otherwise, use the internal access number, uid, and retrieve a
	** a sequence entry ASN pointer, sep.  If for some strange reason
	** that it is NULL report the error and exit.
	**
	** Write a (nucleic acid) sequence to output in FASTA format
	**
	** Close the ASN file stream, free up memory create which was 
	** allocated elsewhere, sep.
	*/

	if (seqentry) {
		uid = EntrezFindSeqId(sip);
		if (! uid)
			Message(MSG_ERROR, "Couldn't find uid");
		else {
			sep = EntrezSeqEntryGet(uid, retcode);
			if (sep == NULL)
				Message(MSG_ERROR, "sep was NULL");
			else {
				output = FileOpen(myargs[2].strvalue, "w");
				SeqEntryToFasta(sep, output, TRUE);  /* nucleic acid output */
				FileClose(output);
				SeqEntryFree(sep);
			}
		}
	} else {
		Message(MSG_ERROR, "Oops");
	}

	/*
	** Close down the CD-ROM or Network access.
	** Return OK to Operating system.
	*/

	EntrezFini();
	return 0;
}


