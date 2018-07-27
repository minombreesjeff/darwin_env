/*   cdscan.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  cdscan.c
*
* Author:  Kans, Schuler, Ostell
*
* Version Creation Date:   2/26/95
*
* $Revision: 6.1 $
*
* File Description: 
*       scans through sequence records on the Entrez discs
*       This program is meant to serve as a model for programs that scan
*       all the files on the Entrez CDROM disks. Basically it does some
*       setup in the main routine to find the disks and get a list of files
*       on them for nucleic acid or protein sequences. It then goes through
*       each file and reads each Seq-entry from them. On the Entrez CDROMs
*       the Seq-entries are Huffman compressed, so the Casn calls decompress
*       them for you. Once you have a Seq-entry, you can do whatever you
*       want with it. In this program we give the options of printing as
*       FASTA file or as GenBank or GenPept file. You can modify this part
*       to do whatever you want. The function that receives the SeqEntry is
*       called "ProcessSeqEntry".
*
*   This function is passed one SeqEntry at a time by the CDROM scanning
*     Routines. Depending on the global variables it will call routines to
*     make fasta, genbank format, or to call a custom routine. The default
*     custom routine (CustomRoutine) just prints the SeqIds of the sequences
*     to the outputfile and to the progress monitor.
*
*   At the end of the file are some custom routines written for various
*     purposes that could be substituted or modified for other purposes.
*     To activate, call them instead of "CustomRoutine"
*
*      The first locates all GenBank entries and prints out a short summary
*      of their citations. It is called GenBankPubs()
*
*      The second prints the sequence of all CdRegion features in the entry.
*      It is called SeqEntryToFeat()
*
*
*       Before trying any of this, be sure you have installed the Entrez
*       application itself and ensured that it works. This program uses the
*       the same configuration file and will not run if Entrez has not been
*       properly installed.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#ifndef _NEW_CdEntrez_
#define _NEW_CdEntrez_
#endif

#include <ncbi.h>
#include <casn.h>
#include <accentr.h>
#include <cdromlib.h>
#include <seqport.h>
#include <asn2ff.h>
#include <tofasta.h>

/*****************************************************************************
*
*   structs used by main routines
*
*****************************************************************************/

typedef struct filelist {
  Int2                  cdnum;
  CharPtr               fdir;
  CharPtr               fname;
  struct filelist PNTR  next;
} FileList, PNTR FileListPtr;

/*****************************************************************************
*
*   Function prototypes for routines in this module
*
*****************************************************************************/
static Boolean LIBCALLBACK EnumerateFiles PROTO((int cdnum, const char *fdir,
                                           const char *fname, long fsize,
                                           void *opaque_data));

static FileListPtr FileListNew PROTO((FileListPtr flp, Int2 cdnum,
                                CharPtr fdir, CharPtr fname));

static void ProcessFileList PROTO((FileListPtr flp, CharPtr outputfile));

static void ProcessFile PROTO((FileListPtr flp, CharPtr root, CharPtr outputfile));

static void ProcessSeqEntry PROTO((SeqEntryPtr sep, FILE *fp));

static void CustomRoutine PROTO((SeqEntryPtr sep, FILE * fp));

static void PrintIdDefLine PROTO((SeqEntryPtr sep, Pointer data,
                         Int4 index, Int2 indent));

/*****************************************************************************
*
*   Static Data used by the main routines
*
*****************************************************************************/

static Char root [PATH_MAX];
static EntrezInfoPtr eip;
static EntrezDivInfo *div_info;

static Int2 format;    /* 1 = GenBank, 2 = FASTA */
static Boolean is_na,  /* TRUE = nucleic acids, FALSE = proteins */
	is_custom;  /* call custom process instead of std ones */
static MonitorPtr pmon = NULL;   /* progress monitor */

#define NUMARGS 5

Args myargs [NUMARGS] = {
  {"Scan DNA (1) or Protein (2)", "1", "1", "2", FALSE, 's', ARG_INT, 0.0, 0, NULL},
  {"Output format: GenBank (1) or FASTA (2)", "1", "1", "2", TRUE, 'f', ARG_INT, 0.0, 0, NULL},
  {"Call custom process", "F", NULL, NULL, TRUE, 'c', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Show progress monitor", "F", NULL, NULL, TRUE, 'p', ARG_BOOLEAN, 0.0, 0, NULL},
  {"Output File", "stdout", NULL, NULL, FALSE, 'o', ARG_FILE_OUT, 0.0, 0, NULL}
};

/*****************************************************************************
*
*   This is the main program.
*     It reads command line or initial dialogue arguments
*     It initializes parse trees, entrez
*     It makes a list of the relevant files to search
*     It then calls ProcessFileList to open each file and process it
*
*****************************************************************************/
Int2 Main (void)
{
  Char         div [8];
  FileListPtr  flp = NULL;
  FILE         *fp;
  Int2         i;
  Boolean      is_network, did_init=FALSE;
  FileListPtr  next;
  CharPtr      outputfile;

  if (! GetArgs ("CdScan", NUMARGS, myargs))   /* get input args */
	goto ret;

											   /* process input args */
  if (myargs[0].intvalue == 1)
  	is_na = TRUE;      /* scan nucleic acids */
  else
  	is_na = FALSE;     /* scan proteins */
  format = (Int2)(myargs[1].intvalue);
  is_custom = (Boolean)(myargs[2].intvalue);
  if (myargs[3].intvalue)   /* show progress */
  {
  	pmon = MonitorStrNew("CdScan", 40);
  }
  outputfile = myargs[4].strvalue;   /* output file name */

  if (pmon != NULL)
  	MonitorStrValue(pmon, "Reading Parse Trees");

  if (! SeqEntryLoad() || ! SubmitAsnLoad())   /* read ASN.1 parse trees */
  {
  	Message(MSG_ERROR, "Can't open parse trees");
  	goto ret;
  }

  if (! PrintTemplateSetLoad ("asn2ff.prt"))
  {
    Message(MSG_ERROR, "Can't load print templates");
	goto ret;
  }

  if (pmon != NULL)
  	MonitorStrValue(pmon, "Initializing Entrez");

  if (! EntrezInit ("cdscan", FALSE, &is_network)) /* init Entrez */
  {
  	Message(MSG_ERROR, "Can't initialize Entrez");
  	goto ret;
  }

  did_init = TRUE;
  if (is_network)
  {
      Message (MSG_ERROR, "Network service does not allow scanning");
  	goto ret;
  }

  if (pmon != NULL)
  	MonitorStrValue(pmon, "Building File List");

  eip = EntrezGetInfo ();                        /* set up the file lists */
  if ((eip == NULL) || (eip->div_info == NULL))
  {
  	Message(MSG_ERROR, "Can't find Entrez file info");
  	goto ret;
  }


  flp = FileListNew (NULL, INT2_MIN, NULL, NULL);
  if (flp == NULL)
  {
  	Message(MSG_ERROR, "Can't allocate file list");
  	goto ret;
  }

  div_info = eip->div_info;
  for (i = 0; i < eip->div_count; i++)
  {
  	StringNCpy (div, div_info [i].tag, sizeof (div) - 1);
  	if (! is_na)
  	{
      	CdEnumFiles (CdDir_rec, TYP_AA, div, EnumerateFiles, &flp);
      }
  	else
  	{
      	CdEnumFiles (CdDir_rec, TYP_NT, div, EnumerateFiles, &flp);
  	}
  }

  fp = FileOpen (outputfile, "w");   /* test that we can open output file */
  if (fp == NULL)
  {
  	Message(MSG_ERROR, "Can't open [%s]", outputfile);
  	goto ret;
  }

  FileClose (fp);     /* will be reopened for each input file */

  ProcessFileList (flp, outputfile);              /* process the file list */

ret:                                                         /* clean up */

  if (pmon != NULL)     /* close the progress monitor */
	MonitorFree(pmon);

  if (did_init)
	  EntrezFini();         /* close entrez */

  while (flp != NULL)       /* free file list */
  {
    next = flp->next;
    MemFree (flp->fdir);
    MemFree (flp->fname);
    MemFree (flp);
    flp = next;
  }

  return 0;
}

/*****************************************************************************
*
*   ProcessSeqEntry (sep, fp)
*
*
*   This function is passed one SeqEntry at a time by the CDROM scanning
*     Routines. Depending on the global variables it will call routines to
*     make fasta, genbank format, or to call a custom routine. The default
*     custom routine (CustomRoutine) just prints the SeqIds of the sequences
*     to the outputfile and to the progress monitor.
*
*   At the end of the file are some custom routines written for various
*     purposes that could be substituted or modified for other purposes.
*     To active, call them instead of "CustomRoutine"
*
*****************************************************************************/
static void ProcessSeqEntry (SeqEntryPtr sep, FILE *fp)

{
  Uint1 fmt;

  if ((sep == NULL) || (fp == NULL))
	return;

  if (is_custom)
	CustomRoutine(sep, fp);    /* this is the one you modify */
  else if (format == 1)   /* genbank format */
  {
	if (is_na)				  /* defined in asn2ff.h */
		fmt = GENBANK_FMT;
	else
		fmt = GENPEPT_FMT;
	SeqEntryToFlat(sep, fp, fmt, RELEASE_MODE);   /* dump like entrez does */
  }  
  else if (format == 2)   /* fasta format */
	SeqEntryToFasta(sep, fp, is_na);   /* defined in tofasta.h */

  return;
}

/*****************************************************************************
*
*   CustomRoutine (sep, fp)
*     This is just a little model of a customized routine
*     Normally you would replace this with one of your own design
*     Some examples follow below. In this routine, it prints the
*     SeqId and definition line of each entry it finds using SeqEntryExplore.
*
*****************************************************************************/
static void CustomRoutine (SeqEntryPtr sep, FILE * fp)
{

	SeqEntryExplore(sep, (Pointer)(fp), PrintIdDefLine);
}


/*****************************************************************************
*
*   PrintIdDefLine
*     SeqEntryExplore callback routine that prints the seqids and definition
*       lines.
*
*****************************************************************************/
static void PrintIdDefLine (SeqEntryPtr sep, Pointer data,
                         Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  FILE * fp;
  Char buf[40];
  CharPtr title = NULL;

  if (IS_Bioseq (sep)) {
	*buf = '\0';
    bsp = (BioseqPtr) sep->data.ptrvalue;
    fp = (FILE *) data;
	title = BioseqGetTitle(bsp);   /* this does not deal with all cases */
	SeqIdPrint(bsp->id, buf, PRINTID_FASTA_LONG);  /* print SeqId */
	if (pmon != NULL)
		MonitorStrValue(pmon, buf);
	fprintf(fp, ">%s", buf);
	if (title != NULL)
		fprintf(fp, " %s", title);
	fprintf(fp, "\n");
  }
  return;
}


/*****************************************************************************
*
*   Other possible Custom routines follow below.
*      The first locates all GenBank entries and prints out a short summary
*      of their citations. It is called GenBankPubs
*
*      The second prints the sequence of all CdRegion features in the entry.
*      It is called SeqEntryToFeat()
*
*****************************************************************************/


/*****************************************************************************
*
*   GenBankPubs(sep, fp)
*     This set of routines finds GenBank entries and prints a summary of
*     their citations
*
*****************************************************************************/
static void GetPubFromGenBank PROTO((SeqEntryPtr sep, Pointer data,
                         Int4 index, Int2 indent));

static void GenBankPubs (SeqEntryPtr sep, FILE * fp)
{

	SeqEntryExplore(sep, (Pointer)(fp), GetPubFromGenBank);
}


static void PubWrite(CharPtr accession, ValNodePtr vnp, FILE * fp)
{
	CitArtPtr cap = NULL;
	CitJourPtr cjp;
	ValNodePtr tvnp;
	ImprintPtr ip;
	Int2 year = 0;
	CharPtr jta = NULL,
		volume,
		pages;
	Char buf[250];

	switch (vnp->choice)
	{
		case PUB_Equiv:
			for (tvnp = (ValNodePtr)(vnp->data.ptrvalue); tvnp != NULL;
				tvnp = tvnp->next)
			{
				PubWrite(accession, tvnp, fp);
			}
			break;
		case PUB_Article:
			cap = (CitArtPtr)(vnp->data.ptrvalue);
			if (cap->from == 1)  /* from a journal */
			{
				cjp = (CitJourPtr)(cap->fromptr);
				ip = cjp->imp;
				for (tvnp = cjp->title; tvnp != NULL; tvnp = tvnp->next)
				{
					switch (tvnp->choice)
					{
						case Cit_title_jta:
						case Cit_title_iso_jta:
						case Cit_title_ml_jta:
							jta = (CharPtr)(tvnp->data.ptrvalue);
							break;
						default:
							break;
					}
					if (jta != NULL) break;
				}

				if (ip->date->data[0] == 1)  /* std date */
					year = (Int2)ip->date->data[1] + 1900;
				else
					year = 0;
				volume = ip->volume;
				if (volume == NULL)
					volume = "(no volume)";
				pages = ip->pages;
				if (pages == NULL)
					pages = "(no pages)";
				if (jta == NULL)
					jta = "(no jta)";
				sprintf(buf, "%s - %s (%d) %s:%s", accession, jta, (int)year,
						volume, pages);
    			fprintf(fp, "%s\n", buf);
    			if (pmon != NULL)
    				MonitorStrValue(pmon, buf);
			}
			break;
		default:
			break;

	}
	return;
}

static void GetPubFromGenBank (SeqEntryPtr sep, Pointer data,
                         Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  BioseqContextPtr bcp;
  SeqFeatPtr sfp;
  ValNodePtr vnp;
  FILE * fp;
  CharPtr accession = NULL;
  ValNode tpub;
  TextSeqIdPtr tsip;
  PubdescPtr pdp;
  Char buf[40];

  if (! IS_Bioseq(sep))
  	return;

  fp = (FILE *) data;   /* get the output file pointer */
	
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (pmon != NULL)
  {
        *buf = '\0';
	SeqIdPrint(bsp->id, buf, PRINTID_FASTA_LONG);
        MonitorStrValue(pmon, buf);
  }
  for (vnp = bsp->id; ((vnp != NULL) && (accession == NULL)); vnp = vnp->next)
  {
  	switch (vnp->choice)
	{
		case SEQID_GENBANK:
		case SEQID_EMBL:
		case SEQID_DDBJ:
			tsip = (TextSeqIdPtr)(vnp->data.ptrvalue);
			if (tsip->accession != NULL)
				accession = tsip->accession;
			break;
		default:
			break;
	}
  }
  
  if (accession == NULL) return;

  bcp = BioseqContextNew(bsp);
  if (bcp == NULL)
	return;

  vnp = NULL;
  tpub.choice = PUB_Equiv;
  tpub.next = NULL;
                                 /* get any pub descriptors */

  while ((vnp = BioseqContextGetSeqDescr(bcp, (Int2)Seq_descr_pub, vnp, NULL)) != NULL)
  {
  	pdp = (PubdescPtr)(vnp->data.ptrvalue);   /* it's a Pubdesc */
	tpub.data.ptrvalue = pdp->pub;			  /* make Pub-equiv into a Pub */
	PubWrite(accession, &tpub, fp);
	
  }

  sfp = NULL;
  while ((sfp = BioseqContextGetSeqFeat(bcp, 0, sfp, NULL, 0)) != NULL)
  {
	if (sfp->data.choice == 6)   /* a pub feature */
	{
	  	pdp = (PubdescPtr)(sfp->data.value.ptrvalue);
		tpub.data.ptrvalue = pdp->pub;
		PubWrite(accession, &tpub, fp);
	}
								 /* get any feature citations */
	if (sfp->cit != NULL)
	{
	  	for (vnp = sfp->cit->data.ptrvalue; vnp != NULL; vnp = vnp->next)
			PubWrite(accession, vnp, fp);
	}
	
  }

  BioseqContextFree(bcp);

  return;

}


/*****************************************************************************
*
*   SeqEntryToFeat()
*     This set of routines finds features (in this case CdRegion) and prints
*     the part of the sequence that they cover
*
*****************************************************************************/


#define CHARSPERLINE  50

typedef struct expstruct {
  FILE      *fp;	  /* file to write sequence to */
  AsnIoPtr  aip;
  Boolean   is_na;	  /* target sequence nucleic acid? */
  Uint1     feat;     /* type of feature to find */
} ExpStruct, PNTR ExpStructPtr;

static void PrintSequence PROTO((BioseqPtr bsp, SeqFeatPtr sfp,
                           FILE *fp, Boolean is_na));
static void LIBCALLBACK GetSeqFeat PROTO((AsnExpOptStructPtr aeosp));

static void SeqEntryToFeat (SeqEntryPtr sep, FILE *fp)

{
  AsnExpOptPtr  aeop;
  AsnIoPtr      aip;
  ExpStructPtr  esp;

  if (sep != NULL && fp != NULL) {
    esp = MemNew (sizeof (ExpStruct));
    if (esp != NULL) {
      aip = AsnIoNullOpen ();
      if (aip != NULL) {
        esp->fp = fp;
        esp->aip = AsnIoNew (ASNIO_TEXT_OUT, fp, NULL, NULL, NULL);
        esp->is_na = is_na;
        esp->feat = 3;  /* look for CdRegion SeqFeat */
        aeop = AsnExpOptNew (aip, "Seq-feat", (Pointer) esp, GetSeqFeat);
        if (aeop != NULL) {
          SeqEntryAsnWrite (sep, aip, NULL);
          fflush (fp);
          AsnExpOptFree (aip, aeop);
        }
        AsnIoClose (aip);
      }
      MemFree (esp);
    }
  }
}


static void PrintSequence (BioseqPtr bsp, SeqFeatPtr sfp,
                           FILE *fp, Boolean is_na)

{
  Char        buffer [255];
  Uint1       code;
  Int2        count;
  Uint1       repr;
  Uint1       residue;
  SeqPortPtr  spp;
  CharPtr     title;
  CharPtr     tmp;

  if (bsp != NULL && fp != NULL) {
    if ((Boolean) ISA_na (bsp->mol) == is_na) {
      repr = Bioseq_repr (bsp);
      if (repr == Seq_repr_raw || repr == Seq_repr_const) {
        title = BioseqGetTitle (bsp);
        tmp = StringMove (buffer, ">");
        tmp = SeqIdPrint (bsp->id, tmp, PRINTID_FASTA_LONG);
        tmp = StringMove (tmp, " ");
        StringNCpy (tmp, title, 200);
        fprintf (fp, "%s\n", buffer);
		if (pmon != NULL)
			MonitorStrValue(pmon, buffer);
        if (is_na) {
          code = Seq_code_iupacna;
        } else {
          code = Seq_code_iupacaa;
        }
        if (sfp != NULL) {
          spp = SeqPortNewByLoc (sfp->location, code);
        } else {
          spp = SeqPortNew (bsp, 0, -1, 0, code);
        }
        if (spp != NULL) {
          count = 0;
          while ((residue = SeqPortGetResidue (spp)) != SEQPORT_EOF) {
            if (! IS_residue (residue)) {
              buffer [count] = '\0';
              fprintf (fp, "%s\n", buffer);
              count = 0;
              switch (residue) {
                case SEQPORT_VIRT :
                  fprintf (fp, "[Gap]\n");
                  break;
                case SEQPORT_EOS :
                  fprintf (fp, "[EOS]\n");
                  break;
                default :
                  fprintf (fp, "[Invalid Residue]\n");
                  break;
              }
            } else {
              buffer [count] = residue;
              count++;
              if (count >= CHARSPERLINE) {
                buffer [count] = '\0';
                fprintf (fp, "%s\n", buffer);
                count = 0;
              }
            }
          }
          if (count != 0) {
            buffer [count] = '\0';
            fprintf (fp, "%s\n", buffer);
          }
          SeqPortFree (spp);
        }
      }
    }
  }
}

static void LIBCALLBACK GetSeqFeat (AsnExpOptStructPtr aeosp)

{
  BioseqPtr     bsp;
  ExpStructPtr  esp;
  SeqFeatPtr    sfp;

  if (aeosp->dvp->intvalue == START_STRUCT) {
    esp = (ExpStructPtr) aeosp->data;
    sfp = (SeqFeatPtr) aeosp->the_struct;
    if (esp != NULL && esp->fp != NULL && sfp != NULL &&
        sfp->data.choice == esp->feat) {
      bsp = BioseqFind (SeqLocId (sfp->location));
      if (bsp != NULL) {
        PrintSequence (bsp, sfp, esp->fp, esp->is_na);
      }
    }
  }
}

/*****************************************************************************
*
*   These are the rest of the utility routines for reading the CDROM.
*
*****************************************************************************/


/*****************************************************************************
*
*   opens a file and reads SeqEntrys
*   calls ProcessSeqEntry to do the actual work on it
*
*****************************************************************************/
static void ProcessFile (FileListPtr flp, CharPtr root, CharPtr outputfile)
{
  CASN_Handle  casnh;
  FILE         *fp;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep;
  CASN_Type    type;
  Char buf[40];
  Int4 ctr = 0;

  if (flp != NULL) {
    fp = FileOpen (outputfile, "a");
    if (fp != NULL) {
	  if (pmon != NULL)
	  {
	  	sprintf(path, "Opening [%s]", flp->fname);
		MonitorStrValue(pmon, path);
	  }
      StringCpy (path, root);
      FileBuildPath (path, flp->fdir, NULL);
      FileBuildPath (path, NULL, flp->fname);
      if ((casnh = CASN_Open (path)) != NULL) {
        if (! is_na) {
          type = CASN_Type_aa;
        } else {
          type = CASN_Type_nt;
        }
        if (CASN_DocType (casnh) == type) {
          while ((sep = CASN_NextSeqEntry (casnh)) != NULL) {
            if (pmon != NULL)
            {
		ctr++;
		sprintf(buf, "Processing %s Entry %ld", flp->fname,(long)ctr);
                MonitorStrValue(pmon, buf);
	    }
            ProcessSeqEntry (sep, fp);
            SeqEntryFree (sep);
          }
        }
        CASN_Close (casnh);
      }
	  else
		Message(MSG_ERROR, "Can't open [%s]", path);
      FileClose (fp);
    } else {
      Message (MSG_FATAL, "Unable to reopen output file [%s]", outputfile);
    }
  }
}


/*****************************************************************************
*
*  Mounts the appropriate cdrom
*  Calls ProcessFile to Open and read through the file
*
*****************************************************************************/
static void ProcessFileList (FileListPtr flp, CharPtr outputfile)

{
  Int2         device;
  FileListPtr  next;
  Char         root [PATH_MAX];

  if (flp != NULL) {
    root [0] = '\0';
    device = flp->cdnum;
    flp = flp->next;
    while (flp != NULL) {
      next = flp->next;
      if (device != flp->cdnum) {
        if (! CdMountEntrezVolume (flp->cdnum, root, sizeof (root))) {
          Message (MSG_FATAL, "CdMountEntrezVolume failed");
          root [0] = '\0';
        }
      }
      ProcessFile (flp, root, outputfile);
      device = flp->cdnum;
      flp = next;
    }
  }
}

/*****************************************************************************
*
*   Add a new file list element
*
*****************************************************************************/
static FileListPtr FileListNew (FileListPtr flp, Int2 cdnum,
                                CharPtr fdir, CharPtr fname)

{
  FileListPtr  newnode;

  newnode = (FileListPtr) MemNew (sizeof (FileList));
  if (newnode != NULL) {
    if (flp != NULL) {
      while (flp->next != NULL && flp->next->cdnum <= cdnum) {
        flp = flp->next;
      }
      newnode->next = flp->next;
      flp->next = newnode;
    }
    newnode->cdnum = cdnum;
    if (fdir != NULL && *fdir != '\0') {
      newnode->fdir = StringSave (fdir);
    }
    if (fname != NULL && *fname != '\0') {
      newnode->fname = StringSave (fname);
    }
  }
  return newnode;
}

/*****************************************************************************
*
*   Get all appropriate files to search
*
*****************************************************************************/
static Boolean LIBCALLBACK EnumerateFiles (int cdnum, const char *fdir,
                                           const char *fname, long fsize,
                                           void *opaque_data)

{
  FileListPtr      flp;
  FileListPtr PNTR head;

  head = (FileListPtr PNTR) opaque_data;
  flp = NULL;
  if (head != NULL) {
    flp = FileListNew (*head, (Int2) cdnum, (CharPtr) fdir, (CharPtr) fname);
    if (*head == NULL) {
      *head = flp;
    }
  } else {
    flp = FileListNew (NULL, (Int2) cdnum, (CharPtr) fdir, (CharPtr) fname);
  }
  return TRUE;
}
