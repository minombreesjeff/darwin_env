 /*****************************************************************************
*
*   asn2ff.c
*   	convert an ASN.1 entry to flat file format, using the FFPrintArrayPtrs. 
*
*****************************************************************************/
#include <accentr.h>
#include "asn2ff.h"
#include "asn2ffp.h"
#include "ffprint.h"
#include <subutil.h>
#include <objall.h>
#include <objcode.h>
#include <lsqfetch.h>
#include <explore.h>

#ifdef ENABLE_ID1
#include <accid1.h>
#endif

FILE *fpl;

Args myargs[] = {
	{"Filename for asn.1 input","stdin",NULL,NULL,TRUE,'a',ARG_FILE_IN,0.0,0,NULL},
	{"Input is a Seq-entry","F", NULL ,NULL ,TRUE,'e',ARG_BOOLEAN,0.0,0,NULL},
	{"Input asnfile in binary mode","F",NULL,NULL,TRUE,'b',ARG_BOOLEAN,0.0,0,NULL},
	{"Output Filename","stdout", NULL,NULL,TRUE,'o',ARG_FILE_OUT,0.0,0,NULL},
	{"Show Sequence?","T", NULL ,NULL ,TRUE,'h',ARG_BOOLEAN,0.0,0,NULL},
	{"Log errors to file named:",NULL,NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL},
	{"Output Format?: b for GenBank, p for GenPept, e for EMBL, s for PseudoEMBL, x for   GenBankSelect, z for EMBLPEPT",
	"b", NULL,NULL,TRUE,'f',ARG_STRING,0.0,0,NULL},
	{"Output mode?: r for release, d for dump, s for Sequin, c for Chromoscope, k for dir-sub-debug, l for dir-sub, e for revise, p for partial report","r", NULL ,NULL ,TRUE,'m',ARG_STRING,0.0,0,NULL},
	{"Input is a Seq-submit","F", NULL ,NULL ,TRUE,'s',ARG_BOOLEAN,0.0,0,NULL},
	{"Show gi numbers?","F", NULL ,NULL ,TRUE,'g',ARG_BOOLEAN,0.0,0,NULL},
	{"Non-Strict gene_binding","T", NULL ,NULL ,TRUE,'n',ARG_BOOLEAN,0.0,0,NULL},
	{"Show error messages","T", NULL ,NULL ,TRUE,'v',ARG_BOOLEAN,0.0,0,NULL},
	{"Show verbose message text","F", NULL ,NULL ,TRUE,'t',ARG_BOOLEAN,0.0,0,NULL},
	{"Use HTML output format?","F", NULL,NULL,TRUE,'w',ARG_BOOLEAN,0.0,0,NULL},
	{"Output is one top bioseq only","F", NULL ,NULL ,TRUE,'q',
	ARG_BOOLEAN,0.0,0,NULL},
	{"Output is one top bioseq only in genome view","F", NULL ,NULL ,TRUE,'G',
	ARG_BOOLEAN,0.0,0,NULL},
	{"Output is map bioseqs only ","F", NULL ,NULL ,TRUE,'M',
	ARG_BOOLEAN,0.0,0,NULL},
	{"Output error logfile","stderr", NULL,NULL,TRUE,'r',ARG_FILE_OUT,0.0,0,NULL},
	{"Show new gene features?","T",NULL,NULL,TRUE,'p',ARG_BOOLEAN,0.0,0,NULL},
	{"New algorithm for orgnames?",
	"F",NULL,NULL,TRUE,'z',ARG_BOOLEAN,0.0,0,NULL},
	{"Print help format only?",
	"F",NULL,NULL,TRUE,'y',ARG_BOOLEAN,0.0,0,NULL},
	{"From to show a region", "0", NULL, NULL, TRUE, 'A', ARG_FLOAT, 0.0, 0, NULL},
	{"To to show a region", "0", NULL, NULL, TRUE, 'B', ARG_FLOAT, 0.0, 0, NULL},
	{"Complex sets (phy-set,mut-set, pop-set)?",
	"T",NULL,NULL,TRUE,'k',ARG_BOOLEAN,0.0,0,NULL},
	{"Use SeqMgr indexing?","F",NULL,NULL,TRUE,'d',ARG_BOOLEAN,0.0,0,NULL},
	{"Use VERSION?","T",NULL,NULL,TRUE,'V',ARG_BOOLEAN,0.0,0,NULL},
	{"Show Bankit comments?","F",NULL,NULL,TRUE,'C',ARG_BOOLEAN,0.0,0,NULL},
	{"For GenBank Release?","F",NULL,NULL,TRUE,'R',ARG_BOOLEAN,0.0,0,NULL},
	{"New LOCUS line format?","T",NULL,NULL,TRUE,'L',ARG_BOOLEAN,0.0,0,NULL}
	};


static MsgAnswer LIBCALLBACK myHook (MsgKey key, ErrSev sev, const char *caption, const char *message)
{
	fprintf(fpl, "%s\n", message);
	return ANS_OK;
}

/*static void FindNuc(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
    BioseqPtr PNTR bp;
    BioseqPtr local_bsp;
 
    bp = (BioseqPtr PNTR) data;
    if (IS_Bioseq(sep))
    {
        local_bsp = (BioseqPtr) sep->data.ptrvalue;
        if (ISA_na(local_bsp->mol))
          *bp = local_bsp;
    }
}
*/
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

Int2 Main(void)
{

	AsnIoPtr aip;
	AsnTypePtr atp = NULL, atp1, atp2;
	AsnModulePtr amp;
	Boolean error_msgs=TRUE, show_gi= TRUE, show_seq = TRUE, non_strict=TRUE;
	SeqEntryPtr the_set;
	SeqSubmitPtr ssp;
	StdPrintOptionsPtr Spop = NULL;
	FILE *fp;
	Uint1 format, mode;
	Boolean good = FALSE;
	Asn2ffJobPtr		ajp;
	Uint2 entityID;
	Int4 num, total;
	SeqLocPtr slp;
	SeqIntPtr sip;
	BioseqPtr bsp;
	Boolean useSeqMgrIndexes;
	/*
	ValNode v;
	LinkStrPtr lsp;
	*/

	if ( ! GetArgs("asn2ff", sizeof(myargs)/sizeof(Args), myargs))
		return 1;
	ErrSetMessageLevel(SEV_NONE);
	ErrSetOptFlags(EO_SHOW_CODES);
	ErrSetOptFlags(EO_XLATE_CODES);
	if (myargs[12].intvalue)   /* show the verbose error messages? */
		ErrSetOptFlags(EO_MSG_MSGTEXT);

	if (myargs[13].intvalue)   /* use HTML format for output? */
		init_www();
	if (! SeqEntryLoad())
		ErrShow();
	
	if (myargs[8].intvalue) {
		if (! SubmitAsnLoad())
			Message(MSG_FATAL, "Unable to load parse trees.");
		
		atp1 = AsnFind("Seq-submit");
		if (atp1 == NULL)
			Message(MSG_FATAL, "Unable to find Seq-submit");
		atp = AsnFind("Seq-submit");
		if (atp == NULL)
			Message(MSG_FATAL, "Unable to find Seq-submit");

	} else {
		atp = AsnFind("Bioseq-set"); /* get the initial type pointers */
		if (atp == NULL)
			ErrShow();
	
		atp2 = AsnFind("Bioseq-set.seq-set.E");
		if (atp2 == NULL)
			ErrShow();
	}

			/* open the i/o files in the right mode */

	if ((aip = 
		AsnIoOpen (myargs[0].strvalue, myargs[2].intvalue?"rb":"r")) == NULL)
		exit (1);

	if ( (fp = FileOpen (myargs[3].strvalue, "w")) == NULL) {
		ErrPostEx(SEV_ERROR,0,0, "Can't open %s", myargs[3].strvalue);
		exit (1);
	}
	if ( (fpl = FileOpen (myargs[17].strvalue, "w")) == NULL) {
		ErrPostEx(SEV_ERROR,0,0, "Can't open %s", myargs[17].strvalue);
		exit (1);
	}

	SetMessageHook(myHook);

    if (myargs[5].strvalue != NULL) { 			/* log errors instead of die */
        if (! ErrSetLog (myargs[5].strvalue))
            ErrShow();
        else
            ErrSetOpts (ERR_TEE, ERR_LOG_ON);
    }

	if (! myargs[4].intvalue)   /* show the sequence */
		show_seq = FALSE;

	if (! myargs[9].intvalue)   /* show the gi numbers? */
		show_gi = FALSE;

	if (!myargs[10].intvalue)   /* Non-Strict binding of genes to feats */
		non_strict = FALSE;

	if (! myargs[11].intvalue)   /* Show validator error messages */
		error_msgs = FALSE;

	format = GENBANK_FMT;
	if (StringICmp(myargs[6].strvalue, "b") == 0) {
		format = GENBANK_FMT;
	} else if (StringICmp(myargs[6].strvalue, "p") == 0) {
		format = GENPEPT_FMT;
	} else if (StringICmp(myargs[6].strvalue, "e") == 0) {
		format = EMBL_FMT;
	} else if (StringICmp(myargs[6].strvalue, "s") == 0) {
		format = PSEUDOEMBL_FMT;
	} else if (StringICmp(myargs[6].strvalue, "x") == 0) {
		format = SELECT_FMT;
	} else if (StringICmp(myargs[6].strvalue, "z") == 0) {
		format = EMBLPEPT_FMT;
	}
	if (format == GENPEPT_FMT) {
		if (!PrintTemplateSetLoad ("asn2ff.prt")) {
			ErrPostEx(SEV_WARNING, 1, 1, "PrintTemplateSetLoad failed");
		}
		if ((Spop = StdPrintOptionsNew(NULL)) != NULL) {
			Spop->newline = "~";
			Spop->indent = "";
		} else {
			ErrPostEx (SEV_FATAL, 1, 1, "StdPrintOptionsNew failed");
		}
	}

	mode = RELEASE_MODE;
	if (StringICmp(myargs[7].strvalue, "r") == 0)
		mode = RELEASE_MODE;
	if (StringICmp(myargs[7].strvalue, "l") == 0)
		mode = DIRSUB_MODE;
	if (StringICmp(myargs[7].strvalue, "k") == 0)
		mode = DIRSUB_DEBUG_MODE;
	if (StringICmp(myargs[7].strvalue, "e") == 0)
		mode = REVISE_MODE;
	if (StringICmp(myargs[7].strvalue, "d") == 0)
		mode = DUMP_MODE;
	if (StringICmp(myargs[7].strvalue, "s") == 0)
		mode = SEQUIN_MODE;
	if (StringICmp(myargs[7].strvalue, "c") == 0)
		mode = CHROMO_MODE;
	if (StringICmp(myargs[7].strvalue, "p") == 0)
		mode = PARTIAL_MODE;

#ifdef ENABLE_ENTREZ
	EntrezBioseqFetchEnable ("asn2ff", FALSE); 
#endif
#ifdef ENABLE_ID1
	ID1BioseqFetchEnable ("asn2ff", FALSE); 
#endif
#ifdef ENABLE_LOCAL
	BioseqFetchInit(FALSE);
#endif
	ajp = (Asn2ffJobPtr) MemNew(sizeof(Asn2ffJob));
	ajp->show_gene = myargs[18].intvalue;
	ajp->show_seq = show_seq;
	ajp->show_gi = show_gi;
	ajp->error_msgs = error_msgs;
	ajp->non_strict = non_strict;
	ajp->null_str = FALSE;
	ajp->format = format;
	ajp->mode = mode;
	ajp->fp = fp;
	ajp->Spop = Spop;
	ajp->gb_style = TRUE;  /* show only non_right_truncated features */
	
	if (myargs[25].intvalue) {
		ajp->show_version = TRUE;
	}
	if (myargs[14].intvalue) {
		ajp->gb_style = FALSE;
		ajp->only_one = TRUE;
		ajp->ignore_top = FALSE;
	}
	if (myargs[15].intvalue) {
		ajp->ignore_top = TRUE;
		ajp->genome_view = TRUE;
	}
	if (myargs[16].intvalue) {
		ajp->map_view = TRUE;
	}
	if (myargs[19].intvalue) {
		ajp->orgname = TRUE;
	}
	if (myargs[20].intvalue) {
		ajp->help = TRUE;
	}
	if (myargs[26].intvalue) {
		ajp->bankit = TRUE;
	}
	if (myargs[27].intvalue) {
		ajp->forgbrel = TRUE;
	}
	if (myargs[28].intvalue) {
		ajp->old_locus_fmt = FALSE;
	} else {
		ajp->old_locus_fmt = TRUE;
	}
	useSeqMgrIndexes = (Boolean)(myargs[24].intvalue);
	/* get pointer to all loaded ASN.1 modules */
	amp = AsnAllModPtr();
	if (amp == NULL)
		ErrShow();

	total = 0;
	num = 0;
	if (myargs[22].floatvalue) {
		if (myargs[1].intvalue) {
			the_set = SeqEntryAsnRead(aip, NULL);
			bsp = NULL;
			SeqEntryExplore(the_set, &bsp, FindNuc);
			if (bsp == NULL) {
				ErrPostEx(SEV_WARNING, 1, 1, "Couldn't find valid bioseq\n");
				SeqEntryFree(the_set);
				exit (1);
			}
			num = 1;			
			slp = ValNodeNew(NULL);
			sip = SeqIntNew();
			slp->choice = SEQLOC_INT;
			slp->data.ptrvalue = sip;
			sip->from = 0;
			if (myargs[21].floatvalue > 0) {
				sip->from = myargs[21].floatvalue-1;
			}
			if (myargs[22].floatvalue > bsp->length) {
				sip->to = bsp->length-1;
			} else {
				sip->to = myargs[22].floatvalue-1;
			}
			sip->id = (SeqIdPtr) SeqIdDup (SeqIdFindBest (bsp->id, 0));
			IndexASeqEntry (the_set, useSeqMgrIndexes);
			SeqLocToFlat(slp, fp, format, ajp->mode);
		}
	} else if (myargs[8].intvalue) {
		if (mode == DUMP_MODE) {
		    aip->scan_for_start = TRUE;  /* scan past any garbage */
		}
		while ((atp = AsnReadId(aip, amp, atp)) != NULL) {
			if (atp == atp1) {	
				ssp = SeqSubmitAsnRead(aip, atp);
				if (ssp->datatype == 1) {
					IndexASeqEntry ((SeqEntryPtr) ssp->data, useSeqMgrIndexes);
	 		if (SeqEntryToFlatAjp (ajp, (SeqEntryPtr) ssp->data, fp, ajp->format, ajp->mode)) {
				num++;
			}
				/*	if ((SeqSubmitToFlat(ssp, fp, mode, FALSE, format, 
							myargs[18].intvalue)) == TRUE)
					{
						num++;
					}*/
				} 
				SeqSubmitFree(ssp);
				
			} else {
				AsnReadVal(aip, atp, NULL);
			} 
		}
	} else if (myargs[1].intvalue) {
		the_set = SeqEntryAsnRead(aip, NULL);
		total++;
/*********TEST*******
	v.choice = SEQID_GI;
	v.data.intvalue = 455854;
	SeqEntryToFlatEx (the_set, fp, ajp->format, ajp->mode, &v, 0);
			if (mode == PARTIAL_MODE) {
			SeqEntryToPartRpt(the_set, stdout);
	for (lsp=SeqEntryToStrArrayEx(the_set,  ajp->format, 5866992, TRUE); lsp;
		lsp=lsp->next) {
		printf ("%s", lsp->line);
	}
	exit (0);
 *********TEST*******/
   		if (myargs[23].intvalue) { /* complex sets */
			IndexASeqEntry (the_set, useSeqMgrIndexes);
	 		if (SeqEntryToFlatAjp (ajp, the_set, fp, ajp->format, ajp->mode)) {
				num++;
			}
		} else {
			if ((entityID = ObjMgrGetEntityIDForPointer(the_set)) == 0) {
				ErrPostStr(SEV_WARNING, 0, 0, "Couldn't get entityID");
			}
			ajp->entityID = entityID;
			ajp->sep = the_set;
			IndexASeqEntry (the_set, useSeqMgrIndexes);
			if (asn2ff_print(ajp)) {
				num++;
			}
		}
		SeqEntryFree(the_set);
	} else {
		while ((atp = AsnReadId(aip, amp, atp)) != NULL) {
			if (atp == atp2) {  /* top level Seq-entry */
				the_set = SeqEntryAsnRead(aip, atp);
				total++;
				if (myargs[23].intvalue) { /* complex sets */
					IndexASeqEntry (the_set, useSeqMgrIndexes);
	 				if (SeqEntryToFlatAjp (ajp, the_set, fp, ajp->format, ajp->mode))
						num++;
	 			} else {
					if ((entityID=ObjMgrGetEntityIDForPointer(the_set)) == 0) {
						ErrPostStr(SEV_WARNING, 0, 0, "Couldn't get entityID");
					}
					ajp->entityID = entityID;
					ajp->sep = the_set;
					IndexASeqEntry (the_set, useSeqMgrIndexes);
					if (asn2ff_print(ajp))
						num++;
				}
				SeqEntryFree(the_set);
			} else {
				AsnReadVal(aip, atp, NULL);
			}
		}
	}

#ifdef ENABLE_ENTREZ
	EntrezBioseqFetchDisable (); 
#endif
#ifdef ENABLE_LOCAL
	BioseqFetchDisable();
#endif
	if (num == 0) {
		ErrPostStr(SEV_WARNING, 1, 1, "No valid entries found");
	} else if (num < total) {
		ErrPostEx(SEV_WARNING, 1, 1, "[%ld] entries have been processed [total - %ld]\n", num, total);
	}
	if (format == GENPEPT_FMT && Spop != NULL) {
		Spop = StdPrintOptionsFree(Spop);
	}
	MemFree(ajp);
	AsnIoClose(aip);
	FileClose(fp);
	return(0);
}
