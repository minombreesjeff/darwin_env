/* $Id: batch.c,v 6.19 2000/08/30 16:44:20 vakatov Exp $
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
* File Name:  $RCSfile: batch.c,v $
*
* Author:  Sergei Shavirin
*
* Version Creation Date: 12/16/1996
*
* $Revision: 6.19 $
*
* File Description:
*         Main file for WWW and Command Line BatchEntrez programs
*
* $Log: batch.c,v $
* Revision 6.19  2000/08/30 16:44:20  vakatov
* Fixed printf() format mismatch
*
* Revision 6.18  2000/05/09 13:37:01  shavirin
* Use functions IS_ntdb_accession() and IS_protdb_accession() to
* verify accession number.
*
* Revision 6.17  2000/02/03 21:00:38  beloslyu
* fix the NCBI_Date initialization
*
* Revision 6.16  1999/10/21 21:10:04  shavirin
* Added possibility to retrive RefSeq accessions.
*
* Revision 6.15  1999/02/24 16:49:23  kans
* use accutils copy of IS_ntdb_accession and IS_protdb_accession
*
* Revision 6.14  1998/12/15 17:56:05  vakatov
* Fixed a tyny C++ compilation bug
*
* Revision 6.13  1998/07/07 13:43:41  shavirin
* Fixed warning of tough compiler setting.
*
* Revision 6.12  1998/05/19 21:54:05  shavirin
* Fixed function, that prints Batch Entrez WWW page
*
* Revision 6.11  1998/05/08 15:51:30  vakatov
* fixed UMR and a tiny typo;  cleaned up some code
*
* Revision 6.10  1998/05/01 17:57:47  shavirin
* New revision
*
* Revision 6.9  1998/04/17 20:53:50  shavirin
* Check for accession format was made more "relaxed".
*
* Revision 6.8  1998/03/26 21:08:42  shavirin
* Changed exit(1) -> return 1 in Main() function.
*
* Revision 6.7  1997/12/10 18:00:24  shavirin
* Removed limits on number of gis to retrieve from command line mode
*
* Revision 6.5  1997/12/09 16:13:44  shavirin
* Removed message in ASN1_GENB case
*
* Revision 6.4  1997/12/01 20:09:56  shavirin
* Removed message in front of ASN1 outputs
*
* Revision 6.3  1997/11/26 21:57:13  shavirin
* Added format 5 - Single GenBank Bioseq-set
*
* Revision 6.2  1997/11/03 20:48:42  shavirin
* Added workaround for API bug with single gi retrieval
*
* Revision 6.1  1997/09/10 14:05:34  shavirin
* Added AE- type of accesssions handling
*
* Revision 6.0  1997/08/25 18:19:05  madden
* Revision changed to 6.0
*
* Revision 1.17  1997/07/23 19:24:32  shavirin
* Changed default background to white
*
* Revision 1.16  1997/07/22 18:57:30  shavirin
* Removed any limits for number of retrieved entryes
* if program used from command line
*
* Revision 1.15  1997/07/21 15:03:18  shavirin
* Now strings like ">Protein" will be ignored
*
* Revision 1.14  1997/07/03 16:23:17  shavirin
* Added ability to retrieve few gis from single accession
*
* Revision 1.13  1997/06/27 18:32:59  shavirin
* Added AF- style nucleotide accessions to be accepted
*
* Revision 1.12  1997/05/14 19:14:31  shavirin
* Added #define LF 10
*
 * Revision 1.11  1997/04/25  04:25:21  shavirin
 * Few fixes due to usage of the program through proxy and small
 * bug with reading from file
 *
 * Revision 1.10  1997/04/09  19:29:24  shavirin
 * Included ability to retrieve Protein accessions
 *
 * Revision 1.9  1997/03/28  18:23:13  shavirin
 * Use PubMed accession index instead of SeqId index. Removed "www,www3"
 * references for better proxying.
 *
 * Revision 1.8  1997/03/14  15:38:38  shavirin
 * Removed difference between capital and small characters for
 * accesssion number checkup.
 *
 * Revision 1.7  1997/03/13  16:15:52  shavirin
 * Added new option for WWW Batch Entrez to retrieve single entry
 * or complete set.
 *
 * Revision 1.6  1997/03/12  22:47:41  shavirin
 * Added option to return only one entry from one entry
 *
 * Revision 1.5  1997/03/04  17:19:22  shavirin
 * Fixed parser for long invalid accessions and added comma as
 * valid delimiter of accesssions/gis
 *
 * Revision 1.4  1997/01/23  19:02:37  shavirin
 * Removed creation of spurious logfiles in command-line mode
 *
 * Revision 1.3  1996/12/17  17:27:18  shavirin
 * Function WWWSendBatchPage() changed to static
 *
 * Revision 1.2  1996/12/16  19:55:35  shavirin
 * Changed file description.
 *
 * Revision 1.1  1996/12/16  19:51:37  shavirin
 * Initial revision
 *
*
* ==========================================================================
*/

#define LogFile "wwwbatch.log"

#include <ncbi.h>
#include <ffprint.h>
#include <accentr.h>
#include <accutils.h>
#include <tofasta.h>
#include <asn2ff.h>
#include <ncbiwww.h>

#define LF 10

typedef struct BatchAccList {
  CharPtr acc;
  Int4 gi;
  struct BatchAccList *next;
} BatchAccList, PNTR BatchAccListPtr;

typedef struct BatchParam {
    Int4     format;
    Boolean  dump;
    Int4     single_entry;
    Int4     request;
    Int4     sequence;
    CharPtr  organism;
    CharPtr  file;
    Boolean  html;
    Boolean  id_lookup;
    Boolean  CommandLine;  
} BatchParam, PNTR BatchParamPtr;

typedef struct BGenBank {
    AsnIoPtr   aip;
    AsnTypePtr atp;
    AsnTypePtr atp_bioseq_set_seq_set;
    AsnTypePtr atp_bioseq_set;
} BGenBank, PNTR BGenBankPtr;

#define REQ_ORG   0
#define REQ_LIST  1

#define F_GEN       0
#define F_FASTA     1
#define F_ASN1      2
#define F_GILIST    3
#define F_DLIST     4
#define F_ASN1_GENB 5

#define BSEQ_NA   TYP_NT
#define BSEQ_AA   TYP_AA

#define SEARCH_LIMIT  20000
#define SEARCH_DLIMIT 70000

static void WWWSendBatchPage(Int4 which);

static Int4 AccessionToGi(CharPtr string,  Int4Ptr PNTR giptr, Int2 seqtype);
static BatchAccListPtr GetAccList(BatchParamPtr batchP, Int4Ptr total);

static Boolean PrintGi(Int4 gi, Int4 format, 
                       FILE *fd, Int4 seq_type, 
                       Int4 single_entry, BGenBankPtr bgbp);

static BatchParamPtr MakeBatchParameters(WWWEntryPtr PNTR entries, Int4 m);
static ByteStorePtr GetGisFromOrg(CharPtr org, 
                             Int4Ptr GiNum, Int4 seqtype);

extern Boolean SeqEntrysToDefline(SeqEntryPtr sep, 
                                  FILE *fp, Boolean is_na, Uint1 group_segs);
static BatchParamPtr MakeCommandLineParameters(void);

void BatchHead(VoidPtr pointer, FILE *fd);
void BatchTail(VoidPtr pointer, FILE *fd);

static CharPtr organism[] = { "(None)",
                              "Arabidopsis thaliana",
                              "Bacillus subtilis",
                              "Bos taurus",
                              "Caenorhabditis elegans",
                              "Dictyostelium discoideum",
                              "Drosophila melanogaster",
                              "Escherichia coli",
                              "Gallus gallus",
                              "Homo sapiens",
                              "Human immunodeficiency virus type 1",
                              "Mus musculus",
                              "Oryctolagus cuniculus",
                              "Oryza sativa",
                              "Ovis aries",
                              "Rattus norvegicus",
                              "Saccharomyces cerevisiae",
                              "Schizosaccharomyces pombe",
                              "Simian immunodeficiency virus",
                              "Xenopus laevis",
                              "Zea mays",
                              NULL
};


#define MACRO_atp_find(atp,name)\
        if((atp = AsnTypeFind(amp, #name))==NULL){\
                ErrPostEx(SEV_ERROR,0,0,\
                        "Could not find type <%s>", #name);\
                return NULL; \
        }


#define BatchTitle "<A HREF=\"/htbin-post/PubMed/imagemap/EntrezBatch/batch.map\"><IMG SRC=\"/EntrezBatch/batch.gif\" BORDER=0 ISMAP HEIGHT=22 WIDTH=500></A>"

#define BatchResultsTitle "<A HREF=\"/htbin-post/PubMed/imagemap/EntrezBatch/batch.map\"><IMG SRC=\"/EntrezBatch/batch_results.gif\" BORDER=0 ISMAP HEIGHT=22 WIDTH=500></A>"
/************************************************************************
 *
 *  void WWWSendPage(Int4 which) - function to draw entry table for 
 *                                 the WWW Blast program
 *
 ***********************************************************************/

static void WWWSendBatchPage(Int4 which) {
  register Int4 i;
   
  printf("HTTP/1.0 200 OK\r\n");
  printf("MIME-Version: 1.0\r\n");

  printf("Content-type: text/html\r\n\r\n");  
  printf("<HTML>\n<HEAD>\n");
  printf("<TITLE>Batch Entrez</TITLE>\n</HEAD>\n");

  printf("<BODY bgcolor=\"#FFFFFF\" text=\"#000000\" "
         "link=\"#0000f0\" vlink=\"#6000b0\" alink=\"#f00000\">\n");
  
  printf("%s%c", BatchTitle, LF);
  
  printf("<FORM ACTION=\"http://%s:%s%s/result\" METHOD=POST "
         "NAME=\"BATCH\" %s >%c", 
         getenv("SERVER_NAME") != NULL ? getenv("SERVER_NAME") : "NOT_SET",
         getenv("SERVER_PORT") != NULL ? getenv("SERVER_PORT") : "NOT_SET",
         getenv("SCRIPT_NAME") != NULL ? getenv("SCRIPT_NAME") : "NOT_SET",
         which ? "ENCTYPE=\"multipart/form-data\" " : "", LF);
  
  printf("<BR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; This page is designed "
         "to allow you to download (receive) a large "
         "number of sequences from Entrez, in a batch mode. "
         "The results of the search will be saved to a local "
         "disk file on your machine.  Upon submitting your query, "
         "you will be prompted to provide the filename "
         "where the results will be stored. Please make sure that "
         "you have enough disk space on your computer before "
         "submitting this request.\n%c", LF);
  
  printf("<BR><BR><B>Choose type of sequences "
         "to search and format of output: </B><BR><BR>");

  printf("Sequence type: %c",LF);

  printf("<select name = SEQ_TYPE>");
  printf("<option> Nucleotide ");
  printf("<option> Protein ");
  printf("</select>");

  printf("&nbsp;&nbsp;"
         "<INPUT TYPE=\"checkbox\" NAME=\"RETURN_ALL_SET\" "
         "VALUE=YES>&nbsp;&nbsp; "
         "Include all records within a segmented set");

  printf("<BR>");
#ifdef NOT_SAVE
  printf("<INPUT TYPE=\"checkbox\" NAME=\"DUMP_TYPE\" "
         "VALUE=FILE></B>&nbsp;&nbsp; Save results to file");
#else
  printf("<INPUT TYPE=\"hidden\" NAME=\"DUMP_TYPE\" "
         "VALUE=FILE >");
#endif
  
  printf("</B>Format: &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
         "&nbsp;&nbsp;&nbsp;&nbsp;%c",LF);

  printf("<select name = FORMAT>");
  printf("<option> GenBank/GenPept");
  printf("<option> FASTA");
  printf("<option> ASN.1");
  printf("<option> List of GIs");
  printf("<option> List of Deflines");
  printf("</select>");

  printf("<INPUT TYPE=\"checkbox\" NAME=\"HTML\" "
         "VALUE=HTML CHECKED>&nbsp;&nbsp; HTML"); 

  printf("<HR>");

  printf("<INPUT TYPE=\"radio\" NAME=\"REQUEST_TYPE\" "
         "VALUE=ORGANISM CHECKED> &nbsp;&nbsp;&nbsp; <B>Retrieve all "
         "sequences for a specific organism.</B>");

  printf("<BR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
         "Enter organism name here"
         /*  "(use full scientific name or common name)" */
         "</B>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
         "<INPUT TYPE=\"text\" NAME=\"ORGNAME\" "
         "VALUE=\"\" MAXLENGTH=\"50\">"); 

  printf("<BR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
         "Or choose it from list: <select name=LIST_ORG>");
  for(i=0; organism[i] != NULL; i++)
    printf("<option> %s ", organism[i]);
  printf("</select>");
  printf("<BR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<A HREF="
         "\"/Taxonomy/tax.html\">"
         "Explore the taxonomy database at NCBI</A>");

  printf("<HR>");
  printf("<INPUT TYPE=\"radio\" NAME=\"REQUEST_TYPE\" "
         "VALUE=FILESUBMIT > &nbsp;&nbsp;&nbsp; <B>Retrieve all sequences from a "
         "%s of Gis/Accessions</b>", 
         which? "file" : "list");

  if(!which) {
    
    printf("<BR>Enter gis/accessions here "
           "(delimited by spaces or newlines)<BR> ");
    printf("<textarea name=\"USERFILE\" rows=6 cols=60>"
           "</textarea>%c", LF); 
  } else {
    printf("<BR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
    printf("Enter filename here &nbsp; "
           "<INPUT TYPE=\"file\" NAME=\"USERFILE\" "
           "onFocus=\"window.status='Press radio button to "
           "activate this search type'; return true;\" "); 
  }
  
  printf("<HR>");
  printf("<BR><INPUT TYPE=\"submit\">%c", LF);
  printf("<INPUT TYPE=\"reset\" VALUE=\"Clear input\">%c", LF);
  printf("</FORM>%c", LF);
  printf("<HR>%c", LF);
  
  printf("<ADDRESS>");
  printf("Comments and suggestions to:"
         "&lt; <a href=\"mailto:info@ncbi.nlm.nih.gov\">"
         "info@ncbi.nlm.nih.gov"
         "</a> &gt; <BR> Credits to: "
         "<a href=\"mailto:shavirin@ncbi.nlm.nih.gov\">"
         "Sergei B. Shavirin</a>\n"
         "<!-- <a href=\"http://www.ncbi.nlm.nih.gov/STS/shavirin.html\">"
         "Sergei B. Shavirin</a> -->"
         "<BR>Acknowledgements to: " 
         "<a href=\"mailto:epstein@ncbi.nlm.nih.gov\">"
         "Jonathan Epstein</a>");
  printf("</ADDRESS>%c", LF);

} /* WWWSendBatchPage() */

/************************************************************************
 *
 *  Int2 Main() - main function for the WWW BatchEntrez search program    
 *
 ***********************************************************************/
#define NUMARGS 8

Args dump_args[NUMARGS] = {
  {"Sequence type\n"
   "         0 - Nucleotide  \n"
   "         1 - Protein",
   NULL, NULL,NULL,FALSE,'s',ARG_INT,0.0,0,NULL},
  {"Format of output\n"
   "         0 - GenBank/GenPept   \n"
   "         1 - FASTA \n" 
   "         2 - ASN.1 \n"
   "         3 - List of GIs\n"
   "         4 - List of Deflines\n"
   "         5 - Single GenBank Bioseq-set ASN.1",
   "0", NULL,NULL,FALSE,'f',ARG_INT, 0.0,0,NULL},
  {"Number of returned entries from single gi/accession\n"
   "         0 - Return all entries in SeqEntry, that available  \n"
   "         1 - Return single entry specified by gi/accession",
   "0", NULL,NULL,FALSE,'n',ARG_INT,0.0,0,NULL},
  {"Text or HTML? (For GenBank/GenPept)\n"
   "         0 - Text output   \n"
   "         1 - HTML output", 
   "0", NULL,NULL,FALSE,'h',ARG_INT, 0.0,0,NULL},
  { "File with list of GIS/Accessions", 
    "stdin", NULL, NULL, TRUE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  {"Organism name (for retrieve by organism)",
   NULL, NULL,NULL,TRUE,'o',ARG_STRING, 0.0,0,NULL},  
  {"ID (accession or gi)",
   NULL, NULL,NULL,TRUE,'u',ARG_STRING, 0.0,0,NULL},
  {"Logfile name:",
   "nbatch.log", NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL}
};

static BatchParamPtr MakeCommandLineParameters(void)
{
  BatchParamPtr batchP;
  FILE *fd;

  if ( !GetArgs ("Nbatch",NUMARGS,dump_args) ) {
    return NULL;
  }

  if( !ErrSetLogfile (dump_args[7].strvalue, ELOG_APPEND) ) {
    exit(1);
  }
  ErrSetLogLevel(SEV_MAX); 

  batchP = (BatchParamPtr) MemNew(sizeof(BatchParam));

  batchP->dump     = TRUE;

  if(dump_args[0].intvalue)
    batchP->sequence = BSEQ_AA;
  else
    batchP->sequence = BSEQ_NA;

  batchP->format  =        dump_args[1].intvalue;
  batchP->single_entry =        dump_args[2].intvalue;
  batchP->html    = (Uchar)dump_args[3].intvalue;
  batchP->request = REQ_LIST;

  if(dump_args[6].strvalue != NULL) {
    
    batchP->file = StringSave(dump_args[6].strvalue);
    batchP->id_lookup = TRUE;    

  } else if((batchP->organism = 
             StringSaveNoNull(dump_args[5].strvalue)) != NULL) {
    batchP->file = StringSave("");    
    batchP->request = REQ_ORG;
  } else if(dump_args[4].strvalue != NULL) {
    if((fd = FileOpen(dump_args[4].strvalue, "r")) == NULL) {
      ErrLogPrintf("Input file do not exists or empty\n");
      return NULL;
    }
    if((batchP->file = WWWReadFileInMemory(fd, 0, TRUE)) == NULL)
      return NULL;
    FileClose(fd);

    batchP->organism = StringSave("");

  } else { /* Error no valid input found This MUST not happen */
    return NULL;
  }

  return batchP;
}
BGenBankPtr BGenBankInit(void)
{
    BGenBankPtr bgbp;
    AsnModulePtr amp;

    AsnTypePtr       atp_bioseq_set;
    AsnTypePtr       atp_bioseq_set_level;
    AsnTypePtr       atp_bioseq_set_class;
    AsnTypePtr       atp_bioseq_set_release;
    AsnTypePtr       atp_bioseq_set_date;
    AsnTypePtr       atp_bioseq_set_seq_set;
    AsnTypePtr       atp_bioseq_set_seq_set_E;

    Char  release[] = "Nbatch Dump";
    Char  date_time[128];
    NCBI_Date       date={{0,0,0,0,255,255,255},NULL};
    DataVal dv;

    bgbp = (BGenBank*)MemNew(sizeof(BGenBank));
    bgbp->aip = AsnIoNew(ASNIO_TEXT_OUT, stdout, NULL, NULL, NULL);
    
    amp = AsnAllModPtr();

    MACRO_atp_find(atp_bioseq_set,Bioseq-set);
    MACRO_atp_find(atp_bioseq_set_level,Bioseq-set.level);
    MACRO_atp_find(atp_bioseq_set_class,Bioseq-set.class);
    MACRO_atp_find(atp_bioseq_set_release,Bioseq-set.release);
    MACRO_atp_find(atp_bioseq_set_date,Bioseq-set.date);
    MACRO_atp_find(atp_bioseq_set_seq_set,Bioseq-set.seq-set);
    MACRO_atp_find(atp_bioseq_set_seq_set_E,Bioseq-set.seq-set.E);

    if(!AsnOpenStruct(bgbp->aip,atp_bioseq_set,NULL))     
        return NULL;
    dv.intvalue = 0;

    if(!AsnWrite(bgbp->aip,atp_bioseq_set_level,&dv))     
        return NULL;
    dv.intvalue = 7;

    if(!AsnWrite(bgbp->aip,atp_bioseq_set_class,&dv))     
        return NULL;
    dv.ptrvalue = release;

    if(!AsnWrite(bgbp->aip,atp_bioseq_set_release,&dv))     
        return NULL;

    Nlm_DayTimeStr(date_time,TRUE,TRUE);
    date.str=date_time;

    if(!DateAsnWrite(&date,bgbp->aip,atp_bioseq_set_date))        
        return NULL;
    
    if(!AsnOpenStruct(bgbp->aip,atp_bioseq_set_seq_set,NULL))     
        return NULL;
    AsnIoFlush(bgbp->aip);
    
    bgbp->atp =atp_bioseq_set_seq_set_E;
    bgbp->atp_bioseq_set_seq_set = atp_bioseq_set_seq_set;
    bgbp->atp_bioseq_set = atp_bioseq_set;
    
    return bgbp;
}
void BGenBankClose(BGenBankPtr bgbp)
{
    AsnCloseStruct(bgbp->aip, bgbp->atp_bioseq_set_seq_set,NULL);
    AsnCloseStruct(bgbp->aip, bgbp->atp_bioseq_set,NULL);
    
    AsnIoClose(bgbp->aip);
    MemFree(bgbp);
    return;
}

Int2 Main ()
{
  Int4 i, gi;
  ByteStorePtr gis_bsp;
  Int4 NumGis;
  BatchParamPtr batchP = NULL;
  BatchAccListPtr AccList, AccTmp;
  Int4 TotalNumAcc =0;
  time_t time_now;
  CharPtr TimeNowStr;
  WWWInfoPtr info;
  WWWErrorCode error;
  FILE *log_file = NULL;
  BGenBankPtr  bgbp = NULL;
  
  if((error = WWWReadPosting(&info)) != WWWErrOk) {
    ErrLogPrintf("Error in processing WWW request\n");
    return 1;
  }

  if(WWWGetMethod(info) != COMMAND_LINE) {
      if( !ErrSetLogfile (LogFile, ELOG_APPEND) ) {
          return 1;
      }
      ErrSetLogLevel(SEV_MAX); 
      log_file = FileOpen(LogFile, "a");
  }
  
  if(WWWGetMethod(info) == COMMAND_LINE) {
      if((batchP = MakeCommandLineParameters()) == NULL) {
          return 1;
      }
      batchP->CommandLine = TRUE;
  } else if (WWWGetMethod(info) == WWW_GET) {
      time_now = GetSecs();
      TimeNowStr = ctime(&time_now);
      TimeNowStr[24] = '\0'; 
      fprintf(log_file, "\n%s|%s|%s|%s|%d",
              TimeNowStr, WWWGetAddress(info), 
              WWWGetHost(info), WWWGetAgent(info), 0);
      
      if(WWWGetBrowser(info) != NETSCAPE)
          WWWSendBatchPage(0);
      else
          WWWSendBatchPage(1);
      
      FileClose(log_file);
      return 1;
  } else { /* method == POST */
      
      if((batchP = MakeBatchParameters(WWWGetWWWEntries(info), 
                                       WWWGetNumEntries(info))) == NULL) {       
          printf("Content-type: text/html\r\n\r\n");
          printf("Error in creating BATCH parameters");
          FileClose(log_file);
          return 1;
      }
  }
  
  time_now = GetSecs();
  TimeNowStr = ctime(&time_now);
  TimeNowStr[24] = '\0'; 
  if(!batchP->CommandLine) {
      fprintf(log_file, "\n%s|%s|%s|%s|%d|%d|%d|%d|%d|%d|%s|%d|",
              TimeNowStr, WWWGetAddress(info), 
              WWWGetHost(info), WWWGetAgent(info), (int)1,
              (int)batchP->format, (int)batchP->html, (int)batchP->dump,
              (int)batchP->request, (int)batchP->sequence,
              (batchP->organism[0] == NULLB) ? "(null)" : batchP->organism,
              (int)(batchP->file[0] == NULLB ? 0 : 1));
  }
  FileClose(log_file);
  
  if((batchP->request == REQ_ORG) && (StringLen(batchP->organism) < 3)) {
      if(!batchP->CommandLine) {
          printf("<TITLE>Batch Entrez Results</TITLE>");
          printf("%s%c<BR><BR><BR><b>", BatchResultsTitle, LF);
      }
      printf("ERROR: Length of organism name must be more "
             "than 2 characters\n");
      if(!batchP->CommandLine)
          printf("</b>");
      return 1;
  }
  if((batchP->file[0] == NULLB) && (batchP->request != REQ_ORG)) {
      if(!batchP->CommandLine) { 
          printf("Content-type: text/html\r\n\r\n");
          printf("<TITLE>Batch Entrez Results</TITLE>");
          printf("%s%c<BR><BR><BR><b>", BatchResultsTitle, LF);
      }
      printf("ERROR: You did not entered filename "
             "with gis/accessions or file may be empty.\n");
      if(!batchP->CommandLine)
          printf("</b>");
      return 1;
  }
  
  if(!batchP->CommandLine) {
      
      printf("HTTP/1.0 200 OK\r\n");
      printf("MIME-Version: 1.0\r\n");
      
      if(batchP->dump) {
          printf("Content-type: application/octet-stream\r\n\r\n");	
      } else {
          printf("Content-type: text/html\r\n\r\n");
          printf("<TITLE>Batch Entrez Results</TITLE>");
          printf("%s%c", BatchResultsTitle, LF);
      }
  }
  
#ifdef DEBUG_MODE
  printf("Content-type: text/html\r\n\r\n");
  for(i=0; i < m; i++) {
      printf("%s : %s\n <BR>%c", entries[i]->name, entries[i]->val, LF); 
  }
  exit(1);
#endif
  
  putenv("USER=BatchEntrez");
  
  if(batchP->html) {
      init_www(); /* initializing WWW mode */
      head_tail_ff(NULL, BatchHead, BatchTail);
  }
  
  if(!batchP->dump || batchP->html)
      printf("<PRE>"); /* Entering text mode */
  
  /* Starting up connection to Entrez */
  
  if (! EntrezInit("BatchEntrez", FALSE, NULL)) {
      printf("Cannot initialize Entrez\r\n");
      return 1;
  }
  
  EntrezBioseqFetchEnable("BatchEntrez", TRUE);
  
  /* Finding all gi's of given organism */
  
  if(batchP->request == REQ_ORG) {
      if((gis_bsp = GetGisFromOrg(batchP->organism, 
                                  &NumGis, batchP->sequence)) == NULL) {
          printf("**** ERROR: Lookup for GI's of organism \"%s\" failed\r\n",
                 batchP->organism);
          return 1;
      } else {
          if(!batchP->dump || (batchP->format != F_FASTA))
              printf("**** %d gi's found for organism \"%s\"\r\n\r\n", 
                     (int)NumGis, batchP->organism);
          /* setting limit of printing */
          if(WWWGetMethod(info) != COMMAND_LINE && 
             NumGis > (batchP->format == F_DLIST ? SEARCH_DLIMIT : SEARCH_LIMIT)) {
              printf("**** The number of retrieved GIs (%d) "
                     "exceeds the current limit of %d.\r\n"
                     "**** Only the list of GIs will be downloaded "
                     "to your computer.  Please divide\r\n"
                     "**** this list into smaller parts and submit "
                     "them separately.\r\n\r\n", 
                     (int)NumGis, (int)(batchP->format == F_DLIST ? 
                              SEARCH_DLIMIT : SEARCH_LIMIT));        
              batchP->format = F_GILIST;
          }
      }
      if(batchP->format != F_GILIST && batchP->html) 
          printf("<HR>");
      
      if(batchP->format == F_ASN1_GENB)
          bgbp = BGenBankInit();
      
      for(i= 0; i < NumGis; i++) {
          BSRead(gis_bsp, &gi, sizeof(Int4));
          if(!PrintGi(gi, batchP->format, stdout, 
                      batchP->sequence, batchP->single_entry, bgbp)) {
              printf("Error in printing gi %d\r\n", (int) gi);        
          }
          if(batchP->html) 
              printf("<HR>");  
      }
      
      if(batchP->format == F_ASN1_GENB)
          BGenBankClose(bgbp);
      
  } else {
      
      /* Printing all sequences from list of accessions in user's file */
      
      if((AccList = GetAccList(batchP, &TotalNumAcc)) == NULL) {
          printf("**** ERROR: No valid Gis/Accessions found\r\n");
          return 1;
      }
    
      if(batchP->format != F_ASN1_GENB &&
         batchP->format != F_FASTA &&
         batchP->format != F_ASN1) {
          if(!batchP->dump && !batchP->id_lookup)
              printf("**** %d validly formatted Gis/Accessions present in "
                     "Entrez-batch request\r\n\r\n", (int)TotalNumAcc);
      }
      
      if(WWWGetMethod(info) != COMMAND_LINE) {
          if(TotalNumAcc > (batchP->format == F_DLIST ? 
                            SEARCH_DLIMIT : SEARCH_LIMIT)) {
              printf("**** The number of retrieved Gis/Accessions (%d) "
                     "exceeds the current limit of %d.\r\n"
                     "**** Please divide this list into smaller parts and submit "
                     "them separately.\r\n\r\n", 
                     (int)TotalNumAcc, (int)(batchP->format == F_DLIST ? 
                                   SEARCH_DLIMIT : SEARCH_LIMIT));        
              return 1;
          }
      }
      if(batchP->html) 
          printf("<HR>");
      
      if(batchP->format == F_ASN1_GENB)
          bgbp = BGenBankInit();
      
      for (AccTmp = AccList; AccTmp != NULL; AccTmp = AccTmp->next) {  
          if(!PrintGi(AccTmp->gi, batchP->format, 
                      stdout, batchP->sequence, batchP->single_entry, bgbp)) {
              printf("**** WARNING: Printing Gi %d (%s) failed \r\n\r\n", 
                     (int)AccTmp->gi, AccTmp->acc);
          }
          
          if(batchP->html) 
              printf("<HR>");
      }
      if(batchP->format == F_ASN1_GENB)
          BGenBankClose(bgbp);
  }
  
  /* terminating Entrez connection */
  
  if(!batchP->CommandLine)
      printf("\r\n\r\n**** Transfer completed successfuly -------=-*\r\n\r\n");
  
  EntrezFini();
  EntrezBioseqFetchDisable();
  fflush(stdout);  
  
  Nlm_FreeArgs(NUMARGS, dump_args);

  return 0;
}


static BatchParamPtr MakeBatchParameters(WWWEntryPtr PNTR entries, Int4 m)
{
  Int4 i;
  BatchParamPtr batchP;
  Boolean OrganismSet = FALSE;

  batchP = (BatchParamPtr) MemNew(sizeof(BatchParam));

  batchP->format   = F_GEN;
  batchP->dump     = FALSE;
  batchP->request  = REQ_ORG;
  batchP->sequence = BSEQ_NA;
  batchP->organism = NULLB;
  batchP->file     = NULLB;
  batchP->html     = FALSE;
  batchP->single_entry  = TRUE;

  for(i=0; i <  m; i++) {

    /* FORMAT */

    if (!StringICmp(entries[i]->name, "FORMAT")) { 
      if (!StringICmp(entries[i]->val, "GenBank/GenPept"))
        batchP->format = F_GEN;
      else if (!StringICmp(entries[i]->val, "FASTA"))
        batchP->format = F_FASTA;
      else if (!StringICmp(entries[i]->val, "ASN.1"))
        batchP->format = F_ASN1;
      else if (!StringICmp(entries[i]->val, "List of GIs"))
        batchP->format = F_GILIST;
      else if (!StringICmp(entries[i]->val, "List of Deflines"))
        batchP->format = F_DLIST;

      /* DUMP TYPE */ 

    } else if (!StringICmp(entries[i]->name, "DUMP_TYPE")) { 
      batchP->dump = TRUE;
    } else if (!StringICmp(entries[i]->name, "RETURN_ALL_SET")) { 
      batchP->single_entry = FALSE;

      /* REQUEST TYPE */

    } else if (!StringICmp(entries[i]->name, "REQUEST_TYPE")) { 
      if (!StringICmp(entries[i]->val, "ORGANISM"))
        batchP->request = REQ_ORG;
      else if (!StringICmp(entries[i]->val, "FILESUBMIT"))
        batchP->request = REQ_LIST;

      /* SEQUENCE TYPE */

    } else if (!StringICmp(entries[i]->name, "SEQ_TYPE")) { 
      if (!StringICmp(entries[i]->val, "nucleotide"))
        batchP->sequence = BSEQ_NA;
      else if (!StringICmp(entries[i]->val, "protein"))
        batchP->sequence = BSEQ_AA;

      /* ORGANISM NAME */

    } else if (!StringICmp(entries[i]->name, "ORGNAME")) { 
      batchP->organism = entries[i]->val;
      if(entries[i]->val[0] != NULLB)   
        OrganismSet = TRUE;
    } else if (!StringICmp(entries[i]->name, "LIST_ORG") && !OrganismSet) { 
      if (StringICmp(entries[i]->val, "(None)"))
        batchP->organism = entries[i]->val;

      /* USER FILE */

    } else if (!StringICmp(entries[i]->name, "USERFILE")) { 
      batchP->file = entries[i]->val;
    } else if (!StringICmp(entries[i]->name, "HTML")) { 
      batchP->html = TRUE;
    }
  } /* for (i=0... */

  return batchP;

}

static Int4 AccessionToGi(CharPtr string,  Int4Ptr PNTR giptr, Int2 seqtype) 
{
    Int4 i, gi = 0;
    CharPtr str;
    ByteStorePtr bsp;
    Int4 GiNum;
    
    if(string == NULL)
        return 0;
    
    if((gi = atol(string)) > 0) { /* He-he this is gi... */
        *giptr = (Int4Ptr) MemNew(sizeof(Int4));
        (*giptr)[0] = gi;
        return 1;
    }
    str = (CharPtr) MemNew(StringLen(string)+30);
    sprintf(str, "\"%s\"[ACCN]", string);
    
    if((bsp = EntrezTLEvalXString(str, seqtype, 
                                  -1, NULL, NULL)) == NULL) {
        ErrLogPrintf("Failure to parse input string");
        return 0;
    }
    MemFree(str);
    
    BSSeek(bsp, 0L, 0);  
    
    if((GiNum = BSLen(bsp)/sizeof(DocUid)) < 1) {
        /* Every accession must have one and ONLY ONE gi */
        return 0;
    }
    *giptr = (Int4Ptr) MemNew(sizeof(Int4)*GiNum);
    for(i = 0; i < GiNum; i++) {
        BSRead(bsp, &gi, sizeof(Int4));
        (*giptr)[i] = gi;
    }

    BSFree(bsp);
    return GiNum;
}

static Boolean PrintGi(Int4 gi, Int4 format, FILE *fd, 
                       Int4 seq_type, Int4 single_entry,
                       BGenBankPtr bgbp)
{
    SeqEntryPtr sep, sep_all;
    AsnIoPtr aip;
    Boolean retvalue = TRUE;
    Boolean is_na = (Boolean)(seq_type == BSEQ_NA);
    SeqIdPtr sip = NULL;
    BioseqPtr bsp;

    if(format == F_GILIST) {
        fprintf(fd, "%d\r\n", (int)gi);
        return TRUE;
    }
    
    if((sep_all = EntrezSeqEntryGet(gi, single_entry)) == NULL) {
        printf("**** WARNING: NULL Seq Entry pointer - "
               "request failed for gi %d\r\n", (int)gi); 
        return FALSE;
    }
    
    if(single_entry) {
        ObjMgrRegister(OBJ_SEQENTRY, sep_all);
        sip = ValNodeNew(NULL);
        sip->choice = SEQID_GI;
        sip->data.intvalue = gi;
        
        if((bsp = BioseqFind(sip)) == NULL) {
            printf("**** WARNING: NULL Bioseq pointer - "
                   "request failed for gi %d\r\n", (int)gi); 
            return FALSE;
        }
        sep = SeqEntryNew();
        sep->choice = 1; /* Bioseq */
        sep->data.ptrvalue = bsp;
    } else {
        sep = sep_all;
    }
    
    switch(format) {
    case F_GEN:
        if(!SeqEntryToFlatEx(sep_all, fd, 
                             (Uint1) (is_na ? GENBANK_FMT : GENPEPT_FMT), 
                             RELEASE_MODE, sip, FF_REGULAR)) {
            
            if(!SeqEntryToFlatEx(sep_all, fd, 
                                 (Uint1)(is_na ? GENPEPT_FMT : GENBANK_FMT), 
                                 RELEASE_MODE, sip, FF_REGULAR)) {
                retvalue = FALSE;
            }
        }
        break;
    case F_FASTA:
        if(!SeqEntryToFasta(sep, fd, is_na)) {
            if(!SeqEntryToFasta(sep, fd, (Uint1)!is_na)) {
                printf("**** WARNING: Printing of FASTA format "
                       "(gi=%d) failed\r\n", (int)gi);
                retvalue = FALSE;
            }
        }
        break;
    case F_DLIST:
        
        if (IS_Bioseq(sep))
            retvalue = SeqEntrysToDefline(sep, fd, is_na, 3);
        else
            retvalue = SeqEntrysToDefline(sep, fd, is_na, 0);
        
        if(retvalue == FALSE) {
            if (IS_Bioseq(sep))
                retvalue = SeqEntrysToDefline(sep, fd, (Uint1) !is_na, 3);
            else
                retvalue = SeqEntrysToDefline(sep, fd, (Uint1) !is_na, 0);
        }
        
        break;
        
    case F_ASN1:
        aip = AsnIoNew(ASNIO_TEXT_OUT, fd, NULL, NULL, NULL);
        retvalue = SeqEntryAsnWrite(sep, aip, NULL);
        AsnIoClose(aip);
        break;
    case F_ASN1_GENB:
        retvalue = SeqEntryAsnWrite(sep, bgbp->aip, bgbp->atp);
        break;
    default:
        return FALSE;
    }
    

    if(single_entry) {
        SeqIdFree(sip);
        MemFree(sep);
    }

    SeqEntryFree(sep_all);
    
    return retvalue;
}


static ByteStorePtr GetGisFromOrg(CharPtr org, Int4Ptr GiNum, Int4 seqtype)
{
  ByteStorePtr bsp;
  CharPtr str;

  str = (CharPtr) MemNew(StringLen(org)+30);
  sprintf(str, "\"%s\"[ORGN]", org);
  
  if((bsp = EntrezTLEvalXString(str, (Int2) seqtype, -1, NULL, NULL)) == NULL) {
    ErrLogPrintf("Failure to parse input string");
    return NULL;
  }
  BSSeek(bsp, 0L, 0);  
  *GiNum = BSLen(bsp)/sizeof(DocUid); 
  return bsp;
}

static BatchAccListPtr GetAccList(BatchParamPtr batchP,
                                  Int4Ptr TotalItems) 
{
    Char TmpBuff[16];
    Int4 i, j, k;
    Int4 FileLen = 0;
    BatchAccListPtr AccList = NULL;
    BatchAccListPtr AccListTmp, AccListLast;
    Int4 NumNotValid = 0;
    Int4 NumGis;
    Int4Ptr giptr;
    CharPtr file;
    Int2 seqtype;
    
    if (!batchP  ||  (file = batchP->file) == NULL  ||  !file[0]) {
      *TotalItems = 0;
      return NULL;
    }
    seqtype = batchP->sequence;
    
    FileLen = StringLen(file);
    
    for(i = 0; i < FileLen; i++) {
        
        if(isspace(file[i]) || file[i] == ',') /* Rolling spaces */
            continue;
        
        /* This is defence from badly formatted requests */
        
        if(!batchP->CommandLine && NumNotValid > 10) {
            printf("**** ERROR: Too many invalid Gis/Accessions, "
                   "parsing aborted\n");
            *TotalItems = 0;
            return NULL;
        }
        
        /* Rolling spaces */
        
        j= 0;
        while (!isspace(file[i]) && j < 10  && i < FileLen) { 
            TmpBuff[j] = file[i];
            j++; i++;
            if(file[i] == ',')  /* Comma is valid delimiter */
                break;
        }
        TmpBuff[j] = NULLB;
        

        /* Ignore strings like ">Protein" */
        
        if(j > 0 && TmpBuff[0] == '>' && IS_ALPHA(TmpBuff[1]))
            continue;

        /* Is gi/accession too long ??? */
        
        if(j == 10) {
            printf("**** WARNING: Gi/Accession \"%s\" is too long\r\n", 
                   TmpBuff);
            NumNotValid++;
            
            while(!isspace(file[i]) || 
                  file[i] == ',' || 
                  file[i] == NULLB) /* Rolling until spaces */
                i++;
            continue;  /* Next may be valid ... who knows...?? */   
        }
        
        /* Now validating accession/gi */
        
        for(k =0; k < j; k++) {
            if(!isdigit(TmpBuff[k])) {
                break;
            }
        }
        if(k != j) { 
            if(!IS_ntdb_accession(TmpBuff) && !IS_protdb_accession(TmpBuff)) {
                printf("**** WARNING: Gi/Accession \"%s\" is not valid\r\n", 
                       TmpBuff);
                NumNotValid++;
                continue;
            }
        } 
            
        /* If this is valid Accession check and tranfer it to gi */
            
        giptr = NULL;
        if((NumGis = AccessionToGi(TmpBuff, &giptr, seqtype)) == 0) {
            printf("**** WARNING: Gi/Accession %s is not found "
                   "in database----\r\n", 
                   TmpBuff);
            NumNotValid++;
            continue;
        } else {
            for (j = 0; j < NumGis; j++) {
                /* It we come here - we got valid text ID */
                
                if(AccList == NULL) { /* first element */
                    AccList = (BatchAccListPtr) MemNew(sizeof(BatchAccList));
                    AccListTmp = AccList;
                    AccListTmp->acc = StringSave(TmpBuff);
                    AccListTmp->gi = giptr[j];
                    AccListTmp->next = NULL;
                    AccListLast=AccListTmp;
                    *TotalItems = *TotalItems +1; 
                } else {
                    AccListTmp = (BatchAccListPtr) 
                        MemNew(sizeof(BatchAccList));
                    AccListLast->next = AccListTmp;
                    AccListTmp->acc = StringSave(TmpBuff);
                    AccListTmp->gi = giptr[j];
                    AccListTmp->next = NULL;
                    AccListLast = AccListTmp;
                    *TotalItems = *TotalItems +1;
                }
            }
            MemFree(giptr);
        }
    }
    if(NumNotValid) {
        printf("**** %d invalid Gi%s/Accession%s present in Entrez-batch "
               "request\r\n", 
               (int)NumNotValid,
               NumNotValid == 1 ? "" : "s",
               NumNotValid == 1 ? "" : "s"
               );
    }
    return AccList;
}
void BatchHead(VoidPtr pointer, FILE *fd)
{
  return;
}
void BatchTail(VoidPtr pointer, FILE *fd)
{
  return;
}

