/*****************************************************************************
*
*   condense.c
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
#include <ent2api.h>
#include <accpubseq.h>
#include <sequtil.h>
#include <tofasta.h>
#include <asn2ff.h>
#include <explore.h>

#define NUMARGS 6
Args myargs[NUMARGS] = {
	{"Filename for output ","stdout", NULL,NULL,FALSE,'o',ARG_FILE_OUT, 0.0,0,NULL},
	{"Verbose",  "F", NULL, NULL, FALSE, 'v', ARG_BOOLEAN, 0.0, 0, NULL } ,
	{"GI id for single Bioseq to dump" ,"0","1","99999999",TRUE,'g',ARG_INT,0.0,0,NULL},
 	{"Fasta style SeqId ENCLOSED IN QUOTES: lcl|int or str bbs|int bbm|int gb|acc|loc emb|acc|loc pir|acc|name sp|acc|name pat|country|patent|seq gi|int dbj|acc|loc prf|acc|name pdb|entry|chain  ",
	NULL,NULL,NULL,TRUE,'s',ARG_STRING,0.0,0,NULL},
	{"Maximum number of neighbors to get" ,"100","1","60000",TRUE,'x',ARG_INT,0.0,0,NULL},
	{"Minimum neighbor score to use" ,"2000","1","99999999",TRUE,'m',ARG_INT,0.0,0,NULL}
	};

static char * norg = "Unknown";

#define PUB_LIMIT 50

typedef struct seqinfo {
	BioseqPtr bsp;
	Int4 gi;
	SeqIdPtr sip;
	Boolean is_pdb, is_protdb, is_refseq, is_model, is_patent;
	Int4 taxid;
	Int4 length;
	CharPtr orgname;
	Char Defline[80];
	Int2 pubctr;   /* number of citations */
	Int4 PubMedId [PUB_LIMIT];
	CitArtPtr cap [PUB_LIMIT];
	ProtRefPtr prp;
	GeneRefPtr grp;
	SeqFeatPtr cds;
} SeqInfo, PNTR SeqInfoPtr;


void BioseqAnalyze (SeqInfoPtr seqs, BioseqPtr bsp, Int4 index);

typedef struct stringlist {
	Int4 count;
	Int4 id;
	VoidPtr data;
	Char * str;
} StrList, PNTR StrListPtr;

#define MAXSTR 1000
StrList sl[MAXSTR];
StrListPtr strp[MAXSTR];
Int4 strctr;

void ResetStrList(void)
{
	strctr = 0;
	return;
}

void AddStrToList(CharPtr str)
{
	Int4 i;

	if (strctr == MAXSTR) return;
	if (str == NULL) return;
	if (*str == '\0') return;

	for (i = 0; i < strctr; i++)
	{
		if (! StringICmp(str, sl[i].str))
		{
			sl[i].count++;
			return;
		}
	}
	sl[strctr].str = str;
	sl[strctr].count = 1;
	strp[strctr] = &(sl[strctr]);
	strctr++;
	return;
}

void AddDataToList(Int4 ID, VoidPtr data)
{
       Int4 i;

        if (strctr == MAXSTR) return;
        if (ID == 0) return;

        for (i = 0; i < strctr; i++)
        {
                if (ID == sl[i].id)
                {
                        sl[i].count++;
                        return;
		}
	}
        sl[strctr].id = ID;
        sl[strctr].count = 1;
	sl[strctr].data = data;
        strp[strctr] = &(sl[strctr]);
	strctr++;
        return;
}

int LIBCALLBACK StrListCountString (VoidPtr a, VoidPtr b)
{
	StrListPtr ap, bp;
	int retval = 0;

	ap = * (StrListPtr *)a;
	bp = * (StrListPtr *)b;

	if (ap->count != bp->count)
		return (int)(bp->count - ap->count);
	return (int)StringICmp(ap->str, bp->str);
}

void SortByString(void)
{
	Nlm_HeapSort((VoidPtr)strp, (size_t)strctr, sizeof(StrListPtr),
			     StrListCountString);
	return;
}

int LIBCALLBACK StrListCountID (VoidPtr a, VoidPtr b)
{
	StrListPtr ap, bp;
	int retval = 0;

	ap = * (StrListPtr *)a;
	bp = * (StrListPtr *)b;

	if (ap->count != bp->count)
		return (int)(bp->count - ap->count);
	return (int)(ap->id - bp->id);
}

void SortByID(void)
{
	Nlm_HeapSort((VoidPtr)strp, (size_t)strctr, sizeof(StrListPtr),
			     StrListCountID);
	return;
}

CharPtr GetGeneString (GeneRefPtr grp)
{
	if (grp == NULL)
		return "No Gene given";
        if (grp->locus != NULL)
                return (grp->locus);
        else if (grp->desc != NULL)
                return (grp->desc);
        else if (grp->syn != NULL)
                return (CharPtr)(grp->syn->data.ptrvalue);

	return "No Gene Name Found";
}

CharPtr GetProtString (ProtRefPtr prp)
{
	if (prp == NULL)
		return "No Protein Given";
	if (prp->name != NULL)
                return (CharPtr) (prp->name->data.ptrvalue);
        else if (prp->desc != NULL)
                return (prp->desc);

	return "No Protein Name Found";
}

Int2 Main(void)
{
	Int2         retcode;
	SeqIdPtr     sip=NULL;      /* Same as a ValNodePtr, generic data ptr implemented */
	                       /*   as a choice and a union. */

	Int4         gi, i, num, j, ctr, year;
	BioseqPtr    query, bsp;
	AsnIoPtr     asnout=NULL;
	FILE *       fp=NULL;
	Boolean      is_network, doit;
		Char tbuf[80];
	CharPtr outmode, title, name;
	ValNode vn;
	CitArtPtr cap;
	ValNodePtr vnp;
	CitJourPtr cjp;
	ImprintPtr ip;
	Int2 pdbcnt = 0, protdbcnt = 0, refseqcnt=0, modelcnt=0, patentcnt=0;
	Boolean verbose;
	Entrez2RequestPtr e2rq;
	Entrez2ReplyPtr e2ry;
	Entrez2LinkSetPtr e2lp;
	Int4Ptr linkuids, linkscores;
	Int4 MaxSeq, MinScore, LowestScore, HighestScore;
        SeqInfoPtr seqs;

	/*
	** Get program arguments
	*/

	if ( !GetArgs("Condense 1.0", NUMARGS, myargs) ) return 1;

	/*
	** Set parameters from the command line
	*/

	verbose = (Boolean)myargs[1].intvalue;
	gi = myargs[2].intvalue;
	retcode = 0;
	MaxSeq = myargs[4].intvalue;
	MinScore = myargs[5].intvalue;
	LowestScore = INT4_MAX;
	HighestScore = 0;

	if (myargs[3].strvalue != NULL)
	{
		if (gi)
		{
			ErrPostEx(SEV_FATAL, 1,0, "Use only one of -g or -s");
			return 1;
		}

		sip = SeqIdParse((CharPtr)(myargs[3].strvalue));
		if (sip == NULL)
		{
			ErrPostEx(SEV_FATAL, 1,0, "Can't parse [%s]",
				(CharPtr)(myargs[3].strvalue));
			return 1;
		}
	}
        else if (! gi)
	{
		ErrPostEx(SEV_FATAL, 1,0, "Must supply one of -g or -s");
		return 1;
	}

	/*
	** Initialize, open
	*/

	if ( !PUBSEQBioseqFetchEnable("Condense", TRUE) ) {
		ErrPostEx(SEV_FATAL, 1,0, "Can't initialize PUBSEQ");
		return 1;
	}

	if (sip != NULL)
	{
		gi = GetGIForSeqId(sip);
		if (! gi)
		{
			PUBSEQFini();
			SeqIdWrite(sip, tbuf, PRINTID_FASTA_SHORT,40);
			ErrPostEx(SEV_FATAL, 1,0, "Couldn't find SeqId [%s]", tbuf);
			return 1;
		}
		SeqIdFree(sip);
	}

	vn.choice = SEQID_GI;
	vn.next = NULL;
	vn.data.intvalue = gi;

	query = BioseqLockById(&vn);


	if (query == NULL)
	{
		ErrPostEx(SEV_FATAL, 1,0,"Could not retrieve entry for GI %ld", (long)gi);
		return 1;
	}

	/** get the neighbors ***/

	EntrezSetProgramName ("Condense");
       
        /***
	EntrezSetServer ("pluto", 5701, "/entrez/olegh/e2s.cgi");
	***/

	e2rq = EntrezCreateGetLinksRequest ( "protein", 0, 1, &gi, NULL,
		"protein_protein", MaxSeq, FALSE, TRUE);

	if (e2rq == NULL)
		printf("Couldn't create link request\n");

	if ((e2ry = EntrezSynchronousQuery(e2rq)) == NULL)
		printf("Link query failed\n");

	if ((e2lp = EntrezExtractLinksReply(e2ry)) == NULL)
		printf("Couldn't extract links from reply\n");

	if (e2lp == NULL)
		printf("Got NULL LinkSet\n");
	else
	{
		num = e2lp->ids->num;
		linkuids = (Int4Ptr) BSMerge(e2lp->ids->uids, NULL);
		linkscores = (Int4Ptr) BSMerge(e2lp->data, NULL);

		if (verbose)
			printf ("Got %ld neighbors\n", (long)(num));
		if (num > MaxSeq)
			num = MaxSeq;

		seqs = (SeqInfoPtr) MemNew((size_t)((num + 1) * sizeof(SeqInfo)));

		BioseqAnalyze(seqs, query, 0);
		ctr = 1;
		for (i = 0; (i < num) && ((linkscores[i]) >= MinScore); i++)
		{
			doit = TRUE;
			gi = linkuids[i];
			for (j = 0; j < num; j++)
			{
				if (seqs[j].gi == gi)
				{
					doit = FALSE;
					break;
				}
			}
			if (doit)
			{
				vn.data.intvalue = gi;
				bsp = BioseqLockById(&vn);
				if (bsp == NULL)
					printf ("Couldn't fetch %ld\n", (long)(gi));
				else if (verbose)
					printf ("[%d] Got %ld weight = %ld\n", (int)i, (long)(gi),
						(long)(linkscores[i]));
				BioseqAnalyze (seqs, bsp, ctr);
				if (linkscores[i] < LowestScore)
					LowestScore = linkscores[i];
				if (linkscores[i] > HighestScore)
					HighestScore = linkscores[i];
				ctr++;
			}
		}
	}

	fp = FileOpen((CharPtr)myargs[0].strvalue, "w");
	for (i = 0; i < ctr; i++)
	{
		if (verbose)
		fprintf(stdout, "[%ld] %s\n", seqs[i].gi, seqs[i].Defline);
		SeqIdWrite(seqs[i].sip, tbuf, PRINTID_FASTA_SHORT, 40);
		if (verbose)
		fprintf(stdout, "    %s  %s\n", tbuf, seqs[i].orgname);
		for (j = 0; j < seqs[i].pubctr; j++)
		{
			title = NULL;
			cap = seqs[i].cap[j];
			if (cap != NULL)
			{
				for (vnp = cap->title; vnp != NULL; vnp = vnp->next)
				{
					switch (vnp->choice)
					{
						case 1: /* name */
						   if (title == NULL)
							title = vnp->data.ptrvalue;
						   break;
						case 3: /* trans */
						   title = vnp->data.ptrvalue;
						   break;
						default:
						   break;
					}
				}
				if (title == NULL)
					title = "No Title";

				if (verbose)
				fprintf(stdout, "     [%ld] %s\n", seqs[i].PubMedId[j], title);
			} else	if (verbose)
				fprintf(stdout, "     [%ld] CitArt is NULL\n", seqs[i].PubMedId[j]);
			if (cap != NULL)
			{
				vn.choice = PUB_Article;
				vn.data.ptrvalue = cap;
				PubLabel(&vn, tbuf, 70, OM_LABEL_CONTENT);
				if (verbose)
				fprintf(stdout, "     [%s]\n", tbuf);
			}
		}
	}

	/**** real report ****/
	SeqIdWrite(seqs[0].sip, tbuf, PRINTID_FASTA_SHORT, 40);

	fprintf(fp, "Report for %s %s\n", tbuf, seqs[0].Defline);
	fprintf(fp, "Organism=[%s] ProtName=[%s] GeneName=[%s]\n", seqs[0].orgname,
			GetProtString(seqs[0].prp), GetGeneString(seqs[0].grp));
	if (seqs[0].cds != NULL)
		fprintf(fp, "A nucleic acid sequence exists for this protein\n");
	fprintf(fp, "\nEvaluating %ld neighbor sequences with scores from %ld to %ld\n\n",
		(long)(ctr), (long)HighestScore, (long)LowestScore);
	for (i = 0; i < ctr; i++)
	{
		if (seqs[i].is_pdb)
			pdbcnt++;
		if (seqs[i].is_protdb)
			protdbcnt++;
		if (seqs[i].is_refseq)
			refseqcnt++;
		if (seqs[i].is_model)
			modelcnt++;
		if (seqs[i].is_patent)
			patentcnt++;
	}
	if (pdbcnt) fprintf(fp, "[%ld] PDB; ", (long)pdbcnt);
	if (protdbcnt) fprintf(fp, "[%ld] Protein Dbs; ", (long)protdbcnt);
	if (refseqcnt) fprintf(fp, "[%ld] RefSeq; ", (long)refseqcnt);
	if (modelcnt) fprintf(fp, "[%ld] Models; ", (long)modelcnt);
	if (patentcnt) fprintf(fp, "[%ld] Patents; ", (long)patentcnt);
	fprintf(fp, "\n");

	ResetStrList();
	num = 0;
	for (i = 0; i < ctr; i++)
	{
		if (seqs[i].taxid == seqs[0].taxid)
			num++;
		else if (seqs[i].taxid != 0)
			AddStrToList(seqs[i].orgname);
	}
	fprintf(fp, "[%ld] were from %s. [%ld] were from other organisms.\n", (long)num,
			seqs[0].orgname, (long)(ctr - num));

	SortByString();
	for (i = 0; i < strctr; i++)
		fprintf(fp, "  [%ld] %s\n", (long)(strp[i]->count), strp[i]->str);
	fprintf(fp, "\n");

	ResetStrList();
	fprintf(fp, "Protein names used were:\n");
	for (i = 0; i < ctr; i++)
	{
		AddStrToList(GetProtString(seqs[i].prp));
	}
	SortByString();
	for (i = 0; i < strctr; i++)
		fprintf(fp, "  [%ld] %s\n", (long)(strp[i]->count), strp[i]->str);

	fprintf(fp, "\n");

	ResetStrList();
	fprintf(fp, "Gene names used were:\n");
	for (i = 0; i < ctr; i++)
	{
		AddStrToList(GetGeneString(seqs[i].grp));
	}
	SortByString();
	for (i = 0; i < strctr; i++)
		fprintf(fp, "  [%ld] %s\n", (long)(strp[i]->count), strp[i]->str);

	fprintf(fp, "\n");

	ResetStrList();
	fprintf(fp, "Publications cited were:\n");
	for (i = 0; i < ctr; i++)
	{
		for (j = 0; j < PUB_LIMIT; j++)
		{
			if (seqs[i].cap[j] != NULL)
				AddDataToList(seqs[i].PubMedId[j], seqs[i].cap[j]);
		}
	}
	SortByID();
	for (i = 0; i < strctr; i++)
	{
		cap = (CitArtPtr)(strp[i]->data);
		vn.choice = PUB_Article;
		vn.data.ptrvalue = cap;
		PubLabel(&vn, tbuf, 70, OM_LABEL_CONTENT);
		fprintf(fp, "\n[%ld] %s\n", (long)(strp[i]->count), tbuf);
		title = NULL;
		for (vnp = cap->title; vnp != NULL; vnp = vnp->next)
		{
			switch (vnp->choice)
			{
				case 1: /* name */
				   if (title == NULL)
					title = vnp->data.ptrvalue;
				   break;
				case 3: /* trans */
				   title = vnp->data.ptrvalue;
				   break;
				default:
				   break;
			}
		}
		if (title != NULL)
			fprintf(fp, "%s\n", title);
	}

	PUBSEQFini();
	FileClose(fp);


	return 0;
}

void BioseqAnalyze (SeqInfoPtr seqs, BioseqPtr bsp, Int4 index)
{
	SeqInfoPtr sip;
	SeqIdPtr tsip;
	SeqMgrDescContext smc;
	SeqMgrFeatContext smf;
	ValNodePtr vnp, xp;
	SeqFeatPtr sfp, cds;
	BioSourcePtr bp;
	OrgRefPtr orp;
	PubdescPtr pdp;
	CitArtPtr cap;
	Int2 ctr;
	Boolean doit;
	GeneRefPtr grp;
	ProtRefPtr prp;

	if (bsp == NULL)
		return;

	SeqMgrIndexFeatures(0, bsp);
	sip = &(seqs[index]);
	for (tsip = bsp->id; tsip != NULL; tsip = tsip->next)
	{
		switch (tsip->choice)
		{
			case SEQID_GI:
				sip->gi = tsip->data.intvalue;
				break;
			case SEQID_GENBANK:
			case SEQID_EMBL:
			case SEQID_DDBJ:
				sip->sip = tsip;
				break;
			case SEQID_SWISSPROT:
			case SEQID_PIR:
			case SEQID_PRF:
				sip->sip = tsip;
				sip->is_protdb = TRUE;
				break;
			case SEQID_PDB:
				sip->sip = tsip;
				sip->is_pdb = TRUE;
				break;
			case SEQID_PATENT:
				sip->is_patent = TRUE;
				break;
			case SEQID_OTHER:
				sip->sip = tsip;
				if (*((TextSeqIdPtr)(tsip->data.ptrvalue))->accession == 'N')
					sip->is_refseq = TRUE;
				else
					sip->is_model = TRUE;
				break;
			default:
				break;

		}
	}
	sip->length = bsp->length;
		
	for (vnp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &smc);
	     vnp != NULL;
	     vnp = SeqMgrGetNextDescriptor (bsp, vnp, Seq_descr_source, &smc))
	{
			bp = (BioSourcePtr)(vnp->data.ptrvalue);
			if (bp->org != NULL)
			{
			orp = bp->org;
			if ((bp->is_focus) || (sip->taxid == 0))
			{
				for (xp = orp->db; xp != NULL; xp = xp->next)
				{
					if (! StringICmp(((DbtagPtr)(xp->data.ptrvalue))->db, "Taxon"))
						sip->taxid = ((DbtagPtr)(xp->data.ptrvalue))->tag->id;
				}
				sip->orgname = orp->taxname;
			}
			}
	}

	CreateDefLine(NULL, bsp, sip->Defline, 70, 0, NULL, sip->orgname);
	if (sip->orgname == NULL)
		sip->orgname = "Unknown Organism";

	for (vnp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_pub, &smc);
	     vnp != NULL;
	     vnp = SeqMgrGetNextDescriptor (bsp, vnp, Seq_descr_pub, &smc))
	{
			pdp = (PubdescPtr)(vnp->data.ptrvalue);
			doit = FALSE;
			ctr = sip->pubctr;
			for (xp = pdp->pub; xp != NULL && ctr < PUB_LIMIT; xp = xp->next)
			{
				switch (xp->choice)
				{
					case PUB_Muid:
						if (sip->PubMedId[ctr] != 0)
							break;
					case PUB_PMid:
						sip->PubMedId[ctr] = xp->data.intvalue;
						doit = TRUE;
						break;
					case PUB_Article:
						sip->cap[ctr] = (CitArtPtr)(xp->data.ptrvalue);
						doit = TRUE;
						break;
					default:
						break;
				}
			}
			if (doit){  /* saved one */
				sip->pubctr++;
       }
       if ( sip->pubctr >= PUB_LIMIT){
          break;
       }
					
	}

	cds = SeqMgrGetCDSgivenProduct(bsp, &smf);
	sip->cds = cds;
	prp = NULL;
	grp = NULL;
	sfp = SeqMgrGetBestProteinFeature(bsp, &smf);
	if (sfp != NULL)
	{
		prp = (ProtRefPtr)(sfp->data.value.ptrvalue);
		grp = SeqMgrGetGeneXref(sfp);
	}
	if ((grp == NULL) && (cds != NULL))
	{
		grp = SeqMgrGetGeneXref(cds);
		if (grp == NULL)
		{
			sfp = SeqMgrGetOverlappingGene(cds->location, &smf);
			if (sfp != NULL)
				grp = (GeneRefPtr)(sfp->data.value.ptrvalue);
		}
	}
	sip->prp = prp;
	sip->grp = grp;
	

	return;
	
}
