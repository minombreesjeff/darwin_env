/*****************************************************************************
*
*   genget.c
*     entrez version
*
*****************************************************************************/
#include <accentr.h>
#include <sequtil.h>
#include <tofasta.h>
#include <asn2ff.h>
#include <asn2ffg.h>
#include <explore.h>
#include "putil.h"
#include <accid1.h>

void SaveProtTabToTabFile(Int4 gi, FILE *fp, SeqEntryPtr sep, Int4 from, Int4 to, CharPtr title);

Args myargs[] = {
	{"GI id for single Bioseq to dump" ,"0","1","99999999",
				TRUE,'g',ARG_INT,0.0,0,NULL},
	{"Input filename ","stdin", NULL,NULL,TRUE,'i',ARG_FILE_IN, 0.0,0,NULL},
	{"Input asnfile in binary mode",
			"F",NULL,NULL,TRUE,'b',ARG_BOOLEAN,0.0,0,NULL},
	{"Output complete flat file?","T", NULL ,NULL ,TRUE,'c',
			ARG_BOOLEAN,0.0,0,NULL},
	{"Output short flat file?","T", NULL ,NULL ,TRUE,'s',
				ARG_BOOLEAN,0.0,0,NULL},
	{"Output FASTA file?","T", NULL ,NULL ,TRUE,'f',ARG_BOOLEAN,0.0,0,NULL},
	{"New output filename", NULL, NULL ,NULL ,TRUE,'o',ARG_STRING,0.0,0,NULL},
	{"Output compress file?","F", NULL ,NULL ,TRUE,'z',ARG_BOOLEAN,0.0,0,NULL},
	{"Input is a Seq-entry","T", NULL ,NULL ,TRUE,'e',ARG_BOOLEAN,0.0,0,NULL},
	{"Input is a Seq-submit","F", NULL ,NULL ,TRUE,'q',ARG_BOOLEAN,0.0,0,NULL},
	{"Output prottable?","T", NULL ,NULL ,TRUE,'P',
				ARG_BOOLEAN,0.0,0,NULL},
	{"Output all proteins FASTA?","T", NULL ,NULL ,TRUE,'p',
				ARG_BOOLEAN,0.0,0,NULL},
	{"Output history assembly table?","T", NULL ,NULL ,TRUE,'l',
				ARG_BOOLEAN,0.0,0,NULL},
	{"Output gi list file?","F", NULL ,NULL ,TRUE,'t',
				ARG_BOOLEAN,0.0,0,NULL},
	{"Output special Sequin table?","F", NULL ,NULL ,TRUE,'S',
				ARG_BOOLEAN,0.0,0,NULL},
	{"Output FASTA file with n's?","F", NULL ,NULL ,TRUE,'v',
				ARG_BOOLEAN,0.0,0,NULL},
	{"Output DNA pieces FASTA ?","F", NULL ,NULL ,TRUE,'w',
				ARG_BOOLEAN,0.0,0,NULL},
	{"Output CDS  nucleotide FASTA ?","T", NULL ,NULL ,TRUE,'n',
				ARG_BOOLEAN,0.0,0,NULL},
	{"Output nucleotide GI ?","F", NULL ,NULL ,TRUE,'G',
				ARG_BOOLEAN,0.0,0,NULL},
	{"Output the sequence in GenBank format?","T", NULL ,NULL ,TRUE,'O',
				ARG_BOOLEAN,0.0,0,NULL},
	{"Is it a Master sequence?","T", NULL ,NULL ,TRUE,'M',
				ARG_BOOLEAN,0.0,0,NULL},
	{"Ignore top features?","T", NULL ,NULL ,TRUE,'I',
				ARG_BOOLEAN,0.0,0,NULL},
	{"Use dump mode?","T", NULL ,NULL ,TRUE,'D',
				ARG_BOOLEAN,0.0,0,NULL},
	};

static void FindTopTitle (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  ValNodePtr	descr, vnp;
  CharPtr PNTR	title;

	title = (CharPtr PNTR) data;
	if (*title != NULL)
		return;
	if (sep == NULL || sep->data.ptrvalue == NULL) return;
	if (IS_Bioseq (sep)) {
		bsp = (BioseqPtr) sep->data.ptrvalue;
		descr = bsp->descr;
	} else if (IS_Bioseq_set (sep)) {
		bssp = (BioseqSetPtr) sep->data.ptrvalue;
		descr = bssp->descr;
	} else {
		return;
	}
	for (vnp = descr; vnp; vnp = vnp->next) {
    	if (vnp->choice == Seq_descr_title) {
    		*title = StringSave(vnp->data.ptrvalue);
    		return;
    	}
	}
	return;
}

static void FindBigCallback(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	ValNodePtr vnp;
	BioseqPtr bsp, curr;

	if (sep->choice != 1)
		return;

	if (data == NULL)
		return;

	vnp = (ValNodePtr)data;
	bsp = sep->data.ptrvalue;
	if (bsp) {
		if (vnp->data.ptrvalue == NULL) {
			vnp->data.ptrvalue = bsp;
		} else {
			curr = (BioseqPtr)(vnp->data.ptrvalue);
			if (bsp->length > curr->length) {
				vnp->data.ptrvalue = bsp;
			}
		}             
	}
}

static BioseqPtr find_big_bioseq(SeqEntryPtr sep)
{
        ValNode vn;

        vn.data.ptrvalue = NULL;
        if (sep != NULL)
                BioseqExplore (sep, (Pointer) (&vn), FindBigCallback);
        return (BioseqPtr)(vn.data.ptrvalue);
}

static void CheckForCookedBioseqs (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BoolPtr    bp;
  BioseqPtr  bsp;

  if (sep == NULL) return;
  if (! IS_Bioseq (sep)) return;
  bp = (BoolPtr) mydata;
  if (bp == NULL) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;
  if (bsp->repr != Seq_repr_raw && bsp->repr != Seq_repr_seg) {
    *bp = FALSE;
  }
}

static void IndexASeqEntry (SeqEntryPtr sep, Boolean useSeqMgrIndexes)

{
  Boolean  allRawOrSeg = TRUE;
  Uint2    entityID;

  if (sep == NULL || (! useSeqMgrIndexes)) return;
  while (sep != NULL) {
    SeqEntryExplore (sep, (Pointer) (&allRawOrSeg), CheckForCookedBioseqs);
    if (allRawOrSeg) {
      entityID = ObjMgrGetEntityIDForChoice (sep);
      SeqMgrIndexFeatures (entityID, NULL);
    }
    sep = sep->next;
  }
}

Int2 Main()
{
	Int2         retcode;  /* Default is -1 (genomes)     */
	Int4         gi = 0;
	SeqEntryPtr  sep;
	FILE *gbk=NULL; 
	FILE *gbs=NULL; 
	FILE *fsa=NULL;
	FILE *pta=NULL;
	FILE *paa=NULL;
	FILE *cds=NULL;
	FILE *tab=NULL;
	FILE *gil=NULL;
	FILE *stb=NULL;
	FILE *dna=NULL;
	FILE *gdna=NULL;
	AsnModulePtr amp;
	BioseqPtr bsp;
	AsnIoPtr aip = NULL, aop = NULL;
	SeqSubmitPtr ssp;
	BioseqSetPtr bssp;
	Boolean      is_network;
	Char tbuf[40];
	CharPtr outmode, title = NULL;
	Asn2ffJobPtr ajp;
	Uint2 entityID;
	CharPtr	infile, infile0, s, tarfile = NULL;
	static Char command[255];
	static CharPtr afile=NULL, bfile=NULL, cfile=NULL, dfile=NULL, dnafile=NULL,
	efile=NULL, pfile=NULL, nfile=NULL, Pfile=NULL, hfile=NULL, gfile=NULL, sfile=NULL;
	Int2 f;
	Int2        argcount;

	argcount = sizeof (myargs) / sizeof (Args);
	if ( !GetArgs("GenGet 1.0", argcount, myargs) ) return 1;

	if ( !EntrezInit("GenGet", FALSE, &is_network) ) {
			ErrPostEx(SEV_FATAL, 1,0, "Can't initialize Entrez");
			return 1;
	}
	SeqEntryLoad();
	amp = AsnAllModPtr();
	EntrezBioseqFetchEnable ("genget", FALSE); 
	ID1BioseqFetchEnable ("asn2ff", FALSE); 

	gi = myargs[0].intvalue;

	if (gi != 0) {
		retcode = -1;
		sep = EntrezSeqEntryGet(gi, retcode);
		if (sep == NULL) {
			ErrPostEx(SEV_FATAL, 1,0,
				"Could not retrieve entry for GI %ld", 	(long)gi);
			return 1;
		} else {
			ErrPostEx(SEV_INFO,0,0,
				"Start processing entry for GI %ld", 	(long)gi);
		}
		
	} else {
		if ((aip = 
		AsnIoOpen (myargs[1].strvalue, myargs[2].intvalue?"rb":"r")) == NULL) {
			ErrPostEx(SEV_FATAL, 1,0,"Could not open file %s", 	myargs[1].strvalue);
			return 1;
		}
		if (myargs[8].intvalue) {
			sep = SeqEntryAsnRead(aip, NULL);
		} else if (myargs[9].intvalue) {
			ssp = SeqSubmitAsnRead(aip, NULL);
			if (ssp->datatype == 1) {
				sep = (SeqEntryPtr) ssp->data;
			}
		} else {
			bssp = BioseqSetAsnRead(aip, NULL);
			sep = SeqEntryNew();
			sep->data.ptrvalue = bssp;
			sep->choice = 2;
		}
		if (sep == NULL) {
			ErrPostEx(SEV_FATAL, 1,0,"SeqEntryAsnRead failed");
			return 1;
		} else {
			ErrPostEx(SEV_INFO,0,0,
				"Start processing entry from file %s", 	myargs[1].strvalue);
		}
	}
	
/*	IndexASeqEntry (sep, TRUE);*/
	SeqMgrIndexFeatures(0, sep);
	
	/* create output filenames */
	if (myargs[6].strvalue != NULL) {
		infile0 = (CharPtr) MemNew(StringLen(myargs[6].strvalue) + 1);
		infile = infile0;
		StringCpy(infile, myargs[6].strvalue);
	} else if (gi) {
		f = (Int2) log10((double) gi) + 1;
		infile0 = (CharPtr) MemNew(f + 2);
		infile = infile0;
		sprintf(infile, "%ld", gi);
		
	} else {
		infile0 = (CharPtr) MemNew(StringLen(myargs[1].strvalue) + 1);
		infile = infile0;
		StringCpy(infile, myargs[1].strvalue);
		if ((s = strrchr(infile, '/')) != NULL) {
			infile = s + 1;
		}
		if ((s = strchr(infile, '.')) != NULL) {
			*s = '\0';
		}
	}
	if (myargs[7].intvalue) {
		tarfile = (CharPtr) MemNew(StringLen(infile) + 5);
		StringCpy(tarfile, infile);
		StringCat(tarfile, ".tar");
	}
		dfile = (CharPtr) MemNew(StringLen(infile) + 5);
		StringCpy(dfile, infile);
		StringCat(dfile, ".asn");
		aop = AsnIoOpen(dfile, "w");
		SeqEntryAsnWrite(sep, aop, NULL);
		AsnIoClose(aop);
		efile = (CharPtr) MemNew(StringLen(infile) + 5);
		StringCpy(efile, infile);
		StringCat(efile, ".val");
		aop = AsnIoOpen(efile, "wb");
		SeqEntryAsnWrite(sep, aop, NULL);
		AsnIoClose(aop);


	if (myargs[3].intvalue) {
		afile = (CharPtr) MemNew(StringLen(infile) + 5);
		StringCpy(afile, infile);
		StringCat(afile, ".gbk");

		if ( (gbk = FileOpen (afile, "w")) == NULL) {
			ErrPostEx(SEV_ERROR,0,0, "Can't open %s", afile);
			exit (1);
		}
	}
	if (myargs[4].intvalue) {
		bfile = (CharPtr) MemNew(StringLen(infile) + 5);
		StringCpy(bfile, infile);
		StringCat(bfile, ".gbs");
		if ( (gbs = FileOpen (bfile, "w")) == NULL) {
			ErrPostEx(SEV_ERROR,0,0, "Can't open %s", bfile);
			exit (1);
		}
	}
	if (myargs[5].intvalue) {
		cfile = (CharPtr) MemNew(StringLen(infile) + 5);
		StringCpy(cfile, infile);
		StringCat(cfile, ".fna");
		if ( (fsa = FileOpen (cfile, "w")) == NULL) {
			ErrPostEx(SEV_ERROR,0,0, "Can't open %s", cfile);
			exit (1);
		}
	}
	if (myargs[10].intvalue) {   /*Prot table */
		Pfile = (CharPtr) MemNew(StringLen(infile) + 5);
		StringCpy(Pfile, infile);
		StringCat(Pfile, ".ptt");
		if ( (pta = FileOpen (Pfile, "w")) == NULL) {
			ErrPostEx(SEV_ERROR,0,0, "Can't open %s", Pfile);
			exit (1);
		}
	}
	if (myargs[11].intvalue) {  /* proteins FASTA */
		pfile = (CharPtr) MemNew(StringLen(infile) + 5);
		StringCpy(pfile, infile);
		StringCat(pfile, ".faa");
		if ( (paa = FileOpen (pfile, "w")) == NULL) {
			ErrPostEx(SEV_ERROR,0,0, "Can't open %s", pfile);
			exit (1);
		}
	}
	if (myargs[17].intvalue) {  /* CDS nucleotide FASTA */
		nfile = (CharPtr) MemNew(StringLen(infile) + 5);
		StringCpy(nfile, infile);
		StringCat(nfile, ".ffn");
		if ( (cds = FileOpen (nfile, "w")) == NULL) {
			ErrPostEx(SEV_ERROR,0,0, "Can't open %s", nfile);
			exit (1);
		}
	}
	if (myargs[12].intvalue) {  /* assembly table */
		hfile = (CharPtr) MemNew(StringLen(infile) + 5);
		StringCpy(hfile, infile);
		StringCat(hfile, ".tab");
		if ( (tab = FileOpen (hfile, "w")) == NULL) {
			ErrPostEx(SEV_ERROR,0,0, "Can't open %s", hfile);
			exit (1);
		}
	}
	if (myargs[13].intvalue) {  /* gi list */
		gfile = (CharPtr) MemNew(StringLen(infile) + 5);
		StringCpy(gfile, infile);
		StringCat(gfile, ".gis");
		if ( (gil = FileOpen (gfile, "w")) == NULL) {
			ErrPostEx(SEV_ERROR,0,0, "Can't open %s", gfile);
			exit (1);
		}
	}
	if (myargs[14].intvalue) {  /* special table */
		sfile = (CharPtr) MemNew(StringLen(infile) + 5);
		StringCpy(sfile, infile);
		StringCat(sfile, ".stb");
		if ( (stb = FileOpen (sfile, "w")) == NULL) {
			ErrPostEx(SEV_ERROR,0,0, "Can't open %s", sfile);
			exit (1);
		}
	}
	if (myargs[16].intvalue) {  /* DNA pieces fasta  */
		dnafile = (CharPtr) MemNew(StringLen(infile) + 5);
		StringCpy(dnafile, infile);
		StringCat(dnafile, ".dna");
		if ( (dna = FileOpen (dnafile, "w")) == NULL) {
			ErrPostEx(SEV_ERROR,0,0, "Can't open %s", dfile);
			exit (1);
		}
	}
	if (myargs[18].intvalue) {  /* DNA pieces GIs */
		dnafile = (CharPtr) MemNew(StringLen(infile) + 5);
		StringCpy(dnafile, infile);
		StringCat(dnafile, ".GI");
		if ( (gdna = FileOpen (dnafile, "w")) == NULL) {
			ErrPostEx(SEV_ERROR,0,0, "Can't open %s", dfile);
			exit (1);
		}
	}
	ajp = (Asn2ffJobPtr) MemNew(sizeof(Asn2ffJob));
	ajp->show_gene = TRUE;
	ajp->show_seq = TRUE;
	ajp->show_gi = TRUE;
	ajp->show_version = TRUE;
	ajp->error_msgs = TRUE;
	ajp->non_strict = FALSE;
	ajp->null_str = FALSE;
	ajp->format = GENBANK_FMT;
	if (myargs[22].intvalue) {
		ajp->mode = DUMP_MODE;
	} else {
		ajp->mode = RELEASE_MODE;
	}
	bsp = find_big_bioseq(sep);
	ajp->gb_style = TRUE;
	if (myargs[20].intvalue) {
		ajp->gb_style = FALSE;
		ajp->only_one = TRUE;
	}
	if (myargs[21].intvalue) {
		ajp->ignore_top = TRUE;
	} else {
		ajp->ignore_top = FALSE;
	}
	ajp->sep = sep;
	if (myargs[19].intvalue == FALSE) {  /* no sequence */
		ajp->show_seq = FALSE;
	}
	entityID = ObjMgrGetEntityIDForPointer(sep);
	SeqMgrIndexFeatures(0, sep);
      	ajp->useSeqMgrIndexes = TRUE;
	if (entityID == 0)
		ErrPostStr(SEV_WARNING, 0, 0, "Couldn't get entityID");
				ajp->entityID = entityID;
	if (myargs[18].intvalue) {  /* DNA pieces GIs */
		SaveNucGiToFile(gdna, sep);
		FileClose(dna);
	}
		
	if (gbk != NULL) {
		ajp->fp = gbk;
		asn2ff_print(ajp);
		FileClose(gbk);
	}
	if (gbs != NULL) {
		ajp->genome_view = TRUE;
		ajp->fp = gbs;
		asn2ff_print(ajp);
		FileClose(gbs);
	}
	if (fsa != NULL) {
		if (myargs[15].intvalue) {  /* fasta with n's */
			SeqVirtualToFastaFile(sep, fsa);
		} else {
			SeqEntrysToFasta(sep, fsa, TRUE, 1); /* for genomes only */
		}
		FileClose(fsa);
	}
	if (pta != NULL) {
		SeqEntryExplore(sep, (Pointer) (&title), FindTopTitle);
		/*SaveProtTabToFile(pta, sep, 0, 0, title);  */
		SaveProtTabToTabFile(gi, pta, sep, 0, 0, title); 
		FileClose(pta);
	}
	if (paa != NULL) {
	/*	SaveFASTAToFile(paa, sep, 0, 0, FALSE);*/
		SaveFASTAIndex(paa, bsp, 0, 0, FALSE);
		FileClose(paa);
	}
	if (cds != NULL) {
		/*	SaveFASTAToFile(cds, sep,(cd 0, TRUE);*/		
		SaveFASTAIndex(cds, bsp, 0, 0, TRUE);
		FileClose(cds);
	}
	if (gil != NULL) {
		SaveProtGiToFile(gil, sep, gi);
		FileClose(gil);
	}
	if (stb != NULL) {
		SaveSpecialProtTabToFile(stb, sep);
		FileClose(stb);
	}
	if (tab != NULL) {
		SaveAssHistToFile(tab, sep);
		FileClose(tab);
	}
	if (dna != NULL) {
		SaveDNApiecesToFile(dna, sep);
		FileClose(dna);
	}

	if (tarfile && gbk && fsa) {
	/*	if ((aop = AsnIoOpen (dfile, "w")) == NULL) {
			ErrPostEx(SEV_FATAL, 1,0,"Could not open file %s", dfile);
		} else {
			SeqEntryAsnWrite(sep, aop, NULL);
			AsnIoClose(aop);
		}
		if ((aop = AsnIoOpen (efile, "wb")) == NULL) {
			ErrPostEx(SEV_FATAL, 1,0,"Could not open file %s", efile);
		} else {
			SeqEntryAsnWrite(sep, aop, NULL);
			AsnIoClose(aop);
		}*/
		sprintf(command, "tar cvf %s %s.*", tarfile, infile);
		system(command);
		sprintf(command, "compress -v %s", tarfile);
		system(command);
	}
	MemFree(infile0);
	MemFree(afile);
	MemFree(bfile);
	MemFree(cfile);
	MemFree(dfile);
	MemFree(efile);
	MemFree(hfile);
	MemFree(Pfile);
	MemFree(pfile);
	MemFree(nfile);
	if (sfile)
		MemFree(sfile);
	if (gfile)
		MemFree(gfile);
	MemFree(ajp);
	EntrezBioseqFetchDisable ();
	if (aip) 
		AsnIoClose(aip);
}
