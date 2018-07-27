/* This is a sample program to call BandAlign */

#include <ncbi.h>
#include <tofasta.h>
#include <lsqfetch.h>
#include <blast.h>
#include <objalign.h>
#include <salutil.h>
#include <salsap.h>
#include <salstruc.h>
#include <bandalgn.h>
#include <txalign.h>
#include <uputil.h>
#include <salign.h>

/*the length of alignment*/
#define LINE 60	

/*#define TXFORMAT_OPTIONS (TXALIGN_END_NUM+TXALIGN_HTML+TXALIGN_COMPRESS+TXALIGN_MISMATCH) */
#define TXFORMAT_OPTIONS (TXALIGN_COMPRESS) 
#define ALN_FASTA     1
#define ALN_SIM2ALN   2
#define ALN_SIM3ALN   3
#define ALN_CSIM      4
#define ALN_SIM4      5
#define ALN_BANDALIGN 6
#define ALN_BLAST     7
#define ALN_BLASTBAND 8
#define ALN_SPLICING  9

#define MYARGI    0
#define MYARGOD   1 
#define MYARGOS   2 
#define MYARGOF   3 
#define MYARGTRS  4
#define MYARGPROT 5
#define MYARGMET  6
#define MYARGBA   7
#define MYARGMA   8
#define MYARGMS   9
#define MYARGGON   10
#define MYARGGEN   11
#define MYARGGO  12
#define MYARGGE  13

#define MYARGMAT  14
#define MYARGWS   15
#define MYARGGXD  16
#define MYARGGXDF 17
#define MYARGFIL  18
#define MYARGGAPPED 19
#define MYARGDOTS 20
#define MYARGMDIN 21

#define NUMARGS	22
Args myargs[NUMARGS] = {
        {"Input file", NULL, NULL, NULL, TRUE, 'i', ARG_STRING, 0.0,0,NULL},
        {"Output file for Text Alignment(NULL==stdout)", NULL, NULL, NULL, TRUE, 'o', ARG_STRING, 0.0,0,NULL},
        {"Output file for SeqAlign", "blastalign.sat", NULL, NULL, FALSE, 'O', ARG_STRING, 0.0,0,NULL},
  	{"The output format 1=TEXT 2=SeqAlign 3=both 4=FASTA+gap", "3", "1", "4", TRUE, 'f', ARG_INT, 0.0, 0, NULL},
        {"Align translation", "F", NULL, NULL, TRUE, 'T', ARG_BOOLEAN, 0.0, 0, NULL},
	{"Input file contains Proteins", "F", NULL, NULL, TRUE, 'P', ARG_BOOLEAN, 0.0, 0, NULL},
	{"Alignment method 1=No 4=CSIM 5=SIM4 6=BandAlign 7=BLAST 10=BlastBandAlign 9=mRNA2genomic", "10", "1", "10", TRUE, 'A', ARG_INT, 0.0, 0, NULL},
	{"Banded alignment method", "2", "0", "5", TRUE, 't', ARG_INT, 0.0, 0, NULL},

	{"Match reward (nucleotide alignment)", "2", NULL, NULL, TRUE, 'r', ARG_INT, 0.0, 0, NULL},
	{"Mismatch penalty (nucleotide alignment)", "-3", NULL, NULL, TRUE, 'p', ARG_INT, 0.0, 0, NULL},
	{"Gap open penalty(nuc)", "10", NULL, NULL, TRUE, 'G', ARG_INT, 0.0, 0, NULL},
	{"Gap extension penalty(nuc)", "2", NULL, NULL, TRUE, 'E', ARG_INT, 0.0, 0, NULL},
	{"Gap open penalty(aa)", "10", NULL, NULL, TRUE, 'g', ARG_INT, 0.0, 0, NULL},
	{"Gap extension penalty(aa)", "2", NULL, NULL, TRUE, 'e', ARG_INT, 0.0, 0, NULL},
	{"Matrix", "BLOSUM62", NULL, NULL, TRUE, 'M', ARG_STRING, 0.0,0,NULL},

	{"Blast: Word size", "11", NULL, NULL, TRUE, 'w', ARG_INT, 0.0, 0, NULL},
	{"Blast: Gapx dropoff", "50", NULL, NULL, TRUE, 'X', ARG_INT, 0.0, 0, NULL},
	{"Blast: Gapx dropoff final", "50", NULL, NULL, TRUE, 'Z', ARG_INT, 0.0, 0, NULL},
	{"Blast: Filter", "F", NULL, NULL, TRUE, 'F', ARG_BOOLEAN, 0.0, 0, NULL},
	{"Use New Gapped Blast when using blast", "T", NULL, NULL, TRUE, 'N', ARG_BOOLEAN, 0.0, 0, NULL},
	{"Options for display", "4", NULL, NULL, TRUE, 'D', ARG_INT, 0.0, 0, NULL},
	{"Display multi-dimensional alignment", "T", NULL, NULL, TRUE, 'm', ARG_BOOLEAN, 0.0, 0, NULL},

};

static void seqalign_write (SeqAlignPtr salp, CharPtr name)
{
        SeqAnnotPtr annot;
        AsnIoPtr aip;

        annot = SeqAnnotNew();
        if (annot==NULL)
           return;
        annot->type = 2;
        annot->data = salp;

        aip = AsnIoOpen(name, "w");
        if(aip !=NULL)
        {
                        SeqAnnotAsnWrite(annot, aip, NULL);
                        AsnIoClose(aip);
        }
}

static SeqAlignPtr write_output(SeqAlignPtr align, Uint1 output_type, CharPtr sat_name, CharPtr ali_name, Uint4 option)
{
	SeqAnnotPtr annot;
	AsnIoPtr aip;
	FILE *fp;
	Uint1 featureOrder[FEATDEF_ANY];
	Uint1 groupOrder[FEATDEF_ANY];

	if(align == NULL)
		return NULL;
	annot = SeqAnnotNew();
        if (annot==NULL)
           return NULL;
	annot->type = 2;
	annot->data = align;

	if((output_type&1) == 1 ) {
	  if(ali_name==NULL) {
	    fp=stdout;
	  } else {
	    fp = FileOpen(ali_name, "w");
	  }
		if(fp !=NULL)
		{
			fprintf(fp, "\n\n\nALIGNMENT\n\n");
			MemSet((Pointer)(featureOrder), 0, (size_t)(FEATDEF_ANY* sizeof(Uint1)));
			MemSet((Pointer)(groupOrder), 0, (size_t)(FEATDEF_ANY* sizeof(Uint1)));
/**
			featureOrder[FEATDEF_CDS] = 1;
			groupOrder[FEATDEF_CDS] = 1;
**/
			ShowTextAlignFromAnnot(annot, LINE, fp, (Uint1Ptr)(&featureOrder), (Uint1Ptr)(&groupOrder), option, NULL, NULL, NULL);
			if (fp!=stdout) FileClose(fp);
		}
		else
			Message(MSG_ERROR, "Fail to write permission for %s", ali_name);
	}
	if((output_type &2)== 2 )
	{
		aip = AsnIoOpen(sat_name, "w");
		if(aip !=NULL)
		{
			SeqAnnotAsnWrite(annot, aip, NULL);
			AsnIoClose(aip);
		}
		else
			Message(MSG_ERROR, "Fail to write permission for %s", ali_name);
	}
        annot->data = NULL;
	return align;
}

static void FindNuc(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
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

Int2 Main(void)
{
  FILE        *ifp, 
              *efp;
  Char        file_name[20];                   
  CharPtr     out_name=NULL,ali_name=NULL;
  CharPtr     path;
  SeqAlignPtr align = NULL;
  SeqEntryPtr sep = NULL,
              septmp = NULL, 
              presep = NULL;
  BioseqSetPtr bssp;
  ValNodePtr  sqlocs=NULL;
  MashPtr     msh;
  BLAST_ScoreBlkPtr sbp;
  Int4        k;
  Int2        nseq,
              i, 
              j = 0;
  Int2        method;
  Uint1       output_type,
              mol_type;
  Uint4       option = TXALIGN_MISMATCH; 
  Boolean     rpt_err;
  Boolean     use_entrez = TRUE;
  Boolean     is_prot;


  if(!GetArgs("blastalign", NUMARGS, myargs)) {
     return 1;
  }
  if (! SeqEntryLoad())  return 1;

  BioseqFetchInit(TRUE);
  if(use_entrez)
     EntrezBioseqFetchEnable ("Blastalign", TRUE);

  /***** OPEN **********/
  is_prot = (Boolean) myargs[MYARGPROT].intvalue;

/***READING FASTA file **/
  if(myargs[MYARGI].strvalue ==NULL) {
    ifp=stdin;
  } else {
    StringMove(file_name, myargs[MYARGI].strvalue);
    if((ifp = FileOpen(file_name, "r")) == NULL)
      {
	fprintf(stderr,"Fail to open file %s\n", file_name);
	return 1;
      }
  }
  while ((septmp = FastaToSeqEntry (ifp, (Boolean)(is_prot==FALSE))) != NULL)
  {
     if (j == 0) sep = septmp;
     else  presep->next = septmp;
     presep = septmp;
     j++;
  }
  FileClose (ifp);
  if (sep == NULL) {
     fprintf(stderr,"No sequences read.\n");
     return 1;
  }
  septmp = NULL;

  if ( (bssp=BioseqSetNew()) != NULL ) {
     bssp->_class = 14;
     bssp->seq_set = sep;
     septmp = SeqEntryNew ();
     if ( septmp  != NULL ) {
        septmp->choice = 2;
        septmp->data.ptrvalue = (Pointer) bssp;
        sep = septmp;
     }
  }
  if (septmp == NULL) {
     fprintf(stderr,"No sequences read.\n");
     return 1;
  }
/* FASTA file ***/
  /***** OUPUT **********/
    
  output_type = myargs[MYARGOF].intvalue;
  if ((output_type&1)==1) {
    if(myargs[MYARGOD].strvalue!=NULL) {
      ali_name=Malloc(StringLen(myargs[MYARGOD].strvalue)+1);
      sprintf(ali_name, "%s", myargs[MYARGOD].strvalue);
    } else {
      ali_name=NULL;
    }
  }

  if ((output_type&2)==2) {
    if(myargs[MYARGOS].strvalue!=NULL) {
      out_name=Malloc(StringLen(myargs[MYARGOS].strvalue)+1);
      sprintf(out_name, "%s", myargs[MYARGOS].strvalue);
    } else {
      out_name=(CharPtr) Malloc(50);
      sprintf(out_name, "blastalign.sat");
    }
  }
  /***** PARAMETERS **********/

  method = (Int2) myargs[MYARGMET].intvalue;
  if (method < 1 && method > ALN_BLAST) method = ALN_BLAST;

  msh = MashNew (is_prot);
  msh->band_method = (Int2) myargs[MYARGBA].intvalue;
  msh->reward = (Int2) myargs[MYARGMA].intvalue;
  msh->penalty = (Int2) myargs[MYARGMS].intvalue;

  if(msh->penalty > 0) {
     fprintf(stderr,"The mismatch weight should be negative.\n");
     return 1;
  }
  if(is_prot ||(Boolean) myargs[MYARGTRS].intvalue ) {
    msh->gap_open = (Int4) 11; 
    msh->gap_extend = (Int4) 1;
  } else {
    msh->gap_open = (Int4) myargs[MYARGGON].intvalue;
    msh->gap_extend = (Int4) myargs[MYARGGEN].intvalue;
  }

  if(msh->gap_open < 0) {
     fprintf(stderr,"The gap-open penalty should be positive.\n");
     msh->gap_open = 0;
  }
  if(msh->gap_extend < 0) {
     fprintf(stderr,"The gap-extend penalty should be positive.\n");
     msh->gap_extend = 0;
  }
  if (!is_prot) {
     msh->wordsize = (Int4) myargs[MYARGWS].intvalue;
     msh->gap_x_dropoff = (Int4) myargs[MYARGGXD].intvalue;
     msh->gap_x_dropoff_final = (Int4) myargs[MYARGGXDF].intvalue;
  }
  msh->is_prot = is_prot;
  msh->multidim = (Boolean) myargs[MYARGMDIN].intvalue;
  msh->splicing = TRUE;    /*** FALSE; **/
  if (method == ALN_SPLICING) 
  msh->splicing = TRUE;
  msh->map_align = FALSE;   /****************TRUE;**************/

  if(is_prot) {
    msh->translate_prot = (Boolean)FALSE;
  } else {
    msh->translate_prot = (Boolean)myargs[MYARGTRS].intvalue;
  }
  if((Boolean)myargs[MYARGFIL].intvalue) {
    if(msh->translate_prot || msh->is_prot) {
      msh->filter = FILTER_SEG;
    } else {
      msh->filter = FILTER_DUST;
    }
  } else {
    msh->filter= FILTER_NONE;
  }
  if(myargs[MYARGMAT].strvalue!=NULL) {
    msh->matrixname=Malloc(StringLen(myargs[MYARGMAT].strvalue)+1);
    StringCpy(msh->matrixname,myargs[MYARGMAT].strvalue);
  } else {
    msh->matrixname=Malloc(StringLen(myargs[MYARGMAT].strvalue)+1);
    StringCpy(msh->matrixname,"BLOSUM62");
  }

  msh->use_gapped_blast = (Boolean) myargs[MYARGGAPPED].intvalue;

  /***** RUN **********/
  if (is_prot)
     mol_type = Seq_mol_aa;
  else
     mol_type = Seq_mol_na; 
/**/
  sqlocs = SeqEntryToSeqLoc (sep,  &nseq, mol_type);
/***/
/**
  sqlocs = read_gifile ("uid");
  sqlocs = gilst2seqloclst (sqlocs);
**/
  align = SeqLocListToSeqAlign (sqlocs, (Int2)method, (Pointer)msh);

  if (align!=NULL) {
    if (output_type <4) {
       option = myargs[MYARGDOTS].intvalue;
       write_output (align, output_type, out_name, ali_name, option);
       write_output (msh->transalp, output_type, out_name, ali_name, option);
    }
    else if (output_type == 4) {
       showfastagap_fromalign (align, (Int4)LINE, stdout);
       showfastagap_fromalign (msh->transalp, (Int4)LINE, stdout);
    }
    else fprintf (stderr, "Wrong output format\n");
  }
  else {
     fprintf (stderr, "No alignment\n");
  }
  align=SeqAlignFree(align);
  BioseqFetchDisable();
  if(use_entrez) EntrezBioseqFetchDisable();
  if (myargs[MYARGMAT].strvalue!=NULL) Free(msh->matrixname);
  if(ali_name!=NULL) Free(ali_name);
  if(out_name!=NULL) Free(out_name);
  return 0;
}
