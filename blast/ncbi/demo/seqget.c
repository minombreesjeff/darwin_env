/*****************************************************************************
*
*   seqget.c
*     entrez version
*
*   "Fasta style" SeqIds include a string indicating the class of SeqId,
*      vertical bar, then fields from the SeqId separated by vertical bar
*      If an (OPTIONAL) field is missing, the vertical bar must still be
*      there.
*
* local = lcl|integer or string
* gibbsq = bbs|integer
* gibbmt = bbm|integer
* giim = gim|integer
* genbank = gb|accession|locus
* embl = emb|accession|locus
* pir = pir|accession|name
* swissprot = sp|accession|name
* patent = pat|country|patent number (string)|seq number (integer)
* other = oth|accession|name|release
* general = gnl|database(string)|id (string or number)
* gi = gi|integer
* ddbj = dbj|accession|locus
* prf = prf|accession|name
* pdb = pdb|entry name (string)|chain id (char)
*
*****************************************************************************/
#include <accentr.h>
#include <sequtil.h>
#include <tofasta.h>
#include <asn2ff.h>

#define NUMARGS 5
Args myargs[NUMARGS] = {
	{"Filename for output ","stdout", NULL,NULL,FALSE,'o',ARG_FILE_OUT, 0.0,0,NULL},
	{"Output type: 1=text asn.1 2=binary asn.1 3=genbank 4=genpept 5=fasta",
	  "1", "1", "5", FALSE, 't', ARG_INT, 0.0, 0, NULL } ,
	{"GI id for single Bioseq to dump" ,"0","1","99999999",TRUE,'g',ARG_INT,0.0,0,NULL},
	{"Maximum complexity of SeqEntry: 0=as stored, 1=Bioseq, 2=Seq-set, 3=nuc-prot set " ,"0","0","3",TRUE,'c',ARG_INT,0.0,0,NULL},
 	{"Fasta style SeqId ENCLOSED IN QUOTES:\nlcl|int or str bbs|int bbm|int gb|acc|loc\nemb|acc|loc pir|acc|name sp|acc|name\npat|country|patent|seq gi|int\ndbj|acc|loc prf|acc|name pdb|entry|chain  ",
	NULL,NULL,NULL,TRUE,'s',ARG_STRING,0.0,0,NULL}};

Int2 Main(void)
{
	Int2         retcode,  /* Default is nuc-prot         */
		         outtype;  

	SeqIdPtr     sip=NULL;      /* Same as a ValNodePtr, generic data ptr implemented */
	                       /*   as a choice and a union. */

	Int4         gi;
	SeqEntryPtr  sep;
	AsnIoPtr     asnout=NULL;
	FILE *       fp=NULL;
	Boolean      is_network;
	Char tbuf[40];
	CharPtr outmode;

	/*
	** Get program arguments
	*/

	if ( !GetArgs("SeqGet 1.0", NUMARGS, myargs) ) return 1;

	/*
	** Set parameters from the command line
	*/

	outtype = (Int2)myargs[1].intvalue;
	gi = myargs[2].intvalue;
	retcode = (Int2)myargs[3].intvalue;

	if (myargs[4].strvalue != NULL)
	{
		if (gi)
		{
			ErrPostEx(SEV_FATAL, 1,0, "Use only one of -g or -s");
			return 1;
		}

		sip = SeqIdParse((CharPtr)(myargs[4].strvalue));
		if (sip == NULL)
		{
			ErrPostEx(SEV_FATAL, 1,0, "Can't parse [%s]",
				(CharPtr)(myargs[4].strvalue));
			return 1;
		}
	}
        else if (! gi)
	{
		ErrPostEx(SEV_FATAL, 1,0, "Must supply one of -g or -s");
		return 1;
	}

	/*
	** Initialize, open and otherwise prepare for CD-ROM or Network access.
	*/

	if ( !EntrezInit("SeqGet", FALSE, &is_network) ) {
		ErrPostEx(SEV_FATAL, 1,0, "Can't initialize Entrez");
		return 1;
	}

	if (sip != NULL)
	{
		gi = EntrezFindSeqId(sip);
		if (! gi)
		{
			EntrezFini();
			SeqIdPrint(sip, tbuf, PRINTID_FASTA_SHORT);
			ErrPostEx(SEV_FATAL, 1,0, "Couldn't find SeqId [%s]", tbuf);
			return 1;
		}
		SeqIdFree(sip);
	}

	sep = EntrezSeqEntryGet(gi, retcode);
	EntrezFini();

	if (sep == NULL)
	{
		ErrPostEx(SEV_FATAL, 1,0,"Could not retrieve entry for GI %ld", (long)gi);
		return 1;
	}

	outmode = "w";
	switch (outtype)
	{
		case 2:
			outmode = "wb";
		case 1:
			asnout = AsnIoOpen((CharPtr)myargs[0].strvalue, outmode);
			if (asnout == NULL)
			{
		         ErrPostEx(SEV_FATAL, 1,0, "Could not open [%s] for asn output",
		            myargs[0].strvalue);
		         return 1;
			}
			break;
		case 3:
		case 4:
		case 5:
			fp = FileOpen((CharPtr)myargs[0].strvalue, outmode);
			if (fp == NULL)
			{
		         ErrPostEx(SEV_FATAL, 1,0, "Could not open [%s] for asn output",
		            myargs[0].strvalue);
		         return 1;
			}
			break;
	}

	switch (outtype)
	{
	    case 1:
		case 2:
			SeqEntryAsnWrite(sep, asnout, NULL);
			break;
		case 3:
			SeqEntryToFlat(sep, fp, GENBANK_FMT, RELEASE_MODE);
			break;
		case 4:
			SeqEntryToFlat(sep, fp, GENPEPT_FMT, RELEASE_MODE);
			break;
		case 5:
			SeqEntryToFasta(sep, fp, TRUE);  /* nuc acids */
			SeqEntryToFasta(sep, fp, FALSE); /* proteins */
			break;
	}


	AsnIoClose(asnout);
	FileClose(fp);
	SeqEntryFree(sep);

	return 0;
}


