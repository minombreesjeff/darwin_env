/*****************************************************************************
*
*   asn2fast.c
*
*****************************************************************************/
#include <tofasta.h>
#include <subutil.h>
#include <sqnutils.h>
#include <accid1.h>
#include <lsqfetch.h>

#define NUMARG 18
Args myargs[NUMARG] = {
	{"Filename for asn.1 input","stdin",NULL,NULL,TRUE,'a',ARG_FILE_IN,0.0,0,NULL},
	{"Input is a Seq-entry","F", NULL ,NULL ,TRUE,'e',ARG_BOOLEAN,0.0,0,NULL},
	{"Input asnfile in binary mode","F",NULL,NULL,TRUE,'b',ARG_BOOLEAN,0.0,0,NULL},
	{"Output Protein Filename","fasta.aa", NULL,NULL,TRUE,'p',ARG_FILE_OUT,0.0,0,NULL},
	{"Output DNA Filename","fasta.na", NULL,NULL,TRUE,'n',ARG_FILE_OUT,0.0,0,NULL},
	{"Log errors to file named:",NULL,NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL},
	{"Combine segmented or delta sequences","F",NULL,NULL,TRUE,'c',ARG_BOOLEAN,0.0,0,NULL},
	{"Produce Protein File","T",NULL,NULL,TRUE,'x',ARG_BOOLEAN,0.0,0,NULL},
	{"Produce DNA File","T",NULL,NULL,TRUE,'d',ARG_BOOLEAN,0.0,0,NULL},
	{"Limit to GenBank","F",NULL,NULL,TRUE,'g',ARG_BOOLEAN,0.0,0,NULL},
	{"Instantiate virtual sequences","F",NULL,NULL,TRUE,'v',ARG_BOOLEAN,0.0,0,NULL},
	{"Input is a Seq-submit","F", NULL ,NULL ,TRUE,'s',ARG_BOOLEAN,0.0,0,NULL},
	{"Produce output file of Quality Scores (DNA sequences only)","F",NULL,NULL,TRUE,'q',ARG_BOOLEAN,0.0,0,NULL},
	{"Output Filename for Quality Scores (DNA sequences only)","scores.ql", NULL,NULL,TRUE,'y',ARG_FILE_OUT,0.0,0,NULL},
	{"Far Genomic Contig function for Quality Scores","F",NULL,NULL,TRUE,'f',ARG_BOOLEAN,0.0,0,NULL},
	{"Remote fetching", "F", NULL, NULL, FALSE, 'r', ARG_BOOLEAN, 0.0, 0, NULL},
	{"Local fetching", "F", NULL, NULL, FALSE, 'k', ARG_BOOLEAN, 0.0, 0, NULL},
	{"Print Quality Score Gap as -1, false prints as 0", "F", NULL, NULL, FALSE, 'z', ARG_BOOLEAN, 0.0, 0, NULL},
};

static void PrintQualProc (CharPtr buf, Uint4 buflen, Pointer userdata)

{
  FILE  *fp;

  fp = (FILE*) userdata;
  fprintf (fp, "%s", buf);
}

static void PrintQualScores (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)

{
	BioseqPtr  bsp;
	FILE       *fp;

	if (IS_Bioseq (sep)) {
		bsp = (BioseqPtr) sep->data.ptrvalue;

		/* WARNING: we're assuming here that asn2fast's quality-score
		   output is DNA-centric, thus protein bioseqs can be ignored
		   in the PrintQualScores callback. --MLC, 5/2000 */

		if (ISA_aa(bsp->mol))
		  return;

		fp = (FILE*) data;
		if (myargs [17].intvalue) {
		  PrintQualityScoresToBuffer (bsp, FALSE, fp, PrintQualProc);
		} else {
		  PrintQualityScoresToBuffer (bsp, TRUE, fp, PrintQualProc);
		}
	}
}

static void PrintFarQualScores (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)

{
	BioseqPtr  bsp;
	FILE       *fp;

	if (IS_Bioseq (sep)) {
		bsp = (BioseqPtr) sep->data.ptrvalue;

		/* WARNING: we're assuming here that asn2fast's quality-score
		   output is DNA-centric, thus protein bioseqs can be ignored
		   in the PrintQualScores callback. --MLC, 5/2000 */

		if (ISA_aa(bsp->mol))
		  return;

		fp = (FILE*) data;
		if (myargs [17].intvalue) {
		  PrintQualityScoresForContig (bsp, FALSE, fp);
		} else {
		  PrintQualityScoresForContig (bsp, TRUE, fp);
		}
	}
}


Boolean CheckIsGenBank(SeqEntryPtr sep);

Int2 Main(void)
{
	AsnIoPtr aip;
	FILE * aa = NULL, * na = NULL, * ql = NULL;
	SeqEntryPtr sep;
	SeqSubmitPtr ssp;
	AsnTypePtr atp, atp2;
	AsnModulePtr amp;
	Uint1 group_segs = 0;
	Boolean limit_to_genbank,
		make_dna,
		make_protein,
		make_quality,
		far_quality,
		do_it;
	

					/* check command line arguments */

	if ( ! GetArgs("asn2fast",NUMARG, myargs))
		return 1;

					/* load the sequence alphabets  */
					/* (and sequence parse trees)   */
	if (! SeqEntryLoad())
	{
		ErrShow();
		return 1;
	}
				    /* get pointer to all loaded ASN.1 modules */
	amp = AsnAllModPtr();
	if (amp == NULL)
	{
		ErrShow();
		return 1;
	}

	if (myargs[11].intvalue) {
		if (! SubmitAsnLoad())
			Message(MSG_FATAL, "Unable to load parse trees.");
		
		atp2 = AsnFind("Seq-submit");
		if (atp2 == NULL)
			Message(MSG_FATAL, "Unable to find Seq-submit");
		atp = AsnFind("Seq-submit");
		if (atp == NULL)
			Message(MSG_FATAL, "Unable to find Seq-submit");

	} else {
		atp = AsnFind("Bioseq-set"); /* get the initial type pointers */
		if (atp == NULL)
		{
			ErrShow();
			return 1;
		}
	
		atp2 = AsnFind("Bioseq-set.seq-set.E");
		if (atp2 == NULL)
		{
			ErrShow();
			return 1;
		}
	}

	make_protein = (Boolean)(myargs[7].intvalue);
	make_dna = (Boolean)(myargs[8].intvalue);
	make_quality = (Boolean)(myargs[12].intvalue);
	far_quality = (Boolean)(myargs[14].intvalue);

					/* open the ASN.1 input file in the right mode */

	if ((aip = AsnIoOpen (myargs[0].strvalue, myargs[2].intvalue?"rb":"r"))
          == NULL)
	{
		ErrShow();
		return 1;
	}

				  				/* open the output file */

	if ((myargs[3].strvalue != NULL) && (make_protein))
	{
		if ( (aa = FileOpen (myargs[3].strvalue, "w")) == NULL)
		{
			ErrShow();
			return 1;
		}
	}

	if ((myargs[4].strvalue != NULL) && (make_dna))
	{
		if ( (na = FileOpen (myargs[4].strvalue, "w")) == NULL)
		{
			ErrShow();
			return 1;
		}
	}

	if ((myargs[13].strvalue != NULL) && (make_quality))
	{
		if ( (ql = FileOpen (myargs[13].strvalue, "w")) == NULL)
		{
			ErrShow();
			return 1;
		}
	}

                                /* log errors instead of die */
    if (myargs[5].strvalue != NULL)
    {
        if (! ErrSetLog (myargs[5].strvalue))
            ErrShow();
        else
            ErrSetOpts (ERR_CONTINUE, ERR_LOG_ON);
   }

	if (myargs[6].intvalue)  /* combine segmented seqs */
	{
		group_segs = 1;
		if (myargs[10].intvalue)
			group_segs = 3;       /* and instantiate virtuals */
	}

	limit_to_genbank = (Boolean)(myargs[9].intvalue);

	if (myargs [15].intvalue) {
		ID1BioseqFetchEnable ("asn2fast", FALSE);
	}
	if (myargs [16].intvalue) {
		LocalSeqFetchInit (FALSE);
	}

	if ( myargs[1].intvalue)   /* read one Seq-entry */
	{

		sep = SeqEntryAsnRead(aip, NULL);
		do_it = TRUE;
		if (limit_to_genbank)
			do_it = CheckIsGenBank(sep);
		if (do_it)
		{
			if (make_protein)
				SeqEntrysToFasta(sep, aa, FALSE, group_segs);
			if (make_dna)
				SeqEntrysToFasta(sep, na, TRUE, group_segs);
			if (make_quality) {
				if (far_quality) {
					SeqEntryExplore (sep, (Pointer) ql, PrintFarQualScores);
				} else {
					SeqEntryExplore (sep, (Pointer) ql, PrintQualScores);
				}
			}
		}
		SeqEntryFree(sep);
	}
	else if ( myargs[11].intvalue)   /* read Seq-submit's */
	{
		while ((atp = AsnReadId(aip, amp, atp)) != NULL)
		{
			if (atp == atp2)    /* top level Seq-entry */
			{
				ssp = SeqSubmitAsnRead(aip, atp);
				if (ssp->datatype == 1)
				{
					sep = (SeqEntryPtr) ssp->data;
					do_it = TRUE;
					if (limit_to_genbank)
						do_it = CheckIsGenBank(sep);
					if (do_it)
					{
						if (make_protein)
							SeqEntrysToFasta(sep, aa, FALSE, group_segs);
						if (make_dna)
							SeqEntrysToFasta(sep, na, TRUE, group_segs);
						if (make_quality) {
							if (far_quality) {
								SeqEntryExplore (sep, (Pointer) ql, PrintFarQualScores);
							} else {
								SeqEntryExplore (sep, (Pointer) ql, PrintQualScores);
							}
						}
					}
				}
				SeqSubmitFree(ssp);
			}
			else
			{
				AsnReadVal(aip, atp, NULL);
			}
		}
	}
	else                      /* read Seq-entry's from a Bioseq-set */
	{
		while ((atp = AsnReadId(aip, amp, atp)) != NULL)
		{
			if (atp == atp2)    /* top level Seq-entry */
			{
				sep = SeqEntryAsnRead(aip, atp);
				do_it = TRUE;
				if (limit_to_genbank)
					do_it = CheckIsGenBank(sep);
				if (do_it)
				{
					if (make_protein)
						SeqEntrysToFasta(sep, aa, FALSE, group_segs);
					if (make_dna)
						SeqEntrysToFasta(sep, na, TRUE, group_segs);
					if (make_quality) {
						if (far_quality) {
							SeqEntryExplore (sep, (Pointer) ql, PrintFarQualScores);
						} else {
							SeqEntryExplore (sep, (Pointer) ql, PrintQualScores);
						}
					}
				}
				SeqEntryFree(sep);
			}
			else
			{
				AsnReadVal(aip, atp, NULL);
			}
		}
	}

	AsnIoClose(aip);
	if (make_protein)
		FileClose(aa);
	if (make_dna)
		FileClose(na);
	if (make_quality)
		FileClose (ql);

	if (myargs [16].intvalue) {
		LocalSeqFetchDisable ();
	}
	if (myargs [15].intvalue) {
		ID1BioseqFetchDisable ();
	}

	return(0);
}

void FindGenBank (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);

Boolean CheckIsGenBank(SeqEntryPtr sep)
{
	Boolean retval = FALSE;

	SeqEntryExplore(sep, (Pointer)(&retval), FindGenBank);

	return retval;
}

void FindGenBank (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	BoolPtr ptr;
	BioseqPtr bsp;
	ValNodePtr vnp;

	ptr = (BoolPtr)data;
	if (*ptr == TRUE)   /* already know */
		return;

	if (IS_Bioseq(sep))
	{

	   bsp = (BioseqPtr)(sep->data.ptrvalue); 
	                    /* GenBank is a limited view of the world */
	   if ( (ISA_na(bsp->mol)) && ( (bsp->repr == Seq_repr_raw) || (bsp->repr == Seq_repr_delta) ) )
	   {
			for (vnp = bsp->id; vnp != NULL; vnp = vnp->next)
			{
				switch (vnp->choice)
				{
					case SEQID_GENBANK:
					case SEQID_EMBL:
					case SEQID_DDBJ:
						*ptr = TRUE;
						return;
					default:
						break;
				}
			}
	   }
	}

	return;
}

